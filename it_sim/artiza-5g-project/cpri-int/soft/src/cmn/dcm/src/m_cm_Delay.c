/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cm_Delay.c
 *  @brief	 Task delay process (delay value < 10ms)
 *  @date   2012/07/08 FFCS)niemsh create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2012-
 */
/*****************************************************************************/

/** @addtogroup TRA_COM
 *  @{
 */
 
/****************************************************************************/
/* 繝倥ャ繝�繧､繝ｳ繧ｯ繝ｫ繝ｼ繝�                                                       */
/****************************************************************************/
#include "m_cm_header.h"			/* TRA共通ヘッダファイル			*/

/**************************************************************************/
/**
 *  @brief Task delay process (delay value < 10ms)
 *  @note Task delay process (delay value < 10ms)
 *  @param  delayTim		[in]		停止時間(1ms単位)
 *  @param  FileNamep		[in]		コール元関数ファイル名
 *  @param  LineNo			[in]		コール元関数実行ライン
 *  @warning   N/A
 *  @FeatureID N/A
 *  @Bug_No    N/A
 *  @CR_No     N/A
 *  @TBD_No    N/A
 *  @date   2012/07/08 FFCS)niemsh create
 *  @date   2013/11/26 alpha)Kondou	zynq化対応
 */
/**************************************************************************/
VOID m_cmR_Delay(
			UINT		delayTim,
			const VOID*	FileNamep,
			UINT		LineNo )
{
	INT					iResult;		/*	ステータス変数				*/
	struct timespec		tDelayTime;		/*	遅延時間					*/
	struct timespec		tRestTime;		/*	残り時間					*/
	

	/* タイマ値を設定 */
	tDelayTime.tv_sec  = (UINT)(delayTim / 1000);
	tDelayTime.tv_nsec = (UINT)(delayTim % 1000)*1000000;
	
	/*	タイマを起動	*/
	iResult = BPF_RU_ITCM_THREAD_DELAY(
				&tDelayTime,
				1,
				&tRestTime
			);
	
	/*	ERRORの場合		*/
	if(iResult != 0){
		BPF_COM_LOG_ASSERT ( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_ITCM_THREAD_DELAY error code=%d", iResult );
	}
	
	return;
	
}

/* @} */
