/*!
 * @skip  $ld:$
 * @file  f_du_reg_cc.h
 * @brief レジスタアドレス定義ヘッダ(CC)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_CC_H
#define F_DU_REG_CC_H

/*!
 * @name CCレジスタアドレス
 * @note CCレジスタアドレス, この値は直接使用してはいけない
 * @{
 */
#define D_DU_REG_CC_TXCAREN					0xA2100000		/* TXCAREN		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXBW0					0xA2100010		/* TXBW0		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXBW1					0xA2100014		/* TXBW1		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXBW2					0xA2100018		/* TXBW2		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXBW3					0xA210001C		/* TXBW3		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXBW4					0xA2100020		/* TXBW4		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXBW5					0xA2100024		/* TXBW5		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXBW6					0xA2100028		/* TXBW6		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXBW7					0xA210002C		/* TXBW7		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXANTEN					0xA2100040		/* TXANTEN		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_LPFGAIN0				0xA2100100		/* LPFGAIN0		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_LPFGAIN1				0xA2100104		/* LPFGAIN1		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_LPFGAIN2				0xA2100108		/* LPFGAIN2		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_LPFGAIN3				0xA210010C		/* LPFGAIN3		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_LPFGAIN4				0xA2100110		/* LPFGAIN4		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_LPFGAIN5				0xA2100114		/* LPFGAIN5		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_LPFGAIN6				0xA2100118		/* LPFGAIN6		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_LPFGAIN7				0xA210011C		/* LPFGAIN7		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXNCO0					0xA2100120		/* TXNCO0		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXNCO1					0xA2100124		/* TXNCO1		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXNCO2					0xA2100128		/* TXNCO2		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXNCO3					0xA210012C		/* TXNCO3		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXNCO4					0xA2100130		/* TXNCO4		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXNCO5					0xA2100134		/* TXNCO5		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXNCO6					0xA2100138		/* TXNCO6		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXNCO7					0xA210013C		/* TXNCO7		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXNCOPH0				0xA21001F0		/* TXNCOPH0		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXNCOPH1				0xA21001F4		/* TXNCOPH1		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXNCOPH2				0xA21001F8		/* TXNCOPH2		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXNCOPH3				0xA21001FC		/* TXNCOPH3		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXNCOPH4				0xA2100200		/* TXNCOPH4		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXNCOPH5				0xA2100204		/* TXNCOPH5		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXNCOPH6				0xA2100208		/* TXNCOPH6		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TXNCOPH7				0xA210020C		/* TXNCOPH7		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TSSIMON					0xA2100240		/* TSSIMON		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TSSISMPL				0xA2100280		/* TSSISMPL		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TSSILAT					0xA2100284		/* TSSILAT		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_TSSI					0xA2100288		/* TSSI			(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_CCTEST					0xA21003F8		/* CCTEST		(sub6 : - / mmw : o)*/
#define D_DU_REG_CC_CCMONSEL				0xA21003FC		/* CCMONSEL		(sub6 : - / mmw : o)*/
/* @} */

/*!
 * @name CCレジスタアドレス算出マクロ
 * @note CCレジスタアドレス算出マクロ, アドレスはこのマクロを使用する Ant は0(Ant#A)～7(Ant#H)である
 * @{
 */
#define	D_DU_OFT_CC_ANT						0xA2100400
#define M_DU_REG_CC_GETADDR(ofs, ant)		(UINT)(ofs  + (ant * D_DU_OFT_CC_ANT))
#define M_DU_REG_CC_TXCAREN(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXCAREN,ant)
#define M_DU_REG_CC_TXBW0(ant)				M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXBW0,ant)
#define M_DU_REG_CC_TXBW1(ant)				M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXBW1,ant)
#define M_DU_REG_CC_TXBW2(ant)				M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXBW2,ant)
#define M_DU_REG_CC_TXBW3(ant)				M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXBW3,ant)
#define M_DU_REG_CC_TXBW4(ant)				M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXBW4,ant)
#define M_DU_REG_CC_TXBW5(ant)				M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXBW5,ant)
#define M_DU_REG_CC_TXBW6(ant)				M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXBW6,ant)
#define M_DU_REG_CC_TXBW7(ant)				M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXBW7,ant)
#define M_DU_REG_CC_TXANTEN(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXANTEN,ant)
#define M_DU_REG_CC_LPFGAIN0(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_LPFGAIN0,ant)
#define M_DU_REG_CC_LPFGAIN1(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_LPFGAIN1,ant)
#define M_DU_REG_CC_LPFGAIN2(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_LPFGAIN2,ant)
#define M_DU_REG_CC_LPFGAIN3(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_LPFGAIN3,ant)
#define M_DU_REG_CC_LPFGAIN4(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_LPFGAIN4,ant)
#define M_DU_REG_CC_LPFGAIN5(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_LPFGAIN5,ant)
#define M_DU_REG_CC_LPFGAIN6(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_LPFGAIN6,ant)
#define M_DU_REG_CC_LPFGAIN7(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_LPFGAIN7,ant)
#define M_DU_REG_CC_TXNCO0(ant)				M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXNCO0,ant)
#define M_DU_REG_CC_TXNCO1(ant)				M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXNCO1,ant)
#define M_DU_REG_CC_TXNCO2(ant)				M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXNCO2,ant)
#define M_DU_REG_CC_TXNCO3(ant)				M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXNCO3,ant)
#define M_DU_REG_CC_TXNCO4(ant)				M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXNCO4,ant)
#define M_DU_REG_CC_TXNCO5(ant)				M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXNCO5,ant)
#define M_DU_REG_CC_TXNCO6(ant)				M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXNCO6,ant)
#define M_DU_REG_CC_TXNCO7(ant)				M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXNCO7,ant)
#define M_DU_REG_CC_TXNCOPH0(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXNCOPH0,ant)
#define M_DU_REG_CC_TXNCOPH1(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXNCOPH1,ant)
#define M_DU_REG_CC_TXNCOPH2(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXNCOPH2,ant)
#define M_DU_REG_CC_TXNCOPH3(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXNCOPH3,ant)
#define M_DU_REG_CC_TXNCOPH4(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXNCOPH4,ant)
#define M_DU_REG_CC_TXNCOPH5(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXNCOPH5,ant)
#define M_DU_REG_CC_TXNCOPH6(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXNCOPH6,ant)
#define M_DU_REG_CC_TXNCOPH7(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_TXNCOPH7,ant)
#define M_DU_REG_CC_TSSIMON(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_TSSIMON,ant)
#define M_DU_REG_CC_TSSISMPL(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_TSSISMPL,ant)
#define M_DU_REG_CC_TSSILAT(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_TSSILAT,ant)
#define M_DU_REG_CC_TSSI(ant)				M_DU_REG_CC_GETADDR(D_DU_REG_CC_TSSI,ant)
#define M_DU_REG_CC_CCTEST(ant)				M_DU_REG_CC_GETADDR(D_DU_REG_CC_CCTEST,ant)
#define M_DU_REG_CC_CCMONSEL(ant)			M_DU_REG_CC_GETADDR(D_DU_REG_CC_CCMONSEL,ant)
/* @} */
#endif
/* @} */
