/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc03_r_evt_cnv.c
 * @brief  イベント変換
 * @date   2007/03/20 FJT)Nagasima create.
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"
#include "rrhApi_L2.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  イベント変換
 * @note   受信メッセージをイベントに変換する
 *         - イベント設定
 *         - CPRI番号設定
 * @param  bufp    [in]  バッファポインタ
 * @param  cpr_nop [out] CPRI番号
 * @param  crr_nop [out] キャリア番号
 * @return イベント
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/20 FFCS)Shihzh modify for S3G PRC.
 * @date   2015/09/04 FPT)Yen modify for FHM
 * @date   2015/10/17 FPT)Yen change EC_REC_C02_EVT_NON -> EC_REC_C03_EVT_NON
 * @date   2015/11/20 FPT)Yen update (REキャリア状態報告応答: crr_no=全て)
 * @date   2021/02/04 M&C)Huan.dh add CARONOFF event
 * @date   2021/01/18 M&C)Tri.hn Add CARLSTS event
 */
/****************************************************************************/
ET_REC_C03_EVT f_trp_rec_Fc03_r_evt_cnv(                      /* イベント:R */
   VOID*   bufp,                                      /* バッファポインタ:I */
   USHORT* cpr_nop,                                           /* CPRI番号:O */
   USHORT* crr_nop                                        /* キャリア番号:O */
) {
    UINT                   msg_id;                          /* メッセージID */
    ET_REC_C03_EVT         evt;                                 /* イベント */
    USHORT                 cpr_no;                              /* CPRI番号 */
    USHORT                 crr_no=0;                        /* キャリア番号 */
    UINT                   key_inf;                       /* タイマキー情報 */
    T_REC_TIMELM           elm;                               /* タイマ要素 */
    USHORT                 cpk;                             /* CPRI信号種別 */
    VOID*                  datp;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /********************/
    /* メッセージID取得 */
    /********************/
    msg_id = ((CMT_TSKIF_MSGID_GET*)bufp)->head.msgid;

    /* メッセージIDで分岐 */
    switch (msg_id) {
    /* タイムアウト発生通知 */
    case CMD_TSKIF_TIMOUTNTC:
        /* キー情報取得 */
        key_inf = ((T_RRH_TIMERTO_NTC*)bufp)->data.userkey;

        /****************************/
        /* タイマキー情報→要素変換 */
        /****************************/
        f_trp_rec_Fcom_t_key_elm_cnv(                             /* なし:R */
            key_inf,                                          /* キー情報:I */
            &elm                                                  /* 要素:O */
        );

        /* 他クラスの場合 */
        if (elm.cls != EC_REC_C03) {
            /*==============*/
            /* イベントなし */
            /*==============*/
            evt    = EC_REC_C03_EVT_NON;
            cpr_no = D_REC_CPRINO_NON;
            crr_no = D_REC_CARNO_NON;
            break;
        }

        /* タイマ種別で分岐 */
        switch (elm.tmk) {
        /* TRX設定確認タイマ */
        case EC_REC_TMK_TXS_CNF:
            /*=========================================*/
            /* タイムアウト発生通知(TRX設定確認タイマ) */
            /*=========================================*/
            evt    = EC_REC_C03_EVT_TO_TXS_CNF;
            cpr_no = elm.cpr_no;
            crr_no = elm.soc_no;
            break;
        /* TRX解放確認タイマ */
        case EC_REC_TMK_TXR_CNF:
            /*=========================================*/
            /* タイムアウト発生通知(TRX解放確認タイマ) */
            /*=========================================*/
            evt    = EC_REC_C03_EVT_TO_TXR_CNF;
            cpr_no = elm.cpr_no;
            crr_no = elm.soc_no;
            break;
        /* その他 */
        default:
            /*==============*/
            /* イベントなし */
            /*==============*/
            evt    = EC_REC_C03_EVT_NON;
            cpr_no = D_REC_CPRINO_NON;
            crr_no = D_REC_CARNO_NON;
            break;
        }
        break;

    /* CPRI信号受信通知 */
    case D_API_MSGID_L2_CPRIMSG_RCV_NTC:
        /* CPRI信号種別取得 */
        datp = &((T_REC_MSGRCV_TBL*)bufp)->datp;
        cpk  = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind;
    	/* Check signal type */
        if( (cpk & CMD_SYS_CHK_CPRID) == CMD_NUM0)
        {
        	/* 3Gの場合 */
            evt    = EC_REC_C03_EVT_NON;
            cpr_no = D_REC_CPRINO_NON;
            break;
        }
        /* CPRI信号種別で分岐 */
        switch (cpk & CMD_SYS_MSK_CPRID) {
        /* TRX設定応答 */
        case CMD_CPRID_TRXSETRES:
            /*===============================*/
            /* (02)REからTRX設定応答受信 */
            /*===============================*/
            evt    = EC_REC_C03_EVT_IT_TXS_RSP;
            cpr_no = ((CMT_TSKIF_CPRIRCV_TRXSETRES*)datp)->
                     cprircv_inf.link_num;
            crr_no = ((CMT_TSKIF_CPRIRCV_TRXSETRES*)datp)->
                     cpridat_trxsetres.carno;
            break;

        /* TRX解放応答 */
        case CMD_CPRID_TRXRELRES:
            /*===============================*/
            /* (01)REからTRX解放応答受信 */
            /*===============================*/
            evt    = EC_REC_C03_EVT_IT_TXR_RSP;
            cpr_no = ((CMT_TSKIF_CPRIRCV_TRXRELRES*)datp)->
                     cprircv_inf.link_num;
            crr_no = ((CMT_TSKIF_CPRIRCV_TRXRELRES*)datp)->
                     cpridat_trxrelres.carno;
            break;

        /* 無変調キャリアON/OFF指定応答 */
        case CMD_CPRID_CARONOFFRES:
            /*===============================*/
            /* (01)REから無変調キャリアON/OFF指定応答受信 */
            /*===============================*/
            evt    = EC_REC_C03_EVT_IT_CARONOFF_RSP;
            cpr_no = ((CMT_TSKIF_CPRIRCV_CARONOFFRES_S3G*)datp)->
                     cprircv_inf.link_num;
            crr_no = ((CMT_TSKIF_CPRIRCV_CARONOFFRES_S3G*)datp)->
                     cpridat_caronoffres.carno;
            break;

        /* REキャリア状態報告要求 */
        case CMD_CPRID_CARLSTSRES:
            /*============================================*/
            /* (10)REキャリア状態報告要求 */
            /*============================================*/
            evt    = EC_REC_C03_EVT_IT_CARLSTS_RES;
            cpr_no = ((CMT_TSKIF_CPRISND_CARLSTSRES*)datp)->
                     cprisnd_inf.link_num;
            crr_no = ((CMT_TSKIF_CPRISND_CARLSTSRES*)datp)->
                     cpridat_carstsres.car_num;
            break;

        /* その他 */
        default:
            /*==============*/
            /* イベントなし */
            /*==============*/
            evt    = EC_REC_C03_EVT_NON;
            cpr_no = D_REC_CPRINO_NON;
            crr_no = D_REC_CARNO_NON;
            break;
        }
        break;

    /* (0x4001)RE Forwarding Notice */
    case CMD_TSKIF_RE_FWDNTC:
        /* CPRI信号種別取得 */
        datp = &((T_REC_MSGRCV_TBL*)bufp)->datp;
        cpk  = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind;
        /* Check signal type */
        if( (cpk & CMD_SYS_CHK_CPRID) == CMD_NUM0)
        {
        	/* 3Gの場合 */
            evt    = EC_REC_C03_EVT_NON;
            cpr_no = D_REC_CPRINO_NON;
            break;
        }
        /* CPRI信号種別で分岐 */
        switch (cpk & CMD_SYS_MSK_CPRID) {
        /* TRX設定要求 */
        case CMD_CPRID_TRXSETREQ:
            /*===============================*/
            /* (05)RECからのTRX設定要求受信 */
            /*===============================*/
            evt    = EC_REC_C03_EVT_L3_TXS_REQ_RSP;
            cpr_no = ((CMT_TSKIF_CPRIRCV_TRXSETREQ*)datp)->
                     cprircv_inf.link_num;
            crr_no = ((CMT_TSKIF_CPRIRCV_TRXSETREQ*)datp)->
                     cpridat_trxsetreq.carno;
            break;

        /* TRX解放要求 */
        case CMD_CPRID_TRXRELREQ:
            /*===============================*/
            /* (06)RECからのTRX解放要求受信 */
            /*===============================*/
            evt    = EC_REC_C03_EVT_L3_TXR_REQ_RSP;
            cpr_no = ((CMT_TSKIF_CPRIRCV_TRXRELREQ*)datp)->
                     cprircv_inf.link_num;
            crr_no = ((CMT_TSKIF_CPRIRCV_TRXRELREQ*)datp)->
                     cpridat_trxrelreq.carno;
            break;

        /* 無変調キャリアON/OFF指定要求 */
        case CMD_CPRID_CARONOFFREQ:
            /*===============================*/
            /* (07)RECからの無変調キャリアON/OFF指定要求受信 */
            /*===============================*/
            evt    = EC_REC_C03_EVT_L3_CARONOFF_REQ_RSP;
            cpr_no = ((CMT_TSKIF_CPRIRCV_CARONOFF_S3G_REQ*)datp)->
                     cprircv_inf.link_num;
            crr_no = ((CMT_TSKIF_CPRIRCV_CARONOFF_S3G_REQ*)datp)->cpridat_caronoffreq.carno;
            break;

        /* REキャリア状態報告要求 */
        case CMD_CPRID_CARLSTSREQ:
            /*===============================*/
            /* (09)REキャリア状態報告応答 */
            /*===============================*/
            evt    = EC_REC_C03_EVT_IT_CARLSTS_REQ;
            cpr_no = ((CMT_TSKIF_CPRISND_CARLSTSREQ*)datp)->
                     cprisnd_inf.link_num;
            crr_no = 1;     // Tri.hn TODO Carrier num must not be 0 because pf/re/rec/src/f_trp_rec_Fc03_r.c:89. But the carrier status request does not have field 'crr_no'
            break;
            
        /* その他 */
        default:
            /*==============*/
            /* イベントなし */
            /*==============*/
            evt    = EC_REC_C03_EVT_NON;
            cpr_no = D_REC_CPRINO_NON;
            crr_no = D_REC_CARNO_NON;
            break;
        }
        break;

    /* その他 */
    default:
        /*==============*/
        /* イベントなし */
        /*==============*/
        evt    = EC_REC_C03_EVT_NON;
        cpr_no = D_REC_CPRINO_NON;
        crr_no = D_REC_CARNO_NON;
        break;
    }

    *cpr_nop = cpr_no;
    *crr_nop = crr_no;
    return evt;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
