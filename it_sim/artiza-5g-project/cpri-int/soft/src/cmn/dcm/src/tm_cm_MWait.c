/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	tm_cm_MWait.c
 *  @brief	タスク一時停止処理
 *  @date   2008/07/18 FFCS)Tangj create
 *  @date   2010/01/13 QNET)Kabasima M-S3G-eNBPF-02869:[品質改善]REデバッグコマンド機能拡充追加修正
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*****************************************************************************/

/** @addtogroup TRA_COM
 *  @{
 */
 
/****************************************************************************/
/* ヘッダインクルード                                                       */
/****************************************************************************/
#include "m_cm_header.h"			/* TRA共通ヘッダファイル			*/
#include <time.h>

/**************************************************************************/
/**
 *  @brief  タスク一時停止処理
 *  @note   タスクの実行を終了する。(自タスク停止状態へ移行) \n
 *          (1) タスク一時停止処理	 \n
 *  @param  waitTim			[in]		停止時間(10ms単位)
 *  @param  FileNamep		[in]		コール元関数ファイル名
 *  @param  LineNo			[in]		コール元関数実行ライン
 *  @return None
 *  @warning   N/A
 *  @FeatureID N/A
 *  @Bug_No    N/A
 *  @CR_No     N/A
 *  @TBD_No    N/A
 *  @date   2008/07/22		FFCS)Tangj	new create
 *  @date   2010/01/13 QNET)Kabasima M-S3G-eNBPF-02869:[品質改善]REデバッグコマンド機能拡充追加修正
 *  @date   2013/09/06 α)H.kondou	zynq化対応
 */
/**************************************************************************/

VOID cmR_MWait( UINT		waitTim,
				const VOID*	FileNamep,
				UINT		LineNo )
{
	int					a_ret;
	unsigned int		a_wait_sec;
	unsigned int		a_wait_msec;
	struct timespec		a_delayTime;
	struct timespec		a_restTime;

	if(waitTim == 0)
	{
		return;
	}

	/* 停止時間(10ms単位)をsecとnsecに変換 */
	a_wait_sec = waitTim / 100;
	a_wait_msec = waitTim % 100;
	a_delayTime.tv_sec  = a_wait_sec;
	a_delayTime.tv_nsec = a_wait_msec * 10000000;

	/* ============================================================= */
	/* ■タスク一時停止処理	                                         */
	/* ============================================================= */
	a_ret = BPF_RU_ITCM_THREAD_DELAY(
						&a_delayTime,
						BPF_RU_ITCM_CNT,
						&a_restTime);

	/*	ERRORの場合		*/
	if(a_ret != 0){
		BPF_COM_LOG_ASSERT ( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_ITCM_THREAD_DELAY error code=%d", a_ret );
	}
	
	return;
}
/* @} */
