/*!
 * @skip  $ld:$
 * @file  f_fdl_install_rcv.c
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
#include "f_fdl_inc.h"

/*!
 * @brief 関数機能概要:ファイルリストの解凍処理を行う。
 * @note  関数処理内容.
 *		-# ファイルの解凍処理を実施。
 *		-# 回答結果を返却する。
 * @param  *Msg_p		[in]	受信MSIメッセージ
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @date 2019/03/04 KCN)上本 Create
 */
UINT	f_fdl_file_unzip(char*	w_file_name)
{
	FILE					*popen_fp ;
	UINT					rtn = D_SYS_OK ;
	char					cmd_str[D_SYS_NUM512] ;
	char					cmd_ret[D_SYS_NUM128] ;

	/* ワークの初期化 */
	memset(&cmd_str , D_RRH_ZERO , sizeof(cmd_str)) ;
	memset(&cmd_ret , D_RRH_ZERO , sizeof(cmd_ret)) ;

	/* Unzipコマンドの作成 */
	sprintf(cmd_str, "cd /tmp ; unzip -o /tmp/%s >/dev/null 2>/dev/null; echo $?", w_file_name );
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER , "unZIP Command Image: %s" , cmd_str) ;

	/* popen実行(unzipコマンドの実行) */
	popen_fp = popen(cmd_str,"r") ;
	if(popen_fp == NULL)
	{
		/* エラー(コマンド実行エラー) */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "unZIP Command Error (Error Code : %x)" , errno) ;
		rtn = E_FDL_ERR_UNZIP ;
	}else {
		if((fgets(cmd_ret, sizeof(cmd_ret), popen_fp)) == NULL)
		{
			/* unZIPコマンド実行エラー(返却なし) */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "unZIP Command Error (Error Info Nothing)" ) ;
			rtn = E_FDL_ERR_UNZIP ;
		}else {
			if(strncmp(cmd_ret, "0", 1 ) != 0)
			{
				/* エラー(実行エラー) */
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "unZIP Command Error (Error Code : %s", cmd_ret ) ;
				rtn = E_FDL_ERR_UNZIP ;
			}
		}
		pclose(popen_fp);
	}
	return rtn ;
}


/*!
 * @brief 関数機能概要:Abort処理を行う。
 * @note  関数処理内容.
 *		-# Abort処理を実施。				f_fdl_install_rcv() 関数Call
 *		-# 固定で正常を上位に返却する。
 * @param  *rcvMsg_p	[in]	受信メッセージ
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @date 2019/02/21 KCN)上本 Create
 */
INT	f_fdl_install_rcv(VOID* rcvMsg_p)
{
	struct stat				stat_buf ;
	long					w_build_ver ;
	INT						ret ;
	UINT					w_messageId;
	UINT					w_requestId;
	INT						w_payloadSizePtr;
	VOID *					w_payloadPtr;
	INT						rtn = D_SYS_NG ;
	UINT					w_slot ;
	char					w_file_name[D_SYS_NUM128] ;
	INT						count ;
	char					w_code_str[D_SYS_NUM16] ;
	char					w_prdct_name[D_SYS_NUM16] ;
	UCHAR					eep_data = 0 ;
	T_FDL_MANIFEST_DATA		Mani_data ;

	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ *	msgP ;

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_RETURN, "f_fdl_install_rcv start.") ;

	memset(&Mani_data , D_RRH_ZERO , sizeof(Mani_data)) ;
	memset(&stat_buf , D_RRH_ZERO , sizeof(stat_buf)) ;
	memset(w_file_name , D_RRH_ZERO , sizeof(w_file_name)) ;
	memset(w_prdct_name , D_RRH_ZERO , sizeof(w_prdct_name)) ;

	/* 内部データの初期化 */
	memset(&f_fdl_inst_build_data , D_RRH_ZERO , sizeof(f_fdl_inst_build_data)) ;
	memset(&f_fdl_inst_state_data , D_RRH_ZERO , sizeof(f_fdl_inst_state_data)) ;
	f_fdl_swact_data[D_SYS_NUM0] = D_FDL_ACT_STATE_RUN ;
	f_fdl_swact_data[D_SYS_NUM1] = D_FDL_ACT_STATE_RUN ;

	/************************************************************/
	/* Get MSI Data Function									*/
	/************************************************************/
	f_com_MSIgetPayload(	rcvMsg_p ,				/* [in]  Receive Message pointer			*/
							&w_messageId ,			/* [out] Receive Message ID					*/
							&w_requestId ,			/* [out] Receive Message Request ID			*/
							&w_payloadSizePtr ,		/* [out] Receive Message payload size		*/
							&w_payloadPtr ) ;		/* [out] Receive Message payload Pointer	*/

	msgP = (T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ *)w_payloadPtr ;

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, "Input Data Slot : %s", msgP->slot_name );
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, "Input Data file_num : %02x", msgP->file_num );
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, "Input Data file_name : %s", msgP->file_name );

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Starting Software Install(%s,%d,%s)... ", msgP->slot_name , msgP->file_num , msgP->file_name );

	/* インプットパラメータの確認 */
	rtn = f_fdl_install_file_check( msgP , &w_slot ) ;
	if(rtn == D_SYS_NG)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Input Parameter Error ") ;
		/* MSI送信(Rej)処理 */
		f_fdl_send_MSIrej( w_requestId , E_FDL_ERR_PARAERR ) ;
		return rtn ;
	}

	/* インプットパラメータの展開 */
	if(w_slot == D_SYS_NUM0)
	{
		sprintf( w_code_str , "%04x" , f_cmw_tra_inv_tbl->cmx_f0_sver[D_SYS_NUM0]) ;
	}
	else {
		sprintf( w_code_str , "%04x" , f_cmw_tra_inv_tbl->cmx_f1_sver[D_SYS_NUM0]) ;
	}

	/* プロダクト名(装置名)の読出し */
	for(count = 0 ; (count + 1) < sizeof(f_cmw_tra_inv_tbl->cmx_cardname); count += 2){
		w_prdct_name[count]
		= ((char *)f_cmw_tra_inv_tbl->cmx_cardname)[count + 1]; 
		w_prdct_name[count + 1]
		= ((char *)f_cmw_tra_inv_tbl->cmx_cardname)[count]; 
	}
	/* 末尾に' 'が入っている場合があるので削除する。"*/
	for(count = 0; count < sizeof(w_prdct_name); count ++){
		if(w_prdct_name[count] == ' '){
			w_prdct_name[count] = 0;
		}
	}

	count = 0 ;

	while(1)
	{
		/* マニフェストファイルのチェック */
		memset(&Mani_data , D_RRH_ZERO , sizeof(Mani_data)) ;
		Mani_data.count = count ;
		rtn = f_fdl_get_install_mani_list( &Mani_data ) ;
		if(rtn != D_SYS_OK)
		{
			/* MSI送信(Rej)処理 */
			f_fdl_send_MSIrej( w_requestId , E_FDL_ERR_INVALID ) ;
			return rtn ;
		}
		/* コード/装置名のチェック */
		/* 装置名のチェック */
		if( strcmp( Mani_data.name , w_prdct_name ) != 0)
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"mani_data.name:%s cmx_cardname : %s", Mani_data.name , w_prdct_name);
			/* 装置名が不一致なら 次のマニフェスト情報の読出し*/
			count++;
			continue ;
		}

		/*コードのチェック */
		if( strcmp( Mani_data.code , f_com_product_code ) != 0 )
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR," f_fdl_get_install_mani_list code:%s", Mani_data.code) ;
			/* 装置コードが不一致なら次のマニフェスト情報の読出し */
			count++;
			continue ;
		}
		/* 更新ファイルが確定 */
		break ;
	}
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, "f_fdl_install_rcv Send Install-CFM.") ;
	/* インストール要求応答の返却 */
	/* MSI送信処理 */
	(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_CFM ,					/* 送信するメッセージID		*/
						 w_requestId ,														/* リクエストID				*/
						 D_RRH_ZERO ,														/* 送信するデータサイズ		*/
						 NULL ,																/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_FDL ) ;											/* pf_eqsのスレッドID		*/

	/* チェックサムの算出 */
	rtn = f_fdl_checksum(Mani_data.file_name , Mani_data.chksum) ;
	if(rtn != D_SYS_OK)
	{
		/* Notification通知 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Check Sum Error(Mani) : %s" , Mani_data.chksum) ;
		f_fdl_send_MSInotif(E_FDL_ERR_CHKSUM , w_slot);
		return rtn ;
	}

	/* ファイルの解凍(FCF) */
	rtn = f_fdl_file_unzip( Mani_data.file_name ) ;
	if( rtn != D_SYS_OK )
	{
		/* リストファイル確認エラー */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Unzip Error ") ;
		f_fdl_send_MSInotif(E_FDL_ERR_UNZIP , w_slot);
		return rtn ;
	}

	/* Buildファイル名/Build Fileバージョン/Build Fileサイズを内部データに格納 */
	/* Buildファイル名を内部データに格納 */
	strncpy( f_fdl_inst_build_data.build_name , Mani_data.file_name , (sizeof(f_fdl_inst_build_data.build_name) -1)) ;

	/* Build Fileバージョンを内部データに格納 */
	w_build_ver = strtol(Mani_data.bldVersion , NULL , 16) ;
	f_fdl_inst_build_data.build_ver[D_SYS_NUM0] = (UCHAR)((w_build_ver & 0xFF00) >> 8 ) ;			/* pgr0539 */
	f_fdl_inst_build_data.build_ver[D_SYS_NUM1] = (UCHAR)(w_build_ver & 0x00FF);

	/* Build Fileサイズを内部データに格納 */
	sprintf( w_file_name , "/tmp/%s" , Mani_data.file_name ) ;
	ret = stat( w_file_name , &stat_buf) ;
	if(ret == D_FDL_STAT_ERROR)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "FGF File Size Read Error.") ;
		f_fdl_send_MSInotif(E_FDL_ERR_BUFERR , w_slot);
		rtn = D_SYS_NG ;
		return rtn ;
	}
	f_fdl_inst_build_data.build_size[D_SYS_NUM0] = (UCHAR)((stat_buf.st_size & 0xFF000000) >> 24) ;
	f_fdl_inst_build_data.build_size[D_SYS_NUM1] = (UCHAR)((stat_buf.st_size & 0x00FF0000) >> 16) ;
	f_fdl_inst_build_data.build_size[D_SYS_NUM2] = (UCHAR)((stat_buf.st_size & 0x0000FF00) >>  8) ;
	f_fdl_inst_build_data.build_size[D_SYS_NUM3] = (UCHAR)(stat_buf.st_size & 0x000000FF) ;

	/* 運用面の読出し */
	if(f_cmw_tra_inv_tbl->cmx_bootup_face == D_RRH_BOOT_ACT0)
	{
		eep_data = (UCHAR)D_RRH_BOOT_ACT0 ;
	}
	else {
		eep_data = (UCHAR)D_RRH_BOOT_ACT1 ;
	}
	/* 軌道面の更新 */
	(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_START_FLG , &eep_data ) ;

	/* ステータス状態をINVALIDに変更 */
	f_cmw_tra_inv_tbl->cmx_slot_status[w_slot] = D_DU_EEP_STATUS_INVALID ;
	eep_data = D_DU_EEP_STATUS_INVALID ;
	(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_SWSLOT_STATUS + w_slot , &eep_data ) ;

	/* EEPROM情報の初期化 */
	f_fdl_file_eep_init(w_slot) ;

	/* masterへの書込み要求送信 */
	ret = f_fdl_flash_write(E_FDL_FLSH_OSPF , w_slot);
	if(ret != D_SYS_OK)
	{
		/* エラー発生(バッファエラー) */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Master Write Event Send Error") ;
		rtn = D_SYS_NG ;
		return rtn ;
	}

	f_fdl_inst_state_data.install_State[E_FDL_FLSH_OSPF] = D_FDL_INST_START ;

	/* Slaveへの書込み要求送信 */
	ret = f_fdl_flash_write(E_FDL_FLSH_FPGAS , w_slot);
	if(ret != D_SYS_OK)
	{
		/* エラー発生(バッファエラー) */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Slave Write Event Send Error") ;
		rtn = D_SYS_NG ;
		return rtn ;
	}
	f_fdl_inst_state_data.install_State[E_FDL_FLSH_FPGAS] = D_FDL_INST_START ;
	f_fdl_inst_state_data.slot = w_slot ;

	f_fdl_thdInfo.thdState = D_FDL_TASK_STS_INSTALL ;

	/* SV-Busのダウンロード(ON)の設定を行う */
	f_com_SVCtl( E_DU_SVCTL_OPE_FLDL , D_DU_SVCTL_ON , D_DU_SVCTL_ON_FL_DWNLD ) ;

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_RETURN, "f_fdl_install_rcv end.") ;
	rtn = D_SYS_OK ;
	return rtn ;
}
/* @} */


