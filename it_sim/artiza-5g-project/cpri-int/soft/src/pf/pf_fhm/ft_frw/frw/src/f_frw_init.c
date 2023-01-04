/*!
 * @skip  $ld:$
 * @file  f_frw_init.c
 * @brief the entrance of pf_frw task
 * @date  2013/11/13 FFCS)linb create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

 /*!
 * @addtogroup RRH_PF_FRW
 * @{
 */

#include "f_frw_inc.h"


/*!
 *  @brief  Task initialization process function
 *  @note   This function is processed as follows.\n
 *          - Initialize the internal table
 *          - Change thread state
 *          - Send "Thread init completion to response" to pf_main
 *  @return VOID
 *  @retval none
 *  @date  2013/11/13 FFCS)linb create\n
 *  @date  2015/08/21 TDI)satou 設定するのみで使用していない変数を削除(f_frww_thrID)
 *  @warning	N/A
 *  @FeatureID	PF_File-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_frw_init(VOID)
{
	T_SYS_INIT_COMP_NTC 		tInitCompNTC;

	memset((VOID*)&tInitCompNTC, 0,sizeof(tInitCompNTC));
	
	/**********************************************/
	/* RAM table initialization 				  */
	/**********************************************/
	memset(&f_frww_runhis_tbl,0,sizeof(f_frww_runhis_tbl));

	f_frww_assert_p = f_cmn_com_assert_th_create( D_RRH_LOG_AST_DEFAULT , "frw thread");

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
					D_SYS_THDQID_PF_FRQ, 
					f_frww_thrState, 
					D_SYS_MSGID_INIT_COMP_NTC );

	return ;	
}

/* @} */
