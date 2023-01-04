/*!
 * @skip  $ld:$
 * @file  f_du_reg_cpi.h
 * @brief レジスタアドレス定義ヘッダ(CPI)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_CPI_H
#define F_DU_REG_CPI_H

/*!
 * @name CPIレジスタアドレス
 * @note CPIレジスタアドレス, この値は直接使用してはいけない
 * @{
 */
#define D_DU_REG_CPI_DLSCS					0xB1101014		/* DLSCS		(sub6 : o / mmw : -)*/
#define D_DU_REG_CPI_DLBW					0xB1101018		/* DLBW			(sub6 : o / mmw : -)*/
#define D_DU_REG_CPI_CR0PRM					0xB1101020		/* CR0PRM		(sub6 : o / mmw : -)*/
#define D_DU_REG_CPI_CR1PRM					0xB1101024		/* CR1PRM		(sub6 : o / mmw : -)*/
#define D_DU_REG_CPI_CPIMONSEL				0xB1101200		/* CPIMONSEL	(sub6 : o / mmw : -)*/
#define D_DU_REG_CPI_OVLOVFLW				0xB1101400		/* OVLOVFLW		(sub6 : o / mmw : -)*/
#define D_DU_REG_CPI_PLSSLP					0xB1101404		/* PLSSLP		(sub6 : o / mmw : -)*/
/* @} */

/*!
 * @name CPIレジスタアドレス算出マクロ
 * @note CPIレジスタアドレス算出マクロ, アドレスはこのマクロを使用する unit は0(TRX-FPGA#0)～1(TRX-FPGA#1)である
 * @{
 */
#define	D_DU_OFT_CPI_UNIT					0x00000000
#define M_DU_REG_CPI_GETADDR(ofs, unit)		(UINT)( ofs  + (unit * D_DU_OFT_CPI_UNIT) )
#define M_DU_REG_CPI_DLSCS(unit)			M_DU_REG_CPI_GETADDR(D_DU_REG_CPI_DLSCS, unit)
#define M_DU_REG_CPI_DLBW(unit)				M_DU_REG_CPI_GETADDR(D_DU_REG_CPI_DLBW, unit)
#define M_DU_REG_CPI_CR0PRM(unit)			M_DU_REG_CPI_GETADDR(D_DU_REG_CPI_CR0PRM, unit)
#define M_DU_REG_CPI_CR1PRM(unit)			M_DU_REG_CPI_GETADDR(D_DU_REG_CPI_CR1PRM, unit)
#define M_DU_REG_CPI_CPIMONSEL(unit)		M_DU_REG_CPI_GETADDR(D_DU_REG_CPI_CPIMONSEL, unit)
#define M_DU_REG_CPI_OVLOVFLW(unit)			M_DU_REG_CPI_GETADDR(D_DU_REG_CPI_OVLOVFLW, unit)
#define M_DU_REG_CPI_PLSSLP(unit)			M_DU_REG_CPI_GETADDR(D_DU_REG_CPI_PLSSLP, unit)
/* @} */
#endif
/* @} */
