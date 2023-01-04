/*!
 * @skip  $ld:$
 * @file  f_com_dpda_init_trainingDataSet_Tx_sub6.c
 * @brief DPDAタスク TX部トレーニングデータ設定処理
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
 * @brief 関数機能概要:TX部トレーニングデータの設定処理を行う
 * @note  関数処理内容.
 *       -# TX部トレーニングデータの設定処理を行う
 * @param  none
 * @return VOID
 * @warning   N/A
 * @FeatureID PF-Rfp-001-001-001
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2018/08/10 KCN)takagi Create
 */
/************************************************************************************************************/
VOID f_com_dpda_init_trainingDataSet_Tx_sub6()
{
	UINT						uiRegData;					/* レジスタ設定値						*/
	USHORT						usTraVal_1;
	USHORT						usTraVal_2;
	USHORT						antCnt;
	USHORT						fbCnt;
	UINT						traPosi;
	int							iRet;
	UINT						uiRegAddr;

	/* 4アンテナ分繰り返し */
	for( antCnt=0; antCnt<D_SYS_ANT_MAX; antCnt++ )
	{
		/************************************************************************************/
		/* RF_GAIN																			*/
		/************************************************************************************/
		traPosi = D_DU_EEP_TRA_TX_RF_GAIN_A - D_DU_I2C_EEP_TR_OFS + (antCnt * D_DU_EEP_TRA_TX_ANT_OFFSET);
		memcpy( &usTraVal_1, &(f_comw_trainingData->traVal[traPosi]), sizeof(usTraVal_1) );
		BPF_COM_END_BIGTOLITTLE16(usTraVal_1);
		
		uiRegData = ( (UINT)usTraVal_1 & D_DU_PD_RFGAIN_RFGAIN );
		BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, M_DU_REG_PD_RFGAIN(antCnt), &uiRegData );
		
		/************************************************************************************/
		/* MC_GAIN																			*/
		/************************************************************************************/
		traPosi = D_DU_EEP_TRA_TX_MCAR_GAIN_A - D_DU_I2C_EEP_TR_OFS + (antCnt * D_DU_EEP_TRA_TX_ANT_OFFSET);
		memcpy( &usTraVal_1, &(f_comw_trainingData->traVal[traPosi]), sizeof(usTraVal_1) );
		BPF_COM_END_BIGTOLITTLE16(usTraVal_1);
		
		uiRegData = ( (UINT)usTraVal_1 & D_DU_PD_MCGAIN_MCGAIN );
		BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, M_DU_REG_PD_MCGAIN(antCnt), &uiRegData );
		
		/************************************************************************************/
		/* DEMGAIN																			*/
		/************************************************************************************/
		traPosi = D_DU_EEP_TRA_TX_DEM_GAIN_Q_A - D_DU_I2C_EEP_TR_OFS + (antCnt * D_DU_EEP_TRA_TX_ANT_OFFSET);
		memcpy( &usTraVal_1, &(f_comw_trainingData->traVal[traPosi]), sizeof(usTraVal_1) );
		BPF_COM_END_BIGTOLITTLE16(usTraVal_1);
		traPosi = D_DU_EEP_TRA_TX_DEM_GAIN_I_A - D_DU_I2C_EEP_TR_OFS + (antCnt * D_DU_EEP_TRA_TX_ANT_OFFSET);
		memcpy( &usTraVal_2, &(f_comw_trainingData->traVal[traPosi]), sizeof(usTraVal_2) );
		BPF_COM_END_BIGTOLITTLE16(usTraVal_2);
		
		uiRegData = ( ( (UINT)usTraVal_2         & D_DU_FB_DEMGAIN_I) |
					  (((UINT)usTraVal_1 << 16 ) & D_DU_FB_DEMGAIN_Q) );
		BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, M_DU_REG_FBANT_DEMGAIN(antCnt), &uiRegData );
		
		/************************************************************************************/
		/* FBDLY																			*/
		/************************************************************************************/
		traPosi = D_DU_EEP_TRA_TX_FB_DLYCLK_A - D_DU_I2C_EEP_TR_OFS + (antCnt * D_DU_EEP_TRA_TX_ANT_OFFSET);
		memcpy( &usTraVal_1, &(f_comw_trainingData->traVal[traPosi]), sizeof(usTraVal_1) );
		BPF_COM_END_BIGTOLITTLE16(usTraVal_1);
		
		traPosi = D_DU_EEP_TRA_TX_FB_DLYFIL_A - D_DU_I2C_EEP_TR_OFS + (antCnt * D_DU_EEP_TRA_TX_ANT_OFFSET);
		memcpy( &usTraVal_2, &(f_comw_trainingData->traVal[traPosi]), sizeof(usTraVal_2) );
		BPF_COM_END_BIGTOLITTLE16(usTraVal_2);
		
		uiRegData = ( (((UINT)usTraVal_1 << 8) & D_DU_FB_FBDLY_CLK) |
					  (((UINT)usTraVal_2 << 1) & D_DU_FB_FBDLY_FIL) );
		BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, M_DU_REG_FBANT_FBDLY(antCnt), &uiRegData );
		

		/************************************************************************************/
		/* PA GateBias DAC設定																*/
		/************************************************************************************/
		/* sub6_PA_AC設定&TXVATT用_DAC設定.xlsx						*/
		/* PA GateBias DAC設定 シート参照							*/
		
		/* PAFA */
		traPosi = D_DU_EEP_TRA_TX_PAFA_A - D_DU_I2C_EEP_TR_OFS + (antCnt * D_DU_EEP_TRA_TX_ANT_OFFSET);
		memcpy( &usTraVal_1, &(f_comw_trainingData->traVal[traPosi]), sizeof(usTraVal_1) );
		BPF_COM_END_BIGTOLITTLE16(usTraVal_1);

		iRet = BPF_HM_DEVC_PAVGDAC_WRITE((unsigned char)antCnt, (UINT)usTraVal_1);
		if(iRet != BPF_HM_DEVC_COMPLETE) {
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "PA GateBias DAC (BPF_HM_DEVC_PAVGDAC)_WRITE NG ant=%d", antCnt );
		}

		/************************************************************************************/
		/* TX VATT調整DAC設定																*/
		/************************************************************************************/
		/* sub6_PA_AC設定&TXVATT用_DAC設定.xlsx						*/
		/* TXVATT用DAC設定 シート参照								*/

		/* TXVATT-DAC */
		traPosi = D_DU_EEP_TRA_TX_FWVATT_DAC_A - D_DU_I2C_EEP_TR_OFS + (antCnt * D_DU_EEP_TRA_TX_ANT_OFFSET);
		memcpy( &usTraVal_1, &(f_comw_trainingData->traVal[traPosi]), sizeof(usTraVal_1) );
		BPF_COM_END_BIGTOLITTLE16(usTraVal_1);

		iRet = BPF_HM_DEVC_TXVATTDAC_WRITE((unsigned char)antCnt, (UINT)usTraVal_1);
		if(iRet != BPF_HM_DEVC_COMPLETE) {
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "TX VATT (BPF_HM_DEVC_TXVATTDAC)_WRITE NG ant=%d", antCnt );
		}
		

		/************************************************************************************/
		/* FBEQ設定																*/
		/************************************************************************************/
		uiRegAddr = 0xB1213000 + ( antCnt * 0x40 );
		traPosi = D_DU_EEP_TRA_TX_FBEQ_A - D_DU_I2C_EEP_TR_OFS + (antCnt * D_DU_EEP_TRA_TX_ANT_OFFSET);

		for(fbCnt=0 ; fbCnt < 15 ; fbCnt++ ){
			uiRegData =  ((UINT)(f_comw_trainingData->traVal[traPosi]  )  << 24 ) & 0xFF000000;
			uiRegData |= ((UINT)(f_comw_trainingData->traVal[traPosi+1])  << 16 ) & 0x00FF0000;
			uiRegData |= ((UINT)(f_comw_trainingData->traVal[traPosi+2])  << 8  ) & 0x0000FF00;
			uiRegData |= ((UINT)(f_comw_trainingData->traVal[traPosi+3]))         & 0x000000FF;

			BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, uiRegAddr + (fbCnt * 4), &uiRegData );

			traPosi += 4;
		}

/* 以下は現時点で不要となったため抑止 */
#if 0
		/************************************************************************************/
		/* VSWR_RTN GAIN設定																*/
		/************************************************************************************/
		traPosi = D_DU_EEP_TRA_TX_VSWR_RTNGAIN_A_Q - D_DU_I2C_EEP_TR_OFS + (antCnt * D_DU_EEP_TRA_TX_ANT_OFFSET);
		memcpy( &usTraVal_1, &(f_comw_trainingData->traVal[traPosi]), sizeof(usTraVal_1) );
		BPF_COM_END_BIGTOLITTLE16(usTraVal_1);
		traPosi = D_DU_EEP_TRA_TX_VSWR_RTNGAIN_A_I - D_DU_I2C_EEP_TR_OFS + (antCnt * D_DU_EEP_TRA_TX_ANT_OFFSET);
		memcpy( &usTraVal_2, &(f_comw_trainingData->traVal[traPosi]), sizeof(usTraVal_2) );
		BPF_COM_END_BIGTOLITTLE16(usTraVal_2);
		uiRegData = ((((UINT)usTraVal_1) << 16) & 0xFFFF0000) | usTraVal_2 ;
		BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, M_DU_REG_FBANT_RTNGAIN(antCnt), &uiRegData );
#endif

	}

	memcpy( &f_comw_trainingData->traVal_vt, &f_comw_trainingData->traVal, D_RRH_RF_TRAING_DATA_SIZE);

	return;
}

/* @} */
