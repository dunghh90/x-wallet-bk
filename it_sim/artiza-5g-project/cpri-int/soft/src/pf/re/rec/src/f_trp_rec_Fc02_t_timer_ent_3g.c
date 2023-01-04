/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_timer_ent_3g.c
 * @brief  タイマ登録
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
 * @brief  タイマ登録
 * @note   タイマを登録する
 *         - タイマ登録
 *         - タイマ管理テーブル追加
 * @param  tmk    [in]  タイマ種別
 * @param  slt_no [in]  スロット番号
 * @return 終了コード
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2010/04/07 FJT)Tokunaga CR-XXX-XXX 局データに65535が設定された場合の動作(API Ver.1.3.7)
 * @date   2010/11/12 FJT)Tokunaga M-S3G-eNBSYS-xxxxx [TS-810項]
 * @date   2015/12/04 FPT)Quynh Fix bug [IT3]RE ALM検出時の自律リセットタイミングが早い
 * @date   2016/10/04 KCN)hfuku 16B:EC_REC_TMK_RECRDSSC_INTの間隔を300->30に変更
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_timer_ent_3g(                            /* 終了コード:R */
    ET_REC_TMK tmk,                                         /* タイマ種別:I */
    USHORT slt_no                                         /* スロット番号:I */
) {
    INT                       sit_ret;                        /* 終了コード */
    UINT                      uit_ret;                        /* 終了コード */
    T_REC_TIMELM              elm;                            /* タイマ要素 */
    bpf_ru_hrtm_key_inf_t     key_inf;                    /* タイマキー情報 */
    bpf_ru_hrtm_notify_type_t ntc_type;                         /* 通知種別 */
    UINT                      tim_id;                       /* タイマ識別子 */
    UINT                      tim_val;                          /* タイマ値 */
    INT                       errcd;                        /* エラーコード */
    CHAR                      errdata[CMD_NUM48];
	UINT					  logDat;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* 要素設定 */
    elm.cls              = EC_REC_C02_3G;                         /* クラス */
    elm.tmk              = tmk;                               /* タイマ種別 */
	elm.cpr_no = D_REC_C02_3G_CPR_NO();
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
    if (tmk == EC_REC_TMK_REAUTRST_WAT) {
		tim_val = CMD_NUM60 * CMD_NUM1000;	/*	Typical値(60秒);REカードへ自律リセットを実施するまでのタイマ	*/

		snprintf(errdata, CMD_NUM48, "[NTC]RE AUTO RST timer start");
		D_REC_IVLINI();
		logDat = D_REC_C02_3G_CPR_NO();
		D_REC_IVLSET_SIZE(&logDat, sizeof(logDat));
		logDat = elm.tmk;
		D_REC_IVLSET_SIZE(&logDat, sizeof(logDat));
		logDat = slt_no;
		D_REC_IVLSET_SIZE(&logDat, sizeof(logDat));
		logDat = tim_val;
		D_REC_IVLSET_SIZE(&logDat, sizeof(logDat));
		D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
    }
	else if(tmk == EC_REC_TMK_RECRDSSC_INT){
		tim_val = CMD_NUM3 * CMD_NUM10;       /* 16B 300ms->30ms KCN change */
	}
	else if(tmk == EC_REC_TMK_RECRRSTS_INT){
		tim_val = CMD_NUM3 * CMD_NUM100;
	}
	else if((tmk == EC_REC_TMK_RECRDSSC_CNF) || (tmk == EC_REC_TMK_RECRRSTS_CNF))
	{
		tim_val = CMD_NUM2 * CMD_NUM100;
	}
	else if (tmk == EC_REC_TMK_ACTCHG_WAT){
		if(D_REC_C02_ACTLNK_GR() != D_TCM_ACTIVE_LINK_GROUP_BRE)
		{
			/* M-LRE case */
			tim_val = f_trp_rec_Fcom_t_tmk_val_cnv(                       /* タイマ値:R */
          	  EC_REC_TMK_ACTCHG_WAT2 + D_REC_C02_ACTLNK_GR() - CMD_NUM2   /* タイマ種別:I */
     	   );
		}
		else
		{
			tim_val = f_trp_rec_Fcom_t_tmk_val_cnv(               /* タイマ値:R */
          	  tmk                                               /* タイマ種別:I */
     	   );
		}
	}
    else {
        tim_val = f_trp_rec_Fcom_t_tmk_val_cnv(               /* タイマ値:R */
            tmk                                             /* タイマ種別:I */
        );
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
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][slt:0x%02x][err:%d]Timer ent NG.", D_REC_C02_3G_CPR_NO(), slt_no, errcd);
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
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][slt:0x%02x][ret:0x%02x]Timer table ent NG.", D_REC_C02_3G_CPR_NO(), slt_no, uit_ret); /* pgr0570 */
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
