/*!
 * @skip  $ld:$
 * @file  dbg_com_type.h
 * @brief Function prototype definition for LOG thread.
 * @date  2013/11//2 ALPHA)宮崎
 *	All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 *	@addtogroup RRH_DEBUG
 *  @{
 */

#ifndef DBG_COM_TYPE_H_
#define DBG_COM_TYPE_H_


/**
 * @brief	デバッグスレッド起動情報
 * @note	デバッグスレッドの起動の際の設定データ
 */
typedef struct {
	UINT		uiProcID			;			/* 起動元プロセスID						*/
	UINT		uiProcQID			;			/* 起動元プロセスメッセージQID			*/
	UINT		uiMainThradQID		;			/* 起動元 xx_main スレッドQID			*/
	char		*cpDbgThreadName	;			/* デバッグスレッド名					*/
	UINT		uiDbgThreadID		;			/* デバッグスレッドのスレッドID			*/
	UINT		uiDbgThreadQID		;			/* デバッグスレッドのメッセージQID		*/
	VOID		(*usrFunc)(char *)	;			/* ユーザー定義関数						*/
} T_DBGTHREAD_EXEC_INFO ;




/**
 * @brief	デバッグコマンド実行要求リクエスト情報型定義
 * @note	デバッグコマンド実行要求リクエストの詳細情報を定義する
 */
typedef struct	{
	char	cmd_exec_thread[10]				;							/**< コマンド実行スレッド名							*/
	char	cmd_str[255]	;							/**< コマンド文字列									*/
} T_DBG_EXEC_CMD_REQ_INFO	;



/**
 * @brief	デバッグコマンド実行要求リクエスト型定義
 * @note	デバッグコマンド実行要求リクエストの型を定義する
 */
typedef struct	{
	T_RRH_HEAD								head;					/**< Thread Common Header						*/
	T_DBG_EXEC_CMD_REQ_INFO						dbg_exec_req_info;		/**< デバッグ実行要求情報						*/
} T_DBG_EXEC_CMD_REQ	;



/**
 * @brief	デバッグコマンド実行要求レスポンス情報型定義
 * @note	デバッグコマンド実行要求レスポンスの詳細情報を定義する
 */
typedef struct	{
	int		cmd_exec_status				;								/**< コマンド起動状態							*/
	int		cmd_exec_result				;								/**< コマンド実行結果							*/
	char	cmd_result_fname[255]		;								/**< 実行結果ファイル名							*/
}	T_DBG_EXEC_CMD_RES_INFO	;


/**
 * @brief	デバッグコマンド実行要求レスポンス型定義
 * @note	デバッグコマンド実行要求レスポンスの型を定義する
 */
typedef struct	{

	T_RRH_HEAD								head;					/**< Thread Common Header						*/

	T_DBG_EXEC_CMD_RES_INFO							dbg_exec_res_info;		/**< デバッグ実行結果レスポンス情報				*/
}	T_DBG_EXEC_CMD_RES	;



#endif /* DBG_COM_TYPE_H_ */

/** @} */

