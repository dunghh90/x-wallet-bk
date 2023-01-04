

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*
*  This module provides the task related functions related to the client
*  reference selection application.
*
******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

/* The Database & User Decision headers. */
#include "zl303xx_ExampleRefSelectApi.h"
#include "zl303xx_ExampleRefSelectInternal.h"
#include "zl303xx_Macros.h"

#include <errno.h>

/*****************   EXTERNS     **********************************************/

/*****************   DEFINES     **********************************************/

/* Some Macros for determining the Message Queue Time out period. Conditions are:
 * - Timeout based on the soonest instance of Refresh, HoldOff or WTR timers.
 * - Timeout cannot be 0 (1 CPU Tick is the minimum).    */
#define MIN_TICKS(a,b)        (((Uint32T)(a) < (Uint32T)(b)) ? (a) : (b))
#define MAX_TICKS(a,b)        (((Uint32T)(a) > (Uint32T)(b)) ? (a) : (b))
#define WAIT_TICKS(a,b,c)     MAX_TICKS(1,MIN_TICKS(a,MIN_TICKS(b,c)))

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/* OS Control IDs. */
OS_TASK_ID   exampleRefSelectTaskId     = OS_TASK_INVALID;
OS_MSG_Q_ID  exampleRefSelectMsgQId     = OS_MSG_Q_INVALID;

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/* Reference Selection Application Configuration Parameters are kept in the main
 * Application module but reference here as well. */
extern exampleRefSelectParamS exampleRefSelectAppParam;

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/*****************   CORE TASK ROUTINES         *******************************/

/* exampleRefSelectTaskStart */
/**

   Starts the OS task associated with the centralised Server database.

  Parameters:
   [in,out]  None

*******************************************************************************/
zlStatusE exampleRefSelectTaskStart(void)
{
   zlStatusE status = ZL303XX_OK;

   /* Create the necessary Message-Queue used by the Task to receive events. */
   if (status == ZL303XX_OK)
   {
      /* If a failure occurs, an error will be printed within the routine. */
      status = exampleRefSelectMsgQueueCreate();
   }

   /* Before starting the Task, verify if it is already running. */
   if (status == ZL303XX_OK)
   {
      if (exampleRefSelectTaskId != OS_TASK_INVALID)
      {
         /* The Task is already running so return OK immediately. */
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
               "exampleRefSelectTaskStart: Server Database Task already active",
               0,0,0,0,0,0);

         return status;
      }

      /* Task not started so spawn it now. */
      else
      {
         exampleRefSelectTaskId = OS_TASK_SPAWN(
                                       REF_SELECT_TASK_NAME,
                                       REF_SELECT_TASK_PRIORITY,
                                       0    /* Flags */,
                                       REF_SELECT_TASK_STACK_SIZE,
                                       (OS_TASK_FUNC_PTR)exampleRefSelectTaskFn,
                                       0    /* Arguments */);

         /* Verify the Task was created. */
         if (exampleRefSelectTaskId == OS_TASK_INVALID)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
                  "exampleRefSelectTaskStart: (ERROR) Server DB task failed to start.",
                  0,0,0,0,0,0);

            status = ZL303XX_RTOS_TASK_CREATE_FAIL;
         }
      }
   }

   return status;
}  /* exampleRefSelectTaskStart */

/* exampleRefSelectTaskStop */
/**

   Stops the OS task associated with the centralised Server database.

  Parameters:
   [in,out]  None

*******************************************************************************/
zlStatusE exampleRefSelectTaskStop(void)
{
   zlStatusE status = ZL303XX_OK;
   zlStatusE tStatus = ZL303XX_OK;
   zlStatusE qStatus = ZL303XX_OK;

   /* Kill the task. */
   if (exampleRefSelectTaskId != OS_TASK_INVALID)
   {
      if (OS_TASK_DELETE(exampleRefSelectTaskId) != OS_OK)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectTaskStop: OS_TASK_DELETE() failed",
            0,0,0,0,0,0);

         tStatus = ZL303XX_RTOS_TASK_DELETE_FAIL;
      }

      /* Regardless of the return status, reset the TaskId */
      exampleRefSelectTaskId = OS_TASK_INVALID;
   }

   /* Delete the Message Queue. */
   if (exampleRefSelectMsgQId != OS_MSG_Q_INVALID)
   {
      if ((qStatus = exampleRefSelectMsgQueueDelete()) != ZL303XX_OK)
      {
          ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
                "exampleRefSelectMsgQueueDelete() returned an error= %d",
                qStatus, 0,0,0,0,0);
      }

      /* Regardless of the return status, reset the MsgQ ID */
      exampleRefSelectMsgQId = OS_MSG_Q_INVALID;
   }

   /* Since there may be multiple failures, assign a priority status. */
   if (tStatus != ZL303XX_OK)
   {
      status = tStatus;
   }
   else if (qStatus != ZL303XX_OK)
   {
      status = qStatus;
   }

   return status;
}  /* END exampleRefSelectTaskStop */

/* exampleRefSelectTaskStatus */
/**

   Returns the status of the Reference Selection Application.

  Parameters:
   [in,out]  None

  Return Value:
     ZL303XX_OK                Task is Running.
     ZL303XX_NOT_RUNNING       Task is NOT Running.

*******************************************************************************/
zlStatusE exampleRefSelectTaskStatus(void)
{
   if (exampleRefSelectTaskId == OS_TASK_INVALID)
   {
      return ZL303XX_NOT_RUNNING;
   }
   else
   {
      return ZL303XX_OK;
   }
}  /* exampleRefSelectTaskStatus */

/*****************   CORE TASK ROUTINES (INTERNAL)   **************************/

/**

  Function Name:
    exampleRefSelectTaskFn

  Details:
   This task waits on event messages from related to the Centralised Server
   Database table.

  Parameters:
        None

  Return Value:
   Never returns normally. Returns -1 if an error occurs

 *****************************************************************************/
zlStatusE exampleRefSelectTaskFn(void)
{
   zlStatusE status = ZL303XX_OK;

   exampleRefSelectMsgS msg;

   /* Link the modules to this task. */

   /* Link the PTSF Updates to this task. */
   if (status == ZL303XX_OK)
   {
      Uint32T flagID;
      for (flagID = 0; flagID < ZL303XX_PTSF_NUM_FLAGS; flagID++)
      {
         (void)zl303xx_PtsfCallbackSet(flagID, exampleInterfacePtsf);
      }
   }

   /* The Task waits on the message queue and processes messages as received. */
   while (status == ZL303XX_OK)
   {
      /* Wait until an event message is received */
      msg.sourceId = REF_SELECT_SRC_MOD_UNKNOWN;
      status = exampleRefSelectMsgQueueRead(&msg);

      if (status == ZL303XX_OK)
      {
          OS_TASKMON_FUNC_START();

         /* Pass messages from each module to their respective Handler routines. */
         switch (msg.sourceId)
         {
            case REF_SELECT_SRC_MOD_APP :
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
                     "exampleRefSelectTaskFn: RX - Application Message (%ld)",
                     msg.sourceId, 0,0,0,0,0);

               /* The only thing to evaluate here is the REFRESH timer setting.
                * The other timers (WTR & HO) are calculated in the default handling
                * below (outside the switch).         */
               (void)exampleRefSelectConvertMsToTicks(
                     exampleRefSelectAppParam.config.refreshRateMs,
                     &exampleRefSelectAppParam.rt.timer.appRefreshTicks);

              /* status = exampleHandleProtocolClock(msg.connectionId, msg.action, msg.bufLen, msg.buffer); */

               if (status != ZL303XX_OK)
               {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
                        "exampleRefSelectTaskFn: RX - Application Handler FAILED - Status (%d)",
                        status, 0,0,0,0,0);

                  /* Don't kill the Task. */
                  status = ZL303XX_OK;
               }

               break;
            }

            case REF_SELECT_SRC_MOD_PROT_CLOCK :
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
                     "exampleRefSelectTaskFn: RX - PROTOCOL CLOCK Message (%ld)",
                     msg.sourceId, 0,0,0,0,0);

               status = exampleHandleProtocolClock(msg.connectionId, msg.action, msg.bufLen, msg.buffer);

               if (status != ZL303XX_OK)
               {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
                        "exampleRefSelectTaskFn: RX - Clock Handler FAILED - Status (%d)",
                        status, 0,0,0,0,0);

                  /* Don't kill the Task. */
                  status = ZL303XX_OK;
               }

               break;
            }

            case REF_SELECT_SRC_MOD_PROT_STREAM :
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
                     "exampleRefSelectTaskFn: RX - PROTOCOL STREAM Message (%ld)",
                     msg.sourceId, 0,0,0,0,0);

               status = exampleHandleProtocolStream(msg.connectionId, msg.action, msg.bufLen, msg.buffer);

               if (status != ZL303XX_OK)
               {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
                        "exampleRefSelectTaskFn: RX - Stream Handler FAILED - Status (%d)",
                        status, 0,0,0,0,0);

                  /* Don't kill the Task. */
                  status = ZL303XX_OK;
               }

               break;
            }

            case REF_SELECT_SRC_MOD_PTSF :
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
                     "exampleRefSelectTaskFn: RX - PTSF Message (%ld) for Stream (%d)",
                     msg.sourceId, msg.connectionId, 0,0,0,0);

               status = exampleHandlePtsf(msg.connectionId, msg.action, msg.bufLen, msg.buffer);

               if (status != ZL303XX_OK)
               {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
                        "exampleRefSelectTaskFn: RX - PTSF Handler FAILED - Status (%d)",
                        status, 0,0,0,0,0);

                  /* Don't kill the Task. */
                  status = ZL303XX_OK;
               }

               break;
            }

            case REF_SELECT_SRC_MOD_ALGORITHM :
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
                     "exampleRefSelectTaskFn: RX - ALGORITHM Message (%ld) for Server (%d)",
                     msg.sourceId, msg.connectionId, 0,0,0,0);

               status = exampleHandleAlgorithm(msg.connectionId, msg.action, msg.bufLen, msg.buffer);

               if (status != ZL303XX_OK)
               {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
                        "exampleRefSelectTaskFn: RX - ALGORITHM Handler FAILED - Status (%d)",
                        status, 0,0,0,0,0);

                  /* Don't kill the Task. */
                  status = ZL303XX_OK;
               }

               break;
            }

            case REF_SELECT_SRC_MOD_CUSTOM :
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
                     "exampleRefSelectTaskFn: RX - CUSTOM Message (%ld)",
                     msg.sourceId, 0,0,0,0,0);
               break;
            }


            /* Potentially the Time-Out condition on the Message-Queue so fire
             * the default processing.         */
            case REF_SELECT_SRC_MOD_UNKNOWN :
            {
               /* Execute Default Processing below. */
               ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
                     "exampleRefSelectTaskFn: UNKNOWN Message (%ld)",
                     msg.sourceId, 0,0,0,0,0);
               break;
            }

            default:
            {
               /* An UNKNOWN or UNDEFINED source module ID that we do not know
                * how to handle. Print an error and continue. */
               ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
                     "exampleRefSelectTaskFn: ERROR - Source Module ID unknown (%ld)",
                     msg.sourceId, 0,0,0,0,0);

               /* Execute Default Processing below. */
               break;
            }
         }

         /* Every time a message is processed, execute the set of DEFAULT routines
          * such as:
          * - re-evaluating server selection.
          * - re-evaluating / calculating timers,
          * - update output data based on Hold-Off and other config.       */
         {
            /*******************************
             * STEP 1: Re-evaluate any connection WTR timers.
             * - This is always done to ensure the timer values are current.
             * - This has to be done prior to making a BMCA selection since connections
             *   previously in WTR-mode may now be available.
             *********************************************************/
            {
               (void)exampleRefSelectEvaluateAllWtrTimers();
            }

            /*******************************
             * STEP 2: Re-evaluate the Selected Server based on the BMCA.
             * - This is only done if Automatic Selection is enabled.
             * - It will update the 'previousBest' pointer of the Application.
             *********************************************************/
            if (exampleRefSelectAppParam.config.selection.autoRun == ZL303XX_TRUE)
            {
               zlStatusE serverStatus = ZL303XX_OK;

               /* The best entry in the table for this iteration. */
               Uint32T newBestIndex;
               exampleRefSelectEntryS newSelectedServer;

               /* Initialize this entry to INVALID value prior to doing a server selection. */
               /* (Since the TASK only ever changes this data, no need for the MUTEX). */
               (void)exampleRefSelectRowInit(&newSelectedServer);

               /* 3 possibilities exist here:
                * 1) OK: indicates a server was found. Update the 'previousServer'
                *    selection of the Application and use this new data later.
                * 2) TABLE_ENTRY_NOT_FOUND: the table has no valid entry. Update the
                *    'previousServer' selection data of the Application with the
                *    default values that were set above.
                * 3) All other errors (MUTEX Fails, DATA Corruption, etc.): the
                *    operation did not complete; continue to use the 'previousServer'
                *    selection of the Application.           */
               serverStatus = exampleRefSelectFindBest(&newBestIndex, &newSelectedServer);

               /* Log a change of server. */
               if (exampleRefSelectAppParam.rt.selection.previousServer.connectionId != newSelectedServer.connectionId)
               {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
                        "TASK: Server Selection status (%d); Previous ConnectionId (%d): This (%d)",
                        serverStatus,
                        exampleRefSelectAppParam.rt.selection.previousServer.connectionId,
                        newSelectedServer.connectionId, 0,0,0);
               }

               if ((serverStatus == ZL303XX_OK) ||
                   (serverStatus == ZL303XX_TABLE_ENTRY_NOT_FOUND))
               {
                  /* Before using this new selection (OK or Empty), determine if a
                   * Wait-to-Restore timer needs to be started for the previous server.       */
                  exampleRefSelectHandleWtrOfServer(
                        &exampleRefSelectAppParam.rt.selection.previousServer,
                        &newSelectedServer);

                  /* Replace the data of the ACTIVE server with the new selection. */
                  exampleRefSelectAppParam.rt.selection.previousServer = newSelectedServer;
               }

               /* ELSE: Error - continue using the existing server. */
            }

            /*******************************
             * STEP 3: Re-evaluate the QL Hold-off timer.
             * - This is always done to ensure the timer value is current.
             *********************************************************/
            {
               /* Use a pointer to make the code easier to read.
                * This may be an invalid server (i.e. none selected). */
               exampleRefSelectEntryS *bestServer = &exampleRefSelectAppParam.rt.selection.previousServer;

               /* The Active QL used for Hold-Off. If no server is selected, then
                * the QL value will be '0' - invoked by the INIT routine - and the
                * Hold-Off will deactivate below. */
               Uint32T lastBestQl = bestServer->protocol.bmcaData.grandmasterClockQuality.clockClass;

               ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 3,
                     "TASK: HO: STEP 2: Evaluating Hold-Off Timer: Sec(%d), Active(%d), Tick(%d), HoQL(%d), BestQL(%d)",
                     exampleRefSelectAppParam.config.protection.holdOffSec,
                     exampleRefSelectAppParam.rt.protection.holdOff.active,
                     exampleRefSelectAppParam.rt.protection.holdOff.tickStart,
                     exampleRefSelectAppParam.rt.protection.holdOff.qlValue,
                     lastBestQl, 0);

               /* Disable the Hold-Off functionality in either of the following cases:
                * - the Hold-Off Timer is Disabled (possibly mid-cycle via the API).
                * - the BMC Algorithm is set to DEFAULT (possibly mid-cycle via the API).
                * Otherwise, Hold-Off needs to be evaluated. However, deactivate in
                * the following cases:
                * - the current selected QL is better than the Hold-Off QL (whether HO is
                *   ACTIVE or not, the same steps are performed).   */
               if ((exampleRefSelectAppParam.config.protection.holdOffSec == 0) ||
                   (exampleRefSelectAppParam.config.selection.bmcAlg == REF_SELECT_BMCA_DEFAULT_PROFILE) ||
                   (exampleRefSelectAppParam.config.selection.bmcAlg == REF_SELECT_BMCA_POWER_PROFILE_V1) ||
                   (lastBestQl <= exampleRefSelectAppParam.rt.protection.holdOff.qlValue))
               {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
                        "TASK: Hold-Off Timer : Disabling.", 0,0,0,0,0,0);

                  exampleRefSelectAppParam.rt.protection.holdOff.active = ZL303XX_FALSE;
                  exampleRefSelectAppParam.rt.protection.holdOff.qlValue = bestServer->protocol.bmcaData.grandmasterClockQuality.clockClass;
                  exampleRefSelectAppParam.rt.timer.holdOffRemainingTicks = REF_SELECT_TIMER_DISABLED;
               }

               /* ELSE: This is a valid BMCA (profile) with Hold-Off programmed and a
                *       drop in QL. Re-evaluate the required state and QL actions. */
               else
               {
                  /* CPU Tick counters for remaining Ticks calculation. */
                  Uint32T totalTicks = exampleRefSelectAppParam.config.protection.holdOffSec * OS_TICK_RATE_GET();
                  Uint32T expiredTicks;

                  /* If this is the start of a Hold-Off cycle, set Initial values. */
                  if (exampleRefSelectAppParam.rt.protection.holdOff.active == ZL303XX_FALSE)
                  {
                     ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
                           "TASK: Hold-Off Timer ACTIVATING.", 0,0,0,0,0,0);

                     exampleRefSelectAppParam.rt.protection.holdOff.active = ZL303XX_TRUE;
                     exampleRefSelectAppParam.rt.protection.holdOff.tickStart = OS_TICK_GET();
                     exampleRefSelectAppParam.rt.timer.holdOffRemainingTicks = totalTicks;
                  }

                  /* Calculate the remaining ticks in all cases. */
                  expiredTicks = (OS_TICK_GET() - exampleRefSelectAppParam.rt.protection.holdOff.tickStart);

                  ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 3,
                        "  StartTick(%d); CurrentTick(%d); ExpiredTicks(%d); HoldoffTicks(%d)",
                        exampleRefSelectAppParam.rt.protection.holdOff.tickStart,
                        OS_TICK_GET(),
                        expiredTicks,
                        totalTicks, 0,0);

                  /* If the Hold-Off timer is expired, deactivate the timer.  */
                  if (expiredTicks >= totalTicks)
                  {
                     ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
                           "TASK: Hold-Off Timer : expired.", 0,0,0,0,0,0);

                     exampleRefSelectAppParam.rt.protection.holdOff.active = ZL303XX_FALSE;
                     exampleRefSelectAppParam.rt.protection.holdOff.qlValue = bestServer->protocol.bmcaData.grandmasterClockQuality.clockClass;
                     exampleRefSelectAppParam.rt.timer.holdOffRemainingTicks = REF_SELECT_TIMER_DISABLED;
                  }
                  else
                  {
                     /* Calculate the number of remaining Ticks. */
                     exampleRefSelectAppParam.rt.timer.holdOffRemainingTicks = (totalTicks - expiredTicks);

                     ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 3,
                           "  Hold-Off Timer Updated : RemainingTicks (%d)",
                           exampleRefSelectAppParam.rt.timer.holdOffRemainingTicks, 0,0,0,0,0);
                  }
               }
            }

            /*******************************
             * STEP 4: Propagate data to the Line-Cards and other Modules.
             * - This is only done if Automatic Selection is enabled.
             *********************************************************/
            if (exampleRefSelectAppParam.config.selection.autoRun == ZL303XX_TRUE)
            {
               exampleRefSelectBpMsgS txMsg;

               /* UPDATE message from the Application. */
               txMsg.source = REF_SELECT_SRC_MOD_APP;
               txMsg.action = REF_SELECT_ACTION_UPDATE;
               txMsg.data.bestRef = exampleRefSelectAppParam.rt.selection.previousServer;

               /* Update the data set with any QL Hold-over value (if Active). */
               if (exampleRefSelectAppParam.rt.protection.holdOff.active == ZL303XX_TRUE)
               {
                  txMsg.data.bestRef.protocol.bmcaData.grandmasterClockQuality.clockClass = exampleRefSelectAppParam.rt.protection.holdOff.qlValue;
                  txMsg.data.bestRef.protocol.telecomData.clockClass = exampleRefSelectAppParam.rt.protection.holdOff.qlValue;
               }

               /* - Update each Line card with the proper selection.
                * - Update the algorithm with the selected reference. */
               (void)exampleRefSelectSendMsgToBackplane(&txMsg);
            }
         }
         OS_TASKMON_FUNC_END();
      }

      /* For some reason, the read of the Message Queue has failed (status != OK). */
      /* Print an error message, delay the task and then continue.  */
      else
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
               "exampleRefSelectTaskFn: ERROR - MsgQ Read status failure (%ld)",
               status, 0,0,0,0,0);

         /* Reset the status */
         status = ZL303XX_OK;

         /* Delay the task briefly. */
         OS_TASK_DELAY(50);
      }
   }  /* END of Task WHILE LOOP */


   /* The current implementation of this task does not reach this point.
      However, if the implementation changes and this task terminates,
      then the following steps should be executed. */

   /* Delete the message queue. */
   /* (void)exampleRefSelectMsgQueueDelete(); */

   /* Delete the data mutex. */
   /*(void)exampleRefSelectMutexDelete();*/

   /* Reset the TaskId */

   /* Task quitting  */
   /*ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2, "exampleRefSelectTaskFn: Task quit ", 0,0,0,0,0,0);*/

   return status;
}  /* END exampleRefSelectTaskFn */

/**

  Function Name:
    exampleRefSelectMsgQueueCreate

  Details:
   Creates an event message queue for the task.

  Parameters:
        None

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE exampleRefSelectMsgQueueCreate(void)
{
   zlStatusE status = ZL303XX_OK;

   /* Make sure the queue is not already created */
   if (exampleRefSelectMsgQId != OS_MSG_Q_INVALID)
   {
      /* If the queue already exist. NOTIFY the user of the scenario but do not
         return an error since the state of the queue is what the user wants. */
      ZL303XX_ERROR_NOTIFY("exampleRefSelectMsgQueueCreate: message queue exists");
   }
   else  /* Create the queue */
   {
      exampleRefSelectMsgQId = OS_MSG_Q_CREATE(
                                    REF_SELECT_DB_MSGQ_SIZE,
                                    sizeof(exampleRefSelectMsgS));

      /* Make sure the queue creation was successful */
      if (exampleRefSelectMsgQId == OS_MSG_Q_INVALID)
      {
         ZL303XX_ERROR_NOTIFY("exampleRefSelectMsgQueueCreate: queue creation failed");

         status = ZL303XX_RTOS_MSGQ_CREATE_FAIL;
      }
   }

   return status;
}  /* END exampleRefSelectMsgQueueCreate */

/**

  Function Name:
    exampleRefSelectMsgQueueDelete

  Details:
   Deletes the event message queue of the task.

  Parameters:
        None

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE exampleRefSelectMsgQueueDelete(void)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 4,
         "exampleRefSelectMsgQueueDelete", 0, 0, 0, 0, 0, 0);

   /* Check if the queue was created */
   if (exampleRefSelectMsgQId == OS_MSG_Q_INVALID)
   {
      /* The queue does not exist. NOTIFY the user of the scenario but do not
         return an error since the state of the queue is what the user wants. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectMsgQueueDelete: queue does not exist",
            0,0,0,0,0,0);
   }
   else  /* Delete the queue */
   {
      OS_STATUS osStatus = OS_MSG_Q_DELETE(exampleRefSelectMsgQId);

      /* Make sure we were able to delete the queue */
      if (osStatus != OS_OK)
      {
         ZL303XX_ERROR_NOTIFY("exampleRefSelectMsgQueueDelete: Error deleting queue");

         status = ZL303XX_RTOS_MSGQ_DELETE_FAIL;
      }
      else  /* Reset the queue id */
      {
         exampleRefSelectMsgQId = OS_MSG_Q_INVALID;
      }
   }

   return status;
}  /* END exampleRefSelectMsgQueueDelete */

/**

  Function Name:
    exampleRefSelectMsgQueueRead

  Details:
   Waits for a message to be received on the server database message queue.

  Parameters:
   [in]  msg   Pointer to a exampleRefSelectMsgS structure.

   [out] msg   The received message

  Return Value:
   zlStatusE

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleRefSelectMsgQueueRead(exampleRefSelectMsgS *msg)
{
   /* Check input parameters */
   zlStatusE status = ZL303XX_OK;

   /* Check that the Rx event queue has been configured */
   if (status == ZL303XX_OK)
   {
      if (exampleRefSelectMsgQId == OS_MSG_Q_INVALID)
      {
         ZL303XX_ERROR_NOTIFY("exampleRefSelectMsgQueueRead: queue does not exist");
         status = ZL303XX_INIT_ERROR;
      }
      else /* Receive the message from the queue */
      {
         Uint32T nBytes;

         /* First, determine how long to wait for a message. If the REFRESH timer
          * is OFF but the Hold-Off or WTR timers are ACTIVE, then set the time-out
          * for the lowest value. If not, the HO or WTR time may pass and never process
          * since the refresh is OFF. Any other timers should also be incorporated. */
         Uint32T minTimerValue = WAIT_TICKS(
               exampleRefSelectAppParam.rt.timer.appRefreshTicks,
               exampleRefSelectAppParam.rt.timer.holdOffRemainingTicks,
               exampleRefSelectAppParam.rt.timer.wtrRemainingTicks);

         /* Wait on a message. */
         nBytes = OS_MSG_Q_RECEIVE(exampleRefSelectMsgQId,
               (Sint8T *)msg,
               sizeof(exampleRefSelectMsgS),
               minTimerValue);

         /* Check if we actually received a message */
         if (nBytes == (Uint32T)OS_ERROR)
         {
            /* No message to process just now */
            msg->sourceId = REF_SELECT_SRC_MOD_UNKNOWN;
            msg->device = (void *)NULL;
         }
      }
   }

   return status;
}  /* END exampleRefSelectMsgQueueRead */

/**
  Function Name:
   exampleRefSelectMsgQueueSend

  Details:
   Sends an event message into the server database message queue

  Parameters:
   [in]  msg   Pointer to a exampleRefSelectMsgS message to send.

  Return Value:
   zlStatusE

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleRefSelectMsgQueueSend(exampleRefSelectMsgS *msg)
{
   /* Check input parameters */
   zlStatusE status = ZL303XX_OK;

   /* Check that the Task is Running & the Rx event queue has been configured */
   if (exampleRefSelectTaskId == OS_TASK_INVALID)
   {
      /* The Task is not yet running. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectMsgQueueSend: Task is not running", 0,0,0,0,0,0);

      status = ZL303XX_NOT_RUNNING;
   }
   else if (exampleRefSelectMsgQId == OS_MSG_Q_INVALID)
   {
      /* The Message Queue is not created yet. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectMsgQueueSend: MSGQ does not exist", 0,0,0,0,0,0);

      status = ZL303XX_RTOS_MSGQ_INVALID;
   }

   /* Send the message into the queue */
   else
   {
       osStatusT osStatus;
       /* Queue the message. If there is no space then discard the message.
         This saves it from becoming overloaded */
       if ((osStatus = OS_MSG_Q_SEND(exampleRefSelectMsgQId,
               (Sint8T *)msg, sizeof(exampleRefSelectMsgS),
               OS_NO_WAIT)) != OS_OK)
       {
          ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
                "exampleRefSelectMsgQueueSend: Error = %d sending message (%d)\n",
                osStatus, errno, 0,0,0,0);

           status = ZL303XX_RTOS_MSGQ_SEND_FAIL;
       }
   }

   return status;
}  /* END exampleRefSelectMsgQueueSend */

/*****************   END   ****************************************************/

