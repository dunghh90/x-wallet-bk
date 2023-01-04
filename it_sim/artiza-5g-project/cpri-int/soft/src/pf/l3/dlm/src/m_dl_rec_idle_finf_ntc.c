/*!
 * @skip   $Id$
 * @file   m_dl_rec_idle_finf_ntc.c
 * @brief  Event:ファイル情報報告通知（対象データ：シスパラ）, 対RE状態:アイドル
 * @date   2015/08/22 FPT)Yen Create

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
 *  @brief  Event:ファイル情報報告通知（対象データ：シスパラ）, 対RE状態:アイドル
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01855
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2015/10/08 FPT)Quynh M-RRU-ZSYS-01855 Fix bug IT1 応答結果の設定値誤りを修正
 *  @date   2015/10/13 FPT)Yen update (check layer3 status)
 *  @date   2015/10/15 FPT)Yen update review comment 260
 *  @date   2021/03/02 M&C)Tri.hn Update based on No.27 (seq 3-7) - Responde NG if 3G
 */

VOID m_dl_rec_idle_finf_ntc(VOID* bufp, USHORT cpri_no)
{
	CMT_TSKIF_CPRIRCV_FILEINFREQ*	ltFileInfoReq_p = NULL;/* CPRI信号データ部	*/
	UINT							ldwResult;
	USHORT							lwSysType;
	USHORT							lwData;
	T_RRH_LAYER3					ltLayer3;

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	#endif
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_rec_idle_finf_ntc] ENTER" );

	/* CPRI信号データ部取得 */
	ltFileInfoReq_p = (CMT_TSKIF_CPRIRCV_FILEINFREQ*)bufp;

	lwSysType = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->signal_kind & CMD_SYS_CHK_CPRID;

	if (CMD_SYS_3G == lwSysType)
	{
		m_dl_cm_CRFileInfRspSnd(CMD_INFRPT_NG, lwSysType);
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	/* 対象データを取得 */
	lwData = ltFileInfoReq_p->cpridat_fileinfreq.data;

	if( CMD_FRM == lwData )
	{
		BPF_COM_LOG_ASSERT_DWL( D_RRH_LOG_AST_LV_ERROR,"[m_dl_rec_idle_finf_ntc] FW data(date=%d)", lwData );
		
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	/* 送信可能な配下REの判定 */
	/* Layer3状態取得 */
	ldwResult = f_cmn_com_layer3_get( lwSysType, &ltLayer3 );
	/* L3リンク状態(②RE起動中状態 || 運用中状態か)をチェックする */
	if(( CMD_OK != ldwResult ) || 
	   (( DLD_L3_STA_2 != ltLayer3.layer3_re[cpri_no - CMD_NUM1] ) && ( DLD_L3_STA_4 != ltLayer3.layer3_re[cpri_no - CMD_NUM1] ) ))
	{
		BPF_COM_LOG_ASSERT_DWL( D_RRH_LOG_AST_LV_ERROR,"[m_dl_rec_idle_finf_ntc] Layer3 Sts NG(ret=%d,l3sta=%d)", ldwResult, ltLayer3.layer3_re[cpri_no - CMD_NUM1] );
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]ldwResult[%d], ltLayer3.layer3_re[cpri_no - CMD_NUM1(%d)]:[%d] \n",__LINE__,ldwResult, (cpri_no - CMD_NUM1), ltLayer3.layer3_re[cpri_no - CMD_NUM1]);
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

//	/* ファイル情報報告応答結果(シスパラ)をNGで初期化 */
//	gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].fileinf_rep_rslt = DLD_RESLT_NG;
	/* ファイル情報報告通知(シスパラ)を対象配下REへ送信 */
	ldwResult = m_dl_fhm_sysp_fileinfo_req( cpri_no, lwSysType );

	if( CMD_OK == ldwResult )
	{
		/* 対RE状態 => ファイル情報報告応答(シスパラ)待ち */ 
		gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].re_sta = DLD_RESTA_FILEINF_SYSPRM_WAIT;
	}
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_rec_idle_finf_ntc] RETURN" );

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
	return ;
}

/* @} */

