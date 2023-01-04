/*!
* @skip $Id:$
* @file f_frw_ftyp.h
* @brief pf_frw thread's function declaration file
* @date  2015/08/07 TDIPS)sasaki update
* @date  2015/08/14 TDIPS)sasaki Add f_frw_flashFwInfoSaveReq
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
*/

#include "f_sys_type.h"

/** @addtogroup RRH_PF_FRW
 *  @{
 */
#ifndef F_FRW_FTYP_H
#define F_FRW_FTYP_H

extern VOID	f_frw_main( VOID );

extern VOID	f_frw_init(VOID);

extern VOID	f_frw_anlz( VOID * );

extern VOID	f_frw_nop(VOID *);

extern VOID	f_frw_flashEraseReq(VOID *);

extern VOID	f_frw_flashWriteReq(VOID *);

extern VOID	f_frw_flashWriteEvtLog(VOID *);

extern VOID	f_frw_troubleLogWriteReq(VOID *);

extern UINT f_frw_cm_EraseFlash(UINT, UINT, UINT);

extern UINT f_frw_com_WriteFlash(UINT, UINT, UINT, UCHAR*);

extern UINT f_frw_com_WriteFlash_slc1chp(UINT, UINT, UINT, UCHAR*);

extern UINT f_frw_cm_WriteEvtLog(UINT, UINT, UINT);

extern VOID f_frw_flashFwInfoSaveReq(VOID *);

extern VOID f_frw_cm_RunHisSet(UINT, UCHAR*);

extern VOID f_frw_logPrint(FILE *outf);

extern INT f_frw_load_fwinfo( VOID );

#endif
    
/** @} */
