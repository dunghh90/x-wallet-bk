/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_Common_proc.c
 *  @brief  pf_eqs Common Process service call function
 *  @date   2019/02/13 KCN)Kamimoto create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019
 */
/*********************************************************************************/

#include <ctype.h>
#include "f_eqs_header.h"			/* RE監視タスクヘッダファイル			*/

/** @addtogroup 5GDU_PF_EQS
* @{ */


/**
* @brief Hardware state Enable Notification send function
* @note TRA O-RAN ietf-hardware state Operationaly state Enable Ind factor MSI Send..\n
* @param w_sts_data [in] Status Data
* @return None
* @date 2019/02/25 KCN)Kamimoto 5GDU-LLS対応
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eqs_enable_Edit( UINT sts_data , E_MPSW_ADMIN_STATE *admin_state , E_AVAILABILITY_TYPE *availability_state )
{
	/* 変数宣言 */

	/* プロセス */
	/* Admin状態の読出しと設定 */
	if( (sts_data & D_DU_SVCTL_BIT_ADM_LOCKED_ALL) != D_RRH_OFF )
	{
		/* Admin状態が"Lock"なら */
		*admin_state = E_MPSW_ADMIN_STATE_LOCKED ;
	}
	else if( (sts_data & D_DU_SVCTL_BIT_ADM_UNLOCKED_ALL) != D_RRH_OFF )
	{
		/* Admin状態が"Unlock"なら */
		*admin_state = E_MPSW_ADMIN_STATE_UNLOCKED ;
	}
	else
	{
		/* Admin状態が"Lock"/"Unlock"以外なら */
		*admin_state = E_MPSW_ADMIN_STATE_UNKNOWN ;
	}
	/* Availability状態の設定 */
	if( (sts_data & D_DU_SVCTL_BIT_AVL_NORMAL) == D_DU_SVCTL_BIT_AVL_NORMAL )
	{
		/* Availability状態が"Normal"なら */
		*availability_state = E_AVAILABILITY_TYPE_NORMAL ;
	}
	else if( (sts_data & D_DU_SVCTL_BIT_AVL_DEGRADED) == D_DU_SVCTL_BIT_AVL_DEGRADED )
	{
		/* Availability状態が"Degraded"なら */
		*availability_state = E_AVAILABILITY_TYPE_DEGRADED ;
	}
	else if( (sts_data & D_DU_SVCTL_BIT_AVL_FAULTY) == D_DU_SVCTL_BIT_AVL_FAULTY )
	{
		/* Availability状態が"Fault"なら */
		*availability_state = E_AVAILABILITY_TYPE_FAULTY ;
	}
	else
	{
		/* Availability状態がその他なら */
		*availability_state = E_AVAILABILITY_TYPE_UNKNOWN ;
	}

	return ;
}


/* @{ */
/**
* @brief TRA card status change function
* @note TRA card status change factor judge,table update, and status change handle.\n
* @param svInf1Dat [in] SVINF1 info data
* @return None
* @date 2019/03/29 FJT)koshida 5G商用DU-LLS対応
* @warning	N/A
* @FeatureID	5GDU-002-004
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eqs_stsChgProc( UINT svInf1Dat )
{
	UINT			wSvInfDat;
	UINT			send_flg = D_EQS_NOTIF_NON;
	UINT			loop;
	UCHAR			stsName[E_EQS_STS_MAX][8] = {"Oper", "Admin", "Usage", "Avail", "Power"};

	/* State Notification通知開始状態のチェック */
	if(f_eqs_status_start_data == D_EQS_NTF_START_RUN)
	{
		/************************/
		/*	Oper Status変化通知	*/
		/************************/
		/*	有効ビットのみ取得	*/
		wSvInfDat = svInf1Dat & f_eqs_stsInfoMng[E_EQS_STS_OPER].svInfMsk;
		
		/*	前回値と差分が有る場合	*/
		if(wSvInfDat != f_eqs_stsInfoMng[E_EQS_STS_OPER].svInfDat)
		{
			/*	Oper状態がEnabe変化の場合	*/
			if(wSvInfDat == D_DU_SVCTL_BIT_OPE_ENABLE)
			{
				/*	装置起動後最初のEnabe変化の場合	*/
				if(f_eqs_operIniFlg == D_RRH_OFF)
				{
					/*	起動時のPA状態がPA OFFの場合	*/
					if( f_cmw_tra_inv_tbl->cmx_paon == D_RRH_OFF )
					{
						/*	SVバスにPA OFFを設定する	*/
						f_com_SVCtl(E_DU_SVCTL_OPE_FIDF1_ALM, D_DU_SVCTL_ON, D_DU_FLTID_TX_OFF);
					}
					f_eqs_operIniFlg = D_RRH_ON;
				}
				/* Operational StateがEnableなら */
				/* MSI送信通知(D_MSG_MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND)の通知 */
				(VOID)f_eqs_enable_notif( svInf1Dat ) ;
				/* Notification通知フラグの設定 */
				send_flg = D_EQS_NOTIF_ENABLE ;
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Enable notification(Oper:%08x->%08x[all:%08x->%08x])",
														 f_eqs_stsInfoMng[E_EQS_STS_OPER].svInfDat,
														 wSvInfDat,
														 f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_01],
														 svInf1Dat );
			}
			/*	Oper状態がDisable変化の場合	*/
			else if(wSvInfDat == D_DU_SVCTL_BIT_OPE_DISABLE)
			{
				/* Operatoinal StateがDisableなら */
				/* MSI送信通知(D_MSG_MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND)の通知 */
				(VOID)f_eqs_disable_notif( svInf1Dat ) ;
				/* Notification通知フラグの設定 */
				send_flg = D_EQS_NOTIF_DIASBLE ;
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Disable notification(Oper:%08x->%08x[all:%08x->%08x])",
														 f_eqs_stsInfoMng[E_EQS_STS_OPER].svInfDat,
														 wSvInfDat,
														 f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_01],
														 svInf1Dat );
			}
			/*	Oper状態がunknownの場合	*/
			else
			{
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Oper Sts unknown(Oper:%08x->%08x[all:%08x->%08x])",
														 f_eqs_stsInfoMng[E_EQS_STS_OPER].svInfDat,
														 wSvInfDat,
														 f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_01],
														 svInf1Dat );
			}
			f_eqs_stsInfoMng[E_EQS_STS_OPER].svInfDat = wSvInfDat;
		}
		/*	Admin,Usage,Availibilty,Powerの変化有無をチェックする	*/
		for(loop = E_EQS_STS_ADMIN; loop < E_EQS_STS_MAX; loop++)
		{
			/*	有効ビットのみ取得	*/
			wSvInfDat = svInf1Dat & f_eqs_stsInfoMng[loop].svInfMsk;
			/*	前回値と差分が有る場合	*/
			if(wSvInfDat != f_eqs_stsInfoMng[loop].svInfDat)
			{
				/*	Notification未送信の場合	*/
				if(send_flg == D_EQS_NOTIF_NON)
				{
					/* MSI送信通知(D_MSG_MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND)の通知 */
					f_eqs_status_change_notif( svInf1Dat );
					send_flg = D_EQS_NOTIF_CHANGE ;
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "change notification(%5s:%08x->%08x[all:%08x->%08x])",
													 (UCHAR*)&stsName[loop][0],
													 f_eqs_stsInfoMng[loop].svInfDat,
													 wSvInfDat,
													 f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_01],
													 svInf1Dat );
				}
				/*	状態を更新する	*/
				f_eqs_stsInfoMng[loop].svInfDat = wSvInfDat;
			}
		}
	}
	/* State Notification通知開始が未の場合 */
	else
	{
		/*	装置起動後最初の場合	*/
		if(f_eqs_operIniFlg == D_RRH_OFF)
		{
			/*	前回値がENABLE OFFの場合	*/
			if((f_eqs_stsInfoMng[E_EQS_STS_OPER].svInfDat & D_DU_SVCTL_BIT_OPE_ENABLE) == 0)
			{
				/*	現状値がENABLE ONの場合(OFF->ON)	*/
				if((svInf1Dat & D_DU_SVCTL_BIT_OPE_ENABLE) != 0)
				{
					/*	起動時のPA状態がPA OFFの場合	*/
					if( f_cmw_tra_inv_tbl->cmx_paon == D_RRH_OFF )
					{
						/*	SVバスにPA OFFを設定する	*/
						f_com_SVCtl(E_DU_SVCTL_OPE_FIDF1_ALM, D_DU_SVCTL_ON, D_DU_FLTID_TX_OFF);
					}
					f_eqs_operIniFlg = D_RRH_ON;
				}
			}
		}
	}
	return ;
}
/* @} */


/**
* @brief FM notification check function
* @note FM notification check function\n
* @param sv_inf [in] svinf data
* @return None
* @date 2019/02/26 FJT)Koshida 5GDU-LLS対応
* @warning	N/A
* @FeatureID	RRH-002-004
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eqs_faultChg( UINT	*sv_inf )
{
	UINT				loop ;
	UINT				wSvInfDat[2];
	UINT				sigSndFlg = D_RRH_OFF;
	
	/* FM Notification通知開始状態のチェック */
	if(f_eqs_alm_start_data != D_EQS_NTF_START_RUN)
	{
		/*	処理終了	*/
		return;
	}
	for ( loop = 0;loop < E_EQS_FAULTID_MAX; loop++)
	{
		/*	fault状態変化が有る場合	*/
		wSvInfDat[0] = sv_inf[f_eqs_faultMng[loop].svInfIdx] & f_eqs_faultMng[loop].svInfMsk;
		/*	拡張FLT管理テーブルが有効な場合	*/
		if(f_eqs_faultMng_ex[loop].svInfIdx < E_EQS_SVINF_MAX)
		{
			wSvInfDat[1] = sv_inf[f_eqs_faultMng_ex[loop].svInfIdx] & f_eqs_faultMng_ex[loop].svInfMsk;
		}
		/*	拡張FLT管理テーブルが無効な場合	*/
		else
		{
			wSvInfDat[1] = 0;	/*	Fault要因無し	*/
		}
		/*	現fault状態と前fault状態が不一致している場合	*/
		if((wSvInfDat[0] != f_eqs_faultMng[loop].svInfDat) || (wSvInfDat[1] != f_eqs_faultMng_ex[loop].svInfDat))
		{
			/*	fault復旧状態へ変化の場合	*/
			if((wSvInfDat[0] == 0) && (wSvInfDat[1] == 0))
			{
				f_eqs_send_alarm_notif(loop, D_EQS_BOOL_TRUE);
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "[%d]FLTID#%04x true change notification(%08x_%08x->%08x_%08x)",
												 loop,
												 f_eqs_fault_info[loop].faultId,
												 f_eqs_faultMng_ex[loop].svInfDat,
												 f_eqs_faultMng[loop].svInfDat,
												 wSvInfDat[1],
												 wSvInfDat[0] );
			}
			/*	fault復旧状態から変化の場合	*/
			else if((f_eqs_faultMng[loop].svInfDat == 0) && (f_eqs_faultMng_ex[loop].svInfDat == 0))
			{
				f_eqs_send_alarm_notif(loop, D_EQS_BOOL_FALSE);
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "[%d]FLTID#%04x false change notification(%08x_%08x->%08x_%08x)",
												 loop,
												 f_eqs_fault_info[loop].faultId,
												 f_eqs_faultMng_ex[loop].svInfDat,
												 f_eqs_faultMng[loop].svInfDat,
												 wSvInfDat[1],
												 wSvInfDat[0] );
				
				/*	signal送信未の場合(最初に検出したfaultのみ)	*/
				if(sigSndFlg == D_RRH_OFF)
				{
					/*	CriticalおよびMajorの場合	*/
					if( (f_eqs_fault_info[loop].fault_severity == E_FAULT_SEVERITY_CRITICAL) ||
						(f_eqs_fault_info[loop].fault_severity == E_FAULT_SEVERITY_MAJOR) )
					{
						/*	FLTID#03(PTP Unlock)以外でsig送信していない場合	*/
						if(( f_eqs_fault_info[loop].faultId !=  D_EQS_FALT_FID03) &&
							 ((sigFltId & (D_EQS_MAJOR_OUTOFPTP_FLTBIT | D_EQS_CRITI_FLTBIT)) != (sv_inf[E_EQS_SVINF_04] & (D_EQS_MAJOR_OUTOFPTP_FLTBIT | D_EQS_CRITI_FLTBIT))))
						{
							/****************/
							/* ALM履歴取得  */
							/****************/
							/* INITプロセスへSignal送信 */
							BPF_RM_SVRM_SIGNAL_SEND(D_RRH_PROCID_INI , SIGUSR1 , D_RRH_SIGUSR_OPT_RESORCE_ALM_NTC) ;
							BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "send SIGNAL of resorce ALM(svif04:0x%08x->0x%08x)", 
													f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_04], sv_inf[E_EQS_SVINF_04]);
							sigSndFlg = D_RRH_ON;
							/*	FLT IDを初期化する	*/
							/*	notification開始後は、必ず変化ありであるため、このフラグは0のままとする	*/
							sigFltId = D_RRH_OFF;
						}
					}
				}
			}
			/*	faultの詳細変化の場合	*/
			else
			{
				/*	処理無し	*/
			}
			/*	fault状態を更新	*/
			f_eqs_faultMng[loop].svInfDat = wSvInfDat[0];
			f_eqs_faultMng_ex[loop].svInfDat = wSvInfDat[1];
		}
	}
}


/* @{ */
/**
* @brief stop wave function
* @note stop wave.\n
* @param a_trainf_p [in] the buffer address pointer of received message
* @return None
* @date 2019/03/29 FJT)koshida 5G商用DU-LLS対応
* @warning	N/A
* @FeatureID	5GDU-002-004
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID	f_eqs_tddEnChg_ntc( UINT antNo, UINT chgval , UINT tdd_iq_enable )
{
	/****************************************/
	/*	pf_ccaに依頼メッセージを送信する	*/
	/****************************************/
	T_SYS_RF_TDDEN_CHG_NTC		*pSndMsg;
	int							errcd;
	int							ret;
	
	//通信用共有メモリを取得する
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(  E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別       */
	                                    sizeof(T_SYS_RF_TDDEN_CHG_NTC), 		/* Size             */
	                                    (void**)&pSndMsg,						/* msgP             */
	                                    &errcd );
	
	if( ret != E_API_RCD_OK )
	{
	    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
	    return;
	}
	//データ設定 Head
	pSndMsg->head.uiEventNo     = D_SYS_MSGID_TDDEN_CHG_NTC;		/* TDD Enable change notice	*/
	pSndMsg->head.uiDstPQueueID = D_RRH_PROCQUE_PF;					/* 送信先Process Queue ID   */
	pSndMsg->head.uiDstTQueueID = D_SYS_THDQID_PF_CCA;				/* 送信先Thread Queue ID    */
	pSndMsg->head.uiSrcPQueueID = D_RRH_PROCQUE_PF;					/* 送信元Process Queue ID   */
	pSndMsg->head.uiSrcTQueueID = D_SYS_THDQID_PF_EQS;				/* 送信元Thread Queue ID    */
	pSndMsg->head.uiLength      = sizeof(T_SYS_RF_TDDEN_CHG_NTC);	/* Length(Head部 + Data部)  */
	
	pSndMsg->tddEn.uiAntNo      = antNo;								/* アンテナNo(0~3)			*/
	pSndMsg->tddEn.uichgVal     = chgval;								/* TDD Enable val			*/

	/* tdd enable,iq停波解除時はDPD ONする、それ以外はDPD OFFのまま */
	if( tdd_iq_enable == D_EQS_STPWV_ANT_EN ){
		pSndMsg->tddEn.uiDpdCtl     = D_SYS_ON;
	}else{
		pSndMsg->tddEn.uiDpdCtl     = D_SYS_OFF;
	}

	/* 停波制御抑止通知を送信する */
	ret = BPF_RU_IPCM_MSGQ_SEND(    D_RRH_PROCQUE_PF,					/* 送信先Process Queue ID   */
	                                (void*)pSndMsg );					/* 送信Message              */

	if( ret != E_API_RCD_OK )
	{
	    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
	    return;
	}
}
/* @} */


/* @{ */
/**
* @brief stop wave function
* @note stop wave.\n
* @param a_trainf_p [in] the buffer address pointer of received message
* @return None
* @date 2019/03/29 FJT)koshida 5G商用DU-LLS対応
* @warning	N/A
* @FeatureID	5GDU-002-004
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID	f_eqs_stpWvProc( UINT *sv_inf )
{
	UINT				loop, loopNum;
	UINT				wSvInfDat;
	UINT				stpWvChkFlg[D_RRH_ANT_MAX] = { D_EQS_STPWV_ANT_DIS_A, D_EQS_STPWV_ANT_DIS_B, D_EQS_STPWV_ANT_DIS_C, D_EQS_STPWV_ANT_DIS_D };
	UINT				paoffChkFlg[D_RRH_ANT_MAX] = { D_EQS_STPWV_ANT_PAOFF_A, D_EQS_STPWV_ANT_PAOFF_B, D_EQS_STPWV_ANT_PAOFF_C, D_EQS_STPWV_ANT_PAOFF_D };
	UINT				stpWvFlag[E_EQS_STPWV_REG_MAX] = {D_EQS_STPWV_ANT_EN, D_EQS_STPWV_ANT_EN};
	UINT				wStpWvf;
	UINT				reg_data;
	UINT				rtn ;
	unsigned char		antName[D_RRH_ANT_MAX][8] = {"ANT.A", "ANT.B", "ANT.C", "ANT.D"};
	
	/*	停波要否を確認する	*/
	for(loop = 0 ; loop < E_EQS_STPWV_FCT_MAX ; loop++)
	{
		wSvInfDat = sv_inf[f_eqs_stpWvMng[f_eqs_StpWv_inhbitFlg][loop].svInfIdx] & f_eqs_stpWvMng[f_eqs_StpWv_inhbitFlg][loop].svInfmsk;
		/*	停波要因が有る場合	*/
		if(wSvInfDat != 0)
		{
			stpWvFlag[E_EQS_TCX_TDANTEN] |= f_eqs_stpWvMng[f_eqs_StpWv_inhbitFlg][loop].tddEn;
			stpWvFlag[E_EQS_CA_TXANTEN] |= f_eqs_stpWvMng[f_eqs_StpWv_inhbitFlg][loop].StpWv;
		}
	}
	if ( M_DU_RRHKIND_KIND_GET(f_comw_rrhKind) == D_RRH_EEP_DEV_KIND_5GDU_SUB6 )
	{
		/* 装置種別がSub6なら */
		loopNum = D_RRH_ANT_MAX ;
	}
	else
	{
		/* 装置種別がmmWなら */
		loopNum = D_RRH_ANT_MAX_MMW ;
	}
	/*	停波状態(TDD Enable)に変化がある場合	*/
	if(stpWvFlag[E_EQS_TCX_TDANTEN] != f_eqs_stpWvFlag[E_EQS_TCX_TDANTEN])
	{
		/*アンテナ番号でループ */
		for ( loop = 0 ; loop < loopNum ; loop++ )
		{
			/*	TDD Enble状態変化が有る場合	*/
			wStpWvf = stpWvFlag[E_EQS_TCX_TDANTEN] & stpWvChkFlg[loop];
			if( wStpWvf != (f_eqs_stpWvFlag[E_EQS_TCX_TDANTEN] & stpWvChkFlg[loop]))
			{
				/*	閉塞変化の場合	*/
				if(wStpWvf == stpWvChkFlg[loop])
				{
					/* アンテナ停波の設定 */
					f_eqs_tddEnChg_ntc(loop, D_EQS_TCX_TDANTEN_DIS , D_EQS_STPWV_ANT_DIS_ALL);
				}
				/*	TxOFF変化の場合	*/
				else if(wStpWvf == paoffChkFlg[loop])
				{
					/* PA OFFを設定 */
					f_eqs_tddEnChg_ntc(loop, D_EQS_TCX_TDANTEN_PAOFF ,D_EQS_STPWV_ANT_DIS_ALL);
				}
				/*	Enable変化の場合	*/
				else if(wStpWvf == D_EQS_STPWV_ANT_EN)
				{
					/* Enableを設定 */
					/* TDD enable解除時に、IQ停波状態もチェック */
					f_eqs_tddEnChg_ntc(loop, D_EQS_TCX_TDANTEN_EN , stpWvFlag[E_EQS_CA_TXANTEN] & stpWvChkFlg[loop] );
				}
				else
				{
					/* 設定値が不明な場合 */
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "stop wave code unnkown = [%d]0x%08x", loop, wStpWvf);
				}
			}
		}
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "TDD Enable change = 0x%08x->0x%08x", f_eqs_stpWvFlag[E_EQS_TCX_TDANTEN], stpWvFlag[E_EQS_TCX_TDANTEN]);
		
		/*	停波状態を更新する	*/
		f_eqs_stpWvFlag[E_EQS_TCX_TDANTEN] = stpWvFlag[E_EQS_TCX_TDANTEN];
	}
	/*	停波状態(STOP wave)に変化がある場合	*/
	if(stpWvFlag[E_EQS_CA_TXANTEN] != f_eqs_stpWvFlag[E_EQS_CA_TXANTEN])
	{
		/*アンテナ番号でループ */
		for ( loop = 0 ; loop < loopNum ; loop++ )
		{
			/*	停波状態変化が有る場合	*/
			wStpWvf = stpWvFlag[E_EQS_CA_TXANTEN] & stpWvChkFlg[loop];
			if( wStpWvf != (f_eqs_stpWvFlag[E_EQS_CA_TXANTEN] & stpWvChkFlg[loop]))
			{
				/*	停波解除変化の場合	*/
				if(wStpWvf == D_EQS_STPWV_ANT_EN)
				{
					/* アンテナ停波解除の設定 */
					reg_data = D_EQS_TXANTEN_ENABLE ;
				}
				/*	停波変化の場合	*/
				else
				{
					/* アンテナ停波の設定 */
					reg_data = D_EQS_TXANTEN_DISABLE ;
				}
				/*	IQ停波を設定	*/
				rtn = BPF_HM_DEVC_REG_WRITE ( D_RRH_LOG_REG_LV_WRITE , M_DU_REG_CA_TXANTEN(loop) , &reg_data );
				if ( rtn != BPF_HM_DEVC_COMPLETE)
				{
					/* レジスタライト失敗なら */
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "%5s CA_TXANTEN write error, errcode = %d data = 0x%08x", antName[loop], rtn, reg_data);
				}else{
					if(wStpWvf == D_EQS_STPWV_ANT_EN){
						/* IQ停波解除時に、TDD enable状態もチェック */
						f_eqs_tddEnChg_ntc( loop, D_EQS_TCX_TDANTEN_DPDON ,stpWvFlag[E_EQS_TCX_TDANTEN] & stpWvChkFlg[loop] );
					}else{
						f_eqs_tddEnChg_ntc( loop, D_EQS_TCX_TDANTEN_DPDOFF ,D_EQS_STPWV_ANT_DIS_ALL);
					}
				}
			}
		}
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "STOP wave change = 0x%08x->0x%08x", f_eqs_stpWvFlag[E_EQS_CA_TXANTEN], stpWvFlag[E_EQS_CA_TXANTEN]);
		
		/*	停波状態を更新する	*/
		f_eqs_stpWvFlag[E_EQS_CA_TXANTEN] = stpWvFlag[E_EQS_CA_TXANTEN];
	}
	return;
}
/* @} */


/**
* @brief Hardware state Enable Notification send function
* @note TRA O-RAN ietf-hardware state Operationaly state Enable Ind factor MSI Send..\n
* @param w_sts_data [in] Status Data
* @return None
* @date 2019/02/26 KCN)Kamimoto 5GDU-LLS対応
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eqs_send_alarm_notif( UINT idx_fault_id , UINT wis_cleared )
{
	/* 変数宣言 */
	T_MPSW_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND		sndMsg ;

	/* プロセス */
	/* パラメータエリアの初期化 */
	memset( &sndMsg , D_RRH_ZERO , sizeof( T_MPSW_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND ));

	/* Notification通知情報の編集 */
	sndMsg.fault_id = f_eqs_fault_info[idx_fault_id].faultId ;
	sndMsg.fault_source = f_eqs_fault_info[idx_fault_id].fault_source ;
	sndMsg.name_num = f_eqs_fault_info[idx_fault_id].affect_cnt;
	if( f_eqs_fault_info[idx_fault_id].affect_cnt != D_RRH_ZERO )
	{
		sndMsg.affected_objects.name[0] = E_FAULT_SOURCE_LLSDU;
	}
	sndMsg.fault_severity = f_eqs_fault_info[idx_fault_id].fault_severity ;
	memcpy(	&sndMsg.fault_text , f_eqs_fault_info[idx_fault_id].fault_text ,f_eqs_fault_info[idx_fault_id].faulttxt_cnt ) ;

	sndMsg.is_cleared = wis_cleared ;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER , "send O_RAN_FM_ALARM_NOTIF_NTF_IND FID:%x Is_Clear : %x", sndMsg.fault_id , sndMsg.is_cleared ) ;
	/* MSI送信(Notification通知 */
	(VOID)f_com_MSIsend(	D_MSG_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND,							/* メッセージID									*/
							D_EQS_REQ_NOTIF,												/* リクエストID									*/
							sizeof(T_MPSW_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND),					/* 送信するデータサイズ							*/
							&sndMsg,														/* 送信データのポインタ							*/
							D_RRH_PROCQUE_PF ,												/* PFのプロセスID								*/
							D_SYS_THDQID_PF_EQS ) ;											/* pf_eqsのスレッドID							*/

	return ;
}


/**
* @brief Hardware state Enable Notification send function
* @note TRA O-RAN ietf-hardware state Operationaly state Enable Ind factor MSI Send..\n
* @param w_sts_data [in] Status Data
* @return None
* @date 2019/02/14 KCN)Kamimoto 5GDU-LLS対応
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eqs_enable_notif( UINT sts_data )
{
	/* 変数宣言 */
	T_MPSW_MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND			sndMsg ;

	/* プロセス */
	/* パラメータエリアの初期化 */
	memset( &sndMsg , D_RRH_ZERO , sizeof( T_MPSW_MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND ));

	f_eqs_enable_Edit( sts_data , &sndMsg.admin_state , &sndMsg.availability_state ) ;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER , "send IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND Admin State : %x Avail State : %x", sndMsg.admin_state , sndMsg.availability_state ) ;
	/* Notification通知を送信 */
	(VOID)f_com_MSIsend(	D_MSG_MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND ,			/* メッセージID									*/
							D_EQS_REQ_NOTIF ,												/* リクエストID									*/
							sizeof(T_MPSW_MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND) ,	/* 送信するデータサイズ							*/
							&sndMsg ,														/* 送信データのポインタ							*/
							D_RRH_PROCQUE_PF ,												/* PFのプロセスID								*/
							D_SYS_THDQID_PF_EQS ) ;											/* pf_eqsのスレッドID							*/

	return ;
}


/**
* @brief Hardware state Disable Notification send function
* @note TRA O-RAN ietf-hardware state Operationaly state Disable Ind factor MSI Send..\n
* @param w_sts_data [in] Status Data
* @return None
* @date 2019/02/14 KCN)Kamimoto 5GDU-LLS対応
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eqs_disable_notif( UINT sts_data )
{
	/* 変数宣言 */
	T_MPSW_MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND		sndMsg ;

	/* プロセス */
	/* パラメータエリアの初期化 */
	memset( &sndMsg , D_RRH_ZERO , sizeof( T_MPSW_MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND ));

	f_eqs_enable_Edit( sts_data , &sndMsg.admin_state , &sndMsg.availability_state ) ;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER , "send IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND Admin State : %x Avail State : %x", sndMsg.admin_state , sndMsg.availability_state ) ;
	/* Notification通知を送信 */
	(VOID)f_com_MSIsend(	D_MSG_MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND ,			/* メッセージID									*/
							D_EQS_REQ_NOTIF ,												/* リクエストID									*/
							sizeof(T_MPSW_MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND) ,	/* 送信するデータサイズ							*/
							&sndMsg ,														/* 送信データのポインタ							*/
							D_RRH_PROCQUE_PF ,												/* PFのプロセスID								*/
							D_SYS_THDQID_PF_EQS ) ;											/* pf_eqsのスレッドID							*/

	return ;
}


/**
* @brief Hardware state Change Notification send function
* @note TRA O-RAN ietf-hardware state Change Ind factor MSI Send..\n
* @param svInf1Dat [in] Status Data
* @return None
* @date 2019/02/14 KCN)Kamimoto 5GDU-LLS対応
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eqs_status_change_notif( UINT svInf1Dat )
{
	/* 変数宣言 */
	T_MPSW_MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND		sndMsg ;

	/* プロセス */
	/* パラメータエリアの初期化 */
	memset( &sndMsg , D_RRH_ZERO , sizeof( sndMsg ));
	
	/* Admin状態の読出しと設定 */
	if( (svInf1Dat & D_DU_SVCTL_BIT_ADM_LOCKED_ALL) != D_RRH_OFF )
	{
		/* Admin状態が"Lock"なら */
		sndMsg.admin_state = E_MPSW_ADMIN_STATE_LOCKED ;
	}
	else if( (svInf1Dat & D_DU_SVCTL_BIT_ADM_UNLOCKED_ALL) != D_RRH_OFF )
	{
		/* Admin状態が"Unlock"なら */
		sndMsg.admin_state = E_MPSW_ADMIN_STATE_UNLOCKED ;
	}
	else
	{
		/* Admin状態が"Lock"/"Unlock"以外なら */
		sndMsg.admin_state = E_MPSW_ADMIN_STATE_UNKNOWN ;
	}
	/* MSI送信通知(D_MSG_MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND) */
	/* Notification通知を送信 */
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER , "send IETF_HARDWARE_STATE_CHANGE_NTF_IND:Admin State : %x", sndMsg.admin_state ) ;
	(VOID)f_com_MSIsend(	D_MSG_MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND ,				/* メッセージID									*/
							D_EQS_REQ_NOTIF ,											/* リクエストID									*/
							sizeof(sndMsg) ,											/* 送信するデータサイズ							*/
							&sndMsg ,													/* 送信データのポインタ							*/
							D_RRH_PROCQUE_PF ,											/* PFのプロセスID								*/
							D_SYS_THDQID_PF_EQS ) ;										/* pf_eqsのスレッドID							*/

	return ;
}

/* @} */


