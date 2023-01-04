/*!
 * @skip  $ld:$
 * @file  f_dpda_init.c
 * @brief DPDAタスク 初期化処理
 * @date  2018/08/10 KCN)takagi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"
#include "BPF_COM_CMD.h"

/*!
 * @brief 関数機能概要:DPDAタスクの初期化処理を行う
 * @note  関数処理内容.
 *       -# DPDAタスクの内部テーブル初期化を行う
 *       -# 初期化タスクに初期化完了通知(Task Init completion notice)を送信する(f_com_msgQSend())
 *       -# タスク状態を"運用状態"に遷移させる
 * @param  msgP				[in]VOID	受信メッセージ
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2018/08/10 KCN)takagi Create
*/
INT f_dpda_init(VOID* msgP)
{
	T_SYS_INIT_COMP_NTC*			sndMsg;					/* 送信メッセージ(初期化完了通知)						*/
	INT								ret;
	INT								errcd;
	UCHAR	dev_kind;
	UCHAR	frq_kind;

	/* EEPROM(0x40)から装置種別取得 */
	dev_kind = f_cmw_tra_inv_tbl->cmx_device_typ;
	frq_kind = f_cmw_tra_inv_tbl->cmx_device_BW;

	if(dev_kind == D_DU_EEP_DEV_KIND_5GDU_SUB6) {
		f_dpdaw_reKind = E_DPDA_REKIND_5GDU_SUB6;
		if(frq_kind == D_DU_EEP_DEV_FREQ_37G)
		{
			f_dpdaw_ruIndex = E_DPDA_RUINDEX_SUB6_37G;
		}
		else
		{
			f_dpdaw_ruIndex = E_DPDA_RUINDEX_SUB6_45G;
		}
	}
	else if (dev_kind == D_DU_EEP_DEV_KIND_5GDU_MMW) {
		f_dpdaw_reKind = E_DPDA_REKIND_5GDU_MMW;
		f_dpdaw_ruIndex = E_DPDA_RUINDEX_MMW_280G;
	}
	else {
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "DEVICE KIND error. dev_kind=%08x", dev_kind);
		f_com_abort( D_SYS_THDID_PF_CCA, D_DU_ALMCD_DEVKIND_ABNORMAL );
	}

	/****************************************************************************************************************/
	/* トレーニングデータ展開とRFデバイス起動																		*/
	/****************************************************************************************************************/
	f_com_dpda_init(dev_kind);

	/****************************************************************************************************************/
	/* DPDAタスク内部テーブル初期化																					*/
	/****************************************************************************************************************/
	f_dpda_init_taskTable();
	
	/****************************************************************************************************************/
	/* レジスタ設定																									*/
	/****************************************************************************************************************/
	f_dpda_init_reg();
	
	/****************************************************************************************************************/
	/* 初期化完了通知送信																							*/
	/****************************************************************************************************************/
	/* 通信用共有メモリを取得する	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別					*/
										sizeof(T_SYS_INIT_COMP_NTC),			/* Size							*/
										(VOID **)&sndMsg,						/* msgP							*/
										&errcd );
	if( ret != D_SYS_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR,
							"failed get process message address %d\n",
							errcd);
		return D_SYS_NG;
	}
	
	/* 送信MSG作成 */
	sndMsg->head.uiEventNo			= D_SYS_MSGID_INIT_COMP_NTC;		/* イベント番号						*/
	sndMsg->head.uiSignalkind		= 0;								/* SignalKind						*/
	sndMsg->head.uiDstPQueueID		= D_RRH_PROCQUE_PF;					/* 最終送信先Process Queue ID		*/
	sndMsg->head.uiDstTQueueID		= D_SYS_THDQID_PF_MAIN;				/* 最終送信先Thread  Queue ID		*/
	sndMsg->head.uiSrcPQueueID		= D_RRH_PROCQUE_PF;					/* 送信元    Process Queue ID		*/
	sndMsg->head.uiSrcTQueueID		= D_SYS_THDQID_PF_CCA;				/* 送信元    Thread  Queue ID		*/
	sndMsg->head.uiLength			= sizeof(T_SYS_INIT_COMP_NTC);		/*Length(Head部 + Data部)			*/

	BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_MAIN,						/* 送信MSGキューID					*/
							sndMsg);									/* 送信MSG							*/
	
	/* タスク状態遷移(運用状態) */
	f_dpdaw_info_all->taskInfo.taskState= D_DPDA_TASK_STS_ACT;
	
	/* duコマンド用メモリ初期化 */
	BPF_COM_CMD_CLR_ENDP(D_DPDA_CELL_INVALID);
	
	return D_SYS_OK;
}

/* @} */
