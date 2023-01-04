



/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains generic functions to configure, start, and stop an
*     example timing application consisting of one or more software modules. The
*     application structure is broken down into 3 major subcomponents:
*
*     - Clock: A hardware device capable generating a timing signal used for time
*       stamping.
*     - Port: A logical point of access to a network carrying timing traffic.
*     - Stream: A uni- or bi-directional connection between two endpoints in the
*       timing network (e.g., server to client).
*
*******************************************************************************/

#ifndef _ZL303XX_EXAMPLE_MAIN_H_
#define _ZL303XX_EXAMPLE_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif


/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_DeviceSpec.h"
#if defined APR_INCLUDED
#include "zl303xx_ExampleApr.h"
#endif

#if defined ZLS30341_INCLUDED
#include "zl303xx_Example34x.h"
#endif

#if defined ZLS30361_INCLUDED
#include "zl303xx_Example36x.h"
#endif

#if defined ZLS30721_INCLUDED
#include "zl303xx_Example72x.h"
#endif

#if defined ZLS30701_INCLUDED
#include "zl303xx_Example70x.h"
#endif

#if defined ZLS30751_INCLUDED
#include "zl303xx_Example75x.h"
#endif

#if defined ZLS30771_INCLUDED
#include "zl303xx_Example77x.h"
#endif

#if defined ZLS30390_INCLUDED 
#include "zl303xx_ExamplePtp.h"
#include "zl303xx_ExamplePtpGlobals.h"
#endif

/*****************   DEFINES   ************************************************/
/*Values to stop calling zl303xx_SetExampleInterface automatically, -2 is ignored. -1 is custom str*/
#define ZL303XX_EXAMPLE_INTERFACE_IGNORE      (-2)
#define ZL303XX_EXAMPLE_INTERFACE_CUSTOM_STR  (-1)
/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

typedef struct
{
#if defined APR_INCLUDED
   exampleAprStreamCreateS apr;
#endif

#if defined ZLS30390_INCLUDED 
   examplePtpStreamCreateS ptp;
#endif

   zl303xx_BooleanE started;
} exampleAppStreamS;

typedef struct
{
#if defined ZLS30390_INCLUDED 
   examplePtpPortCreateS ptp;
   Sint32T exampleInterface;                                            /* Used for calling zl303xx_SetExampleInterface() */
   char exampleInterfaceCustomStr[ZL303XX_MAX_EXAMPLE_INTERFACE_LENGTH];  /* Used for calling zl303xx_SetExampleInterfaceCustomStr() if exampleInterfaceIndex is -1 */
#endif
   zl303xx_BooleanE started;
} exampleAppPortS;

typedef struct
{
#if defined APR_INCLUDED
    exampleAprClockCreateS apr;
#endif

#if defined ZLS30341_INCLUDED
   example34xClockCreateS zl3034x;
#endif

#if defined ZLS30361_INCLUDED
   example36xClockCreateS zl3036x;
#endif

#if defined ZLS30721_INCLUDED
   example72xClockCreateS zl3072x;
#endif

#if defined ZLS30701_INCLUDED
   example70xClockCreateS zl3070x;
#endif

#if defined ZLS30751_INCLUDED
   example75xClockCreateS zl3075x;
#endif

#if defined ZLS30771_INCLUDED
   example77xClockCreateS zl3077x;
#endif

#if defined ZLS30390_INCLUDED 
   examplePtpClockCreateS ptp;
#endif

   zl303xx_BooleanE pktRef;  /* Set FALSE for master, TRUE for slave/BC */

   zl303xx_BooleanE started;
} exampleAppClockS;

typedef struct
{
   exampleAppClockS  *clock;
   Uint32T            clockCount;

   exampleAppPortS   *port;
   Uint32T            portCount;

   exampleAppStreamS *stream;
   Uint32T            streamCount;
   Uint32T            srvId;
} exampleAppS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

extern exampleAppS zlExampleApp;

extern zl303xx_ParamsS *zl303xx_Params0;
extern zl303xx_ParamsS *zl303xx_Params1;

extern Uint8T TARGET_DPLL;
extern Uint8T SYNCE_DPLL;
extern Uint8T CHASSIS_DPLL;

extern zl303xx_BooleanE reset1588HostRegisterOnClockCreate;

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

#ifdef ZLS30390_INCLUDED
/* Example PTP configurations */
zlStatusE examplePtpMultiMaster(void);
zlStatusE examplePtpMultiSlave(void);

zlStatusE examplePtpUniNegMaster(void);
zlStatusE examplePtpUniNegSlave(void);

zlStatusE examplePtpUniNegSlaveHybrid(void);

zlStatusE examplePtpUniNegMaster2(void);
zlStatusE examplePtpUniNegMaster3(void);
zlStatusE examplePtpUniNegMaster4(void);
zlStatusE examplePtpUniNegSlave_4Streams(void);

zlStatusE examplePtpMultiBC(void);
zlStatusE examplePtpMultiBCSlave(void);

/* G8275.1 (edition 1) */
zlStatusE examplePtpTelecomPhaseG8275p1Master(void);
zlStatusE examplePtpTelecomPhaseG8275p1Bc(void);
zlStatusE examplePtpTelecomPhaseG8275p1BcSlave(void);
zlStatusE examplePtpTelecomPhaseG8275p1Slave(void);
zlStatusE examplePtpTelecomPhaseG8275VirtualPortAdd(zl303xx_PtpClockHandleT clockHandle);

/* G8275.1 (edition 2) */
zlStatusE examplePtpTelecomPhaseG8275p1v2Master(void);
zlStatusE examplePtpTelecomPhaseG8275p1v2Bc(void);
zlStatusE examplePtpTelecomPhaseG8275p1v2BcSlave(void);
zlStatusE examplePtpTelecomPhaseG8275p1v2Slave(void);

/* G8275.2 */
zlStatusE examplePtpTelecomPhaseG8275p2Master(void);
zlStatusE examplePtpTelecomPhaseG8275p2Bc(void);
zlStatusE examplePtpTelecomPhaseG8275p2Slave(void);

zlStatusE examplePtpTelecomPhaseMasterIPv4(void);
zlStatusE examplePtpTelecomPhaseBcIPv4(void);
zlStatusE examplePtpTelecomPhaseSlaveIPv4(void);
zlStatusE examplePtpTelecomPhaseBcSlaveIPv4(void);

zlStatusE examplePtpPowerProfileMaster(void);
zlStatusE examplePtpPowerProfileSlave(void);

zlStatusE examplePtpPeerDelayMaster(void);
zlStatusE examplePtpPeerDelaySlave(void);

void setExampleMulticastIpDest(char const *destIP);
void setExampleUniIpSlaveDest(char const *destIP);
zlStatusE exampleConfig(zl303xx_BooleanE isUnicast, zl303xx_BooleanE slaveCapable, zl303xx_PtpVersionE ptpVersion);
zlStatusE examplePtpPortAdd(Uint16T portNumber, Uint8T const *localAddr);
void setExampleIpSrc(const char *srcIp);

zlStatusE examplePtpTelecomMaster(void);
zlStatusE examplePtpTelecomMaster2(void);
zlStatusE examplePtpTelecomMaster3(void);
zlStatusE examplePtpTelecomMaster4(void);
zlStatusE examplePtpTelecomMaster5(void);
zlStatusE examplePtpTelecomMaster6(void);
zlStatusE examplePtpTelecomMaster7(void);
zlStatusE examplePtpTelecomMaster8(void);
zlStatusE examplePtpTelecomMaster9(void);

zlStatusE examplePtpTelecomSlave(void);

zlStatusE examplePtpTelecomMultiSlave(void);
zlStatusE examplePtpTelecomMultiSlave2(void);
zlStatusE examplePtpTelecomMultiSlave3(void);
zlStatusE examplePtpTelecomMultiMaster(void);

zlStatusE examplePtpTelecomBCMaster(void);
zlStatusE examplePtpTelecomBC(void);
zlStatusE examplePtpTelecomBCSlave(void);

zlStatusE examplePtpTwoClock(void);
zlStatusE examplePtpTwoClockMaster(void);
zlStatusE examplePtpTwoClockSlave(void);
#endif

#ifdef ZLS3038X_INCLUDED
zlStatusE exampleAprMain(void);
zlStatusE exampleAprHybrid(void);
#endif



/* Shutdown function */
zlStatusE exampleShutdown(void);

/* Application Management */
zlStatusE exampleEnvInit(void);
zlStatusE exampleEnvClose(void);
zlStatusE exampleAppStructInit(Uint32T numClocks,  Uint32T numPorts,
                               Uint32T numStreams, exampleAppS *pApp);
zlStatusE exampleAppStructFree(exampleAppS *pApp);
zlStatusE exampleAppStart(exampleAppS *pApp);
zlStatusE exampleAppStartClock(exampleAppS *pApp, Uint32T indx);
zlStatusE exampleAppStartPort(exampleAppS *pApp, Uint32T indx);
zlStatusE exampleAppStartStream(exampleAppS *pApp, Uint32T indx);
zlStatusE exampleAppStop(exampleAppS *pApp);
zl303xx_BooleanE exampleAppIsRunning(void);

zlStatusE exampleReset1588HostRegisterOnClockCreateSet(zl303xx_BooleanE set);
zl303xx_BooleanE exampleReset1588HostRegisterOnClockCreateGet(void);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
