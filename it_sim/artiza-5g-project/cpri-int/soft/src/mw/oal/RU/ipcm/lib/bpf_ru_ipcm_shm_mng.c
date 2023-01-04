/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   f_bpf_ru_ipcm_shm_mng.c
 *  @brief  Inter Process Communication Management internal Library(SHM).
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

/** @addtogroup BPF_RU_IPCM
 * @{ */

/* 共有メモリデータ構造体型定義 */
struct ipcm_shm_data_t {
  int size;       /* 共有メモリサイズ   */
  int used;       /* 使用中フラグ       */
  int shmid;      /* 共有メモリ識別子   */
  void *shmaddr;  /* 共有メモリアドレス */
  void *_ataddr;  /* 共有メモリ割付管理アドレス */
};

/* 共有メモリテーブル構造体型定義 */
struct ipcm_shm_tbl_t {
  enum ipcm_shm_type type;   /* 共有メモリタイプ．ミドル用 or ユーザ用 */
  int max;        /* 共有メモリ最大面数 */
  int keymask;    /* IPCキーマスク */
  int flg;        /* 共有メモリ作成用フラグ */
  int atflg;      /* 共有メモリ作成フラグ(ATTACH関数用) */
  struct ipcm_shm_data_t *data; /* 共有メモリデータ領域 */
};

/* 共有メモリ管理テーブル宣言 */
static struct ipcm_shm_tbl_t bpf_ru_ipcm_shm[IPCM_SHM_TYPE_NUM] = {
/* RRH-001 MD alpha)matsuhashi add start */
  { /* ユーザ用共有メモリ情報 */
    IPCM_SHM_USER,       /* type */
    0,                   /* max */
    IPCM_KEYMASK_USER,   /* keymask */
    IPCM_SHMFLG,         /* flg */
    IPCM_SHMATFLG,       /* atflg */
    NULL                 /* data */
  },
  { /* 共有メモリ情報(未使用) */
    IPCM_SHM_DUMMY,      /* type */
    0,                   /* max  */
    IPCM_KEYMASK_DUMMY,  /* keymask */
    IPCM_SHMFLG,         /* flg */
    IPCM_SHMATFLG,       /* atflg */
    NULL                 /* data */
  }
/* RRH-001 MD alpha)matsuhashi add end */
};


/* IPCM_SHM内の排他用ミューテックス */
static pthread_mutex_t bpf_ru_ipcm_shm_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Function prototype */
int bpf_ru_ipcm_shm_init_tbl_set(enum ipcm_shm_type shm_type, bpf_ru_ipcm_sg_data_t *sgdata);

static struct ipcm_shm_create_data_t	shmCreateTable;

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Shared Memory Number Check.
 *  @note   IPCM internal function.
 *  @param  shm_type     [in]  shared memory type
 *                                    IPCM_SHM_MDL
 *                                    IPCM_SHM_USER
 *  @param  shm_num      [in]  shared memory number
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_shm_numcheck(enum ipcm_shm_type shm_type, unsigned int shm_num)
{
    /********************************************
    * shared memory number check processing.    *
    ********************************************/
    /* 範囲チェック．タイプに応じた最大面数と比較して越えていたらNG */
    if( shm_num >= bpf_ru_ipcm_shm[shm_type].max ){
      return BPF_RU_IPCM_NG;
    }
    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Shared Memory Data Write.
 *  @note   IPCM internal function.
 *  @param  shm_type     [in]  shared memory type
 *                                    IPCM_SHM_MDL
 *                                    IPCM_SHM_USER
 *  @param  shm_num      [in]  shared memory number
 *  @param  shmid        [in]  shared memory id
 *  @param  shmaddr      [in]  shared memory address
 *  @param  used         [in]  use information
 *                                    IPCM_SHM_UNUSED
 *                                    IPCM_SHM_USED
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_shm_write_data(enum ipcm_shm_type shm_type, unsigned int shm_num,
                int shmid, void *shmaddr, int used)
{
    /********************************************
    * shared memory data write processing.      *
    ********************************************/
    bpf_ru_ipcm_shm[shm_type].data[shm_num].used = used;
    bpf_ru_ipcm_shm[shm_type].data[shm_num].shmid = shmid;
    bpf_ru_ipcm_shm[shm_type].data[shm_num].shmaddr = shmaddr;
    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Shared Memory Data Read.
 *  @note   IPCM internal function.
 *  @param  shm_type     [in]  shared memory type
 *                                    IPCM_SHM_MDL
 *                                    IPCM_SHM_USER
 *  @param  shm_num      [in]  shared memory number
 *  @param  size         [out] shared memory size
 *  @param  shmid        [out] shared memory id
 *  @param  shmaddr      [out] shared memory address
 *  @param  used         [out] use information
 *                                    IPCM_SHM_UNUSED
 *                                    IPCM_SHM_USED
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_shm_read_data(enum ipcm_shm_type shm_type, unsigned int shm_num,
               int *size, int *shmid,  void **shmaddr, int *used)
{
    /********************************************
    * shared memory data read processing.       *
    ********************************************/
    *size = bpf_ru_ipcm_shm[shm_type].data[shm_num].size;
    *used = bpf_ru_ipcm_shm[shm_type].data[shm_num].used;
    *shmid = bpf_ru_ipcm_shm[shm_type].data[shm_num].shmid;
    *shmaddr = bpf_ru_ipcm_shm[shm_type].data[shm_num].shmaddr;
    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Shared Memory Table Read.
 *  @note   IPCM internal function.
 *  @param  shm_type     [in]  shared memory type
 *                                    IPCM_SHM_MDL
 *                                    IPCM_SHM_USER
 *  @param  type         [out] shared memory type
 *  @param  max          [out] max shared memory number
 *  @param  keymask      [out] IPC key mask
 *  @param  flg          [out] shared memory create flag
 *  @param  atflg        [out] shared memory attach flag
 *  @param  daddr        [out] data address
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_shm_read_tbl(enum ipcm_shm_type shm_type,
              enum ipcm_shm_type *type, int *max,
              int *keymask, int *flg, int *atflg, void **daddr)
{
    /********************************************
    * shared memory table read processing.      *
    ********************************************/
    *type = bpf_ru_ipcm_shm[shm_type].type;
    *max = bpf_ru_ipcm_shm[shm_type].max;
    *keymask = bpf_ru_ipcm_shm[shm_type].keymask;
    *flg = bpf_ru_ipcm_shm[shm_type].flg;
    *atflg = bpf_ru_ipcm_shm[shm_type].atflg;
    *daddr = bpf_ru_ipcm_shm[shm_type].data;
    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Shared Memory Function Initialize.
 *  @note   IPCM internal function.
 *  @param  process_desc [in] process descriptor
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi    modify for BS3001
 *  @date   2013/10/10 ALPHA)yokoyama      modify for ZYNQ docomo
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_shm_init(unsigned int process_desc)
{
    int j;
    int errcd, ret;
    struct{
        int    sg_num;
        bpf_ru_ipcm_sg_data_t *sgdata[3];
    } sg_proc_data;
    enum ipcm_shm_type shm_type;
/* RRH-001 MD alpha)matsuhashi add start */
    int cnt;
    bpf_ru_ipcm_sg_data_t sgdata;
/* RRH-001 MD alpha)matsuhashi add end */
	unsigned int	tablecnt;
	unsigned int	shmmax = 0;
	void			*DataPtr;
	int				shmid;
	void			*shmaddr;
	BPF_MNG_COM_MEMORY	*t_com_memory;			/*	共有メモリ管理テーブル			*/
	unsigned int	keyid;
	unsigned int	com_kind;
	unsigned int	shm_cnt;

    sg_proc_data.sg_num = BPF_COM_NUM0;

    /************************************************
    * 初期化Shared memory Type の決定               *
    ************************************************/
    (void)bpf_com_get_proc_tkind(process_desc);

/* RRH-001 MD alpha)matsuhashi add start */
    shm_type = IPCM_SHM_USER;

    sg_proc_data.sg_num++;
    sg_proc_data.sgdata[BPF_COM_NUM0] = &sgdata;

	sg_proc_data.sgdata[BPF_COM_NUM0]->shm_max = IPCM_SHM_NUM_MAX;
    sg_proc_data.sgdata[BPF_COM_NUM0]->shm_top_no = 0;

	/* 初回以外の場合、共有メモリから共有メモリ作成用のデータを取得する	*/
	if( shmCreateTable.datacnt == 0 )
	{
		/* KEYだけは分かっているので、KEYからSHMIDを取得する	*/
		shmid = shmget( IPCM_SHM_MNG_TBL_ID, IPCM_SHM_MNG_TBL_SIZE, IPCM_SHM_MNG_TBL_AUTH );
		shmaddr  = shmat(shmid, (const void *)NULL, 0);

		shmCreateTable.datacnt = ((struct ipcm_shm_create_data_t *)shmaddr)->datacnt;

		for( tablecnt = 0; tablecnt < shmCreateTable.datacnt; tablecnt++ )
		{
			shmCreateTable.info[tablecnt].shmcnt  = ((struct ipcm_shm_create_data_t *)shmaddr)->info[tablecnt].shmcnt;
			shmCreateTable.info[tablecnt].shmsize = ((struct ipcm_shm_create_data_t *)shmaddr)->info[tablecnt].shmsize;

		}
		shmdt(shmaddr);
	}

	for( tablecnt = 0; tablecnt < shmCreateTable.datacnt; tablecnt++)
	{
	    for(cnt=0; cnt < shmCreateTable.info[tablecnt].shmcnt; cnt++ )
	    {
			/* 共有メモリサイズの設定 */
			sg_proc_data.sgdata[BPF_COM_NUM0]->shm_size[shmmax].size = shmCreateTable.info[tablecnt].shmsize;
			sg_proc_data.sgdata[BPF_COM_NUM0]->shm_size[shmmax].rep_flg = 0;
	    	shmmax++;
	    }
	}

	/* 余りを埋める。暫定でPageSizeにしておく	*/
	for( tablecnt = shmmax; tablecnt < IPCM_SHM_NUM_MAX; tablecnt++ )
	{
		sg_proc_data.sgdata[BPF_COM_NUM0]->shm_size[tablecnt].size = IPCM_SHM_SIZE_4K;
		sg_proc_data.sgdata[BPF_COM_NUM0]->shm_size[tablecnt].rep_flg = 0;
	}

    sg_proc_data.sgdata[BPF_COM_NUM0]->shm_topaddr = NULL;

    bpf_ru_ipcm_shm[shm_type].max = sg_proc_data.sgdata[BPF_COM_NUM0]->shm_max;

/* RRH-001 MD alpha)matsuhashi add end */

    /* Trace log */
    IPCM_DBGLOG_FMT_TRC(
        "SHM init. type=%d max=%d ",
        shm_type, bpf_ru_ipcm_shm[shm_type].max );


    /* 共有メモリ数=0の場合はここで終了 */
    if( bpf_ru_ipcm_shm[shm_type].max == 0){
        IPCM_DBGLOG_FMT_ERR(
          "Shared memory Num is 0. pdesc=%d q_type=%d", process_desc, shm_type);
		return BPF_RU_IPCM_OK;
	}

    /* 初期設定 */
    pthread_mutex_init(&bpf_ru_ipcm_shm_mutex, NULL);

    /********************************************
    * shared memory table create processing.    *
    ********************************************/
    /* データテーブルの領域確保 */
    bpf_ru_ipcm_shm[shm_type].data = malloc(sizeof(struct ipcm_shm_data_t) *
              bpf_ru_ipcm_shm[shm_type].max);
    errcd = errno;
    if( bpf_ru_ipcm_shm[shm_type].data == NULL ){
        /* メモリ確保失敗 */
        IPCM_DBGLOG_FMT_ERR(
            "malloc NG. errno = %d", errcd);
        return BPF_RU_IPCM_NG;
    }

    /* データ用領域を一旦サイズ0でクリア */
    for( j = 0; j < bpf_ru_ipcm_shm[shm_type].max; j++ ){

        bpf_ru_ipcm_shm[shm_type].data[ j].size = 0;
        bpf_ru_ipcm_shm[shm_type].data[ j].used = IPCM_SHM_UNUSED;
        bpf_ru_ipcm_shm[shm_type].data[ j].shmaddr = NULL;
		bpf_ru_ipcm_shm[shm_type].data[ j]._ataddr = NULL;
    }

    /* データ領域設定 */
    for(j=0; j<sg_proc_data.sg_num; j++){
        if( sg_proc_data.sgdata[j]->shm_max != 0 ){
	        ret = bpf_ru_ipcm_shm_init_tbl_set(shm_type, sg_proc_data.sgdata[j]);
	        if( ret != BPF_RU_IPCM_OK ){
	            return BPF_RU_IPCM_NG;
	        }
        }
    }
    /********************************************
    * shared memory create processing.          *
    ********************************************/
	/* attach アドレス指定分のみを実施 */
    for( j = 0; j < bpf_ru_ipcm_shm[shm_type].max; j++ ){

		if( bpf_ru_ipcm_shm[shm_type].data[ j]._ataddr != NULL ){

	        ret = bpf_ru_ipcm_procshm_create(shm_type, j,
			        bpf_ru_ipcm_shm[shm_type].data[ j]._ataddr, &errcd);
	        if(ret != BPF_RU_IPCM_OK){
	            /* エラー処理 */
	            IPCM_DBGLOG_FMT_ERR(
	              "bpf_ru_ipcm_procshm_create NG. ret = %d, type = %d, num = %d",
	              ret, shm_type, j);
	             return BPF_RU_IPCM_NG;
	        }

		    /* ERR log */
		    if( bpf_ru_ipcm_shm[shm_type].data[ j]._ataddr !=
		    			bpf_ru_ipcm_shm[shm_type].data[ j].shmaddr ){
			    IPCM_DBGLOG_FMT_ERR(
			        "different shmat address. type=%d no=%d size=%d ataddr=%p shmaddr=%p",
			        shm_type,
			        j,
			        bpf_ru_ipcm_shm[shm_type].data[ j].size,
			        bpf_ru_ipcm_shm[shm_type].data[ j]._ataddr,
			        bpf_ru_ipcm_shm[shm_type].data[ j].shmaddr);
	             return BPF_RU_IPCM_NG;
			}
	    }
	 }

	/* attach アドレス未指定分のみを実施 */
    for( j = 0; j < bpf_ru_ipcm_shm[shm_type].max; j++ ){

		if( bpf_ru_ipcm_shm[shm_type].data[ j]._ataddr == NULL ){

	        ret = bpf_ru_ipcm_procshm_create(shm_type, j,
			        bpf_ru_ipcm_shm[shm_type].data[ j]._ataddr, &errcd);
	        if(ret != BPF_RU_IPCM_OK){
	            /* エラー処理 */
	            IPCM_DBGLOG_FMT_ERR(
	              "bpf_ru_ipcm_procshm_create NG. ret = %d, type = %d, num = %d",
	              ret, shm_type, j);

	             return BPF_RU_IPCM_NG;
	        }
	    }
	}

	/* 初回はテーブル情報を共有メモリに登録する	*/
	if( shmCreateTable.datacnt!=0 )
	{
		(void)BPF_RU_IPCM_PROCSHM_ADDRGET(
					E_RRH_SHMID_BPF_MNG,				/* 共有メモリ番号				*/
					(void **)&DataPtr,					/* 共有メモリアドレス			*/
					&errcd );							/* 詳細NGコード					*/

		((struct ipcm_shm_create_data_t *)DataPtr)->datacnt = shmCreateTable.datacnt;
		for( tablecnt = 0; tablecnt < shmCreateTable.datacnt; tablecnt++ )
		{
			((struct ipcm_shm_create_data_t *)DataPtr)->info[tablecnt].shmcnt = shmCreateTable.info[tablecnt].shmcnt;
			((struct ipcm_shm_create_data_t *)DataPtr)->info[tablecnt].shmsize = shmCreateTable.info[tablecnt].shmsize;
		}

		/* 通信用の共有メモリ管理データを更新する */
		/* 共有メモリ管理テーブル取得 */
		(void)BPF_RU_IPCM_PROCSHM_ADDRGET(
					E_RRH_SHMID_MNG_COM_MEMORY,
					(void *)&t_com_memory,
					&errcd);

		/* 共有メモリ管理テーブル初期化 */
		/* 通信用共有メモリ種別数分ループ	*/
		keyid = BPF_RU_IPCM_SHMID_START;

		for( com_kind = 0; com_kind < E_BPF_RU_IPCM_SHMKIND_MAX; com_kind++ )
		{
			/* 種別毎の数分ループ	*/
			for( shm_cnt = 0; shm_cnt < BPF_RU_IPCM_SHMID_COMU_MAX; shm_cnt++ )
			{
				t_com_memory[com_kind].com_mem_inf[shm_cnt].keyID = keyid + shm_cnt;
			}

			/* OFFSETを足しこむ	*/
			keyid += BPF_RU_IPCM_SHMID_OFS;
		}

	}

    return BPF_RU_IPCM_OK;

}
/* @} */



/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Shared Memory Function Initialize table.
 *  @note   IPCM internal function.
 *  @param  shm_type [in] Shared memory type
 *  @param  sgdata   [in] SG data address
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_shm_init_tbl_set(enum ipcm_shm_type shm_type, bpf_ru_ipcm_sg_data_t *sgdata)
{
    int j;
    int shm_no;
    int shmsize=0; /* for logging */
    unsigned int pagesize;
    char *shmat_addr;


    /* 共有メモリ割付アドレス */
    shmat_addr =  sgdata->shm_topaddr;

    /* PAGEサイズ取得 */
    pagesize = sysconf(_SC_PAGESIZE);
    if( pagesize <= BPF_COM_NUM0 ){
        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,
                          "page size get errno errno=%d", errno);
        /* abnormal end */
        return BPF_RU_IPCM_NG;
    }
    for( j = 0, shm_no = sgdata->shm_top_no; j < sgdata->shm_max; j++, shm_no++ ){

        bpf_ru_ipcm_shm[shm_type].data[ shm_no].size = sgdata->shm_size[j].size;
        shmsize+=bpf_ru_ipcm_shm[shm_type].data[ shm_no].size; /* For log */

        if( (shmat_addr != NULL) &&
            (bpf_ru_ipcm_shm[shm_type].data[ shm_no].size != 0) &&
            (sgdata->shm_size[j].rep_flg == BPF_COM_NUM1) ){

			bpf_ru_ipcm_shm[shm_type].data[ shm_no]._ataddr = shmat_addr;
			shmat_addr += ( ( (bpf_ru_ipcm_shm[shm_type].data[ shm_no].size + (pagesize-1)) /	/* pgr0689 */
							pagesize) * pagesize );
		}else{

			bpf_ru_ipcm_shm[shm_type].data[ shm_no]._ataddr = NULL;
		}

    }

    /* Trace log */
    IPCM_DBGLOG_FMT_TRC(
        "Alloc total memory.  shm_type=%d SHM=%d mng=%d pagesize=%d top_addr=%p sg_top=%d sg_max=%d",
        shm_type, shmsize,
        (sizeof(struct ipcm_shm_data_t) * (bpf_ru_ipcm_shm[shm_type].max)),
        pagesize, shmat_addr, sgdata->shm_top_no, sgdata->shm_max);

    return BPF_RU_IPCM_OK;
}
/* @} */

/* RRH-001 MD alpha)matsuhashi add start */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Shared Memory Function UnInitialize.
 *  @note   IPCM internal function.
 *  @param  process_desc [in] process descriptor
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_shm_uninit(unsigned int process_desc)
{
    enum ipcm_shm_type shm_type;

    shm_type = IPCM_SHM_USER;

    /* 捕捉したバッファを解放する */
    if (bpf_ru_ipcm_shm[shm_type].data != NULL)
    {
        free(bpf_ru_ipcm_shm[shm_type].data);
        bpf_ru_ipcm_shm[shm_type].data = NULL;
    }

    return BPF_RU_IPCM_OK;

}
/* @} */
/* RRH-001 MD alpha)matsuhashi add end */

/* RRH-001 MD alpha)yokoyama add start */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Shared Memory Set Table.
 *  @note   IPCM internal function.
 *  @param  process_desc [in] process descriptor
 *  @param  dataCnt      [in] dataCnt
 *  @param  shmTable     [in] shmared memory table
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/10/11 ALPHA)yokoyama    create for ZYNQ docomo
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_shm_set_table(unsigned int process_desc, unsigned int dataCnt, T_RRH_SYS_CREATE_SHMMEM *shmTable)
{
	unsigned int			cnt;
	unsigned int pagesize;

	pagesize = sysconf(_SC_PAGESIZE);
	shmCreateTable.datacnt = dataCnt;

	for( cnt = 0; cnt < dataCnt; cnt++ )
	{
		shmCreateTable.info[cnt].shmcnt  = shmTable[cnt].shmCnt;
		shmCreateTable.info[cnt].shmsize = shmTable[cnt].shmSize + (pagesize - ( shmTable[cnt].shmSize % pagesize ));

	}

    return BPF_RU_IPCM_OK;
}
/* @} */
/* RRH-001 MD alpha)yokoyama add end */

/* @} */
