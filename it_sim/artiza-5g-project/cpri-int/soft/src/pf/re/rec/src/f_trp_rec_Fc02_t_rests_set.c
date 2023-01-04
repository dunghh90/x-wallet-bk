/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_rests_set.c
 * @brief  RE状態(セクタ部)管理テーブル(RE状態)設定
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
 * @brief  RE状態(セクタ部)管理テーブル(RE状態)設定
 * @note   RE状態(セクタ部)管理テーブル(RE状態)を設定する
 *         - 受信値設定
 * @param  l3_bufp [in]  L3バッファポインタ
 * @return 変化
 * @retval 0:変化なし
 * @retval 1:変化あり
 * @date   2008/07/21 FFCS)Shihzh create.
 * @date   2015/10/07 FPT)DuongCD update comment
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_rests_set(                                  /* 変化:R */
    VOID* l3_bufp                                   /* L3バッファポインタ:I */
) {
    CMT_CPRIF_RESTSRES* rcvp;                         /* 受信RE状態アドレス */
    T_REC_RESTS*        tblp;                     /* テーブルRE状態アドレス */
    T_REC_RESTS*        tbkp;                 /* テーブル(BK)RE状態アドレス */
	T_RRH_REST_S3G      ltRestaShmSav;
    USHORT  re_serial_idx;                        /* REシリアルインデックス */
    USHORT  fan_cnt    = CMD_NUM0;                   /* FAN数ループカウンタ */
    USHORT  maker_cnt  = CMD_NUM0;                  /* メーカー固有RE情報数 */
    USHORT  *offset    = NULL;                            /* 参照オフセット */
    USHORT  fan_num    = CMD_NUM0;                             /* FAN情報数 */
    USHORT  maker_num  = CMD_NUM0;                  /* メーカー固有RE情報数 */
    INT     cmp_ret;                                            /* 比較結果 */
	USHORT  lwLoop;
	USHORT  cpri_no;
    
	//CPRI number
	cpri_no = D_REC_C02_CPR_NO();
	
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

	re_serial_idx = D_REC_C02_RE_SERIAL_IDX();

    /* 受信REカード状態アドレス抽出 */
    rcvp = (CMT_CPRIF_RESTSRES*)l3_bufp;
    /* テーブルRE状態アドレス抽出 */
    tblp = &(f_trp_rec_Wcom_remcm_tbl.remcm[re_serial_idx].rests);
    /* テーブル(BK)RE状態アドレス抽出 */
    tbkp = &(f_trp_rec_Wcom_remcm_tbk.remcm[re_serial_idx].rests);
    /* テーブル→テーブル(BK)にコピー */
    f_trp_com_Fmemcpy(                                      /* 終了コード:R */
        tbkp,                                       /* (BK)RE状態アドレス:I */
        tblp,                                           /* RE状態アドレス:I */
        sizeof(*tblp)                                 /* (BK)RE状態サイズ:I */
    );
	
	/* Activeリンク切替後の新Activeで変化通知をOAMに送信するため */
	/* M-LREの場合は応答を受信した側のRE番号を設定する */
	if(D_REC_C02_RE_SYS_TYPE() == D_TCM_SYS_RETYP_MLRE)
	{
		f_trp_rec_Wcom_remcm_tbl.remcm[re_serial_idx].rests.re_no = D_REC_C02_CPR_NO();
	}
	
    /* 受信値→テーブルにコピー */
    /****************/
    /* メーカ名設定 */
    /****************/
    f_trp_rec_Wcom_remcm_tbl.remcm[re_serial_idx].rests.mk_name 
        = rcvp->rests_inf.maker_name;

    /* RE状態詳細先頭情報オフセット設定 */
    offset = &(rcvp->rests_inf.maker_name);

    /* FAN情報数OFFSET算出 */
    offset += CMD_NUM1;
    
    /*****************/
    /* FAN情報数設定 */
    /*****************/
    fan_num = *offset;
    f_trp_rec_Wcom_remcm_tbl.remcm[re_serial_idx].rests.faninf_num 
        = fan_num;
    
    /************************************************************************/
    /* FAN情報設定                                                          */
    /************************************************************************/
    /* FAN情報数分ループ */
    for(fan_cnt = CMD_NUM0; fan_cnt < fan_num; fan_cnt++)
    {
        /* FAN情報OFFSET算出 */
        offset += CMD_NUM1;
        
        /* FAN情報設定 */
        f_trp_rec_Wcom_remcm_tbl.remcm[re_serial_idx].rests.faninf[fan_cnt] 
            = *offset;
    }
    
    /* 設定対象外FAN情報数分の未設定領域を初期化する */
    for(fan_cnt = fan_num; fan_cnt < CMD_FAN_NUM; fan_cnt++)
    {
        /* FAN情報設定 */
        f_trp_rec_Wcom_remcm_tbl.remcm[re_serial_idx].rests.faninf[fan_cnt] 
            = (USHORT)CMD_NODATA;
    }
    
    /* メーカー固有RE情報数OFFSET算出 */
    offset += CMD_NUM1;
    
    /************************************************************************/
    /* メーカー固有RE情報設定                                               */
    /************************************************************************/
    maker_num = *offset;
    f_trp_rec_Wcom_remcm_tbl.remcm[re_serial_idx].rests.mkreinf_num 
        = maker_num;
    
    /* メーカー固有RE情報数分ループ */
    for(maker_cnt = CMD_NUM0;maker_cnt < maker_num ;maker_cnt++)
    {
        /* メーカー固有RE情報OFFSET算出 */
        offset += CMD_NUM1;
        
        /* メーカー固有RE情報設定 */
        f_trp_rec_Wcom_remcm_tbl.remcm[re_serial_idx].rests.mkreinf[maker_cnt] 
            = *offset;
    }
    
    /* メーカー固有RE情報数分の未設定領域を初期化する */
    for(maker_cnt = maker_num; maker_cnt < CMD_MAKER_NUM; maker_cnt++) 
    {
        /* メーカー固有RE情報設定 */
        f_trp_rec_Wcom_remcm_tbl.remcm[re_serial_idx].rests.mkreinf[maker_cnt] 
            = (USHORT)CMD_NODATA;
    }
	
	//Get information to save to shared memory
	
	//Signal kind
	ltRestaShmSav.signal_kind = CMD_CPRID_RESTSRES + CMD_SYS_S3G;
	//Result
	ltRestaShmSav.result      = ((CMT_CPRIF_RESTSRES*)l3_bufp)->result;
	//maker name
	ltRestaShmSav.maker_name  = f_trp_rec_Wcom_remcm_tbl.remcm[re_serial_idx].rests.mk_name;
	//fan_num
	ltRestaShmSav.fan_num     = f_trp_rec_Wcom_remcm_tbl.remcm[re_serial_idx].rests.faninf_num;
	//fan information
	for(lwLoop = CMD_NUM0; lwLoop < CMD_FAN_NUM; lwLoop++)
	{
		ltRestaShmSav.fan_inf[lwLoop] = f_trp_rec_Wcom_remcm_tbl.remcm[re_serial_idx].rests.faninf[lwLoop];
	}
	//maker information num
	ltRestaShmSav.maker_inf_num = f_trp_rec_Wcom_remcm_tbl.remcm[re_serial_idx].rests.mkreinf_num;
	//maker information
	for(lwLoop = CMD_NUM0; lwLoop < CMD_MAKER_NUM; lwLoop++)
	{
		ltRestaShmSav.maker_inf[lwLoop] = f_trp_rec_Wcom_remcm_tbl.remcm[re_serial_idx].rests.mkreinf[lwLoop];
	}
	// Save Re status to Shared memory
    f_cmn_com_rest_s3g_set( cpri_no, &ltRestaShmSav);
    /************/
    /* 新旧比較 */
    /************/
    BPF_RU_MACC_BCMP(                                       /* 終了コード:R */
        tbkp,                                       /* (BK)RE状態アドレス:I */
        tblp,                                           /* RE状態アドレス:I */
        sizeof(*tblp),                                /* (BK)RE状態サイズ:I */
        &cmp_ret                                              /* 比較結果:O */
    );

    /* 変化なしの場合 */
    if (cmp_ret == CMD_NUM0) {
        return CMD_OFF;
    }
    /* 変化ありの場合 */
    else {
        return CMD_ON;
    }
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
