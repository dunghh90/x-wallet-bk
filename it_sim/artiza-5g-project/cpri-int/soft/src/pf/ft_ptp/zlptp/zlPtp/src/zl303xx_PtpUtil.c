

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Some text, starts at column 7, same as following lines
*
*******************************************************************************/


/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Os.h"
#include "zl303xx_Trace.h"
#include "zl303xx_ErrTrace.h"
#include "zl303xx_Error.h"
#include "zl303xx_Macros.h"

#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpInternalTypes.h"

#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpProfileDef.h"
#include "zl303xx_PtpG8265p1Api.h"
#include "zl303xx_PtpV2MsgParse.h"

#include "zl303xx_Ptsf.h"
#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpStrings.h"

#ifndef ntohl
  #include <netinet/in.h>
#endif

#ifdef OS_FREERTOS
#include "def.h"
#endif

/*****************   DEFINES     **********************************************/

#define MAX_IPV4_ADDR_STR_LEN 16
#define MAX_IPV6_ADDR_STR_LEN 40
#define MAX_MAC_ADDR_STR_LEN  18

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/
static zlStatusE zl303xx_PtpHwCmdFnCheck(zl303xx_PtpClockS *pPtpClock);

static zlStatusE zl303xx_PtpConvertIPv4ToStr(Uint8T *ipv4, char *str);
static zlStatusE zl303xx_PtpConvertIPv6ToStr(Uint8T *ipv6, char *str);
static zlStatusE zl303xx_PtpConvertMacToStr (Uint8T *mac,  char *str);

/*****************   STATIC GLOBAL VARIABLES   ********************************/
static Uint16T Zl303xx_PtpPortAddrLen[] = {
      0,    /* Invalid */
      4,    /* ZL303XX_NETWORK_PROTOCOL_UDP_IPv4   */
      16,   /* ZL303XX_NETWORK_PROTOCOL_UDP_IPv6   */
      6,    /* ZL303XX_NETWORK_PROTOCOL_802_3      */
      2,    /* ZL303XX_NETWORK_PROTOCOL_DEVICENET  */
      2,    /* ZL303XX_NETWORK_PROTOCOL_CONTROLNET */
      6,    /* ZL303XX_NETWORK_PROTOCOL_PROFINET   */
};

static char const * const Zl303xx_PtpPortAddrStr[] = {
   NULL,               /* Invalid */
   "UDP/IPv4",         /* ZL303XX_NETWORK_PROTOCOL_UDP_IPv4   */
   "UDP/IPv6",         /* ZL303XX_NETWORK_PROTOCOL_UDP_IPv6   */
   "802.3/Ethernet",   /* ZL303XX_NETWORK_PROTOCOL_802_3      */
   "DeviceNet",        /* ZL303XX_NETWORK_PROTOCOL_DEVICENET  */
   "ControlNet",       /* ZL303XX_NETWORK_PROTOCOL_CONTROLNET */
   "PROFINET"          /* ZL303XX_NETWORK_PROTOCOL_PROFINET   */
};

static const char *Zl303xx_PtpPortStateStr[] = {  /* Matched with zl303xx_PortStateE */
    "UNKNOWN",       /*  */
    "INITIALIZING",  /* ZL303XX_PORT_STATE_INITIALIZING */
    "FAULTY",        /* ZL303XX_PORT_STATE_FAULTY       */
    "DISABLED",      /* ZL303XX_PORT_STATE_DISABLED     */
    "LISTENING",     /* ZL303XX_PORT_STATE_LISTENING    */
    "PRE_MASTER",    /* ZL303XX_PORT_STATE_PRE_MASTER   */
    "MASTER",        /* ZL303XX_PORT_STATE_MASTER       */
    "PASSIVE",       /* ZL303XX_PORT_STATE_PASSIVE      */
    "UNCALIBRATED",  /* ZL303XX_PORT_STATE_UNCALIBRATED */
    "SLAVE"          /* ZL303XX_PORT_STATE_SLAVE        */
};

/* Matched with zl303xx_PtpPortFaultTypeE */
static char const * const Zl303xx_PtpPortFaultTypeStr[ZL303XX_PTP_PORT_FAULT_TYPE_MAX] = {
    "Error sending Follow_Up packet",           /*ZL303XX_PTP_PORT_FAULT_FOLLOW_UP_SEND */
    "Error sending Sync packet",                /* ZL303XX_PTP_PORT_FAULT_SYNC_SEND */
    "Error sending Announce packet",            /*ZL303XX_PTP_PORT_FAULT_ANNC_SEND */
    "Error sending Delay_Req packet",           /*ZL303XX_PTP_PORT_FAULT_DELAY_REQ_SEND */
    "Error sending Delay_Resp packet",          /* ZL303XX_PTP_PORT_FAULT_DELAY_RESP_SEND */
    "Error sending PDelay_Req packet",          /*ZL303XX_PTP_PORT_FAULT_PDELAY_REQ_SEND */
    "Error sending Peer Delay_Resp packet",     /*ZL303XX_PTP_PORT_FAULT_PDELAY_RESP_SEND */
    "Error sending Signaling packet",           /* ZL303XX_PTP_PORT_FAULT_SIGNALLING_SEND */
    "Error sending Management packet",          /*ZL303XX_PTP_PORT_FAULT_MGMT_SEND */
    "User Forced Port into Faulty"              /* ZL303XX_PTP_PORT_FAULT_USER_ENFORCED */
};

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/* Multicast addresses extern'ed in zl303xx_PtpConstants_dep.h. The top 4 octets
 * of FF05 make these addresses IPv6 multicast with site-local scope, as
 * defined in IETF RFC 4291. */
const Uint8T Zl303xx_PtpDefaultDomainAddrIPv6[] = {     /* DEFAULT_PTP_DOMAIN_ADDRESS_IPV6    */
                0xFF, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x81};

const Uint8T Zl303xx_PtpAlternateDomain1AddrIPv6[] = {  /* ALTERNATE_PTP_DOMAIN1_ADDRESS_IPV6 */
                0xFF, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x82};

const Uint8T Zl303xx_PtpAlternateDomain2AddrIPv6[] = {  /* ALTERNATE_PTP_DOMAIN2_ADDRESS_IPV6 */
                0xFF, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x83};

const Uint8T Zl303xx_PtpAlternateDomain3AddrIPv6[] = {  /* ALTERNATE_PTP_DOMAIN3_ADDRESS_IPV6 */
                0xFF, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x84};

const char *Zl303xx_PtpDefaultDomainAddrStrIPv6    = "FF05::181/64";
const char *Zl303xx_PtpAlternateDomain1AddrStrIPv6 = "FF05::182/64";
const char *Zl303xx_PtpAlternateDomain2AddrStrIPv6 = "FF05::183/64";
const char *Zl303xx_PtpAlternateDomain3AddrStrIPv6 = "FF05::184/64";

const Uint8T Zl303xx_PtpDefaultDomainAddr802_3[] = {    /* DEFAULT_PTP_DOMAIN_ADDRESS_802_3 */
                0x01, 0x1B, 0x19, 0x00, 0x00, 0x00};

const Uint8T Zl303xx_PtpAlternateDomain1Addr802_3[] = { /* ALTERNATE_PTP_DOMAIN1_ADDRESS_802_3 */
                0x01, 0x1B, 0x19, 0x00, 0x00, 0x01};

const Uint8T Zl303xx_PtpAlternateDomain2Addr802_3[] = { /* ALTERNATE_PTP_DOMAIN2_ADDRESS_802_3 */
                0x01, 0x1B, 0x19, 0x00, 0x00, 0x02};

const Uint8T Zl303xx_PtpAlternateDomain3Addr802_3[] = { /* ALTERNATE_PTP_DOMAIN3_ADDRESS_802_3 */
                0x01, 0x1B, 0x19, 0x00, 0x00, 0x03};

const Uint8T Zl303xx_PtpPdelayDomainAddr802_3[] = {     /* PDELAY_PTP_DOMAIN_ADDRESS_802_3 */
                0x01, 0x80, 0xC2, 0x00, 0x00, 0x0E};

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   IMPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* zl303xx_PtpConvertIPv4ToPortAddress */
/**
   Converts an IPv4 dot-format string to a PTP port address structure.

  Parameters:
   [in]   ipAddr        The IPv4 address string.
   [out]  pPortAddress  The equivalent PTP port address.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    ipAddr or pPortAddress is NULL.
     ZL303XX_PARAMETER_INVALID  ipAddr is formatted incorrectly.

*******************************************************************************/
zlStatusE zl303xx_PtpConvertIPv4ToPortAddress(const char *ipAddr,
                                              zl303xx_PortAddress *pPortAddress)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "zl303xx_PtpConvertIPv4ToPortAddress:", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(ipAddr) |
               ZL303XX_CHECK_POINTER(pPortAddress);
   }

   /* Convert the IPv4 address string. */
   if (status == ZL303XX_OK)
   {
      Uint16T addr[4] = {0};

      if (sscanf(ipAddr, "%3hu.%3hu.%3hu.%3hu",
                 addr, addr + 1, addr + 2, addr + 3) == 4)
      {
         Uint8T byteId = 0;
         for (; byteId < 4; byteId++)
         {
            if (addr[byteId] <= 255)
            {
               pPortAddress->addressField[byteId] = (Uint8T)addr[byteId];
            }
            else
            {
               status = ZL303XX_PARAMETER_INVALID;
               break;
            }
         }
         pPortAddress->networkProtocol = ZL303XX_NETWORK_PROTOCOL_UDP_IPv4;
         pPortAddress->addressLength = 4;
      }
      else
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE_ERROR("   (ERROR) Cannot convert ipAddr=%s.",
                          ipAddr, 0,0,0,0,0);
   }

   return status;
}

/* zl303xx_PtpConvertEth2ToPortAddress */
/**
   Converts an Ethernet-Layer 2 address to a PTP port address structure.

  Parameters:
   [in]   eth2Address   The Ethernet-Layer 2 address.
   [out]  pPortAddress  The equivalent PTP port address.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    eth2Address or pPortAddress is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpConvertEth2ToPortAddress(
      const char *eth2Address,
      zl303xx_PortAddress *pPortAddress)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpConvertEth2ToPortAddress:", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(eth2Address) |
               ZL303XX_CHECK_POINTER(pPortAddress);
   }

   /* Convert the Ethernet-Layer 2 address is an array of bytes so no real
    * conversion is required; just fill in the zl303xx_PortAddress fields. */
   if (status == ZL303XX_OK)
   {
      pPortAddress->networkProtocol = ZL303XX_NETWORK_PROTOCOL_802_3;
      pPortAddress->addressLength = Zl303xx_PtpPortAddrLen[ZL303XX_NETWORK_PROTOCOL_802_3];

      /* Copy the address. */
      OS_MEMCPY(pPortAddress->addressField,
             eth2Address,
             pPortAddress->addressLength);
   }

   return status;
}


/* zl303xx_PtpConvertPortAddressToIPv4 */
/**
   Converts a PTP port address structure to an IPv4 dot-format string.

  Parameters:
   [in]   pPortAddress  The PTP port address.
   [out]  ipAddr        The equivalent IPv4 address string.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    ipAddr or pPortAddress is NULL.
     ZL303XX_PARAMETER_INVALID  pPortAddress does not contain a UDP/IPv4 address.

*******************************************************************************/
zlStatusE zl303xx_PtpConvertPortAddressToIPv4(zl303xx_PortAddress *pPortAddress,
                                              char *ipAddr)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "zl303xx_PtpConvertPortAddressToIPv4:", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pPortAddress) |
               ZL303XX_CHECK_POINTER(ipAddr);
   }
   /* Check if the port address represents a UDP/IPv4 address. */
   if (status == ZL303XX_OK)
   {
      if (pPortAddress->networkProtocol != ZL303XX_NETWORK_PROTOCOL_UDP_IPv4)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Port address is not a UDP/IPv4 address.", 0,0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }
   /* Check that the address length is correct. */
   if (status == ZL303XX_OK)
   {

      if (pPortAddress->addressLength != Zl303xx_PtpPortAddrLen[ZL303XX_NETWORK_PROTOCOL_UDP_IPv4])
      {
         ZL303XX_TRACE_ERROR("   (ERROR) UDP/IPv4 port addressLength=%u (must be %u).",
            pPortAddress->addressLength, Zl303xx_PtpPortAddrLen[ZL303XX_NETWORK_PROTOCOL_UDP_IPv4], 0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Convert the PTP port address to a struct in_addr. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpConvertIPv4ToStr(pPortAddress->addressField, ipAddr);
   }

   return status;
}

/* zl303xx_PtpConvertPortAddressToStr */
/**
   Converts a PTP port address structure to its equivalent string format.

  Parameters:
   [in]   pAddr   The PTP port address.
   [out]  str     The equivalent address string. Should be 40 characters
                       in size to hold the longest possible IPv6 address.

  Return Value:
     ZL303XX_OK                      Success.
     ZL303XX_INVALID_POINTER         pAddr or str is NULL.
     ZL303XX_PARAMETER_INVALID       pAddr does not contain a valid address.
     ZL303XX_UNSUPPORTED_OPERATION   The networkProtocol of pAddr does not
                                          have a string conversion routine.

*******************************************************************************/
zlStatusE zl303xx_PtpConvertPortAddressToStr(zl303xx_PortAddress *pAddr, char *str)
{
   zlStatusE status = ZL303XX_OK;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pAddr) |
               ZL303XX_CHECK_POINTER(str);
   }
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortAddressCheck(pAddr);
   }

   /* Convert to string based on the network protocol type. */
   if (status == ZL303XX_OK)
   {
      switch (pAddr->networkProtocol)
      {
         case ZL303XX_NETWORK_PROTOCOL_UDP_IPv4 :
            status = zl303xx_PtpConvertIPv4ToStr(pAddr->addressField, str);
            break;

         case ZL303XX_NETWORK_PROTOCOL_UDP_IPv6 :
            status = zl303xx_PtpConvertIPv6ToStr(pAddr->addressField, str);
            break;

         case ZL303XX_NETWORK_PROTOCOL_802_3 :
            status = zl303xx_PtpConvertMacToStr(pAddr->addressField, str);
            break;

         /* fall-through for removal of switch-enum warning */
         case ZL303XX_NETWORK_PROTOCOL_DEVICENET :
         case ZL303XX_NETWORK_PROTOCOL_CONTROLNET :
         case ZL303XX_NETWORK_PROTOCOL_PROFINET :
         case ZL303XX_NETWORK_PROTOCOL_UNKNOWN :
         default:
            ZL303XX_TRACE_ALWAYS("Unable to convert port address to string (networkProtocol=%d)",
                                 pAddr->networkProtocol, 0,0,0,0,0);
            status = ZL303XX_UNSUPPORTED_OPERATION;
            break;
      }
   }

   return status;
}

/*

  Function Name:
    zl303xx_PtpConvTimeStampToTimeInterval

  Details:
   Converts from PTP TimeStamp format (similar to TimeRepresentation in V1) to
   PTP TimeInterval format.

  Parameters:
   [in]  pInTimeStamp      TimeStamp value (in format 48-bit sec, 32-bit ns;
                                 assume the sign bit is in bit-31 of ns field).

   [out] pOutTimeInterval  TimeInterval equivalent of the TimeStamp
                                 (in format signed 48-bit ns, 16-bit fractional ns).

  Return Value:
   ZL303XX_OK                   The value was converted properly.
   ZL303XX_INVALID_POINTER      Either of the 2 input pointers is NULL.
   ZL303XX_PARAMETER_INVALID    The Timestamp was too large to be converted to a
                                 TimeInterval equivalent.

  Notes:
   In the strictest sense, Timestamp format is always a positive value only (the
   nanosecondsField is Uint32T). However, TimeInterval is a signed value.
   Therefore, the upper-bit of the nanosecondsField of the Timestamp will be
   used to indicate the 'sign' of the value. This will be used only for internal
   purposes. All 'on-the-wire' values will be positive Timestamp values.
   (Note: the V1 TimeRepresentation used a Sint32T for the nanoseconds field to
   handle negative values).

*******************************************************************************/
zlStatusE zl303xx_PtpConvTimeStampToTimeInterval(
      zl303xx_TimeStamp *pInTimeStamp,
      zl303xx_TimeInterval *pOutTimeInterval)
{
   zlStatusE status = ZL303XX_OK;

   zl303xx_BooleanE bNegativeInput = ZL303XX_FALSE;
   Uint64S tempOutNs = {0,0};
   Uint64S tempInNs  = {0,0};

   /* Check the input pointers. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pInTimeStamp) |
               ZL303XX_CHECK_POINTER(pOutTimeInterval);
   }

   /* Check that the Timestamp value can fit within the range of the
    * TimeInterval type. */
   if (status == ZL303XX_OK)
   {
      /* A TimeInterval can handle 48-bits of nanoseconds:
       *       0xFFFF FFFFFFFF ns = 281474.976710655 seconds
       * so set a limit of 281474 seconds.
       * Conversion from a TimeInterval to TimeStamp does not have this issue. */
      #define ZL303XX_PTP_TIMESTAMP_TO_INTERVAL_CONV_LIMIT_SEC  (Uint32T)281474

      if ((pInTimeStamp->secondsField.hi > 0) ||
          (pInTimeStamp->secondsField.lo > ZL303XX_PTP_TIMESTAMP_TO_INTERVAL_CONV_LIMIT_SEC))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpConvTimeStampToTimeInterval: Input OOR (%lu:%lu:%lu)",
               pInTimeStamp->secondsField.hi, pInTimeStamp->secondsField.lo,
               pInTimeStamp->nanosecondsField, 0,0,0);
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
               "zl303xx_PtpConvTimeStampToTimeInterval: Input OOR (%lu:%lu:%lu)",
               pInTimeStamp->secondsField.hi, pInTimeStamp->secondsField.lo,
               pInTimeStamp->nanosecondsField, 0,0,0);

         OS_MEMSET(pOutTimeInterval, 0x00, sizeof(*pOutTimeInterval));
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Perform the necessary math. */
   if (status == ZL303XX_OK)
   {
      /* If the input is Negative, get the absolute value.
       * (TimeStamps flip only the top bit for the sign indication).  */
      if ((bNegativeInput = isNegativeTime(pInTimeStamp)) == ZL303XX_TRUE)
      {
         negateTime(pInTimeStamp);
      }

      /* The .hi input seconds must be 0 so multiply the .lo seconds by 1Billion. */
      tempOutNs = Mult_U32_U32(pInTimeStamp->secondsField.lo, 1000000000);

      /* Add the input nanoseconds. */
      tempInNs.lo = pInTimeStamp->nanosecondsField;
      pOutTimeInterval->scaledNanoseconds = Add_U64S(tempOutNs, tempInNs, NULL);

      /* Left-Shift the value by 16-bits. */
      pOutTimeInterval->scaledNanoseconds = LShift_U64S(pOutTimeInterval->scaledNanoseconds, 16);

      /* Restore the sign of the value (if necessary).
       * (TimeInterval uses 2's complement format).  */
      if (bNegativeInput == ZL303XX_TRUE)
      {
         /* do a 2's complement negation in stages */
         pOutTimeInterval->scaledNanoseconds.hi = ~pOutTimeInterval->scaledNanoseconds.hi;
         pOutTimeInterval->scaledNanoseconds.lo = ~pOutTimeInterval->scaledNanoseconds.lo + 1;

         if (pOutTimeInterval->scaledNanoseconds.lo == 0)
         {
            pOutTimeInterval->scaledNanoseconds.hi++;
         }
      }
   }

   return status;
}  /* END zl303xx_PtpConvTimeStampToTimeInterval */

/*

  Function Name:
    zl303xx_PtpConvTimeIntervalToTimeStamp

  Details:
   Converts from PTP TimeStamp format (similar to TimeRepresentation in V1) to
   PTP TimeInterval format.

  Parameters:
   [in]  pInTimeInterval   TimeInterval value (in format signed 48-bit ns,
                                 16-bit fractional ns).

   [out} pOutTimeStamp      TimeStamp equivalent of the TimeInterval (in format
                                 48-bit sec, 32-bit ns; assume the sign bit is
                                 in bit-31 of ns field).

  Return Value:
   ZL303XX_OK                   The value was converted properly.
   ZL303XX_INVALID_POINTER      Either of the 2 input pointers is NULL.

  Notes:
   In the strictest sense, Timestamp format is always a positive value only (the
   nanosecondsField is Uint32T). However, TimeInterval is a signed value.
   Therefore, the upper-bit of the nanosecondsField of the Timestamp will be
   used to indicate the 'sign' of the value. This will be used only for internal
   purposes. All 'on-the-wire' values will be positive Timestamp values.
   (Note: the V1 TimeRepresentation used a Sint32T for the nanoseconds field to
   handle negative values).

*******************************************************************************/
zlStatusE zl303xx_PtpConvTimeIntervalToTimeStamp(
      zl303xx_TimeInterval *pInTimeInterval,
      zl303xx_TimeStamp *pOutTimeStamp)
{
   zlStatusE status = ZL303XX_OK;

   Uint64S tempInNs  = {0,0};    /* Copy of the input so it will not get changed. */
   zl303xx_BooleanE bNegativeInput = ZL303XX_FALSE;

   /* Check the input pointers. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pInTimeInterval) |
               ZL303XX_CHECK_POINTER(pOutTimeStamp);
   }

   /* No Range checking required since the output format is much larger than the
    * input can provide. */

   /* Perform the necessary math. */
   if (status == ZL303XX_OK)
   {
      /* If the input is Negative, get the absolute value.
       * (TimeInterval is in signed 2's complement format).  */
      if (pInTimeInterval->scaledNanoseconds.hi & 0x80000000)
      {
         bNegativeInput = ZL303XX_TRUE;

         /* Get the absolute value of the input. */
         tempInNs.hi = ~pInTimeInterval->scaledNanoseconds.hi;
         tempInNs.lo = ~pInTimeInterval->scaledNanoseconds.lo + 1;

         if (tempInNs.lo == 0)
         {
            tempInNs.hi++;
         }
      }
      else
      {
         tempInNs = pInTimeInterval->scaledNanoseconds;
      }

      /* Shift the value by 16-bits in 2 stages and do the rounding if necessary. */
      {
         /* Right-Shift the value by 15-bits. */
         tempInNs = RShift_U64S(tempInNs, 15);

         /* Add Half a second.. */
         tempInNs.lo++;
         if (tempInNs.lo == 0)
         {
            tempInNs.hi++;
         }

         /* Right-Shift the final by 1-bits. */
         tempInNs = RShift_U64S(tempInNs, 1);
      }

      /* Determine the number of seconds and nanoseconds. */
      pOutTimeStamp->secondsField = Div_U64S_U32(tempInNs, 1000000000, &pOutTimeStamp->nanosecondsField);

      /* Restore the sign of the value (if necessary).
       * (TimeStamp uses the 31st-bit of the nanosecondsField to mark the sign).  */
      if (bNegativeInput == ZL303XX_TRUE)
      {
         /* There is one exception here:
          * The TimeInterval input 0xFFFFFFFF:0xFFFFFFFF gets converted to -0.
          * Set the sign properly instead.  */
         if ((pOutTimeStamp->secondsField.hi != 0) ||
             (pOutTimeStamp->secondsField.lo != 0) ||
             (pOutTimeStamp->nanosecondsField != 0))
         {
            negateTime(pOutTimeStamp);
         }
      }
   }

   return status;
}  /* END zl303xx_PtpConvTimeIntervalToTimeStamp */

/*

  Function Name:
    zl303xx_PtpTimeIntervalAdd

  Details:
   Adds 2 Time Interval values. If an overflow occurs, then all 1's is returned
   as per Clause 13.3.2.7: of the IEEE-1588-2008 Std.

  Parameters:
   [in]  tIntvlA  Time Interval A (in format signed 48-bit ns, 16-bit fractional ns).
   [in]  tIntvlB  Time Interval B (in format signed 48-bit ns, 16-bit fractional ns).

  Return Value:
   zl303xx_TimeInterval
      - If the result is less than the maximum of zl303xx_TimeInterval datatype:
        (tIntvlA + tIntvlB)
      - If overflow occurs: 0xFFFFFFFF:FFFFFFFF

*******************************************************************************/
zl303xx_TimeInterval zl303xx_PtpTimeIntervalAdd(zl303xx_TimeInterval tIntvlA,
                                              zl303xx_TimeInterval tIntvlB)
{
   zl303xx_TimeInterval result = {{0,0}};

   /* Get the sign of the value */
   Sint8T tIntvlSignA = zl303xx_PtpTimeIntervalSignage(tIntvlA);
   Sint8T tIntvlSignB = zl303xx_PtpTimeIntervalSignage(tIntvlB);

   /* If either input is already in overflow or underflow, do nothing. */
   /* If both are already overflow/underflow, give priority to A but really it
    * might be indeterminate.    */
   if ((tIntvlSignA == PTP_TIME_INTERVAL_OVERFLOW) ||
       (tIntvlSignA == PTP_TIME_INTERVAL_UNDERFLOW))
   {
      result = tIntvlA;
   }
   else if ((tIntvlSignB == PTP_TIME_INTERVAL_OVERFLOW) ||
            (tIntvlSignB == PTP_TIME_INTERVAL_UNDERFLOW))
   {
      result = tIntvlB;
   }

   /* If either is 0 then just return the other. */
   else if (tIntvlSignA == PTP_TIME_INTERVAL_ZERO)
   {
      result = tIntvlB;
   }
   else if (tIntvlSignB == PTP_TIME_INTERVAL_ZERO)
   {
      result = tIntvlA;
   }

   /* Just add the two numbers and deal with the overflow after. */
   else
   {
      /* Add the inputs. */
      result.scaledNanoseconds = Add_U64S(tIntvlA.scaledNanoseconds,
                                          tIntvlB.scaledNanoseconds, NULL);

      /* If 2 Two's Complement numbers are added, and they both have the same
       * sign (both positive or both negative), then overflow occurs if and only
       * if the result has the opposite sign. Overflow never occurs when adding
       * operands with different signs. */

      /* If both were positive, then check for overflow. */
      if ((tIntvlSignA == PTP_TIME_INTERVAL_POSITIVE) && (tIntvlSignB == PTP_TIME_INTERVAL_POSITIVE))
      {
         /* If overflow occurred, map to the largest possible value. */
         if (PTP_TIME_INTERVAL_NEGATIVE == zl303xx_PtpTimeIntervalSignage(result))
         {
            result.scaledNanoseconds.hi = 0x7FFFFFFF;
            result.scaledNanoseconds.lo = 0xFFFFFFFF;
         }
      }

      /* If both were negative, then check for underflow. */
      else if ((tIntvlSignA == PTP_TIME_INTERVAL_NEGATIVE) && (tIntvlSignB == PTP_TIME_INTERVAL_NEGATIVE))
      {
         /* If overflow occurred, map to the largest possible value. */
         if (PTP_TIME_INTERVAL_POSITIVE == zl303xx_PtpTimeIntervalSignage(result))
         {
            result.scaledNanoseconds.hi = 0x80000000;
            result.scaledNanoseconds.lo = 0x00000000;
         }
      }
   }

   return result;
}

/*

  Function Name:
    zl303xx_PtpTimeIntervalDiff

  Details:
   Performs a simple subtraction operation on 2 Time Interval values.

  Parameters:
   [in]  tIntvlA     Time Interval A (in format signed 48-bit ns, 16-bit fractional ns).
   [in]  tIntvlB     Time Interval B (in format signed 48-bit ns, 16-bit fractional ns).

  Return Value:
   tIntvlA - tIntvlB

*******************************************************************************/
zl303xx_TimeInterval zl303xx_PtpTimeIntervalDiff(zl303xx_TimeInterval tIntvlA,
                                               zl303xx_TimeInterval tIntvlB)
{
   /* Negate the second value to give -tIntvlB */
   tIntvlB = zl303xx_PtpTimeIntervalNegate(tIntvlB);

   /* Now perform tIntvlA + (-tIntvlB) */
   return zl303xx_PtpTimeIntervalAdd(tIntvlA, tIntvlB);
}

/*

  Function Name:
    zl303xx_PtpTimeIntervalNegate

  Details:
   Performs a 2's complement operation on the provided TimeInterval (essentially
   does a (0 - TimeInterval) operation.

  Parameters:
   [in]  timeIntvl   Time Interval to negate.

  Return Value:
   If timeIntvl is not in an overflow or underflow condition: timeIntvl * (-1)
   If timeIntvl is in an overflow or underflow condition: the same condition.

*******************************************************************************/
zl303xx_TimeInterval zl303xx_PtpTimeIntervalNegate(zl303xx_TimeInterval timeIntvl)
{
   zl303xx_TimeInterval result = {{0,0}};

   /* Get the sign of the value */
   Sint8T timeIntvlSign = zl303xx_PtpTimeIntervalSignage(timeIntvl);

   /* If already in overflow or underflow, do not do anything. */
   if ((timeIntvlSign == PTP_TIME_INTERVAL_OVERFLOW) ||
       (timeIntvlSign == PTP_TIME_INTERVAL_UNDERFLOW))
   {
      result = timeIntvl;
   }

   /* A common situation will be that the input is 0. In this case, just
    * return 0 and avoid the math.  */
   else if (timeIntvlSign != PTP_TIME_INTERVAL_ZERO)
   {
      /* Inverse */
      result.scaledNanoseconds.hi = ~(timeIntvl.scaledNanoseconds.hi);
      result.scaledNanoseconds.lo = ~(timeIntvl.scaledNanoseconds.lo);

      /* Add 1 */
      result.scaledNanoseconds.lo++;   /* Will be 0. */

      /* Determine if adding 1 to result.LO requires a carry to result.HI. */
      if (timeIntvl.scaledNanoseconds.lo == 0)  /* means result.LO is now all F's */
      {
         result.scaledNanoseconds.hi++;
      }
   }

   return result;
}

/*

  Function Name:
    zl303xx_PtpTimeIntervalRShift

  Details:
   Performs a right-shift operation on the provided TimeInterval. Issues
   involved:
   - round up / down the shift.
   - maintain sign for negative values.
   - handle over-shift (64-bits or more).

  Parameters:
   [in]  timeIntvl   Time Interval to Right-Shift (divide).
   [in]  rRhift      Number of bits to shift right.

  Return Value:
   timeIntvl shifted by the number of rShift bits.

*******************************************************************************/
zl303xx_TimeInterval zl303xx_PtpTimeIntervalRShift(
      zl303xx_TimeInterval timeIntvl,
      Uint8T rRhift)
{
   zl303xx_TimeInterval result = {{0,0}};

   /* Get the sign of the value */
   Sint8T timeIntvlSign = zl303xx_PtpTimeIntervalSignage(timeIntvl);

   /* If already in overflow, underflow or zero, do not do anything. */
   if ((timeIntvlSign == PTP_TIME_INTERVAL_OVERFLOW) ||
       (timeIntvlSign == PTP_TIME_INTERVAL_UNDERFLOW) ||
       (timeIntvlSign == PTP_TIME_INTERVAL_ZERO))
   {
      result = timeIntvl;
   }

   else
   {
      /* Shift as though this were an UNSIGNED value */
      result.scaledNanoseconds = RShift_U64S(timeIntvl.scaledNanoseconds, rRhift);

      /* The only issue will be with negative values: these will require 1's to
       * be shifted back in from the top.  */
      if (timeIntvlSign == PTP_TIME_INTERVAL_NEGATIVE)
      {
         Uint64S negMask = {0xFFFFFFFF, 0xFFFFFFFF};

         /* Create the upper-1's mask by shifting left. */
         if (rRhift < 64)
         {
            negMask = LShift_U64S(negMask, (64 - rRhift));
         }

         /* Insert the required 1's at the front of the value to maintain signage. */
         result.scaledNanoseconds.hi |= negMask.hi;
         result.scaledNanoseconds.lo |= negMask.lo;

         /* As a test, verify the signage is still negative. */
         if (zl303xx_PtpTimeIntervalSignage(result) != PTP_TIME_INTERVAL_NEGATIVE)
         {
            ZL303XX_TRACE_ALWAYS(
                  "TimeInterval RShift: Signage not maintained: (0x%08X:%08X)(%d)",
                  timeIntvl.scaledNanoseconds.hi, timeIntvl.scaledNanoseconds.lo,
                  rRhift, 0,0,0);
            ZL303XX_TRACE_ALWAYS(
                  "    NegMask: (0x%08X:%08X))",
                  negMask.hi, negMask.lo, 0,0,0,0);
            ZL303XX_TRACE_ALWAYS(
                  "     Result: (0x%08X:%08X))",
                  result.scaledNanoseconds.hi, result.scaledNanoseconds.lo, 0,0,0,0);
         }
      }
   }

   return result;
}

/*

  Function Name:
    zl303xx_PtpTimeIntervalSignage

  Details:
   Determines the signage of a TimeInterval value.

  Parameters:
   [in]  timeIntvl   Time Interval determine the signage of.

  Return Value:
   +2: The TimeInterval value is in overflow condition.
   +1: The TimeInterval value is positive
    0: The TimeInterval value is zero
   -1: The TimeInterval value is negative
   -2: The TimeInterval value is in underflow condition.

*******************************************************************************/
Sint8T zl303xx_PtpTimeIntervalSignage(zl303xx_TimeInterval timeIntvl)
{
   /* Check for zero first (as this will be most common). */
   if ((timeIntvl.scaledNanoseconds.hi == 0) &&
       (timeIntvl.scaledNanoseconds.lo == 0))
   {
      return PTP_TIME_INTERVAL_ZERO;
   }

   /* Negative options. */
   else if (timeIntvl.scaledNanoseconds.hi & 0x80000000)
   {
      /* Underflow. */
      if ((timeIntvl.scaledNanoseconds.hi == 0x80000000) &&
          (timeIntvl.scaledNanoseconds.lo == 0x00000000))
      {
         return PTP_TIME_INTERVAL_UNDERFLOW;
      }

      /* Negative value.  */
      else
      {
         return PTP_TIME_INTERVAL_NEGATIVE;
      }
   }

   /* Positive options. */
   else
   {
      /* Overflow. */
      if ((timeIntvl.scaledNanoseconds.hi == 0x7FFFFFFF) &&
          (timeIntvl.scaledNanoseconds.lo == 0xFFFFFFFF))
      {
         return PTP_TIME_INTERVAL_OVERFLOW;
      }

      /* Positive value.  */
      else
      {
         return PTP_TIME_INTERVAL_POSITIVE;
      }
   }
}



/* zl303xx_PtpRxMsgIsEvent */
/**
   Determines if a PTP message is an event message. This is useful for a lower
   layer function to determine if the packet has been time stamped on ingress.

  Parameters:
   [in]   buffer      The PTP message buffer.
   [in]   bufferLen   Length of the PTP message.

  Return Value:
     ZL303XX_FALSE   The message is GENERAL type, or buffer is NULL, or
                         bufferLen is not valid, or message is corrupt.
     ZL303XX_TRUE    The message is EVENT type.

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtpRxMsgIsEvent(Uint8T *buffer, Uint16T bufferLen)
{
   Uint8T ptpVersion;

   /* Check input pointer. */
   if ((buffer == NULL) ||
       (bufferLen < 21))   /* v1 messageType @ byte 20 */
   {                       /* v2 messageType @ byte 0, but min header Length is 33 */
      return ZL303XX_FALSE;
   }

   /* Extract version number. */
   ptpVersion = buffer[1] & 0x0F;

   if (ptpVersion == 1)
   {
      if (buffer[20] == 1)
      {
         return ZL303XX_TRUE;
      }
   }
   else if (ptpVersion == 2)
   {
      return ZL303XX_PTP_MSG_ID_IS_EVENT(buffer[0]);
   }

   return ZL303XX_FALSE;
}

/* zl303xx_PtpRxMsgIsUnicast */
/**
   Returns the value of the flagField.unicastFlag in a PTPv2 packet.

  Parameters:
   [in]   buffer      Buffer containing the PTP packet.
   [in]   bufferLen   Length of buffer, in bytes.

  Return Value:
     ZL303XX_TRUE   The unicastFlag is set.
     ZL303XX_FALSE  The unicastFlag is not set, or this is not a valid
                        PTPv2 packet.

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtpRxMsgIsUnicast(Uint8T *buffer, Uint16T bufferLen)
{
   /* Check input parameters. */
   if (buffer && (bufferLen >= 8) &&
       ((buffer[1] & 0x0F) == 2))   /* unicastFlag is only available in v2 messages. */
   {
      return ((buffer[6] & 0x04) == 0) ? ZL303XX_FALSE : ZL303XX_TRUE;
   }

   return ZL303XX_FALSE;
}

/* zl303xx_PtpPortAddressLengthGet */
/**
   Returns the value to be used in zl303xx_PortAddress::addressLength for a
   given network protocol.

  Parameters:
   [in]   protocol   Network protocol to return the length of.

  Return Value:  0  protocol is invalid. Otherwise a non-zero value is returned.

*******************************************************************************/
Uint16T zl303xx_PtpPortAddressLengthGet(zl303xx_NetworkProtocolE protocol)
{
   if (protocol <= ZL303XX_NETWORK_PROTOCOL_PROFINET)
   {
      return Zl303xx_PtpPortAddrLen[protocol];
   }
   else
   {
      return 0;
   }
}

/* zl303xx_PtpPortStateToStr */
/**

   Converts a Port State enum value to a string.

  Parameters:
   [in]   state   State to convert.

  Return Value:  (const char *)  The Port State as a string.

*******************************************************************************/
const char *zl303xx_PtpPortStateToStr(zl303xx_PortStateE state)
{
   /* If the state is not valid, use the last index of the array. */
   if (state > ZL303XX_PORT_STATE_SLAVE)
   {
      state = ZL303XX_ARRAY_SIZE(Zl303xx_PtpPortStateStr) - 1;
   }

   return Zl303xx_PtpPortStateStr[state];
}

/*****************   INTERNAL PTP FUNCTION DEFINITIONS   **********************/

/* zl303xx_PtpComparePortAddress */
/** 

   Compares two PTP port addresses.

  Parameters:
   [in]   a  First address in comparison.
   [in]   b  Second address in comparison.

  Return Value:
     0  Addresses match.
     1  Addresses have different zl303xx_PortAddress::networkProtocol values.
     2  Addresses have different zl303xx_PortAddress::addressLength values.
     3  Addresses have different zl303xx_PortAddress::addressField values.

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
Uint16T zl303xx_PtpComparePortAddress(const zl303xx_PortAddress *a,
                                      const zl303xx_PortAddress *b)
{
   /* Compare the network protocol. */
   if (a->networkProtocol != b->networkProtocol)
   {
      return 1;
   }

   /* Compare the address lengths. */
   if (a->addressLength != b->addressLength)
   {
      return 2;
   }

   /* Compare the actual addresses. */
   if (memcmp(a->addressField, b->addressField, a->addressLength) != 0)
   {
      return 3;
   }

   /* Addresses are equal. */
   return 0;
}

/* zl303xx_PtpTracePortAddress */
/** 

   Logs the contents of a zl303xx_PortAddress structure to console. This function
   should not be called directly, but rather wrapped with a ZL303XX_DEBUG_FILTER()
   to avoid this function call if the log level is not high enough.

  Parameters:
   [in]   portAddr   Port address structure to log.

  Return Value:  N/A

  Notes:   Assumes all input pointers are valid.

*******************************************************************************/
void zl303xx_PtpTracePortAddress(zl303xx_PortAddress *portAddr)
{
   switch(portAddr->networkProtocol)
   {
      case ZL303XX_NETWORK_PROTOCOL_UDP_IPv4 :
      {
         ZL303XX_TRACE_ALWAYS("   portAddress=0x%08lX (%s)",
            ntohl(*(Uint32T *)(void *)(portAddr->addressField)),
            Zl303xx_PtpPortAddrStr[portAddr->networkProtocol], 0,0,0,0);
         break;
      }

      case ZL303XX_NETWORK_PROTOCOL_UDP_IPv6 :
      {
         ZL303XX_TRACE_ALWAYS("   portAddress=0x%08lX.%08lX.%08lX.%08lX (%s)",
            ntohl(*(Uint32T *)(void *)(portAddr->addressField)),
            ntohl(*(Uint32T *)(void *)(portAddr->addressField + 4)),
            ntohl(*(Uint32T *)(void *)(portAddr->addressField + 8)),
            ntohl(*(Uint32T *)(void *)(portAddr->addressField + 12)),
            Zl303xx_PtpPortAddrStr[portAddr->networkProtocol], 0);
         break;
      }

      case ZL303XX_NETWORK_PROTOCOL_802_3 :
      case ZL303XX_NETWORK_PROTOCOL_PROFINET :
      {
         ZL303XX_TRACE_ALWAYS("   portAddress=0x%04lX.%04lX.%04lX (%s)",
            ntohl(*(Uint32T *)(void *)(portAddr->addressField)),
            ntohs(*(Uint32T *)(void *)(portAddr->addressField + 4)),
            Zl303xx_PtpPortAddrStr[portAddr->networkProtocol], 0,0,0);
         break;
      }

      case ZL303XX_NETWORK_PROTOCOL_DEVICENET :
      case ZL303XX_NETWORK_PROTOCOL_CONTROLNET :
      {
         ZL303XX_TRACE_ALWAYS("   portAddress=0x%04lX.%04lX (%s)",
            ntohl(*(Uint32T *)(void *)(portAddr->addressField)),
            Zl303xx_PtpPortAddrStr[portAddr->networkProtocol], 0,0,0,0);
         break;
      }

      case   ZL303XX_NETWORK_PROTOCOL_INVALID :             /* fall-through for removal of switch-enum warning */

      default :
         ZL303XX_TRACE_ALWAYS("   portAddress transport type is UNKNOWN.", 0,0,0,0,0,0);
         break;
   }
}

/* zl303xx_PtpTraceComparisonData */
/** 

   Logs the contents of a zl303xx_PtpBmcCompareDataS structure to console.
   This function should not be called directly, but rather wrapped with a
   ZL303XX_DEBUG_FILTER() to avoid this function call if the log level is not
   high enough.

  Parameters:
   [in]  pCompareData   Comparison Data to display.

  Return Value:  N/A

  Notes:   Assumes all input pointers are valid.

*******************************************************************************/
void zl303xx_PtpTraceComparisonData(zl303xx_PtpBmcCompareDataS *pCompareData)
{
   /* Unfortunate but necessary: since the TRACE output is so slow, this
    * function ends before the output is displayed and by that time any local
    * data is destroyed. So, keep a static string for each clockId needed. */
   static char gmClockIdA[CLOCK_ID_STR_LEN];
   static char rxPortIdA[PORT_ID_STR_LEN];
   static char txPortIdA[PORT_ID_STR_LEN];

   /* Convert all necessary clockIdentities to strings. */
   zl303xx_PtpV2ClockIdentityToString(pCompareData->grandmasterIdentity, gmClockIdA);
   zl303xx_PtpV2PortIdentityToString(&pCompareData->receivingPortIdentity, rxPortIdA);
   zl303xx_PtpV2PortIdentityToString(&pCompareData->foreignPortIdentity, txPortIdA);

   /* The Announce Data. */
   ZL303XX_TRACE_ALWAYS("BMC Data Set Comparison", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("**********************************", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("   MEMBER                  ANNOUNCE-A", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("  ********                 ************", 0,0,0,0,0,0);

   ZL303XX_TRACE_ALWAYS(" receivingPortIdentity   : %s",    rxPortIdA, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS(" sourcePortIdentity      : %s",    txPortIdA, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS(" grandmasterIdentity     : %s",    gmClockIdA, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS(" grandmasterPriority1    : %-40d", pCompareData->grandmasterPriority1, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS(" grandmasterPriority2    : %-40d", pCompareData->grandmasterPriority2, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS(" stepsRemoved            : %-40d", pCompareData->stepsRemoved, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS(" currentUtcOffset        : %-40d", pCompareData->timeProperties.currentUtcOffset, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS(" timeSource              : %-40d", pCompareData->timeProperties.timeSource, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS(" grandmasterClockQuality :", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("                 (Class) : %-40d", pCompareData->grandmasterClockQuality.clockClass, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("              (Accuracy) : 0x%-38X", pCompareData->grandmasterClockQuality.clockAccuracy, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("     (scaledLogVariance) : 0x%-38X", (Uint32T)pCompareData->grandmasterClockQuality.offsetScaledLogVariance, 0,0,0,0,0);

   /* Blank Line for Separation. */
   ZL303XX_TRACE_ALWAYS("  ", 0,0,0,0,0,0);
}

/* zl303xx_PtpTraceParentDataSet */
/** 

   Logs the contents of a zl303xx_ParentDS structure to console.
   This function should not be called directly, but rather wrapped with a
   ZL303XX_DEBUG_FILTER() to avoid this function call if the log level is not
   high enough.

  Parameters:
   [in]  pParentDS   Comparison Data to display.

  Return Value:  N/A

  Notes:   Assumes all input pointers are valid.

*******************************************************************************/
void zl303xx_PtpTraceParentDataSet(zl303xx_ParentDS *pParentDS)
{
   /* Unfortunate but necessary: since the TRACE output is so slow, this
    * function ends before the output is displayed and by that time any local
    * data is destroyed. So, keep a static string for each Id needed. */
   static char gmClockId[CLOCK_ID_STR_LEN];
   static char parentPortId[PORT_ID_STR_LEN];

   /* parentPortIdentity */
   zl303xx_PtpV2PortIdentityToString(&pParentDS->parentPortIdentity, parentPortId);
   ZL303XX_TRACE_ALWAYS("Parent port identity: %s", parentPortId, 0,0,0,0,0);

   /* Parent Statistics */

   ZL303XX_TRACE_ALWAYS("Parent statistics valid: %s",
         ZL303XX_BOOL_TO_STR(pParentDS->parentStats), 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS ("   Observed parent offset scaled log variance: %d",
         pParentDS->observedParentOffsetScaledLogVariance, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS ("   Observed parent clock phase change rate   : %d",
         (SINT_T)pParentDS->observedParentClockPhaseChangeRate, 0,0,0,0,0);

   /* Grandmaster fields. */
   zl303xx_PtpV2ClockIdentityToString(pParentDS->grandmasterIdentity, gmClockId);
   ZL303XX_TRACE_ALWAYS("Grandmaster data:", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("   identity: %s", gmClockId, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("   quality class:    %u",
         pParentDS->grandmasterClockQuality.clockClass, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("   quality accuracy: 0x%02X",
         pParentDS->grandmasterClockQuality.clockAccuracy, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("   clock quality offset scaled log variance: 0x%04X",
         pParentDS->grandmasterClockQuality.offsetScaledLogVariance, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("   priority 1: %u",
         pParentDS->grandmasterPriority1, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("   priority 2: %u",
         pParentDS->grandmasterPriority2, 0,0,0,0,0);

   /* Blank Line for Separation. */
   ZL303XX_TRACE_ALWAYS("  ", 0,0,0,0,0,0);
}

/****************   CLOCK & PORT IDENTITY EVALUATION ROUTINES  ***************/

/*

  Function Name:
    zl303xx_PtpV2EqualClockIdentities

  Details:
   Determines whether two clock identities are equal or not.
   This excludes the port number of the identity from the comparison

  Parameters:
   [in]    first         Pointer to first clock identity
   [in]    second        Pointer to second clock identity


  Return Value:
   zl303xx_BooleanE

  Notes:
   Assumes that pointers have been verified.

 *****************************************************************************/
zl303xx_BooleanE zl303xx_PtpV2EqualClockIdentities(zl303xx_ClockIdentity first,
                                                 zl303xx_ClockIdentity second)
{
   if (memcmp(first, second, PTP_V2_CLK_IDENTITY_LENGTH) == 0)
   {
      return ZL303XX_TRUE;  /* identities are equal */
   }
   else
   {

      return ZL303XX_FALSE;
   }
}

/*

  Function Name:
    zl303xx_PtpV2IsAllClockIdentity

  Details:
   Determines whether the specified Clock identity is all 1's

  Parameters:
   [in]  pClockIdentity   Pointer to clock identity to evaluate

  Return Value:
   zl303xx_BooleanE

  Notes:
   This is used to detect a special case for management & signaling messages
   where the message should be applied to all clocks.

 *******************************************************************************/
zl303xx_BooleanE zl303xx_PtpV2IsAllClockIdentity(zl303xx_ClockIdentity pClockIdentity)
{
   /* IEEE-1588-2008 specifically defines the All-Clock Identity as all 1's.  */
   zl303xx_ClockIdentity allClockIdentity;
   OS_MEMSET(allClockIdentity, 0xFF, PTP_V2_CLK_IDENTITY_LENGTH);

   /* Check if the input is all 1's. */
   if (memcmp(allClockIdentity, pClockIdentity, PTP_V2_CLK_IDENTITY_LENGTH) == 0)
   {
      return ZL303XX_TRUE;
   }
   else
   {
      return ZL303XX_FALSE;
   }
}

/*

  Function Name:
    zl303xx_PtpV2EqualPortIdentities

  Details:
   Determines whether two clock port identities are equal or not.

  Parameters:
   [in]    first         Pointer to first port identity
   [in]    second        Pointer to second port identity


  Return Value:
   zl303xx_BooleanE

  Notes:
   Assumes that pointers have been verified.

 *****************************************************************************/
zl303xx_BooleanE zl303xx_PtpV2EqualPortIdentities(zl303xx_PortIdentity *first,
                                                zl303xx_PortIdentity *second)
{
   if ((first->portNumber == second->portNumber) &&
       (zl303xx_PtpV2EqualClockIdentities(first->clockIdentity, second->clockIdentity) == ZL303XX_TRUE))
   {
      return ZL303XX_TRUE;  /* identities are equal */
   }
   else
   {
      return ZL303XX_FALSE;
   }
}

/* ZL303XX_TRACE_V2_CLOCK_ID */
/** 

   Displays a clock identity value for PTP version 2. Intentionally made to
   resemble other TRACE calls.

  Parameters:
   [in]  modId          Module ID from which the trace is called. Used to
                              determine if the trace will be executed based on
                              the trace level.
   [in]  level          The trace level for the module specified.
   [in]  label          A prefix (label) string to display. Can be empty but
                              not a null pointer.
   [in]  clockIdentity  The clock identity to display.

  Return Value:
   Nothing

 *****************************************************************************/
void ZL303XX_TRACE_V2_CLOCK_ID(Uint32T modId, Uint8T level,
                               const char *label,
                               zl303xx_ClockIdentity clockIdentity)
{
   /* Use the FILTER/TRACE_ALWAYS combination to avoid processing the string
    * if the trace level is set too low.    */
   ZL303XX_DEBUG_FILTER(modId, level)
   {
      static char clkIdStr[CLOCK_ID_STR_LEN];

      (void)zl303xx_PtpV2ClockIdentityToString(clockIdentity, clkIdStr);
      ZL303XX_TRACE_ALWAYS("%s...", label, 0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("%s", clkIdStr, 0,0,0,0,0);
   }
}

/* ZL303XX_TRACE_V2_PORT_ID */
/** 

   Displays a port identity value for PTP version 2. Intentionally made to
   resemble other TRACE calls.

  Parameters:
   [in]  modId          Module ID from which the trace is called. Used to
                              determine if the trace will be executed based on
                              the trace level.
   [in]  level          The trace level for the module specified.
   [in]  label          A prefix (label) string to display. Can be empty but
                              not a null pointer.
   [in]  portIdentity   The port identity to display.

  Return Value:
   Nothing

 *****************************************************************************/
void ZL303XX_TRACE_V2_PORT_ID(Uint32T modId, Uint8T level,
                              const char *label,
                              zl303xx_PortIdentity *portIdentity)
{
   /* Use the FILTER/TRACE_ALWAYS combination to avoid processing the string
    * if the trace level is set too low.    */
   ZL303XX_DEBUG_FILTER(modId, level)
   {
      static char portIdStr[PORT_ID_STR_LEN];

      (void)zl303xx_PtpV2PortIdentityToString(portIdentity, portIdStr);
      ZL303XX_TRACE_ALWAYS("%s...", label, 0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("%s", portIdStr, 0,0,0,0,0);
   }
}

/* ZL303XX_TRACE_V2_TIMESTAMP */
/** 

   Displays a Timestamp value for PTP version 2. Intentionally made to
   resemble other TRACE calls.

  Parameters:
   [in]  modId          Module ID from which the trace is called. Used to
                              determine if the trace will be executed based on
                              the trace level.
   [in]  level          The trace level for the module specified.
   [in]  streamId       The PTP stream associated with the timestamp.
   [in]  label          A prefix (label) string to display. Can be empty but
                              not a null pointer.
   [in]  timestamp      The timestamp to display.

  Return Value:
   Nothing

 *****************************************************************************/
void ZL303XX_TRACE_V2_TIMESTAMP(Uint32T modId, Uint8T level,
      Uint32T streamId, const char *label, zl303xx_TimeStamp *timestamp)
{
   /* Use the FILTER/TRACE_ALWAYS combination to avoid processing the string
    * if the trace level is set too low.    */
   ZL303XX_DEBUG_FILTER(modId, level)
   {
      ZL303XX_TRACE_ALWAYS("Stream %u: %s: " PTPD_FORMAT_TIME_STR,
            streamId, label,
            formatTimeStringSign(timestamp),
            formatTimeStringSec(timestamp),
            formatTimeStringNs(timestamp), 0);
   }
}

/* zl303xx_PtpClockTimeGet */
/** 

   Wrapper for the CLOCK_TIME_GET hardware command.

  Parameters:
   [in]   pPtpClock   Pointer to the clock being queried.
   [in]   interface   If this function is being called to fill in all or portions
                           of the time stamp field for an EVENT message, this
                           must be ZL303XX_PTP_INTERFACE_EVENT. Otherwise, call
                           with ZL303XX_PTP_INTERFACE_GENERAL.
   [out]  todTime     Estimate of the current clock time.

*******************************************************************************/
zlStatusE zl303xx_PtpClockTimeGet(zl303xx_PtpClockS *pPtpClock,
                                  zl303xx_PtpInterfaceE interface,
                                  zl303xx_TimeStamp *todTime)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4, "zl303xx_PtpClockTimeGet:", 0,0,0,0,0,0);

   /* Check input parameters. */
   status = zl303xx_PtpHwCmdFnCheck(pPtpClock);

   if (status == ZL303XX_OK)
   {
      Sint32T hwStatus = 0;
      zl303xx_PtpHwClockTimeGetS cmdParams;

      cmdParams.input.clockHandle = pPtpClock->clockHandle;
      cmdParams.input.clockExtData = pPtpClock->config.extData;
      cmdParams.input.twoStepFlag = pPtpClock->config.defaultDS.twoStepFlag;
      cmdParams.input.interface = interface;

      if ((hwStatus = pPtpClock->config.hwCmdFn(ZL303XX_PTP_HW_CMD_CLOCK_TIME_GET,
                                            (void *)&cmdParams)) == 0)
      {
         *todTime = cmdParams.output.ptpTime;
      }
      else
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                       "   (ERROR) ZL303XX_PTP_HW_CMD_CLOCK_TIME_GET handler failed with status = %ld.",
                       hwStatus, 0,0,0,0,0);
         status = ZL303XX_EXT_API_CALL_FAIL;
      }
   }

   return status;
}

/* zl303xx_PtpClockTimeSet */
/** 

   Wrapper for the CLOCK_TIME_SET hardware command.

  Parameters:
   [in]   pPtpClock  Pointer to the clock that will have it's time modified.
   [in]   todTime    New time to set on the clock.

*******************************************************************************/
zlStatusE zl303xx_PtpClockTimeSet(zl303xx_PtpClockS *pPtpClock, zl303xx_TimeStamp *todTime)
{
   zlStatusE status = ZL303XX_OK;
   Sint32T hwStatus = 0;
   zl303xx_PtpHwClockTimeSetS cmdParams;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "zl303xx_PtpClockTimeSet:", 0,0,0,0,0,0);

   /* Check input parameters. */
   status = zl303xx_PtpHwCmdFnCheck(pPtpClock);

   if (status == ZL303XX_OK)
   {
      cmdParams.input.clockHandle  = pPtpClock->clockHandle;
      cmdParams.input.clockExtData = pPtpClock->config.extData;
      cmdParams.input.ptpTime      = *todTime;

      if ((hwStatus = pPtpClock->config.hwCmdFn(ZL303XX_PTP_HW_CMD_CLOCK_TIME_SET,
                                            (void *)&cmdParams)) != 0)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                       "   (ERROR) ZL303XX_PTP_HW_CMD_CLOCK_TIME_SET handler failed with status = %ld.",
                       hwStatus, 0,0,0,0,0);
         status = ZL303XX_EXT_API_CALL_FAIL;
      }
   }

   return status;
}

/* zl303xx_PtpPortPhysicalAddrGet */
/** 

   Retrieves the physical address of a the hardware port that a PTP port is
   associated with.

  Parameters:
   [in]   port                   Pointer to the port being queried.
   [out]  physicalLayerProtocol  Constant string that identifies the network
                                      protocol in use on this communications path.
   [out]  physicalAddressLength  Length of the physical layer address.
   [out]  physicalAddress        Octet array containing the physical layer
                                      address.

*******************************************************************************/
zlStatusE zl303xx_PtpPortPhysicalAddrGet(zl303xx_PortDataS *port, char *physicalLayerProtocol,
                                         Uint16T *physicalAddressLength, Uint8T *physicalAddress)
{
   zlStatusE status = ZL303XX_OK;
   Sint32T hwStatus = 0;
   zl303xx_PtpHwPhysAddrGetS cmdParams;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "zl303xx_PtpPortPhysicalAddrGet:", 0,0,0,0,0,0);

   /* Check input parameters. */
   status = zl303xx_PtpHwCmdFnCheck(port->clock);

   if (status == ZL303XX_OK)
   {
      cmdParams.input.portHandle = port->clockPortHandle;
      cmdParams.input.portExtData = port->config.extData;

      if ((hwStatus = port->clock->config.hwCmdFn(ZL303XX_PTP_HW_CMD_PHYS_ADDR_GET,
                                                  (void *)&cmdParams)) == 0)
      {
         strncpy(physicalLayerProtocol, cmdParams.output.physicalLayerProtocol,
                 sizeof(cmdParams.output.physicalLayerProtocol) - 1);
         *physicalAddressLength = cmdParams.output.physicalAddressLength;
         OS_MEMCPY(physicalAddress, cmdParams.output.physicalAddress,
                cmdParams.output.physicalAddressLength);
      }
      else
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                       "   (ERROR) ZL303XX_PTP_HW_CMD_PHYSICAL_ADDR_GET handler failed with status = %ld.",
                       hwStatus, 0,0,0,0,0);
         status = ZL303XX_EXT_API_CALL_FAIL;
      }
   }

   return status;
}

/* zl303xx_PtpPortLogFaultyState */
/**
   Logs the reason if a PTP Port goes into faulty, also fires
   a ZL303XX_PTP_EVENT_PORT_FAULTY event

  Parameters:
   [in]   txError      The transmit error which caused the fault.
   [in]   faultType     The fault type to send the port into faulty.
   [in]   portData      Pointer to port data which went into faulty

*******************************************************************************/
void zl303xx_PtpPortLogFaultyState(zl303xx_PtpTxStatusE txError, zl303xx_PtpPortFaultTypeE faultType,
                                 zl303xx_PortDataS *portData)
{
   if(portData)
   {
      portData->lastFaultType = faultType;
      portData->lastTxError = txError;

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "Port number %d faulty",
            portData->config.portNumber, 0,0,0,0,0);

      if(faultType < ZL303XX_PTP_PORT_FAULT_TYPE_MAX)
      {
         portData->lastFaultString = Zl303xx_PtpPortFaultTypeStr[faultType];
         if (portData->lastFaultString != NULL)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "Last error: %s",
                  portData->lastFaultString, 0,0,0,0,0);
         }
      }
      else
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "Unknown Error Type %d",
                    faultType, 0,0,0,0,0);
      }

      if ((portData->clock != NULL)
         && (portData->clock->config.eventCallback != NULL))
      { 
         zl303xx_PtpEventPortFaultS eventData;
         eventData.portHandle = portData->clockPortHandle;
         eventData.portExtData = portData->config.extData;
         eventData.portFaultType = faultType;
         eventData.txError = txError;
         portData->clock->config.eventCallback(ZL303XX_PTP_EVENT_PORT_FAULTY,
                                               &eventData);
      }
   }
}

/* zl303xx_PtpClockLockStatusGet */
/** 

   Wrapper for the LOCK_STATUS_GET hardware command.

  Parameters:
   [in]   pPtpClock     Pointer to the clock being queried.
   [out]  lockStatus    Current clock lock status.

*******************************************************************************/
zlStatusE zl303xx_PtpClockLockStatusGet(zl303xx_PtpClockS *pPtpClock,
                                        Uint32T *lockStatus)
{
   zlStatusE status = ZL303XX_OK;
   Sint32T hwStatus = 0;
   zl303xx_PtpHwLockStatusGetS cmdParams;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4, "zl303xx_PtpClockLockStatusGet:", 0,0,0,0,0,0);

   /* Check input parameters. */
   status = zl303xx_PtpHwCmdFnCheck(pPtpClock);

   if (status == ZL303XX_OK)
   {
      cmdParams.input.clockHandle = pPtpClock->clockHandle;
      cmdParams.input.clockExtData = pPtpClock->config.extData;
      cmdParams.input.profile = pPtpClock->config.profile;

      /* Some profiles require a PHASE Lock. */
      if ((pPtpClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_1v1) ||
          (pPtpClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_1v2) ||
          (pPtpClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_2))
      {
         cmdParams.input.phaseRequired = ZL303XX_TRUE;
      }
      else
      {
         cmdParams.input.phaseRequired = ZL303XX_FALSE;
      }

      /* Call the external interface */
      if ((hwStatus = pPtpClock->config.hwCmdFn(ZL303XX_PTP_HW_CMD_LOCK_STATUS_GET,
                                                  (void *)&cmdParams)) == 0)
      {
         *lockStatus = cmdParams.output.lockStatus;
      }
      else
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "   (ERROR) ZL303XX_PTP_HW_CMD_LOCK_STATUS_GET handler failed with status = %ld.",
               hwStatus, 0,0,0,0,0);
         status = ZL303XX_EXT_API_CALL_FAIL;
      }
   }

   return status;
}

/* zl303xx_PtpClockStabilityStatusGet */
/** 

   Wrapper for the CLOCK_STABILITY_GET hardware command.

  Parameters:
   [in]   pPtpClock        Pointer to the clock being queried.
   [out]  freqStability    Clock frequency stability.
   [out]  phaseStability   Clock phase stability.

*******************************************************************************/
zlStatusE zl303xx_PtpClockStabilityStatusGet(zl303xx_PtpClockS *pPtpClock,
                                             Uint32T *freqStability,
                                             Sint32T *phaseStability)
{
   zlStatusE status = ZL303XX_OK;
   Sint32T hwStatus = 0;
   zl303xx_PtpHwClockStabilityGetS cmdParams;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4, "zl303xx_PtpClockStabilityStatusGet:", 0,0,0,0,0,0);

   /* Check input parameters. */
   status = zl303xx_PtpHwCmdFnCheck(pPtpClock);

   if (status == ZL303XX_OK)
   {
      cmdParams.input.clockHandle = pPtpClock->clockHandle;
      cmdParams.input.clockExtData = pPtpClock->config.extData;

      if ((hwStatus = pPtpClock->config.hwCmdFn(ZL303XX_PTP_HW_CMD_CLOCK_STABILITY_GET,
                                                  (void *)&cmdParams)) == 0)
      {
         *freqStability = cmdParams.output.freqStability;
         *phaseStability = cmdParams.output.phaseStability;
      }
      else
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "   (ERROR) ZL303XX_PTP_HW_CMD_CLOCK_STABILITY_GET handler failed with status = %ld.",
               hwStatus, 0,0,0,0,0);
         status = ZL303XX_EXT_API_CALL_FAIL;
      }
   }

   return status;
}

/* zl303xx_PtpClockTimeSetStatusGet */
/** 

   Wrapper for the TIMESET_STATUS_GET hardware command.

  Parameters:
   [in]   pPtpClock        Pointer to the clock being queried.
   [out]  timeSetStatus    Current hardware Time-Set status for this
                                 clock: TRUE  = ToD has been set.
                                        FALSE = ToD not set.

*******************************************************************************/
zlStatusE zl303xx_PtpClockTimeSetStatusGet(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_BooleanE *timeSetStatus)
{
   zlStatusE status = ZL303XX_OK;
   Sint32T hwStatus = 0;
   zl303xx_PtpHwTimeStatusGetS cmdParams;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4,
         "zl303xx_PtpClockTimeSetStatusGet:", 0,0,0,0,0,0);

   /* Check input parameters. */
   status = zl303xx_PtpHwCmdFnCheck(pPtpClock);

   if (status == ZL303XX_OK)
   {
      cmdParams.input.clockHandle = pPtpClock->clockHandle;
      cmdParams.input.clockExtData = pPtpClock->config.extData;

      if ((hwStatus = pPtpClock->config.hwCmdFn(ZL303XX_PTP_HW_CMD_TIMESET_STATUS_GET,
                                                  (void *)&cmdParams)) == 0)
      {
         *timeSetStatus = cmdParams.output.bTimeSet;
      }
      else
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "   (ERROR) TIMESET_STATUS_GET handler failed with status = %ld.",
               hwStatus, 0,0,0,0,0);
         status = ZL303XX_EXT_API_CALL_FAIL;
      }
   }

   return status;
}

/* zl303xx_PtpClockPerformanceGet */
/** 

   Wrapper for the ZL303XX_PTP_HW_CMD_PLL_PERF_DATA_GET hardware command.

  Parameters:
   [in]   pPtpClock     Pointer to the clock being queried.

   [out]  pPllPerfData  Data structure containing performance data about
                              the current operation of the PLL.

*******************************************************************************/
zlStatusE zl303xx_PtpClockPerformanceGet(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpPllPerformanceDataS *pPllPerfData)
{
   zlStatusE status = ZL303XX_OK;
   Sint32T hwStatus = 0;
   zl303xx_PtpHwPllPerformanceGetS cmdParams;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4,
         "zl303xx_PtpClockPerformanceGet:", 0,0,0,0,0,0);

   /* Check input parameters. */
   status = zl303xx_PtpHwCmdFnCheck(pPtpClock);

   if (status == ZL303XX_OK)
   {
      cmdParams.input.clockHandle = pPtpClock->clockHandle;
      cmdParams.input.clockExtData = pPtpClock->config.extData;

      if ((hwStatus = pPtpClock->config.hwCmdFn(ZL303XX_PTP_HW_CMD_PLL_PERF_DATA_GET,
                                                  (void *)&cmdParams)) == 0)
      {
         *pPllPerfData = cmdParams.output.pllPerfData;
      }
      else
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "   (ERROR) PLL_PERF_DATA_GET handler failed with status = %ld.",
               hwStatus, 0,0,0,0,0);
         status = ZL303XX_EXT_API_CALL_FAIL;
      }
   }

   return status;
}

/* zl303xx_PtpTxMsg */
/** 

   Gathers the data required to send a PTP message and calls the user's TX
   function.

  Parameters:
   [in]   buffer     Octet buffer containing the PTP message to send.
   [in]   bufLen     Length of the octet buffer.
   [in]   ifType     The interface this packet is to be sent on (EVENT or GENERAL).
   [in]   port       Pointer to the port this message is being transmitted from.
   [in]   destAddr   Destination port address of this message. Will be NULL if
                          a stream pointer is provided.
   [in]   stream     Pointer to the stream that is transmitting this message.
                          Will be NULL if the message is not associated with any stream.
   [in]   messageType  Value of the messageType field in the PTP header.

*******************************************************************************/
zl303xx_PtpTxStatusE zl303xx_PtpTxMsg(Uint8T *buffer, Uint16T bufLen, zl303xx_PtpInterfaceE ifType,
                           zl303xx_PortDataS *port, zl303xx_PortAddress *destAddr,
                           zl303xx_ClockStreamS *stream, zl303xx_MessageTypeE messageType)
{
   zl303xx_PtpTxStatusE status = ZL303XX_PTP_TX_OK;
   zl303xx_PtpTxMsgDataS txMsg;
   Sint32T txFuncStatus = -1;

   if (status == ZL303XX_PTP_TX_OK)
   {
#ifdef ENFORCE_STRICT_TX_CHECKS
      /* Check the transmit buffer pointer */
      if (buffer == NULL)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Data Buffer is NULL.", 0,0,0,0,0,0);
         status = ZL303XX_PTP_TX_MSG_BUFFER_DATA_NULL;
         return status;
      }
      /* Make sure the buffer is of the right size */
      if (bufLen > PTP_MSG_SIZE_MAX)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Message too big to pack.", 0,0,0,0,0,0);
         status = ZL303XX_PTP_TX_MSG_TOO_BIG;
         return status;
      }
      if (stream != NULL)
      {
         /* Stream has to be in the right state */
         if ((stream->operState < ZL303XX_PTP_STREAM_OPER_STATE_DISABLED)
               || (stream->operState > ZL303XX_PTP_STREAM_OPER_STATE_MONITORING))
         {
            ZL303XX_TRACE_ERROR("   (ERROR) Stream State out of range.", 0,0,0,0,0,0);
            status = ZL303XX_PTP_TX_STREAM_STATE_OUT_OF_RANGE;
            return status;
         }
      }
      if (port == NULL)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Port is NULL.", 0,0,0,0,0,0);
         status = ZL303XX_PTP_TX_PORT_NULL;
         return status;
      }
#endif
      /* Don't send if clock TX is disabled. */
      if (port->clock->enableRxTx == ZL303XX_FALSE)
      {
         return status;
      }

      /* Some sanity checks on the transmit data */
      /* Make sure there is a TX Message function bound to the stack */
      if (port->config.txMsgFn == NULL)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) No message transmit function defined.", 0,0,0,0,0,0);
         status = ZL303XX_PTP_TX_MSG_BINDING_NULL;
         return status;
      }

   }


   if (status == ZL303XX_PTP_TX_OK)
   {
      txMsg.portHandle   = port->clockPortHandle;
      txMsg.clockExtData = port->clock->config.extData;
      txMsg.portExtData  = port->config.extData;
      txMsg.srcAddr      = port->config.localAddr;
      txMsg.interface    = ifType;
      txMsg.destAddr     = (destAddr != NULL) ? *destAddr : stream->config.destAddr;
      txMsg.messageId    = messageType;

#ifdef ENFORCE_STRICT_TX_CHECKS
      if (txMsg.destAddr.networkProtocol != ZL303XX_NETWORK_PROTOCOL_UDP_IPv6
            && txMsg.destAddr.networkProtocol != ZL303XX_NETWORK_PROTOCOL_UDP_IPv4
            && txMsg.destAddr.networkProtocol != ZL303XX_NETWORK_PROTOCOL_802_3)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Network Protocol is not Ipv6, ipv4 or 802.3", 0,0,0,0,0,0);
         status = ZL303XX_PTP_TX_SOCKET_UNSUPPORTED_OPERATION;
         return status;
      }
#endif
      /* Grab the sequenceId out of the message buffer. The offset (30 bytes)
       * is the same for v1 and v2. */
      txMsg.sequenceId = zl303xx_PtpV2MsgSequenceNumGet(buffer);
      /* Increment the sequenceId for required messageTypes */
      if (stream != NULL)
      {
         const Uint32T seqIdList = (1 << ZL303XX_MSG_ID_SYNC) |
                                   (1 << ZL303XX_MSG_ID_DELAY_REQ) |
                                   (1 << ZL303XX_MSG_ID_PEER_DELAY_REQ) |
                                   (1 << ZL303XX_MSG_ID_ANNOUNCE) |
                                   (1 << ZL303XX_MSG_ID_SIGNALING);

         if ((1 << messageType) & seqIdList)
         {
            stream->config.nextTxSequenceId[messageType]++;
         }
      }

      /* Get the flagField out of the buffer. */
      txMsg.flagField = UNPACK_16BIT(buffer + ZL303XX_PTP_V2_HDR_FLAGS_OFST);

      if (stream != NULL)
      {
         txMsg.streamHandle = stream->clockStreamHandle;
         txMsg.streamExtData = stream->config.extData;

         /* Update the TX message counter. */
         stream->counters.msg[messageType].tx++;
      }
      else
      {
         txMsg.streamHandle = (Uint32T)ZL303XX_PTP_INVALID;
         txMsg.streamExtData = NULL;
      }

      /* 1588-2008, Annex E (UDP over IPv6): A transmitting node shall extend the
       * UDP payload of all PTP messages by two octets beyond the end of the PTP message. */
      if (txMsg.destAddr.networkProtocol == ZL303XX_NETWORK_PROTOCOL_UDP_IPv6)
      {
         buffer[bufLen] = 0;
         buffer[bufLen + 1] = 0;
         bufLen += 2;
      }

      txFuncStatus = port->config.txMsgFn(buffer, bufLen, &txMsg);
      if (txFuncStatus != 0)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                       "   (ERROR) Message transmit failed with status=%d.",
                       txFuncStatus, 0,0,0,0,0);
#ifdef ENABLE_EXT_TX_API_CHECKS
          status = ZL303XX_PTP_TX_SOCKET_ERROR;
#endif
      }
   }

   return status;
}

/* zl303xx_PtpEventClockCreate */
/** 

   Packages the CLOCK_CREATE event, and executes the event callback.

  Parameters:
   [in]  pPtpClock   Pointer to the newly created clock.

*******************************************************************************/
void zl303xx_PtpEventClockCreate(zl303xx_PtpClockS *pPtpClock)
{
   if (pPtpClock->config.eventCallback != NULL)
   {
      zl303xx_PtpEventClockCreateS eventData;

      eventData.clockHandle  = pPtpClock->clockHandle;
      eventData.clockExtData = pPtpClock->config.extData;
      eventData.profile      = pPtpClock->config.profile;

      /* Copy the clock default data set to a BMC comparison structure. */
      (void)zl303xx_PtpClockCompareDataGet(pPtpClock, &eventData.clockDefault);

      pPtpClock->config.eventCallback(ZL303XX_PTP_EVENT_CLOCK_CREATE, &eventData);
   }
}  /* END zl303xx_PtpEventClockCreate */

/* zl303xx_PtpEventStreamCreate */
/** 

   Gathers data for the STREAM_CREATE event, and executes the event callback.

  Parameters:
   [in,out]  pStream    Pointer to the created stream.
   [in]      rxExtData  Implementation-specific data from the PTP message
                             that triggered this event.
   [out]     newConfig  Stream configuration that may have been changed by
                             the user inside the callback.

*******************************************************************************/
void zl303xx_PtpEventStreamCreate(zl303xx_ClockStreamS *pStream, Uint8T *rxExtData,
                                  zl303xx_PtpStreamCreateS *newConfig)
{
   zl303xx_PtpEventStreamCreateS eventData;

   if (pStream->portData->clock->config.eventCallback != NULL)
   {
      /* Use a local copy of the current stream config because it's passed to
       * the user as a pointer and may be modified */
      *newConfig = pStream->config;

      eventData.clockHandle  = pStream->portData->clock->clockHandle;
      eventData.portHandle   = pStream->portData->clockPortHandle;
      eventData.clockExtData = pStream->portData->clock->config.extData;
      eventData.portExtData  = pStream->portData->config.extData;
      eventData.portSrcAddr  = pStream->portData->config.localAddr;
      eventData.streamHandle = pStream->clockStreamHandle;
      eventData.streamConfig = newConfig;

      if (rxExtData != NULL)
      {
         OS_MEMCPY(eventData.rxExtData, rxExtData, sizeof(eventData.rxExtData));
      }
      else
      {
         OS_MEMSET(eventData.rxExtData, 0, sizeof(eventData.rxExtData));
      }

      pStream->portData->clock->config.eventCallback(ZL303XX_PTP_EVENT_STREAM_CREATE, &eventData);
   }
}

/* zl303xx_PtpEventContractRejected */
/** 

   Gathers data for the CONTRACT_REJECTED event, and executes the event callback.

  Parameters:
   [in]   pClock       Pointer to the clock this even occurred on.
   [in]   handle       Port or stream handle (depends if this event occurs on
                            a master or slave, respectively).
   [in]   msgType      Value of the messageType field being rejected.
   [in]   contractErr  Type of rejection error.

*******************************************************************************/
void zl303xx_PtpEventContractRejected(zl303xx_PtpClockS *pClock, Uint32T handle,
                                      zl303xx_MessageTypeE msgType,
                                      zl303xx_PtpUniNegContractErrorE contractErr)
{
   zl303xx_PtpEventContractRejectedS eventData;

   if (pClock->config.eventCallback != NULL)
   {
      OS_MEMSET(&eventData, 0, sizeof(eventData));

      eventData.handle = handle;
      eventData.messageId = msgType;
      eventData.errorType = contractErr;

      pClock->config.eventCallback(ZL303XX_PTP_EVENT_CONTRACT_REJECTED, &eventData);
   }
}

/* zl303xx_PtpEventStreamStateChange */
/** 

   Notifies the management system of changes to the operating state of a Stream
   (provided a valid 'Event Callback' routine has been configured.

  Parameters:
   [in]  pPtpStream  Pointer to the clock stream instance data structure
                           for the stream that has changed state.
   [in]  operState   Current operating state of the stream.

  Return Value:
   Nothing

*******************************************************************************/
void zl303xx_PtpEventStreamStateChange(zl303xx_ClockStreamS *pPtpStream,
                                       zl303xx_PtpStreamOperatingStateE operState)
{
   zl303xx_PtpClockS *pPtpClock = pPtpStream->portData->clock;

   /* Notify any management system that an update in state has occurred. */
   if (pPtpClock->config.eventCallback != NULL)
   {
      zl303xx_PtpEventStreamStateChangeS eventData;

      OS_MEMSET(&eventData, 0, sizeof(zl303xx_PtpEventStreamStateChangeS));

      eventData.streamHandle = pPtpStream->clockStreamHandle;
      eventData.streamExtData = pPtpStream->config.extData;
      eventData.operState = operState;

      pPtpClock->config.eventCallback(ZL303XX_PTP_EVENT_STREAM_STATE_CHANGE, &eventData);
   }

   /* Set the operating state of the stream. */
   pPtpStream->operState = operState;
}  /* END zl303xx_PtpEventStreamStateChange */

/* zl303xx_PtpEventStreamKeepAlive */
/** 

   Notifies the management system that a stream is still active (even though
   an update has not been sent for an extended time).

  Parameters:
   [in]  pPtpStream   Pointer to the stream for which the event applies.

  Return Value:
   Nothing

*******************************************************************************/
void zl303xx_PtpEventStreamKeepAlive(zl303xx_ClockStreamS *pPtpStream)
{
   /* Reference the clock pointer associated with the stream. */
   zl303xx_PtpClockS *pPtpClock = pPtpStream->portData->clock;

   /* The event structure. */
   zl303xx_PtpEventStreamKeepAliveS eventData;

   /* FMT loop variables */
   Uint16T entryIndex = 0;
   zl303xx_BooleanE entryFound = ZL303XX_FALSE;
   zl303xx_ForeignMasterRecordS *fmtEntry = NULL;

   /* Notify any management system of an update in the clock BMCA table. */
   if (pPtpClock->config.eventCallback != NULL)
   {
      /* Set any associated data. */
      eventData.clockHandle = pPtpClock->clockHandle;
      eventData.clockExtData = pPtpClock->config.extData;
      eventData.clockProfile = pPtpClock->config.profile;

      /* Search the FMT of the clock for the BMCA data of the stream. */
      for (; entryIndex < pPtpClock->config.maxForeignRecords; entryIndex++)
      {
         /* Dereference the table entry (to make the code easier to read).*/
         fmtEntry = &pPtpClock->foreignMasterDS.foreign[entryIndex];

         if ((fmtEntry->streamData == pPtpStream) &&
             ((fmtEntry->streamData->pUmtEntry == NULL) ||
              (zl303xx_PtpTelecomWtrExpired(&fmtEntry->streamData->pUmtEntry->telecom))))
         {
            /* Add the generic data. */
            eventData.bmcaEntry.streamHandle = pPtpStream->clockStreamHandle;
            eventData.bmcaEntry.farEndPortAddress = fmtEntry->portAddress;
            eventData.bmcaEntry.bmcaData = fmtEntry->compareData;

            /* Add other Profile data (if necessary). */
            if (ZL303XX_OK == zl303xx_PtpProfileBmcaDataAdd(fmtEntry, &eventData.bmcaEntry))
            {
               /* Update the number of entries. */
               entryFound = ZL303XX_TRUE;
            }

            /* Expect a single entry so we send the first found. */
            break;
         }
      }

      if (entryFound == ZL303XX_TRUE)
      {
         /* Send the event. */
         pPtpClock->config.eventCallback(ZL303XX_PTP_EVENT_STREAM_KEEP_ALIVE, &eventData);
      }
      else
      {
         /* No Announce data has been received for this stream.
          * Most likely a Master sending to a client so ignore the Keep-Alive     */
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
               "zl303xx_PtpEventStreamKeepAlive: No FMT entry for stream (%d)",
               pPtpStream->clockStreamHandle, 0,0,0,0,0);
      }
   }

   /* Reset the keep-alive counter for this stream regardless of the lookup result. */
   pPtpStream->keepAliveRemSec = pPtpStream->config.keepAliveSec;

}  /* END zl303xx_PtpEventStreamKeepAlive */

/* zl303xx_PtpEventStreamDisqualified */
/** 

   Notifies the management system that a stream has been disqualified from
   taking part in the BMCA decision.

  Parameters:
   [in]  pPtpStream   Pointer to the stream for which the event applies.

  Return Value:
   Nothing

*******************************************************************************/
void zl303xx_PtpEventStreamDisqualified(zl303xx_ClockStreamS *pPtpStream)
{
   /* Check Input parameters */
   if((pPtpStream) && (pPtpStream->portData))
   {
      zl303xx_PtpClockS *pPtpClock = pPtpStream->portData->clock;

      /* Notify any management system that an update in state has occurred. */
      if (pPtpClock->config.eventCallback != NULL)
      {
         zl303xx_PtpEventStreamDisqualifiedS eventData;

         OS_MEMSET(&eventData, 0, sizeof(zl303xx_PtpEventStreamDisqualifiedS));

         eventData.streamHandle = pPtpStream->clockStreamHandle;
         eventData.clockHandle = pPtpClock->clockHandle;

         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
               "zl303xx_PtpEventStreamDisqualified: Stream with handle (%d) disqualified",
               pPtpStream->clockStreamHandle, 0,0,0,0,0);
         pPtpClock->config.eventCallback(ZL303XX_PTP_EVENT_STREAM_DISQUALIFIED, &eventData);
      }
   }
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "zl303xx_PtpEventStreamDisqualified: Invalid input parameters ",
            0, 0, 0, 0, 0, 0);
   }
}/* END zl303xx_PtpEventStreamDisqualified */

/* zl303xx_PtpEventClockBmcaUpdate */
/** 

   Notifies the management system of updates to a Clock's BMCA table.

  Parameters:
   [in]  pPtpClock   Pointer to the clock for which the BMCA data applies.

  Return Value:
   Nothing

*******************************************************************************/
void zl303xx_PtpEventClockBmcaUpdate(zl303xx_PtpClockS *pPtpClock)
{
   Uint16T entryIndex = 0;
   zl303xx_ForeignMasterRecordS *fmtEntry = NULL;
   zl303xx_PtpEventClockBmcaUpdateS eventData;
   zl303xx_PtpEventClockBmcaEntryS bmcaEntries[ZL303XX_PTP_DEFAULT_PORT_MAX_FOREIGN_REC];

   /* Notify any management system of an update in the clock BMCA table. */
   if ((pPtpClock->config.eventCallback != NULL) &&
       (pPtpClock->config.bmca.updateTrigger != ZL303XX_BMCA_TRIGGER_DISABLED) &&
       (pPtpClock->foreignMasterDS.tableUpdated == ZL303XX_TRUE))
   {
      OS_MEMSET(&eventData, 0, sizeof(zl303xx_PtpEventClockBmcaUpdateS));

      eventData.clockHandle = pPtpClock->clockHandle;
      eventData.profile = pPtpClock->config.profile;

      /* Copy the clock default data set to a BMC comparison structure. */
      (void)zl303xx_PtpClockCompareDataGet(pPtpClock, &eventData.clockDefault);

      /* For G.8275.1: upload clock localPriority as priority1 (which is unused). */
      if ((pPtpClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_1v1) ||
          (pPtpClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_1v2) ||
          (pPtpClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_2))
      {
         eventData.clockDefault.grandmasterPriority1 =
                        pPtpClock->config.profileCfg.g8275p1.localPriority;

         /* For GM or BC, upload the fallback Class for the DefaultDS. */
         if (((pPtpClock->config.clockType == ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_GM) ||
              (pPtpClock->config.clockType == ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_BC)) &&
             (pPtpClock->config.profileCfg.g8275p1.bypassEnabled == ZL303XX_FALSE))
         {
            eventData.clockDefault.grandmasterClockQuality.clockClass =
                                       pPtpClock->profileDS.g8275.fallbackClass;
         }
      }

      /* Add the Clock extra Data. */
      eventData.clockExtData = pPtpClock->config.extData;
      eventData.numBmcaEntries = 0;

      if (pPtpClock->foreignMasterDS.numActiveRecords > 0)
      {
         for (; entryIndex < pPtpClock->config.maxForeignRecords; entryIndex++)
         {
            /* Dereference the table entry (to make the code easier to read).*/
            fmtEntry = &pPtpClock->foreignMasterDS.foreign[entryIndex];

            /* Copy each entry from the foreign Master Table. */
            if ((fmtEntry->streamData != NULL) &&
                (fmtEntry->valid == ZL303XX_TRUE) &&
                ((fmtEntry->streamData->pUmtEntry == NULL) ||
                 (zl303xx_PtpTelecomWtrExpired(&fmtEntry->streamData->pUmtEntry->telecom))))
            {
               /* Add the generic data. */
               bmcaEntries[eventData.numBmcaEntries].streamHandle = fmtEntry->streamData->clockStreamHandle;
               bmcaEntries[eventData.numBmcaEntries].farEndPortAddress = fmtEntry->portAddress;
               bmcaEntries[eventData.numBmcaEntries].bmcaData = fmtEntry->compareData;

               /* Add other Profile data (if necessary). */
               if (ZL303XX_OK == zl303xx_PtpProfileBmcaDataAdd(fmtEntry, &bmcaEntries[eventData.numBmcaEntries]))
               {
                  /* Update the number of entries. */
                  eventData.numBmcaEntries++;
               }

               /* Reset the keep-alive counter for this stream. */
               fmtEntry->streamData->keepAliveRemSec = fmtEntry->streamData->config.keepAliveSec;
            }
            else
            {
               break;
            }
         }
      }

      /* Add any data from Virtual PTP Ports */
      {
         zl303xx_PortDataS *pPtpPort = NULL;

         ZL303XX_PTP_FOR_EACH_PORT(&pPtpClock->portList, pPtpPort)
         {
            if ((pPtpPort->virtualData.isVirtualPort == ZL303XX_TRUE) &&
                (pPtpPort->virtualData.config.ptsf == ZL303XX_FALSE))
            {
               /* Add the generic data. */
               bmcaEntries[eventData.numBmcaEntries].streamHandle = -1;

               OS_MEMSET(&bmcaEntries[eventData.numBmcaEntries].bmcaData, 0x00,
                      sizeof(zl303xx_PtpBmcCompareDataS));

               bmcaEntries[eventData.numBmcaEntries].bmcaData.grandmasterPriority1 =
                                                   pPtpPort->virtualData.config.priority1;
               bmcaEntries[eventData.numBmcaEntries].bmcaData.grandmasterPriority2 =
                                                   pPtpPort->virtualData.config.priority2;
               bmcaEntries[eventData.numBmcaEntries].bmcaData.grandmasterClockQuality =
                                                   pPtpPort->virtualData.config.clockQuality;
               bmcaEntries[eventData.numBmcaEntries].bmcaData.stepsRemoved =
                                                   pPtpPort->virtualData.config.stepsRemoved;

               OS_MEMCPY(bmcaEntries[eventData.numBmcaEntries].bmcaData.grandmasterIdentity,
                      pPtpPort->virtualData.config.clockIdentity,
                      PTP_V2_CLK_IDENTITY_LENGTH);

               bmcaEntries[eventData.numBmcaEntries].bmcaData.receivingPortIdentity =
                                                   pPtpPort->portDS.portIdentity;
               bmcaEntries[eventData.numBmcaEntries].bmcaData.foreignPortIdentity =
                                                   pPtpPort->portDS.portIdentity;

               /* Next line is crucial for VP to work with Default BMCA. It checks if
                * a VP is being compared by the following:
                * - RxClockID == TXClockID &&
                * - RxPortNumber != 0 &&
                * - TxPortNumber == 0 &&  (assumes sorcePortIdentity.portNumber is never 0 in the real world)
                * - RxClockID == GM ID      */
               bmcaEntries[eventData.numBmcaEntries].bmcaData.foreignPortIdentity.portNumber = 0;

               bmcaEntries[eventData.numBmcaEntries].bmcaData.timeProperties =
                                                pPtpPort->virtualData.config.timeProperties;
               bmcaEntries[eventData.numBmcaEntries].bmcaData.timeProperties.timeTraceable =
                                                pPtpPort->virtualData.config.timeSetFlag;

               /* Add other Profile data (if necessary). */
               if (ZL303XX_OK == zl303xx_PtpProfileBmcaVirtualPortAdd(pPtpPort, &bmcaEntries[eventData.numBmcaEntries]))
               {
                  eventData.numBmcaEntries++;
               }
            }
         }
      }

      /* Assign the pointer to the list of entries. */
      if (eventData.numBmcaEntries == 0)
      {
         eventData.bmcaEntry = NULL;
      }
      else
      {
         eventData.bmcaEntry = bmcaEntries;
      }

      pPtpClock->config.eventCallback(ZL303XX_PTP_EVENT_CLOCK_BMCA_UPDATE, &eventData);

      /* Reset the BMCA_UPDATE Timer (if applicable). */
      if (zl303xx_PtpTimerActive(&pPtpClock->bmca.updateTimer))
      {
         zl303xx_PtpTimerRestart(&pPtpClock->bmca.updateTimer);
      }
   }

   /* Reset the 'updated' flag. */
   pPtpClock->foreignMasterDS.tableUpdated = ZL303XX_FALSE;
}  /* END zl303xx_PtpEventClockBmcaUpdate */

/* zl303xx_PtpClockForceBmcaUpdateEvent */
/**

   API routine to force the ZL303XX_PTP_EVENT_CLOCK_BMCA_UPDATE notification
   to be issued (regardless if any changes have occurred within the local
   data Table).

  Parameters:
   [in]  clockHandle  Handle of the clock for which to force the event.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    clockHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockForceBmcaUpdateEvent(zl303xx_PtpClockHandleT clockHandle)
{
   zlStatusE status = ZL303XX_OK;

   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Get a pointer to the clock structure. */
   if ((status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock)) != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "zl303xx_PtpClockForceBmcaUpdateEvent: Invalid Clock (%d)",
            clockHandle, 0,0,0,0,0);
   }
   else
   {
      zl303xx_PtpClockBmcaTriggerE origTriggerConfig = pPtpClock->config.bmca.updateTrigger;

      /* If the Upload Trigger is set to disabled then the BMCA UPLOAD will be
       * blocked. However, since we want to FORCE a 1-time upload, fake the
       * routine by setting enabling the Trigger temporarily.      */
      if (origTriggerConfig == ZL303XX_BMCA_TRIGGER_DISABLED)
      {
         pPtpClock->config.bmca.updateTrigger = ZL303XX_BMCA_TRIGGER_MIN_ANNC_INTVL;
      }

      /* Upload the Foreign Master Table for the clock. */
      ZL303XX_TRACE_ALWAYS("zl303xx_PtpClockBmcaUpdateInitiate %d", __LINE__,0,0,0,0,0);

      zl303xx_PtpClockBmcaUpdateInitiate(pPtpClock);

      /* Set the original configuration back (if it was changed). */
      pPtpClock->config.bmca.updateTrigger = origTriggerConfig;
   }

   return status;
}  /* END zl303xx_PtpClockForceBmcaUpdateEvent */

/* zl303xx_PtpEventCounterAlarm */
/** 

   Gathers data for the COUNTER_ALARM event, and executes the event callback.

  Parameters:
   [in]   pStream          Pointer to the stream this event occurred on.
   [in]   messageType      Value of the messageType field this alarm applies to.
   [in]   rxCountExpected  Expected RX message count.

*******************************************************************************/
void zl303xx_PtpEventCounterAlarm(zl303xx_ClockStreamS *pStream,
                                  zl303xx_MessageTypeE msgType,
                                  Uint32T rxCountExpected)
{
   if (pStream->portData->clock->config.eventCallback != NULL)
   {
      zl303xx_PtpEventCounterAlarmS eventData;
      OS_MEMSET(&eventData, 0, sizeof(eventData));

      eventData.streamHandle    = pStream->clockStreamHandle;
      eventData.streamExtData   = pStream->config.extData;
      eventData.messageType     = msgType;
      eventData.periodSec       = ZL303XX_PTP_STREAM_ALARM_PERIOD_SEC;
      eventData.rxCount         = pStream->countersInt.msg[msgType].rx;
      eventData.rxCountExpected = rxCountExpected;

      pStream->portData->clock->config.eventCallback(ZL303XX_PTP_EVENT_COUNTER_ALARM,
                                                     &eventData);
   }
}

/* zl303xx_PtpEventSquelch */
/** 

   Gathers data for the SQUELCH event, and executes the event callback.

  Parameters:
   [in]   pClock         Pointer to the clock this event occurred on.
   [in]   enableOutput   ZL303XX_FALSE if grandmasters are in failure conditions.

*******************************************************************************/
void zl303xx_PtpEventSquelch(zl303xx_PtpClockS *pClock, zl303xx_BooleanE enableOutput)
{
   if ((pClock->profileDS.telecom.enableOutput != enableOutput) &&
       (pClock->config.telecom.evtSquelchEn) &&
       (pClock->config.eventCallback))
   {
      zl303xx_PtpEventSquelchS eventData;
      OS_MEMSET(&eventData, 0, sizeof(eventData));

      eventData.clockHandle = pClock->clockHandle;
      eventData.clockExtData = pClock->config.extData;
      eventData.enableOutput = enableOutput;

      pClock->config.eventCallback(ZL303XX_PTP_EVENT_SQUELCH, &eventData);

      pClock->profileDS.telecom.enableOutput = enableOutput;
   }
}

/* zl303xx_PtpEventMsgIntvlChange */
/** 

   Gathers data for the MSG_INTVL_CHANGE event, and executes the event callback.

  Parameters:
   [in]   pStream    Pointer to the stream this event occurred on.
   [in]   type       Message type that has changed interval.
   [in]   prevIntvl  Previous message log2 interval.
   [in]   currIntvl  Current message log2 interval

*******************************************************************************/
void zl303xx_PtpEventMsgIntvlChange(zl303xx_ClockStreamS *pStream,
                                    zl303xx_MessageTypeE type,
                                    Sint8T prevIntvl, Sint8T currIntvl)
{
   zl303xx_PtpClockS *pClock = pStream->portData->clock;
   zl303xx_PtpEventMsgIntvlChangeS eventData;

   if (pClock->config.eventCallback != NULL)
   {
      OS_MEMSET(&eventData, 0, sizeof(eventData));

      eventData.streamHandle = pStream->clockStreamHandle;
      eventData.streamExtData = pStream->config.extData;
      eventData.messageType = type;
      eventData.prevIntvl = prevIntvl;
      eventData.currIntvl = currIntvl;

      pClock->config.eventCallback(ZL303XX_PTP_EVENT_MSG_INTVL_CHANGE,
                                   &eventData);
   }
}

/* zl303xx_PtpEventParentDsChange */
/** 

   Gathers data for the PARENT_DATA_SET_CHANGE event, and executes the
   event callback.

  Parameters:
   [in]   pPtpClock  Pointer to the PTP Clock instance on which the PArentDs
                           update occurred.

*******************************************************************************/
void zl303xx_PtpEventParentDsChange(zl303xx_PtpClockS *pPtpClock)
{
   zl303xx_PtpEventParentDsChangeS eventData;

   if (pPtpClock->config.eventCallback != NULL)
   {
      OS_MEMSET(&eventData, 0, sizeof(eventData));

      eventData.clockHandle = pPtpClock->clockHandle;
      OS_MEMCPY(eventData.clockIdentity,
             pPtpClock->config.defaultDS.clockIdentity,
             PTP_V2_CLK_IDENTITY_LENGTH);

      eventData.clockExtData = pPtpClock->config.extData;

      eventData.parentDS = pPtpClock->parentDS;

      pPtpClock->config.eventCallback(ZL303XX_PTP_EVENT_PARENT_DATA_SET_CHANGE, &eventData);
   }
}

/* zl303xx_PtpEventContractGranted */
/** 

   Gathers data for the CONTRACT_GRANTED event, and executes the event callback.

  Parameters:
   [in]   pStream  Pointer to the stream this event occurred on.
   [in]   pGrant   Unpacked GRANT TLV sent or received.
   [in]   flow     Flow type (RX or TX) of the granted contract.

*******************************************************************************/
void zl303xx_PtpEventContractGranted(zl303xx_ClockStreamS *pStream,
                                     zl303xx_PtpV2SignalGrantS *pGrant,
                                     zl303xx_PtpContractFlowE flow)
{
   zl303xx_PtpEventContractGrantedS eventData;

   if (pStream->portData->clock->config.eventCallback != NULL)
   {
      OS_MEMSET(&eventData, 0, sizeof(eventData));

      eventData.streamHandle = pStream->clockStreamHandle;
      eventData.streamExtData = pStream->config.extData;
      eventData.msgType = pGrant->messageType;
      eventData.contractFlow = flow;
      eventData.logMessageInterval = pGrant->logInterMessagePeriod;
      eventData.durationSec = pGrant->durationField;
      eventData.renewalInvited = pGrant->renewalInvited;

      pStream->portData->clock->config.eventCallback(ZL303XX_PTP_EVENT_CONTRACT_GRANTED,
                                                     &eventData);
   }
}

/* zl303xx_PtpEventContractExpired */
/** 

   Gathers data for the CONTRACT_EXPIRED event, and executes the event callback.

  Parameters:
   [in]   pStream  Pointer to the stream this event occurred on.
   [in]   msgType  messageType of the expired contract.
   [in]   flow     Flow type (RX or TX) of the expired contract.

*******************************************************************************/
void zl303xx_PtpEventContractExpired(zl303xx_ClockStreamS *pStream,
                                     zl303xx_MessageTypeE msgType,
                                     zl303xx_PtpContractFlowE flow)
{
   zl303xx_PtpEventContractExpiredS eventData;

   if (pStream->portData->clock->config.eventCallback != NULL)
   {
      OS_MEMSET(&eventData, 0, sizeof(eventData));

      eventData.streamHandle = pStream->clockStreamHandle;
      eventData.streamExtData = pStream->config.extData;
      eventData.msgType = msgType;
      eventData.contractFlow = flow;

      pStream->portData->clock->config.eventCallback(ZL303XX_PTP_EVENT_CONTRACT_EXPIRED,
                                                     &eventData);
   }
}

/* zl303xx_PtpEventMultiplePeerResp */
/** 

   Gathers data for the MULTIPLE_PEER_RESP event, and executes the event callback.

  Parameters:
   [in]  pStream        Pointer to the stream this event occurred on.
   [in]  sequenceId     Sequence ID of the duplicate message.
   [in]  rxTs           Received Timestamp of the duplicate message.
   [in]  txPortIdentity Port Identity of the sender of the duplicate message.
   [in]  rxTs           Port Identity of the receiver of the duplicate message.

*******************************************************************************/
void zl303xx_PtpEventMultiplePeerResp(
      zl303xx_ClockStreamS *pStream,
      Uint16T sequenceId,
      zl303xx_TimeStamp *rxTs,
      zl303xx_PortIdentity *txPortIdentity,
      zl303xx_PortIdentity *reqPortIdentity)
{
   zl303xx_PtpEventMultiplePeerRespS eventData;

   if (pStream->portData->clock->config.eventCallback != NULL)
   {
      OS_MEMSET(&eventData, 0, sizeof(eventData));

      /* Stream Data */
      eventData.streamHandle = pStream->clockStreamHandle;
      eventData.streamExtData = pStream->config.extData;

      /* Port Data */
      eventData.rxPortIdentity = pStream->portData->portDS.portIdentity;
      eventData.portExtData = pStream->portData->config.extData;

      /* Additional Event Data. */
      eventData.rxTs            = *rxTs;
      eventData.sequenceId      = sequenceId;
      eventData.txPortIdentity  = *txPortIdentity;
      eventData.reqPortIdentity = *reqPortIdentity;

      /* Export the event. */
      pStream->portData->clock->config.eventCallback(
                                    ZL303XX_PTP_EVENT_MULTIPLE_PEER_RESP,
                                    &eventData);
   }
}

/* zl303xx_PtpEventUnknownTlv */
/** 

   Gathers data for the UNKNOWN_TLV event, and executes the event callback.

  Parameters:
   [in]  pPtpPort    Pointer to the PTP Port on which the Unknown TLV was
                           received and which generated this event.
   [in]  pPtpStream  Pointer to the PTP Stream on which the Unknown TLV was
                           received (if known) and which generated this event.
                           If NULL, stream handle is set to All 1's (INVALID).
   [in]  pPtpMsgRx   The PTP message data related to the received TLV.
   [in]  tlvLength   Length of the TLV attached.
   [in]  pInTlvBuf   Pointer to the received UNKNOWN TLV buffer (starts with
                           the TYPE field of the TLV).

*******************************************************************************/
void zl303xx_PtpEventUnknownTlv(
      zl303xx_PortDataS *pPtpPort,
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint16T tlvLength,
      Uint8T *pInTlvBuf)
{
   zl303xx_PtpEventUnknownTlvS eventData;

   if ((pPtpPort != NULL) &&
       (pPtpPort->clock->config.eventCallback != NULL))
   {
      OS_MEMSET(&eventData, 0, sizeof(eventData));

      /* Add all PTP Object handles and Port Identity (as available). */
      eventData.clockHandle = pPtpPort->clock->clockHandle;
      eventData.portHandle = pPtpPort->clockPortHandle;
      eventData.receivedPortIdentity = pPtpPort->portDS.portIdentity;

      if (pPtpStream != NULL)
      {
         eventData.streamHandle = pPtpStream->clockStreamHandle;
      }
      else
      {
         eventData.streamHandle = (Uint32T)(-1);
      }

      /* Add external PTP Object Data. */
      eventData.portExtData = pPtpPort->config.extData;
      eventData.clockExtData = pPtpPort->clock->config.extData;

      /* Add the PTP Message header and data. */
      eventData.ptpMsgRx = *pPtpMsgRx;

      /* Add the unknown TLV Buffer */
      eventData.msgLength = tlvLength;
      eventData.msgBuffer = pInTlvBuf;

      pPtpPort->clock->config.eventCallback(ZL303XX_PTP_EVENT_UNKNOWN_TLV, &eventData);
   }
}

/* zl303xx_PtpEventLeapSecondsFlagChange */
/** 

   Notifies the management system of changes to either of the Leap Seconds
   flags of a Clock's dynamic Time Properties data set.

  Parameters:
   [in]  pPtpClock   Pointer to the clock on which the change occurred and
                           from which the current values of the Time Properties
                           data set will be taken.

   [in]  previousTimeProperties  Previous values of the clock time properties
                                       that include the old leap59 & leap 61 flags.

  Return Value:
   Nothing

*******************************************************************************/
void zl303xx_PtpEventLeapSecondsFlagChange(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_TimePropertiesDS *previousTimeProperties)
{
   /* Notify any management system of an update in the clock BMCA table. */
   if (pPtpClock->config.eventCallback != NULL)
   {
      zl303xx_PtpEventLeapSecondsFlagChangeS eventData;

      /* Add the current clock info */
      eventData.clockHandle = pPtpClock->clockHandle;
      eventData.localTimeProperties = pPtpClock->config.localTimeProperties;
      eventData.currentTimeProperties = pPtpClock->timePropertiesDS;
      eventData.clockExtData = pPtpClock->config.extData;

      /* Add the old values. */
      eventData.previousLeap59 = previousTimeProperties->leap59;
      eventData.previousLeap61 = previousTimeProperties->leap61;

      /* Add the current PTP time (if available). */
      OS_MEMSET(&eventData.currentPtpTime, 0x00, sizeof(eventData.currentPtpTime));
      (void)zl303xx_PtpClockTimeGet(pPtpClock, ZL303XX_PTP_INTERFACE_GENERAL, &eventData.currentPtpTime);

      /* Send the message. */
      pPtpClock->config.eventCallback(ZL303XX_PTP_EVENT_LEAP_SECONDS_FLAG_CHANGE, &eventData);
   }
}  /* END zl303xx_PtpEventLeapSecondsFlagChange */

/* zl303xx_PtpEventUtcOffsetChange */
/** 

   Notifies the management system of a change in the currentUtcOffset of a
   Clock's dynamic Time Properties data set.

  Parameters:
   [in]  pPtpClock      Pointer to the clock on which the change occurred and
                              from which the current value of the UTC Offset
                              will be taken.

   [in]  previousTimeProperties  Previous values of the clock time properties
                                       that include the old UTC Offset and UTC
                                       Valid flag.

  Return Value:
   Nothing

*******************************************************************************/
void zl303xx_PtpEventUtcOffsetChange(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_TimePropertiesDS *previousTimeProperties)
{
   /* Notify any management system of an update in the clock BMCA table. */
   if (pPtpClock->config.eventCallback != NULL)
   {
      zl303xx_PtpEventUtcOffsetChangeS eventData;

      /* Add the current clock info */
      eventData.clockHandle = pPtpClock->clockHandle;
      eventData.localTimeProperties = pPtpClock->config.localTimeProperties;
      eventData.currentTimeProperties = pPtpClock->timePropertiesDS;
      eventData.clockExtData = pPtpClock->config.extData;

      /* Add the old value. */
      eventData.previousUtcOffset = previousTimeProperties->currentUtcOffset;
      eventData.previousUtcOffsetValid = previousTimeProperties->currentUtcOffsetValid;

      /* Add the current PTP time (if available). */
      OS_MEMSET(&eventData.currentPtpTime, 0x00, sizeof(eventData.currentPtpTime));
      (void)zl303xx_PtpClockTimeGet(pPtpClock, ZL303XX_PTP_INTERFACE_GENERAL, &eventData.currentPtpTime);

      /* Send the message. */
      pPtpClock->config.eventCallback(ZL303XX_PTP_EVENT_UTC_OFFSET_CHANGE, &eventData);
   }
}  /* END zl303xx_PtpEventUtcOffsetChange */

/* zl303xx_PtpEventRxStepsRemovedExceeded */
/** 

   Notifies the management system that an ANNOUNCE message has been received
   in which the stepsRemoved field exceeds the locally configured
   maxStepsRemoved limit.

  Parameters:
   [in]  pPtpPort   Pointer to the port on which the ANNOUNCE message was
                           received.

   [in]  ptpAnncMsg  Pointer to the received ANNOUNCE message.

  Return Value:
   Nothing

*******************************************************************************/
void zl303xx_PtpEventRxStepsRemovedExceeded(
      zl303xx_PortDataS *pPtpPort,
      zl303xx_PtpV2MsgS *ptpAnncMsg)
{
   /* Notify any management system of a possible rogue ANNOUNCE message. */
   if (pPtpPort->clock->config.eventCallback != NULL)
   {
      zl303xx_PtpEventRxStepsRemovedExceededS eventData;

      /* Add the current clock info */
      eventData.clockHandle = pPtpPort->clock->clockHandle;
      eventData.ptpHeader = ptpAnncMsg->header;
      eventData.announceData = ptpAnncMsg->data.announce;
      eventData.maxStepsRemoved = pPtpPort->clock->config.optionalDefaultDS.maxStepsRemoved;
      eventData.rxPortIdentity = pPtpPort->portDS.portIdentity;
      eventData.clockExtData = pPtpPort->clock->config.extData;

      /* Send the message. */
      pPtpPort->clock->config.eventCallback(ZL303XX_PTP_EVENT_RX_STEPS_REMOVED_EXCEEDED, &eventData);
   }
}  /* END zl303xx_PtpEventRxStepsRemovedExceeded */

/* zl303xx_PtpPortAddressCheck */
/** 

   Checks if the contents of a zl303xx_PortAddress structure are valid.

  Parameters:
   [in]   pa   The port address to check.

*******************************************************************************/
zlStatusE zl303xx_PtpPortAddressCheck(zl303xx_PortAddress *pa)
{
   zlStatusE status = ZL303XX_OK;

   /* Check that networkProtocol is in range. */
   if ((pa->networkProtocol == 0) ||
       (pa->networkProtocol > ZL303XX_NETWORK_PROTOCOL_PROFINET))
   {
      ZL303XX_TRACE_ERROR("   (ERROR) Invalid networkProtocol=%u.",
                          pa->networkProtocol, 0,0,0,0,0);
      status = ZL303XX_PARAMETER_INVALID;
   }

   /* Check that addressLength is correct for the selected networkProtocol. */
   else if (pa->addressLength != Zl303xx_PtpPortAddrLen[pa->networkProtocol])
   {
      ZL303XX_TRACE_ERROR("   (ERROR) Invalid addressLength=%u for networkProtocol=%u.",
                          pa->addressLength, pa->networkProtocol, 0,0,0,0);
      status = ZL303XX_PARAMETER_INVALID;
   }

   return status;
}

/* zl303xx_PtpConvertCurrentDSInternal */
/** 
   Converts a zl303xx_CurrentDSInternal structure to a zl303xx_CurrentDS structure.

  Parameters:
   [in]   cds_i   Internal structure.
   [out]  cds     Standard structure.

*******************************************************************************/
void zl303xx_PtpConvertCurrentDSInternal(zl303xx_CurrentDSInternal *cds_i,
                                         zl303xx_CurrentDS *cds)
{
   cds->stepsRemoved = cds_i->stepsRemoved;
   zl303xx_PtpConvTimeStampToTimeInterval(&cds_i->offset_from_master, &cds->offsetFromMaster);
   zl303xx_PtpConvTimeStampToTimeInterval(&cds_i->mean_path_delay, &cds->meanPathDelay);
}

/* zl303xx_PtpMsgCountExpected */
/** 

   Returns the expected number of messages to be sent/received over some period
   of time.

  Parameters:
   [in]   log2Intvl   The log2 interval rate of the message type.
   [in]   periodSec   The period of time being measured, in seconds.

*******************************************************************************/
Uint32T zl303xx_PtpMsgCountExpected(Sint8T log2Intvl, Uint16T periodSec)
{
   Uint32T msgCount = periodSec;

   if (log2Intvl < 0)
   {
      msgCount <<= -log2Intvl;
   }
   else
   {
      msgCount >>= log2Intvl;
   }

   return msgCount;
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/* zl303xx_PtpHwCmdFnCheck */
/** 

   Checks if the hardware command function for a clock is valid.

  Parameters:
   [in]   pPtpClock  Pointer to the clock structure to check.

*******************************************************************************/
static zlStatusE zl303xx_PtpHwCmdFnCheck(zl303xx_PtpClockS *pPtpClock)
{
   if (pPtpClock->config.hwCmdFn == NULL)
   {
      ZL303XX_TRACE_ERROR("   (ERROR) No hardware command function defined.", 0,0,0,0,0,0);
      return ZL303XX_INIT_ERROR;
   }

   return ZL303XX_OK;
}

/* zl303xx_PtpConvertIPv4ToStr */
/** 

   Converts an IPv4 address to it's equivalent string representation.

  Parameters:
   [in]   ipv4   A byte array containing the IPv4 address.
   [out]  str    The equivalent IPv4 address string.

*******************************************************************************/
static zlStatusE zl303xx_PtpConvertIPv4ToStr(Uint8T *ipv4, char *str)
{
   if (OS_SNPRINTF(str, MAX_IPV4_ADDR_STR_LEN, "%d.%d.%d.%d", ipv4[0], ipv4[1], ipv4[2], ipv4[3]) < 0)
   {
      ZL303XX_TRACE_ERROR("   (ERROR) Cannot convert PTP port address to IPv4 string.",
                          0,0,0,0,0,0);
      str[0] = '\0';
      return ZL303XX_PARAMETER_INVALID;
   }

   return ZL303XX_OK;
}

/* zl303xx_PtpConvertIPv6ToStr */
/** 

   Converts an IPv6 address to it's equivalent string representation.

  Parameters:
   [in]   ipv6   A byte array containing the IPv6 address.
   [out]  str    The equivalent IPv6 address string.

*******************************************************************************/
static zlStatusE zl303xx_PtpConvertIPv6ToStr(Uint8T *ipv6, char *str)
{
   Uint16T count, strIdx = 0, zeroLen = 2, _zeroLen = 0, zeroEnd = 0;
   Uint16T byteNonZero = 0xFFFF, state = 0;
   Sint16T ret;

   /* Find the longest chain of zeros. If more than one chain shares the longest
    * length, the leftmost chain is reduced. */
   for (count = 0; count < Zl303xx_PtpPortAddrLen[ZL303XX_NETWORK_PROTOCOL_UDP_IPv6]; count++)
   {
      if (ipv6[count])
      {
         zeroEnd = 0;
         _zeroLen = 0;
      }
      else
      {
         _zeroLen++;

         if (((_zeroLen & 0xFFFE) > zeroLen) || (zeroEnd > 0))
         {
            zeroEnd = count;
            zeroLen = _zeroLen;
            byteNonZero = ~(((1 << zeroLen) - 1) << (count - zeroLen + 1));
         }
      }
   }

   /* Convert the byte array to a string. */
   for (count = 0; count < Zl303xx_PtpPortAddrLen[ZL303XX_NETWORK_PROTOCOL_UDP_IPv6]; count += 2)
   {
      ret = 0;

      if (byteNonZero & 0x3 << count)
      {
         ret = OS_SNPRINTF(str + strIdx, 3, "%x:", (ipv6[count] << 8) | ipv6[count + 1]);
      }
      else
      {
         if (state == 0)
         {
            if (strIdx > 0)
               strIdx--;

            ret = OS_SNPRINTF(str + strIdx, 2, "::");
         }

         state++;
      }

      if (ret < 0)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Cannot convert PTP port address to IPv6 string.",
                             0,0,0,0,0,0);
         str[0] = '\0';
         return ZL303XX_PARAMETER_INVALID;
      }
      else
      {
         strIdx += ret;
      }
   }

   if (byteNonZero & 0xC000)
      str[strIdx - 1] = '\0';

   return ZL303XX_OK;
}

/* zl303xx_PtpConvertMacToStr */
/** 

   Converts a MAC address to it's equivalent string representation.

  Parameters:
   [in]   mac   A byte array containing the MAC address.
   [out]  str   The equivalent MAC address string.

*******************************************************************************/
static zlStatusE zl303xx_PtpConvertMacToStr(Uint8T *mac, char *str)
{
   if (OS_SNPRINTF(str, MAX_MAC_ADDR_STR_LEN, "%02X:%02X:%02X:%02X:%02X:%02X",
               mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]) < 0)
   {
      ZL303XX_TRACE_ERROR("   (ERROR) Cannot convert PTP port address to 802.3 string.",
                          0,0,0,0,0,0);
      str[0] = '\0';
      return ZL303XX_PARAMETER_INVALID;
   }

   return ZL303XX_OK;
}

/*****************   END   ****************************************************/
