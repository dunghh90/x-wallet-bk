/*!
 * @skip   $Id$
 * @file   m_lg_Send_ToTimer.c
 * @brief  the processing for time out notification(state is log getting).
 * @date   2008/07/29 FFCS)Wuh Create for eNB-009-004.
 * @date   2009/03/05  FFCS)Wuh modify for M-S3G-eNBPF-01471
 *                        stop writing FLASH after receiving CPRI message
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-
 */

/*!
 * @addtogroup TRA_LOG
 * @{ 
 */

#include "m_cm_header.h"
#include "m_lg_header.h"

/*!
 *  @brief  the processing function for time out notification(state is log sending).
 *  @note   This function is processed as follows.
 *          1) dive the trouble log data
 *          2) judge whether the log sending is end
 *             .end:send the RE trouble log get complete notice to REC,and change the state to using
 *                  call API [trouble log delete]
 *             .not end:start the sending interval timer 
 *  @param  *a_rcv_adr [in]  the received message
 *  @param  a_rec_typ  [in]  the type of RE system 3G or S3G
 *  @return VOID
 *  @retval -
 *  @date   2008/07/29 FFCS)Wuh Create.
 *  @date   2011/05/12 FJT)Koshida modify for M-S3G-eNBPF-04042
 *  @date   2013/11/04 ALPHA)yokoyama modify for ZYNQ
 *  @date   2015/08/06 FPT)Tunghv modify for FHM.
 *  @date   2016/02/05 TDI)satou 転送中にリンク断した場合、転送を中止するように
 */

VOID m_lg_Send_ToTimer( UCHAR *a_rcv_adr, USHORT a_rec_typ )
{

    UINT                a_timstart_result = CMD_RES_OK ;  /* タイマ起動結果       */
    UINT                a_snd_startadr    = CMD_NUM0 ;    /* 障害ログ転送開始アドレス */
    UINT                a_snd_size        = CMD_NUM0 ;    /* 送信サイズ               */
    UINT                a_snd_size_calc   = CMD_NUM0 ;    /* 計算機送信サイズ */
    USHORT              a_err_inf         = CMD_NUM1 ;    /* ERR制御レジスタ情報格納変数 */
    UINT                a_timid           = CMD_NUM0 ;    /*-----timer confirmer id-----*/
    UINT                a_timeoutntc      = CMD_NUM0 ;   /*-----timeout ntc-----*/
    E_RRHAPI_RCODE      apiRet;
    INT                 errcd;
    USHORT              lwLoop;
    
    cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_Send_ToTimer] enter") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif
    
    /***** 転送中にリンク断した場合は転送を中止する *****/
    /* CPRIリンクERR設定レジスタ情報Read                */
    a_err_inf = cmw_hcsta[D_RRH_CPRINO_REC][a_rec_typ];
    
    if((a_rec_typ == CMD_SYS_S3G) && (a_err_inf ==CMD_OFF ))
    {
        /* アサートログ書込み                            */
        cm_Assert(D_RRH_LOG_AST_LV_WARNING, CMD_NUM1, "CPRI disconnect") ;
        
        /* 状態遷移処理:ログ取得中状態→通常運用状態    */
        lgw_almlogmng_tbl[a_rec_typ].get_log_sta = LGD_GETFHMSTS_USE;
        cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Send_ToTimer] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return ;
    }
    
    /* 「障害ログデータ転送」を送信    */
    /* UCHARの配列にデータを格納しているが、その何番目かを求める        */
    /* 障害ログ転送開始アドレス = 送信完了数 * 転送単位サイズ(10240)    */
    a_snd_startadr  = lgw_almlogmng_tbl[a_rec_typ].snd_cmplt_num * CMD_MAX_TRNS_SIZ  ;
    
    /* 送信サイズ算出                */
    /* 送信サイズ = 総データ長 - (送信完了数 * 転送単位サイズ)            */
    a_snd_size_calc = (UINT)(lgw_almlogmng_tbl[a_rec_typ].snd_logsize - 
                     ( lgw_almlogmng_tbl[a_rec_typ].snd_cmplt_num * CMD_MAX_TRNS_SIZ )) ;
    
    if(a_snd_size_calc >= CMD_MAX_TRNS_SIZ )
    {
        a_snd_size = CMD_MAX_TRNS_SIZ ;
    }
    else
    {
        a_snd_size = a_snd_size_calc ;
    }
    
    /* 障害ログデータ転送関数コール                */
    (VOID)m_lg_send_logdata( a_snd_startadr, a_snd_size, a_rec_typ ) ;
    
    /* 障害ログ管理テーブル 送信完了数の更新    */
    lgw_almlogmng_tbl[a_rec_typ].snd_cmplt_num ++ ;
    
    /* 分割送信完了判定            */
    /* 送信完了数 < 分割送信数    */
    if( lgw_almlogmng_tbl[a_rec_typ].snd_cmplt_num < lgw_almlogmng_tbl[a_rec_typ].div_snd_num )
    {
        /* 分割送信を継続するため、障害ログ転送間隔タイマを起動    */
        /* 障害    ログ転送間隔タイマ起動    */
        a_timid = E_RRH_TIMID_LOG_SEND_S3G;          /*-----time id for S3G-----*/  
        a_timeoutntc = CMD_TIMEOUTNTC_LOGTRNS_S3G;   /*-----timeout ntc for S3G-----*/   

        a_timstart_result = cm_TStat(   a_timid,      /* TimerID */
                                        LGD_WAIT_TIME_270,     /* Timer値      */
                                        CMD_TIMMD_SINGLE,     /* 1 Short Timer */
                                        a_timeoutntc,         /* TO時EventNo   */
                                        D_RRH_THDQID_L3_LOG, /* TO時送信元ThreadQID */
                                        &errcd ) ;
        
        if( CMD_RES_OK != a_timstart_result )
        {
            /* タイマ停止したままだと固まってしまうのでタイマ起動NG時は通常状態に戻る */
            cm_Assert(D_RRH_LOG_AST_LV_ERROR, a_timstart_result, "cm_TStat") ;
        }
    }
    else
    {
        /* 「障害ログ完了通知」を送信                            */
        m_lg_send_logfinal( a_rec_typ ) ;
        
        /* API:圧縮したFHMと配下REの障害ログファイル削除Call */
        apiRet = rrhApi_Log_Mnr_TroubleLogDelete( D_RRH_PROCQUE_L3,  /* 応答待ちQueueID  */
                           0,   /* 応答待ち時間 */
                           NULL,  /* 取得データポインタ  */
                           (UCHAR*)lgw_almlogmng_tbl[a_rec_typ].logfile_name_path );  /* 作成ファイル名 */
        if( apiRet != E_API_RCD_OK )
        {
            cm_Assert(D_RRH_LOG_AST_LV_ERROR, apiRet, "rrhApi_Log_Mnr_TroubleLogDelete APIERROR(LTE)") ;
        }

        /* 全てのREをループする (16 RE)*/
        for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
        {
            /* 全配下REのRE障害ログ取得状態 ← 未処理 */
            gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] = LGD_GETRESTS_NOTPROC;
        }

        /* 状態遷移処理:ログ取得中状態→通常運用状態            */
        lgw_almlogmng_tbl[a_rec_typ].get_log_sta = LGD_GETFHMSTS_USE;
        
    }
    
    cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Send_ToTimer] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
    
    return ;
}

/* @} */

