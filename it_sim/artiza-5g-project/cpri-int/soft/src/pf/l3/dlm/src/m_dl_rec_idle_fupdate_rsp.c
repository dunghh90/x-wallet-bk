/*!
 * @skip   $Id$
 * @file   m_dl_rec_idle_fupdate_rsp.c
 * @brief  the processing for "運用中REファイル更新開始応答(L3/DLM←RE/REC)" receiving.
 * @date   2015/08/25 FPT)VietCV Create
 * @date   2015/10/02 TDIPS)sasaki Update
 * @date   2015/10/20 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "運用中REファイル更新開始応答(L3/DLM←RE/REC)" receiving.
 *  RE主導:アイドル
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01816 
 * 	@date   2015/08/25 FPT)VietCV create
 *  @date   2015/09/14 FPT)VietCV update
 *  @date   2015/10/02 TDIPS)sasaki update MKレビュー指摘No.235対応
 *  @date   2015/10/14 FPT)DuongCD update correct using size of struct
 *  @date   2015/10/20 TDIPS)sasaki M-RRU-ZSYS-01816 Update IT2問処No.118対処
 */
VOID m_dl_rec_idle_fupdate_rsp(VOID* bufp, USHORT cpri_no)
{
	CMT_TSKIF_UREFILNEWSTARES *lt_fileUpdateRsp_p;
	CMT_TSKIF_CPRISND_FILEINFREQ ltCpriFileInfReq;
	CMT_TSKIF_CPRISND_UFILEINFREQ ltCpriUFileInfReq;
	INT							ldwerrcd;
	USHORT						lwTimerId;
	UINT						ldwResult;
	USHORT						lwTimeVal	= CMD_NUM0;
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_idle_fupdate_rsp] ENTER" );
	
	lt_fileUpdateRsp_p = (CMT_TSKIF_UREFILNEWSTARES *)bufp;
	
	if (CMD_NML != lt_fileUpdateRsp_p->result_code)
	{
#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}
	
	/* 更新中リンク情報の更新対象CPRI#ビットON */
	gt_dlw_dlm_tbl.link_inf |= CMD_ON << (cpri_no-1);
	
	/* 更新開始要因#nにREC主導を設定 */
	gt_dlw_refw_tbl[cpri_no-1].start_factor = DLD_UPDATE_FACTOR_REC_INIT;
	
	if (EC_DLM_MAIN_STN_FILEDATATRANS_FW_WAIT == gt_dlw_rec_tbl.rec_sta)
	{
		/* (0x4001)RE Forwarding Notice(ファイル情報報告通知) */
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
		
		/* 対RE状態#n(※1)を
		ファイル情報報告応答待ちへ遷移 */
		gt_dlw_refw_tbl[cpri_no-1].re_sta = DLD_RESTA_FILEINF_WAIT;
		
		lwTimerId = CMD_TIMID_FIRMFILE_INF + (cpri_no - 1);
		
		/* ファームウェアファイル情報報告確認タイマ(更新用)#n(※1)開始 */
		lwTimeVal = CMD_TIMVAL_FIRMFILE_INF_REP;
	}
	else if (EC_DLM_MAIN_STN_USE_FILEDATATRANS_FW_WAIT == gt_dlw_rec_tbl.rec_sta)
	{
		/* (0x4001)RE Forwarding Notice(運用中ファイル情報報告通知) */
		ltCpriUFileInfReq.head.uiEventNo = CMD_TSKIF_RE_FWDNTC;
		ltCpriUFileInfReq.head.uiSignalkind = CMD_CPRID_UFILEINFREQ + CMD_SYS_LTE;
		ltCpriUFileInfReq.cprisnd_inf.link_num = cpri_no;
		ltCpriUFileInfReq.cpridat_ufileinfreq.data = CMD_FRM;
		ltCpriUFileInfReq.cpridat_ufileinfreq.signal_kind = CMD_CPRID_UFILEINFREQ + CMD_SYS_LTE;
		cm_MemCpy(ltCpriUFileInfReq.cpridat_ufileinfreq.filename, 
					gt_dlw_mnt_reinf[CMD_NUM0].file_name, 
					CMD_NUM20);
		ltCpriUFileInfReq.cpridat_ufileinfreq.size = gt_dlw_mnt_reinf[CMD_NUM0].file_size;

		l3_com_sendMsg( CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0,  &ltCpriUFileInfReq, sizeof(CMT_TSKIF_CPRISND_UFILEINFREQ));
		
		/*対RE状態#n(※1)を
		運用中ファイルデータ送信中(FW)へ遷移 */
		gt_dlw_refw_tbl[cpri_no-1].re_sta = DLD_RESTA_USE_FILEINF_WAIT;
		
		lwTimerId = CMD_TIMID_UFIRMFILE_INF + (cpri_no - 1);
		
		lwTimeVal = CMD_TIMVAL_UFIRMFILE_INF_REP;
	}

	/* ファイル情報報告応答結果を初期化 */
	gt_dlw_refw_tbl[cpri_no-1].fileinf_rep_rslt = DLD_RESLT_IDLE;

	/* start timer */	
	ldwResult = cm_TStat( lwTimerId, lwTimeVal, CMD_TIMMD_SINGLE, CMD_TSKIF_TIMOUTNTC,CMD_TSKID_DLM,(INT*)&ldwerrcd); /* pgr0000 */
	if( ldwResult != CMD_RES_OK)
	{
		cm_Assert( CMD_ASL_USELOW, ldwResult, "ファームウェアファイル情報報告確認タイマ(更新用)#n開始 NG" );
	}
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_idle_fupdate_rsp] RETURN" );
	return;
}
/** @} */
