/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_QSPI_FLASH_READ.c
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
#include <ctype.h>

/** @addtogroup BPF_HM_DEVC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  QSPI-FLASH読み込み
 *  @note   QSPI-FlashのREAD関数を具備する。
 *  @param  face    [in]   Flash面
 *  @param  offset  [in]   Flash読み込みオフセット
 *  @param  size    [in]   読み込みサイズ
 *  @param  buffer  [out]  読み込みデータバッファ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2013/11/22 ALPHA)Yokoyama modify for docomo ZYNQ
 *  @date   2015/05/21 ALPHA)Murakami M-RRU-ZSYS-01584対応
 *  @date   2015/08/04 ALPHA)Ueda M-RRU-ZSYS-01654対応
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_read(unsigned int face,unsigned int offset,unsigned int size,unsigned char *buffer)
{
    int file_descriptor;                            /* ファイルディスクリプタ */
    off_t off_result;                               /* offset バイト数        */
    ssize_t count_result;                           /* バイト数               */
    pthread_mutex_t  *mutex;                        /* mutexオブジェクト      */
    int ret;                                        /* 戻り値                 */
    unsigned int offset_start;                      /* 開始位置               */
    unsigned int readsize;                          /* 読み込みサイズ         */
    unsigned int totalsize;                         /* 全体サイズ             */
    unsigned int start;                             /* 開始位置               */

    /* pointer check */
    if( buffer == NULL )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "buffer == NULL");
        return BPF_HM_DEVC_NG;
    }

    /* 初期化処理 */
    off_result = 0; 
    count_result = 0;
    ret = 0;
    readsize = 0;
    totalsize = 0;
    start = 0;

    /* Trace log */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_QSPI_FLASH_READ face = %d , offset = %08x , size = %d",
                        face,offset,size);

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
    totalsize = offset_start + size;
    for( start = offset_start; start < totalsize; start += BPF_HM_DEVC_FLASH_SECTOR )
    {
        /* 1Mサイズで分割して読み込む */

        /* ファイルディスクリプタをオフセット位置に移動する */
        /* システムコール file lseekを実行する              */
        off_result = lseek(file_descriptor,  /* ファイルディスクリプタ (I) */ 
                           start          ,  /* オフセット             (I) */
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
        
        if ( totalsize - start > BPF_HM_DEVC_FLASH_SECTOR)
        {
            readsize = BPF_HM_DEVC_FLASH_SECTOR;
        }
        else
        {
            readsize = totalsize - start;
        }

        /************************************************
        * file read processing.                         *
        ************************************************/
        /* システムコール file readを実行する。*/
        count_result = read(file_descriptor,   /* ファイルディスクリプタ (I) */ 
                            buffer,            /* 読み出しデータ格納先 (I/O) */ 
                            readsize);         /* 読み出しサイズ         (I) */

        /* file readの結果を判定する */
        if (count_result == BPF_HM_DEVC_SYS_NG){
			/* ファイルを閉じる */
			close(file_descriptor);
        	
            /* Mutex Unlock */
            pthread_mutex_unlock(mutex);

            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "read. file_descriptor = %d, count_result = %d, readsize = 0x%08x error = %d",
                file_descriptor, count_result,readsize,errno);

            /* file read失敗 */
            return BPF_HM_DEVC_NG;
        }
        
        /* ポインタを進める */
        buffer += BPF_HM_DEVC_FLASH_SECTOR;

        /* WTDタイマのクリア */
        BPF_HM_DEVC_WDT_CLEAR();
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
int BPF_HM_DEVC_QSPI_FLASH_READ(unsigned int face,unsigned int offset,unsigned int size,unsigned char *buffer)
{
    int          rtn = BPF_HM_DEVC_NG;
	
	BPF_COM_PLOG_QSPI(E_QSPI_LV_READ_S,offset,size,0,face);

	rtn = bpf_hm_devc_qspi_flash_read( face, offset, size, buffer );
	if(rtn == BPF_HM_DEVC_COMPLETE)
	{
		BPF_COM_PLOG_QSPI(E_QSPI_LV_READ,offset,size,0,face);
	}
	
	return rtn;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
	*  @brief  QSPI-FLASH ELASE(boot version)
 *  @note   QSPI-Flashのboot versionを取得する
 *  @param  face    [in]   Flash面
 *  @param  offset  [in]   Flash読み込みオフセット
 *  @param  ver[4]  [out]  boot version(ascii)
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2016/04/19 ALPHA)fujiiy create
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_QSPI_FLASH_READ_BOOT_VER(unsigned int face,unsigned int offset,unsigned char *buffer)
{
    int             rtn = BPF_HM_DEVC_NG;
	unsigned int    uiBootId = 0;
	unsigned int    uiFsblStart = 0;
	unsigned int    uiFsblSize = 0;
	unsigned int    uiBootStartOffset = 0;
	unsigned int    uiBootVersionOffset = 0;
	unsigned char   bootVer[4];
	unsigned int    cnt;
	
	memset( bootVer, 0x00, sizeof(bootVer) );
	
	if( ((offset & ~BPF_HM_DEVC_FLASH_UL_BOUND) != BPF_HM_DEVC_QSPI_BOOT_OFFSET1) &&
		((offset & ~BPF_HM_DEVC_FLASH_UL_BOUND) != BPF_HM_DEVC_QSPI_BOOT_OFFSET2)   )
	{
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                           "offset out of range = %08x",offset);
		return BPF_HM_DEVC_NG;
	}
	
	/* boot Image Identificationチェック */
	rtn = BPF_HM_DEVC_QSPI_FLASH_READ(	face,
										offset + BPF_HM_DEVC_QSPI_BOOT_ID_OFFSET,
										sizeof(uiBootId),
										(unsigned char *)&uiBootId );
	
	if(rtn != BPF_HM_DEVC_COMPLETE)
	{
		return BPF_HM_DEVC_NG;
	}
	
	/* boot Image Identification が XLNX かどうかチェックする			*/
	/* (NG応答してしまうと呼び元でversionチェック破棄してしまうため		*/
	/*  OK応答とし、versionに0xFFFFを設定する)							*/
	if( 0 != memcmp( &uiBootId, BPF_HM_DEVC_QSPI_BOOT_ID_NAME, 4) )
	{
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
        	"boot id check NG(%08x)",uiBootId);
		memcpy( buffer, "FFFF", 4 );
		return BPF_HM_DEVC_COMPLETE;
	}
	
	/* u-boot.binヘッダーREAD */
	rtn = BPF_HM_DEVC_QSPI_FLASH_READ(	face,
										offset + BPF_HM_DEVC_QSPI_BOOT_FSBL_START,
										sizeof(uiFsblStart),
										(unsigned char *)&uiFsblStart );
	
	if(rtn != BPF_HM_DEVC_COMPLETE)
	{
		return BPF_HM_DEVC_NG;
	}
	
	/* fsbl開始位置リード */
	rtn = BPF_HM_DEVC_QSPI_FLASH_READ(	face,
										offset + BPF_HM_DEVC_QSPI_BOOT_FSBL_SIZE,
										sizeof(uiFsblSize),
										(unsigned char *)&uiFsblSize );
	
	if(rtn != BPF_HM_DEVC_COMPLETE)
	{
		return BPF_HM_DEVC_NG;
	}
	
	/* FSBL開始位置 + FSBLサイズ + 64Byteオフセット = Boot開始位置 */
	/* (u-bootは、64byte境界のところに格納されているため、fsblサイズ + 足りない分は 0xFF詰めされている) */
	uiBootStartOffset = uiFsblStart + uiFsblSize + (64 - ((uiFsblSize - uiFsblStart) % 64));
	uiBootVersionOffset = uiBootStartOffset + BPF_HM_DEVC_QSPI_BOOT_VER;
	
	/* bootが壊れていた場合、異常アクセスがないようにガードする */
	if ( uiBootVersionOffset >= ( offset + 0x100000))
	{
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
        	"flash boot pos NG(%08x)",uiBootVersionOffset);
		memcpy( buffer, "FFFF", 4 );
		return BPF_HM_DEVC_COMPLETE;
	}
	
	rtn = BPF_HM_DEVC_QSPI_FLASH_READ(	face,
										offset + uiBootVersionOffset,
										sizeof(bootVer),
										bootVer );
	if(rtn != BPF_HM_DEVC_COMPLETE)
	{
		return BPF_HM_DEVC_NG;
	}
	
	/* 文字列でない場合はNGする */
	for( cnt=0; cnt<4; cnt++ )
	{
		rtn = isascii( bootVer[cnt] );
		if( 0 == rtn )
		{
			bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3, "boot version is not charactor(%02x)",bootVer );
			memcpy( buffer, "FFFF", 4 );
			return BPF_HM_DEVC_COMPLETE;
		}
	}
	
	memcpy( buffer, bootVer, sizeof(bootVer) );
	
	return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  QSPI-FLASH Flash Kind読み込み
 *  @note   QSPI-FlashのFlash Kindを取得する。
 *  @param  flashKind  [out]  Flash Kind
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2017/09/20 ALPHA)Fujii Create for FlashEOL(SP3)
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_QSPI_FLASH_READ_FLASH_KIND( unsigned int *flashKind )
{
    int file_descriptor;                            /* ファイルディスクリプタ   */
    pthread_mutex_t  *mutex;                        /* mutexオブジェクト        */
    int ret;                                        /* 戻り値                   */
    erase_info_t ei;                                /* erase info(QSPIコマンド) */
    unsigned int cmdGetVal;                         /* command get value        */

    /* pointer check */
    if( flashKind == NULL )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "flashKind == NULL");
        return BPF_HM_DEVC_NG;
    }

    /* 初期化処理 */
    ret = 0;
    memset( &ei, 0x0, sizeof(ei) );
    cmdGetVal = 0;

    /* Trace log */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_QSPI_FLASH_READ_FLASH_KIND");
    
    mutex = &di_devc_data_p->info[BPF_HM_DEVC_QSPI_FLASH].mutex;          /* mutexオブジェクト      */
    /* Mutex lock */
    ret = pthread_mutex_lock(mutex);
    if( ret != 0 ){

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "pthread_mutex_lock mutex = %08x, error = %d",
            mutex,errno);

        return BPF_HM_DEVC_NG;
    }

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

    /* Flash Kind(Manufacture ID)取得 */
    ei.start = BPF_HM_DEVC_QSPICMD_READID;
    cmdGetVal = ioctl(file_descriptor, MEMUNLOCK, &ei );
    
    if( QSPI_MICRON_ID == cmdGetVal )
    {
        di_devc_data_p->ps_flash_kind = 0;  /* Micron */
        *flashKind = (unsigned int)QSPI_MICRON_ID;
    }
    else if( QSPI_MACRONIX_ID == cmdGetVal )
    {
        di_devc_data_p->ps_flash_kind = 1;  /* Macronix */
        *flashKind = (unsigned int)QSPI_MACRONIX_ID;
    }
    else
    {
        *flashKind = (unsigned int)0;
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
           "Flash Kind NG. 0x%02X", cmdGetVal );
    }
    
    /* ファイルを閉じる */
    close(file_descriptor);

    /* Mutex Unlock */
    pthread_mutex_unlock(mutex);

    return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
