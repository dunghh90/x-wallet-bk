/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_com_log_assign.c
 *  @brief  Common Log Assgin.
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_COM /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_l_com.h"
#include "bpf_i_com.h"


/** @addtogroup BPF_COM_LOG
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Common Log Assign.
 *  @note   BPF internal function. 
 *  @param  process_desc       [in]  process descriptor
 *  @return result code
 *  @retval BPF_COM_LOG_INTER_OK    normal end
 *  @retval BPF_COM_LOG_INTER_ERR_SYS
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 */
/********************************************************************************************************************/
int bpf_com_log_assign(
        unsigned int process_desc)
{   
    key_t ipc_key;
    int shmid;
    
    /* 既にアタッチ済みの場合は処理終了 */
    if(di_log_assign_flag == LOG_FLG_ON){
        return BPF_COM_LOG_INTER_OK;
    }
    
    /********************************************
    * IPC key create processing.                *
    ********************************************/
    /* IPCKEY 生成マクロ  */
    ipc_key  = BPF_COM_CREATE_INTERNAL_IPCKEY(BPF_COM_IPCKEY_MASK_LOG,0,0);
    
    /********************************************
    * shared memory get processing.             *
    ********************************************/
    shmid = shmget( ipc_key,
                    sizeof(t_bpf_ru_log_data),
                    BPF_COM_SEMFLG);
    if( shmid == BPF_LOGFUNC_SYSCALL_NG ){

        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"shmget ipckey=%X size=%d errno=%d",
                            ipc_key,sizeof(t_bpf_ru_log_data),errno);
        return BPF_COM_LOG_INTER_ERR_SYS;
    }
        
    /********************************************
    * shared memory attach processing.          *
    ********************************************/
    di_log_data_p = (t_bpf_ru_log_data *)shmat(shmid,0,0);
    if(di_log_data_p == (void *)BPF_LOGFUNC_SYSCALL_NG){	/* pgr0203 */
        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"shmat ipckey=%X size=%d errno=%d shmid=%d",
                            ipc_key,sizeof(t_bpf_ru_log_data),errno,shmid);
        return BPF_COM_LOG_INTER_ERR_SYS;
    }
                
    /* アサインフラグ */
    di_log_assign_flag = LOG_FLG_ON;

    
    return BPF_COM_LOG_INTER_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Common Log Assign Init
 *  @note   BPF internal function. 
 *  @param  process_desc       [in]  process descriptor
 *  @return result code
 *  @retval BPF_COM_LOG_INTER_OK   normal end
 *  @retval BPF_COM_LOG_INTER_ERR_SYS
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 */
/********************************************************************************************************************/
int bpf_com_log_init(
        unsigned int process_desc)
{   
    key_t ipc_key;
    int shmid;
    pthread_mutexattr_t mattr;
        
    /********************************************
    * IPC key create processing.                *
    ********************************************/
    /* IPCKEY 生成マクロ  */
    ipc_key  = BPF_COM_CREATE_INTERNAL_IPCKEY(BPF_COM_IPCKEY_MASK_LOG,0,0);
    
    /********************************************
    * shared memory get processing.             *
    ********************************************/
    shmid = shmget( ipc_key,
                    sizeof(t_bpf_ru_log_data),
                    BPF_COM_SEMFLG);
    if( shmid == BPF_LOGFUNC_SYSCALL_NG ){

        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"shmget ipckey=%X size=%d errno=%d",
                            ipc_key,sizeof(t_bpf_ru_log_data),errno);
        return BPF_COM_LOG_INTER_ERR_SYS;
    }
        
    /********************************************
    * shared memory attach processing.          *
    ********************************************/
    di_log_data_p = (t_bpf_ru_log_data *)shmat(shmid,0,0);
    if(di_log_data_p == (void *)BPF_LOGFUNC_SYSCALL_NG){	/* pgr0203 */
        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"shmat ipckey=%X size=%d errno=%d shmid=%d",
                            ipc_key,sizeof(t_bpf_ru_log_data),errno,shmid);
        return BPF_COM_LOG_INTER_ERR_SYS;
    }
    
    /* mutexオブジェクトの初期化 */
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr,PTHREAD_PROCESS_SHARED);
    pthread_mutex_init( &(di_log_data_p->mutex), &mattr);
    
    /* ログ格納数を0に設定 */
    di_log_data_p->index = 0; 

    /* 前回情報Noを0に設定 */
    di_log_data_p->pre_no = 0;

    /* アサインフラグ */
    di_log_assign_flag = LOG_FLG_ON;

    return BPF_COM_LOG_INTER_OK;
}
/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
