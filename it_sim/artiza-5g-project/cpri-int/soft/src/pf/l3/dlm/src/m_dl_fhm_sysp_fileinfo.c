/*!
 * @skip   $Id$
 * @file   m_dl_fhm_sysp_fileinfo.c
 * @brief  ファイル情報報告通知(シスパラ)
 * @date   2015/08/22 FPT)Yen Create
 * @date   2015/10/20 TDIPS)sasaki Update
 * @date   2015/10/22 TDIPS)sasaki Update

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
 *  @brief  ファイル情報報告通知(シスパラ)を対象配下REへ送信
 *  @note   
 *  @param  awCpriNo [in]  the CPRI no
 *  @param  awSysType [in]  Systerm type 3G/LTE
 *  @return VOID
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01816
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2015/10/20 TDIPS)sasaki M-RRU-ZSYS-01816 Update IT2問処No.118対処
 *  @date   2015/10/22 TDIPS)sasaki Update IT2問処No.130対処
 */
INT m_dl_fhm_sysp_fileinfo_req(USHORT awCpriNo, USHORT awSysType)
{
	CMT_TSKIF_CPRIRCV_FILEINFREQ	ltFileInfReq;
	UINT							llResult;
	UINT							llSize[CMD_MAX_SYSNUM] = 
										{ DLD_SYSPAR3G_SIZE,
										  DLD_SYSPARS3G_SIZE };

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	#endif
	/* メモリクリア処理 */
	cm_MemClr( &ltFileInfReq, sizeof(CMT_TSKIF_CPRIRCV_FILEINFREQ) );
	/* Make msg ファイル情報報告通知 */
	/* Event No: (0x4001)RE Forwarding Notice */
	ltFileInfReq.head.uiEventNo = CMD_TSKIF_RE_FWDNTC;
	/* Signal kind: ファイル情報報告通知 */
	ltFileInfReq.head.uiSignalkind = CMD_CPRID_FILEINFREQ + awSysType;
	/* CPRI番号 */
	ltFileInfReq.cprircv_inf.link_num = awCpriNo;
	/* CPRI信号データ部 */
	/* 信号種別 */
	ltFileInfReq.cpridat_fileinfreq.signal_kind = CMD_CPRID_FILEINFREQ + awSysType;
	/* 対象データ */
	ltFileInfReq.cpridat_fileinfreq.data		= CMD_SYSPAR;
	/* トータルサイズ */
	ltFileInfReq.cpridat_fileinfreq.size		= llSize[awSysType];
	/* ファイル名: 対象データ「1：システムパラメータ」のときは、Don’t careとする．*/

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d] uiEventNo[%d], uiSignalkind[%x], link_num[%d], signal_kind[%x], data, size  \n",__LINE__, 
				ltFileInfReq.head.uiEventNo,
								ltFileInfReq.head.uiSignalkind,
										ltFileInfReq.cprircv_inf.link_num,
												ltFileInfReq.cpridat_fileinfreq.signal_kind,
														ltFileInfReq.cpridat_fileinfreq.data,
																ltFileInfReq.cpridat_fileinfreq.size);
	#endif
	/* ファイル情報報告通知(シスパラ)を対象配下REへ送信 */
	llResult = l3_com_sendMsg( CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0,
								&ltFileInfReq,
								sizeof(CMT_TSKIF_CPRIRCV_FILEINFREQ));
	if(D_RRH_OK != llResult)
	{
		cm_Assert( CMD_ASL_USELOW, llResult,"[m_dl_fhm_sysp_fileinfo_req] Send Msg NG" );
	}

	/* ファイル情報報告応答結果を初期化 */
	gt_dlw_refw_all_tbl[awSysType][awCpriNo-1].fileinf_rep_rslt = DLD_RESLT_IDLE;

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
	return llResult;
}


/*!
 *  @brief  タイマ起動 システムパラメータ情報報告確認タイマ
 *  @note   
 *  @param  awCpriNo [in]  the CPRI no
 *  @param  awSysType [in]  Systerm type 3G/LTE
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 */

VOID m_dl_fhm_sysp_fileinfo_timersta(USHORT awCpriNo, USHORT awSysType)
{
	UINT							llResult;
	INT								llErrcd;
	USHORT							lwTimerId;

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	#endif
	
	lwTimerId = CMD_TIMID_SYSPRM_INF + awCpriNo +  D_RRH_CPRINO_RE_MAX * awSysType - CMD_NUM1;

	/* タイマ起動 システムパラメータ情報報告確認タイマ */
	llResult = cm_TStat( 	lwTimerId,
							CMD_TIMVAL_SYSPRM_INF,			/* 1000 msec */
							CMD_TIMMD_SINGLE,
							CMD_TSKIF_TIMOUTNTC, CMD_TSKID_DLM, &llErrcd);
	/*Start timer NG*/
	if(llResult != CMD_RES_OK)
	{
		cm_Assert( CMD_ASL_USELOW, lwTimerId, "システムパラメータ情報報告確認タイマ Start NG" );
	}

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d] lwTimerId:[%d] llResult:[%d]\n",__LINE__, lwTimerId, llResult);
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
	return ;
}
/* @} */

