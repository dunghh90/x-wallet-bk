/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_FANDAC.c
 *  @brief  
 *  @date   2019/02/18 FJT)Taniguchi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2019
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
 *  @brief  FANDAC書き込み
 *  @note   BPF_HM_DEVC_FANDAC_WRITE
 *  @param  address  [in]   FANDAC書き込みオフセット
 *  @param  data     [out]  書き込みデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2019/02/18 FJT)Taniguchi create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_FANDAC_WRITE(unsigned int data)
{
	int ret;

	/* Trace log */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_FANDAC_WRITE data = 0x%08x",data);

	pthread_mutex_t* mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_2 ].mutex;			/* mutexオブジェクト      */
	/* Mutex lock */
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}

	ret = bpf_hm_devc_spi_pl_spi_fandac(data);

	/* Mutex Unlock */
	pthread_mutex_unlock(mutex);

	BPF_COM_LOG_SPI( BPF_HM_DEVC_SPI2_SS0 , D_RRH_WRITE, 0, data );

	return ret;
}

/* @} */

/* @} */
