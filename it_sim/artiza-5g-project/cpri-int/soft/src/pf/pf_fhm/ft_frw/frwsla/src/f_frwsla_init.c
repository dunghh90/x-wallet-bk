/*!
 * @skip  $ld:$
 * @file  f_frwsla_init.c
 * @brief the entrance of pf_frwsla task
 * @date  2014/12/12 FFCS)linb create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014-2015
 */

 /*!
 * @addtogroup RRH_PF_FRWSLA
 * @{
 */

#include "f_frwsla_inc.h"


/*!
 *  @brief  Task initialization process function
 *  @note   This function is processed as follows.\n
 *          - Initialize the internal table
 *          - Change thread state
 *          - Send "Thread init completion to response" to pf_main
 *  @return VOID
 *  @retval none
 * @date  2014/12/12 FFCS)linb create\n
 *  @warning	N/A
 *  @FeatureID	RRH-011-012
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_frwsla_init(VOID)
{
	T_SYS_INIT_COMP_NTC 		tInitCompNTC;

	memset((VOID*)&tInitCompNTC, 0,sizeof(tInitCompNTC));
	
	/**********************************************/
	/* RAM table initialization 				  */
	/**********************************************/
	memset(&f_frwslaw_runhis_tbl,0,sizeof(f_frwslaw_runhis_tbl));

	f_frwslaw_assert_p = f_cmn_com_assert_th_create( D_RRH_LOG_AST_DEFAULT , "frwsla thread");

	/************************************************************/
	/* Task Init completion notice								*/
	/************************************************************/	
	/* 1. Edit the message header		*/
	tInitCompNTC.head.uiEventNo = D_SYS_MSGID_INIT_COMP_NTC;
	tInitCompNTC.head.uiLength= sizeof(tInitCompNTC);

	/* 2. Send Task Init completion notice	*/
	f_com_msgQSendFHM( D_SYS_THDQID_PF_F_MAIN, 
					(CHAR*)&tInitCompNTC,
					sizeof( T_SYS_INIT_COMP_NTC ), 
					D_SYS_THDQID_PF_FRQS, 
					f_frwslaw_thrState, 
					D_SYS_MSGID_INIT_COMP_NTC );

	return ;	
}

/* @} */
