/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_it_frcstpprc_snd.c
 * @brief  スレッド間(強制停止プロシージャ実行指示)送信
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
 * @brief  スレッド間(強制停止プロシージャ実行指示)送信
 * @note   スレッド間(強制停止プロシージャ実行指示)を送信する
 *         - バッファ取得
 *         - バッファ設定(データ)
 *         - スレッド間送信
 * @param  stopFactor 強制停止要因
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/11/20 FPT)Yen update (キャリア状態不一致：cpri_no = Fc03のCPRI番号)
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_it_frcstpprc_snd(T_RRH_RESTOP_FACTOR stopFactor
) {
    CMT_TSKIF_FRCSTPPRCIND  buf;
    CMT_TSKIF_FRCSTPPRCIND* bufp = &buf;                /* バッファポインタ */
    UINT   msglen = sizeof(CMT_TSKIF_FRCSTPPRCIND) - sizeof(T_MSGHEAD);
                                                            /* メッセージ長 */
    UINT   msgid  = CMD_TSKIF_FRCSTPPRCIND;                 /* メッセージID */
    USHORT thrino = D_TCM_THRINO_REC;                   /* スレッド内部番号 */
    INT    sit_ret;                                           /* 終了コード */
    CHAR  errdata[CMD_NUM48];

    /************************/
    /* バッファ設定(データ) */
    /************************/
	if( E_RRH_RESTOP_FACTOR_MISMATCH_CARR == stopFactor )
	{
	    bufp->cpri_no = D_REC_C03_CPR_NO();                      /* CPRI番号 */
	}
	else
	{
    	bufp->cpri_no = D_REC_C02_CPR_NO();                      /* CPRI番号 */
	}
    bufp->stopFactor = stopFactor;

    /******************/
    /* スレッド間送信 */
    /******************/
    sit_ret = f_trp_rec_Fcom_t_it_snd(                      /* 終了コード:R */
        bufp,                                         /* バッファポインタ:I */
        msgid,                                            /* メッセージID:I */
        thrino,                                       /* スレッド内部番号:I */
        msglen                                            /* メッセージ長:I */
    );
    /* NGの場合 */
    if (sit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][ret:0x%02x]IT send NG.", D_REC_C02_CPR_NO(), sit_ret);
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
