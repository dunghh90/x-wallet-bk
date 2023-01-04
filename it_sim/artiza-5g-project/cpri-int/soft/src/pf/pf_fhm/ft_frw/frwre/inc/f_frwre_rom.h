/*!
 * @skip  $ld:$
 * @file  f_frwre_rom.h
 * @brief ROM definition.
 * @date  2015/08/04 TDIPS)sasaki Create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_frwre_def.h"
    
/*!
 * @addtogroup RRH_PF_FRWRE
 * @{
 */
/****************************************************************************/
/* frwre task function matrix												*/
/****************************************************************************/
extern VOID (*const f_frwrer_matrix[D_FRWRE_STATMAX][D_FRWRE_EVTMAX])( VOID* );

/* @} */
