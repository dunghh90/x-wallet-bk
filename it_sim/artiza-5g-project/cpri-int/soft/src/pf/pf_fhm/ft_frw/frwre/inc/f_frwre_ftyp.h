/*!
* @skip $Id:$
* @file f_frwre_ftyp.h
* @brief pf_frwre thread's function declaration file
* @date 2015/08/04 TDIPS)sasaki create
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
*/

#include "f_sys_type.h"

/** @addtogroup RRH_PF_FRWRE
 *  @{
 */
#ifndef F_FRWRE_FTYP_H
#define F_FRWRE_FTYP_H

extern VOID	f_frwre_main( VOID );

extern VOID	f_frwre_init( VOID );

extern VOID	f_frwre_anlz( VOID* );

extern VOID	f_frwre_nop( VOID* );

extern VOID f_frwre_flashEraseReq( VOID* );

extern VOID f_frwre_flashWriteReq( VOID* );

extern UINT f_frwre_cm_EraseFlash( UINT, UINT, UINT );

extern UINT f_frwre_com_WriteFlash( UINT, UINT, UINT, UCHAR* );

extern VOID f_frwre_cm_RunHisSet( UINT, UCHAR* );

extern VOID f_frwre_logPrint( FILE* );

extern VOID f_frwre_flashReadReq( VOID* );

#endif
    
/** @} */
