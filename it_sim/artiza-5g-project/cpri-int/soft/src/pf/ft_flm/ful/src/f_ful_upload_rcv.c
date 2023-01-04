/*!
 * @skip  $ld:$
 * @file  f_ful_upload_rcv.c
 * @brief FULタスク アップロード受信処理
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

/* 文字列格納用バッファ長 */
#define D_IPADDRESS_BUF_LEN     16
#define D_PORTNUM_BUF_LEN        6
#define D_USER_BUF_LEN           32
#define D_REMOTEPATH_BUF_LEN    256
#define D_FILENAME_BUF_LEN      256
#define D_CMD_BUF_LEN           1024
#define D_CMDRET_BUF_LEN        128

/*!
 * @brief 関数機能概要:アップロード受信処理を行う。
 * @note  関数処理内容.
 *		-# f_ful_filelist_rcv() 関数Call
 *		-# 固定で正常を上位に返却する。
 * @param  *rcvMsg_p	[in]	受信メッセージ
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @date 2019/02/21 KCN)上本 Create
 */
INT	f_ful_upload_rcv(VOID* rcvMsg_p)
{
	T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_NTF_IND		sndMsg_ind ;

	FILE					*popen_fp ;
	INT						rtn = D_SYS_OK ;			/*	処理結果格納変数		*/

	UINT					r_messageId;
	UINT					w_requestId;
	INT						w_payloadSizePtr;
	VOID*					w_payloadPtr;

	char					w_ipaddress[D_IPADDRESS_BUF_LEN];				/* IPアドレス */
	char					w_port[D_PORTNUM_BUF_LEN];						/* PORT番号 */
	char					w_user[D_USER_BUF_LEN];						/* ユーザ名 */
	char					w_file_name[D_FILENAME_BUF_LEN] ;				/* ファイル名 */
	char					w_remort_path[D_REMOTEPATH_BUF_LEN] ;			/* リモートパス名 */
	char					cmd_str[D_CMD_BUF_LEN];					/* コマンド実行文字列 */
	char					cmd_ret[D_CMDRET_BUF_LEN] ;

	char*					w_local_logical_file_path; 		/* ワークポインタ */
	char*					w_remort_pathp;					/* ワークポインタ */
	char*					w_password; 					/* ワークポインタ */
	char*					tmp1; 							/* ワークポインタ */
	char*					tmp2; 							/* ワークポインタ */
	size_t					sftp_len ;
	size_t					tmp_len ;

	char					err_sftp[32]  = "sftp error" ;

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_RETURN, "f_ful_upload_rcv start.") ;

	/* パラメータ初期設定 */
	memset(&sndMsg_ind , D_RRH_ZERO , sizeof(sndMsg_ind));
	memset(&w_ipaddress , D_RRH_ZERO , sizeof(w_ipaddress)) ;
	memset(&w_port , D_RRH_ZERO , sizeof(w_port)) ;
	memset(&w_user , D_RRH_ZERO , sizeof(w_user)) ;
	memset(&w_remort_path , D_RRH_ZERO , sizeof(w_remort_path)) ;
	memset(&w_file_name , D_RRH_ZERO , sizeof(w_file_name)) ;
	memset(&cmd_str , D_RRH_ZERO , sizeof(cmd_str)) ;
	memset(&cmd_ret , D_RRH_ZERO , sizeof(cmd_ret)) ;

	/* MSIデータの取得 */
	f_com_MSIgetPayload(rcvMsg_p ,				/* [in]  Receive Message pointer			*/
						&r_messageId ,			/* [out] Receive Message ID					*/
						&w_requestId ,			/* [out] Receive Message Request ID			*/
						&w_payloadSizePtr ,		/* [out] Receive Message payload size		*/
						&w_payloadPtr ) ;		/* [out] Receive Message payload Pointer	*/

	w_local_logical_file_path = ((T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REQ*)w_payloadPtr)->local_logical_file_path;
	w_remort_pathp = ((T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REQ*)w_payloadPtr)->remote_file_path;
	w_password = ((T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REQ*)w_payloadPtr)->password;


	//notification用に先にコピーしておく。
	strcpy(sndMsg_ind.local_logical_file_path , w_local_logical_file_path );
	strcpy(sndMsg_ind.remote_file_path , w_remort_pathp ) ;

	/* 文字入力列チェック */
	sftp_len = strlen("sftp://") ;
	if(strncmp(w_remort_pathp , "sftp://" , sftp_len) != 0)
	{
		/* エラー */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , " Input Char Error.") ;
		f_ful_send_MSIrej( w_requestId ) ;
		rtn = D_SYS_NG ;
		return rtn ;
	}

	w_remort_pathp = w_remort_pathp + sftp_len ;		/* "sftp://"を読み飛ばす */

	/* port番号の取得(IPアドレス以降の文字列を探す) */
	tmp1 = strstr(w_remort_pathp , "@");
	if(tmp1 == NULL) { /* IPアドレス指定なし */
		/* エラー */
		/* 異常応答メッセージの生成 */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , " Input User Name Error.(User Name Nothing)") ;
		f_ful_send_MSIrej( w_requestId ) ;
		rtn = D_SYS_NG;
		return rtn ;
	}
	tmp1++;
	tmp2 = strstr(tmp1 , ":");
	if(tmp2 == NULL) { /* PORT番号未設定 */
		strcat(w_port,"22"); /* デフォルトPORT番号(22)を設定 */
	}else{
		tmp2++;																			/* pgr0689 */
		tmp1 = strstr(tmp2,"/");		/* ":"の次の文字から"/"までの文字列の検索(Port番号の検索) */
		if(tmp1 == NULL){
			/* エラー */
			/* 異常応答メッセージの生成 */
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , " Input Port No. Error.") ;
			f_ful_send_MSIrej( w_requestId ) ;
			rtn = D_SYS_NG;
			return rtn ;
		}else{
			if((tmp1-tmp2) < sizeof(w_port))
			{
				strncpy(w_port,tmp2,tmp1-tmp2);
			}else{
				/* エラー */
				/* 異常応答メッセージの生成 */
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , " Input Port No. Error.Port : %s" , tmp1) ;
				f_ful_send_MSIrej( w_requestId ) ;
				rtn = D_SYS_NG;
				return rtn ;
			}
		}
	}

	/* ユーザ名の取得 */
	tmp1 = strstr( w_remort_pathp , "@" ); /* "@"までの文字列を検索(ユーザ名検索) */
	if(tmp1 == NULL){
		/* エラー */
		/* 異常応答メッセージの生成 */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , " Input User Name Error.(User Name Nothing)") ;
		f_ful_send_MSIrej( w_requestId ) ;
		rtn = D_SYS_NG;
		return rtn ;
	}else{
		/* 以下の4パターン */
		/* sftp://ユーザ名@ipアドレス/パス */
		/* sftp://ユーザ名@ipアドレス:ポート番号/パス */
		/* sftp://ユーザ名:パスワード@ipアドレス/パス */
		/* sftp://ユーザ名:パスワード@ipアドレス:ポート番号/パス */
		tmp2 = strstr( w_remort_pathp , ":" ) ;	/* ":"までの文字列を検索ユーザ名 */
		if(tmp2 == NULL){
			/* sftp://ユーザ名@ipアドレス/パス */
			/* tmp2までをユーザ名とするため */
			tmp2 = tmp1;
		}
		else if(tmp1 < tmp2){
			/* sftp://ユーザ名@ipアドレス:ポート番号/パス */
			/* tmp2までをユーザ名とするため */
			tmp2 = tmp1;
		}
		/* else:残りの2パターンはtmp2をそのまま使う */

		if(tmp2 - w_remort_pathp < sizeof(w_user)){
			strncpy(w_user,w_remort_pathp,tmp2-w_remort_pathp);
		}else{
			/* エラー */
			/* 異常応答メッセージの生成 */
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , " Input User Name Error. User Name : %s" , w_remort_pathp) ;
			f_ful_send_MSIrej( w_requestId ) ;
			rtn = D_SYS_NG;
			return rtn ;
		}
	}

	/* IPアドレスの取得 */
	tmp1++;
	tmp2 = strstr( tmp1 , ":" ); /* "@"の次の文字":"までの文字列を取り出す */
	if(tmp2 == NULL){
		tmp2 = strstr(tmp1,"/"); /* "@"の次の文字"/"までの文字列を取り出す */
		if(tmp2 == NULL)
		{
			/* エラー */
			/* 異常応答メッセージの生成 */
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , " Input IP Address Error. IP Address Nothing") ;
			f_ful_send_MSIrej( w_requestId ) ;
			rtn = D_SYS_NG;
			return rtn ;
		}else{
			if((tmp2-tmp1) < sizeof(w_ipaddress))
			{
				strncpy(w_ipaddress,tmp1,tmp2-tmp1);
			}else{
				/* エラー */
				/* 異常応答メッセージの生成 */
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , " Input IP Address Error. IP Address : %s",tmp1) ;
				f_ful_send_MSIrej( w_requestId ) ;
				rtn = D_SYS_NG;
				return rtn ;
			}
		}
	}else{
		if((tmp2-tmp1) < sizeof(w_ipaddress))
		{
			strncpy(w_ipaddress,tmp1,tmp2-tmp1);
		}else{
			/* エラー */
			/* 異常応答メッセージの生成 */
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , " Input IP Address Error. IP Address : %s",tmp1) ;
				f_ful_send_MSIrej( w_requestId ) ;
				rtn = D_SYS_NG;
				return rtn ;
		}
	}

	/* フルパスの取得 */
	tmp1 = strstr(w_remort_pathp , "/");
	if(tmp1 == NULL){
		/* エラー */
		/* 異常応答メッセージの生成 */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , " Input Remote-Path Error. Remote-Path Nothing") ;
		f_ful_send_MSIrej( w_requestId ) ;
		rtn = D_SYS_NG;
		return rtn ;
	}else{
		tmp1++;
		tmp_len = strlen(tmp1) ;
		if(tmp_len < sizeof(w_remort_path))
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO , "tmp1 : %s  ", tmp1 );
			strncpy(w_remort_path,tmp1,tmp_len);
		}else{
			/* エラー */
			/* 異常応答メッセージの生成 */
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , " Input Remote-Path Error. Remote-Path : %s",tmp1) ;
			f_ful_send_MSIrej( w_requestId ) ;
			rtn = D_SYS_NG;
			return rtn ;
		}
	}

	/* パスの検査 */
	if(strncmp(w_local_logical_file_path, D_APP_PATH2, strlen(D_APP_PATH2)) != 0){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , " local-logical-file-path [%s]is not allowed.", w_local_logical_file_path) ;
		f_ful_send_MSIrej( w_requestId ) ;
		return D_SYS_NG;
	}

	/* ファイル名の取得 */
	tmp2 = strrchr(w_local_logical_file_path, '/') ; /* 一番最後の/以降の文字列を取り出す */
	if(tmp2 == NULL){
		/* エラー */
		/* 異常応答メッセージの生成 */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , " local-logical-file-path : no filename.") ;
		f_ful_send_MSIrej( w_requestId ) ;
		rtn = D_SYS_NG;
		return rtn ;
	}else{
		tmp2++;
		tmp_len = strlen(tmp2) ;
		if(tmp_len < sizeof(w_file_name))
		{
			strncpy(w_file_name,tmp2,tmp_len);
		}else{
			/* エラー */
			/* 異常応答メッセージの生成 */
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , " Input File Name Error. File Name : %s",tmp2) ;
			f_ful_send_MSIrej( w_requestId ) ;
			rtn = D_SYS_NG;
			return rtn ;
		}
	}

	/* MSI送信(正常応答) */
	(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_CFM ,							/* 送信するメッセージID		*/
						w_requestId ,														/* リクエストID				*/
						D_RRH_ZERO ,														/* 送信するデータサイズ		*/
						NULL ,																/* 送信データのポインタ		*/
						D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
						D_SYS_THDQID_PF_FUL ) ;												/* pf_fulのスレッドID		*/


	BPF_HM_DEVC_SETTIME_OS_SUPPRESS();	/* 時刻更新抑止 */

	/* sftpスクリプト実行 */
	sprintf(cmd_str , "%s %s %s %s %s %s %s %s > /var/log/sftp_upload.log ; echo $?", "sftp_upload" , w_ipaddress, w_port, w_user, w_password, w_file_name, w_remort_path , w_local_logical_file_path);
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "sftp Command Image: %s" , cmd_str) ;

	/*  popen実行(sftp_uploadスクリプトを実行する) */
	popen_fp = popen(cmd_str,"r") ;
	if(popen_fp == NULL)
	{
		/* エラー(コマンド実行エラー) */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "sftp_upload Error (Error Code : %x)" , errno) ;
		/* 異常応答メッセージの生成 */
		sndMsg_ind.msgErr.errorCode = 0 ;
		memcpy(&sndMsg_ind.msgErr.errorDescription[0] , &err_sftp , sizeof(err_sftp));
		rtn = D_SYS_NG;
	}

	if(rtn == D_SYS_OK)
	{
		if((fgets(cmd_ret, sizeof(cmd_ret), popen_fp)) == NULL)				/* pgr0689 */
		{
			/* fgets実行エラー */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "sftp_upload Error (Error Info Nothing)") ;
			sndMsg_ind.msgErr.errorCode = 0 ;
			memcpy(&sndMsg_ind.msgErr.errorDescription[0] , &err_sftp , sizeof(err_sftp));
			rtn = D_SYS_NG;
			pclose(popen_fp);
		}
	}

	if(rtn == D_SYS_OK)
	{
		if(strncmp(cmd_ret, "0", 1 ) != 0)
		{
			/* エラー(実行エラー) */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "sftp_upload Error (Error Code : %s", cmd_ret ) ;
			sndMsg_ind.msgErr.errorCode = 0 ;
			memcpy(&sndMsg_ind.msgErr.errorDescription[0] , &err_sftp , sizeof(err_sftp));
			rtn = D_SYS_NG;
		}
		pclose(popen_fp);
	}

	BPF_HM_DEVC_SETTIME_OS_CORRECTION();	/* 時刻補正 */

	/* notification送信 */
	if(rtn == D_SYS_OK)
	{
		/* 正常応答 */
		sndMsg_ind.status = E_FLM_STATUS_SUCCESS ;
		/* MSI送信 */
		(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_NTF_IND ,						/* 送信するメッセージID		*/
							w_requestId ,														/* リクエストID				*/
							sizeof(T_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_NTF_IND) ,				/* 送信するデータサイズ		*/
							&sndMsg_ind ,														/* 送信データのポインタ		*/
							D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
							D_SYS_THDQID_PF_FUL ) ;												/* pf_fulのスレッドID		*/
	}else{
		/* エラー応答 */
		/* MSI送信 */
		sndMsg_ind.status = E_FLM_STATUS_FAILURE ;
		(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_NTF_IND ,						/* 送信するメッセージID		*/
							w_requestId ,														/* リクエストID				*/
							sizeof(T_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_NTF_IND) ,				/* 送信するデータサイズ		*/
							&sndMsg_ind ,														/* 送信データのポインタ		*/
							D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
							D_SYS_THDQID_PF_FUL ) ;												/* pf_fulのスレッドID		*/
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, " UPLOAD_IND Error Status : %x Error : %s", sndMsg_ind.status , sndMsg_ind.msgErr.errorDescription) ;
	}
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_RETURN, "f_ful_upload_rcv end.") ;
	return rtn;
}

/* @} */
