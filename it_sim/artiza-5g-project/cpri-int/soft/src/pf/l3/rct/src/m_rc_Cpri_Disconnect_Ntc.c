/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_Cpri_Disconnect_Ntc.c
 *  @brief  response to RE slot info report request, carry out RE slot info report process
 *  @date   2015/08/06 FPT)Tuan create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"



/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    RE Disconnecion Request process
 *  @note     RE Disconnecion Request process
 *  @param    parm_p	[in]	parameter pointer
 *  @return   None
 *  @retval   -
 *  @date   2015/08/06 FPT)Tuan create
 */
/********************************************************************************************************************/
VOID m_rc_Cpri_Disconnect_Ntc( CMT_TSKIF_CPRIDISCONN_NTC* parm_p )
{
	if( NULL == parm_p )
	{
		/* 処理終了 */
		return;
	}

	/* (0x4007)RE Disconnecion Request送信 */
	(VOID)l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  parm_p, sizeof( CMT_TSKIF_CPRIDISCONN_REQ ) );
	
	/* 処理終了 */
	return ;
}

/* @} */

/* @} */


