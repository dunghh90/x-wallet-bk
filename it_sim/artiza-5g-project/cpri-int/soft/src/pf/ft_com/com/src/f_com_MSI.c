/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_com_MSI.c
 *  @brief	MSI common function
 *  @date   2018/11/19 FJT)Taniguchi create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/*****************************************************************************/

/** @addtogroup TRA_COM
 *  @{
 */
 /*******************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

#include "f_sys_inc.h"
#include "f_com_inc.h"
#include "f_mpsw_msg_MSI.h"

/*****************************************************************************/
/**
 *  @brief  MSI message send function
 *  @note   MSI message send to CMU
 *  @param  messageId	[in] MSI message ID
 *  @param  requestId	[in] MSI request ID
 *  @param  payloadSize	[in] MSI payload size
 *  @param  payloadPtr	[in] MSI payload pointer
 *  @param  srcPQueueID	[in] Source Process Queue ID
 *  @param  srcTQueueID	[in] Source Thread Queue ID
 *  @return return code 
 *  @retval D_SYS_OK
 *  @retval D_SYS_NG 
 *  @date   2018/11/19 FJT)Taniguchi create
 *  @warning	N/A
 *  @FeatureID	RRH-002-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/*****************************************************************************/
INT f_com_MSIsend(
	UINT		messageId,
	UINT		requestId,
	INT			payloadSize,
	VOID*		payloadPtr,
	UINT		srcPQueueID,
	UINT		srcTQueueID
)
{
	
	VOID*		sndMsg;
	INT			ret;
	INT			errcd;
	T_SYS_MSI_SND_IND*	indPtr;
	T_MPSW_SRV_MSGHEAD*	mpheader;
	CHAR*				mpdata;

	
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			
											sizeof( T_SYS_MSI_SND_IND ) + sizeof(T_MPSW_SRV_MSGHEAD) + payloadSize,		
											(VOID **)&sndMsg,					
											&errcd );								
	if(ret != BPF_RU_IPCM_OK){
		return D_SYS_NG;
	}

	indPtr = (T_SYS_MSI_SND_IND*)sndMsg;
	indPtr->thdIf.uiDstPQueueID	= srcPQueueID;
	indPtr->thdIf.uiDstTQueueID	= D_SYS_THDQID_PF_MPSEND;
	indPtr->thdIf.uiSrcPQueueID	= srcPQueueID;
	indPtr->thdIf.uiSrcTQueueID	= srcTQueueID;
	indPtr->thdIf.uiEventNo		= D_SYS_MSGID_MSI_SND_IND;

	mpheader = (T_MPSW_SRV_MSGHEAD*)(((CHAR*)sndMsg) + sizeof(*indPtr));
	mpheader->msgId = messageId;
	mpheader->requestId = requestId;
	mpheader->msgLength = payloadSize;

	if((payloadSize > 0) && (payloadPtr != NULL)){
		mpdata = ((CHAR*)mpheader) + sizeof(*mpheader);
		memcpy((VOID*)mpdata,payloadPtr,payloadSize);
	}

	BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_MPSEND , (VOID *)sndMsg );

	return D_SYS_OK;
}

/*****************************************************************************/
/**
 *  @brief  MSI message get payload function
 *  @note   MSI message get payload from thread if message
 *  @param  msgPtr			[in] received thread if pointer
 *  @param  messageId		[out] MSI message ID
 *  @param  requestId		[out] MSI request ID
 *  @param  payloadSizePtr	[out] MSI payload size pointer
 *  @param  payloadPtr		[out] MSI payload pointer
 *  @return N/A
 *  @date   2018/11/19 FJT)Taniguchi create
 *  @warning	N/A
 *  @FeatureID	RRH-002-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/*****************************************************************************/
VOID f_com_MSIgetPayload(
	VOID*		msgPtr,
	UINT*		messageId,
	UINT*		requestId,
	INT*		payloadSizePtr,
	VOID**		payloadPtr
)
{

	T_MPSW_SRV_MSGHEAD*	mpheader;
	mpheader = (T_MPSW_SRV_MSGHEAD*)(((CHAR*)msgPtr) + sizeof(T_SYS_MSI_RCV_IND));
	*messageId = mpheader->msgId;
	*requestId = mpheader->requestId;
	*payloadSizePtr = mpheader->msgLength;
	*payloadPtr = ((CHAR*)mpheader) + sizeof(*mpheader);
	return;
}

/* @} */
