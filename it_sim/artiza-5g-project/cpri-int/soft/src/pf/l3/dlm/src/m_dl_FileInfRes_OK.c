/*!
 * @skip   $Id$
 * @file   m_dl_FileInfRes_OK.c
 * @brief  the processing for "ファイル情報報告応答" receiving with result is OK.
 * @date   2015/08/20 FPT)VietCV Create
 * @date   2015/09/30 TDIPS)sasaki Update 

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2010
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */

/*!
 *  @brief  the processing for "ファイル情報報告応答" receiving with result is OK
 *  対REC状態遷移:ファイルデータ送信(FW)待ち state.
 *	対RE状態遷移: ファイル情報報告応答(FW)待ち
 *  @note   
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/20 FPT)VietCV Create
 *  @date   2015/09/21 FPT)Yen update comment(change USHORT -> ULONG )
 *  @date   2015/09/30 TDIPS)sasaki update 転送するファイルデータ送信の分割番号設定値誤りを修正
 */
 
void m_dl_FileInfRes_OK(USHORT cpri_no)
{
	INT								ldwerrcd;
	USHORT							lwTimerId;
	ULONG							llLoop;
	ULONG							llDataBufSize;
	UINT							ldwResult;
	CMT_TSKIF_CPRIRCV_FILEDATSNDINT	ltFileDatSnd;
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	#endif
	
	/* 対RE-FW情報テーブルに
	受信した応答結果を保存 */
	gt_dlw_refw_tbl[cpri_no-1].fileinf_rep_rslt = DLD_RESLT_NML;
	
	lwTimerId = (cpri_no - 1) + CMD_TIMID_FIRMFILE_INF;
	
	/* ファームウェアファイル情報報告確認タイマ#n停止 */
	ldwResult = cm_TStop( lwTimerId, &ldwerrcd);
	
	/* Stop timer NG */
	if( ldwResult != CMD_RES_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, ldwResult, "ファームウェアファイル情報報告確認タイマ#n停止 NG" );
	}
	
	/* ファイルデータ送信転送指示 */
	for (llLoop = CMD_NUM0; llLoop < gt_dlw_rec_tbl.rcv_div_no; llLoop++)
	{
		if (((llLoop + 1) * CMD_FILEDAT_MAX)<= gt_dlw_rec_tbl.rcv_data_size)
		{
			llDataBufSize = CMD_FILEDAT_MAX;
		}
		else
		{
			llDataBufSize = gt_dlw_rec_tbl.rcv_data_size - llLoop * CMD_FILEDAT_MAX;
		}
		
		memset(&ltFileDatSnd, 0, sizeof(ltFileDatSnd));
		ltFileDatSnd.head.uiEventNo = CMD_TSKIF_FILEDATSND_TRANS_IND;
		ltFileDatSnd.cprircv_inf.link_num = cpri_no;
		
		ltFileDatSnd.cpridat_filedatsndint.offset = llLoop * CMD_FILEDAT_MAX ;
		
		ltFileDatSnd.cpridat_filedatsndint.sgmtno1 = (llLoop + 1) >> DLD_BIT16;
		ltFileDatSnd.cpridat_filedatsndint.sgmtno2 = (llLoop + 1) & 0xFFFF;
		
		ltFileDatSnd.cpridat_filedatsndint.datsize1 = llDataBufSize >> DLD_BIT16;
		ltFileDatSnd.cpridat_filedatsndint.datsize2 = llDataBufSize & 0xFFFF;
		
		ltFileDatSnd.cpridat_filedatsndint.signal_kind = CMD_CPRID_FILEDATSND + CMD_SYS_LTE;
		
		l3_com_sendMsg(CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0, &ltFileDatSnd, sizeof( CMT_TSKIF_CPRIRCV_FILEDATSNDINT));
	}
	
	/* 送信済み分割番号 */
	gt_dlw_refw_tbl[cpri_no - CMD_NUM1].trx_div_no = gt_dlw_rec_tbl.rcv_div_no;
	/* 送信済みデータサイズ */
	gt_dlw_refw_tbl[cpri_no - CMD_NUM1].trx_data_size = gt_dlw_rec_tbl.rcv_data_size;

	/* 対RE状態#n(※1)をファイルデータ送信中(FW)へ遷移 */
	gt_dlw_refw_tbl[cpri_no-1].re_sta = DLD_RESTA_FILE_TRANS;
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
	return;
}
/** @} */
