/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_aih_main.c
 *  @brief  5GDU-LLS ALD(AISG) interface main function
 *  @date   2019/07/31 FJT)Takahashi create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019-
 */
/*********************************************************************************/

/**  
* @mainpage		the thread transmit the message of ALD(AISG) for 5GDU-LLS  (pf_aih) 
* @section		Introdution		   
* 				-# receive notice of MSI and send notice of ALD(AISG).
*   			-# receive reply of ALD(AISG) and send reply of MSI.
*				-# receive notice of MSI and control(on/off) ALD device.
*
*/


/**  
* @defgroup RRH_PF_AIH    5GDU-LLS supervision and control external IO Thread 
* @ingroup RRH   
* This class is 5GDU-LLS supervision and control external IO.
*/

/** @addtogroup RRH_PF_AIH
* @{ */

#include "f_aih_inc.h"			/* 5GDU-LLS AIHタスクヘッダファイル */

/* @{ */
/**
* @brief 5GDU-LLS external IO main function
* @note  external IO main function.\n
* @param None
* @return None
* @date 2019/03/18 FJT)Koshida create
* @warning	N/A
* @FeatureID	5GDU-002-007
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_aih_main()
{
	VOID			*pMsg;
	UINT			rcvSize;
	INT		errcd;

	prctl(PR_SET_NAME, "pf_aih", 0, 0, 0);
	
	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_PF,				/* ProcessID		*/
									D_SYS_THDID_PF_AIH,				/* ThreadID			*/
									D_SYS_THDQID_PF_AIH ,			/* ThreadQueueID	*/
									(unsigned long int)pthread_self()) ;			/* Rhread ID  */

	/* 初期化処理を実施 */
	f_aih_init();
	
	/***********************************************************/
	/*	Message Receive and Function dispatched 			   */
	/***********************************************************/	 
	for(;;)
	{
		/************************************************************************************************************/
		/* メッセージ受信																							*/
		/************************************************************************************************************/
		errcd = BPF_RU_IPCM_MSGQ_RECV(
					D_SYS_THDQID_PF_AIH,												/* 受信MSGキューID			*/
					0,																	/* 受信待ち時間				*/
					(VOID**)&pMsg,														/* メッセージ受信バッファ	*/
					&rcvSize	);														/* 受信Messageサイズ		*/
		if( errcd != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RECIEVE NG 0x%p(0x%08x)", pMsg, errcd );
			break;
		}

		/* analyze and dispatch message   */
		f_aih_anlz( pMsg ); 

		/* 受信バッファ解放	*/
		errcd = BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)pMsg );
		if( errcd != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FREE NG 0x%p(0x%08x)", pMsg, errcd );
			break;
		}
		
	}
	return ;
}

/* @} */

