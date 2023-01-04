/*!
 * @skip  $ld:$
 * @file  f_dpda_main.c
 * @brief DPDAタスクメイン処理
 * @date  2011/07/26 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2013
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */
#include <sys/prctl.h>
#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:DPDAタスクメイン。タスク起動からMSG受信待ちのLoop処理を実施
 * @note  関数処理内容.
 *       -# DPDAタスク状態を"初期状態"に設定する
 *       -# 初期化関数をCallする(f_dpda_init())
 *       -# MSG待ち無限Loop処理へ移行する
 *          -# MSG受信関数をCallする(f_com_msgQReceive())
 *          -# 受信メッセージ解析関数をCallする(f_dpda_anlz())
 * @return なし
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A	
 * @date 2011/07/26 ALPHA)横山 Create
 * @date 2013/04/12 ALPHA)中村 B25対応
 * @date 2013/11/26 ALPHA)加藤 ZYNQ対応
 */
VOID f_dpda_main_fhm()
{
	VOID							*rcvMsg	= NULL;					/* 送信メッセージ(初期化完了通知)				*/
	UINT 							msgSize = 0;
	INT								ret;
	INT								errcd;
	
	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "pf_dpda", 0, 0, 0);

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_F_PF,				/* ProcessID		*/
									D_SYS_THDID_PF_F_DPD,				/* ThreadID			*/
									D_SYS_THDQID_PF_F_DPD,			/* ThreadQueueID	*/
								    (unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/

	/* 初期化処理関数Call */
	(VOID)f_dpda_init_fhm(NULL);
	
	/* 1秒周期監視開始 */
	/* 1sec timer start */
	ret = BPF_RU_HRTM_REGISTER(
			BPF_RU_HRTM_REG_ONCE,
			E_RRH_TIMID_DPD_PERIODIC,
			D_DPDA_ONE_SHOT_TIME_1SEC,
			D_SYS_MSGID_TIMOUTNTC,
			D_SYS_THDQID_PF_F_DPD,
			&errcd
		);
	
	if( ret != BPF_RU_HRTM_COMPLETE )
	{
		/* Assert log */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, 
							"Timer Register NG!ret = %d, errcd = %d", 
							ret, errcd);
	}
	/****************************************************************************************************************/
	/* メッセージ受信待ち																							*/
	/****************************************************************************************************************/
	while(1)
	{
		/************************************************************************************************************/
		/* メッセージ受信																							*/
		/************************************************************************************************************/
		BPF_RU_IPCM_MSGQ_RECV(	D_SYS_THDQID_PF_F_DPD,			/* 受信MSGキューID							*/
								0,								/* 応答待ち時間								*/
								(VOID **)&rcvMsg,				/* メッセージ受信バッファ					*/
								&msgSize);						/* 最大受信バッファサイズ					*/
		
		/************************************************************************************************************/
		/* 受信メッセージ解析関数Call																				*/
		/************************************************************************************************************/
		(VOID)f_dpda_anlz((VOID*)rcvMsg);
	
		/* 受信バッファを解放する */
		BPF_RU_IPCM_PROCMSG_ADDRFREE((VOID*)rcvMsg);
	}
	
	return;
}
/* @} */
