/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	bpf_ru_ipcm_sem_mng.c
 *	@brief	Inter Process Communication Management internal Library(SEM).
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_IPCM /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_l_com.h"
#include "bpf_i_ru_ipcm_sem.h"

/** @addtogroup BPF_RU_IPCM
 * @{ */

/* セマフォの管理テーブル */
/* ロックは0～IPCM_SEM_MAX-1を使う */ 
/* semidを-1で初期値を設定し残りは全部0にする */
struct ipcm_sem_tbl_t {
  enum ipcm_sem_type type;     /* セマフォタイプ(ミドルorユーザ) */
  int max;         /* 最大セマフォ数 */
  int keynum;      /* セマフォ番号(0固定) */
  int keymask;     /* IPCキーマスク */
  int flg;         /* セマフォ作成時のフラグ */
  int semid;       /* セマフォ識別子 */
  enum ipcm_sem_used   *used;   /* セマフォ使用中のフラグ */
  bpf_ru_ipcm_semlock_count_t *semlock_cnt; /* セマフォロックカウンタ */
  pthread_mutex_t sem;          /* 管理テーブル用 mutex */
};

static struct ipcm_sem_tbl_t bpf_ru_ipcm_sem[IPCM_SEM_TYPE_NUM] = {
/* RRH-001 MD alpha)matsuhashi add start */
  { /* ユーザー用セマフォ 設定 */
    IPCM_SEM_USER,      /* type */
    0,                 /* max */
    IPCM_SEM_AKEYNUM,  /* keynum */
    IPCM_KEYMASK_USER, /* keymask */
    IPCM_SEMFLG,       /* flg */
    IPCM_SEM_NOCREATE, /* semid */
    NULL,              /* used */
    NULL,              /* semlock_cnt */
    PTHREAD_MUTEX_INITIALIZER    /* sem */
  },
  { /* セマフォ設定(未使用) */
    IPCM_SEM_DUMMY,    /* type */
    0,                 /* max */
    IPCM_SEM_PKEYNUM,  /* keynum */
    IPCM_KEYMASK_DUMMY,/* keymask */
    IPCM_SEMFLG,       /* flg */
    IPCM_SEM_NOCREATE, /* semid */
    NULL,              /* used */
    NULL,              /* semlock_cnt */
    PTHREAD_MUTEX_INITIALIZER    /* sem */
  }
/* RRH-001 MD alpha)matsuhashi add end */
};


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Semaphore Number Check.
 *  @note   IPCM internal function. 
 *  @param  sem_type     [in]  semaphore type
 *                                 IPCM_SEM_MDL
 *                                 IPCM_SEM_USER
 *  @param  sem_num      [in]  semaphore number
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_sem_numcheck(enum ipcm_sem_type sem_type, unsigned int sem_num)
{
    /****************************************************
    * semaphore number check processing.                *
    ****************************************************/
    /* 範囲チェック．タイプに応じた最大数と比較する */
    if( sem_num >= bpf_ru_ipcm_sem[sem_type].max ){
        return BPF_RU_IPCM_NG;
    }
    return BPF_RU_IPCM_OK;
}
/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Semaphore Id Write.
 *  @note   IPCM internal function. 
 *                                 IPCM_SEM_MDL
 *                                 IPCM_SEM_USER
 *  @param  sem_type     [in]  semaphore type
 *  @param  semid        [in]  semaphore id
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_sem_write_semid(enum ipcm_sem_type sem_type, int semid)
{
    /****************************************************
    * semaphore id write processing.                    *
    ****************************************************/
    bpf_ru_ipcm_sem[sem_type].semid = semid;
    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Semaphore Lock Data Increment.
 *  @note   IPCM internal function. 
 *  @param  sem_type     [in]  semaphore type
 *                                 IPCM_SEM_MDL
 *                                 IPCM_SEM_USER
 *  @param  sem_num      [in]  semaphore number
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_sem_write_locknum_clear(enum ipcm_sem_type sem_type,
			      unsigned int sem_num)
{
    /****************************************************
    * semaphore data write processing.(lock conter)     *
    ****************************************************/
    bpf_ru_ipcm_sem[sem_type].semlock_cnt[sem_num].rw_lock_count=0;
    bpf_ru_ipcm_sem[sem_type].semlock_cnt[sem_num].r_lock_count=0;

    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Semaphore Lock Data Increment.
 *  @note   IPCM internal function. 
 *  @param  sem_type     [in]  semaphore type
 *                                 IPCM_SEM_MDL
 *                                 IPCM_SEM_USER
 *  @param  sem_num      [in]  semaphore number
 *  @param  sem_rwlock   [in]  R/Wロック種別
 *                                 BPF_RU_IPCM_LOCK_RW
 *                                 BPF_RU_IPCM_LOCK_R
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_sem_write_locknum_inc(enum ipcm_sem_type sem_type,
			      unsigned int sem_num, int sem_rwlock)
{
    /****************************************************
    * semaphore data write processing.(lock conter)     *
    ****************************************************/
    if(sem_rwlock == BPF_RU_IPCM_LOCK_RW){

        bpf_ru_ipcm_sem[sem_type].semlock_cnt[sem_num].rw_lock_count++;
    }else if(sem_rwlock == BPF_RU_IPCM_LOCK_R){

        bpf_ru_ipcm_sem[sem_type].semlock_cnt[sem_num].r_lock_count++;
    }

    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Semaphore Lock Data Decrement.
 *  @note   IPCM internal function. 
 *  @param  sem_type     [in]  semaphore type
 *                                 IPCM_SEM_MDL
 *                                 IPCM_SEM_USER
 *  @param  sem_num      [in]  semaphore number
 *  @param  sem_rwlock   [in]  R/Wロック種別
 *                                 BPF_RU_IPCM_LOCK_RW
 *                                 BPF_RU_IPCM_LOCK_R
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_sem_write_locknum_dec(enum ipcm_sem_type sem_type,
			      unsigned int sem_num, int sem_rwlock)
{
    /****************************************************
    * semaphore data write processing.(lock conter)     *
    ****************************************************/
    if(sem_rwlock == BPF_RU_IPCM_LOCK_RW){

        bpf_ru_ipcm_sem[sem_type].semlock_cnt[sem_num].rw_lock_count--;
    }else if(sem_rwlock == BPF_RU_IPCM_LOCK_R){

        bpf_ru_ipcm_sem[sem_type].semlock_cnt[sem_num].r_lock_count--;
    }

    /* For Debug */
    /* 未取得のセマフォ解放の検出。read lockについては必ずしも検出できる
       わけではないが、検出した場合はログを収集しておく */
    if((bpf_ru_ipcm_sem[sem_type].semlock_cnt[sem_num].rw_lock_count < 0) ||
       (bpf_ru_ipcm_sem[sem_type].semlock_cnt[sem_num].r_lock_count < 0 )){

        /* Semaphore Lock Counter is minus !! */
        /* Maybe, unnecessary unlock api was called. It's bug? */
        IPCM_DBGLOG_FMT_ERR(
              "[BUG] Unexpected semaphore unlock detect!. type=%d num=%d rwlock=%d rw=%d r=%d", 
              sem_type, sem_num, sem_rwlock,
              bpf_ru_ipcm_sem[sem_type].semlock_cnt[sem_num].rw_lock_count,
              bpf_ru_ipcm_sem[sem_type].semlock_cnt[sem_num].r_lock_count);
    }

    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Semaphore Use Data Write.
 *  @note   IPCM internal function. 
 *  @param  sem_type     [in]  semaphore type
 *                                 IPCM_SEM_MDL
 *                                 IPCM_SEM_USER
 *  @param  sem_num      [in]  semaphore number
 *  @param  used         [in] use information
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_sem_write_used(enum ipcm_sem_type sem_type, 
			    unsigned int sem_num, int used)
{
    /****************************************************
    * semaphore data write processing.(use information) *
    ****************************************************/
    bpf_ru_ipcm_sem[sem_type].used[sem_num] = used;
    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Semaphore Data Read.
 *  @note   IPCM internal function. 
 *  @param  sem_type     [in]  semaphore type
 *                                 IPCM_SEM_MDL
 *                                 IPCM_SEM_USER
 *  @param  sem_num      [in]  semaphore number
 *  @param  semlock_cnt  [out] lock counter
 *  @param  used         [out] use information
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_sem_read_data(enum ipcm_sem_type sem_type, unsigned int sem_num,
			   int *semid, int *used, bpf_ru_ipcm_semlock_count_t *semlock_cnt)
{
    /********************************************
    * semaphore data read processing.           *
    ********************************************/
    *semid = bpf_ru_ipcm_sem[sem_type].semid;
    *used = bpf_ru_ipcm_sem[sem_type].used[sem_num];
    *semlock_cnt = bpf_ru_ipcm_sem[sem_type].semlock_cnt[sem_num];
    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Semaphore Table Read.
 *  @note   IPCM internal function. 
 *  @param  sem_type     [in]  semaphore type
 *                                 IPCM_SEM_MDL
 *                                 IPCM_SEM_USER
 *  @param  type         [out] semaphore type
 *  @param  max          [out] max semaphore number
 *  @param  key          [out] IPC key
 *  @param  flg          [out] semaphore create flag
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_sem_read_tbl(enum ipcm_sem_type sem_type, 
			  enum ipcm_sem_type *type, 
			   int *max, int *key, int *flg)
{
    /********************************************
    * semaphore table read processing.          *
    ********************************************/
    *type = bpf_ru_ipcm_sem[sem_type].type;
    *max = bpf_ru_ipcm_sem[sem_type].max;
    *key = bpf_ru_ipcm_sem[sem_type].keymask | bpf_ru_ipcm_sem[sem_type].keynum;
    *flg = bpf_ru_ipcm_sem[sem_type].flg;
    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Semaphore Function Initialize.
 *  @note   IPCM internal function. 
 *          ミドル用セマフォの管理テーブルとユーザ用セマフォの管理テーブルを
 *          初期化し、指定の値でセマフォを取得する
 *          Process descriptorによりMDL/USERのどちらかのみを初期化する
 *  @param  process_desc [in] process descriptor
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_sem_init(unsigned int process_desc)
{
    int errcd = 0, ret;
    unsigned int sem_num;
    struct{
        int    sg_num;
        bpf_ru_ipcm_sg_data_t *sgdata[3];
    } sg_proc_data;
    enum ipcm_sem_type sem_type;

    sg_proc_data.sg_num = BPF_COM_NUM0;
/* RRH-001 MD alpha)matsuhashi add start */
	bpf_ru_ipcm_sg_data_t sg_data;
/* RRH-001 MD alpha)matsuhashi add end */

    /************************************************
    * 初期化Semaphore Type の決定                   *
    ************************************************/
	(void)bpf_com_get_proc_tkind(process_desc);

/* RRH-001 MD alpha)matsuhashi add start */
    sem_type = IPCM_SEM_USER;

    sg_proc_data.sg_num++;

    /* プロセス間セマフォ数の設定 */
    sg_data.sem_max = IPCM_SEM_NUM_MAX;

    sg_data.sem_top_no = 0;     
    sg_proc_data.sgdata[BPF_COM_NUM0] = &sg_data;

    bpf_ru_ipcm_sem[sem_type].max = sg_proc_data.sgdata[BPF_COM_NUM0]->sem_max;
/* RRH-001 MD alpha)matsuhashi add end */

    /* Trace log */
    IPCM_DBGLOG_FMT_TRC(
        "SEM init. pdesc=%d type=%d max=%d", 
        process_desc, sem_type,
        bpf_ru_ipcm_sem[sem_type].max);

    /* セマフォ数=0の場合はここで終了 */
    if( bpf_ru_ipcm_sem[sem_type].max == 0 ){
        IPCM_DBGLOG_FMT_ERR(
          "Semaphor Num is 0. pdesc=%d q_type=%d", process_desc, sem_type);
		return BPF_RU_IPCM_OK;
	}


    /********************************************
    * semaphore table create processing.        *
    ********************************************/
    /* データ用領域確保 */
    bpf_ru_ipcm_sem[sem_type].semid = IPCM_SEM_NOCREATE;
    bpf_ru_ipcm_sem[sem_type].used = malloc(sizeof(enum ipcm_sem_used) * 
               bpf_ru_ipcm_sem[sem_type].max);
    bpf_ru_ipcm_sem[sem_type].semlock_cnt = malloc(sizeof(bpf_ru_ipcm_semlock_count_t) * 
                 bpf_ru_ipcm_sem[sem_type].max);
    errcd = errno;
    if( bpf_ru_ipcm_sem[sem_type].used == NULL || 
        bpf_ru_ipcm_sem[sem_type].semlock_cnt == NULL ){
      /* メモリ確保失敗 */
      IPCM_DBGLOG_FMT_ERR(
          "malloc failed. errno = %d", errcd);
      return BPF_RU_IPCM_NG;
    }
    pthread_mutex_init(&(bpf_ru_ipcm_sem[sem_type].sem), NULL);

    /********************************************
    * semaphore create processing.              *
    ********************************************/
    /* セマフォ作成 */
    if( (ret = bpf_ru_ipcm_procsem_init(sem_type, &errcd)) != BPF_RU_IPCM_OK ){

        IPCM_DBGLOG_FMT_ERR(
            "bpf_ru_ipcm_procsem_init failed. sem_type = %d, ret = %d, errno = %d",
             sem_type, ret, errcd);
        return BPF_RU_IPCM_NG;
    }
    /********************************************
    * semaphore table initialize processing.    *
    ********************************************/
    /* 管理テーブルを使用中、lock数=0に更新 */
    for( sem_num = 0; sem_num < bpf_ru_ipcm_sem[sem_type].max; sem_num++ ){
        bpf_ru_ipcm_sem_write_locknum_clear(sem_type, sem_num);
        bpf_ru_ipcm_sem_write_used(sem_type, sem_num, IPCM_SEM_USED);
    }
    return BPF_RU_IPCM_OK;

}
/* @} */

/* RRH-001 MD alpha)matsuhashi add start */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Semaphore Function UnInitialize.
 *  @note   IPCM internal function. 
 *  @param  process_desc [in] process descriptor
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_sem_uninit(unsigned int process_desc)
{
    enum ipcm_sem_type sem_type;

    sem_type = IPCM_SEM_USER;

    /* 捕捉したバッファを解放する */
    if (bpf_ru_ipcm_sem[sem_type].used != NULL)
    {
        free(bpf_ru_ipcm_sem[sem_type].used);
        bpf_ru_ipcm_sem[sem_type].used = NULL;
    }
    if (bpf_ru_ipcm_sem[sem_type].semlock_cnt != NULL)
    {
        free(bpf_ru_ipcm_sem[sem_type].semlock_cnt);
        bpf_ru_ipcm_sem[sem_type].semlock_cnt = NULL;
    }

    return BPF_RU_IPCM_OK;

}
/* @} */
/* RRH-001 MD alpha)matsuhashi add end */

/* @} */
