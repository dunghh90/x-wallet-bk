/*!
* @skip $Id:$
* @file f_cpr_rom.h
* @brief pf_cpri thread ROM table extern declaration
* @date 2013/11/14 FFCS)niemsh create.
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
*/
/** @addtogroup RRH_PF_CPR
 *  @{
 */

#ifndef F_CPR_ROM_H
#define F_CPR_ROM_H

/* extm function matrix */
extern VOID (*const f_cprr_matrix[D_CPR_STATMAX][D_CPR_EVTMAX])(UINT* );

extern const char *f_cpreeprom_sfp[D_CPR_EEPROM_SFPITEM_MAX];
extern const int f_sfp_bitrate[D_CPR_EEPROM_SFPITEM_MAX];
extern const int f_sfp_typerec[D_CPR_EEPROM_SFPITEM_MAX];

extern const UINT CPM_VOD_EMPHASIS_data98g[D_RRH_CPRINO_RE_MAX];
extern 	const UINT CPM_VOD_EMPHASIS_data49g[D_RRH_CPRINO_RE_MAX];
extern 	const UINT CPM_VOD_EMPHASIS_data24g[D_RRH_CPRINO_RE_MAX];

#endif

/** @} */
