/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_QSPI_FLASH_ERASE.c
 *  @brief  
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013-2015
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_HM_DEVC /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_i_hm_devc.h"

/** @addtogroup BPF_HM_DEVC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
	*  @brief  QSPI-FLASH ELASE
	*  @note   QSPI-FlashのELASE関数を具備する
 *  @param  face    [in]   Flash面
 *  @param  offset  [in]   Flash読み込みオフセット
 *  @param  size    [in]   読み込みサイズ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2013/11/22 ALPHA)Yokoyama modify for zynq docomo
 *  @date   2015/05/21 ALPHA)Murakami M-RRU-ZSYS-01584対応
 *  @date   2015/08/04 ALPHA)Ueda M-RRU-ZSYS-01654対応
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_erase(unsigned int face,unsigned int offset, unsigned int size)
{
    int file_descriptor;                            /* ファイルディスクリプタ */
    off_t off_result;                               /* offset バイト数        */
    pthread_mutex_t  *mutex;                        /* mutexオブジェクト      */
    int ret;                                        /* 戻り値                 */
    mtd_info_t          mtd_info;
    erase_info_t        ei;
    unsigned int offset_start;                      /* 開始位置               */
    unsigned char buffer[4];
    ssize_t  count_result;                          /* バイト数               */

    /* 初期化処理 */
    off_result = 0; 
    ret = 0;

    /* Trace log */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_QSPI_FLASH_READ face = %d , offset = %08x , size = %d",
                        face,offset,size);

	/* boot領域のイレースは要注意であるため、ログを出す */
	if( offset < BPF_HM_DEVC_FLASH_OFFSET_BOOT_END )
	{
	    bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
    	    "boot area access face = %d , offset = %08x , size = %d",
                        face,offset,size);
	}
	
    mutex = &di_devc_data_p->info[BPF_HM_DEVC_QSPI_FLASH].mutex;          /* mutexオブジェクト      */

    /* 1エリア異常の場合はＮＧで返却する */
    offset_start = offset;
    offset_start &= ~BPF_HM_DEVC_FLASH_UL_BOUND;    /* 基準を0x00000000に戻す       */
    if (offset_start + size > BPF_HM_DEVC_FLASH_UL_BOUND)
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "Data(offset_start + size) Range NG  = %08x",offset_start + size);

        /* データ範囲外 */
        return BPF_HM_DEVC_EPARAM;
    }

    /* Mutex lock */
    ret = bpf_hm_devc_mutex_lock(mutex);
    if( ret != 0 ){

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "pthread_mutex_lock mutex = %08x, error = %d",
            mutex,errno);

        return BPF_HM_DEVC_MUTEX_NG;
    }

/* 2014.11.19 del start */
//    /* QSPI-FLASHアクセス領域指定 */
//    ret = bpf_hm_devc_select_qspi_area(face, offset);
//
//    if (ret != BPF_HM_DEVC_COMPLETE)
//    {
//        /* Mutex Unlock */
//        pthread_mutex_unlock(mutex);
//
//        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
//            "bpf_hm_devc_qspi_sect_set NG. ret = %d", ret);
//
//        /* コマンド発行失敗 */
//        return ret;
//    }
/* 2014.11.19 del end */

    file_descriptor = open(BPF_HM_DEVC_MTD0,O_RDWR);
    
    if (file_descriptor == BPF_HM_DEVC_SYS_NG)
    {
        /* Mutex Unlock */
        pthread_mutex_unlock(mutex);

        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"open NG Name = %s",BPF_HM_DEVC_MTD0);

        /* データ取得失敗 */
        return BPF_HM_DEVC_NG;
    }

/* 2014.11.19 add start */
//    /* QSPI-FLASHアクセス領域指定 */
//    ret = bpf_hm_devc_select_qspi_area2(file_descriptor, face, offset);
//
//    if (ret != BPF_HM_DEVC_COMPLETE)
//    {
//        /* ファイルを閉じる */
//        close(file_descriptor);
//
//        /* Mutex Unlock */
//        pthread_mutex_unlock(mutex);
//
//        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
//            "bpf_hm_devc_qspi_sect_set2 NG. ret = %d", ret);
//
//        /* コマンド発行失敗 */
//        return ret;
//    }
/* 2014.11.19 add end */
	/* 面指定分は開始アドレスにバウンダリー分加算 */
	offset_start = offset + (BPF_HM_DEVC_FLASH_UL_BOUND * (face & 0x01));

    /************************************************
    * file seek processing.                         *
    ************************************************/

    /* ioctl */
    ret = ioctl(file_descriptor, MEMGETINFO, &mtd_info );
    if (ret == BPF_HM_DEVC_SYS_NG){
		/* ファイルを閉じる */
		close(file_descriptor);

        /* Mutex Unlock */
        pthread_mutex_unlock(mutex);

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "ioctl.(MEMGETINFO) file_descriptor = %d, error = %d",
            file_descriptor, errno);

        /* ioctl失敗 */
        return BPF_HM_DEVC_NG;
    }

    /* ファイルディスクリプタをオフセット位置に移動する */
    /* システムコール file lseekを実行する              */

    off_result = lseek(file_descriptor,  /* ファイルディスクリプタ (I) */ 
                       0         ,       /* オフセット             (I) */
                       SEEK_SET);        /* シークモード           (I) */

    /* file seek結果を判定する */
    if (off_result == BPF_HM_DEVC_SYS_NG){
		/* ファイルを閉じる */
		close(file_descriptor);
    	
        /* Mutex Unlock */
        pthread_mutex_unlock(mutex);

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "lseek. file_descriptor = %d, offset = %08x, error = %d",
            file_descriptor, 0 ,errno);

        /* file seek失敗 */
        return BPF_HM_DEVC_NG;
    }

    ei.length = mtd_info.erasesize;
    
    /* メモリのUnlockとErase    */
    for( ei.start = offset_start; ei.start < offset_start + size; ei.start += mtd_info.erasesize )
    {

        /* ioctl */
        ret = ioctl(file_descriptor, MEMERASE, &ei );
        if (ret == BPF_HM_DEVC_SYS_NG){
			/* ファイルを閉じる */
			close(file_descriptor);
        	
            /* Mutex Unlock */
            pthread_mutex_unlock(mutex);

            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "ioctl.(MEMERASE) file_descriptor = %d, error = %d",
                file_descriptor, errno);

            /* ioctl失敗 */
            return BPF_HM_DEVC_NG;
        }

        /* WTDタイマのクリア */
        BPF_HM_DEVC_WDT_CLEAR();
    }

    /*************************************************
    * 待ち合わせのためのQSPI FLASH 4byteダミーリード *
    *************************************************/
    off_result = lseek(file_descriptor,  /* ファイルディスクリプタ (I) */ 
                       offset_start          ,  /* オフセット             (I) */
                       SEEK_SET);        /* シークモード           (I) */

    /* file seek結果を判定する */
    if (off_result == BPF_HM_DEVC_SYS_NG){
		/* ファイルを閉じる */
		close(file_descriptor);
    	
        /* Mutex Unlock */
        pthread_mutex_unlock(mutex);

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "lseek. file_descriptor = %d, offset = %08x, error = %d",
            file_descriptor, offset_start ,errno);

        /* file seek失敗 */
        return BPF_HM_DEVC_NG;
    }

    count_result = read(file_descriptor,   /* ファイルディスクリプタ (I) */ 
                        buffer,            /* 読み出しデータ格納先 (I/O) */ 
                        4);         /* 読み出しサイズ         (I) */

    /* file readの結果を判定する */
    if (count_result == BPF_HM_DEVC_SYS_NG){
		/* ファイルを閉じる */
		close(file_descriptor);
    	
        /* Mutex Unlock */
        pthread_mutex_unlock(mutex);

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "read. file_descriptor = %d, count_result = %d, readsize = 0x%08x error = %d",
            file_descriptor, count_result,4,errno);

        /* file read失敗 */
        return BPF_HM_DEVC_NG;
    }

    /* ファイルを閉じる */
    close(file_descriptor);

    /* Mutex Unlock */
    pthread_mutex_unlock(mutex);

    return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  QSPI-FLASH ELASE
 *  @note   QSPI-FlashのELASE関数を具備する。
 *  @param  face    [in]   Flash面
 *  @param  offset  [in]   Flash読み込みオフセット
 *  @param  size    [in]   読み込みサイズ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/01 HID)Iwabuchi modify for dual
 *  @date   2014/12/05 ALPHA)Yokoyama create
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_QSPI_FLASH_ERASE(unsigned int face,unsigned int offset, unsigned int size)
{
    int          rtn = BPF_HM_DEVC_NG;
	
	BPF_COM_PLOG_QSPI(E_QSPI_LV_ERASE_S,offset,size,0,face);
	
	rtn = bpf_hm_devc_qspi_flash_erase( face, offset, size );
	if(rtn == BPF_HM_DEVC_COMPLETE)
	{
		BPF_COM_PLOG_QSPI(E_QSPI_LV_ERASE,offset,size,0,face);
	}
	
	return rtn;
}
/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
