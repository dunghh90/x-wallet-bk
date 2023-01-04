/*!
 *  @skip	$Id:$
 *  @file	rrunit.c
 *  @brief	rrunit main routine
 *  @author	FJT)Taniguchi
 *  @date	2014/07/03 FJT)Taniguchi create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014
 */

/*!
 * @addtogroup	RRUNIT	rrunit command 
 * @brief		rrunit command 
 * @note		This is rru command function.
 * @{
 */

#include "rrunitlib.h"

/*!
 * @brief		rruコマンド情報構造体
 *
 */
typedef struct
{
	FILE   *fp;			/*!< スクリプトファイルポインタ */
	char*  cmdline;		/*!< コマンド名 */
	int    argc;		/*!< スクリプトパラメータ数 */
	char** argv;		/*!< スクリプト引きパラメータ  */
} rru_t_args;


/*!
 * @brief		rrunitコマンドパラメータ解析処理
 * @note
	<pre>
	コマンドパラメータを取得する処理である.
	コマンドは"rrunit xxxx yyyy zzzz ....."という形式で入力される.
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
static int rru_getargs(mrb_state *mrb, int argc, char **argv, rru_t_args *args)
{
	static const rru_t_args args_zero = { 0 };
	*args = args_zero;

	argc--;
	argv++;

	if(strlen(argv[0]) <= 1)
	{
		return EXIT_FAILURE;
	}
	args->fp = fopen(&argv[0][0],"r");
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
 * @brief	rrunitコマンド内部リソース解放処理
 * @note
	<pre>
	rrunitコマンド実行時に使用したリソースを解放する処理である
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
static void rru_rscclear(mrb_state *mrb, rru_t_args *args)
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
 * @brief	rrunitコマンドメイン処理
 * @note
	<pre>
	rruコマンドのメイン処理である
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
	rru_t_args args;
	mrb_value value;
	int rtn = EXIT_FAILURE;
	mrbc_context *ctxt;
	mrb_sym zero_sym;
	int count;

	if(argc <= 1)
	{
		rrulib_usage(argv[0]);
		return EXIT_FAILURE;
	}
	if((mrb = mrb_open()) == NULL)
	{
		return rtn;
	}
	if((rtn = rru_getargs(mrb, argc, argv, &args)) == EXIT_FAILURE)
	{
		rru_rscclear(mrb, &args);
		return rtn;
	}	

	/* 関数登録 */
	rrulib_install(mrb);
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
			mrb_print_error(mrb);
		}
		rtn = EXIT_FAILURE;
	}
	rrulib_uninstall();
	rru_rscclear(mrb, &args);
	return rtn;
}
/* @} */
