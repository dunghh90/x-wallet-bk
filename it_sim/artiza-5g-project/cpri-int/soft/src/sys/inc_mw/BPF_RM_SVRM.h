/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_RM_SVRM.h
 *  @brief  Definitions for BPF_RM_SVRM API
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#ifndef _BPF_RM_SVRM_H_
#define _BPF_RM_SVRM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_COM.h"

/********************************************************************************************************************/
/* External Definition                                                                                              */
/********************************************************************************************************************/
/********************************************************************************************************************/
/* define                                                                                                           */
/********************************************************************************************************************/
/* Return code:終了コード */
#define BPF_RM_SVRM_COMPLETE    0           /* 正常終了                                     */
#define BPF_RM_SVRM_ERR_DISC    1           /* プロセス識別子異常                           */
#define BPF_RM_SVRM_ERR_EXEC    2           /* 内部処理異常                                 */
#define BPF_RM_SVRM_ERR_SYS     3           /* システムコール失敗                           */

#define BPF_RM_SVRM_APL         0           /* APL */
#define BPF_RM_SVRM_PF          1           /* PF  */

/********************************************************************************************************************/
/* enum                                                                                                             */
/********************************************************************************************************************/
/* 結果コード */
typedef enum {
    BPF_RM_SVRM_OK              = 0x00000000,           /* 結果 OK          */
    BPF_RM_SVRM_NG              = 0x00000001            /* 結果 NG          */
} bpf_rm_svrm_result;

/********************************************************************************************************************/
/* typedef                                                                                                          */
/********************************************************************************************************************/
typedef struct {
	UCHAR	name[32];		/*!< table name 											*/
	UINT	size;			/*!< table size 											*/
	VOID*	addr;			/*!< table address											*/
	UINT	mode;			/*!< table swap size mode(0;noswap,2:2byte,4:4byte)			*/
} bpf_rm_svrm_sigget_tbl_t;

/********************************************************************************************************************/
/* Function Prototype                                                                                               */
/********************************************************************************************************************/
int BPF_RM_SVRM_INIT(unsigned int process_desc);
int BPF_RM_SVRM_ASSIGN(unsigned int process_desc);
int BPF_RM_SVRM_UNASSIGN(unsigned int process_desc);
int BPF_RM_SVRM_INITEND_IND(bpf_rm_svrm_result result);
int BPF_RM_SVRM_INITEND_IND_RCV (unsigned int *prockind , unsigned int *procnum , bpf_rm_svrm_result *result, sigset_t *sigSetVal , unsigned int* sigUsr1Flg , unsigned int* sigUsr2Flg );
int BPF_RM_SVRM_CPUAFFINITY (unsigned int process_desc, int cpu, int* errcd);
int BPF_RM_SVRM_CPUAFFINITY_ADD (unsigned int process_desc, int cpu, int* errcd);
int BPF_RM_SVRM_SHM_CREATE(unsigned int p_desc, unsigned int data_cnt, void *shmTable);
void BPF_RM_SVRM_SIGNAL_SEND(int pid, int signo, int option);
int BPF_RM_SVRM_SIGNAL_HANDLE_ENTRY( int* funcP, sigset_t* sigSetVal , int sig_sndonoff );
char* BPF_RM_SVRM_SIGNAL_COREDUMP(FILE* fp, char* topbuf_p, unsigned int size );
void BPF_RM_SVRM_SIGNAL_TBLSAVE( int fd , int tblnum , char* tbl_top_p);
int BPF_RM_SVRM_SIGNAL_WAIT( sigset_t* sigSetVal, int* signal_pid, int* signal_no, int* signal_option );
int BPF_RM_SVRM_SIGNAL_WAIT_TIME( sigset_t* sigSetVal, struct timespec* time, int* signal_pid, int* signal_no, int* signal_option );
void BPF_RM_SVRM_SIGNAL_OUTPUT_CONTEXT( void* ctx );
int BPF_RM_SVRM_THREAD_INFO_SET( unsigned int processID, unsigned int threadID, unsigned int threadQueueID , unsigned long int pThreadID );

/********************************************* EOF ******************************************************************/
#endif /* _BPF_RM_SVRM_H_ */

