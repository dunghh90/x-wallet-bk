/*!
 * @skip   $Id$
 * @file   m_dl_fileDataTransRes_NG.c
 * @brief  the processing for "ファイルデータ送信完了報告応答" receiving with result is NG.
 * @date   2015/08/20 FPT)VietCV Create

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
 *  @brief  the processing for "ファイルデータ送信完了報告応答" receiving with result is NG
 *  対REC状態:ファイルデータ送信完了報告応答(シスパラ)集約中
 *	対RE状態:ファイルデータ送信完了報告応答(FW)待ち
 *  @note   
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/20 FPT)VietCV Create
 *  @date   2015/09/10 FPT)DuongCD change result code
 *	@date   2015/09/17 FPT)VietCT update
 *  @date   2015/10/08 FPT)Duong update MKレビュー指摘No.213対応
 */
 
VOID m_dl_fileDataTransRes_NG(USHORT cpri_no)
{
	USHORT				lwTimerId;
	UINT				ldwResult;
	INT				ldwerrcd;

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	#endif

	if (gt_dlw_refw_tbl[cpri_no-1].re_sta == DLD_RESTA_USE_FILE_COMP_WAIT)
	{
		/*ファームウェア送信完了確認(更新用)タイマ#n(※1)停止 */
		lwTimerId = (cpri_no - 1) + CMD_TIMID_UFIRMFILE_SNDFIN;
	}
	else
	{
		/* ファームウェア送信完了確認タイマ#n(※1)停止 */
		lwTimerId = (cpri_no - 1) + CMD_TIMID_FIRMFILE_SNDFIN;
	}
	
	ldwResult = cm_TStop( lwTimerId, &ldwerrcd);
	
	/* Stop timer NG */
	if( ldwResult != CMD_RES_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "タイマ#n停止 NG" );
	}
	
	/* 当該対RE-FW情報テーブルの
	ファイルデータ送信完了報告結果にNGを設定 */
	gt_dlw_refw_tbl[cpri_no-1].filedata_trans_rslt = DLD_RESLT_NG;
	
	/* 当該対RE-FW情報テーブルの
	使用中タイマを初期化 */
	gt_dlw_refw_tbl[cpri_no-1].use_timer = DLD_RERETIM_NONE;
	
	/* 更新開始要因#nを初期化 */
	gt_dlw_refw_tbl[cpri_no-1].start_factor = DLD_UPDATE_FACTOR_NONE;
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
}
/** @} */
