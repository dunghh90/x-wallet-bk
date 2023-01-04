/*!
 * @skip  $ld:$
 * @file  f_du_reg_fbmod.h
 * @brief レジスタアドレス定義ヘッダ(FBMOD)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_FBMOD_H
#define F_DU_REG_FBMOD_H

/*!
 * @name FBMODレジスタアドレス
 * @note FBMODレジスタアドレス, この値は直接使用してはいけない
 * @{
 */
#define D_DU_REG_FBMOD_RFPATH				0xB1202800		/* RFPATH		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_RFFBSWBR				0xB1202804		/* RFFBSWBR		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_FBSTRMGO				0xB1202810		/* FBSTRMGO		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_FBSTRMSTOP			0xB1202814		/* FBSTRMSTOP	(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_FBSTRMCFG			0xB1202818		/* FBSTRMCFG	(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_FBSTRMOFST			0xB120281C		/* FBSTRMOFST	(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_BUFLEN				0xB1202820		/* BUFLEN		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_BUFLENFLCT			0xB1202824		/* BUFLENFLCT	(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_GRAVITY				0xB1202828		/* GRAVITY		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_CALCMODE				0xB1202840		/* CALCMODE		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_ITDATA				0xB1202850		/* ITDATA		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_ITRST				0xB1202854		/* ITRST		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_ITMBA				0xB1202858		/* ITMBA		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_ITMBP				0xB120285C		/* ITMBP		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_IT1					0xB1202870		/* IT1			(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_IT2					0xB1202874		/* IT2			(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_IT3					0xB1202878		/* IT3			(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_IT1SUM				0xB1202880		/* IT1SUM		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_IT2SUM				0xB1202884		/* IT2SUM		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_IT3SUM				0xB1202888		/* IT3SUM		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_IT1MSKLO				0xB1202890		/* IT1MSKLO		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_IT2MSKLO				0xB1202894		/* IT2MSKLO		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_IT3MSKLO				0xB1202898		/* IT3MSKLO		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_IT1MSKHI				0xB12028A0		/* IT1MSKHI		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_IT2MSKHI				0xB12028A4		/* IT2MSKHI		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_IT3MSKHI				0xB12028A8		/* IT3MSKHI		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_TFLP					0xB12028B0		/* TFLP			(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_FBMEMKEY				0xB12028C0		/* FBMEMKEY		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_FBTEST				0xB12028C4		/* FBTEST		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_FBDEM				0xB1202940		/* FBDEM		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_FBROT				0xB1202944		/* FBROT		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_DEMGAIN				0xB1202948		/* DEMGAIN		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_FBDLY				0xB1202950		/* FBDLY		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_REFNCO				0xB1202960		/* REFNCO		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_FBEQ					0xB1202970		/* FBEQ			(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_HISTOGRAM			0xB1202980		/* HISTOGRAM	(sub6 : o / mmw : -)*/
#define D_DU_REG_FBMOD_FB_MONSEL			0xB12029FC		/* FB_MONSEL	(sub6 : o / mmw : -)*/
/* @} */

/*!
 * @name FBMODレジスタアドレス算出マクロ
 * @note FBMODレジスタアドレス算出マクロ, アドレスはこのマクロを使用する Ant は0(Ant#A)～7(Ant#H)である
 * @{
 */
#define	D_DU_OFT_FBMOD_ANT					0x00000200
#define M_DU_REG_FBMOD_GETADDR(ofs, ant)	(UINT)(ofs  + (ant * D_DU_OFT_FBMOD_ANT))
#define M_DU_REG_FBMOD_RFPATH(ant)			M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_RFPATH, ant)
#define M_DU_REG_FBMOD_RFFBSWBR(ant)		M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_RFFBSWBR, ant)
#define M_DU_REG_FBMOD_FBSTRMGO(ant)		M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_FBSTRMGO, ant)
#define M_DU_REG_FBMOD_FBSTRMSTOP(ant)		M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_FBSTRMSTOP, ant)
#define M_DU_REG_FBMOD_FBSTRMCFG(ant)		M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_FBSTRMCFG, ant)
#define M_DU_REG_FBMOD_FBSTRMOFST(ant)		M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_FBSTRMOFST, ant)
#define M_DU_REG_FBMOD_BUFLEN(ant)			M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_BUFLEN, ant)
#define M_DU_REG_FBMOD_BUFLENFLCT(ant)		M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_BUFLENFLCT, ant)
#define M_DU_REG_FBMOD_GRAVITY(ant)			M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_GRAVITY, ant)
#define M_DU_REG_FBMOD_CALCMODE(ant)		M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_CALCMODE, ant)
#define M_DU_REG_FBMOD_ITDATA(ant)			M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_ITDATA, ant)
#define M_DU_REG_FBMOD_ITRST(ant)			M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_ITRST, ant)
#define M_DU_REG_FBMOD_ITMBA(ant)			M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_ITMBA, ant)
#define M_DU_REG_FBMOD_ITMBP(ant)			M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_ITMBP, ant)
#define M_DU_REG_FBMOD_IT1(ant)				M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_IT1, ant)
#define M_DU_REG_FBMOD_IT2(ant)				M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_IT2, ant)
#define M_DU_REG_FBMOD_IT3(ant)				M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_IT3, ant)
#define M_DU_REG_FBMOD_IT1SUM(ant)			M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_IT1SUM, ant)
#define M_DU_REG_FBMOD_IT2SUM(ant)			M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_IT2SUM, ant)
#define M_DU_REG_FBMOD_IT3SUM(ant)			M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_IT3SUM, ant)
#define M_DU_REG_FBMOD_IT1MSKLO(ant)		M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_IT1MSKLO, ant)
#define M_DU_REG_FBMOD_IT2MSKLO(ant)		M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_IT2MSKLO, ant)
#define M_DU_REG_FBMOD_IT3MSKLO(ant)		M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_IT3MSKLO, ant)
#define M_DU_REG_FBMOD_IT1MSKHI(ant)		M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_IT1MSKHI, ant)
#define M_DU_REG_FBMOD_IT2MSKHI(ant)		M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_IT2MSKHI, ant)
#define M_DU_REG_FBMOD_IT3MSKHI(ant)		M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_IT3MSKHI, ant)
#define M_DU_REG_FBMOD_TFLP(ant)			M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_TFLP, ant)
#define M_DU_REG_FBMOD_FBMEMKEY(ant)		M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_FBMEMKEY, ant)
#define M_DU_REG_FBMOD_FBTEST(ant)			M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_FBTEST, ant)
#define M_DU_REG_FBMOD_FBDEM(ant)			M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_FBDEM, ant)
#define M_DU_REG_FBMOD_FBROT(ant)			M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_FBROT, ant)
#define M_DU_REG_FBMOD_DEMGAIN(ant)			M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_DEMGAIN, ant)
#define M_DU_REG_FBMOD_FBDLY(ant)			M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_FBDLY, ant)
#define M_DU_REG_FBMOD_REFNCO(ant)			M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_REFNCO, ant)
#define M_DU_REG_FBMOD_FBEQ(ant)			M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_FBEQ, ant)
#define M_DU_REG_FBMOD_HISTOGRAM(ant)		M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_HISTOGRAM, ant)
#define M_DU_REG_FBMOD_FB_MONSEL(ant)		M_DU_REG_FBMOD_GETADDR(D_DU_REG_FBMOD_FB_MONSEL, ant)
/* @} */
#endif
/* @} */
