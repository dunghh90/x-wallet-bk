/*!
 * @skip   $Id$
 * @file   f_dbg_cmd_check_parameter.c
 * @brief  デバッグコマンド 使用法説明表示
 * @date   2013/11/22 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup	RRH_DEBUG
 * @{
 */
#include	"f_dbg_cmd_inc.h"
extern T_DBG_CMD_DEF_TBL	DebugCommandDef[];
extern void print_usage(char *);

/*!
 * @brief 関数機能概要:パラメータチェック
 * @note  関数処理内容.
 *       -# デバッグコマンドのパラメータ数をチェックする
 *       -  最低必要な個数＝共通パラメータ（３個）未満な場合
 *          -# コマンド使用法説明(print_usage())を実行してエラーとして上位へ返す
 *       -# 共通パラメータ部分の確認
 *       -  該当が定義されている場合
 *          -# インデックスを返す。
 *       -  該当するものが無い場合
 *          -# コマンド使用法説明(print_usage())を実行しエラーで返す(-1)。
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char[]	コマンドアーギュメント
 * @return INT
 * @retval 送信先	(-1:エラー)
 * @date 2013/11/25 ALPHA)宮崎 create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 *
 */

int check_parameter(int argc, char *argv[]  ) {

	int 	cmd_ind ;
	int		execno = -1 ;
	char	arg0str[50];
	int		cmd_str_ind ;
	int		cmd_str_found;

	/*
	 * argv[0]から余計な分を取る
	 * （後ろから探し / が出てきたらそこまでを転記する)
	 */
	cmd_str_found = 0 ;
	for( cmd_str_ind = strlen(argv[0]) ; cmd_str_ind > 0 ; cmd_str_ind-- )	{	/* arg[0]の後ろから	*/
		if( *( argv[0] + cmd_str_ind ) == '/' ) {	/* ディレクトリ区切りが見つかる？	*/
			cmd_str_found = cmd_str_ind + 1;	/* ディレクトリ区切りの位置を記憶	*/
			break;
		}
	}
	memset( arg0str, 0, sizeof(arg0str)) ;
	if( cmd_str_found != 0 )	{
		strncpy( arg0str, argv[0]+cmd_str_found, strlen(argv[0]) - cmd_str_ind );
	}
	else {	/* スラッシュが見つからなかった */
		strcpy( arg0str, argv[0] );
	}

	/*
	 * コマンド定義テーブルを検索し指定パラメータと一致するものを探す
	 */
	for( cmd_ind = 0 ; ; cmd_ind++ ){

		if( DebugCommandDef[cmd_ind].arg0 == 0 &&
			DebugCommandDef[cmd_ind].arg1 == 0 &&
			DebugCommandDef[cmd_ind].arg2 == 0  ) {	/* コマンド定義テーブルの終端 */
			break;
		}

		if( strcmp( DebugCommandDef[cmd_ind].arg0 , arg0str ) == 0 ) {					/* コマンド名一致	*/

			/* 第１パラメータ */
			if( strlen( DebugCommandDef[cmd_ind].arg1 ) != 0 ) {						/* 定義上　第１パラメータ有り？	*/
				if( argc > 1 ) {														/* 入力パラメータ２つ以上有り？	*/
					if( strcmp( DebugCommandDef[cmd_ind].arg1, argv[1] ) == 0  ) {		/* 第１パラメータ一致	*/
						/* 第２パラメータ	*/
						if( strlen( DebugCommandDef[cmd_ind].arg2) != 0 )		{		/* 定義上第２パラメータがある？	*/
							if( argc > 2 )		{											/* 第３パラメータが指定されている	*/
								if( strcmp( DebugCommandDef[cmd_ind].arg2, argv[2] ) == 0 )	{		/* 第２パラメータ 一致	*/

									/* 第３パラメータ	*/
									if( strlen( DebugCommandDef[cmd_ind].arg3 ) != 0 ) {				/* 第３パラメータが存在する	*/
										if( argc > 3 )		{												/* 第３パラメータが指定されている	*/
											if( strcmp( DebugCommandDef[cmd_ind].arg3, argv[3] ) == 0 ) {		/* 第３パラメータ 一致	*/

												/* 第４パラメータ	*/
												if( strlen(DebugCommandDef[cmd_ind].arg4) != 0 ) {					/* 第４パラメータが存在する	*/
													if( argc > 4 )		{												/* 第４パラメータが指定されている	*/
														if( strcmp( DebugCommandDef[cmd_ind].arg4, argv[4] ) == 0 ) {		/* 第４パラメータが一致	*/

															/* 第５パラメータ	*/
															if( strlen( DebugCommandDef[cmd_ind].arg5 ) != 0 ) {				/* 第５パラメータが存在する	*/
																if( argc > 5  )	{													/* 第５パラメータが指定されている	*/
																	if( strcmp( DebugCommandDef[cmd_ind].arg5, argv[5] ) == 0 ) {		/* 第５パラメータが一致	*/
																		/* これ以上チェックする対象が無いためこれにて特定できたとする */
																		execno = cmd_ind;													/* 第５パラメータまでの一致で成功とする	*/
																		break;
																	}
																}
															}
															else {
																/* 第４パラメータまで一致で特定できたとする	*/
																execno = cmd_ind;
																break;
															}
														}
													}
												}
												else {
													/* 第３パラメータまで一致で特定できたとする	*/
													execno = cmd_ind ;
													break;
												}
											}
										}
									}
									else {
										execno = cmd_ind;	/* 第２パラメータまでの一致で特定できたとする	*/
										break;
									}
								}
							}
						}
						else {
							/* 第１パラメータまでの一致で特定できたとする	*/
							execno = cmd_ind;
							break;
						}
					}
				}
			}
			else {
				/* パラメータが無いならばコマンド名のみで一致とする	*/
				execno = cmd_ind ;
				break;
			}
		}
	}
	if( execno == -1 ) {	/* テーブルの最後まで行っても対応する送り先が見つからない	*/
		fprintf(stderr, "Error: unknown command and parameters\n");
		/* コマンド使用法説明表示	*/
		print_usage( argv[0] ) ;
		return -1;
	}

	/* */
	return execno ;
}

/* @} */

