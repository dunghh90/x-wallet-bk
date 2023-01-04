/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_PAVGDAC.c
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
 *  @brief  PAVG_DAC書き込み
 *  @note   BPF_HM_DEVC_PAVGDAC_WRITE
 *  @param  ant      [in]   アンテナ番号
 *  @param  data     [in]   書き込みデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_PAVGDAC_WRITE(unsigned char ant, unsigned int data){

	int ret;

	pthread_mutex_t	*mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_3 ].mutex;
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}
	unsigned int devkind;
	unsigned int write_data;
	
	switch(di_devc_data_p->device_kind){
		case D_RRH_EEP_DEV_KIND_5GDU_SUB6:
			ret = bpf_hm_devc_spi_trx_spi_write_sub6(BPF_HM_DEVC_TRXSPI_TARGET_SUB6PAVGDAC,ant,data);
			di_devc_data_p->pa_vgadac[ant & 3] = data;

			devkind = BPF_HM_DEVC_SPI3_SS9;
			break;
		case D_RRH_EEP_DEV_KIND_5GDU_MMW:

			write_data = 0;
			write_data |= ((unsigned int)ant & 3) << 22;
			write_data |= ((unsigned int)1) << 20;
			write_data |= ((unsigned int)(data & 0x3FF)) << 10;
			ret = bpf_hm_devc_spi_trx_spi_write_mmw(BPF_HM_DEVC_TRXSPI_TARGET_MMWPAVGDAC,write_data);
			di_devc_data_p->pa_vgadac[ant & 3] = data;

			devkind = BPF_HM_DEVC_SPI3_SS7;
			break;
		default:
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Device Kind NG:%d", di_devc_data_p->device_kind);
			pthread_mutex_unlock(mutex);
			return ret;
			break;
	}
	pthread_mutex_unlock(mutex);
	BPF_COM_LOG_SPI( devkind, D_RRH_WRITE,ant,data );
	return ret;
}
/* @} */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PAVG_DAC読み込み
 *  @note   BPF_HM_DEVC_PAVGDAC_READ
 *  @param  ant      [in]   アンテナ番号
 *  @param  data_p   [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_PAVGDAC_READ(unsigned char ant, unsigned int *data_p)
{
	int ret;

	pthread_mutex_t	*mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_3 ].mutex;
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}
	*data_p = di_devc_data_p->pa_vgadac[ant & 3];
	unsigned int devkind;
	switch(di_devc_data_p->device_kind){
		case D_RRH_EEP_DEV_KIND_5GDU_SUB6:
			devkind = BPF_HM_DEVC_SPI3_SS9;
			break;
		case D_RRH_EEP_DEV_KIND_5GDU_MMW:
			devkind = BPF_HM_DEVC_SPI3_SS7;
			break;
		default:
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Device Kind NG:%d", di_devc_data_p->device_kind);
			pthread_mutex_unlock(mutex);
			return ret;
			break;
	}
	pthread_mutex_unlock(mutex);
	BPF_COM_LOG_SPI( devkind, D_RRH_READ,ant,*data_p );
	return ret;
}

/* @} */
/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
