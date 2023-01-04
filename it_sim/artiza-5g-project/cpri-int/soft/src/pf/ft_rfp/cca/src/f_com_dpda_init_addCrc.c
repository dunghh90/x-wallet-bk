/*!
 * @skip  $ld:$
 * @file  f_com_dpda_init_addCrc.c (base:f_dpda_init_addCrc.c)
 * @brief CRC16計算処理
 * @date  2011/06/29 ALP)Yokoyama Create
 * ALL Rights Reserved, Copyright (C) FUJITSU Limited 2011-2013
 */

/*!
 * @addtogroup RRH_PF_COM
 * @{
 */
//#include "f_com_inc.h"
#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:CRC16計算処理
 * @note  関数処理内容.
 *			-# CRC16計算処理を行う
 * @param  dat          [in]USHORT		CRC16計算用データ
 * @param  p_crc        [out]USHORT*	CRC16計算過程データ
 * @return VOID
 * @date 2011/06/29 ALPHA)横山 Create
 * @date 2013/04/05 ALPHA)中村 B25対応
 */
VOID f_com_dpda_init_addCrc(USHORT dat, USHORT* p_crc)
{
	USHORT cnt;
	*p_crc ^= dat;
	cnt = 16;
	while(cnt--)
	{
		if ((*p_crc & 0x8000U)!=0)
		{
			*p_crc = (*p_crc << 1) ^ D_COM_INIT_CRCPOLY1;
		}
		else
		{
			*p_crc <<= 1;
		}
	}
	
	return;
}

/* @} */
