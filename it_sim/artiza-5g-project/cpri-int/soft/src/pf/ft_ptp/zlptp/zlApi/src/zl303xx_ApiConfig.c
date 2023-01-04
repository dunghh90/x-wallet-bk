



/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Functions for setting and getting the user configurable parameters.
*
*******************************************************************************/


/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Os.h"
#if defined ZLS30341_INCLUDED
#include "zl303xx.h"        /* Required for zl303xx_ParamS Definition   */
#else
#include "zl303xx_DeviceSpec.h"
#endif
#include "zl303xx_Macros.h"

#include "zl303xx_ApiConfig.h"

#if defined ZLS30390_INCLUDED
#include "zl303xx_Pkt.h"
#include "zl303xx_PtpSetup.h"
#include "zl303xx_PtpApiTypes.h"

#ifdef OS_LINUX
  #include "zl303xx_LinuxSocketTransport.h"
#endif
#ifdef OS_VXWORKS
  #include "zl303xx_VxWorksSocketTransport.h"
  #include <arpLib.h>
#endif

#ifdef OS_LINUX
    #define _LINUX_TYPES_H   /* ZL - Stop multiple type includes */
    #define _LINUX_TIME_H

    #include <sys/types.h>

//  #if defined ZL_LNX_DENX || defined ZL_LNX_CODESOURCERY
//      #define __USE_MISC      /* Socket stuff */
//  #endif

    #include <arpa/inet.h>
    #include <net/if.h>
#endif

#include <string.h>
#if defined ZL_LNX_DENX || defined ZL_LNX_CODESOURCERY
/* Fix an inclusion header issue */
extern char *strtok_r (char *, __const char *, char **);
#endif

/*****************   DEFINES     **********************************************/
/* Amount in Hz by which each consecutive stream frequency differs */
#define ZL303XX_FREQ_ADJ_FACTOR  8
/* Divisor for high packet rates */
#define ZL303XX_PPS_DIVISOR      16

#if !defined TEN_e3
#define TEN_e3 1000
#endif

/*****************************************************************************************/
/*                                                                                       */
/*                                    ! ! ! N O T E ! !                                  */
/*                                                                                       */
/* The following defines are copied and pasted from xxxTopInternal.h in the zl3030x code */
/* Any changes to these defines must be reflected in xxxTopInternal.h and vice-versa     */
/*                                                                                       */
/*****************************************************************************************/

/* There are different formats supported for parameter buffer.
   A master built with version n of the software will generate stream
   parameters in the lowest format possible up to version n to communicate what
   it needs.
   A slave built with version n of the software will support all stream parameter
   formats up to and including version n.
   Subsequent version formats can add fields but can never take fields away */

/* Version 1 bit field definitions: **/
/* Word 0 */
/*    top 4 bits for version field */
#define ZL303XX_STREAM_PARAMS_TRANSFER_VERSION_V1 1
#define ZL303XX_STRMPRMS_VERSION_MASK  (Uint32T)0x0f
#define ZL303XX_STRMPRMS_VERSION_SHIFT  (Uint32T)28
/*    bottom 8 bits for stream ID field */
#define ZL303XX_STRMPRMS_STREAM_ID_MASK  (Uint32T)0xFF
#define ZL303XX_STRMPRMS_STREAM_ID_SHIFT  (Uint32T)0

/* Word 1 */
/*    bottom 9 bits for packet size field */
#define ZL303XX_STRMPRMS_PKT_SIZE_MASK_V1  (Uint32T)0x1FF
#define ZL303XX_STRMPRMS_PKT_SIZE_SHIFT_V1  (Uint32T)0
#define ZL303XX_STRMPRMS_MIN_PKT_SIZE_V1           1  /* Minimum valid packet is one byte */
#define ZL303XX_STRMPRMS_MAX_PKT_SIZE_V1        1500

/* Word 2 */
/*    bottom 15 bits for packet gen freq field */
#define ZL303XX_STRMPRMS_PKT_FRQ_MASK_V1  (Uint32T)0x7FFF
#define ZL303XX_STRMPRMS_PKT_FRQ_SHIFT_V1  (Uint32T)0
#define ZL303XX_STRMPRMS_MIN_PKT_GEN_FREQ_V1     2
#define ZL303XX_STRMPRMS_MAX_PKT_GEN_FREQ_V1 16000

/* Word 3 */
/*    bottom 26 bits for clock freq field */
#define ZL303XX_STRMPRMS_CLK_FRQ_MASK  (Uint32T)0x3FFFFFF
#define ZL303XX_STRMPRMS_CLK_FRQ_SHIFT  (Uint32T)0

/* Word 4 */
/*    all bits reserved */

/* Version 2 bit field definitions:
   (in addition to version 1 fields) **/
/* Word 0 */
/*    top 4 bits for version field */
#define ZL303XX_STREAM_PARAMS_TRANSFER_VERSION_V2 2

/* Word 1 */
/*    bottom 11 bits for packet size field */
#define ZL303XX_STRMPRMS_PKT_SIZE_MASK_V2  (Uint32T)0x7FF
#define ZL303XX_STRMPRMS_PKT_SIZE_SHIFT_V2  (Uint32T)0
#define ZL303XX_STRMPRMS_MIN_PKT_SIZE_V2           1  /* Minimum valid packet is one byte */
#define ZL303XX_STRMPRMS_MAX_PKT_SIZE_V2        1514  /* Maximum packet size, excluding CRC */

/* Word 2 */
/* The packet gen freq field needs special care because a version 1 slave will attempt to
   interpret the value using the version 1 field size. To avoid this the field is split into
   two in version 2 and the high bit from the original field is forced to a one. This will
   cause a version 1 slave to issue an out of range error. */
/*    bits 13:0 for packet gen freq low field */
#define ZL303XX_STRMPRMS_PKT_FRQ_LO_MASK_V2  (Uint32T)0x3FFF
#define ZL303XX_STRMPRMS_PKT_FRQ_LO_SHIFT_V2  (Uint32T)0
/*    bit 14 forced to '1' */
#define ZL303XX_STRMPRMS_PKT_FRQ_FORCE_BIT_MASK_V2  (Uint32T)0x1
#define ZL303XX_STRMPRMS_PKT_FRQ_FORCE_BIT_SHIFT_V2  (Uint32T)14
/*    bits 24:15 for packet gen freq hi field */
#define ZL303XX_STRMPRMS_PKT_FRQ_HI_MASK_V2  (Uint32T)0x3FF
#define ZL303XX_STRMPRMS_PKT_FRQ_HI_SHIFT_V2  (Uint32T)15
#define ZL303XX_STRMPRMS_MIN_PKT_GEN_FREQ_V2        2
#define ZL303XX_STRMPRMS_MAX_PKT_GEN_FREQ_V2 12145000

/* Word 3 */
/*    Same as for version 1 */
/* Word 4 */
/*    all bits reserved (same as for version 1) */

#define MAX_NUM_IPV6_STREAMS 8
#define ZL303XX_NUM_VLAN_TAGS  2  /* Support single- or double-stacked VLAN. */

#if defined ZLS30390_INCLUDED
#define ZL303XX_PKT_HDR_IPV6_ADDR_LEN           (Uint16T)16
#endif

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/* Boolean set once the device is configured and prevents modification to certain key parameters */
zl303xx_BooleanE zl303xx_ClockConfigDone = ZL303XX_FALSE;
zl303xx_BooleanE zl303xx_StreamConfigDone[ZL303XX_PTP_NUM_STREAMS_MAX] = {ZL303XX_FALSE};
Uint8T zl303xxdividePpsInc = 1;

zl303xx_PtpStreamHandleT zl303xx_CurrentClockStream = 0;
zl303xx_BooleanE zl303xx_NoTimestamps = ZL303XX_FALSE;
zl303xx_BooleanE zl303xx_UsePwSeqNum = ZL303XX_FALSE;

zl303xx_BooleanE zl303xx_UseFollowUp = ZL303XX_FALSE;
zl303xx_BooleanE zl303xx_UseSWTimestamps = ZL303XX_FALSE;
Uint8T         zl303xx_DomainNumber = ZL303XX_PTP_DEFAULT_CLOCK_DOMAIN_NUM;
Uint8T         zl303xx_Priority1 = ZL303XX_PTP_DEFAULT_CLOCK_PRI_1;
Uint8T         zl303xx_Priority2 = ZL303XX_PTP_DEFAULT_CLOCK_PRI_2;
zl303xx_BooleanE zl303xx_UseBMC = ZL303XX_FALSE;
zl303xx_BooleanE zl303xx_UseAcceptableMaster = ZL303XX_FALSE;
zl303xx_BooleanE zl303xx_UseAcceptableSlave = ZL303XX_FALSE;
zl303xx_BooleanE zl303xx_GrantDefault = !(ZL303XX_PTP_DEFAULT_PORT_DENY_UNICAST_REQUESTS);
zl303xx_BooleanE zl303xx_UseUniNeg = ZL303XX_FALSE;
zl303xx_BooleanE zl303xx_UseMixMultiUnicast = ZL303XX_FALSE;
zl303xx_BooleanE zl303xx_UnicastStackedTlvEn = ZL303XX_FALSE;
zl303xx_BooleanE zl303xx_BmcaRevertiveEn = ZL303XX_PTP_DEFAULT_CLOCK_REVERTIVE_EN;
zl303xx_BooleanE zl303xx_UseBoundaryFlag = ZL303XX_FALSE;
zl303xx_BooleanE zl303xx_VerifyChecksum = ZL303XX_FALSE;

Uint8T  Zl303xx_AnnounceReceiptTimeout = ZL303XX_PTP_DEFAULT_STREAM_ANNC_TIMEOUT;   /* 3 x Announce Interval */
Uint32T Zl303xx_UnicastNegDuration     = ZL303XX_PTP_DEFAULT_STREAM_UNI_NEG_DUR;    /* 300 seconds */
Uint32T Zl303xx_UnicastNegRenew        = ZL303XX_PTP_DEFAULT_STREAM_UNI_NEG_RENEW;  /* 3 x Query Interval */
zl303xx_BooleanE zl303xx_EvtSquelchEn    = ZL303XX_PTP_DEFAULT_CLOCK_EVT_SQUELCH_EN;
Uint32T zl303xx_WaitToRestore          = ZL303XX_PTP_DEFAULT_CLOCK_WTR;
Uint32T zl303xx_QLHoldOff              = ZL303XX_PTP_DEFAULT_CLOCK_QL_HOLD_OFF;

Uint16T zl303xx_AllowUnicastNodeNumInMixedMode = 32;

zl303xx_PtpVersionE zl303xx_PtpVersion = ZL303XX_PTP_VERSION_2;
Uint8T zl303xx_DefaultClockClass = ZL303XX_PTP_DEFAULT_CLOCK_CLASS;
Uint8T zl303xx_DefaultClockAccuracy = ZL303XX_PTP_DEFAULT_CLOCK_ACCURACY;

Sint16T  zl303xx_PktInterval[ZL303XX_PTP_NUM_STREAMS_MAX] = {0};
Uint16T  zl303xx_PseudoWireField[ZL303XX_PTP_NUM_STREAMS_MAX] = {ZL303XX_DEFAULT_PW_FIELD};
Uint32T  zl303xx_MplsLabel[ZL303XX_PTP_NUM_STREAMS_MAX] = {ZL303XX_DEFAULT_MPLS_LABEL};
Uint32T  zl303xx_VlanTag[ZL303XX_NUM_VLAN_TAGS][ZL303XX_PTP_NUM_STREAMS_MAX] = {{0}};
Uint32T  zl303xx_InitialMplsLabel[ZL303XX_PTP_NUM_STREAMS_MAX] = {ZL303XX_DEFAULT_INITIAL_MPLS_LABEL};

char     zl303xx_SrcIpAddress[ZL303XX_PKT_IP4_DOT_FORMAT_LENGTH];
char     zl303xx_DestIpAddress[ZL303XX_PTP_NUM_STREAMS_MAX][ZL303XX_PKT_IP4_DOT_FORMAT_LENGTH];
Uint8T   zl303xx_SrcAddrIPv6[MAX_NUM_IPV6_STREAMS][ZL303XX_PKT_HDR_IPV6_ADDR_LEN];
Uint8T   zl303xx_DestAddrIPv6[MAX_NUM_IPV6_STREAMS][ZL303XX_PKT_HDR_IPV6_ADDR_LEN];

Uint16T  zl303xx_UdpEvent = DEFAULT_PTP_UDP_EVENT_PORT;
Uint16T  zl303xx_UdpGeneral = DEFAULT_PTP_UDP_GENERAL_PORT;
Sint16T  zl303xx_Pps[ZL303XX_PTP_NUM_STREAMS_MAX] = {ZL303XX_DEFAULT_PPS};
Sint16T  zl303xx_DelayReqPps[ZL303XX_PTP_NUM_STREAMS_MAX] = {ZL303XX_DEFAULT_DELAY_PPS};
Sint16T  zl303xx_AnnouncePps[ZL303XX_PTP_NUM_STREAMS_MAX] = {-2};   /* 1 packet every 2 seconds; not 2 PPS. */
Uint16T  zl303xx_PktSize[ZL303XX_PTP_NUM_STREAMS_MAX] = {ZL303XX_PACKET_MIN_PKT_SIZE};
zl303xx_BooleanE zl303xx_Apr1HzDisabled[ZL303XX_PTP_NUM_STREAMS_MAX] = {ZL303XX_TRUE};

/* String to hold the IP addresses provided by the user */
char zl303xx_DestIpAddrStr[ZL303XX_PKT_IPV6_STR_LENGTH] = {0};
char zl303xx_SrcIpAddrStr[ZL303XX_PKT_IPV6_STR_LENGTH] = {0};

Uint8T zl303xx_TelecomMasterPriority = 255;
zl303xx_BooleanE zl303xx_UnicastMasterReqAnnc       = ZL303XX_FALSE;
zl303xx_BooleanE zl303xx_UnicastMasterReqSync       = ZL303XX_FALSE;
zl303xx_BooleanE zl303xx_UnicastMasterReqDelayResp  = ZL303XX_FALSE;
zl303xx_BooleanE zl303xx_UnicastMasterReqPdelayResp = ZL303XX_FALSE;
zl303xx_BooleanE zl303xx_UnicastMasterReqTiming     = ZL303XX_FALSE;

#endif

#if defined ZLS30390_INCLUDED
zl303xx_ProtocolTypesE zl303xx_ProtocolType = ZL303XX_ETH_IPV4_UDP_PTPV2;
#endif


#if defined ZLS30341_INCLUDED
Uint32T zl303xx_Log2SysInterruptPeriod = ZL303XX_DEFAULT_LOG2_SYS_INTERRUPT_PERIOD;
#endif

#if defined OS_LINUX
Uint8T zl303xx_CliPriority = OS_TASK_MAX_PRIORITY;
#endif

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXTERN FUNCTION DEFINITIONS   **************************/
#if defined ZLS30390_INCLUDED

extern zlStatusE zl303xx_SetPtpProfile(zl303xx_PtpProfileE profile);
extern zl303xx_PtpProfileE zl303xx_GetPtpProfile(void);
#endif

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/
#if defined ZLS30390_INCLUDED
/* zl303xx_SetClockConfigComplete */
/**
   Set Clock Config Complete flag

  Parameters:
      None

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetClockConfigComplete()
{
    zl303xx_ClockConfigDone = ZL303XX_TRUE;
    return ZL303XX_OK;
}

/* zl303xx_ResetClockConfig */
/**
   Resets Clock Config Complete flag

  Parameters:
      None

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_ResetClockConfig()
{
    zl303xx_ClockConfigDone = ZL303XX_FALSE;
    return ZL303XX_OK;
}

/* zl303xx_ClockConfigComplete */
/**
   Fetch ClockConfigComplete flag

  Parameters:
      None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_ClockConfigComplete(void)
{
    return zl303xx_ClockConfigDone;
}

/* zl303xx_SetCurrentClockStream */
/**
   Set the current clockstream for configuration

  Parameters:
   [in]   clockStreamHandle

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetCurrentClockStream(zl303xx_PtpStreamHandleT clockStreamHandle)
{
    zl303xx_CurrentClockStream = clockStreamHandle;
    return ZL303XX_OK;
}

/* zl303xx_GetCurrentClockStream */
/**
   Get the current clockstream

  Parameters:
      None

  Return Value:  zl303xx_PtpStreamHandleT

*******************************************************************************/
zl303xx_PtpStreamHandleT zl303xx_GetCurrentClockStream(void)
{
    return zl303xx_CurrentClockStream;
}

/* zl303xx_SetStreamConfigComplete */
/**
   Set the stream config complete flag

  Parameters:
   [in]   clockStreamHandle

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetStreamConfigComplete(const zl303xx_PtpStreamHandleT clockStreamHandle)
{
    if (clockStreamHandle >= ZL303XX_PTP_NUM_STREAMS_MAX)
    {
       return ZL303XX_PARAMETER_INVALID;
    }

    zl303xx_StreamConfigDone[clockStreamHandle] = ZL303XX_TRUE;
    return ZL303XX_OK;
}

#endif

#if defined ZLS30390_INCLUDED
/* zl303xx_SetProtocolType */
/**
   Set the protocol use by the software

  Parameters:
   [in]   protocolType

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetProtocolType(zl303xx_ProtocolTypesE protocolType)
{
    zl303xx_ProtocolType = protocolType;

    /* This boolean is set automatically when the protocol is selected. If timestamps are not used the period
     * of the packets is used to determine transit times. */
    if ((protocolType == ZL303XX_ETH_IPV4_UDP_PW) ||
        (protocolType == ZL303XX_ETH_MPLS_PW) ||
        (protocolType == ZL303XX_ETH_MPLS_MPLS_PW))
    {
        zl303xx_NoTimestamps = ZL303XX_TRUE;
        zl303xx_UsePwSeqNum = ZL303XX_TRUE;
    }

    return ZL303XX_OK;
}

/* zl303xx_GetProtocolType */
/**
   Fetch the protocol stack used by the system

  Parameters:
      None

  Return Value:  zl303xx_ProtocolTypesE

*******************************************************************************/
zl303xx_ProtocolTypesE zl303xx_GetProtocolType(void)
{
    return zl303xx_ProtocolType;
}

/* zl303xx_IsVlanProtocolType */
/**
   Return TRUE if the protocol is VLAN based.

  Parameters:
      None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_IsVlanProtocolType()
{
    return ((zl303xx_ProtocolType == ZL303XX_ETH_VLAN_IPV4_UDP_PTPV2) ||
            (zl303xx_ProtocolType == ZL303XX_ETH_VLAN_VLAN_IPV4_UDP_PTPV2) ||
            (zl303xx_ProtocolType == ZL303XX_ETH_VLAN_PTPV2));
}

/* zl303xx_IsMplsProtocolType */
/**
   Return TRUE if the protocol is MPLS based.

  Parameters:
      None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_IsMplsProtocolType()
{
    return ((zl303xx_ProtocolType == ZL303XX_ETH_MPLS_IPV4_UDP_PTPV2) ||
            (zl303xx_ProtocolType == ZL303XX_ETH_MPLS_PW) ||
            (zl303xx_ProtocolType == ZL303XX_ETH_MPLS_MPLS_PW));
}

/* zl303xx_IsRawProtocolType */
/**
   Return TRUE if the protocol requires a raw socket.

  Parameters:
      None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_IsRawProtocolType(void)
{
   return (zl303xx_ProtocolType >= ZL303XX_PROTOCOL_RAW_PTP_START) ? ZL303XX_TRUE : ZL303XX_FALSE;
}

/* zl303xx_IsRawPtpProtocolType */
/**
   Returns TRUE if the protocol uses PTP AND requires a raw socket.

  Parameters:
      None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_IsRawPtpProtocolType(void)
{
   return ((zl303xx_ProtocolType >= ZL303XX_PROTOCOL_RAW_PTP_START) &&
           (zl303xx_ProtocolType <= ZL303XX_PROTOCOL_RAW_PTP_END))
              ? ZL303XX_TRUE
              : ZL303XX_FALSE;
}

/* zl303xx_IsIPv4SktProtocol */
/**
   Returns TRUE if the protocol has an IPv4 header and requires a datagram
   socket.

  Parameters:
   [in]   protocol  The protocol type to check.

  Return Value:
     ZL303XX_TRUE   protocol uses an IPv4 datagram socket.
     ZL303XX_FALSE  protocol does not use an IPv4 datagram socket.

*******************************************************************************/
zl303xx_BooleanE zl303xx_IsIPv4SktProtocol(zl303xx_ProtocolTypesE protocol)
{
   return (protocol < ZL303XX_PROTOCOL_RAW_PTP_START) &&
          !zl303xx_IsIPv6SktProtocol(protocol);
}

/* zl303xx_IsIPv6SktProtocol */
/**
   Returns TRUE if the protocol has an IPv6 header and requires a datagram
   socket.

  Parameters:
   [in]   protocol  The protocol type to check.

  Return Value:
     ZL303XX_TRUE   protocol uses an IPv6 datagram socket.
     ZL303XX_FALSE  protocol does not use an IPv6 datagram socket.

*******************************************************************************/
zl303xx_BooleanE zl303xx_IsIPv6SktProtocol(zl303xx_ProtocolTypesE protocol)
{
   return (protocol == ZL303XX_ETH_IPV6_UDP_PTPV2_SKT) ||
          (protocol == ZL303XX_ETH_VLAN_IPV6_UDP_PTPV2_SKT);
}

/* zl303xx_IsIPv6ProtocolType */
/**
   Returns TRUE if the protocol has an IPv6 header and requires a raw socket.

  Parameters:
      None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_IsIPv6ProtocolType(zl303xx_ProtocolTypesE protocol)
{
   return ((protocol == ZL303XX_ETH_IPV6_UDP_PTPV2) ||
           (protocol == ZL303XX_ETH_CUST_ETH_IPV6_UDP_PTPV2))
             ? ZL303XX_TRUE
             : ZL303XX_FALSE;
}

/* zl303xx_IsEthPtpProtocolType */
/**
   Returns TRUE if the protocol uses PTPv2 over Ethernet or VLAN/Ethernet and
   requires a raw socket.

  Parameters:
      None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_IsEthPtpProtocolType(zl303xx_ProtocolTypesE protocol)
{
   return ((protocol == ZL303XX_ETH_PTPV2) ||
           (protocol == ZL303XX_ETH_CUST_ETH_PTPV2) ||
           (protocol == ZL303XX_ETH_VLAN_PTPV2))
             ? ZL303XX_TRUE
             : ZL303XX_FALSE;
}
#endif

#if defined ZLS30390_INCLUDED
/* VLAN - Pack VLAN field if the protocol indicates  - Pre RTP fields */
/* the VLAN header is 32 bits -  16=tagged protocol id, 3=priority, 1=canonical format indicator, 12=vlan id */
/* Since we do not have a VLAN stack we must combine the Ethertype and the VLAN tag into one blob */
/* In our case the first 16 bits (TPI) will be inserted by the RAW output as the Ethertype. We will juxtapose
 * the next 16 bits of the VLAN tag to simulate an actual tag on the stack. Because the RAW Ethernet port will include the VLAN
 * Ethertype, the VLAN tag should be set to the lower 16 bits of the tag (2nd half) and the Ethertype of the data portion */
/**********************************************************************************************************************/
/* VLAN Tag
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |            TPI                | PRI |C|      VLAN ID          |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * Our VLAN Tag
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | PRI |C|      VLAN ID          |    Timimng Packet Ethertype   |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * Normal VLAN stack
 *
 * |DA|SA|VLAN|Ethertype|DATA ... |FCS|
 *
 * Our implementation
 *
 * |DA|SA|Ethertype=VLAN|Flipped VLAN|DATA ... |FCS|
 *
 */

/* zl303xx_SetVlanTag */
/**
   Set the value of the VLAN tag

  Parameters:
   [in]   stackPos           0 is the outer-most VLAN tag.
   [in]   clockStreamHandle
   [in]   vlanTag

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetVlanTag(Uint8T stackPos,
                             const clockStreamHandleT clockStreamHandle,
                             const Uint32T vlanTag)
{
   if ((stackPos >= ZL303XX_NUM_VLAN_TAGS) ||
       (clockStreamHandle >= ZL303XX_PTP_NUM_STREAMS_MAX))
   {
      return ZL303XX_PARAMETER_INVALID;
   }

   /* store the 32 bit VLAN */
   zl303xx_VlanTag[stackPos][clockStreamHandle] = vlanTag;
   return ZL303XX_OK;
}

/* zl303xx_GetVlanTag */
/**
   Get the value of the VLAN tag

  Parameters:
   [in]   stackPos           0 is the outer-most VLAN tag.
   [in]   clockStreamHandle

  Return Value:  Uint32T

*******************************************************************************/
Uint32T zl303xx_GetVlanTag(Uint8T stackPos, const clockStreamHandleT clockStreamHandle)
{
   if ((stackPos >= ZL303XX_NUM_VLAN_TAGS) ||
       (clockStreamHandle >= ZL303XX_PTP_NUM_STREAMS_MAX))
   {
      return 0;
   }

   return zl303xx_VlanTag[stackPos][clockStreamHandle];
}
#endif

#if defined ZLS30390_INCLUDED
/* zl303xx_SetPktSrcIPv6 */
/**
   Sets the source IPv6 address for packets.

  Parameters:
   [in]   clockStreamHandle
   [in]   srcAddr

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetPktSrcIPv6(const clockStreamHandleT clockStreamHandle,
                                const Uint8T *srcAddr)
{
   /* Check input parameters. */
   if (clockStreamHandle >= MAX_NUM_IPV6_STREAMS)
   {
      return ZL303XX_PARAMETER_INVALID;
   }
   if (srcAddr == NULL)
   {
      return ZL303XX_INVALID_POINTER;
   }

   /* Store the 16-byte source IPv6 address. */
   OS_MEMCPY(zl303xx_SrcAddrIPv6[clockStreamHandle], srcAddr,
          sizeof(zl303xx_SrcAddrIPv6[clockStreamHandle]));
   return ZL303XX_OK;
}

/* zl303xx_GetPktSrcIPv6 */
/**
   Gets the source IPv6 address for packets.

  Parameters:
   [in]   clockStreamHandle

  Return Value:  (const Uint8T *)

*******************************************************************************/
const Uint8T *zl303xx_GetPktSrcIPv6(const clockStreamHandleT clockStreamHandle)
{
   /* Check input parameters. */
   if (clockStreamHandle >= MAX_NUM_IPV6_STREAMS)
   {
      return NULL;
   }

   /* Return the 16-byte source IPv6 address. */
   return zl303xx_SrcAddrIPv6[clockStreamHandle];
}

/* zl303xx_SetPktDestIPv6 */
/**
   Sets the destination IPv6 address for packets.

  Parameters:
   [in]   clockStreamHandle
   [in]   destAddr

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetPktDestIPv6(const clockStreamHandleT clockStreamHandle,
                                 const Uint8T *destAddr)
{
   /* Check input parameters. */
   if (clockStreamHandle >= MAX_NUM_IPV6_STREAMS)
   {
      return ZL303XX_PARAMETER_INVALID;
   }
   if (destAddr == NULL)
   {
      return ZL303XX_INVALID_POINTER;
   }

   /* Store the 16-byte destination IPv6 address. */
   memcpy(zl303xx_DestAddrIPv6[clockStreamHandle], destAddr,
          sizeof(zl303xx_DestAddrIPv6[clockStreamHandle]));
   return ZL303XX_OK;
}

/* zl303xx_GetPktDestIPv6 */
/**
   Gets the destination IPv6 address for packets.

  Parameters:
   [in]   clockStreamHandle

  Return Value:  (const Uint8T *)

*******************************************************************************/
const Uint8T *zl303xx_GetPktDestIPv6(const clockStreamHandleT clockStreamHandle)
{
   /* Check input parameters. */
   if (clockStreamHandle >= MAX_NUM_IPV6_STREAMS)
   {
      return NULL;
   }

   /* Return the 16-byte destination IPv6 address. */
   return zl303xx_DestAddrIPv6[clockStreamHandle];
}

/* zl303xx_GetIPv6ClockStream */
/**
   Gets the clock stream handle based on an IPv6 address.

  Parameters:
   [in]   clockStreamHandle
   [out]  srcAddr

  Return Value:  zlStatusE

*******************************************************************************/
clockStreamHandleT zl303xx_GetIPv6ClockStream(const Uint8T *srcAddr)
{
   clockStreamHandleT count;

   if (srcAddr != NULL)
   {
      for (count = 0; count < ZL303XX_ARRAY_SIZE(zl303xx_DestAddrIPv6); count++)
      {
         if (memcmp(zl303xx_DestAddrIPv6[count], srcAddr, sizeof(zl303xx_DestAddrIPv6[count])) == 0)
         {
            return count;
         }
      }
   }

   return (clockStreamHandleT)ZL303XX_INVALID;
}
#endif

#if defined ZLS30390_INCLUDED
/* zl303xx_SetUdpEvent */
/**
   Set the UDP event socket port

  Parameters:
   [in]   udpPort

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetUdpEvent(const Uint16T udpPort)
{
    zl303xx_UdpEvent = udpPort;
    return ZL303XX_OK;
}

/* zl303xx_GetUdpEvent */
/**
   Get the UDP event socket port

  Parameters:
      None

  Return Value:  Uint16T

*******************************************************************************/
Uint16T zl303xx_GetUdpEvent(void)
{
    return zl303xx_UdpEvent;
}

/* zl303xx_SetUdpGeneral */
/**
   Set the UDP general socket port

  Parameters:
   [in]   udpPort

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetUdpGeneral(const Uint16T udpPort)
{
    zl303xx_UdpGeneral = udpPort;
    return ZL303XX_OK;
}

/* zl303xx_GetUdpGeneral */
/**
   Get the UDP general socket port

  Parameters:
      None

  Return Value:  Uint16T

*******************************************************************************/
Uint16T zl303xx_GetUdpGeneral(void)
{
    return zl303xx_UdpGeneral;
}
#endif

#if defined ZLS30390_INCLUDED && (defined _ZL303XX_ZLE1588_BOARD || defined _ZL303XX_ZLE30360_BOARD )
/* zl303xx_StreamConfigComplete */
/**
   Fetch the stream config complete flag

  Parameters:
   [in]   clockStreamHandle

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_StreamConfigComplete(const zl303xx_PtpStreamHandleT clockStreamHandle)
{

    if (clockStreamHandle >= ZL303XX_PTP_NUM_STREAMS_MAX)
    {
        return ZL303XX_FALSE;
    }
    return zl303xx_StreamConfigDone[clockStreamHandle];
}

/* zl303xx_SetPktDest */
/**
   Set the destination IP for the packets

  Parameters:
   [in]   streamHandle
   [in]   destIP

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetPktDest(const zl303xx_PtpStreamHandleT streamHandle, char *destIP)
{
   zl303xx_RemoteAddrT ipAddress;

    if (streamHandle >= ZL303XX_ARRAY_SIZE(zl303xx_DestIpAddress))
    {
        return ZL303XX_PARAMETER_INVALID;
    }
    if (zl303xx_StreamConfigComplete(streamHandle))
    {
        return ZL303XX_INIT_ERROR;
    }
    /* use the function to convert an IP address to a number to check its validity */
    if (inet_aton(destIP, &ipAddress) == OS_ERROR)
    {
        ZL303XX_TRACE_ALWAYS("Destination IP Address=%s = 0x%x is not properly formatted.", destIP, ipAddress.s_addr, 0,0,0,0);
        return ZL303XX_PARAMETER_INVALID;
    }

    memset(zl303xx_DestIpAddress[streamHandle], 0, sizeof(zl303xx_DestIpAddress[streamHandle]));
    strncpy(zl303xx_DestIpAddress[streamHandle], destIP, sizeof(zl303xx_DestIpAddress[streamHandle]) - 1);
    return ZL303XX_OK;
}

/* zl303xx_GetPktDest */
/**
   Get the destination IP  for the packets

  Parameters:
   [in]   clockStreamHandle
   [out]  destIP

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_GetPktDest(const zl303xx_PtpStreamHandleT clockStreamHandle, char *destIP)
{
    if (clockStreamHandle >= ZL303XX_PTP_NUM_STREAMS_MAX)
    {
        return ZL303XX_PARAMETER_INVALID;
    }
    if (destIP == NULL)
    {
        return ZL303XX_INVALID_POINTER;
    }

    strncpy(destIP, zl303xx_DestIpAddress[clockStreamHandle], ZL303XX_PKT_IP4_DOT_FORMAT_LENGTH);
    return ZL303XX_OK;
}

/* zl303xx_InitDefaultPps */
/**
   Set the DEFAULT Sync PPS rates

  Return Value:  N/A

*******************************************************************************/
void zl303xx_InitDefaultPps(void);
void zl303xx_InitDefaultPps(void)
{
    Uint8T indx;

    for (indx = 0; indx < ZL303XX_PTP_NUM_STREAMS_MAX; indx++ )
    {
        zl303xx_Pps[indx] = ZL303XX_DEFAULT_PPS;
    }
}

/* zl303xx_SetPps */
/**
   Set the PPS rate

  Parameters:
   [in]   clockStreamHandle
   [in]   ppsRate

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetPps(const zl303xx_PtpStreamHandleT clockStreamHandle, const Sint16T ppsRate)
{
    if (clockStreamHandle >= ZL303XX_PTP_NUM_STREAMS_MAX)
    {
        return ZL303XX_PARAMETER_INVALID;
    }
    if (zl303xx_StreamConfigComplete(clockStreamHandle))
    {
        printf("zl303xx_SetPps failed, stream config not completed %d\n", clockStreamHandle);
        return ZL303XX_INIT_ERROR;
    }

    /* if any stream has a packet rate greater than ZL303XX_MAX_PKT_PER_SEC but
     * STACK_BYPASS is turned on and the rate is divisible by ZL303XX_PPS_DIVISOR, then the
     * zl303xxdividePpsInc will be set to ZL303XX_PPS_DIVISOR meaning all packets will be divided. Since
     * packets coming in are not associated with a stream then all must be handled this way */
    if (ppsRate > (Sint16T)ZL303XX_MAX_PKT_PER_SEC)
    {
        if (((ppsRate / 4) > (Sint16T)ZL303XX_MAX_PKT_PER_SEC) || (ppsRate % ZL303XX_PPS_DIVISOR != 0))
        {
            ZL303XX_TRACE_ALWAYS("PPS rate cannot surpass %d. Rate out of bounds[%d]. ",
                                 ZL303XX_MAX_PKT_PER_SEC, ppsRate, 0,0,0,0);
            return ZL303XX_PARAMETER_INVALID;
        }

        ZL303XX_TRACE_ALWAYS("PPS rate will be divided by a factor of %d. PPS: %lu  Effective PPS: %lu",
                             ZL303XX_PPS_DIVISOR, ppsRate, (ppsRate / ZL303XX_PPS_DIVISOR), 0,0,0);
        zl303xxdividePpsInc = ZL303XX_PPS_DIVISOR;
    }
    /* Negative rates of -2, -4, -8, and -16 are equivalent to less than 1 PPS
     * -2 = 1 packet every 2 seconds,
     * -4 = 1 packet every 4 seconds, etc. */
    else if (ppsRate < 0)
    {
        if ((ppsRate != -2) && (ppsRate != -4) && (ppsRate != -8) && (ppsRate != -16))
        {
            ZL303XX_TRACE_ALWAYS("Negative PPS rates must be -2, -4, -8, or -16 [%d].", ppsRate, 0,0,0,0,0);
            return ZL303XX_PARAMETER_INVALID;
        }
    }

    zl303xx_Pps[clockStreamHandle] = ppsRate;
    return ZL303XX_OK;
}

/* zl303xx_GetPps */
/**
   Get the PPS rate

  Parameters:
   [in]   clockStreamHandle

  Return Value:  Sint16T

*******************************************************************************/
Sint16T zl303xx_GetPps(const zl303xx_PtpStreamHandleT clockStreamHandle)
{
    if (clockStreamHandle >= ZL303XX_PTP_NUM_STREAMS_MAX)
    {
        return 0;
    }

    return zl303xx_Pps[clockStreamHandle];
}

/* zl303xx_InitDefaultDRPps */
/**
   Set the DEFAULT DelayRequest PPS rates

  Return Value:  N/A

*******************************************************************************/
void zl303xx_InitDefaultDRPps(void);
void zl303xx_InitDefaultDRPps(void)
{
    Uint8T indx;

    for (indx = 0; indx < ZL303XX_PTP_NUM_STREAMS_MAX; indx++ )
    {
        zl303xx_DelayReqPps[indx] = ZL303XX_DEFAULT_DELAY_PPS;
    }
}

/* zl303xx_SetDelayReqPps */
/**
   Set the PPS rate for PTP delay request msg

  Parameters:
   [in]   clockStreamHandle
   [in]   ppsRate

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetDelayReqPps(const zl303xx_PtpStreamHandleT clockStreamHandle, const Sint16T ppsRate)
{
    if (clockStreamHandle >= ZL303XX_PTP_NUM_STREAMS_MAX)
    {
        return ZL303XX_PARAMETER_INVALID;
    }
    if (zl303xx_StreamConfigComplete(clockStreamHandle))
    {
        return ZL303XX_INIT_ERROR;
    }

    /* if any stream has a packet rate greater than ZL303XX_MAX_PKT_PER_SEC but
     * STACK_BYPASS is turned on and the rate is divisible by ZL303XX_PPS_DIVISOR, then the
     * zl303xxdividePpsInc will be set to ZL303XX_PPS_DIVISOR meaning all packets will be divided. Since
     * packets coming in are not associated with a stream then all must be handled this way */
    if (ppsRate > (Sint16T)ZL303XX_MAX_PKT_PER_SEC)
    {
        if (((ppsRate / 4) > (Sint16T)ZL303XX_MAX_PKT_PER_SEC) || (ppsRate % ZL303XX_PPS_DIVISOR != 0))
        {
            ZL303XX_TRACE_ALWAYS("PPS rate cannot surpass %d. Rate out of bounds[%d]. ",
                                 ZL303XX_MAX_PKT_PER_SEC, ppsRate, 0,0,0,0);
            return ZL303XX_PARAMETER_INVALID;
        }

        ZL303XX_TRACE_ALWAYS("PPS rate will be divided by a factor of %d. PPS: %lu  Effective PPS: %lu",
                             ZL303XX_PPS_DIVISOR, ppsRate, (ppsRate / ZL303XX_PPS_DIVISOR), 0,0,0);
        zl303xxdividePpsInc = ZL303XX_PPS_DIVISOR;
    }
    /* Negative rates of -2, -4, -8, and -16 are equivalent to less than 1 PPS
     * -2 = 1 packet every 2 seconds,
     * -4 = 1 packet every 4 seconds, etc. */
    else if (ppsRate < 0)
    {
        if ((ppsRate != -2) && (ppsRate != -4) && (ppsRate != -8) && (ppsRate != -16))
        {
            ZL303XX_TRACE_ALWAYS("Negative PPS rates must be -2, -4, -8, or -16 [%d].", ppsRate, 0,0,0,0,0);
            return ZL303XX_PARAMETER_INVALID;
        }
    }

    zl303xx_DelayReqPps[clockStreamHandle] = ppsRate;
    return ZL303XX_OK;
}

/* zl303xx_GetDelayReqPps */
/**
   Get the PPS rate of Delay Request msg

  Parameters:
   [in]   clockStreamHandle

  Return Value:  Sint16T

*******************************************************************************/
Sint16T zl303xx_GetDelayReqPps(const zl303xx_PtpStreamHandleT clockStreamHandle)
{
    if (clockStreamHandle >= ZL303XX_PTP_NUM_STREAMS_MAX)
    {
        return 0;
    }

    return zl303xx_DelayReqPps[clockStreamHandle];
}

/* zl303xx_InitAnnouncePps */
/**
   Set the DEFAULT Announce PPS rates

  Return Value:  N/A

*******************************************************************************/
void zl303xx_InitAnnouncePps(void);
void zl303xx_InitAnnouncePps(void)
{
    Uint8T indx;

    for (indx = 0; indx < ZL303XX_PTP_NUM_STREAMS_MAX; indx++ )
    {
        zl303xx_AnnouncePps[indx] = ZL303XX_DEFAULT_ANN_PPS;
    }
}

/* zl303xx_SetAnnouncePps */
/**
   Set the PPS rate for Announce messages

  Parameters:
   [in]   clockStreamHandle
   [in]   ppsRate

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetAnnouncePps(const zl303xx_PtpStreamHandleT clockStreamHandle, const Sint16T ppsRate)
{
   if (clockStreamHandle >= ZL303XX_PTP_NUM_STREAMS_MAX)
   {
      return ZL303XX_PARAMETER_INVALID;
   }
   if (zl303xx_StreamConfigComplete(clockStreamHandle))
   {
      return ZL303XX_INIT_ERROR;
   }

   if (ppsRate > (Sint16T)ZL303XX_MAX_PKT_PER_SEC)
   {
      ZL303XX_TRACE_ALWAYS("PPS rate cannot surpass %d. Rate out of bounds[%d]. ",
                           ZL303XX_MAX_PKT_PER_SEC, ppsRate, 0,0,0,0);

      return ZL303XX_PARAMETER_INVALID;
   }

   /* Negative rates of -2, -4, -8, and -16 are equivalent to less than 1 PPS
    * -2 = 1 packet every 2 seconds,
    * -4 = 1 packet every 4 seconds, etc. */
   if (ppsRate < 0)
   {
      if ((ppsRate != -2) && (ppsRate != -4) && (ppsRate != -8) && (ppsRate != -16))
      {
         ZL303XX_TRACE_ALWAYS("Negative PPS rates must be -2, -4, -8, -16 [%d].",
                              ppsRate, 0,0,0,0,0);
         return ZL303XX_PARAMETER_INVALID;
      }
   }

   zl303xx_AnnouncePps[clockStreamHandle] = ppsRate;
   return ZL303XX_OK;
}

/* zl303xx_GetAnnouncePps */
/**
   Get the PPS rate for Announce messages.

  Parameters:
   [in]   clockStreamHandle

  Return Value:  Sint16T

*******************************************************************************/
Sint16T zl303xx_GetAnnouncePps(const zl303xx_PtpStreamHandleT clockStreamHandle)
{
    if (clockStreamHandle >= ZL303XX_PTP_NUM_STREAMS_MAX)
    {
        return 0;
    }

    return zl303xx_AnnouncePps[clockStreamHandle];
}


/* zl303xx_SetPktInterval */
/**
   DEPRECATED. Set the PPS rate

  Parameters:
   [in]   clockStreamHandle
   [in]   ppsRate

  Return Value:  zlStatusE

  Notes:   DEPRECATED. Use zl303xx_SetPps() as alternative

*******************************************************************************/
zlStatusE zl303xx_SetPktInterval(const zl303xx_PtpStreamHandleT clockStreamHandle, const Sint16T pktInterval)
{
    if (clockStreamHandle >= ZL303XX_PTP_NUM_STREAMS_MAX)
    {
       return ZL303XX_PARAMETER_INVALID;
    }

    zl303xx_PktInterval[clockStreamHandle] = pktInterval;

    ZL303XX_TRACE_ALWAYS("WARNING: zl303xx_SetPktInterval() is deprecated!",0,0,0,0,0,0);

    return ZL303XX_OK;
}

/* zl303xx_GetPktInterval */
/**
   DEPRECATED. Get the Packet Interval

  Parameters:
   [in]   clockStreamHandle

  Return Value:  Sint16T

  Notes:   DEPRECATED. Use zl303xx_GetPps() as alternative

*******************************************************************************/
Sint16T zl303xx_GetPktInterval(const zl303xx_PtpStreamHandleT clockStreamHandle)
{
    if (clockStreamHandle >= ZL303XX_PTP_NUM_STREAMS_MAX)
    {
        return 0;
    }

    ZL303XX_TRACE_ALWAYS("WARNING: zl303xx_GetPktInterval() is deprecated!",0,0,0,0,0,0);

    return zl303xx_PktInterval[clockStreamHandle];
}

/* zl303xx_GetDividePpsInc */
/**
   Get the increment value signifying if the PPS rate is divided by a factor of
   16. PPS rates can be divided if they are greater than the usual maximum rate
   but equal or less than 8192 and divisible by 16. This value is set for all
   streams since the idea is to reduce timing messages for high packet rates as
   soon as they appear at the slave. At that point the intended stream is not
   known. All streams must be treated equally.

  Parameters:
      None

  Return Value:  Uint8T

*******************************************************************************/
Uint8T zl303xx_GetDividePpsInc(void)
{
    return zl303xxdividePpsInc;
}


/* zl303xx_SetPktSize */
/**
   Set the packet size

  Parameters:
   [in]   clockStreamHandle
   [in]   pktSize

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetPktSize(const zl303xx_PtpStreamHandleT clockStreamHandle, const Uint16T pktSize)
{
    if (clockStreamHandle >= ZL303XX_PTP_NUM_STREAMS_MAX)
    {
        return ZL303XX_PARAMETER_INVALID;
    }
    if (zl303xx_StreamConfigComplete(clockStreamHandle))
    {
       return ZL303XX_INIT_ERROR;
    }

    /* This limits the packet size quite a bit. Since the buffer is embedded in zl303xx_PtpClockS */
    if ((pktSize < ZL303XX_PACKET_MIN_PKT_SIZE) || (pktSize > ZL303XX_PACKET_MAX_PKT_SIZE_STD))
    {
        ZL303XX_TRACE_ALWAYS("Packet size out of range[%d < %d < %d] ",
                             ZL303XX_PACKET_MIN_PKT_SIZE, pktSize, ZL303XX_MAX_TIMING_PACKET,0,0,0);
        return ZL303XX_PARAMETER_INVALID;
    }

    zl303xx_PktSize[clockStreamHandle] = pktSize;

    return ZL303XX_OK;
}

/* zl303xx_GetPktSize */
/**
   Get the packet size

  Parameters:
   [in]   clockStreamHandle

  Return Value:  Uint16T

*******************************************************************************/
Uint16T zl303xx_GetPktSize(const zl303xx_PtpStreamHandleT clockStreamHandle)
{
    if (clockStreamHandle >= ZL303XX_PTP_NUM_STREAMS_MAX)
    {
        return 0;
    }
    return zl303xx_PktSize[clockStreamHandle];
}

/* zl303xx_SetFollowUpFlag */
/**
   Set the Use Follow Up messages flag

  Parameters:
   [in]   followUpFlag

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetFollowUpFlag(zl303xx_BooleanE followUpFlag)
{
    zl303xx_UseFollowUp = followUpFlag;
    return ZL303XX_OK;
}

/* zl303xx_GetFollowUpFlag */
/**
   Get the Use Follow Up messages flag

  Parameters:
      None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetFollowUpFlag(void)
{
    return zl303xx_UseFollowUp;
}

/* zl303xx_SetSWTimestampFlag */
/**
   Set the Use Software Timestamps flag

  Parameters:
   [in]   swTsFlag

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetSWTimestampFlag(zl303xx_BooleanE swTsFlag)
{
    zl303xx_UseSWTimestamps = swTsFlag;
    return ZL303XX_OK;
}

/* zl303xx_GetSWTimestampFlag */
/**
   Get the Use Software Timestamps flag

  Parameters:
      None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetSWTimestampFlag(void)
{
    return zl303xx_UseSWTimestamps;
}

/* zl303xx_SetDomainNumber */
/**
   Sets the domain number for the next created PTP clock.

  Parameters:
   [in]   domainNumber   Domain number to set.

  Return Value:   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetDomainNumber(Uint8T domainNumber)
{
   zl303xx_DomainNumber = domainNumber;
   return ZL303XX_OK;
}

/* zl303xx_GetDomainNumber */
/**
   Returns the previously set domain number.

  Parameters:
      None

  Return Value:  Uint8T  Domain number.

*******************************************************************************/
Uint8T zl303xx_GetDomainNumber(void)
{
   return zl303xx_DomainNumber;
}

/* zl303xx_SetPriority1 */
/**
   Sets the priority1 value for the next created PTP clock.

  Parameters:
   [in]  p1    priority1 value to set.

  Return Value:   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetPriority1(Uint8T p1)
{
   zl303xx_Priority1 = p1;
   return ZL303XX_OK;
}

/* zl303xx_GetPriority1 */
/**
   Returns the previously set priority1 value.

  Parameters:
      None

  Return Value:  Uint8T  priority1.

*******************************************************************************/
Uint8T zl303xx_GetPriority1(void)
{
   return zl303xx_Priority1;
}

/* zl303xx_SetPriority2 */
/**
   Sets the priority2 value for the next created PTP clock.

  Parameters:
   [in]  p2    priority2 value to set.

  Return Value:   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetPriority2(Uint8T p2)
{
   zl303xx_Priority2 = p2;
   return ZL303XX_OK;
}

/* zl303xx_GetPriority2 */
/**
   Returns the previously set priority2 value.

  Parameters:
      None

  Return Value:  Uint8T  priority2.

*******************************************************************************/
Uint8T zl303xx_GetPriority2(void)
{
   return zl303xx_Priority2;
}

/* zl303xx_SetBmcFlag */
/**
   Set the Use Best Master Clock flag

  Parameters:
   [in]   bmcFlag

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetBmcFlag(zl303xx_BooleanE bmcFlag)
{
    zl303xx_UseBMC = bmcFlag;
    return ZL303XX_OK;
}

/* zl303xx_GetBmcFlag */
/**
   Get the Use BMC flag

  Parameters:
   [in]   None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetBmcFlag(void)
{
    return zl303xx_UseBMC;
}

/* zl303xx_SetAcceptableMasterFlag */
/**
   Set the Use Acceptable Master flag

  Parameters:
   [in]   acceptMasterFlag

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetAcceptableMasterFlag(zl303xx_BooleanE acceptMasterFlag)
{
    zl303xx_UseAcceptableMaster = acceptMasterFlag;
    return ZL303XX_OK;
}

/* zl303xx_GetAcceptableMasterFlag */
/**
   Get the Use Acceptable Master flag

  Parameters:
   [in]   None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetAcceptableMasterFlag(void)
{
    return zl303xx_UseAcceptableMaster;
}

/* zl303xx_SetAcceptableSlaveFlag */
/**
   Sets the Use Acceptable Slave Table flag

  Parameters:
   [in]   acceptSlaveFlag

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetAcceptableSlaveFlag(zl303xx_BooleanE acceptSlaveFlag)
{
    zl303xx_UseAcceptableSlave = acceptSlaveFlag;
    return ZL303XX_OK;
}

/* zl303xx_GetAcceptableSlaveFlag */
/**
   Gets the Use Acceptable Slave Table flag

  Parameters:
      None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetAcceptableSlaveFlag(void)
{
    return zl303xx_UseAcceptableSlave;
}

/* zl303xx_SetUseUniNegFlag */
/**
   Set the Use Unicast Negotiation flag

  Parameters:
   [in]   uniNegFlag

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetUseUniNegFlag(zl303xx_BooleanE uniNegFlag)
{
    zl303xx_UseUniNeg = uniNegFlag;
    return ZL303XX_OK;
}

/* zl303xx_GetUseUniNegFlag */
/**
   Get the Use Unicast Negotiation flag

  Parameters:
   [in]   None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetUseUniNegFlag(void)
{
    return zl303xx_UseUniNeg;
}

/* zl303xx_SetUseMixMultiUnicastFlag */
/**
   Set the Use mix of Multicast and Unicast flag

  Parameters:
   [in]   mixcastFlag

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetUseMixMultiUnicastFlag(zl303xx_BooleanE mixMultiUnicastFlag)
{
    zl303xx_UseMixMultiUnicast = mixMultiUnicastFlag;
    return ZL303XX_OK;
}

/* zl303xx_GetUseMixMultiUnicastFlag */
/**
   Get the Use mix of Multicast and Unicast flag

  Parameters:
   [in]   None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetUseMixMultiUnicastFlag(void)
{
    return zl303xx_UseMixMultiUnicast;
}

/* zl303xx_SetAllowedUnicastNodeNumInMixedMode */
/**
   Set the maximum allowed unicast node number in one multicast group if using
   mixed multicast/unicast.

  Parameters:
   [in]   clientNum

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetAllowedUnicastNodeNumInMixedMode(Uint16T ucastNodeNum)
{
    zl303xx_AllowUnicastNodeNumInMixedMode = ucastNodeNum;
    return ZL303XX_OK;
}

/* zl303xx_GetAllowedUnicastNodeNumInMixedMode */
/**
   Get the maximum allowed unicast node number in one multicast group if using
   mixed multicast/unicast.

  Parameters:
      None

  Return Value:  zlStatusE

*******************************************************************************/
Uint16T zl303xx_GetAllowedUnicastNodeNumInMixedMode(void)
{
    return zl303xx_AllowUnicastNodeNumInMixedMode;
}

/* zl303xx_SetAnnounceReceiptTimeout */
/**
   Set the Announce Receipt Timeout multiplier (# announce intervals).

  Parameters:
   [in]   announceReceiptTimeout  Number of announce intervals before
                                       timeout.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetAnnounceReceiptTimeout(Uint8T announceReceiptTimeout)
{
    Zl303xx_AnnounceReceiptTimeout = announceReceiptTimeout;
    return ZL303XX_OK;
}

/* zl303xx_GetAnnounceReceiptTimeout */
/**
   Gets the Announce Receipt Timeout multiplier (# announce intervals).

  Parameters:
      None

  Return Value:  Uint8T

*******************************************************************************/
Uint8T zl303xx_GetAnnounceReceiptTimeout(void)
{
    return Zl303xx_AnnounceReceiptTimeout;
}

/* zl303xx_SetUnicastNegDuration */
/**
   Sets the Unicast Negotiation Contract Duration in seconds (same for SYNC,
   ANNOUNCE & DELAY-REQ message flows).

  Parameters:
   [in]   contractDuration  Contract duration (in seconds).

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetUnicastNegDuration(Uint32T contractDuration)
{
    Zl303xx_UnicastNegDuration = contractDuration;
    return ZL303XX_OK;
}

/* zl303xx_GetUnicastNegDuration */
/**
   Returns the Unicast Negotiation Contract Duration (in seconds).

  Parameters:
      None

  Return Value:  Uint8T

*******************************************************************************/
Uint32T zl303xx_GetUnicastNegDuration(void)
{
    return Zl303xx_UnicastNegDuration;
}

/* zl303xx_SetUnicastNegRenew */
/**
   Sets the Unicast Negotiation contract pre-renewal in seconds (same for SYNC,
   ANNOUNCE & DELAY-REQ message flows).

  Parameters:
   [in]   renew  Pre-renewal (in seconds).

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetUnicastNegRenew(Uint32T renew)
{
   Zl303xx_UnicastNegRenew = renew;
   return ZL303XX_OK;
}

/* zl303xx_GetUnicastNegRenew */
/**
   Returns the Unicast Negotiation contract pre-renewal in seconds (same for SYNC,
   ANNOUNCE & DELAY-REQ message flows).

  Parameters:
      None

  Return Value:  Uint32T  The pre-renewal value.

*******************************************************************************/
Uint32T zl303xx_GetUnicastNegRenew(void)
{
   return Zl303xx_UnicastNegRenew;
}

/* zl303xx_SetEvtSquelchEn */
/**
   Only applies to Telecom Profile. Sets the flag to determine if the
   ZL303XX_PTP_EVENT_SQUELCH will be generated.

  Parameters:
   [in]   enabled

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetEvtSquelchEn(zl303xx_BooleanE enabled)
{
   zlStatusE status = ZL303XX_CHECK_BOOLEAN(enabled);

   if (status == ZL303XX_OK)
   {
      zl303xx_EvtSquelchEn = enabled;
   }

   return status;
}

/* zl303xx_GetEvtSquelchEn */
/**
   Only applies to Telecom Profile. Returns the flag that determine if the
   ZL303XX_PTP_EVENT_SQUELCH will be generated.

  Parameters:
   [in]   None

  Return Value:  zl303xx_BooleanE   Squelch flag.

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetEvtSquelchEn(void)
{
   return zl303xx_EvtSquelchEn;
}

/* zl303xx_SetWaitToRestore */
/**
   Only applies to Telecom Profile. Sets the Wait to Restore time that must elapse
   for a previously failed masters to be selected by BMCA.

  Parameters:
   [in]   wtrSec   Time in seconds.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetWaitToRestore(Uint32T wtrSec)
{
   zl303xx_WaitToRestore = wtrSec;
   return ZL303XX_OK;
}

/* zl303xx_GetWaitToRestore */
/**
   Only applies to Telecom Profile. Returns the Wait to Restore time.

  Parameters:
      None

  Return Value:  Uint32T  Time in seconds.

*******************************************************************************/
Uint32T zl303xx_GetWaitToRestore(void)
{
   return zl303xx_WaitToRestore;
}

/* zl303xx_SetQLHoldOff */
/**
   Only applies to Telecom Profile. Sets the time that the previous master's
   QL value will be output at a slave, after all masters are in failure conditions.

  Parameters:
   [in]   qlHoldOffSec   Time in seconds.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetQLHoldOff(Uint32T qlHoldOffSec)
{
   zl303xx_QLHoldOff = qlHoldOffSec;
   return ZL303XX_OK;
}

/* zl303xx_GetQLHoldOff */
/**
   Only applies to Telecom Profile. Returns the QL hold off time.

  Parameters:
      None

  Return Value:  Uint32T  Time in seconds.

*******************************************************************************/
Uint32T zl303xx_GetQLHoldOff(void)
{
   return zl303xx_QLHoldOff;
}

/* zl303xx_SetPtpVersion */
/**
   Set the PTP Version

  Parameters:
   [in]   ptpVersion

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetPtpVersion(zl303xx_PtpVersionE ptpVersion)
{
    zl303xx_PtpVersion = ptpVersion;
    return ZL303XX_OK;
}

/* zl303xx_GetPtpVersion */
/**
   Get the PTP Version 

  Parameters:
      None

  Return Value:  zl303xx_PtpVersionE

*******************************************************************************/
zl303xx_PtpVersionE zl303xx_GetPtpVersion(void)
{
    return zl303xx_PtpVersion;
}

/* zl303xx_SetDefaultClockClass */
/**
   Set the default clock class.

  Parameters:
   [in]   clockClass

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetDefaultClockClass(Uint8T clockClass)
{
    zl303xx_DefaultClockClass = clockClass;
    return ZL303XX_OK;
}

/* zl303xx_GetDefaultClockClass */
/**
   Get the default clock class.

  Parameters:
      None

  Return Value:  Uint8T

*******************************************************************************/
Uint8T zl303xx_GetDefaultClockClass(void)
{
   return zl303xx_DefaultClockClass;
}

/* zl303xx_SetDefaultClockAccuracy */
/**
   Set the default Clock Accuracy.

  Parameters:
   [in]   clockAccuracy

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetDefaultClockAccuracy(Uint8T clockAccuracy)
{
    zl303xx_DefaultClockAccuracy = clockAccuracy;
    return ZL303XX_OK;
}

/* zl303xx_GetDefaultClockAccuracy */
/**
   Get the default Clock Accuracy.

  Parameters:
      None

 retval  Uint8T

*******************************************************************************/
Uint8T zl303xx_GetDefaultClockAccuracy(void)
{
   return zl303xx_DefaultClockAccuracy;
}

/* zl303xx_SetBoundaryFlag */
/**
   Set the boundary flag for the system.

  Parameters:
   [in]   boundaryFlag

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetBoundaryFlag(const zl303xx_BooleanE boundaryFlag)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(boundaryFlag);
   }

   if (status == ZL303XX_OK)
   {
      zl303xx_UseBoundaryFlag = boundaryFlag;
   }

   return status;
}

/* zl303xx_GetBoundaryFlag */
/**
   Get the boundary flag.

  Parameters:
      None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetBoundaryFlag(void)
{
   return zl303xx_UseBoundaryFlag;
}

/* zl303xx_SetITUTFlag */
/**
   Set the Use ITU Telecom Profile flag

  Parameters:
   [in]   itutFlag

  Return Value:  zlStatusE

  (Deprecated)  This function has been replaced by zl303xx_SetPtpProfile().

*******************************************************************************/
zlStatusE zl303xx_SetITUTFlag(zl303xx_BooleanE itutFlag)
{
   zlStatusE status = ZL303XX_CHECK_BOOLEAN(itutFlag);

   if (status == ZL303XX_OK)
   {
      status = zl303xx_SetPtpProfile((itutFlag == ZL303XX_TRUE)
                                      ? ZL303XX_PTP_PROFILE_TELECOM_G8265_1
                                      : ZL303XX_PTP_PROFILE_DEFAULT);
   }

   return status;
}

/* zl303xx_SetUnicastStackedTlvEn */
/**
   Sets the Unicast Negotiation TLV transmission mode as stacked (TRUE) or
   single (FALSE).

  Parameters:
   [in]   stackedTlvEn

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetUnicastStackedTlvEn(zl303xx_BooleanE stackedTlvEn)
{
    zl303xx_UnicastStackedTlvEn = stackedTlvEn;
    return ZL303XX_OK;
}

/* zl303xx_GetUnicastStackedTlvEn */
/**
   Gets the Unicast Negotiation TLV transmission mode.

  Parameters:
      None

  Return Value:
     ZL303XX_FALSE  single TLV mode
     ZL303XX_TRUE   stacked TLV mode

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetUnicastStackedTlvEn(void)
{
    return zl303xx_UnicastStackedTlvEn;
}

/* zl303xx_SetUnicastGrantDefault */
/**
   Sets the grantDefault parameter for subsequently created PTP ports.

  Parameters:
   [in]   grantDefault  Default grant behavior to set.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetUnicastGrantDefault(zl303xx_BooleanE grantDefault)
{
   zl303xx_GrantDefault = grantDefault;
   return ZL303XX_OK;
}

/* zl303xx_GetUnicastGrantDefault */
/**
   Returns the grantDefault parameter, used for PTP port creation.

  Parameters:
     None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetUnicastGrantDefault(void)
{
   return zl303xx_GrantDefault;
}

/* zl303xx_SetBmcaRevertiveEn */
/**
   Enables/disables the revertive BMCA option in PTP.

  Parameters:
   [in]   revertive   Revertive switching enabled flag.

  Return Value:   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetBmcaRevertiveEn(zl303xx_BooleanE revertive)
{
   zl303xx_BmcaRevertiveEn = revertive;
   return ZL303XX_OK;
}

/* zl303xx_GetBmcaRevertiveEn */
/**
   Returns the revertive BMCA option flag.

  Parameters:
   [in]   None

  Return Value:
      ZL303XX_TRUE   Revertive switching enabled
      ZL303XX_FALSE  Revertive switching disabled

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetBmcaRevertiveEn(void)
{
   return zl303xx_BmcaRevertiveEn;
}

/* zl303xx_SetTelecomMasterPriority */
/**
   Sets the local priority for the next Unicast Master Table entry. Only affects
   Telecom Profile operation.

  Parameters:
   [in]   priority   Local priority value to set.

  Return Value:   ZL303XX_OK

*******************************************************************************/
zlStatusE zl303xx_SetTelecomMasterPriority(Uint8T priority)
{
   zl303xx_TelecomMasterPriority = priority;
   return ZL303XX_OK;
}

/* zl303xx_GetTelecomMasterPriority */
/**
   Returns the local priority for a Unicast Master. Only applies to Telecom
   Profile operation. Should only be called by exampleConfig().

  Parameters:
      None

  Return Value:  Uint8T

*******************************************************************************/
Uint8T zl303xx_GetTelecomMasterPriority(void)
{
   return zl303xx_TelecomMasterPriority;
}

/* zl303xx_SetUnicastMasterReqAnnc */
/**
   Sets the "request Announce" flag for the next Unicast Master Table entry. Only
   affects Telecom Profile operation.

  Parameters:
   [in]   reqAnnc   Request announce value to set.

  Return Value:   ZL303XX_OK

*******************************************************************************/
zlStatusE zl303xx_SetUnicastMasterReqAnnc(zl303xx_BooleanE reqAnnc)
{
   zlStatusE status = ZL303XX_CHECK_BOOLEAN(reqAnnc);

   if (status == ZL303XX_OK)
   {
      zl303xx_UnicastMasterReqAnnc = reqAnnc;
   }

   return status;
}

/* zl303xx_GetUnicastMasterReqAnnc */
/**
   Returns the "request Announce" flag for a Unicast Master. Only applies to
   Telecom Profile operation. Should only be called by exampleConfig().

  Parameters:
      None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetUnicastMasterReqAnnc(void)
{
   return zl303xx_UnicastMasterReqAnnc;
}

/* zl303xx_SetUnicastMasterReqSync */
/**
   Sets the "request Sync" flag for the next Unicast Master Table entry. Only
   affects Telecom Profile operation.

  Parameters:
   [in]   reqSync   Request Sync value to set.

  Return Value:   ZL303XX_OK

*******************************************************************************/
zlStatusE zl303xx_SetUnicastMasterReqSync(zl303xx_BooleanE reqSync)
{
   zlStatusE status = ZL303XX_CHECK_BOOLEAN(reqSync);

   if (status == ZL303XX_OK)
   {
      zl303xx_UnicastMasterReqSync = reqSync;
   }

   return status;
}

/* zl303xx_GetUnicastMasterReqSync */
/**
   Returns the "request Sync" flag for a Unicast Master. Only applies to
   Telecom Profile operation. Should only be called by exampleConfig().

  Parameters:
      None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetUnicastMasterReqSync(void)
{
   return zl303xx_UnicastMasterReqSync;
}

/* zl303xx_SetUnicastMasterReqDelayResp */
/**
   Sets the "request Delay_Resp" flag for the next Unicast Master Table entry.
   Only affects Telecom Profile operation.

  Parameters:
   [in]   reqDresp   Request Delay_Resp value to set.

  Return Value:   ZL303XX_OK

*******************************************************************************/
zlStatusE zl303xx_SetUnicastMasterReqDelayResp(zl303xx_BooleanE reqDresp)
{
   zlStatusE status = ZL303XX_CHECK_BOOLEAN(reqDresp);

   if (status == ZL303XX_OK)
   {
      zl303xx_UnicastMasterReqDelayResp = reqDresp;
   }

   return status;
}

/* zl303xx_GetUnicastMasterReqDelayResp */
/**
   Returns the "request Delay_Resp" flag for a Unicast Master. Only applies to
   Telecom Profile operation. Should only be called by exampleConfig().

  Parameters:
      None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetUnicastMasterReqDelayResp(void)
{
   return zl303xx_UnicastMasterReqDelayResp;
}

/* zl303xx_SetUnicastMasterReqPdelayResp */
/**
   Sets the "request Peer_Delay_Resp" flag for the next Unicast Master Table entry.
   Only affects Telecom Profile operation.

  Parameters:
   [in]   reqPDresp   Request Peer_Delay_Resp value to set.

  Return Value:   ZL303XX_OK

*******************************************************************************/
zlStatusE zl303xx_SetUnicastMasterReqPdelayResp(zl303xx_BooleanE reqPDresp)
{
   zlStatusE status = ZL303XX_CHECK_BOOLEAN(reqPDresp);

   if (status == ZL303XX_OK)
   {
      zl303xx_UnicastMasterReqPdelayResp = reqPDresp;
   }

   return status;
}

/* zl303xx_GetUnicastMasterReqPdelayResp */
/**
   Returns the "request Peer_Delay_Resp" flag for a Unicast Master.
   Only applies to Telecom Profile operation. Should only be called by exampleConfig().

  Parameters:
      None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetUnicastMasterReqPdelayResp(void)
{
   return zl303xx_UnicastMasterReqPdelayResp;
}

/* zl303xx_SetUnicastMasterReqTiming */
/**
   Sets the "request timing" flag (applies to SYNC and DELAY_RESP messages) for
   the next Unicast Master Table entry. Only affects Telecom Profile operation.

  (Deprecated)
   zl303xx_SetUnicastMasterReqSync() and zl303xx_SetUnicastMasterReqDelayResp()
   should be used instead.

  Parameters:
   [in]   reqTiming   Request timing value to set.

  Return Value:   ZL303XX_OK

*******************************************************************************/
zlStatusE zl303xx_SetUnicastMasterReqTiming(zl303xx_BooleanE reqTiming)
{
   zlStatusE status = ZL303XX_CHECK_BOOLEAN(reqTiming);

   if (status == ZL303XX_OK)
   {
      zl303xx_UnicastMasterReqTiming = reqTiming;
   }

   return status;
}

/* zl303xx_GetUnicastMasterReqTiming */
/**
   Returns the "request announce" flag for a Unicast Master. Only applies to
   Telecom Profile operation. Should only be called by exampleConfig().

  (Deprecated)
   zl303xx_GetUnicastMasterReqSync() and zl303xx_GetUnicastMasterReqDelayResp()
   should be used instead.

  Parameters:
      None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetUnicastMasterReqTiming(void)
{
   return zl303xx_UnicastMasterReqTiming;
}

/* zl303xx_SetSrcIpAddrStr */
/**
   Sets the Local IP Address.

  Parameters:
   [in]   ipAddrStr  Dot-delimited IP Address String

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetSrcIpAddrStr(const char *ipAddrStr)
{
    if (ipAddrStr == NULL)
    {
        return ZL303XX_INVALID_POINTER;
    }

    memset(zl303xx_SrcIpAddrStr, 0, sizeof(zl303xx_SrcIpAddrStr));
    strncpy((char*)zl303xx_SrcIpAddrStr, ipAddrStr, sizeof(zl303xx_SrcIpAddrStr) - 1);
    return ZL303XX_OK;
}

/* zl303xx_GetSrcIpAddrStr */
/**
   Gets the Local IP Address

  Parameters:
      None

  Return Value:  Uint8T  Pointer to Dot-delimited IP Address String

*******************************************************************************/
char *zl303xx_GetSrcIpAddrStr(void)
{
   return zl303xx_SrcIpAddrStr;
}

/* zl303xx_SetDestIpAddrStr */
/**
   Set the Destination Ip Addr String.

  Parameters:
   [in]   ipAddrStr  Dot-delimited IP Address String

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetDestIpAddrStr(const char *ipAddrStr)
{
    if (ipAddrStr == NULL)
    {
        return ZL303XX_INVALID_POINTER;
    }

    memset(zl303xx_DestIpAddrStr, 0, sizeof(zl303xx_DestIpAddrStr));
    (void)strncpy((char *)zl303xx_DestIpAddrStr, ipAddrStr, sizeof(zl303xx_DestIpAddrStr) - 1);
    return ZL303XX_OK;
}

/* zl303xx_GetDestIpAddrStr */
/**
   Get the Destination Ip Addr String.

  Parameters:
      None

  Return Value:  Uint8T  Pointer to Dot-delimited IP Address String

*******************************************************************************/
char *zl303xx_GetDestIpAddrStr(void)
{
   return zl303xx_DestIpAddrStr;
}

static zlStatusE checkIpStr(char *ipStr);
static zlStatusE checkIpStr(char *ipStr)
{
    if(ipStr == NULL)
    {
        printf("Invalid format for IP1 address (NULL) - try ###.###.###.###\n");
        return ZL303XX_PARAMETER_INVALID;
    }
    SINT_T ip1 = 0, ip2 = 0, ip3 = 0, ip4 = 0;
    size_t len = strlen(ipStr);
    char *strContext;

    /* Convert the user input into usable data */
    if (len > 0)
    {
        ipStr = strtok_r(ipStr, ".\n", &strContext);

        sscanf(ipStr, "%d", &ip1);    /* Valid digit */
        sscanf(ipStr, "%2d", &ip1);    /* 2 digits */
        sscanf(ipStr, "%3d", &ip1);    /* 3 digits */
        sscanf(ipStr, "%4d", &ip1);    /* 4 digits - Invalid */
    }
    else
    {
        printf("Invalid format for IP1 address - try ###.###.###.###\n");
        return ZL303XX_PARAMETER_INVALID;
    }

    ipStr = strtok_r(NULL, ".\n", &strContext);
    if (ipStr)
    {
        sscanf(ipStr, "%d", &ip2);
        sscanf(ipStr, "%2d", &ip2);
        sscanf(ipStr, "%3d", &ip2);
        sscanf(ipStr, "%4d", &ip2);
    }
    else
    {
        printf("Invalid format for IP2 address - try %d.###.###.###\n", ip1);
        return ZL303XX_PARAMETER_INVALID;
    }

    ipStr = strtok_r(NULL, ".\n", &strContext);
    if (ipStr)
    {
        sscanf(ipStr, "%d", &ip3);
        sscanf(ipStr, "%2d", &ip3);
        sscanf(ipStr, "%3d", &ip3);
        sscanf(ipStr, "%4d", &ip3);
    }
    else
    {
        printf("Invalid format for IP3 address - try %d.%d.###.###\n", ip1, ip2);
        return ZL303XX_PARAMETER_INVALID;
    }

    ipStr = strtok_r(NULL, ".\n", &strContext);
    if (ipStr)
    {
        sscanf(ipStr, "%d", &ip4);
        sscanf(ipStr, "%2d", &ip4);
        sscanf(ipStr, "%3d", &ip4);
        sscanf(ipStr, "%4d", &ip4);
    }
    else
    {
        printf("Invalid format for IP4 address - try %d.%d.#%d.###\n", ip1, ip2, ip3);
        return ZL303XX_PARAMETER_INVALID;
    }

    /* Verify ip range */
    if (ip1 < 0 || ip1 > 255)
    {
        ZL303XX_ERROR_NOTIFY("Invalid IP1 address value (0 - 255)");
        return ZL303XX_PARAMETER_INVALID;
    }
    if (ip2 < 0 || ip2 > 255)
    {
        ZL303XX_ERROR_NOTIFY("Invalid IP2 address value (0 - 255)");
        return ZL303XX_PARAMETER_INVALID;
    }
    if (ip3 < 0 || ip3 > 255)
    {
        ZL303XX_ERROR_NOTIFY("Invalid IP3 address value (0 - 255)");
        return ZL303XX_PARAMETER_INVALID;
    }
    if (ip4 < 0 || ip4 > 255)
    {
        ZL303XX_ERROR_NOTIFY("Invalid IP4 address value (0 - 255)");
        return ZL303XX_PARAMETER_INVALID;
    }

    ZL303XX_TRACE_ALWAYS("Ip Address= %d.%d.%d.%d\n", ip1, ip2, ip3, ip4, 0,0);


    return ZL303XX_OK;
}

void setIpAddress(char *userCmd);
void setIpAddress(char *userCmd)
{
   char ipAddr[18];
#ifndef SEPARATORS
#define SEPARATORS ", \t\n"
#endif

   userCmd = strtok(NULL, SEPARATORS);
   if (!userCmd)
   {
      ZL303XX_ERROR_NOTIFY("Invalid IP address string format.\tUse format \"###.###.###.###\"");
   }
   else
   {
      /* Ensure the string is NULL terminated */
      memset(ipAddr, 0x00, sizeof(ipAddr));
      (void)strncpy(ipAddr, userCmd, sizeof(ipAddr)-1);

      if (checkIpStr(ipAddr) == ZL303XX_OK)
      {
         zl303xx_SetDestIpAddrStr((const char*)userCmd);
      }
   }
}

/* zl303xx_SetApr1HzDisabled */
/**
   Set the APR 1Hz disabled flag for the specified clock stream.

  Parameters:
   [in]   clockStreamHandle
   [in]   disabled

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetApr1HzDisabled(Uint32T clockStreamHandle,
                                    zl303xx_BooleanE disabled)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      if (clockStreamHandle >= ZL303XX_PTP_NUM_STREAMS_MAX)
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(disabled);
   }

   if (status == ZL303XX_OK)
   {
      if (zl303xx_StreamConfigComplete(clockStreamHandle) == ZL303XX_TRUE)
      {
         status = ZL303XX_INIT_ERROR;
      }
   }

   if (status == ZL303XX_OK)
   {
      zl303xx_Apr1HzDisabled[clockStreamHandle] = disabled;
   }

   return status;
}

/* zl303xx_GetApr1HzDisabled */
/**
   Get the APR 1Hz disable flag

  Parameters:
   [in]   clockStreamHandle

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetApr1HzDisabled(zl303xx_PtpStreamHandleT clockStreamHandle)
{
   zl303xx_BooleanE retVal = ZL303XX_TRUE;

   if (clockStreamHandle < ZL303XX_PTP_NUM_STREAMS_MAX)
   {
      retVal = zl303xx_Apr1HzDisabled[clockStreamHandle];
   }

   return retVal;
}
#endif

#if defined ZLS30341_INCLUDED
/* zl303xx_SetLog2SysInterruptPeriod */
/**
   Set the system interrupt period.

   Interrupt rate[Hz] = 80x10^6 / 2^log2Period

  Parameters:
   [in]   log2Period  Range 20-26 inclusive.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetLog2SysInterruptPeriod(Uint32T log2Period)
{
   zlStatusE status = ZL303XX_OK;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      if ((log2Period < ZL303XX_MIN_LOG2_SYS_INTERRUPT_PERIOD) ||
          (log2Period > ZL303XX_MAX_LOG2_SYS_INTERRUPT_PERIOD))
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      zl303xx_Log2SysInterruptPeriod = log2Period;
   }

   return status;
}

/* zl303xx_GetLog2SysInterruptPeriod */
/**
   Get the system interrupt period.

   Interrupt rate[Hz] = 80x10^6 / 2^zl303xx_GetLog2SysInterruptPeriod()

  Parameters:
      None

  Return Value:  Uint32T

*******************************************************************************/
Uint32T zl303xx_GetLog2SysInterruptPeriod(void)
{
   return zl303xx_Log2SysInterruptPeriod;
}
#endif


/*****************   END   ****************************************************/
