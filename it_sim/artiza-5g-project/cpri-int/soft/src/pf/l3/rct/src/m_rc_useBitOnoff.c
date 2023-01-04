/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_useBitOnoff.c
 *  @brief  Turn on/off USE bit of FhmBase
 *  @date   2015/08/06 FPT)Dung create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"

/********************************************************************************************************************/
/**
 *  @brief    Turn on/off USE bit of FhmBase
 *  @note     Turn on/off USE bit of FhmBase
 *  @param    flag		[in] Set / Clear bit USE
 *  @return   None
 *  @retval   -
 *  @date     2015/08/06 FPT)Dung create
 */
/********************************************************************************************************************/
VOID m_rc_useBitOnoff(USHORT flag)
{
	T_RRH_FHM_STATE		lwFhmState;

	/* 共用メモリFHM状態の呼び出し 取得用 */
	f_cmn_com_fhm_state_get(&lwFhmState);

	/** Flag is off when all carrier released */
	if(CMD_OFF == flag)
	{
		/* FHM基本監視項目 -> Clear bit USE */
		lwFhmState.FhmBase &= RCD_FB_USE_MASK;
	}else
	/** Flag is on for trx set OK */
	{
		/* FHM基本監視項目 -> Set bit USE */
		lwFhmState.FhmBase |= CMD_CARDSV_USE_BIT;
	}

	/* 共用メモリFHM状態の呼び出し 設定用 */
	f_cmn_com_fhm_state_set(&lwFhmState);

	return;
}

/** @} */
