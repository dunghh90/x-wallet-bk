/*!
 * @skip  $ld:$
 * @file  f_fdl_ftyp.h
 * @brief FDLタスク プロトタイプ宣言
 * @date  2019/01/28 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */
#ifndef F_FDL_FTYP_H
#define F_FDL_FTYP_H

/*!
 * @name FDLタスク
 * @note プロトタイプ宣言
 * @{
 */

extern	VOID	f_fdl_main( ) ;

extern	INT		f_fdl_anlz( VOID* ) ;
extern	INT		f_fdl_abort( VOID* ) ;
extern	INT		f_fdl_init( VOID* ) ;

/* タスク処理 */
extern	INT		f_fdl_inventry_rcv ( VOID* ) ;
extern	INT		f_fdl_download_rcv ( VOID* ) ;
extern	INT		f_fdl_install_rcv ( VOID* ) ;
extern	INT		f_fdl_activate_rcv ( VOID* ) ;
extern	INT		f_fdl_msi_err ( VOID* ) ;
extern	INT		f_fdl_filewrite_m ( VOID* ) ;
extern	INT		f_fdl_filewrite_s ( VOID* ) ;

/* Sub関数 */
extern VOID f_fdl_send_MSInotif(INT error_code , UINT w_slot) ;
extern VOID f_fdl_send_MSIrej(UINT w_requestId , INT error_code) ;
extern INT	f_fdl_file_check(VOID) ;
extern INT	f_fdl_checksum(char * file_name , char * mani_data) ;
extern INT	f_fdl_get_file_name(char * check_nm , char * file_name) ;
extern INT	f_fdl_flash_write(UINT write_kind , UINT w_slot) ;
extern INT	f_fdl_install_check( VOID* rcvMsg_p , UINT flsh_id) ;
extern INT	f_fdl_install_file_check( T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ* msgP , UINT *w_slot) ;
extern UINT	f_fdl_file_unzip(char*	w_file_name) ;
extern VOID	f_fdl_file_eep_init( UINT w_slot ) ;
extern INT	f_fdl_get_install_mani_list( T_FDL_MANIFEST_DATA *w_manip ) ;
extern INT	f_fdl_install_data_save( VOID ) ;

/* 内部データ */
extern T_FDL_INFO					f_fdl_thdInfo;

/* 状態分析テーブル */
extern const	T_FDL_CALLSETRELL_MTXTBL f_fdl_callSetRell_mtxtbl[D_FDL_STATENUM][E_FDL_EVE_EVENTNUM];
#endif
/* @} */
