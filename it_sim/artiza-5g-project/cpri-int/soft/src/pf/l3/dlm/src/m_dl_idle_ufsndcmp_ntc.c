/*!
 * @skip   $Id$
 * @file   m_dl_idle_ufsndcmp_ntc.c
 * @brief  the processing for "運用中ファイルデータ送信完了報告通知" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/10/26 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "運用中ファイルデータ送信完了報告通知" receiving at the アイドル REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01913
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/10/26 TDIPS)sasaki Update M-RRU-ZSYS-01913 IT2問処No.146対処 DL開始要因初期化を追加
 */
VOID m_dl_idle_ufsndcmp_ntc(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_idle_ufsndcmp_ntc] ENTER" );
	
	/* 対RE状態遷移(REC主導)へ入力する */
	m_dl_rec_main(bufp, cpri_no);
	
	gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_IDLE;

	/* 実行中DL開始要因に0:未実行を設定 */
	gt_dlw_dlm_tbl.dlstart_factor = DLD_FACTOR_NONE;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_idle_ufsndcmp_ntc] RETURN" );
	return ;
}

/* @} */

