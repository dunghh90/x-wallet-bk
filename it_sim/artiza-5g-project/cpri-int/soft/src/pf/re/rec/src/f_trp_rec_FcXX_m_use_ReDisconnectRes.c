/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_ReDisconnectRes.c
 * @brief  
 * @date   2015/08/05 FPT)Tuan create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015-2016
 */
/****************************************************************************/
#include "f_trp_rec.h"
#include "f_sys_inc.h"
#include "m_cm_header.h"
/*! @addtogroup TRIF_REC
 * @{ */
/****************************************************************************/
/*!
 * @brief  Receive msg Re disconnect request from l3/rct
 * @note   Receive msg Re disconnect request from l3/rct
 *         - send Re disconnect respone to l3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @Bug_No M-RRU-ZSYS-01770
 * @Bug_No M-RRU-ZSYS-01971
 * @date   2015/08/05 FPT)Tuan create
 * @date   2015/08/28 FPT)Yen add for 7-1-8 配下REとのCPRIリンク断
 * @date   2015/09/17 TDI)satou 状態遷移先を修正
 * @date   2015/10/12 TDI)satou M-RRU-ZSYS-01770 問処番号No.75対応
 * @date   2015/10/15 FPT)Duong update review comment 133
 * @date   2015/11/10 TDI)enoki M-RRU-ZSYS-01971 問処番号No.165対応 RE起動中にL3断検出時に、運用中ならば運用中アイドルにするように
 * @date   2015/11/14 TDI)satou 問処番号No.183
 * @date   2015/11/17 FPT)Tuan fix bug.
 * @date   2016/03/03 tdips)enoki RE自律リセットはLTE側処理のみのため、3G時はf_trp_rec_ReDisConn_AutoRst_3g()をCallしない.
 * @date   2016/10/06 KCN)Takagi 16B機能追加
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_ReDisconnectRes(                        /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
	CMT_TSKIF_CPRIDISCONN_RES	rsp_p;
	USHORT						lwReNo;
	VOID						*shm_msgp;
    INT							endcd;
	UINT						keyID;
	INT							ret __attribute__((unused));
//	T_RRH_CPRI_STATE			ltCpriSta;
	CMT_TSKIF_REFILNEWSTP		ltReFilNewStp;
    INT    						sit_ret;            /* 終了コード */
    CHAR  						errdata[CMD_NUM48];
	USHORT						lwLength;
	USHORT                      system_type;
	USHORT                      layer_kind;
	ET_REC_C02_STN				stn;
	USHORT						crr_no;

	if( NULL == bufp )
	{
		return;
	}
	
	lwReNo      = ((CMT_TSKIF_REDIS_REQ*)bufp)->link_num;
	system_type = ((CMT_TSKIF_REDIS_REQ*)bufp)->system_type;
	layer_kind  = ((CMT_TSKIF_REDIS_REQ*)bufp)->layer_kind;

	/*
	 * LTEのリンク断、つまりはLayer1/2断orLayer3断(S3G)の場合、
	 * REファイル更新中止を要求する.
	 */
//	if ((E_RRH_RAYER_KIND_1_2 == layer_kind) || (CMD_SYS_S3G == system_type)) {
    /* REファイル更新中止要求(リンク断CPRI#) */
    ltReFilNewStp.head.uiEventNo = CMD_TSKIF_REFIRM_DL_STOP_REQ;
    ltReFilNewStp.cpri_no = lwReNo;
	ltReFilNewStp.systerm = system_type;

    /******************/
    /* スレッド間送信 */
    /******************/
    lwLength = sizeof(CMT_TSKIF_REFILNEWSTP);

    /* REファイル更新中止要求を送信 */
    sit_ret = f_trp_rec_Fcom_t_msg_snd( &ltReFilNewStp,
                            CMD_TSKIF_REFIRM_DL_STOP_REQ,
                            D_RRH_PROCQUE_L3,
                            0, lwLength);

    /* NGの場合 */
    if (sit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][ret:0x%02x]IT send NG.", lwReNo, sit_ret);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);
    }
//	}

	/*
	 * Layer1/2断の場合、3G/LTEともにリンク断の処理を行う.
	 * Layer3断の場合、該当のリンク種別のみリンク断の処理を行う.
	 */
	if (E_RRH_RAYER_KIND_1_2 == layer_kind) 
	{

	    f_trp_rec_Wc02_prm_tbl.cpr_no = lwReNo;
	    f_trp_rec_Wc02_prm_tbl_3g.cpr_no = lwReNo;
		/* 対RE状態#n(※1)をRE起動中(L2確立前)へ遷移 */
		f_trp_rec_Fc02_t_ownstn_set_3g(                              /* なし:R  */
		        EC_REC_C02_STN_RES_IDL              /* (04)RE起動待ちアイドル:I */
	        );
		/* 対RE状態#n(※1)をRE起動中(L2確立前)へ遷移 */
		f_trp_rec_Fc02_t_ownstn_set(                                 /* なし:R  */
		        EC_REC_C02_STN_RES_IDL              /* (04)RE起動待ちアイドル:I */
	        );

		/* キャリア状態不一致補正について、タイマ停止と状態初期化 */
		f_trp_rec_Wc03_prm_tbl.cpr_no = lwReNo;
		for (crr_no = CMD_CARNO_MIN; crr_no <= CMD_CARNO_MAX; crr_no++)
		{
			f_trp_rec_Wc03_prm_tbl.crr_no = crr_no;
			f_trp_rec_Fc03_t_timer_ccl(EC_REC_TMK_TXS_CNF);
			f_trp_rec_Fc03_t_timer_ccl(EC_REC_TMK_TXR_CNF);
			f_trp_rec_Fc03_t_ownstn_set(EC_REC_C03_STN_IDLE);
		}
		f_trp_rec_Wc03_prm_tbl_3g.cpr_no = lwReNo;
		for (crr_no = CMD_CARNO_MIN; crr_no <= CMD_CARNO_MAX_3G; crr_no++)
		{
			f_trp_rec_Wc03_prm_tbl_3g.crr_no = crr_no;
			f_trp_rec_Fc03_t_timer_ccl_3g(EC_REC_TMK_TXS_CNF);
			f_trp_rec_Fc03_t_timer_ccl_3g(EC_REC_TMK_TXR_CNF);
			f_trp_rec_Fc03_t_ownstn_set_3g(EC_REC_C03_STN_IDLE);
		}

		if( CMD_SYS_S3G == system_type ) {
			f_trp_rec_ReDisConn_AutoRst_lte( lwReNo );
		}
//		else{
//			f_trp_rec_ReDisConn_AutoRst_3g( lwReNo );
//		}
	} 
	else if (CMD_SYS_S3G == system_type) 
	{
	    f_trp_rec_Wc02_prm_tbl.cpr_no = lwReNo;
		/* 対RE状態#n(※1)が運用中状態ならば、RE運用中アイドルへ遷移 */
		stn = f_trp_rec_Fcom_t_c02stn_get( D_REC_C02_CPR_NO() );
		if( EC_REC_C02_STN_USE == stn )
		{
		f_trp_rec_Fc02_t_ownstn_set(                                /* なし:R  */
	            EC_REC_C02_STN_USE_IDL              /* RE運用中アイドル:I      */
	        );
		}
//		f_trp_rec_ReDisConn_AutoRst_lte( lwReNo );

		/* キャリア状態不一致補正について、タイマ停止と状態初期化 */
		f_trp_rec_Wc03_prm_tbl.cpr_no = lwReNo;
		for (crr_no = CMD_CARNO_MIN; crr_no <= CMD_CARNO_MAX; crr_no++)
		{
			f_trp_rec_Wc03_prm_tbl.crr_no = crr_no;
			f_trp_rec_Fc03_t_timer_ccl(EC_REC_TMK_TXS_CNF);
			f_trp_rec_Fc03_t_timer_ccl(EC_REC_TMK_TXR_CNF);
			f_trp_rec_Fc03_t_ownstn_set(EC_REC_C03_STN_IDLE);
		}
	}
	else 
	{
	    f_trp_rec_Wc02_prm_tbl_3g.cpr_no = lwReNo;
		/* 対RE状態#n(※1)が運用中状態ならば、RE運用中アイドルへ遷移 */
		stn = f_trp_rec_Fcom_t_c02stn_get_3g( D_REC_C02_3G_CPR_NO() );
		if( EC_REC_C02_STN_USE == stn )
		{
		f_trp_rec_Fc02_t_ownstn_set_3g(                              /* なし:R */
	            EC_REC_C02_STN_USE_IDL              /* RE運用中アイドル:I      */
	        );
		}
//		f_trp_rec_ReDisConn_AutoRst_3g( lwReNo );

		/* キャリア状態不一致補正について、タイマ停止と状態初期化 */
		f_trp_rec_Wc03_prm_tbl_3g.cpr_no = lwReNo;
		for (crr_no = CMD_CARNO_MIN; crr_no <= CMD_CARNO_MAX_3G; crr_no++)
		{
			f_trp_rec_Wc03_prm_tbl_3g.crr_no = crr_no;
			f_trp_rec_Fc03_t_timer_ccl_3g(EC_REC_TMK_TXS_CNF);
			f_trp_rec_Fc03_t_timer_ccl_3g(EC_REC_TMK_TXR_CNF);
			f_trp_rec_Fc03_t_ownstn_set_3g(EC_REC_C03_STN_IDLE);
		}
	}
	/************************************************************************/
	rsp_p.link_num = lwReNo;
	
	/* Communication Shared Memory Address Get. */	
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(
										E_BPF_RU_IPCM_BUFF_KIND_TASK,		/*!< Buffer種別					*/
										sizeof(CMT_TSKIF_CPRIDISCONN_RES),	/*!< size						*/
										(void **)&shm_msgp,					/*!< 通信用共有メモリのポイン繧?*/
										&endcd);							/*!< エラーコード				*/
	
	if(shm_msgp == D_RRH_NULL)
	{
		/*access log*/
		f_com_assert(endcd,"fail to BPF_RU_IPCM_PROCMSG_ADDRGET");
		return ;
	}

	/*temp save key*/
	keyID = ((T_SYS_COMMON_THDIF*)shm_msgp)->uiKeyID;
	
	/* Update msg */

	/* SrcPQueueID：D_RRH_PROCQUE_RE */
	/* SrcTQueueID：Source thread ID */
	/* DstPQueueID：Destination process ID */
	/* DstTQueueID：Destination thread ID */
	/* Length ：Message size */
	memcpy(shm_msgp,&rsp_p,sizeof(CMT_TSKIF_CPRIDISCONN_RES));
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiDstTQueueID = 0;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiSrcTQueueID = CMD_TSKID_REC;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiDstPQueueID = D_RRH_PROCQUE_L3;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiSrcPQueueID = D_RRH_PROCQUE_RE;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiKeyID = keyID;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiLength = sizeof(CMT_TSKIF_CPRIDISCONN_RES);		
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiEventNo = CMD_TSKIF_RE_DISCONNECT_RSP;
		
	/************************************************************************/
	/* 他プロセスへメッセージを送信する										*/
	/************************************************************************/
	ret = BPF_RU_IPCM_MSGQ_SEND(
							D_RRH_PROCQUE_L3,			/* 直近の送信先QueueID( Thread Queue ID or Process Queue ID )		*/
							(void *)shm_msgp	/* 送信メッセー繧?BPF_RU_IPCM_PROCMSG_ADDRGET)で取得したアドレ繧?*/
							);

	if(ret != BPF_RU_IPCM_OK)
	{
		f_com_assert(ret,"fail to send msg");
		return;
	}
}


/****************************************************************************/
/*!
 * @brief  Process for lte when receive (0x4007)RE Disconnecion Request
 * @note   
 * @param  awReNo [in]  ReNo
 * @return None
 * @date   2015/11/17 FPT)Tuan create.
 * @date   2016/03/03 tdips)enoki 断検出による自律リセット抑止解除を削除(別トリガへ).
 * @date   2016/10/06 KCN)Takagi 16B機能追加
 */
/****************************************************************************/
VOID f_trp_rec_ReDisConn_AutoRst_lte(USHORT awReNo)
{
    CHAR                      errdata[CMD_NUM48];
// [16B] add start
	USHORT slt_cnt;   /* スロット番号(index値:0-15) */
    USHORT slot_no;   /* スロット番号(1-56) */
    USHORT slot_num;
// [16B] add end

// [16B] add start
    // スロット数を抽出
    slot_num = f_trp_rec_Wcom_remsc_tbl.remsc[awReNo-1].resltinf.slt_num;
    for(slt_cnt=0; slt_cnt < slot_num; slt_cnt++) {
// [16B] add end
#if 0   // [16B] del start
    	/* RE自律リセット実施による断検出の場合 */
    	if( CMD_FLG_ON == f_trp_com_AutoResetMngTbl[awReNo - 1].ReAutoRstCtlFlg )
    	{
    		/* RE自律リセット抑止フラグON */
    		f_trp_com_AutoResetMngTbl[awReNo - 1].ReAutoRstPreventFlg = CMD_FLG_ON;

    		/* RE自律リセット制御中フラグON */
    		f_trp_com_AutoResetMngTbl[awReNo - 1].ReAutoRstCtlFlg = CMD_FLG_OFF;
    		
    		snprintf(errdata, CMD_NUM48, "[cpr:%02d][LTE]ReAutoRstPreventFlg on.",awReNo);
    		D_REC_IVLINI();
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
    	}
#endif  // [16B] del end

    	/* RE自律リセット待ちタイマ中の断検出の場合 */
    	if( CMD_FLG_ON == f_trp_com_AutoResetMngTbl[awReNo - 1][slt_cnt].ReAutoRstTimFlg )
    	{
// [16B] add start
            slot_no = f_trp_rec_Wcom_remsc_tbl.remsc[awReNo-1].resltinf.slt_no[slt_cnt];

// [16B] add end
            
    		/* タイマ停止REカード自律リセット待ちタイマ */
    		f_trp_rec_Fc02_t_timer_ccl(		/* なし:R */
    			EC_REC_TMK_REAUTRST_WAT,	/* RE Auto Reset 送信待ちタイマ:I */
// [16B] chg start
//    			CMD_NUM1					/* スロット番号:I */
    			slot_no					/* スロット番号:I */
// [16B] chg end
    		);

    		/* REカード自律リセット待ちタイマOFF */
    		f_trp_com_AutoResetMngTbl[awReNo - 1][slt_cnt].ReAutoRstTimFlg = CMD_FLG_OFF;
    		
// [16B] chg start
    		snprintf(errdata, CMD_NUM48, "[cpr:%02d][slot_no:%02d][LTE]ReAutoRstTimFlg clear.",awReNo, slot_no);
// [16B] chg end
    		D_REC_IVLINI();
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
    	}
// [16B] add start
	}
// [16B] add end
	return;
}
	
	
/****************************************************************************/
/*!
 * @brief  Process for 3g when receive (0x4007)RE Disconnecion Request
 * @note   
 * @param  awReNo [in]  ReNo
 * @return None
 * @date   2015/11/17 FPT)Tuan create.
 */
/****************************************************************************/

VOID f_trp_rec_ReDisConn_AutoRst_3g(USHORT awReNo)
{
    CHAR                      errdata[CMD_NUM48];
	
	if( CMD_FLG_ON == f_trp_com_AutoResetMngTbl_3g[awReNo - 1].ReAutoRstCtlFlg )
	{
		/* RE自律リセット抑止フラグON */
		f_trp_com_AutoResetMngTbl_3g[awReNo - 1].ReAutoRstPreventFlg = CMD_FLG_ON;

		/* RE自律リセット制御中フラグON */
		f_trp_com_AutoResetMngTbl_3g[awReNo - 1].ReAutoRstCtlFlg     = CMD_FLG_OFF;
		
		snprintf(errdata, CMD_NUM48, "[cpr:%02d][3G]ReAutoRstPreventFlg on.",awReNo);
		D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
	}
	else
	{
		/* タイマ停止REカード自律リセット待ちタイマ */
		f_trp_rec_Fc02_t_timer_ccl_3g(		/* なし:R */
			EC_REC_TMK_REAUTRST_WAT,	/* RE Auto Reset 送信待ちタイマ:I */
			CMD_NUM1					/* スロット番号:I */
		);
		/* REカード自律リセット待ちタイマOFF */
		f_trp_com_AutoResetMngTbl_3g[awReNo - 1].ReAutoRstTimFlg = CMD_FLG_OFF;
		/* RE自律リセット時刻クリア */
		f_trp_com_AutoResetMngTbl_3g[awReNo - 1].resetTime = D_TCM_DEFAULT_RSTTIM;
		/* RE自律リセット抑止フラグOFF */
		f_trp_com_AutoResetMngTbl_3g[awReNo - 1].ReAutoRstPreventFlg = CMD_FLG_OFF;
		
		snprintf(errdata, CMD_NUM48, "[cpr:%02d][3G]ReAutoRstPreventFlg clear.",awReNo);
		D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
	}
	return;
}
/****************************************************************************/
/*!
 * @brief  Receive msg  RE SelfReset Prevent Off Request  from l3/rct
 * @note   Receive msg  RE SelfReset Prevent Off Request  from l3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2016/03/03 tdips)enoki create
 * @date   2016/10/06 KCN)Takagi 16B機能追加
 * @date   2016/11/18 FJT)ohashi 16B スロット数制限機能追加
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_ReSelfReset_PreventOff_Req(		/* なし:R */
					VOID* bufp )						/* バッファポインタ:I */
{
	CHAR						errdata[CMD_NUM48];
	USHORT						lwReNo;
// [16B] add start
	USHORT slt_idx;   /* スロット番号(index値:0-15) */
    USHORT slot_no;   /* スロット番号(1-56) */
    USHORT slot_num;
// [16B] add end
//16B スロット数制限機能追加 start
	T_RRH_SLOT_3G_RE     shmSlot3G;
	T_RRH_SLOT_S3G_RE    shmSlotS3G;
    USHORT reoff_flag;		/*RE減設Flag*/
//16B スロット数制限機能追加 end
	lwReNo= ((CMT_TSKIF_RE_SLFRST_PRVTOFF_REQ*)bufp)->link_num;
	reoff_flag= ((CMT_TSKIF_RE_SLFRST_PRVTOFF_REQ*)bufp)->reoff_flag;
// [16B] add start
	/*	修正効果確認要ログ追加	*/
	if((f_trp_rec_Wc02_prm_tbl.cpr_no != lwReNo) || (f_trp_rec_Wc02_prm_tbl_3g.cpr_no != lwReNo))
	{
		snprintf(errdata, CMD_NUM48, "[cpr:%02d]Wc02_cpr_no Deff(3Gcpr=%d, LTEcpr=%d).", 
					lwReNo,f_trp_rec_Wc02_prm_tbl.cpr_no, f_trp_rec_Wc02_prm_tbl_3g.cpr_no);
		D_REC_IVLINI();
		D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);
	}
    // スロット数を抽出
    slot_num = f_trp_rec_Wcom_remsc_tbl.remsc[lwReNo-1].resltinf.slt_num;

    for(slt_idx=0; slt_idx < slot_num; slt_idx++) {

        slot_no = f_trp_rec_Wcom_remsc_tbl.remsc[lwReNo-1].resltinf.slt_no[slt_idx];

        //  自律リセット待ちフラグがONなら、タイマ停止、自律リセット待ちフラグをOFF
    	if( CMD_FLG_ON == f_trp_com_AutoResetMngTbl[lwReNo - CMD_NUM1][slt_idx].ReAutoRstTimFlg )
        {
        	f_trp_rec_Wc02_prm_tbl.cpr_no = lwReNo;
			/***********************************************/
			/* タイマ取消 (REカード自律リセット待ちタイマ)**/
			/***********************************************/
			f_trp_rec_Fc02_t_timer_ccl(                            /* なし:R */
				EC_REC_TMK_REAUTRST_WAT,   /* RE Auto Reset 送信待ちタイマ:I */
		 		slot_no                                    /* スロット番号:I */
			);
            f_trp_com_AutoResetMngTbl[lwReNo - CMD_NUM1][slt_idx].ReAutoRstTimFlg = CMD_FLG_OFF;

        }
// [16B] add end

    	/* RE自律リセット抑止状態の場合 */
    	if( CMD_FLG_OFF != f_trp_com_AutoResetMngTbl[lwReNo - 1][slt_idx].ReAutoRstPreventFlg )
    	{
    		/* RE自律リセット時刻クリア */
    		f_trp_com_AutoResetMngTbl[lwReNo - 1][slt_idx].resetTime = D_TCM_DEFAULT_RSTTIM;
    		/* RE自律リセット抑止フラグOFF */
    		f_trp_com_AutoResetMngTbl[lwReNo - 1][slt_idx].ReAutoRstPreventFlg = CMD_FLG_OFF;
    		
//  		snprintf(errdata, CMD_NUM48, "[cpr:%02d][LTE]ReAutoRstPreventFlg clear.",lwReNo);
   	    	snprintf(errdata, CMD_NUM48, "[cpr:%02d][slot_no:%02d][LTE]ReAutoRstPreventFlg clear.",lwReNo,slot_no); /* pgr0570 */
    		D_REC_IVLINI();
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
    	}else{
//	    	snprintf(errdata, CMD_NUM48, "[cpr:%02d][LTE]ReAutoRstPreventFlg=%d", lwReNo, f_trp_com_AutoResetMngTbl[lwReNo - 1].ReAutoRstPreventFlg);
    		snprintf(errdata, CMD_NUM48, "[cpr:%02d][slot_no:%02d][LTE]ReAutoRstPreventFlg=%d", lwReNo,slot_no, f_trp_com_AutoResetMngTbl[lwReNo - 1][slt_idx].ReAutoRstPreventFlg);
    		D_REC_IVLINI();
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
    	}
// [16B] add start
	}
// [16B] add end
//16B スロット数制限機能追加 start

//RE減設時のスロット数初期化処理
//RE減設時に限りスロット情報をクリアする
	if(reoff_flag == CMD_ON){
		
		f_trp_rec_Wc02_prm_tbl.cpr_no = lwReNo;
		f_trp_rec_Wc02_prm_tbl_3g.cpr_no = lwReNo;
		
	    /* RE状態(セクタ部)管理テーブル(REスロット情報 S3G)初期化 */
	    f_trp_rec_Fc02_t_resltinfsc_ini(D_REC_INK_INI);
		
		/*該当REの共有メモリ初期化(スロット情報 S3G)*/
		memset(&shmSlotS3G,       0, sizeof shmSlotS3G      );
	    f_cmn_com_slot_s3g_set(lwReNo, &shmSlotS3G);
	    
	    /* RE状態(セクタ部)管理テーブル(REスロット情報 3G)初期化 */
		f_trp_rec_Fc02_t_resltinfsc_ini_3g(D_REC_INK_INI);
	    
		/*該当REの共有メモリ初期化(スロット情報 3G)*/
		memset(&shmSlot3G,        0, sizeof shmSlot3G       );
	    f_cmn_com_slot_3g_set(lwReNo, &shmSlot3G);
	}
//16B スロット数制限機能追加 end

}

/* @} */ 
