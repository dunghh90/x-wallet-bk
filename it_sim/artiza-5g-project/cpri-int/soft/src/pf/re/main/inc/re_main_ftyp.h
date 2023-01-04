/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  re_main_ftyp.h
 * @brief re main data colloction function declaration.
 * @date  2015/07/-- TDIPS) maruyamma Create
 * @date  2015/08/04 TDIPS) maruyamma modify DBGƒXƒŒƒbƒh’Ç‰Á
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
 /****************************************************************************/
#ifndef RE_MAIN_FTYP
#define RE_MAIN_FTYP

extern void re_rec_main( void *param );
extern void re_rcv_main( void *param );
extern void re_send_main( void *param );
extern void re_dbg_main( void *param );

extern int re_main_Init(void);

extern void re_main_Create();

extern VOID re_main_systemInit(void);

extern INT re_main_signal( int , void *, void * ,int );

extern VOID re_main_signalProc();

extern VOID re_main_abortLog();

extern VOID re_main_logSave_and_Rsp(INT, INT);
#endif	/*RE_MAIN_FTYP*/
