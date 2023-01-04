

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
* Copyright 2006-2018 Microsemi Semiconductor Limited.
* All rights reserved.
*
*  Module Description:
*     Architecture dependent constants
*
*******************************************************************************/

#ifndef ZL303XX_PTP_CONSTANTS_DEP_H
#define ZL303XX_PTP_CONSTANTS_DEP_H

/* platform dependent */
/* Microsemi defined constants */

#if __BYTE_ORDER == __LITTLE_ENDIAN
  #define PTPD_LSBF
#elif __BYTE_ORDER == __BIG_ENDIAN
  #define PTPD_MSBF
#endif

/* UDP/IPv4 dependent */

#define SUBDOMAIN_ADDRESS_LENGTH  4
#define PORT_ADDRESS_LENGTH       2

#define DEFAULT_PTP_UDP_EVENT_PORT     319
#define DEFAULT_PTP_UDP_GENERAL_PORT   320

#define DEFAULT_PTP_DOMAIN_ADDRESS_IPV4     "224.0.1.129"
#define ALTERNATE_PTP_DOMAIN1_ADDRESS_IPV4  "224.0.1.130"
#define ALTERNATE_PTP_DOMAIN2_ADDRESS_IPV4  "224.0.1.131"
#define ALTERNATE_PTP_DOMAIN3_ADDRESS_IPV4  "224.0.1.132"
#define DEFAULT_MULTICAST_IP_PDELAY         "224.0.0.107"

extern const Uint8T Zl303xx_PtpDefaultDomainAddrIPv6[];
extern const Uint8T Zl303xx_PtpAlternateDomain1AddrIPv6[];
extern const Uint8T Zl303xx_PtpAlternateDomain2AddrIPv6[];
extern const Uint8T Zl303xx_PtpAlternateDomain3AddrIPv6[];

#define DEFAULT_PTP_DOMAIN_ADDRESS_IPV6     Zl303xx_PtpDefaultDomainAddrIPv6
#define ALTERNATE_PTP_DOMAIN1_ADDRESS_IPV6  Zl303xx_PtpAlternateDomain1AddrIPv6
#define ALTERNATE_PTP_DOMAIN2_ADDRESS_IPV6  Zl303xx_PtpAlternateDomain2AddrIPv6
#define ALTERNATE_PTP_DOMAIN3_ADDRESS_IPV6  Zl303xx_PtpAlternateDomain3AddrIPv6

extern const char *Zl303xx_PtpDefaultDomainAddrStrIPv6;
extern const char *Zl303xx_PtpAlternateDomain1AddrStrIPv6;
extern const char *Zl303xx_PtpAlternateDomain2AddrStrIPv6;
extern const char *Zl303xx_PtpAlternateDomain3AddrStrIPv6;

extern const Uint8T Zl303xx_PtpDefaultDomainAddr802_3[];
extern const Uint8T Zl303xx_PtpAlternateDomain1Addr802_3[];
extern const Uint8T Zl303xx_PtpAlternateDomain2Addr802_3[];
extern const Uint8T Zl303xx_PtpAlternateDomain3Addr802_3[];
extern const Uint8T Zl303xx_PtpPdelayDomainAddr802_3[];

#define DEFAULT_PTP_DOMAIN_ADDRESS_802_3      Zl303xx_PtpDefaultDomainAddr802_3
#define ALTERNATE_PTP_DOMAIN1_ADDRESS_802_3   Zl303xx_PtpAlternateDomain1Addr802_3
#define ALTERNATE_PTP_DOMAIN2_ADDRESS_802_3   Zl303xx_PtpAlternateDomain2Addr802_3
#define ALTERNATE_PTP_DOMAIN3_ADDRESS_802_3   Zl303xx_PtpAlternateDomain3Addr802_3
#define PDELAY_PTP_DOMAIN_ADDRESS_802_3       Zl303xx_PtpPdelayDomainAddr802_3

#endif

