/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_jesd
 * @brief レジスタアドレス定義ヘッダ(JESD)
 * @date  2015/3/27 FJT) Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_JESD_H
#define F_RRH_REG_JESD_H

/*!
 * @name JESD Swing voltage/Emphasis設定レジスタアドレス
 * @note JESD Swing voltage/Emphasis設定レジスタアドレス
 * @{
 */
#if defined (OPT_RRH_ARCH_A10)
#define D_RRH_REG_JESD_FW_AB_JESD0_PN1          0xD0026414
#define D_RRH_REG_JESD_FW_AB_JESD0_PN2          0xD0026424
#define D_RRH_REG_JESD_FW_AB_JESD1_PN1          0xD0027414
#define D_RRH_REG_JESD_FW_AB_JESD1_PN2          0xD0027424
#define D_RRH_REG_JESD_FW_AB_JESD2_PN1          0xD0028414
#define D_RRH_REG_JESD_FW_AB_JESD2_PN2          0xD0028424
#define D_RRH_REG_JESD_FW_AB_JESD3_PN1          0xD0029414
#define D_RRH_REG_JESD_FW_AB_JESD3_PN2          0xD0029424
#define D_RRH_REG_JESD_FW_CD_JESD0_PN1          0xD002A414
#define D_RRH_REG_JESD_FW_CD_JESD0_PN2          0xD002A424
#define D_RRH_REG_JESD_FW_CD_JESD1_PN1          0xD002B414
#define D_RRH_REG_JESD_FW_CD_JESD1_PN2          0xD002B424
#define D_RRH_REG_JESD_FW_CD_JESD2_PN1          0xD002C414
#define D_RRH_REG_JESD_FW_CD_JESD2_PN2          0xD002C424
#define D_RRH_REG_JESD_FW_CD_JESD3_PN1          0xD002D414
#define D_RRH_REG_JESD_FW_CD_JESD3_PN2          0xD002D424
#elif defined (OPT_RRH_ARCH_ZYNQ)
#define D_RRH_REG_JESD_FW_AB_JESD0_PN1          0x80026414
#define D_RRH_REG_JESD_FW_AB_JESD0_PN2          0x80026424
#define D_RRH_REG_JESD_FW_AB_JESD1_PN1          0x80027414
#define D_RRH_REG_JESD_FW_AB_JESD1_PN2          0x80027424
#define D_RRH_REG_JESD_FW_AB_JESD2_PN1          0x80028414
#define D_RRH_REG_JESD_FW_AB_JESD2_PN2          0x80028424
#define D_RRH_REG_JESD_FW_AB_JESD3_PN1          0x80029414
#define D_RRH_REG_JESD_FW_AB_JESD3_PN2          0x80029424
#define D_RRH_REG_JESD_FW_CD_JESD0_PN1          0x8002A414
#define D_RRH_REG_JESD_FW_CD_JESD0_PN2          0x8002A424
#define D_RRH_REG_JESD_FW_CD_JESD1_PN1          0x8002B414
#define D_RRH_REG_JESD_FW_CD_JESD1_PN2          0x8002B424
#define D_RRH_REG_JESD_FW_CD_JESD2_PN1          0x8002C414
#define D_RRH_REG_JESD_FW_CD_JESD2_PN2          0x8002C424
#define D_RRH_REG_JESD_FW_CD_JESD3_PN1          0x8002D414
#define D_RRH_REG_JESD_FW_CD_JESD3_PN2          0x8002D424
#else
#endif

#define D_RRH_JESD_FW_PN_VAL1              0x00000046
#define D_RRH_JESD_FW_PN_VAL2              0x00000075
/* @} */

/*!
 * @name JESD ATXPLLレジスタアドレス
 * @note JESD ATXPLLレジスタアドレス
 * @{
 */
#if defined (OPT_RRH_ARCH_A10)
#define D_RRH_REG_JESD_ATXPLL1					0xD002E000
#define D_RRH_REG_JESD_ATXPLL2					0xD002E400

#elif defined (OPT_RRH_ARCH_ZYNQ)
#define D_RRH_REG_JESD_ATXPLL1					0x8002E000
#define D_RRH_REG_JESD_ATXPLL2					0x8002E400
#else
#endif

#define D_RRH_JESD_ATXPLL1_VAL1						0x00000002
#define D_RRH_JESD_ATXPLL1_VAL2						0x00000003
#define D_RRH_JESD_ATXPLL2_VAL1						0x000000B5

/* @} */
#endif
/* @} */
