/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file    M_cm_chksig.c
 *  @brief   Check CPRI signal validate	 
 *  @date   2008/11/11 FFCS)niemsh create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_DCM
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

#include "m_cm_header.h" /* TRA common header file*/


/********************************************************************************************************************/
/**
 *  @brief    Check CPRI signal validate	
 *  @note    Check CPRI signal validate
 *  @param  sig  		[in]   CPRI signal 
 *  @param  sys_p    		[out]   system according to signal
 *  @retval CMD_RES_OK							0: 		Normal end
 *  @retval CMD_RES_NG					F0000001: 		NG end
 *  @return RETURN CODE
 *  @warning   N/A
 *  @FeatureID N/A
 *  @Bug_No    N/A
 *  @CR_No     N/A
 *  @TBD_No    N/A
 *  @date     2008/10/11 FFCS)niemsh create
 *  @date     2013/11/27 alpha)Kondou zynq»¯Œê
 *  @date     2021/01/12 M&C)Huan.dh Intermediate device remove 3G handle
 */
/********************************************************************************************************************/
UINT m_cm_chksig(USHORT sig,USHORT *sys_p)
{
	/* make sure is 3G signal */
	if( (sig & CMD_SYS_CHK_CPRID) == CMD_NUM0)
	{
		*sys_p = CMD_SYS_3G;
		// /* get current system connection*/
		// if (cmw_validsysinfo == CMD_SYS_S3G)
		// {
		// 	/* S3G case*/
		// 	return CMD_RES_NG;			
		// }
		/*  Intermediate device responds NG with 3G CPRI*/
		return CMD_RES_NG;
	}
	/* make sure is S3G signal */
	else
	{
		*sys_p = CMD_SYS_S3G;

		/* get current system connection*/
		if (cmw_validsysinfo == CMD_SYS_3G)
		{
			/* 3G case*/
			return CMD_RES_NG;			
		}

	}
	
	return CMD_RES_OK;
}
/* @} */

/* @} */

