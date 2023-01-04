/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_COM_LOG.h
 *  @brief  Definitions for BPF Internal Common LOG
 *  @date   $Date:$ $Author:$
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_COM_LOG_H_
#define _BPF_COM_LOG_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_COM.h"
#include "BPF_COM_PLOG.h"
#include "BPF_RU_IPCM.h"
#include "BPF_RU_FACC.h"
#include "f_rrh_inc.h"

/* ファイル出力先決定用定義	*/
enum{
	E_BPF_LOG_CMDLOG_OUTDIR_NORMAL,					/* 通常時(Debugコマンドなど)					*/
	E_BPF_LOG_CMDLOG_OUTDIR_ALM,					/* 障害発生時									*/
	E_BPF_LOG_CMDLOG_OUTDIR_SEND,					/* 報告用障害ログファイル作成時(S3G用)			*/
	E_BPF_LOG_CMDLOG_OUTDIR_ALM_HEALTCHK,			/* 障害発生時(HealthCheck断)					*/
	E_BPF_LOG_CMDLOG_OUTDIR_SEND_3G,				/* 報告用障害ログファイル作成時(3G用)			*/
	E_BPF_LOG_CMDLOG_OUTDIR_ALM_RST,				/* 障害発生時(RESET有)							*/
	E_BPF_LOG_CMDLOG_OUTDIR_MAX
};

// 擬似環境 20210316
//#ifndef SIM_DEBUG
#if 0
// 擬似環境 20210316
/* Function prototype */
#define BPF_COM_LOG_ASSERT( logLevel, Message_p, ... )	\
  do {																							\
 			bpf_com_log_assert_real( logLevel,  __FILE__, __LINE__, Message_p, ##__VA_ARGS__ );	\
			BPF_COM_PLOG_TRACE_PROC( logLevel , Message_p, ##__VA_ARGS__ );						\
 }while(0)
#else
//　擬似環境
//#define BPF_COM_LOG_ASSERT( logLevel, Message_p, ... )			0//C9
#define BPF_COM_LOG_ASSERT( logLevel, Message_p, ... )											\
  do {																							\
 			bpf_com_log_assert_real( logLevel,  __FILE__, __LINE__, Message_p, ##__VA_ARGS__ );	\
			BPF_COM_PLOG_TRACE_PROC( logLevel , Message_p, ##__VA_ARGS__ );						\
 }while(0)
//　擬似環境
#endif

VOID bpf_com_log_assert_real(UINT logLevel, const char* name, int line , const char* Message_p, ... );

VOID BPF_COM_LOG_REG( UINT logLevel, UINT access_type, UINT addr, UINT data1, UINT data2 );

UINT BPF_COM_LOG_GET_SYS_LOG( void );

FILE* BPF_COM_LOG_OUTPUT_FILE_OPEN( CHAR* open_filename, UINT kind, UCHAR* output_filename );
UINT BPF_COM_LOG_OUTPUT_FILE_CLOSE( FILE *fp );

VOID BPF_COM_LOG_RST_REASON(UINT version, UINT rstReason, UINT addInfo );
UINT BPF_COM_LOG_GET_RST_REASON( UINT *rstReason );
VOID  BPF_COM_LOG_SAVE_FLASH( void );

VOID  BPF_COM_LOG_SPI( UINT devkind, UINT rwkind, UINT addr, UINT data );

/* SFP 管理ログ */
int BPF_COM_LOG_SFP_INFO_STORE( int ps, T_RRH_SFPINFO *param_sfpinfo );
int BPF_COM_LOG_SFP_INFO_LOAD( int ps, T_RRH_SFPINFO_HIS *sfpinfo );
void BPF_COM_LOG_SFP_INFO_CLR( int ps );
/* */
void BPF_COM_LOG_SVC_STORE(  unsigned int chgType, UINT *svinf ) ;
void BPF_COM_LOG_SVC_LOAD( T_RRH_SVSTATE_RECORD *loadbuf );


/* このマクロでログ取得すると、dmesgコマンドでログを確認できる */
#define BPF_COM_LOG_DMESG( Message_p, ... ) bpf_com_log_fprint( "/dev/kmsg" , 0 , Message_p, ##__VA_ARGS__ )
VOID bpf_com_log_fprint(const char* outname , int logLevel , const char* Message_p , ... );

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define BPF_COM_LOG_ASSERT_DWL( logLevel, Message_p, ... ) BPF_LOG_ASSERT_DWL( logLevel,  __FILE__, __LINE__, Message_p, ##__VA_ARGS__ )
VOID BPF_LOG_ASSERT_DWL(UINT logLevel, const char* name, int line, const char* Message_p, ... );
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/********************************************* EOF ******************************************************************/
#endif /* _BPF_COM_LOG_H__ */
