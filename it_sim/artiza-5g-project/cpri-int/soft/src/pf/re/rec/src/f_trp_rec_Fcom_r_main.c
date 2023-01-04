/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_r_main.c
 * @brief  ルートメイン
 * @date   2007/03/20 FJT)Nagasima create.
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007-2016
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/*!
 * @brief  初期化完了通知送信
 * @return none
 * @date   2015/09/14 TDI)satou create
 */
static VOID f_trp_rec_Fcom_t_it_thrsupcmp_snd(VOID) {
    int endcd;
    T_RRH_HEAD *sndMsg;
    int errcd;

    endcd = BPF_RU_IPCM_PROCMSG_ADDRGET(E_BPF_RU_IPCM_BUFF_KIND_TASK, sizeof(T_RRH_HEAD), (VOID**)&sndMsg, &errcd);
    if (BPF_RU_IPCM_OK != endcd) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "ADDRGET NG %d", endcd);
        return;
    }

    /* 初期化完了通知作成 */
    sndMsg->uiEventNo     = CMD_TSKIF_INIRSP;
    sndMsg->uiSrcTQueueID = CMD_TSKID_REC;
    sndMsg->uiDstTQueueID = CMD_TSKID_INI;
    sndMsg->uiLength      = sizeof *sndMsg;

    /* 初期化完了通知送信 */
    endcd = BPF_RU_IPCM_MSGQ_SEND(sndMsg->uiDstTQueueID, sndMsg);
    if (BPF_RU_IPCM_OK != endcd) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "MSGQ_SEND NG %d", endcd);
        return;
    }
}

/****************************************************************************/
/*!
 * @brief  ルートメイン
 * @param  param スレッド名
 * @note   RECスレッドメイン
 *         - メモリクリア
 *         - スレッド間(スレッド起動完了通知)送信
 *         - メッセージ受信
 *         - バッファ解放
 * @param  -
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2010/06/02 FJT)Tokunaga TS-514項 M-S3G-eNBSYS-01559
 * @date   2015/10/20 TDI)satou 問処番号No.116水平展開
 * @TBD_No RECスレッドのmain処理修正未完了
 * @date   2016/10/04 KCN)hfuku 16B機能追加
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_r_main(VOID *param) {
//TODO
//    VOID* bufp;                                         /* バッファポインタ */
//    UINT  msgid;                          /* receive data source messagQ Id */
//    UINT  msglen;                                    /* receive data length */
//    CHAR errdata[48];
    UINT  ret;
    INT		errcd;
    USHORT	loop_num, loop, kind, cpri, ext, sys;
    VOID *rcvMsg;
    UINT  msgSize;
	T_MSGHEAD ltMsg;

    prctl(PR_SET_NAME, (CHAR*)param, 0, 0, 0);
#ifdef FOR_DBG
    printf("#RE rec thread......\n");
#endif
/* 2020/12/28 M&C) Merge 4G FHM src (start modify) */
// BPF_RM_SVRM_THREAD_INFO_SET(D_RRH_PROCID_RE, D_RRH_THDID_RE_REC, CMD_TSKID_REC);
    BPF_RM_SVRM_THREAD_INFO_SET(D_RRH_PROCID_RE, D_RRH_THDID_RE_REC, CMD_TSKID_REC, (unsigned long int)pthread_self());
/* 2020/12/28 M&C) Merge 4G FHM src (end modify) */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

	/* FWファイルデータ一時保存用共有メモリ */
	(VOID)BPF_RU_IPCM_PROCSHM_ADDRGET(D_RRH_SHMID_APL_LMC_DATA, (VOID**)&f_trp_rec_fwdata_area, &errcd);

    /****************/
    /* メモリクリア */
    /****************/
    /* RE状態管理(セクタ部)テーブル */
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        &f_trp_rec_Wcom_remsc_tbl,                            /* アドレス:I */
        sizeof(f_trp_rec_Wcom_remsc_tbl)                        /* サイズ:I */
    );

    /* RE状態管理(共通部)テーブル */
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        &f_trp_rec_Wcom_remcm_tbl,                            /* アドレス:I */
        sizeof(f_trp_rec_Wcom_remcm_tbl)                        /* サイズ:I */
    );

    /* RE状態管理(セクタ部)テーブル(BK) */
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        &f_trp_rec_Wcom_remsc_tbk,                            /* アドレス:I */
        sizeof(f_trp_rec_Wcom_remsc_tbk)                        /* サイズ:I */
    );

    /* RE状態管理(共通部)テーブル(Bk) */
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        &f_trp_rec_Wcom_remcm_tbk,                            /* アドレス:I */
        sizeof(f_trp_rec_Wcom_remcm_tbk)                        /* サイズ:I */
    );

    /* C02状態管理テーブル */
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        &f_trp_rec_Wc02_stsmng_tbl,                           /* アドレス:I */
        sizeof(f_trp_rec_Wc02_stsmng_tbl)                       /* サイズ:I */
    );

	/* C02状態管理テーブル */
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        &f_trp_rec_Wc02_stsmng_tbl_3g,                           /* アドレス:I */
        sizeof(f_trp_rec_Wc02_stsmng_tbl_3g)                       /* サイズ:I */
    );

    /* C03状態管理テーブル */
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        &f_trp_rec_Wc03_stsmng_tbl,                           /* アドレス:I */
        sizeof(f_trp_rec_Wc03_stsmng_tbl)                       /* サイズ:I */
    );

	/* C03状態管理テーブル */
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        &f_trp_rec_Wc03_stsmng_tbl_3g,                           /* アドレス:I */
        sizeof(f_trp_rec_Wc03_stsmng_tbl_3g)                       /* サイズ:I */
    );

//    /* NE Switch 状態管理テーブル */
//    f_trp_com_Fmemclr(                                      /* 終了コード:R */
//        &f_trp_rec_Wcom_nestsmng_tbl,                         /* アドレス:I */
//        sizeof(f_trp_rec_Wcom_nestsmng_tbl)                     /* サイズ:I */
//    );

    /* C02パラメータテーブル */
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        &f_trp_rec_Wc02_prm_tbl,                              /* アドレス:I */
        sizeof(f_trp_rec_Wc02_prm_tbl)                          /* サイズ:I */
    );

    /* C02パラメータテーブル */
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        &f_trp_rec_Wc02_prm_tbl_3g,                              /* アドレス:I */
        sizeof(f_trp_rec_Wc02_prm_tbl_3g)                          /* サイズ:I */
    );

    /* C03パラメータテーブル */
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        &f_trp_rec_Wc03_prm_tbl,                              /* アドレス:I */
        sizeof(f_trp_rec_Wc03_prm_tbl)                          /* サイズ:I */
    );

    /* C03パラメータテーブル */
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        &f_trp_rec_Wc03_prm_tbl_3g,                              /* アドレス:I */
        sizeof(f_trp_rec_Wc03_prm_tbl_3g)                          /* サイズ:I */
    );
	
//    /* AMP/OFTRXスロット番号→CPRI番号変換テーブル */
//    f_trp_com_Fmemclr(                                      /* 終了コード:R */
//        &f_trp_rec_Wcom_ampslt_cpr_tbl,                       /* アドレス:I */
//        sizeof(f_trp_rec_Wcom_ampslt_cpr_tbl)                   /* サイズ:I */
//    );

//    /* CPRI番号→AMP/OFTRX変換テーブル */
//    f_trp_com_Fmemclr(                                      /* 終了コード:R */
//        &f_trp_rec_Wcom_cpr_amp_tbl,                          /* アドレス:I */
//        sizeof(f_trp_rec_Wcom_cpr_amp_tbl)                      /* サイズ:I */
//    );

    /* タイマ種別→タイマ値変換テーブル */
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        &f_trp_rec_Wcom_tmk_val_tbl,                          /* アドレス:I */
        sizeof(f_trp_rec_Wcom_tmk_val_tbl)                      /* サイズ:I */
    );

    /* タイマ管理テーブル */
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        &f_trp_rec_Wcom_timmng_tbl,                           /* アドレス:I */
        sizeof(f_trp_rec_Wcom_timmng_tbl)                       /* サイズ:I */
    );

    /* RUNHISテーブル */
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        &f_trp_rec_Wcom_runhis_tbl,                           /* アドレス:I */
        sizeof(f_trp_rec_Wcom_runhis_tbl)                       /* サイズ:I */
    );


    f_trp_com_Fmemclr( &f_trp_rec_Wcom_ReqRspLog_tbl, sizeof(f_trp_rec_Wcom_ReqRspLog_tbl) );
    f_trp_com_Fmemclr( &f_trp_rec_Wcom_ReqRspLogSave_tbl, sizeof(f_trp_rec_Wcom_ReqRspLogSave_tbl) );


    /* RUNHISテーブル初期設定 */
    strcpy(f_trp_rec_Wcom_runhis_tbl.tnm, "RUNHIS");
    /* REGHISテーブル初期設定 */
    strcpy(f_trp_rec_Wcom_reghis_tbl.tnm, "REGHIS");

//TODO
//	/* Get the reset hystory for bde card table memory */
//	BPF_RU_IPCM_PROCSHM_ADDRGET(D_SHMID_CPU_RSTTIMTBL, (VOID*)&f_trp_com_AutoResetMngTbl, &errcd);
//
//	for(loop_num = 0;loop_num < CMD_CPRINO_MAX;loop_num++){
//		/*	Auto RE Reset Contorol Flag	*/
//		cmw_cpriconftbl.cpriconfinfo[loop_num].AutoRstCtrlFlg = CMD_ON;
//
//	}

	for(loop_num = 0;loop_num < CMD_CPRINO_MAX;loop_num++){
		CarrStateUnmatch[loop_num] = CMD_OFF;
		timeSetFlag[loop_num] = D_RRH_OFF;
/* 16B KCN add */
		timeRecrdcscCount[loop_num] = CMD_NUM0;               /* REカード状態報告     送信間隔カウンタの初期設定 */
		timeRecrdcscCount3g[loop_num] = CMD_NUM0;             /* REカード状態報告(3G) 送信間隔カウンタの初期設定 */
		timeRecrdSndCount[loop_num] = CMD_NUM0;               /* REカード状態報告     送信slot数カウンタの初期設定 */
		timeRecrdSndCount3g[loop_num] = CMD_NUM0;             /* REカード状態報告(3G) 送信slot数カウンタの初期設定 */
	}

	/*	RE Alarm Protect Time Debug Flag	*/
	RE_AlarmProtectTime_Debug = CMD_OFF;

	RE_AlarmProtectTime = CMD_OFF;

	RE_multiFlag = CMD_OFF;

	/*	タイムアウト情報テーブルを初期化する	*/
	f_trp_com_timerMngTbl.useNumMax = 0;
	f_trp_com_timerMngTbl.useNum = 0;
	f_trp_com_timerMngTbl.count = 0;
	for(loop_num=0; loop_num<256; loop_num++)
	{
		f_trp_com_timerMngTbl.info[loop_num].numMax = 0;
		f_trp_com_timerMngTbl.info[loop_num].num = 0;
		f_trp_com_timerMngTbl.info[loop_num].start = 0;
		f_trp_com_timerMngTbl.info[loop_num].next = 0;
		f_trp_com_timerMngTbl.info[loop_num].last = 383;
		for(loop=0; loop<384; loop++)
		{
			f_trp_com_timerMngTbl.info[loop_num].tim_typ[loop] = 0;
			f_trp_com_timerMngTbl.info[loop_num].tim_info[loop] = 0;
			/*	loopが開始位置の場合	*/
			if(f_trp_com_timerMngTbl.info[loop_num].start == loop)
			{
				f_trp_com_timerMngTbl.info[loop_num].tim_before[0] = 0xffff;
			}
			else
			{
				f_trp_com_timerMngTbl.info[loop_num].tim_before[loop] = loop-1;
			}
			/*	loopが最終位置以外の場合	*/
			if(f_trp_com_timerMngTbl.info[loop_num].last > loop)
			{
				f_trp_com_timerMngTbl.info[loop_num].tim_next[loop] = loop+1;
			}
			else
			{
				f_trp_com_timerMngTbl.info[loop_num].tim_next[loop] = 0xffff;
			}
		}
	}
	for(kind=0; kind<EC_REC_TMK_MAX; kind++)
	{
		for(cpri=0; cpri<D_RRH_CPRINO_RE_MAX; cpri++)
		{
			for(ext=0; ext<CMD_MAXSLOTNO; ext++)
			{
				for(sys=0; sys<CMD_MAX_SYSNUM; sys++)
				{
					f_trp_com_timerInfo[kind][cpri][ext][sys].timerID = 0xffff;
					f_trp_com_timerInfo[kind][cpri][ext][sys].count = 0xffff;
					f_trp_com_timerInfo[kind][cpri][ext][sys].ofst = 0xffff;
					f_trp_com_timerInfo[kind][cpri][ext][sys].times = 0;
					f_trp_com_timerInfo[kind][cpri][ext][sys].timval = 0;
					f_trp_com_timerInfo[kind][cpri][ext][sys].timvalSav = 0;
				}
			}
		}
	}
//TODO
//	/* RE装置構成情報(共有メモリ)クリア */
//	f_com_cmm_FLatestReEqpInfInit();

//TODO
//	/* 管理テーブル初期化 */
//	/* ActiveLinkグループ管理テーブル、RE管理テーブル */
//	/* CpriLinkグループ管理テーブル、Card管理テーブル */
//	f_trp_rec_FEach_ObjMng_Init();
//	f_trp_rec_FEach_Obj_Create();

    /****************************************/
    /* スレッド間(スレッド起動完了通知)送信 */
    /****************************************/
    f_trp_rec_Fcom_t_it_thrsupcmp_snd();

	/* 初期化要求 */
	ltMsg.msgid = CMD_TSKIF_INIREQ;
	f_trp_rec_Fcom_r(&ltMsg);
	
	/*   10msec周期タイマ登録  */
	INT retv;
	retv = BPF_RU_HRTM_REGISTER(
			BPF_RU_HRTM_REG_PERIODIC,		/* Periodic timer 				*/
			EC_REC_TIMD_10MSEC_INT,			/* Time Id 						*/
			10,								/* Time							*/
			CMD_TSKIF_TIMOUTNTC,			/* event 						*/
			CMD_TSKID_REC,					/* Event notification entry		*/
			&errcd);						/* Error Code					*/

	if(retv != BPF_RU_HRTM_COMPLETE)
	{
		/* 入力パラメータ異常 or リソースビジー or タイマ登録失敗   */
		
		F_COMF_CMM_FINVDLOG( 
			D_TCM_INVLOGLV_WARN2, 					/* Logging Level	*/
			"Timer start NG",						/* User Data 1	*/
			(UCHAR*)&retv,							/* User Data 2	*/	
			sizeof(retv));							/* User Data 2 size	*/
	}

	/*********************************************
	 * Message Receive and Function despatched
	 ********************************************/
	for (;;) {

	    ret = cm_Recieve(CMD_TSKID_REC, 0, &rcvMsg, &msgSize);
	    if (BPF_RU_IPCM_OK != ret) {
	        cm_Assert(CMD_ASL_DBGLOW, ret, "[f_trp_rec_Fcom_r_main] receive thread message ng");
	        break;
	    }

	    f_trp_rec_Fcom_r(rcvMsg);

	    if (0 != cm_BFree(rcvMsg)) {
	        printf("error!!\n");
	    }
	}

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
