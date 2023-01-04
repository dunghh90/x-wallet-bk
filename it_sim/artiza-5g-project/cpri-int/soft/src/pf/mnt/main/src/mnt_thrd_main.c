/*!
 *  @skip   $Id$
 *  @file   mnt_thrd_main.c
 *  @brief  the entry of MNT thread server
 *  @date   2013/11/07 FFCS)zhaodx create for zynq\n
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
*/ 

 /*!
 * @addtogroup RRH_MNT_MAIN
 * @{
 */

#include "m_cm_header.h"
#include "mnt_com_inc.h"
#include "rrhApi_Cpri.h"
#include "rrhApi_L2.h"
#include "rrhApi_Svp.h"
#include "rrhApi_Dbg.h"
#include "f_cmn_cmu_inc.h"

extern VOID m_mt_Main(UINT source, UINT event, UINT *inf_p);
extern VOID m_mk_Main(UINT source, UINT event, UINT *inf_p);
extern VOID m_mr_Main(UINT src_id,UINT event_no,UINT*a_inf_p);
extern VOID m_ms_Main(UINT src_id,UINT event_no,UINT* a_inf_p);
extern VOID tm_lr_Main(UINT src_id,UINT event_no,UINT*a_inf_p);
extern VOID tm_ls_Main(UINT src_id,UINT event_no,UINT* a_inf_p);

/********************************************************************************************************************/
/*!
 *  @brief  main fucntion of mnt_mkm thread
 *  @note   This function is processed as follows.\n
 *          - Call m_mk_Main to perform init request.
 *          - Loop to receive the message from queue and call m_mk_Main to process each message
 *          - When message buffer is not null, free it.
 *  @return VOID
 *  @retval none
 * @date  2013/11/08 FFCS)zhaodx create for zynq\n
 */
/********************************************************************************************************************/
VOID mnt_mkm_main( void *param )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT   ret;
	VOID *rcvMsg;
	UINT  msgSize;

	prctl(PR_SET_NAME, (CHAR*)param, 0, 0, 0);
#ifdef FOR_DBG
	printf("#MNT mkm thread......\n");
#endif
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_MT,				/* ProcessID		*/
									D_RRH_THDID_MT_CTRL,			/* ThreadID			*/
								    CMD_TSKID_MKM,			       /* ThreadQueueID	*/
								    (unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/
								
	/************************************************************/
	/* Initialize the tables									*/
	/************************************************************/
	m_mk_Main(CMD_TSKID_INI,CMD_TSKIF_INIREQ,D_RRH_NULL);					 

	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	for(;;)
	{
		/* Receive the message from queue */
		ret = cm_Recieve(
				CMD_TSKID_MKM,			/*receive message queue*/
				0,					
				&rcvMsg,				/*receive data area*/
				&msgSize);				/*receive data area size*/
		if(ret != CMD_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, ret,
			    (CHAR*)"[mnt_mkm_main] Receive thread message NG" );

			break;
		}

		/* API NO. to lpb internal event */
		switch (((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo)
		{
		case D_API_MSGID_CPRI_STATE_STATE_NTC:
		    ((CMT_TSKIF_HEAD*)rcvMsg)->uiEventNo = CMD_TSKIF_CPRISTANTC;
		    break;

		case D_API_MSGID_L2_LINKSTATE_NTC:
		    ((CMT_TSKIF_HEAD*)rcvMsg)->uiEventNo = CMD_TSKIF_L2STANTC;
		    break;

		case D_API_MSGID_SVP_STATE_CHG_NTC:
		    ((CMT_TSKIF_HEAD*)rcvMsg)->uiEventNo = CMD_TSKIF_CARDSTANTC;
		    break;

		case D_API_MSGID_DBG_CPRIMON_DATA_RCV_NTC:
		    ((CMT_TSKIF_HEAD*)rcvMsg)->uiEventNo = CMD_TSKIF_L1MONDATRCVNTC;
		    break;

		case D_API_MSGID_DBG_CPRIMON_DATA_SND_NTC:
		    ((CMT_TSKIF_HEAD*)rcvMsg)->uiEventNo = CMD_TSKIF_L1MONDATSNDNTC;
		    break;

		case D_API_MSGID_DBG_RECV_SIMUCPRIMSG:
		    ((CMT_TSKIF_HEAD*)rcvMsg)->uiEventNo = CMD_TSKIF_L1DATTXREQ;
		    break;
			
		case CMD_TSKIF_TIMOUTNTC:
			((CMT_TSKIF_HEAD*)rcvMsg )->uiSrcPQueueID = ((CMT_TSKIF_HEAD*)rcvMsg )->uiSignalkind;
			break;
		
		default:
		    break;
		}
		/* analyze and dispatch message   */
		m_mk_Main(((CMT_TSKIF_HEAD*)rcvMsg )->uiSrcPQueueID,
					((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo,
					rcvMsg);

		/*buffer is release in thread*/
	}
#ifdef FOR_DBG
	printf("#MNT mkm thread exit!\n");
#endif	

	return;
}

/********************************************************************************************************************/
/*!
 *  @brief  main fucntion of mnt_lnr thread
 *  @note   This function is processed as follows.\n
 *          - Call tm_lr_Main to perform init request.
 *          - Loop to receive the message from queue and call tm_lr_Main to process each message
 *          - When message buffer is not null, free it.
 *  @return VOID
 *  @retval none
 * @date  2013/11/08 FFCS)zhaodx create for zynq\n
 */
/********************************************************************************************************************/
VOID mnt_lnr_main( void *param )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT   ret;
	VOID *rcvMsg;
	UINT  msgSize;

	prctl(PR_SET_NAME, (CHAR*)param, 0, 0, 0);
#ifdef FOR_DBG
	printf("#MNT lnr thread......\n");
#endif

 	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_MT,				/* ProcessID		*/
									D_RRH_THDID_MT_LAN_RECV,		/* ThreadID			*/
									CMD_TSKID_LNR, (unsigned long int)pthread_self());					/* ThreadQueueID	*/
  
	/************************************************************/
	/* Initialize the tables									*/
	/************************************************************/
	tm_lr_Main(CMD_TSKID_INI,CMD_TSKIF_INIREQ,D_RRH_NULL);					 

	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	for(;;)
	{
		/* Receive the message from queue */
		ret = cm_Recieve(
				CMD_TSKID_LNR,			/*receive message queue*/
				0,					
				&rcvMsg,				/*receive data area*/
				&msgSize);				/*receive data area size*/
		if(ret != CMD_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, ret,
			    (CHAR*)"[mnt_lnr_main] Receive thread message NG" );

			break;
		}

		/* analyze and dispatch message   */
		tm_lr_Main(((CMT_TSKIF_HEAD*)rcvMsg )->uiSrcPQueueID,
					((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo,
					rcvMsg);

		/*buffer is release in thread*/
	}
#ifdef FOR_DBG
	printf("#MNT lnr thread exit!\n");
#endif	

	return;
}

/********************************************************************************************************************/
/*!
 *  @brief  main fucntion of mnt_lns thread
 *  @note   This function is processed as follows.\n
 *          - Call tm_ls_Main to perform init request.
 *          - Loop to receive the message from queue and call tm_ls_Main to process each message
 *          - When message buffer is not null, free it.
 *  @return VOID
 *  @retval none
 * @date  2013/11/08 FFCS)zhaodx create zynq\n
 */
/********************************************************************************************************************/
VOID mnt_lns_main( void *param )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT   ret;
	VOID *rcvMsg;
	UINT  msgSize;

	prctl(PR_SET_NAME, (CHAR*)param, 0, 0, 0);
#ifdef FOR_DBG
	printf("#MNT lns thread......\n");
#endif
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_MT,				/* ProcessID		*/
									D_RRH_THDID_MT_LAN_SEND,		/* ThreadID			*/
									CMD_TSKID_LNS, (unsigned long int)pthread_self());				/* ThreadQueueID	*/
	/************************************************************/
	/* Initialize the tables									*/
	/************************************************************/
	tm_ls_Main(CMD_TSKID_INI,CMD_TSKIF_INIREQ,D_RRH_NULL);					 

	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	for(;;)
	{
		/* Receive the message from queue */
		ret = cm_Recieve(
				CMD_TSKID_LNS,			/*receive message queue*/
				0,					
				&rcvMsg,				/*receive data area*/
				&msgSize);				/*receive data area size*/
		if(ret != CMD_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, ret,
			    (CHAR*)"[mnt_lnr_main] Receive thread message NG" );

			break;
		}

		/* analyze and dispatch message   */
		tm_ls_Main(((CMT_TSKIF_HEAD*)rcvMsg )->uiSrcPQueueID,
					((CMT_TSKIF_HEAD*)rcvMsg )->uiEventNo,
					rcvMsg);

		/*buffer is release in thread*/
	}
#ifdef FOR_DBG
	printf("#MNT lns thread exit!\n");
#endif	

	return;
}

extern T_CMU_CREATE_THREAD mnt_mainr_cmusRecvThdCreMng;
VOID mnt_rcv_main( void *param )
{
//#ifndef OPT_RRH_SIM
	f_recv_main(&mnt_mainr_cmusRecvThdCreMng);
//#endif
}

VOID mnt_snd_main( void *param )
{
//#ifndef OPT_RRH_SIM
	T_CMU_CREATE_THREAD tcrt;

	memset(&tcrt,0,sizeof(tcrt));
	tcrt.uiProcID = D_RRH_PROCID_MT;
	tcrt.uiProcQID = D_RRH_PROCQUE_MT;
	tcrt.uiThreadID = D_RRH_THDID_MT_SEND;
	tcrt.uiThreadQID = CMD_TSKID_SND;
	tcrt.uidstThreadQID = CMD_TSKID_INI;
	tcrt.uiThreadInfo = E_RRH_CMU_SEND_ID;
	strcpy(tcrt.uiThreadName,(CHAR*)param);
	f_send_main(&tcrt);
//#endif
}

VOID mnt_dbg_main( void *param )
{
}
/* @} */

