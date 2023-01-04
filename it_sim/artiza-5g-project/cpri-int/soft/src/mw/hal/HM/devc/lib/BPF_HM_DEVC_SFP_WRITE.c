/* RRH-001 MD alpha)yokoyama add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_SFP_WRITE.c
 *  @brief  
 *  @date   2013/11/27 ALPHA)yokoyama create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013-2015
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
 *  @brief  I2C(SFP)書き込み
 *  @note   BPF_HM_DEVC_SFP_WRITE
 *  @param  kind    [in]   種別
 *  @param  offset  [in]   SFP書き込みオフセット
 *  @param  data_p  [in]   書き込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/11/27 ALPHA)yokoyama create
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応 
 *          2018/08/15 KCN)Suehiro       SFP SEL対応追加 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_SFP_WRITE( unsigned int kind, unsigned short offset, unsigned char *data_p)
{
	int setpage;                                    /* デバイスアドレス       */
	int ret;                                        /* 戻り値                 */

	/* Trace log */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_SFP_WRITE offset = %04x" ,offset);

	/* DeviceによってOffsetが必要	*/
	switch( kind )	{
		case BPF_HM_DEVC_SFP_A0:
			offset |= BPF_HM_DEVC_SFP_BASE_ADD_A0;	/* primary a0 */
			break;
		case BPF_HM_DEVC_SFP_A2:
			offset |= BPF_HM_DEVC_SFP_BASE_ADD_A2;	/* primary a2 */
			break;
		default:
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "kind NG = %d", kind );
			return BPF_HM_DEVC_NG;
			break;
	}

	setpage = offset >> 8;

	ret = bpf_hm_i2c_write(	BPF_HM_DEVC_I2C_1,				/* 種別				*/
							setpage,						/* setpage			*/
							offset,							/* オフセット		*/
							data_p,							/* 書き込みデータ	*/
							1 );							/* 書き込みサイズ	*/
	
	if (ret != BPF_HM_DEVC_COMPLETE)
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "bpf_hm_i2c_write NG = %d", ret );
		/* ioctl失敗 */
		return BPF_HM_DEVC_NG;
	}

	BPF_COM_PLOG_I2C(E_I2C_LV_WRITE,offset,(unsigned int)*data_p,1,kind);

	return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @} */
/* RRH-001 MD alpha)yokoyama add end */
