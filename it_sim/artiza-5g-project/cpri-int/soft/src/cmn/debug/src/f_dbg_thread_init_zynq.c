/*!
 * @skip   $Id$
 * @file   f_cmu_dbg_init_zynq.c
 * @brief  デバッグスレッド初期化処理
 * @date   2013/11/22 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */


#include "f_dbg_thread_inc.h"
extern	int BPF_RM_SVRM_INITEND_IND(bpf_rm_svrm_result result);



/*!
 * @brief 関数機能概要:デバッグスレッド　初期化処理
 * @note  関数処理内容.
 *       -#初期化完了通知送信
 * @param  uiProcId		[in]UINT	自プロセスID
 * @return INT
 * @retval 送信先	(-1:エラー)
 * @date 2013/11/25 ALPHA)宮崎 create
 */

UINT f_dbg_thread_init( T_DBGTHREAD_EXEC_INFO *execParam )
{
	T_RRH_HEAD					*sndMsg	;			/* 送信メッセージ(初期化完了通知)	*/
	int							endcd	;
	int							errcd	;

	endcd = BPF_RU_IPCM_PROCMSG_ADDRGET(E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
										sizeof(T_RRH_HEAD),						/* MessageSize						*/
										(VOID **)&sndMsg,						/* MessageBuffer					*/
										&errcd );								/* ErrorCode						*/
	if( endcd != D_RRH_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " ADDRGET NG %d ", endcd );
		return D_RRH_NG;
	}

	/************************************************************/
	/* 初期化完了通知送信設定									*/
	/************************************************************/
	/* 送信MSG作成 */
	sndMsg->uiEventNo			= D_DBG_INIT_COMP_IND;				/* イベント番号						*/
	sndMsg->uiSignalkind		= 0;								/* SignalKind(初期化完了通知は不要)	*/
	sndMsg->uiDstPQueueID		= execParam->uiProcQID;				/* 最終送信先Process Queue ID		*/
	sndMsg->uiDstTQueueID		= execParam->uiMainThradQID;		/* 最終送信先Thread  Queue ID		*/
	sndMsg->uiSrcPQueueID		= execParam->uiProcQID;				/* 送信元    Process Queue ID		*/
	sndMsg->uiSrcTQueueID		= execParam->uiDbgThreadQID;		/* 送信元    Thread  Queue ID		*/
	sndMsg->uiLength			= sizeof(T_RRH_HEAD);				/* Length							*/

	/************************************************************/
	/* 初期化完了通知送信										*/
	/************************************************************/
	endcd = BPF_RU_IPCM_MSGQ_SEND(
			sndMsg->uiDstTQueueID,									/* 送信MSGキューID					*/
				(CHAR *)sndMsg												/* 送信MSGポインタ					*/
			);

	if( endcd != D_RRH_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " MSGQ_SEND NG %d ", endcd );
		return D_RRH_NG;
	}
	return D_RRH_OK;
}



/* @} */

