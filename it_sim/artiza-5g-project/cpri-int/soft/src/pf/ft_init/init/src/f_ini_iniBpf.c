/*!
 * @skip  $ld:$
 * @file  f_ini_iniBpf.c
 * @brief BPF初期化処理
 * @date  2013/03/16 ALPHA) kondou Create RRH-007-000 初期化
 * @data  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include "f_ini_inc.h"
#include "BPF_COM_END.h"
/*!
 * @brief		f_ini_iniBpf
 * @note 		関数処理内容.
 *					-# 共通プロセス管理機能割付け
 * @retval		D_INI_OK		0:正常終了
 * @retval		D_INI_NG		1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-Init-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2013/03/16 ALPHA)kondou Create RRH-007-000 初期化
 * @date 2013/11/07 ALPHA) nakamura modify for ZYNQ
 * @date 2014/01/28 ALPHA) nakajima modify for SRE
 * @data 2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 */
UINT f_ini_iniBpf(VOID* buf_p)
{
	UINT rtn = D_INI_OK;
	INT rtn_bpf = BPF_RM_SVRM_COMPLETE;
	UCHAR	start_flag = 0;
	USHORT	version_ofs = 0;
	UCHAR	version = 0;

	/************************************************************/
	/* プロセス管理機能割付け									*/
	/************************************************************/
	rtn_bpf = BPF_RM_SVRM_ASSIGN(D_RRH_PROCID_INI);
	if(BPF_RM_SVRM_COMPLETE != rtn_bpf)
	{
		rtn = D_INI_NG;
		return(rtn);
	}

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_INI,	/* ProcessID								*/
									D_SYS_THDID_INIT,	/* ThreadID									*/
									0 ,				/* ThreadQueueID:全てProcess間のため無し	*/
									(unsigned long int)pthread_self());

	(VOID)BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_START_FLG, &start_flag);
	if(start_flag == 0){
		version_ofs = D_DU_EEP_FSET1_FLASH0_VER;
	}else{
		version_ofs = D_DU_EEP_FSET1_FLASH1_VER;

	}
	(VOID)BPF_HM_DEVC_EEPROM_READ(version_ofs, &version);
	f_comw_rrhVersion = ((UINT)version) << 8;
	(VOID)BPF_HM_DEVC_EEPROM_READ(version_ofs+1,&version);
	f_comw_rrhVersion |= (UINT)version ;
	return(rtn);
}

/* @} */
