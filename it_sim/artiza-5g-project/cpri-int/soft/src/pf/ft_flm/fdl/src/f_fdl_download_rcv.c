/*!
 * @skip  $ld:$
 * @file  f_fdl_download_rcv.c
 * @brief FDLタスク ダウンロード処理
 * @date  2019/02/21 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */
#include <errno.h>
#include "f_fdl_inc.h"

/* 文字列格納用バッファ長 */
#define D_IPADDRESS_BUF_LEN     16
#define D_PORTNUM_BUF_LEN        6
#define D_USER_BUF_LEN           32
#define D_FILENAMEFULL_BUF_LEN 256
#define D_FILENAME_BUF_LEN      256
#define D_CMD_BUF_LEN           768

/*!
 * @brief 関数機能概要:ダウンロード処理を行う。
 * @note  関数処理内容.
 *		-# ダウンロード処理処理を実施。				f_fdl_download_rcv() 関数Call
 *		-# 固定で正常を上位に返却する。
 * @param  *rcvMsg_p	[in]	受信メッセージ
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @date 2019/02/21 KCN)上本 Create
 */
INT	f_fdl_download_rcv(VOID* rcvMsg_p)
{
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_CFM		sndMsg_cfm ;
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ		sndMsg_rej ;
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND	sndMsg_ind ;

	FILE					*popen_fp ;
	char					cmd_ret[D_SYS_NUM128] ;
	INT						rtn = D_SYS_OK ;

	UINT					r_messageId;
	UINT					w_requestId;
	INT						w_payloadSizePtr;
	VOID *					w_payloadPtr;
	
	char					w_ipaddress[D_IPADDRESS_BUF_LEN];		/* IPアドレス */
	char					w_port[D_PORTNUM_BUF_LEN];				/* PORT番号 */
	char					w_user[D_USER_BUF_LEN];				/* ユーザ名 */
	char					w_filename_full[D_FILENAMEFULL_BUF_LEN];	/* ファイル名(フルパス) */
	char					w_filename[D_FILENAME_BUF_LEN];		/* ファイル名(フルパス) */
	char					cmd_str[D_CMD_BUF_LEN];			/* コマンド実行文字列 */
	
	char*					w_url; 					/* ワークポインタ */
	char*					w_password; 			/* ワークポインタ */
	char*					tmp1; 					/* ワークポインタ */
	char*					tmp2; 					/* ワークポインタ */
	size_t					tmp_len ;

	char					err_param[32] = "parameter error" ;
	char					err_sftp[32]  = "sftp error" ;

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_RETURN, "f_fdl_download_rcv start.") ;

	/* パラメータ初期設定 */
	memset(&sndMsg_cfm , D_RRH_ZERO , sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_CFM)) ;
	memset(&sndMsg_rej , D_RRH_ZERO , sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ)) ;
	memset(&sndMsg_ind , D_RRH_ZERO , sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND)) ;
	memset(&w_ipaddress , D_RRH_ZERO , sizeof(w_ipaddress)) ;
	memset(&w_port , D_RRH_ZERO , sizeof(w_port)) ;
	memset(&w_user , D_RRH_ZERO , sizeof(w_user)) ;
	memset(&w_filename_full , D_RRH_ZERO , sizeof(w_filename_full)) ;
	memset(&w_filename , D_RRH_ZERO , sizeof(w_filename)) ;
	memset(&cmd_str , D_RRH_ZERO , sizeof(cmd_str)) ;
	memset(&cmd_ret , D_RRH_ZERO , sizeof(cmd_ret)) ;

	/* MSIデータの取得 */
	f_com_MSIgetPayload(rcvMsg_p ,				/* [in]  Receive Message pointer			*/
						&r_messageId ,			/* [out] Receive Message ID					*/
						&w_requestId ,			/* [out] Receive Message Request ID			*/
						&w_payloadSizePtr ,		/* [out] Receive Message payload size		*/
						&w_payloadPtr ) ;		/* [out] Receive Message payload Pointer	*/

	/* URLイメージ "sftp://user@home:port/abc/def/ghi/FJ-XX-.FGF.zip" */
	w_url = ((T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ*)w_payloadPtr)->uri;
	w_password = ((T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ*)w_payloadPtr)->password;
	
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Starting Software Download(%s,%s) ...",w_url,w_password) ;

	/* 入力文字列チェック */
	tmp_len = strlen("sftp://") ;
	if(strncmp(w_url,"sftp://",tmp_len) != 0) {
		/* エラー */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "url Check Error (sftp::// Non)") ;
		/* 異常応答メッセージの返却 */
		sndMsg_rej.msgErr.errorCode = 0 ;
		memcpy(&sndMsg_rej.msgErr.errorDescription[0] , &err_param , sizeof(err_param));
		rtn = D_SYS_NG ;

	}
	
	if( rtn == D_SYS_OK )
	{
		w_url = w_url + tmp_len ; /* "sftp://"を読み飛ばす */

		/* PORT番号の取得 */
		tmp1 = strstr(w_url , ":");
		if(tmp1 == NULL) { /* PORT番号未設定 */
			strcat(w_port,"22"); /* デフォルトPORT番号(22)を設定 */
		}else{
			tmp1++;																			/* pgr0689 */
			tmp2 = strstr(tmp1,"/");		/* ":"の次の文字から"/"までの文字列の検索(Port番号の検索) */
			if(tmp2 == NULL){
				/* エラー */
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Port No. Errorurl Check Error (sftp::// Non)") ;
				/* 異常応答メッセージの生成 */
				sndMsg_rej.msgErr.errorCode = 0 ;
				memcpy(&sndMsg_rej.msgErr.errorDescription[0] , &err_param , sizeof(err_param));
				rtn = D_SYS_NG;
			}else{
				if((tmp2-tmp1) < sizeof(w_port))
				{
					strncpy(w_port,tmp1,tmp2-tmp1);
				}else{
					/* エラー */
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Port No. Errorurl Check Error (Port No.Counter Over)") ;
					/* 異常応答メッセージの生成 */
					sndMsg_rej.msgErr.errorCode = 0 ;
					memcpy(&sndMsg_rej.msgErr.errorDescription[0] , &err_param , sizeof(err_param));
					rtn = D_SYS_NG;
				}
			}
		}
	}
	
	/* ユーザ名の取得 */
	if(rtn == D_SYS_OK){
		tmp1 = strstr( w_url , "@" ); /* "@"までの文字列を検索(ユーザ名検索) */
		if(tmp1 == NULL){
			/* エラー */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "User Name Not Found (tmp1 = 0)") ;
			/* 異常応答メッセージの生成 */
			sndMsg_rej.msgErr.errorCode = 0 ;
			memcpy(&sndMsg_rej.msgErr.errorDescription[0] , &err_param , sizeof(err_param));
			rtn = D_SYS_NG;
		}else{
			if((tmp1 - w_url) < sizeof(w_user))
			{
				strncpy(w_user,w_url,tmp1-w_url);
			}else{
				/* エラー */
				/* 異常応答メッセージの生成 */
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "User Name Error (count Over)") ;
				sndMsg_rej.msgErr.errorCode = 0 ;
				memcpy(&sndMsg_rej.msgErr.errorDescription[0] , &err_param , sizeof(err_param));
				rtn = D_SYS_NG;
			}
		}
	}
	
	/* IPアドレスの取得 */
	if(rtn == D_SYS_OK){
		tmp1++;			/* pgr0000 */ /* pgr0520 */ /* pgr0520 */
		tmp2 = strstr( tmp1 , ":" ); /* "@"の次の文字":"までの文字列を取り出す */
		if(tmp2 == NULL){
			tmp2 = strstr(tmp1,"/"); /* "@"の次の文字"/"までの文字列を取り出す */
			if(tmp2 == NULL)
			{
				/* エラー */
				/* 異常応答メッセージの生成 */
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "IP Address Error (IPAddress Nothing)") ;
				sndMsg_rej.msgErr.errorCode = 0 ;
				memcpy(&sndMsg_rej.msgErr.errorDescription[0] , &err_param , sizeof(err_param));
				rtn = D_SYS_NG;
			}else{
				if((tmp2-tmp1) < sizeof(w_ipaddress))
				{
					strncpy(w_ipaddress,tmp1,tmp2-tmp1);
				}else{
					/* エラー */
					/* 異常応答メッセージの生成 */
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "IP Address Error (count Over) Count : %d" , tmp2-tmp1) ;
					sndMsg_rej.msgErr.errorCode = 0 ;
					memcpy(&sndMsg_rej.msgErr.errorDescription[0] , &err_param , sizeof(err_param));
					rtn = D_SYS_NG;
				}
			}
		}else{
			if((tmp2-tmp1) < sizeof(w_ipaddress))
			{
				strncpy(w_ipaddress,tmp1,tmp2-tmp1);
			}else{
				/* エラー */
				/* 異常応答メッセージの生成 */
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "IP Address Error (count Over) Count : %d" , tmp2-tmp1) ;
				sndMsg_rej.msgErr.errorCode = 0 ;
				memcpy(&sndMsg_rej.msgErr.errorDescription[0] , &err_param , sizeof(err_param));
				rtn = D_SYS_NG;
			}
		}
	}

	/* ファイル名(フルパス)の取得 */
	if(rtn == D_SYS_OK){
		tmp1 = strstr(w_url , "/");
		if(tmp1 == NULL){
			/* エラー */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Remote File Path Error (Path Nothing)") ;
			/* 異常応答メッセージの生成 */
			sndMsg_rej.msgErr.errorCode = 0 ;
			memcpy(&sndMsg_rej.msgErr.errorDescription[0] , &err_param , sizeof(err_param));
			rtn = D_SYS_NG;
		}else{
			tmp1++;
			tmp_len = strlen(tmp1) ;
			if(tmp_len < sizeof(w_filename_full))
			{
				strncpy(w_filename_full,tmp1,tmp_len);
			}else {
				/* エラー */
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Remote File Path Error (count Over) Count : %d" , tmp_len) ;
				/* 異常応答メッセージの生成 */
				sndMsg_rej.msgErr.errorCode = 0 ;
				memcpy(&sndMsg_rej.msgErr.errorDescription[0] , &err_param , sizeof(err_param));
				rtn = D_SYS_NG;
			}
		}
	}
	
	/* ファイル名の取得 */
	if(rtn == D_SYS_OK){
		tmp2 = strrchr(w_url, '/') ; /* 一番最後の/以降の文字列を取り出す */
		if(tmp2 == NULL){
			/* エラー */
			/* 異常応答メッセージの生成 */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File name Error (File Path Nothing") ;
			sndMsg_rej.msgErr.errorCode = 0 ;
			memcpy(&sndMsg_rej.msgErr.errorDescription[0] , &err_param , sizeof(err_param));
			rtn = D_SYS_NG;
		}else{
			tmp2++;
			tmp_len = strlen(tmp2) ;
			if(tmp_len < sizeof(w_filename))
			{
				strncpy(w_filename,tmp2,tmp_len);
			}else{
				/* エラー */
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File name Error (count Over) Count : %d" , tmp_len) ;
				/* 異常応答メッセージの生成 */
				sndMsg_rej.msgErr.errorCode = 0 ;
				memcpy(&sndMsg_rej.msgErr.errorDescription[0] , &err_param , sizeof(err_param));
				rtn = D_SYS_NG;
			}
		}
	}

	if(rtn == D_SYS_OK){
		/* 正常応答 */
		sndMsg_cfm.notification_timeout = D_FDL_NOTIF_TO_DL;
		/* MSI送信 */
		(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_CFM ,					/* 送信するメッセージID		*/
							w_requestId ,														/* リクエストID				*/
							sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_CFM) ,			/* 送信するデータサイズ		*/
							&sndMsg_cfm ,														/* 送信データのポインタ		*/
							D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
							D_SYS_THDQID_PF_FDL ) ;												/* pf_eqsのスレッドID		*/
	}else{
		/* エラー応答 */
		/* MSI送信 */
		(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ ,					/* 送信するメッセージID		*/
							w_requestId ,														/* リクエストID				*/
							sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ) ,			/* 送信するデータサイズ		*/
							&sndMsg_rej ,														/* 送信データのポインタ		*/
							D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
							D_SYS_THDQID_PF_FDL ) ;												/* pf_eqsのスレッドID		*/
		return rtn ;
	}

	/* SV-Busのダウンロード(ON)の設定を行う */
	f_com_SVCtl( E_DU_SVCTL_OPE_FLDL , D_DU_SVCTL_ON , D_DU_SVCTL_ON_FL_DWNLD ) ;

	rtn = system("rm -rf /tmp/*.zip 1>/null 2>/null");
	rtn |= system("rm -rf /tmp/*.FGF 1>/null 2>/null");		/* pgr0539 */
	rtn |= system("rm -rf /tmp/Fj5gDuCm* 1>/null 2>/null");
	rtn |= system("rm -rf /tmp/*.bin 1>/null 2>/null");
	rtn |= system("rm -rf /tmp/*.bit 1>/null 2>/null");
	rtn |= system("rm -rf /tmp/installpkg 1>/null 2>/null");
	rtn |= system("rm -rf /tmp/manifest.xml 1>/null 2>/null");
	rtn |= system("rm -rf /tmp/sum_result 1>/null 2>/null");

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "OLD File clean: %d" , rtn) ;

	rtn = D_SYS_OK;

	BPF_HM_DEVC_SETTIME_OS_SUPPRESS();	/* 時刻更新抑止 */

	/* sftpスクリプト実行 */
	sprintf(cmd_str,"%s %s %s %s %s %s > /var/log/sftp_download.log ; echo $?", "sftp_download" , w_ipaddress, w_port, w_user, w_password, w_filename_full);
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "sftp Command Image: %s" , cmd_str) ;

	/*  popen実行(sftp_downloadスクリプトを実行する) */
	popen_fp = popen(cmd_str,"r") ;
	if(popen_fp == NULL)
	{
		/* エラー(コマンド実行エラー) */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "sftp_download Error (Error Code : %x)" , errno) ;
		/* 異常応答メッセージの生成 */
		memcpy(&sndMsg_ind.msgErr.errorDescription[0] , &err_sftp , sizeof(err_sftp));
		sndMsg_ind.status = E_SWM_DOWNLOAD_STATUS_PROTOCOL_ERROR;
		rtn = D_SYS_NG;
	}
	if(rtn == D_SYS_OK)
	{
		if((fgets(cmd_ret, sizeof(cmd_ret), popen_fp)) == NULL)		/* pgr0689 */
		{
			/* fgets実行エラー */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "sftp_download Error (Error Info Nothing)") ;
			/* 異常応答メッセージの生成 */
			memcpy(&sndMsg_ind.msgErr.errorDescription[0] , &err_sftp , sizeof(err_sftp));
			sndMsg_ind.status = E_SWM_DOWNLOAD_STATUS_PROTOCOL_ERROR;
			pclose(popen_fp);
			rtn = D_SYS_NG;
		}
	}
	if(rtn == D_SYS_OK)
	{
		if(strncmp(cmd_ret, "0", 1 ) != 0)
		{
			/* エラー(実行エラー) */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "sftp_download Error (Error Code : %s", cmd_ret ) ;
			/* 異常応答メッセージの生成 */
			memcpy(&sndMsg_ind.msgErr.errorDescription[0] , &err_sftp , sizeof(err_sftp));
			sndMsg_ind.status = E_SWM_DOWNLOAD_STATUS_PROTOCOL_ERROR;
			pclose(popen_fp);
			rtn = D_SYS_NG;
		}
	}

	BPF_HM_DEVC_SETTIME_OS_CORRECTION();	/* 時刻補正 */


	if(rtn == D_SYS_OK)
	{
		pclose(popen_fp);
		/* ダウンロードファイルのチェック(.zip形式なら解凍する) */
		tmp1 = strrchr(w_filename , '.') ;
		if(tmp1 != NULL)
		{
			/* 拡張子の判定 */
			tmp1++;
			if((rtn = strcmp(tmp1, "zip")) == D_RRH_ZERO )
			{
				/* zip形式のファイルなら解凍処理を実行 */
				memset(&cmd_str , D_RRH_ZERO , sizeof(cmd_str)) ;

				sprintf(cmd_str,"cd /tmp ; unzip -o /tmp/%s >/dev/null 2>/dev/null; echo $?", w_filename ) ;
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "unZIP Command Image: %s" , cmd_str) ;
				/*  popen実行(unzipコマンドの実行) */
				popen_fp = popen(cmd_str,"r") ;
				if(popen_fp == NULL)
				{
					/* エラー(コマンド実行エラー) */
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "unZIP Command Error (Error Code : %x)" , errno) ;
					/* 異常応答メッセージの生成 */
					memcpy(&sndMsg_ind.msgErr.errorDescription[0] , &err_sftp , sizeof(err_sftp));
					sndMsg_ind.status = E_SWM_DOWNLOAD_STATUS_APPLICATION_ERROR ;
					rtn = D_SYS_NG;
				}

				if(rtn == D_SYS_OK)
				{
					if((fgets(cmd_ret, sizeof(cmd_ret), popen_fp)) == NULL)			/* pgr0689 */
					{
						/* unZIPコマンド実行エラー(返却なし) */
						BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "unZIP Command Error (Error Info Nothing)" ) ;
						/* 異常応答メッセージの生成 */
						memcpy(&sndMsg_ind.msgErr.errorDescription[0] , &err_sftp , sizeof(err_sftp));
						sndMsg_ind.status = E_SWM_DOWNLOAD_STATUS_APPLICATION_ERROR ;
						pclose(popen_fp);
						rtn = D_SYS_NG;
					}
				}

				if(rtn == D_SYS_OK)
				{
					if(strncmp(cmd_ret, "0", 1 ) != 0)
					{
						/* エラー(実行エラー) */
						BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "unZIP Command Error (Error Code : %s", cmd_ret ) ;
						/* 異常応答メッセージの生成 */
						memcpy(&sndMsg_ind.msgErr.errorDescription[0] , &err_sftp , sizeof(err_sftp));
						sndMsg_ind.status = E_SWM_DOWNLOAD_STATUS_APPLICATION_ERROR ;
						rtn = D_SYS_NG;
					}
					pclose(popen_fp);
				}
			}else {
				rtn = D_SYS_OK ;
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "Non Zip File file name : %s", w_filename) ;
			}
		}
	}

	/* notification送信 */
	if(rtn == D_SYS_OK){
		/* 正常応答 */
		sndMsg_ind.status = E_SWM_DOWNLOAD_STATUS_COMPLETED;
		strcpy(sndMsg_ind.file_name, w_filename);
		/* MSI送信 */
		(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND ,				/* 送信するメッセージID		*/
							w_requestId ,														/* リクエストID				*/
							sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND) ,		/* 送信するデータサイズ		*/
							&sndMsg_ind ,														/* 送信データのポインタ		*/
							D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
							D_SYS_THDQID_PF_FDL ) ;												/* pf_eqsのスレッドID		*/

		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Software Download OK(%s)",w_filename) ;
	}else{
		/* エラー応答 */
		/* MSI送信 */
		strcpy(sndMsg_ind.file_name, w_filename);
		(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND ,				/* 送信するメッセージID		*/
							w_requestId ,														/* リクエストID				*/
							sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND) ,		/* 送信するデータサイズ		*/
							&sndMsg_ind ,														/* 送信データのポインタ		*/
							D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
							D_SYS_THDQID_PF_FDL ) ;												/* pf_eqsのスレッドID		*/
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, " DOWNLOAD_IND Error Status : %x Error : %s", sndMsg_ind.status , sndMsg_ind.msgErr.errorDescription) ;
		/* SV-Busのダウンロード(OFF)の設定を行う */
		f_com_SVCtl( E_DU_SVCTL_OPE_FLDL , D_DU_SVCTL_OFF , D_DU_SVCTL_ON_FL_DWNLD ) ;

		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Software Download NG(%s)",w_filename) ;
	}
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_RETURN, "f_fdl_download_rcv end.") ;
	return rtn ;
}
/* @} */


