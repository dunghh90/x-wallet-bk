/*!
 * @skip   $Id$
 * @file   m_dl_rec_fwreprsp_waiting_fwrep_req.c
 * @brief  the processing for "firmware information report request" receiving.
 * @date   2015/08/18 FPT)DuongCD Create

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
 *  @brief  the processing for "firmware information report request" receiving at the ファームウェアファイル報告応答待ち state .
 *  @note   ファームウェアファイル報告結果初期化ファームウェアファイル報告確認タイマ再起動
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)DuongCD Create
 */

VOID m_dl_rec_fwreprsp_waiting_fwrep_req(VOID* bufp, USHORT cpri_no)
{
	UINT			ldwResult;
	INT				ldwerrcd;
	USHORT			lwTimerId;


	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_fwreprsp_waiting_fwrep_req] ENTER" );
	
	/* LTE system*/
	lwTimerId = CMD_TIMID_FIRMFILE_REP + cpri_no -1;
	
	/* ファームウェアファイル報告確認タイマ#n(※1)停止 */
	ldwResult = cm_TStop( lwTimerId, &ldwerrcd);
	
	/* Stop timer NG */
	if( ldwResult != CMD_RES_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, ldwResult, "ファームウェアファイル報告確認タイマ Stop NG" );
	}
	
	/*  ファームウェアファイル報告結果初期化 */
	m_dl_rec_idle_fwrep_req(bufp, cpri_no);
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_fwreprsp_waiting_fwrep_req] RETURN" );
	return ;
}

/* @} */

