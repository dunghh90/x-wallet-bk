/*!
* @skip $Id:$
* @file f_fmng_rom.h
* @brief RSV ROM table extern declaration
* @date 2008/09/08 FFCS)limin create.
* @date 2015/08/11 TDIPS)sasaki update.
* @date 2015/08/17 TDIPS)sasaki FHM対応
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
*/
/** @addtogroup RRH_PF_FMNG
 *  @{
 */

#include "f_fmng_def.h"
 
#ifndef F_FMNG_ROM_H
#define F_FMNG_ROM_H

/* CDSV function matrix */
extern VOID (*const f_fmngr_matrix[D_FMNG_STATMAX][D_FMNG_EVTMAX])(VOID* );

/* CDSV function matrix for RE-FLASH */
extern VOID (*const f_fmngr_re_matrix[D_FMNG_RE_STATMAX][D_FMNG_RE_EVTMAX])( VOID* );

extern	const	USHORT		f_fmngr_makername_settbl[D_SYS_NUM5];

#endif
/** @} */
