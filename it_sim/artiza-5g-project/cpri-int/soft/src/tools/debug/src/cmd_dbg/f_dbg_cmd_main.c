/*!
 * @skip   $Id$
 * @file   cmd_dbg_main.c
 * @brief  デバッグコマンド・メイン処理
 * @date   2013/11/22 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */

#include	"f_dbg_cmd_inc.h"


/*
 * 関数プロトタイプ宣言
 */

int check_parameter( int , char *[] );
int cat_parameter( int , char *[], int , char *);
int dbg_common_init( int , char *[] ) ;
int dbg_exec_local_cmd( int, char *[], int, char * );
int dbg_recv_exec_cmd_rep( int, char * );
int dbg_send_exec_cmd_req();
void dbg_common_terminate() ;
int display_result(char *) ;


/*!
 * @brief 関数機能概要:デバッグコマンドメイン処理
 * @note  関数処理内容.
 *       -# アーギュメントに従ってコマンド定義テーブルをチェックする(check_parameter())
 *       -  対応しているコマンドの場合
 *          -# コマンド実行要求の送信先の情報をコマンド定義テーブルより取得する
 *       -  対応していないコマンドの場合
 *          -# エラーメッセージを表示し終了する
 *       -# デバッグコマンド実行要求の送信のためのコマンドラインを結合する(cat_parameter())
 *       -# デバッグコマンド共通初期化処理を実行(dbg_common_init())
 *       -# 対象となるプロセスのデバッグスレッドへデバッグコマンド実行要求を送信する(dbg_send_exec_cmd_req())
 *       -# 送信先からの応答(デバッグコマンド実行応答)を待つ
 *       -  デバッグコマンド実行応答の実行結果が良好な場合
 *          -# 同メッセージ中のファイル名として指定されているファイルを catコマンドを使って表示する。(dbg_common_terminate())
 *          -# 表示後ファイルを削除する。
 *       -  異常な場合
 *          -# エラーメッセージを表示する。
 * @param  argc			[in]int		アーギュメント数
 * @param  *argv		[in]char[]	アーギュメント
 * @return INT
 * @retval D_DBG_CMD_RST_SUCCESS	0:正常終了
 * @retval D_DBG_CMD_RST_ERROR		1:異常終了
 * @date 2013/11/25 ALPHA)宮崎 create
 */


int	dbg_cmd_main( int argc, char *argv[] )	{
	char	cat_cmd_str[512]			;
	char	output_filename[PATH_MAX]	;
	int		targetno	= -1			;
	extern T_DBG_CMD_DEF_TBL	DebugCommandDef[];

	/* コマンドパラメータチェック */
	/* コマンドパラメータをチェックして送信先を特定する	*/
	targetno = check_parameter( argc, argv );

	if( targetno == -1 ) {		/* 一致するコマンドが無い？	*/
		/* できなかったらエラーとする
		 * 案内メッセージを既に出力済のため、ここでは何も出力しない
		 */
		return(D_DBG_CMD_RST_ERROR) ;
	}

	/*   コマンドライン文字列結合	*/
	if( cat_parameter( argc, argv, sizeof(cat_cmd_str), cat_cmd_str ) != RTC_OK ) {	/* 文字列結合は成功？	*/
		/* 失敗した場合はエラーとする
		 * 案内メッセージとか既に出力済のため、ここでは何も出力しない
		 */
		return(D_DBG_CMD_RST_ERROR);
	}
	/* デバッグ共通初期化処理	*/
	if( dbg_common_init( argc, argv )	!= RTC_OK ) {	/* 共通初期化処理成功？	*/
		/* 失敗した場合はエラーとする
		 * 案内メッセージとか既に出力済のため、ここでは何も出力しない
		 */
		return(D_DBG_CMD_RST_ERROR);
	}

	/****************/
	/* コマンド実行	*/
	/****************/
	if( strlen( DebugCommandDef[targetno].exec_thread ) == 0 &&
			DebugCommandDef[targetno].send_dbg_thrad_queid == 0 &&
			DebugCommandDef[targetno].send_proc_queid      == 0     )		{	/* ローカル実行の場合	*/

		/****************/
		/* ローカル実行	*/
		/****************/
		memset(output_filename,0,sizeof(output_filename));
		if( dbg_exec_local_cmd(  argc, argv, targetno, output_filename ) != 0 ) {

			/* デバッグ共通終了処理	*/
			dbg_common_terminate();

			return(D_DBG_CMD_RST_ERROR);

		}
	}
	else {

		/************************/
		/* 別プロセスにて実行	*/
		/************************/

		/* コマンド実行要求送信 */
		if( dbg_send_exec_cmd_req( targetno, cat_cmd_str )  != RTC_OK ) {

			/* デバッグ共通終了処理	*/
			dbg_common_terminate();

			return(D_DBG_CMD_RST_ERROR);
		}

		/* 処理結果受信 */
		if( dbg_recv_exec_cmd_rep( targetno, output_filename ) != RTC_OK )	{

			/* デバッグ共通終了処理	*/
			dbg_common_terminate();

			return(D_DBG_CMD_RST_ERROR);
		}
	}

	/* デバッグ共通終了処理	*/
	dbg_common_terminate();

	/* ファイル表示	*/
	if(display_result( output_filename ) ) {	/* 処理結果正常？ */
		return D_DBG_CMD_RST_ERROR;
	}
	return D_DBG_CMD_RST_SUCCESS	;
}

/*!
 * @brief 関数機能概要:デバッグコマンドメイン処理
 * @note  関数処理内容.
 *       -# メイン処理を実行する
 * @param  argc			[in]int		アーギュメント数
 * @param  *argv		[in]char[]	アーギュメント
 * @return INT	メイン処理の返り値
 * @retval D_DBG_CMD_RST_SUCCESS	0:正常終了
 * @retval D_DBG_CMD_RST_ERROR		1:異常終了
 * @date 2013/11/25 ALPHA)宮崎 create
 */
int	main( int argc,	char *argv[] )	{

	/*
	 * デバッグコマンドメイン処理を実行しその返り値でリターンする
	 */
	return dbg_cmd_main( argc, argv ) ;
}

/* @} */

