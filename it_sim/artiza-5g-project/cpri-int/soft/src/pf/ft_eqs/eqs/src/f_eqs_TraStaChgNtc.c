/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_TraStaChgNtc.c
 *  @brief  TRA card status change interruption handle function
 *  @date   2008/07/18 FFCS)Wangjuan create
 * 
 *  @date   2018/08/21 KCN)Kamimoto create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_EQS
* @{ */

#include "f_eqs_header.h"			/* RE監視タスクヘッダファイル			*/

/* @{ */
/**
* @brief TRA card status change interruption handle function
* @note TRA card status change factor judge,table update, and status change handle.\n
* @param a_trainf_p [in] the buffer address pointer of received message
* @return None
* @date 2018/08/23 KCN)Kamimoto 5GDU-LLS対応
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
/* @{ */

/* @{ */
/**
* @brief TRA card status change interruption handle function
* @note TRA card status change factor judge,table update, and status change handle.\n
* @param a_trainf_p [in] the buffer address pointer of received message
* @return None
* @date 2019/02/25 KCN)Kamimoto 5GDU-LLS対応
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eqs_Set_LED( VOID )
{
	/* 変数宣言 */
	UINT	kind, count;
	UINT	ledSta[E_EQS_LED_MAX];

	/*	装置種別がsub6の場合	*/
	if (M_DU_RRHKIND_KIND_GET(f_comw_rrhKind) == D_RRH_EEP_DEV_KIND_5GDU_SUB6 )
	{
		/*	sub6装置	*/
		kind = E_EQS_DEVKIND_SB6;
	}
	/*	装置種別がmmWの場合	*/
	else
	{
		/*	mmW装置	*/
		kind = E_EQS_DEVKIND_MMW;
	}
	/**********************/
	/* LED1状態表示の設定 */
	/**********************/
	/* Critical発生中か？ */
	if((f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_03] & D_DU_SVCTL_BIT_FLV_CRITICAL) == D_DU_SVCTL_BIT_FLV_CRITICAL)
	{
		/* 最高優先のCritical発生中(赤点灯) */
		ledSta[E_EQS_LED1] = D_API_EQS_LEDSTATE_ALM_CRITICAL;
	}
	/* Major発生中か？ */
	else if((f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_03] & D_DU_SVCTL_BIT_FLV_MAJOR) == D_DU_SVCTL_BIT_FLV_MAJOR)
	{
		/* 次優先のMajor発生中(赤点滅) */
		ledSta[E_EQS_LED1] = D_API_EQS_LEDSTATE_ALM_MAJOR;
	}
	/* Admin状態がLock中か? */
	else if((f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_01] & D_DU_SVCTL_BIT_ADM_LOCKED_ALL) != D_RRH_OFF)
	{
		/* Admin State = Lock中(緑赤交互) */
		ledSta[E_EQS_LED1] = D_API_EQS_LEDSTATE_ADMINLOCK;
	}
	/* Ant.A - D 使用中か？(緑点灯) */
	else if((f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_02] & f_eqs_useChkBit[kind]) != D_DU_SVCTL_BIT_OFF)
	{
		/* Use発生中 */
		ledSta[E_EQS_LED1] = D_API_EQS_LEDSTATE_USE;
	}
	else if((f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_01] & D_DU_SVCTL_BIT_OPE_ENABLE) == D_DU_SVCTL_BIT_OPE_ENABLE)
	{
		/* Operation発生(緑点滅) */
		ledSta[E_EQS_LED1] = D_API_EQS_LEDSTATE_OPERATION;
	}
	else
	{
		/* 上記以外(消灯) */
		ledSta[E_EQS_LED1] = D_API_EQS_LEDSTATE_POWERON;
	}

	/**********************/
	/* LED2状態表示の設定 */
	/**********************/
	/* ローディング中状態か？ */
	if((f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_15] & D_DU_SVCTL_FL_DWNLD) == D_DU_SVCTL_FL_DWNLD)
	{
		/* ローディング中状態(橙点滅) */
		ledSta[E_EQS_LED2] = D_API_EQS_LEDSTATE_MAINT_FLDL;
	}
	/* Port1 L1断発生中か？ */
	else if((f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_06] & D_DU_ALMBIT_L1_LDOWN_FH1) == D_DU_ALMBIT_L1_LDOWN_FH1)
	{
		/* Busy状態(橙点灯) */
		ledSta[E_EQS_LED2] = D_API_EQS_LEDSTATE_BUSY;
	}
	/* Minor発生中か？ */
	else if((f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_03] & D_DU_SVCTL_BIT_FLV_MINOR) == D_DU_SVCTL_BIT_FLV_MINOR)
	{
		/* Minor発生中(橙緑交互) */
		ledSta[E_EQS_LED2] = D_API_EQS_LEDSTATE_ALM_MINOR;
	}
	else
	{
		/* 上記以外(緑点灯) */
		ledSta[E_EQS_LED2] = D_API_EQS_LEDSTATE_POWERON;
	}

	/**********************/
	/* LED3, LED4, LED5状態表示の設定 */
	/**********************/
	for(count = E_EQS_LED3; count < E_EQS_LED_MAX; count++)
	{
		/* ローディング中状態か？ */
		if((f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_15] & D_DU_SVCTL_FL_DWNLD) == D_DU_SVCTL_FL_DWNLD)
		{
			/* ローディング中状態(橙点滅) */
			ledSta[count] = D_API_EQS_LEDSTATE_MAINT_FLDL;
		}
		/* Port1 L1断発生中か？ */
		else if((f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_06] & D_DU_ALMBIT_L1_LDOWN_FH1) == D_DU_ALMBIT_L1_LDOWN_FH1)
		{
			/* Busy状態(橙点灯) */
			ledSta[count] = D_API_EQS_LEDSTATE_BUSY;
		}
		/* Minor発生中か？ */
		else if((f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_03] & D_DU_SVCTL_BIT_FLV_MINOR) == D_DU_SVCTL_BIT_FLV_MINOR)
		{
			/* Minor発生中(橙緑交互) */
			ledSta[count] = D_API_EQS_LEDSTATE_ALM_MINOR;
		}
		else
		{
			/* 上記以外(緑点灯) */
			ledSta[count] = D_API_EQS_LEDSTATE_POWERON;
		}
	}

	f_eqs_LED_cntl(ledSta);
	return ;
}
/* @} */


/* @{ */
/**
* @brief usage 状態設定
* @note usage 状態設定
* @date 2019/03/12 FJT)Taniguchi 5GDU-LLS対応
* @warning	N/A
* @FeatureID	5GDU-002-003
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eqs_usage_set( UINT sv_sts_old_1, UINT sv_sts_new_1, UINT *usage )
{
	UINT	usage_set;
	UINT	usage_val;
	UINT	kind;
	UINT	useOld, useNew;
	
	/*	装置種別がsub6の場合	*/
	if (M_DU_RRHKIND_KIND_GET(f_comw_rrhKind) == D_RRH_EEP_DEV_KIND_5GDU_SUB6 )
	{
		/*	sub6装置	*/
		kind = E_EQS_DEVKIND_SB6;
	}
	/*	装置種別がmmWの場合	*/
	else
	{
		/*	mmW装置	*/
		kind = E_EQS_DEVKIND_MMW;
	}
	/*	有効なUSEbitのみ取得する	*/
	useOld = sv_sts_old_1 & f_eqs_useChkBit[kind];
	useNew = sv_sts_new_1 & f_eqs_useChkBit[kind];
	
	/*	ANT使用状況に変化がない場合	*//*	SLP状態はマスクする	*/
	if(useOld == useNew)
	{
		return;	
	}
	/*	以降は、ANT使用状況に変化がある場合	*/
	
	/* IDLE(全bitUSEOFF)へ変化した場合 */
	if(useNew == D_DU_SVCTL_BIT_NON_USE_ALL)
	{
		usage_set = D_RRH_ON;
		usage_val = D_DU_SVCTL_ON_USA_IDLE;
		*usage = D_DU_SVCTL_BIT_USA_IDLE;
	}
	/* SUBY(全bitUSEON)へ変化した場合 */
	else if(useNew == f_eqs_useChkBit[kind])
	{
		usage_set = D_RRH_ON;
		usage_val = D_DU_SVCTL_ON_USA_BUSY;
		*usage = D_DU_SVCTL_BIT_USA_BUSY;
	}
	/* IDLE(全bitUSEOFF)またはBUSY(全bitUSEON)から変化した場合 */
	else if((useOld == D_DU_SVCTL_BIT_NON_USE_ALL) || (useOld == f_eqs_useChkBit[kind]))
	{
		usage_set = D_RRH_ON;
		usage_val = D_DU_SVCTL_ON_USA_ACTIVE;
		*usage = D_DU_SVCTL_BIT_USA_ACTIVE;
	}
	/*	ACTIVEからACTIVEへの変化	*/
	else
	{
		usage_set = D_RRH_OFF;
		usage_val = D_DU_SVCTL_ON_USA_ACTIVE;
	}
	if(usage_set == D_RRH_ON){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Usage State change to 0x%x",usage_val);
		f_com_SVCtl( E_DU_SVCTL_OPE_USA , D_DU_SVCTL_CHG , usage_val );
	}
	return;
}
/* @} */

/* @{ */
/**
* @brief TRA card status change interruption handle function
* @note TRA card status change factor judge,table update, and status change handle.\n
* @param a_trainf_p [in] the buffer address pointer of received message
* @return None
* @date 2018/08/23 KCN)Kamimoto 5GDU-LLS対応
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eqs_TraStaChgNtc( T_SYS_EQS_ALM_INFO *a_trainf_p )
{
	/* 変数宣言 */
	UINT				loop ;
	UINT				usage;
	UINT				wSvInf, wSvInfOld;
	UCHAR				paOnOff;
	
	/********************/
	/*	SVINF履歴ログ	*/
	/********************/
	/*	Control code = IRQ */
	f_comw_svStateLog.chgType = D_SYS_SVSTATE_IRQ;

	/*	SV-Bus状態変化内容を更新 */
	for(loop = 0; loop < D_DU_SVINF_NUM; loop++)
	{
		f_comw_svStateLog.svState[loop] = a_trainf_p->sv_inf[loop];
	}
	/*	状態変化履歴の実行 */
	f_com_SVLogSet(&f_comw_svStateLog);

	/****************************/
	/*	availability State設定	*/
	/****************************/
	/*	Critical発生の場合	*/
	if((a_trainf_p->sv_inf[E_EQS_SVINF_03] & D_DU_SVCTL_BIT_FLV_CRITICAL) != 0)
	{
		/*	faulty以外の場合	*/
		if((a_trainf_p->sv_inf[E_EQS_SVINF_01] & D_DU_SVCTL_BIT_AVL_FAULTY) == 0)
		{
			/*	availability Stateにfaultyを設定	*/
			f_com_SVCtl(E_DU_SVCTL_OPE_AVL, D_DU_SVCTL_CHG, D_DU_SVCTL_ON_AVL_FAULTY);
			/*	新状態もfaultyに更新	*/
			a_trainf_p->sv_inf[E_EQS_SVINF_01] &= ~D_DU_SVCTL_BIT_AVL_ALL;
			a_trainf_p->sv_inf[E_EQS_SVINF_01] |= D_DU_SVCTL_BIT_AVL_FAULTY;
			
			/*	FMのnotification開始未の場合	*/
			/*	notification開始後は、f_eqs_faultChgにてシグナル送信	*/
			if(f_eqs_alm_start_data != D_EQS_NTF_START_RUN)
			{
				/****************/
				/* ALM履歴取得  */
				/****************/
				/* INITプロセスへSignal送信 */
				BPF_RM_SVRM_SIGNAL_SEND(D_RRH_PROCID_INI , SIGUSR1 , D_RRH_SIGUSR_OPT_RESORCE_ALM_NTC) ;
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "send SIGNAL of resorce ALM(svif04:0x%08x->0x%08x)", 
										f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_04], a_trainf_p->sv_inf[E_EQS_SVINF_04]);
				/*	最後にSignal送信した時のFLTIDを保持	*/
				sigFltId = a_trainf_p->sv_inf[E_EQS_SVINF_04];
			}
		}
	}
	/*	Critical未発生 かつ Major発生の場合	*/
	else if((a_trainf_p->sv_inf[E_EQS_SVINF_03] & D_DU_SVCTL_BIT_FLV_MAJOR) != 0)
	{
		/*	degraded以外の場合	*/
		if((a_trainf_p->sv_inf[E_EQS_SVINF_01] & D_DU_SVCTL_BIT_AVL_DEGRADED) == 0)
		{
			/*	availability Stateにdegradedを設定	*/
			f_com_SVCtl(E_DU_SVCTL_OPE_AVL, D_DU_SVCTL_CHG, D_DU_SVCTL_ON_AVL_DEGRADED);
			/*	新状態もdegradedに更新	*/
			a_trainf_p->sv_inf[E_EQS_SVINF_01] &= ~D_DU_SVCTL_BIT_AVL_ALL;
			a_trainf_p->sv_inf[E_EQS_SVINF_01] |= D_DU_SVCTL_BIT_AVL_DEGRADED;
		}
	}
	/*	fault未発生またはMinor(ERR)のみの場合	*/
	else
	{
		/*	normal以外の場合	*/
		if((a_trainf_p->sv_inf[E_EQS_SVINF_01] & D_DU_SVCTL_BIT_AVL_NORMAL) == 0)
		{
			/*	availability Stateにnormalを設定	*/
			f_com_SVCtl(E_DU_SVCTL_OPE_AVL, D_DU_SVCTL_CHG, D_DU_SVCTL_ON_AVL_NORMAL);
			/*	新状態もnormalに更新	*/
			a_trainf_p->sv_inf[E_EQS_SVINF_01] &= ~D_DU_SVCTL_BIT_AVL_ALL;
			a_trainf_p->sv_inf[E_EQS_SVINF_01] |= D_DU_SVCTL_BIT_AVL_NORMAL;
		}
	}
	/********************************/
	/*	MajorによるALMログ履歴取得	*/
	/********************************/
	/*	FMのnotification開始未の場合	*/
	/*	notification開始後は、f_eqs_faultChgにてシグナル送信	*/
	if(f_eqs_alm_start_data != D_EQS_NTF_START_RUN)
	{
		/*	PTP Unlockを除くMajor faultがOFF->ONに変化した場合	*/
		if( ((f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_04] & D_EQS_MAJOR_OUTOFPTP_FLTBIT) == 0) &&
			((a_trainf_p->sv_inf[E_EQS_SVINF_04] & D_EQS_MAJOR_OUTOFPTP_FLTBIT) != 0) )
			
		{
			/****************/
			/* ALM履歴取得  */
			/****************/
			/* INITプロセスへSignal送信 */
			BPF_RM_SVRM_SIGNAL_SEND(D_RRH_PROCID_INI , SIGUSR1 , D_RRH_SIGUSR_OPT_RESORCE_ALM_NTC) ;
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "send SIGNAL of resorce ALM(svif04:0x%08x->0x%08x)", 
									f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_04], a_trainf_p->sv_inf[E_EQS_SVINF_04]);
			/*	最後にSignal送信した時のFLTIDを保持	*/
			sigFltId = a_trainf_p->sv_inf[E_EQS_SVINF_04];
		}
	}
	/* Usage state setting */
	usage = (a_trainf_p->sv_inf[E_EQS_SVINF_01] & D_DU_SVCTL_BIT_USA_ALL);
	f_eqs_usage_set( f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_02], a_trainf_p->sv_inf[E_EQS_SVINF_02], &usage);
	/*	新状態のUsegeも更新	*/
	a_trainf_p->sv_inf[E_EQS_SVINF_01] &= ~D_DU_SVCTL_BIT_USA_ALL;
	a_trainf_p->sv_inf[E_EQS_SVINF_01] |= usage;
	
	/********************/
	/*	状態変化通知	*/
	/********************/
	/*	DU状態(SVINF1)に変化がある場合	*//*	admin stateはremoteとLocalのOR値を渡す	*/
	wSvInf = ((a_trainf_p->sv_inf[E_EQS_SVINF_01] & ~D_DU_SVCTL_BIT_ADM_LO_ALL) | 
				((a_trainf_p->sv_inf[E_EQS_SVINF_01] & D_DU_SVCTL_BIT_ADM_LO_ALL)>>24));
	wSvInfOld = ((f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_01] & ~D_DU_SVCTL_BIT_ADM_LO_ALL) | 
				((f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_01] & D_DU_SVCTL_BIT_ADM_LO_ALL)>>24));
	if(wSvInf != wSvInfOld)
	{
		/*	最終変化時刻を取得する	*/
		f_com_getSysTim_notMs( (T_RRH_SYSTEM_TIME *)&f_eqsw_last_chg) ;
		
		/*	状態変更Notification	*/
		f_eqs_stsChgProc( wSvInf );
	}
	/********************/
	/*	fault変化通知	*/
	/********************/
	f_eqs_faultChg( a_trainf_p->sv_inf );
	
	/*	Tx OFF変化の場合	*/
	wSvInf = a_trainf_p->sv_inf[E_EQS_SVINF_14] & D_DU_ALMBIT_TX_OFF;
	if( wSvInf != (f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_14] & D_DU_ALMBIT_TX_OFF) )
	{
		/*	Tx OFFがOFF	*/
		if(wSvInf == 0)
		{
			/*	PA ON	*/
			paOnOff = 0xff;
		}
		/*	Tx OFFがON	*/
		else
		{
			/*	PA OFF	*/
			paOnOff = 0x0;
		}
		/*	EEPROMに格納する	*/
		(VOID)BPF_HM_DEVC_EEPROM_WRITE( D_DU_EEP_PAONOFF, &paOnOff );
	}
	/***********************************/
	/* アラーム停波/停波解除処理の設定 */
	/***********************************/
	f_eqs_stpWvProc( a_trainf_p->sv_inf );
	
	/* アラーム要因をnewstaにコピー */
	for( loop = 0;loop < D_DU_SVINF_NUM; loop++)
	{
		f_eqsw_tra_sv_newsta.sv_inf[loop] = a_trainf_p->sv_inf[loop] ;
	}

	/* LED設定優先度決定処理 */
	f_eqs_Set_LED() ;

	return ;
}
/* @} */


