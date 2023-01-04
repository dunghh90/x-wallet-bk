/*!
 * @skip   $Id$
 * @file   f_dbg_cmd_send_exec_cmd_req.c
 * @brief  デバッグコマンド　コマンド実行要求送信
 * @date   2013/11/22 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */

#include	"f_dbg_cmd_inc.h"
extern UINT f_dbg_thread_exec_cmd(UINT ulMyProcId, UINT ulMyThreaId, VOID *msgP);

static int dbg_exec_cmd_check( UINT send_proc_queid  ){

	CHAR					str[10];
	CHAR					command[128];
	FILE					*fp;
	UINT					uiFlg = 0;
	CHAR					*ret = NULL;

	memset(str, D_RRH_ZERO, sizeof(str));
	memset(command, D_RRH_ZERO, sizeof(command));
	switch(send_proc_queid){
		case D_RRH_PROCQUE_PF:
			snprintf(command, 128, "ps -ef | grep -v grep | grep DU_pf | grep -v \"DU_pf -p\" |grep -v defunct |wc -l ");
			break;
		case D_RRH_PROCQUE_PTP:
			snprintf(command, 128, "ps -ef | grep -v grep | grep \"DU_pf -p\" |grep -v defunct |wc -l ");
			break;
		case D_RRH_PROCQUE_INIT:
			snprintf(command, 128, "ps -ef | grep -v grep | grep DU_init |grep -v defunct |wc -l ");
			break;
		default:
			return D_RRH_NG;
			break;
	}
	if((fp = popen(command, "r")) == NULL){
		return D_RRH_NG;
	}
	ret = fgets(str, 10, fp);
	if(ret != NULL){
		if(strcmp(str, "1\n") == 0){
			uiFlg = 1;
		}
	}
	pclose(fp);
	if(uiFlg == 0){
		return D_RRH_NG;
	}
	return D_RRH_OK;
}



/*!
 * @brief 関数機能概要:コマンド実行要求送信処理
 * @note  関数処理内容.
 *       -# 通信用共有メモリを取得する
 *       -  取得できない場合
 *          -# エラーメッセージを表示し上位関数へエラーとして返す
 *       -# 通信ヘッダーを設定する
 *       -# コマンド実行要求メッセージ個別情報を設定する。
 *       -# コマンド実行要求を送信する
 *       -  送信エラーの場合
 *          -# エラーメッセージを表示し上位関数へエラーとして返す
 *       -# 上位関数へ正常として返す
 *
 * @param  target		[in]int		コマンド実行先
 * @param  cat_cmd_str	[in]char*	コマンド文字列
 * @return INT
 * @retval result 0:正常/-1:エラー
 * @date 2013/11/25 ALPHA)宮崎 create
 */


int dbg_send_exec_cmd_req( int target, char	*cat_cmd_str   )	{

	int					ret					;
	int					bpf_errcd			;
	int					bpf_result			;
	T_DBG_EXEC_CMD_REQ	*sendMsgP			;	/* 送信用メッセージバッファ	*/
	extern T_DBG_CMD_DEF_TBL	DebugCommandDef[];

	if(( ret = dbg_exec_cmd_check(DebugCommandDef[target].send_proc_queid)) != D_RRH_OK){
		fprintf(stderr, "Error: process dead or not start yet\n") ;
		return D_RRH_NG;
	}

	/* 送信MSG作成(送信先は実行コマンドを含むDBGスレッド)	*/

	/* 通信用共有メモリを取得する   */
    bpf_result = BPF_RU_IPCM_PROCMSG_ADDRGET(   E_BPF_RU_IPCM_BUFF_KIND_TASK,       /* Buffer種別       */
                                        		sizeof(T_DBG_EXEC_CMD_REQ),         /* Size             */
                                        		(void**)&sendMsgP,                    /* msgP             */
                                        		&bpf_errcd );
    if( bpf_result != D_RRH_OK ) {
		fprintf(stderr, "Error: messge send buff alloc error.(code:%d-%d)\n", bpf_result, bpf_errcd ) ;
        return D_RRH_NG;
    }

    /* ヘッダー	*/
	sendMsgP->head.uiEventNo		= CMD_TSKIF_DBG_EXEC_CMD_REQ;					/* デバッグコマンド実行要求			*/
	sendMsgP->head.uiDstPQueueID	= DebugCommandDef[target].send_proc_queid;		/* 送信先Process Queue ID			*/
	sendMsgP->head.uiDstTQueueID	= DebugCommandDef[target].send_dbg_thrad_queid;	/* 送信先Thread Queue ID			*/
	sendMsgP->head.uiSrcPQueueID	= D_RRH_PROCQUE_DBG1;							/* 送信元Process Queue ID			*/
	sendMsgP->head.uiSrcTQueueID	= 0;											/* 送信元Thread Queue ID			*/
	sendMsgP->head.uiLength			= sizeof(T_DBG_EXEC_CMD_REQ);					/* 送信メッセージサイズ				*/
	sendMsgP->head.uiSignalkind		= 0	;											/* 送信シグナル						*/

	/* 個別情報	*/
	strcpy( sendMsgP->dbg_exec_req_info.cmd_exec_thread, DebugCommandDef[target].exec_thread);
	strcpy( sendMsgP->dbg_exec_req_info.cmd_str, cat_cmd_str );

	/* コマンド実行要求を送信する */
	ret = BPF_RU_IPCM_MSGQ_SEND(	sendMsgP->head.uiDstPQueueID,					/* 送信先Process Queue ID			*/
									(void *)sendMsgP );								/* 送信Message						*/
	if( ret != BPF_RU_IPCM_OK )	{
		fprintf(stderr, "command request send error.  code:%d\n", ret );
		return -1;
	}
	return RTC_OK;
}
/* @} */

