/*
 * lcdkit_bias_ic_common.h
 *
 * lcdkit bias head file for lcd driver
 *
 * Copyright (c) 2018-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef _LCDKIT_BIAS_IC_COMMON_H_
#define _LCDKIT_BIAS_IC_COMMON_H_
#include "lcdkit_bias_bl_utility.h"

#define BIAS_IC_READ_INHIBITION	   0x01
#define BIAS_IC_HAVE_E2PROM		   0x02
#define BIAS_IC_CONFIG_NEED_ENABLE	0x04
#define BIAS_IC_RESUME_NEED_CONFIG	0x08

struct lcd_bias_voltage_info
{
	unsigned char   ic_type;
	unsigned char   vpos_reg;
	unsigned char   vneg_reg;
	unsigned char   vpos_val;
	unsigned char   vneg_val;
	unsigned char   vpos_mask;
	unsigned char   vneg_mask;
	unsigned char   state_reg;
	unsigned char   state_val;
	unsigned char   state_mask;
	unsigned char   pull_down_support;
	unsigned char   vpos_down_val;
	unsigned char   vneg_down_val;
};

struct lcdkit_bias_ic_device
{
	struct device *dev;
	struct i2c_client *client;
	struct lcd_bias_voltage_info bias_config;
};

int lcdkit_bias_set_voltage(void);
struct lcd_bias_voltage_info *lcdkit_get_lcd_bias_ic_info(void);
void lcdkit_bias_pull_down_boost(void);
#endif