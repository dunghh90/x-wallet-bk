/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_dhcp_o_ran_mplane_int_getReq.c
 *  @brief  D_MSG_MSI_O_RAN_MPLANE_INT_GET_REQ (o-ran-mplane-int GET Request) Recieved function
 *  @date   2019/06/11 FJT)Ito  create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_DHCP
* @{ */

#include "f_dhcp_inc.h"


/* @{ */
/**
* @brief      MSI_O_RAN_MPLANE_INT_GET_REQ (o-ran-mplane-int GET Request) Recieved function
* @note       o-ran-dhcp GET Request factor judge, and table data read.\n
* @param      req         [in] the buffer address pointer of received message
* @param      w_requestId [in] the ID of MSI requested number
* @return     result of procedure
* @date       2019/06/11 FJT)Ito  new create
* @warning    N/A
* @FeatureID  5GDU-002-001
* @Bug_No     N/A
* @CR_No      N/A
* @TBD_No     N/A
*
*/
INT f_dhcp_o_ran_mplane_int_getReq( VOID *req, UINT w_requestId )
{
	/* 変数宣言 */
	T_MPSW_MSI_O_RAN_MPLANE_INT_GET_CFM		cfmBody;						/* 応答データ部  */

	/* ==================================================================== */
	/* Set MSI Confirm parameter											*/
	/* ==================================================================== */
	memset( (UCHAR *)&cfmBody, 0, sizeof(cfmBody) );

	cfmBody.port = f_comw_netconf_tbl->callhome_ssh_port;		/* call-home-ssh-portと同じ値を設定する 4334固定 */
	cfmBody.mplane_ipv4      = f_comw_du_connection_tbl->mp.ip.ip_addr_cu;

	/* 報告情報表示	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-mplane-int get  : call-home-ssh-port  = %08x",
						cfmBody.port );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-mplane-int get  : mplane_ipv4         = %08x",
						cfmBody.mplane_ipv4 );

	/* MSI送信処理 */
	(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_MPLANE_INT_GET_CFM,				/* 送信するメッセージID		*/
						 w_requestId,										/* リクエストID				*/
						 sizeof(T_MPSW_MSI_O_RAN_MPLANE_INT_GET_CFM),		/* 送信するデータサイズ		*/
						 (VOID *)&cfmBody,									/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_DHC ) ;							/* pf_dhcpのスレッドID		*/

	/* 処理結果表示	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-mplane-int get OK!" );

	return	D_RRH_OK;
}
/* @} */
