/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_cpr_CpriFiberOutput.c
 *  @brief  CPRI L3 Link Error setting request function
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
* @brief 	CPRI L3 Link fiber output function
* @note  	CPRI L3 Link fiber output request function.\n
* @param 	buff_adr [in] the buffer address pointer of received message
* @return 	None
* @date 	2013/11/14 FFCS)niemsh create
 *  @date   2015/7/29 TDIPS)ikeda rev.27585 17�����N�Ή�
* @warning	N/A
* @FeatureID	PF_Cpri-001-001-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_cpr_CpriFiberOutput(UINT * buff_adr)
{
	UINT onOff;
	UINT regVal;
	USHORT	linkno;
	T_API_CPRILINK_FIBEROUTPUT_IND *papiInd;

	papiInd = (T_API_CPRILINK_FIBEROUTPUT_IND *)buff_adr;
	linkno = papiInd->link_num;
	
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI,
		M_RRH_REG_CPRI_SFPCNT(linkno),&regVal);
	onOff = ((T_API_CPRILINK_FIBEROUTPUT_IND*)buff_adr)->onOff;
	if(onOff)
	{
		usleep(1*1000);
		regVal &= ~0x00000003;
		regVal |=  0x00000002;
	}
	else
	{
		regVal &= ~0x00000003;
		regVal |=  0x00000001;
	}
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI,
					M_RRH_REG_CPRI_SFPCNT(linkno),&regVal);
	
	return;
}
/* @} */

