/*!
 * @skip   $Id$
 * @file   m_dl_rec_ufwreprsp_waiting_ufwrep_rsp.c
 * @brief  the processing for "運用中ファームウェアファイル報告応答" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/10/01 TDIPS)sasaki update
 * @date   2015/10/19 TDIPS)sasaki Update
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
 *  @brief  the processing for "運用中ファームウェアファイル報告応答" receiving at the 運用中ファームウェアファイル報告応答待ち RE state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01780
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/08/18 FPT)DuongCD IT update
 *  @date   2015/10/01 TDIPS)sasaki update MKレビュー指摘No.248,249対応
 *  @date   2015/10/19 TDIPS)sasaki M-RRU-ZSYS-01780 Update IT2問処No.85対処
 *  @date   2015/11/04 TDIPS)sasaki Update IT3問処No.49水平展開
 */
VOID m_dl_rec_ufwreprsp_waiting_ufwrep_rsp(VOID* bufp, USHORT cpri_no)
{
	USHORT						lwLoop;
	USHORT						lwmakername;
	USHORT						lwTimerId;
	UINT						ldwResult;
	INT							ldwerrcd;
	
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_ufwreprsp_waiting_ufwrep_rsp] ENTER" );
	
	/*運用中ファームウェアファイル報告応答NG */
	if(CMD_NML != ((CMT_TSKIF_CPRISND_UFIRMFILERES *)bufp)->cpridat_ufirmfileres.result)
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
		lwTimerId = CMD_TIMID_UFIRMFILE_REP + cpri_no -1;
		
		/* ファームウェアファイル報告確認(更新用)タイマ#n(※1)停止 */
		ldwResult = cm_TStop( lwTimerId, &ldwerrcd);
		
		/* Stop timer NG */
		if( ldwResult != CMD_RES_OK )
		{
			cm_Assert( CMD_ASL_DBGLOW, ldwResult, "ファームウェアファイル報告確認(更新用)タイマ Stop NG" );
		}
		
		/* 対RE状態#n(※1)をアイドルへ遷移 */
		gt_dlw_refw_tbl[cpri_no -1].re_sta = DLD_RESTA_IDLE;
		
#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;	
	}
	
	/* 当該対RE-FW情報に受信した応答内容を保存 */
	gt_dlw_refw_tbl[cpri_no -1].mk_jdg 		=  ((CMT_TSKIF_CPRISND_UFIRMFILERES *)bufp)->cpridat_ufirmfileres.firmfile.makername;
	gt_dlw_refw_tbl[cpri_no -1].mjr_ver 	=  ((CMT_TSKIF_CPRISND_UFIRMFILERES *)bufp)->cpridat_ufirmfileres.firmfile.nes_mjr_ver;
	gt_dlw_refw_tbl[cpri_no -1].min_ver 	=  ((CMT_TSKIF_CPRISND_UFIRMFILERES *)bufp)->cpridat_ufirmfileres.firmfile.nes_mir_ver;
	gt_dlw_refw_tbl[cpri_no -1].usmjr_ver 	=  ((CMT_TSKIF_CPRISND_UFIRMFILERES *)bufp)->cpridat_ufirmfileres.firmfile.cur_mjr_ver;
	gt_dlw_refw_tbl[cpri_no -1].usmin_ver 	=  ((CMT_TSKIF_CPRISND_UFIRMFILERES *)bufp)->cpridat_ufirmfileres.firmfile.cur_mir_ver;
	gt_dlw_refw_tbl[cpri_no -1].fw_rep_rslt =  DLD_RESLT_NML;
	
	/*FHM保持REファイル情報更新 */
	lwmakername = ((CMT_TSKIF_CPRISND_UFIRMFILERES *)bufp)->cpridat_ufirmfileres.firmfile.makername;
	/* Loop(RE#1～16) */
	for(lwLoop = 1; lwLoop <= D_RRH_CPRINO_RE_MAX; lwLoop++)
	{
		/* FHM保持REファイル情報更新 */
		if(lwmakername == gt_dlw_mnt_reinf[lwLoop].mk_jdg)
		{
			gt_dlw_mnt_reinf[lwLoop].re_conn_exist |= (CMD_ON << (cpri_no-1));
			BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "ufirmWarefileRsp RCV(RE#%02d,mk_jdg=%x,USE=V%02d%02d/NXT=V%02d%02d,Indx=0x%x,EXIST=0x%x)", 
									cpri_no, gt_dlw_refw_tbl[cpri_no-1].mk_jdg, gt_dlw_refw_tbl[cpri_no-1].usmjr_ver, gt_dlw_refw_tbl[cpri_no-1].usmin_ver, 
									gt_dlw_refw_tbl[cpri_no-1].mjr_ver, gt_dlw_refw_tbl[cpri_no-1].min_ver, 
									lwLoop, gt_dlw_mnt_reinf[lwLoop].re_conn_exist);
			break;
		}
	}
	/* 更新中リンク情報の当該CPRI#ビットON */
	gt_dlw_dlm_tbl.link_inf |= CMD_ON << (cpri_no -1);
	
	/* LTE system*/
	lwTimerId = CMD_TIMID_UFIRMFILE_REP + cpri_no -1;
	
	/* ファームウェアファイル報告確認(更新用)タイマ#n(※1)停止 */
	ldwResult = cm_TStop( lwTimerId, &ldwerrcd);
	
	/* Stop timer NG */
	if( ldwResult != CMD_RES_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, ldwResult, "ファームウェアファイル報告確認(更新用)タイマ Stop NG" );
	}
	
	/* 対RE状態#n(※1)をアイドルへ遷移*/
	gt_dlw_refw_tbl[cpri_no -1].re_sta = DLD_RESTA_IDLE;
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_ufwreprsp_waiting_ufwrep_rsp] RETURN" );
	return;
}

/* @} */

