/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_catxpll
 * @brief レジスタアドレス定義ヘッダ(CATXPLL)
 * @date  2015/3/27 FJT) Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_CATXPLL_H
#define F_RRH_REG_CATXPLL_H

/*!
 * @name CPHY & CPRI ATXPLLレジスタアドレス
 * @note CPHY & CPRI ATXPLLレジスタアドレス
 * @{
 */
#if defined (OPT_RRH_ARCH_A10)
#define	D_RRH_REG_CPHY_RATE1						0xD00100B4 
#define	D_RRH_REG_CPHY_RATE2						0xD0010110
#define	D_RRH_REG_CPHY_RATE3						0xD0010464
#define	D_RRH_REG_CPHY_RATE4						0xD00104E8

#define	D_RRH_REG_CPHY_DCD							0xD0010428
#define	D_RRH_REG_CPHY_VOD1							0xD0010424
#define	D_RRH_REG_CPHY_VOD2							0xD0010414

#define	D_RRH_REG_CPHY_CTLE1						0xD0010468
#define	D_RRH_REG_CPHY_CTLE2						0xD0010470
#define	D_RRH_REG_CPHY_CTLE3						0xD001046C
#define	D_RRH_REG_CPHY_CTLE4						0xD001059C
#define	D_RRH_REG_CPHY_CTLE5						0xD0010580

#define D_RRH_REG_CATXPLL_CAL1						0xD0012000
#define D_RRH_REG_CATXPLL_CAL2						0xD0012400
#elif defined  (OPT_RRH_ARCH_ZYNQ)
#define	D_RRH_REG_CPHY_RATE1						0x8001080C
#define	D_RRH_REG_CPHY_RATE2						0x80010998
#define	D_RRH_REG_CPHY_RATE3						0x8001098C
#define	D_RRH_REG_CPHY_RATE4						0x8001089C
#define	D_RRH_REG_CPHY_RATE5						0x80010840
#define	D_RRH_REG_CPHY_RATE6						0x80010AB4
#define	D_RRH_REG_CPHY_RATE7						0x800109E8
#define	D_RRH_REG_CPHY_RATE8						0x80010A14
#define	D_RRH_REG_CPHY_RATE9						0x800109F0

#define D_RRH_REG_CATXPLL_CAL1						0x80012000
#define D_RRH_REG_CATXPLL_CAL2						0x80012400
#define	D_RRH_REG_CPHY_DCD							0x80010428
#define	D_RRH_REG_CPHY_VOD1							0x80010424
#define	D_RRH_REG_CPHY_VOD2							0x80010414

#define	D_RRH_REG_CPHY_CTLE1						0x80010468
#define	D_RRH_REG_CPHY_CTLE2						0x80010470
#define	D_RRH_REG_CPHY_CTLE3						0x8001046C
#define	D_RRH_REG_CPHY_CTLE4						0x8001059C
#define	D_RRH_REG_CPHY_CTLE5						0x80010580

#define D_RRH_REG_CPHY_CDRCLK						0x80010B18

/* Secondary(Master) */
#define	D_RRH_REG_CPHYS_RATE1						0x8001100C
#define	D_RRH_REG_CPHYS_RATE2						0x80011198
#define	D_RRH_REG_CPHYS_RATE3						0x8001118C
#define	D_RRH_REG_CPHYS_RATE4						0x8001109C
#define	D_RRH_REG_CPHYS_RATE5						0x80011040
#define	D_RRH_REG_CPHYS_RATE6						0x800112B4
#define	D_RRH_REG_CPHYS_RATE7						0x800111E8
#define	D_RRH_REG_CPHYS_RATE8						0x80011214
#define	D_RRH_REG_CPHYS_RATE9						0x800111F0

#else
#endif

#define D_RRH_CATXPLL_CAL1_VAL1						0x00000002
#define D_RRH_CATXPLL_CAL1_VAL2						0x00000003
#define D_RRH_CATXPLL_CAL2_VAL1						0x000000B5

/* @} */
#endif
/* @} */
