

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains the packet timing signal fail (PTSF) implementation.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Os.h"
#include "zl303xx_Trace.h"

#include "zl303xx_Error.h"
#include "zl303xx_Ptsf.h"

#include <string.h>

/*****************   DEFINES   ************************************************/
#define PTSF_ARRAY_SIZE   (ZL303XX_PTSF_NUM_FLAGS / 32 + 1)
#define PTSF_INDEX(n)  ((n) / 32)
#define PTSF_BITPOS(n) ((n) % 32)

#define PTSF_ON(stream, n, member)                      \
           (PtsfStream[stream].member[PTSF_INDEX(n)] |= \
              (1 << PTSF_BITPOS(n)))

#define PTSF_OFF(stream, n, member)                     \
           (PtsfStream[stream].member[PTSF_INDEX(n)] &= \
              ~(1 << PTSF_BITPOS(n)))

/* ISSET() macros return 0 if the flag is clear, non-zero if the flag is raised.
 * Use in if() statements. */
#define PTSF_ISSET(stream, n, member) \
           (PtsfStream[stream].member[PTSF_INDEX(n)] & \
              (1 << PTSF_BITPOS(n)))

/* Macros for individual PTSF flags. */
#define PTSF_FLAG_ON(stream, n)     PTSF_ON(stream, n, flags)
#define PTSF_FLAG_OFF(stream, n)    PTSF_OFF(stream, n, flags)
#define PTSF_FLAG_ISSET(stream, n)  PTSF_ISSET(stream, n, flags)
#define PTSF_FLAG_ISSET_MASKED(stream, n) \
           (PTSF_FLAG_ISSET(stream, n) &  \
            PtsfStream[stream].mask[PTSF_INDEX(n)])

/* GET() macros return 0 if the flag is clear, 1 if the flag is raised.
 * Use when returning a boolean value to the user. */
#define PTSF_FLAG_GET(stream, n)         !!PTSF_FLAG_ISSET(stream, n)
#define PTSF_FLAG_GET_MASKED(stream, n)  !!PTSF_FLAG_ISSET_MASKED(stream, n)

/* Macros for individual PTSF masks. */
#define PTSF_MASK_ON(stream, n)     PTSF_ON(stream, n, mask)
#define PTSF_MASK_OFF(stream, n)    PTSF_OFF(stream, n, mask)
#define PTSF_MASK_ISSET(stream, n)  PTSF_ISSET(stream, n, mask)
#define PTSF_MASK_GET(stream, n)    !!PTSF_MASK_ISSET(stream, n)

#define PTSF_IS_INVALID(n)  ((n) >= ZL303XX_PTSF_NUM_FLAGS)

/*****************   STATIC DATA TYPES   **************************************/
typedef struct
{
   Uint32T flags[PTSF_ARRAY_SIZE];
   Uint32T mask[PTSF_ARRAY_SIZE];
} PtsfStreamS;

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/
static zlStatusE zl303xx_PtsfConfigStructCheck(zl303xx_PtsfConfigS *pConfig);
static zlStatusE zl303xx_PtsfInitCheck(const char *caller);
static zlStatusE zl303xx_PtsfStreamHandleCheck(Uint32T handle, const char *caller);
static zlStatusE zl303xx_PtsfFlagCheck(zl303xx_PtsfFlagE flag, const char *caller);

static void zl303xx_PtsfFlagSetInternal(Uint32T stream, zl303xx_PtsfFlagE flag, zl303xx_BooleanE val);
static void zl303xx_PtsfCallbacksExec(Uint32T stream, zl303xx_PtsfFlagE flag, zl303xx_BooleanE val);

/* Internal callbacks that make up the PTSF tree. */
static void zl303xx_PtsfCallbackInternalInit(void);
static void zl303xx_PtsfMiscEval(zl303xx_PtsfEventS *pEvent);
static void zl303xx_PtsfContractEval(zl303xx_PtsfEventS *pEvent);
static void zl303xx_PtsfTimeoutEval(zl303xx_PtsfEventS *pEvent);
static void zl303xx_PtsfUnusableEval(zl303xx_PtsfEventS *pEvent);
static void zl303xx_PtsfFrequencyEval(zl303xx_PtsfEventS *pEvent);
static void zl303xx_PtsfPhaseEval(zl303xx_PtsfEventS *pEvent);
static void zl303xx_PtsfOorChanged(zl303xx_PtsfEventS *pEvent);
static void zl303xx_PtsfFrequencyChanged(zl303xx_PtsfEventS *pEvent);
static void zl303xx_PtsfMainEval(zl303xx_PtsfEventS *pEvent);
static void zl303xx_PtsfAsCapabaleEval(zl303xx_PtsfEventS *pEvent);

/*****************   STATIC GLOBAL VARIABLES   ********************************/
static OS_MUTEX_ID PtsfMutex = OS_MUTEX_INVALID;

static PtsfStreamS *PtsfStream = NULL;
static Uint32T PtsfMaxStreams = 0;

static zl303xx_PtsfCallbackT PtsfCallbackUser[ZL303XX_PTSF_NUM_FLAGS];
static zl303xx_PtsfCallbackT PtsfCallbackInternal[ZL303XX_PTSF_NUM_FLAGS];

static const char *PtsfStr[] = {
      "PTSF-user",                  /* 0 */
      "PTSF-QLTooLow",
      "PTSF-LOSFromPHY",
      "PTSF-waitToRestoreActive",
      "PTSF-retryAttemptsFail",
      "PTSF-announceRejected",
      "PTSF-syncRejected",
      "PTSF-delayRespRejected",
      "PTSF-pdelayRespRejected",
      "PTSF-announceRateTooLow",
      "PTSF-syncRateTooLow",        /* 10 */
      "PTSF-delayRespRateTooLow",
      "PTSF-pdelayRespRateTooLow",
      "PTSF-lossAnnounce",
      "PTSF-lossSync",
      "PTSF-lossFollowUp",
      "PTSF-lossSyncOrFollowUp",
      "PTSF-lossDelayResp",
      "PTSF-lossPdelayResp",
      "PTSF-lossPdelayFollowUp",
      "PTSF-packetRejectReset",     /* 20 */
      "PTSF-lossTimeStamps",
      "PTSF-localTimeInaccuracyExceeded",
      "PTSF-networkTimeInaccuracyExceeded",
      "PTSF-pullInRangeOOR",
      "PTSF-PDVFreqMetricFail",
      "PTSF-freqVarTooHigh",
      "PTSF-notFreqLock",
      "PTSF-PDVPhaseMetricFail",
      "PTSF-phaseVarTooHigh",
      "PTSF-notPhaseLock",          /* 30 */
      "PTSF-misc",
      "PTSF-contract",
      "PTSF-timeout",
      "PTSF-unusable",
      "PTSF-frequency",
      "PTSF-phase",
      "PTSF",
      "PTSF-pDelayMultipleResponsesRecieved",
      "PTSF-PDelayAboveThreshold",
      "PTSF-pDelayAllowedLostResponses", /* 40 */
      "PTSF-asCapable"
};

static const char *PtsfStrInvalid = "(invalid)";

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* zl303xx_PtsfConfigStructInit */
/**
   Fills a zl303xx_PtsfConfigS structure with the default configuration for
   initializing the PTSF module.

  Parameters:
   [out]   pConfig  Default parameters for initializing the PTSF module.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pConfig is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtsfConfigStructInit(zl303xx_PtsfConfigS *pConfig)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtsfConfigStructInit";

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      if (pConfig == NULL)
      {
         ZL303XX_TRACE_ERROR("%s: Invalid pConfig pointer.", fnName, 0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
   }

   /* Initialize structure with default values. */
   if (status == ZL303XX_OK)
   {
      pConfig->maxStreams = ZL303XX_PTSF_DEFAULT_MAX_STREAMS;
   }

   return status;
}

/* zl303xx_PtsfInit */
/**
   Allocates memory for all internal PTSF structures and sets up the PTSF
   tree. This function sets all PTSF masks to TRUE (all flags are unmasked) by
   default.

  Parameters:
   [in]   pConfig  Parameters used to configure the PTSF module.

  Return Value:
     ZL303XX_OK                      Success.
     ZL303XX_INVALID_POINTER         pConfig is NULL.
     ZL303XX_PARAMETER_INVALID       pConfig->maxStreams is 0.
     ZL303XX_RTOS_MEMORY_FAIL        Failed to allocate memory.
     ZL303XX_RTOS_MUTEX_CREATE_FAIL  Failed to create mutex.

*******************************************************************************/
zlStatusE zl303xx_PtsfInit(zl303xx_PtsfConfigS *pConfig)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtsfInit";

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtsfConfigStructCheck(pConfig);
   }

   if (status == ZL303XX_OK)
   {
      /* Initialize callback arrays. */
      OS_MEMSET(PtsfCallbackUser, 0, sizeof(PtsfCallbackUser));
      zl303xx_PtsfCallbackInternalInit();

      /* Create the stream array for storing PTSF data. */
      PtsfStream = (PtsfStreamS *)OS_CALLOC(pConfig->maxStreams,
                                            sizeof(*PtsfStream));
      if (PtsfStream == NULL)
      {
         ZL303XX_TRACE_ERROR("%s: Memory allocate failed.", fnName, 0,0,0,0,0);
         status = ZL303XX_RTOS_MEMORY_FAIL;
      }
   }

   if (status == ZL303XX_OK)
   {
      PtsfMaxStreams = pConfig->maxStreams;

      /* Create the mutex. */
      PtsfMutex = OS_MUTEX_CREATE();
      if (PtsfMutex == OS_MUTEX_INVALID)
      {
         ZL303XX_TRACE_ERROR("%s: Mutex create failed.", fnName, 0,0,0,0,0);
         status = ZL303XX_RTOS_MUTEX_CREATE_FAIL;

         /* Mutex create failed, so free memory allocated by this function. */
         if (PtsfStream != NULL)
         {
            OS_FREE(PtsfStream);
         }
         PtsfStream = NULL;
      }
   }

   return status;
}

/* zl303xx_PtsfClose */
/**
   Frees all resources used by the PTSF module.

  Parameters:
      None.

  Return Value:
     ZL303XX_OK                      Success.
     ZL303XX_RTOS_MUTEX_DELETE_FAIL  Failed to delete mutex.

*******************************************************************************/
zlStatusE zl303xx_PtsfClose(void)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtsfClose";

   /* Delete the mutex if it has been created. */
   if (status == ZL303XX_OK)
   {
      if (PtsfMutex != OS_MUTEX_INVALID)
      {
         /* Lock the mutex. */
         if (OS_MUTEX_TAKE(PtsfMutex) != OS_OK)
         {
            ZL303XX_TRACE_ERROR("%s: Mutex lock failed.", fnName, 0,0,0,0,0);
            status = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
         }

         /* Delete the mutex. */
         if (status == ZL303XX_OK)
         {
            if (OS_MUTEX_DELETE(PtsfMutex) != OS_OK)
            {
               ZL303XX_TRACE_ERROR("%s: Mutex delete failed.", fnName, 0,0,0,0,0);
               status = ZL303XX_RTOS_MUTEX_DELETE_FAIL;
            }
         }

         if (status == ZL303XX_OK)
         {
            PtsfMutex = OS_MUTEX_INVALID;
         }
      }
   }

   /* Free memory allocated by zl303xx_PtsfInit(). */
   if (status == ZL303XX_OK)
   {
      if (PtsfStream != NULL)
      {
          OS_FREE(PtsfStream);
      }
      PtsfStream = NULL;
      PtsfMaxStreams = 0;
   }

   return status;
}



/* zl303xx_PtsfFlagSet */
/**
   Sets the value of a PTSF flag for a specific stream. The callback function
   registered for this flag will be executed if this function changes the state
   of the flag and the flag is not masked.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   flag          The PTSF flag to modify.
   [in]   value         Value to set for the PTSF flag. ZL303XX_TRUE indicates
                             some kind of failure.

  Return Value:  N/A  If an input parameter is out of range, the error will be logged on
                ZL303XX_MOD_ID_PTSF, level 1.

*******************************************************************************/
void zl303xx_PtsfFlagSet(Uint32T streamHandle, zl303xx_PtsfFlagE flag, zl303xx_BooleanE value)
{
   const char *fnName = "zl303xx_PtsfFlagSet";

   /* Check input parameters. */
   if ((zl303xx_PtsfInitCheck(fnName) != ZL303XX_OK) ||
       (zl303xx_PtsfStreamHandleCheck(streamHandle, fnName) != ZL303XX_OK) ||
       (zl303xx_PtsfFlagCheck(flag, fnName) != ZL303XX_OK) ||
       (ZL303XX_CHECK_BOOLEAN(value) != ZL303XX_OK))
   {
      return;
   }

   /* Set the flag. */
   zl303xx_PtsfFlagSetInternal(streamHandle, flag, value);
}

/* zl303xx_PtsfMaskSetAll */
/**
   Sets the value of all PTSF flags for a specific stream. The callback function
   registered for each flag will be executed if this function changes the state
   of a flag and the flag is not masked.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   value         Value to set for all PTSF flags of the stream.

  Return Value:  N/A  If an input parameter is out of range, the error will be logged on
                ZL303XX_MOD_ID_PTSF, level 1.

*******************************************************************************/
void zl303xx_PtsfFlagSetAll(Uint32T streamHandle, zl303xx_BooleanE value)
{
   const char *fnName = "zl303xx_PtsfFlagSetAll";
   zl303xx_PtsfFlagE flag;

   /* Check input parameters. */
   if ((zl303xx_PtsfInitCheck(fnName) != ZL303XX_OK) ||
       (zl303xx_PtsfStreamHandleCheck(streamHandle, fnName) != ZL303XX_OK) ||
       (ZL303XX_CHECK_BOOLEAN(value) != ZL303XX_OK))
   {
      return;
   }

   for (flag = 0; flag < ZL303XX_PTSF_NUM_FLAGS; flag++)
   {
      zl303xx_PtsfFlagSet(streamHandle, flag, value);
   }
}

/* zl303xx_PtsfFlagGet */
/**
   Returns the value of a PTSF flag for a specific stream with no mask applied.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   flag          The PTSF flag to get.

  Return Value:  zl303xx_BooleanE  PTSF flag value with no mask applied. ZL303XX_FALSE is
                           returned if an input parameter is out of range. This
                           error will be logged on ZL303XX_MOD_ID_PTSF level 1.

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtsfFlagGet(Uint32T streamHandle, zl303xx_PtsfFlagE flag)
{
   const char *fnName = "zl303xx_PtsfFlagGet";

   /* Check input parameters. */
   if ((zl303xx_PtsfInitCheck(fnName) != ZL303XX_OK) ||
       (zl303xx_PtsfStreamHandleCheck(streamHandle, fnName) != ZL303XX_OK) ||
       (zl303xx_PtsfFlagCheck(flag, fnName) != ZL303XX_OK))
   {
      return ZL303XX_FALSE;
   }

   /* Return the flag with no mask applied. */
   return PTSF_FLAG_GET(streamHandle, flag);
}

/* zl303xx_PtsfFlagGetAndMask */
/**
   Returns the value of a PTSF flag for a specific stream with the configured
   mask applied.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   flag          The PTSF flag to get.

  Return Value:  zl303xx_BooleanE  PTSF flag with mask applied. ZL303XX_FALSE is returned if
                           an input parameter is out of range. This error will
                           be logged on ZL303XX_MOD_ID_PTSF level 1.

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtsfFlagGetAndMask(Uint32T streamHandle, zl303xx_PtsfFlagE flag)
{
   const char *fnName = "zl303xx_PtsfFlagGetAndMask";

   /* Check input parameters. */
   if ((zl303xx_PtsfInitCheck(fnName) != ZL303XX_OK) ||
       (zl303xx_PtsfStreamHandleCheck(streamHandle, fnName) != ZL303XX_OK) ||
       (zl303xx_PtsfFlagCheck(flag, fnName) != ZL303XX_OK))
   {
      return ZL303XX_FALSE;
   }

   /* Return the flag with the mask applied. */
   return PTSF_FLAG_GET_MASKED(streamHandle, flag);
}

/* zl303xx_PtsfMaskSet */
/**
   Sets the mask of a PTSF flag for a specific stream. The callback function
   registered for this flag will be executed if the flag is currently TRUE and
   this function changes the value of the mask.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   flag          The PTSF mask to modify.
   [in]   mask          Mask to set for the PTSF flag. 0 masks PTSF flag
                             value changes. Any non-zero value unmasks the flag.

  Return Value:  N/A  If an input parameter is out of range, the error will be logged on
                ZL303XX_MOD_ID_PTSF, level 1.

*******************************************************************************/
void zl303xx_PtsfMaskSet(Uint32T streamHandle, zl303xx_PtsfFlagE flag, Uint32T mask)
{
   const char *fnName = "zl303xx_PtsfMaskSet";

   /* Check input parameters. */
   if ((zl303xx_PtsfInitCheck(fnName) != ZL303XX_OK) ||
       (zl303xx_PtsfStreamHandleCheck(streamHandle, fnName) != ZL303XX_OK) ||
       (zl303xx_PtsfFlagCheck(flag, fnName) != ZL303XX_OK))
   {
      return;
   }

   mask = !!mask;  /* Convert non-zero values to 1 */

   if (PTSF_MASK_GET(streamHandle, flag) != mask)
   {
      if (OS_MUTEX_TAKE(PtsfMutex) != OS_OK)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTSF, 1, "%s: Mutex take failed.", fnName,
                       0,0,0,0,0);
         return;
      }

      if (mask)
      {
         PTSF_MASK_ON(streamHandle, flag);
      }
      else
      {
         PTSF_MASK_OFF(streamHandle, flag);
      }

      /* Give up the mutex here so zl303xx_PtsfFlagSet() can be used from inside
       * a callback function. */
      OS_MUTEX_GIVE(PtsfMutex);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTSF, 2, "Stream %lu: %s mask changed to %d.",
                    streamHandle, PtsfStr[flag], mask, 0,0,0);

      /* If the PTSF flag is TRUE, then changing the mask must execute any
       * callbacks because:
       *    - if the flag is 0, the masked value is always 0
       *    - if the flag is 1, the masked value must change 0->1 or 1->0 */
      if (PTSF_FLAG_ISSET(streamHandle, flag))
      {
         zl303xx_PtsfCallbacksExec(streamHandle, flag, mask);
      }
   }
}

/* zl303xx_PtsfMaskSetAll */
/**
   Sets the mask of all PTSF flags for a specific stream. The callback function
   registered for a flag will be executed if the flag is currently TRUE and
   this function changes the value of its mask.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   mask          Mask to set for all PTSF flags.

  Return Value:  N/A  If an input parameter is out of range, the error will be logged on
                ZL303XX_MOD_ID_PTSF, level 1.

*******************************************************************************/
void zl303xx_PtsfMaskSetAll(Uint32T streamHandle, Uint32T mask)
{
   const char *fnName = "zl303xx_PtsfMaskSetAll";
   zl303xx_PtsfFlagE flag;

   /* Check input parameters. */
   if ((zl303xx_PtsfInitCheck(fnName) != ZL303XX_OK) ||
       (zl303xx_PtsfStreamHandleCheck(streamHandle, fnName) != ZL303XX_OK))
   {
      return;
   }

   for (flag = 0; flag < ZL303XX_PTSF_NUM_FLAGS; flag++)
   {
      zl303xx_PtsfMaskSet(streamHandle, flag, mask);
   }
}

/* zl303xx_PtsfMaskGet */
/**
   Returns the mask of a PTSF flag for a specific stream.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   flag          The PTSF mask being queried.

  Return Value:
     0  PTSF flag is masked, or an input parameter is out of range. This
              error will be logged on ZL303XX_MOD_ID_PTSF level 1.
     1  PTSF flag is unmasked.

*******************************************************************************/
Uint32T zl303xx_PtsfMaskGet(Uint32T streamHandle, zl303xx_PtsfFlagE flag)
{
   const char *fnName = "zl303xx_PtsfMaskGet";

   /* Check input parameters. */
   if ((zl303xx_PtsfInitCheck(fnName) != ZL303XX_OK) ||
       (zl303xx_PtsfStreamHandleCheck(streamHandle, fnName) != ZL303XX_OK) ||
       (zl303xx_PtsfFlagCheck(flag, fnName) != ZL303XX_OK))
   {
      return 0;
   }

   /* Return the mask. */
   return PTSF_MASK_GET(streamHandle, flag);
}

/* zl303xx_PtsfCallbackSet */
/**
   Sets a callback function to be executed when the state of a PSTF flag
   changes.

  Parameters:
   [in]   flag      The PTSF flag to set the callback for.
   [in]   callback  The callback function to use. Use NULL to indicate that
                         no function should be executed.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_NOT_RUNNING        PTSF module has not been initialized yet.
     ZL303XX_PARAMETER_INVALID  flag is out of range.

*******************************************************************************/
zlStatusE zl303xx_PtsfCallbackSet(zl303xx_PtsfFlagE flag, zl303xx_PtsfCallbackT callback)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtsfCallbackSet";

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtsfInitCheck(fnName);
   }
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtsfFlagCheck(flag, fnName);
   }

   /* Set the user's callback function. */
   if (status == ZL303XX_OK)
   {
      PtsfCallbackUser[flag] = callback;
   }

   return status;
}

/* zl303xx_PtsfFlagToStr */
/**
   Returns the string representation of a PTSF flag.

  Parameters:
   [in]   flag  The PTSF flag to convert.

  Return Value:
     (const char *)  String representation
     "(invalid)"     flag is out of range.

*******************************************************************************/
const char *zl303xx_PtsfFlagToStr(zl303xx_PtsfFlagE flag)
{
   if (PTSF_IS_INVALID(flag))
   {
      return PtsfStrInvalid;
   }

   return PtsfStr[flag];
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/* zl303xx_PtsfConfigStructCheck */
/** 

   Checks if a zl303xx_PtsfConfigS structure is valid.

  Parameters:
   [in]   pConfig  The structure to check.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pConfig is NULL.
     ZL303XX_PARAMETER_INVALID  pConfig->maxStreams == 0.

*******************************************************************************/
zlStatusE zl303xx_PtsfConfigStructCheck(zl303xx_PtsfConfigS *pConfig)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtsfConfigStructCheck";

   if (status == ZL303XX_OK)
   {
      if (pConfig == NULL)
      {
         ZL303XX_TRACE_ERROR("%s: Invalid pConfig pointer.", fnName, 0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
   }

   if (status == ZL303XX_OK)
   {
      if (pConfig->maxStreams == 0)
      {
         ZL303XX_TRACE_ERROR("%s: Invalid pConfig->maxStreams=%lu.",
                             fnName, pConfig->maxStreams, 0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   return status;
}

/* zl303xx_PtsfInitCheck */
/** 

   Checks if the PTSF module has been initialized yet.

  Parameters:
   [in]   caller  Name of the calling function.

  Return Value:
     ZL303XX_OK           Success.
     ZL303XX_NOT_RUNNING  Not initialized.

*******************************************************************************/
static zlStatusE zl303xx_PtsfInitCheck(const char *caller)
{
   if (PtsfStream == NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTSF, 1, "%s: PTSF module not initialized.",
                    caller, 0,0,0,0,0);
      return ZL303XX_NOT_RUNNING;
   }

   return ZL303XX_OK;
}

/* zl303xx_PtsfStreamHandleCheck */
/** 

   Checks if a stream handle is valid.

  Parameters:
   [in]   handle  Stream handle to check.
   [in]   caller  Name of the calling function.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  handle is out of range.

*******************************************************************************/
static zlStatusE zl303xx_PtsfStreamHandleCheck(Uint32T handle, const char *caller)
{
   if (handle >= PtsfMaxStreams)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTSF, 1, "%s: Invalid streamHandle=%lu.",
                    caller, handle, 0,0,0,0);
      return ZL303XX_PARAMETER_INVALID;
   }

   return ZL303XX_OK;
}

/* zl303xx_PtsfFlagCheck */
/** 

   Checks if a PTSF flag is valid.

  Parameters:
   [in]   flag    The PTSF flag to check.
   [in]   caller  Name of the calling function.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  flag is out of range.

*******************************************************************************/
static zlStatusE zl303xx_PtsfFlagCheck(zl303xx_PtsfFlagE flag, const char *caller)
{
   if (PTSF_IS_INVALID(flag))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTSF, 1, "%s: Invalid flag=%d.",
                    caller, flag, 0,0,0,0);
      return ZL303XX_PARAMETER_INVALID;
   }

   return ZL303XX_OK;
}

/* zl303xx_PtsfFlagSetInternal */
/** 

   Internal function to set a PTSF flag value. This function is called by
   zl303xx_PtsfFlagSet() and the internal callbacks that make up the PTSF tree.

   This function will call the user and internal callbacks if the value of the
   PTSF flag is changing.

  Parameters:
   [in]   stream  Handle to an existing stream.
   [in]   flag    The PTSF flag to modify.
   [in]   val     Value to set for the PTSF flag.

  Return Value:  N/A

*******************************************************************************/
static void zl303xx_PtsfFlagSetInternal(Uint32T stream, zl303xx_PtsfFlagE flag, zl303xx_BooleanE val)
{
   /* Only take the mutex if the flag will change value. */
   if (PTSF_FLAG_GET(stream, flag) != val)
   {
      if (OS_MUTEX_TAKE(PtsfMutex) != OS_OK)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTSF, 1, "zl303xx_PtsfFlagSetInternal: Mutex take failed.",
                       0,0,0,0,0,0);
         return;
      }

      if (val)
      {
         PTSF_FLAG_ON(stream, flag);
      }
      else
      {
         PTSF_FLAG_OFF(stream, flag);
      }

      /* Unlock the mutex here so zl303xx_PtsfFlagSet() can be used from inside a
       * callback function. */
      OS_MUTEX_GIVE(PtsfMutex);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTSF, 2, "Stream %lu: %s set to %d (mask=%d).",
                          stream, PtsfStr[flag], val, PTSF_MASK_GET(stream, flag),
                          0,0);

      /* Only execute the callbacks if the flag is not masked. */
      if (PTSF_MASK_ISSET(stream, flag))
      {
         zl303xx_PtsfCallbacksExec(stream, flag, val);
      }
   }
}

/* zl303xx_PtsfCallbacksExec */
/** 

   Executes the user and internal callbacks for a PTSF flag change.

  Parameters:
   [in]   stream  Handle to an existing stream.
   [in]   flag    The PTSF flag to modify.
   [in]   val     New value of the PTSF flag.

  Return Value:  N/A

*******************************************************************************/
static void zl303xx_PtsfCallbacksExec(Uint32T stream, zl303xx_PtsfFlagE flag, zl303xx_BooleanE val)
{
   zl303xx_PtsfEventS event;

   /* This trace message covers the case when a PTSF flag OR PTSF mask causes
    * this function to be called. */
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTSF, 3, "Stream %lu: %s changed to %d.",
                 stream, PtsfStr[flag], val, 0,0,0);

   event.streamHandle = stream;
   event.flag         = flag;
   event.value        = val;

   if (PtsfCallbackUser[flag] != NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTSF, 4, "Stream %lu: %s callback executing...",
                    stream, PtsfStr[flag], 0,0,0,0);

      PtsfCallbackUser[flag](&event);
   }

   if (PtsfCallbackInternal[flag] != NULL)
   {
      PtsfCallbackInternal[flag](&event);
   }
}

/* zl303xx_PtsfCallbackInternalInit */
/** 

   Initializes the internal callback array to create the PTSF tree.

  Parameters:
      None
  Return Value:  N/A

*******************************************************************************/
static void zl303xx_PtsfCallbackInternalInit(void)
{
   memset(PtsfCallbackInternal, 0, sizeof(PtsfCallbackInternal));

   /* Flags that generate PTSF-misc */
   PtsfCallbackInternal[ZL303XX_PTSF_USER]                   = zl303xx_PtsfMiscEval;
   PtsfCallbackInternal[ZL303XX_PTSF_QL_TOO_LOW]             = zl303xx_PtsfMiscEval;
   PtsfCallbackInternal[ZL303XX_PTSF_LOS_FROM_PHY]           = zl303xx_PtsfMiscEval;
   PtsfCallbackInternal[ZL303XX_PTSF_WAIT_TO_RESTORE_ACTIVE] = zl303xx_PtsfMiscEval;

   /* Flags that generate PTSF-contract */
   PtsfCallbackInternal[ZL303XX_PTSF_RETRY_ATTEMPTS_FAIL]          = zl303xx_PtsfContractEval;
   PtsfCallbackInternal[ZL303XX_PTSF_ANNOUNCE_REJECTED]            = zl303xx_PtsfContractEval;
   PtsfCallbackInternal[ZL303XX_PTSF_SYNC_REJECTED]                = zl303xx_PtsfContractEval;
   PtsfCallbackInternal[ZL303XX_PTSF_DELAY_RESP_REJECTED]          = zl303xx_PtsfContractEval;
   PtsfCallbackInternal[ZL303XX_PTSF_PEER_DELAY_RESP_REJECTED]     = zl303xx_PtsfContractEval;
   PtsfCallbackInternal[ZL303XX_PTSF_ANNOUNCE_RATE_TOO_LOW]        = zl303xx_PtsfContractEval;
   PtsfCallbackInternal[ZL303XX_PTSF_SYNC_RATE_TOO_LOW]            = zl303xx_PtsfContractEval;
   PtsfCallbackInternal[ZL303XX_PTSF_DELAY_RESP_RATE_TOO_LOW]      = zl303xx_PtsfContractEval;
   PtsfCallbackInternal[ZL303XX_PTSF_PEER_DELAY_RESP_RATE_TOO_LOW] = zl303xx_PtsfContractEval;

   /* Flags that generate PTSF-timeout */
   PtsfCallbackInternal[ZL303XX_PTSF_LOSS_ANNOUNCE]             = zl303xx_PtsfTimeoutEval;
   PtsfCallbackInternal[ZL303XX_PTSF_LOSS_SYNC]                 = zl303xx_PtsfTimeoutEval;
   PtsfCallbackInternal[ZL303XX_PTSF_LOSS_FOLLOW_UP]            = zl303xx_PtsfTimeoutEval;
   PtsfCallbackInternal[ZL303XX_PTSF_LOSS_SYNC_OR_FOLLOW_UP]    = zl303xx_PtsfTimeoutEval;
   PtsfCallbackInternal[ZL303XX_PTSF_LOSS_DELAY_RESP]           = zl303xx_PtsfTimeoutEval;
   PtsfCallbackInternal[ZL303XX_PTSF_LOSS_PEER_DELAY_RESP]      = zl303xx_PtsfTimeoutEval;
   PtsfCallbackInternal[ZL303XX_PTSF_LOSS_PEER_DELAY_FOLLOW_UP] = zl303xx_PtsfTimeoutEval;

   /* Flags that generate PTSF-unusable */
   PtsfCallbackInternal[ZL303XX_PTSF_PACKET_REJECT_RESET]         = zl303xx_PtsfUnusableEval;
   PtsfCallbackInternal[ZL303XX_PTSF_LOSS_TIME_STAMPS]            = zl303xx_PtsfUnusableEval;
   PtsfCallbackInternal[ZL303XX_PTSF_LOCAL_TIME_INACC_EXCEEDED]   = zl303xx_PtsfUnusableEval;
   PtsfCallbackInternal[ZL303XX_PTSF_NETWORK_TIME_INACC_EXCEEDED] = zl303xx_PtsfUnusableEval;

   /* Flags that generate PTSF-frequency */
   PtsfCallbackInternal[ZL303XX_PTSF_PDV_FREQ_METRIC_FAIL] = zl303xx_PtsfFrequencyEval;
   PtsfCallbackInternal[ZL303XX_PTSF_FREQ_VAR_TOO_HIGH]    = zl303xx_PtsfFrequencyEval;
   PtsfCallbackInternal[ZL303XX_PTSF_NOT_FREQ_LOCK]        = zl303xx_PtsfFrequencyEval;

   /* Flags that generate PTSF-phase */
   PtsfCallbackInternal[ZL303XX_PTSF_PDV_PHASE_METRIC_FAIL] = zl303xx_PtsfPhaseEval;
   PtsfCallbackInternal[ZL303XX_PTSF_PHASE_VAR_TOO_HIGH]    = zl303xx_PtsfPhaseEval;
   PtsfCallbackInternal[ZL303XX_PTSF_NOT_PHASE_LOCK]        = zl303xx_PtsfPhaseEval;

   /* PTSF-pullInRangeOOR generates PTSF-frequency and PTSF-phase */
   PtsfCallbackInternal[ZL303XX_PTSF_PULL_IN_RANGE_OOR] = zl303xx_PtsfOorChanged;

   /* PTSF-frequency generates PTSF-phase and the overall PTSF flag */
   PtsfCallbackInternal[ZL303XX_PTSF_FREQUENCY] = zl303xx_PtsfFrequencyChanged;

   /* Flags that generate PTSF-asCapable */
   PtsfCallbackInternal[ZL303XX_PTSF_PEER_DELAY_MULTI_RESP]            = zl303xx_PtsfAsCapabaleEval;
   PtsfCallbackInternal[ZL303XX_PTSF_PEER_DELAY_ABOVE_THRESHOLD]       = zl303xx_PtsfAsCapabaleEval;
   PtsfCallbackInternal[ZL303XX_PTSF_PEER_DELAY_ALLOWED_LOST_RESPONSE] = zl303xx_PtsfAsCapabaleEval;

   /* Flags that generate overall PTSF flag */
   PtsfCallbackInternal[ZL303XX_PTSF_MISC]       = zl303xx_PtsfMainEval;
   PtsfCallbackInternal[ZL303XX_PTSF_CONTRACT]   = zl303xx_PtsfMainEval;
   PtsfCallbackInternal[ZL303XX_PTSF_TIMEOUT]    = zl303xx_PtsfMainEval;
   PtsfCallbackInternal[ZL303XX_PTSF_UNUSABLE]   = zl303xx_PtsfMainEval;
   PtsfCallbackInternal[ZL303XX_PTSF_PHASE]      = zl303xx_PtsfMainEval;
   PtsfCallbackInternal[ZL303XX_PTSF_AS_CAPABLE] = zl303xx_PtsfMainEval;
}

/* zl303xx_PtsfMiscEval */
/** 

   Callback executed when one of the PTSF-misc sub-flags changes state.

  Parameters:
   [in]   pEvent  PTSF change event data.

  Return Value:  N/A

*******************************************************************************/
static void zl303xx_PtsfMiscEval(zl303xx_PtsfEventS *pEvent)
{
   zl303xx_BooleanE value;

   value = PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_USER) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_QL_TOO_LOW) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_LOS_FROM_PHY) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_WAIT_TO_RESTORE_ACTIVE);

   zl303xx_PtsfFlagSetInternal(pEvent->streamHandle, ZL303XX_PTSF_MISC, value);
}

/* zl303xx_PtsfContractEval */
/** 

   Callback executed when one of the PTSF-contract sub-flags changes state.

  Parameters:
   [in]   pEvent  PTSF change event data.

  Return Value:  N/A

*******************************************************************************/
static void zl303xx_PtsfContractEval(zl303xx_PtsfEventS *pEvent)
{
   zl303xx_BooleanE value;

   value = PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_RETRY_ATTEMPTS_FAIL) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_ANNOUNCE_REJECTED) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_SYNC_REJECTED) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_DELAY_RESP_REJECTED) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_PEER_DELAY_RESP_REJECTED) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_ANNOUNCE_RATE_TOO_LOW) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_SYNC_RATE_TOO_LOW) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_DELAY_RESP_RATE_TOO_LOW) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_PEER_DELAY_RESP_RATE_TOO_LOW);

   zl303xx_PtsfFlagSetInternal(pEvent->streamHandle, ZL303XX_PTSF_CONTRACT, value);
}

/* zl303xx_PtsfTimeoutEval */
/** 

   Callback executed when one of the PTSF-timeout sub-flags changes state.

  Parameters:
   [in]   pEvent  PTSF change event data.

  Return Value:  N/A

*******************************************************************************/
static void zl303xx_PtsfTimeoutEval(zl303xx_PtsfEventS *pEvent)
{
   zl303xx_BooleanE value;

   value = PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_LOSS_ANNOUNCE) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_LOSS_SYNC) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_LOSS_FOLLOW_UP) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_LOSS_SYNC_OR_FOLLOW_UP) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_LOSS_DELAY_RESP) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_LOSS_PEER_DELAY_RESP) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_LOSS_PEER_DELAY_FOLLOW_UP);

   zl303xx_PtsfFlagSetInternal(pEvent->streamHandle, ZL303XX_PTSF_TIMEOUT, value);
}

/* zl303xx_PtsfUnusableEval */
/** 

   Callback executed when one of the PTSF-unusable sub-flags changes state.

  Parameters:
   [in]   pEvent  PTSF change event data.

  Return Value:  N/A

*******************************************************************************/
static void zl303xx_PtsfUnusableEval(zl303xx_PtsfEventS *pEvent)
{
   zl303xx_BooleanE value;

   value = PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_PACKET_REJECT_RESET) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_LOSS_TIME_STAMPS) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_LOCAL_TIME_INACC_EXCEEDED) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_NETWORK_TIME_INACC_EXCEEDED);

   zl303xx_PtsfFlagSetInternal(pEvent->streamHandle, ZL303XX_PTSF_UNUSABLE, value);
}

/* zl303xx_PtsfFrequencyEval */
/** 

   Callback executed when one of the PTSF-frequency sub-flags changes state.
   Note that PTSF-pullInRangeOOR is an exception. It is also a sub-flag of
   PTSF-frequency, but its change is handled by zl303xx_PtsfOorChanged() because
   that flag also feeds into PTSF-phase.

  Parameters:
   [in]   pEvent  PTSF change event data.

  Return Value:  N/A

*******************************************************************************/
static void zl303xx_PtsfFrequencyEval(zl303xx_PtsfEventS *pEvent)
{
   zl303xx_BooleanE value;

   value = PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_PULL_IN_RANGE_OOR) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_PDV_FREQ_METRIC_FAIL) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_FREQ_VAR_TOO_HIGH) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_NOT_FREQ_LOCK);

   zl303xx_PtsfFlagSetInternal(pEvent->streamHandle, ZL303XX_PTSF_FREQUENCY, value);
}

/* zl303xx_PtsfPhaseEval */
/** 

   Callback executed when one of the PTSF-phase sub-flags changes state. Note
   that PTSF-pullInRangeOOR is an exception. It is also a sub-flag of
   PTSF-phase, but its change is handled by zl303xx_PtsfOorChanged() because
   that flag also feeds into PTSF-frequency.

  Parameters:
   [in]   pEvent  PTSF change event data.

  Return Value:  N/A

*******************************************************************************/
static void zl303xx_PtsfPhaseEval(zl303xx_PtsfEventS *pEvent)
{
   zl303xx_BooleanE value;

   value = PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_PULL_IN_RANGE_OOR) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_PDV_PHASE_METRIC_FAIL) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_PHASE_VAR_TOO_HIGH) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_NOT_PHASE_LOCK) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_FREQUENCY);

   zl303xx_PtsfFlagSetInternal(pEvent->streamHandle, ZL303XX_PTSF_PHASE, value);
}

/* zl303xx_PtsfAsCapabaleEval */
/** 

   Callback executed when one of the PTSF-ascapable sub-flags changes state.

  Parameters:
   [in]   pEvent  PTSF change event data.

  Return Value:  N/A

*******************************************************************************/
static void zl303xx_PtsfAsCapabaleEval(zl303xx_PtsfEventS *pEvent)
{
   zl303xx_BooleanE value;

   value = PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_PEER_DELAY_MULTI_RESP) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_PEER_DELAY_ABOVE_THRESHOLD) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_PEER_DELAY_ALLOWED_LOST_RESPONSE);

   zl303xx_PtsfFlagSetInternal(pEvent->streamHandle, ZL303XX_PTSF_AS_CAPABLE, value);
}

/* zl303xx_PtsfOorChanged */
/** 

   Callback executed when the PTSF-pullInRangeOOR flag changes state. This flag
   feeds into PTSF-frequency and PTSF-phase.

  Parameters:
   [in]   pEvent  PTSF change event data.

  Return Value:  N/A

*******************************************************************************/
static void zl303xx_PtsfOorChanged(zl303xx_PtsfEventS *pEvent)
{
   zl303xx_PtsfFrequencyEval(pEvent);
   zl303xx_PtsfPhaseEval(pEvent);
}

/* zl303xx_PtsfFrequencyChanged */
/** 

   Callback executed when the PTSF-frequency flag changes state. This flag
   feeds into PTSF-phase and the overall PTSF flag.

  Parameters:
   [in]   pEvent  PTSF change event data.

  Return Value:  N/A

*******************************************************************************/
static void zl303xx_PtsfFrequencyChanged(zl303xx_PtsfEventS *pEvent)
{
   zl303xx_PtsfPhaseEval(pEvent);
   zl303xx_PtsfMainEval(pEvent);
}

/* zl303xx_PtsfMainEval */
/** 

   Callback executed when one of the sub-flags that feeds into the overall PTSF
   changes state.

  Parameters:
   [in]   pEvent  PTSF change event data.

  Return Value:  N/A

*******************************************************************************/
static void zl303xx_PtsfMainEval(zl303xx_PtsfEventS *pEvent)
{
   zl303xx_BooleanE value;

   value = PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_MISC) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_CONTRACT) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_TIMEOUT) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_UNUSABLE) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_FREQUENCY) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_PHASE) ||
           PTSF_FLAG_ISSET_MASKED(pEvent->streamHandle, ZL303XX_PTSF_AS_CAPABLE);

   zl303xx_PtsfFlagSetInternal(pEvent->streamHandle, ZL303XX_PTSF_MAIN, value);
}

/*****************   END   ****************************************************/
