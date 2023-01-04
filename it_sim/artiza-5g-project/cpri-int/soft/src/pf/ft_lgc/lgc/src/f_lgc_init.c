/*!
 * @skip		$ld:$
 * @file		f_lgc_init.c
 * @brief		ログスレッド 初期化処理
 * @author		ALPHA)yokoyama
 * @date		2010/05/24 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2010-2013
 */

/*!
 * @addtogroup RRH_PF_LGC_LGC
 * @{
 */

#include "f_lgc_inc.h"

/*!
 * @brief		関数機能概要:ログスレッドの初期化処理を行う
 * @note		関数処理内容.
 *				-# FrashROM Write(QSPI)スレッドにFlash Erase要求を送信する
 *				-# 初期化スレッドに初期化完了通知(Init completion notice)を送信する
 *				-# ログ情報退避用タイマを起動する
 *				-# スレッド状態を"通常状態"に遷移させる
 * @param		*msgP			[in]	受信メッセージ
 * @retval		D_RRH_OK		0:正常終了
 * @retval		D_RRH_NG		1:異常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2010/05/24 ALPHA)横山 Create
 * @date 2013/03/28 ALPHA)松延 B25 対処
 * @date 2013/10/29 ALPHA)横山 docomo ZYNQ化 & 共通化対応
 * @date 2018/08/24 KCN)若水 カスケードを削除 退避時間を1時間に1回に変更
 */
INT f_lgc_init(VOID* msgP)
{
	T_SYS_INIT_COMP_NTC			*sndMsg;										/* 送信メッセージ(初期化完了通知)	*/
	T_RRH_LOG_LEVEL_MNG*		tLogLevelMng;
	INT							ret;
	INT							errcd;
	
	f_lgcw_assert_p = f_cmn_com_assert_th_create(D_RRH_LOG_AST_DEFAULT,"lgc thread");
	cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_lgc_init] ENTER" );
	

	/************************************************************/
	/* Log Levelを取得											*/
	/************************************************************/
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_LOGLEVEL_MNG,
		(VOID **)&tLogLevelMng,
		&errcd );
	
	/* 退避時間を24時間(24回*1時間)に設定する	*/
	/* コマンドから変更可能とする				*/
	tLogLevelMng->cycleLogCount = 24;
	tLogLevelMng->cycleLogCount_cur = 0;
	if(f_cmw_tra_inv_tbl->almreset_flag[0] != D_RRH_OFF){
		tLogLevelMng->selfResetFlg= D_RRH_ON;
	}else{
		tLogLevelMng->selfResetFlg= D_RRH_OFF;
	}
	tLogLevelMng->selfResetCount = D_RRH_ALM_SELFRESET_TIMECOUNT;
	tLogLevelMng->selfResetCount_cur = 0;
	/****************************************************************************************************************/
	/* EventLog操作要求(退避ログのClear or Write)送信																*/
	/****************************************************************************************************************/
	f_lgc_send_EventLogReq( D_SYS_FLASH_ERASE );
	
	/****************************************************************************************************************/
	/* 初期化完了通知送信																							*/
	/****************************************************************************************************************/
	/* バッファ取得 */
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
										sizeof(T_SYS_INIT_COMP_NTC),			/* MessageSize						*/
										(VOID **)&sndMsg,						/* MessageBuffer					*/
										&errcd );								/* ErrorCode						*/
	
	if( ret != 0 )
	{
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ERROR, "[f_lgc_init] BUFFGET NG %d", ret );
		return D_RRH_NG;
	}
	
	/* 送信MSG作成 */
	sndMsg->head.uiEventNo		= D_SYS_MSGID_INIT_COMP_NTC;					/* メッセージID						*/
	sndMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;								/* 返信先PQID						*/
	sndMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_MAIN;							/* 送信先TQID						*/
	sndMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;								/* 送信元PQID						*/
	sndMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_LGC;							/* 送信元TQID						*/
	
	ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_MAIN,						/* 送信MSGキューID					*/
									(VOID *)sndMsg );							/* 送信MSGポインタ					*/
	
	if( ret != 0 )
	{
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ERROR, "[f_lgc_init] SEND NG %d", ret );
		return D_RRH_NG;
	}
	
	/* ログ退避用タイマを起動する	*/
	ret = BPF_RU_HRTM_REGISTER( BPF_RU_HRTM_REG_PERIODIC,						/* Timer種別						*/
								E_RRH_TIMID_LGC_SHELTER,						/* TimerID							*/
								D_LGC_SHELTER_TIMER,							/* Timer値							*/
								D_SYS_MSGID_CYCLIC_LOG_SHELTER_TO,				/* TimeOut時通知Event番号			*/
								D_SYS_THDQID_PF_LGC,							/* TimeOut時送信先Queue番号			*/
								&errcd );
	
	if( ret != 0 )
	{
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ERROR, "[f_lgc_init] TIMER NG %d", ret );
		return D_RRH_NG;
	}
	
	/* スレッド状態遷移(運用状態) */
	f_lgcw_thdState = D_LGC_THD_STS_ACT;
	
	cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_lgc_init] RETURN" );
	
	return D_RRH_OK;
}

/* @} */
