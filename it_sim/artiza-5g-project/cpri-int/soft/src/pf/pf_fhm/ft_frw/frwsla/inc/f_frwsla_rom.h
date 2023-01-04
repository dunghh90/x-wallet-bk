/*!
 * @skip  $ld:$
 * @file  f_frwsla_rom.c
 * @brief ROM definition.
 * @date  2014/12/12 FFCS)linb Create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014-2015
 */

#include "f_frwsla_def.h"
    
/*!
 * @addtogroup RRH_PF_FRWSLA
 * @{
 */
/****************************************************************************/
/* frwsla task function matrix                                                */
/****************************************************************************/
extern VOID (*const f_frwslar_matrix[D_FRWSLA_STATMAX][D_FRWSLA_EVTMAX])(VOID* );

/* @} */
