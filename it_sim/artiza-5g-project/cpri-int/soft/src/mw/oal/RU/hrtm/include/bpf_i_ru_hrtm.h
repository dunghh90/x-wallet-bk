/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_i_ru_hrtm.h
 *  @brief  Definitions for BPF HRTM Internal
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_I_RU_HRTM_H_
#define _BPF_I_RU_HRTM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <pthread.h>
#include "bpf_l_ru_hrtm.h"
#include "BPF_COM_PLOG.h"

#define HRTM_FLG_ON (1)
#define HRTM_FLG_OFF (0)


/* システムコールNG定義 */
#define HRTM_SYSCALL_NG (-1)

#define HRTMID_MIN 1

/* タイマ時間 */
#define MINTIME 10    /* 試作は10msec */
#define MAXTIME 0x7FFFFFFF

/* タイムアウト関数スタックサイズ */
#define TIMEOUT_THREAD_STACK (256*1024)  /* 256KB */

/* 共有メモリ関連 */
#define HRTM_KEYSHIFT 16 /* ユーザ識別シフトビット数 */

/* 使用中識別 */
#define HRTM_USING 0xFFFFFFFE
/* 最終識別 */
#define HRTM_LAST 0xEEEEEEEF

/* タイマ個数 */
#define HRTM_NUM 100

typedef struct{
	unsigned int				id_next;
	bpf_ru_hrtm_notify_type_t	type;
	bpf_ru_hrtm_key_inf_t		keyinf;
	unsigned int				time_val;
	void(*entry);
	timer_t						timerId;
	unsigned int				threadId;
	unsigned int				cancelFlg;
}t_bpf_ru_hrtm_tim_info;

typedef struct{
	unsigned int	process_desc;
	unsigned int	tim_max_id;
	key_t			tim_ipc_key;
	key_t			info_ipc_key;
	unsigned int	next;
	unsigned int	tail;
	unsigned int	usenum;
	timer_t			can_timerId;
	t_bpf_ru_hrtm_tim_info* info;
}t_bpf_ru_hrtm_tim_tbl;


/* SGデータテーブル */
typedef struct {
    unsigned int hrtm_max_use;		/* Max Use Num */
} t_bpf_ru_hrtm_sg_data_proc;

typedef struct {
	t_bpf_ru_hrtm_sg_data_proc sg_info[64]; 
} t_bpf_ru_hrtm_sg_data;


/* タイマ管理用構造体	*/
typedef struct {
	unsigned int			timerID;
	unsigned int			queueID;
	unsigned int			eventNo;
	unsigned int			periodic;
}T_RRH_TIMER_MNG;

extern t_bpf_ru_hrtm_tim_tbl* di_tim_tbl_p;
extern t_bpf_ru_hrtm_tim_info* di_tim_info_p;

/* mutex obj */
extern pthread_mutex_t bpf_ru_hrtm_mutexobj;
/* SGデータ */
extern t_bpf_ru_hrtm_sg_data *bpf_ru_hrtm_sg_data;

/* Assign flag */
extern unsigned int di_hrtm_assign_flag;

/* timer max val from SG */
extern unsigned int di_tim_max;

/* Function */
extern void bpf_ru_hrtm_init(unsigned int,key_t,key_t);
extern int bpf_ru_hrtm_get(unsigned int*);
extern int bpf_ru_hrtm_del(unsigned int);
extern void bpf_ru_hrtm_handler(sigval_t);
extern VOID bpf_ru_hrtm_timer_log_get(UINT,UINT,UINT,UINT);
	
#endif /* _BPF_I_RU_HRTM_H_ */

