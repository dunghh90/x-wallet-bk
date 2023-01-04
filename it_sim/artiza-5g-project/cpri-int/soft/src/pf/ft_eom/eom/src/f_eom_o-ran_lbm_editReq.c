/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eom_o-ran_if_editReq.c
 *  @brief  MSI_O_RAN_INTERFACES_EDIT_REQ (o-ran-interfaces EDIT Request) Recieved function
 *  @date   2018/11/26 FJT)Yoshida  create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_EOM
* @{ */

#include "f_eom_inc.h"
#include "BPF_COM_CMD.h"

/* @{ */
/**
* @brief      MSI_O_RAN_INTERFACES_EDIT_REQ (ietf-interfaces EDIT Request) Recieved function
* @note       o-ran-interfaces EDIT Request factor judge, and table data read.\n
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
INT f_eom_o_ran_lbm_editReq( VOID *req, UINT w_requestId )
{
	/* 変数宣言 */
	T_MPSW_MSI_O_RAN_LBM_EDIT_REQ			*reqBody;						/* 要求データ部  */
	T_MPSW_MSI_O_RAN_LBM_EDIT_REJ			rejBody;						/* 応答データ部  */

	INT										id_index = 0xFFFFFFFF;
	INT										ng_flag = D_RRH_OFF;

	INT										i;
	INT										endcd;
	INT										errcd;

	unsigned long int						threadId = 0;


	reqBody  = (T_MPSW_MSI_O_RAN_LBM_EDIT_REQ *)req;

	/* 設定情報表示	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-lbm : id       = %s",   reqBody->id );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-lbm : name     = %s",   reqBody->name );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-lbm : md-level = %04x", reqBody->md_level );

	if( f_comw_lbm_mngment_tbl->num_of_use == 0 ) {

		/* ================================================================ */
		/* テーブル初回登録													*/
		/* ================================================================ */
		/* LBR/LBM管理テーブルにMSI_O_RAN_LBM_EDIT_REQパラメータをコピー	*/
		memcpy( f_comw_lbm_mngment_tbl->info[reqBody->md_level].ma_id, reqBody->id, D_EOM_LBM_STR_LEN );
		memcpy( f_comw_lbm_mngment_tbl->info[reqBody->md_level].ma_name, reqBody->name, D_EOM_LBM_STR_NAME_LEN );

		/* テーブル有効化	*/
		f_comw_lbm_mngment_tbl->info[reqBody->md_level].valid_flag = D_RRH_ON;
		f_comw_lbm_mngment_tbl->num_of_use++;

		for( i = 0; i < D_RRH_IF_FHMAX; i++ ) {
			/*	スレッドを生成 (EthernetOaM handler Fronthaule eth1)	*/
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "pf_eom1 Thread create start" ) ;
			endcd = BPF_RU_ITCM_THREAD_CREATE( &threadId,						/* Thread id				*/
											   (INT)BPF_RU_ITCM_SCHED_FIFO,		/* Scheduling policy		*/
											   (INT)D_RRH_PRIORITY_EOM_0,		/* Thread priority			*/
											   (INT)D_RRH_STACKSIZE_DEFAULT,	/* Thread stack size		*/
											   (FUNCPTR)f_eom_main_fheth,		/* Main Function Pointer	*/
											   (VOID*)&f_comw_lbm_mngment_tbl->port_info[i].port,	/* opt	*/
											   &errcd );						/* Error code				*/
			/* Thread Create結果チェック	*/
			if(endcd != BPF_RU_ITCM_OK) {

				/* Thread Create NG	*/
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "CREATE pf_eom%d NG=%08d(%08d) threadid=%ld thdPriority=%d thdStackSize=%d",
									(f_comw_lbm_mngment_tbl->port_info[i].port + 1), endcd, errcd, threadId, D_RRH_PRIORITY_EOM_0, D_RRH_STACKSIZE_DEFAULT );
			} else {

				/* Thread Create OK	*/
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO,  "CREATE pf_eom%d Complete threadid=%ld thdPriority=%d thdStackSize=%d",
									(f_comw_lbm_mngment_tbl->port_info[i].port + 1), threadId, D_RRH_PRIORITY_EOM_0, D_RRH_STACKSIZE_DEFAULT );
			}
		}
	} else {

		/* ================================================================ */
		/* テーブル追加登録													*/
		/* ================================================================ */
		/* 該当IDの使用中チェック	*/
		for( i = D_EOM_LBM_MD_LEVEL_MIN; i <= D_EOM_LBM_MD_LEVEL_MAX; i++ ) {

			if( f_comw_lbm_mngment_tbl->info[i].valid_flag == D_RRH_OFF ) {
				continue;
			}
			/* id使用中チェック	*/
			if( strcmp( f_comw_lbm_mngment_tbl->info[i].ma_id, reqBody->id ) == 0) {
				id_index = i;
				break;
			}
		}

		/* ================================================================ */
		/* テーブル追加登録													*/
		/* ================================================================ */
		/* LBR/LBM管理テーブル使用中チェック	*/
		if( f_comw_lbm_mngment_tbl->info[reqBody->md_level].valid_flag == D_RRH_OFF ) {	/* 登録先未使用	*/

			/* LBR/LBM管理テーブルの追加先にMSI_O_RAN_LBM_EDIT_REQパラメータを設定	*/
			memcpy( f_comw_lbm_mngment_tbl->info[reqBody->md_level].ma_id, 
					reqBody->id, D_EOM_LBM_STR_LEN );
			memcpy( f_comw_lbm_mngment_tbl->info[reqBody->md_level].ma_name, 
					reqBody->name, D_EOM_LBM_STR_NAME_LEN );

			/* テーブル有効化	*/
			f_comw_lbm_mngment_tbl->info[reqBody->md_level].valid_flag = D_RRH_ON;
			f_comw_lbm_mngment_tbl->num_of_use++;

			/* 該当id登録済みチェック	*/
			if( id_index != 0xFFFFFFFF ) {	/* 登録済み => 旧情報削除	*/

				/* 初期化(旧領域)	*/
				f_comw_lbm_mngment_tbl->info[id_index].valid_flag = D_RRH_OFF;
				memset( f_comw_lbm_mngment_tbl->info[id_index].ma_id,   '\0', D_EOM_LBM_STR_LEN ) ;			/* pgr0584 */
				memset( f_comw_lbm_mngment_tbl->info[id_index].ma_name, '\0', D_EOM_LBM_STR_NAME_LEN ) ;	/* pgr0584 */
				f_comw_lbm_mngment_tbl->num_of_use--;
			}

		} else {	/* 登録先使用中	*/

			if( (id_index == 0xFFFFFFFF)			/* 未登録	*/
			 || (id_index != reqBody->md_level) ) {	/* 別情報登録	*/

				ng_flag = D_RRH_ON;
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "md_level set to duplicate" );

				rejBody.msgErr.errorCode = E_MPSW_ERR_CODE_PARAMETER_ERROR;
				memset( rejBody.msgErr.errorDescription, 0, 32 );
				strcpy( rejBody.msgErr.errorDescription, D_EOM_MSIERR_MSG_MDLEVEL_DUP_ERR );

			} else {

				/* LBR/LBM管理テーブルの追加先にMSI_O_RAN_LBM_EDIT_REQパラメータを設定	*/
				memcpy( f_comw_lbm_mngment_tbl->info[reqBody->md_level].ma_name, 
						reqBody->name, D_EOM_LBM_STR_NAME_LEN );
			}
		}
	}
	if( ng_flag != D_RRH_ON ) {

		/* MSI送信処理 */
		(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_LBM_EDIT_CFM,			/* 送信するメッセージID		*/
							 w_requestId,							/* リクエストID				*/
							 D_RRH_ZERO,							/* 送信するデータサイズ		*/
							 (VOID *)D_RRH_NULL,					/* 送信データのポインタ		*/
							 D_RRH_PROCQUE_PF ,						/* PFのプロセスID			*/
							 D_SYS_THDQID_PF_EOM ) ;				/* pf_eomのスレッドID		*/
		/* 処理結果表示	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-lbm edit OK!" );

	} else {

		/* MSI送信処理 */
		(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_LBM_EDIT_REJ,			/* 送信するメッセージID		*/
							 w_requestId,							/* リクエストID				*/
							 sizeof(T_MPSW_MSI_O_RAN_LBM_EDIT_REJ),	/* 送信するデータサイズ		*/
							 (VOID *)&rejBody,						/* 送信データのポインタ		*/
							 D_RRH_PROCQUE_PF ,						/* PFのプロセスID			*/
							 D_SYS_THDQID_PF_EOM ) ;				/* pf_eomのスレッドID		*/
		/* 処理結果表示	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-lbm edit NG(%08x) (%s)", 
							rejBody.msgErr.errorCode, rejBody.msgErr.errorDescription );
	}
	return	D_RRH_OK;
}
/* @} */


