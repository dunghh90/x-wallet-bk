/*!
 * @skip  $ld:$
 * @file  f_frsp_init.c
 * @brief the entrance of pf_nma task
 * @date  2013/11/13 FFCS)linb create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

 /*!
 * @addtogroup RRH_PF_NMA
 * @{
 */

#include "f_nma_inc.h"


/*!
 *  @brief  Task initialization process function
 *  @note   This function is processed as follows.\n
 *          - Initialize the internal table
 *          - Change thread state
 *          - Send "Thread init completion to response" to pf_main
 *  @return VOID
 *  @retval none
 *  @date  2013/11/13 FFCS)linb create\n
 *  @date  2018/08/27 KCN)若水  Create\n
 *  @warning	N/A
 *  @FeatureID	PF_File-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_nma_init(VOID* msgP)
{
	T_SYS_INIT_COMP_NTC 		tInitCompNTC;

	memset((VOID*)&tInitCompNTC, 0,sizeof(tInitCompNTC));
	
	/**********************************************/
	/* RAM table initialization 				  */
	/**********************************************/
	f_nmaw_thrID = D_SYS_THDID_PF_NMA;

	f_nmaw_assert_p = f_cmn_com_assert_th_create( D_RRH_LOG_AST_DEFAULT , "nma thread");

	f_nmaw_thrState = D_NMA_THRDSTA_INIT ;

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
					D_SYS_THDQID_PF_NMA, 
					f_nmaw_thrState, 
					D_SYS_MSGID_INIT_COMP_NTC );

	return ;	
}

/* @} */
