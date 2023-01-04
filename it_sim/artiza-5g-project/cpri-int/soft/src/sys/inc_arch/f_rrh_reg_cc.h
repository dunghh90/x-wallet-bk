/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_cc.h
 * @brief レジスタアドレス定義ヘッダ(CC)
 * @date  2015/3/30 FJT) Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_CC_H
#define F_RRH_REG_CC_H

/*!
 * @name CCレジスタアドレス
 * @note CCレジスタアドレス, この値は直接使用してはいけない
 * @{
 */
#define	D_RRH_REG_CC_BR							0x00000400
#if defined (OPT_RRH_ARCH_A10)
#define	D_RRH_REG_CC_TXCAREN					0xD2000000
#define	D_RRH_REG_CC_TXCNF0						0xD2000010
#define	D_RRH_REG_CC_TXCNF1						0xD2000014
#define	D_RRH_REG_CC_TXPRM0						0xD2000030
#define	D_RRH_REG_CC_TXPRM1						0xD2000034
#define	D_RRH_REG_CC_LPFGAIN0					0xD2000050
#define	D_RRH_REG_CC_LPFGAIN1					0xD2000054
#define	D_RRH_REG_CC_TXANTEN					0xD2000080
#define	D_RRH_REG_CC_HCFR						0xD2000100
#define	D_RRH_REG_CC_ICFR						0xD2000140
#define	D_RRH_REG_CC_IMPMEM						0xD2000144
#define	D_RRH_REG_CC_ECFR						0xD2000180
#define	D_RRH_REG_CC_HCFRMON					0xD20001C0
#define	D_RRH_REG_CC_ICFRMON					0xD20001C4
#define	D_RRH_REG_CC_ECFRMON					0xD20001C8
#define	D_RRH_REG_CC_TSSI0						0xD2000200
#define	D_RRH_REG_CC_TSSI1						0xD2000204
#define	D_RRH_REG_CC_TSSISMPL					0xD2000214
#define	D_RRH_REG_CC_TSSILAT					0xD2000218
#define	D_RRH_REG_CC_TSSI						0xD200021C
#define	D_RRH_REG_CC_CWEN						0xD2000280
#define	D_RRH_REG_CC_CWAMP						0xD2000284
#define	D_RRH_REG_CC_TXNCOPH0					0xD2000300
#define	D_RRH_REG_CC_TXNCOPH1					0xD2000304
#define	D_RRH_REG_CC_CCTEST						0xD20003F8
#define	D_RRH_REG_CC_CCMONSEL					0xD20003FC

#elif defined  (OPT_RRH_ARCH_ZYNQ)
#define	D_RRH_REG_CC_TXCAREN					0x82000000
#define	D_RRH_REG_CC_TXCNF0						0x82000010
#define	D_RRH_REG_CC_TXCNF1						0x82000014
#define	D_RRH_REG_CC_TXPRM0						0x82000030
#define	D_RRH_REG_CC_TXPRM1						0x82000034
#define	D_RRH_REG_CC_LPFGAIN0					0x82000050
#define	D_RRH_REG_CC_LPFGAIN1					0x82000054
#define	D_RRH_REG_CC_TXANTEN					0x82000080
#define	D_RRH_REG_CC_HCFR						0x82000100
#define	D_RRH_REG_CC_ICFR						0x82000140
#define	D_RRH_REG_CC_IMPMEM						0x82000144
#define	D_RRH_REG_CC_ECFR						0x82000180
#define	D_RRH_REG_CC_HCFRMON					0x820001C0
#define	D_RRH_REG_CC_ICFRMON					0x820001C4
#define	D_RRH_REG_CC_ECFRMON					0x820001C8
#define	D_RRH_REG_CC_TSSI0						0x82000200
#define	D_RRH_REG_CC_TSSI1						0x82000204
#define	D_RRH_REG_CC_TSSISMPL					0x82000214
#define	D_RRH_REG_CC_TSSILAT					0x82000218
#define	D_RRH_REG_CC_TSSI						0x8200021C
#define	D_RRH_REG_CC_CWEN						0x82000280
#define	D_RRH_REG_CC_CWAMP						0x82000284
#define	D_RRH_REG_CC_TXNCOPH0					0x82000300
#define	D_RRH_REG_CC_TXNCOPH1					0x82000304
#define	D_RRH_REG_CC_CCTEST						0x820003F8
#define	D_RRH_REG_CC_CCMONSEL					0x820003FC
#else
#endif
/* @} */

/*!
 * @name CCレジスタアドレス算出マクロ
 * @note CCレジスタアドレス算出マクロ, アドレスはこのマクロを使用する br は0 ~ 3である
 * @{
 */
#define M_RRH_REG_CC_GETADDR(ofs,br)		( UINT )( ofs  + ( br * D_RRH_REG_CC_BR) )
#define M_RRH_REG_CC_TXCAREN( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_TXCAREN      , br ))
#define M_RRH_REG_CC_TXCNF0( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_TXCNF0       , br ))
#define M_RRH_REG_CC_TXCNF1( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_TXCNF1       , br ))
#define M_RRH_REG_CC_TXPRM0( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_TXPRM0       , br ))
#define M_RRH_REG_CC_TXPRM1( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_TXPRM1       , br ))
#define M_RRH_REG_CC_LPFGAIN0( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_LPFGAIN0     , br ))
#define M_RRH_REG_CC_LPFGAIN1( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_LPFGAIN1     , br ))
#define M_RRH_REG_CC_TXANTEN( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_TXANTEN      , br ))
#define M_RRH_REG_CC_HCFR( br )				( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_HCFR         , br ))
#define M_RRH_REG_CC_ICFR( br )				( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_ICFR         , br ))
#define M_RRH_REG_CC_IMPMEM( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_IMPMEM       , br ))
#define M_RRH_REG_CC_ECFR( br )				( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_ECFR         , br ))
#define M_RRH_REG_CC_HCFRMON( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_HCFRMON      , br ))
#define M_RRH_REG_CC_ICFRMON( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_ICFRMON      , br ))
#define M_RRH_REG_CC_ECFRMON( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_ECFRMON      , br ))
#define M_RRH_REG_CC_TSSI0( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_TSSI0        , br ))
#define M_RRH_REG_CC_TSSI1( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_TSSI1        , br ))
#define M_RRH_REG_CC_TSSISMPL( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_TSSISMPL     , br ))
#define M_RRH_REG_CC_TSSILAT( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_TSSILAT      , br ))
#define M_RRH_REG_CC_TSSI( br )				( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_TSSI         , br ))
#define M_RRH_REG_CC_CWEN( br )				( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_CWEN         , br ))
#define M_RRH_REG_CC_CWAMP( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_CWAMP        , br ))
#define M_RRH_REG_CC_TXNCOPH0( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_TXNCOPH0     , br ))
#define M_RRH_REG_CC_TXNCOPH1( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_TXNCOPH1     , br ))
#define M_RRH_REG_CC_CCTEST( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_CCTEST	      , br ))
#define M_RRH_REG_CC_CCMONSEL( br )			( M_RRH_REG_CC_GETADDR( D_RRH_REG_CC_CCMONSEL     , br ))
/* @} */

/*!
 * @name CCレジスタbit
 * @note CCレジスタbit
 * @{
 */
/* TXキャリアイネーブル */
#define D_RRH_CC_TXCAREN_C0				0x00000001
#define D_RRH_CC_TXCAREN_C1				0x00000002

/* キャリアTSSI値保持 */
#define D_RRH_CC_TSSILAT_LAT			0x00000001

/* IMPMEM Impulse係数 */
#define D_RRH_CC_IMPMEM_ACS				0x00000010
#define D_RRH_CC_IMPMEM_BEN				0x00000020
#define D_RRH_CC_IMPMEM_CNG				0x00008000
#define D_RRH_CC_IMPMEM_BSY				0x08000000

/* @} */
#endif
/* @} */
