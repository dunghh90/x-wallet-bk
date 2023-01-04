/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_recrrsts_a_ini.c
 * @brief  RE状態(セクタ部)管理テーブル(REキャリア状態)初期化
 * @date   2008/07/21 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RE状態(セクタ部)管理テーブル(REキャリア状態)初期化
 * @note   RE状態(セクタ部)管理テーブル(REキャリア状態)を初期化する
 *         - 
 *           - 初期化,CPRIリンク断,運用中アイドル状態HC OK,報告確認T.O,報告NG以外の場合
 *             - 処理終了
 *         - 初期化
 * @param  ink    [in]  初期化種別
 * @return 変化
 * @retval 0:変化なし
 * @retval 1:変化あり
 * @date   2008/07/21 FFCS)Shihzh create.
 * @date   2009/09/11 FFCS)Shihzh M-S3G-eNBPF-02484.
 * @date   2011/02/10 FJT)Tokunaga CeNB-F-062-033(DCM) 運用パラメータの反映元と反映タイミング
 * @date   2015/08/11 fpt)tan Modify.
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_recrrsts_a_ini_3g(                             /* 変化:R */
    USHORT ink                                              /* 初期化種別:I */
) {
	T_RRH_CARST_3G  ltCarSta3g = {};
	
	
//	/* get share memmory */
//	f_cmn_com_carst_3g_get(D_REC_C02_3G_CPR_NO(), &ltCarSta3g);

	ltCarSta3g.signal_kind = 0;
	
	/* set share memmory */
	f_cmn_com_carst_3g_set( D_REC_C02_3G_CPR_NO(), &ltCarSta3g);
	
	return CMD_NUM0;

}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
