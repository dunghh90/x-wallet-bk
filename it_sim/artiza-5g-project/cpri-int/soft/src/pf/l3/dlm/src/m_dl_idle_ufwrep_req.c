/*!
 * @skip   $Id$
 * @file   m_dl_idle_ufwrep_req.c
 * @brief  the processing for "運用中ファームウェアファイル報告要求" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/10/02 TDIPS)sasaki Update
 * @date   2015/10/26 TDIPS)sasaki Update

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
 *  @brief  the processing for "運用中ファームウェアファイル報告要求" receiving at the アイドル REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01906
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/10/02 TDIPS)sasaki Update MKレビュー指摘No.254対応
 *  @TBD_No 共有メモリからのシスパラ読込処理無効化(RECから転送されたシスパラの共有メモリへの保存処理が未実装の為)
 *  @date   2015/10/14 FPT)DuongCD update comment No.278
 *  @date   2015/10/26 TDIPS)sasaki M-RRU-ZSYS-01906 Update IT2問処No.139対処
 */
VOID m_dl_idle_ufwrep_req(VOID* bufp, USHORT cpri_no)
{
	UINT		a_sigchk;
	USHORT		a_mjr_ver = CMD_NUM0;
	USHORT		a_min_ver = CMD_NUM0;
	USHORT		a_mjr_staver = CMD_NUM0;
	USHORT		a_min_staver = CMD_NUM0;
	USHORT		lwSystemType;
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_idle_ufwrep_req] ENTER" );
    #ifdef FHM_DLM_FOR_DEBUG_IT1
    printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
    #endif

	/**************************/
	/* data getting processing*/
	/**************************/
	a_sigchk = m_cm_chksig(((CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->signal_kind, &lwSystemType);
	if(a_sigchk != CMD_RES_OK)
	{
		m_dl_cm_CRUSFirmRspSnd(CMD_MAKER_RENG, a_mjr_ver, a_min_ver, a_mjr_staver, a_min_staver);
		cm_Assert(CMD_ASL_USELOW, lwSystemType, "validate signal NG");
#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}

//TODO RECから転送されたシスパラの共有メモリへの保存処理が未実装のため
//	/* API to read cmw_sys_mng_tbl*/
//	rrhApi_Inv_Fsm_GetSystemPara(&cmw_sys_mng_tbl,sizeof(cmw_sys_mng_tbl));

	/* 実行中DL開始要因に1:RECを設定 */
	gt_dlw_dlm_tbl.dlstart_factor = DLD_FACTOR_REC;
	
	/* 対RE状態遷移(REC主導)へ入力する(L3状態が起動中か運用中のREを対象) */
	m_dl_rec_main(bufp, cpri_no);
	
	 /* 10: 運用中ファームウェア報告応答集約中 */
	gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_USE_FW_REP_CON;

	/* 運用中ファームウェアファイル報告応答2作成 */
	m_dl_rl_ufwrep_rsp2(NULL, 0);

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_idle_ufwrep_req] RETURN" );
	
	return ;
}

/* @} */

