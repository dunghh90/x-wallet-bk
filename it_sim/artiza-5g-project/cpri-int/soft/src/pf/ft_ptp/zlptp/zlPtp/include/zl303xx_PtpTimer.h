

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Internal PTP timer API.
*
*******************************************************************************/

#ifndef ZL303XX_PTP_TIMER_H_
#define ZL303XX_PTP_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_List.h"

#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpApiTypes.h"

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/* Interval timers are used:
 * - to determine the number of packets to send for the current CPU tick
 * - the time to execute other events (Foreign Master Qualification Window)
 * - timeouts on certain expected packet arrivals (Announce for V2). */
typedef struct zl303xx_PtpTimerS
{
   char             descr[24];
   zl303xx_TimeStamp  interval;
   zl303xx_TimeStamp  carryOver;
   Uint32T          expiredCount;
   zl303xx_BooleanE   forceMinIntvl;
   zl303xx_ListS      listEntry;    /* Linked list anchor */

   void (*callback)(struct zl303xx_PtpTimerS *pTimer);  /* Timer expired callback */
} zl303xx_PtpTimerS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   INTERNAL PTP FUNCTION DECLARATIONS   *********************/
zlStatusE zl303xx_PtpTimerTaskStart(void);
zlStatusE zl303xx_PtpTimerTaskStop (void);

zlStatusE zl303xx_PtpTimerStart  (zl303xx_PtpClockHandleT clockHandle,
                                  zl303xx_PtpTimerS *pTimer,
                                  zl303xx_TimeStamp *pInterval,
                                  zl303xx_BooleanE bExpireAtStart);
zlStatusE zl303xx_PtpTimerRestart(zl303xx_PtpTimerS *pTimer);
void      zl303xx_PtpTimerStop   (zl303xx_PtpTimerS *pTimer);

zl303xx_BooleanE zl303xx_PtpTimerActive (zl303xx_PtpTimerS *pTimer);
Uint32T        zl303xx_PtpTimerExpired(zl303xx_PtpTimerS *pTimer);

void zl303xx_PtpTimerExecAllExpired(zl303xx_PtpClockHandleT clockHandle);
void zl303xx_PtpTimerListShow(zl303xx_PtpClockHandleT clockHandle);
void zl303xx_PtpTimerListToString(zl303xx_PtpClockHandleT clockHandle,char *str);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */

