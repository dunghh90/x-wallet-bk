/*!
 * @skip		$ld:$
 * @file		f_dbg_cmd_type.h
 * @brief		デバッグコマンド構造体型定義ヘッダ
 * @date		2013/11/26 ALPHA)宮崎 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013,2014
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */

#ifndef CMD_DBG_DEF_H_
#define CMD_DBG_DEF_H_



/**
 * @brief	コマンド実行情報定義
 * @note	コマンド実行情報定義の定義
 *
 */
typedef	struct	{
	char	*arg0;						/**< コマンド名				*/
	char	*arg1;						/**< パラメータ#1				*/
	char	*arg2;						/**< パラメータ#2				*/
	char	*arg3;						/**< パラメータ#3				*/
	char	*arg4;						/**< パラメータ#4				*/
	char	*arg5;						/**< パラメータ#5				*/
	int		send_proc_queid;			/**< 送信先プロセスキュー定義	*/
	int		send_dbg_thrad_queid;		/**< 送信先スレッドキュー定義	*/
	char	*exec_thread;				/**< 実行スレッド名			*/
	char	*exec_dlname;				/**< ロードＤＬ名				*/
	char	*exec_func_name;			/**< 実行関数名					*/
}	T_DBG_CMD_DEF_TBL	;


#endif /* CMD_DBG_DEF_H_ */

/* @} */

