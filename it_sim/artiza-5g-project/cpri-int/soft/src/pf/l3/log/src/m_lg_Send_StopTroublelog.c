/*!
 * @skip   $Id$
 * @file   m_lg_Send_StopTroublelog.c
 * @brief  the processing for RE trouble log get stop(state is log getting).
 * @date   2008/07/29 FFCS)Wuh Create for eNB-009-004.
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
 *  @brief  the processing function for RE trouble log get stop(state is log getting).
 *  @note   This function is processed as follows.
 *          1) stop sending interval timer
 *          2) send RE trouble log get stop response to REC
 *          3) change the state to using
 *  @param  *a_rcv_adr [in]  the received message
 *  @param  a_rec_typ  [in]  the type of RE system 3G or S3G
 *  @return Void.
 *  @retval -
 *  @date   2008/07/29 FFCS)Wuh Create.
 *    @date   2008/12/18  FFCS)chenwei modify for M-S3G-eNBPF-01029
 */
VOID m_lg_Send_StopTroublelog( UCHAR *a_rcv_adr, USHORT a_rec_typ )
{
    CMT_TSKIF_CPRIRCV_REOBLOGSTPREQ *a_parm_p = NULL ; /* RE trouble log get stop request */
    UINT                            a_timstop_rslt = CMD_RES_OK; /* タイマ停止結果 */
    UINT                            sigchk_rslt; /* result of checking signal */;
    INT                             errcd;
    E_RRHAPI_RCODE                  apiRet;
    
    cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_Send_StopTroublelog] enter") ;
    
    /* Get receive message address*/
    a_parm_p = (CMT_TSKIF_CPRIRCV_REOBLOGSTPREQ *)a_rcv_adr;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif

    /* Validate signal */
    sigchk_rslt = m_cm_chksig(a_parm_p->cpridat_reoblogstpreq.signal_kind,&a_rec_typ);
    if(sigchk_rslt != CMD_RES_OK)
    {
        cm_Assert(    D_RRH_LOG_AST_LV_WARNING, sigchk_rslt, "validate signal NG");
        cm_Assert(    D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Send_StopTroublelog] return") ;
    }
    
    /* 3Gの場合,「障害ログ取得中止応答(NG)」を送信(42:その他エラー)    */
    if(a_rec_typ == CMD_SYS_3G)
    {
        m_lg_send_stopres( a_rec_typ, CMD_OTHR_ERR_RE) ;
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Send_StopTroublelog") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return ;
    }
    
    /* 障害ログ転送間隔タイマ停止  */
    /*-----stop timer confirmer for S3G-----*/
    a_timstop_rslt = cm_TStop( E_RRH_TIMID_LOG_SEND_S3G, &errcd ) ;    
    
    if( CMD_RES_OK != a_timstop_rslt )
    {
        cm_Assert( D_RRH_LOG_AST_LV_ERROR, a_timstop_rslt, "cm_TStop" );
    }
    
    /* 「障害ログ取得中止応答(OK)」を送信                */
    m_lg_send_stopres( a_rec_typ, CMD_RES_OK ) ;
    
    /* API:圧縮したFHMと配下REの障害ログファイル削除Call */
    apiRet = rrhApi_Log_Mnr_TroubleLogDelete( D_RRH_PROCQUE_L3,  /* 応答待ちQueueID  */
                       0,   /* 応答待ち時間 */
                       NULL,  /* 取得データポインタ  */
                       (UCHAR*)lgw_almlogmng_tbl[a_rec_typ].logfile_name_path );  /* 作成ファイル名 */
    if( apiRet != E_API_RCD_OK )
    {
        cm_Assert(D_RRH_LOG_AST_LV_ERROR, apiRet, "rrhApi_Log_Mnr_TroubleLogDelete APIERROR(LTE)") ;
    }
        
    /* RE障害ログ取得段階状態 ← RE障害ログ取得中止処理中 */
    lgw_almlogmng_tbl[a_rec_typ].get_log_sta = LGD_GETFHMSTS_USE;
    
    cm_Assert(    D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Send_StopTroublelog] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
    
    return ;
}

/* @} */

