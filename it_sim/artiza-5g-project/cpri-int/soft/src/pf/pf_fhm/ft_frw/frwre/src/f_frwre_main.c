/*!
 * @skip  $ld:$
 * @file  f_frwre_main.c
 * @brief the entrance of pf_frwre thread
 * @date 2015/08/04 TDIPS)sasaki create
 * @date 2015/08/21 TDIPS)sasaki コメント修正
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

 /*!
 * @addtogroup RRH_PF_FRW
 * @{
 */
#include "f_frwre_inc.h"

/* @{ */
/**
* @brief frwre management main function
* @note
* 
* @return None
* @date 2015/08/04 TDIPS)sasaki create
* @date 2015/08/21 TDIPS)sasaki コメント修正
* @warning    N/A
* @FeatureID  RRH-007-000
* @Bug_No N/A
* @CR_No  N/A
* @TBD_No N/A
*
*/

VOID f_frwre_main( VOID )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	CHAR	cRcvMsg[1024];    /* receievd message buffer*/
	INT		rcvsize;
	INT		errcd;
	
	/* スレッド名表示		*/
	prctl(PR_SET_NAME, "pf_frwre", 0, 0, 0);
	
	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_F_PF,				/* ProcessID		*/
									D_SYS_THDID_PF_FRQRE,			/* ThreadID			*/
									D_SYS_THDQID_PF_FRQRE,				    /* ThreadQueueID	*/
								    (unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/
	
	/************************************************************/
	/* Initialize the tables									*/
	/************************************************************/
	f_frwre_init( ); 				   
	
	/* Change thread state into running */
	f_frwrew_thrState = D_FRWRE_THRDSTA_RUN;
	
	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	while( D_SYS_NUM1 )
	{
		memset(cRcvMsg, 0, sizeof(cRcvMsg));

		/************************************************************/
		/* Receive the message from queue							*/
		/************************************************************/
		f_com_msgQReceive( D_SYS_THDQID_PF_FRQRE, 
						   sizeof(cRcvMsg), 
						   cRcvMsg, &rcvsize, &errcd );

		/* save running history */
		f_frwre_cm_RunHisSet(D_FRWRE_RUNHIS_FLG_RCV, (UCHAR *)cRcvMsg);
		
		cmn_com_assert_th_get(f_frwrew_assert_p,  D_RRH_LOG_AST_LV_ENTER,"EvtNo:%08X",((T_RRH_HEAD*)cRcvMsg)->uiEventNo);
		/* analyze and dispatch message   */
		f_frwre_anlz( cRcvMsg );
		cmn_com_assert_th_get(f_frwrew_assert_p,  D_RRH_LOG_AST_LV_RETURN,"EvtNo:%08X",((T_RRH_HEAD*)cRcvMsg)->uiEventNo);
	}
	
	return;
}
/* @} */

/* @} */
