/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_EEPROM_WRITE.c
 *  @brief  
 *  @date   2013/04/30 ALPHA)Matsuhashi create
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
 *  @brief  I2C(EEPROM)書き込み
 *  @note   BPF_HM_DEVC_EEPROM_WRITE
 *  @param  offset  [in]   EEPROM書き込みオフセット
 *  @param  data_p  [in]   書き込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_EEPROM_WRITE(unsigned short offset, unsigned char *data_p)
{
    int setpage = 0;                                    /* デバイスアドレス       */
    int ret     = 0;                                    /* 戻り値                 */

    /* 書き込みデータ格納領域チェック */
    if (data_p == NULL){
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"buffer == NULL");
        /* 書き込みデータ無し */
        return BPF_HM_DEVC_NG;
    }

    setpage = ((offset >> 8) & 0x07) | BPF_HM_DEVC_I2C_BASE_ADD;

	ret = bpf_hm_i2c_write(	BPF_HM_DEVC_I2C_0,
							setpage,
							offset,
							data_p,
							1 );

	if(ret == BPF_HM_DEVC_COMPLETE){
		/* packet log send */
		BPF_COM_PLOG_I2C(E_I2C_LV_WRITE,offset,(unsigned int)*data_p,0,0);
	}
    return ret;
}

/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
