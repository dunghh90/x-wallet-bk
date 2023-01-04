/*!
 * @skip  $ld:$
 * @file  f_com_dpda_init_trainingDataRead_Rx.c (base:f_dpda_init_trainingDataRead_Rx.c)
 * @brief RX部トレーニングデータリード処理
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
 * @brief 関数機能概要:RX部トレーニングデータのリード処理を行う
 * @note  関数処理内容.
 *		-# RX部調整データのリード処理を行う
 *		-# リードした情報についてCRC16計算を行う
 *		-# CRC16計算結果とEEPROM内のCRC16を判定
 *			- 値が異なる場合
 *				-# 内部テーブルをDefault値で更新する
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
VOID f_com_dpda_init_trainingDataRead_Rx_sub6()
{
	UINT						uiEepCnt;			/* EEPROMデータ取得回数				*/
	USHORT						usCrc16 = 0;		/* CRC16計算結果					*/
	USHORT						usTraData;
	USHORT						crcPos;
	USHORT						crcSetData;
	
	/* f_com_dpda_init_trainingDataRead_Tx_sub6で             */
	/* 全EEPROMデータをリードしているのでここでは読み込まない */

	/****************************************************************************************/
	/* CRC16計算																			*/
	/****************************************************************************************/
	
	/* RXトレーニングデータのCRC16を除く部分について、CRC16計算を行う */
	for( uiEepCnt = D_DU_EEP_TRA_RX_LEN - D_DU_I2C_EEP_TR_OFS;
		 uiEepCnt < D_DU_EEP_TRA_RX_LEN - D_DU_I2C_EEP_TR_OFS+ D_DU_EEP_TRA_RXCNT - 2;
		 uiEepCnt+=2 )
	{
		/* CRC16計算 */
		memcpy( &usTraData, &f_comw_trainingData->traVal[uiEepCnt], sizeof(usTraData) );
		BPF_COM_END_BIGTOLITTLE16(usTraData);
		
		f_com_dpda_init_addCrc( usTraData, &usCrc16 );
		
	}
	
	/* CRC16計算結果とEEPROMにあったCRC16計算結果比較 */
	crcPos = D_DU_EEP_TRA_RX_CRC16 - D_DU_I2C_EEP_TR_OFS;
	memcpy( &usTraData, &f_comw_trainingData->traVal[crcPos], sizeof(usTraData) );
	BPF_COM_END_BIGTOLITTLE16(usTraData);
	
	if( usCrc16 != usTraData )
	{
		/* 異なる場合はテーブル内のトレーニングデータをDefault値で更新 */
		memcpy( &f_comw_trainingData->traVal[D_DU_EEP_TRA_RX_LEN - D_DU_I2C_EEP_TR_OFS], f_comr_trainingRx, sizeof(f_comr_trainingRx));

		crcSetData = 0xCDCD;  /* CRC NGの時にCRCエリアに設定する値 */

		/* assert処理 */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "sub6 Rx CRC16 error.(0x%04x)", usCrc16 );
		
		/* アラーム通知 */
		f_com_abort( D_SYS_THDID_PF_CCA, D_DU_ALMCD_RX_TRA_PRM_NG1 );
	}
	else
	{
		crcSetData = 0xABAB;  /* CRC OKの時にCRCエリアに設定する値 */
	}
	memcpy( &f_comw_trainingData->traVal[crcPos], &crcSetData, sizeof(crcSetData) );
	
	return;
}

/* @} */
