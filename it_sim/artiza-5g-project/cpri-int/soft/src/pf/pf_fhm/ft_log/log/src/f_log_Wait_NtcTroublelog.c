/*!
 * @skip		$ld:$
 * @file		f_log_Wait_NtcTroublelog.c
 * @brief		ログスレッド 運用ログ出力完了待ち状態_運用ログ出力完了通知受信処理
 * @author		ALPHA)yokoyama
 * @date		2013/10/29 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_LOG_LOG
 * @{
 */

#include "f_log_inc.h"
#include "f_dbg_com_ext.h"
#include "f_dbg_thread_ext.h"
#include <errno.h>

extern VOID f_cpr_logPrint(FILE *);
extern VOID f_fmng_logPrint(FILE *);
extern VOID f_nego_logPrint(FILE *);
extern FILE* f_cmn_com_assert_th_print_all( FILE* fp );

/*!
 * @brief		関数機能概要:運用ログ出力完了通知受信時処理を行う
 * @note		関数処理内容.
 *				-# システム共通情報をファイルに出力する
 *				-# PFプロセス固有情報をファイルに出力する
 *				-# 障害ログを取得する
 *				-# 出力ファイルを圧縮し、ファイルサイズを取得する
 *				-# ファイルサイズが指定MAXサイズより大きい場合は以下の処理を行う
 *					-# 作成したファイルを削除する
 *					-# 障害ログを指定フォルダにコピーし、依頼のあったファイル名にRENAMEする
 *					-# 対象ファイルのファイルサイズを取得する
 *				-# API発行元プロセスへ障害ログファイル作成完了通知を送信する。
 *				-# 状態を通常状態へ遷移させる
 * @param		*msgP			[in]	受信メッセージ
 * @retval		D_RRH_OK		0:正常終了
 * @retval		D_RRH_NG		1:異常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-003-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/10/29 ALPHA)横山 Create
 *  @date    	2015/11/13 FPT)Lay 障害ログ実装改善
 */
INT f_log_Wait_NtcTroublelog(VOID* msgP)
{
	FILE				*fp;
	CHAR				output_file[512];
	CHAR				a_shcmd[512];
	UINT				uiFileSize = 0;
	UINT				log_face = 0;
	UINT				okng;
	UINT				length;
	INT					ret;
	UINT				sysKind;
	CHAR				troublelog[D_SYS_MAX_SYSNUM][3][128] = {	{	"cp -rf /var/log/trouble00.tar.gz /var/log/sendlog_3g  > /dev/null 2>&1",
																		"cp -rf /var/log/trouble01.tar.gz /var/log/sendlog_3g  > /dev/null 2>&1",
																		"cp -rf /var/log/trouble02.tar.gz /var/log/sendlog_3g  > /dev/null 2>&1"},
																	{	"cp -rf /var/log/trouble00.tar.gz /var/log/sendlog  > /dev/null 2>&1",
																		"cp -rf /var/log/trouble01.tar.gz /var/log/sendlog  > /dev/null 2>&1",
																		"cp -rf /var/log/trouble02.tar.gz /var/log/sendlog  > /dev/null 2>&1"} };

	const char			*dbg_rsv_execcmd[] = {
			"cmd_pfdbg rsv dsp svstat"
	};
	UINT				dirKind[D_SYS_MAX_SYSNUM] = { E_BPF_LOG_CMDLOG_OUTDIR_SEND_3G, E_BPF_LOG_CMDLOG_OUTDIR_SEND };

	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_INFO, "Online Troublelog create start" );
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif
	
	sysKind = ((T_SYS_RUN_LOG_OUTPUT_NTC *)msgP)->head.uiSignalkind;
	
	/********************************************/
	/* PFプロセス固有情報をファイルに出力する	*/
	/********************************************/
	fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "pf", dirKind[sysKind], (UCHAR*)output_file );
	if( fp != NULL )
	{
		/* 各種debug command実行 */
		/* pf_rsvデバッグコマンド	*/
		fprintf( fp, "[%s]\n", dbg_rsv_execcmd[0])	;
		strcpy( a_shcmd, dbg_rsv_execcmd[0] ) ;
		dbgcmd_exec_pf_rsv( a_shcmd, fp );

		fprintf(fp, "[cpr log]\n");
		f_cpr_logPrint(fp);

		fprintf(fp, "[fmng log]\n");
		f_fmng_logPrint(fp);

		fprintf(fp, "[nego log]\n");
		f_nego_logPrint(fp);

		fprintf(fp, "[Internal assert log]\n");
		fp = f_cmn_com_assert_th_print_all(fp);
		
		BPF_RU_HRTM_TIMER_LOG_PRINT(fp);

		(VOID)BPF_COM_LOG_OUTPUT_FILE_CLOSE( fp );
	}
	else
	{
		cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "FILE OPEN NG" );
		okng = D_RRH_NG;

		/* API発行元プロセスへ障害ログファイル作成完了通知を送信する */
		f_log_send_APITroubleLogFinNtc( f_logw_rspMngTbl[sysKind][E_LOG_RSP_API_TLOGGET].srcPID,
										f_logw_rspMngTbl[sysKind][E_LOG_RSP_API_TLOGGET].srcTID,
										okng,
										output_file,
										uiFileSize,
										sysKind );

		/* スレッド状態遷移(通常状態) */
		f_logw_thdState[ sysKind ] = D_LOG_THD_STS_ACT;
		cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_INFO, "Online Troublelog create end(%d)", sysKind);
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
		return D_RRH_OK;
	}

	/* 出力済み障害ログを取得する							*/
	/* EEPROMを見る											*/
	(VOID)BPF_HM_DEVC_EEPROM_READ( D_RRH_EEP_TROUBLE_LOG, (UCHAR*)&log_face );

	/* 障害ログを/var/log/sendlogへコピーする（①）			*/
	ret = system(troublelog[sysKind][log_face]);
	if( D_RRH_MNG == ret )
	{
		cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "command failed. %s", a_shcmd );
	}

//TODO 止血 HealthCheckLogを障害ログに含めない
//	/* HealthCheckLogをコピーする	*/
//	memset( a_shcmd, 0x00, sizeof(a_shcmd) );
//	snprintf( a_shcmd, sizeof(a_shcmd), "cp -rf /var/log/healthchklog.tar.gz %s", f_comr_runLogPath[sysKind] );
//	ret = system(a_shcmd);
//
//	if( D_RRH_MNG == ret )
//	{
//		cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "command failed. %s", a_shcmd );
//	}

	/* 出力ファイル(/var/log/sendlog)を圧縮する				*/
	memset( a_shcmd, 0, sizeof(a_shcmd) );
	snprintf( a_shcmd, sizeof(a_shcmd), "tar zc -f %s.tar.gz %s 1>/dev/null 2>&1", f_comr_runLogPath[sysKind], f_comr_runLogPath[sysKind] );
	ret = system(a_shcmd);
	if( D_RRH_MNG == ret )
	{
		cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "command failed. %s", a_shcmd );
	}
	else
	{
		cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_INFO, "Compress SENDLOG1(%d)", sysKind);
	}
	/* APIで指定されたファイル名にする必要があるので、圧縮した後にRENAMEする(②)	*/
	memset( a_shcmd, 0, sizeof(a_shcmd) );
	snprintf( a_shcmd, sizeof(a_shcmd), "mv -f %s.tar.gz %s%s",
				f_comr_runLogPath[sysKind],
				(const CHAR*)f_logw_troublelog_get_info.filename,
				" > /dev/null 2>&1" );
	ret = system(a_shcmd);
	if( D_RRH_MNG == ret )
	{
		cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "command failed. %s", a_shcmd );
	}

	/* API発行元へファイルサイズの通知が必要なので、ファイルサイズを取得する	*/
	fp = fopen( (const CHAR*)f_logw_troublelog_get_info.filename, "r");
	if( fp != NULL )
	{
		fseek(fp, 0, SEEK_END);
		uiFileSize = ftell( fp );
		fclose(fp);
	}
	else
	{
		okng = D_RRH_NG;
		cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "FILE OPEN NG2 %s(%d)", a_shcmd, sysKind);
		
		memset( a_shcmd, 0, sizeof(a_shcmd) );
		snprintf( a_shcmd, sizeof(a_shcmd), "rm -rf %s/* > /dev/null 2>&1", f_comr_runLogPath[sysKind] );
		ret = system( a_shcmd );
		
		if( D_RRH_MNG == ret )
		{
			cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "command failed." );
		}

		/* API発行元プロセスへ障害ログファイル作成完了通知を送信する */
		f_log_send_APITroubleLogFinNtc( f_logw_rspMngTbl[sysKind][E_LOG_RSP_API_TLOGGET].srcPID,
										f_logw_rspMngTbl[sysKind][E_LOG_RSP_API_TLOGGET].srcTID,
										okng,
										(CHAR*)f_logw_troublelog_get_info.filename,
										uiFileSize,
										sysKind );

		/* スレッド状態遷移(通常状態) */
		f_logw_thdState[ sysKind ] = D_LOG_THD_STS_ACT;
		cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_INFO, "Online Troublelog create end");
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
		return D_RRH_OK;
	}

	/* APIで指定されたファイルを超える場合はファイルサイズを小さくする必要がある	*/
	/* その際は運用ログは送信しない													*/
	if( uiFileSize > f_logw_troublelog_get_info.size )
	{
		cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_WARNING, "[f_log_Wait_NtcTroublelog] SIZE OVER(%d,%d)", uiFileSize, sysKind );

		/* ②で作成したファイルを削除する 			*/
		memset( a_shcmd, 0 , sizeof(a_shcmd) );
		strncpy( a_shcmd, "rm -rf ", sizeof(a_shcmd) );
		length = strlen( a_shcmd );
		strncat( a_shcmd, (const CHAR*)f_logw_troublelog_get_info.filename, sizeof(a_shcmd) -length );  /* pgr0585 a_shcmdの最大文字数を超えないことは自明なため問題無し */
		ret = system(a_shcmd);
		if( D_RRH_MNG == ret )
		{
			cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "command failed. %s", a_shcmd );
		}

		/* sendlog配下をクリアする	*/
		memset( a_shcmd, 0, sizeof(a_shcmd) );
		snprintf( a_shcmd, sizeof(a_shcmd), "rm -rf %s/* > /dev/null 2>&1", f_comr_runLogPath[sysKind] );
		ret = system( a_shcmd );
		
		if( D_RRH_MNG == ret )
		{
			cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "command failed." );
		}

		/* 障害ログを/var/log/sendlogへコピーする（①）			*/
		ret = system(troublelog[sysKind][log_face]);
		if( D_RRH_MNG == ret )
		{
			cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "command failed. %s", troublelog[sysKind][log_face] );
		}

//TODO 止血 HealthCheckLogを障害ログに含めない
//		/* HealthCheckLogをコピーする	*/
//		memset( a_shcmd, 0, sizeof(a_shcmd) );
//		snprintf( a_shcmd, sizeof(a_shcmd), "cp -rf /var/log/healthchklog.tar.gz %s", f_comr_runLogPath[sysKind] );
//		ret = system(a_shcmd);
//
//		if( D_RRH_MNG == ret )
//		{
//			cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "command failed." );
//		}

		/* 出力ファイル(/var/log/sendlog)を圧縮する				*/
		memset( a_shcmd, 0, sizeof(a_shcmd) );
		snprintf( a_shcmd, sizeof(a_shcmd), "tar zc -f %s.tar.gz %s 1>/dev/null 2>&1", f_comr_runLogPath[sysKind], f_comr_runLogPath[sysKind] );
		ret = system(a_shcmd);
		
		if( D_RRH_MNG == ret )
		{
			cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "command failed. %s", a_shcmd );
		}
		else
		{
			cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_INFO, "Compress SENDLOG2(%d)",sysKind);
		}
		/* APIで指定されたファイル名にする必要があるので、圧縮した後にRENAMEする(②)	*/
		memset( a_shcmd, 0, sizeof(a_shcmd) );
		snprintf( a_shcmd, sizeof(a_shcmd), "mv -f %s.tar.gz %s%s",
					f_comr_runLogPath[sysKind],
					(const CHAR*)f_logw_troublelog_get_info.filename,
					" > /dev/null 2>&1");
		ret = system(a_shcmd);
		
		if( D_RRH_MNG == ret )
		{
			cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "command failed. %s", a_shcmd );
		}

		/* API発行元へファイルサイズの通知が必要なので、ファイルサイズを取得する	*/
		fp = fopen( (const CHAR*)f_logw_troublelog_get_info.filename, "r");
		if( fp == NULL )
		{
			memset( output_file, 0 ,sizeof(output_file) );
			strncpy( output_file, (const CHAR*)f_logw_troublelog_get_info.filename, sizeof(output_file) );
			okng = D_RRH_NG;
			cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "FILE OPEN NG3 %s", a_shcmd );
			
			memset( a_shcmd, 0, sizeof(a_shcmd) );
			snprintf( a_shcmd, sizeof(a_shcmd), "rm -rf %s/* > /dev/null 2>&1", f_comr_runLogPath[sysKind] );
			ret = system(a_shcmd);
			
			if( D_RRH_MNG == ret )
			{
				cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "command failed." );
			}

			/* API発行元プロセスへ障害ログファイル作成完了通知を送信する */
			f_log_send_APITroubleLogFinNtc( f_logw_rspMngTbl[sysKind][E_LOG_RSP_API_TLOGGET].srcPID,
											f_logw_rspMngTbl[sysKind][E_LOG_RSP_API_TLOGGET].srcTID,
											okng,
											output_file,
											uiFileSize,
											sysKind );

			/* スレッド状態遷移(通常状態) */
			f_logw_thdState[ sysKind ] = D_LOG_THD_STS_ACT;
			cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_INFO, "Online Troublelog create end(%d)", sysKind);
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
			return D_RRH_OK;
		}

		fseek(fp, 0, SEEK_END);
		uiFileSize = ftell( fp );
		fclose(fp);
	}

	/* 障害ログ取得完了通知情報を設定する	*/
	if( uiFileSize == 0 )
	{
		/* サイズが0ということは正常にファイル作成が行われなかったということなのでNGを返す	*/
		okng = D_RRH_NG;
	}
	else
	{
		okng = D_RRH_OK;
	}
	
	memset( a_shcmd, 0, sizeof(a_shcmd) );
	snprintf( a_shcmd, sizeof(a_shcmd), "rm -rf %s/* > /dev/null 2>&1", f_comr_runLogPath[sysKind] );
	ret = system(a_shcmd);

	if( D_RRH_MNG == ret )
	{
		cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "command failed." );
	}

	/* API発行元プロセスへ障害ログファイル作成完了通知を送信する */
	f_log_send_APITroubleLogFinNtc( f_logw_rspMngTbl[sysKind][E_LOG_RSP_API_TLOGGET].srcPID,
									f_logw_rspMngTbl[sysKind][E_LOG_RSP_API_TLOGGET].srcTID,
									okng,
									(CHAR*)f_logw_troublelog_get_info.filename,
									uiFileSize,
									sysKind );

	/* スレッド状態遷移(通常状態) */
	f_logw_thdState[ sysKind ] = D_LOG_THD_STS_ACT;
	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_INFO, "Online Troublelog create end(%d)",sysKind );
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
	return D_RRH_OK;
}

/* @} */
