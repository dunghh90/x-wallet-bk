/*!
 * @skip   $Id$
 * @file   m_dl_rl_fsnd_waiting_sysp_stop_req.c
 * @brief  the processing for "REファイル更新中止要求" receiving.
 * @date   2015/08/18 FPT)DuongCD Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2010
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "REファイル更新中止要求" receiving at the ファイルデータ送信(シスパラ)待ち REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/10/15 FPT)Duong update review comment 133
 */

VOID m_dl_rl_fsnd_waiting_sysp_stop_req(VOID* bufp, USHORT cpri_no)
{
	USHORT				lwLoop;
	USHORT				lwSysType;
	
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fsnd_waiting_sysp_stop_req] ENTER" );
	
	/* 対RE状態遷移(REC主導)へ入力する */
	m_dl_rec_main(bufp, cpri_no);

	lwSysType = ((CMT_TSKIF_REFILNEWSTP *)bufp)->systerm;
	
	/* 全てのREをループする (16 RE)*/
	for( lwLoop = CMD_NUM0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
	{
		if(DLD_RESTA_IDLE != gt_dlw_refw_all_tbl[lwSysType][lwLoop].re_sta)
		{
#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
			return;
		}
	}
	
	/* 全てNG応答 */
	/* アイドル */
	/* 対REC情報初期化 */
	(*gt_dlw_rec_all_tbl[lwSysType]).rec_sta = EC_DLM_MAIN_STN_IDLE;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fsnd_waiting_sysp_stop_req] RETURN" );

	return ;
}

/* @} */

