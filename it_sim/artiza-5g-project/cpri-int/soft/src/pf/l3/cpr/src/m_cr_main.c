/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_main.c
 *  @brief  CPRI management main function
 *  @date   2008/07/18 FFCS)Wangjuan create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"  			/* CPRI管理 task header file */

/* @{ */
/**
* @brief CPRI management main function
* @note Judge the input message and dispatch it to the corresponding handle function.\n
* @param src_id [in] source task id
* @param event_no [in] event no
* @param buff_adr [in] the buffer address pointer of received message
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
*
*/

VOID m_cr_main(UINT src_id, UINT event_no, UINT * buff_adr)
{
	UINT  a_returncode;	/* function call return code */

	/************************************************************************/
	/* CPRI管理走行履歴書込み												*/
	/************************************************************************/
	m_cr_cm_RunHisSet(CRD_RUNHIS_FLG_RCV, src_id, event_no, (UCHAR*)buff_adr);

	a_returncode = m_cr_Anlz(src_id, event_no);
	if (a_returncode == CMD_OK)
	{
		 crr_mtrxtbl[crw_taskstate][crw_taskevent]( buff_adr);
	}
	else
	{
		cm_Assert(CMD_ASL_USELOW, event_no, "unknow event");
	}

	return;
}
/* @} */

/* @} */
