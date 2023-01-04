/*!
 * @skip  $ld:$
 * @file  f_nmas_main.c
 * @brief the entrance of pf_nmas thread
 * @date 2019/02/21 KCN)Kamimoto create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/**  
* @mainpage		Slave FLASH writing thread (pf_nmas)
* @section		Introdution		   
*				-# Receive flash erasing/writing from pf_fdl
*				   
*/

/**  
* @defgroup RRH_PF_NMAS    Slave FLASH writing thread 
* @ingroup RRH   
* This class is Slave FLASH writing thread
*/


 /*!
 * @addtogroup RRH_PF_NMAS
 * @{
 */
#include "f_nmas_inc.h"

/* @{ */
/**
* @brief nmaS management main function
* @note
* @param src_id [in] source task id
* @param event_no [in] event no
* @param buff_adr [in] the buffer address pointer of received message
* @return None
* @date  2019/02/21 KCN)kamimoto  Create\n
* @warning    N/A
* @FeatureID  PF_File-001-001-001
* @Bug_No N/A
* @CR_No  N/A
* @TBD_No N/A
*
*/

VOID f_nmas_main( VOID )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	CHAR cRcvMsg[1024];    /* receievd message buffer*/
	INT		rcvsize;
	INT		errcd;
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_nmas_main] ENTER" );
	
	/* スレッド名表示		*/
	prctl(PR_SET_NAME, "pf_nmas", 0, 0, 0);
	
	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_PF,								/* ProcessID		*/
									D_SYS_THDID_PF_NMAS,								/* ThreadID			*/
									D_SYS_THDQID_PF_NMAS,							/* ThreadQueueID	*/
									(unsigned long int)pthread_self()) ;			/* Rhread ID        */
	
	/************************************************************/
	/* Initialize the tables									*/
	/************************************************************/
	f_nmas_init(NULL); 				   
	
	/* Change thread state into running */
	f_nmasw_thrState = D_NMAS_THRDSTA_RUN;
	
	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	while(1)
	{
		memset(cRcvMsg, 0, sizeof(cRcvMsg));

		/************************************************************/
		/* Receive the message from queue							*/
		/************************************************************/
		f_com_msgQReceive( D_SYS_THDQID_PF_NMAS, 
						   sizeof(cRcvMsg), 
						   cRcvMsg, &rcvsize,&errcd);

		cmn_com_assert_th_get(f_nmasw_assert_p,  D_RRH_LOG_AST_LV_ENTER,"[f_nmas_main] RECIEVE");
		/* analyze and dispatch message   */
		f_nmas_anlz( cRcvMsg );
		cmn_com_assert_th_get(f_nmasw_assert_p,  D_RRH_LOG_AST_LV_RETURN,"[f_nmas_main] RETURN");
	}
	
	return;
}
/* @} */
