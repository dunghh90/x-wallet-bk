/*!
 * @skip   $Id$
 * @file   M_lg_rom.c
 * @brief  LOG read only table definition.
 * @date   2008/07/15 FFCS)Wuh Create for eNB-008-001.
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
*/

/*
 * @addtogroup TRA_LOG
 * @{ 
*/

#include "m_cm_header.h"
#include "m_lg_header.h"

/****************************************************************************/
/*    m_lg_rom.c    【ROMテーブル宣言】                                      */
/*                                                                           */
/* ITEM No    : MB01-002-007-801        保守運用改善(診断、RAS、障害ログ)    */
/*                                                                           */
/* DATE        : 2005/12/13    α)Y.Shimasaki            新規作成            */
/*                                                                           */
/* UPDATE      : 2015/11/09    TDIPS)Takeuchi            技説QA120           */
/*                                                                           */
/*            All Rights Reserved, Copyright (C) FUJITSU LIMITED 2005-2006   */
/****************************************************************************/
/* 関数ジャンプテーブル */

/*!
 * @name Message jump table of LOG thread.
 */
VOID (* const lgr_tskjump_tbl[LGD_GETFHMSTS_MAXNUM][LGD_EVT_MAXNUM])
                                                    ( UCHAR*, USHORT ) =
{
    {
        /* 通常運用状態 */
        m_lg_Idle_GetTroublelog   , /**< RECからのRE障害ログ取得要求*/
        m_lg_nop                  , /**< 配下REからのRE障害ログ取得応答*/
        m_lg_nop                  , /**< 配下REのRE障害ログ取得確認タイマタイムアウト*/
        m_lg_nop                  , /**< 配下REからのファイルデータ送信要求*/
        m_lg_nop                  , /**< 配下REからのファイルデータ送信完了報告通知*/
        m_lg_nop                  , /**< 配下REのRE障害ログ送信確認タイマタイムアウト*/
        m_lg_nop                  , /**< 障害ログファイル作成完了通知*/
        m_lg_nop                  , /**< RECへファイルデータ転送為の障害ログ転送間隔タイマタイムアウト*/
        m_lg_Idle_StopTroublelog  , /**< RECからのRE障害ログ取得中止要求    */
        m_lg_nop                  , /**< 配下REからのRE障害ログ取得中止応答*/
        m_lg_nop                  , /**< 配下REのRE障害ログ取得中止確認タイマタイムアウト*/
		m_lg_nop                  , /**< CPRIリンク断通知 */
        m_lg_nop                    /**< イベント番号設定   */
    },
    {
        /* 配下RE障害ログ取得中状態 */
        m_lg_Wait_GetTroublelog     , /**< RECからのRE障害ログ取得要求*/
        m_lg_Wait_TroublelogGetRes  , /**< 配下REからのRE障害ログ取得応答*/
        m_lg_Wait_TroublelogGetCfTo , /**< 配下REのRE障害ログ取得確認タイマタイムアウト*/
        m_lg_Wait_FilSndReq         , /**< 配下REからのファイルデータ送信要求*/
        m_lg_Wait_FilSndFinNtc      , /**< 配下REからのファイルデータ送信完了報告通知*/
        m_lg_Wait_TroublelogSndCfTo , /**< 配下REのRE障害ログ送信確認タイマタイムアウト*/
        m_lg_nop                    , /**< 障害ログファイル作成完了通知*/
        m_lg_nop                    , /**< RECへファイルデータ転送為の障害ログ転送間隔タイマタイムアウト*/
        m_lg_Wait_StopTroublelog    , /**< RECからのRE障害ログ取得中止要求*/
        m_lg_nop                    , /**< 配下REからのRE障害ログ取得中止応答*/
        m_lg_nop                    , /**< 配下REのRE障害ログ取得中止確認タイマタイムアウト*/
		m_lg_Wait_CpriLinkDwnNtc    , /**< CPRIリンク断通知 */
        m_lg_nop                     /**< イベント番号設定   */
    },
    {
        /* FHMログファイル作成中状態 */
        m_lg_Wait_GetTroublelog   , /**< RECからのRE障害ログ取得要求*/
        m_lg_nop                  , /**< 配下REからのRE障害ログ取得応答*/
        m_lg_nop                  , /**< 配下REのRE障害ログ取得確認タイマタイムアウト*/
        m_lg_nop                  , /**< 配下REからのファイルデータ送信要求*/
        m_lg_nop                  , /**< 配下REからのファイルデータ送信完了報告通知*/
        m_lg_nop                  , /**< 配下REのRE障害ログ送信確認タイマタイムアウト*/
        m_lg_Wait_NtcTroublelog   , /**< 障害ログファイル作成完了通知*/
        m_lg_nop                  , /**< RECへファイルデータ転送為の障害ログ転送間隔タイマタイムアウト*/
        m_lg_Wait_StopTroublelog  , /**< RECからのRE障害ログ取得中止要求*/
        m_lg_nop                  , /**< 配下REからのRE障害ログ取得中止応答*/
        m_lg_nop                  , /**< 配下REのRE障害ログ取得中止確認タイマタイムアウト*/
		m_lg_nop                  , /**< CPRIリンク断通知 */
        m_lg_nop                    /**< イベント番号設定   */
    },
    {
        /* ログ送信中状態 */
        m_lg_Send_GetTroublelog    , /**< RECからのRE障害ログ取得要求*/
        m_lg_nop                   , /**< 配下REからのRE障害ログ取得応答*/
        m_lg_nop                   , /**< 配下REのRE障害ログ取得確認タイマタイムアウト*/
        m_lg_nop                   , /**< 配下REからのファイルデータ送信要求*/
        m_lg_nop                   , /**< 配下REからのファイルデータ送信完了報告通知*/
        m_lg_nop                   , /**< 配下REのRE障害ログ送信確認タイマタイムアウト*/
        m_lg_nop                   , /**< 障害ログファイル作成完了通知*/
        m_lg_Send_ToTimer          , /**< RECへファイルデータ転送為の障害ログ転送間隔タイマタイムアウト*/
        m_lg_Send_StopTroublelog   , /**< RECからのRE障害ログ取得中止要求    */
        m_lg_nop                   , /**< 配下REからのRE障害ログ取得中止応答*/
        m_lg_nop                   , /**< 配下REのRE障害ログ取得中止確認タイマタイムアウト*/
		m_lg_nop                   , /**< CPRIリンク断通知 */
        m_lg_nop                     /**< イベント番号設定   */
    },
    {
        /* RE障害ログ取得中止中状態 */
        m_lg_Idle_GetTroublelog     , /**< RECからのRE障害ログ取得要求*/
        m_lg_nop                    , /**< 配下REからのRE障害ログ取得応答*/
        m_lg_nop                    , /**< 配下REのRE障害ログ取得確認タイマタイムアウト*/
        m_lg_nop                    , /**< 配下REからのファイルデータ送信要求*/
        m_lg_nop                    , /**< 配下REからのファイルデータ送信完了報告通知*/
        m_lg_nop                    , /**< 配下REのRE障害ログ送信確認タイマタイムアウト*/
        m_lg_nop                    , /**< 障害ログファイル作成完了通知*/
        m_lg_nop                    , /**< RECへファイルデータ転送為の障害ログ転送間隔タイマタイムアウト*/
        m_lg_Wait_StopTroublelog    , /**< RECからのRE障害ログ取得中止要求    */
        m_lg_Stop_StopTroublelogRes , /**< 配下REからのRE障害ログ取得中止応答*/
        m_lg_Stop_StopTroublelogCfTo, /**< 配下REのRE障害ログ取得中止確認タイマタイムアウト*/
		m_lg_nop                    , /**< CPRIリンク断通知 */
        m_lg_nop                      /**< イベント番号設定   */
    }
};

/*!
 * @name REs obslog file name.
 */
UCHAR obslog_filename_tbl[D_RRH_CPRINO_RE_MAX][LGD_MAX_FILNAME_NUM] = {
	"RE001.IEF",
	"RE002.IEF",
	"RE003.IEF",
	"RE004.IEF",
	"RE005.IEF",
	"RE006.IEF",
	"RE007.IEF",
	"RE008.IEF",
	"RE009.IEF",
	"RE010.IEF",
	"RE011.IEF",
	"RE012.IEF",
	"RE013.IEF",
	"RE014.IEF",
	"RE015.IEF",
	"RE016.IEF",
};


/* @} */


