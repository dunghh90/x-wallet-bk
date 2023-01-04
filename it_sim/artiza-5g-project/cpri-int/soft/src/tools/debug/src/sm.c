/*!
 * @skip   $Id$
 * @file   sm.c
 * @brief  set memory
 * @date   2013/11/22 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */
#include "f_dbg_cmd_inc.h"

/*!
 * @brief 関数機能概要:レジスター書き込み（DCM既存展開）メイン処理
 * @note  関数処理内容.
 *       -# パラメータに異常がある場合は使用法を表示して終了
 *       -# system関数で cmd_reg_access w argv[1] argv[2] argv[3] を実行する
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @return INT
 * @retval D_DBG_CMD_RST_SUCCESS	0:正常終了
 * @retval D_DBG_CMD_RST_ERROR		1:異常終了
 * @date 2013/11/22 ALPHA)宮崎 create
 */


int sm_main(int argc, char *argv[], FILE *outfp )	{
	char	cmd_str[256];

	if( argc == 3 ) {
		/* 中で機能が重複する cmd_reg_access w アドレス サイズ データを実行する */
		snprintf( cmd_str, sizeof(cmd_str), "cmd_reg_access w %s %s", argv[1], argv[2] );
		return  system( cmd_str );	/* cmd_reg_access 実行 */
	}
	else {
		fprintf( outfp, "usage: sm addr data\n");
		return D_DBG_CMD_RST_ERROR ;
	}
	return D_DBG_CMD_RST_SUCCESS	;
}
/* @} */

