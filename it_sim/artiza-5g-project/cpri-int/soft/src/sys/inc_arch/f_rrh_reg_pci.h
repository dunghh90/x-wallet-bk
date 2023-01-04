/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_pci.h
 * @brief レジスタアドレス定義ヘッダ(PCI)
 * @date  2015/6/18 ALP) Chikama Created.
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_PCI_H
#define F_RRH_REG_PCI_H

/*!
 * @name レジスタアドレス(PCI)
 * @note レジスタアドレス(PCI)
 * @{
 */
#if defined (OPT_RRH_ARCH_A10)
#elif defined  (OPT_RRH_ARCH_ZYNQ)

#define D_RRH_REG_PCI_CMD_REG					0xB0000004	/* PHY Status/Control Register											*/
#define D_RRH_REG_PCI_SLT_BUSNUM				0xB0000018	/* Second Lat Timer/Sub Bus Number/Second Bus Number/Primary Bus Number	*/
#define D_RRH_REG_PCI_BRDG_STS_CTL				0xB0000134	/* Bridge Status and Control Register									*/
#define D_RRH_REG_PCI_INTRPT_DR					0xB0000138	/* Interrupt Decode Register											*/
#define D_RRH_REG_PCI_INTRPT_MR					0xB000013C	/* Interrupt Mask Register												*/
#define D_RRH_REG_PCI_PHY_STS_CTL				0xB0000144	/* PHY Status/Control Register											*/
#define D_RRH_REG_PCI_ROOTPORT_STS_CTL			0xB0000148	/* Root Port Status/Control Register									*/
#define D_RRH_REG_PCI_ROOTPORT_ERR_FIFO			0xB0000154	/* Root Port Error FIFO Read Register									*/
#define D_RRH_REG_PCI_ROOTPORT_INT_FIFO1		0xB0000158	/* Root Port Interrupt FIFO Read Register 1								*/
#define D_RRH_REG_PCI_ROOTPORT_INT_FIFO2		0xB000015C	/* Root Port Interrupt FIFO Read Register 2								*/
#define D_RRH_REG_PCI_BIST_HEAD_LT_CL			0xB010000C	/* BIST/Header/Lat Timer/Cache Ln										*/
#define D_RRH_REG_PCI_STATE_CMD					0xB0100004	/* Status/Command														*/
#define D_RRH_REG_PCI_BASE_ADDREG				0xB0100010	/* Base Address Register 0												*/

#define D_RRH_BIT_PCI_INTRPT_MR					0x00000000	/* 初期化 全Interrupt Disable設定										*/
#define D_RRH_BIT_PCI_INTRPT_DR					0x1FF30FEF	/* 初期化 全Interrupt クリア設定										*/
#define D_RRH_BIT_PCI_PHY_STS_CTL_LINKUP		0x00000800	/* PHY Status/Control Register(Linkup)									*/
#define D_RRH_BIT_PCI_CMD_REG					0x00000147	/* Command Register														*/
#define D_RRH_BIT_PCI_SLT_BUSNUM				0xFFFF0100	/* Second Lat Timer/Sub Bus Number/Second Bus Number/Primary Bus Number	*/
#define D_RRH_BIT_PCI_BIST_HEAD_LT_CL			0x00EF0000	/* BIST/Header/Lat Timer/Cache Ln										*/
#define D_RRH_BIT_PCI_STATE_CMD					0x00000006	/* Status/Command														*/
#define D_RRH_BIT_PCI_BASE_ADDREG				0x00000000	/* Base Address Register 0												*/
#define D_RRH_BIT_PCI_ROOTPORT_STS_CTL_BRGEN	0x00000001	/* Root Port Status/Control Register(Bridge Enable)						*/
#define D_RRH_BIT_PCI_INTRPT_MR2				0x1FF00FEF	/* 初期化 全Interrupt Enable設定([D17:16]を除く))						*/

#else
#endif

/* @} */
#endif
/* @} */
