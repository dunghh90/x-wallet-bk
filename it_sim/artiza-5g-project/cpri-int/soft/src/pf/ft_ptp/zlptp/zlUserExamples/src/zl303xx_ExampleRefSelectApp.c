

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*
*  This module provides functions for managing the selection of a reference on
*  a client system. It manages message handling (dynamic and configuration),
*  data access and output control functions.
*
******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

/* The Database & User Decision headers. */
#include "zl303xx_ExampleRefSelectApi.h"
#include "zl303xx_ExampleRefSelectInternal.h"

/* Headers for external modules. */
#include "zl303xx_PtpApi.h"

#include <string.h>


/*****************   EXTERNS     **********************************************/

/*****************   DEFINES     **********************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   LOCAL FUNCTION DEFINITIONS   *****************************/

/* Utility routines for sorting Table entries. */
zlStatusE exampleRefSelectTableSort(void);
zl303xx_BooleanE exampleRefSelectTableOrderTwoEntries(
      exampleRefSelectBmcaTypeE profile,
      exampleRefSelectEntryS *entryA,
      exampleRefSelectEntryS *entryB);
zl303xx_BooleanE exampleRefSelectTableIsEntryInUse(
      exampleRefSelectEntryS *entryRow);
void exampleRefSelectTableSwapRows(
      exampleRefSelectEntryS *entryA,
      exampleRefSelectEntryS *entryB);

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/* The actual Server Database table. */
exampleRefSelectEntryS exampleRefSelectTable[REF_SELECT_DB_ENTRIES_MAX];

/* Reference Selection Application Configuration Parameters */
exampleRefSelectParamS exampleRefSelectAppParam;

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS (API)   ********************/

/* exampleRefSelectAppStructInit */
/**

   Returns the default configuration values for the Reference Selection
   Application (defined in the corresponding Setup.h file).

  Parameters:
   [in]  configParams   Pointer to the configuration structure to initialize.

   [out] configParams   The initialized configuration structure.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   Input pointer is NULL.

*******************************************************************************/
zlStatusE exampleRefSelectAppStructInit(exampleRefSelectConfigS *configParams)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 4,
         "exampleRefSelectAppStructInit: ENTRY", 0,0,0,0,0,0);

   /* Check for a valid input pointer. */
   if (configParams == NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectAppStructInit: NULL Pointer Input)",
            0,0,0,0,0,0);
      status = ZL303XX_INVALID_POINTER;
   }

   /* Fill in the default parameters. */
   else
   {
      /* CGU device */
      configParams->hwAlgParam = NULL;

      /* Application Refresh Rate. */
      configParams->refreshRateMs = REF_SELECT_DEFAULT_APP_REFRESH_RATE_MS;

      /* Selection Parameters */
      configParams->selection.autoRun     = REF_SELECT_DEFAULT_AUTO_SELECTION_MODE;
      configParams->selection.bmcAlg      = REF_SELECT_DEFAULT_BMCA_ALG;
      configParams->selection.forcedRefId = REF_SELECT_DEFAULT_FORCED_REF_ID;

      /* Telecom Profile protection parameters. */
      configParams->protection.revertiveEn = REF_SELECT_DEFAULT_TP_REVERTIVE_MODE;
      configParams->protection.holdOffSec  = REF_SELECT_DEFAULT_TP_HOLDOFF_SECONDS;
      configParams->protection.squelchEn   = REF_SELECT_DEFAULT_TP_SQUELCH_MODE;
   }

   return status;
}  /* exampleRefSelectAppStructInit */

/* exampleRefSelectAppConfigGet */
/**

   Returns the active configuration of the Reference Selection Application.

  Parameters:
   [in]  configParams   Pointer to the configuration structure to return.

   [out] configParams   The active configuration of the application.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   Input pointer is NULL.

*******************************************************************************/
zlStatusE exampleRefSelectAppConfigGet(exampleRefSelectConfigS *configParams)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 4,
         "exampleRefSelectAppConfigGet: ENTRY", 0,0,0,0,0,0);

   /* Check for a valid input pointer. */
   if (configParams == NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectAppConfigGet: NULL Pointer Input)",
            0,0,0,0,0,0);
      status = ZL303XX_INVALID_POINTER;
   }

   /* Fill in the active parameters. */
   else
   {
      *configParams = exampleRefSelectAppParam.config;
   }

   return status;
}  /* exampleRefSelectAppConfigGet */

/* exampleRefSelectAppConfigSet */
/**

   Initializes (or re-sets) the configuration of the Reference Selection
   Application.

  Parameters:
   [in]  configParams   Pointer to the configuration structure to apply to
                              the Reference Selection Application.

  Return Value:
     ZL303XX_OK                      Success.
     ZL303XX_INVALID_POINTER         Input pointer is NULL.
     ZL303XX_PARAMETER_INVALID       A supplied parameter is INVALID.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL    Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL    Failed to unlock mutex.

*******************************************************************************/
zlStatusE exampleRefSelectAppConfigSet(exampleRefSelectConfigS *configParams)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 4,
         "exampleRefSelectAppConfigSet: ENTRY", 0,0,0,0,0,0);

   /* Check for a valid input pointer. */
   if (configParams == NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectAppConfigSet: NULL Pointer Input)",
            0,0,0,0,0,0);
      status = ZL303XX_INVALID_POINTER;
   }

   /* Verify the supplied configuration parameters. */
   if (status == ZL303XX_OK)
   {
      status = exampleRefSelectConfigCheck(configParams);
   }

   /* Lock the Mutex IF it has been initialized and SET the new configuration.
    * It is possible that this SET routine was called prior to the Application
    * Initialization and the MUTEX may not yet exist. In that case,
    * ZL303XX_RTOS_MUTEX_INVALID will be returned; and the data should still be set.
    * Anything else is a valid failure.  */
   if (status == ZL303XX_OK)
   {
      /* Log the Current and NEW Configuration. */
      (void)exampleRefSelectConfigTrace(&exampleRefSelectAppParam.config, configParams);

      /* Attempt to lock the Mutex. */
      status = exampleRefSelectMutexLock();

      /* Ignore the MUTEX status if uninitialized, otherwise use it. */
      if ((status == ZL303XX_OK) ||
          (status == ZL303XX_RTOS_MUTEX_INVALID))
      {
         /* Update the Application Configuration. */
         exampleRefSelectAppParam.config = *configParams;

         /* If there was a valid MUTEX, Unlock it. */
         if (status == ZL303XX_OK)
         {
            /* This now becomes the overall status. */
            status = exampleRefSelectMutexUnlock();
         }
         /* There was no MUTEX which is OK; Force an OK status since we still
          * SET the configuration.*/
         else
         {
            status = ZL303XX_OK;
         }
      }

      /* A real error in taking the MUTEX. */
      else
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
               "RefSelectAppConfig: ERROR: Failed to TAKE MUTEX (%d)",
               status, 0,0,0,0,0);
      }
   }

   /* Send a 'Config-Change' message to the Application. Two possibilities exist here:
    *  1) The application is active, in which case this message will update
    *     internal controls and re-evaluate the selected Server based on the
    *     new setting. (ZL303XX_OK is the expected return value).
    *  2) The Task is not active and this SET routine was called to configure
    *     startup values for the application. (ZL303XX_NOT_RUNNING is the expected
    *     return value; can be ignored).  */
   if (status == ZL303XX_OK)
   {
      /* Ignore the TASK status if it hasn't started. (The MsgQ should exist
       * if the Task is running so do not ignore that, or any other failure). */
      if ((status = exampleRefSelectConfigChange()) == ZL303XX_NOT_RUNNING)
      {
         /* There was no TASK yet which is OK; Force an OK status since we SET
          * the initial configuration.*/
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
               "RefSelectAppConfig: Setting Initial Configuration Values",
               status, 0,0,0,0,0);

         status = ZL303XX_OK;
      }

      /* A real error sending a message to the Task. */
      else if (status != ZL303XX_OK)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
               "RefSelectAppConfig: ERROR: Failed to Send WAKE-UP (%d)",
               status, 0,0,0,0,0);
      }

      /* Log the fact that the Configuration has possibly been changed. */
      else
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
               "RefSelectAppConfig: Configuration Update",
               0,0,0,0,0,0);
      }
   }

   return status;
}  /* exampleRefSelectAppConfigSet */

/* exampleRefSelectAppStart */
/**

   Starts the Reference Selection Application and all required components:
   - Initializes the Server Table and Data Mutex.
   - Creates the central OS Task and associated Message-Q
   - Initializes all default data.

  Parameters:
   [in]  configParams   Pointer to the configuration structure to apply to
                              the Reference Selection Application. If NULL,
                              then the default configuration will be generated
                              from the Setup.h module.

  Return Value:
     ZL303XX_OK                      Success.
     ZL303XX_MULTIPLE_INIT_ATTEMPT   Application is already running.
     ZL303XX_PARAMETER_INVALID       A supplied or setup.h parameter is INVALID.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL    Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL    Failed to unlock mutex.
     ZL303XX_RTOS_MUTEX_CREATE_FAIL  Failed to create the data mutex.
     ZL303XX_RTOS_TASK_CREATE_FAIL   Failed to create the Application Task.

*******************************************************************************/
zlStatusE exampleRefSelectAppStart(exampleRefSelectConfigS *configParams)
{
   zlStatusE status = ZL303XX_OK;
   exampleRefSelectConfigS appConfig;

   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 4,
         "exampleRefSelectAppStart: ENTRY", 0,0,0,0,0,0);

   /* First, determine if the Application is already running. */
   if (status == ZL303XX_OK)
   {
      /* OK means already running; NOT_RUNNING means it's not. */
      if (exampleRefSelectTaskStatus() == ZL303XX_OK)
      {
         status = ZL303XX_MULTIPLE_INIT_ATTEMPT;
      }
   }

   /****** Initialize the Application Configuration Values.   *******/
   if (status == ZL303XX_OK)
   {
      /* Determine the source of the Application Parameters. */
      if (configParams == NULL)
      {
         status = exampleRefSelectAppStructInit(&appConfig);
      }
      else
      {
         appConfig = *configParams;
      }

      /* Set the defaults or the config (if provided). */
      if (status == ZL303XX_OK)
      {
         status = exampleRefSelectAppConfigSet(&appConfig);
      }
   }

   /****** Initialize internal parameters, controls, timers, etc.  *******/
   if (status == ZL303XX_OK)
   {
      status = exampleRefSelectAppInitRunTimeParams(&exampleRefSelectAppParam.rt);
   }

   /****** Create the Reference Table and related components.  *******/
   /* The Initialization routine TAKEs/GIVEs the MUTEX internally. */
   if (status == ZL303XX_OK)
   {
      /* If a failure occurs, an error will be printed within the routine. */
      status = exampleRefSelectTableInit();
   }

   /****** Reference Selection Task Initialization and related components.  *******/
   if (status == ZL303XX_OK)
   {
      /* If a failure occurs, an error will be printed within the routine. */
      status = exampleRefSelectTaskStart();
   }

   return status;
}  /* exampleRefSelectAppStart */

/* exampleRefSelectAppStop */
/**

   Stops the OS task associated with the centralised Server database.

  Parameters:
   [in]  stopParams     Pointer to the structure for stopping the Application.
                              (Not currently used).

  Return Value:
     ZL303XX_OK                      Success.
     ZL303XX_RTOS_MUTEX_DELETE_FAIL  Failed to delete the data mutex.
     ZL303XX_RTOS_TASK_DELETE_FAIL   Failed to delete the Application Task.

*******************************************************************************/
zlStatusE exampleRefSelectAppStop(exampleRefSelectStopS *stopParams)
{
   zlStatusE status = ZL303XX_OK;     /* Overall Status */
   zlStatusE tStatus = ZL303XX_OK;    /* Task Status */
   zlStatusE dStatus = ZL303XX_OK;    /* Database Status */

   /* The input parameters are not used so do not check the pointer. */
   /* Warning removal */
   if (stopParams) {;}

   /****** Delete the Reference Selection Task and related components.  *******/
   /* Always continue regardless of return status. */
   if ((tStatus = exampleRefSelectTaskStop()) != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectAppStop: ERROR deleting task (%d)",
            tStatus, 0,0,0,0,0);
   }

   /****** Create the Reference Table and related components.  *******/
   /* Always continue regardless of return status. */
   if ((dStatus = exampleRefSelectTableDelete()) != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectAppStop: ERROR deleting DbTable (%d)",
            dStatus, 0,0,0,0,0);
   }

   /* Since there may be multiple failures, assign a priority status. */
   if (tStatus != ZL303XX_OK)
   {
      status = tStatus;
   }
   else if (dStatus != ZL303XX_OK)
   {
      status = dStatus;
   }

   return status;
}  /* END exampleRefSelectAppStop */

/**************    USER DECISION MODULE CONFIGURATION API ROUTINES     ********/

/* exampleRefSelectCguDeviceSet */
/**
   Sets the Time-Sync Algorithm Device pointer for interfacing to the
   algorithm API.

  Parameters:
   [in]  devicePtr   Pointer to the Time-Sync Algorithm Device.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE exampleRefSelectCguDeviceSet(
      void *devicePtr)
{
   zlStatusE status = ZL303XX_OK;

   /* Current Application Configuration. */
   exampleRefSelectConfigS configParams;

   /* Retrieve the current application configuration. */
   if (status == ZL303XX_OK)
   {
      status = exampleRefSelectAppConfigGet(&configParams);
   }

   /* Update the target field and reset the configuration using the generic SET
    * routine. It will do all validation checking on pointers and parameters as
    * well as handle all internal updating, etc.  */
   if (status == ZL303XX_OK)
   {
      configParams.hwAlgParam = devicePtr;

      status = exampleRefSelectAppConfigSet(&configParams);
   }

   return status;
}  /* exampleRefSelectCguDeviceSet */

/* exampleRefSelectRefreshTimerSet */
/**
   Sets the Refresh timer value to re-evaluate internal counters (Hold-off, WTR, etc).

  Parameters:
   [in]  refreshRateMs  The number of milliseconds between refresh cycles.
                              The actual refresh rate is rounded down based on the
                              CPU Tick interval (as long as != 0)
                              - Valid Range: 1 mSec to 100,000,000 mSec (100 kSec).
                                (if OS_TICK Interval > 1mSec; then 1 TICK interval is used).
                              - REF_SELECT_TIMER_DISABLED (-1) disables the refresh.
                              - OS_NO_WAIT (0) is not permitted.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     refreshRateMs is invalid (Out of Range).

*******************************************************************************/
zlStatusE exampleRefSelectRefreshTimerSet(
      Uint32T refreshRateMs)
{
   zlStatusE status = ZL303XX_OK;

   /* Current Application Configuration. */
   exampleRefSelectConfigS configParams;

   /* Retrieve the current application configuration. */
   if (status == ZL303XX_OK)
   {
      status = exampleRefSelectAppConfigGet(&configParams);
   }

   /* Update the target field and reset the configuration using the generic SET
    * routine. It will do all validation checking on pointers and parameters as
    * well as handle all internal updating, etc.  */
   if (status == ZL303XX_OK)
   {
      configParams.refreshRateMs = refreshRateMs;

      status = exampleRefSelectAppConfigSet(&configParams);
   }

   return status;
}  /* exampleRefSelectRefreshTimerSet */

/* exampleRefSelectAutoSelectEn */
/**
   Sets whether or not to automatically execute a Server Selection Cycle when
   a table update or default Task action occurs (or only run a Manual Selection).

  Parameters:
   [in]  autoSelectEn   Flag enabling/disabling automatic server selection.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     autoSelectEn is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE exampleRefSelectAutoSelectEn(
      zl303xx_BooleanE autoSelectEn)
{
   zlStatusE status = ZL303XX_OK;

   /* Current Application Configuration. */
   exampleRefSelectConfigS configParams;

   /* Retrieve the current application configuration. */
   if (status == ZL303XX_OK)
   {
      status = exampleRefSelectAppConfigGet(&configParams);
   }

   /* Update the target field and reset the configuration using the generic SET
    * routine. It will do all validation checking on pointers and parameters as
    * well as handle all internal updating, etc.  */
   if (status == ZL303XX_OK)
   {
      configParams.selection.autoRun = autoSelectEn;

      status = exampleRefSelectAppConfigSet(&configParams);
   }

   return status;
}  /* exampleRefSelectAutoSelectEn */

/* exampleRefSelectBmcAlgorithmSet */
/**
   Sets the BMC Algorithm used by the Reference Selection Application to select
   the best server.

  Parameters:
   [in]  bmcAlg   The BMC Algorithm to use (Default 1588, Telecom Profile
                        or CUSTOM - currently does nothing).

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     bmcAlg is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE exampleRefSelectBmcAlgorithmSet(
      exampleRefSelectBmcaTypeE bmcAlg)
{
   zlStatusE status = ZL303XX_OK;

   /* Current Application Configuration. */
   exampleRefSelectConfigS configParams;

   /* Retrieve the current application configuration. */
   if (status == ZL303XX_OK)
   {
      status = exampleRefSelectAppConfigGet(&configParams);
   }

   /* Update the target field and reset the configuration using the generic SET
    * routine. It will do all validation checking on pointers and parameters as
    * well as handle all internal updating, etc.  */
   if (status == ZL303XX_OK)
   {
      configParams.selection.bmcAlg = bmcAlg;

      status = exampleRefSelectAppConfigSet(&configParams);
   }

   return status;
}  /* exampleRefSelectBmcAlgorithmSet */

/* exampleRefSelectManualServerSet */
/**
   Manually sets the server to select as the best. (Ignores all related PTSF
   and status flags).

  Parameters:
   [in]  serverId    ID of the server to select. A value of
                           REF_SELECT_CONN_ID_INVALID(-1) disables forced selection.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE exampleRefSelectManualServerSet(
      Uint32T serverId)
{
   zlStatusE status = ZL303XX_OK;

   /* Current Application Configuration. */
   exampleRefSelectConfigS configParams;

   /* Retrieve the current application configuration. */
   if (status == ZL303XX_OK)
   {
      status = exampleRefSelectAppConfigGet(&configParams);
   }

   /* Update the target field and reset the configuration using the generic SET
    * routine. It will do all validation checking on pointers and parameters as
    * well as handle all internal updating, etc.  */
   if (status == ZL303XX_OK)
   {
      configParams.selection.forcedRefId = serverId;

      status = exampleRefSelectAppConfigSet(&configParams);
   }

   return status;
}  /* exampleRefSelectManualServerSet */

/* exampleRefSelectRevertiveEn */
/**
   Sets whether or not to use Revertive-Switching when Telecom Profile BMC
   Algorithm is used.

  Parameters:
   [in]  revertiveEn    Flag whether or not to use Revertive-Switching (valid
                              only if Telecom Profile BMC Algorithm is used).

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     revertiveEn is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE exampleRefSelectRevertiveEn(
      zl303xx_BooleanE revertiveEn)
{
   zlStatusE status = ZL303XX_OK;

   /* Current Application Configuration. */
   exampleRefSelectConfigS configParams;

   /* Retrieve the current application configuration. */
   if (status == ZL303XX_OK)
   {
      status = exampleRefSelectAppConfigGet(&configParams);
   }

   /* Update the target field and reset the configuration using the generic SET
    * routine. It will do all validation checking on pointers and parameters as
    * well as handle all internal updating, etc.  */
   if (status == ZL303XX_OK)
   {
      configParams.protection.revertiveEn = revertiveEn;

      status = exampleRefSelectAppConfigSet(&configParams);
   }

   return status;
}  /* exampleRefSelectRevertiveEn */

/* exampleRefSelectQlHoldoffTimeSet */
/**
   Sets the Hold-Off value for the Server Selection when Telecom Profile BMC
   Algorithm is used.

  Parameters:
   [in]  holdoffSec     The hold-off value in seconds. '0' means no QL Hold-Off.
                              (There is no range limitations to check).

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     holdoffSec is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE exampleRefSelectQlHoldoffTimeSet(
      Uint32T holdoffSec)
{
   zlStatusE status = ZL303XX_OK;

   /* Current Application Configuration. */
   exampleRefSelectConfigS configParams;

   /* Retrieve the current application configuration. */
   if (status == ZL303XX_OK)
   {
      status = exampleRefSelectAppConfigGet(&configParams);
   }

   /* Update the target field and reset the configuration using the generic SET
    * routine. It will do all validation checking on pointers and parameters as
    * well as handle all internal updating, etc.  */
   if (status == ZL303XX_OK)
   {
      configParams.protection.holdOffSec = holdoffSec;

      status = exampleRefSelectAppConfigSet(&configParams);
   }

   return status;
}  /* exampleRefSelectQlHoldoffTimeSet */

/* exampleRefSelectSquelchEn */
/**
   Sets whether or not to use Squelch when no Server is available.

  Parameters:
   [in]  squelchEn   Flag indicating whether or not to use Squelch.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     squelchEn is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE exampleRefSelectSquelchEn(
      zl303xx_BooleanE squelchEn)
{
   zlStatusE status = ZL303XX_OK;

   /* Current Application Configuration. */
   exampleRefSelectConfigS configParams;

   /* Retrieve the current application configuration. */
   if (status == ZL303XX_OK)
   {
      status = exampleRefSelectAppConfigGet(&configParams);
   }

   /* Update the target field and reset the configuration using the generic SET
    * routine. It will do all validation checking on pointers and parameters as
    * well as handle all internal updating, etc.  */
   if (status == ZL303XX_OK)
   {
      configParams.protection.squelchEn = squelchEn;

      status = exampleRefSelectAppConfigSet(&configParams);
   }

   return status;
}  /* exampleRefSelectSquelchEn */

/**************    SERVER DATABASE ENTRY CONFIGURATION API ROUTINES     *******/

/* exampleRefSelectEntryLockoutSet */
/**
   Toggles the Locks-Out for the Server Entry (prevents it from being selected
   by the BMC Algorithm).
   Lockout is ignored if the server is manually forced.

  Parameters:
   [in]  serverId    The serverId to set the value for.
   [in]  lockout     Lock-Out or Allow the entry for selection.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     lockout setting is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE exampleRefSelectEntryLockoutSet(
      Uint32T serverId, zl303xx_BooleanE lockout)
{
   zlStatusE status = ZL303XX_OK;
   zlStatusE mStatus = ZL303XX_OK; /* Mutex Status */

   Uint32T rowIndex;

   /* Check that the lockout value is correct. */
   if ((lockout != ZL303XX_TRUE) &&  (lockout != ZL303XX_FALSE))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectEntryLockoutSet: lockout (%d) Unknown",
            lockout, 0,0,0,0,0);

      status = ZL303XX_PARAMETER_INVALID;
   }

   /* Lock the mutex. */
   else
   {
      mStatus = exampleRefSelectMutexLock();
      status = mStatus;
   }

   /* Check that the stream entry exists. */
   if (status == ZL303XX_OK)
   {
      /* Look up the table entry. */
      status = exampleRefSelectTableFindConnection(serverId, &rowIndex);
   }

   /* Update the configuration parameter. */
   if (status == ZL303XX_OK)
   {
      exampleRefSelectTable[rowIndex].protection.lockout = lockout;
   }

   /* Unlock the Mutex before calling a new BMCA cycle since it will
    * need to take it internally. */
   if (mStatus == ZL303XX_OK)
   {
      (void)exampleRefSelectMutexUnlock();
   }

   /* Notify the Task to re-evaluate Selection parameters. */
   if (status == ZL303XX_OK)
   {
      status = exampleRefSelectAppWakeUp();
   }

   return status;
}  /* exampleRefSelectEntryLockoutSet */

/* exampleRefSelectEntryWtrSet */
/**
   Sets the Wait-to-Restore value for the Server Entry.

  Parameters:
   [in]  serverId    The serverId to set the value for.
   [in]  wtrSec      The wait-to-restore value in seconds.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     wtrSec is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE exampleRefSelectEntryWtrSet(
      Uint32T serverId, Uint32T wtrSec)
{
   zlStatusE status = ZL303XX_OK;
   zlStatusE mStatus = ZL303XX_OK; /* Mutex Status */

   Uint32T rowIndex;

   /* Check the range of the input value. */
   if (status == ZL303XX_OK)
   {
      if (wtrSec > REF_SELECT_WTR_TIMER_MAX)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
               "exampleRefSelectEntryWtrSet: WTR value (%d) > MAX (%d)",
               wtrSec, REF_SELECT_WTR_TIMER_MAX, 0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Lock the mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = exampleRefSelectMutexLock();
      status = mStatus;
   }

   /* Check that the stream entry exists. */
   if (status == ZL303XX_OK)
   {
      /* Look up the table entry. */
      status = exampleRefSelectTableFindConnection(serverId, &rowIndex);
   }

   /* Update the configuration parameter. */
   if (status == ZL303XX_OK)
   {
      exampleRefSelectTable[rowIndex].protection.wtr.seconds = wtrSec;
   }

   /* Unlock the Mutex before calling a new BMCA cycle since it will
    * need to take it internally. */
   if (mStatus == ZL303XX_OK)
   {
      (void)exampleRefSelectMutexUnlock();
   }

   /* Notify the Task to re-evaluate Selection parameters. */
   if (status == ZL303XX_OK)
   {
      status = exampleRefSelectAppWakeUp();
   }

   return status;
}  /* exampleRefSelectEntryWtrSet */

/**************    API ROUTINES TO QUERY THE BEST SERVER SELECTION     ********/

/* exampleRefSelectActiveServerGet */
/**
   Retrieves the data related to the current 'best' entry being used by the
   Application. This does not re-execute the selection algorithm, but simply
   returns the previous entry stored in the selection history.

  Parameters:
   [out] bestEntry   Pointer for the data of the best row in the table.

  Return Value:
     ZL303XX_OK                      Success.
     ZL303XX_TABLE_ENTRY_NOT_FOUND   No Server is currently selected.
     ZL303XX_INVALID_POINTER         Provided pointer is NULL.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL    Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL    Failed to unlock mutex.

*******************************************************************************/
zlStatusE exampleRefSelectActiveServerGet(exampleRefSelectEntryS *bestEntry)
{
   zlStatusE status = ZL303XX_OK;

   /* Verify the input Pointer. */
   if (bestEntry == NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectActiveServerGet: bestEntry Input NULL",
            0,0,0,0,0,0);
      status = ZL303XX_INVALID_POINTER;
   }

   /* Lock the clock mutex before copying the data. */
   if (status == ZL303XX_OK)
   {
      status = exampleRefSelectMutexLock();
   }

   /* Copy the data and update the return status. */
   if (status == ZL303XX_OK)
   {
      /* Copy the last history value even if it is INVALID. */
      *bestEntry = exampleRefSelectAppParam.rt.selection.previousServer;

      /* Set the status based on if the entry is valid. */
      if (bestEntry->clockHandle == REF_SELECT_CONN_ID_INVALID)
      {
         status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
      }

      /* If we are here, the MUTEX was taken, so release it (ignore the unlock status). */
      (void)exampleRefSelectMutexUnlock();
   }

   return status;
}  /* exampleRefSelectActiveServerGet */

/* exampleRefSelectExecuteBmca */
/**
   Executes the selection algorithm configured by the Application and returns
   the data of the best server available. This does not replace the internal
   server being used or the server history.
   (To replace the internal data, enable Auto-Selection).

  Parameters:
   [out] bestEntry   Pointer for the data of the best row in the table.

  Return Value:
     ZL303XX_OK                      Success.
     ZL303XX_TABLE_ENTRY_NOT_FOUND   No Server is currently selected.
     ZL303XX_INVALID_POINTER         Provided pointer is NULL.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL    Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL    Failed to unlock mutex.

*******************************************************************************/
zlStatusE exampleRefSelectExecuteBmca(exampleRefSelectEntryS *bestEntry)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T bestIndex;

   /* Verify the input Pointer. */
   if (bestEntry == NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectExecuteBmca: bestEntry Input NULL",
            0,0,0,0,0,0);
      status = ZL303XX_INVALID_POINTER;
   }

   /* Simply call the internal routine and return the data. All MUTEX handling
    * is done internally. */
   if (status == ZL303XX_OK)
   {
      status = exampleRefSelectFindBest(&bestIndex, bestEntry);
   }

   return status;
}  /* exampleRefSelectExecuteBmca */

/* exampleRefSelectGetConnIdFromProtocolId */
/**
   Routine to get the corresponding connectionId (i.e. serverId) from the given
   Protocol clockId / streamId pair.

  Parameters:
   [in]  clockId     ClockId associated with the connection.
   [in]  streamId    StreamId associated with the connection.

   [out] connectionId   ConnectionId (serverId) associated with the protocol pair.

  Return Value:
     ZL303XX_OK                   The protocol entry exists and the return value is valid.
     ZL303XX_INVALID_POINTER      Input connectionId pointer is NULL.
     ZL303XX_PARAMETER_INVALID    Input clockId or streamId are INVALID.

*******************************************************************************/
zlStatusE exampleRefSelectGetConnIdFromProtocolId(
      Uint32T clockId, Uint32T streamId,
      Uint32T *connectionId)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T entryIndex = REF_SELECT_CONN_ID_INVALID;

   /* Verify the input pointers. */
   if (connectionId == NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "GetConnIdFromProtocolId: (ERROR) input pointer is NULL.",
            0,0,0,0,0,0);

      status = ZL303XX_INVALID_POINTER;
   }

   /* Verify the input indexes. */
   else if ((clockId == REF_SELECT_CONN_ID_INVALID) ||
            (streamId == REF_SELECT_CONN_ID_INVALID))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "GetConnIdFromProtocolId: (ERROR) clockId(%d) or streamId(%d) is INVALID.",
            clockId, streamId, 0,0,0,0);

      status = ZL303XX_PARAMETER_INVALID;
   }

   else
   {
      status = exampleRefSelectTableFindProtocolStream(streamId, clockId, &entryIndex);
   }

   /* Assign the return value. */
   if (status == ZL303XX_OK)
   {
      *connectionId = exampleRefSelectTable[entryIndex].connectionId;
   }

   return status;
}  /* exampleRefSelectGetConnIdFromProtocolId */

/* exampleRefSelectGetProtocolIdFromConnId */
/**
   Routine to get the corresponding Protocol clockId / streamId pair from the
   given connectionId (i.e. serverId).

  Parameters:
   [in]  connectionId   ConnectionId (serverId) associated with the protocol pair.

   [out] clockId        ClockId associated with the connectionId.
   [out] streamId       StreamId associated with the connectionId.

  Return Value:
     ZL303XX_OK                   The connectionId entry exists and the return values are valid.
     ZL303XX_INVALID_POINTER      Input clockId or streamId pointers are NULL.
     ZL303XX_PARAMETER_INVALID    Input connectionId is INVALID.

*******************************************************************************/
zlStatusE exampleRefSelectGetProtocolIdFromConnId(
      Uint32T connectionId,
      Uint32T *clockId, Uint32T *streamId)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T entryIndex = REF_SELECT_CONN_ID_INVALID;

   /* Verify the input pointers. */
   if ((clockId == NULL) || (streamId == NULL))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "GetProtocolIdFromConnId: (ERROR) input pointer is NULL.",
            0,0,0,0,0,0);

      status = ZL303XX_INVALID_POINTER;
   }

   /* Verify the input indexes. */
   else if (connectionId == REF_SELECT_CONN_ID_INVALID)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "GetProtocolIdFromConnId: (ERROR) connectionId(%d) is INVALID.",
            connectionId, 0,0,0,0,0);

      status = ZL303XX_PARAMETER_INVALID;
   }

   else
   {
      status = exampleRefSelectTableFindConnection(connectionId, &entryIndex);
   }

   /* Assign the return values. */
   if (status == ZL303XX_OK)
   {
      *clockId = exampleRefSelectTable[entryIndex].clockHandle;
      *streamId = exampleRefSelectTable[entryIndex].streamHandle;
   }

   return status;
}  /* exampleRefSelectGetProtocolIdFromConnId */

/*****************   MODULE INPUT INTERFACE ROUTINES (INTERNAL)  **************/

/**
  Function Name:
   exampleInterfacePtsf

  Details:
   Packages a PTSF update message into a Server message and sends it to the
   message Queue.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
void exampleInterfacePtsf(zl303xx_PtsfEventS *ptsfEventData)
{
   examplePtsfUpdateMsgS ptsfUpdateData;

   /* Fill in the required Server DB data from the received Event. */
   ptsfUpdateData.connectionId = ptsfEventData->streamHandle;
   ptsfUpdateData.flag = ptsfEventData->flag;
   ptsfUpdateData.value = ptsfEventData->value;
   ptsfUpdateData.mask = zl303xx_PtsfMaskGet(ptsfEventData->streamHandle, ptsfEventData->flag);

   /* Use the Input API of the Task to update the database. */
   (void)exampleRefSelectPtsfUpdate(ptsfUpdateData.connectionId, &ptsfUpdateData);
}

/*****************   MODULE HANDLER ROUTINES (INTERNAL)   *********************/

/**
  Function Name:
   exampleHandleProtocolClock

  Details:
   Internal routine for handling any Protocol Clock message.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleHandleProtocolClock(
      Uint32T clockId, exampleRefSelectActionE action, Uint8T msgLen, Uint8T *msgBuffer)
{
   zlStatusE status = ZL303XX_OK;

   /* Take the appropriate action.  */
   if (action == REF_SELECT_ACTION_UPDATE)
   {
      status = exampleHandleProtocolClockUpdate(clockId, msgLen, msgBuffer);
   }
   else if (action == REF_SELECT_ACTION_DELETE)
   {
      status = exampleHandleProtocolClockDelete(clockId, msgLen, msgBuffer);
   }

   /* Otherwise, the requested action is not supported. */
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleHandleProtocolClock: Message ACTION not supported - Action(%d)",
            action, 0,0,0,0,0);

      status = ZL303XX_PARAMETER_INVALID;
   }

   return status;
}

/**
  Function Name:
   exampleHandleProtocolClockUpdate

  Details:
   Internal routine for handling a Protocol Clock Update message.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleHandleProtocolClockUpdate(
      Uint32T clockId, Uint8T msgLen, Uint8T *msgBuffer)
{
   zlStatusE status = ZL303XX_OK;
   zlStatusE mStatus = ZL303XX_OK; /* Mutex Status */
   Uint32T rowIndex = REF_SELECT_CONN_ID_INVALID;

   /* Message Structure Type */
   exampleProtocolClockUpdateMsgS clockUpdateData;

   /* Check that the the message buffer is the expected size. */
   if (msgLen != sizeof(clockUpdateData))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleHandleProtocolClockUpdate: Message size mismatch - Expect(%d) but Received (%d)",
            sizeof(clockUpdateData), msgLen, 0,0,0,0);

      status = ZL303XX_DATA_CORRUPTION;
   }

   /* Lock the clock mutex. */
   else
   {
      mStatus = exampleRefSelectMutexLock();
      status = mStatus;
   }

   /* Check that the stream entry exists. */
   if (status == ZL303XX_OK)
   {
      /* Look up the table entry. */
      status = exampleRefSelectTableFindProtocolClock(clockId, &rowIndex);
   }

   /* If an active row was found OR
    * this is a new entry with space available in the table... */
   if ((status == ZL303XX_OK) ||
       ((status == ZL303XX_TABLE_ENTRY_NOT_FOUND) && (rowIndex < REF_SELECT_DB_ENTRIES_MAX)))
   {
      /* Convert the buffer to the proper structure. */
      memcpy(&clockUpdateData, msgBuffer, msgLen);

      /* Entry Found - UPDATE */
      exampleRefSelectTable[rowIndex].clockHandle = clockId;
      exampleRefSelectTable[rowIndex].streamHandle = REF_SELECT_CONN_ID_INVALID;

      exampleRefSelectTable[rowIndex].entryType = REF_SELECT_CONN_TYPE_PROTOCOL_CLOCK;

      exampleRefSelectTable[rowIndex].protocol.valid = ZL303XX_TRUE;
      exampleRefSelectTable[rowIndex].protocol.profile = clockUpdateData.profile;
      exampleRefSelectTable[rowIndex].protocol.bmcaData = clockUpdateData.bmcaData;
      exampleRefSelectTable[rowIndex].protocol.telecomData = clockUpdateData.telecomData;
      exampleRefSelectTable[rowIndex].protocol.tpPhaseData = clockUpdateData.tpPhaseData;

      /* If this is the first time for an entry for this clock, set some of the
       * other data fields.  */
      if (status == ZL303XX_TABLE_ENTRY_NOT_FOUND)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
               "exampleHandleProtocolClockUpdate: ADDED - Protocol Clock (%d)",
               clockId, 0,0,0,0,0);

         /* Change the status from NOT_FOUND to OK. */
         status = ZL303XX_OK;
      }

      /* Log the Clock Update */
      else
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
               "exampleHandleProtocolClockUpdate: UPDATED - Protocol Clock (%d)",
               clockId, 0,0,0,0,0);
      }
   }

   else if (status == ZL303XX_TABLE_FULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleHandleProtocolClockUpdate: ERROR - No Entry available for clock (%d)",
            clockId, 0,0,0,0,0);
   }

   /* Unlock the Mutex if taken. */
   if (mStatus == ZL303XX_OK)
   {
      /* Ignore the return value */
      (void)exampleRefSelectMutexUnlock();
   }

   return status;
}

/**
  Function Name:
   exampleHandleProtocolClockDelete

  Details:
   Internal routine for handling a Protocol Clock Delete message.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleHandleProtocolClockDelete(
      Uint32T clockId, Uint8T msgLen, Uint8T *msgBuffer)
{
   zlStatusE status = ZL303XX_OK;
   zlStatusE mStatus = ZL303XX_OK; /* Mutex Status */
   Uint32T rowIndex = REF_SELECT_CONN_ID_INVALID;

   /* Message Structure Type */
   exampleProtocolClockDeleteMsgS clockDeleteData;

   /* Check that the the message buffer is the expected size. */
   if (msgLen != sizeof(clockDeleteData))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleHandleProtocolClockDelete: Message size mismatch - Expect(%d) but Received (%d)",
            sizeof(clockDeleteData), msgLen, 0,0,0,0);

      status = ZL303XX_DATA_CORRUPTION;
   }

   /* Lock the clock mutex. */
   else
   {
      mStatus = exampleRefSelectMutexLock();
      status = mStatus;
   }

   /* Check that the stream entry exists. */
   if (status == ZL303XX_OK)
   {
      /* Look up the table entry. */
      status = exampleRefSelectTableFindProtocolClock(clockId, &rowIndex);
   }

   /* If an active row was found, delete it */
   if (status == ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
            "exampleHandleProtocolClockDelete: DELETED - Protocol Clock (%d)",
            clockId, 0,0,0,0,0);

      /* Convert the buffer to the proper structure. */
      memcpy(&clockDeleteData, msgBuffer, msgLen);

      /* Entry Found - Clear it */
      status = exampleRefSelectRowInit(&exampleRefSelectTable[rowIndex]);
   }

   /* Otherwise, we attempted to delete a Clock that does not exist.
    * Return OK. */
   else if ((status == ZL303XX_TABLE_ENTRY_NOT_FOUND) ||
            (status == ZL303XX_TABLE_FULL))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
            "exampleHandleProtocolClockDelete: NOTIFY - Protocol Clock (%d) does not exist",
            clockId, 0,0,0,0,0);

      status = ZL303XX_OK;
   }

   /* Unlock the Mutex if taken. */
   if (mStatus == ZL303XX_OK)
   {
      /* Ignore the return value */
      (void)exampleRefSelectMutexUnlock();
   }

   return status;
}

/**
  Function Name:
   exampleHandleProtocolStream

  Details:
   Internal routine for handling any Protocol Stream message.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleHandleProtocolStream(
      Uint32T streamId, exampleRefSelectActionE action, Uint8T msgLen, Uint8T *msgBuffer)
{
   zlStatusE status = ZL303XX_OK;

   /* Take the appropriate action.  */
   if (action == REF_SELECT_ACTION_ADD)
   {
        status = exampleHandleProtocolStreamAdd(streamId, msgLen, msgBuffer);
   }
   else if (action == REF_SELECT_ACTION_UPDATE)
   {
      status = exampleHandleProtocolStreamUpdate(streamId, msgLen, msgBuffer);
   }
   else if (action == REF_SELECT_ACTION_DELETE)
   {
      status = exampleHandleProtocolStreamDelete(streamId, msgLen, msgBuffer);
   }

   /* Otherwise, the requested action is not supported. */
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleHandleProtocolStream: Message ACTION not supported - Action(%d)",
            action, 0,0,0,0,0);

      status = ZL303XX_PARAMETER_INVALID;
   }

   return status;
}

/**
  Function Name:
   exampleHandleProtocolStreamAdd

  Details:
   Internal routine for handling a Protocol Stream Add message.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleHandleProtocolStreamAdd(
      Uint32T streamId, Uint8T msgLen, Uint8T *msgBuffer)
{
   zlStatusE status = ZL303XX_OK;
   zlStatusE mStatus = ZL303XX_OK; /* Mutex Status */
   Uint32T rowIndex = REF_SELECT_CONN_ID_INVALID;

   /* Message Structure Type */
   exampleProtocolStreamAddMsgS streamAddData;

   /* Check that the the message buffer is the expected size. */
   if (msgLen != sizeof(streamAddData))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleHandleProtocolStreamAdd: Message size mismatch - Expect(%d) but Received (%d)",
            sizeof(streamAddData), msgLen, 0,0,0,0);

      status = ZL303XX_DATA_CORRUPTION;
   }

   /* Lock the mutex. */
   else
   {
      mStatus = exampleRefSelectMutexLock();
      status = mStatus;
   }

   /* Check that the stream entry exists. */
   if (status == ZL303XX_OK)
   {
      /* Convert the buffer to the proper structure. */
      memcpy(&streamAddData, msgBuffer, msgLen);

      /* Look up the table entry. */
      status = exampleRefSelectTableFindProtocolStream(streamAddData.streamHandle, streamAddData.clockHandle, &rowIndex);
   }

   /* If an active row was found OR
    * this is a new entry with space available in the table... */
   if ((status == ZL303XX_OK) ||
       ((status == ZL303XX_TABLE_ENTRY_NOT_FOUND) && (rowIndex < REF_SELECT_DB_ENTRIES_MAX)))
   {
      /* Entry Found - UPDATE */
      exampleRefSelectTable[rowIndex].clockHandle = streamAddData.clockHandle;
      exampleRefSelectTable[rowIndex].streamHandle = streamAddData.streamHandle;
      exampleRefSelectTable[rowIndex].connectionId = streamAddData.serverId;

      exampleRefSelectTable[rowIndex].entryType = REF_SELECT_CONN_TYPE_PROTOCOL_STREAM;

      /* If this is the first time for this stream entry, recover some of the
       * other data fields.  */
      if (status == ZL303XX_TABLE_ENTRY_NOT_FOUND)
      {
         zl303xx_PtsfFlagE ptsfFlag;

         /* Retrieve all Initial PTSF data for this stream. */
         if (exampleRefSelectTable[rowIndex].ptsf.valid == ZL303XX_FALSE)
         {
            for (ptsfFlag = 0; ptsfFlag < ZL303XX_PTSF_NUM_FLAGS; ptsfFlag++)
            {
               exampleRefSelectTable[rowIndex].ptsf.value[ptsfFlag] = zl303xx_PtsfFlagGet(streamId, ptsfFlag);
               exampleRefSelectTable[rowIndex].ptsf.mask[ptsfFlag]  = zl303xx_PtsfMaskGet(streamId, ptsfFlag);
            }

            exampleRefSelectTable[rowIndex].ptsf.valid = ZL303XX_TRUE;
         }

         /* Change the status from NOT_FOUND to OK. */
         status = ZL303XX_OK;

         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
               "exampleHandleProtocolStreamAdd: ADDED - Stream (%d) on Clock (%d): Defined as Connection (%d)",
               streamId, streamAddData.clockHandle, streamAddData.serverId, 0,0,0);
      }

      else  /* Entry already existed but was re-Added */
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
               "exampleHandleProtocolStreamAdd: RE-ADDED - Connection (%d)",
               streamAddData.serverId, 0,0,0,0,0);
      }
   }

   else if (status == ZL303XX_TABLE_FULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleHandleProtocolStreamAdd: ERROR - No Entry available for stream (%d)",
            streamId, 0,0,0,0,0);
   }

   /* Unlock the Mutex if taken. */
   if (mStatus == ZL303XX_OK)
   {
      /* Ignore the return value */
      (void)exampleRefSelectMutexUnlock();
   }

   return status;
}

/**
  Function Name:
   exampleHandleProtocolStreamUpdate

  Details:
   Internal routine for handling a Protocol Stream Update message.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleHandleProtocolStreamUpdate(
      Uint32T streamId, Uint8T msgLen, Uint8T *msgBuffer)
{
   zlStatusE status = ZL303XX_OK;
   zlStatusE mStatus = ZL303XX_OK; /* Mutex Status */
   Uint32T rowIndex = REF_SELECT_CONN_ID_INVALID;

   /* Message Structure Type */
   exampleProtocolStreamUpdateMsgS streamUpdateData;

   /* Check that the the message buffer is the expected size. */
   if (msgLen != sizeof(streamUpdateData))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleHandleProtocolStreamUpdate: Message size mismatch - Expect(%d) but Received (%d)",
            sizeof(streamUpdateData), msgLen, 0,0,0,0);

      status = ZL303XX_DATA_CORRUPTION;
   }

   /* Lock the mutex. */
   else
   {
      mStatus = exampleRefSelectMutexLock();
      status = mStatus;
   }

   /* Check that the stream entry exists. */
   if (status == ZL303XX_OK)
   {
      /* Convert the buffer to the proper structure. */
      memcpy(&streamUpdateData, msgBuffer, msgLen);

      /* Look up the table entry. */
      status = exampleRefSelectTableFindProtocolStream(streamUpdateData.streamHandle, streamUpdateData.clockHandle, &rowIndex);
   }

   /* If an active row was found UPDATE it. */
   if (status == ZL303XX_OK)
   {
      /* Entry Found - UPDATE */
      exampleRefSelectTable[rowIndex].clockHandle = streamUpdateData.clockHandle;
      exampleRefSelectTable[rowIndex].streamHandle = streamUpdateData.streamHandle;

      exampleRefSelectTable[rowIndex].entryType = REF_SELECT_CONN_TYPE_PROTOCOL_STREAM;

      exampleRefSelectTable[rowIndex].protocol.valid = streamUpdateData.streamDataValid;
      /* Only copy any data if the stream is valid */
      if(streamUpdateData.streamDataValid == ZL303XX_TRUE)
      {
         exampleRefSelectTable[rowIndex].protocol.profile = streamUpdateData.profile;
         exampleRefSelectTable[rowIndex].protocol.bmcaData = streamUpdateData.bmcaData;
         exampleRefSelectTable[rowIndex].protocol.telecomData = streamUpdateData.telecomData;
         exampleRefSelectTable[rowIndex].protocol.tpPhaseData = streamUpdateData.tpPhaseData;
      }

      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
            "exampleHandleProtocolStreamUpdate: UPDATED - Protocol Stream (%d)",
            streamId, 0,0,0,0,0);
   }

   else if (status == ZL303XX_TABLE_ENTRY_NOT_FOUND)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
            "exampleHandleProtocolStreamUpdate: (ERROR) - Protocol Stream (%d) not found.",
            streamId, 0,0,0,0,0);
   }

   else if (status == ZL303XX_TABLE_FULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleHandleProtocolStreamUpdate: (ERROR) - No Entry available for Protocol stream (%d)",
            streamId, 0,0,0,0,0);
   }

   /* Unlock the Mutex if taken. */
   if (mStatus == ZL303XX_OK)
   {
      /* Ignore the return value */
      (void)exampleRefSelectMutexUnlock();
   }

   return status;
}

/**
  Function Name:
   exampleHandleProtocolStreamDelete

  Details:
   Internal routine for handling a Protocol Delete Update message.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleHandleProtocolStreamDelete(
      Uint32T streamId, Uint8T msgLen, Uint8T *msgBuffer)
{
   zlStatusE status = ZL303XX_OK;
   zlStatusE mStatus = ZL303XX_OK; /* Mutex Status */
   Uint32T rowIndex = REF_SELECT_CONN_ID_INVALID;

   /* Message Structure Type */
   exampleProtocolStreamDeleteMsgS streamDeleteData;

   /* Check that the the message buffer is the expected size. */
   if (msgLen != sizeof(streamDeleteData))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleHandleProtocolStreamDelete: Message size mismatch - Expect(%d) but Received (%d)",
            sizeof(streamDeleteData), msgLen, 0,0,0,0);

      status = ZL303XX_DATA_CORRUPTION;
   }

   /* Lock the mutex. */
   else
   {
      mStatus = exampleRefSelectMutexLock();
      status = mStatus;
   }

   /* Check that the stream entry exists. */
   if (status == ZL303XX_OK)
   {
      /* Convert the buffer to the proper structure. */
      memcpy(&streamDeleteData, msgBuffer, msgLen);

      /* Look up the table entry. */
      status = exampleRefSelectTableFindProtocolStream(streamId, streamDeleteData.clockHandle, &rowIndex);
   }

   /* If an active row was found, delete it */
   if (status == ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
            "exampleHandleProtocolStreamDelete: DELETING - Protocol Stream (%d)",
            streamId, 0,0,0,0,0);

      status = exampleRefSelectRowInit(&exampleRefSelectTable[rowIndex]);
   }

   /* Otherwise, we attempted to delete a Stream that does not exist.
    * Return OK. */
   else if ((status == ZL303XX_TABLE_ENTRY_NOT_FOUND) ||
            (status == ZL303XX_TABLE_FULL))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
            "exampleHandleProtocolStreamDelete: NOTIFY - Protocol Stream (%d) does not exist",
            streamId, 0,0,0,0,0);

      status = ZL303XX_OK;
   }

   /* Unlock the Mutex if taken. */
   if (mStatus == ZL303XX_OK)
   {
      /* Ignore the return value */
      (void)exampleRefSelectMutexUnlock();
   }

   return status;
}

/**
  Function Name:
   exampleHandlePtsf

  Details:
   Internal routine for handling a PTSF message.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleHandlePtsf(
      Uint32T connectionId, exampleRefSelectActionE action, Uint8T msgLen, Uint8T *msgBuffer)
{
   zlStatusE status = ZL303XX_OK;

   /* Take the appropriate action.  */
   if (action == REF_SELECT_ACTION_UPDATE)
   {
      status = exampleHandlePtsfUpdate(connectionId, msgLen, msgBuffer);
   }

   /* Otherwise, the requested action is not supported. */
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleHandlePtsf: Message ACTION not supported - Action(%d)",
            action, 0,0,0,0,0);

      status = ZL303XX_PARAMETER_INVALID;
   }

   return status;
}

/**
  Function Name:
   exampleHandlePtsfUpdate

  Details:
   Internal routine for handling a PTSF Update message.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleHandlePtsfUpdate(
      Uint32T connectionId, Uint8T msgLen, Uint8T *msgBuffer)
{
   zlStatusE status = ZL303XX_OK;
   zlStatusE mStatus = ZL303XX_OK; /* Mutex Status */
   Uint32T rowIndex = REF_SELECT_CONN_ID_INVALID;

   /* Message Structure Type */
   examplePtsfUpdateMsgS ptsfUpdateData;

   /* Check that the the message buffer is the expected size. */
   if (msgLen != sizeof(ptsfUpdateData))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleHandlePtsfUpdate: Message size mismatch - Expect(%d) but Received (%d)",
            sizeof(ptsfUpdateData), msgLen, 0,0,0,0);

      status = ZL303XX_DATA_CORRUPTION;
   }

   /* Lock the mutex. */
   else
   {
      mStatus = exampleRefSelectMutexLock();
      status = mStatus;
   }

   /* Check that the stream entry exists. */
   if (status == ZL303XX_OK)
   {
      /* Look up the table entry. */
      status = exampleRefSelectTableFindConnection(connectionId, &rowIndex);
   }

   /* If an active row was found... */
   if (status == ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
            "exampleHandlePtsfUpdate: UPDATED - PTSF for Connection (%d)",
            connectionId, 0,0,0,0,0);

      /* Convert the buffer to the proper structure. */
      memcpy(&ptsfUpdateData, msgBuffer, msgLen);

      /* Entry Found - UPDATE */
      exampleRefSelectTable[rowIndex].ptsf.value[ptsfUpdateData.flag] = ptsfUpdateData.value;
      exampleRefSelectTable[rowIndex].ptsf.mask[ptsfUpdateData.flag]  = ptsfUpdateData.mask;
   }

   else if (status == ZL303XX_TABLE_ENTRY_NOT_FOUND)
   {
      /* We have received a PTSF update from a stream that has not yet been created */
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleHandlePtsfUpdate: ERROR - No Entry for Connection (%d)",
            connectionId, 0,0,0,0,0);
   }
   else if (status == ZL303XX_TABLE_FULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleHandlePtsfUpdate: ERROR - No Entry available for Connection (%d)",
            connectionId, 0,0,0,0,0);
   }

   /* Unlock the Mutex if taken. */
   if (mStatus == ZL303XX_OK)
   {
      /* Ignore the return value */
      (void)exampleRefSelectMutexUnlock();
   }

   return status;
}

/**
  Function Name:
   exampleHandleAlgorithm

  Details:
   Internal routine for handling a Time-Sync Algorithm message.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleHandleAlgorithm(
      Uint32T serverId, exampleRefSelectActionE action, Uint8T msgLen, Uint8T *msgBuffer)
{
   zlStatusE status = ZL303XX_OK;

   /* Take the appropriate action.  */
   if (action == REF_SELECT_ACTION_UPDATE)
   {
      status = exampleHandleAlgorithmUpdate(serverId, msgLen, msgBuffer);
   }

   /* Otherwise, the requested action is not supported. */
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleHandleAlgorithm: Message ACTION not supported - Action(%d)",
            action, 0,0,0,0,0);

      status = ZL303XX_PARAMETER_INVALID;
   }

   return status;
}

/**
  Function Name:
   exampleHandleAlgorithmUpdate

  Details:
   Internal routine for handling a Time-Sync Algorithm Update message.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleHandleAlgorithmUpdate(
      Uint32T serverId, Uint8T msgLen, Uint8T *msgBuffer)
{
   zlStatusE status = ZL303XX_OK;
   zlStatusE mStatus = ZL303XX_OK; /* Mutex Status */
   Uint32T rowIndex = REF_SELECT_CONN_ID_INVALID;

   /* Message Structure Type */
   exampleAlgorithmUpdateMsgS algUpdateData;

   /* Check that the the message buffer is the expected size. */
   if (msgLen != sizeof(algUpdateData))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleHandleAlgorithmUpdate: Message size mismatch - Expect(%d) but Received (%d)",
            sizeof(algUpdateData), msgLen, 0,0,0,0);

      status = ZL303XX_DATA_CORRUPTION;
   }

   /* Lock the mutex. */
   else
   {
      mStatus = exampleRefSelectMutexLock();
      status = mStatus;
   }

   /* Check that the stream entry exists. */
   if (status == ZL303XX_OK)
   {
      /* Look up the table entry. */
      status = exampleRefSelectTableFindConnection(serverId, &rowIndex);
   }

   /* If an active row was found... */
   if (status == ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
            "exampleHandleAlgorithmUpdate: UPDATED - Status Flags for Server (%d)",
            serverId, 0,0,0,0,0);

      /* Convert the buffer to the proper structure. */
      memcpy(&algUpdateData, msgBuffer, msgLen);

      exampleRefSelectTable[rowIndex].algorithm.valid = ZL303XX_TRUE;

      /* Entry Found - UPDATE */
      exampleRefSelectTable[rowIndex].algorithm.statusFlag[algUpdateData.statusFlagId] = algUpdateData.value;
   }

   else if (status == ZL303XX_TABLE_ENTRY_NOT_FOUND)
   {
      /* We have received a PTSF update from a stream that has not yet been created */
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleHandleAlgorithmUpdate: ERROR - No Entry for Server (%d)",
            serverId, 0,0,0,0,0);
   }
   else if (status == ZL303XX_TABLE_FULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleHandleAlgorithmUpdate: ERROR - No Entry available for Server (%d)",
            serverId, 0,0,0,0,0);
   }

   /* Unlock the Mutex if taken. */
   if (mStatus == ZL303XX_OK)
   {
      /* Ignore the return value */
      (void)exampleRefSelectMutexUnlock();
   }

   return status;
}

/*****************   TASK INPUT API ROUTINES   ********************************/

/**
  Function Name:
   exampleRefSelectProtocolClockUpdate

  Details:
   Internal routine for packaging a Clock Update command into a Server Message
   and sending it to the Task Message-Q.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleRefSelectProtocolClockUpdate(
      Uint32T clockId, exampleProtocolClockUpdateMsgS *protocolClockDataS)
{
   zlStatusE status = ZL303XX_OK;

   /* General Task message. */
   exampleRefSelectMsgS dbMsg;

   /* Fill in the general message data */
   dbMsg.sourceId = REF_SELECT_SRC_MOD_PROT_CLOCK;
   dbMsg.connectionId = clockId;
   dbMsg.action = REF_SELECT_ACTION_UPDATE;
   dbMsg.device = NULL;
   dbMsg.bufLen = sizeof(exampleProtocolClockUpdateMsgS);

   /* Copy the data into the general message buffer. */
   memcpy(dbMsg.buffer, protocolClockDataS, dbMsg.bufLen);

   status = exampleRefSelectMsgQueueSend(&dbMsg);

   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectProtocolClockUpdate: FAILED to Send to Task for Protocol Clock (%d): Status (%d)",
            clockId, status, 0,0,0,0);
   }

   return status;
}

/**
  Function Name:
   exampleRefSelectProtocolClockDelete

  Details:
   Internal routine for packaging a Clock Delete command into a Server Message
   and sending it to the Task Message-Q.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleRefSelectProtocolClockDelete(
      Uint32T clockId, exampleProtocolClockDeleteMsgS *protocolClockDataS)
{
   zlStatusE status = ZL303XX_OK;

   /* General Task message. */
   exampleRefSelectMsgS dbMsg;

   /* Fill in the general message data */
   dbMsg.sourceId = REF_SELECT_SRC_MOD_PROT_CLOCK;
   dbMsg.connectionId = clockId;
   dbMsg.action = REF_SELECT_ACTION_DELETE;
   dbMsg.device = NULL;
   dbMsg.bufLen = sizeof(exampleProtocolClockDeleteMsgS);

   /* Copy the data into the general message buffer. */
   memcpy(dbMsg.buffer, protocolClockDataS, dbMsg.bufLen);

   status = exampleRefSelectMsgQueueSend(&dbMsg);

   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectProtocolClockDelete: FAILED to Send to Task for Protocol Clock (%d): Status (%d)",
            clockId, status, 0,0,0,0);
   }

   return status;
}

/**
  Function Name:
   exampleRefSelectProtocolStreamAdd

  Details:
   Internal routine for packaging a Stream Add command into a Server Message
   and sending it to the Task Message-Q.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleRefSelectProtocolStreamAdd(
      Uint32T streamId, exampleProtocolStreamAddMsgS *protocolStreamDataS)
{
   zlStatusE status = ZL303XX_OK;

   /* General Task message. */
   exampleRefSelectMsgS dbMsg;

   /* Fill in the general message data */
   dbMsg.sourceId = REF_SELECT_SRC_MOD_PROT_STREAM;
   dbMsg.connectionId = streamId;
   dbMsg.action = REF_SELECT_ACTION_ADD;
   dbMsg.device = NULL;
   dbMsg.bufLen = sizeof(exampleProtocolStreamAddMsgS);

   /* Copy the data into the general message buffer. */
   memcpy(dbMsg.buffer, protocolStreamDataS, dbMsg.bufLen);

   status = exampleRefSelectMsgQueueSend(&dbMsg);

   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleProtocolStreamAddMsgS: FAILED to Send to Task for Protocol Stream (%d): Status (%d)",
            streamId, status, 0,0,0,0);
   }

   return status;
}

/**
  Function Name:
   exampleRefSelectProtocolStreamUpdate

  Details:
   Internal routine for packaging a Stream Update command into a Server Message
   and sending it to the Task Message-Q.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleRefSelectProtocolStreamUpdate(
      Uint32T streamId, exampleProtocolStreamUpdateMsgS *protocolStreamDataS)
{
   zlStatusE status = ZL303XX_OK;

   /* General Task message. */
   exampleRefSelectMsgS dbMsg;

   /* Fill in the general message data */
   dbMsg.sourceId = REF_SELECT_SRC_MOD_PROT_STREAM;
   dbMsg.connectionId = streamId;
   dbMsg.action = REF_SELECT_ACTION_UPDATE;
   dbMsg.device = NULL;
   dbMsg.bufLen = sizeof(exampleProtocolStreamUpdateMsgS);

   /* Copy the data into the general message buffer. */
   memcpy(dbMsg.buffer, protocolStreamDataS, dbMsg.bufLen);

   status = exampleRefSelectMsgQueueSend(&dbMsg);

   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectProtocolStreamUpdate: FAILED to Send to Task for Protocol Stream (%d): Status (%d)",
            streamId, status, 0,0,0,0);
   }

   return status;
}

/**
  Function Name:
   exampleRefSelectProtocolStreamDelete

  Details:
   Internal routine for packaging a Stream Delete command into a Server Message
   and sending it to the Task Message-Q.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleRefSelectProtocolStreamDelete(
      Uint32T streamId, exampleProtocolStreamDeleteMsgS *protocolStreamDataS)
{
   zlStatusE status = ZL303XX_OK;

   /* General Task message. */
   exampleRefSelectMsgS dbMsg;

   /* Fill in the general message data */
   dbMsg.sourceId = REF_SELECT_SRC_MOD_PROT_STREAM;
   dbMsg.connectionId = streamId;
   dbMsg.action = REF_SELECT_ACTION_DELETE;
   dbMsg.device = NULL;
   dbMsg.bufLen = sizeof(exampleProtocolStreamDeleteMsgS);

   /* Copy the data into the general message buffer. */
   memcpy(dbMsg.buffer, protocolStreamDataS, dbMsg.bufLen);

   status = exampleRefSelectMsgQueueSend(&dbMsg);

   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectProtocolStreamDelete: FAILED to Send to Task for Protocol Clock (%d): Status (%d)",
            streamId, status, 0,0,0,0);
   }

   return status;
}

/**
  Function Name:
   exampleRefSelectPtsfUpdate

  Details:
   Internal routine for packaging a PTSF Update command into a Server Message
   and sending it to the Task Message-Q.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleRefSelectPtsfUpdate(
      Uint32T streamId, examplePtsfUpdateMsgS *ptsfUpdateDataS)
{
   zlStatusE status = ZL303XX_OK;

   /* General Task message. */
   exampleRefSelectMsgS dbMsg;

   /* Fill in the general message data */
   dbMsg.sourceId = REF_SELECT_SRC_MOD_PTSF;
   dbMsg.connectionId = streamId;
   dbMsg.action = REF_SELECT_ACTION_UPDATE;
   dbMsg.device = NULL;
   dbMsg.bufLen = sizeof(examplePtsfUpdateMsgS);

   /* Copy the data into the general message buffer. */
   memcpy(dbMsg.buffer, ptsfUpdateDataS, dbMsg.bufLen);

   status = exampleRefSelectMsgQueueSend(&dbMsg);

   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectPtsfUpdate: FAILED to Send to Task for Stream (%d): Status (%d)",
            streamId, status, 0,0,0,0);
   }

   return status;
}

/**
  Function Name:
   exampleRefSelectAlgorithmUpdate

  Details:
   Internal routine for packaging a Algorithm Update Update command into a Server Message
   and sending it to the Task Message-Q.

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleRefSelectAlgorithmUpdate(
      Uint32T serverID, exampleAlgorithmUpdateMsgS *algUpdateData)
{
   zlStatusE status = ZL303XX_OK;

   /* General Task message. */
   exampleRefSelectMsgS dbMsg;

   /* Fill in the general message data */
   dbMsg.sourceId = REF_SELECT_SRC_MOD_ALGORITHM;
   dbMsg.connectionId = serverID;
   dbMsg.action = REF_SELECT_ACTION_UPDATE;
   dbMsg.device = NULL;
   dbMsg.bufLen = sizeof(exampleAlgorithmUpdateMsgS);

   /* Copy the data into the general message buffer. */
   memcpy(dbMsg.buffer, algUpdateData, dbMsg.bufLen);

   status = exampleRefSelectMsgQueueSend(&dbMsg);

   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleAlgorithmUpdateMsgS: FAILED to Send to Task for Server (%d): Status (%d)",
            serverID, status, 0,0,0,0);
   }

   return status;
}

/**
  Function Name:
   exampleRefSelectConfigChange

  Details:
   Internal routine for sending a message to the Task Message-Q to inform it that
   an application configuration change has occurred. This will cause internal
   controls and timers to be re-evaluated and initiate server selection (if enabled).

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleRefSelectConfigChange(void)
{
   zlStatusE status = ZL303XX_OK;

   /* General Task message. */
   exampleRefSelectMsgS dbMsg;

   /* Fill in the general message data */
   dbMsg.sourceId = REF_SELECT_SRC_MOD_APP;
   dbMsg.connectionId = REF_SELECT_CONN_ID_INVALID;
   dbMsg.action = REF_SELECT_ACTION_UPDATE;
   dbMsg.device = NULL;
   dbMsg.bufLen = 0;

   /* No data to copy to the general message buffer. */

   /* Send the message. */
   status = exampleRefSelectMsgQueueSend(&dbMsg);

   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectConfigChange: FAILED (%d) to Send Message to Task",
            status, 0,0,0,0,0);
   }

   return status;
}  /* exampleRefSelectConfigChange */

/**
  Function Name:
   exampleRefSelectAppWakeUp

  Details:
   Internal routine for sending a 'Wake-up' (UNKNOWN) message to the Task
   Message-Q. This can be used whenever execution of Default Task processing
   is required or to break out of a Task WAIT state (i.e. to break out of a
   long refresh cycle).

  Return Value:  NA

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleRefSelectAppWakeUp(void)
{
   zlStatusE status = ZL303XX_OK;

   /* General Task message. */
   exampleRefSelectMsgS dbMsg;

   /* Fill in the general message data */
   dbMsg.sourceId = REF_SELECT_SRC_MOD_UNKNOWN;
   dbMsg.connectionId = REF_SELECT_CONN_ID_INVALID;
   dbMsg.action = REF_SELECT_ACTION_UPDATE;
   dbMsg.device = NULL;
   dbMsg.bufLen = 0;

   /* No data to copy to the general message buffer. */

   /* Send the message. */
   status = exampleRefSelectMsgQueueSend(&dbMsg);

   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectAppWakeUp: FAILED (%d) to Send Wake-Up to Task",
            status, 0,0,0,0,0);
   }

   return status;
}

/**************    BACKPLANE TO SERVER DATABASE TRANSLATIONS ROUTINES    ******/

/* exampleRefSelectSendMsgToBackplane */
/**
   Internal control routine to update the Time-Sync Algorithm and Protocol
   Line Cards with the selected server data.

  Parameters:
   [in]  bpMsg    Pointer to a message to send over a distributed backplane.

  Return Value:
     ZL303XX_OK                      Success.
     ZL303XX_INVALID_POINTER         The bpMsg pointer is NULL.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL    Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL    Failed to unlock mutex.

*******************************************************************************/
zlStatusE exampleRefSelectSendMsgToBackplane(exampleRefSelectBpMsgS *bpMsg)
{
   zlStatusE status = ZL303XX_OK;

   if (bpMsg == NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectSendMsgToBackplane: NULL input Pointer",
            0,0,0,0,0,0);

      /* Return immediately to save the extra indent below. */
      return ZL303XX_INVALID_POINTER;
   }

   /* If the message is from the application, send it to components. */
   if (bpMsg->source == REF_SELECT_SRC_MOD_APP)
   {
      /* The only valid message from the Application is an update to the other
       * modules in the system.     */
      if (bpMsg->action == REF_SELECT_ACTION_UPDATE)
      {
         /* UPDATE the TIME-SYNC SELECTION if configured. */
#if defined APR_INCLUDED
         if (exampleRefSelectAppParam.config.hwAlgParam != NULL)
         {
            if (bpMsg->data.bestRef.entryType == REF_SELECT_CONN_TYPE_ELECTRICAL)
            {
               /* Perform appropriate local electrical switching routines. */
               /* Electrical reference management is not included in the current Application. */
            }

            /* Update the TSA with the selected reference. */
            else if (bpMsg->data.bestRef.entryType == REF_SELECT_CONN_TYPE_PROTOCOL_STREAM)
            {
               zl303xx_AprLockStatusE aprLockStatus;
               Uint16T currentServerId = (Uint16T)(-1);
               zl303xx_BooleanE bSelectNewAprPacketRef = ZL303XX_FALSE;

               static zl303xx_BooleanE bInitialRefSelectionCompleted = ZL303XX_FALSE;

               status = zl303xx_AprGetDeviceCurrActiveRef(exampleRefSelectAppParam.config.hwAlgParam, &currentServerId);

               if (status == ZL303XX_OK)
               {
                  if (currentServerId != (Uint16T)(-1))  /* PACKET MODE */
                  {
                     /* Check if the serverId has changed */
                     if (currentServerId != (Uint16T)bpMsg->data.bestRef.connectionId)
                     {
                        /* Select the new packet server. */
                        bSelectNewAprPacketRef = ZL303XX_TRUE;
                     }
                     /* The new selection is the current one but it has never
                      * been selected yet (so APR may be in HOLDOVER). */
                     else if (bInitialRefSelectionCompleted == ZL303XX_FALSE)
                     {
                        /* Trigger the initial selection. */
                        bSelectNewAprPacketRef = ZL303XX_TRUE;
                     }
                  }
                  else  /* ELECTRICAL Mode if == -1 */
                  {
                     /* In Electric reference mode, if the electric reference fails, change to packet reference */
                     status = zl303xx_AprGetDeviceLockStatus(exampleRefSelectAppParam.config.hwAlgParam, &aprLockStatus);

                     if (status == ZL303XX_OK)
                     {
                        if (aprLockStatus == ZL303XX_LOCK_STATUS_REF_FAILED)
                        {
                           /* Select the packet server instead. */
                           bSelectNewAprPacketRef = ZL303XX_TRUE;
                        }
                     }
                  }

                  if ((status == ZL303XX_OK) &&
                      (bSelectNewAprPacketRef == ZL303XX_TRUE))
                  {
                     /* Select the new packet server. */
                     status = zl303xx_AprSetActiveRef(exampleRefSelectAppParam.config.hwAlgParam, (Uint16T)bpMsg->data.bestRef.connectionId);

                     if (status == ZL303XX_OK)
                     {
                        zl303xx_AprCGUStatusFlagsS cguStatusFlags;

                        /* A selection has been done at lease once. */
                        bInitialRefSelectionCompleted = ZL303XX_TRUE;

                        /* Check the CGU Status. */
                        status = zl303xx_AprGetCGUStatusFlags(exampleRefSelectAppParam.config.hwAlgParam, &cguStatusFlags);

                        if (status == ZL303XX_OK)
                        {
                           /*  Release HOLDOVER only if it was in HOLDHOVER, If not skip it */
                           if ((cguStatusFlags.state == ZL303XX_HOLDOVER) ||
                               (cguStatusFlags.state == ZL303XX_MANUAL_HOLDOVER) ||
                               (cguStatusFlags.state == ZL303XX_MANUAL_SERVO_HOLDOVER))
                           {
                              status = zl303xx_AprSetHoldover(exampleRefSelectAppParam.config.hwAlgParam, ZL303XX_FALSE);

                              if (status != ZL303XX_OK)
                              {
                                 ZL303XX_TRACE_ALWAYS("zl303xx_AprSetHoldover() failed with status=%d", status, 0,0,0,0,0);
                              }
                              else
                              {
                                 ZL303XX_TRACE_ALWAYS("-----> Clear Holdover on ConnId:%d", (Uint16T)bpMsg->data.bestRef.connectionId, 0,0,0,0,0);
                              }
                           }
                        }
                        else
                        {
                           ZL303XX_TRACE_ALWAYS("zl303xx_AprGetCGUStatusFlags() failed with status=%d", status, 0,0,0,0,0);
                        }
                     }
                  }
               }
            }

            /* A local clock or UNKNOWN server was selected. */
            else
            {
               zl303xx_AprCGUStatusFlagsS cguStatusFlags;
               status = zl303xx_AprGetCGUStatusFlags(exampleRefSelectAppParam.config.hwAlgParam, &cguStatusFlags);

               /* Set the local clock into HOLDOVER mode. */
               if ((cguStatusFlags.state != ZL303XX_HOLDOVER) &&
                   (cguStatusFlags.state != ZL303XX_MANUAL_HOLDOVER) &&
                   (cguStatusFlags.state != ZL303XX_MANUAL_SERVO_HOLDOVER))
               {
                  (void)zl303xx_AprSetHoldover(exampleRefSelectAppParam.config.hwAlgParam, ZL303XX_TRUE);
               }
            }
         }
#else
/* #warning MSCC: Package the required data into proprietary format and Tx over backplane to Time-Sync Module */
#endif

         /* UPDATE the PROTOCOL LINE CARDS */
#if defined ZLS30390_INCLUDED
         {
            Uint32T rowIndex;

            /* If no best entry was found, then set each clock to its own default. */
            if (bpMsg->data.bestRef.clockHandle == REF_SELECT_CONN_ID_INVALID)
            {
               for (rowIndex = 0; rowIndex < REF_SELECT_DB_ENTRIES_MAX; rowIndex++)
               {
                  /* Update only the Clocks. */
                  if ((exampleRefSelectTable[rowIndex].clockHandle != REF_SELECT_CONN_ID_INVALID) &&
                      (exampleRefSelectTable[rowIndex].entryType == REF_SELECT_CONN_TYPE_PROTOCOL_CLOCK))
                  {
                     (void)zl303xx_PtpClockParentDsSourceSet(
                           exampleRefSelectTable[rowIndex].clockHandle,
                           NULL, NULL);   /* No ParentDS and Port so restore the default. */
                  }
               }
            }

            /* Send the selected Server Data Set back to each clock for state updates. */
            else
            {
               for (rowIndex = 0; rowIndex < REF_SELECT_DB_ENTRIES_MAX; rowIndex++)
               {
                  /* Update only the Clocks. */
                  if ((exampleRefSelectTable[rowIndex].clockHandle != REF_SELECT_CONN_ID_INVALID) &&
                      (exampleRefSelectTable[rowIndex].entryType == REF_SELECT_CONN_TYPE_PROTOCOL_CLOCK))
                  {
                     (void)zl303xx_PtpClockApplyCompareData(
                                 exampleRefSelectTable[rowIndex].clockHandle,
                                 &bpMsg->data.bestRef.protocol.bmcaData);
                  }
               }
            }
         }
#else
/* #warning MSCC: Updates to the LINE card modules need to be done here. */
#endif

      }
      else  /* Application ACTION is not UPDATE. */
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
               "exampleRefSelectSendMsgToBackplane: Non-Supported Action for Application",
               bpMsg->action, 0,0,0,0,0);

         status = ZL303XX_UNSUPPORTED_OPERATION;
      }
   }

   /* If the message is from a PROTOCOL CLOCK, send it to the Application. */
   else if (bpMsg->source == REF_SELECT_SRC_MOD_PROT_CLOCK)
   {
      /* The only valid message from the Application is an update to the other
       * modules in the system.     */
      if (bpMsg->action == REF_SELECT_ACTION_UPDATE)
      {
         /* Map the backplane message to a Task message. */
         (void)exampleRefSelectProtocolClockUpdate(bpMsg->data.clockUpdate.clockHandle, &bpMsg->data.clockUpdate);
      }
      else if (bpMsg->action == REF_SELECT_ACTION_DELETE)
      {
         /* Map the backplane message to a Task message. */
         (void)exampleRefSelectProtocolClockDelete(bpMsg->data.clockDelete.clockHandle, &bpMsg->data.clockDelete);
      }
      else  /* Protocol Clock ACTION is not UPDATE or DELETE. */
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
               "exampleRefSelectSendMsgToBackplane: Non-Supported Action for PROTOCOL CLOCK",
               bpMsg->action, 0,0,0,0,0);

         status = ZL303XX_UNSUPPORTED_OPERATION;
      }
   }

   /* If the message is from a PROTOCOL STREAM, send it to the Application. */
   else if (bpMsg->source == REF_SELECT_SRC_MOD_PROT_STREAM)
   {
      /* The only valid message from the Application is an update to the other
       * modules in the system.     */
      if (bpMsg->action == REF_SELECT_ACTION_ADD)
      {
         /* Map the backplane message to a Task message. */
         (void)exampleRefSelectProtocolStreamAdd(bpMsg->data.streamAdd.streamHandle, &bpMsg->data.streamAdd);
      }
      else if (bpMsg->action == REF_SELECT_ACTION_UPDATE)
      {
         /* Map the backplane message to a Task message. */
         (void)exampleRefSelectProtocolStreamUpdate(bpMsg->data.streamUpdate.streamHandle, &bpMsg->data.streamUpdate);
      }
      else if (bpMsg->action == REF_SELECT_ACTION_DELETE)
      {
         /* Map the backplane message to a Task message. */
         (void)exampleRefSelectProtocolStreamDelete(bpMsg->data.streamDelete.streamHandle, &bpMsg->data.streamDelete);
      }
      else  /* Protocol Stream ACTION is unsupported. */
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
               "exampleRefSelectSendMsgToBackplane: Non-Supported Action for PROTOCOL STREAM",
               bpMsg->action, 0,0,0,0,0);

         status = ZL303XX_UNSUPPORTED_OPERATION;
      }
   }

   return status;
}

/**************    SERVER SELECTION ROUTINES (INTERNAL)   *********************/

/* exampleRefSelectFindBest */
/**
   Executes the BMC Algorithm configured by the Reference Selection Application
   to select the best server.

  Parameters:
   [out] bestIndex   The table index of the best entry.
   [out] bestEntry   Pointer for the data of the best row in the table.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       Input Pointer is NULL.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE exampleRefSelectFindBest(
      Uint32T *bestIndex,
      exampleRefSelectEntryS *bestEntry)
{
   zlStatusE status = ZL303XX_OK;
   zlStatusE mStatus = ZL303XX_OK; /* Mutex Status */

   /* Log the BMCA execution.    */
   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
         "exampleRefSelectFindBest: BMC Algorithm (%d); Manual Selection (%d)",
         exampleRefSelectAppParam.config.selection.bmcAlg,
         exampleRefSelectAppParam.config.selection.forcedRefId, 0,0,0,0);


   /* Lock the clock mutex before calling the selection routine. */
   if (status == ZL303XX_OK)
   {
      mStatus = exampleRefSelectMutexLock();
      status = mStatus;
   }

   /* Execute the configured selection routine. */
   if (status == ZL303XX_OK)
   {
      /* First, check if the server selection is manual. */
      if (exampleRefSelectAppParam.config.selection.forcedRefId != REF_SELECT_CONN_ID_INVALID)
      {
         if ((status = exampleRefSelectTableFindConnection(exampleRefSelectAppParam.config.selection.forcedRefId, bestIndex)) == ZL303XX_OK)
         {
            *bestEntry = exampleRefSelectTable[*bestIndex];
         }
         else
         {
            /* TABLE_FULL is also NOT_FOUND in this case. */
            status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
         }
      }

      /* Otherwise, use the configured BMC Algorithm. */
      else if ((exampleRefSelectAppParam.config.selection.bmcAlg == REF_SELECT_BMCA_DEFAULT_PROFILE) ||
               (exampleRefSelectAppParam.config.selection.bmcAlg == REF_SELECT_BMCA_POWER_PROFILE_V1))
      {
         status = exampleRefSelectBmc1588(bestIndex, bestEntry);
      }
      else if (exampleRefSelectAppParam.config.selection.bmcAlg == REF_SELECT_BMCA_TELECOM_PROFILE_FREQ)
      {
         status = exampleRefSelectBmcTelecom(bestIndex, bestEntry);
      }
      else if (exampleRefSelectAppParam.config.selection.bmcAlg == REF_SELECT_BMCA_TELECOM_PROFILE_PHASE)
      {
         status = exampleRefSelectBmcTelecomPhase(bestIndex, bestEntry);
      }
      else if (exampleRefSelectAppParam.config.selection.bmcAlg == REF_SELECT_BMCA_CUSTOM)
      {
         status = exampleRefSelectBmcCustom(bestIndex, bestEntry);
      }
      else
      {
         /* Since this routine should only be called internally, there appears to
          * be a corruption.    */
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
               "exampleRefSelectFindBest: BMC Algorithm (%d) Unknown",
               exampleRefSelectAppParam.config.selection.bmcAlg, 0,0,0,0,0);

         status = ZL303XX_DATA_CORRUPTION;
      }

      /* Release the Data MUTEX if it was taken (ignore the unlock status). */
      (void)exampleRefSelectMutexUnlock();
   }

   return status;
}

/* exampleRefSelectBestServersGet */
/**
   Returns an ordered array of the best N entries known to the Reference
   Selection Application.

  Parameters:
   [in]  numEntries  As an input; the number of entries requested.
                           (-1) indicates a request for all valid entries (this
                           assumes the array pointer can handle all entries).

   [out] numEntries     As an output; the number of entries in the output
                              array. 0 indicates no entries returned (status
                              should be checked).
   [out] bestEntries    Pointer for the output server data array.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       Either input Pointer is NULL.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE exampleRefSelectBestServersGet(
      Uint32T *numEntries,
      exampleRefSelectEntryS *bestEntries)
{
   zlStatusE status = ZL303XX_OK;
   zlStatusE mStatus = ZL303XX_OK; /* Mutex Status */

   Uint32T requestedEntries = 0;
   Uint32T maxEntries = 0;

   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
         "exampleRefSelectBestServersGet: BMC Algorithm (%d); Manual Selection (%d)",
         exampleRefSelectAppParam.config.selection.bmcAlg,
         exampleRefSelectAppParam.config.selection.forcedRefId, 0,0,0,0);

   /* Check the input pointers */
   if ((numEntries == NULL) || (bestEntries == NULL))
   {
      ZL303XX_TRACE_ALWAYS(
            "exampleRefSelectBestServersGet: Input NULL POINTER (%p;%p)",
            numEntries, bestEntries, 0,0,0,0);

      status = ZL303XX_INVALID_POINTER;
   }
   else  /* Figure out the MAX length of the output array */
   {
      requestedEntries = *numEntries;

      if ((*numEntries == (Uint32T)(-1)) ||
          (*numEntries > REF_SELECT_DB_ENTRIES_MAX))
      {
         maxEntries = REF_SELECT_DB_ENTRIES_MAX;
      }
      else
      {
         maxEntries = *numEntries;
      }

      /* Set the default return value. */
      *numEntries = 0;
   }

   /* Lock the clock mutex before sorting the entries. */
   if (status == ZL303XX_OK)
   {
      mStatus = exampleRefSelectMutexLock();
      status = mStatus;
   }

   /* First, Sort the Table to get the items in order. */
   if (status == ZL303XX_OK)
   {
      status = exampleRefSelectTableSort();
   }

   /* Copy the entries from the internal table to the output array. */
   if (status == ZL303XX_OK)
   {
      /* Counter for the copy loop. */
      Uint32T tblIndex;

      /* Copy 'N' entries to the output (if valid). */
      for(tblIndex = 0; tblIndex < maxEntries; tblIndex++)
      {
         /* The first time we hit an INVALID entry, we know we are at the end
          * of the Reference Table. */
         if (exampleRefSelectTableIsEntryInUse(&exampleRefSelectTable[tblIndex]) == ZL303XX_TRUE)
         {
            bestEntries[tblIndex] = exampleRefSelectTable[tblIndex];
         }
         else
         {
            break;
         }
      }

      *numEntries = tblIndex;
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
            "exampleRefSelectBestServersGet: Requested (%d); Actual (%d)",
            requestedEntries, *numEntries, 0,0,0,0);
   }

   /* Release the Data MUTEX if it was taken (ignore the unlock status). */
   if (mStatus == ZL303XX_OK)
   {
      (void)exampleRefSelectMutexUnlock();
   }

   return status;
}

/* exampleRefSelectTableSort */
/**
   Sorts the entire Reference Selection Table based on the Profile Configured.

  Return Value:  ZL303XX_OK                    Success.

  Notes: It is the responsibility of the calling routing to lock the Task Mutex
         prior to sorting.

*******************************************************************************/
zlStatusE exampleRefSelectTableSort(void)
{
   zlStatusE status = ZL303XX_OK;

   /* Log the BMCA execution.    */
   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
         "exampleRefSelectTableSort: BMC Algorithm (%d); Manual Selection (%d)",
         exampleRefSelectAppParam.config.selection.bmcAlg,
         exampleRefSelectAppParam.config.selection.forcedRefId, 0,0,0,0);

   /* Do a bubble-sort to reduce effort in the typical scenario where most
    * entries are close to ordered.    */
   if (status == ZL303XX_OK)
   {
      /* Counters for the sorting loops. */
      Uint32T tblIndex, lowEntry, topEntry;

      /* At most, "#tableEntries - 1" iterations will be needed. */
      Uint32T iteration = 0;

      /* If no records swap, the sort is completed.
       * Set to 1 so we do at least 1 iteration. */
      Uint32T swapCount = 1;
      while (swapCount != 0)
      {
         /* If nothing SWAPs, the sort is complete and we can exit the loop. */
         swapCount = 0;

         /* Start at the end and bubble the 'best' record to the top. */
         for((tblIndex = REF_SELECT_DB_ENTRIES_MAX-1);
             (tblIndex > iteration);
             (tblIndex--))
         {
            /* To make the table indexing more obvious in the code. */
            lowEntry = tblIndex;
            topEntry = tblIndex - 1;

            if (ZL303XX_TRUE == exampleRefSelectTableOrderTwoEntries(
                              exampleRefSelectAppParam.config.selection.bmcAlg,
                              &exampleRefSelectTable[topEntry],
                              &exampleRefSelectTable[lowEntry]))
            {
               swapCount++;
            }
         }

         /* Increment the loop count. */
         iteration++;
      }

      /* Now that the table is sorted, update some other indexes. */
      /* First, check if the server selection is manual. */
      if (exampleRefSelectAppParam.config.selection.forcedRefId != REF_SELECT_CONN_ID_INVALID)
      {
         Uint32T bestIndex;

         if (exampleRefSelectTableFindConnection(
                           exampleRefSelectAppParam.config.selection.forcedRefId,
                           &bestIndex) == ZL303XX_OK)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 3,
                  "exampleRefSelectTableSort: BMC Algorithm (%d); Forced Selection (%d)",
                  exampleRefSelectAppParam.config.selection.bmcAlg,
                  exampleRefSelectAppParam.config.selection.forcedRefId, 0,0,0,0);
         }
         else
         {
            /* TABLE_FULL is also NOT_FOUND in this case. */
            status = ZL303XX_TABLE_ENTRY_NOT_FOUND;

            ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
                  "exampleRefSelectTableSort: BMC Algorithm (%d); Forced Selection (%d) not found",
                  exampleRefSelectAppParam.config.selection.bmcAlg,
                  exampleRefSelectAppParam.config.selection.forcedRefId, 0,0,0,0);
         }
      }
   }

   return status;
}

/* exampleRefSelectTableOrderTwoEntries */
/**
   Orders 2 entries in the Reference Selection Table based on the Profile
   Configured.

  Return Value:
     ZL303XX_TRUE     If the order of the 2 entries were swapped.
     ZL303XX_FALSE    If the 2 entries were not swapped (already in order).

  Notes: It is the responsibility of the calling routing to lock the Task Mutex
         prior to sorting and validate all input pointers.

*******************************************************************************/
zl303xx_BooleanE exampleRefSelectTableOrderTwoEntries(
      exampleRefSelectBmcaTypeE profile,
      exampleRefSelectEntryS *entryA,
      exampleRefSelectEntryS *entryB)
{
   zl303xx_BooleanE validEntryA = ZL303XX_TRUE;
   zl303xx_BooleanE validEntryB = ZL303XX_TRUE;

   /* Compare only Valid, used rows. */
   validEntryA = exampleRefSelectTableIsEntryInUse(entryA);
   validEntryB = exampleRefSelectTableIsEntryInUse(entryB);

   /* Do the first sort based on whether each entry is used. */
   /* If both rows are valid, we will continue. */
   if (validEntryB == ZL303XX_FALSE)
   {
      /* If the second entry is invalid we know there won't be a sort regardless
       * of whether the first row is good or not. */
      return ZL303XX_FALSE;
   }
   else if (validEntryA == ZL303XX_FALSE)
   {
      /* If the first entry is invalid but the second is valid, then swap now
       * and return. */
      (void)exampleRefSelectTableSwapRows(entryA, entryB);
      return ZL303XX_TRUE;
   }

   /* If we are here then both rows were in-use. */

   /* G.8265 profile only: */
   /* Ignore any locked-out or WTR entries. */
   if (profile == REF_SELECT_BMCA_TELECOM_PROFILE_FREQ)
   {
      if (validEntryA == ZL303XX_TRUE)
      {
         if ((entryA->protection.lockout == ZL303XX_TRUE) ||
             (entryA->protection.wtr.active == ZL303XX_TRUE) ||
             (entryA->protocol.valid == ZL303XX_FALSE))
         {
            validEntryA = ZL303XX_FALSE;
         }
      }

      if (validEntryB == ZL303XX_TRUE)
      {
         if ((entryB->protection.lockout == ZL303XX_TRUE) ||
             (entryB->protection.wtr.active == ZL303XX_TRUE) ||
             (entryB->protocol.valid == ZL303XX_FALSE))
         {
            validEntryB = ZL303XX_FALSE;
         }
      }

      /* In this case, if both rows are invalid, we still want to order them
       * based on the BMCA. Only if one is or the other is invalid, do we want
       * to order and return.  */
      if ((validEntryA == ZL303XX_TRUE) && (validEntryB == ZL303XX_FALSE))
      {
         /* A already better so no sort needed. */
         return ZL303XX_FALSE;
      }
      else if ((validEntryA == ZL303XX_FALSE) && (validEntryB == ZL303XX_TRUE))
      {
         /* Swap now and return. */
         (void)exampleRefSelectTableSwapRows(entryA, entryB);
         return ZL303XX_TRUE;
      }
   }

   /* Now sort based on the profile. */
   /**********************************/
#if defined ZLS30390_INCLUDED 
   if ((profile == REF_SELECT_BMCA_DEFAULT_PROFILE) ||
       (profile == REF_SELECT_BMCA_POWER_PROFILE_V1))
   {
      if (zl303xx_PtpV2DefaultProfileBmcaComparison(
            &entryA->protocol.bmcaData,
            &entryA->protocol.bmcaData) < 0)
      {
         /* Entry B is better than A so swap the 2 rows. */
         (void)exampleRefSelectTableSwapRows(entryA, entryB);
         return ZL303XX_TRUE;
      }
   }
   else if (profile == REF_SELECT_BMCA_TELECOM_PROFILE_FREQ)
   {
      if (zl303xx_PtpTelecomProfileBmcaComparison(
            &entryA->protocol.telecomData,
            &entryB->protocol.telecomData,
            exampleRefSelectAppParam.config.protection.revertiveEn) < 0)
      {
         /* Entry B is better than A so swap the 2 rows. */
         (void)exampleRefSelectTableSwapRows(entryA, entryB);
         return ZL303XX_TRUE;
      }
   }
   else if (profile == REF_SELECT_BMCA_TELECOM_PROFILE_PHASE)
   {
      if (zl303xx_PtpBmcaG8275ProfileComparison(
            &entryA->protocol.tpPhaseData,
            &entryB->protocol.tpPhaseData, NULL) < 0)
      {
         /* Entry B is better than A so swap the 2 rows. */
         (void)exampleRefSelectTableSwapRows(entryA, entryB);
         return ZL303XX_TRUE;
      }
   }
   else
   {
      /* Since this routine should only be called internally, there appears to
       * be a corruption.    */
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectTableOrderTwoEntries: BMC Algorithm (%d) Unknown",
            profile, 0,0,0,0,0);
   }
#else
   /* #warning Can only be used with ZLS30390! */
#endif

   return ZL303XX_FALSE;
}  /* END exampleRefSelectTableOrderTwoEntries */

/* exampleRefSelectTableIsEntryInUse */
/**
   Checks if the Table entry is in use.

  Return Value:
     ZL303XX_TRUE     If the entry is being used.
     ZL303XX_FALSE    If the entry is not being used.

  Notes: It is the responsibility of the calling routing to lock the Task Mutex
         prior to calling and validate the input pointer.

*******************************************************************************/
zl303xx_BooleanE exampleRefSelectTableIsEntryInUse(
      exampleRefSelectEntryS *entryRow)
{
   /* Compare only Valid, used rows. */
   if ((entryRow->clockHandle == REF_SELECT_CONN_ID_INVALID) &&
       (entryRow->streamHandle == REF_SELECT_CONN_ID_INVALID))
   {
      return ZL303XX_FALSE;
   }
   else
   {
      return ZL303XX_TRUE;
   }
}

/* exampleRefSelectTableSwapRows */
/**
   Swaps the order of 2 rows in the table.

  Notes: It is the responsibility of the calling routing to lock the Task Mutex
         prior to calling and validate the input pointers.

*******************************************************************************/
void exampleRefSelectTableSwapRows(
      exampleRefSelectEntryS *entryA,
      exampleRefSelectEntryS *entryB)
{
   exampleRefSelectEntryS tempEntry;

   tempEntry = *entryA;
   *entryA = *entryB;
   *entryB = tempEntry;
}

/* exampleRefSelectBmc1588 */
/**
   Executes the Default 1588 BMC Algorithm to determine the best server in the table.

  Parameters:
   [out] bestIndex   The table index of the best entry.
   [out] bestEntry   Pointer to data for the best entry in the table.

  Return Value:
     ZL303XX_OK                      A best entry was determined.
     ZL303XX_TABLE_ENTRY_NOT_FOUND   No qualified entry was determined.

*******************************************************************************/
zlStatusE exampleRefSelectBmc1588(
      Uint32T *bestIndex,
      exampleRefSelectEntryS *bestEntry)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T rowIndex;

   /* Set the default return value (also used a flag within the loop. */
   *bestIndex = REF_SELECT_CONN_ID_INVALID;

   for (rowIndex = 0; rowIndex < REF_SELECT_DB_ENTRIES_MAX; rowIndex++)
   {
      /* Ignore any locked-out entries or ones without PTP data. */
      if ((exampleRefSelectTable[rowIndex].protection.lockout == ZL303XX_TRUE) ||
          (exampleRefSelectTable[rowIndex].protocol.valid == ZL303XX_FALSE))
         continue;

      /* Compare only Valid rows. */
      if ((exampleRefSelectTable[rowIndex].clockHandle != REF_SELECT_CONN_ID_INVALID) ||
          (exampleRefSelectTable[rowIndex].streamHandle != REF_SELECT_CONN_ID_INVALID))
      {
         /* The first valid row automatically becomes the best. */
         if (*bestIndex == REF_SELECT_CONN_ID_INVALID)
         {
            *bestIndex = rowIndex;
         }

         else
         {
         #if defined ZLS30390_INCLUDED 
            if (zl303xx_PtpV2DefaultProfileBmcaComparison(
                  &exampleRefSelectTable[*bestIndex].protocol.bmcaData,
                  &exampleRefSelectTable[rowIndex].protocol.bmcaData) < 0)
            {
               /* This entry is better than the existing one so select it. */
               *bestIndex = rowIndex;
            }
         #else
            /* #warning Can only be used with ZLS30390! */
         #endif
         }
      }
   }

   if (*bestIndex == REF_SELECT_CONN_ID_INVALID)
   {
      status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
   }
   else
   {
      *bestEntry = exampleRefSelectTable[*bestIndex];
   }

   return status;
}  /* exampleRefSelectBmc1588 */

/* exampleRefSelectBmcTelecom */
/**
   Executes the Telecom Profile BMC Algorithm to determine the best server
   in the table.

  Parameters:
   [out] bestIndex   The table index of the best entry.
   [out] bestEntry   Pointer to data for the best entry in the table.

  Return Value:
     ZL303XX_OK                      A best entry was determined.
     ZL303XX_TABLE_ENTRY_NOT_FOUND   No qualified entry was determined.

*******************************************************************************/
zlStatusE exampleRefSelectBmcTelecom(
      Uint32T *bestIndex,
      exampleRefSelectEntryS *bestEntry)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T rowIndex;

   /* Set the default return value (also used a flag within the loop. */
   *bestIndex = REF_SELECT_CONN_ID_INVALID;

   for (rowIndex = 0; rowIndex < REF_SELECT_DB_ENTRIES_MAX; rowIndex++)
   {
      /* Ignore any locked-out or WTR entries or ones without PTP data. */
      if ((exampleRefSelectTable[rowIndex].protection.lockout == ZL303XX_TRUE) ||
          (exampleRefSelectTable[rowIndex].protection.wtr.active == ZL303XX_TRUE) ||
          (exampleRefSelectTable[rowIndex].protocol.valid == ZL303XX_FALSE))
      {
         continue;
      }

      /* Compare only Valid rows. */
      if ((exampleRefSelectTable[rowIndex].clockHandle != REF_SELECT_CONN_ID_INVALID) ||
          (exampleRefSelectTable[rowIndex].streamHandle != REF_SELECT_CONN_ID_INVALID))
      {
         /* The first valid row automatically becomes the best. */
         if (*bestIndex == REF_SELECT_CONN_ID_INVALID)
         {
            *bestIndex = rowIndex;
         }

         else
         {
         #if defined ZLS30390_INCLUDED 
            if (zl303xx_PtpTelecomProfileBmcaComparison(
                  &exampleRefSelectTable[*bestIndex].protocol.telecomData,
                  &exampleRefSelectTable[rowIndex].protocol.telecomData,
                  exampleRefSelectAppParam.config.protection.revertiveEn) < 0)
            {
               /* This entry is better than the existing one so select it. */
               *bestIndex = rowIndex;
            }
         #else
            /* #warning Can only be used with ZLS30390! */
         #endif
         }
      }
   }

   if (*bestIndex == REF_SELECT_CONN_ID_INVALID)
   {
      status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
   }
   else
   {
      *bestEntry = exampleRefSelectTable[*bestIndex];
   }

   return status;
}  /* exampleRefSelectBmcTelecom */

/* exampleRefSelectBmcTelecomPhase */
/**
   Executes the G.8275 (Telecom Profile for Phase) BMC Algorithm to determine
   the best server in the table.

  Parameters:
   [out] bestIndex   The table index of the best entry.
   [out] bestEntry   Pointer to data for the best entry in the table.

  Return Value:
     ZL303XX_OK                      A best entry was determined.
     ZL303XX_TABLE_ENTRY_NOT_FOUND   No qualified entry was determined.

*******************************************************************************/
zlStatusE exampleRefSelectBmcTelecomPhase(
      Uint32T *bestIndex,
      exampleRefSelectEntryS *bestEntry)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T rowIndex;

   /* Set the default return value (also used a flag within the loop. */
   *bestIndex = REF_SELECT_CONN_ID_INVALID;

   for (rowIndex = 0; rowIndex < REF_SELECT_DB_ENTRIES_MAX; rowIndex++)
   {
      /* Ignore any locked-out entries or ones without PTP data. */
      if ((exampleRefSelectTable[rowIndex].protection.lockout == ZL303XX_TRUE) ||
          (exampleRefSelectTable[rowIndex].protocol.valid == ZL303XX_FALSE))
         continue;

      /* Compare only Valid rows. */
      if ((exampleRefSelectTable[rowIndex].clockHandle != REF_SELECT_CONN_ID_INVALID) ||
          (exampleRefSelectTable[rowIndex].streamHandle != REF_SELECT_CONN_ID_INVALID))
      {
         /* The first valid row automatically becomes the best. */
         if (*bestIndex == REF_SELECT_CONN_ID_INVALID)
         {
            *bestIndex = rowIndex;
         }

         else
         {
         #if defined ZLS30390_INCLUDED 
            if (zl303xx_PtpBmcaG8275ProfileComparison(
                  &exampleRefSelectTable[*bestIndex].protocol.tpPhaseData,
                  &exampleRefSelectTable[rowIndex].protocol.tpPhaseData, NULL) < 0)
            {
               /* This entry is better than the existing one so select it. */
               *bestIndex = rowIndex;
            }
         #else
            /* #warning Can only be used with ZLS30390! */
         #endif
         }
      }
   }

   if (*bestIndex == REF_SELECT_CONN_ID_INVALID)
   {
      status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
   }
   else
   {
      *bestEntry = exampleRefSelectTable[*bestIndex];
   }

   return status;
}  /* exampleRefSelectBmcTelecomPhase */

/* exampleRefSelectBmcCustom */
/**
   Executes a CUSTOM BMC Algorithm to determine the best server in the table.

  Parameters:
   [out] bestIndex   The table index of the best entry.
   [out] bestEntry   Pointer to data for the best entry in the table.

  Return Value:
     ZL303XX_OK                      A best entry was determined.
     ZL303XX_TABLE_ENTRY_NOT_FOUND   No qualified entry was determined.

*******************************************************************************/
zlStatusE exampleRefSelectBmcCustom(
      Uint32T *bestIndex,
      exampleRefSelectEntryS *bestEntry)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T rowIndex;

   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
         "exampleRefSelectBmcCustom: NOT Provided",
         0,0,0,0,0,0);

   /* Set the default return value (also used a flag within the loop. */
   *bestIndex = REF_SELECT_CONN_ID_INVALID;

   for (rowIndex = 0; rowIndex < REF_SELECT_DB_ENTRIES_MAX; rowIndex++)
   {
      /* Ignore any locked-out or WTR entries or ones without PTP data. */
      if ((exampleRefSelectTable[rowIndex].protection.lockout == ZL303XX_TRUE) ||
          (exampleRefSelectTable[rowIndex].protection.wtr.active == ZL303XX_TRUE) ||
          (exampleRefSelectTable[rowIndex].protocol.valid == ZL303XX_FALSE))
      {
         continue;
      }

      /* Compare only Valid rows. */
      if ((exampleRefSelectTable[rowIndex].clockHandle != REF_SELECT_CONN_ID_INVALID) ||
          (exampleRefSelectTable[rowIndex].streamHandle != REF_SELECT_CONN_ID_INVALID))
      {
         /* This will have to be defined by the USER.  Some option may be:
          * - LOCK & VALID bits must be good.
          * - OVERALL PTSF must be good.
          * - Decide based on other parameters:
          *   - quality level
          *   - packet rate
          *   - etc. */
         *bestIndex = REF_SELECT_CONN_ID_INVALID;
      }
   }

   if (*bestIndex == REF_SELECT_CONN_ID_INVALID)
   {
      status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
   }
   else
   {
      *bestEntry = exampleRefSelectTable[*bestIndex];
   }

   return status;
}  /* exampleRefSelectBmcCustom */

/*****************   UTILITY ROUTINES   ***************************************/

/* exampleRefSelectConfigCheck */
/**

   Checks the configuration values of the Reference Selection Application.

  Parameters:
   [in]  configParams   Pointer to the configuration structure to check the
                              values for.

  Return Value:
     ZL303XX_OK                      Success.
     ZL303XX_INVALID_POINTER         Input pointer is NULL.
     ZL303XX_PARAMETER_INVALID       A supplied parameter is INVALID.

  Notes:
   Assumes the input pointer for this internal routine is already veified.

*******************************************************************************/
zlStatusE exampleRefSelectConfigCheck(exampleRefSelectConfigS *configParams)
{
   zlStatusE status = ZL303XX_OK;

   /* Verify the supplied configuration parameters. */
   if (status == ZL303XX_OK)
   {
      /******************/
      /*** CGU device ***/
      /******************/
      /* Any value of hwAlgParam is valid, including NULL, so nothing to check */

      /**************************************/
      /*** Application Control Parameters ***/
      /**************************************/
      /* Verify that the Refresh value is within the required range.
       * - Cannot be '0' (NO_WAIT).
       * - Cannot be less than MIN
       * - Cannot be more than MAX unless it's DISABLED. */
      /* This could be combined into a single IF statement but it has been
       * separated for clarity. */
      if ((configParams->refreshRateMs == (Uint32T)OS_NO_WAIT) ||
          (configParams->refreshRateMs < REF_SELECT_APP_REFRESH_MIN) ||
          ((configParams->refreshRateMs > REF_SELECT_APP_REFRESH_MAX) && (configParams->refreshRateMs != REF_SELECT_TIMER_DISABLED)))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
               "exampleRefSelectConfigCheck: : Refresh rate (%d mSec) not valid (%d mSec <= rate <= %d mSec)",
               configParams->refreshRateMs, REF_SELECT_APP_REFRESH_MIN, REF_SELECT_APP_REFRESH_MAX, 0,0,0);

         status = ZL303XX_PARAMETER_INVALID;
      }

      /****************************/
      /*** Selection Parameters ***/
      /****************************/
      /* Check that the selection.autoRun value is BOOLEAN. */
      else if ((configParams->selection.autoRun != ZL303XX_TRUE) &&
               (configParams->selection.autoRun != ZL303XX_FALSE))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
               "exampleRefSelectConfigCheck: selection.autoRun (%d) Non-Boolean",
               configParams->selection.autoRun, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }

      /* Check that the BMC Algorithm value is valid. */
      else if (configParams->selection.bmcAlg > REF_SELECT_BMCA_CUSTOM)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
               "exampleRefSelectConfigCheck: selection.bmcAlg (%d) Unknown",
               configParams->selection.bmcAlg, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }

      /* No check required on the Manual/Forced Server ID. */

      /**********************************************/
      /*** Telecom Profile protection parameters. ***/
      /**********************************************/
      /* Check that the protection.revertiveEn value is BOOLEAN. */
      else if ((configParams->protection.revertiveEn != ZL303XX_TRUE) &&
               (configParams->protection.revertiveEn != ZL303XX_FALSE))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
               "exampleRefSelectConfigCheck: protection.revertiveEn (%d) Non-Boolean",
               configParams->protection.revertiveEn, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }

      /* Check that the protection.holdOffSec value is less than the MAX. */
      else if (configParams->protection.holdOffSec > REF_SELECT_HOLDOFF_TIMER_MAX)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
               "exampleRefSelectConfigCheck: protection.holdOffSec (%d) > MAX (%d)",
               configParams->protection.holdOffSec,
               REF_SELECT_HOLDOFF_TIMER_MAX, 0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }

      /* Check that the protection.squelchEn value is BOOLEAN. */
      else if ((configParams->protection.squelchEn != ZL303XX_TRUE) &&
               (configParams->protection.squelchEn != ZL303XX_FALSE))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
               "exampleRefSelectConfigCheck: protection.squelchEn (%d) Non-Boolean",
               configParams->protection.squelchEn, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   return status;
}  /* exampleRefSelectConfigCheck */

/* exampleRefSelectAppInitRunTimeParams */
/**

   Initializes the internal parameters of the Application. (Timers, counters and
   other variables that are maintained at run-time but have no initialization
   value).

  Parameters:
   [in]  rtParams    Pointer to the Application Run-Time parameters.

  Return Value:  ZL303XX_OK                      Success.

*******************************************************************************/
zlStatusE exampleRefSelectAppInitRunTimeParams(exampleRefSelectRunTimeS *rtParams)
{
   /* Selection Parameters. */
   /*************************/
   /* 'previous' selection is not valid at the start (MUTEX is not created yet). */
   (void)exampleRefSelectRowInit(&(rtParams->selection.previousServer));

   /* Telecom Profile Protection Mechanism. */
   /*****************************************/
   /* Hold-Off state and counters are all invalid. */
   rtParams->protection.holdOff.active = ZL303XX_FALSE;
   rtParams->protection.holdOff.tickStart = 0;
   rtParams->protection.holdOff.qlValue = 0;

   /* Internal Timer Values. */
   /**************************/
   /* Only the Application Refresh Timer is potentially on. */
   (void)exampleRefSelectConvertMsToTicks(
         exampleRefSelectAppParam.config.refreshRateMs,
         &rtParams->timer.appRefreshTicks);

   /* Others are off until the table is evaluated. */
   rtParams->timer.holdOffRemainingTicks = REF_SELECT_TIMER_DISABLED;
   rtParams->timer.wtrRemainingTicks = REF_SELECT_TIMER_DISABLED;

   return ZL303XX_OK;
}  /* exampleRefSelectAppInitRunTimeParams */

/* exampleRefSelectEqualServerEntries */
/**
   Determines if 2 Table entries are equal.

  Parameters:
   [in]  serverA     Pointer to server A.
   [in]  serverB     Pointer to server B.

  Return Value:
     ZL303XX_TRUE     Both servers are the same (or both NULL inputs).
     ZL303XX_FALSE    Both servers are different.

*******************************************************************************/
zl303xx_BooleanE exampleRefSelectEqualServerEntries(
      exampleRefSelectEntryS *serverA,
      exampleRefSelectEntryS *serverB)
{
   /* Set the default return value. */
   zl303xx_BooleanE isEqual = ZL303XX_FALSE;

   /* Both NULL: return EQUAL */
   if ((serverA == NULL) && (serverB == NULL))
   {
      isEqual = ZL303XX_TRUE;
   }

   /* Both VALID: evaluate */
   else if ((serverA != NULL) && (serverB != NULL))
   {
      /* Test the server identities. */
      if ((serverA->clockHandle == serverB->clockHandle) &&
          (serverA->streamHandle == serverB->streamHandle) &&
          (serverA->connectionId == serverB->connectionId))
      {
         isEqual = ZL303XX_TRUE;
      }
   }

   /* ELSE */
   /* One is VALID but one is NULL: return NOT-EQUAL (default) */

   return isEqual;
}  /* exampleRefSelectEqualServerEntries */

/* exampleRefSelectEvaluateAllWtrTimers */
/**
   Loops through the Server table and updates the state of all Wait-to-Restore
   timers.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      Input event pointer is NULL.

*******************************************************************************/
zlStatusE exampleRefSelectEvaluateAllWtrTimers(void)
{
   /* Counters used in the WTR calculations. */
   Uint32T totalTicks;
   Uint32T expiredTicks;

   /* Rather than call the OS routines multiple times... */
   Uint32T tickRate = OS_TICK_RATE_GET();
   Uint32T currentTick = OS_TICK_GET();

   /* Table indexes for the loop. */
   Uint32T rowIndex;
   exampleRefSelectEntryS *rowEntry = NULL;

   /* When setting the MSG-Q time-out value later, wake up the TASK at the
    * shortest required interval. */
   exampleRefSelectAppParam.rt.timer.wtrRemainingTicks = REF_SELECT_TIMER_DISABLED;

   for (rowIndex = 0; rowIndex < REF_SELECT_DB_ENTRIES_MAX; rowIndex++)
   {
      /* For easier code management, use a pointer to dereference the row. */
      rowEntry = &exampleRefSelectTable[rowIndex];

      if ((rowEntry->clockHandle != REF_SELECT_CONN_ID_INVALID) &&
          (rowEntry->entryType == REF_SELECT_CONN_TYPE_PROTOCOL_STREAM))
      {
         /* There are some conditions that will automatically disable WTR even
          * if previously active (may have changed via API).     */
         if ((rowEntry->protection.wtr.seconds == 0) ||
             (exampleRefSelectAppParam.config.selection.bmcAlg == REF_SELECT_BMCA_DEFAULT_PROFILE) ||
             (exampleRefSelectAppParam.config.selection.bmcAlg == REF_SELECT_BMCA_POWER_PROFILE_V1))
         {
            rowEntry->protection.wtr.active = ZL303XX_FALSE;
            rowEntry->protection.wtr.ticksRemaining = REF_SELECT_TIMER_DISABLED;
         }

         /* For any ACTIVE Timers, determine if they have expired. */
         else if (rowEntry->protection.wtr.active == ZL303XX_TRUE)
         {
            /* Calculate the remaining WTR ticks. */
            totalTicks = rowEntry->protection.wtr.seconds * tickRate;
            expiredTicks = (currentTick - rowEntry->protection.wtr.tickStart);

            ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 3,
                  "WTR UPDATE: StartTick(%d); CurrentTick(%d); ExpiredTicks(%d); WtrTicks(%d)",
                  rowEntry->protection.wtr.tickStart,
                  currentTick, expiredTicks, totalTicks, 0,0);

            /* If the Wait-to-Restore timer is expired, deactivate it.  */
            if (expiredTicks >= totalTicks)
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2, "WTR UPDATE: Timer expired for server (%d).",
                     rowEntry->connectionId, 0,0,0,0,0);

               /* Deactivate the timer. */
               rowEntry->protection.wtr.active = ZL303XX_FALSE;
               rowEntry->protection.wtr.ticksRemaining = REF_SELECT_TIMER_DISABLED;
            }

            /* Otherwise, Calculate the number of remaining Ticks. */
            else
            {
               rowEntry->protection.wtr.ticksRemaining = (totalTicks - expiredTicks);

               /* For active timers, record the lowest WTR interval. */
               if (rowEntry->protection.wtr.ticksRemaining < exampleRefSelectAppParam.rt.timer.wtrRemainingTicks)
               {
                  exampleRefSelectAppParam.rt.timer.wtrRemainingTicks = rowEntry->protection.wtr.ticksRemaining;
               }

               ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 3,
                     "WTR UPDATE: RemainingTicks (%d)",
                     rowEntry->protection.wtr.ticksRemaining, 0,0,0,0,0);
            }
         }
      }
   }  /* FOR LOOP */

   /* If the lowest WTR interval was '0', make it at least 1. */
   if (exampleRefSelectAppParam.rt.timer.wtrRemainingTicks == 0)
   {
      exampleRefSelectAppParam.rt.timer.wtrRemainingTicks = 1;
   }

   return ZL303XX_OK;
}  /* exampleRefSelectEvaluateAllWtrTimers */

/* exampleRefSelectHandleWtrOfServer */
/**
   Evaluates the configuration of the existing server and determines if the
   Wait-to-Restore timer needs to be started for it.

  Parameters:
   [in]  oldServer   The configuration of the old server.
   [in]  newServer   The configuration of the newly selected server.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      Input event pointer is NULL.

*******************************************************************************/
zlStatusE exampleRefSelectHandleWtrOfServer(
      exampleRefSelectEntryS *oldServer,
      exampleRefSelectEntryS *newServer)
{
   /* Table indexes for the lookup. */
   Uint32T rowIndex;
   exampleRefSelectEntryS *rowEntry = NULL;


   /* The following conditions must be met to start the WTR Timer for an entry.
    * 1) It was previously selected as the best server and is being replaced
    *    with another selection (which is why this routine was called).
    * 2) The Application is using the Telecom Profile.
    * 3) The previous selection was a VALID Protocol Stream entry.
    * 4) The previous selection had a WTR valid configured.
    * 5) The newServer is not the same as the oldServer.        **/

   /* Test the conditions. */
   if ((exampleRefSelectAppParam.config.selection.bmcAlg == REF_SELECT_BMCA_DEFAULT_PROFILE) ||  /* #2 */
       (exampleRefSelectAppParam.config.selection.bmcAlg == REF_SELECT_BMCA_POWER_PROFILE_V1) || /* #2 */
       (oldServer->clockHandle == REF_SELECT_CONN_ID_INVALID) ||                                 /* #3 */
       (oldServer->entryType != REF_SELECT_CONN_TYPE_PROTOCOL_STREAM) ||                         /* #3 */
       (oldServer->protection.wtr.seconds == 0) ||                                               /* #4 */
       (exampleRefSelectEqualServerEntries(oldServer, newServer) == ZL303XX_TRUE))                 /* #5 */
   {
      /* No WTR timer required. */
      return ZL303XX_OK;
   }

   /* If we've made it here, then activate this WTR timer. */
   /* First, get the actual entry from the internal table. If there is no entry
    * then there is nothing to start. */
   if (exampleRefSelectTableFindProtocolStream(oldServer->streamHandle, oldServer->clockHandle, &rowIndex) == ZL303XX_OK)
   {
      /* For easier code management, use a pointer to dereference the row. */
      rowEntry = &exampleRefSelectTable[rowIndex];

      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2, "WTR UPDATE: Timer ACTIVATED for server (%d).",
            rowEntry->connectionId, 0,0,0,0,0);

      /* Set the initial values of the timer. */
      rowEntry->protection.wtr.active = ZL303XX_TRUE;
      rowEntry->protection.wtr.tickStart = OS_TICK_GET();
      rowEntry->protection.wtr.ticksRemaining = rowEntry->protection.wtr.seconds * OS_TICK_RATE_GET();

      /* The minimum ticks value can be 1. */
      if (rowEntry->protection.wtr.ticksRemaining == 0)
      {
         rowEntry->protection.wtr.ticksRemaining = 1;
      }

      /* The lowest overall WTR interval (for MsgQ timeout) may need to be adjusted. */
      if (rowEntry->protection.wtr.ticksRemaining < exampleRefSelectAppParam.rt.timer.wtrRemainingTicks)
      {
         exampleRefSelectAppParam.rt.timer.wtrRemainingTicks = rowEntry->protection.wtr.ticksRemaining;
      }
   }

   return ZL303XX_OK;
}  /* exampleRefSelectHandleWtrOfServer */

/* exampleRefSelectConvertMsToTicks */
/**
   Calculates the number of CPU Ticks for the number of milliseconds provided
   (rounded down). Used to convert the Application Refresh Rate, Hold-Off
   remaining ticks, and WTR tick count in various places.

  Parameters:
   [in]  milliSec    The time value in milliseconds. (Also handles the
                           WAIT_FOREVER case by returning WAIT_FOREVER).

   [out] cpuTicks    The equivalent time in CPU Ticks. In it's current usage,
                           if the input time is less than 1 CPU tick, then '1' is
                           returned instead of to prevent a NO_WAIT situation.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      Input event pointer is NULL.

*******************************************************************************/
zlStatusE exampleRefSelectConvertMsToTicks(
      Uint32T milliSec,
      Uint32T *cpuTicks)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T cpuTicksPerSec = (Uint32T)OS_TICK_RATE_GET();


   /* Handle the special case of WAIT_FOREVER. */
   if (milliSec == REF_SELECT_TIMER_DISABLED)
   {
      *cpuTicks = REF_SELECT_TIMER_DISABLED;
   }

   /* Some numerical manipulation required to avoid 32-bit overflow and
    * to handle the special case. */
   /* No chance of 32-bit overflow */
   else if (((Uint32T)2000000000 / cpuTicksPerSec) > milliSec)
   {
      *cpuTicks = (cpuTicksPerSec * milliSec) / 1000;
   }

   else  /* Potential Overflow: Could be some rounding error but at this rate it's OK. */
   {
      *cpuTicks = (cpuTicksPerSec * (milliSec / 1000));
   }

   /* Make sure the result is never 0. */
   if (*cpuTicks == 0)
   {
      *cpuTicks = 1;
   }

   return status;
}  /* exampleRefSelectConvertMsToTicks */

/* exampleRefSelectDisplay */
/**
   Displays the Server Entries in the Table. By default, all entries are displayed
   whether VAILD or empty.

  Parameters:
   [in]  bShowHeader    Show the Table column names in the display.
   [in]  bShowValid     Show only VALID entries (empty entries are ignored).
   [in]  bShowBest      Shows only the BEST entry (all others are ignored).

  Return Value:  ZL303XX_OK          Success.

*******************************************************************************/
zlStatusE exampleRefSelectDisplay(
      zl303xx_BooleanE bShowHeader,
      zl303xx_BooleanE bShowValid,
      zl303xx_BooleanE bShowBest)
{
   zlStatusE status = ZL303XX_OK;

   Uint32T rowIndex;
   zl303xx_BooleanE bestValid;
   zl303xx_BooleanE bestEntry;
   zl303xx_BooleanE showThisRow;


   char headStr[]  = "   ROW | clkID | strm# | svrId |  QL | Type | PT-V | PS-V | AG-V |\0";
   char topStr[]   = "------------------------------------------------------------------\0";
   char entryStr[] = "* 1000 | 65535 | 65535 | 65535 | 255 | 0001 | 0001 | 0001 | 0001 |\0";
   char endStr[]   = "-----------------------------END----------------------------------\0";

   /* Display the Header if required. */
   if (bShowHeader == ZL303XX_TRUE)
   {
      ZL303XX_TRACE_ALWAYS("%s", headStr, 0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("%s", topStr, 0,0,0,0,0);
   }

   /* Determine if the current 'best' is VALID. */
   if (exampleRefSelectAppParam.rt.selection.previousServer.clockHandle == (Uint32T)(-1))
   {
      bestValid = ZL303XX_FALSE;
   }
   else
   {
      bestValid = ZL303XX_TRUE;
   }


   for (rowIndex = 0; rowIndex < REF_SELECT_DB_ENTRIES_MAX; rowIndex++)
   {
      /* By default, show every row. */
      showThisRow = ZL303XX_TRUE;

      /* Clear the bestEntry '*' indication. */
      bestEntry = ZL303XX_FALSE;
      /* Only use the bestEntry '*' if the current best is VALID. */
      if (bestValid == ZL303XX_TRUE)
      {
         bestEntry = exampleRefSelectEqualServerEntries(
               &exampleRefSelectAppParam.rt.selection.previousServer,
               &exampleRefSelectTable[rowIndex]);
      }

      /* Decide if a filter is on. */
      if ((bShowValid == ZL303XX_TRUE) &&
          (exampleRefSelectTable[rowIndex].clockHandle == REF_SELECT_CONN_ID_INVALID))
      {
         showThisRow = ZL303XX_FALSE;
      }
      else if ((bShowBest == ZL303XX_TRUE) &&
               (bestEntry != ZL303XX_TRUE))
      {
         showThisRow = ZL303XX_FALSE;
      }

      /* Show the Row if required */
      if (showThisRow == ZL303XX_TRUE)
      {
         sprintf(entryStr,
               "%s %4d | %5d | %5d | %5d | %3d | %4d | %4d | %4d | %4d",
               ((bestEntry == ZL303XX_TRUE) ? "*" : " "),
               rowIndex,
               exampleRefSelectTable[rowIndex].clockHandle,
               exampleRefSelectTable[rowIndex].streamHandle,
               exampleRefSelectTable[rowIndex].connectionId,
               exampleRefSelectTable[rowIndex].protocol.bmcaData.grandmasterClockQuality.clockClass,
               exampleRefSelectTable[rowIndex].entryType,
               exampleRefSelectTable[rowIndex].protocol.valid,
               exampleRefSelectTable[rowIndex].ptsf.valid,
               exampleRefSelectTable[rowIndex].algorithm.valid);
         ZL303XX_TRACE_ALWAYS("%s", entryStr, 0,0,0,0,0);
      }
   }

   /* Display the End row if required. */
   if (bShowHeader == ZL303XX_TRUE)
   {
      ZL303XX_TRACE_ALWAYS("%s", endStr, 0,0,0,0,0);
   }

   return status;
}

/* Show all Valid entries. */
zlStatusE exampleRefSelectShow(void)    { return exampleRefSelectDisplay(ZL303XX_TRUE, ZL303XX_TRUE, ZL303XX_FALSE); }
/* Show every entry. */
zlStatusE exampleRefSelectShowAll(void) { return exampleRefSelectDisplay(ZL303XX_TRUE, ZL303XX_FALSE, ZL303XX_FALSE);}
/* Show the BEST entry. */
zlStatusE exampleRefSelectShowBest(void){ return exampleRefSelectDisplay(ZL303XX_TRUE, ZL303XX_FALSE, ZL303XX_TRUE); }

/* exampleRefSelectConfigTrace */
/**
   Displays the Task configuration values. This is called when a configuration
   parameter changes (TRACE level 3) to compare the old vs new parameters. (If
   the second input is NULL, then just config-A is logged).

  Parameters:
   [in]  configA  Pointer to a Task Configuration structure (must be provided).
   [in]  configB  Pointer to a Task Configuration structure (optional).

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   configA is NULL (configB is optional).

*******************************************************************************/
zlStatusE exampleRefSelectConfigTrace(
      exampleRefSelectConfigS *configA,
      exampleRefSelectConfigS *configB)
{
   zlStatusE status = ZL303XX_OK;

   /* Check for a valid input pointer. */
   /* configB can be NULL. */
   if (configA == NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectConfigTrace: No Data Provided)",
            0,0,0,0,0,0);

      /* Return immediately to save indents below. */
      return ZL303XX_INVALID_POINTER;
   }

   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 3,
         "exampleRefSelectConfigTrace:  A       %s", (configB) ? "B" : " ", 0,0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 3,
         "  >> hwAlgParam              %s       %s",
         (configA->hwAlgParam == NULL) ? "NULL" : (configA->hwAlgParam),
         (configB) ? ((configB->hwAlgParam == NULL) ? "NULL" : (configB->hwAlgParam)) : 0,
          0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 3,
         "  >> refreshRateMs           %d       %d",
         configA->refreshRateMs, ((configB) ? configB->refreshRateMs : 0), 0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 3,
         "  >> autoRun                 %d       %d",
         configA->selection.autoRun, ((configB) ? configB->selection.autoRun : 0), 0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 3,
         "  >> bmcAlg                  %d       %d",
         configA->selection.bmcAlg, ((configB) ? configB->selection.bmcAlg : 0), 0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 3,
         "  >> forcedRefId             %d       %d",
         configA->selection.forcedRefId, ((configB) ? configB->selection.forcedRefId : 0), 0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 3,
         "  >> revertiveEn             %d       %d",
         configA->protection.revertiveEn, ((configB) ? configB->protection.revertiveEn : 0), 0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 3,
         "  >> holdOffSec              %d       %d",
         configA->protection.holdOffSec, ((configB) ? configB->protection.holdOffSec : 0), 0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 3,
         "  >> squelchEn               %d       %d",
         configA->protection.squelchEn, ((configB) ? configB->protection.squelchEn : 0), 0,0,0,0);

   ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 3, "***************************************", 0,0,0,0,0,0);

   return status;
}


/*****************   END   ****************************************************/
