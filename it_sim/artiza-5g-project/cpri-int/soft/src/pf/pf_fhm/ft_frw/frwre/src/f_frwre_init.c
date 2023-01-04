/*!
 * @skip  $ld:$
 * @file  f_frwre_init.c
 * @brief the entrance of pf_frwre task
 * @date  2015/08/04 TDIPS)sasaki create
 * @date  2015/08/28 TDIPS)sasaki update
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

 /*!
 * @addtogroup RRH_PF_FRW
 * @{
 */

#include "f_frwre_inc.h"


/*!
 *  @brief  Task initialization process function
 *  @note   This function is processed as follows.\n
 *          - Initialize the internal table
 *          - Change thread state
 *          - Send "Thread init completion to response" to pf_main
 *  @return VOID
 *  @retval none
 *  @date  2015/08/04 TDIPS)sasaki create
 *  @date  2015/08/28 TDIPS)sasaki レビュー指摘no.117対処
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_frwre_init(VOID)
{
	T_SYS_INIT_COMP_NTC 		tInitCompNTC;

	memset((VOID*)&tInitCompNTC, 0,sizeof(tInitCompNTC));
	
	/**********************************************/
	/* RAM table initialization 				  */
	/**********************************************/
	memset(&f_frwrew_runhis_tbl,0,sizeof(f_frwrew_runhis_tbl));

	f_frwrew_assert_p = f_cmn_com_assert_th_create( D_RRH_LOG_AST_DEFAULT , "frwre thread");

	/************************************************************/
	/* Task Init completion notice								*/
	/************************************************************/	
	/* 1. Edit the message header		*/
	tInitCompNTC.head.uiEventNo = D_SYS_MSGID_INIT_COMP_NTC;
	tInitCompNTC.head.uiLength = sizeof(tInitCompNTC);

	/* 2. Send Task Init completion notice	*/
	f_com_msgQSendFHM( D_SYS_THDQID_PF_F_MAIN, 
					(CHAR*)&tInitCompNTC,
					sizeof( T_SYS_INIT_COMP_NTC ), 
					D_SYS_THDQID_PF_FRQRE, 
					f_frwrew_thrState, 
					D_SYS_MSGID_INIT_COMP_NTC );

	return ;	
}

/* @} */
