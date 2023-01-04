/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_tc_ini.c
 * @brief  残トライ回数初期化(RE強制リセット送信以外)
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
 * @brief  残トライ回数初期化(RE強制リセット送信以外)
 * @note   残トライ回数を初期化する(RE強制リセット送信以外)
 *         - 残トライ回数(RE強制リセット)初期化
 *         - 残トライ回数(REリセット)初期化
 *         - 残トライ回数(Toffset値報告要求)初期化
 *         - 残トライ回数(RE装置構成情報報告要求)初期化
 *         - 残トライ回数(REスロット情報報告要求)初期化
 *         - 残トライ回数(RE状態報告要求)初期化
 *         - 残トライ回数(RE PORT状態報告要求)初期化
 *         - 残トライ回数(REカード状態報告要求)初期化
 *         - 残トライ回数(REキャリア状態報告要求)初期化
 * @param  -
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/18 FFCS)Shihzh modify for S3G PRC.
 * @date   2009/01/28 FFCS)Shihzh M-S3G-eNBPF-01182.
 * @date   2011/02/17 FJT)Koshida create for M-S3G-eNBSYS-02253(TS-111)対応.
 * @date   2015/09/22 FPT)Yen update(add 残トライ回数(MTアドレス設定)
 * @date   2016/05/13 FJT)Koshida update 16A RE Port対応(運用中残トライ回数(RE PORT状態報告要求)初期化).
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_tc_ini(                                     /* なし:R */
) {
    USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    USHORT slt_no;                        /* スロット番号リストインデックス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**************************************/
    /* 残トライ回数(RE強制リセット)初期化 */
    /**************************************/
    f_trp_rec_Fc02_t_tc_refrcrst_ini(                             /* なし:R */
    );

    /**********************************/
    /* 残トライ回数(REリセット)初期化 */
    /**********************************/
    f_trp_rec_Fc02_t_tc_rerstreq_ini(                             /* なし:R */
    );

    /*****************************************/
    /* 残トライ回数(Toffset値報告要求)初期化 */
    /*****************************************/
    f_trp_rec_Fc02_t_tc_tofvalreq_ini(                            /* なし:R */
    );
    
    /**********************************************/
    /* 残トライ回数(RE装置構成情報報告要求)初期化 */
    /**********************************************/
    f_trp_rec_Fc02_t_tc_reeqpinfreq_ini(                          /* なし:R */
    );
    
    /**********************************************/
    /* 残トライ回数(REスロット情報報告要求)初期化 */
    /**********************************************/
    f_trp_rec_Fc02_t_tc_resltinfreq_ini(                          /* なし:R */
    );
    
    /*****************************************/
    /* 残トライ回数(RE状態報告要求)初期化    */
    /*****************************************/
    f_trp_rec_Fc02_t_tc_restsreq_ini(                             /* なし:R */
    );
    /*	運用中残トライ回数(RE状態報告要求)初期化	*/
    f_trp_rec_Fc02_t_use_restsreq_ini(                            /* なし:R */
    );
    
    /******************************************/
    /* 残トライ回数(RE PORT状態報告要求)初期化 */
    /******************************************/
    f_trp_rec_Fc02_t_tc_reprtstsreq_ini(                          /* なし:R */
    );
    /*	運用中残トライ回数(RE PORT状態報告要求)初期化	*/
    f_trp_rec_Fc02_t_use_reprtstsreq_ini(                            /* なし:R */
    );
    
    /* スロット番号数分ループ */
    for (slt_no = CMD_SLTNO_MIN; slt_no <= CMD_SLTNO_MAX; slt_no++) {
        /********************************************/
        /* 残トライ回数(REカード状態報告要求)初期化 */
        /********************************************/
        f_trp_rec_Fc02_t_tc_recrdstsreq_ini(                      /* なし:R */
            slt_no                                        /* スロット番号:I */
        );
        f_trp_rec_Fc02_t_use_recrdstsreq_ini(                      /* なし:R */
            slt_no                                        /* スロット番号:I */
        );
    }
    
    /**********************************************/
    /* 残トライ回数(REキャリア状態報告要求)初期化 */
    /**********************************************/
    f_trp_rec_Fc02_t_tc_recrrstsreq_ini(                          /* なし:R */
    );
    f_trp_rec_Fc02_t_use_recrrstsreq_ini(                          /* なし:R */
    );

	/**********************************************/
    /* 残トライ回数(MTアドレス設定)初期化         */
    /**********************************************/
	f_trp_rec_Fc02_t_tc_mtsetfreq_set(CMD_NUM0);
	f_trp_rec_Fc02_t_tc_mtsetsreq_set(CMD_NUM0);

    f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].crdssc_num = CMD_NUM0;
    f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].crdscm_num = CMD_NUM0;

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/