/*!
 * @skip  $ld:$
 * @file  f_ful_filelist_rcv.c
 * @brief FULタスク ファイルリスト処理
 * @date  2019/02/21 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */
#include <errno.h>
#include "f_ful_inc.h"

/*!
 * @brief 関数機能概要:ファイルリスト（エラー応答）送信を行う。
 * @note  関数処理内容.
 *		-# エラー応答のMSI送信を実施
 * @param  なし
 * @return なし
 * @date 2019/03/21 KCN)上本 Create
 */
VOID	f_ful_list_MSIsendRej(UINT w_requestId)
{
	T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REJ		sndMsgRej ;
	char				err_param[D_SYS_NUM32] = "parameter error" ;

	memset(&sndMsgRej , D_RRH_ZERO , sizeof(sndMsgRej)) ;
	sndMsgRej.msgErr.errorCode = 0 ;
	memcpy(&sndMsgRej.msgErr.errorDescription[0] , &err_param , sizeof(err_param));
	/* MSI送信 */
	(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REJ ,						/* 送信するメッセージID		*/
						w_requestId ,														/* リクエストID				*/
						sizeof(T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REJ) ,				/* 送信するデータサイズ		*/
						&sndMsgRej ,														/* 送信データのポインタ		*/
						D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
						D_SYS_THDQID_PF_FUL ) ;												/* pf_fulのスレッドID		*/
	return ;
}


/* @} */
/*!
 * @brief 関数機能概要:ファイルリスト転送受信処理を行う。
 * @note  関数処理内容.
 *		-# Abort処理を実施。				f_ful_filelist_rcv() 関数Call
 *		-# 固定で正常を上位に返却する。
 * @param  *rcvMsg_p	[in]	受信メッセージ
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @date 2019/02/21 KCN)上本 Create
 */
INT	f_ful_filelist_rcv(VOID* rcvMsg_p)
{
	FILE	*popen_fp ;
	INT	rtn = D_SYS_OK ;
	UINT	r_messageId = 0;
	UINT	w_requestId = 0;
	INT	w_payloadSizePtr = 0;
	VOID   *w_payloadPtr;

	char   *w_logical_path = NULL;
	char   *w_file_name = NULL;
	char	fixed_path[D_SYS_NUM256 + 1];
	char	cmd_str[1024] ;
	char	cmd_ret[D_SYS_NUM128] ;
	size_t	path_len  = 0;

	T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_CFM		sndMsgCfm ;

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_RETURN , "f_ful_filelist_rcv start. ") ;

	/* ワーク領域の初期化 */
	memset(&sndMsgCfm , 0 , sizeof(sndMsgCfm)) ;
	memset(fixed_path, 0, sizeof(fixed_path));
	memset(cmd_str , 0 , sizeof(cmd_str)) ;
	memset(cmd_ret , 0 , sizeof(cmd_ret)) ;

	/* MSIデータの取得 */
	f_com_MSIgetPayload(rcvMsg_p ,				/* [in]  Receive Message pointer			*/
						&r_messageId ,			/* [out] Receive Message ID					*/
						&w_requestId ,			/* [out] Receive Message Request ID			*/
						&w_payloadSizePtr ,		/* [out] Receive Message payload size		*/
						&w_payloadPtr ) ;		/* [out] Receive Message payload Pointer	*/

	w_file_name = ((T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REQ*)w_payloadPtr)->file_name_filter ;
	w_logical_path = ((T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REQ*)w_payloadPtr)->logical_path ;

	/* ロジカルパスの解析 */
	path_len = strlen( w_logical_path ) ;
	if( path_len == D_SYS_NUM0 )
	{
		/* エラー */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , "logical-path is empty.");
		f_ful_list_MSIsendRej(w_requestId) ;
		return D_SYS_NG ; 
	}

	if(strcmp(w_logical_path , "*") == 0)
	{
		/* ワイルドカード指定 */
		strcpy( fixed_path , D_APP_PATH);
	}
	else {
		/* 絶対パスに変換 */
		if(w_logical_path[0] != '/'){
			snprintf(fixed_path, sizeof(fixed_path), "/%s", w_logical_path);
		}
		else{
			snprintf(fixed_path, sizeof(fixed_path), "%s", w_logical_path);
		}

		/* 決まったパスの下以外は拒否 */
		if(strcmp(fixed_path, D_APP_PATH) != 0
			&& strncmp(fixed_path, D_APP_PATH2, strlen(D_APP_PATH2)) != 0){
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , "logical-path [%s] is not allowed.", fixed_path);
			f_ful_list_MSIsendRej(w_requestId) ;
			return D_SYS_NG ; 
		}
	}

	/* ファイルリストの作成 */
	if(strcmp(w_file_name , "*") == 0)
	{
		snprintf(cmd_str, sizeof(cmd_str), "find %s -type f > /tmp/file_list 2>/dev/null; echo $?", fixed_path ) ;
	}else {
		snprintf(cmd_str, sizeof(cmd_str), "find %s -type f -name '%s' > /tmp/file_list 2>/dev/null; echo $?", fixed_path , w_file_name ) ;
	}
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, "find Command Image: %s" , cmd_str) ;
	

	/* popen実行(findコマンド実行) */
	popen_fp = popen(cmd_str,"r") ;
	if(popen_fp == NULL)
	{
		/* エラー(コマンド実行エラー) */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "find Command Error (Error Code : %x)" , errno) ;
		f_ful_list_MSIsendRej(w_requestId) ;
		rtn = D_SYS_NG ;
		return rtn ;
	}

	if((fgets(cmd_ret, sizeof(cmd_ret), popen_fp)) == NULL)
	{
		/* findコマンド実行エラー */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "find Command Error (Error Info Nothing)" ) ;
		f_ful_list_MSIsendRej(w_requestId) ;
		pclose(popen_fp);
		rtn = D_SYS_NG ;
		return rtn ;
	}
	pclose(popen_fp);

	if(strncmp(cmd_ret, "0", 1 ) != 0)
	{
		/* エラー(実行エラー) */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "find Command Error (Error Code : %s", cmd_ret ) ;
		f_ful_list_MSIsendRej(w_requestId) ;
		rtn = D_SYS_NG ;
		return rtn ;
	}

	/* ファイルリスト応答の編集 */
	strcpy(sndMsgCfm.file_name , "/tmp/file_list");
	/* MSI送信 */
	(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_CFM ,						/* 送信するメッセージID		*/
						w_requestId ,														/* リクエストID				*/
						sizeof(T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_CFM) ,				/* 送信するデータサイズ		*/
						&sndMsgCfm ,														/* 送信データのポインタ		*/
						D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
						D_SYS_THDQID_PF_FUL ) ;												/* pf_fulのスレッドID		*/

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_RETURN , "f_ful_filelist_rcv end. ") ;
	return rtn;
}

/* @} */
