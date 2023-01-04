/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_recrrsts_chk.c
 * @brief  REキャリア状態報告応答パラメータチェック処理
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
 * @brief  REキャリア状態報告応答パラメータチェック処理
 * @note   REキャリア状態報告応答のパラメータチェックを行う
 * @param  l3_bufp [in]  L3バッファポインタ
 * @return 応答結果
 * @retval 0:正常
 * @retval 1:異常
 * @date   2008/07/21 FFCS)Shihzh create.
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_recrrsts_chk_3g(                            /* 変化:R */
    VOID* l3_bufp                                   /* L3バッファポインタ:I */
) {
    USHORT    rcv_crrnum    = CMD_NUM0;            /* TRX設定済みキャリア数 */
    USHORT    rcv_crrno     = CMD_NUM0;                     /* キャリア番号 */
    USHORT    rcv_crreqp    = CMD_NUM0;       /* キャリア状態オフセット算出 */
    USHORT    rcv_alminfnum = CMD_NUM0;                        /* ALM情報数 */
    USHORT    rcv_alminf    = CMD_NUM0;                          /* ALM情報 */
    USHORT    rcv_errinfnum = CMD_NUM0;                        /* ERR情報数 */
    USHORT    rcv_errinf    = CMD_NUM0;                          /* ERR情報 */
    USHORT    rcv_crdinfnum = CMD_NUM0;                         /* カード数 */
    USHORT    crr_cnt       = CMD_NUM0;         /* キャリア数ループカウンタ */
    USHORT    alm_cnt       = CMD_NUM0;              /* ALM数ループカウンタ */
    USHORT    err_cnt       = CMD_NUM0;              /* ERR数ループカウンタ */
    USHORT    *offset       = NULL;                       /* 参照オフセット */
    CHAR     errdata[CMD_NUM48];
    CMT_CPRIF_CARLSTSRES *bufp;                /* REキャリア状態報告応答MSG */
    
    /* REキャリア状態報告応答MSGアドレス設定 */
    bufp = (CMT_CPRIF_CARLSTSRES *)l3_bufp;
    
    /************************************************************************/
    /* REキャリア状態報告応答パラメータチェック処理                          */
    /************************************************************************/
    /* TRX設定済みキャリア数設定 */
    rcv_crrnum = bufp->car_num;
    
    /********************************************/
    /* CHECK! TRX設定済みキャリア数(範囲:0～4)    */
    /********************************************/
    if(rcv_crrnum > CMD_CARNO_MAX_3G) {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][crrnum:0x%02x]Carrier number check NG.", D_REC_C02_3G_CPR_NO(), rcv_crrnum); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
        return CMD_NG ;
    }
    
    /* ==================================================================== */
    /* キャリア情報はTRX設定済みのキャリアのみを対象に報告してくるため      */
    /* 以下、TRX設定済みキャリア数分ループしパラメータチェックを実施する    */
    /* ==================================================================== */
    /* キャリア状態詳細先頭情報オフセット設定 */
    offset = &(bufp->car_num);
    offset++;
    for(crr_cnt = CMD_NUM0; crr_cnt < rcv_crrnum ; crr_cnt++) {
        /* キャリア番号設定 */
        rcv_crrno = *offset;
        /************************************/
        /* CHECK!キャリア番号(範囲:1~4)       */
        /************************************/
        if((rcv_crrno < CMD_CARNO_MIN) || (rcv_crrno > CMD_CARNO_MAX_3G)) {
            /* パラメータ範囲外のためアサートを出力 */
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][crrno:0x%02x]Carrier no check NG.", D_REC_C02_3G_CPR_NO(), rcv_crrno); /* pgr0570 */
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
            return CMD_NG ;
        }
        
        /* キャリア状態オフセット算出 */
        offset += CMD_NUM1;

        /* キャリア構成状態 */
        offset += CMD_NUM1;
        rcv_crreqp = *offset;
        /************************************/
        /* CHECK!キャリア構成状態(範囲:0～2)*/
        /************************************/
        if (rcv_crreqp > CMD_NUM2) {
            /* パラメータ範囲外のためアサートを出力 */
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][crrno:0x%02x]Carrier_eqp check NG.", D_REC_C02_3G_CPR_NO(), rcv_crrno); /* pgr0570 */
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
            return CMD_NG ;
        }

        /* ALM情報数OFFSET算出 */
        offset += CMD_NUM1; 
        /* ALM情報数設定 */
        rcv_alminfnum = *offset;
        /************************************/
        /* CHECK!ALM情報数(範囲:0～255)     */
        /************************************/
        if(rcv_alminfnum > CMD_CARALM_INFNUMMAX) {
            /* パラメータ範囲外のためアサートを出力 */
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][almnum:0x%02x]Alm number check NG.", D_REC_C02_3G_CPR_NO(), rcv_alminfnum); /* pgr0570 */
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
            return CMD_NG ;
        }

        /* ALM情報数分ループ */
        for(alm_cnt = CMD_NUM0; alm_cnt < rcv_alminfnum; alm_cnt++) {
            /* ALM情報OFFSET算出 */
            offset += CMD_NUM1;            
            /* ALM情報設定 */
            rcv_alminf = *offset;            
            /************************************/
            /* CHECK!ALM情報(範囲:221～255)     */
            /************************************/
            if((rcv_alminf < CMD_CARALM_INFMIN) ||
               (rcv_alminf > CMD_CARALM_INFMAX) ){
                /* パラメータ範囲外のためアサートを出力 */
                snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][alminf:0x%02x]Alm inf check NG.", D_REC_C02_3G_CPR_NO(), rcv_alminf);
                D_REC_IVLINI();
                D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
                D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
                return CMD_NG ;
            }
        }

        /************************************************/
        /*                                              */
        /*                      ERR情報数               */
        /*                                              */
        /************************************************/
        
        
        /* ERR情報数OFFSET算出 */
        offset += CMD_NUM1;
        /* ERR情報数設定 */
        rcv_errinfnum = *offset;
        /************************************/
        /* CHECK!ERR情報数(範囲:0～255)     */
        /************************************/
        if(rcv_errinfnum > CMD_CARERR_INFNUMMAX) {
            /* パラメータ範囲外のためアサートを出力 */
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][errnum:0x%02x]Err number check NG.", D_REC_C02_3G_CPR_NO(), rcv_errinfnum); /* pgr0570 */
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
            return CMD_NG ;
        }
        
        /* ERR情報数分ループ */
        for(err_cnt = CMD_NUM0; err_cnt < rcv_errinfnum; err_cnt++) {
            /* ALM情報OFFSET算出 */
            offset += CMD_NUM1;            
            /* ALM情報設定 */
            rcv_errinf = *offset;            
            /************************************/
            /* CHECK!ERR情報(範囲:221～255)     */
            /************************************/
            if((rcv_errinf < CMD_CARERR_INFMIN) ||
               (rcv_errinf > CMD_CARERR_INFMAX) ){
                /* パラメータ範囲外のためアサートを出力 */
                snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][errinf:0x%02x]Err inf check NG.", D_REC_C02_3G_CPR_NO(), rcv_errinf);
                D_REC_IVLINI();
                D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
                D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
                return CMD_NG ;
            }
        }

        /* カード数OFFSET算出 */
        offset += CMD_NUM1;
        /* カード数設定 */
        rcv_crdinfnum = *offset;
        /************************************/
        /* CHECK!カード数(範囲:0～6)        */
        /************************************/
        if(rcv_crdinfnum > CMD_CARCRDMAX) {
            /* パラメータ範囲外のためアサートを出力 */
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][crdnum:0x%02x]Card number check NG.", D_REC_C02_3G_CPR_NO(), rcv_crdinfnum); /* pgr0570 */
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
            return CMD_NG ;
        }
        
        offset += rcv_crdinfnum;

        /* 次のキャリア番号を対象にパラメータチェックを行う */
        offset += CMD_NUM1;
    }
    
    /* 復帰値返却 */
    return CMD_OK;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
