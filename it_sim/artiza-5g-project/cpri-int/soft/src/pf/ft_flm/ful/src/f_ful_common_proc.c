/*!
 * @skip  $ld:$
 * @file  f_ful_common_proc.c
 * @brief FDLタスク 共通関数処理
 * @date  2019/03/11 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */
#include <dirent.h>
#include <sys/stat.h>
#include "f_ful_inc.h"

	char					err_param[32] = "parameter error" ;

/*!
 * @brief 関数機能概要:エラー応答(upload REJ)を送信する。
 * @note  関数処理内容.
 * @param  *path			[in]	検索するディレクトり
 *         recursive		[in]	
 * @return なし
 * @date 2019/03/13 KCN)上本 Create
 */
VOID f_ful_send_MSIrej(UINT w_requestId)
{
	T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REJ	sndMsg ;

	/* 初期化 */
	memset(&sndMsg , D_RRH_ZERO , sizeof(sndMsg)) ;

	sndMsg.msgErr.errorCode = 0 ;
	memcpy(&sndMsg.msgErr.errorDescription[0] , &err_param , sizeof(err_param));

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, "f_ful_send_MSIrej errorCode : %x" , sndMsg.msgErr.errorCode ) ;

	/* MSI送信処理 */
	(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REJ ,						/* 送信するメッセージID		*/
						 w_requestId ,														/* リクエストID				*/
						 sizeof(T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REJ) ,				/* 送信するデータサイズ		*/
						 &sndMsg ,															/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_FUL ) ;											/* pf_eqsのスレッドID		*/

	return ;
}


