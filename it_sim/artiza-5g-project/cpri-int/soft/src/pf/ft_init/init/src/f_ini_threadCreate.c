/*!
 * @skip  $ld:$
 * @file  f_ini_threadCreate.c
 * @brief INIT 初期状態 処理
 * @date  2013/03/19 ALPHA)松延 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2014
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include "f_ini_inc.h"

/*!
 * @brief 関数機能概要:INITスレッド初期状態処理。
 * @note  関数処理内容.
 *		-# 各スレッド生成・起動
 * @param  *rcvMsg_p	[in]	受信メッセージ
 * @return INT
 * @retval D_RRH_OK		0:正常終了
 * @retval D_RRH_NG		1:異常終了
 * @warning		N/A
 * @FeatureID	PF-Hdl-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2014/02/01 ALPHA)yokoyama Create
 */
UINT f_ini_threadCreate( VOID* buf_p )
{
	UINT					thdIniCnt	= 0;
	INT						endcd		= 0;
	unsigned long int		threadId	= 0;									/*	スレッドID格納変数				*/
	INT						iErrorCode	= 0;									/*	詳細エラーコード				*/
	
	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ENTER, "[f_ini_threadCreate] ENTER" );
	
	/****************************************************************************************************************/
	/* 各スレッド生成・起動																							*/
	/****************************************************************************************************************/
	for(thdIniCnt = 0; thdIniCnt < (D_INI_INITHDNO_MAX - 1); thdIniCnt++)
	{
		/*	スレッドを生成		*/
		endcd = BPF_RU_ITCM_THREAD_CREATE(
						(unsigned long int *)&threadId,							/* ISスレッド名称					*/
						BPF_RU_ITCM_SCHED_OTHER,								/* IAスレッド優先度					*/
						(INT)f_inir_thdCreMng[thdIniCnt].thdPriority,			/* オプション						*/
						(INT)f_inir_thdCreMng[thdIniCnt].thdStackSize,			/* IAスレッドスタックサイズ			*/
						(INT*)(FUNCPTR)f_inir_thdCreMng[thdIniCnt].thdFunc,		/* IAスレッドメイン関数ポインタ		*/
						D_RRH_NULL,												/* NIスレッド番号(For Log)			*/
						&iErrorCode );											/* NIスレッド状態(For Log)			*/

		/*	ERRORの場合	*/
		if(endcd != D_SYS_OK)
		{
			cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "CREATE NG %d threadid=%d thdPriority=%d thdStackSize=%d",
					endcd,threadId,(INT)f_inir_thdCreMng[thdIniCnt].thdPriority,(INT)f_inir_thdCreMng[thdIniCnt].thdStackSize);

			return D_INI_NG;
		}
	}
	
	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_RETURN, "[f_ini_threadCreate] RETURN" );
	return D_INI_OK;
}

UINT f_ini_threadCreate2( VOID* buf_p )
{
	UINT					thdIniCnt	= (D_INI_INITHDNO_MAX - 1);
	INT						endcd		= 0;
	unsigned long int		threadId	= 0;									/*	スレッドID格納変数				*/
	INT						iErrorCode	= 0;									/*	詳細エラーコード				*/
	
	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ENTER, "[f_ini_threadCreate2] ENTER" );
	
	/****************************************************************************************************************/
	/* 各スレッド生成・起動																							*/
	/****************************************************************************************************************/
	/*	スレッドを生成		*/
	endcd = BPF_RU_ITCM_THREAD_CREATE(
						(unsigned long int *)&threadId,							/* ISスレッド名称					*/
						BPF_RU_ITCM_SCHED_OTHER,								/* IAスレッド優先度					*/
						(INT)f_inir_thdCreMng[thdIniCnt].thdPriority,			/* オプション						*/
						(INT)f_inir_thdCreMng[thdIniCnt].thdStackSize,			/* IAスレッドスタックサイズ			*/
						(INT*)(FUNCPTR)f_inir_thdCreMng[thdIniCnt].thdFunc,		/* IAスレッドメイン関数ポインタ		*/
						D_RRH_NULL,												/* NIスレッド番号(For Log)			*/
						&iErrorCode );											/* NIスレッド状態(For Log)			*/

	/*	ERRORの場合	*/
	if(endcd != D_SYS_OK){
		cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "CREATE NG %d threadid=%d thdPriority=%d thdStackSize=%d",
				endcd,threadId, (INT)f_inir_thdCreMng[thdIniCnt].thdPriority, (INT)f_inir_thdCreMng[thdIniCnt].thdStackSize);
		return D_INI_NG;
	}

	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_RETURN, "[f_ini_threadCreate2] RETURN" );
	return D_INI_OK;
}
/* @} */
