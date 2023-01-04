/*!
 * @skip   $Id$
 * @file   m_dl_reFileUpdateNone.c
 * @brief  the processing RE file update none
 * @date   2015/08/20 FPT)VietCV Create
 * @date   2015/10/17 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */

/*!
 *  @brief  the processing RE file update none
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01791 
 *  @date   2015/08/20 FPT)VietCV Create
 *  @date   2015/08/20 FPT)DuongCD remove 運用中REファイル更新完了通知(7-2-9)
 *  @date   2015/10/14 FPT)DuongCD update comment No.277
 *  @date   2015/10/17 TDIPS)sasaki M-RRU-ZSYS-01791 update IT2問処No.96対処 配下への転送不要と判明した時点の DL開始要因初期化処理を削除 全配下REからOK応答なしのルートで応答結果223を返すよう修正
 */
VOID m_dl_reFileUpdateNone(VOID* bufp, USHORT cpri_no)
{
	CMT_TSKIF_CPRISND_FILSNDFINNTC ltCpriFilSndFinNtc;
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start cpri_no = %d\n",__LINE__,__FUNCTION__, cpri_no);
	#endif
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_reFileUpdateNone] ENTER" );
	
	/* (0x4001)RE Forwarding Notice(ファイルデータ送信完了報告通知(更新なし)) */
	ltCpriFilSndFinNtc.head.uiEventNo = CMD_TSKIF_RE_FWDNTC;
	ltCpriFilSndFinNtc.head.uiSignalkind = CMD_CPRID_FILSNDFINNTC + CMD_SYS_LTE;
	ltCpriFilSndFinNtc.cprisnd_inf.link_num = cpri_no;
	ltCpriFilSndFinNtc.cpridat_filsndfinntc.tgtdat = CMD_FRM_NONEW;
	
	ltCpriFilSndFinNtc.cpridat_filsndfinntc.signal_kind = CMD_CPRID_FILSNDFINNTC + CMD_SYS_LTE;
	
	l3_com_sendMsg(CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0, &ltCpriFilSndFinNtc, sizeof(CMT_TSKIF_CPRISND_FILSNDFINNTC));
	
	/* 更新開始要因#nを初期化 */
	gt_dlw_refw_tbl[cpri_no-1].start_factor = DLD_UPDATE_FACTOR_NONE;
	
	/* 更新中リンク情報の更新対象CPRI#ビットOFF */
	gt_dlw_dlm_tbl.link_inf &= ~(CMD_ON << (cpri_no-1));
	
//	/* 更新中リンクなし */
//	if (gt_dlw_dlm_tbl.link_inf == CMD_NUM0)
//	{
//		/* 実行中DL開始要因に0:未実行を設定 */
//		gt_dlw_dlm_tbl.dlstart_factor = DLD_UPDATE_FACTOR_NONE;
//	}
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
	return;
}
/** @} */
