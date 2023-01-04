/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file   f_rsv_AxC.c
 *  @brief  AxC設定
 *  @date   2015/11/03 TDI)satou create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015-
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
* @brief    AxC設定
* @param    ind 受信メッセージ
* @return   None
* @date     2015/10/30 TDI)satou その他未実装-003 create
*/
/*********************************************************************************/
VOID f_rsv_SetAxC(T_API_SVP_SET_AxC_IND *ind)
{
    UINT reg_dat = 0;
    UINT reg_adr = D_RRH_REG_DIS_SYS;

    reg_dat |= (ind->bruse & (BIT0|BIT1          )) << 28;
    reg_dat |= (ind->axcbw & (BIT0|BIT1|BIT2     )) << 20;
    reg_dat |= (ind->ltebw & (BIT0|BIT1|BIT2     )) << 16;
    reg_dat |= (ind->lcnum & (BIT0|BIT1|BIT2     )) << 12;
    reg_dat |= (ind->wcnum & (BIT0|BIT1|BIT2     )) <<  4;
    reg_dat |= (ind->wcuse & (BIT0|BIT1|BIT2|BIT3));

    (VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, reg_adr, &reg_dat);
}
/* @} */

