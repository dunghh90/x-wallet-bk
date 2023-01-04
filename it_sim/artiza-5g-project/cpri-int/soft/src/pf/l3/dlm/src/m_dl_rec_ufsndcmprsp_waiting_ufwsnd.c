/*!
 * @skip   $Id$
 * @file   m_dl_rec_ufsndcmprsp_waiting_ufwsnd.c
 * @brief  the processing for "ファイルデータ送信" receiving.
 * @date   2015/08/25 FPT)VietCV Create
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
 *  @brief  the processing for "ファイルデータ送信" receiving
 *  対RE状態遷移:運用中ファイルデータ送信完了報告応答(FW)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 * 	@date   2015/08/25 FPT)VietCV Create
 * 	@date   2015/10/02 TDIPS)sasaki Update 再ダウンロード準備処理を修正
 * 	@date   2015/10/08 FPT)Duong Update MKレビュー指摘241対応
*/

VOID m_dl_rec_ufsndcmprsp_waiting_ufwsnd(VOID* bufp, USHORT cpri_no)
{
	UINT							a_div_no	= CMD_NUM0;		/* 分割番号			*/
	CMT_TSKIF_CPRIRCV_UFILEDATSND 	*a_cpridat_p = NULL;		/* CPRI信号データ部	*/
	INT								ldwerrcd;
	USHORT							lwTimerId;
	UINT							ldwResult;
//	CMT_TSKIF_CPRIRCV_FILEDATSNDINT	ltFileDatSnd;
	UINT							lldatsize = CMD_NUM0;		/* データサイズ		*/

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_ufsndcmprsp_waiting_ufwsnd] ENTER" );
	/******************/
	/* データ取得処理 */
	/******************/
	/* CPRI信号データ部取得 */
	a_cpridat_p = (CMT_TSKIF_CPRIRCV_UFILEDATSND*)bufp;
	/* 分割番号取得 */
	a_div_no = (a_cpridat_p->cpridat_ufiledatsnd.sgmtno1 << DLD_BIT16) | 
				a_cpridat_p->cpridat_ufiledatsnd.sgmtno2;
	/* データサイズ取得 */
	lldatsize = (a_cpridat_p->cpridat_ufiledatsnd.datsize1 << DLD_BIT16) |
				 a_cpridat_p->cpridat_ufiledatsnd.datsize2;

	if (a_div_no == CMD_NUM1)
	{
		/* ファームウェア送信完了確認(更新用)タイマ#n停止 */
		lwTimerId = (cpri_no - 1) + CMD_TIMID_UFIRMFILE_SNDFIN;
		
		ldwResult = cm_TStop( lwTimerId, &ldwerrcd);
		
		/* Stop timer NG */
		if( ldwResult != CMD_RES_OK )
		{
			cm_Assert( CMD_ASL_DBGLOW, ldwResult, "ファームウェア送信完了確認(更新用)タイマ#n停止 NG" );
		}
	
		/* 再ダウンロード準備 */
		gt_dlw_refw_tbl[cpri_no - 1].trx_div_no = 0;
		gt_dlw_refw_tbl[cpri_no - 1].trx_data_size = 0;
		/* ファイルデータ送信完了報告結果初期化 */		
		gt_dlw_refw_tbl[cpri_no-1].filedata_trans_rslt = DLD_RESLT_IDLE;
		/* 対RE状態#nを運用中ファイルデータ送信中(FW)へ遷移 */
		gt_dlw_refw_tbl[cpri_no-1].re_sta = DLD_RESTA_USE_FILE_TRANS;
	}
#if 0
//	/* ファイルデータ送信転送指示(※2) */
//	memset(&ltFileDatSnd, 0, sizeof(ltFileDatSnd));
//	ltFileDatSnd.head.uiEventNo =  CMD_TSKIF_FILEDATSND_TRANS_IND;
//	ltFileDatSnd.cprircv_inf.link_num = cpri_no;
//
//	ltFileDatSnd.cpridat_filedatsndint.offset = gt_dlw_rec_tbl.rcv_data_size - lldatsize;
//
//	ltFileDatSnd.cpridat_filedatsndint.sgmtno1 = a_cpridat_p->cpridat_ufiledatsnd.sgmtno1;
//	ltFileDatSnd.cpridat_filedatsndint.sgmtno2 = a_cpridat_p->cpridat_ufiledatsnd.sgmtno2;
//
//	ltFileDatSnd.cpridat_filedatsndint.datsize1 = a_cpridat_p->cpridat_ufiledatsnd.datsize1;
//	ltFileDatSnd.cpridat_filedatsndint.datsize2 = a_cpridat_p->cpridat_ufiledatsnd.datsize2;
//
//	ltFileDatSnd.cpridat_filedatsndint.signal_kind = CMD_CPRID_UFILEDATSND + CMD_SYS_LTE;
//
//	l3_com_sendMsg(CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0, &ltFileDatSnd, sizeof( CMT_TSKIF_CPRIRCV_FILEDATSNDINT));
#endif
	m_dl_fileDatSndMsg_sndNtc(cpri_no, (CMD_CPRID_UFILEDATSND + CMD_SYS_LTE), a_div_no, lldatsize, a_cpridat_p->cpridat_ufiledatsnd.fildat);
	
	/* 送信済み分割番号 */
	gt_dlw_refw_tbl[cpri_no - 1].trx_div_no++;
	/* 送信済みデータサイズ */
	gt_dlw_refw_tbl[cpri_no - 1].trx_data_size += lldatsize;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_ufsndcmprsp_waiting_ufwsnd] RETURN" );
}
/** @} */
