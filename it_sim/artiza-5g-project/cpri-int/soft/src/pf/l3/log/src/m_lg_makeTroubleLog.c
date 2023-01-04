/*!
 * @skip   $Id$
 * @file   m_lg_makeTroubleLog.c
 * @brief  ログスレッド 障害ログ出力処理
 * @date   2013/12/11 ALPHA)上田 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup TRA_LOG
 * @{ 
 */

#include "m_cm_header.h"
#include "m_lg_header.h"

/*!
 *  @brief  the function processing after receiving RE trouble log get request.
 *  @note   This function is processed as follows.
 *          1) send RE trouble log get response(NG)
 *          2) don't change the state
 *  @param  *a_rcv_adr [in]  the received message
 *  @param  a_rec_typ  [in]  the type of RE system 3G or S3G
 *  @return VOID
 *  @retval -
 *  @date   2013/12/11 ALPHA)ueda modiry for ZYNQ
 *  @date   2015/08/02 FPT)Tung modify for FHM.
 */

VOID  m_lg_makeTroubleLog( UCHAR *a_rcv_adr, USHORT a_rec_typ )
{
    CMT_TSKIF_CPRIRCV_REOBSLOGREQ *a_parm_p = NULL ; /* 障害ログ取得要求型ポインタ */
    UINT                          sigchk_rslt = 0; /* result of checking signal */

    BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, "[m_lg_makeTroubleLog] enter");
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif
    
    /* 受信メッセージをRE障害ログ取得要求型でかぶせる */
    a_parm_p = (CMT_TSKIF_CPRIRCV_REOBSLOGREQ *)a_rcv_adr ;

    /* validate signal */
    sigchk_rslt = m_cm_chksig(a_parm_p->cpridat_reobslogreq.signal_kind,&a_rec_typ);
    if(sigchk_rslt != CMD_RES_OK)
    {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "validate signal NG %d", a_rec_typ );
    }
    
    /* 障害ログ取得応答送信(障害ログ取得中・送信データなし)    */
    if(a_rec_typ == CMD_SYS_3G)
    {
        /* 3G (42:その他エラー(RE))  */
        m_lg_send_getres( CMD_OTHR_ERR_RE, a_rec_typ, 0 ) ;
    }
    else
    {
        /* LTE (51:その他エラー(RE)) */
        m_lg_send_getres( CMD_HDL_NG, a_rec_typ, 0 ) ;
    }
 
    /* 状態遷移処理:状態遷移なし */

    BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_RETURN, "[m_lg_makeTroubleLog] return");
}

/* @} */

