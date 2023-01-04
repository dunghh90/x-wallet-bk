/*!
 * @skip  $ld:$
 * @file  f_ful_ftyp.h
 * @brief FULタスク プロトタイプ宣言
 * @date  2019/01/28 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */
#ifndef F_FUL_FTYP_H
#define F_FUL_FTYP_H

/*!
 * @name FULタスク
 * @note プロトタイプ宣言
 * @{
 */

extern	VOID	f_ful_main( ) ;

extern	INT		f_ful_anlz( VOID* ) ;
extern	INT		f_ful_abort( VOID* ) ;
extern	INT		f_ful_init( VOID* ) ;

/* タスク処理 */
extern	INT		f_ful_filelist_rcv ( VOID* ) ;
extern	INT		f_ful_upload_rcv ( VOID* ) ;

extern	VOID joinpath(char *path, const char *path1, const char *path2) ;
extern	VOID f_ful_listfiles(char *path, int recursive , FILE *fp , char *filnm) ;
extern	VOID f_ful_send_MSIrej(UINT w_requestId) ;
/* 内部データ */
extern	T_FUL_INFO					f_ful_thdInfo;

/* 状態分析テーブル */
extern	const	T_FUL_CALLSETRELL_MTXTBL f_ful_callSetRell_mtxtbl[D_FUL_STATENUM][E_FUL_EVE_EVENTNUM];
															/**< FUL State MngSTM  */


#endif
/* @} */
