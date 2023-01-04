/*!
 * @skip  $ld:$
 * @file  f_ful_def.h
 * @brief FULタスク デファイン定義.
 * @date  2019/01/28 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */


/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */
#ifndef F_FUL_DEF_H
#define F_FUL_DEF_H

/*!
 * @name FUL戻り値
 * @note FUL戻り値の定義
 * @{
 */
#define	D_FUL_RTN_COMPLETE				0			/*!< 正常終了					*/
#define	D_FUL_RTN_NG					1			/*!< 異常終了					*/
#define	D_FUL_RTN_INPUTERR				256			/*!< 異常(インプットエラー)		*/
/* @} */


/*!
 * @name FULタスク状態
 * @note FULタスクの状態定義
 * @{
 */
#define	D_FUL_TASK_STS_INIT				0x0000		/*!< 初期状態					*/
#define	D_FUL_TASK_STS_ACT				0x0001		/*!< 運用状態					*/
#define	D_FUL_STATENUM					0x0002		/*!< FULタスク状態数			*/
/* @} */

/*!
 * @name 
 * @note 
 * @{
 */


/* @} */


/*!
 * @name FULタスクイベント
 * @note FULタスクのイベント定義
 * @{
 */
enum
{
	E_FUL_EVE_INIT = 0 ,								/*!< スレッド起動						*/
	E_FUL_EVE_FILELIST ,								/*!< File-List転送要求イベント			*/
	E_FUL_EVE_UPLOAD ,									/*!< アップロード要求イベント			*/
	E_FUL_EVE_EVENTNUM									/*!< FULタスクイベント数				*/
};
/* @} */


enum
{
	E_FUL_ERR_PARA = 0 ,								/*!< 入力パラメータエラー				*/
	E_FUL_ERR_SFTP ,									/*!< sftpコマンドエラー					*/
	E_FUL_ERR_NUMMAX
};

#define D_APP_PATH "/O-RAN"
#define D_APP_PATH2 "/O-RAN/"

#endif
/* @} */
