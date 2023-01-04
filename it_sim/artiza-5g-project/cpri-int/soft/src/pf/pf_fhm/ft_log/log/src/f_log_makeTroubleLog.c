/*!
 * @skip		$ld:$
 * @file		f_log_makeTroubleLog.c
 * @brief		ログスレッド 障害ログ出力処理
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
 * @brief		関数機能概要:障害ログ出力処理を行う
 * @note		関数処理内容.
 *				-# 障害種別によって出力先を変更する
 *				-# システム共通情報をファイルに出力する
 *				-# PFプロセス固有情報をファイルに出力する
 *				-# 出力ファイルを圧縮する
 *				-# INITプロセスへ障害ログ出力完了を通知する
 * @param		uiTroubleKind   [in]	障害種別
 * @retval		D_RRH_OK		0:正常終了
 * @retval		D_RRH_NG		1:異常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-002-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/10/29 ALPHA)横山 Create
 */
INT f_log_makeTroubleLog( UINT  uiTroubleKind )
{
	FILE	*fp					= NULL;
	UCHAR	output_file[256]	= {0};
	char	a_shcmd[200];
	UINT	output_dir			= 0;
	const char			*dbg_rsv_execcmd[] = {
			"cmd_pfdbg rsv dsp svstat"
	};

	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_log_makeTroubleLog] ENTER" );

	if( E_SYS_TROUBLE_KIND_NORMAL == uiTroubleKind )
	{
		/* 通常障害の場合	*/
		output_dir = E_BPF_LOG_CMDLOG_OUTDIR_ALM;
	}
	else if( E_SYS_TROUBLE_KIND_RST == uiTroubleKind )
	{
		/* 障害(RESET有)の場合	*/
		output_dir = E_BPF_LOG_CMDLOG_OUTDIR_ALM_RST;
	}
	else
	{
		/* ヘルスチェック断の場合	*/
		output_dir = E_BPF_LOG_CMDLOG_OUTDIR_ALM_HEALTCHK;
	}

	/********************************************/
	/* PFプロセス固有情報をファイルに出力する	*/
	/********************************************/
	fp = BPF_COM_LOG_OUTPUT_FILE_OPEN( "pf", output_dir, output_file );

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
		cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "FILE OPEN NG filename:%s errno:%d", output_file, errno  );
	}

	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_log_makeTroubleLog] RETURN" );

	return D_RRH_OK;
}

/* @} */
