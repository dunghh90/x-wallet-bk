/*!
 * @skip  $ld:$
 * @file  f_cmn_com_assert_th.c
 * @brief スレッド内assert処理
 * @date  2014/03/06 FJT)Taniguchi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h> 
#include <sys/mman.h>
#include <pthread.h>
#include <errno.h>
#include "f_rrh_def.h"
#include "f_rrh_type.h"
#include "f_cmn_com_typ.h"
#include "f_cmn_com_ext.h"

#define D_CMN_COM_LOG_MAX_ASSERTNUM_TH		240
#define D_CMN_COM_LOG_MAX_ASSERTLOGNUM_TH	255
#define D_CMN_COM_LOG_ASSERT_THDTBLNUM		64			/* max 64 threadまで登録可能 */
#define D_CMN_COM_LOG_ASSERT_TBLNAME		32

#define D_CMN_COM_PAGESIZE					4096

typedef	struct{
	struct timespec 				t_count;
	unsigned int					level;
	unsigned int					flg;
	char							str[D_CMN_COM_LOG_MAX_ASSERTNUM_TH];
}T_CMN_COM_LOG_ASSERT_TH_INFO;

typedef	struct{
	unsigned char					name[D_CMN_COM_LOG_ASSERT_TBLNAME];	/* 32 */
	unsigned int					index;
	unsigned int					level;
	unsigned int					level_old;
	int								tid;
	pthread_mutex_t					mutex;	/* maybe 24byte*/
	char							reserve[184];
	T_CMN_COM_LOG_ASSERT_TH_INFO	info[D_CMN_COM_LOG_MAX_ASSERTLOGNUM_TH];	/* 255*256 */
}T_CMN_COM_LOG_ASSERT_TH;	/* 256*256 byte=64K */


typedef	struct{
	unsigned int					thlog_counter;
	int								create_pid;	
	pthread_mutex_t					mutex;	/* maybe 24byte*/
	T_CMN_COM_LOG_ASSERT_TH*		thlog_p[ D_CMN_COM_LOG_ASSERT_THDTBLNUM + 1 ];	/* 0 is no use */
}T_CMN_COM_LOG_ASSERT_MNG;	/* 32byte */

static T_CMN_COM_LOG_ASSERT_MNG* cmn_com_assert_mng_p = NULL;

/*!
 * @brief		メモリチェック処理
 * @note		プロセス領域内のメモリなのかをチェックする
 * @param		*chkbufp	[in]	チェック対象アドレス
 * @return		int
 * @retval		0	:OK
 * @retval		0以外:NG(プロセス領域外)
 * @warning		本関数はinlineで展開される
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2014/03/06 FJT)Taniguchi create
 */
static inline int f_cmn_com_memchecker( unsigned char* chkbufp )
{
	unsigned char wrkbufp[D_CMN_COM_PAGESIZE];
	unsigned long        chkaddr = (unsigned long)(chkbufp) - ((unsigned long)(chkbufp) % D_CMN_COM_PAGESIZE);		/* page size alignment */
	return mincore((void*)chkaddr,1,wrkbufp);
}

/*!
 * @brief		プロセス内アサートログ情報作成処理
 * @note		プロセス内のアサートログ情報を作成する. 64スレッド分まで作成可能である
 * @param		logLevel	[in]	ログレベル
 * @param		name_p		[in]	ログ情報名
 * @return		void*
 * @retval		NULL以外	:プロセス内アサートログ情報アドレス
 * @retval		NULL		:作成失敗
 * @warning		本関数はスレッド毎で1回だけコールすること
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2014/03/06 FJT)Taniguchi create
 */
void* f_cmn_com_assert_th_create( unsigned int logLevel , char* name_p )
{
	T_CMN_COM_LOG_ASSERT_TH* wrk_p = NULL;
	pthread_mutexattr_t mattr;
	int	rtn;

	if( cmn_com_assert_mng_p == NULL)
	{
		cmn_com_assert_mng_p = (T_CMN_COM_LOG_ASSERT_MNG*)malloc( sizeof(T_CMN_COM_LOG_ASSERT_MNG) );
		memset(cmn_com_assert_mng_p,0,sizeof(*cmn_com_assert_mng_p));	
		pthread_mutexattr_init(&mattr);
		pthread_mutexattr_setpshared( &mattr , PTHREAD_PROCESS_PRIVATE );
		pthread_mutex_init( &(cmn_com_assert_mng_p->mutex), &mattr );
		pthread_mutexattr_destroy(&mattr);
		cmn_com_assert_mng_p->create_pid = getpid();
		
	}
	if (( rtn = f_cmn_com_memchecker((unsigned char*)cmn_com_assert_mng_p)) != 0 )
	{
		return NULL;
	}
	if( pthread_mutex_lock(&(cmn_com_assert_mng_p->mutex)) != 0)
	{
		return NULL;
	}
	if(cmn_com_assert_mng_p->thlog_counter >= D_CMN_COM_LOG_ASSERT_THDTBLNUM )
	{
		pthread_mutex_unlock(&(cmn_com_assert_mng_p->mutex));
		return NULL;
	}
	/* buffer get */
	if(( wrk_p = (T_CMN_COM_LOG_ASSERT_TH*)malloc(sizeof(T_CMN_COM_LOG_ASSERT_TH ))) == NULL )
	{
		pthread_mutex_unlock(&(cmn_com_assert_mng_p->mutex));
		return NULL;
	}
	memset(wrk_p,0,sizeof(T_CMN_COM_LOG_ASSERT_TH));
	wrk_p->level = logLevel;	/* same as ASSERT log level */
	wrk_p->level_old = logLevel;
	wrk_p->tid = (int)syscall(SYS_gettid);
	if( ( name_p != NULL ) && ( strlen(name_p) > 0 ) )
	{
		snprintf((char*) wrk_p->name, sizeof(wrk_p->name),"%s",name_p);
	}
	else
	{
		snprintf((char*) wrk_p->name, sizeof(wrk_p->name),"Tid:%d",wrk_p->tid);
	}
	pthread_mutexattr_init(&mattr);
	pthread_mutexattr_setpshared(&mattr,PTHREAD_PROCESS_PRIVATE);	/* プロセス内限定 */
	pthread_mutex_init( &(wrk_p->mutex), &mattr );
	pthread_mutexattr_destroy(&mattr);
	cmn_com_assert_mng_p->thlog_counter++;
	cmn_com_assert_mng_p->thlog_p[cmn_com_assert_mng_p->thlog_counter] = wrk_p;
	pthread_mutex_unlock(&(cmn_com_assert_mng_p->mutex));
	return( (void*)wrk_p);
}

/*!
 * @brief		プロセス内アサートログ取得停止処理
 * @note		プロセス内のアサートログ取得を停止する
 * @param		buf_p		[in]	停止対象プロセス内アサートログ情報アドレス
 * @return		N/A
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2014/03/06 FJT)Taniguchi create
 */
void f_cmn_com_assert_th_stop( void* buf_p )
{
	T_CMN_COM_LOG_ASSERT_TH*	wrk_p = buf_p;
	int rtn;
	if (( rtn = f_cmn_com_memchecker((unsigned char*)wrk_p)) != 0 )
	{
		return;
	}
	if( pthread_mutex_trylock(&(wrk_p->mutex)) != 0)
	{
		return;
	}
	wrk_p->level_old = wrk_p->level;
	wrk_p->level = 0;
	pthread_mutex_unlock(&(wrk_p->mutex));
	return;
}

/*!
 * @brief		プロセス内アサートログ取得再開処理
 * @note		プロセス内のアサートログ取得を再開する
 * @param		logLevel	[in]	再開後のログレベル
 * @param		buf_p		[in]	再開対象プロセス内アサートログ情報アドレス
 * @return		N/A
 * @warning		プロセス内のアサートログ取得は初期化される
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2014/03/06 FJT)Taniguchi create
 */
void f_cmn_com_assert_th_restart(unsigned int logLevel , void* buf_p )
{
	T_CMN_COM_LOG_ASSERT_TH*	wrk_p = buf_p;
	int rtn;

	if (( rtn = f_cmn_com_memchecker((unsigned char*)wrk_p)) != 0 )
	{
		return;
	}
	if( pthread_mutex_trylock(&(wrk_p->mutex)) != 0)
	{
		return;
	}
	memset(wrk_p->info,0,sizeof(wrk_p->info));
	wrk_p->level = logLevel;
	wrk_p->level_old = logLevel;
	wrk_p->index = 0;
	pthread_mutex_unlock(&(wrk_p->mutex));
	return;
}

/*!
 * @brief		プロセス内アサートログ取得開始処理
 * @note		プロセス内のアサートログ取得を開始する
 * @param		buf_p		[in]	再開始対象プロセス内アサートログ情報アドレス
 * @return		N/A
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2014/03/06 FJT)Taniguchi create
 */
void f_cmn_com_assert_th_start( void* buf_p )
{
	T_CMN_COM_LOG_ASSERT_TH*	wrk_p = buf_p;
	int rtn;
	
	if (( rtn = f_cmn_com_memchecker((unsigned char*)wrk_p)) != 0 )
	{
		return;
	}
	if( pthread_mutex_trylock(&(wrk_p->mutex)) != 0)
	{
		return;
	}
	wrk_p->level = wrk_p->level_old;
	pthread_mutex_unlock(&(wrk_p->mutex));
	return;
}

/*!
 * @brief		プロセス内アサートログ情報解放処理
 * @note		プロセス内アサートログ情報をすべて解放する
 * @param		N/A
 * @return		N/A
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2014/03/06 FJT)Taniguchi create
 */
void f_cmn_com_assert_th_destroy( void )
{
	unsigned int	count;
	int		rtn;

	if(cmn_com_assert_mng_p == NULL)
	{
		return;
	}
	if (( rtn = f_cmn_com_memchecker((unsigned char*)cmn_com_assert_mng_p)) != 0 )
	{
		return;
	}
	for(count = 1 ; count < (cmn_com_assert_mng_p->thlog_counter) ; count++)
	{
		if ( cmn_com_assert_mng_p->thlog_p[count] == NULL)
		{
			continue;
		}
		if( pthread_mutex_destroy( &(cmn_com_assert_mng_p->thlog_p[count]->mutex)) != 0 )
		{
			return;
		}
	}
	for(count = 1 ; count <= ( cmn_com_assert_mng_p->thlog_counter ) ; count++)
	{
		if ( cmn_com_assert_mng_p->thlog_p[count] == NULL)
		{
			continue;
		}
		free(cmn_com_assert_mng_p->thlog_p[count]);
	}
	pthread_mutex_destroy( &(cmn_com_assert_mng_p->mutex));
	free( cmn_com_assert_mng_p );
	cmn_com_assert_mng_p = NULL;
	return;
}

/*!
 * @brief		プロセス内アサートログ情報取得処理
 * @note		プロセス内アサートログ情報を取得
 * @param		*buf_p		[in]	プロセス内アサートログ情報アドレス
 * @param		logLevel	[in]	ログレベル
 * @param		funcname	[in]	アサートコール関数名
 * @param		format		[in]	可変長文字列
 * @return		N/A
 * @warning		ユーザは本処理を直接コールしてはいけない。必ずcmn_com_assert_th_getマクロをコールすること
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2014/03/06 FJT)Taniguchi create
 */
void f_cmn_com_assert_th_get(void* buf_p, unsigned int logLevel , const char* funcname ,  const char* format, ...)
{
	T_CMN_COM_LOG_ASSERT_TH*	wrk_p = buf_p;
	va_list args;
	int		len;
	int		rtn;
	int		self;
	int		index;

	if (( rtn = f_cmn_com_memchecker((unsigned char*)wrk_p)) != 0 )
	{
		return;
	}
	if(( wrk_p->level & logLevel) == 0)
	{
		return;
	}
	self = (int)syscall(SYS_gettid);
	if(wrk_p->tid != self )
	{
		return;
	}
	if( pthread_mutex_trylock(&(wrk_p->mutex)) != 0)
	{
		return;
	}
	memset(&(wrk_p->info[wrk_p->index]),0,sizeof(wrk_p->info[wrk_p->index]));
	index = wrk_p->index;
	pthread_mutex_unlock(&(wrk_p->mutex));
	len = snprintf(wrk_p->info[index].str,sizeof(wrk_p->info[index].str),"%s;", funcname );
	va_start(args, format);	/* pgr0039 */
	len +=  vsnprintf( &wrk_p->info[index].str[len] , sizeof(wrk_p->info[index].str) - len, format, args);
	va_end(args);
	clock_gettime(CLOCK_REALTIME,&(wrk_p->info[index].t_count));
	wrk_p->info[index].level = logLevel;
	if( pthread_mutex_trylock(&(wrk_p->mutex)) != 0)
	{
		return;
	}
	wrk_p->info[index].flg = 1;
	wrk_p->index = (unsigned int)((index + 1) % D_CMN_COM_LOG_MAX_ASSERTLOGNUM_TH);	
	pthread_mutex_unlock(&(wrk_p->mutex));
	return;
}

/*!
 * @brief		スレッド毎アサートログ情報出力処理
 * @note		スレッド毎アサートログ情報出力する処理
 * @param		*fp			[in]	ファイルポインタ
 * @param		*buf_p		[in]	プロセス内アサートログ情報アドレス
 * @return		*fp	:	出力後のファイルポインタ
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2014/03/06 FJT)Taniguchi create
 */
FILE* f_cmn_com_assert_th_print( FILE* fp , void* buf_p )
{
	T_CMN_COM_LOG_ASSERT_TH*	wrk_p = buf_p;
	struct tm time;
	struct tm *tmp_time;
	unsigned int	count,count2;
	unsigned int	index;
	int rtn;
	int unsec;
	
	if (( rtn = f_cmn_com_memchecker((unsigned char*)wrk_p)) != 0 )
	{
		fprintf(fp,"bufp addr error \n");
		fflush(fp);
		return fp;
	}
	/* だめならあきらめる */
	if( pthread_mutex_trylock(&(wrk_p->mutex)) != 0)
	{
		fprintf(fp,"f_cmn_com_assert_th_print lock error \n");
		fflush(fp);
		return fp;
	}
	index = wrk_p->index;
	fprintf(fp,"NAME:%s,Index:%d,level:0x%x,Tid:%d \n",wrk_p->name,wrk_p->index,wrk_p->level,wrk_p->tid);
	fprintf(fp,"------------------------------------------------------------------------------------------------------\n");
	for(count=0 , count2 = 1 ; count < D_CMN_COM_LOG_MAX_ASSERTLOGNUM_TH ; count++ , index++)
	{
		index %= D_CMN_COM_LOG_MAX_ASSERTLOGNUM_TH;
		if( wrk_p->info[index].flg != 1)
		{
			continue;
		}
		if(( tmp_time = localtime_r( &(wrk_p->info[index].t_count.tv_sec),&time)) == NULL)
		{
			continue;
		}
		unsec = (int)((wrk_p->info[index].t_count.tv_nsec) % 1000000);
		fprintf(fp,"[No:%03d]%04d/%02d/%02d %02d:%02d:%02d:%03d.%03d.%03d;%08X;%s\n",
				count2,
				time.tm_year+1900,	
				time.tm_mon+1,
				time.tm_mday,
				time.tm_hour,
				time.tm_min,
				time.tm_sec,
				(int)((wrk_p->info[index].t_count.tv_nsec) / 1000000),
				(int)(unsec / 1000),
				(int)(unsec % 1000),
				wrk_p->info[index].level,
				wrk_p->info[index].str
		);
		count2++;
	}
	fflush(fp);
	pthread_mutex_unlock(&(wrk_p->mutex));
	return fp;
}

/*!
 * @brief		プロセス内全スレッドアサートログ情報出力処理
 * @note		プロセス内の全スレッドのアサートログ情報出力する処理
 * @param		*fp			[in]	ファイルポインタ
 * @return		*fp	:	出力後のファイルポインタ
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2014/03/06 FJT)Taniguchi create
 */
FILE* f_cmn_com_assert_th_print_all( FILE* fp )
{
	unsigned int	count;
	int		rtn;
	
	if(cmn_com_assert_mng_p == NULL)
	{
		fprintf(fp,"not entry \n");
		fflush(fp);
		return fp;
	}
	if (( rtn = f_cmn_com_memchecker((unsigned char*)cmn_com_assert_mng_p)) != 0 )
	{
		fprintf(fp,"cmn_com_assert_mng_p error \n");
		fflush(fp);
		return fp;
	}
	if( pthread_mutex_trylock(&(cmn_com_assert_mng_p->mutex)) != 0)
	{
		fprintf(fp,"f_cmn_com_assert_th_print_all lock error \n");
		fflush(fp);
		return fp;
	}
	fprintf(fp,"PID:%d LogNum:%d \n",cmn_com_assert_mng_p->create_pid,cmn_com_assert_mng_p->thlog_counter);
	for(count = 1 ; ( count <= ( cmn_com_assert_mng_p->thlog_counter )) && ( count <= D_CMN_COM_LOG_ASSERT_THDTBLNUM ) ; count++)
	{
		if ( cmn_com_assert_mng_p->thlog_p[count] == NULL)
		{
			break;
		}
		fprintf(fp,"------------------------------------------------------------------------------------------------------\n");
		fprintf(fp,"[No:%d]",count);
		fp = f_cmn_com_assert_th_print(fp,cmn_com_assert_mng_p->thlog_p[count]);
	}
	fflush(fp);
	pthread_mutex_unlock(&(cmn_com_assert_mng_p->mutex));
	return fp;
}

#include <unistd.h>
#define D_CMN_COM_DBGCMD_MAX	16

/*!
 * @brief		スレッド内アサートログ出力コマンド処理
 * @note		スレッド内アサートログ出力コマンド処理
 * @param		*cmdStr		[in]	ファイルポインタ
 * @param		*fp			[in]	ファイルポインタ
 * @return		コマンド結果
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2014/03/06 FJT)Taniguchi create
 */
int dbgcmd_exec_ast_th_main( int argc ,char **argv , FILE* outFp)
{
	int				rtn;
	
	while((rtn = getopt(argc, argv, "d")) != -1)
	{
		switch( rtn )
		{
			case 'd':	/* dsp */
				/* all */
				(void)f_cmn_com_assert_th_print_all(outFp);	
				break;
			default:
				printf("not supported\n");
				break;
		}
		break;
	}
	return 0;
}


/*!
 * @brief		スレッド内アサートログ出力コマンド受付処理
 * @note		スレッド内アサートログ出力コマンド受付処理
 * @param		*cmdStr		[in]	ファイルポインタ
 * @param		*fp			[in]	ファイルポインタ
 * @return		コマンド結果
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2014/03/06 FJT)Taniguchi create
 */
int dbgcmd_exec_ast_th( char *cmdStr, FILE *outFp )
{
	unsigned int		cmdCnt;
	unsigned int		argcCnt = 0;
	char				*argvParam[ D_CMN_COM_DBGCMD_MAX ];
	char				*getToken, *savePtr;
	char				*delimiter = " ";


	if((outFp == NULL) || (cmdStr == NULL) )
	{
		printf("File or cmdStr pointer Error %p , %p \n",outFp,cmdStr);
		return 0;
	}
	getToken = cmdStr;
	for(argcCnt = 0 ; argcCnt < D_CMN_COM_DBGCMD_MAX ; argcCnt++ )
	{
		argvParam[ argcCnt ] = strtok_r( getToken, delimiter, &savePtr );
		if( NULL == argvParam[ argcCnt ] )
		{
			break;
		}
		/* 2パラメータ以降は NULL指定で取り出す */
		getToken = NULL;
	}
	if(argcCnt < 3)
	{
		fprintf(outFp,"command param error %d\n",argcCnt);
		return 0;
	}
	return dbgcmd_exec_ast_th_main ( argcCnt , argvParam ,outFp );
}


#ifdef OPT_CMN_ASSERTLOG_TEST	/* 以下はサンプルである */
int main(int argc, char **argv)
{
	void* buf_p;
	FILE* fp;
	unsigned int count;

	/* 1.スレッド毎に起動時に以下をコールしてね 。return値であるbuf_pは以降で使うので、忘れず保存しておいてね */
	/* 第一引数はスレッド内で設定できるログレベルだよ */
	/* 第二引数はログテーブルの名前だよ。好きな名前にしてねただし文字数には制限があるよ */
	buf_p = f_cmn_com_assert_th_create(D_RRH_LOG_AST_DEFAULT,"test threads ");

	/* 2.スレッド内でログとりたい場合は以下のようにすれば取れるよ */
	/* 第一引数はf_cmn_com_assert_th_createで帰ってきたretrun値だよ */
	/* 第二引数以降はBPFのアサート関数と一緒だよ */


	cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ENTER   ,"%s","test D_RRH_LOG_AST_LV_ENTER");
	cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_RETURN  ,"%s","test D_RRH_LOG_AST_LV_RETURN");
	cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_INFO    ,"%s","test D_RRH_LOG_AST_LV_INFO");
	cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_WARNING ,"%s","test D_RRH_LOG_AST_LV_WARNING");
	cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR   ,"%s","test D_RRH_LOG_AST_LV_ERROR");
	cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_CRITICAL,"%s","test D_RRH_LOG_AST_LV_CRITICAL");
	for(count=0; count < 1024 ; count++)
	{
		cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_CRITICAL,"all test %d , %d ",count,getpid());
	}
	/* 3.障害ログ時にファイル出力する場合は、以下のようにできるよ */

	/* プロセス内の全スレッド分の一括取得時 */
	fp = f_cmn_com_assert_th_print_all(stdout);	/* これはtest用にstdoutにしているけど、通常はファイルポインタでお願いね */

	for(count=0; count < 1024 ; count++)
	{
		cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR ,"one test %d , %d ",count,getpid());
	}
	/* スレッド個別取得時 */
	fp = f_cmn_com_assert_th_print(stdout,buf_p);	/* これはtest用にstdoutにしているけど、通常はファイルポインタでお願いね */

	/* 以下の関数はつかうことないと思うよ(test用だよ) */
	f_cmn_com_assert_th_destroy();	/* 不要なら破壊 */

	return(0);
}
#endif
/* @} */
