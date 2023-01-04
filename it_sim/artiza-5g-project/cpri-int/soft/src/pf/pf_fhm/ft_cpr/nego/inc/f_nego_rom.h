/*!
* @skip $Id:$
* @file f_nego_rom.h
* @brief pf_nego thread ROM table extern declaration
* @date 2015/04/22 FFCS)fuxg create.
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
*/
/** @addtogroup RRH_PF_NEGO
 *  @{
 */

#ifndef F_NEGO_ROM_H
#define F_NEGO_ROM_H

/* extm function matrix */
extern VOID (*const f_nego_matrix[D_NEGO_STATMAX][D_NEGO_EVTMAX])(UINT* );

#endif

/** @} */
