

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

#ifndef _ZL303XX_OS_HEADERS_H_
#define _ZL303XX_OS_HEADERS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#if defined (OS_VXWORKS)

   /* include the VxWorks specific headers */
   #include <vxWorks.h>
   #include <vme.h>
   #include <memLib.h>
   #include <cacheLib.h>
   #include <semLib.h>
   #include <selectLib.h>
   #include <msgQLib.h>
   #include <wdLib.h>
   #include <ioLib.h>
   #include <in.h>
   #include <taskLib.h>
   #include <taskHookLib.h>
   #include <tickLib.h>
   #include <sysLib.h>
   #include <errnoLib.h>
   #include <intLib.h>
   #include <iv.h>
   #include <hostLib.h>
   #include <usrLib.h>
   #include <ctype.h>
   #include <timers.h>
   #include <logLib.h>
   #include <stdio.h>
   #include <string.h>

#ifndef __VXWORKS_65
   #include <routeLib.h>
#endif
#ifdef __VXWORKS_54
/* Local implementation */
int snprintf(char *destP, int size, const char *formatP, ...);
#endif

#else   /* !OS_VXWORKS */

  #if defined (OS_LINUX)
    #if defined (ZL_LNX_DENX) || defined (ZL_LNX_INTEL)
        #define OK      0
        #define ERROR   (-1)
        #include <sys/errno.h>
        #include <asm/types.h>
        #include <stdio.h>
        #include <string.h>
        #include <linux/stddef.h>
        #include <unistd.h>
        #include <sys/mman.h>
        #include <netinet/in.h>
    #endif   /* ZL_LNX_DENX */

     #if defined(ZL_LNX_CODESOURCERY)

        #define OK      0
        #define ERROR   (-1)
        #include <sys/errno.h>

        #include <stddef.h>
        #include <stdio.h>
        #include <string.h>
        #include <unistd.h>
        #include <sys/mman.h>

     #endif   /* ZL_LNX_CODESOURCERY */

     #include <err.h>
     #include <stdlib.h>
     #include <string.h>
     #include <ctype.h>
     #include <errno.h>



  #endif    /* OS_LINUX */

#endif   /* !OS_VXWORKS */

/* max() and min() */
#ifndef max
   #define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif
#ifndef min
   #define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

#ifdef __cplusplus
}
#endif


#endif

