/*!
 * @skip   $Id$
 * @file   f_dbg_cmd_recv_exec_cmd_rep.c
 * @brief  デバッグコマンド　コマンド実行結果受信処理
 * @date   2013/11/22 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */

#include	"f_dbg_cmd_inc.h"

extern T_DBG_CMD_DEF_TBL	DebugCommandDef[];

/*!
 * @brief 関数機能概要:コマンド実行結果受信処理
 * @note  関数処理内容.
 *       -# デバッグスレッドからの実行結果を受信する
 *       -# 受信待ちにあたり 5 秒の受信待ちを行い、タイムアウトした場合はエラーとする
 *       -# 受信したメッセージをチェックする
 *       -  コマンド要求を実行した先からの返信で無い場合
 *          -# エラーメッセージを表示し上位関数へエラーとして返る
 *       -  正しいところからの送信の場合
 *          -# デバックコマンド実行結果ファイル名へ受信メッセージの値を基に設定する
 *          -# 正常として上位関数へ返す
 * @param  target		[in]int		コマンド実行先
 * @param  output_filename	[out]char*	デバックコマンド実行結果ファイル名
 * @return INT
 * @retval result 0:正常/-1:エラー
 * @date 2013/11/25 ALPHA)宮崎 create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
int dbg_recv_exec_cmd_rep( int target, char *output_filename )	{
	int					recv_result;
	unsigned int		recvmsgsiz;
	unsigned int		waitTime;;
	T_DBG_EXEC_CMD_RES	*res_msg	;
	
	/* 診断の場合のみ、wait timeを長く設定する */
	if( 0 == strcmp( DebugCommandDef[target].exec_thread, "diag" ) )
	{
		waitTime = 100000;
	}
	else
	{
		waitTime = 5000;
	}
	
	/*	MSG受信待ち			*/
	recv_result =  BPF_RU_IPCM_MSGQ_RECV(
			D_RRH_PROCQUE_DBG1,			/* プロセスQID(スレッドQID)		*/
			waitTime,					/* 応答待ち時間(5秒：暫定) TODO:待ち時間数	*/
			(VOID**)&res_msg,			/* 受信MSGポインタ				*/
			&recvmsgsiz					/* 受信MSGサイズ				*/
	);
	if( recv_result != BPF_RU_IPCM_OK || recvmsgsiz == 0 )	{
		fprintf(stderr, "Error: message receive eror. code:%d\n", recv_result );
		*output_filename = 0 ;
		return RTC_NG ;
	}
	/* 出力先ファイル名を異常状態で初期化（正常時は↓でファイル名設定)	*/
	*output_filename = 0 ;
	if( res_msg->dbg_exec_res_info.cmd_exec_status == D_DBG_CMD_EXEC_SUCCESS )	{
		if( res_msg->dbg_exec_res_info.cmd_exec_result == D_DBG_CMD_RST_SUCCESS ){	/* コマンド処理実行結果が正常	*/
			/* 出力先ファイル名を設定	*/
			strcpy( output_filename, res_msg->dbg_exec_res_info.cmd_result_fname )	;
		}
	}
	else {
		fprintf(stderr, "Error: Command Execute Fail.(code:%d)\n", res_msg->dbg_exec_res_info.cmd_exec_status );
	}
	/* 受信メッセージバッファ解放	*/
	(VOID)BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)res_msg );

	return RTC_OK ;
}
/* @} */
