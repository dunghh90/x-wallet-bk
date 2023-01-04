/* RRH-001 MD alpha)yokoyama add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_MMI_CPLD.c
 *  @brief  
 *  @date   2015/04/08 ALPHA)yokoyama create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2015
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
 *  @brief  MMI-CPLD読み込み
 *  @note   BPF_HM_DEVC_MMI_CPLD_READ
 *  @param  offset  [in]   読み込みアドレス
 *  @param  temp_p  [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2015/04/08 ALPHA)yokoyama create
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_MMI_CPLD_READ(unsigned int offset, unsigned int *data_p)
{
	/* Trace log */
	bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"not supported  offset=%08x", offset);
		
	return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  MMI-CPLD書き込み
 *  @note   BPF_HM_DEVC_MMI_CPLD_WRITE
 *  @param  offset  [in]   書き込みアドレス
 *  @param  data_p  [in]   書き込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2015/04/08 ALPHA)yokoyama create
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_MMI_CPLD_WRITE(unsigned int offset, unsigned int *data_p)
{
	bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"not supported  offset=%08x", offset);
	return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @} */
/* RRH-001 MD alpha)yokoyama add end */
