/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_recrdssc_i_ini.c
 * @brief  RE状態(セクタ部)管理テーブル(REカード状態)初期化
 * @date   2007/03/20 FJT)Nagasima create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007-2016
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */
/****************************************************************************/
/*!
 * @brief  RE状態(セクタ部)管理テーブル(REカード状態)初期化
 * @note   RE状態(セクタ部)管理テーブル(REカード状態)を初期化する
 *         - 
 *           - 報告確認T.O.以外の場合
 *             - 処理終了
 *         - 初期化
 * @param  ink    [in]  初期化種別
 * @param  slt_no [in]  スロット番号
 * @return 変化
 * @retval 0:変化なし
 * @retval 1:変化あり
 * @Bug_No M-RRU-ZSYS-01981
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/21 FFCS)Shihzh modify for S3G PRC.
 * @date   2009/09/18 FFCS)Shihzh M-S3G-eNBPF-02549.
 * @date   2010/04/07 FJT)Tokunaga CR-XXX-XXX カード状態管理テーブル
 * @date   2010/07/21 CMS)Yorozuya CR-XXX-XXX コメント票F-1061対応
 * @date   2011/01/05 FJT)Tokunaga M-S3G-eNBPF-03971
 * @date   2011/09/30 FJT)Tokunaga CeNB-F-075-040(DCM) REの機能部故障状態についての確認事項
 * @date   2015/11/17 FPT)Yen M-RRU-ZSYS-01981 fix bug IT3 No84 大きいサイズの変数をstaticに
 * @date   2016/10/04 KCN)hfuku 16B機能追加
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_recrdssc_i_ini(                             /* 変化:R */
    USHORT ink,                                             /* 初期化種別:I */
    USHORT slt_no                                         /* スロット番号:I */
) {
    static T_RRH_CARDST_S3G lwCardStaClr;
    USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;
    USHORT slt_index;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* 報告確認T.O.以外の場合 */
    if ((ink != D_REC_INK_RTO) &&
        (ink != D_REC_INK_RNG) ) {
        /* 初期化必要なし */
        /* 終了 */
        return CMD_OFF;
    }
//TBD 5-2-7(配下REのALM検出による自律リセット) Q&A    
//    /* カード状態オブジェクト取得 */
//    cardObj = f_trp_rec_FCard_Obj_GetByNoWithCpri(D_REC_C02_CPR_NO(),slt_no);
//    
//    /*****************************************************************/
//    /* 自律リセット保護用タイマ / 自律リセット制御中フラグクリア処理 */
//    /*****************************************************************/
//    f_trp_rec_Fc02_t_autorst_ccl(cardObj);
    
    /**************/
    /* タイマ取消 */
    /**************/
    f_trp_rec_Fc02_t_timer_ccl(                                   /* なし:R */
        EC_REC_TMK_REAUTRST_WAT,          /* RE auto Reset 送信待ちタイマ:I */
        slt_no                                            /* スロット番号:I */
    );

	/* Slot番号からスロットIndex値(0-15)を算出する 16B KCN add */
	f_cmn_com_slotsrch_s3g(D_REC_C02_CPR_NO(), slt_no, &slt_index);

	/* REカード自律リセット待ちタイマOFF 16B KCN add */
	f_trp_com_AutoResetMngTbl[cpr_no_idx][slt_index].ReAutoRstTimFlg = CMD_FLG_OFF;
	
	cm_MemClr(&lwCardStaClr, sizeof(T_RRH_CARST_S3G)); /* pgr0584 */

	/* set share memmory 16B KCN chg */
	f_cmn_com_cardstslt_s3g_set(D_REC_C02_CPR_NO(), slt_index, &lwCardStaClr);

    return D_REC_RFBSTS_CHG;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
