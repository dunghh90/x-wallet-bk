/*!
 * @skip  $ld:$
 * @file  f_hdl_main_sv.c
 * @brief pf_hdl 子スレッド メイン処理(f_hdl_main_sv)
 * @date  2018/08/20 Fujitsu) 吉田 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */
 
/*!
 * @addtogroup RRH_PF_HDL
 * @{
 */

#include "f_du_reg_ctb.h"
#include "f_hdl_inc.h"
#include "f_com_inc.h"

/*!
 * @brief 関数機能概要: pf_hdl子スレッドメイン(f_hdl_main_sv)。スレッド起動からMSG受信待ちのLoop処理を実施
 * @note  関数処理内容.
 *       -# 割り込み通知先管理テーブルより、割り込み待ち種別を取得
 *       -# 割り込み待ち無限Loop処理へ移行する
 *          -# 
 *          -# 
 * @param	-
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2018/08/20 Fujitsu) 吉田 Create
 */
VOID f_hdl_main_sv()
{
	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "pf_hdsv", 0, 0, 0);

	UINT							interruptFactor[D_SYS_NUM16];
	UINT							endcd = BPF_HM_DEVC_COMPLETE;
	INT								errcd ;
	T_SYS_EQS_ALM_INFO				*msgp;
	int								count;


	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_hdl_main_sv] ENTER" );

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET( D_RRH_PROCID_PF,					/* ProcessID						*/
								 D_SYS_THDID_PF_HDSV,				/* ThreadID							*/
								 D_SYS_THDQID_PF_HDSV,				/* ThreadQueueID					*/
								 (unsigned long int)pthread_self());	/* pThreadID					*/

	/****************************************************************************************************/
	/* 割り込み受信待ち																					*/
	/****************************************************************************************************/
	while(1)
	{
		/* 割り込み受信		*/
		endcd	= BPF_HM_DEVC_RESERVE(BPF_IRQ_SV);					/* IRQ割り込み待ち処理				*/
		
		/* エラーログ取得	*/
		if( endcd != BPF_HM_DEVC_COMPLETE )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_hdl_main_sv : error = 0x%x ", endcd);
			break;
		}

		/* 初期化 */
		memset ((VOID*)&interruptFactor, 0 , sizeof(interruptFactor));

		/* レジスタ読み込み(svinf1～16) */
		endcd = BPF_HM_DEVC_REG_READ_BUFFER(0, D_DU_REG_CTB_SV1INF,
											sizeof(interruptFactor), (UINT *)&interruptFactor) ;
		/* エラーログ取得	*/
		if( endcd != BPF_HM_DEVC_COMPLETE )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " errorREG_READ 0x%08lx ", endcd );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " errorREG Size 0x%08lx ", sizeof(interruptFactor));
			break;
		}

		/* 通信用共有メモリ(→pf_rsv)を取得する	*/
		endcd = BPF_RU_IPCM_PROCMSG_ADDRGET(E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer種別				*/
											sizeof(T_SYS_EQS_ALM_INFO),		/* RRE装置用Size			*/
											(void**)&msgp,					/* msgP						*/
											&errcd );
		if( endcd != BPF_HM_DEVC_COMPLETE )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "ADDRGET 0x%08lx ", endcd );
			break;
		}
		/*****************************************************************/
		/* 送信処理(→pf_eqs)                                            */
		/*****************************************************************/
		/* Header部設定	*/
		msgp->thdIf.uiEventNo 	 = D_SYS_MSGID_TRA_IRQNTC;					/* カード状態変化割込通知(0x10020011)*/
		msgp->thdIf.uiDstPQueueID = D_RRH_PROCQUE_PF;						/* 送信先Process Queue ID	*/
		msgp->thdIf.uiDstTQueueID = D_SYS_THDQID_PF_EQS;					/* 送信先Thread Queue ID	*/
		msgp->thdIf.uiSrcPQueueID = D_RRH_PROCQUE_PF;						/* 送信元Process Queue ID	*/
		msgp->thdIf.uiSrcTQueueID = D_SYS_THDQID_PF_HDSV;					/* 送信元Thread Queue ID	*/
		msgp->thdIf.uiLength 	 = sizeof(T_SYS_EQS_ALM_INFO);				/* Length(Head部 + Data部)	*/
		/* データ部の設定を行う */
		for(count = 0; count < D_DU_SVINF_NUM ; count++)
		{
			msgp->sv_inf[count] = interruptFactor[count] ;
		}

		/* カード状態変化割込通知を送信する */
				endcd = BPF_RU_IPCM_MSGQ_SEND(D_SYS_THDQID_PF_EQS,					/* 送信先Process Queue ID	*/
												(void*)msgp );						/* 送信Message				*/
		if( endcd != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "SEND 0x%08lx ", endcd );
		}
	}
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_hdl_main_sv] RETURN" );
	return;
}
/* @} */
