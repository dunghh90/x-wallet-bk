/*!
 * @skip   $Id$
 * @file   m_dl_rec_ufinfrsp_waiting_ufwsnd.c
 * @brief  the processing for "運用中ファイルデータ送信" receiving.
 * @date   2015/08/18 FPT)DuongCD Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "運用中ファイルデータ送信" receiving at the 運用中ファイル情報報告応答(FW)待ち RE state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/28 FPT)DuongCD Create
 */

VOID m_dl_rec_ufinfrsp_waiting_ufwsnd(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_rec_ufinfrsp_waiting_ufwsnd] ENTER" );
	
	/* TBD */
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_rec_ufinfrsp_waiting_ufwsnd] RETURN" );
	return ;
}

/* @} */

