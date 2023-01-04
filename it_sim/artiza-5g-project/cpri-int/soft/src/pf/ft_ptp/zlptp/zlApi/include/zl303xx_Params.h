

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Prototypes for structure initialisation and locking.
*
*******************************************************************************/

#ifndef _ZL303XX_PARAMS_H_
#define _ZL303XX_PARAMS_H_

#ifdef __cplusplus
extern "C" {
#endif

zlStatusE zl303xx_CreateDeviceInstance(zl303xx_ParamsS **zl303xx_Params);
zlStatusE zl303xx_FreeDeviceInstance(zl303xx_ParamsS **zl303xx_Params);

zlStatusE zl303xx_LockDevParams(zl303xx_ParamsS *zl303xx_Params);
zlStatusE zl303xx_UnlockDevParams(zl303xx_ParamsS *zl303xx_Params);

#ifdef __cplusplus
}
#endif

#endif


