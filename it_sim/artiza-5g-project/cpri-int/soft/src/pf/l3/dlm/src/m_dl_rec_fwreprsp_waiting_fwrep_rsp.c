/*!
 * @skip   $Id$
 * @file   m_dl_rec_fwreprsp_waiting_fwrep_rsp.c
 * @brief  the processing for "ファームウェアファイル報告応答" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/09/28 TDIPS)sasaki Update
 * @date   2015/10/19 TDIPS)sasaki Update
 * @date   2015/10/27 TDIPS)sasaki Update
 * @date   2015/11/04 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"								/* MDIF common head file			*/
#include "m_dl_header.h"								/* download management task head file */
#include "rrhApi_File.h"
/*!
 *  @brief  the processing for "ファームウェアファイル報告応答" receiving at the ファームウェアファイル報告応答待ち RE state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01780
 *  @Bug_No M-RRU-ZSYS-01914
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/09/21 FPT)DuongCD IT update
 *  @date   2015/09/28 TDIPS)sasaki Update MKレビュー指摘No.222対応
 *  @date   2015/10/19 TDIPS)sasaki M-RRU-ZSYS-01780 Update IT2問処No.85対処
 *  @date   2015/10/27 TDIPS)sasaki M-RRU-ZSYS-01914 Update IT2問処No.147対処 ファームウェアファイル報告応答のバージョン情報を対RE状態テーブルに保存するよう修正
 *  @date   2015/11/04 TDIPS)sasaki Update IT3問処No.49水平展開
 */
VOID m_dl_rec_fwreprsp_waiting_fwrep_rsp(VOID* bufp, USHORT cpri_no)
{
	USHORT						lwLoop;
	USHORT						lwmakername;
	USHORT						lwTimerId;
	UINT						ldwResult;
	INT							ldwerrcd;
	USHORT						lw_res_result;	/* 応答結果 */
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start cpri_no = %d\n",__LINE__,__FUNCTION__, cpri_no);
	#endif
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_fwreprsp_waiting_fwrep_rsp] ENTER" );
	
	
	lw_res_result = ((CMT_TSKIF_CPRISND_FIRMFILERES *)bufp)->cpridat_firmfileres.result;
	/* ファームウェアファイル報告応答NG */
	if(CMD_NML != lw_res_result)
	{
		/* 当該対RE-FW情報テーブルのファームウェアファイル報告結果にNGを設定 */
		gt_dlw_refw_tbl[cpri_no -1].fw_rep_rslt = DLD_RESLT_NG;
		
		/* 当該対RE-FW情報テーブルの使用中タイマを初期化 */
		gt_dlw_refw_tbl[cpri_no -1].use_timer = DLD_RERETIM_NONE;
		
		/* 更新開始要因#nを初期化 */
		gt_dlw_refw_tbl[cpri_no -1].start_factor = DLD_UPDATE_FACTOR_NONE;
		
		/* 更新中リンク情報のT.O.発生CPRI#ビットOFF */
		gt_dlw_dlm_tbl.link_inf &= ~(CMD_ON << (cpri_no -1));
		
		/* LTE system*/
		lwTimerId = CMD_TIMID_FIRMFILE_REP + cpri_no -1;
		
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s lwTimerId[%d]\n",__LINE__,__FUNCTION__, lwTimerId);
		#endif
		/* ファームウェアファイル報告確認タイマ#n(※1)停止 */
		ldwResult = cm_TStop( lwTimerId, &ldwerrcd);
		
		/* Stop timer NG */
		if( ldwResult != CMD_RES_OK )
		{
			cm_Assert( CMD_ASL_DBGLOW, ldwResult, "firmware file rep conf timer Stop NG" );
		}
		
		/* 対RE状態#n(※1)をアイドルへ遷移 */
		gt_dlw_refw_tbl[cpri_no -1].re_sta = DLD_RESTA_IDLE;

		/* 「ファームウェアなし」ならば応答結果を正常に更新 */
		if(lw_res_result == CMD_FRM_NOFILE_S3G)
		{
			lw_res_result = CMD_NML;
		}
		else
		{
			BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "firmware file rsp NG(rslt=%d, cpri_no=%d)", lw_res_result, cpri_no);
		}

		/* processing for 7-2-9 運用中REファイル更新完了通知 */
		m_dl_reFileUpdateCompNtc(cpri_no, lw_res_result);
		
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
		#endif
		return;	
	}
	
	/* 当該対RE-FW情報に受信した応答内容を保存 */
	gt_dlw_refw_tbl[cpri_no-1].mk_jdg = ((CMT_TSKIF_CPRISND_FIRMFILERES *)bufp)->cpridat_firmfileres.firmfile.makername;
	gt_dlw_refw_tbl[cpri_no-1].usmjr_ver = ((CMT_TSKIF_CPRISND_FIRMFILERES *)bufp)->cpridat_firmfileres.firmfile.mjr_ver;
	gt_dlw_refw_tbl[cpri_no-1].usmin_ver = ((CMT_TSKIF_CPRISND_FIRMFILERES *)bufp)->cpridat_firmfileres.firmfile.mir_ver;
	gt_dlw_refw_tbl[cpri_no-1].fw_rep_rslt = DLD_RESLT_NML;
	
	lwmakername = ((CMT_TSKIF_CPRISND_FIRMFILERES *)bufp)->cpridat_firmfileres.firmfile.makername;
	/* Loop(RE#1～16) */
	for(lwLoop = 1; lwLoop <= D_RRH_CPRINO_RE_MAX; lwLoop++ )
	{
		/* FHM保持REファイル情報更新 */
		if(lwmakername == gt_dlw_mnt_reinf[lwLoop].mk_jdg)
		{
			gt_dlw_mnt_reinf[lwLoop].re_conn_exist |= (CMD_ON << (cpri_no-1));
			BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "firmWarefileRsp RCV(RE#%02d,mk_jdg=%x,V%02d%02d,Indx=0x%x,EXIST=0x%x)", 
									cpri_no, gt_dlw_refw_tbl[cpri_no-1].mk_jdg, gt_dlw_refw_tbl[cpri_no-1].usmjr_ver, gt_dlw_refw_tbl[cpri_no-1].usmin_ver, lwLoop, gt_dlw_mnt_reinf[lwLoop].re_conn_exist);
			break;
		}
	}
	/* 更新中リンク情報の当該CPRI#ビットON */
	gt_dlw_dlm_tbl.link_inf |= CMD_ON << (cpri_no -1);
	
	/* LTE system*/
	lwTimerId = CMD_TIMID_FIRMFILE_REP + cpri_no -1;
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s lwTimerId[%d]\n",__LINE__,__FUNCTION__, lwTimerId);
	#endif
	/* ファームウェアファイル報告確認タイマ#n(※1)停止 */
	ldwResult = cm_TStop( lwTimerId, &ldwerrcd);
	
	/* Stop timer NG */
	if( ldwResult != CMD_RES_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, ldwResult, "firmware file rep conf timer Stop NG" );
	}
	
	/* 対RE状態#n(※1)をアイドルへ遷移 */
	gt_dlw_refw_tbl[cpri_no -1].re_sta = DLD_RESTA_IDLE;
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
		
	return;
}

/* @} */

