/*!
 * @skip   $Id$
 * @file   m_dl_rec_fsnd_waiting_fwsnd.c
 * @brief  the processing for "ファイルデータ送信" receiving.
 * @date   2015/08/25 FPT)VietCV Create
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
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "ファイルデータ送信" receiving
 *  対RE状態遷移:ファイルデータ送信中(FW)
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 * 	@date   2015/08/25 FPT)VietCV Create
 *  @date   2015/09/21 FPT)Yen update comment(correct offset + update 送信済みデータサイズ )
 * 	@date   2015/10/02 TDIPS)sasaki Update 再ダウンロード準備処理を修正
*/

VOID m_dl_rec_fsnd_waiting_fwsnd(VOID* bufp, USHORT cpri_no)
{
	UINT								a_div_no	= CMD_NUM0;		/* 分割番号			*/

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_fsnd_waiting_fwsnd] ENTER" );

	/* 分割番号取得 */
	a_div_no = (((CMT_TSKIF_CPRIRCV_FILEDATSND*)bufp)->cpridat_filedatsnd.sgmtno1 << DLD_BIT16) | 
				((CMT_TSKIF_CPRIRCV_FILEDATSND*)bufp)->cpridat_filedatsnd.sgmtno2;

#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s a_div_no: %d\n",__LINE__,__FUNCTION__, a_div_no );
#endif
	if (a_div_no == CMD_NUM1)
	{
		/* 再ダウンロード準備 */
		gt_dlw_refw_tbl[cpri_no - 1].trx_div_no = 0;
		gt_dlw_refw_tbl[cpri_no - 1].trx_data_size = 0;
		/* ファイルデータ送信完了報告結果初期化 */
		gt_dlw_refw_tbl[cpri_no-1].filedata_trans_rslt = DLD_RESLT_IDLE;
	}

	/* ファイルデータ送信転送指示 */
	m_dl_rec_fwsnd_waiting_fwsnd(bufp, cpri_no);

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_fsnd_waiting_fwsnd] RETURN" );

}


/*!
 *  @brief  the processing for send "ファイルデータ送信転送指示" 
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/09/23 FPT)Yen create
*/

VOID m_dl_rec_fwsnd_waiting_fwsnd(VOID* bufp, USHORT cpri_no)
{
	UINT								lldatsize	= CMD_NUM0;		/* データサイズ		*/
	CMT_TSKIF_CPRIRCV_FILEDATSNDINT		ltFileDatSnd;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_fwsnd_waiting_fwsnd] ENTER" );
	/* データサイズ取得 */
	lldatsize = (((CMT_TSKIF_CPRIRCV_FILEDATSND*)bufp)->cpridat_filedatsnd.datsize1 << DLD_BIT16) | 
				 ((CMT_TSKIF_CPRIRCV_FILEDATSND*)bufp)->cpridat_filedatsnd.datsize2;
#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s lldatsize: %d\n",__LINE__,__FUNCTION__, lldatsize);
#endif
	
	/* ファイルデータ送信転送指示(※2) */
	memset(&ltFileDatSnd, 0, sizeof(ltFileDatSnd));
	ltFileDatSnd.head.uiEventNo =  CMD_TSKIF_FILEDATSND_TRANS_IND;
	
	ltFileDatSnd.cpridat_filedatsndint.offset = gt_dlw_rec_tbl.rcv_data_size - lldatsize;
	ltFileDatSnd.cprircv_inf.link_num = cpri_no;
	
	ltFileDatSnd.cpridat_filedatsndint.sgmtno1 = ((CMT_TSKIF_CPRIRCV_FILEDATSND*)bufp)->cpridat_filedatsnd.sgmtno1;
	ltFileDatSnd.cpridat_filedatsndint.sgmtno2 = ((CMT_TSKIF_CPRIRCV_FILEDATSND*)bufp)->cpridat_filedatsnd.sgmtno2;
	
	ltFileDatSnd.cpridat_filedatsndint.datsize1 = ((CMT_TSKIF_CPRIRCV_FILEDATSND*)bufp)->cpridat_filedatsnd.datsize1;
	ltFileDatSnd.cpridat_filedatsndint.datsize2 = ((CMT_TSKIF_CPRIRCV_FILEDATSND*)bufp)->cpridat_filedatsnd.datsize2;
	
	ltFileDatSnd.cpridat_filedatsndint.signal_kind = CMD_CPRID_FILEDATSND + CMD_SYS_LTE;
	
	l3_com_sendMsg(CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0, &ltFileDatSnd, sizeof( CMT_TSKIF_CPRIRCV_FILEDATSNDINT));

	/* 送信済み分割番号 */
	gt_dlw_refw_tbl[cpri_no - CMD_NUM1].trx_div_no++;
	/* 送信済みデータサイズ */
	gt_dlw_refw_tbl[cpri_no - CMD_NUM1].trx_data_size += lldatsize;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_fwsnd_waiting_fwsnd] RETURN" );
	return;

	return;
}
/** @} */
