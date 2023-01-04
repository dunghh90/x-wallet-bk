/*!
 * @skip   $Id$
 * @file   M_lg_ram.c
 * @brief  LOG read/write table definition.
 * @date   2008/07/30 FFCS)Wuh Create for eNB-009-004.
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
*/

/*!
 * @addtogroup TRA_LOG
 * @{
 */

#include "m_cm_header.h"
#include "m_lg_header.h"

/****************************************************************************/
/*	m_lg_ram.c	【RAMテーブル宣言】											*/
/*																			*/
/* ITEM No	: MB01-002-007-801		保守運用改善(診断、RAS、障害ログ)		*/
/*																			*/
/* DATE		: 2005/12/13	α)Y.Shimasaki			新規作成				*/
/*																			*/
/* UPDATE 	: 2006/02/22	α)Y.Shimasaki	WCDMA119-0470対応				*/
/*												障害ログ3面構造に伴う修正	*/
/*																			*/
/*			: 2006/04/18	α)Y.Shimasaki	WCDMA119-1494対応				*/
/*			: 2006/07/19	FJT)O.Yamamoto	MB01-002-007-801				*/
/*										運用ログ取得変更に伴うテーブル変更	*/
/*			: 2011/04/22	FJT)Koshida		M-S3G-eNBPF-04038				*/
/*						運用中ダウンロードと障害ログの競合でTRAリセット発生	*/
/*																			*/
/*			All Rights Reserved, Copyright (C) FUJITSU LIMITED 2005-2011	*/
/****************************************************************************/
/* 障害ログ管理テーブル(RAM)									*/
/*!
 * @name trouble log management table
 */
/* @{ */
LGT_ALMLOGESC_T	lgw_almlogmng_tbl[CMD_MAX_SYSNUM] ;
/* @} */

/* 応答送信先管理テーブル  	       */
/*!
 * @name Response management table
 */
/* @{ */
LGT_RSPMNG_T	lgw_rspmng_tbl[CMD_MAX_SYSNUM][LGD_L3_MESSAGE_MAX];     /*-----for 3G and S3G-----*/
/* @} */

/**
 * @brief RE障害ログ送信確認タイマ開始済確認フラグ
 * @note  [ 値の説明 ] ON/OFF
 * @note  [ 用途     ] RE障害ログ送信確認タイマが開始した状態を表示する
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ] タイマ起動時 = ON, タイマ停止時 = OFF, タイムアウト時 = OFF
 */
USHORT gw_lgw_RETroublelogSendTimerStartFlg = D_RRH_OFF;

/**
 * @brief RE障害ログ取得状態
 * @note  [ 値の説明 ] 未処理、取得要求中、配下REからのファイルデータ送信中、配下REからのファイルデータ送信タイムアウト、完了
 * @note  [ 用途     ] RE障害ログ取得状態を保持する
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
USHORT gw_lgw_REGetLogSta_tbl[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX];

/**
 * @brief RE障害ログ取得応答保持
 * @note  [ 値の説明 ] 応答結果
 * @note  [ 用途     ] 各対象配下REのRE障害ログ取得応答を保持する
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
LGT_REOBSLOGRESMNG_T gw_lgw_REobLogGetRes_tbl[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX];

/**
 * @brief RE障害ログ取得中止要求の再送回数
 * @note  [ 値の説明 ] 
 * @note  [ 用途     ] RE障害ログ取得中止要求の再送回数を保持する
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ] 初送時に初期化、応答待ちタイムアウト時にカウントアップ、再送回数最大値を超えたらRECへ応答を返す
 */
USHORT gw_lgw_REGetLogStopReqCnt = 0;


/**
 * @brief ファイルデータ送信中の配下RE数
 * @note  [ 値の説明 ] ファイルデータ送信中の配下RE数
 * @note  [ 用途     ] ファイルデータ送信中の配下RE数を保持する
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
USHORT gw_lgw_FilSndNum[CMD_MAX_SYSNUM];

/**
 * @brief 総受信ログサイズ
 * @note  [ 値の説明 ] 総受信ログサイズ
 * @note  [ 用途     ] 配下REから受信したデータ送信のデータサイズを保持する
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT gw_lgw_LogFilRcvSize[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX];

/* @} */

