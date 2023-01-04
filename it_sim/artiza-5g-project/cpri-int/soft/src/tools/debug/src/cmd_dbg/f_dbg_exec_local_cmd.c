/*!
 * @skip   $Id$
 * @file   f_dbg_exec_local_cmd.c
 * @brief  ローカルコマンド実行処理
 * @date   2013/11/22 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */

#include <dlfcn.h>
#include	"f_dbg_cmd_inc.h"


/*!
 * @brief 関数機能概要:ローカルコマンド実行処理
 * @note  関数処理内容.
 * @param  tergetno     [in]int     実行コマンド
 * @param  output_filename[out]char * 出力先ファイル名
 * @return INT
 * @retval result 0:正常/-1:エラー
 * @date 2013/12/20 ALPHA)宮崎 create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 *
 */
extern T_DBG_CMD_DEF_TBL	DebugCommandDef[];

int dbg_exec_local_cmd( int argc,  char *argv[], int tergetno, char *output_filename )	{

	int							(*execFunc)(int , char *[], FILE *fp);
    void    					*dlHandle		;		/* DLハンドラ*/
    char    					*dlErrorMessage	;
	int							cmdResult		;


	/* ダイナミックリンクライブラリのオープン */
    if(( dlHandle = dlopen( DebugCommandDef[tergetno].exec_dlname, RTLD_LAZY ) ) == (void *)NULL) {
    	fprintf(stderr, "Open dynamic link library Fail. name:%s", DebugCommandDef[tergetno].exec_dlname ) ;
    	return RTC_NG;
    }

    /* 実行関数のポインタ取得   */
    execFunc = dlsym( dlHandle, DebugCommandDef[tergetno].exec_func_name ) ;
    dlErrorMessage = dlerror() ;
    if( dlErrorMessage != NULL ) {       /* 失敗有り */
    	fprintf( stderr, "Execute Function Search Error: %s\n", dlErrorMessage );

		/* ダイナミックリンクファイルクローズ	*/
		dlclose( dlHandle ) ;

		return RTC_NG;
    }

    /* 取得した関数の実行  出力は標準出力へ出す */
	cmdResult = (*execFunc)( argc, argv, stdout ) ;

	/* ダイナミックリンクファイルクローズ	*/
	dlclose( dlHandle ) ;

	if( cmdResult != 0 ) {
		return RTC_NG;
	}
	return RTC_OK;
}
/* @} */
