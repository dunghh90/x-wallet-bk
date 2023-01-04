/*!
 * @skip   $Id$
 * @file   m_lg_Idle_StopTroublelog.c
 * @brief  the processing for RE trouble log get stop(state is using).
 * @date   2008/07/29 FFCS)Wuh Create for eNB-009-004.
 * @date   2009/08/08 FFCS)Wangjuan modify for 
 *							  CR-00058-005(CPRI spec V1.0.9) 
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */

/*!
 * @addtogroup TRA_LOG
 * @{ 
 */

#include "m_cm_header.h"
#include "m_lg_header.h"

/*!
 *  @brief  the processing function for RE trouble log get stop(state is using).
 *  @note   This function is processed as follows.
 *          1) send RE trouble log get stop response(NG) to REC
 *          2) don't need change the state
 *  @param  *a_rcv_adr [in]  the received message
 *  @param  a_rec_typ  [in]  the type of RE system 3G or S3G
 *  @return VOID
 *  @retval -
 *  @date   2008/07/29 FFCS)Wuh Create.
 *  @date   2008/12/18 FFCS)chenwei modify for M-S3G-eNBPF-01029
 *  @date   2013/11/04 ALPHA)yokoyama modify for ZYNQ
 *  @date 2014/12/23 FFCS)qiuchh add 3G process for znqy_ppcEOL
 *  @date 2015/08/10 FPT)Tung modify for FHM 
 */

VOID m_lg_Idle_StopTroublelog( UCHAR *a_rcv_adr, USHORT a_rec_typ )
{
    CMT_TSKIF_CPRIRCV_REOBLOGSTPREQ *a_parm_p = NULL ; /* RE trouble log get stop request */
    UINT                            sigchk_rslt; /* result of checking signal */
    
    cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_Idle_StopTroublelog] enter") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif
    
    /* Get receive message address*/
    a_parm_p = (CMT_TSKIF_CPRIRCV_REOBLOGSTPREQ *)a_rcv_adr;
    
    /* validate signal */
    sigchk_rslt = m_cm_chksig(a_parm_p->cpridat_reoblogstpreq.signal_kind,&a_rec_typ);
    if(sigchk_rslt != CMD_RES_OK)
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "validate signal NG");
    }
    
    /* 3Gの場合,「障害ログ取得中止応答(NG)」を送信(42:その他エラー)    */
    if(a_rec_typ == CMD_SYS_3G)
    {
        m_lg_send_stopres( a_rec_typ, CMD_OTHR_ERR_RE) ;
    }
    /* LTEの場合,「障害ログ取得中止応答(NG)」を送信(51:処理NG(RE))    */
    else
    {
        m_lg_send_stopres( a_rec_typ, CMD_HDL_NG) ;
    }

    /* 状態遷移処理:状態遷移なし    */

    cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Idle_StopTroublelog] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
    return ;
}

/* @} */

