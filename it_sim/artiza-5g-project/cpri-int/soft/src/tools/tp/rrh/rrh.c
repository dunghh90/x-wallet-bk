/*!
 *  @skip	$Id:$
 *  @file	rrh.c
 *  @brief	rrh command main routine
 *  @author	FJT)Taniguchi
 *  @date	2013/11/18 FJT)Taniguchi create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup	RRH_TP_CMD	rrh command 
 * @brief		rrh command 
 * @note		This is rrh command function.
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <mruby.h>
#include <mruby/proc.h>
#include <mruby/array.h>
#include <mruby/string.h>
#include <mruby/compile.h>
#include <mruby/dump.h>
#include <mruby/variable.h>
#include "rrhlib.h"
#include "rrhlib_local.h"

/*!
 * @brief		rrhコマンドパラメータ解析処理
 * @note
	<pre>
	コマンドパラメータを取得する処理である.
	コマンドは"rrh -xxxx yyyy zzzz ....."という形式で入力される.
	xxxxはスクリプトのファイル名であり、yyyy、zzzz、....はスクリプト起動時に渡すパラメータとなる.
	本処理では、xxxxファイルをopenし、yyyy,zzzz,....をコマンド情報構造体に格納する処理である.
	</pre>
 * @param		mrb		[in]	mruby statusポインタ
 * @param		argc	[in]	コマンドパラメータ数
 * @param		argv	[in]	コマンドパラメータポインタ
 * @param		args	[out]	コマンド情報構造体ポインタ
 * @return		結果コード
 * @retval		EXIT_SUCCESS パラメータ取得成功
 * @retval		EXIT_FAILURE パラメータ取得失敗
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
static int rrh_getargs(mrb_state *mrb, int argc, char **argv, rrh_t_args *args)
{
	static const rrh_t_args args_zero = { 0 };
	*args = args_zero;

	argc--;
	argv++;
	if (argv[0][0] != '-')
	{
		return EXIT_FAILURE;
	}
	if(strlen(argv[0]) <= 1)
	{
		return EXIT_FAILURE;
	}
	args->fp = fopen(&argv[0][1],"r");
	if (args->fp == NULL)
	{
		return EXIT_FAILURE;
	}
	args->cmdline = argv[0];
	argc--;
	argv++;
	args->argv = (char **)mrb_realloc(mrb, args->argv, sizeof(char*) * (argc + 1));
	memcpy(args->argv, argv, (argc+1) * sizeof(char*));
	args->argc = argc;
	return EXIT_SUCCESS;
}

/*!
 * @brief	rrhコマンド内部リソース解放処理
 * @note
	<pre>
	rrhコマンド実行時に使用するリソースを解放する処理である
	</pre>
 * @param		mrb		[in]	mruby statusポインタ
 * @param		args	[in]	コマンド情報構造体ポインタ
 * @return		N/A
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
static void rrh_rscclear(mrb_state *mrb, rrh_t_args *args)
{
	if(args->fp)
	{
		fclose(args->fp);
	}
	if(args->argv)
	{
		mrb_free(mrb, args->argv);
	}
	mrb_close(mrb);
	return;
}

/*!
 * @brief	rrhコマンドメイン処理
 * @note
	<pre>
	rrhコマンドのメイン処理である
	</pre>
 * @param		argc	[in]	コマンドパラメータ数
 * @param		argv	[in]	コマンドパラメータポインタ
 * @return		結果
 * @retval		EXIT_SUCCESS OK
 * @retval		EXIT_FAILURE NG
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
int main(int argc, char **argv)
{

	mrb_value ARGV;
	mrb_state* mrb;
	rrh_t_args args;
	mrb_value value;
	int rtn = EXIT_FAILURE;
	mrbc_context *ctxt;
	mrb_sym zero_sym;
	int count;

	if(argc <= 1)
	{
		rrhlib_usage(argv[0]);
		return EXIT_FAILURE;
	}

#ifndef OPT_RRH_CT
	if( chdir("/opt/tp") != 0)
	{
		return EXIT_FAILURE;
	}
#endif
    if (strncmp(argv[1], "-talise", 7) == 0) {  /* 先頭7文字一致判定 */
    	/* プロセス管理機能割付けを行う */
    	if ((rtn = BPF_RM_SVRM_ASSIGN(D_DBG_TP_PROCESS_DESC)) != BPF_RM_SVRM_COMPLETE) {
    		printf("talise TP BPF Assign NG rc:%x\n", rtn);
    		return rtn;
    	}
    	/* Taliseコマンド実行 */
//    	rtn = talise_cmd_main(argc, argv);
    	/* プロセス管理機能解放、Taliseコマンド実行結果関係無しに行う */
    	(void)BPF_RM_SVRM_UNASSIGN(D_DBG_TP_PROCESS_DESC);
    	return rtn;
    }

	if((mrb = mrb_open()) == NULL)
	{
		return rtn;
	}
	if((rtn = rrh_getargs(mrb, argc, argv, &args)) == EXIT_FAILURE)
	{
		rrh_rscclear(mrb, &args);
		return rtn;
	}
	/* 関数登録 */
	rrhlib_install(mrb);
	/* scriptに渡す、ARGVパラメータの作成 */
	ARGV = mrb_ary_new_capa(mrb, args.argc);
	for(count = 0; count < args.argc; count++)
	{
		mrb_ary_push(mrb, ARGV, mrb_str_new(mrb, args.argv[count], strlen(args.argv[count])));
	}
	mrb_define_global_const(mrb, "ARGV", ARGV);
	ctxt = mrbc_context_new(mrb);
	zero_sym = mrb_intern_lit(mrb, "$0");
	mrbc_filename(mrb , ctxt, args.cmdline);
	mrb_gv_set(mrb, zero_sym, mrb_str_new_cstr(mrb,args.cmdline));
	value = mrb_load_file_cxt(mrb, args.fp, ctxt);
	mrbc_context_free(mrb,ctxt);
	if(mrb->exc)
	{
		if(!mrb_undef_p(value))
		{
			rrhlib_print_error(mrb);
		}
		rtn = EXIT_FAILURE;
	}
	/* ライブラリ開放処理 */
	rrhlib_clean(mrb);
	rrh_rscclear(mrb, &args);
	return rtn;
}
/* @} */
