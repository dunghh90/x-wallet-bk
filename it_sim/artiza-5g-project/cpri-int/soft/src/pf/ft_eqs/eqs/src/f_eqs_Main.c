/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_eqs_Main.c
 *  @brief  5GDU-LLS supervision main function
 *  @date   2018/08/21 KCN)Kamimoto create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018-
 */
/*********************************************************************************/

/**  
* @mainpage		5GDU-LLS supervision Task (pf_eqs) 
* @section		Introdution		   
*   				-# Handle health check message to maintain L3 link status
* 				-# Stop health check and release TRX when CPRI link disconnect happened.
*				-# Supervise self-card and external equipment status.
*
*
*
*/


/**  
* @defgroup RRH_PF_EQS    5GDU-LLS supervision Task 
* @ingroup RRH   
* This class is 5GDU-LLS supervision.
*/

/** @addtogroup RRH_PF_EQS
* @{ */

#include "f_eqs_header.h"			/* 5GDU-LLS監視タスクヘッダファイル			*/

/* @{ */
/**
* @brief 5GDU-LLS supervision main function
* @note RE supervision main function.\n
* @param None
* @return None
* @date 2018/08/21 KCN)Kamimoto create
* @warning	N/A
* @FeatureID	5GDU-002-004
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eqs_Main(  )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT	a_ret __attribute__((unused))		= D_SYS_OK;		/* 戻り値格納変数				*/
	VOID			*pMsg;
	UINT			rcvSize;
	INT		errcd;

	prctl(PR_SET_NAME, "pf_eqs", 0, 0, 0);
	
	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_PF,				/* ProcessID		*/
									D_SYS_THDID_PF_EQS,				/* ThreadID			*/
									D_SYS_THDQID_PF_EQS ,			/* ThreadQueueID	*/
									(unsigned long int)pthread_self()) ;			/* Rhread ID        */

	/* 初期化処理を実施 */
	f_eqs_InitReq();
	
	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	for(;;)
	{
		/************************************************************************************************************/
		/* メッセージ受信																							*/
		/************************************************************************************************************/
		errcd = BPF_RU_IPCM_MSGQ_RECV(
					D_SYS_THDQID_PF_EQS,												/* 受信MSGキューID			*/
					0,																	/* 受信待ち時間				*/
					(VOID**)&pMsg,														/* メッセージ受信バッファ	*/
					&rcvSize	);														/* 受信Messageサイズ		*/
		if( errcd != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "[f_eqs_main] RECIEVE" );
			break;
		}

		/* analyze and dispatch message   */
		f_eqs_anlz( pMsg ); 

		/* 受信バッファ解放	*/
		errcd = BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)pMsg );
		if( errcd != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "[f_eqs_main] FREE" );
			break;
		}
		
	}
	return ;
}

/* @} */

/* @} */
