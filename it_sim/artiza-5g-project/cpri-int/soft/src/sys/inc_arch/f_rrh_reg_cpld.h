/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_cpld.h
 * @brief レジスタアドレス定義ヘッダ(CPLD)
 * @date  2015/3/30 FJT) Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_CPLD_H
#define F_RRH_REG_CPLD_H

/***** 5G-DUハソ仕様書対応	*****/
/*!
 * @name CPLDレジスタアドレス
 * @note CPLDレジスタアドレス
 * @{
 */
#define	D_RRH_REG_CPLD_VER						0xE000							/* CPLDバージョン情報						*/
#define	D_RRH_REG_CPLD_REV						0xE001							/* CPLDリビジョン情報						*/
#define	D_RRH_REG_CPLD_RWCHK1					0xE004							/* ライト／リードチェック(1)				*/
#define	D_RRH_REG_CPLD_RWCHK2					0xE005							/* ライト／リードチェック(2)				*/
#define	D_RRH_REG_CPLD_RST_FACT					0xE020							/* リセット要因								*/
#define	D_RRH_REG_CPLD_PS_STATE					0xE021							/* PSステータス								*/
#define	D_RRH_REG_CPLD_PS_STATE_FLG1			0xE024							/* 電源状態フラグ1							*/
#define	D_RRH_REG_CPLD_PS_STATE_FLG2			0xE025							/* 電源状態フラグ2							*/
#define	D_RRH_REG_CPLD_PS_STATE_FLG3			0xE026							/* 電源状態フラグ3							*/
#define	D_RRH_REG_CPLD_RST_MSK					0xE028							/* リセットマスク							*/
#define	D_RRH_REG_CPLD_PS_STATE_MSK1			0xE02C							/* 電源状態フラグ1マスク					*/
#define	D_RRH_REG_CPLD_PS_STATE_MSK2			0xE02D							/* 電源状態フラグ2マスク					*/
#define	D_RRH_REG_CPLD_PS_STATE_MSK3			0xE02E							/* 電源状態フラグ3マスク					*/
#define	D_RRH_REG_CPLD_UART						0xE040							/* UART切替									*/
#define	D_RRH_REG_CPLD_SV_ERR					0xE044							/* SVI/F エラー表示							*/
#define	D_RRH_REG_CPLD_ANAIN1_1					0xE080							/* アナログ入力ANAIN1 ADCリード値			*/
#define	D_RRH_REG_CPLD_ANAIN1_2					0xE081							/* 											*/
#define	D_RRH_REG_CPLD_ADC1IN1_1				0xE082							/* アナログ入力ADC1IN１ ADCリード値			*/
#define	D_RRH_REG_CPLD_ADC1IN1_2				0xE083							/* 											*/
#define	D_RRH_REG_CPLD_ADC1IN2_1				0xE084							/* アナログ入力ADC1IN2 ADCリード値			*/
#define	D_RRH_REG_CPLD_ADC1IN2_2				0xE085							/* 											*/
#define	D_RRH_REG_CPLD_ADC1IN3_1				0xE086							/* アナログ入力ADC1IN3 ADCリード値			*/
#define	D_RRH_REG_CPLD_ADC1IN3_2				0xE087							/* 											*/
#define	D_RRH_REG_CPLD_ADC1IN4_1				0xE088							/* アナログ入力ADC1IN4 ADCリード値			*/
#define	D_RRH_REG_CPLD_ADC1IN4_2				0xE089							/* 											*/
#define	D_RRH_REG_CPLD_ADC1IN5_1				0xE08A							/* アナログ入力ADC1IN5 ADCリード値			*/
#define	D_RRH_REG_CPLD_ADC1IN5_2				0xE08B							/* 											*/
#define	D_RRH_REG_CPLD_ADC1IN6_1				0xE08C							/* アナログ入力ADC1IN6 ADCリード値			*/
#define	D_RRH_REG_CPLD_ADC1IN6_2				0xE08D							/* 											*/
#define	D_RRH_REG_CPLD_ADC1IN7_1				0xE08E							/* アナログ入力ADC1IN7 ADCリード値			*/
#define	D_RRH_REG_CPLD_ADC1IN7_2				0xE08F							/* 											*/
#define	D_RRH_REG_CPLD_ADC1IN8_1				0xE090							/* アナログ入力ADC1IN8 ADCリード値			*/
#define	D_RRH_REG_CPLD_ADC1IN8_2				0xE091							/* 											*/
#define	D_RRH_REG_CPLD_ANAIN2_1					0xE092							/* アナログ入力ANAIN2 ADCリード値			*/
#define	D_RRH_REG_CPLD_ANAIN2_2					0xE093							/* 											*/
#define	D_RRH_REG_CPLD_ADC2IN1_1				0xE094							/* アナログ入力ADC2IN1 ADCリード値			*/
#define	D_RRH_REG_CPLD_ADC2IN1_2				0xE095							/* 											*/
#define	D_RRH_REG_CPLD_ADC2IN2_1				0xE096							/* アナログ入力ADC2IN2 ADCリード値			*/
#define	D_RRH_REG_CPLD_ADC2IN2_2				0xE097							/* 											*/
#define	D_RRH_REG_CPLD_ADC2IN3_1				0xE098							/* アナログ入力ADC2IN3 ADCリード値			*/
#define	D_RRH_REG_CPLD_ADC2IN3_2				0xE099							/* 											*/
#define	D_RRH_REG_CPLD_ADC2IN4_1				0xE09A							/* アナログ入力ADC2IN4 ADCリード値			*/
#define	D_RRH_REG_CPLD_ADC2IN4_2				0xE09B							/* 											*/
#define	D_RRH_REG_CPLD_ADC2IN5_1				0xE09C							/* アナログ入力ADC2IN5 ADCリード値			*/
#define	D_RRH_REG_CPLD_ADC2IN5_2				0xE09D							/* 											*/
#define	D_RRH_REG_CPLD_ADC2IN6_1				0xE09E							/* アナログ入力ADC2IN6 ADCリード値			*/
#define	D_RRH_REG_CPLD_ADC2IN6_2				0xE09F							/* 											*/
#define	D_RRH_REG_CPLD_ADC2IN7_1				0xE0A0							/* アナログ入力ADC2IN7 ADCリード値			*/
#define	D_RRH_REG_CPLD_ADC2IN7_2				0xE0A1							/* 											*/
#define	D_RRH_REG_CPLD_ADC2IN8_1				0xE0A2							/* アナログ入力ADC2IN8 ADCリード値			*/
#define	D_RRH_REG_CPLD_ADC2IN8_2				0xE0A3							/* 											*/
#define	D_RRH_REG_CPLD_RST						0xE0E0							/* 個別リセット								*/
#define	D_RRH_REG_CPLD_SV_TEST					0xE0E4							/* SVテスト									*/
#define	D_RRH_REG_CPLD_DBG_VER					0xE0F0							/* デバッグバージョン情報					*/
/* @} */
/*!
 * @name CPLDレジスタbit & Shift
 * @note CPLDレジスタbit & Shift
 * @{
 */

/* @} */

/***** 3.5G-RRE 4TRXハソ仕様書対応 *****/
/*!
 * @name MMICPLDレジスタアドレス
 * @note MMICPLDレジスタアドレス
 * @{
 */
#define D_RRH_REG_CPLD_MMI_VER				0xE800
#define D_RRH_REG_CPLD_MMI_CHECK			0xE804
#define D_RRH_REG_CPLD_MMI_SFPRATE			0xE828
#define D_RRH_REG_CPLD_MMI_SFPSTS			0xE82A
#define D_RRH_REG_CPLD_MMI_SFPI2CEN			0xE82E
#define D_RRH_REG_CPLD_MMI_SWSTS			0xE840
#define D_RRH_REG_CPLD_MMI_PORTOUT			0xE860
#define D_RRH_REG_CPLD_MMI_PORTIN			0xE862
#define D_RRH_REG_CPLD_MMI_PSALM			0xE868
#define D_RRH_REG_CPLD_MMI_LED				0xE870
#define D_RRH_REG_CPLD_MMI_CPRILNK_LED		0xE872
#define D_RRH_REG_CPLD_MMI_TEST				0xE8E0
#define D_RRH_REG_CPLD_MMI_SVDIS			0xE8E2
#define D_RRH_REG_CPLD_MMI_SVTEST			0xE8E4
#define D_RRH_REG_CPLD_MMI_SVSTS			0xE8E6
/* @} */

/*!
 * @name LCCPLDレジスタアドレス(SPI0)
 * @note LCCPLDレジスタアドレス(SPI0)
 * @{
 */
#define D_RRH_REG_CPLD_LCC_SPI0_CPLDVER		0x0000
#define D_RRH_REG_CPLD_LCC_SPI0_CHECK		0x0004
#define D_RRH_REG_CPLD_LCC_SPI0_RSTREQ		0x0030						/*!< soft reset */
#define D_RRH_REG_CPLD_LCC_SPI0_RSTST		0x0032
#define D_RRH_REG_CPLD_LCC_SPI0_RSTMSK		0x0034
#define D_RRH_REG_CPLD_LCC_SPI0_PLLRREQ		0x0038
#define D_RRH_REG_CPLD_LCC_SPI0_PHYRREQ		0x003A
#define D_RRH_REG_CPLD_LCC_SPI0_SWST		0x0040
#define D_RRH_REG_CPLD_LCC_SPI0_CLKSEL		0x0042
#define D_RRH_REG_CPLD_LCC_SPI0_RFCNTSTA	0x0050
#define D_RRH_REG_CPLD_LCC_SPI0_RFCNTSTB	0x0052
#define D_RRH_REG_CPLD_LCC_SPI0_RFCNTSTC	0x0054
#define D_RRH_REG_CPLD_LCC_SPI0_RFCNTSTD	0x0056
#define D_RRH_REG_CPLD_LCC_SPI0_XPSBUF		0x0058
#define D_RRH_REG_CPLD_LCC_SPI0_AISGPSEN	0x005C
#define D_RRH_REG_CPLD_LCC_SPI0_TDDALM		0x0068
#define D_RRH_REG_CPLD_LCC_SPI0_CNTTEST		0x00E0
#define D_RRH_REG_CPLD_LCC_SPI0_PORREQ		0x00E8
#define D_RRH_REG_CPLD_LCC_SPI0_RSTREQ_DEV	0x00EA						/*!< DDR or QSPI or .. reset */
#define D_RRH_REG_CPLD_LCC_SPI0_DIPSW		0x00EC
#define	D_RRH_REG_CPLD_LCC_SPI0_BR			0x0020
#define D_RRH_REG_CPLD_LCC_SPI0_PATXDLY		0x0100
#define D_RRH_REG_CPLD_LCC_SPI0_PADDLY		0x0102
#define D_RRH_REG_CPLD_LCC_SPI0_TXTXDLY		0x0104
#define D_RRH_REG_CPLD_LCC_SPI0_TXDDLY		0x0106
#define D_RRH_REG_CPLD_LCC_SPI0_RSTXDLY		0x0108
#define D_RRH_REG_CPLD_LCC_SPI0_RSDDLY		0x010A
#define D_RRH_REG_CPLD_LCC_SPI0_LATXDLY		0x010C
#define D_RRH_REG_CPLD_LCC_SPI0_LADDLY		0x010E
#define D_RRH_REG_CPLD_LCC_SPI0_TX2TXDLY	0x0110
#define D_RRH_REG_CPLD_LCC_SPI0_TX2DDLY		0x0112
#define D_RRH_REG_CPLD_LCC_SPI0_TDCNTA		0x0118
#define D_RRH_REG_CPLD_LCC_SPI0_TDCNT2A		0x011A
#define D_RRH_REG_CPLD_LCC_SPI0_PSEN		0x011C
#define M_RRH_REG_CPLD_LCC_SPI0_GETADDR(ofs,br)			( UINT )( ofs  + ( br * D_RRH_REG_CPLD_LCC_SPI0_BR ) )
#define M_RRH_REG_CPLD_LCC_SPI0_PATXDLY( br )		( M_RRH_REG_CPLD_LCC_SPI0_GETADDR(  D_RRH_REG_CPLD_LCC_SPI0_PATXDLY      , br ))
#define M_RRH_REG_CPLD_LCC_SPI0_PADDLY( br )		( M_RRH_REG_CPLD_LCC_SPI0_GETADDR(  D_RRH_REG_CPLD_LCC_SPI0_PADDLY       , br ))
#define M_RRH_REG_CPLD_LCC_SPI0_TXTXDLY( br )		( M_RRH_REG_CPLD_LCC_SPI0_GETADDR(  D_RRH_REG_CPLD_LCC_SPI0_TXTXDLY      , br ))
#define M_RRH_REG_CPLD_LCC_SPI0_TXDDLY( br )		( M_RRH_REG_CPLD_LCC_SPI0_GETADDR(  D_RRH_REG_CPLD_LCC_SPI0_TXDDLY       , br ))
#define M_RRH_REG_CPLD_LCC_SPI0_RSTXDLY( br )		( M_RRH_REG_CPLD_LCC_SPI0_GETADDR(  D_RRH_REG_CPLD_LCC_SPI0_RSTXDLY      , br ))
#define M_RRH_REG_CPLD_LCC_SPI0_RSDDLY( br )		( M_RRH_REG_CPLD_LCC_SPI0_GETADDR(  D_RRH_REG_CPLD_LCC_SPI0_RSDDLY       , br ))
#define M_RRH_REG_CPLD_LCC_SPI0_LATXDLY( br )		( M_RRH_REG_CPLD_LCC_SPI0_GETADDR(  D_RRH_REG_CPLD_LCC_SPI0_LATXDLY      , br ))
#define M_RRH_REG_CPLD_LCC_SPI0_LADDLY( br )		( M_RRH_REG_CPLD_LCC_SPI0_GETADDR(  D_RRH_REG_CPLD_LCC_SPI0_LADDLY       , br ))
#define M_RRH_REG_CPLD_LCC_SPI0_TX2TXDLY( br )		( M_RRH_REG_CPLD_LCC_SPI0_GETADDR(  D_RRH_REG_CPLD_LCC_SPI0_TX2TXDLY     , br ))
#define M_RRH_REG_CPLD_LCC_SPI0_TX2DDLY( br )		( M_RRH_REG_CPLD_LCC_SPI0_GETADDR(  D_RRH_REG_CPLD_LCC_SPI0_TX2DDLY      , br ))
#define M_RRH_REG_CPLD_LCC_SPI0_TDCNTA( br )		( M_RRH_REG_CPLD_LCC_SPI0_GETADDR(  D_RRH_REG_CPLD_LCC_SPI0_TDCNTA       , br ))
#define M_RRH_REG_CPLD_LCC_SPI0_TDCNT2A( br )		( M_RRH_REG_CPLD_LCC_SPI0_GETADDR(  D_RRH_REG_CPLD_LCC_SPI0_TDCNT2A      , br ))
#define M_RRH_REG_CPLD_LCC_SPI0_PSEN( br )			( M_RRH_REG_CPLD_LCC_SPI0_GETADDR(  D_RRH_REG_CPLD_LCC_SPI0_PSEN         , br ))
/* @} */

/*!
 * @name LCCPLDレジスタアドレス(SPI1)
 * @note LCCPLDレジスタアドレス(SPI1)
 * @{
 */
#define D_RRH_REG_CPLD_LCC_SPI1_CPLDVER		0x0000
#define D_RRH_REG_CPLD_LCC_SPI1_REGCHK		0x0004
#define D_RRH_REG_CPLD_LCC_SPI1_SPICNT		0x0010
/* @} */



/*!
 * @name レジスタ値
 * @note レジスタ値
 * @{
 */
#define D_RRH_CPLD_LCC_SPI0_RSTREQ			0x0001						/*!< soft reset */
/* @} */



#endif
/* @} */
