/*!
 * @skip  $ld:$
 * @file  f_hdl_main_sv.c
 * @brief pf_hdl 子スレッド メイン処理(f_hdl_main_sv)
 * @date  2013/03/25 ALPHA)松延 Create
 * @date  2013/11/10 ALPHA)松延 modify for docomo zynq
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */
 
/*!
 * @addtogroup RRH_PF_HDL
 * @{
 */

#include "f_hdl_inc.h"

/*!
 * @brief 関数機能概要: pf_hdl子スレッドメイン(f_hdl_main_sv)。スレッド起動からMSG受信待ちのLoop処理を実施
 * @note  関数処理内容.
 *       -# 割り込み通知先管理テーブルより、割り込み待ち種別を取得
 *       -# 割り込み待ち無限Loop処理へ移行する
 *          -# 割り込み受信関数をCallする(BPF_HM_DEVC_RESERVE)
 *          -# 受信メッセージを解析し、pf_rsvに割り込みを通知する
 * @param	-
 * @return		N/A
 * @warning		N/A
 * @FeatureID	PF-Hdl-002-005-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2013/03/25 ALPHA)松延 Create
 * @date 2013/11/10 ALPHA)鮫島 modify for docomo zynq
 * @date 2015/04/15 ALPHA)鎌田 modify for TDD
 */
VOID f_hdl_main_sv()
{
	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "pf_hdsv", 0, 0, 0);

	UINT					interruptFactor[D_RRH_SV_INF_NUM];		/* 割込み要因						*/
	INT						endcd;
	INT						errcd;
	T_SYS_TRA_IRQNTC		*msgp;
	UINT 					count;
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_hdl_main_svchg] ENTER" );
	
	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_F_PF,				/* ProcessID						*/
									D_SYS_THDID_PF_HDSV,			/* ThreadID							*/
									D_SYS_THDQID_PF_HDSV, (unsigned long int)pthread_self());			/* ThreadQueueID					*/

	/****************************************************************************************************/
	/* 割り込み受信待ち																					*/
	/****************************************************************************************************/
	while(1)
	{
		/* 割り込み受信		*/
		endcd = BPF_HM_DEVC_RESERVE(BPF_IRQ_SV);					/* IRQ割り込み待ち処理				*/
		
		/* エラーログ取得	*/
		if(endcd != BPF_HM_DEVC_COMPLETE )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " errorBPF_HM_DEVC_RESERVE 0x%08lx ", endcd );
			break;
		}
		
		/* 初期化 */
		memset((VOID*)&interruptFactor, 0, sizeof(interruptFactor));
		
		/* レジスタ読み込み */
		endcd = BPF_HM_DEVC_REG_READ_BUFFER(D_RRH_LOG_REG_LV_HDL_READ_BUF, D_RRH_REG_CNT_SV1INF, sizeof(interruptFactor), interruptFactor);

		/* エラーログ取得	*/
		if(endcd != BPF_HM_DEVC_COMPLETE)
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " errorREG_READ 0x%08lx ", endcd );
			break;
		}
		
		/************************************************************************************************/
		/* 送信処理(→pf_rsv)																			*/
		/************************************************************************************************/
		/* 通信用共有メモリを取得する	*/
		endcd = BPF_RU_IPCM_PROCMSG_ADDRGET(E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer種別				*/
											sizeof(T_SYS_TRA_IRQNTC),		/* Size						*/
											(void**)&msgp,					/* msgP						*/
											&errcd );
		if( endcd != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "ADDRGET 0x%08lx ", endcd );
			break;
		}
		
		/* Header部設定	*/
		msgp->head.uiEventNo 	 = D_SYS_MSGID_TRA_IRQNTC;					/* カード状態変化割込通知(0x10020011)*/
		msgp->head.uiDstPQueueID = D_RRH_PROCQUE_F_PF;						/* 送信先Process Queue ID	*/
		msgp->head.uiDstTQueueID = D_SYS_THDQID_PF_RSV;						/* 送信先Thread Queue ID	*/
		msgp->head.uiSrcPQueueID = D_RRH_PROCQUE_F_PF;						/* 送信元Process Queue ID	*/
		msgp->head.uiSrcTQueueID = D_SYS_THDQID_PF_HDSV;					/* 送信元Thread Queue ID	*/
		msgp->head.uiLength 	 = sizeof(T_SYS_TRA_IRQNTC);				/* Length(Head部 + Data部)	*/
		/* データの設定を行う */
		for(count = 0; count < D_RRH_SV_INF_NUM; count++)
		{
			msgp->sv_inf[count] = interruptFactor[count];					/* IRQ割り込み要因の設定	*/
		}
		
		
		/* TRAカード状態変化割込通知を送信する */
		endcd = BPF_RU_IPCM_MSGQ_SEND(D_SYS_THDQID_PF_RSV,					/* 送信先Process Queue ID	*/
										(void*)msgp );						/* 送信Message				*/
		
		if( endcd != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "SEND 0x%08lx ", endcd );
			break;
		}

	}
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_hdl_main_svchg] RETURN" );
	return;
}

/* @} */
