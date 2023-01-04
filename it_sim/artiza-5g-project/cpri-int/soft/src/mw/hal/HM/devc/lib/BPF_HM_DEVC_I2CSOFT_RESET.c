/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_I2CSOFT_RESET.c
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
 *  @brief  I2C(SoftMacro)リセット
 *  @note   BPF_HM_DEVC_I2CSOFT_RESET
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_I2CSOFT_RESET()
{
    int ret     = 0;                                    /* 戻り値                 */

    /* I2C(SoftMacro)リセット */
    ret = bpf_hm_i2c_soft_reset();

    return ret;
}

/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
