/*!
 * @skip		$ld:$
 * @file		f_ini_AccidentlogSave.c
 * @brief		障害ログ出力処理
 * @date		2013/12/03 ALPHA)近藤 Create
 * @data  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_INI
 * @{
 */

#include "f_ini_inc.h"

/*!
 * @brief		関数機能概要:障害ログ出力処理を行う
 * @note		関数処理内容.
 *				-# 出力ファイルを圧縮する
 *				-# FlashClearを実施する
 *				-# FlashWriteを実施する
 * @param		trouble kind	[in]	障害種別
 * @return		N/A
 * @warning		N/A
 * @FeatureID	PF-Init-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/12/03 ALPHA)近藤 Create
 * @data  		2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 */
VOID f_ini_AccidentlogSave( UINT  uiTroubleKind , VOID* buf_p )
{
	FILE*				LOG_r;
	LONG				file_size;
	UINT				log_face = 0;
	INT					iReadSize;
	INT					iRet;
	UINT				flash_face = 0;
	UINT				flash_addr = 0;
	CHAR				system_cmd[3][84] = {	"tar zc -f /var/log/trouble_rst00.tar.gz /var/log/troublelog_rst 1>/dev/null 2>&1",
												"tar zc -f /var/log/trouble_rst01.tar.gz /var/log/troublelog_rst 1>/dev/null 2>&1",
												"tar zc -f /var/log/trouble_rst02.tar.gz /var/log/troublelog_rst 1>/dev/null 2>&1" };
	CHAR				compfile[3][32] = {	"/var/log/trouble_rst00.tar.gz",
											"/var/log/trouble_rst01.tar.gz",
											"/var/log/trouble_rst02.tar.gz" };
											
	T_INIT_LOGSAVE f_inir_logsaveaddr[4] =
	{
		{ 0, D_RRH_QSPI_FLASH_OFFSET_TROUBLE0 },        /* E_RRH_QSPI_FLASH_TROUBLELOG_FACE0 */
		{ 0, D_RRH_QSPI_FLASH_OFFSET_TROUBLE1 },        /* E_RRH_QSPI_FLASH_TROUBLELOG_FACE1 */
		{ 0, D_RRH_QSPI_FLASH_OFFSET_TROUBLE2 },        /* E_RRH_QSPI_FLASH_TROUBLELOG_FACE2 */
		{ 0, D_RRH_QSPI_FLASH_OFFSET_HEALTH   }         /* D_RRH_QSPI_FLASH_OFFSET_HEALTH */
	};

	CHAR				cmd[256];


	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ENTER, "[f_ini_AccidentlogSave] ENTER" );

	/************************************************/
	/* 出力ファイルを圧縮する						*/
	/************************************************/
	if( E_INI_LOGTYPE_TROUBLE == uiTroubleKind )
	{
		/* 障害ログの退避面を取得する	*/
		(VOID)BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_TROUBLE_LOG, (UCHAR*)&log_face );
		
		/* 退避面を更新	*/
		/* EEPROMへの反映は最後に行う	*/
		log_face++;
		if( log_face >= E_RRH_QSPI_FLASH_TROUBLELOG_FACE_MAX )
		{
			log_face = 0;
		}
		
		flash_face  = f_inir_logsaveaddr[log_face].face;
		flash_addr  = f_inir_logsaveaddr[log_face].addr;

		/* /var/log/troublelog_rst配下を圧縮する	*/
		iRet = system( system_cmd[log_face] );
		if( D_RRH_MNG == iRet )
		{
			cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "Compress failed. %s", system_cmd[log_face] );
		}
		else
		{
			cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_INFO, "Compress TROUBLE finish face:%d",log_face);
		}
		
		/* 圧縮ファイルをOPENする	*/
		LOG_r = fopen( compfile[log_face] ,"rb");
		if(LOG_r == NULL)
		{
			cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "open error" );
			return;
		}

		fseek(LOG_r, 0, SEEK_END);
		file_size = ftell(LOG_r);
		if(file_size > 0 ) {
			if( (file_size + 4) >= D_RRH_QSPI_FLASH_TROUBLELOG_SIZE ){
				fclose(LOG_r);

				cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "Compress TROUBLE log size limit over :%ld",file_size);
				memset(cmd,0,sizeof(cmd));
				snprintf(cmd,sizeof(cmd),"rm -rf %s 1>/dev/null 2>/dev/null",compfile[log_face]);
				iRet = system(cmd);
				if( D_RRH_MNG == iRet )
				{
					cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "%s error",cmd);
				}
				iRet = system("rm -rf /var/log/troublelog_rst/core.* 1>/dev/null 2>/dev/null");	/* coreのみ削除 */
				iRet = system( system_cmd[log_face] );	/* 再度圧縮 */
				if( D_RRH_MNG == iRet )
				{
					cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "rm failed. %ld",file_size);
				}
				LOG_r = fopen( compfile[log_face] ,"rb");
				if(LOG_r == NULL)
				{
					cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "open error" );
					return;
				}
			}
			else{
				cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "Compress TROUBLE log size OK :%ld",file_size);
				fseek(LOG_r,0,SEEK_SET);
			}
			/* ログ格納フォルダをクリアする	*/
			iRet = system("rm -rf /var/log/troublelog_rst/*");
			if( D_RRH_MNG == iRet )
			{
				cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "rm failed. %ld",file_size);
			}
		}
		else{
			fclose(LOG_r);
			cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "file size error %ld",file_size);
			return;
		}
	}
	else
	{
		/* ヘルスチェック断の場合	*/
		/* Flash退避面、アドレス設定 */
		flash_face  = f_inir_logsaveaddr[3].face;
		flash_addr  = f_inir_logsaveaddr[3].addr;
		
		/* /var/log/healthchklogフォルダを圧縮する */
		iRet = system( "tar zc -f /var/log/healthchklog.tar.gz /var/log/healthchklog 1>/dev/null 2>&1" );
		if( D_RRH_MNG == iRet )
		{
			cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "Compress failed." );
		}
		else
		{
			LOG_r = fopen( "/var/log/healthchklog.tar.gz" ,"rb");
			if(LOG_r == NULL)
			{
				cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "open error" );
				return;
			}
			fseek(LOG_r, 0, SEEK_END);
			file_size = ftell(LOG_r);
			fclose(LOG_r);
			if( (file_size + 4) >= D_RRH_QSPI_FLASH_TROUBLELOG_SIZE ){		/* pgr0539 */
				cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "Compress TROUBLE log size limit over :%ld",file_size);
				iRet = system("rm -rf /var/log/healthchklog.tar.gz 1>/dev/null 2>/dev/null");
				iRet = system("rm -rf /var/log/healthchklog/*.pcap 1>/dev/null 2>/dev/null");
				iRet = system("tar zc -f /var/log/healthchklog.tar.gz /var/log/healthchklog 1>/dev/null 2>&1");
				if( D_RRH_MNG == iRet ){
					cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "Compress failed." );
				}
			}
			cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_INFO, "Compress HEALTH finis:%ld",file_size);
		}
		/* ログ格納フォルダをクリアする	*/
		iRet = system("rm -rf /var/log/healthchklog/*");
		if( D_RRH_MNG == iRet )
		{
			cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "rm failed." );
		}
		
		/* 圧縮ファイルをOPENする	*/
		LOG_r = fopen( "/var/log/healthchklog.tar.gz" ,"rb");
		if(LOG_r == NULL)
		{
			cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "open error" );
			return;
		}
	}

	/****************************************************************/
	/* FLASH CLEARを行ってからWRITEする								*/
	/* CLEARサイズは今回書き込む分のサイズ+復元用サイズ領域(4byte)	*/
	/****************************************************************/
	/* 書き込み対象のファイルサイズを求める */
	fseek(LOG_r, 0, SEEK_END);
	file_size = ftell(LOG_r);
	
	/* 復帰値を整合性を判定*/
	if(file_size == -1 ) { /* pgr0539 */
		cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "File tell error " );
		cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_RETURN, "[f_ini_AccidentlogSave] RETURN" );
		fclose(LOG_r);
		return;
	}
	
	/* 512kbyte以上は書き込み不可	*/
	if( (file_size + 4) >= D_RRH_QSPI_FLASH_TROUBLELOG_SIZE ){ /* pgr0539 */
		cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "File size is too big" );
		fclose(LOG_r);
		return;
	}

	fseek(LOG_r,0,SEEK_SET);
	
	/************************************************/
	/* FlashClearを実施								*/
	/* 実体＋復元用のサイズ(4byte)					*/
	/************************************************/
	BPF_HM_DEVC_QSPI_FLASH_ERASE(flash_face, flash_addr, (file_size+4) );
	
	/* FileSizeを冒頭4byteに書き込む	*/
	memcpy( &f_iniw_compressData[0],
			(CHAR*)&file_size,
			sizeof(file_size));
	
	iRet = BPF_HM_DEVC_QSPI_FLASH_WRITE(flash_face,
										flash_addr,
										4,
										f_iniw_compressData);
	flash_addr += 4;
	
	/************************************************/
	/* FlashWriteを実施								*/
	/************************************************/
	while( (iReadSize = fread( f_iniw_compressData, 1, D_INI_COMPDATA, LOG_r ) ) != 0 )
	{
		iRet = BPF_HM_DEVC_QSPI_FLASH_WRITE(flash_face,
											flash_addr,
											iReadSize,
											f_iniw_compressData);
		if( iRet != BPF_HM_DEVC_COMPLETE )
		{
			cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_RETURN, "[f_ini_AccidentlogSave] RETURN" );
			fclose(LOG_r);
			return;
		}
		
		memset( f_iniw_compressData, 0xFF, sizeof(f_iniw_compressData) );
		flash_addr += iReadSize;
	}
	
	if( E_INI_LOGTYPE_TROUBLE == uiTroubleKind )
	{
		/* 障害ログ退避面を更新	*/
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_TROUBLE_LOG, (UCHAR *)&log_face);
	}

	/* 圧縮ファイルをCLOSEする	*/
	fclose(LOG_r);

	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_RETURN, "[f_ini_AccidentlogSave] RETURN" );

	return;
}

/* @} */
