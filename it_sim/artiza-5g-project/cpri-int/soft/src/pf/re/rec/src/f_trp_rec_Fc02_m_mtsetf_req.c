/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_mtsetf_req.c
 * @brief  RE起動中MTアドレス設定(FLD-MT)開始処理
 * @date   2007/03/20 FJT)Nagasima create.
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RE起動中MTアドレス設定(REC主導)受信処理
 * @note   RE起動中MTアドレス設定(REC主導)受信処理を行う
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/07/30 FPT)HieuNT
 * @date   2015/09/22 FPT)Yen Update
 * @date   2015/10/07 FPT)Tuan Fix bug IT1
 * @date   2015/10/26 FPT)Yen Fix bug IT2 No 140
 * @date   2015/11/08 TDIPS)Takeuchi 技説QA120
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_mtsetf_req(                             /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
	T_RRH_LAYER3	ltLayer3Sts;	/* Layer3状態 */
	USHORT		sigsys;
	USHORT		lwCpri;		/* */
	USHORT 		sendFlg = CMD_FALSE;
#ifdef FHM_DEBUG_CXX_FOR_IT1
	USHORT		i;
#endif

	/**********************************************/
	/* 配下REへ(MTアドレス設定要求(REC主導))中継 */
	/**********************************************/
	sigsys = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->signal_kind & CMD_SYS_CHK_CPRID;
	/* 共用メモリLayer3の呼び出し 取得用 */
	if( CMD_SYS_3G == sigsys)
	{
		f_cmn_com_layer3_get(CMD_SYS_3G, &ltLayer3Sts);
	}
	else
	{
		f_cmn_com_layer3_get(CMD_SYS_LTE, &ltLayer3Sts);
	}

#ifdef FHM_DEBUG_CXX_FOR_IT1
    for( i = 0; i< D_RRH_CPRINO_RE_MAX; i++)
    {
        printf( "%02x ", ltLayer3Sts.layer3_re[i]);
    }
    printf( "\n" );
#endif
	/*  CPRIリンク数だけループ */
	for(lwCpri = CMD_NUM0; lwCpri < D_RRH_CPRINO_RE_MAX; lwCpri++)
	{
		/* 対RE Layer3状態が「②RE起動中状態」または「④運用中状態」の場合 */
		if(( CMD_NUM2 == ltLayer3Sts.layer3_re[ lwCpri ] )||( CMD_NUM4 == ltLayer3Sts.layer3_re[ lwCpri ] ))
		{
#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "%s %s %d <lwCpri>%d\n", __FILE__, __FUNCTION__, __LINE__, lwCpri);
#endif
			((CMT_TSKIF_CPRIRCV_MTADDSETREQ *)bufp)->cprircv_inf.link_num = lwCpri + CMD_NUM1;

			/* l2/lpbへ転送 */
			f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_MTADDSETREQ) );

			if( CMD_SYS_3G == sigsys)
			{
				/* Set MTアドレス設定状態 */
				f_trp_rec_Fc02_t_mtsetsts_3g( lwCpri+CMD_NUM1, EC_REC_MTADDSET_STN_RUNNING );
				/* MTアドレス設定開始要因設定: REC主導*/
				f_trp_rec_Fc02_t_tc_mtsetffactor_set_3g( lwCpri+CMD_NUM1, D_MT_ADD_SET_FACTOR_REC );
#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( " %s %d <lwCpri>%d\n MTアドレス設定開始要因設定: REC主導\n ", __FUNCTION__, __LINE__, lwCpri);
#endif
			}
			else
			{
				/* Set MTアドレス設定状態 */
				f_trp_rec_Fc02_t_mtsetsts( lwCpri+CMD_NUM1, EC_REC_MTADDSET_STN_RUNNING );
				/* MTアドレス設定開始要因設定: REC主導*/
				f_trp_rec_Fc02_t_tc_mtsetffactor_set( lwCpri+CMD_NUM1, D_MT_ADD_SET_FACTOR_REC );
#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( " %s %d <lwCpri>%d\n MTアドレス設定開始要因設定: REC主導\n ", __FUNCTION__, __LINE__, lwCpri);
#endif
			}
			sendFlg = CMD_TRUE;
		}
	}

	if(CMD_FALSE == sendFlg)
	{
		if( CMD_SYS_3G == sigsys)
		{
			/* MTアドレス設定応答((3G)(1:設定NG)) */
			f_trp_rec_Fc02_m_mtsetf_res_snd_3g( CMD_OTH_ERR );
		}
		else
		{
			/* MTアドレス設定応答((LTE) 51:処理NG(RE)) */
			f_trp_rec_Fc02_m_mtsetf_res_snd( CMD_OTH_ERR );
		}
	}
	else
	{
		cm_MemClr( &f_trp_rec_Wcom_MtAdSetInfo[sigsys], sizeof(CMT_CPRIF_MTADDSETREQ) );

		/* Save MT address seting info */
		memcpy( &f_trp_rec_Wcom_MtAdSetInfo[sigsys], &((CMT_TSKIF_CPRIRCV_MTADDSETREQ *)bufp)->cpridat_mtaddsetreq, sizeof(CMT_CPRIF_MTADDSETREQ) );

		/* MTアドレス設定要求(REC主導)再送回数初期化 */
		f_trp_rec_Wcom_MtAdSetRetryCnt[sigsys] = 0;
	}

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
