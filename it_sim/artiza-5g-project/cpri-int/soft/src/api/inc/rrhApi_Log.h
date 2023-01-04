/*!
 * @skip  $ld:$
 * @file  rrhApi_Log.h
 * @brief API LOG INC定義
 * @date  2013/11/20 ALPHA)yokoyama Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_API_LOG
 * @{
 */
#ifndef RRHAPI_LOG_H
#define RRHAPI_LOG_H

#include "rrhApi_Com.h"

/********************************************************************************************************************/
/* API LOG 構造体定義                                                                                               */
/********************************************************************************************************************/
/********************************************************************************************************************/
/*!
 * @name (0xA0020001)障害ログファイル作成要求
 * @note (0xA0020001)障害ログファイル作成要求定義
 * @{
 */
/********************************************************************************************************************/
/**
 * @brief   障害ログファイル作成要求情報
 * @note    for T_API_LOG_TROUBLELOG_GET
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	UCHAR								filename[256];			/* 出力ファイル名									*/
	UINT								size;					/* 出力ファイルMAXサイズ							*/
}T_API_LOG_TROUBLELOG_GET_INFO;

/**
 * @brief   障害ログファイル作成要求
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	T_RRH_HEAD							head;					/**< 装置共通ヘッダ									*/
	T_API_LOG_TROUBLELOG_GET_INFO		info;					/**< 障害ログファイル作成要求情報					*/
}T_API_LOG_TROUBLELOG_GET;
/* @} */


/********************************************************************************************************************/
/*!
 * @name (0xA0020002)障害ログファイル作成完了通知
 * @note (0xA0020002)障害ログファイル作成完了通知定義
 * @{
 */
/********************************************************************************************************************/
/**
 * @brief   障害ログファイル作成完了通知情報
 * @note    for T_API_LOG_TROUBLELOG_GET_RSP
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	UINT								okng;					/**< OKNG											*/
	UCHAR								filename[256];			/**< 出力ファイル名									*/
	UINT								size;					/**< 出力ファイルサイズ								*/
	UINT								sysKind;				/**<L システム種別(3G/S3G)							*/
}T_API_LOG_TROUBLELOG_GET_RSP_INFO;

/**
 * @brief   障害ログファイル作成完了通知
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	T_RRH_HEAD							head;					/**< 装置共通ヘッダ									*/
	T_API_LOG_TROUBLELOG_GET_RSP_INFO	info;					/**< 障害ログファイル作成完了通知情報				*/
}T_API_LOG_TROUBLELOG_GET_RSP;
/* @} */

typedef struct{
	UCHAR								filename[256];			/* 出力ファイル名									*/
	UINT								size;					/* 出力ファイルMAXサイズ							*/
	UINT								m_kind;					/* 装置構成情報(装置構成がRRECSCD親REの場合有効)	*/
}T_API_LGC_TROUBLELOG_GET_INFO;

typedef struct{
	T_RRH_HEAD							head;					/**< 装置共通ヘッダ									*/
	T_API_LGC_TROUBLELOG_GET_INFO		info;					/**< 障害ログファイル作成要求情報					*/
}T_API_LGC_TROUBLELOG_GET;
/* @} */


/********************************************************************************************************************/
/*!
 * @name (0xA0020002)障害ログファイル作成完了通知
 * @note (0xA0020002)障害ログファイル作成完了通知定義
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	UINT								okng;					/* OKNG												*/
	UCHAR								filename[256];			/* 出力ファイル名									*/
	UINT								size;					/* 出力ファイルサイズ								*/
	UINT								sysKind;				/* システム種別(SUB6/MMW)								*/
}T_API_LGC_TROUBLELOG_GET_RSP_INFO;

typedef struct{
	T_RRH_HEAD							head;					/**< 装置共通ヘッダ									*/
	T_API_LGC_TROUBLELOG_GET_RSP_INFO	info;					/**< 障害ログファイル作成完了通知情報				*/
}T_API_LGC_TROUBLELOG_GET_RSP;
/* @} */

/********************************************************************************************************************/
/*!
 * @name (0xA0020003)障害ログファイル作成中止
 * @note (0xA0020003)障害ログファイル作成中止定義
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	T_RRH_HEAD							head;					/**< 装置共通ヘッダ									*/
}T_API_LOG_TROUBLELOG_GET_STOP;
/* @} */


/********************************************************************************************************************/
/*!
 * @name (0xA0020005)障害ログファイル削除
 * @note (0xA0020005)障害ログファイル削除定義
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	UCHAR								filename[256];			/* 削除ファイル名									*/
}T_API_LOG_TROUBLELOG_DEL_INFO;

typedef struct{
	T_RRH_HEAD							head;					/**< 装置共通ヘッダ									*/
    T_API_LOG_TROUBLELOG_DEL_INFO		info;					/**< 障害ログファイル削除情報						*/
}T_API_LOG_TROUBLELOG_DEL;

/* CascadeRRE対応 */
/********************************************************************************************************************/
/*!
 * @name (0xA0020007)子障害ログファイル完了通知
 * @note (0xA0020007)障害ログファイル完了通知定義
 * @{
 */
/********************************************************************************************************************/
typedef struct{
	T_RRH_HEAD							head;					/**< 装置共通ヘッダ									*/
}T_API_LOG_CSCDTROUBLELOG_END;



/********************************************************************************************************************/
/* プロトタイプ宣言                                                                                                 */
/********************************************************************************************************************/
/* 2020/12/28 M&C) Merge 4G FHM src (start modify - remove param 'm_kind') */
// E_RRHAPI_RCODE rrhApi_Log_Mnr_TroubleLogGet( INT qid, INT wtime, VOID *data_p, UCHAR *filename, UINT size, UINT m_kind );
E_RRHAPI_RCODE rrhApi_Log_Mnr_TroubleLogGet( INT qid, INT wtime, VOID *data_p, UCHAR *filename, UINT size   );
/* 2020/12/28 M&C) Merge 4G FHM src (end modify - remove param 'm_kind') */

E_RRHAPI_RCODE rrhApi_Log_Mnr_TroubleLogDelete( INT qid, INT wtime, VOID *data_p, UCHAR *filename );
E_RRHAPI_RCODE rrhApi_Log_Mnt_TroubleLogGetStop( INT qid, INT wtime, VOID *data_p );
E_RRHAPI_RCODE rrhApi_Log_Mnr_TroubleLogGet_MMW( INT qid, INT wtime, VOID *data_p, UCHAR *filename, UINT size );
E_RRHAPI_RCODE rrhApi_Log_Mnr_TroubleLogDelete_MMW( INT qid, INT wtime, VOID *data_p, UCHAR *filename );
E_RRHAPI_RCODE rrhApi_Log_Mnt_TroubleLogGetStop_MMW( INT qid, INT wtime, VOID *data_p );
/* 2020/12/28 M&C) Merge 4G FHM src (start modify - remove param 'm_kind') */
// E_RRHAPI_RCODE rrhApi_Log_TroubleLogGet_com( INT qid, INT wtime, VOID *data_p, UCHAR *filename, UINT size, UINT m_kind, UINT sysKind );
E_RRHAPI_RCODE rrhApi_Log_TroubleLogGet_com( INT qid, INT wtime, VOID *data_p, UCHAR *filename, UINT size, UINT   sysKind );
/* 2020/12/28 M&C) Merge 4G FHM src (end modify - remove param 'm_kind') */
E_RRHAPI_RCODE rrhApi_Log_TroubleLogDelete_com( INT qid, INT wtime, VOID *data_p, UCHAR *filename, UINT sysKind );
E_RRHAPI_RCODE rrhApi_Log_TroubleLogGetStop_com( INT qid, INT wtime, VOID *data_p, UINT sysKind );
/* CascadeRRE対応 */
E_RRHAPI_RCODE rrhApi_Log_Mnr_cscdTroubleLogEnd( INT qid, INT wtime, VOID *data_p );
E_RRHAPI_RCODE rrhApi_Log_Mnr_cscdTroubleLogEnd_com( INT qid, INT wtime, VOID *data_p, UINT sysKind );

/********************************************************************************************************************/
/* API LOG EventID																									*/
/********************************************************************************************************************/
#define D_API_LOG_MNR_TROUBLE_LOG_GET_REQ			0xA0020001						/* 障害ログファイル作成要求		*/
#define D_API_LOG_MNR_TROUBLE_LOG_GET_RSP			0xA0020002						/* 障害ログファイル作成完了通知	*/
#define D_API_LOG_MNT_STOP_TROUBLE_LOG_GET			0xA0020003						/* 障害ログファイル作成中止		*/
#define D_API_LOG_MNR_TROUBLE_LOG_DEL				0xA0020005						/* 障害ログファイル削除			*/

#endif
/* @} */
