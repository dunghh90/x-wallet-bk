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
 */
VOID f_main_threadCreate()
{
	UINT	thdIniCnt	= 0;
	INT		endcd		= 0;
	UINT	threadId	= 0;		/*	スレッドID格納変数				*/
	INT		iErrorCode	= 0;		/*	詳細エラーコード				*/

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_threadCreate] ENTER" );

	/************************************************************************************************/
	/* 各スレッド生成・起動																			*/
	/************************************************************************************************/
	for(thdIniCnt = 0; thdIniCnt < D_MAIN_THDID_MAX; thdIniCnt++)
	{
		/* シリアル起動以外のスレッドを生成・起動	*/
		if(f_mainr_thdCreMng_TDD[thdIniCnt].thdCreateFlag == D_MAIN_CREATE_PTN_NOT_WDT)
		{
			/*	スレッドを生成		*/
			endcd = BPF_RU_ITCM_THREAD_CREATE(
							(unsigned long int *)&threadId,							/* Thread id						*/
							(INT)f_mainr_thdCreMng_TDD[thdIniCnt].SchPolicy,			/* Scheduling policy				*/
							(INT)f_mainr_thdCreMng_TDD[thdIniCnt].thdPriority,			/* Thread priority					*/
							(INT)f_mainr_thdCreMng_TDD[thdIniCnt].thdStackSize,			/* Thread stack size				*/
							(INT*)(FUNCPTR)f_mainr_thdCreMng_TDD[thdIniCnt].thdFunc,	/* IAスレッドメイン関数ポインタ		*/
							(VOID*)f_mainr_thdCreMng_TDD[thdIniCnt].option,				/* オプション						*/
							&iErrorCode );											/* Error code						*/

			/*	ERRORの場合	*/
			if(endcd != BPF_RU_ITCM_OK)
			{
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "CREATE %s NG %d threadid=%d thdPriority=%d thdStackSize=%d",
					f_mainr_thdCreMng_TDD[thdIniCnt].thdName, endcd, (UINT)threadId, (INT)f_mainr_thdCreMng_TDD[thdIniCnt].thdPriority, (INT)f_mainr_thdCreMng_TDD[thdIniCnt].thdStackSize);
			}
			
			/* スレッドID保持				*/
			f_mainw_thdId.thdId[thdIniCnt]	= threadId;
		}
	}
	
	/* スレッド状態遷移(初期化完了待ち)		*/
	f_mainw_thdmng_state = D_MAIN_THD_STS_INI_WAITING;
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_threadCreate] RETURN" );
}

/* @} */
