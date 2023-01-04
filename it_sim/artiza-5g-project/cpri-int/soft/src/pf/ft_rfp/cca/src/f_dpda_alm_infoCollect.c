/********************************************************************************************************************/
/*!
 * @skip  $ld:$
 * @file  f_dpda_alm_infoCollect.c
 * @brief アラーム情報収集処理
 * @date  2011/07/19 ALP)藤井 Create
 * ALL Rights Reserved, Copyright (C) FUJITSU Limited 2011-2015
 */
/********************************************************************************************************************/

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */
#include "f_dpda_inc.h"

/********************************************************************************************************************/
/*!
 * @brief 関数機能概要: アラーム情報収集処理
 * @note  関数処理内容.
 *		-# アラーム判定に必要な情報を収集し、判定しやすい型に整形する
 *			-# アラーム閾値情報設定
 *			-# Tx/Rxキャリア情報
 *			-# Tx Gainアラーム関連設定
 *			-# アラーム状態取得
 *			-# TDDEN状態取得
 *			-# Tc取得
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-011-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2011/07/27 ALPHA)藤井 Create
 * @date 2012/03/21 ALPHA)藤井 N21GLTE対応
 * @date 2012/04/02 ALPHA)藤井 M-RRU-ZSYS-00840対処(IT1問処No.2)
 * @date 2012/10/30 ALPHA)藤井 MTCD対応
 * @date 2012/10/31 ALPHA)藤井 M-RRU-ZSYS-00896対処
 * @date 2013/01/10 ALPHA)藤井 M-RRU-ZSYS-00930対処
 * @date 2013/01/15 ALPHA)藤井 M-RRU-ZSYS-00933対処
 * @date 2013/01/28 ALPHA)藤井 M-RRU-ZSYS-00936対処
 * @date 2013/06/03 ALPHA)中村 B25対応
 * @date 2013/12/16 ALPHA)藤井 DCM-Zynq対応
 * @date 2014/02/28 ALPHA)藤井 DCM-SRE対応
 * @date 2015/09/04 ALPHA)藤原 DCM_3.5G TDD-RRE(Zynq)対応
 * @date 2015/11/18 ALPHA)藤原 DCM_3.5G TDD-RRE(Zynq)ハソ変対応(ALMERR v1.6)
 * @date 2018/09/08 KCN)吉木   5G-DU対応
 */
/********************************************************************************************************************/
VOID f_dpda_alm_infoCollect(VOID)
{
	

	UINT				uiAntCnt;					/* アンテナ数分のカウンタ		*/
	USHORT				usAntDirCnt;				/* アンテナ方向のカウンタ		*/
	UINT				usCarCnt;
	USHORT				usAlmInfoIndex;
	USHORT				usCarIndex[ D_SYS_ANT_DIRECTION_NUM ] = {	E_DPDA_ALM_COLLECT_INFO_RX_CAR_A,
																	E_DPDA_ALM_COLLECT_INFO_TX_CAR_A };
	UINT				dlSwRefPow;
	UINT				dlSwFbPow;
	UINT				uiAlmCollectInfo;
	T_DPDA_ANT_INFO		*antInfo;
	T_DPDA_CAR_INFO		*carInfo;

	/****************************************************************************************************************/
	/*	アラーム閾値情報設定																						*/
	/****************************************************************************************************************/
	/* f_dpdar_almInfo で共有メモリを直接参照できないため、ここでコピーする */
	memcpy( (VOID *)f_dpdaw_almThreshold, (VOID *)f_dpdaw_info_all->almThreshold, sizeof(f_dpdaw_almThreshold) );
	
	/****************************************************************************************************************/
	/*	Tx/Rxキャリア情報																							*/
	/****************************************************************************************************************/
	/*	Tx/Rxキャリア状態をアラーム判定用に保存				*/
	for( usAntDirCnt=0; usAntDirCnt<D_SYS_ANT_DIRECTION_NUM; usAntDirCnt++ )
	{
		usAlmInfoIndex = usCarIndex[usAntDirCnt];
		
		for( uiAntCnt=0; uiAntCnt<f_dpdaw_ant_max_num; uiAntCnt++ )
		{
			f_dpdaw_almCollectInfo[usAlmInfoIndex] = D_SYS_OFF;
			
			antInfo = &(f_dpdaw_antCarStateInfo.ant_info[ uiAntCnt ]);
			
			/* キャリア状態取得 */
			for( usCarCnt=0; usCarCnt<f_dpdaw_carrier_max_num; usCarCnt++ )
			{
				carInfo = &(antInfo->car_info[ usAntDirCnt ][ usCarCnt ]);
				
				if( D_DPDA_CAR_ON == carInfo->CarOnOff )
				{
					f_dpdaw_almCollectInfo[usAlmInfoIndex] = D_SYS_ON;
					
				}
			}
			usAlmInfoIndex ++;
		}
	}
	
	/****************************************************************************************************************/
	/*	アンテナ状態情報																							*/
	/****************************************************************************************************************/
	/*	アンテナ状態(0:disable/1:enable)をアラーム判定用に保存				*/
	for( uiAntCnt=0; uiAntCnt<f_dpdaw_ant_max_num; uiAntCnt++ )
	{
		if(f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6){
			uiAlmCollectInfo = (DL)f_dpdaw_regdataReal.regVal[ f_dpdaw_index_AntDisPos + uiAntCnt ];
		}
		else{
			/* mmWはアンテナ2本なのでmmW用の場所を参照する */
			uiAlmCollectInfo = (DL)f_dpdaw_regdataReal.regVal[ f_dpdaw_index_AntDisPos + uiAntCnt ];
		}

		f_dpdaw_almCollectInfo[(E_DPDA_ALM_COLLECT_INFO_ANT_STAT_A+uiAntCnt)] = uiAlmCollectInfo;
	}
	

	/****************************************************************************************************************/
	/*	Tx Gainアラーム関連設定																						*/
	/****************************************************************************************************************/
	/*	送信系利得(Tx Low gain用)																	*/
	/*	1s周期でSW連動Ref信号電力とSW連動FB信号電力取得し、SW連動Ref信号電力/SW連動FB信号電力が		*/
	/*	2倍以上 or 1/2以下が3回連続でアラームとするための情報を生成									*/
	for( uiAntCnt=0; uiAntCnt<f_dpdaw_ant_max_num; uiAntCnt++ )
	{
		dlSwRefPow = (DL)f_dpdaw_regdataReal.regVal[ f_dpdaw_index_RefPowPos + uiAntCnt ];
		dlSwFbPow  = (DL)f_dpdaw_regdataReal.regVal[ f_dpdaw_index_FbPowPos  + uiAntCnt ];

		if((dlSwRefPow == 0) || (dlSwFbPow == 0)){
			uiAlmCollectInfo = 0;
		}
		else{
			if(dlSwFbPow >= dlSwRefPow){
				/* 例:2/1で計算し2以上でALMとなる */
				uiAlmCollectInfo = (UINT)(dlSwFbPow / dlSwRefPow);
			}
			else{
				/* 例:1/2のとき2/1で計算し、2以上(=0.5以下)でALMとなる*/
				uiAlmCollectInfo = (UINT)(dlSwRefPow / dlSwFbPow);
			}
		}
		f_dpdaw_almCollectInfo[(E_DPDA_ALM_COLLECT_INFO_TX_GAIN_ANTA+uiAntCnt)] = uiAlmCollectInfo;
	}
	
	for( uiAntCnt=0; uiAntCnt < f_dpdaw_ant_max_num; uiAntCnt++ )
	{
		f_dpdaw_almCollectInfo[E_DPDA_ALM_COLLECT_INFO_ANT_REF_A + uiAntCnt] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_RefPowPos + uiAntCnt ];
		f_dpdaw_almCollectInfo[E_DPDA_ALM_COLLECT_INFO_ANT_RXPOW_A + uiAntCnt] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_RxPowPos + uiAntCnt ];
	}
	
	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_A000_0200 ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_A000_0200 ];
	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_A000_0210 ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_A000_0210 ];
	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_A000_0B10 ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_A000_0B10 ];
	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_A000_0B20 ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_A000_0B20 ];
	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_A000_0B30 ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_A000_0B30 ];
	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_B000_0B10 ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_B000_0B10 ];
	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_B000_0B20 ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_B000_0B20 ];
	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_B000_0200 ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_B000_0200 ];
	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_B000_0210 ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_B000_0210 ];
	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_B000_02C0 ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_B000_02C0 ];
	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_B100_0040 ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_B100_0040 ];
	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_B100_0044 ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_B100_0044 ];
	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_B100_0048 ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_B100_0048 ];
	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_B100_004C ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_B100_004C ];

	f_dpdaw_almCollectInfo[E_DPDA_ALM_COLLECT_INFO_RFPLL_6E]  = (f_dpdaw_extdataReal.regVal[0] & 0x00000600);	/*	bit[10:9]	*/
	f_dpdaw_almCollectInfo[E_DPDA_ALM_COLLECT_INFO_TXDAC_281] = f_dpdaw_extdataReal.regVal[1];
	f_dpdaw_almCollectInfo[E_DPDA_ALM_COLLECT_INFO_TXDAC_7B5] = f_dpdaw_extdataReal.regVal[2];
	f_dpdaw_almCollectInfo[E_DPDA_ALM_COLLECT_INFO_RXADC_56F] = f_dpdaw_extdataReal.regVal[3];
	f_dpdaw_almCollectInfo[E_DPDA_ALM_COLLECT_INFO_RXADC_11B] = f_dpdaw_extdataReal.regVal[4];

	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_A200_0720 ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_A200_0720 ];
	/*	TOVER ALM	*/
	f_dpdaw_almCollectInfo[E_DPDA_ALM_COLLECT_INFO_A201_43C8] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_A201_43C8 ];

	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_B100_0050 ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_B100_0050 ];
	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_B100_0054 ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_B100_0054 ];
	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_B100_0058 ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_B100_0058 ];
	f_dpdaw_almCollectInfo[ E_DPDA_ALM_COLLECT_INFO_B100_005C ] = f_dpdaw_regdataReal.regVal[ f_dpdaw_index_B100_005C ];

	return;
}

/* @} */
