/*!
 * @skip 		$ld:$
 * @file 		f_eom_InitReq.c
 * @brief		pf_eomスレッド 初期化処理
 * @date		2018/02/19 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_EOM
 * @{
 */

#include "f_eom_inc.h"

/****************************************************************************************************************************/
/*!
 * @brief		f_eom_InitReq
 * @note		pf_eomスレッドの初期化処理を行う
 *				-# pf_eomスレッド内部テーブル初期化
 *				-# pf_mainに初期化完了通知(Thread Init completion notice)を送信する(f_com_msgQSend())
 *				-# スレッド状態を"運用状態"に遷移させる
 * @param		*msgP			[in]VOID	受信メッセージ
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/02/19 FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
INT	f_eom_InitReq( VOID	*msgP )
{
	T_SYS_INIT_COMP_NTC				*sendMsg;							/* 送信msgポインタ		*/

	INT								endcd;								/* 返り値				*/
	INT								errcd;								/* 詳細NGコード			*/


	/********************************************************************************************/
	/* パラメータ初期設定																		*/
	/********************************************************************************************/

	/********************************************************************************************/
	/* 初期化完了通知 送信																		*/
	/********************************************************************************************/
	/* 通信用共有メモリを取得する	*/
	endcd = BPF_RU_IPCM_PROCMSG_ADDRGET( E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer種別			*/
										 sizeof(T_SYS_INIT_COMP_NTC),	/* Size					*/
										 (VOID **)&sendMsg,				/* msgP					*/
										 &errcd );
	/* 処理結果チェック	*/
	if( endcd != D_SYS_OK ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed get process message address %d", errcd);
		return D_SYS_NG;
	}

	/* 初期化完了通知msg編集	*/
	sendMsg->head.uiEventNo		= D_SYS_MSGID_INIT_COMP_NTC;		/* 初期化完了通知			*/
	sendMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;					/* 送信先Process Queue ID	*/
	sendMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_MAIN;				/* 送信先Thread Queue ID	*/
	sendMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;					/* 送信元Process Queue ID	*/
	sendMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_EOM;				/* 送信元Thread Queue ID	*/
	sendMsg->head.uiLength		= sizeof(T_SYS_INIT_COMP_NTC);		/* Length(Header部 + Data部)*/

	/* pf_mainへ初期化完了通知を送信	*/
	endcd = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_MAIN,			/* 送信先Thread Queue ID	*/
									(void *)sendMsg );				/* 送信msg					*/
	/* 処理結果チェック	*/
	if( endcd != D_SYS_OK ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed send message. %d", endcd);
		return D_SYS_NG;
	}
	/* Thread状態遷移(運用状態)	*/
	f_eomw_thdInfo.thdState  = E_EOM_STATE_ACT;

	return D_SYS_OK;
}

/* @} */
