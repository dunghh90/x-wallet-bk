/*!
 * @skip  $ld:$
 * @file  f_du_reg_fbant.h
 * @brief レジスタアドレス定義ヘッダ(FBANT)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_FBANT_H
#define F_DU_REG_FBANT_H

/*!
 * @name FBANTレジスタアドレス
 * @note FBANTレジスタアドレス, この値は直接使用してはいけない
 * @{
 */
#define D_DU_REG_FBANT_FSDORPT				0xB1203000		/* FSDORPT		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_FSDOCTRL				0xB1203004		/* FSDOCTRL		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_FSDOMASK				0xB1203008		/* FSDOMASK		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_FSDOMODE				0xB120300C		/* FSDOMODE		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_FSDOMAX				0xB1203010		/* FSDOMAX		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_FBDEM				0xB1203040		/* FBDEM		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_FBROT				0xB1203044		/* FBROT		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_DEMGAIN				0xB1203048		/* DEMGAIN		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_FBDTERR				0xB120304C		/* FBDTERR		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_FBDLY				0xB1203050		/* FBDLY		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_DFIL					0xB1203054		/* DFIL			(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_FBDLYAC				0xB1203058		/* FBDLYAC		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_REFNCO				0xB1203060		/* REFNCO		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_FBEQ					0xB1203070		/* FBEQ			(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_RTNGAIN				0xB1203074		/* RTNGAIN		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_SSPMU1				0xB1203080		/* SSPMU1		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_SSPMU2				0xB1203084		/* SSPMU2		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_SSPMU2ROW			0xB1203088		/* SSPMU2ROW	(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_LUT0XCFG				0xB1203090		/* LUT0XCFG		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_DLTOFST				0xB12030A0		/* DLTOFST		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_DLTCLP				0xB12030A4		/* DLTCLP		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_DLTRELIM				0xB12030A8		/* DLTRELIM		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_DLTOFSTMON			0xB12030B0		/* DLTOFSTMON	(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_POWLAT				0xB12030C0		/* POWLAT		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_REFPSMPL				0xB12030C4		/* REFPSMPL		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_PSWSMPL				0xB12030C8		/* PSWSMPL		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_POWCTRL				0xB12030CC		/* POWCTRL		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_FPERR				0xB12030D0		/* FPERR		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_POWREFRF				0xB12030E0		/* POWREFRF		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_POWDPDRF				0xB12030E4		/* POWDPDRF		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_POWREFSW				0xB12030E8		/* POWREFSW		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_POWFBKSW				0xB12030EC		/* POWFBKSW		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_POWRVSSW				0xB12030F0		/* POWRVSSW		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_NUMRFPOWSW			0xB12030F4		/* NUMRFPOWSW	(sub6 : o / mmw : -)*/
#define D_DU_REG_FBANT_POWREFSW_VSWR		0xB12030F8		/* POWREFSW_VSWR	(sub6 : o / mmw : -)*/
/* @} */

/*!
 * @name FBANTレジスタアドレス算出マクロ
 * @note FBANTレジスタアドレス算出マクロ, アドレスはこのマクロを使用する Ant は0(Ant#A)～7(Ant#H)である
 * @{
 */
#define	D_DU_OFT_FBANT_ANT					0x00000400

#define M_DU_REG_FBANT_GETADDR(ofs, ant)	(UINT)(ofs + (ant* D_DU_OFT_FBANT_ANT) )
#define M_DU_REG_FBANT_FSDORPT(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_FSDORPT, ant)
#define M_DU_REG_FBANT_FSDOCTRL(ant)		M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_FSDOCTRL, ant)
#define M_DU_REG_FBANT_FSDOMASK(ant)		M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_FSDOMASK, ant)
#define M_DU_REG_FBANT_FSDOMODE(ant)		M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_FSDOMODE, ant)
#define M_DU_REG_FBANT_FSDOMAX(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_FSDOMAX, ant)
#define M_DU_REG_FBANT_FBDEM(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_FBDEM, ant)
#define M_DU_REG_FBANT_FBROT(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_FBROT, ant)
#define M_DU_REG_FBANT_DEMGAIN(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_DEMGAIN, ant)
#define M_DU_REG_FBANT_FBDTERR(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_FBDTERR, ant)
#define M_DU_REG_FBANT_FBDLY(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_FBDLY, ant)
#define M_DU_REG_FBANT_DFIL(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_DFIL, ant)
#define M_DU_REG_FBANT_FBDLYAC(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_FBDLYAC, ant)
#define M_DU_REG_FBANT_REFNCO(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_REFNCO, ant)
#define M_DU_REG_FBANT_FBEQ(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_FBEQ, ant)
#define M_DU_REG_FBANT_RTNGAIN(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_RTNGAIN, ant)
#define M_DU_REG_FBANT_SSPMU1(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_SSPMU1, ant)
#define M_DU_REG_FBANT_SSPMU2(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_SSPMU2, ant)
#define M_DU_REG_FBANT_SSPMU2ROW(ant)		M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_SSPMU2ROW, ant)
#define M_DU_REG_FBANT_LUT0XCFG(ant)		M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_LUT0XCFG, ant)
#define M_DU_REG_FBANT_DLTOFST(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_DLTOFST, ant)
#define M_DU_REG_FBANT_DLTCLP(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_DLTCLP, ant)
#define M_DU_REG_FBANT_DLTRELIM(ant)		M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_DLTRELIM, ant)
#define M_DU_REG_FBANT_DLTOFSTMON(ant)		M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_DLTOFSTMON, ant)
#define M_DU_REG_FBANT_POWLAT(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_POWLAT, ant)
#define M_DU_REG_FBANT_REFPSMPL(ant)		M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_REFPSMPL, ant)
#define M_DU_REG_FBANT_PSWSMPL(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_PSWSMPL, ant)
#define M_DU_REG_FBANT_POWCTRL(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_POWCTRL, ant)
#define M_DU_REG_FBANT_FPERR(ant)			M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_FPERR, ant)
#define M_DU_REG_FBANT_POWREFRF(ant)		M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_POWREFRF, ant)
#define M_DU_REG_FBANT_POWDPDRF(ant)		M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_POWDPDRF, ant)
#define M_DU_REG_FBANT_POWREFSW(ant)		M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_POWREFSW, ant)
#define M_DU_REG_FBANT_POWFBKSW(ant)		M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_POWFBKSW, ant)
#define M_DU_REG_FBANT_POWRVSSW(ant)		M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_POWRVSSW, ant)
#define M_DU_REG_FBANT_NUMRFPOWSW(ant)		M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_NUMRFPOWSW, ant)
#define M_DU_REG_FBANT_POWREFSW_VSWR(ant)	M_DU_REG_FBANT_GETADDR(D_DU_REG_FBANT_POWREFSW_VSWR, ant)

/*!
 * @name レジスタ値
 * @note レジスタ値
 * @{
 */
/* DEMGAIN */
#define D_DU_FB_DEMGAIN_I					0x00003FFF
#define D_DU_FB_DEMGAIN_Q					0x3FFF0000

/* FBDLY */
#define D_DU_FB_FBDLY_CLK					0x0003FF00
#define D_DU_FB_FBDLY_FIL					0x000000FE

/* FBDLY */
#define D_DU_FB_POWLAT_LAT					0x00000001

/* @} */
#endif
/* @} */
