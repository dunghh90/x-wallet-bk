/*!
 * @skip		$ld:$
 * @file		f_main_main.c
 * @brief		pf_main メイン処理
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include "f_main_inc.h"

/*!
 * @brief		main
 * @note		PFプロセスのmain関数.
 *					-# [初期化状態]へ状態遷移
 *					-# 初期化処理								f_main_init関数コール
 *					-# MSG待ち無限Loop処理へ移行				BPF_RU_IPCM_MSGQ_RECV関数コール
 *					-# MSG受信時は解析処理を実施。				f_main_anlz関数コール
 * @param		-
 * @return		N/A
 * @warning		N/A
 * @FeatureID	PF-MAIN-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 */
INT main()
{
	UINT					rcvSize = 0;
	T_MAIN_RCV_MSG_MNG*		rcvMsg  = NULL;

	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "pf_main", 0, 0, 0);

	/* タスク状態に初期状態を設定	*/
	f_mainw_thdmng_state = D_MAIN_THD_STS_INIT;

	/* 初期化タスク内初期化処理		*/
	(VOID)f_main_init(NULL);

	/********************************************************************************/
	/* MSG受信処理の無限Loop														*/
	/********************************************************************************/
	for(;;)
	{
		/****************************************************************************/
		/* メッセージ受信															*/
		/****************************************************************************/
		BPF_RU_IPCM_MSGQ_RECV(
					D_SYS_THDQID_PF_F_MAIN,					/* 受信MSGキューID			*/
					0,										/* 受信待ち時間				*/
					(void**)&rcvMsg,						/* メッセージ受信バッファ	*/
					&rcvSize	);							/* 受信Messageサイズ		*/

		/*	受信MSG解析処理		*/
		(VOID)f_main_anlz((VOID*)rcvMsg);
		
		/* 受信バッファを解放する */
		BPF_RU_IPCM_PROCMSG_ADDRFREE((VOID*)rcvMsg);
	}

	return 0;
}

/* @} */
