/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: als para table sensortek stk3638 source file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#include "als_para_table_sensortek_stk3638.h"

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

stk3638_als_para_table stk3638_als_para[] = {
	{ TAURUS, V3, DEFAULT_TPLCD, BLACK,
	  { 1000, 1800, 227, 1595, 243, 263, 1400, 1200, 4400, 112, 2100, 166,
	    3000, 138, 25, 968, 570, 70, 16, 663, 3672, 32, 4094, 19400, 7712,
	    9911, 6851, 10214, 4000, 250 }
	},
	{ TAURUS, V3, DEFAULT_TPLCD, GRAY,
	  { 1000, 1800, 227, 1595, 243, 263, 1400, 1200, 4400, 112, 2100, 166,
	    3000, 138, 25, 968, 570, 70, 16, 663, 3672, 32, 4094, 19400, 7712,
	    9911, 6851, 10214, 4000, 250 }
	},
	{ TAURUS, V3, DEFAULT_TPLCD, BLACK2,
	  { 1000, 1800, 227, 1595, 243, 263, 1400, 1200, 4400, 112, 2100, 166,
	    3000, 138, 25, 968, 570, 70, 16, 663, 3672, 32, 4094, 19400, 7712,
	    9911, 6851, 10214, 4000, 250 }
	},
	{ TAURUS, V3, DEFAULT_TPLCD, GOLD,
	  { 1000, 1800, 227, 1595, 243, 263, 1400, 1200, 4400, 112, 2100, 166,
	    3000, 138, 25, 968, 570, 70, 16, 663, 3672, 32, 4094, 19400, 7712,
	    9911, 6851, 10214, 4000, 250 }
	},
	{ TAURUS, V3, DEFAULT_TPLCD, WHITE,
	  { 1000, 1800, 227, 1595, 243, 263, 1400, 1200, 4400, 112, 2100, 166,
	    3000, 138, 25, 968, 570, 70, 16, 663, 3672, 32, 4094, 19400, 7712,
	    9911, 6851, 10214, 4000, 250 }
	},
	{ TAURUS, V4, DEFAULT_TPLCD, BLACK,
	  { 1000, 1800, 227, 1595, 243, 263, 1400, 1200, 4400, 112, 2100, 166,
	    3000, 138, 25, 968, 570, 70, 16, 663, 3672, 32, 4094, 19400, 8150,
	    10537, 7462, 11225, 4000, 250 }
	},
	{ TAURUS, V4, DEFAULT_TPLCD, GRAY,
	  { 1000, 1800, 227, 1595, 243, 263, 1400, 1200, 4400, 112, 2100, 166,
	    3000, 138, 25, 968, 570, 70, 16, 663, 3672, 32, 4094, 19400, 8150,
	    10537, 7462, 11225, 4000, 250 }
	},
	{ TAURUS, V4, DEFAULT_TPLCD, BLACK2,
	  { 1000, 1800, 227, 1595, 243, 263, 1400, 1200, 4400, 112, 2100, 166,
	    3000, 138, 25, 968, 570, 70, 16, 663, 3672, 32, 4094, 19400, 8150,
	    10537, 7462, 11225, 4000, 250 }
	},
	{ TAURUS, V4, DEFAULT_TPLCD, GOLD,
	  { 1000, 1800, 227, 1595, 243, 263, 1400, 1200, 4400, 112, 2100, 166,
	    3000, 138, 25, 968, 570, 70, 16, 663, 3672, 32, 4094, 19400, 8150,
	    10537, 7462, 11225, 4000, 250 }
	},
	{ TAURUS, V4, DEFAULT_TPLCD, WHITE,
	  { 1000, 1800, 227, 1595, 243, 263, 1400, 1200, 4400, 112, 2100, 166,
	    3000, 138, 25, 968, 570, 70, 16, 663, 3672, 32, 4094, 19400, 8150,
	    10537, 7462, 11225, 4000, 250 }
	},
	{ TAURUS, VN1, DEFAULT_TPLCD, BLACK,
	  { 1000, 1800, 227, 1595, 243, 263, 1400, 1200, 4400, 112, 2100, 166,
	    3000, 138, 25, 968, 570, 70, 16, 663, 3672, 32, 4094, 19400, 8150,
	    10537, 7462, 11225, 4000, 250 }
	},
	{ TAURUS, VN1, DEFAULT_TPLCD, GRAY,
	  { 1000, 1800, 227, 1595, 243, 263, 1400, 1200, 4400, 112, 2100, 166,
	    3000, 138, 25, 968, 570, 70, 16, 663, 3672, 32, 4094, 19400, 8150,
	    10537, 7462, 11225, 4000, 250 }
	},
	{ TAURUS, VN1, DEFAULT_TPLCD, BLACK2,
	  { 1000, 1800, 227, 1595, 243, 263, 1400, 1200, 4400, 112, 2100, 166,
	    3000, 138, 25, 968, 570, 70, 16, 663, 3672, 32, 4094, 19400, 8150,
	    10537, 7462, 11225, 4000, 250 }
	},
	{ TAURUS, VN1, DEFAULT_TPLCD, GOLD,
	  { 1000, 1800, 227, 1595, 243, 263, 1400, 1200, 4400, 112, 2100, 166,
	    3000, 138, 25, 968, 570, 70, 16, 663, 3672, 32, 4094, 19400, 8150,
	    10537, 7462, 11225, 4000, 250 }
	},
	{ TAURUS, VN1, DEFAULT_TPLCD, WHITE,
	  { 1000, 1800, 227, 1595, 243, 263, 1400, 1200, 4400, 112, 2100, 166,
	    3000, 138, 25, 968, 570, 70, 16, 663, 3672, 32, 4094, 19400, 8150,
	    10537, 7462, 11225, 4000, 250 }
	},

	{ TETON, V3, DEFAULT_TPLCD, BLACK,
	  { 1000, 1800, 143, 1595, 184, 222, 1400, 1200, 4400, 143, 2100, 166,
	    3000, 138, 5300, -8, 2582, 75, 4, 0, 3347, 52, 6815, 3202, 22712,
	    17643, 10493, 4828, 4000, 250 }
	},
	{ TETON, V3, DEFAULT_TPLCD, GRAY,
	  { 1000, 1800, 143, 1595, 184, 222, 1400, 1200, 4400, 143, 2100, 166,
	    3000, 138, 5300, -8, 2582, 75, 4, 0, 3347, 52, 6815, 3202, 22712,
	    17643, 10493, 4828, 4000, 250 }
	},
	{ TETON, V3, DEFAULT_TPLCD, BLACK2,
	   { 1000, 1800, 143, 1595, 184, 222, 1400, 1200, 4400, 143, 2100, 166,
	    3000, 138, 5300, -8, 2582, 75, 4, 0, 3347, 52, 6815, 3202, 22712,
	    17643, 10493, 4828, 4000, 250 }
	},
	{ TETON, V3, DEFAULT_TPLCD, GOLD,
	   { 1000, 1800, 143, 1595, 184, 222, 1400, 1200, 4400, 143, 2100, 166,
	    3000, 138, 5300, -8, 2582, 75, 4, 0, 3347, 52, 6815, 3202, 22712,
	    17643, 10493, 4828, 4000, 250 }
	},
	{ TETON, V3, DEFAULT_TPLCD, WHITE,
	   { 1000, 1800, 143, 1595, 184, 222, 1400, 1200, 4400, 143, 2100, 166,
	    3000, 138, 5300, -8, 2582, 75, 4, 0, 3347, 52, 6815, 3202, 22712,
	    17643, 10493, 4828, 4000, 250 }
	},

	{ LION, V3, DEFAULT_TPLCD, BLACK,
	  { 1000, 1800, 221, 1590, 236, 261, 1400, 1250, 4400, 93, 2100, 150,
	    3000, 121, 25, 968, 570, 63, 16, 663, 3672, 32, 4201, 19450, 6858,
	    9278, 6296, 9339, 4000, 250 }
	},
	{ LION, V3, DEFAULT_TPLCD, GRAY,
	  { 1000, 1800, 221, 1590, 236, 261, 1400, 1250, 4400, 93, 2100, 150,
	    3000, 121, 25, 968, 570, 63, 16, 663, 3672, 32, 4201, 19450, 6858,
	    9278, 6296, 9339, 4000, 250 }
	},
	{ LION, V3, DEFAULT_TPLCD, BLACK2,
	  { 1000, 1800, 221, 1590, 236, 261, 1400, 1250, 4400, 93, 2100, 150,
	    3000, 121, 25, 968, 570, 63, 16, 663, 3672, 32, 4201, 19450, 6858,
	    9278, 6296, 9339, 4000, 250 }
	},
	{ LION, V3, DEFAULT_TPLCD, GOLD,
	  { 1000, 1800, 221, 1590, 236, 261, 1400, 1250, 4400, 93, 2100, 150,
	    3000, 121, 25, 968, 570, 63, 16, 663, 3672, 32, 4201, 19450, 6858,
	    9278, 6296, 9339, 4000, 250 }
	},
	{ LION, V3, DEFAULT_TPLCD, WHITE,
	  { 1000, 1800, 221, 1590, 236, 261, 1400, 1250, 4400, 93, 2100, 150,
	    3000, 121, 25, 968, 570, 63, 16, 663, 3672, 32, 4201, 19450, 6858,
	    9278, 6296, 9339, 4000, 250 }
	},
	{ LION, V4, DEFAULT_TPLCD, BLACK,
	  { 1000, 1800, 221, 1590, 236, 261, 1400, 1250, 4400, 93, 2100, 150,
	    3000, 121, 25, 968, 570, 63, 16, 663, 3672, 32, 4201, 19450, 9148,
	    11646, 8705, 12563, 4000, 250}
	},
	{ LION, V4, DEFAULT_TPLCD, GRAY,
	  { 1000, 1800, 221, 1590, 236, 261, 1400, 1250, 4400, 93, 2100, 150,
	    3000, 121, 25, 968, 570, 63, 16, 663, 3672, 32, 4201, 19450, 9148,
	    11646, 8705, 12563, 4000, 250}
	},
	{ LION, V4, DEFAULT_TPLCD, BLACK2,
	  { 1000, 1800, 221, 1590, 236, 261, 1400, 1250, 4400, 93, 2100, 150,
	    3000, 121, 25, 968, 570, 63, 16, 663, 3672, 32, 4201, 19450, 9148,
	    11646, 8705, 12563, 4000, 250}
	},
	{ LION, V4, DEFAULT_TPLCD, GOLD,
	  { 1000, 1800, 221, 1590, 236, 261, 1400, 1250, 4400, 93, 2100, 150,
	    3000, 121, 25, 968, 570, 63, 16, 663, 3672, 32, 4201, 19450, 9148,
	    11646, 8705, 12563, 4000, 250}
	},
	{ LION, V4, DEFAULT_TPLCD, WHITE,
	  { 1000, 1800, 221, 1590, 236, 261, 1400, 1250, 4400, 93, 2100, 150,
	    3000, 121, 25, 968, 570, 63, 16, 663, 3672, 32, 4201, 19450, 9148,
	    11646, 8705, 12563, 4000, 250}
	},
	{ LION, VN1, DEFAULT_TPLCD, BLACK,
	  { 1000, 1800, 221, 1590, 236, 261, 1400, 1250, 4400, 93, 2100, 150,
	    3000, 121, 25, 968, 570, 63, 16, 663, 3672, 32, 4201, 19450, 9148,
	    11646, 8705, 12563, 4000, 250}
	},
	{ LION, VN1, DEFAULT_TPLCD, GRAY,
	  { 1000, 1800, 221, 1590, 236, 261, 1400, 1250, 4400, 93, 2100, 150,
	    3000, 121, 25, 968, 570, 63, 16, 663, 3672, 32, 4201, 19450, 9148,
	    11646, 8705, 12563, 4000, 250}
	},
	{ LION, VN1, DEFAULT_TPLCD, BLACK2,
	  { 1000, 1800, 221, 1590, 236, 261, 1400, 1250, 4400, 93, 2100, 150,
	    3000, 121, 25, 968, 570, 63, 16, 663, 3672, 32, 4201, 19450, 9148,
	    11646, 8705, 12563, 4000, 250}
	},
	{ LION, VN1, DEFAULT_TPLCD, GOLD,
	  { 1000, 1800, 221, 1590, 236, 261, 1400, 1250, 4400, 93, 2100, 150,
	    3000, 121, 25, 968, 570, 63, 16, 663, 3672, 32, 4201, 19450, 9148,
	    11646, 8705, 12563, 4000, 250}
	},
	{ LION, VN1, DEFAULT_TPLCD, WHITE,
	  { 1000, 1800, 221, 1590, 236, 261, 1400, 1250, 4400, 93, 2100, 150,
	    3000, 121, 25, 968, 570, 63, 16, 663, 3672, 32, 4201, 19450, 9148,
	    11646, 8705, 12563, 4000, 250}
	},
	{ ELSA, V3, DEFAULT_TPLCD, BLACK,
	  { 1000, 4600, 181, 3600, 229, 284, 0, 0, 4300, 455, 2200, 181,
	    3200, 420, 35, 737, 570, 50, 17, 663, 4152, 34, 4201, 18312,
	    8056, 10599, 7537, 11325, 4000, 250 }
	},
	{ ELSA, V3, DEFAULT_TPLCD, GRAY,
	  { 1000, 4600, 181, 3600, 229, 284, 0, 0, 4300, 455, 2200, 181,
	    3200, 420, 35, 737, 570, 50, 17, 663, 4152, 34, 4201, 18312,
	    8056, 10599, 7537, 11325, 4000, 250 }
	},
	{ ELSA, V3, DEFAULT_TPLCD, BLACK2,
	  { 1000, 4600, 181, 3600, 229, 284, 0, 0, 4300, 455, 2200, 181,
	    3200, 420, 35, 737, 570, 50, 17, 663, 4152, 34, 4201, 18312,
	    8056, 10599, 7537, 11325, 4000, 250 }
	},
	{ ELSA, V3, DEFAULT_TPLCD, GOLD,
	  { 1000, 4600, 181, 3600, 229, 284, 0, 0, 4300, 455, 2200, 181,
	    3200, 420, 35, 737, 570, 50, 17, 663, 4152, 34, 4201, 18312,
	    8056, 10599, 7537, 11325, 4000, 250 }
	},
	{ ELSA, V3, DEFAULT_TPLCD, WHITE,
	  { 1000, 4600, 181, 3600, 229, 284, 0, 0, 4300, 455, 2200, 181,
	    3200, 420, 35, 737, 570, 50, 17, 663, 4152, 34, 4201, 18312,
	    8056, 10599, 7537, 11325, 4000, 250 }
	},
	{ ELSAP, V3, DEFAULT_TPLCD, BLACK,
	  { 1000, 4600, 181, 3600, 228, 284, 0, 0, 4300, 87, 2200, 181,
	    3200, 420, 35, 737, 570, 50, 17, 663, 4152, 34, 4201, 18312,
	    8056, 10599, 7537, 11325, 4000, 250 }
	},
	{ ELSAP, V3, DEFAULT_TPLCD, GRAY,
	  { 1000, 4600, 181, 3600, 228, 284, 0, 0, 4300, 87, 2200, 181,
	    3200, 420, 35, 737, 570, 50, 17, 663, 4152, 34, 4201, 18312,
	    8056, 10599, 7537, 11325, 4000, 250 }
	},
	{ ELSAP, V3, DEFAULT_TPLCD, BLACK2,
	  { 1000, 4600, 181, 3600, 228, 284, 0, 0, 4300, 87, 2200, 181,
	    3200, 420, 35, 737, 570, 50, 17, 663, 4152, 34, 4201, 18312,
	    8056, 10599, 7537, 11325, 4000, 250 }
	},
	{ ELSAP, V3, DEFAULT_TPLCD, GOLD,
	  { 1000, 4600, 181, 3600, 228, 284, 0, 0, 4300, 87, 2200, 181,
	    3200, 420, 35, 737, 570, 50, 17, 663, 4152, 34, 4201, 18312,
	    8056, 10599, 7537, 11325, 4000, 250 }
	},
	{ ELSAP, V3, DEFAULT_TPLCD, WHITE,
	  { 1000, 4600, 181, 3600, 228, 284, 0, 0, 4300, 87, 2200, 181,
	    3200, 420, 35, 737, 570, 50, 17, 663, 4152, 34, 4201, 18312,
	    8056, 10599, 7537, 11325, 4000, 250 }
	},
};

stk3638_als_para_table *als_get_stk3638_table_by_id(uint32_t id)
{
	if (id >= ARRAY_SIZE(stk3638_als_para))
		return NULL;
	return &(stk3638_als_para[id]);
}

stk3638_als_para_table *als_get_stk3638_first_table(void)
{
	return &(stk3638_als_para[0]);
}

uint32_t als_get_stk3638_table_count(void)
{
	return ARRAY_SIZE(stk3638_als_para);
}
