/*!
 * @skip  $ld:$
 * @file  f_cmn_com_log.c
 * @brief 各プロセスでのログファイル生成処理
 * @date  2016/10/04 ALPHA) fujiiy Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2016
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_cmn_com_inc.h"

/*!
 * @brief 関数機能概要:ログ作成スレッド生成処理
 * @note  関数処理内容.
 *       -#  スレッド生成
 * @param  procName      [in]CHAR *       プロセス名称
 * @param  logCreateFunc [in]VOID *       ログ生成関数
 * @param  msgQid        [in]UINT         メッセージQID
 * @return VOID
 * @retval none
 * @warning   N/A
 * @FeatureID N/A
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date  2016/10/04 ALPHA) fujiiy Create
 */
VOID f_cmn_com_log_threadCreate( CHAR * procName,  VOID (*logCreateFunc)(FILE *), UINT msgQid )
{
	unsigned long int					thrdID;
	INT						ret;
	INT						errcd;
	T_CMN_LOG_SAVE_INFO		logInfo;
	
	memset( &logInfo, 0x00, sizeof(logInfo) );
	
	/* スレッド生成時のINパラ情報作成 */
	strncpy( logInfo.procName, procName, sizeof(logInfo.procName) );
	logInfo.logSaveFunc = (VOID (*)(FILE *))logCreateFunc;
	logInfo.msgQid      = msgQid;
	
	/* スレッド生成 */
	ret = BPF_RU_ITCM_THREAD_CREATE((unsigned long int *)&thrdID,
									BPF_RU_ITCM_SCHED_OTHER,
									0,								/* priority   */
									D_RRH_STACKSIZE_DEFAULT,		/* stack size */
									f_cmn_com_log_main,
									(void *)&logInfo,
									&errcd);
	
	if(ret != BPF_RU_ITCM_OK)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF thread create NG! ret=%d(%d,%s)", ret, errcd, logInfo.procName );
	}
    
    return;
}

/*!
 * @brief 関数機能概要:ログ作成要求送信
 * @note  関数処理内容.
 *       -# ログ作成要求送信
 * @param  logKind  [in]UINT						ログ種別
 * @param  msgQid   [in]UINT						メッセージQID
 * @return VOID
 * @retval none
 * @warning   N/A
 * @FeatureID N/A
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date  2016/10/04 ALPHA) fujiiy Create
 */
VOID f_cmn_com_log_create_send( UINT logKind, UINT msgQid )
{
	INT						ret;
	T_CMN_LOG_CREATE_REQ	*logCreReq;
	INT						errcd;
	
	/* 通信用共有メモリを取得する	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(E_BPF_RU_IPCM_BUFF_KIND_TASK,
										sizeof(T_CMN_LOG_CREATE_REQ),
										(void**)&logCreReq,
										&errcd );
	if( ret != BPF_RU_IPCM_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " ADDRGET NG(%d,%d) ", ret, errcd );
		return;
	}
	
	/* Header部設定	*/
	logCreReq->thdif.uiEventNo		= CMD_TSKIF_LOG_CREATE_REQ;
	logCreReq->thdif.uiLength		= sizeof(T_CMN_LOG_CREATE_REQ);
	logCreReq->logKind				= logKind;
	
	/* メッセージ送信する */
	ret = BPF_RU_IPCM_MSGQ_SEND(msgQid,
								(void*)logCreReq );
	
	if( ret != BPF_RU_IPCM_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " MSGQ_SEND NG(%d)", ret );
	}
	
	return;
}

/*!
 * @brief 関数機能概要:ログ作成スレッドメイン
 * @note  関数処理内容.
 *       -# スレッド間メッセージ待ち
 *       -# ログファイル作成関数call
 * @param  logInfo  [in]T_CMN_LOG_SAVE_INFO *       ログ生成関数
 * @return VOID
 * @retval none
 * @warning   N/A
 * @FeatureID N/A
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date  2016/10/04 ALPHA) fujiiy Create
 */
VOID f_cmn_com_log_main( T_CMN_LOG_SAVE_INFO * logInfo )
{
	CHAR					thdName[16];
	T_CMN_LOG_SAVE_INFO		a_logInfo;
	FILE					*fp;
	UCHAR					output_file[256];
	T_CMN_LOG_CREATE_REQ	*logCreReq;
	VOID					*rcvMsg	= NULL;					/* 送信メッセージ(初期化完了通知)				*/
	UINT 					msgSize = 0;
	
	memcpy( &a_logInfo, logInfo, sizeof(a_logInfo) );
	
	snprintf( thdName, sizeof(thdName), "%s_lgmk", a_logInfo.procName );
	prctl(PR_SET_NAME, thdName, 0, 0, 0);
	
	while(1)
	{
		BPF_RU_IPCM_MSGQ_RECV(	a_logInfo.msgQid,				/* 受信MSGキューID							*/
								0,								/* 応答待ち時間								*/
								(VOID **)&rcvMsg,				/* メッセージ受信バッファ					*/
								&msgSize);						/* 最大受信バッファサイズ					*/
		
		logCreReq = (T_CMN_LOG_CREATE_REQ *)rcvMsg;
		
		if( CMD_TSKIF_LOG_CREATE_REQ == logCreReq->thdif.uiEventNo )
		{
			/* ログファイルオープン */
			fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( a_logInfo.procName, logCreReq->logKind, output_file );
			if( fp != NULL )
			{
				/* ログファイル作成関数call */
				(VOID)(*(a_logInfo.logSaveFunc))( fp );
				
				(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
			}
			else
			{
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FILE OPEN NG(%d)", logCreReq->logKind );
			}
		}
		else
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Rcv Msg NG(0x%04x,%s)", logCreReq->thdif.uiEventNo, a_logInfo.procName );
		}
		
		/* 受信バッファを解放する */
		BPF_RU_IPCM_PROCMSG_ADDRFREE((VOID*)rcvMsg);
	}
}

/* @} */
