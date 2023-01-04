/*!
 * @skip  $ld:$
 * @file  f_ful_main.c
 * @brief FULタスクメイン処理
 * @date  2019/01/28 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */
#include <sys/prctl.h>
#include "f_ful_inc.h"

/*!
 * @brief 関数機能概要:FULタスクメイン。タスク起動からMSG受信待ちのLoop処理を実施
 * @note  関数処理内容.
 *       -# FULタスク状態を"初期状態"に設定する
 *       -# 初期化関数をCallする(f_ful_init())
 *       -# MSG待ち無限Loop処理へ移行する
 *          -# MSG受信関数をCallする(f_com_msgQReceive())
 *          -# 受信メッセージ解析関数をCallする(f_ful_anlz())
 * @return なし
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/01/28 KCN)上本 Create
 */
VOID f_ful_main()
{
	VOID			*rcvMsg = NULL ;					/* 受信メッセージ				*/
	UINT			msgSize = 0 ;
	
	/* スレッド名表示 */
	prctl( PR_SET_NAME, "pf_ful", 0, 0, 0 ) ;

	/* BPFへスレッド情報登録 */
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_PF,					/* ProcessID		*/
									D_SYS_THDID_PF_FUL,					/* ThreadID			*/
									D_SYS_THDQID_PF_FUL,				/* THreadQueueID	*/
									(unsigned long)pthread_self());		/* pThreadID		*/

	/* 初期化処理Call */
	(VOID)f_ful_init( NULL ) ;

	/************************************************************************/
	/* メッセージ受信待ち													*/
	/************************************************************************/
	while(1)
	{
		/********************************************************************/
		/* メッセージ受信待ち												*/
		/********************************************************************/
		BPF_RU_IPCM_MSGQ_RECV(	D_SYS_THDQID_PF_FUL,					/* 受信MSGキューID			*/
								0,										/* 応答待ち時間				*/
								(VOID**)&rcvMsg,						/* メッセージ受信バッファ	*/
								&msgSize ) ;							/* 最大受信バッファサイズ	*/
		
		/********************************************************************/
		/* 受信メッセージ解析関数Call										*/
		/********************************************************************/
		(VOID)f_ful_anlz( (VOID*)rcvMsg ) ;
		
		/* 受信バッファを解放する */
		BPF_RU_IPCM_PROCMSG_ADDRFREE((VOID*)rcvMsg);
	}
	
	return ;
}

/* @} */
