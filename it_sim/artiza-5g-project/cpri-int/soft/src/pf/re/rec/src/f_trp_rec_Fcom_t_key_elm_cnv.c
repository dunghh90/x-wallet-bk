/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_key_elm_cnv.c
 * @brief  タイマキー情報→要素変換
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
 * @brief  タイマキー情報→要素変換
 * @note   タイマキー情報→要素に変換する
 * @param  key_inf1 [out] タイマキー情報
 * @param  elmp     [in]  タイマ要素
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_key_elm_cnv(                                /* なし:R */
    UINT key_inf1,
    T_REC_TIMELM* elmp                                      /* タイマ要素:O */
) {
    UCHAR cls;                                                    /* クラス */
    UCHAR tmk;                                                /* タイマ種別 */
    UCHAR cpr_no;                                               /* CPRI番号 */
    UCHAR soc_no;                   /* スロット番号orキャリア番号 */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**************/
    /* 要素クリア */
    /**************/
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        elmp,                                             /* 要素アドレス:I */
        sizeof(*elmp)                                       /* 要素サイズ:I */
    );

    /************/
    /* 要素抽出 */
    /************/
     cls    = D_REC_GETBYTE(key_inf1, CMD_NUM3);
     tmk    = D_REC_GETBYTE(key_inf1, CMD_NUM2);
     cpr_no = D_REC_GETBYTE(key_inf1, CMD_NUM1);
     soc_no = D_REC_GETBYTE(key_inf1, CMD_NUM0);

    /************/
    /* 要素設定 */
    /************/
    elmp->cls    = (USHORT)(cls   );
    elmp->tmk    = (USHORT)(tmk   );
    elmp->cpr_no = (USHORT)(cpr_no);
    elmp->soc_no = (USHORT)(soc_no);

     return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
