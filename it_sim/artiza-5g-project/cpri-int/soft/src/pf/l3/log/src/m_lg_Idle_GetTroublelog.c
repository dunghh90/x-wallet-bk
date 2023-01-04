/*!
 * @skip   $Id$
 * @file   m_lg_Idle_GetTroublelog.c
 * @brief  the processing after receiving RE trouble log get request(the state is using).
 * @date   2008/07/29 FFCS)Wuh Create for eNB-009-004.
 * @date   2009/06/09 FFCS)Wuh      M-S3G-eNBPF-02047 RE version response incorrect
 * @date   2009/08/08 FFCS)Wangjuan CR-00058-005(CPRI spec V1.0.9)
 * @date   2009/11/17 QNET)Kabasima M-S3G-eNBPF-02868:[品質改善]RE障害ログ要求受信時の障害ログFLASH管理データ異常時処理
 * @date   2010/01/20 QNET)Kabasima M-S3G-eNBPF-03127:[品質改善]RE障害ログ収集改善
 * @date   2010/03/09 QNET)Kabasima M-S3G-eNBPF-03234:障害未発生カードに対して障害ログ収集した場合に運用ログが収集されない
 * @date   2011/04/22 FJT)Koshida M-S3G-eNBPF-04038:運用中ダウンロードと障害ログの競合でTRAリセット発生
 * @date   2011/05/06 FJT)Koshida M-S3G-eNBPF-04042:REファイルの運用中ダウンロードと障害ログ取得の競合にてTRAにALM発生
 * @date   2011/05/13 FJT)Koshida M-S3G-eNBPF-04046:運用中ダウンロードと障害ログ取得の競合にて、障害ログ取得応答TO発生
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2011
 */

/*!
 * @addtogroup TRA_LOG
 * @{
 */

#include "m_cm_header.h"
#include "m_lg_header.h"

/*!
 *  @brief  the function processing after receiving RE trouble log get request(the state is using).
 *  @note   This function is processed as follows.
 *          1) compress and save the sending data
 *          2) set management table
 *          3) start the timer for the sending interval
 *  @param  *a_rcv_adr [in]  the received message (障害ログ取得要求)
 *  @param  a_rec_typ  [in]  the type of RE system 3G or S3G
 *  @return Void.
 *  @retval -
 *  @date   2008/07/29 FFCS)Wuh Create.
 *  @date   2008/12/18 FFCS)chenwei  M-S3G-eNBPF-01029
 *  @date   2009/11/17 QNET)Kabasima M-S3G-eNBPF-02868:[品質改善]RE障害ログ要求受信時の障害ログFLASH管理データ異常時処理
 *  @date   2010/03/09 QNET)Kabasima M-S3G-eNBPF-03234:障害未発生カードに対して障害ログ収集した場合に運用ログが収集されない
 *  @date   2011/04/22 FJT)Koshida M-S3G-eNBPF-04038:運用中ダウンロードと障害ログの競合でTRAリセット発生
 *  @date   2011/05/06 FJT)Koshida M-S3G-eNBPF-04042:REファイルの運用中ダウンロードと障害ログ取得の競合にてTRAにALM発生
 *  @date   2011/05/13 FJT)Koshida M-S3G-eNBPF-04046:運用中ダウンロードと障害ログ取得の競合にて、障害ログ取得応答TO発生
 *  @date   2013/11/04 ALPHA)yokoyama modify for ZYNQ
 *  @date   2015/08/04 FPT)Tung modify for FHM
 *  @date	2015/10/19 FPT)Lay ファイル名が"/fm/XXXX.IEF"と"/"が入る対応
 *  @date   2015/10/24 TDIPS)sasaki Warning対処
 *  @date   2015/11/09 TDIPS)Takeuchi 技説QA120
 */
VOID m_lg_Idle_GetTroublelog( UCHAR *a_rcv_adr, USHORT a_rec_typ )
{
    CMT_TSKIF_CPRIRCV_REOBSLOGREQ   *a_parm_p = NULL ; /* 障害ログ取得要求型ポインタ */
    UINT                            sigchk_rslt; /*result of checking signal */
    UINT                            parmchk_rslt = CMD_OK; /* result of checking parameter */
    USHORT                          lwLoop;    /* For loop procedure */
    USHORT                          lwfhmself = CMD_TRUE; /* For check FHMが単体で動いている場合 */
    T_RRH_LAYER3                    ltLayer3Sts;
    USHORT                          lwLayer3ReSts;
    UINT                            a_timstart_result = CMD_RES_OK; /* タイマ起動結果 */
    UINT                            a_timid = CMD_NUM0; /*-----timer confirmer id------- */
    UINT                            a_timeoutntc = CMD_NUM0; /*-----timeout ntc-------------- */
	UINT							retval = CMD_RES_OK;
    INT                             errcd = CMD_NUM0;
	CHAR							*filename_p = NULL;
    CHAR                         	a_filename[512];
    E_RRHAPI_RCODE      			apiRet;

    cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_Idle_GetTroublelog] enter") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif

    /* 受信メッセージをRE障害ログ取得要求型でかぶせる */
    a_parm_p = (CMT_TSKIF_CPRIRCV_REOBSLOGREQ *)a_rcv_adr ;

    /* validate signal */
    sigchk_rslt = m_cm_chksig(a_parm_p->cpridat_reobslogreq.signal_kind,&a_rec_typ);
    if(sigchk_rslt != CMD_RES_OK)
    {
        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "validate signal NG");
    }

    /* 3G-RECへ送信できる障害ログファイルのサイズ上限は1MBのため、3Gの障害ログ取得要求受信時はNG応答にする */
    if( CMD_SYS_3G == a_rec_typ)
    {
        /* RE障害ログ取得応答送信 (42:その他エラー(RE))        */
        m_lg_send_getres( CMD_OTHR_ERR_RE, a_rec_typ, 0 ) ;

        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "Other error, 3G system not support" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_GetTroublelogReq] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return;
    }

    /* パラメータRE障害ログ取得要求, RE共通部/REセクタ部指定チェック */
    /* RE共通部/REセクタ部指定は範囲外の場合 */
    if( a_parm_p->cpridat_reobslogreq.comsec != CMD_RE_COM &&
       a_parm_p->cpridat_reobslogreq.comsec != CMD_RE_SEC )
    {
        parmchk_rslt = CMD_NG;
    }
    /*「パラメータチェックNG 」の場合 */
    if( parmchk_rslt != CMD_OK )
    {
        /* RE障害ログ取得応答送信 (51:処理NG(RE))        */
        m_lg_send_getres( CMD_HDL_NG, a_rec_typ, 0 ) ;

        cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "Parameter Check NG" );
        cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_GetTroublelogReq] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
        return;
    }

    /* 障害ログ管理テーブル初期値設定  */
    /* 送信ログサイズ    */
    lgw_almlogmng_tbl[a_rec_typ].snd_logsize   = LGD_SIZE_INI ;
    /* 分割送信数        */
    lgw_almlogmng_tbl[a_rec_typ].div_snd_num   = LGD_NUM_INI  ;
    /* 送信完了数        */
    lgw_almlogmng_tbl[a_rec_typ].snd_cmplt_num = LGD_NUM_INI  ;
    /* 入力パラメータ1    */
    lgw_almlogmng_tbl[a_rec_typ].in_parm1      = LGD_PARM_INI ;
    /* 入力パラメータ2 : RE共通部/REセクタ部指定  */
    lgw_almlogmng_tbl[a_rec_typ].in_parm2      = a_parm_p->cpridat_reobslogreq.comsec ;

    /* ログファイル名    */
	if( (filename_p = strrchr((CHAR *)&a_parm_p->cpridat_reobslogreq.filename[CMD_NUM0], '/')) != NULL )
	{
		filename_p++;
	}
	else
	{
		filename_p = (CHAR *)&a_parm_p->cpridat_reobslogreq.filename[CMD_NUM0];
	}
    cm_MemCpy( &lgw_almlogmng_tbl[a_rec_typ].logfile_name[CMD_NUM0],
               filename_p,
               (sizeof(UCHAR) * LGD_MAX_FILNAME_NUM) ) ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s logfilename:%s\n", __LINE__, __FUNCTION__, &lgw_almlogmng_tbl[a_rec_typ].logfile_name[CMD_NUM0]);
#endif

    /* 実サイズ  */
    lgw_almlogmng_tbl[a_rec_typ].realsize  = LGD_SIZE_INI;

#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
	printf( "[%d]%s get_log_sta:%d\n", __LINE__, __FUNCTION__, lgw_almlogmng_tbl[a_rec_typ].get_log_sta);
#endif
	/* RE障害ログ取得段階状態がRE障害ログ取得中止処理中 の場合 */
	if( lgw_almlogmng_tbl[a_rec_typ].get_log_sta == LGD_GETFHMSTS_STOPPROCESSING )
	{
		/* 「RE障害ログ取得中止確認タイマ」停止 */
		retval = cm_TStop(E_RRH_TIMID_LOG_GET_STOP_LTE, &errcd);
		if(retval != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGLOW, E_RRH_TIMID_LOG_GET_STOP_LTE, "Timer stop NG");
		}

		/* RE障害ログ取得中止要求の再送回数初期化 */
		gw_lgw_REGetLogStopReqCnt = 0;
	}

    /* 障害ログファイル保存用フォルダのクリアを要求 */
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

    /* 全てのREをループする (16 RE)*/
    for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
    {
        /* 全配下REのRE障害ログ取得状態 ← 未処理 */
        gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] = LGD_GETRESTS_NOTPROC;

        /* 受信ログデータサイズを初期化 */
        gw_lgw_LogFilRcvSize[a_rec_typ][lwLoop] = LGD_SIZE_INI;

        /* RE障害ログ取得応答管理テーブルを初期化 */
        cm_MemClr( &gw_lgw_REobLogGetRes_tbl[a_rec_typ][lwLoop], sizeof( LGT_REOBSLOGRESMNG_T ) );
    }

    /* Get layer3 status*/
    (VOID)f_cmn_com_layer3_get(a_rec_typ, &ltLayer3Sts);

    /* 全てのREをループする (16 RE)*/
    for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
    {
        lwLayer3ReSts = ltLayer3Sts.layer3_re[lwLoop];
        /* RE 状態が②RE起動中状態、または④運用中状態の場合 */
        if(( D_L3_STA_2 == lwLayer3ReSts ) || ( D_L3_STA_4 == lwLayer3ReSts ))
        {
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s l3_com_sendMsg <link_num>%d <signal_kind>0x%x\n", __LINE__, __FUNCTION__, lwLoop+1, a_parm_p->cpridat_reobslogreq.signal_kind);
#endif
            /********************/
            /* CPRI番号設定 */
            /********************/
            a_parm_p->cprircv_inf.link_num = lwLoop + 1;

			/*EventNo for sending to re/rec*/
			a_parm_p->head.uiEventNo = CMD_TSKIF_RE_FWDNTC;

		    /* 配下REのログファイル名に変換  */
		    cm_MemCpy( &a_parm_p->cpridat_reobslogreq.filename[CMD_NUM0],
					   &obslog_filename_tbl[lwLoop][0],
               		   (sizeof(UCHAR) * LGD_MAX_FILNAME_NUM) ) ;

           /* RE障害ログ取得要求をre/recに送信する */
            l3_com_sendMsg(CMD_TSKID_LOG, D_RRH_PROCQUE_RE, 0, a_parm_p, sizeof(CMT_TSKIF_CPRIRCV_REOBSLOGREQ));

            /* 対象配下REに対してRE障害ログ取得状態←取得要求中 */
            gw_lgw_REGetLogSta_tbl[a_rec_typ][lwLoop] = LGD_GETRESTS_REQUESTING;

            lwfhmself = CMD_FALSE;
        }

        /* 各対象配下REのRE障害ログ取得応答結果をNG結果として初期化 */
        gw_lgw_REobLogGetRes_tbl[a_rec_typ][lwLoop].result = CMD_NG;
    }

    /* 各対象配下REがある場合 */
    if(lwfhmself == CMD_FALSE)
    {
        /* 「RE障害ログ取得確認タイマ」開始 */
        a_timid = E_RRH_TIMID_LOG_GET_LTE;          /*-----time id for LTE-----*/
        a_timeoutntc = CMD_TIMEOUTNTC_LOGGETCF;   /*-----timeout event for LTE-----*/

        a_timstart_result = cm_TStat( a_timid, /* TimerID */
                                      CMD_TIMVAL_RE_TROUBLELOG_GET_CONF, /* Timer値 */
                                      CMD_TIMMD_SINGLE, /* 1 Short Timer */
                                      a_timeoutntc, /* TO時EventNo */
                                      D_RRH_THDQID_L3_LOG, /* TO時送信元ThreadQID */
                                      &errcd ) ;
        if( a_timstart_result != BPF_RU_HRTM_COMPLETE ){
            cm_Assert(D_RRH_LOG_AST_LV_ERROR, a_timstart_result, "[m_lg_GetTroublelogReq] cm_TStat Error") ;
        }

        /* RE障害ログ取得段階状態 ← 配下RE障害ログ取得処理中 */
        lgw_almlogmng_tbl[a_rec_typ].get_log_sta = LGD_GETFHMSTS_REPROCESSING;
    }
    /* FHMが単体で動いている場合 */
    else {
        /* RE障害ログ取得応答 (0:正常) */
        m_lg_send_getres( CMD_OK, a_rec_typ, LGD_MAX_COMPDAT ) ;

        /* 障害ログファイル作成    */
        m_lg_FhmLogFilMake(a_rec_typ);

        /* RE障害ログ取得段階状態 ← FHMログファイル作成中状態 */
        lgw_almlogmng_tbl[a_rec_typ].get_log_sta = LGD_GETFHMSTS_ACHFILCREATING;
    }

    /* RE障害ログ送信確認タイマ開始済フラグ　←　OFF*/
    gw_lgw_RETroublelogSendTimerStartFlg = D_RRH_OFF;

    /* ファイルデータ送信中の配下RE数← 0 */
    gw_lgw_FilSndNum[a_rec_typ] = CMD_NUM0;

    cm_Assert( D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_Idle_GetTroublelog] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif

    return ;
}
/* @} */

