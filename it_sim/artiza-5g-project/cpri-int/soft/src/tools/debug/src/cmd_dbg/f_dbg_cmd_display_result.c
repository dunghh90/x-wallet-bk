/*!
 * @skip   $Id$
 * @file   f_dbg_cmd_display_result.c
 * @brief  デバッグコマンド　コマンド実行結果表示処理
 * @date   2013/11/22 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */

#include <string.h>
#include	"f_dbg_cmd_inc.h"


/*!
 * @brief 関数機能概要:コマンド実行結果表示処理
 * @note  関数処理内容.
 *       -#  パラメータのファイル名をチェックする
 *       -   ファイル名が空の場合
 *           -# エラーメッセージを表示し正常として上位へ戻る
 *       -#   cat コマンドを使って実行結果ファイルを表示する
 *       -#   表示終了後ファイルを削除する
 * @param  target		[in]int		コマンド実行先
 * @param  cmd_str		[in]char*	コマンドパラメータ文字列
 * @return INT
 * @retval result 0:正常/-1:エラー
 * @date 2013/11/25 ALPHA)宮崎 create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 *
 */

int display_result( char *output_filename )	{

	char	cmd_str[D_DBG_CMD_STR_MAX_LEN]	;
	int		rt;

	if( output_filename == NULL )	{	/* ファイル名が空	*/
		fprintf(stderr, "Error: no output file.\n");
		return RTC_NG ;	/* ファイル名部分は既に確保されている領域のはずなためNULLが渡ってきたら何かの障害が有ったと考えられる*/
	}

	if( strlen(output_filename) == 0 )	{	/* ファイル名が空	*/
//		fprintf(stderr, "Error: no output file.\n");
		return RTC_NG ;	/* ファイル名部分は既に確保されている領域のはずなためNULLが渡ってきたら何かの障害が有ったと考えられる*/
	}
	/* system 関数実行文字列編集	*/
	snprintf( cmd_str, D_DBG_CMD_STR_MAX_LEN, "cat %s", output_filename ) ;

	/* system関数実行	*/
	rt = system( cmd_str ) ;
	if( rt != 0 ) {
		fprintf(stderr, "Error: result output display error.(code:%d)\n", errno );
		fprintf(stderr, "please check output file : %s\n", output_filename );
		return RTC_NG;
	}

	/* 出力ファイル削除	*/
	if( unlink( output_filename ) == -1 ) {
		fprintf( stderr, "Error: result output file delete error.(code:%d)\n", errno );
		fprintf( stderr, "please check output file : %s\n", output_filename );
		return RTC_NG;
	}
	return RTC_OK;
}
/* @} */
