/*!
 * @skip  $ld:$
 * @file  f_fdl_common_proc.c
 * @brief FDLタスク Abort処理
 * @date  2019/02/21 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */
#include <errno.h>
#include <sys/stat.h>
#include <byteswap.h>
#include <bpf_i_ru_facc_xml.h>
#include "f_fdl_inc.h"

static	char	f_fdl_os_check_code[]	= {"SW_NAME"} ;
static	char	f_fdl_fpgam_chk_code[]	= {"PLFPGA_NAME"} ;
static	char	f_fdl_fpgas_chk_code[]	= {"TRXFPGA_NAME"} ;
static	char	f_fdl_rficbt_chk_code[]	= {"RFIC_BOOT_NAME"} ;
static	char	f_fdl_rficam_chk_code[]	= {"RFIC_ARM_NAME"} ;
static	char	f_fdl_ptpcnf_chk_code[]	= {"PTPCNF_NAME"} ;


/*!
 * @brief 関数機能概要:Notification通知送信を行う。
 * @note  関数処理内容.
 *		-# notification(install)の編集。
 *		-# MSI送信処理を実行。
 * @param  error_code		[in]	エラーコード
 *         w_slot			[in]	slot番号
 * @return なし
 * @date 2019/03/04 KCN)上本 Create
 */
VOID f_fdl_send_MSInotif(INT error_code , UINT w_slot)
{
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND		sndMsg ;
	INT														w_index ;
	UCHAR													eep_data ;

	/* 初期化 */
	memset(&sndMsg , D_RRH_ZERO , sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND)) ;

	w_index = error_code ;

	/* エラーメッセージ編集 */
	switch( error_code )
	{
		case	E_FDL_ERR_OK:									/* 正常終了 */
			sndMsg.status = E_SWM_INSTALL_STATUS_COMPLETED ;
			break ;
		case	E_FDL_ERR_OPEN:									/* ファイルオープンエラー	*/
			sndMsg.msgErr.errorCode = E_MPSW_ERR_CODE_PARAMETER_ERROR	;
			sndMsg.status = E_SWM_INSTALL_STATUS_FILE_ERROR ;
			break ;
		case	E_FDL_ERR_UNZIP:								/* unzip実行エラー 			*/
			sndMsg.msgErr.errorCode = E_MPSW_ERR_CODE_PARAMETER_ERROR ;
			sndMsg.status = E_SWM_INSTALL_STATUS_APPLICATION_ERROR ;
			break ;
		case	E_FDL_ERR_ERSERR:								/* Flash消去失敗			*/
			sndMsg.msgErr.errorCode = E_MPSW_ERR_CODE_HW_ACCESS_ERROR ;
			sndMsg.status = E_SWM_INSTALL_STATUS_FILE_ERROR ;
			break ;
		case	E_FDL_ERR_WRTERR:								/* Flash書込み失敗			*/
			sndMsg.msgErr.errorCode = E_MPSW_ERR_CODE_HW_ACCESS_ERROR ;
			sndMsg.status = E_SWM_INSTALL_STATUS_FILE_ERROR ;
			break ;
		case	E_FDL_ERR_BUFERR:								/* バッファエラー			*/
			sndMsg.msgErr.errorCode = E_MPSW_ERR_CODE_RESOURCE_BUSY ;
			sndMsg.status = E_SWM_INSTALL_STATUS_APPLICATION_ERROR ;
			break ;
		case	E_FDL_ERR_NOFILE:								/* 更新ファイルなし			*/
			sndMsg.msgErr.errorCode = E_MPSW_ERR_CODE_PARAMETER_ERROR ;
			sndMsg.status = E_SWM_INSTALL_STATUS_APPLICATION_ERROR ;
			break ;
		case	E_FDL_ERR_CHKSUM:								/* チェックサムエラー		*/
			sndMsg.msgErr.errorCode = E_MPSW_ERR_CODE_PARAMETER_ERROR ;
			sndMsg.status = E_SWM_INSTALL_STATUS_APPLICATION_ERROR ;
			break ;
		case	E_FDL_ERR_INVALID:								/* 更新ファイル無効		*/
			sndMsg.msgErr.errorCode = E_MPSW_ERR_CODE_PARAMETER_ERROR ;
			sndMsg.status = E_SWM_INSTALL_STATUS_APPLICATION_ERROR ;
			break ;
		case	E_FDL_ERR_PARAERR:								/* パラメータエラー			*/
			sndMsg.msgErr.errorCode = E_MPSW_ERR_CODE_PARAMETER_ERROR ;
			sndMsg.status = E_SWM_INSTALL_STATUS_APPLICATION_ERROR ;
			break ;
		default		:											/* その他			*/
			w_index = E_FDL_ERR_PARAERR ;
			sndMsg.msgErr.errorCode = E_MPSW_ERR_CODE_PARAMETER_ERROR ;
			sndMsg.status = E_SWM_INSTALL_STATUS_APPLICATION_ERROR ;
			break ;
	}

	if(error_code != E_FDL_ERR_OK)
	{
		memcpy(&sndMsg.msgErr.errorDescription , &f_fdl_install_errtbl[w_index] , D_SYS_NUM32 ) ;
		/* SV-Busのダウンロード(OFF)の設定を行う */
		f_com_SVCtl( E_DU_SVCTL_OPE_FLDL , D_DU_SVCTL_OFF , D_DU_SVCTL_ON_FL_DWNLD ) ;
	}
	memcpy(&sndMsg.slot_name , &f_fdl_Slot_nametbl[w_slot] , D_SYS_NUM5);

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, "f_fdl_send_MSInotif Send Install-IND. status : %x" , sndMsg.status ) ;

	/* MSI送信処理 */
	(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND ,				/* 送信するメッセージID		*/
						 D_RRH_ZERO ,														/* リクエストID				*/
						 sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND) ,		/* 送信するデータサイズ		*/
						 &sndMsg ,															/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_FDL ) ;											/* pf_eqsのスレッドID		*/

	if(error_code == E_FDL_ERR_OK)
	{
		/* EEPのステータスにVALIDを設定 */
		f_cmw_tra_inv_tbl->cmx_slot_status[w_slot] = D_DU_EEP_STATUS_VALID ;
		eep_data = D_DU_EEP_STATUS_VALID ;
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_SWSLOT_STATUS + w_slot , &eep_data) ;

		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Software Install OK");

	}else{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Software Install NG(0x%x)",error_code);
	}

	return ;
}


/*!
 * @brief 関数機能概要:MSI応答(REJ)の送信を行う
 * @note  関数処理内容.
 *		-# Install Rejの編集。
 *		-# MSI送信処理を実行。
 * @param  error_code		[in]	エラーコード
 *         w_slot			[in]	slot番号
 * @return なし
 * @date 2019/03/04 KCN)上本 Create
 */
VOID f_fdl_send_MSIrej(UINT w_requestId , INT error_code)
{
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ		sndMsg ;
	INT													w_index ;

	/* 初期化 */
	memset(&sndMsg , D_RRH_ZERO , sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ)) ;

	w_index = error_code ;

	/* エラーメッセージ編集 */
	switch( error_code )
	{
		case	E_FDL_ERR_PARAERR:								/* パラメータエラー			*/
			sndMsg.msgErr.errorCode = E_MPSW_ERR_CODE_PARAMETER_ERROR	;
			break ;
		case	E_FDL_ERR_NOFILE:								/* 更新ファイルなし			*/
			sndMsg.msgErr.errorCode = E_MPSW_ERR_CODE_PARAMETER_ERROR ;
			break ;
		case	E_FDL_ERR_INVALID:								/* 更新ファイル無効			*/
			sndMsg.msgErr.errorCode = E_MPSW_ERR_CODE_PARAMETER_ERROR ;
			break ;
		default		:											/* その他			*/
			sndMsg.msgErr.errorCode = E_MPSW_ERR_CODE_PARAMETER_ERROR ;
			w_index = E_FDL_ERR_PARAERR ;
			break ;
	}

	memcpy(&sndMsg.msgErr.errorDescription , &f_fdl_install_errtbl[w_index] , D_SYS_NUM32 ) ;

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, "f_fdl_send_MSIrej errorCode : %x" , sndMsg.msgErr.errorCode ) ;

	/* MSI送信処理 */
	(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ ,					/* 送信するメッセージID		*/
						 w_requestId ,														/* リクエストID				*/
						 sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ) ,			/* 送信するデータサイズ		*/
						 &sndMsg ,															/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_FDL ) ;											/* pf_eqsのスレッドID		*/

	/* SV-Busのダウンロード(OFF)の設定を行う */
	f_com_SVCtl( E_DU_SVCTL_OPE_FLDL , D_DU_SVCTL_OFF , D_DU_SVCTL_ON_FL_DWNLD ) ;
	return ;
}


/*!
 * @brief 関数機能概要:ファイル更新の完了確認
 * @note  関数処理内容.
 *		-# すべてのファイルが更新されているかを確認する
 *         ファイルが更新されていれば、Notificatoin(完了)を通知
 * @param  なし
 * @return なし
 * @retval D_SYS_OK		0:正常終了
 * @date 2019/03/08 KCN)上本 Create
 */
INT	f_fdl_file_check(VOID)
{
	INT				count ;
	INT				ret = D_SYS_OK ;
	
	/* プロセス */

	/* ファイル更新完了チェック */
	for(count = 0 ; count < E_FDL_FLSH_FILEMAX ; count++)
	{
		if(f_fdl_inst_state_data.install_State[count] != D_FDL_INST_STOP)
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "File Installing Now : %x %x %x %x %x %x" ,
					f_fdl_inst_state_data.install_State[D_SYS_NUM0] , f_fdl_inst_state_data.install_State[D_SYS_NUM1] ,
					f_fdl_inst_state_data.install_State[D_SYS_NUM2] , f_fdl_inst_state_data.install_State[D_SYS_NUM3] ,
					f_fdl_inst_state_data.install_State[D_SYS_NUM4] , f_fdl_inst_state_data.install_State[D_SYS_NUM5] ) ;
			ret = D_SYS_NG ;
			break ;
		}
	}
	return ret ;
}


/*!
 * @brief 関数機能概要:サム値の確認
 * @note  関数処理内容.
 *		-# md5sumコマンドでファイルのサム値を算出
 *		-# マニフェストファイルのサム値との確認
 * @param  *file_name	[in]	サム値算出ファイル名
 *		   *mani_data	[in]	マニフェストファイル内のサム値記録行
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @date 2019/03/05 KCN)上本 Create
 */
INT	f_fdl_checksum(char * file_name , char * mani_data)
{
	FILE		*popen_fp ;
	FILE		*sum_fp ;
	INT			rtn ;
	char		read_data[D_SYS_NUM128] ;
	char		file_check_sum[D_SYS_NUM256] ;
	char		cmd_str[D_SYS_NUM256] ;
	char		cmd_ret[D_SYS_NUM128] ;
	char*		sum_file ;

	memset(&read_data , D_RRH_ZERO , sizeof(read_data));
	memset(&file_check_sum , D_RRH_ZERO , sizeof(file_check_sum));
	memset(&cmd_str , D_RRH_ZERO , sizeof(cmd_str));
	memset(&cmd_ret , D_RRH_ZERO , sizeof(cmd_ret)) ;

	/* ファイルのチェックサムを起動 */
	sprintf(cmd_str, "%s /tmp/%s %s 2>/dev/null; echo $?", "md5sum " , file_name , " > /tmp/sum_result" ) ;
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER , "md5sum Command Image: %s" , cmd_str) ;

	/*  popen実行 */
	popen_fp = popen(cmd_str,"r") ;
	if(popen_fp == NULL)
	{
		/* エラー(コマンド実行エラー) */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "md5sum Command Error (Error Code : %x)" , errno) ;
		rtn = D_SYS_NG ;
		return rtn ;
	}

	if((fgets(cmd_ret, sizeof(cmd_ret), popen_fp)) == NULL)
	{
		/* md5sumコマンド実行エラー */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "md5sum Command Error (Error Info Nothing)" ) ;
		rtn = D_SYS_NG ;
		pclose(popen_fp);
		return rtn ;
	}

	if(strncmp(cmd_ret, "0", 1 ) != 0)
	{
		/* エラー(実行エラー) */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "md5sum Command Error (Error Code : %s", cmd_ret ) ;
		rtn = D_SYS_NG ;
		pclose(popen_fp);
		return rtn ;
	}
	pclose(popen_fp);

	if(( sum_fp = fopen("/tmp/sum_result","r")) == NULL)
	{
		/* ファイルオープン失敗 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Open Error(sum_result)") ;
		rtn = D_SYS_NG ;
		return rtn ;
	}

	/* サム値の取り出し */
	if((fgets(read_data , D_SYS_NUM128 , sum_fp)) == NULL )
	{
		/* サム値取り出し失敗 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Check sum Data Put Error : %s" , read_data) ;
		rtn = D_SYS_NG ;
		fclose(sum_fp) ;
		return rtn ;
	}

	fclose(sum_fp) ;
	if((sum_file = strtok(read_data , " ")) == NULL)
	{
		/* サム値取り出し失敗 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Check sum Data Put Error : %s" , read_data) ;
		rtn = D_SYS_NG ;
		return rtn ;
	}

	strcpy(file_check_sum , sum_file) ;

	/* サム値のチェック */
	if(strncmp(file_check_sum , mani_data , D_SYS_NUM32) != 0)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Check sum Error : File :%s mani:%s" , file_check_sum , mani_data ) ;
		rtn = D_SYS_NG ;
		return rtn ;
	}

	rtn = D_SYS_OK ;
	return rtn ;
}


/*!
 * @brief 関数機能概要:更新File名の取得。
 * @note  関数処理内容.
 *		-# 更新するファイル名の取得
 * @param  *check_nm	[in]	更新するファイルの題名
 *         *file_name   [in/out]	取得する更新ファイル名
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @date 2019/02/21 KCN)上本 Create
 */
INT	f_fdl_get_file_name(char * check_nm , char * file_name)
{
	FILE*		pkgfp ;
	INT			rtn ;
	char		w_file_list[D_SYS_NUM256] ;
	char *		wfile_name;
	char *		w_tmp1 ;

	memset(&w_file_list , D_RRH_ZERO , D_SYS_NUM256) ;

	if((pkgfp = fopen("/tmp/installpkg","r")) == NULL)
	{
		/* ファイル名誤りでNotification通知 */
		rtn = E_FDL_ERR_OPEN ;
		return rtn ;
	}

	while(1)
	{
		if(fgets(w_file_list , D_SYS_NUM256 , pkgfp) == NULL )
		{
			/* ファイルなし返却 */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Nothing") ;
			rtn = E_FDL_ERR_NOFILE ;
			fclose(pkgfp) ;
			return rtn ;
			break ;
		}

		if((strncmp(w_file_list , check_nm , strlen(check_nm))) == 0)
		{
			if((w_tmp1 = strstr(w_file_list , "\"")) != NULL)
			{
				w_tmp1++;
				if((strstr(w_tmp1 , "\"")) == NULL)
				{
					/* ファイル名なし(拡張子が見つからない) */
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Nothing 2 ") ;
					rtn = E_FDL_ERR_NOFILE ;
					fclose(pkgfp) ;
					return rtn ;
					break ;
				}
				wfile_name = strtok(w_tmp1, "\"");
				break ;
			}
		}
	}
	sprintf(file_name , "/tmp/%s", wfile_name) ;

	fclose(pkgfp) ;
	rtn = D_SYS_OK ;
	return rtn ;
}


/*!
 * @brief 関数機能概要:ファイル書込み要求送信処理を行う。
 * @note  関数処理内容.
 *		-# 更新するファイル名を引き渡しファイル書込み要求を送信する。
 * @param  write_kind	[in]	ファイル更新種別
 *		   w_slot		[in]	更新スロット(面)
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @date 2019/03/04 KCN)上本 Create
 */
INT	f_fdl_flash_write(UINT write_kind , UINT w_slot)
{
	int			rtn = D_SYS_NG ;
	int			ret ;
	int			errcd ;
	char		w_file_list[D_SYS_NUM256] ;
	T_SYS_NMA_FILE_WRITE_REQ *		sndMsg ;

	memset(&w_file_list , D_RRH_ZERO , sizeof(w_file_list)) ;

	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
										sizeof(T_SYS_NMA_FILE_WRITE_REQ),		/* Size								*/
										(VOID **)&sndMsg,						/* sndMsg							*/
										&errcd );
	if(ret != D_SYS_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Resource Busy ") ;
		/* リソースビジーでNotification通知 */
		f_fdl_send_MSInotif(E_FDL_ERR_BUFERR , w_slot) ;
		return rtn ;
	}
	else
	{
		sndMsg->thdIf.uiDstPQueueID	= D_RRH_PROCQUE_PF;						/* 送信先Process Queue ID			*/
		sndMsg->thdIf.uiSrcPQueueID	= D_RRH_PROCQUE_PF;						/* 送信元Process Queue ID			*/
	}

	/* ファイル更新種別の確認 */
	switch( write_kind )
	{
		case	E_FDL_FLSH_OSPF:
			if(w_slot == D_SYS_NUM0)
			{
				/* Slot0の設定 */
				sndMsg->kind = E_RRH_FILE_KIND_KERNEL0 ;
			}
			else
			{
				sndMsg->kind = E_RRH_FILE_KIND_KERNEL1 ;
			}
			sndMsg->thdIf.uiEventNo		= D_SYS_MSGID_NMA_FILE_WRITE_REQ ;		/* ファイル書込み要求				*/
			sndMsg->thdIf.uiDstTQueueID	= D_SYS_THDQID_PF_NMA;					/* 送信先Thread Queue ID			*/
			sndMsg->thdIf.uiSrcTQueueID	= D_SYS_THDQID_PF_FDL;					/* 送信元Thread Queue ID			*/

			/* ファイル名のコピー */
			ret = f_fdl_get_file_name(f_fdl_os_check_code , w_file_list) ;
			if(ret != D_SYS_OK)
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Name Error(File Nothing) ") ;
				f_fdl_send_MSInotif(rtn , w_slot);
				BPF_RU_IPCM_PROCMSG_ADDRFREE((VOID*)sndMsg) ;
				return rtn ;
			}
			break ;
		case	E_FDL_FLSH_FPGAS:
			if(w_slot == D_SYS_NUM0)
			{
				/* Slot0の設定 */
				sndMsg->kind = E_RRH_FILE_KIND_TXRPL0 ;
			}
			else
			{
				sndMsg->kind = E_RRH_FILE_KIND_TXRPL1 ;
			}
			sndMsg->thdIf.uiEventNo		= D_SYS_MSGID_NMAS_FILE_WRITE_REQ ;		/* ファイル書込み要求				*/
			sndMsg->thdIf.uiDstTQueueID	= D_SYS_THDQID_PF_NMAS ;				/* 送信先Thread Queue ID			*/
			sndMsg->thdIf.uiSrcTQueueID	= D_SYS_THDQID_PF_FDL ;					/* 送信元Thread Queue ID			*/
			
			/* ファイル名のコピー */
			ret = f_fdl_get_file_name(f_fdl_fpgas_chk_code , w_file_list) ;
			if(ret != D_SYS_OK)
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Name Error(File Nothing) ") ;
				f_fdl_send_MSInotif(rtn , w_slot);
				BPF_RU_IPCM_PROCMSG_ADDRFREE((VOID*)sndMsg) ;
				return rtn ;
			}
			break ;
		case	E_FDL_FLSH_FPGAM:
			if(w_slot == D_SYS_NUM0)
			{
				/* Slot0の設定 */
				sndMsg->kind = E_RRH_FILE_KIND_PLTFRM0 ;
			}
			else
			{
				sndMsg->kind = E_RRH_FILE_KIND_PLTFRM1 ;
			}
			sndMsg->thdIf.uiEventNo		= D_SYS_MSGID_NMA_FILE_WRITE_REQ ;		/* ファイル書込み要求				*/
			sndMsg->thdIf.uiDstTQueueID	= D_SYS_THDQID_PF_NMA;					/* 送信先Thread Queue ID			*/
			sndMsg->thdIf.uiSrcTQueueID	= D_SYS_THDQID_PF_FDL;					/* 送信元Thread Queue ID			*/

			/* ファイル名のコピー */
			ret = f_fdl_get_file_name(f_fdl_fpgam_chk_code , w_file_list) ;
			if(ret != D_SYS_OK)
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Name Error(File Nothing) ") ;
				f_fdl_send_MSInotif(rtn , w_slot);
				BPF_RU_IPCM_PROCMSG_ADDRFREE((VOID*)sndMsg) ;
				return rtn ;
			}
			break ;
		case	E_FDL_FLSH_RFIC_BOOT:
			if(w_slot == D_SYS_NUM0)
			{
				/* Slot0の設定 */
				sndMsg->kind = E_RRH_FILE_KIND_RFICARM0 ;
			}
			else
			{
				sndMsg->kind = E_RRH_FILE_KIND_RFICARM1 ;
			}
			sndMsg->thdIf.uiEventNo		= D_SYS_MSGID_NMA_FILE_WRITE_REQ ;		/* ファイル書込み要求				*/
			sndMsg->thdIf.uiDstTQueueID	= D_SYS_THDQID_PF_NMA;					/* 送信先Thread Queue ID			*/
			sndMsg->thdIf.uiSrcTQueueID	= D_SYS_THDQID_PF_FDL;					/* 送信元Thread Queue ID			*/

			/* ファイル名のコピー */
			ret = f_fdl_get_file_name(f_fdl_rficbt_chk_code , w_file_list) ;
			if(ret != D_SYS_OK)
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Name Error(File Nothing) ") ;
				f_fdl_send_MSInotif(rtn , w_slot);
				BPF_RU_IPCM_PROCMSG_ADDRFREE((VOID*)sndMsg) ;
				return rtn ;
			}
			break ;
		case	E_FDL_FLSH_RFIC_ARM:
			if(w_slot == D_SYS_NUM0)
			{
				/* Slot0の設定 */
				sndMsg->kind = E_RRH_FILE_KIND_RFICFRM0 ;
			}
			else
			{
				sndMsg->kind = E_RRH_FILE_KIND_RFICFRM1 ;
			}
			sndMsg->thdIf.uiEventNo		= D_SYS_MSGID_NMA_FILE_WRITE_REQ ;		/* ファイル書込み要求				*/
			sndMsg->thdIf.uiDstTQueueID	= D_SYS_THDQID_PF_NMA;					/* 送信先Thread Queue ID			*/
			sndMsg->thdIf.uiSrcTQueueID	= D_SYS_THDQID_PF_FDL;					/* 送信元Thread Queue ID			*/

			/* ファイル名のコピー */
			ret = f_fdl_get_file_name(f_fdl_rficam_chk_code , w_file_list) ;
			if(ret != D_SYS_OK)
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Name Error(File Nothing) ") ;
				f_fdl_send_MSInotif(rtn , w_slot);
				BPF_RU_IPCM_PROCMSG_ADDRFREE((VOID*)sndMsg) ;
				return rtn ;
			}
			break ;
		case	E_FDL_FLSH_PTP_PARAM:
			if(w_slot == D_SYS_NUM0)
			{
				/* Slot0の設定 */
				sndMsg->kind = E_RRH_FILE_KIND_PTPPLL0 ;
			}
			else
			{
				sndMsg->kind = E_RRH_FILE_KIND_PTPPLL1 ;
			}
			sndMsg->thdIf.uiEventNo		= D_SYS_MSGID_NMA_FILE_WRITE_REQ ;		/* ファイル書込み要求				*/
			sndMsg->thdIf.uiDstTQueueID	= D_SYS_THDQID_PF_NMA;					/* 送信先Thread Queue ID			*/
			sndMsg->thdIf.uiSrcTQueueID	= D_SYS_THDQID_PF_FDL;					/* 送信元Thread Queue ID			*/

			/* ファイル名のコピー */
			ret = f_fdl_get_file_name(f_fdl_ptpcnf_chk_code , w_file_list) ;
			if(ret != D_SYS_OK)
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Name Error(File Nothing) ") ;
				f_fdl_send_MSInotif(rtn , w_slot);
				BPF_RU_IPCM_PROCMSG_ADDRFREE((VOID*)sndMsg) ;
				return rtn ;
			}
			break ;
		default	:
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Default Error case default :%x" , write_kind ) ;
			/* デフォルトエラー */
			f_fdl_send_MSInotif(E_FDL_ERR_OPEN , w_slot) ;
			BPF_RU_IPCM_PROCMSG_ADDRFREE((VOID*)sndMsg) ;
			return rtn ;
			break ;
	}

	/* NMA/NMASにイベント送信 */
	memcpy(&(sndMsg->name) , &w_file_list , sizeof(sndMsg->name)) ;
	memcpy(&f_fdl_inst_state_data.name[write_kind] , &w_file_list , sizeof(f_fdl_inst_state_data.name[write_kind])) ;

	if(write_kind == E_FDL_FLSH_FPGAS)
	{
		ret = BPF_RU_IPCM_MSGQ_SEND(D_SYS_THDQID_PF_NMAS ,			/* 送信先Thread Queue ID			*/
									(void*)sndMsg) ;				/* 送信Message						*/
	}
	else
	{
		ret = BPF_RU_IPCM_MSGQ_SEND(D_SYS_THDQID_PF_NMA,			/* 送信先Thread Queue ID			*/
									(void*)sndMsg) ;				/* 送信Message						*/
	}
	/*  */
	rtn = D_SYS_OK ;
	return rtn ;
}
/* @} */


/*!
 * @brief 関数機能概要:更新ファイル書込み応答結果の確認処理
 * @note  関数処理内容.
 *		-# インストール時に更新するファイルの応答結果を確認する
 *		-# 固定で正常を上位に返却する。
 * @param  *rcvMsg_p	[in]	受信メッセージ
 *		    flsh_id		[in]	更新スロット(面)
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @date 2019/03/04 KCN)上本 Create
 */
INT	f_fdl_install_check( VOID* rcvMsg_p , UINT flsh_id)
{
	FILE*			fp ;
	char			w_file_list[D_SYS_NUM256] ;
	char*			version_nm ;
	char *			tmp1 ;
	INT				rtn = D_SYS_OK ;
	int				ret = D_SYS_OK ;
	T_SYS_NMA_FILE_WRITE_RSP *	RCVMsg;
	struct stat		stat_buf ;
	UINT			stat_size = 0 ;
	UCHAR			eep_data[4];
	long			ver_num ;

	
	memset(&w_file_list , D_RRH_ZERO , sizeof(w_file_list)) ;
	memset(&stat_buf , D_RRH_ZERO , sizeof(stat_buf)) ;
	RCVMsg = (T_SYS_NMA_FILE_WRITE_RSP *)rcvMsg_p ;

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_RETURN, "f_fdl_install_check start. kind : %x Result : %x" , RCVMsg->kind , RCVMsg->result) ;

	f_fdl_inst_state_data.install_State[flsh_id] = D_FDL_INST_STOP ;

	switch(RCVMsg->result)
	{
		case	D_SYS_FLSRSLT_OK :				/* ファイル更新正常なら */
			/* ファイルサイズの書込み */
			ret = stat( f_fdl_inst_state_data.name[flsh_id], &stat_buf) ;
			if(ret == D_FDL_STAT_ERROR)
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Install Error : %x %x %x %x %x %x" ,
						f_fdl_inst_state_data.install_State[D_SYS_NUM0] , f_fdl_inst_state_data.install_State[D_SYS_NUM1] ,
						f_fdl_inst_state_data.install_State[D_SYS_NUM2] , f_fdl_inst_state_data.install_State[D_SYS_NUM3] ,
						f_fdl_inst_state_data.install_State[D_SYS_NUM4] , f_fdl_inst_state_data.install_State[D_SYS_NUM5] ) ;
				f_fdl_inst_state_data.install_State[flsh_id] = D_FDL_INST_ERROR ;
				f_fdl_send_MSInotif(E_FDL_ERR_NOFILE , f_fdl_inst_state_data.slot);
				rtn = D_SYS_NG ;
				return rtn ;
			}
			stat_size = stat_buf.st_size ;

			UCHAR *addr = NULL;
			UINT swapped_size = 0;
			UINT slot = 0;

			slot = f_fdl_inst_state_data.slot;

			switch (flsh_id){
                        case E_FDL_FLSH_OSPF:
				addr = &f_cmw_tra_inv_tbl->cmx_soft_fsize[slot][0];
                                break;
			case E_FDL_FLSH_FPGAM:
				addr = &f_cmw_tra_inv_tbl->cmx_fpga_fsize[slot][0];
                                break;
			case E_FDL_FLSH_FPGAS:
				addr = &f_cmw_tra_inv_tbl->cmx_trxfpga_fsize[slot][0];
                                break;
			case E_FDL_FLSH_RFIC_BOOT:
				addr = &f_cmw_tra_inv_tbl->cmx_rficboot_fsize[slot][0];
                                break;
			case E_FDL_FLSH_RFIC_ARM:
				addr = &f_cmw_tra_inv_tbl->cmx_rficarm_fsize[slot][0];
                                break;
			case E_FDL_FLSH_PTP_PARAM:
				addr = &f_cmw_tra_inv_tbl->cmx_ptpcond_size[slot][0];
                                break;
			default:
				return D_SYS_NG;
			}

			swapped_size = bswap_32(stat_size);

			memcpy(	addr, &swapped_size , sizeof(swapped_size));
			memcpy( &eep_data , &stat_size , sizeof(eep_data)) ;
			if(f_fdl_inst_state_data.slot == D_SYS_NUM0)
			{
				(VOID)BPF_HM_DEVC_EEPROM_WRITE(f_fdl_eepwrit_point[flsh_id].buf_size0 , &eep_data[D_SYS_NUM3] ) ;
				(VOID)BPF_HM_DEVC_EEPROM_WRITE((f_fdl_eepwrit_point[flsh_id].buf_size0 + 1) , &eep_data[D_SYS_NUM2] ) ;
				(VOID)BPF_HM_DEVC_EEPROM_WRITE((f_fdl_eepwrit_point[flsh_id].buf_size0 + 2) , &eep_data[D_SYS_NUM1] ) ;
				(VOID)BPF_HM_DEVC_EEPROM_WRITE((f_fdl_eepwrit_point[flsh_id].buf_size0 + 3) , &eep_data[D_SYS_NUM0] ) ;
			}else if(f_fdl_inst_state_data.slot == D_SYS_NUM1){
				(VOID)BPF_HM_DEVC_EEPROM_WRITE(f_fdl_eepwrit_point[flsh_id].buf_size1 , &eep_data[D_SYS_NUM3] ) ;
				(VOID)BPF_HM_DEVC_EEPROM_WRITE((f_fdl_eepwrit_point[flsh_id].buf_size1 + 1) , &eep_data[D_SYS_NUM2] ) ;
				(VOID)BPF_HM_DEVC_EEPROM_WRITE((f_fdl_eepwrit_point[flsh_id].buf_size1 + 2) , &eep_data[D_SYS_NUM1] ) ;
				(VOID)BPF_HM_DEVC_EEPROM_WRITE((f_fdl_eepwrit_point[flsh_id].buf_size1 + 3) , &eep_data[D_SYS_NUM0] ) ;
			}

			/* ファイルバージョンの書込み */
			if((fp = fopen("/tmp/installpkg","r")) == NULL)
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Error (Non installpkg)") ;
				/* ファイル名誤りでNotification通知 */
				f_fdl_inst_state_data.install_State[flsh_id] = D_FDL_INST_ERROR ;
				f_fdl_send_MSInotif(E_FDL_ERR_OPEN , f_fdl_inst_state_data.slot);
				rtn = D_SYS_NG ;
				return rtn ;
			}
			while(1)
			{
				if(fgets(w_file_list , D_SYS_NUM256 , fp) == NULL )
				{
					/* ファイルなし返却 */
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Nothing") ;
					f_fdl_inst_state_data.install_State[flsh_id] = D_FDL_INST_ERROR ;
					f_fdl_send_MSInotif(E_FDL_ERR_NOFILE , f_fdl_inst_state_data.slot);
					fclose(fp) ;
					rtn = D_SYS_NG ;
					return rtn ;
					break ;
				}

				if((strncmp(w_file_list , f_fdl_eepwrit_point[flsh_id].ver_name , strlen(f_fdl_eepwrit_point[flsh_id].ver_name))) == 0)
				{
					if((tmp1 = strstr(w_file_list , "\"")) != NULL)
					{
						tmp1++;
						if((strstr(tmp1 , "\"")) == NULL)
						{
							BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Nothing 2") ;
							/* ファイル名なし(拡張子が見つからない) */
							f_fdl_inst_state_data.install_State[flsh_id] = D_FDL_INST_ERROR ;
							f_fdl_send_MSInotif(E_FDL_ERR_NOFILE , f_fdl_inst_state_data.slot);
							fclose(fp) ;
							rtn = D_SYS_NG ;
							return rtn ;
							break ;
						}
						version_nm = strtok(tmp1, "\"");
						break ;
					}
				}
			}

			/* バージョンIDの設定 */
			ver_num = strtol( version_nm , NULL , 16) ;						/* pgr0520 */
			eep_data[D_SYS_NUM0] = (UCHAR)((ver_num & 0xFF00) >> 8) ;		/* pgr0539 */
			eep_data[D_SYS_NUM1] = (UCHAR)(ver_num & 0x00FF) ;

			if(f_fdl_inst_state_data.slot == D_SYS_NUM0)
			{
				(VOID)BPF_HM_DEVC_EEPROM_WRITE(f_fdl_eepwrit_point[flsh_id].ver_num0 , &eep_data[D_SYS_NUM0] ) ;
				(VOID)BPF_HM_DEVC_EEPROM_WRITE((f_fdl_eepwrit_point[flsh_id].ver_num0 + 1) , &eep_data[D_SYS_NUM1] ) ;
			}
			else
			{
				(VOID)BPF_HM_DEVC_EEPROM_WRITE(f_fdl_eepwrit_point[flsh_id].ver_num1 , &eep_data[D_SYS_NUM0] ) ;
				(VOID)BPF_HM_DEVC_EEPROM_WRITE((f_fdl_eepwrit_point[flsh_id].ver_num1 + 1) , &eep_data[D_SYS_NUM1] ) ;
			}
			fclose(fp) ;
			break ;
		case	D_SYS_FLSRSLT_PRAERR :				/* パラメータエラーなら */
			f_fdl_send_MSInotif(E_FDL_ERR_PARAERR , f_fdl_inst_state_data.slot);
			rtn = D_SYS_NG ;
			break ;
		case	D_SYS_FLSRSLT_OPNERR :				/* ファイルオープンエラーなら */
			f_fdl_send_MSInotif(E_FDL_ERR_OPEN , f_fdl_inst_state_data.slot);
			rtn = D_SYS_NG ;
			break ;
		case	D_SYS_FLSRSLT_ERSERR :				/* イレース失敗なら */
			f_fdl_send_MSInotif(E_FDL_ERR_ERSERR , f_fdl_inst_state_data.slot);
			rtn = D_SYS_NG ;
			break ;
		case	D_SYS_FLSRSLT_HNTERR :				/* リソース不足なら */
			f_fdl_send_MSInotif(E_FDL_ERR_BUFERR , f_fdl_inst_state_data.slot);
			rtn = D_SYS_NG ;
			break ;
		case	D_SYS_FLSRSLT_WRTERR :				/* 書込み失敗なら */
			f_fdl_send_MSInotif(E_FDL_ERR_WRTERR , f_fdl_inst_state_data.slot);
			rtn = D_SYS_NG ;
			break ;
		default	:									/*その他エラーなら */
			f_fdl_send_MSInotif(E_FDL_ERR_BUFERR , f_fdl_inst_state_data.slot);
			rtn = D_SYS_NG ;
			break ;
	}
	return rtn ;
}


/*!
 * @brief 関数機能概要:Input情報の確認(チェック)
 * @note  関数処理内容.
 *		-# Input情報(スロット番号、更新ファイル名、実行状態）をチェックする
 * @param  *rcvMsg_p	[in]		受信メッセージ
 *		    w_slot 		[out]		更新スロット(面)
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @date 2019/03/09 KCN)上本 Create
 */
INT	f_fdl_install_file_check( T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ* msgP , UINT *w_slot)
{
	INT						rtn = D_SYS_OK ;
	UINT					count ;
	UCHAR					w_file_num ;
	char					w_file_name[D_SYS_NUM256] ;
	INT						tmp_len ;

	/* スロットの確定 */ 
	tmp_len = strlen(msgP->slot_name) ;
	if(tmp_len != D_SYS_NUM5)
	{
		/*スロット番号エラー(文字数オーバ) */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR , "Input Parameter Error. Slot : %s", msgP->slot_name) ;
		rtn = D_SYS_NG ;
		return rtn ;
	}
	if(strncmp(msgP->slot_name, f_fdl_Slot_nametbl[D_SYS_NUM0] , D_SYS_NUM5 ) == 0)
	{
		/* スロット0と確定 */
		*w_slot = D_SYS_NUM0 ;
	}else {
		if(strncmp(msgP->slot_name, f_fdl_Slot_nametbl[D_SYS_NUM1] , D_SYS_NUM5 ) == 0)
		{
			/* スロット1と確定 */
			*w_slot = D_SYS_NUM1 ;
		}else {
			/* 指定スロットエラー */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR , "Input Parameter Error. Slot : %s", msgP->slot_name) ;
			rtn = D_SYS_NG ;
			return rtn ;
		}
	}

	/* 実行状態(runnning状態)チェック */
	if(f_cmw_tra_inv_tbl->cmx_bootup_face == *w_slot)
	{
		/* 運用面へのインストール */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR , "Runninng Face Install") ;
		rtn = D_SYS_NG ;
		return rtn ;
	}

	/* ファイル数の確認 */
	w_file_num = msgP->file_num ;

	/* 個別ファイル名のチェック */
	rtn = D_SYS_NG ;
	for(count = 0 ; count < w_file_num ; count++)
	{
		memset(&w_file_name , D_RRH_ZERO , sizeof(w_file_name));
		strncpy(w_file_name , msgP->file_name[count] , (sizeof(w_file_name) - 1)) ;
		/* 富士通結合ファイルの確認 */
		if(strncmp(w_file_name , "FJ" , 2) == 0)
		{
			rtn = D_SYS_OK ;
			break ;
		}
	}

	if(rtn == D_SYS_NG)
	{
		/* ファイルチェックエラー処理 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR , "Input File name Parameter Error.(Install FIle Nothing)") ;
	}
	return rtn ;
}


/*!
 * @brief 関数機能概要:Input情報の確認(チェック)
 * @note  関数処理内容.
 *		-# Input情報(スロット番号、更新ファイル名、実行状態）をチェックする
 * @param  *w_manip		[in]		受信メッセージ
 *		    w_slot 		[out]		更新スロット(面)
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @date 2019/03/09 KCN)上本 Create
 */
INT	f_fdl_get_install_mani_list( T_FDL_MANIFEST_DATA *w_manip )
{
	INT								rtn = D_SYS_OK ;
	INT								errcd = 0 ;
	bpf_xmlDocPtr					doc;
	bpf_xmlXPathContextPtr			ctx;
	bpf_xmlXPathObjectPtr			pro_obj;
	bpf_xmlXPathObjectPtr			bld_obj;
	bpf_xmlXPathObjectPtr			fil_obj;
	char							*attchar ;
	char							*vend_name ;
	char							*buildId ;
	char							temp[D_SYS_NUM512] ;

	memset(temp , D_RRH_ZERO , sizeof(temp));

	/* xmlファイルのオープン */
	rtn = BPF_RU_FACC_XML_OPEN_FILE("/tmp/manifest.xml" , &doc , &errcd);
	if(rtn != BPF_RU_FACC_COMPLETE)
	{
		/* エラー */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "xml FIle Open Error (Error Code : %x) rtn:%x", errcd , rtn) ;
		rtn = D_SYS_NG ;
		return rtn ;
	}

	/* XPathコンテキスト生成 */
	rtn = BPF_RU_FACC_XML_XPATH_CREATE_CTX (doc, &ctx, &errcd);
	if(rtn != BPF_RU_FACC_COMPLETE)
	{
		/* エラー */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "xml Context FIle Create Error (Error Code : %x) rtn:%x", errcd , rtn) ;
		(VOID)BPF_RU_FACC_XML_FREE_DOC (doc);
		rtn = D_SYS_NG ;
		return rtn ;
	}

	/* XPath検索(product情報) */
	sprintf( temp , "//products/product") ;
	rtn = BPF_RU_FACC_XML_XPATH_SEARCH_STR(ctx, temp, &pro_obj, &errcd);
	if(rtn != BPF_RU_FACC_COMPLETE)
	{
		/* エラー */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "xml First xPath Serch Error (Error Code : %x) rtn:%x", errcd , rtn) ;
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_CTX(ctx);
		(VOID)BPF_RU_FACC_XML_FREE_DOC (doc);
		rtn = D_SYS_MNG ;
		return rtn ;
	}
	/* プロダクトベンダ名の取り出し */
	rtn = BPF_RU_FACC_XML_GET_ATTRIBUTE( pro_obj , w_manip->count , "vendor" , &attchar , &errcd) ;
	if(rtn != BPF_RU_FACC_COMPLETE)
	{
		/* エラー */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "xml File Name Error (Error Code : %x)", errcd) ;
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_OBJ(pro_obj);
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_CTX(ctx);
		(VOID)BPF_RU_FACC_XML_FREE_DOC (doc);
		rtn = D_SYS_NG ;
		return rtn ;
	}
	strncpy(w_manip->product_vendor , attchar , (sizeof(w_manip->product_vendor) -1)) ;
	BPF_RU_FACC_XML_FREE_DATA(attchar) ;

	/* コード名の取り出し */
	rtn = BPF_RU_FACC_XML_GET_ATTRIBUTE( pro_obj , w_manip->count , "code" , &attchar , &errcd) ;
	if(rtn != BPF_RU_FACC_COMPLETE)
	{
		/* エラー */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "xml File Name Error (Error Code : %x)", errcd) ;
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_OBJ(pro_obj);
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_CTX(ctx);
		(VOID)BPF_RU_FACC_XML_FREE_DOC (doc);
		rtn = D_SYS_NG ;
		return rtn ;
	}
	strncpy(w_manip->code , attchar , (sizeof(w_manip->code) -1)) ;
	BPF_RU_FACC_XML_FREE_DATA(attchar) ;

	/* 装置名の取り出し */
	rtn = BPF_RU_FACC_XML_GET_ATTRIBUTE( pro_obj , w_manip->count , "name" , &vend_name , &errcd) ;
	if(rtn != BPF_RU_FACC_COMPLETE)
	{
		/* エラー */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "xml File Name Error (Error Code : %x) rtn:%x", errcd , rtn) ;
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_OBJ(pro_obj);
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_CTX(ctx);
		(VOID)BPF_RU_FACC_XML_FREE_DOC (doc);
		rtn = D_SYS_NG ;
		return rtn ;
	}
	strncpy(w_manip->name , vend_name , (sizeof(w_manip->name) -1)) ;
	BPF_RU_FACC_XML_FREE_DATA(vend_name) ;

	/* build-Idの取り出し(O-RAN: build-Id, xRAN: buildId 互換性を確保) */
	rtn = BPF_RU_FACC_XML_GET_ATTRIBUTE( pro_obj , w_manip->count , "build-Id" , &buildId , &errcd) ;
	if(rtn != BPF_RU_FACC_COMPLETE)
	{
		rtn = BPF_RU_FACC_XML_GET_ATTRIBUTE( pro_obj , w_manip->count , "buildId" , &buildId , &errcd) ;
		if(rtn != BPF_RU_FACC_COMPLETE)
		{
		/* エラー */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "xml File Name Error (Error Code : %x) rtn:%x", errcd , rtn) ;
			(VOID)BPF_RU_FACC_XML_XPATH_FREE_OBJ(pro_obj);
			(VOID)BPF_RU_FACC_XML_XPATH_FREE_CTX(ctx);
			(VOID)BPF_RU_FACC_XML_FREE_DOC (doc);
			rtn = D_SYS_NG ;
			return rtn ;
		}
	}
	strncpy(w_manip->buildId , buildId , (sizeof(w_manip->buildId) -1)) ;
	BPF_RU_FACC_XML_FREE_DATA(buildId) ;

	/* XPath検索(builds情報) */
	(VOID)BPF_RU_FACC_XML_XPATH_FREE_OBJ(pro_obj);
	memset(temp , D_RRH_ZERO , sizeof(temp)) ;
	sprintf(temp , "//builds/build") ;
	rtn = BPF_RU_FACC_XML_XPATH_SEARCH_STR(ctx, temp , &bld_obj, &errcd);
	if(rtn != BPF_RU_FACC_COMPLETE)
	{
		/* エラー */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "xml First xPath Serch Error (Error Code : %x) rtn:%x", errcd , rtn) ;
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_CTX(ctx);
		(VOID)BPF_RU_FACC_XML_FREE_DOC (doc);
		rtn = D_SYS_MNG ;
		return rtn ;
	}

	/* BUILD名の取り出し */
	rtn = BPF_RU_FACC_XML_GET_ATTRIBUTE( bld_obj , w_manip->count , "bldName" , &attchar , &errcd) ;
	if(rtn != BPF_RU_FACC_COMPLETE)
	{
		/* エラー */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "xml File Name Error (Error Code : %x) rtn:%x", errcd , rtn) ;
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_OBJ(bld_obj);
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_CTX(ctx);
		(VOID)BPF_RU_FACC_XML_FREE_DOC (doc);
		rtn = D_SYS_NG ;
		return rtn ;
	}
	strncpy(w_manip->bldName , attchar , (sizeof(w_manip->bldName) -1)) ;
	BPF_RU_FACC_XML_FREE_DATA(attchar) ;

	/* BUILD Versionの取り出し */
	rtn = BPF_RU_FACC_XML_GET_ATTRIBUTE( bld_obj , w_manip->count , "bldVersion" , &attchar , &errcd) ;
	if(rtn != BPF_RU_FACC_COMPLETE)
	{
		/* エラー */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "xml File Name Error (Error Code : %x) rtn:%x", errcd , rtn) ;
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_OBJ(bld_obj);
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_CTX(ctx);
		(VOID)BPF_RU_FACC_XML_FREE_DOC (doc);
		rtn = D_SYS_NG ;
		return rtn ;
	}
	strncpy(w_manip->bldVersion , attchar , (sizeof(w_manip->bldVersion) -1)) ;
	BPF_RU_FACC_XML_FREE_DATA(attchar) ;

	/* XPath検索(file情報) */
	(VOID)BPF_RU_FACC_XML_XPATH_FREE_OBJ(bld_obj);
	memset(temp , D_RRH_ZERO , sizeof(temp)) ;
	sprintf(temp , "//builds/build/file") ;
	rtn = BPF_RU_FACC_XML_XPATH_SEARCH_STR(ctx, temp, &fil_obj, &errcd);
	if(rtn != BPF_RU_FACC_COMPLETE)
	{
		/* エラー */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "xml First xPath Serch Error (Error Code : %x) rtn:%x", errcd , rtn) ;
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_CTX(ctx);
		(VOID)BPF_RU_FACC_XML_FREE_DOC (doc);
		rtn = D_SYS_MNG ;
		return rtn ;
	}

	/* ファイル名の取り出し */
	rtn = BPF_RU_FACC_XML_GET_ATTRIBUTE( fil_obj , w_manip->count , "fileName" , &attchar , &errcd) ;
	if(rtn != BPF_RU_FACC_COMPLETE)
	{
		/* エラー */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "xml File Name Error (Error Code : %x) rtn:%x", errcd , rtn) ;
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_OBJ(fil_obj);
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_CTX(ctx);
		(VOID)BPF_RU_FACC_XML_FREE_DOC (doc);
		rtn = D_SYS_NG ;
		return rtn ;
	}
	strncpy(w_manip->file_name , attchar , (sizeof(w_manip->file_name) -1)) ;
	BPF_RU_FACC_XML_FREE_DATA(attchar) ;
	
	/* ファイルバージョンの取り出し */
	rtn = BPF_RU_FACC_XML_GET_ATTRIBUTE( fil_obj , w_manip->count , "fileVersion" , &attchar , &errcd) ;
	if(rtn != BPF_RU_FACC_COMPLETE)
	{
		/* エラー */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "xml File Version Error (Error Code : %x) rtn:%x", errcd , rtn) ;
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_OBJ(fil_obj);
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_CTX(ctx);
		(VOID)BPF_RU_FACC_XML_FREE_DOC (doc);
		rtn = D_SYS_NG ;
		return rtn ;
	}
	strncpy(w_manip->file_ver , attchar , (sizeof(w_manip->file_ver) -1)) ;
	BPF_RU_FACC_XML_FREE_DATA(attchar) ;
	
	/* チェックサム値の取り出し */
	rtn = BPF_RU_FACC_XML_GET_ATTRIBUTE( fil_obj , w_manip->count , "checksum" , &attchar , &errcd) ;
	if(rtn != BPF_RU_FACC_COMPLETE)
	{
		/* エラー */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "xml Check Sum DataError (Error Code : %x) rtn:%x", errcd , rtn) ;
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_OBJ(fil_obj);
		(VOID)BPF_RU_FACC_XML_XPATH_FREE_CTX(ctx);
		(VOID)BPF_RU_FACC_XML_FREE_DOC (doc);
		rtn = D_SYS_NG ;
		return rtn ;
	}
	strncpy(w_manip->chksum , attchar , (sizeof(w_manip->chksum) -1)) ;
	BPF_RU_FACC_XML_FREE_DATA(attchar) ;

	/* 各種メモリの解放 */
	(VOID)BPF_RU_FACC_XML_XPATH_FREE_OBJ(fil_obj);
	(VOID)BPF_RU_FACC_XML_XPATH_FREE_CTX(ctx);
	(VOID)BPF_RU_FACC_XML_FREE_DOC (doc);
	return rtn ;
}


/*!
 * @brief 関数機能概要:EEP-ROM内のビルド情報を初期化
 * @note  関数処理内容.
 *		-# EEP-ROMのビルドVer、ビルド名を初期化(NULLコード)する
 * @param  w_slot   	[in]		更新スロット(面)
 * @return なし
 * @date 2019/03/09 KCN)上本 Create
 */
VOID	f_fdl_file_eep_init( UINT w_slot )
{
	UCHAR		eep_data = D_RRH_ZERO ;
	USHORT		w_build_ver_p = D_RRH_ZERO ;
	USHORT		w_build_name_p = D_RRH_ZERO ;
	INT			count ;

	if(w_slot == D_SYS_NUM0)
	{
		w_build_ver_p	= D_DU_EEP_FSET1_FLASH0_VER ;
		w_build_name_p	= D_DU_EEP_BUILD0_NAME ;
		memset(&f_cmw_tra_inv_tbl->cmx_build0_name , D_RRH_ZERO , sizeof(f_cmw_tra_inv_tbl->cmx_build0_name));
		memset(&f_cmw_tra_inv_tbl->cmx_f0_sver ,D_RRH_ZERO , sizeof(f_cmw_tra_inv_tbl->cmx_f0_sver));
		/* 0面のファイルサイズ/バージョンの初期化 */
		for(count = 0 ; count < E_FDL_FLSH_FILEMAX ; count++)
		{
			eep_data = 0 ;
			(VOID)BPF_HM_DEVC_EEPROM_WRITE(f_fdl_eepwrit_point[count].buf_size0 , &eep_data) ;
			(VOID)BPF_HM_DEVC_EEPROM_WRITE((f_fdl_eepwrit_point[count].buf_size0 + 1) , &eep_data) ;
			(VOID)BPF_HM_DEVC_EEPROM_WRITE((f_fdl_eepwrit_point[count].buf_size0 + 2) , &eep_data) ;
			(VOID)BPF_HM_DEVC_EEPROM_WRITE((f_fdl_eepwrit_point[count].buf_size0 + 3) , &eep_data) ;

			(VOID)BPF_HM_DEVC_EEPROM_WRITE(f_fdl_eepwrit_point[count].ver_num0 ,  &eep_data) ;
			(VOID)BPF_HM_DEVC_EEPROM_WRITE((f_fdl_eepwrit_point[count].ver_num0 + 1),  &eep_data) ;
		}
		/* Build ファイルサイズの初期化 */
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_BUILD_FILE_SIZE_0 , &eep_data) ;
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_BUILD_FILE_SIZE_0 + 1 , &eep_data) ;
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_BUILD_FILE_SIZE_0 + 2 , &eep_data) ;
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_BUILD_FILE_SIZE_0 + 3 , &eep_data) ;

	}else {
		w_build_ver_p	= D_DU_EEP_FSET1_FLASH1_VER ;
		w_build_name_p	= D_DU_EEP_BUILD1_NAME ;
		memset(&f_cmw_tra_inv_tbl->cmx_build1_name , D_RRH_ZERO , sizeof(f_cmw_tra_inv_tbl->cmx_build1_name));
		memset(&f_cmw_tra_inv_tbl->cmx_f1_sver ,D_RRH_ZERO , sizeof(f_cmw_tra_inv_tbl->cmx_f1_sver));
		/* 1面のファイルサイズ/バージョンの初期化 */
		for(count = 0 ; count < E_FDL_FLSH_FILEMAX ; count++)
		{
			eep_data = 0 ;
			(VOID)BPF_HM_DEVC_EEPROM_WRITE(f_fdl_eepwrit_point[count].buf_size1 , &eep_data) ;
			(VOID)BPF_HM_DEVC_EEPROM_WRITE((f_fdl_eepwrit_point[count].buf_size1 + 1) , &eep_data) ;
			(VOID)BPF_HM_DEVC_EEPROM_WRITE((f_fdl_eepwrit_point[count].buf_size1 + 2) , &eep_data) ;
			(VOID)BPF_HM_DEVC_EEPROM_WRITE((f_fdl_eepwrit_point[count].buf_size1 + 3) , &eep_data) ;

			(VOID)BPF_HM_DEVC_EEPROM_WRITE(f_fdl_eepwrit_point[count].ver_num1 ,  &eep_data) ;
			(VOID)BPF_HM_DEVC_EEPROM_WRITE((f_fdl_eepwrit_point[count].ver_num1 + 1),  &eep_data) ;
		}
		/* Build ファイルサイズの初期化 */
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_BUILD_FILE_SIZE_1 , &eep_data) ;
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_BUILD_FILE_SIZE_1 + 1 , &eep_data) ;
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_BUILD_FILE_SIZE_1 + 2 , &eep_data) ;
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_BUILD_FILE_SIZE_1 + 3 , &eep_data) ;
	}

	for(count = 0 ; count < D_SYS_NUM32 ; count++)
	{
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(w_build_name_p + count , &eep_data) ;
	}

	/* Buile Verの設定 */
	for(count = 0 ; count < D_SYS_NUM6 ; count++)
	{
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(w_build_ver_p + count , &eep_data) ;
	}

	/* 各ファイルのファイルサイズの初期化 */
	memset( &f_cmw_tra_inv_tbl->cmx_soft_fsize[w_slot], D_RRH_ZERO , sizeof(f_cmw_tra_inv_tbl->cmx_soft_fsize[w_slot])) ;
	memset( &f_cmw_tra_inv_tbl->cmx_fpga_fsize[w_slot], D_RRH_ZERO , sizeof(f_cmw_tra_inv_tbl->cmx_fpga_fsize[w_slot])) ;
	memset( &f_cmw_tra_inv_tbl->cmx_trxfpga_fsize[w_slot], D_RRH_ZERO , sizeof(f_cmw_tra_inv_tbl->cmx_trxfpga_fsize[w_slot])) ;
	memset( &f_cmw_tra_inv_tbl->cmx_rficboot_fsize[w_slot], D_RRH_ZERO , sizeof(f_cmw_tra_inv_tbl->cmx_rficboot_fsize[w_slot])) ;
	memset( &f_cmw_tra_inv_tbl->cmx_rficarm_fsize[w_slot], D_RRH_ZERO , sizeof(f_cmw_tra_inv_tbl->cmx_rficarm_fsize[w_slot])) ;
	memset( &f_cmw_tra_inv_tbl->cmx_ptpcond_size[w_slot], D_RRH_ZERO , sizeof(f_cmw_tra_inv_tbl->cmx_ptpcond_size[w_slot])) ;
	memset( &f_cmw_tra_inv_tbl->cmx_build_size[w_slot], D_RRH_ZERO , sizeof(f_cmw_tra_inv_tbl->cmx_build_size[w_slot])) ;

	return ;
}

/* @} */


/*!
 * @brief 関数機能概要:EEP-ROM内のビルド情報を設定
 * @note  関数処理内容.
 *		-# EEP-ROMのビルドVer、ビルド名をマニフェストリストから読み出した情報更新する
 * @param  なし
 * @return 処理結果
 *         更新データ : Buildファイル名
 *                      Build Fileバージョン
 *                      Build Fileサイズ
 * @date 2019/03/19 KCN)上本 Create
 */
INT	f_fdl_install_data_save( VOID )
{
	INT			rtn = D_SYS_OK ;
	FILE*		pkgfp ;
	INT			count ;
	USHORT		w_build_ver_p  = D_RRH_ZERO ;
	USHORT		w_build_name_p = D_RRH_ZERO ;
	USHORT		w_build_size_p = D_RRH_ZERO ;
	char		w_file_list[D_SYS_NUM256] ;
	char *		w_build_date ;
	char *		w_build_tmp ;
	long		w_date ;
	UCHAR		w_eep_date[D_SYS_NUM4] ;
	USHORT		tmp_ver = 0;

	memcpy(&tmp_ver, f_fdl_inst_build_data.build_ver, sizeof(tmp_ver));
	tmp_ver = bswap_16(tmp_ver);

	/* インストール更新面の判定 */
	if(f_fdl_inst_state_data.slot == D_SYS_NUM0)
	{
		w_build_ver_p	= D_DU_EEP_FSET1_FLASH0_VER ;
		w_build_name_p	= D_DU_EEP_BUILD0_NAME ;
		w_build_size_p  = D_DU_EEP_BUILD_FILE_SIZE_0 ;
		/* Buildファイル名の保存 */
		memcpy(&f_cmw_tra_inv_tbl->cmx_build0_name ,
				f_fdl_inst_build_data.build_name ,
				sizeof(f_cmw_tra_inv_tbl->cmx_build0_name)) ;
		/* Buildファイルバージョンの保存 */
		memcpy(&f_cmw_tra_inv_tbl->cmx_f0_sver ,
				&tmp_ver,
				sizeof(tmp_ver)) ;
	}else if(f_fdl_inst_state_data.slot == D_SYS_NUM1)
	{
		w_build_ver_p	= D_DU_EEP_FSET1_FLASH1_VER ;
		w_build_name_p	= D_DU_EEP_BUILD1_NAME ;
		w_build_size_p  = D_DU_EEP_BUILD_FILE_SIZE_1 ;
		memcpy(&f_cmw_tra_inv_tbl->cmx_build1_name ,
				f_fdl_inst_build_data.build_name ,
				sizeof(f_cmw_tra_inv_tbl->cmx_build1_name)) ;
		memcpy(&f_cmw_tra_inv_tbl->cmx_f1_sver ,
				&tmp_ver,
				sizeof(tmp_ver)) ;
	}
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "build_size       : %x", f_fdl_inst_build_data.build_size) ;
	memcpy(&f_cmw_tra_inv_tbl->cmx_build_size[f_fdl_inst_state_data.slot] ,
			&f_fdl_inst_build_data.build_size ,
			sizeof(f_cmw_tra_inv_tbl->cmx_build_size[f_fdl_inst_state_data.slot])) ;

	/* EEP-ROMにBuildファイル名を設定 */
	for(count = 0 ; count < D_SYS_NUM32 ; count++)
	{
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(w_build_name_p + count , (UCHAR *)&f_fdl_inst_build_data.build_name[count]) ;
	}

	/* EEP-ROMにBuild Verを設定 */
	for(count = 0 ; count < D_SYS_NUM2 ; count++)
	{
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(w_build_ver_p + count , &f_fdl_inst_build_data.build_ver[count]) ;
	}

	/* EEP-ROMにBuildファイルサイズを設定 */
	for(count = 0 ; count < D_SYS_NUM4 ; count++)
	{
		(VOID)BPF_HM_DEVC_EEPROM_WRITE( w_build_size_p + count , &f_fdl_inst_build_data.build_size[count]) ;
	}

	/* ビルド日付の設定 */
	memset(&w_file_list , D_RRH_ZERO , sizeof(w_file_list)) ;

	/* ファイルオープン */
	if((pkgfp = fopen("/tmp/installpkg","r")) == NULL)
	{
		/* ファイル名誤りでNotification通知 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Open Error.") ;
	}

	while(1)
	{
		/* installpkgから内容を読み出す */
		if(fgets(w_file_list , D_SYS_NUM256 , pkgfp) == NULL )				/* pgr0520 */
		{
			/* すべて読み出したら */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Nothing") ;
			fclose(pkgfp) ;
			rtn = D_SYS_NG ;
			return rtn ;
			break ;
		}

		if((strncmp(w_file_list , "PKG_DATE", strlen("PKG_DATE"))) == 0)
		{
			/* 日付情報の記載された行を検出 */
			if((w_build_tmp = strstr(w_file_list , "\"")) != NULL)
			{
				/* 日付情報の先頭を検出 */
				w_build_tmp++;
				if((strstr(w_build_tmp , "\"")) == NULL)
				{
					/* 日付情報なし */
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Nothing 2") ;
					fclose(pkgfp) ;
					rtn = D_SYS_NG ;
					return rtn ;
					break ;
				}
				w_build_date = strtok(w_build_tmp , "\"");
				if(strlen(w_build_date) != D_SYS_NUM8)									/* pgr0520 */
				{
					/* 日付情報異常 */
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "File Nothing 3") ;
					fclose(pkgfp) ;
					rtn = D_SYS_NG ;
					return rtn ;
					break ;
				}
				w_date = strtol(w_build_date , NULL , 16) ;
				w_eep_date[D_SYS_NUM0] = (UCHAR)((w_date & 0xFF000000) >> 24) ;			/* pgr0539 */
				w_eep_date[D_SYS_NUM1] = (UCHAR)((w_date & 0x00FF0000) >> 16) ;
				w_eep_date[D_SYS_NUM2] = (UCHAR)((w_date & 0x0000FF00) >> 8) ;
				w_eep_date[D_SYS_NUM3] = (UCHAR)(w_date & 0x000000FF) ;
				break ;
			}
		}
	}

	/* Build Ver(下位４バイト)の設定 */
        UCHAR *addr = NULL;
	if(f_fdl_inst_state_data.slot == D_SYS_NUM0)
	{
		/* Buildファイルバージョン(日付部分)の保存 */
		addr = (UCHAR *)&f_cmw_tra_inv_tbl->cmx_f0_sver[D_SYS_NUM1];
	}
	else
	{
		addr = (UCHAR *)&f_cmw_tra_inv_tbl->cmx_f1_sver[D_SYS_NUM1];
	}

	*addr  = w_eep_date[1];
	*(addr + 1)  = w_eep_date[0];
	*(addr + 2)  = w_eep_date[3];
	*(addr + 3)  = w_eep_date[2];

	/* EEP-ROMに日付部分の書込み */
	for(count = 0 ; count < D_SYS_NUM4 ; count++)
	{
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(w_build_ver_p + D_SYS_NUM2 + count , &w_eep_date[count]) ;
	}

	fclose(pkgfp) ;
	return rtn ;
}

/* @} */


