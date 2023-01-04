/*!
 * @skip   $Id$
 * @file   m_dl_rl_fsnd_waiting_syspsnd_cnf_to.c
 * @brief  Event:システムパラメータ送信確認タイマタイムアウト通知, 対REC状態:ファイルデータ送信(シスパラ)待ち.
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
 *  @brief Event:システムパラメータ送信確認タイマタイムアウト通知, 対REC状態:ファイルデータ送信(シスパラ)待ち.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2015/10/13 FPT)Yen update (del: change rec_sta)
 *  @date   2015/10/14 FPT)DuongCD update comment No.263
 */

VOID m_dl_rl_fsnd_waiting_syspsnd_cnf_to(VOID* bufp, USHORT cpri_no)
{
	USHORT		lwSysType;
	USHORT		lwToResult[CMD_MAX_SYSNUM]= {	CMD_TIMOUT,
												CMD_TIMOUT_S3G
								 			};

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_fsnd_waiting_syspsnd_cnf_to] ENTER" );

	lwSysType = (( CMT_TSKIF_HEAD *)bufp)->uiSrcPQueueID;

	/* REC向けファイルデータ送信完了報告応答結果にタイムアウトを設定 */
	(*gt_dlw_rec_all_tbl[lwSysType]).filedata_trans_rslt = lwToResult[lwSysType];

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_fsnd_waiting_syspsnd_cnf_to] RETURN" );

	return ;
}


/* @} */

