/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_f_fhs_user_write_rsp.c
 *  @brief  check of flash write result to ssh user information 
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
* @brief      f_fhs_user_write_rsp.c
* @note       check of flash write result to ssh user information.
* @param      req         [in] NMA_NETCONF_WRITE_RSP pointer
* @return     result of procedure
* @date       2018/11/26  FJT)Yoshida  new create
* @warning    N/A
* @FeatureID  5GDU-002-001
* @Bug_No     N/A
* @CR_No      N/A
* @TBD_No     N/A
*
*/
INT f_fhs_user_write_rsp( VOID	*msgP )
{
	/* 変数宣言 */
	T_SYS_NMA_NETCONF_WRITE_RSP			*recvMsg;			/* Thread間IF	*/


	/* ==================================================================== */
	/* Check to result of Thread interface									*/
	/* ==================================================================== */
	recvMsg = (T_SYS_NMA_NETCONF_WRITE_RSP *)msgP;

	if( recvMsg->result != D_RRH_OK ) {

		/* 処理結果表示	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "o-ran-usermgmt edit NG = %08x", recvMsg->result );

		/* Abort処理 (FLASH-ROM書込失敗)	*/
		f_com_abort( D_RRH_PROCID_PF, D_DU_ALMCD_FWNG );
	}
	return	D_RRH_OK;
}
/* @} */


