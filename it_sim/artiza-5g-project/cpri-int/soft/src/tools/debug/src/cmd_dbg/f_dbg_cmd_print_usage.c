/*!
 * @skip   $Id$
 * @file   f_dbg_cmd_print_usage.c
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


/*!
 * @brief 関数機能概要:使用法説明
 * @note  関数処理内容.
 *       -# 使用法を説明する
 *       -# 表示する内容は argv[0] が一致したものを全て表示する。
 *       -# 一致しない場合は、全てのコマンドについて表示する。
 * @param  argv0		[in]char	コマンドアーギュメント#0
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

void print_usage( char *arg0 )	{

	int	cmd_ind	;
	int dsp_all_usage = 1 ;

	printf("usage: \n");
	for(cmd_ind = 0 ; ; cmd_ind++ )	{

		if( DebugCommandDef[cmd_ind].arg0 == 0 &&
			DebugCommandDef[cmd_ind].arg1 == 0 &&
			DebugCommandDef[cmd_ind].arg2 == 0  ) {	/* 終了判定 */
			break;
		}
		if( strcmp( arg0, DebugCommandDef[cmd_ind].arg0 ) == 0 ) {	/* コマンド名一致	*/
			/* コマンド、パラメータを表示  */
			printf("    %s %s %s %s %s %s\n",
					DebugCommandDef[cmd_ind].arg0,
					DebugCommandDef[cmd_ind].arg1,
					DebugCommandDef[cmd_ind].arg2,
					DebugCommandDef[cmd_ind].arg3,
					DebugCommandDef[cmd_ind].arg4,
					DebugCommandDef[cmd_ind].arg5 ) ;
			/* 表示対象が見つかったため、全部は表示しない。 */
			dsp_all_usage = 0;
		}
	}
	if( dsp_all_usage == 1 )	{	/* 全てのヘルプを表示する	*/

		for(cmd_ind = 0 ; ; cmd_ind++ )	{

			if( DebugCommandDef[cmd_ind].arg0 == 0 &&
				DebugCommandDef[cmd_ind].arg1 == 0 &&
				DebugCommandDef[cmd_ind].arg2 == 0  ) {	/* 終了判定 */
				break;
			}
			/* コマンド、パラメータを表示  */
			printf("    %s %s %s %s %s %s\n",
					DebugCommandDef[cmd_ind].arg0,
					DebugCommandDef[cmd_ind].arg1,
					DebugCommandDef[cmd_ind].arg2,
					DebugCommandDef[cmd_ind].arg3,
					DebugCommandDef[cmd_ind].arg4,
					DebugCommandDef[cmd_ind].arg5 ) ;
			
		}
	}
}
/* @} */

