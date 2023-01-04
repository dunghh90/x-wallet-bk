/*!
 * @skip   $Id$
 * @file   m_dl_rl_fwreprsp_waiting_fwrep_req.c
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
 *  @brief  the processing for "firmware information report request" receiving at the アイドル state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)DuongCD Create
 */

VOID m_dl_rl_fwreprsp_waiting_fwrep_req(VOID* bufp, USHORT cpri_no)
{
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fwreprsp_waiting_fwrep_req] ENTER" );
	
	/* 対RE状態遷移(REC主導)へ入力する(L3状態が起動中か運用中のREを対象) */
	m_dl_rec_main(bufp, cpri_no);
	
	/* REC主導ファームウェア報告応答集約中 */
	gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_FW_REP_CON;
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fwreprsp_waiting_fwrep_req] RETURN" );
	
	return ;
}

/* @} */

