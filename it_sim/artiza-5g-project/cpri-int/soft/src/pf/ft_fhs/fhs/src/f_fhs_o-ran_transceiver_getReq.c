/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file   f_fhs_o-ran_transceiver_getReq.c
 *  @brief  MSI_O_RAN_TRANSCEIVER_GET_REQ (o-ran-transceiver GET Request) Recieved function
 *  @date   2019/02/15 FJT)Yoshida  create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_FHS
* @{ */

#include "f_fhs_inc.h"

static UINT	f_fhsw_log_present = D_RRH_OFF;
/* @{ */
/**
* @brief      MSI_O_RAN_INTERFACES_GET_REQ (ietf-interfaces GET Request) Recieved function
* @note       o-ran-interfaces GET Request factor judge, and table data read.\n
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
INT f_fhs_o_ran_transceiver_getReq( VOID *req, UINT w_requestId )
{
	/* 変数宣言 */
	T_MPSW_MSI_O_RAN_TRANSCEIVER_GET_CFM	cfmBody;						/* 応答データ部  */
	T_MPSW_MSI_O_RAN_TRANSCEIVER_GET_REJ	rejBody;						/* 応答データ部  */

	INT						Flag = D_RRH_OFF;
	INT						endcd;
	INT						errcd;
	struct bpf_timespec		dtime;	/* sem waittime */	


	memset( &cfmBody, 0, sizeof(T_MPSW_MSI_O_RAN_TRANSCEIVER_GET_CFM) );

	dtime.tv_sec  = 0;
	dtime.tv_nsec = BPF_RU_IPCM_WAITTIME_LOG;

	/* Take semphore of SFP monitor information */
	endcd = BPF_RU_IPCM_PROCSEM_TAKE( E_RRH_SEMID_APL_SFP_INFO, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, &dtime, &errcd );
	if(endcd != BPF_RU_IPCM_OK) {

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "SFP monitor information sem take error rtn_bpf=0x%08x errcd=0x%08x", endcd, errcd );
	}

	/* SFP読出し結果チェック	*/
	if(f_comw_transcelver_mon_tbl.info[0].present == D_COM_SFP_MON_STATE_INIT){		/* SFP#0読出し実績なし	*/

		rejBody.msgErr.errorCode = E_MPSW_ERR_CODE_HW_ACCESS_ERROR;

		if( f_comw_transcelver_mon_tbl.error_cnt != 0 ) {

			memset( rejBody.msgErr.errorDescription, 0, 32 );
			strcpy( rejBody.msgErr.errorDescription, D_FHS_MSIERR_MSG_SFP_NO_READ );
		} else {

			memset( rejBody.msgErr.errorDescription, 0, 32 );
			strcpy( rejBody.msgErr.errorDescription, D_FHS_MSIERR_MSG_SFP_READ_FAIL );
		}

		/* MSI送信処理 */
		(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_TRANSCEIVER_GET_REJ,				/* 送信するメッセージID		*/
							 w_requestId,										/* リクエストID				*/
							 sizeof(T_MPSW_MSI_O_RAN_TRANSCEIVER_GET_REJ),		/* 送信するデータサイズ		*/
							 (VOID *)&rejBody,									/* 送信データのポインタ		*/
							 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
							 D_SYS_THDQID_PF_FHS ) ;							/* pf_fhsのスレッドID		*/
		/* 処理結果表示	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-transceiver get  NG(%08x) (%s)", 
							rejBody.msgErr.errorCode, rejBody.msgErr.errorDescription );
	} else {

		memcpy( (VOID *)&cfmBody.transceiver_data[0], (VOID *)&f_comw_transcelver_mon_tbl.info[0], sizeof(T_COM_TRANSCEIVER_INFO) );

		/* SFPアクセス異常時 */
		if(f_comw_transcelver_mon_tbl.error_cnt != 0){
			if(f_fhsw_log_present == D_RRH_OFF){
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "SFP is present(%d) but SFP access is ERROR(%d).",   cfmBody.transceiver_data[0].present,f_comw_transcelver_mon_tbl.error_cnt );
				f_fhsw_log_present = D_RRH_ON;	/* ログ出すぎ防止 */
			}
			cfmBody.transceiver_data[0].present = D_RRH_OFF;
		}else{
			Flag = D_RRH_ON;
			f_fhsw_log_present = D_RRH_OFF;
		}

		/* MSI送信処理 */
		(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_TRANSCEIVER_GET_CFM,				/* 送信するメッセージID		*/
							 w_requestId,										/* リクエストID				*/
							 sizeof(T_MPSW_MSI_O_RAN_TRANSCEIVER_GET_CFM),		/* 送信するデータサイズ		*/
							 (VOID *)&cfmBody,									/* 送信データのポインタ		*/
							 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
							 D_SYS_THDQID_PF_FHS ) ;							/* pf_fhsのスレッドID		*/
	}

	/* Give semphore of SFP monitor information */
	endcd =  BPF_RU_IPCM_PROCSEM_GIVE( E_RRH_SEMID_APL_SFP_INFO, BPF_RU_IPCM_LOCK_RW, &errcd);
	if(endcd != BPF_RU_IPCM_OK) {

		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "SFP monitor information sem give error rtn_bpf=0x%08x errcd=0x%08x", endcd, errcd);
	}

	if( Flag != D_RRH_OFF ) {

		/* 報告情報表示	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-transceiver get  : [0] present             = %d",   cfmBody.transceiver_data[0].present );
		if(cfmBody.transceiver_data[0].present == D_RRH_ON){
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-transceiver get  : [0] vendor_id           = %s",   (CHAR *)cfmBody.transceiver_data[0].vendor_id );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-transceiver get  : [0] vendor_part         = %s",   (CHAR *)cfmBody.transceiver_data[0].vendor_part );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-transceiver get  : [0] vendor_rev          = %s",   (CHAR *)cfmBody.transceiver_data[0].vendor_rev );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-transceiver get  : [0] serial_no           = %s",   (CHAR *)cfmBody.transceiver_data[0].serial_no );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-transceiver get  : [0] SFF8472-comp-code   = %02x", cfmBody.transceiver_data[0].SFF8472_compliance_code );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-transceiver get  : [0] connector_type      = %02x", cfmBody.transceiver_data[0].connector_type );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-transceiver get  : [0] nominal_bitrate     = %02x", cfmBody.transceiver_data[0].nominal_bitrate );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-transceiver get  : [0] low_bitrate_margin  = %02x", cfmBody.transceiver_data[0].low_bitrate_margin );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-transceiver get  : [0] high_bitrate_margin = %02x", cfmBody.transceiver_data[0].high_bitrate_margin );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-transceiver get  : [0] rx_power_type       = %02x", cfmBody.transceiver_data[0].rx_power_type );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-transceiver get  : [0] rx_power            = %f",   cfmBody.transceiver_data[0].rx_power );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-transceiver get  : [0] tx_power            = %f",   cfmBody.transceiver_data[0].tx_power );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-transceiver get  : [0] tx_bias_cur         = %f",   cfmBody.transceiver_data[0].tx_bias_current );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-transceiver get  : [0] voltage             = %f",   cfmBody.transceiver_data[0].voltage );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-transceiver get  : [0] temperature         = %f",   cfmBody.transceiver_data[0].temperature );
		}
		/* 処理結果表示	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-transceiver get  OK!" );
	}
	return	D_RRH_OK;
}
/* @} */
