/*!
 * @skip   $Id$
 * @file   m_dl_rl_ufsndcmprsp_waiting_refw_stop_req.c
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
 *  @brief  the processing for "REファイル更新中止要求" receiving at the 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2016/03/04 TDI)satou 更新中リンクが存在しない場合(FHMに対するダウンロードの場合)は処理なしに. \n
 *                               LTE-RECからDL中に、MasterのCPRIリンク断が発生すると状態初期化されてしまうのを修正
 */

VOID m_dl_rl_ufsndcmprsp_waiting_refw_stop_req(VOID* bufp, USHORT cpri_no)
{
	USHORT						lwLoop;
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_ufsndcmprsp_waiting_refw_stop_req] ENTER" );
	
	/* 更新中リンクが存在しない場合(FHMに対するダウンロードの場合)は処理なし */
	if ((DLD_FACTOR_REC == gt_dlw_dlm_tbl.dlstart_factor) && (0 == gt_dlw_dlm_tbl.link_inf))
	{
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "[m_dl_rl_fsndcmprsp_waiting_refw_stop_req] nop(fact=%d, link=%d)", gt_dlw_dlm_tbl.dlstart_factor, gt_dlw_dlm_tbl.link_inf);
		return;
	}

	/*更新中リンク情報のリンク断CPRI#ビットOFF */
	gt_dlw_dlm_tbl.link_inf &= ~(CMD_ON << (cpri_no -1));
	
	/* 対RE状態遷移(REC主導)へ入力する */
	m_dl_rec_main(bufp, cpri_no);
	
	/* 全てのREをループする (16 RE)*/
	for( lwLoop = CMD_NUM0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
	{
		if(DLD_RESTA_IDLE != gt_dlw_refw_tbl[lwLoop ].re_sta)
		{
			 /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
			gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_USE_FILEDATATRANS_FINREP_WAIT; 
			
#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
			return;
		}
	}
	/* 1: アイドル */
	gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_IDLE;
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_ufsndcmprsp_waiting_refw_stop_req] RETURN" );
	
	return ;
}

/* @} */

