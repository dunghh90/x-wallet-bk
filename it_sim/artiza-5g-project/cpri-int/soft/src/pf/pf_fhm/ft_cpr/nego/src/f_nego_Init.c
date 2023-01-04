/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_nego_init.c
 *  @brief  pf_nego thread init function
 *  @date   2015/04/22 FFCS)fuxg create
 *  @date   2015/8/4 TDIPS)ikeda rev.27800 2.4G�r�b�g���[�g�Ή�
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_NEGO
* @{ */

#include "f_sys_inc.h"
#include "f_nego_inc.h"

/**
* @brief 	pf_nego thread init function
* @note 	init pf_nego thread related common table.\n
*
* @return None
* @date 	2015/04/22 FFCS)fuxg create
* @warning	N/A
* @FeatureID	RRH-012-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_nego_Init()
{
	/**********************************************/
	/* Local parameter declaration				  */
	/**********************************************/
	T_SYS_INIT_COMP_NTC tInitCompNTC;
	
	/**********************************************/
	/* Local parameter initialization			  */
	/**********************************************/
	/* initialize the external event number*/
	f_nego_extEventNo = D_SYS_NUM0;
	
	/* Change thread state into init */
	f_nego_thrdState = D_NEGO_THRDSTA_INIT;

	f_nego_dbg_result_24g = D_SYS_ALLF_INT;
	f_nego_dbg_result_49g = D_SYS_ALLF_INT;
	f_nego_dbg_result_98g = D_SYS_ALLF_INT;

	memset(&f_nego_runhis_tbl, 0, sizeof(f_nego_runhis_tbl));

	/* EXT PLL Hold Over set */
	BPF_HM_DEVC_EXT_PLL_READ(D_RRH_REG_PLL_LMK4826_HOLDOVER_FORCE, &f_nego_pll_holdover_value);

	/************************************************************/
	/* Task Init completion notice								*/
	/************************************************************/
	memset(&tInitCompNTC,0,sizeof(tInitCompNTC));
	/* 1. Edit the message header		*/
	tInitCompNTC.head.uiEventNo = D_SYS_MSGID_INIT_COMP_NTC;
 	tInitCompNTC.head.uiLength = sizeof(tInitCompNTC);

	/* write running history */
	f_nego_cm_runHisSet(D_NEGO_SENDIEVEFLG, (UCHAR*)&tInitCompNTC);

	/* 2. Send Task Init completion notice	*/
	f_com_msgQSendFHM( D_SYS_THDQID_PF_F_MAIN, 
					(CHAR*)&tInitCompNTC,
					sizeof( T_SYS_INIT_COMP_NTC ), 
					D_SYS_THDQID_PF_NEGO, 
					f_nego_thrdState, 
					tInitCompNTC.head.uiEventNo);
	
	/* Change thread state into idle status */
	f_nego_thrdState = D_NEGO_THRDSTA_IDLE;
	
	return;
}
/* @} */

