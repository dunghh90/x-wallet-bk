

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Top level header file that defines the exported functions interface for the
*     PTP stack
*
*******************************************************************************/

#ifndef _ZL303XX_PTP_FUNCTION_EXPORT_H_
#define _ZL303XX_PTP_FUNCTION_EXPORT_H_

/*****************   INCLUDE FILES                *****************************/
/* Include key files from the zlApi */
#include "zl303xx_Global.h"

#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpDatatypes.h"

/*****************   DEFINES     **********************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/* Functions for internal API use. Should not need to be called by applications */

zlStatusE zl303xx_PtpGetNumServers(zl303xx_PtpStreamHandleT streamHandle,
                                   Sint16T *numMcastServers,
                                   Sint16T *numUcastServers);

zlStatusE zl303xx_SetCurrentPTPTime(zl303xx_PtpPortHandleT clockPortHandle, Uint32T secondsLo, Uint32T secondsHi);
zlStatusE zl303xx_SetCurrentUTCTime(zl303xx_PtpPortHandleT clockPortHandle, char *dateTimeStr);

/* Routines to search for an internal Clock Data Structure. */
zlStatusE zl303xx_PtpClockDataGet(zl303xx_PtpClockHandleT clockHandle, zl303xx_PtpClockS **ppClock);
zlStatusE zl303xx_PtpClockDataGetForProfile(zl303xx_PtpClockHandleT clockHandle, zl303xx_PtpProfileE ptpProfile, zl303xx_PtpClockS **ppClock);
zlStatusE zl303xx_PtpClockDataGetForIdentity(zl303xx_ClockIdentity clockIdentity, zl303xx_PtpClockS **ppClock);

/* Routines to search for an internal Port Data Structure. */
zlStatusE zl303xx_PtpPortDataGet(zl303xx_PtpPortHandleT portHandle, zl303xx_PortDataS **ppPort);
zlStatusE zl303xx_PtpPortDataGetForProfile(zl303xx_PtpPortHandleT portHandle, zl303xx_PtpProfileE ptpProfile, zl303xx_PortDataS **ppPort);
zlStatusE zl303xx_PtpVirtualPortDataGet(zl303xx_PtpPortHandleT portHandle, zl303xx_PortDataS **ppPort);
zlStatusE zl303xx_PtpPortDataFromPortNumber(zl303xx_PtpClockS *pPtpClock, Uint16T portNumber, zl303xx_PortDataS **portData);
zlStatusE zl303xx_PtpPortDataFromPortIdentity(zl303xx_PortIdentity *pPortIdentity, zl303xx_PortDataS **portData);

/* Routines to search for an internal Stream Data Structure. */
zlStatusE zl303xx_PtpStreamDataGet(zl303xx_PtpStreamHandleT streamHandle, zl303xx_ClockStreamS **ppStream);
zlStatusE zl303xx_PtpStreamDataGetForProfile(zl303xx_PtpStreamHandleT streamHandle, zl303xx_PtpProfileE ptpProfile, zl303xx_ClockStreamS **ppStream);

/* Initiates a Unicast Negotiation Message from an existing Stream. */
zlStatusE zl303xx_PtpV2UnicastNegTlvInitiate(
      zl303xx_PortAddress *destAddr,
      zl303xx_ClockStreamS *pPtpStream,
      const zl303xx_PortIdentity *pTargetPortId,
      Uint8T *pUniNegTlvBuf, Uint8T uniNegTlvBufLen);

/* Responds to a received Unicast TLV message (the STREAM may not be known
 * so any necessary response data is supplied). */
zlStatusE zl303xx_PtpV2UnicastNegTlvRespond(
      zl303xx_PortAddress *destAddr,
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PortDataS *pPtpPort,
      zl303xx_PtpV2MsgHeaderS *pPtpV2MsgHdrRx,
      Uint8T *pUniNegTlvBuf, Uint8T uniNegTlvBufLen);

void zl303xx_PtpV2UnicastQueryInterval(zl303xx_ClockStreamS *pStream);

#endif   /* MULTIPLE INCLUDE BARRIER */
