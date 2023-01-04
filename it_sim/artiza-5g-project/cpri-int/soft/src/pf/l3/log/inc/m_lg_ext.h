/*!
 * @skip  $ld:$
 * @file  M_lg_ext.h
 * @brief Data Colloction external declaration.
 * @date  2008/07/30 FFCS)Wuh Create
 * @date  2011/04/22 FJT)Koshida modify for M-S3G-eNBPF-04038
 *	All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008-2011
 */
#ifndef M_LG_EXT_HEAD
#define M_LG_EXT_HEAD
/****************************************************************************/
/* テーブル宣言																*/
/****************************************************************************/
/* 障害ログ管理テーブル(RAM)						*/
/*! 
 * @name trouble log management table
 */
extern	LGT_ALMLOGESC_T		lgw_almlogmng_tbl[CMD_MAX_SYSNUM] ;

/* 障害ログジャンプテーブル							*/
/*! 
 * @name trouble log jump table
 */
extern	VOID (* const lgr_tskjump_tbl[LGD_GETFHMSTS_MAXNUM][LGD_EVT_MAXNUM])
																( UCHAR*, USHORT ) ;

/* 応答送信先管理テーブル  	       */
/*!
 * @name Response management table
 */
extern	LGT_RSPMNG_T	lgw_rspmng_tbl[CMD_MAX_SYSNUM][LGD_L3_MESSAGE_MAX];     /*-----for 3G and S3G-----*/

/* RE障害ログ送信確認タイマ開始フラグ        */
/*!
 * @name Re Trouble Log Send Confirm Timer Start flag
 */
extern USHORT                   gw_lgw_RETroublelogSendTimerStartFlg;

/* RE障害ログ取得状態管理テーブル  	       */
/*!
 * @name RE log get status management table
 */
extern	USHORT			gw_lgw_REGetLogSta_tbl[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX];     /*-----for 3G and S3G-----*/

/* RE障害ログ取得応答結果  	       */
/*!
 * @name Trouble log get response result management table
 */
extern	LGT_REOBSLOGRESMNG_T	gw_lgw_REobLogGetRes_tbl[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX];     /*-----for 3G and S3G-----*/

/* RE障害ログ取得中止要求の再送回数 	       */
/*!
 * @name The number of retransmissions of RE log get stop request
 */
extern	USHORT gw_lgw_REGetLogStopReqCnt;	/*-----for S3G-----*/

/* ファイルデータ送信中の配下RE数  	       */
/*!
 * @name The number of RE that is sending file
 */
extern	USHORT			gw_lgw_FilSndNum[CMD_MAX_SYSNUM];					     /*-----for 3G and S3G-----*/

/* 総受信ログサイズテーブル	       */
/*!
 * @name File's size received  management table
 */
extern	UINT			gw_lgw_LogFilRcvSize[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX];     /*-----for 3G and S3G-----*/

/*!
 * @name REs obslog file name.
 */
extern	UCHAR			obslog_filename_tbl[D_RRH_CPRINO_RE_MAX][LGD_MAX_FILNAME_NUM];

#endif /* M_LG_EXT_HEAD */
