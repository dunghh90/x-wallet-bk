/*!
 * @skip  $ld:$
 * @file  f_dpda_set_antCal.c
 * @brief DPDAタスク AntCal
 * @date 2019/03/26 FJT)Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"



/*!
 * @brief CAL LOG SAVE
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-004-004
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/03/26 FJT)Taniguchi
 */
VOID f_dpda_cal_log_save(UINT caltype , DL refPower , DL fbPower, UINT calCount ,UINT freqNo , T_RRH_LOG_CAL_HIS_RECORD_ONE* recode_p )
{
	recode_p->validCode = D_RRH_OFF;

	recode_p->calType = caltype;
	recode_p->freqNo = freqNo;
	recode_p->refPower = refPower;
	recode_p->fbPower = fbPower;
	recode_p->calCount = calCount;

	BPF_HM_DEVC_GETTIME(&(recode_p->sysTime));

	recode_p->validCode = D_RRH_ON;
	return;
}

/*!
 * @brief CAL LOG SLOW
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-004-004
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/03/26 FJT)Taniguchi
 */
VOID f_dpda_cal_log_slow(UINT caltype , UINT antno ,UINT dir , DL refPower , DL fbPower , UINT calCount ,UINT freqNo )
{
	T_RRH_LOG_CAL_HIS_INFO*		info;
	T_RRH_LOG_CAL_HIS_RECORD*	recode;
	UINT	index;

	if(dir == 0){
		info = &f_dpdaw_cal_log->info_tx[antno];
		recode = &f_dpdaw_cal_log->tRecord_tx[antno];
	}else{
		info = &f_dpdaw_cal_log->info_rx[antno];
		recode = &f_dpdaw_cal_log->tRecord_rx[antno];
	}

	/* REFが0以外(有効)の場合のみログ保存 */
	if(refPower != 0.0){
		index = info->slowIndex;
		f_dpda_cal_log_save(caltype,refPower,fbPower,calCount,freqNo,&(recode->slow[index]));
		index++;
		if(index >= D_DU_LOG_CAL_HIS_SLOW_NUM){
			index=0;
		}
		info->slowIndex = index;
	}

	info->slowAllCnt++;
	
	return;
}

/*!
 * @brief CAL LOG FAST
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-004-004
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/03/26 FJT)Taniguchi
 */
VOID f_dpda_cal_log_fast(UINT caltype , UINT antno ,UINT dir , DL refPower , DL fbPower, UINT calCount,UINT freqNo )
{
	T_RRH_LOG_CAL_HIS_INFO*		info;
	T_RRH_LOG_CAL_HIS_RECORD*	recode;
	UINT	index;

	if(dir == 0){
		info = &f_dpdaw_cal_log->info_tx[antno];
		recode = &f_dpdaw_cal_log->tRecord_tx[antno];
	}else{
		info = &f_dpdaw_cal_log->info_rx[antno];
		recode = &f_dpdaw_cal_log->tRecord_rx[antno];
	}

	/* REFが0以外(有効)の場合のみログ保存 */
	if(refPower != 0.0){
		index = info->fastIndex;
		f_dpda_cal_log_save(caltype,refPower,fbPower,calCount,freqNo,&(recode->fast[index]));
		index++;
		if(index >= D_DU_LOG_CAL_HIS_FAST_NUM){
			index=0;
		}
		info->fastIndex = index;
	}

	info->fastAllCnt++;

	return;
}


/*!
 * @brief CAL LOG ERR
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-004-004
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/03/26 FJT)Taniguchi
 */
VOID f_dpda_cal_log_err(UINT caltype , UINT antno ,UINT dir , DL refPower , DL fbPower, UINT calCount,UINT freqNo )
{
	T_RRH_LOG_CAL_HIS_INFO*		info;
	T_RRH_LOG_CAL_HIS_RECORD*	recode;
	UINT	index;

	if(dir == 0){
		info = &f_dpdaw_cal_log->info_tx[antno];
		recode = &f_dpdaw_cal_log->tRecord_tx[antno];
	}else{
		info = &f_dpdaw_cal_log->info_rx[antno];
		recode = &f_dpdaw_cal_log->tRecord_rx[antno];
	}

	/* REFが0以外(有効)の場合のみログ保存 */
	if(refPower != 0.0){
		index = info->errIndex;
		f_dpda_cal_log_save(caltype,refPower,fbPower,calCount,freqNo,&(recode->err[index]));
		index++;
		if(index >= D_DU_LOG_CAL_HIS_ERR_NUM){
			index=0;
		}
		info->errIndex = index;
	}

	if(caltype == D_DU_LOG_CAL_TYPE_FAST_ERR){
		info->fastErrAllCnt++;
	}else{
		info->slowErrAllCnt++;
	}

	return;
}



/*!
 * @brief CAL LOG
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-004-004
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/03/26 FJT)Taniguchi
 */
VOID f_dpda_cal_log(UINT caltype , UINT antno ,UINT dir , DL refPower , DL fbPower , UINT calCount ,UINT freqNo )
{
	switch(caltype){
		case D_DU_LOG_CAL_TYPE_SLOW:	/* slow */
			f_dpda_cal_log_slow(caltype,antno,dir,refPower,fbPower,calCount , freqNo);
			f_dpdaw_cal_log->validFlg = D_RRH_ON;
			break;
		case D_DU_LOG_CAL_TYPE_FAST:	/* fast */
			f_dpda_cal_log_fast(caltype,antno,dir,refPower,fbPower,calCount, freqNo);
			f_dpdaw_cal_log->validFlg = D_RRH_ON;
			break;
		case D_DU_LOG_CAL_TYPE_SLOW_ERR:
		case D_DU_LOG_CAL_TYPE_FAST_ERR:
			f_dpda_cal_log_err(caltype,antno,dir,refPower,fbPower,calCount,freqNo);
			f_dpdaw_cal_log->validFlg = D_RRH_ON;
			break;
		default:
			break;
	}
	return;
}

/*!
 * @brief TX ANTCAL
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-004-004
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/03/26 FJT)Taniguchi
 */
DL f_dpda_txcal_antcal( UINT antno , INT* result )
{
	UINT	cnt;

	*result = D_SYS_OK;

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "%s ANT%d start", __func__ ,antno);

	for(cnt=0 ; cnt < D_DPDA_TXANTCAL_PATH_NUM ; cnt++){
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, f_dpdar_txantcal_regpath[cnt].regAddr, (UINT*)&(f_dpdar_txantcal_regpath[cnt].regData_ant[antno]));
	}
	
	f_com_taskDelay(10,0,0,0);

	DL		TXCALPHASE = 0.0;
	UINT	i = 0;
	UINT	COUNT = 0;
	DL		FBISHIFTSUM = 0.0;
	DL		FBQSHIFTSUM = 0.0;
	DL		REFISHIFTSUM = 0.0;
	DL		REFQSHIFTSUM = 0.0;
	UINT	regval;
	UINT	regaddr;
	SHORT	REFI;
	SHORT	REFQ;
	SHORT	FBI;
	SHORT	FBQ;
	DL		AMPREF;
	DL		AMPFB;
	DL		COS;
	DL		SIN;
	DL		FBISHIFT;
	DL		FBQSHIFT;
	DL		REFISHIFT;
	DL		REFQSHIFT;
	DL		SUMI;
	DL		SUMQ;

	regval = 0x8000;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_CTX_CAP, &regval);

	regval = 0x06;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_FBMOD_FB_MONSEL, &regval);

	f_com_taskDelay(10,0,0,0);
	
	for(i=0;i < 4096; i++){
		regaddr = 0xB0E00004 + (0x8 * i);
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, regaddr, &regval);
		REFI = (SHORT)((regval & 0xFFFF0000) >> 16);
		REFQ = (SHORT)(regval & 0xFFFF);

		regaddr = 0xB0E00000 + (0x8 * i);
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, regaddr, &regval);
		FBI = (SHORT)((regval & 0xFFFF0000) >> 16);
		FBQ = (SHORT)(regval & 0xFFFF);
		
		AMPREF = pow((DL)REFI,(DL)2.0) + pow((DL)REFQ,(DL)2.0);
		AMPFB  = pow((DL)FBI,(DL)2.0) + pow((DL)FBQ,(DL)2.0);

		if((AMPREF == 0.0) || (AMPFB == 0.0)){
			continue;
		}

		COS = cos(( i * 2.0 * M_PI * (-750.0)) / 491520.0);
		SIN = sin(( i * 2.0 * M_PI * (-750.0)) / 491520.0);
		FBISHIFT = ((DL)FBI * COS) - ((DL)FBQ * SIN);
		FBQSHIFT = ((DL)FBI * SIN) + ((DL)FBQ * COS);
		FBISHIFTSUM += FBISHIFT;
		FBQSHIFTSUM += FBQSHIFT;
		REFISHIFT = ((DL)REFI * COS) - ((DL)REFQ * SIN);
		REFQSHIFT = ((DL)REFI * SIN) + ((DL)REFQ * COS);
		REFISHIFTSUM += REFISHIFT;
		REFQSHIFTSUM += REFQSHIFT;
		COUNT++;
	}

	regval = 0x0;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_FBMOD_FB_MONSEL, &regval);
	
	if(COUNT == 0){
		TXCALPHASE = 0.0;
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "FAULT! COUNT 0 ANT%d TXCALPHASE:0.0",antno);
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "FAULT! ANT%d TX_CAL_FAIL_INIT(0x%x) ",antno,((D_DU_ALMCD_TX_CAL_FAIL_INIT_A+antno) & 0xFFFF));
//	TODO	f_com_abort(D_SYS_THDID_PF_CCA , D_DU_ALMCD_TX_CAL_FAIL_INIT_A + antno);
//	TODO	*result = D_SYS_NG;


	}else{
		SUMI = ( FBISHIFTSUM * REFISHIFTSUM ) + ( FBQSHIFTSUM * REFQSHIFTSUM );
		SUMQ = ( FBISHIFTSUM * REFQSHIFTSUM ) - ( FBQSHIFTSUM * REFISHIFTSUM );
		TXCALPHASE = atan2(SUMQ,SUMI);

		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO , "ANT%d TXCALPHASE:%f FBISHIFTSUM:%f REFISHIFTSUM:%f FBQSHIFTSUM:%f REFQSHIFTSUM:%f ",antno,TXCALPHASE,
				FBISHIFTSUM,REFISHIFTSUM,FBQSHIFTSUM,REFQSHIFTSUM);
	}
	
	return TXCALPHASE;

}

/*!
 * @brief RX ANTCAL
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-004-004
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/03/26 FJT)Taniguchi
 */
DL f_dpda_rxcal_antcal( UINT antno , INT* result )
{
	UINT	regval;

	*result = D_SYS_OK;

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "%s ANT%d start", __func__ ,antno);
	
	regval = 0x800;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, M_DU_REG_RX_RXTEST(0,antno), &regval);

	regval = 1;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_CTX_CAPEXT, &regval);

	regval = 0x27;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_FBMOD_FB_MONSEL, &regval);
	
	regval = 0x8000;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_CTX_CAP, &regval);

	
	f_com_taskDelay(10,0,0,0);

	DL		RXCALPHASE = 0.0;
	UINT	i = 0;
	UINT	COUNT = 0;
	DL		FBISHIFTSUM = 0.0;
	DL		FBQSHIFTSUM = 0.0;
	DL		REFISHIFTSUM = 0.0;
	DL		REFQSHIFTSUM = 0.0;
	DL		AMPREFSUM = 0.0;
	DL		AMPFBSUM = 0.0;
	UINT	regaddr;
	SHORT	REFI;
	SHORT	REFQ;
	SHORT	FBI;
	SHORT	FBQ;
	DL		AMPREF;
	DL		AMPFB;
	DL		COS;
	DL		SIN;
	DL		FBISHIFT;
	DL		FBQSHIFT;
	DL		REFISHIFT;
	DL		REFQSHIFT;
	DL		SUMI;
	DL		SUMQ;


	for(i=0;i < 4096; i++){
		regaddr = 0xB0E00004 + (0x8 * i);
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, regaddr, &regval);
		REFI = (SHORT)((regval & 0xFFFF0000) >> 16);
		REFQ = (SHORT)(regval & 0xFFFF);

		regaddr = 0xB0E00000 + (0x8 * i);
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, regaddr, &regval);
		FBI = (SHORT)((regval & 0xFFFF0000) >> 16);
		FBQ = (SHORT)(regval & 0xFFFF);
		
		AMPREF = pow((DL)REFI,2) + pow((DL)REFQ,2);
		AMPFB  = pow((DL)FBI,2) + pow((DL)FBQ,2);
		
		if((AMPREF != 0.0) && (AMPFB != 0.0)){
			COS = cos(( i * 2.0 * M_PI * (-750.0)) / 245760.0);
			SIN = sin(( i * 2.0 * M_PI * (-750.0)) / 245760.0);
			FBISHIFT = ((DL)FBI * COS) - ((DL)FBQ * SIN);
			FBQSHIFT = ((DL)FBI * SIN) + ((DL)FBQ * COS);
			FBISHIFTSUM += FBISHIFT;
			FBQSHIFTSUM += FBQSHIFT;
			REFISHIFT = ((DL)REFI * COS) - ((DL)REFQ * SIN);
			REFQSHIFT = ((DL)REFI * SIN) + ((DL)REFQ * COS);
			REFISHIFTSUM += REFISHIFT;
			REFQSHIFTSUM += REFQSHIFT;
			COUNT++;
		}

		AMPREFSUM += AMPREF;
		AMPFBSUM += AMPFB;
	}

	regval = 0;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_FBMOD_FB_MONSEL, &regval);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_CTX_CAPEXT, &regval);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, M_DU_REG_RX_RXTEST(0,antno), &regval);
	
	if(COUNT == 0){
		RXCALPHASE = 0.0;
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "FAULT! COUNT 0 ANT%d RXCALPHASE:0.0",antno);
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "FAULT! ANT%d RX_CAL_FAIL_INIT(0x%x) ",antno,((D_DU_ALMCD_RX_CAL_FAIL_INIT_A+antno) & 0xFFFF));

//	TODO		f_com_abort(D_SYS_THDID_PF_CCA , D_DU_ALMCD_RX_CAL_FAIL_INIT_A + antno);
//	TODO		*result = D_SYS_NG;

	}else{
		if((AMPREFSUM != 0.0 ) && ( AMPFBSUM / AMPREFSUM ) > 0.003 ){

			SUMI = ( FBISHIFTSUM * REFISHIFTSUM ) + ( FBQSHIFTSUM * REFQSHIFTSUM );
			SUMQ = ( FBISHIFTSUM * REFQSHIFTSUM ) - ( FBQSHIFTSUM * REFISHIFTSUM );
			RXCALPHASE = atan2(SUMQ,SUMI);

			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO , "ANT%d RXCALPHASE:%f FBISHIFTSUM:%f REFISHIFTSUM:%f FBQSHIFTSUM:%f REFQSHIFTSUM:%f ",
						antno,RXCALPHASE,FBISHIFTSUM,REFISHIFTSUM,FBQSHIFTSUM,REFQSHIFTSUM);
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO , "      AMPFBSUM:%f AMPREFSUM:%f ",AMPFBSUM,AMPREFSUM);
		}else{
			RXCALPHASE = 0.0;
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "FAULT! ANT%d LNA_ABNORMAL_INIT AMPFBSUM:%f AMPREFSUM:%f ",antno,AMPFBSUM,AMPREFSUM);
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FAULT! ANT%d LNA_ABNORMAL_INIT AMPFBSUM:%f AMPREFSUM:%f ",antno,AMPFBSUM,AMPREFSUM);
			/* LNA ALM */
//	TODO			f_com_abort(D_SYS_THDID_PF_CCA , D_DU_ALMCD_LNA_ABNORMAL_INIT_A + antno);
//	TODO			*result = D_SYS_NG;
		}
	}
	
	
	return RXCALPHASE;
	
}

/*!
 * @brief TXCAL起動時
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-004-004
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/03/26 FJT)Taniguchi
 */
INT f_dpda_txcal_startup( VOID )
{
	UINT	count;
	INT		result;

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "%s start", __func__ );

	for(count=0 ; count < D_DPDA_TXANTCAL_REGINIT_NUM ; count++){
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, f_dpdar_txantcal_reginit[count].regAddr, (UINT*)&(f_dpdar_txantcal_reginit[count].regData));
	}
	
	for(count=0 ; count < D_DPDA_TXANTCAL_SEND_NUM ; count++){
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, f_dpdar_txantcal_regsend[count].regAddr, (UINT*)&(f_dpdar_txantcal_regsend[count].regData_on));
	}	
	
	f_com_taskDelay(500,0,0,0);
	
	f_comw_txcal_coef_tbl->phase[D_DPDA_ANTA] = f_dpda_txcal_antcal(D_DPDA_ANTA,&result);
	if(result == D_SYS_OK){
		f_comw_txcal_coef_tbl->phase[D_DPDA_ANTC] = f_dpda_txcal_antcal(D_DPDA_ANTC,&result);
		if(result == D_SYS_OK){
			f_comw_txcal_coef_tbl->phase[D_DPDA_ANTB] = f_dpda_txcal_antcal(D_DPDA_ANTB,&result);
			if(result == D_SYS_OK){
				f_comw_txcal_coef_tbl->phase[D_DPDA_ANTD] = f_dpda_txcal_antcal(D_DPDA_ANTD,&result);
			}
		}
	}
	/* oRXCAL */
	for(count=0 ; count < D_DPDA_ORXCAL_NUM ; count++){
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, f_dpdar_orxcal_regp[count].regAddr, (UINT*)&(f_dpdar_orxcal_regp[count].regData[0]));
	}
	f_com_taskDelay(500,0,0,0);
	for(count=0 ; count < D_DPDA_ORXCAL_NUM ; count++){
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, f_dpdar_orxcal_regp[count].regAddr, (UINT*)&(f_dpdar_orxcal_regp[count].regData[1]));
	}
	f_com_taskDelay(500,0,0,0);
	for(count=0 ; count < D_DPDA_ORXCAL_NUM ; count++){
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, f_dpdar_orxcal_regp[count].regAddr, (UINT*)&(f_dpdar_orxcal_regp[count].regData[2]));
	}
	
	for(count=0 ; count < D_DPDA_TXANTCAL_SEND_NUM ; count++){
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, f_dpdar_txantcal_regsend[count].regAddr, (UINT*)&(f_dpdar_txantcal_regsend[count].regData_off));
	}

	for(count=0 ; count < D_DPDA_TXANTCAL_END_NUM ; count++){
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, f_dpdar_txantcal_regend[count].regAddr, (UINT*)&(f_dpdar_txantcal_regend[count].regData));
	}

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "%s end",__func__);

	return result;
}


/*!
 * @brief RXCAL起動時
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-004-004
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/03/26 FJT)Taniguchi
 */
INT f_dpda_rxcal_startup( VOID )
{
	UINT	count;
	INT		result;

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "%s start", __func__ );

	for(count=0 ; count < D_DPDA_RXANTCAL_REGINIT_NUM ; count++){
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, f_dpdar_rxantcal_reginit[count].regAddr, (UINT*)&(f_dpdar_rxantcal_reginit[count].regData));
	}
	
	// ANTA
	for(count=0 ; count < D_DPDA_RXANTCAL_PATH_NUM ; count++){
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, f_dpdar_rxantcal_regpath[count].regAddr, (UINT*)&(f_dpdar_rxantcal_regpath[count].regData_ant[D_DPDA_ANTA]));
	}
	
	for(count=0 ; count < D_DPDA_RXANTCAL_SEND_NUM ; count++){
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, f_dpdar_rxantcal_regsend[count].regAddr, (UINT*)&(f_dpdar_rxantcal_regsend[count].regData_on));
	}

	f_com_taskDelay(500,0,0,0);
	

	// ANTA
	f_comw_rxcal_coef_tbl->phase[D_DPDA_ANTA] = f_dpda_rxcal_antcal(D_DPDA_ANTA,&result);
	if(result == D_SYS_OK){
		// ANTC
		for(count=0 ; count < D_DPDA_RXANTCAL_PATH_NUM ; count++){
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, f_dpdar_rxantcal_regpath[count].regAddr, (UINT*)&(f_dpdar_rxantcal_regpath[count].regData_ant[D_DPDA_ANTC]));
		}
		f_comw_rxcal_coef_tbl->phase[D_DPDA_ANTC] = f_dpda_rxcal_antcal(D_DPDA_ANTC,&result);
		if(result == D_SYS_OK){
			// ANTB
			for(count=0 ; count < D_DPDA_RXANTCAL_PATH_NUM ; count++){
				BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, f_dpdar_rxantcal_regpath[count].regAddr, (UINT*)&(f_dpdar_rxantcal_regpath[count].regData_ant[D_DPDA_ANTB]));
			}
			f_comw_rxcal_coef_tbl->phase[D_DPDA_ANTB] = f_dpda_rxcal_antcal(D_DPDA_ANTB,&result);
			if(result == D_SYS_OK){
				// ANTD
				for(count=0 ; count < D_DPDA_RXANTCAL_PATH_NUM ; count++){
					BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, f_dpdar_rxantcal_regpath[count].regAddr, (UINT*)&(f_dpdar_rxantcal_regpath[count].regData_ant[D_DPDA_ANTD]));
				}
				f_comw_rxcal_coef_tbl->phase[D_DPDA_ANTD] = f_dpda_rxcal_antcal(D_DPDA_ANTD,&result);
			}
		}
	}

	// 送信停止
	for(count=0 ; count < D_DPDA_RXANTCAL_SEND_NUM ; count++){
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, f_dpdar_rxantcal_regsend[count].regAddr, (UINT*)&(f_dpdar_rxantcal_regsend[count].regData_off));
	}

	for(count=0 ; count < D_DPDA_RXANTCAL_END_NUM ; count++){
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, f_dpdar_rxantcal_regend[count].regAddr, (UINT*)&(f_dpdar_rxantcal_regend[count].regData));
	}

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "%s end",__func__);
	return result;
}

/*!
 * @brief TXCAL運用中PATH戻し(ANT共通)
 * @return VOID
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/07/19 FJT)Hasegawa
 */
VOID f_dpda_txcal_ope_pathret( VOID )
{   
    UINT count;

	for(count=0 ; count < D_DPDA_TXANTCAL_OPE_PATH_RET_NUM ; count++){
		BPF_HM_DEVC_REG_WRITE(0, f_dpdar_txantcal_ope_regpath_ret[count].regAddr, (UINT*)&(f_dpdar_txantcal_ope_regpath_ret[count].regData));
	}
} 

/*!
 * @brief RXCAL運用中PATH戻し(ANT共通)
 * @return VOID
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/07/19 FJT)Hasegawa
 */
VOID f_dpda_rxcal_ope_pathret( VOID )
{   
    UINT count;

	for(count=0 ; count < D_DPDA_RXANTCAL_OPE_PATH_RET_NUM ; count++){
		BPF_HM_DEVC_REG_WRITE(0, f_dpdar_rxantcal_ope_regpath_ret[count].regAddr, (UINT*)&(f_dpdar_rxantcal_ope_regpath_ret[count].regData));
	}
} 


/*!
 * @brief CAL LOG Get
 * @return VOID
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/07/25 FJT)Taniguchi
 */
VOID f_dpda_get_callog( UINT antno , UINT txrx )
{
	CHAR callog_cmd_str[64];
	CHAR* callog_ant_srt[D_DPDA_ANTMAX] = { "A","B","C","D"};
	CHAR* callog_txrx_srt[2] = { "TX","RX" };
	int	ret;

	/* ひたすら書く */
	memset(callog_cmd_str,0,sizeof(callog_cmd_str));
	snprintf(callog_cmd_str,sizeof(callog_cmd_str),"cmd_calcapdump %s %s >> /var/log/CALlog.log",callog_ant_srt[antno],callog_txrx_srt[txrx & 1]);

	ret = system(callog_cmd_str);
	BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "GET CAL LOG %s %d",callog_cmd_str,ret);	/* pgr0539 */

	return;
}

/*!
 * @brief TXCAL運用中係数計算(ANT.X)
 * @return VOID
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/07/19 FJT)Hasegawa
 */
T_DPDA_OPECAL_PHASE_VALUE_TBL f_dpda_txcal_ope_antcal( UINT antno, UINT freq, INT* result )
{    

    UINT	count;
    T_DPDA_OPECAL_PHASE_VALUE_TBL  cal_val={0.0, 0.0,0.0, 0.0};

	*result = D_SYS_OK;
    f_dpdaw_info_all->taskInfo.flg.cal_error_flg = 0;

    UINT    read_bit;
	UINT	i = 0;
	UINT	COUNT = 0;
	DL		FBISHIFTSUM = 0.0;
	DL		FBQSHIFTSUM = 0.0;
	DL		REFISHIFTSUM = 0.0;
	DL		REFQSHIFTSUM = 0.0;
	UINT	regval;
	UINT	regaddr;
	SHORT	REFI;
	SHORT	REFQ;
	SHORT	FBI;
	SHORT	FBQ;
	DL		AMPREF;
	DL		AMPFB;
	DL		AMPREFSUM;
	DL		AMPFBSUM;
	DL		COS;
	DL		SIN;
	DL		FBISHIFT;
	DL		FBQSHIFT;
	DL		REFISHIFT;
	DL		REFQSHIFT;
	DL		SUMI;
	DL		SUMQ;
	DL		DELTA_f = 0.0;

    switch (freq) {
        case D_DPDA_FREQ_F1:
            DELTA_f = -9495.0;
            break;
        case D_DPDA_FREQ_F2:
            DELTA_f = -34500.0;
            break;
        case D_DPDA_FREQ_F3:
            DELTA_f = -65220.0;
            break;
        case D_DPDA_FREQ_F4:
            DELTA_f = -90225.0;
            break;
        case D_DPDA_FREQ_FAST:
            DELTA_f = -49860.0;
            break;
        default:
            break;
    }

	for(count=0 ; count < D_DPDA_TXANTCAL_OPE_PATH_NUM ; count++){
		BPF_HM_DEVC_REG_WRITE(0, f_dpdar_txantcal_ope_regpath[count].regAddr, (UINT*)&(f_dpdar_txantcal_ope_regpath[count].regData_ant[antno]));
	}
	f_com_taskDelay(1,0,0,0);

	regval = 0x20002000;
	BPF_HM_DEVC_REG_WRITE(0, 0xB1202948, &regval);
	regval = 0x0;
	BPF_HM_DEVC_REG_WRITE(0, 0xB1202960, &regval);
	regval = 0x0;
	BPF_HM_DEVC_REG_WRITE(0, 0xB0000FD4, &regval);
	regval = 0xB;
	BPF_HM_DEVC_REG_WRITE(0, 0xB12029FC, &regval);
	regval = 0x8000;
	BPF_HM_DEVC_REG_WRITE(0, 0xB0000FD0, &regval);

	f_com_taskDelay(50,0,0,0);
 
    BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, 0xB0000FD0, 0x4000, &read_bit);

    if(read_bit == 0x4000) {
        f_com_taskDelay(1,0,0,0);
        BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, 0xB0000FD0, 0x4000, &read_bit);
        if(read_bit == 0x4000) {
            f_dpdaw_info_all->taskInfo.flg.cal_error_flg = D_RRH_ON; 

            regval = 0x0;
        	BPF_HM_DEVC_REG_WRITE(0, 0xB12029FC, &regval);
        	BPF_HM_DEVC_REG_WRITE(0, 0xB0000FD0, &regval);
            f_dpda_txcal_ope_pathret();

			f_com_taskDelay(1,0,0,0);
           	
			BPF_HM_DEVC_REG_WRITE(0, 0xB0000FD4, &regval);

            BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, 0xB1202410, 0xF, &read_bit);

            regval = (0x100 | read_bit);
	        BPF_HM_DEVC_REG_WRITE(0, 0xB120214C, &regval);

			f_com_taskDelay(100,0,0,0);

  			regval = 0x0;
    		BPF_HM_DEVC_REG_WRITE(0, 0xB120214C, &regval);

            BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Val:%x Cal NG",__func__, antno, read_bit);
            return cal_val;
        }
    }

	AMPREFSUM = 0.0;
	AMPFBSUM = 0.0;
	for(i=0;i < 924; i++){
		regaddr = 0xB0E00004 + (0x8 * i);
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, regaddr, &regval);
		REFI = (SHORT)((regval & 0xFFFF0000) >> 16);
		REFQ = (SHORT)(regval & 0xFFFF);

		regaddr = 0xB0E00000 + (0x8 * i);
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, regaddr, &regval);
		FBI = (SHORT)((regval & 0xFFFF0000) >> 16);
		FBQ = (SHORT)(regval & 0xFFFF);
		
		AMPREF = pow((DL)REFI,(DL)2.0) + pow((DL)REFQ,(DL)2.0);
		AMPFB  = pow((DL)FBI,(DL)2.0) + pow((DL)FBQ,(DL)2.0);
    	AMPREFSUM += AMPREF;
	    AMPFBSUM  += AMPFB;
		if((AMPREF == 0.0) || (AMPFB == 0.0)){
			continue;
		}

		COS = cos(( i * 2.0 * M_PI * (DELTA_f)) / 491520.0);
		SIN = sin(( i * 2.0 * M_PI * (DELTA_f)) / 491520.0);
		FBISHIFT = ((DL)FBI * COS) - ((DL)FBQ * SIN);
		FBQSHIFT = ((DL)FBI * SIN) + ((DL)FBQ * COS);
		FBISHIFTSUM += FBISHIFT;
		FBQSHIFTSUM += FBQSHIFT;
		REFISHIFT = ((DL)REFI * COS) - ((DL)REFQ * SIN);
		REFQSHIFT = ((DL)REFI * SIN) + ((DL)REFQ * COS);
		REFISHIFTSUM += REFISHIFT;
		REFQSHIFTSUM += REFQSHIFT;
		COUNT++;
	}

	regval = 0x0;
	BPF_HM_DEVC_REG_WRITE(0, D_DU_REG_FBMOD_FB_MONSEL, &regval);

    f_dpda_txcal_ope_pathret();

    f_com_taskDelay(1,0,0,0);

    BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, 0xB1202410, 0xF, &read_bit);

    regval = (0x100 | read_bit);
	BPF_HM_DEVC_REG_WRITE(0, 0xB120214C, &regval);

	f_com_taskDelay(100,0,0,0);

  	regval = 0x0;
    BPF_HM_DEVC_REG_WRITE(0, 0xB120214C, &regval);

	if(COUNT == 0) {
		BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_ERROR , "COUNT 0 ANT%d TXCALPHASE:0.0",antno);
        f_dpdaw_antCarStateInfo.ant_info[antno].txopecal_info.calFailCount = 0;
        return cal_val;
	}
    else if(AMPREFSUM != 0 && AMPFBSUM / AMPREFSUM>0.003 ){
		SUMI = ( FBISHIFTSUM * REFISHIFTSUM ) + ( FBQSHIFTSUM * REFQSHIFTSUM );
		SUMQ = ( FBISHIFTSUM * REFQSHIFTSUM ) - ( FBQSHIFTSUM * REFISHIFTSUM );
        cal_val.CALCOS = SUMI / sqrt(SUMI*SUMI + SUMQ*SUMQ);
        cal_val.CALSIN = SUMQ / sqrt(SUMI*SUMI + SUMQ*SUMQ);
        cal_val.AMPREFSUM = AMPREFSUM;
        cal_val.AMPFBSUM = AMPFBSUM;

        f_dpdaw_antCarStateInfo.ant_info[antno].txopecal_info.cal_ant_flg[freq] = D_RRH_ON;
		BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_INFO , "ANT%d FBISHIFTSUM:%f REFISHIFTSUM:%f FBQSHIFTSUM:%f REFQSHIFTSUM:%f CALCOS:%f CALSIN:%f",antno,
				FBISHIFTSUM,REFISHIFTSUM,FBQSHIFTSUM,REFQSHIFTSUM, cal_val.CALCOS, cal_val.CALSIN);
		BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_INFO , "ANT%d FREQ:%d AMPFBSUM:%f AMPREFSUM:%f",antno,freq,AMPFBSUM,AMPREFSUM);
        f_dpdaw_antCarStateInfo.ant_info[antno].txopecal_info.calFailCount = 0;
	}
    else {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Tx Cal failed antno = %d", antno);
		BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_INFO , "ANT%d FBISHIFTSUM:%f REFISHIFTSUM:%f FBQSHIFTSUM:%f REFQSHIFTSUM:%f",antno ,
				FBISHIFTSUM,REFISHIFTSUM,FBQSHIFTSUM,REFQSHIFTSUM);
		BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_INFO , "ANT%d FREQ:%d AMPFBSUM:%f AMPREFSUM:%f",antno,freq,AMPFBSUM,AMPREFSUM);
        f_dpdaw_antCarStateInfo.ant_info[antno].txopecal_info.calFailCount++;
        if(f_dpdaw_antCarStateInfo.ant_info[antno].txopecal_info.calFailCount >= 3) {
            f_dpdaw_info_all->taskInfo.flg.cal_error_flg = D_RRH_ON;
			if( D_SYS_OFF == f_dpdaw_info_all->almmsk ){
				if(f_dpdaw_antCarStateInfo.ant_info[antno].txopecal_info.calFailCount == 3){
					f_com_SVCtl(E_DU_SVCTL_OPE_FID10_ALM_A + antno ,D_DU_SVCTL_ON,D_DU_FLTID_TX_CAL_FAIL_ANT_A + antno);
					f_dpda_get_callog(antno,0);
				}
			}
            f_dpdaw_antCarStateInfo.ant_info[antno].txopecal_info.calFailCount = 3;	/* 3でラッチ */
        }
        *result = D_SYS_NG;
        cal_val.AMPREFSUM = AMPREFSUM;
        cal_val.AMPFBSUM = AMPFBSUM;
        return cal_val;
    }
    return cal_val;
}


/*!
 * @brief RXCAL運用中係数計算(ANT.X)
 * @return VOID
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/07/19 FJT)Hasegawa
 */
T_DPDA_OPECAL_PHASE_VALUE_TBL f_dpda_rxcal_ope_antcal( UINT antno, UINT freq, INT* result )
{    

    UINT	count;
    T_DPDA_OPECAL_PHASE_VALUE_TBL  cal_val={0.0, 0.0,0.0, 0.0};

	*result = D_SYS_OK;
    f_dpdaw_info_all->taskInfo.flg.cal_error_flg = 0;

	UINT	regtable[D_RRH_ANT_MAX] = {
		0xB13003F8, 0xB13007F8, 0xB1300BF8, 0xB1300FF8
	};

    UINT    read_bit;
	UINT	i = 0;
	UINT	COUNT = 0;
	DL		FBISHIFTSUM = 0.0;
	DL		FBQSHIFTSUM = 0.0;
	DL		REFISHIFTSUM = 0.0;
	DL		REFQSHIFTSUM = 0.0;
	UINT	regval;
	UINT	regaddr;
	SHORT	REFI;
	SHORT	REFQ;
	SHORT	FBI;
	SHORT	FBQ;
	DL		AMPREF;
	DL		AMPFB;
	DL		AMPREFSUM;
	DL		AMPFBSUM;
	DL		COS;
	DL		SIN;
	DL		FBISHIFT;
	DL		FBQSHIFT;
	DL		REFISHIFT;
	DL		REFQSHIFT;
	DL		SUMI;
	DL		SUMQ;
	DL		DELTA_f = 0.0;

    switch (freq) {
        case D_DPDA_FREQ_F1:
            DELTA_f = -9495.0;
            break;
        case D_DPDA_FREQ_F2:
            DELTA_f = -34500.0;
            break;
        case D_DPDA_FREQ_F3:
            DELTA_f = -65220.0;
            break;
        case D_DPDA_FREQ_F4:
            DELTA_f = -90225.0;
            break;
        case D_DPDA_FREQ_FAST:
            DELTA_f = -49860.0;
            break;
        default:
            break;
    }

	for(count=0 ; count < D_DPDA_RXANTCAL_OPE_PATH_NUM ; count++){
		BPF_HM_DEVC_REG_WRITE(0, f_dpdar_rxantcal_ope_regpath[count].regAddr, (UINT*)&(f_dpdar_rxantcal_ope_regpath[count].regData_ant[antno]));
	}
	f_com_taskDelay(1,0,0,0);

	regval = 0x20002000;
	BPF_HM_DEVC_REG_WRITE(0, 0xB1202948, &regval);
	regval = 0x0;
	BPF_HM_DEVC_REG_WRITE(0, 0xB1202960, &regval);
	regval = 0x800;
	BPF_HM_DEVC_REG_WRITE(0, regtable[antno], &regval);
	regval = 0x0;
	BPF_HM_DEVC_REG_WRITE(0, 0xB0000FD4, &regval);
	regval = 0x2C;
	BPF_HM_DEVC_REG_WRITE(0, 0xB12029FC, &regval);
	regval = 0x8000;
	BPF_HM_DEVC_REG_WRITE(0, 0xB0000FD0, &regval);

	f_com_taskDelay(50,0,0,0);
 
    BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, 0xB0000FD0, 0x4000, &read_bit);

    if(read_bit == 0x4000) {
        f_com_taskDelay(1,0,0,0);
        BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, 0xB0000FD0, 0x4000, &read_bit);
        if(read_bit == 0x4000) {
            f_dpdaw_info_all->taskInfo.flg.cal_error_flg = D_RRH_ON; 

            regval = 0x0;
        	BPF_HM_DEVC_REG_WRITE(0, 0xB12029FC, &regval);
        	BPF_HM_DEVC_REG_WRITE(0, 0xB0000FD0, &regval);
            f_dpda_rxcal_ope_pathret();
            switch(antno) {
                case D_RRH_ANT_A:
                    BPF_HM_DEVC_REG_WRITE(0, 0xB13003F8, &regval);
                    break;
                case D_RRH_ANT_B:
                    BPF_HM_DEVC_REG_WRITE(0, 0xB13007F8, &regval);
                    break;
                case D_RRH_ANT_C:
                    BPF_HM_DEVC_REG_WRITE(0, 0xB1300BF8, &regval);
                    break;
                case D_RRH_ANT_D:
                    BPF_HM_DEVC_REG_WRITE(0, 0xB1300FF8, &regval);
                    break;
                default:
                    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "antno range over antno = %d", antno);
                    break;
            }
            regval = 0x0;
           	BPF_HM_DEVC_REG_WRITE(0, 0xB0000FD4, &regval);

			f_com_taskDelay(1,0,0,0);
        	
            BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, 0xB1202410, 0xF, &read_bit);

            regval = (0x100 | read_bit);
	        BPF_HM_DEVC_REG_WRITE(0, 0xB120214C, &regval);

			f_com_taskDelay(100,0,0,0);

  			regval = 0x0;
    		BPF_HM_DEVC_REG_WRITE(0, 0xB120214C, &regval);
            
            BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Val:%x Cal NG",__func__, antno, read_bit);
            return cal_val;
        }
    }

	AMPREFSUM = 0.0;
	AMPFBSUM = 0.0;
	for(i=0;i < 412; i++){
		regaddr = 0xB0E00004 + (0x8 * i);
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, regaddr, &regval);
		REFI = (SHORT)((regval & 0xFFFF0000) >> 16);
		REFQ = (SHORT)(regval & 0xFFFF);

		regaddr = 0xB0E00000 + (0x8 * i);
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, regaddr, &regval);
		FBI = (SHORT)((regval & 0xFFFF0000) >> 16);
		FBQ = (SHORT)(regval & 0xFFFF);
		
		AMPREF = pow((DL)REFI,(DL)2.0) + pow((DL)REFQ,(DL)2.0);
		AMPFB  = pow((DL)FBI,(DL)2.0) + pow((DL)FBQ,(DL)2.0);
    	AMPREFSUM += AMPREF;
	    AMPFBSUM  += AMPFB;
		if((AMPREF == 0.0) || (AMPFB == 0.0)){
			continue;
		}

		COS = cos(( i * 2.0 * M_PI * (DELTA_f)) / 245760.0);
		SIN = sin(( i * 2.0 * M_PI * (DELTA_f)) / 245760.0);
		FBISHIFT = ((DL)FBI * COS) - ((DL)FBQ * SIN);
		FBQSHIFT = ((DL)FBI * SIN) + ((DL)FBQ * COS);
		FBISHIFTSUM += FBISHIFT;
		FBQSHIFTSUM += FBQSHIFT;
		REFISHIFT = ((DL)REFI * COS) - ((DL)REFQ * SIN);
		REFQSHIFT = ((DL)REFI * SIN) + ((DL)REFQ * COS);
		REFISHIFTSUM += REFISHIFT;
		REFQSHIFTSUM += REFQSHIFT;
		COUNT++;
	}

    regval = 0x0;
	BPF_HM_DEVC_REG_WRITE(0, D_DU_REG_FBMOD_FB_MONSEL, &regval);

    regval = 0x0;
    switch(antno) {
        case D_RRH_ANT_A:
            BPF_HM_DEVC_REG_WRITE(0, 0xB13003F8, &regval);
            break;
        case D_RRH_ANT_B:
            BPF_HM_DEVC_REG_WRITE(0, 0xB13007F8, &regval);
            break;
        case D_RRH_ANT_C:
            BPF_HM_DEVC_REG_WRITE(0, 0xB1300BF8, &regval);
            break;
        case D_RRH_ANT_D:
            BPF_HM_DEVC_REG_WRITE(0, 0xB1300FF8, &regval);
            break;
        default:
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "antno range over antno = %d", antno);
            break;
    }
    BPF_HM_DEVC_REG_WRITE(0, 0xB0000FD4, &regval);

    f_dpda_rxcal_ope_pathret();

	f_com_taskDelay(1,0,0,0);

    BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, 0xB1202410, 0xF, &read_bit);

    regval = (0x100 | read_bit);
	BPF_HM_DEVC_REG_WRITE(0, 0xB120214C, &regval);

	f_com_taskDelay(100,0,0,0);

  	regval = 0x0;
    BPF_HM_DEVC_REG_WRITE(0, 0xB120214C, &regval);

	if(COUNT == 0) {
		BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_ERROR , "COUNT 0 ANT%d RXCALPHASE:0.0",antno);
        f_dpdaw_antCarStateInfo.ant_info[antno].rxopecal_info.calFailCount = 0;
        return cal_val;
	}
    else if(AMPREFSUM != 0 && AMPFBSUM / AMPREFSUM>0.003 ){
		SUMI = ( FBISHIFTSUM * REFISHIFTSUM ) + ( FBQSHIFTSUM * REFQSHIFTSUM );
		SUMQ = ( FBISHIFTSUM * REFQSHIFTSUM ) - ( FBQSHIFTSUM * REFISHIFTSUM );
        cal_val.CALCOS = SUMI / sqrt(SUMI*SUMI + SUMQ*SUMQ);
        cal_val.CALSIN = SUMQ / sqrt(SUMI*SUMI + SUMQ*SUMQ);
        cal_val.AMPREFSUM = AMPREFSUM;
        cal_val.AMPFBSUM = AMPFBSUM;

        f_dpdaw_antCarStateInfo.ant_info[antno].rxopecal_info.cal_ant_flg[freq] = D_RRH_ON;
        BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_INFO , "ANT%d FBISHIFTSUM:%f REFISHIFTSUM:%f FBQSHIFTSUM:%f REFQSHIFTSUM:%f CALCOS:%f CALSIN:%f",antno,
				FBISHIFTSUM,REFISHIFTSUM,FBQSHIFTSUM,REFQSHIFTSUM, cal_val.CALCOS, cal_val.CALSIN);
		BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_INFO , "ANT%d FREQ:%d AMPFBSUM:%f AMPREFSUM:%f",antno,freq,AMPFBSUM,AMPREFSUM);
        f_dpdaw_antCarStateInfo.ant_info[antno].rxopecal_info.calFailCount = 0;
	}
    else {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Rx Cal failed antno = %d", antno);
		BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_INFO , "ANT%d FBISHIFTSUM:%f REFISHIFTSUM:%f FBQSHIFTSUM:%f REFQSHIFTSUM:%f",antno,
				FBISHIFTSUM,REFISHIFTSUM,FBQSHIFTSUM,REFQSHIFTSUM);
		BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_INFO , "ANT%d FREQ:%d AMPFBSUM:%f AMPREFSUM:%f",antno,freq,AMPFBSUM,AMPREFSUM);
        f_dpdaw_antCarStateInfo.ant_info[antno].rxopecal_info.calFailCount++;
        if(f_dpdaw_antCarStateInfo.ant_info[antno].rxopecal_info.calFailCount >= 3) {
			f_dpdaw_info_all->taskInfo.flg.cal_error_flg = D_RRH_ON;
			if( D_SYS_OFF == f_dpdaw_info_all->almmsk ){
				if(f_dpdaw_antCarStateInfo.ant_info[antno].rxopecal_info.calFailCount == 3){
					f_com_SVCtl(E_DU_SVCTL_OPE_FID11_ALM_A + antno ,D_DU_SVCTL_ON,D_DU_FLTID_RX_CAL_FAIL_ANT_A + antno);
					f_dpda_get_callog(antno,1);
				}
		 	}
			f_dpdaw_antCarStateInfo.ant_info[antno].rxopecal_info.calFailCount = 3;	/* 3でラッチ */
        }
        *result = D_SYS_NG;
        cal_val.AMPREFSUM = AMPREFSUM;
        cal_val.AMPFBSUM = AMPFBSUM;

        return cal_val;
    }
    return cal_val;
}

/*!
 * @brief TXCAL運用中(TXCAL(ANT.X))
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/07/19 FJT)Hasegawa
 */
INT f_dpda_txcal_ope_txcal( UINT antno)
{
    UINT count, freq, count2;
    INT result;
    struct timespec start_time, end_time;
	UINT wait_time;
    T_DPDA_OPECAL_PHASE_VALUE_TBL  cal_val={0.0, 0.0,0.0, 0.0}, cal_val_tmp;

	BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_ENTER , "%s ANT%d start", __func__ ,antno);

    for(freq=0; freq<D_DPDA_CAL_FREQ_NUM; freq++) {
        (void)clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);

        f_dpdaw_antCarStateInfo.ant_info[antno].txopecal_info.cal_ant_flg[freq] = D_RRH_OFF;
        /* TXCAL信号送信設定 */
	    for(count=0 ; count < D_DPDA_TXANTCAL_OPE_SEND_NUM ; count++){
            if(antno == D_RRH_ANT_A || antno == D_RRH_ANT_C ) {
    		    BPF_HM_DEVC_REG_WRITE(0, f_dpdar_txantcal_ope_regsend[count].regAddr[D_DPDA_ANT_AC], (UINT*)&(f_dpdar_txantcal_ope_regsend[count].regData[freq]));
            }
            else {
       		    BPF_HM_DEVC_REG_WRITE(0, f_dpdar_txantcal_ope_regsend[count].regAddr[D_DPDA_ANT_BD], (UINT*)&(f_dpdar_txantcal_ope_regsend[count].regData[freq]));
            }
	    }
        f_com_taskDelay(10,0,0,0);
        /* TXCAL係数計算(ANT.X) */
        cal_val.CALCOS = 0.0;
        cal_val.CALSIN = 0.0;
        cal_val.AMPREFSUM = 0.0;
        cal_val.AMPFBSUM = 0.0;
        
        for(count=0; count<10; count++) {
            cal_val_tmp = f_dpda_txcal_ope_antcal( antno, freq, &result);
            if(result != D_SYS_OK) {
                /* TXCAL信号送信終了 */
	            for(count2=0 ; count2 < D_DPDA_TXANTCAL_OPE_END_NUM ; count2++){
                    BPF_HM_DEVC_REG_WRITE(0, f_dpdar_txantcal_ope_regend[count2].regAddr, (UINT*)&(f_dpdar_txantcal_ope_regend[count2].regData));
  	            }
                BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Tx Ope Cal NG",__func__, antno);

	            cal_val.AMPREFSUM += cal_val_tmp.AMPREFSUM;
	            cal_val.AMPFBSUM += cal_val_tmp.AMPFBSUM;

				f_dpda_cal_log(D_DU_LOG_CAL_TYPE_SLOW_ERR,antno,0,cal_val.AMPREFSUM,cal_val.AMPFBSUM,count+1,freq);

                return D_SYS_NG;
            }
            cal_val.CALCOS += cal_val_tmp.CALCOS;
            cal_val.CALSIN += cal_val_tmp.CALSIN;
            cal_val.AMPREFSUM += cal_val_tmp.AMPREFSUM;
            cal_val.AMPFBSUM += cal_val_tmp.AMPFBSUM;
        }

		/* 先頭の周波数のみ */
		if(freq == 0){
			f_dpda_cal_log(D_DU_LOG_CAL_TYPE_SLOW,antno,0,cal_val.AMPREFSUM,cal_val.AMPFBSUM,count,freq);
		}

        /* 異常flgが全部=1 (SIN=0かつCOS=0)でない場合、計算値を使う */
        /* それ以外の場合は現在の設定値(初期値0)を使う              */
        if(cal_val.CALSIN != 0.0 || cal_val.CALCOS != 0.0) {
            f_dpdaw_antCarStateInfo.ant_info[antno].txopecal_info.phase[freq] = atan2(cal_val.CALSIN, cal_val.CALCOS);
            BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_INFO , "ANT%d FREQ:%d PHASE:%f REF:%f FB:%f",antno, freq, 
                f_dpdaw_antCarStateInfo.ant_info[antno].txopecal_info.phase[freq], cal_val.AMPREFSUM, cal_val.AMPFBSUM);
        }

        /* TXCAL信号送信終了 */
	    for(count=0 ; count < D_DPDA_TXANTCAL_OPE_END_NUM ; count++){
            BPF_HM_DEVC_REG_WRITE(0, f_dpdar_txantcal_ope_regend[count].regAddr, (UINT*)&(f_dpdar_txantcal_ope_regend[count].regData));
  	    }

        (void)clock_gettime(CLOCK_MONOTONIC_RAW, &end_time);

        wait_time = end_time.tv_nsec + 1000000000*(end_time.tv_sec-start_time.tv_sec) - start_time.tv_nsec;
        wait_time /= 1000000;
        if(wait_time > 1000) {
            wait_time = 1000;
        }
		f_com_taskDelay(1000 - wait_time,0,0,0);
    }

	BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_ENTER , "%s end",__func__);
    return D_SYS_OK;
}

/*!
 * @brief RXCAL運用中(RXCAL(ANT.X))
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/07/19 FJT)Hasegawa
 */
INT f_dpda_rxcal_ope_rxcal( UINT antno)
{
    UINT count, freq, count2;
    INT result;
    struct timespec start_time, end_time;
	UINT wait_time;
    T_DPDA_OPECAL_PHASE_VALUE_TBL  cal_val={0.0, 0.0,0.0, 0.0}, cal_val_tmp;

	BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_ENTER , "%s ANT%d start", __func__ ,antno);

    for(freq=0; freq<D_DPDA_CAL_FREQ_NUM; freq++) {
        (void)clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);

        f_dpdaw_antCarStateInfo.ant_info[antno].rxopecal_info.cal_ant_flg[freq] = D_RRH_OFF;
        /* RXCAL信号送信設定 */
	    for(count=0 ; count < D_DPDA_RXANTCAL_OPE_SEND_NUM ; count++){
            if(antno == D_RRH_ANT_A || antno == D_RRH_ANT_C ) {
    		    BPF_HM_DEVC_REG_WRITE(0, f_dpdar_rxantcal_ope_regsend[count].regAddr[D_DPDA_ANT_AC], (UINT*)&(f_dpdar_rxantcal_ope_regsend[count].regData[freq]));
            }
            else {
       		    BPF_HM_DEVC_REG_WRITE(0, f_dpdar_rxantcal_ope_regsend[count].regAddr[D_DPDA_ANT_BD], (UINT*)&(f_dpdar_rxantcal_ope_regsend[count].regData[freq]));
            }
	    }
        f_com_taskDelay(10,0,0,0);
        /* RXCAL係数計算(ANT.X) */
        cal_val.CALCOS = 0.0;
        cal_val.CALSIN = 0.0;
        cal_val.AMPREFSUM = 0.0;
        cal_val.AMPFBSUM = 0.0;
        
        for(count=0; count<10; count++) {
            cal_val_tmp= f_dpda_rxcal_ope_antcal( antno, freq, &result);
            if(result != D_SYS_OK) {
                /* RXCAL信号送信終了 */
	            for(count2=0 ; count2 < D_DPDA_RXANTCAL_OPE_END_NUM ; count2++){
                    BPF_HM_DEVC_REG_WRITE(0, f_dpdar_rxantcal_ope_regend[count2].regAddr, (UINT*)&(f_dpdar_rxantcal_ope_regend[count2].regData));
  	            }
                BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Rx Ope Cal NG",__func__, antno);

	            cal_val.AMPREFSUM += cal_val_tmp.AMPREFSUM;
	            cal_val.AMPFBSUM += cal_val_tmp.AMPFBSUM;
				/* CALログ */
				f_dpda_cal_log(D_DU_LOG_CAL_TYPE_SLOW_ERR,antno,1,cal_val.AMPREFSUM,cal_val.AMPFBSUM,count+1,freq);

                return D_SYS_NG;
            }
            cal_val.CALCOS += cal_val_tmp.CALCOS;
            cal_val.CALSIN += cal_val_tmp.CALSIN;
            cal_val.AMPREFSUM += cal_val_tmp.AMPREFSUM;
            cal_val.AMPFBSUM += cal_val_tmp.AMPFBSUM;
        }

		/* 先頭の周波数のみ */
		if(freq == 0){
			f_dpda_cal_log(D_DU_LOG_CAL_TYPE_SLOW,antno,1,cal_val.AMPREFSUM,cal_val.AMPFBSUM,count,freq);
		}

        /* 異常flgが全部=1 (SIN=0かつCOS=0)でない場合、計算値を使う */
        /* それ以外の場合は現在の設定値(初期値0)を使う              */
        if(cal_val.CALSIN != 0.0 || cal_val.CALCOS != 0.0) {
            f_dpdaw_antCarStateInfo.ant_info[antno].rxopecal_info.phase[freq] = atan2(cal_val.CALSIN, cal_val.CALCOS);
            BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_INFO , "ANT%d FREQ:%d PHASE:%f REF:%f FB:%f",antno, freq, 
                f_dpdaw_antCarStateInfo.ant_info[antno].rxopecal_info.phase[freq], cal_val.AMPREFSUM, cal_val.AMPFBSUM);
        }

        /* RXCAL信号送信終了 */
	    for(count=0 ; count < D_DPDA_RXANTCAL_OPE_END_NUM ; count++){
            BPF_HM_DEVC_REG_WRITE(0, f_dpdar_rxantcal_ope_regend[count].regAddr, (UINT*)&(f_dpdar_rxantcal_ope_regend[count].regData));
  	    }

        (void)clock_gettime(CLOCK_MONOTONIC_RAW, &end_time);

        wait_time = end_time.tv_nsec + 1000000000*(end_time.tv_sec-start_time.tv_sec) - start_time.tv_nsec;
        wait_time /= 1000000;
        if(wait_time > 1000) {
            wait_time = 1000;
        }
		f_com_taskDelay(1000 - wait_time,0,0,0);
    }

	BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_ENTER , "%s end",__func__);
    return D_SYS_OK;
}


/*!
 * @brief TXCAL運用中(TXCAL(ANT.X))(高速モード)
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/07/19 FJT)Hasegawa
 */
INT f_dpda_txfastcal_ope_txcal( UINT antno)
{
    UINT count, cnt, count2;
    INT result;
    DL   PHASE;
    T_DPDA_OPECAL_PHASE_VALUE_TBL  cal_val={0.0, 0.0,0.0, 0.0}, cal_val_tmp;

	BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_ENTER , "%s ANT%d start", __func__ ,antno);

    for(cnt=0; cnt<2; cnt++) {

        f_dpdaw_antCarStateInfo.ant_info[antno].txopecal_info.cal_ant_flg[D_DPDA_FREQ_FAST] = D_RRH_OFF;
        /* TXCAL信号送信設定 */
	    for(count=0 ; count < D_DPDA_TXANTCAL_OPE_SEND_NUM ; count++) {
            if(antno == D_RRH_ANT_A || antno == D_RRH_ANT_C ) {
    		    BPF_HM_DEVC_REG_WRITE(0, f_dpdar_txantfastcal_ope_regsend[count].regAddr[D_DPDA_ANT_AC], (UINT*)&(f_dpdar_txantfastcal_ope_regsend[count].regData));
            }
            else {
       		    BPF_HM_DEVC_REG_WRITE(0, f_dpdar_txantfastcal_ope_regsend[count].regAddr[D_DPDA_ANT_BD], (UINT*)&(f_dpdar_txantfastcal_ope_regsend[count].regData));
            }
	    }
        f_com_taskDelay(10,0,0,0);
        /* TXCAL係数計算(ANT.X) */
        cal_val.CALCOS = 0.0;
        cal_val.CALSIN = 0.0;
        cal_val.AMPREFSUM = 0.0;
        cal_val.AMPFBSUM = 0.0;

        for(count=0; count<6; count++) {
            cal_val_tmp = f_dpda_txcal_ope_antcal( antno, D_DPDA_FREQ_FAST, &result);
            if(result != D_SYS_OK) {
                /* TXCAL信号送信終了 */
	            for(count2=0 ; count2 < D_DPDA_TXANTCAL_OPE_END_NUM ; count2++){
                    BPF_HM_DEVC_REG_WRITE(0, f_dpdar_txantcal_ope_regend[count2].regAddr, (UINT*)&(f_dpdar_txantcal_ope_regend[count2].regData));
  	            }
                BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Tx Fast Cal NG",__func__, antno);

	            cal_val.AMPREFSUM += cal_val_tmp.AMPREFSUM;
	            cal_val.AMPFBSUM += cal_val_tmp.AMPFBSUM;
				/* CALログ */
				f_dpda_cal_log(D_DU_LOG_CAL_TYPE_FAST_ERR,antno,0,cal_val.AMPREFSUM,cal_val.AMPFBSUM,count+1,0xFF);

                return D_SYS_NG;
            }
            cal_val.CALCOS += cal_val_tmp.CALCOS;
            cal_val.CALSIN += cal_val_tmp.CALSIN;
            cal_val.AMPREFSUM += cal_val_tmp.AMPREFSUM;
            cal_val.AMPFBSUM += cal_val_tmp.AMPFBSUM;
        }

		/* CALログ */
		f_dpda_cal_log(D_DU_LOG_CAL_TYPE_FAST,antno,0,cal_val.AMPREFSUM,cal_val.AMPFBSUM,count,0xFF);

        /* 異常flgが全部=1 (SIN=0かつCOS=0)でない場合、計算値を使う */
        /* それ以外の場合は現在の設定値(初期値0)を使う              */
        if(cal_val.CALSIN != 0.0 || cal_val.CALCOS != 0.0) {
            PHASE = atan2(cal_val.CALSIN, cal_val.CALCOS);
            BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_INFO , "ANT%d PHASE:%f REF:%f FB:%f",antno, PHASE, cal_val.AMPREFSUM, cal_val.AMPFBSUM);
        
		    /* 高速モードの場合は4周波数全て同じ値を設定 */
		    for(count=0; count<D_DPDA_CAL_FREQ_NUM; count++) {
        	    f_dpdaw_antCarStateInfo.ant_info[antno].txopecal_info.phase[count] = PHASE;
		    }
        }

        /* TXCAL信号送信終了 */
	    for(count=0 ; count < D_DPDA_TXANTCAL_OPE_END_NUM ; count++){
            BPF_HM_DEVC_REG_WRITE(0, f_dpdar_txantcal_ope_regend[count].regAddr, (UINT*)&(f_dpdar_txantcal_ope_regend[count].regData));
  	    }
		  if(f_dpdaw_info_all->taskInfo.flg.cal_error_flg == D_RRH_OFF) {
			  break;
		  }
    }
	if(cnt >= 2 && f_dpdaw_info_all->taskInfo.flg.cal_error_flg == D_RRH_OFF) {
		for(count=0; count<D_DPDA_CAL_FREQ_NUM; count++) {
        	f_dpdaw_antCarStateInfo.ant_info[antno].txopecal_info.phase[count] = 0.0;
		}
	}

	BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_ENTER , "%s end",__func__);
    return D_SYS_OK;
}

/*!
 * @brief RXCAL運用中(RXCAL(ANT.X))(高速モード)
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/07/19 FJT)Hasegawa
 */
INT f_dpda_rxfastcal_ope_rxcal( UINT antno)
{
    UINT count, cnt, count2;
    INT result;
    DL   PHASE;
    T_DPDA_OPECAL_PHASE_VALUE_TBL  cal_val={0.0, 0.0,0.0, 0.0}, cal_val_tmp;

	BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_ENTER , "%s ANT%d start", __func__ ,antno);

    for(cnt=0; cnt<2; cnt++) {

        f_dpdaw_antCarStateInfo.ant_info[antno].rxopecal_info.cal_ant_flg[D_DPDA_FREQ_FAST] = D_RRH_OFF;
        /* RXCAL信号送信設定 */
	    for(count=0 ; count < D_DPDA_RXANTCAL_OPE_SEND_NUM ; count++) {
            if(antno == D_RRH_ANT_A || antno == D_RRH_ANT_C ) {
    		    BPF_HM_DEVC_REG_WRITE(0, f_dpdar_rxantfastcal_ope_regsend[count].regAddr[D_DPDA_ANT_AC], (UINT*)&(f_dpdar_rxantfastcal_ope_regsend[count].regData));
            }
            else {
       		    BPF_HM_DEVC_REG_WRITE(0, f_dpdar_rxantfastcal_ope_regsend[count].regAddr[D_DPDA_ANT_BD], (UINT*)&(f_dpdar_rxantfastcal_ope_regsend[count].regData));
            }
	    }
        f_com_taskDelay(10,0,0,0);
        /* RXCAL係数計算(ANT.X) */
        cal_val.CALCOS = 0.0;
        cal_val.CALSIN = 0.0;
        cal_val.AMPREFSUM = 0.0;
        cal_val.AMPFBSUM = 0.0;

        for(count=0; count<6; count++) {
            cal_val_tmp = f_dpda_rxcal_ope_antcal( antno, D_DPDA_FREQ_FAST, &result);
            if(result != D_SYS_OK) {
                /* RXCAL信号送信終了 */
	            for(count2=0 ; count2 < D_DPDA_RXANTCAL_OPE_END_NUM ; count2++){
                    BPF_HM_DEVC_REG_WRITE(0, f_dpdar_rxantcal_ope_regend[count2].regAddr, (UINT*)&(f_dpdar_rxantcal_ope_regend[count2].regData));
  	            }
                BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Rx Fast Cal NG",__func__, antno);

	            cal_val.AMPREFSUM += cal_val_tmp.AMPREFSUM;
	            cal_val.AMPFBSUM += cal_val_tmp.AMPFBSUM;

				/* CALログ */
				f_dpda_cal_log(D_DU_LOG_CAL_TYPE_FAST_ERR,antno,1,cal_val.AMPREFSUM,cal_val.AMPFBSUM,count+1,0xFF);

                return D_SYS_NG;
            }
            cal_val.CALCOS += cal_val_tmp.CALCOS;
            cal_val.CALSIN += cal_val_tmp.CALSIN;
            cal_val.AMPREFSUM += cal_val_tmp.AMPREFSUM;
            cal_val.AMPFBSUM += cal_val_tmp.AMPFBSUM;
        }

		/* CALログ */
		f_dpda_cal_log(D_DU_LOG_CAL_TYPE_FAST,antno,1,cal_val.AMPREFSUM,cal_val.AMPFBSUM,count,0xFF);

        /* 異常flgが全部=1 (SIN=0かつCOS=0)でない場合、計算値を使う */
        /* それ以外の場合は現在の設定値(初期値0)を使う              */
        if(cal_val.CALSIN != 0.0 || cal_val.CALCOS != 0.0) {
            PHASE = atan2(cal_val.CALSIN, cal_val.CALCOS);
            BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_INFO , "ANT%d PHASE:%f REF:%f FB:%f",antno, PHASE, cal_val.AMPREFSUM, cal_val.AMPFBSUM);

		    /* 高速モードの場合は4周波数全て同じ値を設定 */
		    for(count=0; count<D_DPDA_CAL_FREQ_NUM; count++) {
        	    f_dpdaw_antCarStateInfo.ant_info[antno].rxopecal_info.phase[count] = PHASE;
		    }
        }

        /* RXCAL信号送信終了 */
	    for(count=0 ; count < D_DPDA_RXANTCAL_OPE_END_NUM ; count++){
            BPF_HM_DEVC_REG_WRITE(0, f_dpdar_rxantcal_ope_regend[count].regAddr, (UINT*)&(f_dpdar_rxantcal_ope_regend[count].regData));
  	    }

		  if(f_dpdaw_info_all->taskInfo.flg.cal_error_flg == D_RRH_OFF) {
			  break;
		  }
    }
	if(cnt >= 2 && f_dpdaw_info_all->taskInfo.flg.cal_error_flg == D_RRH_OFF) {
		for(count=0; count<D_DPDA_CAL_FREQ_NUM; count++) {
        	f_dpdaw_antCarStateInfo.ant_info[antno].rxopecal_info.phase[count] = 0.0;
		}
	}

	BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_ENTER , "%s end",__func__);
    return D_SYS_OK;
}
/*!
 * @brief TXCAL運用中(TXCAL係数更新 ANT.X)
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/07/19 FJT)Hasegawa
 */
VOID f_dpda_txcal_ope_update( UINT antno)
{
	UINT    read_bit;
	UINT	regval;
	UCHAR	data;
	DL		TXCALPAHSE_dash[D_DPDA_CAL_FREQ_NUM]={};
	DL		TXCALCOEFI = 0.0;
	DL		TXCALCOEFQ = 0.0;
    UINT    TXCALCOEF = 0;
	UINT	count;
	UINT	regtable[D_RRH_ANT_MAX] = {
		0xB1100000, 0xB1100400, 0xB1100800, 0xB1100C00
	};
	USHORT	TXCALPHASEOFFSET_pre;
    INT ret;
    UINT traPosi;

	BPF_HM_DEVC_REG_READ_BIT(0, regtable[antno], 1, &read_bit);
	if(read_bit == 1 && f_dpdaw_antCarStateInfo.ant_info[antno].car_info[D_SYS_ANT_DL][0].CarOnOff != D_DPDA_CAR_OFF) {

        /* CAL位相値TXCALPHASE[0-3]#X'を計算 */
        for(count=0; count<D_DPDA_CAL_FREQ_NUM; count++) {
		    switch(antno) {
		    	case D_RRH_ANT_A:
		    		TXCALPAHSE_dash[count] = 0.0;
		    		break;
		    	case D_RRH_ANT_B:
		    		TXCALPAHSE_dash[count] = 0.0;
		    		break;
		    	case D_RRH_ANT_C:
            	case D_RRH_ANT_D:
                    if(antno == D_RRH_ANT_C) {
		         		TXCALPAHSE_dash[count] = f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_C].txopecal_info.phase[count] - f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_A].txopecal_info.phase[count];
                    }
                    else {
                        TXCALPAHSE_dash[count] = f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_D].txopecal_info.phase[count] - f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_B].txopecal_info.phase[count];
                    }
                    traPosi = D_DU_EEP_TRA_TX_CAL_PHASE_OFFSET_ANT_A   - D_DU_I2C_EEP_TR_OFS + (antno * D_DU_EEP_TRA_TX_ANT_OFFSET);
                    data = f_comw_trainingData->traVal[traPosi];
           	    	TXCALPHASEOFFSET_pre = (((USHORT)data << 8) & 0xFF00);
                    data = f_comw_trainingData->traVal[traPosi+1];
       	        	TXCALPHASEOFFSET_pre |= ((USHORT)data & 0xFF);
                    if((DL)TXCALPHASEOFFSET_pre > 359.0) {
                        TXCALPHASEOFFSET_pre = 359;
                    }
           	    	TXCALPAHSE_dash[count] = TXCALPAHSE_dash[count] + ((DL)TXCALPHASEOFFSET_pre * M_PI) / 180.0;
		    		break;
		    	default:
		    		break;
		    }
        }

        /* TXCALCOEFの計算と格納を同時に実行するため、先にレジスタ設定をする(動作上問題無し) */
        regval = antno;
        BPF_HM_DEVC_REG_WRITE(0, D_DPDA_CAL_TX_DPHASFTSEL_OFFSET, &regval);

        /* CAL補正値TXCALCOEF[0-255]#Xを計算 */
        for(count=0; count<D_DPDA_CAL_COEF_NUM; count++) {
		    switch(antno) {
		    	case D_RRH_ANT_A:
		    	case D_RRH_ANT_B:
		    		TXCALCOEF = 0x40000000;
		    		break;
		    	case D_RRH_ANT_C:
		    	case D_RRH_ANT_D:
                    if(count <= 43) {
                        TXCALCOEFI = cos(TXCALPAHSE_dash[0]);
                        TXCALCOEFQ = sin(TXCALPAHSE_dash[0]);
                    }
                    else if(44 <= count && count <= 95) {
                        TXCALCOEFI = cos(TXCALPAHSE_dash[0])+(count-43)*(cos(TXCALPAHSE_dash[1])-cos(TXCALPAHSE_dash[0]))/52.0;
                        TXCALCOEFQ = sin(TXCALPAHSE_dash[0])+(count-43)*(sin(TXCALPAHSE_dash[1])-sin(TXCALPAHSE_dash[0]))/52.0;
                    }
                    else if(96 <= count && count <= 159) {
                        TXCALCOEFI = cos(TXCALPAHSE_dash[1])+(count-95)*(cos(TXCALPAHSE_dash[2])-cos(TXCALPAHSE_dash[1]))/64.0;
                        TXCALCOEFQ = sin(TXCALPAHSE_dash[1])+(count-95)*(sin(TXCALPAHSE_dash[2])-sin(TXCALPAHSE_dash[1]))/64.0;
                    }
                    else if(160 <= count && count <= 211) {
                        TXCALCOEFI = cos(TXCALPAHSE_dash[2])+(count-159)*(cos(TXCALPAHSE_dash[3])-cos(TXCALPAHSE_dash[2]))/52.0;
                        TXCALCOEFQ = sin(TXCALPAHSE_dash[2])+(count-159)*(sin(TXCALPAHSE_dash[3])-sin(TXCALPAHSE_dash[2]))/52.0;
                    }
                    else if(212 <= count && count <= 255) {
                        TXCALCOEFI = cos(TXCALPAHSE_dash[3]);
                        TXCALCOEFQ = sin(TXCALPAHSE_dash[3]);
                    }
                    TXCALCOEF  =  ((SHORT)(round(16384.0 * TXCALCOEFQ / sqrt(pow(TXCALCOEFI, 2.0) + pow(TXCALCOEFQ, 2.0))))      & (UINT)0xFFFF);
                    TXCALCOEF |=  (((INT)(round(16384.0 * TXCALCOEFI / sqrt(pow(TXCALCOEFI, 2.0) + pow(TXCALCOEFQ, 2.0)))) <<16) &  0xFFFF0000);
		    		break;
		    	default:
		    		break;
		    }
            regval = TXCALCOEF;
   		    BPF_HM_DEVC_REG_WRITE(0, D_DPDA_CAL_TX_DCALCOEFREA_OFFSET+count*0x04, &regval);
        }

        /* DCALENB */
        ret = BPF_HM_DEVC_REG_WRITE_BITON(0, D_DPDA_CAL_TX_DCALENB_OFFSET, (0x000000001 << antno));
        if(ret != BPF_HM_DEVC_COMPLETE){
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "DCALENB write error=0x%08x", ret );
        }

        /* DCALENB */
        ret = BPF_HM_DEVC_REG_WRITE_BITON(0, D_DPDA_CAL_TX_DCALENB_OFFSET, (0x000010000 << antno));
        if(ret != BPF_HM_DEVC_COMPLETE){
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "DCALENB write error=0x%08x", ret );
	    }
    }

}

/*!
 * @brief RXCAL運用中(RXCAL係数更新 ANT.X)
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/07/19 FJT)Hasegawa
 */
VOID f_dpda_rxcal_ope_update(UINT antno)
{
	UINT    read_bit;
	UINT	regval;
	UCHAR	data;
	DL		RXCALPAHSE_dash[D_DPDA_CAL_FREQ_NUM] = {};
	DL		RXCALCOEFI = 0.0;
	DL		RXCALCOEFQ = 0.0;
    UINT    RXCALCOEF = 0;
	UINT	count;
	UINT	regtable[D_RRH_ANT_MAX] = {
		0xB1300000, 0xB1300400, 0xB1300800, 0xB1300C00
	};
	USHORT	RXCALPHASEOFFSET_pre;
    INT ret;
    UINT traPosi;

	BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, regtable[antno], 1, &read_bit);
	if(read_bit == 1 && f_dpdaw_antCarStateInfo.ant_info[antno].car_info[D_SYS_ANT_UL][0].CarOnOff != D_DPDA_CAR_OFF) {

        /* CAL位相値RXCALPHASE[0-3]#X'を計算 */
        for(count=0; count<D_DPDA_CAL_FREQ_NUM; count++) {
		    switch(antno) {
		    	case D_RRH_ANT_A:
		    		RXCALPAHSE_dash[count] = 0.0;
		    		break;
		    	case D_RRH_ANT_B:
		    		RXCALPAHSE_dash[count] = 0.0;
		    		break;
		    	case D_RRH_ANT_C:
		    	case D_RRH_ANT_D:
                    if(antno == D_RRH_ANT_C) {
		         		RXCALPAHSE_dash[count] = f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_C].rxopecal_info.phase[count] - f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_A].rxopecal_info.phase[count];
                    }
                    else {
                        RXCALPAHSE_dash[count] = f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_D].rxopecal_info.phase[count] - f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_B].rxopecal_info.phase[count];
                    }
                    if (f_cmw_tra_inv_tbl->cmx_rxcalcmp_flag == 1) {
                        traPosi = D_DU_EEP_TRA_RX_CAL_PHASE_OFFSET_ANT_A   - D_DU_I2C_EEP_TR_OFS + (antno * D_DU_EEP_TRA_RX_ANT_OFFSET);
                        data = f_comw_trainingData->traVal[traPosi];
           	    		RXCALPHASEOFFSET_pre = (((USHORT)data << 8) & 0xFF00);
                        data = f_comw_trainingData->traVal[traPosi+1];
       	        		RXCALPHASEOFFSET_pre |= ((USHORT)data & 0xFF);
                        if((DL)RXCALPHASEOFFSET_pre > 359.0) {
                            RXCALPHASEOFFSET_pre = 359;
                        }
           	    		RXCALPAHSE_dash[count] = RXCALPAHSE_dash[count] + ((DL)RXCALPHASEOFFSET_pre * M_PI) / 180.0;
                    }
		    		break;		    		
		    	default:
		    		break;
		    }
        }

        /* RXCALCOEFの計算と格納を同時に実行するため、先にレジスタ設定をする(動作上問題無し) */
		regval = antno;
		BPF_HM_DEVC_REG_WRITE(0, D_DPDA_CAL_RX_DPHASFTSEL_OFFSET, &regval);
        BPF_HM_DEVC_REG_WRITE(0, D_DPDA_CAL_RX_RFCALREGSEL_OFFSET, &regval);

        /* CAL補正値RXCALCOEF[0-255]#Xを計算 */
        for(count=0; count<D_DPDA_CAL_COEF_NUM; count++) {
		    switch(antno) {
		    	case D_RRH_ANT_A:
		    	case D_RRH_ANT_B:
		    		RXCALCOEF = 0x40000000;
		    		break;
		    	case D_RRH_ANT_C:
		    	case D_RRH_ANT_D:
                    if(count <= 43) {
                        RXCALCOEFI = cos(RXCALPAHSE_dash[0]);
                        RXCALCOEFQ = sin(RXCALPAHSE_dash[0]);
                    }
                    else if(44 <= count && count <= 95) {
                        RXCALCOEFI = cos(RXCALPAHSE_dash[0])+(count-43)*(cos(RXCALPAHSE_dash[1])-cos(RXCALPAHSE_dash[0]))/52.0;
                        RXCALCOEFQ = sin(RXCALPAHSE_dash[0])+(count-43)*(sin(RXCALPAHSE_dash[1])-sin(RXCALPAHSE_dash[0]))/52.0;
                    }
                    else if(96 <= count && count <= 159) {
                        RXCALCOEFI = cos(RXCALPAHSE_dash[1])+(count-95)*(cos(RXCALPAHSE_dash[2])-cos(RXCALPAHSE_dash[1]))/64.0;
                        RXCALCOEFQ = sin(RXCALPAHSE_dash[1])+(count-95)*(sin(RXCALPAHSE_dash[2])-sin(RXCALPAHSE_dash[1]))/64.0;
                    }
                    else if(160 <= count && count <= 211) {
                        RXCALCOEFI = cos(RXCALPAHSE_dash[2])+(count-159)*(cos(RXCALPAHSE_dash[3])-cos(RXCALPAHSE_dash[2]))/52.0;
                        RXCALCOEFQ = sin(RXCALPAHSE_dash[2])+(count-159)*(sin(RXCALPAHSE_dash[3])-sin(RXCALPAHSE_dash[2]))/52.0;
                    }
                    else if(212 <= count && count <= 255) {
                        RXCALCOEFI = cos(RXCALPAHSE_dash[3]);
                        RXCALCOEFQ = sin(RXCALPAHSE_dash[3]);
                    }
                    RXCALCOEF  = ((SHORT)(round(16384.0 * RXCALCOEFQ / sqrt(pow(RXCALCOEFI, 2.0) + pow(RXCALCOEFQ, 2.0)))) &    (UINT)0xFFFF);
                    RXCALCOEF |= (((INT)(round(16384.0 * RXCALCOEFI / sqrt(pow(RXCALCOEFI, 2.0) + pow(RXCALCOEFQ, 2.0)))) <<16) & 0xFFFF0000);
		    		break;
		    	default:
		    		break;
		    }
            regval = RXCALCOEF;
   		    BPF_HM_DEVC_REG_WRITE(0, D_DPDA_CAL_RX_DCALCOEFREA_OFFSET+count*0x04, &regval);
            switch (count) {
                case 43:
                    BPF_HM_DEVC_REG_WRITE(0, D_DPDA_CAL_RX_RFCALCOEF_OFFSET,      &regval);
                    break;
                case 95:
                    BPF_HM_DEVC_REG_WRITE(0, D_DPDA_CAL_RX_RFCALCOEF_OFFSET+0x04, &regval);
                    break;
                case 159:
                    BPF_HM_DEVC_REG_WRITE(0, D_DPDA_CAL_RX_RFCALCOEF_OFFSET+0x08, &regval);
                    break;
                case 212:
                    BPF_HM_DEVC_REG_WRITE(0, D_DPDA_CAL_RX_RFCALCOEF_OFFSET+0x0C, &regval);
                    break;
                default:
                    break;
            }
        }

        /* DCALENB */
        ret = BPF_HM_DEVC_REG_WRITE_BITON(0, D_DPDA_CAL_RX_DCALENB_OFFSET, (0x000000001 << antno));
        if(ret != BPF_HM_DEVC_COMPLETE){
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "DCALENB write error=0x%08x", ret );
        }

        /* DCALENB */
        ret = BPF_HM_DEVC_REG_WRITE_BITON(0, D_DPDA_CAL_RX_DCALENB_OFFSET, (0x000010000 << antno));
        if(ret != BPF_HM_DEVC_COMPLETE){
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "DCALENB write error=0x%08x", ret );
	    }
	}
}

/*!
 * @brief 運用中CALの完了チェック
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/09/10 FJT)Hasegawa
 */
UINT f_dpda_cal_complete_check(UINT cal_ant_flg[], UINT num)
{
    UINT ret = D_RRH_ON;
    for(int i=0; i<num; i++) {
        if(cal_ant_flg[i] != D_RRH_ON) {
            ret = D_RRH_OFF;
        }
    }
    return ret;
}

/*!
 * @brief TXCAL運用中(TXCAL係数反映)
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/07/19 FJT)Hasegawa
 */
VOID f_dpda_txcal_ope_ref(UINT uiAntPairNum, UINT fastMode)
{
	if(uiAntPairNum == D_DPDA_ANT_AC) {
	    if((fastMode== D_RRH_OFF && 
            f_dpda_cal_complete_check(f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_A].txopecal_info.cal_ant_flg, D_DPDA_CAL_FREQ_NUM) == D_RRH_ON && 
            f_dpda_cal_complete_check(f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_C].txopecal_info.cal_ant_flg, D_DPDA_CAL_FREQ_NUM) == D_RRH_ON) ||
            (fastMode== D_RRH_ON &&
            f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_A].txopecal_info.cal_ant_flg[D_DPDA_FREQ_FAST] == D_RRH_ON &&
            f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_C].txopecal_info.cal_ant_flg[D_DPDA_FREQ_FAST] == D_RRH_ON)) {
    	    f_dpda_txcal_ope_update( D_RRH_ANT_A);
       	    f_dpda_txcal_ope_update( D_RRH_ANT_C);
        }
    }
    else if(uiAntPairNum == D_DPDA_ANT_BD) {
	    if((fastMode== D_RRH_OFF && 
            f_dpda_cal_complete_check(f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_B].txopecal_info.cal_ant_flg, D_DPDA_CAL_FREQ_NUM) == D_RRH_ON && 
            f_dpda_cal_complete_check(f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_D].txopecal_info.cal_ant_flg, D_DPDA_CAL_FREQ_NUM) == D_RRH_ON) ||
            (fastMode== D_RRH_ON &&
            f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_B].txopecal_info.cal_ant_flg[D_DPDA_FREQ_FAST] == D_RRH_ON &&
            f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_D].txopecal_info.cal_ant_flg[D_DPDA_FREQ_FAST] == D_RRH_ON)) {
            f_dpda_txcal_ope_update( D_RRH_ANT_B);
            f_dpda_txcal_ope_update( D_RRH_ANT_D);
        }
    }
}

/*!
 * @brief RXCAL運用中(RXCAL係数反映)
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/07/19 FJT)Hasegawa
 */
VOID f_dpda_rxcal_ope_ref(UINT uiAntPairNum, UINT fastMode)
{
	UINT	regval;

	if(uiAntPairNum == D_DPDA_ANT_AC) {
	    if((fastMode== D_RRH_OFF && 
            f_dpda_cal_complete_check(f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_A].rxopecal_info.cal_ant_flg, D_DPDA_CAL_FREQ_NUM) == D_RRH_ON && 
            f_dpda_cal_complete_check(f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_C].rxopecal_info.cal_ant_flg, D_DPDA_CAL_FREQ_NUM) == D_RRH_ON) ||
            (fastMode== D_RRH_ON &&
            f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_A].rxopecal_info.cal_ant_flg[D_DPDA_FREQ_FAST] == D_RRH_ON &&
            f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_C].rxopecal_info.cal_ant_flg[D_DPDA_FREQ_FAST] == D_RRH_ON)) {
    	    f_dpda_rxcal_ope_update( D_RRH_ANT_A);
        	f_dpda_rxcal_ope_update( D_RRH_ANT_C);
    	}
	}
	else if(uiAntPairNum == D_DPDA_ANT_BD) {
	    if((fastMode== D_RRH_OFF && 
            f_dpda_cal_complete_check(f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_B].rxopecal_info.cal_ant_flg, D_DPDA_CAL_FREQ_NUM) == D_RRH_ON && 
            f_dpda_cal_complete_check(f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_D].rxopecal_info.cal_ant_flg, D_DPDA_CAL_FREQ_NUM) == D_RRH_ON) ||
            (fastMode== D_RRH_ON &&
            f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_B].rxopecal_info.cal_ant_flg[D_DPDA_FREQ_FAST] == D_RRH_ON &&
            f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_D].rxopecal_info.cal_ant_flg[D_DPDA_FREQ_FAST] == D_RRH_ON)) {
    	    f_dpda_rxcal_ope_update( D_RRH_ANT_B);
        	f_dpda_rxcal_ope_update( D_RRH_ANT_D);
    	}
	}

    regval = 0x1;
	BPF_HM_DEVC_REG_WRITE(0, 0xA2401180, &regval);
}

/*!
 * @brief TxON/OFFの判定を行う
 * @param	N/A
 * @return  VOID
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No
 * @date 2019/08/22 FJT)Hasegawa
 */
INT f_dpda_cal_get_txonoff(UINT uiAntNum)
{
	INT ret = D_DPDA_ANT_DIS;
    UINT count;

    /* TXに1つ以上キャリアON状態のキャリアがあればONで抜ける */
    for(count = 0; count < D_RRH_CAR_MAX; count++) {
        if(f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[D_SYS_ANT_DL][count].CarOnOff == D_DPDA_CAR_ON) {
		    ret = D_DPDA_ANT_ENA;
        }
    }

	return ret;
}

/*!
 * @brief CALのOK値(apdハソ 5章.CAL 参照)を取得する
 * @param	N/A
 * @return  VOID
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No
 * @date 2020/02/22 FJT)Hasegawa
 */
UINT f_dpda_cal_get_CalOkValue()
{
	UINT ret = 0x0;
    if(f_dpda_peri_checkPaCar(D_RRH_ANT_A) == D_SYS_ON &&
        f_dpda_peri_checkPaCar(D_RRH_ANT_C) == D_SYS_ON) {
        /* A/CがONの場合 b0101 を立てる*/
            ret |= 0x5;
    }
    if(f_dpda_peri_checkPaCar(D_RRH_ANT_B) == D_SYS_ON &&
        f_dpda_peri_checkPaCar(D_RRH_ANT_D) == D_SYS_ON) {
        /* B/DがONの場合 b1010 を立てる*/
            ret |= 0xA;
    }
	return ret;
}


/*!
 * @brief TXCAL運用中
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/07/19 FJT)Hasegawa
 */
INT f_dpda_txcal_operation(UINT uiAntPairNum)
{
    INT     result = D_SYS_NG;
	INT 	COUNT;
    UINT    read_bit;
	UINT	regval;
    UINT    read_P;
    UINT    read_Q;

	BPF_COM_PLOG_TRACE_CAL(D_RRH_LOG_AST_LV_ENTER , "%s ANTPAIR%d start", __func__, uiAntPairNum);

	BPF_HM_DEVC_REG_WRITE_BITOFF(0, 0xB1202000, 1);

    BPF_HM_DEVC_REG_READ(0, 0xB1000040, &read_P);
    BPF_HM_DEVC_REG_READ(0, 0xB1000050, &read_Q);

    if((read_P != 0x3) || (read_Q != 0x3))
    {
        /* DCALENB */
        BPF_HM_DEVC_REG_WRITE_BITOFF(0, D_DPDA_CAL_TX_DCALENB_OFFSET, 0x00000001);
        
        regval = 0x3;
        BPF_HM_DEVC_REG_WRITE(0, 0xB1000040, &regval);
        regval = 0x3;
        BPF_HM_DEVC_REG_WRITE(0, 0xB1000050, &regval);
    }

    for(COUNT = 0; COUNT < 200; COUNT++) {
        f_com_taskDelay(1, 0, 0, 0);
        BPF_HM_DEVC_REG_READ_BIT(0, 0xB1202410, 0xF0000000, &read_bit);
        if(read_bit == 0xF0000000) {

            /* ログ強化 */
            BPF_HM_DEVC_REG_READ(0, 0xB1202410, &read_bit);
            BPF_COM_PLOG_TRACE_CAL(D_RRH_LOG_AST_LV_ENTER , "[LOG]%s TX CAL 0xB1202410=0x%x", __func__, read_bit);
            read_bit = f_dpda_cal_get_CalOkValue();
            BPF_COM_PLOG_TRACE_CAL(D_RRH_LOG_AST_LV_ENTER , "[LOG]%s TX CAL f_dpda_cal_get_CalOkValue=0x%x", __func__, read_bit);
            /* ログ強化 */

            BPF_HM_DEVC_REG_READ_BIT(0, 0xB1202410, 0xF, &read_bit);
            if(read_bit == f_dpda_cal_get_CalOkValue()) {
                if(uiAntPairNum == D_DPDA_ANT_AC) {
                    result = f_dpda_txcal_ope_txcal(D_RRH_ANT_A);
                    if(result != D_SYS_OK) {
                        BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Tx Cal NG",__func__, D_RRH_ANT_A);
                    }
                    else {
                        result = f_dpda_txcal_ope_txcal(D_RRH_ANT_C);
                        if(result != D_SYS_OK) {
                            BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Tx Cal NG",__func__, D_RRH_ANT_C);
                        }
                    }
                } 
                else {
                    result = f_dpda_txcal_ope_txcal(D_RRH_ANT_B);
                    if(result != D_SYS_OK) {
                        BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Tx Cal NG",__func__, D_RRH_ANT_B);
                    }
                    else {
                        result = f_dpda_txcal_ope_txcal(D_RRH_ANT_D);
                        if(result != D_SYS_OK) {
                            BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Tx Cal NG",__func__, D_RRH_ANT_D);
                        }
                    }

                }
                if(result == D_SYS_OK) {
                    /* TXCAL係数反映 */
                    f_dpda_txcal_ope_ref(uiAntPairNum, D_RRH_OFF);
                }
            }
            break;
        }
    }
    BPF_HM_DEVC_REG_WRITE_BITON(0, 0xB1202000, 1);

    BPF_HM_DEVC_REG_WRITE(0, 0xB1000040, &read_P);
    BPF_HM_DEVC_REG_WRITE(0, 0xB1000050, &read_Q);

	BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_ENTER , "%s end",__func__);
    return result;
}


/*!
 * @brief RXCAL運用中
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/07/19 FJT)Hasegawa
 */
INT f_dpda_rxcal_operation(UINT uiAntPairNum)
{
    INT     result = D_SYS_NG;
	INT 	COUNT;
    UINT    read_bit;
	UINT	regval;
    UINT    read_P;
    UINT    read_Q;
    UINT    read_R;

	BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_ENTER , "%s ANTPAIR%d start", __func__, uiAntPairNum);

  	BPF_HM_DEVC_REG_WRITE_BITOFF(0, 0xB1202000, 1);

    BPF_HM_DEVC_REG_READ(0, 0xB1000040, &read_P);
    BPF_HM_DEVC_REG_READ(0, 0xB1000050, &read_Q);
    BPF_HM_DEVC_REG_READ(0, 0xB1100000, &read_R);

    if((read_P != 0x3) || (read_Q != 0x3)|| (read_R != 0x1))
    {
        /* DCALENB */
        BPF_HM_DEVC_REG_WRITE_BITOFF(0, D_DPDA_CAL_TX_DCALENB_OFFSET, 0x00000001);
           
        regval = 0x1;
        BPF_HM_DEVC_REG_WRITE(0, 0xB1100000, &regval);
        regval = 0x9;
        BPF_HM_DEVC_REG_WRITE(0, 0xB1100010, &regval);
        regval = 0x02000C6C;
        BPF_HM_DEVC_REG_WRITE(0, 0xB1100240, &regval);
        regval = 0x02000C6C;
        BPF_HM_DEVC_REG_WRITE(0, 0xB1100244, &regval);
        regval = 0xC6C;
        BPF_HM_DEVC_REG_WRITE(0, 0xB1100280, &regval);
        regval = 0x3;
        BPF_HM_DEVC_REG_WRITE(0, 0xB1000040, &regval);
        regval = 0x3;
        BPF_HM_DEVC_REG_WRITE(0, 0xB1000050, &regval);
    }

    for(COUNT = 0; COUNT < 200; COUNT++) {
        f_com_taskDelay(1, 0, 0, 0);
        BPF_HM_DEVC_REG_READ_BIT(0, 0xB1202410, 0xF0000000, &read_bit);
        if(read_bit == 0xF0000000) {

            /* ログ強化 */
            BPF_HM_DEVC_REG_READ(0, 0xB1202410, &read_bit);
            BPF_COM_PLOG_TRACE_CAL(D_RRH_LOG_AST_LV_ENTER , "[LOG]%s RX CAL 0xB1202410=0x%x", __func__, read_bit);
            read_bit = f_dpda_cal_get_CalOkValue();
            BPF_COM_PLOG_TRACE_CAL(D_RRH_LOG_AST_LV_ENTER , "[LOG]%s RX CAL f_dpda_cal_get_CalOkValue=0x%x", __func__, read_bit);
            /* ログ強化 */

            BPF_HM_DEVC_REG_READ_BIT(0, 0xB1202410, 0xF, &read_bit);
            if(read_bit == f_dpda_cal_get_CalOkValue()) {
                if(uiAntPairNum == D_DPDA_ANT_AC) {
                    result = f_dpda_rxcal_ope_rxcal(D_RRH_ANT_A);
                    if(result != D_SYS_OK) {
                        BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Rx Cal NG",__func__, D_RRH_ANT_A);
                    }
                    else {
                        result = f_dpda_rxcal_ope_rxcal(D_RRH_ANT_C);
                        if(result != D_SYS_OK) {
                            BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Rx Cal NG",__func__, D_RRH_ANT_C);
                        }
                    }
                }
                else {
                    result = f_dpda_rxcal_ope_rxcal(D_RRH_ANT_B);
                    if(result != D_SYS_OK) {
                        BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Rx Cal NG",__func__, D_RRH_ANT_B);
                    }
                    else {
                        result = f_dpda_rxcal_ope_rxcal(D_RRH_ANT_D);
                        if(result != D_SYS_OK) {
                            BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Rx Cal NG",__func__, D_RRH_ANT_D);
                        }
                    }
                }

                if(result == D_SYS_OK) {
                    /* RXCAL係数反映 */
                    f_dpda_rxcal_ope_ref(uiAntPairNum, D_RRH_OFF);
                }
            }
            break;
        }
    }

    BPF_HM_DEVC_REG_WRITE(0, 0xB1000040, &read_P);
    BPF_HM_DEVC_REG_WRITE(0, 0xB1000050, &read_Q);
    BPF_HM_DEVC_REG_WRITE(0, 0xB1100000, &read_R);

    BPF_HM_DEVC_REG_WRITE_BITON(0, 0xB1202000, 1);
	BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_ENTER , "%s end",__func__);
    return result;
}


/*!
 * @brief TXCAL運用中(高速モード)
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/07/19 FJT)Hasegawa
 */
INT f_dpda_txfastcal_operation(UINT uiAntPairNum)
{
    INT     result = D_SYS_NG;
	INT 	COUNT;
    UINT    read_bit;
	UINT	regval;
    UINT    read_P;
    UINT    read_Q;

	BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_ENTER , "%s ANTPAIR%d start", __func__, uiAntPairNum);

	BPF_HM_DEVC_REG_WRITE_BITOFF(0, 0xB1202000, 1);

    BPF_HM_DEVC_REG_READ(0, 0xB1000040, &read_P);
    BPF_HM_DEVC_REG_READ(0, 0xB1000050, &read_Q);

    if((read_P != 0x3) || (read_Q != 0x3))
    {
        /* DCALENB */
        BPF_HM_DEVC_REG_WRITE_BITOFF(0, D_DPDA_CAL_TX_DCALENB_OFFSET, 0x00000001);
        
        regval = 0x3;
        BPF_HM_DEVC_REG_WRITE(0, 0xB1000040, &regval);
        regval = 0x3;
        BPF_HM_DEVC_REG_WRITE(0, 0xB1000050, &regval);
    }

    for(COUNT = 0; COUNT < 200; COUNT++) {
        f_com_taskDelay(1, 0, 0, 0);
        BPF_HM_DEVC_REG_READ_BIT(0, 0xB1202410, 0xF0000000, &read_bit);
        if(read_bit == 0xF0000000) {        

            /* ログ強化 */
            BPF_HM_DEVC_REG_READ(0, 0xB1202410, &read_bit);
            BPF_COM_PLOG_TRACE_CAL(D_RRH_LOG_AST_LV_ENTER , "[LOG]%s TX FAST CAL 0xB1202410=0x%x", __func__, read_bit);
            /* ログ強化 */

            if(uiAntPairNum == D_DPDA_ANT_AC) {
                result = f_dpda_txfastcal_ope_txcal(D_RRH_ANT_A);
                if(result != D_SYS_OK) {
                    BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Tx Fast Cal NG",__func__, D_RRH_ANT_A);
                }
                else {
                    result = f_dpda_txfastcal_ope_txcal(D_RRH_ANT_C);
                    if(result != D_SYS_OK) {
                        BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Tx Fast Cal NG",__func__, D_RRH_ANT_C);
                    }
                }
            }
        else {
            result = f_dpda_txfastcal_ope_txcal(D_RRH_ANT_B);
            if(result != D_SYS_OK) {
                BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Tx Fast Cal NG",__func__, D_RRH_ANT_B);
            }
            else {
                result = f_dpda_txfastcal_ope_txcal(D_RRH_ANT_D);
                if(result != D_SYS_OK) {
                    BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Tx Fast Cal NG",__func__, D_RRH_ANT_D);
                }
            }
        }
        if(result == D_SYS_OK) {
            /* TXCAL係数反映 */
	        f_dpda_txcal_ope_ref(uiAntPairNum, D_RRH_ON);
        }
        break;
        }
    }
	BPF_HM_DEVC_REG_WRITE_BITON(0, 0xB1202000, 1);

    BPF_HM_DEVC_REG_WRITE(0, 0xB1000040, &read_P);
    BPF_HM_DEVC_REG_WRITE(0, 0xB1000050, &read_Q);

	BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_ENTER , "%s end",__func__);
    return result;
}


/*!
 * @brief RXCAL運用中(高速)
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2019/07/19 FJT)Hasegawa
 */
INT f_dpda_rxfastcal_operation(UINT uiAntPairNum)
{
    INT     result = D_SYS_NG;
	INT 	COUNT;
    UINT    read_bit;
	UINT	regval;
    UINT    read_P;
    UINT    read_Q;
    UINT    read_R;

	BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_ENTER , "%s ANTPAIR%d start", __func__, uiAntPairNum);
	
    BPF_HM_DEVC_REG_WRITE_BITOFF(0, 0xB1202000, 1);

    BPF_HM_DEVC_REG_READ(0, 0xB1000040, &read_P);
    BPF_HM_DEVC_REG_READ(0, 0xB1000050, &read_Q);
    BPF_HM_DEVC_REG_READ(0, 0xB1100000, &read_R);

    if((read_P != 0x3) || (read_Q != 0x3)|| (read_R != 0x1))
    {
        /* DCALENB */
        BPF_HM_DEVC_REG_WRITE_BITOFF(0, D_DPDA_CAL_TX_DCALENB_OFFSET, 0x00000001);
            
        regval = 0x00000001;
        BPF_HM_DEVC_REG_WRITE(0, 0xB1100000, &regval);
        regval = 0x00000009;
        BPF_HM_DEVC_REG_WRITE(0, 0xB1100010, &regval);
        regval = 0x02000C6C;
        BPF_HM_DEVC_REG_WRITE(0, 0xB1100240, &regval);
        regval = 0x02000C6C;
        BPF_HM_DEVC_REG_WRITE(0, 0xB1100244, &regval);
        regval = 0x00000C6C;
        BPF_HM_DEVC_REG_WRITE(0, 0xB1100280, &regval);
        regval = 0x3;
        BPF_HM_DEVC_REG_WRITE(0, 0xB1000040, &regval);
        regval = 0x3;
        BPF_HM_DEVC_REG_WRITE(0, 0xB1000050, &regval);
	}

    for(COUNT = 0; COUNT < 200; COUNT++) {
        f_com_taskDelay(1, 0, 0, 0);
        BPF_HM_DEVC_REG_READ_BIT(0, 0xB1202410, 0xF0000000, &read_bit);
        if(read_bit == 0xF0000000) {

            /* ログ強化 */
            BPF_HM_DEVC_REG_READ(0, 0xB1202410, &read_bit);
            BPF_COM_PLOG_TRACE_CAL(D_RRH_LOG_AST_LV_ENTER , "[LOG]%s RX FAST CAL 0xB1202410=0x%x", __func__, read_bit);
            /* ログ強化 */

            if(uiAntPairNum == D_DPDA_ANT_AC) {
                result = f_dpda_rxfastcal_ope_rxcal(D_RRH_ANT_A);
                if(result != D_SYS_OK) {
                    BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Rx Fast Cal NG",__func__, D_RRH_ANT_A);
                }
                else {
                    result = f_dpda_rxfastcal_ope_rxcal(D_RRH_ANT_C);
                    if(result != D_SYS_OK) {
                        BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Rx Fast Cal NG",__func__, D_RRH_ANT_C);
                    }
                }
            }
            else {
                result = f_dpda_rxfastcal_ope_rxcal(D_RRH_ANT_B);
                if(result != D_SYS_OK) {
                    BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Rx Fast Cal NG",__func__, D_RRH_ANT_B);
                }
                else {
                    result = f_dpda_rxfastcal_ope_rxcal(D_RRH_ANT_D);
                    if(result != D_SYS_OK) {
                        BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_WARNING , "%s Ant:%d Rx Fast Cal NG",__func__, D_RRH_ANT_D);
                    }
                }
            }

            if(result == D_SYS_OK) {
                /* RXCAL係数反映 */
	            f_dpda_rxcal_ope_ref(uiAntPairNum, D_RRH_ON);
            }
            break;
        }
    }

    BPF_HM_DEVC_REG_WRITE(0, 0xB1000040, &read_P);
    BPF_HM_DEVC_REG_WRITE(0, 0xB1000050, &read_Q);
    BPF_HM_DEVC_REG_WRITE(0, 0xB1100000, &read_R);

    BPF_HM_DEVC_REG_WRITE_BITON(0, 0xB1202000, 1);

	BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_ENTER , "%s end",__func__);
    return result;
}
/* @} */

