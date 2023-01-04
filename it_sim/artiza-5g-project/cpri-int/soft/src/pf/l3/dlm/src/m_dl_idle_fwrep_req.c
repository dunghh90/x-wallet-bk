/*!
 * @skip   $Id$
 * @file   m_dl_idle_fwrep_req.c
 * @brief  the processing for "firmware information report request" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/09/30 TDIPS)sasaki Update
 * @date   2015/10/26 TDIPS)sasaki Update
 * @date   2015/10/27 TDIPS)sasaki Update
 * @date   2015/11/17 TDIPS)sasaki Update
 * @date   2015/11/18 TDIPS)sasaki Update

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
 *  @brief  the processing for "firmware information report request" receiving at the アイドル state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01906
 *  @Bug_No M-RRU-ZSYS-01914
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/09/17 FPT)Yen add "FHM保持FWファイル情報を取得".
 *  @date   2015/09/30 TDIPS)sasaki Update FHM保持FWファイル(FHM)情報の保存先誤りを修正
 *  @date   2015/10/26 TDIPS)sasaki M-RRU-ZSYS-01906 Update IT2問処No.139対処
 *  @date   2015/10/27 TDIPS)sasaki M-RRU-ZSYS-01914 Update IT2問処No.147水平展開 ファームウェアファイル報告応答のバージョン情報を対RE状態テーブルに保存するよう修正
 *  @date   2015/11/17 TDIPS)sasaki Update IT3問処No.83対処
 *  @date   2015/11/18 TDIPS)sasaki Update REC CNT-N/E切替問題対応
 */
VOID m_dl_idle_fwrep_req(VOID* bufp, USHORT cpri_no)
{
	UINT			a_sigchk;							/* result of checking signal */
	USHORT			a_mjr_ver	= CMD_NUM0;				/* major version	*/
	USHORT			a_min_ver	= CMD_NUM0;				/* mini version	*/
	USHORT			lwSystemType;
	USHORT			lwReConnSave;
	INT				llRrhRtn;
	T_RRH_SIDE_USE	ltRrhSideUse;						/* FHM保持FWファイル運用面 */
	T_RRH_FW_INFO	ltRrhFwInfo;						/* FHM保持FWファイル情報 */
	CHAR			lcBuf[128];							/* ログ編集用 */
	USHORT			lwReNo;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_idle_fwrep_req] ENTER" );
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	/**************************/
	/* data getting processing*/
	/**************************/
	a_sigchk = m_cm_chksig(((CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->signal_kind,&lwSystemType);
	if(a_sigchk != CMD_RES_OK)
	{
		m_dl_cm_CRFirmRspSnd(CMD_MAKER_RENG, a_mjr_ver, a_min_ver,lwSystemType);

		BPF_COM_LOG_ASSERT_DWL(	D_RRH_LOG_AST_LV_ERROR, "validate signal NG(ret=%d,sys=%d)", a_sigchk, lwSystemType);
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	/* 3Gシステムからの要求の場合は、3G優先でも「ファームウェアファイルなし」応答する */
	if(lwSystemType == CMD_SYS_3G)
	{
		BPF_COM_LOG_ASSERT_DWL( D_RRH_LOG_AST_LV_INFO, "No FirmWare(sys=%d)", lwSystemType );
		m_dl_cm_CRFirmRspSnd(CMD_FRM_NOFILE, a_mjr_ver, a_min_ver, CMD_SYS_3G); 
		
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		
		return;
	}
	/* FHMファイル、REファイル共にここでFWファイル情報を共有メモリから取得する */
	/* FHM保持FWファイル運用面取得 */
	llRrhRtn = f_cmn_com_side_use_get(&ltRrhSideUse);
	if( D_RRH_OK != llRrhRtn )
	{
		BPF_COM_LOG_ASSERT_DWL( D_RRH_LOG_AST_LV_ERROR, "[f_cmn_com_side_use_get] False(ret=%d)", llRrhRtn );
	}
	else
	{
		cm_MemClr(&ltRrhFwInfo, sizeof(T_RRH_FW_INFO));

		/*  FHMファイル */
		gt_dlw_dlm_tbl.mk_jdg = 'X';

		/* FHM保持FWファイル情報(運用面)取得 */
		llRrhRtn = f_cmn_com_fw_info_fhm_get(ltRrhSideUse.use_side, &ltRrhFwInfo);
		if( D_RRH_OK != llRrhRtn )
		{
			cm_Assert( CMD_ASL_USELOW, CMD_NUM0, "[f_cmn_com_fw_info_fhm_get] False" );
		}
		gt_dlw_dlm_tbl.usmjr_ver = ltRrhFwInfo.major_ver;
		gt_dlw_dlm_tbl.usmin_ver = ltRrhFwInfo.minor_ver;

		/* FHM保持FWファイル情報(待機面)取得 */
		llRrhRtn = f_cmn_com_fw_info_fhm_get((ltRrhSideUse.use_side ^ 0x1), &ltRrhFwInfo);
		if( D_RRH_OK != llRrhRtn )
		{
			cm_Assert( CMD_ASL_USELOW, CMD_NUM0, "[f_cmn_com_fw_info_fhm_get] False" );
		}
		gt_dlw_dlm_tbl.mjr_ver = ltRrhFwInfo.major_ver;
		gt_dlw_dlm_tbl.min_ver = ltRrhFwInfo.minor_ver;
	}

	/* Loop RE#1~16 */
	for(lwReNo = CMD_NUM0; lwReNo < CMD_NUM16; lwReNo++)
	{
		cm_MemClr(&ltRrhFwInfo, sizeof(T_RRH_FW_INFO));
		/* FHM保持FWファイル情報 (RE-FW)取得 */
		f_cmn_com_fw_info_re_get(lwReNo,  &ltRrhFwInfo);
		if( D_RRH_OK != llRrhRtn )
		{
			sprintf( &lcBuf[0], "[f_cmn_com_fw_info_re_get(ReNo:%d)] False", lwReNo);
			cm_Assert( CMD_ASL_USELOW, CMD_NUM0, &lcBuf[0] );
		}

		/* 同一メーカRE接続有無を退避 */
		lwReConnSave = gt_dlw_mnt_reinf[lwReNo + 1].re_conn_exist;
		/* FHM保持REファイル情報テーブルに共有メモリ上のデータを展開 */
		cm_MemCpy(&gt_dlw_mnt_reinf[lwReNo + 1], &ltRrhFwInfo, sizeof(T_RRH_FW_INFO));
		/* 同一メーカRE接続有無を再設定 */
		gt_dlw_mnt_reinf[lwReNo + 1].re_conn_exist = lwReConnSave;
		/* 分割数算出 */
		gt_dlw_mnt_reinf[lwReNo + 1].div_num = (gt_dlw_mnt_reinf[lwReNo + 1].file_size + CMD_FILEDAT_MAX - 1) / CMD_FILEDAT_MAX;
	}

	/* 対RE状態遷移(REC主導)へ入力する(L3状態が起動中か運用中のREを対象) */
	m_dl_rec_main(bufp, cpri_no);

	/* 実行中DL開始要因に1:RECを設定 */
	gt_dlw_dlm_tbl.dlstart_factor = DLD_FACTOR_REC;
	
	/* REC主導ファームウェア報告応答集約中 */
	gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_FW_REP_CON;

	/* ファームウェアファイル報告応答2作成 */
	m_dl_rl_fwrep_rsp2(NULL, 0);

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
	
	return ;
}

/* @} */

