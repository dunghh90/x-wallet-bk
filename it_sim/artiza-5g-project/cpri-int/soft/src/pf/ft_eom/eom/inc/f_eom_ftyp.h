/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_eom_ftyp.h
 *  @brief	EOM thread function prototype header
 *  @date	2018/11/23 FJT)H.Yoshida Create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/******************************************************************************************************************************/

/* 二重取込み防止 */
#ifndef		F_EOM_FTYP
#define		F_EOM_FTYP
/* ======================================================================== */
/*	EOM Thread 関数プロトタイプ定義											*/
/* ======================================================================== */

extern	INT		f_eom_abort( VOID* );
extern	INT		f_eom_anlz( VOID* );
extern	INT		f_eom_InitReq( VOID* );
extern	VOID	f_eom_main( void );
extern	INT		f_eom_msi_recv_ind( VOID* );
extern	INT		f_eom_o_ran_lbm_editReq( VOID*,  UINT );
extern	VOID	f_eom_main_fheth( INT* );
#endif
