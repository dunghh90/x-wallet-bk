/*!
 * @skip   $Id$
 * @file   m_dl_rec_finfrsp_waiting_sysp_finf_rsp.c
 * @brief  Event:ファイル情報報告応答(FHM←RE), 対RE状態:ファイル情報報告応答(シスパラ)待ち
 * @date   2015/08/22 FPT)Yen Create

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
 *  @brief  Event:ファイル情報報告応答(FHM←RE), 対RE状態:ファイル情報報告応答(シスパラ)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 */

VOID m_dl_rec_finfrsp_waiting_sysp_finf_rsp(VOID* bufp, USHORT cpri_no)
{
	CMT_TSKIF_CPRIRCV_FILEINFRES*	ltFileInfoRes_p = NULL;/* CPRI信号データ部	*/
	UINT							lwResult;
	UINT							lwSysType;

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_rec_finfrsp_waiting_sysp_finf_rsp] ENTER" );

	/* CPRI信号データ部取得 */
	ltFileInfoRes_p = (CMT_TSKIF_CPRIRCV_FILEINFRES*)bufp;
	lwSysType = ltFileInfoRes_p->cpridat_fileinfres.signal_kind & CMD_SYS_CHK_CPRID;
	/* 対象データを取得 */
	lwResult = ltFileInfoRes_p->cpridat_fileinfres.result;
	if(CMD_NML != lwResult)
	{
		lwResult = DLD_RESLT_NG;
	}
	else
	{
		lwResult = DLD_RESLT_NML;
	}
	
	/* ファイル情報報告応答結果(シスパラ)保存 */
	gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].fileinf_rep_rslt = lwResult;
	
	/* NGの場合 */
	if( DLD_RESLT_NML != lwResult )
	{
		/* 対RE状態 => アイドル */ 
		gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].re_sta = DLD_RESTA_IDLE;
	}
	/* OKの場合 */
	else
	{
		/* 対RE状態 => ファイルデータ送信(シスパラ)待ち */
		gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].re_sta = DLD_RESTA_FILE_TRANS_WAIT;
	}

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_rec_finfrsp_waiting_sysp_finf_rsp] RETURN" );

	return ;
}

/* @} */

