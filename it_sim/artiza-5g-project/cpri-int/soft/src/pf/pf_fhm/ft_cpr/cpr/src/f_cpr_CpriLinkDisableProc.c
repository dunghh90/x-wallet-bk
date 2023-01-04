/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_cpr_CpriLinkDisableProc.c
 *  @brief  cpri link disable indication function
 *  @date 2013/11/14 FFCS)niemsh create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_CPR
* @{ */

#include "f_sys_inc.h"
#include "f_cpr_inc.h"
#include "rrhApi_Cpri.h"
/**
* @brief 	cpri link disable indication function
* @note  	cpri link disable indication function.\n
* @param 	buff_adr [in] the buffer address pointer of received message
* @return 	None
* @date 	2013/11/14 FFCS)niemsh create
* @date     2015.08.11 TDIPS)ikeda 17リンク対応
* @date     2015.09.03 TDIPS)ikeda メッセージ対応
* @date     2015/09/05 TDI)satou 設定値見直し
* @warning	N/A
* @FeatureID	PF_Cpri-003-002-001
*/
VOID f_cpr_CpriLinkDisableProc(UINT * buff_adr)
{
	INT 		num;
	UINT    regAddr;
	UINT	regValue;
	T_API_CPRILINK_DISABLE_IND* a_msg;

	a_msg = (T_API_CPRILINK_DISABLE_IND*)buff_adr;
	regAddr = M_RRH_REG_CNTS_IRQCPSM(a_msg->link_num);

	/*mask cpri IRQ register*/
	/* read register */
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, regAddr, &regValue);

	if (D_RRH_CPRINO_REC == a_msg->link_num) {
	    regValue |= 0xFFFF;
	} else {
	    /*
	     * master側は1つのレジスタに2ポートの情報が存在する。
	     * 下位16bit : master#1, #3, #5 ...
	     * 上位16bit : master#2, #4, #6 ...
	     */
	    if (0 != (a_msg->link_num % 2)) {
	        regValue |= 0x0000FFFF;
	    } else {
	        regValue |= 0xFFFF0000;
	    }
	}

	/* set register */
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, regAddr, &regValue);

#ifndef OPT_RRH_SIM
	/* if f_cprw_cprStatChgNtc_QID has valid,send notice to apl */
	for(num = 0; num < sizeof(f_cprw_cprStatChgNtc_QID) / sizeof(UINT);num++)
	{
		if(f_cprw_cprStatChgNtc_QID[ num] != 0)
		{
			f_cpr_com_CpriStateNtc(a_msg->link_num, f_cprw_cprStatChgNtc_QID[ num]);
		}
	}
#endif

	return;
}
/* @} */

