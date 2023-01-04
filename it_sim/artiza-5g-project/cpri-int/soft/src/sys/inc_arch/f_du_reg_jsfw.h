/*!
 * @skip  $ld:$
 * @file  f_du_reg_jsfw.h
 * @brief レジスタアドレス定義ヘッダ(JSFW)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_JSFW_H
#define F_DU_REG_JSFW_H

/*!
 * @name JSFWレジスタアドレス
 * @note JSFWレジスタアドレス, この値は直接使用してはいけない
 * @{
 */
#define D_DU_REG_JSFW_JSESRCTR				0xB1204000		/* JSESRCTR		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSESRLOG				0xB1204004		/* JSESRLOG		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWSRCTR				0xB1204020		/* JSFWSRCTR	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWSYNCCTR			0xB1204030		/* JSFWSYNCCTR	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWSYNCMON			0xB1204034		/* JSFWSYNCMON	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWEN				0xB1204080		/* JSFWEN		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWRF				0xB1204084		/* JSFWRF		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWCTR				0xB1204090		/* JSFWCTR		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWDTSEL				0xB1204094		/* JSFWDTSEL	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWRSMD				0xB12040A0		/* JSFWRSMD		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWCPC0				0xB1204100		/* JSFWCPC0		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWCPC1				0xB1204104		/* JSFWCPC1		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWCPC2				0xB1204108		/* JSFWCPC2		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWCPC3				0xB120410C		/* JSFWCPC3		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWCPL0				0xB1204110		/* JSFWCPL0		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWCPL1				0xB1204114		/* JSFWCPL1		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWCPL2				0xB1204118		/* JSFWCPL2		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWCPL3				0xB120411C		/* JSFWCPL3		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWCPL4				0xB1204120		/* JSFWCPL4		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWCPL5				0xB1204124		/* JSFWCPL5		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWCPL6				0xB1204128		/* JSFWCPL6		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWCPL7				0xB120412C		/* JSFWCPL7		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWXBAR				0xB1204200		/* JSFWXBAR		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWST				0xB1204300		/* JSFWST		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWPFST				0xB1204304		/* JSFWPFST		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWPUL				0xB120430C		/* JSFWPUL		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWERC1				0xB1204360		/* JSFWERC1		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWERC2				0xB1204364		/* JSFWERC2		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWSTIM				0xB1204380		/* JSFWSTIM		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWIRQ				0xB1204400		/* JSFWIRQ		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWIRQC				0xB1204404		/* JSFWIRQC		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWIRQM				0xB1204408		/* JSFWIRQM		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWLMFC				0xB1204500		/* JSFWLMFC		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWSTMON				0xB1204F00		/* JSFWSTMON	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWSTLOG				0xB1204F04		/* JSFWSTLOG	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWILASCNT			0xB1204F08		/* JSFWILASCNT	(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWTEST				0xB1204FF0		/* JSFWTEST		(sub6 : o / mmw : o)*/
#define D_DU_REG_JSFW_JSFWMSEL				0xB1204FFC		/* JSFWMSEL		(sub6 : o / mmw : o)*/
/* @} */

/*!
 * @name JSFWレジスタアドレス算出マクロ
 * @note JSFWレジスタアドレス算出マクロ, アドレスはこのマクロを使用する unit は0～3である
 * @{
 */
#define	D_DU_OFT_JSFW_UNIT					0x00000000
#define M_DU_REG_JSFW_GETADDR(ofs, unit)	(UINT)(ofs  + ((unit < 2) ? 0 : D_DU_OFT_JSFW_UNIT))
#define M_DU_REG_JSFW_JSESRCTR(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSESRCTR,unit)
#define M_DU_REG_JSFW_JSESRLOG(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSESRLOG,unit)
#define M_DU_REG_JSFW_JSFWSRCTR(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWSRCTR,unit)
#define M_DU_REG_JSFW_JSFWSYNCCTR(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWSYNCCTR,unit)
#define M_DU_REG_JSFW_JSFWSYNCMON(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWSYNCMON,unit)
#define M_DU_REG_JSFW_JSFWEN(unit)			M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWEN,unit)
#define M_DU_REG_JSFW_JSFWRF(unit)			M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWRF,unit)
#define M_DU_REG_JSFW_JSFWCTR(unit)			M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWCTR,unit)
#define M_DU_REG_JSFW_JSFWDTSEL(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWDTSEL,unit)
#define M_DU_REG_JSFW_JSFWRSMD(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWRSMD,unit)
#define M_DU_REG_JSFW_JSFWCPC0(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWCPC0,unit)
#define M_DU_REG_JSFW_JSFWCPC1(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWCPC1,unit)
#define M_DU_REG_JSFW_JSFWCPC2(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWCPC2,unit)
#define M_DU_REG_JSFW_JSFWCPC3(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWCPC3,unit)
#define M_DU_REG_JSFW_JSFWCPL0(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWCPL0,unit)
#define M_DU_REG_JSFW_JSFWCPL1(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWCPL1,unit)
#define M_DU_REG_JSFW_JSFWCPL2(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWCPL2,unit)
#define M_DU_REG_JSFW_JSFWCPL3(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWCPL3,unit)
#define M_DU_REG_JSFW_JSFWCPL4(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWCPL4,unit)
#define M_DU_REG_JSFW_JSFWCPL5(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWCPL5,unit)
#define M_DU_REG_JSFW_JSFWCPL6(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWCPL6,unit)
#define M_DU_REG_JSFW_JSFWCPL7(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWCPL7,unit)
#define M_DU_REG_JSFW_JSFWXBAR(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWXBAR,unit)
#define M_DU_REG_JSFW_JSFWST(unit)			M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWST,unit)
#define M_DU_REG_JSFW_JSFWPFST(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWPFST,unit)
#define M_DU_REG_JSFW_JSFWPUL(unit)			M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWPUL,unit)
#define M_DU_REG_JSFW_JSFWERC1(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWERC1,unit)
#define M_DU_REG_JSFW_JSFWERC2(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWERC2,unit)
#define M_DU_REG_JSFW_JSFWSTIM(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWSTIM,unit)
#define M_DU_REG_JSFW_JSFWIRQ(unit)			M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWIRQ,unit)
#define M_DU_REG_JSFW_JSFWIRQC(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWIRQC,unit)
#define M_DU_REG_JSFW_JSFWIRQM(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWIRQM,unit)
#define M_DU_REG_JSFW_JSFWLMFC(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWLMFC,unit)
#define M_DU_REG_JSFW_JSFWSTMON(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWSTMON,unit)
#define M_DU_REG_JSFW_JSFWSTLOG(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWSTLOG,unit)
#define M_DU_REG_JSFW_JSFWILASCNT(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWILASCNT,unit)
#define M_DU_REG_JSFW_JSFWTEST(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWTEST,unit)
#define M_DU_REG_JSFW_JSFWMSEL(unit)		M_DU_REG_JSFW_GETADDR(D_DU_REG_JSFW_JSFWMSEL,unit)
/* @} */
#endif
/* @} */
