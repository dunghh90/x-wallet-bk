/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  l2_main_ftyp.h
 * @brief l2 main data colloction function declaration.
 * @date  2013/11/15 FFCS)hongj Create for zynq
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
 /****************************************************************************/
#ifndef L2_MAIN_FTYP
#define L2_MAIN_FTYP


/* Main part */

extern VOID l2_lpb_main( void *param );

extern VOID l2_txs_main( void *param );

extern VOID l2_rxs_main( void *param );

extern VOID l2_pol_main( void *param );

extern VOID l2_main_abort(UINT *inf_p);

/* Sub part */
extern VOID l2_main_main(void);

extern UINT l2_main_Anlz(UINT a_event);

extern VOID l2_main_Init();

extern VOID l2_main_Create();

extern VOID l2_main_systemInit(void);

extern INT l2_main_signal( int , void *, void * ,int );

extern VOID l2_main_signalProc();

extern VOID l2_main_logSave(int);

extern VOID l2_main_logSave_and_Rsp(INT, INT);

extern VOID l2_main_abortLog();

extern VOID	l2_main_regMsgMapping();
#endif	/*L2_MAIN_FTYP*/
