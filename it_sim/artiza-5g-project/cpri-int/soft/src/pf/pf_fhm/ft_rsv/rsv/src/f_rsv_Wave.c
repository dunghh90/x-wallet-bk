/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file   f_rsv_Wave.c
 *  @brief  停波設定/解除
 *  @date   2015/10/30 TDI)satou create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/*********************************************************************************/
/**
 * @addtogroup RRH_PF_RSV
 * @{
 */
/*********************************************************************************/
/* include file                                                                  */
/*********************************************************************************/
#include "f_rsv_header.h"
/*********************************************************************************/
/**
* @brief    停波設定/解除
* @param    ind [in] received message
* @return   None
* @date     2015/10/30 TDI)satou その他未実装-002 create
*/
/*********************************************************************************/
VOID f_rsv_SetWave(T_API_SVP_SET_WAVE_IND *ind)
{
    UINT reg_dat;
    UINT reg_adr;

    /**************************************************
     * 下りLTE用停波設定/解除レジスタ
     *************************************************/
    reg_adr = D_RRH_REG_DIS_LDISEN;
    reg_dat = ind->dis_ldisen;
    (VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, reg_adr, &reg_dat);

    /**************************************************
     * 上りLTE用停波設定/解除レジスタ
     *************************************************/
    reg_adr = D_RRH_REG_MIX_LMIXEN;
    reg_dat = ind->mix_lmixen;
    (VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, reg_adr, &reg_dat);

    /**************************************************
     * 下りWcdma用停波設定/解除レジスタ
     *************************************************/
    reg_adr = D_RRH_REG_DIS_WDISEN;
    reg_dat = ind->dis_wdisen;
    (VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, reg_adr, &reg_dat);

    /**************************************************
     * 上りWcdma用停波設定/解除レジスタ
     *************************************************/
    reg_adr = D_RRH_REG_MIX_WMIXEN;
    reg_dat = ind->mix_wmixen;
    (VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, reg_adr, &reg_dat);
}
/* @} */

