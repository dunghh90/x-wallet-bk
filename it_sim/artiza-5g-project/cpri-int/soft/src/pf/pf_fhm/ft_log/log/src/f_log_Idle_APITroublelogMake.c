/*!
 * @skip		$ld:$
 * @file		f_log_Idle_APITroublelogMake.c
 * @brief		ログスレッド 運用状態_障害ログファイル作成要求受信処理
 * @author		ALPHA)yokoyama
 * @date 		2013/10/29 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_LOG_LOG
 * @{
 */

#include "f_log_inc.h"
#include "f_com_inc.h"
#include <signal.h>

/*!
 * @brief		関数機能概要:障害ログファイル作成要求受信時処理を行う
 * @note		関数処理内容.
 *				-# 運用ログ出力先フォルダ配下をクリアする
 *				-# INITプロセスへSignal（運用ログ出力要求）を送信する
 *				-# 状態を運用ログ出力完了待ちへ遷移させる
 * @param		*msgP			[in]	受信メッセージ
 * @retval		D_RRH_OK		0:正常終了
 * @retval		D_RRH_NG		1:異常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-003-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2013/10/29 ALPHA)横山 Create
 * @date 2014/12/23 ALPHA)横山 modify for ppcEL
 */
INT f_log_Idle_APITroublelogMake(VOID* msgP)
{
	T_API_LOG_TROUBLELOG_GET*				apiMsg;
	INT										ret;
	UINT									sysKind;
	UINT									sigOpt[D_SYS_MAX_SYSNUM] = { D_RRH_SIGUSR_OPT_RUNLOG_REQ_3G, D_RRH_SIGUSR_OPT_RUNLOG_REQ };
	CHAR									a_shcmd[64];
	
	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_log_Idle_APITroublelogMake] ENTER" );
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif
	
	apiMsg = ( T_API_LOG_TROUBLELOG_GET* )msgP;
	sysKind = apiMsg->head.uiSignalkind;
	
	/* API発行元QueueID/ThreadIDを保持する */
	f_logw_rspMngTbl[sysKind][E_LOG_RSP_API_TLOGGET].srcPID = apiMsg->head.uiSrcPQueueID;
	f_logw_rspMngTbl[sysKind][E_LOG_RSP_API_TLOGGET].srcTID = apiMsg->head.uiSrcTQueueID;
	
	/* APIのパラメータを保持する	*/
	memcpy( (VOID *)&f_logw_troublelog_get_info, 
			(VOID *)&(apiMsg->info), 
			sizeof(T_API_LOG_TROUBLELOG_GET_INFO) );
	
	/****************************************/
	/* /var/log/sendlog配下をクリアする		*/
	/****************************************/
	memset( a_shcmd, 0x00, sizeof(a_shcmd) );
	snprintf( a_shcmd, sizeof(a_shcmd), "rm -rf %s/* > /dev/null 2>&1", f_comr_runLogPath[sysKind] );
	ret = system(a_shcmd);
	
	if( D_RRH_MNG == ret )
	{
		cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "rm failed." );
	}
	
	/* INITプロセスへSignal(運用ログ出力要求)を送信する	*/
	BPF_RM_SVRM_SIGNAL_SEND(	D_RRH_PROCID_INI,				/* 送信先プロセスID			*/
								SIGUSR1,						/* 送信するSignal			*/
								sigOpt[sysKind] );				/* オプション				*/
	
	/* スレッド状態遷移(運用ログ出力完了待ち) */
	f_logw_thdState[ sysKind ] = D_LOG_THD_STS_WAITLOG;
	
	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_log_Idle_APITroublelogMake] RETURN(%d)",sysKind );
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
	
	return D_RRH_OK;
}

/* @} */
