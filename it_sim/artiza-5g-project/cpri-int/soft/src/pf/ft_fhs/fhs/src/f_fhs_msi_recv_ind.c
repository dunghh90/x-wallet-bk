/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_fhs_msi_recv_ind.c
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
* @brief MSI_IETF_INTERFACES_GET_REQ (ietf-interfaces GET Request) Recieved function
* @note ietf-interfaces GET Request factor judge, and table data read.\n
* @param a_trareq_p [in] the buffer address pointer of received message
* @return None
* @date 2018/08/23 KCN)Kamimoto 5GDU-LLS対応
* @warning	N/A
* @FeatureID 5GDU-002-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
INT f_fhs_msi_recv_ind( VOID *msgP )
{
	UINT						w_messageId;
	UINT						w_requestId;
	INT							w_payloadSizePtr;
	VOID						*w_payloadPtr;

	INT							ret;


	f_com_MSIgetPayload ( msgP,					/* [in]  Receive Message pointer			*/
						  &w_messageId ,		/* [out] Receive Message ID					*/
						  &w_requestId ,		/* [out] Receive Message Request ID			*/
						  &w_payloadSizePtr ,	/* [out] Receive Message payload size		*/
						  &w_payloadPtr ) ;		/* [out] Receive Message payload Pointer	*/

	switch( w_messageId )
	{
		case D_MSG_MSI_IETF_INTERFACES_GET_REQ :				/* ietf-interfaces GET要求	*/
			ret = f_fhs_ietf_if_getReq( w_payloadPtr, w_requestId );
			break;

		case D_MSG_MSI_IETF_IP_EDIT_REQ :						/* ietf-ip GET要求			*/
			ret = f_fhs_ietf_ip_editReq( w_payloadPtr, w_requestId );
			break;

		case D_MSG_MSI_O_RAN_INTERFACE_EDIT_REQ :				/* o-ran-interface EDITE要求	*/
			ret = f_fhs_o_ran_if_editReq( w_payloadPtr, w_requestId );
			break;

		case D_MSG_MSI_O_RAN_TRANSCEIVER_GET_REQ :				/* o-ran-interface EDITE要求	*/
			ret = f_fhs_o_ran_transceiver_getReq( w_payloadPtr, w_requestId );
			break;

		case D_MSG_MSI_O_RAN_USERMGMT_EDIT_REQ :				/* o-ran-interface EDITE要求	*/
			ret = f_fhs_o_ran_usermgmt_editReq( w_payloadPtr, w_requestId );
			break;

		case D_MSG_MSI_O_RAN_SUPERVISION_NTF_START_IND :				/* o-ran-supervision開始通知	*/
			ret = f_fhs_o_ran_supervision_ind( w_payloadPtr, w_requestId );
			break;

		default :								/* その他 */
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Exception MSI Event Rcv! msgId=0x%08x", w_messageId );
			return	D_RRH_NG;
	}
	if( ret == D_RRH_NG ) {

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "MSI send response error!" );
	}
	return	D_RRH_OK;
}
/* @} */


