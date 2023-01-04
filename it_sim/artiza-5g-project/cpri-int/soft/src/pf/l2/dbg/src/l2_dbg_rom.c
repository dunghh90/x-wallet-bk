/*!
 * @skip  $ld:$
 * @file  l2_dbg_rom.c
 * @brief ROM table
 * @date  2013/12/03 FFCS)hongj Create for zynq\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
 
/*!
 * @addtogroup RRH_L2_DBG
 * @{
 */
#include "m_cm_header.h"
#include "l2_com_inc.h"
#include "l2_dbg_inc.h"


/**	Main matrix table */
VOID (* const l2_dbgr_mtrxtbl[L2_DBG_MX_EVENT])(UINT *inf_p) = 
{
	l2_dbg_cpriMsgSub,		/* cpri message subscribe request 		*/
	l2_dbg_cpriMsgUnsub,	/* cpri message unsubscribe request 	*/
	l2_dbg_l2StateSub,		/* l2 link state subscribe request 		*/
	l2_dbg_l2StateUnsub,	/* l2 link state unsubscribe request 	*/
	l2_dbg_monStart,		/* cpri monitor startup indication		*/
	l2_dbg_monStop,			/* cpri monitor stop indication			*/
	l2_dbg_simuStart,		/* cpri simulator start request			*/
	l2_dbg_simuStop,		/* cpri simulator stop request			*/
	l2_dbg_statisticsInd	/* cpri message statistics indication	*/
};                              

/* @} */
