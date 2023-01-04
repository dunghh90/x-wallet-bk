/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_pfm_ftyp.h
 *  @brief	PFM thread function prototype header
 *  @date	2018/11/23 FJT)H.Yoshida Create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/******************************************************************************************************************************/

/* 二重取込み防止 */
#ifndef		F_PFM_FTYP
#define		F_PFM_FTYP
/* ======================================================================== */
/*	PFM Thread 関数プロトタイプ定義										*/
/* ======================================================================== */

extern	INT		f_pfm_abort( VOID* );
extern	INT		f_pfm_anlz( VOID* );
extern	INT		f_pfm_InitReq( VOID* );
extern	VOID	f_pfm_main( void );
extern	INT		f_pfm_timeout_ind( VOID* );
extern	INT		f_pfm_log_timeout( VOID * );
extern	INT		f_pfm_msi_recv_ind( VOID* );
extern	INT		f_pfm_o_ran_pfm_mgmt_editReq( VOID*,  UINT );
extern	INT		f_pfm_o_ran_pfm_mgmt_getReq( VOID*,  UINT );
extern	INT		f_pfm_timeout_ind( VOID* );
extern	INT		f_fhs_msi_recv_ind( VOID* );
extern	VOID	f_pfm_set_log( UINT, VOID*, UINT, UINT*,UINT, UINT* );
extern	VOID	f_pfm_set_log_peri( VOID*, UINT , UINT*,  UINT* );

#endif
