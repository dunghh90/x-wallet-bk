/* RRH-001 MD alpha)yokoyama add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_PTP.c
 *  @brief  
 *  @date   2015/04/07 ALPHA)yokoyama create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_HM_DEVC /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_i_hm_devc.h"
#include "BPF_COM_LOG.h"
#include <linux/types.h>
#include <linux/spi/spidev.h>

/** @addtogroup BPF_HM_DEVC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PTP情報読み込み詳細(MDL内部処理)
 *  @note   PTP情報読み込み詳細を行う(MDL内部処理)
 *  @param  bus            [in]   bus
 *  @param  cs             [in]   cs
 *  @param  addr           [in]   アドレス
 *  @param  data           [out]  読み込み情報
 *  @param  configSpeed    [in]   SPEED情報
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2014/01/15 ALPHA)Yokoyama create
 *  @date   2015/06/05 ALPHA)Yokoyama PPC 1chipから流用して修正
 */
/********************************************************************************************************************/
int bpf_hm_devc_spi_read_data_ptp_detail(	unsigned int bus,
											unsigned int cs,
											unsigned int addr,
											unsigned int *p_data,
											unsigned int configSpeed)
{
	unsigned int			len_dat;
	unsigned int			tx_dat[3];
	unsigned int			rx_dat[3];
	unsigned int			ret;
	
	/* 読み出しのための各種設定を行う	*/
	tx_dat[0] = (addr & 0x0000007F) | 0x00000080;
	tx_dat[1] = 0;
	
	/* Length設定	*/
	len_dat = 2;
	
	/* 情報を読み出す	*/
	ret = bpf_hm_devc_spi_ps_spi ( bus, cs, len_dat, tx_dat, rx_dat, BPF_HM_DEVC_SPI_CS0, configSpeed);
	
	/* アウトパラメータに読み出し値を設定する	*/
	*p_data = rx_dat[1];
	
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PTP READ
 *  @note   PTP READを行う。
 *  @param  address			[in]		address
 *  @param  data_p			[out]		data_p
 *  @return int
 *  @date   2015/04/06 ALPHA)yokoyama create
 *  @date   2015/08/04 ALPHA)ueda M-RRU-ZSYS-01654対応
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_PTP_READ (UINT address, UINT *data_p)
{
	int							ret;
	pthread_mutex_t				*mutex;								/* mutexオブジェクト      */
	unsigned int				bus;
	unsigned int				cs;
	
	/* Trace log */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_PTP_READ address=%08x",address);
	
	mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_0 ].mutex;			/* mutexオブジェクト      */
	
	/* Mutex lock */
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}
	
	/* 初期値設定	*/
	bus = BPF_HM_DEVC_SPI_SPI0;
	cs = BPF_HM_DEVC_SPI_CS0;

	/* PTP情報を読み出す	*/
	ret = bpf_hm_devc_spi_read_data_ptp_detail( bus, cs, address, data_p, 0);
	
	/* Mutex Unlock */
	pthread_mutex_unlock(mutex);
	
	/* SPIアクセス履歴を取得する	*/
//	BPF_COM_LOG_SPI( (BPF_HM_DEVC_SPI0_SS0 | BPF_HM_DEVC_CPLD_SET_PLL), D_RRH_READ, address, *data_p );
	
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PTP情報書き込み詳細(MDL内部処理)
 *  @note   PTP情報書き込み詳細を行う(MDL内部処理)
 *  @param  bus            [in]   bus
 *  @param  cs             [in]   cs
 *  @param  addr           [in]   アドレス
 *  @param  data           [in]   書き込み情報
 *  @param  configSpeed    [in]   SPEED情報
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2014/01/15 ALPHA)Yokoyama create
 *  @date   2015/06/05 ALPHA)Yokoyama PPC 1chipから流用して修正
 */
/********************************************************************************************************************/
int bpf_hm_devc_spi_write_data_ptp_detail(	unsigned int bus,
											unsigned int cs,
											unsigned int addr,
											unsigned int data,
											unsigned int configSpeed)
{
	unsigned int			len_dat;
	unsigned int			tx_dat[3];
	unsigned int			rx_dat[3];
	unsigned int			ret;
	
	/* 書き込みのための各種設定を行う	*/
	tx_dat[0] = addr & 0x0000007F;
	tx_dat[1] = data & 0x000000FF;
	
	/* Length設定	*/
	len_dat = 2;
	
	/* 情報を書き込む	*/
	ret = bpf_hm_devc_spi_ps_spi ( bus, cs, len_dat, tx_dat, rx_dat, BPF_HM_DEVC_SPI_CS0, configSpeed);
	
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PTP WRITE
 *  @note   PTP WRITEを行う。
 *  @param  address			[in]		address
 *  @param  data			[in]		data
 *  @return int
 *  @date   2015/04/07 ALPHA)yokoyama create
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_PTP_WRITE (UINT address, UINT data)
{
	int							ret;
	pthread_mutex_t				*mutex;								/* mutexオブジェクト      */
	unsigned int				bus;
	unsigned int				cs;
	
	/* Trace log */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_PTP_WRITE address=%08x data=%08x",address, data);

	mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_0 ].mutex;			/* mutexオブジェクト      */
	
	/* Mutex lock */
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}
	
	/* 初期値設定	*/
	bus = BPF_HM_DEVC_SPI_SPI0;
	cs = BPF_HM_DEVC_SPI_CS0;
	
	/* PTP情報を書き込む	*/
	ret = bpf_hm_devc_spi_write_data_ptp_detail( bus, cs, address, data, 0);
	
	/* Mutex Unlock */
	pthread_mutex_unlock(mutex);
	
	/* SPIアクセス履歴を取得する	*/
//	BPF_COM_LOG_SPI( (BPF_HM_DEVC_SPI0_SS0 | BPF_HM_DEVC_CPLD_SET_PLL), D_RRH_WRITE, address, data );
	
	return ret;
}
/* @} */

/* @} */
