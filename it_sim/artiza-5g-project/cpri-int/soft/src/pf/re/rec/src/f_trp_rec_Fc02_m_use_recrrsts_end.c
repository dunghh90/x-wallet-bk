/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_use_recrrsts_end.c
 * @brief  運用中REキャリア状態報告終了処理
 * @date   2008/07/21 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2010
 */
/****************************************************************************/
#include "f_trp_rec.h"
#include "f_rrh_reg_dis.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  運用中REキャリア状態報告終了処理
 * @note   運用中REキャリア状態報告終了処理を行う
 *         - RE状態(セクタ部)管理テーブル(REキャリア状態情報)設定
 *           - 変化ありの場合
 *             - プロセス間(REキャリア状態変化通知)送信
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2008/07/21 FFCS)Shihzh create. 
 * @date   2010/08/11 CMS)Takahashi modify for TS-656
 * @date   2015/10/08 FPT)Tan Update comment
 * @date   2015/10/28 FPT)Yen fix bug IT3
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_use_recrrsts_end(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    VOID*  datp;
    VOID*  l3_bufp;                                   /* L3バッファポインタ */
    USHORT  ret_code;                                         /* 結果コード */
    UINT    uit_ret;                                          /* 終了コード */
    CHAR    errdata[CMD_NUM48];
    T_RRH_CARST_S3G         shmCarSt = {0};
	
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* L3バッファポインタ抽出 */
    datp    = &((T_REC_MSGRCV_TBL*)bufp)->datp;
    l3_bufp = &(((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind);
    /* 結果コード抽出 */
    ret_code = ((CMT_CPRIF_CARLSTSRES*)l3_bufp)->result;

	/*	キャリア状態報告応答受信ログ格納	*/
//	f_trp_rec_Fcom_t_l3_LogStore( D_REC_LOGKIND_CARRIER, D_REC_C02_CPR_NO(), D_REC_MSGKIND_RSP );

    /**************/
    /* タイマ取消 */
    /**************/
    uit_ret = f_trp_rec_Fc02_t_timer_ccl(                         /* なし:R */
        EC_REC_TMK_RECRRSTS_CNF,              /* REキャリア状態確認タイマ:I */
        D_REC_SLT_IV                                      /* スロット番号:I */
    );
    
    if (uit_ret == D_REC_TIM_IV) {
        return;
    }
    
    /**********************/
    /* 残トライ回数初期化 */
    /**********************/
    f_trp_rec_Fc02_t_use_recrrstsreq_ini(                          /* なし:R */
    );

    if (ret_code == CMD_NML) { 
        ret_code |= f_trp_rec_Fc02_t_cprpar_chk(datp);
    }
    /* OK以外の場合 */ 
    if (ret_code != CMD_OK) {

        /***************************************************************/
        /* RE状態(共通部)管理テーブル(REキャリア状態報告)初期化        */
        /***************************************************************/
    	shmCarSt.signal_kind = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind;
		shmCarSt.result = ret_code;

    	/* set share memmory */
		f_cmn_com_carst_s3g_set( D_REC_C02_CPR_NO(), &shmCarSt);

        if (uit_ret == CMD_ON) {
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][ret:0x%04x]RE Carrier sts report NG.", D_REC_C02_CPR_NO(), ret_code); /* pgr0570 */
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(T_MSGHEAD));
            D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);
        }

    	return;
    }
	
   /***************************************************************/
   /* RE状態(共通部)管理テーブル(REキャリア状態報告)設定          */
   /***************************************************************/
   f_trp_rec_Fc02_t_recrrsts_set(                  /* 変化:R */
       l3_bufp                                 /* L3バッファポインタ:I */
   );

    /***************************************************************/
	/*送信系を１系に切り替える処理                                 */
    /***************************************************************/
    f_trp_rec_Fc02_m_use_sendtypeswitch();

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
