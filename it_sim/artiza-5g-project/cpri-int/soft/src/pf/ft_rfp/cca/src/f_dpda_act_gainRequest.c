/*!
 * @skip  $ld:$
 * @file  f_dpda_act_gainRequest.c
 * @brief MNT_POWER_ADJUST_REQ
 * @date  2019/3/29 FJT)Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"
#include "BPF_COM_CMD.h"

/*!
 * @brief 関数機能概要:MCGAIN処理
 * @note  関数処理内容:MCGAIN処理
 * @param  uiAntNum			[in]	uiAntNum
 * @param  uiSetCoeffNo		[in]	uiSetCoeffNo
 * @param  temp				[in]	温度
 * @return		N/A
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @retval D_SYS_OK		0:正常終了
 * @date  2019/3/29 FJT)Taniguchi
 */
VOID f_dpda_ld_TxgainSetting(UINT tempIndex ,UINT uiAntNum , UINT uiSetCoeffNo )
{
	DL							dlDefConst;
	DL							dlTempCoef;		/* 温度補正係数						 */
	DL							dlTxgainCoef;	/* gain調整係数						 */
	UINT						uiSetAddr;
	UINT						uiSetVal;
	UINT						uiSetVal_pre;

	if( f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6 ){
		uiSetAddr = M_DU_REG_PD_MCGAIN(uiAntNum);
	}else{
		uiSetAddr = M_DU_REG_CA_MCGAIN_MMW(uiAntNum);
	}
	dlDefConst = (DL)D_DPDA_NOMOD_TX_GAIN_BASEVAL;
	dlTxgainCoef = f_dpdar_TxGainAdjustCoeff[uiSetCoeffNo];
	dlTempCoef = f_dpdar_tempAdjCoef[tempIndex].tempCoef_tx[f_dpdaw_ruIndex];

	uiSetVal = (UINT)( dlDefConst * dlTxgainCoef * dlTempCoef );
	uiSetVal_pre = f_dpda_IO_read( uiSetAddr,E_DPDA_BPF_NOMAL_MODE,D_RRH_LOG_REG_LV_READ );
	if(uiSetVal != uiSetVal_pre){
		f_dpda_IO_write(uiSetAddr,uiSetVal,E_DPDA_BPF_NOMAL_MODE,D_RRH_LOG_REG_LV_WRITE_POL);
		if(uiAntNum == 0){	/* ant-aのみログ */
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_WARNING, "TXGAIN Temp:%d Set:0x%08x -> 0x%08x Def:%f txGainCoef:%f TempCoef:%f",
										f_dpdaw_statusInfo.Temperature,uiSetVal_pre,uiSetVal,dlDefConst,dlTxgainCoef,dlTempCoef);
		}
	}
	return;
}

/*!
 * @brief 関数機能概要:FB gain処理
 * @note  関数処理内容:FB gain処理
 * @param  uiAntNum			[in]	uiAntNum
 * @param  uiSetCoeffNo		[in]	uiSetCoeffNo
 * @param  temp				[in]	温度
 * @return		N/A
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @retval D_SYS_OK		0:正常終了
 * @date  2019/3/29 FJT)Taniguchi
 */
VOID f_dpda_ld_FbgainSetting(UINT tempIndex ,UINT uiAntNum )
{
	UINT						uiDefConst;
	DL							dlDefConst_upper;
	DL							dlDefConst_lower;

	DL							dlTempCoef;		/* 温度補正係数						 */
	UINT						uiSetVal;
	UINT						uiSetVal_pre;


	if( f_dpdaw_reKind != E_DPDA_REKIND_5GDU_SUB6 ){
		return;
	}

	uiDefConst = f_dpdaw_info_all->taskInfo.fbGainCoefNo[uiAntNum];
	dlDefConst_upper = (DL)(( uiDefConst & 0xFFFF0000 ) >> 16);
	dlDefConst_lower = (DL)(uiDefConst & 0xFFFF);
	dlTempCoef = f_dpdar_tempAdjCoef[tempIndex].tempCoef_fb[f_dpdaw_ruIndex];

	uiSetVal = (UINT)(((UINT)( dlDefConst_upper * dlTempCoef ) << 16) & 0xFFFF0000) | (UINT)((UINT)( dlDefConst_lower * dlTempCoef ) & 0xFFFF) ;
	uiSetVal_pre = f_dpda_IO_read( M_DU_REG_FBANT_DEMGAIN(uiAntNum),E_DPDA_BPF_NOMAL_MODE,D_RRH_LOG_REG_LV_READ );

	if(uiSetVal != uiSetVal_pre){
		f_dpda_IO_write(M_DU_REG_FBANT_DEMGAIN(uiAntNum),uiSetVal,E_DPDA_BPF_NOMAL_MODE,D_RRH_LOG_REG_LV_WRITE_POL);
		if(uiAntNum == 0){	/* ant-aのみログ */
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_WARNING, "FBGAIN Temp:%d Set:0x%08x -> 0x%08x DefUpper:%f DefLower :%f TempCoef:%f",
										f_dpdaw_statusInfo.Temperature,uiSetVal_pre,uiSetVal,dlDefConst_upper,dlDefConst_lower,dlTempCoef);
		}
	}
	return;
}

/*!
 * @brief 関数機能概要:RX gain処理
 * @note  関数処理内容:RX gain処理
 * @param  uiAntNum			[in]	uiAntNum
 * @param  uiSetCoeffNo		[in]	uiSetCoeffNo
 * @param  temp				[in]	温度
 * @return		N/A
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @retval D_SYS_OK		0:正常終了
 * @date  2019/3/29 FJT)Taniguchi
 */
VOID f_dpda_ld_RxgainSetting(UINT tempIndex ,UINT uiAntNum )
{
	DL							dlDefConst;
	DL							dlTempCoef;		/* 温度補正係数						 */
	UINT						uiSetVal;
	UINT						uiSetVal_pre;
	UINT						maxCc;
	UINT						count;


	if(f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6){
		maxCc = D_DPDA_CARMAX_SUB6;
	}else{
		maxCc = D_DPDA_CARMAX;
	}
	dlDefConst = (DL)D_DPDA_NOMOD_RX_GAIN_BASEVAL;
	dlTempCoef = f_dpdar_tempAdjCoef[tempIndex].tempCoef_rx[f_dpdaw_ruIndex];
	uiSetVal = (UINT)( dlDefConst * dlTempCoef );
	for(count = 0 ; count < maxCc ; count++){
		uiSetVal_pre = f_dpda_IO_read( M_DU_REG_RX_RXCHFGAIN(count,uiAntNum),E_DPDA_BPF_NOMAL_MODE,D_RRH_LOG_REG_LV_READ );
		if(uiSetVal != uiSetVal_pre){
			f_dpda_IO_write(M_DU_REG_RX_RXCHFGAIN(count,uiAntNum),uiSetVal,E_DPDA_BPF_NOMAL_MODE,D_RRH_LOG_REG_LV_WRITE_POL);
			if((count == 0) && (uiAntNum == 0)){	/* ant-aのみログ */
				BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_WARNING, "RXGAIN Temp:%d Set:0x%08x -> 0x%08x Def:%f TempCoef:%f",
											f_dpdaw_statusInfo.Temperature,uiSetVal_pre,uiSetVal,dlDefConst,dlTempCoef);
			}
		}

		uiSetVal_pre = f_dpda_IO_read( M_DU_REG_OTH_RX_RXCHFGAIN_PRACH(count,uiAntNum),E_DPDA_BPF_NOMAL_MODE,D_RRH_LOG_REG_LV_READ );
		if(uiSetVal != uiSetVal_pre){
			f_dpda_IO_write( M_DU_REG_OTH_RX_RXCHFGAIN_PRACH(count,uiAntNum),uiSetVal,E_DPDA_BPF_NOMAL_MODE,D_RRH_LOG_REG_LV_WRITE_POL );
			if((count == 0) && (uiAntNum == 0)){	/* ant-aのみログ */
				BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_WARNING, "PRACH RXGAIN Temp:%d Set:0x%08x -> 0x%08x Def:%f TempCoef:%f",
											f_dpdaw_statusInfo.Temperature,uiSetVal_pre,uiSetVal,dlDefConst,dlTempCoef);
			}
		}
	}
	return;
}



/*!
 * @brief 関数機能概要:MNT_POWER_ADJUST_REQ処理
 * @note  関数処理内容:MNT_POWER_ADJUST_REQ処理
 * @param  *rcvMsg_p	[in]	受信メッセージ
 * @return INT
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @retval D_SYS_OK		0:正常終了
 * @date  2019/3/29 FJT)Taniguchi
 */
INT f_dpda_act_mnt_txgainRequest(VOID* rcvMsg_p)
{
	T_API_RFP_POW_ADJ_REQ*		msgPtr;			/*	メッセージ変数			*/
	UCHAR						ucSetCoeffNo;
	UCHAR						ucSetCoeffNo_pre;
	UINT						maxAnt;
	UINT						count;

	msgPtr = (T_API_RFP_POW_ADJ_REQ*)rcvMsg_p;
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER, "%s call AntNo:0x%x VAL:0x%x",__func__,msgPtr->powAdjInfo.branch,msgPtr->powAdjInfo.command);

	switch(msgPtr->powAdjInfo.branch){
		case E_API_RFP_NUM_BRANCH_ALL:
			break;
		default:
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR, "Not supported AntNo:0x%x VAL:0x%x",msgPtr->powAdjInfo.branch,msgPtr->powAdjInfo.command);
			return D_SYS_OK;
			break;
	}

	if(f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6){
		maxAnt = D_DPDA_ANTMAX;
	}else{
		maxAnt = D_DPDA_ANTMAX_MMW;
	}
	if(msgPtr->powAdjInfo.command > 10){
		ucSetCoeffNo = 0;
	}else{
		ucSetCoeffNo = (UCHAR)(msgPtr->powAdjInfo.command);
	}

	UINT				tempIndex;
	if( f_dpdaw_statusInfo.Temperature < f_dpdar_tempAdjCoef[D_DPDA_TEMP_ADJ_TEMP_MIN].tempVal){
		tempIndex = D_DPDA_TEMP_ADJ_TEMP_MIN;
	}else if(f_dpdaw_statusInfo.Temperature > f_dpdar_tempAdjCoef[D_DPDA_TEMP_ADJ_TEMP_MAX].tempVal){
		tempIndex = D_DPDA_TEMP_ADJ_TEMP_MAX;
	}else{
		tempIndex = D_DPDA_TEMP_ADJ_COEF_NUM;
		for(count = 0; count < D_DPDA_TEMP_ADJ_COEF_NUM ;count++){
			if(f_dpdaw_statusInfo.Temperature == f_dpdar_tempAdjCoef[count].tempVal){
				tempIndex = count;
				break;
			}
		}
	}

	if(tempIndex < D_DPDA_TEMP_ADJ_COEF_NUM){
		for( count = D_DPDA_ANTA ; count < maxAnt ; count++ ){
			f_dpda_ld_TxgainSetting(tempIndex , count, (UINT)ucSetCoeffNo);
		}
	}else{
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR, "tempindex not found %d",f_dpdaw_statusInfo.Temperature);
	}

	f_dpdaw_info_all->taskInfo.txGainCoefNo = (UINT)ucSetCoeffNo;
	(VOID)BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_TXGAIN, &ucSetCoeffNo_pre);
	if(ucSetCoeffNo_pre != ucSetCoeffNo){
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_TXGAIN, &ucSetCoeffNo);
		f_cmw_tra_inv_tbl->cmx_txgain = ucSetCoeffNo;
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_WARNING, "Save TXGAIN %d -> %d",ucSetCoeffNo_pre,ucSetCoeffNo );

		BPF_COM_CMD_SET_TXGAIN((UINT)ucSetCoeffNo);

	}

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER, "%s call finish(%d)",__func__,ucSetCoeffNo);
	return D_SYS_OK;
}

/* @} */
