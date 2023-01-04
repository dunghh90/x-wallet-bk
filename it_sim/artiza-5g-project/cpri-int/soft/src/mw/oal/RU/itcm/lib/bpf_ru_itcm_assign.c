/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_ru_itcm_assign.c
 *  @brief  Inter Thread Communication Management Initialize.
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_ITCM /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_l_com.h"
#include "bpf_l_ru_itcm.h"
#include "bpf_i_ru_itcm.h"


int bpf_itcm_sem_init( enum itcm_sem_type, int );
int bpf_itcm_msgq_init( enum itcm_msgq_type, int );
int bpf_itcm_msgq_statis_shmalloc( int proc_desc, int msgq_num);

/** @addtogroup BPF_RU_ITCM
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Thread Communication Management Initialize.
 *  @note   BPF internal function. 
 *  @param  process_desc       [in]  process descriptor
 *  @return result code
 *  @retval BPF_RU_ITCM_OK        normal end
 *  @retval BPF_RU_ITCM_ASSIGN_NG abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int bpf_ru_itcm_assign(unsigned int process_desc)
{
    int ret;
    int proc_kind; 
    enum itcm_sem_type sem_type;
    enum itcm_msgq_type mq_type;
/* RRH-001 MD alpha)matsuhashi add start */
    int proc_seq;
    int cnt;
/* RRH-001 MD alpha)matsuhashi add end */

    
    /* Get the current process kind */
    proc_kind = bpf_com_get_proc_tkind(process_desc);
    switch( proc_kind ){
/* RRH-001 MD alpha)matsuhashi add start */
        case BPF_COM_PTKIND_APL:
        case BPF_COM_PTKIND_PF:
            sem_type = ITCM_SEM_USER;
            mq_type  = ITCM_MSGQ_USER;
/* RRH-001 MD alpha)matsuhashi add end */
            break;
        default:
            return BPF_RU_ITCM_ASSIGN_NG;
    }
    

    /********************************************
    * SG data get processing.                   *
    ********************************************/
/* RRH-001 MD alpha)matsuhashi add start */
    /* Get the Current proccess Seq No */
    proc_seq = bpf_com_get_proc_num(process_desc);
    
    /* スレッド間セマフォ最大使用数の設定 */
    sg.sg_info[proc_seq].sem_info.itcm_sem_max = ITCM_SEM_MAX;

    /* スレッド間メッセージ管理数の設定 */
    sg.sg_info[proc_seq].msgq_info.itcm_msg_qnum = ITCM_QUEUE_NUM_MAX;
    
    for (cnt = 0 ; cnt < sg.sg_info[proc_seq].msgq_info.itcm_msg_qnum ; cnt ++ )
    {
        /* スレッド間メッセージQueue数の設定 */
        sg.sg_info[proc_seq].msgq_info.info[cnt].maxqlen = ITCM_MSG_MAXQLEN;

        /* スレッド間メッセージQueue長の設定 */
        sg.sg_info[proc_seq].msgq_info.info[cnt].maxmsglen = ITCM_MSG_MAXMSGLEN;
    }
    
    bpf_ru_itcm_sg_data_p = &sg;
    
/* RRH-001 MD alpha)matsuhashi add end */
    
    /********************************************
    * semaphore initialize processing.          *
    ********************************************/
    ret = bpf_itcm_sem_init(sem_type,process_desc);
    if( ret != BPF_RU_ITCM_OK ){
        return BPF_RU_ITCM_ASSIGN_NG;
    }
    
    /********************************************
    * message queue initialize processing.      *
    ********************************************/
    ret = bpf_itcm_msgq_init(mq_type,process_desc);
    if( ret != BPF_RU_ITCM_OK ){
        return BPF_RU_ITCM_ASSIGN_NG;
    }
    
    return BPF_RU_ITCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  ITCM Semaphore Function Block Initialize.
 *  @note   BPF internal function. 
 *  @param  type           [in]  ITCM semaphor type
 *  @param  proc_desc      [in]  process descriptor
 *  @return result code
 *  @retval BPF_RU_ITCM_OK        normal end
 *  @retval BPF_RU_ITCM_NG        abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_itcm_sem_init( enum itcm_sem_type type, int proc_desc)
{
    int sem_num;
    int errcd;
    int proc_seq;
    
    /* Get the Current proccess Seq No */
    proc_seq = bpf_com_get_proc_num(proc_desc);

    memset(&itcm_sem,0,sizeof(struct itcm_sem_tbl_t));
    itcm_sem.data = NULL;
    
    if( bpf_ru_itcm_sg_data_p->
        sg_info[proc_seq].
        sem_info.itcm_sem_max == 0 ){
        
            return BPF_RU_ITCM_OK;
    }
    
    itcm_sem.max = 
        bpf_ru_itcm_sg_data_p->sg_info[proc_seq].sem_info.itcm_sem_max;
    itcm_sem.data =
        malloc(sizeof(struct itcm_sem_data_t) * itcm_sem.max);
    errcd = errno;
    
    if( itcm_sem.data == NULL ){
        bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3,
                            "malloc NG : errno = %d",errcd);
        return BPF_RU_ITCM_NG;
    }
    
    /* 
        pthread_mutex_init() Call
    */
    
    for( sem_num = 0; sem_num < itcm_sem.max; sem_num++ ){
        pthread_mutex_init(&itcm_sem.data[sem_num].sem, NULL);
        itcm_sem.data[sem_num].used = ITCM_SEM_USED;
    }
    
    /* Trace log */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"itcm assign ok mutex pn:%d max:%d sz:%d",
                        proc_seq,itcm_sem.max,
                        (sizeof(struct itcm_sem_data_t)*itcm_sem.max)); 
    
    return BPF_RU_ITCM_OK;
}
/* @} */



/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  ITCM Msgq Function Block Initialize.
 *  @note   BPF internal function. 
 *  @param  type           [in]  ITCM semaphor type
 *  @param  proc_desc      [in]  process descriptor
 *  @return result code
 *  @retval BPF_RU_ITCM_OK        normal end
 *  @retval BPF_RU_ITCM_NG        abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
/*****************************************************************************/
/*キュー管理情報                                                             */
/*┌────┐                                                               */
/*│        │                                                               */
/*├────┤                                                               */
/*│        │                                                               */
/*├────┤                                                               */
/*│        │                                                               */
/*├────┤  ┌──────────────────┐                     */
/*│        ├─┼→┬───┐←┐                    │                     */
/*├────┤  │  │      ├─┼─→┌───┐←┐  ↓                     */
/*│        ├─┘  ├───┤  │    │      ├─┼→┌───┐             */
/*└────┘      │NULL  │  │    ├───┤  │  ｜NULL  │             */
/*                  ├───┤  └──┤      │  │  ├───┤             */
/*                  │      │        ├───┤  └─┤      │             */
/*                  ├───┤        │      │      ├───┤             */
/*                  │      │        ├───┤      │      │             */
/*                  └───┘        │      │      ├───┤             */
/*                   メッセージ       └───┘      │      │             */
/*                                      メッセージ    └───┘             */
/*                                                    メッセージ             */
/*                                                                           */
/*****************************************************************************/
int bpf_itcm_msgq_init(  enum itcm_msgq_type type, int proc_desc)
{
    int errcd;
    int cnt;
    int proc_seq;
    int ret;
    
    /* Get the Current proccess Seq No */
    proc_seq = bpf_com_get_proc_num(proc_desc);

    memset(&itcm_msg,0,sizeof(struct itcm_msg_tbl_t));
    itcm_msg.msgq = NULL;
    
/* RRH-001 MD alpha)matsuhashi add start */
    /* SG  Value */
    if( bpf_ru_itcm_sg_data_p->
        sg_info[proc_seq].
        msgq_info.itcm_msg_qnum == 0 ){
        
            return BPF_RU_ITCM_OK;
    }
    
    /* msgq_num */
    itcm_msg.max  = 
        bpf_ru_itcm_sg_data_p->sg_info[proc_seq].msgq_info.itcm_msg_qnum;
    
    /* allocate dedicate msgq management area */
    itcm_msg.msgq = 
        malloc(sizeof(struct itcm_msg_data_t) * itcm_msg.max);
    errcd = errno;
    if( itcm_msg.msgq == NULL ){
        bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3,
                            "malloc NG : errno = %d", errcd);
        return BPF_RU_ITCM_NG;
    }
    
    for(cnt = 0; cnt < itcm_msg.max; cnt++ ){
        /* Max que length(num) */
        itcm_msg.msgq[cnt].maxqlen =
                bpf_ru_itcm_sg_data_p->sg_info[proc_seq].
                    msgq_info.info[cnt].maxqlen;
        
        /* Max msg length(byte) */
        itcm_msg.msgq[cnt].maxmsglen =
                bpf_ru_itcm_sg_data_p->sg_info[proc_seq].
                    msgq_info.info[cnt].maxmsglen;
        
        /* Reservation msg count */
        itcm_msg.msgq[cnt].msgcount = 0;
        
        /* Max Reservation msg count */
        itcm_msg.msgq[cnt].peakmsgnum = 0;
        
        /* used flag set */
        itcm_msg.msgq[cnt].used = ITCM_MSGQ_USED;
        
        /* mutex */
        pthread_mutex_init( &(itcm_msg.msgq[cnt].mutex), NULL );
        
        /* pthread cond state set */
        pthread_cond_init( &(itcm_msg.msgq[cnt].cond), NULL );
        
        /* Set Massage List */
        itcm_msg.msgq[cnt].head = NULL;
        itcm_msg.msgq[cnt].tail = NULL;
    }
/* RRH-001 MD alpha)matsuhashi add end */
    
    /* メッセージテーブル未使用リストの初期化 */
    pthread_mutex_init( &(itcm_msg_flist.mutex), NULL);
    pthread_cond_init( &(itcm_msg_flist.cond), NULL);
    itcm_msg_flist.head = NULL;  
    itcm_msg_flist.tail = NULL;
    
    /* Statistics information */
    itcm_msg_flist.msgcount = 0;
    itcm_msg_flist.peakmsgnum = 0;

    /* Shared memory statistics information */
    ret = bpf_itcm_msgq_statis_shmalloc(proc_desc, itcm_msg.max);
    if( ret != BPF_RU_ITCM_OK ){
        return BPF_RU_ITCM_NG;
    }
    
    /* Trace log */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"itcm assign ok mq typ:%d pn:%d qnum:%d",
                        type,proc_seq,itcm_msg.max);

    return BPF_RU_ITCM_OK;
}
/* @} */

/* @{ */
/*****************************************************************************************************
***************/
/**
 *  @brief  ITCM Msgq Allocation shared memory for Statistics .
 *  @note   BPF internal function.
 *  @param  proc_desc      [in]  process descriptor
 *  @param  msgq_num       [in]  msgqueue number
 *  @return result code
 *  @retval BPF_RU_ITCM_OK        normal end
 *  @retval BPF_RU_ITCM_NG        abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/*****************************************************************************************************
***************/
int bpf_itcm_msgq_statis_shmalloc( int proc_desc, int msgq_num)
{
    key_t ipckey;      /* IPC Key              */
    int shmid;         /* Shard memory ID      */
    int shmsize;       /* Shard memory size    */
    void *addr;

    ipckey = BPF_COM_CREATE_INTERNAL_IPCKEY(
                   BPF_COM_IPCKEY_MASK_ITCM, proc_desc, ITCM_SHMNUM_MSGQ_STATIS);

    shmsize = sizeof(struct itcm_msgq_statis_t) + 
                 sizeof(struct itcm_msgq_statis_info_t) * msgq_num;

    shmid = shmget((key_t)ipckey , (size_t)shmsize , BPF_COM_SEMFLG );	/* pgr0821 */
    if( shmid == ITCM_SYSCALL_NG ){

        bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L2,
               "statistics shmget failed. disc=%d key=%x errno=%d size=%d",
               proc_desc, ipckey, errno, shmsize);

        return BPF_RU_ITCM_NG;
    }

    addr = shmat(shmid, 0, 0);
    if( addr == (void*)ITCM_SYSCALL_NG ){ /* pgr0203 */

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
               "statistics shmat failed. disc=%d key=%x errno=%d size=%d",
               proc_desc, ipckey, errno, shmsize);

        return BPF_RU_ITCM_NG;
    }

    itcm_msgq_statis_p = (struct itcm_msgq_statis_t*)addr;

    /* Initialization statistics information area */
    memset(itcm_msgq_statis_p, 0, shmsize);
    itcm_msgq_statis_p->qnum = msgq_num;

    return BPF_RU_ITCM_OK;
}
/* @} */

/* RRH-001 MD alpha)matsuhashi add start */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Thread Communication Management UnInitialize.
 *  @note   BPF internal function. 
 *  @param  process_desc       [in]  process descriptor
 *  @return result code
 *  @retval BPF_RU_ITCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int bpf_ru_itcm_unassign(unsigned int process_desc)
{
    /* 捕捉したバッファを解放する */
    if (itcm_sem.data != NULL)
    {
        free(itcm_sem.data);
        itcm_sem.data = NULL;
    }
    if (itcm_msg.msgq != NULL)
    {
        free(itcm_msg.msgq);
        itcm_msg.msgq = NULL;
    }
    
    return BPF_RU_ITCM_OK;
}
/* @} */
/* RRH-001 MD alpha)matsuhashi add end */


/* @} */
