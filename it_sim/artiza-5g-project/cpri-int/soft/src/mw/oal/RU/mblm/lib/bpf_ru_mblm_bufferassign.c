/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	bpf_ru_mblm_bufferassign.c
 *	@brief	Buffer Management Buffer Assign.
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_MBLM /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_l_com.h"
#include "bpf_i_ru_mblm.h"

/** @addtogroup BPF_RU_MBLM
 * @{ */

/* Function prototypr */
int bpf_ru_mblm_bufmng_init(void* , void* , int );
void bpf_ru_mblm_free_mng_init(int, t_bpf_ru_mblm_free_blk_mng *, t_bpf_ru_mblm_free_blk_list_elm *);
void bpf_ru_mblm_mblock_init(char*, unsigned int, unsigned int, unsigned int, unsigned int );
int bpf_ru_mblm_mutex_init(pthread_mutex_t *);
int bpf_ru_mblm_calc_mng_shmsize(void);
int bpf_ru_mblm_calc_mbm_shmsize(void);
int bpf_ru_mblm_set_systeminfo();


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Buffer Management Buffer Assign.
 *  @note   BPF internal function.
 *  @param  process_desc [in]  process descriptor
 *  @return result code
 *  @retval BPF_RU_MBLM_OK  normal end
 *  @retval BPF_RU_MBLM_ASSIGN_NG assign error
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int bpf_ru_mblm_bufferassign(unsigned int process_desc)
{

    int mng_shmsize;       /* Shard memory size    */
    int mbm_shmsize;       /* Shard memory size    */
    key_t mng_ipckey;      /* IPC Key              */
    key_t mbm_ipckey;      /* IPC Key              */
    int mng_shmid;         /* Shard memory ID      */
    int mbm_shmid;         /* Shard memory ID      */
    int func_ret;          /* Function return      */
    int mng_errcd;         /* Error code           */
    int mbm_errcd;         /* Error code           */
    void *mng_sm = NULL;   /* Shard memory address */
    void *mbm_sm = NULL;   /* Shard memory address */


    /************************************************
    * system data table set processing.             *
    ************************************************/
/* RRH-001 MD alpha)matsuhashi add start */
    func_ret = bpf_ru_mblm_set_systeminfo();
/* RRH-001 MD alpha)matsuhashi add end */

    if(func_ret != BPF_RU_MBLM_OK){

        /* ERROR(System info ng.) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
              "System info get error.. prcess_desc=%d",process_desc);

        return BPF_RU_MBLM_ASSIGN_NG;
    }

    /* Culculate the shard memory size */
    mng_shmsize = bpf_ru_mblm_calc_mng_shmsize();
    mbm_shmsize = bpf_ru_mblm_calc_mbm_shmsize();

    if( (mng_shmsize == 0) || (mbm_shmsize == 0) ){

        /* ERROR(Size is zero) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
              "Non buffer.. prcess_desc=%d",process_desc);

        return BPF_RU_MBLM_OK;
    }


    /************************************************
    * IPC key create processing.                    *
    ************************************************/
    mng_ipckey = BPF_RU_MBLM_MNG_IPCKEY(process_desc);
    mbm_ipckey = BPF_RU_MBLM_MBM_IPCKEY(process_desc);

    /* TRACE */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,
           "MBLM Shmem size. proc_desc=%d mng:key=%08x size=%d mbm:key=%08x size=%d",
           process_desc,
           mng_ipckey, mng_shmsize,
           mbm_ipckey, mbm_shmsize);

    /************************************************
    * shared memory get processing.                 *
    ************************************************/
    mng_shmid = shmget( (key_t)mng_ipckey , (size_t)mng_shmsize , BPF_COM_SEMFLG );	/* pgr0821 */
    mng_errcd = errno;
    mbm_shmid = shmget( (key_t)mbm_ipckey , (size_t)mbm_shmsize , BPF_COM_SEMFLG );	/* pgr0821 */
    mbm_errcd = errno;

    if((mng_shmid == BPF_RU_MBLM_SYSCALL_NG) ||
       (mbm_shmid == BPF_RU_MBLM_SYSCALL_NG)){

        /* ERROR(Shard memory get error) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
               "shmget failed. disc=%d mng;key=%x ret=%d errcd=%d size=%d mbm;key=%x ret=%d errcd=%d size=%d",
               process_desc,
               mng_ipckey, mng_shmid, mng_errcd, mng_shmsize,
               mbm_ipckey, mbm_shmid, mbm_errcd, mbm_shmsize);

        return BPF_RU_MBLM_ASSIGN_NG;
    }

    /************************************************
    * shared memory attach processing.              *
    ************************************************/
    mng_sm = shmat(mng_shmid, 0, 0);
    mng_errcd = errno;
    mbm_sm = shmat(mbm_shmid, 0, 0);
    mbm_errcd = errno;

    if((mng_sm == (void*)BPF_RU_MBLM_SYSCALL_NG) ||	/* pgr0203 */
       (mbm_sm == (void*)BPF_RU_MBLM_SYSCALL_NG)){	/* pgr0203 */

        /* ERROR(Shard memory attach error) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
               "shmat failed. disc=%d mng;key=%x ret=%d errcd=%d mbm;key=%x ret=%d errcd=%d",
               process_desc,
               mng_ipckey, mng_sm, mng_errcd,
               mbm_ipckey, mbm_sm, mbm_errcd);

        return BPF_RU_MBLM_ASSIGN_NG;
    }

    /* set 0 at shard memory area */
    memset(mng_sm, 0, mng_shmsize);
    memset(mbm_sm, 0, mbm_shmsize);
    /************************************************
    * process infomation set processing.            *
    ************************************************/
    bpf_ru_mblm_prc_info.process_desc = process_desc;  /* process descriptor         */
    bpf_ru_mblm_prc_info.mng_shmaddr  = mng_sm;        /* management area address    */
    bpf_ru_mblm_prc_info.mng_shmsize  = mng_shmsize ;  /* management area size       */
    bpf_ru_mblm_prc_info.shmaddr      = mbm_sm;        /* memory block area address  */
    bpf_ru_mblm_prc_info.shmsize      = mbm_shmsize;   /* memory block area size     */

    /************************************************
    * buffer management table initialize processing.*
    ************************************************/
    func_ret = bpf_ru_mblm_bufmng_init(bpf_ru_mblm_prc_info.mng_shmaddr, bpf_ru_mblm_prc_info.shmaddr,
                            bpf_ru_mblm_prc_info.shmsize);

    if(func_ret != BPF_RU_MBLM_OK){

        /* ERROR */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
                       "Mblm buffer mng initial ng.ret=%d ", func_ret);

        return BPF_RU_MBLM_ASSIGN_NG;
    }

    /* Set MBLM assign flag */
    bpf_ru_mblm_assign_flag = BPF_RU_MBLM_FLG_ON;

    return BPF_RU_MBLM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Buffer Management Buffer Management Information Initialize.
 *  @note   MBLM internal function.
 *  @param  mng_buff     [in]  buffer management area address
 *  @param  mbm_buff     [in]  memory block address
 *  @param  mbm_size     [in]  memory block size
 *  @return result code
 *  @retval BPF_RU_MBLM_OK        normal end
 *  @retval BPF_RU_MBLM_NG        abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int bpf_ru_mblm_bufmng_init(void* mng_buff, void* mbm_buff, int mbm_size)
{
    int buftype;        /* Buffer type                            */
    t_bpf_ru_mblm_free_blk_list_elm *freelist_top;
                        /* the address of free memory block management list table */
    char *mbmtop;       /* memory block top address */
    int  mbmsize_total; /* memory block total size  */
    int  ret;           /* function return value    */

    /* Buffer management information taable address set */
    bpf_ru_mblm_buffer_mng = (t_bpf_ru_mblm_buffer_mng*)mng_buff;

    /* Free memory block management list Top address set. */
    freelist_top = (t_bpf_ru_mblm_free_blk_list_elm*)(bpf_ru_mblm_buffer_mng+BPF_RU_MBLM_BUFTYPE_NUM);

    /***********************************************************
    * buffer management information table initialize proessing.*
    ***********************************************************/
    mbmtop = (char*)mbm_buff;
/* RRH-001 MD alpha)matsuhashi add start */
    for(buftype=BPF_RU_MBLM_BUFTYPE_256; buftype<BPF_RU_MBLM_BUFTYPE_NUM; buftype++){
/* RRH-001 MD alpha)matsuhashi add end */

        /* Set buffer management information table */
        bpf_ru_mblm_buffer_mng[buftype].buffer_size = bpf_ru_mblm_system_data[buftype].buffer_size;

        if( bpf_ru_mblm_buffer_mng[buftype].buffer_size == 0){
            /* IF buffer size is 0, the following are not set.  */
            continue;
        }

        bpf_ru_mblm_buffer_mng[buftype].mbm_size    = (sizeof(t_bpf_ru_mblm_mbm_top_info) +
                                                bpf_ru_mblm_system_data[buftype].buffer_size +
                                                sizeof(t_bpf_ru_mblm_mbm_tail_info));
        bpf_ru_mblm_buffer_mng[buftype].mbm_num     = bpf_ru_mblm_system_data[buftype].buffer_num;
        bpf_ru_mblm_buffer_mng[buftype].mbm_top     = mbmtop;

        /* Size of the all memory block */
        mbmsize_total = (bpf_ru_mblm_buffer_mng[buftype].mbm_size
                         * bpf_ru_mblm_buffer_mng[buftype].mbm_num);

        bpf_ru_mblm_buffer_mng[buftype].mbm_bottom  = (mbmtop + mbmsize_total);

        bpf_ru_mblm_buffer_mng[buftype].total_allocreq_num = 0;
        bpf_ru_mblm_buffer_mng[buftype].total_allocng_num = 0;
        bpf_ru_mblm_buffer_mng[buftype].minimam_nouse_num = bpf_ru_mblm_buffer_mng[buftype].mbm_num;

        /* Mutex initialization */
        ret = bpf_ru_mblm_mutex_init(&bpf_ru_mblm_buffer_mng[buftype].mutex);
        if(ret != BPF_RU_MBLM_OK){
            return BPF_RU_MBLM_NG;
        }

        /* Free memory block managemnet information initialization */
        bpf_ru_mblm_free_mng_init(bpf_ru_mblm_buffer_mng[buftype].mbm_num,
                               &bpf_ru_mblm_buffer_mng[buftype].free_mng,
                               freelist_top);

        /* Memory block management area initialization */
        bpf_ru_mblm_mblock_init(bpf_ru_mblm_buffer_mng[buftype].mbm_top,
                             bpf_ru_mblm_buffer_mng[buftype].mbm_num,
                             buftype,
                             bpf_ru_mblm_buffer_mng[buftype].buffer_size,
                             bpf_ru_mblm_buffer_mng[buftype].mbm_size);

        /* Free memory block management list Top address update. */
        freelist_top += bpf_ru_mblm_buffer_mng[buftype].mbm_num;

        /* Memory block, Top address update. */
        mbmtop += mbmsize_total;

    }

    return BPF_RU_MBLM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Buffer Management Free Memory Block Management Information Initialize.
 *  @note   MBLM internal function.
 *  @param  total_num    [in]  total number of free memory block
 *  @param  free_mng     [in]  free memory block management table address
 *  @param  mbm_list     [in]  free memory block management list size
 *  @return None
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
void bpf_ru_mblm_free_mng_init(int total_num,
                                   t_bpf_ru_mblm_free_blk_mng *free_mng,
                                   t_bpf_ru_mblm_free_blk_list_elm *mbm_list)
{
    int mbmno;          /* Memory block number  */

    /***********************************************************
    * Free Memory Block Management table initialize proessing. *
    ***********************************************************/
    free_mng->mbm_list = mbm_list;
    free_mng->free_num = total_num;
    free_mng->head     = 0;
    free_mng->tail     = (total_num-1);

    /***********************************************************
    * Free Memory Block Management list initialize proessing.  *
    ***********************************************************/
    for(mbmno=0; mbmno<total_num; mbmno++){
        if(mbmno != (total_num-1)){
            /* excluding last */
            mbm_list->next = mbmno+1;
        }else{
            /* Last(Set Last Identifier) */
            mbm_list->next = BPF_RU_MBLM_LISTEND;
        }
        mbm_list++;
    }
    return;

}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Buffer Management Memory Block Area Initialize.
 *  @note   MBLM internal function.
 *  @param  None
 *  @return None
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
void bpf_ru_mblm_mblock_init(char* mbm_addr, unsigned int mbm_num,
                                 unsigned int buftype, unsigned int buffer_size,
                                 unsigned int mbm_size)
{
    int  mbmno;                             /* Memory block number     */
    t_bpf_ru_mblm_mbm_top_info  *top_info;  /* Memory block head information */
    t_bpf_ru_mblm_mbm_tail_info *tail_info; /* Memory block tail information */

    /**********************************************************
    * momory block chain initialize proessing.                *
    ***********************************************************/
    for(mbmno=0; mbmno<mbm_num; mbmno++){

        top_info  = (t_bpf_ru_mblm_mbm_top_info*)mbm_addr;
        tail_info = MBML_TAIL_INFO_ADDR(mbm_addr, buffer_size);

        /* Memory block head information */
        top_info->buftop_id = BPF_RU_MBLM_MBM_TOP_ID;
        top_info->mbm_info.use_flg = BPF_RU_MBLM_FLG_OFF;
        top_info->mbm_info.thread_id = 0;
        top_info->mbm_info.buffer_type = buftype;
        top_info->mbm_info.mbm_no = mbmno;
        top_info->mbm_info.alloc_cnt = 0;

        /* Memory block tail information */
        tail_info->buftail_id = BPF_RU_MBLM_MBM_TAIL_ID;

        mbm_addr += mbm_size;
    }

    return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Buffer Management Buffer Mutex Initialize.
 *  @note   MBLM internal function.
 *  @param  mutex        [in]  mutex object
 *  @return result code
 *  @retval BPF_RU_MBLM_OK        normal end
 *  @retval BPF_RU_MBLM_NG        abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_mblm_mutex_init(pthread_mutex_t *mutex)
{
    int  sysret;         /* system call return value     */

    /**********************************************************
    * buffer mutex initialize proessing.                      *
    ***********************************************************/
    sysret = pthread_mutex_init(mutex, NULL);
    if(sysret){

        /* ERROR */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
                       "Mutex init ng. errcd=%d ",
                       errno);

        /* Mutex initialization failure */
        return BPF_RU_MBLM_NG;
    }

    return BPF_RU_MBLM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Buffer Management Shared Memory Size Calculate for Management.
 *  @note   MBLM internal function.
 *  @param  None
 *  @return None
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int bpf_ru_mblm_calc_mng_shmsize(void)
{
    int bufmng_size;    /* Buffer management information size     */
    int freelist_size;  /* Free memory block management list size */
    int buftype;        /* Buffer type                            */

    /**********************************************************
    * shared Memory size calculate proessing for management.  *
    ***********************************************************/
    /* Buffer management informaton size */
    bufmng_size = (sizeof(t_bpf_ru_mblm_buffer_mng) * BPF_RU_MBLM_BUFTYPE_NUM);

    /* Free memory block management table size */
    freelist_size = 0;
/* RRH-001 MD alpha)matsuhashi add start */
    for(buftype=BPF_RU_MBLM_BUFTYPE_256; buftype<BPF_RU_MBLM_BUFTYPE_NUM; buftype++){
/* RRH-001 MD alpha)matsuhashi add end */
        /* Free Memory block size */
        freelist_size += (bpf_ru_mblm_system_data[buftype].buffer_num *
                           sizeof(t_bpf_ru_mblm_free_blk_list_elm));
    }

    return(bufmng_size+freelist_size);
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Buffer Management Shared Memory Size Calculate for Momory Block.
 *  @note   MBLM internal function.
 *  @param  None
 *  @return None
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int bpf_ru_mblm_calc_mbm_shmsize(void)
{
    int mbm_size;       /* Memory block size  */
    int buftype;        /* Buffer type        */

    mbm_size = 0;
    /**********************************************************
    * shared Memory size calculate proessing for memory block.*
    ***********************************************************/
/* RRH-001 MD alpha)matsuhashi add start */
    for(buftype=BPF_RU_MBLM_BUFTYPE_256; buftype<=BPF_RU_MBLM_BUFTYPE_8192; buftype++){
/* RRH-001 MD alpha)matsuhashi add end */

        /* Memory block size */
        mbm_size += ((sizeof(t_bpf_ru_mblm_mbm_top_info) +
                      bpf_ru_mblm_system_data[buftype].buffer_size
                       + sizeof(t_bpf_ru_mblm_mbm_tail_info))
                     * bpf_ru_mblm_system_data[buftype].buffer_num) ;
    }

    return(mbm_size);
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Buffer Management System Information Set.
 *  @note   BPF internal function.
 *  @param  process_desc [in]  process descriptor
 *  @return result code
 *  @retval BPF_RU_MBLM_OK        normal end
 *  @retval BPF_RU_MBLM_NG        abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
/* RRH-001 MD alpha)matsuhashi add start */
int bpf_ru_mblm_set_systeminfo()
{
/* RRH-001 MD alpha)matsuhashi add end */

/* RRH-001 MD alpha)matsuhashi add start */
	/* バッファの定義値を設定する */
    /* Set data */
    bpf_ru_mblm_system_data[BPF_RU_MBLM_BUFTYPE_256].buffer_size = BPF_RU_MBLM_BUFFER_SIZE_256;
    bpf_ru_mblm_system_data[BPF_RU_MBLM_BUFTYPE_256].buffer_num  = BPF_RU_MBLM_BUFFER_NUM_128;
    bpf_ru_mblm_system_data[BPF_RU_MBLM_BUFTYPE_4096].buffer_size = BPF_RU_MBLM_BUFFER_SIZE_4096;
    bpf_ru_mblm_system_data[BPF_RU_MBLM_BUFTYPE_4096].buffer_num  = BPF_RU_MBLM_BUFFER_NUM_128;
    bpf_ru_mblm_system_data[BPF_RU_MBLM_BUFTYPE_8192].buffer_size = BPF_RU_MBLM_BUFFER_SIZE_8192;
    bpf_ru_mblm_system_data[BPF_RU_MBLM_BUFTYPE_8192].buffer_num  = BPF_RU_MBLM_BUFFER_NUM_128;
/* RRH-001 MD alpha)matsuhashi add end */

    return BPF_RU_MBLM_OK;

}
/* @} */
/* @} */
