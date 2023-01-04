/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_l_ru_ipcm.h
 *  @brief  Definitions for BPF ipcm library
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_L_RU_IPCM_H_
#define _BPF_L_RU_IPCM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_RU_IPCM.h"

/* メッセージキュー関係 */
/* メッセージキュータイプの定義 */
enum ipcm_msgq_type {
  IPCM_MSGQ_USER = 0,
  IPCM_MSGQ_DUMMY,
  IPCM_MSGQ_TYPE_END
};

/* セマフォ関係 */
/* セマフォタイプの定義 */
enum ipcm_sem_type {
  IPCM_SEM_USER = 0,
  IPCM_SEM_DUMMY,
  IPCM_SEM_TYPE_END,
};

/* 共有メモリ関係 */
/* 共有メモリタイプの定義 */
enum ipcm_shm_type {
  IPCM_SHM_USER = 0,
  IPCM_SHM_DUMMY,
  IPCM_SHM_TYPE_END,
};

/* Function Prototype */
/* 共通 */
int bpf_ru_ipcm_assign(unsigned int);
int bpf_ru_ipcm_unassign(unsigned int);

/* プロセス間メッセージキュー関係 */
int bpf_ru_ipcm_procmsgq_send2(enum ipcm_msgq_type, unsigned int, const void *,
                            unsigned int, int , int *);
int bpf_ru_ipcm_procmsgq_receive2(enum ipcm_msgq_type, unsigned int, int, 
                               unsigned int, void **, unsigned int *, int *);
int bpf_ru_ipcm_procmsgq_nummsgs2(enum ipcm_msgq_type, unsigned int, 
                               unsigned int *, int *);
int bpf_ru_ipcm_procmsgq_create(enum ipcm_msgq_type, unsigned int, int *);

void bpf_ru_ipcm_real_time_log_open();

/* セマフォ関係 */
int bpf_ru_ipcm_procsem_take2(enum ipcm_sem_type, unsigned int, int, int,
			   struct bpf_timespec *, int *);
int bpf_ru_ipcm_procsem_give2(enum ipcm_sem_type, unsigned int, int, int *);

int bpf_ru_ipcm_procsem_init(enum ipcm_sem_type , int *);

/* 共有メモリ関係 */
int bpf_ru_ipcm_procshm_addrget2(enum ipcm_shm_type, unsigned int, 
                              void **,int *);
int bpf_ru_ipcm_procshm_create(enum ipcm_shm_type, unsigned int, char *, int *);

/********************************************* EOF ******************************************************************/
#endif /* _BPF_L_RU_IPCM_H_ */

