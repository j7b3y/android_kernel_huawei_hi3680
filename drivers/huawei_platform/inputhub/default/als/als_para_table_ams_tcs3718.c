/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: als para table ams tcs3718 source file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#include "als_para_table_ams_tcs3718.h"

#include <linux/err.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/slab.h>
#include <linux/types.h>

#include <securec.h>

#include "als_tp_color.h"
#include "contexthub_boot.h"
#include "contexthub_route.h"

tcs3701_als_para_table tcs3718_als_para_diff_tp_color_table[] = {
	{ OCEAN, V3, DEFAULT_TPLCD, WHITE,
	  { 50, 1960, -1136, 3870, -2772, 100, 0, 1960, -1136, 3870, -2772, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 19088, 9252,
	    7684, 3156, 4000, 250 }
	},
	{ OCEAN, V3, DEFAULT_TPLCD, GRAY,
	  { 50, 1960, -1136, 3870, -2772, 100, 0, 1960, -1136, 3870, -2772, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 19088, 9252,
	    7684, 3156, 4000, 250 }
	},
	{ OCEAN, V3, DEFAULT_TPLCD, BLACK,
	  { 50, 1960, -1136, 3870, -2772, 100, 0, 1960, -1136, 3870, -2772, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 19088, 9252,
	    7684, 3156, 4000, 250 }
	},
	{ OCEAN, V3, DEFAULT_TPLCD, BLACK2,
	  { 50, 1960, -1136, 3870, -2772, 100, 0, 1960, -1136, 3870, -2772, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 19088, 9252,
	    7684, 3156, 4000, 250 }
	},
	{ OCEAN, V3, DEFAULT_TPLCD, GOLD,
	  { 50, 1960, -1136, 3870, -2772, 100, 0, 1960, -1136, 3870, -2772, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 19088, 9252,
	    7684, 3156, 4000, 250 }
	},

	{ OCEAN, VN1, BOE_RUIDING_TPLCD, BLACK,
	  { 50, -521, 613, 1054, 513, 1000, 0, 1960, -1136, 3870, -2772, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 19088, 9252,
	    7684, 3156, 4000, 250 }
	},
	{ OCEAN, VN1, BOE_RUIDING_TPLCD, WHITE,
	  { 50, -521, 613, 1054, 513, 1000, 0, 1960, -1136, 3870, -2772, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 19088, 9252,
	    7684, 3156, 4000, 250 }
	},
	{ OCEAN, VN1, BOE_RUIDING_TPLCD, GRAY,
	  { 50, -521, 613, 1054, 513, 1000, 0, 1960, -1136, 3870, -2772, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 19088, 9252,
	    7684, 3156, 4000, 250 }
	},
	{ OCEAN, VN1, BOE_RUIDING_TPLCD, GOLD,
	  { 50, -521, 613, 1054, 513, 1000, 0, 1960, -1136, 3870, -2772, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 19088, 9252,
	    7684, 3156, 4000, 250 }
	},
	{ OCEAN, VN1, DEFAULT_TPLCD, WHITE,
	  { 50, -2000, 2120, 2410, 1850, 1000, 0, 1960, -1136, 3870, -2772, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 19088, 9252,
	    7684, 3156, 4000, 250 }
	},
	{ OCEAN, VN1, DEFAULT_TPLCD, GRAY,
	  { 50, -2000, 2120, 2410, 1850, 1000, 0, 1960, -1136, 3870, -2772, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 19088, 9252,
	    7684, 3156, 4000, 250 }
	},
	{ OCEAN, VN1, DEFAULT_TPLCD, BLACK,
	  { 50, -2000, 2120, 2410, 1850, 1000, 0, 1960, -1136, 3870, -2772, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 19088, 9252,
	    7684, 3156, 4000, 250 }
	},
	{ OCEAN, VN1, DEFAULT_TPLCD, BLACK2,
	  { 50, -2000, 2120, 2410, 1850, 1000, 0, 1960, -1136, 3870, -2772, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 19088, 9252,
	    7684, 3156, 4000, 250 }
	},
	{ OCEAN, VN1, DEFAULT_TPLCD, GOLD,
	  { 50, -2000, 2120, 2410, 1850, 1000, 0, 1960, -1136, 3870, -2772, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 19088, 9252,
	    7684, 3156, 4000, 250 }
	},

	{ NOAH, V3, DEFAULT_TPLCD, WHITE,
	  { 50, 2762, -1154, 7577, -4201, 100, 0, 2762, -1154, 7577, -4201, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263, 5584,
	    4890, 2200, 4000, 250 }
	},
	{ NOAH, V3, DEFAULT_TPLCD, GRAY,
	  { 50, 2762, -1154, 7577, -4201, 100, 0, 2762, -1154, 7577, -4201, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263, 5584,
	    4890, 2200, 4000, 250 }
	},
	{ NOAH, V3, DEFAULT_TPLCD, BLACK,
	  { 50, 2762, -1154, 7577, -4201, 100, 0, 2762, -1154, 7577, -4201, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263, 5584,
	    4890, 2200, 4000, 250 }
	},
	{ NOAH, V3, DEFAULT_TPLCD, BLACK2,
	  { 50, 2762, -1154, 7577, -4201, 100, 0, 2762, -1154, 7577, -4201, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263, 5584,
	    4890, 2200, 4000, 250 }
	},
	{ NOAH, V3, DEFAULT_TPLCD, GOLD,
	  { 50, 2762, -1154, 7577, -4201, 100, 0, 2762, -1154, 7577, -4201, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263, 5584,
	    4890, 2200, 4000, 250 }
	},

	{ NOAH, V4, DEFAULT_TPLCD, WHITE,
	  { 50, -42, 142, 870, -199, 1167, 0, -42, 142, 870, -199, 1167,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263, 5584,
	    4890, 2200, 4000, 250 }
	},
	{ NOAH, V4, DEFAULT_TPLCD, GRAY,
	  { 50, -42, 142, 870, -199, 1167, 0, -42, 142, 870, -199, 1167,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263, 5584,
	    4890, 2200, 4000, 250 }
	},
	{ NOAH, V4, DEFAULT_TPLCD, BLACK,
	  { 50, -42, 142, 870, -199, 1167, 0, -42, 142, 870, -199, 1167,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263, 5584,
	    4890, 2200, 4000, 250 }
	},
	{ NOAH, V4, DEFAULT_TPLCD, BLACK2,
	  { 50, -42, 142, 870, -199, 1167, 0, -42, 142, 870, -199, 1167,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263, 5584,
	    4890, 2200, 4000, 250 }
	},
	{ NOAH, V4, DEFAULT_TPLCD, GOLD,
	  { 50, -42, 142, 870, -199, 1167, 0, -42, 142, 870, -199, 1167,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263, 5584,
	    4890, 2200, 4000, 250 }
	},

	{ NOAH, VN1, DEFAULT_TPLCD, WHITE,
	  { 50, 1202, -1046, 791, -1342, 493, 0, -42, 142, 870, -199, 1167,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263, 5584,
	    4890, 2200, 4000, 250 }
	},
	{ NOAH, VN1, DEFAULT_TPLCD, GRAY,
	  { 50, 1202, -1046, 791, -1342, 493, 0, -42, 142, 870, -199, 1167,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263, 5584,
	    4890, 2200, 4000, 250 }
	},
	{ NOAH, VN1, DEFAULT_TPLCD, BLACK,
	  { 50, 1202, -1046, 791, -1342, 493, 0, -42, 142, 870, -199, 1167,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263, 5584,
	    4890, 2200, 4000, 250 }
	},
	{ NOAH, VN1, DEFAULT_TPLCD, BLACK2,
	  { 50, 1202, -1046, 791, -1342, 493, 0, -42, 142, 870, -199, 1167,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263, 5584,
	    4890, 2200, 4000, 250 }
	},
	{ NOAH, VN1, DEFAULT_TPLCD, GOLD,
	  { 50, 1202, -1046, 791, -1342, 493, 0, -42, 142, 870, -199, 1167,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263, 5584,
	    4890, 2200, 4000, 250 }
	},

	{ NOAHP, V3, DEFAULT_TPLCD, WHITE,
	  { 50, 5690, -4716, 2529, -5526, 100, 0, 5690, -4716, 2529, -5526, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263,
	    5584, 4890, 2200, 4000, 250 }
	},
	{ NOAHP, V3, DEFAULT_TPLCD, GRAY,
	  { 50, 5690, -4716, 2529, -5526, 100, 0, 5690, -4716, 2529, -5526, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263,
	    5584, 4890, 2200, 4000, 250 }
	},
	{ NOAHP, V3, DEFAULT_TPLCD, BLACK,
	  { 50, 5690, -4716, 2529, -5526, 100, 0, 5690, -4716, 2529, -5526, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263,
	    5584, 4890, 2200, 4000, 250 }
	},
	{ NOAHP, V3, DEFAULT_TPLCD, BLACK2,
	  { 50, 5690, -4716, 2529, -5526, 100, 0, 5690, -4716, 2529, -5526, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263,
	    5584, 4890, 2200, 4000, 250 }
	},
	{ NOAHP, V3, DEFAULT_TPLCD, GOLD,
	  { 50, 5690, -4716, 2529, -5526, 100, 0, 5690, -4716, 2529, -5526, 100,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263,
	    5584, 4890, 2200, 4000, 250 }
	},

	{ NOAHP, V4, DEFAULT_TPLCD, WHITE,
	  { 50, -718, 879, 1503, 607, 1000, 0, -718, 879, 1503, 607, 1000,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263,
	    5584, 4890, 2200, 4000, 250 }
	},
	{ NOAHP, V4, DEFAULT_TPLCD, GRAY,
	  { 50, -718, 879, 1503, 607, 1000, 0, -718, 879, 1503, 607, 1000,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263,
	    5584, 4890, 2200, 4000, 250 }
	},
	{ NOAHP, V4, DEFAULT_TPLCD, BLACK,
	  { 50, -718, 879, 1503, 607, 1000, 0, -718, 879, 1503, 607, 1000,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263,
	    5584, 4890, 2200, 4000, 250 }
	},
	{ NOAHP, V4, DEFAULT_TPLCD, BLACK2,
	  { 50, -718, 879, 1503, 607, 1000, 0, -718, 879, 1503, 607, 1000,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263,
	    5584, 4890, 2200, 4000, 250 }
	},
	{ NOAHP, V4, DEFAULT_TPLCD, GOLD,
	  { 50, -718, 879, 1503, 607, 1000, 0, -718, 879, 1503, 607, 1000,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263,
	    5584, 4890, 2200, 4000, 250 }
	},

	{ NOAHP, VN1, DEFAULT_TPLCD, WHITE,
	  { 50, 624, -561, 459, -701, 908, 0, -718, 879, 1503, 607, 1000,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263,
	    5584, 4890, 2200, 4000, 250 }
	},
	{ NOAHP, VN1, DEFAULT_TPLCD, GRAY,
	  { 50, 624, -561, 459, -701, 908, 0, -718, 879, 1503, 607, 1000,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263,
	    5584, 4890, 2200, 4000, 250 }
	},
	{ NOAHP, VN1, DEFAULT_TPLCD, BLACK,
	  { 50, 624, -561, 459, -701, 908, 0, -718, 879, 1503, 607, 1000,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263,
	    5584, 4890, 2200, 4000, 250 }
	},
	{ NOAHP, VN1, DEFAULT_TPLCD, BLACK2,
	  { 50, 624, -561, 459, -701, 908, 0, -718, 879, 1503, 607, 1000,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263,
	    5584, 4890, 2200, 4000, 250 }
	},
	{ NOAHP, VN1, DEFAULT_TPLCD, GOLD,
	  { 50, 624, -561, 459, -701, 908, 0, -718, 879, 1503, 607, 1000,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263,
	    5584, 4890, 2200, 4000, 250 }
	},

	{ BRQ, V3, BOE_TPLCD, OTHER,
	  { 50, 375, -249, 1167, -1625, 753, 0, 375, -249, 1167, -1625, 753,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 12263,
	    5584, 4890, 2200, 4000, 250 }
	},

	{ BRQ, V3, DEFAULT_TPLCD, OTHER,
	  { 0 }
	},

	{ JADE, V3, DEFAULT_TPLCD, WHITE,
	  { 50, -367, 554, 1987, -1015, 857, 0, -42, 142, 870, -199, 1167,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 11600, 5650,
	    4292, 2081, 4000, 250 }
	},
	{ JADE, V3, DEFAULT_TPLCD, GRAY,
	  { 50, -367, 554, 1987, -1015, 857, 0, -42, 142, 870, -199, 1167,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 11600, 5650,
	    4292, 2081, 4000, 250 }
	},
	{ JADE, V3, DEFAULT_TPLCD, BLACK,
	  { 50, -367, 554, 1987, -1015, 857, 0, -42, 142, 870, -199, 1167,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 11600, 5650,
	    4292, 2081, 4000, 250 }
	},
	{ JADE, V3, DEFAULT_TPLCD, BLACK2,
	  { 50, -367, 554, 1987, -1015, 857, 0, -42, 142, 870, -199, 1167,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 11600, 5650,
	    4292, 2081, 4000, 250 }
	},
	{ JADE, V3, DEFAULT_TPLCD, GOLD,
	  { 50, -367, 554, 1987, -1015, 857, 0, -42, 142, 870, -199, 1167,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 11600, 5650,
	    4292, 2081, 4000, 250 }
	},

	{ BRQ, VN1, BOE_TPLCD, OTHER,
	  { 50, -290, 179, 2009, -900, 801, 0, 375, -249, 1167, -1625, 753,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 8671,
	    4245, 3514, 1321, 10000, 100 }
	},

	{ BRQ, VN1, VISI_TPLCD, OTHER,
	  { 50, -54, -36, 894, -123, 799, 0, 375, -249, 1167, -1625, 753,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 14225,
	    5734, 6126, 3054, 10000, 100 }
	},
};

tcs3701_als_para_table *als_get_tcs3718_table_by_id(uint32_t id)
{
	if (id >= ARRAY_SIZE(tcs3718_als_para_diff_tp_color_table))
		return NULL;
	return &(tcs3718_als_para_diff_tp_color_table[id]);
}

tcs3701_als_para_table *als_get_tcs3718_first_table(void)
{
	return &(tcs3718_als_para_diff_tp_color_table[0]);
}

uint32_t als_get_tcs3718_table_count(void)
{
	return ARRAY_SIZE(tcs3718_als_para_diff_tp_color_table);
}
