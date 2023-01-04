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
 *  @brief  PTP���ǂݍ��ݏڍ�(MDL��������)
 *  @note   PTP���ǂݍ��ݏڍׂ��s��(MDL��������)
 *  @param  bus            [in]   bus
 *  @param  cs             [in]   cs
 *  @param  addr           [in]   �A�h���X
 *  @param  data           [out]  �ǂݍ��ݏ��
 *  @param  configSpeed    [in]   SPEED���
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2014/01/15 ALPHA)Yokoyama create
 *  @date   2015/06/05 ALPHA)Yokoyama PPC 1chip���痬�p���ďC��
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
	
	/* �ǂݏo���̂��߂̊e��ݒ���s��	*/
	tx_dat[0] = (addr & 0x0000007F) | 0x00000080;
	tx_dat[1] = 0;
	
	/* Length�ݒ�	*/
	len_dat = 2;
	
	/* ����ǂݏo��	*/
	ret = bpf_hm_devc_spi_ps_spi ( bus, cs, len_dat, tx_dat, rx_dat, BPF_HM_DEVC_SPI_CS0, configSpeed);
	
	/* �A�E�g�p�����[�^�ɓǂݏo���l��ݒ肷��	*/
	*p_data = rx_dat[1];
	
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PTP READ
 *  @note   PTP READ���s���B
 *  @param  address			[in]		address
 *  @param  data_p			[out]		data_p
 *  @return int
 *  @date   2015/04/06 ALPHA)yokoyama create
 *  @date   2015/08/04 ALPHA)ueda M-RRU-ZSYS-01654�Ή�
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_PTP_READ (UINT address, UINT *data_p)
{
	int							ret;
	pthread_mutex_t				*mutex;								/* mutex�I�u�W�F�N�g      */
	unsigned int				bus;
	unsigned int				cs;
	
	/* Trace log */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_PTP_READ address=%08x",address);
	
	mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_0 ].mutex;			/* mutex�I�u�W�F�N�g      */
	
	/* Mutex lock */
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}
	
	/* �����l�ݒ�	*/
	bus = BPF_HM_DEVC_SPI_SPI0;
	cs = BPF_HM_DEVC_SPI_CS0;

	/* PTP����ǂݏo��	*/
	ret = bpf_hm_devc_spi_read_data_ptp_detail( bus, cs, address, data_p, 0);
	
	/* Mutex Unlock */
	pthread_mutex_unlock(mutex);
	
	/* SPI�A�N�Z�X�������擾����	*/
//	BPF_COM_LOG_SPI( (BPF_HM_DEVC_SPI0_SS0 | BPF_HM_DEVC_CPLD_SET_PLL), D_RRH_READ, address, *data_p );
	
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PTP��񏑂����ݏڍ�(MDL��������)
 *  @note   PTP��񏑂����ݏڍׂ��s��(MDL��������)
 *  @param  bus            [in]   bus
 *  @param  cs             [in]   cs
 *  @param  addr           [in]   �A�h���X
 *  @param  data           [in]   �������ݏ��
 *  @param  configSpeed    [in]   SPEED���
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2014/01/15 ALPHA)Yokoyama create
 *  @date   2015/06/05 ALPHA)Yokoyama PPC 1chip���痬�p���ďC��
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
	
	/* �������݂̂��߂̊e��ݒ���s��	*/
	tx_dat[0] = addr & 0x0000007F;
	tx_dat[1] = data & 0x000000FF;
	
	/* Length�ݒ�	*/
	len_dat = 2;
	
	/* ������������	*/
	ret = bpf_hm_devc_spi_ps_spi ( bus, cs, len_dat, tx_dat, rx_dat, BPF_HM_DEVC_SPI_CS0, configSpeed);
	
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PTP WRITE
 *  @note   PTP WRITE���s���B
 *  @param  address			[in]		address
 *  @param  data			[in]		data
 *  @return int
 *  @date   2015/04/07 ALPHA)yokoyama create
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_PTP_WRITE (UINT address, UINT data)
{
	int							ret;
	pthread_mutex_t				*mutex;								/* mutex�I�u�W�F�N�g      */
	unsigned int				bus;
	unsigned int				cs;
	
	/* Trace log */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_PTP_WRITE address=%08x data=%08x",address, data);

	mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_0 ].mutex;			/* mutex�I�u�W�F�N�g      */
	
	/* Mutex lock */
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}
	
	/* �����l�ݒ�	*/
	bus = BPF_HM_DEVC_SPI_SPI0;
	cs = BPF_HM_DEVC_SPI_CS0;
	
	/* PTP������������	*/
	ret = bpf_hm_devc_spi_write_data_ptp_detail( bus, cs, address, data, 0);
	
	/* Mutex Unlock */
	pthread_mutex_unlock(mutex);
	
	/* SPI�A�N�Z�X�������擾����	*/
//	BPF_COM_LOG_SPI( (BPF_HM_DEVC_SPI0_SS0 | BPF_HM_DEVC_CPLD_SET_PLL), D_RRH_WRITE, address, data );
	
	return ret;
}
/* @} */

/* @} */
