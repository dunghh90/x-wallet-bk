/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_RESERVE.c
 *  @brief  
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_HM_DEVC /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_i_hm_devc.h"
#include "BPF_COM_LOG.h"

/** @addtogroup BPF_HM_DEVC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  IRQ割り込み待ち処理
 *  @note   ユーザから指定されたIRQ番号の割り込み待ち処理を行う。
 *          (UIOで実現するため、デバイスをOpen⇒Readする形で待ち受ける)
 *           割り込みが発生した場合は、起動元へ復帰する。
 *  @param  dev     [in]   デバイス種別
 *  @param  value   [out]  割り込み情報(要因Bit情報)
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_OPENNG
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_RESERVE(unsigned int dev)
{
    int file_descriptor;                            /* ファイルディスクリプタ */
    int ret;                                        /* 戻り値                 */
    ssize_t count_result;                           /* バイト数               */
    pthread_mutex_t  *mutex;                        /* mutexオブジェクト      */
    unsigned int icount;
    unsigned tmp;
    
    /* 初期化処理 */
    count_result = 0;
    icount = 0;
    ret = 0;

    /* Trace log */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_RESERVE dev = %d",dev);

    /* オフセット情報から、取得対象のデータの情報を取得する */
    ret = bpf_hm_dev_irq_info(dev,&file_descriptor,&mutex);
    if (ret != BPF_HM_DEVC_COMPLETE)
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "dev  NG. dev = %d",
            dev);

        /* データ取得失敗 */
        return BPF_HM_DEVC_NG;
    }

    /* Mutex lock */
    ret = bpf_hm_devc_mutex_lock(mutex);
    if( ret != 0 ){

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "pthread_mutex_lock mutex = %08x, error = %d",
            mutex,errno);

        return BPF_HM_DEVC_NG;
    }

    /************************************************
    * file seek processing.                         *
    ************************************************/
    /* ファイルディスクリプタをオフセット位置に移動する */
    /* システムコール file lseekを実行する              */

    count_result = lseek(file_descriptor,  /* ファイルディスクリプタ (I) */ 
                       0        ,          /* オフセット             (I) */
                       SEEK_SET);          /* シークモード           (I) */

    /* file seek結果を判定する */
    if (count_result == BPF_HM_DEVC_SYS_NG){

        /* Mutex Unlock */
        pthread_mutex_unlock(mutex);

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "lseek. file_descriptor = %d, offset = %08x, error = %d",
            file_descriptor, 0,errno);

        /* file seek失敗 */
        return BPF_HM_DEVC_NG;
    }

    /************************************************
    * file write processing.                        *
    ************************************************/
    /* 先頭１バイトに書き込み先のアドレスを指定 */
    tmp = 0x1;
    
    count_result = write(file_descriptor,                /* ファイルディスクリプタ (I) */
                         &tmp         ,                  /* 書き込みデータ         (I) */ 
                         sizeof(tmp));                   /* 書き込みサイズ         (I) */

    /* file writeの結果を判定する */
    if (count_result == BPF_HM_DEVC_SYS_NG || count_result == BPF_HM_DEVC_SIZE_ZERO){

        /* Mutex Unlock */
        pthread_mutex_unlock(mutex);

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "write. file_descriptor = %d, count_result = %d, error = %d",
            file_descriptor, count_result,errno);

        /* file write失敗 */
        return BPF_HM_DEVC_NG;
    }
 
    /************************************************
    * file read processing.                         *
    ************************************************/
    /* システムコール file readを実行する。*/
    count_result = read(file_descriptor,       /* ファイルディスクリプタ (I) */ 
                        &icount,               /* 読み出しデータ格納先 (I/O) */ 
                        sizeof(unsigned int)); /* 読み出しサイズ         (I) */

    /* file readの結果を判定する */
    if (count_result == BPF_HM_DEVC_SYS_NG){

        /* Mutex Unlock */
        pthread_mutex_unlock(mutex);

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "read. file_descriptor = %d, count_result = %d, error = %d",
            file_descriptor, count_result,errno);

        /* file read失敗 */
        return BPF_HM_DEVC_NG;
    }

    /* Mutex Unlock */
    pthread_mutex_unlock(mutex);

    return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
