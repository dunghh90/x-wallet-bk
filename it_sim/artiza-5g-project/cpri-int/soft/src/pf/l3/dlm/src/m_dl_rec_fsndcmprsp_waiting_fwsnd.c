/*!
 * @skip   $Id$
 * @file   m_dl_rec_fsndcmprsp_waiting_fwsnd.c
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
 *  対RE状態遷移:ファイルデータ送信完了報告応答(FW)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 * 	@date   2015/08/25 FPT)VietCV Create
 *  @date   2015/09/23 FPT)Yen update comment
 *  @date   2015/10/02 TDIPS)sasaki Update 再ダウンロード準備処理を修正
*/

VOID m_dl_rec_fsndcmprsp_waiting_fwsnd(VOID* bufp, USHORT cpri_no)
{
	UINT								a_div_no	= CMD_NUM0;		/* 分割番号			*/
	CMT_TSKIF_CPRIRCV_FILEDATSND 		*a_cpridat_p = NULL;		/* CPRI信号データ部	*/
	INT									ldwerrcd;
	USHORT								lwTimerId;
	UINT								ldwResult;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_fsndcmprsp_waiting_fwsnd] ENTER" );
	/******************/
	/* データ取得処理 */
	/******************/
	/* CPRI信号データ部取得 */
	a_cpridat_p = (CMT_TSKIF_CPRIRCV_FILEDATSND*)bufp;
	/* 分割番号取得 */
	a_div_no = (a_cpridat_p->cpridat_filedatsnd.sgmtno1 << DLD_BIT16) | 
				a_cpridat_p->cpridat_filedatsnd.sgmtno2;

	if (a_div_no == CMD_NUM1)
	{
		/* ファームウェア送信完了確認タイマ#n(※1)停止 */
		lwTimerId = (cpri_no - 1) + CMD_TIMID_FIRMFILE_SNDFIN;
		
		ldwResult = cm_TStop( lwTimerId, &ldwerrcd);
		
		/* Stop timer NG */
		if( ldwResult != CMD_RES_OK )
		{
			cm_Assert( CMD_ASL_DBGLOW, ldwResult, "ファームウェア送信完了確認タイマ#n(※1)停止 NG" );
		}
	
		/* 再ダウンロード準備 */
		gt_dlw_refw_tbl[cpri_no - 1].trx_div_no = 0;
		gt_dlw_refw_tbl[cpri_no - 1].trx_data_size = 0;
		/* ファイルデータ送信完了報告結果初期化 */
		gt_dlw_refw_tbl[cpri_no - 1].filedata_trans_rslt = DLD_RESLT_IDLE;
	}

	/* ファイルデータ送信転送指示(※2) */
	m_dl_rec_fwsnd_waiting_fwsnd(bufp, cpri_no);

	/* 対RE状態#nをファイルデータ		送信中(FW)へ遷移 */
	gt_dlw_refw_tbl[cpri_no-1].re_sta = DLD_RESTA_FILE_TRANS;
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_fsndcmprsp_waiting_fwsnd] RETURN" );

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_fsndcmprsp_waiting_fwsnd] RETURN" );
	return;
}
/** @} */
