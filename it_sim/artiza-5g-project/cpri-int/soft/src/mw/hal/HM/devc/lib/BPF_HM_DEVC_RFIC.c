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
 *  @brief  RF-IC情報読み込み詳細(MDL内部処理)
 *  @note   RF-IC情報読み込み詳細を行う(MDL内部処理)
 *  @param  bus            [in]   bus
 *  @param  cs             [in]   cs
 *  @param  addr           [in]   アドレス
 *  @param  buffer         [out]  読み込み情報
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/11/21 ALPHA)Yokoyama create
 *  @date   2015/06/05 ALPHA)Yokoyama PPC 1chipから流用して修正
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
	
	/* 読み出しのための各種設定を行う	*/
	address = addr >> 8;
    tx_dat[0] = address | 0x00000080;
    tx_dat[1] = addr & 0x000000FF;
    tx_dat[2] = 0;
	
	/* Length設定	*/
	len_dat = 3;
	
	/* 情報を読み出す	*/
	ret = bpf_hm_devc_spi_ps_spi (bus, cs, len_dat, tx_dat, rx_dat, BPF_HM_DEVC_SPI_CS0, configSpeed);
	
	/* アウトパラメータに読み出し値を設定する	*/
	*p_data = rx_dat[2];
	
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RFIC READ
 *  @note   RFIC READを行う。
 *  @param  txRx			[in]		TX/RX kind【未使用】
 *  @param  unit			[in]		unit
 *  @param  address			[in]		address
 *  @param  data_p			[out]		data_p
 *  @return int
 *  @date   2015/04/06 ALPHA)yokoyama create
 *  @date   2015/06/05 ALPHA)yokoyama TDD ZYNQに伴い全面的に修正
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_RFIC_READ(	UINT		txRx,
							UINT		unit,
							UINT		address,
							UINT		*data_p)
{
	int							ret;
	pthread_mutex_t				*mutex;								/* mutexオブジェクト      */
	unsigned int				bus;
	unsigned int				cs;
/*	unsigned int				devkind;	*/

/* 起動時間に影響するためひとまず抑止																*/
	/* Trace log */
/*	bpf_com_dbglog_fmt(	BPF_COM_LOG_TRACE,														*/
/*						"BPF_HM_DEVC_RFIC_READ TxRx=%08x unit=%08x address=%08x",				*/
/*						txRx, unit, address);													*/

	/* INパラチェック	*/
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

	mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_1 ].mutex;			/* mutexオブジェクト      */
	
	/* Mutex lock */
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}

	/* 初期値設定	*/
	bus = BPF_HM_DEVC_SPI_SPI1;

	/* Clemente情報を読み出す	*/
	ret = bpf_hm_devc_spi_read_data_clemente_detail( bus, cs, address, data_p, 0 );

	/* Mutex Unlock */
	pthread_mutex_unlock(mutex);

	/* SPIアクセス履歴を取得する	*/
/*	BPF_COM_LOG_SPI( devkind, E_SPI_LV_READ_LOW , address, *data_p );	*/
	
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RFIC情報書き込み詳細(MDL内部処理)
 *  @note   RFIC情報書き込み詳細を行う(MDL内部処理)
 *  @param  bus            [in]   bus
 *  @param  cs             [in]   cs
 *  @param  addr           [in]   アドレス
 *  @param  data           [in]   書き込み情報
 *  @param  configSpeed    [in]   SPEED情報
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/11/21 ALPHA)Yokoyama create
 *  @date   2015/06/05 ALPHA)Yokoyama PPC 1chipから流用して修正
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
	
	/* 書き出しのための各種設定を行う	*/
	tx_dat[0] = (addr >> 8) & 0x000000FF;
	tx_dat[1] = addr & 0x000000FF;
	tx_dat[2] = data & 0x000000FF;
	
	/* Length設定	*/
	len_dat = 3;
	
	/* 情報を書き出す	*/
	ret = bpf_hm_devc_spi_ps_spi (bus, cs, len_dat, tx_dat, rx_dat, BPF_HM_DEVC_SPI_CS0, configSpeed);
	
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RFIC WRITE
 *  @note   RFIC WRITEを行う。
 *  @param  txRx			[in]		TX/RX kind【未使用】
 *  @param  unit			[in]		unit
 *  @param  address			[in]		address
 *  @param  data			[in]		data
 *  @return int
 *  @date   2015/04/07 ALPHA)yokoyama create
 *  @date   2015/06/05 ALPHA)yokoyama TDD ZYNQに伴い全面的に修正
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_RFIC_WRITE(	UINT		txRx,
							UINT		unit,
							UINT		address,
							UINT		data )
{
	int							ret;
	pthread_mutex_t				*mutex;								/* mutexオブジェクト      */
	unsigned int				bus;
	unsigned int				cs;
/*	unsigned int				devkind;	*/

/* 起動時間に影響するためひとまず抑止																*/
	/* Trace log */
/*	bpf_com_dbglog_fmt(	BPF_COM_LOG_TRACE,															*/
/*						"BPF_HM_DEVC_RFIC_WRITE TxRx=%08x unit=%08x address=%08x data=%08x",		*/
/*						txRx, unit, address, data);													*/

	/* INパラチェック	*/
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
	
	mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_1 ].mutex;			/* mutexオブジェクト      */
	
	/* Mutex lock */
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}

	/* 初期値設定	*/
	bus = BPF_HM_DEVC_SPI_SPI1;

	/* Clemente情報を書き出す	*/
	ret = bpf_hm_devc_spi_write_data_rfic_detail( bus, cs, address, data, 0 );
	
	/* Mutex Unlock */
	pthread_mutex_unlock(mutex);
	
	/* SPIアクセス履歴を取得する	*/
/*	BPF_COM_LOG_SPI( devkind, E_SPI_LV_WRITE_LOW, address, data );	*/
	
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RFIC RESET
 *  @note   RFIC RESETを行う。
 *  @param  txRx			[in]		TX/RX kind【未使用】
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
