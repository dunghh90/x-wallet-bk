/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_recrdsts_chk.c
 * @brief  REカード状態報告応答パラメータチェック処理
 * @date   2008/07/21 FFCS)Shihzh create.
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2016
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  REカード状態報告応答パラメータチェック処理
 * @note   REカード状態報告応答のパラメータチェックを行う
 * @param  l3_bufp [in]  L3バッファポインタ
 * @return 応答結果
 * @retval 0:正常
 * @retval 1:異常
 * @date   2008/07/21 FFCS)Shihzh create.
 * @date   2009/04/11 FFCS)Shihzh CR-XXX-XXX CPRI Spec V1.0.7.
 * @date   2016/10/04 KCN)hfuku 16Aのバグ修正
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_recrdsts_chk_3g(                               /* 変化:R */
    VOID* l3_bufp                                   /* L3バッファポインタ:I */
) {
    USHORT    *offset       = CMD_NUM0;                   /* 参照オフセット */
    USHORT	  rcv_alminfnum;
    USHORT	  rcv_errinfnum;
    CMT_CPRIF_CARDSTSRES *bufp;                  /* REカード状態報告応答MSG */
    CHAR  errdata[CMD_NUM48];

    /* アドレス設定 */
    bufp = (CMT_CPRIF_CARDSTSRES *)l3_bufp;

    /************************************************************************/
    /* REカード状態報告応答パラメータチェック処理                            */
    /************************************************************************/
    /**********************************************/
    /* CHECK!スロット情報(範囲:1～56) 16B KCN chg */
    /**********************************************/
    if( (bufp->cardsts_inf.slot_no < CMD_MINSLOTNO)||
        (bufp->cardsts_inf.slot_no > CMD_MAXSLOTNO) )
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][slt:0x%02x]Slot No check NG.", D_REC_C02_3G_CPR_NO(), bufp->cardsts_inf.slot_no);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
        return CMD_NG ;
    }

    /****************************************/
    /* CHECK!年(範囲:0～9999)                */
    /****************************************/
    if(bufp->cardsts_inf.year > CMD_YEARMAX)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][year:0x%02x]Year check NG.", D_REC_C02_3G_CPR_NO(), bufp->cardsts_inf.year);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
        return CMD_NG ;
    }

    /****************************************/
    /* CHECK!月日(範囲:0～0x1231)                */
    /****************************************/
    if(bufp->cardsts_inf.month_day >CMD_MONDAYMAX)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][month_day:0x%02x]Month day check NG.", D_REC_C02_3G_CPR_NO(), bufp->cardsts_inf.month_day); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
        return CMD_NG ;
    }

    /******************************************/
    /* CHECK!時(範囲:0～0x0023)(上位1バイトはD.C.)*/
    /******************************************/
    if(((UCHAR)(bufp->cardsts_inf.hour)) > CMD_HOURMAX)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][hour:0x%02x]Hour check NG.", D_REC_C02_3G_CPR_NO(), bufp->cardsts_inf.hour);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
        return CMD_NG ;
    }

    /****************************************/
    /* CHECK!分秒(範囲:0～5999)             */
    /****************************************/
    if(bufp->cardsts_inf.minsec > CMD_MINSECMAX)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][minsec:0x%02x]Minsec check NG.", D_REC_C02_3G_CPR_NO(), bufp->cardsts_inf.minsec);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
        return CMD_NG ;
    }
    /* 機能部数先頭情報オフセット設定 */
    offset = &(bufp->cardsts_inf.car_inf);
    /************************************/
    /* CHECK!ALM情報数(範囲:0～255)        */
    /************************************/
    offset += CMD_NUM1;
    rcv_alminfnum = *offset;
    if(rcv_alminfnum > CMD_CRDALM_INFNUMMAX)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][almnum:0x%02x]Alm number check NG.", D_REC_C02_3G_CPR_NO(), rcv_alminfnum); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
        return CMD_NG ;
    }

    offset += rcv_alminfnum;

    /* ERR情報数OFFSET算出 */
    offset += CMD_NUM1;
    /* ERR情報数設定 */
    rcv_errinfnum = *offset;
    /************************************/
    /* CHECK!ERR情報数(範囲:0～255)        */
    /************************************/
    if(rcv_errinfnum > CMD_CRDERR_INFNUMMAX)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][errnum:0x%02x]Err number check NG.", D_REC_C02_3G_CPR_NO(), rcv_errinfnum); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
        return CMD_NG ;
    }


    /* 戻り値返却 */
    return CMD_OK ;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
