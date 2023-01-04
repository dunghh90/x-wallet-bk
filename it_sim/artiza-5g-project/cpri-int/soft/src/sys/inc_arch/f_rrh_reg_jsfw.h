/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_jsfw.h
 * @brief レジスタアドレス定義ヘッダ(JSFW)
 * @date  2015/3/27 FJT) Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_JSFW_H
#define F_RRH_REG_JSFW_H

#if 0
/***** 5G-DUハソ仕様書対応	*****/
/***** D_RRH_REG_JSFW が仮設定であるため、既存defineを有効化	*****/
/*!
 * @name JSFWレジスタアドレス
 * @note JSFWレジスタアドレス
 * @{
 */
#define	D_RRH_REG_JSFW_JSESRCTR					(D_RRH_REG_JSFW + 0x00003000)	/* External SYSREF Control					*/
#define	D_RRH_REG_JSFW_JSESRLOG					(D_RRH_REG_JSFW + 0x00003004)	/* External SYSREF Log						*/
#define	D_RRH_REG_JSFW_JSFWSRCTR				(D_RRH_REG_JSFW + 0x00003020)	/* Internal SYSREF Control					*/
#define	D_RRH_REG_JSFW_JSFWSYNCCTR				(D_RRH_REG_JSFW + 0x00003030)	/* JSFW XSYNC Control						*/
#define	D_RRH_REG_JSFW_JSFWSYNCMON				(D_RRH_REG_JSFW + 0x00003034)	/* JSFW XSYNC Monitor						*/
#define	D_RRH_REG_JSFW_JSFWEN					(D_RRH_REG_JSFW + 0x00003080)	/* JSFW JESD Enable							*/
#define	D_RRH_REG_JSFW_JSFWRF					(D_RRH_REG_JSFW + 0x00003084)	/* JSFW JESD Use RF Devive Num				*/
#define	D_RRH_REG_JSFW_JSFWCTR					(D_RRH_REG_JSFW + 0x00003090)	/* JSFW JESD Control						*/
#define	D_RRH_REG_JSFW_JSFWDTSEL				(D_RRH_REG_JSFW + 0x00003094)	/* JSFW Initial Data Sesect					*/
#define	D_RRH_REG_JSFW_JSFWRSMD					(D_RRH_REG_JSFW + 0x000030A0)	/* JSFW Re-Synchronaization Mode			*/
#define	D_RRH_REG_JSFW_JSFWCPC0					(D_RRH_REG_JSFW + 0x00003100)	/* JSFW Config Parameter (Common0) 			*/
#define	D_RRH_REG_JSFW_JSFWCPC1					(D_RRH_REG_JSFW + 0x00003104)	/* JSFW Config Parameter (Common1) 			*/
#define	D_RRH_REG_JSFW_JSFWCPC2					(D_RRH_REG_JSFW + 0x00003108)	/* JSFW Config Parameter (Common2) 			*/
#define	D_RRH_REG_JSFW_JSFWCPC3					(D_RRH_REG_JSFW + 0x0000310C)	/* JSFW Config Parameter (Common3) 			*/
#define	D_RRH_REG_JSFW_JSFWCPL0					(D_RRH_REG_JSFW + 0x00003110)	/* JSFW Config Parameter (Lane0) 			*/
#define	D_RRH_REG_JSFW_JSFWCPL1					(D_RRH_REG_JSFW + 0x00003114)	/* JSFW Config Parameter (Lane1) 			*/
#define	D_RRH_REG_JSFW_JSFWCPL2					(D_RRH_REG_JSFW + 0x00003118)	/* JSFW Config Parameter (Lane2) 			*/
#define	D_RRH_REG_JSFW_JSFWCPL3					(D_RRH_REG_JSFW + 0x0000311C)	/* JSFW Config Parameter (Lane3) 			*/
#define	D_RRH_REG_JSFW_JSFWCPL4					(D_RRH_REG_JSFW + 0x00003120)	/* JSFW Config Parameter (Lane4) 			*/
#define	D_RRH_REG_JSFW_JSFWCPL5					(D_RRH_REG_JSFW + 0x00003124)	/* JSFW Config Parameter (Lane5) 			*/
#define	D_RRH_REG_JSFW_JSFWCPL6					(D_RRH_REG_JSFW + 0x00003128)	/* JSFW Config Parameter (Lane6) 			*/
#define	D_RRH_REG_JSFW_JSFWCPL7					(D_RRH_REG_JSFW + 0x0000312C)	/* JSFW Config Parameter (Lane7) 			*/
#define	D_RRH_REG_JSFW_JSFWXBAR					(D_RRH_REG_JSFW + 0x00003200)	/* JSFW Antena Select						*/
#define	D_RRH_REG_JSFW_JSFWST					(D_RRH_REG_JSFW + 0x00003300)	/* JSFW Status								*/
#define	D_RRH_REG_JSFW_JSFWPFST					(D_RRH_REG_JSFW + 0x00003304)	/* JSFW PHY FIFO Status						*/
#define	D_RRH_REG_JSFW_JSFWPUL					(D_RRH_REG_JSFW + 0x0000330C)	/* JSFW PLL Status							*/
#define	D_RRH_REG_JSFW_JSFWERC1					(D_RRH_REG_JSFW + 0x00003360)	/* JRFIC-1系対向XSYNC↓エッジ検出回数		*/
#define	D_RRH_REG_JSFW_JSFWERC2					(D_RRH_REG_JSFW + 0x00003364)	/* RFIC-2系対向XSYNC↓エッジ検出回数		*/
#define	D_RRH_REG_JSFW_JSFWSTIM					(D_RRH_REG_JSFW + 0x00003380)	/* JSFW XSYNC Input Timing					*/
#define	D_RRH_REG_JSFW_JSFWIRQ					(D_RRH_REG_JSFW + 0x00003400)	/* JSFW IRQ									*/
#define	D_RRH_REG_JSFW_JSFWIRQC					(D_RRH_REG_JSFW + 0x00003404)	/* JSFW IRQ									*/
#define	D_RRH_REG_JSFW_JSFWIRQM					(D_RRH_REG_JSFW + 0x00003408)	/* JSFW IRQ									*/
#define	D_RRH_REG_JSFW_JSFWLMFC					(D_RRH_REG_JSFW + 0x00003500)	/* JSFW Timing Offset						*/
#define	D_RRH_REG_JSFW_JSFWSTMON				(D_RRH_REG_JSFW + 0x00003F00)	/* JSFW State Monitor						*/
#define	D_RRH_REG_JSFW_JSFWSTLOG				(D_RRH_REG_JSFW + 0x00003F04)	/* JSFW State Change Log					*/
#define	D_RRH_REG_JSFW_JSFWILASCNT				(D_RRH_REG_JSFW + 0x00003F08)	/* JSFW ILAS Count							*/
#define	D_RRH_REG_JSFW_JSFWTEST					(D_RRH_REG_JSFW + 0x00003FF0)	/* JSFW TEST								*/
#define	D_RRH_REG_JSFW_JSFWMSEL					(D_RRH_REG_JSFW + 0x00003FFC)	/* JSFW Monitor								*/
/* @} */

/*!
 * @name JSFWレジスタbit & Shift
 * @note JSFWレジスタbit & Shift
 * @{
 */
/* @} */
#else
/*!
 * @name JSFWレジスタアドレス
 * @note JSFWレジスタアドレス
 * @{
 */
#if defined (OPT_RRH_ARCH_A10)
#define	D_RRH_REG_JSFW_JSESRCTR						0xD2003000
#define	D_RRH_REG_JSFW_JSESRLOG						0xD2003004
#define	D_RRH_REG_JSFW_JSFWSRCTR					0xD2003020
#define	D_RRH_REG_JSFW_JSFWSYNCCTR					0xD2003030
#define	D_RRH_REG_JSFW_JSFWSYNCMON					0xD2003034
#define	D_RRH_REG_JSFW_JSFWEN						0xD2003080
#define	D_RRH_REG_JSFW_JSFWRF						0xD2003084
#define	D_RRH_REG_JSFW_JSFWCTR						0xD2003090
#define	D_RRH_REG_JSFW_JSFWDTSEL					0xD2003094
#define	D_RRH_REG_JSFW_JSFWRSMD						0xD20030A0
#define	D_RRH_REG_JSFW_JSFWCPC0						0xD2003100
#define	D_RRH_REG_JSFW_JSFWCPC1						0xD2003104
#define	D_RRH_REG_JSFW_JSFWCPC2						0xD2003108
#define	D_RRH_REG_JSFW_JSFWCPC3						0xD200310C
#define	D_RRH_REG_JSFW_JSFWCPL0						0xD2003110
#define	D_RRH_REG_JSFW_JSFWCPL1						0xD2003114
#define	D_RRH_REG_JSFW_JSFWCPL2						0xD2003118
#define	D_RRH_REG_JSFW_JSFWCPL3						0xD200311C
#define	D_RRH_REG_JSFW_JSFWCPL4						0xD2003120
#define	D_RRH_REG_JSFW_JSFWCPL5						0xD2003124
#define	D_RRH_REG_JSFW_JSFWCPL6						0xD2003128
#define	D_RRH_REG_JSFW_JSFWCPL7						0xD200312C
#define	D_RRH_REG_JSFW_JSFWXBAR						0xD2003200
#define	D_RRH_REG_JSFW_JSFWST						0xD2003300
#define	D_RRH_REG_JSFW_JSFWPFST						0xD2003304
#define	D_RRH_REG_JSFW_JSFWPUL						0xD200330C
#define	D_RRH_REG_JSFW_JSFWERC1						0xD2003360
#define	D_RRH_REG_JSFW_JSFWERC2						0xD2003364
#define	D_RRH_REG_JSFW_JSFWSTIM						0xD2003380
#define	D_RRH_REG_JSFW_JSFWIRQ						0xD2003400
#define	D_RRH_REG_JSFW_JSFWIRQC						0xD2003404
#define	D_RRH_REG_JSFW_JSFWIRQM						0xD2003408
#define	D_RRH_REG_JSFW_JSFWLMFC						0xD2003500
#define	D_RRH_REG_JSFW_JSFWSTMON					0xD2003F00
#define	D_RRH_REG_JSFW_JSFWSTLOG					0xD2003F04
#define	D_RRH_REG_JSFW_JSFWILASCNT					0xD2003F08
#define	D_RRH_REG_JSFW_JSFWTEST						0xD2003FF0
#define	D_RRH_REG_JSFW_JSFWMSEL						0xD2003FFC
#elif defined (OPT_RRH_ARCH_ZYNQ)
#define	D_RRH_REG_JSFW_JSESRCTR						0x82003000
#define	D_RRH_REG_JSFW_JSESRLOG						0x82003004
#define	D_RRH_REG_JSFW_JSFWSRCTR					0x82003020
#define	D_RRH_REG_JSFW_JSFWSYNCCTR					0x82003030
#define	D_RRH_REG_JSFW_JSFWSYNCMON					0x82003034
#define	D_RRH_REG_JSFW_JSFWEN						0x82003080
#define	D_RRH_REG_JSFW_JSFWRF						0x82003084
#define	D_RRH_REG_JSFW_JSFWCTR						0x82003090
#define	D_RRH_REG_JSFW_JSFWDTSEL					0x82003094
#define	D_RRH_REG_JSFW_JSFWRSMD						0x820030A0
#define	D_RRH_REG_JSFW_JSFWCPC0						0x82003100
#define	D_RRH_REG_JSFW_JSFWCPC1						0x82003104
#define	D_RRH_REG_JSFW_JSFWCPC2						0x82003108
#define	D_RRH_REG_JSFW_JSFWCPC3						0x8200310C
#define	D_RRH_REG_JSFW_JSFWCPL0						0x82003110
#define	D_RRH_REG_JSFW_JSFWCPL1						0x82003114
#define	D_RRH_REG_JSFW_JSFWCPL2						0x82003118
#define	D_RRH_REG_JSFW_JSFWCPL3						0x8200311C
#define	D_RRH_REG_JSFW_JSFWCPL4						0x82003120
#define	D_RRH_REG_JSFW_JSFWCPL5						0x82003124
#define	D_RRH_REG_JSFW_JSFWCPL6						0x82003128
#define	D_RRH_REG_JSFW_JSFWCPL7						0x8200312C
#define	D_RRH_REG_JSFW_JSFWXBAR						0x82003200
#define	D_RRH_REG_JSFW_JSFWST						0x82003300
#define	D_RRH_REG_JSFW_JSFWPFST						0x82003304
#define	D_RRH_REG_JSFW_JSFWPUL						0x8200330C
#define	D_RRH_REG_JSFW_JSFWERC1						0x82003360
#define	D_RRH_REG_JSFW_JSFWERC2						0x82003364
#define	D_RRH_REG_JSFW_JSFWSTIM						0x82003380
#define	D_RRH_REG_JSFW_JSFWIRQ						0x82003400
#define	D_RRH_REG_JSFW_JSFWIRQC						0x82003404
#define	D_RRH_REG_JSFW_JSFWIRQM						0x82003408
#define	D_RRH_REG_JSFW_JSFWLMFC						0x82003500
#define	D_RRH_REG_JSFW_JSFWSTMON					0x82003F00
#define	D_RRH_REG_JSFW_JSFWSTLOG					0x82003F04
#define	D_RRH_REG_JSFW_JSFWILASCNT					0x82003F08
#define	D_RRH_REG_JSFW_JSFWTEST						0x82003FF0
#define	D_RRH_REG_JSFW_JSFWMSEL						0x82003FFC
#else
#endif
/* @} */

/*!
 * @name レジスタ値
 * @note レジスタ値
 * @{
 */
/* JSFWIRQC */
#define	D_RRH_JSFW_JSFWIRQC_PLLE					0x00000100
#define	D_RRH_JSFW_JSFWIRQC_FIFOE2					0x00000020
#define	D_RRH_JSFW_JSFWIRQC_FIFOE1					0x00000010
#define	D_RRH_JSFW_JSFWIRQC_LDN2					0x00000002
#define	D_RRH_JSFW_JSFWIRQC_LDN1					0x00000001
#define	D_RRH_JSFW_JSFWIRQC_FWJESD_ALM_AB			( D_RRH_JSFW_JSFWIRQC_FIFOE1 | D_RRH_JSFW_JSFWIRQC_LDN1 )
#define	D_RRH_JSFW_JSFWIRQC_FWJESD_ALM_CD			( D_RRH_JSFW_JSFWIRQC_FIFOE2 | D_RRH_JSFW_JSFWIRQC_LDN2 )
/* @} */
#endif
#endif
/* @} */
