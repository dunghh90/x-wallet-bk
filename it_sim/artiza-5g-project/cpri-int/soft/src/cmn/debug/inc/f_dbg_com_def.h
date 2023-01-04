/*!
 * @skip  $ld:$
 * @file  dbg_com_def.h
 * @brief Function prototype definition for LOG thread.
 * @date  2013/11//2 ALPHA)宮崎
 *	All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

#ifndef DBG_COM_DEF_H_
#define DBG_COM_DEF_H_

/*
 * コマンド実行結果
 */
#define		RTC_OK						0				/**< 正常				*/
#define		RTC_NG						1				/**< 異常				*/
#define		D_CMD_OK					0				/**< 正常				*/
#define		D_CMD_NG					1				/**< 異常				*/

#define		DBG_CMD_LOGLEVEL			0				/**< デバッグコマンドログレベル	*/

#define		D_DBG_CMD_RST_SUCCESS		0				/**< */
#define		D_DBG_CMD_RST_ERROR			11				/**< */

/*
 *
 */
#define		D_DBG_INIT_COMP_IND				0x0102		/**< 初期化完了通知					*/

/**
 * @name コマンド実行状態
 * @note コマンド実行した際の状態について定義する
 */
enum		{
			D_DBG_CMD_EXEC_SUCCESS  =	0,				/**< コマンド実行成功				*/
			D_DBG_CMD_EXEC_PROC_MISMATCH,				/**< コマンド実行対象不正			*/
			D_DBG_CMD_EXEC_DLOPEN_ERR,					/**< コマンド実行用DLオープンエラー	*/
			D_DBG_CMD_EXEC_OFILE_OP_ERR,				/**< コマンド実行結果出力先ファイルオープンエラー	*/
			D_DBG_CMD_EXEC_FUNC_ERR,					/**< コマンド処理実行関数エラー	*/
			D_DBG_CMD_EXEC_OFILE_CL_ERR,				/**< コマンド実行結果出力ファイルクローズエラー	*/
} E_DBG_CMD_STATUS	;

#define		D_DBG_CMD_STR_MAX_LEN		512				/**< コマンド実行文字列最大数	*/

#endif /* DBG_COM_DEF_H_ */
