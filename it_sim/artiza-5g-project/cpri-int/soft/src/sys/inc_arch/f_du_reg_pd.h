/*!
 * @skip  $ld:$
 * @file  f_du_reg_pd.h
 * @brief レジスタアドレス定義ヘッダ(PD)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_PD_H
#define F_DU_REG_PD_H

/*!
 * @name PDレジスタアドレス
 * @note PDレジスタアドレス, この値は直接使用してはいけない
 * @{
 */
#define D_DU_REG_PD_ADR1ADJ					0xB1200000		/* ADR1ADJ		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_ADR2ADJ					0xB1200004		/* ADR2ADJ		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_ADR3ADJ					0xB1200008		/* ADR3ADJ		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_ADR1CNT					0xB1200010		/* ADR1CNT		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_ADR2CNT					0xB1200014		/* ADR2CNT		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_ADR3CNT					0xB1200018		/* ADR3CNT		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_ADR1RPT					0xB1200020		/* ADR1RPT		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_ADR2RPT					0xB1200024		/* ADR2RPT		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_ADR3RPT					0xB1200028		/* ADR3RPT		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_TRTIME1					0xB1200040		/* TRTIME1		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_TRTIME2					0xB1200044		/* TRTIME2		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_TRTIME3					0xB1200048		/* TRTIME3		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_TRTIME4					0xB120004C		/* TRTIME4		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_TRTIME5					0xB1200050		/* TRTIME5		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_TRTIME6					0xB1200054		/* TRTIME6		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_TRTIME7					0xB1200058		/* TRTIME7		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_LUT_X					0xB1200080		/* LUT%X		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_LUTWDIS					0xB12000C0		/* LUTWDIS		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_MCGAIN					0xB12000D0		/* MCGAIN		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_RFGAIN					0xB12000E0		/* RFGAIN		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_RFDCO					0xB12000E4		/* RFDCO		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_APDON					0xB1200100		/* APDON		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_CALDT					0xB1200110		/* CALDT		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_CALLEN					0xB1200114		/* CALLEN		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_DLTOFSTMON				0xB12001C0		/* DLTOFSTMON	(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_MAXLUTX					0xB12001D0		/* MAXLUTX%		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_PDMEMKEY				0xB12003F0		/* PDMEMKEY		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_PDLASTMODI				0xB12003F4		/* PDLASTMODI	(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_PDTEST					0xB12003F8		/* PDTEST		(sub6 : o / mmw : -)*/
#define D_DU_REG_PD_PDMONSEL				0xB12003FC		/* PDMONSEL		(sub6 : o / mmw : -)*/
/* @} */

/*!
 * @name PDレジスタアドレス算出マクロ
 * @note PDレジスタアドレス算出マクロ, アドレスはこのマクロを使用する Ant は0(Ant#A)～7(Ant#H)である
 * @{
 */
#define	D_DU_OFT_PD_ANT						0x00000400
#define M_DU_REG_PD_GETADDR(ofs, ant)		(UINT)(ofs + ((ant % 4) * D_DU_OFT_PD_ANT))
#define M_DU_REG_PD_ADR1ADJ(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_ADR1ADJ,ant)
#define M_DU_REG_PD_ADR2ADJ(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_ADR2ADJ,ant)
#define M_DU_REG_PD_ADR3ADJ(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_ADR3ADJ,ant)
#define M_DU_REG_PD_ADR1CNT(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_ADR1CNT,ant)
#define M_DU_REG_PD_ADR2CNT(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_ADR2CNT,ant)
#define M_DU_REG_PD_ADR3CNT(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_ADR3CNT,ant)
#define M_DU_REG_PD_ADR1RPT(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_ADR1RPT,ant)
#define M_DU_REG_PD_ADR2RPT(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_ADR2RPT,ant)
#define M_DU_REG_PD_ADR3RPT(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_ADR3RPT,ant)
#define M_DU_REG_PD_TRTIME1(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_TRTIME1,ant)
#define M_DU_REG_PD_TRTIME2(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_TRTIME2,ant)
#define M_DU_REG_PD_TRTIME3(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_TRTIME3,ant)
#define M_DU_REG_PD_TRTIME4(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_TRTIME4,ant)
#define M_DU_REG_PD_TRTIME5(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_TRTIME5,ant)
#define M_DU_REG_PD_TRTIME6(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_TRTIME6,ant)
#define M_DU_REG_PD_TRTIME7(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_TRTIME7,ant)
#define M_DU_REG_PD_LUT_X(ant)				M_DU_REG_PD_GETADDR(D_DU_REG_PD_LUT_X,ant)
#define M_DU_REG_PD_LUTWDIS(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_LUTWDIS,ant)
#define M_DU_REG_PD_MCGAIN(ant)				M_DU_REG_PD_GETADDR(D_DU_REG_PD_MCGAIN,ant)
#define M_DU_REG_PD_RFGAIN(ant)				M_DU_REG_PD_GETADDR(D_DU_REG_PD_RFGAIN,ant)
#define M_DU_REG_PD_RFDCO(ant)				M_DU_REG_PD_GETADDR(D_DU_REG_PD_RFDCO,ant)
#define M_DU_REG_PD_APDON(ant)				M_DU_REG_PD_GETADDR(D_DU_REG_PD_APDON,ant)
#define M_DU_REG_PD_CALDT(ant)				M_DU_REG_PD_GETADDR(D_DU_REG_PD_CALDT,ant)
#define M_DU_REG_PD_CALLEN(ant)				M_DU_REG_PD_GETADDR(D_DU_REG_PD_CALLEN,ant)
#define M_DU_REG_PD_DLTOFSTMON(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_DLTOFSTMON,ant)
#define M_DU_REG_PD_MAXLUTX(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_MAXLUTX,ant)
#define M_DU_REG_PD_PDMEMKEY(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_PDMEMKEY,ant)
#define M_DU_REG_PD_PDLASTMODI(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_PDLASTMODI,ant)
#define M_DU_REG_PD_PDTEST(ant)				M_DU_REG_PD_GETADDR(D_DU_REG_PD_PDTEST,ant)
#define M_DU_REG_PD_PDMONSEL(ant)			M_DU_REG_PD_GETADDR(D_DU_REG_PD_PDMONSEL,ant)

/*!
 * @name レジスタ値
 * @note レジスタ値
 * @{
 */
/* MCGAIN */
#define D_DU_PD_MCGAIN_MCGAIN					0x00003FFF

/* RFGAIN */
#define D_DU_PD_RFGAIN_RFGAIN					0x0000FFFF
/* @} */
#endif
/* @} */
