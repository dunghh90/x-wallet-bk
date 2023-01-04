/*!
 * @skip  $ld:$
 * @file  f_hdl_main.c
 * @brief pf_hdl メイン処理
 * @date  2013/03/19 ALPHA)松延 Create
 * @date  2013/11/10 ALPHA)鮫島 modify for docomo zynq
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_HDL
 * @{
 */

#include "f_hdl_inc.h"

/*!
 * @brief 関数機能概要:pf_hdlメインスレッド。起動からMSG受信待ちのLoop処理を実施
 * @note  関数処理内容.
 *		-# 初期化タスク状態を"初期化状態"に設定
 *		-# hdl無い各IRQ待ち受けスレッド生成			f_hdl_init()        関数Call
 *		-# MSG待ち無限Loop処理へ移行				f_com_msgQReceive() 関数Call
 * @return		N/A
 * @warning		N/A
 * @FeatureID	PF-Hdl-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2013/03/19 ALPHA)松延 Create
 * @date 2013/11/10 ALPHA)鮫島 modify for docomo zynq
 */
VOID f_hdl_main(T_HDL_CRT_EVENT_CTBL f_hdlr_thdCreMng[])
{
	UINT			rcvSize;
	VOID			*pMsg;
	INT				ret;
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_hdl_main] ENTER" );
	
	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "pf_hdlm", 0, 0, 0);

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "thdPriority=%d thdStackSize=%d",
		(INT)f_hdlr_thdCreMng[0].thdPriority,(INT)f_hdlr_thdCreMng[0].thdStackSize);

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_F_PF,				/* ProcessID		*/
									D_SYS_THDID_PF_HDLM,			/* ThreadID			*/
									D_SYS_THDQID_PF_HDLM,			/* ThreadQueueID	*/
								    (unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/
	
	/* タスク状態に初期状態を設定	*/
	f_hdlw_thdState = D_HDL_THD_STS_INIT;
	
	
	/* 初期化タスク内初期化処理		*/
	memcpy(f_hdlw_thdCreMng, f_hdlr_thdCreMng, sizeof(T_HDL_CRT_EVENT_CTBL) * D_RRH_HDLTHDNO_MAX);
	f_hdl_init();
	
	/* タスク状態に運用中状態を設定	*/
	f_hdlw_thdState = D_HDL_THD_STS_ACT;
	
	/********************************************************************************/
	/* MSG受信処理の無限Loop														*/
	/********************************************************************************/
	for(;;)
	{
		/************************************************************************************************************/
		/* メッセージ受信																							*/
		/************************************************************************************************************/
		ret = BPF_RU_IPCM_MSGQ_RECV(
					D_SYS_THDQID_PF_HDLM,												/* 受信MSGキューID			*/
					0,																	/* 受信待ち時間				*/
					(VOID**)&pMsg,														/* メッセージ受信バッファ	*/
					&rcvSize	);														/* 受信Messageサイズ		*/
		if( ret != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "[f_hdl_main] RECIEVE" );
			break;
		}
		
		/* 受信バッファ解放	*/
		ret = BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)pMsg );
		if( ret != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "[f_hdl_main] FREE" );
			break;
		}
		
	}
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_hdl_main] RETURN" );
}

/* @} */
