/*!
 * @skip  $ld:$
 * @file  rrhApi_Dbg.c
 * @brief API : related to Debug operation
 * @date  2013/11/08 FFCS) Create.
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/** @addtogroup RRH_API_DBG
 *  @{
 */
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "f_rrh_inc.h"
#include "l2_com_def.h"
#include "BPF_RU_IPCM.h"

#include "rrhApi_Dbg.h"

/********************************************************************************************************************/
/**
 *  @brief  API : The request of startup cpri simu mode
 *  @note   PF EVENT ID : 0xA00A0001
 *          Reponse  ID : 0xA00A0002
 *          TYPE        : MNR
 *  @param  qid     [I] : response queue id (calling process QueueID)
 *  @param  wtime   [I] : wait time(msec) for response
 *  @param  data_p  [O] : destination of response message
 *  @param  linkno  [I] : link NO.
 *  @param  mode    [I] : simulator mode(L2 or L3)
 *  
 *  @return INT
 *  @retval ret
 *  @date   2013/11/08 FFCS) Create.
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Dbg_Mqr_StartCpriSimu( 
								INT	qid,	INT	wtime,VOID *data_p,
								UINT      linkno,
								UINT	mode)
{
	T_API_DBG_CPRISIMU_STARTUP_REQ	apiReq;
	T_API_DBG_CPRISIMU_STARTUP_RSP	apiRsp;
	INT									errcd;
	INT									ret;
	UINT								size;
	UINT								msgLen;
	
	if(rrhApi_com_checkRespQ(qid) != E_API_RCD_OK)
		return E_API_RCD_EPARAM;

	memset(&apiReq,0,sizeof(apiReq));
	memset(&apiRsp,0,sizeof(apiRsp));

	size = sizeof(T_API_DBG_CPRISIMU_STARTUP_REQ);

	/*fill IF*/
	apiReq.header.uiEventNo		= D_API_MSGID_DBG_CPRISIMU_START_REQ;/* Event  ID         */
	apiReq.header.uiSignalkind	= 0;								/* Signal Kind       */
	apiReq.header.uiDstPQueueID	= D_RRH_PROCQUE_L2;					/* Destination P QID */
	apiReq.header.uiDstTQueueID	= 0;							/* Destination T QID */
	apiReq.header.uiSrcPQueueID	= qid;								/* Source P QID      */
	apiReq.header.uiSrcTQueueID	= 0;								/* Source T QID      */
	apiReq.header.uiLength		= sizeof(apiReq);					/* Length            */
	apiReq.data.linkno          = linkno;                           /* Link NO.          */
	apiReq.data.mode            = mode;                             /* Simulator mode    */
	
	/* Queue Clear	*/
	(VOID)BPF_RU_IPCM_PROCMSGQ_FREE_STACK_BUFF( qid );
	
	/* call MW func */
	ret = rrhApi_com_msgQSend(
				D_RRH_PROCQUE_L2,					/* Message QID             */
				(void *)&apiReq,					/* The message to transmit */
				sizeof(apiReq),						/* The size of message     */
				&errcd								/* Error code              */
				);
	/* checking parameter */
	if( ret != BPF_RU_IPCM_OK ) {
		rrhApi_com_log(__func__, errcd);
		
		return BPF_RU_IPCM_NG;
	}

	size = sizeof(apiRsp);
	ret = rrhApi_com_msgQReceive(
					qid,				/* [in]Que no					*/
					wtime,				/* [in]Wait Time					*/
					size,				/* [in]Pointer of recieve buffer	*/
					&apiRsp,			/* [in]Recieve Buffer Size			*/
					&msgLen,			/* [out]Reciept Message Size		*/
					&errcd				/* Error code              */
					);
	/* checking parameter */
	if( ret != BPF_RU_IPCM_OK ) 
	{
		rrhApi_com_log(__func__, errcd);
		
		return E_API_RCD_NG;
	}

	memcpy(data_p,&(apiRsp.data),sizeof(T_API_COM_RSP_DATA));
	
	return E_API_RCD_OK;
}


/********************************************************************************************************************/
/**
 *  @brief  API : The request of startup cpri simu mode
 *  @note   PF EVENT ID : 0xA00A0001
 *          Reponse  ID : 0xA00A0002
 *          TYPE        : MNR
 *  @param  qid     [I] : response queue id (calling process QueueID)
 *  @param  wtime   [I] : wait time(msec) for response
 *  @param  data_p  [O] : destination of response message
 *  @param  linkno  [I] : link NO.
 *  
 *  @return INT
 *  @retval ret
 *  @date   2013/11/08 FFCS) Create.
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Dbg_Mqr_StopCpriSimu(INT	qid,	INT	wtime,VOID *data_p,
								UINT linkno)
{
	T_API_DBG_CPRISIMU_STOP_REQ	apiReq;
	T_API_DBG_CPRISIMU_STOP_RSP	apiRsp;
	INT								errcd;
	INT								ret;
	UINT							size;
	UINT							msgLen;
	
	if(rrhApi_com_checkRespQ(qid) != E_API_RCD_OK)
		return E_API_RCD_EPARAM;

	memset(&apiReq,0,sizeof(apiReq));
	memset(&apiRsp,0,sizeof(apiRsp));

	size = sizeof(T_API_DBG_CPRISIMU_STOP_REQ);

	/*fill IF*/
	apiReq.header.uiEventNo		= D_API_MSGID_DBG_CPRISIMU_STOP_REQ;/* Event  ID         */
	apiReq.header.uiSignalkind	= 0;								/* Signal Kind       */
	apiReq.header.uiDstPQueueID	= D_RRH_PROCQUE_L2;					/* Destination P QID */
	apiReq.header.uiDstTQueueID	= 0;								/* Destination T QID */
	apiReq.header.uiSrcPQueueID	= qid;								/* Source P QID      */
	apiReq.header.uiSrcTQueueID	= 0;								/* Source T QID      */
	apiReq.header.uiLength		= sizeof(apiReq);					/* Length            */
	apiReq.data.linkno          = linkno;                           /* Link NO.          */

	/* Queue Clear	*/
	(VOID)BPF_RU_IPCM_PROCMSGQ_FREE_STACK_BUFF( qid );
	
	/* call MW func */
	ret = rrhApi_com_msgQSend(
				D_RRH_PROCQUE_L2,					/* Message QID             */
				(void *)&apiReq,					/* The message to transmit */
				sizeof(apiReq),						/* The size of message     */
				&errcd								/* Error code              */
				);
	/* checking parameter */
	if( ret != BPF_RU_IPCM_OK ) {
		rrhApi_com_log(__func__, errcd);

		return BPF_RU_IPCM_NG;
	}

	size = sizeof(apiRsp);
	ret = rrhApi_com_msgQReceive(
					qid,				/* [in]Que no					*/
					wtime,				/* [in]Wait Time				*/
					size,				/* [in]Pointer of recieve buffer	*/
					&apiRsp,			/* [in]Recieve Buffer Size		*/
					&msgLen,			/* [out]Reciept Message Size		*/
					&errcd				/* Error code              */
					);
	/* checking parameter */
	if( ret != BPF_RU_IPCM_OK ) 
	{
		rrhApi_com_log(__func__, errcd);
		
		return E_API_RCD_NG;
	}

	memcpy(data_p,&(apiRsp.data),sizeof(T_API_COM_RSP_DATA));

	return  E_API_RCD_OK;
}
/********************************************************************************************************************/
/**
 *  @brief  API : The notice of start cpri monitor mode
 *  @note   PF EVENT ID : 0xA00A0005
 *          TYPE        : MNT
 *  @param  qid     [I] : response queue id (calling process QueueID)
 *  @param  wtime   [I] : wait time(msec) for response
 *  @param  data_p  [I] : not use
 *  @param  linkno  [I] : link NO.
 *  @return INT
 *  @retval ret
 *  @date   2013/11/08 FFCS) Create.
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Dbg_Mnt_StartupCpriMonitor(	INT	qid,	INT	wtime,VOID *data_p,
										UINT linkno)
{
	T_API_DBG_CPRIMON_START_IND	apiInd;
	INT								errcd;
	INT								ret;

	memset(&apiInd,0,sizeof(apiInd));

	/*fill IF*/
	apiInd.header.uiEventNo		= D_API_MSGID_DBG_CPRIMON_START_IND;/* Event  ID         */
	apiInd.header.uiSignalkind	= 0;								/* Signal Kind       */
	apiInd.header.uiDstPQueueID	= D_RRH_PROCQUE_L2;					/* Destination P QID */
	apiInd.header.uiDstTQueueID	= 0;								/* Destination T QID */
	apiInd.header.uiSrcPQueueID	= qid;								/* Source P QID      */
	apiInd.header.uiSrcTQueueID	= 0;								/* Source T QID      */
	apiInd.header.uiLength		= sizeof(apiInd);					/* Length            */
	apiInd.data.linkno          = linkno;                           /* Link NO.          */

	/* call MW func */
	ret = rrhApi_com_msgQSend(
				D_RRH_PROCQUE_L2,					/* Message QID             */
				(void *)&apiInd,					/* The message to transmit */
				sizeof(apiInd),						/* The size of message     */
				&errcd								/* Error code              */
				);

	/* checking parameter */
	if( ret != BPF_RU_IPCM_OK ) {
		rrhApi_com_log(__func__, errcd);

		return E_API_RCD_NG;
	}

	return E_API_RCD_OK;
}
/********************************************************************************************************************/
/**
 *  @brief  API : The notice of stop cpri monitor mode
 *  @note   PF EVENT ID : 0xA00A0007
 *          TYPE        : MNT
 *  @param  qid     [I] : response queue id (calling process QueueID)
 *  @param  wtime   [I] : wait time(msec) for response
 *  @param  data_p  [I] : not use 
 *  @param  linkno  [I] : link NO.
 *  @return INT
 *  @retval ret
 *  @date   2013/11/08 FFCS) Create.
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Dbg_Mnt_StopCpriMonitor(	INT	qid,	INT	wtime,VOID *data_p,
									UINT linkno)
{
	T_API_DBG_CPRIMON_STOP_IND	    apiInd;
	INT								errcd;
	INT								ret;

	memset(&apiInd,0,sizeof(apiInd));

	/*fill IF*/
	apiInd.header.uiEventNo		= D_API_MSGID_DBG_CPRIMON_STOP_IND; /* Event  ID         */
	apiInd.header.uiSignalkind	= 0;								/* Signal Kind       */
	apiInd.header.uiDstPQueueID	= D_RRH_PROCQUE_L2;					/* Destination P QID */
	apiInd.header.uiDstTQueueID	= 0;								/* Destination T QID */
	apiInd.header.uiSrcPQueueID	= qid;								/* Source P QID      */
	apiInd.header.uiSrcTQueueID	= 0;								/* Source T QID      */
	apiInd.header.uiLength		= sizeof(apiInd);					/* Length            */
	apiInd.data.linkno          = linkno;                           /* Link NO.          */

	/* call MW func */
	ret = rrhApi_com_msgQSend(
				D_RRH_PROCQUE_L2,					/* Message QID             */
				(void *)&apiInd,					/* The message to transmit */
				sizeof(apiInd),						/* The size of message     */
				&errcd								/* Error code              */
				);

	/* checking parameter */
	if( ret != BPF_RU_IPCM_OK ) {
		rrhApi_com_log(__func__, errcd);

		return E_API_RCD_NG;
	}

	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief  API : The request of startup cpri simu mode
 *  @note   PF EVENT ID : 0xA00A0008
 *          Reponse  ID : 0xA00A0009
 *          TYPE        : MNR
 *  @param  qid     [I] : response queue id (calling process QueueID)
 *  @param  wtime   [I] : wait time(msec) for response 
 *  @param  data_p  [O] : destination of response message
 *  @param  ipAddr  [I]	: IP addrss to configure
 *  @param  ipMask  [I]	: IP addrss mask to configure
 *  
 *  @return INT
 *  @retval ret
 *  @date   2013/11/08 FFCS) Create.
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Dbg_Mqr_IfConfig(	INT	qid,	INT	wtime,VOID *data_p,
								UINT			ipAddr,
								UINT			ipMask)
{
	T_API_DBG_IFCONFIG_REQ	apiReq;
	T_API_DBG_IFCONFIG_RSP	apiRsp;
	INT							errcd;
	INT							ret;
	UINT							size;
	UINT							msgLen;

	memset(&apiReq,0,sizeof(apiReq));
	memset(&apiRsp,0,sizeof(apiRsp));

	/*fill IF*/
	apiReq.header.uiEventNo		= D_API_MSGID_DBG_IFCONFIG_REQ;		/* Event  ID         */
	apiReq.header.uiSignalkind	= 0;								/* Signal Kind       */
	apiReq.header.uiDstPQueueID	= D_RRH_PROCQUE_PF;				/* Destination P QID */
	apiReq.header.uiDstTQueueID	= 0;								/* Destination T QID */
	apiReq.header.uiSrcPQueueID	= qid;								/* Source P QID      */
	apiReq.header.uiSrcTQueueID	= 0;								/* Source T QID      */
	apiReq.header.uiLength		= sizeof(apiReq);					/* Length            */
	apiReq.data.ipAddr          = ipAddr;                           /* IP addrss         */
	apiReq.data.ipMask          = ipMask;                           /* IP addrss mask    */

	/* call MW func */
	ret = rrhApi_com_msgQSend(
				D_RRH_PROCQUE_PF,					/* Message QID             */
				(void *)&apiReq,					/* The message to transmit */
				sizeof(apiReq),						/* The size of message     */
				&errcd								/* Error code              */
				);
	/* checking parameter */
	if( ret != BPF_RU_IPCM_OK ) {
		rrhApi_com_log(__func__, errcd);

		return BPF_RU_IPCM_NG;
	}

	size = sizeof(apiRsp);
	ret = rrhApi_com_msgQReceive(
					qid,				/* [in]Que no					*/
					wtime,				/* [in]Wait Time				*/
					size,				/* [in]Pointer of recieve buffer	*/
					&apiRsp,			/* [in]Recieve Buffer Size		*/
					&msgLen,			/* [out]Reciept Message Size		*/
					&errcd				/* Error code              */
					);
	/* checking parameter */
	if( ret != BPF_RU_IPCM_OK ) 
	{
		rrhApi_com_log(__func__, errcd);
		
		return E_API_RCD_NG;
	}

	if(data_p)
	memcpy(data_p,&apiRsp,sizeof(T_API_DBG_IFCONFIG_RSP));

	return E_API_RCD_OK;
}
/********************************************************************************************************************/
/**
 *  @brief  API : Send cpri message to L2 RXS thread
 *  @note   PF EVENT ID : 0xA00A000a
 *          TYPE        : MNT
 *  @param  qid    [I]  : response queue id (calling process QueueID)
 *  @param  wtime  [I]  : wait time(msec) for response
 *  @param  data_p [I]  : not use
 *  @param  inf_p  [I]  : msg pointer.
 *  @return INT
 *  @retval ret
 *  @date   2013/11/29 FFCS) Create.
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
*/
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Dbg_Mnt_SendSimuCpriMsg(	INT	qid,	INT	wtime,VOID *data_p,
									VOID*		inf_p)
{
	T_API_DBG_SEND_SIMUCPRIMSG_IND	*snd;
	UINT							size;
	INT								errcd;
	INT								ret;

	snd = (T_API_DBG_SEND_SIMUCPRIMSG_IND *)inf_p;
	size = snd->header.uiLength;

	/*fill IF*/
	snd->header.uiEventNo		= D_API_MSGID_DBG_SEND_SIMUCPRIMSG; /*!< Event  ID         */
	snd->header.uiSignalkind	= 0;								/*!< Signal Kind       */
	snd->header.uiDstPQueueID	= D_RRH_PROCQUE_L2;					/*!< Destination P QID */
	snd->header.uiDstTQueueID	= 0;					 /*!< Destination T QID */
	snd->header.uiSrcPQueueID	= qid;								/*!< Source P QID      */
	snd->header.uiSrcTQueueID	= 0;								/*!< Source T QID      */


	/* call MW func */
	ret = rrhApi_com_msgQSend(
				D_RRH_PROCQUE_L2,					/*!< Message QID             */
				(void *)snd,						/*!< The message to transmit */
				size,								/*!< The size of message     */
				&errcd								/*!< Error code              */
				);

	/* checking parameter */
	if( ret != BPF_RU_IPCM_OK ) {
		rrhApi_com_log(__func__, errcd);

		return E_API_RCD_NG;
	}

	return E_API_RCD_OK;
}
/** @} */

