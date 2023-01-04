/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_recrrsts_set.c
 * @brief  RE状態(セクタ部)管理テーブル(REキャリア状態)設定
 * @date   2008/07/21 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2010
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */
/****************************************************************************/
/*!
 * @brief  RE状態(セクタ部)管理テーブル(REキャリア状態)設定
 * @note   RE状態(セクタ部)管理テーブル(REキャリア状態)を設定する
 *         - 受信値設定
 * @param  l3_bufp [in]  L3バッファポインタ
 * @return 変化
 * @retval 0:変化なし
 * @retval 1:変化あり
 * @Bug_No M-RRU-ZSYS-01819
 * @date   2008/07/21 FFCS)Shihzh create.
 * @date   2009/09/07 FFCS)Shihzh M-S3G-eNBPF-02475
 * @date   2010/12/09 CMS)Kudou modify for M-S3G-eNBPF-03928
 * @date   2015/08/01 FPT)Yen modify
 * @date   2015/10/05 FPT)Yen fix bug IT2
 * @date   2015/10/21 TDI)satou M-RRU-ZSYS-01819 問処番号No.121水平展開
 * @date   2015/10/27 FPT)Yen fix bug IT3 No36
 * @date   2015/10/29 FPT)Yen fix bug IT3 No39
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_recrrsts_set(                               /* 変化:R */
    VOID* l3_bufp                                   /* L3バッファポインタ:I */
) {
    CMT_CPRIF_CARLSTSRES   *rcvp;             /* 受信REキャリア状態アドレス */
    T_RRH_CARST_S3G_CAR_EVERY           *crrsts_infp;            /* キャリア情報ポインタ */
    USHORT  cpr_idx         = D_REC_C02_CPR_NO() - CMD_NUM1;     /* CpriINDEX */
    USHORT  crr_idx         = CMD_NUM0;                    /* キャリアINDEX */
    USHORT  crr_no          = CMD_NUM0;                     /* キャリア番号 */
    USHORT  rcv_brcnum      = CMD_NUM0;                      /*  ブランチ数 */
    USHORT  rcv_alminfnum   = CMD_NUM0;                        /* ALM情報数 */
    USHORT  rcv_crdinfnum   = CMD_NUM0;                         /* カード数 */
    USHORT  alm_cnt         = CMD_NUM0;              /* ALM数ループカウンタ */
    USHORT  slt_cnt         = CMD_NUM0;         /* スロット数ループカウンタ */
    USHORT  *offset         = CMD_NUM0;                   /* 参照オフセット */
    USHORT  crr_cnt         = CMD_NUM0;             /* キャリアインデックス */
    USHORT  brc_cnt         = CMD_NUM0;             /* ブランチインデックス */
    USHORT  nonset_almnum   = CMD_NUM0;              /* 設定対象外ALM情報数 */
    T_TRXSET_PARA_LTE       shmTrxSet;
    T_RRH_CARST_S3G         shmCarSt = {0};

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* 受信REカード状態アドレス抽出 */
    rcvp = (CMT_CPRIF_CARLSTSRES*)l3_bufp;

	memset( f_trp_rec_Wcom_reCarrUseSts[cpr_idx], 0, sizeof( f_trp_rec_Wcom_reCarrUseSts[cpr_idx] ) );
	/* 信号種別 */
	shmCarSt.signal_kind = rcvp->signal_kind;
	/* 応答結果 */
	shmCarSt.result = rcvp->result;
	/* キャリア番号 */
	shmCarSt.car_num = rcvp->car_num;
//TBD    /*  Del Carrier status RPT NG/TO ALM  */
//TBD    for (crr_idx = CMD_NUM0; crr_idx < CMD_CARNO_MAX; crr_idx++) {
//TBD        crrsts_infp = &(f_trp_rec_Wcom_remsc_tbl.remsc[cpr_idx].recrrsts.sts[crr_idx]);
//TBD        if (D_REC_GETBIT(crrsts_infp->crrsts, CMD_RESSTBIT_ALM) != CMD_ON){
//TBD            continue;
//TBD        }
//TBD        if ((crrsts_infp->alminf[CMD_NUM0] == D_REC_CRR_ALM_RNG) ||
//TBD			(crrsts_infp->alminf[CMD_NUM0] == D_REC_CRR_ALM_DSC) ||
//TBD			(crrsts_infp->alminf[CMD_NUM0] == D_REC_CRR_ALM_RTO) ){
//TBD            f_trp_com_Fmemclr(                              /* 終了コード:R */
//TBD                &(tblp->sts[crr_idx]),          /* REキャリア状態アドレス:I */
//TBD                sizeof(tblp->sts[crr_idx])        /* REキャリア状態サイズ:I */
//TBD            );
//TBD            tblp->sts[crr_idx].crr_no = crr_idx + CMD_NUM1;
//TBD        }
//TBD    }


    /* キャリア状態詳細先頭情報オフセット設定 */
    offset = &(rcvp->car_num);
	/* キャリア番号OFFSET算出 */
	offset++;

    /*******************************************/
    /* 正常CPRI 信号受信（自キャリア情報あり） */
    /*******************************************/
    /* キャリア数分ループする */
    for (crr_cnt = CMD_NUM0; crr_cnt < rcvp->car_num; crr_cnt++) {
        /* ループカウンタの該当するキャリア番号を取得 */
        crr_no = *offset;

    	/* キャリアインデックス設定(0) */
        crr_idx = (USHORT)(crr_no - CMD_NUM1);

    	/* キャリア状態 */
    	f_trp_rec_Wcom_reCarrUseSts[cpr_idx][crr_idx] = *(offset + 1) & CMD_RESSTBIT_USE;

        /* キャリア情報ポインタ設定 */
        crrsts_infp = &(shmCarSt.car_every[crr_idx]);

        /* キャリア状態の使用中ビットがOFFの場合 */
        f_cmn_com_trxset_para_lte_get(crr_no, &shmTrxSet);
        if (shmTrxSet.wSigKind == 0) {
          /* テーブル設定はせず、次のキャリア情報用に、オフセットのみずらす */
            offset += CMD_NUM3;                   /* キャリア状態OFFSET算出 */
            offset ++;                              /* ブランチ数OFFSET算出 */
            rcv_brcnum   = *offset;                   /* ブランチ情報数取得 */
            offset += (rcv_brcnum * CMD_NUM4);
        	offset ++;                               /* ALM情報数OFFSET算出 */
	        rcv_alminfnum = *offset;                       /* ALM情報数取得 */
	        offset += rcv_alminfnum;         /* ALM情報(最終位置)OFFSET算出 */
	        offset ++;                                /* カード数OFFSET算出 */
	        rcv_crdinfnum = *offset;                        /* カード数取得 */
	        offset += rcv_crdinfnum;    /* スロット番号(最終位置)OFFSET算出 */
            offset ++;          /* 次のキャリア情報のキャリア番号OFFSET算出 */
            continue;
        }

        /* 設定対象外ALM情報数初期化 */
        nonset_almnum = CMD_NUM0;
        
        /************************/
        /* キャリア番号設定     */
        /************************/
        shmCarSt.car_every[crr_idx].car_num = crr_no;

        /************************/
        /* キャリア状態設定     */
        /************************/
        /* キャリア状態OFFSET算出 */
        offset += CMD_NUM1;
        crrsts_infp->car_st    = (*offset) & CMD_RESSTBIT_VBIT;
        offset += CMD_NUM1;
        crrsts_infp->car_st_ul = (*offset) & CMD_RESSTBIT_VBIT;
        offset += CMD_NUM1;
        crrsts_infp->car_st_dl = (*offset) & CMD_RESSTBIT_VBIT;

        /******************/
        /* ブランチ数設定 */
        /******************/
        /* ブランチ数OFFSET算出 */
        offset += CMD_NUM1; 
        rcv_brcnum = *offset;
        crrsts_infp->br_num = rcv_brcnum;
        /********************/
        /* ブランチ情報設定 */
        /********************/
        for (brc_cnt = CMD_NUM0; brc_cnt < rcv_brcnum; brc_cnt++) {
            /* ブランチ番号OFFSET算出 */
            offset += CMD_NUM1; 
            /* ブランチ番号設定 */
            crrsts_infp->br_every[brc_cnt].br_num = *offset;
            /* ブランチ状態OFFSET算出 */
            offset += CMD_NUM1; 
            /* ブランチ状態設定 */
            crrsts_infp->br_every[brc_cnt].car_st    = (*offset) & CMD_RESSTBIT_VBIT;
            offset += CMD_NUM1; 
            crrsts_infp->br_every[brc_cnt].car_st_ul = (*offset) & CMD_RESSTBIT_VBIT;
            offset += CMD_NUM1; 
            crrsts_infp->br_every[brc_cnt].car_st_dl = (*offset) & CMD_RESSTBIT_VBIT;
        }
	    /************************/
	    /* ALM情報数設定        */
	    /************************/
	    /* ALM情報数OFFSET算出 */
	    offset += CMD_NUM1;        
	    /* ALM情報数設定                                                    */
	    rcv_alminfnum = *offset;

	    /********************************************************************/
	    /* ALM情報設定                                                      */
	    /* ALM情報数の最大値は255で最大でALM情報を255個有効として受信する。 */
	    /* RE状態管理テーブルでは最大16個の情報保持領域しかないため、       */
	    /* ALM情報数が16以上の場合は17個目からのALM情報を破棄する。         */
	    /********************************************************************/
	    if (rcv_alminfnum > CMD_ALM_NUM ) {
	        /* ALM情報数をテーブル格納可能な16とする */
	        shmCarSt.car_every[crr_idx].alm_num = CMD_ALM_NUM;
	        /* 設定対象外ALM情報数を算出する */
	        nonset_almnum = (USHORT)(rcv_alminfnum - CMD_ALM_NUM);
	    }
	    else {
	        shmCarSt.car_every[crr_idx].alm_num = rcv_alminfnum;
	    }
	    /* ALM情報数分ループ */
	    for (alm_cnt = CMD_NUM0; alm_cnt < shmCarSt.car_every[crr_idx].alm_num; alm_cnt++) {
	        /* ALM情報OFFSET算出 */
	        offset += CMD_NUM1;
	        /* ALM情報設定 */
	        shmCarSt.car_every[crr_idx].alm_inf[alm_cnt] = *offset;
	    }

	    /************************/
	    /* カード数設定         */
	    /************************/
	    /* カード数OFFSET算出 */
	    offset += CMD_NUM1; 
	    /* カード数設定 */
	    rcv_crdinfnum = *offset;
	    shmCarSt.car_every[crr_idx].card_num = rcv_crdinfnum;

	    /************************/
	    /* スロット番号設定        */
	    /************************/
	    for(slt_cnt = CMD_NUM0; slt_cnt < rcv_crdinfnum; slt_cnt++) {
	        /* スロット番号OFFSET算出 */
	        offset += CMD_NUM1;            
	        /* スロット番号ソート設定 */
	        shmCarSt.car_every[crr_idx].slot_num[slt_cnt] = *offset;
	    }
        offset ++;          /* 次のキャリア情報のキャリア番号OFFSET算出 */
    }

	/* キャリア状態(S3G)設定 */
	f_cmn_com_carst_s3g_set( D_REC_C02_CPR_NO(), &shmCarSt);

	return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
