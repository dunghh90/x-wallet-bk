/*!
 * @skip  $ld:$
 * @file  f_fdl_def.h
 * @brief FDLタスク デファイン定義.
 * @date  2019/01/28 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */


/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */
#ifndef F_FDL_DEF_H
#define F_FDL_DEF_H

/*!
 * @name FDL戻り値
 * @note FDL戻り値の定義
 * @{
 */
#define	D_FDL_RTN_COMPLETE				0			/*!< 正常終了					*/
#define	D_FDL_RTN_NG					1			/*!< 異常終了					*/
#define	D_FDL_RTN_INPUTERR				256			/*!< 異常(インプットエラー)		*/
/* @} */


/*!
 * @name FDLタスク状態
 * @note FDLタスクの状態定義
 * @{
 */
#define	D_FDL_TASK_STS_INIT				0x0000		/*!< 初期状態					*/
#define	D_FDL_TASK_STS_ACT				0x0001		/*!< 運用状態					*/
#define	D_FDL_TASK_STS_INSTALL			0x0002		/*!< ローディング中状態			*/
#define	D_FDL_STATENUM					0x0003		/*!< FDLタスク状態数			*/
/* @} */

/*!
 * @name 
 * @note 
 * @{
 */
#define	D_FDL_ACT_STATE_ERROR			0x0000		/*!< 初期状態					*/
#define	D_FDL_ACT_STATE_RUN				0x0001		/*!< Activate設定状態			*/

/* @} */


/*!
 * @name FDLタスクイベント
 * @note FDLタスクのイベント定義
 * @{
 */
enum
{
	E_FDL_EVE_INIT = 0 ,								/*!< スレッド起動								*/
	E_FDL_EVE_INVENTRY ,								/*!< インベントリ要求イベント					*/
	E_FDL_EVE_DOWNLOAD ,								/*!< ダウンロード要求イベント					*/
	E_FDL_EVE_INSTALL ,									/*!< インストール要求イベント					*/
	E_FDL_EVE_ACTIBATE ,								/*!< アクティベート要求イベント					*/
	E_FDL_EVE_FILEWRT_M ,								/*!< マスタ側ファイル更新書込み応答イベント		*/
	E_FDL_EVE_FILEWRT_S ,								/*!< スレーブ側ファイル更新書込み応答イベント	*/
	E_FDL_EVE_EVENTNUM									/*!< FDLタスクイベント数						*/
};
/* @} */

#define	TRUE			1
#define	FALSE			0

#define	D_FDL_INST_STOP			0
#define	D_FDL_INST_START		1
#define	D_FDL_INST_ERROR		2
#define	D_FDL_STAT_ERROR		-1

#define	D_FDL_NOTIF_TO_DL			300
#define	D_FDL_NOTIF_TO_ACT			10

#define	D_DFL_MASTR_ACSS		0
#define	D_DFL_SLAVE_ACSS		1

enum
{
 E_FDL_ERR_OK = 0,  /* エラーなし */
 E_FDL_ERR_OPEN,    /* ファイルオープンエラー */
 E_FDL_ERR_UNZIP,   /* unzip実行エラー */
 E_FDL_ERR_NOINS,   /* 更新ファイル名一致 */
 E_FDL_ERR_ERSERR,  /* Flash消去失敗 */
 E_FDL_ERR_WRTERR,  /* Flash書込み失敗 */
 E_FDL_ERR_BUFERR,  /* バッファエラー */
 E_FDL_ERR_NOFILE,  /* 更新ファイルなし */
 E_FDL_ERR_CHKSUM,  /* 更新ファイルチェックサムエラー */
 E_FDL_ERR_INVALID, /* 更新ファイル無効 */
 E_FDL_ERR_PARAERR, /* パラメータエラー */
 E_FDL_ERR_ERRMAX   /* エラー番号数 */
};

enum
{
	E_FDL_FLSH_OSPF = 0 ,								/* OS書込み					*/
	E_FDL_FLSH_FPGAM ,									/* FPGA_MASTER書込み		*/
	E_FDL_FLSH_FPGAS ,									/* FPGA_SLAVE書込み			*/
	E_FDL_FLSH_RFIC_BOOT ,								/* RFIC Boot書込み			*/
	E_FDL_FLSH_RFIC_ARM ,								/* RFIC ARM書込み			*/
	E_FDL_FLSH_PTP_PARAM,								/* PTP パラメータ書込み		*/
	E_FDL_FLSH_FILEMAX
};

#define E_FDL_SLOT_NUM 2
#define E_FDL_LOCAL_PATH "/tmp"
#define E_FDL_VENDOR_CODE "FJ"

#endif
/* @} */


