

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains functions to configure, start, and stop PTP
*
*******************************************************************************/

#ifndef ZL303XX_EXAMPLE_PTP_GLOBALS_H_
#define ZL303XX_EXAMPLE_PTP_GLOBALS_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "zl303xx_Global.h"  /* This should always be the first include file */
#include "zl303xx_Error.h"
#include "zl303xx.h"

#if defined ZLS30390_INCLUDED
#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpConstants_dep.h"
#include "zl303xx_SocketTransport.h"

zlStatusE zl303xx_SetPtpProfile(zl303xx_PtpProfileE profile);
zl303xx_PtpProfileE zl303xx_GetPtpProfile(void);

zlStatusE exampleSetEgressClassUpdateTrigger(zl303xx_PtpClockEgressQlUpdateE updateTrigger);
zl303xx_PtpClockEgressQlUpdateE exampleGetEgressClassUpdateTrigger(void);

/***** General constants *****/
#define ZL303XX_INVALID (-1)
#define ZL303XX_INVALID_STREAM ZL303XX_INVALID
/* Maximum length of an internet address (IPv6) in ASCII format including NULL terminator */
#define ZL303XX_INET_ADDR_MAX_LEN   46


typedef struct
{
   zl303xx_PtpStreamHandleT clockStream;
   char localIpAddr[ZL303XX_INET_ADDR_MAX_LEN];
   char destIpAddr[ZL303XX_INET_ADDR_MAX_LEN];
} examplePtpStreamS;

extern examplePtpStreamS exampleMultiPtpStream;
extern examplePtpStreamS exampleUniPtpStream;
extern examplePtpStreamS exampleIPv6PtpStream;
extern examplePtpStreamS exampleIPv6UniPtpStream;
extern examplePtpStreamS exampleMultiPtpStream2;
extern examplePtpStreamS examplePdelayPtpStream;
extern examplePtpStreamS exampleUniPtpStream2;

extern zl303xx_PtpClockHandleT  PtpClockHandle;
extern zl303xx_PtpPortHandleT   PtpClockPortHandle;
extern zl303xx_PtpStreamHandleT PtpClockStreamHandle;

extern Uint32T FpeFilterHandle;
extern Uint32T FpeFilterHandleGen;

extern Uint8T *exampleSourceMac;
extern Uint8T *exampleDestMac;
#endif

#if defined ZLS30390_INCLUDED
/* Defines taken from transport headers not included */
#define ZL303XX_PKT_HDR_ETH2_ADDR_LEN           (Uint16T)6
#define ZL303XX_PKT_HDR_IPV6_ADDR_LEN           (Uint16T)16
#define ZL303XX_PKT_HDR_COOKIE_MAGIC_LEN        (Uint16T)8

/* MAC addresses are needed for all raw socket examples. */
extern Uint8T exampleMaster1MacAddr[ZL303XX_PKT_HDR_ETH2_ADDR_LEN];
extern Uint8T exampleMaster2MacAddr[ZL303XX_PKT_HDR_ETH2_ADDR_LEN];
extern Uint8T exampleSlave1MacAddr[ZL303XX_PKT_HDR_ETH2_ADDR_LEN];
extern Uint8T exampleSlave2MacAddr[ZL303XX_PKT_HDR_ETH2_ADDR_LEN];

/* Examples use 2 PTP ports sharing the same physical interface. */
extern Uint8T exampleBoundary1MacAddr[ZL303XX_PKT_HDR_ETH2_ADDR_LEN];
extern Uint8T exampleBoundary2MacAddr[ZL303XX_PKT_HDR_ETH2_ADDR_LEN];

extern Uint8T exampleBlockMacAddr[ZL303XX_PKT_HDR_ETH2_ADDR_LEN];
extern Uint8T exampleZeroMacAddr[ZL303XX_PKT_HDR_ETH2_ADDR_LEN];

/* IPv6 addresses are needed for raw socket examples with an IPv6 header. */
extern Uint8T exampleMaster1IPv6Addr[ZL303XX_PKT_HDR_IPV6_ADDR_LEN];
extern Uint8T exampleMaster2IPv6Addr[ZL303XX_PKT_HDR_IPV6_ADDR_LEN];

extern Uint8T exampleSlave1IPv6Addr[ZL303XX_PKT_HDR_IPV6_ADDR_LEN];
extern Uint8T exampleSlave2IPv6Addr[ZL303XX_PKT_HDR_IPV6_ADDR_LEN];

extern Uint8T exampleBoundary1IPv6Addr[ZL303XX_PKT_HDR_IPV6_ADDR_LEN];
extern Uint8T exampleBoundary2IPv6Addr[ZL303XX_PKT_HDR_IPV6_ADDR_LEN];

extern Uint8T const *exampleDestMacAddr;

extern Uint8T exampleCookieMagic[ZL303XX_PKT_HDR_COOKIE_MAGIC_LEN];

#define EVENT_PORT_CONFIG     DEFAULT_PTP_UDP_EVENT_PORT
#define GENERAL_PORT_CONFIG   DEFAULT_PTP_UDP_GENERAL_PORT

#endif

zlStatusE exampleSetTwoStepFlag(zl303xx_BooleanE twoStep);
zl303xx_BooleanE exampleGetTwoStepFlag(void);

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD 
/* Default compile-time ethernet interface for examples (see zl303xx_SetExampleInterfaceStr() for run-time) */
#if defined OS_VXWORKS
    #if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
        #define ZL303XX_MAX_EXAMPLE_INTERFACE_PORTS 4
        #define EXAMPLE_DEFAULT_INTERFACE_NUM     1
        /* ZLE30360 board uses VLAN tagging to control the switched ethernet ports */
        /* eth1.1 corresponds to PHY port B and port 1 to the s/w */
        #define EXAMPLE_ETHERNET_INTERFACE_0       "eth0"
        #define EXAMPLE_ETHERNET_INTERFACE_1       "eth1.1"
        #define EXAMPLE_ETHERNET_INTERFACE_2       "eth1.2"
        #define EXAMPLE_ETHERNET_INTERFACE_3       "eth1.3"
    #else
        #define ZL303XX_MAX_EXAMPLE_INTERFACE_PORTS 1
        #define EXAMPLE_ETHERNET_INTERFACE_0       "eth0"
        #define EXAMPLE_ETHERNET_INTERFACE_1       "eth1"
/* Original
 *      #warning MSCC: DEFAULT Ethernet port definitions for VxWorks
 */
     #endif

#elif defined OS_LINUX
    #if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD 
        #define ZL303XX_MAX_EXAMPLE_INTERFACE_PORTS 4
        #define EXAMPLE_DEFAULT_INTERFACE_NUM     1
        /* ZLE30360 board uses VLAN tagging to control the switched ethernet ports */
        /* eth1.1 corresponds to PHY port B and port 1 to the s/w */
        #define EXAMPLE_ETHERNET_INTERFACE_0       "eth0"
        #define EXAMPLE_ETHERNET_INTERFACE_1       "eth1.1"
        #define EXAMPLE_ETHERNET_INTERFACE_2       "eth1.2"
        #define EXAMPLE_ETHERNET_INTERFACE_3       "eth1.3"
    #endif
/* Original
 *#else
 *  #warning MSCC: Ethernet port definitions required for unknown OS
 */
#endif

#ifndef EXAMPLE_DEFAULT_INTERFACE_NUM
   #define EXAMPLE_DEFAULT_INTERFACE_NUM  0
#endif

/* No Board defined. */
#else
/* #warning MSCC: zl303xx_ExampleInterfaceStr DEFAULTS USED */
    #define ZL303XX_MAX_EXAMPLE_INTERFACE_PORTS 2 /* 1->2 */
    #define EXAMPLE_DEFAULT_INTERFACE_NUM  0
#endif

#if defined ZLS30390_INCLUDED 
/** exampleSetUseAnnexF

*   The function sets the global for using ANNEX_F Eth2 Mac
*   address for PeerToPeer (PeerDelay)

  Return Value: zlStatusE

****************************************************************************/
zlStatusE exampleSetUseAnnexF(zl303xx_BooleanE enabled);

/** exampleGetUseAnnexF

*   The function gets the global for using ANNEX_F Eth2 Mac
*   address for PeerToPeer (PeerDelay)

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE exampleGetUseAnnexF(void);

/** exampleSetUsePeerToPeer

*   The function sets the global for using Peer to Peer
*   timestamp format for PeerDelay

  Return Value: zlStatusE

****************************************************************************/
zlStatusE exampleSetUsePeerToPeer(zl303xx_BooleanE enabled);

/** exampleGetUsePeerToPeer

*   The function gets the global for using Peer to Peer
*   timestamp format for PeerDelay

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE exampleGetUsePeerToPeer(void);

zlStatusE zl303xx_SetUseDelayResponseFlag(zl303xx_BooleanE useDelayResponse);
zl303xx_BooleanE zl303xx_GetUseDelayResponseFlag(void);
zlStatusE zl303xx_SetUsePeerDelayResponseFlag(zl303xx_BooleanE usePeerDelayResponse);
zl303xx_BooleanE zl303xx_GetUsePeerDelayResponseFlag(void);

/** zl303xx_GetClockType / zl303xx_SetClockType

*   Function to get or set the clockType parameter used when initializing
*   and creating a PTP Clock.

  Return Value: Uint16T

****************************************************************************/
zlStatusE zl303xx_SetClockType(Uint16T clockType);
Uint16T zl303xx_GetClockType(void);

#endif

#if defined ZLS30390_INCLUDED

#define ZL303XX_MAX_EXAMPLE_INTERFACE_LENGTH 32
/* zl303xx_SetExampleInterface

   The function sets the ethernet interface to use for the given PTP port number
   portNo -1 is the custom interface (see zl303xx_SetExampleInterfaceCustomStr)

   Ex:
        zl303xx_SetExampleInterface(0);    // Use eth1 for PTP port 0

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_SetExampleInterface(Uint32T portNo);

/* zl303xx_SetExampleInterfaceCustomStr

   The function sets the custom ethernet interface to use for custom interfaceNo (-1)

   Ex: Use br0 interface
        zl303xx_SetExampleInterfaceCustomStr("br0");
        zl303xx_SetExampleInterface(-1);

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_SetExampleInterfaceCustomStr(const char *interfaceStr);

/* zl303xx_GetExampleInterface

   The function gets the ethernet interface in use

  Return Value: Uint32T exampleUseInterfaceNo
****************************************************************************/
Uint32T zl303xx_GetExampleInterface(void);


/** zl303xx_GetExampleInterfaceStr

   The function gets the ethernet interface to use for the given PTP port number

  Return Value: char *

****************************************************************************/
char * zl303xx_GetExampleInterfaceStr(Uint32T portNo);

#endif



#ifdef __cplusplus
}
#endif

#endif
