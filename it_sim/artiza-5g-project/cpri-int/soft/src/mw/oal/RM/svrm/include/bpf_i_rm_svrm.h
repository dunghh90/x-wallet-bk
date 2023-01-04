/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_i_rm_svrm.h
 *  @brief  Definitions for BPF SVRM Local header
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#ifndef _BPF_I_RM_SVRM_H_
#define _BPF_I_RM_SVRM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_RM_SVRM.h"
#include "bpf_l_com.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include "f_rrh_inc.h"

/********************************************************************************************************************/
/* 共通定義 */

/* マクロ定義 */
#define SVRM_DBGLOG_FMT_ERR(...) \
  do { \
    bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, __VA_ARGS__); \
  } while(0)
#define SVRM_DBGLOG_FMT_TRC(...) \
  do { \
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE, __VA_ARGS__); \
  } while(0)

/********************************************************************************************************************/
/* Internal Definition                                                                                              */
/********************************************************************************************************************/
/* メッセージキュー作製時のフラグ */
#define SVRM_MSGFLG IPC_CREAT|S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
/* メッセージキューに詰めるメッセージのタイプ(1固定) */
#define SVRM_MSGQ_MSGTYPE 1

/* システムコール終了コード定義 */
#define SVRM_MSGCTL_NG -1
#define SVRM_MSGGET_NG -1
#define SVRM_MSGSND_NG -1
#define SVRM_MSGRCV_NG -1

/********************************************************************************************************************/
/* Internal Structure                                                                                               */
/********************************************************************************************************************/

/* メッセージバッファ構造体 */
struct svrm_msgbuf {
  long mtype;
  unsigned int prockind;
  unsigned int procnum;
  bpf_rm_svrm_result result;
};

/********************************************************************************************************************/
/* Internal enum                                                                                                    */
/********************************************************************************************************************/


/********************************************************************************************************************/
/* Internal Function Routine Prototype                                                                              */
/********************************************************************************************************************/
extern unsigned int bpf_com_my_proc_descriptor;

void bpf_rm_svrm_usr_sigusr1_hdl( int signal, siginfo_t *siginfo, void *u_contxt  );
void bpf_rm_svrm_signal_handler(int signum, siginfo_t *info, void *ctx);
int bpf_rm_svrm_thread_info_get( unsigned int *processID, unsigned int *threadID );

/********************************************* EOF ******************************************************************/
#endif /* _BPF_I_RM_SVRM_H_ */

