/*!
 * @skip  $ld:$
 * @file  f_wdt_main.c
 * @brief WDTスレッドメイン処理
 * @date  2013/11/15 ALPHA) 中村 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2016
 */

/*!
 * @addtogroup RRH_PF_WDT
 * @{
 */

#include "f_wdt_inc.h"

/*!
 * @brief f_wdt_main
 * @note  関数機能概要:1000ms周期で起動しWDT停止・設定・開始の制御を行う。
 *       -# pf_mainへ「Task Init completion notice」を送信する。
 *       -# 以下無限Loopで処理を実施
 *       -# 1000msのdelayを実施。
 *       -# 1000ms delay後、WDT制御レジスタのWDT停止を設定
 *       -# WDT制御レジスタへ8秒を設定する。
 *       -# WDT制御レジスタへWDT開始を設定する(8秒とOrしてWrite)
 * @param		-
 * @return		N/A
 * @warning		N/A
 * @FeatureID	PF-WDT-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2013/11/15 ALPHA) 中村 Create
 * @date  2014/12/23 ALPHA) 藤井 modify for ppcEOL
 * @date  2015/04/16 ALPHA) 鎌田 modify for TDD
 * @date  2015/09/08 ALPHA) kamada add＆mod RRH-006-000 対応
 * @date  2016/03/14 ALPHA) fujiiy modify for M-RRU-ZSYS-02088
 */
VOID f_wdt_main()
{
	T_SYS_INIT_COMP_NTC*	sndMsg;			/* 送信メッセージ(初期化完了通知)	*/
	INT						rtn_bpf;
	INT						errcd;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_wdt_main] ENTER" );

	/* core0追加 */
	if( ( rtn_bpf = BPF_RM_SVRM_CPUAFFINITY_ADD(D_RRH_PROCID_PF, 0, &errcd)) != BPF_RM_SVRM_COMPLETE )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "CPUAFFINITY ADD(Core 0) ERROR %d", errcd );
	}

	/* core2追加 */
	if( ( rtn_bpf = BPF_RM_SVRM_CPUAFFINITY_ADD(D_RRH_PROCID_PF, 2, &errcd)) != BPF_RM_SVRM_COMPLETE )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "CPUAFFINITY ADD(Core 2) ERROR %d", errcd );
	}

	/* core3追加 */
	if( ( rtn_bpf = BPF_RM_SVRM_CPUAFFINITY_ADD(D_RRH_PROCID_PF, 3, &errcd)) != BPF_RM_SVRM_COMPLETE )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "CPUAFFINITY ADD(Core 3) ERROR %d", errcd );
	}

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_PF,				/* ProcessID		*/
									D_SYS_THDID_PF_WDT,				/* ThreadID			*/
									D_SYS_THDQID_PF_WDT,			/* ThreadQueueID	*/
									(unsigned long int)pthread_self()) ;			/* Rhread ID        */
	rtn_bpf = BPF_RU_IPCM_OK;

	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "pf_wdt", 0, 0, 0);

	/************************************************************/
	/*WDT停止(リセット)解除                                     */
	/************************************************************/
	rtn_bpf = BPF_HM_DEVC_WDT_SET( D_SYS_AXI_TIMER_CLR, D_SYS_SWDT_TIMER_CLR );
	if( rtn_bpf != BPF_HM_DEVC_COMPLETE )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "WDT TIMEOUTRANGE CLEAR error=%08x", rtn_bpf );
	}
	
	/************************************************************/
	/*WDT起動                                                   */
	/************************************************************/
	rtn_bpf = BPF_HM_DEVC_WDT_SET( D_SYS_AXI_TIMER_SET, D_SYS_SWDT_TIMER_SET );
	if( rtn_bpf != BPF_HM_DEVC_COMPLETE )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "WDT TIMEOUTRANGE SET error=%08x", rtn_bpf );
	}
	
	/****************************************************************************************************************/
	/* 初期化完了通知送信																							*/
	/****************************************************************************************************************/
	/* バッファ取得 */
	rtn_bpf = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,		/* Buffer種別						*/
											sizeof(T_SYS_INIT_COMP_NTC),		/* MessageSize						*/
											(VOID **)&sndMsg,					/* MessageBuffer					*/
											&errcd );							/* ErrorCode						*/
	
	if( rtn_bpf != BPF_RU_IPCM_OK  )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BUFFER GET error=%08x", rtn_bpf );
	}

	/* 送信MSG作成 */
	sndMsg->head.uiEventNo		= D_SYS_MSGID_INIT_COMP_NTC;					/* メッセージID(0x00000102)			*/
	sndMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;								/* 返信先PQID						*/
	sndMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_MAIN;							/* 送信先TQID						*/
	sndMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;								/* 送信元PQID						*/
	sndMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_WDT;							/* 送信元TQID						*/
	sndMsg->head.uiLength		= sizeof(T_SYS_INIT_COMP_NTC);					/* データ部バイト長					*/
	
	BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_MAIN,								/* 送信MSGキューID					*/
							(VOID *)sndMsg );	

	for(;;)
	{
		/* 1000ms delay					*/
		f_com_taskDelay(
			D_WDT_DELAY_VALUE,													/*	100Tick(1000ms)					*/
			D_SYS_THDQID_PF_WDT,												/*	WDTスレッド番号					*/
			0,																	/*	スレッド状態(状態なしのため0)	*/
			0x01																/*	イベント番号					*/
		);

		if(f_comw_wdtHndlFlg == D_SYS_ON)
		{
			/* AXI_TIMER(IP Macro)割り込みが上がっている場合、SWDTのタイマクリアを避ける */
		}
		else
		{
	        /* WDT Counter Restart */
	        rtn_bpf = BPF_HM_DEVC_WDT_CLEAR();
			
			if( rtn_bpf != BPF_HM_DEVC_COMPLETE )
	        {
	            BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "WDT RESTART error=%08x", rtn_bpf);
		    }
		}

	}
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_wdt_main] RETURN" );
}

/* @} */
