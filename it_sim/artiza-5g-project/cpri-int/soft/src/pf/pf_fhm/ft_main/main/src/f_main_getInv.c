/*!
 * @skip		$ld:$
 * @file		f_main_getInv.c
 * @brief		pf_main 装置構成情報取得処理
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 * @date  		2014/12/24 ALPHA) okabe Create RRH-013-000 共通ppc対応
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
/*!
 * @brief		f_main_getInv
 * @note		EEPROMよりデータ取得し共有メモリへ展開.
 *					-# EEPROMよりデータ取得
 *						- EEPROMアクセス正常終了
 *							-# 取得データを共有メモリへ展開
 *						- EEPROMアクセス異常終了
 *							-# ログを取得
 *							-# アクセスNG時はアラーム制御を実施(EEPROMアクセス異常)
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
 * @date  		2014/12/24 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  		2015/06/08 ALPHA) tomioka Create RRH-007-000 TDD-Zynq対応
 */
UINT f_main_getInv()
{
	UINT rtn = D_SYS_OK;					/*	関数戻り値				*/
	INT rtn_bpf = 0;
	UCHAR *shmem = NULL;
	INT errcd = 0;
	INT eepCnt = 0;
	UCHAR invData;
	UCHAR *invMng = NULL;
	UCHAR invBuff[D_RRH_I2C_EEP_DATANUM];

	UCHAR		buffer[128];
	UCHAR		writebuffer[128];
	FILE*		fpVersion;
	USHORT		*shortBuff;
	UINT		uiCnt;
	size_t		fw_cnt;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_getInv] ENTER" );

	/************************************************************/
	/* EEPROMよりテーブルへデータ取得							*/
	/************************************************************/
	memset(invBuff, 0, sizeof(invBuff));
	invMng = (UCHAR *)invBuff;

	/* EEPROMデータ取得 */
	for(eepCnt=0; eepCnt<D_RRH_I2C_EEP_DATANUM ; eepCnt++)
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

	/* 00 or 01以外の場合は0面起動と判断 */
	if (( invBuff[ D_RRH_EEP_START_FLG_ENA ] != D_RRH_BOOT_ACT0 ) && ( invBuff[ D_RRH_EEP_START_FLG_ENA ] != D_RRH_BOOT_ACT1 ))
	{
		invBuff[ D_RRH_EEP_START_FLG_ENA ] = D_RRH_BOOT_ACT0;
	}
	
	/* テーブルへ設定 */
	memcpy(&f_mainw_inv_mng, invBuff, sizeof(f_mainw_inv_mng));

	/* USHORT型データのエンディアン変換 */
	shortBuff = (USHORT *)&f_mainw_inv_mng;
	for(uiCnt=0; uiCnt<(D_RRH_I2C_EEP_DATANUM/2); uiCnt++)
	{
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

	/* RE side TRX-INF type は EEPROMの値を値を設定する */
	f_cmw_re_dev_tbl->cmx_trxinf_typ = f_cmw_tra_inv_tbl->cmx_tra_inf_typ;

	/* セマフォ解放 */
	rtn_bpf =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_INVENTORY, BPF_RU_IPCM_LOCK_RW, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Inventory sem give rtn_bpf=%08x", rtn_bpf);
		rtn = D_SYS_NG;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_getInv] RETURN" );
		return(rtn);
	}

	/************************************************************************/
	/* Version情報取得(APLのみ)												*/
	/* Version情報出力コマンドを叩くたびにFlashアクセスすると				*/
	/* Flashの劣化を早めてしまうため、ここでファイルに出力する				*/
	/* ただし、TX-FPGA/RX-FPGA/CPLDなどはRESET解除を行わないと				*/
	/* 取得できないため、ここではAPLのみ取得する							*/
	/************************************************************************/
	memset(buffer, 0 , sizeof(buffer));
	memset(writebuffer, 0 , sizeof(writebuffer));
	rtn_bpf = BPF_HM_DEVC_QSPI_FLASH_READ_BOOT_VER( 0, D_RRH_QSPI_FLASH_OFFSET_BOOT, (UCHAR *)buffer );
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Can't get boot ver. rtn_bpf=%08x", rtn_bpf);		
		return D_SYS_NG;
	}
	
	fpVersion = fopen( "/var/log/version" ,"wb");
	if( fpVersion == 0 )
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Can't open version file. rtn_bpf=%08x", rtn_bpf);
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_getInv] RETURN" );
		return D_SYS_NG;
	}

	fseek(fpVersion,0,SEEK_SET);

	snprintf((CHAR *)writebuffer, sizeof(writebuffer), "Boot  ver      : %s\n", buffer);
	fw_cnt = fwrite(writebuffer, 1, strlen((CHAR *)writebuffer), fpVersion );
	if(fw_cnt < strlen((CHAR *)writebuffer))
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "fwrite error. fw_cnt=%08x", fw_cnt);
	}
	memset(writebuffer, 0 , sizeof(writebuffer));

	memset(buffer, 0 , sizeof(buffer));

	rtn_bpf = BPF_HM_DEVC_QSPI_FLASH_READ(	invBuff[ D_RRH_EEP_START_FLG_ENA ],
						D_MAIN_INV_UBOOT_FLASH_OFFSET2,
						D_MAIN_INV_UBOOT_FLASH_SIZE3,
						buffer );
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Can't access QSPI flash 3rd. rtn_bpf=%08x", rtn_bpf);
		fclose(fpVersion);
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_getInv] RETURN" );
		return D_SYS_NG;
	}

	snprintf((CHAR *)writebuffer, sizeof(writebuffer), "APL total ver  : %02x%02x%02x%02x\n", buffer[4],buffer[5],buffer[6],buffer[7]);
	fw_cnt = 	fwrite(writebuffer, 1, strlen((CHAR *)writebuffer), fpVersion );
	if(fw_cnt < strlen((CHAR *)writebuffer))
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "fwrite error. fw_cnt=%08x", fw_cnt);
	}
	memset(writebuffer, 0 , sizeof(writebuffer));

	snprintf((CHAR *)writebuffer, sizeof(writebuffer), "  - Kernel ver : %02x%02x%02x%02x\n", buffer[44],buffer[45],buffer[46],buffer[47]);
	fw_cnt = 	fwrite(writebuffer, 1, strlen((CHAR *)writebuffer), fpVersion );
	if(fw_cnt < strlen((CHAR *)writebuffer))
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "fwrite error. fw_cnt=%08x", fw_cnt);
	}
	memset(writebuffer, 0 , sizeof(writebuffer));

	snprintf((CHAR *)writebuffer, sizeof(writebuffer), "  - DevTree ver: %02x%02x%02x%02x\n", buffer[60],buffer[61],buffer[62],buffer[63]);
	fw_cnt = 	fwrite(writebuffer, 1, strlen((CHAR *)writebuffer), fpVersion );
	if(fw_cnt < strlen((CHAR *)writebuffer))
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "fwrite error. fw_cnt=%08x", fw_cnt);
	}
	memset(writebuffer, 0 , sizeof(writebuffer));

	snprintf((CHAR *)writebuffer, sizeof(writebuffer), "  - Ramdisk ver: %02x%02x%02x%02x\n", buffer[76],buffer[77],buffer[78],buffer[79]);
	fw_cnt = 	fwrite(writebuffer, 1, strlen((CHAR *)writebuffer), fpVersion );
	if(fw_cnt < strlen((CHAR *)writebuffer))
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "fwrite error. fw_cnt=%08x", fw_cnt);
	}

	fclose(fpVersion);

	/* APLver/統合バージョンをAssert表示する	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "******** Boot up %d Soft ver:%02x%02x%02x%02x File ver:%02x%02x %02x%02x/%02x/%02x ********",
										invBuff[D_RRH_EEP_START_FLG_ENA],
										buffer[4],buffer[5],buffer[6],buffer[7],
										( invBuff[D_RRH_EEP_START_FLG_ENA] == 0 )? invBuff[D_RRH_EEP_FLASH0_SOFT_VER]    : invBuff[D_RRH_EEP_FLASH1_SOFT_VER],
										( invBuff[D_RRH_EEP_START_FLG_ENA] == 0 )? invBuff[D_RRH_EEP_FLASH0_SOFT_VER + 1]    : invBuff[D_RRH_EEP_FLASH1_SOFT_VER + 1],
										( invBuff[D_RRH_EEP_START_FLG_ENA] == 0 )? invBuff[D_RRH_EEP_FLASH0_SOFT_YEAR]   : invBuff[D_RRH_EEP_FLASH1_SOFT_YEAR],
										( invBuff[D_RRH_EEP_START_FLG_ENA] == 0 )? invBuff[D_RRH_EEP_FLASH0_SOFT_YEAR + 1]   : invBuff[D_RRH_EEP_FLASH1_SOFT_YEAR + 1],
										( invBuff[D_RRH_EEP_START_FLG_ENA] == 0 )? invBuff[D_RRH_EEP_FLASH0_SOFT_MONDAY] : invBuff[D_RRH_EEP_FLASH1_SOFT_MONDAY],
										( invBuff[D_RRH_EEP_START_FLG_ENA] == 0 )? invBuff[D_RRH_EEP_FLASH0_SOFT_MONDAY + 1] : invBuff[D_RRH_EEP_FLASH1_SOFT_MONDAY + 1]
									);

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_getInv] RETURN" );
	return(rtn);
}

/* @} */
