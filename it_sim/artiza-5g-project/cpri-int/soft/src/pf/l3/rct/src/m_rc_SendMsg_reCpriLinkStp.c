/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file   m_rc_SendMsg_reCpriLinkStp.c
 *  @brief  RE CPRI Link Stop Noticeをl3_cprに通知
 *  @date   2015/10/06 TDI)satou create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/*********************************************************************************/
/**
 * @addtogroup RRH_L3_RCT
 * @{
 */
#include "m_cm_header.h"
#include "m_cr_header.h"
#include "m_rc_header.h"
#include "f_rrh_def.h"

/*********************************************************************************/
/**
 * @brief  RE CPRI Link Stop Noticeをcprに通知
 * @return None
 * @date   2015/10/06 TDI)satou create
 */
/*********************************************************************************/
VOID m_rc_SendMsg_reCpriLinkStp(USHORT link_num) {
    UINT a_rtn;
    CMT_TSKIF_RE_CPRILNKSTOP_NTC *a_msg;

    a_rtn = cm_BReq(CMD_BUFCA_TSKIF, sizeof *a_msg, (VOID**)&a_msg);
    if (CMD_RES_OK != a_rtn) {
        cm_MAbort(CMD_ALMCD_BUFGET, "m_rc_SendMsg_reCpriLinkStp", "cm_BReq NG", a_rtn, sizeof *a_msg, 0);
        return;
    }

    a_msg->head.uiEventNo     = CMD_TSKIF_RE_CPRI_STOP_NTC;
    a_msg->head.uiDstPQueueID = D_RRH_PROCQUE_L3;
    a_msg->head.uiDstTQueueID = CMD_TSKID_CPR;
    a_msg->head.uiSrcPQueueID = D_RRH_PROCQUE_L3;
    a_msg->head.uiSrcTQueueID = CMD_TSKID_RCT;
    a_msg->head.uiLength      = sizeof *a_msg;
    a_msg->link_num           = link_num;

    /* Set running history. */
    m_cr_cm_RunHisSet(CRD_RUNHIS_FLG_SND, CMD_TSKID_RCT, CMD_TSKIF_RE_CPRI_STOP_NTC, (UCHAR*)a_msg);

    a_rtn = cm_Enter(CMD_TSKID_CPR, CMD_QRB_NORMAL, CMD_TSKIF_RE_CPRI_STOP_NTC, a_msg);
    if (CMD_RES_OK != a_rtn) {
        cm_Assert(CMD_ASL_DBGHIGH, a_rtn, "cm_Enter NG");
    }
}

/** @} */
