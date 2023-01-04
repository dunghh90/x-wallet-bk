/*!
 * @skip  $ld:$
 * @file  f_com_logSaveFlash.c
 * @brief 各種ログのFLASH退避
 * @date  2013/03/16 ALPHA) kondou Create RRH-007-000 初期化
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2016
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include "f_com_inc.h"

/*!
 * @brief		f_com_logSaveFlashFHM
 * @note		関数処理内容.
 *					-# 各種ログをFLASHに退避する
 * @return		N/A
 * @warning		N/A
 * @FeatureID	PF-Init-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2013/03/16 ALPHA)kondou Create RRH-007-000 初期化
 * @date 2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  2015/08/18 TDIPS) maruyama RE種別（f_comw_reKind）で配列の切り分けを取りやめ（データを直接セットする）
 * @date  2016/03/03 FJT)harada 35G-SREからの水平展開,サイクリックのログ展開失敗問題対処
 * @date 2021/01/19 M&C) Merge 4G FHM src
 */
VOID f_com_logSaveFlashFHM()
{
	/****************************************************************************************************************/
	/* Local parameter declaration																					*/
	/****************************************************************************************************************/
	UINT						logCnt;
	UCHAR						rData;												/* Flash面情報読み込みデータ	*/
	UCHAR						wData;												/* Flash面情報書き込みデータ	*/
	UINT						face=0;												/* Flash面						*/
	VOID						*LogPtr;
	INT							endcd;												/* 終了コード					*/
	INT							errcd;												/* 詳細NGコード					*/
	INT							iRet;
	UINT						uiOutLen;
	UINT						offset =0;
	UCHAR						temp_data[4];
	UINT						clearstate[2] = { D_RRH_EEP_CYCLIC_CLEAR_FLG0, D_RRH_EEP_CYCLIC_CLEAR_FLG1 };
	UCHAR						clearflg;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_com_logSaveFlash] ENTER" );
	
	/* Flash面情報取得 */
	endcd = BPF_HM_DEVC_EEPROM_READ(D_RRH_EEP_CYCLIC_LOG, &rData);
	if( endcd != BPF_HM_DEVC_COMPLETE )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Flash cyclic log side read error=0x%08x", endcd );
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_com_logSaveFlash] RETURN" );
		return;
	}
	/* QSPI FASH0面の場合*/
	if(rData == BPF_HM_DEVC_QSPI_FLASH_FACE_0 )
	{
		face = BPF_HM_DEVC_QSPI_FLASH_FACE_1;
	}
	/* QSPI FASH1面の場合*/
	else
	{
		face = BPF_HM_DEVC_QSPI_FLASH_FACE_0;
	}
	
	/* CyclicログClear状態を取得する */
	endcd = BPF_HM_DEVC_EEPROM_READ(clearstate[face], &clearflg);
	if( endcd != BPF_HM_DEVC_COMPLETE )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Flash cyclic state read error=0x%08x", endcd );
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_com_logSaveFlash] RETURN" );
		return;
	}
	
	/* Cyclicログ退避場所がClearされていればそのままWriteする	*/
	/* ClearされていなければClearしてからWriteする				*/
	if( clearflg == D_RRH_FLASH_ERASED )
	{
		/* そのままWrite処理を行う	*/
	}
	else
	{
		if( clearflg == D_RRH_FLASH_WRITE_FIN )
		{
			/* 時間短縮のため必要な分のみeraseする	*/
			/* 冒頭4byteにサイズ情報が格納されているのでそのサイズを取得	*/
			switch(f_comw_reKind)
			{
				case E_RRH_REKIND_FHM:
					endcd = BPF_HM_DEVC_QSPI_FLASH_READ(	face,
															D_RRH_QSPI_FLASH_OFFSET_INFO,
															sizeof(temp_data),
															temp_data);
					break;
				default:
					endcd = BPF_HM_DEVC_QSPI_FLASH_READ(	face,
															0,
															sizeof(temp_data),
															temp_data);
					break;
			}
			
			if(BPF_HM_DEVC_COMPLETE != endcd)
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Flash Log Info load NG. endcd=%08x", endcd);
			}
			
			/************************************************************************************/
			/* サイズを取得する																	*/
			/************************************************************************************/
			uiOutLen =	(temp_data[3] << 24) + 
						(temp_data[2] << 16) + 
						(temp_data[1] <<  8) + 
						(temp_data[0]);
			
			/* サイズが1Mを超える場合はクリップする	*/
			/* サイズが0の場合もサイクリックログ最大数分イレースする */
			if( (uiOutLen >= D_RRH_QSPI_FLASH_CYCLICLOG_SIZE) || (uiOutLen == 0) )
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "FlashSize Invalid(%d,%d)", face, uiOutLen);
				uiOutLen = D_RRH_QSPI_FLASH_CYCLICLOG_SIZE;
			}
			else
			{
				/* length格納領域も含めてEraseする */
				uiOutLen += 4;
			}
		}
		/* not erased 状態であり、サイズ不定であるため、全体をErase処理を実施する */
		else /* if( clearflg == D_RRH_FLASH_NOT_ERASED ) */
		{
			uiOutLen = D_RRH_QSPI_FLASH_CYCLICLOG_SIZE;
		}
		
		/* Clearする	*/
		switch(f_comw_reKind)
		{
			case E_RRH_REKIND_FHM:
				endcd = BPF_HM_DEVC_QSPI_FLASH_ERASE( face, D_RRH_QSPI_FLASH_OFFSET_INFO, uiOutLen );
				break;
			default:
				endcd = BPF_HM_DEVC_QSPI_FLASH_ERASE( face, 0, uiOutLen );
				break;
		}
		
		if( endcd != D_SYS_OK )
		{
			/* Assert */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "FlashClear NG endcd=%d", endcd);
		}
		else
		{
			/* CyclicログClear状態をClear済みに更新	*/
			clearflg = D_RRH_FLASH_ERASED;
			(VOID)BPF_HM_DEVC_EEPROM_WRITE( clearstate[ face ], &clearflg);
		}
	}
	
	memset( &f_comw_orgData, 0, sizeof(f_comw_orgData) );
	
	/* 1つのBUFFERに共有メモリ情報をすべてコピーしてから圧縮する				*/
	/* 解凍後にデータの展開が必要なため、KEYIDと元の構造体サイズも設定しておく	*/
	for( logCnt=0; logCnt<D_RRH_FLASH_LOG_MAX; logCnt++ )
	{
		/* 共有メモリからログ情報取得 */
		endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
					f_comr_flash_log_save_info[logCnt].shmid,						/* 共有メモリ番号				*/
					(VOID **)&LogPtr,												/* 共有メモリアドレス			*/
					&errcd );														/* 詳細NGコード					*/
		
		if( endcd != BPF_RU_IPCM_OK )
		{
			/* ログ保存はスキップし処理継続 */
			continue;
		}
		
		/* 共有メモリの情報をまとめる	*/
		
		/* 共有メモリのKEYIDをコピー	*/
		memcpy( &f_comw_orgData[offset],
				(CHAR*)&f_comr_flash_log_save_info[logCnt].shmid,
				sizeof(f_comr_flash_log_save_info[logCnt].shmid) );
		offset += sizeof(f_comr_flash_log_save_info[logCnt].shmid);
		
		/* 共有メモリの構造体サイズをコピー	*/
		memcpy( &f_comw_orgData[offset],
				(CHAR*)&f_comr_flash_log_save_info[logCnt].logSize,
				sizeof(f_comr_flash_log_save_info[logCnt].logSize) );
		offset += sizeof(f_comr_flash_log_save_info[logCnt].logSize);
		
		/* 共有メモリの実体をコピー	*/
		memcpy( &f_comw_orgData[offset],
				(CHAR*)LogPtr,
				f_comr_flash_log_save_info[logCnt].logSize );
		offset += f_comr_flash_log_save_info[logCnt].logSize;
	}
	
	/* 共有メモリの情報を圧縮する	*/
	uiOutLen = 0;
	iRet = f_com_FA_encode(	(UCHAR*)f_comw_orgData,
							f_comw_compressData_cycle.logdata,
							offset,
							D_RRH_FLASH_LOG_SIZE, 
							&f_comw_fa_dictionary,
							&uiOutLen);
	
	if (iRet != D_RRH_OK){
		/* 圧縮失敗時は処理終了	*/
		return;
	}
	
	/* 圧縮後のサイズをコピーする	*/
	memcpy( f_comw_compressData_cycle.compsize,
			(CHAR*)&uiOutLen,
			sizeof(uiOutLen));
	
	/* ログサイズ領域も含めてWriteする */
	uiOutLen += 4;
	
	/* 先にCyclicログClear状態をNotClearedに更新	*/
	clearflg = D_RRH_FLASH_NOT_ERASED;
	(VOID)BPF_HM_DEVC_EEPROM_WRITE( clearstate[ face ], &clearflg);
	
	/* Flash Write */
	switch(f_comw_reKind)
	{
		case E_RRH_REKIND_FHM:
			endcd = BPF_HM_DEVC_QSPI_FLASH_WRITE(
						face,															/* Flash面						*/
						D_RRH_QSPI_FLASH_OFFSET_INFO,									/* Flash Offset					*/
						uiOutLen,														/* logサイズ					*/
						f_comw_compressData_cycle.compsize );							/* log領域						*/
			break;
		default:
			endcd = BPF_HM_DEVC_QSPI_FLASH_WRITE(
						face,															/* Flash面						*/
						0,																/* Flash Offset					*/
						uiOutLen,														/* logサイズ					*/
						f_comw_compressData_cycle.compsize );							/* log領域						*/
			break;
	}
	wData = face;
	endcd = BPF_HM_DEVC_EEPROM_WRITE(D_RRH_EEP_CYCLIC_LOG, &wData);
	if( endcd != BPF_HM_DEVC_COMPLETE )
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Flash cyclic log side write error=0x%08x", endcd);
	}

	/* CyclicログClear状態を書き込み完了に更新	*/
	clearflg = D_RRH_FLASH_WRITE_FIN;
	(VOID)BPF_HM_DEVC_EEPROM_WRITE( clearstate[ face ], &clearflg);
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_com_logSaveFlash] RETURN" );

	return;
}

/*!
 * @brief		f_com_logSaveFlash
 * @note		関数処理内容.
 *					-# 各種ログをFLASHに退避する
 * @param		wdtLogFlg		[in] UINT  WDT logも併せて保存するかどうかを表す
 * @return		N/A
 * @warning		N/A
 * @FeatureID	PF-Init-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2013/03/16 ALPHA)kondou Create RRH-007-000 初期化
 * @data 2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @data 2016/03/14 ALPHA)fujjiiy modify for M-RRU-ZSYS-02088
 */
VOID f_com_logSaveFlash( UINT wdtLogFlg )
{
	/****************************************************************************************************************/
	/* Local parameter declaration																					*/
	/****************************************************************************************************************/
	UINT						logCnt;
	UCHAR						rData;												/* Flash面情報読み込みデータ	*/
	UCHAR						wData;												/* Flash面情報書き込みデータ	*/
	UINT						face=0;												/* Flash面						*/
	VOID						*LogPtr;
	INT							endcd;												/* 終了コード					*/
	INT							errcd;												/* 詳細NGコード					*/
	INT							iRet;
	UINT						uiOutLen;
	UINT						offset =0;
	UCHAR						temp_data[4];
	UINT						clearstate[2] = { D_DU_EEP_CYCLIC_CLEAR_FLG0, D_DU_EEP_CYCLIC_CLEAR_FLG1 };
	UCHAR						clearflg;
	UINT						flashoffset;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_com_logSaveFlash] ENTER" );
	
	flashoffset = D_RRH_QSPI_FLASH_OFFSET_INFO;	

	/* Flash面情報取得 */
	endcd = BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_CYCLIC_LOG, &rData);
	if( endcd != BPF_HM_DEVC_COMPLETE )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Flash cyclic log side read error=0x%08x", endcd );
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_com_logSaveFlash] RETURN" );
		return;
	}
	/* QSPI FASH0面の場合*/
	if(rData == BPF_HM_DEVC_QSPI_FLASH_FACE_0 )
	{
		face = BPF_HM_DEVC_QSPI_FLASH_FACE_1;
	}
	/* QSPI FASH1面の場合*/
	else
	{
		face = BPF_HM_DEVC_QSPI_FLASH_FACE_0;
	}
	
	/* CyclicログClear状態を取得する */
	endcd = BPF_HM_DEVC_EEPROM_READ(clearstate[face], &clearflg);
	if( endcd != BPF_HM_DEVC_COMPLETE )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Flash cyclic state read error=0x%08x", endcd );
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_com_logSaveFlash] RETURN" );
		return;
	}
	
	/* Cyclicログ退避場所がClearされていればそのままWriteする	*/
	/* ClearされていなければClearしてからWriteする				*/
	if( clearflg == D_RRH_FLASH_ERASED )
	{
		/* そのままWrite処理を行う	*/
	}
	else
	{
		if( clearflg == D_RRH_FLASH_WRITE_FIN )
		{
			/* 時間短縮のため必要な分のみeraseする	*/
			/* 冒頭4byteにサイズ情報が格納されているのでそのサイズを取得	*/
			endcd = BPF_HM_DEVC_QSPI_FLASH_READ(	face,
													flashoffset,
													sizeof(temp_data),
													temp_data);
			
			if(BPF_HM_DEVC_COMPLETE != endcd)
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Flash Log Info load NG. endcd=%08x", endcd);
			}
			
			/************************************************************************************/
			/* サイズを取得する																	*/
			/************************************************************************************/
			uiOutLen =	(temp_data[3] << 24) + 
						(temp_data[2] << 16) + 
						(temp_data[1] <<  8) + 
						(temp_data[0]);
			
			/* サイズが3Mを超える場合はクリップする	*/
			/* サイズが0の場合もサイクリックログ最大数分イレースする */
			if( (uiOutLen >= D_RRH_QSPI_FLASH_CYCLICLOG_SIZE) || (uiOutLen == 0) )
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "FlashSize Invalid(%d,%d)", face, uiOutLen);
				uiOutLen = D_RRH_QSPI_FLASH_CYCLICLOG_SIZE;
			}
			else
			{
				/* length格納領域も含めてEraseする */
				uiOutLen += 4;
			}
		}
		/* not erased 状態であり、サイズ不定であるため、全体をErase処理を実施する */
		else /* if( clearflg == D_RRH_FLASH_NOT_ERASED ) */
		{
			uiOutLen = D_RRH_QSPI_FLASH_CYCLICLOG_SIZE;
		}
		
		/* Clearする	*/
		endcd = BPF_HM_DEVC_QSPI_FLASH_ERASE( face, flashoffset, uiOutLen );
		
		if( endcd != D_SYS_OK )
		{
			/* Assert */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "FlashClear NG endcd=%d", endcd);
		}
		else
		{
			/* CyclicログClear状態をClear済みに更新	*/
			clearflg = D_RRH_FLASH_ERASED;
			(VOID)BPF_HM_DEVC_EEPROM_WRITE( clearstate[ face ], &clearflg);
		}
	}
	
	memset( &f_comw_orgData, 0, sizeof(f_comw_orgData) );
	
	/* 1つのBUFFERに共有メモリ情報をすべてコピーしてから圧縮する				*/
	/* 解凍後にデータの展開が必要なため、KEYIDと元の構造体サイズも設定しておく	*/
	for( logCnt=0; logCnt<D_RRH_FLASH_LOG_MAX; logCnt++ )
	{
		/* 共有メモリからログ情報取得 */
		endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
					f_comr_flash_log_save_info[logCnt].shmid,						/* 共有メモリ番号				*/
					(VOID **)&LogPtr,												/* 共有メモリアドレス			*/
					&errcd );														/* 詳細NGコード					*/
		
		if( endcd != BPF_RU_IPCM_OK )
		{
			/* ログ保存はスキップし処理継続 */
			continue;
		}
		
		/* 共有メモリの情報をまとめる	*/
		
		/* 共有メモリのKEYIDをコピー	*/
		memcpy( &f_comw_orgData[offset],
				(CHAR*)&f_comr_flash_log_save_info[logCnt].shmid,
				sizeof(f_comr_flash_log_save_info[logCnt].shmid) );
		offset += sizeof(f_comr_flash_log_save_info[logCnt].shmid);
		
		/* 共有メモリの構造体サイズをコピー	*/
		memcpy( &f_comw_orgData[offset],
				(CHAR*)&f_comr_flash_log_save_info[logCnt].logSize,
				sizeof(f_comr_flash_log_save_info[logCnt].logSize) );
		offset += sizeof(f_comr_flash_log_save_info[logCnt].logSize);
		
		/* 共有メモリの実体をコピー	*/
		memcpy( &f_comw_orgData[offset],
				(CHAR*)LogPtr,
				f_comr_flash_log_save_info[logCnt].logSize );
		offset += f_comr_flash_log_save_info[logCnt].logSize;
	}
	
	/* 共有メモリの情報を圧縮する	*/
	uiOutLen = 0;
	iRet = f_com_FA_encode(	(UCHAR*)f_comw_orgData,
							f_comw_compressData_cycle.logdata,
							offset,
							(D_RRH_FLASH_LOG_SIZE-4), 
							&f_comw_fa_dictionary,
							&uiOutLen);
	
	if (iRet != D_RRH_OK){
		/* 圧縮失敗時は処理終了	*/
		return;
	}
	
	/* 圧縮後のサイズをコピーする	*/
	memcpy( f_comw_compressData_cycle.compsize,
			(CHAR*)&uiOutLen,
			sizeof(uiOutLen));
	
	/* ログサイズ領域も含めてWriteする */
	uiOutLen += 4;
	
	/* 先にCyclicログClear状態をNotClearedに更新	*/
	clearflg = D_RRH_FLASH_NOT_ERASED;
	(VOID)BPF_HM_DEVC_EEPROM_WRITE( clearstate[ face ], &clearflg);
	
	/* Flash Write */
	endcd = BPF_HM_DEVC_QSPI_FLASH_WRITE(
				face,															/* Flash面						*/
				flashoffset,													/* Flash Offset					*/
				uiOutLen,														/* logサイズ					*/
				f_comw_compressData_cycle.compsize );							/* log領域						*/

	/* CyclicログClear状態を書き込み完了に更新	*/
	clearflg = D_RRH_FLASH_WRITE_FIN;
	(VOID)BPF_HM_DEVC_EEPROM_WRITE( clearstate[ face ], &clearflg);

	wData = face;
	endcd = BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_CYCLIC_LOG, &wData);
	if( endcd != BPF_HM_DEVC_COMPLETE )
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Flash cyclic log side write error=0x%08x", endcd);
	}
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_com_logSaveFlash] RETURN" );

	return;
}

/* @} */
