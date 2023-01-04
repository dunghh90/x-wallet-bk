/*!
* @skip $Id:$
* @file f_nma_ftyp.h
* @brief pf_nma thread's function declaration file
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
*/

#include "f_sys_type.h"

/** @addtogroup RRH_PF_NMA
 *  @{
 */
#ifndef F_NMA_FTYP_H
#define F_NMA_FTYP_H

extern VOID	f_nma_main( VOID );

extern VOID	f_nma_init(VOID * );

extern VOID	f_nma_anlz( VOID * );

extern VOID	f_nma_nop(VOID *);

extern VOID	f_nma_flashWriteEvtLog(VOID *);

extern VOID	f_nma_troubleLogWriteReq(VOID *);

extern VOID	f_nma_InstallFilWriteReq(VOID *);

extern VOID	f_nma_NetConfDataWriteReq(VOID *);

extern VOID	f_nma_VLANDataWriteReq(VOID *);

extern UINT f_nma_cm_EraseFlash(UINT, UINT, UINT);

extern UINT f_nma_com_WriteFlash(UINT, UINT, UINT, UCHAR*);

extern UINT f_nma_cm_WriteEvtLog(UINT, UINT, UINT);

extern VOID f_nma_logPrint(FILE *outf);

#endif
    
/** @} */
