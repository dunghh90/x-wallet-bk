/*!
 * @skip  $ld:$
 * @file  f_com_dpda_init_trainingDataRead_Tx.c (base:f_dpda_init_trainingDataRead_Tx.c)
 * @brief DPDAタスク TX部トレーニングデータリード処理
 * @date  2011/07/26 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2013
 */

/*!
 * @addtogroup RRH_PF_COM
 * @{
 */

//#include "f_com_inc.h"
#include "f_dpda_inc.h"

/************************************************************************************************************/
/*!
 * @brief 関数機能概要:TX部トレーニングデータのリード処理を行う
 * @note  関数処理内容.
 *		-# TX部トレーニングデータのリード処理を行う
 *		-# リードした情報についてCRC16計算を行う
 *		-# CRC16計算結果とEEPROM内のCRC16を判定
 *			- 値が異なる場合
 *				-# 内部テーブルをDefault値で更新する
 * @param  none
 * @return VOID
 * @warning   N/A
 * @FeatureID PF-Rfp-001-001-001
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2011/07/26 ALPHA)横山 Create
 * @date 2011/11/15 ALPHA)横山 M-RRU-ZSYS-00728対処
 * @date 2012/01/10 ALPHA)高橋 M-RRU-ZSYS-00766対処
 * @date 2012/10/05 ALPHA)横山 MTCD対応
 * @date 2013/04/03 ALPHA)中村 B25対応
 * @date 2013/12/10 ALPHA)藤井 DCM-Zynq対応
 */
/************************************************************************************************************/
VOID f_com_dpda_init_trainingDataRead_Tx_sub6()
{
	UINT						uiEepCnt;			/* EEPROMデータ取得回数					*/
	USHORT						usCrc16 = 0;		/* CRC16計算結果						*/
	USHORT						usTraData;
	USHORT						crcPos;
	USHORT						crcSetData;
	
	/****************************************************************************************/
	/* CRC16計算																			*/
	/****************************************************************************************/
	/* EEPROM内のCRC16以外のTX部トレーニングデータについてCRC16計算を行う */
	for( uiEepCnt = D_DU_EEP_TRA_TX_LEN - D_DU_I2C_EEP_TR_OFS;
		 uiEepCnt < (D_DU_EEP_TRA_TX_LEN - D_DU_I2C_EEP_TR_OFS + D_DU_EEP_TRA_TXCNT - 2);
		 uiEepCnt+=2 )
	{
		/* CRC16計算 */
		memcpy( &usTraData, &f_comw_trainingData->traVal[uiEepCnt], sizeof(usTraData) );
		BPF_COM_END_BIGTOLITTLE16(usTraData);
		
		f_com_dpda_init_addCrc( usTraData, &usCrc16);
	}
	
	/* CRC16計算結果とEEPROMにあったCRC16計算結果比較 */
	crcPos = D_DU_EEP_TRA_TX_CRC16 - D_DU_I2C_EEP_TR_OFS;
	memcpy( &usTraData, &f_comw_trainingData->traVal[crcPos], sizeof(usTraData) );
	BPF_COM_END_BIGTOLITTLE16(usTraData);
	
	if( usCrc16 != usTraData )
	{
		/* 異なる場合はテーブル内のトレーニングデータをDefault値で更新 */
		memcpy( &f_comw_trainingData->traVal[D_DU_EEP_TRA_TX_LEN - D_DU_I2C_EEP_TR_OFS], f_comr_trainingTx, sizeof(f_comr_trainingTx));
		
		crcSetData = 0xCDCD;  /* CRC NGの時にCRCエリアに設定する値 */

		/* assert処理 */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "sub6 Tx CRC16 error.(0x%04x)", usCrc16 );
		
		/* アラーム通知 */
		f_com_abort( D_SYS_THDID_PF_CCA, D_DU_ALMCD_TX_TRA_PRM_NG1 );
	}
	else
	{
		crcSetData = 0xABAB;  /* CRC OKの時にCRCエリアに設定する値 */
	}
	memcpy( &f_comw_trainingData->traVal[crcPos], &crcSetData, sizeof(crcSetData) );
	
	return;
}

/* @} */
