/*
 * Copyright (c) 2020-2020 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __CLK_BALT_H
#define __CLK_BALT_H
/* clk_crgctrl */
#define CLKIN_SYS		0
#define CLKIN_SYS_SERDES		1
#define CLKIN_REF		2
#define CLK_FLL_SRC		3
#define CLK_PPLL0		4
#define CLK_PPLL1		5
#define CLK_PPLL2		6
#define CLK_PPLL2_B		7
#define CLK_PPLL3		8
#define CLK_PPLL5		9
#define CLK_PPLL7		10
#define CLK_SPLL		11
#define CLK_MODEM_BASE		12
#define CLK_ULPPLL_1		13
#define CLK_LBINTJPLL		14
#define CLK_AUPLL		15
#define CLK_SCPLL		16
#define CLK_FNPLL		17
#define CLK_PPLL_PCIE		18
#define CLK_PPLL_PCIE1		19
#define PCLK		20
#define CLK_GATE_PPLL0		21
#define CLK_GATE_PPLL1		22
#define CLK_GATE_PPLL2		23
#define CLK_GATE_PPLL2_B		24
#define CLK_GATE_PPLL3		25
#define CLK_GATE_PPLL7		26
#define CLK_GATE_PPLL0_MEDIA		27
#define CLK_GATE_PPLL2_MEDIA		28
#define CLK_GATE_PPLL2B_MEDIA		29
#define CLK_GATE_PPLL3_MEDIA		30
#define CLK_GATE_PPLL7_MEDIA		31
#define CLK_GATE_PPLL0_M2		32
#define CLK_GATE_PPLL2_M2		33
#define CLK_GATE_PPLL2B_M2		34
#define CLK_GATE_PPLL3_M2		35
#define CLK_PPLL_EPS		36
#define CLK_SYS_INI		37
#define CLK_DIV_SYSBUS		38
#define CLK_DIV_CFGBUS		39
#define PCLK_GPIO0		40
#define PCLK_GPIO1		41
#define PCLK_GPIO2		42
#define PCLK_GPIO3		43
#define PCLK_GPIO4		44
#define PCLK_GPIO5		45
#define PCLK_GPIO6		46
#define PCLK_GPIO7		47
#define PCLK_GPIO8		48
#define PCLK_GPIO9		49
#define PCLK_GPIO10		50
#define PCLK_GPIO11		51
#define PCLK_GPIO12		52
#define PCLK_GPIO13		53
#define PCLK_GPIO14		54
#define PCLK_GPIO15		55
#define PCLK_GPIO16		56
#define PCLK_GPIO17		57
#define PCLK_GPIO18		58
#define PCLK_GPIO19		59
#define PCLK_GATE_WD0_HIGH		60
#define CLK_GATE_WD0_HIGH		61
#define PCLK_GATE_WD0		62
#define PCLK_GATE_WD1		63
#define CLK_MUX_WD0		64
#define CODECCSSI_MUX		65
#define CLK_GATE_CODECSSI		66
#define PCLK_GATE_CODECSSI		67
#define CLK_FACTOR_TCXO		68
#define CLK_GATE_TIMER5_A		69
#define CLK_ANDGT_HSDT1_USBDP		70
#define CLK_DIV_HSDT1_USBDP		71
#define CLK_GATE_HSDT1_USBDP		72
#define AUTODIV_SYSBUS		73
#define AUTODIV_HSDT1BUS		74
#define PCLK_GATE_IOC		75
#define PCLK_GATE_HSDT0_PCIE		76
#define PCLK_GATE_HSDT1_PCIE1		77
#define PCLK_DIV_PCIE		78
#define PCLK_ANDGT_PCIE		79
#define CLK_ATDVFS		80
#define ATCLK		81
#define TRACKCLKIN		82
#define PCLK_DBG		83
#define PCLK_DIV_DBG		84
#define TRACKCLKIN_DIV		85
#define ACLK_GATE_PERF_STAT		86
#define PCLK_GATE_PERF_STAT		87
#define CLK_DIV_PERF_STAT		88
#define CLK_PERF_DIV_GT		89
#define CLK_GATE_PERF_STAT		90
#define CLK_DIV_CSSYSDBG		91
#define CLK_GATE_CSSYSDBG		92
#define CLK_DIV_DMABUS		93
#define CLK_GATE_DMAC		94
#define CLK_GATE_DMA_IOMCU		95
#define CLK_GATE_CSSYS_ATCLK		96
#define CLK_GATE_SOCP_ACPU		97
#define CLK_DIV_SOCP_DEFLATE		98
#define CLK_SOCP_DEFLATE_GT		99
#define CLK_GATE_SOCP_DEFLATE		100
#define CLK_GATE_TCLK_SOCP		101
#define CLK_GATE_TIME_STAMP_GT		102
#define CLK_DIV_TIME_STAMP		103
#define CLK_GATE_TIME_STAMP		104
#define CLK_GATE_IPF		105
#define CLK_GATE_SDIO		106
#define HCLK_GATE_SDIO		107
#define PCLK_GATE_DSI0		108
#define PCLK_GATE_DSI1		109
#define CLK_GATE_LDI0		110
#define CLK_GATE_VENC2		111
#define CLK_GATE_PPLL5		112
#define CLK_GATE_FD_FUNC		113
#define CLK_GATE_FDAI_FUNC		114
#define CLK_GATE_SD		115
#define CLK_DIV_HSDT1BUS		116
#define CLK_MUX_SD_SYS		117
#define CLK_MUX_SD_PLL		118
#define CLK_DIV_SD		119
#define CLK_ANDGT_SD		120
#define CLK_SD_SYS		121
#define CLK_SD_SYS_GT		122
#define CLK_MUX_A53HPM		123
#define CLK_A53HPM_ANDGT		124
#define CLK_DIV_A53HPM		125
#define CLK_MUX_320M		126
#define CLK_320M_PLL_GT		127
#define CLK_DIV_320M		128
#define CLK_GATE_UART1		129
#define CLK_GATE_UART4		130
#define PCLK_GATE_UART1		131
#define PCLK_GATE_UART4		132
#define CLK_MUX_UARTH		133
#define CLK_DIV_UARTH		134
#define CLK_ANDGT_UARTH		135
#define CLK_GATE_UART2		136
#define CLK_GATE_UART5		137
#define PCLK_GATE_UART2		138
#define PCLK_GATE_UART5		139
#define CLK_MUX_UARTL		140
#define CLK_DIV_UARTL		141
#define CLK_ANDGT_UARTL		142
#define CLK_GATE_UART0		143
#define PCLK_GATE_UART0		144
#define CLK_MUX_UART0		145
#define CLK_DIV_UART0		146
#define CLK_ANDGT_UART0		147
#define CLK_FACTOR_UART0		148
#define CLK_UART0_DBG		149
#define CLK_GATE_I2C2_ACPU		150
#define CLK_GATE_I2C3		151
#define CLK_GATE_I2C4		152
#define CLK_GATE_I2C6_ACPU		153
#define CLK_GATE_I2C7		154
#define PCLK_GATE_I2C2		155
#define PCLK_GATE_I2C3		156
#define PCLK_GATE_I2C4		157
#define PCLK_GATE_I2C6_ACPU		158
#define PCLK_GATE_I2C7		159
#define CLK_DIV_I2C		160
#define CLK_MUX_I2C		161
#define CLK_ANDGT_I3C4		162
#define CLK_DIV_I3C4		163
#define CLK_GATE_I3C4		164
#define PCLK_GATE_I3C4		165
#define CLK_GATE_SPI1		166
#define CLK_GATE_SPI4		167
#define PCLK_GATE_SPI1		168
#define PCLK_GATE_SPI4		169
#define CLK_MUX_SPI		170
#define CLK_DIV_SPI		171
#define CLK_ANDGT_SPI		172
#define CLK_GATE_USB3OTG_REF		173
#define CLK_FACTOR_USB3PHY_PLL		174
#define CLK_USB2PHY_REF_DIV		175
#define CLK_GATE_ABB_USB		176
#define CLK_PCIE1PLL_SERDES		177
#define CLKGT_HSDT1_PCIE1		178
#define DIV_HSDT1_PCIE1		179
#define ACLK_GATE_HSDT1_PCIE1		180
#define CLK_GATE_UFSPHY_REF		181
#define CLK_GATE_UFSIO_REF		182
#define CLK_GATE_AO_ASP		183
#define CLK_DIV_AO_ASP		184
#define CLK_MUX_AO_ASP		185
#define CLK_DIV_AO_ASP_GT		186
#define PCLK_GATE_PCTRL		187
#define CLK_ANDGT_PTP		188
#define CLK_DIV_PTP		189
#define CLK_GATE_PWM		190
#define CLK_GATE_BLPWM		191
#define CLK_SYSCNT_DIV		192
#define CLK_GATE_GPS_REF		193
#define CLK_MUX_GPS_REF		194
#define CLK_GATE_MDM2GPS0		195
#define CLK_GATE_MDM2GPS1		196
#define CLK_GATE_MDM2GPS2		197
#define PERI_VOLT_HOLD		198
#define PERI_VOLT_MIDDLE		199
#define PERI_VOLT_LOW		200
#define EPS_VOLT_HIGH		201
#define EPS_VOLT_MIDDLE		202
#define EPS_VOLT_LOW		203
#define VENC_VOLT_HOLD		204
#define VDEC_VOLT_HOLD		205
#define EDC_VOLT_HOLD		206
#define EFUSE_VOLT_HOLD		207
#define LDI0_VOLT_HOLD		208
#define HISE_VOLT_HOLD		209
#define CLK_GT_DPCTRL_16M		210
#define CLK_FIX_DIV_DPCTRL		211
#define CLK_DIV_DPCTRL_16M		212
#define CLK_GATE_DPCTRL_16M		213
#define CLK_GT_ISP_I2C		214
#define CLK_DIV_ISP_I2C		215
#define CLK_GATE_ISP_I2C_MEDIA		216
#define CLK_GATE_ISP_SNCLK0		217
#define CLK_GATE_ISP_SNCLK1		218
#define CLK_GATE_ISP_SNCLK2		219
#define CLK_GATE_ISP_SNCLK3		220
#define CLK_ISP_SNCLK_MUX0		221
#define CLK_ISP_SNCLK_DIV0		222
#define CLK_ISP_SNCLK_MUX1		223
#define CLK_ISP_SNCLK_DIV1		224
#define CLK_ISP_SNCLK_MUX2		225
#define CLK_ISP_SNCLK_DIV2		226
#define CLK_ISP_SNCLK_MUX3		227
#define CLK_ISP_SNCLK_DIV3		228
#define CLK_ISP_SNCLK_FAC		229
#define CLK_ISP_SNCLK_ANGT		230
#define CLK_GATE_TXDPHY0_CFG		231
#define CLK_GATE_TXDPHY0_REF		232
#define CLK_GATE_TXDPHY1_CFG		233
#define CLK_GATE_TXDPHY1_REF		234
#define PCLK_GATE_LOADMONITOR		235
#define CLK_GATE_LOADMONITOR		236
#define CLK_DIV_LOADMONITOR		237
#define CLK_GT_LOADMONITOR		238
#define PCLK_GATE_LOADMONITOR_L		239
#define CLK_GATE_LOADMONITOR_L		240
#define PCLK_GATE_LOADMONITOR_2		241
#define CLK_GATE_LOADMONITOR_2		242
#define CLK_GATE_MEDIA_TCXO		243
#define CLK_AO_HIFD_MUX		244
#define CLK_ANDGT_AO_HIFD		245
#define CLK_DIV_AO_HIFD		246
#define CLK_GATE_AO_HIFD		247
#define CLK_GATE_SPE_GT		248
#define CLK_DIV_SPE		249
#define CLK_GATE_SPE_REF		250
#define HCLK_GATE_SPE		251
#define CLK_GATE_SPE		252
#define CLK_GATE_AXI_MEM_GS		253
#define CLK_GATE_AXI_MEM		254
#define ACLK_GATE_AXI_MEM		255
#define CLK_GATE_MAA_REF		256
#define ACLK_GATE_MAA		257
#define CLK_UART6		258
#define CLK_GATE_I2C0		259
#define CLK_GATE_I2C1		260
#define CLK_GATE_I2C2		261
#define CLK_GATE_SPI0		262
#define CLK_FAC_180M		263
#define CLK_GATE_IOMCU_PERI0		264
#define CLK_GATE_SPI2		265
#define CLK_GATE_UART3		266
#define CLK_GATE_UART8		267
#define CLK_GATE_UART7		268
#define OSC32K		269
#define OSC19M		270
#define CLK_480M		271
#define CLK_INVALID		272
#define AUTODIV_CFGBUS		273
#define AUTODIV_DMABUS		274
#define AUTODIV_ISP_DVFS		275
#define AUTODIV_ISP		276
#define CLK_GATE_ATDIV_HSDT1BUS		277
#define CLK_GATE_ATDIV_DMA		278
#define CLK_GATE_ATDIV_CFG		279
#define CLK_GATE_ATDIV_SYS		280
#define CLK_FPGA_1P92		281
#define CLK_FPGA_2M		282
#define CLK_FPGA_10M		283
#define CLK_FPGA_19M		284
#define CLK_FPGA_20M		285
#define CLK_FPGA_24M		286
#define CLK_FPGA_26M		287
#define CLK_FPGA_27M		288
#define CLK_FPGA_32M		289
#define CLK_FPGA_40M		290
#define CLK_FPGA_48M		291
#define CLK_FPGA_50M		292
#define CLK_FPGA_57M		293
#define CLK_FPGA_60M		294
#define CLK_FPGA_64M		295
#define CLK_FPGA_80M		296
#define CLK_FPGA_100M		297
#define CLK_FPGA_160M		298

/* clk_hsdt_crg */
#define CLK_GATE_PCIE0PLL		0
#define CLK_GATE_PCIEPHY_REF		1
#define PCLK_GATE_PCIE_SYS		2
#define PCLK_GATE_PCIE_PHY		3
#define ACLK_GATE_PCIE		4
#define CLK_GATE_HSDT_TCU		5
#define CLK_GATE_HSDT_TBU		6

/* clk_hsdt1_crg */
#define CLK_GATE_PCIE1PLL		0
#define HCLK_GATE_USB3OTG		1
#define ACLK_GATE_USB3OTG		2
#define CLK_GATE_PCIE1PHY_REF		3
#define PCLK_GATE_PCIE1_SYS		4
#define PCLK_GATE_PCIE1_PHY		5
#define HCLK_GATE_SD		6
#define CLK_ANDGT_USB2PHY_REF		7
#define CLK_GATE_USB2PHY_REF		8
#define CLK_MUX_ULPI		9
#define CLK_GATE_ULPI_REF		10
#define CLK_HSDT1_EUSB_MUX		11
#define CLK_GATE_HSDT1_EUSB		12
#define CLK_GATE_PCIEAUX1		13
#define CLK_GATE_HSDT1_TBU		14
#define CLK_GATE_HSDT1_TCU		15
#define CLK_GATE_USB2_ULPI		16
#define ACLK_GATE_PCIE1		17
#define PCLK_GATE_DPCTRL		18
#define ACLK_GATE_DPCTRL		19

/* clk_sctrl */
#define CLK_GATE_AUPLL		0
#define PCLK_GPIO20		1
#define PCLK_GPIO21		2
#define CLK_DIV_AOBUS		3
#define CLK_GATE_TIMER5_B		4
#define CLK_MUX_TIMER5_A		5
#define CLK_MUX_TIMER5_B		6
#define CLK_GATE_TIMER5		7
#define CLK_MUX_I2C9		8
#define CLK_ANDGT_I2C9		9
#define CLK_DIV_I2C9		10
#define CLK_GATE_I2C9		11
#define PCLK_GATE_I2C9		12
#define CLK_GATE_SPI		13
#define PCLK_GATE_SPI		14
#define CLK_GATE_SPI5		15
#define PCLK_GATE_SPI5		16
#define CLK_ANDGT_32KPLL_PCIEAUX		17
#define CLK_DIV_32KPLL_PCIEAUX		18
#define CLK_MUX_PCIEAUX		19
#define CLK_GATE_PCIEAUX		20
#define CLK_ANDGT_IOPERI		21
#define CLK_DIV_IOPERI		22
#define CLK_ANDGT_ULPPLL		23
#define DIV_IOPERI_ULPPLL		24
#define CLK_MUX_IOPERI		25
#define PCLK_GATE_RTC		26
#define PCLK_AO_GPIO0		27
#define PCLK_AO_GPIO1		28
#define PCLK_AO_GPIO2		29
#define PCLK_AO_GPIO3		30
#define PCLK_AO_GPIO4		31
#define PCLK_AO_GPIO5		32
#define PCLK_AO_GPIO6		33
#define PCLK_AO_GPIO29		34
#define PCLK_AO_GPIO30		35
#define PCLK_AO_GPIO31		36
#define PCLK_AO_GPIO32		37
#define PCLK_AO_GPIO33		38
#define PCLK_AO_GPIO34		39
#define PCLK_AO_GPIO35		40
#define PCLK_AO_GPIO36		41
#define PCLK_GATE_SYSCNT		42
#define CLK_GATE_SYSCNT		43
#define CLKMUX_SYSCNT		44
#define CLK_ASP_BACKUP		45
#define CLKGT_ASP_CODEC		46
#define CLKDIV_ASP_CODEC		47
#define CLK_MUX_ASP_CODEC		48
#define CLK_ASP_CODEC		49
#define CLK_GATE_ASP_SUBSYS		50
#define CLK_MUX_ASP_PLL		51
#define CLK_AO_ASP_32KPLL_MUX		52
#define CLK_AO_ASP_MUX		53
#define CLK_GATE_ASP_TCXO		54
#define CLK_GATE_DP_AUDIO_PLL		55
#define CLKDIV_DP_AUDIO_PLL_AO		56
#define CLKGT_DP_AUDIO_PLL_AO		57
#define CLK_MUX_AO_CAMERA		58
#define CLK_ANDGT_AO_CAMERA		59
#define CLK_DIV_AO_CAMERA		60
#define CLK_GATE_AO_CAMERA		61
#define CLK_GATE_RXDPHY0_CFG		62
#define CLK_GATE_RXDPHY1_CFG		63
#define CLK_GATE_RXDPHY2_CFG		64
#define CLK_GATE_RXDPHY3_CFG		65
#define CLK_GATE_RXDPHY4_CFG		66
#define CLK_GATE_RXDPHY_CFG		67
#define CLK_DIV_RXDPHY_CFG		68
#define CLKANDGT_RXDPHY_CFG		69
#define CLK_MUX_RXDPHY_CFG		70
#define PCLK_GATE_AO_LOADMONITOR		71
#define CLK_GATE_AO_LOADMONITOR		72
#define CLK_DIV_AO_LOADMONITOR		73
#define CLK_GT_AO_LOADMONITOR		74
#define CLK_SW_AO_LOADMONITOR		75
#define CLK_GATE_HIFD_TCXO		76
#define CLK_ANDGT_HIFD_FLL		77
#define CLK_DIV_HIFD_FLL		78
#define CLK_GATE_HIFD_FLL		79
#define CLK_HIFD_PLL_MUX		80
#define CLK_ANDGT_HIFD_PLL		81
#define CLK_DIV_HIFD_PLL		82
#define CLK_GATE_HIFD_PLL		83

/* clk_iomcu_crgctrl */
#define CLK_I2C1_GATE_IOMCU		0

/* clk_media1_crg */
#define PCLK_GATE_ISP_QIC_SUBSYS		0
#define ACLK_GATE_ISP_QIC_SUBSYS		1
#define ACLK_GATE_MEDIA_COMMON		2
#define ACLK_GATE_QIC_DSS		3
#define PCLK_GATE_MEDIA_COMMON		4
#define PCLK_GATE_QIC_DSS_CFG		5
#define PCLK_GATE_MMBUF_CFG		6
#define PCLK_GATE_DISP_QIC_SUBSYS		7
#define ACLK_GATE_DISP_QIC_SUBSYS		8
#define PCLK_GATE_DSS		9
#define ACLK_GATE_DSS		10
#define ACLK_GATE_ISP		11
#define CLK_MUX_VIVOBUS		12
#define CLK_GATE_VIVOBUS_ANDGT		13
#define CLK_DIV_VIVOBUS		14
#define CLK_GATE_VIVOBUS		15
#define CLK_MUX_LDI1		16
#define CLK_ANDGT_LDI1		17
#define CLK_DIV_LDI1		18
#define CLK_GATE_LDI1		19
#define CLK_MUX_ISPI2C		20
#define CLK_GATE_ISPI2C		21
#define CLK_GATE_ISP_SYS		22
#define CLK_MUX_ISPCPU		23
#define CLK_ANDGT_ISPCPU		24
#define CLK_DIV_ISPCPU		25
#define CLK_GATE_ISPCPU		26
#define CLK_ANDGT_ISP_I3C		27
#define CLK_DIV_ISP_I3C		28
#define CLK_GATE_ISP_I3C		29
#define CLK_GATE_BRG		30
#define PCLK_GATE_MEDIA1_LM		31
#define CLK_GATE_LOADMONITOR_MEDIA1		32
#define ACLK_GATE_ASC		33
#define CLK_GATE_DSS_AXI_MM		34
#define PCLK_GATE_MMBUF		35
#define PCLK_DIV_MMBUF		36
#define PCLK_MMBUF_ANDGT		37
#define CLK_GATE_ATDIV_VIVO		38
#define CLK_GATE_ATDIV_ISPCPU		39

/* clk_media2_crg */
#define CLK_GATE_VCODECBUS		0
#define CLK_GATE_VCODECBUS2DDR		1
#define CLK_DIV_VCODECBUS		2
#define CLK_GATE_VCODECBUS_GT		3
#define CLK_VCODEC_SYSPLL0		4
#define CLK_MUX_VCODECBUS		5
#define CLK_MUX_VDEC		6
#define CLK_ANDGT_VDEC		7
#define CLK_DIV_VDEC		8
#define CLK_GATE_VDECFREQ		9
#define PCLK_GATE_VDEC		10
#define ACLK_GATE_VDEC		11
#define CLK_MUX_VENC		12
#define CLK_ANDGT_VENC		13
#define CLK_DIV_VENC		14
#define CLK_GATE_VENCFREQ		15
#define PCLK_GATE_VENC		16
#define ACLK_GATE_VENC		17
#define PCLK_GATE_MEDIA2_LM		18
#define CLK_GATE_LOADMONITOR_MEDIA2		19
#define CLK_GATE_IVP32DSP_TCXO		20
#define CLK_MUX_IVP32DSP_CORE		21
#define CLK_ANDGT_IVP32DSP_CORE		22
#define CLK_DIV_IVP32DSP_CORE		23
#define CLK_GATE_IVP32DSP_COREFREQ		24
#define CLK_MUX_IVP1DSP_CORE		25
#define CLK_ANDGT_IVP1DSP_CORE		26
#define CLK_DIV_IVP1DSP_CORE		27
#define CLK_GATE_IVP1DSP_COREFREQ		28
#define CLK_MUX_ARPP		29
#define CLK_ANDGT_ARPP		30
#define CLK_DIV_ARPP		31
#define CLK_GATE_ARPPFREQ		32
#define CLK_ADE_FUNC_MUX		33
#define CLK_JPG_FUNC_MUX		34
#define CLK_ANDGT_ADE_FUNC		35
#define CLK_ANDGT_JPG_FUNC		36
#define CLK_DIV_ADE_FUNC		37
#define CLK_DIV_JPG_FUNC		38
#define CLK_GATE_ADE_FUNCFREQ		39
#define CLK_GATE_JPG_FUNCFREQ		40
#define ACLK_GATE_JPG		41
#define PCLK_GATE_JPG		42
#define CLK_GATE_AUTODIV_VCODECBUS		43
#define CLK_GATE_ATDIV_VDEC		44
#define CLK_GATE_ATDIV_VENC		45

/* clk_pctrl */

/* clk_xfreqclk */
#define CLK_CLUSTER0		0
#define CLK_CLUSTER1		1
#define CLK_G3D		2
#define CLK_DDRC_FREQ		3
#define CLK_DDRC_MAX		4
#define CLK_DDRC_MIN		5
#define CLK_DMSS_MIN		6

/* clk_pmuctrl */
#define CLK_GATE_ABB_192		0
#define CLK_PMU32KA		1
#define CLK_PMU32KB		2
#define CLK_PMU32KC		3
#define CLK_PMUAUDIOCLK		4
#define CLK_EUSB_38M4		5

/* clk_interactive */

#define CLK_GATE_EDC0		0
#define CLK_GATE_VDEC		1
#define CLK_GATE_VENC		2
#define CLK_GATE_ISPFUNC		3
#define CLK_GATE_JPG_FUNC		4
#define CLK_GATE_MEDIA_COMMON		5
#define CLK_GATE_IVP32DSP_CORE		6
#define CLK_GATE_IVP1DSP_CORE		7
#define CLK_GATE_ISPFUNC2		8
#define CLK_GATE_ISPFUNC3		9
#define CLK_GATE_ISPFUNC4		10
#define CLK_GATE_ISPFUNC5		11
#define CLK_GATE_ADE_FUNC		12
#define CLK_GATE_ARPP		13
#define CLK_GATE_HIFACE		14

#define CLK_GATE_ISPFUNCFREQ		0
#define CLK_GATE_ISPFUNC2FREQ		1
#define CLK_GATE_ISPFUNC3FREQ		2
#define CLK_GATE_ISPFUNC4FREQ		3
#define CLK_GATE_ISPFUNC5FREQ		4
#define CLK_GATE_MEDIA_COMMONFREQ		5
#define CLK_GATE_MMBUF		6
#define CLK_GATE_EDC0FREQ		7

#endif
