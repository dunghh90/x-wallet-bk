/*!
 * @skip  $ld:$
 * @file  f_nmas_init.c
 * @brief the entrance of pf_nma task
 * @date  2019/02/21 KCN)kamimoto create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

 /*!
 * @addtogroup RRH_PF_NMAS
 * @{
 */

#include "f_nmas_inc.h"


/*!
 *  @brief  Task initialization process function
 *  @note   This function is processed as follows.\n
 *          - Initialize the internal table
 *          - Change thread state
 *          - Send "Thread init completion to response" to pf_main
 *  @return VOID
 *  @retval none
 *  @date  2019/02/21 KCN)kamimoto  Create\n
 *  @warning	N/A
 *  @FeatureID	PF_File-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_nmas_init(VOID* msgP)
{
	T_SYS_INIT_COMP_NTC 		tInitCompNTC;
	INT							iRet;
	INT							errcd = 0;

	memset((VOID*)&tInitCompNTC, 0,sizeof(tInitCompNTC));
	
	/**********************************************/
	/* RAM table initialization 				  */
	/**********************************************/
	f_nmasw_thrID = D_SYS_THDID_PF_NMAS;

	f_nmasw_assert_p = f_cmn_com_assert_th_create( D_RRH_LOG_AST_DEFAULT , "nmas thread");

	iRet = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_LUT_LOG,(VOID**)&f_nmas_lut_log,&errcd);
	if(iRet != BPF_RU_IPCM_OK){
		cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_ERROR, "E_RRH_SHMID_APL_LUT_LOG error %d", errcd);
	}
	else{
		f_nmas_LutLogDecode();
	}
	
	/************************************************************/
	/* Task Init completion notice								*/
	/************************************************************/	
	/* 1. Edit the message header		*/
	tInitCompNTC.head.uiEventNo = D_SYS_MSGID_INIT_COMP_NTC;
	tInitCompNTC.head.uiLength= sizeof(tInitCompNTC);

	/* 2. Send Task Init completion notice	*/
	f_com_msgQSend( D_SYS_THDQID_PF_MAIN, 
					(CHAR*)&tInitCompNTC,
					sizeof( T_SYS_INIT_COMP_NTC ), 
					D_SYS_THDQID_PF_NMAS, 
					f_nmasw_thrState, 
					D_SYS_MSGID_INIT_COMP_NTC );

	return ;	
}

/* @} */
