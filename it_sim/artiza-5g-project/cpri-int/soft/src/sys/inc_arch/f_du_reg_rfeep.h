/*!
 * @skip  $ld:$
 * @file  f_du_reg_rfeep.h
 * @brief System共通定義
 * @date  2018/08/09	KCN)takagi	New Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_RFEEP_H
#define F_DU_REG_RFEEP_H

/*!
 * @name EEPROMアドレスオフセット(共通部)
 * @note EEPROMアクセス用のアドレスオフセット
 * @{
 */
#define	D_DU_I2C_RF_EEP_DATANUM						0x0400
#define	D_DU_I2C_RF_EEP_TR_OFS						0x0400  /* 5GDU_chg */

//RFボードインベントリ情報
#define	D_DU_RF_EEP_MAKE_DATE1						0x0010				/* 製造番号(年/月)				*/
#define	D_DU_RF_EEP_MAKE_DATE2						0x0011

#define	D_DU_RF_EEP_MAKE_SERIAL_H1					0x0014				/* 製造番号(シリアル上位)		*/
#define	D_DU_RF_EEP_MAKE_SERIAL_H2					0x0015
#define	D_DU_RF_EEP_MAKE_SERIAL_L1					0x0016				/* 製造番号(シリアル下位)		*/
#define	D_DU_RF_EEP_MAKE_SERIAL_L2					0x0017

#define	D_DU_RF_EEP_TEST_YEAR1						0x0018				/* 試験日(年)					*/
#define	D_DU_RF_EEP_TEST_YEAR2						0x0019
#define	D_DU_RF_EEP_TEST_MONDAY1					0x001A				/* 試験日(月日)					*/
#define	D_DU_RF_EEP_TEST_MONDAY2					0x001B

#define	D_DU_RF_EEP_HARD_VER1						0x001C				/* ハードバージョン(総合版数)	*/
#define	D_DU_RF_EEP_HARD_VER2						0x001D
#define	D_DU_RF_EEP_HARD_VER3						0x001E
#define	D_DU_RF_EEP_HARD_VER4						0x001F
#define	D_DU_RF_EEP_HARD_VER5						0x0020
#define	D_DU_RF_EEP_HARD_VER6						0x0021

#define	D_DU_RF_EEP_CARD_NO							0x0000				/* カード図番					*/


/*!
 * @name EEPROMアドレスオフセット(トレーニング部)
 * @note EEPROMアクセス用のアドレスオフセット
 * @{
 */
 
#define	D_DU_RF_EEP_TRA_ALLDATAT					0x800			/* EEPROM全データ数(共通部、トレーニング部)	*/
#define	D_DU_RF_EEP_TRA_TXCNT_NML					0x100			/* トレーニングデータ数(Tx) NMLモード		*/
#define	D_DU_RF_EEP_TRA_TXCNT_PWRSAVE				0x100			/* トレーニングデータ数(Tx) PWRSAVEモード	*/
#define	D_DU_RF_EEP_TRA_RXCNT_NML					0x100			/* トレーニングデータ数(Rx) NMLモード		*/
#define	D_DU_RF_EEP_TRA_RXCNT_PWRSAVE				0x100			/* トレーニングデータ数(Rx) PWRSAVEモード	*/


//トレーニングフラグ 通常電力モード
#define D_DU_RF_EEP_N_QMOD_V_FLAG					0x030
#define D_DU_RF_EEP_N_QMOD_H_FLAG					0x031
#define D_DU_RF_EEP_N_TXVATT_V_FLAG					0x032
#define D_DU_RF_EEP_N_TXVATT_H_FLAG					0x033
#define D_DU_RF_EEP_N_QMOD_DAC_V_FLAG				0x034
#define D_DU_RF_EEP_N_QMOD_DAC_H_FLAG				0x035
#define D_DU_RF_EEP_N_AMP_DAC_V_FLAG				0x036
#define D_DU_RF_EEP_N_AMP_DAC_H_FLAG				0x037
#define D_DU_RF_EEP_N_QDEM_V_FLAG					0x039
#define D_DU_RF_EEP_N_QDEM_H_FLAG					0x03A
#define D_DU_RF_EEP_N_RXVATT_V_FLAG					0x03B
#define D_DU_RF_EEP_N_RXVATT_H_FLAG					0x03C
#define D_DU_RF_EEP_N_QDEM_DAC_V_FLAG				0x03D
#define D_DU_RF_EEP_N_QDEM_DAC_H_FLAG				0x03E


//トレーニングフラグ 低消費電力モード
#define D_DU_RF_EEP_L_QMOD_V_FLAG					0x040
#define D_DU_RF_EEP_L_QMOD_H_FLAG					0x041
#define D_DU_RF_EEP_L_TXVATT_V_FLAG					0x042
#define D_DU_RF_EEP_L_TXVATT_H_FLAG					0x043
#define D_DU_RF_EEP_L_QMOD_DAC_V_FLAG				0x044
#define D_DU_RF_EEP_L_QMOD_DAC_H_FLAG				0x045
#define D_DU_RF_EEP_L_AMP_DAC_V_FLAG				0x046
#define D_DU_RF_EEP_L_AMP_DAC_H_FLAG				0x047
#define D_DU_RF_EEP_L_QDEM_V_FLAG					0x049
#define D_DU_RF_EEP_L_QDEM_H_FLAG					0x04A
#define D_DU_RF_EEP_L_RXVATT_V_FLAG					0x04B
#define D_DU_RF_EEP_L_RXVATT_H_FLAG					0x04C
#define D_DU_RF_EEP_L_QDEM_DAC_V_FLAG				0x04D
#define D_DU_RF_EEP_L_QDEM_DAC_H_FLAG				0x04E


//TXトレーニング情報 通常電力モード
#define D_DU_RF_EEP_N_QMOD_IM_I_V					0x410
#define D_DU_RF_EEP_N_QMOD_IM_Q_V					0x412
#define D_DU_RF_EEP_N_QMOD_LO_I_V					0x414
#define D_DU_RF_EEP_N_QMOD_LO_Q_V					0x416
#define D_DU_RF_EEP_N_QMOD_IM_I_H					0x418
#define D_DU_RF_EEP_N_QMOD_IM_Q_H					0x41A
#define D_DU_RF_EEP_N_QMOD_LO_I_H					0x41C
#define D_DU_RF_EEP_N_QMOD_LO_Q_H					0x41E
#define D_DU_RF_EEP_N_TXVATT_V						0x424
#define D_DU_RF_EEP_N_TXVATT_H						0x425
#define D_DU_RF_EEP_N_QMOD_DAC_V					0x42C
#define D_DU_RF_EEP_N_QMOD_DAC_H					0x42E
#define D_DU_RF_EEP_N_AMP_DAC_V						0x434
#define D_DU_RF_EEP_N_AMP_DAC_H						0x436
#define D_DU_RF_EEP_N_TXCRC16						0x4FE


//TXトレーニング情報 低電力モード
#define D_DU_RF_EEP_L_QMOD_IM_I_V					0x510
#define D_DU_RF_EEP_L_QMOD_IM_Q_V					0x512
#define D_DU_RF_EEP_L_QMOD_LO_I_V					0x514
#define D_DU_RF_EEP_L_QMOD_LO_Q_V					0x516
#define D_DU_RF_EEP_L_QMOD_IM_I_H					0x518
#define D_DU_RF_EEP_L_QMOD_IM_Q_H					0x51A
#define D_DU_RF_EEP_L_QMOD_LO_I_H					0x51C
#define D_DU_RF_EEP_L_QMOD_LO_Q_H					0x51E
#define D_DU_RF_EEP_L_TXVATT_V						0x524
#define D_DU_RF_EEP_L_TXVATT_H						0x525
#define D_DU_RF_EEP_L_QMOD_DAC_V					0x52C
#define D_DU_RF_EEP_L_QMOD_DAC_H					0x52E
#define D_DU_RF_EEP_L_AMP_DAC_V						0x534
#define D_DU_RF_EEP_L_AMP_DAC_H						0x536
#define D_DU_RF_EEP_L_TXCRC16						0x5FE


//RXトレーニング情報 通常電力モード
#define D_DU_RF_EEP_N_QDEM_IM_V						0x610
#define D_DU_RF_EEP_N_QDEM_IM_H						0x612
#define D_DU_RF_EEP_N_RXVATT_V						0x618
#define D_DU_RF_EEP_N_RXVATT_H						0x619
#define D_DU_RF_EEP_N_QDEM_DAC_V					0x620
#define D_DU_RF_EEP_N_QDEM_DAC_H					0x622
#define D_DU_RF_EEP_N_RXCRC16						0x6FE


//RXトレーニング情報 低電力モード
#define D_DU_RF_EEP_L_QDEM_IM_V						0x710
#define D_DU_RF_EEP_L_QDEM_IM_H						0x712
#define D_DU_RF_EEP_L_RXVATT_V						0x718
#define D_DU_RF_EEP_L_RXVATT_H						0x719
#define D_DU_RF_EEP_L_QDEM_DAC_V					0x720
#define D_DU_RF_EEP_L_QDEM_DAC_H					0x722
#define D_DU_RF_EEP_L_RXCRC16						0x7FE


/****************************/
/* Tx関連設定情報 NMLモード	*/
/****************************/
#define	D_DU_RF_EEP_TRA_TX_LEN_NML					0x0400			/* TXのLength情報 NMLモード		*/
#define	D_DU_RF_EEP_TRA_TX_OFS_NML					0x0000			/* TXのLength情報 NMLモード		*/
#define	D_DU_RF_EEP_TRA_TX_CRC16_NML				0x00FE			/* CRC16値(Tx) NMLモード		*/


/********************************/
/* Tx関連設定情報 PWRSAVEモード	*/
/********************************/
#define	D_DU_RF_EEP_TRA_TX_LEN_PWRSAVE				0x0500			/* TXのLength情報 PWRSAVEモード	*/
#define	D_DU_RF_EEP_TRA_TX_OFS_PWRSAVE				0x0100			/* TXのLength情報 PWRSAVEモード	*/
#define	D_DU_RF_EEP_TRA_TX_CRC16_PWRSAVE			0x01FE			/* CRC16値(Tx) PWRSAVEモード	*/

/****************************/
/* Rx関連設定情報 NMLモード	*/
/****************************/

#define	D_DU_RF_EEP_TRA_RX_LEN_NML					0x0600			/* RXのLength情報 NMLモード			*/
#define	D_DU_RF_EEP_TRA_RX_OFS_NML					0x0200			/* RXのLength情報 NMLモード			*/
#define	D_DU_RF_EEP_TRA_RX_CRC16_NML				0x02FE			/* CRC16値(Rx) NMLモード			*/

/********************************/
/* Rx関連設定情報 PWRSAVEモード	*/
/********************************/
#define	D_DU_RF_EEP_TRA_RX_LEN_PWRSAVE				0x0700			/* RXのLength情報 PWRSAVEモード		*/
#define	D_DU_RF_EEP_TRA_RX_OFS_PWRSAVE				0x0300			/* RXのLength情報 PWRSAVEモード		*/
#define	D_DU_RF_EEP_TRA_RX_CRC16_PWRSAVE			0x03FE			/* CRC16値(Rx) PWRSAVEモード		*/

 
/* @} */
#endif
/* @} */

