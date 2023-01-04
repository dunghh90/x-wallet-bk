/*!
 * @skip  $ld:$
 * @file  f_com_dpda_init_RFtrainingDataRead_Rx_mmW.c
 * @brief mmW RF RX部トレーニングデータリード処理
 * @date  2018/08/10 KCN)takagi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_COM
 * @{
 */

//#include "f_com_inc.h"
#include "f_dpda_inc.h"

/************************************************************************************************************/
/*!
 * @brief 関数機能概要:mmW用のRF RX部トレーニングデータのリード処理を行う
 * @note  関数処理内容.
 *		-# EEPROMの全データをリードする
 *		-# リードしたRX情報についてCRC16計算を行う(NMLモード領域)
 *		-# CRC16計算結果とEEPROM内のCRC16を判定
 *			- 値が異なる場合
 *				-# 内部テーブルをDefault値で更新する
 *		-# リードしたTX情報についてCRC16計算を行う(PWRSABEモード領域)
 *		-# CRC16計算結果とEEPROM内のCRC16を判定
 *			- 値が異なる場合
 *				-# 内部テーブルをDefault値で更新する
 * @param  mmw_pwr_mode			[in]UCHAR	mmW電力モード(01:低電力モード 01以外:通常モード
 * @return VOID
 * @warning   N/A
 * @FeatureID PF-Rfp-001-001-001
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2018/08/10 KCN)takagi Create
 */
/************************************************************************************************************/
VOID f_com_dpda_init_RFtrainingDataRead_Rx_mmW(VOID)
{
	UINT						uiEepCnt;			/* EEPROMデータ取得回数				*/
	USHORT						usCrc16 = 0;		/* CRC16計算結果					*/
	USHORT						usTraData;
	USHORT						crcPos;
	USHORT						crcSetData;
	
	/* f_com_dpda_init_RFtrainingDataRead_Tx_mmWで             */
	/* 全EEPROMデータをリードしているのでここでは読み込まない */

	/****************************************************************************************/
	/* CRC16計算(Rx関連設定情報 NMLモード領域)												*/
	/****************************************************************************************/
	
	/* RXトレーニングデータのCRC16を除く部分について、CRC16計算を行う */
	for( uiEepCnt = D_DU_RF_EEP_TRA_RX_OFS_NML;
		 uiEepCnt < D_DU_RF_EEP_TRA_RX_OFS_NML + D_DU_RF_EEP_TRA_RXCNT_NML - 2;
		 uiEepCnt+=2 )
	{
		/* CRC16計算 */
		memcpy( &usTraData, &f_comw_rftrainingData->traVal[uiEepCnt], sizeof(usTraData) );
		BPF_COM_END_BIGTOLITTLE16(usTraData);
		
		f_com_dpda_init_addCrc( usTraData, &usCrc16 );
		
	}
	
	/* CRC16計算結果とEEPROMにあったCRC16計算結果比較 */
	crcPos = D_DU_RF_EEP_TRA_RX_CRC16_NML;
	memcpy( &usTraData, &f_comw_rftrainingData->traVal[crcPos], sizeof(usTraData) );
	BPF_COM_END_BIGTOLITTLE16(usTraData);
	
	if( usCrc16 != usTraData )
	{
		/* 異なる場合はテーブル内のトレーニングデータをDefault値で更新 */
		memcpy( &f_comw_rftrainingData->traVal[D_DU_RF_EEP_TRA_RX_OFS_NML], f_comr_RftrainingRx_mmW_NML, sizeof(f_comr_RftrainingRx_mmW_NML));

		crcSetData = 0xCDCD;  /* CRC NGの時にCRCエリアに設定する値 */

		/* assert処理 */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "mmW RF NML mode Rx CRC16 error.(0x%04x)", usCrc16 );
		
		/* アラーム通知 */
		f_com_abort( D_SYS_THDID_PF_CCA, D_DU_ALMCD_RX_TRA_PRM_NG1 );
	}
	else
	{
		crcSetData = 0xABAB;  /* CRC OKの時にCRCエリアに設定する値 */
	}
	memcpy( &f_comw_rftrainingData->traVal[crcPos], &crcSetData, sizeof(crcSetData) );
	
	return;
}

/* @} */
