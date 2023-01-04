/*!
 * @skip   $Id$
 * @file   m_dl_rl_ufwrep_rsp2.c
 * @brief  the processing for "運用中ファームウェアファイル報告応答" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/10/02 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2010
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "運用中ファームウェアファイル報告応答" receiving at the REC主導
 *  運用中ファームウェア報告応答集約中 state.
 *  @note   A_function
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/10/02 TDIPS)sasaki Update MKレビュー指摘No.250,251,252対応
 *  @date   2015/10/09 FPT)Yen update メッセージサイズ
 */

VOID m_dl_rl_ufwrep_rsp2(VOID* bufp, USHORT cpri_no)
{
	T_RRH_SIDE_USE 						ltside_fhm;
	T_RRH_FW_INFO						ltfwinfo_fhm;
	UINT 								ldwret_breq;								/* Return value of buffer request */
	CMT_TSKIF_CPRISND_UFIRMFILERES2 	*lt_firmfile_p	= NULL;						/* 運用中ファームウェアファイル報告応答 */
	USHORT								lwLoop, reLoop;
	UINT								ldwSnd_ret;
	INT									ldwShmGetResult;
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0, "[m_dl_rl_ufwrep_rsp2] ENTER" );
	
	/* 全てのREをループする (16 RE)*/
	for( lwLoop = CMD_NUM0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
	{
		if(DLD_RESTA_IDLE != gt_dlw_refw_tbl[lwLoop ].re_sta)
		{
			return;
		}
	}
	
	/* 対REC状態をアイドルへ遷移 */
	gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_IDLE;
	
	/************************/
	/* 送信バッファ取得処理 */
	/************************/
	/* MSG送信用のバッファを取得 */
	ldwret_breq = CMD_NUM0;
	ldwret_breq = cm_L2BfGet( CMD_BUFCA_LAPSND, 
						   sizeof( CMT_TSKIF_CPRISND_UFIRMFILERES2 ), 
						   CMD_NUM1, 
						   (VOID**)&lt_firmfile_p );
	
	/* If buffer request NG		*/
	if(ldwret_breq != CMD_RES_OK)
	{
		cm_MAbort(	CMD_ALMCD_BUFGET, 
					"m_dl_rl_ufwrep_rsp2", 
					"Buffer request error", 
					ldwret_breq, 
					sizeof(CMT_TSKIF_CPRISND_UFIRMFILERES2), 
					CMD_NUM0);
	}
	
	/********************************************/
	/* ファームウェアファイル報告応答2作成 		*/
	/********************************************/

	/* CPRI信号データ部 */
	 /*-----the S3G CPRI signal-----*/
	lt_firmfile_p->cpridat_ufirmfileres2.signal_kind = CMD_CPRID_UFIRMFILERES2 | CMD_SYS_S3G;
	
	/* 共有メモリFHM保持FWファイル運用面の呼び出し 取得用 */
	f_cmn_com_side_use_get(&ltside_fhm);
	
	/* 共有メモリFHM保持FWファイル情報(0,1面)の呼び出し 取得用*/
	ldwShmGetResult = f_cmn_com_fw_info_fhm_get( ltside_fhm.use_side, &ltfwinfo_fhm);
	if(ldwShmGetResult != D_RRH_OK)
	{
		lt_firmfile_p->cpridat_ufirmfileres2.verinf_num = 0;
		
		/* 応答結果 */
		lt_firmfile_p->cpridat_ufirmfileres2.result = CMD_HDL_NG;
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "ufwrrsp2:NG(%d)", lt_firmfile_p->cpridat_ufirmfileres2.result);
	}
	else
	{
		lt_firmfile_p->cpridat_ufirmfileres2.verinf_num = 1;
		
		/* 応答結果 */
		lt_firmfile_p->cpridat_ufirmfileres2.result = CMD_NML;
		
		/* ファームウェアファイル情報群	 */
		lt_firmfile_p->cpridat_ufirmfileres2.firmfile[CMD_NUM0].makername = ltfwinfo_fhm.maker_id;
		lt_firmfile_p->cpridat_ufirmfileres2.firmfile[CMD_NUM0].cur_mjr_ver = ltfwinfo_fhm.major_ver;
		lt_firmfile_p->cpridat_ufirmfileres2.firmfile[CMD_NUM0].cur_mir_ver = ltfwinfo_fhm.minor_ver;

		(VOID)f_cmn_com_fw_info_fhm_get((ltside_fhm.use_side ^ 0x1), &ltfwinfo_fhm);
		lt_firmfile_p->cpridat_ufirmfileres2.firmfile[CMD_NUM0].nes_mjr_ver = ltfwinfo_fhm.major_ver;
		lt_firmfile_p->cpridat_ufirmfileres2.firmfile[CMD_NUM0].nes_mir_ver = ltfwinfo_fhm.minor_ver;

		/* Loop(RE#1～16) */
		for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
		{
			if(DLD_RESLT_NML == gt_dlw_refw_tbl[lwLoop].fw_rep_rslt)
			{
				/* 応答結果 */
				lt_firmfile_p->cpridat_ufirmfileres2.result = CMD_NML;
				lt_firmfile_p->cpridat_ufirmfileres2.firmfile[lt_firmfile_p->cpridat_ufirmfileres2.verinf_num].makername = gt_dlw_refw_tbl[lwLoop].mk_jdg;
				lt_firmfile_p->cpridat_ufirmfileres2.firmfile[lt_firmfile_p->cpridat_ufirmfileres2.verinf_num].nes_mjr_ver = gt_dlw_refw_tbl[lwLoop].mjr_ver;
				lt_firmfile_p->cpridat_ufirmfileres2.firmfile[lt_firmfile_p->cpridat_ufirmfileres2.verinf_num].nes_mir_ver = gt_dlw_refw_tbl[lwLoop].min_ver;
				lt_firmfile_p->cpridat_ufirmfileres2.firmfile[lt_firmfile_p->cpridat_ufirmfileres2.verinf_num].cur_mjr_ver = gt_dlw_refw_tbl[lwLoop].usmjr_ver;
				lt_firmfile_p->cpridat_ufirmfileres2.firmfile[lt_firmfile_p->cpridat_ufirmfileres2.verinf_num].cur_mir_ver = gt_dlw_refw_tbl[lwLoop].usmin_ver;
				
				lt_firmfile_p->cpridat_ufirmfileres2.verinf_num++;
			}
		}
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "ufwrrsp2:Num=%d, FHM:%x=V%02d%02d", 
								lt_firmfile_p->cpridat_ufirmfileres2.verinf_num, lt_firmfile_p->cpridat_ufirmfileres2.firmfile[CMD_NUM0].makername, 
								lt_firmfile_p->cpridat_ufirmfileres2.firmfile[0].cur_mjr_ver, lt_firmfile_p->cpridat_ufirmfileres2.firmfile[0].cur_mir_ver);
		for(lwLoop = 0; lwLoop < 4; lwLoop++)
		{
			reLoop = lwLoop*4;
			BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "         RE%2d:%x=V%02d%02d(rtn=%d), RE%2d:%x=V%02d%02d(rtn=%d), RE%2d:%x=V%02d%02d(rtn=%d), RE%2d:%x=V%02d%02d(rtn=%d)", 
									reLoop+1, gt_dlw_refw_tbl[reLoop].mk_jdg, gt_dlw_refw_tbl[reLoop].mjr_ver, gt_dlw_refw_tbl[reLoop].min_ver, gt_dlw_refw_tbl[reLoop].fw_rep_rslt, 
									reLoop+2, gt_dlw_refw_tbl[reLoop+1].mk_jdg, gt_dlw_refw_tbl[reLoop+1].mjr_ver, gt_dlw_refw_tbl[reLoop+1].min_ver, gt_dlw_refw_tbl[reLoop+1].fw_rep_rslt, 
									reLoop+3, gt_dlw_refw_tbl[reLoop+2].mk_jdg, gt_dlw_refw_tbl[reLoop+2].mjr_ver, gt_dlw_refw_tbl[reLoop+2].min_ver, gt_dlw_refw_tbl[reLoop+2].fw_rep_rslt, 
									reLoop+4, gt_dlw_refw_tbl[reLoop+3].mk_jdg, gt_dlw_refw_tbl[reLoop+3].mjr_ver, gt_dlw_refw_tbl[reLoop+3].min_ver, gt_dlw_refw_tbl[reLoop+3].fw_rep_rslt);
		}
	}
	/* タスク間ヘッダ部 */
	lt_firmfile_p->head.uiLength = 	sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_CPRISIGSND_INF ) + 
									CMD_NUM3 * sizeof(USHORT) + 
									lt_firmfile_p->cpridat_ufirmfileres2.verinf_num * sizeof( UFIRM_FILE_INF );

	/*************************************************/
	/* (LTE)(0x2131)ファームウェアファイル報告応答2 */
	/*************************************************/
	ldwSnd_ret = m_cm_L3MsgSend_REC( lt_firmfile_p );
	
	if(ldwSnd_ret != CMD_L3_OK)
	{
		cm_Assert(CMD_ASL_USELOW, CMD_NUM1, "m_dl_rl_ufwrep_rsp2 MSGSndNG");
	}
	
	/* 1: アイドル */
	gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_IDLE;
	
	return ;
}

/* @} */

