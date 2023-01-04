/*!
 * @skip   $Id$
 * @file   f_dbg_thread_type.h
 * @brief  デバッグスレッド型定義ヘッダ
 * @date   2013/11/22 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */

#ifndef F_DBG_THREAD_TYPE_H_
#define F_DBG_THREAD_TYPE_H_
#if 0
/**
 * @brief	デバッグコマンド実行要求リクエスト情報型定義
 * @note	デバッグコマンド実行要求リクエストの詳細情報を定義する
 */
typedef struct	{
	UINT	cmd_exec_procQid				;							/**< コマンド実行プロセス							*/
	UINT	cmd_exec_threadQid				;							/**< デバッグスレッドキューID						*/
	char	cmd_exec_thread[10]				;							/**< コマンド実行スレッド名							*/
	char	cmd_str[512]	;							/**< コマンド文字列									*/
} T_DBG_EXEC_CMD_REQ_INFO	;



/**
 * @brief	デバッグコマンド実行要求リクエスト型定義
 * @note	デバッグコマンド実行要求リクエストの型を定義する
 */
typedef struct	{
	CMT_TSKIF_HEAD								head;					/**< Thread Common Header						*/
	T_DBG_EXEC_CMD_REQ_INFO						dbg_exec_req_info;		/**< デバッグ実行要求情報						*/
} T_DBG_EXEC_CMD_REQ	;



/**
 * @brief	デバッグコマンド実行要求レスポンス情報型定義
 * @note	デバッグコマンド実行要求レスポンスの詳細情報を定義する
 */
typedef struct	{
	UINT	cmd_exec_procQid				;							/**< コマンド実行プロセス							*/
	UINT	cmd_exec_threadQid				;							/**< デバッグスレッドキューID						*/
	char	cmd_exec_thread[10]				;							/**< コマンド実行スレッド名							*/
	char	cmd_str[512]					;							/**< コマンド文字列									*/
	int		cmd_result						;							/**< コマンド実行結果								*/
	char	cmd_result_fname[200]			;							/**< 実行結果ファイル名								*/
}	T_DBG_EXEC_CMD_RES_INFO	;


/**
 * @brief	デバッグコマンド実行要求レスポンス型定義
 * @note	デバッグコマンド実行要求レスポンスの型を定義する
 */
typedef struct	{

	CMT_TSKIF_HEAD								head;					/**< Thread Common Header						*/

	T_DBG_EXEC_CMD_RES_INFO							dbg_exec_res_info;		/**< デバッグ実行結果レスポンス情報				*/
}	T_DBG_EXEC_CMD_RES	;

#endif


/**
 * @brief スレッド情報テーブル型
 * @note デバッグスレッドのプロセスID、スレッドIDとQueIdとスレッド名を定義する
 */

typedef struct {
	char	*myThreadName;		/**< スレッド名		*/
	UINT	myProcessId;		/**< プロセスID		*/
	UINT	myProcessQId;		/**< プロセスQueID	*/
	UINT	myThreadId;			/**< スレッドID		*/
	UINT	myThreadQId;		/**< スレッドQueID	*/
} T_DBG_MYTHREAD_INFO	;


/**
 * @brief コマンド実行情報型
 * @note コマンド実行時の実行対象関数を定義する
 */
#if 0
typedef	struct	{
	UINT	myProcessQid	;	/**< プロセスQueID		*/
	char	*execThreadName	;	/**< 実行スレッド名		*/
	int		(*execFunc)(char *cmd, FILE *ofp)	;	/**< 実行関数名			*/
} T_DBGCMD_EXEC_FUNC;
#endif

typedef	struct	{
	UINT	myProcessQid	;	/**< プロセスQueID									*/
	char	*execThreadName	;	/**< 実行スレッド名									*/
	char	*dlName			;	/**< デバッグ関数格納ダイナミックリンクライブラリ名	*/
	char	*execFuncName	;	/**< 実行関数名										*/
} T_DBGCMD_EXEC_FUNC;

#endif /* F_DBG_THREAD_TYPE_H_ */

/* @} */
