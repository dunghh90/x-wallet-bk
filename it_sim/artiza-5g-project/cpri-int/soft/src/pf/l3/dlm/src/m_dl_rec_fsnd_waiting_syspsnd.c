/*!
 * @skip   $Id$
 * @file   m_dl_rec_fsnd_waiting_syspsnd.c
 * @brief  Event:ファイルデータ送信, 対RE状態:ファイルデータ送信(シスパラ)待ち
 * @date   2015/08/22 FPT)Yen Create
 * @date   2015/09/28 TDIPS)sasaki Update

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
 *  @brief  Event:ファイルデータ送信, 対RE状態:ファイルデータ送信(シスパラ)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2015/09/28 TDIPS)sasaki Update MKレビュー指摘No.244対応
 *  @date   2015/10/09 FPT)Yen update メッセージサイズ
 *  @date   2021/03/03 M&C) Update src based on No.27 (seq 3-9) - Responde NG if 3G
 */

VOID m_dl_rec_fsnd_waiting_syspsnd(VOID* bufp, USHORT cpri_no)
{
	CMT_TSKIF_CPRIRCV_FILEDATSND	ltFileDataSnd;
	UINT							lwSysType;
	UINT							llResult;
	UINT							llDivNo = CMD_NUM1; 			/* 分割番号は1固定 */
	UINT							llDataSize[CMD_MAX_SYSNUM] = 	/* データサイズは64byte(LTE)、32byte(3G)で固定 */
										{ DLD_SYSPAR3G_SIZE,
										  DLD_SYSPARS3G_SIZE
										};
	UCHAR*							dlw_rcvsyspa_tbl[CMD_MAX_SYSNUM] =
										{ dlw_rcvsyspa_tbl_3g, dlw_rcvsyspa_tbl_s3g };
	UINT							llMsgSize;

#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_rec_fsnd_waiting_syspsnd] ENTER" );

	lwSysType = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->signal_kind & CMD_SYS_CHK_CPRID;

	if (CMD_SYS_3G == lwSysType)
	{
		m_dl_cm_CRFileInfRspSnd(CMD_OTHR_ERR_RE, lwSysType);
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

//	/* ファイルデータ送信 */
//	m_dl_fhm_sysp_filedatasend_req(cpri_no, lwSysType, (INT*)dlw_rcvsyspa_tbl[lwSysType]);

	/* メモリクリア処理 */
	cm_MemClr( &ltFileDataSnd, sizeof(CMT_TSKIF_CPRIRCV_FILEDATSND) );

	/* Make msg ファイル情報報告通知 */
	/* Event No: (0x4001)RE Forwarding Notice */
	ltFileDataSnd.head.uiEventNo = CMD_TSKIF_RE_FWDNTC;
	/* Signal kind: ファイルデータ送信 */
	ltFileDataSnd.head.uiSignalkind = CMD_CPRID_FILEDATSND;

	ltFileDataSnd.cprircv_inf.link_num = cpri_no;
	/* CPRI信号データ部 */
	/* 信号種別 */
	ltFileDataSnd.cpridat_filedatsnd.signal_kind = CMD_CPRID_FILEDATSND + lwSysType;
	
	/* 分割番号1 */
	ltFileDataSnd.cpridat_filedatsnd.sgmtno1	= ( llDivNo >> DLD_BIT16 );
	/* 分割番号2 */
	ltFileDataSnd.cpridat_filedatsnd.sgmtno2	= ( llDivNo & 0x0000FFFF );
	/* データサイズ1 */
	ltFileDataSnd.cpridat_filedatsnd.datsize1	= ( llDataSize[lwSysType] >> DLD_BIT16 );
	/* データサイズ2 */
	ltFileDataSnd.cpridat_filedatsnd.datsize2	= ( llDataSize[lwSysType] & 0x0000FFFF );
	/* ファイルデータ */
	cm_MemCpy( &ltFileDataSnd.cpridat_filedatsnd.fildat[CMD_NUM0],
			   dlw_rcvsyspa_tbl[lwSysType], llDataSize[lwSysType] );

	llMsgSize = sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_CPRISIGSND_INF ) + 
				CMD_NUM5 * sizeof(USHORT) + 
				llDataSize[lwSysType] * sizeof(UCHAR);
	/* ファイルデータ送信を対象配下REへ送信 */
	llResult = l3_com_sendMsg( CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0,
								&ltFileDataSnd,
								llMsgSize);
	if(D_RRH_OK != llResult)
	{
		cm_Assert( CMD_ASL_USELOW, llResult,"[m_dl_rec_fsnd_waiting_syspsnd] File data send NG " );
	}

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_rec_fsnd_waiting_syspsnd] RETURN" );

#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif

	return ;
}

/* @} */

