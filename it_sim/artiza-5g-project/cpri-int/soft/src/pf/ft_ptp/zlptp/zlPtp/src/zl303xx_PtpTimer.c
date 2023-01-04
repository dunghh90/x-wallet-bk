

/*****************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
* Copyright 2006-2018 Microsemi Semiconductor Limited.
* All rights reserved.
*
*  Module Description:
*     This file contains support for software timer functions for the
*     IEEE-1588 ptp protocols
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_Os.h"
#include "zl303xx_Trace.h"
#include "zl303xx_List.h"
#include "zl303xx_Macros.h"

#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpSetup.h"
#include "zl303xx_PtpTimer.h"

#include <string.h>

/*****************   DEFINES   ************************************************/
#define ZL303XX_PTP_FOR_EACH_TIMER(head, timer)  \
           ZL303XX_LIST_FOR_EACH_TYPE(head, timer, zl303xx_PtpTimerS, listEntry)

#define PTPTIMERSHUTDOWN -1

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/
static Sint32T zl303xx_PtpTimerTask(void);
static Uint32T zl303xx_PtpTimerListUpdate(zl303xx_ListS *timerList, Uint32T deltaNs);

/*****************   STATIC GLOBAL VARIABLES   ********************************/
static OS_TASK_ID PtpTimerTaskId = OS_TASK_INVALID;

/* Each clock has its own linked list of active timers. */
static zl303xx_ListS PtpTimerList[ZL303XX_PTP_NUM_CLOCKS_MAX];

#ifdef OS_LINUX
#include "zl303xx_LinuxOs.h"
#include "zl303xx_HWTimer.h"
#define TIMER_INVALID 0
static timer_t periodicTimerid = TIMER_INVALID;

/* MsgQ-related */
static OS_MSG_Q_ID zl303xx_Timer_QueueId = OS_MSG_Q_INVALID;
static Uint32T zl303xx_TimerQueueSize = 2;


/* Timer wakeUp msg */
typedef struct
{
    Sint32T wakeUpTimerId;  /* Which timer expired? */
} zl303xx_TimerWakeMsgS;      /* Scheduler and Timer task wake up msgs */

static void wakeTimerWithMsg(Sint32T timerThatFired, Sint32T pid);
#endif

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/
extern zl303xx_PtpClockS *Zl303xx_PtpClockArray[];

/*****************   INTERNAL PTP FUNCTION DEFINITIONS   **********************/

/* zl303xx_PtpTimerTaskStart */
/** 

   Starts the PTP timer task which updates the interval timers associated with
   each stream that are used to send out packets at regular intervals.

  Parameters:
   [in]   None.

*******************************************************************************/
zlStatusE zl303xx_PtpTimerTaskStart(void)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T count;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "zl303xx_PtpTimerTaskStart:", 0,0,0,0,0,0);

   if (PtpTimerTaskId != OS_TASK_INVALID)
   {
        ZL303XX_TRACE_ALWAYS("zl303xx_PtpTimerTaskStart: ZL303XX_MULTIPLE_INIT_ATTEMPT", 0,0,0,0,0,0);
        return ZL303XX_MULTIPLE_INIT_ATTEMPT;
   }

#ifdef OS_LINUX
   if (status == ZL303XX_OK)
   {
      /* Set msg Queue */
      zl303xx_Timer_QueueId = OS_MSG_Q_CREATE((Sint32T)zl303xx_TimerQueueSize,
                                              sizeof(zl303xx_TimerWakeMsgS));

      if (zl303xx_Timer_QueueId == OS_MSG_Q_INVALID)
      {
         status = ZL303XX_RTOS_MSGQ_CREATE_FAIL;
         ZL303XX_ERROR_TRAP("Error creating MsgQ for the Timer task");
      }
   }
#endif

   if (status == ZL303XX_OK)
   {
      /* Initialize each clock's timer linked list. */
      for (count = 0; count < ZL303XX_ARRAY_SIZE(PtpTimerList); count++)
      {
         zl303xx_ListInit(&PtpTimerList[count]);
      }

      /* Create the timer task. */
      PtpTimerTaskId = OS_TASK_SPAWN(ZL303XX_PTP_TIMER_TASK_NAME,
                                     ZL303XX_PTP_TIMER_TASK_PRIORITY,
                                     0,
                                     ZL303XX_PTP_TIMER_TASK_STACK_SIZE,
                                     (OS_TASK_FUNC_PTR)zl303xx_PtpTimerTask,
                                     0);

      /* If task creation failed, report and cleanup. */
      if (PtpTimerTaskId == OS_TASK_INVALID)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Timer task create failed.", 0,0,0,0,0,0);
         status = ZL303XX_RTOS_TASK_CREATE_FAIL;
      }
   }

 #ifdef OS_LINUX
   if (status == ZL303XX_OK)
   {
       OS_TASK_DELAY(500);  /* Let the PTP task start */

       /* create timer */
      status = zl303xx_SetHWTimer(ZLTICKTIMERSIG, &periodicTimerid,
                                ZL303XX_PTP_TIMER_TASK_INTVL_MS, &wakeTimerWithMsg, ZL303XX_TRUE);
      if (status != ZL303XX_OK)
      {
         ZL303XX_ERROR_NOTIFY("timer_create() FAILED");
         status = ZL303XX_ERROR;
      }
   }
 #endif

   return status;
}

/* zl303xx_PtpTimerTaskStart */
/** 

   Stops the PTP timer task.

  Parameters:
   [in]   None.

*******************************************************************************/
zlStatusE zl303xx_PtpTimerTaskStop(void)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "zl303xx_PtpTimerTaskStop:", 0,0,0,0,0,0);

   /* Check if the timer task exists. */
   if (status == ZL303XX_OK)
   {
      if (PtpTimerTaskId == OS_TASK_INVALID)
      {
         /* This is not an error. */
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "   Timer task does not exist.", 0,0,0,0,0,0);
         return status;
      }
   }

   /* Delete the timer list. */
   if (status == ZL303XX_OK)
   {
      Uint32T count;

      /* Reset each clock's timer list to the empty set. */
      for (count = 0; count < ZL303XX_ARRAY_SIZE(PtpTimerList); count++)
      {
         zl303xx_ListInit(&PtpTimerList[count]);
      }
   }

#ifdef OS_LINUX
   if (status == ZL303XX_OK && periodicTimerid != TIMER_INVALID)
   {
      if (zl303xx_DeleteHWTimer(ZLTICKTIMERSIG, &periodicTimerid) != ZL303XX_OK)
      {
          ZL303XX_ERROR_NOTIFY("timer_delete() FAILED");
      }
      periodicTimerid = TIMER_INVALID;
   }

   /* Delete the timer task via msgQ. */
   if (status == ZL303XX_OK)
   {
       zl303xx_TimerWakeMsgS msg;

       OS_MEMSET(&msg, 0, sizeof(zl303xx_TimerWakeMsgS));
       msg.wakeUpTimerId = PTPTIMERSHUTDOWN;

       OS_MSG_Q_SEND(zl303xx_Timer_QueueId, (Sint8T *)&msg, sizeof(zl303xx_TimerWakeMsgS), OS_WAIT_FOREVER);
       OS_TASK_DELAY(250);
   }

   if (status == ZL303XX_OK)
   {
       if (OS_TASK_DELETE(PtpTimerTaskId) == OS_OK)
       {
          PtpTimerTaskId = OS_TASK_INVALID;
       }
       else
       {
          ZL303XX_TRACE_ERROR("   (ERROR) Timer task delete failed.", 0,0,0,0,0,0);
          status = ZL303XX_RTOS_TASK_DELETE_FAIL;
       }
   }

   if (status == ZL303XX_OK)
   {
      if (zl303xx_Timer_QueueId != OS_MSG_Q_INVALID)                  /* Remove the MsgQ */
      {
         OS_STATUS osStatus = OS_MSG_Q_DELETE(zl303xx_Timer_QueueId);
         if (osStatus != OS_OK)
         {
             ZL303XX_ERROR_NOTIFY("Error deleting MsgQ for the Timer task");
             status = ZL303XX_RTOS_MSGQ_DELETE_FAIL;
         }

         zl303xx_Timer_QueueId = OS_MSG_Q_INVALID;
      }
   }
#else
   if (status == ZL303XX_OK)
   {
       if (OS_TASK_DELETE(PtpTimerTaskId) == OS_OK)
       {
          PtpTimerTaskId = OS_TASK_INVALID;
       }
       else
       {
          ZL303XX_TRACE_ERROR("   (ERROR) Timer task delete failed.", 0,0,0,0,0,0);
          status = ZL303XX_RTOS_TASK_DELETE_FAIL;
       }
   }
#endif

   return status;
}

/* zl303xx_PtpTimerStart */
/** 

   Starts a PTP timer (used to send packets or mark an event timeout). If the
   timer is already active, then it is restarted with the new interval
   provided and all expiry counts are reset.

  Parameters:
   [in]  clockHandle       Handle to a clock containing its own timer list.
   [in]  pTimer            The specific timer to start.
   [in]  pInterval         The interval to time.
   [in]  bExpireAtStart    Flag indicating if the Timer should fire at
                                 creation or on first expiry. Example: ANNOUNCE
                                 messages should be sent immediately, not after
                                 the first expiry interval.

  Return Value:
     ZL303XX_OK                    The timer was started successfully.
     ZL303XX_PARAMETER_INVALID     The supplied interval is 0.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock the timer mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock the timer mutex.

  Notes:   Assumes all input values are valid.

*******************************************************************************/
zlStatusE zl303xx_PtpTimerStart(zl303xx_PtpClockHandleT clockHandle,
                                zl303xx_PtpTimerS *pTimer,
                                zl303xx_TimeStamp *pInterval,
                                zl303xx_BooleanE bExpireAtStart)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TIMER, 3,
         "zl303xx_PtpTimerStart: %s; set interval: " PTPD_FORMAT_TIME_STR " using pTimer=%p",
         pTimer->descr,
         (Uint32T) formatTimeStringSign(pInterval),
         formatTimeStringSec(pInterval),
         formatTimeStringNs(pInterval), pTimer, 0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      if (isZeroTime(pInterval))
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Timer interval value must be non-zero (%s).",
               pTimer->descr, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Initialize the timer values. */
   if (status == ZL303XX_OK)
   {
      /* Return early if the timer is already active and the interval value is
       * not being changed. */
      if (zl303xx_ListValid(&pTimer->listEntry) &&
          (memcmp(&pTimer->interval, pInterval, sizeof(pTimer->interval)) == 0))
      {
         return status;
      }

      pTimer->interval = *pInterval;
      OS_MEMSET(&pTimer->carryOver, 0, sizeof(pTimer->carryOver));
      pTimer->expiredCount = ((bExpireAtStart == ZL303XX_TRUE) ? 1 : 0);

      /* If the timer is not in the active list, add it. */
      if (!zl303xx_ListValid(&pTimer->listEntry))
      {
         zl303xx_ListAdd(&PtpTimerList[clockHandle], &pTimer->listEntry);
      }

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
                    "zl303xx_PtpTimerStart: %s; set interval: " PTPD_FORMAT_TIME_STR " using pTimer=%p",
                    pTimer->descr,
                    (Uint32T) formatTimeStringSign(pInterval),
                    formatTimeStringSec(pInterval),
                    formatTimeStringNs(pInterval), pTimer, 0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TIMER, 3,
                    "zl303xx_PtpTimerStart: %s; set interval: " PTPD_FORMAT_TIME_STR " using pTimer=%p",
                    pTimer->descr,
                    (Uint32T) formatTimeStringSign(pInterval),
                    formatTimeStringSec(pInterval),
                    formatTimeStringNs(pInterval), pTimer, 0);
   }

   return status;
}

/* zl303xx_PtpTimerRestart */
/** 

   Restarts (or resets) an ALREADY ACTIVE PTP timer (typically used for event
   timeouts and not for packet launching timers. Specifically used for the
   ANNOUNCE_RECEIPT timer). The routine uses the existing interval value of the
   timer and resets the other counts to 0.

  Parameters:
   [in]   pTimer       The specific timer to restart.

  Return Value:
     ZL303XX_OK                    The timer was restarted successfully.
     ZL303XX_PARAMETER_INVALID     Attempted to restart a timer that has never
                                       been started before.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock the timer mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock the timer mutex.

  Notes:   Assumes all input values are valid.

*******************************************************************************/
zlStatusE zl303xx_PtpTimerRestart(zl303xx_PtpTimerS *pTimer)
{
   zlStatusE status = ZL303XX_OK;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      if (!zl303xx_PtpTimerActive(pTimer))
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Re-Start of INACTIVE timer.", 0,0,0,0,0,0);
         status = ZL303XX_NOT_RUNNING;
      }
   }

   if (status == ZL303XX_OK)
   {
      OS_MEMSET(&pTimer->carryOver, 0, sizeof(pTimer->carryOver));
      pTimer->expiredCount = 0;

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TIMER, 3,
            "zl303xx_PtpTimerRestart: %s; set interval: " PTPD_FORMAT_TIME_STR " using pTimer=%p",
            pTimer->descr,
            (Uint32T) formatTimeStringSign(&pTimer->interval),
            formatTimeStringSec(&pTimer->interval),
            formatTimeStringNs(&pTimer->interval), pTimer, 0);
   }

   return status;
}

/* zl303xx_PtpTimerStop */
/** 

   Stops a PTP timer if it is active. Inactive timers return ZL303XX_OK as well.

  Parameters:
   [in]   pTimer  The specific timer to stop.

  Return Value:
     ZL303XX_OK                    The timer was stopped successfully.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to TAKE the timer mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to GIVE the timer mutex.

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
void zl303xx_PtpTimerStop(zl303xx_PtpTimerS *pTimer)
{
   /* Just remove the timer from the active list -- this will invalidate the
    * linked list pointers. All other fields will get reset when
    * zl303xx_PtpTimerStart() is called. */

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TIMER, 3,
         "zl303xx_PtpTimerStop: %s; set interval: " PTPD_FORMAT_TIME_STR " for pTimer=%p",
         pTimer->descr,
         (Uint32T) formatTimeStringSign(&pTimer->interval),
         formatTimeStringSec(&pTimer->interval),
         formatTimeStringNs(&pTimer->interval), pTimer, 0);

   zl303xx_ListRemove(&pTimer->listEntry);

   /* Set the expired count to 0 so the handler will not fire after removal. */
   pTimer->expiredCount = 0;
}

/* zl303xx_PtpTimerActive */
/** 

   Returns TRUE if the timer is part of the active list.

  Parameters:
   [in]   pTimer  The specific timer to check the state of.

  Return Value:
     ZL303XX_TRUE   The timer has active.
     ZL303XX_FALSE  The timer has not active.

  Notes:   Assumes all input values are valid.

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtpTimerActive(zl303xx_PtpTimerS *pTimer)
{

   return zl303xx_ListValid(&pTimer->listEntry);
}

/* zl303xx_PtpTimerExpired */
/** 

   Returns the number of times the specified timer has expired (i.e., the number
   of timer intervals that have elapsed). Typically used to determine the number
   of packets (for the specified timer) that are due to be launched OR that are
   expected to have been received. Resets the backlog to 0 but maintains any
   partial counts.

  Parameters:
   [in]  pTimer  Timer to query for expired count.

  Return Value:  Uint32T  The number of times this timer has expired since this function
                    was last called.

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
Uint32T zl303xx_PtpTimerExpired(zl303xx_PtpTimerS *pTimer)
{
   Uint32T retVal = 0;

   if (zl303xx_PtpTimerActive(pTimer) && (pTimer->expiredCount > 0))
   {
      /* Reset the packet backlog count. */
      retVal = pTimer->expiredCount;
      pTimer->expiredCount = 0;
   }

   return retVal;
}

/* zl303xx_PtpTimerExecAllExpired */
/** 

   Executes the callback function for every timer that has expired at least once
   since the last time this function or zl303xx_PtpTimerExpired() was called.

  Parameters:
   [in]   clockHandle  Handle to the clock whose list will be traversed.

*******************************************************************************/
void zl303xx_PtpTimerExecAllExpired(zl303xx_PtpClockHandleT clockHandle)
{
   zl303xx_ListS *curr;
   zl303xx_PtpTimerS *timer;

   ZL303XX_LIST_FOR_EACH(&PtpTimerList[clockHandle], curr)
   {
      timer = ZL303XX_CONTAINER_GET(curr, zl303xx_PtpTimerS, listEntry);

      if ((timer->expiredCount > 0) && (timer->callback != NULL))
      {
          zl303xx_TimeStamp *pInterval = &timer->interval;

          ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TIMER, 3,
                      "Timer expired: %s; interval: " PTPD_FORMAT_TIME_STR " using pTimer=%p",
                      timer->descr,
                      (Uint32T) formatTimeStringSign(pInterval),
                      formatTimeStringSec(pInterval),
                      formatTimeStringNs(pInterval), timer, 0);

         timer->callback(timer);
         timer->expiredCount = 0;

         /* It's possible that this timer got deleted inside the callback
          * function. Detect this case, and recover by restarting the FOR_EACH
          * loop. FOR_EACH_SAFE cannot be used here because the next timer
          * may have deleted also/instead. */
         if (!zl303xx_ListValid(curr))
         {
            curr = &PtpTimerList[clockHandle];
         }
      }
   }
}

/* zl303xx_PtpTimerListShow */
/** 

   Routine to display all timers active on a PTP Clock.

  Parameters:
   [in]   clockHandle  Handle to the clock whose list will be traversed.

*******************************************************************************/
void zl303xx_PtpTimerListShow(zl303xx_PtpClockHandleT clockHandle)
{
   zl303xx_ListS *curr;
   zl303xx_PtpTimerS *timer;

   ZL303XX_LIST_FOR_EACH(&PtpTimerList[clockHandle], curr)
   {
      timer = ZL303XX_CONTAINER_GET(curr, zl303xx_PtpTimerS, listEntry);

       ZL303XX_TRACE_ALWAYS(
             "Timer (%p)(%s): interval: " PTPD_FORMAT_TIME_STR " Count %u",
             timer, timer->descr,
             (Uint32T) formatTimeStringSign(&timer->interval),
             formatTimeStringSec(&timer->interval),
             formatTimeStringNs(&timer->interval),
             timer->expiredCount);
   }
}


/* zl303xx_PtpTimerListToString */
/** 

   Routine that returns all timers active on a PTP Clock in a string.

  Parameters:
   [in]   clockHandle  Handle to the clock whose list will be traversed.
             str          string to store the timer list

*******************************************************************************/
void zl303xx_PtpTimerListToString(zl303xx_PtpClockHandleT clockHandle,char *str)
{

    zl303xx_ListS *curr;
    zl303xx_PtpTimerS *timer;

    ZL303XX_LIST_FOR_EACH(&PtpTimerList[clockHandle], curr)
    {
        timer = ZL303XX_CONTAINER_GET(curr, zl303xx_PtpTimerS, listEntry);

        sprintf(str+strlen(str),
             "Timer (%p)(%s): interval: " PTPD_FORMAT_TIME_STR " Count %u\n",
             timer, timer->descr,
             (Uint32T) formatTimeStringSign(&timer->interval),
             formatTimeStringSec(&timer->interval),
             formatTimeStringNs(&timer->interval),
             timer->expiredCount);
   }
}



/******************   STATIC FUNCTION DEFINITIONS  **************************/

/* zl303xx_PtpTimerTask */
/** 

   The VxWorks Task function executes every ZL303XX_PTP_TIMER_TASK_INTVL_MS or one
   system tick if the tick rate is less than 200, updates all timers and posts a
   message to the event queue. Linux uses a periodic timer to drive a msgQ.

  Parameters:
      None.

  Return Value:  -1  An error occurred. Otherwise, will never return.

*******************************************************************************/
static Sint32T zl303xx_PtpTimerTask(void);
static Sint32T zl303xx_PtpTimerTask(void)
{
   Uint32T nTicksPerSec = (Uint32T)OS_TICK_RATE_GET();
   Uint32T count, expiredTimers, deltaNs;
   zl303xx_BooleanE secondFlag;
   Uint64S thisTime, lastTime;

#ifdef OS_LINUX
   zl303xx_TimerWakeMsgS msg;
#else
   Uint32T nanosecPerTick = TEN_e9 / nTicksPerSec;
   Uint32T timerTicks = (nanosecPerTick >= (ZL303XX_PTP_TIMER_TASK_INTVL_MS * TEN_e6))
                           ? 1
                           : (ZL303XX_PTP_TIMER_TASK_INTVL_MS * TEN_e6 / nanosecPerTick);
#endif

   OS_TIME_GET(&lastTime, NULL);

#ifdef OS_LINUX
   while (1)
   {
       osStatusT osStatus = OS_OK;

       OS_MEMSET(&msg, 0, sizeof(zl303xx_TimerWakeMsgS));

      if (!zl303xx_Timer_QueueId || ((osStatus = OS_MSG_Q_RECEIVE(zl303xx_Timer_QueueId,
            (Sint8T *)&msg,                /* Pend on msgQ */
            sizeof(msg),
            (Sint32T)nTicksPerSec))) == OS_ERROR)   /* up to 1 sec. */
      {
         fprintf(stderr, "Error reading MsgQ in zl303xx_PtpTimerTask loop");
         OS_TASK_DELAY(100);
         continue;
      }

      if (osStatus == WAIT_TIMEOUT)
      {
          OS_TASK_DELAY(100);
          continue;
      }

      if (msg.wakeUpTimerId == PTPTIMERSHUTDOWN)
      {
          OS_TASK_DELAY(2000);
          break;
      }
#else
   /* Sleep for ZL303XX_PTP_TIMER_TASK_INTVL_MS or one system tick */
   while (OS_TICK_DELAY(timerTicks) == OS_OK)
   {
#endif
      OS_TASKMON_FUNC_START();

      OS_TIME_GET(&thisTime, NULL);
      if (thisTime.hi > lastTime.hi)    /* Assume sub-second timer */
      {
         deltaNs = TEN_e9 + thisTime.lo - lastTime.lo;
         secondFlag = ZL303XX_TRUE;
      }
      else
      {
         deltaNs = thisTime.lo - lastTime.lo;
         secondFlag = ZL303XX_FALSE;
      }
      lastTime = thisTime;

      for (count = 0; count < ZL303XX_ARRAY_SIZE(PtpTimerList); count++)
      {
         if ((Zl303xx_PtpClockArray[count] != NULL) &&
             (zl303xx_PtpClockMutexLock(Zl303xx_PtpClockArray[count]) == ZL303XX_OK))
         {
            expiredTimers = zl303xx_PtpTimerListUpdate(&PtpTimerList[count], deltaNs);

            if ((expiredTimers > 0) || secondFlag)
            {
               /* Notify the appropriate PTP clock task that a timer has fired. */
               (void)zl303xx_PtpSendTimerTick(Zl303xx_PtpClockArray[count], secondFlag);
            }

            (void)zl303xx_PtpClockMutexUnlock(Zl303xx_PtpClockArray[count], ZL303XX_OK);
         }
      }
      OS_TASKMON_FUNC_END();

   }  /* Marks the end of the while() loop. */

   return PTPTIMERSHUTDOWN;
}

/* zl303xx_PtpTimerListUpdate */
/** 

   Updates all PTP timers in a list.

  Parameters:
   [in]  timerList   The list of PTP timers to update.
   [in]  deltaNs     The elapsed time since the last update, in nanoseconds.

  Return Value:  Uint32T  The number of timers in the list that have expired this iteration.
         0        No timers have expired, or an error occurred.

*******************************************************************************/
static Uint32T zl303xx_PtpTimerListUpdate(zl303xx_ListS *timerList, Uint32T deltaNs)
{
   Uint32T newPkts, expiredTimers = 0;
   zl303xx_PtpTimerS *timer;

   if (!zl303xx_ListValid(timerList) || zl303xx_ListEmpty(timerList))
   {
       return 0;
   }

   /* Iterate around the PTP timers and update them. */
   ZL303XX_PTP_FOR_EACH_TIMER(timerList, timer)
   {
       newPkts = 0;

      /* Assume we are driven by a sub-second timer */
      timer->carryOver.nanosecondsField += deltaNs;
      timer->carryOver.secondsField.lo += timer->carryOver.nanosecondsField / TEN_e9;
      timer->carryOver.nanosecondsField %= TEN_e9;


      /* Determine if the amount of time that has passed is greater than
       * the required interval time between packets. If so, another packet(s)
       * transmission is required.  */

      /* For faster packet rates (greater than 1 packet per second) */
      if ((timer->interval.secondsField.lo == 0) &&
          (timer->interval.nanosecondsField > 0))
      {
         /* The seconds portion of the interval value is 0 in this case so
          * ignore it.  The seconds portion of the carryOver value may not
          * necessarily be 0. */

         /* We will assume this timer is being updated more than once every 4
          * seconds so that carryOver.seconds is never greater than 4.     */
         timer->carryOver.nanosecondsField += (timer->carryOver.secondsField.lo * TEN_e9);
         timer->carryOver.secondsField.lo = 0;
         newPkts = timer->carryOver.nanosecondsField /
                   timer->interval.nanosecondsField;

         timer->carryOver.nanosecondsField -= (newPkts * timer->interval.nanosecondsField);

         #define INTERVAL_FOR_64PPS 15625000
         if (newPkts && timer->forceMinIntvl && timer->interval.nanosecondsField >= INTERVAL_FOR_64PPS) /* This works up to 64pps */
         {
             timer->carryOver.nanosecondsField = 0; /* If we are trying to hit strict inter-pkt-gaps then drop the carryOver */
             timer->carryOver.secondsField.lo = 0;
         }
      }

      /* For very slow packet rates (less than 1 packet per second) that are a
       * log2 interval (i.e. 1 packet per 1,2,4,8,etc... seconds. */
      else if ((timer->interval.secondsField.lo > 0) &&
               (timer->interval.nanosecondsField == 0))
      {
         /* The nanoseconds portion of the interval value must be 0 in this
          * case so ignore it. */
         if (timer->carryOver.secondsField.lo >= timer->interval.secondsField.lo)
         {
            newPkts = timer->carryOver.secondsField.lo /
                      timer->interval.secondsField.lo;

            timer->carryOver.secondsField.lo -= (newPkts * timer->interval.secondsField.lo);

            /* There is no affect on the carryOver.nanoseconds value. */
         }
      }

      /* Slow packet rates of non-log2 intervals (ex: 1 pkt / 3 sec). */
      else if ((timer->interval.secondsField.lo > 0) &&
               (timer->interval.nanosecondsField > 0) &&

               /* Only do the below calculations if newPkts will be >= 1 */
               ((timer->carryOver.secondsField.lo > timer->interval.secondsField.lo) ||
                   ((timer->carryOver.secondsField.lo == timer->interval.secondsField.lo) &&
                    (timer->carryOver.nanosecondsField > timer->interval.nanosecondsField))))
      {
         Uint64S carryNs, intervalNs, mod;

         /* Convert carryOver to a 64-bit nanoseconds value. */
         carryNs.hi = 0;
         carryNs.lo = timer->carryOver.nanosecondsField;
         carryNs    = Add_U64S(Mult_U32_U32(timer->carryOver.secondsField.lo, TEN_e9),
                               carryNs, NULL);

         /* Convert interval to a 64-bit nanoseconds value. */
         intervalNs.hi = 0;
         intervalNs.lo = timer->interval.nanosecondsField;
         intervalNs    = Add_U64S(Mult_U32_U32(timer->interval.secondsField.lo, TEN_e9),
                                  intervalNs, NULL);

         newPkts = Div_U64S_U64S(carryNs, intervalNs, &mod).lo;

         /* Calculate the new carryOver value. */
         timer->carryOver.secondsField.lo =
            Div_U64S_U32(mod, TEN_e9, &timer->carryOver.nanosecondsField).lo;
      }

      timer->expiredCount += newPkts;
      expiredTimers += !!newPkts;  /* 0 if newPkts == 0, 1 otherwise */
   }

   return expiredTimers;
}

#ifdef OS_LINUX
static void wakeTimerWithMsg(Sint32T timerThatFired, Sint32T pid)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_TimerWakeMsgS msg;
    register Sint32T errNum;
    register Uint8T failedOnce = 0;

    if (pid) {}; /* Warning removal */

    msg.wakeUpTimerId = timerThatFired;

#undef EXTRA_DEBUG

    /* Send the message into the queue */
    if (status == ZL303XX_OK)
    {
       {
reTryTimer:
           /* Queue the PtpTimer message. If there isn't space then discard the message. This saves it getting overloaded */
           if (zl303xx_Timer_QueueId &&
               (errNum = OS_MSG_Q_SEND(zl303xx_Timer_QueueId, (Sint8T *)&msg, sizeof(zl303xx_TimerWakeMsgS), OS_NO_WAIT)) != OS_OK)
           {
               if (!failedOnce)
               {
                   failedOnce = 1;
                   OS_TASK_DELAY(1);
                   goto reTryTimer;
               }
               status = ZL303XX_ERROR;
           }

           failedOnce = 0;
       }
    }

    return;
}

#endif
