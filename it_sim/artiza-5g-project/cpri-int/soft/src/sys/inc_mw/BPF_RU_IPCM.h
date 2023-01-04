/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_RU_IPCM.h
 *  @brief  Definitions for BPF_RU_IPCM API
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_RU_IPCM_H_
#define _BPF_RU_IPCM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_COM.h"

/********************************************************************************************************************/
/* definitions                                                                                                      */
/********************************************************************************************************************/

/* Return code */
#define BPF_RU_IPCM_OK			 0
#define BPF_RU_IPCM_NG			 1
#define BPF_RU_IPCM_EPARAM		 2
#define BPF_RU_IPCM_ESIZEOVER	 3
#define BPF_RU_IPCM_ENOQNUM		 4
#define BPF_RU_IPCM_ENOSPACE	 5
#define BPF_RU_IPCM_EADDR		 6
#define BPF_RU_IPCM_EINTR		 7
#define BPF_RU_IPCM_ENOMSG		 8
#define BPF_RU_IPCM_ENOSEMNUM	 9
#define BPF_RU_IPCM_ENOSEMGET	 10
#define BPF_RU_IPCM_ETIMEOUT	 11
#define BPF_RU_IPCM_ENOSHMNUM	 12
#define BPF_RU_IPCM_ENOSEMCREATE 13
#define BPF_RU_IPCM_ESEMINIT     14
#define BPF_RU_IPCM_ESHMINIT     15
#define BPF_RU_IPCM_EMSGQINIT    16
#define BPF_RU_IPCM_ENOTATTACH   17

/*!
 * @name 通信用共有メモリ管理用各種定義
 * @note 通信用共有メモリ管理用各種定義
 * @{
 */
/* 通信用共有メモリ取得用			*/
/* BPF_RU_IPCM_PROCMSG_ADDRGET / BPF_RU_IPCM_PROCMSG_ADDRFREEで使用する引数はこちら	*/
enum{
	E_BPF_RU_IPCM_BUFF_KIND_TASK = 0,
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	E_BPF_RU_IPCM_BUFF_KIND_LAPBS,										/*!< LAPB送信用		*/
	E_BPF_RU_IPCM_BUFF_KIND_LAPBR,
	E_BPF_RU_IPCM_BUFF_KIND_MONITOR,
	E_BPF_RU_IPCM_BUFF_KIND_MTMMLR,
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	E_BPF_RU_IPCM_BUFF_KIND_MAX
};
/* @} */


/*!
 * @name 通信用共有メモリ詳細定義
 * @note 通信用共有メモリ詳細定義
 * @{
 */
/* 主にBPF内部で使用する変数だが	*/
/* BPF_RU_IPCM_BUFFER_FULL_OCCUR_CALLBACK_SET / BPF_RU_IPCM_BUFFER_FULL_CLEAR_CALLBACK_SETに渡す引数はこちらになる	*/
enum{
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	E_BPF_RU_IPCM_SHMKIND_LAPBS_S = 0,					/* LAPB通信(LAPB用)-送信用		*/
	E_BPF_RU_IPCM_SHMKIND_LAPBS_M,						/* LAPB通信(LAPB用)-送信用		*/
	E_BPF_RU_IPCM_SHMKIND_LAPBS_L,						/* LAPB通信(LAPB用)-送信用		*/
	E_BPF_RU_IPCM_SHMKIND_LAPBS_LL,						/* LAPB通信(LAPB用)-送信用		*/
	E_BPF_RU_IPCM_SHMKIND_LAPBR_S,						/* LAPB通信(LAPB用)-受信用		*/
	E_BPF_RU_IPCM_SHMKIND_LAPBR_M,						/* LAPB通信(LAPB用)-受信用		*/
	E_BPF_RU_IPCM_SHMKIND_LAPBR_L,						/* LAPB通信(LAPB用)-受信用		*/
	E_BPF_RU_IPCM_SHMKIND_LAPBR_LL,						/* LAPB通信(LAPB用)-受信用		*/
	E_BPF_RU_IPCM_SHMKIND_MONITOR_S,					/* モニタ用						*/
	E_BPF_RU_IPCM_SHMKIND_MONITOR_M,					/* モニタ用						*/
	E_BPF_RU_IPCM_SHMKIND_MONITOR_L,					/* モニタ用						*/
	E_BPF_RU_IPCM_SHMKIND_MONITOR_LL,					/* モニタ用						*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	E_BPF_RU_IPCM_SHMKIND_TASK_S,						/* 一般(タスク間通信)用			*/
	E_BPF_RU_IPCM_SHMKIND_TASK_M,						/* 一般(タスク間通信)用			*/
	E_BPF_RU_IPCM_SHMKIND_TASK_L,						/* 一般(タスク間通信)用			*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	E_BPF_RU_IPCM_SHMKIND_TASK_LL,						/* 一般(タスク間通信)用			*/
	E_BPF_RU_IPCM_SHMKIND_MTMMLR_BUFF,					/* MTM/MLR用					*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	E_BPF_RU_IPCM_SHMKIND_MAX
};

/* Wait flag */
#define BPF_RU_IPCM_WAIT        1
#define BPF_RU_IPCM_NOWAIT      0

#define BPF_RU_IPCM_WAITTIME_LOG	 10000000	/* 10msec */
#define BPF_RU_IPCM_WAITTIME_BUFF	 50000000	/* 50msec */
#define BPF_RU_IPCM_WAITCOUNT_BUFF	      140	/* 7sec(=>140*50msec) */

/* Semapho R/W lock kind */
#define BPF_RU_IPCM_LOCK_RW     1
#define BPF_RU_IPCM_LOCK_R      2

/* Function prototype */

int BPF_RU_IPCM_PROCMSGQ_FREE_STACK_BUFF(unsigned int );
int BPF_RU_IPCM_PROCMSGQ_RECEIVE(unsigned int, int, unsigned int, void **,unsigned int *, int *);
int BPF_RU_IPCM_PROCMSGQ_NUMMSGS(unsigned int, unsigned int *, int *);
int BPF_RU_IPCM_MSGQ_SEND(	unsigned int, void * );
int BPF_RU_IPCM_MSGQ_RECV(	unsigned int, unsigned int, void**, unsigned int* );

/* PROCSEM */
int BPF_RU_IPCM_PROCSEM_TAKE(unsigned int, int, int, struct bpf_timespec *,int *);
int BPF_RU_IPCM_PROCSEM_GIVE(unsigned int, int, int *);

/* PROCSHM */
int BPF_RU_IPCM_PROCSHM_ADDRGET(unsigned int, void **, int *);

/* 通信用共有メモリ	*/
int BPF_RU_IPCM_PROCMSG_ADDRGET(unsigned int, unsigned int, void **, int *);
int BPF_RU_IPCM_PROCMSG_ADDRFREE( void * );

/* Buffer閾値超え/復旧時のCallBack関数登録	*/
void BPF_RU_IPCM_BUFFER_FULL_OCCUR_CALLBACK_SET( void* funcP );
void BPF_RU_IPCM_BUFFER_FULL_CLEAR_CALLBACK_SET( void* funcP );

/* デバッグ用	*/
VOID BPF_RU_IPCM_PRINT_PROCMSG_BUFF_HIST( FILE* fp );
int BPF_RU_IPCM_PRINT_PROCMSG_BUFF_STATE( FILE* fp );

/********************************************* EOF ******************************************************************/
#endif /* _BPF_RU_IPCM_H_ */

