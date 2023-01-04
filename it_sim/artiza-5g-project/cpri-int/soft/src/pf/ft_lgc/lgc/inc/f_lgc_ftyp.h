/*!
 * @skip		$ld:$
 * @file		f_lgc_ftyp.h
 * @brief		ログスレッド プロトタイプ宣言
 * @date		2010/05/24 ALPHA)横山 Create
 * @date		2013/03/28 ALPHA)松延 B25 対処
 * @date		2013/10/29 ALPHA)横山 docomo ZYNQ化 & 共通化対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2010-2013
 */

/*!
 * @addtogroup RRH_PF_LGC
 * @{
 */
#ifndef F_LGC_FTYP_H
#define F_LGC_FTYP_H

/*!
 * @name ログスレッド状態
 * @note ログスレッドの状態定義
 * @{
 */

extern	VOID	f_lgc_main(VOID);
extern	INT		f_lgc_init(VOID*);
extern	INT		f_lgc_anlz(VOID*);
extern	INT		f_lgc_nop(VOID*);

extern	INT		f_lgc_Idle_MakeALMTroublelog(VOID*);
extern	INT		f_lgc_Idle_ToTakeShelter(VOID*);
extern	INT 	f_lgc_send_EventLogReq( UINT	kind );
extern	INT 	f_lgc_takeShelter();

extern	INT		f_lgc_makeTroubleLog(UINT  uiTroubleKind );
extern	VOID	f_lgc_abortLog(UINT kind);




extern	INT f_lgc_MSI_rcv_ind(VOID* rcvMsg_p);
extern	INT f_lgc_MSI_send_ind(VOID* rcvMsg_p);



/* @} */


#endif
/* @} */
