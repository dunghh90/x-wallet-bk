/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_dhcp_vlan_write_rsp.c
 *  @brief  Reception of VLAN_WRITE_RSP from pf_nma process function
 *  @date   2019/02/18 FJT)Yoshida  create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_FHS
* @{ */

#include "f_dhcp_inc.h"

/* @{ */
/**
* @brief      Reception of VLAN_WRITE_RSP from pf_nma process function
* @note       Reception of VLAN_WRITE_RSP from pf_nma
* @param      msgP        [in] the buffer address pointer of received message
* @return     result of procedure
* @date       2019/02/18  FJT)Yoshida  new create
* @warning    N/A
* @FeatureID  5GDU-002-001
* @Bug_No     N/A
* @CR_No      N/A
* @TBD_No     N/A
*
*/
INT f_dhcp_vlan_write_rsp( VOID	*msgP )
{
	/* 変数宣言 */
	T_SYS_NMA_VLAN_WRITE_RSP			*recvMsg;			/* Thread間IF	*/


	/* ==================================================================== */
	/* Check to result of Thread interface									*/
	/* ==================================================================== */
	recvMsg = (T_SYS_NMA_VLAN_WRITE_RSP *)msgP;

	if( recvMsg->result != D_RRH_OK ) {

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "searchable-access-vlans Flash write NG, Code = %08x", recvMsg->result );

		/* Abort処理 (FLASH-ROM書込失敗)	*/
		f_com_abort( D_RRH_PROCID_PF, D_DU_ALMCD_FWNG );
	}
	return	D_RRH_OK;
}
/* @} */


