/*!
 * @skip   $Id$
 * @file   m_lg_Wait_FilSndFinNtc.c
 * @brief  the processing after receiving RE trouble log get request(the state is using).
 * @date   2015/08/06 FPT)Tunghv.
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup TRA_LOG
 * @{ 
 */

#include "m_cm_header.h"
#include "m_lg_header.h"

/********************************************************************************************************************/
/**
 *  @brief  ファイルデータ送信完了報告通知処理 (対RE)
 *  @note 
 *  @param  *a_rcv_adr [in]  the received message
 *  @param  a_rec_typ  [in]  the type of RE system 3G or LTE
 *  @return Void
 *  @date    2015/08/06 FPT)Tunghv Create.
 *  @date    2015/10/27 FPT)Lay IT3問処No30対応(REに対してファイルデータ送信完了報告応答を返している。REへの応答は不要) .
 */
/********************************************************************************************************************/

VOID m_lg_Wait_FilSndFinNtc( UCHAR *a_rcv_adr, USHORT a_rec_typ )
{
                                            
    CMT_TSKIF_CPRIRCV_FILSNDFINNTC *a_parm_p = NULL ; /* ファイルデータ送信完了報告通知型ポインタ */
    USHORT                         lwReReq;
    USHORT                         lwLoop;
    USHORT                         lwFilSndFinRE[CMD_MAX_SYSNUM] = {CMD_NUM0, CMD_NUM0}; /* RE障害ログ取得状態の完了の配下RE数 */
    UINT                           retval; /* Return value of timer operation    */
    UINT                           a_timid = CMD_NUM0; /*-----timer confirmer id-------*/
    INT                            errcd = CMD_OK;
    
    cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_Wait_FilSndFinNtc] enter") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif

    /* In case buffer is Null */
    if( NULL == a_rcv_adr )
    {
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        /* end processing */
        return;
    }

    /* FHMでは、3Gの障害ログ対応外にする */
    if (a_rec_typ == CMD_SYS_3G)
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "3G system not support" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Wait_FilSndFinNtc] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return;
    }

    /* 受信メッセージをRE障害ログ取得要求型でかぶせる */
    a_parm_p = (CMT_TSKIF_CPRIRCV_FILSNDFINNTC *)a_rcv_adr ;

    if( a_parm_p->cprircv_inf.link_num < D_RRH_CPRINO_RE_MIN ||
         a_parm_p->cprircv_inf.link_num > D_RRH_CPRINO_RE_MAX )
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "Cpri Link Number not support" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Wait_FilSndFinNtc] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return;
    }
    /* 該当配下RE設定 */
    lwReReq = a_parm_p->cprircv_inf.link_num - D_RRH_CPRINO_RE_MIN;

    /*「該当配下REのRE障害ログ取得状態が配下REからのファイルデータ送信中」
     * && 「RE障害ログ取得段階状態が配下RE障害ログ取得処理中」の場合 */
    if( gw_lgw_REGetLogSta_tbl[a_rec_typ][lwReReq] == LGD_GETRESTS_FILSENDING )
    {
        /* 該当配下REのRE障害ログ取得状態 ← 完了 */
        gw_lgw_REGetLogSta_tbl[a_rec_typ][lwReReq] = LGD_GETRESTS_COMP;

	/* ファイルデータ送信完了通知応答*/
	//m_lg_FileSendFinResSendMsg(a_rec_typ, lwReReq+1, CMD_OK);//IT3問処No30対応 
    }
    else {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "Re was not a target for process" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Wait_FilSndFinNtc return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return ;
    }

    /* 全てのREをループする (16 RE)*/
    for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
    {
        /* 配下REのRE障害ログ取得状態が完了の場合 */
        if(gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] == LGD_GETRESTS_COMP)
        {
            lwFilSndFinRE[a_rec_typ] ++;
        }
    }

    /* RE障害ログ取得状態の完了の配下RE数 == ファイルデータ送信中の配下RE数 */
    if( lwFilSndFinRE[a_rec_typ] == gw_lgw_FilSndNum[a_rec_typ] ) 
    {
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s REs obslog send finsh\n", __LINE__, __FUNCTION__);
#endif
        /* time id 設定 */
        a_timid = E_RRH_TIMID_LOG_SEND_S3G; /*-----time id for LTE-----*/

        /* 「RE障害ログ送信確認タイマ」停止 */
        retval = cm_TStop(a_timid, &errcd);
        if(retval != CMD_RES_OK)
        {
            cm_Assert(CMD_ASL_DBGLOW, a_timid, "Timer stop NG");
        }
        
        /* RE障害ログ送信確認タイマ開始済フラグ←　OFF */
        gw_lgw_RETroublelogSendTimerStartFlg = D_RRH_OFF;

        /* FHM障害ログファイル作成  */
        m_lg_FhmLogFilMake(a_rec_typ);

        /* RE障害ログ取得段階状態 ← FHMログファイル作成中状態 */
        lgw_almlogmng_tbl[a_rec_typ].get_log_sta = LGD_GETFHMSTS_ACHFILCREATING;
    }
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif

    return ;
}

/* @} */

