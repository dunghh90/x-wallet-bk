

/***************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
* Copyright 2006-2018 Microsemi Semiconductor Limited.
* All rights reserved.
*
*  Module Description:
*     This file contains interface for a PTP clock data structure
*
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"
#include "zl303xx_ErrTrace.h"
#include "zl303xx_List.h"

#include "zl303xx_PtpInternal.h"
#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpSetup.h"
#include "zl303xx_PtpForeignMasterTbl.h"
#include "zl303xx_PtpControl.h"
#include "zl303xx_PtpProfileDef.h"

/*****************   DEFINES     **********************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/
typedef struct
{
   Uint32T offset;
   Uint32T size;
} zl303xx_PtpParamS;

#define PARAM_OFFSET_SIZE(param)  {offsetof(zl303xx_PtpClockS, param), sizeof(((zl303xx_PtpClockS *)0)->param)}

static zl303xx_PtpParamS Zl303xx_PtpClockParam[CLOCK_PARAM_NUM_TYPES] = {
      PARAM_OFFSET_SIZE(config.ingressClockClass),
      PARAM_OFFSET_SIZE(config.egressClockClass),
      PARAM_OFFSET_SIZE(config.egressClassUpdateTrigger)
};

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/* Structure check functions. */
static zlStatusE zl303xx_PtpClockCreateStructCheck(zl303xx_PtpClockCreateS *clockCreate);
static zlStatusE zl303xx_PtpClockDeleteStructCheck(zl303xx_PtpClockDeleteS *pDelete);

/* Verification routine for over writing Clock Data Sets. */
static zlStatusE zl303xx_PtpClockCheckDefaultDS(
      zl303xx_PtpClockCreateS *pConfig,
      zl303xx_DefaultDS *pDefaultDS);

/* Config functions. */
static void zl303xx_PtpClockConfigApply(
      zl303xx_PtpClockS *pClock,
      zl303xx_PtpClockCreateS *pConfig);
static void zl303xx_PtpClockConfigRevert(
      zl303xx_PtpClockS *pClock,
      zl303xx_PtpClockCreateS *pNewCfg);

/* Routines to update Clock Data Sets. Will trigger Event notifications if required. */
static void zl303xx_PtpClockTimePropertiesApply(
      zl303xx_PtpClockS *pClock,
      zl303xx_TimePropertiesDS *newTimeProperties);

/* Clock State Control Routines. */
zlStatusE zl303xx_PtpClockStateAudit(
      zl303xx_PtpClockS *pPtpClock);

void zl303xx_PtpClockStateToS0(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_ClockStreamS *pBestStream);
void zl303xx_PtpClockStateToS1M3(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_ClockStreamS *pBestStream);
void zl303xx_PtpClockStateToM1(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_ClockStreamS *pBestStream);
void zl303xx_PtpClockStateToM2(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_ClockStreamS *pBestStream);

/* Other Data Set Update routines. */
void zl303xx_PtpV2UseLocalForClockDS(
      zl303xx_PtpClockS *pPtpClock);
void zl303xx_PtpV2UseStreamForClockDS(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpClockS *pPtpClock);
void zl303xx_PtpV2UseExternalForClockDS(
      zl303xx_PtpClockS *pPtpClock);

/* Clock Timer Routines. */
/* Callback Routine on Timer Expiry. */
static void zl303xx_PtpTimerBmcaUpdateInterval(
      zl303xx_PtpTimerS *pTimer);

/* Determine the fastest ANNC stream. */
static Sint8T zl303xx_PtpClockFindFastestAnncInterval(
      zl303xx_PtpClockS *pPtpClock);

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXTERNAL GLOBAL VARIABLES   ******************************/
extern zl303xx_PtpClockS *Zl303xx_PtpClockArray[ZL303XX_PTP_NUM_CLOCKS_MAX];
extern zl303xx_PortDataS *Zl303xx_PtpPortArray[ZL303XX_PTP_NUM_PORTS_MAX];
extern zl303xx_ClockStreamS *Zl303xx_PtpStreamArray[ZL303XX_PTP_NUM_STREAMS_MAX];



/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* zl303xx_PtpClockCreateStructInit */
/**
   Fills a zl303xx_PtpClockCreateS structure with the default configuration for
   initializing a PTP clock.

  Parameters:
   [out]  pClockCreate  Default parameters for creating a PTP clock.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pClockCreate is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpClockCreateStructInit(zl303xx_PtpClockCreateS *pClockCreate)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpClockCreateStructInit:", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pClockCreate);
   }

   /* Assign default values. */
   if (status == ZL303XX_OK)
   {
      /* The clockType member can be set prior to the initialization process so
       * that different values may be set by the profile depending on the expected
       * purpose of the clock. So, maintain this value.       */
      Uint16T origClockType = pClockCreate->clockType;

      /* Clear all values in the structure. */
      OS_MEMSET(pClockCreate, 0, sizeof(*pClockCreate));

      /* Restore the requested clockType. */
      pClockCreate->clockType = origClockType;

      /* Task parameters. */
      strncpy(pClockCreate->taskName, ZL303XX_PTP_DEFAULT_CLOCK_TASK_NAME,
              sizeof(pClockCreate->taskName) - 1);
      pClockCreate->taskPriority  = ZL303XX_PTP_DEFAULT_CLOCK_TASK_PRIORITY;
      pClockCreate->taskStackSize = ZL303XX_PTP_DEFAULT_CLOCK_TASK_STACK_SIZE;
      pClockCreate->msgQLength    = ZL303XX_PTP_DEFAULT_CLOCK_MSG_Q_LENGTH;

      /* Clock Configuration Parameters... */
      pClockCreate->profile       = ZL303XX_PTP_PROFILE_DEFAULT;
      pClockCreate->profileStrict = ZL303XX_PTP_DEFAULT_CLOCK_PROFILE_STRICT;

      /* Use the available Default Parameters routine. */
      (void)zl303xx_PtpClockDefaultDSDefaultParams(&pClockCreate->defaultDS);

      /* Use the available Default Parameters routine. */
      (void)zl303xx_PtpTimePropertiesDefaultParams(&pClockCreate->localTimeProperties);

      /* Set the maximum Foreign Master Table size. */
      pClockCreate->maxForeignRecords = ZL303XX_PTP_DEFAULT_CLOCK_MAX_FOREIGN_REC;

      /* Set BMCA update event interval */
      pClockCreate->bmca.updateTrigger = ZL303XX_PTP_DEFAULT_CLOCK_EVT_BMCA_TRIGGER;
      /* Set BMCA update event interval */
      pClockCreate->bmca.updateEventSec = ZL303XX_PTP_DEFAULT_CLOCK_EVT_BMCA_UPDATE_SEC;
      /* Set revertive mode for BMCA */
      pClockCreate->bmca.revertiveEn = ZL303XX_PTP_DEFAULT_CLOCK_REVERTIVE_EN;

      /* Telecom Profile-specific members. */
      pClockCreate->telecom.waitToRestoreSec = ZL303XX_PTP_DEFAULT_CLOCK_WTR;
      pClockCreate->telecom.qlHoldOffSec     = ZL303XX_PTP_DEFAULT_CLOCK_QL_HOLD_OFF;
      pClockCreate->telecom.evtSquelchEn     = ZL303XX_PTP_DEFAULT_CLOCK_EVT_SQUELCH_EN;

      /* Disable the override class values. */
      pClockCreate->ingressClockClass        = PTP_CLOCK_CLASS_OVERRIDE_DISABLED;
      pClockCreate->egressClockClass         = PTP_CLOCK_CLASS_OVERRIDE_DISABLED;
      pClockCreate->egressClassUpdateTrigger = ZL303XX_EGRESS_QL_UPDATE_ON_QUALIFY;

      /* Clock MAX-Steps Removed configuration. */
      pClockCreate->optionalDefaultDS.maxStepsRemoved = ZL303XX_PTP_DEFAULT_CLOCK_STEPS_REMOVED_MAX;

      /* Clock UNCALIBRATED configuration. */
      pClockCreate->uncalibrated.usePreviousClass = ZL303XX_FALSE;

      /* Unicast negotiation parameters. */
      pClockCreate->unicastNegotiation.maxAnnounceCount   = ZL303XX_PTP_DEFAULT_CLOCK_UNI_NEG_ANNC_MAX;
      pClockCreate->unicastNegotiation.maxSyncCount       = ZL303XX_PTP_DEFAULT_CLOCK_UNI_NEG_SYNC_MAX;
      pClockCreate->unicastNegotiation.maxDelayRespCount  = ZL303XX_PTP_DEFAULT_CLOCK_UNI_NEG_DELAY_MAX;
      pClockCreate->unicastNegotiation.maxPdelayRespCount = ZL303XX_PTP_DEFAULT_CLOCK_UNI_NEG_PDELAY_MAX;

      /* Unicast Service limits. */
      pClockCreate->unicastNegotiation.totalPpsMax[ZL303XX_MSG_ID_SYNC]            = ZL303XX_PTP_DEFAULT_CLOCK_UNI_NEG_LIMIT_SYNC;
      pClockCreate->unicastNegotiation.totalPpsMax[ZL303XX_MSG_ID_ANNOUNCE]        = ZL303XX_PTP_DEFAULT_CLOCK_UNI_NEG_LIMIT_ANNC;
      pClockCreate->unicastNegotiation.totalPpsMax[ZL303XX_MSG_ID_DELAY_RESP]      = ZL303XX_PTP_DEFAULT_CLOCK_UNI_NEG_LIMIT_DELAY;
      pClockCreate->unicastNegotiation.totalPpsMax[ZL303XX_MSG_ID_PEER_DELAY_RESP] = ZL303XX_PTP_DEFAULT_CLOCK_UNI_NEG_LIMIT_PDELAY;

      pClockCreate->eventCallback   = NULL;
      pClockCreate->hwCmdFn         = NULL;
      pClockCreate->extData         = NULL;
      pClockCreate->requestedHandle = (zl303xx_PtpClockHandleT)ZL303XX_PTP_INVALID;

      /* Stream update mode:: TRUE = automatic; FALSE = manual. */
      pClockCreate->autoUpdateStreamStates = ZL303XX_TRUE;

      /* Optional PATH-TRACE functionality. */
      pClockCreate->pathTrace.enabled = ZL303XX_PTP_DEFAULT_CLOCK_PATH_TRACE_ENABLED;
      /* Optional Synchronization-Uncertain functionality. */
      pClockCreate->synchronizationUncertain.enabled = ZL303XX_PTP_DEFAULT_CLOCK_SYNCHRO_UNCERTAIN_EN;

   }

   return status;
}

/* zl303xx_PtpClockCreate */
/**
   Creates a PTP clock instance and associated OS task(s). Each PTP clock
   will have its own task in addition to one common timer task for all
   PTP clocks managed by the CPU.

   A PTP clock represents a local oscillator that provides time information.
   This PTP clock may drive a timing signal to multiple PTP ports, which
   may each have multiple streams, which may master packet streams or
   slave to them.

  Parameters:
   [in]   pClockCreate  Clock configuration parameters.
   [out]  pClockHandle  Handle assigned to this clock. used to reference it
                             when calling other functions.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       pClockCreate or pClockHandle is NULL.
     ZL303XX_PARAMETER_INVALID     A member of pClockCreate is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.
     ZL303XX_TABLE_FULL            Maximum number of clocks already created.
     ZL303XX_RTOS_MEMORY_FAIL      Failed to allocate memory for clock data.

*******************************************************************************/
zlStatusE zl303xx_PtpClockCreate(zl303xx_PtpClockCreateS *pClockCreate,
                                 zl303xx_PtpClockHandleT *pClockHandle)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpClockCreate:", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pClockCreate) |
               ZL303XX_CHECK_POINTER(pClockHandle);
   }

   /* Check the clock creation parameters. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockCreateStructCheck(pClockCreate);
   }

   /* Check that the clockIdentity is unique. This isn't checked in the StructCheck()
    * function because that function is also called when reconfiguring a clock. In
    * the reconfig case, having the same clockIdentity is expected, not an error. */
   if (status == ZL303XX_OK)
   {
      if (zl303xx_PtpNodeClockIdentityExists(pClockCreate->defaultDS.clockIdentity) != NULL)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) A clock with this identity already exists.", 0,0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Allocate memory for the clock structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockStructAllocate(pClockCreate, &pPtpClock);
   }

   /* Create the mutex for this clock. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexCreate(pPtpClock);
   }

   if (status == ZL303XX_OK)
   {
      /* Use the provided parameters to configure the clock structure. */
      zl303xx_PtpClockConfigApply(pPtpClock, pClockCreate);

      /* Add the clock to the node's clockList. This also populates the index member,
       * which is needed by zl303xx_PtpClockTaskStart(). */
      status = zl303xx_PtpNodeAttachClock(pPtpClock);
   }

   /* Create the message queue for this clock. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMsgQCreate(pPtpClock);
   }

   /* Start the OS task for this clock. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockTaskStart(pPtpClock);
   }

   if (status == ZL303XX_OK)
   {
      /* Wait a MAX of 40 loops for the clock creation to complete (40 * 50ms = 2 Sec). */
      Uint8T maxWaitLoops = 2 * 20;

      /* Start the Clock Timers: Currently just a BMCA UPDATE Interval Timer */
      zl303xx_PtpClockTimersInit(pPtpClock);

      /* Enable clock's RX/TX */
      pPtpClock->enableRxTx = ZL303XX_TRUE;

      /* Return the handle to this clock structure */
      *pClockHandle = pPtpClock->clockHandle;

      /* Wait for the task to read the new clock pointer before returning. */
      while (zl303xx_PtpNodeLastClockExists() == ZL303XX_TRUE)
      {
         if (--maxWaitLoops == 0)
         {
            status = ZL303XX_TIMEOUT;
            break;
         }
         else
         {
            OS_TASK_DELAY(50);
         }
      }
   }

   /* Execute any Profile Specific actions related to the clock (may be NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpProfileClockPostCreate(pPtpClock);
   }

   if (status == ZL303XX_OK)
   {
      /* Issue an event that the Clock has been created. */
      (void)zl303xx_PtpEventClockCreate(pPtpClock);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "   PTP clock (%d) created successfully.",
            pPtpClock->clockHandle, 0,0,0,0,0);
   }
   else
   {
      /* The Clock failed to create at some point along the way.
       * FREE any associated memory and delete the object from the Node.  */
      ZL303XX_TRACE_ALWAYS(
            "   PTP clock creation FAILED: status = %d.", status, 0,0,0,0,0);

      /* Ignore any return code from the DELETE but return the original status. */
      if (pPtpClock != NULL)
      {
         zl303xx_PtpClockDeleteS clockDelete;
         (void)zl303xx_PtpClockDeleteStructInit(&clockDelete);
         (void)zl303xx_PtpClockDeleteInternal(pPtpClock, &clockDelete);
      }

      /* We expect the above call to free the clock memory but in case it had an
       * error, Free the memory manually. */
      if (pPtpClock != NULL)
      {
         OS_FREE(pPtpClock);
         pPtpClock = NULL;
      }
   }

   return status;
}

/* zl303xx_PtpClockDeleteStructInit */
/**
   Fills a zl303xx_PtpClockDeleteS structure with the default configuration for
   deleting a PTP clock.

  Parameters:
   [out]  pClockDelete  Default parameters to shut down a PTP clock.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pClockDelete is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpClockDeleteStructInit(zl303xx_PtpClockDeleteS *pClockDelete)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpClockDeleteStructInit:", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pClockDelete);
   }

   /* Initialize the clock shutdown structure. */
   if (status == ZL303XX_OK)
   {
      OS_MEMSET(pClockDelete, 0, sizeof(*pClockDelete));

      pClockDelete->forceDelete = ZL303XX_PTP_DEFAULT_CLOCK_FORCE_DELETE;
   }

   return status;
}

/* zl303xx_PtpClockDelete */
/**
   Deletes a PTP clock and its associated OS task(s).

  Parameters:
   [in]  clockHandle   Handle to an existing PTP clock.
   [in]  pClockDelete  (Optional) Parameters to shut down this PTP clock.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockDelete(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpClockDeleteS *pClockDelete)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* PTP Event Notification parameters. */
   zl303xx_PtpEventClockDeleteS eventData;
   zl303xx_PtpEventFnT eventCallback;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpClockDelete: clockHandle=%lu", clockHandle, 0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      if ((status = ZL303XX_CHECK_CLOCK_HANDLE(clockHandle)) != ZL303XX_OK)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Invalid clockHandle=%lu.", clockHandle, 0,0,0,0,0);
      }
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDeleteStructCheck(pClockDelete);
   }

   if (status == ZL303XX_OK)
   {
      /* Get a pointer to the clock data structure. Don't call
       * zl303xx_PtpClockDataGet() because we want to exit the function with
       * ZL303XX_OK if the clock does not exist. */
      pPtpClock = Zl303xx_PtpClockArray[clockHandle];

      if (pPtpClock == NULL)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "   No clock with clockHandle=%lu exists.",
               clockHandle, 0,0,0,0,0);

         /* Return early to keep the code easier to flow. */
         return status;
      }
      else
      {
         /* TAKE the Clock MUTEX.
          * IF the deletion of the clock is successful, then the MUTEX will also
          * be deleted and no MUTEX release will be required. If and error
          * occurs, then we may have to try and release the MUTEX. */
         mStatus = zl303xx_PtpClockMutexLock(pPtpClock);
         status = mStatus;
      }
   }

   /* If we are here, then the clock actually exists. */
   if (status == ZL303XX_OK)
   {
      eventData.clockHandle  = pPtpClock->clockHandle;
      eventData.clockExtData = pPtpClock->config.extData;
      eventCallback          = pPtpClock->config.eventCallback;

      /* Call the internal routine to delete this Clock. */
      status = zl303xx_PtpClockDeleteInternal(pPtpClock, pClockDelete);

      /* Send a Clock Delete Event. */
      if ((status == ZL303XX_OK) && (eventCallback != NULL))
      {
         eventCallback(ZL303XX_PTP_EVENT_CLOCK_DELETE, &eventData);
      }
   }

   /* If we managed to take the MUTEX and the clock deletion was successful then
    * there is no Clock MUTEX left to release.
    * Only attempt an UNLOCK if an error occurred at the clock deletion. */
   if ((mStatus == ZL303XX_OK) &&
       (status != ZL303XX_OK))
   {
      if ((pPtpClock != NULL) &&
          (pPtpClock->mutexId != OS_MUTEX_INVALID))
      {
         status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
      }
   }

   return status;
}  /* END zl303xx_PtpClockDelete */

/* zl303xx_PtpClockConfigGet */
/**
   Returns the current configuration parameters of a PTP clock.

  Parameters:
   [in]   clockHandle   Handle to an existing clock.
   [out]  pClockConfig  A copy of the clock's current configuration.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pClockConfig is NULL.
     ZL303XX_PARAMETER_INVALID  clockHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockConfigGet(zl303xx_PtpClockHandleT clockHandle,
                                    zl303xx_PtpClockCreateS *pClockConfig)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pClockConfig);
   }

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Return a copy of the configuration structure. */
   if (status == ZL303XX_OK)
   {
      *pClockConfig = pPtpClock->config;
   }

   return status;
}

/* zl303xx_PtpClockConfigSet */
/**
   Updates a clock's configuration parameters. All ports attached to the clock
   must be deleted prior to calling this function.

   The following parameters cannot be modified using this function:
      - taskName
      - taskPriority
      - taskStackSize
      - msgQLength
      - defaultDS::clockIdentity
      - defaultDS::numberPorts
      - requestedHandle

   To change these parameters, the clock must be deleted and re-created.

  Parameters:
   [in]      clockHandle   Handle to an existing clock.
   [in,out]  pClockConfig  New clock configuration to apply. Parameters that
                                cannot be changed will be reverted to the clock's
                                existing values.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       pClockConfig is NULL.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid.
     ZL303XX_INVALID_OPERATION     There are still ports attached to the clock.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockConfigSet(zl303xx_PtpClockHandleT clockHandle,
                                    zl303xx_PtpClockCreateS *pClockConfig)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   const char *fnName = "zl303xx_PtpClockConfigSet";
   zl303xx_PtpClockS *pClock;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pClockConfig);
   }
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockCreateStructCheck(pClockConfig);
   }

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pClock);
   }

   /* Cannot reconfig the clock if there are still ports attached to it. */
   if (status == ZL303XX_OK)
   {
      if (pClock->portCount > 0)
      {
         ZL303XX_TRACE_ERROR("%s: Clock %lu still has %u port(s) attached.",
                             fnName, pClock->clockHandle, pClock->portCount, 0,0,0);
         status = ZL303XX_INVALID_OPERATION;
      }
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pClock);
      status = mStatus;
   }

   if (status == ZL303XX_OK)
   {
      /* Revert any parameters that can't be changed and update the clock's config.  */
      zl303xx_PtpClockConfigRevert(pClock, pClockConfig);
      zl303xx_PtpClockConfigApply(pClock, pClockConfig);

      /* Resend the Foreign Master Table on a config change. */
      zl303xx_PtpClockBmcaUpdateInitiate(pClock);
   }

   /* Release the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pClock, status);
   }

   return status;
}

/* zl303xx_PtpClockExtDataGet */
/**
   Returns the external data pointer associated with a clock.

  Parameters:
   [in]   clockHandle   Handle to an existing clock.
   [out]  ppExtData     Pointer to user-specified data structure.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    ppExtData is NULL.
     ZL303XX_PARAMETER_INVALID  clockHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockExtDataGet(zl303xx_PtpClockHandleT clockHandle,
                                     void **ppExtData)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(ppExtData);
   }

   /* Get a pointer to the clock data structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Return the pointer to user data. */
   if (status == ZL303XX_OK)
   {
      *ppExtData = pPtpClock->config.extData;
   }

   return status;
}

/* zl303xx_PtpClockExtDataSet */
/**
   Sets the external data pointer associated with a clock.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   pExtData     User-specified data structure.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockExtDataSet(zl303xx_PtpClockHandleT clockHandle,
                                     void *pExtData)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pClock;

   /* Don't bother checking pExtData, as the user may want to set it to NULL. */

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pClock);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pClock);
   }

   if (status == ZL303XX_OK)
   {
      pClock->config.extData = pExtData;
      status = zl303xx_PtpClockMutexUnlock(pClock, status);
   }

   return status;
}

/* zl303xx_PtpClockEventCallbackSet */
/**
   Sets the event callback function for a clock.

  Parameters:
   [in]   clockHandle    Handle to an existing clock.
   [in]   eventCallback  New callback function to use (may be NULL).

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockEventCallbackSet(zl303xx_PtpClockHandleT clockHandle,
                                           zl303xx_PtpEventFnT eventCallback)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pClock = NULL;

   /* It is OK for eventCallback to be NULL. */

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pClock);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pClock);
   }

   /* Update the eventCallback; unlock mutex. */
   if (status == ZL303XX_OK)
   {
      pClock->config.eventCallback = eventCallback;

      status = zl303xx_PtpClockMutexUnlock(pClock, status);
   }

   return status;
}

/* zl303xx_PtpClockHardwareCmdFnSet */
/**
   Sets the hardware command handler for an existing clock.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   hwCmdFn      New command handler function to use.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       hwCmdFn is NULL.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockHardwareCmdFnSet(zl303xx_PtpClockHandleT clockHandle,
                                           zl303xx_PtpHwCmdFnT hwCmdFn)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pClock = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(hwCmdFn);
   }

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pClock);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pClock);
   }

   /* Update the hwCmdFn; unlock mutex. */
   if (status == ZL303XX_OK)
   {
      pClock->config.hwCmdFn = hwCmdFn;

      status = zl303xx_PtpClockMutexUnlock(pClock, status);
   }

   return status;
}

/* zl303xx_PtpClockEnabledGet */
/**
   Returns the enabled status of a clock. A clock is considered enabled if one
   or more ports is attached to it.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [out]  pEnabled     ZL303XX_TRUE if the clock is enabled. ZL303XX_FALSE otherwise.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pEnabled is NULL.
     ZL303XX_PARAMETER_INVALID  clockHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockEnabledGet(zl303xx_PtpClockHandleT clockHandle,
                                     zl303xx_BooleanE *pEnabled)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pClock = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pEnabled);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pClock);
   }

   if (status == ZL303XX_OK)
   {
      *pEnabled = (pClock->portCount > 0) ? ZL303XX_TRUE : ZL303XX_FALSE;
   }

   return status;
}

/* zl303xx_PtpClockCurrentDSGet */
/**
   Returns the currentDS of a clock.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [out]  pCurrentDS   The currentDS of the clock.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pCurrentDS is NULL.
     ZL303XX_PARAMETER_INVALID  clockHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockCurrentDSGet(zl303xx_PtpClockHandleT clockHandle,
                                       zl303xx_CurrentDS *pCurrentDS)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pCurrentDS);
   }

   /* Get a pointer to the clock structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Copy the CurrentDS members to the supplied structure. Some conversions
    * are required. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpConvertCurrentDSInternal(&pPtpClock->currentDSInt, pCurrentDS);
   }

   return status;
}

/* zl303xx_PtpClockParentDSGet */
/**
   Returns the parentDS of a clock.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [out]  pParentDS    The parentDS of the clock.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pParentDS is NULL.
     ZL303XX_PARAMETER_INVALID  clockHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockParentDSGet(zl303xx_PtpClockHandleT clockHandle,
                                      zl303xx_ParentDS *pParentDS)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pParentDS);
   }

   /* Get a pointer to the clock structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Return the clock's parentDS. */
   if (status == ZL303XX_OK)
   {
      *pParentDS = pPtpClock->parentDS;
   }

   return status;
}  /* END zl303xx_PtpClockParentDSGet */

/* zl303xx_PtpClockParentDSSet */
/**
   This routine is kept for backwards-compatibility. It call the new routine
   zl303xx_PtpClockParentDsSourceSet to perform necessary actions.

  Parameters:
   [in]  clockHandle    Handle to an existing clock.
   [in]  pParentDS      The parentDS to apply to the clock. If NULL, the the
                              defaultDS of the clock is used as the parentDS.
   [in]  receivingPortIdentity   The source port that received the ParentDS
                                       information.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or a member of pParentDS is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockParentDSSet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_ParentDS *pParentDS,
      zl303xx_PortIdentity *receivingPortIdentity)
{
   zl303xx_PtpClockServerSourceS serverSource;

   if (receivingPortIdentity != NULL)
   {
      serverSource.receivingPortIdentity = *receivingPortIdentity;
      serverSource.receivingClockHandle = (Uint32T)(-1);
      serverSource.receivingStreamHandle = (Uint32T)(-1);

      return zl303xx_PtpClockParentDsSourceSet(clockHandle, pParentDS, &serverSource);
   }
   else
   {
      return zl303xx_PtpClockParentDsSourceSet(clockHandle, pParentDS, NULL);
   }
}  /* zl303xx_PtpClockParentDSSet */


/* zl303xx_PtpClockParentDsSourceSet */
/**
   Sets the parentDS of a clock from the server source provided. This triggers
   the clock to re-evaluate each of its local streams to reset their states.

  Parameters:
   [in]  clockHandle    Handle to an existing clock.
   [in]  pParentDS      The parentDS to apply to the clock. If NULL, the the
                              defaultDS of the clock is used as the parentDS.
   [in]  rxSource       The source port that received the ParentDS information.
                              This may be a local port, an external port, or the local
                              clock (portNumber=0) indicating the defaultDS is used.
                              If NULL, it indicates that the source is unknown and the
                              clock attempts to match the ParentDS with local data.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or a member of pParentDS is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockParentDsSourceSet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_ParentDS *pParentDS,
      zl303xx_PtpClockServerSourceS *serverSource)
{
   zlStatusE status = ZL303XX_OK;

   const char *fnName = "zl303xx_PtpClockParentDsSourceSet";

   zl303xx_PtpClockS *pPtpClock = NULL;
   zl303xx_PortDataS *pPtpPort = NULL;

   /* By default, set the rxPort as External. */
   Uint16T rxPortNumber = (Uint16T)ZL303XX_PTP_INVALID;
   zl303xx_ParentDS appliedParentDS;


   /* Get a pointer to the clock structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* The following logic is applied below for assigning the appliedParentDS and
    * rxPortNumber.
                   -------------------------------------------------------
                  |                   serverSource                        |
                    ------------------------------------------------------|
     P            |     NULL    |    LOCAL    |    LOCAL    |  EXTERNAL   |
     a            |             |    CLOCK    |    PORT(X)  |             |
     r             -------------------------------------------------------|
     e   NULL or  |     restore DefaultDS     |             |             |
     n   Default  |     rxPortNumber = 0      |    ERROR    |    ERROR    |
     t             -------------------------------------------------------|
     D  pParentDS | Search CLK  |   ERROR     | Confirm     |     use     |
     S   != NULL  |             |             |   Port(X)   |  pParentDS  |
                   -------------------------------------------------------   */

   /* Check if a ParentDS is provided. If not, get the DefaultDS. */
   if (status == ZL303XX_OK)
   {
      /* Determine the source of the ParentDS. */
      /* (This is ROW-1 in the table above). */
      if ((pParentDS == NULL) ||
          (zl303xx_PtpV2EqualClockIdentities(
                     pPtpClock->config.defaultDS.clockIdentity,
                     pParentDS->parentPortIdentity.clockIdentity) == ZL303XX_TRUE))
      {
         /* This is Row-1; Columns 3 & 4 above. */
         if (serverSource != NULL)
         {
            if ((zl303xx_PtpV2EqualClockIdentities(
                           pPtpClock->config.defaultDS.clockIdentity,
                           serverSource->receivingPortIdentity.clockIdentity) == ZL303XX_FALSE) ||
                (serverSource->receivingPortIdentity.portNumber != 0))
            {
               ZL303XX_TRACE_ALWAYS("%s: Invalid receivingPortIdentity for Default restore", fnName, 0,0,0,0,0);
               status = ZL303XX_PARAMETER_INVALID;
            }
         }

         if (status == ZL303XX_OK)
         {
            if (pParentDS == NULL)
            {
               /* Recover the ParentDS from the DefaultDS. */
               zl303xx_PtpConvertDefaultDsToParentDs(&pPtpClock->config.defaultDS, &appliedParentDS);
            }
            else
            {
               appliedParentDS = *pParentDS;

               /* This implementation does not support parent statistics so set the
                * values as per IEEE-1588-2008: Section 8.2.3.4. */
               appliedParentDS.parentStats = ZL303XX_FALSE;
               appliedParentDS.observedParentOffsetScaledLogVariance = 0xFFFF;
               appliedParentDS.observedParentClockPhaseChangeRate = 0x7FFFFFFF;
            }

            /* Set the received Port ID to the clock (portNumber = 0 indicates the clock). */
            rxPortNumber = 0;
         }
      }

      /* A remote ParentDS was provided. */
      /* (This is ROW-2 in the table above). */
      else
      {
         /* The ParentDS is provided. Do a brief validation for now, more
          * detailed ones are done later.  */
         /* Check the parentPortIdentity... */
         status = zl303xx_PtpV2PortIdentityCheck(&pParentDS->parentPortIdentity);

         /* Check the grandmasterIdentity... */
         if (status == ZL303XX_OK)
         {
            status = zl303xx_PtpV2ClockIdentityCheck(pParentDS->grandmasterIdentity);
         }

         /* If the provided ParentDS is OK, forward it to the Clock. */
         if (status == ZL303XX_OK)
         {
            appliedParentDS = *pParentDS;

            /* This implementation does not support parent statistics so set the
             * values as per IEEE-1588-2008: Section 8.2.3.4. */
            appliedParentDS.parentStats = ZL303XX_FALSE;
            appliedParentDS.observedParentOffsetScaledLogVariance = 0xFFFF;
            appliedParentDS.observedParentClockPhaseChangeRate = 0x7FFFFFFF;

            /* Determine if the receivingPortIdentity is KNOWN, LOCAL or EXTERNAL. */
            if (serverSource == NULL)
            {
               /* This indicates the receivedPort is unknown and the clock should
                * attempt to assign it locally.    */
               ZL303XX_PTP_FOR_EACH_PORT(&pPtpClock->portList, pPtpPort)
               {
                  /* Search every Port on the Clock for the ParentDS provided. */
                  if (zl303xx_PtpPortFindStreamForParentDS(pPtpPort, pParentDS) != NULL)
                  {
                     if ((rxPortNumber == (Uint16T)ZL303XX_PTP_INVALID) ||
                         (pPtpPort->config.portNumber < rxPortNumber))
                     {
                        rxPortNumber = pPtpPort->config.portNumber;
                     }
                  }
               }

               /* else: */
               /* pPtpPort->config.portNumber == NOT FOUND
                * (rxPortNumber = ZL303XX_PTP_INVALID); is already set by default.    */
            }

            /* A receivingPortIdentity is provided. Determine if it was received
             * on the local clock. */
            else if (zl303xx_PtpV2EqualClockIdentities(
                                 pPtpClock->config.defaultDS.clockIdentity,
                                 serverSource->receivingPortIdentity.clockIdentity) == ZL303XX_TRUE)
            {
               if (serverSource->receivingPortIdentity.portNumber == 0)
               {
                  /* The pParentDS cannot have been received by the Clock-port. */
                  ZL303XX_TRACE_ALWAYS("%s: Invalid portNumber (%d) for external ParentDS",
                        fnName, serverSource->receivingPortIdentity.portNumber, 0,0,0,0);

                  status = ZL303XX_PARAMETER_INVALID;
               }
               else
               {
                  /* Confirm the supplied ParentDS for the indicated portNumber. */
                  if ((status = zl303xx_PtpPortDataFromPortIdentity(&serverSource->receivingPortIdentity, &pPtpPort)) != ZL303XX_OK)
                  {
                     ZL303XX_TRACE_ALWAYS("%s: portNumber (%d) not found: Status %d",
                           fnName, serverSource->receivingPortIdentity.portNumber, status, 0,0,0);
                  }
                  else
                  {
                     /* Look for the corresponding Stream on the specified Port. */
                     if (zl303xx_PtpPortFindStreamForParentDS(pPtpPort, pParentDS) == NULL)
                     {
                        /* The receivingPortIdentity indicated a local Stream
                         * should have been found.    */
                        ZL303XX_TRACE_ALWAYS("%s: ParentDS not found on Port %d",
                              fnName, serverSource->receivingPortIdentity.portNumber, 0,0,0,0);

                        status = ZL303XX_ERROR;
                     }

                     else  /* The local port matches the one provided. */
                     {
                        rxPortNumber = serverSource->receivingPortIdentity.portNumber;
                     }
                  }
               }
            }

            /* The supplied pParentDS was recovered on an external port
             * (potentially a distributed client). */
            else
            {
               /* Validate the receivingPortIdentity. */
               if ((status = zl303xx_PtpV2PortIdentityCheck(&serverSource->receivingPortIdentity)) != ZL303XX_OK)
               {
                  ZL303XX_TRACE_ALWAYS("%s: Invalid receivingPortIdentity", fnName, 0,0,0,0,0);
               }

               /* else: */
               /* The receivingPortIdentity is NOT on the local Clock.
                * (rxPortNumber = ZL303XX_PTP_INVALID); is already set by default.    */
            }
         }
      }
   }

   /* Pass the Parent Data Set info to the Clock Task to update the configuration
    * and trigger a clock state decision cycle to re-evaluate all internal states. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_SetClockParentData(clockHandle, &appliedParentDS, rxPortNumber);
   }

   return status;
}  /* END zl303xx_PtpClockParentDsSourceSet */

/* zl303xx_PtpClockForceSlaveStreamSet */
/**
   Sets the parentDS of a clock. This triggers the clock to re-evaluate each of
   its local streams to reset their states (if necessary).

  Parameters:
   [in]   clockHandle    Handle to an existing clock.
   [in]   pParentDS      The parentDS to apply to the clock. If NULL, the the
                              defaultDS of the clock is used as the parentDS.
   [in]   serverSource   Data about the Clock/Stream on which the ParentDS was received.
                              This may be a local port, an external port, or the local
                              clock Identity (portNumber=0) in the case that the
                              defaultDS is used.
                              If NULL, it indicates that the source is unknown and the
                              clock attempts to match the ParentDS with local FMT data.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or a member of pParentDS is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockForceSlaveStreamSet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpStreamHandleT streamHandle)
{
   zlStatusE status = ZL303XX_OK;

   const char *fnName = "zl303xx_PtpClockForceSlaveStreamSet";

   zl303xx_PtpClockS *pPtpClock = NULL;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   Uint16T rxPortNumber = (Uint16T)ZL303XX_PTP_INVALID;
   zl303xx_ParentDS appliedParentDS;

   /* Get a pointer to the clock structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Determine the source of the ParentDS */
   if (status == ZL303XX_OK)
   {
      /* Restore the DefaultDS of the Clock. */
      if (streamHandle == PTP_RESTORE_DEFAULT_DS)
      {
         /* Recover the ParentDS from the DefaultDS. */
         zl303xx_PtpConvertDefaultDsToParentDs(&pPtpClock->config.defaultDS, &appliedParentDS);

         /* Set the received Port ID to the clock (portNumber = 0 indicates the clock). */
         rxPortNumber = 0;
      }

      /* Attempt to recover the ParentDS from the indicated stream. */
      else
      {
         /* Get a pointer to the stream structure. */
         if ((status = zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream)) != ZL303XX_OK)
         {
            ZL303XX_TRACE_ALWAYS("%s: Invalid streamHandle (%d) - Status (%d)",
                  fnName, streamHandle, status, 0,0,0);
         }

         else if (pPtpStream->portData->clock->clockHandle != clockHandle)
         {
            ZL303XX_TRACE_ALWAYS("%s: Invalid association : stream (%d) on clock (%d), not (%d)",
                  fnName, streamHandle, pPtpStream->portData->clock->clockHandle, clockHandle, 0,0);

            status = ZL303XX_STREAM_NOT_IN_USE;
         }

         /* If everything is OK, create a ParentDS from this stream and set it on the clock. */
         else
         {
            status = zl303xx_PtpStreamParentDSGet(streamHandle, &appliedParentDS);

            /* Regardless of the Status, set the other necessary fields. */

            /* This implementation does not support parent statistics so set the
             * values as per IEEE-1588-2008: Section 8.2.3.4. */
            appliedParentDS.parentStats = ZL303XX_FALSE;
            appliedParentDS.observedParentOffsetScaledLogVariance = 0xFFFF;
            appliedParentDS.observedParentClockPhaseChangeRate = 0x7FFFFFFF;

            rxPortNumber = pPtpStream->portData->config.portNumber;
         }
      }
   }

   /* Pass the Parent Data Set info to the Clock Task to update the configuration
    * and trigger a clock state decision cycle to re-evaluate all internal states. */
   if (status == ZL303XX_OK)
   {
      if ((pPtpStream != NULL) &&
          (pPtpStream->config.mode == ZL303XX_PTP_STREAM_MASTER_ONLY))
      {
         ZL303XX_TRACE_ALWAYS(
               "%s: Stream (%u) Override Mode is MASTER_ONLY: Expect unreliable behavior.",
               fnName, streamHandle, 0,0,0,0);
      }

      status = zl303xx_SetClockParentData(clockHandle, &appliedParentDS, rxPortNumber);
   }

   return status;
}  /* END zl303xx_PtpClockForceSlaveStreamSet */

/* zl303xx_PtpClockApplyCompareData */
/**
   Forces the ParentDS data set of the clock according to a BMCA Comparison
   data structure.
   If the comparison data originated from the local clock (card) then internal
   data is used to build egress ANNOUNCE messages.
   If the comparison data originated from an EXTERNAL clock (card) then this
   comparison data is used to build egress ANNOUNCE messages.

   If the clock ParentDS is updated via any other routine ( such as
   zl303xx_PtpClockParentDsSourceSet() ), any previously 'Forced' data will
   persist although it will be ignored as long as the ParentDS Source is not
   EXTERNAL.

  Parameters:
   [in]  clockHandle    Handle to an existing clock.
   [in]  pCompareDS     An implementation specific data structure containing all
                              necessary fields to update the Clock ParentDS and
                              construct ANNOUNCE messages. (If NULL, this 'Force'
                              functionality is disabled).

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or a member of pCompareDS is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockApplyCompareData(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpBmcCompareDataS *pCompareDS)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;
   zl303xx_ParentDS equalParentDS;

   /* Get a pointer to the clock structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Determine if the functionality is being Enabled or Disabled. */
   if (status == ZL303XX_OK)
   {
      /* It is being ENABLED. */
      if (pCompareDS != NULL)
      {
         /* Update the ParentDS from this Data*/
         zl303xx_PtpConvertCompareDataToParentDs(pCompareDS, &equalParentDS);

         /* Save this Comparison Data to the External copy. It may or may not be
          * used by the clock as the ParentDS. */
         pPtpClock->gmSource.extComparisonData = *pCompareDS;

         /* Send a message to the Clock to use this ParentDS. */
         /* This sets the ParentDS Source flag which will decide on the data values
          * to package in future ANNOUNCE Messages. */
         status = zl303xx_PtpClockParentDsSourceSet(clockHandle, &equalParentDS, NULL);
      }

      /* It is being DISABLED. */
      else
      {
         /* Copy Local Clock parameters to the 'external' comparison structure. */
         (void)zl303xx_PtpClockCompareDataGet(pPtpClock, &pPtpClock->gmSource.extComparisonData);
      }
   }

   return status;
}  /* END zl303xx_PtpClockApplyCompareData */

/* zl303xx_PtpClockCompareDataGet */
/** 
 *
   Uses the data sets of the specified PTP Clock to construct an equivalent
   BMCA Compare recored.

  Parameters:
   [in]  pPtpClock      Pointer to a PTP Clock structure.
   [in]  pCompareData   An implementation specific data structure containing all
                              necessary fields to perform a BMCA comparison.

  Return Value:  ZL303XX_OK          Success.

  Notes:
   Assumes the pParentDS pointer has already been checked for this internal
   routine.
*******************************************************************************/
zlStatusE zl303xx_PtpClockCompareDataGet(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpBmcCompareDataS *pCompareData)
{
   zlStatusE status = ZL303XX_OK;


   /* Copy the clock default data set to the 'external' BMC comparison structure. */
   /* This also sets stepsRemoved to '0' for the Clock. */
   zl303xx_PtpConvertDefaultDsToCompareData(&pPtpClock->config.defaultDS, pCompareData);

   /* Set the Time Properties fields. */
   pCompareData->timeProperties = pPtpClock->config.localTimeProperties;

   return status;
}  /* END zl303xx_PtpClockCompareDataGet */

/* zl303xx_PtpClockTimePropertiesDSGet */
/**
   Returns the timePropertiesDS of a clock. This will be the far-end master's
   timePropertiesDS if the clock is locked to another, or it will be the local
   timePropertiesDS if no best master exists.

  Parameters:
   [in]   clockHandle        Handle to an existing clock.
   [out]  pTimePropertiesDS  The timePropertiesDS of the clock.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pTimePropertiesDS is NULL.
     ZL303XX_PARAMETER_INVALID  clockHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockTimePropertiesDSGet(zl303xx_PtpClockHandleT clockHandle,
                                              zl303xx_TimePropertiesDS *pTimePropertiesDS)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pClock = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pTimePropertiesDS);
   }

   /* Get a pointer to the clock structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pClock);
   }

   /* Return the clock's ParentDS. */
   if (status == ZL303XX_OK)
   {
      *pTimePropertiesDS = pClock->timePropertiesDS;
   }

   return status;
}

/********************** Clock level operations ******************************/

/* zl303xx_PtpClockDefaultDSDefaultParams */
/**
   Initializes a zl303xx_DefaultDS structure with default values.

  Parameters:
   [out]  pDefaultDS  Default parameters for creating a PTP clock.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pDefaultDS is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpClockDefaultDSDefaultParams(zl303xx_DefaultDS *pDefaultDS)
{
   /* Check the structure pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pDefaultDS);

   if (status == ZL303XX_OK)
   {
      OS_MEMSET(pDefaultDS->clockIdentity, 0, PTP_V2_CLK_IDENTITY_LENGTH);

      pDefaultDS->twoStepFlag  = ZL303XX_PTP_DEFAULT_CLOCK_TWO_STEP;
      pDefaultDS->numberPorts  = ZL303XX_PTP_DEFAULT_CLOCK_NUM_PORTS;
      pDefaultDS->priority1    = ZL303XX_PTP_DEFAULT_CLOCK_PRI_1;
      pDefaultDS->priority2    = ZL303XX_PTP_DEFAULT_CLOCK_PRI_2;
      pDefaultDS->domainNumber = ZL303XX_PTP_DEFAULT_CLOCK_DOMAIN_NUM;
      pDefaultDS->slaveOnly    = ZL303XX_PTP_DEFAULT_CLOCK_SLAVE_ONLY;

      /* Use the available Default Parameters routine. */
      (void)zl303xx_PtpClockQualityDefaultParams(&pDefaultDS->clockQuality);
   }

   return status;
}  /* END zl303xx_PtpClockDefaultDSDefaultParams */

/* zl303xx_PtpClockDefaultDSGet */
/**
   Returns the Active Default Data Set configuration of a clock. If the clock is
   the GM then this is the configured DefaultDS. If a Virtual Port is selected
   then the VP configuration is returned as the DefaultDS.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [out]  pDefaultDS   The clock's active Default Data Set.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pDefaultDS is NULL.
     ZL303XX_PARAMETER_INVALID  clockHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockDefaultDSGet(zl303xx_PtpClockHandleT clockHandle,
                                       zl303xx_DefaultDS *pDefaultDS)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check the input pointer. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pDefaultDS);
   }

   /* Get the associated clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* If all is well, check if a Virtual Port is selected or there is a
    * fallback Class associated with a profile. */
   if (status == ZL303XX_OK)
   {
      /* Pointer to a Virtual Port (that may not be selected). */
      zl303xx_PortDataS *pPtpVirtualPort = NULL;

      /* Set the configured values as the ones to return. */
      *pDefaultDS = pPtpClock->config.defaultDS;

      /* If a Virtual Port is Selected, over write any of the clock defaults. */
      if (zl303xx_PtpClockHasSelectedVirtualPort(pPtpClock, &pPtpVirtualPort) == ZL303XX_TRUE)
      {
         pDefaultDS->clockQuality = pPtpVirtualPort->virtualData.config.clockQuality;
         pDefaultDS->priority1 = pPtpVirtualPort->virtualData.config.priority1;
         pDefaultDS->priority2 = pPtpVirtualPort->virtualData.config.priority2;
      }
      else if ((pPtpClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_1v1) ||
               (pPtpClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_1v2) ||
               (pPtpClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_2))
      {
         if ((pPtpClock->config.clockType == ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_BC) &&
             (pPtpClock->config.profileCfg.g8275p1.bypassEnabled == ZL303XX_FALSE))
         {
            pDefaultDS->clockQuality.clockClass = pPtpClock->profileDS.g8275.fallbackClass;
         }
      }
   }

   return status;
}  /* END zl303xx_PtpClockDefaultDSGet */

/* zl303xx_PtpClockDefaultDSGetLocal */
/**
   Returns the Configured Default Data Set of a Clock (the exact values
   configured with the zl303xx_PtpClockDefaultDSSet command)..

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [out]  pDefaultDS   The clock's configured Default Data Set.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pDefaultDS is NULL.
     ZL303XX_PARAMETER_INVALID  clockHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockDefaultDSGetLocal(zl303xx_PtpClockHandleT clockHandle,
                                            zl303xx_DefaultDS *pDefaultDS)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check the input pointer. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pDefaultDS);
   }

   /* Get the associated clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* If all is well, copy the clock's Default Data Set. */
   if (status == ZL303XX_OK)
   {
      /* No need for a MUTEX for this read. */
      *pDefaultDS = pPtpClock->config.defaultDS;
   }

   return status;
}  /* END zl303xx_PtpClockDefaultDSGetLocal */

/* zl303xx_PtpClockDefaultDSSet */
/**
   Sets a clock's Default Data Set parameters. These parameters are used by the
   PTP engine to determine whether this clock is a server or client in the
   network.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   pDefaultDS   The Default Data Set parameters to use on this clock.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       pDefaultDS is NULL.
     ZL303XX_PARAMETER_INVALID     clockHandle or a parameter in pDefaultDS is
                                       invalid. Or, there is a slaveOnly &
                                       clockClass conflict.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockDefaultDSSet(zl303xx_PtpClockHandleT clockHandle,
                                       zl303xx_DefaultDS *pDefaultDS)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check the input pointer. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pDefaultDS);
   }

   /* Get the associated clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Pass the data to the core Default Data Set update routine. It will:
    * - verify all members of the pDefaultDS structure.
    * - update the clock's configured parameters.
    * - update the clock's active values.
    * - trigger a clock state decision cycle to re-evaluate the clock state. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockConfigureDefaultDataSet(pPtpClock, pDefaultDS, ZL303XX_TRUE);
   }

   return status;
}  /* END zl303xx_PtpClockDefaultDSSet */

/* zl303xx_PtpClockQualityDefaultParams */
/**
   Initializes a zl303xx_ClockQuality structure with default clock quality values.
   All defaults are defined in zl303xx_PtpSetup.h

  Parameters:
   [out]  pClockQuality  Default clock quality values.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pClockQuality is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpClockQualityDefaultParams(zl303xx_ClockQuality *pClockQuality)
{
   /* Check the structure pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pClockQuality);

   if (status == ZL303XX_OK)
   {
      pClockQuality->clockClass = ZL303XX_PTP_DEFAULT_CLOCK_CLASS;
      pClockQuality->clockAccuracy = ZL303XX_PTP_DEFAULT_CLOCK_ACCURACY;
      pClockQuality->offsetScaledLogVariance = ZL303XX_PTP_DEFAULT_CLOCK_SCALED_LOG_VAR;
   }

   return status;
}  /* END zl303xx_PtpClockQualityDefaultParams */

/* zl303xx_PtpClockQualitySet */
/**
   Sets the clock quality properties for the clock. These are the parameters
   that are used by the PTP engine to determine whether this clock is a master
   or slave in the network. Note that setting clockClass to 255 will force
   the clock's defaultDS::slaveOnly to ZL303XX_TRUE, and setting it to < 255 will
   force it to ZL303XX_FALSE. This behaviour is different than
   zl303xx_PtpClockDefaultDSSet(), which gives the slaveOnly parameter priority
   over clockClass.

  Parameters:
   [in]   clockHandle    Handle to an existing clock.
   [in]   pClockQuality  Clock quality parameters.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       pClockQuality is NULL.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockQualitySet(zl303xx_PtpClockHandleT clockHandle,
                                     zl303xx_ClockQuality *pClockQuality)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_DefaultDS tempDefaultDS;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check the input pointer. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pClockQuality);
   }

   /* Get the associated clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Use the entire current data configuration to test the new member
    * configuration. */
   if (status == ZL303XX_OK)
   {
      /* Copy the current configuration to a temporary variable. */
      tempDefaultDS = pPtpClock->config.defaultDS;

      /* Update the targeted member(s). */
      tempDefaultDS.clockQuality = *pClockQuality;

      if ((tempDefaultDS.clockQuality.clockClass != PTP_SLAVE_ONLY_CLOCK_CLASS) &&
          (tempDefaultDS.slaveOnly == ZL303XX_TRUE))
      {
         ZL303XX_TRACE_ALWAYS("   Forcing slaveOnly=FALSE for clockClass=%u.",
               tempDefaultDS.clockQuality.clockClass, 0,0,0,0,0);
         tempDefaultDS.slaveOnly = ZL303XX_FALSE;
      }

      /* Pass the data to the core Default Data Set update routine. It will:
       * - verify all members of the tempDefaultDS structure.
       * - update the clock's configured parameters.
       * - update the clock's active values.
       * - trigger a clock state decision cycle to re-evaluate the clock state. */
      status = zl303xx_PtpClockConfigureDefaultDataSet(pPtpClock, &tempDefaultDS, ZL303XX_TRUE);
   }

   return status;
}  /* END zl303xx_PtpClockQualitySet */

/* zl303xx_PtpClockClassSet */
/**
   Updates a clock's defaultDS::clockQuality::clockClass parameter. Note that
   setting clockClass to 255 will force defaultDS::slaveOnly to ZL303XX_TRUE, and
   setting it to < 255 will force defaultDS::slaveOnly to ZL303XX_FALSE. This
   behaviour is different than zl303xx_PtpClockDefaultDSSet(), which gives the
   slaveOnly parameter priority over clockClass.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   clockClass   New clockClass to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or clockClass is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockClassSet(zl303xx_PtpClockHandleT clockHandle,
                                   Uint8T clockClass)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_DefaultDS defaultDS;

   /* Get the clock's defaultDS. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDefaultDSGet(clockHandle, &defaultDS);
   }

   /* Update the clockClass and set the clockQuality. */
   if (status == ZL303XX_OK)
   {
      defaultDS.clockQuality.clockClass = clockClass;
      status = zl303xx_PtpClockQualitySet(clockHandle, &defaultDS.clockQuality);
   }

   return status;
}

/* zl303xx_PtpClockSlaveOnlySet */
/**
   Updates a clock's defaultDS::slaveOnly parameter. Note that setting slaveOnly
   to ZL303XX_TRUE will force the clock's class to be set to 255.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   slaveOnly    The slaveOnly value to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or slaveOnly is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockSlaveOnlySet(zl303xx_PtpClockHandleT clockHandle,
                                       zl303xx_BooleanE slaveOnly)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_DefaultDS tempDefaultDS;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check the input setting. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(slaveOnly);
   }

   /* Get the associated clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Use the entire current data configuration to test the new member
    * configuration. */
   if (status == ZL303XX_OK)
   {
      /* Copy the current configuration to a temporary variable. */
      tempDefaultDS = pPtpClock->config.defaultDS;

      /* Update the targeted member(s). */
      tempDefaultDS.slaveOnly = slaveOnly;

      /* Pass the data to the core Default Data Set update routine. It will:
       * - verify all members of the tempDefaultDS structure.
       * - update the clock's configured parameters.
       * - update the clock's active values.
       * - trigger a clock state decision cycle to re-evaluate the clock state. */
      status = zl303xx_PtpClockConfigureDefaultDataSet(pPtpClock, &tempDefaultDS, ZL303XX_TRUE);
   }

   return status;
}  /* END zl303xx_PtpClockSlaveOnlySet */

/* zl303xx_PtpClockTwoStepFlagSet */
/**
   Sets a clock's defaultDS::twoStepFlag parameter. This determines if a clock
   will transmit Follow_Up messages. Note that twoStepFlag is considered to be a
   static member of the defaultDS (i.e., inherent physical or operational
   property of the clock). Therefore, this operation is non-standard, and the
   expected behaviour is not covered by IEEE 1588-2008.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   twoStepFlag  The twoStepFlag value to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or twoStepFlag is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockTwoStepFlagSet(zl303xx_PtpClockHandleT clockHandle,
                                         zl303xx_BooleanE twoStepFlag)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_DefaultDS tempDefaultDS;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check the input setting. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(twoStepFlag);
   }

   /* Get the associated clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Use the entire current data configuration to test the new member
    * configuration. */
   if (status == ZL303XX_OK)
   {
      /* Copy the current configuration to a temporary variable. */
      tempDefaultDS = pPtpClock->config.defaultDS;

      /* Update the targeted member(s). */
      tempDefaultDS.twoStepFlag = twoStepFlag;

      /* Pass the data to the core Default Data Set update routine. It will:
       * - verify all members of the tempDefaultDS structure.
       * - update the clock's configured parameters.
       * - update the clock's active values.
       * - trigger a clock state decision cycle to re-evaluate the clock state. */
      status = zl303xx_PtpClockConfigureDefaultDataSet(pPtpClock, &tempDefaultDS, ZL303XX_TRUE);
   }

   return status;
}

/* zl303xx_PtpClockDomainNumberSet */
/**
   Updates a clock's defaultDS::domainNumber parameter. Calling this function
   will invalidate all entries in every port's foreign master table if the new
   domain number is different than the current.

  Parameters:
   [in]   clockHandle   Handle to an existing clock.
   [in]   domainNumber  The domainNumber value to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or domainNumber is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockDomainNumberSet(zl303xx_PtpClockHandleT clockHandle,
                                          Uint8T domainNumber)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_DefaultDS tempDefaultDS;
   zl303xx_PtpClockS *pClock = NULL;

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pClock);
   }

   if (status == ZL303XX_OK)
   {
      /* Copy the current configuration to a temporary variable. */
      tempDefaultDS = pClock->config.defaultDS;

      /* Update the targeted member(s). */
      tempDefaultDS.domainNumber = domainNumber;

      /* Pass the data to the core Default Data Set update routine. It will:
       * - verify all members of the tempDefaultDS structure.
       * - update the clock's configured parameters.
       * - update the clock's active values.
       * - trigger a clock state decision cycle to re-evaluate the clock state. */
      status = zl303xx_PtpClockConfigureDefaultDataSet(pClock, &tempDefaultDS, ZL303XX_TRUE);
   }

   return status;
}

/* zl303xx_PtpClockTypeSet */
/**
   Updates a clock's clockType parameter. This will:
      - This will change the current clockType value (what is sent in any TLVs).
      - Affect how new Ports / Streams are created.
      - NOT affect existing Port / Stream configurations.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   clockType    The clockType value to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockTypeSet(zl303xx_PtpClockHandleT clockHandle,
                                  Uint16T clockType)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Get the associated clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   /* Set the new value. */
   if (status == ZL303XX_OK)
   {
      /* Update the targeted member. */
      pPtpClock->clockType = clockType;

      /* Force an evaluation of the clockType. */
      zl303xx_PtpClockTypeEvaluate(pPtpClock);

      /* Release the clock mutex. */
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* END zl303xx_PtpClockTypeSet */


/* zl303xx_PtpClockBmcaFlagClear */
/**
   Clears a given individual bit of the BMCA flags variable

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   flagBit    The Bit to clear.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle/flagBit is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockBmcaFlagClear(zl303xx_PtpClockHandleT clockHandle,
                                          zl303xx_BMCAFlagsE flagBit)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;
   
   /* check that the flagBit is valid */
   if( !(flagBit == STATE_FM_QUALIFICATION_FLAG || flagBit < ZL303XX_PTP_VALID_NUM_BMCA_FLAGS) )
   {
	   ZL303XX_TRACE_ERROR("   (ERROR) Invalid BMCA flags bit field=%u.",
                          flagBit, 0,0,0,0,0);
	   status = ZL303XX_PARAMETER_INVALID;
   }
   
   /* Get the associated clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   /* Set the new value. */
   if (status == ZL303XX_OK)
   {
      /* clear the target bit field of the bmcaFlags variable. */
      ZL303XX_BIT_CLEAR(pPtpClock->bmcaFlags,flagBit);


      /* Release the clock mutex. */
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* END zl303xx_PtpClockBmcaFlagClear */


/* zl303xx_PtpClockBmcaFlagStatusGet */
/**
   Get the status of a given individual bit of the BMCA flags variable

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   flagBit      Individual bit of BMCA flags variable.
   [out]  pFlagStatus  status of the individual bit

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle/flagBit is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockBmcaFlagStatusGet(zl303xx_PtpClockHandleT clockHandle,
                              zl303xx_BMCAFlagsE flagBit,zl303xx_BooleanE *pFlagStatus)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;
   
   /* check the zl303xx_BooleanE pointer */
   if(status == ZL303XX_OK)
   {
	   status = ZL303XX_CHECK_POINTER(pFlagStatus);
   }
   
   /* check that the flagBit is valid */
   if(status == ZL303XX_OK && !(flagBit == STATE_FM_QUALIFICATION_FLAG || flagBit < ZL303XX_PTP_VALID_NUM_BMCA_FLAGS) )
   {
	   ZL303XX_TRACE_ERROR("   (ERROR) Invalid BMCA flags bit field=%u.",
                          flagBit, 0,0,0,0,0);
	   status = ZL303XX_PARAMETER_INVALID;
   }
   
   /* Get the associated clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Get value of flagBit. */
   if (status == ZL303XX_OK)
   {
      if (ZL303XX_BIT_TEST(pPtpClock->bmcaFlags, flagBit))
	  {
		  *pFlagStatus = ZL303XX_TRUE;
	  }
	  else
	  {
		  *pFlagStatus = ZL303XX_FALSE;
	  }
   }

   return status;
}  /* END zl303xx_PtpClockBmcaFlagStatusGet */


/* zl303xx_PtpClockRevertiveEnSet */
/**
   Sets a clock's revertive BMCA parameters. These parameters are used by the
   PTP engine to determine whether this clock will actively switch to the
   master with the best clockClass.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   revertiveEn  Set to ZL303XX_TRUE to enable revertive switching.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or revertiveEn is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockRevertiveEnSet(zl303xx_PtpClockHandleT clockHandle,
                                       zl303xx_BooleanE revertiveEn)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check the input setting. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(revertiveEn);
   }

   /* Get the associated clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   /* Update the values in the Clock configuration. */
   if (status == ZL303XX_OK)
   {
      pPtpClock->config.bmca.revertiveEn = revertiveEn;

      /* If revertive mode is enabled, invalidate the selected master. */
      if (pPtpClock->config.bmca.revertiveEn == ZL303XX_TRUE)
      {
         pPtpClock->bmca.selectedFmtEntry = ZL303XX_PTP_FMT_INDEX_INVALID;
         OS_MEMSET(&pPtpClock->bmca.selectedAddr, 0, sizeof(pPtpClock->bmca.selectedAddr));
      }

      /* If revertive mode is being disabled, the selected master will already be
       * invalid, and the current best master will become the selected one. */

      /* Release the clock mutex. */
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* END zl303xx_PtpClockRevertiveEnSet */

/* zl303xx_PtpClockPrioritySet */
/**
   Sets a clock's priority1 & priority2 parameters. These parameters are used by
   the PTP engine to determine whether this clock is a server or client in the
   network.

  Parameters:
   [in]  clockHandle    Handle to an existing clock.
   [in]  priority1      Pointer to the priority1 value to set. (If NULL, then
                              this parameter is not updated).
   [in]  priority2      Pointer to the priority2 value to set. (If NULL, then
                              this parameter is not updated).

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       Both priority parameters are NULL.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockPrioritySet(zl303xx_PtpClockHandleT clockHandle,
                                      Uint8T *priority1, Uint8T *priority2)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_DefaultDS tempDefaultDS;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Assume the routine was called for a reason, so both inputs should
    * not be NULL. */
   if ((priority1 == NULL) && (priority2 == NULL))
   {
      status = ZL303XX_INVALID_POINTER;
   }

   /* Get the associated clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Use the entire current data configuration to test the new member
    * configuration. */
   if (status == ZL303XX_OK)
   {
      /* Copy the current configuration to a temporary variable. */
      tempDefaultDS = pPtpClock->config.defaultDS;

      /* Update the targeted member(s). */
      if (priority1 != NULL)
      {
         tempDefaultDS.priority1 = *priority1;
      }
      if (priority2 != NULL)
      {
         tempDefaultDS.priority2 = *priority2;
      }

      /* Pass the data to the core Default Data Set update routine. It will:
       * - verify all members of the tempDefaultDS structure.
       * - update the clock's configured parameters.
       * - update the clock's active values.
       * - trigger a clock state decision cycle to re-evaluate the clock state. */
      status = zl303xx_PtpClockConfigureDefaultDataSet(pPtpClock, &tempDefaultDS, ZL303XX_TRUE);
   }

   return status;
}  /* END zl303xx_PtpClockPrioritySet */

/* zl303xx_PtpTimePropertiesDefaultParams */
/**
   Initializes a pTimeProperties structure with default time properties values.

  Parameters:
   [out]  pTimeProperties  Default parameters for creating a PTP clock.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pTimeProperties is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpTimePropertiesDefaultParams(zl303xx_TimePropertiesDS *pTimeProperties)
{
   /* Check the structure pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pTimeProperties);

   if (status == ZL303XX_OK)
   {
      pTimeProperties->currentUtcOffset         = UTC_OFFSET;
      pTimeProperties->currentUtcOffsetValid    = ZL303XX_FALSE;
      pTimeProperties->leap59                   = ZL303XX_FALSE;
      pTimeProperties->leap61                   = ZL303XX_FALSE;
      pTimeProperties->timeTraceable            = ZL303XX_FALSE;
      pTimeProperties->frequencyTraceable       = ZL303XX_FALSE;
      pTimeProperties->ptpTimescale             = ZL303XX_TRUE;
      pTimeProperties->timeSource               = ZL303XX_TIME_SOURCE_INTERNAL_OSCILLATOR;
      pTimeProperties->synchronizationUncertain = ZL303XX_FALSE;
   }

   return status;
}  /* END zl303xx_PtpTimePropertiesDefaultParams */

/* zl303xx_PtpTimePropertiesSetLocal */
/**
   Defines the local (hardware) time properties for the clock. These are the
   parameters that characterize this clock when it is the grandmaster,
   i.e. when it is not slaved to another clock.

  Parameters:
   [in]  clockHandle       Handle to an existing clock.
   [in]  pTimeProperties   Pointer to a zl303xx_TimePropertiesDS structure.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       pTimeProperties is NULL.
     ZL303XX_PARAMETER_INVALID     clockHandle is not valid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOX_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpTimePropertiesSetLocal(zl303xx_PtpClockHandleT clockHandle,
                                       zl303xx_TimePropertiesDS *pTimeProperties)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pTimeProperties);

      if (status == ZL303XX_OK)
      {
         /* An error is returned if the clock is NULL. */
         status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
      }
   }

   /* If all is well, update the local time properties configuration. */
   if (status == ZL303XX_OK)
   {
      /* Lock the clock mutex. */
      status = zl303xx_PtpClockMutexLock(pPtpClock);

      /* We have the MUTEX so update the data. */
      if (status == ZL303XX_OK)
      {
         /* Update the local time properties data set. */
         pPtpClock->config.localTimeProperties = *pTimeProperties;

         /* Release the clock mutex. */
         status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
      }
   }

   /* Any time the Clock configuration is changed, the Clock State-Decision
    * algorithm should be run to re-evaluate its state (and that of any
    * associated ports & streams).   */
   /* In this case, the local localTimeProperties change may need to be
    * propagated to clock timePropertiesDS depending on the current state of the
    * clock (i.e if the local clock is M1/M2, then Announce messages need to
    * advertise the new settings).  */
   if (status == ZL303XX_OK)
   {
      /* Resend the Foreign Master Table on a config change. */
      zl303xx_PtpClockBmcaUpdateInitiate(pPtpClock);
   }

   return status;
}  /* END zl303xx_PtpTimePropertiesSetLocal */

/* zl303xx_PtpTimePropertiesGetLocal */
/**
   Returns the local (hardware) time properties for the clock. These
   parameters relate only to the local oscillator and are used by the
   PTP engine only if the clock is operating as a grandmaster.

  Parameters:
   [in]   clockHandle       Handle of the clock for which to get the data.
   [out]  pTimeProperties   The local time properties configuration.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pTimeProperties is NULL.
     ZL303XX_PARAMETER_INVALID  clockHandle is not valid.

*******************************************************************************/
zlStatusE zl303xx_PtpTimePropertiesGetLocal(zl303xx_PtpClockHandleT clockHandle,
                                       zl303xx_TimePropertiesDS *pTimeProperties)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pTimeProperties);

      if (status == ZL303XX_OK)
      {
         /* An error is returned if the clock is NULL. */
         status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
      }
   }

   /* If all is well, copy the local time properties configuration. */
   if (status == ZL303XX_OK)
   {
      /* No need for a MUTEX this read. */

      /* Return the local time properties configuration. */
      *pTimeProperties = pPtpClock->config.localTimeProperties;
   }

   return status;
}  /* END zl303xx_PtpTimePropertiesGetLocal */

/* zl303xx_PtpCurrentUtcOffsetSetLocal */
/**
   Sets the Current UTC property of the local time properties data set.

  Parameters:
   [in]  clockHandle       Handle to an existing clock.
   [in]  localUtcOffset    The current UTC Offset of the local clock.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle is not valid.

*******************************************************************************/
zlStatusE zl303xx_PtpCurrentUtcOffsetSetLocal(
      zl303xx_PtpClockHandleT clockHandle,
      Sint16T localUtcOffset)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   if (status == ZL303XX_OK)
   {
      /* An error is returned if the clock is NULL. */
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* If all is well, update the currentUtcOffset member of the local time
    * properties configuration. */
   if (status == ZL303XX_OK)
   {
      pPtpClock->config.localTimeProperties.currentUtcOffset = localUtcOffset;
   }

   return status;
}  /* END zl303xx_PtpCurrentUtcOffsetSetLocal */

/* zl303xx_PtpClockTaiGet */
/**
   Returns a clock's offset from the PTP epoch (1 January 1970 00:00:00 TAI).
   This function requires that the clock has a valid handler for
   ZL303XX_PTP_HW_CMD_CLOCK_TIME_GET.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [out]  pTai         Current TAI of the clock.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pTai is NULL.
     ZL303XX_PARAMETER_INVALID  clockHandle is invalid.
     ZL303XX_INIT_ERROR         No hwCmdFn() defined for this clock.
     ZL303XX_EXT_API_CALL_FAIL  The clock's hwCmdFn failed somewhere.

*******************************************************************************/
zlStatusE zl303xx_PtpClockTaiGet(zl303xx_PtpClockHandleT clockHandle,
                                 zl303xx_TimeStamp *pTai)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pClock;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pTai);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pClock);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockTimeGet(pClock, ZL303XX_PTP_INTERFACE_GENERAL, pTai);
   }

   return status;
}

/* zl303xx_PtpClockUtcGet */
/**
   Returns the current UTC (Coordinated Universal Time) of a clock. This
   function requires that the clock has a valid handler for
   ZL303XX_PTP_HW_CMD_CLOCK_TIME_GET.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [out]  pUtc         Current UTC of the clock.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pUtc is NULL.
     ZL303XX_PARAMETER_INVALID  clockHandle is invalid.
     ZL303XX_INIT_ERROR         No hwCmdFn() defined for this clock.
     ZL303XX_EXT_API_CALL_FAIL  The clock's hwCmdFn failed somewhere.

*******************************************************************************/
zlStatusE zl303xx_PtpClockUtcGet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_TimeStamp *pUtc)
{
   zl303xx_BooleanE utcValid;
   return zl303xx_PtpClockUtcGetWithValid(clockHandle, pUtc, &utcValid);
}

/* zl303xx_PtpClockUtcGetWithValid */
/**
   Returns the current UTC (Coordinated Universal Time) of a clock along with
   a flag indicating if the UTC is valid. This function requires that the clock
   has a valid handler for ZL303XX_PTP_HW_CMD_CLOCK_TIME_GET.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [out]  pUtc         Current UTC of the clock.
   [out]  utcValid     Current UTC VALID Flag.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pUtc or utcValid is NULL.
     ZL303XX_PARAMETER_INVALID  clockHandle is invalid.
     ZL303XX_INIT_ERROR         No hwCmdFn() defined for this clock.
     ZL303XX_EXT_API_CALL_FAIL  The clock's hwCmdFn failed somewhere.

*******************************************************************************/
zlStatusE zl303xx_PtpClockUtcGetWithValid(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_TimeStamp *pUtc,
      zl303xx_BooleanE *utcValid)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pClock;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pUtc) |
               ZL303XX_CHECK_POINTER(utcValid);
   }

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pClock);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockUtcGetInternal(pClock, pUtc, utcValid);
   }

   return status;
}

/* zl303xx_PtpClockIngressClockClassSet */
/**
   Sets the ingress override clock class value for the specified clock. If the
   value is non-zero, it will replace the clockClass field in a received
   Announce message provided the stream override is disabled.

  Parameters:
   [in]   clockHandle   Handle to an existing clock.
   [in]   ingressClass  Ingress clock class over-ride value. A value of 0
                             disables the over-ride.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  clockHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockIngressClockClassSet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint8T ingressClass)
{
   return zl303xx_PtpCtrlClockParamSet(clockHandle,
                                       CLOCK_PARAM_INGRESS_CLOCK_CLASS,
                                       &ingressClass);
}

/* zl303xx_PtpClockEgressClockClassSet */
/**
   Sets the egress override clock class value for a clock. If the value is
   non-zero, it will be used as the transmitted clockClass value provided the
   stream override is disabled. Otherwise, the clock or parentDS value is used.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   egressClass  Egress clock class over-ride value. A value of 0
                            disables the over-ride.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  clockHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockEgressClockClassSet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint8T egressClass)
{
   return zl303xx_PtpCtrlClockParamSet(clockHandle,
                                       CLOCK_PARAM_EGRESS_CLOCK_CLASS,
                                       &egressClass);
}

/* zl303xx_PtpClockStepsRemovedMaxSet */
/**
   Sets the stepsRemoved limit (MAX) for a clock. If the stepsRemoved field of
   a received ANNOUNCE Message is below the limit, it will be accepted.
   Otherwise, it is discarded.

  Parameters:
   [in]  clockHandle       Handle to an existing clock.
   [in]  maxStepsRemoved   stepsRemoved limit of the clock.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  clockHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockStepsRemovedMaxSet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint16T maxStepsRemoved)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock;

   /* Get the clock data */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* There is no need to TAKE a MUTEX here. Any routine that accesses the data
    * before the change will get the old value (as it should). Any routine that
    * accesses the data after the change will get the new value (as it should).  */
   if (status == ZL303XX_OK)
   {
      pPtpClock->config.optionalDefaultDS.maxStepsRemoved = maxStepsRemoved;
   }

   return status;
}

/* zl303xx_PtpClockEgressQlUpdateSet */
/**
   Sets when the egress clock class value for the specified clock is updated.
   Possible triggers are:
      - ZL303XX_EGRESS_QL_UPDATE_ON_QUALIFY:
            As soon as a qualified stream is selected, begin to use its
            QL (clockClass) in all transmitted packets.
      - ZL303XX_EGRESS_QL_UPDATE_ON_LOCK:
            Keep using the local Default QL until the clock achieves LOCK
            with a selected server.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   qlUpdate     Egress QL update trigger.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  clockHandle or qlUpdate is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockEgressQlUpdateSet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpClockEgressQlUpdateE qlUpdate)
{
   zlStatusE status = ZL303XX_OK;

   /* Verify a valid input parameter. */
   if (status == ZL303XX_OK)
   {
      if ((qlUpdate != ZL303XX_EGRESS_QL_UPDATE_ON_QUALIFY) &&
          (qlUpdate != ZL303XX_EGRESS_QL_UPDATE_ON_LOCK))
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Invalid egress QL update trigger=%d.",
               qlUpdate, 0,0,0,0,0);

         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpCtrlClockParamSet(clockHandle,
                                            CLOCK_PARAM_EGRESS_QL_UPDATE,
                                            &qlUpdate);
   }

   return status;
}

/* zl303xx_PtpClockEgressOverrideSet */
/**
   Sets over-ride values for certain egress clock parameters.
   When messages are sent from this clock, the software will check if an
   override value is configured and force that value into the message.

   This routine DOES NOT verify override parameters. It is expected that if a
   override value is configured, the user is aware of its implications.

  Parameters:
   [in]  clockHandle    Handle to an existing clock.
   [in]  overrideId     ID of the parameter to over-ride.
   [in]  overrideValue  Value of the override parameter (passed as a VOID
                              pointer to accommodate various types). If set to
                              NULL, any current override value is cleared.

 update trigger.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  clockHandle or overrideId is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockEgressOverrideSet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpClockOverrideEgressE overrideId,
      void *overrideValue)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check the input override ID. */
   if (status == ZL303XX_OK)
   {
      if (overrideId >= ZL303XX_PTP_CLOCK_OVERRIDE_NUM_ENTRIES)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpClockEgressOverrideSet: Invalid ID (%u)",
               overrideId, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Get the associated clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   if (status == ZL303XX_OK)
   {
      /* Clear any NULL values. */
      if (overrideValue == NULL)
      {
         pPtpClock->config.override.enabled[overrideId] = ZL303XX_FALSE;
      }

      /* Set any provided values. */
      else
      {
         /* Indicate an Override exists. */
         pPtpClock->config.override.enabled[overrideId] = ZL303XX_TRUE;

         switch (overrideId)
         {
            /** DefaultDS override values. */
            case ZL303XX_PTP_CLOCK_OVERRIDE_CLASS :
            {
               pPtpClock->config.override.defaultDS.clockQuality.clockClass = *(Uint8T*)overrideValue;
               break;
            }
            case ZL303XX_PTP_CLOCK_OVERRIDE_ACCURACY :
            {
               pPtpClock->config.override.defaultDS.clockQuality.clockAccuracy = *(zl303xx_ClockAccuracyE*)overrideValue;
               break;
            }
            case ZL303XX_PTP_CLOCK_OVERRIDE_OFFSET_SCALED_LOG_VARIANCE :
            {
               pPtpClock->config.override.defaultDS.clockQuality.offsetScaledLogVariance = *(Uint16T*)overrideValue;
               break;
            }
            case ZL303XX_PTP_CLOCK_OVERRIDE_PRIORITY1 :
            {
               pPtpClock->config.override.defaultDS.priority1 = *(Uint8T*)overrideValue;
               break;
            }
            case ZL303XX_PTP_CLOCK_OVERRIDE_PRIORITY2 :
            {
               pPtpClock->config.override.defaultDS.priority2 = *(Uint8T*)overrideValue;
               break;
            }
            case ZL303XX_PTP_CLOCK_OVERRIDE_DOMAIN :
            {
               pPtpClock->config.override.defaultDS.domainNumber = *(Uint8T*)overrideValue;
               break;
            }

            /** TimePropertiesDS override values. */
            case ZL303XX_PTP_CLOCK_OVERRIDE_CURRENT_UTC_OFFSET :
            {
               pPtpClock->config.override.timePropertiesDS.currentUtcOffset = *(Sint16T*)overrideValue;
               break;
            }
            case ZL303XX_PTP_CLOCK_OVERRIDE_CURRENT_UTC_OFFSET_VALID :
            {
               pPtpClock->config.override.timePropertiesDS.currentUtcOffsetValid = *(zl303xx_BooleanE*)overrideValue;
               break;
            }
            case ZL303XX_PTP_CLOCK_OVERRIDE_TIME_TRACEABLE :
            {
               pPtpClock->config.override.timePropertiesDS.timeTraceable = *(zl303xx_BooleanE*)overrideValue;
               break;
            }
            case ZL303XX_PTP_CLOCK_OVERRIDE_FREQUENCY_TRACEABLE :
            {
               pPtpClock->config.override.timePropertiesDS.frequencyTraceable = *(zl303xx_BooleanE*)overrideValue;
               break;
            }

            /* We allow this optional parameter to override even if the
             * functionality is disabled. */
            case ZL303XX_PTP_CLOCK_OVERRIDE_SYNCHRO_UNCERTAIN :
            {
               pPtpClock->config.override.timePropertiesDS.synchronizationUncertain = *(zl303xx_BooleanE*)overrideValue;
               break;
            }

            case ZL303XX_PTP_CLOCK_OVERRIDE_TIME_SOURCE :
            {
               pPtpClock->config.override.timePropertiesDS.timeSource = *(zl303xx_TimeSourceE*)overrideValue;
               break;
            }

            /** CurrentDS override values. */
            case ZL303XX_PTP_CLOCK_OVERRIDE_STEPS_REMOVED :
            {
               pPtpClock->config.override.currentDS.stepsRemoved = *(Uint16T*)overrideValue;
               break;
            }

            case ZL303XX_PTP_CLOCK_OVERRIDE_NUM_ENTRIES :
            default :
            {
               /* This can never happen since a check is done at the start of
                * this routine. */
               break;
            }
         } /* SWITCH */
      }
   }

   return status;
}

/* zl303xx_PtpClockEgressOverrideShow */
/**
   Displays the over-ride configuration and values for the egress clock
   over-ride parameters.

  Parameters:
   [in]  clockHandle    Handle to an existing clock.

 update trigger.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  clockHandle or overrideId is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockEgressOverrideShow(
      zl303xx_PtpClockHandleT clockHandle)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Get the associated clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   if (status == ZL303XX_OK)
   {
      printf("\nEgress Override Param:   Override   Value\n");
      printf("------------------------------------------\n");
      printf("          Clock Class:      %s        %d\n",
            (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_CLASS]) ? "T" : "F",
             pPtpClock->config.override.defaultDS.clockQuality.clockClass);
      printf("       Clock Accuracy:      %s        0x%02X\n",
            (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_ACCURACY]) ? "T" : "F",
             pPtpClock->config.override.defaultDS.clockQuality.clockAccuracy);
      printf("      Offset Variance:      %s        0x%04X\n",
            (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_OFFSET_SCALED_LOG_VARIANCE]) ? "T" : "F",
             pPtpClock->config.override.defaultDS.clockQuality.offsetScaledLogVariance);
      printf("            Priority1:      %s        %d\n",
            (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_PRIORITY1]) ? "T" : "F",
             pPtpClock->config.override.defaultDS.priority1);
      printf("            Priority2:      %s        %d\n",
            (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_PRIORITY2]) ? "T" : "F",
             pPtpClock->config.override.defaultDS.priority2);
      printf("               Domain:      %s        %d\n",
            (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_DOMAIN]) ? "T" : "F",
             pPtpClock->config.override.defaultDS.domainNumber);
      printf("   Current UTC Offset:      %s        %d\n",
            (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_CURRENT_UTC_OFFSET]) ? "T" : "F",
             pPtpClock->config.override.timePropertiesDS.currentUtcOffset);
      printf("     UTC Offset VALID:      %s        %s\n",
            (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_CURRENT_UTC_OFFSET_VALID]) ? "T" : "F",
            (pPtpClock->config.override.timePropertiesDS.currentUtcOffsetValid ? "T" : "F"));
      printf("       Time Traceable:      %s        %s\n",
            (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_TIME_TRACEABLE]) ? "T" : "F",
            (pPtpClock->config.override.timePropertiesDS.timeTraceable ? "T" : "F"));
      printf("  Frequency Traceable:      %s        %s\n",
            (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_FREQUENCY_TRACEABLE]) ? "T" : "F",
            (pPtpClock->config.override.timePropertiesDS.frequencyTraceable ? "T" : "F"));
      printf("          Time Source:      %s        %d\n",
            (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_TIME_SOURCE]) ? "T" : "F",
             pPtpClock->config.override.timePropertiesDS.timeSource);
      printf("        Steps Removed:      %s        0x%02X\n",
            (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_STEPS_REMOVED]) ? "T" : "F",
             pPtpClock->config.override.currentDS.stepsRemoved);
   }

   return status;
}  /* END zl303xx_PtpClockEgressOverrideShow */

/* zl303xx_PtpClockUncalibratedUsePreviousClassSet */
/**
   Set whether the clock advertises the new Parent clockClass while in
   UNCALIBRATED state or it keeps the old clockClass until the SLAVE state is
   achieved.

  Parameters:
   [in]  clockHandle    Handle to an existing clock.
   [in]  usePrevClass   Boolean indicating to use the old or new clockClass
                              while in the UNCALIBRATED state.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or usePrevClass is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockUncalibratedUsePreviousClassSet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_BooleanE usePrevClass)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock;

   /* Check the boolean value. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(usePrevClass);
   }

   /* Get the stream data */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   if (status == ZL303XX_OK)
   {
      pPtpClock->config.uncalibrated.usePreviousClass = usePrevClass;

      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}

/* zl303xx_PtpClockStreamStateAutoUpdateEn */
/**
   Sets the enabled flag to determine if the local Stream States of the specified
   Clock will be updated automatically or if manual updates are expected.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   enabled      Automatic STream State updates enabled or not.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid or Boolean value is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockStreamStateAutoUpdateEn(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_BooleanE enabled)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_PtpClockS *pPtpClock;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(enabled);
   }

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pPtpClock);
      status = mStatus;
   }

   if (status == ZL303XX_OK)
   {
      /* Set the enabled flag. */
      pPtpClock->config.autoUpdateStreamStates = enabled;

      /* It's possible that this has run for some time without an update.
       * If enabling AUTO-UPDATE, trigger an Audit cycle immediately. */
      if (pPtpClock->config.autoUpdateStreamStates == ZL303XX_TRUE)
      {
         status = zl303xx_PtpClockStateAudit(pPtpClock);
      }
   }

   /* Release the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}

/* zl303xx_PtpClockPathTraceEnable */
/**
   Enables or disables the optional PATH TRACE functionality on the Clock.
   (Refer to IEEE-1588-2008: Clause 16.2)

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   enabled      TRUE = enable PATH TRACE; disable otherwise.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid or Boolean value is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockPathTraceEnable(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_BooleanE enable)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_PtpClockS *pPtpClock;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(enable);
   }

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pPtpClock);
      status = mStatus;
   }

   if (status == ZL303XX_OK)
   {
      /* Set the enabled flag. */
      pPtpClock->config.pathTrace.enabled = enable;

      /* If disabling the functionality, clear the current history. */
      if (enable == ZL303XX_FALSE)
      {
         /* Clear PATH TRACE history. */
         status = zl303xx_PtpClockPathTraceListUpdate(&pPtpClock->pathTraceList, 0, NULL);
      }
   }

   /* Release the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* zl303xx_PtpClockPathTraceEnable */

/* zl303xx_PtpClockPathTraceListGet */
/**

   Retrieves the current PATH TRACE list from the specified clock if the
   functionality is ENABLED (otherwise, 0 entries are returned). Includes:
   - items it has received from the upstream parent.
   - its own Clock Identity appended to the end.

  Parameters:
   [in]  clockHandle  Handle of the targeted PTP Clock.
   [in]  listCount    As an input, the maximum number of entries requested
                           by the user - should correspond with the size of
                           the array passed in as the output.
                           (Use -1 or 0 to get all entries).

   [out] listCount   As an output, the number of entries returned.
   [out] pPtList     Pointer to the array of Path Trace entries returned.
                           pPtList[listCount - 1] is always the local Clock
                           identity if the PATH TRACE functionality is ENABLED.

  Return Value:
     ZL303XX_OK                      Success
     ZL303XX_PARAMETER_INVALID       The clockHandle is invalid.
     ZL303XX_INVALID_POINTER         The listCount or pPtList pointer is NULL.
     ZL303XX_STATISTICS_NOT_ENABLED  PATH TRACE functionality is disabled.
     ZL303XX_UNSUPPORTED_OPERATION   The input array is to small for the
                                          current PATH TRACE list.

 *****************************************************************************/
zlStatusE zl303xx_PtpClockPathTraceListGet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint16T *listCount,
      zl303xx_ClockIdentity * pPtList)
{
   /* Status variables */
   zlStatusE status = ZL303XX_OK;
   zlStatusE mStatus = ZL303XX_ERROR;

   /* Pointer to the targeted clock. */
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check the input pointers */
   if ((listCount == NULL) || (pPtList == NULL))
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_PtpClockPathTraceListGet(): INVALID PTR;   ",
            0,0,0,0,0,0);
      status = ZL303XX_INVALID_POINTER;
   }

   /* Get the Clock Data for the target */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);

      if (status != ZL303XX_OK)
      {
         ZL303XX_TRACE_ALWAYS (
               "zl303xx_PtpClockPathTraceListGet(): Call to zl303xx_PtpClockDataGet() failed!",
               0,0,0,0,0,0);
      }

      /* Lock the Clock mutex before accessing the dynamic data. */
      else
      {
         mStatus = zl303xx_PtpClockMutexLock(pPtpClock);
         status = mStatus;
      }
   }

   /* Check if the functionality is enabled. */
   if (status == ZL303XX_OK)
   {
      if (pPtpClock->config.pathTrace.enabled == ZL303XX_FALSE)
      {
         ZL303XX_TRACE_ALWAYS (
               "zl303xx_PtpClockPathTraceListGet(): PATH TRACE disabled",
               0,0,0,0,0,0);

         status = ZL303XX_STATISTICS_NOT_ENABLED;
      }

      /* Never return partial data. If the input array is too small for the
       * total dynamic data then return an error.  */
      else if ((*listCount != 0) &&
               (*listCount != (Uint16T)(-1)) &&

               /* Equal is not big enough since the local clock ID is added.*/
               (*listCount <= pPtpClock->pathTraceList.listLength))
      {
         ZL303XX_TRACE_ALWAYS (
               "zl303xx_PtpClockPathTraceListGet(): Array size (%d) too small; %d required",
               *listCount,
               1 + pPtpClock->pathTraceList.listLength, 0,0,0,0);

         status = ZL303XX_UNSUPPORTED_OPERATION;
      }
   }

   /* Copy all necessary data. */
   if (status == ZL303XX_OK)
   {
      /* First copy the dynamic table. */
      *listCount = pPtpClock->pathTraceList.listLength;
      if (pPtpClock->pathTraceList.listLength > 0)
      {
         OS_MEMCPY(pPtList,
                pPtpClock->pathTraceList.pClockIdList,
                pPtpClock->pathTraceList.listLength * PTP_V2_CLK_IDENTITY_LENGTH);
      }

      /* Copy the local clock identity. */
      OS_MEMCPY(&pPtList[*listCount], pPtpClock->config.defaultDS.clockIdentity, PTP_V2_CLK_IDENTITY_LENGTH);
      (*listCount)++;
   }

   /* For any of the error scenarios, return 0. */
   else if (listCount != NULL)
   {
      *listCount = 0;
   }

   /* Release the Clock Mutex if we ever had it. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* zl303xx_PtpClockPathTraceListGet */

/* zl303xx_PtpClockPathTraceForExtParent */
/**
   Intended for distributed systems only.
   Routine to set the PATH-TRACE list that will be issued by a clock when an
   EXTERNAL-PARENT has been selected as the Parent Source

   The supplied list SHOULD NOT include the current clock identity; it will be
   added automatically to any transmitted ANNOUNCE messages.

   The following determines the source of the PATH-TRACE information:
   - If Parent Source is LOCAL-CLOCK or LOCAL-PORT, only the DefaultDS Clock
     Identity is appended.
   - If Parent Source is LOCAL-STREAM, the PATH-TRACE list in the FMT is added
     followed by the DefaultDS Clock Identity.
   - If Parent Source is EXTERNAL, the PATH-TRACE list set by this API is added
     followed by the DefaultDS Clock Identity.

  Parameters:
   [in]  clockHandle    Handle to an existing clock.
   [in]  listCount      Number of Clock Identities in the list. If 0, then
                              the current list is deleted (if it existed).
   [in]  pPtList        Pointer to a list of Clock Identity entries. Can
                              only be NULL if listCount is '0'.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid.
     ZL303XX_INVALID_POINTER       pPtList is NULL but listCount > 0.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockPathTraceForExtParent(
      zl303xx_PtpClockHandleT clockHandle,
      Uint16T listCount,
      zl303xx_ClockIdentity * pPtList)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Input parameters are checked in the UPDATE sub-routine. */

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pPtpClock);
      status = mStatus;
   }

   /* Update the List. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockPathTraceListUpdate(
                  &pPtpClock->gmSource.extPathTraceList, listCount, pPtList);
   }

   /* Release the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* END zl303xx_PtpClockPathTraceForExtParent */

/* zl303xx_PtpClockSynchronizationUncertainEn */
/**
   Enables or disables the optional Synchronization Uncertain functionality
   on the Clock. (Refer to ITU-G.8275.1: Annex E).

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   enabled      TRUE = enable Synchronization Uncertain; disable otherwise.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid or Boolean value is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockSynchronizationUncertainEn(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_BooleanE enable)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(enable);
   }

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* No need to lock the clock mutex for this BOOLEAN. */
   /* Set the enabled flag. */
   if (status == ZL303XX_OK)
   {
      pPtpClock->config.synchronizationUncertain.enabled = enable;
   }

   return status;
}  /* zl303xx_PtpClockSynchronizationUncertainEn */

/* zl303xx_PtpClockSynchronizationUncertainEvaluate */
/**
   Evaluates what the egress value if the clocks synchronizationUncertain flag
   should be (i.e. any override value will replace this evaluation).

   Rules are:
   1) If the optional functionality is disabled, egress flag = FALSE.
   2) If the parent's synchronizationUncertain flag is TRUE, egress flag = TRUE.
   3) If the Server Stream is UNCALIBRATED, egress flag = TRUE.
   4) Profile specific evaluation value (if provisioned).

  Parameters:
   [in]  pPtpClock   Pointer to a zl303xx_PtpClockS structure.

   [out] synchroUncertain  Value of the synchronizationUncertain evaluation.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      Boolean pointer is NULL.


*******************************************************************************/
zlStatusE zl303xx_PtpClockSynchronizationUncertainEvaluate(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_BooleanE *synchroUncertain)
{
   zlStatusE status = ZL303XX_OK;

   /* Check input parameters. */
   if (synchroUncertain != NULL)
   {
      /* Set a default. */
      *synchroUncertain = ZL303XX_FALSE;
   }
   else
   {
      status = ZL303XX_INVALID_POINTER;
   }

   /* Set the enabled flag. */
   if ((status == ZL303XX_OK) &&
       (pPtpClock->config.synchronizationUncertain.enabled == ZL303XX_TRUE))
   {
      /* Conditions for setting this field:
       * 1) If flag from the ParentDS is TRUE then advertise TRUE.
       * 2) If the parent stream is UNCALIBRATED then advertise TRUE.
       * 3) Profile or implementation specific. */
	   
      /* Satisfying condition (1) for remote parent, VP source or DefaultDS
       * source: select the current ParentDS value. */
      /* The ParentDS value is in the Clock's active TimePropertiesDS */
      *synchroUncertain = pPtpClock->timePropertiesDS.synchronizationUncertain;
 
      /* If a local Stream is selected as the server but is not yet locked,
       * advertise Uncertain as TRUE. */
      if ((pPtpClock->gmSource.pPtpStream != NULL) &&
          (pPtpClock->gmSource.pPtpStream->streamSubstate == 
                                             PTP_STREAM_SUBSTATE_UNCALIBRATED))
      {
         *synchroUncertain = ZL303XX_TRUE;
      }
   }

   return status;
}  /* zl303xx_PtpClockSynchronizationUncertainEvaluate */

/*

  Function Name:
   zl303xx_PtpGetNumServers

  Details:
   Gets the number of available servers known by a stream.

  Parameters:
   [in]   streamHandle      Handle to a previously created stream.
   [out]  numMcastServers   The number of visible multicast servers.
   [out]  numUcastServers   The number of visible unicast servers.

  Return Value:
   zlStatusE

 *****************************************************************************/
zlStatusE zl303xx_PtpGetNumServers(zl303xx_PtpStreamHandleT streamHandle,
                                   Sint16T *numMcastServers,
                                   Sint16T *numUcastServers)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(numMcastServers) |
               ZL303XX_CHECK_POINTER(numUcastServers);
   }

   /* Get a pointer to the stream structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   if (status == ZL303XX_OK)
   {
      if (stream->config.unicast == ZL303XX_FALSE)
      {
         /* Multicast stream. */
         *numMcastServers = stream->portData->clock->foreignMasterDS.numActiveRecords;
         *numUcastServers = 0;
      }
      else
      {
         *numMcastServers = 0;
         *numUcastServers = 1;
      }
   }

   return status;
}

/* zl303xx_PtpClockDataGet */
/** 

   Returns a pointer to an internal clock data structure.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [out]  ppClock      Pointer to the clock data structure.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    ppClock is NULL.
     ZL303XX_PARAMETER_INVALID  clockHandle is out of range or references a
                                    clock that does not exist.

  Notes:
   For internal API use only. Should not be called by an application.

*******************************************************************************/
zlStatusE zl303xx_PtpClockDataGet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpClockS **ppClock)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtpClockDataGet";

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(ppClock);
   }

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_CLOCK_HANDLE(clockHandle);
      if (status != ZL303XX_OK)
      {
         ZL303XX_TRACE_ALWAYS("%s: (ERROR) clockHandle=%u out of range (0-%u)",
               fnName, clockHandle, ZL303XX_PTP_NUM_CLOCKS_MAX - 1, 0,0,0);
      }
   }

   if (status == ZL303XX_OK)
   {
      *ppClock = Zl303xx_PtpClockArray[clockHandle];
      if (*ppClock == NULL)
      {
         ZL303XX_TRACE_ALWAYS("%s: clockHandle=%u not initialized",
               fnName, clockHandle, 0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   return status;
}

/* zl303xx_PtpClockDataGetForProfile */
/** 

   Returns a pointer to an internal clock data structure ONLY IF the clock is
   operating the indicated PTP Profile.

   With the addition of PTP Profiles and unions to store profile specific data,
   a clock must be running the indicated profile in order for certain data to
   map correctly to the profile specific structure.

  Parameters:
   [in]  clockHandle    Handle to an existing clock.
   [in]  ptpProfile     Required PTP Profile.

   [out]  ppClock      Pointer to the clock data structure.

  Return Value:
    ZL303XX_OK                 Success.
    ZL303XX_INVALID_POINTER    ppClock is NULL.
    ZL303XX_PARAMETER_INVALID  clockHandle is out of range, references a
                                    clock that does not exist.
    ZL303XX_INVALID_MODE       clockHandle references a clock that is not
                                    running the required PTP profile.

  Notes:
   For internal API use only. Should not be called by an application.

*******************************************************************************/
zlStatusE zl303xx_PtpClockDataGetForProfile(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpProfileE ptpProfile,
      zl303xx_PtpClockS **ppClock)
{
   zlStatusE status = ZL303XX_OK;

   /* First, see if the clock even exists. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, ppClock);
   }

   /* Check the Profile value. */
   if (status == ZL303XX_OK)
   {
      if ((*ppClock)->config.profile != ptpProfile)
      {
         ZL303XX_TRACE_ALWAYS("zl303xx_PtpClockDataGetForProfile: Clock(%u) Profile(%u)(%u expected)",
               clockHandle, (*ppClock)->config.profile, ptpProfile, 0,0,0);
         status = ZL303XX_INVALID_MODE;
      }
   }

   return status;
}

/* zl303xx_PtpClockDataGetForIdentity */
/** 

   Returns a pointer to an internal clock data structure for the Clock
   Identity provided. (Assumes clockIdentities are unique on multi-clock
   nodes).

  Parameters:
   [in]   clockIdentity    Clock Identity of an existing clock.
   [out]  ppClock          Pointer to the clock data structure.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    ppClock is NULL.
     ZL303XX_PARAMETER_INVALID  clockHandle is out of range or references a
                                    clock that does not exist.

  Notes:
   For internal API use only. Should not be called by an application.

*******************************************************************************/
zlStatusE zl303xx_PtpClockDataGetForIdentity(
      zl303xx_ClockIdentity clockIdentity,
      zl303xx_PtpClockS **ppClock)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockHandleT clockId;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(ppClock);
   }

   if (status == ZL303XX_OK)
   {
      /* Assume not found. */
      *ppClock = NULL;
      status = ZL303XX_TABLE_ENTRY_NOT_FOUND;

      for (clockId = 0; clockId < ZL303XX_PTP_NUM_CLOCKS_MAX; clockId++)
      {
         if (Zl303xx_PtpClockArray[clockId])
         {
            if (zl303xx_PtpV2EqualClockIdentities(
                           Zl303xx_PtpClockArray[clockId]->config.defaultDS.clockIdentity,
                           clockIdentity))
            {
               *ppClock = Zl303xx_PtpClockArray[clockId];

               /* Mark the entry as found. */
               status = ZL303XX_OK;
               break;
            }
         }
      }
   }

   return status;
}

/*
  Function Name:
   zl303xx_PtpFindFirstValidClock

  Details:
   Returns a pointer to the first valid clock data structure.

  Parameters:
   [in]  pPtpClock   Pointer to a zl303xx_PtpClockS structure

   [out] pPtpClock   Pointer to a clock data structure. NULL if no valid
                           clocks exist.

  Return Value:
   zlStatusE

  Notes:
   For internal API use only. Should not need to be called by an application

*******************************************************************************/
zlStatusE zl303xx_PtpFindFirstValidClock(zl303xx_PtpClockS **ppPtpClock)
{
   zlStatusE status;
   zl303xx_PtpClockHandleT clockId;

   status = ZL303XX_CHECK_POINTER(ppPtpClock);

   if (status == ZL303XX_OK)
   {
      *ppPtpClock = NULL;

      for (clockId = 0; clockId < ZL303XX_PTP_NUM_CLOCKS_MAX; clockId++)
      {
         if (Zl303xx_PtpClockArray[clockId])
         {
            *ppPtpClock = Zl303xx_PtpClockArray[clockId];
            break;
         }
      }
   }

   return status;
}

/*

  Function Name:
   zl303xx_PtpFindFirstActiveStream

  Details:
   Retrieves a pointer to the data structure for the first configured stream on this clock

  Parameters:
   [out]    clockStream Pointer to the data structure for the first configured clock stream
               or NULL if there are no configured streams

  Return Value:
   zlStatusE

  Notes:
   This is used for API internal purposes only to obtain data members that
   would be otherwise hidden

*******************************************************************************/

zlStatusE zl303xx_PtpFindFirstActiveStream(zl303xx_ClockStreamS **clockStream)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T streamId;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(clockStream);
   }
   if (status == ZL303XX_OK)
   {
      *clockStream = NULL;
      for (streamId = 0; streamId < ZL303XX_PTP_NUM_STREAMS_MAX; streamId++)
      {
         if (Zl303xx_PtpStreamArray[streamId])
         {
            *clockStream = Zl303xx_PtpStreamArray[streamId];
            break;
         }
      }
   }

   return status;
}

/* zl303xx_PtpClockCurrMasterGet */
/**
   Returns the stream handle that is currently in slave mode (S1 state) for a
   clock.

  Parameters:
   [in]   clockHandle    Handle to an existing clock.
   [out]  pStreamHandle  Handle to the stream that communicates with the
                              master that is currently driving the PTP clock.
                              -1 indicates there is no best master.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pStreamHandle is NULL.
     ZL303XX_PARAMETER_INVALID  clockHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockCurrMasterGet(zl303xx_PtpClockHandleT clockHandle,
                                        zl303xx_PtpStreamHandleT *pStreamHandle)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pClock;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pStreamHandle);
   }

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pClock);
   }

   /* Return the handle to the current master. */
   if (status == ZL303XX_OK)
   {
      if ((pClock->gmSource.pPtpStream != NULL) &&
          (pClock->gmSource.pPtpStream->streamSubstate == PTP_STREAM_SUBSTATE_SLAVE))
      {
         *pStreamHandle = pClock->gmSource.pPtpStream->clockStreamHandle;
      }
      else
      {
         *pStreamHandle = (Uint32T)ZL303XX_PTP_INVALID;
      }
   }

   return status;
}

/* zl303xx_PtpClockManualSwitch */
/**
   Forces the BMCA state machine to select a specific master when in
   non-revertive mode.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   pPortAddr    Port address of the master to select.

  Return Value:
     ZL303XX_OK                     Success.
     ZL303XX_INVALID_POINTER        pPortAddr is NULL.
     ZL303XX_PARAMETER_INVALID      clockHandle is not valid or pPortAddr cannot
                                        be found in the foreign master table.
     ZL303XX_UNSUPPORTED_OPERATION  Revertive BMCA is enabled.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL   Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL   Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockManualSwitch(zl303xx_PtpClockHandleT clockHandle,
                                       zl303xx_PortAddress *pPortAddr)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_PtpClockS *pClock;
   const char *fnName = "zl303xx_PtpClockManualSwitch";

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pPortAddr);
   }
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortAddressCheck(pPortAddr);
   }

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pClock);
   }

   /* Manual switching is only supported in non-revertive BMCA mode. */
   if (status == ZL303XX_OK)
   {
      if (pClock->config.bmca.revertiveEn)
      {
         ZL303XX_TRACE_ALWAYS("%s: Manual switching only supported in non-revertive mode.",
                              fnName, 0,0,0,0,0);
         status = ZL303XX_UNSUPPORTED_OPERATION;
      }
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pClock);
      status = mStatus;
   }

   /* Search the clock's FMT for a master with a matching port address. */
   if (status == ZL303XX_OK)
   {
      Uint16T count;
      zl303xx_ForeignMasterDS *fmt = &pClock->foreignMasterDS;

      for (count = 0; count < fmt->numActiveRecords; count++)
      {
         if ((fmt->foreign[count].streamData) &&
             (zl303xx_PtpComparePortAddress(&fmt->foreign[count].portAddress,
                                            pPortAddr) == 0))
         {
            /* Match found. */
            break;
         }
      }

      /* No matching entry */
      if (count == fmt->numActiveRecords)
      {
         ZL303XX_TRACE_ALWAYS("%s: No FMT entry found matching port address.",
                              fnName, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }

      /* Matching entry, but not valid yet. */
      else if (fmt->foreign[count].valid == ZL303XX_FALSE)
      {
         ZL303XX_TRACE_ALWAYS("%s: Matching FMT entry not valid -- no switch performed.",
                              fnName, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }

      /* Valid, matching entry. Make this the "selected" address. */
      else
      {
         pClock->bmca.selectedAddr = *pPortAddr;
      }
   }

   /* Release the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pClock, status);
   }

   return status;
}

/* zl303xx_PtpClockUnicastStackedTlvEnSet */
/**
   Sets the unicast negotiation stacked TLV enabled setting for all ports
   attached to a clock.

  Parameters:
   [in]   clockHandle   Handle to an existing clock.
   [in]   stackedTlvEn  ZL303XX_TRUE to enable sending stacked TLVs from this
                             clock. ZL303XX_FALSE to send a single TLV per Signaling
                             message.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or stackedTlvEn is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockUnicastStackedTlvEnSet(zl303xx_PtpClockHandleT clockHandle,
                                                 zl303xx_BooleanE stackedTlvEn)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_PtpClockS *pClock;
   zl303xx_PortDataS *pPort = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(stackedTlvEn);
   }

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pClock);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pClock);
      status = mStatus;
   }

   /* Update the stackedTlvEn setting for each port attached to this clock. */
   if (status == ZL303XX_OK)
   {
      ZL303XX_PTP_FOR_EACH_PORT(&pClock->portList, pPort)
      {
         pPort->config.unicastNegotiation.stackedTlvEn = stackedTlvEn;
      }
   }

   /* Release the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pClock, status);
   }

   return status;
}

/* zl303xx_PtpClockStackAllServicesOn1stRequest */
/**
   Determines if a stacked TLV, which includes all required services, will be
   sent on the first unicast contract request.
   In all other cases the 'UnicastStackedTlvEn' setting will be used:
      - Stacked-TLV == TRUE: Stacked-TLV-Override == ANY
        Initial and all subsequent requests are Stacked TLVs.
      - Stacked-TLV == FALSE: Stacked-TLV-Override == FALSE
        Initial and all subsequent requests are Single TLVs.
      - Stacked-TLV == FALSE: Stacked-TLV-Override == TRUE
        Initial request is Single TLV, subsequent requests are Single TLVs.

  Parameters:
   [in]  clockHandle          Handle to an existing clock.
   [in]  stackInitialRequest  Set to ZL303XX_TRUE to force the initial unicast
                                   contract request to use a stacked TLV that
                                   includes all services.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or stackedTlvEn is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockStackAllServicesOn1stRequest(zl303xx_PtpClockHandleT clockHandle,
                                                 zl303xx_BooleanE stackInitialRequest)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check the input setting. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(stackInitialRequest);
   }

   /* Get the associated clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Check if the configuration needs to be changed;
    * the clock may already have the required setting. */
   if ((status == ZL303XX_OK) &&
       (pPtpClock->stackAllServicesOn1stRequest != stackInitialRequest))
   {
      /* Lock the clock mutex. */
      if (zl303xx_PtpClockMutexLock(pPtpClock) != ZL303XX_OK)
      {
         ZL303XX_ERROR_NOTIFY("stackInitialRequest: OS_MUTEX_TAKE() failed");
      }

      /* If we have the MUTEX, update the data. */
      else
      {
         /* Update the values in the clock configuration. */
         pPtpClock->stackAllServicesOn1stRequest = stackInitialRequest;

         /* Release the clock mutex */
         (void)zl303xx_PtpClockMutexUnlock(pPtpClock, status);
      }
   }

   return status;
}  /* END zl303xx_PtpClockStackAllServicesOn1stRequest */

/* zl303xx_PtpClockBmcaUpdateIntervalSet */
/**
   Sets the BMCA Update Event interval for automatic uploading.

  Parameters:
   [in]   clockHandle   Handle to an existing clock.
   [in]   intervalSec   Update interval in seconds (0 = disabled).

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockBmcaUpdateIntervalSet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint32T intervalSec)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   /* Set the BMCA Update Interval time and re-initialize the timers. */
   if (status == ZL303XX_OK)
   {
      /* Set the BMCA Update Interval time. */
      pPtpClock->config.bmca.updateEventSec = intervalSec;

      /* Reset the timer. */
      zl303xx_PtpClockTimersInit(pPtpClock);

      /* Release the clock mutex. */
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}

/* zl303xx_PtpClockBmcaUpdateTriggerSet */
/**
   Sets the BMCA Update Event trigger for data uploading according to the
   zl303xx_PtpClockBmcaTriggerE enum.

  Parameters:
   [in]  clockHandle    Handle to an existing clock.
   [in]  updateTrigger  Update trigger to cause BMCA update.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or updateTrigger invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockBmcaUpdateTriggerSet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpClockBmcaTriggerE updateTrigger)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Check the parameter range */
   if (status == ZL303XX_OK)
   {
      if (updateTrigger > ZL303XX_BMCA_TRIGGER_DISABLED)
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   /* Set the BMCA Update Trigger and re-evaluate the BMCA timer. */
   if (status == ZL303XX_OK)
   {
      /* Set the BMCA Update Interval time. */
      pPtpClock->config.bmca.updateTrigger = updateTrigger;

      /* Reset the timer. */
      zl303xx_PtpClockTimersInit(pPtpClock);

      /* Release the clock mutex. */
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}

/*****************   INTERNAL PTP FUNCTION DEFINITIONS   **********************/

/* zl303xx_PtpClockStructAllocate */
/** 

   Allocates memory for a PTP clock data structure.

  Parameters:
   [in]  pClockCreate  Structure containing the Clock creation parameters.

   [out]  ppPtpClock   Pointer to the allocated structure.

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockStructAllocate(
      zl303xx_PtpClockCreateS *pClockCreate,
      zl303xx_PtpClockS **ppPtpClock)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *newClock = NULL;

   /* Allocate memory for the clock structure. */
   if (status == ZL303XX_OK)
   {
      newClock = (zl303xx_PtpClockS *)OS_CALLOC(1, sizeof(zl303xx_PtpClockS));

      if (newClock == NULL)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Clock memory allocate failed.", 0,0,0,0,0,0);
         status = ZL303XX_RTOS_MEMORY_FAIL;
      }
   }

   if (status == ZL303XX_OK)
   {
      /* Set the table size. */
      newClock->foreignMasterDS.maxRecords = pClockCreate->maxForeignRecords;

      status = zl303xx_PtpFmtCreate(&(newClock->foreignMasterDS));
   }

   /* Return the clock pointer to the caller. */
   if (status == ZL303XX_OK)
   {
      *ppPtpClock = newClock;
   }
   else if (newClock != NULL)
   {
      /* Free any allocated port memory. */
      OS_FREE(newClock);
      newClock = NULL;
   }

   return status;
}

/* zl303xx_PtpClockStructFree */
/** 

   Frees memory used by a PTP clock data structure.

  Parameters:
   [in,out]  ppPtpClock   Pointer to the previously allocated structure. It
                                will point to NULL after this function completes.

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockStructFree(zl303xx_PtpClockS **ppPtpClock)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      /* Free the Foreign Master Table.
       * status is always OK */
      status = zl303xx_PtpFmtDelete(&((*ppPtpClock)->foreignMasterDS));

      /* Free any EXTERNAL Path Trace information that has been provisioned. */
      (void)zl303xx_PtpClockPathTraceListUpdate(
            &(*ppPtpClock)->gmSource.extPathTraceList, 0, NULL);

      /* Free the current Active Path Trace information. */
      (void)zl303xx_PtpClockPathTraceListUpdate(
            &(*ppPtpClock)->pathTraceList, 0, NULL);

      /* Free the rest of the clock object. */
      OS_FREE(*ppPtpClock);
      *ppPtpClock = NULL;
   }

   return status;
}

/* zl303xx_PtpClockMutexLock */
/** 

   Locks the mutex associated with a PTP clock.

  Parameters:
   [in]  clock   Pointer to a clock data structure.

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockMutexLock(zl303xx_PtpClockS *pPtpClock)
{
   if (!pPtpClock->mutexId || (OS_MUTEX_TAKE(pPtpClock->mutexId) != OS_OK))
   {
      ZL303XX_TRACE_ERROR("   (ERROR) Clock mutex take failed.", 0,0,0,0,0,0);
      return ZL303XX_RTOS_MUTEX_TAKE_FAIL;
   }

   return ZL303XX_OK;
}

/* zl303xx_PtpClockMutexUnlock */
/** 

   Releases the mutex associated with a PTP clock.

  Parameters:
   [in]  pPtpClock  Pointer to a clock data structure.
   [in]  status     Current status before this function was called. This value
                         will be returned if it is not passed in as ZL303XX_OK.

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockMutexUnlock(zl303xx_PtpClockS *pPtpClock, zlStatusE status)
{
   if (!pPtpClock->mutexId || (OS_MUTEX_GIVE(pPtpClock->mutexId) != OS_OK))
   {
      ZL303XX_TRACE_ERROR("   (ERROR) Clock mutex give failed.", 0,0,0,0,0,0);
      return (status == ZL303XX_OK) ? ZL303XX_RTOS_MUTEX_GIVE_FAIL : status;
   }

   return status;
}

/* zl303xx_PtpClockMsgQCreate */
/** 

   Creates a message queue for a PTP clock.

  Parameters:
   [in,out]  pPtpClock  Pointer to a clock data structure. Requires the
                              zl303xx_PtpClockS::config structure to be populated.
                              This function modifies the zl303xx_PtpClockS::msgQId
                              member.

  Notes: Assumes all input parameters are valid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockMsgQCreate(zl303xx_PtpClockS *pPtpClock)
{
   zlStatusE status = ZL303XX_OK;

   /* Check the configured length of the queue. */
   if (status == ZL303XX_OK)
   {
      if (pPtpClock->config.msgQLength == 0)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Invalid msgQLength=%ld.",
                             pPtpClock->config.msgQLength, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Create the message queue. */
   if (status == ZL303XX_OK)
   {
      pPtpClock->msgQId = OS_MSG_Q_CREATE(pPtpClock->config.msgQLength, sizeof(zl303xx_PtpTaskMsgU));

      /* Error creating message queue. */
      if (pPtpClock->msgQId == OS_MSG_Q_INVALID)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Message queue create failed.", 0,0,0,0,0,0);
         status = ZL303XX_RTOS_MSGQ_CREATE_FAIL;
      }
   }

   return status;
}

/* zl303xx_PtpClockMsgQDelete */
/** 

   Deletes the message queue associated with a PTP clock.

  Parameters:
   [in,out]  pPtpClock  Pointer to a clock data structure. This function
                              modifies the zl303xx_PtpClockS::msgQId member.

  Notes: Assumes all input parameters are valid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockMsgQDelete(zl303xx_PtpClockS *pPtpClock)
{
   zlStatusE status = ZL303XX_OK;

   /* Bugfix for some versions of pthreads: Putting the thread to sleep here
    * prevents a shutdown issue where something in the pthreads library was
    * causing 100% CPU usage. */
   OS_TASK_DELAY(1);

   if (OS_MSG_Q_DELETE(pPtpClock->msgQId) == OS_OK)
   {
      pPtpClock->msgQId = OS_MSG_Q_INVALID;
   }
   else
   {
      ZL303XX_TRACE_ERROR("   (ERROR) Message queue delete failed.", 0,0,0,0,0,0);
      status = ZL303XX_RTOS_MSGQ_DELETE_FAIL;
   }

   return status;
}

/* zl303xx_PtpClockMsgQSend */
/** 

   Sends a message to a PTP clock.

  Parameters:
   [in]   pPtpClock  Pointer to a clock data structure.
   [in]   msg        Pointer to the clock message to send. Must be a
                          structure type in zl303xx_PtpTaskMsgU.
   [in]   msgSize    sizeof(*msg) as passed to this function.

  Notes: Assumes all input parameters are valid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockMsgQSend(zl303xx_PtpClockS *pPtpClock, void *msg,
                                   Uint32T msgSize)
{
   zlStatusE status = ZL303XX_OK;
   osStatusT osStatus;

   if (pPtpClock->msgQId == OS_MSG_Q_INVALID)
   {
      ZL303XX_TRACE_ERROR("   (ERROR) Clock message queue does not exist.", 0,0,0,0,0,0);
      status = ZL303XX_RTOS_MSGQ_INVALID;
   }
   else  /* Send the message into the queue. */
   {
      /* Queue the message. If there is no space, wait 1. This
       * will throttle the ingress pkt rate. */
      if (OS_OK != (osStatus = OS_MSG_Q_SEND(pPtpClock->msgQId, msg, msgSize, 1)))
      {
          fputs("*^", stderr); /* reentrant - we are in an isr handler! */
          status = ZL303XX_RTOS_MSGQ_SEND_FAIL;
      }
   }

   return status;
}

/* zl303xx_PtpClockMsgQRecv */
/** 

   Reads a message sent to a PTP clock.

  Parameters:
   [in]   pPtpClock  Pointer to a clock data structure.
   [out]  msg        Received PTP clock message.

  Notes: Assumes all input pointers are valid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockMsgQRecv(zl303xx_PtpClockS *pPtpClock,
                                   zl303xx_PtpTaskMsgU *msg)
{
   zlStatusE status = ZL303XX_OK;

   if (pPtpClock->msgQId == OS_MSG_Q_INVALID)
   {
      ZL303XX_TRACE_ERROR("   (ERROR) Message queue does not exist.", 0,0,0,0,0,0);
      status = ZL303XX_RTOS_MSGQ_INVALID;
   }
   else  /* Receive the message from the queue. */
   {
      /* Check if a message was actually received. */
      if (OS_MSG_Q_RECEIVE(pPtpClock->msgQId, (Sint8T *)msg,
                           sizeof(*msg), OS_WAIT_FOREVER) == OS_ERROR)
      {
         status = ZL303XX_RTOS_MSGQ_RECEIVE_FAIL;
      }
   }

   return status;
}

/* zl303xx_PtpClockTaskStart */
/** 

   Starts the OS task associated with a PTP clock.

  Parameters:
   [in,out]  pPtpClock  Pointer to a clock data structure. This function
                           modifies the zl303xx_PtpClockS::taskId and
                           zl303xx_PtpClockS::config::taskname members.

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockTaskStart(zl303xx_PtpClockS *pPtpClock)
{
   zlStatusE status = ZL303XX_OK;
   char newName[OS_MAX_TASK_NAME] = {'\0'};

   /* Start a task for this clock. */
   OS_SNPRINTF(newName, sizeof(newName), "%s%dzl303xx", pPtpClock->config.taskName, (SINT_T)pPtpClock->clockHandle);
   strncpy(pPtpClock->config.taskName, newName, sizeof(pPtpClock->config.taskName));
   pPtpClock->config.taskName[sizeof(pPtpClock->config.taskName) - 1] = '\0';

   pPtpClock->taskId = OS_TASK_SPAWN(pPtpClock->config.taskName,
                                     pPtpClock->config.taskPriority,
                                     0,
                                     pPtpClock->config.taskStackSize,
                                     (OS_TASK_FUNC_PTR)zl303xx_PtpTask,
                                     (Sint32T)ZL303XX_FALSE);

   if (pPtpClock->taskId == OS_TASK_INVALID)
   {
      ZL303XX_TRACE_ERROR("   (ERROR) Clock task spawn failed.", 0,0,0,0,0,0);
      status = ZL303XX_RTOS_TASK_CREATE_FAIL;
   }

   return status;
}

/* zl303xx_PtpClockTaskStop */
/** 

   Stops the OS task associated with a PTP clock.

  Parameters:
   [in,out]  pPtpClock  Pointer to a clock data structure. This function modifies
                             the zl303xx_PtpClockS::taskId member.

  Notes:   Assumes all input parameters are valid. Assumes the clock mutex has
        already been locked.

*******************************************************************************/
zlStatusE zl303xx_PtpClockTaskStop(zl303xx_PtpClockS *pPtpClock)
{
   /* Lock the clock mutex to ensure the clock task is not holding it when the
    * task gets deleted. */
   zlStatusE status = ZL303XX_OK;

   zl303xx_PtpClockMsgQueueS msg;

   OS_MEMSET(&msg, 0, sizeof(msg));
    msg.taskMsgId = PTP_TASK_MSG_QUIT;
   /* Send the message into the queue */
   status = zl303xx_PtpClockMsgQSend(pPtpClock, &msg, sizeof(msg));
   OS_TASK_DELAY(500);

   /* Delete the OS task associated with this clock. */
   if (OS_TASK_DELETE(pPtpClock->taskId) == OS_OK)
   {
      pPtpClock->taskId = OS_TASK_INVALID;
   }
   else
   {
      ZL303XX_TRACE_ERROR("   (ERROR) Clock task delete failed.", 0,0,0,0,0,0);
      status = ZL303XX_RTOS_TASK_DELETE_FAIL;
   }

   return status;
}

/* zl303xx_PtpClockAttachPort */
/** 

   Adds a port to a clock's portList.

  Parameters:
   [in]      pPtpClock Pointer to the clock that the port will be attached to.
   [in,out]  port      Pointer to the port to attach. This function modifies
                            the zl303xx_PortDataS::index member.

  Notes:   Assumes all input parameters are valid. Assumes the clock mutex has
        already been taken prior to calling this function.

*******************************************************************************/
zlStatusE zl303xx_PtpClockAttachPort(zl303xx_PtpClockS *pPtpClock, zl303xx_PortDataS *port)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   Uint32T n = 0;

   /* Lock the node mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpNodeMutexLock();
      status = mStatus;
   }

   /* requestedHandle must be a valid array index and not already in use, or -1 */
   if (status == ZL303XX_OK)
   {
      /* These checks are not done in zl303xx_PtpPortCreateStructCheck() because there
       * is a (very unlikely) race condition where two ports could be created with
       * the same requested handle. So, only do checks when holding the node mutex. */
      if (port->config.requestedHandle != (zl303xx_PtpPortHandleT)ZL303XX_PTP_INVALID)
      {
         n = port->config.requestedHandle;

         if (ZL303XX_CHECK_PORT_HANDLE(n) != ZL303XX_OK)
         {
            ZL303XX_TRACE_ERROR("   Clock (ERROR) requestedHandle=%u is out of range.",
                                n, 0,0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
         }
         else if (Zl303xx_PtpPortArray[n] != NULL)
         {
            ZL303XX_TRACE_ERROR("   Clock (ERROR) requestedHandle=%u is already in use.",
                                n, 0,0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
         }
      }
      /* requestedHandle is -1, so find an empty port array entry */
      else
      {
         for (n = 0; n < ZL303XX_PTP_NUM_PORTS_MAX; n++)
         {
            if (Zl303xx_PtpPortArray[n] == NULL)
               break;
         }

         if (n == ZL303XX_PTP_NUM_PORTS_MAX)
         {
            ZL303XX_TRACE_ERROR("   (ERROR) Maximum number of ports attached (%lu).",
                                ZL303XX_PTP_NUM_PORTS_MAX, 0,0,0,0,0);
            status = ZL303XX_TABLE_FULL;
         }
      }
   }

   if (status == ZL303XX_OK)
   {
      Zl303xx_PtpPortArray[n] = port;
      port->clockPortHandle = n;
   }

   /* Release the node mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpNodeMutexUnlock(status);
   }

   /* Add the port to the clock's portList. */
   if (status == ZL303XX_OK)
   {
      zl303xx_ListAdd(&pPtpClock->portList, &port->listEntry);
      pPtpClock->portCount++;

      /* Evaluate clock parameters. */
      zl303xx_PtpClockTypeEvaluate(pPtpClock);
      zl303xx_PtpClockNumberPortsEvaluate(pPtpClock);
   }

   return status;
}

/* zl303xx_PtpClockDetachPort */
/** 

   Removes a port from a clock's portList.

  Parameters:
   [in]      pPtpClock Pointer to the clock that the port will be detached from.
   [in,out]  port      Pointer to the port to detach. This function modifies
                            the zl303xx_PortDataS::index member.

  Notes:   Assumes all input parameters are valid. Assumes the clock mutex has
        already been taken prior to calling this function.

*******************************************************************************/
zlStatusE zl303xx_PtpClockDetachPort(zl303xx_PtpClockS *pPtpClock, zl303xx_PortDataS *port)
{
   zlStatusE status = ZL303XX_OK;

   /* Remove the port from the clock's portList. */
   if (status == ZL303XX_OK)
   {
      zl303xx_ListRemove(&port->listEntry);
      pPtpClock->portCount--;

      /* Evaluate clock parameters. */
      zl303xx_PtpClockTypeEvaluate(pPtpClock);
      zl303xx_PtpClockNumberPortsEvaluate(pPtpClock);

      /* Lock the node mutex. */
      status = zl303xx_PtpNodeMutexLock();
   }

   /* Remove the port from the global node array. */
   if (status == ZL303XX_OK)
   {
      Zl303xx_PtpPortArray[port->clockPortHandle] = NULL;
      status = zl303xx_PtpNodeMutexUnlock(status);
   }

   return status;
}

/* zl303xx_PtpClockParamSet */
/** 

   Function to set a clock parameter generically. No error checking is done on
   the data.

  Parameters:
   [in,out]  pClock  Clock data structure that will be modified.
   [in]      param   Clock parameter to set.
   [in]      data    Clock parameter data.

  Notes:  Assumes all input parameters are valid.

*******************************************************************************/
void zl303xx_PtpClockParamSet(zl303xx_PtpClockS *pClock,
                              zl303xx_PtpClockParamE param,
                              const void *data)
{
   /* Cast as a (char *) to do 1-byte pointer math. */
   char *dest = (char *)(void *)pClock + Zl303xx_PtpClockParam[param].offset;
   OS_MEMCPY(dest, data, Zl303xx_PtpClockParam[param].size);
}

/* zl303xx_PtpClockParamPack */
/** 

   Packs a clock parameter into a buffer.

  Parameters:
   [in]   param   Clock parameter type to pack.
   [in]   data    Data to pack.
   [out]  buffer  data is copied into this buffer.

  Return Value:  Uint32T  Number of bytes packed.

  Notes:  Assumes all input parameters are valid.

*******************************************************************************/
Uint32T zl303xx_PtpClockParamPack(zl303xx_PtpClockParamE param,
                                  const void *data,
                                  Uint8T *buffer)
{
   OS_MEMCPY(buffer, data, Zl303xx_PtpClockParam[param].size);
   return Zl303xx_PtpClockParam[param].size;
}

/*
  Function Name:
   zl303xx_PtpClockHasSlavePorts

  Details:
   Determines whether any of the ports attached to the clock are slaved to
   a remote clock

  Parameters:
   [in]  pPtpClock   Pointer to the PTP clock structure instance.

  Return Value:
   ZL303XX_TRUE if this device has at least one slave port

  Notes:
   Iterates through the clock's port list (and associated stream lists).
   If this function returns ZL303XX_FALSE, it does not necessarily mean the clock
   is a grandmaster since it may also have no ports in the master state as well.

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtpClockHasSlavePorts(zl303xx_PtpClockS *pPtpClock)
{
   /* Ignore the UNCALIBRATED state. This implies the stream is collecting
    * timing data but it is NOT being applied (i.e not selected as the active
    * server stream). Therefore, it is not considered a SLAVE stream. */
   return zl303xx_PtpClockHasStreamInState(pPtpClock, PTP_STREAM_SUBSTATE_SLAVE);
}

/*
  Function Name:
   zl303xx_PtpClockHasStreamInState

  Details:
   Determines whether any of the streams (including virtual ports) attached to
   the clock are in the specified state.

  Parameters:
   [in]  pPtpClock   Pointer to the PTP clock structure instance.

  Return Value:
   ZL303XX_TRUE     if this device has at least one slave port/stream in the
                     specified state.
   ZL303XX_FALSE    Otherwise.

  Notes:
   Iterates through the clock's port list (and associated stream lists).
   Assumes inputs are valid since this is an internal routine.

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtpClockHasStreamInState(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpStreamSubStateE streamState)
{
   /* Variables for looping through the clock's linked lists. */
   zl303xx_PortDataS *portData = NULL;
   zl303xx_ClockStreamS *streamData = NULL;

   /* LOOP through each port of the clock... */
   ZL303XX_PTP_FOR_EACH_PORT(&pPtpClock->portList, portData)
   {
      /* Check the Virtual PTP Port state. */
      if (portData->virtualData.isVirtualPort == ZL303XX_TRUE)
      {
         /* Stream State ENUM is not the same as the Port State ENUM so the
          * comparison has to be explicit here.     */
         if ((streamState == PTP_STREAM_SUBSTATE_SLAVE) &&
             (zl303xx_GetV2PortState(portData) ==ZL303XX_PORT_STATE_SLAVE))
         {
            return ZL303XX_TRUE;
         }
         else if ((streamState == PTP_STREAM_SUBSTATE_PASSIVE) &&
                  (zl303xx_GetV2PortState(portData) ==ZL303XX_PORT_STATE_PASSIVE))
         {
            return ZL303XX_TRUE;
         }
      }
      else /* LOOP through each stream of the port... */
      {
         ZL303XX_PTP_FOR_EACH_STREAM(&portData->streamList, streamData)
         {
            /* Check for the specified state. */
            if (streamData->streamSubstate == streamState)
            {
               return ZL303XX_TRUE;
            }
         }
      }
   }

   /* No stream on the clock is in the specified state. */
   return ZL303XX_FALSE;
}  /* END zl303xx_PtpClockHasStreamInState */

/*
  Function Name:
   zl303xx_PtpClockIsOwnMaster

  Details:
   Compares the Clock's defaultDS::clockIdentity against its own
   parentDS::grandmasterIdentity and returns TRUE / FALSE. This may not mean
   that the clock is a master as it may be a SLAVE-ONLY clock without a server.

  Parameters:
   [in]  pPtpClock   Pointer to the PTP clock structure instance.

  Return Value:
   ZL303XX_TRUE if this device is its own MASTER (according to the parentDS).

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtpClockIsOwnMaster(zl303xx_PtpClockS *pPtpClock)
{
   /* Compare DefaultDS::clockIdentiy and ParentDS::grandmasterIdentity */
   return zl303xx_PtpV2EqualClockIdentities(
                        pPtpClock->config.defaultDS.clockIdentity,
                        pPtpClock->parentDS.parentPortIdentity.clockIdentity);
}

/*
  Function Name:
   zl303xx_PtpClockHasSelectedVirtualPort

  Details:
   Compares the Clock's parentDS::parentPortIdentity against the portIdentity
   of all local ports to determine if the Clock has currently selected a Virtual
   PTP port.

  Parameters:
   [in]  pPtpClock   Pointer to the PTP clock structure instance.

   [out] pPtpPortx   Pointer to the PTP port structure selected if returned
                           value is TRUE; NULL otherwise.

  Return Value:
   ZL303XX_TRUE if the Clock is currently locked to a PRTC traceable Source.

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtpClockHasSelectedVirtualPort(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PortDataS **pPtpPortx)
{
   zl303xx_PortDataS *pPtpLoopPort = NULL;
   *pPtpPortx = NULL;

   ZL303XX_PTP_FOR_EACH_PORT(&pPtpClock->portList, pPtpLoopPort)
   {
      if ((pPtpLoopPort->virtualData.isVirtualPort == ZL303XX_TRUE) &&    /* A Virtual Port */
          (zl303xx_PtpV2EqualPortIdentities(                              /* Is the selected Port. */
                &pPtpLoopPort->portDS.portIdentity,
                &pPtpClock->gmSource.extComparisonData.receivingPortIdentity)))
      {
         *pPtpPortx = pPtpLoopPort;
         return ZL303XX_TRUE;
      }
   }

   return ZL303XX_FALSE;
}

/* zl303xx_PtpClockTypeEvaluate */
/** 

   Sets the ORDINARY or BOUNDARY bits of the clockType field based on the
   number of ports present on the clock.

  Parameters:
   [in,out]   pClock  Pointer to a clock structure. The clockType
                           field may be modified by this function.

  Notes:  Assumes that the clock mutex has already been taken.

*******************************************************************************/
void zl303xx_PtpClockTypeEvaluate(zl303xx_PtpClockS *pClock)
{
   if (pClock->config.clockType == 0)
   {
      if (pClock->portCount <= 1)
      {
         pClock->clockType |=  ZL303XX_PTP_CLOCK_TYPE_ORDINARY;
         pClock->clockType &= ~ZL303XX_PTP_CLOCK_TYPE_BOUNDARY;
      }
      else
      {
         pClock->clockType |=  ZL303XX_PTP_CLOCK_TYPE_BOUNDARY;
         pClock->clockType &= ~ZL303XX_PTP_CLOCK_TYPE_ORDINARY;
      }
   }
   else
   {
      pClock->clockType = pClock->config.clockType;
   }
}

/* zl303xx_PtpClockNumberPortsEvaluate */
/** 

   Determines the value of the numberPorts field based on the
   greatest value of portNumber on this clock.

  Parameters:
   [in]   pClock  Pointer to a clock structure. The defaultDS::numberPorts
                       field may be modified by this function.

*******************************************************************************/
void zl303xx_PtpClockNumberPortsEvaluate(zl303xx_PtpClockS *pClock)
{
   zl303xx_PortDataS *port;

   /* Configured value may be 0. */
   pClock->defaultDS.numberPorts = pClock->config.defaultDS.numberPorts;

   /* Get the greatest portNumber (if greater than the configured numberPorts). */
   ZL303XX_PTP_FOR_EACH_PORT(&pClock->portList, port)
   {
      if (port->config.portNumber > pClock->defaultDS.numberPorts)
      {
         pClock->defaultDS.numberPorts = port->config.portNumber;
      }
   }
}

/* zl303xx_PtpClockUncalibratedClassEvaluate */
/** 

   Determines the value of the egress clockClass value to export based the
   state of the clock (and state of local streams) and the clock configuration.

  Parameters:
   [in]  pPtpClock      Pointer to an existing clock.

   [out] exportedClass  Class to advertise in an egress ANNC message.


*******************************************************************************/
void zl303xx_PtpClockUncalibratedClassEvaluate(
      zl303xx_PtpClockS *pPtpClock,
      Uint8T *exportedClass)
{
   /* Pointer to a Virtual Port (may not be selected. */
   zl303xx_PortDataS *pPtpVirtualPort = NULL;

   /* Do a quick check to make sure the previousClass has been initialized. */
   if (pPtpClock->uncalibrated.previousClockClass == 0)
   {
      pPtpClock->uncalibrated.previousClockClass = pPtpClock->config.defaultDS.clockQuality.clockClass;
   }

   /* When there is a reduction in class, track it immediately regardless of
    * whether the functionality is ENABLED. (Include the >= so the == case does
    * not fall through to the next ELSE). */
   if (pPtpClock->parentDS.grandmasterClockQuality.clockClass >=
                                 pPtpClock->uncalibrated.previousClockClass)
   {
      pPtpClock->uncalibrated.previousClockClass = pPtpClock->parentDS.grandmasterClockQuality.clockClass;
      *exportedClass = pPtpClock->uncalibrated.previousClockClass;
   }

   /* If the Parent is better than the previous (<) then do the evaluation
    * as long as the functionality is enabled (otherwise) the 1588 default behavior prevails. */
   else if (pPtpClock->config.uncalibrated.usePreviousClass == ZL303XX_FALSE)
   {
      /* Do not update the previousClass in case the user enables the flag
       * while in UNCALIBRATED state. */
      *exportedClass = pPtpClock->parentDS.grandmasterClockQuality.clockClass;
   }

   /* Functionality is enabled so determine what to send based on Stream State. */
   else
   {
      /* It would make sense that if any LOCAL stream or Virtual Port is in
       * SLAVE mode that would be the source of the ParentDS.   */
      if (  /* Stream */
            ((pPtpClock->gmSource.pPtpStream != NULL) &&
             (pPtpClock->gmSource.pPtpStream->streamSubstate == PTP_STREAM_SUBSTATE_SLAVE)) ||
            /* Virtual Port */
            (zl303xx_PtpClockHasSelectedVirtualPort(pPtpClock, &pPtpVirtualPort) == ZL303XX_TRUE))
      {
         pPtpClock->uncalibrated.previousClockClass = pPtpClock->parentDS.grandmasterClockQuality.clockClass;
         *exportedClass = pPtpClock->parentDS.grandmasterClockQuality.clockClass;
      }

      else
      {
         *exportedClass = pPtpClock->uncalibrated.previousClockClass;
      }
   }
}  /* END zl303xx_PtpClockUncalibratedClassEvaluate */

/* zl303xx_PtpClockMutexCreate */
/** 

   Creates a mutex for a PTP clock.

  Parameters:
   [in,out]  pPtpClock  Pointer to a clock data structure. This function
                             modifies the zl303xx_PtpClockS::mutexId member.

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockMutexCreate(zl303xx_PtpClockS *pPtpClock)
{
   zlStatusE status = ZL303XX_OK;

   /* Create the mutex. */
   pPtpClock->mutexId = OS_MUTEX_CREATE();

   if (pPtpClock->mutexId == OS_MUTEX_INVALID)
   {
      ZL303XX_TRACE_ERROR("   (ERROR) Clock mutex create failed.", 0,0,0,0,0,0);
      status = ZL303XX_RTOS_MUTEX_CREATE_FAIL;
   }

   return status;
}

/* zl303xx_PtpClockMutexDelete */
/** 

   Deletes a mutex associated with a PTP clock.

  Parameters:
   [in,out]  pPtpClock  Pointer to a clock data structure. This function
                             modifies the zl303xx_PtpClockS::mutexId member.

  Notes:   Assumes all input parameters are valid. Assumes the clock mutex has
        already been taken.

*******************************************************************************/
zlStatusE zl303xx_PtpClockMutexDelete(zl303xx_PtpClockS *pPtpClock)
{
   zlStatusE status = ZL303XX_OK;

   /* Delete the mutex. */
   if (OS_MUTEX_DELETE(pPtpClock->mutexId) == OS_OK)
   {
      pPtpClock->mutexId = OS_MUTEX_INVALID;
   }
   else
   {
      ZL303XX_TRACE_ERROR("   (ERROR) Clock mutex delete failed.", 0,0,0,0,0,0);
      status = ZL303XX_RTOS_MUTEX_DELETE_FAIL;
   }

   return status;
}

/* zl303xx_PtpClockUtcGetInternal */
/** 

   Calls the hardware handler to get the current time then subtracts the
   UTC offset.

  Parameters:
   [in]   pClock     Pointer to a clock structure.
   [out]  pUtc       Clock's UTC.
   [out]  pUtcValid  Flag indicating if the UTC is valid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockUtcGetInternal(
      zl303xx_PtpClockS *pClock,
      zl303xx_TimeStamp *pUtc,
      zl303xx_BooleanE  *pUtcValid)
{
   zlStatusE status = ZL303XX_OK;

   /* Set default return values. */
   Uint64S utcOffset = {0, 0};
   zl303xx_BooleanE  utcOffsetValid = ZL303XX_FALSE;

   /* Get the hardware clock time. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockTimeGet(pClock, ZL303XX_PTP_INTERFACE_GENERAL, pUtc);
   }

   /* UTC = TAI - UTC_OFFSET */
   if (status == ZL303XX_OK)
   {
      if (pClock->timePropertiesDS.currentUtcOffset >= 0)
      {
         utcOffset.lo = pClock->timePropertiesDS.currentUtcOffset;
         pUtc->secondsField = Diff_U64S(pUtc->secondsField, utcOffset, NULL);
      }
      else
      {
         utcOffset.lo = -pClock->timePropertiesDS.currentUtcOffset;
         pUtc->secondsField = Add_U64S(pUtc->secondsField, utcOffset, NULL);
      }

      utcOffsetValid = pClock->timePropertiesDS.currentUtcOffsetValid;

      if (pUtcValid)
      {
         *pUtcValid = utcOffsetValid;
      }
   }

   return status;
}

/* zl303xx_PtpClockPathTraceListUpdate */
/** 

   Updates the target PATH-TRACE List with the list provided.
   If newCount == 0 or the new List == NULL then the current list is cleared.

  Parameters:
   [in]  pPtStruct   Pointer to the PATH-TRACE structure to update.
   [in]  newCount    Number of Clock Identities in the new list. If 0, then
                              the current list is deleted (if it existed).
   [in]  pNewPtList  Pointer to a new list of Clock Identity entries. Can
                           only be NULL if listCount is '0'.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   pPtList is NULL but listCount > 0.
     ZL303XX_RTOS_MEMORY_FAIL  Failed to allocate required memory.

  Notes:    Assumes other pointers are verified by the parent routine.
         Assumes any MUTEX control is handled by the parent routine.

*******************************************************************************/
zlStatusE zl303xx_PtpClockPathTraceListUpdate(
      zl303xx_PtpPathTraceListS *pPtStruct,
      Uint16T newCount,
      zl303xx_ClockIdentity *pNewPtList)
{
   zlStatusE status = ZL303XX_OK;

   /* Check input parameters. */
   /* A newCount of '0' or pNewPtList==NULL is OK (triggers a CLEAR).
    * However, if newCount > 0 then pNewPtList must be provided. */
   if (status == ZL303XX_OK)
   {
      if (newCount > 0)
      {
         status = ZL303XX_CHECK_POINTER(pNewPtList);
      }
   }

   /* The 2 most common cases are likely to be:
    * 1) The update will be with the same Path data that is already present. In
    *    that case, no action is required.
    * 2) The first time a packet is received, memory is allocated.   */

   /* Put an early check for case (1). It would have been covered below but
    * would re-copy every entry unnecessarily. */
   if (status == ZL303XX_OK)
   {
      if ((newCount == pPtStruct->listMax) &&
          (memcmp(pNewPtList, pPtStruct->pClockIdList,
                  newCount * sizeof(zl303xx_ClockIdentity)) == 0))
      {
         /* This is the same data so return. */
         return status;
      }
   }

   if (status == ZL303XX_OK)
   {
      /* First, determine if we need to FREE & RE_ALLOCATE memory.
       * - If memory was previously allocated
       * - AND either of the following is TRUE
       *   - If newCount == 0 then this is a List-Remove operation.
       *   - If the new list is bigger than the current allocated space then just
       *     FREE the old memory and ALLOCATE a new block. */
      if ((pPtStruct->pClockIdList != NULL) &&
          ((newCount == 0) || (newCount > pPtStruct->listMax)))
      {
         /* FREE the Allocated portion of the structure. Even if the current list
          * is NULL the FREE will pass, so do not check the list pointer. */
         OS_FREE(pPtStruct->pClockIdList);

         /* Update current List structure parameters. */
         pPtStruct->listMax = 0;
         pPtStruct->listLength = 0;
         pPtStruct->pClockIdList = NULL;     /* For good measure. */
      }

      /* Next, update the new list. */
      if (newCount > 0)
      {
         /* The possibility of pNewPtList being NULL was checked earlier so no
          * checks are required here.   */

         /* Allocate new memory for the table if required. */
         if (newCount > pPtStruct->listMax)
         {
            /* Allocate memory for the new list. */
            pPtStruct->pClockIdList = (zl303xx_ClockIdentity *)OS_CALLOC(newCount, sizeof(zl303xx_ClockIdentity));

            /* Verify all is well. */
            if (pPtStruct->pClockIdList == NULL)
            {
               ZL303XX_TRACE_ALWAYS(
                     "Path-Trace List Create: Memory allocation failed.",
                     0,0,0,0,0,0);

               status = ZL303XX_RTOS_MEMORY_FAIL;
            }

            /* Record the new maximum size. */
            else
            {
               pPtStruct->listMax = newCount;
            }
         }

         /* Copy the new items to the target list. */
         if ((status == ZL303XX_OK) &&
             (pPtStruct->pClockIdList != NULL))
         {
            Uint16T count = 0;
            for (; count < newCount; count++)
            {
               OS_MEMCPY(&pPtStruct->pClockIdList[count],
                      &pNewPtList[count],
                      sizeof(zl303xx_ClockIdentity));
            }

            /* Adjust the new list length. */
            pPtStruct->listLength = newCount;
         }
      }
   }

   return status;
}  /* END zl303xx_PtpClockPathTraceListUpdate */

/* zl303xx_PtpClockPathTraceListShow */
/** 

   Displays the target PATH-TRACE List.

  Parameters:
   [in]  pPtStruct   Pointer to a PATH-TRACE structure.

  Return Value:  ZL303XX_OK                Success.

  Notes:    Assumes the input pointer is verified by the parent routine.
         Assumes any MUTEX control is handled by the parent routine.

*******************************************************************************/
zlStatusE zl303xx_PtpClockPathTraceListShow(
      zl303xx_PtpPathTraceListS *pPtStruct)
{
   zlStatusE status = ZL303XX_OK;

   Uint16T count = 0;
   static char ptClockIdStr[64];

   if (status == ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS("*** PATH TRACE Data ********************", 0,0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("   Number of Entries: %d", pPtStruct->listLength, 0,0,0,0,0);

      for (; count < pPtStruct->listLength; count++)
      {
         (void)zl303xx_PtpV2ClockIdentityToString(pPtStruct->pClockIdList[count], ptClockIdStr);
         ZL303XX_TRACE_ALWAYS("     Entry[%d] : %s", count, ptClockIdStr, 0,0,0,0);
      }
      ZL303XX_TRACE_ALWAYS("****************************************", 0,0,0,0,0,0);
   }

   return status;
}  /* END zl303xx_PtpClockPathTraceListShow */

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/* zl303xx_PtpClockConfigApply */
/** 

   Configures internal clock parameters based on a user config structure.

  Parameters:
   [in]   pClock   Pointer to an existing clock.
   [in]   pConfig  Config structure to apply.

*******************************************************************************/
static void zl303xx_PtpClockConfigApply(zl303xx_PtpClockS *pClock,
                                        zl303xx_PtpClockCreateS *pConfig)
{
   /* Save the user's configuration. */
   pClock->config = *pConfig;

   /* Set the clock time traceability properties. */
   /* These configured hardware time properties remain a part of the clock
    * CONFIG field and will be used whenever the clock becomes a grandmaster.
    * For now, copy them to the clock's timePropertiesDS since we are
    * initializing and know for sure that there are no other references
    * available (from the packet network).     */
   (void)zl303xx_PtpClockTimePropertiesApply(pClock, &pConfig->localTimeProperties);

   pClock->defaultDS = pConfig->defaultDS;
   pClock->clockType = pConfig->clockType;

   /* Clear the Current data set */
   OS_MEMSET(&pClock->currentDSInt, 0, sizeof(pClock->currentDSInt));

   /* Clear the Parent data set. (This implementation does not support parent
    * statistics so set the values as per IEEE-1588-2008: Section 8.2.3.4  */
   pClock->parentDS.parentStats = ZL303XX_FALSE;
   pClock->parentDS.observedParentOffsetScaledLogVariance = 0xFFFF;
   pClock->parentDS.observedParentClockPhaseChangeRate = 0x7FFFFFFF;

   /* Set the appropriate Profile data. */
   /* Search the Profile Definitions for a Binding (Default is set internally otherwise). */
   (void)zl303xx_PtpProfileInfoGet(pClock->config.profile, &pClock->profile);

   /* A seed used to generate pseudo-random intervals for delay request messages */
   pClock->random_seed = (Uint32T)0x78195041;
   
   /* Set individual bits of BMCA flags variable. It is the default behavior */
   pClock->bmcaFlags = (Uint32T) 0x3F;

   /* Reset BMCA parameters so that an update will automatically occur. */
   /* Kick the clock's BMCA state machine by assigning the DefaultDS as the
    * ParentDS initially. */
   {
      /* Update the clock's dynamic parameters. */
      pClock->gmSource.location = ZL303XX_PTP_GM_SRC_LOCAL_CLOCK;
      pClock->gmSource.pPtpStream = NULL;
      pClock->gmSource.pPtpPort = NULL;

      /* Set the ParentDS based on the Default. */
      zl303xx_PtpConvertDefaultDsToParentDs(&pClock->config.defaultDS, &pClock->parentDS);

      /* Copy the clock default data set to the 'external' BMC comparison
       * structure to initialize the data during creation. */
      (void)zl303xx_PtpClockCompareDataGet(pClock, &pClock->gmSource.extComparisonData);
   }

   /* Port list. */
   zl303xx_ListInit(&pClock->portList);
   pClock->portCount = 0;

   /* Clock-level contract limits */
   pClock->contractLimits[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].maxCount = pConfig->unicastNegotiation.maxAnnounceCount;
   pClock->contractLimits[PTP_NEGOTIATED_MSG_ID_SYNC].maxCount = pConfig->unicastNegotiation.maxSyncCount;
   pClock->contractLimits[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].maxCount = pConfig->unicastNegotiation.maxDelayRespCount;
   pClock->contractLimits[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].maxCount = pConfig->unicastNegotiation.maxPdelayRespCount;

   /* Alternate Time Offset Setup. */
   {
      Uint16T tblIndex;
      for(tblIndex = 0; tblIndex < ZL303XX_PTP_ALT_TIME_OFFSET_TBL_ENTRIES; tblIndex++)
      {
         pClock->altTimeOffset.entryTbl[tblIndex].keyField = 255;
      }
   }

}

/* zl303xx_PtpClockConfigRevert */
/** 

   Reverts parameters of a new config structure to the current value if they
   cannot be changed.

  Parameters:
   [in]      pCurrCfg  Current clock config.
   [in,out]  pNewCfg   New config to check and revert.

*******************************************************************************/
static void zl303xx_PtpClockConfigRevert(zl303xx_PtpClockS *pClock,
                                         zl303xx_PtpClockCreateS *pNewCfg)
{
   zl303xx_PtpClockCreateS *pCurrCfg = &pClock->config;

   /* If any of the following parameters are different, revert the new config
    * structure to the existing value. The parameters are listed in the comment
    * block for zl303xx_PtpClockConfigSet(). */
   if (strcmp(pCurrCfg->taskName, pNewCfg->taskName) != 0)
   {
      ZL303XX_TRACE_ALWAYS("   taskName change denied: %s -> %s",
         pCurrCfg->taskName, pNewCfg->taskName, 0,0,0,0);

      OS_MEMSET(pNewCfg->taskName, 0, sizeof(pNewCfg->taskName));
      strncpy(pNewCfg->taskName, pCurrCfg->taskName, sizeof(pNewCfg->taskName) - 1);
   }
   if (pCurrCfg->taskPriority != pNewCfg->taskPriority)
   {
      ZL303XX_TRACE_ALWAYS("   taskPriority change denied: %ld -> %ld",
         pCurrCfg->taskPriority, pNewCfg->taskPriority, 0,0,0,0);
      pNewCfg->taskPriority = pCurrCfg->taskPriority;
   }
   if (pCurrCfg->taskStackSize != pNewCfg->taskStackSize)
   {
      ZL303XX_TRACE_ALWAYS("   taskStackSize change denied: %ld -> %ld",
         pCurrCfg->taskStackSize, pNewCfg->taskStackSize, 0,0,0,0);
      pNewCfg->taskStackSize = pCurrCfg->taskStackSize;
   }
   if (pCurrCfg->msgQLength != pNewCfg->msgQLength)
   {
      ZL303XX_TRACE_ALWAYS("   msgQLength change denied: %ld -> %ld",
         pCurrCfg->msgQLength, pNewCfg->msgQLength, 0,0,0,0);
      pNewCfg->msgQLength = pCurrCfg->msgQLength;
   }
   if (memcmp(pCurrCfg->defaultDS.clockIdentity,
              pNewCfg->defaultDS.clockIdentity,
              PTP_V2_CLK_IDENTITY_LENGTH) != 0)
   {
      ZL303XX_TRACE_ALWAYS("   clockIdentity change denied", 0,0,0,0,0,0);
      OS_MEMCPY(pNewCfg->defaultDS.clockIdentity, pCurrCfg->defaultDS.clockIdentity,
             PTP_V2_CLK_IDENTITY_LENGTH);
   }
   if (pCurrCfg->defaultDS.numberPorts != pNewCfg->defaultDS.numberPorts)
   {
      ZL303XX_TRACE_ALWAYS("   numberPorts change denied: %u -> %u",
         pCurrCfg->defaultDS.numberPorts, pNewCfg->defaultDS.numberPorts, 0,0,0,0);
      pNewCfg->defaultDS.numberPorts = pCurrCfg->defaultDS.numberPorts;
   }
   if (pNewCfg->requestedHandle != (zl303xx_PtpStreamHandleT)ZL303XX_PTP_INVALID &&
       pCurrCfg->requestedHandle != pClock->clockHandle)
   {
      ZL303XX_TRACE_ALWAYS("   requestedHandle change denied: %u -> %u",
         pClock->clockHandle, pNewCfg->requestedHandle, 0,0,0,0);
      pNewCfg->requestedHandle = pCurrCfg->requestedHandle;
   }
}

/* zl303xx_PtpClockTimePropertiesApply */
/** 

   Applies the new Time Properties values to an existing clock. Determines if
   any Event Notifications need to be issued as a result of the change.

  Parameters:
   [in]  pPtpClock            Pointer to an existing clock.
   [in]  newTimeProperties    New Time Properties to apply.

*******************************************************************************/
static void zl303xx_PtpClockTimePropertiesApply(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_TimePropertiesDS *newTimeProperties)
{
   /* Keep a copy of the old Time Properties. */
   zl303xx_TimePropertiesDS oldTimeProperties = pPtpClock->timePropertiesDS;

   /* Set the new values. */
   pPtpClock->timePropertiesDS = *newTimeProperties;

   /* Determine if any Events should be generated. */

   /* ZL303XX_PTP_EVENT_LEAP_SECONDS_FLAG_CHANGE */
   if ((newTimeProperties->leap59 != oldTimeProperties.leap59) ||
       (newTimeProperties->leap61 != oldTimeProperties.leap61))
   {
      (void)zl303xx_PtpEventLeapSecondsFlagChange(pPtpClock, &oldTimeProperties);
   }

   /* ZL303XX_PTP_EVENT_LEAP_SECONDS_FLAG_CHANGE */
   if ((newTimeProperties->currentUtcOffset != oldTimeProperties.currentUtcOffset) ||
       (newTimeProperties->currentUtcOffsetValid != oldTimeProperties.currentUtcOffsetValid))
   {
      (void)zl303xx_PtpEventUtcOffsetChange(pPtpClock, &oldTimeProperties);
   }
}

/* zl303xx_PtpClockDeleteInternal */
/**
   Internal function to delete a Clock and its associated OS task(s).

  Parameters:
   [in]  pPtpClock      Pointer to the clock data structure of the clock
                              to delete.
   [in]  pClockDelete   Optional parameters to shut down the clock (may be
                             NULL, so check before using).

  Notes:   Assumes all input parameters are valid. Assumes the clock mutex has
        already been taken.

*******************************************************************************/
zlStatusE zl303xx_PtpClockDeleteInternal(zl303xx_PtpClockS *pPtpClock,
                                         zl303xx_PtpClockDeleteS *pClockDelete)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T portHandle;
   zl303xx_PortDataS *pPtpPort = NULL;
   zl303xx_PtpEventPortDeleteS eventData;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpClockDeleteInternal:", 0,0,0,0,0,0);

   /* If clock deletion is not forced, it can only be deleted if no ports are attached. */
   if ((pClockDelete != NULL) &&
       (pClockDelete->forceDelete == ZL303XX_FALSE) &&
       (pPtpClock->portCount > 0))
   {
      ZL303XX_TRACE_ALWAYS("  (ERROR) Cannot delete clock %lu: forceDelete=FALSE and %u port(s) still attached.",
                           pPtpClock->clockHandle, pPtpClock->portCount, 0,0,0,0);
      status = ZL303XX_INVALID_OPERATION;
   }

   /* Delete any ports attached to this clock. */
   if (status == ZL303XX_OK)
   {
      /* We could follow the linked list of ports attached to the clock but
       * since this is the only time all ports are deleted, the inefficiency
       * of looping the entire port array is a safe and acceptable approach. */
      for (portHandle = 0; portHandle < ZL303XX_PTP_NUM_PORTS_MAX; portHandle++)
      {
         pPtpPort = Zl303xx_PtpPortArray[portHandle];
         if ((pPtpPort != NULL) &&
             (pPtpPort->clock == pPtpClock))
         {
            /* Gather data for the port delete event. */
            if (pPtpPort->clock->config.eventCallback != NULL)
            {
               eventData.portHandle  = pPtpPort->clockPortHandle;
               eventData.portExtData = pPtpPort->config.extData;
               eventData.portAddr    = pPtpPort->config.localAddr;
            }

            /* Delete the port and all its sub-components. */
            status = zl303xx_PtpPortDeleteInternal(pPtpPort, NULL);

            /* Fire the event handler because this is an automatic port delete. */
            if (status == ZL303XX_OK)
            {
               if (pPtpClock->config.eventCallback != NULL)
               {
                  pPtpClock->config.eventCallback(ZL303XX_PTP_EVENT_PORT_DELETE,
                                                  &eventData);
               }
            }
            else
            {
               ZL303XX_TRACE_ALWAYS("  (ERROR): removing port (%d) on clock (%d)",
                     portHandle, pPtpClock->clockHandle,0,0,0,0);
               break;
            }
         }
      }
   }

   /* Stop all timers associated with this clock, so they get removed from the
    * list of active timers. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpTimerStop(&pPtpClock->profileDS.telecom.qlHoldOffTimer);
      zl303xx_PtpTimerStop(&pPtpClock->bmca.updateTimer);
   }

   /* Stop the protocol task. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockTaskStop(pPtpClock);
   }

   /* Delete the message queue for this clock. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMsgQDelete(pPtpClock);
   }

   /* Delete the mutex for this clock. */
   /* The only routine to currently call this routine is zl303xx_PtpClockDelete.
    * It has already taken the MUTEX.*/
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexDelete(pPtpClock);
   }

   /* Remove this clock from the node's clockList. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpNodeDetachClock(pPtpClock);
   }

   /* Free the memory used by this clock structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockStructFree(&pPtpClock);
   }

   /* Give the task some time to delete the clock. A race condition can occur
    * when deleting a Clock prior to PtpShutdown (it is recommended to run just
    * PtpShutdown and allow it to do the clock deletions).*/
   if (status == ZL303XX_OK)
   {
      OS_TASK_DELAY(250);
   }

   return status;
}  /* END zl303xx_PtpClockDeleteInternal */

/* zl303xx_PtpClockCreateStructCheck */
/** 

   Verifies that all of the members of a zl303xx_PtpClockCreateS have been
   set correctly.

  Parameters:
   [in]   clockCreate  Structure to verify.

*******************************************************************************/
static zlStatusE zl303xx_PtpClockCreateStructCheck(zl303xx_PtpClockCreateS *clockCreate)
{
   zlStatusE status = ZL303XX_OK;

   /* Check task related parameters. */
   if (status == ZL303XX_OK)
   {
      if (OS_STRLEN(clockCreate->taskName) == 0)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) taskName is zero-length.", 0,0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }

      /* Don't check taskPriority. */

      /* Check taskStackSize. */
      if (clockCreate->taskStackSize == 0)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) taskStackSize must be non-zero.", 0,0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }

      /* Check msgQLength. */
      if (clockCreate->msgQLength == 0)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) msgQLength must be non-zero.", 0,0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Check profile related parameters. */
   if (status == ZL303XX_OK)
   {
      if (clockCreate->profile >= ZL303XX_PTP_PROFILE_NUM_TYPES)
      {
         ZL303XX_TRACE_ALWAYS(
            "   (WARNING) profile=%u is INVALID. Defaulting to %u.",
            clockCreate->profile, ZL303XX_PTP_PROFILE_DEFAULT, 0,0,0,0);
         clockCreate->profile = ZL303XX_PTP_PROFILE_DEFAULT;
      }

      status = ZL303XX_CHECK_BOOLEAN(clockCreate->profileStrict);
   }

   /* Check the Default Data Set configuration. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockCheckDefaultDS(clockCreate, &clockCreate->defaultDS);
   }



   /* Boolean parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(clockCreate->bmca.revertiveEn) |
               ZL303XX_CHECK_BOOLEAN(clockCreate->telecom.evtSquelchEn);
   }

   /* Hardware command binding. */
   if (status == ZL303XX_OK)
   {
      if (clockCreate->hwCmdFn == NULL)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) hwCmdFn must have a function binding.", 0,0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Warn if QL hold off is being used with the squelch event. */
   if (status == ZL303XX_OK)
   {
      if ((clockCreate->telecom.qlHoldOffSec > 0) &&
          (clockCreate->telecom.evtSquelchEn))
      {
         ZL303XX_TRACE_ALWAYS("   (WARNING) QL hold off and SQUELCH event are both enabled.",
                              0,0,0,0,0,0);
      }
   }

   return status;
}

/* zl303xx_PtpClockDeleteStructCheck */
/** 

   Verifies that all of the members of a zl303xx_PtpClockDeleteS have been
   set correctly.

  Parameters:
   [in]   pDelete  Structure to verify.

*******************************************************************************/
static zlStatusE zl303xx_PtpClockDeleteStructCheck(zl303xx_PtpClockDeleteS *pDelete)
{
   zlStatusE status = ZL303XX_OK;

   /* A NULL pointer may have been passed into zl303xx_PtpClockDelete(). This is
    * fine, and is handled inside zl303xx_PtpClockDeleteInternal(). */
   if (pDelete != NULL)
   {
      /* Check boolean parameters. */
      status = ZL303XX_CHECK_BOOLEAN(pDelete->forceDelete);
   }

   return status;
}

/* zl303xx_PtpClockCheckDefaultDS */
/** 

   Verifies that all of the members of a zl303xx_DefaultDS structure have been
   set correctly.

  Parameters:
   [in]   pConfig      Structure used to create the clock.
   [in]   pDefaultDS   Pointer to the zl303xx_DefaultDS structure to verify.

  Notes:
   Assumes the pDefaultDS pointer has already been checked for this internal
   routine.

*******************************************************************************/
static zlStatusE zl303xx_PtpClockCheckDefaultDS(zl303xx_PtpClockCreateS *pConfig,
                                                zl303xx_DefaultDS *pDefaultDS)
{
   zlStatusE status = ZL303XX_OK;

   /* Verify Boolean values. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(pDefaultDS->twoStepFlag) |
               ZL303XX_CHECK_BOOLEAN(pDefaultDS->slaveOnly);
   }

   /* The domain number and range is defined by the Profile. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpProfileDomainNumberCheck(pConfig->profile,
                           pConfig->profileStrict, &pDefaultDS->domainNumber);
   }

   /* The Priority values may be limited by the Profile. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpProfileClockPriorityCheck(
                           pConfig->profile,
                           pConfig->profileStrict,
                           &pDefaultDS->priority1, &pDefaultDS->priority2,
                           pDefaultDS->slaveOnly);
   }

   /* The number of port is limited by the MAX port allowed. */
   if (status == ZL303XX_OK)
   {
      if (pDefaultDS->numberPorts > ZL303XX_PTP_NUM_PORTS_MAX)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) numberPorts (%d) greater than MAX (%d)",
               pDefaultDS->numberPorts,
               ZL303XX_PTP_NUM_PORTS_MAX, 0,0,0,0);

         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Check clockIdentity. */
   if (status == ZL303XX_OK)
   {
      if (zl303xx_PtpV2ClockIdentityCheck(pDefaultDS->clockIdentity) != ZL303XX_OK)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) clockIdentity is invalid (all 0's or all 1's).",
                             0,0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Check clockClass vs. slaveOnly Configuration. */
   if (status == ZL303XX_OK)
   {
      /* Slave_Only clocks must be configured with a clockClass of 255 */
      if ((pDefaultDS->slaveOnly == ZL303XX_TRUE) &&
          (pDefaultDS->clockQuality.clockClass != PTP_SLAVE_ONLY_CLOCK_CLASS))
      {
         ZL303XX_TRACE_ALWAYS("   Forcing clockClass=%u for slaveOnly=TRUE.",
               PTP_SLAVE_ONLY_CLOCK_CLASS, 0,0,0,0,0);
         pDefaultDS->clockQuality.clockClass = PTP_SLAVE_ONLY_CLOCK_CLASS;
      }

      else if ((pDefaultDS->slaveOnly == ZL303XX_FALSE) &&
               (pDefaultDS->clockQuality.clockClass == PTP_SLAVE_ONLY_CLOCK_CLASS))
      {
         ZL303XX_TRACE_ALWAYS("   Forcing slaveOnly=TRUE for clockClass=%u.",
               PTP_SLAVE_ONLY_CLOCK_CLASS, 0,0,0,0,0);
         pDefaultDS->slaveOnly = ZL303XX_TRUE;
      }

      /* The Clock Class (or other quality parameters) may be limited by the Profile. */
      status = zl303xx_PtpProfileClockQualityCheck(
                           pConfig->profile,
                           pConfig->profileStrict,
                           &pDefaultDS->clockQuality);
   }

   /* The Priority values may be limited by the Profile. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpProfileClockPriorityCheck(
                           pConfig->profile,
                           pConfig->profileStrict,
                           &pDefaultDS->priority1, &pDefaultDS->priority2,
                           pDefaultDS->slaveOnly);
   }

   return status;
}  /* END zl303xx_PtpClockCheckDefaultDS */

/* zl303xx_PtpClockCheckParentDS */
/** 

   Verifies that all of the members of a zl303xx_ParentDS structure have been set
   correctly. Also, does a sanity against the Clock defaultDs to verify that the
   parent being set is actually better than the local clock.

  Parameters:
   [in]  pPtpClock   Pointer to the Clock for which the ParentDS is being set.
   [in]  pParentDS   Pointer to the zl303xx_ParentDS structure to verify.

  Notes:
   Assumes the pParentDS pointer has already been checked for this internal
   routine.

*******************************************************************************/
zlStatusE zl303xx_PtpClockCheckParentDS(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_ParentDS *pParentDS)
{
   zlStatusE status = ZL303XX_OK;

   /* Verify Boolean values. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(pParentDS->parentStats);
   }

   /* The restrictions on the parentPortIdentity member will be:
    * - clockIdentity:
    *   - cannot be the All-Clock Identity.
    *   - cannot be All-Zeros.
    * - portNumber:
    *   - cannot be All-Ports (all 1's).
    *   - cannot be 0 unless the corresponding clockIdentity is the local clock.  */
   if (status == ZL303XX_OK)
   {
      /* clockIdentity */
      /*---------------*/
      status = zl303xx_PtpV2ClockIdentityCheck(pParentDS->parentPortIdentity.clockIdentity);
   }

   if (status == ZL303XX_OK)
   {
      /* portNumber */
      /*------------*/
      if (pParentDS->parentPortIdentity.portNumber == 0xFFFF)
      {
         ZL303XX_TRACE_ERROR("CheckParentDS:  parentPortIdentity.port ALL-ONES", 0,0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
      else if (pParentDS->parentPortIdentity.portNumber == 0)
      {
         /* This is OK if the associated clockIdentity is the local clock.
          * (This means the DefaultDS is being restored to the ParentDS). */
         if (zl303xx_PtpV2EqualClockIdentities(pParentDS->parentPortIdentity.clockIdentity,
                                               pPtpClock->config.defaultDS.clockIdentity) == ZL303XX_FALSE)
         {
            ZL303XX_TRACE_ERROR("CheckParentDS:  parentPortIdentity.port == 0", 0,0,0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
         }
      }
   }

   /* The restrictions on the grandmasterIdentity member will be:
    *   - cannot be the All-Clock Identity.
    *   - cannot be All-Zeros. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2ClockIdentityCheck(pParentDS->grandmasterIdentity);
   }

   /* The following members do not require checking:
    * - observedParentOffsetScaledLogVariance
    * - observedParentClockPhaseChangeRate
    * - grandmasterPriority1/2 (unless the Profile limits it)
    * - members of grandmasterClockQuality except clockClass    */

   /* The following Sanity will verify some of the new ParentDS values against
    * the local DefaultDS. Logging is performed but NO ERROR is returned.   */
   if (status == ZL303XX_OK)
   {
      /* Check the ParentDS priority1 against the DefaultDS priority1. */
      if (pParentDS->grandmasterPriority1 > pPtpClock->config.defaultDS.priority1)
      {
         ZL303XX_TRACE_ERROR("CheckParentDS:  grandmasterPriority1 (%d) > local priority (%d)",
               pParentDS->grandmasterPriority1,
               pPtpClock->config.defaultDS.priority1, 0,0,0,0);
      }

      /* If priority1 values are equal, compare clockClasses. */
      else if (pParentDS->grandmasterPriority1 == pPtpClock->config.defaultDS.priority1)
      {
         if (pParentDS->grandmasterClockQuality.clockClass > pPtpClock->config.defaultDS.clockQuality.clockClass)
         {
            ZL303XX_TRACE_ERROR("CheckParentDS:  grandmaster clockClass (%d) > local clockClass (%d)",
                  pParentDS->grandmasterClockQuality.clockClass,
                  pPtpClock->config.defaultDS.clockQuality.clockClass, 0,0,0,0);
         }
      }
   }

   return status;
}  /* END zl303xx_PtpClockCheckParentDS */

/* zl303xx_PtpConvertDefaultDsToParentDs */
/** 

   Given a DefaultDS (standard format), converts the fields to an equivalent
   ParentDS (standard format).

  Parameters:
   [in]   pDefaultDS  Pointer to a defaultDS in standard format.
   [out]  pParentDS   Pointer to the equivalent ParentDS.

  Return Value:  void

  Notes:    If the DefaultDS is used as the ParentDS, then the calling routine
         must set the pPtpClock::gmSource::pPtpStream member to NULL.
*******************************************************************************/
void zl303xx_PtpConvertDefaultDsToParentDs(zl303xx_DefaultDS *pDefaultDS,
                                           zl303xx_ParentDS *pParentDS)
{
   /* Clear the structure to begin. */
   OS_MEMSET(pParentDS, 0, sizeof(*pParentDS));

   /* The parentPortIdentity is the local clockIdentity. */
   OS_MEMCPY(pParentDS->parentPortIdentity.clockIdentity,
          pDefaultDS->clockIdentity,
          PTP_V2_CLK_IDENTITY_LENGTH);
   pParentDS->parentPortIdentity.portNumber = 0;

   /* Parent Statistics are INVALID; values were cleared by the OS_MEMSET. */
   /* This implementation does not support parent statistics so set the
    * values as per IEEE-1588-2008: Section 8.2.3.4. */
   pParentDS->parentStats = ZL303XX_FALSE;
   pParentDS->observedParentOffsetScaledLogVariance = 0xFFFF;
   pParentDS->observedParentClockPhaseChangeRate = 0x7FFFFFFF;

   /* Grandmaster values come from the local Clock As well. */
   OS_MEMCPY(&pParentDS->grandmasterIdentity,
          &pDefaultDS->clockIdentity,
          PTP_V2_CLK_IDENTITY_LENGTH);
   pParentDS->grandmasterClockQuality = pDefaultDS->clockQuality;
   pParentDS->grandmasterPriority1 = pDefaultDS->priority1;
   pParentDS->grandmasterPriority2 = pDefaultDS->priority2;
}  /* END zl303xx_PtpConvertDefaultDsToParentDs */

/* zl303xx_PtpClockConfigureDefaultDataSet */
/** 

   Configures the Default Data Set parameters of the specified clock. These
   parameters are used by the PTP engine to determine whether this clock is a
   server or client. This routine is the base for all API calls related to
   setting any of the Default Data Set parameters.

  Parameters:
   [in]  pPtpClock    Pointer to the target clock's data structure.
   [in]  pDefaultDS   Pointer to the Default Data Set to configure.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     One of the parameters of the pDefaultDS is
                                       not valid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockConfigureDefaultDataSet(zl303xx_PtpClockS *pPtpClock,
                                                  zl303xx_DefaultDS *pDefaultDS,
                                                  zl303xx_BooleanE lockMutex)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;

   /* Check the input pointers. */
   if ((pPtpClock == NULL) || (pDefaultDS == NULL))
   {
      status = ZL303XX_INVALID_POINTER;
   }

   /* Check the new configuration for potential errors.  */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockCheckDefaultDS(&pPtpClock->config, pDefaultDS);
   }

   /* Lock the clock mutex, if necessary. */
   if (status == ZL303XX_OK)
   {
      if (lockMutex)
      {
         mStatus = zl303xx_PtpClockMutexLock(pPtpClock);
         status = mStatus;
      }
   }

   /* If we have the MUTEX, update the data. */
   if (status == ZL303XX_OK)
   {
      /* If the domain number changed, disqualify every entry in all foreign
       * master tables. */
      if (pPtpClock->config.defaultDS.domainNumber != pDefaultDS->domainNumber)
      {
         zl303xx_PtpFmtDisqualifyAll(pPtpClock);
      }

      /* Update the values in the Clock configuration and propagate them to
       * the clock's Active default data set. */
      pPtpClock->config.defaultDS = *pDefaultDS;
      pPtpClock->defaultDS = pPtpClock->config.defaultDS;

      /* If this clock previously selected itself it was because it was the best
       * at the time. However, if we have reconfigured the DefaultDS then it is
       * possible a better GM now exists. So, don't automatically advertise the
       * new config yet; let the BMCA select it if appropriate. */
      /* Any time the Clock configuration is changed, the Clock State-Decision
       * algorithm should be run to re-evaluate its state (and that of any
       * associated ports & streams).   */
      /* Resend the Foreign Master Table on a config change. */
      zl303xx_PtpClockBmcaUpdateInitiate(pPtpClock);
   }

   /* Release the clock mutex, if necessary. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* END zl303xx_PtpClockConfigureDefaultDataSet */

/* zl303xx_PtpClockConfigureParentDataSet */
/** 

   Configures the Parent Data Set parameters of the specified clock. These
   parameters are used by the PTP engine to determine whether this clock is a
   server or client.

  Parameters:
   [in]  pPtpClock   Pointer to the target clock's data structure.
   [in]  pParentDS   The ParentDS to apply to the clock. If NULL, the the
                           DefaultDS of the clock is used as the ParentDS.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       The pPtpClock pointer is NULL.
     ZL303XX_PARAMETER_INVALID     One of the members of the pParentDS is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Unable to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Unable to release mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockConfigureParentDataSet(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_ParentDS *pParentDS,
      zl303xx_BooleanE lockMutex)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;

   /* For Logging. */
   const char *fnName = "zl303xx_PtpClockConfigureParentDataSet";

   /* Check the input pointers. */
   if (pPtpClock == NULL)
   {
      status = ZL303XX_INVALID_POINTER;
   }

   /* Lock the clock mutex, if necessary. */
   if (status == ZL303XX_OK)
   {
      if (lockMutex)
      {
         mStatus = zl303xx_PtpClockMutexLock(pPtpClock);
         status = mStatus;
      }
   }

   /* If we have the MUTEX, update the data. */
   if (status == ZL303XX_OK)
   {
      /* Copy the required ParentDS values to the Clock structure. */
      pPtpClock->parentDS = *pParentDS;

      /* Notify the management system of the change. */
      zl303xx_PtpEventParentDsChange(pPtpClock);

      /* Using the DefaultDS & ParentDS, evaluate the required state of each
       * Stream (and update the pPtpClock->gmSource.pPtpStream member).  */
      if (pPtpClock->config.autoUpdateStreamStates == ZL303XX_TRUE)
      {
         status = zl303xx_PtpClockStateAudit(pPtpClock);
      }
   }

   /* Release the clock mutex, if necessary. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   /* Log the result of the Audit. */
   if (status == ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 3,
            "%s: Clock Class (%d); ParentDS Source (%d); Server Stream (%d)",
            fnName,
            pPtpClock->config.defaultDS.clockQuality.clockClass,
            pPtpClock->gmSource.location,
            (((pPtpClock->gmSource.pPtpStream)
                  ?  pPtpClock->gmSource.pPtpStream->clockStreamHandle
                  : (zl303xx_PtpStreamHandleT)ZL303XX_PTP_INVALID)), 0,0);
   }

   return status;
}  /* END zl303xx_PtpClockConfigureParentDataSet */

/* zl303xx_PtpPortFindStreamForParentDS */
/** 

   Searches through a PTP Clock's streams until it finds the one associated with
   the ParentDS provided.

  Parameters:
   [in]  pPtpPort    Pointer to the PTP Port to evaluate the streams for.
   [in]  pParentDS   Pointer to the ParentDS to compare each stream with.

  Return Value:
   NULL if no stream is associated with the specified ParentDS.
   The pointer to the Stream associated with the ParentDS.

  Notes:
   Assumes all pointers are valid for this local routine.

 *****************************************************************************/
zl303xx_ClockStreamS *zl303xx_PtpPortFindStreamForParentDS(
      zl303xx_PortDataS *pPtpPort,
      zl303xx_ParentDS *pParentDS)
{
   /* Table index counter. */
   Uint16T tblIndex;

   /* Create a shortcut to the FMT of the Clock. */
   zl303xx_ForeignMasterDS *fmTbl = &pPtpPort->clock->foreignMasterDS;

   /* Loop through the Port FMT for the specified Foreign Master Record. */
   /* If fmTbl->numActiveRecords == 0) then this loop will not execute. */
   for(tblIndex = 0; tblIndex < fmTbl->numActiveRecords; tblIndex++)
   {
      /* Check if this Entry is associated with the parentDS provided.
       * The key fields are:
       * - parentPortIdentity of the parentDS == foreignPortIdentity of the FMT entry
       * - grandmasterIdentity of the parentDS == grandmasterIdentity of the FMT entry
       * - PortIdentity of pPtpPort == receivingPortIdentity of the FMT entry */
      if ((zl303xx_PtpV2EqualClockIdentities(
                     fmTbl->foreign[tblIndex].compareData.grandmasterIdentity,
                     pParentDS->grandmasterIdentity) == ZL303XX_TRUE) &&
          (zl303xx_PtpV2EqualPortIdentities(
                     &fmTbl->foreign[tblIndex].compareData.foreignPortIdentity,
                     &pParentDS->parentPortIdentity) == ZL303XX_TRUE) &&
          (zl303xx_PtpV2EqualPortIdentities(
                     &fmTbl->foreign[tblIndex].compareData.receivingPortIdentity,
                     &pPtpPort->portDS.portIdentity) == ZL303XX_TRUE))
      {
         /* This FMT entry matches the ParentDS data so return the stream.
          * (Returns only the first matching entry).*/
         return fmTbl->foreign[tblIndex].streamData;
      }
   }

   /* If it reaches here, then no matching entry was found. */
   return NULL;
}  /* END zl303xx_PtpPortFindStreamForParentDS */

/*****************   INTERNAL ROUTINES FOR CLOCK STATE CONTROL  ***************/

/* zl303xx_PtpClockStateAudit */
/** 

   Using its DefaultDS and ParentDS as reference data, this routine loops
   through all Streams associated with this Clock and sets the state of each
   object accordingly.

   No Error messages are returned from this routine. However, notification logs
   are generated in the following scenarios:
   - the Stream over-ride mode interferes with the Audit decision.
   - The local Clock Class is in conflict with the Audit such as when:
     - The Local class is MASTER_ONLY and the Server is External.
     - The Local class is MASTER_ONLY a local Stream has a better Server.
   A Stream's state may be fixed based on the

  Parameters:
   [in]  pPtpClock   Pointer to the target clock's data structure.

  Notes:   Assumes all input pointers to this local routine are valid.
        Assumes the mutex for the clock is already taken.


*******************************************************************************/
zlStatusE zl303xx_PtpClockStateAudit(
      zl303xx_PtpClockS *pPtpClock)
{
   zlStatusE status = ZL303XX_OK;

   /* Pointer to track the local Stream associated with the GM (if this remains
    * NULL then the local clock is the GM). */
   zl303xx_ClockStreamS *pGmStream = NULL;

   /* A temporary Stream structure to store a representation of the ParentDS
    * (if an EXTERNAL Source is provided).   */
   /* These local variables are 'static' since they are fairly large at it helps
    * lower stack size requirements. */
   static zl303xx_ClockStreamS parentStreamBest;
   static zl303xx_PortDataS parentPortDS;

   /* For Logging. */
   const char *fnName = "zl303xx_PtpClockStateAudit";

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
         "%s: GM-Source location (%d): pStream(%p): pPort(%d)",
         fnName,
         pPtpClock->gmSource.location,
         pPtpClock->gmSource.pPtpStream,
         pPtpClock->gmSource.pPtpPort, 0,0);

   /* If a local Stream is associated with the best GM, get its data. */
   if (pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_LOCAL_STREAM)
   {
      /* If a LOCAL stream is selected then its data pointer should also be set.
       * If not then this a a major problem.    */
      if (pPtpClock->gmSource.pPtpStream == NULL)
      {
         ZL303XX_TRACE_ALWAYS(
               "%s: GM-Source location is STREAM(%d): but pStream is NULL (%p)",
               fnName,
               pPtpClock->gmSource.location,
               pPtpClock->gmSource.pPtpStream, 0,0,0);

         return ZL303XX_DATA_CORRUPTION;
      }
      else
      {
         pGmStream = pPtpClock->gmSource.pPtpStream;
      }
   }

   /* If an EXTERNAL Source or Virtual Port is providing the ParentDS, fake a
    * local 'Stream' dataset so the data can be compared later. In the case of a
    * Distributed System a VP may not be local to this Line Card. */
   else if ((pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_EXTERNAL) ||
            (pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_LOCAL_PORT))
   {
      /* Fake a PortDS for the Parent 'Stream' */
      {
         parentPortDS.config.masterOnly = ZL303XX_FALSE;
         parentPortDS.portDS.portIdentity = pPtpClock->gmSource.extComparisonData.receivingPortIdentity;
         parentPortDS.clock = pPtpClock;
      }

      /* Attach the temp port to the temp stream */
      parentStreamBest.portData = &parentPortDS;

      /* Data fill members that may be needed for internal comparisons. */
      parentStreamBest.farEndParentDS.u.v2 = pPtpClock->parentDS;
      parentStreamBest.farEndCurrentDS = pPtpClock->currentDSInt;
      parentStreamBest.farEndCurrentDS.stepsRemoved = pPtpClock->gmSource.extComparisonData.stepsRemoved;
      parentStreamBest.config.mode = ZL303XX_PTP_STREAM_USE_BMC;

      if ((pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_LOCAL_PORT) &&
          (pPtpClock->gmSource.pPtpPort != NULL) &&
          (pPtpClock->gmSource.pPtpPort->virtualData.isVirtualPort == ZL303XX_TRUE))
      {
         parentStreamBest.config.profileCfg.g8275p1.localPriority =
                  pPtpClock->gmSource.pPtpPort->virtualData.config.localPriority;
      }
      else
      {
         parentStreamBest.config.profileCfg.g8275p1.localPriority = 128;
      }

      /* Assign this remote 'Parent-Stream' as the best. */
      pGmStream = &parentStreamBest;
   }

   /* LOCAL_CLOCK or Unknown so Server stream is NULL */
   else
   {
      pGmStream = NULL;
   }

   /* Update the qualFmtEntry flag for all streams. */
   zl303xx_PtpFmtQualifyStreams(pPtpClock);

   /* MASTER_ONLY behavior is referenced in Figures 23 & 26 of IEEE-1588-2008. */
   if (pPtpClock->config.defaultDS.clockQuality.clockClass <= PTP_MASTER_ONLY_CLOCK_CLASS_MAX)
   {
      /* The ParentDS should always be the DefaultDS for MASTER_ONLY Clocks.
       * Otherwise, the Clock should enter the PASSIVE state.
       * This situation is handled within the following sub-routines. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
            "%s: MASTER_ONLY clock (ID: %d) entering M1: GM-Stream(%p).",
            fnName, pPtpClock->clockHandle, pGmStream, 0,0,0);

      /* Will put most Ports in MASTER mode (unless stream over-ride is set).
       * If pGmStream != NULL then that stream will be PASSIVE. */
      zl303xx_PtpClockStateToM1(pPtpClock, pGmStream);
   }

   /* SLAVE-ONLY behaviour is described in Figure 24 of IEEE-1588-2008 */
   else if ((pPtpClock->config.defaultDS.slaveOnly == ZL303XX_TRUE) ||
            (pPtpClock->config.defaultDS.clockQuality.clockClass == PTP_SLAVE_ONLY_CLOCK_CLASS))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
            "%s: SLAVE_ONLY clock (ID: %d) entering S0: GM-Stream(%p).",
            fnName, pPtpClock->clockHandle, pGmStream, 0,0,0);

      /* Will put the pGmStream into SLAVE state (if it is not NULL).
       * All others will go to LISTENING or UNCALIBRATED as required. */
      zl303xx_PtpClockStateToS0(pPtpClock, pGmStream);
   }

   /* This Clock is configured for BMC behaviour. The reference selection
    * manager provides the ParentDS of the best server and local Stream states
    * are set accordingly.  */
   /* Standard BMC behaviour as described in Figures 23 & the lower-right
    * portion Figure 26 (M2 state) of IEEE-1588-2008. */
   else
   {
      /* If no better server has been provided (or the local one is actually
       * Best) then the local Clock will enter MASTER state forcing all
       * local Streams to follow.   */
      if (pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_LOCAL_CLOCK)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
               "%s: Local Clock (%d) entering M2 mode: GM-Stream(%p).",
               fnName, pPtpClock->clockHandle, pGmStream, 0,0,0);

         /* In this case we expect (pGmStream == NULL) otherwise this should be
          * going to the S1/M3 mode. */
         zl303xx_PtpClockStateToM2(pPtpClock, pGmStream);
      }

      /* ELSE: a better Server exists in the network, either recovered from a
       * local Stream or provided by a remote node. */
      else if ((pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_LOCAL_STREAM) ||
               (pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_LOCAL_PORT)  ||
               (pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_EXTERNAL))
      {
         /* The Server is recovered locally, this stream will enter SLAVE state. */
         if (pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_LOCAL_STREAM)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
                  "%s: Local Clock (%d) entering CLIENT mode; Stream (%d)(%p)",
                  fnName, pPtpClock->clockHandle,
                  pGmStream->clockStreamHandle, pGmStream, 0,0);
         }
         /* The Server is recovered locally from a Virtual Port. */
         else if (pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_LOCAL_PORT)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
                  "%s: Local Clock (%d) entering CLIENT mode; Virtual Port (%d)(%p)",
                  fnName, pPtpClock->clockHandle,
                  pPtpClock->gmSource.extComparisonData.receivingPortIdentity.portNumber,
                  pGmStream, 0,0);
         }
         /* The Server is recovered from another client node and provided to
          * the local Clock via the ParentDS. */
         else if (pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_EXTERNAL)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
                  "%s: Local Clock (%d) entering CLIENT mode; External Server (%p)",
                  fnName, pPtpClock->clockHandle, pGmStream, 0,0,0);
         }

         /* Perform: S1 updates on the BEST stream (if it exists locally),
          *          M3 updates on all other streams,  */
         zl303xx_PtpClockStateToS1M3(pPtpClock, pGmStream);
      }

   }

   return status;
}  /* END zl303xx_PtpClockStateAudit */

/* zl303xx_PtpClockStateToS0 */
/** 

   Handles port & stream state transitions for SLAVE_ONLY clocks. This is
   outlined in Figure 24 of the IEEE-1588-2008 standard. In this case,
   - only a single stream can be in SLAVE mode.
   - all other streams should go to LISTENING if unqualified OR UNCALIBRATED if
     qualified.
   - any MASTER_ONLY streams are a configuration ERROR; this is Logged but
     allowed to continue in case of an over-ride mode setting.

  Parameters:
   [in]  pPtpClock      Pointer to the SLAVE_ONLY clock to configure.
   [in]  pBestStream    Pointer to the Stream with the best network server.
                              If this is NULL, then all Streams of this
                              SLAVE_ONLY clock should enter UNCALIBRATED).

  Return Value:
   Nothing

  Notes:
   Assumes all pointers are valid for this local routine.

 *****************************************************************************/
void zl303xx_PtpClockStateToS0(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_ClockStreamS *pBestStream)
{
   /* Variables for looping through the clock's linked lists. */
   zl303xx_PortDataS *pPtpPort = NULL;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* LOOP through each port of the clock... */
   ZL303XX_PTP_FOR_EACH_PORT(&pPtpClock->portList, pPtpPort)
   {
      /* If this is a mixed-mode port, only update the state of the multicast
       * steam (provided it exists). The unicast streams will mirror this
       * state within the stream state machine.        */
      if (pPtpPort->config.mixedMode == ZL303XX_TRUE)
      {
         if (pPtpPort->mcastStream != NULL)
         {
            zl303xx_PtpStreamStateToS0(pPtpPort->mcastStream, pBestStream);
         }
      }

      /* If not a mixed-mode port, LOOP through the port's stream list and
       * perform state updates. */
      else
      {
         ZL303XX_PTP_FOR_EACH_STREAM(&pPtpPort->streamList, pPtpStream)
         {
            /* Instruct all Streams to go to S0. The routine will look after
             * whether this will actually be SLAVE, LISTENING or UNCALIBRATED
             * (as well as handle any over-ride configuration).    */
            zl303xx_PtpStreamStateToS0(pPtpStream, pBestStream);
         }

         /* Update the other Clock Data Sets. */
         if ((pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_LOCAL_STREAM) &&
             (pBestStream != NULL))
         {
            zl303xx_PtpV2UseStreamForClockDS(pBestStream, pPtpClock);
         }
         else if ((pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_EXTERNAL) &&
                  (pBestStream != NULL))
         {
            zl303xx_PtpV2UseStreamForClockDS(pBestStream, pPtpClock);
         }
         else
         {
            zl303xx_PtpV2UseLocalForClockDS(pPtpClock);
         }
      }
   }
}  /* END zl303xx_PtpClockStateToS0 */

/* zl303xx_PtpClockStateToM1 */
/** 

   Handles port & stream state transitions for MASTER_ONLY clocks. This is
   defined as a Clock with clockClass <= 127. (Refer to Figures 23 & 26 of the
   IEEE-1588-2008 standard). In this case, the zl303xx_PtpStreamStateToM1
   routine is called to execute the individual stream updates.

  Parameters:
   [in]  pPtpClock      Pointer to the MASTER_ONLY clock to configure.
   [in]  pBestStream    Pointer to the Stream with the best network server or
                              NULL if none exists. (For this MASTER_ONLY
                              scenario, this can be NULL).

  Return Value:
   Nothing

  Notes:
   Assumes all pointers are valid for this local routine.

 *****************************************************************************/
void zl303xx_PtpClockStateToM1(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_ClockStreamS *pBestStream)
{
   /* Variables for looping through the clock's linked lists. */
   zl303xx_PortDataS *pPtpPort = NULL;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* For M1 Clocks there can be a BEST (pBestStream may be direct from a local
    * Stream or built from an External ParentDS). If there is then this M1 Clock
    * will enter the PASSIVE state; if not, go to MASTER.   */
   if (pBestStream != NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
            "zl303xx_PtpClockStateToM1: Best Stream(%d) is NOT NULL on M1 Clock: Expect PASSIVE",
            pBestStream->clockStreamHandle, 0,0,0,0,0);
   }

   /* LOOP through each port of the clock... */
   ZL303XX_PTP_FOR_EACH_PORT(&pPtpClock->portList, pPtpPort)
   {
      /* If this is a mixed-mode port, only update the state of the multicast
       * steam (provided it exists). The unicast streams will mirror this
       * state within the stream state machine.        */
      if (pPtpPort->config.mixedMode == ZL303XX_TRUE)
      {
         if (pPtpPort->mcastStream != NULL)
         {
            zl303xx_PtpStreamStateToM1(pPtpPort->mcastStream, pBestStream);
         }
      }

      /* If not a mixed-mode port, LOOP through the port's stream list and
       * perform state updates. */
      else
      {
         ZL303XX_PTP_FOR_EACH_STREAM(&pPtpPort->streamList, pPtpStream)
         {
            /* Instruct all Streams to go to M1. The routine will look after
             * whether this will actually be M1 or PASSIVE (as well as handle
             * any over-ride configuration).    */
            zl303xx_PtpStreamStateToM1(pPtpStream, pBestStream);
         }

         /* A Master-Only Clock can only use its own Data Sets. */
        zl303xx_PtpV2UseLocalForClockDS(pPtpClock);
      }
   }
}  /* END zl303xx_PtpClockStateToM1 */

/* zl303xx_PtpClockStateToM2 */
/** 

   Configures the specified clock as the grandmaster (specifically the M2 state
   of IEEE-1588-2008 Figure 26). This is defined as a Clock with
   clockClass >= 128. In this case, the zl303xx_PtpStreamStateToM2 routine is
   called to execute the individual stream updates.

  Parameters:
   [in]  pPtpClock      Pointer to the clock to configure as Grandmaster.
   [in]  pBestStream    Pointer to the Stream with the best network server
                              which should always be NULL for this routine
                              (Argument in the Parameters list for error
                              checking and consistency with similar routines).

  Return Value:
   Nothing

  Notes:
   Assumes all pointers are valid for this local routine.

 *****************************************************************************/
void zl303xx_PtpClockStateToM2(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_ClockStreamS *pBestStream)
{
   /* Variables for looping through the clock's linked lists. */
   zl303xx_PortDataS *pPtpPort = NULL;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* For M2 Clocks there should be no BEST; the reason we are entering M2 is
    * because we ARE the BEST. If there is another BEST then report an error
    * but continue.   */
   if (pBestStream != NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
            "zl303xx_PtpClockStateToM2: Best Stream(%d) is NOT NULL on M2 Clock.",
            pBestStream->clockStreamHandle, 0,0,0,0,0);
   }

   /* LOOP through each port of the clock... */
   ZL303XX_PTP_FOR_EACH_PORT(&pPtpClock->portList, pPtpPort)
   {
      /* If this is a mixed-mode port, only update the state of the multicast
       * steam (provided it exists). The unicast streams will mirror this
       * state within the stream state machine.        */
      if (pPtpPort->config.mixedMode == ZL303XX_TRUE)
      {
         if (pPtpPort->mcastStream != NULL)
         {
            zl303xx_PtpStreamStateToM2(pPtpPort->mcastStream);
         }
      }

      /* If not a mixed-mode port, LOOP through the port's stream list and
       * perform state updates. */
      else
      {
         /* LOOP through the stream table for each port and do updates. */
         ZL303XX_PTP_FOR_EACH_STREAM(&pPtpPort->streamList, pPtpStream)
         {
            /* Instruct all Streams to go to M2. The routine will look after
             * whether this will actually be MASTER (as well as handle any
             * over-ride configuration).    */
            zl303xx_PtpStreamStateToM2(pPtpStream);
         }
      }
   }

   /* A Clock in M2 state can only use itself as the Data Sets. */
   zl303xx_PtpV2UseLocalForClockDS(pPtpClock);

}  /* END zl303xx_PtpClockStateToM2 */

/* zl303xx_PtpClockStateToS1M3 */
/** 

   Handles port & stream state transitions for BMC client (SLAVE) clocks.
   This is the regular S1 state of Figure 24 in the IEEE-1588-2008 standard when
   clock is in BMC mode rather than SLAVE_ONLY mode. In this case,
   - only a single stream can be in SLAVE mode.
   - all other streams should go to MASTER (M3).

  Parameters:
   [in]  pPtpClock      Pointer to the client clock to configure.
   [in]  pBestStream    Pointer to the Stream with the best network server.
                              If this is NULL (implying the Server is recovered
                              by another client node), then all local ports will
                              become M3 (depending on Stream Over-ride mode).

  Return Value:
   Nothing

  Notes:
   Assumes all pointers are valid for this local routine.

 *****************************************************************************/
void zl303xx_PtpClockStateToS1M3(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_ClockStreamS *pBestStream)
{
   /* Variables for looping through the clock's linked lists. */
   zl303xx_PortDataS *pPtpPort = NULL;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* LOOP through each port of the clock... */
   ZL303XX_PTP_FOR_EACH_PORT(&pPtpClock->portList, pPtpPort)
   {
      /* If this is a mixed-mode port, only update the state of the multicast
       * steam (provided it exists). The unicast streams will mirror this
       * state within the stream state machine.        */
      if (pPtpPort->config.mixedMode == ZL303XX_TRUE)
      {
         if (pPtpPort->mcastStream != NULL)
         {
            zl303xx_PtpStreamStateToM3(pPtpPort->mcastStream, pBestStream);
         }
      }

      /* If not a mixed-mode port, LOOP through the port's stream list and
       * perform state updates. */
      else
      {
         zl303xx_ClockStreamS *pPtpErrorCheckStream = NULL;
         zlStatusE statusPointer = ZL303XX_OK;

         /* LOOP through the stream table for each port and do updates. */
         ZL303XX_PTP_FOR_EACH_STREAM(&pPtpPort->streamList, pPtpStream)
         {
            if (pPtpStream == NULL)
            {
               ZL303XX_TRACE_ALWAYS(">>>ERROR: Stream pointer NULL: ", 0,0,0,0,0,0);
               break;
            }
            else if (((statusPointer = zl303xx_PtpStreamDataGet(pPtpStream->clockStreamHandle, &pPtpErrorCheckStream)) != ZL303XX_OK ) ||
                     (pPtpErrorCheckStream != pPtpStream))
            {
               ZL303XX_TRACE_ALWAYS(">>>ERROR: Stream pointer Mismatch: status(%d): errorHandle(%d): list(%p): array(%p)",
                     statusPointer, pPtpStream->clockStreamHandle, pPtpStream, pPtpErrorCheckStream, 0,0);
               break;
            }
            else
            {
               zl303xx_PtpStreamStateToM3(pPtpStream, pBestStream);
            }
         }

         /* Update the other Clock Data Sets. */
         if ((pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_LOCAL_STREAM) &&
             (pBestStream != NULL))
         {
            zl303xx_PtpV2UseStreamForClockDS(pBestStream, pPtpClock);
         }
         else if ((pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_EXTERNAL) &&
                  (pBestStream != NULL))
         {
            zl303xx_PtpV2UseStreamForClockDS(pBestStream, pPtpClock);
         }
         else
         {
            /* Should be a Virtual Port at this point. */
            zl303xx_PtpV2UseLocalForClockDS(pPtpClock);
         }
      }
   }
}  /* END zl303xx_PtpClockStateToS1M3 */

/*

  Function Name:
    zl303xx_PtpV2UseLocalForClockDS

  Details:
   Done for "M1" & "M2" update (as defined in 1588 spec v2, table 13) or any
   time the local hardware is better than any server (or if a server is not
   present):
   - clear the clock's currenDS.
   - uses the local hardware Time-Properties for the clock's timePropertiesDS.

  Parameters:
   [in]  pPtpClock  Pointer to the clock that is becoming a grandmaster &
                          from which the default data will be copied.

  Return Value:
   Nothing

*******************************************************************************/
void zl303xx_PtpV2UseLocalForClockDS(zl303xx_PtpClockS *pPtpClock)
{
   /* Clear the Clock's Current data set */
   OS_MEMSET(&pPtpClock->currentDSInt, 0, sizeof(pPtpClock->currentDSInt));

   /* Invalidate the grandmaster stream handle of the clock. */
   pPtpClock->gmSource.pPtpStream = NULL;

   /* If a Virtual Port is Selected, use its configuration. */
   /* Remember that, in the case of a distributed system, the VP may exist on
    * another line card (or externally) so there isn't much point in looking
    * it up. Just use the data directly as provided. */
   if (pPtpClock->gmSource.location != ZL303XX_PTP_GM_SRC_LOCAL_CLOCK)
   {
      /* Current data set */
      pPtpClock->currentDSInt.stepsRemoved =
                        pPtpClock->gmSource.extComparisonData.stepsRemoved;

      /* Time Properties data set */
      (void)zl303xx_PtpClockTimePropertiesApply(
                        pPtpClock,
                        &pPtpClock->gmSource.extComparisonData.timeProperties);
   }

   /* Actually is the local hardware configuration. */
   else
   {
      /* Update the Time Properties data set with the local hardware configuration. */
      (void)zl303xx_PtpClockTimePropertiesApply(pPtpClock, &pPtpClock->config.localTimeProperties);
   }

   /* Update Profile specific data based on this ParentDS update. */
   (void)zl303xx_PtpProfileClockDsUpdate(pPtpClock);
}  /* END zl303xx_PtpV2UseLocalForClockDS */

/*

  Function Name:
    zl303xx_PtpV2UseStreamForClockDS

  Details:
   Performed as a part of an "S1" update (as defined in 1588 spec v2, table 25)
   for this implementation. When a stream becomes a SLAVE, its data set
   information becomes the parent information for the clock. This routine
   copies the stream data to the clock data sets.

  Parameters:
   [in]  pPtpStream  Pointer to the clock stream from which to copy the
                           data set information.
   [in]  pPtpClock   Pointer to the clock structure to which the stream
                           data set information will be copied.
  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpV2UseStreamForClockDS(zl303xx_ClockStreamS *pPtpStream,
                                            zl303xx_PtpClockS *pPtpClock)
{
   /* Since this stream is in the S1 state, it must be the best for this clock
    * so copy the data to the Clock's Data Sets as well. All other streams will
    * pick up this data as their Master states are evaluated.   */

   /* Update the Clock's Current data set */
   /***************************************/
   pPtpClock->currentDSInt = pPtpStream->farEndCurrentDS;

   /* Update the Clock's Time Properties data set. */
   /**************************************************/
   (void)zl303xx_PtpClockTimePropertiesApply(pPtpClock, &pPtpStream->farEndTimePropertiesDS);

   /* Copy the grandmaster stream handle to the clock. */
   pPtpClock->gmSource.pPtpStream = pPtpStream;

   /* Update Profile specific data based on this ParentDS update. */
   (void)zl303xx_PtpProfileClockDsUpdate(pPtpClock);
}  /* END zl303xx_PtpV2UseStreamForClockDS */

/*

  Function Name:
    zl303xx_PtpV2UseExternalForClockDS

  Details:
   Performed as a part of an "S1" update (as defined in 1588 spec v2, table 25)
   for this implementation. When an external connection is designated as the
   SLAVE, its data set is provided to the local clock and applied.

  Parameters:
   [in]  pPtpClock   Pointer to the clock structure to which the external
                           data set information will be copied.

  Return Value:
   Nothing

*******************************************************************************/
void zl303xx_PtpV2UseExternalForClockDS(zl303xx_PtpClockS *pPtpClock)
{
   /* Clear the Clock's Current data set */
   OS_MEMSET(&pPtpClock->currentDSInt, 0, sizeof(pPtpClock->currentDSInt));

   /* Invalidate the grandmaster stream handle of the clock. */
   pPtpClock->gmSource.pPtpStream = NULL;

   /* Use any provided external data for the data sets and transmit ANNOUNCE messages.  */
   /* Current data set */
   pPtpClock->currentDSInt.stepsRemoved = pPtpClock->gmSource.extComparisonData.stepsRemoved;
   /* Time Properties data set */
   (void)zl303xx_PtpClockTimePropertiesApply(pPtpClock, &pPtpClock->gmSource.extComparisonData.timeProperties);


   /* Update Profile specific data based on this ParentDS update. */
   (void)zl303xx_PtpProfileClockDsUpdate(pPtpClock);
}  /* END zl303xx_PtpV2UseExternalForClockDS */

/* zl303xx_PtpClockContractCountGet */
/**
   Returns the number of unicast negotiation contracts granted by a clock for a
   specific messageType.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   msgType      The messageType to query.
   [out]  count        Number of contracts granted for messageType.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    count is NULL.
     ZL303XX_PARAMETER_INVALID  clockHandle or msgType is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockContractCountGet(zl303xx_PtpClockHandleT clockHandle,
                                           zl303xx_MessageTypeE msgType,
                                           Uint32T *count)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock;
   zl303xx_V2UnicastNegotiatedMsgTypeE type;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(count);
   }

   /* Get a pointer to the clock structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MessageTypeToIndex(msgType, &type);
   }

   if (status == ZL303XX_OK)
   {
      *count = zl303xx_PtpV2GetClockContractCount(pPtpClock, NULL, type);
   }

   return status;
}

/* zl303xx_PtpClockContractCountMaxGet */
/**
   Returns the maximum number of unicast negotiation contracts that can be
   granted by a clock for a specific messageType.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   msgType      The messageType to query.
   [out]  limit        Maximum number of contracts that can be granted for
                            messageType (-1 = unlimited).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    limit is NULL.
     ZL303XX_PARAMETER_INVALID  clockHandle or msgType is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockContractCountMaxGet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_MessageTypeE msgType,
      Uint32T *limit)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock;
   zl303xx_V2UnicastNegotiatedMsgTypeE type;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(limit);
   }

   /* Get a pointer to the clock structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MessageTypeToIndex(msgType, &type);
   }

   if (status == ZL303XX_OK)
   {
      *limit = pPtpClock->contractLimits[type].maxCount;
   }

   return status;
}

/* zl303xx_PtpClockContractCountMaxSet */
/**
   Sets the maximum number of unicast negotiation contracts that can be granted
   by a clock for a specific messageType.

   In the event that this parameter is lowered at run-time, to a value less than
   the current number of existing contracts, then those existing contracts will
   be permitted to expire normally. (Since the IEEE-1588 Std suggests that once
   a contract is GRANTED it should be honored for the duration of the contract
   and it would be unfair to cancel one contract but allow another to continue).

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   msgType      The messageType to limit.
   [in]   limit        Maximum number of contracts to grant (-1 = unlimited).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  clockHandle or msgType is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpClockContractCountMaxSet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_MessageTypeE msgType,
      Uint32T limit)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;
   zl303xx_V2UnicastNegotiatedMsgTypeE type;

   /* Get a pointer to the clock structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MessageTypeToIndex(msgType, &type);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   /* Update the contract limit; unlock mutex. */
   if (status == ZL303XX_OK)
   {
      pPtpClock->contractLimits[type].maxCount = limit;

      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}

/* zl303xx_PtpClockTotalPpsMaxSet */
/**
   Sets a clock's total PPS service limit for a specific message type. Note that
   this will not affect any existing unicast negotiation contracts. Only new
   or renewed contracts will use the new value.

  Parameters:
   [in]  clockHandle    Handle to an existing clock.
   [in]  msgType        The message type limit to modify.
   [in]  totalPpsMax    New PPS service limit.
                              - Setting to 0 will reject all contracts.
                              - Setting to -1 will set UNLIMITED pps.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or msgType is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockTotalPpsMaxSet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_MessageTypeE msgType,
      Uint32T totalPpsMax)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check input parameters. */
   if ((status == ZL303XX_OK) &&
       ((status = ZL303XX_CHECK_MESSAGE_TYPE(msgType)) != ZL303XX_OK))
   {
      ZL303XX_TRACE_ALWAYS("Invalid msgType=%d", msgType, 0,0,0,0,0);
   }

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   /* Update the PPS limit. Unlock the mutex. */
   if (status == ZL303XX_OK)
   {
      pPtpClock->config.unicastNegotiation.totalPpsMax[msgType] = totalPpsMax;
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* END zl303xx_PtpClockTotalPpsMaxSet */

/* zl303xx_PtpClockTotalPpsGet */
/**
   Returns the total PPS counters for all unicast negotiated message types
   (ANNOUNCE, SYNC, and DELAY_RESP) on a specified clock.

  Parameters:
   [in]  clockHandle    Handle to a previously created clock.

   [out] pTotal         Returned PPS counters.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       pTotal is NULL.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid
     ZL303XX_INVALID_MODE          Unicast negotiation not enabled on any port.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockTotalPpsGet(zl303xx_PtpClockHandleT clockHandle,
                                      zl303xx_PtpTotalPpsS *pTotal)
{
   zlStatusE status = ZL303XX_OK;

   /* Pointer to the clock data and associated Ports */
   zl303xx_PtpClockS *pPtpClock = NULL;
   zl303xx_PortDataS *pPtpPort = NULL;

   /* The returned PPS rates are for Unicast Contracts only. If no Port
    * is in Unicast Negotiation mode then return ZL303XX_PARAMETER_INVALID. */
   zl303xx_BooleanE unicastEnabled = ZL303XX_FALSE;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pTotal);
   }

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Take the Mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   /* Total the PPS rates for all Unicast Negotiation Ports. */
   /* Get the total PPS counters. */
   if (status == ZL303XX_OK)
   {
      /* Clear the PPS count array. */
      OS_MEMSET(pTotal, 0, sizeof(*pTotal));

      /* For every Unicast Neg enabled Port on the Clock, add the negotiated
       * PPS to the clock's total (Ignores packet rates < 1pps). */
      ZL303XX_PTP_FOR_EACH_PORT(&pPtpClock->portList, pPtpPort)
      {
         if (pPtpPort->config.unicastNegotiation.enabled == ZL303XX_TRUE)
         {
            /* At least 1 port is in the correct */
            unicastEnabled = ZL303XX_TRUE;

            /* Add the negotiated rates for this port. */
            pTotal->msg[ZL303XX_MSG_ID_SYNC]            += zl303xx_PtpV2GetMsgTypePortTotalPps(pPtpPort, NULL, PTP_NEGOTIATED_MSG_ID_SYNC);
            pTotal->msg[ZL303XX_MSG_ID_ANNOUNCE]        += zl303xx_PtpV2GetMsgTypePortTotalPps(pPtpPort, NULL, PTP_NEGOTIATED_MSG_ID_ANNOUNCE);
            pTotal->msg[ZL303XX_MSG_ID_DELAY_RESP]      += zl303xx_PtpV2GetMsgTypePortTotalPps(pPtpPort, NULL, PTP_NEGOTIATED_MSG_ID_DELAY_RESP);
            pTotal->msg[ZL303XX_MSG_ID_PEER_DELAY_RESP] += zl303xx_PtpV2GetMsgTypePortTotalPps(pPtpPort, NULL, PTP_NEGOTIATED_MSG_ID_PDELAY_RESP);
         }
      }

      /* Release the Clock MUTEX. */
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   /* If everything else is OK, check if any Port was in Uni-Neg mode.
    * (This is not done if there was any PARAM or MUTEX error. */
   if (status == ZL303XX_OK)
   {
      if (unicastEnabled == ZL303XX_FALSE)
      {
         status = ZL303XX_INVALID_MODE;
      }
   }

   return status;
}  /* END zl303xx_PtpClockTotalPpsGet */

/* zl303xx_PtpClockMessagingEnabled */
/**
   If clock messaging is enabled, all RX and TX PTP message processing happens
   normally. If it's disabled, no TX messages will be sent and all RX messages
   will be blocked.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   enabled      Clock messaging behavior to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or enabled is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockMessagingEnabled(zl303xx_PtpClockHandleT clockHandle,
                                           zl303xx_BooleanE enabled)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pClock;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(enabled);
   }
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pClock);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pClock);
   }

   if (status == ZL303XX_OK)
   {
      pClock->enableRxTx = enabled;
      status = zl303xx_PtpClockMutexUnlock(pClock, status);
   }

   return status;
}

/* zl303xx_PtpTimerBmcaUpdateInterval */
/** 

   Callback for zl303xx_ClockS::bmca::updateEventTimer::callback().

   Determines if the BMCA Update Event will automatically be issues.

  Parameters:
   [in]   pTimer  Clock BMCA Update Event Timer.

*******************************************************************************/
static void zl303xx_PtpTimerBmcaUpdateInterval(zl303xx_PtpTimerS *pTimer)
{
   /* Get the clock pointer associated with the timer. */
   zl303xx_PtpClockS *pPtpClock = ZL303XX_CONTAINER_GET(pTimer, zl303xx_PtpClockS, bmca.updateTimer);

   /* Upload the Foreign Master Table for the clock. */
   zl303xx_PtpClockBmcaUpdateInitiate(pPtpClock);
}

/* zl303xx_PtpClockTimersInit */
/** 

   Initializes the clock timers. (Only has a  BMCA Update Interval Timer).
   - Sets the callback routine.
   - Starts / stops the timers based on initialization values.

  Parameters:
   [in,out]  pPtpClock  Clock that will have its timers initialized.

*******************************************************************************/
void zl303xx_PtpClockTimersInit(zl303xx_PtpClockS *pPtpClock)
{
   /* Timer Interval */
   zl303xx_TimeStamp interval = {{0,0},0};

   /* Set the default callback routine for this structure. */
   pPtpClock->bmca.updateTimer.callback = zl303xx_PtpTimerBmcaUpdateInterval;
   OS_SPRINTF(pPtpClock->bmca.updateTimer.descr, "%d:%s", pPtpClock->clockHandle, "BMCA_UPDT");

   /* The Timer is not needed in the following scenarios. */
   if ((pPtpClock->config.bmca.updateEventSec == 0) ||
       (pPtpClock->config.bmca.updateTrigger == ZL303XX_BMCA_TRIGGER_DISABLED) ||
       (pPtpClock->config.bmca.updateTrigger == ZL303XX_BMCA_TRIGGER_ON_RX_ANNC_DELTA))
   {
      zl303xx_PtpTimerStop(&pPtpClock->bmca.updateTimer);
   }

   else
   {
      /* Defined TIMER Interval */
      if (pPtpClock->config.bmca.updateTrigger == ZL303XX_BMCA_TRIGGER_TIMER)
      {
         interval.secondsField.lo = pPtpClock->config.bmca.updateEventSec;
      }

      /* Highest ANNOUNCE Rate */
      else if (pPtpClock->config.bmca.updateTrigger == ZL303XX_BMCA_TRIGGER_MIN_ANNC_INTVL)
      {
         /* Get the fastest ANNC stream on this clock. */
         Sint8T minIntvl = zl303xx_PtpClockFindFastestAnncInterval(pPtpClock);

         /* If there are no streams on the current clock then the last function
          * will return the maximum of 0x7F = a very long time that will not fit
          * into the seconds field of the time variable. If such a long period
          * was actually intended then it would be best to just DISABLE the timer.
          * Here, choose a reasonable update period. */
         if (minIntvl > 31)   /* only support 32 bits. */
         {
            interval.secondsField.lo = ZL303XX_PTP_DEFAULT_CLOCK_EVT_BMCA_UPDATE_SEC;
         }

         /* Convert the interval to a time. */
         else
         {
            zl303xx_PtpConvertLog2IntervalToTime(minIntvl, &interval);
         }
      }

      /* If the Timer is already active, just update it without losing any of
       * the previous carryOver.  */
      if (zl303xx_PtpTimerActive(&pPtpClock->bmca.updateTimer))
      {
         pPtpClock->bmca.updateTimer.interval = interval;
      }

      /* The Timer is being ENABLED from scratch.  */
      else
      {
         zl303xx_PtpTimerStart(pPtpClock->clockHandle,
                              &pPtpClock->bmca.updateTimer,
                              &interval,
                              ZL303XX_FALSE);
      }
   }
}

/* zl303xx_PtpClockFindFastestAnncInterval */
/**
   Utility to determine the stream with the highest Announce rate so that the
   BMCA update timer can be set to upload the data at the same rate.

  Parameters:
   [in]  pPtpClock		Pointer to an existing clock.

  Return Value:  Sint8T	Smallest interval value (equals the fastest PPS).

*******************************************************************************/
static Sint8T zl303xx_PtpClockFindFastestAnncInterval(zl303xx_PtpClockS *pPtpClock)
{
   /* Variables for looping through the clock's linked lists. */
   zl303xx_PortDataS *pPtpPort = NULL;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Set default return value to the slowest rate (max interval). */
   Sint8T minIntvl = 0x7F;

   /* Loop through all stream for the fastest Announce rate. */
   ZL303XX_PTP_FOR_EACH_PORT(&pPtpClock->portList, pPtpPort)
   {
      /* Virtual PTP Ports should still trigger updates. For example, on a
       * GM with just a VP, there is no ANNC rate and therefore this calculation
       * would produce minIntvl of 127 (over 100 years). So, we just use the
       * default BMCA Update rate of 8 seconds (intvl = 3) in that case. */
      if (pPtpPort->virtualData.isVirtualPort == ZL303XX_TRUE)
      {
         Sint32T defMinIntvl = log2PacketsPerSecond(-ZL303XX_PTP_DEFAULT_CLOCK_EVT_BMCA_UPDATE_SEC);
         if (defMinIntvl < minIntvl)
         {
            minIntvl = defMinIntvl;
         }
      }
      else /* LOOP through each stream of the port... */
      {
         ZL303XX_PTP_FOR_EACH_STREAM(&pPtpPort->streamList, pPtpStream)
         {
            /* If this stream is configured for unicast negotiation, use the contract rate. */
            if ((pPtpStream->config.unicast == ZL303XX_TRUE) &&
                (pPtpStream->portData->config.unicastNegotiation.enabled == ZL303XX_TRUE))
            {
               if ((pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].remainingSec > 0) &&
                   (pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].req.interval < minIntvl))
               {
                  minIntvl = pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].req.interval;
               }
            }
            else  /* Multicast: use the configured rate */
            {
               if (pPtpStream->config.logAnnounceInterval < minIntvl)
               {
                  minIntvl = pPtpStream->config.logAnnounceInterval;
               }
            }
         }
      }
   }

   return minIntvl;
}  /* END zl303xx_PtpClockFindFastestAnncInterval */

/* zl303xx_PtpClockBmcaUpdateInitiate */
/**

   Internal routine to trigger the ZL303XX_PTP_EVENT_CLOCK_BMCA_UPDATE event.

  Parameters:
   [in]  pPtpClock   Existing Clock that will send its BMCA data.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      pPtpClock is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpClockBmcaUpdateInitiate(zl303xx_PtpClockS *pPtpClock)
{
   zlStatusE status = ZL303XX_OK;

   /* Check the clock pointer to the clock structure. */
   if (pPtpClock)
   {
      /* Upload the Foreign Master Table for the clock. */
      pPtpClock->foreignMasterDS.tableUpdated = ZL303XX_TRUE;
      zl303xx_PtpEventClockBmcaUpdate(pPtpClock);
   }
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "zl303xx_PtpClockBmcaUpdateInitiate: Invalid Clock Pointer",
            0,0,0,0,0,0);
      status = ZL303XX_INVALID_POINTER;
   }

   return status;
}  /* END zl303xx_PtpClockBmcaUpdateInitiate */

