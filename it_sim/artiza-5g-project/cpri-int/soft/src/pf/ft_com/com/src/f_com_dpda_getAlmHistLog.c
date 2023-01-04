/*!
 * @skip  $ld:$
 * @file  f_com_dpda_getAlmHistLog.c
 * @brief 共通関数 Alarm History Log情報取得処理
 * @date  2011/10/06 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2013
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_com_inc.h"

/************************************************************************************************/
/*!
 * @brief 関数機能概要: Alarm History Log情報(1秒周期)取得処理を行う
 * @note  関数処理内容.
 *		-# Alarm History Log情報(1秒周期)取得処理を行う
 	* @param		pAlmHistLogInfo		[out]T_RRH_ALARM_HISTORYLOG_INFO*	Alarm History Log情報(1秒周期)
 * @return		VOID
 * @warning		N/A
 * @FeatureID	PF-Log-002-002-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2013/12/10 ALPHA)加藤 ZYNQ対応 Create
 */
/************************************************************************************************/
VOID f_com_dpda_getAlmHistLog(	T_RRH_ALARM_HISTORYLOG_INFO		*pAlmHistLogInfo )
{
	T_RRH_ALM_LOG_FOR_HIST		*almHisCollect;
	UINT						logCnt;
	UINT						logPos;
	INT							ret;
	INT							errcd;
	
	/* Alarm History log収集準備Table共有メモリから情報取得 */
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_ALM_HIS_PREPARE,(VOID**)&almHisCollect,&errcd);
	
	if(ret != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Get shared memory failed [%d]", errcd);
		return;
	}
	
	/* 時系列に並べてアウトパラに返す */
	logPos = almHisCollect->dataPosi;
	for( logCnt=0; logCnt<D_RRH_ALMHIS_LOG_MEAS_NUM; logCnt++ )
	{
		memcpy( &(pAlmHistLogInfo[ logCnt ]),
				&(almHisCollect->logInfo[ logPos ]),
				sizeof( T_RRH_ALARM_HISTORYLOG_INFO ) );
		
		logPos ++;
		logPos %= D_RRH_ALMHIS_LOG_MEAS_NUM;
	}
	
	return;
}

/* @} */
