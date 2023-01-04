/*!
 * @skip  $ld:$
 * @file  f_du_reg.h
 * @brief レジスタアドレス共通定義ヘッダ
 * @date  2015/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_H
#define F_DU_REG_H

/*!
 * @name レジスタTOPアドレス(使わないかもしれないが、あると便利)
 * @note レジスタTOPアドレス(使わないかもしれないが、あると便利)
 * @{
 */
#define	D_DU_REG_CCB_BASE			0xA2100000			/* CCB		(sub6:o/mmW:o)	*/
#define	D_DU_REG_CTB_BASE			0xA0000000			/* CTB		(sub6:o/mmW:-)	*/
#define	D_DU_REG_DLP_BASE			0xA2010000			/* DLP		(sub6:o/mmW:o)	*/
#define	D_DU_REG_ETH_BASE			0xA1000000			/* ETH		(sub6:o/mmW:o)	*/
#define	D_DU_REG_RXB_BASE			0xB1300000			/* RXB		(sub6:o/mmW:o)	*/
#define	D_DU_REG_TCB_BASE			0xA2000000			/* TCP		(sub6:o/mmW:o)	*/
#define	D_DU_REG_ULP_BASE			0xA2200000			/* ULP		(sub6:o/mmW:o)	*/
#define	D_DU_REG_CA_BASE			0xB1100000			/* CA		(sub6:o/mmW:-)	*/
#define	D_DU_REG_CPI_BASE			0xB1100000			/* CPI		(sub6:o/mmW:-)	*/
#define	D_DU_REG_CPR_BASE			0xB1100000			/* CPR		(sub6:o/mmW:-)	*/
#define	D_DU_REG_CC_BASE			0xA2100000			/* CC		(sub6:-/mmW:o)	*/
#define	D_DU_REG_CTX_BASE			0xB0010000			/* CTX		(sub6:o/mmW:-)	*/
#define	D_DU_REG_FBANT_BASE			0xB2100000			/* FBANT	(sub6:o/mmW:-)	*/
#define	D_DU_REG_FBATC_BASE			0xB1200000			/* FBATC	(sub6:o/mmW:-)	*/
#define	D_DU_REG_FBMOD_BASE			0xB1200000			/* FBMOD	(sub6:o/mmW:-)	*/
#define	D_DU_REG_PD_BASE			0xB1200000			/* PD		(sub6:o/mmW:-)	*/
#define	D_DU_REG_RAFFT_BASE			0xA1000000			/* RAFFT	(sub6:o/mmW:-)	*/
#define	D_DU_REG_RAPKT_BASE			0xB1400000			/* RAPKT	(sub6:o/mmW:o)	*/
#define	D_DU_REG_RX_BASE			0xB1300000			/* RX		(sub6:o/mmW:o)	*/
#define	D_DU_REG_TCX_BASE			0xB1000000			/* TCX		(sub6:o/mmW:o)	*/
#define	D_DU_REG_JSFB_BASE			0xB1200000			/* JSFB		(sub6:o/mmW:-)	*/
#define	D_DU_REG_JSFW_BASE			0xB1200000			/* JSFW		(sub6:o/mmW:o)	*/

#define	D_DU_REG_LUT_BASE			0xB1220000			/* LUT		(sub6:o/mmW:-)	*/
#define	D_DU_REG_JSRX_BASE			0xB1300000			/* JSRX		(sub6:o/mmW:o)	*/

#define	D_DU_REG_DLF_SB6_BASE		0xA1000000			/* DLF		(sub6:o/mmW:-)	*/
#define	D_DU_REG_DLF_MMW_BASE		0xA2100000			/* DLF		(sub6:-/mmW:o)	*/

#define	D_DU_REG_ULF_SB6_BASE		0xA2020000			/* ULF		(sub6:o/mmW:-)	*/
#define	D_DU_REG_ULF_MMW_BASE		0xA1000000			/* ULF		(sub6:-/mmW:o)	*/

#define	D_DU_REG_RAFIR_SB6_BASE		0xB1300000			/* RAFIR	(sub6:o/mmW:-)	*/
#define	D_DU_REG_RAFIR_MMW_BASE		0xB1400000			/* RAFIR	(sub6:-/mmW:o)	*/

/* @} */

/*!
 * @name ユーティリティ系マクロ
 * @note ユーティリティ系マクロ (よかったら使ってね系)
 * @{
 */
#define D_DU_VAL_32BIT( val )							( UINT )( val )
#define D_DU_VAL_32BIT_M( val , msk )					( UINT )( val & msk )
#define D_DU_VAL_32BIT_N( val , msk )					( UINT )( val & ~msk )
#define D_DU_VAL_16BIT( val )							( USHORT )( val )
#define D_DU_VAL_16BIT_M( val , msk )					( USHORT )( val & msk )
#define D_DU_VAL_16BIT_N( val , msk )					( USHORT )( val & msk )
#define D_DU_VAL_8BIT( val )							( UCHAR )( val )
#define D_DU_VAL_8BIT_M( val , msk )					( UCHAR )( val & msk )
#define D_DU_VAL_8BIT_N( val , msk )					( UCHAR )( val & ~msk )
#define D_DU_VAL_SHIFT_L( val , shift )					( UINT )( val << shift )
#define D_DU_VAL_SHIFT_L_M( val , shift , msk )			( UINT )(( val << shift ) & msk )
#define D_DU_VAL_SHIFT_L_N( val , shift , msk )			( UINT )(( val << shift ) & ~msk )
#define D_DU_VAL_SHIFT_R( val , shift )					( UINT )( val >> shift )
#define D_DU_VAL_SHIFT_R_M( val , shift , msk )			( UINT )(( val >> shift ) & msk )
#define D_DU_VAL_SHIFT_R_N( val , shift , msk )			( UINT )(( val >> shift ) & ~msk )

/* @} */
#endif
/* @} */
