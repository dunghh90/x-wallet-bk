/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_r.c
 * @brief  ルート
 * @date   2007/03/20 FJT)Nagasima create.
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  ルート
 * @note   イベント受信時の処理を行う
 *         - イベント変換
 *         - 状態番号取得
 *         - マトリクス関数起動
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @TBD_No 修正未完了
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_r(                                            /* なし:R */
   VOID* bufp                                         /* バッファポインタ:I */
) {
    ET_REC_C02_EVT evt;                                         /* イベント */
    ET_REC_C02_STN stn;                                         /* 状態番号 */
    USHORT         evt_cpr_no;                          /* イベントCPRI番号 */
    USHORT         cpr_no;                                      /* CPRI番号 */
    USHORT         cpr_no_min;                               /* CPRI番号MIN */
    USHORT         cpr_no_max;                               /* CPRI番号MAX */
    CHAR          errdata[CMD_NUM48];

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /****************/
    /* イベント変換 */
    /****************/
    evt = f_trp_rec_Fc02_r_evt_cnv(                           /* イベント:R */
        bufp,                                         /* バッファポインタ:I */
        &evt_cpr_no                                           /* CPRI番号:O */
    );

    /* イベントなしの場合 */
    if (evt == EC_REC_C02_EVT_NON) {
        /* 終了 */
        return;
    }
#ifdef FHM_DEBUG_TIMER_FOR_IT1
    printf( "%s %s %d <cpri>%d <evt>%d\n", __FILE__, __FUNCTION__, __LINE__, evt_cpr_no, evt );
#endif


    /***********************/
    /* CPRI番号MIN/MAX設定 */
    /***********************/
    /* ALLの場合 */
    if (evt_cpr_no == D_REC_CPRINO_ALL) {
        cpr_no_min = CMD_CPRINO_MIN;
        cpr_no_max = CMD_CPRINO_MAX;
    }
    /* その他の場合 */
    else {
        cpr_no_min = evt_cpr_no;
        cpr_no_max = evt_cpr_no;
    }

    if ((cpr_no_min < CMD_CPRINO_MIN) ||
        (cpr_no_max > CMD_CPRINO_MAX) ){
        snprintf(errdata, CMD_NUM48, "[ERR][min:%02d][max:%02d]Link No Err.", cpr_no_min, cpr_no_max);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(T_MSGHEAD));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);
        return;
    }

    /* 必要CPRI番号数分ループ */
    for (cpr_no = cpr_no_min; cpr_no <= cpr_no_max; cpr_no++) {
        /* パラメータテーブル設定 */
        f_trp_rec_Wc02_prm_tbl.cpr_no = cpr_no;

        /****************/
        /* イベント設定 */
        /****************/
        f_trp_rec_Fc02_t_evt_set(                                /* なし:R */
            evt                                               /*イベント:I */
        );

        /****************/
        /* 状態番号取得 */
        /****************/
        stn = f_trp_rec_Fc02_t_ownstn_get(                    /* 状態番号:R */
        );

        /**********************/
        /* マトリクス関数起動 */
        /**********************/
        f_trp_rec_Rc02_m_fnc[evt][stn](bufp);

#ifdef DEBUG_TRACE_T
		TRACE(" REC02   :Main[END][msg=0x%08X][evt=0x%08X][thr=%s(%d)][lnk=%d] ####",
						 ((T_MSGHEAD *)bufp)->msgid, evt,
						 f_trp_com_Rlogfnc_tbl.inf[(((T_MSGHEAD *)bufp)->srcFbNo&0xFFF)].thr_name,
						 ((T_MSGHEAD *)bufp)->srcFbNo, cpr_no);
#endif
    }

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
