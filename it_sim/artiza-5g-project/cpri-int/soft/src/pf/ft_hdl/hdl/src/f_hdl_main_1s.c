/*!
 * @skip  $ld:$
 * @file  f_hdl_main_1s.c
 * @brief pf_hdl 子スレッド メイン処理(f_hdl_main_1s)
 * @date  2018/08/20 Fujitsu) 吉田 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */
 
/*!
 * @addtogroup RRH_PF_HDL
 * @{
 */

#include "f_hdl_inc.h"
#include "f_com_inc.h"

/*!
 * @brief 関数機能概要: pf_hdl子スレッドメイン(f_hdl_main_1s)。スレッド起動からMSG受信待ちのLoop処理を実施
 * @note  関数処理内容.
 *       -# 割り込み通知先管理テーブルより、割り込み待ち種別を取得
 *       -# 割り込み待ち無限Loop処理へ移行する
 *          -# 割り込み受信関数をCallする(BPF_HM_DEVC_RESERVE)
 *          -# 受信メッセージを解析し、pf_dpdaに割り込みを通知する
 * @param	-
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2018/08/20 Fujitsu) 吉田 Create
 */
VOID f_hdl_main_1s()
{
	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "pf_hd1s", 0, 0, 0);

	UINT							endcd;
	INT								errcd;
	T_RRH_HEAD						*msgp;
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_hdl_main_1s] ENTER" );
	
	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_PF,				/* ProcessID						*/
									D_SYS_THDID_PF_HD1S,			/* ThreadID							*/
									D_SYS_THDQID_PF_HD1S,			/* ThreadQueueID					*/
									(unsigned long int)pthread_self());	/* pThreadID	*/

	
	/****************************************************************************************************/
	/* 割り込み受信待ち																					*/
	/****************************************************************************************************/
	while(1)
	{
		/* 割り込み受信		*/
		endcd	= BPF_HM_DEVC_RESERVE(BPF_IRQ_1SEC);				/* IRQ割り込み待ち処理				*/
		
		/* エラーログ取得	*/
		if( endcd != BPF_HM_DEVC_COMPLETE )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_hdl_main_1s : error = 0x%x ", endcd);
			break;
		}
		
		if( f_comw_rf_setup_flg == D_RRH_OFF ){
			continue;
		}
		
		/* レジスタ読み込み(割込み要因取得) 1sec 割り込み要因なし	*/
		/************************************************************************************************/
		/* 送信処理(→pf_dpd)																			*/
		/************************************************************************************************/
		/* 通信用共有メモリを取得する	*/
		endcd = BPF_RU_IPCM_PROCMSG_ADDRGET(E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer種別				*/
											sizeof(T_RRH_HEAD),				/* Size						*/
											(void**)&msgp,					/* msgP						*/
											&errcd );
		if( endcd != D_RRH_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " ADDRGET ret=%08x, errcd=%08x ", endcd, errcd );
			break;
		}
		
		/* Header部設定	*/
		msgp->uiEventNo		= D_SYS_MSGID_TIMOUTNTC;					/* 1秒周期割り込み通知(0x10020003)	*/
		msgp->uiSignalkind	= D_DU_IFLOG_OFF;							/* IFログ保存なし					*/
		msgp->uiDstPQueueID = D_RRH_PROCQUE_PF;							/* 送信先Process Queue ID			*/
		msgp->uiDstTQueueID = D_SYS_THDQID_PF_CCA;						/* 送信先Thread Queue ID			*/
		msgp->uiSrcPQueueID = D_RRH_PROCQUE_PF;							/* 送信元Process Queue ID			*/
		msgp->uiSrcTQueueID = D_SYS_THDQID_PF_HD1S;			 			/* 送信元Thread Queue ID			*/
		msgp->uiLength = sizeof(T_RRH_HEAD);							/* Length(Head部のみ)				*/

		/* データの設定を行う */
		/* データ部無し */
		
		/* ログが埋まってしまうため削除	*/
		/* BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, " hdl SEND 1sec %d ", msgp->uiEventNo ); */
		
		/* 1秒周期割り込み通知を送信する */
		endcd = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_CCA,			/* 送信先Process Queue ID		*/
										(void*)msgp );					/* 送信Message					*/
		
		if( endcd != BPF_HM_DEVC_COMPLETE )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " MSGQ_SEND %d ", endcd );
			break;
		}
	}
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_hdl_main_1s] RETURN" );
	return;
}
/* @} */
