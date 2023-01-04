/*!
 * @skip   $Id$
 * @file   m_dl_nop.c
 * @brief  
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
 *  @brief 
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 */

VOID m_dl_nop(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_nop] " );
	return ;
}

/*!
 *  @brief 
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 */

VOID m_dl_err(VOID* bufp, USHORT cpri_no)
{
	EC_DLM_MAIN_STN lwStaNo;
	EC_DLM_MAIN_EVT lwEventId;
	USHORT 			lwSysType = CMD_SYS_3G;
	CHAR errdata[CMD_NUM48];
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	/* for 3G */
	/* ��Ԕԍ��擾 */
    lwStaNo = m_dl_ownstn_get(lwSysType);	
	/* �C�x���g�擾 */
    lwEventId = m_dl_evt_get(lwSysType);
	/* ������������ */
	memset( errdata, 0, sizeof(errdata));
	snprintf(errdata, CMD_NUM48, "[m_dl_err][sys: %02d][cpr:%02d][stn:%02d][evt:%02d].",
											lwSysType,
														cpri_no,
																lwStaNo,
																			lwEventId );
	cm_Assert( CMD_ASL_DBGHIGH, CMD_NUM0, errdata );
	
	/* for S3G */
	lwSysType = CMD_SYS_S3G;
	/* ��Ԕԍ��擾 */
    lwStaNo = m_dl_ownstn_get(lwSysType);	
	/* �C�x���g�擾 */
    lwEventId = m_dl_evt_get(lwSysType);
	/* ������������ */
	memset( errdata, 0, sizeof(errdata));
	snprintf(errdata, CMD_NUM48, "[m_dl_err][sys: %02d][cpr:%02d][stn:%02d][evt:%02d].",
											lwSysType,
														cpri_no,
																lwStaNo,
																			lwEventId );
	cm_Assert( CMD_ASL_DBGHIGH, CMD_NUM0, errdata );
	
	return ;
}
/* @} */

