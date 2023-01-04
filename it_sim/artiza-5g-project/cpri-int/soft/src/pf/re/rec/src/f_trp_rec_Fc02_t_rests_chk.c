/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_rests_chk.c
 * @brief  REスロット情報報告応答パラメータチェック処理
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
 * @brief  RE状態報告応答パラメータチェック処理
 * @note   RE状態報告応答のパラメータチェックを行う
 * @param  l3_bufp [in]  L3バッファポインタ
 * @return 応答結果
 * @retval 0:正常
 * @retval 1:異常
 * @date   2008/07/21 FFCS)Shihzh create.
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_rests_chk(                                  /* 変化:R */
    VOID* l3_bufp                                   /* L3バッファポインタ:I */
) {
    USHORT    rcv_faninfnum    = CMD_NUM0;                     /* FAN情報数 */
    USHORT    rcv_faninf       = CMD_NUM0;                       /* FAN情報 */
    USHORT    rcv_makerinfnum  = CMD_NUM0;          /* メーカー固有RE情報数 */
    USHORT    rcv_makerinf     = CMD_NUM0;            /* メーカー固有RE情報 */
    USHORT    fan_cnt          = CMD_NUM0;           /* FAN数ループカウンタ */
    USHORT    maker_cnt        = CMD_NUM0; 
                                      /* メーカー固有RE情報数ループカウンタ */
    USHORT    *off_set         = NULL;                    /* 参照オフセット */
    CMT_CPRIF_RESTSRES *bufp;                          /* RE状態報告応答MSG */
    CHAR     errdata[CMD_NUM48];
    
    /* アドレス設定 */
    bufp = (CMT_CPRIF_RESTSRES *)l3_bufp;
    
    /************************************************************************/
    /* RE状態報告応答パラメータチェック処理                                 */
    /************************************************************************/
    /****************************************/
    /* CHECK!メーカ名(範囲:65～90)            */
    /****************************************/
    if((bufp->rests_inf.maker_name < CMD_MAKERMIN)||
       (bufp->rests_inf.maker_name > CMD_MAKERMAX) )
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][mak:0x%02x]Maker check NG.", D_REC_C02_CPR_NO(), bufp->rests_inf.maker_name);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
        return CMD_NG ;
        
    }

    /* RE状態詳細先頭情報オフセット設定 */
    off_set = &(bufp->rests_inf.maker_name);

    /* FAN情報数OFFSET算出 */
    off_set += CMD_NUM1;
    
    /* FAN情報数設定 */
    rcv_faninfnum = *off_set;

    /************************************/
    /* CHECK!FAN情報数(範囲:0～16)        */
    /************************************/
    if(rcv_faninfnum > CMD_FAN_NUM)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][fannum:0x%02x]Fan inf number check NG.", D_REC_C02_CPR_NO(), rcv_faninfnum); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
        return CMD_NG ;
    }

    /* FAN情報数分ループ */
    for(fan_cnt = CMD_NUM0;fan_cnt < rcv_faninfnum ;fan_cnt++)
    {
        /* FAN情報OFFSET算出 */
        off_set += CMD_NUM1;
        
        /* FAN情報設定 */
        rcv_faninf = *off_set;
        
        /****************************************/
        /* CHECK!FAN情報(範囲:1～)              */
        /****************************************/
        if(rcv_faninf < CMD_FANMIN)
        {
            /* パラメータ範囲外のためアサートを出力 */
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][faninf:0x%02x]Fan inf check NG.", D_REC_C02_CPR_NO(), rcv_faninf);
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
            return CMD_NG ;
        }
    }

    /* メーカー固有RE情報数OFFSET算出 */
    off_set += CMD_NUM1;
    
    /* メーカー固有RE情報数設定 */
    rcv_makerinfnum = *off_set;
    
    /********************************************/
    /* CHECK!メーカー固有RE情報数(範囲:0～32)   */
    /********************************************/
    if(rcv_makerinfnum > CMD_MAKER_NUM)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][maknum:0x%02x]Maker inf number check NG.", D_REC_C02_CPR_NO(), rcv_makerinfnum); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);
        return CMD_NG ;
    }

    /* メーカー固有RE情報数分ループ */
    for(maker_cnt = CMD_NUM0;maker_cnt < rcv_makerinfnum ;maker_cnt++)
    {
        /* メーカー固有RE情報OFFSET算出 */
        off_set += CMD_NUM1;
        
        /* メーカー固有RE情報設定 */
        rcv_makerinf = *off_set;
        
        /****************************************/
        /* CHECK!メーカー固有RE情報(範囲:1～)   */
        /****************************************/
        if(rcv_makerinf < CMD_MK_REMIN)
        {
            /* パラメータ範囲外のためアサートを出力 */
            snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][makinf:0x%02x]Maker inf check NG.", D_REC_C02_CPR_NO(), rcv_makerinf);
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
            D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);
            return CMD_NG ;
        }
    }
    
    /* 復帰値返却 */
    return CMD_OK;    
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
