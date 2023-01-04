/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_eqs_ftyp.h
 *  @brief	5GDU-LLS Supervision common function prototype definition
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2010/03/02 QNET)Kabasima MOD M-S3G-eNBPF-03221:CPRI共用からLTE単独構成変更時にREのレジスタに3G-L3ERRが残る場合がある
 *  @date   2018/08/21 KCN)Kamimoto 5GDU-LLS Update
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2019
 */
/******************************************************************************************************************************/

/* 二重取込み防止 */
#ifndef		F_EQS_FTYP
#define		F_EQS_FTYP
/* ======================================================================== */
/*	5GDU-LLS監視タスク関数プロトタイプ定義										*/
/* ======================================================================== */

extern	VOID f_eqs_Main();
extern	VOID f_eqs_anlz( VOID * );
extern	VOID f_eqs_InitReq( VOID );
extern	VOID f_eqs_TraStaChgNtc( T_SYS_EQS_ALM_INFO* );
extern	VOID f_eqs_LED_cntl( UINT* );

extern	VOID f_eqs_Tra_o_ran_GETReq( UINT w_req_id , VOID* w_payloadPtr );
extern	VOID f_eqs_TraietfGETReq( UINT w_req_id , VOID* w_payloadPtr );
extern	VOID f_eqs_TraietfStsGETReq( UINT w_req_id , VOID* w_payloadPtr );
extern	VOID f_eqs_TraietfStsEDITReq( UINT w_req_id , VOID* w_payloadPtr );
extern	VOID f_eqs_Tra_o_ran_ResetReq( UINT w_req_id , VOID* w_payloadPtr );
extern	VOID f_eqs_Tra_o_ran_FMalrmStaIND( UINT w_req_id , VOID* w_payloadPtr );
extern	VOID f_eqs_TraietfStartIND( UINT w_req_id , VOID* w_payloadPtr );
extern	VOID f_eqs_Tra_o_ran_StsEditReq( UINT w_req_id , VOID* w_payloadPtr );

extern	VOID f_eqs_o_ran_operations_getReq( UINT w_req_id , VOID* w_payloadPtr );
extern	VOID f_eqs_o_ran_operations_editReq( UINT w_req_id , VOID* w_payloadPtr );

extern	VOID f_eqs_enable_Edit( UINT sts_data , E_MPSW_ADMIN_STATE *admin_state , E_AVAILABILITY_TYPE *availability_state ) ;
extern	VOID f_eqs_send_alarm_notif( UINT idx_fault_id , UINT wis_cleared ) ;
extern	VOID f_eqs_enable_notif( UINT sts_data ) ;
extern	VOID f_eqs_disable_notif( UINT sts_data ) ;
extern	VOID f_eqs_status_change_notif( UINT ) ;
extern	VOID f_eqs_fm_alarm_notif( T_SYS_EQS_ALM_INFO * alarm_info_p , uint idx_fault_id ) ;
extern	VOID f_eqs_faultChg( UINT * );
extern	VOID f_eqs_inhibitStpWv( VOID );
extern	VOID f_eqs_stsChgProc( UINT );
extern	VOID f_eqs_stpWvProc( UINT * );


#endif	/* M_EQS_FTYP */
