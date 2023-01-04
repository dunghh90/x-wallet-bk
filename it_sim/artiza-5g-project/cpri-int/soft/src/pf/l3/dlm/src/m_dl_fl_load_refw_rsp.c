/*!
 * @skip   $Id$
 * @file   m_dl_fl_load_refw_rsp.c
 * @brief  Event: REファイルデータ読込応答(L3/DLM←L3/FRW_RE) REC status EC_DLM_MAIN_STN_FHMDL 2
 * @date   2015/08/22 FPT)Khiemnv1 Create
 * @date   2015/10/12 TDIPS)sasaki Update

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
 *  @brief  Event: REファイルデータ読込応答(L3/DLM←L3/FRW_RE) REC status EC_DLM_MAIN_STN_FHMDL 2
 *  @note   -------7.5.7---------------------------------
			--1
			"対RE状態遷移(FHM自律)へ入力する
			DL中REファイル読込状態を読込済に設定
			downloading RE file read status ->read complete
			=>2"
			call m_dl_fhm_main
			  >>m_dl_fhm_fwload_waiting_fwload_rsp
			....>>if(res = OK) call 7.5.7
			    >>if(res = NG) call 7.5.7
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 *  @date   2015/10/12 TDIPS)sasaki Update MKレビュー指摘No.282対応
 */

VOID m_dl_fl_load_refw_rsp(VOID* bufp, USHORT cpri_no)
{
	USHORT lw_all_RE_idle = 0;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fl_load_refw_rsp] ENTER" );

	m_dl_fhm_main(bufp, cpri_no);

    lw_all_RE_idle = all_RE_idle();
    if (lw_all_RE_idle)
    {
		set_DownLoading_RE_file_read_status(DLD_READ_NONE);
		set_DownLoading_RE_file_info(0xFFFF);
		set_REC_status(EC_DLM_MAIN_STN_IDLE);
	}
	else
	{
		set_DownLoading_RE_file_read_status(DLD_READ_DONE);
		set_REC_status(EC_DLM_MAIN_STN_FHMDL);
	}

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fl_load_refw_rsp] RETURN" );
}
/** @} */
