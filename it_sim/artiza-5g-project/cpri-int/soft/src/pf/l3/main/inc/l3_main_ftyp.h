/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  l2_main_ftyp.h
 * @brief l2 main data colloction function declaration.
 * @date  2013/11/15 FFCS)niemsh Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008
 */
 /****************************************************************************/
#ifndef L3_MAIN_FTYP
#define L3_MAIN_TX_FTYP


/* Main part */
extern VOID l3_main_Create();
extern VOID l3_main_regMsgMapping();
extern VOID l3_main_abort(VOID*);
extern INT l3_main_signal( INT , VOID *, VOID * ,INT );
extern VOID l3_main_signalProc();
extern VOID l3_main_logSave(INT);
extern VOID l3_main_logSave_and_Rsp(INT, INT);
extern INT	l3_main_Init(VOID);
extern VOID l3_main_systemInit(VOID);
extern VOID l3_main_abortLog();
extern VOID l3_main_subGetVersion(VOID);
	
#endif	/*TM_TX_FTYP*/
