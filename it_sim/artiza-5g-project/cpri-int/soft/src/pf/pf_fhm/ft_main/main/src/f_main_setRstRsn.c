/*!
 * @skip		$ld:$
 * @file		f_main_setRstRsn.c
 * @brief		pf_main リセット要因設定処理
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 * @date  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  		2015/06/09 ALPHA) tomioka modify RRH-007-000 TDD-Zynq対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include "f_main_inc.h"

/*!
 * @brief		f_main_setRstRsn_tdd
 * @note		ハードRESET要因またはログから前回リセット理由を共有メモリ、FLASHに設定.
 * @param       regReadDat          [in ]   リセット要因レジスタ値
 * @param		*resetReason		[out]	リセット要因
 * @param		*errMsg				[out]	リセットメッセージ
 * @param       *almreset_off_flag  [out]   ALM reset off flag
 * @param       *addInfo            [out]   SIGUSR2 option information
 * 
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @FeatureID   PF-MAIN-007-001-001
 * @date		2013/11/11 alpha)中嶋
 * @date  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  		2015/06/09 ALPHA) tomioka modify RRH-007-000 TDD-Zynq対応
 * @date        2015/10/13 TDI)satou ハソ-QA-004 Local-RST追加
 * @date        2015/12/17 TDI) enoki 自律リセット保護解除されない問題対応
 */
UINT f_main_setRstRsn_tdd(UINT regReadDat, UINT *resetReason, CHAR *errMsg,UINT *almreset_off_flag, UINT *addInfo)
{
	UINT				rtn = D_SYS_OK;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_setRstRsn] ENTER" );
	
	switch(regReadDat)
	{
		case ( D_RRH_SLCR_RST_FACT_POR):
			/*	power-cycle-reset	*/
			*resetReason = D_RRH_RST_RSN_POWER;
			*addInfo = 0;
			strncpy(errMsg, "Power on reset", D_MAIN_MESSAGE_SIZE );
			*almreset_off_flag = D_RRH_TRUE;
			break;

		case ( D_RRH_SLCR_RST_FACT_RLBK):
			/*	ロールバック発生通知	*/
			*resetReason = D_RRH_RST_RSN_RLBK;
			*addInfo = D_RRH_SIGUSR2_OPT_RLBK;
			strncpy(errMsg, "Roll Back", D_MAIN_MESSAGE_SIZE );
			*almreset_off_flag = D_RRH_TRUE;
			break;

		case ( D_RRH_SLCR_RST_FACT_WDT):
			/* WDT T.O reset	*/
			*resetReason = D_RRH_RST_RSN_WDT;
			*addInfo = D_RRH_SIGUSR2_OPT_WDTTO;
			strncpy(errMsg, "WDT T.O reset", D_MAIN_MESSAGE_SIZE );
			break;

		case ( D_RRH_SLCR_RST_FACT_SWRST):
			/*	Software Reset	*/
			*resetReason = D_RRH_RST_RSN_SWRST1;
			*addInfo = 0;
			strncpy(errMsg, "Software Reset", D_MAIN_MESSAGE_SIZE );
			*almreset_off_flag = D_RRH_TRUE;
			break;

		case ( D_RRH_SLCR_RST_FACT_LOCAL_RST ):
		    /* Local-RST */
		    *resetReason = D_RRH_RST_RSN_RSTSW;
		    *addInfo = D_RRH_SIGUSR2_OPT_RSTSW;
		    strncpy(errMsg, "Local reset", D_MAIN_MESSAGE_SIZE);
			*almreset_off_flag = D_RRH_TRUE;
		    break;

		case ( D_RRH_SLCR_RST_FACT_L1RST1):
			/*	FPGA リクエスト要求(CPRI L1 RST)	*/
			*resetReason = D_RRH_RST_RSN_L1RST1;
			*addInfo = D_RRH_SIGUSR2_OPT_CPRIL1RST;
			strncpy(errMsg, "FPGA Request(CPRI L1 RST)", D_MAIN_MESSAGE_SIZE );
			*almreset_off_flag = D_RRH_TRUE;
			break;
		
		default:
            *resetReason = D_RRH_RST_RSN_UNKNWN;
			*almreset_off_flag = D_RRH_TRUE;
            strncpy(errMsg, "UNKNWN reset", D_MAIN_MESSAGE_SIZE );
            break;
	}
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_setRstRsn] RETURN" );

	return(rtn);
}

/*!
 * @brief		f_main_setRstRsn
 * @note		ハードRESET要因またはログから前回リセット理由を共有メモリ、FLASHに設定.
 *					-# EEPROMよりリセット制御フラグ読込
 *					-# EEPROMアクセス結果判定が正常の場合
 *						-# ハードRESET要因取得
 *						- リセット制御フラグOnの場合
 *							-# リセット制御フラグをOff設定後EEPROMへWriteする
 *						- リセット制御フラグOffの場合
 *							-# ハードのRESET要因判定
 *								- [0x20000000]がON: L1-RST発生後のリセット起動
 *								- [0x08000000]がON: Soft RST発生後のリセット起動
 *								- [0x04000000]がON: WDT T.O発生後のリセット起動
 *								- [0x02000000]がON: ロールバック発生後のリセット起動
 *								- [0x00400000]がON: パワーON RST起動
 *								- リセット要因が上記以外なら、MNT-SWをリセット理由に設定
 *								- 設定したリセット理由を共有メモリ、FLASHに設定
 *						-# 設定したログをFlashROMに退避する。
 *					-# EEPROMアクセス結果判定が異常の場合
 *						- ログ取得
 *					-# EEPROMアクセス処理結果の戻り値を返却
 * @param		*resetReason		[out]	リセット要因
 * @param		*errMsg				[out]	リセットメッセージ
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-MAIN-007-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 * @date  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  		2015/06/09 ALPHA) tomioka modify RRH-007-000 TDD-Zynq対応
 * @date        2015/07/16 ALPHA) ueda modify for M-RRU-ZSYS-01646対応
 * @date        2015/12/17 TDI) enoki 自律リセット保護解除されない問題対応
 */
UINT f_main_setRstRsn(UINT *resetReason, CHAR *errMsg)
{
	UINT				rtn = D_SYS_OK;
	INT					rtn_bpf = BPF_HM_DEVC_COMPLETE;
	UCHAR				resetCtlFlg = 0;
	UCHAR				selfRstFlg = 0;
	UCHAR				almreset_flag = 0;
	T_RRH_TRAINVTBL		*invTbl;
	INT					errcd;
	INT					endcd;
	UINT				almreset_off_flag = D_RRH_FALSE;
	UINT				*sw_ver;
	UINT				addInfo=0;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_setRstRsn] ENTER" );

	/************************************************************/
	/* EEPROMよりリセット制御フラグ(0x205)取得					*/
	/************************************************************/
	/* ソフトリセット有無フラグを取得 */
	rtn_bpf = BPF_HM_DEVC_EEPROM_READ(D_RRH_EEP_SOFT_RESET_FLG, &resetCtlFlg);
	/* ALM自律リセット有無フラグを取得 */
	rtn_bpf |= BPF_HM_DEVC_EEPROM_READ(D_RRH_EEP_ALM_SELF_RESET_FLG, &selfRstFlg);
	if(BPF_HM_DEVC_COMPLETE == rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"reset reason , resetCtlFlg=%d, selfRstFlg=%d",  resetCtlFlg, selfRstFlg );

		/* ソフトリセット実施の場合 */
		if((resetCtlFlg == D_RRH_SOFT_RESET_NORMAL) || (resetCtlFlg == D_RRH_SOFT_RESET_BOOTDWL))
		{
//自律リセット後のバージョン表示､リセット要因の書き換えはFHMでは不要
//			/* Boot DWL case */
//			if( resetCtlFlg == D_RRH_SOFT_RESET_BOOTDWL ){
//				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, " RestReason Boot DWL");
//				endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
//					E_RRH_SHMID_APL_SOFTWARE_VERSION,
//					(VOID **)&sw_ver,
//					&errcd
//				);
//				if( endcd != BPF_RU_IPCM_OK ){
//					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, " BPF_RU_IPCM_PROCSHM_ADDRGET error %08x " , errcd);
//				}
//				BPF_COM_LOG_RST_REASON( *sw_ver, D_RRH_RST_RSN_AUTORST, 0);
//			}
			/* FHM起動時のBOOT更新に失敗している場合、リセットフラグを更新しない */
			if( resetCtlFlg != D_RRH_SOFT_RESET_BOOTDWL ){
				/*	リセット制御フラグOn ⇒ リセット指示認識済み	*/
				/****************************************************************/
				/* EEPROMにリセット制御フラグ設定								*/
				/* ソフトリセットの場合は、リセット理由が共有メモリへ展開済み	*/
				/****************************************************************/
				resetCtlFlg = D_RRH_OFF;
				rtn_bpf = BPF_HM_DEVC_EEPROM_WRITE(D_RRH_EEP_SOFT_RESET_FLG, &resetCtlFlg);
				if(BPF_RM_SVRM_COMPLETE != rtn_bpf)
				{
					/*	Assert Log		*/
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"Reset Flag EEPROM Write Access Fail. rtn_bpf = %08x", rtn_bpf);
					rtn = D_SYS_NG;
				}
			}
			/* ALM自律リセットによる起動以外は自律リセットフラグをON（自律リセット保護は解除） */
			if( 0 == ( f_mainw_reboot_status & D_RRH_SLCR_RST_FACT_SWRST2 ))
			{
				/* 自律リセット以外の起動 */
				almreset_off_flag = D_RRH_TRUE;
			}
		
		}
		/* ハード自律リセットの場合 */
		else
		{
            /* リセット要因設定 */
            rtn = f_main_setRstRsn_tdd(f_mainw_reboot_status & D_RRH_SLCR_RST_FACT_MASK,resetReason,errMsg,&almreset_off_flag, &addInfo);

            BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Hard RestReason %s" , errMsg);

			/************************************************************/
			/* ソフトウェアバージョンを共有メモリから取得               */
			/************************************************************/
			/* 共有メモリ取得 */
			endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
				E_RRH_SHMID_APL_SOFTWARE_VERSION,
				(VOID **)&sw_ver,
				&errcd
			);

			if( endcd != BPF_RU_IPCM_OK )
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, " BPF_RU_IPCM_PROCSHM_ADDRGET error %08x " , errcd);
			}

			/************************************************************/
			/* リセット理由を共有メモリに保存							*/
			/************************************************************/
			BPF_COM_LOG_RST_REASON( *sw_ver, *resetReason, addInfo);
		}
	}
	else
	{
		/*	Assert Log		*/
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Reset Flag EEPROM Read Access Fail.");
		rtn = D_SYS_NG;
	}

	rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_INVENTORY, (VOID **)&invTbl, &errcd);
	if(BPF_RM_SVRM_COMPLETE == rtn_bpf)
	{
		/* リセット要因取得完了のため、制御フラグをOFFする */
		invTbl->softreset_flag[0] = D_RRH_OFF;
	}

	/* 自律リセットフラグがONであり、かつ自律リセットフラグの初期化フラグが有効時 */
	if( ( almreset_off_flag == D_RRH_TRUE ) && (invTbl->almreset_flag[0] == D_RRH_ALM_SELF_RESET_ON) )
	{

		/* 自律リセットフラグをOFFにする */
		invTbl->almreset_flag[0] = D_RRH_ALM_SELF_RESET_OFF;

		almreset_flag = D_RRH_ALM_SELF_RESET_OFF;
	
		rtn_bpf = BPF_HM_DEVC_EEPROM_WRITE(D_RRH_EEP_ALM_SELF_RESET_FLG, &almreset_flag);
		if(BPF_HM_DEVC_COMPLETE != rtn_bpf)
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "REG Write Access Fail Alm Self Reset Flag. rtn_bpf=%08x", rtn_bpf);
		}

		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Alm Reset Flag OFF.");
	}
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_setRstRsn] RETURN" );

	return(rtn);
}


/* @} */
