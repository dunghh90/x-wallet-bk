/*!
 * @skip   $Id$
 * @file   m_dl_rl_finfrsp_waiting_sysp_finf_rsp.c
 * @brief  Event:ファイル情報報告応答(L3信号：FHM←RE), 対REC状態:REC主導ファイル情報報告応答(シスパラ)集約中.
 * @date   2015/08/22 FPT)Yen Create
 * @date   2015/10/15 TDIPS)sasaki Update
 * @date   2015/10/26 TDIPS)sasaki Update

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
 *  @brief  Event:ファイル情報報告応答(L3信号：FHM←RE), 対REC状態:REC主導ファイル情報報告応答(シスパラ)集約中.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2015/08/31 FPT)Yen Update
 */

VOID m_dl_rl_finfrsp_waiting_sysp_finf_rsp(VOID* bufp, USHORT cpri_no)
{
	USHORT		lwSysType;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_finfrsp_waiting_sysp_finf_rsp] ENTER" );

	lwSysType = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->signal_kind & CMD_SYS_CHK_CPRID;

	/* 対RE状態遷移(REC主導)へ入力する */
	m_dl_rec_main(bufp, cpri_no);

	m_dl_rl_finfrsp_waiting_sysp_proc(lwSysType);
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_finfrsp_waiting_sysp_finf_rsp] RETURN" );

	return ;
}
/*!
 *  @brief  Event:ファイル情報報告応答(L3信号：FHM←RE), 対REC状態:REC主導ファイル情報報告応答(シスパラ)集約中.
 *  @note   
 *  @param  awSysType       [in]    system type
 *  
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01854
 *  @Bug_No M-RRU-ZSYS-01906
 *  @date   2015/08/31 FPT)Yen Create
 *  @date   2015/10/08 FPT)Duong M-RRU-ZSYS-01854 Fix bug IT1 タイマID誤りを修正
 *  @date   2015/10/15 TDIPS)sasaki Update IT2問処No.89対処
 *  @date   2015/10/15 TDIPS)sasaki Update IT2問処No.91水平展開
 *  @date   2015/10/26 TDIPS)sasaki M-RRU-ZSYS-01906 Update IT2問処No.139対処
 *  @date   2015/11/17 FPT)Yen update (その他未実装-015)
 */
VOID m_dl_rl_finfrsp_waiting_sysp_proc(USHORT awSysType)
{
	USHORT		lwReNo;
	USHORT		lwRspResult;
	USHORT		lwTimerId;
	USHORT		lwTimerVal;
	USHORT		lwDiscLinks = 0;
	INT			llResult;
	INT			llErrcd;
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_finfrsp_waiting_sysp_proc] ENTER" );

	if( CMD_SYS_3G == awSysType )
	{
		/* 1:情報報告NG */
		lwRspResult = CMD_INFRPT_NG;
	}
	else
	{
		/* 51:処理NG（RE）*/
		lwRspResult = CMD_HDL_NG;
	}
	/* loop all RE, check response's result */
	for(lwReNo = D_RRH_CPRINO_RE_MIN; lwReNo <= D_RRH_CPRINO_RE_MAX; lwReNo++)
	{
		/* 未応答有り */
		/* 対RE状態がファイル情報報告応答(シスパラ)待ち */
		if( DLD_RESTA_FILEINF_SYSPRM_WAIT == gt_dlw_refw_all_tbl[awSysType][lwReNo - CMD_NUM1].re_sta )
		{
			/* 対REC状態:REC主導ファイル情報報告応答(シスパラ)集約中 */
			/* 遷移なし */
#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
			return;
		}
		if( DLD_RESTA_IDLE == gt_dlw_refw_all_tbl[awSysType][lwReNo - CMD_NUM1].re_sta &&
			DLD_RESLT_IDLE == gt_dlw_refw_all_tbl[awSysType][lwReNo - CMD_NUM1].fileinf_rep_rslt)
		{
			lwDiscLinks++;
		}
		/* Checkファイル情報報告結果 */
		if( DLD_RESLT_NML == gt_dlw_refw_all_tbl[awSysType][lwReNo - CMD_NUM1].fileinf_rep_rslt )
		{
			lwRspResult = CMD_NML;
		}
	}
	/* タイマ停止 システムパラメータ情報報告確認タイマ */
	/* All Re start 1 timer */
	lwTimerId = CMD_TIMID_SYSPRM_INF + D_RRH_CPRINO_RE_MAX * awSysType;

	llResult = cm_TStop( lwTimerId, &llErrcd );
	/*Stop timer NG*/
	if(llResult != CMD_RES_OK)
	{
		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "システムパラメータ情報報告確認タイマ Stop NG" );
	}

	/* 配下REなし */
	if(lwDiscLinks == D_RRH_CPRINO_RE_MAX)
	{
		lwRspResult = CMD_NML;
	}

	/********************/
	/* CPRI信号送信処理 */
	/********************/
	/* ファイル情報報告応答送信処理起動 */
	m_dl_cm_CRFileInfRspSnd( lwRspResult, awSysType );

	if(CMD_NML != lwRspResult)
	{
		lwRspResult = DLD_RESLT_NG;
	}
	else
	{
		lwRspResult = DLD_RESLT_NML;
	}

	/* REC向けファイル情報報告結果設定 */
	(*gt_dlw_rec_all_tbl[awSysType]).fileinf_rep_rslt = lwRspResult;

	/* 全配下REからの応答受信済、かつ全てNG応答 */
	if( DLD_RESLT_NML != lwRspResult )
	{
		for(lwReNo = D_RRH_CPRINO_RE_MIN; lwReNo <= D_RRH_CPRINO_RE_MAX; lwReNo++)
		{
			gt_dlw_refw_all_tbl[awSysType][lwReNo-1].re_sta = DLD_RESTA_IDLE;
		}
		/* 対REC状態:アイドル */
		(*gt_dlw_rec_all_tbl[awSysType]).rec_sta = EC_DLM_MAIN_STN_IDLE;
	}
	else
	{
		/* タイマ起動 システムパラメータ送信確認タイマ */
		lwTimerId = CMD_TIMID_SYSPRMSND_3G + awSysType;
		lwTimerVal = cmw_sys_mng_tbl[awSysType].sys_timer;
		/* タイマ起動 システムパラメータ情報報告確認タイマ */
		llResult = cm_TStat( 	lwTimerId,
								TIMVAL_GET(lwTimerVal),			/* 単位:10 msec */
								CMD_TIMMD_SINGLE,
								CMD_TSKIF_TIMOUTNTC, CMD_TSKID_DLM, &llErrcd);
		/*Start timer NG*/
		if(llResult != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_USELOW, lwTimerId, "システムパラメータ送信確認イマ Start NG" );
		}
		/* 対REC状態:受信済み分割番号初期化 */
		(*gt_dlw_rec_all_tbl[awSysType]).rcv_div_no = 0;
		/* 対REC状態:受信済みデータサイズ初期化 */
		(*gt_dlw_rec_all_tbl[awSysType]).rcv_data_size = 0;
		/* 対REC状態:ファイルデータ送信(シスパラ)待ち */
		(*gt_dlw_rec_all_tbl[awSysType]).rec_sta = EC_DLM_MAIN_STN_FILEDATATRANS_SYSPARA_WAIT;
		
		/* ファイル情報報告通知のトータルデータサイズ保存 */
		/* Store in function m_dl_SysPro */

		/* REC向けファイルデータ送信完了報告応答結果設定 0:正常 */
		(*gt_dlw_rec_all_tbl[awSysType]).filedata_trans_rslt = CMD_NML;
	}
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_finfrsp_waiting_sysp_proc] RETURN" );
}
/* @} */

