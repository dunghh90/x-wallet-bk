/*!
 * @skip   $Id$
 * @file   f_dbg_cmd_cat_parametor.c
 * @brief  デバッグコマンド　パラメータ文字列結合
 * @date   2013/11/22 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */

#include	"f_dbg_cmd_inc.h"


/*!
 * @brief 関数機能概要:パラメータ結合
 * @note  関数処理内容.
 *       -# コマンドパス（argv[0]）のディレクトリを除いたコマンド名部分のみを切り出す
 *       -# 切り出したコマンド名部分＋他のアーギュメントをスペース区切りで結合する。(その際、パラメータstrlen_limitで渡された文字列数を超過しないように結合する)
 *       -# 超過した場合、エラーとして上位に返す
 *       -# 超過しないで全てのパラメータを結合できた場合は、正常として上位に返す。
 *
 * @param  argc			[in]int			コマンドアーギュメント数
 * @param  argv			[in]char*[]		コマンドアーギュメント
 * @param  strlen_limit [in]int			コマンドライン結合最大長限度
 * @param  cat_cmd_str	[out]char *		結合結果
 * @return INT
 * @retval result 0:正常/1:エラー
 * @date 2013/11/25 ALPHA)宮崎 create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */

int cat_parameter( int argc, char *argv[], int strlen_limit, char *cat_cmd_str ){

	int 	argind = 0	;	/* 追加位置		*/
	int 	adderr = 0 	;	/* 追加エラー	*/
	int		cmd_str_found = 0 ;
	int 	cmd_str_ind = 0 ;
	char	arg0str[20];

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

	strcpy( cat_cmd_str, arg0str );

	for( argind = 1 ; argind < argc - 1 ; argind++ ) { /* コマンドアーギュメント数分 - 1個分  */
		if( strlen(cat_cmd_str) + strlen( argv[argind+1] ) + 1 >= strlen_limit ) {  /* 後ろへつける前にサイズ超過を確認 */
			fprintf(stderr,"Error: argument too long..\n");
			adderr = 1 ;
			break;
		}
		if( argind != 0 ) {	/* 初回以外	*/
			strcat( cat_cmd_str, " ");	/* スペースで次々とつなげる */
		}
		strcat( cat_cmd_str, argv[argind] ) ;
	}
	if( argc > argind ) {
		if( adderr == 0 && strlen(cat_cmd_str) + strlen(argv[argind]) + 1 <= strlen_limit ) {	/* 次のパラメータ分も追加できるか？	*/
			strcat( cat_cmd_str, " ");
			strcat( cat_cmd_str, argv[argind] ); /* 最後に１回追加する   */
		}
		else {
			fprintf(stderr, "Error: commnd argment too long..\n");
			return -1;
		}
	}
	return 0 ;
}

/* @} */

