/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_resltinf_set_3g.c
 * @brief  RE状態(セクタ部/共通部)管理テーブル(REスロット情報)設定
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
 * @brief  RE状態(セクタ部/共通部)管理テーブル(REスロット情報)設定
 * @note   RE状態(セクタ部/共通部)管理テーブル(REスロット情報)を設定する
 *         - 受信値設定
 * @param  l3_bufp [in]  L3バッファポインタ
 * @return 変化
 * @retval 0:変化なし
 * @retval 1:変化あり
 * @date   2008/07/21 FFCS)Shihzh create.
 * @date   2010/09/10 CMS)Takahashi modify for CeNB-F-065-003
 * @date   2011/06/14 CMS)Yorozuya TS-14項 M-S3G-eNBSYS-02395
 * @date   2016/10/04 KCN)hfuku 16B機能追加
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_resltinf_set_3g(                               /* 変化:R */
    VOID* l3_bufp                                   /* L3バッファポインタ:I */
) {
    T_REC_RESLTINF*		tblp_sec;         /* テーブルREスロット情報アドレス */
    T_REC_RESLTINF*		tbkp;     /* テーブル(BK)REスロット情報アドレス */
    USHORT		 		cpr_idx   = D_REC_C02_3G_CPR_NO() - CMD_NUM1;    /* CPRI番号INDEX  */
    USHORT				rcv_slotno[CMD_NUM16];     /* スロット番号(0クリア) 16B KCN chg */
    USHORT				rcv_cardfnc[CMD_NUM16]; /* REカード機能部情報(0クリア)  16B KCN chg */
    USHORT				*offset;                                     /* 参照オフセット  */
    INT					cmp_ret   = CMD_NUM0;                               /* 比較結果 */
    USHORT				rcv_slotnum;
    USHORT				rcv_result;                           /* スロット数 16B KCN add */
    USHORT				slt_cnt;            /* スロット数分のループカウンタ 16B KCN add */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* テーブルREスロット情報アドレス抽出 */
    tblp_sec = &(f_trp_rec_Wcom_remsc_tbl_3g.remsc[cpr_idx].resltinf);
    /* テーブル(BK)REスロット情報アドレス抽出 */
    tbkp = &(f_trp_rec_Wcom_remsc_tbk_3g.remsc[cpr_idx].resltinf);
    /* テーブル→テーブル(BK)にコピー */
    f_trp_com_Fmemcpy(                                      /* 終了コード:R */
        tbkp,                               /* (BK)REスロット情報アドレス:I */
        tblp_sec,                                   /* REスロット情報アドレス:I */
        sizeof(*tblp_sec)                         /* (BK)REスロット情報サイズ:I */
    );

    /****************************************************/
    /* RE状態管理スロットテーブル初期化処理(セクタ部) */
    /****************************************************/
    f_trp_rec_Fc02_t_resltinfsc_ini_3g(D_REC_INK_INI);
    
    /************************************************************************/
    /* REスロット情報格納処理                                               */
    /************************************************************************/
    /* 受信REスロットアドレス抽出 */
    offset = &(((CMT_CPRIF_SLOTINFRES*)l3_bufp)->result);
    /* 応答結果 */
    rcv_result = *offset;

    /* スロット情報数OFFSET算出 */
    offset += CMD_NUM1;

    /* スロット情報数設定 16B KCN add*/
    rcv_slotnum = *offset;

    /* 16B KCN chg */
    for (slt_cnt = 0 ; slt_cnt < rcv_slotnum ; slt_cnt++)
    {
        /* スロット番号OFFSET算出 */
        offset += CMD_NUM1;
        /* スロット番号設定 */
        rcv_slotno[slt_cnt] = *offset;
        /* REカード機能部情報OFFSET算出 */
        offset += CMD_NUM1;
        /* REカード機能部情報設定 */
        rcv_cardfnc[slt_cnt] = *offset; 
    }

    /* REセクタ部の機能部を持つスロットはセクタ部テーブルに登録する */
    /**************************/
    /* 信号種別設定       */
    /**************************/
    f_trp_rec_Wcom_remsc_tbl_3g.remsc[cpr_idx].resltinf.signal_kind = 
        ((CMT_CPRIF_SLOTINFRES*)l3_bufp)->signal_kind;

    /**************************/
    /* 応答結果設定       */
    /**************************/
    f_trp_rec_Wcom_remsc_tbl_3g.remsc[cpr_idx].resltinf.result = rcv_result;

    /**************************/
    /* Slot数設定 16B KCN add */
    /**************************/
    f_trp_rec_Wcom_remsc_tbl_3g.remsc[cpr_idx].resltinf.slt_num = rcv_slotnum;

    /* 16B KCN chg */
    for (slt_cnt = 0 ; slt_cnt < rcv_slotnum ; slt_cnt++)
    {
        /**************************/
        /* 全スロット番号を保存   */
        /**************************/
        f_trp_rec_Wcom_remsc_tbl_3g.remsc[cpr_idx].resltinf.slt_no[slt_cnt] = rcv_slotno[slt_cnt]; /* pgr0000 */
        /******************************/
        /* 全REカード機能部情報を保存 */
        /******************************/
        f_trp_rec_Wcom_remsc_tbl_3g.remsc[cpr_idx].resltinf.crdcap[slt_cnt] = rcv_cardfnc[slt_cnt]; /* pgr0000 */
        /***********************************************/
        /* スロット番号に対応したIndex値を格納しておく */
        /***********************************************/
        f_trp_rec_Wcom_remsc_tbl_3g.remsc[cpr_idx].resltinf.slt_idx[rcv_slotno[slt_cnt] - 1] = slt_cnt; /* pgr0000 */
    }
	
	/* 共有メモリ(3G)スロット情報の呼び出し設定用 */
	f_cmn_com_slot_3g_set( cpr_idx+1,(T_RRH_SLOT_3G_RE*)&f_trp_rec_Wcom_remsc_tbl_3g.remsc[cpr_idx].resltinf.signal_kind );
	
    /**********************/
    /* セクタ部の新旧比較 */
    /**********************/
    BPF_RU_MACC_BCMP(                                   /* 終了コード:R */
        tbkp,                           /* (BK)REスロット情報アドレス:I */
        tblp_sec,                               /* REスロット情報アドレス:I */
        sizeof(*tblp_sec),                    /* (BK)REスロット情報サイズ:I */
        &cmp_ret                                          /* 比較結果:O */
    );

    /* 変化なしの場合 */
    if (cmp_ret == CMD_NUM0) {
        return CMD_OFF;
    }


    return D_REC_OTRSTS_CHG;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
