/*!
 * @skip  $ld:$
 * @file  m_lg_def.h
 * @brief Constant definition.
 * @name  Constant definition.
 * @date  2008/07/30 FFCS)Wuh Create
 *	All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008
 */

#ifndef M_LG_DEF_HEAD
#define M_LG_DEF_HEAD

/*!
 * @name l3 messageの応答送信先管理テーブル用
 * @{
 */
enum{
    LGD_L3_TROUBLELOG_GET_RSP = 0,          /**< RE障害ログ取得応答         */
    LGD_L3_FILEDATA_SEND,                   /**< ファイルデータ送信         */
    LGD_L3_FILEDATA_FINISH,                 /**< ファイルデータ送信完了通知 */
    LGD_L3_TROUBLELOG_STOP,                 /**< RE障害ログ取得中止         */
    LGD_L3_MESSAGE_MAX
};
/* @} */

/*!
 * @name using number define
 * @{
 */
/* 汎用デファイン */
#define LGD_WAIT_TIME_270       270         /**< Wait時間(270ms) */


#define LGD_MAX_FILNAME_NUM     256         /**< ファイル名最大文字数 */
#define LGD_MAX_COMPDAT         1048576     /**< 圧縮データ格納最大領域(1M) */
#define LGD_MAX_FHMRECOMPDAT    1048576*17  /**< 圧縮データ格納最大領域(1M) */
#define LGD_SIZE_INI            0           /**< サイズ初期化デファイン */
#define LGD_NUM_INI             0           /**< 送信数/完了数初期化デファイン */
#define LGD_PARM_INI            0           /**< パラメータ初期化デファイン */
#define	LGD_RETRY_STOP_REQ_NUM_MAX	3		/**< RE障害ログ取得中止要求の再送回数最大値 */

/*!
 * @name using string define
 * @{
 */
/* 汎用デファイン */
#define LGD_FHM_TROUBLE_LOG_NAME (UCHAR*)"/var/log/sendLog2Rec/FHM.IEF"	/* FHM障害ログのファイル名 (フルパス) */
#define LGD_REFHM_TROUBLE_LOG_FOLDER_PATH (UCHAR*)"/var/log/sendLog2Rec"	/* REとFHMの障害ログを保存するフォルダ (フルパス) */
/* @} */

/*!
 * @name RE障害ログ取得状態
 * @{
 */
/* RE障害ログ取得状態 */
#define LGD_GETRESTS_NOTPROC        0       /**< 未処理 */
#define LGD_GETRESTS_REQUESTING     1       /**< 取得要求中 */
#define LGD_GETRESTS_FILSENDING     2       /**< 配下REからのファイルデータ送信中 */
#define LGD_GETRESTS_COMP           3       /**< 配下REからのファイルデータ送信完了 */
#define LGD_STOPRESTS_REQUESTING    4       /**< 配下REへ障害ログ中止要求中 */
#define LGD_GETRESTS_MAXNUM         5       /**< RE障害ログ取得状態最大数*/
/* @} */

/*!
 * @name RE障害ログ取得段階状態
 * @{
 */
/* RE障害ログ取得段階状態 */
#define LGD_GETFHMSTS_USE               0   /**< 通常運用状態 */
#define LGD_GETFHMSTS_REPROCESSING      1   /**< 配下RE障害ログ取得中状態 */
#define LGD_GETFHMSTS_ACHFILCREATING    2   /**< ログファイル作成中状態 */
#define LGD_GETFHMSTS_RECTRANSFERING    3   /**< ログ送信中状態 */
#define LGD_GETFHMSTS_STOPPROCESSING    4   /**< RE障害ログ取得中止中状態 */
#define LGD_GETFHMSTS_MAXNUM            5   /**< RE障害ログ取得状態最大数 */
/* @} */

/*!
 * @name event
 * @{
 */
/* 発生事象(EVENT)		*/
#define LGD_EVT_RECOBSLOGREQ        0       /**< RECからのRE障害ログ取得要求 */
#define LGD_EVT_REOBSLOGRES         1       /**< 配下REからのRE障害ログ取得応答 */
#define LGD_EVT_REGETLOGCFTO        2       /**< 配下REのRE障害ログ取得確認タイマタイムアウト */
#define LGD_EVT_REFILSNDREQ         3       /**< 配下REからのファイルデータ送信要求 */
#define LGD_EVT_REFILSNDFINNTC      4       /**< 配下REからのファイルデータ送信完了報告通知 */
#define LGD_EVT_TROUBLELOGSNDCFTO   5       /**< 配下REのRE障害ログ送信確認タイマタイムアウト */
#define LGD_EVT_REOBSLOGCREFINNTC   6       /**< 障害ログファイル作成完了通知 */
#define LGD_EVT_RECFILSNDTO         7       /**< RECへファイルデータ転送為の障害ログ転送間隔タイマタイムアウト */
#define LGD_EVT_RECGETLOGSTPREQ     8       /**< RECからのRE障害ログ取得中止要求 */
#define LGD_EVT_REGETLOGSTPRES      9       /**< 配下REからのRE障害ログ取得中止応答 */
#define LGD_EVT_LOGGETSTPCFTO       10      /**< 配下REのRE障害ログ取得中止確認タイマタイムアウト */
#define LGD_EVT_CPRIDWNNTC			11		/**< CPRIリンク断通知 */
#define LGD_EVT_EXCEPTION           12      /**< イベント番号設定 */
#define LGD_EVT_MAXNUM              13      /**< イベント最大数 */
/* @} */

#endif /* M_LG_DEF_HEAD */

/* @} */
