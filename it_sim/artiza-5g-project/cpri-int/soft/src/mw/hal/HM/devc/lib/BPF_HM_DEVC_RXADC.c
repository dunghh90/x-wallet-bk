/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_RXADC.c
 *  @brief  
 *  @date   2018/08/02 KCN)Nanjo create
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
 *  @brief  RXADC読み込み
 *  @note   BPF_HM_DEVC_RXADC_READ
 *  @param  address  [in]   RXADC読み込みオフセット
 *  @param  data_p   [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_RXADC_READ(unsigned int address, unsigned int *data_p)
{
    /* Trace log */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_RXADC_READ address = %04x" ,address);

    int             ret = 0;        /* 戻り値               */
    unsigned int    len_dat;        /* Length設定           */
    unsigned int    tx_dat;         /* 送信データ設定       */
    unsigned int    target;         /* ターゲット設定       */
    unsigned int    rx_data_base;   /* 送信データ設定ベース */
    unsigned int    adderss_base;   /* 送信データ設定ベース */

    /* mmw装置以外の場合NG応答 */
    if(di_devc_data_p->device_kind != D_RRH_EEP_DEV_KIND_5GDU_MMW)
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Device Kind NG:%d", di_devc_data_p->device_kind);
        ret = BPF_HM_DEVC_NG;
        return ret;
    }

    /* 送信データ設定ベース */
    rx_data_base = 0x08000000;

    /* ターゲット設定 */
    target = BPF_HM_DEVC_SPI_TYPE_RXADC;

    /* Length設定 */
    len_dat = 1;

    /* 送信データ設定*/
    adderss_base = address & 0x7FFF;
    tx_dat       = rx_data_base | (adderss_base << 12);

	pthread_mutex_t* mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_2 ].mutex;			/* mutexオブジェクト      */
	/* Mutex lock */
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}

    /* RXADC書き込み処理を実施 */
    ret = bpf_hm_devc_spi_pl_spi(len_dat, &tx_dat, target, data_p);

	/* Mutex Unlock */
	pthread_mutex_unlock(mutex);

	BPF_COM_LOG_SPI( BPF_HM_DEVC_SPI2_SS2 , D_RRH_READ, address, *data_p );

    return ret;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RXADC書き込み
 *  @note   BPF_HM_DEVC_RXADC_WRITE
 *  @param  address  [in]   RXADC書き込みオフセット
 *  @param  data     [out]  書き込みデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_RXADC_WRITE(unsigned int address, unsigned int data)
{
    /* Trace log */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_RXADC_WRITE address = %04x" ,address);

    int             ret = 0;        /* 戻り値               */
    unsigned int    len_dat;        /* Length設定           */
    unsigned int    tx_dat;         /* 送信データ設定       */
    unsigned int    target;         /* ターゲット設定       */
    unsigned int    rx_data_base;   /* 送信データ設定ベース */
    unsigned int    adderss_base;   /* 送信データ設定ベース */
    unsigned int    data_base;      /* 送信データ設定ベース */
    unsigned int    rx_data;        /* read値格納用(未使用) */

    /* mmw装置以外の場合NG応答 */
    if(di_devc_data_p->device_kind != D_RRH_EEP_DEV_KIND_5GDU_MMW)
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Device Kind NG:%d", di_devc_data_p->device_kind);
        ret = BPF_HM_DEVC_NG;
        return ret;
    }

    /* 送信データ設定ベース */
    rx_data_base = 0x00000000;

    /* ターゲット設定 */
    target = BPF_HM_DEVC_SPI_TYPE_RXADC;

    /* Length設定 */
    len_dat = 1;

    /* 送信データ設定*/
    adderss_base = address & 0x7FFF;
    data_base    = data & 0xFF;
    tx_dat       = rx_data_base | (adderss_base << 12) | (data_base << 4);

	pthread_mutex_t* mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_2 ].mutex;			/* mutexオブジェクト      */
	/* Mutex lock */
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}

    /* RXADC書き込み処理を実施 */
    ret = bpf_hm_devc_spi_pl_spi(len_dat, &tx_dat, target, &rx_data);

	/* Mutex Unlock */
	pthread_mutex_unlock(mutex);

	BPF_COM_LOG_SPI( BPF_HM_DEVC_SPI2_SS2 , D_RRH_WRITE, address, data );


    return ret;
}

/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
