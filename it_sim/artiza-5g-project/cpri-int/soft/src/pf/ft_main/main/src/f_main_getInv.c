/*!
 * @skip		$ld:$
 * @file		f_main_getInv.c
 * @brief		pf_main 装置構成情報取得処理
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 * @data  		2014/12/24 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  		2015/06/08 ALPHA) tomioka Create RRH-007-000 TDD-Zynq対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include "f_main_inc.h"
#include "BPF_COM_CMD.h"
/*!
 * @brief		f_main_getInv
 * @note		EEPROMよりデータ取得し共有メモリへ展開.
 *					-# EEPROMよりデータ取得
 *						- EEPROMアクセス正常終了
 *							-# 取得データを共有メモリへ展開
 *						- EEPROMアクセス異常終了
 *							-# ログを取得
 *							-# アクセスNG時はアラーム制御を実施(EEPROMアクセス異常)
 *					-# SFPからvendor spec情報を読み出し下記設定を実施
 *						-# 装置構成情報TRX-INFタイプを設定(f_comw_sfpType, f_comw_sfpTypeSecondaryにも設定)
 *						-# カード品名名称を設定
 *					-# Version情報を取得&展開する
 *					-# 処理結果を返却する。
 * @param		-
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-MAIN-006-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 * @data  		2014/12/24 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  		2015/06/08 ALPHA) tomioka Create RRH-007-000 TDD-Zynq対応
 * @date		2015/08/27 ALPHA) miyazaki modify to TDD-RRE-Zynq 
 * @date		2015/10/28 ALPHA) miyazaki ハソ1.13対応
 */
UINT f_main_getInv()
{

	UINT rtn = D_SYS_OK;					/*	関数戻り値				*/
	INT rtn_bpf = 0;
	UCHAR *shmem = NULL;
	INT errcd = 0;
	UCHAR invBuff[D_RRH_I2C_EEP_DATANUM/2];
	UCHAR	eep_data;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_getInv] ENTER" );

	/************************************************************/
	/* EEPROMよりテーブルへデータ取得							*/
	/************************************************************/
	memset(invBuff, 0, sizeof(invBuff));

	UCHAR invData;
	INT eepCnt = 0;
	UCHAR *invMng = NULL;
	invMng = (UCHAR *)invBuff;

	/* EEPROMデータ取得 */
	for(eepCnt=0; eepCnt<D_RRH_I2C_EEP_DATANUM/2 ; eepCnt++)
	{
		rtn_bpf = BPF_HM_DEVC_EEPROM_READ(eepCnt, &invData);
		if(BPF_HM_DEVC_COMPLETE != rtn_bpf)
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "EEPROM Inventory data read rtn_bpf=%08x", rtn_bpf);
			rtn = D_SYS_NG;
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_getInv] RETURN" );
			return(rtn);
		}
		*(invMng + eepCnt) = invData;
	}

	/* EEPROMアドレス0x372の値が6以下の場合はEEPROM、共有メモりともに値を7に置き換える */
	if(invBuff[D_DU_EEP_VID_ACPT_CLK_CLASS] <= 6)
	{
		invBuff[D_DU_EEP_VID_ACPT_CLK_CLASS] = 7;
		rtn_bpf = BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_VID_ACPT_CLK_CLASS, &invBuff[D_DU_EEP_VID_ACPT_CLK_CLASS]);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
		{
			/* エラー */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "EEP-ROM Write Error(%04x) rtn_bpf=%08x", D_DU_EEP_VID_ACPT_CLK_CLASS, rtn_bpf);
		}
	}

	/* 00 or 01以外の場合は0面起動と判断 */
	if (( invBuff[ D_DU_EEP_START_FLG ] != D_RRH_BOOT_ACT0 ) && ( invBuff[ D_DU_EEP_START_FLG ] != D_RRH_BOOT_ACT1 ))
	{
		invBuff[ D_DU_EEP_START_FLG ] = D_RRH_BOOT_ACT1;
	}

	/* 起動面設定 */
	BPF_COM_CMD_SET_CUR_START_FACE((UINT)invBuff[ D_DU_EEP_START_FLG ]);

	/* テーブルへ設定 */
	memcpy(&f_mainw_inv_mng, invBuff, sizeof(f_mainw_inv_mng));

/* TODO */
	USHORT		*shortBuff;
	UINT		uiCnt;
	/* USHORT型データのエンディアン変換 */
	shortBuff = (USHORT *)&f_mainw_inv_mng;
	for(uiCnt=0; uiCnt<(D_RRH_I2C_EEP_DATANUM/4); uiCnt++)
	{
		/* CHAR:0 SHORT:1 */
		/* SHORTの場合 */
		if(f_mainr_invShortInfo[uiCnt] != 0)
		{
			BPF_COM_END_BIGTOLITTLE16(*(shortBuff+uiCnt));
		}
	}

	/* 共有メモリアドレス取得 */
	rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_INVENTORY, (VOID **)&shmem, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Inventory sh memory address rtn_bpf=%08x", rtn_bpf);
		rtn = D_SYS_NG;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_getInv] RETURN" );
		return(rtn);
	}

	/* セマフォ取得 */
	rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_INVENTORY, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, NULL, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Inventory sem take rtn_bpf=%08x", rtn_bpf);
		rtn = D_SYS_NG;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_getInv] RETURN" );
		return(rtn);
	}

	/* テーブルから共有メモリへ展開 */
	memcpy(shmem, (VOID *)&f_mainw_inv_mng, sizeof(f_mainw_inv_mng));

	/* 共有メモリのハードウェアバージョン書き換え	*/
	if( M_DU_RRHKIND_KIND_GET(f_comw_rrhKind) == D_DU_EEP_DEV_KIND_5GDU_SUB6 ) {	/* sub6	*/

		/* FAN回転数モードチェック	*/
		if( f_cmw_tra_inv_tbl->cmx_fan_spin_mode != D_RRH_ON ) {	/* 通常モード	*/

			/* ハードバージョンの1byteに0x00:通常FAN,旧Beamを設定	*/
			f_cmw_tra_inv_tbl->cmx_hver1 = (f_cmw_tra_inv_tbl->cmx_hver1 & D_DU_EEP_HARD_VER_MASK) | D_DU_EEP_FANNML_BFOLD;
		} else {

			/* ハードバージョンの1byteに0x01:低速FAN,旧Beamを設定	*/
			f_cmw_tra_inv_tbl->cmx_hver1 = (f_cmw_tra_inv_tbl->cmx_hver1 & D_DU_EEP_HARD_VER_MASK) | D_DU_EEP_FANLOW_BFOLD;
		}
	} else {	/* mmW	*/

		/* mmW BeamID 新旧モードチェック	*/
		if( f_cmw_tra_inv_tbl->cmx_bf_envelope_mode != D_RRH_ON ) {	/* 旧Beam動作モード	*/

			/* FAN回転数モードチェック	*/
			if( f_cmw_tra_inv_tbl->cmx_fan_spin_mode != D_RRH_ON ) {	/* 通常モード	*/

				/* ハードバージョンの1byteに0x00:通常FAN,旧Beamを設定	*/
				f_cmw_tra_inv_tbl->cmx_hver1 = (f_cmw_tra_inv_tbl->cmx_hver1 & D_DU_EEP_HARD_VER_MASK) | D_DU_EEP_FANNML_BFOLD;
			} else {

				/* ハードバージョンの1byteに0x01:低速FAN,旧Beamを設定	*/
				f_cmw_tra_inv_tbl->cmx_hver1 = (f_cmw_tra_inv_tbl->cmx_hver1 & D_DU_EEP_HARD_VER_MASK) | D_DU_EEP_FANLOW_BFOLD;
			}

		} else {

			/* FAN回転数モードチェック	*/
			if( f_cmw_tra_inv_tbl->cmx_fan_spin_mode != D_RRH_ON ) {	/* 通常モード	*/

				/* ハードバージョンの1byteに0x08:通常FAN,新Beamを設定	*/
				f_cmw_tra_inv_tbl->cmx_hver1 = (f_cmw_tra_inv_tbl->cmx_hver1 & D_DU_EEP_HARD_VER_MASK) | D_DU_EEP_FANNML_BFNEW;
			} else {

				/* ハードバージョンの1byteに0x09:低速FAN,新Beamを設定	*/
				f_cmw_tra_inv_tbl->cmx_hver1 = (f_cmw_tra_inv_tbl->cmx_hver1 & D_DU_EEP_HARD_VER_MASK) | D_DU_EEP_FANLOW_BFNEW;
			}
		}
	}
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Inventory chg hard ver %02x%02x", 
						(CHAR)(f_cmw_tra_inv_tbl->cmx_hver1 & 0x00FF), 
						(CHAR)((f_cmw_tra_inv_tbl->cmx_hver1 & 0xFF00) >> 8) );

	/* セマフォ解放 */
	rtn_bpf =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_INVENTORY, BPF_RU_IPCM_LOCK_RW, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Inventory sem give rtn_bpf=%08x", rtn_bpf);
		rtn = D_SYS_NG;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_getInv] RETURN" );
		return(rtn);
	}

	/* フェールセーフABカウンタ初期値チェック */
	if(f_cmw_tra_inv_tbl->cmx_sfp_failsafe_ab_cnt == 0xff)
	{
		eep_data = 0;
		rtn_bpf = BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_SFP_FAILSAFE_AB_CNT, &eep_data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
		{
			/* エラー */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "EEP-ROM Write Error(%04x) rtn_bpf=%08x", D_DU_EEP_SFP_FAILSAFE_AB_CNT, rtn_bpf);
		}
		f_cmw_tra_inv_tbl->cmx_sfp_failsafe_ab_cnt = eep_data;
	}
	/* フェールセーフABCDカウンタ初期値チェック */
	if(f_cmw_tra_inv_tbl->cmx_sfp_failsafe_abcd_cnt == 0xff)
	{
		eep_data = 0;
		rtn_bpf = BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_SFP_FAILSAFE_ABCD_CNT, &eep_data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
		{
			/* エラー */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "EEP-ROM Write Error(%04x) rtn_bpf=%08x", D_DU_EEP_SFP_FAILSAFE_ABCD_CNT, rtn_bpf);
		}
		f_cmw_tra_inv_tbl->cmx_sfp_failsafe_abcd_cnt = eep_data;
	}

	/* 共有メモリアドレス取得 */
	rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_TRAINING_DATA, (VOID **)&f_comw_trainingData, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "TRAINING_DATA sh memory address rtn_bpf=%08x", rtn_bpf);
		rtn = D_SYS_NG;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_getInv] RETURN" );
		return(rtn);
	}

	/* RF用 共有メモリアドレス取得 */ /* 5GDU_chg */
	rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_RF_TRAINING_DATA, (VOID **)&f_comw_rftrainingData, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "RF TRAINING_DATA sh memory address rtn_bpf=%08x", rtn_bpf);
		rtn = D_SYS_NG;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_getInv] RETURN" );
		return(rtn);
	}

	/* トレーニング領域およびvirtual領域をall Fで初期化 */
	memset(f_comw_trainingData,0xFF,sizeof(T_RRH_TRAINING_DATA));
	memset(f_comw_rftrainingData,0xFF,sizeof(T_RRH_RF_TRAINING_DATA));

	UINT  trainofs;
	UINT  rf_trainval;
	UCHAR trainval = 0;
	UCHAR device_kind0 = 0;

	/* sub6、mmW共通の処理 */
	for(eepCnt = D_DU_I2C_EEP_TR_OFS,trainofs = 0 ;
		eepCnt < (D_DU_I2C_EEP_TR_OFS + D_RRH_TRAING_DATA_SIZE) ;
		eepCnt++ ,trainofs++){

		rtn_bpf = BPF_HM_DEVC_EEPROM_READ(eepCnt, &trainval);
		if(BPF_HM_DEVC_COMPLETE != rtn_bpf)
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "EEPROM training data read rtn_bpf=%08x", rtn_bpf);
			rtn = D_SYS_NG;
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_getInv] RETURN" );
			return(rtn);
		}
		f_comw_trainingData->traVal[trainofs] = trainval;
	}


	(VOID)BPF_HM_DEVC_EEPROM_READ(D_RRH_EEP_DEVICE_KIND0, &device_kind0);
	switch(device_kind0){
		case D_RRH_EEP_DEV_KIND_5GDU_SUB6:
			/* sub6の場合は取得済のため処理なし */
			break;
		case D_RRH_EEP_DEV_KIND_5GDU_MMW:
			/* mmwの場合は RF-EEPROMからトレーニングデータ取得 */
			for(eepCnt = D_DU_I2C_RF_EEP_TR_OFS,trainofs = 0 ; 
				eepCnt < (D_DU_I2C_RF_EEP_TR_OFS + D_RRH_RF_TRAING_DATA_SIZE) ; 
				eepCnt++ ,trainofs++){

				rtn_bpf = BPF_HM_DEVC_RFEEP_READ(eepCnt, &rf_trainval);
				if(BPF_HM_DEVC_COMPLETE != rtn_bpf){
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "RF-EEPROM data read rtn_bpf=%08x", rtn_bpf);
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_getInv] RETURN" );
					rtn = D_SYS_OK;
					break;		/* 未実装の場合があるのでskip */
				}
				f_comw_rftrainingData->traVal[trainofs] = (UCHAR)rf_trainval;
			}
			break;
		default:
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "devkind error %d",device_kind0 );
			break;
	}

	/* MT情報更新 */
	f_com_mtCtl_init();

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_getInv] RETURN" );
	return(rtn);
}

/* @} */
