/*!
 * @skip   $Id$
 * @file   m_dl_rl_ufwrep_cnf_to.c
 * @brief  the processing for "ファームウェアファイル報告確認(更新用)" receiving.
 * @date   2015/08/18 FPT)DuongCD Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2010
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"												/* MDIF common head file			*/
#include "m_dl_header.h"												/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "ファームウェアファイル報告確認(更新用)" receiving at the 運用中ファームウェア報告応答集約中 REC state
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)DuongCD Create
 */

VOID m_dl_rl_ufwrep_cnf_to(VOID* bufp, USHORT cpri_no)
{	
	
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_ufwrep_cnf_to] ENTER" );
	
	
	/* 対RE状態遷移(REC主導)へ入力する */
	m_dl_rec_main(bufp, cpri_no);
	
	/* call A_function */
	m_dl_rl_ufwrep_rsp2(bufp, cpri_no);
	
	cm_Assert( CMD_ASL_RETURN, CMD_NUM0,	"[m_dl_rl_ufwrep_cnf_to] RETURN" );
	return ;

}

/* @} */

