/*!
 * @skip   $Id$
 * @file   m_lg_Wait_StopTroublelog.c
 * @brief  the processing for RE trouble log get stop(state is log waiting).
 * @date   2013/11/04 ALPHA)yokoyama Create for ZYNQ
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
 *  @brief  the processing function for RE trouble log get stop(state is log waiting).
 *  @note   This function is processed as follows.
 *          1) send RE trouble log get stop response to REC
 *          2) call API [trouble log get stop]
 *          3) change the state to using
 *  @param  *a_rcv_adr [in]  the received message
 *  @param  a_rec_typ  [in]  the type of RE system 3G or S3G
 *  @return VOID
 *  @retval -
 *  @date   2013/11/04 ALPHA)yokoyama Create.
 *  @date   2015/08/02 FPT)Tung modify for FHM.
 *  @date   2015/11/08 TDIPS)Takeuchi 技説QA120
 *  @date   2015/11/17 FPT)Lay 障害ログ実装改善
 */
VOID m_lg_Wait_StopTroublelog( UCHAR *a_rcv_adr, USHORT a_rec_typ )
{
    CMT_TSKIF_CPRIRCV_REOBLOGSTPREQ *a_parm_p = NULL ; /* RE trouble log get stop request */
    UINT                            sigchk_rslt; /* result of checking signal */
    E_RRHAPI_RCODE                  apiRet; /* API復帰値 */
    CHAR                         	a_filename[256];
    USHORT                          lwLoop;
    UINT                            retval = CMD_RES_OK;
    INT                             errcd = CMD_NUM0;

    /* アサートログ書込み                                */
    cm_Assert( D_RRH_LOG_AST_LV_ENTER, CMD_NUM0, "[ALMLOG] m_lg_Wait_StopTroublelog" ) ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif

    /* Get receive message address*/
    a_parm_p = (CMT_TSKIF_CPRIRCV_REOBLOGSTPREQ *)a_rcv_adr;

    /* 3Gの場合,「障害ログ取得中止応答(NG)」を送信(42:その他エラー)    */
    if(a_rec_typ == CMD_SYS_3G)
    {
        m_lg_send_stopres( a_rec_typ, CMD_OTHR_ERR_RE) ;
        cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Wait_StopTroublelog] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return ;
    }

    sigchk_rslt = m_cm_chksig(a_parm_p->cpridat_reoblogstpreq.signal_kind,&a_rec_typ);
    if(sigchk_rslt != CMD_RES_OK)
    {
        cm_Assert(  D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "validate signal NG");
    }

	/* RE障害ログ取得段階状態がRE障害ログ取得中止処理中 の場合 */
	if( lgw_almlogmng_tbl[a_rec_typ].get_log_sta == LGD_GETFHMSTS_STOPPROCESSING )
	{
		/* 「RE障害ログ取得中止確認タイマ」停止 */
		retval = cm_TStop(E_RRH_TIMID_LOG_GET_STOP_LTE, &errcd);
		if(retval != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGLOW, E_RRH_TIMID_LOG_GET_STOP_LTE, "Timer stop NG");
		}
	}

    /* RE障害ログ取得段階状態が配下RE障害ログ取得中状態 or RE障害ログ取得中止処理中 の場合 */
    if( lgw_almlogmng_tbl[a_rec_typ].get_log_sta == LGD_GETFHMSTS_REPROCESSING ||
		lgw_almlogmng_tbl[a_rec_typ].get_log_sta == LGD_GETFHMSTS_STOPPROCESSING )
    {
    	/* 全てのREをループする (16 RE)*/
        for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
        {
            /* RE障害ログ取得状態が「	取得要求中 or
										配下REからのファイルデータ送信中 or
										配下REへ障害ログ中止要求中 」の配下REの場合 */
            if( gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] == LGD_GETRESTS_REQUESTING ||
                gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] == LGD_GETRESTS_FILSENDING ||
				gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] == LGD_STOPRESTS_REQUESTING )
            {
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s Send StopTroubleLog Req to RE#%d\n", __LINE__, __FUNCTION__, lwLoop+1);
#endif
                /* 配下REへ要求送信 */
                m_lg_StopTroublelogL3SendMsg(a_rec_typ, lwLoop + 1);

                /*  RE障害ログ取得状態が 配下REへ障害ログ中止要求中 */
                gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] = LGD_STOPRESTS_REQUESTING;
            }
            /* RE障害ログ取得中止応答結果がNGとして初期化 */
            gw_lgw_REobLogGetRes_tbl[a_rec_typ][lwLoop].result = CMD_SET_NG;
        }

        /* 「RE障害ログ取得中止確認タイマ」開始 */
        retval = cm_TStat(   E_RRH_TIMID_LOG_GET_STOP_LTE, /* TimerID */
                                        CMD_TIMVAL_LOGGETSTOP, /* Timer値 */
                                        CMD_TIMMD_SINGLE,      /* 1 Short Timer */
                                        CMD_TIMEOUTNTC_LOGGETSTPCF,   /* TO時EventNo */
                                        D_RRH_THDQID_L3_LOG,   /* TO時送信元ThreadQID */
                                        &errcd ) ;

        if( retval != BPF_RU_HRTM_COMPLETE ){
            cm_Assert(D_RRH_LOG_AST_LV_ERROR, retval, "[m_lg_StopTroublelog] cm_TStat Error") ;
        }

		/* RE障害ログ取得中止要求の再送回数初期化 */
		gw_lgw_REGetLogStopReqCnt = 0;

        /* RE障害ログ取得段階状態 ← RE障害ログ取得中止処理中 */
        lgw_almlogmng_tbl[a_rec_typ].get_log_sta = LGD_GETFHMSTS_STOPPROCESSING;
    }

    /* RE障害ログ取得段階状態がFHMログファイル作成中の場合 */
    if( lgw_almlogmng_tbl[a_rec_typ].get_log_sta == LGD_GETFHMSTS_ACHFILCREATING )
    {
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s Send StopTroubleLog Req to FHM%d\n", __LINE__, __FUNCTION__, lwLoop+1);
#endif
        /* API CALL(FHM 障害ログ作成中止) */
        apiRet = rrhApi_Log_Mnt_TroubleLogGetStop( D_RRH_PROCQUE_L3, /* 応答待ち受けPQID */
                                                   0, /* Wait時間 */
                                                   NULL ); /* OutParameter */
        if( apiRet != 0 )
        {
            cm_Assert(D_RRH_LOG_AST_LV_ERROR, 0, "[m_lg_Wait_StopTroublelog] API Error(LTE)") ;
        }

        /* RE障害ログ取得中止応答 (0:正常) */
        m_lg_send_stopres( a_rec_typ, CMD_NML) ;

        /* RE障害ログ取得段階状態 ← 通常運用状態 */
        lgw_almlogmng_tbl[a_rec_typ].get_log_sta = LGD_GETFHMSTS_USE;
    }

    memset( a_filename, 0x00, sizeof(a_filename) );
    snprintf( a_filename, sizeof(a_filename), "%s/*", LGD_REFHM_TROUBLE_LOG_FOLDER_PATH);
    /* API:圧縮したFHMと配下REの障害ログファイル削除Call */
    apiRet = rrhApi_Log_Mnr_TroubleLogDelete( D_RRH_PROCQUE_L3,  /* 応答待ちQueueID  */
                       0,   /* 応答待ち時間 */
                       NULL,  /* 取得データポインタ  */
                       (UCHAR*)a_filename);  /* ファイル名 */
    if( apiRet != E_API_RCD_OK )
    {
        cm_Assert(D_RRH_LOG_AST_LV_ERROR, apiRet, "rrhApi_Log_Mnr_TroubleLogDelete APIERROR(LTE)") ;
    }

    cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Wait_StopTroublelog] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif

    return ;
}

/* @} */

