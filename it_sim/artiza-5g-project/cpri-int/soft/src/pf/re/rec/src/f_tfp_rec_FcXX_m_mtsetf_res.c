/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_mtsetf_req.c
 * @brief  RE起動中MTアドレス設定応答処理
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RE起動中MTアドレス設定応答(REC主導)受信処理
 * @note   RE起動中MTアドレス設定応答(REC主導)受信処理を行う
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/07/30 FPT)HieuNT
 * @date   2015/09/22 FPT)Yen update
 * @date   2015/09/27 TDI)satou f_trp_rec_Fc02_m_res_mtsetres_rec()がループ回数呼ばれる不具合を修正
 * @date   2015/09/22 FPT)Yen add 3G
 * @date   2015/11/08 TDIPS)Takeuchi 技説QA120
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_mtsetf_res(                             /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
	USHORT		lwLoop;		/* */
	USHORT		lwSts;
	USHORT		lwCpriNo;
	USHORT		lwResult;
	USHORT		lwSysType;
	USHORT		lwFactor;

#ifdef FHM_DEBUG_CXX_FOR_IT1
	printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

	/*対象配下REのMTアドレス設定状態←応答結果*/
	lwCpriNo = ((CMT_TSKIF_CPRIRCV_MTADDSETRES *)bufp)->cprircv_inf.link_num;
	lwSysType = ((CMT_TSKIF_CPRIRCV_MTADDSETRES *)bufp)->cpridat_mtaddsetres.signal_kind & CMD_SYS_CHK_CPRID;
	if( CMD_SYS_LTE == lwSysType )
	{
		lwFactor = f_trp_rec_Fc02_t_tc_mtsetffactor_get(lwCpriNo);
		lwSts = f_trp_rec_Fc02_t_mtgetsts( lwCpriNo );
	}
	else
	{
		lwFactor = f_trp_rec_Fc02_t_tc_mtsetffactor_get_3g(lwCpriNo);
		lwSts = f_trp_rec_Fc02_t_mtgetsts_3g( lwCpriNo );
	}

	if( D_MT_ADD_SET_FACTOR_REC != lwFactor )
	{
#ifdef FHM_DEBUG_CXX_FOR_IT1
	printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif
		return;
	}

	if( EC_REC_MTADDSET_STN_RUNNING != lwSts )
	{
#ifdef FHM_DEBUG_CXX_FOR_IT1
	printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif
		return;
	}
	/* check 応答結果 */
	lwResult = ((CMT_TSKIF_CPRIRCV_MTADDSETRES *)bufp)->cpridat_mtaddsetres.result;
	/* 正常の場合 */
	if( CMD_NML == lwResult )
	{
		lwSts = EC_REC_MTADDSET_STN_RESOK;
	}
	else
	{
#ifdef FHM_DEBUG_CXX_FOR_IT1
	printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif
		/* NG応答は破棄する。RCT検出TO契機による要求再送を期待。*/
		return;
	}

	if( CMD_SYS_LTE == lwSysType )
	{
		/* MTアドレス設定状態設定 */
		f_trp_rec_Fc02_t_mtsetsts( lwCpriNo, lwSts );
	}
	else
	{
		/* MTアドレス設定状態設定 */
		f_trp_rec_Fc02_t_mtsetsts_3g( lwCpriNo, lwSts );
	}

	/*Loop all RE*/
	for( lwLoop = CMD_CPRINO_MIN; lwLoop <= CMD_CPRINO_MAX; lwLoop++)
	{
		/*Get MTアドレス設定状態*/
		if( CMD_SYS_LTE == lwSysType )
		{
			lwSts = f_trp_rec_Fc02_t_mtgetsts( lwLoop );
		}
		else
		{
			lwSts = f_trp_rec_Fc02_t_mtgetsts_3g( lwLoop );
		}
		/*各対象配下REのMTアドレス設定状態の設定中がまだ残っている？*/
		if( EC_REC_MTADDSET_STN_RUNNING == lwSts )
		{
			return;
		}
	}

	/* MTアドレス設定要求(REC主導)再送回数初期化 */
	f_trp_rec_Wcom_MtAdSetRetryCnt[lwSysType] = 0;

	if( CMD_SYS_LTE == lwSysType )
	{
    	/*MTアドレス設定応答(REC)*/
    	f_trp_rec_Fc02_m_res_mtsetres_rec();
	}
	else
	{
		/*MTアドレス設定応答(REC)*/
    	f_trp_rec_Fc02_m_res_mtsetres_rec_3g();
	}
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
