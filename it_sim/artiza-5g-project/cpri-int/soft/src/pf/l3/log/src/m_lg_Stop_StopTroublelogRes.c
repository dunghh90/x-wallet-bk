/*!
 * @skip   $Id$
 * @file   m_lg_Stop_StopTroublelogRes.c
 * @brief  RE障害ログ取得中止応答受信処理.
 * @date   2015/07/31 FPT)Tunghv Create.
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup TRA_LOG
 * @{ 
 */

#include "m_cm_header.h"
#include "m_lg_header.h"

/*!
 *  @brief  RE障害ログ取得中止応答受信処理.
 *  @note
 *  @param  *a_rcv_adr [in]  the received message
 *  @param  a_rec_typ  [in]  the type of RE system 3G or LTE
 *  @return VOID
 *  @retval -
 *  @date   2015/07/31 FPT)Tunghv create for FHM.
 *  @date   2015/11/08 TDIPS)Takeuchi 技説QA120
 */

VOID m_lg_Stop_StopTroublelogRes( UCHAR *a_rcv_adr, USHORT a_rec_typ )
{
    CMT_TSKIF_CPRIRCV_REOBLOGSTPRES *a_parm_p = NULL ; /* RE trouble log get stop response */
    UINT                            sigchk_rslt; /* result of checking signal */
    UINT                            retval; /* Return value of timer operation */
    USHORT                          lwLoop;
    INT                             errcd = CMD_NUM0 ;
    USHORT                          lwReRes; /* CPRI番号 */
    USHORT                          lwstopfin[CMD_MAX_SYSNUM] = {CMD_TRUE, CMD_TRUE};
    USHORT                          lwrslt_is_ng[CMD_MAX_SYSNUM] = {CMD_TRUE, CMD_TRUE};
    
    cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_Stop_StopTroublelogRes] enter") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif

    if( NULL == a_rcv_adr )
    {
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return;
    }

    /* Get receive message address*/
    a_parm_p = (CMT_TSKIF_CPRIRCV_REOBLOGSTPRES *)a_rcv_adr;
    
    /* validate signal */
    sigchk_rslt = m_cm_chksig(a_parm_p->cpridat_reoblogstpres.signal_kind,&a_rec_typ);
    if(sigchk_rslt != CMD_RES_OK)
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "validate signal NG");
    }

    /* FHMでは、3Gの障害ログ対応外にする */
    if (a_rec_typ == CMD_SYS_3G)
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "3G system not support" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Stop_StopTroublelogRes] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return;
    }

    /* 該当配下RE設定 */
    if( a_parm_p->cprircv_inf.link_num < D_RRH_CPRINO_RE_MIN ||
         a_parm_p->cprircv_inf.link_num > D_RRH_CPRINO_RE_MAX )
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "Cpri Link Number not support" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Wait_TroublelogGetRes] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return;
    }
    lwReRes = a_parm_p->cprircv_inf.link_num - D_RRH_CPRINO_RE_MIN;

    /* RE障害ログ取得状態が 障害ログ中止要求中の状態ではない*/
    if(gw_lgw_REGetLogSta_tbl[a_rec_typ][lwReRes] != LGD_STOPRESTS_REQUESTING)
    {
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Stop_StopTroublelogRes") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return ;
    }

    /* 該当配下REのRE障害ログ取得中止応答結果を保存 */
    gw_lgw_REobLogGetRes_tbl[a_rec_typ][lwReRes].result = a_parm_p->cpridat_reoblogstpres.result;

    /* RE障害ログ取得中止応答結果がNGの場合は破棄し、タイムアウト契機の再送を期待する */
    if(gw_lgw_REobLogGetRes_tbl[a_rec_typ][lwReRes].result != CMD_NML)
    {
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Stop_StopTroublelogRes") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return ;
    }

    /* RE障害ログ取得状態が未処理 */
    gw_lgw_REGetLogSta_tbl[a_rec_typ][lwReRes] = LGD_GETRESTS_NOTPROC;

    /* 全てのREをループする (16 RE)*/
    for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
    {
        /* RE障害ログ取得状態が 障害ログ中止要求中の状態*/
        if(gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] == LGD_STOPRESTS_REQUESTING)
        {
            lwstopfin[a_rec_typ] = CMD_FALSE;
        }

        /* RE障害ログ取得中止応答結果がOKの場合 */
        if(gw_lgw_REobLogGetRes_tbl[a_rec_typ][lwLoop].result == CMD_NML)
        {
            lwrslt_is_ng[a_rec_typ] = CMD_FALSE;
        }
    }

    /* 全配下REの RE障害ログ取得状態が未処理の場合 */
    if(lwstopfin[a_rec_typ] == CMD_TRUE)
    {
        /* 「RE障害ログ取得中止確認タイマ」停止 */
        retval = cm_TStop(E_RRH_TIMID_LOG_GET_STOP_LTE, &errcd);
        if(retval != CMD_RES_OK)
        {
            cm_Assert(CMD_ASL_DBGLOW, E_RRH_TIMID_LOG_GET_STOP_LTE, "Timer stop NG");
        }

        /* 全配下REのRE障害ログ取得中止応答結果がNGの場合 */
        if(lwrslt_is_ng[a_rec_typ] == CMD_TRUE)
        {
            /* RE障害ログ取得中止応答 (51:その他エラー(RE))        */
            m_lg_send_stopres( a_rec_typ, CMD_OTHR_ERR_RE ) ;

        }
        else {
            /* RE障害ログ取得中止応答 (0:正常)    */
            m_lg_send_stopres( a_rec_typ, CMD_NML ) ;
        }

		/* RE障害ログ取得中止要求の再送回数初期化 */
		gw_lgw_REGetLogStopReqCnt = 0;

    	/* RE障害ログ取得段階状態 ← 通常運用状態 */
        lgw_almlogmng_tbl[a_rec_typ].get_log_sta = LGD_GETFHMSTS_USE;
    }

#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
    return ;
}

/* @} */

