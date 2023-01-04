/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_i_ru_ipcm_shm.h
 *  @brief  Definitions for BPF IPCM Internal
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_I_RU_IPCM_SHM_H_
#define _BPF_I_RU_IPCM_SHM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "bpf_i_ru_ipcm.h"

/* 共有メモリ取得時のフラグ */
#define IPCM_SHMFLG IPC_CREAT|S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
/* 共有メモリアタッチ時のフラグ */
#define IPCM_SHMATFLG S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP

/* 構造体定義 */
/* 共有メモリが使用中かどうかのフラグ定義 */
enum ipcm_shm_used {
  IPCM_SHM_UNUSED = 0,    /* 未使用 */
  IPCM_SHM_AT,            /* アタッチ状態 */
  IPCM_SHM_DT             /* デタッチ状態 */
};

/* 共有メモリ作成用の事前登録テーブル	*/
struct ipcm_shm_create_data_detail_t {
	unsigned int	shmcnt;
	unsigned int	shmsize;
};

struct ipcm_shm_create_data_t {
	unsigned int							datacnt;
	struct ipcm_shm_create_data_detail_t	info[128];		/* 128固定	*/
};


/* 内部関数 */
int bpf_ru_ipcm_shm_numcheck(enum ipcm_shm_type, unsigned int);
int bpf_ru_ipcm_shm_write_data(enum ipcm_shm_type, unsigned int, 
			    int, void *, int);
int bpf_ru_ipcm_shm_read_data(enum ipcm_shm_type, unsigned int, int *, int *, 
			   void **, int *);
int bpf_ru_ipcm_shm_read_tbl(enum ipcm_shm_type, enum ipcm_shm_type *, int *, 
			  int *, int *, int *, void **);


/********************************************************************************************************************/


#endif /* _BPF_I_RU_IPCM_SHM_H_ */

