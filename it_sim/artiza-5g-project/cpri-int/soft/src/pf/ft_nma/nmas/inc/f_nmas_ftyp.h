/*!
* @skip $Id:$
* @file f_nmas_ftyp.h
* @brief pf_nmas thread's function declaration file
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
*/

#include "f_sys_type.h"

/** @addtogroup RRH_PF_NMAS
 *  @{
 */
#ifndef F_NMAS_FTYP_H
#define F_NMAS_FTYP_H

extern VOID	f_nmas_main( VOID );

extern VOID	f_nmas_init(VOID * );

extern VOID	f_nmas_anlz( VOID * );

extern VOID	f_nmas_nop(VOID *);

extern VOID	f_nmas_flashWriteEvtLog(VOID *);

extern VOID	f_nmas_troubleLogWriteReq(VOID *);

extern VOID	f_nmas_InstallFilWriteReq(VOID *);

extern VOID	f_nmas_NetConfDataWriteReq(VOID *);

extern VOID	f_nmas_VLANDataWriteReq(VOID *);

extern UINT f_nmas_cm_EraseFlash(UINT flshOffset, UINT size);

extern UINT f_nmas_com_WriteFlash(UINT, UINT, UINT, UCHAR*);

extern UINT f_nmas_cm_WriteEvtLog(UINT, UINT, UINT);

extern VOID f_nmas_logPrint(FILE *outf);

extern VOID f_nmas_LutLogWriteNtf(VOID* );
extern VOID f_nmas_LutLogDecode( VOID );

#endif
    
/** @} */
