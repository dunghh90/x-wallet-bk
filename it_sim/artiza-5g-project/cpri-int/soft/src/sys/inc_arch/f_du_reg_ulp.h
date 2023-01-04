/*!
 * @skip  $ld:$
 * @file  f_du_reg_ulp.h
 * @brief レジスタアドレス定義ヘッダ(ULP)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_ULP_H
#define F_DU_REG_ULP_H

/*!
 * @name ULPレジスタアドレス
 * @note ULPレジスタアドレス, sub6とmmWでdesineが異なるので注意！
 * @{
 */
#define D_DU_REG_ULP_ULPEN					0xA2200000		/* ULPEN		(sub6 : o / mmw : o)*/
#define D_DU_REG_ULP_UPBITW					0xA2200010		/* UPBITW		(sub6 : o / mmw : o)*/
#define D_DU_REG_ULP_ULSCS					0xA2200014		/* ULSCS		(sub6 : o / mmw : o)*/
#define D_DU_REG_ULP_ULBW					0xA2200018		/* ULBW			(sub6 : o / mmw : o)*/
#define D_DU_REG_ULP_CRPRM					0xA2200020		/* CRPRM		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULP_CR0PRM					0xA2200020		/* CR0PRM		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULP_CR1PRM					0xA2200024		/* CR1PRM		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULP_A_PRM					0xA2200100		/* A%PRM		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULP_0_PRM					0xA2200100		/* %0PRM		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULP_1_PRM					0xA2200140		/* %1PRM		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULP_B_PRM					0xA2200180		/* B%PRM		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULP_PMONLTCRALL			0xa2200700
#define D_DU_REG_ULP_PMONLTCRCOM			0xa2200704
#define D_DU_REG_ULP_PMONLTALL				0xa2200740
#define D_DU_REG_ULP_PMONLTCOM				0xa2200744
#define D_DU_REG_ULP_PME1SNDUP				0xA2200800		/* PME1SNDUP	(sub6 : o / mmw : o)*/
#define D_DU_REG_ULP_PME2SNDUP				0xA2200880		/* PME2SNDUP	(sub6 : o / mmw : o)*/
#define D_DU_REG_ULP_ULPCAPEN				0xA2200D00		/* ULPCAPEN		(sub6 : o / mmw : o)*/
#define D_DU_REG_ULP_ULPE1CAPPRM0			0xA2200D10		/* ULPE1CAPPRM0	(sub6 : o / mmw : o)*/
#define D_DU_REG_ULP_ULPE1CAPPRM1			0xA2200D14		/* ULPE1CAPPRM1	(sub6 : o / mmw : o)*/
#define D_DU_REG_ULP_ULPE2CAPPRM0			0xA2200D20		/* ULPE2CAPPRM0	(sub6 : o / mmw : o)*/
#define D_DU_REG_ULP_ULPE2CAPPRM1			0xA2200D24		/* ULPE2CAPPRM1	(sub6 : o / mmw : o)*/
#define D_DU_REG_ULP_ULPTEST1				0xA2200E00		/* ULPTEST1		(sub6 : o / mmw : o)*/
#define D_DU_REG_ULP_ULPTEST2				0xA2200E04		/* ULPTEST2		(sub6 : o / mmw : o)*/
#define D_DU_REG_ULP_SECOVF					0xA2200F00		/* SECOVF		(sub6 : o / mmw : o)*/
#define D_DU_REG_ULP_CAROVF					0xA2200F04		/* CAROVF		(sub6 : o / mmw : o)*/
#define D_DU_REG_ULP_SBBUFOVF				0xA2200F08		/* SBBUFOVF		(sub6 : o / mmw : o)*/
#define D_DU_REG_ULP_RABUFOVF				0xA2200F0C		/* RABUFOVF		(sub6 : o / mmw : o)*/
#define D_DU_REG_ULP_UPRDCONF				0xA2200F10		/* UPRDCONF		(sub6 : o / mmw : o)*/
#define D_DU_REG_ULP_ULPMONSEL				0xA2200FFC		/* ULPMONSEL	(sub6 : o / mmw : o)*/
/* @} */
#endif
/* @} */


