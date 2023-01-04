/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_i_ru_ipcm_msgq.h
 *  @brief  Definitions for BPF IPCM Internal
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_I_RU_IPCM_MSGQ_H_
#define _BPF_I_RU_IPCM_MSGQ_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "bpf_i_ru_ipcm.h"

/********************************************************************************************************************/

/* メッセージキュー作製時のフラグ */
#define IPCM_MSGFLG IPC_CREAT|S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
/* メッセージキューに詰めるメッセージのタイプ(1固定) */
#define IPCM_MSGQ_MSGTYPE 1

/* 構造体定義 */
/* メッセージキューが使用中かどうかのフラグ */
enum ipcm_msgq_used {
  IPCM_MSGQ_UNUSED = 0,
  IPCM_MSGQ_USED
};

/* メッセージバッファ構造体 */
struct ipcm_msgbuf {
  long mtype;
  char mtext[];
};

/* 送受信データ用領域構造体宣言 */
struct bpf_ru_ipcm_msg_databuf_t {
  pthread_mutex_t     mutex;        /* ミューテックス         */
  int use_flg;                      /* データ用領域使用フラグ */
  struct ipcm_msgbuf *data_buf;     /* データ用領域           */
};


/* 内部関数 */
int bpf_ru_ipcm_msgq_numcheck(enum ipcm_msgq_type, unsigned int);
int bpf_ru_ipcm_msgq_msgszcheck(enum ipcm_msgq_type, unsigned int, unsigned int);
int bpf_ru_ipcm_msgq_write_data(enum ipcm_msgq_type, unsigned int, int, int);
int bpf_ru_ipcm_msgq_read_data(enum ipcm_msgq_type, unsigned int, int *, 
			    unsigned int *, int *);
int bpf_ru_ipcm_msgq_read_tbl(int, enum ipcm_msgq_type *, int *, int *, int *);
int bpf_ru_ipcm_msgq_read_sdatabuf(enum ipcm_msgq_type q_type, unsigned int q_num,
			    struct bpf_ru_ipcm_msg_databuf_t **sndbuf);
int bpf_ru_ipcm_msgq_read_rdatabuf(enum ipcm_msgq_type q_type, unsigned int q_num,
			    struct bpf_ru_ipcm_msg_databuf_t **rcvbuf);


#endif /* _BPF_I_RU_IPCM_MSGQ_H_ */

