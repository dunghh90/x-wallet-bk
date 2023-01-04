

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Example socket abstraction. Create/manage sockets and simple socket 
*     receive task
*
*******************************************************************************/

#ifndef _ZL303XX_EXAMPLE_PTP_SOCKET_H_
#define _ZL303XX_EXAMPLE_PTP_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_PtpApiTypes.h"

#ifndef ntohl
  #include <netinet/in.h>
#endif

#if defined OHLY_TIMESTAMPS
/* Only timestamps for uni-neg master mode needs unique socket per slave connection (for egress timestamps).
   Set max number of sockets to 2x max number of streams(slaves) + 2x max number of ports.

   NOTE: this value must be less than FD_SETSIZE.
*/
#include "zl303xx_PtpSetup.h"
#define EXAMPLE_PTP_SOCKETS_MAX              ((2 * ZL303XX_PTP_NUM_STREAMS_MAX) + (2 * ZL303XX_PTP_NUM_PORTS_MAX)) /* 256 + 16 */
#else
#define EXAMPLE_PTP_SOCKETS_MAX              10
#endif

/*****************   DEFINES   ************************************************/

#ifdef OS_LINUX
/* TODO: Adjust as needed for your system capabilities */
#if !defined ZL_INCLUDE_IPV6_SOCKET
#define ZL_INCLUDE_IPV6_SOCKET
#endif
#if !defined ZL_INCLUDE_ETH_SOCKET
#define ZL_INCLUDE_ETH_SOCKET
#endif
#endif

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/* Opaque socket structure. */
typedef struct examplePtpSocketS *examplePtpSocketT;

/* Socket table structure */
typedef struct
{
   Uint16T         active;
   zl303xx_BooleanE     master;
   Sint32T         fd;
   zl303xx_PortAddress  srcAddr;
   zl303xx_PortAddress  mcastAddr;
   Uint16T         udpPort;
   Sint32T         portHandle;
   Sint32T         streamHandle;
   Sint32T         ifindex;
} examplePtpSocketTblS;

/* ExamplePtp.c‚Å‚àŽg‚¢‚½‚¢‚½‚ß.c‚©‚çˆÚ“® */
typedef struct
{
   Sint32T fd;
   Uint16T udpPort;
   Sint32T ifindex;
   char    ifname[32];
} socketS;

typedef struct examplePtpSocketS
{
   socketS socket[ZL303XX_PTP_INTERFACE_NUM_TYPES];
} examplePtpSocketS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/
examplePtpSocketTblS ExamplePtpSocketTbl[EXAMPLE_PTP_SOCKETS_MAX];
/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/* Socket functions */
zlStatusE examplePtpSocketEnvInit(void);
zlStatusE examplePtpSocketEnvClose(void);
zlStatusE examplePtpSocketUcastOpen(zl303xx_PortAddress  *pPortAddr,
                                    examplePtpSocketT *pPtpSktT);
zlStatusE examplePtpSocketMcastOpen(zl303xx_PortAddress  *pLocalAddr,
                                    zl303xx_PortAddress  *pMcastAddr,
                                    examplePtpSocketT *pPtpSktT);
zlStatusE examplePtpSocketClose(examplePtpSocketT *pPtpSktT);
zlStatusE examplePtpSocketTx(examplePtpSocketT ptpSktT,
                             Uint8T *buffer, Uint16T bufferLen,
                             zl303xx_PortAddress *pDestAddr, zl303xx_PtpInterfaceE interface);

zlStatusE examplePtpSocketPortHandleSet(examplePtpSocketT pPtpSktT, Sint32T portHandle);
zlStatusE examplePtpSocketStreamHandleSet(examplePtpSocketT pPtpSktT, Sint32T streamHandle);
void examplePtpSocketTblPrint(void);

zlStatusE examplePtpSocketStrToPortAddress(const char *strAddr, zl303xx_PortAddress *portAddr);
zlStatusE examplePtpSocketPortAddressToStr(zl303xx_PortAddress *portAddr, char *strAddr, Uint32T strSize);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
