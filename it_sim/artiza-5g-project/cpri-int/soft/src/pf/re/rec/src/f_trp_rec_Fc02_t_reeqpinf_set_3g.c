/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_reeqpinf_set.c
 * @brief  RE状態(セクタ部)管理テーブル(RE装置構成情報)設定
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
 * @brief  RE状態(セクタ部)管理テーブル(RE装置構成情報)設定
 * @note   RE状態(セクタ部)管理テーブル(RE装置構成情報)を設定する
 *         - 受信値設定
 * @param  l3_bufp [in]  L3バッファポインタ
 * @return 変化
 * @retval 0:変化なし
 * @retval 1:変化あり
 * @date   2008/07/21 FFCS)Shihzh create.
 * @date   2011/06/09 FJT)Tokunaga M-S3G-eNBPF-04052対応
 * @date   2011/09/17 FJT)Tokunaga CeNB-F-078-010(F)(Super3G無線基地局装置CPRI技術説明資料(案)Ver.2.0.5について)対応
 * @TBD_No 修正未完了
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_reeqpinf_set_3g(                               /* 変化:R */
    VOID* l3_bufp                                   /* L3バッファポインタ:I */
) {
    USHORT cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    CPRIEQPSTR_INF       *rcvp;               /* 受信RE装置構成情報アドレス */
    T_REC_REEQPINF       *tblp;           /* テーブルRE装置構成情報アドレス */
    T_REC_REEQPINF       *tbkp;       /* テーブル(BK)RE装置構成情報アドレス */
    INT                  cmp_ret;                               /* 比較結果 */
//    UINT                 ui_ret;
//    CHAR                 errdata[CMD_NUM48];

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* 設定RE装置構成情報アドレス抽出 */
    rcvp = &(((CMT_CPRIF_EQPSTRRES*)l3_bufp)->cpristr_inf);
    /* テーブルRE装置構成情報アドレス抽出 */
    tblp = &(f_trp_rec_Wcom_remsc_tbl_3g.remsc[cpr_no_idx].reeqpinf);
    /* テーブルRE装置構成情報アドレス抽出 */
    tbkp = &(f_trp_rec_Wcom_remsc_tbk_3g.remsc[cpr_no_idx].reeqpinf);
    
    /* テーブル→設定RE装置構成情報アドレス */
    f_trp_com_Fmemcpy(                                      /* 終了コード:R */
        tbkp,                               /* 設定RE装置構成情報アドレス:I */
        tblp,                           /* テーブルRE装置構成情報アドレス:I */
        sizeof(*tblp)                             /* RE装置構成情報サイズ:I */
    );
    /* テーブル→設定RE装置構成情報アドレス */
    f_trp_com_Fmemcpy(                                      /* 終了コード:R */
        tblp,                               /* 設定RE装置構成情報アドレス:I */
        rcvp,                           /* テーブルRE装置構成情報アドレス:I */
        sizeof(*tblp)                             /* RE装置構成情報サイズ:I */
    );
	/* save RE device composition information to shared memory*/
	f_cmn_com_eqp_3g_set(                                   /* 終了コード:R */
        D_REC_C02_3G_CPR_NO(),                    /* CPRI番号インデックス:I */
        (T_RRH_EQP_3G *)l3_bufp             /* 設定RE装置構成情報アドレス:I */
    );
    /************/
    /* 新旧比較 */
    /************/
    BPF_RU_MACC_BCMP(                                       /* 終了コード:R */
        tbkp,                               /* (BK)RE装置構成情報アドレス:I */
        tblp,                                   /* RE装置構成情報アドレス:I */
        sizeof(*tbkp),                       /* (BK)TRE装置構成情報サイズ:I */
        &cmp_ret                                              /* 比較結果:O */
    );

    /* 変化なしの場合 */
    if (cmp_ret == CMD_NUM0) {
        return CMD_OFF;
    	
#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
    	
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
