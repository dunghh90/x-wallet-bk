/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_PGCLUT.c
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

/** @addtogroup BPF_HM_DEVC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PGCLUT読み込み
 *  @note   BPF_HM_DEVC_PGCLUT_READ
 *  @param  grp     [in]   デバイスグループ
 *  @param  sa_add  [in]   SA(Device)
 *  @param  chrow   [in]   Channel & Row
 *  @param  size    [in]   サイズ
 *  @param  address [in]   PGCLUT読み込みオフセット
 *  @param  data_p  [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_PGCLUT_READ(unsigned char grp, unsigned char sa_add, unsigned char chrow, unsigned char address, unsigned int *data_p)
{
	bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"BPF_HM_DEVC_PGCLUT_READ not supported ");
	return BPF_HM_DEVC_NG;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PGCLUT書き込み
 *  @note   BPF_HM_DEVC_PGCLUT_WRITE
 *  @param  grp     [in]   デバイスグループ
 *  @param  sa_add  [in]   SA(Device)
 *  @param  chrow   [in]   Channel & Row
 *  @param  address [in]   PGCLUT書き込みオフセット
 *  @param  kind    [in]   装置種別
 *  @param  data    [out]  書き込みデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_PGCLUT_WRITE(unsigned char grp, unsigned char sa_add, unsigned char chrow, unsigned char address, unsigned int data)
{
	bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"BPF_HM_DEVC_PGCLUT_WRITE not supported ");
	return BPF_HM_DEVC_NG;
}

/* @} */



/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PGCLUT書き込み2
 *  @note   BPF_HM_DEVC_PGCLUT_WRITE2
 *  @param  grp     [in]   デバイスグループ
 *  @param  sa_add  [in]   SA(Device)
 *  @param  chrow   [in]   Channel & Row
 *  @param  address [in]   PGCLUT書き込みオフセット
 *  @param  kind    [in]   装置種別
 *  @param  data    [out]  書き込みデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2019/02/19 FJT)Taniguchi create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_PGCLUT_WRITE2(unsigned int data_len, unsigned char* data_p)
{
	unsigned int offset = BPF_HM_DEVC_SPI_PGCLUT_OFFSET_START;
	unsigned int count;
	unsigned int max_count;
	int		ret;

	max_count = (BPF_HM_DEVC_SPI_PGCLUT_OFFSET_END - BPF_HM_DEVC_SPI_PGCLUT_OFFSET_START) / 4;

	pthread_mutex_t	*mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_4 ].mutex;
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}
	if(data_len == 0){
		for(count=0; count < max_count ; count++){
			bpf_hm_devc_spi_pl_spi_reg_write(offset,0);
			offset += 4;
			if(offset >= BPF_HM_DEVC_SPI_PGCLUT_OFFSET_END){
				break;
			}
		}

	}else{
		unsigned int remain_count = 0;
		if(max_count > (data_len/4)){
			remain_count = max_count - (data_len/4);
			max_count = (data_len/4);
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Warning length len:0x%08x(%d) 0x%08x(%d)",data_len,data_len,remain_count,remain_count);
		}

		unsigned int* write_data_p = (unsigned int*)data_p;
		for(count=0; count < max_count ; count++,write_data_p++){
			bpf_hm_devc_spi_pl_spi_reg_write(offset,*write_data_p);
			offset += 4;
			if(offset >= BPF_HM_DEVC_SPI_PGCLUT_OFFSET_END){
				break;
			}
		}

		for(count=0; count < remain_count ; count++){
			bpf_hm_devc_spi_pl_spi_reg_write(offset,0);
			offset += 4 ;
			if(offset >= BPF_HM_DEVC_SPI_PGCLUT_OFFSET_END){
				break;
			}
		}
	}

	bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"BPF_HM_DEVC_PGCLUT_WRITE2 len:0x%08x(%d) 0x%08x(%d)",data_len,data_len,max_count,max_count);

	pthread_mutex_unlock(mutex);

	return BPF_HM_DEVC_COMPLETE;
}

/* @} */



/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
