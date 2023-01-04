/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_resltinf_chk.c
 * @brief  REスロット情報報告応答パラメータチェック処理
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
 * @brief  REスロット情報報告応答パラメータチェック処理
 * @note   REスロット情報報告応答のパラメータチェックを行う
 * @param  l3_bufp [in]  L3バッファポインタ
 * @return 応答結果
 * @retval 0:正常
 * @retval 1:異常
 * @date   2008/07/21 FFCS)Shihzh create.
 * @date   2015/09/21 FPT)Duong modify for FHM.
 * @date   2016/10/04 KCN)hfuku 16B機能追加
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_resltinf_chk(                               /* 変化:R */
    VOID* l3_bufp                                   /* L3バッファポインタ:I */
) {
    USHORT    rcv_slotinfnum    = CMD_NUM0;               /* スロット情報数 */
    USHORT    rcv_slotno        = CMD_NUM0;                 /* スロット番号 */
    USHORT    *offset           = CMD_NUM0;               /* 参照オフセット */
//    USHORT    slt_cnt           = CMD_NUM0;     /* スロット数ループカウンタ */
    CHAR     errdata[CMD_NUM48];
    CMT_CPRIF_SLOTINFRES *bufp;                /* REスロット情報報告応答MSG */
    USHORT    slt_cnt;

    /* アドレス設定 */
    bufp = (CMT_CPRIF_SLOTINFRES *)l3_bufp;
    
    /************************************************************************/
    /* REスロット情報報告応答パラメータチェック処理                            */
    /************************************************************************/
    /* REスロット情報詳細先頭情報オフセット設定 */
    offset = &(bufp->result);
    
    /* スロット情報数OFFSET算出 */
    offset += CMD_NUM1;
    
    /* スロット情報数設定 */
    rcv_slotinfnum = *offset;
	
    /*************************************************************************/
    /* スロット情報数が１～１６以外の場合をチェックする。 16B KCN chg        */
    /*************************************************************************/
    if( (rcv_slotinfnum < CMD_NUM1)  || (CMD_SLOTINF_MAX < rcv_slotinfnum) )
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d]Param check NG(slotinfnum: %d).", D_REC_C02_CPR_NO(), rcv_slotinfnum);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
        return CMD_NG;
    }

    /* スロット番号OFFSET算出 */
    offset += CMD_NUM1;

    for(slt_cnt = CMD_NUM0; slt_cnt < rcv_slotinfnum; slt_cnt++)
    {
        /* スロット番号設定 */
        rcv_slotno = *offset;

        /*************************************************************************/
        /* スロット番号が１～５６以外の場合をチェックする。 16B KCN chg          */
        /*************************************************************************/
        if( (rcv_slotno < CMD_MINSLOTNO) || (CMD_MAXSLOTNO < rcv_slotno) )
        {
            /* パラメータ範囲外のためアサートを出力 */
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d]Param check NG(slotno: %d).", D_REC_C02_CPR_NO(), rcv_slotno);
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
            return CMD_NG;
        }
        /* スロット番号OFFSET算出(機能情報部分をSkipするので２加算) */
        offset += CMD_NUM2;

    }
    /* 復帰値返却 */
    return CMD_OK;
    
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
