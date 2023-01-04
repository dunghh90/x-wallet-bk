/*!
 *  @skip   $Id$
 *  @file   main.c
 *  @brief  the entry of antpi server
 *  @date   2013/04/01 FFCS)niemsh Create\n
 *  @date   2013/11/14 FFCS)niemsh modify for zynq\n
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
*/

 /*!
 * @addtogroup RRH_APL
 * @{
 */

#include "f_rrh_inc.h"

#include "BPF_RM_SVRM.h"
#include "BPF_RU_IPCM.h"
#include "BPF_RU_ITCM.h"
#include "f_com_aplCom.h"
/*!
 *  @brief  the proccess of hunter server
 *  @note   This function is processed as follows. \n
 *          - Process manage function assign  \n
 *          - schedule setting \n
 *          - Init related table \n
 *          - load share memory \n
 *          - startup	thread	 \n
 *          - receve init completion from all thread\n
 *          - perfrom debug function\n
 *  @return VOID
 *  @retval none
 *  @date   2013/04/01 FFCS)niemsh Create\n
 */
typedef struct
{
	unsigned long int thrdId;
	UINT thrdHdl;
	UINT initFlg;
}T_THRDINIT_MNG;


INT apl_main_main(UINT procID,UINT mainQ,const T_SYS_THRDINFO *thrInfo,UINT thdMax)
{
	printf("\n C9 debug file %s ,line %d ,thdMax = %d\n",__FILE__,__LINE__,thdMax); //C9 add code
	INT sit_ret;								/**/
	INT errcd;
	INT kind;
#ifndef OPT_RRH_IT1
	struct sched_param sch_prm;					/* */
#endif
	char rcvBuf[256];
	INT msgLen;
	INT		pid ;
	UINT	num;
	UINT	pri;

	T_RRH_HEAD			*ptHeader;
	T_RRH_PID_MNG		*procIdTbl;
	T_THRDINIT_MNG	thrdInit[32];

	/******************************/
	/*	Process Descriptor setting */
	/******************************/
	memset(thrdInit,0,sizeof(thrdInit));

#ifndef OPT_RRH_IT1
	sit_ret = BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_PROC_ID_MNG_INFO,
		(VOID **)&procIdTbl,
		&errcd
	);
	if( sit_ret != BPF_RU_IPCM_OK )
	{
		apl_com_alm(0,"sharememory NG");
	}
	pid = getpid();
	for(num = 0; num < procIdTbl->count;num++)
	{
		if(procIdTbl->info[num].aplProcessID == procID)
		{
			procIdTbl->info[num].realProcessID = pid;
			procIdTbl->info[num].PQID = mainQ;
			break;
		}
	}
	if(num >= procIdTbl->count)
	{
		procIdTbl->info[ procIdTbl->count ].aplProcessID = procID;
		procIdTbl->info[ procIdTbl->count ].realProcessID = pid;
		procIdTbl->info[ procIdTbl->count ].PQID = mainQ;
		procIdTbl->count++;
	}
#endif

	for(num = 0;num < thdMax;num++)
	{
		thrdInit[num+1].thrdId = thrInfo[num+1].thrdId;
	}

#ifndef OPT_RRH_IT1
	/*schedule setting */
	sch_prm.sched_priority
		= thrInfo[0/*main*/].thrPrio;

	sit_ret = sched_setscheduler(0,
				BPF_RU_ITCM_SCHED_FIFO,
				&sch_prm);
#endif
	/*NG case*/
	if (sit_ret != BPF_RM_SVRM_COMPLETE)
	{
		return D_RRH_NG;
	}

	/***********************************/
	/*	startup	thread					*/
	/***********************************/
	for(num = 0; num < thdMax;num++)
	{
		if(thrInfo[num+1].schedule != D_RRH_ON) {
            kind = BPF_RU_ITCM_SCHED_OTHER;
            pri = 0;
		}
		else
	{
		kind = BPF_RU_ITCM_SCHED_FIFO;
			pri = thrInfo[num].thrPrio;
		}
//		kind = BPF_RU_ITCM_SCHED_FIFO;
//		if(thrInfo[num+1].schedule != D_RRH_ON)
//		kind = BPF_RU_ITCM_SCHED_OTHER;

//擬似環境
		printf("[BPF_RU_ITCM_THREAD_CREATE] kind = %d,thrInfo[num+1].thrPrio=%d ,thrInfo[num+1].thrStack =%d ,(char *)thrInfo[num+1].thrdName=%s\n"
		,kind
		,pri
		,thrInfo[num+1].thrStack
		,(char *)thrInfo[num+1].thrdName
		);
//擬似環境

		sit_ret = BPF_RU_ITCM_THREAD_CREATE(
			(unsigned long int *)&thrdInit[num+1].thrdHdl,
			kind,
//			thrInfo[num+1].thrPrio,
			pri,
			thrInfo[num+1].thrStack,	/* stack size */
			thrInfo[num+1].thrFunc,
			(void *)thrInfo[num+1].thrdName,
			&errcd);
		/*NG case*/
		printf("\n C9 debug file %s ,line %d,thrInfo[num+1].thrPrio=%d ,thrInfo[num+1].thrStack =%d ,(char *)thrInfo[num+1].thrdName=%s sit_ret = %d\n",__FILE__,__LINE__,pri,thrInfo[num+1].thrStack,(char *)thrInfo[num+1].thrdName,sit_ret);
		if (sit_ret != BPF_RM_SVRM_COMPLETE)
		{
			return D_RRH_NG;
		}
	}

	/***********************************/
	/*	loop thread startup info		*/
	/***********************************/
	thrdInit[0].initFlg = D_RRH_ON;
	ptHeader = (T_RRH_HEAD*)rcvBuf;
	for(;;)
	{
		sit_ret = apl_com_msgQReceive(
							thrInfo[0].thrdId,	/*!< [in]Que no							*/
							sizeof(rcvBuf),		/*!< [in]Recieve Buffer Size			*/
							rcvBuf,				/*!< [out]Pointer of recieve buffer		*/
							&msgLen,			/*!< [out]Reciept Message Size			*/
							&errcd);

		/*loop thrdInit save init flag*/
		for(num = 0;num < thdMax;num++)
		{

			if(thrdInit[num+1].thrdId == ptHeader->uiSrcTQueueID)
			{
			printf("\n C9 debug file %s ,line %d,thrdInit[num+1].thrdId =%d ,ptHeader->uiSrcTQueueID = %d, num =%d\n",__FILE__,__LINE__,thrdInit[num+1].thrdId,ptHeader->uiSrcTQueueID,num);
				thrdInit[num+1].initFlg = D_RRH_ON;
				break;
			}
		}

		/*loop thrdInit to check all thread status*/
		for(num = 0;num < thdMax;num++)
		{
			if(thrdInit[num+1].initFlg != D_RRH_ON)
			break;
		}
		if(num >= thdMax)
		{
			printf("\n C9 debug file %s ,line %d ,thdMax = %d\n",__FILE__,__LINE__,thdMax);
			break;

		}
	}
printf("\n C9 debug file %s ,line %d ,thdMax = %d\n",__FILE__,__LINE__,thdMax);
	return D_RRH_OK;
}

/*!
 *  @brief  the proccess of alarm report for apl
 *  @note   This function is processed as follows. \n
 *          - direct write access\n
 *  @return VOID
 *  @retval none
 *  @date   2013/04/01 FFCS)niemsh Create\n
 */


void apl_com_alm(int code,CHAR *errDesc)
{

	exit(-1);
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

INT apl_com_msgQReceive(UINT msgQ,INT msgSize,VOID*rcvMsg,INT*rcvLen,INT*errcd)
{
	VOID	*shm_msgp;
	int		ret;

	shm_msgp = 0;
	ret = BPF_RU_IPCM_MSGQ_RECV(
						msgQ,				/* 自分のQueueID								*/
						0,					/* 応答待ち時間(Process間Message受信時のみ有効)	*/
						(void **)&shm_msgp,	/* 通信用共有メモリのポインタ					*/
						(UINT*)rcvLen );			/* Receiveしたメッセージのサイズ*/

	if(ret != BPF_RU_IPCM_OK)
		return D_RRH_NG;

	if(((T_RRH_HEAD*)shm_msgp)->uiLength > msgSize)
		memcpy(rcvMsg,shm_msgp,msgSize);
	else
		memcpy(rcvMsg,shm_msgp,((T_RRH_HEAD*)shm_msgp)->uiLength);

	ret = BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)shm_msgp );				/* 共有メモリのアドレス	*/

	return D_RRH_OK;
}

/*!
 *  @brief  the proccess of sending message for apl
 *  @note   This function is processed as follows. \n
 *          - call BPF_RU_IPCM_PROCMSG_ADDRGET to hunter share memor buffer\n
 *          - copy message to share memory buffer\n
 *          - call BPF_RU_IPCM_MSGQ_SEND\n
 *  @return VOID
 *  @retval none
 *  @date   2013/12/01 FFCS)niemsh Create\n
 */

INT apl_com_msgQSend(UINT msgQid,UINT msgid,void *msgp,UINT msgsz )
{
		/************************************************************************/
	/* Local parameter declaration											*/
	/************************************************************************/
	INT		endcd  __attribute__((unused));						/*!< 終了コード						*/
	INT		errcd;
	VOID	*shm_msgp;
	UINT	keyID;

	endcd = BPF_RU_IPCM_PROCMSG_ADDRGET(
										E_BPF_RU_IPCM_BUFF_KIND_TASK,					/*!< Bufferç¨®åˆ¥					*/
										msgsz,					/*!< size						*/
										(void **)&shm_msgp,		/*!< é€šä¿¡ç”¨å…±æœ‰ãƒ¡ãƒ¢ãƒªã®ãƒã‚¤ãƒ³ã‚?*/
										&errcd);				/*!< ã‚¨ãƒ©ãƒ¼ã‚³ãƒ¼ãƒ‰				*/
	if(endcd != BPF_RU_IPCM_OK)
	{
		/*access log*/
		return D_RRH_NG;
	}

	/*temp save key*/
	keyID = ((T_RRH_HEAD*)shm_msgp)->uiKeyID;

	memcpy(shm_msgp,msgp,msgsz);
	((T_RRH_HEAD*)shm_msgp)->uiKeyID = keyID;
	((T_RRH_HEAD*)shm_msgp)->uiLength = msgsz;

	/************************************************************************/
	/* 他プロセスへメッセージを送信する										*/
	/************************************************************************/
	endcd = BPF_RU_IPCM_MSGQ_SEND(
							msgQid,			/* 直近の送信先QueueID( Thread Queue ID or Process Queue ID )		*/
							(void *)shm_msgp	/* 送信メッセージ(BPF_RU_IPCM_PROCMSG_ADDRGET)で取得したアドレス	*/
							);

	return D_RRH_OK;
}

/* @} */

