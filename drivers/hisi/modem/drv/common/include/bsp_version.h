/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __BSP_VERSION_H__
#define __BSP_VERSION_H__

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(__OS_VXWORKS__) || defined(__OS_RTOSCK__) || defined(__KERNEL__)) ||defined(__OS_RTOSCK_SMP__) ||defined(__OS_RTOSCK_TVP__) ||defined(__OS_RTOSCK_TSP__)
#include <mdrv_version.h>
#endif
#include "product_config.h"
#include <osl_types.h>
#include <bsp_print.h>
#include <bsp_shared_ddr.h>

#ifndef VER_ERROR
#define VER_ERROR (-1)
#endif

#ifndef VER_OK
#define VER_OK 0
#endif

#define ver_print_error(fmt, ...)   (bsp_err("<%s> "fmt, __FUNCTION__, ##__VA_ARGS__))

#define HW_VER_INVALID				(BSP_U32)0XFFFFFFFF
#define HW_VER_UDP_MASK				(BSP_U32)0XFF000000 	    /*MBB UDP单板掩码*/
#define HW_VER_UDP_UNMASK			(BSP_U32)(~HW_VER_UDP_MASK) /*MBB UDP掩码取反*/
#define MODEMID_VENDOR_MASK			(~(BSP_U32)0x3FF)           /*PHONE UDP单板掩码*/

#define HW_VER_HIONE_UDP_MAGIC      (BSP_U32)0X13245768

#define HW_VER_V711_UDP             (BSP_U32)0X71000000 /*V711 UDP*/
#define HW_VER_V750_UDP             (BSP_U32)0X75000000 /*V7R5 UDP*/
#define HW_VER_V722_UDP             (BSP_U32)0X72000000 /*V722 UDP*/
#define HW_VER_V765_UDP             (BSP_U32)0X76000000 /*V722 UDP*/
#define HW_VER_V5000_UDP            (BSP_U32)0X77000000 /*V5000 UDP*/
#define HW_VER_V5000_EMDM_UDP       (BSP_U32)0X37000000 /*V5000 extra modem UDP*/
#define HW_VER_P532                 (BSP_U32)0XFF000000 /*P532*/
#define HW_VER_P533                 (BSP_U32)0XFE000000 /*P533*/
#define HW_VER_P535                 (BSP_U32)0XFD000000 /*P535*/

#define VERSION_MODULE_MAGIC        (0x2017)

typedef enum
{
	PLAT_ASIC= 0x0,
	PLAT_ESL = 0x1,
	PLAT_FPGA = 0xa,
	PLAT_EMU = 0xe
}VERSION_PLAT_TYPE_E;

typedef enum{
	 BSP_BOARD_TYPE_BBIT    = 0,
	 BSP_BOARD_TYPE_SFT,
	 BSP_BOARD_TYPE_ASIC,
	 BSP_BOARD_TYPE_SOC,
	 BSP_BOARD_TYPE_PORTING,
	 BSP_BOARD_TYPE_ESL,
	 BSP_BOARD_TYPE_MAX
}VERSION_BOARD_TYPE_E;

typedef enum{
	PRODUCT_MBB= 0x0,
	PRODUCT_PHONE = 0x1,
	PRODUCT_ERROR = 0x2
}VERSION_PRODUCT_TYPE_E;

typedef enum
{
	TYPE_ESL = 0x000,
	TYPE_ESL_EMU = 0x010,
	TYPE_ES = 0x100,
	TYPE_CS = 0x110,
	TYPE_ERR = 0xFFF
}VERSION_CSES_TYPE_E;

typedef struct
{
	u32 board_id;				/*硬件版本号，通过hkadc读取。NV和dts在用*/
	u32 board_id_udp_masked;	/*屏蔽扣板信息的硬件版本号。ioshare在用*/
	u32 chip_version;			/*芯片版本号*/
	u16 chip_type;				/*芯片类型，如CHIP_V711=0x6921*/
	u8  plat_type;				/*平台类型，如asic/proting/emu*/
	u8  board_type;				/*平台类型，如BBIT SOC ASIC SFT*/
	u8  bbit_version;				/*bbit 平台，如dalass bbit/722 BBIT/chicago bbit*/
	u8  product_type;			/*MBB or PHONE*/
	u16 product_name;			/*平台名称，如PRODUCT_722，将722 porting/bbit/sft/udp统一归类为722*/
	u16 cses_type;				/*从boston起，芯片第一版逻辑为es(100)，第二版为cs(110)*/
	u16 version_magic;			/*0x2017*/
	u32 udp_flag;               /* udp or phone,if udp = 0x13245768 */
}BSP_VERSION_INFO_S;

#define BOARD_ID_UDP_MASK 0x0000FC00
#define BOARD_ID_MASK 0xFFFFFFF0

enum
{
    BOARDID_MSG_INIT = 0,
    BOARDID_MSG_REQUEST,
    BOARDID_MSG_UPDATE
};

struct boardid_pcie_msg
{
    u32 boardid;
    u32 msg_type;
};

/*****************************************************************************
*                                                                            *
*            以下提供给version_balong.c(a/c)                                 *
*                                                                            *
******************************************************************************/

#if defined(__OS_VXWORKS__) || defined(__OS_RTOSCK__)
#define StrParamType (unsigned int)
#define MemparamType (int)
#else
#define StrParamType (int)
#define MemparamType (unsigned int)
#endif

#define VERSION_MAX_LEN 32



/*****************************************************************************
*                                                                            *
*            以下提供给version.c(fastboot)                                   *
*                                                                            *
******************************************************************************/

typedef struct
{
    u16 vol_low;
    u16 vol_high;
}voltage_range;

#define CHIP_TYPE_MASK 0xffff0000
#define PLAT_TYPE_MASK 0x0000f000
#define CSES_TYPE_MASK 0x00000fff



/*****************************************************************************
*                                                                            *
*            以下提供给adp_version.c(a/c)                                    *
*                                                                            *
******************************************************************************/

#ifndef isdigit
#define isdigit(c)      (((c) >= '0') && ((c) <= '9'))
#endif

#ifndef VER_MAX_LENGTH
#define VER_MAX_LENGTH  30
#endif

typedef struct
{
    unsigned char CompId;                            /* 组件号：参见COMP_TYPE */
    unsigned char CompVer[VER_MAX_LENGTH+1];         /* 最大版本长度 30 字符+ \0 */
}VERSIONINFO;



/*****************************************************************************
*                                                                            *
*            以下提供给virtual boardid功能                                   *
*                                                                            *
******************************************************************************/

#define VIRTUAL_BOARDID_SET_OK      0x12345000
#define VIRTUAL_BOARDID_NO_SET      0x12345001
#define VIRTUAL_BOARDID_CMD_NULL    0x12345002
#define VIRTUAL_BOARDID_ERR_FORMAT  0x12345003
#define VIRTUAL_BOARDID_NV_NOBURN   0x12345004
#define VIRTUAL_BOARDID_SET_FLAG    0x12345005

#define MISC_VERSION_OFFSET 100

typedef struct  {
    unsigned err_code;
    unsigned virtual_boardid;
    unsigned timestamp;
    unsigned set_ok_flag;
} misc_ptn_version_info;

typedef enum {
	VIRTUAL_BOARDID_MISC_OK = 0,
	VIRTUAL_BOARDID_MISC_ERROR,
}virtual_boardid_misc_return_type;



/*****************************************************************************
*                                                                            *
*            以下为对外头文件声明                                            *
*                                                                            *
******************************************************************************/

char * bsp_version_get_hardware(void);
char * bsp_version_get_product_inner_name(void);
char * bsp_version_get_product_out_name(void);
char * bsp_version_get_build_date_time(void);
char * bsp_version_get_chip(void);
char * bsp_version_get_firmware(void);
char * bsp_version_get_release(void);

int bsp_version_acore_init(void);
int bsp_version_ccore_init(void);
int bsp_version_ddr_init(void);
void mdrv_ver_init(void);

bool bsp_b5000_is_esl_emu_version(void);
const BSP_VERSION_INFO_S* bsp_get_version_info(void);
const BSP_VERSION_INFO_S* bsp_get_version_info_early(void);

int bsp_version_debug(void);

int update_version_boardid(void);



#ifdef __cplusplus
}
#endif

#endif

