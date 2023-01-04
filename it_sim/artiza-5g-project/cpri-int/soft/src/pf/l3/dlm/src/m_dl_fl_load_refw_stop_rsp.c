/*!
 * @skip   $Id$
 * @file   m_dl_fl_load_refw_stop_rsp.c
 * @brief  Event: REファイルデータ読込中止応答(L3/DLM←L3/FRW_RE) REC status EC_DLM_MAIN_STN_FHMDL 2
 * @date   2015/08/22 FPT)Khiemnv1 Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */

/*!
 *  @brief  Event: REファイルデータ読込中止応答(L3/DLM←L3/FRW_RE) REC status EC_DLM_MAIN_STN_FHMDL 2
 *  @note   -------7.5.18-----------------------------
			--1
			"対RE状態遷移(FHM自律)へ入力する
			対RE状態が全てアイドル=>1
			else =>2"
			call m_dl_fhm_main
			  >>m_dl_fhm_fwload_waiting_fwloadstop_rsp
			....>>7.5.18
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 */

VOID m_dl_fl_load_refw_stop_rsp(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fl_load_refw_stop_rsp] ENTER" );

	USHORT lw_all_RE_idle = 0;

	/*DL中REファイル読込状態を0:未実行に設定
	 *set DL RE file read status to 0
	 */
	set_DownLoading_RE_file_read_status(
		DLD_READ_NONE);

	m_dl_fhm_main(bufp, cpri_no);

	lw_all_RE_idle = all_RE_idle();
	if (lw_all_RE_idle)
	{
		set_REC_status(EC_DLM_MAIN_STN_IDLE);
	}
	else
	{
		set_REC_status(EC_DLM_MAIN_STN_FHMDL);
	}
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fl_load_refw_stop_rsp] RETURN" );
}

/* @} */