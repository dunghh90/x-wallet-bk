/*!
 * @skip   $Id$
 * @file   M_dl_ram.c
 * @brief  DLM read/write table definition.
 * @date   2008/07/30 FFCS)Wuh Create for eNB-008-001.
 * @date   2008/08/05 FFCS)Wuh Divide the data save table for two tables of 3G and S3G.
 * @date   2009/03/05 FFCS)Wuh modify for M-S3G-eNBPF-01471
 *                          stop writing FLASH after receiving CPRI message
 * @date  2009/04/08  FFCS)Wuh
 *                      add a new BTI setting flag to judge whether need do BTI setting
 * @date  2009/04/13  FFCS)Wuh modify for S3G PRC CR-0038-005 (CPRI spec V1.07)
 * @date  2009/07/20  FFCS)Tangj modify for S3G PRC 7th release function system parameter mechanism change
 *                   delete dlw_sysbtiset_flg
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2009
*/

/*! 
 * @addtogroup RRH_L3_DLM
 * @{ 
 */


/****************************************************************************/
/*		define(s)/include(s)												*/
/****************************************************************************/
#include "m_cm_header.h"			/* MDIF共通ヘッダファイル				*/
#include "m_dl_header.h"			/* RE監視タスクヘッダファイル			*/

/****************************************************************************/
/* NAME		: ダウンロード管理RAMテーブル宣言ファイル( m_dl_ram.c )			*/
/*																			*/
/* FUNCTION	: ダウンロード管理タスクで使用するRAMテーブル宣言				*/
/*																			*/
/* ITEM No	: MB01-002-004-811					ファイル転送				*/
/*																			*/
/* DATE		: 2005/12/12	α)A.Etou			新規作成					*/
/*																			*/
/* UPDATE 	: 2006/03/03	α)A.Etou			WCDMA119-0225対応			*/
/*			  2007/05/07	FJT)T.Yanagisawa	CR-07A-0079対応				*/
/*								 次期OF-TRX用REファイル対応について			*/
/*			  2008/05/16	FJT)T.Yanagisawa	CR08A-0008対応				*/
/*								 800M-VA/BD/LSI対応							*/
/*																			*/
/*			All Rights Reserved, Copyright (C) FUJITSU LIMITED 2005-2008	*/
/****************************************************************************/

/********************************************************************************************************************/
/**
 *  @brief    declare file for DLM table 
 *  @note     declare file for DLM table 
 *  @param    None		
 *  @return   None
 *  @retval   -
 *  @date     2008/07/30 FFCS)Wuh create
 *  @date   2008/08/05 FFCS)Wuh Divide the data save table for two tables of 3G and S3G.
 *  @date   2009/03/05 FFCS)Wuh modify for M-S3G-eNBPF-01471
 *                          stop writing FLASH after receiving CPRI message
 *  @date  2009/04/08  FFCS)Wuh
 *                      add a new BTI setting flag to judge whether need do BTI setting
 *  @date  2009/04/13  FFCS)Wuh modify for S3G PRC CR-0038-005 (CPRI spec V1.07)
 *  @date  2009/07/20  FFCS)Tangj modify for S3G PRC 7th release function system parameter mechanism change
 *                   delete dlw_sysbtiset_flg
 */
/********************************************************************************************************************/


/****************************************************************************/
/* タスク状態管理テーブル													*/
/****************************************************************************/
/*!
 * @name task state management table
 */
USHORT					dlw_tsksta_tbl;

/****************************************************************************/
/* ダウンロード管理管理型													*/
/****************************************************************************/
/*!
 * @name download management table
 */

DLT_DLMNG_TBL			dlw_dlmng_tbl[CMD_MAX_SYSNUM];      /* for 3G and S3G */

/****************************************************************************/
/* タイマ管理テーブル														*/
/****************************************************************************/
/*!
 * @name timer management table
 */
UINT					dlw_timmng_tbl[CMD_MAX_SYSNUM];     /* for 3G and S3G */
UINT					dlw_timrf_tbl;		/* the timer for RF type */

/****************************************************************************/
/* データ格納テーブル														*/
/****************************************************************************/
/*!
 * @name data save table
 */
UCHAR                   dlw_rcvsyspa_tbl_3g[DLD_SYSPAR3G_SIZE];
UCHAR                   dlw_rcvsyspa_tbl_s3g[DLD_SYSPARS3G_SIZE];


/****************************************************************************/
/* データ展開テーブル														*/
/***************************************************l*************************/


/****************************************************************************/
/* old primary system kind table									*/
/****************************************************************************/
/*!
 * @name old primary system kind table
 */
 /* @{ */
USHORT                  dlw_prisys_tbl;
/* @} */

/*!
 * @name systemparameter collect complete flag
 */
 /* @{ */
USHORT					dlw_syscolcmp_flg;
/* @} */

/*!
 * @name the first download flag
 */
 /* @{ */
UINT					dlw_1stdlm_flg;
/* @} */

/*!
 * @name abnormal data sending flag
 */
 /* @{ */
USHORT					dlw_abdata_flg[CMD_MAX_SYSNUM];
/* @} */

/*!
 * @name the currently bandwidth
 */
 /* @{ */
USHORT					dlw_current_band;
/* @} */

/*!
 * @name download type table
 */
/* @{ */
USHORT					dlw_downtyp_tbl;
/* @} */

/* @} */

/* @FLASH erase&write state */
/* @{ */
DLT_RUNHIS_TBL	dlw_runhis_tbl ;
/* @} */

/** 
 * [Name]       DLM管理テーブル
 *
 * [値の説明]
 
 * [用途]	
 *
 * [注意事項]
 *
 * [値の設定タイミング]
 * 
 *
 */
DLT_DLM_TBL	gt_dlw_dlm_tbl;

/** 
 * [Name]       対REC情報テーブル
 *
 * [値の説明]
 
 * [用途]	
 *
 * [注意事項]
 *
 * [値の設定タイミング]
 * 
 *
 */
DLT_REC_TBL			gt_dlw_rec_tbl;

/** 
 * [Name]       対REC情報テーブル(3G)
 *
 * [値の説明]
 
 * [用途]	
 *
 * [注意事項]
 *
 * [値の設定タイミング]
 * 
 *
 */
DLT_REC_TBL			gt_dlw_rec_3g_tbl;

/**
 * [Name]       対RE-FW情報テーブル
 *
 * [値の説明]	[0～15]:[RE#1～16]
 
 * [用途]	
 *
 * [注意事項]
 *
 * [値の設定タイミング]
 * 
 *
 */
DLT_REFW_TBL	gt_dlw_refw_tbl[D_RRH_CPRINO_RE_MAX];

/**
* [Name]       対RE-FW情報テーブル(3G)
 *
 * [値の説明]	[0～15]:[RE#1～16]
 
 * [用途]	
 *
 * [注意事項]
 *
 * [値の設定タイミング]
 * 
 *
 */
DLT_REFW_TBL	gt_dlw_refw_3g_tbl[D_RRH_CPRINO_RE_MAX];

/** 
 * [Name]       FHM保持REファイル情報
 *
 * [値の説明]	[0～15]:[RE#1～16]
 
 * [用途]	
 *
 * [注意事項]
 *
 * [値の設定タイミング]
 * 
 *
 */
DLT_MTN_REINF	gt_dlw_mnt_reinf[D_RRH_CPRINO_RE_MAX + CMD_NUM1];

/** 
* [Name]       FHM保持REファイル情報(3G)
 *
 * [値の説明]	[0～15]:[RE#1～16]
 
 * [用途]	
 *
 * [注意事項]
 *
 * [値の設定タイミング]
 * 
 *
 */
DLT_MTN_REINF	gt_dlw_mnt_3g_reinf[D_RRH_CPRINO_RE_MAX + CMD_NUM1];



/* 対REC情報テーブル */
DLT_REC_TBL* 	gt_dlw_rec_all_tbl[CMD_MAX_SYSNUM] = {	&gt_dlw_rec_3g_tbl,
														&gt_dlw_rec_tbl
												  	 };

/* 対RE-FW情報テーブル */
DLT_REFW_TBL*	gt_dlw_refw_all_tbl[CMD_MAX_SYSNUM] ={	&gt_dlw_refw_3g_tbl[CMD_NUM0],
														&gt_dlw_refw_tbl[CMD_NUM0]
													 };

/* FHM保持REファイル情報 */
DLT_MTN_REINF* gt_dlw_mnt_all_reinf[CMD_MAX_SYSNUM] ={	&gt_dlw_mnt_3g_reinf[CMD_NUM0],
														&gt_dlw_mnt_reinf[CMD_NUM0]
													 };

EC_DLM_MAIN_EVT	ge_dlw_main_event[CMD_MAX_SYSNUM];
USHORT 			gw_dl_FileType;