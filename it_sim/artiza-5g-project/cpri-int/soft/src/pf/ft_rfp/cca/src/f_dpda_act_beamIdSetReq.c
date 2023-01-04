/*!
 * @skip  $ld:$
 * @file  f_dpda_act_beamIdSetReq.c
 * @brief BEAMID設定処理
 * @date  2019/6/6 FJT)Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

#define D_DPDA_BEAMID_SET_MAX 	7

T_DPDA_BEAMID_SET	f_dpdar_beamId_TxVal_sub6[E_DPDA_RUINDEX_NUM][D_DPDA_BEAMID_SET_MAX] = {
	{	/* 3.7G */
		{ 0xA2014C00,0xD2BF2D41 },
		{ 0xA2014C04,0x0B1D3F07 },
		{ 0xA2014C08,0x326F2767 },
		{ 0xA2014C0C,0x40000000 },
		{ 0xA2014C10,0x326FD899 },
		{ 0xA2014C14,0x0B1DC0F9 },
		{ 0xA2014C18,0xD2BFD2BF }
	},
	{	/* 4.5G */
		{ 0xA2014C00,0xC3DC15E4 },
		{ 0xA2014C04,0x05943FC2 },
		{ 0xA2014C08,0x304D29FD },
		{ 0xA2014C0C,0x40000000 },
		{ 0xA2014C10,0x304DD603 },
		{ 0xA2014C14,0x0594C03E },
		{ 0xA2014C18,0xC3DCEA1C }
	},
	{	/* dummy */
		{ 0x0,0x0 },
		{ 0x0,0x0 },
		{ 0x0,0x0 },
		{ 0x0,0x0 },
		{ 0x0,0x0 },
		{ 0x0,0x0 },
		{ 0x0,0x0 }
	}
};

T_DPDA_BEAMID_SET	f_dpdar_beamId_RxVal_sub6[E_DPDA_RUINDEX_NUM][D_DPDA_BEAMID_SET_MAX] = {
	{	/* 3.7G */
		{ 0xA2204C00,0xD2BF2D41 },
		{ 0xA2204C04,0x0B1D3F07 },
		{ 0xA2204C08,0x326F2767 },
		{ 0xA2204C0C,0x40000000 },
		{ 0xA2204C10,0x326FD899 },
		{ 0xA2204C14,0x0B1DC0F9 },
		{ 0xA2204C18,0xD2BFD2BF }
	},
	{	/* 4.5G */
		{ 0xA2204C00,0xC3DC15E4 },
		{ 0xA2204C04,0x05943FC2 },
		{ 0xA2204C08,0x304D29FD },
		{ 0xA2204C0C,0x40000000 },
		{ 0xA2204C10,0x304DD603 },
		{ 0xA2204C14,0x0594C03E },
		{ 0xA2204C18,0xC3DCEA1C }
	},
	{	/* dummy */
		{ 0x0,0x0 },
		{ 0x0,0x0 },
		{ 0x0,0x0 },
		{ 0x0,0x0 },
		{ 0x0,0x0 },
		{ 0x0,0x0 },
		{ 0x0,0x0 }
	}
};


T_DPDA_BEAMID_SET	f_dpdar_beamId_Val_sub6[D_DPDA_BEAMID_SET_MAX + 1][2] = {
	{
		{ 0xA2014388,0x00000808 },
		{ 0xA2204388,0x00000808 }
	},
	{
		{ 0xA2014388,0x00000909 },
		{ 0xA2204388,0x00000909 }
	},
	{
		{ 0xA2014388,0x00000a0a },
		{ 0xA2204388,0x00000a0a }
	},
	{
		{ 0xA2014388,0x00000b0b },
		{ 0xA2204388,0x00000b0b }
	},
	{
		{ 0xA2014388,0x00000c0c },
		{ 0xA2204388,0x00000c0c }
	},
	{
		{ 0xA2014388,0x00000d0d },
		{ 0xA2204388,0x00000d0d }
	},
	{
		{ 0xA2014388,0x00000e0e },
		{ 0xA2204388,0x00000e0e }
	},
	{
		{ 0xA2014388,0x00000f0f },
		{ 0xA2204388,0x00000f0f }
	}
};


UINT	f_dpdar_beamId_Val_mmw[] = {
	0x00000000,
	0x00000100,
	0x00000200,
	0x00000300,
	0x00000400,
	0x00000500,
	0x00000600,
	0x00000700,
	0x00000800,
	0x00000900,
	0x00000A00,
	0x00000B00,
	0x00000C00,
	0x00000D00
};

/*!
 * @brief 関数機能概要:BEAMID設定応答処理
 * @note  関数処理内容:BEAMID設定応答処理
 * @param  *rcvMsg_p	[in]	受信メッセージ
 * @return INT
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @retval D_SYS_OK		0:正常終了
 * @date  2019/6/6 FJT)Taniguchi
 */
static VOID f_dpda_act_mnt_beamidSetRsp(VOID* rcvMsg_p, UINT uiOK_NG ,UINT uiNG_Code)
{
	T_SYS_MT_BEAMID_SET_REQ*	req;
	T_SYS_MT_BEAMID_SET_RSP*	thdif_rsp;
	INT							rtn_bpf;
	INT							errcd;

	req = (T_SYS_MT_BEAMID_SET_REQ*)rcvMsg_p;
	
	rtn_bpf = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,
						sizeof( T_SYS_MT_BEAMID_SET_RSP ),
						(VOID **)&thdif_rsp,
						&errcd );

	if( rtn_bpf != BPF_RU_IPCM_OK ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Buff get error. %d", rtn_bpf);
		return;
	}

	thdif_rsp->thdIf.uiEventNo		= D_SYS_MSGID_BEAMID_SET_RSP;
	thdif_rsp->thdIf.uiDstPQueueID	= D_RRH_PROCQUE_PF;
	thdif_rsp->thdIf.uiDstTQueueID	= D_SYS_THDQID_PF_MTI;
	thdif_rsp->thdIf.uiSrcPQueueID	= D_RRH_PROCQUE_PF;
	thdif_rsp->thdIf.uiSrcTQueueID	= D_SYS_THDQID_PF_CCA;
	thdif_rsp->thdIf.uiLength		= sizeof(T_SYS_MT_BEAMID_SET_RSP);
	thdif_rsp->uiBeamId             = req->uiBeamId;
	thdif_rsp->uiOK_NG              = uiOK_NG;
	thdif_rsp->uiNG_Code            = uiNG_Code;


	rtn_bpf = BPF_RU_IPCM_MSGQ_SEND( D_SYS_THDQID_PF_MTI, (void *)thdif_rsp );
	if( rtn_bpf != BPF_RU_IPCM_OK ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed send message. %d", rtn_bpf);
	}

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO , "send MSGID_BEAMID_SET_RSP beamId:%d ret:0x%x code:0x%x", req->uiBeamId,uiOK_NG, uiNG_Code);
	return;
}

/*!
 * @brief 関数機能概要:BEAMID check処理
 * @note  関数処理内容:BEAMID check処理
 * @param  N/A
 * @return UINT
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @retval D_SYS_OK		0:正常終了
 * @date  2019/6/6 FJT)Taniguchi
 */
UINT f_dpda_act_mnt_beamidSetCheck( UINT expect_TxAntCarNum ,UINT expect_RxAntCarNum )
{
	UINT				svinfo[E_RRH_ALMHIS_ALM_FACTOR_NUM];
	UINT				uiAntCnt;
	UINT				uiPaOn;
	UINT				uiCarCnt;
	UINT				uiTxCarOn;
	UINT				uiRxCarOn;
	T_DPDA_ANT_INFO		*antInfo;
	T_DPDA_CAR_INFO		*carInfo;
	UINT				uiTxAntCarNum;
	UINT				uiRxAntCarNum;

	BPF_HM_DEVC_REG_READ_BUFFER(D_RRH_LOG_REG_LV_READ_BUF, D_DU_REG_CTB_SV1INF,sizeof(svinfo),svinfo);
	if((svinfo[2] & ( D_DU_SVCTL_BIT_FLV_MAJOR | D_DU_SVCTL_BIT_FLV_CRITICAL)) != 0){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "ERROR,Mismatch SV State (0x%08x)", svinfo[2]);
		return D_SYS_NG;
	}
	
	uiPaOn = 0;
	uiTxAntCarNum = 0;
	uiRxAntCarNum = 0;
	for( uiAntCnt=0; uiAntCnt < f_dpdaw_ant_max_num ; uiAntCnt++ ){
		antInfo = &(f_dpdaw_antCarStateInfo.ant_info[ uiAntCnt ]);
		if(antInfo->PaOnOff == D_SYS_ON){
			uiPaOn++;
		}
		uiTxCarOn = 0;
		uiRxCarOn = 0;
		for( uiCarCnt=0; uiCarCnt < f_dpdaw_carrier_max_num; uiCarCnt++ ){
			carInfo = &(antInfo->car_info[ 1 ][ uiCarCnt ]);
			if( D_DPDA_CAR_ON == carInfo->CarOnOff ){
				uiTxCarOn++;
			}
			carInfo = &(antInfo->car_info[ 0 ][ uiCarCnt ]);
			if( D_DPDA_CAR_ON == carInfo->CarOnOff ){
				uiRxCarOn++;
			}
		}
		if( uiTxCarOn > 0 ){
			uiTxAntCarNum++;
		}
		if( uiRxCarOn > 0 ){
			uiRxAntCarNum++;
		}
	}
	if(uiPaOn != f_dpdaw_ant_max_num){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_WARNING , "WARN,Mismatch PA on(%d, expected:%d) antCarNum:%d txAnt:%d rxAnt:%d",uiPaOn,f_dpdaw_ant_max_num ,uiTxAntCarNum,uiRxAntCarNum);
		return 2;
	}
	if( uiTxAntCarNum < expect_TxAntCarNum ){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_WARNING , "WARN,Mismatch TX-ANT-Carrier on(%d, expected:%d)",uiTxAntCarNum,expect_TxAntCarNum );
		return 3;
	}
	if( uiRxAntCarNum < expect_RxAntCarNum ){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_WARNING , "WARN,Mismatch RX-ANT-Carrier on(%d, expected:%d)",uiRxAntCarNum,expect_RxAntCarNum );
		return 4;
	}
	return D_SYS_OK;
}

/*!
 * @brief 関数機能概要:全キャリアON sub6用
 * @note  関数処理内容:全キャリアON設定処理 sub6用
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @retval D_SYS_OK		0:正常終了
 * @date  2019/6/6 FJT)Taniguchi
 */
VOID f_dpda_act_mnt_allCarrierOn_sub6(VOID)
{
	UINT	uiAntNum;
	UINT	uiCarrier = 0;
	UINT	uiUnitType = E_DPDA_REKIND_5GDU_SUB6;
	int		ret = D_DPDA_RTN_COMPLETE;
	T_DPDA_CAR_INFO		*carInfo;
	UINT			uiAccellId;
	UINT			uiCcId;
	UINT			uiDuPortId;
	UINT			uiCrCentFreq;
	UINT			uiBw;
	long long int	Gain;

	/* TX */
	for(uiAntNum=0 ; uiAntNum < f_dpdaw_ant_max_num ; uiAntNum++){
		carInfo = &(f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier]);
		uiAccellId     = carInfo->AcellId;
		uiCcId         = carInfo->CcId;
		uiDuPortId     = carInfo->PortId;
		uiCrCentFreq   = carInfo->ChannelNum;
		uiBw           = carInfo->BandWidth;
		Gain          = (long long int)(carInfo->Gain);

		if((ret = f_dpda_car_setTxParameter(uiAntNum, uiCarrier, uiAccellId, uiCcId, uiDuPortId, uiUnitType, uiBw)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}

		if((ret = f_dpda_car_setTxBwandNco(uiAntNum, uiCarrier, uiCrCentFreq, uiBw, uiUnitType)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}

		if((ret = f_dpda_car_setTxPhaseCompensationTable(uiCarrier, uiCrCentFreq, E_DPDA_SCS_30KHZ, uiUnitType)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}

		if((ret = f_dpda_car_setTxLPFGain(uiAntNum, uiCarrier, uiUnitType , Gain)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}

		if((ret = f_dpda_com_setTddConfig(uiUnitType)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}

		f_dpda_com_setDpdStop(uiAntNum);

		if((ret = f_dpda_car_setTxCalPhaseCorrection(uiCarrier,uiAntNum)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}

		if((ret = f_dpda_car_lutUpdate(uiAntNum)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}

		if((ret = f_dpda_com_setImpulseCoefficient(uiAntNum, 1, uiCrCentFreq)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}

		if((ret = f_dpda_car_TxcarEnDis(uiAntNum, uiCarrier, uiUnitType, 1)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}

		if((ret = f_dpda_com_TddAntEnDis(uiCarrier, uiAntNum, uiUnitType, 1)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}

		if((ret = f_dpda_com_setDpdStart(uiAntNum, D_RRH_OFF)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}
	}

	for(uiAntNum=0 ; uiAntNum < f_dpdaw_ant_max_num ; uiAntNum++){
		carInfo = &(f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[0][uiCarrier]);
		uiAccellId     = carInfo->AcellId;
		uiCcId         = carInfo->CcId;
		uiDuPortId     = carInfo->PortId;
		uiCrCentFreq   = carInfo->ChannelNum;
		uiBw           = carInfo->BandWidth;

		if((ret = f_dpda_car_setRxParameter(uiAntNum, uiCarrier, uiAccellId, uiCcId, uiDuPortId, uiUnitType, uiBw)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}
		if((ret = f_dpda_car_setRxBwandNco(uiAntNum, uiCarrier, uiCrCentFreq, uiBw, uiUnitType)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}
		if((ret = f_dpda_car_setRxCalPhaseCorrection(uiCarrier,uiAntNum)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}
		if((ret = f_dpda_car_setRxPhaseCompensationTable(uiCarrier, uiCrCentFreq, E_DPDA_SCS_30KHZ, uiUnitType)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}
		if((ret = f_dpda_com_setTddConfig(uiUnitType)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}
		if((ret = f_dpda_car_RxcarEnDis(uiAntNum, uiCarrier, uiUnitType, 1)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}
		if((ret = f_dpda_com_TddAntEnDis(uiCarrier, uiAntNum, uiUnitType, 1)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}
	}

	/* BF有、TX/RX 全ANT 高速CAL モードON */
	if(f_comw_bf_mode->flag == D_RRH_OFF) {
		f_dpdaw_info_all->taskInfo.flg.txcal_ant_fast_mode_flg[D_DPDA_ANT_AC] = D_RRH_ON;
		f_dpdaw_info_all->taskInfo.flg.txcal_ant_fast_mode_flg[D_DPDA_ANT_BD] = D_RRH_ON;
		f_dpdaw_info_all->taskInfo.flg.rxcal_ant_fast_mode_flg[D_DPDA_ANT_AC] = D_RRH_ON;
		f_dpdaw_info_all->taskInfo.flg.rxcal_ant_fast_mode_flg[D_DPDA_ANT_BD] = D_RRH_ON;
	}

	f_dpda_com_clearQEC();

	return;
}


/*!
 * @brief 関数機能概要:全キャリアOFF sub6用
 * @note  関数処理内容:全キャリアOFF設定処理 sub6用
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @retval D_SYS_OK		0:正常終了
 * @date  2019/6/6 FJT)Taniguchi
 */
VOID f_dpda_act_mnt_allCarrierOff_sub6(VOID)
{
	UINT	uiAntNum;
	UINT	uiCarrier = 0;
	UINT	uiUnitType = E_DPDA_REKIND_5GDU_SUB6;
	int		ret = D_DPDA_RTN_COMPLETE;

	for(uiAntNum=0 ; uiAntNum < f_dpdaw_ant_max_num; uiAntNum++){
		if((ret = f_dpda_com_setDpdStop(uiAntNum)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}
		if((ret = f_dpda_com_setImpulseCoefficient(uiAntNum, 0, 0)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}
		if((ret = f_dpda_car_lutUpdate(uiAntNum)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}
		if((ret = f_dpda_car_TxcarEnDis(uiAntNum, uiCarrier, uiUnitType, 0)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}
		if((ret = f_dpda_com_TddAntEnDis(uiCarrier, uiAntNum, uiUnitType, 0)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}
	}
	
	for(uiAntNum=0 ; uiAntNum < f_dpdaw_ant_max_num; uiAntNum++){
		if((ret = f_dpda_car_RxcarEnDis(uiAntNum, uiCarrier, uiUnitType, 0)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}
		if((ret = f_dpda_com_TddAntEnDis(uiCarrier, uiAntNum, uiUnitType, 0)) != D_DPDA_RTN_COMPLETE){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Ant%d ret:%d",uiAntNum,ret);
			continue;
		}
	}
	return;
}


/*!
 * @brief 関数機能概要:BEAMID設定処理 sub6用
 * @note  関数処理内容:BEAMID設定処理 sub6用
 * @param  *rcvMsg_p	[in]	受信メッセージ
 * @return INT
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @retval D_SYS_OK		0:正常終了
 * @date  2019/6/6 FJT)Taniguchi
 */
INT f_dpda_act_mnt_beamidSetReq_sub6(VOID* rcvMsg_p)
{
	T_SYS_MT_BEAMID_SET_REQ*	req;
	UINT						uiOK_NG;
	UINT						uiNG_Code;
	UINT						count;
	INT							ret;

	req = (T_SYS_MT_BEAMID_SET_REQ*)rcvMsg_p;

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO , "recv MSGID_BEAMID_SET_REQ beamId:%d", req->uiBeamId);

	/* 全Ant-carrierがONであること */
	if((uiOK_NG = f_dpda_act_mnt_beamidSetCheck(f_dpdaw_ant_max_num,f_dpdaw_ant_max_num)) == D_SYS_OK){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Setting BeamId %d and 1sec irq stop", req->uiBeamId);
		f_com_SVCtl(E_DU_SVCTL_OPE_FIDF2_ALM, D_DU_SVCTL_ON, D_DU_FLTID_BEAM_ID_FIX);
		f_comw_rf_setup_flg = D_RRH_OFF;	/* 1sec割り込み抑止,calが10秒くらいかかるため */

		f_dpda_act_mnt_allCarrierOff_sub6();

		struct sched_param	set_prams;
		struct sched_param	cur_prams;
		int					cur_policy;
		pthread_t			cur_thread = pthread_self();

		(void)pthread_getschedparam(cur_thread, &cur_policy,&cur_prams);

		/* のんびりやる */
		set_prams.sched_priority = 0;
		(void)pthread_setschedparam(cur_thread,SCHED_OTHER,(const struct sched_param*)&set_prams);

		ret = system("rrh -txcal 1>/var/log/mt_txcal.log 2>/var/log/mt_txcal_err.log");
		if( ret == D_RRH_MNG ){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_WARNING , "txcal command error:%d",ret);
		}

		ret = system("rrh -rxcal 1>/var/log/mt_rxcal.log 2>/var/log/mt_rxcal_err.log");
		if( ret == D_RRH_MNG ){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_WARNING , "rxcal command error:%d",ret);
		}

		(void)pthread_setschedparam(cur_thread,cur_policy,(const struct sched_param*)&cur_prams);

		for(count=0 ; count < D_DPDA_BEAMID_SET_MAX; count++){
			f_dpda_IO_write(f_dpdar_beamId_TxVal_sub6[f_dpdaw_ruIndex][count].regAddr,
							f_dpdar_beamId_TxVal_sub6[f_dpdaw_ruIndex][count].regData,
							E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE);
		}

		for(count=0 ; count < D_DPDA_BEAMID_SET_MAX; count++){
			f_dpda_IO_write(f_dpdar_beamId_RxVal_sub6[f_dpdaw_ruIndex][count].regAddr,
							f_dpdar_beamId_RxVal_sub6[f_dpdaw_ruIndex][count].regData,
							E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE);
		}

		for(count=0 ; count < 2; count++){
			f_dpda_IO_write(f_dpdar_beamId_Val_sub6[req->uiBeamId][count].regAddr,
							f_dpdar_beamId_Val_sub6[req->uiBeamId][count].regData,
							E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE);
		}

		f_dpda_act_mnt_allCarrierOn_sub6();
		f_comw_rf_setup_flg = D_RRH_ON;	/* 1sec割り込み抑止解除 */

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Finish BeamId %d OK and 1sec irq restart",req->uiBeamId);
		uiOK_NG = D_SYS_OK;
		uiNG_Code = D_SYS_OK;

	}else{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Setting BeamId %d NG(%u)", req->uiBeamId,uiOK_NG);
		uiOK_NG = D_SYS_NG;
		uiNG_Code = D_SYS_NG;
	}

	f_dpda_act_mnt_beamidSetRsp(rcvMsg_p,uiOK_NG,uiNG_Code);
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO , "BeamID:%d return(%u)",req->uiBeamId,uiOK_NG );
	return D_SYS_OK;
}

/*!
 * @brief 関数機能概要:BEAMID設定処理 mmW用
 * @note  関数処理内容:BEAMID設定処理 mmW用
 * @param  *rcvMsg_p	[in]	受信メッセージ
 * @return INT
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @retval D_SYS_OK		0:正常終了
 * @date  2019/6/6 FJT)Taniguchi
 */
INT f_dpda_act_mnt_beamidSetReq_mmW(VOID* rcvMsg_p)
{
	T_SYS_MT_BEAMID_SET_REQ*	req;
	UINT						uiOK_NG = D_SYS_OK;
	UINT						uiNG_Code = D_SYS_OK;
	UINT						count1,count2,count3;
	UINT						maxCnt=0;
	UINT						base=0;
	UINT						val=0;

	req = (T_SYS_MT_BEAMID_SET_REQ*)rcvMsg_p;
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO , "recv MSGID_BEAMID_SET_REQ beamId:%d", req->uiBeamId);

	/* 最低1 Ant-carrier設定済 */
	if((uiOK_NG = f_dpda_act_mnt_beamidSetCheck(1,1)) == D_SYS_OK){

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Setting BeamId %d and 1sec irq stop", req->uiBeamId);

		f_com_SVCtl(E_DU_SVCTL_OPE_FIDF2_ALM, D_DU_SVCTL_ON, D_DU_FLTID_BEAM_ID_FIX);
		f_comw_rf_setup_flg = D_RRH_OFF;	/* 1sec割り込み抑止,sub6と合わせるため */
		f_dpda_IO_write(D_DUM_REG_DLP_DBGBMENB,0x00000001,E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE);

		maxCnt = sizeof(f_dpdar_beamId_Val_mmw)/sizeof(f_dpdar_beamId_Val_mmw[0]);
		for(count1 = 0 ; count1 < 4 ; count1++){
			switch(count1){
				case 0:		/* DL_AntA_+45°偏波 */
					base = 0x80100000;
					break;
				case 1:		/* UL_AntA_+45°偏波 */
					base = 0x80000000;
					break;
				case 2:		/* DL_AntB_-45°偏波 */
					base = 0x80110000;
					break;
				default:	/* UL_AntB_-45°偏波 */
					base = 0x80010000;
					break;
			}
			for(count2 = 0 ; count2 < 8 ; count2++){
				val = base + (count2*0x1000);
				for(count3 = 0 ; count3 < maxCnt ; count3++ ){
					f_dpda_IO_write(D_DUM_REG_DLP_DBGBMCNT,val + f_dpdar_beamId_Val_mmw[count3] + req->uiBeamId,
									E_DPDA_BPF_NOMAL_MODE, D_RRH_LOG_REG_LV_WRITE);
				}
			}
		}
		f_comw_rf_setup_flg = D_RRH_ON;	/* 1sec割り込み抑止解除 */

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Finish BeamId %d OK and 1sec irq restart", req->uiBeamId);
		uiOK_NG = D_SYS_OK;
		uiNG_Code = D_SYS_OK;

	}else{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Setting BeamId %d NG(%u)", req->uiBeamId,uiOK_NG);
		uiOK_NG = D_SYS_NG;
		uiNG_Code = D_SYS_NG;
	}
	f_dpda_act_mnt_beamidSetRsp(rcvMsg_p,uiOK_NG,uiNG_Code);
	return D_SYS_OK;
}

/* @} */
