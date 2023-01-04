/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_recrdssc_i_merge.c
 * @brief  RE状態(セクタ部)管理テーブル(REカード状態3G)集約(マージ)
 * @date   2016/10/05 KCN)Fukushima create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2016
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */
extern USHORT gw_3g_alm_code_re[CMD_MAX_ALM_NUM];
extern USHORT gw_3g_err_code_re[CMD_MAX_ERR_NUM];

/****************************************************************************/
/*!
 * @brief  RE状態(セクタ部)管理テーブル(REカード状態3G)集約(マージ)
 * @note   RE状態(セクタ部)管理テーブル(REカード状態3G)を集約(マージ)する
 * @param  なし
 * @return None
 * @date   2016/10/05 KCN)Fukushima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_recrdssc_i_merge_3g(                        /* 変化:R */
) {

	USHORT cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
	USHORT lwTotalCardSvitem = CMD_NUM0;		/* カード基本監視頁E岼			*/
	USHORT lwTotalCardExtSvitem = CMD_NUM0;		/* カード拡張監視頁E岼			*/
	USHORT lwTotalCardCntitem = CMD_NUM0;		/* カード制御項目 */
	USHORT lwTotalCntCode = CMD_NUM0;			/* 制御コード					*/
	USHORT lwTotalCarInf = CMD_NUM0;			/* キャリア情報					*/
	USHORT slot_num;
	USHORT slot_cnt;
	USHORT alm_num = CMD_NUM0;					/* 発生ALM数					*/
	USHORT err_num = CMD_NUM0;					/* 発生ERR数					*/
	USHORT lwCpyAlmNum;
	USHORT lwCpyErrNum;
	T_RRH_CARDST_3G	ltCardStaSlt3G;				/* (3G)REカード状態(Slot毎)		*/
	T_RRH_CARDST_3G	ltCardSta3G;				/* (3G)REカード状態 			*/

	cm_MemClr(gw_3g_alm_code_re, sizeof(gw_3g_alm_code_re));
	cm_MemClr(gw_3g_err_code_re, sizeof(gw_3g_err_code_re));

	/* スロット数を抽出しておく */
    slot_num = f_trp_rec_Wcom_remsc_tbl.remsc[cpr_no_idx].resltinf.slt_num;

	/* REカード状態報告(Port集約用)管理テーブルにより最新のREカード状態を集約 */
	for( slot_cnt = CMD_NUM0; slot_cnt < slot_num ; slot_cnt++ )
	{
		/* 共有メモリ(3G)装置構成情報(全Slot)の呼び出し 取得用 */
		f_cmn_com_cardstslt_3g_get( cpr_no_idx + CMD_NUM1, slot_cnt, &ltCardStaSlt3G );
		/* 信号種別が0の場合はDon't Care */
		if(( CMD_NUM0 == ltCardStaSlt3G.signal_kind ))
		{
			continue;
		}

		if (CMD_OK != ltCardStaSlt3G.result)
		{
		   continue;
		}

		/* カード基本監視項目 */
		lwTotalCardSvitem |= ltCardStaSlt3G.card_svitem;
		/* カード拡張監視項目 */
		lwTotalCardExtSvitem |= ltCardStaSlt3G.card_extsvitem;
		/* カード制御項目 */
		lwTotalCardCntitem |= ltCardStaSlt3G.card_cntitem;
		/* 制御コード */
		lwTotalCntCode     |= ltCardStaSlt3G.cnt_code;
		/* キャリア情報 */
		lwTotalCarInf |=  ltCardStaSlt3G.car_inf;
		/* RE# ALM */
		if( ( alm_num + ltCardStaSlt3G.alm_num ) > CMD_MAX_ALM_NUM )
		{
			lwCpyAlmNum = CMD_MAX_ALM_NUM - alm_num;
		}
		else
		{
			lwCpyAlmNum = ltCardStaSlt3G.alm_num;
		}
		/* 発生アラーム情報取得 */
		cm_MemCpy(	&gw_3g_alm_code_re[alm_num],
					&ltCardStaSlt3G.alm_inf[CMD_NUM0],
					sizeof(USHORT) * lwCpyAlmNum);
		alm_num += lwCpyAlmNum;

		/* RE# ERR */
		if( ( err_num + ltCardStaSlt3G.err_num ) > CMD_MAX_ERR_NUM )
		{
			lwCpyErrNum = CMD_MAX_ERR_NUM - err_num;
		}
		else
		{
			lwCpyErrNum = ltCardStaSlt3G.err_num;
		}
		/* エラーコード取得 */
		cm_MemCpy(	&gw_3g_err_code_re[err_num],
					&ltCardStaSlt3G.err_inf[CMD_NUM0],
					sizeof(USHORT) * lwCpyErrNum);
		err_num += lwCpyErrNum;
	}

	ltCardSta3G.signal_kind    = ltCardStaSlt3G.signal_kind; /* pgr0872 pgr0000 */
	ltCardSta3G.result         = ltCardStaSlt3G.result;      /* pgr0872 */
	ltCardSta3G.card_svitem    = lwTotalCardSvitem;
	ltCardSta3G.card_extsvitem = lwTotalCardExtSvitem;
	ltCardSta3G.card_cntitem   = lwTotalCardCntitem;
	ltCardSta3G.cnt_code       = lwTotalCntCode;
	ltCardSta3G.car_inf        = lwTotalCarInf;

	/*******************************************/
	/* ALM情報数/ALM情報/ERR情報数/ERR情報設定 */
	/*******************************************/
	/*
	* FHMもしくは配下REのいずれかでALMが通知された場合、ALM情報数1、ALMコード0xFFを報告する。
	* FHM、またすべての配下REについてALMが発生していない場合、ALM情報数0で報告する。
	* つまり、ALM情報数は0か1しかない。
	*/
	cm_MemClr(&ltCardSta3G.alm_inf[CMD_NUM0], sizeof(gw_3g_alm_code_re));
	if (CMD_NUM0 == alm_num)
	{
		ltCardSta3G.alm_num        = CMD_NUM0;
	}
	else
	{
		ltCardSta3G.alm_num        = CMD_NUM1;
		ltCardSta3G.alm_inf[CMD_NUM0] = 0xFF;
	}

	cm_MemClr(&ltCardSta3G.err_inf[CMD_NUM0], sizeof(gw_3g_err_code_re));
	ltCardSta3G.err_num        = err_num;
	cm_MemCpy(	&ltCardSta3G.err_inf[CMD_NUM0],
				&gw_3g_err_code_re[CMD_NUM0],
				sizeof(USHORT) * err_num);

	/* 共有メモリ(3G)装置構成情報(集約結果)の呼び出し 取得用 */
	f_cmn_com_cardst_3g_set( cpr_no_idx + CMD_NUM1, &ltCardSta3G );
    
    return;

}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
