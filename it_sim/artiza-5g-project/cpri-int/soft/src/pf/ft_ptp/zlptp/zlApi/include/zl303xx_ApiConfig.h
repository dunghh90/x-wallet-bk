

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Functions for setting and getting the user configurable parameters.
*
*******************************************************************************/

#ifndef ZL303XX_APICONFIG_H_
#define ZL303XX_APICONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#if defined ZLS30341_INCLUDED
#include "zl303xx.h"        /* Required for zl303xx_ParamS Definition   */
#else
#include "zl303xx_DeviceSpec.h"
#endif
#include "zl303xx_Init.h"

#if defined ZLS30390_INCLUDED
#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpDeprecated.h"
#endif

#if defined APR_INCLUDED
#include "zl303xx_Apr.h"
#endif

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXPORTED FUNCTION DEFINITIONS   ***************************/
#if defined ZLS30390_INCLUDED
/* Completion flags */
zlStatusE zl303xx_SetClockConfigComplete(void);
zlStatusE zl303xx_ResetClockConfig(void);
zl303xx_BooleanE zl303xx_ClockConfigComplete(void);

zlStatusE zl303xx_SetCurrentClockStream(zl303xx_PtpStreamHandleT clockStreamHandle);
zl303xx_PtpStreamHandleT zl303xx_GetCurrentClockStream(void);

zlStatusE zl303xx_SetStreamConfigComplete(const zl303xx_PtpStreamHandleT clockStreamHandle);
zl303xx_BooleanE zl303xx_StreamConfigComplete(const zl303xx_PtpStreamHandleT clockStreamHandle);

/* Protocol Type */
zlStatusE zl303xx_SetProtocolType(zl303xx_ProtocolTypesE protocolType);
zl303xx_ProtocolTypesE zl303xx_GetProtocolType(void);
zl303xx_BooleanE zl303xx_IsMplsProtocolType(void);
zl303xx_BooleanE zl303xx_IsVlanProtocolType(void);
zl303xx_BooleanE zl303xx_IsRawProtocolType(void);
zl303xx_BooleanE zl303xx_IsRawPtpProtocolType(void);
zl303xx_BooleanE zl303xx_IsIPv4SktProtocol(zl303xx_ProtocolTypesE protocol);
zl303xx_BooleanE zl303xx_IsIPv6SktProtocol(zl303xx_ProtocolTypesE protocol);
zl303xx_BooleanE zl303xx_IsIPv6ProtocolType(zl303xx_ProtocolTypesE protocol);
zl303xx_BooleanE zl303xx_IsEthPtpProtocolType(zl303xx_ProtocolTypesE protocol);

/* warning‘Î‰ž‚Å’Ç‰Á */
zlStatusE zl303xx_SetVlanTag(Uint8T stackPos, const clockStreamHandleT clockStreamHandle, const Uint32T vlanTag);
Uint32T zl303xx_GetVlanTag(Uint8T stackPos, const clockStreamHandleT clockStreamHandle);
zlStatusE zl303xx_SetPktSrcIPv6(const clockStreamHandleT clockStreamHandle, const Uint8T *srcAddr);
const Uint8T *zl303xx_GetPktSrcIPv6(const clockStreamHandleT clockStreamHandle);
zlStatusE zl303xx_SetPktDestIPv6(const clockStreamHandleT clockStreamHandle, const Uint8T *destAddr);
const Uint8T *zl303xx_GetPktDestIPv6(const clockStreamHandleT clockStreamHandle);
clockStreamHandleT zl303xx_GetIPv6ClockStream(const Uint8T *srcAddr);
zlStatusE zl303xx_SetUdpEvent(const Uint16T udpPort);
Uint16T zl303xx_GetUdpEvent(void);
zlStatusE zl303xx_SetUdpGeneral(const Uint16T udpPort);
Uint16T zl303xx_GetUdpGeneral(void);



#endif







#if defined ZLS30341_INCLUDED
/* System interrupt config. */
zlStatusE zl303xx_SetLog2SysInterruptPeriod(Uint32T log2Period);
Uint32T zl303xx_GetLog2SysInterruptPeriod(void);
#endif


#ifdef OS_LINUX
zlStatusE zl303xx_SetCliPriority(Uint8T taskPriority);
Uint8T zl303xx_GetCliPriority(void);
#endif  /* OS_LINUX */

#ifdef __cplusplus
}
#endif

#endif /*ZL303XX_APICONFIG_H_*/
