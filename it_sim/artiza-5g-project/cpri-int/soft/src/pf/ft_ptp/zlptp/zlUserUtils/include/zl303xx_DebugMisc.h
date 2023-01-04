

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This is the header file for the accessing routines in zl303xx_DebugMisc.c.
*
*******************************************************************************/

#ifndef _ZL303XX_DEBUGMISC_H
#define _ZL303XX_DEBUGMISC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "zl303xx_Global.h"
#include "zl303xx.h"

zlStatusE zl303xx_DebugHwRefStatus(zl303xx_ParamsS *zl303xx_Params, Uint32T refId);
zlStatusE zl303xx_DebugDpllStatus(zl303xx_ParamsS *zl303xx_Params, Uint32T dpllId);

void zl303xx_DebugApiBuildInfo(void);

#ifdef __cplusplus
}
#endif

#endif /* _ZL303XX_DEBUGMISC_H */
