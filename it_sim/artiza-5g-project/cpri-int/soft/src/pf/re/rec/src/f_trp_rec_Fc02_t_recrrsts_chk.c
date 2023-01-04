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
UINT f_trp_rec_Fc02_t_recrrsts_chk(                               /* 変化:R */
    VOID* l3_bufp                                   /* L3バッファポインタ:I */
) {
    USHORT    rcv_crrnum    = CMD_NUM0;            /* TRX設定済みキャリア数 */
    USHORT    rcv_crrno     = CMD_NUM0;                     /* キャリア番号 */
    USHORT    rcv_brcnum    = CMD_NUM0;                       /* ブランチ数 */
    USHORT    rcv_brcno     = CMD_NUM0;                     /* ブランチ番号 */
    USHORT    rcv_alminfnum = CMD_NUM0;                        /* ALM情報数 */
    USHORT    rcv_alminf    = CMD_NUM0;                          /* ALM情報 */
    USHORT    rcv_crdinfnum = CMD_NUM0;                         /* カード数 */
    USHORT    rcv_slotno    = CMD_NUM0;                     /* スロット番号 */
    USHORT    crr_cnt       = CMD_NUM0;         /* キャリア数ループカウンタ */
    USHORT    brc_cnt       = CMD_NUM0;         /* ブランチ数ループカウンタ */
    USHORT    alm_cnt       = CMD_NUM0;              /* ALM数ループカウンタ */
    USHORT    slt_cnt       = CMD_NUM0;         /* スロット数ループカウンタ */
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
    /* CHECK! TRX設定済みキャリア数(範囲:0～5)    */
    /********************************************/
    if(rcv_crrnum > CMD_CARNO_MAX) {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][crrnum:0x%02x]Carrier number check NG.", D_REC_C02_CPR_NO(), rcv_crrnum); /* pgr0570 */
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
        /* CHECK!キャリア番号(範囲:1～5)       */
        /************************************/
        if((rcv_crrno < CMD_CARNO_MIN)  || (rcv_crrno > CMD_CARNO_MAX)){
            /* パラメータ範囲外のためアサートを出力 */
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][crrno:0x%02x]Carrier no check NG.", D_REC_C02_CPR_NO(), rcv_crrno); /* pgr0570 */
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
            return CMD_NG ;
        }
        
        /* キャリア状態オフセット算出 */
        offset += CMD_NUM3;        
        /********************************************************************/
        /* ※キャリア状態は範囲指定されていないためパラメータチェック対象外 */
        /********************************************************************/
        
        /* ブランチ数オフセット算出 */
        offset += CMD_NUM1;
        /* ブランチ数設定 */
        rcv_brcnum = *offset;
        /************************************/
        /* CHECK!ブランチ数(範囲:0～8)      */
        /************************************/
        if((rcv_brcnum < D_REC_BRCNUM_MIN) ||  /* pgr0036 */
           (rcv_brcnum > D_REC_BRCNUM_MAX) ){
            /* パラメータ範囲外のためアサートを出力 */
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][brcnum:0x%02x]Branch number check NG.", D_REC_C02_CPR_NO(), rcv_brcnum); /* pgr0570 */
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
            return CMD_NG ;
        }

        /* ブランチ情報数分ループ */
        for (brc_cnt = CMD_NUM0; brc_cnt < rcv_brcnum; brc_cnt++) {
            /* ブランチ番号オフセット算出 */
            offset += CMD_NUM1;
            /* ブランチ番号設定 */
            rcv_brcno = *offset;
            /*********************************/
            /* CHECK!ブランチ番号(範囲:0～7) */
            /*********************************/
            if (rcv_brcno > D_REC_BRCNO_MAX) {
                /* パラメータ範囲外のためアサートを出力 */
                snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][brcno:0x%02x]Branch no check NG.", D_REC_C02_CPR_NO(), rcv_brcno); /* pgr0570 */
                D_REC_IVLINI();
                D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
                D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
                return CMD_NG ;
            }
            
            /* ブランチ#1 キャリア状態オフセット算出 */
            offset += CMD_NUM3;        
            /*******************************************************************************/
            /* ※ブランチ#1 キャリア状態は範囲指定されていないためパラメータチェック対象外 */
            /*******************************************************************************/
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
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][almnum:0x%02x]Alm number check NG.", D_REC_C02_CPR_NO(), rcv_alminfnum); /* pgr0570 */
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
                snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][alminf:0x%02x]Alm inf check NG.", D_REC_C02_CPR_NO(), rcv_alminf); /* pgr0570 */
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
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][crdnum:0x%02x]Card number check NG.", D_REC_C02_CPR_NO(), rcv_crdinfnum); /* pgr0570 */
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
            return CMD_NG ;
        }
        
        /* カード数分ループ */
        for(slt_cnt = CMD_NUM0; slt_cnt < rcv_crdinfnum; slt_cnt++) {
            /* スロット番号OFFSET算出 */
            offset += CMD_NUM1;            
            /* スロット番号設定 */
            rcv_slotno = *offset;        
            /************************************/
            /* CHECK!スロット番号(範囲:1～56)    */
            /************************************/
            if((rcv_slotno < CMD_MINSLOTNO) || 
               (rcv_slotno > CMD_MAXSLOTNO) ){
                /* パラメータ範囲外のためアサートを出力 */
                snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][slt:0x%02x]Slot No check NG.", D_REC_C02_CPR_NO(), rcv_slotno);
                D_REC_IVLINI();
                D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
                D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
                return CMD_NG ;
            }
        }
    
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
