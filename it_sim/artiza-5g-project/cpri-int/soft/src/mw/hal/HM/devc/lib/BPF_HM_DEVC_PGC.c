/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_PGC.c
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
 *  @brief  PGC読み込み
 *  @note   BPF_HM_DEVC_PGC_READ
 *  @param  grp     [in]   デバイスグループ
 *  @param  sa_add  [in]   SA(Device)
 *  @param  srn     [in]   SRN
 *  @param  size    [in]   サイズ
 *  @param  data_p  [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_PGC_READ(unsigned char grp, unsigned char sa_add, unsigned char srn, unsigned int *data_p)
{
	bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"BPF_HM_DEVC_PGC_READ not supported ");
	return BPF_HM_DEVC_NG;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PGC書き込み
 *  @note   BPF_HM_DEVC_PGC_WRITE
 *  @param  grp     [in]   デバイスグループ
 *  @param  sa_add  [in]   SA(Device)
 *  @param  srn     [in]   SRN
 *  @param  data    [out]  書き込みデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_PGC_WRITE(unsigned char grp, unsigned char sa_add, unsigned char srn, unsigned int data)
{
	bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"BPF_HM_DEVC_PGC_WRITE not supported ");
	return BPF_HM_DEVC_NG;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PGC書き込み
 *  @note   BPF_HM_DEVC_PGC_WRITE
 *  @param  grp     [in]   デバイスグループ
 *  @param  sa_add  [in]   SA(Device)
 *  @param  srn     [in]   SRN
 *  @param  data    [out]  書き込みデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2019/02/19 FJT)Taniguchi create
 
 */
/********************************************************************************************************************/
int bpf_hm_devc_pgc_busy_check()
{
	unsigned int loop = 0;
	unsigned int	data;
	int ret;

	while(1){
		data = 0;
		if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_IF_OFFSET, &data)) != BPF_HM_DEVC_COMPLETE){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
			return BPF_HM_DEVC_NG;
		}

		if((data & BPF_HM_DEVC_SPI_PGC_BUSY) == 0){
			ret = BPF_HM_DEVC_COMPLETE;
			break;
		}
		bpf_hm_devc_delay(0,1000000);
		loop++;
		if(loop >= 1000){
			ret = BPF_HM_DEVC_NG;
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET busy(0x%08x) ",data);
			break;
		}
	}
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PGC読み込み
 *  @note   BPF_HM_DEVC_PGC_GAIN_PHASE_READ
 *  @param  grp     [in]   デバイスグループ
 *  @param  sa_add  [in]   SA(Device)
 *  @param  srn     [in]   SRN
 *  @param  size    [in]   サイズ
 *  @param  data_p  [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2019/02/19 FJT)Taniguchi create
 
 */
/********************************************************************************************************************/
static int BPF_HM_DEVC_PGC_WRITE_local(
	 unsigned char chain_grp,
	 unsigned int ctl1,
	 unsigned int ctl2,
	 unsigned int ctl3,
	 unsigned int ctl4,
	 unsigned int gain1 ,
	 unsigned int phase1,
	 unsigned int gain2 ,
	 unsigned int phase2,
	 unsigned int gain3 ,
	 unsigned int phase3,
	 unsigned int gain4 ,
	 unsigned int phase4
)
{
	
	int ret = BPF_HM_DEVC_UNSUPPORT;
	if(di_devc_data_p->device_kind != D_RRH_EEP_DEV_KIND_5GDU_MMW){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Device Kind NG:%d", di_devc_data_p->device_kind);
		return ret;
	}

	unsigned int	data;
	unsigned int	loop = 0;

	if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_IF_OFFSET, &data)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	
	data |= BPF_HM_DEVC_SPI_PGC_MODE_ENABLE;
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_IF_OFFSET, data)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}

	while(1){
		data = 0;
		if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_IF_OFFSET, &data)) != BPF_HM_DEVC_COMPLETE){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
			break;
		}
		if((data & BPF_HM_DEVC_SPI_PGC_MODE_ENABLE) != 0){
			if((ret = bpf_hm_devc_pgc_busy_check()) != BPF_HM_DEVC_COMPLETE){
				bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET busy");
			}
			break;
		}
		bpf_hm_devc_delay(0,1000000);
		loop++;
		if(loop > 1000){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET is not ENABLE(0x%08x)",data);
			ret = BPF_HM_DEVC_NG;
			break;
		}
	}
	if(ret != BPF_HM_DEVC_COMPLETE){
		return ret;
	}
	
	if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_IF_OFFSET, &data)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	
	data &= 0xFFFFFF00;
	data |= ((unsigned int)1) << (chain_grp - 1);

	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_IF_OFFSET, data)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}

	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH1_GAIN_OFFSET, gain1)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH1_GAIN_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH1_PAHSE_OFFSET, phase1)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH1_PAHSE_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH1_CTL_OFFSET, ctl1)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH1_CTL_OFFSET error");
		return BPF_HM_DEVC_NG;
	}

	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH2_GAIN_OFFSET, gain2)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH2_GAIN_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH2_PAHSE_OFFSET, phase2)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH2_PAHSE_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH2_CTL_OFFSET, ctl2)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH2_CTL_OFFSET error");
		return BPF_HM_DEVC_NG;
	}

	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH3_GAIN_OFFSET, gain3)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH3_GAIN_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH3_PAHSE_OFFSET, phase3)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH3_PAHSE_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH3_CTL_OFFSET, ctl3)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH3_CTL_OFFSET error");
		return BPF_HM_DEVC_NG;
	}


	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH4_GAIN_OFFSET, gain4)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH4_GAIN_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH4_PAHSE_OFFSET, phase4)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH4_PAHSE_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH4_CTL_OFFSET, ctl4)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH4_CTL_OFFSET error");
		return BPF_HM_DEVC_NG;
	}

	data |= BPF_HM_DEVC_SPI_PGC_ACCESS;
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_IF_OFFSET, data)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}

	if((ret = bpf_hm_devc_pgc_busy_check()) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET busy");
		return BPF_HM_DEVC_NG;
	}

	if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_IF_OFFSET, &data)) == BPF_HM_DEVC_COMPLETE){
		data &= ~BPF_HM_DEVC_SPI_PGC_MODE_ENABLE;
		if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_IF_OFFSET, data)) != BPF_HM_DEVC_COMPLETE){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error(0x%x)",data);
		}
	}else{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error(0x%x)",data);
	}

	return ret;
}


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PGC読み込み
 *  @note   BPF_HM_DEVC_PGC_GAIN_PHASE_READ
 *  @param  grp     [in]   デバイスグループ
 *  @param  sa_add  [in]   SA(Device)
 *  @param  srn     [in]   SRN
 *  @param  size    [in]   サイズ
 *  @param  data_p  [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2019/02/19 FJT)Taniguchi create
 
 */
/********************************************************************************************************************/
static int BPF_HM_DEVC_PGC_READ_local(
	 unsigned char chain_grp,
	 unsigned int  ctl,
	 unsigned int* gain1 ,
	 unsigned int* phase1,
	 unsigned int* reg1,
	 unsigned int* gain2 ,
	 unsigned int* phase2,
	 unsigned int* reg2,
	 unsigned int* gain3 ,
	 unsigned int* phase3,
	 unsigned int* reg3,
	 unsigned int* gain4 ,
	 unsigned int* phase4,
	 unsigned int* reg4
)
{
	
	int ret = BPF_HM_DEVC_UNSUPPORT;
	if(di_devc_data_p->device_kind != D_RRH_EEP_DEV_KIND_5GDU_MMW){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Device Kind NG:%d", di_devc_data_p->device_kind);
		return ret;
	}

	unsigned int	data;
	unsigned int loop = 0;

	if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_IF_OFFSET, &data)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	
	data |= BPF_HM_DEVC_SPI_PGC_MODE_ENABLE;
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_IF_OFFSET, data)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}

	while(1){
		data = 0;
		if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_IF_OFFSET, &data)) != BPF_HM_DEVC_COMPLETE){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
			break;
		}
		if((data & BPF_HM_DEVC_SPI_PGC_MODE_ENABLE) != 0){
			if((ret = bpf_hm_devc_pgc_busy_check()) != BPF_HM_DEVC_COMPLETE){
				bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET busy");
			}
			break;
		}
		bpf_hm_devc_delay(0,1000000);
		loop++;
		if(loop > 1000){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET is not ENABLE(0x%08x)",data);
			ret = BPF_HM_DEVC_NG;
			break;
		}
	}
	if(ret != BPF_HM_DEVC_COMPLETE){
		return ret;
	}
	
	if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_IF_OFFSET, &data)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	
	data &= 0xFFFFFF00;
	data |= ((unsigned int)1) << (chain_grp - 1);
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_IF_OFFSET, data)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}

	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH1_GAIN_OFFSET, 0)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH1_GAIN_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH1_PAHSE_OFFSET, 0)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH1_PAHSE_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH1_CTL_OFFSET, ctl)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH1_CTL_OFFSET error");
		return BPF_HM_DEVC_NG;
	}

	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH2_GAIN_OFFSET, 0)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH2_GAIN_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH2_PAHSE_OFFSET, 0)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH2_PAHSE_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH2_CTL_OFFSET, ctl)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH2_CTL_OFFSET error");
		return BPF_HM_DEVC_NG;
	}

	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH3_GAIN_OFFSET, 0)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH3_GAIN_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH3_PAHSE_OFFSET, 0)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH3_PAHSE_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH3_CTL_OFFSET, ctl)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH3_CTL_OFFSET error");
		return BPF_HM_DEVC_NG;
	}


	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH4_GAIN_OFFSET, 0)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH4_GAIN_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH4_PAHSE_OFFSET, 0)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH4_PAHSE_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_CH4_CTL_OFFSET, ctl)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_CH4_CTL_OFFSET error");
		return BPF_HM_DEVC_NG;
	}

	data |= BPF_HM_DEVC_SPI_PGC_ACCESS;
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_IF_OFFSET, data)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}

	if((ret = bpf_hm_devc_pgc_busy_check()) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET busy");
		return BPF_HM_DEVC_NG;
	}

	data |= BPF_HM_DEVC_SPI_PGC_ACCESS;
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_IF_OFFSET, data)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}

	if((ret = bpf_hm_devc_pgc_busy_check()) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET busy");
		return BPF_HM_DEVC_NG;
	}
	
	if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_CH1_RX_GAIN_OFFSET, gain1)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_CH1_RX_PAHSE_OFFSET, phase1)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_CH1_RX_CTL_OFFSET, reg1)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	
	if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_CH2_RX_GAIN_OFFSET, gain2)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_CH2_RX_PAHSE_OFFSET,phase2)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_CH2_RX_CTL_OFFSET, reg2)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	
	if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_CH3_RX_GAIN_OFFSET, gain3)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_CH3_RX_PAHSE_OFFSET, phase3)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_CH3_RX_CTL_OFFSET, reg3)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	
	if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_CH4_RX_GAIN_OFFSET, gain4)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_CH4_RX_PAHSE_OFFSET, phase4)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PGC_CH4_RX_CTL_OFFSET, reg4)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
		return BPF_HM_DEVC_NG;
	}
	
	data &= ~BPF_HM_DEVC_SPI_PGC_MODE_ENABLE;
	if((ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PGC_IF_OFFSET, data)) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "BPF_HM_DEVC_SPI_PGC_IF_OFFSET error");
	}
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PGC GAIN PAHSE書き込み
 *  @note   BPF_HM_DEVC_PGC_GAIN_PHASE_WRITE
 *  @param  grp     [in]   デバイスグループ
 *  @param  sa_add  [in]   SA(Device)
 *  @param  srn     [in]   SRN
 *  @param  data    [out]  書き込みデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2019/02/19 FJT)Taniguchi create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_PGC_GAIN_PHASE_WRITE(
	 unsigned char chain_grp,
	 unsigned char txrx,
	 unsigned int gain1 ,
	 unsigned int phase1,
	 unsigned int gain2 ,
	 unsigned int phase2,
	 unsigned int gain3 ,
	 unsigned int phase3,
	 unsigned int gain4 ,
	 unsigned int phase4
){
	unsigned int ctl = 0;
	int			ret;
	
	ctl |= ((unsigned int)(txrx & 3)) << 12;

	pthread_mutex_t	*mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_4 ].mutex;
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}

	ret = BPF_HM_DEVC_PGC_WRITE_local(chain_grp,ctl,ctl,ctl,ctl,gain1,phase1,gain2,phase2,gain3,phase3,gain4,phase4);

	pthread_mutex_unlock(mutex);

	return(ret);
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PGC 書き込み2
 *  @note   BPF_HM_DEVC_PGC_WRITE2
 *  @param  grp     [in]   デバイスグループ
 *  @param  sa_add  [in]   SA(Device)
 *  @param  srn     [in]   SRN
 *  @param  data    [out]  書き込みデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2019/02/19 FJT)Taniguchi create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_PGC_WRITE2(
	 unsigned char chain_grp,
	 unsigned int data1,
	 unsigned int data2,
	 unsigned int data3,
	 unsigned int data4
){
	unsigned int ctl1 = 0;
	unsigned int ctl2 = 0;
	unsigned int ctl3 = 0;
	unsigned int ctl4 = 0;

	int			ret;
	

	ctl1 |= (data1 & 0x4000);
	ctl1 |= (data1 & 0x0F00);
	ctl1 |= (data1 & 0x00FF);

	ctl2 |= (data2 & 0x4000);
	ctl2 |= (data2 & 0x0F00);
	ctl2 |= (data2 & 0x00FF);

	ctl3 |= (data3 & 0x4000);
	ctl3 |= (data3 & 0x0F00);
	ctl3 |= (data3 & 0x00FF);

	ctl4 |= (data4 & 0x4000);
	ctl4 |= (data4 & 0x0F00);
	ctl4 |= (data4 & 0x00FF);

	pthread_mutex_t	*mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_4 ].mutex;
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}

	ret = BPF_HM_DEVC_PGC_WRITE_local(chain_grp,ctl1,ctl2,ctl3,ctl4,0,0,0,0,0,0,0,0);

	pthread_mutex_unlock(mutex);

	return(ret);
}
/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PGC GAIN PAHSE読み込み
 *  @note   BPF_HM_DEVC_PGC_GAIN_PHASE_READ
 *  @param  grp     [in]   デバイスグループ
 *  @param  sa_add  [in]   SA(Device)
 *  @param  srn     [in]   SRN
 *  @param  size    [in]   サイズ
 *  @param  data_p  [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2019/02/19 FJT)Taniguchi create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_PGC_GAIN_PHASE_READ(
	 unsigned char chain_grp,
	 unsigned char txrx,
	 unsigned int* gain1 ,
	 unsigned int* phase1,
	 unsigned int* reg1,
	 unsigned int* gain2 ,
	 unsigned int* phase2,
	 unsigned int* reg2,
	 unsigned int* gain3 ,
	 unsigned int* phase3,
	 unsigned int* reg3,
	 unsigned int* gain4 ,
	 unsigned int* phase4,
	 unsigned int* reg4
){
	unsigned int ctl = 0;
	int			ret;
	
	if(txrx == 1){
		/* TXread */
		ctl |= ((unsigned int)4) << 8;
	}else{
		/* RXread */
		ctl |= ((unsigned int)3) << 8;
	}

	pthread_mutex_t	*mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_4 ].mutex;
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}
	ret = BPF_HM_DEVC_PGC_READ_local(chain_grp,ctl,gain1,phase1,reg1,gain2,phase2,reg2,gain3,phase3,reg3,gain4,phase4,reg4);

	pthread_mutex_unlock(mutex);

	return(ret);
}
/* @} */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PGC読み込み
 *  @note   BPF_HM_DEVC_PGC_GAIN_PHASE_READ
 *  @param  grp     [in]   デバイスグループ
 *  @param  sa_add  [in]   SA(Device)
 *  @param  srn     [in]   SRN
 *  @param  size    [in]   サイズ
 *  @param  data_p  [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2019/02/19 FJT)Taniguchi create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_PGC_READ2(
	 unsigned char chain_grp,
	 unsigned char bank,
	 unsigned int* gain1 ,
	 unsigned int* phase1,
	 unsigned int* reg1,
	 unsigned int* gain2 ,
	 unsigned int* phase2,
	 unsigned int* reg2,
	 unsigned int* gain3 ,
	 unsigned int* phase3,
	 unsigned int* reg3,
	 unsigned int* gain4 ,
	 unsigned int* phase4,
	 unsigned int* reg4
){
	unsigned int ctl = 0;
	int			ret;

	ctl |= ((unsigned int)bank) << 8;

	pthread_mutex_t	*mutex = &di_devc_data_p->info[ BPF_HM_DEVC_SPI_4 ].mutex;
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}

	ret = BPF_HM_DEVC_PGC_READ_local(chain_grp,ctl,gain1,phase1,reg1,gain2,phase2,reg2,gain3,phase3,reg3,gain4,phase4,reg4);

	pthread_mutex_unlock(mutex);
	return(ret);
}
/* @} */
/* @} */
