/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_ QSPI_FLASH_WRITE.c
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
 *  @brief  QSPI-FLASH書き込み
 *  @note   QSPI-FlashのWRITE関数を具備する。
 *  @param  face    [in]   Flash面
 *  @param  offset  [in]   Flash書き込みオフセット
 *  @param  size    [in]   書き込みサイズ
 *  @param  buffer  [in]   書き込みデータバッファ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2013/11/22 ALPHA)Yokoyama modify for zynq docomo
 *  @date   2015/05/21 ALPHA)Murakami M-RRU-ZSYS-01584対応
 *  @date   2015/08/04 ALPHA)Ueda M-RRU-ZSYS-01654対応
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_write(unsigned int face,unsigned int offset,unsigned int size,unsigned char *buffer)
{
    int file_descriptor;                            /* ファイルディスクリプタ */
    off_t off_result;                               /* offset バイト数        */
    ssize_t count_result;                           /* バイト数               */
    pthread_mutex_t  *mutex;                        /* mutexオブジェクト      */
    int ret;                                        /* 戻り値                 */
    unsigned int offset_start;                      /* 開始位置               */
    unsigned int writesize;                         /* 書き込みサイズ         */
    unsigned int totalsize;                         /* 全体サイズ             */
    unsigned int start;                             /* 開始位置               */
    unsigned char * tmp_buffer;
    unsigned char read_buffer[4];

    /* 初期化処理 */
    off_result = 0; 
    count_result = 0;
    ret = 0;
    writesize = 0;
    totalsize = 0;
    start = 0;
    tmp_buffer = NULL;

    /* Trace log */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_QSPI_FLASH_WRITE face = %d , offset = %08x , size = %d",
                        face,offset,size);
	
	/* boot領域のライトは要注意であるため、ログを出す */
	if( offset < BPF_HM_DEVC_FLASH_OFFSET_BOOT_END )
	{
	    bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
    	    "boot area access face = %d , offset = %08x , size = %d",
                        face,offset,size);
	}
	
    mutex = &di_devc_data_p->info[BPF_HM_DEVC_QSPI_FLASH].mutex;          /* mutexオブジェクト      */

    /* 書き込みデータ格納領域チェック */
    if (buffer == NULL){

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "buffer == NULL");

        /* 書き込みデータ無し */
        return BPF_HM_DEVC_NG;
    }

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
    totalsize = offset_start + size;
    for( start = offset_start; start < totalsize; start += BPF_HM_DEVC_FLASH_SECTOR )
    {

        /* 1Mサイズで分割して書き込む */

        /* ファイルディスクリプタをオフセット位置に移動する */
        /* システムコール file lseekを実行する              */
        off_result = lseek(file_descriptor,  /* ファイルディスクリプタ (I) */ 
                           start         ,   /* オフセット             (I) */
                           SEEK_SET);        /* シークモード           (I) */

        /* file seek結果を判定する */
        if (off_result == BPF_HM_DEVC_SYS_NG){
			/* ファイルを閉じる */
			close(file_descriptor);
        	
            /* Mutex Unlock */
            pthread_mutex_unlock(mutex);

            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "lseek. file_descriptor = %d, start = 0x%08x, error = %d",
                file_descriptor, start,errno);

            /* file seek失敗 */
            return BPF_HM_DEVC_NG;
        }

        /************************************************
        * file write processing.                        *
        ************************************************/
        if ( totalsize - start > BPF_HM_DEVC_FLASH_SECTOR)
        {
            /* 残データ量がセクタサイズを超える場合はセクタサイズ分をwriteする */
            /* システムコール file writeを実行 */
            count_result = write(file_descriptor,           /* ファイルディスクリプタ (I) */
                                 buffer,                    /* 書き込みデータ         (I) */
                                 BPF_HM_DEVC_FLASH_SECTOR); /* 読み出しサイズ         (I) */
        }
        else
        {
            /* 残データ量がセクタサイズを超えない場合は、残データに加えて */
            /* 4byteの境界まで0xFFを埋めたデータをwriteする               */
            writesize = totalsize - start;
            if ( (writesize % 4) != 0 )
            {
                /* 4byteの境界まで0xFFを埋めた場合のwrite size算出 */
                writesize = writesize + 4 - (writesize % 4);
            }

            /* 4byteの境界まで0xFFを埋めたwrite data作成 */
            tmp_buffer = malloc(writesize);
            if(tmp_buffer == NULL)
            {
				/* ファイルを閉じる */
				close(file_descriptor);
            	
                /* Mutex Unlock */
                pthread_mutex_unlock(mutex);

                bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                    "malloc failed. size = 0x%08x, errno = %d",
                    writesize,errno);
                /* malloc失敗 */
                return BPF_HM_DEVC_NG;
            }
            memset(tmp_buffer, 0xFF, writesize);
            memcpy(tmp_buffer, buffer, totalsize - start);

            /* システムコール file writeを実行 */
            count_result = write(file_descriptor,        /* ファイルディスクリプタ (I) */
                                 tmp_buffer,             /* 書き込みデータ         (I) */
                                 writesize);             /* 読み出しサイズ         (I) */

            free(tmp_buffer);
        }

        /* file writeの結果を判定する */
        if (count_result == BPF_HM_DEVC_SYS_NG || count_result == BPF_HM_DEVC_SIZE_ZERO){
			/* ファイルを閉じる */
			close(file_descriptor);
        	
            /* Mutex Unlock */
            pthread_mutex_unlock(mutex);

            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "read. file_descriptor = %d, count_result = %d, error = %d",
                file_descriptor, count_result,errno);

            /* file write失敗 */
            return BPF_HM_DEVC_NG;
        }

        /* ポインタを進める */
        buffer += BPF_HM_DEVC_FLASH_SECTOR;
        
        /* WTDタイマのクリア */
        BPF_HM_DEVC_WDT_CLEAR();
    }

    /*************************************************
    * 待ち合わせのためのQSPI FLASH 4byteダミーリード *
    *************************************************/
    off_result = lseek(file_descriptor,  /* ファイルディスクリプタ (I) */ 
                       offset_start,     /* オフセット             (I) */
                       SEEK_SET);        /* シークモード           (I) */

    /* file seek結果を判定する */
    if (off_result == BPF_HM_DEVC_SYS_NG){
		/* ファイルを閉じる */
		close(file_descriptor);
    	
        /* Mutex Unlock */
        pthread_mutex_unlock(mutex);

        /* file seek失敗 */
        return BPF_HM_DEVC_NG;
    }

    count_result = read(file_descriptor,   /* ファイルディスクリプタ (I) */ 
                        read_buffer,       /* 読み出しデータ格納先 (I/O) */ 
                        4);                /* 読み出しサイズ         (I) */

    /* file readの結果を判定する */
    if (count_result == BPF_HM_DEVC_SYS_NG){
		/* ファイルを閉じる */
		close(file_descriptor);
    	
        /* Mutex Unlock */
        pthread_mutex_unlock(mutex);

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
 *  @brief  QSPI-FLASH書き込み
 *  @note   QSPI-FlashのWRITE関数を具備する。
 *  @param  face    [in]   Flash面
 *  @param  offset  [in]   Flash書き込みオフセット
 *  @param  size    [in]   書き込みサイズ
 *  @param  buffer  [in]   書き込みデータバッファ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/01 HID)Iwabuchi modify for dual
 *  @date   2014/12/05 ALPHA)Yokoyama create
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_QSPI_FLASH_WRITE(unsigned int face,unsigned int offset,unsigned int size,unsigned char *buffer)
{
    int          rtn = BPF_HM_DEVC_NG;
	
	/* packet log send */
	BPF_COM_PLOG_QSPI(E_QSPI_LV_WRITE_S,offset,size,0,face);

	rtn = bpf_hm_devc_qspi_flash_write( face, offset, size, buffer );
	if(rtn == BPF_HM_DEVC_COMPLETE)
	{
		/* packet log send */
		BPF_COM_PLOG_QSPI(E_QSPI_LV_WRITE,offset,size,0,face);
	}
	return rtn;
}
/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
