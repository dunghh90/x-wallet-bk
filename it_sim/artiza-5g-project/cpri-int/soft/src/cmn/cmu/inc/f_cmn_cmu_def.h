/*!
 * @skip		$ld:$
 * @file		f_cmn_cmu_def.h
 * @brief		cmu define定義
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_CMU
 * @{
 */

#ifndef F_CMU_DEF_H
#define F_CMU_DEF_H

/*!
 * @name CMUスレッド状態
 * @note CMUスレッド状態定義
 * @{
 */
/* CMUスレッドの状態定義						*/
#define	D_CMU_THD_STS_INIT							0x0000						/*!< 初期状態						*/
#define	D_CMU_THD_STS_USE							0x0001						/*!< 運用中							*/
#define	D_CMU_STATENUM								0x0002						/*!< 状態数							*/
/* @} */

/*!
 * @name CMUイベント
 * @note CMUイベント定義
 * @{
 */
/* CMUイベント定義						*/
#define	D_CMU_THD_EVE_INIT							0x0000						/*!< 初期化イベント					*/
#define	D_CMU_THD_EVE_MSG							0x0001						/*!< メッセージ受信イベント			*/
#define	D_CMU_STATENUM								0x0002						/*!< イベント数						*/
/* @} */

/* 初期化完了通知 */
#define	D_CMU_INIT_REQ								0x0101						/*!< 初期化要求						*/
#define	D_CMU_INIT_COMP_IND							0x0102						/*!< 初期化完了通知					*/
/* 各種ID数 */
#define	D_CMU_RECVEVT_NUM							2

/* CMUスレッド数 */
#define	D_CMU_THREAD_NUM							2
/* CMUスレッド名格納配列数 */
#define	D_CMU_NAME_NUM								32

#if defined (OPT_RRH_DCM_ZYNQ)
/* CPRI信号受信通知は複数の意味を持つイベントIDのためCMUで定義する */
#define D_CMD_CPRIRCVNTC							0x0301
#define D_CMD_SIGKIND_BIT							0x0001
#else
/* DCM以外では複数の意味を持つイベントIDが存在するか不明だが、あればここで定義すること */
#define D_CMD_CPRIRCVNTC							0x0301 //M&C update
#define D_CMD_SIGKIND_BIT							0x0001 //M&C update
#endif


/* Data 最大値 */
#define	D_CMU_DATA_MAX								0x00021000
/* thread名のサイズ */
#define	D_CMU_THDNAME_LEN							16

/* route information of message table の最大値 */
// 擬似環境 20210311
//#define	D_CMU_TABLE_MAX								64							/*!< テーブルの最大数				*/
#define	D_CMU_TABLE_MAX								109							/*!< テーブルの最大数				*/
// 擬似環境 20210311

/* スレッド内イベント番号(ログ用)				*/
#define	D_CMU_EVT_NO1								0x1001						/*!< スレッド内イベント番号No1		*/
#define	D_CMU_EVT_NO2								0x1002						/*!< スレッド内イベント番号No2		*/
#define	D_CMU_EVT_NO3								0x1003						/*!< スレッド内イベント番号No3		*/

/* CMU ROMテーブル				*/
#define	D_CMU_INDEX_MSGQUE							0							/*!< プロセスキューID				*/
#define	D_CMU_INDEX_THREAD_QID						1							/*!< スレッドキューID				*/

/* CMUスレッドRun History関連				*/
#define	D_CMU_MAX_RUNHISBODY						128							/*!< Maximum running history body		*/
#define	D_CMU_MAX_RUNHISREC							D_COM_MAX_RUNHISREC			/*!< Maximum running history record		*/
#define	D_CMU_RECIEVEFLG							0							/*!< cmu-recv recieve Flag				*/
#define	D_CMU_SENDFLG								1							/*!< cmu-recv send flag					*/

/* CMU ROMテーブル				*/
#define	D_CMU_OK									0							/*!< 正常							*/
#define	D_CMU_NG									1							/*!< 異常							*/
#define	D_CMU_NG_MUTEX_LOCK							2							/*!< Mutexロック失敗				*/
#define	D_CMU_NG_MUTEX_UNLOCK						3							/*!< Mutexアンロック失敗			*/
#define	D_CMU_NG_TABLE_FULL							4							/*!< テーブルが満					*/
#define	D_CMU_NG_TABLE_EMPTY						5							/*!< テーブルが空					*/
#define	D_CMU_NG_EXIST								6							/*!< 同一IDが存在					*/


#define	D_CMU_MPTABLE_MAX							64							/*!< テーブルの最大数				*/



#endif
/* @} */
