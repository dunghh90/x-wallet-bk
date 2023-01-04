

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Types and prototypes needed by the APR debug routines
*
*******************************************************************************/

#ifndef _ZL303XX_DEBUG_APR_H_
#define _ZL303XX_DEBUG_APR_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Int64.h"
#include "zl303xx_Error.h"

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

zlStatusE zl303xx_GetAprDeviceConfigInfo(void *hwParams);
zlStatusE zl303xx_GetAprDeviceStatus(void *hwParams);
zlStatusE zl303xx_GetAprServerConfigInfo(void *hwParams, Uint16T serverId);
zlStatusE zl303xx_GetAprServerStatus(void *hwParams, Uint16T serverId);
void zl303xx_DebugAprBuildInfo(void);

zlStatusE zl303xx_DebugPrintAprByStreamNum(void *hwParams, Uint32T streamNumber, Uint32T algorithmNumber);
zlStatusE zl303xx_DebugPrintAprByReferenceId(void *hwParams, Uint32T referenceID);


zlStatusE zl303xx_DebugGetAprPathStatistics(void * hwParams);
zlStatusE zl303xx_AprGetDeviceCurrentPathDelays(void * hwParams);
zlStatusE zl303xx_AprGetServerCurrentPathDelays(void * hwParams, Uint16T serverId);

zlStatusE zl303xx_DebugGetAllAprStatistics(void * hwParams);
zlStatusE zl303xx_DebugAprPrint1HzData(void *hwParams, Uint32T streamNumber, Uint32T algorithmNumber);
zlStatusE zl303xx_DebugAprGet1HzData(void *hwParams, Uint32T streamNumber, Uint32T algorithmNumber);
zlStatusE zl303xx_DebugAprPrintPSLFCLData(void *hwParams);

zlStatusE zl303xx_DebugHybridState(void *hwParams);
zlStatusE zl303xx_DebugGetServerStatus(void *hwParams, Uint16T serverId);
zlStatusE zl303xx_DebugCGULockState(void *hwParams);

zlStatusE zl303xx_DebugAprHoldoverQualityParams(void *hwParams);

zlStatusE zl303xx_DebugAprGetSWVersion(void);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */

