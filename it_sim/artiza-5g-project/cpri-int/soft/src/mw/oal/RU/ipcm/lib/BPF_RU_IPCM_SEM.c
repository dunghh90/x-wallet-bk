/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file   BPF_RU_IPCM_SEM.c
 *	@brief	Inter Process Communication Management Library(SEM).
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_IPCM /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "bpf_l_com.h"
#include "bpf_i_ru_ipcm_sem.h"

extern int semtimedop (int semid, struct sembuf *sops, size_t nsops,
                       const struct timespec *timeout);

/** @addtogroup BPF_RU_IPCM
 * @{ */

/* セマフォタイプの範囲チェック */
#define IPCM_SEM_TYPECHECK(type)      \
  do { \
    if( type >= IPCM_SEM_TYPE_END ){  \
      IPCM_DBGLOG_FMT_ERR(\
          "semafo type range NG. semafo type = %d", type);  \
      return BPF_RU_IPCM_EPARAM;         \
    }                                 \
  } while(0)


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Semaphore log.
 *  @note   internal function.
 *  @param  semID        [in] semaphore id
 *  @param  kind         [in] kind
 *  @return none
 *  @date   2013/11/19 ALPHA)yokoyama
 */
/********************************************************************************************************************/
VOID bpf_ru_ipcm_procsem_log_get( unsigned int semID, unsigned int kind )
{
	T_RRH_SYSTEM_TIME		tDateTime;
	T_RRH_LOG_SEM_HIS*		semaphoreHistoryLog;
	int						iRetCode;
	int						iErrCode;
	unsigned int			logIndex;

	/* 時間取得 */
	(VOID)BPF_HM_DEVC_GETTIME(&tDateTime);

	/* 共有メモリのセマフォ取得解放履歴を取得する	*/
	/************************************************************/
	/* Get shared memory										*/
	/************************************************************/
	iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_SEMAPHORE_HISTORY,
		(VOID **)&semaphoreHistoryLog,
		&iErrCode
	);

	if( D_RRH_OK != iRetCode )
	{
		return;
	}

	/* Index取得	*/
	logIndex = semaphoreHistoryLog->uiWriteIdx;

	/* 共有メモリの情報を更新	*/
	/* Set timestamp			*/
	semaphoreHistoryLog->tRecord[logIndex].tDate.year	= tDateTime.year;
	semaphoreHistoryLog->tRecord[logIndex].tDate.month	= tDateTime.month;
	semaphoreHistoryLog->tRecord[logIndex].tDate.day	= tDateTime.day;
	semaphoreHistoryLog->tRecord[logIndex].tDate.hour	= tDateTime.hour;
	semaphoreHistoryLog->tRecord[logIndex].tDate.min	= tDateTime.min;
	semaphoreHistoryLog->tRecord[logIndex].tDate.sec	= tDateTime.sec;
	semaphoreHistoryLog->tRecord[logIndex].tDate.msec	= tDateTime.msec;
	semaphoreHistoryLog->tRecord[logIndex].tDate.ucount	= tDateTime.ucount;

	// セマフォIDと種別を取得
	semaphoreHistoryLog->tRecord[logIndex].semID = semID;
	semaphoreHistoryLog->tRecord[logIndex].kind = kind;

	/* Indexインクリメント	*/
	logIndex++;

	/* サイクリック判定	*/
	if( logIndex >=  D_RRH_LOG_SEMAPHORE_RCD_NUM )
	{
		/* Indexを0に戻す	*/
		semaphoreHistoryLog->uiWriteIdx = 0;
		/* サqイクリックフラグONする	*/
		semaphoreHistoryLog->uiCycleflg = D_RRH_ON;
	}else{
		semaphoreHistoryLog->uiWriteIdx = logIndex;
	}
	return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Semaphore Take.
 *  @note   API function.
 *  @param  sem_num      [in]  semaphore number
 *  @param  sem_rwlock   [in]  R/Wロック種別
 *                                 BPF_RU_IPCM_LOCK_RW
 *                                 BPF_RU_IPCM_LOCK_R
 *  @param  sem_flag     [in]  wait flag
 *                                 BPF_RU_IPCM_WAIT
 *                                 BPF_RU_IPCM_NOWAIT
 *  @param  timeout      [in]  timer value
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_IPCM_OK           normal end
 *  @retval BPF_RU_IPCM_NG           abnormal end
 *  @retval BPF_RU_IPCM_EPARAM       parameter error
 *  @retval BPF_RU_IPCM_ENOSEMNUM    no semaphore number error
 *  @retval BPF_RU_IPCM_ENOSEMGET    semaphore get error
 *  @retval BPF_RU_IPCM_ETIMEOUT     timeout error
 *  @retval BPF_RU_IPCM_EINTR        signal interupt error
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int BPF_RU_IPCM_PROCSEM_TAKE(unsigned int sem_num, int sem_rwlock, int sem_flag,
                          struct bpf_timespec *timeout, int *errcd)
{
    int ret;
    /***********************************************************
    * semaphore give processing.(call internal routine)        *
    ***********************************************************/
/* RRH-001 MD alpha)matsuhashi add start */
    ret = bpf_ru_ipcm_procsem_take2(IPCM_SEM_USER, sem_num, sem_rwlock, sem_flag, timeout, errcd);
/* RRH-001 MD alpha)matsuhashi add end */

	bpf_ru_ipcm_procsem_log_get(sem_num, D_RRH_LOG_SEMAPHOREKIND_TAKE);

    return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Semaphore Take2.
 *  @note   BPF internal function.
 *  @param  sem_type     [in]  semaphore type
 *  @param  sem_num      [in]  semaphore number
 *  @param  sem_rwlock   [in]  R/Wロック種別
 *                                 BPF_RU_IPCM_LOCK_RW
 *                                 BPF_RU_IPCM_LOCK_R
 *  @param  sem_flag     [in]  wait flag
 *                                 BPF_RU_IPCM_WAIT
 *                                 BPF_RU_IPCM_NOWAIT
 *  @param  timeout      [in]  timer value
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_IPCM_OK           normal end
 *  @retval BPF_RU_IPCM_NG           abnormal end
 *  @retval BPF_RU_IPCM_EPARAM       parameter error
 *  @retval BPF_RU_IPCM_ENOSEMNUM    no semaphore number error
 *  @retval BPF_RU_IPCM_ENOSEMGET    semaphore get error
 *  @retval BPF_RU_IPCM_ETIMEOUT     timeout error
 *  @retval BPF_RU_IPCM_EINTR        signal interupt error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_procsem_take2(enum ipcm_sem_type sem_type, unsigned int sem_num, int sem_rwlock,
			   int sem_flag, struct bpf_timespec *timeout,
			   int *errcd)
{
    int ret, semid, used;
    bpf_ru_ipcm_semlock_count_t semlock_cnt;
    unsigned int nops = 1;   /* nopsはセマフォ命令の数なので1固定 */
    struct sembuf sb;
    short sem_op;            /* semapho operetion value */

    /********************************************
    * parameter check processing.               *
    ********************************************/
    /* パラメータチェック */
    /* セマフォタイプのチェック(NGの時はここでBPF_RU_IPCM_EPARAMを返す) */
    IPCM_SEM_TYPECHECK(sem_type);
    /* セマフォ番号チェック */
    if( bpf_ru_ipcm_sem_numcheck(sem_type, sem_num) != BPF_RU_IPCM_OK ){
        IPCM_DBGLOG_FMT_ERR(
    	"semafo number check NG. semafo type = %d, num = %u",
    	sem_type, sem_num);
        return BPF_RU_IPCM_EPARAM;
    }
    /* フラグチェック */
    if( sem_flag != BPF_RU_IPCM_NOWAIT &&
        sem_flag != BPF_RU_IPCM_WAIT ){
        IPCM_DBGLOG_FMT_ERR(
            "wait flag check NG. flag = %d", sem_flag);
        return BPF_RU_IPCM_EPARAM;
    }

    /* R/Wロック種別チェック */
    if( sem_rwlock != BPF_RU_IPCM_LOCK_RW &&
        sem_rwlock != BPF_RU_IPCM_LOCK_R ){
        IPCM_DBGLOG_FMT_ERR(
            "read wite flag check NG. flag = %d", sem_rwlock);
        return BPF_RU_IPCM_EPARAM;
    }

    /********************************************
    * semaphore check processing.               *
    ********************************************/
    /* 管理データ読み出し */
    bpf_ru_ipcm_sem_read_data(sem_type, sem_num, &semid, &used, &semlock_cnt);
    if( semid == IPCM_SEM_NOCREATE ||
        used == IPCM_SEM_UNUSED ){
        IPCM_DBGLOG_FMT_ERR(
            "semafo is unused. semafo type = %d, num = %d",
            sem_type, sem_num);
        return BPF_RU_IPCM_ENOSEMNUM;    /* セマフォが生成されてなかった */
    }

    /* semapho operation value */
    if( sem_rwlock == BPF_RU_IPCM_LOCK_RW ){
        /* Read/Write lock */
        sem_op = (-1)*BPF_RU_IPCM_SEMVAL_MAX;
    }else{
        /* Read lock */
        sem_op = -1;
    }

    /********************************************
    * semaphore lock processing.                *
    ********************************************/
    /* セマフォを取得する */
    sb.sem_num = sem_num;
    sb.sem_op = sem_op;
    sb.sem_flg = SEM_UNDO;

    if( sem_flag == BPF_RU_IPCM_NOWAIT ){ /* 一回やってみるだけ */
        sb.sem_flg = sb.sem_flg | IPC_NOWAIT;
        /* セマフォ取得実行 */
        ret = semop(semid, &sb, nops);
    } else { /* ウェイトあり(BPF_RU_IPCM_WAIT)  */
        /* 時間待ちのあるsemtimedopを使用する */
        ret = semtimedop(semid, &sb, nops, (const struct timespec *)timeout);
    }
    *errcd = errno;
    if(ret == IPCM_SEMOP_OK){ /* semop, semtimedopが正常終了した場合 */
        /* セマフォlock数を+1する */
        bpf_ru_ipcm_sem_write_locknum_inc(sem_type, sem_num, sem_rwlock);
        return BPF_RU_IPCM_OK;
    } else if( *errcd == EAGAIN ){
        if( sem_flag == BPF_RU_IPCM_NOWAIT ){
            /* NOWAITでEAGAINの場合は他の誰かがロックしていたことになる */
            IPCM_DBGLOG_FMT_ERR(
                "semop failed. semaphore type = %d, num = %d, ret = %d, errno = %d",
                sem_type, sem_num, ret, *errcd);
            return BPF_RU_IPCM_ENOSEMGET;
        } else {
            /* WAIT で EAGAINで復帰したならタイムアウト発生 */
            IPCM_DBGLOG_FMT_ERR(
                "semop failed. semaphore type = %d, num = %d, ret = %d, errno = %d",
                sem_type, sem_num, ret, *errcd);
            return BPF_RU_IPCM_ETIMEOUT;
        }
    } else if( *errcd == EINTR ){
        IPCM_DBGLOG_FMT_ERR(
            "semop failed. semaphore type = %d, num = %d, ret = %d, errno = %d",
            sem_type, sem_num, ret, *errcd);
        return BPF_RU_IPCM_EINTR;
    }
    /* それ以外はなんらんかのNG発生 */
    IPCM_DBGLOG_FMT_ERR(
        "semop failed. semaphore type = %d, num = %d, ret = %d, errno = %d",
        sem_type, sem_num, ret, *errcd);
    return BPF_RU_IPCM_NG;

}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Semaphore Give.
 *  @note   API function.
 *  @param  sem_num      [in]  semaphore number
 *  @param  sem_rwlock   [in]  R/Wロック種別
 *                                 BPF_RU_IPCM_LOCK_RW
 *                                 BPF_RU_IPCM_LOCK_R
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_IPCM_OK           normal end
 *  @retval BPF_RU_IPCM_NG           abnormal end
 *  @retval BPF_RU_IPCM_EPARAM       parameter error
 *  @retval BPF_RU_IPCM_ENOSEMNUM    no semaphore number error
 *  @retval BPF_RU_IPCM_ENOLOCKED    no lock error
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int BPF_RU_IPCM_PROCSEM_GIVE(unsigned int sem_num, int sem_rwlock, int *errcd)
{
    int ret;

	bpf_ru_ipcm_procsem_log_get(sem_num, D_RRH_LOG_SEMAPHOREKIND_GIVE);
    /***********************************************************
    * semaphore give processing.(call internal routine)        *
    ***********************************************************/
/* RRH-001 MD alpha)matsuhashi add start */
    ret = bpf_ru_ipcm_procsem_give2(IPCM_SEM_USER, sem_num, sem_rwlock, errcd);
/* RRH-001 MD alpha)matsuhashi add end */

    return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Semaphore Give2.
 *  @note   BPF internal function.
 *  @param  sem_type     [in]  semaphore type
 *  @param  sem_num      [in]  semaphore number
 *  @param  sem_rwlock   [in]  R/Wロック種別
 *                                 BPF_RU_IPCM_LOCK_RW
 *                                 BPF_RU_IPCM_LOCK_R
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_IPCM_OK           normal end
 *  @retval BPF_RU_IPCM_NG           abnormal end
 *  @retval BPF_RU_IPCM_EPARAM       parameter error
 *  @retval BPF_RU_IPCM_ENOSEMNUM    no semaphore number error
 *  @retval BPF_RU_IPCM_ENOLOCKED    no lock error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_procsem_give2(enum ipcm_sem_type sem_type, unsigned int sem_num, int sem_rwlock,
			   int *errcd)
{
    int ret, semid, used;
    bpf_ru_ipcm_semlock_count_t semlock_cnt;
    unsigned int nops = 1;
    struct sembuf sb;
    short sem_op;            /* semapho operetion value */

    /********************************************
    * parameter check processing.               *
    ********************************************/
    /* パラメータチェック */
    /* セマフォタイプのチェック(NGの時はここでBPF_RU_IPCM_EPARAMを返す) */
    IPCM_SEM_TYPECHECK(sem_type);
    /* セマフォ番号チェック */
    if( bpf_ru_ipcm_sem_numcheck(sem_type, sem_num) != BPF_RU_IPCM_OK ){
        IPCM_DBGLOG_FMT_ERR(
            "semaphore number check NG. semaphore type = %d, num = %u",
            sem_type, sem_num);
        return BPF_RU_IPCM_EPARAM;
    }

    /* R/Wロック種別チェック */
    if( sem_rwlock != BPF_RU_IPCM_LOCK_RW &&
        sem_rwlock != BPF_RU_IPCM_LOCK_R ){
        IPCM_DBGLOG_FMT_ERR(
            "read wite flag check NG. flag = %d", sem_rwlock);
        return BPF_RU_IPCM_EPARAM;
    }

    /********************************************
    * semaphore check processing.               *
    ********************************************/
    /* 管理データ読み込み */
    bpf_ru_ipcm_sem_read_data(sem_type, sem_num, &semid, &used, &semlock_cnt);
    if( semid == IPCM_SEM_NOCREATE ||
        used == IPCM_SEM_UNUSED ){
        IPCM_DBGLOG_FMT_ERR(
            "semaphore is unused. semaphore type = %d, num = %d",
            sem_type, sem_num);
        return BPF_RU_IPCM_ENOSEMNUM;   /* セマフォが生成されてなかった */
    }

    /* semapho operation value */
    if( sem_rwlock == BPF_RU_IPCM_LOCK_RW ){
        /* Read/Write lock */
        sem_op = BPF_RU_IPCM_SEMVAL_MAX;
    }else{
        /* Read lock */
        sem_op = 1;
    }

    /* セマフォ取得有無についてはわからないため無条件に解放処理を実施 */
    /********************************************
    * semaphore unlock processing.              *
    ********************************************/
    sb.sem_num = sem_num;
    sb.sem_op = sem_op;
    sb.sem_flg = SEM_UNDO;
    /* 解除する前にlock数を-1する */
    bpf_ru_ipcm_sem_write_locknum_dec(sem_type, sem_num, sem_rwlock);
    /* セマフォを解除する */
    ret = semop(semid, &sb, nops);
    *errcd = errno;
    if(ret == IPCM_SEMOP_OK){  /* セマフォ操作成功 */
        return BPF_RU_IPCM_OK;
    }
    /* semopが正の数の時に失敗するのはパーミッションが無い場合のみなので */
    /* 通常は成功する */
    IPCM_DBGLOG_FMT_ERR(
        "semop failed. semaphore type = %d, num = %d, ret = %d, errno = %d",
         sem_type, sem_num, ret, *errcd);
    return BPF_RU_IPCM_NG;

}
/* @} */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Semaphore Initialize.
 *  @note   BPF internal function.
 *          指定されたタイプのセマフォを設定されたセマフォ集合数で作成する
 *  @param  sem_type     [in]  semaphore type
 *                                 IPCM_SEM_MDL
 *                                 IPCM_SEM_USER
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_IPCM_OK           normal end
 *  @retval BPF_RU_IPCM_NG           abnormal end
 *  @retval BPF_RU_IPCM_EPARAM       parameter error
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_procsem_init(enum ipcm_sem_type sem_type, int *errcd)
{
    enum ipcm_sem_type type;
    int ret, max, key, flg, semid, sem_num;
    struct semid_ds sds;
    union semun {
        unsigned short *array;
    }su;

    /********************************************
    * parameter check processing.               *
    ********************************************/
    /* パラメータチェック */
    /* セマフォタイプのチェック(NGの時はここでBPF_RU_IPCM_EPARAMを返す) */
    IPCM_SEM_TYPECHECK(sem_type);
    /* 情報読み込み */
    bpf_ru_ipcm_sem_read_tbl(sem_type, &type, &max, &key, &flg);
    /* セマフォ作成  */

    semid = semget( key, max, flg);
    *errcd = errno;
    if( semid == IPCM_SEM_NOCREATE ){
        /* セマフォ作成失敗 */
        IPCM_DBGLOG_FMT_ERR(
    	"semget failed. key = %08x, max = %d, flg = %d, errno = %d",
            key, max, flg, *errcd);
        return BPF_RU_IPCM_NG;
    }

    /* セマフォ情報確認 */
    ret = semctl(semid, 0, IPC_STAT, &sds);
    *errcd = errno;
    if( ret == IPCM_SEMCTL_NG ){ /* 情報取得失敗 */
        IPCM_DBGLOG_FMT_ERR(
    	"semctl(IPC_STAT) failed. semid = %d, errno = %d",
            semid, *errcd);
        return BPF_RU_IPCM_NG;
    }

    /********************************************
    * semaphore initialize processing.          *
    ********************************************/
    if( sds.sem_otime == 0 ){ /* otimeは最後のsemopの時間なので0なら未使用 */
        IPCM_DBGLOG_FMT_TRC(
    			"execute semaphore initialize.");
        /* だれも使ってないので初期化しとく */
        su.array = malloc(sizeof(unsigned short)*max);
        *errcd = errno;
        if( su.array == NULL ){
            /* メモリ確保失敗 */
            IPCM_DBGLOG_FMT_ERR(
                "malloc failed. errno = %d", *errcd);
            return BPF_RU_IPCM_NG;
      }
      for(sem_num = 0; sem_num < max; sem_num++){
          su.array[sem_num] = BPF_RU_IPCM_SEMVAL_MAX; /* 初期値設定 */
      }
      ret = semctl(semid, 0, SETALL, su);
      *errcd = errno;
      /* 不要になったメモリは開放しとく */
      free(su.array);
      /* semctlの結果判定 */
      if( ret == IPCM_SEMCTL_NG ){ /* 情報設定失敗 */
          IPCM_DBGLOG_FMT_ERR(
              "semctl(SETALL) failed. errno = %d", *errcd);
          return BPF_RU_IPCM_NG;
      }
    }
    /********************************************
    * management table update processing.       *
    ********************************************/
    /* 管理テーブルにsemidを登録しておく */
    bpf_ru_ipcm_sem_write_semid(sem_type, semid);
    return BPF_RU_IPCM_OK;

}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Print Inter Process Communication Management semaphore History.
 *  @note   internal function.
 *  @param  file dicriptor   [in] file discriptor
 *  @return none
 *  @date   2013/11/18 ALPHA)yokoyama
 */
/********************************************************************************************************************/
VOID BPF_RU_IPCM_PRINT_SEMAPHORE_HIST( FILE* fp )
{
	T_RRH_LOG_SEM_HIS*		semaphoreHistoryLog;
	int						iRetCode;
	int						iErrCode;
	unsigned int			logCount;
	unsigned int			logIndex;
	char* 					kindMessages[D_RRH_LOG_SEMAPHOREKIND_NUM] = { "TAKE", "GIVE" };

	/* 共有メモリのバッファ取得解放履歴を取得する	*/
	/************************************************************/
	/* Get shared memory										*/
	/************************************************************/
	iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_SEMAPHORE_HISTORY,
		(VOID **)&semaphoreHistoryLog,
		&iErrCode
	);

	if( D_RRH_OK != iRetCode )
	{
		return;
	}

	logIndex = ( semaphoreHistoryLog->uiWriteIdx + D_RRH_LOG_SEMAPHORE_RCD_NUM ) % D_RRH_LOG_SEMAPHORE_RCD_NUM ;

	fprintf(fp,"==================================================\n");
	fprintf(fp,"[xxxx] Date       Time              SemId    Kind\n");
	fprintf(fp,"--------------------------------------------------\n");
  	for( logCount = 0; logCount < D_RRH_LOG_SEMAPHORE_RCD_NUM; logCount++ )
	{
		if( semaphoreHistoryLog->tRecord[logIndex].tDate.year != 0 ) {
			/* 時刻表示	*/
			fprintf( fp, "[%04d] %04d/%02d/%02d %02d:%02d:%02d:%03d.%u  ",
					logIndex,
					semaphoreHistoryLog->tRecord[logIndex].tDate.year,
					semaphoreHistoryLog->tRecord[logIndex].tDate.month,
					semaphoreHistoryLog->tRecord[logIndex].tDate.day,
					semaphoreHistoryLog->tRecord[logIndex].tDate.hour,
					semaphoreHistoryLog->tRecord[logIndex].tDate.min,
					semaphoreHistoryLog->tRecord[logIndex].tDate.sec,
					semaphoreHistoryLog->tRecord[logIndex].tDate.msec*10,
					semaphoreHistoryLog->tRecord[logIndex].tDate.ucount
			);

			/* 詳細情報表示	*/
			fprintf( fp, "%08x %s\n",
					semaphoreHistoryLog->tRecord[logIndex].semID,
					kindMessages[semaphoreHistoryLog->tRecord[logIndex].kind]
			);
		}
		logIndex = ( logIndex + 1) % D_RRH_LOG_SEMAPHORE_RCD_NUM ;


	}

	return ;
}
/* @} */

/* @} */
