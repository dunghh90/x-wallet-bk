/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_rx.h
 * @brief レジスタアドレス定義ヘッダ(RX)
 * @date  2015/3/27 FJT) Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_RX_H
#define F_RRH_REG_RX_H
#if 0
/***** 5G-DUハソ仕様書対応	*****/
/***** 機能未対応のため、既存defineを有効化	*****/
/*!
 * @name RXレジスタアドレス
 * @note RXレジスタアドレス
 * @{
 */
#define	D_RRH_REG_RX_RXCAREN					(D_RRH_REG_RX + 0x00000000)	/* RxEnable									*/
#define	D_RRH_REG_RX_RXCNF0						(D_RRH_REG_RX + 0x00000010)	/* System (Cr0 config 信号帯域設定)			*/
#define	D_RRH_REG_RX_RXCNF1						(D_RRH_REG_RX + 0x00000014)	/* System (Cr1 config 信号帯域設定)			*/
#define	D_RRH_REG_RX_RXPRM0						(D_RRH_REG_RX + 0x00000030)	/* Parameter (Cr0 遅延調整値(粗:1/3.84MHz))	*/
#define	D_RRH_REG_RX_RXPRM1						(D_RRH_REG_RX + 0x00000034)	/* Parameter (Cr1 遅延調整値(粗:1/3.84MHz))	*/
#define	D_RRH_REG_RX_RXDGCGAIN0					(D_RRH_REG_RX + 0x00000040)	/* Parameter (Cr0 GAIN調整値)				*/
#define	D_RRH_REG_RX_RXDGCGAIN1					(D_RRH_REG_RX + 0x00000044)	/* Parameter (Cr1 GAIN調整値)				*/
#define	D_RRH_REG_RX_CHF0						(D_RRH_REG_RX + 0x00000050)	/* Parameter (Cr0 CHF係数)					*/
#define	D_RRH_REG_RX_CHF1						(D_RRH_REG_RX + 0x00000054)	/* Parameter (Cr1 CHF係数)					*/
#define	D_RRH_REG_RX_RXANTEN					(D_RRH_REG_RX + 0x00000080)	/* Antenna Enable							*/
#define	D_RRH_REG_RX_QCHINV						(D_RRH_REG_RX + 0x000000C0)	/* Qch INV									*/
#define	D_RRH_REG_RX_RXIVGAH					(D_RRH_REG_RX + 0x00000100)	/* RX AGC (IF-VGA ( High Input Mode))			*/
#define	D_RRH_REG_RX_RXIVGAM					(D_RRH_REG_RX + 0x00000104)	/* RX AGC (IF-VGA ( Mid Input Mode))			*/
#define	D_RRH_REG_RX_RXIVGAL					(D_RRH_REG_RX + 0x00000108)	/* RX AGC (IF-VGA ( Low Input Mode))			*/
#define	D_RRH_REG_RX_RXDVGAH					(D_RRH_REG_RX + 0x0000010C)	/* RX AGC (Digital-VGA ( High Input Mode))		*/
#define	D_RRH_REG_RX_RXDVGAM					(D_RRH_REG_RX + 0x00000110)	/* RX AGC (Digital-VGA ( Mid Input Mode))		*/
#define	D_RRH_REG_RX_RXDVGAL					(D_RRH_REG_RX + 0x00000114)	/* RX AGC (Digital-VGA ( Low Input Mode))		*/
#define	D_RRH_REG_RX_VGATRG						(D_RRH_REG_RX + 0x00000118)	/* RX AGC (VGA適用トリガ)						*/
#define	D_RRH_REG_RX_VGATIM						(D_RRH_REG_RX + 0x0000011C)	/* RX AGC (VGA適用timing調整)					*/
#define	D_RRH_REG_RX_SDLT						(D_RRH_REG_RX + 0x00000120)	/* RX AGC (Slow Detect Lower Threshold (H→M))	*/
#define	D_RRH_REG_RX_DWELLTIM					(D_RRH_REG_RX + 0x00000124)	/* RX AGC (Slow Detect Dwell Time)				*/
#define	D_RRH_REG_RX_RGMMON						(D_RRH_REG_RX + 0x00000130)	/* RX AGC (Rx Gain Mod表示)						*/
#define	D_RRH_REG_RX_DWELLCONT					(D_RRH_REG_RX + 0x00000134)	/* RX AGC (DWELLCONT表示)						*/
#define	D_RRH_REG_RX_VGACTRL					(D_RRH_REG_RX + 0x00000138)	/* RX AGC (VGA SW/HW Mode切り替え)				*/
#define	D_RRH_REG_RX_RXPOWREP					(D_RRH_REG_RX + 0x00000150)	/* POW (受信利得電力報告値(FPGA入力段測定))	*/
#define	D_RRH_REG_RX_RXPOWSMPL					(D_RRH_REG_RX + 0x00000154)	/* POW (受信利得電力累積サンプル数設定)		*/
#define	D_RRH_REG_RX_RXPOWLAT					(D_RRH_REG_RX + 0x00000158)	/* POW (受信利得電力保持トリガ)				*/
#define	D_RRH_REG_RX_RXPOW						(D_RRH_REG_RX + 0x0000015C)	/* POW (受信利得電力保持値クリア)			*/
#define	D_RRH_REG_RX_RTWPREP0					(D_RRH_REG_RX + 0x00000160)	/* RTWP (キャリア0RTWP報告値)				*/
#define	D_RRH_REG_RX_RTWPREP1					(D_RRH_REG_RX + 0x00000164)	/* RTWP (キャリア1RTWP報告値)				*/
#define	D_RRH_REG_RX_RTWPSMPL					(D_RRH_REG_RX + 0x00000168)	/* RTWP (RTWP用 累積サンプル数設定)			*/
#define	D_RRH_REG_RX_RTWPLAT					(D_RRH_REG_RX + 0x0000016C)	/* RTWP (RTWP用 電力保持トリガ)				*/
#define	D_RRH_REG_RX_RTWP						(D_RRH_REG_RX + 0x00000170)	/* RTWP (RTWP用 電力保持値クリア)			*/
#define	D_RRH_REG_RX_DLPOWREP					(D_RRH_REG_RX + 0x000001C0)	/* DLPOW (DL区間電力報告値(FPGA入力段測定))	*/
#define	D_RRH_REG_RX_DLPOWSMPL					(D_RRH_REG_RX + 0x000001C4)	/* DLPOW (DL区間電力累積サンプル数設定)		*/
#define	D_RRH_REG_RX_SREFMON					(D_RRH_REG_RX + 0x00000250)	/* SYSREF MON								*/
#define	D_RRH_REG_RX_RXNCOPH0					(D_RRH_REG_RX + 0x00000300)	/* TEST(NCO) (Cr0 周波数シフト初期位相設定)	*/
#define	D_RRH_REG_RX_RXNCOPH1					(D_RRH_REG_RX + 0x00000304)	/* TEST(NCO) (Cr1 周波数シフト初期位相設定)	*/
#define	D_RRH_REG_RX_RXMONFPSEL					(D_RRH_REG_RX + 0x000003E0)	/* モニタ遅延 (RXモニタFP選択)				*/
#define	D_RRH_REG_RX_RXMONFPDLY					(D_RRH_REG_RX + 0x000003E4)	/* モニタ遅延 (RXモニタFP遅延調整)			*/
#define	D_RRH_REG_RX_RXTEST						(D_RRH_REG_RX + 0x000003F8)	/* TESTモード								*/
#define	D_RRH_REG_RX_RXMONSEL					(D_RRH_REG_RX + 0x000003FC)	/* RXモニタ									*/
/* @} */
#else
/*!
 * @name RXレジスタアドレス
 * @note RXレジスタアドレス
 * @{
 */
#define	D_RRH_REG_RX_BR							0x00000400

#define	D_RRH_REG_RX_RXCAREN					0x82100000
#define	D_RRH_REG_RX_RXCNF0						0x82100010
#define	D_RRH_REG_RX_RXCNF1						0x82100014
#define	D_RRH_REG_RX_RXCPRM0					0x82100030
#define	D_RRH_REG_RX_RXCPRM1					0x82100034
#define	D_RRH_REG_RX_RXANTEN					0x82100080
#define	D_RRH_REG_RX_RXIVGAH					0x82100100
#define	D_RRH_REG_RX_RXIVGAM					0x82100104
#define	D_RRH_REG_RX_RXIVGAL					0x82100108
#define	D_RRH_REG_RX_RXDVGAH					0x8210010C
#define	D_RRH_REG_RX_RXDVGAM					0x82100110
#define	D_RRH_REG_RX_RXDVGAL					0x82100114
#define	D_RRH_REG_RX_VGATRG						0x82100118
#define	D_RRH_REG_RX_VGATIM						0x8210011C
#define	D_RRH_REG_RX_SDLT						0x82100120
#define	D_RRH_REG_RX_DWELLTIM					0x82100124
#define	D_RRH_REG_RX_RGMMON						0x82100130
#define	D_RRH_REG_RX_DWELLCONT					0x82100134
#define	D_RRH_REG_RX_VGACTRL					0x82100138
#define	D_RRH_REG_RX_RXPOWREP					0x82100150
#define	D_RRH_REG_RX_RXPOWSMPL					0x82100154
#define	D_RRH_REG_RX_RXPOWLAT					0x82100158
#define	D_RRH_REG_RX_RXPOW						0x8210015C
#define	D_RRH_REG_RX_RTWPREP0					0x82100160
#define	D_RRH_REG_RX_RTWPREP1					0x82100164
#define	D_RRH_REG_RX_RTWPSMPL					0x82100174
#define	D_RRH_REG_RX_RTWPLAT					0x82100178
#define	D_RRH_REG_RX_RTWP						0x8210017C
#define	D_RRH_REG_RX_VGSPSWTRG					0x82100200
#define	D_RRH_REG_RX_VGSPSWDATA					0x82100204
#define	D_RRH_REG_RX_SREFMON					0x82100250
#define	D_RRH_REG_RX_RXNCOPH0					0x82100300
#define	D_RRH_REG_RX_RXNCOPH1					0x82100304
#define	D_RRH_REG_RX_RXLPFCOE					0x82100350
#define	D_RRH_REG_RX_RXTEST						0x821003F8
#define	D_RRH_REG_RX_RXMONSEL					0x821003FC
/* @} */
#endif
/*!
 * @name RXレジスタアドレス算出マクロ
 * @note RXレジスタアドレス算出マクロ, アドレスはこのマクロを使用する br は0 ~ 3である
 * @{
 */
#define M_RRH_REG_RX_GETADDR(ofs,br)		( UINT )( ofs  + ( br * D_RRH_REG_RX_BR) )
#define M_RRH_REG_RX_RXCAREN( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXCAREN      , br ))
#define M_RRH_REG_RX_RXCNF0( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXCNF0       , br ))
#define M_RRH_REG_RX_RXCNF1( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXCNF1       , br ))
#define M_RRH_REG_RX_RXCPRM0( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXCPRM0      , br ))
#define M_RRH_REG_RX_RXCPRM1( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXCPRM1      , br ))
#define M_RRH_REG_RX_RXANTEN( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXANTEN      , br ))
#define M_RRH_REG_RX_RXIVGAH( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXIVGAH      , br ))
#define M_RRH_REG_RX_RXIVGAM( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXIVGAM      , br ))
#define M_RRH_REG_RX_RXIVGAL( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXIVGAL      , br ))
#define M_RRH_REG_RX_RXDVGAH( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXDVGAH      , br ))
#define M_RRH_REG_RX_RXDVGAM( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXDVGAM      , br ))
#define M_RRH_REG_RX_RXDVGAL( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXDVGAL      , br ))
#define M_RRH_REG_RX_VGATRG( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_VGATRG       , br ))
#define M_RRH_REG_RX_VGATIM( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_VGATIM       , br ))
#define M_RRH_REG_RX_SDLT( br )				( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_SDLT         , br ))
#define M_RRH_REG_RX_DWELLTIM( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_DWELLTIM     , br ))
#define M_RRH_REG_RX_RGMMON( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RGMMON       , br ))
#define M_RRH_REG_RX_DWELLCONT( br )		( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_DWELLCONT    , br ))
#define M_RRH_REG_RX_VGACTRL( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_VGACTRL      , br ))
#define M_RRH_REG_RX_RXPOWREP( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXPOWREP     , br ))
#define M_RRH_REG_RX_RXPOWSMPL( br )		( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXPOWSMPL    , br ))
#define M_RRH_REG_RX_RXPOWLAT( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXPOWLAT     , br ))
#define M_RRH_REG_RX_RXPOW( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXPOW        , br ))
#define M_RRH_REG_RX_RTWPREP0( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RTWPREP0     , br ))
#define M_RRH_REG_RX_RTWPREP1( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RTWPREP1     , br ))
#define M_RRH_REG_RX_RTWPSMPL( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RTWPSMPL     , br ))
#define M_RRH_REG_RX_RTWPLAT( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RTWPLAT      , br ))
#define M_RRH_REG_RX_RTWP( br )				( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RTWP         , br ))
#define M_RRH_REG_RX_VGSPSWTRG( br )		( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_VGSPSWTRG    , br ))
#define M_RRH_REG_RX_VGSPSWDATA( br )		( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_VGSPSWDATA   , br ))
#define M_RRH_REG_RX_SREFMON( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_SREFMON      , br ))
#define M_RRH_REG_RX_RXNCOPH0( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXNCOPH0     , br ))
#define M_RRH_REG_RX_RXNCOPH1( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXNCOPH1     , br ))
#define M_RRH_REG_RX_RXLPFCOE( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXLPFCOE     , br ))
#define M_RRH_REG_RX_RXTEST( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXTEST       , br ))
#define M_RRH_REG_RX_RXMONSEL( br )			( M_RRH_REG_RX_GETADDR( D_RRH_REG_RX_RXMONSEL     , br ))
/* @} */

/*!
 * @name RXレジスタbit
 * @note RXレジスタbit
 * @{
 */
/* 受信利得電力保持トリガ */
#define D_RRH_RX_RXPOWLAT_LAT			0x00000001

/* RTWP用 電力保持トリガ */
#define D_RRH_RX_RTWPLAT_LAT			0x00000001
/* @} */

#endif
/* @} */
