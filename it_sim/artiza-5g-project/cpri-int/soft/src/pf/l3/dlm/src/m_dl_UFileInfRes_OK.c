/*!
 * @skip   $Id$
 * @file   m_dl_UFileInfRes_OK.c
 * @brief  the processing for "運用中ファイル情報報告応答" receiving with result is OK.
 * @date   2015/08/20 FPT)VietCV Create
 * @date   2015/10/02 TDIPS)sasaki Update

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
 *  @brief  the processing for "運用中ファイル情報報告応答" receiving with result is OK
 *  @note   
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/20 FPT)VietCV Create
 *  @date   2015/09/03 FPT)DuongCD add stop timer, change condition to set bufer size
 *  @date   2015/10/02 TDIPS)sasaki Update MKレビュー指摘No.230対応
 */
 
void m_dl_UFileInfRes_OK(USHORT cpri_no)
{
//	CMT_TSKIF_CPRIRCV_UFILEDATSNDINT	ltFileDatSnd;
	ULONG								llLoop;
	ULONG								llDataBufSize;
	UINT								offset;

	if(dlw_handle == CMD_NUM0)
	{
		cm_Assert(	CMD_ASL_USELOW, dlw_handle, "not save fileDataSendMsg");
		return;
	}
	offset = (UINT)(dlw_handle + (sizeof(UINT) * 8));
	/* ファイルデータ送信転送指示 */
	for (llLoop = CMD_NUM0; llLoop < gt_dlw_rec_tbl.rcv_div_no; llLoop++)
	{
		if (((llLoop + 1) * CMD_FILEDAT_MAX) <= gt_dlw_rec_tbl.rcv_data_size)
		{
			llDataBufSize = CMD_FILEDAT_MAX;
		}
		else
		{
			llDataBufSize = gt_dlw_rec_tbl.rcv_data_size - llLoop * CMD_FILEDAT_MAX;
		}
#if 0
//		memset(&ltFileDatSnd, 0, sizeof(ltFileDatSnd));
//		ltFileDatSnd.head.uiEventNo =  CMD_TSKIF_FILEDATSND_TRANS_IND;
//		ltFileDatSnd.cprircv_inf.link_num = cpri_no;
//
//		ltFileDatSnd.cpridat_ufiledatsndint.offset = llLoop * CMD_FILEDAT_MAX ;
//		
//		ltFileDatSnd.cpridat_ufiledatsndint.sgmtno1 = (llLoop + 1) >> DLD_BIT16;
//		ltFileDatSnd.cpridat_ufiledatsndint.sgmtno2 = (llLoop + 1) & 0xFFFF;
//		
//		ltFileDatSnd.cpridat_ufiledatsndint.datsize1 = llDataBufSize >> DLD_BIT16;
//		ltFileDatSnd.cpridat_ufiledatsndint.datsize2 = llDataBufSize & 0xFFFF;
//		
//		ltFileDatSnd.cpridat_ufiledatsndint.signal_kind = CMD_CPRID_UFILEDATSND + CMD_SYS_LTE;
//		
//		l3_com_sendMsg(CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0, &ltFileDatSnd, sizeof( CMT_TSKIF_CPRIRCV_UFILEDATSNDINT));
#endif
		m_dl_fileDatSndMsg_sndNtc(cpri_no, (CMD_CPRID_UFILEDATSND + CMD_SYS_LTE), (llLoop+1), llDataBufSize, (UCHAR *)offset);
		offset += llDataBufSize;
	}
	
	/* 送信済み分割番号 */
	gt_dlw_refw_tbl[cpri_no - CMD_NUM1].trx_div_no = gt_dlw_rec_tbl.rcv_div_no;
	/* 送信済みデータサイズ */
	gt_dlw_refw_tbl[cpri_no - CMD_NUM1].trx_data_size = gt_dlw_rec_tbl.rcv_data_size;
	/* 対RE状態#n(※1)をファイルデータ送信中(FW)へ遷移 */
	gt_dlw_refw_tbl[cpri_no-1].re_sta = DLD_RESTA_USE_FILE_TRANS;

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
	return;
}

VOID m_dl_fileDatSndMsg_sndNtc(USHORT cpri_no, USHORT msgKind, UINT sgmtno, UINT datSize, UCHAR *offset)
{
	CMT_TSKIF_CPRIRCV_UFILEDATSND	*ltuFileDatSndp;
	UINT							ldrtn;
	USHORT							msgSize;
	INT		endcd, rtn;						/*!< 終了コー繝?					*/
	
	msgSize = sizeof( CMT_TSKIF_CPRIRCV_UFILEDATSND ) - CMD_FILEDAT_MAX + datSize;
	/* バッファ取得 */
	ldrtn = BPF_RU_IPCM_PROCMSG_ADDRGET(
										E_BPF_RU_IPCM_BUFF_KIND_LAPBS,	/*!< Buffer種別(LAPB送信用)			*/
										msgSize,							/*!< size						*/
										(void **)&ltuFileDatSndp,			/*!< 通信用共有メモリのポイン繧?*/
										&endcd);						/*!< エラーコード				*/
	/* 戻り値判定 */
	if( ldrtn != CMD_RES_OK ){
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW, ldrtn,"BPF_RU_IPCM_PROCMSG_ADDRGET NG");
		return;
	}
//	ltuFileDatSndp->head.uiEventNo = CMD_TSKIF_FILEDATSND_TRANS_IND;
	ltuFileDatSndp->head.uiLength = msgSize;
	ltuFileDatSndp->head.uiDstPQueueID = D_RRH_PROCQUE_L2;
	ltuFileDatSndp->head.uiSrcPQueueID = D_RRH_PROCQUE_L3;
	ltuFileDatSndp->head.uiSrcTQueueID = CMD_TSKID_DLM;
//	ltFileDatSndp->cprircv_inf.link_num = cpri_no;
	ltuFileDatSndp->cpridat_ufiledatsnd.signal_kind = msgKind;
	
	ltuFileDatSndp->cpridat_ufiledatsnd.sgmtno1 = (USHORT)(sgmtno >> DLD_BIT16);
	ltuFileDatSndp->cpridat_ufiledatsnd.sgmtno2 = (USHORT)(sgmtno & 0xFFFF);
	
	ltuFileDatSndp->cpridat_ufiledatsnd.datsize1 = (USHORT)(datSize >> DLD_BIT16);
	ltuFileDatSndp->cpridat_ufiledatsnd.datsize2 = (USHORT)(datSize & 0xFFFF);
	
	memcpy(ltuFileDatSndp->cpridat_ufiledatsnd.fildat, offset, datSize);
	rtn = m_cm_L3MsgSend_RE(cpri_no, ltuFileDatSndp);
	if( rtn != CMD_RES_OK )
	{
		cm_BFree( ltuFileDatSndp );
		cm_Assert(	CMD_ASL_USELOW, ldrtn,"m_cm_L3MsgSend_RE NG");
	}
	usleep(5000);
	return;
}

/** @} */
