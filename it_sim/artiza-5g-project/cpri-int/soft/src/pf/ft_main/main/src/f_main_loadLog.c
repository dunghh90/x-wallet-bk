/*!
 * @skip		$ld:$
 * @file		f_main_loadLog.c
 * @brief		pf_main 各種ログの展開
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 * @data  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015-2016
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */
#include "f_main_inc.h"
#include "fldc4.h"
#include "fldccom.h"

/* 解凍後データ	*/
/* 1つのログの取りうるサイズは最大で共有メモリのサイズ(512Kbyte)である	*/
/* これは全てのログで共通するため512Kbyteのサイズを用意する				*/
unsigned char			f_compw_compOut[D_RRH_FLASH_LOG_SIZE_BEFORE];

/* 圧縮データ	*/
/* 圧縮データが512Kbyteあるとは考えにくいがデータによって圧縮率が異なる	*/
/* どの程度のサイズを用意したら良いか不明であるため、					*/
/* 共有メモリのサイズ(512Kbyte)の領域を確保することとする				*/
unsigned char			f_compw_readData[D_RRH_QSPI_FLASH_TROUBLELOG_SIZE];

/* 解凍用temporary領域	*/
unsigned char			comw_fa_temporary[D_RRH_FLASH_LOG_DECOMP_TEMP] ;
/**< fldc4 compress dictionary  */
FLDC4_ELEM				comw_fa_dictionary;

extern int f_com_FA_decode(
 unsigned char *InBuf,		 	/* 圧縮データ用バッファアドレス 		*/
 unsigned char *OutBuf, 	 	/* 復元データ用バッファアドレス 		*/
 unsigned int	inlen,	 		/* 圧縮データのデータバイトサイズ	 	*/
 unsigned int	outbsz, 		/* 復元データ用バッファバイトサイズ		*/
 unsigned char *TmpBuf,			/* 復元テンポラリバッファアドレス		*/
 FLDC4_ELEM    *blsadr,			/* fldc4用圧縮辞書アドレス 				*/
 unsigned int  *outlen	 		/* 復元後のデータバイトサイズ		 	*/
);

/* loadLog用の面＆オフセット定義 */
typedef struct{
	UINT		face;
	UINT		offset;
}T_MAIN_LOADLOG;

T_MAIN_LOADLOG f_inir_loadLog[1][4] =
{
		/* face   ,  addr */
	{
		{ D_RRH_QSPI_FLASH_FACE0, D_RRH_QSPI_FLASH_OFFSET_TROUBLE0 },        /* E_RRH_QSPI_FLASH_TROUBLELOG_FACE0 */
		{ D_RRH_QSPI_FLASH_FACE0, D_RRH_QSPI_FLASH_OFFSET_TROUBLE1 },        /* E_RRH_QSPI_FLASH_TROUBLELOG_FACE1 */
		{ D_RRH_QSPI_FLASH_FACE0, D_RRH_QSPI_FLASH_OFFSET_TROUBLE2 },        /* E_RRH_QSPI_FLASH_TROUBLELOG_FACE2 */
		{ D_RRH_QSPI_FLASH_FACE0, D_RRH_QSPI_FLASH_OFFSET_HEALTH   }         /* D_RRH_QSPI_FLASH_OFFSET_HEALTH */
	}
};

/*!
 * @brief		f_main_loadLog_develop
 * @note		FlashROMからログの展開処理を行う.
 *					-# Flash面情報取得.
 *					- Flash面情報取得に失敗した場合.
 *						-# アサートログを出力.
 *						-# D_SYS_NGを返却し処理終了.
 *					-# 再取得フラグをログ数分ONに設定.
 *					-# Active面のログ情報を展開.
 *						-# RAMに展開するログ数分ループ(以下のログを取得).
 *								-# Register Access Log.
 *								-# CPRI LAPB/L3送受信ログ.
 *								-# Alarm Hisotry Log.
 *								-# CPRI L1統計情報.
 *								-# CPRI L1復旧断履歴.
 *								-# CPRI L1リンク障害ログ.
 *								-# CPRI L2統計情報.
 *								-# CPRI L3統計情報.
 *								-# CPRI L3復旧断履歴.
 *						- ログ種別が同じ場合.
 *							-# ループ再開.
 *						- Flashのサイズとログのサイズが異なる場合.
 *							-# ログを出力しループ終了.
 *						- ログのオフセット位置が異常(0x00D00000未満)の場合.
 *							-# ログを出力しループ終了.
 *						-# 圧縮サイズをQSPI-FLASHからread.			BPF_HM_DEVC_QSPI_FLASH_READ関数コール
 *						-# 圧縮サイズ使用し、ログ解凍処理を実施.	f_com_FA_decode関数コール
 *						- 解凍に失敗した場合
 *							-# ログを出力しループ終了.
 *						- 解凍後の実サイズが以上の場合
 *							-# ログを出力しループ終了.
 *						-# 共有メモリへ展開.
 *							- 共有メモリアクセス失敗した場合.
 *							-# アサートログを出力しループ終了.
 *						-# 再取得フラグをOFFに変更.
 *						- SYS Logの場合.
 *							-# /var/logにsyslog.tar.gzとして出力する.
 *					- 再取得フラグがONの場合.
 *						-# 異常終了を返却し、処理終了.
 * @param		-
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-MAIN-003-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 * @data  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 */
UINT f_main_loadLog_develop(UCHAR cyclic_log_face)
{
	INT						rtn_bpf = 0;
	UCHAR					*shmem = NULL;
	INT						errcd = 0;
	UINT					logCnt;
	INT						iRet;
	UINT					uiOutLen;
	UINT					uiFaceCnt;
	USHORT					usRetry[D_RRH_FLASH_LOG_MAX];
	UINT					uiOffset;
	UINT					uiFileSize;
	UINT					uiKeyId;
	UINT					uiFaceState[ 2 ] = { D_DU_EEP_CYCLIC_CLEAR_FLG0, D_DU_EEP_CYCLIC_CLEAR_FLG1 };
	UCHAR					uiState;


	/************************************************************************/
	/*																		*/
	/*	本関数ではASSERTログに展開するため、Assertログ取得は禁止			*/
	/*																		*/
	/************************************************************************/

	BPF_COM_PLOG_TRACE_PROC( D_RRH_LOG_AST_LV_ENTER, "[f_main_loadLog_develop] ENTER" );
	
	/************************************************************************************/
	/* 初めに指定面のFlashからログを取得する											*/
	/* ログの内容が壊れている、解凍できない、型が違うなどの場合は逆面からログを取得する	*/
	/* そのため2回ループし、最初は指定面、次は逆面で取得処理を行う						*/
	/************************************************************************************/
	/* Flash⇒RAM展開するログ数分ループ	*/
	for( logCnt=0; logCnt<D_RRH_FLASH_LOG_MAX; logCnt++ )
	{
		/* 再取得フラグをONにしておく						*/
		/* 初回で正常に取得できたらOFFし、再取得を行わない	*/
		usRetry[logCnt] = D_RRH_ON;
	}
	
	/* EEPROM面が0/1の時のみ展開する	*/
	if( (cyclic_log_face == 0) || (cyclic_log_face == 1) )
	{
		/* QSPI Flash面数ループ	*/
		for( uiFaceCnt = 0; uiFaceCnt < 2; uiFaceCnt++ )
		{
			rtn_bpf = BPF_HM_DEVC_EEPROM_READ(uiFaceState[cyclic_log_face], &uiState);
			if( rtn_bpf != BPF_HM_DEVC_COMPLETE )
			{
				BPF_COM_PLOG_TRACE_PROC( D_RRH_LOG_AST_LV_ERROR, "EEPROM READ NG");
				break;
			}
			
			/* 書き込み完了時以外は展開できない	*/
			if( uiState != D_RRH_FLASH_WRITE_FIN )
			{
				/* 逆面をTRY	*/
				cyclic_log_face = ~cyclic_log_face & 1;
				continue;
			}
			
			/* Flashに展開したログ情報を取得する	*/
			memset( f_mainw_orgData, 0, sizeof(f_mainw_orgData) );
			
			rtn_bpf = BPF_HM_DEVC_QSPI_FLASH_READ(	(UINT)cyclic_log_face,
													D_RRH_QSPI_FLASH_OFFSET_INFO,
													sizeof(f_mainw_orgData),
													(UCHAR *)f_mainw_orgData);
			
			if(BPF_HM_DEVC_COMPLETE != rtn_bpf){
				BPF_COM_PLOG_TRACE_PROC( D_RRH_LOG_AST_LV_ERROR, "Flash Log Info load NG. rtn_bpf=%08x", rtn_bpf);
				/* 逆面をTRY	*/
				cyclic_log_face = ~cyclic_log_face & 1;
				continue;
			}

			/************************************************************************************/
			/* 解凍処理を行う																	*/
			/************************************************************************************/
			uiFileSize =(f_mainw_orgData[3] << 24) + 
						(f_mainw_orgData[2] << 16) + 
						(f_mainw_orgData[1] <<  8) + 
						(f_mainw_orgData[0]);
			
			/* MAX1Mbyte	*/
			if( (uiFileSize >= D_RRH_FLASH_LOG_SIZE) ||
				(uiFileSize == 0 ))
			{
				/* 正常なデータでない場合は展開しない	*/
				BPF_COM_PLOG_TRACE_PROC( D_RRH_LOG_AST_LV_ERROR, "File Size error. uiFileSize = %d", uiFileSize);
				/* 逆面をTRY	*/
				cyclic_log_face = ~cyclic_log_face & 1;
				continue;
			}
			
			memset( f_compw_compOut, 0x00, sizeof(f_compw_compOut) );
			
			iRet = f_com_FA_decode(	&f_mainw_orgData[4],							/* 圧縮データ用バッファアドレス 		*/
									f_compw_compOut,								/* 復元データ用バッファアドレス 		*/
									uiFileSize,										/* 圧縮データのデータバイトサイズ	 	*/
									sizeof(f_compw_compOut),						/* 復元データ用バッファバイトサイズ		*/
									comw_fa_temporary,								/* 復元テンポラリバッファアドレス		*/
									&comw_fa_dictionary,							/* fldc4用圧縮辞書アドレス 				*/
									&uiOutLen);										/* 復元後のデータバイトサイズ		 	*/

			if( NORMAL != iRet )
			{
				/* 解凍に失敗した場合は展開しない	*/
				BPF_COM_PLOG_TRACE_PROC( D_RRH_LOG_AST_LV_ERROR, "Flash Log decompress NG. iRet = %d", iRet);
				/* 逆面をTRY	*/
				cyclic_log_face = ~cyclic_log_face & 1;
				continue;
			}

			BPF_COM_PLOG_TRACE_PROC( D_RRH_LOG_AST_LV_INFO, "Face:%d Log Size:0x%08x(%d) Log Dec Size:0x%08x(%d)",cyclic_log_face, uiFileSize,uiFileSize,uiOutLen,uiOutLen );

			/* オフセット初期化	*/
			uiOffset = 0;

			/* Flash⇒RAM展開するログ数分ループ	*/
			for( logCnt=0; logCnt<D_RRH_FLASH_LOG_MAX; logCnt++ )
			{
				/* 再取得フラグがOFFの場合はログ取得を行わない	*/
				if( D_RRH_OFF == usRetry[logCnt] )
				{
					continue;
				}

				/* 解凍後のデータから共有メモリのKEYを取得する	*/
				uiKeyId =	(f_compw_compOut[ uiOffset + 3 ] << 24) +
							(f_compw_compOut[ uiOffset + 2 ] << 16) +
							(f_compw_compOut[ uiOffset + 1 ] <<  8) +
							 f_compw_compOut[ uiOffset     ];
				/* オフセット更新	*/
				uiOffset += 4;

				/* コピーするサイズを取得する	*/
				uiOutLen = (f_compw_compOut[ uiOffset + 3 ] << 24) +
						   (f_compw_compOut[ uiOffset + 2 ] << 16) +
						   (f_compw_compOut[ uiOffset + 1 ] <<  8) +
						    f_compw_compOut[ uiOffset     ];
				/* オフセット更新	*/
				uiOffset += 4;
				
				/* ログサイズ(uiOutLen) に異常なデータが入っていた場合は処理を抜ける */
				if(		( uiOutLen > D_RRH_QSPI_FLASH_CYCLICLOG_SIZE )
					||	( (uiOffset + uiOutLen) > sizeof(f_compw_compOut) ) )
				{
					BPF_COM_PLOG_TRACE_PROC( D_RRH_LOG_AST_LV_ERROR, "Invalid Cyclic Log Size(%d,%d,0x%08x)", logCnt,uiKeyId,uiOutLen );
					break;
				}
				
				/* 退避時に使用したテーブルのKEYと構造体サイズをそのまま比較し、*/
				/* 共有メモリのKEYと構造体サイズが一致すれば展開する			*/
				/* そうでなければ共有メモリを破壊する可能性があるため展開しない	*/
				if( ( uiKeyId != f_comr_flash_log_save_info[ logCnt ].shmid   ) ||
					( uiOutLen!= f_comr_flash_log_save_info[ logCnt ].logSize ) )
				{
					/* オフセット更新	*/
					uiOffset += uiOutLen;
					BPF_COM_PLOG_TRACE_PROC( D_RRH_LOG_AST_LV_ERROR, "Detected Log mismatch.flash:id:0x%x size:0x%x tbl:id:0x%x size:0x%x", 
							uiKeyId,uiOutLen,
							f_comr_flash_log_save_info[ logCnt ].shmid,f_comr_flash_log_save_info[ logCnt ].logSize);
					break;
				}
				
				/* 共有メモリアドレス取得 */
				rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(	uiKeyId,
														(VOID **)&shmem,
														&errcd );
				
				if(BPF_RU_IPCM_OK != rtn_bpf)
				{
					BPF_COM_PLOG_TRACE_PROC( D_RRH_LOG_AST_LV_ERROR, "Shared memory address NG. rtn_bpf=%08x", rtn_bpf);
					break;
				}

				/* 共有メモリへの展開を行う	*/
				memcpy( shmem, &f_compw_compOut[uiOffset], uiOutLen );

				/* オフセット更新	*/
				uiOffset += uiOutLen;
			}
			/* 解凍->展開が正常に取得できたのでbreakする	*/
			break;
		}
	}
	

	BPF_COM_PLOG_TRACE_PROC( D_RRH_LOG_AST_LV_RETURN, "[f_main_loadLog_develop] RETURN" );

	return D_SYS_OK;
}

/*!
 * @brief		f_main_loadLog_outputFile
 * @note		ログ出力展開処理を行う.
 *					-# ログ出力
 * @param		-
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-MAIN-003-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @data  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date		2015/08/27 ALPHA) miyazaki modify to TDD-RRE-Zynq 
 */
UINT f_main_loadLog_outputFile(UCHAR filename_info)
{
	INT						rtn_bpf = 0;
	INT						cnt;
	UINT					pattern = 0;
	char					filename[4][128] = {"/var/log/trouble_rst00.tar.gz",
												"/var/log/trouble_rst01.tar.gz",
												"/var/log/trouble_rst02.tar.gz",
												"/var/log/healthchklog.tar.gz"};
	FILE* 					output_w;							/* 出力先ファイルポインタ	*/
	UINT					uiFileSize;
	size_t					fw_cnt;
	
	UINT					max;
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_loadLog_outputFile] ENTER" );

	if(filename_info >= E_RRH_QSPI_FLASH_TROUBLELOG_FACE_MAX){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "TROUBLELOG was not saved yet info=%08x", filename_info);
		filename_info = 3;
		max = 1;
	}else{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "TROUBLELOG was saved info=%08x", filename_info);
		max = 2;
	}

	/* ループの1回目は障害ログ用。ループの2回目はヘルスチェックログ用 */
	for( cnt = 0 ; cnt < max ;  cnt++ )
	{
		rtn_bpf = BPF_HM_DEVC_QSPI_FLASH_READ(	
						f_inir_loadLog[pattern][filename_info].face,	/* 面			*/
						f_inir_loadLog[pattern][filename_info].offset,	/* オフセット	*/
						D_RRH_QSPI_FLASH_TROUBLELOG_SIZE,				/* SIZE			*/
						(UCHAR *)f_compw_readData);						/* temp領域		*/

		if(BPF_HM_DEVC_COMPLETE != rtn_bpf)
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Flash Log load NG. rtn_bpf=%08x", rtn_bpf );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_loadLog] RETURN" );
			return D_SYS_NG;
		}

		/* 冒頭にゴミが入っていたら異常データなので展開しない				*/
		uiFileSize =(f_compw_readData[3] << 24) + 
					(f_compw_readData[2] << 16) + 
					(f_compw_readData[1] <<  8) + 
					(f_compw_readData[0]);
		
		/* 正常なデータの場合にのみ出力する	*/
		if( (uiFileSize < D_RRH_QSPI_FLASH_TROUBLELOG_SIZE) &&
			(uiFileSize != 0) )
		{
			/* /var/log/配下にログを出力 */
			output_w = fopen( filename[filename_info] , "wb");
			
			if( NULL != output_w )
			{
				/* ファイルに出力する	*/
				fw_cnt = fwrite( &f_compw_readData[4], 1, uiFileSize, output_w );
				if(fw_cnt < uiFileSize)
				{
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "fwrite error. fw_cnt=%08x", fw_cnt);
				}
				
				/* ファイルクローズ	*/
				fclose(output_w);
			}
		}
		
		/* ループ1回目は障害ログ[0～2]のいずれかに対する処理を実施。 */
		/* ループ2回目はヘルスチェックログに対する処理を実施。       */
		/* ループ2回目にいくのでfilename_infoを3に設定しておくことで */
		/* f_inir_loadLogおよびfilenameからヘルスチェック関連の      */
		/* 情報を正しく取得する。                                    */
		filename_info = 3;	
	}

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_loadLog_outputFile] RETURN" );

	return D_SYS_OK;
}



/*!
 * @brief		f_main_loadLog
 * @note		FlashROMからログの展開処理を行う.
 *					-# Flash面情報取得.
 *						-# ログ展開
 *						-# ログ出力
 * @param		-
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-MAIN-003-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 * @data  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 */
UINT f_main_loadLog()
{
	UINT					rtn			= D_SYS_OK;
	INT						rtn_bpf		= 0;
	UCHAR					rData;
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_loadLog] ENTER" );

	/************************************************************/
	/* Log展開													*/
	/************************************************************/
	/* Flash面情報取得 */
	rtn_bpf = BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_CYCLIC_LOG, &rData);
	if( rtn_bpf != BPF_HM_DEVC_COMPLETE )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "EEPROM TROUBLELOG Side read NG. rtn_bpf=%08x", rtn_bpf);
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_loadLog] RETURN" );
		return D_SYS_OK;
	}
	
	rtn = f_main_loadLog_develop(rData);
	if(rtn != D_SYS_OK)
	{
		/* エラーログはサブ関数で出力している内容で問題なし */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_loadLog] RETURN" );
		return D_SYS_OK;
	}
	
	/************************************************/
	/* 最新の障害ログをファイルに出力する			*/
	/************************************************/
	/* 障害ログ退避面情報取得 */
	rtn_bpf = BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_TROUBLE_LOG, &rData);
	if( rtn_bpf != BPF_HM_DEVC_COMPLETE )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " EEPROM EVTLOG Side read NG. rtn_bpf=%08x", rtn_bpf);
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_loadLog] RETURN" );
		return D_SYS_OK;
	}
	
	rtn = f_main_loadLog_outputFile(rData);
	if(rtn != D_SYS_OK)
	{
		/* エラーログはサブ関数で出力している内容で問題なし */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_loadLog] RETURN" );
		return D_SYS_OK;
	}


	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_loadLog] RETURN" );

	return D_SYS_OK;
}
/* @} */
