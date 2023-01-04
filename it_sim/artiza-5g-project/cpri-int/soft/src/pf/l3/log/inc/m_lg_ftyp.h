/*!
 * @skip  $ld:$
 * @file  m_lg_ftyp.h
 * @brief Function prototype definition for LOG thread.
 * @date  2008/07/30 FFCS)Wuh Create
 *	All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008
 */
#ifndef M_LG_FTYP_HEAD
#define M_LG_FTYP_HEAD
/****************************************************************************/
/*																			*/
/*					診断タスク関数プロトタイプ定義							*/
/*																			*/
/****************************************************************************/

/* 障害ログタスクメイン関数							*/
extern VOID m_lg_main( VOID ) ;

/* 受信メッセージ解析関数							*/
extern INT m_lg_anlz( VOID* ) ;

/* 初期化要求受付関数								*/
extern VOID m_lg_init( UCHAR*, USHORT ) ;

/* 実行条件チェック関数								*/
//extern UINT m_lg_chek_condition( USHORT ) ;
/* RE障害ログ取得要求受信関数(通常運用状態)	*/
extern VOID m_lg_Idle_GetTroublelog( UCHAR*, USHORT ) ;

/* RE障害ログ取得中止要求受信関数(通常運用状態)	*/
extern VOID m_lg_Idle_StopTroublelog( UCHAR*, USHORT ) ;

/* RE障害ログ取得要求受信関数(配下RE障害ログ取得中状態 and ログファイル作成中状態) */
extern VOID m_lg_Wait_GetTroublelog( UCHAR*, USHORT ) ;

/* 配下REからのRE障害ログ取得応答関数(配下RE障害ログ取得中状態 and ログファイル作成中状態)*/
extern VOID m_lg_Wait_TroublelogGetRes( UCHAR*, USHORT ) ;

/* 配下REのRE障害ログ取得確認タイマタイムアウト関数(配下RE障害ログ取得中状態) */
extern VOID m_lg_Wait_TroublelogGetCfTo( UCHAR*, USHORT ) ;

/* 配下REからのファイルデータ送信要求関数(配下RE障害ログ取得中状態) */
extern VOID m_lg_Wait_FilSndReq( UCHAR*, USHORT ) ;

/* 配下REからのファイルデータ送信完了報告通知関数(配下RE障害ログ取得中状態) */
extern VOID m_lg_Wait_FilSndFinNtc( UCHAR*, USHORT ) ;

/* 配下REのRE障害ログ送信確認タイマタイムアウト関数(配下RE障害ログ取得中状態) */
extern VOID m_lg_Wait_TroublelogSndCfTo( UCHAR*, USHORT ) ;

/* RECからのRE障害ログ取得中止要求関数(配下RE障害ログ取得中状態) */
extern VOID m_lg_Wait_StopTroublelog( UCHAR*, USHORT ) ;

/* 障害ログファイル作成完了通知関数(ログファイル作成中状態) */
extern VOID m_lg_Wait_NtcTroublelog( UCHAR*, USHORT ) ;

/* RECからのRE障害ログ取得中止要求関数(ログファイル作成中状態) */
extern VOID m_lg_Wait_StopTroublelog( UCHAR*, USHORT ) ;

/* RECからのRE障害ログ取得要求関数(ログ送信中状態) */
extern VOID m_lg_Send_GetTroublelog( UCHAR*, USHORT ) ;

/* RECへファイルデータ転送為の障害ログ転送間隔タイマタイムアウト関数(ログ送信中状態) */
extern VOID m_lg_Send_ToTimer( UCHAR*, USHORT ) ;

/* RECからのRE障害ログ取得中止要求関数(ログ送信中状態) */
extern VOID m_lg_Send_StopTroublelog( UCHAR*, USHORT ) ;

/* RECからのRE障害ログ取得要求関数(RE障害ログ取得中止中状態) */
extern VOID m_lg_Stop_GetTroublelog( UCHAR*, USHORT ) ;

/* RECからのRE障害ログ取得中止要求関数(RE障害ログ取得中止中状態) */
extern VOID m_lg_Stop_StopTroublelog( UCHAR*, USHORT ) ;

/* 配下REからのRE障害ログ取得中止応答関数(RE障害ログ取得中止中状態) */
extern VOID m_lg_Stop_StopTroublelogRes( UCHAR*, USHORT ) ;

/* 配下REのRE障害ログ取得中止確認タイマタイムアウト関数(RE障害ログ取得中止中状態) */
extern VOID m_lg_Stop_StopTroublelogCfTo( UCHAR*, USHORT ) ;

/* 例外イベント受付関数								*/
extern VOID m_lg_nop( UCHAR*, USHORT ) ;

/* RE障害ログ取得応答送信関数						*/
extern VOID m_lg_send_getres( UINT, USHORT, UINT ) ;

/* RE障害ログ取得中止応答送信関数					*/
extern VOID m_lg_send_stopres( USHORT, UINT ) ;

/* RE障害ログ転送関数								*/
extern VOID m_lg_send_logdata( UINT, UINT, USHORT ) ;

/* RE障害ログ転送完了通知送信関数					*/
extern VOID m_lg_send_logfinal( USHORT ) ;

/* RE障害ログ出力関数								*/
extern VOID m_lg_makeTroubleLog( UCHAR*, USHORT );

/* 障害ログファイル作成    */
extern VOID m_lg_FhmLogFilMake( USHORT a_rec_typ );

/* RE障害ログ取得中止要求処理時に、l3/logからRECにメッセージ送信処理 */
extern INT m_lg_StopTroublelogL3SendMsg( USHORT sigsys, USHORT link_num );

/* FHM障害ログファイルと取得済配下RE障害ログを書庫化処理 */
extern VOID m_lg_FhmRELogFilMake( UCHAR *a_rcv_adr, USHORT a_rec_typ );

/* 配下RE障害ログのファイルにデータを追加処理 */
extern VOID m_lg_RELogFilMake( UCHAR *a_filedatsnd, UINT a_size, USHORT a_rec_typ, USHORT a_re_no );

/* 取得済配下RE障害ログファイルを削除処理 */
extern VOID m_lg_RELogFilDelete( USHORT a_rec_typ, USHORT a_re_no );

/* ファイルデータ送信完了通知応答 */
extern INT m_lg_FileSendFinResSendMsg( USHORT sigsys, USHORT link_num, USHORT result );

/* CPRIリンク断通知 */
extern VOID m_lg_Wait_CpriLinkDwnNtc(UCHAR *a_rcv_adr, USHORT a_rec_typ);

#endif /* M_LG_FTYP_HEAD */
