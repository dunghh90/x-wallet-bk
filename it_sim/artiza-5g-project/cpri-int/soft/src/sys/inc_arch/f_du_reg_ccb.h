/*!
 * @skip  $ld:$
 * @file  f_du_reg_ccb.h
 * @brief レジスタアドレス定義ヘッダ(CCB)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_CCB_H
#define F_DU_REG_CCB_H

/*!
 * @name CCBレジスタアドレス
 * @note CCBレジスタアドレス, この値は直接使用してはいけない
 * @{
 */
#define D_DU_REG_CCB_TXCAREN				0xA2100000		/* TXCAREN		(sub6 : o / mmw : o)*/
#define D_DU_REG_CCB_TXBW0					0xA2100010		/* TXBW0		(sub6 : o / mmw : o)*/
#define D_DU_REG_CCB_TXBW1					0xA2100014		/* TXBW1		(sub6 : o / mmw : o)*/
#define D_DU_REG_CCB_TXBW2					0xA2100018		/* TXBW2		(sub6 : o / mmw : o)*/
#define D_DU_REG_CCB_TXBW3					0xA210001C		/* TXBW3		(sub6 : o / mmw : o)*/
#define D_DU_REG_CCB_TXBW4					0xA2100020		/* TXBW4		(sub6 : - / mmw : o)*/
#define D_DU_REG_CCB_TXBW5					0xA2100024		/* TXBW5		(sub6 : - / mmw : o)*/
#define D_DU_REG_CCB_TXBW6					0xA2100028		/* TXBW6		(sub6 : - / mmw : o)*/
#define D_DU_REG_CCB_TXBW7					0xA210002C		/* TXBW7		(sub6 : - / mmw : o)*/
#define D_DU_REG_CCB_TXANTEN				0xA2100040		/* TXANTEN		(sub6 : o / mmw : o)*/
#define D_DU_REG_CCB_TSSI0					0xA2100100		/* TSSI0		(sub6 : o / mmw : o)*/
#define D_DU_REG_CCB_TSSI1					0xA2100104		/* TSSI1		(sub6 : o / mmw : o)*/
#define D_DU_REG_CCB_TSSI2					0xA2100108		/* TSSI2		(sub6 : - / mmw : o)*/
#define D_DU_REG_CCB_TSSI3					0xA210010C		/* TSSI3		(sub6 : - / mmw : o)*/
#define D_DU_REG_CCB_TSSI4					0xA2100110		/* TSSI4		(sub6 : - / mmw : o)*/
#define D_DU_REG_CCB_TSSI5					0xA2100114		/* TSSI5		(sub6 : - / mmw : o)*/
#define D_DU_REG_CCB_TSSI6					0xA2100118		/* TSSI6		(sub6 : - / mmw : o)*/
#define D_DU_REG_CCB_TSSI7					0xA210011C		/* TSSI7		(sub6 : - / mmw : o)*/
#define D_DU_REG_CCB_SSISMPL				0xA2100120		/* SSISMPL		(sub6 : o / mmw : o)*/
#define D_DU_REG_CCB_SSILAT					0xA2100124		/* SSILAT		(sub6 : o / mmw : o)*/
#define D_DU_REG_CCB_SSI					0xA2100128		/* SSI			(sub6 : o / mmw : o)*/
#define D_DU_REG_CCB_CWEN					0xA2100180		/* CWEN			(sub6 : o / mmw : o)*/
#define D_DU_REG_CCB_CWAMPI					0xA2100184		/* CWAMPI		(sub6 : o / mmw : o)*/
#define D_DU_REG_CCB_CWAMPQ					0xA2100188		/* CWAMPQ		(sub6 : o / mmw : o)*/
#define D_DU_REG_CCB_LPF0					0xA21001A0		/* LPF0			(sub6 : o / mmw : -)*/
#define D_DU_REG_CCB_LPF1					0xA21001A4		/* LPF1			(sub6 : o / mmw : -)*/
#define D_DU_REG_CCB_LPFREGEN				0xA21001B0		/* LPFREGEN		(sub6 : o / mmw : -)*/
#define D_DU_REG_CCB_CCTEST					0xA21001F8		/* CCTEST		(sub6 : o / mmw : o)*/
#define D_DU_REG_CCB_CCMONSEL				0xA21001FC		/* CCMONSEL		(sub6 : o / mmw : o)*/
/* @} */

/*!
 * @name CCBレジスタアドレス算出マクロ
 * @note CCBレジスタアドレス算出マクロ, アドレスはこのマクロを使用する Ant は0(Ant#A)～7(Ant#H)である
 * @{
 */
#define	D_DU_OFT_CCB_ANT					0x00000200
#define M_DU_REG_CCB_GETADDR(ofs, ant)		(UINT)(ofs  + (ant * D_DU_OFT_CCB_ANT))
#define M_DU_REG_CCB_TXCAREN(ant)			M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_TXCAREN, ant)
#define M_DU_REG_CCB_TXBW0(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_TXBW0, ant)
#define M_DU_REG_CCB_TXBW1(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_TXBW1, ant)
#define M_DU_REG_CCB_TXBW2(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_TXBW2, ant)
#define M_DU_REG_CCB_TXBW3(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_TXBW3, ant)
#define M_DU_REG_CCB_TXBW4(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_TXBW4, ant)
#define M_DU_REG_CCB_TXBW5(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_TXBW5, ant)
#define M_DU_REG_CCB_TXBW6(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_TXBW6, ant)
#define M_DU_REG_CCB_TXBW7(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_TXBW7, ant)
#define M_DU_REG_CCB_TXANTEN(ant)			M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_TXANTEN, ant)
#define M_DU_REG_CCB_TSSI0(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_TSSI0, ant)
#define M_DU_REG_CCB_TSSI1(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_TSSI1, ant)
#define M_DU_REG_CCB_TSSI2(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_TSSI2, ant)
#define M_DU_REG_CCB_TSSI3(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_TSSI3, ant)
#define M_DU_REG_CCB_TSSI4(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_TSSI4, ant)
#define M_DU_REG_CCB_TSSI5(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_TSSI5, ant)
#define M_DU_REG_CCB_TSSI6(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_TSSI6, ant)
#define M_DU_REG_CCB_TSSI7(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_TSSI7, ant)
#define M_DU_REG_CCB_SSISMPL(ant)			M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_SSISMPL, ant)
#define M_DU_REG_CCB_SSILAT(ant)			M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_SSILAT, ant)
#define M_DU_REG_CCB_SSI(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_SSI, ant)
#define M_DU_REG_CCB_CWEN(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_CWEN, ant)
#define M_DU_REG_CCB_CWAMPI(ant)			M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_CWAMPI, ant)
#define M_DU_REG_CCB_CWAMPQ(ant)			M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_CWAMPQ, ant)
#define M_DU_REG_CCB_LPF0(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_LPF0, ant)
#define M_DU_REG_CCB_LPF1(ant)				M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_LPF1, ant)
#define M_DU_REG_CCB_LPFREGEN(ant)			M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_LPFREGEN, ant)
#define M_DU_REG_CCB_CCTEST(ant)			M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_CCTEST, ant)
#define M_DU_REG_CCB_CCMONSEL(ant)			M_DU_REG_CCB_GETADDR(D_DU_REG_CCB_CCMONSEL, ant)
/* @} */
#endif
/* @} */
