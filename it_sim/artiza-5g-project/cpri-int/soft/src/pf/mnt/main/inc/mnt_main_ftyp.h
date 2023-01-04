/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  mnt_main_ftyp.h
 * @brief mnt main data colloction function declaration.
 * @date  2013/11/15 FFCS)zhaodx Create for zynq
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
 /****************************************************************************/
#ifndef MNT_MAIN_FTYP
#define MNT_MAIN_FTYP


/* Main part */

extern VOID mnt_mkm_main( void *param );

extern VOID mnt_lnr_main( void *param );

extern VOID mnt_lns_main( void *param );

extern VOID mnt_main_abort(UINT *inf_p);


/* Sub part */
extern INT mnt_main_Init(void);

extern VOID mnt_main_Create(void);

extern VOID mnt_main_TcpServerInit(void);

extern VOID mnt_main_systemInit(void);

extern UINT mnt_main_Anlz(UINT a_event);

extern VOID mnt_main_regMsgMapping();

extern INT mnt_main_signal( int signum, void *info, void *ctx , int fd );

extern VOID mnt_main_signalProc();

extern VOID mnt_main_abortLog();

extern VOID mnt_main_logSave(INT);

extern VOID mnt_main_logSave_and_Rsp(INT, INT);

#endif	/*MNT_MAIN_FTYP*/
