/*!
 * @skip  $ld:$
 * @file  trd_proc_com.c
 * @brief the common function of thread
 *  @date  2013/11/27 FFCS)hongj create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */



/** @addtogroup RRH_PF_COM
 *  @{
 */
#include "f_rrh_inc.h"
#include "f_sys_inc.h"

#include "BPF_RU_IPCM.h"
#include "f_com_inc.h"

/*!
 *  @brief  The fucntion of sending message to APL 
 *  @note   This function is processed as follows.\n
 *          - Call MW function to send message to dest process
 *  @return VOID
 *  @retval none
 *  @date  2013/11/27 FFCS)hongj create\n
 * @date 2021/01/19 M&C) Merge 4G FHM src
 */

VOID f_com_sendMsgToAplFHM(UINT msgQid,void *msgp,UINT msgsz,UINT msgId,UINT thrdQid )
{
	/************************************************************************/
	/* Local parameter declaration											*/
	/************************************************************************/
	INT		endcd;						/*!< 終了コー繝?					*/
	VOID	*shm_msgp;
	UINT	keyID;
	INT		ret __attribute__((unused));
	
	if((msgp == D_RRH_NULL) || (msgsz == 0))
	{
		f_com_assert(msgId,"invalid dest proces Qid");
		return ;
	}

	if(((T_SYS_COMMON_THDIF*)msgp)->uiEventNo != msgId)
	{
		f_com_assert(msgId,"invalid event ID");
	}

	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(
										E_BPF_RU_IPCM_BUFF_KIND_TASK,					/*!< Buffer種別					*/
										msgsz,					/*!< size						*/
										(void **)&shm_msgp,		/*!< 通信用共有メモリのポイン繧?*/
										&endcd);				/*!< エラーコード				*/
	if(shm_msgp == D_RRH_NULL)
	{
		/*access log*/
		f_com_assert(msgId,"fail to BPF_RU_IPCM_PROCMSG_ADDRGET");
		return ;
	}

	/*temp save key*/
	keyID = ((T_SYS_COMMON_THDIF*)shm_msgp)->uiKeyID;
	
	memcpy(shm_msgp,msgp,msgsz);
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiSrcPQueueID = D_RRH_PROCQUE_F_PF;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiSrcTQueueID = thrdQid;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiDstPQueueID = msgQid;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiDstTQueueID = 0;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiKeyID = keyID;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiLength = msgsz;

	/************************************************************************/
	/* 他プロセスへメッセージを送信する										*/
	/************************************************************************/
	ret = BPF_RU_IPCM_MSGQ_SEND(
							msgQid,			/* 直近の送信先QueueID( Thread Queue ID or Process Queue ID )		*/
							(void *)shm_msgp	/* 送信メッセー繧?BPF_RU_IPCM_PROCMSG_ADDRGET)で取得したアドレ繧?*/
							);

	if(ret != BPF_RU_IPCM_OK)
	{
		f_com_assert(ret,"fail to BPF_RU_IPCM_MSGQ_SEND");
		BPF_RU_IPCM_PROCMSG_ADDRFREE(shm_msgp);
	}

	return;
}

/*!
 *  @brief  The fucntion of sending message to APL 
 *  @note   This function is processed as follows.\n
 *          - Call MW function to send message to dest process
 *  @return VOID
 *  @retval none
 *  @date  2013/11/27 FFCS)hongj create\n
 */
VOID f_com_sendMsgToApl(UINT msgQid,void *msgp,UINT msgsz,UINT msgId,UINT thrdQid )
{
	/************************************************************************/
	/* Local parameter declaration											*/
	/************************************************************************/
	INT		endcd;						/*!< 終了コー繝?					*/
	VOID	*shm_msgp;
	UINT	keyID;
	INT		ret __attribute__((unused));
	
	if((msgp == D_RRH_NULL) || (msgsz == 0))
	{
		f_com_assert(msgId,"invalid dest proces Qid");
		return ;
	}

	if(((T_SYS_COMMON_THDIF*)msgp)->uiEventNo != msgId)
	{
		f_com_assert(msgId,"invalid event ID");
	}

	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(
										E_BPF_RU_IPCM_BUFF_KIND_TASK,					/*!< Buffer種別					*/
										msgsz,					/*!< size						*/
										(void **)&shm_msgp,		/*!< 通信用共有メモリのポイン繧?*/
										&endcd);				/*!< エラーコード				*/
	if(ret != BPF_RU_IPCM_OK)
	{
		/*access log*/
		f_com_assert(msgId,"fail to BPF_RU_IPCM_PROCMSG_ADDRGET");
		return ;
	}

	/*temp save key*/
	keyID = ((T_SYS_COMMON_THDIF*)shm_msgp)->uiKeyID;
	
	memcpy(shm_msgp,msgp,msgsz);
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiSrcPQueueID = D_RRH_PROCQUE_PF;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiSrcTQueueID = thrdQid;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiDstPQueueID = msgQid;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiDstTQueueID = 0;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiKeyID = keyID;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiLength = msgsz;

	/************************************************************************/
	/* 他プロセスへメッセージを送信する										*/
	/************************************************************************/
	ret = BPF_RU_IPCM_MSGQ_SEND(
							msgQid,			/* 直近の送信先QueueID( Thread Queue ID or Process Queue ID )		*/
							(void *)shm_msgp	/* 送信メッセー繧?BPF_RU_IPCM_PROCMSG_ADDRGET)で取得したアドレ繧?*/
							);

	return;
}

/*!
 *  @brief  The fucntion of sending message between PF threads 
 *  @note   This function is processed as follows.\n
 *          - Call MW function to send message to dest thread
 *  @return VOID
 *  @retval none
 *  @date  2013/11/27 FFCS)hongj create\n
 */

INT f_com_msgQSendFHM(UINT msgQid,VOID *msgp,UINT msgsz,UINT thrdQId,UINT state,UINT eventId )
{
	/************************************************************************/
	/* Local parameter declaration											*/
	/************************************************************************/
	INT		errcd;						/*!< 終了コー繝?					*/
	INT		endcd;						/*!< 終了コー繝?					*/
	VOID	*shm_msgp;
	UINT	keyID;
	
	endcd = BPF_RU_IPCM_PROCMSG_ADDRGET(
										E_BPF_RU_IPCM_BUFF_KIND_TASK,					/*!< Buffer種別					*/
										msgsz,					/*!< size						*/
										(void **)&shm_msgp,		/*!< 通信用共有メモリのポイン繧?*/
										&errcd);				/*!< エラーコード				*/
	if(shm_msgp == D_RRH_NULL)
	{
		/*access log*/
		f_com_assert(msgQid,"fail to BPF_RU_IPCM_PROCMSG_ADDRGET");
		return D_RRH_NG;
	}

	/*temp save key*/
	keyID = ((T_SYS_COMMON_THDIF*)shm_msgp)->uiKeyID;
	
	memcpy(shm_msgp,msgp,msgsz);
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiDstTQueueID = msgQid;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiSrcTQueueID = thrdQId;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiDstPQueueID = D_RRH_PROCQUE_F_PF;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiSrcPQueueID = D_RRH_PROCQUE_F_PF;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiKeyID = keyID;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiLength = msgsz;

	/************************************************************************/
	/* 他プロセスへメッセージを送信する										*/
	/************************************************************************/
	endcd = BPF_RU_IPCM_MSGQ_SEND(
							msgQid,			/* 直近の送信先QueueID( Thread Queue ID or Process Queue ID )		*/
							(void *)shm_msgp	/* 送信メッセー繧?BPF_RU_IPCM_PROCMSG_ADDRGET)で取得したアドレ繧?*/
							);

	if(endcd != BPF_RU_IPCM_OK)
	{
		f_com_assert(endcd,"fail to BPF_RU_IPCM_MSGQ_SEND");
		BPF_RU_IPCM_PROCMSG_ADDRFREE(shm_msgp);

		return D_RRH_NG;
	}

	return D_RRH_OK;
}

INT f_com_msgQSend(UINT msgQid,VOID *msgp,UINT msgsz,UINT thrdQId,UINT state,UINT eventId )
{
	/************************************************************************/
	/* Local parameter declaration											*/
	/************************************************************************/
	INT		errcd;						/*!< 終了コー繝?					*/
	INT		endcd;						/*!< 終了コー繝?					*/
	VOID	*shm_msgp;
	UINT	keyID;
	
	endcd = BPF_RU_IPCM_PROCMSG_ADDRGET(
										E_BPF_RU_IPCM_BUFF_KIND_TASK,					/*!< Buffer種別					*/
										msgsz,					/*!< size						*/
										(void **)&shm_msgp,		/*!< 通信用共有メモリのポイン繧?*/
										&errcd);				/*!< エラーコード				*/
	if(endcd != BPF_RU_IPCM_OK)
	{
		/*access log*/
		f_com_assert(msgQid,"fail to BPF_RU_IPCM_PROCMSG_ADDRGET");
		return D_RRH_NG;
	}

	/*temp save key*/
	keyID = ((T_SYS_COMMON_THDIF*)shm_msgp)->uiKeyID;
	
	memcpy(shm_msgp,msgp,msgsz);
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiDstTQueueID = msgQid;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiSrcTQueueID = thrdQId;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiDstPQueueID = D_RRH_PROCQUE_PF;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiSrcPQueueID = D_RRH_PROCQUE_PF;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiKeyID = keyID;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiLength = msgsz;

	/************************************************************************/
	/* 他プロセスへメッセージを送信する										*/
	/************************************************************************/
	endcd = BPF_RU_IPCM_MSGQ_SEND(
							msgQid,			/* 直近の送信先QueueID( Thread Queue ID or Process Queue ID )		*/
							(void *)shm_msgp	/* 送信メッセー繧?BPF_RU_IPCM_PROCMSG_ADDRGET)で取得したアドレ繧?*/
							);

	if(endcd != BPF_RU_IPCM_OK)
		return D_RRH_NG;

	return D_RRH_OK;
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
INT f_com_msgQReceive(UINT msgQ,INT msgSize,VOID*rcvMsg,INT*rcvmsgsz,INT*errcd)
{
	VOID	*shm_msgp;
	INT		endcd;
	INT		ret;

	ret = BPF_RU_IPCM_MSGQ_RECV(	msgQ,				/* ﾌQueueID								*/
									0,					/* ҂ﾔ(ProcessﾔMessageM̂ݗL・	*/
									(void **)&shm_msgp,	/* ʐMpL̃|C^					*/
									(UINT*)&endcd );			/* ReceivebZ[W̃TCY	*/

	if(ret != BPF_RU_IPCM_OK)
		return D_RRH_NG;

	if(((T_RRH_HEAD*)shm_msgp)->uiLength > msgSize)
		memcpy(rcvMsg,shm_msgp,msgSize);
	else
		memcpy(rcvMsg,shm_msgp,((T_RRH_HEAD*)shm_msgp)->uiLength);

	ret = BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)shm_msgp );				/* 共有メモリのアドレス	*/

	return D_RRH_OK;
}

/* @} */
