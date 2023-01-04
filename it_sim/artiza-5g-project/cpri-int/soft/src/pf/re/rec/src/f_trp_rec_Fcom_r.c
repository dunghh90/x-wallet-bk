/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_r.c
 * @brief  ルート
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2007/06/07 FJT)Nagasima CR-00019-001
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"
#include "f_rrh_reg_dis.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
void f_trp_rec_timeout_ntc()
{
	UINT	count, nextCnt;
	UINT	num;
	UINT	timLp;
	UINT	tim_ofst, nextOfst;
	INT		errcd;
	UINT	keyinf1;
    UINT	times, timValSv, timerID;
    T_RRH_TIMERTO_NTC	buf;                          /* バッファポインタ:I */
    bpf_ru_hrtm_key_inf_t key_inf;
    UCHAR	timKind, timCpri, timExtNo, timTyp, timVal, timSysNo;
    UINT	wstart=0, wnext=0, wlast=0;
    
	/*	現在のタイムアウト数をインクリメントする	*/
	count = ((f_trp_com_timerMngTbl.count + 1) % 256);
	/*	タイムアウトカウントを更新する	*/
	f_trp_com_timerMngTbl.count = count;
	
	/*	タイムアウト数を取得する	*/
	num = f_trp_com_timerMngTbl.info[count].num;
	if(num != 0)
	{
		/*スタート位置を取得する*/
		tim_ofst = f_trp_com_timerMngTbl.info[count].start;
		/*	タイムアウト数分LOOPする	*/
		for(timLp=0; timLp<num; timLp++)
		{
			/*	タイマ情報を退避する	*/
			keyinf1  = f_trp_com_timerMngTbl.info[count].tim_info[tim_ofst];  /* pgr0039 */
			timTyp   = f_trp_com_timerMngTbl.info[count].tim_typ[tim_ofst];   /* pgr0039 */
			timKind  = f_trp_com_timerMngTbl.info[count].tim_kind[tim_ofst];  /* pgr0039 */
			timCpri  = f_trp_com_timerMngTbl.info[count].tim_cpri[tim_ofst];  /* pgr0039 */
			timExtNo = f_trp_com_timerMngTbl.info[count].tim_extNo[tim_ofst]; /* pgr0039 */
			timSysNo = f_trp_com_timerMngTbl.info[count].tim_sysNo[tim_ofst]; /* pgr0039 */
			times    = f_trp_com_timerInfo[timKind][timCpri][timExtNo][timSysNo].times;
			timValSv = f_trp_com_timerInfo[timKind][timCpri][timExtNo][timSysNo].timvalSav;
			timerID  = f_trp_com_timerInfo[timKind][timCpri][timExtNo][timSysNo].timerID;
			timVal   = f_trp_com_timerInfo[timKind][timCpri][timExtNo][timSysNo].timval;
			nextCnt  = f_trp_com_timerInfo[timKind][timCpri][timExtNo][timSysNo].dummy;
			
			if(keyinf1 == 0)
			{
				wstart = f_trp_com_timerMngTbl.info[count].start;
				wnext = f_trp_com_timerMngTbl.info[count].next;
				wlast = f_trp_com_timerMngTbl.info[count].last;
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Timeout Error[%d]:c=%d,s=%d,l=%d,n=%d;[%d][%d].typ=%d,key=%08x,tval=%d", 
								timLp, num, wstart, wlast, wnext, count, tim_ofst, timTyp, keyinf1, timValSv);
				continue;
			}
			/*	次のオフセットを算出する	*/
			tim_ofst = f_trp_com_timerMngTbl.info[count].tim_next[tim_ofst];
			
			/*	タイムアウトしたタイマを削除する	*/
			(VOID)f_trp_com_Fcancel_tim( keyinf1 );
			
			if(times != 0)
			{
				times--;
			}
			/*	最後に設定するタイマ値が0msecの場合	*/
			if((times == 1)&&(timVal == 0))
			{
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Timeout Occur[%d]:c=%d,s=%d,l=%d,n=%d;[%d][%d].typ=%d,key=%08x,tval=%d,cnt=%d", 
								timLp, num, wstart, wlast, wnext, count, tim_ofst, timTyp, keyinf1, timVal, times);
				/*	タイムアウトと扱う	*/
				times = 0;
			}
			/*	タイムアウトの場合	*/
			if(times == 0)
			{
				/*	タイムアウト通知を送信する	*/
				buf.header.uiEventNo = CMD_TSKIF_TIMOUTNTC;
				buf.header.uiSignalkind = timerID;
				buf.data.userkey = keyinf1;
				f_trp_rec_Fcom_r(&buf);
//				ret = f_trp_rec_Fcom_t_msg_snd( &buf, CMD_TSKIF_TIMOUTNTC, D_RRH_PROCQUE_RE, CMD_TSKID_REC, msglen);
				
				/*	周期タイマの場合	*/
				if(timTyp == BPF_RU_HRTM_REG_PERIODIC)
				{
					/*	タイマを再登録する	*/
					key_inf.keyinf1 = keyinf1;
					(VOID)f_trp_com_Fregister_tim(timTyp, timValSv, key_inf, timerID, &errcd);
				}
				else
				{
					BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_INFO, "Timer TimeOut:0x%08x,tid=%d,tVal=%d,sCnt=%d,tCnt=%d,tOfst=%d",
													 buf.data.userkey, buf.header.uiSignalkind, timValSv, nextCnt, count, tim_ofst);
				}
			}
			/*	タイムアウトでない場合	*/
			else
			{
				/*	最後の一回以外の場合	*/
				if(times != 1)
				{
					timVal = 200;
				}
				/*	タイムアウト位置(次のタイムアウトから)を設定する	*/
				nextCnt = (f_trp_com_timerMngTbl.count + timVal) % 256;
				/*	同じタイムアウト位置に登録されているNEXTオフセットを取得する	*/
				nextOfst = f_trp_com_timerMngTbl.info[nextCnt].next;
				/*	タイマ情報を更新する	*/
				f_trp_com_timerMngTbl.info[nextCnt].tim_typ[nextOfst] = timTyp;
				f_trp_com_timerMngTbl.info[nextCnt].tim_info[nextOfst] = keyinf1;
				f_trp_com_timerMngTbl.info[nextCnt].tim_kind[nextOfst] = timKind;
				f_trp_com_timerMngTbl.info[nextCnt].tim_cpri[nextOfst] = timCpri;
				f_trp_com_timerMngTbl.info[nextCnt].tim_extNo[nextOfst] = timExtNo;
				f_trp_com_timerMngTbl.info[nextCnt].tim_sysNo[nextOfst] = timSysNo;
				
				f_trp_com_timerMngTbl.info[nextCnt].num++;
				f_trp_com_timerMngTbl.useNum++;
				/*	最終オフセットを更新する	*/
				f_trp_com_timerMngTbl.info[nextCnt].next = f_trp_com_timerMngTbl.info[nextCnt].tim_next[nextOfst];
				
				f_trp_com_timerInfo[timKind][timCpri][timExtNo][timSysNo].count = nextCnt;
				f_trp_com_timerInfo[timKind][timCpri][timExtNo][timSysNo].ofst = nextOfst;
				f_trp_com_timerInfo[timKind][timCpri][timExtNo][timSysNo].times = times;
				if(f_trp_com_timerMngTbl.useNum > f_trp_com_timerMngTbl.useNumMax)
				{
					f_trp_com_timerMngTbl.useNumMax = f_trp_com_timerMngTbl.useNum;
				}
				if(f_trp_com_timerMngTbl.info[nextCnt].num > f_trp_com_timerMngTbl.info[nextCnt].numMax)
				{
					f_trp_com_timerMngTbl.info[nextCnt].numMax = f_trp_com_timerMngTbl.info[nextCnt].num;
				}
			}
		}
	}
	return;
}


/****************************************************************************/
/*!
 * @brief  ルート
 * @note   イベント受信時の処理を行う
 *         -
 *           - ONE_SHOTタイマのタイムアウト発生通知の場合
 *             - タイマ管理テーブル削除
 *         - C01(前処理)ルート関数起動
 *         - C02ルート関数起動
 *         - C03ルート関数起動
 *         - C04ルート関数起動
 *         - CFFルート関数起動
 *         - C01(後処理)ルート関数起動
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2007/06/07 FJT)Nagasima CR-00019-001
 * @date   2015/09/19 TDI)satou 終了処理のメモリクリア処理を削除
 * @TBD_No 修正未完了
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_r(                                            /* なし:R */
   VOID* bufp                                         /* バッファポインタ:I */
) {
    VOID*                     datp = NULL;
    UINT                      msg_id;                       /* メッセージID */
    bpf_ru_hrtm_key_inf_t     key_inf;                    /* タイマキー情報 */
    UINT  key_inf1;                                        /* タイマキー情報 */
    bpf_ru_hrtm_notify_type_t ntc_type;                         /* 通知種別 */
    T_REC_TIMELM              elm;                            /* タイマ要素 */
    UINT                      bufmsg_flg;
    UINT                      uit_ret;                        /* 終了コード */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**************/
    /* RUNHIS追加 */
    /**************/
    f_trp_rec_Fcom_t_runhis_add(                            /* 終了コード:R */
        D_REC_RUNHIS_RHK_RCV,                                     /* 受信:I */
        bufp                                          /* バッファポインタ:I */
    );

    /********************/
    /* メッセージID取得 */
    /********************/
    msg_id     = ((CMT_TSKIF_MSGID_GET*)bufp)->head.msgid;
    bufmsg_flg = CMD_OFF;

	/* タイムアウト発生通知の場合 */
    if (msg_id == CMD_TSKIF_TIMOUTNTC) {
		/*	自律10msec周期タイマの場合	*/
		if(((T_RRH_TIMERTO_NTC*)bufp)->header.uiSignalkind == EC_REC_TIMD_10MSEC_INT)
		{
			f_trp_rec_timeout_ntc();
			return;
		}
        /* キー情報取得 */
       // key_inf = ((CMT_TSKIF_TIMOUTNTC*)bufp)->key_inf;
        key_inf1 = ((T_RRH_TIMERTO_NTC*)bufp)->data.userkey;
    	f_trp_com_Fmemclr(&key_inf, sizeof(key_inf));
		key_inf.keyinf1 = key_inf1;
        /****************************/
        /* タイマキー情報→要素変換 */
        /****************************/
        f_trp_rec_Fcom_t_key_elm_cnv(                             /* なし:R */
            key_inf1,                                         /* キー情報:I */
            &elm                                                  /* 要素:O */
        );

        /****************************/
        /* タイマ種別→通知種別変換 */
        /****************************/
        ntc_type = f_trp_rec_Fcom_t_tmk_ntc_cnv(              /* 通知種別:R */
            elm.tmk                                         /* タイマ種別:I */
        );

        /* ONE_SHOTタイマの場合 */
        if (ntc_type == BPF_RU_HRTM_REG_ONCE) {
            /**************************/
            /* タイマ管理テーブル削除 */
            /**************************/
            uit_ret = f_trp_rec_Fcom_t_timmng_tbl_del(      /* 終了コード:R */
                &key_inf                                      /* キー情報:O */
            );
            /* 一致情報なしの場合 */
            if (uit_ret != CMD_OK) {
                /* 終了 */
                return;
            }
        }
    }

    /* CPRI信号受信通知の場合 */
    if (msg_id == CMD_TSKIF_CPRIRCVNTC) {
         bufmsg_flg = CMD_ON;
         datp = &((T_REC_MSGRCV_TBL*)bufp)->datp;

        if (datp == NULL) {
            F_COMF_CMM_FINVDLOG(D_TCM_INVLOGLV_WARN2, "RCV CPRI Buffer NULL\n", NULL, CMD_NUM0);
            return;
        }
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d Mesage endian conver (NO)\n", __FILE__, __FUNCTION__, __LINE__);
#endif

        /************************************/
        /* CPRI信号バイトオーダー(NTOH)変換 */
        /************************************/
//        f_trp_rec_Fcom_t_cpr_ntoh_cnv(                            /* なし:R */
//            &(((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind),
//                                                     /* CPRI信号データ部:IO */
//            ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->cprircv_inf.length
//                                                              /* レングス:I */
//        );
    }

	f_trp_rec_Fc02_evt = CMD_OFF;
	f_trp_rec_Fc02_carr_evt = CMD_OFF;
	f_trp_rec_Fc02_3g_evt = CMD_OFF;
	f_trp_rec_Fc02_carr_3g_evt = CMD_OFF;
    /*********************/
    /* C02ルート関数起動 */
    /*********************/
    f_trp_rec_Fc02_r(                                             /* なし:R */
        bufp                                          /* バッファポインタ:I */
    );

	if(CMD_OFF == f_trp_rec_Fc02_evt )
	{

		/*********************/
	    /* C02_3Gルート関数起動 */
	    /*********************/
	    f_trp_rec_Fc02_r_3g(                                          /* なし:R */
	        bufp                                          /* バッファポインタ:I */
	    );
	}

	if((( CMD_OFF == f_trp_rec_Fc02_evt ) || ( CMD_ON == f_trp_rec_Fc02_carr_evt )) && (CMD_OFF == f_trp_rec_Fc02_3g_evt))
	{
	    /*********************/
	    /* C03ルート関数起動 */
	    /*********************/
	    f_trp_rec_Fc03_r(                                             /* なし:R */
	        bufp                                          /* バッファポインタ:I */
	    );
	}

	if((( CMD_OFF == f_trp_rec_Fc02_3g_evt ) || ( CMD_ON == f_trp_rec_Fc02_carr_3g_evt )) && (CMD_OFF == f_trp_rec_Fc02_evt))
	{
	    /*********************/
	    /* C03_3Gルート関数起動 */
	    /*********************/
	    f_trp_rec_Fc03_r_3g(                                             /* なし:R */
	        bufp                                          /* バッファポインタ:I */
	    );
	}

	if((CMD_ON != f_trp_rec_Fc02_evt) && (CMD_ON != f_trp_rec_Fc02_3g_evt))
	{
		 /*********************/
	    /* CXXルート関数起動 */
	    /*********************/
	    f_trp_rec_FcXX_r(                                             /* なし:R */
	        bufp                                          /* バッファポインタ:I */
	    );
	}
/*
 * TRIFでは受信メッセージで使用していたメモリを使いまわして送信用に使っていたため
 * メモリを解放させないための処理を行っていましたが、FHMでは都度メモリを取り直す
 * ようになっているため以下の処理は不要のはず。
 */
#if 0
    if ((bufmsg_flg == CMD_ON) &&
        (datp       != NULL  ) ){
        /*********************/
        /* Free msg buffer   */
        /*********************/
        f_trp_com_Ffree_buf(
            datp
        );
    }
    else {
        /*********************/
        /* Buffer clear      */
        /*********************/
        f_trp_com_Fmemclr(
            bufp,
            sizeof(T_MSGHEAD) + CMD_NUM4
        );
    }
#endif

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
