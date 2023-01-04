/*!
 * @skip   $Id$
 * @file   m_dl_fl_fwrep_cnf_to.c
 * @brief  Event: ファームウェアファイル報告確認タイマタイムアウト通知 REC status EC_DLM_MAIN_STN_FHMDL 2
 * @date   2015/08/18 FPT)DuongCD Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2010
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"												/* MDIF common head file			*/
#include "m_dl_header.h"												/* download management task head file */
#include "rrhApi_File.h"

/*!
 * @brief  Event: ファームウェアファイル報告確認タイマタイムアウト通知 REC status EC_DLM_MAIN_STN_FHMDL 2
 *  @note   -------7.5.11-----------------------------
			--1
			"対RE状態遷移(FHM自律)へ入力する
			対RE状態が全てアイドル =>1
			else =>2"
			call m_dl_fhm_main
			  >>m_dl_fhm_fwreprsp_waiting_fwrep_cnf_to
			....>>7.5.11
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)Khiemnv Create
 */

VOID m_dl_fl_fwrep_cnf_to(VOID* bufp, USHORT cpri_no)
{
	USHORT			lwSysType;
	USHORT lw_all_RE_idle = 0;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fl_fwrep_cnf_to] ENTER" );

	lwSysType = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->signal_kind & CMD_SYS_CHK_CPRID;

	m_dl_fhm_main(bufp, cpri_no);

	if( CMD_SYS_3G == lwSysType )
	{
		/* DLMスレッドメイン(対REC状態遷移) => アイドル */
		gt_dlw_rec_3g_tbl.rec_sta = EC_DLM_MAIN_STN_IDLE;
	}
	else
	{
		lw_all_RE_idle = all_RE_idle();
		if (lw_all_RE_idle)
		{
			set_REC_status(EC_DLM_MAIN_STN_IDLE);
		}
		else
		{
			set_REC_status(EC_DLM_MAIN_STN_FHMDL);
		}
	}
	return;
}


/* @} */

