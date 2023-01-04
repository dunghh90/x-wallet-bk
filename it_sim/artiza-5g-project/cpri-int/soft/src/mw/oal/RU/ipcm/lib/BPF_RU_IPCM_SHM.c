/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   f_BPF_RU_IPCM_SHM.c
 *  @brief  Inter Process Communication Management Library(SHM).
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_IPCM /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_l_com.h"
#include "bpf_i_ru_ipcm_shm.h"
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/mman.h>

/** @addtogroup BPF_RU_IPCM
 * @{ */

/* 共有メモリタイプの範囲チェック */
#define IPCM_SHM_TYPECHECK(type)      \
  do { \
    if( type >= IPCM_SHM_TYPE_END ){  \
      IPCM_DBGLOG_FMT_ERR(\
          "shm type range NG. shm type = %d", type);  \
      return BPF_RU_IPCM_EPARAM;         \
    }                                 \
  } while(0)

/* Buffer取得閾値超えの場合のCallBack関数	*/
void	(*buffer_full_occur_function_addr)(int);
/* Buffer取得閾値超えから回復した場合のCallBack関数	*/
void	(*buffer_full_clear_function_addr)(int);

static	unsigned int bpf_ru_ipcm_procshm_addr_log_dup_use[E_BPF_RU_IPCM_SHMKIND_MAX];
static	unsigned int bpf_ru_ipcm_procshm_addr_log_dup_free[E_BPF_RU_IPCM_SHMKIND_MAX];

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Shared Memory Address Get2.
 *  @note   API function.
 *  @param  shm_num      [in]  shared memory number
 *  @param  addr         [out  shared memory address
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_IPCM_OK           normal end
 *  @retval BPF_RU_IPCM_NG           abnormal end
 *  @retval BPF_RU_IPCM_EPARAM       parameter error
 *  @retval BPF_RU_IPCM_ENOSHMNUM    no shared memory number error
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int BPF_RU_IPCM_PROCSHM_ADDRGET(unsigned int shm_num, void **addr, int *errcd)
{
    int ret;
    /**************************************************************
    * shared memory address get processing.(call internal routine)*
    **************************************************************/
/* RRH-001 MD alpha)matsuhashi add start */
    ret = bpf_ru_ipcm_procshm_addrget2(IPCM_SHM_USER, shm_num, addr, errcd);
/* RRH-001 MD alpha)matsuhashi add end */
    return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Shared Memory Address Get2.
 *  @note   BPF internal function.
 *  @param  shm_type     [in]  shared memory type
 *  @param  shm_num      [in]  shared memory number
 *  @param  addr         [out  shared memory address
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_IPCM_OK           normal end
 *  @retval BPF_RU_IPCM_NG           abnormal end
 *  @retval BPF_RU_IPCM_EPARAM       parameter error
 *  @retval BPF_RU_IPCM_ENOSHMNUM    no shared memory number error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_procshm_addrget2(enum ipcm_shm_type shm_type,
                  unsigned int shm_num, void **addr, int *errcd)
{
    int shmid, ret, used, size;

    /********************************************
    * parameter check processing.               *
    ********************************************/

    /* パラメータチェック */
    /* 共有メモリタイプの範囲チェック(NG時はここでBPF_RU_IPCM_EPARAMを返す) */
    IPCM_SHM_TYPECHECK(shm_type);
    /* パラメータチェック(共有メモリ番号) */
    if( bpf_ru_ipcm_shm_numcheck(shm_type, shm_num) != BPF_RU_IPCM_OK ){
        IPCM_DBGLOG_FMT_ERR(
        "shm number check NG. shm type = %d, num = %u",
            shm_type, shm_num);
        return BPF_RU_IPCM_EPARAM;
    }

    /********************************************
    * management table read processing.         *
    ********************************************/
    /* addrに共有メモリアドレスが格納されている */
    bpf_ru_ipcm_shm_read_data(shm_type, shm_num, &size, &shmid, addr, &used);
    switch(used){
        case IPCM_SHM_UNUSED:
        case IPCM_SHM_DT:     /*未使用かデタッチ状態の時はエラー */
            IPCM_DBGLOG_FMT_ERR("shm status NG. shm type = %d, num = %u, used = %d",
              shm_type, shm_num, used);
            ret = BPF_RU_IPCM_ENOSHMNUM;
            break;
        case IPCM_SHM_AT:     /*アタッチ状態なので正常終了 */
            ret = BPF_RU_IPCM_OK;
            break;
        default:              /* それ以外の状態はない */
            IPCM_DBGLOG_FMT_ERR("shm status NG. shm type = %d, num = %u, used = %d",
              shm_type, shm_num, used);
            ret = BPF_RU_IPCM_NG;
    }
    return ret;

}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Shared Memory Create.
 *  @note   BPF internal function.
 *  @param  shm_type     [in]  shared memory type
 *  @param  shm_num      [in]  shared memory number
 *  @param  shmat_addr   [in]  shared memory attch address(If not set address, set NULL)
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_IPCM_OK           normal end
 *  @retval BPF_RU_IPCM_NG           abnormal end
 *  @retval BPF_RU_IPCM_EPARAM       parameter error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_procshm_create(enum ipcm_shm_type shm_type,
                unsigned int shm_num, char *shmat_addr, int *errcd)
{
    enum ipcm_shm_type type;
    int max, keymask, flg, atflg;
    int size, shmid, used;
    void *shmaddr, *daddr;

    /********************************************
    * parameter check processing.               *
    ********************************************/
    /* パラメータチェック */
    /* 共有メモリタイプの範囲チェック(NG時はここでBPF_RU_IPCM_EPARAMを返す) */
    IPCM_SHM_TYPECHECK(shm_type);
    /* パラメータチェック(共有メモリ番号) */
    if( bpf_ru_ipcm_shm_numcheck(shm_type, shm_num) != BPF_RU_IPCM_OK){
        IPCM_DBGLOG_FMT_ERR("shm number check NG. shm type = %d, num = %u",
            shm_type, shm_num);
        return BPF_RU_IPCM_EPARAM;
    }
    /* テーブル情報読み込み(keymaskとflgがほしい．) */
    bpf_ru_ipcm_shm_read_tbl(shm_type, &type, &max, &keymask, &flg, &atflg, &daddr);
    /* データ情報読み込み(サイズがほしい．shmidはDon't care) */
    bpf_ru_ipcm_shm_read_data(shm_type, shm_num, &size, &shmid, &shmaddr, &used);


    /* サイズ=0の場合はスキップ */
    if( size == 0 ){
/*        IPCM_DBGLOG_FMT_TRC("SHMAT size 0. shmtype=%d shm_num=%d", shm_type, shm_num);*/
        return BPF_RU_IPCM_OK;
    }


    /********************************************
    * shared memory create processing.          *
    ********************************************/
    /* 共有メモリ作成 */
    shmid = shmget( keymask | shm_num , size ,flg);
    *errcd = errno;
    /* 共有メモリ作成の結果がNGかどうかチェック */
    if( shmid == IPCM_SHMGET_NG ){
        IPCM_DBGLOG_FMT_ERR(
            "shmget failed. keymask = %08x, num = %u, size = %d, flg = %d, errno = %d",
            keymask, shm_num, size, flg, *errcd);
        return BPF_RU_IPCM_NG;
    }
    /********************************************
    * shared memory attach processing.          *
    ********************************************/
    /* 共有メモリアタッチ */
    shmaddr  = shmat(shmid, shmat_addr, 0);
    *errcd = errno;
    /* アタッチ失敗かどうかチェック */
    if( shmaddr == IPCM_SHMAT_NG ){ /* pgr0203 */
        /* 失敗しても共有メモリは残したままにしておく */
        IPCM_DBGLOG_FMT_ERR("shmat failed. shmid = %d, errno = %d",
            shmid, *errcd);
        return BPF_RU_IPCM_NG;
    }

    /********************************************
    * management table update processing.       *
    ********************************************/
    /* 管理データに情報を書き込む */
    bpf_ru_ipcm_shm_write_data(shm_type, shm_num, shmid, shmaddr, IPCM_SHM_AT);
    return BPF_RU_IPCM_OK;

}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Shared Memory Address Release.
 *  @note   API function.
 *  @param  addr         [out  shared memory address
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_IPCM_OK           normal end
 *  @retval BPF_RU_IPCM_NG           abnormal end
 *  @date   2013/10/17 ALPHA)samejid
 */
/********************************************************************************************************************/
int BPF_RU_IPCM_PROCSHM_ADDRREL(void *addr, int *errcd)
{
    /**************************************************************
    * shared memory address detach processing.(call internal routine)*
    **************************************************************/

	(void)shmdt(addr);
	if(addr == IPCM_SHMAT_NG) /* pgr0203 */
	{
		*errcd = errno;
        IPCM_DBGLOG_FMT_ERR("shmdt failed. , errno = %d",
             *errcd);
        return BPF_RU_IPCM_NG;
	}
	return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Shared Memory Address History.
 *  @note   internal function.
 *  @param  kind         [in] buffer get/free
 *  @param  key          [in] shared memory key
 *  @return none
 *  @date   2013/11/16 ALPHA)yokoyama
 */
/********************************************************************************************************************/
VOID bpf_ru_ipcm_procshm_addr_log( unsigned int kind, unsigned int key ,unsigned int bufkind , unsigned int useCount)
{
	switch(kind)
	{
		case BPF_RU_IPCM_SHMBUF_GET:
			if( bpf_ru_ipcm_procshm_addr_log_dup_use[bufkind] != useCount){
				BPF_COM_PLOG_RSC_BUF(E_BUF_LV_GET,key,bufkind,useCount,getpid());
			}
			else{
				BPF_COM_PLOG_RSC_BUF(E_BUF_LV_GET_LOW,key,bufkind,useCount,getpid());
			}
			bpf_ru_ipcm_procshm_addr_log_dup_use[bufkind] = useCount;
			break;
		case BPF_RU_IPCM_SHMBUF_FREE:
			if( bpf_ru_ipcm_procshm_addr_log_dup_free[bufkind] != useCount){
				BPF_COM_PLOG_RSC_BUF(E_BUF_LV_FREE,key,bufkind,useCount,getpid());
			}
			else{
				BPF_COM_PLOG_RSC_BUF(E_BUF_LV_FREE_LOW,key,bufkind,useCount,getpid());
			}
			bpf_ru_ipcm_procshm_addr_log_dup_free[bufkind] = useCount;
			break;
		default:
			break;
	}
	return ;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Communication Shared Memory Address Get.
 *  @note   API function.
 *  @param  kind         [in]  buffer kind
 *  @param  size         [in]  buffer size
 *  @param  addr         [out] shared memory address
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_IPCM_OK           normal end
 *  @retval BPF_RU_IPCM_NG           abnormal end
 *  @date   2013/11/07 ALPHA)samejid
 */
/********************************************************************************************************************/
int BPF_RU_IPCM_PROCMSG_ADDRGET(unsigned int kind, unsigned int size, void **addr, int *errcd)
{
	/* 通常の共有メモリは初期化時に生成し、アドレスをBPFの内部テーブルに保持している。	*/
	/* プロセスが変われば、メモリ空間が変わるためASSIGNで再度取得しなければならない。	*/
	/* 上記の処理を行うと非常に時間がかかるため、通信用共有メモリは動的に生成し、		*/
	/* BPF内のテーブルへの保持は行わない												*/
	/* ただし、どの共有メモリを使用しているかを管理するテーブルの更新は行う				*/

	BPF_MNG_COM_MEMORY*		t_com_memory;					/*	共有メモリ管理テーブル	*/
    int						ret = BPF_RU_IPCM_OK;
	unsigned int			a_loop;						/*	loop counter					*/
	unsigned int			a_keyid = 0;
	unsigned int			a_size;
	unsigned int			bufkind;
	int						shmid;
	unsigned int			useCount;

	/* 種別ごとに取得するバッファ種別を決定する */
	switch ( kind )
	{
	/* 2021/02/23 M&C) Update for intermediate device - Add start */
	/* バッファ種別:LAPB送信用の場合	*/
	case E_BPF_RU_IPCM_BUFF_KIND_LAPBS:
		/* サイズチェック	*/
		if( size > 0x00001000 )
		{
			/* L Large */
			a_size = BPF_MESSAGE_SIZE_L;
			bufkind = E_BPF_RU_IPCM_SHMKIND_LAPBS_L;
		}
		else
		{
			/* Small */
			a_size = BPF_MESSAGE_SIZE_S;
			bufkind = E_BPF_RU_IPCM_SHMKIND_LAPBS_S;
		}
		break;

	/* バッファ種別:LAPB受信用の場合	*/
	case E_BPF_RU_IPCM_BUFF_KIND_LAPBR:
		/* サイズチェック	*/
		if( size > 0x00001000 )
		{
			/* L Large */
			a_size = BPF_MESSAGE_SIZE_L;
			bufkind = E_BPF_RU_IPCM_SHMKIND_LAPBR_L;
		}
		else
		{
			/* Small */
			a_size = BPF_MESSAGE_SIZE_S;
			bufkind = E_BPF_RU_IPCM_SHMKIND_LAPBR_S;
		}
		break;

	/* バッファ種別:モニタ用の場合	*/
	case E_BPF_RU_IPCM_BUFF_KIND_MONITOR:
		/* サイズチェック	*/
		if( size > 0x00001000 )
		{
			/* L Large */
			a_size = BPF_MESSAGE_SIZE_L;
			bufkind = E_BPF_RU_IPCM_SHMKIND_MONITOR_L;
		}
		else
		{
			/* Small */
			a_size = BPF_MESSAGE_SIZE_S;
			bufkind = E_BPF_RU_IPCM_SHMKIND_MONITOR_S;
		}
		break;
	// 2021/02/23 M&C) Update for intermediate device - Add end */
	
	/* バッファ種別:一般通信用の場合	*/
	case E_BPF_RU_IPCM_BUFF_KIND_TASK:
		/* サイズチェック	*/
		if( size <= BPF_MESSAGE_SIZE_S )
		{
			a_size = BPF_MESSAGE_SIZE_S;
			bufkind = E_BPF_RU_IPCM_SHMKIND_TASK_S;
		}
		else if( size <= BPF_MESSAGE_SIZE_M )
		{
			a_size = BPF_MESSAGE_SIZE_M;
			bufkind = E_BPF_RU_IPCM_SHMKIND_TASK_M;
		}
		else
		{
			a_size = BPF_MESSAGE_SIZE_L;
			bufkind = E_BPF_RU_IPCM_SHMKIND_TASK_L;
		}
		break;
	/* 2021/02/23 M&C) Update for intermediate device - Add start */
	case E_BPF_RU_IPCM_BUFF_KIND_MTMMLR:
		a_size = BPF_MESSAGE_SIZE_L;
		bufkind = E_BPF_RU_IPCM_SHMKIND_MTMMLR_BUFF;
		break;
	// 2021/02/23 M&C) Update for intermediate device - Add end */
	default:
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3, "buffer kind error. kind = 0x%08x", kind );
		return -1;
		break;
	}

	/****************************************************************************/
	/* Take Semaphore															*/
	/****************************************************************************/
	struct bpf_timespec		dtime;	/* sem waittime */	
	unsigned int			count;
	dtime.tv_sec = 0;
	dtime.tv_nsec = BPF_RU_IPCM_WAITTIME_BUFF;	/* 50msecで刻む */
	/* 連続7秒とれなかったらシステムとして終わっている */
	/* ユーザプロセスの処理を見直してください */
	for( count = 0 ; count < BPF_RU_IPCM_WAITCOUNT_BUFF ; count++ )
	{
		ret = BPF_RU_IPCM_PROCSEM_TAKE(
			E_RRH_SEMID_MNG_COM_MEMORY,				/* セマフォ番号					*/
			BPF_RU_IPCM_LOCK_RW,					/* R/Wロック種					*/
			BPF_RU_IPCM_WAIT,						/* ウェイトフラグ				*/
			&dtime,									/* タイマ時間					*/
			errcd									/* 詳細NGコード					*/
		);
		if(ret == D_RRH_OK)
		{
			break;
		}
	}
	if( count >= BPF_RU_IPCM_WAITCOUNT_BUFF )
	{
		IPCM_DBGLOG_FMT_ERR("BGET SEM GET TIMEOUT kind:0x%x size:0x%x ",kind,size);
		return -1;
	}
	/* 未使用となるKEYID取得処理 */
	/*共有メモリ管理テーブルを共有メモリから取得 */
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_MNG_COM_MEMORY,
										(void *)&t_com_memory,
										errcd);


	/* 未使用キュー検索 当該KEYID取得処理 */
	for(a_loop = 0; a_loop < BPF_RU_IPCM_SHMID_COMU_MAX; a_loop++)
	{
		/* 未使用中バッファ取得 */
		if(t_com_memory[bufkind].com_mem_inf[a_loop].state == 0)
		{
			a_keyid = t_com_memory[bufkind].com_mem_inf[a_loop].keyID;
			break;
		}
	}
	if(a_loop == BPF_RU_IPCM_SHMID_COMU_MAX)
	{
		ret = BPF_RU_IPCM_PROCSEM_GIVE(
			E_RRH_SEMID_MNG_COM_MEMORY,				/* セマフォ番号					*/
			BPF_RU_IPCM_LOCK_RW,					/* R/Wロック種					*/
			errcd									/* 詳細NGコード					*/
		);
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3, "buffer full occur." );
		return -1;
	}

	/* 検索した空キューに該当する、データ格納共有メモリのアドレスを取得する(attach)	*/
	shmid = shmget( a_keyid, a_size , (IPC_CREAT | IPC_EXCL |S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH) );
	if( shmid == -1 )
	{
		printf("shmget error a_keyid=%d ,errno = %d\n",a_keyid,errno);
		ret = BPF_RU_IPCM_PROCSEM_GIVE(
			E_RRH_SEMID_MNG_COM_MEMORY,				/* セマフォ番号					*/
			BPF_RU_IPCM_LOCK_RW,					/* R/Wロック種					*/
			errcd									/* 詳細NGコード					*/
		);
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3, "shmget ng. keyid = 0x%08x", a_keyid );
		return -1;
	}
	*addr  = shmat(shmid, NULL, 0);
	if( *addr == IPCM_SHMAT_NG ) /* pgr0203 */
	{
		printf("shmat error shmid = %d ,a_keyid=%d ,errno = %d\n",shmid,a_keyid,errno);
		ret = BPF_RU_IPCM_PROCSEM_GIVE(
			E_RRH_SEMID_MNG_COM_MEMORY,				/* セマフォ番号					*/
			BPF_RU_IPCM_LOCK_RW,					/* R/Wロック種					*/
			errcd									/* 詳細NGコード					*/
		);
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3, "shmat ng. keyid = 0x%08x", a_keyid );
		return 1;
	}

	/* KEYID/Lengthを設定する	*/
	(( T_RRH_HEAD *)(*addr))->uiKeyID = a_keyid;
	(( T_RRH_HEAD *)(*addr))->uiLength = size;

	/* 共有メモリ管理テーブルを使用中に変更 */
	t_com_memory[bufkind].com_mem_inf[a_loop].state = 1;
	/* 使用数をインクリメント	*/
	t_com_memory[bufkind].useCount++;
	useCount = t_com_memory[bufkind].useCount;

	/* 通信用共有メモリ取得解放ログを取得する	*/
	bpf_ru_ipcm_procshm_addr_log( BPF_RU_IPCM_SHMBUF_GET, a_keyid , bufkind,useCount);

	/****************************************************************************/
	/* Give Semaphore															*/
	/****************************************************************************/
	ret = BPF_RU_IPCM_PROCSEM_GIVE(
		E_RRH_SEMID_MNG_COM_MEMORY,				/* セマフォ番号					*/
		BPF_RU_IPCM_LOCK_RW,					/* R/Wロック種					*/
		errcd									/* 詳細NGコード					*/
	);
	
	/* 使用数が閾値を超えた場合でさらにCallBack関数が登録されていればCallBack関数をCallする	*/
	/* 何度も出さないために閾値と同じ値になった場合にのみ行う								*/
	if( useCount == BPF_RU_IPCM_SHMID_COMU_OCCUR )
	{
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3, "[WARNING] This buffer reach threshold. kind=%d", bufkind );
		
		if( buffer_full_occur_function_addr != NULL )
		{
			/* UserのCall Back関数をCall	*/
			buffer_full_occur_function_addr( bufkind );
		}
	}

	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Communication Shared Memory Address Free.
 *  @note   API function.
 *  @param  addr         [in] shared memory address
 *  @return result code
 *  @retval BPF_RU_IPCM_OK           normal end
 *  @retval BPF_RU_IPCM_NG           abnormal end
 *  @date   2013/11/07 ALPHA)samejid
 */
/********************************************************************************************************************/
int BPF_RU_IPCM_PROCMSG_ADDRFREE( void *addr )
{
	/* 通信用共有メモリはINパラに指定された共有メモリのKEYIDを取得してshmdtを行う。	*/
	/* 管理テーブルの使用中フラグを落とす											*/

	BPF_MNG_COM_MEMORY*		t_com_memory;					/*	共有メモリ管理テーブル	*/
    int						ret = BPF_RU_IPCM_OK;
	int						errcd;
	unsigned int			a_keyid;
	unsigned int			bufkind;
	unsigned char			buf[D_RRH_BUFSIZE_4096];			/* PAGE_SIZE */
	unsigned int			useCount;

	/********************************************************************/
	/* 共有メモリが有効(該当プロセスにattachされている)かをチェック		*/
	/********************************************************************/
	if(mincore(addr,1,buf))
	{
		/* not attach case  */
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3, "not attached" );
		return BPF_RU_IPCM_ENOTATTACH;
	}

	/* KEYIDを取得する	*/
	a_keyid = (( T_RRH_HEAD *)addr)->uiKeyID;

	/* マスクを掛けて種別を取得する	*/
	switch( (a_keyid & 0xFFFF0000) )
	{
		/* 2021/03/04 M&C) Update for intermediate device - Add start */
/***********	バッファ種別:LAPB送信用		****************/
	case BPF_RU_IPCM_SHMID_LAPBS_S:
		/* Size : S	*/
		bufkind = E_BPF_RU_IPCM_SHMKIND_LAPBS_S;
		break;

	case BPF_RU_IPCM_SHMID_LAPBS_M:
		/* Size : M	*/
		bufkind = E_BPF_RU_IPCM_SHMKIND_LAPBS_M;
		break;

	case BPF_RU_IPCM_SHMID_LAPBS_L:
		/* Size : L	*/
		bufkind = E_BPF_RU_IPCM_SHMKIND_LAPBS_L;
		break;

	case BPF_RU_IPCM_SHMID_LAPBS_LL:
		/* Size : LL	*/
		bufkind = E_BPF_RU_IPCM_SHMKIND_LAPBS_LL;
		break;

/***********	バッファ種別:LAPB送信用		****************/
	case BPF_RU_IPCM_SHMID_LAPBR_S:
		/* Size : S	*/
		bufkind = E_BPF_RU_IPCM_SHMKIND_LAPBR_S;
		break;

	case BPF_RU_IPCM_SHMID_LAPBR_M:
		/* Size : M	*/
		bufkind = E_BPF_RU_IPCM_SHMKIND_LAPBR_M;
		break;

	case BPF_RU_IPCM_SHMID_LAPBR_L:
		/* Size : L	*/
		bufkind = E_BPF_RU_IPCM_SHMKIND_LAPBR_L;
		break;

	case BPF_RU_IPCM_SHMID_LAPBR_LL:
		/* Size : LL	*/
		bufkind = E_BPF_RU_IPCM_SHMKIND_LAPBR_LL;
		break;

/***********	バッファ種別:モニタ用		****************/
	case BPF_RU_IPCM_SHMID_MONITOR_S:
		/* Size : S	*/
		bufkind = E_BPF_RU_IPCM_SHMKIND_MONITOR_S;
		break;

	case BPF_RU_IPCM_SHMID_MONITOR_M:
		/* Size : M	*/
		bufkind = E_BPF_RU_IPCM_SHMKIND_MONITOR_M;
		break;

	case BPF_RU_IPCM_SHMID_MONITOR_L:
		/* Size : L	*/
		bufkind = E_BPF_RU_IPCM_SHMKIND_MONITOR_L;
		break;

	case BPF_RU_IPCM_SHMID_MONITOR_LL:
		/* Size : LL	*/
		bufkind = E_BPF_RU_IPCM_SHMKIND_MONITOR_LL;
		break;

		/* 2021/03/04 M&C) Update for intermediate device - End start */
/***********	バッファ種別:一般通信用		****************/
	case BPF_RU_IPCM_SHMID_TASK_S:
		/* Size : S	*/
		bufkind = E_BPF_RU_IPCM_SHMKIND_TASK_S;
		break;

	case BPF_RU_IPCM_SHMID_TASK_M:
		/* Size : M	*/
		bufkind = E_BPF_RU_IPCM_SHMKIND_TASK_M;
		break;

	case BPF_RU_IPCM_SHMID_TASK_L:
		/* Size : L	*/
		bufkind = E_BPF_RU_IPCM_SHMKIND_TASK_L;
		break;

		/* 2021/03/04 M&C) Update for intermediate device - Add start */
	case BPF_RU_IPCM_SHMID_TASK_LL:
		/* Size : LL	*/
		bufkind = E_BPF_RU_IPCM_SHMKIND_TASK_LL;
		break;

/***********	バッファ種別:MTM/MLR用		****************/
	case BPF_RU_IPCM_SHMID_MTMMLR_BUFF:
		bufkind = E_BPF_RU_IPCM_SHMKIND_MTMMLR_BUFF;
		break;

		/* 2021/03/04 M&C) Update for intermediate device - End start */
	default:
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3, "unknown keyid 0x%08x", a_keyid );
		return -1;
	}

	/* 共有メモリをデタッチする	*/
	shmdt(addr);
	int shmid;
	if((shmid = shmget(a_keyid,0,0)) != -1)
	{
		if( shmctl(shmid,IPC_RMID,0) == -1)
		{
			bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3, "shmctl ng 0x%08x", a_keyid );
			return -1;
		}
	}
	else
	{
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3, "shmget ng 0x%08x", a_keyid );
		return -1;
	}

	/****************************************************************************/
	/* Take Semaphore															*/
	/****************************************************************************/
	struct bpf_timespec		dtime;	/* sem waittime */	
	unsigned int			count;
	dtime.tv_sec = 0;
	dtime.tv_nsec = BPF_RU_IPCM_WAITTIME_BUFF;	/* 50msecで刻む */
	/* 連続7秒とれなかったらシステムとして終わっている */
	/* ユーザプロセスの処理を見直してください */
	for( count = 0 ; count < BPF_RU_IPCM_WAITCOUNT_BUFF ; count++ )
	{
		ret = BPF_RU_IPCM_PROCSEM_TAKE(
			E_RRH_SEMID_MNG_COM_MEMORY,				/* セマフォ番号					*/
			BPF_RU_IPCM_LOCK_RW,					/* R/Wロック種					*/
			BPF_RU_IPCM_WAIT,						/* ウェイトフラグ				*/
			&dtime,									/* タイマ時間					*/
			&errcd									/* 詳細NGコード					*/
		);
		if(ret == D_RRH_OK)
		{
			break;
		}
	}
	if( count >= BPF_RU_IPCM_WAITCOUNT_BUFF )
	{
		IPCM_DBGLOG_FMT_ERR("BFREE SEM GET TIMEOUT keyid:0x%x addr:0x%p",a_keyid,(void*)addr);
		return -1;
	}
	/* 未使用となるKEYID取得処理 */
	/*共有メモリ管理テーブルを共有メモリから取得 */
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_MNG_COM_MEMORY,
										(void**)&t_com_memory,
										&errcd);

	/* 管理テーブル用にKEYIDをマスクし該当共有メモリを未使用にする	*/
	t_com_memory[bufkind].com_mem_inf[(a_keyid & 0x0000FFFF)].state = 0;
	/* 使用数をデクリメント	*/
	t_com_memory[bufkind].useCount--;
	useCount = t_com_memory[bufkind].useCount;
	/* 通信用共有メモリ取得解放ログを取得する	*/
	bpf_ru_ipcm_procshm_addr_log( BPF_RU_IPCM_SHMBUF_FREE, a_keyid ,bufkind,useCount);

	/****************************************************************************/
	/* Give Semaphore															*/
	/****************************************************************************/
	ret = BPF_RU_IPCM_PROCSEM_GIVE(
		E_RRH_SEMID_MNG_COM_MEMORY,				/* セマフォ番号					*/
		BPF_RU_IPCM_LOCK_RW,					/* R/Wロック種					*/
		&errcd									/* 詳細NGコード					*/
	);

	/* 使用数が閾値を下回った場合でさらにCallBack関数が登録されていればCallBack関数をCallする	*/
	/* 何度も出さないために閾値と同じ値になった場合にのみ行う									*/
	if( useCount == BPF_RU_IPCM_SHMID_COMU_CLEAR )
	{
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3, "[CLEAR] buffer threshold is clear. kind=%d", bufkind );
		
		if( buffer_full_clear_function_addr != NULL )
		{
			/* UserのCall Back関数をCall	*/
			buffer_full_clear_function_addr( bufkind );
		}
	}

	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Call Back function set(Buffer Full Occur).
 *  @note   internal function.
 *  @param  function pointer   [in] function pointer
 *  @return none
 *  @date   2013/12/09 ALPHA)yokoyama
 */
/********************************************************************************************************************/
void BPF_RU_IPCM_BUFFER_FULL_OCCUR_CALLBACK_SET( void* funcP )
{
	/* Buffer Fullの閾値を超えた場合のCall Back関数を登録する	*/
	buffer_full_occur_function_addr = funcP;

	return ;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Call Back function set(Buffer Full Clear).
 *  @note   internal function.
 *  @param  function pointer   [in] function pointer
 *  @return none
 *  @date   2013/12/09 ALPHA)yokoyama
 */
/********************************************************************************************************************/
void BPF_RU_IPCM_BUFFER_FULL_CLEAR_CALLBACK_SET( void* funcP )
{
	/* Buffer Fullの閾値超えから復旧した場合のCall Back関数を登録する	*/
	buffer_full_clear_function_addr = funcP;

	return ;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Print Inter Process Communication Management Shared Memory Address History.
 *  @note   internal function.
 *  @param  file dicriptor   [in] file discriptor
 *  @return none
 *  @date   2013/11/18 ALPHA)yokoyama
 */
/********************************************************************************************************************/
VOID BPF_RU_IPCM_PRINT_PROCMSG_BUFF_HIST( FILE* fp )
{
	T_RRH_LOG_BUFF_HIS*		buffhisLog;
	int						iRetCode;
	int						iErrCode;
	unsigned int			logCount;
	unsigned int			logIndex;
	char*					kindMessages[D_RRH_LOG_BUFFLOG_BUFFKIND_NUM] = { "GET    ", "RELEASE" };

	/* 共有メモリのバッファ取得解放履歴を取得する	*/
	/************************************************************/
	/* Get shared memory										*/
	/************************************************************/
	iRetCode = BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_BUFFER_HISTORY,
		(VOID **)&buffhisLog,
		&iErrCode
	);

	if( D_RRH_OK != iRetCode )
	{
		return;
	}

	logIndex = ( buffhisLog->uiWriteIdx + D_RRH_LOG_BUFFLOG_RCD_NUM ) % D_RRH_LOG_BUFFLOG_RCD_NUM ;

	fprintf( fp, "=======================================================================\n");
	fprintf( fp, "[xxxx] Date       Time              PID      TID      key      kind    \n");
	fprintf( fp, "-----------------------------------------------------------------------\n");

	for( logCount = 0; logCount < D_RRH_LOG_BUFFLOG_RCD_NUM; logCount++ )
	{
		if( buffhisLog->tRecord[logIndex].tDate.year != 0 ) {

			/* 時刻表示	*/
			fprintf( fp, "[%04d] %04d/%02d/%02d %02d:%02d:%02d:%03d.%u  ",
					logIndex,
					buffhisLog->tRecord[logIndex].tDate.year,
					buffhisLog->tRecord[logIndex].tDate.month,
					buffhisLog->tRecord[logIndex].tDate.day,
					buffhisLog->tRecord[logIndex].tDate.hour,
					buffhisLog->tRecord[logIndex].tDate.min,
					buffhisLog->tRecord[logIndex].tDate.sec,
					buffhisLog->tRecord[logIndex].tDate.msec*10,
					buffhisLog->tRecord[logIndex].tDate.ucount);
	
			/* 詳細情報表示	*/
			fprintf( fp, "%08x %08x %08x %s\n",
					buffhisLog->tRecord[logIndex].pid,
					buffhisLog->tRecord[logIndex].tid,
					buffhisLog->tRecord[logIndex].buffer_kind,
					kindMessages[buffhisLog->tRecord[logIndex].get_release]
			);
		}
		logIndex = ( logIndex + 1) % D_RRH_LOG_BUFFLOG_RCD_NUM ;

	}
	return ;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Print Inter Process Communication Management Shared Memory Address State.
 *  @note   internal function.
 *  @param  file dicriptor   [in] file discriptor
 *  @return none
 *  @date   2013/11/18 ALPHA)yokoyama
 */
/********************************************************************************************************************/
int BPF_RU_IPCM_PRINT_PROCMSG_BUFF_STATE( FILE* fp )
{
	BPF_MNG_COM_MEMORY*		t_com_memory;					/*	共有メモリ管理テーブル	*/
    int						ret = BPF_RU_IPCM_OK;
	unsigned int			shmKindCnt;
	int						iErrCode;

	/*共有メモリ管理テーブルを共有メモリから取得 */
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_MNG_COM_MEMORY,
										(void**)&t_com_memory,
										&iErrCode);

	if( ret != BPF_RU_IPCM_OK )
	{
		return ret;
	}

	fprintf( fp, "=================================\n");
	fprintf( fp, "BUFFKIND  MAX       USE          \n");
	fprintf( fp, "---------------------------------\n");

	/* 通信用共有メモリの種別数分ループする	*/
	for( shmKindCnt = 0; shmKindCnt < E_BPF_RU_IPCM_SHMKIND_MAX; shmKindCnt++ )
	{
		fprintf( fp, "%08d  %08d  %08d\n", shmKindCnt,  BPF_RU_IPCM_SHMID_COMU_MAX, t_com_memory[shmKindCnt].useCount );
	}

	return ret;
}
/* @} */


/* @} */
