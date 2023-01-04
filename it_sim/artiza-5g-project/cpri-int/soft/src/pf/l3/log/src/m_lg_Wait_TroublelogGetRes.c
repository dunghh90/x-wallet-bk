/*!
 * @skip   $Id$
 * @file   m_lg_Wait_TroublelogGetRes.c
 * @brief  RE障害ログ取得応答受信処理.
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
 *  @brief  RE障害ログ取得応答受信処理 (対RE)
 *  @note 
 *  @param  *a_rcv_adr [in]  the received message
 *  @param  a_rec_typ  [in]  the type of RE system 3G or LTE
 *  @return VOID
 *  @Bug_No  M-RRU-ZSYS-01979
 *  @date    2015/08/06 FPT)Tunghv.
 *  @date    2015/11/11 FPT)Lay M-RRU-ZSYS-01979 IT2問処番号173の対処 タイマイベントを修正。配下REの障害ログ取得待ち時間を再計算
 */
/********************************************************************************************************************/
VOID m_lg_Wait_TroublelogGetRes( UCHAR *a_rcv_adr, USHORT a_rec_typ )
{
                                            
    CMT_TSKIF_CPRIRCV_REOBSLOGRES *a_parm_p = NULL; /* 障害ログ取得要求型ポインタ */
    UINT                          sigchk_rslt; /*result of checking signal */
    UINT                          retval; /* Return value of timer operation */
    UINT                          a_timstart_result = CMD_RES_OK; /* タイマ起動結果 */
    UINT                          a_timid           = CMD_NUM0; /*-----timer confirmer id-------*/
    UINT                          a_timeoutntc      = CMD_NUM0; /*-----timeout ntc--------------*/
    UINT                          a_timevalue       = CMD_NUM0; /*-----timeout value--------------*/
    UINT                          luiFilSizeTotal = CMD_NUM0;
    USHORT                        lwLoop;
    USHORT                        lwResRslt_chk = CMD_NG; /* RE障害ログ取得応答結果のOKがあるかどうかをチェック */
    USHORT                        lwRESts_chk = CMD_FALSE; /* 配下REがRE障害ログ取得応答中かどうかをチェック */
    USHORT                        lwReRes;
    INT                           errcd = CMD_NUM0 ;

    cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_Wait_TroublelogGetRes] enter") ;

    /* In case buffer is Null */
    if( NULL == a_rcv_adr )
    {
        /* end processing */
        return;
    }

    /* 受信メッセージをRE障害ログ取得要求型でかぶせる */
    a_parm_p = (CMT_TSKIF_CPRIRCV_REOBSLOGRES *)a_rcv_adr ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s Response result:0x%x start\n", __LINE__, __FUNCTION__, a_parm_p->cpridat_reobslogres.result);
#endif

    /* validate signal */
    sigchk_rslt = m_cm_chksig(a_parm_p->cpridat_reobslogres.signal_kind,&a_rec_typ);
    if(sigchk_rslt != CMD_RES_OK)
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "validate signal NG");
    }

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
    /* 該当配下RE設定 */
    lwReRes = a_parm_p->cprircv_inf.link_num - D_RRH_CPRINO_RE_MIN;

    /* FHMでは、3Gの障害ログ対応外にする */
    if (a_rec_typ == CMD_SYS_3G)
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "3G system not support" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Wait_TroublelogGetRes] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return;
    }

    /* 該当配下REのRE障害ログ取得状態が取得要求中ではないの場合 */
    if (gw_lgw_REGetLogSta_tbl[a_rec_typ][lwReRes] != LGD_GETRESTS_REQUESTING)
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, lwReRes, "Re is not or still not a target of GetTroubleLog" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Wait_TroublelogGetRes] return" );
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return ;
    }

    a_timid = E_RRH_TIMID_LOG_SEND_S3G;          /*-----time id for LTE-----*/
    a_timeoutntc = CMD_TIMEOUTNTC_LOGSNDCF;   /*-----timeout event for LTE-----*/   
	/* 2015/11/11: (REからのデータ送信間隔(実際280msぐらい) * 最大送信回数(1MB/10KB=100) + FHMでのデータ保存処理時間) からみる実際試験時の時間が34sec程度*/ 
	a_timevalue =  CMD_TIMVAL_RE_TROUBLELOG_SND_CONF*400; /* 80sec (最大16の配下REの障害ログ取得できるように待つ時間)*/   

    /* 応答結果がOK && トータルデータサイズ < 1MBの場合 */
    if ((a_parm_p->cpridat_reobslogres.result == CMD_NML) &&
         (a_parm_p->cpridat_reobslogres.datsize <= LGD_MAX_COMPDAT))
    {
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s RE response result is OK\n", __LINE__, __FUNCTION__);
#endif
        /* 該当配下REのRE障害ログ取得応答を保存 */
        gw_lgw_REobLogGetRes_tbl[a_rec_typ][lwReRes].result = a_parm_p->cpridat_reobslogres.result;
        gw_lgw_REobLogGetRes_tbl[a_rec_typ][lwReRes].datsize = a_parm_p->cpridat_reobslogres.datsize;
        cm_MemCpy( &gw_lgw_REobLogGetRes_tbl[a_rec_typ][lwReRes].filename[CMD_NUM0], 
                   &a_parm_p->cpridat_reobslogres.filename[CMD_NUM0],
                   (sizeof(UCHAR) * LGD_MAX_FILNAME_NUM) ); 

        /* RE障害ログ送信確認タイマ開始済かどうかをチェック */
        if( gw_lgw_RETroublelogSendTimerStartFlg == D_RRH_OFF ) 
        {
            /* 「RE障害ログ送信確認タイマ」開始    */
            a_timstart_result = cm_TStat( a_timid, /* TimerID */
                                          a_timevalue, /* Timer値 */
                                          CMD_TIMMD_SINGLE,    /* 1 Short Timer */
                                          a_timeoutntc,        /* TO時EventNo */
                                          D_RRH_THDQID_L3_LOG, /* TO時送信元ThreadQID */
                                          &errcd ) ;

            if( a_timstart_result != BPF_RU_HRTM_COMPLETE ){
                cm_Assert(D_RRH_LOG_AST_LV_ERROR, a_timstart_result, "[m_lg_Wait_TroublelogGetRes] cm_TStat Error") ;
            }

            /* RE障害ログ送信確認タイマ開始済フラグ　←　ON  */
            gw_lgw_RETroublelogSendTimerStartFlg = D_RRH_ON; 
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s Start Timer <timid>0x%x <value>%d Flag ON\n", __LINE__, __FUNCTION__, E_RRH_TIMID_LOG_SEND_S3G, a_timevalue);
#endif
        }

        /* 該当配下REのRE障害ログ取得状態 ←　配下REからのファイルデータ送信中 */
        gw_lgw_REGetLogSta_tbl[a_rec_typ][lwReRes] = LGD_GETRESTS_FILSENDING;

        /* ファイルデータ送信中の配下RE数 ← +1 */
        gw_lgw_FilSndNum[a_rec_typ] ++;
    }
    else {
        /* 該当配下REのRE障害ログ取得状態 ←　未処理 */
        gw_lgw_REGetLogSta_tbl[a_rec_typ][lwReRes] = LGD_GETRESTS_NOTPROC;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s LGD_GETRESTS_NOTPROC\n", __LINE__, __FUNCTION__);
#endif
    }

    /* 全てのREをループする (16 RE)*/
    for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
    {
        /* RE障害ログ取得応答結果のOKの配下REがあるかどうかをチェック */
        if( ( gw_lgw_REobLogGetRes_tbl[a_rec_typ][lwLoop].result == CMD_NML ) && 
            ( lwResRslt_chk == CMD_NG ) )
        {
            lwResRslt_chk = CMD_OK;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s ReobLogGetRes result have OK\n", __LINE__, __FUNCTION__);
#endif
        }
        /* RE障害ログ取得状態が取得要求中の配下REがあるかどうかをチェック */
        if( ( gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] == LGD_GETRESTS_REQUESTING ) &&
            ( lwRESts_chk == CMD_FALSE ) )
        {
            lwRESts_chk = CMD_TRUE;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s Have RE that still not send ReobLogGetRes\n", __LINE__, __FUNCTION__);
#endif
        }
        /* トータルサイズ計算 */
        luiFilSizeTotal += gw_lgw_REobLogGetRes_tbl[a_rec_typ][lwLoop].datsize;
    }

    /* RE障害ログ取得状態が取得要求中の配下REがない場合 */
    if( lwRESts_chk == CMD_FALSE )
    {
        /* 「RE障害ログ取得確認タイマ」停止 */
        retval = cm_TStop(E_RRH_TIMID_LOG_GET_LTE, &errcd);
        if(retval != CMD_RES_OK)
        {
            cm_Assert(CMD_ASL_DBGLOW, a_timid, "Timer stop NG");
        }    

        /* 全配下REのRE障害ログ取得応答結果がNGの場合 */
        if(lwResRslt_chk == CMD_NG)
        {
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s All of ReobLogGetRes result have NG!!\n", __LINE__, __FUNCTION__);
#endif
            /* RE障害ログ取得応答 (0:正常)        */
            m_lg_send_getres( CMD_OK, a_rec_typ, LGD_MAX_COMPDAT ) ;

            /* 障害ログファイル作成 (配下REの障害ログ取得応答が全てNGなので、
                                     FHM自身の障害ログを作成するだけ) */
            m_lg_FhmLogFilMake(a_rec_typ);

            /* 状態遷移処理:通常運用状態→ログファイル作成中状態    */
            lgw_almlogmng_tbl[a_rec_typ].get_log_sta = LGD_GETFHMSTS_ACHFILCREATING ;
        
        }
        else 
        {
            /* 「トータルサイズ」が17MBを超える場合 */
            if( luiFilSizeTotal > LGD_MAX_FHMRECOMPDAT )
            {
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s FilSizeTotal over 17MB\n", __LINE__, __FUNCTION__);
#endif
                /* RE障害ログ取得応答 (NG) */
                m_lg_send_getres( CMD_HDL_NG, a_rec_typ, 0 );

                /* 状態遷移処理: 通常運用状態 */
                lgw_almlogmng_tbl[a_rec_typ].get_log_sta = LGD_GETFHMSTS_USE;
    
            }
            else {
                /* RE障害ログ取得応答 (0:正常) */
                m_lg_send_getres( CMD_OK, a_rec_typ, luiFilSizeTotal + LGD_MAX_COMPDAT );
            }
        }
    }
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif

    return ;
}

/* @} */

