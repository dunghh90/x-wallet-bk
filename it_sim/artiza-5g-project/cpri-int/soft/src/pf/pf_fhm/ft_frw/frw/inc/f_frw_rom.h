/*!
 * @skip  $ld:$
 * @file  f_frw_rom.c
 * @brief ROM definition.
 * @date  2013/1/13 FFCS)linb Create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

#include "f_frw_def.h"
    
/*!
 * @addtogroup RRH_PF_FRW
 * @{
 */
/****************************************************************************/
/* frw task function matrix                                                */
/****************************************************************************/
extern VOID (*const f_frwr_matrix[D_FRW_STATMAX][D_FRW_EVTMAX])(VOID* );

/* @} */
