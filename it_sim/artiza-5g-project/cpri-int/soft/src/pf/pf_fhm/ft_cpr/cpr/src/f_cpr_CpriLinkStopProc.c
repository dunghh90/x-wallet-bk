/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file  f_cpr_CpriLinkStopProc.c
 *  @brief cpri link stop indication function
 *  @date  2015/10/06 TDI)satou create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/*********************************************************************************/

/**
 * @addtogroup RRH_PF_CPR
 * @{
 */
#include "f_sys_inc.h"
#include "f_cpr_inc.h"
#include "BPF_RU_ITCM.h"
#include "BPF_RU_HRTM.h"
#include "rrhApi_Cpri.h"

/**
* @brief 	cpri link stop indication function
* @param 	buff_adr [in] the buffer address pointer of received message
* @return 	None
* @date 	2015/10/06 TDI)satou create
* @date     2015/11/06 TDI)satou ハソ-QA-017
* @date     2015/11/10 TDI)satou その他未実装-017 「CPM#n LTE DL補正済みクリア」追加
* @date     2016/11/18 FJT)ohashi 16B スロット数制限機能追加
*/
VOID f_cpr_CpriLinkStopProc(UINT * buff_adr)
{
    T_API_CPRILINK_STOP_IND *apiInd;
    UINT regVal, offset;
    USHORT linkno;

    apiInd = (T_API_CPRILINK_STOP_IND*)buff_adr;
    if (D_RRH_CPRINO_REC == apiInd->link_num) {
        return;
    }

    /* Stop timer */
	f_cprLinkUpTimerCtrl(apiInd->link_num, D_RRH_OFF);

    /* CPM#n ERR ALL OFF */
    linkno = apiInd->link_num;
    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_L1ERR(linkno)             , D_SYS_OFF);
    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_L2ERR(linkno)             , D_SYS_OFF);
    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_L3ERR(linkno)          , D_SYS_OFF);
    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_L3ERR(linkno)         , D_SYS_OFF);
    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_SFPERR1(linkno)           , D_SYS_OFF);
    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_SFPERR2(linkno)           , D_SYS_OFF);
    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_SLOT_ERR(linkno)          , D_SYS_OFF);
    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_DELAY_ERR(linkno)      , D_SYS_OFF);
    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_DELAY_ERR(linkno)     , D_SYS_OFF);
    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_RESTARTUP_ERR(linkno)  , D_SYS_OFF);
    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_DL_NG_ERR(linkno)         , D_SYS_OFF);
    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_CARR_RESET_ERR(linkno) , D_SYS_OFF);
    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_CARR_RESET_ERR(linkno), D_SYS_OFF);
    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_RESTARTUP_ERR(linkno) , D_SYS_OFF);

    /* SFPログ取得 */
    f_cpr_sfp_log(linkno, E_RRH_SFPLOG_TRIGGER_CPRI_UNUSE);
    
    /* CPM#n 光OFF & CPRI Disable設定 */
    offset = M_RRH_REG_CPRI_SFPCNT(apiInd->link_num);
    BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, offset, &regVal);
    regVal &= ~0x7;
    regVal |= 0x1;
    BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, offset, &regVal);

    /* SFP power off */
    offset = D_RRH_REG_CNTS_SFPLDODIS;
    regVal = 0x00010000 << (apiInd->link_num - 1);
    BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE_CPRI, offset, regVal);

    /* CPM#n LTE DL補正済みクリア */
    f_cpr_DelayAdjust_CPMnDisable(apiInd->link_num);
}
/* @} */
