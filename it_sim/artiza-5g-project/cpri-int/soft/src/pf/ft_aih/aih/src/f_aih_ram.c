/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_aih_ram.c
 *  @brief  ALD(AISG) interace RAM table
 *  @date   2019/08/01 FJT)Takahashi create
 * 
 *  ALL Rights Reserved, Copyright FUJITSU Limited 2019-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_AIH
* @{ */

/****************************************************************************/
/*		define(s)/include(s)												*/
/****************************************************************************/
#include "f_aih_inc.h"			/* 5GDU-LLS AIHタスクヘッダファイル */

/** @name AIH thread state */
/* @{ */
UINT						f_aih_thrdstate;
/* @} */

/** @name ALD log table pointer */
/* @{ */
T_RRH_AIH_ALD_LOG_TBL		*f_aih_aldLogp;     /* ALD time stamp log tbl */
T_RRH_AIH_ALD_CTRL_TBL		*f_aih_aldCtrlInfoLogp; /* ALD latest info log tbl */
T_RRH_AIH_ALD_CTRL_INFO		*f_aih_aldCtrlLogp; /* ALD latest info log tbl */
/* @} */


/* @} */
