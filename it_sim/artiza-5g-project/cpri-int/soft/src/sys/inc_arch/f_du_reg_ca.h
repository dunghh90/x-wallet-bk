/*!
 * @skip  $ld:$
 * @file  f_du_reg_ca.h
 * @brief レジスタアドレス定義ヘッダ(CA)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_CA_H
#define F_DU_REG_CA_H

/*!
 * @name CAレジスタアドレス
 * @note CAレジスタアドレス, この値は直接使用してはいけない
 * @{
 */
#define D_DU_REG_CA_TXCAREN					0xB1100000		/* TXCAREN		(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_TXBW0					0xB1100010		/* TXBW0		(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_TXBW1					0xB1100014		/* TXBW1		(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_TXANTEN					0xB1100040		/* TXANTEN		(sub6 : o / mmw : -)*/

#define D_DU_REG_CC_AEQFCOEF				0xb11001f0
#define D_DU_REG_CC_AEQFEN					0xb11001f4

#define D_DU_REG_CC_ALPFTSSI0              (0xb11001c0)
#define D_DU_REG_CC_ALPFTSSI1              (0xb11001c4)

#define D_DU_REG_CC_ALPFTSSI0_MMW          (0xb1100100)


#define D_DU_REG_CA_LPFGAIN0				0xB1100200		/* LPFGAIN0		(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_LPFGAIN1				0xB1100204		/* LPFGAIN1		(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_TXNCO0					0xB1100220		/* TXNCO0		(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_TXNCO1					0xB1100224		/* TXNCO1		(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_ICFRB					0xB1100240		/* ICFRB		(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_ICFRA					0xB1100244		/* ICFRA		(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_MPMEM					0xB1100248		/* MPMEM		(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_IMPMEMRDSEL				0xB110024C		/* IMPMEMRDSEL	(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_ECFR					0xB1100280		/* ECFR			(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_ICFRBFRMON				0xB11002C0		/* ICFRBFRMON	(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_ICFRAFRMON				0xB11002C4		/* ICFRAFRMON	(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_ECFRMON					0xB11002D0		/* ECFRMON		(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_TXNCOPH0				0xB11002F0		/* TXNCOPH0		(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_TXNCOPH1				0xB11002F4		/* TXNCOPH1		(sub6 : o / mmw : -)*/

#define D_DU_REG_CA_TSSILAT					0xB11002E0		/* TXNCOPH1		(sub6 : o / mmw : -)*/

#define D_DU_REG_CA_MCGAIN_MMW				0xB110030C

#define D_DU_REG_CC_APGCPOWMON				0xB1100380

#define D_DU_REG_CC_PGCP					0xB1100398


#define D_DU_REG_CA_CCTEST					0xB11003F8		/* CCTEST		(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_CCMONSEL				0xB11003FC		/* CCMONSEL		(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_CRTEST					0xB11003F8		/* CRTEST		(sub6 : o / mmw : -)*/
#define D_DU_REG_CA_CRMONSEL				0xB11003FC		/* CRMONSEL		(sub6 : o / mmw : -)*/

#define D_DU_REG_CC_ACWEN                  (0xb1100140)
#define D_DU_REG_CC_ACWAMPI                (0xb1100144)
#define D_DU_REG_CC_ACWAMPQ                (0xb1100148)



/* @} */

/*!
 * @name CAレジスタアドレス算出マクロ
 * @note CAレジスタアドレス算出マクロ, アドレスはこのマクロを使用する Ant は0(Ant#A)～7(Ant#H)である
 * @{
 */
#define	D_DU_OFT_CA_UNIT					0x00000000
#define	D_DU_OFT_CA_ANT						0x00000400
#define M_DU_REG_CA_GETADDR(ofs, ant)		(UINT)( ofs  + (ant  * D_DU_OFT_CA_ANT))
#define M_DU_REG_CA_TXCAREN(ant)			M_DU_REG_CA_GETADDR(D_DU_REG_CA_TXCAREN, ant)
#define M_DU_REG_CA_TXBW0(ant)				M_DU_REG_CA_GETADDR(D_DU_REG_CA_TXBW0, ant)
#define M_DU_REG_CA_TXBW1(ant)				M_DU_REG_CA_GETADDR(D_DU_REG_CA_TXBW1, ant)
#define M_DU_REG_CA_TXANTEN(ant)			M_DU_REG_CA_GETADDR(D_DU_REG_CA_TXANTEN, ant)
#define M_DU_REG_CC_AEQFCOEF(ant)			M_DU_REG_CA_GETADDR(D_DU_REG_CC_AEQFCOEF, ant)
#define M_DU_REG_CC_AEQFEN(ant)				M_DU_REG_CA_GETADDR(D_DU_REG_CC_AEQFEN, ant)
#define M_DU_REG_CA_LPFGAIN0(ant)			M_DU_REG_CA_GETADDR(D_DU_REG_CA_LPFGAIN0, ant)
#define M_DU_REG_CA_LPFGAIN1(ant)			M_DU_REG_CA_GETADDR(D_DU_REG_CA_LPFGAIN1, ant)
#define M_DU_REG_CA_TXNCO0(ant)				M_DU_REG_CA_GETADDR(D_DU_REG_CA_TXNCO0, ant)
#define M_DU_REG_CA_TXNCO1(ant)				M_DU_REG_CA_GETADDR(D_DU_REG_CA_TXNCO1, ant)
#define M_DU_REG_CA_ICFRB(ant)				M_DU_REG_CA_GETADDR(D_DU_REG_CA_ICFRB, ant)
#define M_DU_REG_CA_ICFRA(ant)				M_DU_REG_CA_GETADDR(D_DU_REG_CA_ICFRA, ant)
#define M_DU_REG_CA_MPMEM(ant)				M_DU_REG_CA_GETADDR(D_DU_REG_CA_MPMEM, ant)
#define M_DU_REG_CA_IMPMEMRDSEL(ant)		M_DU_REG_CA_GETADDR(D_DU_REG_CA_IMPMEMRDSEL, ant)
#define M_DU_REG_CA_ECFR(ant)				M_DU_REG_CA_GETADDR(D_DU_REG_CA_ECFR, ant)
#define M_DU_REG_CA_ICFRBFRMON(ant)			M_DU_REG_CA_GETADDR(D_DU_REG_CA_ICFRBFRMON, ant)
#define M_DU_REG_CA_ICFRAFRMON(ant)			M_DU_REG_CA_GETADDR(D_DU_REG_CA_ICFRAFRMON, ant)
#define M_DU_REG_CA_ECFRMON(ant)			M_DU_REG_CA_GETADDR(D_DU_REG_CA_ECFRMON, ant)
#define M_DU_REG_CA_TXNCOPH0(ant)			M_DU_REG_CA_GETADDR(D_DU_REG_CA_TXNCOPH0, ant)
#define M_DU_REG_CA_TXNCOPH1(ant)			M_DU_REG_CA_GETADDR(D_DU_REG_CA_TXNCOPH1, ant)
#define M_DU_REG_CA_MCGAIN_MMW(ant)			M_DU_REG_CA_GETADDR(D_DU_REG_CA_MCGAIN_MMW, ant)
#define M_DU_REG_CA_CCTEST(ant)				M_DU_REG_CA_GETADDR(D_DU_REG_CA_CCTEST, ant)
#define M_DU_REG_CA_CCMONSEL(ant)			M_DU_REG_CA_GETADDR(D_DU_REG_CA_CCMONSEL, ant)
#define M_DU_REG_CA_CRTEST(ant)				M_DU_REG_CA_GETADDR(D_DU_REG_CA_CRTEST, ant)
#define M_DU_REG_CA_CRMONSEL(ant)			M_DU_REG_CA_GETADDR(D_DU_REG_CA_CRMONSEL, ant)
#define M_DU_REG_CC_APGCPOWMON(ant)			M_DU_REG_CA_GETADDR(D_DU_REG_CC_APGCPOWMON, ant)
#define M_DU_REG_CC_PGCP(ant)				M_DU_REG_CA_GETADDR(D_DU_REG_CC_PGCP, ant)

#define M_DU_REG_CC_ACWEN(ant)				M_DU_REG_CA_GETADDR(D_DU_REG_CC_ACWEN, ant)
#define M_DU_REG_CC_ACWAMPI(ant)			M_DU_REG_CA_GETADDR(D_DU_REG_CC_ACWAMPI, ant)
#define M_DU_REG_CC_ACWAMPQ(ant)			M_DU_REG_CA_GETADDR(D_DU_REG_CC_ACWAMPQ, ant)

/* @} */
#endif
/* @} */
