/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReCardCnt.c
 *  @brief  response to RE card control request, carry out RE card control process
 *  @date   2008/07/25 FFCS)linlj create
 *  @date   2008/10/08 FFCS)Linlj (M-S3G-eNBPF-00898) card control kind is different for S3G
 *  @date   2009/02/06 FFCS)Linlj (M-S3G-eNBPF-01318) remote reset no response before card reset
 *  @date   2009/08/08 FFCS)Wangjuan modify for 
 *							  CR-00058-005(CPRI spec V1.0.9) 
 *  @date   2010/03/12 QNET)Kabasima CR-xxxxx-xxx [DCM向け]TS-145-ALM制御時のRE動作の件
 *  @date   2011/09/23 FFCS)Linlj  modify for 省電力対応
 *  @date   2011/10/24 FJT)Tokunaga modify for M-S3G-eNBPF-04078対応
 *  @date   2012/07/11 FFCS)Xut modify for CeNB-F-083-028対応
 *  @date	2012/10/22 FFCS)Zhangfeng modify for M-S3G-eNBPF-04288
 *  @date   2013/11/19 FFCS)Xut modify for ZYNQ.
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2016
 */
/********************************************************************************************************************/

/** @addtogroup RRH_L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"
#include "rrhApi_Svp.h"

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    RE card control process
 *  @note     m_rc_ReCardCntChkの中でInパラがLTEメッセージの場合、制御パラが3G用に変換される
 *  @param    parm_p [in] the buffer address pointer of received message
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)linlj create
 *  @date     2008/10/08 FFCS)Linlj (M-S3G-eNBPF-00898) card control kind is different for S3G 
 *  @date     2009/02/06 FFCS)Linlj (M-S3G-eNBPF-01318) remote reset no response before card reset 
 */
/********************************************************************************************************************/


VOID m_rc_ReCardCnt(CMT_TSKIF_CPRIRCV_RECARDCNTREQ *parm_p)
{
    CMT_TSKIF_CPRISND_RECARDCNTRES *rsp_p;  /* タスク間フレームポインタ     */
    USHORT chk_rslt;                        /* 条件チェック結果             */
    UINT bufget_rslt;                       /* バッファ獲得結果             */
    UINT sigchk_rslt;                       /*  result of checking signal */
    USHORT sigsys_rslt;                     /*  system by checking signal */

    /* validate signal */
    sigchk_rslt = m_cm_chksig(parm_p->cpridat_recardcntreq.signal_kind,&sigsys_rslt);
    if(sigchk_rslt != CMD_RES_OK)
    {
        cm_Assert(	CMD_ASL_USELOW,
                    sigsys_rslt,
                    "validate signal NG");
    }
    /* ポインタ初期化 */
    rsp_p = NULL;

    /****************/
    /* バッファ獲得 */
    /****************/
    bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
                                sizeof(CMT_TSKIF_CPRISND_RECARDCNTRES),
                                CMD_NUM1,
                                (VOID **)&rsp_p	);
    /********************/
    /* バッファ獲得失敗 */
    /********************/
    if(bufget_rslt != CMD_RES_OK)
    {
        /* ASSERT */
        cm_Assert(	CMD_ASL_USELOW,
                    bufget_rslt,
                    "m_rc_ReCardCnt cm_L2BfGet NG");
        return;
    }

    /* CPRI信号応答結果にOKを設定 */
    rsp_p->cpridat_recardcntres.result = CMD_NML;

    /* CPRI信号種別設定 */
    rsp_p->cpridat_recardcntres.signal_kind = CMD_CPRID_RECARDCNTRES + sigsys_rslt;

    /* CPRI信号応答スロット番号設定(インパラメータのスロット番号を設定) */
    rsp_p->cpridat_recardcntres.slot_no = parm_p->cpridat_recardcntreq.slot_no;

    /**********************/
    /* 信号種別アンマッチ */
    /**********************/
    if(sigchk_rslt != CMD_RES_OK)
    {
        if(CMD_SYS_S3G != sigsys_rslt) {
            rsp_p->cpridat_recardcntres.result = CMD_OTHR_ERR_RE;
        } else {
            rsp_p->cpridat_recardcntres.result = CMD_HDL_NG;
        }
        /**********************/
        /* タスク間ヘッダ作成 */
        /**********************/
        rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(CMT_CPRIF_RECARDCNTRES);

        /* CPRI信号をLAPBタスクに送信 */
        m_cm_L3MsgSend_REC(rsp_p);

        return;
    }

	/****************************************/
	/* パラメータ範囲チェックと条件チェック */
	/****************************************/
	/* REカード制御条件チェック関数コール */
	chk_rslt = m_rc_ReCardCntChk(parm_p, sigsys_rslt);

	/****************************/
	/* 条件チェックが正常の場合 */
	/****************************/
	if(chk_rslt == CMD_OK)
	{
		/************************************/
		/* カード制御実施(自カードリセット) */
		/************************************/
		(VOID) m_rc_ReCardCntExec(parm_p, rsp_p, sigsys_rslt);

        /* (m_rc_ReCardCntExec内でLAPBタスクにCPRI信号送信済み) */
        /* Main向けにOKを返却 */
        return;
	}

	/****************************/
	/* 条件チェックが異常の場合 */
	/****************************/
	else
	{
		/* CPRI信号応答結果にNGを設定 */
		rsp_p->cpridat_recardcntres.result = chk_rslt;
	}

    /**********************/
    /* タスク間ヘッダ作成 */
    /**********************/
    rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(CMT_CPRIF_RECARDCNTRES);

    /* CPRI信号をLAPBタスクに送信 */
    (VOID)m_cm_L3MsgSend_REC(rsp_p);
	
	return ;
}

/********************************************************************************************************************/
/**
 *  @brief    RE card control forward resonse process
 *  @note     RE card control forward RE-REC resonse process
 *  @param    parm_p [in] the buffer address pointer of received message
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 M&C)Huan.dh create
 */
/********************************************************************************************************************/


VOID m_rc_ReCardCntRes(CMT_TSKIF_CPRIRCV_RECARDCNTRES *parm_p)
{
    CMT_TSKIF_CPRISND_RECARDCNTRES *rsp_p;  /* タスク間フレームポインタ     */
    USHORT chk_rslt;                        /* 条件チェック結果             */
    UINT bufget_rslt;                       /* バッファ獲得結果             */
    UINT sigchk_rslt;                       /*  result of checking signal */
    USHORT sigsys_rslt;                     /*  system by checking signal */

    /* validate signal */
    sigchk_rslt = m_cm_chksig(parm_p->cpridat_recardcntres.signal_kind,&sigsys_rslt);
    if(sigchk_rslt != CMD_RES_OK)
    {
        cm_Assert(	CMD_ASL_USELOW,
                    sigsys_rslt,
                    "validate signal NG");
    }
    /* ポインタ初期化 */
    rsp_p = NULL;

    
    /**********************/
    /* 信号種別アンマッチ */
    /**********************/
    if(sigchk_rslt != CMD_RES_OK)
    {
        /****************/
        /* バッファ獲得 */
        /****************/
        bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
                                sizeof(CMT_TSKIF_CPRISND_RECARDCNTRES),
                                CMD_NUM1,
                                (VOID **)&rsp_p	);

        if(CMD_SYS_S3G != sigsys_rslt) {
            rsp_p->cpridat_recardcntres.result = CMD_OTHR_ERR_RE;
        } else {
            rsp_p->cpridat_recardcntres.result = CMD_HDL_NG;
        }
        /**********************/
        /* タスク間ヘッダ作成 */
        /**********************/
        rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(CMT_CPRIF_RECARDCNTRES);

        /* CPRI信号をLAPBタスクに送信 */
        m_cm_L3MsgSend_REC(rsp_p);

        return;
    }

    /* CPRI信号をLAPBタスクに送信 */
    (VOID)m_cm_L3MsgSend_REC(parm_p);
	
	return ;
}

/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    RE card control parameter check, card ALM check too
 *  @note     RE card control parameter check card ALM check too
 *  @param    parm_p [in] the buffer address pointer of received message
 *  @param    sigsys_rslt [in] 3G/LTE
 *  @return   RETURN CODE
 *  @retval   CMD_OK							0: 	Normal end
 *  @retval   CMD_OTHR_ERR_RE			42: 	Others error
 *  @retval   CMD_CNT_NG_RE				43: 	RE control NG
 *  @date     2008/07/25 FFCS)linlj create
 *  @date     2009/08/08 FFCS)Wangjuan modify for 
 *							  CR-00058-005(CPRI spec V1.0.9) 
 *  @date     2011/09/23 FFCS)Linlj  modify for 省電力対応
 *  @date     2011/10/31 FJT)Tokunaga modify for M-S3G-eNBPF-04081対応
 *  @date     2011/11/15 COMAS)Uchida modify for M-S3G-eNBPF-04087対応
 *  @date     2015/07/29 TDI)satou スリープ制御条件チェック処理を削除
 *  @date     2021/03/23 M&C)Tri.hn Fix bug IT 1-3-20-1 (No.19) (remove checking for slot no)
 *  @TBD_No   REカード制御はリセットのみ受け付けるように変更する
*/
/********************************************************************************************************************/
USHORT m_rc_ReCardCntChk(CMT_TSKIF_CPRIRCV_RECARDCNTREQ *parm_p, USHORT sigsys_rslt)
{
    CHAR  log_data[CMD_NUM64];              /* アサート情報収集用エリア     */
    UINT   slotchk_rslt;                    /* スロット番号チェック結果     */
    USHORT  retcode = CMD_OK;               /* 戻り値判定用                 */

	/****************/
	/* 範囲チェック */
	/****************/

	/* 制御が有効範囲外(1以外)の場合 */
	/*****************************************************************/
	/* S3GとRRHで制御パラメータ値が異なるものはここでRRHの値に統一する */
	/*****************************************************************/
	retcode = m_rc_ReCardCntChk_CntKind(parm_p, sigsys_rslt);
	if(retcode != CMD_OK)
	{
		return retcode;
	}

	// /* スロット番号範囲チェック関数コール	*/
	// slotchk_rslt = m_rc_cm_SlotNoChk(parm_p->cpridat_recardcntreq.slot_no);
	
	// /* スロット番号範囲チェック結果判定		*/
	// if(slotchk_rslt != CMD_OK)
	// {
	// 	/* ログデータに要因をコピー */
	// 	sprintf(	log_data, 
	// 				"m_rc_ReCardCnt ParmCHK NG 4 %d %d %d ",
	// 				parm_p->cpridat_recardcntreq.slot_no, 
	// 				parm_p->cpridat_recardcntreq.cnt_kind,
	// 				parm_p->cpridat_recardcntreq.cnt_code);

	// 	/* アサート情報出力 */
	// 	cm_Assert(	CMD_ASL_USELOW,
	// 				CMD_NUM0, 
	// 				log_data);

	// 	if (CMD_SYS_S3G == sigsys_rslt) {
	// 	    return CMD_HDL_NG;
	// 	} else {
	// 	    return CMD_OTHR_ERR_RE;
	// 	}
	// }

	return retcode;
}

/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    Execute RE card control
 *  @note     Execute RE card control
 *  @param    parm_p [in] the buffer address pointer of received message
 *  @param    rsp_p [out] the buffer address pointer of response message 
 *  @param    sigsys_rslt [in] 3G/LTE
 *  @return   RETURN CODE
 *  @retval   RCD_MDIF_RST				1: 	Self card reset
 *  @date     2008/07/25 FFCS)linlj create
 *  @date     2010/03/12 QNET)Kabasima CR-xxxxx-xxx [DCM向け]TS-145-ALM制御時のRE動作の件
 *  @date     2011/09/23 FFCS)Linlj  modify for 省電力対応
 *  @date     2012/01/12 FJT)Tokunaga M-S3G-eNBPF-04091
 *                       CeNB-F-075-061(F)(F-1256(DCM)回答：BRE(RF共用)装置におけるUSE条件およびセクタ障害警報信号送信条件について)
 *  @date     2012/02/20 ALPHA)Oone SLP安定化時間対応
 *  @date     2012/07/11 FFCS)Xut modify for CeNB-F-083-028対応
 *  @date     2012/10/22 FFCS)Zhangfeng modify for M-S3G-eNBPF-04288
 *  @date     2013/11/19 FFCS)Xut modify for ZYNQ.
 *  @date     2015/09/01 TDI)satou リセット制御のみ受け付けるように
 *  @date     2015/09/01 TDI)satou REカード制御要求を配下REに転送する処理を追加
 *  @date     2015/10/06 TDI)satou 「REリセット要求後にstateBに落ちる時間が遅い（4秒かかっている） IT3」の問題対処
 *  @date     2016/10/06 KCN)Takagi 16B機能追加
 *  @date     2021/01/13 M&C)Huan.dh disable intermmediate device self reset 
 *  @date     2021/03/23 M&C)Tri.hn Fix bug IT 1-3-20-1 (No.19) (remove checking for slot no max)
 */
/********************************************************************************************************************/
USHORT m_rc_ReCardCntExec(CMT_TSKIF_CPRIRCV_RECARDCNTREQ *parm_p, CMT_TSKIF_CPRISND_RECARDCNTRES *rsp_p, USHORT sigsys_rslt)
{
    T_RRH_LAYER3 ltLayer3Sta;                   /* Layer3状態 */
    UINT cpri_no;                               /* CPRIリンク番号 */

// [16B] add start
	T_RRH_SLOT_S3G_RE	buf_LTEslotInfo_re	= {};           /* 共有メモリ取得用(配下REの報告内容 LTE) */
	T_RRH_SLOT_3G_RE	buf_3GslotInfo_re	= {};           /* 共有メモリ取得用(配下REの報告内容 3G) */
	// USHORT *slot_no;   /* スロット番号 */
	// USHORT slot_no_max; /* スロット情報N */
	USHORT slot_no_wk;
	USHORT i;
// [16B] add start

    BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"RE CARD CONTROL REQ (RESET)");

// [16B] add start
	slot_no_wk = parm_p->cpridat_recardcntreq.slot_no;
// [16B] add start

    if (CMD_SYS_3G == sigsys_rslt) {
        parm_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF) + sizeof(CMT_CPRIF_RECARDCNTREQ);
    } else {
        parm_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF) + sizeof(CMT_CPRIF_RECARDCNTREQ_S3G);
    }

    /*
     * 共有メモリのLayer3状態取得し、Layer3状態が「RE起動中状態」または
     * 「RE運用中状態」の配下REにREカード制御要求を中継する
     */
    (VOID)f_cmn_com_layer3_get(sigsys_rslt, &ltLayer3Sta);
    for (cpri_no = 0; cpri_no < D_RRH_CPRINO_RE_MAX; cpri_no++) {
        if ((E_RRH_LAYER3_RE_START == ltLayer3Sta.layer3_re[cpri_no]) ||
            (E_RRH_LAYER3_RE_OPE   == ltLayer3Sta.layer3_re[cpri_no]) ){
            parm_p->cprircv_inf.link_num = cpri_no + D_RRH_CPRINO_RE_MIN;
//            l3_com_sendMsg(CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0, parm_p, parm_p->head.uiLength);

// [16B] add start
            // 該当RE(CPRI)のスロット情報報告を共有メモリから取得
            // 3Gの場合
		    // if (CMD_SYS_3G == sigsys_rslt) {
					// f_cmn_com_slot_3g_get(cpri_no+1,&buf_3GslotInfo_re);
					// slot_no_max = buf_3GslotInfo_re.slot_num;
					// slot_no = (USHORT *)buf_3GslotInfo_re.slot_no;
			// }
            // LTEの場合
			// else {
					// f_cmn_com_slot_s3g_get(cpri_no+1,&buf_LTEslotInfo_re);
					// slot_no_max = buf_LTEslotInfo_re.slot_num;
					// slot_no = (USHORT *)buf_LTEslotInfo_re.slot_no;
			// }
		    // for(i=0; i < slot_no_max; i++) {
                // 先頭のスロット番号から、ポインタを更新しながら次のスロット番号を取得する
                // このやり方が可能なのは、T_RRH_SLOT_S3G_RE,T_RRH_SLOT_3G_REの構造体の定義を
                // REから受信する型(スロット番号とREカード機能部情報)と異なるように定義しているためである。
				// parm_p->cpridat_recardcntreq.slot_no = *slot_no;
				// slot_no++;
	            l3_com_sendMsg(CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0, parm_p, parm_p->head.uiLength);
			// }
// [16B] add end

        }
    }

    /* CPRI信号応答結果に正常(0)を設定		*/
    rsp_p->cpridat_recardcntres.result = CMD_NML;

    /* CPRI信号応答スロット番号設定			*/
    /* (インパラメータのスロット番号を設定)	*/
#if 0 // [16B]
    rsp_p->cpridat_recardcntres.slot_no = parm_p->cpridat_recardcntreq.slot_no;
#else  // [16B]
    rsp_p->cpridat_recardcntres.slot_no = slot_no_wk;
#endif // [16B]

    /* タスク間ヘッダ作成 */
    rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF)
                                    +	sizeof(CMT_CPRIF_RECARDCNTRES);

    /* MDIFリセット時は先に応答を返却する	*/
    (VOID) m_cm_L3MsgSend_REC(rsp_p);

/* Reset intermediate device with slot number 51-56 */
if (slot_no_wk > 50 && slot_no_wk < 57)
{
    #if 0 // [16B]
        /* Wait 200ms */
        cm_MWait(2*10);
    #else  // [16B]
        /* Wait 300ms */
        cm_MWait(3*10);
    #endif // [16B]

        /* 自カードリセット実施					*/
        /* MB  be RCD_RMT_RST_COM, PRC change to RCD_RMT_RST_NOCOM*/
        m_rc_cm_TraRstExec(RCD_RMT_RST_NOCOM, D_RRH_RSTFACT_CRDCNT);
}
    /* 自カードリセット(1)を返却			*/
    return RCD_MDIF_RST;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief     RE card control check cnt kind
 *  @note      RE card control check cnt kind
 *  @param     parm_p [in] the buffer address pointer of received message
 *  @param     sigsys_rslt [in] 3G/LTE
 *  @return    RETURN CODE
 *  @retval    CMD_OK					0:  Normal end
 *  @retval    CMD_OTHR_ERR_RE          42: その他エラー(RE)
 *  @retval    CMD_HDL_NG				51: 処理NG（RE)
 *  @date   2015/04/27 FFCS)Zhangxq  create for TDD
 *  @date   2015/07/29 TDI)satou 「リセット」のみ受け付けるように
*/
/********************************************************************************************************************/
USHORT m_rc_ReCardCntChk_CntKind(CMT_TSKIF_CPRIRCV_RECARDCNTREQ *parm_p, USHORT sigsys_rslt)
{
	USHORT retcode = CMD_OK;

	/* FHMでは「リセット」のみ受け付ける */
	if (CMD_CARDRST != parm_p->cpridat_recardcntreq.cnt_kind) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,
                           "Parm cnt kind chk NG %d %d",
                           parm_p->cpridat_recardcntreq.cnt_kind,
                           parm_p->cpridat_recardcntreq.slot_no);

        if (CMD_SYS_S3G == sigsys_rslt) {
            retcode = CMD_HDL_NG;
        } else {
            retcode = CMD_OTHR_ERR_RE;
        }
	}

	return retcode;
}
/* @} */

/* @} */


