/*!
 * @skip   $Id$
 * @file   m_dl_fl_fwrep_rsp.c
 * @brief  Event: ファームウェアファイル報告応答(L3信号：FHM←RE) REC status EC_DLM_MAIN_STN_FHMDL 2
 * @date   2015/08/22 FPT)Khiemnv1 Create
 * @date   2015/11/18 TDIPS)sasaki Update

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
 *  @brief  Event: ファームウェアファイル報告応答(L3信号：FHM←RE) REC status EC_DLM_MAIN_STN_FHMDL 2
 *  @note   -------7.5.2---------------------------
			--1
			"対RE状態遷移(FHM自律)へ入力する
			対RE状態が全て
			アイドル=>1
			else =>2"
			call m_dl_fhm_main()
			  >>m_dl_fhm_fwreprsp_waiting_fwrep_rsp
			    >>7.5.2
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 *  @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.243対応
 *  @date   2015/11/18 TDIPS)sasaki Update IT3問処No.82対処
 */

VOID m_dl_fl_fwrep_rsp(VOID* bufp, USHORT cpri_no)
{
	USHORT			idx;
	USHORT			lwSysType = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->signal_kind & CMD_SYS_CHK_CPRID;
	DLT_REFW_TBL*	pReFwInfoTbl = gt_dlw_refw_all_tbl[lwSysType];

	cm_Assert(CMD_ASL_NOLEVEL, cpri_no, "[m_dl_fl_fwrep_rsp] ENTER");

	/* 対RE状態遷移(FHM自律)へ入力する */
	m_dl_fhm_main(bufp, cpri_no);

	/* 対RE状態が全てアイドルなら対REC状態をアイドルへ遷移 */
	for(idx = 0; idx < D_RRH_CPRINO_RE_MAX; idx++)
	{
		if(pReFwInfoTbl[idx].re_sta != DLD_RESTA_IDLE)
		{
			return;
		}
	}
	gt_dlw_rec_all_tbl[lwSysType]->rec_sta = EC_DLM_MAIN_STN_IDLE;

	cm_Assert(CMD_ASL_NOLEVEL, cpri_no, "[m_dl_fl_fwrep_rsp] RETURN");
}

/* @} */

