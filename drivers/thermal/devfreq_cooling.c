/*
 * devfreq_cooling: Thermal cooling device implementation for devices using
 *                  devfreq
 *
 * Copyright (C) 2014-2015 ARM Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * TODO:
 *    - If OPPs are added or removed after devfreq cooling has
 *      registered, the devfreq cooling won't react to it.
 */

#include <linux/devfreq.h>
#include <linux/devfreq_cooling.h>
#include <linux/export.h>
#include <linux/idr.h>
#include <linux/slab.h>
#include <linux/pm_opp.h>
#include <linux/thermal.h>

#include <trace/events/thermal.h>

#ifdef CONFIG_DRG
#include <linux/drg.h>
#endif
#ifdef CONFIG_ITS_IPA
#include <linux/lpm_thermal.h>
#include <linux/hisi/dpm_hwmon_user.h>
#endif
#ifdef CONFIG_IPA_THERMAL
#include <trace/events/thermal_power_allocator.h>
extern unsigned int g_ipa_freq_limit[];
extern unsigned int g_ipa_soc_freq_limit[];
extern unsigned int g_ipa_board_freq_limit[];
extern unsigned int g_ipa_board_state[];
extern unsigned int g_ipa_soc_state[];
extern int update_devfreq(struct devfreq *devfreq);
#endif
#define SCALE_ERROR_MITIGATION 100

static DEFINE_IDA(devfreq_ida);

/**
 * struct devfreq_cooling_device - Devfreq cooling device
 * @id:		unique integer value corresponding to each
 *		devfreq_cooling_device registered.
 * @cdev:	Pointer to associated thermal cooling device.
 * @devfreq:	Pointer to associated devfreq device.
 * @cooling_state:	Current cooling state.
 * @power_table:	Pointer to table with maximum power draw for each
 *			cooling state. State is the index into the table, and
 *			the power is in mW.
 * @freq_table:	Pointer to a table with the frequencies sorted in descending
 *		order.  You can index the table by cooling device state
 * @freq_table_size:	Size of the @freq_table and @power_table
 * @power_ops:	Pointer to devfreq_cooling_power, used to generate the
 *		@power_table.
 * @res_util:	Resource utilization scaling factor for the power.
 *		It is multiplied by 100 to minimize the error. It is used
 *		for estimation of the power budget instead of using
 *		'utilization' (which is	'busy_time / 'total_time').
 *		The 'res_util' range is from 100 to (power_table[state] * 100)
 *		for the corresponding 'state'.
 */
struct devfreq_cooling_device {
	int id;
	struct thermal_cooling_device *cdev;
	struct devfreq *devfreq;
	unsigned long cooling_state;
	u32 *power_table;
	u32 *freq_table;
	size_t freq_table_size;
	struct devfreq_cooling_power *power_ops;
	u32 res_util;
	int capped_state;
#ifdef CONFIG_ITS_IPA
	unsigned long normalized_powerdata;
#endif
};

#ifndef CONFIG_IPA_THERMAL
/**
 * partition_enable_opps() - disable all opps above a given state
 * @dfc:	Pointer to devfreq we are operating on
 * @cdev_state:	cooling device state we're setting
 *
 * Go through the OPPs of the device, enabling all OPPs until
 * @cdev_state and disabling those frequencies above it.
 */
static int partition_enable_opps(struct devfreq_cooling_device *dfc,
				 unsigned long cdev_state)
{
	int i;
	struct device *dev = dfc->devfreq->dev.parent;

	for (i = 0; i < dfc->freq_table_size; i++) {/*lint !e574*/
		struct dev_pm_opp *opp;
		int ret = 0;
		unsigned int freq = dfc->freq_table[i];
		bool want_enable = i >= cdev_state ? true : false;/*lint !e574*/

		opp = dev_pm_opp_find_freq_exact(dev, freq, !want_enable);

		if (PTR_ERR(opp) == -ERANGE)
			continue;
		else if (IS_ERR(opp))
			return PTR_ERR(opp);

		dev_pm_opp_put(opp);

		if (want_enable)
			ret = dev_pm_opp_enable(dev, freq);
		else
			ret = dev_pm_opp_disable(dev, freq);

		if (ret)
			return ret;
	}

	return 0;
}
#endif

static int devfreq_cooling_get_max_state(struct thermal_cooling_device *cdev,
					 unsigned long *state)
{
	struct devfreq_cooling_device *dfc = cdev->devdata;

	*state = dfc->freq_table_size - 1;

	return 0;
}

static int devfreq_cooling_get_cur_state(struct thermal_cooling_device *cdev,
					 unsigned long *state)
{
	struct devfreq_cooling_device *dfc = cdev->devdata;

	*state = dfc->cooling_state;

	return 0;
}

static int devfreq_cooling_set_cur_state(struct thermal_cooling_device *cdev,
					 unsigned long state)
{
	struct devfreq_cooling_device *dfc = cdev->devdata;
	struct devfreq *df = dfc->devfreq;
	struct device *dev = df->dev.parent;
#ifdef CONFIG_IPA_THERMAL
	unsigned long freq;
	unsigned long limit_state;
	int gpu_id = -1;
#endif
	int ret;

#ifdef CONFIG_IPA_THERMAL
	gpu_id = ipa_get_actor_id("gpu");
	if (gpu_id < 0)
		return -EINVAL;
	if (g_ipa_soc_state[gpu_id] < dfc->freq_table_size)
		g_ipa_soc_freq_limit[gpu_id] = dfc->freq_table[g_ipa_soc_state[gpu_id]];

	if (g_ipa_board_state[gpu_id] < dfc->freq_table_size)
		g_ipa_board_freq_limit[gpu_id] = dfc->freq_table[g_ipa_board_state[gpu_id]];

	limit_state = max(g_ipa_soc_state[gpu_id], g_ipa_board_state[gpu_id]);/*lint !e1058*/
	if (limit_state < dfc->freq_table_size)
		state = max(state, limit_state);
#endif

	if (state == dfc->cooling_state)
		return 0;

	dev_dbg(dev, "Setting cooling state %lu\n", state);

#ifdef CONFIG_DRG
	if (state < dfc->freq_table_size)
		drg_devfreq_cooling_update(df, dfc->freq_table[state]);
#endif

#ifdef CONFIG_IPA_THERMAL
	if (state == THERMAL_NO_LIMIT) {
		freq = 0;
	} else {
		if (state >= dfc->freq_table_size)
			return -EINVAL;

		freq = dfc->freq_table[state];
	}

	g_ipa_freq_limit[gpu_id] = freq;
	trace_IPA_actor_gpu_cooling(freq/1000, state);

	if (df->max_freq != freq) {
		/*NOTE use devfreq_qos_set_max,because gpufreq not support VOTE */
		mutex_lock(&df->lock);
		ret = update_devfreq(df);
		mutex_unlock(&df->lock);
		if (ret)
			dev_dbg(dev, "update devfreq fail %d\n", ret);
	}
#else
	if (state >= dfc->freq_table_size)
		return -EINVAL;

	ret = partition_enable_opps(dfc, state);
	if (ret)
		return ret;
#endif

	dfc->cooling_state = state;

	return 0;
}

/**
 * freq_get_state() - get the cooling state corresponding to a frequency
 * @dfc:	Pointer to devfreq cooling device
 * @freq:	frequency in Hz
 *
 * Return: the cooling state associated with the @freq, or
 * THERMAL_CSTATE_INVALID if it wasn't found.
 */
static unsigned long
freq_get_state(struct devfreq_cooling_device *dfc, unsigned long freq)
{
	int i;

	for (i = 0; i < dfc->freq_table_size; i++) {/*lint !e574*/
		if (dfc->freq_table[i] == freq)
			return i; /* [false alarm]:fortify */
	}

	return THERMAL_CSTATE_INVALID;/*lint !e501*/
}

static unsigned long get_voltage(struct devfreq *df, unsigned long freq)
{
	struct device *dev = df->dev.parent;
	unsigned long voltage;
	struct dev_pm_opp *opp;

	opp = dev_pm_opp_find_freq_exact(dev, freq, true);
	if (PTR_ERR(opp) == -ERANGE)
		opp = dev_pm_opp_find_freq_exact(dev, freq, false);

	if (IS_ERR(opp)) {
		dev_err_ratelimited(dev, "Failed to find OPP for frequency %lu: %ld\n",
				    freq, PTR_ERR(opp));
		return 0;
	}

	voltage = dev_pm_opp_get_voltage(opp) / 1000; /* mV */
	dev_pm_opp_put(opp);

	if (voltage == 0) {
		dev_err_ratelimited(dev,
				    "Failed to get voltage for frequency %lu\n",
				    freq);
	}

	return voltage;
}

/**
 * get_static_power() - calculate the static power
 * @dfc:	Pointer to devfreq cooling device
 * @freq:	Frequency in Hz
 *
 * Calculate the static power in milliwatts using the supplied
 * get_static_power().  The current voltage is calculated using the
 * OPP library.  If no get_static_power() was supplied, assume the
 * static power is negligible.
 */
static unsigned long
get_static_power(struct devfreq_cooling_device *dfc, unsigned long freq)
{
	struct devfreq *df = dfc->devfreq;
	unsigned long voltage;

	if (!dfc->power_ops->get_static_power)
		return 0;

	voltage = get_voltage(df, freq);

	if (voltage == 0)
		return 0;

	return dfc->power_ops->get_static_power(df, voltage);
}

/**
 * get_dynamic_power - calculate the dynamic power
 * @dfc:	Pointer to devfreq cooling device
 * @freq:	Frequency in Hz
 * @voltage:	Voltage in millivolts
 *
 * Calculate the dynamic power in milliwatts consumed by the device at
 * frequency @freq and voltage @voltage.  If the get_dynamic_power()
 * was supplied as part of the devfreq_cooling_power struct, then that
 * function is used.  Otherwise, a simple power model (Pdyn = Coeff *
 * Voltage^2 * Frequency) is used.
 */
static unsigned long
get_dynamic_power(struct devfreq_cooling_device *dfc, unsigned long freq,
		  unsigned long voltage)
{
	u64 power;
	u32 freq_mhz;
	struct devfreq_cooling_power *dfc_power = dfc->power_ops;

	if (dfc_power->get_dynamic_power)
		return dfc_power->get_dynamic_power(dfc->devfreq, freq,
						    voltage);

	freq_mhz = freq / 1000000;
	power = (u64)dfc_power->dyn_power_coeff * freq_mhz * voltage * voltage;
	do_div(power, 1000000000);

	return power;
}


static inline unsigned long get_total_power(struct devfreq_cooling_device *dfc,
					    unsigned long freq,
					    unsigned long voltage)
{
	return get_static_power(dfc, freq) + get_dynamic_power(dfc, freq,
							       voltage);
}

#ifdef CONFIG_ITS_IPA
unsigned long normalize_devfreq_power(unsigned long voltage,
		unsigned int dynamic_power, unsigned long freq)
{
	unsigned long normalized_data = 0;
	unsigned long freq_mhz;

	/* freq is hz, div 1000000 for mHz */
	freq_mhz = freq / HZ_TO_MHZ_DIVISOR;

	if (freq != 0 && voltage != 0)
		normalized_data = ITS_NORMALIZED_RATIO * dynamic_power /
				  (voltage * voltage * freq_mhz);

	trace_IPA_actor_gpu_normalize_power(normalized_data, dynamic_power,
					    freq_mhz, voltage);
	return normalized_data;
}

int devfreq_normalized_data_to_freq(struct devfreq_cooling_device *devfreq_cdev,
				    unsigned long power)
{
	struct devfreq *df = devfreq_cdev->devfreq;
	unsigned long state, freq, voltage, freq_mhz;
	unsigned long long calc_power = 0;

	for (state = 0; state < devfreq_cdev->freq_table_size - 1; state++) {
		freq = devfreq_cdev->freq_table[state];
		freq_mhz = freq / HZ_TO_MHZ_DIVISOR;
		voltage = get_voltage(df, freq);
		if (freq != 0)
			calc_power = ((unsigned long long)
				      devfreq_cdev->normalized_powerdata *
				      freq_mhz * voltage * voltage) /
				      ITS_NORMALIZED_RATIO;

		trace_IPA_actor_gpu_pdata_to_freq(state, freq_mhz, voltage,
				calc_power, devfreq_cdev->power_table[state],
				devfreq_cdev->normalized_powerdata);
		if ((unsigned long long)power >= calc_power ||
		    power >= devfreq_cdev->power_table[state])
			break;
	}
	return state;
};
#endif

static int devfreq_cooling_get_requested_power(struct thermal_cooling_device *cdev,
					       struct thermal_zone_device *tz,
					       u32 *power)
{
	struct devfreq_cooling_device *dfc = cdev->devdata;
	struct devfreq *df = dfc->devfreq;
	struct devfreq_dev_status *status = &df->last_status;
	unsigned long state;
	unsigned long freq = status->current_frequency;
	unsigned long voltage;
	u32 dyn_power = 0;
	u32 static_power = 0;
	int res;
#ifdef CONFIG_IPA_THERMAL
	unsigned long load = 0;
#endif
#ifdef CONFIG_ITS_IPA
	unsigned long long temp_power = 0;
#endif
	state = freq_get_state(dfc, freq);
	if (state == THERMAL_CSTATE_INVALID) {/*lint !e501*/
		res = -EAGAIN;
		goto fail;
	}

	if (dfc->power_ops->get_real_power) {
		voltage = get_voltage(df, freq);
		if (voltage == 0) {
			res = -EINVAL;
			goto fail;
		}

		res = dfc->power_ops->get_real_power(df, power, freq, voltage);
		if (!res) {
			state = dfc->capped_state;
			dfc->res_util = dfc->power_table[state];
			dfc->res_util *= SCALE_ERROR_MITIGATION;

			if (*power > 1)
				dfc->res_util /= *power;
		} else {
			goto fail;
		}
	} else {
#ifdef CONFIG_ITS_IPA
		/* check device is GPU */
		if (strncmp(cdev->type, "thermal-devfreq-0",
			    strlen("thermal-devfreq-0")) == 0 &&
		    check_its_enabled() &&
		    check_dpm_enabled(DPM_GPU_MODULE)) {
			voltage = get_voltage(df, freq);
			dyn_power = 0;
			static_power = 0;
			res = get_gpu_dynamic_power(&temp_power);
			if (res == 0)
				dyn_power = (unsigned long)temp_power;
			res = get_gpu_leakage_power(&temp_power);
			if (res == 0)
				static_power = (unsigned long)temp_power;

			*power = dyn_power + static_power;
			dfc->normalized_powerdata =
				normalize_devfreq_power(voltage, dyn_power,
							freq);
		} else {
#endif
			dyn_power = dfc->power_table[state];
			/* Scale dynamic power for utilization */
#ifdef CONFIG_IPA_THERMAL
			if (status->total_time) {
#endif
				dyn_power *= status->busy_time;
				dyn_power /= status->total_time;
#ifdef CONFIG_IPA_THERMAL
			}
#endif
		/* Get static power */
		static_power = get_static_power(dfc, freq);

		*power = dyn_power + static_power;
#ifdef CONFIG_ITS_IPA
		}
#endif
	}

	trace_thermal_power_devfreq_get_power(cdev, status, freq, dyn_power,
					      static_power, *power);
#ifdef CONFIG_IPA_THERMAL
	if (status->total_time)
		load = 100 * status->busy_time / status->total_time;
	if (tz->is_soc_thermal)
		trace_IPA_actor_gpu_get_power((freq/1000), load, dyn_power, static_power, *power);
#endif

#ifdef CONFIG_IPA_THERMAL
	cdev->current_load = load;
	cdev->current_freq = freq;
#endif

	return 0;
fail:
	/* It is safe to set max in this case */
	dfc->res_util = SCALE_ERROR_MITIGATION;
	return res;
}

static int devfreq_cooling_state2power(struct thermal_cooling_device *cdev,
				       struct thermal_zone_device *tz,
				       unsigned long state,
				       u32 *power)
{
	struct devfreq_cooling_device *dfc = cdev->devdata;
	unsigned long freq;
	u32 static_power;

	if (state >= dfc->freq_table_size)
		return -EINVAL;

	freq = dfc->freq_table[state];
	static_power = get_static_power(dfc, freq);

	*power = dfc->power_table[state] + static_power;
	return 0;
}

static int devfreq_cooling_power2state(struct thermal_cooling_device *cdev,
				       struct thermal_zone_device *tz,
				       u32 power, unsigned long *state)
{
	struct devfreq_cooling_device *dfc = cdev->devdata;
	struct devfreq *df = dfc->devfreq;
	struct devfreq_dev_status *status = &df->last_status;
	unsigned long freq = status->current_frequency;
	unsigned long busy_time;
	s32 dyn_power = 0;
	u32 static_power;
	s32 est_power = 0;
	int i;
#ifdef CONFIG_ITS_IPA
	bool its_dpm_enabled = false;
	unsigned long long leakage = 0;
	int ret;
#endif

	if (dfc->power_ops->get_real_power) {
		/* Scale for resource utilization */
		est_power = power * dfc->res_util;
		est_power /= SCALE_ERROR_MITIGATION;
	} else {
#ifdef CONFIG_ITS_IPA
		its_dpm_enabled = check_its_enabled() &&
				  check_dpm_enabled(DPM_GPU_MODULE);
		if (strncmp(cdev->type, "thermal-devfreq-0",
			    strlen("thermal-devfreq-0")) == 0 &&
		    its_dpm_enabled) {
			ret = get_gpu_leakage_power(&leakage);
			if (ret != 0)
				static_power = 0;
			else
				static_power = (u32)leakage;

			dyn_power = power - static_power;
			dyn_power = dyn_power > 0 ? dyn_power : 0;
		} else {
#endif
			static_power = get_static_power(dfc, freq);
			dyn_power = power - static_power;
			dyn_power = dyn_power > 0 ? dyn_power : 0;

			/* Scale dynamic power for utilization */
			busy_time = status->busy_time ?: 1;
			est_power = (dyn_power * status->total_time) /
				    busy_time;
#ifdef CONFIG_ITS_IPA
		}
#endif
	}

	/*
	 * Find the first cooling state that is within the power
	 * budget for dynamic power.
	 */
#ifdef CONFIG_ITS_IPA
	if (strncmp(cdev->type, "thermal-devfreq-0",
		    strlen("thermal-devfreq-0")) == 0 &&
	    its_dpm_enabled) {
		i = devfreq_normalized_data_to_freq(dfc, dyn_power);
	} else {
#endif
		for (i = 0; i < dfc->freq_table_size - 1; i++)/*lint !e574*/
			if (est_power >= dfc->power_table[i])/*lint !e574*/
				break;
#ifdef CONFIG_ITS_IPA
	}
#endif

	*state = i;
	dfc->capped_state = i;
	trace_thermal_power_devfreq_limit(cdev, freq, *state, power);
#ifdef CONFIG_IPA_THERMAL
	trace_IPA_actor_gpu_limit(freq / 1000, *state, power);
#endif
	return 0;
}

static struct thermal_cooling_device_ops devfreq_cooling_ops = {
	.get_max_state = devfreq_cooling_get_max_state,
	.get_cur_state = devfreq_cooling_get_cur_state,
	.set_cur_state = devfreq_cooling_set_cur_state,
};

/**
 * devfreq_cooling_gen_tables() - Generate power and freq tables.
 * @dfc: Pointer to devfreq cooling device.
 *
 * Generate power and frequency tables: the power table hold the
 * device's maximum power usage at each cooling state (OPP).  The
 * static and dynamic power using the appropriate voltage and
 * frequency for the state, is acquired from the struct
 * devfreq_cooling_power, and summed to make the maximum power draw.
 *
 * The frequency table holds the frequencies in descending order.
 * That way its indexed by cooling device state.
 *
 * The tables are malloced, and pointers put in dfc.  They must be
 * freed when unregistering the devfreq cooling device.
 *
 * Return: 0 on success, negative error code on failure.
 */
static int devfreq_cooling_gen_tables(struct devfreq_cooling_device *dfc)
{
	struct devfreq *df = dfc->devfreq;
	struct device *dev = df->dev.parent;
	int ret, num_opps;
	unsigned long freq;
	u32 *power_table = NULL;
	u32 *freq_table;
	int i;
#ifdef CONFIG_IPA_THERMAL
	unsigned long power_static;
	unsigned long power_dyn;
#endif

	num_opps = dev_pm_opp_get_opp_count(dev);

	if (dfc->power_ops) {
		power_table = kcalloc(num_opps, sizeof(*power_table),
				      GFP_KERNEL);
		if (!power_table)
			return -ENOMEM;
	}

	freq_table = kcalloc(num_opps, sizeof(*freq_table),
			     GFP_KERNEL);
	if (!freq_table) {
		ret = -ENOMEM;
		goto free_power_table;
	}

	for (i = 0, freq = ULONG_MAX; i < num_opps; i++, freq--) {
		unsigned long power, voltage;
		struct dev_pm_opp *opp;

		opp = dev_pm_opp_find_freq_floor(dev, &freq);
		if (IS_ERR(opp)) {
			ret = PTR_ERR(opp);
			goto free_tables;
		}

		voltage = dev_pm_opp_get_voltage(opp) / 1000; /* mV */
		dev_pm_opp_put(opp);

		if (dfc->power_ops) {
			if (dfc->power_ops->get_real_power)
				power = get_total_power(dfc, freq, voltage);
			else
				power = get_dynamic_power(dfc, freq, voltage);

#ifdef CONFIG_IPA_THERMAL
			power_static = get_static_power(dfc, freq);
			power_dyn = get_dynamic_power(dfc, freq, voltage);
			pr_debug("%lu MHz @ %lu mV: %lu + %lu = %lu mW\n",
					freq / 1000000, voltage,
					power_dyn, power_static, power_dyn + power_static);
#else
			dev_dbg(dev, "Power table: %lu MHz @ %lu mV: %lu = %lu mW\n",
				freq / 1000000, voltage, power, power);
#endif

			power_table[i] = power;/* [false alarm]:fortify *//*lint !e613*/
		}

		freq_table[i] = freq;
	}

	if (dfc->power_ops)
		dfc->power_table = power_table;

	dfc->freq_table = freq_table;
	dfc->freq_table_size = num_opps;

	return 0;/*lint !e593*/

free_tables:
	kfree(freq_table);
free_power_table:
	kfree(power_table);/*lint !e668*/

	return ret;
}

/**
 * of_devfreq_cooling_register_power() - Register devfreq cooling device,
 *                                      with OF and power information.
 * @np:	Pointer to OF device_node.
 * @df:	Pointer to devfreq device.
 * @dfc_power:	Pointer to devfreq_cooling_power.
 *
 * Register a devfreq cooling device.  The available OPPs must be
 * registered on the device.
 *
 * If @dfc_power is provided, the cooling device is registered with the
 * power extensions.  For the power extensions to work correctly,
 * devfreq should use the simple_ondemand governor, other governors
 * are not currently supported.
 */
struct thermal_cooling_device *
of_devfreq_cooling_register_power(struct device_node *np, struct devfreq *df,
				  struct devfreq_cooling_power *dfc_power)
{
	struct thermal_cooling_device *cdev;
	struct devfreq_cooling_device *dfc;
	char dev_name[THERMAL_NAME_LENGTH];/*lint !e578*/
	int err;

	dfc = kzalloc(sizeof(*dfc), GFP_KERNEL);
	if (!dfc)
		return ERR_PTR(-ENOMEM);

	dfc->devfreq = df;

	if (dfc_power) {
		dfc->power_ops = dfc_power;

		devfreq_cooling_ops.get_requested_power =
			devfreq_cooling_get_requested_power;
		devfreq_cooling_ops.state2power = devfreq_cooling_state2power;
		devfreq_cooling_ops.power2state = devfreq_cooling_power2state;
	}

	err = devfreq_cooling_gen_tables(dfc);
	if (err)
		goto free_dfc;

	err = ida_simple_get(&devfreq_ida, 0, 0, GFP_KERNEL);
	if (err < 0)
		goto free_tables;
	dfc->id = err;

	snprintf(dev_name, sizeof(dev_name), "thermal-devfreq-%d", dfc->id);/* unsafe_function_ignore: snprintf */

	cdev = thermal_of_cooling_device_register(np, dev_name, dfc,
						  &devfreq_cooling_ops);
	if (IS_ERR(cdev)) {
		err = PTR_ERR(cdev);
		dev_err(df->dev.parent,
			"Failed to register devfreq cooling device (%d)\n",
			err);
		goto release_ida;
	}

	dfc->cdev = cdev;

	return cdev;

release_ida:
	ida_simple_remove(&devfreq_ida, dfc->id);
free_tables:
	kfree(dfc->power_table);
	kfree(dfc->freq_table);
free_dfc:
	kfree(dfc);

	return ERR_PTR(err);
}
EXPORT_SYMBOL_GPL(of_devfreq_cooling_register_power);

/**
 * of_devfreq_cooling_register() - Register devfreq cooling device,
 *                                with OF information.
 * @np: Pointer to OF device_node.
 * @df: Pointer to devfreq device.
 */
struct thermal_cooling_device *
of_devfreq_cooling_register(struct device_node *np, struct devfreq *df)
{
	return of_devfreq_cooling_register_power(np, df, NULL);
}
EXPORT_SYMBOL_GPL(of_devfreq_cooling_register);

/**
 * devfreq_cooling_register() - Register devfreq cooling device.
 * @df: Pointer to devfreq device.
 */
struct thermal_cooling_device *devfreq_cooling_register(struct devfreq *df)
{
	return of_devfreq_cooling_register(NULL, df);
}
EXPORT_SYMBOL_GPL(devfreq_cooling_register);

/**
 * devfreq_cooling_unregister() - Unregister devfreq cooling device.
 * @dfc: Pointer to devfreq cooling device to unregister.
 */
void devfreq_cooling_unregister(struct thermal_cooling_device *cdev)
{
	struct devfreq_cooling_device *dfc;

	if (!cdev)
		return;

	dfc = cdev->devdata;

	thermal_cooling_device_unregister(dfc->cdev);
	ida_simple_remove(&devfreq_ida, dfc->id);
	kfree(dfc->power_table);
	kfree(dfc->freq_table);

	kfree(dfc);
}
EXPORT_SYMBOL_GPL(devfreq_cooling_unregister);
