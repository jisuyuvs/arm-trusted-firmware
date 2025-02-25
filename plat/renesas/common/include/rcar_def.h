/*
 * Copyright (c) 2015-2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RCAR_DEF_H
#define RCAR_DEF_H

#include <common/tbbr/tbbr_img_def.h>
#include <lib/utils_def.h>

#define RCAR_PRIMARY_CPU		0x0
#define RCAR_TRUSTED_SRAM_BASE		0x44000000
#define RCAR_TRUSTED_SRAM_SIZE		0x0003E000
#define RCAR_SHARED_MEM_BASE		(RCAR_TRUSTED_SRAM_BASE + \
					RCAR_TRUSTED_SRAM_SIZE)
#define RCAR_SHARED_MEM_SIZE		U(0x00001000)
#define FLASH0_BASE			U(0x08000000)
#define FLASH0_SIZE			U(0x04000000)
#define FLASH_MEMORY_SIZE		U(0x04000000)	/* hyper flash */
#define FLASH_TRANS_SIZE_UNIT		U(0x00000100)
#define DEVICE_RCAR_BASE		U(0xE6000000)
#define DEVICE_RCAR_SIZE		U(0x00300000)
#define DEVICE_RCAR_BASE2		U(0xE6360000)
#define DEVICE_RCAR_SIZE2		U(0x19CA0000)
#define DEVICE_SRAM_BASE		U(0xE6300000)
#define DEVICE_SRAM_SIZE		U(0x00002000)
#define DEVICE_SRAM_STACK_BASE		(DEVICE_SRAM_BASE + DEVICE_SRAM_SIZE)
#define DEVICE_SRAM_STACK_SIZE		U(0x00001000)
#define DRAM_LIMIT			ULL(0x0000010000000000)
#define DRAM1_BASE			U(0x40000000)
#define DRAM1_SIZE			U(0x80000000)
#define DRAM1_NS_BASE			(DRAM1_BASE + U(0x10000000))
#define DRAM1_NS_SIZE			(DRAM1_SIZE - DRAM1_NS_BASE)
#define DRAM_40BIT_BASE			ULL(0x0400000000)
#define DRAM_40BIT_SIZE			ULL(0x0400000000)
#define DRAM_PROTECTED_BASE		ULL(0x43F00000)
#define DRAM_40BIT_PROTECTED_BASE	ULL(0x0403F00000)
#define DRAM_PROTECTED_SIZE		ULL(0x03F00000)
#define RCAR_BL31_CRASH_BASE		U(0x4403F000)
#define RCAR_BL31_CRASH_SIZE		U(0x00001000)
/* Entrypoint mailboxes */
#define MBOX_BASE			RCAR_SHARED_MEM_BASE
#define MBOX_SIZE			0x200
/* Base address where parameters to BL31 are stored */
#define PARAMS_BASE			(MBOX_BASE + MBOX_SIZE)
#define BOOT_KIND_BASE			(RCAR_SHARED_MEM_BASE + \
					RCAR_SHARED_MEM_SIZE - 0x100)
/*
 * The number of regions like RO(code), coherent and data required by
 * different BL stages which need to be mapped in the MMU
 */
#if USE_COHERENT_MEM
#define RCAR_BL_REGIONS			(3)
#else
#define RCAR_BL_REGIONS			(2)
#endif
/*
 * The RCAR_MAX_MMAP_REGIONS depends on the number of entries in rcar_mmap[]
 * defined for each BL stage in rcar_common.c.
 */
#if IMAGE_BL2
#define RCAR_MMAP_ENTRIES		(9)
#endif
#if IMAGE_BL31
#define RCAR_MMAP_ENTRIES		(9)
#endif
#if IMAGE_BL2
#define REG1_BASE			U(0xE6400000)
#define REG1_SIZE			U(0x04C00000)
#define ROM0_BASE			U(0xEB100000)
#define ROM0_SIZE			U(0x00028000)
#define REG2_BASE			U(0xEC000000)
#define REG2_SIZE			U(0x14000000)
#endif
/* BL33  */
#define NS_IMAGE_OFFSET			(DRAM1_BASE + U(0x09000000))
/* BL31 */
#define RCAR_DEVICE_BASE		DEVICE_RCAR_BASE
#define RCAR_DEVICE_SIZE		(0x1A000000)
#define RCAR_LOG_RES_SIZE		(64)
#define RCAR_LOG_HEADER_SIZE		(16)
#define RCAR_LOG_OTHER_SIZE		(RCAR_LOG_HEADER_SIZE + \
					RCAR_LOG_RES_SIZE)
#define RCAR_BL31_LOG_MAX		(RCAR_BL31_LOG_SIZE - \
					RCAR_LOG_OTHER_SIZE)
#define RCAR_CRASH_STACK		RCAR_BL31_CRASH_BASE
#define AARCH64_SPACE_BASE		ULL(0x00000000000)
#define AARCH64_SPACE_SIZE		ULL(0x10000000000)
/* CCI related constants */
#define CCI500_BASE				U(0xF1200000)
#define CCI500_CLUSTER0_SL_IFACE_IX		(2)
#define CCI500_CLUSTER1_SL_IFACE_IX		(3)
#define CCI500_CLUSTER0_SL_IFACE_IX_FOR_M3	(1)
#define CCI500_CLUSTER1_SL_IFACE_IX_FOR_M3	(2)
#define RCAR_CCI_BASE				CCI500_BASE
/* GIC */
#define RCAR_GICD_BASE			U(0xF1010000)
#define RCAR_GICR_BASE			U(0xF1010000)
#define RCAR_GICC_BASE			U(0xF1020000)
#define RCAR_GICH_BASE			U(0xF1040000)
#define RCAR_GICV_BASE			U(0xF1060000)
#define ARM_IRQ_SEC_PHY_TIMER		U(29)
#define ARM_IRQ_SEC_SGI_0		U(8)
#define ARM_IRQ_SEC_SGI_1		U(9)
#define ARM_IRQ_SEC_SGI_2		U(10)
#define ARM_IRQ_SEC_SGI_3		U(11)
#define ARM_IRQ_SEC_SGI_4		U(12)
#define ARM_IRQ_SEC_SGI_5		U(13)
#define ARM_IRQ_SEC_SGI_6		U(14)
#define ARM_IRQ_SEC_SGI_7		U(15)
#define ARM_IRQ_SEC_RPC			U(70)
#define ARM_IRQ_SEC_TIMER		U(166)
#define ARM_IRQ_SEC_TIMER_UP		U(171)
#define ARM_IRQ_SEC_WDT			U(173)
#define ARM_IRQ_SEC_CRYPT		U(102)
#define ARM_IRQ_SEC_CRYPT_SecPKA	U(97)
#define ARM_IRQ_SEC_CRYPT_PubPKA	U(98)
/* Timer control */
#define RCAR_CNTC_BASE		U(0xE6080000)
/* Reset */
#define RCAR_MODEMR		U(0xE6160060)	/* Mode pin             */
#define RCAR_CA57RESCNT		U(0xE6160040)	/* Reset control A57    */
#define RCAR_CA53RESCNT		U(0xE6160044)	/* Reset control A53    */
#define RCAR_SRESCR		U(0xE6160110)	/* Soft Power On Reset  */
#define RCAR_CA53WUPCR		U(0xE6151010)	/* Wake-up control A53  */
#define RCAR_CA57WUPCR		U(0xE6152010)	/* Wake-up control A57  */
#define RCAR_CA53PSTR		U(0xE6151040)	/* Power status A53     */
#define RCAR_CA57PSTR		U(0xE6152040)	/* Power status A57     */
#define RCAR_CA53CPU0CR		U(0xE6151100)	/* CPU control  A53     */
#define RCAR_CA57CPU0CR		U(0xE6152100)	/* CPU control  A57     */
#define RCAR_CA53CPUCMCR	U(0xE6151184)	/* Common power A53     */
#define RCAR_CA57CPUCMCR	U(0xE6152184)	/* Common power A57     */
#define RCAR_WUPMSKCA57		U(0xE6180014)	/* Wake-up mask A57     */
#define RCAR_WUPMSKCA53		U(0xE6180018)	/* Wake-up mask A53     */
/* SYSC	*/
#define RCAR_PWRSR3		U(0xE6180140)	/* Power stat A53-SCU   */
#define RCAR_PWRSR5		U(0xE61801C0)	/* Power stat A57-SCU   */
#define RCAR_SYSCIER		U(0xE618000C)	/* Interrupt enable     */
#define RCAR_SYSCIMR		U(0xE6180010)	/* Interrupt mask       */
#define RCAR_SYSCSR		U(0xE6180000)	/* SYSC status          */
#define RCAR_PWRONCR3		U(0xE618014C)	/* Power resume A53-SCU */
#define RCAR_PWRONCR5		U(0xE61801CC)	/* Power resume A57-SCU */
#define RCAR_PWROFFCR3		U(0xE6180144)	/* Power shutoff A53-SCU */
#define RCAR_PWROFFCR5		U(0xE61801C4)	/* Power shutoff A57-SCU */
#define RCAR_PWRER3		U(0xE6180154)	/* shutoff/resume error */
#define RCAR_PWRER5		U(0xE61801D4)	/* shutoff/resume error */
#define RCAR_SYSCISR		U(0xE6180004)	/* Interrupt status     */
#define RCAR_SYSCISCR		U(0xE6180008)	/* Interrupt stat clear */
#define	RCAR_SYSCEXTMASK	U(0xE61802F8)	/* External Request Mask */
						/* H3/H3-N, M3 v3.0, M3-N, E3 */
/* Product register */
#define RCAR_PRR			U(0xFFF00044)
#define RCAR_M3_CUT_VER11		U(0x00000010)	/* M3 Ver.1.1/Ver.1.2 */
#define RCAR_D3_CUT_VER10		U(0x00000000)	/* D3 Ver.1.0 */
#define RCAR_D3_CUT_VER11		U(0x00000010)	/* D3 Ver.1.1 */
#define RCAR_MAJOR_MASK			U(0x000000F0)
#define RCAR_MINOR_MASK			U(0x0000000F)
#define PRR_PRODUCT_SHIFT		U(8)
#define RCAR_MAJOR_SHIFT		U(4)
#define RCAR_MINOR_SHIFT		U(0)
#define RCAR_MAJOR_OFFSET		U(1)
#define RCAR_M3_MINOR_OFFSET		U(2)
#define PRR_PRODUCT_H3_CUT10		(PRR_PRODUCT_H3 | U(0x00))	/* 1.0 */
#define PRR_PRODUCT_H3_CUT11		(PRR_PRODUCT_H3 | U(0x01))	/* 1.1 */
#define PRR_PRODUCT_H3_CUT20		(PRR_PRODUCT_H3 | U(0x10))	/* 2.0 */
#define PRR_PRODUCT_M3_CUT10		(PRR_PRODUCT_M3 | U(0x00))	/* 1.0 */
#define PRR_PRODUCT_M3_CUT11		(PRR_PRODUCT_M3 | U(0x10))
#define PRR				0xFFF00044U
#define PRR_PRODUCT_MASK		0x00007F00U
#define PRR_CUT_MASK			0x000000FFU
#define PRR_PRODUCT_H3			0x00004F00U	/* R-Car H3 */
#define PRR_PRODUCT_M3			0x00005200U	/* R-Car M3-W */
#define PRR_PRODUCT_V3M			0x00005400U	/* R-Car V3M */
#define PRR_PRODUCT_M3N			0x00005500U	/* R-Car M3-N */
#define PRR_PRODUCT_V3H			0x00005600U	/* R-Car V3H */
#define PRR_PRODUCT_E3			0x00005700U	/* R-Car E3 */
#define PRR_PRODUCT_D3			0x00005800U	/* R-Car D3 */
#define PRR_PRODUCT_10			0x00U		/* Ver.1.0 */
#define PRR_PRODUCT_11			0x01U		/* Ver.1.1 */
#define PRR_PRODUCT_20			0x10U		/* Ver.2.0 */
#define PRR_PRODUCT_21			0x11U		/* Ver.2.1 */
#define PRR_PRODUCT_30			0x20U		/* Ver.3.0 */
#define RCAR_CPU_MASK_CA57		U(0x80000000)
#define RCAR_CPU_MASK_CA53		U(0x04000000)
#define RCAR_CPU_HAVE_CA57		U(0x00000000)
#define RCAR_CPU_HAVE_CA53		U(0x00000000)
#define RCAR_SSCG_MASK			U(0x1000)	/* MD12 */
#define RCAR_SSCG_ENABLE		U(0x1000)
/* MD pin information */
#define MODEMR_BOOT_CPU_MASK		U(0x000000C0)
#define MODEMR_BOOT_CPU_CR7		U(0x000000C0)
#define MODEMR_BOOT_CPU_CA57		U(0x00000000)
#define MODEMR_BOOT_CPU_CA53		U(0x00000040)
#define MODEMR_BOOT_DEV_MASK		U(0x0000001E)
#define MODEMR_BOOT_DEV_HYPERFLASH160	U(0x00000004)
#define MODEMR_BOOT_DEV_HYPERFLASH80	U(0x00000006)
#define MODEMR_BOOT_DEV_QSPI_FLASH40	U(0x00000008)
#define MODEMR_BOOT_DEV_QSPI_FLASH80	U(0x0000000C)
#define MODEMR_BOOT_DEV_EMMC_25X1	U(0x0000000A)
#define MODEMR_BOOT_DEV_EMMC_50X8	U(0x0000001A)
#define MODEMR_BOOT_PLL_MASK		U(0x00006000)
#define MODEMR_BOOT_PLL_SHIFT		U(13)
/* Memory mapped Generic timer interfaces */
#define ARM_SYS_CNTCTL_BASE		RCAR_CNTC_BASE
/* MODEMR PLL masks and bitfield values */
#define CHECK_MD13_MD14			U(0x6000)
#define MD14_MD13_TYPE_0		U(0x0000)	/* MD14=0 MD13=0 */
#define MD14_MD13_TYPE_1		U(0x2000)	/* MD14=0 MD13=1 */
#define MD14_MD13_TYPE_2		U(0x4000)	/* MD14=1 MD13=0 */
#define MD14_MD13_TYPE_3		U(0x6000)	/* MD14=1 MD13=1 */
/* Frequency of EXTAL(Hz) */
#define EXTAL_MD14_MD13_TYPE_0		U(8333300)	/* MD14=0 MD13=0 */
#define EXTAL_MD14_MD13_TYPE_1		U(10000000)	/* MD14=0 MD13=1 */
#define EXTAL_MD14_MD13_TYPE_2		U(12500000)	/* MD14=1 MD13=0 */
#define EXTAL_MD14_MD13_TYPE_3		U(16666600)	/* MD14=1 MD13=1 */
#define EXTAL_SALVATOR_XS		U(8320000)	/* Salvator-XS */
#define EXTAL_EBISU			U(24000000)	/* Ebisu */
#define EXTAL_DRAAK			U(24000000)	/* Draak */
/* CPG write protect registers	*/
#define CPGWPR_PASSWORD			(0x5A5AFFFFU)
#define CPGWPCR_PASSWORD		(0xA5A50000U)
/* CA5x Debug Resource control registers */
#define CPG_CA57DBGRCR			(CPG_BASE + 0x2180U)
#define CPG_CA53DBGRCR			(CPG_BASE + 0x1180U)
#define DBGCPUPREN			((uint32_t)1U << 19U)
#define CPG_PLL0CR			(CPG_BASE + 0x00D8U)
#define CPG_PLL2CR			(CPG_BASE + 0x002CU)
#define CPG_PLL4CR			(CPG_BASE + 0x01F4U)
#define CPG_CPGWPCR			(CPG_BASE + 0x0904U)
/* RST Registers */
#define RST_BASE			(0xE6160000U)
#define RST_WDTRSTCR			(RST_BASE + 0x0054U)
#define RST_MODEMR			(RST_BASE + 0x0060U)
#define WDTRSTCR_PASSWORD		(0xA55A0000U)
#define WDTRSTCR_RWDT_RSTMSK		((uint32_t)1U << 0U)
/* MFIS Registers */
#define MFISWPCNTR_PASSWORD		(0xACCE0000U)
#define MFISWPCNTR			(0xE6260900U)
/* IPMMU registers */
#define IPMMU_MM_BASE			(0xE67B0000U)
#define IPMMUMM_IMSCTLR			(IPMMU_MM_BASE + 0x0500U)
#define IPMMUMM_IMAUXCTLR		(IPMMU_MM_BASE + 0x0504U)
#define IPMMUMM_IMSCTLR_ENABLE		(0xC0000000U)
#define IPMMUMM_IMAUXCTLR_NMERGE40_BIT	(0x01000000U)
#define IMSCTLR_DISCACHE		(0xE0000000U)
#define IPMMU_VP0_BASE			(0xFE990000U)
#define IPMMUVP0_IMSCTLR		(IPMMU_VP0_BASE + 0x0500U)
#define IPMMU_VI0_BASE			(0xFEBD0000U)
#define IPMMUVI0_IMSCTLR		(IPMMU_VI0_BASE + 0x0500U)
#define IPMMU_VI1_BASE			(0xFEBE0000U)
#define IPMMUVI1_IMSCTLR		(IPMMU_VI1_BASE + 0x0500U)
#define IPMMU_PV0_BASE			(0xFD800000U)
#define IPMMUPV0_IMSCTLR		(IPMMU_PV0_BASE + 0x0500U)
#define IPMMU_PV1_BASE			(0xFD950000U)
#define IPMMUPV1_IMSCTLR		(IPMMU_PV1_BASE + 0x0500U)
#define IPMMU_PV2_BASE			(0xFD960000U)
#define IPMMUPV2_IMSCTLR		(IPMMU_PV2_BASE + 0x0500U)
#define IPMMU_PV3_BASE			(0xFD970000U)
#define IPMMUPV3_IMSCTLR		(IPMMU_PV3_BASE + 0x0500U)
#define IPMMU_HC_BASE			(0xE6570000U)
#define IPMMUHC_IMSCTLR			(IPMMU_HC_BASE + 0x0500U)
#define IPMMU_RT_BASE			(0xFFC80000U)
#define IPMMURT_IMSCTLR			(IPMMU_RT_BASE + 0x0500U)
#define IPMMU_MP_BASE			(0xEC670000U)
#define IPMMUMP_IMSCTLR			(IPMMU_MP_BASE + 0x0500U)
#define IPMMU_DS0_BASE			(0xE6740000U)
#define IPMMUDS0_IMSCTLR		(IPMMU_DS0_BASE + 0x0500U)
#define IPMMU_DS1_BASE			(0xE7740000U)
#define IPMMUDS1_IMSCTLR		(IPMMU_DS1_BASE + 0x0500U)
/* ARMREG registers */
#define P_ARMREG_SEC_CTRL		(0xE62711F0U)
#define P_ARMREG_SEC_CTRL_PROT		(0x00000001U)
/* MIDR */
#define MIDR_CA57			(0x0D07U << MIDR_PN_SHIFT)
#define MIDR_CA53			(0x0D03U << MIDR_PN_SHIFT)
/* for SuspendToRAM */
#define GPIO_BASE			(0xE6050000U)
#define GPIO_INDT1			(GPIO_BASE + 0x100CU)
#define GPIO_INDT3			(GPIO_BASE + 0x300CU)
#define GPIO_INDT6			(GPIO_BASE + 0x540CU)
#define GPIO_OUTDT1			(GPIO_BASE + 0x1008U)
#define GPIO_OUTDT3			(GPIO_BASE + 0x3008U)
#define GPIO_OUTDT6			(GPIO_BASE + 0x5408U)
#define RCAR_COLD_BOOT			(0x00U)
#define RCAR_WARM_BOOT			(0x01U)
#if PMIC_ROHM_BD9571 && RCAR_SYSTEM_RESET_KEEPON_DDR
#define KEEP10_MAGIC		(0x55U)
#endif
/* lossy registers */
#define LOSSY_PARAMS_BASE		(0x47FD7000U)
#define AXI_DCMPAREACRA0		(0xE6784100U)
#define AXI_DCMPAREACRB0		(0xE6784104U)
#define LOSSY_ENABLE			(0x80000000U)
#define LOSSY_DISABLE			(0x00000000U)
#define LOSSY_FMT_YUVPLANAR		(0x00000000U)
#define LOSSY_FMT_YUV422INTLV		(0x20000000U)
#define LOSSY_FMT_ARGB8888		(0x40000000U)
#define LOSSY_ST_ADDR0			(0x54000000U)
#define LOSSY_END_ADDR0			(0x57000000U)
#define LOSSY_FMT0			LOSSY_FMT_YUVPLANAR
#define LOSSY_ENA_DIS0			LOSSY_ENABLE
#define LOSSY_ST_ADDR1			0x0U
#define LOSSY_END_ADDR1			0x0U
#define LOSSY_FMT1			LOSSY_FMT_ARGB8888
#define LOSSY_ENA_DIS1			LOSSY_DISABLE
#define LOSSY_ST_ADDR2			0x0U
#define LOSSY_END_ADDR2			0x0U
#define LOSSY_FMT2			LOSSY_FMT_YUV422INTLV
#define LOSSY_ENA_DIS2			LOSSY_DISABLE

#endif /* RCAR_DEF_H */
