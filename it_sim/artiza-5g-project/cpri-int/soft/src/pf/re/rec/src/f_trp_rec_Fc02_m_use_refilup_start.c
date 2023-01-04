/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_use_refilup_start.c
 * @brief  運用中REファイル更新開始通知処理
 * @date   2015/08/21 FPT)Yen create.
 * @date   2015/09/30 TDIPS)sasaki update
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  運用中REファイル更新開始通知
 * @note   運用中REファイル更新開始通知処理を行う
 *         - 対RE状態#nがRE運用中以外
 *           - 運用中REファイル更新開始応答(NG,更新対象CPRI#)
 *         - else
 *           - 運用中REファイル更新開始応答(OK,更新対象CPRI#)
 *           - 対RE状態#n(※1)をRE運用中(REファイル更新中)へ遷移
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/08/21 FPT)Yen create.
 * @date   2015/09/30 TDIPS)sasaki update 自状態遷移処理を削除
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_use_refilup_start_req(                      /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    CHAR  			errdata[CMD_NUM48];
    ET_REC_C02_STN 	stn;                                         /* 状態番号 */
	USHORT			lwCpriNo;
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

	lwCpriNo = ((CMT_TSKIF_UREFILNEWSTAREQ *)bufp)->cpri_no;
	/****************/
    /* 状態番号取得 */
    /****************/
    stn = f_trp_rec_Fcom_t_c02stn_get( lwCpriNo );                   /* 状態番号:R */

	/* 対RE状態#nがRE運用中以外 */
	if(stn < EC_REC_C02_STN_RES_L3ECMP || stn > EC_REC_C02_STN_USE)
	{
		snprintf(errdata, CMD_NUM48, "[FRCRST][cpr:%02d]f_trp_rec_Fc02_m_use_refilup_start Re status != using", D_REC_C02_CPR_NO()); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(CMT_TSKIF_UREFILNEWFINRES));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);

		/* 運用中REファイル更新開始応答(NG,更新対象CPRI#) */
		f_trp_rec_Fc02_m_use_refilup_start_res(CMD_RLTCODE_NG, lwCpriNo );

		return;
	}
	else
	{
		/* 運用中REファイル更新開始応答(OK,更新対象CPRI#) */
		f_trp_rec_Fc02_m_use_refilup_start_res(CMD_RLTCODE_OK, lwCpriNo );
	}

//    /******************/
//    /* 自状態番号設定 */
//    /******************/
//    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
//        EC_REC_C02_STN_RES_REFILING         /* RE起動中(REファイル更新中):I */
//    );

    return;
}


/****************************************************************************/
/*!
 * @brief  運用中REファイル更新開始応答
 * @note   運用中REファイル更新開始応答処理を行う
 * @param  rst [in]  応答結果
 * @param  cpriNo [in]  更新対象CPRI number
 * @return None
 * @date   2015/08/21 FPT)Yen create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_use_refilup_start_res(                      /* なし:R */
    USHORT rst,                                               /* 応答結果:I */
    USHORT cpriNo                                        /* 更新対象CPRI#:I */
) {
    CMT_TSKIF_UREFILNEWSTARES  buf;
    CMT_TSKIF_UREFILNEWSTARES* bufp = &buf;             /* バッファポインタ */
    UINT   msgid  = CMD_TSKIF_USEREFILE_UPDATE_START_RSP;   /* メッセージID */
    INT    sit_ret;                                           /* 終了コード */
    CHAR  errdata[CMD_NUM48];
	USHORT	lwLength;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /************************/
    /* バッファ設定(データ) */
    /************************/
    bufp->cpri_no = cpriNo;                                     /* CPRI番号 */
    bufp->result_code = rst;                                    /* 応答結果 */

    /******************/
    /* スレッド間送信 */
    /******************/
	lwLength = sizeof(CMT_TSKIF_UREFILNEWSTARES);

	/* 運用中REファイル更新開始応答を送信 */
	sit_ret = f_trp_rec_Fcom_t_msg_snd( bufp, msgid, D_RRH_PROCQUE_L3, 0, lwLength);

    /* NGの場合 */
    if (sit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][ret:0x%02x]IT send NG.", cpriNo, sit_ret);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);
    }

    return;
}
/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
