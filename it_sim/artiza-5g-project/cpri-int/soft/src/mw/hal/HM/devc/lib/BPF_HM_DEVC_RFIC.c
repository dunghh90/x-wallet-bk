/* RRH-001 MD alpha)yokoyama add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_RFIC.c
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

/** @addtogroup BPF_HM_DEVC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RF-IC���ǂݍ��ݏڍ�(MDL��������)
 *  @note   RF-IC���ǂݍ��ݏڍׂ��s��(MDL��������)
 *  @param  bus            [in]   bus
 *  @param  cs             [in]   cs
 *  @param  addr           [in]   �A�h���X
 *  @param  buffer         [out]  �ǂݍ��ݏ��
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/11/21 ALPHA)Yokoyama create
 *  @date   2015/06/05 ALPHA)Yokoyama PPC 1chip���痬�p���ďC��
 */
/********************************************************************************************************************/
int bpf_hm_devc_spi_read_data_clemente_detail(	unsigned int bus,
												unsigned int cs,
												unsigned int addr,
												unsigned int *p_data,
												unsigned int configSpeed)
{
	unsigned int			len_dat;
	unsigned int			tx_dat[4];
	unsigned int			rx_dat[4];
	unsigned int			ret;
	unsigned int			address;
	
	/* �ǂݏo���̂��߂̊e��ݒ���s��	*/
	address = addr >> 8;
    tx_dat[0] = address | 0x00000080;
    tx_dat[1] = addr & 0x000000FF;
    tx_dat[2] = 0;
	
	/* Length�ݒ�	*/
	len_dat = 3;
	
	/* ����ǂݏo��	*/
	ret = bpf_hm_devc_spi_ps_spi (bus, cs, len_dat, tx_dat, rx_dat, BPF_HM_DEVC_SPI_CS0, configSpeed);
	
	/* �A�E�g�p�����[�^�ɓǂݏo���l��ݒ肷��	*/
	*p_data = rx_dat[2];
	
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RFIC READ
 *  @note   RFIC READ���s���B
 *  @param  txRx			[in]		TX/RX kind�y���g�p�z
 *  @param  unit			[in]		unit
 *  @param  address			[in]		address
 *  @param  data_p			[out]		data_p
 *  @return int
 *  @date   2015/04/06 ALPHA)yokoyama create
 *  @date   2015/06/05 ALPHA)yokoyama TDD ZYNQ�ɔ����S�ʓI�ɏC��
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE�Ή�
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_RFIC_READ(	UINT		txRx,
							UINT		unit,
							UINT		address,
							UINT		*data_p)
{
	int							ret;
	pthread_mutex_t				*mutex;								/* mutex�I�u�W�F�N�g      */
	unsigned int				bus;
	unsigned int				cs;
/*	unsigned int				devkind;	*/

/* �N�����Ԃɉe�����邽�߂ЂƂ܂��}�~																*/
	/* Trace log */
/*	bpf_com_dbglog_fmt(	BPF_COM_LOG_TRACE,														*/
/*						"BPF_HM_DEVC_RFIC_READ TxRx=%08x unit=%08x address=%08x",				*/
/*						txRx, unit, address);													*/

	/* IN�p���`�F�b�N	*/
	switch( unit )
	{
	case BPF_HM_DEVC_RFIC_UNIT_RFICALL:
		cs = BPF_HM_DEVC_SPI_CS0;
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Unit(%d) not supported, address is 0x%08x.So, only cs0 read", unit ,address);
		break;
	case BPF_HM_DEVC_RFIC_UNIT_RFIC1:
		cs = BPF_HM_DEVC_SPI_CS0;
/*		devkind = BPF_HM_DEVC_SPI1_SS0;	*/
		break;
	case BPF_HM_DEVC_RFIC_UNIT_RFIC2:
		/* OK	*/
		cs = BPF_HM_DEVC_SPI_CS1;
/*		devkind = BPF_HM_DEVC_SPI1_SS1;	*/
		break;
	default:
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "parameter unit NG = %d", unit );
		return BPF_HM_DEVC_PARAMERR;
	}

	mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_1 ].mutex;			/* mutex�I�u�W�F�N�g      */
	
	/* Mutex lock */
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}

	/* �����l�ݒ�	*/
	bus = BPF_HM_DEVC_SPI_SPI1;

	/* Clemente����ǂݏo��	*/
	ret = bpf_hm_devc_spi_read_data_clemente_detail( bus, cs, address, data_p, 0 );

	/* Mutex Unlock */
	pthread_mutex_unlock(mutex);

	/* SPI�A�N�Z�X�������擾����	*/
/*	BPF_COM_LOG_SPI( devkind, E_SPI_LV_READ_LOW , address, *data_p );	*/
	
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RFIC��񏑂����ݏڍ�(MDL��������)
 *  @note   RFIC��񏑂����ݏڍׂ��s��(MDL��������)
 *  @param  bus            [in]   bus
 *  @param  cs             [in]   cs
 *  @param  addr           [in]   �A�h���X
 *  @param  data           [in]   �������ݏ��
 *  @param  configSpeed    [in]   SPEED���
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/11/21 ALPHA)Yokoyama create
 *  @date   2015/06/05 ALPHA)Yokoyama PPC 1chip���痬�p���ďC��
 */
/********************************************************************************************************************/
int bpf_hm_devc_spi_write_data_rfic_detail(	unsigned int bus,
											unsigned int cs,
											unsigned int addr,
											unsigned int data,
											unsigned int configSpeed)
{
	unsigned int			len_dat;
	unsigned int			tx_dat[4];
	unsigned int			rx_dat[4];
	unsigned int			ret;
	
	/* �����o���̂��߂̊e��ݒ���s��	*/
	tx_dat[0] = (addr >> 8) & 0x000000FF;
	tx_dat[1] = addr & 0x000000FF;
	tx_dat[2] = data & 0x000000FF;
	
	/* Length�ݒ�	*/
	len_dat = 3;
	
	/* ���������o��	*/
	ret = bpf_hm_devc_spi_ps_spi (bus, cs, len_dat, tx_dat, rx_dat, BPF_HM_DEVC_SPI_CS0, configSpeed);
	
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RFIC WRITE
 *  @note   RFIC WRITE���s���B
 *  @param  txRx			[in]		TX/RX kind�y���g�p�z
 *  @param  unit			[in]		unit
 *  @param  address			[in]		address
 *  @param  data			[in]		data
 *  @return int
 *  @date   2015/04/07 ALPHA)yokoyama create
 *  @date   2015/06/05 ALPHA)yokoyama TDD ZYNQ�ɔ����S�ʓI�ɏC��
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE�Ή�
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_RFIC_WRITE(	UINT		txRx,
							UINT		unit,
							UINT		address,
							UINT		data )
{
	int							ret;
	pthread_mutex_t				*mutex;								/* mutex�I�u�W�F�N�g      */
	unsigned int				bus;
	unsigned int				cs;
/*	unsigned int				devkind;	*/

/* �N�����Ԃɉe�����邽�߂ЂƂ܂��}�~																*/
	/* Trace log */
/*	bpf_com_dbglog_fmt(	BPF_COM_LOG_TRACE,															*/
/*						"BPF_HM_DEVC_RFIC_WRITE TxRx=%08x unit=%08x address=%08x data=%08x",		*/
/*						txRx, unit, address, data);													*/

	/* IN�p���`�F�b�N	*/
	switch( unit )
	{
	case BPF_HM_DEVC_RFIC_UNIT_RFICALL:
		ret = BPF_HM_DEVC_RFIC_WRITE(BPF_HM_DEVC_RFIC_TXRX_KIND_TX,BPF_HM_DEVC_RFIC_UNIT_RFIC1,address,data);
		if(ret == BPF_HM_DEVC_COMPLETE){
			ret = BPF_HM_DEVC_RFIC_WRITE(BPF_HM_DEVC_RFIC_TXRX_KIND_TX,BPF_HM_DEVC_RFIC_UNIT_RFIC2,address,data);
		}else{
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_RFIC_WRITE unit NG = %d %d 0x%08x 0x%08x", unit ,ret ,address,data);
		}
		/* OK   */
		return ret;
		break;
	case BPF_HM_DEVC_RFIC_UNIT_RFIC1:
		cs = BPF_HM_DEVC_SPI_CS0;
/*		devkind = BPF_HM_DEVC_SPI1_SS0;	*/
		break;
	case BPF_HM_DEVC_RFIC_UNIT_RFIC2:
		/* OK	*/
		cs = BPF_HM_DEVC_SPI_CS1;
/*		devkind = BPF_HM_DEVC_SPI1_SS1;	*/
		break;
	default:
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "parameter unit NG = %d", unit );
		return BPF_HM_DEVC_PARAMERR;
	}
	
	mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_1 ].mutex;			/* mutex�I�u�W�F�N�g      */
	
	/* Mutex lock */
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}

	/* �����l�ݒ�	*/
	bus = BPF_HM_DEVC_SPI_SPI1;

	/* Clemente���������o��	*/
	ret = bpf_hm_devc_spi_write_data_rfic_detail( bus, cs, address, data, 0 );
	
	/* Mutex Unlock */
	pthread_mutex_unlock(mutex);
	
	/* SPI�A�N�Z�X�������擾����	*/
/*	BPF_COM_LOG_SPI( devkind, E_SPI_LV_WRITE_LOW, address, data );	*/
	
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RFIC RESET
 *  @note   RFIC RESET���s���B
 *  @param  txRx			[in]		TX/RX kind�y���g�p�z
 *  @param  unit			[in]		unit
 *  @return int
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_RFIC_RESET(	UINT		txRx,
							UINT		unit )
{
	return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @} */
