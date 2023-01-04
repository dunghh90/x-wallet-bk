/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eio_ram.c
 *  @brief  supervision and control external IO RAM table
 *  @date   2019/03/18 FJT)Koshida create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_EIO
* @{ */

/****************************************************************************/
/*		define(s)/include(s)												*/
/****************************************************************************/
#include "f_eio_header.h"			/* 外部IOタスクヘッダファイル			*/

/** @name EIO thread state */
/* @{ */
UINT						f_eiow_thrdstate;
/* @} */

/** @name external IO manage table */
/* @{ */
T_EIOW_EXTIO				f_eiow_extIo;
/* @} */

/** @name EIO SV Cntorl manage table pointer */
/* @{ */
T_RRH_DU_EIO_SVCTRL_MNG		*f_eiow_svctrl_mngp;
/* @} */

/** @name EIO SV Cntorl log table pointer */
/* @{ */
T_RRH_DU_EIO_SVCTRL_LOG		*f_eiow_svctrl_logp;
/* @} */


/** @name EIO FAN manage table */
/* @{ */
T_EIOW_FANMNG				f_eiow_fanMngTbl;
/* @} */

/** @name EIO ANT manage table */
/* @{ */
T_EIO_ANT_INFO				f_eiow_antMngTbl[D_RRH_ANT_MAX];
/* @} */

T_RRH_DU_PROCESS_INFO		*f_eiow_DuProcInfo;
/* @} */
