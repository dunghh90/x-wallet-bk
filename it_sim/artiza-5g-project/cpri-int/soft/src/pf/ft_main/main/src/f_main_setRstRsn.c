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
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-MAIN-007-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 * @data  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  		2015/06/09 ALPHA) tomioka modify RRH-007-000 TDD-Zynq対応
 * @date  		2015/07/16 ALPHA) ueda modify for M-RRU-ZSYS-01646対応
 */
UINT f_main_setRstRsn(UINT *resetReason)
{
	UINT				rtn = D_SYS_OK;
	INT					rtn_bpf = BPF_HM_DEVC_COMPLETE;
	UCHAR				resetCtlFlg = 0;
	UINT				addInfo=0;
	UCHAR				almreset_flag_old;
	UCHAR				almreset_flag_new;
	UINT				resetVal;
	CHAR*				resetReason_str[6] = {
											"Unknown",				/* 0 */
											"NORMAL Reset",			/* 1 */
											"CU RPC Reset",			/* 2 */
											"RU ALM self Reset",	/* 3 */
											"MP LINK DOWN Reset",	/* 4 */
											"MP SV WDT Reset"		/* 5 */
										  };
// 擬似環境 20210316
//#ifdef SIM_DEBUG
    printf("\n return func %s",__func__);
	return D_SYS_OK;//C9
//#endif
// 擬似環境 20210316
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_setRstRsn] ENTER" );

	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_OTH_RESET_REASON, &resetVal);

	almreset_flag_new = D_RRH_OFF;
	if((resetVal & D_DU_REG_OTH_RESET_BIT_POR) != 0){
		*resetReason = D_RRH_RST_RSN_POWER;

		BPF_COM_LOG_DMESG("Last Reset Reason : PowerON Reset(0x%08x)\n",resetVal);
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Last Reset Reason : PowerON Reset(0x%08x)",resetVal);

	}else{
		if((resetVal & D_DU_REG_OTH_RESET_BIT_SWDT) != 0){		/* SWDT T.O */
			*resetReason = D_RRH_RST_RSN_WDT;

			BPF_COM_LOG_DMESG("Last Reset Reason : SWDT(0x%08x)\n",resetVal);
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Last Reset Reason : SWDT(0x%08x)",resetVal);
			
		}else if((resetVal & D_DU_REG_OTH_RESET_BIT_SRST) != 0){	/* srst */
			*resetReason = D_RRH_RST_RSN_MMI;

			BPF_COM_LOG_DMESG("Last Reset Reason : SRST(0x%08x)\n",resetVal);
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Last Reset Reason : SRST(0x%08x)",resetVal);

		}else if((resetVal & D_DU_REG_OTH_RESET_BIT_SOFT) != 0){	/* soft */
			*resetReason = D_RRH_RST_RSN_SFT_FAIL;
			almreset_flag_new = D_RRH_ON;

			BPF_COM_LOG_DMESG("Last Reset Reason : SOFT Self RST(0x%08x)\n",resetVal);
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Last Reset Reason : SOFT Self RST(0x%08x)",resetVal);

		}else{
			*resetReason = D_RRH_RST_RSN_POWER;

			BPF_COM_LOG_DMESG("Last Reset Reason : Unknown(0x%08x)\n",resetVal);
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Last Reset Reason : Unknown(0x%08x)",resetVal);

		}
	}

	rtn_bpf = BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_ALM_SELF_RESET_FLG, &almreset_flag_old);
	if( almreset_flag_old != almreset_flag_new){
		rtn_bpf = BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_ALM_SELF_RESET_FLG, &almreset_flag_new);
	}
	f_cmw_tra_inv_tbl->almreset_flag[0] = almreset_flag_new;

	/************************************************************/
	/* EEPROMよりリセット制御フラグ(0x205)取得					*/
	/************************************************************/
	/* ソフトリセット有無フラグを取得 */
	rtn_bpf = BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_SOFT_RESET_FLG, &resetCtlFlg);
	if(BPF_HM_DEVC_COMPLETE == rtn_bpf)
	{
		switch(resetCtlFlg){
			case D_RRH_SOFT_RESET_NORMAL:
			case D_RRH_SOFT_RESET_CU:
			case D_RRH_SOFT_RESET_ALM:
			case D_RRH_SOFT_RESET_MPLINKDOWN:
			case D_RRH_SOFT_RESET_MPWDT:

				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"reset reason:%s(%d)",resetReason_str[resetCtlFlg],resetCtlFlg );

				/*	リセット制御フラグOn ⇒ リセット指示認識済み	*/
				/****************************************************************/
				/* EEPROMにリセット制御フラグ設定								*/
				/* ソフトリセットの場合は、リセット理由が共有メモリへ展開済み	*/
				/****************************************************************/
				resetCtlFlg = D_RRH_OFF;
				rtn_bpf = BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_SOFT_RESET_FLG, &resetCtlFlg);
				if(BPF_RM_SVRM_COMPLETE != rtn_bpf)
				{
					/*	Assert Log		*/
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"Reset Flag EEPROM Write Access Fail. rtn_bpf = %08x", rtn_bpf);
					rtn = D_SYS_NG;
				}
				BPF_COM_LOG_GET_RST_REASON(resetReason);
				break;
			default:
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"reset reason:%s(%d)",resetReason_str[0],resetCtlFlg );
				BPF_COM_LOG_RST_REASON( f_comw_rrhVersion, *resetReason, addInfo);
				break;
		}
	}
	else
	{
		/*	Assert Log		*/
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Reset Flag EEPROM Read Access Fail.");
		rtn = D_SYS_NG;
	}

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_setRstRsn] RETURN" );

	return(rtn);
}


/* @} */
