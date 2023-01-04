/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Rc03_m_idle_sts.c
 * @brief  Event : [REキャリア状態報告応答] Status: TRX制御状態がアイドル
 * @date
 */
/****************************************************************************/
#include "f_trp_rec.h"
/****************************************************************************/
/*!
 * @brief  Event : [REキャリア状態報告応答(LTE)] Status: TRX制御状態がアイドル
 * @param  bufp [in] バッファポインタ
 * @Bug_No M-RRU-ZSYS-01761
 * @Bug_No M-RRU-ZSYS-01810
 * @date   2015/08/20 FPT)Dung create
 * @date   2015/10/01 FPT)Yen fix bug IT2
 * @date   2015/10/09 TDIPS)Lay M-RRU-ZSYS-01761 TRX設定/解放の条件を修正
 * @date   2015/10/20 TDI)satou M-RRU-ZSYS-01810 問処番号No.116 キャリア状態不一致検出するキャリア数は最大キャリア数で行うように
 * @date   2015/11/08 FPT)Yen fix bug IT3 No58.
 * @date   2015/11/11 FPT)Yen Fix bug IT3 No70
 * @date   2015/11/20 FPT)Yen Update
 */
/****************************************************************************/
VOID f_trp_rec_Rc03_m_idle_sts(VOID *bufp) {
	printf("\n====================[INFO] [L3] [%s:%d] Enter function", __FILE__, __LINE__);
    
    T_RRH_LAYER3                    ltLayer3;
    CMT_TSKIF_CPRISND_CARLSTSRES    *rcvp;                            /* 受信REキャリア状態アドレス */
    USHORT                          cpr_idx       = D_REC_C03_CPR_NO() - CMD_NUM1;     /* CpriINDEX */
    USHORT                          crr_idx       = D_REC_C03_CRR_NO() - CMD_NUM1; /* キャリアINDEX */
    T_TRXSET_PARA_LTE               shmTrxSet;

    /* Layer3状態取得 */
    f_cmn_com_layer3_get(CMD_SYS_LTE, &ltLayer3);
    /* 当該REのLayer3状態が「運用中状態」以外 */
    if( E_RRH_LAYER3_RE_OPE != ltLayer3.layer3_re[cpr_idx] )
    {
        /* Layer3状態が「運用中状態」の場合のみ、TRX設定/解放要求を配下REに送信したいため、*/
        /* 起動シーケンスにおけるキャリア状態報告では不一致補正しない。                    */
        return;
    }
    /* 受信REカード状態アドレス抽出 */
    rcvp = (CMT_TSKIF_CPRISND_CARLSTSRES*)bufp;

    /* 応答結果チェック*/
    if( CMD_NML != rcvp->cpridat_carstsres.result)
    {
        return;
    }

    /****************************************************/
    /* 使用中ビットON／受信メッセージにキャリア情報なし */
    /* →REC:TRX設定済み／RE:TRX未設定のアンマッチ      */
    /****************************************************/
    memset(&shmTrxSet, 0, sizeof(shmTrxSet));
    f_cmn_com_trxset_para_lte_get(crr_idx+1, &shmTrxSet);

    /* TrxSet,TrxRelease is running */
    if ( E_RRH_TRXSTA_IDLE != shmTrxSet.wStatus )
    {
        /* キャリア状態不一致補正未処理にする */
        return;
    }

    if ((shmTrxSet.wSigKind != 0) && (f_trp_rec_Wcom_reCarrUseSts[cpr_idx][crr_idx] != CMD_RESSTBIT_USE)) {
        /* RE/RECからのメッセージをREに送信する(TRX設定要求) */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "Setting car status(LTE) is mismatch(SET <-> NOUSE)");
        /* L3(TRX設定要求)送信 */
        f_trp_rec_Fc03_t_l3_txsreq_snd();
        /*テーブル設定TRX制御状態 ← TRX設定(自律)中*/
        f_trp_rec_Fc03_t_ownstn_set(EC_REC_C03_STN_TXSAING);
        /*テーブル設定キャリア状態不一致補正カウンタ ← 0回*/
        f_trp_rec_Wc03_stsmng_tbl.sts[cpr_idx][crr_idx].tc.txs = EC_REC_C03_TRXSET_RTRY_TIME;
    }
    /*****************************************************/
    /* 使用中ビットOFF／受信メッセージにキャリア情報あり */
    /* →REC:TRX未設定／RE:TRX設定済みのアンマッチ       */
    /*****************************************************/
    else if ((shmTrxSet.wSigKind == 0) && (f_trp_rec_Wcom_reCarrUseSts[cpr_idx][crr_idx] == CMD_RESSTBIT_USE)) {
        /* RE/RECからのメッセージをREに送信する(TRX解放要求) */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "Setting car status(LTE) is mismatch(NOSET <-> USE)");
        /* L3(TRX解放要求)送信 */
        f_trp_rec_Fc03_t_l3_txrreq_snd();
        /* テーブル設定TRX制御状態 ← TRX設定(自律)中 */
        f_trp_rec_Fc03_t_ownstn_set(EC_REC_C03_STN_TXRAING);
        /* テーブル設定キャリア状態不一致補正カウンタ ← 0回 */
        f_trp_rec_Wc03_stsmng_tbl.sts[cpr_idx][crr_idx].tc.txr = EC_REC_C03_TRXREL_RTRY_TIME;
    }
}

/****************************************************************************/
/*!
 * @brief  Event : [REキャリア状態報告応答(3G)] Status: TRX制御状態がアイドル
 * @note
 * @param  bufp [in] バッファポインタ
 * @date   2015/08/20 FPT)Dung create
 * @date   2015/10/12 FPT)Dung update
 * @date   2015/10/20 TDI)satou 問処番号No.116水平展開
 * @date   2015/10/24 TDIPS)sasaki Warning対処
 * @date   2015/11/08 FPT)Yen fix bug IT3 No58.
 * @date   2015/11/11 FPT)Yen Fix bug IT3 No70
 * @date   2015/11/20 FPT)Yen Update
 */
/****************************************************************************/
VOID f_trp_rec_Rc03_m_idle_sts_3g(VOID *bufp){
    T_RRH_LAYER3                    ltLayer3;
    CMT_TSKIF_CPRISND_CARLSTSRES    *rcvp;                            /* 受信REキャリア状態アドレス */
    USHORT                          cpr_idx    = D_REC_C03_3G_CPR_NO() - CMD_NUM1;     /* CpriINDEX */
    USHORT                          crr_idx    = D_REC_C03_3G_CRR_NO() - CMD_NUM1; /* キャリアINDEX */
    T_TRXSET_PARA_3G                shmTrxSet;

    /* Layer3状態取得 */
    f_cmn_com_layer3_get(CMD_SYS_3G, &ltLayer3);
    /* 当該REのLayer3状態が「運用中状態」以外 */
    if( E_RRH_LAYER3_RE_OPE != ltLayer3.layer3_re[cpr_idx] )
    {
        /* Layer3状態が「運用中状態」の場合のみ、TRX設定/解放要求を配下REに送信したいため、*/
        /* 起動シーケンスにおけるキャリア状態報告では不一致補正しない。                    */
        return;
    }
    /* 受信REカード状態アドレス抽出 */
    rcvp = (CMT_TSKIF_CPRISND_CARLSTSRES*)bufp;

    /* 応答結果チェック*/
    if( CMD_NML != rcvp->cpridat_carstsres.result)
    {
        return;
    }

    /****************************************************/
    /* 使用中ビットON／受信メッセージにキャリア情報なし */
    /* →REC:TRX設定済み／RE:TRX未設定のアンマッチ      */
    /****************************************************/
    f_cmn_com_trxset_para_3g_get(crr_idx+1, &shmTrxSet);
    /* TrxSet,TrxRelease is running */
    if ( E_RRH_TRXSTA_IDLE != shmTrxSet.wStatus )
    {
        /* キャリア状態不一致補正未処理にする */
        return;
    }

    if ((shmTrxSet.wSigKind != 0) && (f_trp_rec_Wcom_reCarrUseSts_3g[cpr_idx][crr_idx] != CMD_RESSTBIT_USE)) {
        /* RE/RECからのメッセージをREに送信する(TRX設定要求) */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "Setting car status(3G) is mismatch(SET <-> NOUSE)");
        /* L3(TRX設定要求)送信 */
        f_trp_rec_Fc03_t_l3_txsreq_snd_3g();
        /*テーブル設定TRX制御状態 ← TRX設定(自律)中*/
        f_trp_rec_Fc03_t_ownstn_set_3g(EC_REC_C03_STN_TXSAING);
        /*テーブル設定キャリア状態不一致補正カウンタ ← 0回*/
        f_trp_rec_Wc03_stsmng_tbl_3g.sts[cpr_idx][crr_idx].tc.txs = EC_REC_C03_TRXSET_RTRY_TIME;
    }
    /*****************************************************/
    /* 使用中ビットOFF／受信メッセージにキャリア情報あり */
    /* →REC:TRX未設定／RE:TRX設定済みのアンマッチ       */
    /*****************************************************/
    else if ((shmTrxSet.wSigKind == 0) &&  (f_trp_rec_Wcom_reCarrUseSts_3g[cpr_idx][crr_idx] == CMD_RESSTBIT_USE)) {
        /* RE/RECからのメッセージをREに送信する(TRX解放要求) */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "Setting car status(3G) is mismatch(NOSET <-> USE)");
        /* L3(TRX解放要求)送信 */
        f_trp_rec_Fc03_t_l3_txrreq_snd_3g();
        /* テーブル設定TRX制御状態 ← TRX設定(自律)中 */
        f_trp_rec_Fc03_t_ownstn_set_3g(EC_REC_C03_STN_TXRAING);
        /* テーブル設定キャリア状態不一致補正カウンタ ← 0回 */
        f_trp_rec_Wc03_stsmng_tbl_3g.sts[cpr_idx][crr_idx].tc.txr = EC_REC_C03_TRXREL_RTRY_TIME;
    }
}

