

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Ohly-style timestamp socket setup and read..
*
*******************************************************************************/

#ifndef _zl303xx_LinuxOhlyTS_H_
#define _zl303xx_LinuxOhlyTS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_ExamplePtpSocket.h"

#include <net/ethernet.h>


typedef struct ether_header EtherHead_t;
typedef struct PtpEthFrameStruct
{
    EtherHead_t ethHdr;
    Uint8T      payload[1600];
} PtpEthFrame_t;


/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

zlStatusE zl303xx_ConfigIPV4Address(const char* interface, const char* srcIpAddressP);
zlStatusE zl303xx_ConfigOhlyHWTimestamping(char const * ifName, Sint32T fd);
zlStatusE zl303xx_RetrieveOhlyHWTimestamp(examplePtpSocketTblS *pTblEntry, Uint32T flags);







#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */

