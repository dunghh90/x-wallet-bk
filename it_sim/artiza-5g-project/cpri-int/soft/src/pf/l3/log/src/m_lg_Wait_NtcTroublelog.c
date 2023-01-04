/*!
 * @skip   $Id$
 * @file   m_lg_Wait_NtcTroublelog.c
 * @brief  the processing after receiving trouble log make finish notification.
 * @date   2013/10/30 ALPHA)yokoyama Create for ZYNQ_docomo
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
 *  @brief  the processing after receiving trouble log make finish notification.
 *  @note   This function is processed as follows.
 *          1) set management table
 *          2) start the timer for the sending interval 
 *  @param  *a_rcv_adr [in]  -  the received message
 *  @param  a_rec_typ  [in]  the type of RE system 3G or LTE
 *  @return VOID
 *  @retval -
 *  @date   2013/10/30 ALPHA)yokoyama Create.
 *  @date   2015/08/02 FPT)Tung modify for FHM.
 *  @date	2016/02/05 TDI)satou 転送中にリンク断した場合、転送を中止するように
 *  @date   2016/02/06 TDI)satou 分割数算出方法見直し. 障害ログ取得中にCPRIリンク断->復旧すると、RECからログが再度すぐに取得できない問題の対処
 */

VOID m_lg_Wait_NtcTroublelog( UCHAR *a_rcv_adr, USHORT a_rec_typ )
{
                                            
    UINT                         a_timstart_result = CMD_RES_OK ; /* タイマ起動結果  */
    UINT                         a_all_len         = CMD_NUM0 ;  /* 総データサイズ  */
    UINT                         a_timid           = CMD_NUM0 ;  /*-----timer confirmer id-------*/
    UINT                         a_timeoutntc      = CMD_NUM0 ;  /*-----timeout ntc--------------*/
    UINT                         a_snd_startadr    = CMD_NUM0 ;  /* 障害ログ転送開始アドレス        */
    UINT                         a_snd_size        = CMD_NUM0 ;  /* 送信サイズ                    */
    UINT                         a_snd_size_calc   = CMD_NUM0 ;  /* 計算機送信サイズ                */
    USHORT                       a_err_inf         = CMD_NUM1 ; /* ERR制御レジスタ情報格納変数    */
    INT                          errcd             = CMD_NUM0 ;
    USHORT                       lwLoop;
    
    cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_Wait_NtcTroublelog] enter") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif
    
    /***** 転送中にリンク断した場合は転送を中止する *****/
    /* CPRIリンクERR設定レジスタ情報Read                */
    a_err_inf = cmw_hcsta[D_RRH_CPRINO_REC][a_rec_typ];
    
    if((a_rec_typ == CMD_SYS_LTE) && (a_err_inf == CMD_OFF ))
    {
        /* アサートログ書込み                            */
        cm_Assert(D_RRH_LOG_AST_LV_WARNING, CMD_NUM0, "[ALMLOG_L3DISCONLTE]") ;
        
        /* 状態遷移処理:ログファイル作成中状態→通常運用状態    */
        lgw_almlogmng_tbl[a_rec_typ].get_log_sta = LGD_GETFHMSTS_USE ;
        cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Wait_NtcTroublelog] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return ;
    }
    
    /* FHM障害ログファイルと配下RE障害ログの書庫化処理 */
    m_lg_FhmRELogFilMake( a_rcv_adr, a_rec_typ );

    /* データサイズを取得 : FHMと正常に受信した配下REのファイルサイズの台数 × 1Mbyte*/
    for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
	{	/*
		 * 分割サイズの算出は、REからRE障害ログ取得応答で通知されたサイズと、FHMのサイズの合計で求める。
		 *
		 * 途中でRE側のCPRIリンク断発生などの要因で、上述の合計サイズよりも書庫化したファイルサイズが
		 * 小さくなるケースがある。この時、正常に受信したRE(LGD_GETRESTS_COMP)だけを対象にして
		 * 分割数を求めると、RECに通知したサイズよりも小さいサイズでファイルを送ることになる。
		 * その結果、REC側でNGを検出し、障害ログがRECのSDNに保存されない現象が発生する。
		 *
		 * gw_lgw_REobLogGetRes_tblは、RECから障害ログ取得要求を受信時に0クリアしている。
		 */
//        if (gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] == LGD_GETRESTS_COMP)
//        {
            a_all_len += gw_lgw_REobLogGetRes_tbl[a_rec_typ][lwLoop].datsize;
//        }
    }
    a_all_len += LGD_MAX_COMPDAT;
    
    /* 分割送信数を計算                */
    /* 総データサイズ/10240バイト    */
    lgw_almlogmng_tbl[a_rec_typ].div_snd_num =  (USHORT)( a_all_len / CMD_MAX_TRNS_SIZ ) ;
    
    /* 総データサイズを設定 */
    lgw_almlogmng_tbl[a_rec_typ].snd_logsize = a_all_len ;
    
    /* 総データサイズ % 10240バイト が 0バイトでない            */
    if( ( a_all_len % CMD_MAX_TRNS_SIZ ) != CMD_NUM0 )
    {
        /* 余りが発生する場合は分割送信数をカウントアップする    */
        lgw_almlogmng_tbl[a_rec_typ].div_snd_num ++ ;
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
    m_lg_send_logdata( a_snd_startadr, a_snd_size, a_rec_typ ) ;
    
    /* 障害ログ管理テーブル 送信完了数の更新    */
    lgw_almlogmng_tbl[a_rec_typ].snd_cmplt_num ++ ;
    
    a_timid = E_RRH_TIMID_LOG_SEND_S3G;          /*-----time id for LTE-----*/
    a_timeoutntc = CMD_TIMEOUTNTC_LOGTRNS_LTE;   /*-----timeout event for LTE-----*/   
    
    a_timstart_result = cm_TStat(   a_timid,  /* TimerID  */
                                    LGD_WAIT_TIME_270,  /* Timer値  */
                                    CMD_TIMMD_SINGLE,   /* 1 Short Timer */
                                    a_timeoutntc,       /* TO時EventNo   */
                                    D_RRH_THDQID_L3_LOG,  /* TO時送信元ThreadQID  */
                                    &errcd ) ;

    if( a_timstart_result != BPF_RU_HRTM_COMPLETE ){
        cm_Assert(D_RRH_LOG_AST_LV_ERROR, a_timstart_result, "[m_lg_Wait_NtcTroublelog] cm_TStat Error") ;
    }
    
    /* 状態遷移処理:ログファイル作成中状態→ログ送信中状態    */
    lgw_almlogmng_tbl[a_rec_typ].get_log_sta = LGD_GETFHMSTS_RECTRANSFERING ;
    
    cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Wait_NtcTroublelog] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
    
    return ;
}

/* @} */

