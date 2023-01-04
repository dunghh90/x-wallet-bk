/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_fhs_ftyp.h
 *  @brief	FHS thread function prototype header
 *  @date	2018/11/23 FJT)H.Yoshida Create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/******************************************************************************************************************************/

/* 二重取込み防止 */
#ifndef		F_FHS_FTYP
#define		F_FHS_FTYP
/* ======================================================================== */
/*	FHS Thread 関数プロトタイプ定義										*/
/* ======================================================================== */

extern	INT		f_fhs_abort( VOID* );
extern	INT		f_fhs_anlz( VOID* );
extern	INT		f_fhs_InitReq( VOID* );
extern	VOID	f_fhs_main( void );
extern	INT		f_fhs_msi_recv_ind( VOID* );
extern	INT		f_fhs_user_write_rsp( VOID* );
extern	INT		f_fhs_ietf_if_getReq( VOID*,  UINT );
extern	INT		f_fhs_ietf_ip_editReq( VOID*, UINT );
extern	INT		f_fhs_o_ran_if_editReq( VOID*, UINT );
extern	INT		f_fhs_o_ran_if_getReq( VOID*,  UINT );
extern	INT		f_fhs_o_ran_transceiver_getReq( VOID*,  UINT );
extern	INT		f_fhs_o_ran_usermgmt_editReq( VOID*,  UINT );
extern	INT		f_fhs_o_ran_supervision_ind( VOID*, UINT );
#endif
