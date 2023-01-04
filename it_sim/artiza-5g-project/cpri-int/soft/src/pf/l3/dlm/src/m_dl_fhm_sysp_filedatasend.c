/*!
 * @skip   $Id$
 * @file   m_dl_fhm_sysp_filedatasend.c
 * @brief  ファイルデータ送信
 * @date   2015/08/22 FPT)Yen Create
 * @date   2015/09/26 TDIPS)sasaki Update
 * @date   2015/10/20 TDIPS)sasaki Update

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
 *  @brief  ファイルデータ送信を対象配下REへ送信
 *  @note   
 *  @param  awCpriNo [in]  the CPRI no
 *  @param  awSysType [in]  Systerm type 3G/LTE
 *  @param  acData_p [in]  Data
 *  @return VOID
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01702
 *  @Bug_No M-RRU-ZSYS-01816
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2015/09/26 TDIPS)sasaki M-RRU-ZSYS-01702 Update データサイズ2設定誤りを修正
 *  @date   2015/10/09 FPT)Yen update メッセージサイズ
 *  @date   2015/10/20 TDIPS)sasaki M-RRU-ZSYS-01816 Update IT2問処No.118対処
 */
INT m_dl_fhm_sysp_filedatasend_req(USHORT awCpriNo, USHORT awSysType, INT* acData_p)
{
	CMT_TSKIF_CPRIRCV_FILEDATSND	ltFileDataSnd;
	CMT_TSKIF_CPRIRCV_FILSNDFINNTC	ltFileDataSndFin;
	UINT							llResult;
	UINT							llDivNo = CMD_NUM1; 			/* 分割番号は1固定 */
	UINT							llDataSize[CMD_MAX_SYSNUM] = 	/* データサイズは64byte(LTE)、32byte(3G)で固定 */
										{ DLD_SYSPAR3G_SIZE,
										  DLD_SYSPARS3G_SIZE
										};
	USHORT							lwChksumCalc;
	UINT							llMsgSize;

	/* メモリクリア処理 */
	cm_MemClr( &ltFileDataSnd, sizeof(CMT_TSKIF_CPRIRCV_FILEDATSND) );
	cm_MemClr( &ltFileDataSndFin, sizeof(CMT_TSKIF_CPRIRCV_FILSNDFINNTC) );

	/* Make msg ファイル情報報告通知 */
	/* Event No: (0x4001)RE Forwarding Notice */
	ltFileDataSnd.head.uiEventNo = CMD_TSKIF_RE_FWDNTC;
	/* Signal kind: ファイルデータ送信 */
	ltFileDataSnd.head.uiSignalkind = CMD_CPRID_FILEDATSND + awSysType;

	ltFileDataSnd.cprircv_inf.link_num = awCpriNo;
	/* CPRI信号データ部 */
	/* 信号種別 */
	ltFileDataSnd.cpridat_filedatsnd.signal_kind = CMD_CPRID_FILEDATSND + awSysType;
	
	/* 分割番号1 */
	ltFileDataSnd.cpridat_filedatsnd.sgmtno1	= ( llDivNo & 0xFFFF0000 ) >> DLD_BIT16;
	/* 分割番号2 */
	ltFileDataSnd.cpridat_filedatsnd.sgmtno2	= ( llDivNo & 0x0000FFFF );
	/* データサイズ1 */
	ltFileDataSnd.cpridat_filedatsnd.datsize1	= ( llDataSize[awSysType] & 0xFFFF0000 ) >> DLD_BIT16;
	/* データサイズ2 */
	ltFileDataSnd.cpridat_filedatsnd.datsize2	= ( llDataSize[awSysType] & 0x0000FFFF );
	/* ファイルデータ */
	cm_MemCpy( &ltFileDataSnd.cpridat_filedatsnd.fildat[CMD_NUM0],
			   acData_p, llDataSize[awSysType] );

	llMsgSize = sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_CPRISIGSND_INF ) + 
				CMD_NUM5 * sizeof(USHORT) + 
				llDataSize[awSysType] * sizeof(UCHAR);

	/* ファイルデータ送信を対象配下REへ送信 */
	llResult = l3_com_sendMsg( CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0,
								&ltFileDataSnd,
								llMsgSize);
	if(D_RRH_OK != llResult)
	{
		cm_Assert( CMD_ASL_USELOW, llResult,"[m_dl_fhm_sysp_filedatasend_req] File data send NG " );
		return llResult;
	}

	/* Make msg ファイルデータ送信完了報告通知 */
	/* Event No: (0x4001)RE Forwarding Notice */
	ltFileDataSndFin.head.uiEventNo = CMD_TSKIF_RE_FWDNTC;
	/* Signal kind: ファイルデータ送信 */
	ltFileDataSndFin.head.uiSignalkind = CMD_CPRID_FILSNDFINNTC + awSysType;
	/* CPRI信号データ部 */
	/* 信号種別 */
	ltFileDataSndFin.cpridat_filsndfinntc.signal_kind = CMD_CPRID_FILSNDFINNTC + awSysType;
	ltFileDataSndFin.cprircv_inf.link_num = awCpriNo;
	/* 対象データ */
	ltFileDataSndFin.cpridat_filsndfinntc.tgtdat	= CMD_SYSPAR;

	/* チェックサム計算処理 */
	cm_CheckSumCalcCpri(acData_p, llDataSize[awSysType], &lwChksumCalc);
	/* チェックサム */
	ltFileDataSndFin.cpridat_filsndfinntc.chksum	= lwChksumCalc;

	/* ファイルデータ送信完了報告通知を対象配下REへ送信 */
	llResult = l3_com_sendMsg( CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0,
								&ltFileDataSndFin,
								sizeof(CMT_TSKIF_CPRIRCV_FILSNDFINNTC));
	if(D_RRH_OK != llResult)
	{
		cm_Assert( CMD_ASL_USELOW, llResult,"[m_dl_fhm_sysp_filedatasend_req] File data send finish NG " );
	}

	/* ファイルデータ送信完了報告応答結果を初期化 */
	gt_dlw_refw_tbl[awCpriNo-1].filedata_trans_rslt = DLD_RESLT_IDLE;

	return llResult;
}

/*!
 *  @brief  タイマ起動 システムパラメータ送信完了確認タイマ
 *  @note   
 *  @param  awCpriNo [in]  the CPRI no
 *  @param  awSysType [in]  Systerm type 3G/LTE
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 */

VOID m_dl_fhm_sysp_filedatasendfin_timersta(USHORT awCpriNo, USHORT awSysType)
{
	UINT							llResult;
	INT								llErrcd;
	USHORT							lwTimerId;

	lwTimerId = CMD_TIMID_SYSPRM_SNDFIN + awCpriNo +  D_RRH_CPRINO_RE_MAX * awSysType - CMD_NUM1;

	/* タイマ起動 システムパラメータ送信完了確認タイマ */
	llResult = cm_TStat( 	lwTimerId,
							CMD_TIMVAL_SYSPRM_SNDFIN,			/* 8000 msec */
							CMD_TIMMD_SINGLE,
							CMD_TSKIF_TIMOUTNTC, CMD_TSKID_DLM, &llErrcd);
	/*Start timer NG*/
	if(llResult != CMD_RES_OK)
	{
		cm_Assert( CMD_ASL_USELOW, lwTimerId, "システムパラメータ送信完了確認タイマ Start NG" );
	}

	return ;
}
/* @} */

