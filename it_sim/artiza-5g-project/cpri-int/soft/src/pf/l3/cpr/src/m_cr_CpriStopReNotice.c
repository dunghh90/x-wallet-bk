/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cr_CpriStopReNotice.c
 *  @brief  RE CPRI Link Stop Notice handler function
 *  @date   2015/10/06 TDI)satou create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/
/**
 * @addtogroup RRH_L3_CPR
 * @{
 */
#include "m_cm_header.h"
#include "m_cr_header.h"
#include "rrhApi_L2.h"

/**
* @brief  RE CPRI Link Stop Notice
* @note   L3リンク異常の解消やヘルスチェックの停止といった処理が必要である.
*         これらは、pf_cprに「CPRI link stop indication」を送信し、
*         光停止した結果の「CPRI State変化通知」をPFプロセスから受信した
*         場所で処理する. そのため、ここでは処理しない .
* @param  buff_adr [in] the buffer address pointer of received message
* @return None
* @date   2015/10/06 TDI)satou create
*/
VOID m_cr_CpriStopReNotice(UINT *buff_adr) {
    CMT_TSKIF_RE_CPRILNKSTOP_NTC *a_msg;
    INT ret;

    a_msg = (CMT_TSKIF_RE_CPRILNKSTOP_NTC*)buff_adr;

    /* pf_cprに「CPRI link stop indication」を送信 */
    ret = rrhApi_Cpri_Mnt_StopLink(D_RRH_PROCQUE_L3, a_msg->link_num);
    if (E_API_RCD_OK != ret) {
        cm_Assert(CMD_ASL_USELOW, ret, "rrhApi_Cpri_Mnt_StopLink NG");
    }
}

/* @} */
