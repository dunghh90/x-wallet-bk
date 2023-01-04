/*!
 * @skip		$ld:$
 * @file		f_main_iniBpf.c
 * @brief		pf_main BPF初期化処理
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include "f_main_inc.h"
#include <errno.h>

/*!
 * @brief		f_main_iniBpf
 * @note		BPFの初期設定を行う.
 *					-# プロセス管理機能割付け.
 *					- BPF_RM_SVRM_COMPLETE以外の場合.
 *						-# D_SYS_NGを返却し処理終了.
 *					- BPF_RM_SVRM_COMPLETEの場合.
 *						-# D_SYS_OKを返却し処理終了.
 * @param		-
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-MAIN-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 */
UINT f_main_iniBpf()
{
	UINT rtn = D_SYS_OK;
	INT rtn_bpf = BPF_RM_SVRM_COMPLETE;
	UINT	regw_data;
	UCHAR	device_kind0 = 0;

	/************************************************************/
	/* プロセス管理機能割付け									*/
	/************************************************************/
	rtn_bpf = BPF_RM_SVRM_ASSIGN(D_RRH_PROCID_PF);
	if(BPF_RM_SVRM_COMPLETE != rtn_bpf)
	{
		rtn = D_SYS_NG;
	}
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_iniBpf] ENTER %d",rtn );
	/* mmWの場合、I2C(SoftMacro) init を実施する */
	(VOID)BPF_HM_DEVC_EEPROM_READ(D_RRH_EEP_DEVICE_KIND0, &device_kind0);
	if(device_kind0 == D_RRH_EEP_DEV_KIND_5GDU_MMW){
		/* I2C(SoftMacro) init */
		regw_data = 0x0000000F;
		BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, 0xA0030120, &regw_data );
	}
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_iniBpf] RETURN" );
	return(rtn);
}


/*!
 * @brief		f_main_iniBpf_ptp
 * @note		BPFの初期設定を行う.
 *					-# プロセス管理機能割付け.
 * @param		-
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-MAIN-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/4/27 Taniguchi
 */
UINT f_main_iniBpf_ptp()
{
	UINT rtn = D_SYS_OK;
	INT rtn_bpf = BPF_RM_SVRM_COMPLETE;

	rtn_bpf = BPF_RM_SVRM_ASSIGN(D_RRH_PROCID_PTP);
	if(BPF_RM_SVRM_COMPLETE != rtn_bpf) {

		rtn = D_SYS_NG;
	}
	return(rtn);
}
/* @} */
