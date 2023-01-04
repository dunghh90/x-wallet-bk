/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_FirmFile_fw.c
 * @brief  Forward ファイルデータ送信完了報告通知	l3/dlm -> re/rec -> l2/lpb
 *				   ファイルデータ送信完了報告応答	l2/lpb -> re/rec -> l3/dlm
 * @date   2015/08/21 FPT)Yen create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015-2016
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */
/****************************************************************************/
/*!
 * @brief  Receive msg ファイルデータ送信完了報告通知 from l3/dlm
 * @note   Receive msg ファイルデータ送信完了報告通知 from l3/dlm
 *         - send [ファイルデータ送信完了報告通知] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @Bug_No M-RRU-ZSYS-01972
 * @date   2015/08/21 FPT)Yen create
 * @date   2015/11/09 FPT)Yen M-RRU-ZSYS-01972 fix bug IT2 No165 配下RE強制停止状態の場合は配下REへ転送しない
 * @date   2016/04/25 TDI)satou RE PORT状態報告送信間隔タイマの起動を追加
 * @date   2016/10/27 KCN)hfuku 16B機能追加
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_FilSndFinNtc_fw(                        /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
	USHORT		lwRet;
    /*	koshida try	*/
	USHORT		cpr_no_idx;
	CMT_TSKIF_CPRISND_FILSNDFINNTC		*msg_p;
	T_RRH_LAYER3	ltReLayer3Stas3g;
	UINT		logDat;

	if( NULL == bufp )
	{
		return;
	}

	lwRet = f_trp_rec_FcXX_m_frcstp_refilnewstp(bufp);
	if( CMD_OK == lwRet )
	{
		return;
	}
    /*	koshida try	*/
	msg_p = (CMT_TSKIF_CPRISND_FILSNDFINNTC*)bufp;
	cpr_no_idx = msg_p->cprisnd_inf.link_num-1;
	/*	RECからのファイルデータ送信完了報告通知(ファームウェア更新なし)の場合	*/
	if(msg_p->cpridat_filsndfinntc.tgtdat == CMD_FRM_NONEW)
	{
		(VOID)f_cmn_com_layer3_get( CMD_SYS_S3G, &ltReLayer3Stas3g );
		if((ltReLayer3Stas3g.layer3_rec == E_RRH_LAYER3_REC_OPE)&&(timeSetFlag[cpr_no_idx] != D_RRH_ON))
		{
			/*	周期監視タイマを生成	*/
			f_trp_rec_FcXX_timer_ent(EC_REC_TMK_RESTS_INT, msg_p->cprisnd_inf.link_num, D_REC_SLT_IV);
			
			f_trp_rec_FcXX_timer_ent(EC_REC_TMK_RECRDSSC_INT, msg_p->cprisnd_inf.link_num, D_REC_SLT_IV);
			
/* 16B KCN add */
			timeRecrdcscCount[cpr_no_idx] = CMD_NUM0;               /* REカード状態報告 送信間隔カウンタの初期設定 */
			timeRecrdSndCount[cpr_no_idx] = CMD_NUM0;               /* REカード状態報告 送信slot数カウンタの初期設定 */
			
			f_trp_rec_FcXX_timer_ent(EC_REC_TMK_RECRRSTS_INT, msg_p->cprisnd_inf.link_num, D_REC_SLT_IV);

			f_trp_rec_FcXX_timer_ent(EC_REC_TMK_REPRTSTS_INT, msg_p->cprisnd_inf.link_num, D_REC_SLT_IV);

			logDat = (UINT)((cpr_no_idx<<24)+(msg_p->cpridat_filsndfinntc.tgtdat<<16)+(ltReLayer3Stas3g.layer3_rec<<8)+timeSetFlag[cpr_no_idx]);
			cm_Assert(	CMD_ASL_DBGLOW, logDat, "f_trp_rec_FcXX_m_use_FilSndFinNtc_fw Timer Start");
			timeSetFlag[cpr_no_idx] = D_RRH_ON;
		}
	}
	/* Send msg [ファイルデータ送信完了報告通知] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_FILSNDFINNTC) );

    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg ファイルデータ送信完了報告応答 from L2/lpb
 * @note   Receive msg ファイルデータ送信完了報告応答 from L2/lpb
 *         - send [ファイルデータ送信完了報告応答] to l3/dlm
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/08/21 FPT)Yen create
 * @date   2015/10/17 TDI)satou FHM技説-QA-013
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_FilSndFinRes_fw(                        /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [ファイルデータ送信完了報告応答] to l3/dlm */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, ((CMT_TSKIF_CPRISND_FILSNDFINRES*)bufp)->head.uiLength );

    return;
}

/*	koshida try	*/
UINT f_trp_rec_FcXX_timer_ent(                            /* 終了コード:R */
    ET_REC_TMK tmk,                                       /* タイマ種別:I */
    USHORT cpr_no,                                        /* CPRIリンク番号:I */
    USHORT slt_no                                         /* スロット番号:I */
)
{
    INT                       sit_ret;                        /* 終了コード */
    UINT                      uit_ret;                        /* 終了コード */
    T_REC_TIMELM              elm;                            /* タイマ要素 */
    bpf_ru_hrtm_key_inf_t     key_inf;                    /* タイマキー情報 */
    bpf_ru_hrtm_notify_type_t ntc_type;                         /* 通知種別 */
    UINT                      tim_id;                       /* タイマ識別子 */
    UINT                      tim_val;                          /* タイマ値 */
    INT                       errcd;                        /* エラーコード */
    CHAR                      errdata[CMD_NUM48];

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* 要素設定 */
    elm.cls              = EC_REC_C02;                            /* クラス */
    elm.tmk              = tmk;                               /* タイマ種別 */
	elm.cpr_no = cpr_no;
    elm.soc_no = slt_no;                                    /* スロット番号 */

    /****************************/
    /* タイマ要素→キー情報変換 */
    /****************************/
    f_trp_rec_Fcom_t_elm_key_cnv(                                 /* なし:R */
        &elm,                                                     /* 要素:I */
        &key_inf                                              /* キー情報:O */
    );
	
    /**************************/
    /* タイマ管理テーブル検索 */
    /**************************/
    tim_id = f_trp_rec_Fcom_t_timmng_tbl_sch(             /* タイマ識別子:R */
        &key_inf                                              /* キー情報:O */
    );
    /* 一致した場合 */
    if (tim_id != CMD_NUM0) {
    	return CMD_OK;
    }

    tim_id = f_trp_rec_Fcom_t_timmng_tbl_get(
        elm                                                 /* タイマ要素:I  */
    );
	/* Time Id not found */
    if (CMD_NUM0 == tim_id) {
    	return CMD_NG;
    }
    /****************************/
    /* タイマ種別→通知種別変換 */
    /****************************/
    ntc_type = f_trp_rec_Fcom_t_tmk_ntc_cnv(                  /* 通知種別:R */
        tmk                                                 /* タイマ種別:I */
    );

    /****************************/
    /* タイマ種別→タイマ値変換 */
    /****************************/
/* [16B] */
//	if((tmk == EC_REC_TMK_RECRDSSC_INT) || (tmk == EC_REC_TMK_RECRRSTS_INT)){
//		tim_val = CMD_NUM2 * CMD_NUM100;
//	}
	if(tmk == EC_REC_TMK_RECRDSSC_INT){
		tim_val = CMD_NUM2 * CMD_NUM10;
	}
	else if(tmk == EC_REC_TMK_RECRRSTS_INT){
		tim_val = CMD_NUM2 * CMD_NUM100;
	}
/* [16B] */
	else
	{
		tim_val = CMD_NUM10 * CMD_NUM100;
    }
    /**************/
    /* タイマ登録 */
    /**************/
    sit_ret = f_trp_com_Fregister_tim(                      /* 終了コード:R */
        ntc_type,                                             /* 通知種別:I */
        tim_val,                                              /* タイマ値:I */
        key_inf,                                              /* キー情報:I */
        tim_id,                                           /* タイマ識別子:I */
        &errcd                                            /* 詳細NGコード:O */
    );
    /* タイマ登録NGの場合 */
    if (sit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][slt:0x%02x][err:%d]Timer ent NG.", cpr_no, slt_no, errcd);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(&key_inf, sizeof(key_inf));
        D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
        return CMD_NG;
    }

    /**************************/
    /* タイマ管理テーブル追加 */
    /**************************/
    uit_ret = f_trp_rec_Fcom_t_timmng_tbl_add(              /* 終了コード:R */
        &key_inf,                                             /* キー情報:I */
        tim_id                                            /* タイマ識別子:I */
    );
    /* タイマ管理テーブル追加NGの場合 */
    if (uit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][slt:0x%02x][ret:0x%02x]Timer table ent NG.",cpr_no, slt_no, uit_ret); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(&key_inf, sizeof(key_inf));
        D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
        return CMD_NG;
    }

    return CMD_OK;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
