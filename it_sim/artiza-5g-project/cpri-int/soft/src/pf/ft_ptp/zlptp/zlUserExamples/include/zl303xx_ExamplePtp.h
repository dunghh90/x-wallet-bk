

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains functions to configure, start, and stop a PTP
*     application. Also included are example function bindings for various PTP
*     events and a simple socket abstraction.
*
*******************************************************************************/

#ifndef _ZL303XX_EXAMPLE_PTP_H_
#define _ZL303XX_EXAMPLE_PTP_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_ExamplePtpSocket.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
/* #include <net/if.h> */
#include <linux/sockios.h>

#ifndef ntohl
  #include <netinet/in.h>
#endif

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

typedef struct
{
   zl303xx_PtpClockCreateS config;
   zl303xx_PtpClockHandleT handle;
} examplePtpClockCreateS;

typedef struct
{
   zl303xx_PtpPortCreateS config;
   zl303xx_PtpPortHandleT handle;
} examplePtpPortCreateS;

typedef struct
{
   zl303xx_PtpStreamCreateS config;
   zl303xx_PtpStreamHandleT handle;
} examplePtpStreamCreateS;

typedef struct
{
   void *cguId;
   examplePtpSocketT skt;
} examplePtpStreamExtDataS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/* Configurable PTP clock parameters. */
/* Enable the twoStepFlag if the underlying hardware is incapable of inserting
 * time stamps into egress packets on-the-fly. */

/* Configurable PTP port parameters. */
extern zl303xx_PortAddress examplePtpLocalAddr[];
extern zl303xx_BooleanE examplePtpUseDelayMechanism;
extern zl303xx_DelayMechanismE examplePtpDelayMechanism;

/* Configurable PTP stream parameters. */
extern zl303xx_PortAddress examplePtpDestAddr[];
extern Sint8T examplePtpLogAnnounceInterval;
extern Uint8T examplePtpAnnounceReceiptTimeout;
extern Sint8T examplePtpLogSyncInterval;
extern Sint8T examplePtpLogMinDelayReqInterval;

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

zlStatusE examplePtpEnvInit(void);
zlStatusE examplePtpEnvClose(void);

zlStatusE examplePtpClockCreateStructInit(examplePtpClockCreateS *pClock);
zlStatusE examplePtpClockCreate(examplePtpClockCreateS *pClock);

/*zlStatusE examplePtpPortCreateStructInit(examplePtpPortCreateS *pPort); */
zlStatusE examplePtpPortCreateStructInit(examplePtpPortCreateS *pPort, int count);
zlStatusE examplePtpPortCreate(examplePtpPortCreateS *pPort);

zlStatusE examplePtpStreamCreateStructInit(examplePtpStreamCreateS *pStream, int count);
/* zlStatusE examplePtpStreamCreateStructInit(examplePtpStreamCreateS *pStream); */
zlStatusE examplePtpStreamCreate(examplePtpStreamCreateS *pStream);
zlStatusE examplePtpStreamCguIdSet(examplePtpStreamCreateS *pStream, void *cguId);

zlStatusE examplePtpLocalIpv4SetDefault(const char *ipv4Addr);
zlStatusE examplePtpDestIpv4SetDefault(const char *ipv4Addr);

zlStatusE examplePtpLocalIpv4Set(const char *ipv4Addr, zl303xx_BooleanE forceAddr,
                                 examplePtpPortCreateS *pPort);
zlStatusE examplePtpDestIpv4Set(const char *ipv4Addr, zl303xx_BooleanE forceAddr,
                                examplePtpStreamCreateS *pStream);
zlStatusE examplePtpDestEthL2Set(const char *eth2Addr, zl303xx_BooleanE forceAddr,
                                 examplePtpStreamCreateS *pStream);
/* Management functions */
zlStatusE examplePtpStop(void);
zlStatusE examplePtpClockDelete(zl303xx_PtpClockHandleT clockHandle);
zlStatusE examplePtpPortDelete(zl303xx_PtpPortHandleT portHandle);
zlStatusE examplePtpStreamDelete(zl303xx_PtpStreamHandleT streamHandle);
zlStatusE examplePtpContractsRequest(examplePtpStreamCreateS *pStream,
                                     Uint32T msgTypeList);
zlStatusE examplePtpUmtEntryPrioritySet(examplePtpStreamCreateS *pStream,
                                        Uint8T priority);

/* PTP function bindings */
void    examplePtpEventHandler(zl303xx_PtpEventE eventType, void *eventData);
Sint32T examplePtpHwCmdHandler(zl303xx_PtpHwCmdE cmdType, void *cmdParams);
Sint32T examplePtpTx(Uint8T *buffer, Uint16T bufferLen, zl303xx_PtpTxMsgDataS *pTxMsgData);

/* Function wrappers */
zlStatusE examplePtpRxMsgProcess(Uint8T *buffer, Uint16T bufferLen,
                                 zl303xx_PortAddress *pSrcAddr,
                                 zl303xx_PortAddress *pDestAddr);

/* timestampëŒâûÇ≈ä÷êîà¯êîïœçX */
zlStatusE examplePtpPortRxMsgProcess(Sint32T fd,
                                 zl303xx_PtpPortHandleT portHandle, 
                                 Uint8T *buffer, Uint16T bufferLen,
                                 zl303xx_PortAddress *pSrcAddr,
                                 zl303xx_PortAddress *pDestAddr);

zlStatusE exampleSetSrcDestAddressesToStructs(const char *srcIpv4Addr, const char *destIpv4Addr);

zlStatusE getPtpStatus(void);
void	getPtpParameter(void);
char*	getPtpParameter_strBool(int	pram);
char*	getPtpParameter_strUpdateTrigger(int	pram);
char*	getPtpParameter_strClockAccuracy(int	pram);
char*	getPtpParameter_strTimeSource(int	pram);
char*	getPtpParameter_strProfile(int	pram);
char*	getPtpParameter_strEgressQlUpdate(int	pram);
char*	getPtpParameter_strEvalMethod(int	pram);
char*	getPtpParameter_strNetworkProtocol(int	pram);
char*	getPtpParameter_strDelayMechanism(int	pram);
char*	getPtpParameter_strPdelayRespMethod(int	pram);
char*	getPtpParameter_strStreamMode(int	pram);

#if defined MUX_PTP_STREAMS_TO_APR_SLOTS
void zl303xx_InitPtpStreamMuxToAprSlots(void);
zlStatusE zl303xx_DisplayPtpStreamToAprSlots(void);
#endif


#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
