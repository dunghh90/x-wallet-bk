/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_recrdsts_chk.c
 * @brief  REカード状態報告応答パラメータチェック処理
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
 * @brief  REカード状態報告応答パラメータチェック処理
 * @note   REカード状態報告応答のパラメータチェックを行う
 * @param  l3_bufp [in]  L3バッファポインタ
 * @return 応答結果
 * @retval 0:正常
 * @retval 1:異常
 * @Bug_No M-RRU-ZSYS-01826
 * @date   2008/07/21 FFCS)Shihzh create.
 * @date   2009/04/11 FFCS)Shihzh CR-XXX-XXX CPRI Spec V1.0.7.
 * @date   2015/10/28 TDIPS)Lay M-RRU-ZSYS-01826 パラメータチェックの範囲を修正
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_recrdsts_chk(                               /* 変化:R */
    VOID* l3_bufp                                   /* L3バッファポインタ:I */
) {
    USHORT    rcv_fbnum     = CMD_NUM0;                         /* 機能部数 */
    USHORT    fb_cnt        = CMD_NUM0;           /* 機能部数ループカウンタ */
    USHORT    rcv_fbtyp     = CMD_NUM0;                       /* 機能部種別 */
    USHORT    rcv_trbsts    = CMD_NUM0;                   /* 機能部故障状態 */
    USHORT    rcv_crrnum    = CMD_NUM0;                       /* キャリア数 */
    USHORT    rcv_crrno     = CMD_NUM0;                     /* キャリア番号 */
    USHORT    crr_cnt       = CMD_NUM0;           /* キャリアループカウンタ */
    USHORT    rcv_brcnum    = CMD_NUM0;                       /* ブランチ数 */
    USHORT    rcv_brcno     = CMD_NUM0;                     /* ブランチ番号 */
    USHORT    rcv_crrbnd    = CMD_NUM0;         /* キャリアルシステム帯域幅 */
    USHORT    brc_cnt       = CMD_NUM0;           /* ブランチループカウンタ */
    USHORT    alm_cnt       = CMD_NUM0;              /* ALM数ループカウンタ */
    USHORT    rcv_alminfnum = CMD_NUM0;                        /* ALM情報数 */
    USHORT    rcv_alminf    = CMD_NUM0;                          /* ALM情報 */
    USHORT    err_cnt       = CMD_NUM0;              /* ERR数ループカウンタ */
    USHORT    rcv_errinfnum = CMD_NUM0;                        /* ERR情報数 */
    USHORT    rcv_errinf    = CMD_NUM0;                          /* ERR情報 */
    USHORT    *offset       = CMD_NUM0;                   /* 参照オフセット */
    CMT_CPRIF_CARDSTSRES_S3G *bufp;                  /* REカード状態報告応答MSG */
    CHAR  errdata[CMD_NUM48];

    /* アドレス設定 */
    bufp = (CMT_CPRIF_CARDSTSRES_S3G *)l3_bufp;

    /************************************************************************/
    /* REカード状態報告応答パラメータチェック処理                            */
    /************************************************************************/
    /****************************************/
    /* CHECK!スロット情報(範囲:1～56)        */
    /****************************************/
    if( (bufp->cardsts_inf.slot_no < CMD_MINSLOTNO)||
        (bufp->cardsts_inf.slot_no > CMD_MAXSLOTNO) )
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][slt:0x%02x]Slot No check NG.", D_REC_C02_CPR_NO(), bufp->cardsts_inf.slot_no);
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
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][year:0x%02x]Year check NG.", D_REC_C02_CPR_NO(), bufp->cardsts_inf.year);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
        return CMD_NG ;
    }

    /****************************************/
    /* CHECK!月日(範囲:0～9999)                */
    /****************************************/
    if(bufp->cardsts_inf.month_day >CMD_MONDAYMAX)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][month_day:0x%02x]Month day check NG.", D_REC_C02_CPR_NO(), bufp->cardsts_inf.month_day); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
        return CMD_NG ;
    }
        
    /******************************************/
    /* CHECK!時(範囲:0～99)(上位1バイトはD.C.)*/
    /******************************************/
    if(((UCHAR)(bufp->cardsts_inf.hour)) > CMD_HOURMAX)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][hour:0x%02x]Hour check NG.", D_REC_C02_CPR_NO(), bufp->cardsts_inf.hour);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
        return CMD_NG ;
    }

    /****************************************/
    /* CHECK!分秒(範囲:0～9999)             */
    /****************************************/
    if(bufp->cardsts_inf.minsec > CMD_MINSECMAX)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][minsec:0x%02x]Minsec check NG.", D_REC_C02_CPR_NO(), bufp->cardsts_inf.minsec);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
        return CMD_NG ;
    }

    /* 機能部数先頭情報オフセット設定 */
    offset = &(bufp->cardsts_inf.card_extcntitem);
	offset++;
    /* 機能部数設定 */
    rcv_fbnum = *offset;
    /****************************************/
    /* CHECK!機能部数(範囲:0～29)           */
    /****************************************/
    if (rcv_fbnum > D_REC_FBNUM_MAX)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][fbnum:0x%02x]Function number check NG.", D_REC_C02_CPR_NO(), rcv_fbnum); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
        return CMD_NG ;
    }

    /* 対象の機能部情報数分ループ  */
    for (fb_cnt = CMD_NUM0; fb_cnt < rcv_fbnum; fb_cnt++) 
    {
        /* 機能部種別OFFSET算出 */
        offset += CMD_NUM1;    
        /* 機能部種別設定 */
        rcv_fbtyp = *offset;
        /****************************************/
        /* CHECK!機能部種別(範囲:0～28)         */
        /****************************************/
        if (rcv_fbtyp > D_REC_FBTYP_MAX)
        {
            /* パラメータ範囲外のためアサートを出力 */
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][fbtype:0x%02x]Function type check NG.", D_REC_C02_CPR_NO(), rcv_fbtyp); /* pgr0570 */
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
            return CMD_NG ;
        }
        
        /* 機能部故障状態OFFSET算出 */
        offset += CMD_NUM1;    
        /* 機能部故障状態設定 */
        rcv_trbsts = *offset;
        /****************************************/
        /* CHECK!機能部故障状態(範囲:0～3)      */
        /****************************************/
        if (rcv_trbsts > D_REC_TRBSTS_MAX)
        {
            /* パラメータ範囲外のためアサートを出力 */
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][trbsts:0x%02x]Trouble status check NG.", D_REC_C02_CPR_NO(), rcv_trbsts); /* pgr0570 */
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
            return CMD_NG ;
        }
        
        /* 対象キャリア数OFFSET算出 */
        offset += CMD_NUM3;    
        /* 対象キャリア数設定 */
        rcv_crrnum = *offset;
        /****************************************/
        /* CHECK!対象キャリア数(範囲:0～1)      */
        /****************************************/
        if (rcv_crrnum > CMD_CARNO_MAX)
        {
            /* パラメータ範囲外のためアサートを出力 */
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][crrnum:0x%02x]Carrier number check NG.", D_REC_C02_CPR_NO(), rcv_crrnum); /* pgr0570 */
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
            return CMD_NG ;
        }
         
        /* 対象のキャリア情報数分ループ  */
        for (crr_cnt = CMD_NUM0; crr_cnt < rcv_crrnum; crr_cnt++) 
        {
            /* 報告対象のキャリア番号OFFSET算出 */
            offset += CMD_NUM1;    
            /* 報告対象のキャリア番号設定 */
            rcv_crrno = *offset;
            /****************************************/
            /* CHECK!対象のキャリア番号(1~2)     */
            /****************************************/
        	if ((rcv_crrno < CMD_CARNO_MIN) || (rcv_crrno > CMD_CARNO_MAX))
            {
                /* パラメータ範囲外のためアサートを出力 */
                snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][crrno:0x%02x]Carrier number check NG.", D_REC_C02_CPR_NO(), rcv_crrno); /* pgr0570 */
                D_REC_IVLINI();
                D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
                D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
                return CMD_NG ;
            }

            /* 報告対象のシステム帯域幅OFFSET算出 */
            offset += CMD_NUM1;
            /* 報告対象のシステム帯域幅設定 */
            rcv_crrbnd = *offset;
            if ( (rcv_crrbnd < D_REC_SYSBND_05MHZ) || 
                 (rcv_crrbnd > D_REC_SYSBND_20MHZ) )
            {
                /* パラメータ範囲外のためアサートを出力 */
                snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][crrbnd:0x%02x]Carrier sysbnd check NG.", D_REC_C02_CPR_NO(), rcv_crrbnd); /* pgr0570 */
                D_REC_IVLINI();
                D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
                D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
                return CMD_NG ;
            }            
            
            /* 報告対象のブランチ数OFFSET算出 */
            offset += CMD_NUM3;    
            /* 報告対象のブランチ数設定 */
            rcv_brcnum = *offset;
            /****************************************/
            /* CHECK!対象のブランチ数(範囲:0～8)    */
            /****************************************/
            if (rcv_brcnum > D_REC_BRCNUM_MAX)
            {
                /* パラメータ範囲外のためアサートを出力 */
                snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][trcnum:0x%02x]Branch number check NG.", D_REC_C02_CPR_NO(), rcv_brcnum); /* pgr0570 */
                D_REC_IVLINI();
                D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
                D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
                return CMD_NG ;
            }

            /* 対象のブランチ情報数分ループ  */
            for (brc_cnt = CMD_NUM0; brc_cnt < rcv_brcnum; brc_cnt++)
            {
                /* 報告対象のブランチ番号OFFSET算出 */
                offset += CMD_NUM1;    
                /* 報告対象のブランチ番号設定 */
                rcv_brcno = *offset;
                /****************************************/
                /* CHECK!対象のブランチ番号(範囲:0～7)  */
                /****************************************/
                if (rcv_brcno > D_REC_BRCNO_MAX)
                {
                    /* パラメータ範囲外のためアサートを出力 */
                    snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][brcno:0x%02x]Branch No check NG.", D_REC_C02_CPR_NO(), rcv_brcno);
                    D_REC_IVLINI();
                    D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
                    D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
                    return CMD_NG ;
                }
                
                /* 報告対象のブランチ情報(最終位置)OFFSET算出 */
                offset += CMD_NUM2;
            } /* 対象のブランチ情報数分ループ終了 */
        } /* 対象のキャリア情報数分ループ終了 */
    } /* 対象の機能部情報数分ループ終了 */

    /* ALM情報数OFFSET算出 */
    offset += CMD_NUM1;    
    /* ALM情報数設定 */
    rcv_alminfnum = *offset;    
    /************************************/
    /* CHECK!ALM情報数(範囲:0～255)        */
    /************************************/
    if(rcv_alminfnum > CMD_CRDALM_INFNUMMAX_HIG)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][almnum:0x%02x]Alm number check NG.", D_REC_C02_CPR_NO(), rcv_alminfnum); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
        return CMD_NG ;
    }
    
    /* ALM情報数分ループ */
    for(alm_cnt = CMD_NUM0;alm_cnt < rcv_alminfnum ;alm_cnt++)
    {
        /* ALM情報OFFSET算出 */
        offset += CMD_NUM1;        
        /* ALM情報設定 */
        rcv_alminf = *offset;        
        /************************************/
        /* CHECK!ALM情報(範囲:1～255)        */
        /************************************/
        if((rcv_alminf < CMD_CRDALM_INFMIN)||(rcv_alminf > CMD_CRDALM_INFMAX_HIG))
        {
            /* パラメータ範囲外のためアサートを出力 */
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][alminf:0x%02x]Alm inf check NG.", D_REC_C02_CPR_NO(), rcv_alminf);
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
            return CMD_NG ;
        }
    }
    
    /* ERR情報数OFFSET算出 */
    offset += CMD_NUM1;    
    /* ERR情報数設定 */
    rcv_errinfnum = *offset;    
    /************************************/
    /* CHECK!ERR情報数(範囲:0～255)        */
    /************************************/
    if(rcv_errinfnum > CMD_CRDERR_INFNUMMAX_HIG)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][errnum:0x%02x]Err number check NG.", D_REC_C02_CPR_NO(), rcv_errinfnum); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
        return CMD_NG ;
    }
    
    /* ERR情報数分ループ */
    for(err_cnt = CMD_NUM0; err_cnt < rcv_errinfnum ; err_cnt++)
    {
        /* ERR情報OFFSET算出 */
        offset += CMD_NUM1; 
        /* ERR情報設定 */
        rcv_errinf = *offset;    
        /************************************/
        /* CHECK!ERR情報(範囲:1～255)        */
        /************************************/
        if((rcv_errinf < CMD_CRDERR_INFMIN)|| (rcv_errinf > CMD_CRDERR_INFMAX_HIG))
        {
            /* パラメータ範囲外のためアサートを出力 */
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][errinf:0x%02x]Err inf check NG.", D_REC_C02_CPR_NO(), rcv_errinf);
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
            return CMD_NG ;
        }
    }

    /* 戻り値返却 */
    return CMD_OK ;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
