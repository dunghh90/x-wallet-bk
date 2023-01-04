/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_ushortsort_asc.c
 * @brief  昇順ソート関数(unsigned short用)
 * @date   2008/07/21 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  昇順ソート関数(unsigned short用)
 * @note   運用中動作パターンに応じた処理を行う
 * @param  elm_num [in] ソート対象要素数
 * @param  elm_siz [in] ソート対象要素数
 * @param  elm     [in] ソート対象要素配列アドレス
 * @return None
 * @date   2008/07/21 FFCS)Shihzh create.
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_ushortsort_asc(                             /* なし:R */
    USHORT elm_num,                                   /*  ソート対象要素数  */
    USHORT elm_siz,                                   /*  ソート対象要素数  */
    USHORT *elm                             /*  ソート対象要素配列アドレス  */
) {
    USHORT* elm_wk;                                 /* 要素入れ替え用ワーク */
    USHORT  i;                                            /* ループカウンタ */
    USHORT  j;                                            /* ループカウンタ */
    INT     sit_ret;                                          /* 終了コード */
    CHAR    errdata[CMD_NUM48];
    
    if ((elm_num == CMD_NUM0) || (elm_siz == CMD_NUM0)) {
        return;
    }

    /****************/
    /* バッファ取得 */
    /****************/
    sit_ret = f_trp_com_Falloc_buf(                         /* 終了コード:R */
        elm_siz * sizeof(USHORT),                       /* バッファサイズ:I */
        (VOID*)&elm_wk                                /* バッファアドレス:O */
    );
    /* NGの場合 */
    if (sit_ret != CMD_OK) {
#ifndef D_OPT_TRIF_BUFNGLOG_INVALID
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][siz:0x%08x][ret:0x%02x]Alloc buffer NG.", D_REC_C02_CPR_NO(), elm_siz, sit_ret); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
#endif /* D_OPT_TRIF_BUFNGLOG_INVALID */
        return;
    }

    for(i = CMD_NUM0; i < (USHORT)(elm_num-CMD_NUM1); i++) {
        for(j = elm_num-CMD_NUM1; j > i; j--) {
            if(elm[j*elm_siz] < elm[(j-CMD_NUM1)*elm_siz]){
                /* elm_wk   = elm[j];   */
                f_trp_com_Fmemcpy(
                    elm_wk, 
                    &elm[j*elm_siz],
                    elm_siz * sizeof(USHORT)
                );
                /* elm[j]   = elm[j-1]; */
                f_trp_com_Fmemcpy(
                    &elm[j*elm_siz],
                    &elm[(j-CMD_NUM1)*elm_siz],
                    elm_siz * sizeof(USHORT)
                );
                /* elm[j-1] = elm_wk;   */
                f_trp_com_Fmemcpy(
                    &elm[(j-CMD_NUM1)*elm_siz],
                    elm_wk,
                    elm_siz * sizeof(USHORT)
                );
            }
        }
    }
    
    /****************/
    /* バッファ解放 */
    /****************/
    f_trp_com_Ffree_buf(                                          /* なし:R */
        elm_wk                                        /* バッファアドレス:I */
    );
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
