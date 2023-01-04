/*!
 * @skip  $ld:$
 * @file  f_hdl_main_wdt.c
 * @brief pf_hdl 子スレッド メイン処理(f_hdl_main_wdt)
 * @date  2018/08/20 Fujitsu) 吉田 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */
 
/*!
 * @addtogroup RRH_PF_HDL
 * @{
 */

#include "f_rrh_reg_tim.h"
#include "f_hdl_inc.h"
#include "f_com_inc.h"

/*!
 * @brief 関数機能概要: pf_hdl子スレッドメイン(f_hdl_main_wdt)。スレッド起動からMSG受信待ちのLoop処理を実施
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
VOID f_hdl_main_wdt()
{
	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "pf_hdwdt", 0, 0, 0);

	UINT							endcd;
	UINT							regw_data ;


	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_hdl_main_wdt] ENTER" );

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET( D_RRH_PROCID_PF,					/* ProcessID						*/
								 D_SYS_THDID_PF_HDWDT,				/* ThreadID							*/
								 D_SYS_THDQID_PF_HDWDT,				/* ThreadQueueID					*/
								 (unsigned long int)pthread_self());	/* pThreadID					*/

	/****************************************************************************************************/
	/* 割り込み受信待ち																					*/
	/****************************************************************************************************/
	while(1)
	{
		/* 割り込み受信		*/
		endcd	= BPF_HM_DEVC_RESERVE(BPF_IRQ_WDT);					/* IRQ割り込み待ち処理				*/
		
		/* エラーログ取得	*/
		if( endcd != BPF_HM_DEVC_COMPLETE )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_hdl_main_wdt : error = 0x%x ", endcd);
			break;
		}

		/* WDT割り込みdisable処理(AXI Timer IRQクリア) */
		regw_data = D_RRH_TIM_WDT_AXITIMER_DIS;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_RRH_REG_TIM_AXITIMERCLRPLS0, &regw_data );
		
		/* WDT(IP Macro)割り込みFlagをONに設定 */
		f_comw_wdtHndlFlg = D_SYS_ON;
		
		/* INITプロセスにWDT TO リセット発生シグナルを送信  */
		BPF_RM_SVRM_SIGNAL_SEND(D_RRH_PROCID_INI, SIGUSR2, D_RRH_SIGUSR2_OPT_WDTTO );
	}
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_hdl_main_wdt] RETURN" );
	return;
}
/* @} */
