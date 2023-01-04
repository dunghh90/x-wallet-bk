/*!
 * @skip		$ld:$
 * @file		f_lgc_takeShelter.c
 * @brief		ログスレッド 24時間ログ退避処理
 * @author		ALPHA)yokoyama
 * @date		2013/11/04 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_LGC_LGC
 * @{
 */

#include "f_lgc_inc.h"

/*!
 * @brief		関数機能概要:ログ退避処理を行う
 * @note		関数処理内容.
 *				-# TO回数インクリメント
 *				-# 24時間経過した場合は以下の処理を行う
 *					-# EventLogWriteRequest送信
 *					-# EventLogEraseRequest送信
 *					-# 統計情報取得
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
 * @date		2018/08/24 KCN)若水 ログ退避処理を1時間に1回に変更
 */
INT f_lgc_takeShelter()
{
	T_RRH_LOG_LEVEL_MNG*		tLogLevelMng;
	INT							errcd;
	UCHAR						eep_alm_selfreset = 0;

	
	cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_lgc_takeShelter] ENTER" );
	
	/************************************************************/
	/* Log Levelを取得											*/
	/************************************************************/
	(VOID)BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_LOGLEVEL_MNG,
		(VOID **)&tLogLevelMng,
		&errcd );
	
	/* TO回数インクリメント */
	tLogLevelMng->cycleLogCount_cur++;
	if(tLogLevelMng->selfResetFlg == D_RRH_ON){
		tLogLevelMng->selfResetCount_cur++;
	}
	
	cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_INFO, "[f_lgc_takeShelter] COUNT LOG=0x%x SELF=0x%x(FLG:%d)",
								 tLogLevelMng->cycleLogCount_cur , tLogLevelMng->selfResetCount_cur, tLogLevelMng->selfResetFlg);
	
	/* 指定回数以上受けたらログ退避処理を行う	*/
	/* Default値は24時間(24回*1時間)である		*/
	if( tLogLevelMng->cycleLogCount_cur >= tLogLevelMng->cycleLogCount )
	{
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_INFO, "[f_lgc_takeShelter] CYCLIC LOG SAVE START");
	
		/* Writeを行ってからEraseを行う	*/
		f_lgc_send_EventLogReq( D_SYS_FLASH_WRITE );
		f_lgc_send_EventLogReq( D_SYS_FLASH_ERASE );
		
		/* TO回数クリア */
		tLogLevelMng->cycleLogCount_cur = 0;
	}

	/* 指定回数以上受けたら自律リセットフラグをクリアにする	*/
	/* Default値は48時間(48回*1時間)である		*/
	if((tLogLevelMng->selfResetFlg == D_RRH_ON) && (tLogLevelMng->selfResetCount_cur >= tLogLevelMng->selfResetCount)){
		eep_alm_selfreset = D_RRH_ALM_SELF_RESET_OFF;
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_ALM_SELF_RESET_FLG, &eep_alm_selfreset);
		tLogLevelMng->selfResetFlg = D_RRH_OFF;
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_WARNING, "ALM Self Reset FLG Clear(1h x %d reset:0x%x)" ,tLogLevelMng->selfResetCount_cur,f_cmw_tra_inv_tbl->almreset_flag[0]);
		f_cmw_tra_inv_tbl->almreset_flag[0] = eep_alm_selfreset;
	}
	cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_lgc_takeShelter] RETURN" );
	
	return D_RRH_OK;
}

/* @} */
