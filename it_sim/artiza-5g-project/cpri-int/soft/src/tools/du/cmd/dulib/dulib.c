/*!
 *  @skip	$Id:$
 *  @file	dulib.c
 *  @brief	du command library routine
 *  @author	FJT)Taniguchi
 *  @date	2013/11/18 FJT)Taniguchi create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup	RRH_TP_CMD_LIB	rrh command library
 * @brief		rrh command library
 * @note		This is rrh command library.

	引数の説明:	 
	 - スクリプトからモジュールを呼び出すと、モジュールに割り付いたC関数が呼び出される. \n
	 - 呼び出されたC関数は以下の形式で引数を受け取る. \n

	<table border=2 width=500 align=center>
	<tr bgcolor="#cccccc">
		<th>スクリプトフォーマット</th><th>C関数で受け取る引数の種類</th><th>C関数内の型</th>
	</tr>
	<tr align=center>
		<td>o</td><td>Object</td><td>mrb_value</td>
	</tr>
	<tr align=center>
		<td>S</td><td>String</td><td>mrb_value</td>
	</tr>
	<tr align=center>
		<td>A</td><td>Array</td><td>mrb_value</td>
	</tr>
	<tr align=center>
		<td>H</td><td>Hash</td><td>mrb_value</td>
	</tr>
	<tr align=center>
		<td>s</td><td>String</td><td>char*,int</td>
	</tr>
	<tr align=center>
		<td>z</td><td>String</td><td>char*</td>
	</tr>
	<tr align=center>
		<td>a</td><td>Array</td><td>mrb_value*,int</td>
	</tr>
	<tr align=center>
		<td>f</td><td>Float</td><td>mrb_float</td>
	</tr>
	<tr align=center>
		<td>i</td><td>Integer</td><td>mrb_int</td>
	</tr>
	<tr align=center>
		<td>n</td><td>Symbol</td><td>mrb_sym</td>
	</tr>
	<tr align=center>
		<td>&</td><td>Block</td><td>mrb_value</td>
	</tr>
	<tr align=center>
		<td>*</td><td>rest argument</td><td>mrb_value*,int</td>
	</tr>
	<tr align=center>
		<td>|</td><td>optional</td><td>-</td>
	</tr>
	</table>
 * @{
 */

#include <signal.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "dulib_local.h"

static unsigned int rrhlib_flg_dbg;
static mrb_state* rrhlib_dcatch_state;
unsigned int rrhlib_assignflg;
unsigned int rrhlib_unassignflg;
int rrhlib_pagesize;

extern unsigned int  rrhlib_install(mrb_state* mrb );

/*!
 * @brief		rrhコマンドデバッグメッセージ出力処理
 * @note
	<pre>
	デバッグコマンド実行時のデバッグメッセージを出力する処理である.
	デバッグフラグがO以外の場合はメッセージ出力を行う.
	デバッグフラグがOの場合はメッセージ出力を行わない.
	</pre>
 * @param		filename	[in]	ファイル名
 * @param		line		[in]	ライン数
 * @param		format		[in]	可変長ポインタ
 * @param		...			[in]	...
 * @return		N/A
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
void f_rrhlib_dmsg(const char* filename , const int line,  const char* format, ...)
{
	if(rrhlib_flg_dbg == 0)
	{
		return;
	}
	char str[256];
	int len;
	va_list args;

	len = snprintf(str,sizeof(str),"DEBUG;%s(%d);", filename,line );
	va_start(args, format);	/* pgr0039 */
	len +=  vsnprintf( &str[len] , sizeof(str) - len, format, args);
	va_end(args);

	fprintf(stdout, str);
	fflush(stdout);
	return;
}

/*!
 * @brief		ライブラリエラー出力処理
 * @note
	<pre>
	スクリプト上でエラー出力処理
	</pre>
 * @param		mrb		[in]	mrb status
 * @return		N/A
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
void rrhlib_print_error(mrb_state* mrb)
{
	mrb_print_error(mrb);
	return;
}

/*!
 * @brief		モジュール開放処理
 * @note
<pre>
	不要になったリソースを開放する処理である.
</pre>
 * @param		mrb	[in]	mrb status
 * @return		N/A
 * @date		2014/01/20 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
void rrhlib_clean(mrb_state* mrb)
{
	int pid = getpid();
	if(dulib_cmdshm_addr != NULL){
		if(pid == dulib_cmdshm_addr->com_info.pid){
			dulib_cmdshm_addr->com_info.pid = -1;
		}
	}
	if ( rrhlib_assignflg > 0 ){
		(void)BPF_RM_SVRM_UNASSIGN(D_RRH_PROCID_DUCMD);
	}
	rrhlib_assignflg = 0;
	rrhlib_unassignflg = 0;
	return;
}

/*!
 * @brief		シグナルキャッチ処理
 * @note
	<pre>
	シグナルをキャッチし、raiseを実行する
	ただし、SIGSEGVのみ、強制終了する
	</pre>
 * @return		N/A
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
void rrhlib_sigcatch(int no_sig)
{
	char str[128];

	switch(no_sig)
	{
		case SIGSEGV:
		case SIGBUS:
		case SIGFPE:
		case SIGHUP:
		case SIGILL:
		case SIGTERM:
		case SIGABRT:
		case SIGTSTP:
		case SIGQUIT:
		case SIGPIPE:
		case SIGINT:
			rrhlib_clean(rrhlib_dcatch_state);
			exit(EXIT_FAILURE);	/* 強制終了*/
			break;
		default:
			memset(str,0,sizeof(str));
			snprintf(str,sizeof(str),"raise \"signo:%d\"",no_sig);
			mrb_load_string(rrhlib_dcatch_state, str);
			break;
	}
	return;
}

/*!
 * @brief		ライブラリ初期化処理
 * @note
	<pre>
	rrhコマンドライブラリの初期化を行う.
	</pre>
 * @param		mrb		[in]	mrb status
 * @return		N/A
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
void rrhlib_init(mrb_state* mrb)
{
	/* 必要に応じて設定 */
	rrhlib_pagesize = getpagesize();
	rrhlib_flg_dbg = 0;
	rrhlib_dcatch_state = mrb;
	rrhlib_assignflg = 0;
	rrhlib_unassignflg = 0;
	signal(SIGSEGV,rrhlib_sigcatch);
	signal(SIGBUS,rrhlib_sigcatch);
	signal(SIGFPE,rrhlib_sigcatch);
	signal(SIGHUP,rrhlib_sigcatch);
	signal(SIGILL,rrhlib_sigcatch);
	signal(SIGINT ,rrhlib_sigcatch);
	signal(SIGTERM,rrhlib_sigcatch);
	signal(SIGABRT,rrhlib_sigcatch);
	signal(SIGTSTP,rrhlib_sigcatch);
	signal(SIGQUIT,rrhlib_sigcatch);
	signal(SIGURG ,rrhlib_sigcatch);
	signal(SIGPIPE,rrhlib_sigcatch);

	INT						rtn_bpf;
	rtn_bpf = BPF_RM_SVRM_ASSIGN(D_RRH_PROCID_DUCMD);
	if(BPF_RM_SVRM_COMPLETE != rtn_bpf)
	{
		rrhlib_dmsg("error2 = %d\n", rtn_bpf );
		return;
	}
	rrhlib_assignflg++;
	rrhlib_dmsg("Init \n");
	return;
}

#ifdef RRHLIB_MEM_CHK
/*!
 * @brief		メモリチェック処理
 * @note
	<pre>
		自プロセス内のメモリかどうかをチェックする関数である
	</pre>
 * @param		chkbufp	[in]	チェック対象メモリアドレス
 * @return		結果
 * @retval		0:プロセス内メモリ(正常時)
 * @retval		-1:プロセス外メモリ(異常時)
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
int f_rrhlib_memchecker( unsigned char* chkbufp )
{
	unsigned char wrkbufp[rrhlib_pagesize];
	unsigned long  chkaddr = (unsigned long)(chkbufp) - ((unsigned long)(chkbufp) % rrhlib_pagesize);		/* page size alignment */
	return mincore((void*)chkaddr,1,wrkbufp);
}
#endif

/*!
 * @brief		usage処理
 * @note
	<pre>
	rrh commandのusageである
	</pre>
 * @param		argv	[in]	コマンドパラメータ
 * @return		N/A
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
void rrhlib_usage(char* argv)
{
	printf("du command usage \n");
	printf(" * du -<option> \n");
	return;
}

/*!
 * @brief		デバッグメッセージ有効無効処理
 * @note
	<pre>
	スクリプト上からデバッグメッセージの出力を変更する関数である.
	</pre>	
 * @tparam		デバッグon/offフラグ	[in]	Integer	
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		現状のデバッグ値
 * @retval		0:無効
 * @retval		1:有効
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.dmsg()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_dmsg_onoff(mrb_state* mrb, mrb_value self)
{
	mrb_int onoff;

	mrb_get_args(mrb, "i", &onoff );
	if( onoff == 2)
	{
		onoff = rrhlib_flg_dbg;
	}
	else
	{
		rrhlib_flg_dbg = onoff;
	}
	return (mrb_fixnum_value(onoff));
}

/*!
 * @brief		fflush処理
 * @note
	<pre>
	スクリプト上からfflushする関数である.
	</pre>	
 * @tparam		デバッグon/offフラグ	[in]	Integer	
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		現状のデバッグ値
 * @retval		0:無効
 * @retval		1:有効
 * @date		2015/11/23 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.fflush()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_fflush(mrb_state* mrb, mrb_value self)
{
	fflush(stdout);
	return mrb_true_value();
}

/*!
 * @brief		デバッグメッセージ取得処理
 * @note
	<pre>
	スクリプト上からデバッグメッセージの出力を行う関数である.
	出力先はplogである
	</pre>	
 * @tparam		デバッグメッセージ	[in]	Integer	 or String
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		現状のデバッグ値
 * @retval		true:失敗
 * @retval		fals:有効
 * @date		2015/01/26 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.logger()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_logger(mrb_state* mrb, mrb_value self)
{
	mrb_value		*argv;
	int				argc;
	unsigned int	count;
	void*			str_p;
	void*			wrk_p;
	int				maxlen;
	int				len;
	e_bpf_com_plog_level	level = E_TRC_LV_DEBUG;

	mrb_get_args(mrb, "*", &argv, &argc);
	if(argc < 1){
		return mrb_false_value();
	}
	maxlen = argc * 128;
	if(( str_p = malloc(maxlen)) == NULL){
		return mrb_false_value();
	}
	memset(str_p,0,sizeof(maxlen));
	len = 0;
	wrk_p = str_p;
	for( count = 0 ; count < argc ; count++ , wrk_p = str_p + len ){
		switch( mrb_type(argv[count])){
			case MRB_TT_FIXNUM:
				len += snprintf(wrk_p,maxlen - len,"%lX ",mrb_fixnum(argv[count]));
				break;
			case MRB_TT_STRING:
				if(count == 0){
					if(!strcmp("LV_DEBUG", RSTRING_PTR(argv[ count ]))){
						level = E_TRC_LV_DEBUG;
						break;
					}
					else if(!strcmp("LV_INFO", RSTRING_PTR(argv[ count ]))){
						level = E_TRC_LV_INFO;
						break;
					}
					else if(!strcmp("LV_SERIOUS", RSTRING_PTR(argv[ count ]))){
						level = E_TRC_LV_SERIOUS;
						break;
					}
					else if(!strcmp("LV_BUG", RSTRING_PTR(argv[ count ]))){
						level = E_TRC_LV_BUG;
						break;
					}
					else{
						;
					}
				}
				len += snprintf(wrk_p,maxlen - len,"%s ",RSTRING_PTR(argv[ count ]) );
				break;
			default:
				break;
		}
	}
	BPF_COM_PLOG_TRACE(level,"%s",str_p);
	free(str_p);
	return mrb_true_value();
}


/*!
 * @brief		デバッグメッセージレベル取得処理
 * @note
	<pre>
	スクリプト上からデバッグメッセージレベルの取得を行う関数である.
	出力先はplogである
	</pre>	
 * @tparam		ログレベルファイル名	[in]	String
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		現状のログレベル値
 * @retval		LV_DEBUG
 * @retval		LV_INFO
 * @retval		LV_SERIOUS
 * @retval		LV_BUG
 * @retval		値
 * @date		2015/01/26 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.loglvlset()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_loglvlget(mrb_state* mrb, mrb_value self)
{
	mrb_value	str;
	int			level;
	char		level_str[16];
	int			len;

	mrb_get_args(mrb, "S", &str );
	level = bpf_com_plog_get_level(RSTRING_PTR(str));
	rrhlib_dmsg("log level get %s -> %d\n",RSTRING_PTR(str),level);
	memset(level_str,0,sizeof(level_str));
	switch(level)
	{
		case E_TRC_LV_DEBUG:
			len = snprintf(level_str,sizeof(level_str),"LV_DEBUG");
			break;
		case E_TRC_LV_INFO:
			len = snprintf(level_str,sizeof(level_str),"LV_INFO");
			break;
		case E_TRC_LV_SERIOUS:
			len = snprintf(level_str,sizeof(level_str),"LV_SERIOUS");
			break;
		case E_TRC_LV_BUG:
			len = snprintf(level_str,sizeof(level_str),"LV_BUG");
			break;
		default:
			len = snprintf(level_str,sizeof(level_str),"%d",level);
			break;
	}
	return (mrb_str_new(mrb, level_str, len));
}

/*!
 * @brief		デバッグメッセージレベル設定処理
 * @note
	<pre>
	スクリプト上からデバッグメッセージレベル設定を行う関数である.
	出力先はplogである
	</pre>	
 * @tparam		ログレベルファイル名	[in]	String
 * @tparam		ログレベル				[in]	String
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		現状のデバッグ値
 * @retval		true:失敗
 * @retval		fals:有効
 * @date		2015/01/26 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.loglvlset()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_loglvlset(mrb_state* mrb, mrb_value self)
{
	mrb_value	str;
	mrb_value	level_str;
	int			level = 0;

	mrb_get_args(mrb, "SS", &str ,&level_str);
	if(!strcmp(RSTRING_PTR(level_str),"LV_DEBUG")){
		level = E_TRC_LV_DEBUG;
	}
	else if(!strcmp(RSTRING_PTR(level_str),"LV_INFO")){
		level = E_TRC_LV_INFO;
	}
	else if(!strcmp(RSTRING_PTR(level_str),"LV_SERIOUS")){
		level = E_TRC_LV_SERIOUS;
	}
	else if(!strcmp(RSTRING_PTR(level_str),"LV_BUG")){
		level = E_TRC_LV_BUG;
	}
	else{
		if( sscanf( RSTRING_PTR(level_str), "%d", &level) == EOF)
		{
			rrhlib_dmsg("log level set err %s\n",level_str);
			return mrb_false_value();
		}
	}
	rrhlib_dmsg("log level set %s <- %d\n",RSTRING_PTR(str),level);
	bpf_com_plog_set_level(RSTRING_PTR(str),level);
	return mrb_true_value();
}


/*!
 * @brief		require処理
 * @note
	<pre>
	スクリプト上からrequireを行うための関数である.
	</pre>
 * @tparam		スクリプトファイルパス名	[in]	String	
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		現状のデバッグ値
 * @retval		0:無効
 * @retval		1:有効
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.require()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_require(mrb_state* mrb, mrb_value self)
{
	mrb_value  str;
	mrbc_context *ctxt;
	FILE   *fp;
	mrb_value value;

	mrb_get_args(mrb, "S", &str );

	rrhlib_dmsg("Require \"%s\"\n",RSTRING_PTR(str));

	if((fp = fopen(RSTRING_PTR(str),"r")) == NULL)
	{
		rrhlib_dmsg("Require file opem error \n");
		return mrb_false_value();
	}

	if((ctxt = mrbc_context_new(mrb)) == NULL)
	{
		rrhlib_dmsg("Require ctx ng \n");
		fclose(fp);
		return mrb_false_value();
	}
	mrbc_filename(mrb , ctxt , RSTRING_PTR(str));
	value = mrb_load_file_cxt(mrb, fp, ctxt);
	rrhlib_dmsg("Require result %X \n",mrb_undef_p(value));
	mrbc_context_free(mrb,ctxt);
	fclose(fp);
	return mrb_true_value();
}
/*!
 * @brief		getline処理
 * @note
	<pre>
	スクリプト上からgetlineを行うための関数である.
	</pre>
 * @tparam		N/A	
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		コンソール入力文字
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.getline()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_getline(mrb_state* mrb, mrb_value self)
{
	char *linebuf = NULL;
	ssize_t num = 0;
	size_t bufsize = 0;
	char str[256];
	
	rrhlib_dmsg("Getline \n");
	memset(str,0,sizeof(str));
	if((num = getline(&linebuf, &bufsize, stdin)) > 1)
	{
		if(num < sizeof(str))
		{
			strncpy(str , linebuf, num - 1);
		}
	}
	rrhlib_dmsg("line %s size %d num %d \n",str,bufsize,num);
	free(linebuf);
	return ( mrb_str_new_cstr(mrb, str) );
}

/*!
 * @brief		sleep処理
 * @note
	<pre>
	スクリプト上からsleepを行うための関数
	</pre>
 * @tparam		秒		[in]	Integer	
 * @tparam		ナノ秒	[in]	Integer	
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		self
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.sleep()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_sleep(mrb_state* mrb, mrb_value self)
{

	mrb_int sec = 0;
	mrb_int nsec = 0;
	struct timespec stime;

	mrb_get_args(mrb, "i|i", &sec , &nsec );

	rrhlib_dmsg("Sleep %u.%09u sec \n",sec,nsec);
	fflush(stdout);
	stime.tv_sec = (int)sec;
	stime.tv_nsec = (int)nsec;
	nanosleep(&stime,NULL);
	return ( self );
}


/*!
 * @brief		system処理
 * @note
	<pre>
	スクリプト上からsystemを行うための関数
	</pre>
 * @tparam		コマンド名	[in]	String
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		結果コード
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.system()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_system(mrb_state* mrb, mrb_value self)
{
	mrb_value  str;
	mrb_get_args(mrb, "S", &str);
	rrhlib_dmsg("System \"%s\" \n",RSTRING_PTR(str));
#ifdef OPT_RRH_CT
	printf("%s\n",RSTRING_PTR(str));
	return mrb_fixnum_value( 0 );
#else
	fflush(stdout);
	return mrb_fixnum_value( system(RSTRING_PTR(str)) );
#endif
}

/*!
 * @brief		malloc処理
 * @note
	<pre>
	スクリプト上からmallocを行うための関数
	</pre>
 * @tparam		バッファ獲得サイズ	[in]	Integer
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		結果コード
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.malloc()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_malloc(mrb_state* mrb, mrb_value self)
{
	mrb_int	 size;
	void*	 buf;
	unsigned long	 bufaddr;

	mrb_get_args(mrb, "i", &size);
	buf = mrb_malloc(mrb , (size_t)size );
	rrhlib_dmsg("Malloc Addr:0x%p Size:%d \n",(void*)buf , size);
	memset(buf,0,size);
	bufaddr = (unsigned long)buf ;
	return mrb_fixnum_value( bufaddr);
}

/*!
 * @brief		free処理
 * @note
	<pre>
	スクリプト上からfreeを行うための関数
	</pre>
 * @tparam		バッファアドレス	[in]	Integer
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		結果コード
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.free()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_free(mrb_state* mrb, mrb_value self)
{
	mrb_int	 buf;
	int rtn;

	mrb_get_args(mrb, "i", &buf);
	rrhlib_dmsg("Free Addr:0x%08x \n",(int)buf);
	if (( rtn = rrhlib_memchecker((unsigned char*)buf)) == 0 )
	{
		mrb_free(mrb,(void*)buf);
	}
	else{
		rrhlib_dmsg("Free Addr error %x \n",rtn);
		return mrb_false_value();
	}
	return mrb_true_value();
}

/*!
 * @brief		memcmp処理
 * @note
	<pre>
	スクリプト上からmemcmpを行うための関数
	</pre>
 * @tparam		比較バッファアドレス	[in]	Integer
 * @tparam		比較バッファアドレス	[in]	Integer
 * @tparam		比較サイズ				[in]	Integer
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		結果コード
 * @date		2014/01/14 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.memcmp()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_memcmp(mrb_state* mrb, mrb_value self)
{
	mrb_int	 src;
	mrb_int	 dst;
	mrb_int	 size;
	int rtn;
	
	mrb_get_args(mrb, "iii", &src,&dst,&size);	

	rrhlib_dmsg("Memcmp:s %08x d %08x size %08x \n",src,dst,size);
	if((src == dst ) || ( size == 0 ))
	{
		rrhlib_dmsg("Memcmp:OK\n");
		return mrb_true_value();
	}
	if(
	 ( rrhlib_memchecker( (unsigned char*)(src + size)) != 0 ) ||
	   ( rrhlib_memchecker((unsigned char*)(dst + size)) != 0 )
	)
	{
		rrhlib_dmsg("Memcmp:add error \n");
		return mrb_false_value();
	}
	if(( rtn = memcmp((const void*)src , (const void*)dst , size) ) != 0)
	{
		rrhlib_dmsg("Memcmp: mismatch %x \n",rtn);
		return mrb_false_value();
	}
	rrhlib_dmsg("Memcmp:OK\n");
	return mrb_true_value();
}

/*!
 * @brief		timer処理
 * @note
	<pre>
	スクリプト上からtimerを行うための関数
	SIGALRMのシグナルハンドラを登録後、alarmでタイマ設定を行う
	</pre>
 * @tparam		タイマ値(sec)	[in]	Integer
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		結果コード
 * @date		2014/01/14 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.timer()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_timer(mrb_state* mrb, mrb_value self)
{
	mrb_int	 sec;
	mrb_get_args(mrb, "i", &sec);	
	if( sec > 0)
	{
		signal(SIGALRM, rrhlib_sigcatch);
	}
	alarm(sec);
	if(sec == 0)
	{
		signal(SIGALRM, SIG_DFL);
	}
	rrhlib_dmsg("Timer:%d sec \n",sec);
	return mrb_true_value();
}

/*!
 * @brief		dcatch処理
 * @note
	<pre>
	スクリプト上からシグナル登録するための関数
	dcatchの名前の由来はdream catcherである
	</pre>
 * @tparam		シグナル名	[in]	String
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		結果コード
 * @date		2014/01/14 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.dcatch()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_dcatch(mrb_state* mrb, mrb_value self)
{
	mrb_value  str;
	int signo;
	mrb_get_args(mrb, "S", &str);

	if( !strcmp( RSTRING_PTR(str) , "SIGALRM" ) )
	{
		signo = SIGALRM;
	}
	else if ( !strcmp( RSTRING_PTR(str) , "SIGCHLD" ) )
	{
		signo = SIGCHLD;
	}
	else
	{
		/* 未サポート */
		return mrb_false_value();
	}
	signal(signo, rrhlib_sigcatch);
	rrhlib_dmsg("Dcatch %x \n",signo);
	return mrb_true_value();
}

/*!
 * @brief		sigqueue処理
 * @note
	<pre>
	スクリプト上からシグナル送信するための関数
	</pre>
 * @tparam		シグナル名	[in]	String
 * @tparam		スレッドID	[in]	Integer
 * @tparam		付加情報	[in]	Integer
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		結果コード
 * @date		2014/03/07 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.sigqueue()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_sigqueue(mrb_state* mrb, mrb_value self)
{
	mrb_value     str;
	mrb_value     pid_s ;
	mrb_int       value = 0;
	int           signo;
	union sigval  send_value ;
	int            pid;

	mrb_get_args(mrb, "SS|i", &str, &pid_s , &value );

	if( !strcmp( RSTRING_PTR(str) , "SIGHUP") )
	{
		signo = SIGHUP;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGINT"))
	{
		signo = SIGINT;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGQUIT"))
	{
		signo = SIGQUIT;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGILL"))
	{
		signo = SIGILL;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGTRAP"))
	{
		signo = SIGTRAP;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGABRT"))
	{
		signo = SIGABRT;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGBUS"))
	{
		signo = SIGBUS;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGFPE"))
	{
		signo = SIGFPE;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGKILL"))
	{
		signo = SIGKILL;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGUSR1"))
	{
		signo = SIGUSR1;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGUSR2"))
	{
		signo = SIGUSR2;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGSEGV"))
	{
		signo = SIGSEGV;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGPIPE"))
	{
		signo = SIGPIPE;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGALRM"))
	{
		signo = SIGALRM;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGTERM"))
	{
		signo = SIGTERM;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGSTKFLT"))
	{
		signo = SIGSTKFLT;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGCHLD"))
	{
		signo = SIGCHLD;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGCONT"))
	{
		signo = SIGCONT;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGSTOP"))
	{
		signo = SIGSTOP;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGTSTP"))
	{
		signo = SIGTSTP;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGTTIN"))
	{
		signo = SIGTTIN;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGTTOU"))
	{
		signo = SIGTTOU;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGURG"))
	{
		signo = SIGURG;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGXCPU"))
	{
		signo = SIGXCPU;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGXFSZ"))
	{
		signo = SIGXFSZ;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGVTALRM"))
	{
		signo = SIGVTALRM;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGPROF"))
	{
		signo = SIGPROF;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGWINCH"))
	{
		signo = SIGWINCH;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGPOLL"))
	{
		signo = SIGPOLL;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGPWR"))
	{
		signo = SIGPWR;
	}
	else if( !strcmp( RSTRING_PTR(str) , "SIGSYS"))
	{
		signo = SIGSYS;
	}
	else
	{
		rrhlib_dmsg("Sigqueue unknown:%s pid:%s value:%x \n",RSTRING_PTR(str),RSTRING_PTR(pid_s),value);
		return mrb_false_value();
	}
	
	INT	rtn_bpf = D_RRH_OK;
	T_RRH_DU_PROCESS_INFO*	tDuProcInfo;
	INT	errcd;
	
	rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(
				E_RRH_SHMID_APL_DU_PROCESS_INFO,
				(VOID **)&tDuProcInfo,
				&errcd );
	if(rtn_bpf != BPF_RU_IPCM_OK){
		rrhlib_dmsg("Process error1:%s pid:%s value:%x \n",RSTRING_PTR(str),RSTRING_PTR(pid_s),value);
		return mrb_false_value();
	}
	if( !strcmp( RSTRING_PTR(pid_s) , "DU_init") ){
		pid = tDuProcInfo->pid[D_RRH_PROCID_INI];
	}
	else if( !strcmp( RSTRING_PTR(pid_s) , "DU_pf") ){
		pid = tDuProcInfo->pid[D_RRH_PROCID_PF];
	}
	else if( !strcmp( RSTRING_PTR(pid_s) , "DU_ptp") ){
		pid = tDuProcInfo->pid[D_RRH_PROCID_PTP];
	}
	else{
		rrhlib_dmsg("Process error2:%s pid:%s value:%x \n",RSTRING_PTR(str),RSTRING_PTR(pid_s),value);
		return mrb_false_value();
	}
	send_value.sival_int = value;
	int rtn = sigqueue( pid, signo ,send_value );
	rrhlib_dmsg("Sigqueue rtn:%d,%d,name:%s pid:%s(%d) value:%x \n",rtn,errno,RSTRING_PTR(str),RSTRING_PTR(pid_s),pid,value);
	return mrb_fixnum_value( rtn );
}


/*********************************************************/
/*                    文字列演算用                       */
/*********************************************************/
/*!
 * @brief		文字列演算処理
 * @note
	<pre>
	文字列表記された値を演算する処理
	2変数,1オペランド限定
	</pre>
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		演算結果(文字列)
 * @date		2013/12/05
 * @warning		本関数はスクリプト上でRrh.calc()として使用できる.
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_string_clac(mrb_state* mrb, mrb_value self)
{
	mrb_value	str1;
	mrb_value	ope;
	mrb_value	str2;
    unsigned int var1,var2,ans;
    char rtn_str[64];
    int size;
    mrb_value rtn;

	mrb_get_args(mrb, "SSS", &str1, &ope, &str2);
    
        /* exchange string to number */
    if( sscanf( RSTRING_PTR(str1), "%08x", &var1) == EOF ){
        rrhlib_dmsg("Invlid value is detected in face : %s\n", RSTRING_PTR(str1));
        return mrb_false_value();
    }
    if( sscanf( RSTRING_PTR(str2), "%08x", &var2) == EOF ){
        rrhlib_dmsg("Invlid value is detected in addr : %s\n", RSTRING_PTR(str2));
        return mrb_false_value();
    }
    rrhlib_dmsg("%08x %s %08x\n", var1, RSTRING_PTR(ope), var2);
    
    switch ( *RSTRING_PTR(ope) ) {
        case '+' :
            ans = var1 + var2;
            break;
        case '-' : 
            ans = var1 - var2;
            break;
        case '*' : 
            ans = var1 * var2;
            break;
        case '/' : 
            ans = var1 / var2;
            break;
        case '%' : 
            ans = var1 % var2;
            break;
        case '>' : 
            ans = var1 >> var2;
            break;
        case '<' : 
            ans = var1 << var2;
            break;
        case '&' : 
            ans = var1 & var2;
            break;
        case '|' : 
            ans = var1 | var2;
            break;
         case '^' : 
            ans = var1 ^ var2;
            break;
        default : 
            return mrb_false_value();
    }
    
    /* exchange number to string */
    size = snprintf(rtn_str, sizeof(rtn_str), "%08x", ans);
    if(size < 0){
        rrhlib_dmsg("Invlid value is detected\n");
        return mrb_false_value();
    }
    
    rtn = mrb_str_new(mrb, rtn_str, size);
    return rtn;
}

/*!
 * @brief		文字列演算処理 for training
 * @note
	<pre>
	トレーニングコマンド用 演算モジュール
	</pre>
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		演算結果(文字列)
 * @date		2013/01/30
 * @warning		本関数はスクリプト上でRrh.calc_training()として使用できる.
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_string_clac_training(mrb_state* mrb, mrb_value self)
{
	mrb_value	it1_re_mrb;
	mrb_value	it1_im_mrb;
	mrb_value	it2_re_mrb;
	mrb_value	it2_im_mrb;
	mrb_float	sum_step1_mrb;
	mrb_int		stepnum;
	int			it1_re;
	int			it1_im;
	int			it2_re;
	int			it2_im;
    double		var1,var2,ans;
	double		temp;
    mrb_value rtn;

	mrb_get_args(mrb, "SSSSi|f", &it1_re_mrb, &it1_im_mrb, &it2_re_mrb, &it2_im_mrb, &stepnum, &sum_step1_mrb);

    /* exchange string to number */
    if( sscanf( RSTRING_PTR(it1_re_mrb), "%08x", &it1_re) == EOF ){
        rrhlib_dmsg("Invlid value is detected in it1_re : %s\n", RSTRING_PTR(it1_re_mrb));
        return mrb_false_value();
    }
    if( sscanf( RSTRING_PTR(it1_im_mrb), "%08x", &it1_im) == EOF ){
        rrhlib_dmsg("Invlid value is detected in it1_im : %s\n", RSTRING_PTR(it1_im_mrb));
        return mrb_false_value();
    }
    if( sscanf( RSTRING_PTR(it2_re_mrb), "%08x", &it2_re) == EOF ){
        rrhlib_dmsg("Invlid value is detected in it2_re : %s\n", RSTRING_PTR(it2_re_mrb));
        return mrb_false_value();
    }
    if( sscanf( RSTRING_PTR(it2_im_mrb), "%08x", &it2_im) == EOF ){
        rrhlib_dmsg("Invlid value is detected in it2_im : %s\n", RSTRING_PTR(it2_im_mrb));
        return mrb_false_value();
    }
	
    rrhlib_dmsg("it1(%d,%d) it2(%d,%d)\n", it1_re, it1_im, it2_re, it2_im);
    
	var1 = (double)it1_re*it1_re +  (double)it1_im*it1_im;
	var2 = (double)it2_re*it2_re +  (double)it2_im*it2_im;
	
	if((int)stepnum == 1){
		if(var2==0){
			rrhlib_dmsg("zero devide in step1\n");
        	return mrb_false_value();
		}
		temp = var1/var2;
		rrhlib_dmsg("A=%f, B=%f, C=%f\n", var1, var2, temp);
		ans = sum_step1_mrb + temp;
	}else if((int)stepnum == 2){
		ans = var1-var2;
		if(ans<0){
			ans = ans*(-1);
		}
		rrhlib_dmsg("A=%f, B=%f, C=%f\n", var1, var2, ans);
	}else{
		rrhlib_dmsg("Invlid value is detected in stepnum : %lu\n", (int)stepnum);
        return mrb_false_value();
	}
    
	rtn = mrb_float_value(mrb, ans);
    return rtn;
}

/*!
 * @brief		文字列数値変換処理
 * @note
	<pre>
	数値の文字列を指定された形式に応じて変換する関数
	</pre>
 * @tparam		変換前の形式	[in]	String
 * @tparam		変換後の形式	[in]	String
 * @tparam		変換前の値		[in]	String
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		変換後の値(String)
 * @date		2013/12/10 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.v_to_v()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_val_to_val(mrb_state* mrb, mrb_value self)
{
	mrb_value  str;
	mrb_value  from;
	mrb_value  to;
	unsigned int val;
	unsigned int len;
	unsigned int count;
	char val_string[33];
	
	mrb_get_args(mrb, "SSS", &from,&to,&str);

	memset(val_string,0,sizeof(val_string));
	
	if ( strcmp("h",RSTRING_PTR(from)) == 0 )
	{
		sscanf(RSTRING_PTR(str),"%x",&val);
	}
	else if( strcmp("d",RSTRING_PTR(from)) == 0 ) 
	{
		sscanf(RSTRING_PTR(str),"%d",&val);
	}
	else if( strcmp("b",RSTRING_PTR(from)) == 0 )
	{
		strncpy(val_string,RSTRING_PTR(str),sizeof(val_string));
		len = strlen(val_string);
		val = 0;
		for(count=0 ; count < len ; count++)
		{
			if(val_string[count] == '1')
			{
				val |= (0x00000001 << (len - count - 1)) ;
			}
		}
		memset(val_string,0,sizeof(val_string));
	}
	else
	{
		return mrb_false_value();
	}
	if( strcmp("h",RSTRING_PTR(to)) == 0 ) 
	{
		len = snprintf(val_string, sizeof(val_string) , "%x", val);
	}
	else if( strcmp("d",RSTRING_PTR(to)) == 0 ) 
	{
		len = snprintf(val_string,sizeof(val_string),"%d", val);
	}
	else if( strcmp("b",RSTRING_PTR(to)) == 0 ) 
	{
		for( count= 0 ,len = 0 ; count <= 31 ; count++ ,len++)
		{
			if(((val >> count) & 0x00000001) != 0)
			{
				val_string[ 31 - count ] = '1';
			}
			else
			{
				val_string[ 31 - count ] = '0';
			}
		}
	}
	else
	{
		return mrb_false_value();
	}
	rrhlib_dmsg("Val to Val %s -> %s  \n",RSTRING_PTR(str),val_string );
	return mrb_str_new(mrb, val_string, len);
}


/* @{ */
/********************************************************************************************************************/
/**
 * @brief 関数機能概要:CRC16計算処理
 * @note  関数処理内容.
 *          -# CRC16計算処理を行う
 * @param  dat          [in]USHORT      CRC16計算用データ
 * @param  p_crc        [out]USHORT*    CRC16計算過程データ
 */
/********************************************************************************************************************/
mrb_value rrhlib_add_CRC(mrb_state* mrb, mrb_value self)
{
	mrb_int	v1;
	mrb_int	v2;
    unsigned short var1,var2;
    unsigned short cnt;
    mrb_value rtn;

	mrb_get_args(mrb, "ii", &v1, &v2);
    
	var1 = (unsigned short)v1;
	var2 = (unsigned short)v2;
	
    var2 ^= var1;
    cnt = 16;
    while(cnt--)
    {
        if ((var2 & 0x8000U)!=0)
        {
            var2 = (var2 << 1) ^ 0x1021;
        }
        else
        {
            var2 <<= 1;
        }
    }
    
    rtn = mrb_fixnum_value(var2);
    return rtn;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 * @brief 関数機能概要:CRC16計算処理
 * @note  関数処理内容.
 *          -# CRC16計算処理を行う
 * @param  dat          [in]USHORT      CRC16計算用データ
 * @param  p_crc        [out]USHORT*    CRC16計算過程データ
 */
/********************************************************************************************************************/
mrb_value rrhlib_little_CRC(mrb_state* mrb, mrb_value self)
{
	mrb_int	v1;
	mrb_int	v2;
    unsigned short var1,var2;
    unsigned short cnt;
    mrb_value rtn;

	mrb_get_args(mrb, "ii", &v1, &v2);
    
	var1 = (unsigned short)v1;
	var2 = (unsigned short)v2;
	
    BPF_COM_END_LITTLETOBIG16(var1);
	
    var2 ^= var1;
    cnt = 16;
    while(cnt--)
    {
        if ((var2 & 0x8000U)!=0)
        {
            var2 = (var2 << 1) ^ 0x1021;
        }
        else
        {
            var2 <<= 1;
        }
    }
	
    rtn = mrb_fixnum_value(var2);
    return rtn;
}
/* @} */

/*!
 * @brief        プロセス管理機能割付け
 * @note
<pre>
     プロセス管理機能割付けを行う.
 	 BPFを用いるのに必要
</pre>
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/17
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.assign
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_assign(mrb_state* mrb, mrb_value self)
{
	int rtn_bpf;
	
	if ( rrhlib_assignflg > 0)
	{
		rrhlib_assignflg++;
		if(rrhlib_assignflg == 0){
			rrhlib_assignflg++;
		}
		rrhlib_dmsg("already %d count assigned \n",rrhlib_assignflg );
		return mrb_true_value();
	}
	rtn_bpf = BPF_RM_SVRM_ASSIGN(D_RRH_PROCID_DUCMD);
	if(BPF_RM_SVRM_COMPLETE != rtn_bpf)
	{
		rrhlib_dmsg("error2 = %d\n", rtn_bpf );
		return mrb_false_value();
	}
	rrhlib_assignflg++;
	rrhlib_dmsg("%d count assigned \n",rrhlib_assignflg );
	return mrb_true_value();
}


/*!
 * @brief        プロセス管理機能開放
 * @note
<pre>
     プロセス管理機能開放を行う.
 	 ASSIGN時には最後に実行すること
</pre>
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/19
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.unassign
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_unassign(mrb_state* mrb, mrb_value self)
{
	rrhlib_unassignflg++;
	rrhlib_dmsg("%d count assigned ,%d count assigned \n",rrhlib_assignflg,rrhlib_unassignflg);
	return mrb_true_value();
}

/*!
 * @brief		オブジェクトtoバッファ設定処理
 * @note
	<pre>
		オブジェクト情報から指定byte分、バッファに設定する処理である.
	</pre>
 * @param		buf_p	[in] バッファポインタ
 * @param		ptr		[in] オブジェクト情報
 * @param		byte	[in] 設定サイズ
 * @return		設定後のバッファポインタ
 * @date		2014/01/25 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
static char* rrhlib_obj_to_buf(mrb_state* mrb , char* buf_p , mrb_value ptr , int byte )
{
	char* data_p = buf_p;
	switch ( mrb_type(ptr) )
	{
		case MRB_TT_STRING :
			rrhlib_dmsg("set MRB_TT_STRING byte %d \n",byte);
			memcpy(data_p , RSTRING_PTR(ptr) ,byte );
			data_p += byte;
			break;
		case MRB_TT_FIXNUM :
			rrhlib_dmsg("set MRB_TT_FIXNUM byte %d \n",byte);
			switch (byte)
			{
				case 1:
					*data_p = (unsigned char)mrb_fixnum(ptr);
					break;
				case 2:
					*(unsigned short*)data_p = (unsigned short)mrb_fixnum(ptr);
					break;
				case 4:
					*(unsigned int*)data_p = (unsigned int)mrb_fixnum(ptr);
					break;
				default:
					memset(data_p,(unsigned char)mrb_fixnum(ptr),byte);
					break;
			}
			data_p += byte;
			break;
		default:
			rrhlib_dmsg("set UNknown type%d byte %d \n", mrb_type(ptr) ,byte);
			break;
	}
	return (data_p);
}

/*!
 * @brief		バッファtoオブジェクト設定処理
 * @note
	<pre>
		バッファから指定byte分をオブジェクトに設定する処理である.
	</pre>
 * @param		ptr		[in] オブジェクト情報
 * @param		buf_p	[in] バッファポインタ
 * @param		byte	[in] 設定サイズ
 * @return		設定後のバッファポインタ
 * @date		2014/01/25 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
static char* rrhlib_buf_to_obj(mrb_state* mrb , mrb_value ptr , char* buf_p ,int byte )
{
	char*		 data_p = buf_p;
	char*		 strbuf;
	int			 len = ( byte * 2 ) + 1;
	int			 size;
	unsigned int count;
	int			 type;

	switch( mrb_type(ptr) )
	{
		case MRB_TT_STRING:
			strbuf = malloc(len);
			if(strbuf == NULL){
				return NULL;
			}
			memset(strbuf,0,len);
			type = 0;
			if(!strncmp(RSTRING_PTR(ptr),"txt",sizeof("txt")))
			{
				type = 1;
			}
			size = 0;
			for(count=0 ; count < byte ; count++ , data_p++ )
			{
				if(type == 1)
				{
					if( (((unsigned char)*data_p) >= 0x21) && (((unsigned char)*data_p) <= 0x7e) )
					{
						size += snprintf(&strbuf[size], len - size ,"%c", (unsigned char)*data_p );
					}
					else
					{
						size += snprintf(&strbuf[size], len - size ,".");
					}
				}
				else
				{
					size += snprintf(&strbuf[size], len - size ,"%02x", (unsigned char)*data_p );
				}
			}
			ptr =  mrb_str_new(mrb, strbuf, size);
			free(strbuf);
			break;
		default:
			switch (byte)
			{
				case 1:
				ptr = mrb_fixnum_value(*(unsigned char*)data_p);
				break;
				case 2:
					ptr = mrb_fixnum_value(*(unsigned short*)data_p);
					break;
				case 4:
					ptr = mrb_fixnum_value(*(unsigned int*)data_p);
					break;
				default:
					rrhlib_dmsg("msgget unsupported len:%d\n",byte);
					byte = 0;
					break;
			}
			data_p += byte;
			break;
	}
	return (data_p);
}

/*!
 * @brief		メッセージ設定処理
 * @note
	<pre>
	スクリプト上でメッセージ情報をmallocしたバッファ設定する処理.
	メッセージ情報はレングス(Integer)、パラメータ(Integer or String)の順で設定しなければならない.
	</pre>
 * @tparam		バッファアドレス			[in]	Integer
 * @tparam		メッセージ情報				[in]	Object
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		設定結果
 * @retval		false		: 設定NG
 * @retval		false以外	: 設定したバッファサイズ
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.msgset()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_msgset(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 buf;
	mrb_value 	 obj;
	int			 len;
	char*		 data_p;
	char*		 newdata_p;
	mrb_int 	 num;
	mrb_value 	 *ptr;
	mrb_int		 count;
	int			 rtn;
	int			 index;


	mrb_get_args(mrb, "io",&buf,&obj);

	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	rrhlib_dmsg("msgset num:%d buf 0x%08x \n",num,buf);
	if((num == 0) || (( num % 2 ) > 0 ))
	{
		rrhlib_dmsg("msgset obj num error %d \n");
		return mrb_false_value();
	}
	if (( rtn = rrhlib_memchecker((unsigned char*)buf)) != 0 )
	{
		rrhlib_dmsg("msgset buf addr error %d \n",rtn);
		return mrb_false_value();
	}
	data_p = (char*)buf;
	for(count=0 ; count < num ; count += 2 )
	{
		index = count;
		if ( mrb_type(ptr[index]) != MRB_TT_FIXNUM )
		{
			rrhlib_dmsg("msgset format error No.%d len is type:%x \n",index,mrb_type(ptr[index]));
			return mrb_false_value();
		}
		len = mrb_fixnum(ptr[index]);
		if ( rrhlib_memchecker((unsigned char*)((unsigned long)data_p + len)) != 0 )
		{
			rrhlib_dmsg("msgset addr over len:%x \n",len);
			return mrb_false_value();
		}
		index++;
		if (( newdata_p = rrhlib_obj_to_buf(mrb,data_p,ptr[index],len)) == data_p)
		{
			rrhlib_dmsg("msgset format error No.%d para is type:%x \n",index,mrb_type(ptr[index]));
			return mrb_false_value();
		}
		data_p = newdata_p;
	}
	rrhlib_dmsg("msgset len:%ld \n",((unsigned long)data_p - buf));
	return mrb_fixnum_value( (unsigned long)data_p - buf );
}

/*!
 * @brief		メッセージ設定処理(バイト固定)
 * @note
	<pre>
	スクリプト上でメッセージ情報をmallocしたバッファに設定する処理.
	バッファにはメッセージ情報と設定サイズ固定で一括設定する.
	</pre>
 * @tparam		バッファアドレス			[in]	Integer
 * @tparam		メッセージ情報				[in]	Object
 * @tparam		設定サイズ					[in]	Integer
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		設定結果
 * @retval		false		: 設定NG
 * @retval		false以外	: 設定したバッファサイズ
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.msgset()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_msgset_b(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 buf = 0;
	mrb_int 	 size = 0;
	mrb_value 	 obj;
	char*		 data_p;
	char*		 newdata_p;
	mrb_int 	 num;
	mrb_value 	 *ptr;
	int			 rtn;
	mrb_int		 index;

	mrb_get_args(mrb, "ioi",&buf,&obj,&size);

	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	rrhlib_dmsg("msgset_b num:%d buf 0x%08x size %d \n",num,buf,size);
	if((num == 0) || (size == 0) || (buf == 0))
	{
		rrhlib_dmsg("msgset_b obj or size num error %d \n");
		return mrb_false_value();
	}
	if (( rtn = rrhlib_memchecker((unsigned char*)buf)) != 0 )
	{
		rrhlib_dmsg("msgse_b buf addr error %d \n",rtn);
		return mrb_false_value();
	}
	data_p = (char*)buf;
	for(index=0 ; index < num ; index++ )
	{
		if ( rrhlib_memchecker((unsigned char*)((unsigned long)data_p + size)) != 0 )
		{
			rrhlib_dmsg("msgse_b addr over len:%x \n",size);
			return mrb_false_value();
		}
		if (( newdata_p = rrhlib_obj_to_buf(mrb,data_p,ptr[index],size)) == data_p)
		{
			rrhlib_dmsg("msgse_b format error No.%d para is type:%x \n",index,mrb_type(ptr[index]));
			return mrb_false_value();
		}
		data_p = newdata_p;
	}
	rrhlib_dmsg("msgse_b len:%ld \n",((unsigned long)data_p - buf));
	return mrb_fixnum_value( (unsigned long)data_p - buf );
}

/*!
 * @brief		メッセージ獲得処理
 * @note
	<pre>
	スクリプト上でmallocしたバッファからメッセージ情報を獲得する処理.
	メッセージ情報はレングス(Integer)、パラメータ(Integer or String)の順で獲得する.
	</pre>
 * @tparam		メッセージ情報				[in]	Object
 * @tparam		バッファアドレス			[in]	Integer
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		メッセージ獲得結果
 * @retval		false		: メッセージ獲得NG
 * @retval		false以外	: メッセージ獲得数
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.msgget()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_msgget(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 buf;
	mrb_value 	 obj;
	int			 len;
	int			 alllen = 0;
	char*		 data_p;
	char*		 newdata_p;
	mrb_int 	 num;
	mrb_value 	 *ptr;
	mrb_int		 count;
	int			 rtn;
	int			 index;

	mrb_get_args(mrb, "oi",&obj,&buf);

	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	rrhlib_dmsg("msgget num:%d buf 0x%08x \n",num,buf);
	if((num == 0) || (( num % 2 ) > 0 ))
	{
		return mrb_false_value();
	}
	if (( rtn = rrhlib_memchecker((unsigned char*)buf)) != 0 )
	{
		rrhlib_dmsg("msgget buf addr error %d \n",rtn);
		return mrb_false_value();
	}
	data_p = (char*)buf;
	for(count=0 ; count < num ; count += 2 )
	{
		index = count;
		if ( mrb_type(ptr[index]) != MRB_TT_FIXNUM )
		{
			rrhlib_dmsg("msgget format error No.%d len is type:%x \n",index,mrb_type(ptr[index]));
			break;
		}
		len = mrb_fixnum(ptr[index]);
		if ( rrhlib_memchecker((unsigned char*)((unsigned long)data_p + len)) != 0 )
		{
			rrhlib_dmsg("msgget addr over len:%x \n",len);
			break;
		}
		index++;
		if (( newdata_p = rrhlib_buf_to_obj(mrb,ptr[index],data_p,len)) == data_p)
		{
			rrhlib_dmsg("msgget addr over len:%x \n",len);
			break;
		}
		data_p = newdata_p;
		alllen++;
	}
	rrhlib_dmsg("msgget count:%d \n",alllen);
	return mrb_fixnum_value( alllen );
}

/*!
 * @brief		メッセージ獲得処理(バイト固定)
 * @note
	<pre>
	スクリプト上でmallocしたバッファからメッセージ情報を獲得する処理.
	バッファからメッセージ情報を獲得サイズ固定で一括獲得する.
	</pre>
 * @tparam		メッセージ情報				[in]	Object
 * @tparam		バッファアドレス			[in]	Integer
 * @tparam		獲得サイズ					[in]	Integer
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		メッセージ獲得結果
 * @retval		false		: メッセージ獲得NG
 * @retval		false以外	: メッセージ獲得数
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.msgget()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_msgget_b(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 buf;
	mrb_value 	 obj;
	int			 alllen = 0;
	char*		 data_p;
	char*		 newdata_p;
	mrb_int 	 num;
	mrb_value 	 *ptr;
	int			 rtn;
	mrb_int		 index;
	int			 size;

	mrb_get_args(mrb, "oii",&obj,&buf,&size);

	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	rrhlib_dmsg("msgget_b num:%d buf 0x%08x \n",num,buf);
	if((num == 0) || ( size == 0 ) || (buf == 0) )
	{
		rrhlib_dmsg("msgget_b num or size or buf error \n");
		return mrb_false_value();
	}
	if (( rtn = rrhlib_memchecker((unsigned char*)buf)) != 0 )
	{
		rrhlib_dmsg("msgget_b buf addr error %d \n",rtn);
		return mrb_false_value();
	}
	data_p = (char*)buf;
	for(index=0 ; index < num ; index++)
	{
		if ( rrhlib_memchecker((unsigned char*)((unsigned long)data_p + size)) != 0 )
		{
			rrhlib_dmsg("msgget_b addr over len:%x \n",size);
			break;
		}
		if (( newdata_p = rrhlib_buf_to_obj(mrb,ptr[index],data_p,size)) == data_p)
		{
			rrhlib_dmsg("msgget_b addr over len:%x \n",size);
			break;
		}
		data_p = newdata_p;
		alllen++;
	}
	rrhlib_dmsg("msgget_b count:%d \n",alllen);
	return mrb_fixnum_value( alllen );
}

/*!
 * @brief		メッセージ送信処理
 * @note
	<pre>
	スクリプト上でプロセス間メッセージ送信処理.
	</pre>
 * @tparam		プロセス間ヘッダ情報[in]	Object
 * @tparam		バッファアドレス	[in]	Integer
 * @tparam		バッファ種別		[in]	Integer
 * @tparam		バッファレングス	[in]	Integer
 * @param		mrb					[in]	mrb status
 * @param		self				[in]	mrb self
 * @return		送信結果
 * @retval		false	: 送信NG
 * @retval		true	: 送信OK
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.msgsend()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_msgsend(mrb_state* mrb, mrb_value self)
{
	mrb_value 	 hobj;
	mrb_int 	 qid = 0;
	mrb_int 	 buf = 0;
	mrb_int 	 bkind = 0;
	mrb_int 	 buflen = 0;
	mrb_int 	 num = 0;
	mrb_value 	 *ptr;
	char*		 data_p;
	mrb_int		 count;
	T_RRH_HEAD	 *msg = NULL;
	T_RRH_HEAD	 head;
	int			 errcd;
	unsigned int endcd;
	unsigned int* head_p;

	mrb_get_args(mrb, "ioi|ii",&qid,&hobj,&bkind,&buf,&buflen);
	rrhlib_dmsg("msgsend qid:0x%08X buf:0x%08x bkind:0x%08x len:%d \n",qid,buf,bkind,buflen);
	if(bkind >= E_BPF_RU_IPCM_BUFF_KIND_MAX)
	{
		return mrb_false_value();
	}
	if ((buflen > 0) && (( errcd = rrhlib_memchecker((unsigned char*)buf)) != 0 ))
	{
		rrhlib_dmsg("msgsend buf addr error %d \n",errcd);
		return mrb_false_value();
	}
	
	ptr = RARRAY_PTR(hobj);
	num = RARRAY_LEN(hobj);
	head_p = (unsigned int*)&head;
	for(count=0 ; ( count < num ) && ( count < (sizeof(head)/4) ) ; count++ ,head_p++)
	{
		switch( mrb_type(ptr[count]))
		{
			case MRB_TT_FIXNUM:
				*head_p = (unsigned int)mrb_fixnum(ptr[count]);
				break;
			case MRB_TT_STRING:
				if( sscanf( RSTRING_PTR(ptr[count]), "%08x", head_p) == EOF ){
					return mrb_false_value();
				}
				break;
			default:
				rrhlib_dmsg("msgsend header type error No:%d \n",count);
				return mrb_false_value();
		}
	}

	if((endcd = BPF_RU_IPCM_PROCMSG_ADDRGET(bkind,buflen+sizeof(*msg),(void**)&msg,&errcd )) != 0 )
	{
		rrhlib_dmsg("msgsend buf get error %d %d \n",endcd,errcd);
		return mrb_false_value();
	}
	msg->uiEventNo 	   = head.uiEventNo;
	msg->uiSignalkind  = head.uiSignalkind ;
	msg->uiDstPQueueID = head.uiDstPQueueID;
	msg->uiDstTQueueID = head.uiDstTQueueID;
	msg->uiSrcPQueueID = head.uiSrcPQueueID;
	msg->uiSrcTQueueID = head.uiSrcTQueueID;
	data_p = (char*)msg;
	data_p += sizeof(*msg);
	if (buflen > 0)
	{
		memcpy(data_p,(char*)(buf),buflen);
	}
	if((endcd = BPF_RU_IPCM_MSGQ_SEND(qid,(void*)msg)) != 0 )
	{
		rrhlib_dmsg("msgsend NG %x %d \n",msg,endcd);
		(VOID)BPF_RU_IPCM_PROCMSG_ADDRFREE( (void*)msg );
		return mrb_false_value();
	}
	rrhlib_dmsg("msgsend OK evt %08x sig %08x sq %08x st %08x dq %08x dt %08x \n",
			head.uiEventNo,
			head.uiSignalkind ,
			head.uiDstPQueueID,
			head.uiDstTQueueID,
			head.uiSrcPQueueID,
			head.uiSrcTQueueID);

	return mrb_true_value();
}

/*!
 * @brief		メッセージ受信処理
 * @note
	<pre>
	スクリプト上でプロセス間メッセージ受信処理.
	</pre>
 * @tparam		プロセス間ヘッダ情報[in]	Object
 * @tparam		バッファアドレス	[in]	Integer
 * @tparam		バッファ種別		[in]	Integer
 * @tparam		バッファレングス	[in]	Integer
 * @param		mrb					[in]	mrb status
 * @param		self				[in]	mrb self
 * @return		受信結果
 * @retval		nil		: 受信NG
 * @retval		nil以外 : ヘッダレングスを除いたメッセージレングス
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.msgsend()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_msgrecv(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 qid;
	mrb_value 	 hobj;
	mrb_int 	 buf;
	mrb_int 	 buflen;
	mrb_int 	 wtime;
	mrb_int 	 num;
	mrb_value 	 *ptr;
	char*		 data_p;
	mrb_int		 count;
	T_RRH_HEAD	 *msg = NULL;
	T_RRH_HEAD	 head;
	int			 errcd;
	unsigned int* head_p;
	unsigned int  mlen = 0;

	mrb_get_args(mrb, "iio|ii",&qid,&wtime,&hobj,&buf,&buflen);
	rrhlib_dmsg("msgrecv qid:0x%08X wtime:%d buf:0x%08x len:%d \n",qid,wtime,buf,buflen);
	if ((buflen > 0) && (( errcd = rrhlib_memchecker((unsigned char*)buf)) != 0 ))
	{
		rrhlib_dmsg("msgrecv buf addr error %d \n",errcd);
		return mrb_false_value();
	}
	if ( ( errcd = BPF_RU_IPCM_MSGQ_RECV(qid,wtime,(void**)&msg,&mlen) ) != 0 )
	{
		rrhlib_dmsg("msgrecv error %d \n",errcd);
		if ( msg )
		{
			(VOID)BPF_RU_IPCM_PROCMSG_ADDRFREE( (void*)msg );
		}
		return mrb_false_value();
	}
	if ( mlen < sizeof(*msg) )
	{
		rrhlib_dmsg("msgrecv msg len error %d \n",mlen);
		if ( msg )
		{
			(VOID)BPF_RU_IPCM_PROCMSG_ADDRFREE( (void*)msg );
		}
		return mrb_false_value();
	}
	ptr = RARRAY_PTR(hobj);
	num = RARRAY_LEN(hobj);
	head_p = (unsigned int*)msg;
	for(count=0 ; ( count < num ) && ( count < (sizeof(head)/4) ) ; count++ ,head_p++)		/* pgr0689 */
	{
		ptr[count] = mrb_fixnum_value( *head_p );		/* pgr0689 */
	}
	data_p = (char*)msg;
	data_p += sizeof(*msg);	/* pgr0689 */
	mlen -= sizeof(*msg);
	if((mlen <= 0) || ( buflen < 0))
	{
		;	/* do nothing */
	}
	else if ( buflen <= mlen)
	{
		memcpy((char*)(buf),data_p,buflen);
		mlen = buflen;
	}
	else
	{
		memcpy((char*)(buf),data_p,mlen);
	}
	rrhlib_dmsg("msgrecv OK evt %08x sig %08x sq %08x st %08x dq %08x dt %08x mlen %d \n",			/* pgr0689 */
			msg->uiEventNo,
			msg->uiSignalkind ,
			msg->uiDstPQueueID,
			msg->uiDstTQueueID,
			msg->uiSrcPQueueID,
			msg->uiSrcTQueueID,
			mlen
			);

	(VOID)BPF_RU_IPCM_PROCMSG_ADDRFREE( (void*)msg );
	return mrb_fixnum_value(mlen);
}

/*!
 * @brief        プロセス起動状態取得
 * @note
<pre>
     プロセス起動状態を取得する.
</pre>
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/17
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.psstat
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_psstat(mrb_state* mrb, mrb_value self)
{
	mrb_value	name;
	UINT		procId;

	mrb_get_args(mrb, "S", &name);
	if( !strcmp( RSTRING_PTR(name) , "D_RRH_PROCID_INI" ) ){
		procId = D_RRH_PROCID_INI;
	}
	else if ( !strcmp( RSTRING_PTR(name) , "D_RRH_PROCID_PF" ) ){
		procId = D_RRH_PROCID_PF;
	}
	else if ( !strcmp( RSTRING_PTR(name) , "D_RRH_PROCID_DBG" ) ){
		procId = D_RRH_PROCID_DBG;
	}
	else if ( !strcmp( RSTRING_PTR(name) , "D_RRH_PROCID_PTP" ) ){
		procId = D_RRH_PROCID_PTP;
	}
	else{
		/* 未サポート */
		rrhlib_dmsg("Unknown process \n");
		return mrb_false_value();
	}

	T_RRH_DU_PROCESS_INFO*	tDuProcInfo;
	INT						rtn_bpf;
	INT						errcd;

	rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(
			E_RRH_SHMID_APL_DU_PROCESS_INFO,
			(VOID **)&tDuProcInfo,
			&errcd );

	if( rtn_bpf != BPF_RU_IPCM_OK ){
		rrhlib_dmsg("%s:%d NOT started \n",RSTRING_PTR(name),procId );
		return mrb_false_value();
	}

	if( tDuProcInfo->pState[procId] == D_RRH_ON){
		rrhlib_dmsg("%s:%d NOT started \n",RSTRING_PTR(name),procId );
		return mrb_false_value();
	}
	rrhlib_dmsg("%s:%d started \n",RSTRING_PTR(name),procId );
	return mrb_true_value();
}

/*!
 * @brief		exit処理
 * @note
	<pre>
	スクリプト上からexitを行うための関数
	</pre>
 * @tparam		コマンド名	[in]	String
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		結果コード
 * @date		2018/08/27 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.exit()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_exit(mrb_state* mrb, mrb_value self)
{
	mrb_int 	result;

	mrb_get_args(mrb, "i",&result);
	rrhlib_clean(mrb);
	exit(result);
	return mrb_true_value();
}


/*!
 * @brief		TCP用ソケットオープン処理
 * @note
<pre>
	TCP通信用ソケットをオープンする処理である.
</pre>
 * @tparam		N/A
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		TCPソケット番号
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_tsocket(mrb_state* mrb, mrb_value self)
{
	int sd;
	mrb_value  msd;
	int onoff = 1;

	errno = 0;
	sd = socket(AF_INET, SOCK_STREAM, 0);
	rrhlib_dmsg("Socket Socket:%u result:%s \n",sd,strerror(errno));
	errno = 0;
	setsockopt( sd, IPPROTO_TCP, TCP_NODELAY, (char *)&onoff, sizeof(onoff) );
	rrhlib_dmsg("Socket Opt result:%s \n",strerror(errno));
	msd = mrb_fixnum_value( sd );
	return msd;
}

/*!
 * @brief		TCP用コネクト処理
 * @note
<pre>
	TCPサーバ(IPアドレス、ポート番号)に対し、コネクション要求(SYN)を行う処理である.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @tparam		IPアドレス				[in]	String
 * @tparam		IPポート番号			[in]	Integer
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		エラー番号
 * @retval		0		:正常
 * @retval		0以外	:異常
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.tconnect()として使用できる.
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_tconnect(mrb_state* mrb, mrb_value self)
{
	mrb_value  network;
	mrb_int    port;
	mrb_int    sd;
	struct sockaddr_in ssaddr;

	mrb_get_args(mrb, "iSi", &sd,&network,&port);
	ssaddr.sin_family = AF_INET;
	ssaddr.sin_port = htons(port);
	ssaddr.sin_addr.s_addr = inet_addr(RSTRING_PTR(network));
	errno = 0;
	connect(sd,(struct sockaddr *)&ssaddr, sizeof(ssaddr));
	rrhlib_dmsg("Connect Socket:%u Host:\"%s\" Port:%u result:%s \n", sd,RSTRING_PTR(network),port,strerror(errno));
	return mrb_fixnum_value( errno);
}

/*!
 * @brief		メッセージ構造体
 */
typedef struct
{
	unsigned int data[4*1024];
}t_msg2;


/*!
 * @brief		TCP用データ送信処理
 * @note
<pre>
	TCPデータ送信を行う処理である.
	TCP送信データはint型(値)かstring型(文字列)を許容する.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @tparam		送信データ情報			[in]	Object
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		送信回数
 * @retval		0		:送信失敗
 * @retval		0以外	:送信成功
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.tsend()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_tsend2(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 sd;
	mrb_int 	 num;
	mrb_value 	 obj;
	mrb_value 	 *ptr;
	mrb_int		 count;
	t_msg2 		 msg;
	int  		 mlen;

	mrb_get_args(mrb, "io",&sd,&obj);

	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	rrhlib_dmsg("Send Socket:%u Num:%u \n",sd,num);
	if(num <= 0)
	{
		return mrb_fixnum_value(0);
	}

	for(count=0; count< num ; count++){
		switch(mrb_type(ptr[count])){
			case MRB_TT_FIXNUM:
				msg.data[count]  = htonl(mrb_fixnum(ptr[count]));
				break;
			default:
				break;
		}
	}
	errno = 0;
	if((mlen = sendto(sd, (const void*)&msg, num * 4, 0, NULL, 0)) <= 0){
		rrhlib_dmsg("Send Rlen:%d result:%s \n",mlen,strerror(errno));
	}else{
		rrhlib_dmsg("Send %d Len:%d SLen:%d \n",num,num * 4,mlen);
	}
	return mrb_fixnum_value(count);
}

/*!
 * @brief		TCP用データ受信処理
 * @note
<pre>
	TCPデータ受信を行う処理である.
	TCP受信データはint型(値)かstring型(文字列)を許容する.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @tparam		受信データ情報			[in]	Object
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		受信回数
 * @retval		0		:受信失敗
 * @retval		0以外	:受信成功
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.trecv()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_trecv2(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 sd;
	int			 rlen;
	mrb_value 	 obj;
	mrb_value 	 *ptr;
	mrb_int 	 num;
	mrb_int      count;
	t_msg2 		 msg;

	mrb_get_args(mrb, "io", &sd,&obj);
	
	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	if(num <= 0)
	{
		return mrb_fixnum_value(0);
	}
	rrhlib_dmsg("Recv Socket:%u \n",sd);
	errno = 0;
	if((rlen = recv(sd, &msg, sizeof(msg), 0)) <= 0){
		rrhlib_dmsg("Recv Rlen:%d result:%s \n",rlen,strerror(errno));
		return mrb_fixnum_value(0);
	}
	rrhlib_dmsg("Recv RLen:%d \n",rlen);
	for(count = 0 ; count < num ; count++){
		ptr[count] = mrb_fixnum_value(ntohl(msg.data[count]));
		rlen -= 4;
		if(rlen <= 0){
			break;
		}
	}
	rrhlib_dmsg("Recv End:%u\n",count);
	return mrb_fixnum_value(count);
}

/*!
 * @brief		ソケットクローズ処理
 * @note
<pre>
	オープンしたソケットをクローズする処理である.
	全てのプロトコルソケットをクローズできる.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		self
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.tclose(),Rrh.uclose()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_close(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 sd;

	mrb_get_args(mrb, "i", &sd);
	rrhlib_dmsg("Close Socket:%u \n",sd);
	close(sd);
	return self;
}


/*!
 * @brief		モジュール登録処理
 * @note
<pre>
	スクリプト上で使用するクラス、モジュール、メソッドを登録する関数である.
</pre>
 * @param		mrb	[in]	mrb status
 * @return		N/A
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
unsigned int rrhlib_install(mrb_state* mrb )
{
	struct RClass * module_class;
	unsigned int result;

	rrhlib_init(mrb);
	/* scriptで使用できるモジュールを登録する */
	module_class = mrb_define_module(mrb, "Rrh");
	mrb_define_module_function(mrb, module_class, "dmsg"  ,    rrhlib_dmsg_onoff  , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "logger",    rrhlib_logger      , MRB_ARGS_ANY());
	mrb_define_module_function(mrb, module_class, "loglvlget", rrhlib_loglvlget   , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "loglvlset", rrhlib_loglvlset   , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "fflush"  ,  rrhlib_fflush      , MRB_ARGS_NONE());
	mrb_define_module_function(mrb, module_class, "require",   rrhlib_require     , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "getline",   rrhlib_getline     , MRB_ARGS_NONE());
	mrb_define_module_function(mrb, module_class, "sleep" ,    rrhlib_sleep       , MRB_ARGS_ARG(1,1));
	mrb_define_module_function(mrb, module_class, "system",    rrhlib_system      , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "malloc",    rrhlib_malloc      , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "free"  ,    rrhlib_free        , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "calc"  ,    rrhlib_string_clac , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "calc_training",rrhlib_string_clac_training , MRB_ARGS_REQ(5) | MRB_ARGS_OPT(1));
	mrb_define_module_function(mrb, module_class, "v_to_v" ,   rrhlib_val_to_val  , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "timer"  ,   rrhlib_timer       , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "dcatch" ,   rrhlib_dcatch      , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "addcrc" ,   rrhlib_add_CRC     , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "littlecrc" , rrhlib_little_CRC  , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "assign"  ,  rrhlib_assign      , MRB_ARGS_NONE());
	mrb_define_module_function(mrb, module_class, "unassign",  rrhlib_unassign    , MRB_ARGS_NONE());
	mrb_define_module_function(mrb, module_class, "msgset"  ,  rrhlib_msgset      , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "msgset_b"  ,rrhlib_msgset_b    , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "msgget"  ,  rrhlib_msgget      , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "msgget_b" , rrhlib_msgget_b    , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "msgsend"  , rrhlib_msgsend     , MRB_ARGS_ARG(3,2));
	mrb_define_module_function(mrb, module_class, "msgrecv"  , rrhlib_msgrecv     , MRB_ARGS_ARG(3,2));
	mrb_define_module_function(mrb, module_class, "memcmp"  ,  rrhlib_memcmp      , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "sigqueue" , rrhlib_sigqueue    , MRB_ARGS_ARG(2,1));
	mrb_define_module_function(mrb, module_class, "psstat" ,   rrhlib_psstat      , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "exit"  ,    rrhlib_exit        , MRB_ARGS_REQ(1));

	mrb_define_module_function(mrb, module_class, "tsocket",   rrhlib_tsocket     , MRB_ARGS_NONE());
	mrb_define_module_function(mrb, module_class, "tconnect",  rrhlib_tconnect    , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "tsend2",    rrhlib_tsend2      , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "trecv2",    rrhlib_trecv2      , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "tclose",    rrhlib_close       , MRB_ARGS_REQ(1));


	if(( result = rrhlib_tbl_access_install(mrb,module_class)) == D_RRH_OK){
		rrhlib_device_access_install(mrb,module_class);
		rrhlib_api_install(mrb,module_class);
	}
	return result;
}

/* @} */
