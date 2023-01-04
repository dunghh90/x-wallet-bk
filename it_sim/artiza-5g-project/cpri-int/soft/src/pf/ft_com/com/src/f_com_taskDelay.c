/*!
 * @skip  $ld:$
 * @file  f_com_taskDelay.c
 * @brief タイマ起動
 * @date  2008/08/29 ALPHA) 井上 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008-2013
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_com_inc.h"

/*!
 * @brief 関数機能概要:呼び出しタスクに特定された接続のCPUを放棄し遅れさせる。
 * @note  関数処理内容.
 *       -# パラメータを受け取る.
 *       -# タイマ起動処理を行う（BPF関数をコール）.
 *       -# APIエラー応答時、アボート関数をコール f_com_abort()
 * @param  tick			[in]int			Delay時間(単位1ms)
 * @param  task_no		[in]UINT		タスク番号(ログ用)
 * @param  state		[in]UINT		タスク状態(ログ用)
 * @param  evt_no		[in]UINT		MSGイベント番号(ログ用)
 * @return VOID
 * @warning   N/A
 * @FeatureID PF-COM-001-001-001
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2008/08/29 ALPHA)井上 Create
 * @date 2010/02/22 ALPHA)横山 LTE-RRH対応
 * @date 2013/03/16 ALPHA)fujiiy    modiry for B25
 * @date 2013/11/26 ALPHA)kondou    zynq化対応
 */
VOID f_com_taskDelay(
			int			tick,
			UINT		task_no,		/*	タスク番号(ログ用)			*/
			UINT		state,			/*	タスク状態(ログ用)			*/
			UINT		evt_no			/*	MSGイベント番号(ログ用)		*/
){
	
	INT					iResult;		/*	ステータス変数				*/
	struct timespec		tDelayTime;		/*	遅延時間					*/
	struct timespec		tRestTime;		/*	残り時間					*/
	

	/* タイマ値を設定 */
	tDelayTime.tv_sec  = (UINT)(tick / 1000);
	tDelayTime.tv_nsec = (UINT)((tick % 1000)*1000000);	/* pgr0062	*/
	
	/*	タイマを起動	*/
	iResult = BPF_RU_ITCM_THREAD_DELAY(
				&tDelayTime,
				1,
				&tRestTime
			);
	
	/*	ERRORの場合		*/
	if(iResult != 0){
		/*assert log*/
		BPF_COM_LOG_ASSERT ( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_ITCM_THREAD_DELAY error code=%d", iResult );
	}
	
	return;
	
}

/* @} */
