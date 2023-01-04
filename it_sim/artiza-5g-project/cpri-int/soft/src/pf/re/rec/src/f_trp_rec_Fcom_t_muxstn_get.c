/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_muxstn_get.c
 * @brief  カード実装の状態を読み出し
 * @date   2008/10/16 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2011
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  カード実装新状態を読み出し
 * @note   カード実装新状態をを読み出する
 *         - レジスタ読み出し
 * @param  cpr_no [in] CPR番号
 * @return CPRIMUX カード実装新状態
 * @retval CMD_ON :カード実装
 * @retval CMD_OFF:カード実装なし
 * @date   2008/10/16 FFCS)Shihzh create.
 * @date   2010/11/17 FJT)Tokunaga	M-S3G-eNBSYS-02100;☆ST_ENB☆TS841項：CPRI-MUX含まない再開を実施した際に3G側にてBRE ALM検出
 * @date   2011/11/11 FJT)K.Koshida	D-MICRO-008:3002形対応(H/S inf変更対応)
 */
/****************************************************************************/
USHORT f_trp_rec_Fcom_t_muxstn_get(                               /* なし:R */
    USHORT cpr_no                                              /* CPR番号:I */
) {
#if !defined(OPT_CPRI_L2HARD)
    UINT  uit_ret;                                            /* 終了コード */
    UINT  red_val;                                    /* 読み出しレジスタ値 */
    UINT  mux_msk;
    CHAR errdata[CMD_NUM48];
#endif

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

#if !defined(OPT_CPRI_L2HARD)
    /********************/
    /* レジスタ読み出し */
    /********************/
    uit_ret = f_trp_rec_Fcom_t_nrg_red(                     /* 終了コード:R */
        EC_REC_NRK_CRD,                                   /* レジスタ種別:I */
        &red_val                                            /* レジスタ値:O */
    );
    /* NGの場合 */
    if (uit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][ret:0x%02x]Read card status register NG.", uit_ret);
        D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
    }

    if (cpr_no <= (CMD_CPRINO_MAX/2)) {
        mux_msk = D_REC_RGV_MUXINSTAL_MSK1;
    }
    else {
        mux_msk = D_REC_RGV_MUXINSTAL_MSK2;
    }

    if( D_REC_GETBIT(red_val, mux_msk) == CMD_ON) {
       return CMD_ON;
    }

	/* CPRI-IF実装された場合は、CPRI-MUX含むリセット情報(Inband Reset要否)にて復帰 */
//    return(cmw_cprilnkstatbl.cmuxrstinfo);
    return(CMD_ON); /*TBD*/
#else
	/*	3002形では、CPRI-MUXが未実装であることはない	*/
    return(CMD_ON);
#endif

}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
