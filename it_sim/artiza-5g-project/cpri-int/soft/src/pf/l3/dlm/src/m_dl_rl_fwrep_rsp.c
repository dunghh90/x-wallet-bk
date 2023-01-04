/*!
 * @skip   $Id$
 * @file   m_dl_rl_fwrep_rsp.c
 * @brief  the processing for "ファームウェアファイル報告応答" receiving.
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
 *  @brief  the processing for "ファームウェアファイル報告応答" receiving at the ファームウェア報告応答集約中 REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)DuongCD Create
 */

VOID m_dl_rl_fwrep_rsp(VOID* bufp, USHORT cpri_no)
{
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start cpri_no = %d\n",__LINE__,__FUNCTION__, cpri_no);
	#endif
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_ufwrep_rsp] ENTER" );
	
	/* 対RE状態遷移(REC主導)へ入力する */
	m_dl_rec_main(bufp, cpri_no);
	
	/*A_function */
	m_dl_rl_fwrep_rsp2(bufp, cpri_no);
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
	
	return ;
}

/* @} */

