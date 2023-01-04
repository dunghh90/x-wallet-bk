/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_cprpar_chk.c
 * @brief  パラメータチェック処理
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
 * @brief  パラメータチェック処理
 * @note   CPRI信号受信通知で受信したデータ部についてパラメータチェックを行う
 *         - CPRI信号受信値チェック
 * @param  bufp [in]  バッファポインタ
 * @return 応答結果
 * @retval 0:正常
 * @retval 1:異常
 * @date   2008/07/21 FFCS)Shihzh create.
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_cprpar_chk(                                 /* 変化:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    UINT    ret_code = CMD_OK;                                    /*  結果  */
    CMT_TSKIF_CPRIRCV_SIGNALGET* rcvp;
                                                   /* CPRI信号受信ポインタ  */
    rcvp = (CMT_TSKIF_CPRIRCV_SIGNALGET*)bufp;
                                            /* CPRI信号受信通知アドレス設定 */
    /************************************************************************/
    /* CPRI信号受信通知パラメータチェック処理                               */
    /************************************************************************/
    /* メッセージの信号種別で分岐 */
    switch (rcvp->signal_kind & CMD_SYS_MSK_CPRID)
    {
    case CMD_CPRID_TOFFSETINFRES:                       /* Toffset値報告応答 */
    case CMD_CPRID_REPORTSTSRES:                      /* RE PORT状態報告応答 */
                       /* パラメータチェック対象が存在しないためチェック終了 */
        return CMD_OK;

    /****************************/
    /* RE装置構成情報報告応答    */
    /****************************/
    case CMD_CPRID_EQPSTRRES:
        ret_code = f_trp_rec_Fc02_t_reeqpinf_chk(
            (VOID *)&(rcvp->signal_kind)
        );
                                /* RE装置構成情報パラメータチェック処理を起動 */
        break ;
    
    /****************************/
    /* REスロット情報報告応答    */
    /****************************/
    case CMD_CPRID_SLOTINFRES:
        ret_code = f_trp_rec_Fc02_t_resltinf_chk(
            (VOID *)&(rcvp->signal_kind)
        );
                                /* REスロット情報パラメータチェック処理を起動 */
        break ;

    /****************************/
    /* RE状態報告応答            */
    /****************************/
    case CMD_CPRID_RESTSRES:
        ret_code = f_trp_rec_Fc02_t_rests_chk(
            (VOID *)&(rcvp->signal_kind)
        );
                                       /* RE状態パラメータチェック処理を起動 */
        break ;

    /****************************/
    /* REカード状態報告応答        */
    /****************************/
    case CMD_CPRID_CARDSTSRES:
        ret_code = f_trp_rec_Fc02_t_recrdsts_chk(
            (VOID *)&(rcvp->signal_kind)
        );
                                 /* REカード状態パラメータチェック処理を起動 */
        break ;

    /****************************/
    /* REキャリア状態報告応答    */
    /****************************/
    case CMD_CPRID_CARLSTSRES:
        ret_code = f_trp_rec_Fc02_t_recrrsts_chk(
            (VOID *)&(rcvp->signal_kind)
        );
                                /* REキャリア状態パラメータチェック処理を起動 */
        break;

    /****************************/
    /* 信号種別不特定の場合        */
    /****************************/
    default:
        return CMD_NG;
    }
    /* 復帰値返却 */
    return ret_code;
    
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
