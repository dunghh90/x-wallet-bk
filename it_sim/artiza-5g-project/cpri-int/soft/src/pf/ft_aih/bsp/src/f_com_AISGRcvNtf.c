/*!
 * @skip  $ld:$
 * @file  f_com_AISGRcvNtf.c
 * @brief Send AISG data receiving notification to AL2 task.
 * @date  2010/10/10 FFCS)Zouw create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2010
 */
 
/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_com_inc.h"

/*!
 *  @brief  Send AISG data receiving interrupt notification to AL2 task.
 *          - this function is called by AISG interrupt.
 *  @note   This function is processed as follows.\n
 *         - fill message in message head 
 *         - send AISG receiving notification to AL2 task
 *  @param   *f_rcvFunc [in] function pointer
 *                iParam   [in] the input param
 *  @return VOID
 *  @retval none  
 *  @date   2010/10/10 FFCS)Zouw create
 */

void f_com_AISGRcvIntNtf(VOIDFUNCPTR f_rcvFunc,VOID *iParam)
{
	T_SYS_AISG_RCV_INT_NTC Rcvntc;   /* receiving data notification table */

	memset(&Rcvntc,0,sizeof(Rcvntc));
	/*************************/
	/*fill message */
	/*************************/
	/* 1. message head */
	Rcvntc.thdIf.uiEventNo = D_SYS_MSGID_AIH_ALDRCV_NTF;
	Rcvntc.thdIf.uiLength = sizeof(Rcvntc);
	Rcvntc.thdIf.uiSignalkind = D_DU_IFLOG_OFF;	/* ログoff */

	/* 2. message body */
	Rcvntc.f_func = f_rcvFunc;
	Rcvntc.iParam = iParam;

	/*************************/
	/*send a message */
	/************************/
	f_com_msgQSend(
				D_SYS_THDQID_PF_AIH,
				(CHAR*)&Rcvntc,
				sizeof(T_SYS_AISG_RCV_INT_NTC),
				D_SYS_THDQID_PF_AIH,
				D_SYS_ALLF_INT,
				D_SYS_ALLF_INT);

	return;
}

/*!
 *  @brief  Send AISG receiving interrupt notification to AL2 task.
 *          - this function is called by AISG interrupt.
 *  @note   This function is processed as follows.\n
 *         - send AISG receiving notification to AL2 task
 *  @param   none
 *  @return VOID
 *  @retval none  
 *  @date   2015/10/14 FFCS)Wuqy create
 */

void f_com_AISGRcvingIntNtf()
{
	T_SYS_AISG_RCVING_INT_NTC Rcvntc;   /* receiving data notification table */

	memset(&Rcvntc,0,sizeof(Rcvntc));
	/*************************/
	/*fill message */
	/*************************/
	/* 1. message head */
	Rcvntc.thdIf.uiEventNo = D_SYS_MSGID_AIH_ALDRCV_START;
	Rcvntc.thdIf.uiLength = sizeof(Rcvntc);
	Rcvntc.thdIf.uiSignalkind = D_DU_IFLOG_OFF;	/* ログoff */

	/*************************/
	/*send a message */
	/************************/
	f_com_msgQSend(
				D_SYS_THDQID_PF_AIH,
				(CHAR*)&Rcvntc,
				sizeof(T_SYS_AISG_RCVING_INT_NTC),
				D_SYS_THDQID_PF_AIH,
				D_SYS_ALLF_INT,
				D_SYS_ALLF_INT);

	return;
}


/*!
 *  @brief  Send AISG data receiving data notification to AL2 task.
 *          - this function is called by AISG interrupt.
 *  @note   This function is processed as follows.\n
 *         - parameter check
 *         - fill message in message head 
 *            if L1 data is correct, copy data buffer into message buffer
 *            else just save the error code in the message
 *         - send AISG receiving notification to AL2 task
 *  @param  *pData [in] buffer pointer
 *                nSize   [in] receiving data length
 *                uiResult [in] receiving data result
 *  @return VOID
 *  @retval none  
 *  @date   2010/10/10 FFCS)Zouw create
 */

void f_com_AISGRcvDatNtf(char *pData,UINT nSize, UINT uiResult,USHORT *usErr_p,UINT allSize)
{
	T_SYS_AISG_RCV_DAT_NTC Rcvntc;   	/* receiving data notification table */
	UINT uiDatalen;              			/* data length */

	memset(&Rcvntc,0,sizeof(Rcvntc));

	/*************************/
	/*fill message */
	/*************************/
	/* 1. message head */
	Rcvntc.thdIf.uiEventNo = D_SYS_MSGID_AIH_ALDRCV_END;
	Rcvntc.thdIf.uiLength = sizeof(T_SYS_AISG_RCV_DAT_NTC);
	Rcvntc.thdIf.uiSignalkind = D_DU_IFLOG_OFF;	/* ログoff */

	/* 2. message body */
	Rcvntc.uiResult = uiResult;
	Rcvntc.uiLength = nSize;
	Rcvntc.uiAllLength = allSize;

	if(uiResult == D_SYS_OK) /* L1 data correct */
	{
		/* 3. set data length */
		if(nSize <= D_SYS_NUM1200 )
		{
			uiDatalen = nSize;
		}
		else
		{
			uiDatalen = D_SYS_NUM1200;
		}
		/*4. data copy */
		memcpy(Rcvntc.ucData, pData, uiDatalen);
	}
	else{
		memcpy(Rcvntc.usErr,usErr_p,sizeof(Rcvntc.usErr));
	}
	/*************************/
	/*send a message */
	/************************/
	f_com_msgQSend(
				D_SYS_THDQID_PF_AIH,
				(CHAR*)&Rcvntc,
				sizeof(T_SYS_AISG_RCV_DAT_NTC),
				D_SYS_THDQID_PF_AIH,
				D_SYS_ALLF_SHORT,
				D_SYS_ALLF_SHORT);
	
	return;
}
/* @} */

