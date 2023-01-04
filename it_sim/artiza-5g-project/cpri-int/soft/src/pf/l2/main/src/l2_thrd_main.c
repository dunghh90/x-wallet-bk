/*!
 *  @skip   $Id$
 *  @file   l2_thrd_main.c
 *  @brief  the entry of l2 threads
 *  @date   2013/11/07 FFCS)hongj modify for zynq\n
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
*/ 

 /*!
 * @addtogroup RRH_L2_MAIN
 * @{
 */

#include "l2_com_inc.h"
#include "rrhApi_L2.h"
#include "rrhApi_Dbg.h"
#include "f_cmn_cmu_inc.h"
#include "f_dbg_thread_inc.h"
#include "f_dbg_com_ftype.h"
#include <sys/prctl.h>

extern UINT  lbw_lnk_set_respQId[D_RRH_CPRINO_NUM];
extern UINT  lbw_lnk_rel_respQId[D_RRH_CPRINO_NUM];
/********************************************************************************************************************/
/*!
 *  @brief  main fucntion of lpb thread
 *  @note   This function is processed as follows.\n
 *          - Call tm_lb_Main to perform init request.
 *          - Loop to receive the message from queue and call tm_lb_Main to process each message
 *          - When message buffer is not null, free it.
 *  @return VOID
 *  @retval none
 * @date  2013/11/08 FFCS)hongj create for zynq\n
 */
/********************************************************************************************************************/
VOID l2_lpb_main( void *param )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT   ret;
	VOID *rcvMsg;
	UINT  msgSize;

#ifdef FOR_DBG
	printf("#L2 lpb thread......\n");
#endif
	prctl(PR_SET_NAME, (CHAR*)param, 0, 0, 0);

	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_L2,			/* ProcessID		*/
									D_RRH_THDID_L2_LAPB_MNG,	/* ThreadID			*/
									CMD_TSKID_LPB,              /* ThreadQueueID	*/
									(unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/
	
	
	/************************************************************/
	/* Initialize the tables									*/
	/************************************************************/
	tm_lb_Main(CMD_TSKID_INI, CMD_TSKIF_INIREQ, D_RRH_NULL);					 

 	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	for(;;)
	{
		/* Receive the message from queue */
		ret = cm_Recieve(
				CMD_TSKID_LPB,			/*receive message queue*/
				0,					
				&rcvMsg,				/*receive data area*/
				&msgSize);				/*receive data area size*/

		if(ret != CMD_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, ret,
			    (CHAR*)"[l2_lpb_main] Receive thread message NG" );
			return;
		}

		/* API NO. to lpb internal event */
		switch (((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo)
		{
		case D_API_MSGID_L2_LINK_SETUP_REQ:
		    ((CMT_TSKIF_HEAD*)rcvMsg)->uiEventNo = CMD_TSKIF_ESTREQ;
		    lbw_lnk_set_respQId[((T_API_L2LINK_SETUP_REQ*)rcvMsg)->link_num] =
		    	((CMT_TSKIF_HEAD*)rcvMsg )->uiSrcPQueueID;
		    break;

		case D_API_MSGID_L2_LINK_REL_REQ:
		    ((CMT_TSKIF_HEAD*)rcvMsg)->uiEventNo = CMD_TSKIF_RELREQ;
		    lbw_lnk_rel_respQId[((T_API_L2LINK_REL_REQ*)rcvMsg)->link_num] =
		    	((CMT_TSKIF_HEAD*)rcvMsg )->uiSrcPQueueID;
		    break;
		case D_API_MSGID_L2_LINK_DEACT_IND:
		    ((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo = CMD_TSKIF_L2DEACTREQ;
		    break;
		case D_L2_EVTNO_L3_TEST_START_NOTI:
		    ((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo = CMD_TSKIF_L3MODESTRNTC;
		    break;
		case D_L2_EVTNO_L3_TEST_STOP_NOTI:
			((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo = CMD_TSKIF_L3MODESTPNTC;
			break;
		case D_API_MSGID_L2_CPRIMSG_SEND_IND:
			((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo = CMD_TSKIF_CPRISNDNTC;
			((CMT_TSKIF_HEAD*)rcvMsg )->uiDstPQueueID = 0;
			break;
		case CMD_TSKIF_TIMOUTNTC:
			((CMT_TSKIF_HEAD*)rcvMsg )->uiSrcPQueueID = ((CMT_TSKIF_HEAD*)rcvMsg )->uiSignalkind;
			break;		
		default:
			break;
		}

		/* analyze and dispatch message   */
		tm_lb_Main(((CMT_TSKIF_HEAD*)rcvMsg )->uiSrcPQueueID,
					((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo, (UINT *)rcvMsg);
	}
#ifdef FOR_DBG
	printf("#L2 lpb thread exit!\n");
#endif	

	return;
}

/********************************************************************************************************************/
/*!
 *  @brief  main fucntion of txs thread
 *  @note   This function is processed as follows.\n
 *          - Call tm_tx_Main to perform init request.
 *          - Loop to receive the message from queue and call tm_tx_Main to process each message
 *          - When message buffer is not null, free it.
 *  @return VOID
 *  @retval none
 * @date  2013/11/08 FFCS)hongj create for zynq\n
 */
/********************************************************************************************************************/
VOID l2_txs_main( void *param )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT   ret;
	UINT   rcvmsgsz;                /* receivied message size	*/
	VOID   *rcvMsg;

#ifdef FOR_DBG
	printf("#L2 txs thread......\n");
#endif
	prctl(PR_SET_NAME, (CHAR*)param, 0, 0, 0);

	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_L2,			/* ProcessID		*/
									D_RRH_THDID_L2_LAPB_SEND,	/* ThreadID			*/
									CMD_TSKID_TXS,				/* ThreadQueueID	*/
									(unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/
	
	/************************************************************/
	/* Initialize the tables									*/
	/************************************************************/
	tm_tx_Main(CMD_TSKID_INI, CMD_TSKIF_INIREQ, D_RRH_NULL);					 

	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	for(;;)
	{
		/* Receive the message from queue */
		/* Receive the message from queue */
		ret = cm_Recieve(
				CMD_TSKID_TXS,	/*receive message queue*/
				0,					
				&rcvMsg,				/*receive data area*/
				&rcvmsgsz);				/*receive data area size*/

		if(ret != CMD_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, ret,
			    (CHAR*)"[l2_txs_main] Receive thread message NG" );

			return;
		}

		/* analyze and dispatch message   */
		tm_tx_Main(((CMT_TSKIF_HEAD*)rcvMsg )->uiSrcPQueueID,
					((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo, (UINT *)rcvMsg);
	}
#ifdef FOR_DBG
	printf("#L2 txs thread exit!\n");
#endif	

	return;
}

/********************************************************************************************************************/
/*!
 *  @brief  main fucntion of txs thread
 *  @note   This function is processed as follows.\n
 *          - Call tm_rx_Main to perform init request.
 *          - Loop to receive the message from queue and call tm_rx_Main to process each message
 *          - When message buffer is not null, free it.
 *  @return VOID
 *  @retval none
 * @date  2013/11/08 FFCS)hongj create for zynq\n
 */
 /********************************************************************************************************************/
VOID l2_rxs_main( void *param )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT   ret;
	UINT   rcvmsgsz;                /* receivied message size	*/
	VOID   *rcvMsg;

#ifdef FOR_DBG
	printf("#L2 rxs thread......\n");
#endif
	prctl(PR_SET_NAME, (CHAR*)param, 0, 0, 0);

	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_L2,			/* ProcessID		*/
									D_RRH_THDID_L2_LAPB_RECV,	/* ThreadID			*/
									CMD_TSKID_RXS,				/* ThreadQueueID	*/
									(unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/
	
	/************************************************************/
	/* Initialize the tables									*/
	/************************************************************/
	tm_rx_Main(CMD_TSKID_INI, CMD_TSKIF_INIREQ, D_RRH_NULL);					 

	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	for(;;)
	{
		/* Receive the message from queue */
		/* Receive the message from queue */
		ret = cm_Recieve(
				CMD_TSKID_RXS,	/*receive message queue*/
				0,					
				&rcvMsg,				/*receive data area*/
				&rcvmsgsz);				/*receive data area size*/

		if(ret != CMD_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, ret,
			    (CHAR*)"[l2_rxs_main] Receive thread message NG" );
			return;
		}
		
		/* API NO. to rxs internal event */
		switch (((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo)
		{
		case D_API_MSGID_DBG_SEND_SIMUCPRIMSG:
		    ((CMT_TSKIF_HEAD*)rcvMsg)->uiEventNo = CMD_TSKIF_L1DATRCVNTC;
		    ((CMT_TSKIF_HEAD*)rcvMsg )->uiDstPQueueID = 0;
		    break;

		default:
		    break;
		}

		/* analyze and dispatch message   */
		tm_rx_Main(((CMT_TSKIF_HEAD*)rcvMsg )->uiSrcTQueueID,
					((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo, (UINT *)rcvMsg);
	}
#ifdef FOR_DBG
	printf("#L2 lpb thread exit!\n");
#endif	

	return;
}

/********************************************************************************************************************/
/*!
 *  @brief  main fucntion of pol thread
 *  @note   This function is processed as follows.\n
 *          - Call tm_pl_Main to perform init request.
 *          - Loop to receive the message from queue and call tm_pl_Main to process each message
 *          - When message buffer is not null, free it.
 *  @return VOID
 *  @retval none
 * @date  2013/11/08 FFCS)hongj create for zynq\n
 */
/********************************************************************************************************************/
VOID l2_pol_main( void *param )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT   ret;
	UINT   rcvmsgsz;                /* receivied message size	*/
	VOID   *rcvMsg;

#ifdef FOR_DBG
	printf("#L2 pol thread......\n");
#endif
	prctl(PR_SET_NAME, (CHAR*)param, 0, 0, 0);

	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_L2,			/* ProcessID		*/
									D_RRH_THDID_L2_POLL,		/* ThreadID			*/
									CMD_TSKID_POL,				/* ThreadQueueID	*/
									(unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/
	
	/************************************************************/
	/* Initialize the tables									*/
	/************************************************************/
	tm_pl_Main(CMD_TSKID_INI, CMD_TSKIF_INIREQ, D_RRH_NULL);					 

	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	for(;;)
	{
		/* Receive the message from queue */
		ret = cm_Recieve(
				CMD_TSKID_POL,	/*receive message queue*/
				0,					
				&rcvMsg,				/*receive data area*/
				&rcvmsgsz);				/*receive data area size*/

		if(ret != CMD_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, ret,
			    (CHAR*)"[l2_pol_main] Receive thread message NG" );
			return;
		}

		/* analyze and dispatch message   */
		tm_pl_Main(((CMT_TSKIF_HEAD*)rcvMsg )->uiSrcPQueueID,
					((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo, (UINT *)rcvMsg);
	}
#ifdef FOR_DBG
	printf("#L2 pol thread exit!\n");
#endif	

	return;
}

/********************************************************************************************************************/
/*!
 *  @brief  main fucntion of dbg thread
 *  @note   This function is processed as follows.\n
 *          - Call tm_dbg_Main to perform init request.
 *          - Loop to receive the message from queue and call tm_dbg_Main to process each message
 *          - When message buffer is not null, free it.
 *  @return VOID
 *  @retval none
 * @date  2013/12/03 FFCS)hongj create for zynq\n
 */
/********************************************************************************************************************/
VOID l2_dbg_main( void *param )
{
//#ifndef OPT_RRH_SIM
	T_DBGTHREAD_EXEC_INFO tdbg;

	memset(&tdbg,0,sizeof(tdbg));

	tdbg.uiProcID = D_RRH_PROCID_L2;
	tdbg.uiProcQID = D_RRH_PROCQUE_L2;
	tdbg.uiMainThradQID= CMD_TSKID_INI;
	tdbg.uiDbgThreadID = D_RRH_THDID_L2_DBG;
	tdbg.uiDbgThreadQID = CMD_TSKID_DBG;
	tdbg.cpDbgThreadName = (char*)param;
	tdbg.usrFunc = tm_dbg_Main;
	f_dbg_main(&tdbg);
//#endif

	return;
}
/*!
 *  @brief  main fucntion of CMU receiving thread
 *  @note   This function is processed as follows.\n
 *          - directly call f_send_main
 *  @return VOID
 *  @retval none
 * @date  2013/12/08 FFCS)linb create\n
 */
VOID l2_snd_main( void *param )
{
//#ifndef OPT_RRH_SIM
	T_CMU_CREATE_THREAD tcrt;

	memset(&tcrt,0,sizeof(tcrt));
	tcrt.uiProcID = D_RRH_PROCID_L2;
	tcrt.uiProcQID = D_RRH_PROCQUE_L2;
	tcrt.uiThreadID = D_RRH_THDID_L2_SEND;
	tcrt.uiThreadQID = CMD_TSKID_SND;
	tcrt.uidstThreadQID = CMD_TSKID_INI;
	tcrt.uiThreadInfo = E_RRH_CMU_SEND_ID;
	strcpy(tcrt.uiThreadName,(CHAR*)param);
	f_send_main(&tcrt);
//#endif
}

/*!
 *  @brief  main fucntion of CMU receiving thread
 *  @note   This function is processed as follows.\n
 *          - directly call f_recv_main
 *  @return VOID
 *  @retval none
 * @date  2013/12/08 FFCS)linb create\n
 */
extern T_CMU_CREATE_THREAD l2_mainr_cmusRecvThdCreMng;

VOID l2_rcv_main( void *param )
{
//#ifndef OPT_RRH_SIM
	f_recv_main(&l2_mainr_cmusRecvThdCreMng);
//#endif
}
/* @} */

