

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Includes for various Linux variants
*
*******************************************************************************/

#ifndef _ZL303XX_LNX_VARIANTS_H_
#define _ZL303XX_LNX_VARIANTS_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef OS_LINUX

#ifdef ZL_LNX_CODESOURCERY

    /* ZL - Normally you do not do any of this or include bits - but I could find no other way */
    #define REMOVE_SUSECS_T

    #ifdef NEED_POSIX_TIME
        #ifndef __USE_POSIX199309
        #define __USE_POSIX199309
        #endif
        #include <time.h>
    #endif

    #ifdef NEED_SIGINFO
        #define ___USE_POSIX199309
        #define ___USE_POSIX
//      #define __USE_MISC
//      #define __USE_POSIX
        #include <signal.h>
//        #include <bits/sigset.h>
//        #include <bits/siginfo.h>
    #endif


    #ifdef NEED_SYSDEP_TIME
        #define __USE_POSIX199309
        #include <time.h>
    #endif

    #ifdef NEED_LINUX_TIME
        #define _STRUCT_TIMESPEC
        typedef long suseconds_t;
        #define __need_timeval
        #define __USE_POSIX199309
        #include <linux/time.h>
    #endif

    #include <bits/local_lim.h>

#endif  /* ZL_LNX_CODESOURCERY */



#ifdef ZL_LNX_DENX
/* ZL - Normally you do not do any of this or include bits - but I could find no other way */

    #ifdef NEED_SIGINFO
        #define ___USE_POSIX199309
//      #define __USE_POSIX
//      #define __USE_MISC
        #include <signal.h>
        #include <bits/siginfo.h>
        #include <bits/sigset.h>
    #endif

    #ifdef NEED_POSIX_TIME
        #ifndef __USE_POSIX
        #define __USE_POSIX
        #endif
        #ifndef __USE_POSIX199309
        #define __USE_POSIX199309
        #endif
        #include <time.h>
    #endif

    #ifdef NEED_SYSDEP_TIME
        #include <bits/time.h>
    #endif

    #ifdef NEED_LINUX_TIME
        #define _STRUCT_TIMESPEC
        #ifndef REMOVE_NEED_TIMEVAL
            #define __need_timeval
        #else
            #undef __need_timeval
        #endif
        #ifndef REMOVE_NEED_TIMESPEC
            #define __need_timespec
        #else
            #undef __need_timespec
        #endif
        #ifndef REMOVE_SUSECS_T
            typedef long suseconds_t;
        #endif
        #define __USE_POSIX199309
        #include <linux/time.h>
    #endif

    #include <bits/local_lim.h>

#endif  /* ZL_LNX_DENX */

#if defined ZL_LNX_INTEL
    #ifdef NEED_LINUX_TIME
    /*#warning NEED_LINUX_TIME*/
//      #define __USE_MISC
        #define REMOVE_NEED_TIMESPEC
        #ifdef REMOVE_NEED_TIMESPEC
            #define _STRUCT_TIMESPEC
        #else
            #undef _STRUCT_TIMESPEC
        #endif
        #ifndef REMOVE_SUSECS_T
            typedef long suseconds_t;
        #endif
        #include <linux/time.h>
    #endif

    #ifdef NEED_SYSDEP_TIME
    /*#warning NEED_SYSDEP_TIME*/
//      #define __USE_MISC
        #define __USE_POSIX199309
        #include <time.h>
    #endif

    #ifdef NEED_POSIX_TIME
    /*#warning NEED_POSIX_TIME*/
        #ifndef __USE_POSIX199309
        #define __USE_POSIX199309
        #endif
        #include <time.h>
    #endif

    #ifdef NEED_SIGINFO
    /*#warning NEED_SIGINFO*/
        #define __USE_POSIX
        #define __USE_POSIX199309
//      #define __USE_MISC
        #include <signal.h>
        #include <bits/siginfo.h>
    #endif

    #include <bits/local_lim.h>

    #ifndef CLOCK_MONOTONIC
        #define CLOCK_REALTIME  0
        #define CLOCK_MONOTONIC 1
    #endif

#endif  /* ZL_LNX_INTEL */


#include <signal.h>

/* see SIGRTMAX in bits/local_lim.h or bits/signum.h or ... */
/* SIGRTZLBLOCK should/may align with your kernel SIGRTMAX */
/* #warning MSCC: SIGRTZLBLOCK should align with your kernel SIGRTMAX */
#define SIGRTZLBLOCK      (64)          /* We need a block of real-time signals - counting down from here */

#if defined ZL_INTR_USES_SIGACTION
   #define SIGZL0HIGH     (SIGRTZLBLOCK -1)   /* Must match with kernel driver code! */
   #define SIGZL0LOW      (SIGRTZLBLOCK -2)   /* Must match with kernel driver code! */
   #define ZLWDTIMERSIG   (SIGRTZLBLOCK -3)   /* S/W WatchDog for high interrupts */
   #define ZLTICKTIMERSIG (SIGRTZLBLOCK -4)   /* PTP Timer */
   #define ZLAPRTIMERSIG  (SIGRTZLBLOCK -5)   /* Apr Timer */
#else
   #define ZLTICKTIMERSIG (SIGRTZLBLOCK -1)   /* PTP Timer */
   #define ZLAPRTIMERSIG  (SIGRTZLBLOCK -2)   /* Apr Timer */
   #define ZLWDTIMERSIG   (SIGRTZLBLOCK -3)   /* S/W WatchDog for high interrupts */
  #if defined POLL_FOR_ZL303XX_INTR
   #define SIGZL0HIGH     (SIGRTZLBLOCK -7)   /* Fake interrupt with timer */
   #define SIGZL0LOW      (SIGRTZLBLOCK -8)   /* Unused but required by handler */
  #endif
   #define ZLCSTTIMERSIG  (SIGRTZLBLOCK -10)   /* Clock settling timer - Need ZL303XX_PTP_NUM_CLOCKS_MAX (now 4) so 10 to 14 used */
#endif


#endif /* OS_LINUX */

#ifdef __cplusplus
}
#endif

#endif  /* _ZL303XX_LNX_VARIANTS_H_ */

