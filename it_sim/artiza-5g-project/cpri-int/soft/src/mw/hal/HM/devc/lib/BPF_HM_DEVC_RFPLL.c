/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_RFPLL.c
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
 *  @brief  RF-PLL情報読み込み詳細(MDL内部処理)
 *  @note   RF-PLL情報読み込み詳細を行う(MDL内部処理)
 *  @param  bus            [in]   bus
 *  @param  cs             [in]   cs
 *  @param  addr           [in]   アドレス
 *  @param  data           [out]  読み込み情報
 *  @param  configSpeed    [in]   SPEED情報
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 */
/********************************************************************************************************************/
int bpf_hm_devc_spi_read_data_rfpll_detail( unsigned int bus,
                                            unsigned int cs,
                                            unsigned int addr,
                                            unsigned int *p_data,
                                            unsigned int configSpeed)
{
    unsigned int            len_dat;
    unsigned int            tx_dat[3];
    unsigned int            rx_dat[3];
    unsigned int            ret;

    /* 読み出しのための各種設定を行う   */
    tx_dat[0] = (addr & 0x0000007F) | 0x00000080;
    tx_dat[1] = 0;
    tx_dat[2] = 0;

    /* Length設定   */
    len_dat = 3;

    /* 情報を読み出す   */
    ret = bpf_hm_devc_spi_ps_spi ( bus, cs, len_dat, tx_dat, rx_dat, BPF_HM_DEVC_SPI_CS0, configSpeed);

    /* アウトパラメータに読み出し値を設定する   */
    *p_data = (rx_dat[1] << 8 & 0xFF00) | ( rx_dat[2] & 0xFF);

    return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RF-PLL読み込み
 *  @note   BPF_HM_DEVC_RFPLL_READ
 *  @param  address  [in]   RF-PLL読み込みオフセット
 *  @param  data_p   [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_RFPLL_READ(unsigned int address, unsigned int *data_p)
{
    int                         ret = 0;                            /* 戻り値                 */
    pthread_mutex_t             *mutex;                             /* mutexオブジェクト      */
    unsigned int                bus;
    unsigned int                cs;

    /* Trace log */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_RFPLL_READ address = %04x" ,address);

    /* mmw装置以外の場合NG応答 */
    if(di_devc_data_p->device_kind != D_RRH_EEP_DEV_KIND_5GDU_MMW)
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Device Kind NG:%d", di_devc_data_p->device_kind);
        ret = BPF_HM_DEVC_NG;
        return ret;
    }

    /* mutexオブジェクト */
    mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_0 ].mutex;
    /* Mutex lock */
    ret = bpf_hm_devc_mutex_lock(mutex);
    if( ret != 0 ){
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
        return BPF_HM_DEVC_MUTEX_NG;
    }

	bpf_hm_devc_pll_select(BPF_HM_DEVC_PLL_SELECT_ENB_RFPLL);

    /* 初期値設定   */
    bus = BPF_HM_DEVC_SPI_SPI0;
    cs = BPF_HM_DEVC_SPI_CS2;

    /* RF-PLL情報を読み出す */
    ret = bpf_hm_devc_spi_read_data_rfpll_detail( bus, cs, address, data_p, 0);


	bpf_hm_devc_pll_select(BPF_HM_DEVC_PLL_SELECT_DIS);


    /* Mutex Unlock */
    pthread_mutex_unlock(mutex);

	BPF_COM_LOG_SPI( BPF_HM_DEVC_SPI0_SS2, D_RRH_READ, address, *data_p );

    return ret;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RF-PLL情報書き込み詳細(MDL内部処理)
 *  @note   RF-PLL情報書き込み詳細を行う(MDL内部処理)
 *  @param  bus            [in]   bus
 *  @param  cs             [in]   cs
 *  @param  addr           [in]   アドレス
 *  @param  data           [in]   書き込み情報
 *  @param  configSpeed    [in]   SPEED情報
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 */
/********************************************************************************************************************/
int bpf_hm_devc_spi_write_data_rfll_detail( unsigned int bus,
                                            unsigned int cs,
                                            unsigned int addr,
                                            unsigned int data,
                                            unsigned int configSpeed)
{
    unsigned int            len_dat;
    unsigned int            tx_dat[3];
    unsigned int            rx_dat[3];
    unsigned int            ret;

    /* 書き込みのための各種設定を行う   */
    tx_dat[0] = addr & 0x0000007F;
    tx_dat[1] = (data >> 8) & 0x000000FF;
    tx_dat[2] = data & 0x000000FF;

    /* Length設定   */
    len_dat = 3;

    /* 情報を書き込む   */
    ret = bpf_hm_devc_spi_ps_spi ( bus, cs, len_dat, tx_dat, rx_dat, BPF_HM_DEVC_SPI_CS0, configSpeed);

    return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RF-PLL書き込み
 *  @note   BPF_HM_DEVC_RFPLL_WRITE
 *  @param  address  [in]   RF-PLL書き込みオフセット
 *  @param  data_p   [out]  書き込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_RFPLL_WRITE(unsigned int address, unsigned int data)
{
    /* Trace log */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_RFPLL_WRITE address = %04x" ,address);

    int                         ret = 0;                            /* 戻り値                 */
    pthread_mutex_t             *mutex;                             /* mutexオブジェクト      */
    unsigned int                bus;
    unsigned int                cs;

    /* mmw装置以外の場合NG応答 */
    if(di_devc_data_p->device_kind != D_RRH_EEP_DEV_KIND_5GDU_MMW)
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Device Kind NG:%d", di_devc_data_p->device_kind);
        ret = BPF_HM_DEVC_NG;
        return ret;
    }

    mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_0 ].mutex;           /* mutexオブジェクト      */
    /* Mutex lock */
    ret = bpf_hm_devc_mutex_lock(mutex);
    if( ret != 0 ){
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
        return BPF_HM_DEVC_MUTEX_NG;
    }

	bpf_hm_devc_pll_select(BPF_HM_DEVC_PLL_SELECT_ENB_RFPLL);

    /* 初期値設定   */
    bus = BPF_HM_DEVC_SPI_SPI0;
    cs = BPF_HM_DEVC_SPI_CS2;

    /* RF-PLL情報を書き込む */
    ret = bpf_hm_devc_spi_write_data_rfll_detail( bus, cs, address, data, 0);

	bpf_hm_devc_pll_select(BPF_HM_DEVC_PLL_SELECT_DIS);

    /* Mutex Unlock */
    pthread_mutex_unlock(mutex);

	BPF_COM_LOG_SPI( BPF_HM_DEVC_SPI0_SS2, D_RRH_WRITE, address, data );

    return ret;
}

/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
