/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_elm_key_cnv_3g.c
 * @brief  タイマ要素→キー情報変換
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
 * @brief  タイマ要素→キー情報変換
 * @note   タイマ要素→キー情報に変換する
 * @param  elmp     [in]  タイマ要素
 * @param  key_infp [out] タイマキー情報
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_elm_key_cnv(                                /* なし:R */
    T_REC_TIMELM* elmp,                                     /* タイマ要素:I */
    bpf_ru_hrtm_key_inf_t* key_infp                        /* タイマキー情報:O */
) {
    UCHAR cls;                                                    /* クラス */
    UCHAR tmk;                                                /* タイマ種別 */
    UCHAR cpr_no;                                               /* CPRI番号 */
    UCHAR soc_no;                             /* スロット番号orキャリア番号 */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /******************/
    /* キー情報クリア */
    /******************/
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        key_infp,                                     /* キー情報アドレス:I */
        sizeof(*key_infp)                               /* キー情報サイズ:I */
    );

    /************/
    /* 要素抽出 */
    /************/
    cls    = (UCHAR)(elmp->cls   );
    tmk    = (UCHAR)(elmp->tmk   );
    cpr_no = (UCHAR)(elmp->cpr_no);
    soc_no = (UCHAR)(elmp->soc_no);

    /****************/
    /* キー情報設定 */
    /****************/
     D_REC_SETBYTE(key_infp->keyinf1, CMD_NUM3, cls   );
     D_REC_SETBYTE(key_infp->keyinf1, CMD_NUM2, tmk   );
     D_REC_SETBYTE(key_infp->keyinf1, CMD_NUM1, cpr_no);
     D_REC_SETBYTE(key_infp->keyinf1, CMD_NUM0, soc_no);

     return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
