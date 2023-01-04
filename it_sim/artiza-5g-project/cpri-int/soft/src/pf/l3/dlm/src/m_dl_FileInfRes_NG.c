/*!
 * @skip   $Id$
 * @file   m_dl_FileInfRes_NG.c
 * @brief  the processing for "ファイル情報報告応答" receiving with result is NG.
 * @date   2015/08/20 FPT)VietCV Create
 * @date   2015/10/20 TDIPS)sasaki Update
 * @date   2015/12/04 TDIPS)sasaki Update

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
 *  @brief  the processing for "ファイル情報報告応答" receiving with result is NG
 *  対REC状態遷移:ファイルデータ送信(FW)待ち state.
 *	対RE状態遷移: ファイル情報報告応答(FW)待ち
 *  @param  bufp    [in]  pointer of received message   
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01816 
 *  @date   2015/08/20 FPT)VietCV Create
 *  @date   2015/10/20 TDIPS)sasaki M-RRU-ZSYS-01816 Update IT2問処No.118対処
 *  @date   2015/12/04 TDIPS)sasaki Update IT3問処No.126対処
 */
 VOID m_dl_FileInfRes_NG(VOID* bufp, USHORT cpri_no)
{
	INT							ldwerrcd;
	USHORT						lwTimerId;
	UINT						ldwResult;
	USHORT						lwTimeVal	= CMD_NUM0;
	CMT_TSKIF_CPRISND_FILEINFREQ ltCpriFileInfReq;
	
	lwTimerId = (cpri_no - 1) + CMD_TIMID_FIRMFILE_INF;
	
	/* ファームウェアファイル情報報告確認タイマ#n停止 */
	ldwResult = cm_TStop( lwTimerId, &ldwerrcd);
	
	/* Stop timer NG */
	if( ldwResult != CMD_RES_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, ldwResult, "ファームウェアファイル情報報告確認タイマ#n停止 NG" );
	}
	
	/* 再送回数＜再送回数最大値 */
	if (gt_dlw_refw_tbl[cpri_no-1].fileinf_rep_tc < DLD_FILEINFREP_MAX)
	{
		/* 再送回数[ファイル情報報告通知]#nを加算 */
		gt_dlw_refw_tbl[cpri_no-1].fileinf_rep_tc++;
		
		/* ファームウェアファイル情報報告確認タイマ#n開始 */
		lwTimeVal = CMD_TIMVAL_FIRMFILE_INF_REP;
		
		ldwResult = cm_TStat( lwTimerId, lwTimeVal, CMD_TIMMD_SINGLE, CMD_TSKIF_TIMOUTNTC,CMD_TSKID_DLM,(INT*)&ldwerrcd);
		if( ldwResult != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_USELOW, ldwResult, "ファームウェアファイル情報報告確認タイマ#n開始 NG" );
		}
		
		/* RE Forwarding Notice(ファイル情報報告通知) */
		ltCpriFileInfReq.head.uiEventNo = CMD_TSKIF_RE_FWDNTC;
		ltCpriFileInfReq.head.uiSignalkind = CMD_CPRID_FILEINFREQ + CMD_SYS_LTE;
		ltCpriFileInfReq.cprisnd_inf.link_num = cpri_no;
		ltCpriFileInfReq.cpridat_fileinfreq.data = CMD_FRM;
		ltCpriFileInfReq.cpridat_fileinfreq.signal_kind = CMD_CPRID_FILEINFREQ + CMD_SYS_LTE;
		cm_MemCpy(ltCpriFileInfReq.cpridat_fileinfreq.filename, 
					gt_dlw_mnt_reinf[CMD_NUM0].file_name, 
					CMD_NUM20);
		ltCpriFileInfReq.cpridat_fileinfreq.size = gt_dlw_mnt_reinf[CMD_NUM0].file_size;
		
		l3_com_sendMsg( CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0,  &ltCpriFileInfReq, sizeof(CMT_TSKIF_CPRISND_FILEINFREQ));
		
		/* ファイル情報報告結果を初期化 */
		gt_dlw_refw_tbl[cpri_no-1].fileinf_rep_rslt = DLD_RESLT_IDLE;
		/* change status of re status */
		gt_dlw_refw_tbl[cpri_no-1].re_sta = DLD_RESTA_FILEINF_WAIT;
		return;
	}
	
	/* 対RE-FW情報テーブルの
	ファイル情報報告結果にNGを設定 */
	gt_dlw_refw_tbl[cpri_no-1].fileinf_rep_rslt = DLD_RESLT_NG;
	
	/* 対RE-FW情報テーブルの
	使用中タイマを初期化 */
	gt_dlw_refw_tbl[cpri_no-1].use_timer = DLD_RERETIM_NONE;
	
	/* 再送回数[ファイル情報報告通知]#n初期化 */
	gt_dlw_refw_tbl[cpri_no-1].fileinf_rep_tc = CMD_NUM0;
	
	/* 更新開始要因#nを初期化 */
	gt_dlw_refw_tbl[cpri_no-1].start_factor = DLD_UPDATE_FACTOR_NONE;
	
	/* 更新中リンク情報の当該CPRI#ビットOFF */
	gt_dlw_dlm_tbl.link_inf &= ~(CMD_ON << (cpri_no-1));
	
	m_dl_reFileUpdateCompNtc(cpri_no, DLD_RESLT_NG);
}
/** @} */
