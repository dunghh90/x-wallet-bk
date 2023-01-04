/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_EEPROM_READ.c
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
 *  @brief  I2C(EEPROM)読み込み
 *  @note   BPF_HM_DEVC_EEPROM_READ
 *  @param  offset  [in]   EEPROM読み込みオフセット
 *  @param  data_p  [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_EEPROM_READ(unsigned short offset, unsigned char *data_p)
{
	int setpage;                                    /* デバイスアドレス       */
	int ret;                                        /* 戻り値                 */

	/* Trace log */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_EEPROM_READ offset = %04x" ,offset);

	setpage = ((offset >> 8) & 0x07) | BPF_HM_DEVC_I2C_BASE_ADD;

	ret = bpf_hm_i2c_read(	BPF_HM_DEVC_I2C_0,				/* 種別				*/
							setpage,						/* setpage			*/
							offset,							/* オフセット		*/
							1,								/* 読み込みサイズ	*/
							BPF_HM_DEVC_ACCESS_1BYTE,		/* アクセスタイプ	*/
							data_p );						/* 読み込みデータ	*/
	
	if (ret != BPF_HM_DEVC_COMPLETE)
	{
		/* ioctl失敗 */
		return BPF_HM_DEVC_NG;
	}
	
	BPF_COM_PLOG_I2C(E_I2C_LV_READ,offset,(unsigned int)(*data_p),0,0);	/* packet log */

	return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
