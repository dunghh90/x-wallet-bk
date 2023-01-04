/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_tmk_val_tbl_gen.c
 * @brief  タイマ種別→タイマ値変換テーブル生成
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2007/05/25 FJT)Nagasima CR-00012-001
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2009
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  タイマ種別→タイマ値変換テーブル生成
 * @note   タイマ種別→タイマ値変換テーブルを生成する
 * @param  -
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2007/05/25 FJT)Nagasima CR-00012-001
 * @date   2009/03/04 FFCS)Shihzh CR-00031-005.
 * @date   2010/11/12 FJT)Tokunaga M-S3G-eNBSYS-xxxxx [TS-810項]
 * @date   2015/09/04 FPT)Quynh update for FHM
 * @date   2015/10/06 FPT)Dung update (add timer value for Trx)
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_tmk_val_tbl_gen(                            /* なし:R */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /****************/
    /* タイマ値設定 */
    /****************/

    /* Toffset値報告確認タイマ                  */
    f_trp_rec_Wcom_tmk_val_tbl.val[EC_REC_TMK_TOFVAL_CNF  ].tim_val
     = D_TCM_SYSDFT_TOFVAL_CNFTIM * CMD_NUM10; 
     /* RE装置構成情報報告確認タイマ             */
    f_trp_rec_Wcom_tmk_val_tbl.val[EC_REC_TMK_REEQPINF_CNF].tim_val
     = D_TCM_SYSDFT_REEQPINF_CNFTIM * CMD_NUM10;
    /* REスロット情報報告確認タイマ             */
    f_trp_rec_Wcom_tmk_val_tbl.val[EC_REC_TMK_RESLTINF_CNF].tim_val
     = D_TCM_SYSDFT_RESLTINF_CNFTIM * CMD_NUM10;
    /* RE状態報告送信間隔タイマ                 */
    f_trp_rec_Wcom_tmk_val_tbl.val[EC_REC_TMK_RESTS_INT   ].tim_val
     = D_TCM_SYSDFT_RESTSREQ_ITVTIM * CMD_NUM10;
    /* RE状態報告確認タイマ                     */
    f_trp_rec_Wcom_tmk_val_tbl.val[EC_REC_TMK_RESTS_CNF   ].tim_val
     = D_TCM_SYSDFT_RESTSREQ_CNFTIM * CMD_NUM10;
    /* RE PORT状態報告確認タイマ                */
    f_trp_rec_Wcom_tmk_val_tbl.val[EC_REC_TMK_REPRTSTS_CNF].tim_val
     = D_TCM_SYSDFT_REPORTSTS_CNFTIM * CMD_NUM10;
    /* REカード状態報告(セクタ部)送信間隔タイマ */
    f_trp_rec_Wcom_tmk_val_tbl.val[EC_REC_TMK_RECRDSSC_INT].tim_val
     = D_TCM_SYSDFT_RECRDSTS_ITVTIM * CMD_NUM10; 
    /* REカード状態報告(セクタ部)確認タイマ     */
    f_trp_rec_Wcom_tmk_val_tbl.val[EC_REC_TMK_RECRDSSC_CNF].tim_val
     = D_TCM_SYSDFT_RECRDSTS_CNFTIM * CMD_NUM10; 
    /* REキャリア状態報告送信間隔タイマ         */
    f_trp_rec_Wcom_tmk_val_tbl.val[EC_REC_TMK_RECRRSTS_INT].tim_val
     = D_TCM_SYSDFT_RECARSTS_ITVTIM * CMD_NUM10;
    /* REキャリア状態報告確認タイマ             */
    f_trp_rec_Wcom_tmk_val_tbl.val[EC_REC_TMK_RECRRSTS_CNF].tim_val
     = D_TCM_SYSDFT_RECARSTS_CNFTIM * CMD_NUM10;
    /* REリセット確認タイマ                     */
    f_trp_rec_Wcom_tmk_val_tbl.val[EC_REC_TMK_RERST_CNF].tim_val
     = D_TCM_SYSDFT_RERST_CNFTIM * CMD_NUM10;
    /* RE強制リセット確認タイマ                 */
    f_trp_rec_Wcom_tmk_val_tbl.val[EC_REC_TMK_REFRCRST_CNF].tim_val
     = D_TCM_SYSDFT_REFRCRST_CNFTIM * CMD_NUM10;
    /* 二次リセット待ちタイマ                   */
    f_trp_rec_Wcom_tmk_val_tbl.val[EC_REC_TMK_2NDRST_WAT  ].tim_val
     = D_REC_2NDRST_STATC * CMD_NUM1000;
    /* MTアドレス設定確認タイマ（FLD-MT/ SV-MT/eNB-MT） */
    f_trp_rec_Wcom_tmk_val_tbl.val[EC_REC_TMK_MT_ADD_SET  ].tim_val
     = D_TCM_SYSDFT_FLDMTADR_CNFTIM * CMD_NUM10;

	f_trp_rec_Wcom_tmk_val_tbl.val[EC_REC_TMK_TXS_CNF  ].tim_val
     = D_TCM_SYSDFT_TRXSET_CNFTIM * CMD_NUM10;
	f_trp_rec_Wcom_tmk_val_tbl.val[EC_REC_TMK_TXR_CNF  ].tim_val
     = D_TCM_SYSDFT_TRXREL_CNFTIM * CMD_NUM10;

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
