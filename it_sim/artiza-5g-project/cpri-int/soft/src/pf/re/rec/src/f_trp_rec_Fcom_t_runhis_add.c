/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_runhis_add.c
 * @brief  RUNHIS追加
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2007/06/07 FJT)Nagasima CR-00019-001
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RUNHIS追加
 * @note   RUNHISを追加する
 * @param  rhk  [in]  RUNHIS種別
 * @param  bufp [in]  バッファポインタ
 * @return 終了コード
 * @retval 0:書き込み有
 * @retval 1:書き込み無
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2007/06/07 FJT)Nagasima CR-00019-001
 * @date   2009/02/19 FFCS)Shihzh M-S3G-eNBPF-01316.
 * @date   2021/01/14 M&C)Huan.dh intermmediate device add RE control response handle
 * @date   2021/02/04 M&C)Huan.dh add CARONOFF case
 */
/****************************************************************************/
UINT f_trp_rec_Fcom_t_runhis_add(                           /* 終了コード:R */
    UCHAR rhk,                                              /* RUNHIS種別:I */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    UINT               msgid;                               /* メッセージID */
    UINT               fb_no;                                     /* FB番号 */
    USHORT             sig_kind;                                /* 信号種別 */
    UCHAR              re_kind;                                  /* RE Kind */
    UCHAR              cpr_no;                                  /* CPRI番号 */
    UCHAR              soc_no;                /* スロット番号orキャリア番号 */
    UCHAR              ret_code;                              /* 結果コード */
    USHORT             cpr_no2;                         /* CPRI番号(分岐用) */
    UINT               key_inf;                        /* タイマキー情報 */
    T_REC_TIMELM       elm;                                   /* タイマ要素 */
    UINT               wrt_idx;                             /* 書き込み位置 */
    UINT               log_wrt_idx;                 /* 強化ログ書き込み位置 */
    VOID*              bodp;                        /* ボディデータポインタ */
    T_REC_BCDTIM       bcdtim;                                   /* BCD時刻 */
#if 0
    CHAR               tim_as[CMD_NUM32];                    /* 時刻(ASCII) */
    CHAR               msg_as[CMD_NUM16];            /* メッセージID(ASCII) */
    CHAR               sig_as[CMD_NUM8];                 /* 信号種別(ASCII) */
    CHAR               rek_as[CMD_NUM8];                 /* RE Kind(ASSCII) */
    CHAR               cpr_as[CMD_NUM8];                 /* CPRI番号(ASCII) */
    CHAR               soc_as[CMD_NUM8];
                                       /* スロット番号orキャリア番号(ASCII) */
    CHAR               ret_as[CMD_NUM8];               /* 結果コード(ASCII) */
    CHAR               fbn_as[CMD_NUM16];                  /* FB番号(ASCII) */
#endif
    VOID*              datp;
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* RUNHISを残さないイベントを篩いにかける */
    /* メッセージIDで分岐 */
    msgid = ((CMT_TSKIF_MSGID_GET*)bufp)->head.msgid;
    switch (msgid) {
    /* CPRI信号 */
    case CMD_TSKIF_CPRIRCVNTC:
    case CMD_TSKIF_CPRISNDIND:
        //datp = ((T_MSG*)bufp)->data;
    	datp = bufp;
        /* CPRI信号種別で分岐 */
        sig_kind = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind;
        cpr_no2  = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->cprircv_inf.link_num;
        switch (sig_kind) {
        /* RE状態取得用 */
        case CMD_CPRID_TOFFSETINFREQ:
        case CMD_CPRID_TOFFSETINFRES:
        case CMD_CPRID_EQPSTRREQ:
        case CMD_CPRID_EQPSTRRES:
        case CMD_CPRID_RESTSREQ:
        case CMD_CPRID_RESTSRES:
        case CMD_CPRID_REPORTSTSREQ:
        case CMD_CPRID_REPORTSTSRES:
        case CMD_CPRID_CARDSTSREQ:
        case CMD_CPRID_CARDSTSRES:
        case CMD_CPRID_CARLSTSREQ:
        case CMD_CPRID_CARLSTSRES:
            /* C02状態番号が運用中の場合 */
            if (f_trp_rec_Fcom_t_c02stn_get(cpr_no2) == EC_REC_C02_STN_USE) {
                /* フラグOFFなら書き込み無 */
                if (f_trp_rec_Wcom_runhis_tbl.col_flg.use_rests == CMD_OFF) {
#ifndef DEBUG_TRACE
                    return CMD_NG;
#endif
                }
            }
            break;
        default:
            break;
        }
        break;

    /* タイムアウト発生通知 */
    case CMD_TSKIF_TIMOUTNTC:
    	if(((T_RRH_TIMERTO_NTC*)bufp)->header.uiSignalkind == EC_REC_TIMD_10MSEC_INT)
    	{
			return CMD_NG;
		}
        /* タイマ種別で分岐 */
        key_inf = ((T_RRH_TIMERTO_NTC*)bufp)->data.userkey;
        f_trp_rec_Fcom_t_key_elm_cnv(key_inf, &elm);
        switch (elm.tmk) {
        /* 送信間隔タイマ */
        case EC_REC_TMK_TOFVAL_INT:
        case EC_REC_TMK_REEQPINF_INT:
        case EC_REC_TMK_RESTS_INT:
        case EC_REC_TMK_REPRTSTS_INT:
        case EC_REC_TMK_RECRDSSC_INT:
        case EC_REC_TMK_RECRDSCM_INT:
        case EC_REC_TMK_RECRRSTS_INT:
        case EC_REC_TMK_ACTCHG_WAT:
        case EC_REC_TMK_T14RTY_WAT:
            /* フラグOFFなら書き込み無 */
            if (f_trp_rec_Wcom_runhis_tbl.col_flg.int_to == CMD_OFF) {
#ifndef DEBUG_TRACE
                return CMD_NG;
#endif
            }
            break;
        default:
            break;
        }
        break;
    /* defaultの場合はログをとらない */
    default:
    	return CMD_OK;
    }

    /**********************/
    /* 収集データ初期設定 */
    /**********************/
    msgid = ((CMT_TSKIF_MSGID_GET*)bufp)->head.msgid;       /* メッセージID */
    if (rhk == D_REC_RUNHIS_RHK_RCV) {
        fb_no = ((CMT_TSKIF_MSGID_GET*)bufp)->head.srcFbNo;       /* FB番号 */
    }
    else {
        fb_no = ((CMT_TSKIF_MSGID_GET*)bufp)->head.dstFbNo;       /* FB番号 */
    }
    sig_kind = CMD_ALLF_SHORT;                                  /* 信号種別 */
    re_kind  = CMD_ALLF_CHAR;                                    /* RE Kind */
    cpr_no   = CMD_ALLF_CHAR;                                   /* CPRI番号 */
    soc_no   = CMD_ALLF_CHAR;                 /* スロット番号orキャリア番号 */
    ret_code = CMD_ALLF_CHAR;                                 /* 結果コード */

    /********************************/
    /* 収集データ設定(ヘッダデータ) */
    /********************************/
    /* メッセージIDで分岐 */
    switch (msgid) {
    /**************/
    /* スレッド間 */
    /**************/
//    /* スレッド起動完了通知 */
//    case CMD_TSKIF_THRSUPCMP_NTC:
//        ret_code = ((CMT_TSKIF_THRSUPCMP_NTC*)bufp)->result;
//        break;
//    /* スレッド初期化応答 */
//    case CMD_TSKIF_THRINI_RSP:
//        ret_code = ((CMT_TSKIF_THRINI_RSP*)bufp)->result;
//        break;
    /* タイムアウト発生通知 */
    case CMD_TSKIF_TIMOUTNTC:
        key_inf = ((T_RRH_TIMERTO_NTC*)bufp)->data.userkey;
        f_trp_rec_Fcom_t_key_elm_cnv(key_inf, &elm);
        D_REC_SETBYTE(sig_kind, CMD_NUM1, elm.cls);
        D_REC_SETBYTE(sig_kind, CMD_NUM0, elm.tmk);
        cpr_no = elm.cpr_no;
        soc_no = elm.soc_no;
        break;
    /* RE Forwarding Notice */
    case CMD_TSKIF_RE_FWDNTC:
    /* REC Forwarding Notice */
    case CMD_TSKIF_REC_FWDNTC:
    	 //datp = ((T_MSG*)bufp)->data;
    	datp = bufp;
        sig_kind = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind;
        cpr_no   = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->cprircv_inf.link_num;
    	break;
    /* CPRI信号受信通知 */
    case CMD_TSKIF_CPRIRCVNTC:
         //datp = ((T_MSG*)bufp)->data;
    	datp = bufp;
        sig_kind = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind;
        cpr_no   = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->cprircv_inf.link_num;
        ret_code = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->dummy;
        /* 信号種別で分岐 */
        switch(sig_kind) {
        /* TRX設定応答 */
        case CMD_CPRID_TRXSETRES:
            soc_no = ((CMT_TSKIF_CPRIRCV_TRXSETRES*)datp)->cpridat_trxsetres.carno;
            break;
        /* TRX解放応答 */
        case CMD_CPRID_TRXRELRES:
            soc_no = ((CMT_TSKIF_CPRIRCV_TRXRELRES*)datp)->cpridat_trxrelres.carno;
            break;
        /* 無変調キャリアON/OFF指定応答 */
        case CMD_CPRID_CARONOFFRES:
            soc_no = ((CMT_TSKIF_CPRIRCV_CARONOFFRES*)datp)->cpridat_caronoffres.carno;
            break;
//        /* REカード制御応答 */
        case CMD_CPRID_RECARDCNTRES:
            soc_no = ((CMT_TSKIF_CPRIRCV_RECARDCNTRES*)datp)->cpridat_recardcntres.slot_no;
//            break;
//        /* REカード診断応答 */
//        case CMD_CPRID_RECRDDIARES:
//            soc_no = ((CMT_TSKIF_CPRIRCV_RECRDDIARES*)datp)->cpridat_recrddiares.slot_no;
//            break;
//        /* REバージョン報告応答 */
//        case CMD_CPRID_REVERRES:
//            soc_no = ((CMT_TSKIF_CPRIRCV_REVERRES*)datp)->cpridat_reverres.rever_inf.slot_no;
//            break;
        /* REカード状態報告応答 */
        case CMD_CPRID_CARDSTSRES:
            soc_no = ((CMT_TSKIF_CPRIRCV_CARDSTSRES*)datp)->cpridat_cardstsres.cardsts_inf.slot_no;
            break;
        }
        break;
    /* CPRI信号送信指示 */
    case CMD_TSKIF_CPRISNDIND: 
         //datp = ((T_MSG*)bufp)->data;
    	datp = bufp;
        sig_kind = ((CMT_TSKIF_CPRISND_SIGNALGET*)datp)->signal_kind;
        cpr_no   = ((CMT_TSKIF_CPRISND_SIGNALGET*)datp)->cprisnd_inf.link_num;
        /* 信号種別で分岐 */
        switch(sig_kind) {
        /* TRX設定要求 */
        case CMD_CPRID_TRXSETREQ:
            soc_no = ((CMT_TSKIF_CPRISND_TRXSETREQ*)datp)->cpridat_trxsetreq.carno;
            break;
        /* TRX解放要求 */
        case CMD_CPRID_TRXRELREQ:
            soc_no = ((CMT_TSKIF_CPRISND_TRXRELREQ*)datp)->cpridat_trxrelreq.carno;
            break;
        /* 無変調キャリアON/OFF指定要求 */
        case CMD_CPRID_CARONOFFREQ:
            soc_no = ((CMT_TSKIF_CPRISND_CARONOFFREQ*)datp)->cpridat_caronoffreq.carno;
            break;
//        /* REバージョン報告要求 */
//        case CMD_CPRID_REVERREQ:
//            soc_no = ((CMT_TSKIF_CPRISND_REVERREQ*)datp)->cpridat_reverreq.slot_no;
//            break;
//        /* REカード制御要求 */
//        case CMD_CPRID_RECARDCNTREQ:
//            soc_no = ((CMT_TSKIF_CPRISND_RECARDCNTREQ*)datp)->cpridat_recardcntreq.slot_no;
//            break;
//        /* REカード診断要求 */
//        case CMD_CPRID_RECRDDIAREQ:
//            soc_no = ((CMT_TSKIF_CPRISND_RECRDDIAREQ*)datp)->cpridat_recrddiareq.slot_no;
//            break;
        /* REカード状態報告要求 */
        case CMD_CPRID_CARDSTSREQ:
            soc_no = ((CMT_TSKIF_CPRISND_CARDSTSREQ*)datp)->cpridat_cardstsreq.slot_no;
            break;
        }
        break;
//    /* CPRIリンク状態変化通知 */
//    case CMD_TSKIF_CPRSTACHGNTC:
//        sig_kind = ((CMT_TSKIF_CPRSTACHGNTC*)bufp)->link_sts;
//        cpr_no   = ((CMT_TSKIF_CPRSTACHGNTC*)bufp)->cpri_no;
//        break;
//    /* CPRIリンク断通知 */
//    case CMD_TSKIF_CPRDSCNTC:
//        sig_kind = ((CMT_TSKIF_CPRDSCNTC*)bufp)->link_sts;
//        cpr_no   = ((CMT_TSKIF_CPRDSCNTC*)bufp)->cpri_no;
//        break;
//    /* Active設定要求 */
//    case CMD_TSKIF_ACTSETREQ:
//        cpr_no   = ((CMT_TSKIF_ACTSETREQ*)bufp)->cpri_no;
//        break;
//    /* Active設定応答 */
//    case CMD_TSKIF_ACTSETRSP:
//        cpr_no   = ((CMT_TSKIF_ACTSETRSP*)bufp)->activelink_no;
//        ret_code = ((CMT_TSKIF_ACTSETRSP*)bufp)->result_code;
//        break;
//    /* Active切替指示 */
//    case CMD_TSKIF_ACTCHGIND:
//        sig_kind = ((CMT_TSKIF_ACTCHGIND*)bufp)->chg_trg;
//        cpr_no   = ((CMT_TSKIF_ACTCHGIND*)bufp)->chglink_no;
//        break;
//    /* Active切替結果通知 */
//    case CMD_TSKIF_ACTCHGRLTNTC:
//        sig_kind = ((CMT_TSKIF_ACTCHGRLTNTC*)bufp)->chg_trg;
//        if (((CMT_TSKIF_ACTCHGRLTNTC*)bufp)->chg_result == CMD_RLTCODE_OK) {
//            cpr_no   = ((CMT_TSKIF_ACTCHGRLTNTC*)bufp)->newactlink_no;
//        }
//        else {
//            cpr_no   = ((CMT_TSKIF_ACTCHGRLTNTC*)bufp)->chglink_no;
//        }
//        ret_code = ((CMT_TSKIF_ACTCHGRLTNTC*)bufp)->chg_result;
//        break;
//    /* Negative通知 */
//    case CMD_TSKIF_NEGNTC:
//        sig_kind = ((CMT_TSKIF_NEGNTC*)bufp)->actdsc_inf;
//        cpr_no   = ((CMT_TSKIF_NEGNTC*)bufp)->oldactlink_no;
//        break;
//    /* 二次リセット要求 */
//    case CMD_TSKIF_2NDRST_REQ:
//        cpr_no   = ((CMT_TSKIF_2NDRST_REQ*)bufp)->cpri_no;
//        break;
//    /* 二次リセット応答 */
//    case CMD_TSKIF_2NDRST_RSP:
//        cpr_no   = ((CMT_TSKIF_2NDRST_RSP*)bufp)->cpri_no;
//        break;
    /* 強制停止通知 */ 
    case CMD_TSKIF_RECMPSTPNTC:
        cpr_no   = ((CMT_TSKIF_RECMPSTPNTC*)bufp)->link_num;
        break;
//    /* RE起動完了通知 */
//    case CMD_TSKIF_RESRTCMPNTC:
//        cpr_no   = ((CMT_TSKIF_RESRTCMPNTC*)bufp)->cpri_no;
//        break;
    /* REシステムパラメータ更新要求 */
    case CMD_TSKIF_RESYSNEWREQ:
        cpr_no   = ((CMT_TSKIF_RESYSNEWREQ*)bufp)->cpri_no;
        break;
    /* REシステムパラメータ更新応答 */
    case CMD_TSKIF_RESYSNEWRES:
        cpr_no   = ((CMT_TSKIF_RESYSNEWRES*)bufp)->cpri_no;
        ret_code = ((CMT_TSKIF_RESYSNEWRES*)bufp)->result_code;
        break;
    /* RE時刻設定プロシージャ実行指示 */
    case CMD_TSKIF_RETIMSETPRCIND:
        cpr_no   = ((CMT_TSKIF_RETIMSETPRCIND*)bufp)->cpri_no;
        break;
    /* REシステムパラメータ更新プロシージャ実行指示 */
    case CMD_TSKIF_REPRMPRCIND:
        cpr_no   = ((CMT_TSKIF_REPRMPRCIND*)bufp)->cpri_no;
        break;
    /* REファイル更新プロシージャ実行指示 */
    case CMD_TSKIF_REFILPRCIND:
        cpr_no   = ((CMT_TSKIF_REFILPRCIND*)bufp)->cpri_no;
        break;
    /* Toffset値報告プロシージャ実行指示 */
    case CMD_TSKIF_TOFVALPRCIND:
        cpr_no   = ((CMT_TSKIF_TOFVALPRCIND*)bufp)->cpri_no;
        break;
    /* Active設定プロシージャ実行指示 */
    case CMD_TSKIF_ACTSETPRCIND:
        cpr_no   = ((CMT_TSKIF_ACTSETPRCIND*)bufp)->cpri_no;
        break;
    /* RE装置構成情報報告プロシージャ実行指示 */
    case CMD_TSKIF_REEQPINFPRCIND:
        cpr_no   = ((CMT_TSKIF_REEQPINFPRCIND*)bufp)->cpri_no;
        break;
    /* REスロット情報報告プロシージャ実行指示 */
    case CMD_TSKIF_RESLTINFPRCIND:
        cpr_no   = ((CMT_TSKIF_RESLTINFPRCIND*)bufp)->cpri_no;
        break;
    /* RE状態報告確プロシージャ実行指示 */
    case CMD_TSKIF_RESTSPRCIND:
        cpr_no   = ((CMT_TSKIF_RESTSPRCIND*)bufp)->cpri_no;
        break;
    /* RE PORT状態報告プロシージャ実行指示 */
    case CMD_TSKIF_REPRTSTSPRCIND:
        cpr_no   = ((CMT_TSKIF_REPRTSTSPRCIND*)bufp)->cpri_no;
        break;
    /* REカード状態報告(セクタ部)プロシージャ実行指示 */
    case CMD_TSKIF_RECRDSSCPRCIND:
        cpr_no   = ((CMT_TSKIF_RECRDSSCPRCIND*)bufp)->cpri_no;
        break;
    /* REカード状態報告(共通部)プロシージャ実行指示 */
    case CMD_TSKIF_RECRDSCMPRCIND:
        cpr_no   = ((CMT_TSKIF_RECRDSCMPRCIND*)bufp)->cpri_no;
        break;
    /* REキャリア状態報告プロシージャ実行指示 */
    case CMD_TSKIF_RECRRSTSPRCIND:
        cpr_no   = ((CMT_TSKIF_RECRRSTSPRCIND*)bufp)->cpri_no;
        break;
    /* MTアドレス設定(FLD-MT)プロシージャ実行指示 */
    case CMD_TSKIF_MTSETFPRCIND:
        cpr_no   = ((CMT_TSKIF_MTSETFPRCIND*)bufp)->cpri_no;
        break;
    /* MTアドレス設定(SV-MT)プロシージャ実行指示 */
    case CMD_TSKIF_MTSETSPRCIND:
        cpr_no   = ((CMT_TSKIF_MTSETSPRCIND*)bufp)->cpri_no;
        break;
    /* RE起動完了プロシージャ実行指示 */
    case CMD_TSKIF_RESCMPPRCIND:
        cpr_no   = ((CMT_TSKIF_RESCMPPRCIND*)bufp)->cpri_no;
        break;
    /* REリセットプロシージャ実行指示 */
    case CMD_TSKIF_RERSTPRCIND:
        cpr_no   = ((CMT_TSKIF_RERSTPRCIND*)bufp)->cpri_no;
        break;
    /* RE強制リセットプロシージャ実行指示 */
    case CMD_TSKIF_REFRCRSTPRCIND:
        cpr_no   = ((CMT_TSKIF_REFRCRSTPRCIND*)bufp)->cpri_no;
        break;
    /* RE強制リセット設置プロシージャ実行指示 */
    case CMD_TSKIF_REFRCRSTSETPRCIND:
        cpr_no   = ((CMT_TSKIF_REFRCRSTSETPRCIND*)bufp)->cpri_no;
        break;    
    /* 強制停止プロシージャ実行指示 */
    case CMD_TSKIF_FRCSTPPRCIND:
        cpr_no   = ((CMT_TSKIF_FRCSTPPRCIND*)bufp)->cpri_no;
        break;
//    /* TRX設定(自律)プロシージャ実行指示 */
//    case CMD_TSKIF_TXSAPRCIND:
//        cpr_no   = ((CMT_TSKIF_TXSAPRCIND*)bufp)->cpri_no;
//        soc_no   = ((CMT_TSKIF_TXSAPRCIND*)bufp)->crr_no;
//        break;
//    /* TRX解放(自律)プロシージャ実行指示 */
//    case CMD_TSKIF_TXRAPRCIND:
//        cpr_no   = ((CMT_TSKIF_TXRAPRCIND*)bufp)->cpri_no;
//        soc_no   = ((CMT_TSKIF_TXRAPRCIND*)bufp)->crr_no;
//        break;
//    /* 無変調キャリアON/OFF指定(自律)プロシージャ実行指示 */
//    case CMD_TSKIF_NMCAPRCIND:
//        cpr_no   = ((CMT_TSKIF_NMCAPRCIND*)bufp)->cpri_no;
//        soc_no   = ((CMT_TSKIF_NMCAPRCIND*)bufp)->crr_no;
//        break;
    /* REファイル更新要求 */
    case CMD_TSKIF_REFIRM_DL_REQ:
        cpr_no   = ((CMT_TSKIF_REFILNEWREQ*)bufp)->cpri_no;
        break;
    /* REファイル更新応答 */
    case CMD_TSKIF_REFIRM_DL_RSP:
        cpr_no   = ((CMT_TSKIF_REFILNEWRES*)bufp)->cpri_no;
        ret_code = ((CMT_TSKIF_REFILNEWRES*)bufp)->result_code;
        break;
        
#ifdef D_TCM_MTSET_FLD
    /* MTアドレス設定要求 */
    case CMD_TSKIF_MTASTREQ:
        cpr_no   = ((CMT_TSKIF_MTASTREQ*)bufp)->link_num;
        sig_kind = ((CMT_TSKIF_MTASTREQ*)bufp)->mt_object;
        break;
    /* MTアドレス設定応答 */
    case CMD_TSKIF_MTASTRES:
        cpr_no   = ((CMT_TSKIF_MTASTRSP*)bufp)->link_num;
        sig_kind = ((CMT_TSKIF_MTASTRSP*)bufp)->mt_object;
        ret_code = ((CMT_TSKIF_MTASTRSP*)bufp)->result;
        break;
#endif

//    case CMD_TSKIF_NECHGCMPNTC:
//        ret_code = ((CMT_TSKIF_NECHGCMPNTC*)bufp)->prockind;
//        break;
//    case CMD_TSKIF_NEDATCMPNTC:
//        ret_code = ((CMT_TSKIF_NEDATCMPNTC*)bufp)->prockind;
//        break;

    /**************/
    /* プロセス間 */
    /**************/
//    /* CPRIlink establishment complete notification */
//    case D_MSG_CPRIESTBLSHCMP_NTF:
//        cpr_no   = ((T_MSG_CPRIESTBLSHCMP_NTF*)bufp)->msgData.CpriLinknum;
//        break;        
//    /* REファイル更新指示 */
//    case D_MSG_REFUPDTSTART_IND:
//        soc_no   = ((T_MSG_REFUPDTSTART_IND*)bufp)->msgData.sdmside;
//        sig_kind = ((T_MSG_REFUPDTSTART_IND*)bufp)->msgData.checktimer;
//        break;
//    /* REファイル更新結果通知 */
//    case D_MSG_REFUPDTCMP_NTF:
//        break;
//    /* TRX設定要求 */
//    case D_MSG_TRXSET_REQ:
//        cpr_no   = ((T_MSG_TRXSET_REQ*)bufp)->data.trxSetReqdata.cpriLinkNo;
//        soc_no   = ((T_MSG_TRXSET_REQ*)bufp)->data.trxSetReqdata.carNo;
//        break;
//    /* TRX設定応答 */
//    case D_MSG_TRXSET_RES:
//        cpr_no   = ((T_MSG_TRXSET_RES*)bufp)->data.trxSetResdata.cpriLinkNo;
//        soc_no   = ((T_MSG_TRXSET_RES*)bufp)->data.trxSetResdata.carNo;
//        ret_code = ((T_MSG_TRXSET_RES*)bufp)->data.resCommon.fctCode;
//        break;
//    /* TRX解放要求 */
//    case D_MSG_TRXREL_REQ:
//        cpr_no   = ((T_MSG_TRXREL_REQ*)bufp)->data.trxRelReqdata.cpriLinkNo;
//        soc_no   = ((T_MSG_TRXREL_REQ*)bufp)->data.trxRelReqdata.carNo;
//        break;
//    /* TRX解放応答 */
//    case D_MSG_TRXREL_RES:
//        cpr_no   = ((T_MSG_TRXREL_RES*)bufp)->data.trxRelResdata.cpriLinkNo;
//        soc_no   = ((T_MSG_TRXREL_RES*)bufp)->data.trxRelResdata.carNo;
//        ret_code = ((T_MSG_TRXREL_RES*)bufp)->data.resCommon.fctCode;
//        break;
//    /* 無変調キャリアON/OFF指定要求 */
//    case D_MSG_CARONOFF_REQ:
//        cpr_no   = ((T_MSG_CARONOFF_REQ*)bufp)->data.carOnOffReqdata.cpriLinkNo;
//        sig_kind = ((T_MSG_CARONOFF_REQ*)bufp)->data.carOnOffReqdata.onoff;
//        soc_no   = ((T_MSG_CARONOFF_REQ*)bufp)->data.carOnOffReqdata.carNo;
//        break;
//    /* 無変調キャリアON/OFF指定応答 */
//    case D_MSG_CARONOFF_RES:
//        cpr_no   = ((T_MSG_CARONOFF_RES*)bufp)->data.carOnOffResdata.cpriLinkNo;
//        soc_no   = ((T_MSG_CARONOFF_RES*)bufp)->data.carOnOffResdata.carNo;
//        ret_code = ((T_MSG_CARONOFF_RES*)bufp)->data.resCommon.fctCode;
//        break;
//    /* T14 value change interription notice */
//    case D_MSG_T14CHG_INT_NTC:
//         cpr_no   = ((T_MSG_T14CHG_INT_NTC*)bufp)->data.cpr_no;
//         break;
//    /* RE Status Collect Request */
//    case D_MSG_RESTSCLT_REQ:
//        cpr_no   = ((T_MSG_RESTSCLT_REQ*)bufp)->data.cpr_no;
//        break;
//    /* RE状態管理テーブル初期化通知 */
//    case D_MSG_RESTSMNGINI_NTC:
//    /* RE再開完了通知 */
//    case D_MSG_RESTARTUPCMP_NTC:
//    /* RE Status Collect Response */
//    case D_MSG_RESTSCLT_RSP:
//        bodp = ((UCHAR*)bufp) + sizeof(T_MSGHEAD);
//        sig_kind = ((T_DATA_RESTSMNG*)bodp)->fct;
//        cpr_no   = ((T_DATA_RESTSMNG*)bodp)->cpr_no;
//        break;
//    /* RE共通部リンク確立状態変化通知 */
//    case D_MSG_REACTCHG_NTC:
//        sig_kind = ((T_MSG_REACTCHG_NTC*)bufp)->data.ansts.ansts;
//        cpr_no   = ((T_MSG_REACTCHG_NTC*)bufp)->data.ansts.cpr_no;
//        break;
//    /* FLD-MT Active Status Change Notice */
//    case D_MSG_ACTSTSCHG_NTC:
//        sig_kind = ((T_MSG_ACTSTSCHG_NTC*)bufp)->data.status;
//        re_kind  = ((T_MSG_ACTSTSCHG_NTC*)bufp)->data.recre_kind;
//        cpr_no   = ((T_MSG_ACTSTSCHG_NTC*)bufp)->data.re_no;
//        break;
//    /* CPRI Delay Information Change Notice */
//    case D_MSG_DLYCHG_NTC:
//        cpr_no   = ((T_MSG_DLYCHG_NTC*)bufp)->data.cpr_no;
//        break;
//    /* RE装置構成情報変化通知 */
//    case D_MSG_REEQPINFCHG_NTC:
//        cpr_no   = ((T_MSG_REEQPINFCHG_NTC*)bufp)->data.cpr_no;
//        break;
//    /* RE状態変化通知 */
//    case D_MSG_RESTSCHG_NTC:
//        re_kind  = ((T_MSG_RESTSCHG_NTC*)bufp)->data.re_kind;
//        cpr_no   = ((T_MSG_RESTSCHG_NTC*)bufp)->data.am_no;
//        break;        
//    /* RE Slot情報変化通知 */
//    case D_MSG_RESLTINFCHG_NTC:
//        cpr_no   = ((T_MSG_RESLTINFCHG_NTC*)bufp)->data.cpr_no;
//        break;
//    /* REカード状態変化通知 */
//    case D_MSG_RECRDSTSCHG_NTC:
//        re_kind  = ((T_MSG_RECRDSTSCHG_NTC*)bufp)->data.re_kind;
//        cpr_no   = ((T_MSG_RECRDSTSCHG_NTC*)bufp)->data.am_no;
//        soc_no   = ((T_MSG_RECRDSTSCHG_NTC*)bufp)->data.recrdsts.slt_no;
//        break;
//    /* RE PORT 状態変化通知 */
//    case D_MSG_POTSSTACHG_NTY:
//        re_kind  = ((T_MSG_POTSSTACHG_NTY*)bufp)->data.recre_kind;
//        cpr_no   = ((T_MSG_POTSSTACHG_NTY*)bufp)->data.re_no;
//        break;
//    /* RE Carrier 状態変化通知 */
//    case D_MSG_RECRRSTSCHG_NTC:
//        cpr_no   = ((T_MSG_RECRRSTSCHG_NTC*)bufp)->data.cpr_no;
//        break;
//    /* REカード制御要求 */
//    case D_MSG_CARDCONTROL_REQ:
//        re_kind  = ((T_MSG_CARDCONTROL_REQ*)bufp)->data.recre_kind;
//        cpr_no   = ((T_MSG_CARDCONTROL_REQ*)bufp)->data.re_num;
//        soc_no   = ((T_MSG_CARDCONTROL_REQ*)bufp)->data.slot_num;
//        break;
//    /* REカード制御応答 */
//    case D_MSG_CARDCONTROL_RES:
//        re_kind  = ((T_MSG_CARDCONTROL_RES*)bufp)->data.recre_kind;
//        cpr_no   = ((T_MSG_CARDCONTROL_RES*)bufp)->data.re_num;
//        soc_no   = ((T_MSG_CARDCONTROL_RES*)bufp)->data.slot_num;
//        ret_code = ((T_MSG_CARDCONTROL_RES*)bufp)->data.flg;
//        break;
//    /* REカード診断要求 */
//    case D_MSG_DIAG_REQ:
//        re_kind  = ((T_MSG_DIAG_REQ*)bufp)->data.recre;
//        cpr_no   = ((T_MSG_DIAG_REQ*)bufp)->data.rrelre;
//        soc_no   = ((T_MSG_DIAG_REQ*)bufp)->data.slt;
//        break;
//    /* REカード診断応答 */
//    case D_MSG_DIAG_RES:
//        re_kind  = ((T_MSG_DIAG_RES*)bufp)->data.recre;
//        cpr_no   = ((T_MSG_DIAG_RES*)bufp)->data.rrelre;
//        soc_no   = ((T_MSG_DIAG_RES*)bufp)->data.slt;
//        ret_code = ((T_MSG_DIAG_RES*)bufp)->data.flg;
//        break;
//    /* RE PORT制御要求 */
//    case D_MSG_POTOPTRLYCTL_REQ:
//        re_kind  = ((T_MSG_POTOPTRLYCTL_REQ*)bufp)->data.recre_kind;
//        cpr_no   = ((T_MSG_POTOPTRLYCTL_REQ*)bufp)->data.re_no;
//        break;
//    /* RE PORT制御応答 */
//    case D_MSG_POTOPTRLYCTL_RES:
//        re_kind  = ((T_MSG_POTOPTRLYCTL_RES*)bufp)->data.recre_kind;
//        cpr_no   = ((T_MSG_POTOPTRLYCTL_RES*)bufp)->data.re_no;
//        ret_code = ((T_MSG_POTOPTRLYCTL_RES*)bufp)->data.sts_flg;
//        break;
//    /* バージョン報告要求 */
//    case D_MSG_VERREP_REQ:
//        re_kind  = ((T_MSG_VERREP_REQ*)bufp)->data.recre;
//        cpr_no   = ((T_MSG_VERREP_REQ*)bufp)->data.renum;
//        soc_no   = ((T_MSG_VERREP_REQ*)bufp)->data.slt;
//        break;
//    /* バージョン報告応答  */
//    case D_MSG_VERREP_RES:
//        re_kind  = ((T_MSG_VERREP_RES*)bufp)->data.recre;
//        cpr_no   = ((T_MSG_VERREP_RES*)bufp)->data.renum;
//        soc_no   = ((T_MSG_VERREP_RES*)bufp)->data.slt;
//        ret_code = ((T_MSG_VERREP_RES*)bufp)->data.state;
//        break;
//    /* RE 障害ログ取得要求 */
//    case D_MSG_TRBLOG_RE_GET_REQ:
//        re_kind  = ((T_MSG_TRBLOG_RE_GET_REQ*)bufp)->data.recre;
//        cpr_no   = ((T_MSG_TRBLOG_RE_GET_REQ*)bufp)->data.cpriNo;
//        soc_no   = ((T_MSG_TRBLOG_RE_GET_REQ*)bufp)->data.slt;
//        break;
//    /* RE 障害ログ取得応答 */
//    case D_MSG_TRBLOG_GET_RES:
//        re_kind  = ((T_MSG_TRBLOG_GET_RES*)bufp)->data.recre;
//        cpr_no   = ((T_MSG_TRBLOG_GET_RES*)bufp)->data.cpriNo;
//        ret_code = ((T_MSG_TRBLOG_GET_RES*)bufp)->data.ngfct;
//        break;
//    /* RE 障害ログ中止要求 */
//    case D_MSG_TRBLOG_GETSTOP_REQ:
//        re_kind  = ((T_MSG_TRBLOG_GETSTOP_REQ*)bufp)->data.recre;
//        cpr_no   = ((T_MSG_TRBLOG_GETSTOP_REQ*)bufp)->data.cpriNo;
//        soc_no   = ((T_MSG_TRBLOG_GETSTOP_REQ*)bufp)->data.slt;
//        break;
//    /* RE 障害ログ中止応答 */
//    case D_MSG_TRBLOG_GETSTOP_RES:
//        re_kind  = ((T_MSG_TRBLOG_GETSTOP_RES*)bufp)->data.recre;
//        cpr_no   = ((T_MSG_TRBLOG_GETSTOP_RES*)bufp)->data.cpriNo;
//        ret_code = ((T_MSG_TRBLOG_GETSTOP_RES*)bufp)->data.ngfct;
//        break;
//    /* RE障害ログ送信      */
//    case D_MSG_TRBLOG_DATA_NTY:
//        break;
//    /* RE障害ログ送信完了  */
//    case D_MSG_TRBLOG_COMPLETE_NTY:
//        ret_code = ((T_MSG_TRBLOG_COMPLETE_NTY*)bufp)->data.retFlg;
//        break;
//    /* CPRIリンクリセット要求 */
//    case D_MSG_CPRILINKRESET_REQ:
//        sig_kind = ((T_MSG_CPRILINKRESET_REQ*)bufp)->data.rst_kind;
//        cpr_no   = ((T_MSG_CPRILINKRESET_REQ*)bufp)->data.cpri_link;
//        break;
//    /* CPRIリンクリセット応答 */
//    case D_MSG_CPRILINKRESET_RES:
//        cpr_no   = ((T_MSG_CPRILINKRESET_RES*)bufp)->data.cpr_no;
//        ret_code = ((T_MSG_CPRILINKRESET_RES*)bufp)->data.ret_code;
//        break;
//    /* REリセット要求 */
//    case D_MSG_RERST_REQ:
//        re_kind  = ((T_MSG_RERST_REQ*)bufp)->data.recre_kind;
//        cpr_no   = ((T_MSG_RERST_REQ*)bufp)->data.re_no;
//        break;
//    /* REリセット応答 */
//    case D_MSG_RERST_RES:
//        re_kind  = ((T_MSG_RERST_RES*)bufp)->data.recre_kind;
//        cpr_no   = ((T_MSG_RERST_RES*)bufp)->data.re_no;
//        ret_code = ((T_MSG_RERST_RES*)bufp)->data.ngFact;
//        break;

    /* defaultの場合はログをとらない */
    default:
        return CMD_OK;
    }

    /********************************/
    /* 収集データ設定(ボディデータ) */
    /********************************/
    switch (msgid) {
    /* CPRI信号 */
    case CMD_TSKIF_CPRIRCVNTC:
    case CMD_TSKIF_CPRISNDIND:
         //datp = ((T_MSG*)bufp)->data;
    	datp = bufp;
        bodp = &(((CMT_TSKIF_CPRISND_SIGNALGET*)datp)->signal_kind);
        break;
    /* 通常 */
    default:
        bodp = ((UCHAR*)bufp) + sizeof(T_MSGHEAD);
        break;
    }

#ifdef DEBUG_TRIF
    if(rhk == D_REC_RUNHIS_RHK_RCV)
    {
		TRACE(" TRIF(REC):RunHis[RCV][evt=0x%08X][thr=%s(%d)][lnk=%d][No=%d][sig=0x%04X] ####",
					 msgid, f_trp_com_Rlogfnc_tbl.inf[(fb_no&0xFFF)].thr_name, fb_no, cpr_no, soc_no, sig_kind);
	}
	else if(rhk == D_REC_RUNHIS_RHK_SND)
    {
		TRACE(" TRIF(REC):RunHis[SND][evt=0x%08X][thr=%s(%d)][lnk=%d][No=%d][sig=0x%04X] ####",
					 msgid, f_trp_com_Rlogfnc_tbl.inf[(fb_no&0xFFF)].thr_name, fb_no, cpr_no, soc_no, sig_kind);
	}
	else
    {
		TRACE(" TRIF(REC):RunHis[ETC][evt=0x%08X][thr=%s(%d)][lnk=%d][No=%d][sig=0x%04X] ####",
					 msgid, f_trp_com_Rlogfnc_tbl.inf[(fb_no&0xFFF)].thr_name, fb_no, cpr_no, soc_no, sig_kind);
	}
#endif
    /****************************/
    /* 収集データテーブル書込み */
    /****************************/
    /* BCD時刻取得 */
    f_trp_rec_Fcom_t_bcdtim_get(&bcdtim);
    /* 書き込み位置取得 */
    wrt_idx = f_trp_rec_Wcom_runhis_tbl.wrt_idx;
    log_wrt_idx = f_trp_com_WlogWriteIndex;
    /* ヘッダデータ書き込み */
    f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].head.year     = bcdtim.year;
    f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].head.month    = bcdtim.month;
    f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].head.day      = bcdtim.day;
    f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].head.hour     = bcdtim.hour;
    f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].head.min      = bcdtim.min;
    f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].head.sec      = bcdtim.sec;
    f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].head.msec1    = bcdtim.msec1;
    f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].head.msec2    = bcdtim.msec2;
    f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].head.msgid    = msgid;
    f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].head.fb_no    = fb_no;
    f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].head.sig_kind = sig_kind;
    f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].head.re_kind  = re_kind;
    f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].head.cpr_no   = cpr_no;
    f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].head.soc_no   = soc_no;
    f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].head.ret_code = ret_code;
    f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].head.rhk      = rhk;
    /* ボディデータ書き込み */
    f_trp_com_Fmemcpy(                                      /* 終了コード:R */
        &(f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].body),       /* コピー先:I */
        bodp,                                                 /* コピー元:I */
        sizeof(f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].body)
                                                          /* コピーサイズ:I */
    );
    
#if 0
    /************************/
    /* 表示用キャラクタ作成 */
    /************************/
    /* 時刻 */
    sprintf(tim_as, "%02x/%02x/%02x %02x:%02x:%02x.%02x%02x",
        bcdtim.year, bcdtim.month, bcdtim.day,   bcdtim.hour,
        bcdtim.min,  bcdtim.sec,   bcdtim.msec1, bcdtim.msec2
    );
    /* メッセージID */
    sprintf(msg_as, "0x%08x", msgid);
    /* 信号種別 */
    if (sig_kind == CMD_ALLF_SHORT) {
        sprintf(sig_as, "      ");
    }
    else {
        sprintf(sig_as, "0x%04x", sig_kind);
    }
    /* RE Kind */
    if (re_kind == CMD_ALLF_CHAR){
        sprintf(cpr_as, "    ");
    }
    else {
        sprintf(rek_as, "0x%02x", re_kind);
    }
    /* CPRI番号 */
    if (cpr_no == CMD_ALLF_CHAR) {
        sprintf(cpr_as, "    ");
    }
    else {
        sprintf(cpr_as, "0x%02x", cpr_no);
    }
    /* スロット番号orキャリア番号 */
    if (soc_no == CMD_ALLF_CHAR) {
        sprintf(soc_as, "    ");
    }
    else {
        sprintf(soc_as, "0x%02x", soc_no);
    }
    /* 結果コード */
    if (ret_code == CMD_ALLF_CHAR) {
        sprintf(ret_as, "    ");
    }
    else {
        sprintf(ret_as, "0x%02x", ret_code);
    }
    /* FB番号 */
    sprintf(fbn_as, "0x%08x", fb_no);

    /********************/
    /* トレースログ表示 */
    /********************/
    /* RUNHIS種別で分岐 */
    switch (rhk) {
    /* 受信 */
    case D_REC_RUNHIS_RHK_RCV:
        F_COM_CMM_FANCRACE("[REC_RUNHIS][%s]<---[msg:%s][sig:%s][rek:%s][cpr:%s][soc:%s][ret:%s]----[src:%s]",
            tim_as, msg_as, sig_as, rek_as, cpr_as, soc_as, ret_as, fbn_as);
        break;
    /* 送信 */
    case D_REC_RUNHIS_RHK_SND:
        F_COM_CMM_FANCRACE("[REC_RUNHIS][%s]----[msg:%s][sig:%s][rek:%s][cpr:%s][soc:%s][ret:%s]--->[dst:%s]",
            tim_as, msg_as, sig_as, rek_as, cpr_as, soc_as, ret_as, fbn_as);
        break;
    }
#endif

    /**********************/
    /* 次走行履歴書込準備 */
    /**********************/
    /* 書き込み位置更新 */
    D_REC_INCLBK(f_trp_rec_Wcom_runhis_tbl.wrt_idx, D_REC_RUNHIS_NUM);
    /* 次エリア0クリア */
    wrt_idx = f_trp_rec_Wcom_runhis_tbl.wrt_idx;
    f_trp_com_Fmemclr(
        &(f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx]),
        sizeof(f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx])
    );

    return CMD_OK;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
/*
┏━━━━━━━━━━━━━━━━━━━━━━━━━┯━━━━━━━━━━━━━┯━━━━━━━━━━━━━┯━━━━━━━━━━━━━┯━━━━━━━━━━━━━┓
┃スレッド間イベント                                │信号種別                  │CPRI番号                  │スロット番号orキャリア番号│結果コード                ┃
┠─────────────────────────┼─────────────┼─────────────┼─────────────┼─────────────┨
┃スレッド起動完了通知                              │-                         │-                         │-                         │結果コード                ┃
┃スレッド初期化応答                                │-                         │-                         │-                         │結果コード                ┃
┃タイムアウト発生通知                              │クラス＋タイマ種別        │CPRI番号                  │スロット番号orキャリア番号│-                         ┃
┠─────────────────────────┼─────────────┼─────────────┼─────────────┼─────────────┨
┃CPRI信号受信通知                                  │CPRI信号種別              │(下記参照)                │(下記参照)                │(下記参照)                ┃
┠ - - - - - - - - - - - - - - - - - - - - - - - - -├ - - - - - - - - - - - - -├ - - - - - - - - - - - - -├ - - - - - - - - - - - - -├ - - - - - - - - - - - - -┃
┃TRX設定応答                                       │-                         │CPRI番号                  │キャリア番号              │結果コード                ┃
┃Toffset値報告応答                                 │-                         │CPRI番号                  │-                         │結果コード                ┃
┃無変調キャリアON/OFF指定応答                      │-                         │CPRI番号                  │キャリア番号              │結果コード                ┃
┃REバージョン報告応答                              │-                         │CPRI番号                  │スロット番号              │結果コード                ┃
┃REカード制御応答                                  │-                         │CPRI番号                  │スロット番号              │結果コード                ┃
┃REカード状態報告応答                              │-                         │CPRI番号                  │スロット番号              │結果コード                ┃
┠─────────────────────────┼─────────────┼─────────────┼─────────────┼─────────────┨
┃CPRI信号送信指示                                  │CPRI信号種別              │(下記参照)                │(下記参照)                │(下記参照)                ┃
┠ - - - - - - - - - - - - - - - - - - - - - - - - -├ - - - - - - - - - - - - -├ - - - - - - - - - - - - -├ - - - - - - - - - - - - -├ - - - - - - - - - - - - -┃
┃REリセット要求                                    │-                         │CPRI番号                  │-                         │-                         ┃
┃RE時刻設定要求                                    │-                         │CPRI番号                  │-                         │-                         ┃
┃TRX設定要求                                       │-                         │CPRI番号                  │キャリア番号              │-                         ┃
┃Toffset値報告要求                                 │-                         │CPRI番号                  │-                         │-                         ┃
┃無変調キャリアON/OFF指定要求                      │-                         │CPRI番号                  │キャリア番号              │-                         ┃
┃REバージョン報告要要求                            │-                         │CPRI番号                  │スロット番号              │-                         ┃
┃REカード制御要求                                  │-                         │CPRI番号                  │スロット番号              │-                         ┃
┃REカード状態報告要求                              │-                         │CPRI番号                  │スロット番号              │-                         ┃
┠─────────────────────────┼─────────────┼─────────────┼─────────────┼─────────────┨
┃CPRIリンク状態変化通知                            │リンク状態                │CPRI番号                  │-                         │-                         ┃
┃CPRIリンク断通知                                  │リンク状態                │CPRI番号                  │-                         │-                         ┃
┃Active設定要求                                    │-                         │CPRI番号                  │-                         │-                         ┃
┃Active設定応答                                    │-                         │Active CPRI番号           │-                         │結果コード                ┃
┃Active切替結果通知                                │切替トリガ                │OKの場合:切替先CPRI番号   │-                         │結果コード                ┃
┃                                                  │                          │NGの場合:切替元CPRI番号   │-                         │-                         ┃
┃Negative通知                                      │1:Negative 2:NegativeTO   │旧Active CPRI番号         │-                         │-                         ┃
┃二次リセット要求                                  │-                         │CPRI番号                  │-                         │-                         ┃
┃二次リセット応答                                  │-                         │CPRI番号                  │-                         │-                         ┃
┃強制停止通知                                      │-                         │CPRI番号                  │-                         │-                         ┃
┃RE起動完了通知                                    │-                         │CPRI番号                  │-                         │-                         ┃
┃RE時刻設定プロシージャ実行指示                    │-                         │CPRI番号                  │-                         │-                         ┃
┃REシステムパラメータ更新プロシージャ実行指示      │-                         │CPRI番号                  │-                         │-                         ┃
┃REファイル更新プロシージャ実行指示                │-                         │CPRI番号                  │-                         │-                         ┃
┃Toffset値報告プロシージャ実行指示                 │-                         │CPRI番号                  │-                         │-                         ┃
┃Active設定プロシージャ実行指示                    │-                         │CPRI番号                  │-                         │-                         ┃
┃RE装置構成情報報告プロシージャ実行指示            │-                         │CPRI番号                  │-                         │-                         ┃
┃REスロット情報報告プロシージャ実行指示            │-                         │CPRI番号                  │-                         │-                         ┃
┃RE状態報告確プロシージャ実行指示                  │-                         │CPRI番号                  │-                         │-                         ┃
┃RE PORT状態報告プロシージャ実行指示               │-                         │CPRI番号                  │-                         │-                         ┃
┃REカード状態報告(セクタ部)プロシージャ実行指示    │-                         │CPRI番号                  │-                         │-                         ┃
┃REカード状態報告(共通部)プロシージャ実行指示      │-                         │CPRI番号                  │-                         │-                         ┃
┃REキャリア状態報告プロシージャ実行指示            │-                         │CPRI番号                  │-                         │-                         ┃
┃MTアドレス設定(FLD-MT)プロシージャ実行指示        │-                         │CPRI番号                  │-                         │-                         ┃
┃MTアドレス設定(SV-MT)プロシージャ実行指示         │-                         │CPRI番号                  │-                         │-                         ┃
┃RE起動完了プロシージャ実行指示                    │-                         │CPRI番号                  │-                         │-                         ┃
┃REリセットプロシージャ実行指示                    │-                         │CPRI番号                  │-                         │-                         ┃
┃RE強制リセットプロシージャ実行指示                │-                         │CPRI番号                  │-                         │-                         ┃
┃強制停止プロシージャ実行指示                      │-                         │CPRI番号                  │-                         │-                         ┃
┃TRX設定(自律)プロシージャ実行指示                 │-                         │CPRI番号                  │キャリア番号              │-                         ┃
┃TRX解放(自律)プロシージャ実行指示                 │-                         │CPRI番号                  │キャリア番号              │-                         ┃
┃無変調キャリアON/OFF指定(自律)プロシージャ実行指示│-                         │CPRI番号                  │キャリア番号              │-                         ┃
┃REシステムパラメータ更新要求                      │-                         │CPRI番号                  │-                         │-                         ┃
┃REシステムパラメータ更新応答                      │-                         │CPRI番号                  │-                         │結果コード                ┃
┃REファイル更新要求                                │-                         │CPRI番号                  │-                         │-                         ┃
┃REファイル更新応答                                │-                         │CPRI番号                  │-                         │結果コード                ┃
┃その他イベント                                    │-                         │-                         │-                         │-                         ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━┷━━━━━━━━━━━━━┷━━━━━━━━━━━━━┷━━━━━━━━━━━━━┷━━━━━━━━━━━━━┛
┏━━━━━━━━━━━━━━━━━━━━━━━━━┯━━━━━━━━━━━━━┯━━━━━━━━━━━━━┯━━━━━━━━━━━━━┯━━━━━━━━━━━━━┓
┃プロセス間イベント                                │信号種別                  │CPRI番号                  │スロット番号orキャリア番号│結果コード                ┃
┠─────────────────────────┼─────────────┼─────────────┼─────────────┼─────────────┨
┃TRX設定要求                                       │-                         │CPRI番号                  │キャリア番号              │-                         ┃
┃TRX設定応答                                       │-                         │CPRI番号                  │-                         │NG要因                    ┃
┃無変調キャリアON/OFF指定要求                      │-                         │CPRI番号                  │キャリア番号              │-                         ┃
┃無変調キャリアON/OFF指定応答                      │-                         │CPRI番号                  │-                         │NG要因                    ┃
┃REカード制御要求                                  │-                         │-                         │スロット番号              │-                         ┃
┃REカード制御応答                                  │-                         │-                         │スロット番号              │結果コード                ┃
┃REカード状態変化通知                              │-                         │-                         │スロット番号              │-                         ┃
┃RE状態管理テーブル初期化通知                      │要因                      │CPRI番号                  │-                         │-                         ┃
┃RE再開完了通知                                    │要因(D.C.)                │CPRI番号                  │-                         │-                         ┃
┃RE状態收集要求                                    │-                         │CPRI番号                  │-                         │-                         ┃
┃RE状態收集応答                                    │Link状態                  │CPRI番号                  │-                         │-                         ┃
┃RE共通部リンク確立状態変化通知                    │Active/Negative状態       │CPRI番号                  │-                         │-                         ┃
┃バージョン報告要求                                │対象指定                  │AMP/OFTRX                 │スロット番号              │-                         ┃
┃バージョン報告応答                                │対象指定                  │AMP/OFTRX                 │スロット番号              │結果コード                ┃
┃REリセット要求                                    │-                         │AMP/OFTRX                 │-                         │-                         ┃
┃REリセット要求                                    │-                         │AMP/OFTRX                 │-                         │結果コード                ┃
┃CPRIリンクリセット要求                            │リセット種別              │CPRI番号                  │-                         │-                         ┃
┃CPRIリンクリセット応答                            │リセット種別              │CPRI番号                  │-                         │結果コード                ┃
┃その他イベント                                    │-                         │-                         │-                         │-                         ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━┷━━━━━━━━━━━━━┷━━━━━━━━━━━━━┷━━━━━━━━━━━━━┷━━━━━━━━━━━━━┛
*/

