/*!
 * @skip   $Id$
 * @file   m_dl_rl_fsndcmprsp_waiting_sysp_stop_req.c
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
 *  @brief  the processing for "REファイル更新中止要求" receiving at the ファイルデータ送信完了報告応答(シスパラ)集約中 REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)DuongCD Create
 */
VOID m_dl_rl_fsndcmprsp_waiting_sysp_stop_req(VOID* bufp, USHORT cpri_no)
{
	USHORT				lwSysType;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fsndcmprsp_waiting_sysp_stop_req] ENTER" );
	
	/* 対RE状態遷移(REC主導)へ入力する */
	m_dl_rec_main(bufp, cpri_no);

	lwSysType = ((CMT_TSKIF_REFILNEWSTP *)bufp)->systerm;
	m_dl_rl_fsndcmprsp_waiting_sysp_proc(lwSysType);

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fsndcmprsp_waiting_sysp_stop_req] RETURN" );

	return ;
}

/* @} */

