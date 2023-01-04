/*!
 * @skip   $Id$
 * @file   m_dl_SysParaSet.c
 * @brief  the processing for systemparameter setting.
 * @date   2008/07/30 FFCS)Wuh Create for eNB-008-001.
 * @date   2008/08/05 FFCS)Change the method of sector chip writing by the new HS.
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"			/* MDIF common head file			*/
#include "m_dl_header.h"			/* download management task head file */

/*!
 *  @brief  the function processing for firmware setting.
 *  @note   This function is processed as follows.
 *          1) judge the message from 3G or S3G
 *          2) transfer the function to write systemparameter for 3G or S3G
 *  @param  a_sys_typ		 	[in]  the type of system,3G or S3G
 *	@return Result code is returned.
 *  @retval D_OK	0:success 
 *  @retval D_NG	1:error
 *  @date   2008/07/30 FFCS)Wuh Create.
 */

UINT m_dl_SysParaSet( USHORT a_sys_typ )
{
	UINT	a_result	= CMD_RES_OK;

	if( CMD_SYS_3G == a_sys_typ )
	{
		a_result = m_dl_SysParaSet_3G();
	}
	else
	{
		a_result = m_dl_SysParaSet_S3G();
	}

	return a_result;
	
}

/* @} */

