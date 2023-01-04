/********************************************************************************************************************/
/*!
 * @skip  $ld:$
 * @file  f_ini_bootUpdate.c
 * @brief INIT boot更新処理
 * @date  2016/04/19 ALPHA)fujiiy Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2016
 */
/********************************************************************************************************************/

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include "f_ini_inc.h"
#include <sys/types.h>
#include <sys/wait.h>
#include "errno.h"

#define D_INI_BOOT_HEADER_WSIZE		64				/* 64byte */

UCHAR	f_iniw_bootFile[ 0x80000 ];

/* 2020/12/28 M&C) Implement device startup (start modify) */
const USHORT		f_inir_eepAddr_bootVer[8] = 
{
	D_RRH_EEP_BOOTVER_H0_BK,		/* BOOT版数(0面 Backup)			*/
	D_RRH_EEP_BOOTVER_H0_PR,		/* BOOT版数(0面 Primary)		*/
	D_RRH_EEP_BOOTVER_H1_BK,		/* BOOT版数(1面 Backup)			*/
	D_RRH_EEP_BOOTVER_H1_PR,		/* BOOT版数(1面 Primary)		*/
	D_RRH_EEP_STA_BOOTVER_H0_BK,	/* 起動時BOOT版数(0面 Backup)	*/
	D_RRH_EEP_STA_BOOTVER_H0_PR,	/* 起動時BOOT版数(0面 Primary)	*/
	D_RRH_EEP_STA_BOOTVER_H1_BK,	/* 起動時BOOT版数(1面 Backup)	*/
	D_RRH_EEP_STA_BOOTVER_H1_PR,	/* 起動時BOOT版数(1面 Primary)	*/
};
/* 2020/12/28 M&C) Implement device startup (end modify) */

/********************************************************************************************************************/
/*!
 * @brief 関数機能概要:INIT boot更新処理
 * @note  関数処理内容.
 *		-# 各スレッド生成・起動
 * @param  none
 * @return none
 * @warning		N/A
 * @FeatureID	PF-Hdl-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2016/04/19 ALPHA)fujiiy Create
 */
/********************************************************************************************************************/
VOID f_ini_bootUpdate( VOID* buf_p )
{
#if 0 /* TODO */
	FILE *					file_p;
	INT						iReadSize;
	INT						rtn_bpf;
	UINT					bootVer_verFile;
	UINT					flashCnt;
	UINT					flashFace;
	UINT					flashOffset;
	UINT					bootVer_flash;
	UCHAR					bootVer_eep[2];
	UCHAR					bootVer_eep_tmp[2];
	UINT					bootVer_eep_char;
	UCHAR					bootVer_eep_str[16];
	USHORT					bootVer_eep_hex;
	UINT					fileReadFlag = D_SYS_OFF;
	UINT					bootUpdateFlag = D_SYS_OFF;
	UCHAR					soft_reset_flag = 0;
	CHAR * 					endptr = "\0";
	UINT					retry;
	UINT					okng;
	UINT					okng_result;
	UINT					regVal;
	UCHAR					device_kind0 = 0;
	UCHAR					device_kind1 = 0;
	UINT					*shm_rrhKind;
	INT						errcd = 0;
	
	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ENTER, "[f_ini_bootUpdate] ENTER" );
	
	/* backupで起動しているときに backupがイレースされていると起動しなくなることを防ぐため */
	/* (backupで起動時は、0x0000C010 で設定されている ) */
	BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, 0xF800702C, &regVal );
	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_INFO, "multi boot reg 0x%08x", regVal );
	BPF_COM_LOG_DMESG("multi boot reg 0x%08x\n", regVal );
	regVal &= 0xFFFFE000;
	BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, 0xF800702C, &regVal );
	
	/* ソフトリセットフラグ取得 */
	rtn_bpf = BPF_HM_DEVC_EEPROM_READ(D_RRH_EEP_SOFT_RESET_FLG, &soft_reset_flag);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
	{
		cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "e2p read NG 0x%02x(%x)", D_RRH_EEP_SOFT_RESET_FLG, rtn_bpf );
		BPF_COM_LOG_DMESG("e2p read NG 0x%02x(%x)\n", D_RRH_EEP_SOFT_RESET_FLG, rtn_bpf );
		return;
	}

	BPF_COM_LOG_DMESG("boot version check(%d) ... \n",soft_reset_flag);	
	/****************************************************************************************************************/
	/* FIRM領域に保持するBOOTバージョン情報取得																		*/
	/****************************************************************************************************************/
	file_p = fopen( D_RRH_BOOT_RADISK_VER,"rb");
	if(file_p == NULL)
	{
		cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "boot version file open NG" );
		BPF_COM_LOG_DMESG( "boot version file open NG\n" );
/* 2020/12/28 M&C) Implement device startup (start add) */
		/*	EEPROMの起動bootバージョン0面 Primaryを初期化する	*/
		(VOID)BPF_HM_DEVC_EEPROM_WRITE( D_RRH_EEP_STA_BOOTVER_H0_PR, (UCHAR*)&bootVer_eep_tmp[0] );
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(( D_RRH_EEP_STA_BOOTVER_H0_PR+1), (UCHAR*)&bootVer_eep_tmp[1] );
/* 2020/12/28 M&C) Implement device startup (end add) */
		return;
	}
	
	iReadSize = fread( (VOID *)&bootVer_verFile, 1, 4, file_p );
	if( 0 == iReadSize )
	{
		cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "boot version file size NG" );
		BPF_COM_LOG_DMESG( "boot version file size NG\n" );
		fclose(file_p);
/* 2020/12/28 M&C) Implement device startup (start add) */
		/*	EEPROMの起動bootバージョン0面 Primaryを初期化する	*/
		(VOID)BPF_HM_DEVC_EEPROM_WRITE( D_RRH_EEP_STA_BOOTVER_H0_PR, (UCHAR*)&bootVer_eep_tmp[0] );
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(( D_RRH_EEP_STA_BOOTVER_H0_PR+1), (UCHAR*)&bootVer_eep_tmp[1] );
/* 2020/12/28 M&C) Implement device startup (end add) */
		return;
	}
	fclose(file_p);
	
	/* FIRM領域に保持するBOOTファイルをオープンできなかった場合には、versionチェックせず以降の処理はスキップ */
	file_p = fopen( D_RRH_BOOT_RADISK_FILE,"rb");
	if(file_p == NULL)
	{
		cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "boot file open NG" );
		BPF_COM_LOG_DMESG( "boot file open NG\n" );
/* 2020/12/28 M&C) Implement device startup (start add) */
		/*	EEPROMの起動bootバージョン0面 Primaryを初期化する	*/
		(VOID)BPF_HM_DEVC_EEPROM_WRITE( D_RRH_EEP_STA_BOOTVER_H0_PR, (UCHAR*)&bootVer_eep_tmp[0] );
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(( D_RRH_EEP_STA_BOOTVER_H0_PR+1), (UCHAR*)&bootVer_eep_tmp[1] );
/* 2020/12/28 M&C) Implement device startup (end add) */
		return;
	}
	
	/* EEPROM boot version情報の更新 */
	errno=0;
	bootVer_eep_hex = strtoul((const CHAR *)&bootVer_verFile, &endptr, 16);
	if( 0 != errno )
	{
		cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "version convert NG(%x,0x%x)", bootVer_verFile ,errno);
		BPF_COM_LOG_DMESG( "version convert NG(0x%x ,0x%x)\n", bootVer_verFile , errno );
		fclose(file_p);
/* 2020/12/28 M&C) Implement device startup (start add) */
		/*	EEPROMの起動bootバージョン0面 Primaryを初期化する	*/
		(VOID)BPF_HM_DEVC_EEPROM_WRITE( D_RRH_EEP_STA_BOOTVER_H0_PR, (UCHAR*)&bootVer_eep_tmp[0] );
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(( D_RRH_EEP_STA_BOOTVER_H0_PR+1), (UCHAR*)&bootVer_eep_tmp[1] );
/* 2020/12/28 M&C) Implement device startup (end add) */
		return;
	}
	
	BPF_COM_END_BIGTOLITTLE16( bootVer_eep_hex );  /* pgr0539 */
	memcpy( bootVer_eep, &bootVer_eep_hex, 2 );

	okng = D_RRH_OK;
	okng_result = D_RRH_OK;
	for( flashCnt=0; flashCnt < 4; flashCnt ++ )
	{
		/* 一度でもNGだった場合には OKNG最終状態をNGとする */
		if( okng == D_RRH_NG )
		{
			okng_result = D_RRH_NG;
		}
		
		/* backup面 NGの場合は、primary面の更新をスキップする */
		if(		(okng == D_RRH_NG)
			&&	( (flashCnt == 1) || (flashCnt == 3) ) ){
			BPF_COM_LOG_DMESG("face%d Primary Skip(%d) \n",flashFace,(flashCnt+1));
			okng = D_RRH_OK;
			continue;
		}
		/* flash面, offset設定 */
		if( flashCnt < 2 )
		{
			flashFace = 0;
		}
		else
		{
			flashFace = 1;
		}
		if( 0 == (flashCnt % 2) )
		{
			flashOffset = D_RRH_QSPI_FLASH_OFFSET_BOOT_2;
		}
		else
		{
			flashOffset = D_RRH_QSPI_FLASH_OFFSET_BOOT;
		}
		bootVer_flash = 0;
		bootVer_eep_char = 0;
		/* flash 0/1面 Backup/Primary領域 書込み済 bootバージョン取得 */
		rtn_bpf = BPF_HM_DEVC_QSPI_FLASH_READ_BOOT_VER( flashFace, flashOffset, (UCHAR *)&bootVer_flash );
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
		{
			cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "flash read boot ver NG 0x%x-0x%06x(0x%x)", flashFace, flashOffset, rtn_bpf );
			BPF_COM_LOG_DMESG("flash read boot ver NG %x-%06x(%x)\n", flashFace, flashOffset, rtn_bpf );
			okng = D_RRH_NG;
			continue;
		}
		
		/* EEP格納 Backup/Primary(0/1面) bootバージョン取得 */
		rtn_bpf =  BPF_HM_DEVC_EEPROM_READ( f_inir_eepAddr_bootVer[flashCnt], (UCHAR*)&bootVer_eep_tmp[0] );
		rtn_bpf |= BPF_HM_DEVC_EEPROM_READ( (f_inir_eepAddr_bootVer[flashCnt]+1), (UCHAR*)&bootVer_eep_tmp[1] );
		
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
		{
			cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "e2p read NG 0x%02x(0x%x)", f_inir_eepAddr_bootVer[flashCnt], rtn_bpf );
			BPF_COM_LOG_DMESG("e2p read NG %02x(%x)\n", f_inir_eepAddr_bootVer[flashCnt], rtn_bpf );
			okng = D_RRH_NG;
			continue;
		}
		memset(bootVer_eep_str,0,sizeof(bootVer_eep_str));
		snprintf( (CHAR *)&bootVer_eep_str, sizeof(bootVer_eep_str), "%02X%02X", bootVer_eep_tmp[0], bootVer_eep_tmp[1] );
		memcpy((CHAR *)&bootVer_eep_char,bootVer_eep_str,sizeof(bootVer_eep_char));
		for(retry= 1 ; retry < 4 ; retry++)
		{
			/*		ramDisk上のboot versionファイルと、Flash上のbootファイルVersionが不一致の場合	*/
			/*	or	ramDisk上のboot versionファイルと、EEPROM上のbootファイルVersionが不一致の場合	*/
			if(		( bootVer_verFile != bootVer_flash)
				||	( bootVer_verFile != bootVer_eep_char ) )
			{
				okng = D_RRH_NG;
				bootUpdateFlag = D_SYS_ON;
				BPF_COM_LOG_DMESG("(%d)Mismatch face:%d ofs:0x%x r:0x%x f:0x%x e:0x%x \n",retry,flashFace,flashOffset,bootVer_verFile,bootVer_flash,bootVer_eep_char);
				/* SWDT 60秒設定 */
				(VOID)BPF_HM_DEVC_WDT_SET( D_SYS_AXI_TIMER_SKP, D_SYS_SWDT_TIMER_SET_60S );
				/* ramDiskからbootファイルを読み出す */
				if( D_SYS_OFF == fileReadFlag )
				{
					BPF_COM_LOG_DMESG("boot file read from file system to mem ...\n");
					memset(f_iniw_bootFile,0,sizeof(f_iniw_bootFile));
					iReadSize = fread( (VOID *)f_iniw_bootFile, 1, sizeof(f_iniw_bootFile), file_p );
					if(		( iReadSize < D_INI_BOOT_HEADER_WSIZE )
						||	( iReadSize > D_DU_QSPI_FLASH_SIZE_BOOT_PART )
					)
					{
						cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "boot file size NG" );
						BPF_COM_LOG_DMESG("boot file size NG \n");
						fclose(file_p);
						return;
					}
					fileReadFlag = D_SYS_ON;
				}

				/* EEPROM boot version情報のクリア */
				bootVer_eep_tmp[0] = 0xFF;
				bootVer_eep_tmp[1] = 0xFF;
				BPF_COM_LOG_DMESG("eeprom(0x%x<-0x%x,0x%x<-0x%x) boot ver clear ...\n",
					 f_inir_eepAddr_bootVer[flashCnt],bootVer_eep_tmp[0],
					 (f_inir_eepAddr_bootVer[flashCnt]+1),bootVer_eep_tmp[1]
				);
				rtn_bpf =  BPF_HM_DEVC_EEPROM_WRITE( f_inir_eepAddr_bootVer[flashCnt], (UCHAR*)&bootVer_eep_tmp[0] );
				rtn_bpf |= BPF_HM_DEVC_EEPROM_WRITE( (f_inir_eepAddr_bootVer[flashCnt]+1), (UCHAR*)&bootVer_eep_tmp[1] );
				if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
				{
					cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "e2p write NG 0x%02x(0x%x)",f_inir_eepAddr_bootVer[flashCnt], rtn_bpf );
					BPF_COM_LOG_DMESG("e2p write NG 0x%02x(0x%x)\n",f_inir_eepAddr_bootVer[flashCnt], rtn_bpf );
					continue;
				}
				BPF_COM_LOG_DMESG("qspi flash erase ...\n");
				/* Flash Erase */
				rtn_bpf = BPF_HM_DEVC_QSPI_FLASH_ERASE( flashFace, flashOffset, iReadSize );
				if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
				{
					cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "flash erase NG 0x%x-%06x(0x%x)",flashFace, flashOffset, rtn_bpf );
					BPF_COM_LOG_DMESG("flash erase NG 0x%x-%06x(0x%x)\n",flashFace, flashOffset, rtn_bpf );
					continue;
				}
				BPF_COM_LOG_DMESG("qspi flash boot payload write ...\n");
				/* Flash Write */
				rtn_bpf = BPF_HM_DEVC_QSPI_FLASH_WRITE( flashFace, flashOffset + D_INI_BOOT_HEADER_WSIZE, (UINT)(iReadSize - D_INI_BOOT_HEADER_WSIZE) , &f_iniw_bootFile[D_INI_BOOT_HEADER_WSIZE] );
				if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
				{
					cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "flash write NG 0x%x-0x%06x(0x%x)", flashFace, flashOffset, rtn_bpf );
					BPF_COM_LOG_DMESG("flash write NG 0x%x-0x%06x(0x%x)\n", flashFace, flashOffset, rtn_bpf );
					continue;
				}
				BPF_COM_LOG_DMESG("qspi flash boot header write ...\n");
				/* Flash Write */
				rtn_bpf = BPF_HM_DEVC_QSPI_FLASH_WRITE( flashFace, flashOffset, D_INI_BOOT_HEADER_WSIZE , f_iniw_bootFile );
				if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
				{
					cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "flash write NG 0x%x-0x%06x(0x%x)", flashFace, flashOffset, rtn_bpf );
					BPF_COM_LOG_DMESG("flash write NG 0x%x-0x%06x(0x%x)\n", flashFace, flashOffset, rtn_bpf );
					continue;
				}

				BPF_COM_LOG_DMESG("boot ver verify check ...\n");
				/* flash 0/1面 Backup/Primary領域 書込み済 bootバージョン取得 */
				rtn_bpf = BPF_HM_DEVC_QSPI_FLASH_READ_BOOT_VER( flashFace, flashOffset, (UCHAR *)&bootVer_flash );
				if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
				{
					cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "flash read boot ver NG 0x%x-0x%06x(0x%x)",flashFace, flashOffset, rtn_bpf );
					BPF_COM_LOG_DMESG("flash read boot ver NG 0x%x-0x%06x(0x%x)\n",flashFace, flashOffset, rtn_bpf );
					continue;
				}
				if( bootVer_verFile != bootVer_flash )
				{
					cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "boot ver verify check NG 0x%x - 0x%x (0x%x)",bootVer_verFile, bootVer_flash, rtn_bpf );
					BPF_COM_LOG_DMESG("NG(0x%x)\n",bootVer_flash);
					continue;
				}
				BPF_COM_LOG_DMESG("OK eeprom(0x%x<-0x%x,0x%x<-0x%x) boot ver write ...\n",
					 f_inir_eepAddr_bootVer[flashCnt], bootVer_eep[0],
					 (f_inir_eepAddr_bootVer[flashCnt]+1),bootVer_eep[1]
				);
				rtn_bpf =  BPF_HM_DEVC_EEPROM_WRITE( f_inir_eepAddr_bootVer[flashCnt], (UCHAR*)&bootVer_eep[0] );
				rtn_bpf |= BPF_HM_DEVC_EEPROM_WRITE( (f_inir_eepAddr_bootVer[flashCnt]+1), (UCHAR*)&bootVer_eep[1] );
				if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
				{
					cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "e2p write NG 0x%02x(0x%x)",f_inir_eepAddr_bootVer[flashCnt], rtn_bpf );
					BPF_COM_LOG_DMESG("e2p write NG 0x%02x(0x%x)\n",f_inir_eepAddr_bootVer[flashCnt], rtn_bpf );
					continue;
				}
				BPF_COM_LOG_DMESG("face%d OK finish \n",flashFace);
				okng = D_RRH_OK;
			}
			break;
		}
	}
	
	fclose(file_p);
	
	/* boot更新があった場合には再起動 */
	if( D_SYS_ON == bootUpdateFlag )
	{
		if( soft_reset_flag == D_RRH_SOFT_RESET_BOOTDWL )
		{
			if( D_RRH_NG == okng_result )
			{
				/* 起動busyフラグbit OFF(このフラグを見てロールバックするかどうかをbootで決定するためOFF設定が必要) */
				BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_RRH_REG_SLCR_ZYNQ_REBOOT_STATUS, D_RRH_SLCR_RST_FACT_BTBUSY );
				
				/* abort */
				f_ini_err_eth();/* eth接続 */
				BPF_COM_LOG_DMESG("... 2nd Boot DWL NG ALM \n");
				(VOID)BPF_HM_DEVC_WDT_SET( D_SYS_AXI_TIMER_CLR, D_SYS_SWDT_TIMER_CLR );
				
				/* 状態をALMとして、処理終了 */
				f_ini_led_alm(buf_p);

#if 0	/***** TODO	*****/
				regVal = D_DU_ALMCD_FWNG;
				BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, D_DU_REG_CTB_SV4INF, &regVal );
				BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTB_SV1INF, D_RRH_CNT_SVINF_ALM );
#endif	/***** TODO	*****/

				/* デバッグコマンドを使用可能にできるよう RRH Kindを設定する */
				(VOID)BPF_HM_DEVC_EEPROM_READ(D_RRH_EEP_DEVICE_KIND0, &device_kind0);
				(VOID)BPF_HM_DEVC_EEPROM_READ(D_RRH_EEP_DEVICE_KIND1, &device_kind1);
				(VOID)BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_RRH_KIND, (void**)&shm_rrhKind, &errcd );
				*shm_rrhKind = D_RRH_EEP_DEV_KIND_5GDU;
				exit(0);
			}
		}
		else
		{
			soft_reset_flag = D_RRH_SOFT_RESET_BOOTDWL;
		}
		
		
		
		(void)BPF_HM_DEVC_EEPROM_WRITE(D_RRH_EEP_SOFT_RESET_FLG, &soft_reset_flag);

		/* 0面起動とするために、Flash 0面リード */
		(void)BPF_HM_DEVC_QSPI_FLASH_READ_BOOT_VER( 0, 0, (UCHAR *)&bootVer_flash );

		BPF_COM_LOG_DMESG("... boot DWL Reset(0x%x 0x%x %d) \n",bootVer_verFile, bootVer_flash,soft_reset_flag);
		
		/* 起動busyフラグbit OFF(このフラグを見てロールバックするかどうかをbootで決定するためOFF設定が必要) */
		BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_RRH_REG_SLCR_ZYNQ_REBOOT_STATUS, D_RRH_SLCR_RST_FACT_BTBUSY );
		
		/* リセット制御 */
		BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE,
									D_RRH_REG_SLCR_ZYNQ_PSS_RST_CTRL,
									D_RRH_BIT_SLCR_ZYNQ_PSS_RST_CTRL);
	}

	BPF_COM_LOG_DMESG("... all boot version was same(0x%x) \n",bootVer_verFile);
	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_RETURN, "[f_ini_bootUpdate] RETURN" );
#endif
	return;
}
/* @} */

/********************************************************************************************************************/
/*!
 * @brief 関数機能概要:INIT PTP config file更新処理
 * @note  関数処理内容.
 * @param  none
 * @return none
 * @warning		N/A
 * @FeatureID	PF-Hdl-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2018/08/21 Taniguchi Create
 */
/********************************************************************************************************************/
VOID f_ini_ptpConfUpdate(VOID)
{
	UCHAR	config_ptn;
	UCHAR	face;
	USHORT	sizeofs;
	UCHAR	size;
	UINT	filesize;
	UINT	flashofs;
	UINT	count1,count2;
	UCHAR*	buf_p;
	int		ret;
	size_t	len;
	FILE*	fp;

	(void)BPF_HM_DEVC_EEPROM_READ(0x326 , &config_ptn);
	if((config_ptn != 1) && (config_ptn != 2)){
		BPF_COM_LOG_DMESG("ptp Config is not test mode...(0x%x)\n",config_ptn);
		return;
	}

	(void)BPF_HM_DEVC_EEPROM_READ(0x50 , &face);
	switch(face){
		case 0:
			flashofs = 0x06D00000;
			sizeofs = 0x1ae;
			break;
		case 1:
			flashofs = 0x0ED00000;
			sizeofs = 0x1b2;
			break;
		default:
			BPF_COM_LOG_DMESG("ptp Config update skip...(0x%x) \n",face);
			return;
			break;
	}

	filesize = 0;
	for(count1 = sizeofs,count2=0 ; count1 < (sizeofs + 4); count1++,count2++){
		BPF_HM_DEVC_EEPROM_READ(count1 , &size);
		switch(count2){
			case 0:
				filesize |= (UINT)((size << 24) & 0xFF000000);
				break;
			case 1:
				filesize |= (UINT)((size << 16) & 0x00FF0000);
				break;
			case 2:
				filesize |= (UINT)((size << 8) & 0x0000FF00);
				break;
			case 3:
				filesize |= (UINT)(size & 0x000000FF);
				break;
			default:
				break;
		}
	}
	if((filesize > 0x00020000) || (filesize == 0)){
		BPF_COM_LOG_DMESG("ptp Config file was not saved yet...(0x%x)\n",filesize);
		return;
	}

	if((fp = fopen("/tmp/ptpconf.zip","w")) == NULL){
		BPF_COM_LOG_DMESG("ptp Config file open(%d) error...\n",errno);
		return;
	}

	buf_p = (UCHAR*)malloc(0x00020000);
	if(buf_p == NULL){
		fclose(fp);
		BPF_COM_LOG_DMESG("ptp Config buffer error...\n");
		return;	
	}

	memset(buf_p,0,sizeof(0x00020000));	
	BPF_HM_DEVC_QSPI_FLASH_READ(0,flashofs,filesize,buf_p);

	if(( len = fwrite(buf_p,filesize,1,fp) ) < 1 ){
		fclose(fp);
		free(buf_p);
		BPF_COM_LOG_DMESG("ptp Config file(%d) write error...\n",len);
		return;	
	}

	fflush(fp);
	fclose(fp);
	free(buf_p);

	ret = system("unzip /tmp/ptpconf.zip -d /tmp > /dev/kmsg");
	if( WEXITSTATUS(ret) != 0 ){		/* pgr0539 */
		BPF_COM_LOG_DMESG("ptp Config unzip error...\n");
		return;
	}
	
	ret = 0;
	switch(config_ptn){
		case 1:
			ret = system("cp -f /tmp/PTP_reg_set_1.txt /etc/ptp/PTP_reg_set.txt > /dev/kmsg");
			break;
		case 2:
			ret = system("cp -f /tmp/PTP_reg_set_2.txt /etc/ptp/PTP_reg_set.txt > /dev/kmsg");
			break;
		default:
			return;
			break;
	}
	if( WEXITSTATUS(ret) != 0 ){	/* pgr0539 */
		BPF_COM_LOG_DMESG("ptp Config(PTP_reg_set_%d.txt) UPLOAD ERROR!!! \n",config_ptn);
	}else{
		BPF_COM_LOG_DMESG("ptp Config(PTP_reg_set_%d.txt) UPLOAD OK!!! \n",config_ptn);
	}
	return;
}
/* @} */
/* @} */
