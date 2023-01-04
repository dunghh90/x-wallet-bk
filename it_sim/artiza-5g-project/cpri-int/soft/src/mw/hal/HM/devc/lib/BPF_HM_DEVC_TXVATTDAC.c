/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_TXVATTDAC.c
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
 *  @brief  TxVATT_DAC読み込み
 *  @note   BPF_HM_DEVC_TXVATTDAC_READ
 *  @param  ant      [in]   アンテナ番号
 *  @param  data_p   [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_TXVATTDAC_READ(unsigned char ant, unsigned int *data_p)
{
	int ret;

	pthread_mutex_t	*mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_3 ].mutex;
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}
	*data_p = di_devc_data_p->tx_vatt[ant & 3];

	unsigned int devkind;
	switch(di_devc_data_p->device_kind){
		case D_RRH_EEP_DEV_KIND_5GDU_SUB6:
			devkind = BPF_HM_DEVC_SPI3_SS5;
			break;
		case D_RRH_EEP_DEV_KIND_5GDU_MMW:
			if(ant == 0){
				devkind = BPF_HM_DEVC_SPI3_SS10;

			}else{
				devkind = BPF_HM_DEVC_SPI3_SS12;
			}
			break;
		default:
			pthread_mutex_unlock(mutex);
			return ret;
			break;
	}
	pthread_mutex_unlock(mutex);
	BPF_COM_LOG_SPI( devkind, D_RRH_READ,ant,*data_p );
	return ret;
}
/* @} */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  TxVATT_DAC書き込み
 *  @note   BPF_HM_DEVC_TXVATTDAC_WRITE
 *  @param  ant      [in]   アンテナ番号
 *  @param  data     [in]   書き込みデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_TXVATTDAC_WRITE(unsigned char ant, unsigned int data)
{
	int ret;

	pthread_mutex_t	*mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_3 ].mutex;
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}
	unsigned int devkind;
	unsigned int write_data;
	unsigned int write_bit;

	di_devc_data_p->tx_vatt[ant & 3] = data;

	switch(di_devc_data_p->device_kind){
		case D_RRH_EEP_DEV_KIND_5GDU_SUB6:
			devkind = BPF_HM_DEVC_SPI3_SS8;
			ret = bpf_hm_devc_spi_trx_spi_write_sub6(BPF_HM_DEVC_TRXSPI_TARGET_SUB6TXVATTDAC,ant,data);
			break;
		case D_RRH_EEP_DEV_KIND_5GDU_MMW:

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
				ret = bpf_hm_devc_spi_trx_spi_write_mmw(BPF_HM_DEVC_TRXSPI_TARGET_MMWTXVATTDAC_V,write_data);
				devkind = BPF_HM_DEVC_SPI3_SS10;

			}else{
				ret = bpf_hm_devc_spi_trx_spi_write_mmw(BPF_HM_DEVC_TRXSPI_TARGET_MMWTXVATTDAC_H,write_data);
				devkind = BPF_HM_DEVC_SPI3_SS12;
			}

			break;
		default:
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Device Kind NG:%d", di_devc_data_p->device_kind);
			pthread_mutex_unlock(mutex);
			return BPF_HM_DEVC_UNSUPPORT;
			break;
	}
	pthread_mutex_unlock(mutex);
	BPF_COM_LOG_SPI( devkind, D_RRH_WRITE,ant,data );
	return ret;
}
/* @} */
/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
