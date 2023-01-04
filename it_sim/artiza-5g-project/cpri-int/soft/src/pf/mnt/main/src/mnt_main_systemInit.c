/*!
 *  @skip   $Id:$
 *  @file   mnt_main_systemInit.c
 *  @brief  system init
 *  @date   2013/11/20 FFCS)zhaodx Create for zynq
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */


 /*!
 * @addtogroup RRH_MNT_MAIN
 * @{
 */


/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "mnt_com_inc.h"
#include "mnt_main_inc.h"
#include "f_com_aplCom.h"
#include "rrhApi_Inv.h"
#include "rrhApi_L2.h"
#include "rrhApi_Cpri.h"


/********************************************************************************************************************/
/*!
 *  @brief  system init.
 *  @note   send system init complete notice to init process and init system
 *  @param  -      [in]  -
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2013/11/20 FFCS)zhaodx Create for zynq
 *  @date   2015/10/24 TDIPS)sasaki Warning対処
 */
/********************************************************************************************************************/
VOID mnt_main_systemInit(void)
{
	INT 				ret;
	INT					errcd;							/* error code				*/
	INT					rcvmsgsz;						/* receivied message size	*/
	UCHAR				rcvBuf[512];
//	USHORT linkno;
// 擬似環境 20210316
//#ifndef SIM_DEBUG
#if 0
// 擬似環境 20210316
	/* receive system init from init process*/
	ret = apl_com_msgQReceive(
			CMD_TSKID_INI,
			sizeof(rcvBuf),						/* received buffer size	*/
			(VOID *)rcvBuf, 					/* received buffer		*/
			&rcvmsgsz,							/* Received message length*/
			&errcd								/* Error code			*/
		);
	/* checking parameter */
	if( ret != D_RRH_OK ) {
		cm_Assert( CMD_ASL_DBGLOW, ret,
			(CHAR*)"[mnt_main_systemInit] Receive system init from init process NG" );
		return;
	}
#endif
#if 0
	for ( linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++ )
	{
		/* call api to subscribe cpri message */
		rrhApi_L2_Mpr_SubscribeCpriMsg(linkno, D_RRH_PROCQUE_MT, 0, D_RRH_NULL, (UINT *)mnt_comr_cprimsg_subTbl,MBD_CPRIMSGNUM);

		/* call api to subscribe cpri link state message */
		rrhApi_Cpri_Mpr_SubscribeLinkState(linkno, D_RRH_PROCQUE_MT, 0, D_RRH_NULL);
	}
#endif

	mnt_mainw_stateno = MNT_MAIN_ST_USE;

	return;
}


/* @} */

