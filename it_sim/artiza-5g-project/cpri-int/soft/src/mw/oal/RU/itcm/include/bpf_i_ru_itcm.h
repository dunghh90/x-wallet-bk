/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_i_ru_itcm.h
 *  @brief  Definitions for BPF ITCM Internal
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_I_RU_ITCM_H_
#define _BPF_I_RU_ITCM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_RU_ITCM.h"
#include "bpf_l_ru_itcm.h"
#include "bpf_l_com.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <pthread.h>
#include <limits.h>
/********************************************************************************************************************/

/* 
	ITCM Common
*/
#define ITCM_THREAD_FUNC_OK (0)      /* pthread関係の関数の戻り値 */
#define ITCM_SYSCALL_NG     (-1)     /* syscall NG */

/* 共有メモリ番号(ITCM内通し番号) */
#define ITCM_SHMNUM_MSGQ_STATIS 0   /* MSGQ 統計情報 */

/******************************
	ITCM pthread 
*******************************/
#define ITCM_THREAD_PRIO_ZERO 0  /* thread priority zero */
#define ITCM_THREAD_PRIO_MIN  1  /* thread priority min  */
#define ITCM_THREAD_PRIO_MAX  99 /* thread priority max  */

#define ITCM_THREAD_STACK_ZERO 0              /* stack size zero */
#define ITCM_THREAD_SYS_STACKSIZE 0x400000    /* default stack size 4MB */

/******************************
	ITCM Semaphore 
*******************************/
/* 使用中フラグの定義 */
enum itcm_sem_used {
  ITCM_SEM_UNUSED = 0,
  ITCM_SEM_USED
};

/* 管理データ構造体 */
struct itcm_sem_data_t{
  enum itcm_sem_used used; /* 使用中判別フラグ */
  pthread_mutex_t sem;     /* セマフォ */
};

/* 管理テーブル構造体 */
struct itcm_sem_tbl_t{
  int max;                      /* セマフォ最大数 */
  struct itcm_sem_data_t *data; /* データポインタ */
};

/******************************
	ITCM message 
*******************************/
enum itcm_msgq_used {
  ITCM_MSGQ_UNUSED = 0,
  ITCM_MSGQ_USED
};

/* スレッド間メッセージ管理数 */
#define ITCM_QUEUE_NUM_MAX  65

/* 共有メモリ数 */
#define ITCM_SEM_MAX  128

/* スレッド間メッセージQueue数 */
#define ITCM_MSG_MAXQLEN  1024

/* スレッド間メッセージQueue長 */
#define ITCM_MSG_MAXMSGLEN  128

/* リスト構造体 */
struct list_t{
  struct list_t *p;   /* 前のデータのアドレス */
  struct list_t *n;   /* 次のデータのアドレス */
};

/* メッセージ構造体 */
struct itcm_msg_t{
  struct itcm_msg_t *p;     /* 前のデータのアドレス */
  struct itcm_msg_t *n;     /* 次のデータのアドレス */
  int               size;   /* メッセージサイズ */
  void              *msgp;  /* メッセージ格納領域のアドレス */
};

/* メッセージキュー管理情報構造体 */
struct itcm_msg_data_t {
  int                 maxqlen;      /* 最大キュー長 */
  int                 maxmsglen;    /* 最大メッセージサイズ */
  int                 msgcount;     /* 現在保留中のメッセージ数 */
  int                 peakmsgnum;   /* 最大保留したメッセージ数 */
  enum itcm_msgq_used used;         /* 使用中判定フラグ */
  pthread_mutex_t     mutex;        /* ミューテックス */
  pthread_cond_t      cond;         /* 状態変数 */
  struct itcm_msg_t   *head;      /* メッセージリストの先頭アドレス */
  struct itcm_msg_t   *tail;      /* メッセージリストの最後尾アドレス */
};

/* メッセージキューテーブル構造体 */
struct itcm_msg_tbl_t {
  int max;                       /* メッセージキューの最大数(キューの本数) */
  struct itcm_msg_data_t *msgq;  /* メッセージデータのアドレス */
};


/* スレッド間キュー統計情報構造体 */
struct itcm_msgq_statis_info_t {
   unsigned int msgnum_now;   /* 現在保留中のメッセージ数 */
   unsigned int msgnum_peek;  /* 最大保留したメッセージ数 */
   unsigned int msgnum_total; /* 総送信メッセージ数   */
   unsigned int msgnum_ng;    /* QFULLによる送信NGメッセージ数 */
};
struct itcm_msgq_statis_t {
   unsigned int qnum;
   struct itcm_msgq_statis_info_t statis_info[];
};


/* スレッド間通信用 */
typedef struct {
    unsigned int it_header[8];
    void*  msg_p;
} t_bpf_ru_itcm_msgq_expand;

/******************************
	ITCM SG  
*******************************/

/* セマフォ関連SG */
typedef struct {
    int itcm_sem_max;		/* Max Use Num */
} t_bpf_ru_itcm_sg_data_sem;

/* メッセージ関連SG */
typedef struct {
	int maxqlen;
	int maxmsglen;
} t_bpf_ru_itcm_sg_msgq_info;

typedef struct {
    int itcm_msg_qnum;										/* msgq num         */
    t_bpf_ru_itcm_sg_msgq_info info[ITCM_QUEUE_NUM_MAX]; 	/* SG area Max 32  */
} t_bpf_ru_itcm_sg_data_msgq;

typedef struct {
	t_bpf_ru_itcm_sg_data_sem sem_info;
	t_bpf_ru_itcm_sg_data_msgq msgq_info;
} t_bpf_ru_itcm_sg_data_info;

typedef struct {
	t_bpf_ru_itcm_sg_data_info sg_info[BPF_COM_PNUM]; 
} t_bpf_ru_itcm_sg_data;

/* For BPF Fixed Value */
#define BPF_RU_ITCM_QNUM_MW 4
#define BPF_RU_ITCM_QMAX_SZ_MW 32
#define BPF_RU_ITCM_QMAX_NM_MW 100 



/* extern */
extern struct itcm_sem_tbl_t itcm_sem;
extern struct itcm_msg_tbl_t itcm_msg;
extern struct itcm_msg_data_t itcm_msg_flist;

extern t_bpf_ru_itcm_sg_data* bpf_ru_itcm_sg_data_p;
extern struct itcm_msgq_statis_t *itcm_msgq_statis_p;
extern t_bpf_ru_itcm_sg_data sg;

/* Function Proto */
//int bpf_ru_itcm_sem_init();

/* __USE_XOPEN2K を定義しないといけないようだが、いろいろエラーが出るので */
/* 個別に定義する                                                         */
extern int pthread_mutex_timedlock (pthread_mutex_t *__restrict __mutex,
                                    __const struct timespec *__restrict
                                    __abstime) __THROW;

#endif /* _BPF_I_RU_ITCM_H_ */

