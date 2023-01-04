
/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_recrdssc_a_ini.c
 * @brief  RE状態(セクタ部)管理テーブル(REカード状態)初期化
 * @date   2007/03/20 FJT)Nagasima create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007-2016
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RE状態(セクタ部)管理テーブル(REカード状態)初期化
 * @note   RE状態(セクタ部)管理テーブル(REカード状態)を初期化する
 *         - 
 *           - 初期化,CPRIリンク断以外の場合
 *             - 処理終了
 *         - 初期化
 * @param  ink    [in]  初期化種別
 * @return 変化
 * @retval 0:変化なし
 * @retval 1:変化あり
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/21 FFCS)Shihzh modify for S3G PRC.
 * @date   2009/09/18 FFCS)Shihzh M-S3G-eNBPF-02549.
 * @date   2010/07/27 FJT)Tokunaga M-S3G-eNBPF-03644.
 * @date   2010/08/11 CMS)Takahashi M-S3G-eNBSYS-01815.
 * @date   2011/09/30 FJT)Tokunaga CeNB-F-075-040(DCM) REの機能部故障状態についての確認事項
 * @date   2016/10/06 KCN)Takagi 16B機能追加
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_recrdssc_a_ini(                             /* 変化:R */
    USHORT ink                                              /* 初期化種別:I */
) {
	T_RRH_CARDST_S3G	tCardState = {};
// [16B] add start
    USHORT              cnt;
// [16B] add end

	tCardState.signal_kind = 0;
// [16B] chg start
	f_cmn_com_cardst_s3g_set( D_REC_C02_CPR_NO(), &tCardState );
    // REカード機能部情報を初期化(16スロット)
    for ( cnt = 0; cnt < CMD_SLOTINF_MAX; cnt++ ) 
    {
        f_cmn_com_cardstslt_s3g_set( D_REC_C02_CPR_NO(), cnt, &tCardState );

    }
// [16B] chg end

#if 0
	USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    USHORT          ampoft_no;
    USHORT          ampoft_idx;                    /* AMP/OFTRXインデックス */
    T_REC_RECRDSTS* tblp;                   /* テーブルREカード状態アドレス */
    T_REC_RECRDSTS* tbkp;               /* テーブル(BK)REカード状態アドレス */
    T_REC_RESLTINF* sltp;                       /* 受信REカード状態アドレス */
    USHORT          crdsts_idx;                   /* カード状態インデックス */
    USHORT          fb_idx;                       /* 機能部情報インデックス */
    USHORT          fb_idx_bk;
    USHORT          sltfb_flg;
    INT             cmp_ret;                                    /* 比較結果 */
    struct bpf_tm   tm;                                     /* 年月日時分秒 */
    USHORT          year_bcd;                              /* 年(BCDコード) */
    USHORT          mo___bcd;                            /* 月__(BCDコード) */
    USHORT          __dy_bcd;                            /* __日(BCDコード) */
    USHORT          mody_bcd;                            /* 月日(BCDコード) */
    USHORT          hour_bcd;                              /* 時(BCDコード) */
    USHORT          mn___bcd;                            /* 分__(BCDコード) */
    USHORT          __sc_bcd;                            /* __秒(BCDコード) */
    USHORT          mnsc_bcd;                            /* 分秒(BCDコード) */

    USHORT          crr_cnt;

	USHORT			slot_no;
	USHORT			fb_type;
    UINT   uit_ret;                                      /* 終了コード */
	T_REC_CARDOBJ*	cardPropNode;
	T_REC_CPRILINKOBJ*  cpriLinkObj;
	USHORT          multi_ret;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* 初期化,CPRIリンク断以外の場合 */
    if ( (ink != D_REC_INK_INI) && 
         (ink != D_REC_INK_DSC) ) {
        /* 初期化必要なし */
        /* 終了 */
        return CMD_OFF;
    }
    
//    /* CPRILinkオブジェクト取得 */
//    cpriLinkObj = f_trp_rec_FCpriLink_Obj_GetByNo(D_REC_C02_CPR_NO());
//    
//    cardPropNode = f_trp_rec_FCpriLink_AllCardList_GetFirst(cpriLinkObj);
//    
//    while(cardPropNode != NULL)
//    {
//        /*****************************************************************/
//        /* 自律リセット保護用タイマ / RE自律リセット制御状態クリア処理   */
//        /*****************************************************************/
//        f_trp_rec_Fc02_t_autorst_ccl(cardPropNode);
//        
//        /**************/
//        /* タイマ取消 */
//        /**************/
//        f_trp_rec_Fc02_t_timer_ccl(                                   /* なし:R */
//            EC_REC_TMK_REAUTRST_WAT,          /* RE auto Reset 送信待ちタイマ:I */
//            cardPropNode->slotNo                              /* スロット番号:I */
//        );
//        
//        cardPropNode = f_trp_rec_FCpriLink_AllCardList_GetNext(cpriLinkObj,cardPropNode);
//        
//    }
//    
//    /* 複数SLOTのREの有無判定 */
//    multi_ret = f_trp_rec_Fc02_t_single_or_multi_get();
//    
//    if( multi_ret == CMD_ON )
//    {
//        // 複数スロットの場合は処理分岐
//        uit_ret = f_trp_rec_Fc02_t_recrd_a_multi_ini(    /* 変化:R */
//            ink                                          /* 初期化種別 */
//        );
//
//        return uit_ret;
//    }
//
//    /*************************/
//    /* CPRI番号→AMP/OFTRX変換 */
//    /*************************/
//    ampoft_no = f_trp_rec_Fcom_t_cpr_ampoft_cnv(         /* AMP/OFTRX NO:R */
//        D_REC_C02_CPR_NO()                                    /* CPRI番号:I */
//    );
//    ampoft_idx = ampoft_no - CMD_NUM1;

    /* テーブルREカード状態アドレス抽出 */
    tblp = &(f_trp_rec_Wcom_remsc_tbl.remsc[cpr_no_idx].recrdsts);
    sltp = &(f_trp_rec_Wcom_remsc_tbl.remsc[cpr_no_idx].resltinf);

    /* テーブル(BK)REカード状態アドレス抽出 */
    tbkp = &(f_trp_rec_Wcom_remsc_tbk.remsc[cpr_no_idx].recrdsts);
    /* テーブル→テーブル(BK)にコピー */
    f_trp_com_Fmemcpy(                                      /* 終了コード:R */
        tbkp,                                   /* (BK)カード状態アドレス:I */
        tblp,                                       /* カード状態アドレス:I */
        sizeof(*tbkp)                             /* (BK)カード状態サイズ:I */
    );

    /**********/
    /* 初期化 */
    /**********/
    /* カード分ループ */
	crdsts_idx = CMD_NUM0; 

	/* メモリ初期化(スロット番号以外) */
	f_trp_com_Fmemclr(                                  /* 終了コード:R */
		tblp->sts[crdsts_idx].crd_name,         /* カード状態アドレス:I */
		sizeof(tblp->sts[crdsts_idx]) - 
		sizeof(tblp->sts[crdsts_idx].slt_no)      /* カード状態サイズ:I */
	);

	crr_cnt = CMD_NUM0;
	/* 機能部数のLoop */
	for(fb_idx = CMD_NUM0; fb_idx < CMD_NUM12; fb_idx ++)
	{
		sltfb_flg = D_REC_GETBIT(sltp->inf[crdsts_idx].crdcap, f_trp_rec_Rcom_sltfbinf_tbl.cnv[fb_idx].fb_bit);
		if (sltfb_flg == CMD_OFF)
		{
			/* 報告対象外機能部であり、skip */
			continue;
		}
		
		tblp->sts[crdsts_idx].fbsts[tblp->sts[crdsts_idx].fb_num].fb_typ = f_trp_rec_Rcom_sltfbinf_tbl.cnv[fb_idx].fb_typ;
		if(ink == D_REC_INK_DSC)
		{
			/**********************/
			/* 故障状態の初期設定 */
			/**********************/
			slot_no = tblp->sts[crdsts_idx].slt_no;
			fb_type = tblp->sts[crdsts_idx].fbsts[tblp->sts[crdsts_idx].fb_num].fb_typ;
			for(fb_idx_bk = CMD_NUM0; fb_idx_bk < tbkp->sts[crdsts_idx].fb_num; fb_idx_bk++)
			{
				if(tbkp->sts[crdsts_idx].fbsts[fb_idx_bk].fb_typ == fb_type)
				{
					break;
				}
			}
			
			if(ampoft_no == CMD_AMPNO_1)
			{
				/*************/
				/* BREルート */
				/*************/
				/* 故障状態 = 65535(NG) */
				f_trp_rec_Wcom_secfbsts_tbl.slot_sv[ slot_no - 1 ].fbsts[ fb_type ].trb_sts = D_REC_FBTRB_NG;
				
				f_trp_rec_Wcom_secfbsts_tbl.slot_sv[ slot_no - 1 ].sector = tbkp->sts[crdsts_idx].fbsts[fb_idx_bk].crr_sv[crr_cnt].sct_no;
				f_trp_rec_Wcom_secfbsts_tbl.slot_sv[ slot_no - 1 ].fbsts[ fb_type ].fb_bsvtrm = CMD_NUM0;
				f_trp_rec_Wcom_secfbsts_tbl.slot_sv[ slot_no - 1 ].fbsts[ fb_type ].fb_esvtrm = CMD_NUM0;
				f_trp_rec_Wcom_secfbsts_tbl.slot_sv[ slot_no - 1 ].fbsts[ fb_type ].sec_num = CMD_NUM1;
				
				f_trp_rec_Wcom_secfbsts_tbl.slot_sv[ slot_no - 1 ].set_flag = CMD_ON;
			}
			else
			{
				/*****************/
				/* RRE/LREルート */
				/*****************/
				/* ★BREと類似処理だが、現状BREでも故障状態以外の本テーブルのパラメータの使用用途がない	*/
				/*   そのため、RRE/LREでは故障状態しかテーブルデータを持たない							*/
				/* ※既存の処理に影響がない様に本処理を追加しているが、時間が取れるならテーブルの持ち方	*/
				/*   等を変更して、BRE含めf_trp_rec_Wcom_remsc_tbl等に組み込む形に修正した方が良い		*/
				f_trp_rec_Wcom_secfbsts_tbl.slot_sv_oftrx[D_REC_C02_CPR_NO() - 1][ slot_no - 1 ].fbsts[ fb_type ].trb_sts = D_REC_FBTRB_NG;
			}
			
			/* 機能部種別がDUP/LNA/T-PA/TRX/TRX-INF/3GRF-INFの場合 */
			if( (tblp->sts[crdsts_idx].fbsts[tblp->sts[crdsts_idx].fb_num].fb_typ == D_REC_FBTYP_DUP)		||
				(tblp->sts[crdsts_idx].fbsts[tblp->sts[crdsts_idx].fb_num].fb_typ == D_REC_FBTYP_LNA)		||
				(tblp->sts[crdsts_idx].fbsts[tblp->sts[crdsts_idx].fb_num].fb_typ == D_REC_FBTYP_TPA)		||
				(tblp->sts[crdsts_idx].fbsts[tblp->sts[crdsts_idx].fb_num].fb_typ == D_REC_FBTYP_TRX)		||
				(tblp->sts[crdsts_idx].fbsts[tblp->sts[crdsts_idx].fb_num].fb_typ == D_REC_FBTYP_TRXINF)	||
				(tblp->sts[crdsts_idx].fbsts[tblp->sts[crdsts_idx].fb_num].fb_typ == D_REC_FBTYP_3GRFINF)	){
				/* セクタ・キャリア数 = 1(固定) */
				tblp->sts[crdsts_idx].fbsts[tblp->sts[crdsts_idx].fb_num].crr_num = CMD_NUM1;
			}
			/* 機能部種別がOA-RA-INF/TILT-INF/PORT-INF/MT-INF/EX-OA-RA-INF/CLK-INFの場合 */
			else{
				/* セクタ・キャリア数 = 0(固定) */
				tblp->sts[crdsts_idx].fbsts[tblp->sts[crdsts_idx].fb_num].crr_num = CMD_NUM0;
			}
			/* セクタ番号 */
			tblp->sts[crdsts_idx].fbsts[tblp->sts[crdsts_idx].fb_num].crr_sv[crr_cnt].sct_no = tbkp->sts[crdsts_idx].fbsts[fb_idx_bk].crr_sv[crr_cnt].sct_no;
			/* キャリア番号 = 1(固定) */
			tblp->sts[crdsts_idx].fbsts[tblp->sts[crdsts_idx].fb_num].crr_sv[crr_cnt].crr_no = CMD_NUM1;
			/* システム帯域幅 */
			tblp->sts[crdsts_idx].fbsts[tblp->sts[crdsts_idx].fb_num].crr_sv[crr_cnt].crr_bnd = tbkp->sts[crdsts_idx].fbsts[fb_idx_bk].crr_sv[crr_cnt].crr_bnd;
			/* 機能部基本監視項目 = 0(固定) */
			tblp->sts[crdsts_idx].fbsts[tblp->sts[crdsts_idx].fb_num].crr_sv[crr_cnt].crr_bsvtrm = CMD_NUM0;
			/* 機能部拡張監視項目 = 0(固定) */
			tblp->sts[crdsts_idx].fbsts[tblp->sts[crdsts_idx].fb_num].crr_sv[crr_cnt].crr_esvtrm = CMD_NUM0;
			/* ブランチ数 */
			tblp->sts[crdsts_idx].fbsts[tblp->sts[crdsts_idx].fb_num].crr_sv[crr_cnt].brc_num = CMD_NUM0;
		}
		/* 機能部数 */
		tblp->sts[crdsts_idx].fb_num++;
	}

    /* 時刻取得 */
    f_trp_com_Fget_tim(&tm);
    year_bcd = D_REC_CNVBCD(tm.tm_year+1900);
    mo___bcd = D_REC_CNVBCD(tm.tm_mon +   1);
    __dy_bcd = D_REC_CNVBCD(tm.tm_mday     );
    mody_bcd = (mo___bcd<<8) + __dy_bcd;
    hour_bcd = D_REC_CNVBCD(tm.tm_hour     );
    mn___bcd = D_REC_CNVBCD(tm.tm_min      );
    __sc_bcd = D_REC_CNVBCD(tm.tm_sec      );
    mnsc_bcd = (mn___bcd<<8) + __sc_bcd;

    /****************************************/
    /* 新旧比較(の前に時刻を対象外にしとく) */
    /****************************************/
    /* カード分ループ */
    for (crdsts_idx = CMD_NUM0; crdsts_idx < sltp->num; crdsts_idx++) {
        tblp->sts[crdsts_idx].y  = year_bcd;                          /* 年 */
        tblp->sts[crdsts_idx].md = mody_bcd;                        /* 月日 */
        tblp->sts[crdsts_idx].h  = hour_bcd;                 /* (未使用+)時 */
        tblp->sts[crdsts_idx].ms = mnsc_bcd;                        /* 分秒 */

        tbkp->sts[crdsts_idx].y  = tblp->sts[crdsts_idx].y;
        tbkp->sts[crdsts_idx].md = tblp->sts[crdsts_idx].md;
        tbkp->sts[crdsts_idx].h  = tblp->sts[crdsts_idx].h;
        tbkp->sts[crdsts_idx].ms = tblp->sts[crdsts_idx].ms;
    }
//    /************/
//    /* 新旧比較 */
//    /************/
//    BPF_RU_MACC_BCMP(                                       /* 終了コード:R */
//        tbkp,                                    /* (BK)Toffset値アドレス:I */
//        tblp,                                        /* Toffset値アドレス:I */
//        sizeof(*tbkp),                             /* (BK)Toffset値サイズ:I */
//        &cmp_ret                                              /* 比較結果:O */
//    );
//
//    /* 変化なしの場合 */
//    if (cmp_ret == CMD_NUM0) {
//        return CMD_OFF;
//    }
//    
//    /************************************/
//    /* Update RE Function Block Status  */
//    /************************************/
//    cmp_ret = f_trp_rec_Fc02_t_refbsts_gen(
//    );
//    /* 変化なしの場合 */
//    if (cmp_ret == CMD_NUM0) {
//        return D_REC_OTRSTS_CHG;
//    }
#endif   
    return D_REC_RFBSTS_CHG;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
