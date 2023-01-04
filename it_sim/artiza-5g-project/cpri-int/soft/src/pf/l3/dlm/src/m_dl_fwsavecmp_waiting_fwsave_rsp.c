/*!
 * @skip   $Id$
 * @file   m_dl_fwsavecmp_waiting_fwsave_rsp.c
 * @brief  the processing for "FWファイル更新応答" receiving.
 * @date   2015/11/17 TDIPS)sasaki Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "フラッシュ書込フラグ" OFF
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/11/17 TDIPS)sasaki Create
 */

VOID m_dl_fwsavecmp_waiting_fwsave_rsp(VOID* bufp, USHORT cpri_no)
{
	cm_Assert(CMD_ASL_NOLEVEL, CMD_NUM0, "[m_dl_fwsavecmp_waiting_fwsave_rsp] ENTER");

	/* フラッシュ書込フラグをOFF */
	gt_dlw_dlm_tbl.write_flg = CMD_OFF;
	return ;
}

/* @} */

