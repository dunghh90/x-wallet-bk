/*!
 * @skip  $ld:$
 * @file  f_ini_ftyp.h
 * @brief 初期化プロセス プロトタイプ宣言
 * @date  2013/03/31 ALPHA) kondou Create RRH-007-000 初期化
 * @date  2013/11/07 ALPHA) nakamura modify for ZYNQ
 * @date  2014/12/04 ALPHA) okabe modify
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */
#ifndef F_INI_FTYP_H
#define F_INI_FTYP_H


/*!
 * @name 関数外部宣言
 * @note 初期化プロセスの関数外部宣言
 * @{
 */
VOID	f_ini_AccidentlogGet( T_INIT_SIG_INFO_TBL* infoTbl, VOID* buf_p );
VOID	f_ini_led_alm(VOID* buf_p );
VOID	f_ini_AccidentlogSave(UINT uiTroubleKind, VOID* buf_p );
UINT	f_ini_iniBpf( VOID* buf_p );
VOID	f_ini_iniTbl( VOID );
#ifdef OPT_RRH_CT
INT f_ini_main(INT argc, CHAR *argv[]);
#else
INT		main(INT argc, CHAR *argv[]);
#endif
UINT	f_ini_procCreate(INT, CHAR** ,VOID* buf_p );
UINT	f_ini_threadCreate(VOID* buf_p );
UINT	f_ini_threadCreate2(VOID* buf_p );
VOID	f_ini_reset(UINT resetReason, UINT addInfo, VOID* buf_p );
INT		f_ini_sigHandler(INT, siginfo_t *, VOID * , int);
VOID	f_ini_bootUpdate();

VOID f_ini_logget_finish( T_INIT_SIG_INFO_TBL* infoTbl ,VOID* buf_p );
VOID f_ini_logget_sigCheck( siginfo_t *sigInfo, UINT *kind, UINT *eventNo ,VOID* buf_p );
INT f_ini_logget_alm_chkRST(siginfo_t *sigInfo , VOID* buf_p , T_INIT_SIG_INFO_TBL* tmptbl );
VOID  f_ini_logget_alm_reset(T_INIT_SIG_INFO_TBL* infoTbl, VOID* buf_p );

INT  f_ini_logget_init_alm(VOID* msg_p, VOID* buf_p );
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
INT  f_ini_logget_init_hlt(siginfo_t *sigInfo, VOID* buf_p );
INT  f_ini_logget_init_run(siginfo_t *sigInfo, VOID* buf_p );
VOID f_ini_disable(UINT sigState, VOID* buf_p );
VOID f_ini_CpriLinkLed(UINT reg_data);
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
INT  f_ini_nop(VOID* msg_p, VOID* buf_p );
INT  f_ini_recieve_SIGUSR2(siginfo_t *sigInfo,VOID* buf_p );
VOID f_ini_logget_sigInfo( INT, INT, INT ,VOID* buf_p );

VOID f_ini_main_alm(VOID);
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
VOID f_ini_main_hlt(VOID);
VOID f_ini_main_run(VOID);
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
VOID f_ini_main_con(VOID);

VOID f_ini_ptpConfUpdate(VOID);

/* @} */

#endif
/* @} */
