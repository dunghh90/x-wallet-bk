/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_rests_ini.c
 * @brief  RE状態(共通部)管理テーブル(RE状態)初期化
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
 * @brief  RE状態(共通部)管理テーブル(RE状態)初期化
 * @note   RE状態(共通部)管理テーブル(RE状態)を初期化する
 *         - 
 *           - 初期化,報告確認T.O.,Activeない以外の場合
 *             - 処理終了
 *         - 初期化
 * @param  ink    [in]  初期化種別
 * @return 変化
 * @retval 0:変化なし
 * @retval 1:変化あり
 * @date   2008/07/21 FFCS)Shihzh create.
 * @date   2010/08/24 CMS)Takahashi modify for M-S3G-eNBPF-03710
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_rests_ini_3g(                                  /* 変化:R */
    USHORT ink                                              /* 初期化種別:I */
) {
   USHORT re_serial_idx = CMD_NUM0;                   /* AMP/OFTRXインデックス */
                                                   
    T_REC_RESTS* tblp;                            /* テーブルRE状態アドレス */
    T_REC_RESTS* tbkp;                        /* テーブル(BK)RE状態アドレス */
    INT             cmp_ret;                                    /* 比較結果 */

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
    
    /* 初期化,報告確認T.O.,Activeない以外の場合 */
    if ( (ink != D_REC_INK_INI) && 
         (ink != D_REC_INK_RTO) &&
         (ink != D_REC_INK_RNG) && 
         (ink != D_REC_INK_NOA) ) {
        /* 初期化必要なし */
        /* 終了 */
         	
#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
        return CMD_OFF;
    }

	re_serial_idx = D_REC_C02_RE_SERIAL_IDX();
    /* テーブルRE状態アドレス抽出 */
    tblp = &(f_trp_rec_Wcom_remcm_tbl_3g.remcm[re_serial_idx].rests);
    /* テーブル(BK)RE状態アドレス抽出 */
    tbkp = &(f_trp_rec_Wcom_remcm_tbk_3g.remcm[re_serial_idx].rests);
    /* テーブル→テーブル(BK)にコピー */
    f_trp_com_Fmemcpy(                                      /* 終了コード:R */
        tbkp,                                       /* (BK)RE状態アドレス:I */
        tblp,                                           /* RE状態アドレス:I */
        sizeof(*tblp)                                 /* (BK)RE状態サイズ:I */
    );

    /**********/
    /* 初期化 */
    /**********/
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        tblp,                                           /* RE状態アドレス:I */
        sizeof(*tblp)                                     /* RE状態サイズ:I */
    );
	
    /************/
    /* 新旧比較 */
    /************/
    BPF_RU_MACC_BCMP(                                       /* 終了コード:R */
        tbkp,                                       /* (BK)RE状態アドレス:I */
        tblp,                                           /* RE状態アドレス:I */
        sizeof(*tblp)      ,                          /* (BK)RE状態サイズ:I */
        &cmp_ret                                              /* 比較結果:O */
    );

    /* 変化なしの場合 */
    if (cmp_ret == CMD_NUM0) {
    	
#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
        return CMD_OFF;
    }
    /* 変化ありの場合 */
    else {
    	
#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
        return CMD_ON;
    }
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
