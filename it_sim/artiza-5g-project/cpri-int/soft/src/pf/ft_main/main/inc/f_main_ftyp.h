/*!
 * @skip		$ld:$
 * @file		f_main_ftyp.h
 * @brief		pf_main プロトタイプ宣言
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#ifndef F_MAIN_FTYP_H
#define F_MAIN_FTYP_H

INT		main();
UINT	f_main_abort(VOID *rcvMsg_p);
UINT	f_main_anlz(VOID *msgP);
UINT	f_main_init(VOID* rcvMsg_p);
UINT	f_main_init_ptp(VOID* rcvMsg_p);
UINT	f_main_getInv();
UINT	f_main_iniBpf();
UINT	f_main_iniBpf_ptp();
UINT	f_main_iniCard();
UINT	f_main_iniHw();
UINT	f_main_iniWait(VOID* rcvMsg_p);
VOID	f_main_mp_comp_wait();
VOID 	f_main_sigWait();
VOID	f_main_troubelelog_send(INT log_status);
VOID	f_main_flash_write_req_send(INT log_status);
INT		f_main_sigHandler(INT signal, siginfo_t *siginfo, VOID *u_contxt ,INT fd);
UINT	f_main_loadLog();
VOID	f_main_iniTbl();
UINT	f_main_setRstRsn(UINT *resetReason);
VOID	f_main_threadCreate();
UINT	f_main_rrhKindGet();
UINT	f_main_userIpQspiSet();
UINT	f_main_PCIe();
VOID	f_main_iniTbl_eepread( USHORT	addr, UCHAR	size, UCHAR	*data_p );
INT		f_main_iniTbl_flash();

VOID f_rsv_Main(  );
/* @} */

#endif
/* @} */
