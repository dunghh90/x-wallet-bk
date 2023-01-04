/*!
 * @skip  $ld:$
 * @file  f_rrh_reg.h
 * @brief レジスタアドレス共通定義ヘッダ
 * @date  2015/3/27 FJT) Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_H
#define F_RRH_REG_H

/*!
 * @name レジスタTOPアドレス(使わないかもしれないが、あると便利)
 * @note レジスタTOPアドレス(使わないかもしれないが、あると便利)
 * @{
 */
#define D_RRH_ARCH					"zynq"				/* addr range                                  */
/***** 5G-DUハソ仕様書対応	*****/
//#define	D_RRH_REG_JSFB			0x00200000			/* addr : size 0x1000	*//* Base Address不明… */
//#define	D_RRH_REG_JSFW			0x00200000			/* addr : size 0x1000	*//* Base Address不明… */
//#define	D_RRH_REG_JSRX			0x00300000			/* addr : size 0x1000	*//* Base Address不明… */
#define	D_RRH_REG_CTX				0xB0000000			/* addr : size 0x1000	*/
#define	D_RRH_REG_CTB				0xA0000000			/* addr : size 0x1000	*/
#define	D_RRH_REG_ETH				0xA1000000			/* addr : size 0x1000	*/
#define	D_RRH_REG_DLF				0xA1000000			/* addr : size 0x1000	*//* 被り[0] */
#define	D_RRH_REG_ULF				0xA1000000			/* addr : size 0x1000	*//* 被り[0] */
#define	D_RRH_REG_TCB				0xA2000000			/* addr : size 0x1000	*/
#define	D_RRH_REG_DLP				0xA2010000			/* addr : size 0x1000	*/
#define	D_RRH_REG_ULP				0xA2020000			/* addr : size 0x1000	*/
#define	D_RRH_REG_DPD				0x82000000			/* addr : size 0x1000	*/
#define	D_RRH_REG_RACH				0xB1300000			/* addr : size 0x1000	*//* 被り[1] */
//#define	D_RRH_REG_RX			0xB1300000			/* addr : size 0x1000	*//* 被り[1] */
#define	D_RRH_REG_TCX				0xB1000000			/* addr : size 0x1000	*/
/***** 3.5G-RRE 4TRXハソ仕様書対応 *****/
#define	D_RRH_REG_CNT				0x40000000			/* addr : size 0x1000  */
#define	D_RRH_REG_CM_RX				0x41000000			/* addr : size 0x40000 */
#define	D_RRH_REG_CM_RX_S			0x41040000			/* addr : size 0x40000 */
#define	D_RRH_REG_CNTS				0x80000000			/* addr : size 0x1000  */
#define	D_RRH_REG_CPHY				0x80010000			/* addr : size 0x1000  */
#define	D_RRH_REG_CPHY_S			0x80011000			/* addr : size 0x1000  */
#define	D_RRH_REG_CATXPLL			0x80012000			/* addr : size 0x1000  */
#define	D_RRH_REG_JESD				0x80020000			/* addr : size 0x10000 */
#define	D_RRH_REG_CPRI				0x81000000			/* addr : size 0x2000  */
#define	D_RRH_REG_CM				0x81002000			/* addr : size 0x800   */
#define	D_RRH_REG_CM_S				0x81002800			/* addr : size 0x800   */
#define	D_RRH_REG_UP				0x81003000			/* addr : size 0x800   */
#define	D_RRH_REG_TD				0x81003800			/* addr : size 0x800   */
#define	D_RRH_REG_CM_TX				0x81010000			/* addr : size 0x4000  */
#define	D_RRH_REG_CMS_TX			0x81014000			/* addr : size 0x4000  */
#define	D_RRH_REG_CM_TX_S			0x81018000			/* addr : size 0x8000  */
#define	D_RRH_REG_DATAGEN			0x81100000			/* addr : size 0x4B00  */
#define	D_RRH_REG_CC				0x82000000			/* addr : size 0x1000  */
#define	D_RRH_REG_PD				0x82001000			/* addr : size 0x1000  */
#define	D_RRH_REG_FB				0x82002000			/* addr : size 0x1000  */
#define	D_RRH_REG_JSFW				0x82003000			/* addr : size 0x1000  */
#define	D_RRH_REG_JSFB				0x82004000			/* addr : size 0x1000  */
#define	D_RRH_REG_RX				0x82100000			/* addr : size 0x1000  */
#define	D_RRH_REG_JSRX				0x82101000			/* addr : size 0x1000  */
#define	D_RRH_REG_SOC				0xF8000000			/* addr : size xxxxxxx */
#define D_RRH_REG_DDR_RX			0x1FC00000			/* addr : size 0x10000 */
#define D_RRH_REG_DDRS_RX			0x1FC20000			/* addr : size 0x10000 */
/* @} */

/*!
 * @name ユーティリティ系マクロ
 * @note ユーティリティ系マクロ (よかったら使ってね系)
 * @{
 */
#define D_RRH_VAL_32BIT( val )							( UINT )( val )
#define D_RRH_VAL_32BIT_M( val , msk )					( UINT )( val & msk )
#define D_RRH_VAL_32BIT_N( val , msk )					( UINT )( val & ~msk )
#define D_RRH_VAL_16BIT( val )							( USHORT )( val )
#define D_RRH_VAL_16BIT_M( val , msk )					( USHORT )( val & msk )
#define D_RRH_VAL_16BIT_N( val , msk )					( USHORT )( val & msk )
#define D_RRH_VAL_8BIT( val )							( UCHAR )( val )
#define D_RRH_VAL_8BIT_M( val , msk )					( UCHAR )( val & msk )
#define D_RRH_VAL_8BIT_N( val , msk )					( UCHAR )( val & ~msk )
#define D_RRH_VAL_SHIFT_L( val , shift )				( UINT )( val << shift )
#define D_RRH_VAL_SHIFT_L_M( val , shift , msk )		( UINT )(( val << shift ) & msk )
#define D_RRH_VAL_SHIFT_L_N( val , shift , msk )		( UINT )(( val << shift ) & ~msk )
#define D_RRH_VAL_SHIFT_R( val , shift )				( UINT )( val >> shift )
#define D_RRH_VAL_SHIFT_R_M( val , shift , msk )		( UINT )(( val >> shift ) & msk )
#define D_RRH_VAL_SHIFT_R_N( val , shift , msk )		( UINT )(( val >> shift ) & ~msk )

/* @} */
#endif
/* @} */
