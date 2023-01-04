/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_QDEM.c
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
 *  @brief  QDEM読み込み
 *  @note   BPF_HM_DEVC_QDEM_READ
 *  @param  ant      [in]   FPGA種別
 *  @param  sel      [in]   セレクタ番号
 *  @param  address  [in]   QDEM読み込みオフセット
 *  @param  data_p   [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_QDEM_READ(unsigned char ant, unsigned int sel, unsigned int address, unsigned int *data_p)
{
	int ret = BPF_HM_DEVC_UNSUPPORT;
	if(di_devc_data_p->device_kind != D_RRH_EEP_DEV_KIND_5GDU_MMW){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Device Kind NG:%d", di_devc_data_p->device_kind);
		return ret;
	}
	pthread_mutex_t	*mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_3 ].mutex;
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}

	unsigned int devkind;
	unsigned int write_data;

	switch(sel){
		case BPF_HM_DEVC_TRXSPI_SEL2:
			write_data = 0;
			write_data |= ((unsigned int)1) << 27;
			write_data |= (address & 0x3F) << 21;
			if(ant == 0){
				ret = bpf_hm_devc_spi_trx_spi_read_mmw(BPF_HM_DEVC_TRXSPI_TARGET_QDEM_V,write_data,data_p);
				devkind = BPF_HM_DEVC_SPI3_SS2;
			}else{
				ret = bpf_hm_devc_spi_trx_spi_read_mmw(BPF_HM_DEVC_TRXSPI_TARGET_QDEM_H,write_data,data_p);
				devkind = BPF_HM_DEVC_SPI3_SS5;
			}
			break;
		case BPF_HM_DEVC_TRXSPI_SEL3:
			*data_p = di_devc_data_p->qdem_dac[ant & 3];
			devkind = BPF_HM_DEVC_SPI3_SS6;
			break;
		default:
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Device sel NG:%d", sel);
			pthread_mutex_unlock(mutex);
			return ret;
			break;
	}

	pthread_mutex_unlock(mutex);
	BPF_COM_LOG_SPI( devkind, D_RRH_READ,
					(unsigned int)(((ant << 16 ) & 0xFFFF0000) | (address & BPF_HM_DEVC_TRXSPI_QMOD_ADDRESS_EN_BIT)),
					 *data_p );

	return ret;
	
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  QDEM書き込み
 *  @note   BPF_HM_DEVC_QDEM_WRITE
 *  @param  ant      [in]   FPGA種別
 *  @param  sel      [in]   セレクタ番号
 *  @param  address  [in]   QDEM書き込みオフセット
 *  @param  data     [in]   書き込みデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_QDEM_WRITE(unsigned char ant, unsigned int sel, unsigned int address, unsigned int data)
{
	int ret = BPF_HM_DEVC_UNSUPPORT;

	if(di_devc_data_p->device_kind != D_RRH_EEP_DEV_KIND_5GDU_MMW){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Device Kind NG:%d", di_devc_data_p->device_kind);
		return ret;
	}
	pthread_mutex_t	*mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_3 ].mutex;
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}
	unsigned int devkind;
	unsigned int write_data;

	switch(sel){
		case BPF_HM_DEVC_TRXSPI_SEL2:
			write_data = 0;
			write_data |= (address & 0x3F) << 21;
			write_data |= (data & 0xFFFF) << 5;

			if(ant == 0){
				ret = bpf_hm_devc_spi_trx_spi_write_mmw(BPF_HM_DEVC_TRXSPI_TARGET_QDEM_V,write_data);
				devkind = BPF_HM_DEVC_SPI3_SS2;
			}else{
				ret = bpf_hm_devc_spi_trx_spi_write_mmw(BPF_HM_DEVC_TRXSPI_TARGET_QDEM_H,write_data);
				devkind = BPF_HM_DEVC_SPI3_SS5;
			}
			break;
		case BPF_HM_DEVC_TRXSPI_SEL3:
			write_data = 0;
			write_data |= (((unsigned int)(ant+2)) & 3) << 22;
			write_data |= ((unsigned int)1) << 20;
			write_data |= ((unsigned int)(data & 0x3FF)) << 10;
			
			ret = bpf_hm_devc_spi_trx_spi_write_mmw(BPF_HM_DEVC_TRXSPI_TARGET_MMWQMODDAC,write_data);
			di_devc_data_p->qdem_dac[ant & 3] = data;

			devkind = BPF_HM_DEVC_SPI3_SS6;
			break;
		default:
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Device sel NG:%d", sel);
			pthread_mutex_unlock(mutex);
			return ret;
			break;
	}

	pthread_mutex_unlock(mutex);
	BPF_COM_LOG_SPI( devkind, D_RRH_WRITE,
					(unsigned int)(((ant << 16 ) & 0xFFFF0000) | (address & BPF_HM_DEVC_TRXSPI_QMOD_ADDRESS_EN_BIT)),
					 data );

	return ret;
}
/* @} */
/* @} */
