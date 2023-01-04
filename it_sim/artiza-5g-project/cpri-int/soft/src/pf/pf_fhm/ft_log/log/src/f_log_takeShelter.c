/*!
 * @skip		$ld:$
 * @file		f_log_takeShelter.c
 * @brief		ログスレッド 24時間ログ退避処理
 * @author		ALPHA)yokoyama
 * @date		2013/11/04 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_LOG_LOG
 * @{
 */

#include "f_log_inc.h"
UINT	wdtRestartFlg = D_RRH_OFF;

/*!
 * @brief		関数機能概要:ログ退避処理を行う
 * @note		関数処理内容.
 *				-# TO回数インクリメント
 *				-# 24時間経過した場合は以下の処理を行う
 *					-# EventLogWriteRequest送信
 *					-# EventLogEraseRequest送信
 *					-# TO回数をクリアする
 *				-# ログ情報退避用タイマ開始
 * @retval		D_RRH_OK		0:正常終了
 * @retval		D_RRH_NG		1:異常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-002-002-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/10/29 ALPHA)横山 Create
 * @date		2016/06/30 FJT)Koshida modify for WDT alarm effective by exceeding it for 48 hours
 */
INT f_log_takeShelter()
{
	T_RRH_LOG_LEVEL_MNG*		tLogLevelMng;
	INT							errcd;
	UCHAR						eep_alm_selfreset = D_RRH_ALM_SELF_RESET_OFF;
	struct timespec rstBaseTim;                                 /* 時刻 */
	
	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_log_takeShelter] ENTER" );
	
	/************************************************************/
	/* Log Levelを取得											*/
	/************************************************************/
	(VOID)BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_LOGLEVEL_MNG,
		(VOID **)&tLogLevelMng,
		&errcd );
	
	/* TO回数インクリメント */
	f_logw_logShelterTimCount++;
	
	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_INFO, "[f_log_takeShelter] CYCLIC LOG COUNT=%d", f_logw_logShelterTimCount );
	
	/* 指定回数以上受けたらログ退避処理を行う	*/
	/* Default値は24時間(12回*2時間)である		*/
	if( f_logw_logShelterTimCount >= tLogLevelMng->cycleLogCount )
	{
		cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_INFO, "[f_log_takeShelter] CYCLIC LOG SAVE START");
	
		/* Writeを行ってからEraseを行う	*/
		f_log_send_EventLogReq( D_SYS_FLASH_WRITE );
		f_log_send_EventLogReq( D_SYS_FLASH_ERASE );
		
		/* TO回数クリア */
		f_logw_logShelterTimCount = 0;
	}
	/********************************************************/
	/*	自律リセット起動から48時間経過したら、WDTを再開する	*/
	/********************************************************/
	/*	WDTリスタート判定が必要の場合	*/
	if(wdtRestartFlg == D_RRH_OFF)
	{
		/*	自律リセットフラグを取得	*/
		(VOID)BPF_HM_DEVC_EEPROM_READ(D_RRH_EEP_ALM_SELF_RESET_FLG, &eep_alm_selfreset);
		/*	自律リセットフラグをONの場合	*/
		if( eep_alm_selfreset == D_RRH_ALM_SELF_RESET_ON )
		{
			/* 現在時刻情報の取得 */
			(void)clock_gettime( CLOCK_MONOTONIC_RAW   , &rstBaseTim);	/* 時刻取得(bootup時刻) */
			/*	48時間以上の場合	*/
			if(rstBaseTim.tv_sec >= D_RRH_ALM_PROTECT_TIM)
			{
				/*	48時間以上の場合、EEPROMの値に関係なく、自律リセット有効であるため	*/
				/*	EEPROMやインベントリテーブルは更新せず、WDTのみ再起動する			*/
				(VOID)BPF_HM_DEVC_WDT_SET( D_SYS_AXI_TIMER_SET, D_SYS_SWDT_TIMER_SET );
				/* 自律リセットフラグをOFFする */
				eep_alm_selfreset = D_RRH_ALM_SELF_RESET_OFF;
				(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_RRH_EEP_ALM_SELF_RESET_FLG, &eep_alm_selfreset);
				cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_WARNING, "WDT restart for 48h over (flg=%d, time=%d)" ,eep_alm_selfreset, rstBaseTim.tv_sec);
				/*	WDTリスタート不要を設定	*/
				wdtRestartFlg = D_RRH_ON;
			}
		}
		/*	自律リセットフラグがOFFの場合	*/
		else
		{
			/*	WDTリスタート不要を設定	*/
			cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_INFO, "WDT restart not need (flg=%d)" ,eep_alm_selfreset);
			wdtRestartFlg = D_RRH_ON;
		}
	}
	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_log_takeShelter] RETURN" );
	
	return D_RRH_OK;
}

/* @} */
