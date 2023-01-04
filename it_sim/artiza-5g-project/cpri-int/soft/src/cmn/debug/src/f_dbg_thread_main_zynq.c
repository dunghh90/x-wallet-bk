/*!
 * @skip   $Id$
 * @file   f_cmu_dbg_main_zynq.c
 * @brief  デバッグスレッドメイン処理
 * @date   2013/11/22 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */

#include 	"f_dbg_thread_inc.h"
extern UINT f_dbg_thread_init( T_DBGTHREAD_EXEC_INFO * ) ;
extern UINT f_dbg_thread_anlz( T_DBGTHREAD_EXEC_INFO *, VOID *);


/*!
 * @brief 関数機能概要:デバッグスレッド　メイン処理
 * @note  関数処理内容.
 *       -# スレッド起動情報を基にスレッド名等を保存する
 *       -# スレッド名表示.
 *       -# 初期化処理関数コール(f_dbg_thread_init()).
 *       -# MSG待ち無限Loop処理へ移行関数コール(BPF_RU_IPCM_MSGQ_RECV()).
 *          -# MSG受信時は解析処理を実施関数コール(f_dbg_thread_anlz()).
 *          -# 受信メッセージバッファを解放する(BPF_RU_IPCM_PROCMSG_ADDRFREE)
 * @param  execParam	[in]T_DBGTHREAD_EXEC_INFO*         デバッグスレッド起動情報
 * @return INT
 * @date 2013/11/25 ALPHA)宮崎 create
 */

void f_dbg_main( T_DBGTHREAD_EXEC_INFO  *execParam )	{

	UINT	myProcID			;
	UINT	myThreadID			;
	UINT	myThreadQID			;
	char	myThreadName[10]	;
	int		recvMsgSize		 = 0 ;
	char	*recvMsg		 = NULL ;
	int		ret;
	sigset_t sigSetVal;
	printf("\n C9 debug file %s ,line %d\n",__FILE__,__LINE__); //C9 add code
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, "debug thread start." ) ;
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER,  "debug thread startup info.<pid:%08x pqid:%08x thnam:[%s] thid:%08x thqid:%08x>",
			execParam->uiProcID,
			execParam->uiProcQID,
			execParam->cpDbgThreadName,
			execParam->uiDbgThreadID,
			execParam->uiDbgThreadQID ) ;

	/* 起動時情報保存	*/
	myProcID 	= execParam->uiProcID					;	/* プロセスIDの保存		*/
	myThreadID 	= execParam->uiDbgThreadID				;	/* スレッドIDの保存		*/
	myThreadQID	= execParam->uiDbgThreadQID				;	/* スレッドQIDの保存	*/
	strncpy(myThreadName, execParam->cpDbgThreadName, sizeof(myThreadName) );

	/* スレッド名設定	*/
	prctl( PR_SET_NAME, myThreadName, 0, 0, 0);

	/* スレッド情報設定 */
	BPF_RM_SVRM_THREAD_INFO_SET(myProcID,			/* ProcessID		*/
								myThreadID,			/* ThreadID			*/
								myThreadQID,		/* ThreadQueueID	*/
								(unsigned long int)pthread_self());

	/* デバッグスレッド初期化処理	*/
	(VOID)f_dbg_thread_init(execParam);


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
		/*	MSG受信待ち			*/
		printf("\ninit f_dbg_main fhm before  ");//C9 add code
		ret = BPF_RU_IPCM_MSGQ_RECV(myThreadQID,							/* プロセスQID(スレッドQID)		*/
									0,										/* 応答待ち時間					*/
									(VOID **)&recvMsg,						/* 受信MSGポインタ				*/
									(unsigned int*)&recvMsgSize				/* 受信MSGサイズ				*/
		);
		printf("\ninit f_dbg_main fhm after rev EvtNo:%08X",((T_RRH_HEAD*)recvMsg)->uiEventNo);//C9 add code
		printf("\ninit f_dbg_main recv");//C9 add code
		if(BPF_RU_IPCM_OK == ret){
			/*	受信MSG解析処理		*/
			(VOID)f_dbg_thread_anlz( execParam, (VOID*)recvMsg );

			/* 受信メッセージバッファ解放	*/
			(VOID)BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)recvMsg );
		}
	}

}
/* @} */

