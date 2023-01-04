



/*******************************************************************************

   $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa

   Copyright 2006-2018 Microsemi Semiconductor Limited.
   All rights reserved.
   The timing algorithms implemented in the software code are Patent Pending.

*******************************************************************************/

#ifndef _ZL303XX_HOLDOVERUTILS_H
#define _ZL303XX_HOLDOVERUTILS_H


#ifdef __cplusplus
extern "C" {
#endif


/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_Apr.h"

/*****************   # DEFINES   **********************************************/

/*****************   # TYPES     **********************************************/


Sint32T zl303xx_DpllHoldoverStateChange(void *hwParams, zl303xx_HoldoverActionE holdoverAction);

zlStatusE zl303xx_GetHoldoverQuality(void *hwParams, zl303xx_HoldoverQualityParamsS *holdoverQualityParamsP);


#ifdef __cplusplus
}
#endif

#endif

