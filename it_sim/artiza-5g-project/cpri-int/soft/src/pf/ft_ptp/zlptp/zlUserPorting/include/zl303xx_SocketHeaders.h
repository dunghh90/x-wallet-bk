

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     The actual OS headers are included in this file to provide ANSI standard
*     functions etc. and definitions for the OS specific functions as required
*     by the porting layer.
*
******************************************************************************/

#ifndef _ZL303XX_SOCKET_HEADERS_H_
#define _ZL303XX_SOCKET_HEADERS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_OsHeaders.h"

#if defined (OS_VXWORKS)
   /* include the VxWorks specific headers */
   #include <sockLib.h>
   #include <socket.h>
   #include <inetLib.h>
   #include <ioctl.h>
   #include <netinet/if_ether.h>
#ifdef __VXWORKS_65
   #include <arpa/inet.h>
#else
   #include <net/inet.h>
#endif

#elif defined OS_LINUX
   #include <sys/select.h>

#endif

#ifdef __cplusplus
}
#endif

#endif
