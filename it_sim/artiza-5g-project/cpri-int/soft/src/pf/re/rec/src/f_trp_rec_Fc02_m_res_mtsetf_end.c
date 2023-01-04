/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_mtsetf_end.c
 * @brief  RE起動中MTアドレス設定(FLD-MT)終了処理
 * @date   2009/01/17 FFCS)Shihzh create.
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RE起動中MTアドレス設定(FLD-MT)終了処理
 * @note   RE起動中MTアドレス設定(FLD-MT)終了処理を行う
 *         - スレッド間(MTアドレス設定(SV-MT)プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(MTアドレス設定(FLD-MT)完了))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/07/30 FPT)Hieu create.
 * @date   2015/09/22 FPT)Yen update
 * @date   2015/11/08 TDIPS)Takeuchi 技説QA120
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_mtsetf_end(                             /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
	CMT_TSKIF_CPRIRCV_MTADDSETRES* ltRes_p = (CMT_TSKIF_CPRIRCV_MTADDSETRES*)bufp;
	USHORT	lwSts;

#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif
	/*Get MTアドレス設定状態*/
	lwSts = f_trp_rec_Fc02_t_mtgetsts( D_REC_C02_CPR_NO());
	if( lwSts != EC_REC_MTADDSET_STN_RUNNING)
	{
		return;
	}

	/*Response NG*/
	if( CMD_OK != ltRes_p->cpridat_mtaddsetres.result )
	{
		/*破棄し、タイムアウトによる再送を期待*/
		return;
	}

	/**************/
	/* タイマ取消 */
	/**************/
	f_trp_rec_Fc02_t_timer_ccl(                                   /* なし:R */
	    EC_REC_TMK_MT_ADD_SET,            /* MTアドレス設定確認タイマ:I */
	    D_REC_CPR_IV                                      /* スロット番号:I */
	);


	/*Set MTアドレス設定状態*/
	f_trp_rec_Fc02_t_mtsetsts( D_REC_C02_CPR_NO(), EC_REC_MTADDSET_STN_INI );

	/*************************************************************/
    /* スレッド間(MTアドレス設定(SV-MT)プロシージャ実行指示)送信 */
    /*************************************************************/
    f_trp_rec_Fc02_t_it_mtsetsprc_snd(                            /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_RES_MTSETFCMP
                                  /* RE起動中(MTアドレス設定(FLD-MT)完了):I */
    );

    return;
}


/****************************************************************************/
/*!
 * @brief  RE起動中MTアドレス設定(SV-MT)終了処理
 * @note   RE起動中MTアドレス設定(SV-MT)終了処理を行う
 *         - スレッド間(RE起動完了プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(MTアドレス設定(SV-MT)完了))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/07/30 FPT)Hieu create.
 * @date   2015/09/22 FPT)Yen update
 * @date   2015/11/08 TDIPS)Takeuchi 技説QA120
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_mtsets_end(                             /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
	CMT_TSKIF_CPRIRCV_MTADDSETRES* ltRes_p = (CMT_TSKIF_CPRIRCV_MTADDSETRES*)bufp;
	USHORT	lwSts;
#ifdef FHM_DEBUG_C02_FOR_IT1
	printf( "**FHM**[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif
	/*Get MTアドレス設定状態*/
	lwSts = f_trp_rec_Fc02_t_mtgetsts( D_REC_C02_CPR_NO());
	if( lwSts != EC_REC_MTADDSET_STN_RUNNING)
	{
		return;
	}

//	lwFactor = f_trp_rec_Fc02_t_tc_mtsetffactor_get();
#ifdef FHM_DEBUG_C02_FOR_IT1
	printf( "**FHM**[%d]%s Response result:%d \n",__LINE__,__FUNCTION__, ltRes_p->cpridat_mtaddsetres.result);
#endif

	/*Response NG*/
	if( CMD_OK != ltRes_p->cpridat_mtaddsetres.result )
	{
		/*破棄し、タイムアウトによる再送を期待*/
		return;
	}

	/**************/
	/* タイマ取消 */
	/**************/
	f_trp_rec_Fc02_t_timer_ccl(                                 /* なし:R */
	    EC_REC_TMK_MT_ADD_SET,          /* REスロット情報報告確認タイマ:I */
	    D_REC_CPR_IV                                    /* スロット番号:I */
	);

	/*Set MTアドレス設定状態*/
	f_trp_rec_Fc02_t_mtsetsts( D_REC_C02_CPR_NO(), EC_REC_MTADDSET_STN_INI );


	/***************************************************/
    /* スレッド間((RE起動完了プロシージャ実行指示)送信 */
    /***************************************************/
    f_trp_rec_Fc02_t_it_rescmpprc_snd(                            /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_RES_MTSETSCMP
                                   /* RE起動中(MTアドレス設定(SV-MT)完了):I */
    );


#ifdef FHM_DEBUG_C02_FOR_IT1
	printf( "**FHM**[%d]%s End \n",__LINE__,__FUNCTION__);
#endif
    return;
}

/****************************************************************************/
/*!
 * @brief  MTアドレス設定応答(REC主導)処理 (A process)
 * @note   MTアドレス設定応答(REC主導)処理を行う
 * @return None
 * @date   2015/07/30 FPT)Hieu create.
 * @date   2015/09/22 FPT)Yen update.
 * @date   2015/11/08 TDIPS)Takeuchi 技説QA120
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_mtsetres_rec( )
{
	USHORT	lwLoop;
	USHORT	lwResOkCnt = 0;
	USHORT	lwSts;

	/*Loop all RE*/
	for(lwLoop = CMD_CPRINO_MIN; lwLoop <= CMD_CPRINO_MAX; lwLoop++)
	{
		/*Get MTアドレス設定状態*/
		lwSts = f_trp_rec_Fc02_t_mtgetsts( lwLoop );
		/*各対象配下REのMTアドレス設定状態はOK応答？*/
		if( EC_REC_MTADDSET_STN_RESOK == lwSts )
		{
			lwResOkCnt++;
			break;
		}
	}
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s lwResOkCnt: %d \n",__LINE__,__FUNCTION__, lwResOkCnt);
#endif
	/* 配下REから1つもOK応答が受信できなかった場合 */
	if( 0 == lwResOkCnt )
	{
		/* 再送回数が最大値未満の場合 */
		if( D_MT_ADD_SET_RETRY_MAX > f_trp_rec_Wcom_MtAdSetRetryCnt[CMD_SYS_LTE] )
		{
			/*  CPRIリンク数だけループ */
			for(lwLoop = CMD_CPRINO_MIN; lwLoop <= CMD_CPRINO_MAX; lwLoop++)
			{
				/* Get MTアドレス設定状態 */
				lwSts = f_trp_rec_Fc02_t_mtgetsts( lwLoop );

				/* OK応答未受信のREへ再送する */
				if( EC_REC_MTADDSET_STN_RUNNING == lwSts )
				{
					/* MTアドレス設定要求(REC主導)再送 */
					f_trp_rec_Fc02_m_mtsetf_req_snd( lwLoop );
				}
			}

			/* MTアドレス設定要求(REC主導)再送回数カウントアップ */
			f_trp_rec_Wcom_MtAdSetRetryCnt[CMD_SYS_LTE]++;

			return;
		}
		else
		{
			/* MTアドレス設定応答(51:処理NG(RE)) */
			f_trp_rec_Fc02_m_mtsetf_res_snd( CMD_HDL_NG );
		}
	}
	else
	{
		/* MTアドレス設定応答(0:正常) */
		f_trp_rec_Fc02_m_mtsetf_res_snd( CMD_OK );
	}

	/* MTアドレス設定要求(REC主導)再送回数初期化 */
	f_trp_rec_Wcom_MtAdSetRetryCnt[CMD_SYS_LTE] = 0;

	/*全配下REのMTアドレス設定状態←未処理*/
	/*Loop all RE*/
	for( lwLoop = CMD_CPRINO_MIN; lwLoop <= CMD_CPRINO_MAX; lwLoop++)
	{
		/*Set MTアドレス設定状態*/
		f_trp_rec_Fc02_t_mtsetsts( lwLoop, EC_REC_MTADDSET_STN_INI );
	}

	return;
}

/****************************************************************************/
/*!
 * @brief  MTアドレス設定要求(REC主導)再送処理
 * @note   MTアドレス設定要求(REC主導)再送処理を行う
 * @param  awCpriNo [in] the CPRI No
 * @return None
 * @date   2015/11/08 TDIPS)Takeuchi create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_mtsetf_req_snd( USHORT awCpriNo )
{
	CMT_TSKIF_CPRIRCV_MTADDSETREQ	*bufp;
	INT								sit_ret;					/* 終了コード */
	CHAR							errdata[CMD_NUM48];

	/* 関数トレースログ */
	F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_C02_FOR_IT1
	printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

	/****************/
	/* バッファ取得 */
	/****************/
	sit_ret = f_trp_com_Falloc_buf( sizeof(*bufp), (VOID*)&bufp );

	/* NGの場合 */
	if (sit_ret != CMD_OK) {
#ifndef D_OPT_TRIF_BUFNGLOG_INVALID
		/* 無効処理履歴 */
		snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][siz:0x%08x][ret:0x%02x]Alloc buffer NG.", awCpriNo, sizeof(*bufp), sit_ret);
		D_REC_IVLINI();
		D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
#endif /* D_OPT_TRIF_BUFNGLOG_INVALID */
		return ;
	}

	/****************/
	/* バッファ設定 */
	/****************/
	bufp->cprircv_inf.link_num				= awCpriNo;								/* CPRI番号					*/
	bufp->cprircv_inf.length				= sizeof( CMT_CPRIF_MTADDSETREQ );		/* buffer length			*/
	memcpy( &bufp->cpridat_mtaddsetreq, &f_trp_rec_Wcom_MtAdSetInfo[CMD_SYS_LTE], sizeof(CMT_TSKIF_CPRIRCV_MTADDSETREQ) );

	/****************/
	/* l2/lpbへ転送 */
	/****************/
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_MTADDSETREQ) );

	return;
}

/****************************************************************************/
/*!
 * @brief  MTアドレスRetry(FHM)(FLD-MT)処理 (B process)
 * @note   MTアドレスRetry(FHM)処理を行う
 * 
 * @return None
 * @date   2015/07/30 FPT)Hieu create.
 * @date   2015/11/08 TDIPS)Takeuchi 技説QA120
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_mtsetfres_fhm( )
{
	USHORT lwRetry;
    CHAR  errdata[CMD_NUM48];
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif
	/*get MTアドレス設定要求(FHM主導)再送カウンタ*/
	lwRetry = f_trp_rec_Fc02_t_tc_mtsetfreq_get();

	/*alt MTアドレス設定要求(FHM主導)再送カウンタ < (変数名C)MTアドレス設定要求再送回数 ? */
	if( lwRetry < D_TCM_SYSDFT_FLDMTADR_RTNUM )
	{
		 /**********************************************/
	    /* スレッド間(MTアドレス設定(FLD-MT)要求)送信(FHM主導) */
	    /**********************************************/
	    f_trp_rec_Fc02_t_it_mtsetfreq_snd(                            /* なし:R */
	    );

		/**************/
	    /* タイマ登録 */
	    /**************/
	    f_trp_rec_Fc02_t_timer_ent(                                   /* なし:R */
	        EC_REC_TMK_MT_ADD_SET,                /* MTアドレス設定確認タイマ:I */
	        D_REC_CPR_IV                                      /* スロット番号:I */
	    );

		lwRetry++;
		/* 残トライ回数(MTアドレス(FLD-MT)設定要求)設定 */
		f_trp_rec_Fc02_t_tc_mtsetfreq_set( lwRetry );
	}
	else
	{
        snprintf(errdata, CMD_NUM48, "[FRCRST][cpr:%02d]f_trp_rec_Fc02_m_res_mtsetfres_fhm NG.", D_REC_C02_CPR_NO());
        D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN2, errdata);

		 /******************************************************/
        /* スレッド間(RE強制リセットプロシージャ実行指示)送信 */
        /******************************************************/
        f_trp_rec_Fc02_t_it_refrcrstprc_snd(                  /* なし:R */
        );
	}
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s End \n",__LINE__,__FUNCTION__);
#endif
}

/****************************************************************************/
/*!
 * @brief  MTアドレスRetry(FHM)(SV-MT)処理 (B process)
 * @note   MTアドレスRetry(FHM)処理を行う
 * 
 * @return None
 * @date   2015/09/15 FPT)Yen Create
 * @date   2015/11/08 TDIPS)Takeuchi 技説QA120
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_mtsetsres_fhm( )
{
	USHORT lwRetry;
    CHAR  errdata[CMD_NUM48];
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif
	/*get MTアドレス設定要求(FHM主導)再送カウンタ*/
	lwRetry = f_trp_rec_Fc02_t_tc_mtsetsreq_get();

	/*alt MTアドレス設定要求(FHM主導)再送カウンタ < (変数名C)MTアドレス設定要求再送回数 ? */
	if( lwRetry < D_TCM_SYSDFT_SVMTADR_RTNUM )
	{
		 /**********************************************/
	    /* スレッド間(MTアドレス設定(SV-MT)要求)送信 */
	    /**********************************************/
	    f_trp_rec_Fc02_t_it_mtsetsreq_snd(                            /* なし:R */
	    );

		/**************/
	    /* タイマ登録 */
	    /**************/
	    f_trp_rec_Fc02_t_timer_ent(                                   /* なし:R */
	        EC_REC_TMK_MT_ADD_SET,                /* MTアドレス設定確認タイマ:I */
	        D_REC_CPR_IV                                      /* スロット番号:I */
	    );

		lwRetry++;
		/* 残トライ回数(MTアドレス(SV-MT)設定要求)設定 */
		f_trp_rec_Fc02_t_tc_mtsetsreq_set( lwRetry );
	}
	else
	{
        snprintf(errdata, CMD_NUM48, "[FRCRST][cpr:%02d]f_trp_rec_Fc02_m_res_mtsetsres_fhm NG.", D_REC_C02_CPR_NO());
        D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN2, errdata);

		 /******************************************************/
        /* スレッド間(RE強制リセットプロシージャ実行指示)送信 */
        /******************************************************/
        f_trp_rec_Fc02_t_it_refrcrstprc_snd(                  /* なし:R */
        );
	}
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s End \n",__LINE__,__FUNCTION__);
#endif
}

/****************************************************************************/
/*!
 * @brief  Send MTアドレス設定応答
 * @note   Send MTアドレス設定応答
 * @param  awResutl [in] 応答結果
 * @return None
 * @date   2015/07/30 FPT)Hieu create.
 * @date   2015/09/27 TDI)satou L3/rctに送信するように修正
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_mtsetf_res_snd( USHORT awResutl)
{
    CMT_TSKIF_CPRIRCV_MTADDSETRES ltRes;

    /*data seting*/
    memset(&ltRes, 0, sizeof(ltRes));
    ltRes.head.uiSignalkind               = CMD_CPRID_MTADDSETRES + CMD_SYS_LTE;
    ltRes.cpridat_mtaddsetres.signal_kind = CMD_CPRID_MTADDSETRES + CMD_SYS_LTE;
    ltRes.cpridat_mtaddsetres.result      = awResutl;

    /* Send msg [MTアドレス設定応答] to L3/rct */
    f_trp_rec_Fcom_t_l3_msg_fw(&ltRes, sizeof(ltRes));
}

/****************************************************************************/
/*!
 * @brief  「MTアドレス設定確認タイマ」(FLD-MT)タイムアウト(FHM)
 * @note   「MTアドレス設定確認タイマ」(FLD-MT)タイムアウト(FHM)
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/07/30 FPT)Hieu create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_mtsetf_fhm_to( VOID* bufp )
{
	/*MTアドレスRetry(FHM)(FLD-MT)処理を行う*/
	f_trp_rec_Fc02_m_res_mtsetfres_fhm( );
	return;
}

/****************************************************************************/
/*!
 * @brief  「MTアドレス設定確認タイマ」(SV-MT)タイムアウト(FHM)
 * @note   「MTアドレス設定確認タイマ」(SV-MT)タイムアウト(FHM)
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/09/15 FPT)Yen create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_mtsets_fhm_to( VOID* bufp )
{
	/*MTアドレスRetry(FHM)(SV-MT)処理を行う*/
	f_trp_rec_Fc02_m_res_mtsetsres_fhm( );
	return;
}

/****************************************************************************/
/*!
 * @brief  「MTアドレス設定確認タイマ」タイムアウト(REC主導)
 * @note   「MTアドレス設定確認タイマ」タイムアウト(REC主導)
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/07/30 FPT)Hieu create.
 * @date   2015/10/28 FPT)Yen update fix bug IT2 No 140
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_mtsetf_rec_to( VOID* bufp )
{
	USHORT		lwSystemType;

	lwSystemType = ((CMT_TSKIF_CPRISND_ADDSETTOREQ *)bufp)->cpridat_addsettoreq.signal_kind & CMD_SYS_CHK_CPRID;

	if(CMD_SYS_S3G == lwSystemType)
	{
		/* MTアドレス設定応答(REC)処理 (A process) */
		f_trp_rec_Fc02_m_res_mtsetres_rec( );
	}
	else
	{
		/* MTアドレス設定応答(REC)処理 (A process) */
		f_trp_rec_Fc02_m_res_mtsetres_rec_3g( );
	}

	return;
}
/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
