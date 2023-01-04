/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_i_ru_ipcm.h
 *  @brief  Definitions for BPF IPCM Internal
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_I_RU_IPCM_H_
#define _BPF_I_RU_IPCM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "bpf_l_ru_ipcm.h"
#include "BPF_COM_PLOG.h"

/********************************************************************************************************************/
/* 共通定義 */

/* マクロ定義 */
#define IPCM_DBGLOG_FMT_ERR(...) \
  do { \
    bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, __VA_ARGS__); \
  } while(0)
#define IPCM_DBGLOG_FMT_TRC(...) \
  do { \
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE, __VA_ARGS__); \
  } while(0)

/* メッセージキューのタイプ数．USER用とミドル用のふたつ */
#define IPCM_MSGQ_TYPE_NUM  2
/* プロセス間メッセージQueue数 */
#define IPCM_QUEUE_NUM_MAX  256

/* プロセス間メッセージQueue長 */
#define IPCM_QUEUE_MSGSIZE  128

/* プロセス間セマフォ数 */
#define IPCM_SEM_NUM_MAX     200

/* 共有メモリサイズ */
#define IPCM_SHM_SIZE_4K    0x1000
#define IPCM_SHM_SIZE_256K  0x40000
#define IPCM_SHM_SIZE_512K  0x80000
#define IPCM_SHM_SIZE_38M   0x2600000

/* 共有メモリのタイプ．APL用とPF用のふたつ */
#define IPCM_SHM_TYPE_NUM 2

/* 共有メモリ数 */
/* 2021/02/18 M&C)Tri.hn Update for intermediate device - Add start */
// #define IPCM_SHM_NUM_MAX  64 + 1
#define IPCM_SHM_NUM_MAX  E_RRH_SHMID_APL_MAX
// 2021/02/18 M&C)Tri.hn Update for intermediate device - Add end */

/* 共有メモリ管理用(生値)	*/
#define IPCM_SHM_MNG_TBL_ID				0x10000000				/* ID			*/
#define IPCM_SHM_MNG_TBL_SIZE			0x1000					/* TableSize	*/
#define IPCM_SHM_MNG_TBL_AUTH			0666					/* 権限			*/

/* セマフォタイプの数．APL用とPF用でふたつ */
#define IPCM_SEM_TYPE_NUM 2


/* IPCキーのマスク定義 */
#define IPCM_KEYMASK_USER     BPF_COM_IPCKEY_MASK_IPCM_USER
#define IPCM_KEYMASK_DUMMY    BPF_COM_IPCKEY_MASK_IPCM_DUMMY

/* システムコール終了コード定義 */
/* semopとsemtimedopの返り値は同じ */
#define IPCM_SEMOP_OK 0
#define IPCM_SEMOP_NG -1
#define IPCM_SEMCTL_NG -1
#define IPCM_MSGGET_NG -1
#define IPCM_MSGSND_OK 0
#define IPCM_MSGSND_NG -1
#define IPCM_MSGRCV_NG -1
#define IPCM_MSGCTL_NG -1
#define IPCM_SHMGET_NG -1
#define IPCM_SHMCTL_NG -1
#define IPCM_SHMAT_NG (void *)-1
#define IPCM_SHMDT_NG -1
#define IPCM_MUTEX_OK 0

/* ON/OFF設定 */
#define BPF_RU_IPCM_FLG_ON   1
#define BPF_RU_IPCM_FLG_OFF  0

/* 通信用共有メモリ取得解放種別(ログ用)	*/
#define BPF_RU_IPCM_SHMBUF_GET	0
#define BPF_RU_IPCM_SHMBUF_FREE	1

/* 構造体宣言 */

typedef struct {
	int size;
	int rep_flg;
} bpf_ru_ipcm_sg_shmsize_t;

/*!
 * @name 通信用共有メモリ管理テーブル
 * @note 通信用共有メモリ管理テーブル構造体
 * @{
 */
#define	BPF_RU_IPCM_SHMID_COMU_OCCUR		 921		/* 送信用共有メモリ閾値超え(MAXの60%)	*/
#define	BPF_RU_IPCM_SHMID_COMU_CLEAR		 911		/* 送信用共有メモリ閾値超えから復旧		*/
#define	BPF_RU_IPCM_SHMID_COMU_MAX			1536		/* 送信用共有メモリの1種別あたりのMAX数	*/

typedef struct{
	unsigned int		keyID;				/**< KEYID			*/
	unsigned int		state;				/**< 使用有無		*/
}BPF_MNG_COM_MEMORY_INF;

typedef struct{
	unsigned int					useCount;			/**< 使用数			*/
	BPF_MNG_COM_MEMORY_INF			com_mem_inf[BPF_RU_IPCM_SHMID_COMU_MAX];		/**< 種別毎共有メモリ数		*/
}BPF_MNG_COM_MEMORY;
/* @} */

/* SG定義 */
typedef struct {
    /* queue */
    int que_max;                      /* キュー数       */
    int que_top_no;                   /* キュー開始番号 */
    int que_size[IPCM_QUEUE_NUM_MAX]; /* キューサイズ   */
    /* semaphor */
    int sem_max;                      /* セマフォ数       */
    int sem_top_no;                   /* セマフォ開始番号 */
    /* shared memory */
    int shm_max;                      /* 共有メモリ数       */
    int shm_top_no;                   /* 共有メモリ開始番号 */
    bpf_ru_ipcm_sg_shmsize_t shm_size[IPCM_SHM_NUM_MAX];    /* 共有メモリサイズ */
    void *shm_topaddr;                /* 共有メモリ割付仮想アドレス先頭 */
} bpf_ru_ipcm_sg_data_t;

/* Function prototype */
int bpf_ru_ipcm_sem_init(unsigned int);
int bpf_ru_ipcm_msgq_init(unsigned int);
int bpf_ru_ipcm_shm_init(unsigned int);
int bpf_ru_ipcm_sem_uninit(unsigned int);
int bpf_ru_ipcm_msgq_uninit(unsigned int);
int bpf_ru_ipcm_shm_uninit(unsigned int);
int bpf_ru_ipcm_shm_set_table(unsigned int, unsigned int, T_RRH_SYS_CREATE_SHMMEM*);
void bpf_ru_ipcm_real_time_log_get(unsigned int, unsigned int, unsigned int, unsigned int, const void*);

#endif /* _BPF_I_RU_IPCM_H_ */

