/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_fhs_ietf_ip_editReq.c
 *  @brief  MSI_IETF_IP_EDIT_REQ (ietf-ip EDIT Request) Recieved function
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
* @brief      MSI_IETF_IP_EDIT_REQ (ietf-ip EDIT Request) Recieved function
* @note       ietf-ip EDIT Request factor judge, and table data read.\n
* @param      req         [in] the buffer address pointer of received message
* @param      w_requestId [in] the ID of MSI requested number
* @return     result of procedure
* @date       2018/11/26  FJT)Yoshida  new create
* @warning    N/A
* @date       2018/11/26  FJT)Yoshida  new create
* @Bug_No     N/A
* @CR_No      N/A
* @TBD_No     N/A
*
*/
INT f_fhs_ietf_ip_editReq( VOID *req, UINT w_requestId )
{
	/* 変数宣言 */
	T_MPSW_MSI_IETF_IP_EDIT_REQ				*reqBody;						/* 要求データ部  */
	T_MPSW_MSI_IETF_IP_EDIT_REJ				rejBody;						/* 要求データ部  */

	UINT									prefix_len_mask = 0x80000000;	/* Prefix len計算マスク値  */
	UINT									prefix_len = D_RRH_ZERO;		/* Prefix length値  */

	INT										ret = D_RRH_OK;					/* 終了コード  */


	reqBody = (T_MPSW_MSI_IETF_IP_EDIT_REQ *)req;

	while(1) {
		/*** ipチェック  ***/
		if( reqBody->ip != f_comw_du_connection_tbl->mp.ip.ip_addr_du ) {
			/* 不一致 → Reject  */
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "ip-address error, cu-set=%08x / du-set=%08x",
								reqBody->ip, f_comw_du_connection_tbl->mp.ip.ip_addr_du );

			rejBody.msgErr.errorCode = E_MPSW_ERR_CODE_PARAMETER_ERROR;
			memset( rejBody.msgErr.errorDescription, 0, 32 );
			strcpy( rejBody.msgErr.errorDescription, D_FHS_MSIERR_MSG_IP );

			ret = D_RRH_NG;
			break;
		}
		/*** prefix length計算  ***/
		while( prefix_len_mask != 0 ) {

			if( (f_comw_du_connection_tbl->mp.ip.subnet_mask & prefix_len_mask) == prefix_len_mask ) {
				prefix_len++;
				prefix_len_mask = prefix_len_mask >> 1;
			} else {
				break;
			}
		}
		/*** prefix lengthチェック  ***/
		if( reqBody->prefix_length != prefix_len ) {
			/* 不一致 → Reject  */
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "prefix_len error, cu-set=%08x / du-set=%08x",
								reqBody->prefix_length , prefix_len );

			rejBody.msgErr.errorCode = E_MPSW_ERR_CODE_PARAMETER_ERROR;
			memset( rejBody.msgErr.errorDescription, 0, 32 );
			strcpy( rejBody.msgErr.errorDescription, D_FHS_MSIERR_MSG_PREFIX );

			ret = D_RRH_NG;
			break;
		}
		break;
	}

	if( ret == D_RRH_OK ) {

		/* MSI送信処理 */
		(VOID)f_com_MSIsend( D_MSG_MSI_IETF_IP_EDIT_CFM,					/* 送信するメッセージID		*/
							 w_requestId,									/* リクエストID				*/
							 D_RRH_ZERO,									/* 送信するデータサイズ		*/
							 (VOID *)D_RRH_NULL,							/* 送信データのポインタ		*/
							 D_RRH_PROCQUE_PF ,								/* PFのプロセスID			*/
							 D_SYS_THDQID_PF_FHS ) ;						/* pf_fhsのスレッドID		*/
	} else {

		/* MSI送信処理 */
		(VOID)f_com_MSIsend( D_MSG_MSI_IETF_IP_EDIT_REJ,					/* 送信するメッセージID		*/
							 w_requestId,									/* リクエストID				*/
							 sizeof(T_MPSW_MSI_IETF_IP_EDIT_REJ),			/* 送信するデータサイズ		*/
							 (VOID *)&rejBody,								/* 送信データのポインタ		*/
							 D_RRH_PROCQUE_PF ,								/* PFのプロセスID			*/
							 D_SYS_THDQID_PF_FHS ) ;						/* pf_fhsのスレッドID		*/
	}

	return	D_RRH_OK;
}
/* @} */


