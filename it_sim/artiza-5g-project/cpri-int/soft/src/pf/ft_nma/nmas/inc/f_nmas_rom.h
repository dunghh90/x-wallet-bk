/*!
 * @skip  $ld:$
 * @file  f_nmas_rom.c
 * @brief ROM definition.
 * @date  2019/02/21 KCN)kamimoto  Create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

#include "f_nmas_def.h"
    
/*!
 * @addtogroup RRH_PF_NMAS
 * @{
 */
/****************************************************************************/
/* nmas task function matrix                                                */
/****************************************************************************/
extern VOID (*const f_nmasr_matrix[D_NMAS_STATMAX][D_NMAS_EVTMAX])(VOID* );

/* @} */
