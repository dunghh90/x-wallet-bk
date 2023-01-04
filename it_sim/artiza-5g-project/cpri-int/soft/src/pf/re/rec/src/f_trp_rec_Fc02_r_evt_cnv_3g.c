/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_r_evt_cnv_3g.c
 * @brief  イベント変換
 * @date   2007/03/20 FJT)Nagasima create.
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"
#include "rrhApi_Cpri.h"
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
 * @return イベント
 * @Bug_No M-RRU-ZSYS-01770
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2009/01/17 FFCS)Shihzh modify for BI-TBD-557
 * @date   2009/01/19 FFCS)Shihzh modify for eNB-004-003 BD-TBD-11
 * @date   2010/10/26 FJT)Koshida modify for eNB-004 of TS656案5対応
 * @date   2010/11/12 FJT)Tokunaga M-S3G-eNBSYS-xxxxx [TS-810項]
 * @date   2015/10/12 TDI)satou M-RRU-ZSYS-01770 問処番号No.75対応
 * @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.243,257対応
 * @date   2015/10/19 TDI)satou ハソ-QA-051 二次リセット
 * @date   2015/11/17 FPT)Tuan add case RE autoreset to convert.
 * @date   2021/01/19 M&C)DuongMX Intermediate device remove RE device composition information report response
 * @date   2021/02/09 M&C)Huan.dh Intermediate remove REPORTSTSRES for 3G.
 */
/****************************************************************************/
ET_REC_C02_EVT f_trp_rec_Fc02_r_evt_cnv_3g(                      /* イベント:R */
   VOID*   bufp,                                      /* バッファポインタ:I */
   USHORT* cpr_nop                                            /* CPRI番号:O */
) {
    UINT                   msg_id;                          /* メッセージID */
    ET_REC_C02_EVT         evt = EC_REC_C02_EVT_NON;            /* イベント */
    USHORT                 cpr_no;                              /* CPRI番号 */
    UINT  key_inf;                                        /* タイマキー情報 */
    T_REC_TIMELM           elm;                               /* タイマ要素 */
    USHORT                 cpk;                             /* CPRI信号種別 */
    USHORT                 link_sts;                          /* リンク状態 */
#ifdef D_TCM_MTSET_FLD
    USHORT                 mt_obj;
#endif
    VOID*                  datp;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /********************/
    /* メッセージID取得 */
    /********************/
    msg_id = ((CMT_TSKIF_MSGID_GET*)bufp)->head.msgid;

    /* メッセージIDで分岐 */
    switch (msg_id) {
    /* スレッド初期化要求 */
    case CMD_TSKIF_INIREQ:
        /*====================*/
        /* スレッド初期化要求 */
        /*====================*/
        evt    = EC_REC_C02_EVT_IT_THRINI_REQ;
        cpr_no = D_REC_CPRINO_ALL;
        break;

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
        if (elm.cls != EC_REC_C02_3G) {
            /*==============*/
            /* イベントなし */
            /*==============*/
            evt    = EC_REC_C02_EVT_NON;
            cpr_no = D_REC_CPRINO_NON;
            break;
        }

#ifdef DEBUG_TRACE_T
		/*	自動受信フラグOFFの場合		*/
		if( dbgFlg[1] == 0)
		{
            /*==============*/
            /* イベントなし */
            /*==============*/
            evt    = EC_REC_C02_EVT_NON;
            cpr_no = D_REC_CPRINO_NON;
			return evt;
		}
#endif
        /* タイマ種別で分岐 */
        switch (elm.tmk) {
       
        /* REカード自律リセット待ちタイマ */
        case EC_REC_TMK_REAUTRST_WAT:
            /*======================================================*/
            /* タイムアウト発生通知(REカード自律リセット待ちタイマ) */
            /*======================================================*/
            evt    = EC_REC_C02_EVT_TO_REAUTRST_WAT;
            cpr_no = elm.cpr_no;
            break;
        
        /* Toffset値報告確認タイマ */
        case EC_REC_TMK_TOFVAL_CNF:
            /*===============================================*/
            /* タイムアウト発生通知(Toffset値報告確認タイマ) */
            /*===============================================*/
            evt    = EC_REC_C02_EVT_TO_TOFVAL_CNF;
            cpr_no = elm.cpr_no;
            break;

        /* RE装置構成情報報告確認タイマ */
        case EC_REC_TMK_REEQPINF_CNF:
            /*====================================================*/
            /* タイムアウト発生通知(RE装置構成情報報告確認タイマ) */
            /*====================================================*/
            evt    = EC_REC_C02_EVT_TO_REEQPINF_CNF;
            cpr_no = elm.cpr_no;
            break;

        /* REスロット情報報告確認タイマ */
        case EC_REC_TMK_RESLTINF_CNF:
            /*====================================================*/
            /* タイムアウト発生通知(REスロット情報報告確認タイマ) */
            /*====================================================*/
            evt    = EC_REC_C02_EVT_TO_RESLTINF_CNF;
            cpr_no = elm.cpr_no;
            break;

        /* RE状態報告確認タイマ */
        case EC_REC_TMK_RESTS_CNF:
            /*====================================================*/
            /* タイムアウト発生通知(RE状態報告確認タイマ)         */
            /*====================================================*/
            evt    = EC_REC_C02_EVT_TO_RESTS_CNF;
            cpr_no = elm.cpr_no;
            break;

        /* RE PORT状態報告確認タイマ */
        case EC_REC_TMK_REPRTSTS_CNF:
            /*====================================================*/
            /* タイムアウト発生通知(RE PORT状態報告確認タイマ)    */
            /*====================================================*/
            evt    = EC_REC_C02_EVT_TO_REPRTSTS_CNF;
            cpr_no = elm.cpr_no;
            break;

        /* REカード状態報告(セクタ部)確認タイマ */
        case EC_REC_TMK_RECRDSSC_CNF:
            /*============================================================*/
            /* タイムアウト発生通知(REカード状態報告(セクタ部)確認タイマ) */
            /*============================================================*/
            evt    = EC_REC_C02_EVT_TO_RECRDSSC_CNF;
            cpr_no = elm.cpr_no;
            break;

        /* REキャリア状態報告確認タイマ */
        case EC_REC_TMK_RECRRSTS_CNF:
            /*==========================================================*/
            /* タイムアウト発生通知(REキャリア状態報告確認タイマ)       */
            /*==========================================================*/
            evt    = EC_REC_C02_EVT_TO_RECRRSTS_CNF;
            cpr_no = elm.cpr_no;
            break;

        /* REリセット確認タイマ */
        case EC_REC_TMK_RERST_CNF:
            /*============================================*/
            /* タイムアウト発生通知(REリセット確認タイマ) */
            /*============================================*/
            evt    = EC_REC_C02_EVT_TO_RERST_CNF;
            cpr_no = elm.cpr_no;
            break;

        /* RE強制リセット確認タイマ */
        case EC_REC_TMK_REFRCRST_CNF:
            /*================================================*/
            /* タイムアウト発生通知(RE強制リセット確認タイマ) */
            /*================================================*/
            evt    = EC_REC_C02_EVT_TO_REFRCRST_CNF;
            cpr_no = elm.cpr_no;
            break;

        /* RE状態報告送信間隔タイマ */
        case EC_REC_TMK_RESTS_INT:
            /*========================================================*/
            /* タイムアウト発生通知(RE状態報告送信間隔タイマ)         */
            /*========================================================*/
            f_trp_rec_Fc02_3g_evt = CMD_ON;
            evt    = EC_REC_C02_EVT_TO_RESTS_INT;
            cpr_no = elm.cpr_no;
            break;

        /* REカード状態報告(セクタ部)送信間隔タイマ */
        case EC_REC_TMK_RECRDSSC_INT:
            /*==============================================================*/
            /* タイムアウト発生通知(REカード状態報告(セクタ部)送信間隔タイマ*/
            /*==============================================================*/
            f_trp_rec_Fc02_3g_evt = CMD_ON;
            evt    = EC_REC_C02_EVT_TO_RECRDSSC_INT;
            cpr_no = elm.cpr_no;
            break;

        /* REキャリア状態報告送信間隔タイマ */
        case EC_REC_TMK_RECRRSTS_INT:
            /*==============================================================*/
            /* タイムアウト発生通知(REEキャリア状態報告送信間隔タイマ)      */
            /*==============================================================*/
            f_trp_rec_Fc02_3g_evt = CMD_ON;
            evt    = EC_REC_C02_EVT_TO_RECRRSTS_INT;
            cpr_no = elm.cpr_no;
            break;

        /* 二次リセット待ちタイマ */
        case EC_REC_TMK_2NDRST_WAT:
            /*==============================================*/
            /* タイムアウト発生通知(二次リセット待ちタイマ) */
            /*==============================================*/
            evt    = EC_REC_C02_EVT_TO_2NDRST_WAT;
            cpr_no = elm.cpr_no;
            break;

        /* Active切替指示送信待ちタイマ */
        case EC_REC_TMK_ACTCHG_WAT:
            /*====================================================*/
            /* タイムアウト発生通知(Active切替指示送信待ちタイマ) */
            /*====================================================*/
            evt    = EC_REC_C02_EVT_TO_ACTCHG_WAT;
            cpr_no = elm.cpr_no;
            break;

        /* MTアドレス設定確認タイマ */
        case EC_REC_TMK_MT_ADD_SET:
            /*============================================*/
            /*============================================*/
            evt    = EC_REC_C02_EVT_TO_MTADDSET;
            cpr_no = elm.cpr_no;
            break;
        /* その他 */
        default:
            /*==============*/
            /* イベントなし */
            /*==============*/
            evt    = EC_REC_C02_EVT_NON;
            cpr_no = D_REC_CPRINO_NON;
            break;
        }
        break;

    /* CPRI信号受信通知 */
    case D_API_MSGID_L2_CPRIMSG_RCV_NTC:
        /* CPRI信号種別取得 */
        datp   = &((T_REC_MSGRCV_TBL*)bufp)->datp;
        cpk    = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind;
        /* Check signal type */
        if( (cpk & CMD_SYS_CHK_CPRID) == CMD_NUM1)
        {
        	/* S3Gの場合 */
            evt    = EC_REC_C02_EVT_NON;
            cpr_no = D_REC_CPRINO_NON;
            break;
        }
        cpr_no = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->cprircv_inf.link_num;
        /* CPRI信号種別で分岐 */
        switch (cpk & CMD_SYS_MSK_CPRID) {
        /* Toffset値報告応答 */
        case CMD_CPRID_TOFFSETINFRES:
            /*=====================================*/
            /* CPRI信号受信通知(Toffset値報告応答) */
            /*=====================================*/
            evt    = EC_REC_C02_EVT_L3_TOFVAL_RSP;
            cpr_no = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->
                     cprircv_inf.link_num;
            break;

        /* 2021/01/19 M&C)DuongMX Item No.20 - RE Device Configuration Information Monitoring (start delete) */
        /* RE装置構成情報報告応答 */
        // case CMD_CPRID_EQPSTRRES:
        //     /*=========================================*/
        //     /* CPRI信号受信通知(RE装置構成情報報告応答) */
        //     /*=========================================*/
        //     evt    = EC_REC_C02_EVT_L3_REEQPINF_RSP;
        //     cpr_no = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->
        //              cprircv_inf.link_num;
        //     break;
        /* 2021/01/19 M&C)DuongMX Item No.20 - RE Device Configuration Information Monitoring (end delete) */

        /* REスロット情報報告応答 */
        case CMD_CPRID_SLOTINFRES:
            /*=========================================*/
            /* CPRI信号受信通知(REスロット情報報告応答) */
            /*=========================================*/
            evt    = EC_REC_C02_EVT_L3_RESLTINF_RSP;
            cpr_no = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->
                     cprircv_inf.link_num;
            break;

        /* RE状態報告応答 */
        case CMD_CPRID_RESTSRES:
            /*=========================================*/
            /* CPRI信号受信通知(RE状態報告応答)        */
            /*=========================================*/
            f_trp_rec_Fc02_3g_evt = CMD_ON;
            evt    = EC_REC_C02_EVT_L3_RESTS_RSP;
            cpr_no = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->
                     cprircv_inf.link_num;
            break;

        /* RE PORT状態報告応答 */
        // case CMD_CPRID_REPORTSTSRES:
        //     /*=========================================*/
        //     /* CPRI信号受信通知(RE PORT状態報告応答)   */
        //     /*=========================================*/
        //     evt    = EC_REC_C02_EVT_L3_REPRTSTS_RSP;
        //     cpr_no = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->
        //              cprircv_inf.link_num;
        //     break;

        /* REカード状態報告応答 */
        case CMD_CPRID_CARDSTSRES:
            f_trp_rec_Fc02_3g_evt = CMD_ON;
            evt    = EC_REC_C02_EVT_L3_RECRDSSC_RSP;
            cpr_no = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->
                     cprircv_inf.link_num;
            break;

        /* REキャリア状態報告応答 */
        case CMD_CPRID_CARLSTSRES:
            /*============================================*/
            /* CPRI信号受信通知(REキャリア状態報告応答)   */
            /*============================================*/
            f_trp_rec_Fc02_3g_evt = CMD_ON;
        	f_trp_rec_Fc02_carr_3g_evt = CMD_ON;
            evt    = EC_REC_C02_EVT_L3_RECRRSTS_RSP;
            cpr_no = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->
                     cprircv_inf.link_num;
            break;
        /* MTアドレス設定応答 */
	    case CMD_CPRID_MTADDSETRES:
            cpr_no = ((CMT_TSKIF_CPRIRCV_MTADDSETRES*)bufp)->cprircv_inf.link_num;
            /* MTアドレス設定開始要因設定: REC主導*/
            if( D_MT_ADD_SET_FACTOR_REC == f_trp_rec_Fc02_t_tc_mtsetffactor_get_3g(cpr_no) ) 
            {
                /*==============*/
                /* イベントなし */
                /*==============*/
                evt    = EC_REC_C02_EVT_NON;
                cpr_no = D_REC_CPRINO_NON;
                break;
            }
            /*============================*/
            /* MTアドレス設定(FLD-MT)応答 */
            /*============================*/
            evt    = EC_REC_C02_EVT_IT_MTSETF_RSP;
            break;

        /* その他 */
        default:
            /*==============*/
            /* イベントなし */
            /*==============*/
            evt    = EC_REC_C02_EVT_NON;
            cpr_no = D_REC_CPRINO_NON;
            break;
        }
        break;

    /* CPRIリンク状態変化通知 */
    case CMD_TSKIF_CPRSTACHGNTC:
        /* CPRIリンク状態取得 */
        link_sts = ((CMT_TSKIF_CPRSTACHGNTC*)bufp)->link_sts;
        /* CPRIリンク状態で分岐 */
        switch(link_sts) {
        /* StateB */
        case D_RRH_CPRISTAT_B:
            /*================================*/
            /* CPRIリンク状態変化通知(StateB) */
            /*================================*/
            evt    = EC_REC_C02_EVT_IT_CPRSTSNTC_B;
            cpr_no = ((CMT_TSKIF_CPRSTACHGNTC*)bufp)->cpri_no;
            break;

        /* StateE */
        case D_RRH_CPRISTAT_E:
            /*================================*/
            /* CPRIリンク状態変化通知(StateE) */
            /*================================*/
            evt    = EC_REC_C02_EVT_IT_CPRSTSNTC_E;
            cpr_no = ((CMT_TSKIF_CPRSTACHGNTC*)bufp)->cpri_no;
            break;

        /* StateF */
        case D_RRH_CPRISTAT_F:
            /*================================*/
            /* CPRIリンク状態変化通知(StateF) */
            /*================================*/
            evt    = EC_REC_C02_EVT_IT_CPRSTSNTC_F;
            cpr_no = ((CMT_TSKIF_CPRSTACHGNTC*)bufp)->cpri_no;
            break;

        /* その他 */
        default:
            /*==============*/
            /* イベントなし */
            /*==============*/
            evt    = EC_REC_C02_EVT_NON;
            cpr_no = D_REC_CPRINO_NON;
            break;
        }
        break;
    /* 0x4005 RE Startup Request */
    case CMD_TSKIF_RE_STARTUP_REQ:
        /* Check signal type */
        cpk = ((CMT_TSKIF_RESTR_REQ*)bufp)->system_type;
        if( (cpk & CMD_SYS_CHK_CPRID) == CMD_NUM1) 
        {
            /* LTEの場合 */
            evt    = EC_REC_C02_EVT_NON;
            cpr_no = D_REC_CPRINO_NON;
            break;
        }
        evt    = EC_REC_C02_EVT_IT_CPRSTSNTC_L;
        cpr_no = ((CMT_TSKIF_RESTR_REQ*)bufp)->link_num;
        break;
    /* CPRIリンク断通知 */
    case CMD_TSKIF_CPRDSCNTC:
        /* CPRIリンク状態取得 */
        link_sts = ((CMT_TSKIF_CPRDSCNTC*)bufp)->link_sts;
        /* CPRIリンク状態で分岐 */
        if( D_RRH_CPRISTAT_F != D_RRH_CPRISTAT_F )
        {
        	/*==============================*/
            /* CPRIリンク断通知(StateF未満) */
            /*==============================*/
            evt    = EC_REC_C02_EVT_IT_CPRDSCNTC_E;
            cpr_no = ((CMT_TSKIF_CPRDSCNTC*)bufp)->cpri_no;
        }
        else
        {
            /*==============*/
            /* イベントなし */
            /*==============*/
            evt    = EC_REC_C02_EVT_NON;
            cpr_no = D_REC_CPRINO_NON;
        }
        break;
	/* (0x4007)RE Disconnecion Request */
    case CMD_TSKIF_RE_DISCONNECT_REQ:
        if (E_RRH_RAYER_KIND_1_2 == ((CMT_TSKIF_REDIS_REQ*)bufp)->layer_kind) {
            /* Layer1/2断の場合、3G/LTEともにリンク断の処理を行う */
            evt    = EC_REC_C02_EVT_IT_CPRDSCNTC_E;
            cpr_no = ((CMT_TSKIF_REDIS_REQ*)bufp)->link_num;
        } else {
            /* Layer3断(3G)の場合のみリンク断の処理を行う。S3Gの断はイベントなし */
            if (CMD_SYS_3G == ((CMT_TSKIF_REDIS_REQ*)bufp)->system_type) {
                evt    = EC_REC_C02_EVT_IT_CPRDSCNTC_F;
                cpr_no = ((CMT_TSKIF_REDIS_REQ*)bufp)->link_num;
            } else {
                evt    = EC_REC_C02_EVT_NON;
                cpr_no = D_REC_CPRINO_NON;
            }
        }
        break;

    /* 二次リセット要求 */
    case CMD_TSKIF_2NDRST_REQ:
        /*==================*/
        /* 二次リセット要求 */
        /*==================*/
        evt    = EC_REC_C02_EVT_IT_2NDRST_REQ;
        cpr_no = ((CMT_TSKIF_CPRINOGET*)bufp)->cpri_no;
        break;

    /* 二次リセットプロシージャ実行指示 */
    case CMD_TSKIF_2NDRSTPRCIND_3G:
        /*==================================*/
        /* 二次リセットプロシージャ実行指示 */
        /*==================================*/
        evt    = EC_REC_C02_EVT_IT_2NDPRC_IND;
        cpr_no = D_REC_CPRINO_ALL;
        break;

    /* RE時刻設定プロシージャ実行指示 */
    case CMD_TSKIF_RETIMSETPRCIND_3G:
        /*================================*/
        /* RE時刻設定プロシージャ実行指示 */
        /*================================*/
        evt    = EC_REC_C02_EVT_IT_RETIMSETPRC_IND;
        cpr_no = ((CMT_TSKIF_RETIMSETPRCIND*)bufp)->cpri_no;
        break;

    /* REシステムパラメータ更新プロシージャ実行指示 */
    case CMD_TSKIF_REPRMPRCIND_3G:
        /*==============================================*/
        /* REシステムパラメータ更新プロシージャ実行指示 */
        /*==============================================*/
        evt    = EC_REC_C02_EVT_IT_REPRMPRC_IND;
        cpr_no = ((CMT_TSKIF_REPRMPRCIND*)bufp)->cpri_no;
        break;

    /* REファイル更新プロシージャ実行指示 */
    case CMD_TSKIF_REFILPRCIND_3G:
        /*====================================*/
        /* REファイル更新プロシージャ実行指示 */
        /*====================================*/
        evt    = EC_REC_C02_EVT_IT_REFILPRC_IND;
        cpr_no = ((CMT_TSKIF_REFILPRCIND*)bufp)->cpri_no;
        break;

    /* Toffset値報告プロシージャ実行指示 */
    case CMD_TSKIF_TOFVALPRCIND_3G:
        /*===================================*/
        /* Toffset値報告プロシージャ実行指示 */
        /*===================================*/
        evt    = EC_REC_C02_EVT_IT_TOFVALPRC_IND;
        cpr_no = ((CMT_TSKIF_TOFVALPRCIND*)bufp)->cpri_no;
        break;

    /* Active設定プロシージャ実行指示 */
    case CMD_TSKIF_ACTSETPRCIND_3G:
        /*================================*/
        /* Active設定プロシージャ実行指示 */
        /*================================*/
        evt    = EC_REC_C02_EVT_IT_ACTSETPRC_IND;
        cpr_no = ((CMT_TSKIF_ACTSETPRCIND*)bufp)->cpri_no;
        break;

    /* RE装置構成情報報告プロシージャ実行指示 */
    case CMD_TSKIF_REEQPINFPRCIND_3G:
        /*========================================*/
        /* RE装置構成情報報告プロシージャ実行指示 */
        /*========================================*/
        evt    = EC_REC_C02_EVT_IT_REEQPINFPRC_IND;
        cpr_no = ((CMT_TSKIF_REEQPINFPRCIND*)bufp)->cpri_no;
        break;

    /* REスロット情報報告プロシージャ実行指示 */
    case CMD_TSKIF_RESLTINFPRCIND_3G:
        /*========================================*/
        /* REスロット情報報告プロシージャ実行指示 */
        /*========================================*/
        evt    = EC_REC_C02_EVT_IT_RESLTINFPRC_IND;
        cpr_no = ((CMT_TSKIF_RESLTINFPRCIND*)bufp)->cpri_no;
        break;

    /* RE状態報告確プロシージャ実行指示 */
    case CMD_TSKIF_RESTSPRCIND_3G:
        /*==================================*/
        /* RE状態報告確プロシージャ実行指示 */
        /*==================================*/
        evt    = EC_REC_C02_EVT_IT_RESTSPRC_IND;
        cpr_no = ((CMT_TSKIF_RESTSPRCIND*)bufp)->cpri_no;
        break;

    /* RE PORT状態報告プロシージャ実行指示 */
    case CMD_TSKIF_REPRTSTSPRCIND_3G:
        /*=====================================*/
        /* RE PORT状態報告プロシージャ実行指示 */
        /*=====================================*/
        evt    = EC_REC_C02_EVT_IT_REPRTPRC_IND;
        cpr_no = ((CMT_TSKIF_REPRTSTSPRCIND*)bufp)->cpri_no;
        break;

    /* REカード状態報告(セクタ部)プロシージャ実行指示 */
    case CMD_TSKIF_RECRDSSCPRCIND_3G:
        /*================================================*/
        /* REカード状態報告(セクタ部)プロシージャ実行指示 */
        /*================================================*/
        evt    = EC_REC_C02_EVT_IT_RECRDSSCPRC_IND;
        cpr_no = ((CMT_TSKIF_RECRDSSCPRCIND*)bufp)->cpri_no;
        break;

    /* REキャリア状態報告プロシージャ実行指示 */
    case CMD_TSKIF_RECRRSTSPRCIND_3G:
        /*========================================*/
        /* REキャリア状態報告プロシージャ実行指示 */
        /*========================================*/
        evt    = EC_REC_C02_EVT_IT_RECRRSTSPRC_IND;
        cpr_no = ((CMT_TSKIF_RECRRSTSPRCIND*)bufp)->cpri_no;
        break;

    /* MTアドレス設定(FLD-MT)プロシージャ実行指示 */
    case CMD_TSKIF_MTSETFPRCIND_3G:
        /*============================================*/
        /* MTアドレス設定(FLD-MT)プロシージャ実行指示 */
        /*============================================*/
        evt    = EC_REC_C02_EVT_IT_MTSETFPRC_IND;
        cpr_no = ((CMT_TSKIF_MTSETFPRCIND*)bufp)->cpri_no;
        break;

    /* MTアドレス設定(SV-MT)プロシージャ実行指示 */
    case CMD_TSKIF_MTSETSPRCIND_3G:
        /*===========================================*/
        /* MTアドレス設定(SV-MT)プロシージャ実行指示 */
        /*===========================================*/
        evt    = EC_REC_C02_EVT_IT_MTSETSPRC_IND;
        cpr_no = ((CMT_TSKIF_MTSETSPRCIND*)bufp)->cpri_no;
        break;

    /* RE起動完了プロシージャ実行指示 */
    case CMD_TSKIF_RESCMPPRCIND_3G:
        /*================================*/
        /* RE起動完了プロシージャ実行指示 */
        /*================================*/
        evt    = EC_REC_C02_EVT_IT_RESCMPPRC_IND;
        cpr_no = ((CMT_TSKIF_RESCMPPRCIND*)bufp)->cpri_no;
        break;

    /* REリセットプロシージャ実行指示 */
    case CMD_TSKIF_RERSTPRCIND_3G:
        /*================================*/
        /* REリセットプロシージャ実行指示 */
        /*======v=========================*/
        evt    = EC_REC_C02_EVT_IT_RERSTPRC_IND;
        cpr_no = ((CMT_TSKIF_RERSTPRCIND*)bufp)->cpri_no;
        break;

    /* RE強制リセット設置プロシージャ実行指示 */
    case CMD_TSKIF_REFRCRSTSETPRCIND_3G:
        /*========================================*/
        /* RE強制リセット設置プロシージャ実行指示 */
        /*========================================*/
        evt    = EC_REC_C02_EVT_IT_REFRCRSTPRC_IND;
        cpr_no = ((CMT_TSKIF_REFRCRSTSETPRCIND*)bufp)->cpri_no;
        break;

    /* RE強制リセットプロシージャ実行指示 */
    case CMD_TSKIF_REFRCRSTPRCIND_3G:
        /*====================================*/
        /* RE強制リセットプロシージャ実行指示 */
        /*====================================*/
        evt    = EC_REC_C02_EVT_IT_REFRCRSTPRC_IND;
        cpr_no = ((CMT_TSKIF_REFRCRSTPRCIND*)bufp)->cpri_no;
        break;

    /* 強制停止プロシージャ実行指示 */
    case CMD_TSKIF_FRCSTPPRCIND_3G:
        /*==============================*/
        /* 強制停止プロシージャ実行指示 */
        /*==============================*/
        evt    = EC_REC_C02_EVT_IT_FRCSTPPRC_IND;
        cpr_no = ((CMT_TSKIF_FRCSTPPRCIND*)bufp)->cpri_no;
        break;

    /* REシステムパラメータ更新応答 */
    case CMD_TSKIF_RESYSPARA_UPDATE_RSP:
        if( CMD_SYS_LTE == ((CMT_TSKIF_RESYSNEWRES*)bufp)->systype )
        {
            /* LTEの場合 */
            evt    = EC_REC_C02_EVT_NON;
            cpr_no = D_REC_CPRINO_NON;
            break;
        }
        /*==============================*/
        /* REシステムパラメータ更新応答 */
        /*==============================*/
        evt    = EC_REC_C02_EVT_IT_REPRM_RSP;
        cpr_no = ((CMT_TSKIF_RESYSNEWRES*)bufp)->cpri_no;
        break;

    /* REファイル更新応答 */
    case CMD_TSKIF_REFIRM_DL_RSP:
        if( CMD_SYS_LTE == ((CMT_TSKIF_REFILNEWRES*)bufp)->dummy )
        {
            /* LTEの場合 */
            evt    = EC_REC_C02_EVT_NON;
            cpr_no = D_REC_CPRINO_NON;
            break;
        }
        /*====================*/
        /* REファイル更新応答 */
        /*====================*/
        evt    = EC_REC_C02_EVT_IT_REFIL_RSP;
        cpr_no = ((CMT_TSKIF_REFILNEWRES*)bufp)->cpri_no;
        break;
        

    /* MTアドレス設定応答 */
//    case CMD_TSKIF_MTASTRES:
//        mt_obj = ((CMT_TSKIF_MTASTRSP*)bufp)->mt_object;
//        cpr_no = ((CMT_TSKIF_MTASTRSP*)bufp)->link_num;
//        switch (mt_obj) {
//        case CMD_FLD_MT:
//            /*============================*/
//            /* MTアドレス設定(FLD-MT)応答 */
//            /*============================*/
//            evt    = EC_REC_C02_EVT_IT_MTSETF_RSP;
//            break;
//        case CMD_SV_MT:
//            /*===========================*/
//            /* MTアドレス設定(SV-MT)応答 */
//            /*===========================*/
//            evt    = EC_REC_C02_EVT_IT_MTSETS_RSP;
//            break;
//        default:
//            evt    = EC_REC_C02_EVT_NON;
//            break;
//        }
//        break;

//
//    /* N/E切替開始要求 */
//    case CMD_TSKIF_NECHGSRTREQ:
//        /*=================*/
//        /* N/E切替開始要求 */
//        /*=================*/
//        evt    = EC_REC_C02_EVT_IT_NECHGSTA_REQ;
//        cpr_no = D_REC_CPRINO_ALL;
//        break;
//
//    /* N/E切替完了通知 */
//    case CMD_TSKIF_NECHGCMPNTC:
//        /*=================*/
//        /* N/E切替完了通知 */
//        /*=================*/
//        evt    = EC_REC_C02_EVT_IT_NECHGCMP_NTC;
//        cpr_no = D_REC_CPRINO_ALL;
//        break;
//
//    /* データ引継ぎ開始通知 */
//    case CMD_TSKIF_NEDATSRTNTC:
//        /*======================*/
//        /* データ引継ぎ開始通知 */
//        /*======================*/
//        evt    = EC_REC_C02_EVT_IT_NEDATSTA_REQ;
//        cpr_no = D_REC_CPRINO_ALL;
//        break;
//
//    /* データ引継ぎ完了通知 */
//    case CMD_TSKIF_NEDATCMPNTC:
//        /*======================*/
//        /* データ引継ぎ完了通知 */
//        /*======================*/
//        evt    = EC_REC_C02_EVT_IT_NEDATCMP_NTC;
//        cpr_no = D_REC_CPRINO_ALL;
//        break;
//
//    /* CPRI link Recovery完了通知  */
//    case CMD_TSKIF_CPRIRCVCMPNTC:
//        /*============================*/
//        /* CPRI link Recovery完了通知 */
//        /*============================*/
//        evt    = EC_REC_C02_EVT_IT_LNKRCV_NTC;
//        cpr_no = D_REC_CPRINO_ALL;
//        break;
//
//    /* ファイルデータ送信許可要求  */
//    case CMD_TSKIF_REDLPERMITREQ:
//        /*============================*/
//        /* ファイルデータ送信許可要求 */
//        /*============================*/
//        evt    = EC_REC_C02_EVT_IT_DLPMIT_REQ;
//        cpr_no = ((CMT_TSKIF_REDLPERMITREQ*)bufp)->cpri_no;
//        break;
//
//    /* MTデータ送信許可要求  */
//    case CMD_TSKIF_MTDATAPERMITREQ:
//        /*======================*/
//        /* MTデータ送信許可要求 */
//        /*======================*/
//        evt    = EC_REC_C02_EVT_IT_MTPMIT_REQ;
//        cpr_no = ((CMT_TSKIF_MTDATAPERMITREQ*)bufp)->cpri_no;
//        break;
//
//    /* データリンク解放表示  */
//    case CMD_TSKIF_RELIND:
//        /*======================*/
//        /* データリンク解放表示 */
//        /*======================*/
//        evt    = EC_REC_C02_EVT_IT_L2RELIND;
//        cpr_no = ((CMT_TSKIF_L2RELIND*)bufp)->link_num;
//        break;

    /* その他 */
    default:
        /*==============*/
        /* イベントなし */
        /*==============*/
        evt    = EC_REC_C02_EVT_NON;
        cpr_no = D_REC_CPRINO_NON;
        break;
    }

    *cpr_nop = cpr_no;
    return evt;

}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
