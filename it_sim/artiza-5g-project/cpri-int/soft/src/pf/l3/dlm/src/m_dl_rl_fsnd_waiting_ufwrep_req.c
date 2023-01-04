/*!
 * @skip   $Id$
 * @file   m_dl_rl_fsnd_waiting_ufwrep_req.c
 * @brief  the processing for "運用中ファームウェアファイル報告要求" receiving.
 * @date   2015/11/18 TDIPS)sasaki Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "運用中ファームウェアファイル報告要求" receiving at the "REC主導ファイルデータ送信(FW)待ち"
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/11/18 TDIPS)sasaki Create REC CNT-N/E切替問題対応
 */

VOID m_dl_rl_fsnd_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no)
{
	CMT_TSKIF_CPRISND_UFIRMFILERES2*	pRsp;
	T_RRH_SIDE_USE 						ltside_fhm;
	T_RRH_FW_INFO						ltfwinfo_fhm;
	UINT 								ldwret_breq;
	UINT								ldwSnd_ret;
	INT									ldwShmGetResult;
	
	cm_Assert(CMD_ASL_NOLEVEL, cpri_no, "[m_dl_rl_fsnd_waiting_ufwrep_req] ENTER");

	/* MSG送信用のバッファを取得 */
	ldwret_breq = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(CMT_TSKIF_CPRISND_UFIRMFILERES2), CMD_NUM1, (VOID**)&pRsp);
	if(ldwret_breq != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET,
					"m_dl_rl_fsnd_waiting_ufwrep_req",
					"Buffer request error",
					ldwret_breq,
					sizeof(CMT_TSKIF_CPRISND_UFIRMFILERES2),
					CMD_NUM0);
		return;
	}
	
	/********************************************/
	/* ファームウェアファイル報告応答2作成 		*/
	/********************************************/
	pRsp->cpridat_ufirmfileres2.signal_kind = CMD_CPRID_UFIRMFILERES2 | CMD_SYS_S3G;

	/* 共有メモリFHM保持FWファイル運用面の呼び出し 取得用 */
	f_cmn_com_side_use_get(&ltside_fhm);
	
	/* 共有メモリFHM保持FWファイル情報(運用面)取得用 */
	ldwShmGetResult = f_cmn_com_fw_info_fhm_get(ltside_fhm.use_side, &ltfwinfo_fhm);
	if(ldwShmGetResult != D_RRH_OK)
	{
		pRsp->cpridat_ufirmfileres2.verinf_num = 0;
		pRsp->cpridat_ufirmfileres2.result = CMD_HDL_NG;
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "ufwrrsp2:NG(%d)", pRsp->cpridat_ufirmfileres2.result);
	}
	else
	{
		pRsp->cpridat_ufirmfileres2.verinf_num = 1;
		pRsp->cpridat_ufirmfileres2.result = CMD_NML;

		pRsp->cpridat_ufirmfileres2.firmfile[CMD_NUM0].makername = ltfwinfo_fhm.maker_id;
		pRsp->cpridat_ufirmfileres2.firmfile[CMD_NUM0].cur_mjr_ver = ltfwinfo_fhm.major_ver;
		pRsp->cpridat_ufirmfileres2.firmfile[CMD_NUM0].cur_mir_ver = ltfwinfo_fhm.minor_ver;

		/* 共有メモリFHM保持FWファイル情報(待機面)取得用 */
		(VOID)f_cmn_com_fw_info_fhm_get((ltside_fhm.use_side ^ 0x1), &ltfwinfo_fhm);
		pRsp->cpridat_ufirmfileres2.firmfile[CMD_NUM0].nes_mjr_ver = ltfwinfo_fhm.major_ver;
		pRsp->cpridat_ufirmfileres2.firmfile[CMD_NUM0].nes_mir_ver = ltfwinfo_fhm.minor_ver;
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "ufwrrsp2:Num=%d, FHM:%x=V%02d%02d", 
								pRsp->cpridat_ufirmfileres2.verinf_num, pRsp->cpridat_ufirmfileres2.firmfile[CMD_NUM0].makername, 
								pRsp->cpridat_ufirmfileres2.firmfile[0].cur_mjr_ver, pRsp->cpridat_ufirmfileres2.firmfile[0].cur_mir_ver);
	}

	/* タスク間ヘッダ部 */
	pRsp->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + CMD_NUM3 * sizeof(USHORT) + pRsp->cpridat_ufirmfileres2.verinf_num * sizeof(UFIRM_FILE_INF);

	/************************************************/
	/* (0x2131)ファームウェアファイル報告応答2送信	*/
	/************************************************/
	ldwSnd_ret = m_cm_L3MsgSend_REC(pRsp);
	if(ldwSnd_ret != CMD_L3_OK)
	{
		cm_Assert(CMD_ASL_USELOW, ldwSnd_ret, "MSG Snd NG");
	}

	return;
}
/* @} */

