/*!
 * @skip		$ld:$
 * @file		f_main_threadCreate.c
 * @brief		pf_main スレッド起動処理
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include "f_main_inc.h"

/*!
 * @brief		f_main_threadCreate
 * @note		各スレッドを起動する.
 *					-# スレッド数分ループし、スレッド生成.
 *					-# [初期化完了待ち]へ状態遷移.
 * @param		-
 * @return		N/A
 * @warning		N/A
 * @FeatureID	PF-MAIN-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 * @date		2015/08/27 ALPHA) miyazaki modify to TDD-RRE-Zynq 
 */
VOID f_main_threadCreate()
{
	UINT					thdIniCnt	= 0;
	INT						endcd		= 0;
	unsigned long int		threadId	= 0;		/*	スレッドID格納変数				*/
	INT						iErrorCode	= 0;		/*	詳細エラーコード				*/
	T_MAIN_CRT_EVENT_CTBL	*f_mainr_pThdCreMng ;


	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_threadCreate] ENTER" );
	/************************************************************************************************/
	/* 各スレッド生成・起動																			*/
	/************************************************************************************************/
	printf("\n f_mainw_process_start_mode %d",f_mainw_process_start_mode);
	if(f_mainw_process_start_mode == 1){
		f_mainr_pThdCreMng = &f_mainr_thdCreMng_ptp[0];
	}
	else{
		f_mainr_pThdCreMng = &f_mainr_thdCreMng[0];
	}
	
	f_mainw_threadinitcmpbit = D_RRH_OFF;

	for(thdIniCnt = 0; thdIniCnt < D_MAIN_THDID_MAX; thdIniCnt++)
	{
		/* シリアル起動以外のスレッドを生成・起動	*/
		if( (f_mainr_pThdCreMng + thdIniCnt )->thdCreateFlag == D_MAIN_CREATE_PTN_NOT_WDT)
		{
			/*	スレッドを生成		*/
			endcd = BPF_RU_ITCM_THREAD_CREATE(
							(unsigned long int *)&threadId,							/* Thread id						*/
							(INT)( f_mainr_pThdCreMng + thdIniCnt )->SchPolicy,		/* Scheduling policy				*/
							(INT)( f_mainr_pThdCreMng + thdIniCnt )->thdPriority,	/* Thread priority					*/
							(INT)( f_mainr_pThdCreMng + thdIniCnt )->thdStackSize,	/* Thread stack size				*/
							(INT*)(FUNCPTR)( f_mainr_pThdCreMng + thdIniCnt )->thdFunc,	/* IAスレッドメイン関数ポインタ	*/
							(VOID*)(f_mainr_pThdCreMng + thdIniCnt )->option,			/* オプション					*/
							&iErrorCode );											/* Error code						*/

			cmn_com_assert_th_get(f_mainw_assert_p,D_RRH_LOG_AST_LV_INFO, "%s Thread create start", (f_mainr_pThdCreMng + thdIniCnt)->thdName);
			printf("\n file %s , line %d ,endcd %d  , thdName =%s",__FILE__,__LINE__,endcd,(f_mainr_pThdCreMng + thdIniCnt)->thdName);//C9 add code debug
			/*	ERRORの場合	*/
			if(endcd != BPF_RU_ITCM_OK)
			{
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "CREATE %s NG %d threadid=%ld thdPriority=%d thdStackSize=%d",
					(f_mainr_pThdCreMng + thdIniCnt)->thdName, 
							endcd, 
							threadId, 
							(INT)( f_mainr_pThdCreMng + thdIniCnt )->thdPriority, 
							(INT)( f_mainr_pThdCreMng + thdIniCnt )->thdStackSize);
			}
			
			/* スレッドID保持				*/
			f_mainw_thdId.thdId[thdIniCnt]	= threadId;
			f_mainw_threadinitcmpbit |= (UINT)((UINT)(D_RRH_ON) << thdIniCnt);

			cmn_com_assert_th_get( f_mainw_assert_p,D_RRH_LOG_AST_LV_INFO, "%s Thread create comp  id:0x%lx pri:%d stack:0x%x set:0x%08x exp:0x%08x",
								   (f_mainr_pThdCreMng + thdIniCnt)->thdName,
								   (ULONG)threadId,
								   (INT)( f_mainr_pThdCreMng + thdIniCnt )->thdPriority,
								   (INT)( f_mainr_pThdCreMng + thdIniCnt )->thdStackSize,
								   (INT)( (UINT)((UINT)(D_RRH_ON) << thdIniCnt) ),
								   f_mainw_threadinitcmpbit);
		}
	}
	
	/* スレッド状態遷移(初期化完了待ち)		*/
	f_mainw_thdmng_state = D_MAIN_THD_STS_INI_WAITING;
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_threadCreate] RETURN");
}

/* @} */
