/*!
 * @skip		$ld:$
 * @file		f_log_ftyp.h
 * @brief		ログスレッド プロトタイプ宣言
 * @date		2010/05/24 ALPHA)横山 Create
 * @date		2013/03/28 ALPHA)松延 B25 対処
 * @date		2013/10/29 ALPHA)横山 docomo ZYNQ化 & 共通化対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2010-2013
 */

/*!
 * @addtogroup RRH_PF_LOG
 * @{
 */
#ifndef F_LOG_FTYP_H
#define F_LOG_FTYP_H

/*!
 * @name ログスレッド状態
 * @note ログスレッドの状態定義
 * @{
 */

extern	VOID	f_log_main(VOID);
extern	INT		f_log_init(VOID*);
extern	INT		f_log_anlz(VOID*);
extern	INT		f_log_nop(VOID*);

extern	INT		f_log_Idle_APITroublelogMake(VOID*);
extern	INT		f_log_Idle_APITroublelogDel(VOID*);
extern	INT		f_log_Idle_NtcTroublelog(VOID*);
extern	INT		f_log_Idle_MakeALMTroublelog(VOID*);
extern	INT		f_log_Idle_ToTakeShelter(VOID*);

extern	INT 	f_log_send_APITroubleLogFinNtc( UINT dstPQID, UINT dstTQID, UINT okng, CHAR* filename, UINT size, UINT sysKind );
extern	INT 	f_log_send_EventLogReq( UINT	kind );
extern	INT 	f_log_takeShelter();
 	
extern	INT		f_log_Wait_StopAPITroublelog(VOID*);
extern	INT		f_log_Wait_APITroublelogStop(VOID*);
extern	INT		f_log_Wait_APITroublelogMake(VOID*);
extern	INT		f_log_Wait_APITroublelogDel(VOID*);
extern	INT		f_log_Wait_ToTakeShelter(VOID*);
extern	INT		f_log_Wait_NtcTroublelog(VOID*);
extern	INT		f_log_Wait_MakeALMTroublelog(VOID*);

extern	INT		f_log_makeTroubleLog(UINT  uiTroubleKind );
/* @} */


#endif
/* @} */
