/*!
* @skip $Id:$
* @file f_frwsla_ftyp.h
* @brief pf_frwsla thread's function declaration file
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
*/

#include "f_sys_type.h"

/** @addtogroup RRH_PF_FRWSLA
 *  @{
 */
#ifndef F_FRWSLA_FTYP_H
#define F_FRWSLA_FTYP_H

extern VOID	f_frwsla_main( VOID );

extern VOID	f_frwsla_init(VOID);

extern VOID	f_frwsla_anlz( VOID * );

extern VOID	f_frwsla_nop(VOID *);

extern VOID	f_frwsla_flashEraseReq(VOID *);

extern VOID	f_frwsla_flashWriteReq(VOID *);

extern UINT f_frwsla_cm_EraseFlash(UINT, UINT, UINT);

extern UINT f_frwsla_com_WriteFlash(UINT, UINT, UINT, UCHAR*);

extern VOID f_frwsla_cm_RunHisSet(UINT, UCHAR*);

extern VOID f_frwsla_logPrint(FILE *outf);

extern VOID f_frwsla_flashReadReq(void *);
#endif
    
/** @} */
