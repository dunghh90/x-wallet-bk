/*!
 * @skip   $Id$
 * @file   m_dl_fileDataTransRes_OK.c
 * @brief  the processing for "ファイルデータ送信完了報告応答" receiving with result is OK.
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
 *  @brief  the processing for "ファイルデータ送信完了報告応答" receiving with result is OK
 *  対REC状態:ファイルデータ送信完了報告応答(シスパラ)集約中
 *	対RE状態:ファイルデータ送信完了報告応答(FW)待ち
 *  @note   
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/20 FPT)VietCV Create
 *  @date   2015/09/10 FPT)DuongCD change result code
 */
 
VOID m_dl_fileDataTransRes_OK(USHORT cpri_no)
{
	INT							ldwerrcd;
	USHORT						lwTimerId;
	UINT						ldwResult;
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start cpri_no = %d\n",__LINE__,__FUNCTION__, cpri_no);
	#endif
	
	/* 当該対RE-FW情報に受信した応答結果を保存 */
	gt_dlw_refw_tbl[cpri_no-1].filedata_trans_rslt = DLD_RESLT_NML;
	
	/* 当該対RE-FW情報の
	起動用バージョン#nを更新 */
	gt_dlw_refw_tbl[cpri_no-1].min_ver = gt_dlw_mnt_reinf[CMD_NUM0].min_ver;
	gt_dlw_refw_tbl[cpri_no-1].mjr_ver = gt_dlw_mnt_reinf[CMD_NUM0].mjr_ver;
	
	/* 更新開始要因#nを初期化 */
	gt_dlw_refw_tbl[cpri_no-1].start_factor = DLD_UPDATE_FACTOR_NONE;
	
	/* 更新中リンク情報の応答CPRI#ビットOFF */
	gt_dlw_dlm_tbl.link_inf &= ~(CMD_ON << (cpri_no-1));
	
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
		cm_Assert( CMD_ASL_DBGLOW, ldwResult, "ファームウェア送信完了確認タイマ#n(※1)停止 NG" );
	}
	
	/* 運用中REファイル更新完了通知 */
	m_dl_reFileUpdateCompNtc(cpri_no, CMD_NML);
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
}
/** @} */
