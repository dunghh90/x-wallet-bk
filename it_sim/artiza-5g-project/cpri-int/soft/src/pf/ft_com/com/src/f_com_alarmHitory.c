/*!
 * @skip  $ld:$
 * @file  f_com_alarmHitory.c
 * @brief Alarm History データ制御処理
 * @date  2011/01/10 COMAS) Nakajima Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2015
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_com_inc.h"
//#include "f_dpda_inc_slc.h"

/************************************************************************************************************/
/*!
 * @brief Alarm History Log 設定処理
 * @note  関数処理内容.
 *       -# セマフォ取得(E_COM_SEMID_KIND_ALMHISTLOG)
 *       -# 時刻を取得する。
 *       -# Alarm history logレコードを取得する
 *       -# 以下情報を設定する
 *       -# ・起動面のVersion(統合版数)
 *       -# ・カード状態情報（Alarm）
 *       -# ・測定情報(VUPUタスク)
 *       -# 管理情報更新
 *       -# セマフォ解放(E_COM_SEMID_KIND_ALMHISTLOG)
 *       -#
 * @param  factorType		[in] UINT  アラーム要因区分(HW/SW) D_RRH_ALARM_FACTOR_TYPE_HW/D_RRH_FACTOR_TYPE_SW
 * @param  swAlmCode		[in] UINT ソフトアラームのアラームコード
 * @param  hwAlarmFactor	[in] UINT ハードアラーム要因(Alarm要因)
 * @return void
 * @retval none
 *
 * @date 2011/01/11 COMAS)Nakajima Create
 * @date 2011/03/01 COMAS)Nakajima modify for MSO CM
 * @date 2011/05/16 COMAS)Hanaizumi modify M-RRU-ZSYS-00662
 * @date 2012/03/13 COMAS)Hanaizumi NSN2.1G
 * @date 2013/06/14 ALPHA)fujii B25
 * @date 2013/07/26 ALPHA)yokoyama B25
 * @date 2013/12/24 ALPHA)kondou zynq化対応
 * @date 2014/12/21 ALPHA)fujii modify for ppcEOL
 * @date 2015/04/27 ALPHA)fujii modify for TDD
 */
/************************************************************************************************************/
VOID f_com_alarmHitoryLogSet_com( UINT factorType, UINT swAlmCode, UINT *alarmFactor)
{
	T_RRH_ALARM_HISTORYMNG*		phistlogMng;
	T_RRH_ALARM_HISTORYLOG*		phistlog;		/* Alarm History Log pointer*/
	INT							errcd;
	INT							rtn_bpf;
	T_RRH_TRAINVTBL				*invTbl;
	USHORT						sw_ver;
	T_RRH_LOG_ASSERT*			assertLog;
	UINT						logIndex;
	UINT						logCnt;
	T_RRH_SYSTEM_TIME			systim;

	/****************************************************************************/
	/* Alarm History Log 設定                                                   */
	/****************************************************************************/
	/* 共有メモリアドレス取得(アラームヒストリーログ) */
	rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(	E_RRH_SHMID_APL_ALM_HISTORY_LOG,
											(VOID **)&phistlogMng,
											&errcd );
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
			"Alarm history log address get error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
		return;
	}

	/* 共有メモリアドレス取得(ソフトバージョン) */
	rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_INVENTORY, (VOID **)&invTbl, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
			"Software version get error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
		return;
	}
	if( invTbl->cmx_bootup_face == 0 )
	{/* 起動面が0の場合、0面のソフトバージョンを保存 */
		sw_ver = invTbl->cmx_f0_sver[0];
	}
	else
	{/* 起動面が1の場合、1面のソフトバージョンを保存 */
		sw_ver = invTbl->cmx_f1_sver[0];
	}

	/* 共有メモリアドレス取得(アサートログ) */
	rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(	E_RRH_SHMID_APL_ASSERT_LOG,
											(VOID **)&assertLog,
											&errcd );
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
			"Assert log get error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
		return;
	}

	/* OS時刻取得 */
	f_com_getSysTim(&systim);

	/* セマフォ取得 */
	rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_ALM_HISTORY_LOG,
		BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, NULL, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
			"Alarm history log sem take error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
	}

	/* INDEXが異常な場合はINDEXを0にする(フェールセーフ)	*/
	if( phistlogMng->alm_log_index >= D_RRH_ALMHIS_LOG_RECORD_NUM )
	{
		phistlogMng->alm_log_index = 0;
	}

	phistlog = &phistlogMng->histlog[ phistlogMng->alm_log_index ];

	/* OS時刻取得 */
	memcpy( (VOID *)&phistlog->tDate, (VOID *)&systim, sizeof( T_RRH_SYSTEM_TIME ) );

	/* Alarm要因 */
	phistlog->swAlarmFactor = swAlmCode;
	memcpy( phistlog->hwAlarmFactor, alarmFactor, sizeof(phistlog->hwAlarmFactor) );

	/* Version(統合版数) */
	phistlog->dlFile_ver = sw_ver;

	/* 要因 */
	phistlog->factorType = factorType;

	/* assert log情報 */
	if(assertLog->uiWriteIdx >= D_RRH_ALMHIS_AST_LOG_NUM)
	{
		logIndex = assertLog->uiWriteIdx - D_RRH_ALMHIS_AST_LOG_NUM;
	}
	else
	{
		logIndex = D_RRH_LOG_MAX_ASSERTLOGNUM - D_RRH_ALMHIS_AST_LOG_NUM + assertLog->uiWriteIdx;
	}
	for(logCnt=0; logCnt<D_RRH_ALMHIS_AST_LOG_NUM; logCnt++)
	{
		memcpy((VOID*)&(phistlog->astLog[logCnt]), (VOID*)&(assertLog->tRecord[logIndex]), sizeof(T_RRH_LOG_ASSERT_RECORD));
		logIndex++;
		if(logIndex >=  D_RRH_LOG_MAX_ASSERTLOGNUM)
		{
			logIndex = D_RRH_ZERO;
		}
	}

	/* dpda alarm history information */
	f_com_dpda_getAlmHistLog(phistlog->info); /* 5GDU_chg */

	/* 管理情報更新 */
	phistlogMng->alm_log_wrtflg = D_RRH_ON;		/* Alarm log write flag	*/
	phistlogMng->alm_log_index++;				/* Alarm log index      */

	/* Alarm history log レコード終端チェック */
	if (D_RRH_ALMHIS_LOG_RECORD_NUM <= phistlogMng->alm_log_index)
	{
		/* 先頭から格納する */
		phistlogMng->alm_log_index = 0;
		phistlogMng->uiCycleFlg = D_RRH_ON;
	}

    /* セマフォ解放 */
	rtn_bpf =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_ALM_HISTORY_LOG,
		BPF_RU_IPCM_LOCK_RW, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
			"Alarm history log sem give error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
	}

	return;
}

/************************************************************************************************************/
/*!
* @brief Alarm History Log 設定処理
 * @note  関数処理内容.
 *       -# Alarm History Log共通関数をcallする
 *       -#
 * @param  factorType		[in] UINT  アラーム要因区分(HW/SW/INIT) D_RRH_ALARM_FACTOR_TYPE_HW/D_RRH_FACTOR_TYPE_SW/D_RRH_ALARM_FACTOR_TYPE_INIT
 * @param  swAlmCode		[in] UINT  ソフトアラームのアラームコード
 * @param  hwAlarmFactor	[in] UINT* ハードアラーム要因(Alarm要因)
 * @return void
 * @retval none
 *
 * @date 2014/12/21 ALPHA)fujii modify for ppcEOL
 * @date 2015/04/27 ALPHA)fujii modify for TDD
 */
/************************************************************************************************************/
VOID f_com_alarmHitoryLogSet( UINT factorType, UINT swAlmCode, UINT *alarmFactor )
{
	UINT						hwAlmFactAry[E_RRH_ALMHIS_ALM_FACTOR_NUM];
	
	memset( hwAlmFactAry, 0x00, sizeof(hwAlmFactAry) );
	memcpy( (VOID *)hwAlmFactAry, alarmFactor, sizeof(hwAlmFactAry) );
	
	if(factorType == D_RRH_ALARM_FACTOR_TYPE_HW){
		if((hwAlmFactAry[3] & D_DU_SVCTL_BIT_OCCUR_FID_ALL) == 0){
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"Alarm history save log skip ,not found FaultID:0x%08x", hwAlmFactAry[3]);
			return;
		}
	}

	/* Alarm History Log設定共通関数をcall */
	f_com_alarmHitoryLogSet_com( factorType, swAlmCode, hwAlmFactAry );

	return;
}

/* @} */
