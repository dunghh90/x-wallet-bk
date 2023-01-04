/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Rc03_m_txs_fw.c
 * @brief  メッセージをl2/lpbに送ってください。
 * @date
 *
 *
 */
/****************************************************************************/
#include "f_trp_rec.h"

/****************************************************************************/
/*!
 * @brief  メッセージをl2/lpbに送ってください。(LTE)
 * @TBD_No 
 * @date   2015/10/29 FPT)Duongcd Fix bug IT3 No27
 *
 *
 */
/****************************************************************************/
VOID f_trp_rec_Rc03_m_txs_fw(VOID *bufp) {

	/* Receive msg from l3/rct, send msg to l2/lpb */
    f_trp_rec_Fcom_t_l2_msg_fw(bufp, (( CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->head.uiLength);

     /**************/
    /* タイマ削除 */
    /**************/
    f_trp_rec_Fc03_t_timer_ccl(                                 /* なし:R */
        EC_REC_TMK_TXR_CNF                         /* TRX解放確認タイマ:I */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc03_t_ownstn_set(                                  /* なし:R */
        EC_REC_C03_STN_IDLE                                     /* アイドル */
    );
}

/****************************************************************************/
/*!
 * @brief  メッセージをl2/lpbに送ってください。(3G)
 * @TBD_No 
 * @date
 *
 *
 */
/****************************************************************************/
VOID f_trp_rec_Rc03_m_txs_fw_3g(VOID *bufp) {

    /* Receive msg from l3/rct, send msg to l2/lpb */
    f_trp_rec_Fcom_t_l2_msg_fw(bufp, sizeof(CMT_TSKIF_CPRIRCV_TRXSETREQ));

     /**************/
    /* タイマ削除 */
    /**************/
    f_trp_rec_Fc03_t_timer_ccl_3g(                                  /* なし:R */
        EC_REC_TMK_TXR_CNF                             /* TRX解放確認タイマ:I */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc03_t_ownstn_set_3g(                                 /* なし:R */
        EC_REC_C03_STN_IDLE                                       /* アイドル */
    );
}
