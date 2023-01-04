/*!
 * @skip   $Id$
 * @file   m_dl_rl_ufinfrsp_waiting_ufinf_ntc.c
 * @brief  the processing for "運用中ファイル情報報告通知" receiving.
 * @date   2015/08/18 FPT)DuongCD Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2010
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "運用中ファイル情報報告通知" receiving at the 運用中ファイルデータ転送先決定待ち REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.227対応
 */

VOID m_dl_rl_ufinfrsp_waiting_ufinf_ntc(VOID* bufp, USHORT cpri_no)
{
	USHORT  							lwTimerId;
	INT									ldwerrcd;
	UINT								ldwResult;
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_ufinfrsp_waiting_ufinf_ntc] ENTER" );
	
	/* 対REC状態がファイルデータ転送先決定待ち */
	/* タイマ管理テーブルのタイマ起動情報がタイマ起動中のとき */
	
	/* 使用タイマなし */
	gt_dlw_rec_tbl.use_timer = DLD_RECRETIM_NONE;
	
	lwTimerId = CMD_TIMID_USEFIRMWARESND;
	/* ファームウェア送信確認(更新用)タイマ停止 */
	ldwResult = cm_TStop(lwTimerId,&ldwerrcd);
	
	/* Stop timer NG */
	if( ldwResult != CMD_RES_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, ldwResult, "ファームウェア送信確認(更新用)タイマ停止 Stop NG" );
	}
	
	/* ダウンロード終了処理起動 */
	m_dl_cm_DlEnd(CMD_SYS_S3G);

	m_dl_UFimPro(bufp);

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_ufinfrsp_waiting_ufinf_ntc] RETURN" );
	
	return ;
}

/* @} */

