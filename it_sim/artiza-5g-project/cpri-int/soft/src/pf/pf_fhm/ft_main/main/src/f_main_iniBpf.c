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

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_iniBpf] ENTER" );

	/************************************************************/
	/* プロセス管理機能割付け									*/
	/************************************************************/
	rtn_bpf = BPF_RM_SVRM_ASSIGN(D_RRH_PROCID_F_PF);
	if(BPF_RM_SVRM_COMPLETE != rtn_bpf)
	{
		rtn = D_SYS_NG;
	}

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_iniBpf] RETURN" );
	return(rtn);
}

/* @} */
