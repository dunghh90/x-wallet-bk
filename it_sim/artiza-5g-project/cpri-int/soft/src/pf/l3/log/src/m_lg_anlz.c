/*!
 * @skip  $ld:$
 * @file  m_lg_anlz.c
 * @brief 受信メッセージ解析処理
 * @date  2013/11/04 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup TRA_LOG
 * @{
 */

#include "m_cm_header.h"
#include "m_lg_header.h"
#include "rrhApi_L2.h"

/*!
 * @brief 関数機能概要:受信メッセージ内イベント番号とスレッド状態から処理関数を実行する
 * @note  関数処理内容.
 *       -# 受信メッセージ内イベント番号異常又はスレッド状態異常の場合、アボート処理を実施する
 *       -# イベント番号とスレッド状態をIndexとしたジャンプテーブルより該当処理をCallする
 * @param  *msgP            [in]VOID    受信メッセージ
 * @return INT
 * @retval D_RRH_OK        0:正常終了
 * @retval D_RRH_NG        1:異常終了
 * @date 2013/11/04 ALPHA)横山 create
 * @date 2014/12/23 FFCS)qiuchh add 3G process for znqy_ppcEOL
 */
INT m_lg_anlz(VOID* msgP)
{

    INT                               rtn = D_RRH_OK;       /* 復帰値         */
    CMT_TSKIF_HEAD*                   rcvMsgP;              /* 受信メッセージ    */
    USHORT                            a_rec_typ = CMD_NUM0; /*-----3G or LTE------*/
    USHORT                            a_signal = CMD_NUM0;  /* the CPRI signal    */
    UINT                              a_event_no;           /* イベント番号      */
    UINT                              a_status_no;
    CMT_TSKIF_CPRIRCV_SIGNALGET*      a_rcv_adr;
    CMT_TSKIF_CPRIDISCONN_NTC*        a_rcv_dsc;
    T_API_LOG_TROUBLELOG_GET_RSP*     a_rcv_apiadr;
        
    cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_anlz] enter") ;
    
    rcvMsgP = (CMT_TSKIF_HEAD*)msgP;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s <EventNo>0x%x start\n", __LINE__, __FUNCTION__,rcvMsgP->uiEventNo);
#endif
    
    /* 受信イベント番号からスレッド内イベント番号へ変換（Phase毎に修正必要） */
    switch( rcvMsgP->uiEventNo )
    {
    /* ================================================================ */
    /* ■CPRI信号受信通知受信時                                            */
    /* ================================================================ */
    case D_API_MSGID_L2_CPRIMSG_RCV_NTC:
        a_rcv_adr = (CMT_TSKIF_CPRIRCV_SIGNALGET*)msgP;
        
        /* CPRI信号種別で処理を分岐            */
        a_signal  = a_rcv_adr->signal_kind & CMD_SYS_MSK_CPRID;
        a_rec_typ = a_rcv_adr->signal_kind & CMD_SYS_CHK_CPRID;
        
        switch(a_signal)
        {
            /* ======================================================== */
            /* ■RECからのRE障害ログ取得要求受信時                         */
            /* ======================================================== */
            case CMD_CPRID_REOBSLOGREQ :
                /* イベント番号設定(0)    */
                a_event_no = LGD_EVT_RECOBSLOGREQ ;
                
                /* 応答送信先を保存する    */
                lgw_rspmng_tbl[a_rec_typ][LGD_L3_TROUBLELOG_GET_RSP].dstPQID = rcvMsgP->uiSrcPQueueID;
                lgw_rspmng_tbl[a_rec_typ][LGD_L3_TROUBLELOG_GET_RSP].dstTQID = rcvMsgP->uiSrcTQueueID;

                
                /* 正常に動作する場合は関数内で                                            */
                /* ファイルデータ送信/ファイルデータ送信完了通知送信の送信先を保存する    */
                break;
            /* ======================================================== */
            /* ■RECからのRE障害ログ取得中止要求受信時                     */
            /* ======================================================== */
            case CMD_CPRID_REOBLOGSTPREQ :
                /* イベント番号設定(8)    */
                a_event_no = LGD_EVT_RECGETLOGSTPREQ ;

                /* 応答送信先を保存する    */
                lgw_rspmng_tbl[a_rec_typ][LGD_L3_TROUBLELOG_STOP].dstPQID = rcvMsgP->uiSrcPQueueID;
                lgw_rspmng_tbl[a_rec_typ][LGD_L3_TROUBLELOG_STOP].dstTQID = rcvMsgP->uiSrcTQueueID;
                
                break;

            default :
                /* アサートログ書込み    */
                cm_Assert(D_RRH_LOG_AST_LV_WARNING, a_signal, "[LOG] ERROR-001") ;
                /* イベント番号設定    */
                a_event_no = LGD_EVT_EXCEPTION ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s LGD_EVT_EXCEPTION end\n", __LINE__, __FUNCTION__);
#endif
                break;
	}
	break;

    /* ================================================================ */
    /* REC Forwarding Notice */
    /* ================================================================ */
    case CMD_TSKIF_REC_FWDNTC:
        a_rcv_adr = (CMT_TSKIF_CPRIRCV_SIGNALGET*)msgP;
        
        /* CPRI信号種別で処理を分岐            */
        a_signal  = a_rcv_adr->signal_kind & CMD_SYS_MSK_CPRID;
        a_rec_typ = a_rcv_adr->signal_kind & CMD_SYS_CHK_CPRID;
        
        switch(a_signal)
        {
            /* ======================================================== */
            /* ■配下REからのRE障害ログ取得応答受信時                     */
            /* ======================================================== */
            case CMD_CPRID_REOBSLOGRES :
                /* イベント番号設定(1)    */
                a_event_no = LGD_EVT_REOBSLOGRES ;
                
                break;
            /* ======================================================== */
            /* ■配下REからのファイルデータ送信要求受信時                */
            /* ======================================================== */
            case CMD_CPRID_FILEDATSND :
                /* イベント番号設定(3)    */
                a_event_no = LGD_EVT_REFILSNDREQ ;
                
                break;
            /* ======================================================== */
            /* ■配下REからのファイルデータ送信完了報告通知受信時        */
            /* ======================================================== */
            case CMD_CPRID_FILSNDFINNTC :
                /* イベント番号設定(4)    */
                a_event_no = LGD_EVT_REFILSNDFINNTC ;
                
                break;
            /* ======================================================== */
            /* ■配下REからのRE障害ログ取得中止応答受信時                 */
            /* ======================================================== */
            case CMD_CPRID_REOBLOGSTPRES :
                /* イベント番号設定(9)    */
                a_event_no = LGD_EVT_REGETLOGSTPRES ;
                
                break;
            
            default :
                /* アサートログ書込み    */
                cm_Assert(D_RRH_LOG_AST_LV_WARNING, a_signal, "[LOG] ERROR-001") ;
                /* イベント番号設定    */
                a_event_no = LGD_EVT_EXCEPTION ;
                break;
        }
        break;
    
    /* ================================================================ */
    /* ■API:障害ログファイル作成完了通知受信                            */
    /* ================================================================ */
    case D_API_LOG_MNR_TROUBLE_LOG_GET_RSP :
        a_rcv_apiadr = (T_API_LOG_TROUBLELOG_GET_RSP*)msgP;
        a_rec_typ = a_rcv_apiadr->info.sysKind;
        a_event_no = LGD_EVT_REOBSLOGCREFINNTC ;
        break ;

    /* ================================================================ */
    /* ■RECへファイルデータ転送為の障害ログ転送間隔TO発生通知受信時    */
    /* ================================================================ */
    case CMD_TIMEOUTNTC_LOGTRNS_LTE :
        a_rec_typ = CMD_SYS_LTE;
        a_event_no = LGD_EVT_RECFILSNDTO ;
        break ;

    /* ================================================================ */
    /* ■配下REのRE障害ログ取得確認タイマTO発生通知受信時                */
    /* ================================================================ */
    case CMD_TIMEOUTNTC_LOGGETCF :
        a_rec_typ = CMD_SYS_LTE;
        a_event_no = LGD_EVT_REGETLOGCFTO ;
        break ;
    /* ================================================================ */
    /* ■配下REのRE障害ログ送信確認タイマTO発生通知受信時                */
    /* ================================================================ */
    case CMD_TIMEOUTNTC_LOGSNDCF :
        a_rec_typ = CMD_SYS_LTE;
        a_event_no = LGD_EVT_TROUBLELOGSNDCFTO ;
        break ;
    /* ================================================================ */
    /* ■配下REのRE障害ログ取得中止確認タイマTO発生通知受信時            */
    /* ================================================================ */
    case CMD_TIMEOUTNTC_LOGGETSTPCF :
        a_rec_typ = CMD_SYS_LTE;
        a_event_no = LGD_EVT_LOGGETSTPCFTO ;
        break ;

	/* ================================================================ */
	/* ■CPRIリンク断通知受信時 */
	/* ================================================================ */
    case CMD_TSKIF_CPRI_DISCONNECT_NTC:
    	a_rcv_dsc = (CMT_TSKIF_CPRIDISCONN_NTC*)msgP;
    	a_rec_typ = a_rcv_dsc->system_type;
    	a_event_no = LGD_EVT_CPRIDWNNTC;
    	break;

    default:
        /*    受信 起動要因異常によるNOP    */
        (VOID)m_lg_nop( (UCHAR *)rcvMsgP, a_rec_typ );
        
        cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_anlz] return") ;
        
        /*    異常のため処理中断    */
        return D_RRH_NG;
    }
    
    /*    スレッド状態異常の場合はNOP    */
    if( lgw_almlogmng_tbl[a_rec_typ].get_log_sta >= LGD_GETFHMSTS_MAXNUM )
    {
        /*    スレッド状態異常によるNOP        */
        (VOID)m_lg_nop( (UCHAR *)rcvMsgP, a_rec_typ );
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s <a_rec_typ>:%d <get_log_sta>:%d\n", __LINE__, __FUNCTION__, a_rec_typ, lgw_almlogmng_tbl[a_rec_typ].get_log_sta);
#endif
    }
    else
    {
        /* 状態番号設定                                        */
        a_status_no = lgw_almlogmng_tbl[a_rec_typ].get_log_sta ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s <a_rec_typ>:%d <get_log_sta>:%d <a_event_no>:%d\n", __LINE__, __FUNCTION__, a_rec_typ, a_status_no, a_event_no);
#endif
        /* マトリクステーブルに状態、起動要因を設定 */
        lgr_tskjump_tbl[a_status_no][a_event_no]( (UCHAR *)rcvMsgP, a_rec_typ) ;
    }
    
    cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_anlz] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s <a_rec_typ>:%d <get_log_sta>:%d end\n", __LINE__, __FUNCTION__, a_rec_typ, lgw_almlogmng_tbl[a_rec_typ].get_log_sta);
#endif
    
    return rtn;
}

/* @} */
