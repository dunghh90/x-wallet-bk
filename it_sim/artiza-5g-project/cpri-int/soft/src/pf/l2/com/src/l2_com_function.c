/*!
 * @skip  $ld:$
 * @file  l2_com_function.c
 * @brief L2 共通 
 * @date   2013/11/20 FFCS)hongj create for zynq
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/** @addtogroup RRH_L2
 *  @{
 */
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "l2_com_inc.h"
#include "m_cm_header.h"
#include "rrhApi_Dbg.h"
#include "rrhApi_L2.h"
/*!
 *  @brief  The fucntion of sending message to APL 
 *  @note   This function is processed as follows.\n
 *          - Call MW function to send message to xam_main
 *  @return VOID
 *  @retval none
 *  @date  2013/11/27 FFCS)hongj create\n
 */

VOID l2_com_comMsgSnd(UINT msgQid,UINT msgid,void *msgp,UINT msgsz )
{
	/************************************************************************/
	/* Local parameter declaration											*/
	/************************************************************************/
	INT		endcd __attribute__((unused));						/*!< 終了コード  					*/
	INT		errcd;
	VOID	*shm_msgp;
	UINT	keyID;

	shm_msgp = NULL;
	/* Now the third parameter of BPF_RU_IPCM_PROCMSG_ADDRGET must be wrong, it should be "VOID **".*/
	endcd = BPF_RU_IPCM_PROCMSG_ADDRGET(
										E_BPF_RU_IPCM_BUFF_KIND_TASK,					/*!< Buffer種別					*/
										msgsz,					/*!< size						 */
										(void **)&shm_msgp,		/*!< 通信用共有メモリのポイント  */
										&errcd);				/*!< エラーコード				 */
	if(shm_msgp == D_RRH_NULL)
	{
		/*access log*/
		return ;
	}

	keyID = ((CMT_TSKIF_HEAD*)shm_msgp)->uiKeyID;
	memcpy(shm_msgp,msgp,msgsz);
	((CMT_TSKIF_HEAD*)shm_msgp)->uiKeyID = keyID;
	/************************************************************************/
	/* 他プロセスへメッセージを送信する										*/
	/************************************************************************/
	endcd = BPF_RU_IPCM_MSGQ_SEND(
							msgQid,			/* 直近の送信先QueueID( Thread Queue ID or Process Queue ID )		*/
							(void *)shm_msgp/* 送信メッセース BPF_RU_IPCM_PROCMSG_ADDRGET)で取得したアドレス    */
							);

	return;
}

/*!
 *  @brief  the proccess of sending message for apl
 *  @note   This function is processed as follows. \n
 *          - call BPF_RU_IPCM_MSGQ_RECV to hunter share memor buffer\n
 *          - copy message to apl memory buffer\n
 *          - call BPF_RU_IPCM_PROCMSG_ADDRFREE to release share memory\n
 *  @return VOID
 *  @retval none
 *  @date   2013/12/01 FFCS)niemsh Create\n
 */

INT l2_com_comMsgRcv(UINT msgQ,INT msgSize,VOID*rcvMsg,UINT*rcvLen,INT*errcd)
{
	VOID	*shm_msgp;
	int		ret;

	ret = BPF_RU_IPCM_MSGQ_RECV(
						msgQ,				/* 自分のQueueID								*/
						0,				/* 応答待ち時間(Process間Message受信時のみ有効      */
						(void **)&shm_msgp,	/* 通信用共有メモリのポイント  			*/
						rcvLen );			/* Receiveしたメッセージのサイス 		*/

	if(ret != BPF_RU_IPCM_OK)
		return D_RRH_NG;

	if(((CMT_TSKIF_HEAD*)shm_msgp)->uiLength > msgSize)
		memcpy(rcvMsg,shm_msgp,msgSize);
	else
		memcpy(rcvMsg,shm_msgp,((CMT_TSKIF_HEAD*)shm_msgp)->uiLength);

	ret = BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)shm_msgp );				/* 共有メモリのアドレス	*/

	return D_RRH_OK;
}
/*!
 *  @brief  The fucntion of sending message to APL 
 *  @note   This function is processed as follows.\n
 *          - Call MW function to send message to xam_main
 *  @return VOID
 *  @retval none
 *  @date  2013/11/27 FFCS)hongj create\n
 */

VOID l2_com_monitorMsgToApl(UINT msgQid,void *msgp,UINT msgsz )
{
	INT endcd __attribute__((unused));

	if(((CMT_TSKIF_HEAD*)msgp)->uiSrcTQueueID == CMD_TSKID_RXS)
	((CMT_TSKIF_HEAD*)msgp)->uiEventNo = D_API_MSGID_DBG_CPRIMON_DATA_RCV_NTC;
	else
	((CMT_TSKIF_HEAD*)msgp)->uiEventNo = D_API_MSGID_DBG_CPRIMON_DATA_SND_NTC;
	((CMT_TSKIF_HEAD*)msgp)->uiSignalkind = 0;
	((CMT_TSKIF_HEAD*)msgp)->uiLength = msgsz;

	/************************************************************************/
	/* 他プロセスへメッセージを送信する										*/
	/************************************************************************/
	endcd = BPF_RU_IPCM_MSGQ_SEND(
							msgQid,			/* 直近の送信先QueueID( Thread Queue ID or Process Queue ID )		*/
							(void *)msgp	/* 送信メッセース  BPF_RU_IPCM_PROCMSG_ADDRGET)で取得したアドレス   */
							);

	return;
}

/**************************************************************************************************/
/*!
 *  @brief  APLへメッセージを送信する
 *  @note   APLへメッセージを送信する。
 *  @param  msgQid      [in    ]    message queue id
 *  @param  msgp        [in/out]    pointer of message
 *  @param  msgsz       [in    ]    message size
 *  
 *  @return none
 *  @date   2013/11/27 FFCS)hongj create for zynq
 */
/**************************************************************************************************/
VOID l2_com_cpriMsgToApl(UINT msgQid,void *msgp,UINT msgsz )
{
	INT endcd __attribute__((unused));

	if(CMD_TSKID_LPB == ((CMT_TSKIF_HEAD*)msgp)->uiSrcTQueueID)
	((CMT_TSKIF_HEAD*)msgp)->uiEventNo = D_API_MSGID_L2_CPRIMSG_RCV_NTC;
	else
	{
		((CMT_TSKIF_HEAD*)msgp)->uiEventNo = D_API_MSGID_DBG_RECV_SIMUCPRIMSG;
		((CMT_TSKIF_HEAD*)msgp)->uiSignalkind = 0;
	}
	((CMT_TSKIF_HEAD*)msgp)->uiLength = msgsz;

	/************************************************************************/
	/* 他プロセスへメッセージを送信する										*/
	/************************************************************************/
	endcd = BPF_RU_IPCM_MSGQ_SEND(
							msgQid,			/* 直近の送信先QueueID( Thread Queue ID or Process Queue ID )		*/
							(void *)msgp	/* 送信メッセージ(BPF_RU_IPCM_PROCMSG_ADDRGET)で取得したアドレス    */
							);

	return;
}

/**************************************************************************************************/
/*!
 *  @brief  compare time
 *  @note   compare time
 *  @param  t1   [in]   time
 *  @param  t2   [in]   time that compare to t1
 *  @return compare result(1: t1 after t2, 0: t1 equal with t2, -1: t1 before t2)
 *  @date   2013/11/28 FFCS)hongj create for zynq
 */
/**************************************************************************************************/
INT l2_com_timeCompare(T_RRH_SYSTEM_TIME t1, T_RRH_SYSTEM_TIME t2 )
{
	if(t1.year > t2.year)
	{
	    return D_L2_COM_AFTER;
	}
	else if(t1.year < t2.year)
	{
	    return D_L2_COM_BEFORE;
	}

	if(t1.month> t2.month)
	{
	    return D_L2_COM_AFTER;
	}
	else if(t1.month < t2.month)
	{
	    return D_L2_COM_BEFORE;
	}

	if(t1.day> t2.day)
	{
	    return D_L2_COM_AFTER;
	}
	else if(t1.day < t2.day)
	{
	    return D_L2_COM_BEFORE;
	}

	if(t1.hour> t2.hour)
	{
	    return D_L2_COM_AFTER;
	}
	else if(t1.hour < t2.hour)
	{
	    return D_L2_COM_BEFORE;
	}

	if(t1.min> t2.min)
	{
	    return D_L2_COM_AFTER;
	}
	else if(t1.min < t2.min)
	{
	    return D_L2_COM_BEFORE;
	}

	if(t1.sec> t2.sec)
	{
	    return D_L2_COM_AFTER;
	}
	else if(t1.sec < t2.sec)
	{
	    return D_L2_COM_BEFORE;
	}

	if(t1.msec> t2.msec)
	{
	    return D_L2_COM_AFTER;
	}
	else if(t1.msec < t2.msec)
	{
	    return D_L2_COM_BEFORE;
	}

	if(t1.ucount> t2.ucount)
	{
	    return D_L2_COM_AFTER;
	}
	else if(t1.ucount < t2.ucount)
	{
	    return D_L2_COM_BEFORE;
	}

	return D_L2_COM_EQUAL;
}
/** @} */

