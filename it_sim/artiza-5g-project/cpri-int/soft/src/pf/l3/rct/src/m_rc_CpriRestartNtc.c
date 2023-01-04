/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_CpriRestartNtc.c
 *  @brief  CPRI再起動通知
 *  @note   l3/cprがRECとの間にEDWNを検出時、装置起動時と同じ起動をRCTに依頼する
 *  @date   2015/11/13 TDIPS)satou その他未実装-013 create
 *  @date	2015/11/14 TDIPS)sasaki Update
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015-
 */
/********************************************************************************************************************/
/**
 * @addtogroup L3_RCT
 * @{
 */
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"
#include "m_rc_def.h"
#include "m_rc_ftyp.h"
/********************************************************************************************************************/
/**
 *  @brief	CPRI再起動通知
 *  @return	None
 *  @date	2015/11/13 TDIPS)satou その他未実装-013 create
 *  @date	2015/11/14 TDIPS)sasaki Update IT2問処No.186対処
 */
/********************************************************************************************************************/
VOID m_rc_CpriRestartNtc(VOID)
{
	INT		errcd;

	(VOID)cm_TStop(CMD_TIMID_STARTUPRE, &errcd);

	rec_Startup_First_flg[CMD_SYS_3G ]       = D_RRH_OFF;
    rec_Startup_First_flg[CMD_SYS_S3G]       = D_RRH_OFF;
    re_StartUp_CPRI_Conn_flg                 = D_RRH_OFF;
}
/** @} */
