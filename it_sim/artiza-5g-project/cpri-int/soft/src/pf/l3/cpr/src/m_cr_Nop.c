/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_Nop.c
 *  @brief  no-operation function 
 *  @date   2008/07/18 FFCS)Wangjuan create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"  /* CPRI管理 task header file */

/* @{ */
/**
* @brief no-operation function 
* @note  no-operation.\n
* @param buff_adr [in] the buffer address pointer of received message
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
*
*/

VOID m_cr_Nop(UINT * buff_adr)
{
	/* save current task state and input event */
	cm_Assert(CMD_ASL_USELOW, (UINT)crw_taskstate,"m_cr_Nop");
	cm_Assert(CMD_ASL_USELOW, ((CMT_TSKIF_HEAD*)buff_adr)->uiEventNo,"uiEventNo");
	cm_Assert(CMD_ASL_USELOW, ((CMT_TSKIF_HEAD*)buff_adr)->uiSignalkind,"uiSignalkind");
	cm_Assert(CMD_ASL_USELOW, ((CMT_TSKIF_HEAD*)buff_adr)->uiSrcPQueueID,"uiSrcPQueueID");
	cm_Assert(CMD_ASL_USELOW, ((CMT_TSKIF_HEAD*)buff_adr)->uiSrcTQueueID,"uiSrcTQueueID");
	if(CMD_TSKIF_L2STANTC == ((CMT_TSKIF_HEAD*)buff_adr)->uiEventNo){
		T_API_L2_LINKSTATE_CHG_NTC	*apiNtc_pt;
		apiNtc_pt =  (T_API_L2_LINKSTATE_CHG_NTC*)buff_adr;
		cm_Assert(CMD_ASL_USELOW, apiNtc_pt->data.link_num,"linkno");
		cm_Assert(CMD_ASL_USELOW, apiNtc_pt->data.state,"state");
	}
	return;
}
/* @} */

/* @{ */
/**
* @brief no-operation function 2
* @note  no-operation 2.\n
* @param buff_adr [in] the buffer address pointer of received message
* @return None
* @date 2017/02/24 FJT)koshida create
*
*/

VOID m_cr_Nop2(UINT * buff_adr)
{
	T_API_L2_LINKSTATE_CHG_NTC	*apiNtc_pt;
	
	if(CMD_TSKIF_L2STANTC == ((CMT_TSKIF_HEAD*)buff_adr)->uiEventNo)
	{
		apiNtc_pt = (T_API_L2_LINKSTATE_CHG_NTC*)buff_adr;
		/* save current task state and input event */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Tsts=%d,evnt=0x%08x,lnkNo=%d,L2sts=%d",
							 crw_taskstate, apiNtc_pt->header.uiEventNo, apiNtc_pt->data.link_num, apiNtc_pt->data.state);
	}
	else
	{
		/* save current task state and input event */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Tsts=%d,evnt=0x%08x,Sigknd",
							 crw_taskstate, ((CMT_TSKIF_HEAD*)buff_adr)->uiEventNo, ((CMT_TSKIF_HEAD*)buff_adr)->uiSignalkind);
	}
	return;
}
/* @} */

/* @} */  
