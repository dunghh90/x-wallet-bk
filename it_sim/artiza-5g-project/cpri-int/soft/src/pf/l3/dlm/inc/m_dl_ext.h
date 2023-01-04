/*!
 * @skip  $ld:$
 * @file  M_dl_ext.h
 * @brief Data Colloction external declaration.
 * @date  2008/07/30 FFCS)Wuh Create
 * @date  2009/03/05 FFCS)Wuh modify for M-S3G-eNBPF-01471
 *					    stop writing FLASH after receiving CPRI message
 * @date  2009/04/08 FFCS)Wuh
 *                      add a new BTI setting flag to judge whether need do BTI setting
 * @date  2009/04/13 FFCS)Wuh modify for S3G PRC CR-00038-005 (CPRI spec V1.07)
 * @date  2009/07/20 FFCS)Tangj modify for S3G PRC 7th release function system parameter mechanism change
 *                   delete dlw_sysbtiset_flg
 *
 *	All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008-2009
 */

/* 二重取込み防止 */
#ifndef		M_DL_EXT
#define		M_DL_EXT

/****************************************************************************/
/* タスク状態管理テーブル													*/
/****************************************************************************/
/*! 
 * @name task state management table
 */
extern	USHORT				dlw_tsksta_tbl;


/****************************************************************************/
/* ダウンロード管理テーブル													*/
/****************************************************************************/
/*! 
 * @name download management table
 */
extern	DLT_DLMNG_TBL		dlw_dlmng_tbl[CMD_MAX_SYSNUM];      /*-----for 3G and S3G-----*/

/****************************************************************************/
/* タイマ管理テーブル														*/
/****************************************************************************/
/*! 
 * @name timer management table
 */
extern	UINT		dlw_timmng_tbl[CMD_MAX_SYSNUM];     /*-----for 3G and S3G-----*/
extern  	UINT             dlw_timrf_tbl;		/* the timer for RF type */
/****************************************************************************/
/* データ格納テーブル														*/
/****************************************************************************/
/*! 
 * @name data save table
 */
extern 	UCHAR 		dlw_rcvsyspa_tbl_3g[DLD_SYSPAR3G_SIZE];
extern	UCHAR 		dlw_rcvsyspa_tbl_s3g[DLD_SYSPARS3G_SIZE];

/****************************************************************************/
/* データ展開テーブル														*/
/****************************************************************************/
/*! 
 * @name 3G and S3G systemparameter size table.
 */
extern	const	USHORT		dlr_syssize_settbl[CMD_NUM2];
/*! 
 * @name 3G and S3G systemparameter size table.
 */
extern	const	USHORT		dlr_makername_settbl[CMD_NUM4];

/****************************************************************************/
/* old primary system kind table									*/
/****************************************************************************/
/*! 
 * @name old primary system kind table
 */
extern  	USHORT			dlw_prisys_tbl;

/*! 
 * @name systemparameter collect complete flag
 */
extern	USHORT			dlw_syscolcmp_flg;

/*!
 * @name abnormal data sending flag
 */
extern	USHORT			dlw_abdata_flg[CMD_MAX_SYSNUM];
/* @} */

/*!
 * @name save the currently bandwidth
 */
extern	USHORT			dlw_current_band;
/* @} */

/*!
 * @name download type table
 */
/* @{ */
extern	USHORT			dlw_downtyp_tbl;
/* @} */

extern const DLT_SYSPRMCHK dlr_sysprmchk[DLD_SYSPRAD_NUM];

extern const DLT_SYSPRMCHK dlr_sysprmefchk[DLD_SYSPRAEF_NUM];


/*!
 * @name the first download flag
 */
 /* @{ */
extern UINT				dlw_1stdlm_flg;
/* @} */
/*!
 * @name download memory handle
 */
/* @{ */
//extern INT				dlw_handle;
/* @} */

/* @FLASH erase&write state */
/* @{ */
extern USHORT 				 dlw_flash_state;
/* @} */

/* @FLASH erase&write state */
/* @{ */
extern DLT_RUNHIS_TBL	dlw_runhis_tbl ;
/* @} */

/** 
*
* [Name]  実行中DL開始要因 
*
* [値の説明] 未実行/REC...
*
* [用途] Save 実行中DL開始要因
*
* [注意事項]
*
* [値の設定タイミング]
*
*                    立ち上げ 未実行
*
*/
extern USHORT	dlw_Dl_Start_Factor;

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
extern DLT_DLM_TBL			gt_dlw_dlm_tbl;

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
extern DLT_REC_TBL			gt_dlw_rec_tbl;
extern DLT_REC_TBL			gt_dlw_rec_3g_tbl;
extern DLT_REC_TBL*			gt_dlw_rec_all_tbl[CMD_MAX_SYSNUM];

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
extern DLT_MTN_REINF	gt_dlw_mnt_reinf[D_RRH_CPRINO_RE_MAX + CMD_NUM1];
extern DLT_MTN_REINF	gt_dlw_mnt_3g_reinf[D_RRH_CPRINO_RE_MAX + CMD_NUM1];
extern DLT_MTN_REINF*	gt_dlw_mnt_all_reinf[CMD_MAX_SYSNUM];

/** 
 * [Name]    対REC状態
 *
 * [値の説明]	
 *				0:アイドル
 *				1:ファイルデータ送信(FW)待ち
 *				2:運用中ファイルデータ送信(FW)待ち
 *				3:ファイルデータ転送先決定待ち
 *
 * [用途]	
 *
 * [注意事項]
 *
 * [値の設定タイミング]
 * 	0:以下に該当しない状態
 *	1:最初のファイルデータ送信受信(REC→FHM) ～ 
 *		ファイルデータ送信完了報告通知受信(REC→FHM)まで
 *	2:最初の運用中ファイルデータ送信受信(REC→FHM) ～
 *		運用中ファイルデータ送信完了報告通知受信(REC→FHM)まで
 *	3:ファイル情報報告/運用中ファイル情報報告応答送信(REC←FHM) ～ 
 *		最初のファイルデータ送信/運用中ファイルデータ送信受信(REC→FHM)まで
 *
 */
extern USHORT	gw_dlw_rec_sta;

/** 
 * [Name]    対RE状態
 *
 * [値の説明]	[0～15]	[RE#1～16]
 *				0	アイドル
 *				2	REファイルデータ読込中
 *				3	ファームウェアファイル報告応答待ち
 *				5	ファイル情報報告応答(FW)待ち
 *				6	ファイルデータ送信中(FW)
 *				7	ファイルデータ送信完了報告応答(FW)待ち
 *				8	運用中ファームウェアファイル報告応答待ち
 *				9	運用中ファイル情報報告応答(FW)待ち
 *				10	運用中ファイルデータ送信中(FW)
 *				11	運用中ファイルデータ送信完了報告応答(FW)待ち
 *				12	REファイル更新開始待ち
 *
 * [用途]	
 *
 * [注意事項]
 *
 * [値の設定タイミング]
 * 				0  以下に該当しない状態
 *				2  FHMが保持するREファイルの読込(共有メモリへの展開)処理中
 *				3  ファームウェアファイル報告要求送信(FHM→RE) ～ ファームウェアファイル報告応答受信(FHM←RE)まで	
 *				5  ファイル情報報告通知送信(FHM→RE) ～ ファイル情報報告応答受信(FHM←RE)まで	
 *				6  ファイル情報報告応答受信(FHM←RE) ～ ファイルデータ送信完了報告通知送信(FHM→RE)まで	
 *				7  ファイルデータ送信完了報告通知送信(FHM→RE) ～ ファイルデータ送信完了報告応答受信(FHM←RE)まで	
 *				8  運用中ファームウェアファイル報告要求送信(FHM→RE) ～ 運用中ファームウェアファイル報告応答受信(FHM←RE)まで	
 *				9  運用中ファイル情報報告通知送信(FHM→RE) ～ 運用中ファイル情報報告応答受信(FHM←RE)まで	
 *				10 運用中ファイル情報報告応答受信(FHM←RE) ～ 運用中ファイルデータ送信完了報告通知送信(FHM→RE)まで	
 *				11 運用中ファイルデータ送信完了報告通知送信(FHM→RE) ～ 運用中ファイルデータ送信完了報告応答受信(FHM←RE)まで	
 *				12 RECスレッドからのREファイル更新要求受信～ファームウェアファイル報告要求送信(FHM→RE)まで
 *
 */
extern USHORT	gw_dlw_re_sta[D_RRH_CPRINO_RE_MAX];

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
extern DLT_REFW_TBL		gt_dlw_refw_tbl[D_RRH_CPRINO_RE_MAX];
extern DLT_REFW_TBL		gt_dlw_refw_3g_tbl[D_RRH_CPRINO_RE_MAX];
extern DLT_REFW_TBL*	gt_dlw_refw_all_tbl[CMD_MAX_SYSNUM];

extern DLT_EVENT_CTBL    const dlr_event_ctbl[EC_DLM_MAIN_EVT_MAX];
extern VOID    (* const dlr_mtrxtbl[EC_DLM_MAIN_EVT_MAX][EC_DLM_MAIN_STN_MAX]) (VOID* bufp, USHORT cpri_no);
extern VOID    (* const dlr_fhm_mtrxtbl[EC_DLM_FHMDL_EVT_MAX][EC_DLM_FHMDL_STN_MAX]) (VOID* bufp, USHORT cpri_no);
extern VOID    (* const dlr_rec_mtrxtbl[EC_DLM_RECDL_EVT_MAX][EC_DLM_RECDL_STN_MAX]) (VOID* bufp, USHORT cpri_no);
extern EC_DLM_MAIN_EVT	ge_dlw_main_event[CMD_MAX_SYSNUM];

extern USHORT 	gw_dl_FileType;
#endif	/* M_DL_EXT */
