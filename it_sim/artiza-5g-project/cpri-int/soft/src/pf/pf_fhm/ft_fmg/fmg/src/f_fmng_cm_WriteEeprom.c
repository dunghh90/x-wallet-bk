/*!
 * @skip   $Id$
 * @file   f_fmng_cm_WriteEeprom.c
 * @brief  the processing of EEPROM writing.
 * @date   2008/09/26 FFCS)zhengmh Create 
 * @date   2013/11/15 FFCS)linb Modify for DCM Zynq 
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup TRA_PF_FMNG
 * @{ 
 */

#include "f_fmng_inc.h"						/* FMNG task head file */

/*!
 *  @brief  the function of EEPROM writing.
 *  @note   This function is processed as follows
            1) transfer the common function of EEPROM reading,if writing is faild,abort
 *  @param  a_eepadr      [in]  EEPROM address
 *  @param  *a_val        [in] the value which is writed
 *  @return Void.
 *  @retval -
 *  @date   2008/09/26 FFCS)zhengmh Create.
 *  @date   2013/11/15 FFCS)linb Modify for DCM Zynq.
 *  @warning	N/A
 *  @FeatureID	PF_File-002-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

void f_fmng_cm_WriteEeprom( UINT a_eepadr, USHORT a_val )
{
	INT		iRtnCode;
	UCHAR	a_writeData;
	
	a_writeData = (UCHAR)(a_val >> 8);
	iRtnCode = BPF_HM_DEVC_EEPROM_WRITE(a_eepadr, &a_writeData);
	if( BPF_HM_DEVC_COMPLETE != iRtnCode )
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ERROR, "write error1 %d,0x%x,0x%x",iRtnCode,a_eepadr,a_writeData);
	}

	a_writeData = (UCHAR)a_val;
	iRtnCode = BPF_HM_DEVC_EEPROM_WRITE(a_eepadr+1, (UCHAR*)&a_writeData);
	if( BPF_HM_DEVC_COMPLETE != iRtnCode )
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ERROR, "write error2 %d,0x%x,0x%x",iRtnCode,a_eepadr,a_writeData);
	}
	return ;
}

/* @} */
