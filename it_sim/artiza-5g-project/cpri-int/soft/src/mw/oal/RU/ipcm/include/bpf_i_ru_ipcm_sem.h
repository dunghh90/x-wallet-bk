/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_i_ru_ipcm_sem.h
 *  @brief  Definitions for BPF IPCM Internal
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_I_RU_IPCM_SEM_H_
#define _BPF_I_RU_IPCM_SEM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "bpf_i_ru_ipcm.h"

/********************************************************************************************************************/

/* セマフォ取得時のフラグ */
#define IPCM_SEMFLG IPC_CREAT|S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
/* セマフォ取得してないときは-1をいれとく */
#define IPCM_SEM_NOCREATE -1
/* セマフォ値の最大数(=同時Readセマフォ数) */
#define BPF_RU_IPCM_SEMVAL_MAX   2048 

/* ユーザ用セマフォキーの番号 */
#define IPCM_SEM_AKEYNUM 0    
/* PF用セマフォキーの番号 */
#define IPCM_SEM_PKEYNUM 0    

/* 構造体定義 */
/* セマフォが使用中かどうかのフラグ定義 */
enum ipcm_sem_used {
  IPCM_SEM_UNUSED = 0,
  IPCM_SEM_USED
};

/* セマフォロック 数 */
typedef struct {
	int rw_lock_count;
	int r_lock_count;
} bpf_ru_ipcm_semlock_count_t;

/* IPCM内部関数 */
int bpf_ru_ipcm_sem_numcheck(enum ipcm_sem_type, unsigned int);
int bpf_ru_ipcm_sem_write_semid(enum ipcm_sem_type, int);
int bpf_ru_ipcm_sem_write_locknum_clear(enum ipcm_sem_type, unsigned int);
int bpf_ru_ipcm_sem_write_locknum_inc(enum ipcm_sem_type, unsigned int, int);
int bpf_ru_ipcm_sem_write_locknum_dec(enum ipcm_sem_type, unsigned int, int);
int bpf_ru_ipcm_sem_write_used(enum ipcm_sem_type, unsigned int, int);
int bpf_ru_ipcm_sem_read_data(enum ipcm_sem_type, unsigned int,
			  int *, int *, bpf_ru_ipcm_semlock_count_t *);
int bpf_ru_ipcm_sem_read_tbl(enum ipcm_sem_type, enum ipcm_sem_type *, int *, 
			  int *, int *);


#endif /* _BPF_I_RU_IPCM_SEM_H_ */

