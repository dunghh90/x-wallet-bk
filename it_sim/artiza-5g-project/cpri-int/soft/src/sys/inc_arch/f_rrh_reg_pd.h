/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_pd.h
 * @brief レジスタアドレス定義ヘッダ(PD)
 * @date  2015/3/27 FJT) Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_PD_H
#define F_RRH_REG_PD_H

/*!
 * @name PDレジスタアドレス
 * @note PDレジスタアドレス
 * @{
 */
#define	D_RRH_REG_PD_BR							0x00000400
#if defined (OPT_RRH_ARCH_A10)
#define	D_RRH_REG_PD_ADR1ADJ					0xD2001000
#define	D_RRH_REG_PD_ADR2ADJ					0xD2001004
#define	D_RRH_REG_PD_ADR1CNT					0xD2001010
#define	D_RRH_REG_PD_ADR2CNT					0xD2001014
#define	D_RRH_REG_PD_ADRRPT						0xD2001040
#define	D_RRH_REG_PD_LUT0X						0xD2001100
#define	D_RRH_REG_PD_LUT1X						0xD2001104
#define	D_RRH_REG_PD_LUT2X						0xD2001108
#define	D_RRH_REG_PD_LUT3X						0xD200110C
#define	D_RRH_REG_PD_LUT4X						0xD2001110
#define	D_RRH_REG_PD_LUTTR						0xD2001120
#define	D_RRH_REG_PD_LUTWDIS					0xD2001140
#define	D_RRH_REG_PD_TRON						0xD2001180
#define	D_RRH_REG_PD_TRTIME1					0xD2001190
#define	D_RRH_REG_PD_TRTIME2					0xD2001194
#define	D_RRH_REG_PD_TRTIME3					0xD2001198
#define	D_RRH_REG_PD_TRTIME4					0xD200119C
#define	D_RRH_REG_PD_TRTIME5					0xD20011A0
#define	D_RRH_REG_PD_TRTIME6					0xD20011A4
#define	D_RRH_REG_PD_TRTIME7					0xD20011A8
#define	D_RRH_REG_PD_TRTIME8					0xD20011AC
#define	D_RRH_REG_PD_MCGAIN						0xD2001200
#define	D_RRH_REG_PD_RFGAIN						0xD2001210
#define	D_RRH_REG_PD_RFDCO						0xD2001214
#define	D_RRH_REG_PD_APDON						0xD2001300
#define	D_RRH_REG_PD_DPDOVF						0xD2001340
#define	D_RRH_REG_PD_AVELUTX0					0xD2001350
#define	D_RRH_REG_PD_AVELUTX1					0xD2001354
#define	D_RRH_REG_PD_AVELUTX2					0xD2001358
#define	D_RRH_REG_PD_AVELUTX3					0xD200135C
#define	D_RRH_REG_PD_AVELUTX4					0xD2001360
#define	D_RRH_REG_PD_PDMEMKEY					0xD20013F0
#define	D_RRH_REG_PD_PDLASTMODI					0xD20013F4
#define	D_RRH_REG_PD_PDTEST						0xD20013F8
#define	D_RRH_REG_PD_PDMONSEL					0xD20013FC

#elif defined (OPT_RRH_ARCH_ZYNQ)
#define	D_RRH_REG_PD_ADR1ADJ					0x82001000
#define	D_RRH_REG_PD_ADR2ADJ					0x82001004
#define	D_RRH_REG_PD_ADR1CNT					0x82001010
#define	D_RRH_REG_PD_ADR2CNT					0x82001014
#define	D_RRH_REG_PD_ADRRPT						0x82001040
#define	D_RRH_REG_PD_LUT0X						0x82001100
#define	D_RRH_REG_PD_LUT1X						0x82001104
#define	D_RRH_REG_PD_LUT2X						0x82001108
#define	D_RRH_REG_PD_LUT3X						0x8200110C
#define	D_RRH_REG_PD_LUT4X						0x82001110
#define	D_RRH_REG_PD_LUTTR						0x82001120
#define	D_RRH_REG_PD_LUTWDIS					0x82001140
#define	D_RRH_REG_PD_TRON						0x82001180
#define	D_RRH_REG_PD_TRTIME1					0x82001190
#define	D_RRH_REG_PD_TRTIME2					0x82001194
#define	D_RRH_REG_PD_TRTIME3					0x82001198
#define	D_RRH_REG_PD_TRTIME4					0x8200119C
#define	D_RRH_REG_PD_TRTIME5					0x820011A0
#define	D_RRH_REG_PD_TRTIME6					0x820011A4
#define	D_RRH_REG_PD_TRTIME7					0x820011A8
#define	D_RRH_REG_PD_TRTIME8					0x820011AC
#define	D_RRH_REG_PD_MCGAIN						0x82001200
#define	D_RRH_REG_PD_RFGAIN						0x82001210
#define	D_RRH_REG_PD_RFDCO						0x82001214
#define	D_RRH_REG_PD_APDON						0x82001300
#define	D_RRH_REG_PD_DPDOVF						0x82001340
#define	D_RRH_REG_PD_AVELUTX0					0x82001350
#define	D_RRH_REG_PD_AVELUTX1					0x82001354
#define	D_RRH_REG_PD_AVELUTX2					0x82001358
#define	D_RRH_REG_PD_AVELUTX3					0x8200135C
#define	D_RRH_REG_PD_AVELUTX4					0x82001360
#define	D_RRH_REG_PD_PDMEMKEY					0x820013F0
#define	D_RRH_REG_PD_PDLASTMODI					0x820013F4
#define	D_RRH_REG_PD_PDTEST						0x820013F8
#define	D_RRH_REG_PD_PDMONSEL					0x820013FC

#else
#endif
/* @} */

/*!
 * @name PDレジスタアドレス算出マクロ
 * @note PDレジスタアドレス算出マクロ, アドレスはこのマクロを使用する br は0 ~ 3である
 * @{
 */
#define M_RRH_REG_PD_GETADDR(ofs,br)		( UINT )( ofs  + ( br * D_RRH_REG_PD_BR) )
#define M_RRH_REG_PD_ADR1ADJ( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_ADR1ADJ     , br ))
#define M_RRH_REG_PD_ADR2ADJ( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_ADR2ADJ     , br ))
#define M_RRH_REG_PD_ADR1CNT( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_ADR1CNT     , br ))
#define M_RRH_REG_PD_ADR2CNT( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_ADR2CNT     , br ))
#define M_RRH_REG_PD_ADRRPT( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_ADRRPT      , br ))
#define M_RRH_REG_PD_LUT0X( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_LUT0X       , br ))
#define M_RRH_REG_PD_LUT1X( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_LUT1X       , br ))
#define M_RRH_REG_PD_LUT2X( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_LUT2X       , br ))
#define M_RRH_REG_PD_LUT3X( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_LUT3X       , br ))
#define M_RRH_REG_PD_LUT4X( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_LUT4X       , br ))
#define M_RRH_REG_PD_LUTTR( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_LUTTR       , br ))
#define M_RRH_REG_PD_LUTWDIS( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_LUTWDIS     , br ))
#define M_RRH_REG_PD_TRON( br )				( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_TRON        , br ))
#define M_RRH_REG_PD_TRTIME1( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_TRTIME1     , br ))
#define M_RRH_REG_PD_TRTIME2( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_TRTIME2     , br ))
#define M_RRH_REG_PD_TRTIME3( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_TRTIME3     , br ))
#define M_RRH_REG_PD_TRTIME4( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_TRTIME4     , br ))
#define M_RRH_REG_PD_TRTIME5( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_TRTIME5     , br ))
#define M_RRH_REG_PD_TRTIME6( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_TRTIME6     , br ))
#define M_RRH_REG_PD_TRTIME7( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_TRTIME7     , br ))
#define M_RRH_REG_PD_TRTIME8( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_TRTIME8     , br ))
#define M_RRH_REG_PD_MCGAIN( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_MCGAIN      , br ))
#define M_RRH_REG_PD_RFGAIN( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_RFGAIN      , br ))
#define M_RRH_REG_PD_RFDCO( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_RFDCO       , br ))
#define M_RRH_REG_PD_APDON( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_APDON       , br ))
#define M_RRH_REG_PD_DPDOVF( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_DPDOVF      , br ))
#define M_RRH_REG_PD_AVELUTX0( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_AVELUTX0    , br ))
#define M_RRH_REG_PD_AVELUTX1( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_AVELUTX1    , br ))
#define M_RRH_REG_PD_AVELUTX2( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_AVELUTX2    , br ))
#define M_RRH_REG_PD_AVELUTX3( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_AVELUTX3    , br ))
#define M_RRH_REG_PD_AVELUTX4( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_AVELUTX4    , br ))
#define M_RRH_REG_PD_PDMEMKEY( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_PDMEMKEY    , br ))
#define M_RRH_REG_PD_PDLASTMODI( br )		( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_PDLASTMODI  , br ))
#define M_RRH_REG_PD_PDTEST( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_PDTEST      , br ))
#define M_RRH_REG_PD_PDMONSEL( br )			( M_RRH_REG_PD_GETADDR( D_RRH_REG_PD_PDMONSEL    , br ))
/* @} */

/*!
 * @name レジスタ値
 * @note レジスタ値
 * @{
 */
/* MCGAIN */
#define D_RRH_PD_MCGAIN_MCGAIN					0x00003FFF

/* RFGAIN */
#define D_RRH_PD_RFGAIN_RFGAIN					0x0000FFFF
/* @} */

#endif
/* @} */
