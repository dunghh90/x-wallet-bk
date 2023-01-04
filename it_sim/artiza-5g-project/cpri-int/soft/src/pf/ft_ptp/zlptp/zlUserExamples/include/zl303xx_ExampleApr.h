

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Header file for APR example files
*
*******************************************************************************/

#ifndef _ZL303XX_EXAMPLE_APR_H_
#define _ZL303XX_EXAMPLE_APR_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_Apr.h"
#include "zl303xx_DeviceSpec.h"

/*****************   DEFINES   ************************************************/

#define ZL303XX_HYBRID_TRANSIENT_ON   (1)
#define ZL303XX_HYBRID_TRANSIENT_OFF  (0)

/* ƒ‰ƒbƒ`’x‰„ŽžŠÔ */
#define LTC_LATCH_DELAY_NS            0

/*****************   DATA TYPES   *********************************************/

typedef struct
{
   zl303xx_ParamsS *cguId;
   zl303xx_AprAddServerS server;
} exampleAprStreamCreateS;

typedef struct
{
   zl303xx_ParamsS *cguId;
   zl303xx_AprAddDeviceS device;

} exampleAprClockCreateS;



/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

zlStatusE exampleAprEnvInit(void);
zlStatusE exampleAprEnvClose(void);

zlStatusE exampleAprClockCreateStructInit(exampleAprClockCreateS *pClock);
zlStatusE exampleAprClockCreate(exampleAprClockCreateS *pClock);
zlStatusE exampleAprClockRemove(exampleAprClockCreateS *pClock);

zlStatusE exampleAprStreamCreateStructInit(exampleAprStreamCreateS *pStream);
zlStatusE exampleAprStreamCreate(exampleAprStreamCreateS *pStream);
zlStatusE exampleAprStreamRemove(exampleAprStreamCreateS *pStream);

Sint32T exampleAprDcoSetFreq(void *clkGenId, Sint32T freqOffsetInPartsPerTrillion);
Sint32T exampleAprDcoGetFreq(void *clkGenId, Sint32T *freqOffsetInPartsPerTrillion);
Sint32T exampleAprGetHwManualFreerunStatus(void *hwParams, Sint32T *manStatus);
Sint32T exampleAprGetHwManualHoldoverStatus(void *hwParams, Sint32T *manStatus);
Sint32T exampleAprGetHwLockStatus(void *hwParams, Sint32T *manStatus);
Sint32T exampleAprDefaultgetHwSyncInputEnStatus(void *hwParams, Sint32T *manStatus);
Sint32T exampleAprDefaultgetHwOutOfRangeStatus(void *hwParams, Sint32T *manStatus);
Sint32T exampleAprDefaultSwitchToPacketRef(void *hwParams);
Sint32T exampleAprDefaultSwitchToElectricalRef(void *hwParams);

Sint32T exampleAprSetTimeTsu(void *clkGenId, 
                             Uint64S deltaTimeSec, 
                             Uint32T deltaTimeNanoSec,
                             zl303xx_BooleanE negative);
Sint32T exampleAprStepTimeTsu(void *clkGenId, 
                             Sint32T deltaTimeNs);
Sint32T exampleAprJumpTimeTsu(void *clkGenId,
                             Uint64S deltaTimeSec,
                             Uint32T deltaTimeNanoSec,
                             zl303xx_BooleanE negative);

zlStatusE exampleAprHandleHybridTransient(void *hwParams, zl303xx_BCHybridTransientType transient);

/* Message router example for custom device */
Sint32T zl303xx_DpllCustomMsgRouter(void *hwParams, void *inData, void *outData);

/* Example code message router */
Sint32T zl303xx_UserMsgRouter(void *hwParams, void *inData, void *outData);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
