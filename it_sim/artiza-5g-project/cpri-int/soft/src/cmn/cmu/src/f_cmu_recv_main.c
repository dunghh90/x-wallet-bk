/*!
 * @skip		$ld:$
 * @file		f_cmu_recv_main.c
 * @brief		cmu メイン処理
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_CMU
 * @{
 */

#include "f_cmn_cmu_inc.h"

/*!
 * @brief		f_recv_main
 * @note		f_recv_main関数.
 *					-# スレッド名表示.
 *					-# スレッド情報登録関数コール(BPF_RM_SVRM_THREAD_INFO_SET()).
 *					-# [初期化状態]へ状態遷移.
 *					-# 初期化処理関数コール(f_recv_init()).
 *					-# MSG待ち無限Loop処理へ移行関数コール(BPF_RU_IPCM_MSGQ_RECV()).
 *					-# MSG受信時は解析処理を実施関数コール(f_recv_anlz()).
 * @param		tRcvMsg					[in]	受信メッセージ
 * @return		N/A
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 */

VOID f_recv_main(T_CMU_CREATE_THREAD *tRcvMsg)
{
	UINT	uiSize;
	VOID*	rcvMsg_p;
	INT		ret;
	sigset_t sigSetVal;

	/* スレッド名表示	*/
	prctl(PR_SET_NAME, tRcvMsg->uiThreadName, 0, 0, 0);
	
	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	tRcvMsg->uiProcID,				/* ProcessID		*/
									tRcvMsg->uiThreadID,				/* ThreadID			*/
									tRcvMsg->uiThreadQID,			/* ThreadQueueID	*/
									(unsigned long int)pthread_self());

	/* assert log */
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, 
						(const char*)"ThreadName:%s, ProcessID :0x%08lX, ProcessQID:0x%08lX, ThreadID:0x%08lX, ThreadQID:0x%08lX.",
						tRcvMsg->uiThreadName,
						tRcvMsg->uiProcID,
						tRcvMsg->uiProcQID,
						tRcvMsg->uiThreadID,
						tRcvMsg->uiThreadQID);
	
	/* 初期状態を設定	*/
	f_recvw_thdState = D_CMU_THD_STS_INIT;
	
	
	/* 初期化処理		*/
	(VOID)f_cmu_com_init(tRcvMsg, &f_recvw_route, &f_recvw_ProcTbl, &f_recvw_thdState);
	printf("\ninit f_recv_main init success");//C9 add code

	/* PTP用のシグナルが大量発生するため、本スレッドではBLOCKする  */
	sigemptyset( &sigSetVal );
	(void)sigaddset( &sigSetVal, SIGRTMAX - 1  );	/* ZLTICKTIMERSIG */
	(void)sigaddset( &sigSetVal, SIGRTMAX - 2  );	/* ZLAPRTIMERSIG */
	(void)sigaddset( &sigSetVal, SIGRTMAX - 3  );	/* ZLWDTIMERSIG */
	(void)sigaddset( &sigSetVal, SIGRTMAX - 10  );	/* ZLCSTTIMERSIG */
	(void)pthread_sigmask(SIG_BLOCK, &sigSetVal, NULL);

	/********************************************************************************/
	/* MSG受信処理の無限Loop														*/
	/********************************************************************************/
	for(;;)
	{
		/*プロセス間MSG受信待ち			*/
		ret = BPF_RU_IPCM_MSGQ_RECV(	tRcvMsg->uiProcQID,			/* プロセスQID					*/
										0,							/* 応答待ち時間					*/
										(void**)&rcvMsg_p,			/* 受信MSGポインタ				*/
										&uiSize						/* 受信MSGサイズ				*/
		);

		if(BPF_RU_IPCM_OK == ret){
			/*	受信MSG解析処理		*/
			(VOID)f_recv_anlz((VOID*)rcvMsg_p);
		}
	}	
	
}

/* @} */
