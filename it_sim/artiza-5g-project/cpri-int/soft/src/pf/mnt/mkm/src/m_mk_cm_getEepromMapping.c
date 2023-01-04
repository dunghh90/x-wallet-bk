/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_mk_cm_getEepromMapping.c
 *  @brief  Eeprom register mapping
 *  @date   2013/12/04 FFCS)zhaodx create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_MKM
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

#include "m_cm_header.h"			/*  RRH common head file							*/
#include "mnt_com_inc.h"			/*	Maker MT management task header 	*/
#include "m_mk_header.h"			/*	Maker MT management task header 	*/



/********************************************************************************************************************/
/**
 *  @brief    Eeprom register mapping
 *  @note     Eeprom register mapping
 *  @param    slot	[in]	slot
 *  @return   UINT
 *  @retval   register address
 *  @date     2013/12/04 FFCS)zhaodx create
 */
/********************************************************************************************************************/

UINT  m_mk_cm_getEepromMapping(USHORT slot)
{
	UINT ret;

	/* calculate register address */
	ret = (UINT)mnt_com_swap_USHORT(slot);

	return ret;
}
/* @} */