/*!
 * @skip  $ld:$
 * @file  f_du_reg_cpi.h
 * @brief レジスタアドレス定義ヘッダ(CPR)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_CPR_H
#define F_DU_REG_CPR_H

/*!
 * @name CPRレジスタアドレス
 * @note CPRレジスタアドレス, この値は直接使用してはいけない
 * @{
 */
#define D_DU_REG_CPR_ULSCS					0xB1102014		/* ULSCS		(sub6 : o / mmw : -)*/
#define D_DU_REG_CPR_ULBW					0xB1102018		/* ULBW			(sub6 : o / mmw : -)*/
#define D_DU_REG_CPR_CR0PRM					0xB1102020		/* CR0PRM		(sub6 : o / mmw : -)*/
#define D_DU_REG_CPR_CR1PRM					0xB1102024		/* CR1PRM		(sub6 : o / mmw : -)*/
#define D_DU_REG_CPR_CPRMVSTPOINT			0xB1102080		/* CPRMVSTPOINT	(sub6 : o / mmw : -)*/
#define D_DU_REG_CPR_CPRMVMODE				0xB1102084		/* CPRMVMODE	(sub6 : o / mmw : -)*/
#define D_DU_REG_CPR_CPRMONSEL				0xB1102300		/* CPRMONSEL	(sub6 : o / mmw : -)*/
#define D_DU_REG_CPR_ESBUFSLP				0xB1102400		/* ESBUFSLP		(sub6 : o / mmw : -)*/
#define D_DU_REG_CPR_ESBUFPINT				0xB1102404		/* ESBUFPINT	(sub6 : o / mmw : -)*/
#define D_DU_REG_CPR_FFTMUX					0xB1102408		/* FFTMUX		(sub6 : o / mmw : -)*/
#define D_DU_REG_CPR_PLSSLP					0xB110240C		/* PLSSLP		(sub6 : o / mmw : -)*/
/* @} */

/*!
 * @name CPRレジスタアドレス算出マクロ
 * @note CPRレジスタアドレス算出マクロ, アドレスはこのマクロを使用する unit は0(TRX-FPGA#0)～1(TRX-FPGA#1)である
 * @{
 */
#define	D_DU_OFT_CPR_UNIT					0x02000000
#define M_DU_REG_CPR_GETADDR(ofs, unit)		(UINT)( ofs  + (unit * D_DU_OFT_CPR_UNIT) )
#define M_DU_REG_CPR_ULSCS					M_DU_REG_CPR_GETADDR(D_DU_REG_CPR_ULSCS, unit)
#define M_DU_REG_CPR_ULBW					M_DU_REG_CPR_GETADDR(D_DU_REG_CPR_ULBW, unit)
#define M_DU_REG_CPR_CR0PRM					M_DU_REG_CPR_GETADDR(D_DU_REG_CPR_CR0PRM, unit)
#define M_DU_REG_CPR_CR1PRM					M_DU_REG_CPR_GETADDR(D_DU_REG_CPR_CR1PRM, unit)
#define M_DU_REG_CPR_CPRMVSTPOINT			M_DU_REG_CPR_GETADDR(D_DU_REG_CPR_CPRMVSTPOINT, unit)
#define M_DU_REG_CPR_CPRMVMODE				M_DU_REG_CPR_GETADDR(D_DU_REG_CPR_CPRMVMODE, unit)
#define M_DU_REG_CPR_CPRMONSEL				M_DU_REG_CPR_GETADDR(D_DU_REG_CPR_CPRMONSEL, unit)
#define M_DU_REG_CPR_ESBUFSLP				M_DU_REG_CPR_GETADDR(D_DU_REG_CPR_ESBUFSLP, unit)
#define M_DU_REG_CPR_ESBUFPINT				M_DU_REG_CPR_GETADDR(D_DU_REG_CPR_ESBUFPINT, unit)
#define M_DU_REG_CPR_FFTMUX					M_DU_REG_CPR_GETADDR(D_DU_REG_CPR_FFTMUX, unit)
#define M_DU_REG_CPR_PLSSLP					M_DU_REG_CPR_GETADDR(D_DU_REG_CPR_PLSSLP, unit)
/* @} */
#endif
/* @} */
