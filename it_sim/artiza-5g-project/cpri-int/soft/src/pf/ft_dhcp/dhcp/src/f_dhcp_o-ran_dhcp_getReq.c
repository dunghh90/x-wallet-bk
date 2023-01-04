/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_dhcp_o-ran_dhcp_getReq.c
 *  @brief  MSI_O_RAN_DHCP_GET_REQ (o-ran-dhcp GET Request) Recieved function
 *  @date   2019/02/18 FJT)Yoshida  create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_DHCP
* @{ */

#include "f_dhcp_inc.h"


/* @{ */
/**
* @brief      MSI_O_RAN_DHCP_GET_REQ (o-ran-dhcp GET Request) Recieved function
* @note       o-ran-dhcp GET Request factor judge, and table data read.\n
* @param      req         [in] the buffer address pointer of received message
* @param      w_requestId [in] the ID of MSI requested number
* @return     result of procedure
* @date       2019/02/18  FJT)Yoshida  new create
* @warning    N/A
* @FeatureID  5GDU-002-001
* @Bug_No     N/A
* @CR_No      N/A
* @TBD_No     N/A
*
*/
INT f_dhcp_o_ran_dhcp_getReq( VOID *req, UINT w_requestId )
{
	/* 変数宣言 */
	T_MPSW_MSI_O_RAN_DHCP_GET_CFM			cfmBody;						/* 応答データ部  */

	/* ==================================================================== */
	/* Set MSI Confirm parameter											*/
	/* ==================================================================== */
	memset( (UCHAR *)&cfmBody, 0, sizeof(cfmBody) );

	cfmBody.interface = f_comw_du_connection_tbl->mp.port_no;

	cfmBody.dhcpv4.dhcp_server_id                  = f_comw_du_connection_tbl->mp.ip.ip_addr_dhcps;
	cfmBody.dhcpv4.default_gateway                 = f_comw_du_connection_tbl->mp.ip.default_gateway;
	cfmBody.dhcpv4.netconf_client_id               = f_comw_du_connection_tbl->mp.ip.ip_addr_cu;
	cfmBody.m_plane_dhcp.private_enterprise_number = D_DHCP_MSI_ENTERPRISE_NUM;

	/* 報告情報表示	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-dhcp get  : dhcp_server_id            = %08x",
						cfmBody.dhcpv4.dhcp_server_id );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-dhcp get  : default_gateway           = %08x",
						cfmBody.dhcpv4.default_gateway );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-dhcp get  : netconf_client_id         = %08x",
						cfmBody.dhcpv4.netconf_client_id );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-dhcp get  : private_enterprise_number = %04x",
						cfmBody.m_plane_dhcp.private_enterprise_number );

	/* MSI送信処理 */
	(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_DHCP_GET_CFM,						/* 送信するメッセージID		*/
						 w_requestId,										/* リクエストID				*/
						 sizeof(T_MPSW_MSI_O_RAN_DHCP_GET_CFM),				/* 送信するデータサイズ		*/
						 (VOID *)&cfmBody,									/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_DHC ) ;							/* pf_dhcpのスレッドID		*/

	/* 処理結果表示	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-dhcp get OK!" );

	return	D_RRH_OK;
}
/* @} */
