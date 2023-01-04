/*!
 * @skip		$ld:$
 * @file		f_main_rrhKindGet.c
 * @brief		pf_main 装置種別取得処理
 * @author		alpha)岡部
 * @date  		2014/12/19 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include "f_main_inc.h"

/*!
 * @brief		f_main_rrhKindGet
 * @note		装置種別取得処理
 * 		
 * @retval		D_SYS_OK	0:正常終了
 *				D_SYS_NG	0:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-MAIN-XXX-XXX-XXX
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  		2014/12/18 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 */
UINT f_main_rrhKindGet( )
{

	INT							errcd			= 0;
	INT							rtn_bpf			= 0;
	UCHAR						device_kind0	= 0;
	UINT						*shm_rrhKind;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_rrhKindGet] ENTER" );

	/************************************************************/
	/* 装置種別取得												*/
	/************************************************************/
	/* RE種別をEEPROMから取得。グローバル変数に格納。 */
	rtn_bpf = BPF_HM_DEVC_EEPROM_READ(D_RRH_EEP_DEVICE_KIND0, &device_kind0);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "REG Read Access Fail Device Kind0. rtn_bpf=%08x", rtn_bpf);
		f_com_abort(D_SYS_THDID_PF_F_MAIN, D_RRH_ALMCD_DNG);
	}
	f_comw_reKind = device_kind0;
	
	switch(f_comw_reKind)
	{
		case E_RRH_REKIND_FHM:
			f_comw_rrhKind = E_SYS_RRHKIND_SRE_35G;
			break;
		default :
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "unknown RE Kind RE=%d", f_comw_reKind);
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_RETURN,"[f_main_rrhKindGet] RETURN" );
			return D_SYS_NG;
			break;
	}

	/* 装置種別を共有メモリに設定	*/
	rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(	E_RRH_SHMID_APL_RRH_KIND,
											(void**)&shm_rrhKind,
											&errcd );

	if( rtn_bpf != D_SYS_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", rtn_bpf);
		f_com_abort(D_SYS_THDID_PF_F_MAIN, D_RRH_ALMCD_FNG);
	}

	*shm_rrhKind = f_comw_rrhKind;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_rrhKindGet] RETURN" );
	return D_SYS_OK;

}
/* @} */
