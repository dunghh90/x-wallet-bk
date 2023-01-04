/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_fhs_o-ran_if_getReq.c
 *  @brief  MSI_O_RAN_USERMGMT_EDIT_REQ (o-ran-usermgmt edit Request) Recieved function
 *  @date   2018/11/26 FJT)Yoshida  create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_FHS
* @{ */

#include "f_fhs_inc.h"


/* @{ */
/**
* @brief      MSI_O_RAN_USERMGMT_EDIT_REQ (o-ran-usermgmt edit Request) Recieved function
* @note       o-ran-usermgmt edit to user, password and enabled.
* @param      req         [in] the buffer address pointer of received message
* @param      w_requestId [in] the ID of MSI requested number
* @return     result of procedure
* @date       2018/11/26  FJT)Yoshida  new create
* @warning    N/A
* @FeatureID  5GDU-002-001
* @Bug_No     N/A
* @CR_No      N/A
* @TBD_No     N/A
*
*/
INT f_fhs_o_ran_usermgmt_editReq( VOID *req, UINT w_requestId )
{
	/* 変数宣言 */
	T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ	*reqBody;			/* 要求データ部	*/
	T_SYS_NMA_NETCONF_WRITE_REQ			*sendMsg;			/* Thread間IF	*/
	T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REJ	rejBody;			/* 応答データ部	*/

	INT						i;
	INT						endcd;
	INT						errcd;

	UINT					chg_cnt = 0;
	INT						ng_flag = D_RRH_OFF;

	INT						cmd_rtn;
	char					cmd_str[D_FHS_USER_CHG_CMD_LENGTH];

	struct bpf_timespec		dtime;	/* sem waittime */	


	dtime.tv_sec  = 0;
	dtime.tv_nsec = BPF_RU_IPCM_WAITTIME_LOG;

	/* ==================================================================== */
	/* Check of NETCONF user information									*/
	/* ==================================================================== */
	reqBody = (T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ *)req;

	/* 設定情報表示	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-usermgmt edit : user       = %s",   reqBody->user );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-usermgmt edit : password   = %s",   reqBody->password );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-usermgmt edit : enabled    = %08x", reqBody->enabled );

	/* Take semphore of NETCONF user information */
	endcd = BPF_RU_IPCM_PROCSEM_TAKE( E_RRH_SEMID_APL_USER_INFO, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, &dtime, &errcd );
	if(endcd != BPF_RU_IPCM_OK) {

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "NETCONF user info sem take error rtn_bpf=0x%08x errcd=0x%08x", endcd, errcd );
	}
	for( i = 0; i < f_comw_netconf_tbl->num_of_use; i++ ) {

		/* ================================================================ */
		/* Check of Changed NETCONF user information						*/
		/* ================================================================ */
		if( strcmp(f_comw_netconf_tbl->info[i].user, reqBody->user) == 0) {

			/* ============================================================ */
			/* Check of Changed password of NETCONF user information		*/
			/* ============================================================ */
			if( (reqBody->password[0] != 0)
			 && (strcmp( f_comw_netconf_tbl->info[i].password, reqBody->password ) != 0) ) {

				/* パスワード変更 (request ID保存)	*/
				memcpy( f_comw_netconf_tbl->info[i].password, reqBody->password, D_DU_NETCONF_CHAR_OF_PASS );	/* 共有メモリ保存	*/

				/* LINUXコマンド発行(password変更)	*/
				memset( cmd_str, '\0', D_FHS_USER_CHG_CMD_LENGTH );
				sprintf( cmd_str, D_FHS_USER_CHG_CMD_PASS_CHG, f_comw_netconf_tbl->info[i].user, f_comw_netconf_tbl->info[i].password );
				cmd_rtn = system ( cmd_str );

				if( cmd_rtn ==  D_RRH_MNG ) {
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "command error, 0x%08d = %s", cmd_rtn, cmd_str );
				}
				chg_cnt++;
			}
			/* ============================================================ */
			/* Check of Changed enabled/disabled of NETCONF user information*/
			/* ============================================================ */
			if( (reqBody->enabled != 0xFFFFFFFF)
			 && (f_comw_netconf_tbl->info[i].enabled != reqBody->enabled) ) {

				/* enabled変更 (request ID保存)	*/
				f_comw_netconf_tbl->info[i].enabled = reqBody->enabled;	/* 共有メモリ保存	*/

				if( f_comw_netconf_tbl->info[i].enabled == 0 ) {	/* disable	*/
					/* LINUXコマンド発行 (アカウント属性変更:disable)	*/
					memset( cmd_str, '\0', D_FHS_USER_CHG_CMD_LENGTH );
					sprintf( cmd_str, D_FHS_USER_CHG_CMD_USER_DISABLE, f_comw_netconf_tbl->info[i].user );
					cmd_rtn = system ( cmd_str );
				} else {	/* enable	*/
					/* LINUXコマンド発行 (アカウント属性変更:enable)	*/
					memset( cmd_str, '\0', D_FHS_USER_CHG_CMD_LENGTH );
					sprintf( cmd_str, D_FHS_USER_CHG_CMD_USER_ENABLE, f_comw_netconf_tbl->info[i].user );
					cmd_rtn = system ( cmd_str );
				}
				if( cmd_rtn ==  D_RRH_MNG ) {
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "command error, 0x%08d = %s", cmd_rtn, cmd_str );
				}
				chg_cnt++;
			}
			break;
		}
	}
	/* ==================================================================== */
	/* Addition of NETCONF user information									*/
	/* ==================================================================== */
	/* ユーザー登録数が最大未満、かつ、一致するユーザーなし	*/
	if( (ng_flag != D_RRH_ON) && (i == f_comw_netconf_tbl->num_of_use) ) {

		if( f_comw_netconf_tbl->num_of_use < D_DU_NETCONF_NUM_OF_USER ) {

			memcpy( &f_comw_netconf_tbl->info[i], reqBody, sizeof(T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ) );
			f_comw_netconf_tbl->num_of_use++;

			/* LINUXコマンド発行(ユーザー追加)	*/
			memset( cmd_str, '\0', D_FHS_USER_CHG_CMD_LENGTH );
			sprintf( cmd_str, D_FHS_USER_CHG_CMD_USER_ADD, f_comw_netconf_tbl->info[i].user );
			cmd_rtn = system ( cmd_str );
			if( cmd_rtn ==  D_RRH_MNG ) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "command error, 0x%08d = %s", cmd_rtn, cmd_str );
			}

			/* LINUXコマンド発行(password設定)	*/
			if( f_comw_netconf_tbl->info[i].password[0] != 0 ) {
				memset( cmd_str, '\0', D_FHS_USER_CHG_CMD_LENGTH );
				sprintf( cmd_str, D_FHS_USER_CHG_CMD_PASS_CHG, f_comw_netconf_tbl->info[i].user, f_comw_netconf_tbl->info[i].password );
				cmd_rtn = system ( cmd_str );

				if( cmd_rtn ==  D_RRH_MNG ) {
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "command error, 0x%08d = %s", cmd_rtn, cmd_str );
				}
			}

			/* LINUXコマンド発行 (アカウント属性設定)	*/
			if( f_comw_netconf_tbl->info[i].enabled == 0 ) {	/* disable	*/
				memset( cmd_str, '\0', D_FHS_USER_CHG_CMD_LENGTH );
				sprintf( cmd_str, D_FHS_USER_CHG_CMD_USER_DISABLE, f_comw_netconf_tbl->info[i].user );
				cmd_rtn = system ( cmd_str );
			} else {	/* enable	*/
				memset( cmd_str, '\0', D_FHS_USER_CHG_CMD_LENGTH );
				sprintf( cmd_str, D_FHS_USER_CHG_CMD_USER_ENABLE, f_comw_netconf_tbl->info[i].user );
				cmd_rtn = system ( cmd_str );
			}
			if( cmd_rtn ==  D_RRH_MNG ) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "command error, 0x%08d = %s", cmd_rtn, cmd_str );
			}
			chg_cnt += 3;

		} else {
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Registrate over of NETCONF -> number of user : %d, user : %s", 
								f_comw_netconf_tbl->num_of_use, f_comw_netconf_tbl->info[i].user );
			rejBody.msgErr.errorCode = E_MPSW_ERR_CODE_PARAMETER_ERROR;
			memset( rejBody.msgErr.errorDescription, 0, 32 );
			strcpy( rejBody.msgErr.errorDescription, D_FHS_MSIERR_MSG_NUM_OF_USER_OVER );
			ng_flag = D_RRH_ON;
		}
	}
	/* Give semphore of NETCONF user information */
	endcd =  BPF_RU_IPCM_PROCSEM_GIVE( E_RRH_SEMID_APL_USER_INFO, BPF_RU_IPCM_LOCK_RW, &errcd);
	if(endcd != BPF_RU_IPCM_OK) {

		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "NETCONF user info sem give error rtn_bpf=0x%08x errcd=0x%08x", endcd, errcd);
	}

	/* ==================================================================== */
	/* Send to MSI or Thread Interface										*/
	/* ==================================================================== */
	/* NG要因あり	*/
	if( ng_flag == D_RRH_ON ) {

		/* MSI送信処理 */
		(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_USERMGMT_EDIT_REJ,					/* 送信するメッセージID		*/
							 w_requestId,										/* リクエストID				*/
							 sizeof(T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REJ),			/* 送信するデータサイズ		*/
							 (VOID *)&rejBody,									/* 送信データのポインタ		*/
							 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
							 D_SYS_THDQID_PF_FHS ) ;							/* pf_fhsのスレッドID		*/
		/* 処理結果表示	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-usermgmt edit NG(%08x) (%s)", 
							rejBody.msgErr.errorCode, rejBody.msgErr.errorDescription );
	} else {

		/* MSI送信処理 */
		(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_USERMGMT_EDIT_CFM,					/* 送信するメッセージID		*/
							 w_requestId,										/* リクエストID				*/
							 D_RRH_ZERO,										/* 送信するデータサイズ		*/
							 (VOID *)D_RRH_NULL,								/* 送信データのポインタ		*/
							 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
							 D_SYS_THDQID_PF_FHS ) ;							/* pf_fhsのスレッドID		*/
		/* 処理結果表示	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-usermgmt edit OK!" );

		/* NETCONFユーザー情報 変化数チェック	*/
		if( chg_cnt != 0 ) {

			/* Thread間IF送信	*/
			/* 通信用共有メモリを取得する	*/
			endcd = BPF_RU_IPCM_PROCMSG_ADDRGET( E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別			*/
												 sizeof(T_SYS_NMA_NETCONF_WRITE_REQ),	/* Size					*/
												 (VOID **)&sendMsg,						/* msgP					*/
												 &errcd );
			/* 処理結果チェック	*/
			if( endcd != D_SYS_OK ) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed get process message address %d", errcd);
				return D_SYS_NG;
			}
			/* 初期化完了通知msg編集	*/
			sendMsg->thdIf.uiEventNo	 = D_SYS_MSGID_NMA_NETCONF_WRITE_REQ;		/* 初期化完了通知			*/
			sendMsg->thdIf.uiDstPQueueID = D_RRH_PROCQUE_PF;						/* 送信先Process Queue ID	*/
			sendMsg->thdIf.uiDstTQueueID = D_SYS_THDQID_PF_NMA;						/* 送信先Thread Queue ID	*/
			sendMsg->thdIf.uiSrcPQueueID = D_RRH_PROCQUE_PF;						/* 送信元Process Queue ID	*/
			sendMsg->thdIf.uiSrcTQueueID = D_SYS_THDQID_PF_FHS;						/* 送信元Thread Queue ID	*/
			sendMsg->thdIf.uiLength		 = sizeof(T_SYS_NMA_NETCONF_WRITE_REQ);		/* Length(Header部 + Data部)*/
			sendMsg->requestID			 = w_requestId;								/* Request ID				*/

			/* pf_nmaにT_SYS_NMA_NETCONF_WRITE_REQを送信	*/
			endcd = BPF_RU_IPCM_MSGQ_SEND( D_SYS_THDQID_PF_NMA, (void *)sendMsg );
			if( endcd != D_SYS_OK ) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed send message. %d", endcd);
				return D_SYS_NG;
			}

			/* default userが抑止未の場合 */
			if(f_comw_default_user_disabled_flag == D_RRH_OFF){
				memset(cmd_str,0,sizeof(cmd_str));
				sprintf( cmd_str, D_FHS_USER_CHG_CMD_USER_DISABLE, "ysjgmr");
				cmd_rtn = system ( cmd_str );
				if( cmd_rtn ==  D_RRH_MNG ) {
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "command error, 0x%08d = %s", cmd_rtn, cmd_str );
				}else{
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "default user disable 0x%x %s", cmd_rtn, cmd_str );
					f_comw_default_user_disabled_flag = D_RRH_ON;
				}
			}
		}
	}
	return	D_RRH_OK;
}
/* @} */


