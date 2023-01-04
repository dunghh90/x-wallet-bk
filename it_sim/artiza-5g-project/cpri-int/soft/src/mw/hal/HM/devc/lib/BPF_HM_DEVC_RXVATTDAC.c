/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_RXVATTDAC.c
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
 *  @brief  RxVATT_DAC読み込み
 *  @note   BPF_HM_DEVC_RXVATTDAC_READ
 *  @param  ant      [in]   アンテナ番号
 *  @param  data_p   [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_RXVATTDAC_READ(unsigned char ant, unsigned int *data_p)
{
	if(di_devc_data_p->device_kind == D_RRH_EEP_DEV_KIND_5GDU_MMW)
	{
		int ret;
		unsigned int devkind;

		pthread_mutex_t	*mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_3 ].mutex;
		ret = bpf_hm_devc_mutex_lock(mutex);
		if( ret != 0 ){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
			return BPF_HM_DEVC_MUTEX_NG;
		}
		*data_p = di_devc_data_p->rx_vatt[ant & 3];
		if(ant == 0){
			devkind = BPF_HM_DEVC_SPI3_SS11;

		}else{
			devkind = BPF_HM_DEVC_SPI3_SS13;
		}
		
		pthread_mutex_unlock(mutex);
		BPF_COM_LOG_SPI( devkind, D_RRH_READ,ant,*data_p );
		return ret;
	}
	bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Device Kind NG:%d", di_devc_data_p->device_kind);
	return BPF_HM_DEVC_UNSUPPORT;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RxVATT_DAC書き込み
 *  @note   BPF_HM_DEVC_RXVATTDAC_WRITE
 *  @param  ant      [in]   アンテナ番号
 *  @param  data     [in]   書き込みデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_RXVATTDAC_WRITE(unsigned char ant, unsigned int data)
{
	if(di_devc_data_p->device_kind == D_RRH_EEP_DEV_KIND_5GDU_MMW)
	{
		int ret;
		unsigned int devkind;
		unsigned int write_data;
		unsigned int write_bit;

		pthread_mutex_t	*mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_3 ].mutex;
		ret = bpf_hm_devc_mutex_lock(mutex);
		if( ret != 0 ){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
			return BPF_HM_DEVC_MUTEX_NG;
		}

		write_data = 0;
		write_bit = data & 0x1;
		write_data |= (write_bit  << 8 ) | (write_bit << 9);
		write_bit = ( data & 0x2 ) >> 1;
		write_data |= (write_bit  << 10) | (write_bit << 11);
		write_bit = ( data & 0x4 ) >> 2;
		write_data |= (write_bit  << 12) | (write_bit << 13);
		write_bit = ( data & 0x8 ) >> 3;
		write_data |= (write_bit  << 14) | (write_bit << 15);
		write_bit = ( data & 0x10 ) >> 4;
		write_data |= (write_bit  << 16) | (write_bit << 17);
		write_bit = ( data & 0x20 ) >> 5;
		write_data |= (write_bit  << 18) | (write_bit << 19);

		if(ant == 0){
			bpf_hm_devc_spi_trx_spi_write_mmw(BPF_HM_DEVC_TRXSPI_TARGET_MMWRXVATTDAC_V,write_data);
			devkind = BPF_HM_DEVC_SPI3_SS11;

		}else{
			bpf_hm_devc_spi_trx_spi_write_mmw(BPF_HM_DEVC_TRXSPI_TARGET_MMWRXVATTDAC_H,write_data);
			devkind = BPF_HM_DEVC_SPI3_SS13;
		}

		di_devc_data_p->rx_vatt[ant & 3] = data;

		pthread_mutex_unlock(mutex);
		BPF_COM_LOG_SPI( devkind, D_RRH_WRITE,ant,data );

		return ret;
	}
	bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Device Kind NG:%d", di_devc_data_p->device_kind);
	return BPF_HM_DEVC_UNSUPPORT;
}
/* @} */
/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
