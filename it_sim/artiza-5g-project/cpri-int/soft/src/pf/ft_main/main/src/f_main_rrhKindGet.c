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
 * @param		
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
 * @date		2015/08/27 ALPHA) miyazaki modify to TDD-RRE-Zynq 
 */
UINT f_main_rrhKindGet( )
{

	INT							errcd			= 0;
	INT							rtn_bpf			= 0;
	UCHAR						device_kind0	= 0;
	UCHAR						device_kind1	= 0;
	UINT						*shm_rrhKind;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_rrhKindGet] ENTER" );

	/************************************************************/
	/* 装置種別取得 (From EEPROM)								*/
	/************************************************************/
	/* RE種別をEEPROMから取得。グローバル変数に格納。 */
	rtn_bpf = BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_DEVICE_KIND0, &device_kind0);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
	{
		/* Assert log出力	*/
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "REG Read Access Fail Device Kind0. rtn_bpf=%08x", rtn_bpf);

		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_EENG);
		return	D_SYS_NG;
	}

	/* 対応周波数帯読み込み */
	rtn_bpf = BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_DEVICE_KIND1, &device_kind1);
	if(BPF_HM_DEVC_COMPLETE != rtn_bpf)
	{
		/* Assert log出力	*/
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "REG Read Access Fail Device Kind1. rtn_bpf=%08x", rtn_bpf);

		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_EENG);
		return	D_SYS_NG;
	}

	/************************************************************/
	/* 装置種別チェック (NGの場合はabort、プロト機もabort)		*/
	/************************************************************/
// 擬似環境 装置種別判定はスキップ 本来は0x10が正しい 20210316
	device_kind0 = 0x10;
	device_kind1 = 0x00;
//	if( device_kind0 == E_RRH_RRHKIND_5GDU_LLS_SUB6 ) {

//		/* sub6の場合、4.5G/3.7G以外は未対応装置	*/
//		if( (device_kind1 != E_RRH_RRHKIND_FREQ_45G) && (device_kind1 != E_RRH_RRHKIND_FREQ_37G) ) {

//			/* Assert log出力	*/
//			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "DevFreq Error. DevKind=%02x, DevFreq=%02x", device_kind0, device_kind1);

//			/* Abort処理	*/
//			f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_DEVKIND_ABNORMAL);
//			return	D_SYS_NG;
//		}
//	} else if( device_kind0 == E_RRH_RRHKIND_5GDU_LLS_MMW ) {

//		/* mmWの場合、28G以外は未対応装置	*/
//		if( device_kind1 != E_RRH_RRHKIND_FREQ_28G ) {

//			/* Assert log出力	*/
//			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "DevFreq Error. DevKind=%02x, DevFreq=%02x", device_kind0, device_kind1);

//			/* Abort処理	*/
//			f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_DEVKIND_ABNORMAL);
//			return	D_SYS_NG;
//		}
//	} else {

//		/* sub6/mmW以外は未対応装置	*/
//		/* Assert log出力	*/
//		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "DevKind Error. DevKind=%02x, DevFreq=%02x", device_kind0, device_kind1);

//		/* Abort処理	*/
//		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_DEVKIND_ABNORMAL);
//		return	D_SYS_NG;
//	}
// 擬似環境  20210316

	/************************************************************/
	/* 装置種別設定 (For PF Processグローバル領域)				*/
	/************************************************************/
	f_comw_rrhKind = (UINT)((device_kind1 << D_DU_RRHKIND_FREQ_SHFT) | device_kind0);
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "DU kind=0x%08X", f_comw_rrhKind);

	/************************************************************/
	/* 装置種別設定 (For 共有メモリ)							*/
	/************************************************************/
	/* 装置種別を共有メモリに設定	*/
	rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(	E_RRH_SHMID_APL_RRH_KIND,
											(void**)&shm_rrhKind,
											&errcd );
	if( rtn_bpf != D_SYS_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", rtn_bpf);
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_FNG);
	}
	*shm_rrhKind = f_comw_rrhKind;

	UCHAR	start_flag = 0;
	USHORT	version_ofs = 0;
	UCHAR	version = 0;
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

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_rrhKindGet] RETURN" );
	return D_SYS_OK;

}
/* @} */
