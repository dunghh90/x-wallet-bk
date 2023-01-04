/*!
 *  @skip   $Id$
 *  @file   l3_thrd_main.c
 *  @brief  the entry of L3 server
 *  @date   2013/11/11 FFCS)linb create for zynq\n
 *  @date   2015/09/07 TDIPS)sasaki update l3_dlm_main
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
*/ 

 /*!
 * @addtogroup RRH_L3_MAIN
 * @{
 */
#include "m_cm_header.h"
#include "l3_com_inc.h"
#include "f_cmn_cmu_inc.h"
#include "f_dbg_thread_inc.h"
#include "rrhApi_Cpri.h"
#include "rrhApi_Nego.h"
#include "rrhApi_L2.h"
#include "rrhApi_Svp.h"
#include "m_dl_header.h"

#include <sys/prctl.h>

extern VOID m_cr_main(UINT , UINT , UINT * );
extern VOID m_rc_ReCntMain( UINT , UINT , VOID * );
extern VOID m_dl_main( UINT , UINT , VOID*  );
extern VOID m_lg_main();
/*!
 *  @brief  main fucntion of cpr thread
 *  @note   This function is processed as follows.\n
 *          - Call m_cpr_main to perform init request.
 *          - Loop to receive the message from queue and call m_cpr_main to process each message
 *          - When message buffer is not null, free it.
 *  @return VOID
 *  @retval none
 * @date  2013/11/08 FFCS)linb create\n
 */

VOID l3_cpr_main( void *param )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT ret;
	VOID *rcvMsg;
	UINT  msgSize;

	prctl(PR_SET_NAME, (CHAR*)param, 0, 0, 0);
#ifdef FOR_DBG
	printf("#L3 cpr thread......\n");
#endif
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_L3,				/* ProcessID		*/
									D_RRH_THDID_L3_CPRI,			/* ThreadID			*/
									CMD_TSKID_CPR,				    /* ThreadQueueID	*/
									(unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/

	/************************************************************/
	/* Initialize the tables									*/
	/************************************************************/
	m_cr_main(CMD_TSKID_INI,CMD_TSKIF_INIREQ,(UINT*)D_RRH_NULL);					 

	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	for(;;)
	{
		/* Receive the message from queue */
		ret = cm_Recieve(
				CMD_TSKID_CPR,			/*receive message queue*/
				0,					
				&rcvMsg,				/*receive data area*/
				&msgSize);				/*receive data area size*/
	
		if(ret != CMD_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, ret,
			    "[l3_cpr_main] Receive thread message NG" );

			break;
		}
		switch(((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo)
		{
			case D_API_MSGID_CPRI_STATE_STATE_NTC:
			((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo = CMD_TSKIF_CPRISTACHGNTC;
			break;

			case D_API_MSGID_L2_LINKSTATE_NTC:
			((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo = CMD_TSKIF_L2STANTC;
			break;

			case D_API_MSGID_L2_CPRIMSG_RCV_NTC:
			((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo = CMD_TSKIF_CPRIRCVNTC;
			break;

			case D_API_MSGID_SVP_STATE_CHG_NTC:
			((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo = CMD_TSKIF_CARDSTANTC;
			break;
	
			case CMD_TSKIF_TIMOUTNTC:
			case CMD_TSKIF_TIMOUTRSPNTC:
			case CMD_TSKIF_TIMOUTSNDNTC:
			((CMT_TSKIF_HEAD*)rcvMsg )->uiSrcPQueueID = ((CMT_TSKIF_HEAD*)rcvMsg )->uiSignalkind;
			break;

			default:
			break;
		}
		/* analyze and dispatch message   */
		m_cr_main(((CMT_TSKIF_HEAD*)rcvMsg )->uiSrcPQueueID,
						((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo,
						rcvMsg);

		cm_BFree( rcvMsg );
	}
#ifdef FOR_DBG
	printf("#L3 cpr thread exit!\n");
#endif	

	return;
}

/*!
 *  @brief  main fucntion of rct thread
 *  @note   This function is processed as follows.\n
 *          - Call m_rct_Main to perform init request.
 *          - Loop to receive the message from queue and call m_rct_Main to process each message
 *          - When message buffer is not null, free it.
 *  @return VOID
 *  @retval none
 * @date  2013/11/08 FFCS)linb create\n
 */

VOID l3_rct_main( void *param )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT ret;
	VOID *rcvMsg;
	UINT  msgSize;
	
	prctl(PR_SET_NAME, (CHAR*)param, 0, 0, 0);
#ifdef FOR_DBG
	printf("#L3 rct thread......\n");
#endif
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_L3,				/* ProcessID		*/
									D_RRH_THDID_L3_RCT,			/* ThreadID			*/
									CMD_TSKID_RCT,				/* ThreadQueueID	*/
									(unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/

	/************************************************************/
	/* Initialize the tables									*/
	/************************************************************/
	m_rc_ReCntMain(CMD_TSKID_INI,CMD_TSKIF_INIREQ,(UINT*)D_RRH_NULL);					 

	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	for(;;)
	{
		/*memset((VOID*)(&cRcvMsg),D_SYS_NUM0,sizeof(cRcvMsg));*/
		/* Receive the message from queue */
		ret = cm_Recieve(
				CMD_TSKID_RCT,	/*receive message queue*/
				0,					
				&rcvMsg,				/*receive data area*/
				&msgSize);				/*receive data area size*/
	
		if(ret != CMD_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, ret,
			    "[l3_rct_main] Receive thread message NG" );

			break;
		}
		/* judege whether the message is timeout */
		if(((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo == CMD_TSKIF_TIMOUTNTC)
			((CMT_TSKIF_HEAD*)rcvMsg )->uiSrcPQueueID = ((CMT_TSKIF_HEAD*)rcvMsg )->uiSignalkind;
		if(((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo == D_API_MSGID_L2_CPRIMSG_RCV_NTC)
			((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo = CMD_TSKIF_CPRIRCVNTC;
		if(((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo == D_API_MSGID_SVP_STATE_CHG_NTC)
			((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo = CMD_TSKIF_CARDSTANTC;

		/* write running history		  */						   
		/* analyze and dispatch message   */
		m_rc_ReCntMain(((CMT_TSKIF_HEAD*)rcvMsg )->uiSrcPQueueID,
						((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo,
						rcvMsg);
		cm_BFree( rcvMsg );
	}
#ifdef FOR_DBG
	printf("#L3 rct thread exit!\n");
#endif	

	return;
}

/*!
 *  @brief  main fucntion of dlm thread
 *  @note   This function is processed as follows.\n
 *          - Call l3_dlm_Main to perform init request.
 *          - Loop to receive the message from queue and call l3_dlm_Main to process each message
 *          - When message buffer is not null, free it.
 *  @return VOID
 *  @retval none
 * @date  2013/11/08 FFCS)linb create\n
 * @date  2015/09/07 TDIPS)sasaki 初期化処理用のm_dl_init関数を直接呼ぶように修正
 */

VOID l3_dlm_main( void *param )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT ret;
	VOID *rcvMsg;
	UINT  msgSize;

	prctl(PR_SET_NAME, (CHAR*)param, 0, 0, 0);
#ifdef FOR_DBG
	printf("#L3 dlm thread......\n");
#endif
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_L3,				/* ProcessID		*/
									D_RRH_THDID_L3_DLM,				/* ThreadID			*/
									CMD_TSKID_DLM,					/* ThreadQueueID	*/
									(unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/

	/************************************************************/
	/* Initialize the tables									*/
	/************************************************************/
	printf("\n C9 debug file %s ,line %d \n",__FILE__,__LINE__); //C9 add code debug
	m_dl_init((UINT*)D_RRH_NULL, 0);
printf("\n C9 debug file %s ,line %d,m_dl_init = ok \n",__FILE__,__LINE__);//C9 add code debug
	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	for(;;)
	{
		#ifdef FHM_DEBUG_FOR_IT1
		printf( "[%d]%s\n",__LINE__,__FUNCTION__);
		#endif
		/*memset((VOID*)(&cRcvMsg),D_SYS_NUM0,sizeof(cRcvMsg));*/
		/* Receive the message from queue */
		ret = cm_Recieve(
				CMD_TSKID_DLM,			/*receive message queue*/
				0,					
				&rcvMsg,				/*receive data area*/
				&msgSize);				/*receive data area size*/
	
		#ifdef FHM_DEBUG_FOR_IT1
		printf( "[%d]%s\n",__LINE__,__FUNCTION__);
		#endif
		if(ret != CMD_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, ret,
			    "[l3_dlm_main] Receive thread message NG" );
			#ifdef FHM_DEBUG_FOR_IT1
			printf( "[%d]%s [l3_dlm_main] Receive thread message NG\n ",__LINE__,__FUNCTION__);
			#endif
			break;
		}

		/* judege whether the message is timeout */
		if(((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo == CMD_TSKIF_TIMOUTNTC)
			((CMT_TSKIF_HEAD*)rcvMsg )->uiSrcPQueueID = ((CMT_TSKIF_HEAD*)rcvMsg )->uiSignalkind;
		if(((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo == D_API_MSGID_L2_CPRIMSG_RCV_NTC)
			((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo = CMD_TSKIF_CPRIRCVNTC;

		/* write running history		  */						   
		/* analyze and dispatch message   */
		#ifdef FHM_DEBUG_FOR_IT1
		printf( "[%d]%s\n",__LINE__,__FUNCTION__);
		#endif
		m_dl_main(((CMT_TSKIF_HEAD*)rcvMsg )->uiSrcPQueueID,
						((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo,
						rcvMsg);
		
		cm_BFree( rcvMsg );
	}
#ifdef FOR_DBG
	printf("#L3 dlm thread exit!\n");
#endif	

	return;
}

/*!
 *  @brief  main fucntion of log thread
 *  @note   This function is processed as follows.\n
 *          - Call l3_log_Main to perform init request.
 *          - Loop to receive the message from queue and call l3_log_Main to process each message
 *          - When message buffer is not null, free it.
 *  @return VOID
 *  @retval none
 * @date  2013/11/08 FFCS)linb create\n
 */
/*!
 *  @brief  main fucntion of dia thread
 *  @note   This function is processed as follows.\n
 *          - Call l3_log_Main to perform init request.
 *          - Loop to receive the message from queue and call l3_log_Main to process each message
 *          - When message buffer is not null, free it.
 *  @return VOID
 *  @retval none
 * @date  2013/11/08 FFCS)linb create\n
 */
extern const T_CMU_CREATE_THREAD l3_mainr_cmusRecvThdCreMng;

VOID l3_rcv_main( void *param )
{
//#ifndef OPT_RRH_SIM
	f_recv_main((T_CMU_CREATE_THREAD *)&l3_mainr_cmusRecvThdCreMng);
//#endif
}

VOID l3_snd_main( void *param )
{
//#ifndef OPT_RRH_SIM
	T_CMU_CREATE_THREAD tcrt;

	memset(&tcrt,0,sizeof(tcrt));
	tcrt.uiProcID = D_RRH_PROCID_L3;
	tcrt.uiProcQID = D_RRH_PROCQUE_L3;
	tcrt.uiThreadID = D_RRH_THDID_L3_SEND;
	tcrt.uiThreadQID = CMD_TSKID_SND;
	tcrt.uidstThreadQID = CMD_TSKID_INI;
	tcrt.uiThreadInfo = E_RRH_CMU_SEND_ID;
	strcpy(tcrt.uiThreadName,(CHAR*)param);
	f_send_main(&tcrt);
//#endif
}

/*!
 *  @brief  main fucntion of debug thread
 *  @note   This function is processed as follows.\n
 *          - directly call f_send_main
 *  @return VOID
 *  @retval none
 * @date  2013/12/08 FFCS)linb create\n
 */
extern void f_dbg_main( T_DBGTHREAD_EXEC_INFO  * )	;
VOID l3_dbg_main( void *param )
{
//#ifndef OPT_RRH_SIM
	T_DBGTHREAD_EXEC_INFO tdbg;

	memset(&tdbg,0,sizeof(tdbg));

	tdbg.uiProcID = D_RRH_PROCID_L3;
	tdbg.uiProcQID = D_RRH_PROCID_L3;
	tdbg.uiDbgThreadID = D_RRH_THDID_L3_DBG;
	tdbg.uiDbgThreadQID = CMD_TSKID_DBG;
	tdbg.cpDbgThreadName = (char*)param;
	printf("\n C9 debug file %s ,line %d\n",__FILE__,__LINE__); //C9 add code debug
	f_dbg_main(&tdbg);
//#endif
	return;
}

void l3_log_main(void *p)
{
	m_lg_main();
}
/* @} */

