/*!
 * @skip   $Id$
 * @file   m_dl_reFileUpdateTermination.c
 * @brief  the processing for Re File Update Termination
 * @date   2015/08/20 FPT)VietCV Create
 * @date   2015/09/30 TDIPS)sasaki Update
 * @date   2015/10/02 TDIPS)sasaki Update
 * @date   2015/10/17 TDIPS)sasaki Update
 * @date   2015/10/20 TDIPS)sasaki Update
 * @date   2015/10/20 TDIPS)sasaki Update
 * @date   2015/10/24 TDIPS)sasaki Update
 * @date   2015/10/28 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */
#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */
/*!
 *  @brief  the processing for Re File Update Termination
 *  @note   
 *  
 *  @return Void.
 *  @Bug_No M-RRU-ZSYS-01737
 *  @Bug_No M-RRU-ZSYS-01791
 *  @Bug_No M-RRU-ZSYS-01838
 *  @Bug_No M-RRU-ZSYS-01851
 *  @Bug_No M-RRU-ZSYS-01930
 *  @date   2015/08/20 FPT)VietCV Create
 *  @date   2015/09/16 FPT)VietCV update
 *  @date   2015/09/24 FPT)DuongCD M-RRU-ZSYS-01851 IT update add check code
 *  @date   2015/09/30 TDIPS)sasaki M-RRU-ZSYS-01737 update MKレビュー指摘No.208,209,210,211,212対応
 *  @date   2015/09/30 TDIPS)sasaki update ファイルデータ送信完了報告応答メッセージを共有メモリに確保するよう修正
 *  @date   2015/10/02 TDIPS)sasaki update 未応答RE有無の判定文見直し(運用中DLにも対応)
 *  @date   2015/10/17 TDIPS)sasaki M-RRU-ZSYS-01791 update IT2問処No.96,97対処 配下への転送不要と判明した時点の DL開始要因初期化処理を削除 全配下REからOK応答なしのルートで 応答結果223を返すよう修正
 *  @date   2015/10/20 TDIPS)sasaki update IT2問処No.97対処誤りを修正
 *  @date   2015/10/20 TDIPS)sasaki Update ハソ-QA-031対応
 *  @date   2015/10/24 TDIPS)sasaki M-RRU-ZSYS-01838 Update IT2問処No.137対処
 *  @date   2015/10/28 TDIPS)sasaki M-RRU-ZSYS-01930 Update IT2問処No.153対処
 *  @date   2017/08/22 FJT)ohashi   17A(3201-16B-TS92)対応 DL完了対象RE判定処理,初期化処理修正
 */
VOID m_dl_reFileUpdateTermination()
{

	USHORT	lwLoop;
	USHORT 	lwOkRes = 0;
	USHORT 	lwRetryFileInfNtc = CMD_OFF;
	USHORT	lwResult = gt_dlw_rec_tbl.filedata_trans_rslt;
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	#endif
	
	/* フラッシュ書込フラグON */
	if (CMD_ON == gt_dlw_dlm_tbl.write_flg)
	{
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
	
	for (lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++)
	{
		/* 応答結果が0:未実行の転送先RE有り */
		if ((gt_dlw_refw_tbl[lwLoop].re_sta == DLD_RESTA_FILE_COMP_WAIT ||
			 gt_dlw_refw_tbl[lwLoop].re_sta == DLD_RESTA_USE_FILE_COMP_WAIT) &&
			 gt_dlw_refw_tbl[lwLoop].filedata_trans_rslt == DLD_RESLT_IDLE)
		{
			#ifdef FHM_DLM_FOR_DEBUG_IT1
			printf( "[%d]%s end gt_dlw_refw_tbl[%d].filedata_trans_rslt == DLD_RESLT_IDLE\n",__LINE__,__FUNCTION__, lwLoop);
			#endif
			return;
		}
		/* 転送先REで応答結果が0有り */
		else if (gt_dlw_refw_tbl[lwLoop].filedata_trans_rslt == DLD_RESLT_NML)
		{
			lwOkRes = 1;
		}
		/* 転送先REで応答結果がNG or TO有り */
		else if(gt_dlw_refw_tbl[lwLoop].fileinf_rep_rslt == DLD_RESLT_NG || 
				gt_dlw_refw_tbl[lwLoop].fileinf_rep_rslt == DLD_RESLT_TO)
		{
			lwRetryFileInfNtc = CMD_ON;
			BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "fwsndcmp_rsp cpri=0x%04x NG., rslt=0x0x%04x", lwLoop, gt_dlw_refw_tbl[lwLoop].fileinf_rep_rslt);
		}
	}

	/* 一部の配下REからOK応答あり */
	if (lwOkRes)
	{
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "fwsndcmp_rsp OK. link_inf=0x%04x", gt_dlw_dlm_tbl.link_inf);

		/* '7-2-9 運用中REファイル更新完了通知(NG)を */
		for (lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++)
		{
			if ((gt_dlw_refw_tbl[lwLoop].re_sta == DLD_RESTA_FILE_COMP_WAIT) && 
				(gt_dlw_refw_tbl[lwLoop].filedata_trans_rslt == DLD_RESLT_NG || 
				 gt_dlw_refw_tbl[lwLoop].filedata_trans_rslt == DLD_RESLT_TO))
			{
				m_dl_reFileUpdateCompNtc(lwLoop+1, CMD_NG);
			}

			/* 全対RE状態#nをアイドルへ遷移 */
			gt_dlw_refw_tbl[lwLoop].re_sta = DLD_RESTA_IDLE;
		}

		lwResult = CMD_NML;

		/* 対REC状態をアイドルへ遷移 */
		gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_IDLE;

		/* 更新中リンク情報初期化0:未実行を設定 */
		gt_dlw_dlm_tbl.link_inf = CMD_NUM0;

		/* 実行中DL開始要因に */
		gt_dlw_dlm_tbl.dlstart_factor	= DLD_FACTOR_NONE;

		/* ダウンロード終了処理起動 */
		m_dl_cm_DlEnd(CMD_SYS_S3G);

		/* LED設定処理 */
		m_cm_WriteLEDReg();

	}
	/* 全配下REからOK応答なし */
	else
	{
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "fwsndcmp_rsp ALLRE_NG. link_inf=0x%04x", gt_dlw_dlm_tbl.link_inf);

		/* 再ダウンロード準備 */
		if (EC_DLM_MAIN_STN_USE_FILEDATATRANS_FINREP_WAIT == gt_dlw_rec_tbl.rec_sta)
		{
			gt_dlw_rec_tbl.rec_sta = (lwRetryFileInfNtc == CMD_ON) ? EC_DLM_MAIN_STN_USE_FILEDATATRANS_DES_WAIT : EC_DLM_MAIN_STN_USE_FILEDATATRANS_FW_WAIT;
			m_dl_cm_USDlRetry();
		}
		else if (EC_DLM_MAIN_STN_FILEDATATRANS_FINREP_WAIT == gt_dlw_rec_tbl.rec_sta)
		{
			gt_dlw_rec_tbl.rec_sta = (lwRetryFileInfNtc == CMD_ON) ? EC_DLM_MAIN_STN_FILEDATATRANS_DES_WAIT : EC_DLM_MAIN_STN_FILEDATATRANS_FW_WAIT;
			m_dl_cm_DlRetry(CMD_SYS_S3G);
		}
	}

	if(dlw_downtyp_tbl == DLD_TSK_TYP_USE)
	{
		m_dl_cm_CRUSDataEndRspSnd(lwResult);
	}
	else
	{
		m_dl_cm_CRDataEndRspSnd_S3G(lwResult, CMD_SYS_S3G, dlw_prisys_tbl);
	}

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
	return;
}
/** @} */
