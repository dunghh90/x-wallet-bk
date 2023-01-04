/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_cm_SlotNoChk.c
 *  @brief  TRA slot number check
 *  @date   2008/07/25 FFCS)linlj create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/********************************************************************************************************************/

/** @addtogroup TRA_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"



/* @{ */
/********************************************************************************************************************/
/**
 *  @brief     TRA slot number check
 *  @note     TRA slot number check
 *  @param    slot_no [in] slot no
 *  @return   RETURN CODE
 *  @retval    CMD_OK							0: 	Normal end
 *  @retval    CMD_NG							1: 	NG
 *  @date     2008/07/25 FFCS)linlj create
 *  @date     2015/09/01 TDI)satou スロット番号1以外の場合はNGとするように
 */
/********************************************************************************************************************/
UINT m_rc_cm_SlotNoChk(UINT slot_no)
{
	/* 戻り値初期化 */
	UINT rtn = CMD_OK;
	
    /* スロット番号1以外の場合 */
    if(slot_no != CMD_OF_MAX_SLOTNO)
    {
        /* NGを設定 */
        rtn = CMD_NG;
    }

	return rtn;
}

/* @} */

/* @} */

