/*
 * da_combine_regs.h
 *
 * codec regs for da_combine driver
 *
 * Copyright (c) 2015-2019 Huawei Technologies Co., Ltd.
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

#ifndef __DA_COMBINE_REGS_H__
#define __DA_COMBINE_REGS_H__

/* page base addr def begin */
#define CODEC_BASE_ADDR    0x20000000
#define BASE_ADDR_PAGE_CFG 0x7000
#define BASE_ADDR_PAGE_IO  0x1000
#define BASE_ADDR_PAGE_DIG 0x7200
#define BASE_ADDR_PAGE_MM  0xD000

/* page base addr def end */
#define DA_COMBINE_VERSION_CS 0x11
#define DA_COMBINE_VERSION_ES 0x10
#define DA_COMBINE_VERSION_REG  0x20007000
#define DA_COMBINE_CHIP_ID_REG0 0X20007092
#define DA_COMBINE_CHIP_ID_REG1 0x20007093
#define DA_COMBINE_CHIP_ID_REG2 0x20007094
#define DA_COMBINE_CHIP_ID_REG3 0x20007095

#define DA_COMBINE_HS_PLUGIN_BIT        6
#define DA_COMBINE_VAD_BIT              4
#define DA_COMBINE_MBHC_VREF_BIT        7
#define DA_COMBINE_MICBIAS_ECO_ON_BIT   0
#define DA_COMBINE_PLUGIN_IRQ_BIT       3
#define DA_COMBINE_SARADC_CMP_BIT       7
#define DA_COMBINE_SARADC_RD_BIT        2
#define DA_COMBINE_MASK_PLL_UNLOCK_BIT  4
#define DA_COMBINE_PLL_RST_BIT          2
#define DA_COMBINE_SARADC_PD_BIT        6
#define DA_COMBINE_SAR_START_BIT        5
#define DA_COMBINE_MBHC_ON_BIT          4
#define DA_COMBINE_SARADC_RDY_BIT       0
#define PLL_LOCK_VALUE              0x02
/* SARADC */
#define DA_COMBINE_SAR_AUTO_EN_BIT              0
#define DA_COMBINE_MBHC_MASK_PLUGOUT_BIT        7
#define DA_COMBINE_MBHC_MASK_PLUGIN_BIT         6
#define DA_COMBINE_MBHC_MASK_ECODOWN_BIT        5
#define DA_COMBINE_MBHC_MASK_ECOUP_BIT          4
#define DA_COMBINE_MBHC_MASK_DOWN2_BIT          3
#define DA_COMBINE_MBHC_MASK_UP2_BIT            2
#define DA_COMBINE_MBHC_MASK_DOWN_BIT           1
#define DA_COMBINE_MBHC_MASK_UP_BIT             0
#define PLL_PD_VALUE                        0x02
/* sctrl_reg */
#define DA_COMBINE_VAD_INT_SET                  0x20003000

/* cfg reg begin */
/* CFG CLK */
#define DA_COMBINE_CFG_REG_CLK_CTRL_REG        (BASE_ADDR_PAGE_CFG + 0x038)
#define DA_COMBINE_CFG_REG_CLK_SEL_BIT         0
#define DA_COMBINE_CFG_REG_CLK_SW_REQ_BIT      1
#define DA_COMBINE_CFG_REG_TMUX_CLKB_BP_BIT    2
#define DA_COMBINE_CFG_REG_CLK_STATUS          (BASE_ADDR_PAGE_CFG + 0x039)

#define DA_COMBINE_SC_MAD_CTRL0                (BASE_ADDR_PAGE_CFG + 0x013)
#define DA_COMBINE_CODEC_MAINPGA_SEL           (BASE_ADDR_PAGE_CFG + 0x0AA)
#define DA_COMBINE_CODEC_MAINPGA_SEL_BIT       1

#define DA_COMBINE_DSP_SW_RST_REQ              (BASE_ADDR_PAGE_CFG + 0x001)
#define DA_COMBINE_DSP_SC_DSP_CTRL0            (BASE_ADDR_PAGE_CFG + 0x004)
#define DA_COMBINE_DSP_SC_MAD_CTRL0            (BASE_ADDR_PAGE_CFG + 0x013)
#define DA_COMBINE_DSP_RAM2AXI_CTRL            (BASE_ADDR_PAGE_CFG + 0x020)
#define DA_COMBINE_SLIM_CTRL1                  (BASE_ADDR_PAGE_CFG + 0x031)
#define DA_COMBINE_AUDIO_CLK_EN                (BASE_ADDR_PAGE_CFG + 0x040)
#define DA_COMBINE_S2_DP_CLK_EN                (BASE_ADDR_PAGE_CFG + 0x042)

#define DA_COMBINE_DSP_DAC_DP_CLK_EN_1         (BASE_ADDR_PAGE_CFG + 0x047)
#define DA_COMBINE_DSP_LP_CTRL                 (BASE_ADDR_PAGE_CFG + 0x050)
#define DA_COMBINE_AXI_CSYSREQ_BIT             4
#define DA_COMBINE_DSP_CLK_CFG                 (BASE_ADDR_PAGE_CFG + 0x052)
#define DA_COMBINE_DSP_HIFI_DIV_NUM_START_BIT  0
#define DA_COMBINE_DSP_HIFI_DIV_NUM_END_BIT    3
#define DA_COMBINE_DSP_OCDHALTON_RST           (BASE_ADDR_PAGE_CFG + 0x5D)
#define DA_COMBINE_DSP_DEBUG_RST               (BASE_ADDR_PAGE_CFG + 0x5E)

#define DA_COMBINE_DSP_APB_CLK_CFG             (BASE_ADDR_PAGE_CFG + 0x053)
#define DA_COMBINE_DSP_DSP_NMI                 (BASE_ADDR_PAGE_CFG + 0x06d)
#define DA_COMBINE_DSP_DSP_STATUS0             (BASE_ADDR_PAGE_CFG + 0x06e)
#define DA_COMBINE_DSP_CMD_STATUS_VLD          (BASE_ADDR_PAGE_CFG + 0x078)
#define DA_COMBINE_DSP_CMD_STATUS              (BASE_ADDR_PAGE_CFG + 0x079)
/* DSP SC */
#define DA_COMBINE_IRQ_SC_DSP_CTRL0            (BASE_ADDR_PAGE_CFG + 0x004)
#define DA_COMBINE_CODEC_ANA_PLL               (BASE_ADDR_PAGE_CFG + 0x0F6)


#define DA_COMBINE_AP_GPIO0_SEL                (CODEC_BASE_ADDR + BASE_ADDR_PAGE_CFG + 0x027)
#define DA_COMBINE_REG_IRQ_0                   (CODEC_BASE_ADDR + BASE_ADDR_PAGE_CFG + 0x014)
#define DA_COMBINE_REG_IRQ_1                   (CODEC_BASE_ADDR + BASE_ADDR_PAGE_CFG + 0x015)
#define DA_COMBINE_REG_IRQ_2                   (CODEC_BASE_ADDR + BASE_ADDR_PAGE_CFG + 0x016)
#define DA_COMBINE_REG_IRQ_3                   (CODEC_BASE_ADDR + BASE_ADDR_PAGE_CFG + 0x02D)
#define DA_COMBINE_REG_IRQM_0                  (CODEC_BASE_ADDR + BASE_ADDR_PAGE_CFG + 0x017)
#define DA_COMBINE_REG_IRQM_1                  (CODEC_BASE_ADDR + BASE_ADDR_PAGE_CFG + 0x018)
#define DA_COMBINE_REG_IRQM_2                  (CODEC_BASE_ADDR + BASE_ADDR_PAGE_CFG + 0x019)
#define DA_COMBINE_REG_IRQM_3                  (CODEC_BASE_ADDR + BASE_ADDR_PAGE_CFG + 0x02C)
#define DA_COMBINE_REG_AXI_DLOCK_IRQ_1         (CODEC_BASE_ADDR + BASE_ADDR_PAGE_CFG + 0x01E)
#define DA_COMBINE_REG_AXI_DLOCK_IRQ_2         (CODEC_BASE_ADDR + BASE_ADDR_PAGE_CFG + 0x01F)
#define DA_COMBINE_REG_WRITE_DSP_STATUS        (CODEC_BASE_ADDR + BASE_ADDR_PAGE_CFG + 0x021)
#define DA_COMBINE_REG_READ_DSP_STATUS         (CODEC_BASE_ADDR + BASE_ADDR_PAGE_CFG + 0x022)
/* cfg reg end */

/* reg bit musk */
#define DA_COMBINE_WRITE_DSP_STATUS_BIT_MUSK   0x10
#define DA_COMBINE_READ_DSP_STATUS_BIT_MUSK    0x10
#define DA_COMBINE_AXI_DLOCK_IRQ_BIT_MUSK_1    0xF3
#define DA_COMBINE_AXI_DLOCK_IRQ_BIT_MUSK_2    0x1F

/* io reg begin */
#define DA_COMBINE_DSP_IOS_AF_CTRL0            (BASE_ADDR_PAGE_IO + 0x100)
#define DA_COMBINE_DSP_IOS_IOM_I2S2_SDO        (BASE_ADDR_PAGE_IO + 0x238)
#define DA_COMBINE_DSP_IOS_IOM_UART_TXD        (BASE_ADDR_PAGE_IO + 0x278)

/* io reg end */

/* dig reg begin */
#define DA_COMBINE_SC_S4_IF_L                  (BASE_ADDR_PAGE_DIG + 0x006)
#define DA_COMBINE_SC_CODEC_MUX_SEL3_0         (BASE_ADDR_PAGE_DIG + 0x014)
#define DA_COMBINE_SC_S1_SRC_LR_CTRL_M         (BASE_ADDR_PAGE_DIG + 0x037)
#define DA_COMBINE_SC_S2_SRC_LR_CTRL_M         (BASE_ADDR_PAGE_DIG + 0x049)
#define DA_COMBINE_SC_S3_SRC_LR_CTRL_M         (BASE_ADDR_PAGE_DIG + 0x058)
#define DA_COMBINE_SC_S4_SRC_LR_CTRL_M         (BASE_ADDR_PAGE_DIG + 0x067)
#define DA_COMBINE_SC_MISC_SRC_CTRL_H          (BASE_ADDR_PAGE_DIG + 0x088)
#define DA_COMBINE_SC_FS_S1_CTRL_H             (BASE_ADDR_PAGE_DIG + 0x0AF)
#define DA_COMBINE_SC_FS_S2_CTRL_H             (BASE_ADDR_PAGE_DIG + 0x0B1)
#define DA_COMBINE_SC_FS_S3_CTRL_H             (BASE_ADDR_PAGE_DIG + 0x0B3)
#define DA_COMBINE_SC_FS_S4_CTRL_L             (BASE_ADDR_PAGE_DIG + 0x0B4)
#define DA_COMBINE_SC_FS_S4_CTRL_H             (BASE_ADDR_PAGE_DIG + 0x0B5)
#define DA_COMBINE_SC_FS_MISC_CTRL             (BASE_ADDR_PAGE_DIG + 0x0B6)
#define DA_COMBINE_SC_FS_SELC_TRL              (BASE_ADDR_PAGE_DIG + 0x0F2)
#define DA_COMBINE_CODEC_DP_CLK_EN             (BASE_ADDR_PAGE_DIG + 0x1D8)

/* dig reg end */

#endif

