/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_dhcp_ftyp.h
 *  @brief	DHCP thread function prototype header
 *  @date	2018/02/23 FJT)H.Yoshida Create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/******************************************************************************************************************************/

/* 二重取込み防止 */
#ifndef		F_DHCP_FTYP
#define		F_DHCP_FTYP
/* ======================================================================== */
/*	DHCP Thread 関数プロトタイプ定義										*/
/* ======================================================================== */

extern	INT		f_dhcp_abort( VOID* );
extern	INT		f_dhcp_anlz( VOID* );
extern	INT		f_dhcp_ConnectReadyWrap( VOID* );
extern	INT		f_dhcp_ConnectReady( VOID* );
extern	INT		f_dhcp_InitReq( VOID* );
extern	VOID	f_dhcp_main( void );
extern	INT		f_dhcp_get_ipaddr_vd( USHORT, USHORT, USHORT*, UINT );
extern	INT		f_dhcp_get_ipaddr_d( USHORT, USHORT, USHORT*, UINT );
extern	INT		f_dhcp_get_ipaddr_v( USHORT, USHORT, USHORT*, UINT );
extern	INT		f_dhcp_get_ipaddr( USHORT, USHORT, USHORT*, UINT );
extern	INT		f_dhcp_fread_lease( VOID );
extern	INT		f_dhcp_msi_recv_ind( VOID* );
extern	INT		f_dhcp_o_ran_dhcp_getReq( VOID*, UINT );
extern	INT		f_dhcp_o_ran_mplane_int_editReq( VOID*, UINT );
extern	INT		f_dhcp_o_ran_mplane_int_getReq( VOID*, UINT );
extern	INT		f_dhcp_vlan_write_rsp( VOID* );
#endif
