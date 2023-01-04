/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_fhs_ietf_if_getReq.c
 *  @brief  MSI_IETF_INTERFACES_GET_REQ (ietf-interfaces GET Request) Recieved function
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
* @brief      MSI_IETF_INTERFACES_GET_REQ (ietf-interfaces GET Request) Recieved function
* @note       ietf-interfaces GET Request factor judge, and table data read.\n
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
INT f_fhs_ietf_if_getReq( VOID *req, UINT w_requestId )
{
	/* 変数宣言 */
	T_MPSW_MSI_IETF_INTERFACES_GET_REQ		*reqBody;						/* 要求データ部  */
	T_MPSW_MSI_IETF_INTERFACES_GET_CFM		rspBody;						/* 応答データ部  */

	UINT									reg_data;						/* レジスタ読出し値  */
	INT										ret = D_RRH_OK;					/* 終了コード  */
	UINT									portno = 0;


	reqBody = (T_MPSW_MSI_IETF_INTERFACES_GET_REQ *)req;

	/* 報告情報表示	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "ietf-interfaces get   : type                         = %d", reqBody->type );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "ietf-interfaces get   : plane                        = %d", reqBody->plane );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "ietf-interfaces get   : port                         = %d", reqBody->port );

	/* 処理結果表示	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "MSI_IETF_INTERFACES_GET_REQ recv OK!" );


	portno = reqBody->port;
	if(portno >= D_RRH_SFP_MAXNUM){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "portno error=0x%08x",portno );
		portno = 0;
	}	
	/*** enabled設定  ***/
	rspBody.enabled = D_RRH_ON;

	/*** oper-state設定  ***/
	if( reqBody->type == D_FHS_IF_O_RAN_INTERFACE_TYPE ) {
		/* O_RAN-INTERFACE → L1リンク / SFP障害をチェック  */
		ret = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV6INF, &reg_data);
		if(ret != BPF_HM_DEVC_COMPLETE) {

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_HM_DEVC_REG_READ error=0x%08x",ret );
			return	D_RRH_NG;
		}

		if( (reg_data & f_fhsr_reg_state_mask[portno][0]) != D_RRH_OFF ) {
			/* L1リンク断  */
			rspBody.oper_status = D_FHS_OPER_STATE_DOWN;

		} else if((reg_data & f_fhsr_reg_state_mask[portno][1]) != D_RRH_OFF) {
			/* SFP Unmount  */
			rspBody.oper_status = D_FHS_OPER_STATE_NOT_PRESENT;

		} else if((reg_data & f_fhsr_reg_state_mask[portno][2]) != D_RRH_OFF) {
			/* SFP Power Down  */
			rspBody.oper_status = D_FHS_OPER_STATE_LOW_LAYER_DOWN;

		} else {
			rspBody.oper_status = D_FHS_OPER_STATE_UP;
		}

	} else {

		/* O_RAN-INTERFACE-TYPE以外は、up固定	*/
		rspBody.oper_status = D_FHS_OPER_STATE_UP;
	}

	/*** phys_address設定  ***/
	memcpy( &rspBody.phys_address[0], 
			&f_comw_du_netinfo_eep_tbl->localMacAddress[0][0], 
			D_RRH_MAC_ADDR_SIZE );

	/*** discontinuity-time設定  ***/
	memcpy( &rspBody.discontinuity_time, &f_fhsw_startup_time, sizeof(T_FHS_STARTUP_TIME) );

	/*** Traffic情報  ***//* TODO:値を設定すること！ */
	rspBody.in_octets             = D_RRH_ZERO;		/**< in-octets  */
	rspBody.in_unicast_pkts       = D_RRH_ZERO;		/**< in-unicast-pkts  */
	rspBody.in_broadcast_pkts     = D_RRH_ZERO;		/**< in-broadcast-pkts  */
	rspBody.in_multicast_pkts     = D_RRH_ZERO;		/**< in-multicast-pkts  */
	rspBody.in_discards           = D_RRH_ZERO;		/**< in-discards  */
	rspBody.in_errors             = D_RRH_ZERO;		/**< in-errors  */
	rspBody.in_unknown_protos     = D_RRH_ZERO;		/**< in-unknown-protos  */
	rspBody.out_octets            = D_RRH_ZERO;		/**< out-octets  */
	rspBody.out_unicast_pkts      = D_RRH_ZERO;		/**< out-unicast-pkts  */
	rspBody.out_broadcast_pkts    = D_RRH_ZERO;		/**< out-broadcast-pkts  */
	rspBody.out_multicast_pkts    = D_RRH_ZERO;		/**< out-multicast-pkts  */
	rspBody.out_discards          = D_RRH_ZERO;		/**< out-discards  */
	rspBody.out_errors            = D_RRH_ZERO;		/**< out-errors  */

	/* MSI送信処理 */
	(VOID)f_com_MSIsend( D_MSG_MSI_IETF_INTERFACES_GET_CFM,					/* 送信するメッセージID		*/
						 w_requestId,										/* リクエストID				*/
						 sizeof(T_MPSW_MSI_IETF_INTERFACES_GET_CFM),		/* 送信するデータサイズ		*/
						 (VOID *)&rspBody,									/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_FHS ) ;							/* pf_fhsのスレッドID		*/

	/* 報告情報表示	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "ietf-interfaces get   : enabled                      = %d", rspBody.enabled );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "ietf-interfaces get   : oper-status                  = %d", rspBody.oper_status );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "ietf-interfaces get   : physical-address             = %02x:%02x:%02x:%02x:%02x:%02x", 
												rspBody.phys_address[0], rspBody.phys_address[1], rspBody.phys_address[2], 
												rspBody.phys_address[3], rspBody.phys_address[4], rspBody.phys_address[5] );
	/* 処理結果表示	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "MSI_IETF_INTERFACES_GET_CFM send OK!" );

	return	D_RRH_OK;
}
/* @} */
