/*!
 * @skip  $ld:$
 * @file  rrhApi_L2.c
 * @brief API : related to L2 operation
 * @date  2013/11/08 FFCS) Create.
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/** @addtogroup RRH_API_L2
 *  @{
 */
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "f_rrh_inc.h"
#include "BPF_RU_IPCM.h"

#include "rrhApi_L2.h"

/********************************************************************************************************************/
/**
 *  @brief  API: Setup lapb link
 *  @note   PF EVENT ID : 0xA00B0001
 *          Reponse  ID : -
 *          TYPE        : MNR
 *  @param  linkno  [I] : CPRIリンク番号
 *  @param  qid     [I] : response queue id (calling process QueueID)
 *  @param  wtime   [I] : wait time(msec) for response 
 *  @param  data_p  [O] : destination of response message
 *  
 *  @return INT
 *  @retval ret
 *  @date   2013/11/08 FFCS) Create.
 *  @date   2013/11/08 FFCS) Create.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_L2_Mqr_SetupLink(USHORT linkno,	INT		qid,
								INT		wtime,
								VOID	*data_p )
{
	T_API_L2LINK_SETUP_REQ          apiReq;
	T_API_L2LINK_SETUP_RSP          apiRsp;
	INT								errcd;
	INT								ret;
	UINT							size;
	UINT							msgLen;
	
	if(rrhApi_com_checkRespQ(qid) != E_API_RCD_OK)
		return E_API_RCD_EPARAM;

	memset(&apiReq,0,sizeof(apiReq));
	memset(&apiRsp,0,sizeof(apiRsp));
	
	/*fill IF*/
	apiReq.header.uiEventNo		= D_API_MSGID_L2_LINK_SETUP_REQ;	/* Event  ID         */
	apiReq.header.uiSignalkind	= 0;								/* Signal Kind       */
	apiReq.header.uiDstPQueueID	= D_RRH_PROCQUE_L2;				/* Destination P QID */
	apiReq.header.uiDstTQueueID	= 0;								/* Destination T QID */
	apiReq.header.uiSrcPQueueID	= qid;								/* Source P QID      */
	apiReq.header.uiSrcTQueueID	= 0;								/* Source T QID      */
	apiReq.header.uiLength		= sizeof(apiReq);					/* Length            */
	apiReq.link_num = linkno;

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

		return E_API_RCD_NG;
	}

	size = sizeof(apiRsp);
	ret = rrhApi_com_msgQReceive(
					qid,				/* [in]Que no					*/
					wtime,				/* [in]Wait Time				*/
					size,				/* [in]Pointer of recieve buffer*/
					&apiRsp,			/* [in]Recieve Buffer Size	*/
					&msgLen,			/* [out]Reciept Message Size	*/
					&errcd				/* Error code                 */
					);
	/* checking parameter */
	if( ret != BPF_RU_IPCM_OK ) {
		rrhApi_com_log(__func__, errcd);

       	return E_API_RCD_NG;
	}

	memcpy(data_p,&apiRsp.data,sizeof(T_API_COM_RSP_DATA));

	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief  API: Release lapb link
 *  @note   PF EVENT ID : 0xA00B0003
 *          Reponse  ID : -
 *          TYPE        : MNR
 *  @param  linkno  [I] : CPRIリンク番号
 *  @param  qid   [I]   : response queue id (calling process QueueID)
 *  @param  wtime [I]   : wait time(msec) for response
 *  @param  data_p  [O] : destination of response message
 *  
 *  @return INT
 *  @retval ret
 *  @date   2013/11/08 FFCS) Create.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_L2_Mqr_ReleaseLink(USHORT linkno, 	INT		qid,
								INT		wtime,
								VOID	*data_p )
{
	T_API_L2LINK_REL_REQ            apiReq;
	T_API_L2LINK_REL_RSP            apiRsp;
	INT								errcd;
	INT								ret;
	UINT							size;
	UINT							msgLen;
	
	if(rrhApi_com_checkRespQ(qid) != E_API_RCD_OK)
		return E_API_RCD_EPARAM;

	memset(&apiReq,0,sizeof(apiReq));
	memset(&apiRsp,0,sizeof(apiRsp));

	/*fill IF*/
	apiReq.header.uiEventNo		= D_API_MSGID_L2_LINK_REL_REQ;		/* Event  ID         */
	apiReq.header.uiSignalkind	= 0;								/* Signal Kind       */
	apiReq.header.uiDstPQueueID	= D_RRH_PROCQUE_L2;				/* Destination P QID */
	apiReq.header.uiDstTQueueID	= 0;								/* Destination T QID */
	apiReq.header.uiSrcPQueueID	= qid;								/* Source P QID      */
	apiReq.header.uiSrcTQueueID	= 0;								/* Source T QID      */
	apiReq.header.uiLength		= sizeof(apiReq);					/* Length            */
	apiReq.link_num = linkno;

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
					size,				/* [in]Pointer of recieve buffer*/
					&apiRsp,			/* [in]Recieve Buffer Size	*/
					&msgLen,			/* [out]Reciept Message Size	*/
					&errcd				/* Error code                 */
					);
	/* checking parameter */
	if( ret != BPF_RU_IPCM_OK )	{
		rrhApi_com_log(__func__, errcd);

        return E_API_RCD_NG;
	}
	memcpy(data_p,&apiRsp.data,sizeof(T_API_COM_RSP_DATA));

	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief  API: Deative lapb link
 *  @note   PF EVENT ID : 0xA00B0005
 *          Reponse  ID : -
 *          TYPE        : MNT
 *  @param  linkno  [I] : CPRIリンク番号
 *  @param  qid     [I] : response queue id (calling process QueueID)
 *  @param  wtime   [I] : wait time(msec) for response 
 *  @param  data_p  [I] : not use
 *  @return INT
 *  @date   2013/11/08 FFCS) Create.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_L2_Mnt_DeactLapbLink(USHORT linkno, INT		qid,
									INT		wtime,VOID	*data_p  )
{
	T_API_L2LINK_DEACT_IND          apiReq;
	INT								errcd;
	INT								ret;

	memset(&apiReq,0,sizeof(apiReq));

	/*fill IF*/
	apiReq.header.uiEventNo		= D_API_MSGID_L2_LINK_DEACT_IND;	/* Event  ID         */
	apiReq.header.uiSignalkind	= 0;								/* Signal Kind       */
	apiReq.header.uiDstPQueueID	= D_RRH_PROCQUE_L2;				/* Destination P QID */
	apiReq.header.uiDstTQueueID	= 0;								/* Destination T QID */
	apiReq.header.uiSrcPQueueID	= qid;								/* Source P QID      */
	apiReq.header.uiSrcTQueueID	= 0;								/* Source T QID      */
	apiReq.header.uiLength		= sizeof(apiReq);					/* Length            */
	apiReq.link_num = linkno;

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

		return E_API_RCD_NG;
	}

	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief  API: Make subscription of receiving CPRI message
 *  @note   PF EVENT ID : 0xA00B0007
 *          Reponse  ID : -
 *          TYPE        : MPR
 *  @param  linkno  [I] : CPRIリンク番号
 *  @param  qid     [I] : response queue id (calling process QueueID)
 *  @param  wtime   [I] : wait time(msec) for response 
 *  @param  data_p  [I] : not use
 *  @param  msgs    [I] : the pointer to message 
 *  @param  nMsg    [I] : the count of message indicated in parameter 4
 *  @return INT
 *  @retval ret
 *  @date   2013/11/08 FFCS) Create.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_L2_Mpr_SubscribeCpriMsg(USHORT linkno,	INT		qid, INT		wtime,VOID	*data_p ,
								   UINT* msgs,UINT nMsg)
{
	T_API_CPRIMSG_SUB_REQ         apiReq;
	INT								errcd;
	INT								ret;

	if(rrhApi_com_checkRespQ(qid) != E_API_RCD_OK)
		return E_API_RCD_EPARAM;

	memset(&apiReq,0,sizeof(apiReq));
	
	/*fill IF*/
	apiReq.header.uiEventNo		= D_API_MSGID_L2_CPRIMSG_SUB_REQ;	/* Event  ID         */
	apiReq.header.uiSignalkind	= 0;								/* Signal Kind       */
	apiReq.header.uiDstPQueueID	= D_RRH_PROCQUE_L2;				/* Destination P QID */
	apiReq.header.uiDstTQueueID	= 0;								/* Destination T QID */
	apiReq.header.uiSrcPQueueID	= qid;								/* Source P QID      */
	apiReq.header.uiSrcTQueueID	= 0;								/* Source T QID      */
	apiReq.header.uiLength		= sizeof(apiReq);					/* Length            */
	apiReq.data.msgCount		= nMsg;                             /* the count of message */
	apiReq.link_num = linkno;
	memcpy(apiReq.data.msgs, msgs, sizeof(UINT) * nMsg);                 /* messages          */

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

		return E_API_RCD_NG;
	}
	
	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief  API : Make un-subscription of receiving CPRI message
 *  @note   PF EVENT ID : 0xA00B0009
 *          TYPE        : MNT
 *  @param  linkno  [I] : CPRIリンク番号
 *  @param  qid     [I] : response queue id (calling process QueueID)
 *  @param  wtime   [I] : wait time(msec) for response 
 *  @param  data_p  [I] : not use
 *  @return INT
 *  @retval ret
 *  @date   2013/11/08 FFCS) Create.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_L2_Mnt_UnSubscribeCpriMsg(USHORT linkno,	INT		qid,
										INT		wtime ,VOID	*data_p )
{
	T_API_CPRIMSG_UNSUB_IND       apiInd;
	INT								errcd;
	INT								ret;

	memset(&apiInd,0,sizeof(apiInd));

	/*fill IF*/
	apiInd.header.uiEventNo		= D_API_MSGID_L2_CPRIMSG_UNSUB_IND;	/* Event  ID         */
	apiInd.header.uiSignalkind	= 0;								/* Signal Kind       */
	apiInd.header.uiDstPQueueID	= D_RRH_PROCQUE_L2;				/* Destination P QID */
	apiInd.header.uiDstTQueueID	= 0;								/* Destination T QID */
	apiInd.header.uiSrcPQueueID	= qid;								/* Source P QID      */
	apiInd.header.uiSrcTQueueID	= 0;								/* Source T QID      */
	apiInd.header.uiLength		= sizeof(apiInd);					/* Length            */
	apiInd.link_num = linkno;
	apiInd.system_type = 0xFFFF;	/*	未使用	*/

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
 *  @brief  API: Make subscription of LAPB link state
 *  @note   PF EVENT ID : 0xA00B000D
 *          Reponse  ID : -
 *          TYPE        : MPR
 *  @param  linkno  [I] : CPRIリンク番号
 *  @param  qid     [I] : response queue id (calling process QueueID)
 *  @param  wtime   [I] : wait time(msec) for response 
 *  @param  data_p  [I] : not use
 *  @return INT
 *  @retval ret
 *  @date   2013/11/08 FFCS) Create.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
*/
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_L2_Mpr_SubscribeLinkState(USHORT linkno,	INT		qid,
										INT		wtime,VOID	*data_p  )
{
	T_API_L2_LINKSTATE_SUB_REQ      apiReq;
	INT								errcd;
	INT								ret;
	
	if(rrhApi_com_checkRespQ(qid) != E_API_RCD_OK)
		return E_API_RCD_EPARAM;

	memset(&apiReq,0,sizeof(apiReq));

	/*fill IF*/
	apiReq.header.uiEventNo		= D_API_MSGID_L2_LINKSTATE_SUB_REQ;	/* Event  ID         */
	apiReq.header.uiSignalkind	= 0;								/* Signal Kind       */
	apiReq.header.uiDstPQueueID	= D_RRH_PROCQUE_L2;				/* Destination P QID */
	apiReq.header.uiDstTQueueID	= 0;								/* Destination T QID */
	apiReq.header.uiSrcPQueueID	= qid;								/* Source P QID      */
	apiReq.header.uiSrcTQueueID	= 0;								/* Source T QID      */
	apiReq.header.uiLength		= sizeof(apiReq);					/* Length            */
	apiReq.link_num = linkno;

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
		
		return E_API_RCD_NG;
	}

	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief  API : Make un-subscription of LAPB link state
 *  @note   PF EVENT ID : 0xA00B000F
 *          TYPE        : MNT
 *  @param  linkno  [I] : CPRIリンク番号
 *  @param  qid     [I] : response queue id (calling process QueueID)
 *  @param  wtime   [I] : wait time(msec) for response 
 *  @param  data_p  [I] : not use
 *  @return INT
 *  @retval ret
 *  @date   2013/11/08 FFCS) Create.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_L2_Mnt_UnsubscribeLinkState(	USHORT linkno, INT		qid,
										INT		wtime ,VOID	*data_p )
{
	T_API_CPRIMSG_UNSUB_IND       apiInd;
	INT								errcd;
	INT								ret;

	memset(&apiInd,0,sizeof(apiInd));
	
	/*fill IF*/
	apiInd.header.uiEventNo		= D_API_MSGID_L2_LINKSTATE_UNSUB_IND;/* Event  ID         */
	apiInd.header.uiSignalkind	= 0;								/* Signal Kind       */
	apiInd.header.uiDstPQueueID	= D_RRH_PROCQUE_L2;				/* Destination P QID */
	apiInd.header.uiDstTQueueID	= 0;								/* Destination T QID */
	apiInd.header.uiSrcPQueueID	= qid;								/* Source P QID      */
	apiInd.header.uiSrcTQueueID	= 0;								/* Source T QID      */
	apiInd.header.uiLength		= sizeof(apiInd);					/* Length            */
	apiInd.link_num = linkno;
	apiInd.system_type = 0xFFFF;	/*	未使用	*/

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
 *  @brief  API : Make un-subscription of LAPB link state
 *  @note   PF EVENT ID : 0xA00B000F
 *          TYPE        : MNT
 *  @param  linkno  [I] : CPRIリンク番号
 *  @param  system  [I] : システム(0:3G, 1:LTE)
 *  @param  qid     [I] : response queue id (calling process QueueID)
 *  @param  wtime   [I] : wait time(msec) for response 
 *  @param  data_p  [I] : not use
 *  @return INT
 *  @date   2013/11/08 FFCS) Create.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_L2_Mnt_CpriMsgStatistic(USHORT linkno, 	USHORT system, 	INT		qid,
									INT		wtime,VOID	*data_p  )
{
	T_API_CPRIMSG_UNSUB_IND       apiInd;
	INT								errcd;
	INT								ret;

	memset(&apiInd,0,sizeof(apiInd));
	
	/*fill IF*/
	apiInd.header.uiEventNo		= D_API_MSGID_L2_CPRIMSG_STASTIC_IND;/* Event  ID         */
	apiInd.header.uiSignalkind	= 0;								/* Signal Kind       */
	apiInd.header.uiDstPQueueID	= D_RRH_PROCQUE_L2;				/* Destination P QID */
	apiInd.header.uiDstTQueueID	= 0;								/* Destination T QID */
	apiInd.header.uiSrcPQueueID	= qid;								/* Source P QID      */
	apiInd.header.uiSrcTQueueID	= 0;								/* Source T QID      */
	apiInd.header.uiLength		= sizeof(apiInd);					/* Length            */
	apiInd.link_num = linkno;
	apiInd.system_type = system;

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
 *  @brief  API : the request of sending CPRI message
 *  @note   PF EVENT ID : 0xA00B000F
 *          TYPE        : MNT
 *  @param  qid     [I] : response queue id (calling process QueueID)
 *  @param  wtime   [I] : wait time(msec) for response 
 *  @param  data_p  [I] : not use
 *  @param  msg_p   [I] : pointer of sending message
 *  @return INT
 *  @retval ret
 *  @date   2013/11/08 FFCS) Create.
 *  @Bug_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_L2_Mnt_SendCpriMsg(	INT		qid, 
								INT		wtime,VOID	*data_p,VOID	*msg_p  )
{
	INT							retcd;

	if(msg_p == D_RRH_NULL)
		return E_API_RCD_EPARAM;
	if(((T_API_COMMON_HEADER*)msg_p)->uiLength < 1)
		return E_API_RCD_EPARAM;

	((T_API_COMMON_HEADER*)msg_p)->uiDstPQueueID = D_RRH_PROCQUE_L2;
	((T_API_COMMON_HEADER*)msg_p)->uiDstTQueueID = 0;
	((T_API_COMMON_HEADER*)msg_p)->uiSignalkind = 0;
	((T_API_COMMON_HEADER*)msg_p)->uiEventNo = D_API_MSGID_L2_CPRIMSG_SEND_IND;

	retcd = BPF_RU_IPCM_MSGQ_SEND(	
					D_RRH_PROCQUE_L2,	/* 直近の送信先QueueID( Thread Queue ID or Process Queue ID )		*/
					(void *)msg_p		/* 送信メッセージで取得したアドレス	*/
					);
	if( retcd != BPF_RU_IPCM_OK ) {
		rrhApi_com_log(__func__,retcd);

		return E_API_RCD_NG;
	}
		
	return E_API_RCD_OK;
}
/** @} */

