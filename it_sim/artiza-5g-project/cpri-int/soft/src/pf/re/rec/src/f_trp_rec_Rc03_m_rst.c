/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Rc03_m_rst.c
 * @brief  初期状態へ戻す
 * @date
 *
 *
 */
/****************************************************************************/
#include "f_trp_rec.h"

/****************************************************************************/
/*!
 * @brief  初期状態へ戻す
 * @TBD_No 処理未作成
 * @date
 *
 *
 */
/****************************************************************************/
VOID f_trp_rec_Rc03_m_rst(VOID *bufp) {

    /* Receive msg from l3/rct, send msg to l2/lpb */
    f_trp_rec_Fcom_t_l2_msg_fw(bufp, sizeof(CMT_TSKIF_CPRIRCV_TRXRELREQ));

     /**************/
    /* タイマ削除 */
    /**************/
    f_trp_rec_Fc03_t_timer_ccl(                                 /* なし:R */
        EC_REC_TMK_TXS_CNF                         /* TRX設定確認タイマ:I */
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
 * @brief  初期状態へ戻す
 * @TBD_No 処理未作成
 * @date
 *
 *
 */
/****************************************************************************/
VOID f_trp_rec_Rc03_m_rst_3g(VOID *bufp) {

    /* Receive msg from l3/rct, send msg to l2/lpb */
    f_trp_rec_Fcom_t_l2_msg_fw(bufp, sizeof(CMT_TSKIF_CPRIRCV_TRXRELREQ));

     /**************/
    /* タイマ削除 */
    /**************/
    f_trp_rec_Fc03_t_timer_ccl_3g(                                  /* なし:R */
        EC_REC_TMK_TXS_CNF                             /* TRX設定確認タイマ:I */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc03_t_ownstn_set_3g(                                  /* なし:R */
        EC_REC_C03_STN_IDLE                                        /* アイドル */
    );
}
