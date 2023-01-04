/*!
 * @skip  $ld:$
 * @file  f_nma_rom.c
 * @brief ROM definition.
 * @date  2013/1/13 FFCS)linb Create\n
 * @date  2018/08/27 KCN)若水  Create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

#include "f_nma_def.h"
    
/*!
 * @addtogroup RRH_PF_NMA
 * @{
 */
/****************************************************************************/
/* nma task function matrix                                                */
/****************************************************************************/
extern VOID (*const f_nmar_matrix[D_NMA_STATMAX][D_NMA_EVTMAX])(VOID* );

/* @} */
