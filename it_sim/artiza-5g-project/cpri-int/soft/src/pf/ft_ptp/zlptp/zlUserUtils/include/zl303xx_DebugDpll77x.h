

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Debug functions for the ZL3077X DPLL.
*
*******************************************************************************/

#ifndef ZL303XX_DEBUG_DPLL_77X_H_
#define ZL303XX_DEBUG_DPLL_77X_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_DeviceSpec.h"

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/
zlStatusE zl303xx_DebugPllStatus77x(zl303xx_ParamsS *zl303xx_Params);
zlStatusE zl303xx_DebugHwRefStatus77x(zl303xx_ParamsS *zl303xx_Params, Uint32T refId);
zlStatusE zl303xx_DebugHwRefCfg77x(zl303xx_ParamsS *zl303xx_Params, Uint32T refId);
zlStatusE zl303xx_DebugDpllConfig77x(zl303xx_ParamsS *zl303xx_Params);
zlStatusE zl303xx_DebugPrintMailbox77x(zl303xx_ParamsS *zl303xx_Params,
                                        ZLS3077X_MailboxE mb, Uint32T mbNum);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
