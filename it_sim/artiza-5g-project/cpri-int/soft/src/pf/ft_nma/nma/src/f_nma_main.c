/*!
 * @skip  $ld:$
 * @file  f_nma_main.c
 * @brief the entrance of pf_nma thread
 * @date 2013/11/14 FFCS)linb create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/**  
* @mainpage		FLASH writing thread (pf_nma)
* @section		Introdution		   
*				-# Receive flash erasing/writing from pf_fmng
*				-# Receive flash erasing/writing from pf_diag
*				   
*/

/**  
* @defgroup RRH_PF_NMA    FLASH writing thread 
* @ingroup RRH   
* This class is FLASH writing thread
*/


 /*!
 * @addtogroup RRH_PF_NMA
 * @{
 */
#include "f_nma_inc.h"

/* @{ */
/**
* @brief nma management main function
* @note
* @param src_id [in] source task id
* @param event_no [in] event no
* @param buff_adr [in] the buffer address pointer of received message
* @return None
* @date 2013/11/14 FFCS)linb create
* @date  2018/08/27 KCN)若水  Create\n
* @warning    N/A
* @FeatureID  PF_File-001-001-001
* @Bug_No N/A
* @CR_No  N/A
* @TBD_No N/A
*
*/

VOID f_nma_main( VOID )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	CHAR cRcvMsg[1024];    /* receievd message buffer*/
	INT		rcvsize;
	INT		errcd;
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_nma_main] ENTER" );
	
	/* スレッド名表示		*/
	prctl(PR_SET_NAME, "pf_nma", 0, 0, 0);
	
	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_PF,								/* ProcessID		*/
									D_SYS_THDID_PF_NMA,								/* ThreadID			*/
									D_SYS_THDQID_PF_NMA,							/* ThreadQueueID	*/
									(unsigned long int)pthread_self()) ;			/* Rhread ID        */
	
	/************************************************************/
	/* Initialize the tables									*/
	/************************************************************/
	f_nma_init(NULL); 				   
	
	/* Change thread state into running */
	f_nmaw_thrState = D_NMA_THRDSTA_RUN;
	
	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	while(1)
	{
		memset(cRcvMsg, 0, sizeof(cRcvMsg));

		/************************************************************/
		/* Receive the message from queue							*/
		/************************************************************/
		f_com_msgQReceive( D_SYS_THDQID_PF_NMA, 
						   sizeof(cRcvMsg), 
						   cRcvMsg, &rcvsize,&errcd);

		cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_ENTER,"[f_nma_main] RECIEVE");
		/* analyze and dispatch message   */
		f_nma_anlz( cRcvMsg );
		cmn_com_assert_th_get(f_nmaw_assert_p,  D_RRH_LOG_AST_LV_RETURN,"[f_nma_main] RETURN");
	}
	
	return;
}
/* @} */
