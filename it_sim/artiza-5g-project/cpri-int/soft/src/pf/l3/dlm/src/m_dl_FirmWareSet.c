/*!
 * @skip   $Id$
 * @file   m_dl_FirmWareSet.c
 * @brief  the processing for firmware setting.
 * @date   2008/07/29 FFCS)Wuh Create for eNB-008-001.
 * @date   2009/03/05  FFCS)Wuh modify for M-S3G-eNBPF-01471
 *                          stop writing FLASH after receiving CPRI message
 * @date   2015/09/30 TDIPS)sasaki Update
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */	
#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */
/*!
 *  @brief  the function processing for firmware setting.
 *  @note   This function is processed as follows.
 *          1) judge the message from 3G or S3G
 *          2) transfer the function to write RE file to flash for 3G or S3G
 *  @param  a_sys_typ		 	[in]  the type of system,3G or S3G
 *  @param  checksum            [in]  check sum
 *  @param  time                [in]  wait time
 *  @return Result code is returned.
 *  @retval D_OK	0:success 
 *  @retval D_NG	1:error
 *  @date   2008/07/30 FFCS)Wuh Create.
 *  @date   2015/09/30 TDIPS)sasaki Update MKレビュー指摘No.247対応
 */
UINT m_dl_FirmWareSet( USHORT a_sys_typ,USHORT checksum, USHORT time )
{
	UINT	a_result	= CMD_RES_OK;
	UINT	a_dl_type	= (dlw_downtyp_tbl == DLD_TSK_TYP_USE) ? CMD_USE_TYP : CMD_STAT_TYP;

	a_result = m_dl_FirmWareSet_S3G(a_dl_type, checksum, time);
	return a_result;
}

/* @} */

