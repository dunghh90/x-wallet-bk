/*!
 * @skip  $ld:$
 * @file  mnt_main_ram.c
 * @brief RAM table(proc)
 * @date  2013/11/20 FFCS)zhaodx Create for zynq\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
 
/*!
 * @addtogroup RRH_MNT_MAIN
 * @{
 */
#include "m_cm_header.h" 
#include "mnt_com_inc.h"


UINT mnt_mainw_stateno ;					/* State number		*/

UINT mnt_mainw_eventno ;					/* Event number		*/

#ifdef OPT_RRH_ZYNQ_REC
UINT internal_retrun_on=0;
#endif

/* @} */
