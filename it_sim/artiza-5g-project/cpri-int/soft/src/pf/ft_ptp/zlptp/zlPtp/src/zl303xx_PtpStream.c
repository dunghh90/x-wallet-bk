

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Some text, starts at column 7, same as following lines
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_Trace.h"

#include "zl303xx_Macros.h"
#include "zl303xx_Ptsf.h"

#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpSetup.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpStrings.h"
#include "zl303xx_PtpFunctionExport.h"
#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpTimer.h"
#include "zl303xx_PtpForeignMasterTbl.h"
#include "zl303xx_PtpProfileDef.h"
#include "zl303xx_PtpG8265p1Api.h"
#include "zl303xx_PtpG8275p1Api.h"

#include "zl303xx_Int64.h"   /* For zl303xx_PtpStreamCalculateDrift */

/*****************   DEFINES     **********************************************/
/* Converts a timer pointer to the stream pointer that contains this timer. */
#define ZL303XX_PTP_TIMER_TO_STREAM(timer, id) \
           ZL303XX_CONTAINER_GET(timer, zl303xx_ClockStreamS, itimer[id])

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/
static zlStatusE zl303xx_PtpContractRxCancel(zl303xx_ClockStreamS *stream,
                                             zl303xx_MessageTypeE msgType);

static zlStatusE zl303xx_PtpStreamLogMsgIntvlSet(zl303xx_PtpStreamHandleT streamHandle,
                                                 zl303xx_MessageTypeE msgType,
                                                 Sint8T log2Intvl);

static void zl303xx_PtpStreamHandleCreateEvent(zl303xx_ClockStreamS *pStream,
                                               Uint8T *rxExtData);

static void zl303xx_PtpStreamTimersInit(zl303xx_ClockStreamS *stream);

static void zl303xx_PtpStreamMsgAlarmCheck(zl303xx_ClockStreamS *pStream,
                                           zl303xx_MessageTypeE msgType,
                                           Sint8T log2Intvl);

/* Stream timer callbacks. */
static void zl303xx_PtpTimerAnncRx         (zl303xx_PtpTimerS *pTimer);
static void zl303xx_PtpTimerSyncTx         (zl303xx_PtpTimerS *pTimer);
static void zl303xx_PtpTimerAnncTx         (zl303xx_PtpTimerS *pTimer);
static void zl303xx_PtpTimerQualification  (zl303xx_PtpTimerS *pTimer);
static void zl303xx_PtpTimerPreMaster      (zl303xx_PtpTimerS *pTimer);
static void zl303xx_PtpTimerUncalibrated   (zl303xx_PtpTimerS *pTimer);
static void zl303xx_PtpTimerDelayReqTx     (zl303xx_PtpTimerS *pTimer);
static void zl303xx_PtpTimerPeerDelayReqTx (zl303xx_PtpTimerS *pTimer);
static void zl303xx_PtpTimerUcastQueryIntvl(zl303xx_PtpTimerS *pTimer);
static void zl303xx_PtpTimerOneSec         (zl303xx_PtpTimerS *pTimer);

static void zl303xx_PtpTimerMsgTx(zl303xx_ClockStreamS *pStream,
                                  zl303xx_MessageTypeE type,
                                  Uint32T numToSend,
                                  void (*msgTxFn)(zl303xx_ClockStreamS *));

void zl303xx_PtpStreamCalculateDrift(zl303xx_ClockStreamS *pPtpStream);

/*****************   STATIC GLOBAL VARIABLES   ********************************/

static const char *Zl303xx_PtpStreamSubStateStr[] = {
   "INITIALIZING",   /* PTP_STREAM_SUBSTATE_INIT        */
   "IDLE",           /* PTP_STREAM_SUBSTATE_IDLE       */
   "PASSIVE",        /* PTP_STREAM_SUBSTATE_PASSIVE    */
   "LISTENING",      /* PTP_STREAM_SUBSTATE_LISTENING         */
   "PRE_MASTER",     /* PTP_STREAM_SUBSTATE_PRE_MASTER       */
   "UNCALIBRATED",   /* PTP_STREAM_SUBSTATE_UNCALIBRATED */
   "MASTER",         /* PTP_STREAM_SUBSTATE_MASTER          */
   "SLAVE",          /* PTP_STREAM_SUBSTATE_SLAVE      */
   "INVALID"
};

/*****************   IMPORTED GLOBAL VARIABLES   ******************************/

extern zl303xx_ClockStreamS *Zl303xx_PtpStreamArray[ZL303XX_PTP_NUM_STREAMS_MAX];
extern const char zl303xx_PtpReleaseVersion[];

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* zl303xx_PtpStreamCreateStructInit */
/**
   Fills a zl303xx_PtpStreamCreateS structure with the default configuration for
   initializing a PTP stream.

  Parameters:
   [out]  pStreamCreate  Default parameters for creating a PTP stream.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pStreamCreate is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamCreateStructInit(zl303xx_PtpStreamCreateS *pStreamCreate)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpStreamCreateStructInit:", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pStreamCreate);
   }

   /* Assign default values. */
   if (status == ZL303XX_OK)
   {
      OS_MEMSET(pStreamCreate, 0, sizeof(*pStreamCreate));

      pStreamCreate->portHandle                 = 0;
      pStreamCreate->mode                       = ZL303XX_PTP_DEFAULT_STREAM_MODE;
      pStreamCreate->unicast                    = ZL303XX_PTP_DEFAULT_STREAM_UNICAST;
      pStreamCreate->logAnnounceInterval        = ZL303XX_PTP_DEFAULT_STREAM_LOG_ANNC_INTVL;
      pStreamCreate->announceReceiptTimeout     = ZL303XX_PTP_DEFAULT_STREAM_ANNC_TIMEOUT;
      pStreamCreate->logSyncInterval            = ZL303XX_PTP_DEFAULT_STREAM_LOG_SYNC_INTVL;
      pStreamCreate->logMinDelayReqInterval     = ZL303XX_PTP_DEFAULT_STREAM_LOG_DELAY_INTVL;
      pStreamCreate->logMinPdelayReqInterval    = ZL303XX_PTP_DEFAULT_STREAM_LOG_PEER_DELAY_INTVL;
      pStreamCreate->delayReqTimeSetRequired    = ZL303XX_FALSE;

      /* Delay Asymmetry. */
      pStreamCreate->delayAsymmetry.ingress.scaledNanoseconds.lo = (ZL303XX_PTP_DEFAULT_STREAM_DELAY_ASYM_INGRESS & 0xFFFFFFFF);
      pStreamCreate->delayAsymmetry.ingress.scaledNanoseconds.hi = (((ZL303XX_PTP_DEFAULT_STREAM_DELAY_ASYM_INGRESS >> 16) >> 16) & 0xFFFFFFFF);
      pStreamCreate->delayAsymmetry.egress.scaledNanoseconds.lo  = (ZL303XX_PTP_DEFAULT_STREAM_DELAY_ASYM_EGRESS & 0xFFFFFFFF);
      pStreamCreate->delayAsymmetry.egress.scaledNanoseconds.hi  = (((ZL303XX_PTP_DEFAULT_STREAM_DELAY_ASYM_EGRESS >> 16) >> 16) & 0xFFFFFFFF);

      pStreamCreate->unicastNegotiationDuration = ZL303XX_PTP_DEFAULT_STREAM_UNI_NEG_DUR;
      pStreamCreate->maxClockClass              = ZL303XX_PTP_DEFAULT_STREAM_MAX_CLOCK_CLASS;

      /* Event Message Length Extension */
      pStreamCreate->msgLenPadding[ZL303XX_MSG_ID_SYNC]            = ZL303XX_PTP_DEFAULT_MSG_LEN_EXTENSION_SYNC;
      pStreamCreate->msgLenPadding[ZL303XX_MSG_ID_DELAY_REQ]       = ZL303XX_PTP_DEFAULT_MSG_LEN_EXTENSION_DELAY_REQ;
      pStreamCreate->msgLenPadding[ZL303XX_MSG_ID_PEER_DELAY_REQ]  = ZL303XX_PTP_DEFAULT_MSG_LEN_EXTENSION_PDELAY_REQ;
      pStreamCreate->msgLenPadding[ZL303XX_MSG_ID_PEER_DELAY_RESP] = ZL303XX_PTP_DEFAULT_MSG_LEN_EXTENSION_PDELAY_RESP;

      /* Event Message PAD TLV Extension */
      pStreamCreate->padTlvLen[ZL303XX_MSG_ID_SYNC]            = ZL303XX_PTP_DEFAULT_PAD_TLV_LEN_SYNC;
      pStreamCreate->padTlvLen[ZL303XX_MSG_ID_DELAY_REQ]       = ZL303XX_PTP_DEFAULT_PAD_TLV_LEN_DELAY_REQ;
      pStreamCreate->padTlvLen[ZL303XX_MSG_ID_PEER_DELAY_REQ]  = ZL303XX_PTP_DEFAULT_PAD_TLV_LEN_PDELAY_REQ;
      pStreamCreate->padTlvLen[ZL303XX_MSG_ID_PEER_DELAY_RESP] = ZL303XX_PTP_DEFAULT_PAD_TLV_LEN_PDELAY_RESP;

      /* This renewal time is set afterwards to be multiplied with logQueryInterval */
      pStreamCreate->unicastNegotiationRenew    = ZL303XX_PTP_DEFAULT_STREAM_UNI_NEG_RENEW;

      /* Best Effort configuration */
      pStreamCreate->unicastNegotiation.bestEffort[ZL303XX_MSG_ID_ANNOUNCE].enabled            = ZL303XX_FALSE;
      pStreamCreate->unicastNegotiation.bestEffort[ZL303XX_MSG_ID_ANNOUNCE].intervalMax        = PTP_MESSAGE_INTERVAL_MAX;
      pStreamCreate->unicastNegotiation.bestEffort[ZL303XX_MSG_ID_SYNC].enabled                = ZL303XX_FALSE;
      pStreamCreate->unicastNegotiation.bestEffort[ZL303XX_MSG_ID_SYNC].intervalMax            = PTP_MESSAGE_INTERVAL_MAX;
      pStreamCreate->unicastNegotiation.bestEffort[ZL303XX_MSG_ID_DELAY_RESP].enabled          = ZL303XX_FALSE;
      pStreamCreate->unicastNegotiation.bestEffort[ZL303XX_MSG_ID_DELAY_RESP].intervalMax      = PTP_MESSAGE_INTERVAL_MAX;
      pStreamCreate->unicastNegotiation.bestEffort[ZL303XX_MSG_ID_PEER_DELAY_RESP].enabled     = ZL303XX_FALSE;
      pStreamCreate->unicastNegotiation.bestEffort[ZL303XX_MSG_ID_PEER_DELAY_RESP].intervalMax = PTP_MESSAGE_INTERVAL_MAX;

      /* Unicast Negotiation Retry configuration */
      pStreamCreate->unicastNegotiation.retry.denial.logQueryCount = ZL303XX_PTP_DEFAULT_STREAM_UNI_NEG_RETRIES_MAX;
      pStreamCreate->unicastNegotiation.retry.denial.lockoutSec = ZL303XX_PTP_DEFAULT_STREAM_UNI_NEG_LOCKOUT_SEC;
      pStreamCreate->unicastNegotiation.retry.timeout.logQueryCount = ZL303XX_PTP_DEFAULT_STREAM_UNI_NEG_RETRIES_MAX;
      pStreamCreate->unicastNegotiation.retry.timeout.lockoutSec = ZL303XX_PTP_DEFAULT_STREAM_UNI_NEG_LOCKOUT_SEC;

      /* TRUE to have MASTER streams request SYNC/DELAY messages. */
      pStreamCreate->unicastMonitorTiming       = ZL303XX_PTP_DEFAULT_STREAM_UNI_MONITOR_TIMING;

      pStreamCreate->numTsRecords               = ZL303XX_PTP_DEFAULT_STREAM_NUM_TS_RECORDS;
      pStreamCreate->numTwoStepRecords          = ZL303XX_PTP_DEFAULT_STREAM_NUM_TWO_STEP_REC;

      /* Disable the override class values and use the ParentDS. */
      pStreamCreate->ingressClockClass          = PTP_CLOCK_CLASS_OVERRIDE_DISABLED;
      pStreamCreate->egressClockClass           = PTP_CLOCK_CLASS_OVERRIDE_DISABLED;

      /* By default, send a keep-alive event once per minute. */
      pStreamCreate->keepAliveSec               = ZL303XX_PTP_DEFAULT_STREAM_KEEP_ALIVE_SEC;

      /* By default, disable the UNCALIBRATED Interval and LOCK requirement. */
      pStreamCreate->uncalibrated.anncIntervals = ZL303XX_PTP_DEFAULT_STREAM_UNCALIBRATED_INTVL;
      pStreamCreate->uncalibrated.lockRequired  = ZL303XX_FALSE;

      pStreamCreate->extData                    = NULL;
      pStreamCreate->requestedHandle            = (zl303xx_PtpStreamHandleT)ZL303XX_PTP_INVALID;
/*   #warning MSCC: Default PTP inter-pkt rates for both SYNC and DREQ are now disabled */
      pStreamCreate->createFlags                = 0;
      /* To re-eable use ZL303XX_PTP_DREQ_FORCE_MIN_INTVL and ZL303XX_PTP_SYNC_FORCE_MIN_INTVL
      pStreamCreate->createFlags                = ZL303XX_PTP_DREQ_FORCE_MIN_INTVL; */
   }

   return status;
}

/* zl303xx_PtpStreamCreate */
/**
   Creates a PTP stream instance associated with an existing PTP port.

   A PTP stream represents a uni- or bi-directional connection between two
   endpoints in the timing network (e.g., server to client).

  Parameters:
   [in]   pStreamCreate  Stream configuration parameters.
   [out]  pStreamHandle  Handle assigned to this stream. Used to reference it
                              when calling other functions.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       pStreamCreate or pStreamHandle is NULL.
     ZL303XX_PARAMETER_INVALID     A member of pStreamCreate is invalid.
     ZL303XX_RTOS_MEMORY_FAIL      Failed to allocate memory for stream data.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.
     ZL303XX_TABLE_FULL            Maximum number of streams already created.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamCreate(zl303xx_PtpStreamCreateS *pStreamCreate,
                                  zl303xx_PtpStreamHandleT *pStreamHandle)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_PortDataS *port = NULL;
   zl303xx_ClockStreamS *pStream = NULL;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpStreamCreate:", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pStreamCreate) |
               ZL303XX_CHECK_POINTER(pStreamHandle);
   }

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(pStreamCreate->portHandle, &port);
   }

   /* Check for invalid configurations that ONLY apply to user-created streams. */

   /* Unicast streams on a mixed-mode port should only be created by the PTP
    * protocol itself. */
   if (status == ZL303XX_OK)
   {
      if ((pStreamCreate->unicast == ZL303XX_TRUE) &&
          (port->config.mixedMode == ZL303XX_TRUE))
      {
         ZL303XX_TRACE_ERROR(
               "   (ERROR) Unicast streams may not be manually configured on a mixed-mode port.",
               0,0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(port->clock);
      status = mStatus;
   }

   /* Call the internal stream create function. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamCreateInternal(pStreamCreate, port, NULL, &pStream);
   }

   /* Execute any Profile Specific actions related to the stream (may be NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpProfileStreamPostCreate(pStream);
   }

   /* Release the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(port->clock, status);
   }

   /* Return the stream handle. */
   if (status == ZL303XX_OK)
   {
      *pStreamHandle = pStream->clockStreamHandle;
   }

   return status;
}  /* END zl303xx_PtpStreamCreate */

/* zl303xx_PtpStreamDeleteStructInit */
/**
   Fills a zl303xx_PtpStreamDeleteS structure with the default configuration for
   deleting a PTP stream.

  Notes:
   Does not need to be called since zl303xx_PtpStreamDeleteS does not currently
   have any members.

  Parameters:
   [out]  pStreamDelete  Default parameters to shut down a PTP stream.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pStreamDelete is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamDeleteStructInit(zl303xx_PtpStreamDeleteS *pStreamDelete)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpStreamDeleteStructInit:", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pStreamDelete);
   }

   /* Initialize the stream delete structure. */
   if (status == ZL303XX_OK)
   {
      /* Zero-fill the structure; it does not contain any members. */
      OS_MEMSET(pStreamDelete, 0, sizeof(*pStreamDelete));
   }

   return status;
}

/* zl303xx_PtpStreamDelete */
/**
   Deletes a PTP stream.

  Parameters:
   [in]      streamHandle   Handle to an existing stream.
   [in,out]  pStreamDelete  (Optional) Parameters to shut down this PTP stream.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle is not valid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamDelete(zl303xx_PtpStreamHandleT streamHandle,
                                  zl303xx_PtpStreamDeleteS *pStreamDelete)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_ClockStreamS *stream = NULL;
   zl303xx_PtpClockS *pClock = NULL;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpStreamDelete:", 0,0,0,0,0,0);

   /* pStreamDelete may be NULL--this is handled in zl303xx_PtpStreamDeleteInternal(). */

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      if ((status = ZL303XX_CHECK_STREAM_HANDLE(streamHandle)) != ZL303XX_OK)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Invalid streamHandle=%lu.", streamHandle, 0,0,0,0,0);
      }
   }

   if (status == ZL303XX_OK)
   {
      /* Get a pointer to the stream data. Don't call zl303xx_PtpStreamDataGet()
       * because we want to exit the function with ZL303XX_OK if the stream does
       * not exist. */
      stream = Zl303xx_PtpStreamArray[streamHandle];

      if (stream == NULL)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "   No stream with handle=%lu exists.",
               streamHandle, 0,0,0,0,0);

         return status;
      }

      /* Lock the clock mutex. */
      pClock = stream->portData->clock;
      mStatus = zl303xx_PtpClockMutexLock(pClock);
      status = mStatus;
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDeleteInternal(stream, pStreamDelete);
   }

   /* Release the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pClock, status);
   }

   return status;
}  /* END zl303xx_PtpStreamDelete */

/* zl303xx_PtpStreamConfigGet */
/**
   Returns the zl303xx_PtpStreamCreateS structure used to initialize a PTP stream.

  Parameters:
   [in]   streamHandle   Handle to an existing stream.
   [out]  pStreamConfig  A copy of the structure used to create the port.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pStreamConfig is NULL.
     ZL303XX_PARAMETER_INVALID  streamHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamConfigGet(zl303xx_PtpStreamHandleT streamHandle,
                                     zl303xx_PtpStreamCreateS *pStreamConfig)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream = NULL;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
         "zl303xx_PtpStreamConfigGet:", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pStreamConfig);
   }

   /* Get a pointer to the stream data structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   /* Return a copy of the configuration structure. */
   if (status == ZL303XX_OK)
   {
      *pStreamConfig = stream->config;
   }

   return status;
}

/* zl303xx_PtpStreamOverrideModeSet */
/**
   Sets (over-rides) the stream mode configuration to one of the following:
   - USE_BMC:     Allows the Clock to change the Stream's State based on
                     external reference selection.
   - SLAVE_ONLY:  Forces the Stream into SLAVE state (can go to LISTENING or
                     UNCALIBRATED as well depending on reference selection).
   - MASTER_ONLY: Forces the Stream into MASTER mode (Never PASSIVE).

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   overrideMode  Value of the stream mode.

  Return Value:
     ZL303XX_OK                      Success.
     ZL303XX_PARAMETER_INVALID       streamHandle or overrideMode is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL    Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL    Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamOverrideModeSet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_PtpStreamModeE overrideMode)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;

   /* Pointers to target Data Objects */
   zl303xx_ClockStreamS *pPtpStream = NULL;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Get a pointer to the stream data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream);
   }

   /* Check the value of the mode parameter. */
   if (status == ZL303XX_OK)
   {
      pPtpClock = pPtpStream->portData->clock;

      if (overrideMode >= ZL303XX_PTP_STREAM_MODE_NUM_TYPES)
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Lock the clock mutex since this goes down into the Profile binding. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pPtpClock);
      status = mStatus;
   }

   /* Perform Profile specific checks here to see if this setting is allowed. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpProfileStreamModeCheck(pPtpStream, &overrideMode);
   }

   /* Since the over-ride mode of a stream is a non-Standard setting, the
    * following logging is done in case the over-ride setting may conflict with
    * Standard Clock operations. These conflicts will not be flagged as errors
    * but may affect expected operation so WARN the user.  */
   /*
    *                                    Stream::MODE
    *                   ===================================================
    *                  |     USE_BMC    |   SLAVE_ONLY   |   MASTER_ONLY   |
    *                   ===================================================
    *        DEFAULT   |     DEFAULT    |   SLAVE ONLY   |   MASTER ONLY   |
    *  P   MASTER ONLY |    OPERATION   |    STREAM      |     STREAM      |
    *  O    == FALSE   |                |                |    OPERATION    |
    *  R                ---------------------------------------------------
    *  T   MASTER ONLY |   MASTER ONLY  |    CONFLICT    |   MASTER ONLY   |
    *       == TRUE    |   PORT/STREAM  |   MASTER ONLY  |   PORT/STREAM   |
    *                  |    OPERATION   |    OPERATION   |    OPERATION    |
    *                   ---------------------------------------------------
    */
   if ((status == ZL303XX_OK) &&
       (overrideMode != ZL303XX_PTP_STREAM_USE_BMC)) /* BMC is the standard mode */
   {
      /* Log a general WARNING. */
      const char *streamModeStr = zl303xx_PtpStreamModeToStr(overrideMode);
      ZL303XX_TRACE_ALWAYS(
            "Forcing Stream (%d) to non-standard over-ride mode (%d)(%s)",
            pPtpStream->clockStreamHandle,
            overrideMode, streamModeStr, 0,0,0);

      /* Check for specific conflict scenarios for SLAVE_ONLY Mode:
       * - Setting Stream-override = SLAVE_ONLY on a MASTER_ONLY Clock.
       * - Setting Stream-override = SLAVE_ONLY on a MASTER_ONLY Port. */
      if (overrideMode == ZL303XX_PTP_STREAM_SLAVE_ONLY)
      {
         if (pPtpClock->config.defaultDS.clockQuality.clockClass <= PTP_MASTER_ONLY_CLOCK_CLASS_MAX)
         {
            ZL303XX_TRACE_ALWAYS(
                  "   Stream SLAVE_ONLY Override may conflict with MASTER Clock operation.",
                  0,0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS(
                  "   Any SLAVE_ONLY Streams will operate in PASSIVE mode.",
                  0,0,0,0,0,0);
         }

         /* No ELSE so both conditions are checked. */
         if (pPtpStream->portData->config.masterOnly == ZL303XX_TRUE)
         {
            ZL303XX_TRACE_ALWAYS(
                  "   Stream SLAVE_ONLY Override may conflict with MASTER_ONLY Port operation.",
                  0,0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS(
                  "   Any SLAVE_ONLY Streams will operate in PASSIVE mode.",
                  0,0,0,0,0,0);
         }
      }

      /* Check for specific conflict scenarios for MASTER_ONLY Mode:
       * - Setting Stream-override = MASTER_ONLY on a SLAVE_ONLY Clock.
       * - Setting Stream-override = MASTER_ONLY on a BMC Port is not really an
       *   issue so don't log anything. */
      else if (overrideMode == ZL303XX_PTP_STREAM_MASTER_ONLY)
      {
         if ((pPtpClock->config.defaultDS.slaveOnly == ZL303XX_TRUE) ||
             (pPtpClock->config.defaultDS.clockQuality.clockClass == PTP_SLAVE_ONLY_CLOCK_CLASS))
         {
            ZL303XX_TRACE_ALWAYS(
                  "   Stream MASTER_ONLY Override may conflict with SLAVE_ONLY Clock operation.",
                  0,0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS(
                  "   Any MASTER_ONLY Streams will operate in MASTER mode.",
                  0,0,0,0,0,0);
         }
      }
   }

   /* Set the new over-ride mode. */
   if (status == ZL303XX_OK)
   {
      pPtpStream->config.mode = overrideMode;

      /* The new setting may impact the Clock or Stream State. Execute the Clock
       * State-Decision algorithm to re-evaluate all states.   */
      zl303xx_PtpClockBmcaUpdateInitiate(pPtpClock);
   }

   /* Release the clock mutex, if necessary. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* END zl303xx_PtpStreamOverrideModeSet */

/* zl303xx_PtpStreamClockHandleGet */
/**
   Gets the clock handle associated with a stream.

  Parameters:
   [in]   streamHandle   Handle to an existing stream.
   [out]  clockHandle    Handle of the clock associated with the stream.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    clockHandle is NULL.
     ZL303XX_PARAMETER_INVALID  streamHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamClockHandleGet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_PtpClockHandleT *clockHandle)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(clockHandle);
   }

   /* Get a pointer to the stream data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream);
   }

   /* Return the pointer to user data. */
   if (status == ZL303XX_OK)
   {
      *clockHandle = pPtpStream->portData->clock->clockHandle;
   }

   return status;
}

/* zl303xx_PtpStreamExtDataGet */
/**
   Gets the external data pointer associated with a stream.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [out]  ppExtData     Pointer to user-specified data structure.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    ppExtData is NULL.
     ZL303XX_PARAMETER_INVALID  streamHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamExtDataGet(zl303xx_PtpStreamHandleT streamHandle,
                                       void **ppExtData)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(ppExtData);
   }

   /* Get a pointer to the stream data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   /* Return the pointer to user data. */
   if (status == ZL303XX_OK)
   {
      *ppExtData = stream->config.extData;
   }

   return status;
}

/* zl303xx_PtpStreamExtDataSet */
/**
   Sets the external data pointer associated with a stream.

  Parameters:
   [in]   streamHandle  Handle to a previously created PTP stream.
   [in]   pExtData      Pointer to user-specified data structure.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  streamHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamExtDataSet(zl303xx_PtpStreamHandleT streamHandle,
                                      void *pExtData)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream = NULL;

   /* Don't bother checking pExtData, as the user may want to set it to NULL. */

   /* Get a pointer to the stream data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   /* Set the external data pointer. */
   if (status == ZL303XX_OK)
   {
      stream->config.extData = pExtData;
   }

   return status;
}

/* zl303xx_PtpStreamIngressClockClassSet */
/**
   Sets the ingress override clock class value for the specified stream.
   If the value is non-zero, it will replace the clockClass field in the
   received Announce message.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   ingressClass  Ingress clock class over-ride value. A value of
                             0 leaves the received value as is.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  streamHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamIngressClockClassSet(
      zl303xx_PtpStreamHandleT streamHandle,
      Uint8T ingressClass)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream = NULL;

   /* Get a pointer to the stream data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(stream->portData->clock);
   }

   /* Set the ingress over-ride value for the clockClass. */
   if (status == ZL303XX_OK)
   {
      stream->config.ingressClockClass = ingressClass;
      status = zl303xx_PtpClockMutexUnlock(stream->portData->clock, status);
   }

   return status;
}

/* zl303xx_PtpStreamEgressClockClassSet */
/**
   Sets the egress override clock class value for the specified stream.
   If the value is non-zero, it will be used as the transmitted clockClass value.
   Otherwise, the clock or parentDS value is used.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   egressClass   Egress clock class over-ride value. A value of
                             0 means that the clock or parentDS value is used.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  streamHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamEgressClockClassSet(
      zl303xx_PtpStreamHandleT streamHandle,
      Uint8T egressClass)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream = NULL;

   /* Get a pointer to the stream data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(stream->portData->clock);
   }

   /* Set the egress over-ride value for the clockClass. */
   if (status == ZL303XX_OK)
   {
      stream->config.egressClockClass = egressClass;
      status = zl303xx_PtpClockMutexUnlock(stream->portData->clock, status);
   }

   return status;
}

/* zl303xx_PtpStreamEgressOverrideAnncFieldSet */
/**
   Sets over-ride values for certain egress parameters of ANNOUNCE messages
   (Header & Data portions) on a per-stream level. When messages are sent from
   this stream, the software will check if an ANNOUNCE override value is
   configured and force that value into the message.

   This routine DOES NOT verify override parameters. It is expected that if a
   override value is configured, the user is aware of its implications.

  Parameters:
   [in]  streamHandle   Handle to an existing stream.
   [in]  overrideId     ID of the stream Announec parameter to over-ride.
   [in]  overrideValue  Value of the override parameter (passed as a VOID
                              pointer to accommodate various types). If set to
                              NULL, any current override value is cleared.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  streamHandle or overrideId is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamEgressOverrideAnncFieldSet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_PtpStreamOverrideAnncEgressE overrideId,
      void *overrideValue)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Check the input override ID. */
   if (status == ZL303XX_OK)
   {
      if (overrideId >= ZL303XX_PTP_STREAM_OVERRIDE_NUM_ANNC_ENTRIES)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpStreamEgressOverrideAnncFieldSet: Invalid ID (%u)",
               overrideId, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Get the associated stream data structure.
    * (An error is returned if the stream is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream);
   }

   if (status == ZL303XX_OK)
   {
      /* Clear any NULL values. */
      if (overrideValue == NULL)
      {
         pPtpStream->config.override.anncOvrdEn[overrideId] = ZL303XX_FALSE;
      }

      /* Set any provided values. */
      else
      {
         /* Indicate an Override exists. */
         pPtpStream->config.override.anncOvrdEn[overrideId] = ZL303XX_TRUE;

         switch (overrideId)
         {
            /** PTP Header override values. */
            /********************************/
            case ZL303XX_PTP_STREAM_OVERRIDE_TRANSPORT :
            {
               pPtpStream->config.override.ptpHeader.transportSpecific = *(Uint4T*)overrideValue;
               break;
            }
            case ZL303XX_PTP_STREAM_OVERRIDE_DOMAIN :
            {
               pPtpStream->config.override.ptpHeader.domainNumber = *(Uint8T*)overrideValue;
               break;
            }
            case ZL303XX_PTP_STREAM_OVERRIDE_CORRECTION :
            {
               pPtpStream->config.override.ptpHeader.correctionField = *(Uint64S*)overrideValue;
               break;
            }
            case ZL303XX_PTP_STREAM_OVERRIDE_SRC_PORT_ID :
            {
               pPtpStream->config.override.ptpHeader.sourcePortIdentity = *(zl303xx_PortIdentity*)overrideValue;
               break;
            }

            /** TimePropertiesDS override values. */
            /**************************************/
            case ZL303XX_PTP_STREAM_OVERRIDE_CURRENT_UTC_OFFSET_VALID :
            {
               pPtpStream->config.override.timeProperties.currentUtcOffsetValid = *(zl303xx_BooleanE*)overrideValue;
               break;
            }
            case ZL303XX_PTP_STREAM_OVERRIDE_TIME_TRACEABLE :
            {
               pPtpStream->config.override.timeProperties.timeTraceable = *(zl303xx_BooleanE*)overrideValue;
               break;
            }
            case ZL303XX_PTP_STREAM_OVERRIDE_FREQUENCY_TRACEABLE :
            {
               pPtpStream->config.override.timeProperties.frequencyTraceable = *(zl303xx_BooleanE*)overrideValue;
               break;
            }

            /* We allow this optional parameter to override even if the
             * functionality is disabled. */
            case ZL303XX_PTP_STREAM_OVERRIDE_SYNCHRO_UNCERTAIN :
            {
               pPtpStream->config.override.timeProperties.synchronizationUncertain = *(zl303xx_BooleanE*)overrideValue;
               break;
            }

            /** Announce DATA override values. */
            /***********************************/
            case ZL303XX_PTP_STREAM_OVERRIDE_ORIGIN_TIMESTAMP :
            {
               pPtpStream->config.override.anncData.originTimestamp = *(zl303xx_TimeStamp*)overrideValue;
               break;
            }
            case ZL303XX_PTP_STREAM_OVERRIDE_CURRENT_UTC_OFFSET :
            {
               pPtpStream->config.override.anncData.currentUtcOffset = *(Sint16T*)overrideValue;
               break;
            }
            case ZL303XX_PTP_STREAM_OVERRIDE_GM_PRIORITY1 :
            {
               pPtpStream->config.override.anncData.grandmasterPriority1 = *(Uint8T*)overrideValue;
               break;
            }
            case ZL303XX_PTP_STREAM_OVERRIDE_GM_PRIORITY2 :
            {
               pPtpStream->config.override.anncData.grandmasterPriority2 = *(Uint8T*)overrideValue;
               break;
            }
            case ZL303XX_PTP_STREAM_OVERRIDE_GM_CLASS :
            {
               pPtpStream->config.override.anncData.grandmasterClockQuality.clockClass = *(Uint8T*)overrideValue;
               break;
            }
            case ZL303XX_PTP_STREAM_OVERRIDE_GM_ACCURACY :
            {
               pPtpStream->config.override.anncData.grandmasterClockQuality.clockAccuracy = *(zl303xx_ClockAccuracyE*)overrideValue;
               break;
            }
            case ZL303XX_PTP_STREAM_OVERRIDE_GM_VARIANCE :
            {
               pPtpStream->config.override.anncData.grandmasterClockQuality.offsetScaledLogVariance = *(Uint16T*)overrideValue;
               break;
            }
            case ZL303XX_PTP_STREAM_OVERRIDE_GM_IDENTITY :
            {
               OS_MEMCPY(pPtpStream->config.override.anncData.grandmasterIdentity,
                      overrideValue,
                      PTP_V2_CLK_IDENTITY_LENGTH);
               break;
            }
            case ZL303XX_PTP_STREAM_OVERRIDE_STEPS_REMOVED :
            {
               pPtpStream->config.override.anncData.stepsRemoved = *(Uint16T*)overrideValue;
               break;
            }
            case ZL303XX_PTP_STREAM_OVERRIDE_TIME_SOURCE :
            {
               pPtpStream->config.override.anncData.timeSource = *(zl303xx_TimeSourceE*)overrideValue;
               break;
            }

            case ZL303XX_PTP_STREAM_OVERRIDE_NUM_ANNC_ENTRIES :
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
}  /* END zl303xx_PtpStreamEgressOverrideAnncFieldSet */

/* zl303xx_PtpStreamEgressOverrideShow */
/**
   Displays the configuration and values for the egress Stream
   over-ride parameters.

  Parameters:
   [in]  streamHandle    Handle to an existing Stream.

 update trigger.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  streamHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamEgressOverrideShow(
      zl303xx_PtpStreamHandleT streamHandle)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   static char srcPortIdStr[64];
   static char gmClockIdStr[64];

   /* Get the associated stream data structure.
    * (An error is returned if the stream is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream);
   }

   if (status == ZL303XX_OK)
   {
      /** PTP Header override values. */
      /**************************************/
      printf("\nEgress Override Param:   Override    Value\n");
      printf("------------------------------------------\n");
      printf("   Transport Specific:      %s        0x%02X\n",
            (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_TRANSPORT]) ? "T" : "F",
             pPtpStream->config.override.ptpHeader.transportSpecific);
      printf("               Domain:      %s        %d\n",
            (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_DOMAIN]) ? "T" : "F",
             pPtpStream->config.override.ptpHeader.domainNumber);
      printf("     Correction Field:      %s        %010u:%010u\n",
            (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_CORRECTION]) ? "T" : "F",
             pPtpStream->config.override.ptpHeader.correctionField.hi,
             pPtpStream->config.override.ptpHeader.correctionField.lo);

      zl303xx_PtpV2PortIdentityToString(&pPtpStream->config.override.ptpHeader.sourcePortIdentity, srcPortIdStr);
      printf("       Source Port ID:      %s        %s\n",
            (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_SRC_PORT_ID]) ? "T" : "F",
             srcPortIdStr);

      /** TimePropertiesDS override values. */
      /**************************************/
      printf("     UTC Offset VALID:      %s        %s\n",
            (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_CURRENT_UTC_OFFSET_VALID]) ? "T" : "F",
            (pPtpStream->config.override.timeProperties.currentUtcOffsetValid) ? "T" : "F");
      printf("       Time Traceable:      %s        %s\n",
            (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_TIME_TRACEABLE]) ? "T" : "F",
            (pPtpStream->config.override.timeProperties.timeTraceable) ? "T" : "F");
      printf("  Frequency Traceable:      %s        %s\n",
            (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_FREQUENCY_TRACEABLE]) ? "T" : "F",
            (pPtpStream->config.override.timeProperties.frequencyTraceable) ? "T" : "F");
      printf("   Synchro Uncertain:       %s        %s\n",
            (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_SYNCHRO_UNCERTAIN]) ? "T" : "F",
            (pPtpStream->config.override.timeProperties.synchronizationUncertain) ? "T" : "F");

      /** Announce DATA override values. */
      /***********************************/
      printf("     Origin Timestamp:      %s        %u:%010u:%010u\n",
            (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_ORIGIN_TIMESTAMP]) ? "T" : "F",
             pPtpStream->config.override.anncData.originTimestamp.secondsField.hi,
             pPtpStream->config.override.anncData.originTimestamp.secondsField.lo,
             pPtpStream->config.override.anncData.originTimestamp.nanosecondsField);

      printf("   Current UTC Offset:      %s        %d\n",
            (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_CURRENT_UTC_OFFSET]) ? "T" : "F",
             pPtpStream->config.override.anncData.currentUtcOffset);
      printf("            Priority1:      %s        %d\n",
            (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_GM_PRIORITY1]) ? "T" : "F",
             pPtpStream->config.override.anncData.grandmasterPriority1);
      printf("            Priority2:      %s        %d\n",
            (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_GM_PRIORITY2]) ? "T" : "F",
             pPtpStream->config.override.anncData.grandmasterPriority2);
      printf("       GM Clock Class:      %s        %d\n",
            (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_GM_CLASS]) ? "T" : "F",
             pPtpStream->config.override.anncData.grandmasterClockQuality.clockClass);
      printf("    GM Clock Accuracy:      %s        0x%02X\n",
            (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_GM_ACCURACY]) ? "T" : "F",
             pPtpStream->config.override.anncData.grandmasterClockQuality.clockAccuracy);
      printf("      Offset Variance:      %s        0x%04X\n",
            (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_GM_VARIANCE]) ? "T" : "F",
             pPtpStream->config.override.anncData.grandmasterClockQuality.offsetScaledLogVariance);

      zl303xx_PtpV2ClockIdentityToString(pPtpStream->config.override.anncData.grandmasterIdentity, gmClockIdStr);
      printf("          GM Clock ID:      %s        %s\n",
            (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_GM_IDENTITY]) ? "T" : "F",
             gmClockIdStr);

      printf("        Steps Removed:      %s        %d\n",
            (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_STEPS_REMOVED]) ? "T" : "F",
             pPtpStream->config.override.anncData.stepsRemoved);
      printf("          Time Source:      %s        0x%02X\n",
            (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_TIME_SOURCE]) ? "T" : "F",
             pPtpStream->config.override.anncData.timeSource);
   }

   return status;
}

/* zl303xx_PtpStreamDelayAsymmetrySet */
/**
   Sets the Delay Asymmetry values for a stream if a value is specified. Can
   change both ingress & egress in one command or leave one NULL to maintain its
   current configuration.

  Parameters:
   [in]  streamHandle   Handle to an existing stream.
   [in]  ingressDelay   The ingress delay asymmetry value in nSec * (2^16).
                              If NULL, the current value is maintained.
   [in]  egressDelay    The egress delay asymmetry value in nSec * (2^16).
                              If NULL, the current value is maintained.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or either delay asymmetry value
                                        is invalid.
     ZL303XX_INVALID_POINTER       Both input parameters are NULL.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamDelayAsymmetrySet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_TimeInterval *ingressDelay,
      zl303xx_TimeInterval *egressDelay)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pStream = NULL;

   Sint8T signage;

   /* Check input parameters. */
   if ((status == ZL303XX_OK) && (ingressDelay == NULL) && (egressDelay == NULL))
   {
      ZL303XX_TRACE_ALWAYS("One or both input delay values must be specified", 0,0,0,0,0,0);
      status = ZL303XX_INVALID_POINTER;
   }

   /* Don't allow either of the over/under flow values to be set. */
   else
   {
      if (ingressDelay != NULL)
      {
         signage = zl303xx_PtpTimeIntervalSignage(*ingressDelay);

         if ((signage == PTP_TIME_INTERVAL_OVERFLOW) ||
             (signage == PTP_TIME_INTERVAL_UNDERFLOW))
         {
            ZL303XX_TRACE_ALWAYS(" >> ingressDelay: value Invalid (0x%08X:%08X)",
                  ingressDelay->scaledNanoseconds.hi,
                  ingressDelay->scaledNanoseconds.lo, 0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
         }
      }

      /* This this too, even if ingressDelay was not valid. */
      if (egressDelay != NULL)
      {
         signage = zl303xx_PtpTimeIntervalSignage(*egressDelay);

         if ((signage == PTP_TIME_INTERVAL_OVERFLOW) ||
             (signage == PTP_TIME_INTERVAL_UNDERFLOW))
         {
            ZL303XX_TRACE_ALWAYS(" >> egressDelay: value Invalid (0x%08X:%08X)",
                  egressDelay->scaledNanoseconds.hi,
                  egressDelay->scaledNanoseconds.lo, 0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
         }
      }
   }

   /* Get a pointer to the stream data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pStream);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pStream->portData->clock);
   }

   /* Update the provided Delay Asymmetry value(s). Unlock the mutex. */
   if (status == ZL303XX_OK)
   {
      if (ingressDelay != NULL)
      {
         pStream->config.delayAsymmetry.ingress = *ingressDelay;
      }

      if (egressDelay != NULL)
      {
         pStream->config.delayAsymmetry.egress = *egressDelay;
      }

      status = zl303xx_PtpClockMutexUnlock(pStream->portData->clock, status);
   }

   return status;
}  /* END zl303xx_PtpStreamDelayAsymmetrySet */

/* zl303xx_PtpStreamDelayAsymmetryNsSet */
/**
   Sets the Delay Asymmetry values for a stream for the direction specified.
   This routine calls zl303xx_PtpStreamDelayAsymmetrySet() after formatting the
   parameters properly.

  Parameters:
   [in]  streamHandle   Handle to an existing stream.
   [in]  isIngress      Boolean indicating which direction the delay value
                              applies to:
                              TRUE = ingress
                              FALSE = egress
   [in]  delayNs        The delay (in nanoseconds) to apply to the specified
                              direction.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or isIngress is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex (in sub-routine).
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to lock mutex (in sub-routine).

*******************************************************************************/
zlStatusE zl303xx_PtpStreamDelayAsymmetryNsSet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_BooleanE isIngress,
      Sint32T delayNs)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_TimeInterval delayInterval = {{0,0}};

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(isIngress);
   }

   /* Convert the delay value to a TimeInterval and set the appropriate
    * direction. */
   if (status == ZL303XX_OK)
   {
      /* Convert... */
      delayInterval.scaledNanoseconds.hi = (delayNs >> SCALED_NS32_FRACT_BITS);
      delayInterval.scaledNanoseconds.lo = (delayNs << SCALED_NS32_FRACT_BITS);

      /* Apply... */
      if (isIngress == ZL303XX_TRUE)
      {
         status = zl303xx_PtpStreamDelayAsymmetrySet(streamHandle, &delayInterval, NULL);
      }
      else
      {
         status = zl303xx_PtpStreamDelayAsymmetrySet(streamHandle, NULL, &delayInterval);
      }
   }

   return status;
}  /* END zl303xx_PtpStreamDelayAsymmetryNsSet */

/* zl303xx_PtpStreamBestEffortEn */
/**
   Enables / Disables Best-Effort Unicast Negotiation for the specified
   messageType on the target stream.

  Parameters:
   [in]  streamHandle   Handle of an existing stream.
   [in]  msgType        The contract type to set.
   [in]  enable         Flag to enable or disable the functionality.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle, msgType or enable are invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamBestEffortEn(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_MessageTypeE msgType,
      zl303xx_BooleanE enable)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(enable) |
               ZL303XX_CHECK_MESSAGE_TYPE(msgType);
   }

   /* Get the stream data */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpStream->portData->clock);
   }

   /* Enable/Disable; Unlock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      pPtpStream->config.unicastNegotiation.bestEffort[msgType].enabled = enable;

      status = zl303xx_PtpClockMutexUnlock(pPtpStream->portData->clock, status);
   }

   return status;
}  /* END zl303xx_PtpStreamBestEffortEn */

/* zl303xx_PtpStreamBestEffortIntervalSet */
/**
   Sets the maximum interval (i.e. lowest packet rate) to accept for the
   specified messageType on the target stream when the Best-Effort Unicast
   Negotiation functionality is enabled.

  Parameters:
   [in]  streamHandle   Handle of an existing stream.
   [in]  msgType        The contract type to set.
   [in]  maxInterval    The maximum interval (i.e. lowest packet rate) to
                              accept for this messageType.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or msgType are invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamBestEffortIntervalSet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_MessageTypeE msgType,
      Sint8T maxInterval)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_MESSAGE_TYPE(msgType);
   }

   /* Get the stream data */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpStream->portData->clock);
   }

   /* Enable/Disable; Unlock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      pPtpStream->config.unicastNegotiation.bestEffort[msgType].intervalMax = maxInterval;

      status = zl303xx_PtpClockMutexUnlock(pPtpStream->portData->clock, status);
   }

   return status;
}  /* END zl303xx_PtpStreamBestEffortIntervalSet */

/* zl303xx_PtpStreamEventMsgExtensionSet */
/**
   Sets the messageLength pad size to add for the specified EVENT messageType.
   Any added bytes are set to a value of 0x00.

  Parameters:
   [in]  streamHandle   Handle of an existing stream.
   [in]  msgType        The EVENT messageType to append padding for.
   [in]  padZerosLen    The number of zeros to pad to the message. This must
                              be a multiple of 2 to comply to PTP requirements.
                              This pad is limited to 128 Bytes.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    streamHandle or msgType are invalid or
                                       padLength is not a multiple of 2.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamEventMsgExtensionSet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_MessageTypeE msgType,
      Uint32T padZerosLen)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Check input msgType. */
   if (status == ZL303XX_OK)
   {
      if ((status = ZL303XX_CHECK_EVENT_MSG_TYPE_VALID(msgType)) != ZL303XX_OK)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpStreamEventMsgExtensionSet: msgType (%d) must be EVENT.",
               msgType, 0,0,0,0,0);
      }
   }

   /* Check the padLength is a multiple of 2. */
   if (status == ZL303XX_OK)
   {
      if (padZerosLen & 0x01)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpStreamEventMsgExtensionSet: padLength (%d) must be even.",
               padZerosLen, 0,0,0,0,0);

         status = ZL303XX_PARAMETER_INVALID;
      }
      else if (padZerosLen > PTP_MSG_SIZE_MAX)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpStreamEventMsgExtensionSet: padLength (%d) must be <= %d.",
               padZerosLen, PTP_MSG_SIZE_MAX, 0,0,0,0);

         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Get the stream data */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpStream->portData->clock);
   }

   /* Set the Minimum Message Length; Unlock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      pPtpStream->config.msgLenPadding[msgType] = padZerosLen;

      status = zl303xx_PtpClockMutexUnlock(pPtpStream->portData->clock, status);
   }

   return status;
}  /* END zl303xx_PtpStreamEventMsgExtensionSet */

/* zl303xx_PtpStreamEventTlvPadLengthSet */
/**
   Sets the overall length of the PAD TLV to add for the specified EVENT
   messageType. The minimum size must be 4 to account for the TYPE and LENGTH
   fields. Any remaining bytes are set to a value of 0x00.

  Parameters:
   [in]  streamHandle   Handle of an existing stream.
   [in]  msgType        The EVENT messageType to append a PAD TLV.
   [in]  ttlPadTlvLen   The total number of bytes to add to the message.
                              - Must be 4 or more bytes
                              - Must be a multiple of 2 (PTP requirements).
                              - Total TLV limited to 128 Bytes.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    streamHandle or msgType are invalid or
                                       ttlPadTlvLen is not valid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamEventTlvPadLengthSet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_MessageTypeE msgType,
      Uint32T ttlPadTlvLen)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Check input msgType. */
   if (status == ZL303XX_OK)
   {
      if ((status = ZL303XX_CHECK_EVENT_MSG_TYPE_VALID(msgType)) != ZL303XX_OK)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpStreamEventTlvPadLengthSet: msgType (%d) must be EVENT.",
               msgType, 0,0,0,0,0);
      }
   }

   /* Check the ttlPadTlvLen. */
   if (status == ZL303XX_OK)
   {
      if (ttlPadTlvLen & 0x01)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpStreamEventTlvPadLengthSet: ttlPadTlvLen (%d) must be even.",
               ttlPadTlvLen, 0,0,0,0,0);

         status = ZL303XX_PARAMETER_INVALID;
      }
      else if (ttlPadTlvLen > PTP_MSG_SIZE_MAX)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpStreamEventTlvPadLengthSet: ttlPadTlvLen (%d) must be <= %d.",
               ttlPadTlvLen, PTP_MSG_SIZE_MAX, 0,0,0,0);

         status = ZL303XX_PARAMETER_INVALID;
      }
      else if (ttlPadTlvLen < ZL303XX_PTP_TLV_BASE_LEN)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpStreamEventTlvPadLengthSet: ttlPadTlvLen (%d) must be >= %d.",
               ttlPadTlvLen, ZL303XX_PTP_TLV_BASE_LEN, 0,0,0,0);

         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Get the stream data */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpStream->portData->clock);
   }

   /* Set the Minimum Message Length; Unlock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      pPtpStream->config.padTlvLen[msgType] = ttlPadTlvLen;

      status = zl303xx_PtpClockMutexUnlock(pPtpStream->portData->clock, status);
   }

   return status;
}  /* END zl303xx_PtpStreamEventTlvPadLengthSet */

/* zl303xx_PtpStreamHandleGetFromDestAddr */
/**
   Returns the stream handle for a given destination port address.

  Parameters:
   [in]   pDestAddr      Destination port address to search for.
   [out]  pStreamHandle  Stream handle of match.

  Return Value:
     ZL303XX_OK                     Success.
     ZL303XX_INVALID_POINTER        pDestAddr or pStreamHandle is NULL.
     ZL303XX_TABLE_ENTRY_NOT_FOUND  No stream match was found.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamHandleGetFromDestAddr(zl303xx_PortAddress *pDestAddr,
                                                 zl303xx_PtpStreamHandleT *pStreamHandle)
{
   zlStatusE status = ZL303XX_OK;
   Uint16T streamId = 0;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pDestAddr) |
               ZL303XX_CHECK_POINTER(pStreamHandle);
   }

   if (status == ZL303XX_OK)
   {
      zlStatusE mStatus = ZL303XX_ERROR;

      if (status == ZL303XX_OK)
      {
         mStatus = zl303xx_PtpNodeMutexLock();
         status = mStatus;
      }

      for (streamId = 0; streamId < ZL303XX_PTP_NUM_STREAMS_MAX; streamId++)
      {
         if ((Zl303xx_PtpStreamArray[streamId] != NULL) &&
             (zl303xx_PtpComparePortAddress(&Zl303xx_PtpStreamArray[streamId]->config.destAddr,
                                            pDestAddr) == 0))
         {
            *pStreamHandle = Zl303xx_PtpStreamArray[streamId]->clockStreamHandle;
            break;
         }
      }

      if (mStatus == ZL303XX_OK)
      {
         status = zl303xx_PtpNodeMutexUnlock(status);
      }

      if (streamId == ZL303XX_PTP_NUM_STREAMS_MAX)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "   (ERROR) No matching stream found.", 0,0,0,0,0,0);
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "   (ERROR) No matching stream found.", 0,0,0,0,0,0);
         ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_ENGINE, 5)
         {
            zl303xx_PtpTracePortAddress(pDestAddr);
         }
         status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
      }
   }

   return status;
}

/* zl303xx_PtpStreamCurrentDSGet */
/**
   Returns the CurrentDS of a stream.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [out]  pCurrentDS    The CurrentDS of the stream.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pCurrentDS is NULL.
     ZL303XX_PARAMETER_INVALID  streamHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamCurrentDSGet(zl303xx_PtpStreamHandleT streamHandle,
                                        zl303xx_CurrentDS *pCurrentDS)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pCurrentDS);
   }

   /* Get a pointer to the stream structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   /* Convert the internal CurrentDS format to the standard CurrentDS format. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpConvertCurrentDSInternal(&stream->farEndCurrentDS, pCurrentDS);
   }

   return status;
}

/* zl303xx_PtpStreamParentDSGet */
/**
   Returns the ParentDS of a stream.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [out]  pParentDS     The ParentDS of the stream.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pParentDS is NULL.
     ZL303XX_PARAMETER_INVALID  streamHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamParentDSGet(zl303xx_PtpStreamHandleT streamHandle,
                                       zl303xx_ParentDS *pParentDS)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pStream = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pParentDS);
   }

   /* Get a pointer to the stream structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pStream);
   }

   /* Map the stream's internal ParentDS to the standard structure. */
   if (status == ZL303XX_OK)
   {
      *pParentDS = pStream->farEndParentDS.u.v2;
   }

   return status;
}

/* zl303xx_PtpStreamCountersGet */
/**
   Returns the message counter data collected by a stream.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [out]  pCounters     Structure containing all PTP message counters.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pCounters is NULL.
     ZL303XX_PARAMETER_INVALID  streamHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamCountersGet(zl303xx_PtpStreamHandleT streamHandle,
                                       zl303xx_PtpStreamCountersS *pCounters)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pStream;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pCounters);
   }

   /* Get a pointer to the stream structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pStream);
   }

   /* Copy the stream's counters into the supplied structure. */
   if (status == ZL303XX_OK)
   {
      *pCounters = pStream->counters;
   }

   return status;
}

/* zl303xx_PtpStreamCountersReset */
/**
   Initializes all stream message counters to 0.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamCountersReset(zl303xx_PtpStreamHandleT streamHandle)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pStream;

   /* Get a pointer to the stream structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pStream);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pStream->portData->clock);
   }

   if (status == ZL303XX_OK)
   {
      /* Reset all message counters. */
      zl303xx_PtpStreamCountersInit(pStream);

      /* Unlock the clock mutex. */
      status = zl303xx_PtpClockMutexUnlock(pStream->portData->clock, status);
   }

   return status;
}

/* zl303xx_PtpStreamLogSyncIntvlSet */
/**
   Sets the log2 Sync interval for a stream.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   logSyncIntvl  Sync interval to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or logSyncIntvl is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamLogSyncIntvlSet(zl303xx_PtpStreamHandleT streamHandle,
                                           Sint8T logSyncIntvl)
{
   return zl303xx_PtpStreamLogMsgIntvlSet(streamHandle, ZL303XX_MSG_ID_SYNC,
                                          logSyncIntvl);
}

/* zl303xx_PtpStreamLogDelayIntvlSet */
/**
   Sets the log2 Delay_Req/Resp interval for a stream. This function can be
   used on a multicast master to limit the Delay_Req rate of a slave. The
   logDelayIntvl value will be used in multicast Delay_Resp messages, and a
   slave must not send Delay_Reqs faster than this. This function will update
   the logMinDelayReqInterval value on a unicast master, but will otherwise not
   affect its operation. Unicast Delay_Resp messages advertise 0x7F in their
   logMessageInterval field.

  Parameters:
   [in]   streamHandle   Handle to an existing stream.
   [in]   logDelayIntvl  Delay_Req/Resp interval to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or logDelayIntvl is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamLogDelayIntvlSet(zl303xx_PtpStreamHandleT streamHandle,
                                            Sint8T logDelayIntvl)
{
   return zl303xx_PtpStreamLogMsgIntvlSet(streamHandle, ZL303XX_MSG_ID_DELAY_RESP,
                                          logDelayIntvl);
}

/* zl303xx_PtpStreamLogPdelayIntvlSet */
/**
   Sets the log2 Peer Delay_Req/Resp interval for a stream. This function can be
   used on a multicast master to limit the Peer Delay_Req rate of a slave. The
   logPdelayIntvl value will be used in multicast Peer Delay_Resp messages, and a
   slave must not send Peer Delay_Reqs faster than this. This function will update
   the logMinPdelayReqInterval value on a unicast master, but will otherwise not
   affect its operation. Unicast Peetr Delay_Resp messages advertise 0x7F in their
   logMessageInterval field.

  Parameters:
   [in]  streamHandle      Handle to an existing stream.
   [in]  logPdelayIntvl    Peer Delay_Req/Resp interval to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or logDelayIntvl is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamLogPdelayIntvlSet(zl303xx_PtpStreamHandleT streamHandle,
                                             Sint8T logPdelayIntvl)
{
   return zl303xx_PtpStreamLogMsgIntvlSet(streamHandle, ZL303XX_MSG_ID_PEER_DELAY_RESP,
                                          logPdelayIntvl);
}

/* zl303xx_PtpStreamLogAnnounceIntvlSet */
/**
   Sets the log2 Announce interval for a stream.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   logAnncIntvl  Announce interval to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or logAnncIntvl is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamLogAnnounceIntvlSet(zl303xx_PtpStreamHandleT streamHandle,
                                               Sint8T logAnncIntvl)
{
   return zl303xx_PtpStreamLogMsgIntvlSet(streamHandle, ZL303XX_MSG_ID_ANNOUNCE,
                                          logAnncIntvl);
}

/* zl303xx_PtpStreamLogMsgIntvlSet */
/** 

   Helper function to set a stream's message rate. If the stream is configured
   for unicast negotiation the appropriate contract request function may be
   called instead.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   msgType       Message type to change.
   [in]   log2Intvl     Log2 interval to set for msgType.

*******************************************************************************/
static zlStatusE zl303xx_PtpStreamLogMsgIntvlSet(zl303xx_PtpStreamHandleT streamHandle,
                                                 zl303xx_MessageTypeE msgType,
                                                 Sint8T log2Intvl)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MessageTypeToIndex(msgType, &uniNegMsgType);
   }

   /* Do any Profile specific message rate checks here (for all multicast, unicast
    * or static configurations). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpProfileMsgIntervalCheck(
                           stream->portData->clock->config.profile,
                           stream->portData->clock->config.profileStrict,
                           msgType,
                           &log2Intvl);
   }

   if (status == ZL303XX_OK)
   {
      /* If this stream is configured for unicast negotiation requests, use the
       * contract request function. */
      if (stream->config.unicast && stream->portData->config.unicastNegotiation.enabled)
      {
         zl303xx_PtpContractS contract = stream->contractRx[uniNegMsgType].req;
         contract.interval = log2Intvl;
         return zl303xx_PtpContractRequest(streamHandle, msgType, &contract);
      }

      /* This stream is multicast or static unicast. Update the appropriate
       * config parameter. */
      status = zl303xx_PtpClockMutexLock(stream->portData->clock);
   }

   if (status == ZL303XX_OK)
   {
      zl303xx_PtpStreamConfigIntvlSet(stream, msgType, log2Intvl);
      zl303xx_PtpV2SetRequiredTimers(stream);

      /* Any new ANNOUNCE rate may affect the Clock's BMCA update timer. */
      if (msgType == ZL303XX_MSG_ID_ANNOUNCE)
         zl303xx_PtpClockTimersInit(stream->portData->clock);

      status = zl303xx_PtpClockMutexUnlock(stream->portData->clock, status);
   }

   return status;
}  /* END zl303xx_PtpStreamLogMsgIntvlSet */

/* zl303xx_PtpStreamDelayReqTimeRequiredSet */
/**
   Set whether the local clock must set the local Time prior to issuing a
   Delay Request message.

  Parameters:
   [in]  streamHandle   Handle to an existing stream.
   [in]  timeRequired   Boolean indicating if the Time is required to be
                              set on the local clock before a delay request
                              can be issued.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or timeRequired is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamDelayReqTimeRequiredSet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_BooleanE timeRequired)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream;

   /* Check the boolean value. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(timeRequired);
   }

   /* Get the stream data */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpStream->portData->clock);
   }

   if (status == ZL303XX_OK)
   {
      pPtpStream->config.delayReqTimeSetRequired = timeRequired;

      status = zl303xx_PtpClockMutexUnlock(pPtpStream->portData->clock, status);
   }

   return status;
}

/* zl303xx_PtpStreamAnnounceTimoutSet */
/**
   Sets the announce receipt timeout value for a stream.

  Parameters:
   [in]   streamHandle            Handle to an existing stream.
   [in]   announceReceiptTimeout  Timeout value to set. This value is
                                       multiplied by logAnnounceInterval to
                                       calculate the actual timeout.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or announceReceiptTimeout is
                                       invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamAnnounceTimoutSet(zl303xx_PtpStreamHandleT streamHandle,
                                             Uint8T announceReceiptTimeout)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream;

   /* Get the stream data */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   /* Check input parameters against the configured profile. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpProfileAnncReceiptTimeoutCheck(
                     stream->portData->clock->config.profile,
                     stream->portData->clock->config.profileStrict,
                     &announceReceiptTimeout);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(stream->portData->clock);
   }

   if (status == ZL303XX_OK)
   {
      stream->config.announceReceiptTimeout = announceReceiptTimeout;

      /* Restart the ANNOUNCE_RECEIPT_TIMER if the value has changed. */
      zl303xx_PtpV2SetRequiredTimers(stream);

      status = zl303xx_PtpClockMutexUnlock(stream->portData->clock, status);
   }

   return status;
}  /* END zl303xx_PtpStreamAnnounceTimoutSet */

/* zl303xx_PtpStreamUncalibratedIntervalSet */
/**
   Set the number of Announce Intervals that a Stream must wait in the
   UNCALIBRATED state before transitioning to the SLAVE state.

  Parameters:
   [in]  streamHandle   Handle to an existing stream.
   [in]  anncIntervals  The number of Announce Intervals that a Stream must
                              wait in the UNCALIBRATED state before moving
                              to the SLAVE state.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamUncalibratedIntervalSet(
      zl303xx_PtpStreamHandleT streamHandle,
      Uint8T anncIntervals)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream;

   /* Get the stream data */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpStream->portData->clock);
   }

   if (status == ZL303XX_OK)
   {
      pPtpStream->config.uncalibrated.anncIntervals = anncIntervals;

      /* Evaluate the new value. */
      zl303xx_PtpV2SetRequiredTimers(pPtpStream);

      status = zl303xx_PtpClockMutexUnlock(pPtpStream->portData->clock, status);
   }

   return status;
}  /* END zl303xx_PtpStreamUncalibratedIntervalSet */

/* zl303xx_PtpStreamUncalibratedLockRequiredSet */
/**
   Set whether a LOCK is required before a Stream can transition from the
   UNCALIBRATED state to the SLAVE state.

  Parameters:
   [in]  streamHandle   Handle to an existing stream.
   [in]  lockRequired   Boolean indicating if a LOCK is required before a
                              stream can transition from the UNCALIBRATED
                              state to the SLAVE state.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or lockRequired is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamUncalibratedLockRequiredSet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_BooleanE lockRequired)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream;

   /* Check the boolean value. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(lockRequired);
   }

   /* Get the stream data */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpStream->portData->clock);
   }

   if (status == ZL303XX_OK)
   {
      pPtpStream->config.uncalibrated.lockRequired = lockRequired;

      status = zl303xx_PtpClockMutexUnlock(pPtpStream->portData->clock, status);
   }

   return status;
}

/* zl303xx_PtpStreamUniNegDurationSet */
/**
   Sets the duration, in seconds, for all unicast negotiation contracts.

  Parameters:
   [in]   streamHandle   Handle to an existing stream.
   [in]   durationSec    Contract duration to use, in seconds.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or durationSec is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamUniNegDurationSet(zl303xx_PtpStreamHandleT streamHandle,
                                             Uint32T durationSec)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream;
   zl303xx_PtpContractS contract;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;
   zl303xx_MessageTypeE msgType;

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   for (uniNegMsgType = PTP_NEGOTIATED_MSG_ID_SYNC;
        ((uniNegMsgType < PTP_NEGOTIATED_NUM_MSG_TYPES) && (status == ZL303XX_OK));
        uniNegMsgType++)
   {
      status = zl303xx_PtpV2IndexToMessageType(uniNegMsgType, &msgType);

      if (status == ZL303XX_OK)
      {
         contract = stream->contractRx[uniNegMsgType].req;
         contract.durationSec = durationSec;
         status = zl303xx_PtpContractRequest(streamHandle, msgType, &contract);
      }
   }

   return status;
}

/* zl303xx_PtpStreamUniNegRenewSet */
/**
   Sets the pre-expiry renewal for unicast negotiation message flow contracts,
   in seconds.

  Parameters:
   [in]   streamHandle  Handle to a previously created stream.
   [in]   renewSec      Pre-expiry renewal, in seconds.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or renewSec is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamUniNegRenewSet(zl303xx_PtpStreamHandleT streamHandle,
                                          Uint32T renewSec)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream;

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   if (status == ZL303XX_OK)
   {
      if (stream->portData->config.unicastNegotiation.enabled == ZL303XX_FALSE)
      {
         ZL303XX_TRACE_ALWAYS("Unicast negotiation not enabled for this stream/port.", 0,0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2CheckUnicastSlaveData(
                     stream->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].req.durationSec,
                     stream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].req.durationSec,
                     stream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].req.durationSec,
                     stream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].req.durationSec,
                     renewSec);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(stream->portData->clock);
   }

   if (status == ZL303XX_OK)
   {
      stream->config.unicastNegotiationRenew = renewSec;
      stream->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].renewSec = renewSec;
      stream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].renewSec = renewSec;
      stream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].renewSec = renewSec;
      stream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].renewSec = renewSec;

      status = zl303xx_PtpClockMutexUnlock(stream->portData->clock, status);
   }

   return status;
}  /* END zl303xx_PtpStreamUniNegRenewSet */

/* zl303xx_PtpStreamUniNegMonitorTimingSet */
/**
   Sets a stream::config::unicastMonitorTiming value.
   - FALSE (default): request only ANNOUNCE service when a UMT entry exists,
   - TRUE: request ANNOUNCE, SYNC and DELAY services as configured by the UMT
           (even in MASTER mode).

  Parameters:
   [in]  streamHandle            Handle to an existing stream.
   [in]  bUniNegMonitorTiming    As listed above.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    streamHandle or bUniNegMonitorTiming is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamUniNegMonitorTimingSet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_BooleanE bUniNegMonitorTiming)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(bUniNegMonitorTiming);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpStream->portData->clock);
   }
   /* Set the Port configuration. */
   if (status == ZL303XX_OK)
   {
      pPtpStream->config.unicastMonitorTiming = bUniNegMonitorTiming;
      /* The DELAY_REQ Timer may need to be enabled or disabled. */
      zl303xx_PtpV2SetRequiredTimers(pPtpStream);
      status = zl303xx_PtpClockMutexUnlock(pPtpStream->portData->clock, status);
   }

   /* This Stream configuration change does not affect BMCA so do not initiate
    * a BMCA UPDATE event.   */

   return status;
}  /* END zl303xx_PtpStreamUniNegMonitorTimingSet */

/* zl303xx_PtpStreamUniNegDenialRetrySet */
/**
   Sets a Stream's Unicast negotiation retry configuration when a server DENIES
   the request.

  Parameters:
   [in]  streamHandle   Handle to an existing stream.
   [in]  retryCount     Number of times to retry the contract request before
                              locking out the stream. Use 0xFFFFFFFF to disable.
   [in]  lockoutSec     Period (in Seconds) to lock out the contract after
                              the number of retries has expired. Use 0 to disable.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    streamHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamUniNegDenialRetrySet(
      zl303xx_PtpStreamHandleT streamHandle,
      Uint32T retryCount,
      Uint32T lockoutSec)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream);
   }

   /* Lock the MUTEX since multiple parameters are set. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpStream->portData->clock);
   }

   /* Set the Stream configuration values. */
   if (status == ZL303XX_OK)
   {
      pPtpStream->config.unicastNegotiation.retry.denial.logQueryCount = retryCount;
      pPtpStream->config.unicastNegotiation.retry.denial.lockoutSec = lockoutSec;

      /* Reinitialize with the new values. */
      {
         Uint8T msgIdx;
         for (msgIdx = PTP_NEGOTIATED_MSG_ID_SYNC; msgIdx < PTP_NEGOTIATED_NUM_MSG_TYPES; msgIdx++)
         {
            zl303xx_PtpContractMsgRetryStatusInit(&pPtpStream->contractRx[msgIdx].retry,
                                                  &pPtpStream->config.unicastNegotiation.retry);
         }
      }

      /* Release the Mutex */
      status = zl303xx_PtpClockMutexUnlock(pPtpStream->portData->clock, status);
   }

   return status;
}  /* END zl303xx_PtpStreamUniNegDenialRetrySet */

/* zl303xx_PtpStreamUniNegTimeoutRetrySet */
/**
   Sets a Stream's Unicast negotiation retry configuration when a request to a
   server experiences a TIMEOUT.

  Parameters:
   [in]  streamHandle   Handle to an existing stream.
   [in]  retryCount     Number of times to retry the contract request before
                              locking out the stream. Use 0xFFFFFFFF to disable.
   [in]  lockoutSec     Period (in Seconds) to lock out the contract after
                              the number of retries has expired. Use 0 to disable.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    streamHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamUniNegTimeoutRetrySet(
      zl303xx_PtpStreamHandleT streamHandle,
      Uint32T retryCount,
      Uint32T lockoutSec)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream);
   }

   /* Lock the MUTEX since multiple parameters are set. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpStream->portData->clock);
   }

   /* Set the Stream configuration values. */
   if (status == ZL303XX_OK)
   {
      pPtpStream->config.unicastNegotiation.retry.timeout.logQueryCount = retryCount;
      pPtpStream->config.unicastNegotiation.retry.timeout.lockoutSec = lockoutSec;

      /* Reinitialize with the new values. */
      {
         Uint8T msgIdx;
         for (msgIdx = PTP_NEGOTIATED_MSG_ID_SYNC; msgIdx < PTP_NEGOTIATED_NUM_MSG_TYPES; msgIdx++)
         {
            zl303xx_PtpContractMsgRetryStatusInit(&pPtpStream->contractRx[msgIdx].retry,
                                                  &pPtpStream->config.unicastNegotiation.retry);
         }
      }

      /* Release the Mutex */
      status = zl303xx_PtpClockMutexUnlock(pPtpStream->portData->clock, status);
   }

   return status;
}  /* END zl303xx_PtpStreamUniNegTimeoutRetrySet */

/* zl303xx_PtpStreamMaxClockClassSet */
/**
   Sets the maximum clock class used to generate PTSF-QLTooLow.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   max           Maximum clock class to set. Set to 0 to disable.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_ROTX_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamMaxClockClassSet(zl303xx_PtpStreamHandleT streamHandle,
                                            Uint8T max)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream;

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(stream->portData->clock);
   }

   if (status == ZL303XX_OK)
   {
      stream->config.maxClockClass = max;
      status = zl303xx_PtpClockMutexUnlock(stream->portData->clock, status);
   }

   return status;
}

/* zl303xx_PtpStreamStateGet */
/**
   Returns the operating state of a stream.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [out]  state         Operating state of the stream.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  streamHandle is invalid.
     ZL303XX_INVALID_POINTER    state is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamStateGet(zl303xx_PtpStreamHandleT streamHandle,
                                    zl303xx_PtpStreamOperatingStateE *state)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(state);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   if (status == ZL303XX_OK)
   {
      *state = stream->operState;
   }

   return status;
}

/* zl303xx_PtpStreamNextTxSequenceIdGet */
/**
   Returns an array of the next transmitted sequenceId for all messageTypes.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [out]  seqId         Array of next sequenceIds.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  streamHandle is invalid.
     ZL303XX_INVALID_POINTER    seqId is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamNextTxSequenceIdGet(zl303xx_PtpStreamHandleT streamHandle,
                                               Uint16T seqId[ZL303XX_PTP_V2_NUM_MSG_IDS])
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(seqId);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   if (status == ZL303XX_OK)
   {
      OS_MEMCPY(seqId, stream->config.nextTxSequenceId,
             sizeof(Uint16T) * ZL303XX_PTP_V2_NUM_MSG_IDS);
   }

   return status;
}

/* zl303xx_PtpTsLogConfigStructInit */
/**
   Fills a zl303xx_PtpTsLogConfigS structure with invalid values. They must be
   changed before calling zl303xx_PtpTsLogConfigSet().

  Parameters:
   [in]   pConfig  Structure to initialize.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pConfig is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpTsLogConfigStructInit(zl303xx_PtpTsLogConfigS *pConfig)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pConfig);
   }

   if (status == ZL303XX_OK)
   {
      pConfig->durationSec = (Uint32T)ZL303XX_PTP_INVALID;
      pConfig->callback = NULL;
   }

   return status;
}

/* zl303xx_PtpTsLogConfigSet */
/**
   Starts/stops logging of time stamps received by PTP to a callback function.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   pConfig       Logging configuration.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       pConfig or pConfig->callback is NULL.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_ROTX_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpTsLogConfigSet(zl303xx_PtpStreamHandleT streamHandle,
                                    zl303xx_PtpTsLogConfigS *pConfig)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtpTsLogConfigSet";
   zl303xx_ClockStreamS *stream;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pConfig);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   if (status == ZL303XX_OK)
   {
      if (pConfig->durationSec == 0)
      {
         if (stream->tsLogging.remainingSec > 0)
         {
            ZL303XX_TRACE_ALWAYS("%s: durationSec=0, logging will be stopped.",
                                 fnName, 0,0,0,0,0);
         }
      }
      else
      {
         status = ZL303XX_CHECK_POINTER(pConfig->callback);
      }
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(stream->portData->clock);
   }

   if (status == ZL303XX_OK)
   {
      stream->tsLogging.remainingSec = pConfig->durationSec;
      stream->tsLogging.callback = (pConfig->durationSec > 0) ? pConfig->callback : NULL;

      status = zl303xx_PtpClockMutexUnlock(stream->portData->clock, status);
   }

   return status;
}

/* zl303xx_PtpContractStructInit */
/**
   Fills a zl303xx_PtpContractS structure with default values.

  Parameters:
   [in]   pContract  Structure to initialize.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pContract is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpContractStructInit(zl303xx_PtpContractS *pContract)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pContract);
   }

   if (status == ZL303XX_OK)
   {
      pContract->interval = 0;
      pContract->durationSec = ZL303XX_PTP_DEFAULT_STREAM_UNI_NEG_DUR;
   }

   return status;
}

/* zl303xx_PtpContractRequest */
/**
   Configures a stream to request unicast service for a specific messageType.
   The stream must have a UMT entry associated with it (i.e., it was created
   by calling zl303xx_PtpUmtEntryAdd()).

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   msgType       The messageType service to request.
   [in]   pContract     (Optional) Additional parameters that can be set on
                             a per-contract basis. If NULL, the previous
                             contract parameters are used.

  Notes:  A new contract will not be requested if no parameters have changed.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or msgType is invalid. The stream
                                      is not associated with a UMT entry.
                                      pContract->durationSec is 0.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpContractRequest(zl303xx_PtpStreamHandleT streamHandle,
                                     zl303xx_MessageTypeE msgType,
                                     zl303xx_PtpContractS *pContract)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtpContractRequest";
   zl303xx_ClockStreamS *stream;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MessageTypeToIndex(msgType, &uniNegMsgType);
   }

   /* Get a pointer to the stream. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   /* Check contract parameters, if provided. */
   if (pContract != NULL)
   {
      /* DURATION Checks */
      if (status == ZL303XX_OK)
      {
         if (pContract->durationSec == 0)
         {
            ZL303XX_TRACE_ERROR("%s: Invalid durationSec=%lu.",
                  fnName, pContract->durationSec, 0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
         }
      }

      /* Profile specific DURATION Checks */
      if (status == ZL303XX_OK)
      {
         status = zl303xx_PtpProfileUnicastDurationCheck(
               stream->portData->clock->config.profile,
               stream->portData->clock->config.profileStrict,
               &pContract->durationSec);
      }

      /* Profile specific MESSAGE INTERVAL Checks */
      if (status == ZL303XX_OK)
      {
         status = zl303xx_PtpProfileMsgIntervalCheck(
                              stream->portData->clock->config.profile,
                              stream->portData->clock->config.profileStrict,
                              msgType,
                              &pContract->interval);
      }
   }

   /* Check if a UMT entry exists for this stream. */
   if (status == ZL303XX_OK)
   {
      if (stream->pUmtEntry == NULL)
      {
         ZL303XX_TRACE_ERROR("%s: Stream %lu has no associated UMT entry.",
                             fnName, streamHandle, 0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(stream->portData->clock);
   }

   /* Update the requested services. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpUmtEntryConfigS *entryConfig = &stream->pUmtEntry->entryConfig;

      if (msgType == ZL303XX_MSG_ID_SYNC)
         entryConfig->reqSync = ZL303XX_TRUE;
      else if ((msgType == ZL303XX_MSG_ID_DELAY_RESP) &&
               (stream->portData->config.delayMechanism == ZL303XX_DELAY_MECHANISM_E2E))
         entryConfig->reqDelayResp = ZL303XX_TRUE;
      else if ((msgType == ZL303XX_MSG_ID_PEER_DELAY_RESP) &&
               (stream->portData->config.delayMechanism == ZL303XX_DELAY_MECHANISM_P2P))
         entryConfig->reqPdelayResp = ZL303XX_TRUE;
      else if (msgType == ZL303XX_MSG_ID_ANNOUNCE)
         entryConfig->reqAnnc = ZL303XX_TRUE;

      if (pContract != NULL)
      {
         /* Only negotiate a new contract if parameters have changed. */
         if ((stream->contractRx[uniNegMsgType].req.interval != pContract->interval) ||
             (stream->contractRx[uniNegMsgType].req.durationSec != pContract->durationSec))
         {
            stream->contractRx[uniNegMsgType].req = *pContract;
            stream->contractRx[uniNegMsgType].renegotiate = ZL303XX_TRUE;
         }
      }

      zl303xx_PtpUmtEntryServicesCheck(stream->portData, stream->pUmtEntry);

      if ((stream->portData->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1) ||
          (stream->portData->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_2))
      {
         zl303xx_PtpTelecomPtsfEval(streamHandle, entryConfig->reqAnnc,
                                    entryConfig->reqSync, entryConfig->reqDelayResp,
                                    entryConfig->reqPdelayResp);
      }

      status = zl303xx_PtpClockMutexUnlock(stream->portData->clock, status);
   }

   return status;
}

/* zl303xx_PtpContractCancel */
/**
   Configures a stream to cancel unicast service for a specific messageType.
   All future REQUESTS received on this stream will be DENIED until the
   zl303xx_PtpTxContractAllow() function is used to re-enable GRANT service.

   Cancelling a messageType that has not been requested yet (e.g., Announce has
   been granted, but a REQUEST TLV for Sync has not been sent yet) will prevent
   that messageType from being requested.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   msgType       The messageType service to cancel.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or msgType is invalid. The
                                       stream is not associated with a UMT entry.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpContractCancel(zl303xx_PtpStreamHandleT streamHandle,
                                    zl303xx_MessageTypeE msgType)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_ClockStreamS *stream;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;

   /* Check msgType by converting it. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MessageTypeToIndex(msgType, &uniNegMsgType);
   }

   /* Get a pointer to the stream. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(stream->portData->clock);
      status = mStatus;
   }

   if (status == ZL303XX_OK)
   {
      /* RX contracts take precedence over TX contracts */
      /* If RX and TX contracts are IDLE, calling the function below will set
       * the "request service" flag to FALSE. */
      if (stream->contractRx[uniNegMsgType].state != ZL303XX_PTP_CONTRACT_RX_IDLE ||
          stream->contractTx[uniNegMsgType].state == ZL303XX_PTP_CONTRACT_TX_IDLE)
      {
         status = zl303xx_PtpContractRxCancel(stream, msgType);
      }
      /* If a TX contract exists, change the state to CANCELLING. */
      else
      {
         zl303xx_PtpContractTxStateSet(stream, uniNegMsgType,
                                       ZL303XX_PTP_CONTRACT_TX_CANCELLING);
      }
   }

   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(stream->portData->clock, status);
   }

   return status;
}

/* zl303xx_PtpContractManualCancel */
/**
   Manually send a CANCEL TLV to a destination address. If a stream matching the
   destination address already exists, the function reverts to calling
   zl303xx_PtpContractCancel(). If no stream exists, only a single CANCEL TLV will
   be sent.

  Parameters:
   [in]   portHandle  Handle to an existing port.
   [in]   msgType     The messageType to send a CANCEL TLV for.
   [in]   pDestAddr   Destination address of the Signaling message.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       pDestAddr is NULL.
     ZL303XX_PARAMETER_INVALID     portHandle, msgType, or pDestAddr is
                                       invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpContractManualCancel(zl303xx_PtpPortHandleT portHandle,
                                          zl303xx_MessageTypeE msgType,
                                          zl303xx_PortAddress *pDestAddr)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *port;
   zl303xx_ClockStreamS *stream;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pDestAddr);
   }
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &port);
   }
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MessageTypeToIndex(msgType, &uniNegMsgType);
   }

   if (status == ZL303XX_OK)
   {
      /* If a stream matching the dest. addr already exists with an active
       * RX or TX contract, use the default contract cancel function. */
      stream = zl303xx_PtpStreamExistsOnPort(port, pDestAddr);
      if ((stream != NULL) &&
          (stream->contractRx[uniNegMsgType].state != ZL303XX_PTP_CONTRACT_RX_IDLE ||
           stream->contractTx[uniNegMsgType].state != ZL303XX_PTP_CONTRACT_TX_IDLE))
      {
         return zl303xx_PtpContractCancel(stream->clockStreamHandle, msgType);
      }

      status = zl303xx_PtpPortAddressCheck(pDestAddr);
   }

   /* Stream does not exist or has no active contracts for this messageType */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpSendManualCancel(port, stream, msgType, pDestAddr);
   }

   return status;
}

/* zl303xx_PtpTxContractStop */
/**
   Silently stops unicast message service. No CANCEL TLV will be sent to inform
   the grantee that the service is being stopped. This function is intended to
   be used to implement PTP redundancy.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   msgType       The messageType service to stop.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or msgType is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpTxContractStop(zl303xx_PtpStreamHandleT streamHandle,
                                    zl303xx_MessageTypeE msgType)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MessageTypeToIndex(msgType, &uniNegMsgType);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(stream->portData->clock);
   }

   if (status == ZL303XX_OK)
   {
      /* Changing the state of the TX flow will stop the transmit timer */
      zl303xx_PtpContractTxStateSet(stream, uniNegMsgType,
                                    ZL303XX_PTP_CONTRACT_TX_IDLE);

      status = zl303xx_PtpClockMutexUnlock(stream->portData->clock, status);
   }

   return status;
}

/* zl303xx_PtpTxContractCancel */
/**
   Indefinitely cancels unicast message service by issuing a CANCEL TLV to
   inform the grantee that the msgType service is being cancelled. All future
   REQUESTS received on this stream will be DENIED until the
   zl303xx_PtpTxContractAllow() function is used to re-enable GRANT service.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   msgType       The messageType service to cancel.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or msgType is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpTxContractCancel(zl303xx_PtpStreamHandleT streamHandle,
                                    zl303xx_MessageTypeE msgType)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MessageTypeToIndex(msgType, &uniNegMsgType);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(stream->portData->clock);
   }

   if (status == ZL303XX_OK)
   {
      /* This will cancel the current TX Contract & Deny any future REQUESTS. */
      zl303xx_PtpContractTxStateSet(stream, uniNegMsgType,
                                    ZL303XX_PTP_CONTRACT_TX_CANCELLING);

      status = zl303xx_PtpClockMutexUnlock(stream->portData->clock, status);
   }

   return status;
}

/* zl303xx_PtpTxContractAllow */
/**
   Configures a stream to allow (or re-enable) unicast service for a specific
   messageType. The granting of a service may have been blocked by previously
   running either of the _dev_PtpContractCancel() or
   zl303xx_PtpTxContractCancel() commands.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   msgType       The messageType service to re-enable.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or msgType is invalid. The
                                       stream is not associated with a UMT entry.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpTxContractAllow(zl303xx_PtpStreamHandleT streamHandle,
                                     zl303xx_MessageTypeE msgType)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream;

   /* Get a pointer to the stream. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(stream->portData->clock);
   }

   if (status == ZL303XX_OK)
   {
      /* Re-enable future TX Contract REQUESTS. */
      zl303xx_PtpGrantEntryUpdate(stream->portData, &stream->config.destAddr,
                                  msgType, ZL303XX_TRUE);

      status = zl303xx_PtpClockMutexUnlock(stream->portData->clock, status);
   }

   return status;
}

/* zl303xx_PtpStreamSubStateToStr */
/**

   Converts a stream sub-state enum value to a string.

  Parameters:
   [in]   state   State to convert.

  Return Value:  (const char *)  The stream sub-state as a string.

*******************************************************************************/
const char *zl303xx_PtpStreamSubStateToStr(zl303xx_PtpStreamSubStateE state)
{
   /* If the state is not valid, use the last index of the array ("INVALID"). */
   if (state > PTP_STREAM_SUBSTATE_SLAVE)
   {
      state = ZL303XX_ARRAY_SIZE(Zl303xx_PtpStreamSubStateStr) - 1;
   }

   return Zl303xx_PtpStreamSubStateStr[state];
}

/* zl303xx_PtpContractRxCancel */
/** 

   Helper function for zl303xx_PtpContractCancel(). Cancels an RX contract for
   a given messageType.

  Parameters:
   [in]   stream   Stream with contract to be cancelled.
   [in]   msgType  messageType service to cancel.

*******************************************************************************/
static zlStatusE zl303xx_PtpContractRxCancel(zl303xx_ClockStreamS *stream,
                                             zl303xx_MessageTypeE msgType)
{
   zlStatusE status = ZL303XX_OK;

   /* Check if a UMT entry exists for this stream. */
   if (status == ZL303XX_OK)
   {
      if (stream->pUmtEntry == NULL)
      {
         ZL303XX_TRACE_ERROR("zl303xx_PtpContractRxCancel: Stream %u has no associated UMT entry.",
                             stream->clockStreamHandle, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Update the service request flags */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpUmtEntryConfigS *entryConfig = &stream->pUmtEntry->entryConfig;

      if (msgType == ZL303XX_MSG_ID_SYNC)
         entryConfig->reqSync = ZL303XX_FALSE;
      else if (msgType == ZL303XX_MSG_ID_DELAY_RESP)
         entryConfig->reqDelayResp = ZL303XX_FALSE;
      else if (msgType == ZL303XX_MSG_ID_PEER_DELAY_RESP)
         entryConfig->reqPdelayResp = ZL303XX_FALSE;
      else if (msgType == ZL303XX_MSG_ID_ANNOUNCE)
         entryConfig->reqAnnc = ZL303XX_FALSE;

      zl303xx_PtpUmtEntryServicesCheck(stream->portData, stream->pUmtEntry);

      if ((stream->portData->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1) ||
          (stream->portData->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_2))
      {
         zl303xx_PtpTelecomPtsfEval(stream->clockStreamHandle, entryConfig->reqAnnc,
               entryConfig->reqSync, entryConfig->reqDelayResp,
               entryConfig->reqPdelayResp);
      }
   }

   return status;
}

/* zl303xx_PtpContractStatusGet */
/**
   Returns the status of the most recent unicast negotiation contract (active
   or inactive).

   If (pStatus->durationSec == 0), no contract for this messageType has ever
   been granted on this stream.

   If (pStatus->remainingSec == 0), then the contract has expired or was
   cancelled.

  Notes:  pStatus->nextSequenceId will always be 0 for a Delay_Resp TX contract.
  Notes:  pStatus->nextSequenceId for an RX Delay_Resp contract will be the next
          transmitted sequenceId for a Delay_Req message.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   msgType       messageType of contract to query.
   [in]   flow          Flow direction of contract to query.
   [out]  pStatus       Contract status data.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  streamHandle, msgType, or flow is invalid.
     ZL303XX_INVALID_POINTER    pStatus is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpContractStatusGet(zl303xx_PtpStreamHandleT streamHandle,
                                       zl303xx_MessageTypeE msgType,
                                       zl303xx_PtpContractFlowE flow,
                                       zl303xx_PtpContractStatusS *pStatus)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtpContractStatusGet";
   zl303xx_ClockStreamS *stream;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MessageTypeToIndex(msgType, &uniNegMsgType);
   }

   if (status == ZL303XX_OK)
   {
      if (flow >= ZL303XX_PTP_CONTRACT_NUM_TYPES)
      {
         ZL303XX_TRACE_ERROR("%s: Invalid flow=%d", fnName, flow, 0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pStatus);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   if (status == ZL303XX_OK)
   {
      if (flow == ZL303XX_PTP_CONTRACT_RX)
      {
         zl303xx_PtpContractRxS *rx = &stream->contractRx[uniNegMsgType];

         pStatus->logMessageInterval = rx->grant.interval;
         pStatus->durationSec = rx->grant.durationSec;
         pStatus->remainingSec = rx->remainingSec;

         if (msgType == ZL303XX_MSG_ID_DELAY_RESP)
         {
            pStatus->nextSequenceId = stream->config.nextTxSequenceId[ZL303XX_MSG_ID_DELAY_REQ];
         }
         else if (stream->msgParams[msgType].lastSequenceIdRxInit)
         {
            pStatus->nextSequenceId = stream->msgParams[msgType].lastSequenceIdRx + 1;
         }
         else
         {
            pStatus->nextSequenceId = 0;
         }
      }
      else
      {
         zl303xx_PtpContractTxS *tx = &stream->contractTx[uniNegMsgType];

         pStatus->logMessageInterval = tx->grant.interval;
         pStatus->durationSec = tx->grant.durationSec;
         pStatus->remainingSec = tx->remainingSec;

         if (msgType == ZL303XX_MSG_ID_DELAY_RESP)
         {
            pStatus->nextSequenceId = 0;
         }
         else
         {
            pStatus->nextSequenceId = stream->config.nextTxSequenceId[msgType];
         }
      }

      pStatus->farEndPortAddr = stream->config.destAddr;
   }

   return status;
}

/* zl303xx_PtpTxContractStatusSet */
/** 

   Function to manually create TX unicast service without receiving a REQUEST
   TLV or modify an existing contract. This function is intended to be used to
   implement PTP redundancy. This function will create a new stream if required.

  Parameters:
   [in]   portHandle     Handle to an existing port.
   [in]   msgType        messageType to create TX service for.
   [in]   pStatus        The status of the TX contract to manually set.
   [out]  pStreamHandle  (Optional) Handle to the stream associated with
                              pStatus->farEndPortAddr.

  Notes:  Does not check any port limits (e.g., contract count or max PPS) before
       creating the unicast service.

*******************************************************************************/
zlStatusE zl303xx_PtpTxContractStatusSet(zl303xx_PtpPortHandleT portHandle,
                                         zl303xx_MessageTypeE msgType,
                                         zl303xx_PtpContractStatusS *pStatus,
                                         zl303xx_PtpStreamHandleT *pStreamHandle)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_PortDataS *port;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;
   zl303xx_ClockStreamS *stream;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pStatus);
   }
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &port);
   }
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpUniNegRequestValid(port, msgType, pStatus->durationSec, NULL);
   }
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MessageTypeToIndex(msgType, &uniNegMsgType);
   }

   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(port->clock);
      status = mStatus;
   }

   /* Find the stream if it exists. Otherwise, create a new one. */
   if (status == ZL303XX_OK)
   {
      stream = zl303xx_PtpStreamExistsOnPort(port, &pStatus->farEndPortAddr);

      if (stream == NULL)
      {
         status = zl303xx_PtpUniNegStreamCreate(port, &pStatus->farEndPortAddr,
                                                msgType, pStatus->logMessageInterval,
                                                NULL, &stream);
      }
   }

   /* Manually set the TX contract parameters. */
   if (status == ZL303XX_OK)
   {
      stream->config.nextTxSequenceId[msgType] = pStatus->nextSequenceId;
      zl303xx_PtpStreamConfigIntvlSet(stream, msgType, pStatus->logMessageInterval);

      stream->contractTx[uniNegMsgType].grant.interval = pStatus->logMessageInterval;
      stream->contractTx[uniNegMsgType].grant.durationSec = pStatus->durationSec;
      stream->contractTx[uniNegMsgType].remainingSec = pStatus->remainingSec;

      if (pStatus->remainingSec > 0)
      {
         zl303xx_PtpContractTxStateSet(stream, uniNegMsgType, ZL303XX_PTP_CONTRACT_TX_ACTIVE);
      }

      if (pStreamHandle != NULL)
      {
         *pStreamHandle = stream->clockStreamHandle;
      }
   }

   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(port->clock, status);
   }

   return status;
}

/*****************   INTERNAL PTP FUNCTION DEFINITIONS   **********************/

/* zl303xx_PtpStreamCreateStructCheck */
/** 

   Verifies that all of the members of a zl303xx_PtpStreamCreateS have been
   set correctly.

  Parameters:
   [in]   streamCreate  Structure to verify.
   [in]   pPort         Port this stream will be attached to.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamCreateStructCheck(zl303xx_PtpStreamCreateS *streamCreate,
                                             zl303xx_PortDataS *pPort)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      zl303xx_PtpNextTxSequenceIdCheck(streamCreate->nextTxSequenceId);

      /* Check boolean parameters. */
      status = ZL303XX_CHECK_BOOLEAN(streamCreate->unicast);
   }

   /* Only one multicast stream per port is allowed. */
   if (status == ZL303XX_OK)
   {
      if ((streamCreate->unicast == ZL303XX_FALSE) &&
          (pPort->mcastStream != NULL) &&
          (pPort->pdelayStream != NULL))
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Multicast clock streams already initialized for this port.",
                             0,0,0,0,0,0);
         status = ZL303XX_PROTOCOL_ENGINE_ERROR;
      }
   }

   if (status == ZL303XX_OK)
   {
      if (streamCreate->mode >= ZL303XX_PTP_STREAM_MODE_NUM_TYPES)
      {
         ZL303XX_TRACE_ALWAYS("   (ERROR) Invalid value for mode=%lu.",
                              streamCreate->mode, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }

      /* Check announceReceiptTimeout against the configured profile. */
      if (status == ZL303XX_OK)
      {
         status = zl303xx_PtpProfileAnncReceiptTimeoutCheck(
                        pPort->clock->config.profile,
                        pPort->clock->config.profileStrict,
                        &streamCreate->announceReceiptTimeout);
      }

      if ((streamCreate->unicast == ZL303XX_TRUE) &&
          (pPort->config.unicastNegotiation.enabled))
      {
         /* Pass in the same duration for all message types. */
         status |= zl303xx_PtpV2CheckUnicastSlaveData(
                      streamCreate->unicastNegotiationDuration,
                      streamCreate->unicastNegotiationDuration,
                      streamCreate->unicastNegotiationDuration,
                      streamCreate->unicastNegotiationDuration,
                      streamCreate->unicastNegotiationRenew);
      }

      /* Check profile specific contract duration if unicast negotiation is enabled. */
      if (pPort->config.unicastNegotiation.enabled == ZL303XX_TRUE)
      {
         status |= zl303xx_PtpProfileUnicastDurationCheck(
                                 pPort->clock->config.profile,
                                 pPort->clock->config.profileStrict,
                                 &streamCreate->unicastNegotiationDuration);
      }

      /* Check profile specific message intervals. */
      {
         status |= zl303xx_PtpProfileMsgIntervalCheck(
                                 pPort->clock->config.profile,
                                 pPort->clock->config.profileStrict,
                                 ZL303XX_MSG_ID_SYNC,
                                 &streamCreate->logSyncInterval)  |
                     zl303xx_PtpProfileMsgIntervalCheck(
                                 pPort->clock->config.profile,
                                 pPort->clock->config.profileStrict,
                                 ZL303XX_MSG_ID_DELAY_REQ,
                                 &streamCreate->logMinDelayReqInterval)  |
                     zl303xx_PtpProfileMsgIntervalCheck(
                                 pPort->clock->config.profile,
                                 pPort->clock->config.profileStrict,
                                 ZL303XX_MSG_ID_ANNOUNCE,
                                 &streamCreate->logAnnounceInterval);
      }
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortAddressCheck(&streamCreate->destAddr);
   }

   return status;
}

/* zl303xx_PtpStreamStructAllocate */
/** 

   Allocates memory for a PTP stream data structure.

  Parameters:
   [in]  pStreamCreate  Stream creation parameters required to allocate memory:
                              - Number of time stamp records.

   [out] ppStream    Pointer to the allocated structure.


  Notes:  Assumes all input parameters are valid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamStructAllocate(
      zl303xx_PtpStreamCreateS *pStreamCreate,
      zl303xx_ClockStreamS **ppStream)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *newStream = NULL;

   Uint16T msgId;

   if (status == ZL303XX_OK)
   {
      /* Allocate memory for the stream structure. */
      newStream = (zl303xx_ClockStreamS *)OS_CALLOC(1, sizeof(zl303xx_ClockStreamS));
      if (newStream == NULL)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Stream memory allocate failed.", 0,0,0,0,0,0);
         goto err_calloc_stream;
      }

      /* Allocate memory for the time stamp record array. */
      for(msgId = 0; msgId < ZL303XX_PTP_V2_MSG_NUM_EVENT_TYPES; msgId++)
      {
         newStream->tsRecordArray[msgId] = (zl303xx_PtpTsRecordTblEntryS *)OS_CALLOC(pStreamCreate->numTsRecords,
                                                                                    sizeof(zl303xx_PtpTsRecordTblEntryS));
         if(newStream->tsRecordArray[msgId] == NULL)
         {
            ZL303XX_TRACE_ERROR("   (ERROR) Time stamp record array memory allocate failed.", 0,0,0,0,0,0);
            goto err_calloc_tsrec;
         }
         else
         {
            newStream->tsRecordIdxMask = pStreamCreate->numTsRecords - 1;
         }
      }

      newStream->twoStepRecordArray = (zl303xx_PtpTwoStepRecordS *)OS_CALLOC(pStreamCreate->numTwoStepRecords,
                                                                             sizeof(zl303xx_PtpTwoStepRecordS));
      if (newStream->twoStepRecordArray != NULL)
      {
         newStream->twoStepRecordIdxMask = pStreamCreate->numTwoStepRecords - 1;
      }
      else
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Two-step record array memory allocate failed.", 0,0,0,0,0,0);
         goto err_calloc_twostep;
      }

      /* Return the stream pointer to the caller. */
      *ppStream = newStream;
   }

   return status;

/* Memory cleanup */
err_calloc_twostep:
   for(msgId = 0; msgId < ZL303XX_PTP_V2_MSG_NUM_EVENT_TYPES; msgId++)
   {
       OS_FREE(newStream->tsRecordArray[msgId]);
       newStream->tsRecordArray[msgId] = NULL;
   }

err_calloc_tsrec:
   OS_FREE(newStream);

err_calloc_stream:
   *ppStream = NULL;
   return ZL303XX_RTOS_MEMORY_FAIL;
}

/* zl303xx_PtpStreamStructFree */
/** 

   Frees memory used by a PTP stream data structure.

  Parameters:
   [in,out]  ppStream  Pointer to the previously allocated structure. It will
                            point to NULL after this function completes.

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamStructFree(zl303xx_ClockStreamS **ppStream)
{
   zlStatusE status = ZL303XX_OK;
   Uint16T msgId;

   if (status == ZL303XX_OK)
   {
       OS_FREE((*ppStream)->twoStepRecordArray);
       (*ppStream)->twoStepRecordArray = NULL;

       for(msgId = 0; msgId < ZL303XX_PTP_V2_MSG_NUM_EVENT_TYPES; msgId++)
       {
           OS_FREE((*ppStream)->tsRecordArray[msgId]);
           (*ppStream)->tsRecordArray[msgId] = NULL;
       }

       OS_FREE(*ppStream);
       *ppStream = NULL;
   }

   return status;
}

/* zl303xx_PtpStreamCreateInternal */
/** 

   Allocates memory for a stream structure and attaches it to the appropriate
   port. Will either be called via the user API stream create function or by
   the unicast state machine.

  Parameters:
   [in]   streamCreate  Parameters used to create the stream.
   [in]   port          Pointer to the port to attach this stream to.
   [in]   rxExtData     Implementation-specific data from the PTP message
                             that triggered this event.
   [out]  ppStream      Pointer to the created stream.

  Notes:   Assumes all input parameters are valid. Assumes the clock mutex has
        already been locked.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamCreateInternal(zl303xx_PtpStreamCreateS *streamCreate,
                                          zl303xx_PortDataS *port,
                                          Uint8T *rxExtData,
                                          zl303xx_ClockStreamS **ppStream)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream = NULL;
   Uint32T count;

   /* Set the contract renewal period now that port data is available */
   if (port->config.unicastNegotiation.enabled == ZL303XX_TRUE)
   {
      /* renewTime is the number of seconds before the contract
       * expires when reissue requests will begin transmitting */
      zl303xx_TimeStamp queryInterval, renewTime;

      zl303xx_PtpConvertLog2IntervalToTime(port->unicastMasterTable.logQueryInterval, &queryInterval);
      mulTime(&renewTime, &queryInterval, ZL303XX_PTP_DEFAULT_STREAM_UNI_NEG_RENEW);
      streamCreate->unicastNegotiationRenew = renewTime.secondsField.lo;
   }

   /* Check the validity of the streamCreate structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamCreateStructCheck(streamCreate, port);
   }

   /* Allocate memory for the stream structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamStructAllocate(streamCreate, &stream);
   }

   /* Attach the stream to the port's streamList. */
   if (status == ZL303XX_OK)
   {
      /* Save a copy of the configuration structure. */
      stream->config = *streamCreate;
      status = zl303xx_PtpPortAttachStream(port, stream);
   }

   /* Port data now configured. Configure the clock stream. */
   if (status == ZL303XX_OK)
   {
      /* Set the pointer to the port data structure */
      stream->portData = port;

      /* If the clock is slave only, the stream must be slave only too. */
      if (port->clock->config.defaultDS.slaveOnly == ZL303XX_TRUE)
      {
         if (streamCreate->mode != ZL303XX_PTP_STREAM_SLAVE_ONLY)
         {
            ZL303XX_TRACE_ALWAYS("   Forcing stream to SLAVE_ONLY mode.", 0,0,0,0,0,0);
            streamCreate->mode = ZL303XX_PTP_STREAM_SLAVE_ONLY;
         }
      }

      /* If the clock's class attribute dictates it is a MASTER-ONLY but
       * the stream is being configured for SLAVE-ONLY mode, display a message. */
      else if ((port->clock->config.defaultDS.clockQuality.clockClass <= PTP_MASTER_ONLY_CLOCK_CLASS_MAX) &&
               (streamCreate->mode == ZL303XX_PTP_STREAM_SLAVE_ONLY))
      {
         ZL303XX_TRACE_ALWAYS("Clock is MASTER-ONLY but stream is SLAVE-ONLY.", 0,0,0,0,0,0);
         ZL303XX_TRACE_ALWAYS("... Stream can only achieve LISTENING state.", 0,0,0,0,0,0);
      }

      if (stream->portData->config.delayMechanism == ZL303XX_DELAY_MECHANISM_E2E)
      {
         /* For E2E mode the peer path delay is unused and should be zero */
         OS_MEMSET(&stream->u.v2.peer_mean_path_delay, 0, sizeof(stream->u.v2.peer_mean_path_delay));
         /* Note, log_min_mean_pdelay_interval is set in zl303xx_PtpV2InitPortData() */
      }

      /* Initialize the Keep-Alive Timer value. */
      stream->keepAliveRemSec = streamCreate->keepAliveSec;

      /* Force it to initialize */
      stream->streamSubstate = PTP_STREAM_SUBSTATE_INIT;     /* Set the output value */

      /* Initialize unicast data */
      (void)zl303xx_PtpV2InitUnicastStreamData(stream);

      /* Set the Stream's farEnd ParentDS & CurrentDS to the local clock
       * values by default. */
      {
         /* CurrentDS */
         OS_MEMSET(&(stream->farEndCurrentDS), 0x00, sizeof(stream->farEndCurrentDS));

         /* ParentDS */
         zl303xx_PtpConvertDefaultDsToParentDs(
               &(stream->portData->clock->config.defaultDS),
               &(stream->farEndParentDS.u.v2));

         /* This implementation does not support parent statistics so set the
          * values as per IEEE-1588-2008: Section 8.2.3.4. */
         stream->farEndParentDS.u.v2.parentStats = ZL303XX_FALSE;
         stream->farEndParentDS.u.v2.observedParentOffsetScaledLogVariance = 0xFFFF;
         stream->farEndParentDS.u.v2.observedParentClockPhaseChangeRate = 0x7FFFFFFF;
      }

      if (stream->config.unicast == ZL303XX_TRUE)
      {
         /* Set the default values for the slave to use when negotiating a stream data.
            Use the same requested duration value for each message type  */
         (void)zl303xx_PtpV2SetUnicastSlaveData(stream,
                  streamCreate->logAnnounceInterval, streamCreate->unicastNegotiationDuration,
                  streamCreate->logSyncInterval, streamCreate->unicastNegotiationDuration,
                  streamCreate->logMinDelayReqInterval, streamCreate->unicastNegotiationDuration,
                  streamCreate->logMinPdelayReqInterval, streamCreate->unicastNegotiationDuration,
                  streamCreate->unicastNegotiationRenew);
      }

      /* This is either the PDelay multicast stream or the general multicast stream. */
      else
      {
         /* We expect any PDELAY stream to be created first (required for
          * unicast Negotiation operation with multicast PDelay).          */
         if ((port->config.delayMechanism == ZL303XX_DELAY_MECHANISM_P2P) &&
             (port->pdelayStream == NULL))
         {
            port->pdelayStream = stream;
         }

         else if (port->mcastStream == NULL)
         {
            port->mcastStream = stream;
         }
      }

      /* Initialize the stream counters. */
      zl303xx_PtpStreamCountersInit(stream);
   }

   if (status == ZL303XX_OK)
   {
      /* Some manual configuration needs to get done for unicast NON-negotiated mode. */
      if ((stream->config.unicast == ZL303XX_TRUE) &&
          (port->config.unicastNegotiation.enabled == ZL303XX_FALSE))
      {
         /* Check if the port is in the initial state */
         if (port->portSuperState != PTP_PORT_INT_STATE_ACTIVE)
         {
             zl303xx_PtpV2ToPortState(port, PTP_PORT_INT_STATE_INIT);
         }
      }
   }

   if (status == ZL303XX_OK)
   {
      /* Initialize members used for interval change detection. */
      for (count = 0; count < ZL303XX_PTP_V2_NUM_MSG_IDS; count++)
      {
         stream->msgParams[count].prevIntvl = PTP_V2_LMMI_NULL_VALUE;
      }

      /* Used to detect when master changes it's logMinDelayReqInterval */
      stream->msgParams[ZL303XX_MSG_ID_DELAY_REQ].log2intervalAct = stream->config.logMinDelayReqInterval;
      stream->msgParams[ZL303XX_MSG_ID_PEER_DELAY_REQ].log2intervalAct = stream->config.logMinPdelayReqInterval;

      /* Mask all PTSF flags and clear any remnant data by default */
      zl303xx_PtsfMaskSetAll(stream->clockStreamHandle, 0);
      zl303xx_PtsfFlagSetAll(stream->clockStreamHandle, 0);

      if (port->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1)
      {
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_RETRY_ATTEMPTS_FAIL, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_LOSS_ANNOUNCE, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_LOSS_SYNC, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_LOSS_FOLLOW_UP, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_LOSS_SYNC_OR_FOLLOW_UP, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_LOSS_DELAY_RESP, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_LOSS_PEER_DELAY_RESP, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_LOSS_PEER_DELAY_FOLLOW_UP, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_CONTRACT, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_TIMEOUT, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_UNUSABLE, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_MAIN, 1);
      }
      else if (port->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_2)
      {
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_LOSS_SYNC, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_LOSS_FOLLOW_UP, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_LOSS_SYNC_OR_FOLLOW_UP, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_CONTRACT, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_TIMEOUT, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_UNUSABLE, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_MISC, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_USER, 1);
         zl303xx_PtsfMaskSet(stream->clockStreamHandle, ZL303XX_PTSF_MAIN, 1);
      }

      /* Fire the stream created event, if requested. */
      zl303xx_PtpStreamHandleCreateEvent(stream, rxExtData);

      /* If this new stream has a faster ANNOUNCE rate than others then the
       * BMCA UPDATE Interval Timer needs to be changed. */
      zl303xx_PtpClockTimersInit(stream->portData->clock);

      /* Initialize the timers after the (possible) event callback. The user
       * may have changed the "force minimum interval" createFlags. */
      zl303xx_PtpStreamTimersInit(stream);

      /* Return a pointer to this stream. */
      *ppStream = stream;
   }

   return status;
}

/* zl303xx_PtpStreamHandleCreateEvent */
/** 

   Calls the event handler if the stream is configured to fire it. Updates the
   internal stream config with (acceptable) changes made by the user inside the
   callback.

  Parameters:
   [in]   pStream    Pointer to the created stream.
   [in]   rxExtData  Implementation-specific data from the PTP message
                          that triggered the stream to be created.

*******************************************************************************/
static void zl303xx_PtpStreamHandleCreateEvent(zl303xx_ClockStreamS *pStream,
                                               Uint8T *rxExtData)
{
   zl303xx_PtpStreamCreateS newConfig;

   /* createFlags that are allowed to be changed in the callback by the user */
   const Uint32T allowedFlags = ZL303XX_PTP_SYNC_FORCE_MIN_INTVL |
                                ZL303XX_PTP_DREQ_FORCE_MIN_INTVL |
                                ZL303XX_PTP_PDREQ_FORCE_MIN_INTVL;

   if (pStream->config.createFlags & ZL303XX_PTP_STREAM_CREATE_FIRE_EVENT)
   {
      zl303xx_PtpEventStreamCreate(pStream, rxExtData, &newConfig);

      /* Change createFlags based on above bitmap */
      pStream->config.createFlags &= ~allowedFlags;
      pStream->config.createFlags |= newConfig.createFlags & allowedFlags;

      /* Don't allow non-zero sequenceIds for invalid messageTypes */
      zl303xx_PtpNextTxSequenceIdCheck(newConfig.nextTxSequenceId);
      OS_MEMCPY(pStream->config.nextTxSequenceId, newConfig.nextTxSequenceId,
             sizeof(pStream->config.nextTxSequenceId));
   }
}

/* zl303xx_PtpStreamDeleteInternal */
/** 

   Internal function to delete a stream. Will either be called via the user API
   stream delete function or by the port delete function.

  Parameters:
   [in]      stream         Pointer to a stream data structure. The contents
                                 of this pointer are no longer valid after
                                 calling this function.
   [in,out]  pStreamDelete  (Optional) Parameters to shut down the stream.

  Notes:   Assumes all input parameters are valid. Assumes the clock mutex has
        already been taken.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamDeleteInternal(zl303xx_ClockStreamS *stream,
                                          zl303xx_PtpStreamDeleteS *pStreamDelete)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      if (pStreamDelete != NULL)
      {
         /* Return next sequenceId for all messageTypes to user */
         OS_MEMCPY(pStreamDelete->nextTxSequenceId, stream->config.nextTxSequenceId,
                sizeof(pStreamDelete->nextTxSequenceId));
      }

      /* If this is the Multicast or PDelay stream associated with the port then
       * Detach it from the Port before we destroy the stream.  */
      if (stream->portData->mcastStream == stream)
      {
         stream->portData->mcastStream = NULL;
      }
      else if (stream->portData->pdelayStream == stream)
      {
         stream->portData->pdelayStream = NULL;
      }

      /* If this is the current grandmaster Stream then Detach it from the Clock
       * before we destroy it.  */
      if (stream->portData->clock->gmSource.pPtpStream == stream)
      {
         stream->portData->clock->gmSource.pPtpStream = NULL;
      }

      /* Remove the UMT entry if it exists for this stream. */
      if (stream->pUmtEntry != NULL)
      {
         /* Invalidate the stream handle to stop the function below from
          * attempting to delete the stream. */
         stream->pUmtEntry->streamHandle = (Uint32T)ZL303XX_PTP_INVALID;
         status = zl303xx_PtpUmtEntryIntRemove(stream->portData,
                                               stream->pUmtEntry);
      }
   }

   /* Delete any FMT entries associated with this stream. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpFmtEntryDelete(&stream->portData->clock->foreignMasterDS, stream);
   }

   /* Stop all timers associated with this stream, so they get removed from the
    * list of active timers. */
   if (status == ZL303XX_OK)
   {
      Uint16T count;

      for (count = 0; count < TIMER_ARRAY_SIZE; count++)
      {
         zl303xx_PtpTimerStop(&stream->itimer[count]);
      }

      /* Detach this stream from the port's streamList. */
      status = zl303xx_PtpPortDetachStream(stream->portData, stream);
   }

   /* Free the stream structure memory. */
   if (status == ZL303XX_OK)
   {
   #if defined FREE_USER_MANAGED_MEMORY
       void *pStreamExtData = NULL;

       if ((status = zl303xx_PtpStreamExtDataGet(stream->clockStreamHandle, (void **)&pStreamExtData)) != ZL303XX_OK)
       {
           ZL303XX_TRACE_ALWAYS("zl303xx_PtpStreamDeleteInternal:  zl303xx_PtpStreamExtDataGet=%lu", status, 0,0,0,0,0);
       }
       else
       {
           if (pStreamExtData)  /* If user allocated, we remove it */
           {
               OS_FREE(pStreamExtData);
               pStreamExtData = NULL;
           }
       }
   #endif

       if (status == ZL303XX_OK && (status = zl303xx_PtpStreamStructFree(&stream)) != ZL303XX_OK)
       {
           ZL303XX_TRACE_ALWAYS("zl303xx_PtpStreamDeleteInternal:  zl303xx_PtpStreamStructFree=%lu", status, 0,0,0,0,0);
       }
   }

   return status;
}

/* zl303xx_PtpStreamDataGet */
/** 

   Returns a pointer to an internal stream data structure.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [out]  ppStream      Pointer to the stream data structure.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    ppStream is NULL.
     ZL303XX_PARAMETER_INVALID  streamHandle is out of range or references a
                                    stream that does not exist.

  Notes:
   For internal API use only. Should not be called by an application.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamDataGet(zl303xx_PtpStreamHandleT streamHandle,
                                   zl303xx_ClockStreamS **ppStream)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtpStreamDataGet";

   if (status == ZL303XX_OK)
   {
      ZL303XX_CHECK_POINTER(ppStream);
   }

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_STREAM_HANDLE(streamHandle);
      if (status != ZL303XX_OK)
      {
         ZL303XX_TRACE_ALWAYS("%s: (ERROR) streamHandle=%u out of range (0-%u)",
               fnName, streamHandle, ZL303XX_PTP_NUM_STREAMS_MAX - 1, 0,0,0);
      }
   }

   if (status == ZL303XX_OK)
   {
      /* Do not modify *ppStream unless the stream array entry is valid */
      if (Zl303xx_PtpStreamArray[streamHandle] != NULL)
      {
         *ppStream = Zl303xx_PtpStreamArray[streamHandle];
      }
      else
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   return status;
}

/* zl303xx_PtpStreamDataGetForProfile */
/** 

   Returns a pointer to an internal stream data structure ONLY IF the stream is
   operating the indicated PTP Profile.

   With the addition of PTP Profiles and unions to store profile specific data,
   a stream must be running the indicated profile in order for certain data to
   map correctly to the profile specific structure.

  Parameters:
   [in]  streamHandle   Handle to an existing stream.
   [in]  ptpProfile     Required PTP Profile.

   [out]  ppStream      Pointer to the stream data structure.

  Return Value:
    ZL303XX_OK                 Success.
    ZL303XX_INVALID_POINTER    ppStream is NULL.
    ZL303XX_PARAMETER_INVALID  streamHandle is out of range, references a
                                    stream that does not exist.
    ZL303XX_INVALID_MODE       streamHandle references a stream that is not
                                    running the required PTP profile.

  Notes:
   For internal API use only. Should not be called by an application.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamDataGetForProfile(
      zl303xx_PtpClockHandleT streamHandle,
      zl303xx_PtpProfileE ptpProfile,
      zl303xx_ClockStreamS **ppStream)
{
   zlStatusE status = ZL303XX_OK;

   /* First, see if the stream even exists. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, ppStream);
   }

   /* Check the Profile value. */
   if (status == ZL303XX_OK)
   {
      if ((*ppStream)->portData->clock->config.profile != ptpProfile)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpStreamDataGetForProfile: Stream(%u) Profile(%u)(%u expected)",
               streamHandle, (*ppStream)->portData->clock->config.profile, ptpProfile, 0,0,0);
         status = ZL303XX_INVALID_MODE;
      }
   }

   return status;
}

/* zl303xx_PtpStreamExistsOnPort */
/** 

   Checks if a stream with a specific destination address exists on a port.

  Parameters:
   [in]   port   Pointer to the port whose streamList will be checked.
   [in]   addr   Port address to search for.

*******************************************************************************/
zl303xx_ClockStreamS *zl303xx_PtpStreamExistsOnPort(zl303xx_PortDataS *port,
                                                  zl303xx_PortAddress *addr)
{
   zl303xx_ClockStreamS *stream;

   ZL303XX_PTP_FOR_EACH_STREAM(&port->streamList, stream)
   {
      if (zl303xx_PtpComparePortAddress(addr, &stream->config.destAddr) == 0)
      {
         return stream;
      }
   }

   return NULL;
}

/* zl303xx_PtpStreamExists */
/** 

   Checks if a stream with a specific destination address exists on a port
   first. If the stream does not exist, all ports attached to the clock are
   searched for a matching stream.

  Parameters:
   [in]   port   Pointer to the port whose streamList will be checked.
   [in]   addr   Port address to search for.

*******************************************************************************/
zl303xx_ClockStreamS *zl303xx_PtpStreamExists(zl303xx_PortDataS *pPort,
                                            zl303xx_PortAddress *pAddr)
{
   zl303xx_PortDataS *currPort;
   zl303xx_ClockStreamS *currStream;

   /* Search the current port's stream list. */
   currStream = zl303xx_PtpStreamExistsOnPort(pPort, pAddr);
   if (currStream != NULL)
   {
      return currStream;
   }

   /* Stream not found on the current port. Expand the search to all ports
    * attached to this clock. */
   ZL303XX_PTP_FOR_EACH_PORT(&pPort->clock->portList, currPort)
   {
          /* The port passed into this function has already been searched. */
      if ((currPort == pPort) ||
          /* Only search ports with the same local address. */
          (zl303xx_PtpComparePortAddress(&pPort->config.localAddr,
                                         &currPort->config.localAddr) != 0))
      {
         continue;
      }

      ZL303XX_PTP_FOR_EACH_STREAM(&currPort->streamList, currStream)
      {
         if (zl303xx_PtpComparePortAddress(pAddr, &currStream->config.destAddr) == 0)
         {
            return currStream;
         }
      }
   }

   /* If we have reached this point then no stream was found. */
   {
      static char portAddressStr[64];
      if (ZL303XX_OK == zl303xx_PtpConvertPortAddressToStr(pAddr, portAddressStr))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
               "Non-existent PTP stream on any Port for networkProtocol = %d (addr: %s)",
               pAddr->networkProtocol, portAddressStr, 0,0,0,0);
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 2,
               "Non-existent PTP stream on any Port for networkProtocol = %d (addr: %s)",
               pAddr->networkProtocol, portAddressStr, 0,0,0,0);
      }
      else
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
               "Non-existent PTP stream on any Port for networkProtocol = %d",
               pAddr->networkProtocol, 0,0,0,0,0);
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 2,
               "Non-existent PTP stream on any Port for networkProtocol = %d",
               pAddr->networkProtocol, 0,0,0,0,0);
      }
   }

   return NULL;
}

/* zl303xx_PtpStreamCountersInit */
/** 

   Initializes all stream message counters to 0.

  Parameters:
   [in]   pStream   Pointer to a stream to initialize.

  Notes:   Assumes the PTP clock mutex has already been taken.

*******************************************************************************/
void zl303xx_PtpStreamCountersInit(zl303xx_ClockStreamS *pStream)
{
   OS_MEMSET(&pStream->counters, 0, sizeof(pStream->counters));
   OS_MEMSET(&pStream->countersInt, 0, sizeof(pStream->countersInt));
}

/* zl303xx_PtpStreamAlarmsCheck */
/** 

   Checks alarm conditions based on stream message counters and generates
   events if the conditions are met.

  Parameters:
   [in]   pStream   Pointer to the stream to check alarms conditions on.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamAlarmsCheck(zl303xx_ClockStreamS *pStream)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      /* Only check alarm conditions for streams that aren't MASTER/IDLE. */
      if ((pStream->streamSubstate == PTP_STREAM_SUBSTATE_INIT      ) ||
          (pStream->streamSubstate == PTP_STREAM_SUBSTATE_IDLE      ) ||
          (pStream->streamSubstate == PTP_STREAM_SUBSTATE_PASSIVE   ) ||
          (pStream->streamSubstate == PTP_STREAM_SUBSTATE_PRE_MASTER) ||
          (pStream->streamSubstate == PTP_STREAM_SUBSTATE_MASTER    )    )
      {
            pStream->countersInt.currSec = 0;
            return status;
      }

      /* Return early if the alarm period has not elapsed yet. */
      if (++pStream->countersInt.currSec < ZL303XX_PTP_STREAM_ALARM_PERIOD_SEC)
      {
         return status;
      }

      /* Check SYNC rate alarm. */
      zl303xx_PtpStreamMsgAlarmCheck(pStream, ZL303XX_MSG_ID_SYNC,
                                   pStream->config.logSyncInterval);

      /* Check DELAY_RESP rate alarm. */
      if (pStream->portData->config.delayMechanism == ZL303XX_DELAY_MECHANISM_E2E)
      {
         zl303xx_PtpStreamMsgAlarmCheck(pStream, ZL303XX_MSG_ID_DELAY_RESP,
               pStream->msgParams[ZL303XX_MSG_ID_DELAY_REQ].log2intervalAct);
      }

      /* Check PEER_DELAY_RESP rate alarm. */
      if (pStream->portData->config.delayMechanism == ZL303XX_DELAY_MECHANISM_P2P)
      {
         zl303xx_PtpStreamMsgAlarmCheck(pStream, ZL303XX_MSG_ID_PEER_DELAY_RESP,
               pStream->msgParams[ZL303XX_MSG_ID_PEER_DELAY_REQ].log2intervalAct);
      }

      /* Check ANNOUNCE rate alarm. */
      zl303xx_PtpStreamMsgAlarmCheck(pStream, ZL303XX_MSG_ID_ANNOUNCE,
                                   pStream->config.logAnnounceInterval);

      /* Reset the internal counters to 0. */
      OS_MEMSET(&pStream->countersInt, 0, sizeof(pStream->countersInt));
   }

   return status;
}

/* zl303xx_PtpStreamLostMsgCheck */
/** 

   Updates the lost message counter.

  Parameters:
   [in]  stream      Pointer to an existing stream.
   [in]  pPtpMsgRx   Received message unpacked.

*******************************************************************************/
void zl303xx_PtpStreamLostMsgCheck(zl303xx_ClockStreamS *stream,
                                   zl303xx_PtpV2MsgS *pPtpMsgRx)
{
   const Sint32T ucastMsgMask = (1 << ZL303XX_MSG_ID_SYNC) |
                                (1 << ZL303XX_MSG_ID_DELAY_REQ) |
                                (1 << ZL303XX_MSG_ID_FOLLOWUP) |
                                (1 << ZL303XX_MSG_ID_DELAY_RESP) |
                                (1 << ZL303XX_MSG_ID_ANNOUNCE);

   const Sint32T ucastStateMask = (1 << PTP_STREAM_SUBSTATE_PRE_MASTER) |
                                  (1 << PTP_STREAM_SUBSTATE_UNCALIBRATED) |
                                  (1 << PTP_STREAM_SUBSTATE_MASTER) |
                                  (1 << PTP_STREAM_SUBSTATE_SLAVE);

   const Sint32T mcastMsgMask = (1 << ZL303XX_MSG_ID_SYNC) |
                                (1 << ZL303XX_MSG_ID_FOLLOWUP) |
                                (1 << ZL303XX_MSG_ID_DELAY_RESP) |
                                (1 << ZL303XX_MSG_ID_ANNOUNCE);

   const Sint32T mcastStateMask = (1 << PTP_STREAM_SUBSTATE_UNCALIBRATED) |
                                  (1 << PTP_STREAM_SUBSTATE_SLAVE);

   /* Check conditions to exit early. */

   /* Duplicated sequenceId. */
   if (pPtpMsgRx->header.sequenceId == stream->msgParams[pPtpMsgRx->header.messageType].lastSequenceIdRx)
   {
      return;
   }

   /* Check unicast messageType and stream state. */
   if (stream->config.unicast == ZL303XX_TRUE)
   {
      if (!((1 << pPtpMsgRx->header.messageType) & ucastMsgMask) ||
          !((1 << stream->streamSubstate) & ucastStateMask))
      {
         return;
      }
   }
   /* Check multicast messageType and stream state. */
   else
   {
      if (!((1 << pPtpMsgRx->header.messageType) & mcastMsgMask) ||
          !((1 << stream->streamSubstate) & mcastStateMask))
      {
         return;
      }
   }

   if (stream->msgParams[pPtpMsgRx->header.messageType].lastSequenceIdRxInit)
   {
      /* Use a local 16-bit type to avoid type promotion. */
      Uint16T thisLoss = (pPtpMsgRx->header.sequenceId -
                          stream->msgParams[pPtpMsgRx->header.messageType].lastSequenceIdRx) - 1;

      stream->counters.msg[pPtpMsgRx->header.messageType].lost += thisLoss;
   }

   stream->msgParams[pPtpMsgRx->header.messageType].lastSequenceIdRx = pPtpMsgRx->header.sequenceId;
   stream->msgParams[pPtpMsgRx->header.messageType].lastSequenceIdRxInit = ZL303XX_TRUE;
}

/* zl303xx_PtpStreamPtsfCheck */
/** 

   Checks conditions to raise or clear various PTSF flags.

  Parameters:
   [in]   pStream  Pointer to the stream to check.

*******************************************************************************/
void zl303xx_PtpStreamPtsfCheck(zl303xx_ClockStreamS *pStream)
{
   zl303xx_PtpUmtEntryIntS *pEntry;

   /* Only for G.8265.1 and G.8275.2 */
   if (((pStream->portData->clock->config.profile != ZL303XX_PTP_PROFILE_TELECOM_G8265_1) &&
        (pStream->portData->clock->config.profile != ZL303XX_PTP_PROFILE_TELECOM_G8275_2)) ||
       (pStream->pUmtEntry == NULL))
   {
      return;
   }

   pEntry = pStream->pUmtEntry;

   /* Check for the PTSF-lossSync condition. */
   if ((pStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].remainingSec > 0) &&
       (pEntry->telecom.syncReceiptTimeoutSec > 0))
   {
      /* If the SYNC message count has not changed in the last second, decrement
       * the SYNC timeout counter. */
      if (pEntry->telecom.syncPrevCount == pStream->counters.msg[ZL303XX_MSG_ID_SYNC].rx)
      {
         if (pEntry->telecom.syncTimeoutCounter > 0)
         {
            pEntry->telecom.syncTimeoutCounter--;
         }
      }
      /* SYNC message count has changed; reset the timeout counter. */
      else
      {
         pEntry->telecom.syncTimeoutCounter = pEntry->telecom.syncReceiptTimeoutSec;
      }

      /* If the SYNC timeout counter has reached 0, raise the PTSF-lossSync flag. */
      zl303xx_PtsfFlagSet(pStream->clockStreamHandle, ZL303XX_PTSF_LOSS_SYNC,
                      !pEntry->telecom.syncTimeoutCounter);
   }
   else
   {
      zl303xx_PtsfFlagSet(pStream->clockStreamHandle, ZL303XX_PTSF_LOSS_SYNC, ZL303XX_FALSE);
   }

   /* Check for the PTSF-lossFollowUp condition. */
   if ((pStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].remainingSec > 0) &&
       (pEntry->telecom.followReceiptTimeoutSec > 0))
   {
      /* If the Follow_Up message count has not changed in the last second,
       * decrement the Follow_Up timeout counter. */
      if (pEntry->telecom.followPrevCount == pStream->counters.msg[ZL303XX_MSG_ID_FOLLOWUP].rx)
      {
         if (pEntry->telecom.followTimeoutCounter > 0)
         {
            pEntry->telecom.followTimeoutCounter--;
         }
      }
      /* Follow_Up message count has changed; reset the timeout counter. */
      else
      {
         pEntry->telecom.followTimeoutCounter = pEntry->telecom.followReceiptTimeoutSec;
      }

      /* If the Follow_Up timeout counter has reached 0, raise the PTSF-lossFollowUp flag. */
      zl303xx_PtsfFlagSet(pStream->clockStreamHandle, ZL303XX_PTSF_LOSS_FOLLOW_UP,
                      !pEntry->telecom.followTimeoutCounter);
   }
   else
   {
      zl303xx_PtsfFlagSet(pStream->clockStreamHandle, ZL303XX_PTSF_LOSS_FOLLOW_UP, ZL303XX_FALSE);
   }

   /* Check for the PTSF-lossSyncOrFollowUp condition.*/
   if ((pStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].remainingSec > 0) &&
       (pEntry->telecom.sofReceiptTimeoutSec > 0))
   {
      if (pEntry->telecom.sofTimeoutCounter > 0)
      {
         pEntry->telecom.sofTimeoutCounter--;
      }

      zl303xx_PtsfFlagSet(pStream->clockStreamHandle, ZL303XX_PTSF_LOSS_SYNC_OR_FOLLOW_UP,
                      !pEntry->telecom.sofTimeoutCounter);
   }
   else
   {
      zl303xx_PtsfFlagSet(pStream->clockStreamHandle, ZL303XX_PTSF_LOSS_SYNC_OR_FOLLOW_UP, ZL303XX_FALSE);
   }

   /* Check for the PTSF-lossDelayResp condition. */
   if ((pStream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].remainingSec > 0) &&
       (pEntry->telecom.delayReceiptTimeoutSec > 0))
   {
      /* If the DELAY_RESP message count has not changed in the last
       * second, decrement the DELAY_RESP timeout counter. */
      if (pEntry->telecom.delayPrevCount == pStream->counters.msg[ZL303XX_MSG_ID_DELAY_RESP].rx)
      {
         if (pEntry->telecom.delayTimeoutCounter > 0)
         {
            pEntry->telecom.delayTimeoutCounter--;
         }
      }
      /* DELAY_RESP message count has changed; reset the timeout counter. */
      else
      {
         pEntry->telecom.delayTimeoutCounter = pEntry->telecom.delayReceiptTimeoutSec;
      }

      /* If the DELAY_RESP timeout counter has reached 0, raise the PTSF-lossDelayResp flag. */
      zl303xx_PtsfFlagSet(pStream->clockStreamHandle, ZL303XX_PTSF_LOSS_DELAY_RESP,
                      !pEntry->telecom.delayTimeoutCounter);
   }
   else
   {
      zl303xx_PtsfFlagSet(pStream->clockStreamHandle, ZL303XX_PTSF_LOSS_DELAY_RESP, ZL303XX_FALSE);
   }

   /* Check for the PTSF-lossPdelayResp condition. */
   if ((pStream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].remainingSec > 0) &&
         (pEntry->telecom.pdelayReceiptTimeoutSec > 0))
   {
      /* If the PEER_DELAY_RESP message count has not changed in the last
       * second, decrement the PEER_DELAY_RESP timeout counter. */
      if (pEntry->telecom.pdelayPrevCount == pStream->counters.msg[ZL303XX_MSG_ID_PEER_DELAY_RESP].rx)
      {
         if (pEntry->telecom.pdelayTimeoutCounter > 0)
         {
            pEntry->telecom.pdelayTimeoutCounter--;
         }
      }
      /* PEER_DELAY_RESP message count has changed; reset the timeout counter. */
      else
      {
         pEntry->telecom.pdelayTimeoutCounter = pEntry->telecom.pdelayReceiptTimeoutSec;
      }

      /* If the DELAY_RESP timeout counter has reached 0, raise the PTSF-lossDelayResp flag. */
      zl303xx_PtsfFlagSet(pStream->clockStreamHandle, ZL303XX_PTSF_LOSS_PEER_DELAY_RESP,
            !pEntry->telecom.pdelayTimeoutCounter);
   }
   else
   {
      zl303xx_PtsfFlagSet(pStream->clockStreamHandle, ZL303XX_PTSF_LOSS_PEER_DELAY_RESP, ZL303XX_FALSE);
   }

   /* Check for the PTSF-lossPdelayFollowUp condition. */
   if ((pStream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].remainingSec > 0) &&
       (pEntry->telecom.pdelayFollowReceiptTimeoutSec > 0))
   {
      /* If the Follow_Up message count has not changed in the last second,
       * decrement the Follow_Up timeout counter. */
      if (pEntry->telecom.pdelayFollowPrevCount == pStream->counters.msg[ZL303XX_MSG_ID_PEER_DELAY_FOLLOWUP].rx)
      {
         if (pEntry->telecom.pdelayFollowTimeoutCounter > 0)
         {
            pEntry->telecom.pdelayFollowTimeoutCounter--;
         }
      }
      /* Follow_Up message count has changed; reset the timeout counter. */
      else
      {
         pEntry->telecom.pdelayFollowTimeoutCounter = pEntry->telecom.pdelayFollowReceiptTimeoutSec;
      }

      /* If the Follow_Up timeout counter has reached 0, raise the PTSF-lossPdelayFollowUp flag. */
      zl303xx_PtsfFlagSet(pStream->clockStreamHandle, ZL303XX_PTSF_LOSS_PEER_DELAY_FOLLOW_UP,
                      !pEntry->telecom.pdelayFollowTimeoutCounter);
   }
   else
   {
      zl303xx_PtsfFlagSet(pStream->clockStreamHandle, ZL303XX_PTSF_LOSS_PEER_DELAY_FOLLOW_UP, ZL303XX_FALSE);
   }


   /* Update previous message count variables. */
   pEntry->telecom.syncPrevCount   = pStream->counters.msg[ZL303XX_MSG_ID_SYNC].rx;
   pEntry->telecom.followPrevCount = pStream->counters.msg[ZL303XX_MSG_ID_FOLLOWUP].rx;
   pEntry->telecom.delayPrevCount  = pStream->counters.msg[ZL303XX_MSG_ID_DELAY_RESP].rx;
   pEntry->telecom.pdelayPrevCount = pStream->counters.msg[ZL303XX_MSG_ID_PEER_DELAY_RESP].rx;
   pEntry->telecom.pdelayFollowPrevCount = pStream->counters.msg[ZL303XX_MSG_ID_PEER_DELAY_FOLLOWUP].rx;

   /* Mask the PTSF-lossTimeStamps condition based on timer. */
   if ((pStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].remainingSec > 0) ||
       (pStream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].remainingSec > 0) ||
       (pStream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].remainingSec > 0))
   {
      if (pEntry->telecom.unusableTimeoutCounter > 0)
      {
         pEntry->telecom.unusableTimeoutCounter--;
      }
   }
   else
   {
      pEntry->telecom.unusableTimeoutCounter = pEntry->telecom.unusableTimeoutSec;
   }

   zl303xx_PtsfMaskSet(pStream->clockStreamHandle, ZL303XX_PTSF_LOSS_TIME_STAMPS,
                     !pEntry->telecom.unusableTimeoutCounter);
}

/* zl303xx_PtpStreamTimerStart */
/** 

   Wrapper to simplify calling zl303xx_PtpTimerStart() for stream timers.

  Parameters:
   [in]   stream   Stream that needs a timer started.
   [in]   timerId  Type of stream timer being started.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamTimerStart(zl303xx_ClockStreamS *stream,
                                      zl303xx_TimerIdE timerId)
{
   zl303xx_TimeStamp interval;
   zl303xx_BooleanE expireAtStart = ZL303XX_FALSE;

   switch(timerId)
   {
      /* v1 timers with duplicate case values commented out */
      /*case SYNC_RECEIPT_TIMER :*/
      case ANNOUNCE_RECEIPT_TIMER:
         zl303xx_PtpGetAnnounceReceiptTimeout(stream, &interval);
         break;

      case SYNC_INTERVAL_TIMER :
         zl303xx_PtpGetSyncIntervalTimeout(stream, &interval);
         expireAtStart = ZL303XX_TRUE;
         break;

      case ANNOUNCE_INTERVAL_TIMER :
         zl303xx_PtpGetAnnounceIntervalTimeout(stream, &interval);
         expireAtStart = ZL303XX_TRUE;
         break;

      case QUALIFICATION_TIMER :
         zl303xx_PtpGetQualificationTimeout(stream, &interval);
         break;

      case PRE_MASTER_TIMER :
         zl303xx_PtpGetPreMasterTimeout(stream, &interval);
         break;

      case UNCALIBRATED_TIMER :
         zl303xx_PtpGetUncalibratedTimeout(stream, &interval);
         break;

      /*case Q_TIMER :*/
      case DELAY_REQ_TIMER :
         zl303xx_PtpGetDelayReqIntervalTimeout(stream, &interval);
         expireAtStart = ZL303XX_TRUE;
         break;

      case PEER_DELAY_REQ_TIMER :
         zl303xx_PtpGetPeerDelayReqIntervalTimeout(stream, &interval);
         expireAtStart = ZL303XX_TRUE;
         break;

      /*case SYNC_BURST_DELAY_TIMER :*/
      case UNICAST_QUERY_INTERVAL :
         zl303xx_PtpConvertLog2IntervalToTime(
               stream->portData->unicastMasterTable.logQueryInterval,
               &interval);
         expireAtStart = ZL303XX_TRUE;
         break;

      case ONE_SEC_TIMER :
         interval.secondsField.hi = 0;
         interval.secondsField.lo = 1;
         interval.nanosecondsField = 0;
         expireAtStart = ZL303XX_TRUE;
         break;

      case TIMER_ARRAY_SIZE :
      default :
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                     "Stream %lu: Invalid timerId=%d passed to zl303xx_PtpStreamTimerStart().",
                     stream->clockStreamHandle, timerId, 0,0,0,0);
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TIMER, 1,
                     "Stream %lu: Invalid timerId=%d passed to zl303xx_PtpStreamTimerStart().",
                     stream->clockStreamHandle, timerId, 0,0,0,0);
         return ZL303XX_PARAMETER_INVALID;
   }

   return zl303xx_PtpTimerStart(stream->portData->clock->clockHandle,
                                &stream->itimer[timerId],
                                &interval,
                                expireAtStart);
}

/* zl303xx_PtpStreamIntvlDetect */
/** 

   Detects an interval change on a stream for a specific messageType. Performs
   logging and event generation.

  Parameters:
   [in]   pStream  Pointer to an existing stream.
   [in]   type     messageType to check.
   [in]   intvl    Current message interval.

*******************************************************************************/
void zl303xx_PtpStreamIntvlDetect(zl303xx_ClockStreamS *pStream,
                                  zl303xx_MessageTypeE type,
                                  Sint8T intvl)
{
   const char *typeStr;

   if ((pStream->msgParams[type].prevIntvl != intvl) &&
       (pStream->msgParams[type].prevIntvl != PTP_V2_LMMI_NULL_VALUE))
   {
      typeStr = zl303xx_PtpMessageTypeToStr(type);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "Stream %u: %s %s interval change detected (%d to %d).",
         pStream->clockStreamHandle,
         pStream->config.unicast ? "Unicast" : "Multicast",
         typeStr,
         pStream->msgParams[type].prevIntvl, intvl, 0);

      zl303xx_PtpEventMsgIntvlChange(
         pStream, type, pStream->msgParams[type].prevIntvl, intvl);
   }

   pStream->msgParams[type].prevIntvl = intvl;

   /* Any new ANNOUNCE rate may affect the Clock's BMCA update timer. */
   /* Always re-evaluate the timers even if the rate hasn't changed. */
   if (type == ZL303XX_MSG_ID_ANNOUNCE)
      zl303xx_PtpClockTimersInit(pStream->portData->clock);
}

/* zl303xx_PtpStreamPtsfReset */
/** 

   Sets all PTP-specific PTSF flags to FALSE.

  Parameters:
   [in]   pStream  Pointer to an existing stream.

*******************************************************************************/
void zl303xx_PtpStreamPtsfReset(zl303xx_ClockStreamS *pStream)
{
   const zl303xx_PtsfFlagE flags[] = {ZL303XX_PTSF_QL_TOO_LOW,
                                      ZL303XX_PTSF_WAIT_TO_RESTORE_ACTIVE,
                                      ZL303XX_PTSF_ANNOUNCE_REJECTED,
                                      ZL303XX_PTSF_SYNC_REJECTED,
                                      ZL303XX_PTSF_DELAY_RESP_REJECTED,
                                      ZL303XX_PTSF_PEER_DELAY_RESP_REJECTED,
                                      ZL303XX_PTSF_ANNOUNCE_RATE_TOO_LOW,
                                      ZL303XX_PTSF_SYNC_RATE_TOO_LOW,
                                      ZL303XX_PTSF_DELAY_RESP_RATE_TOO_LOW,
                                      ZL303XX_PTSF_PEER_DELAY_RESP_RATE_TOO_LOW,
                                      ZL303XX_PTSF_LOSS_ANNOUNCE,
                                      ZL303XX_PTSF_LOSS_SYNC,
                                      ZL303XX_PTSF_LOSS_FOLLOW_UP,
                                      ZL303XX_PTSF_LOSS_SYNC_OR_FOLLOW_UP,
                                      ZL303XX_PTSF_LOSS_DELAY_RESP,
                                      ZL303XX_PTSF_LOSS_PEER_DELAY_RESP,
                                      ZL303XX_PTSF_LOSS_PEER_DELAY_FOLLOW_UP};
   Uint32T count;

   for (count = 0; count < ZL303XX_ARRAY_SIZE(flags); count++)
   {
      zl303xx_PtsfFlagSet(pStream->clockStreamHandle, flags[count], ZL303XX_FALSE);
   }
}

/* zl303xx_PtpStreamLogTs */
/** 

   Package the time stamp data and make user callout if configured

  Parameters:
   [in]   streamData  Ptr to the stream's internal data
   [in]   servoData   Servo event data to map to a zl303xx_TsLogDataS structure.

*******************************************************************************/
void zl303xx_PtpStreamTsLogSend(zl303xx_ClockStreamS *streamData,
                                zl303xx_PtpEventServoDataS *servoData)
{
   zl303xx_TsLogDataS tsData;

   if (streamData->tsLogging.callback != NULL)
   {
      OS_MEMSET(&tsData, 0, sizeof(tsData));

      tsData.streamNum   = streamData->clockStreamHandle;
      tsData.sequenceNum = servoData->sequenceId;
      tsData.fwd         = servoData->tsPairType == ZL303XX_PTP_TS_PAIR_SYNC;
      tsData.txTs        = servoData->txTs.secondsField;
      tsData.txSubSec    = servoData->txTs.nanosecondsField;
      tsData.rxTs        = servoData->rxTs.secondsField;
      tsData.rxSubSec    = servoData->rxTs.nanosecondsField;
      tsData.corr        = servoData->correctionField;

      streamData->tsLogging.callback(&tsData);
   }
}

/* zl303xx_PtpStreamTsLogOneSec */
/** 

   Function called by the stream's one second timer to update the time remaining
   on the time stamp logging mechanism.

*******************************************************************************/
static void zl303xx_PtpStreamTsLogOneSec(zl303xx_ClockStreamS *pStream)
{
   if (pStream->tsLogging.remainingSec > 0)
   {
      if (--pStream->tsLogging.remainingSec == 0)
      {
         ZL303XX_TRACE_ALWAYS("Stream %u: PTP time stamp logging finished",
               pStream->clockStreamHandle, 0,0,0,0,0);
         pStream->tsLogging.callback = NULL;
      }
   }
}

/* zl303xx_PtpNextTxSequenceIdCheck */
/** 

   Sets any sequenceId for a non-valid messageType to 0.

*******************************************************************************/
void zl303xx_PtpNextTxSequenceIdCheck(Uint16T seqId[ZL303XX_PTP_V2_NUM_MSG_IDS])
{
   Uint32T count;

   for (count = 0; count < ZL303XX_PTP_V2_NUM_MSG_IDS; count++)
   {
      if (ZL303XX_CHECK_MESSAGE_TYPE(count) != ZL303XX_OK)
      {
         seqId[count] = 0;
      }
   }
}

/* zl303xx_PtpStreamConfigIntvlSet */
/** 

   Updates a stream's config structure based on messageType and log2 interval.

  Parameters:
   [in]   stream   Modified stream structure.
   [in]   msgType  messageType being updated.
   [in]   intvl    log2 message interval to set.

*******************************************************************************/
void zl303xx_PtpStreamConfigIntvlSet(zl303xx_ClockStreamS *stream,
                                     zl303xx_MessageTypeE msgType, Sint8T intvl)
{
   if (msgType == ZL303XX_MSG_ID_SYNC)
   {
      stream->config.logSyncInterval = intvl;
   }
   else if (msgType == ZL303XX_MSG_ID_DELAY_RESP)
   {
      stream->config.logMinDelayReqInterval = intvl;
      stream->msgParams[ZL303XX_MSG_ID_DELAY_REQ].log2intervalAct = intvl;
   }
   else if (msgType == ZL303XX_MSG_ID_PEER_DELAY_RESP)
   {
      stream->config.logMinPdelayReqInterval = intvl;
      stream->msgParams[ZL303XX_MSG_ID_PEER_DELAY_REQ].log2intervalAct = intvl;
   }
   else if (msgType == ZL303XX_MSG_ID_ANNOUNCE)
   {
      stream->config.logAnnounceInterval = intvl;
   }
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/* zl303xx_PtpStreamTimersInit */
/** 

   Initializes the callback functions for stream timers.

  Parameters:
   [in,out]  stream  Stream that will have its timers initialized.

*******************************************************************************/
static void zl303xx_PtpStreamTimersInit(zl303xx_ClockStreamS *stream)
{
   /* ANNOUNCE_RECEIPT_TIMER */
   OS_SPRINTF(stream->itimer[ANNOUNCE_RECEIPT_TIMER].descr,
         "%d:%s", stream->clockStreamHandle, "ANNC_RX");
   stream->itimer[ANNOUNCE_RECEIPT_TIMER].callback  = zl303xx_PtpTimerAnncRx;

   /* SYNC_INTERVAL_TIMER */
   OS_SPRINTF(stream->itimer[SYNC_INTERVAL_TIMER].descr,
         "%d:%s", stream->clockStreamHandle, "SYNC_PPS");
   stream->itimer[SYNC_INTERVAL_TIMER].callback     = zl303xx_PtpTimerSyncTx;

   /* ANNOUNCE_INTERVAL_TIMER */
   OS_SPRINTF(stream->itimer[ANNOUNCE_INTERVAL_TIMER].descr,
         "%d:%s", stream->clockStreamHandle, "ANNC_PPS");
   stream->itimer[ANNOUNCE_INTERVAL_TIMER].callback = zl303xx_PtpTimerAnncTx;

   /* QUALIFICATION_TIMER */
   OS_SPRINTF(stream->itimer[QUALIFICATION_TIMER].descr,
         "%d:%s", stream->clockStreamHandle, "FMT_QUAL");
   stream->itimer[QUALIFICATION_TIMER].callback     = zl303xx_PtpTimerQualification;

   /* PRE_MASTER_TIMER */
   OS_SPRINTF(stream->itimer[PRE_MASTER_TIMER].descr,
         "%d:%s", stream->clockStreamHandle, "PRE_MASTER");
   stream->itimer[PRE_MASTER_TIMER].callback        = zl303xx_PtpTimerPreMaster;

   /* UNCALIBRATED_TIMER */
   OS_SPRINTF(stream->itimer[UNCALIBRATED_TIMER].descr,
         "%d:%s", stream->clockStreamHandle, "UNCALIB");
   stream->itimer[UNCALIBRATED_TIMER].callback      = zl303xx_PtpTimerUncalibrated;

   /* DELAY_REQ_TIMER */
   OS_SPRINTF(stream->itimer[DELAY_REQ_TIMER].descr,
         "%d:%s", stream->clockStreamHandle, "DELAY_PPS");
   stream->itimer[DELAY_REQ_TIMER].callback         = zl303xx_PtpTimerDelayReqTx;

   /* PEER_DELAY_REQ_TIMER */
   OS_SPRINTF(stream->itimer[PEER_DELAY_REQ_TIMER].descr,
         "%d:%s", stream->clockStreamHandle, "PDELAY_PPS");
   stream->itimer[PEER_DELAY_REQ_TIMER].callback    = zl303xx_PtpTimerPeerDelayReqTx;

   /* UNICAST_QUERY_INTERVAL */
   OS_SPRINTF(stream->itimer[UNICAST_QUERY_INTERVAL].descr,
         "%d:%s", stream->clockStreamHandle, "UNI_QUERY");
   stream->itimer[UNICAST_QUERY_INTERVAL].callback  = zl303xx_PtpTimerUcastQueryIntvl;

   /* ONE_SEC_TIMER */
   OS_SPRINTF(stream->itimer[ONE_SEC_TIMER].descr,
         "%d:%s", stream->clockStreamHandle, "ONE_SEC");
   stream->itimer[ONE_SEC_TIMER].callback           = zl303xx_PtpTimerOneSec;

   /* Check FORCE flags */
   if (stream->config.createFlags & ZL303XX_PTP_SYNC_FORCE_MIN_INTVL)
      stream->itimer[SYNC_INTERVAL_TIMER].forceMinIntvl = ZL303XX_TRUE;

   if (stream->config.createFlags & ZL303XX_PTP_DREQ_FORCE_MIN_INTVL)
      stream->itimer[DELAY_REQ_TIMER].forceMinIntvl = ZL303XX_TRUE;

   if (stream->config.createFlags & ZL303XX_PTP_PDREQ_FORCE_MIN_INTVL)
      stream->itimer[PEER_DELAY_REQ_TIMER].forceMinIntvl = ZL303XX_TRUE;

}

/* zl303xx_PtpStreamMsgAlarmCheck */
/** 

   Checks the message rate of a single messageType and generates an alarm if it
   is below 50%.

  Parameters:
   [in]   pStream    Pointer to the stream the counters are checked on.
   [in]   msgType    Message type counters to check.
   [in]   log2Intvl  Expected log2 interval of msgType.

*******************************************************************************/
static void zl303xx_PtpStreamMsgAlarmCheck(zl303xx_ClockStreamS *pStream,
                                           zl303xx_MessageTypeE msgType,
                                           Sint8T log2Intvl)
{
   /* Check expected rate for this message type. */
   Uint32T rxCountExpected =
      zl303xx_PtpMsgCountExpected(log2Intvl, ZL303XX_PTP_STREAM_ALARM_PERIOD_SEC);

   if (pStream->countersInt.msg[msgType].rx < (rxCountExpected / 2))
   {
      /* Generate the alarm event. */
      zl303xx_PtpEventCounterAlarm(pStream, msgType, rxCountExpected);
   }
}

/* zl303xx_PtpTimerAnncRx */
/** 

   Callback for zl303xx_ClockStreamS::itimer[ANNOUNCE_RECEIPT_TIMER]::callback().

   Disqualifies a stream's FMT entry (for port and clock) and runs the BMCA
   state decision.
   Default Profile, unicast negotiation: cancels all contracts.
   Telecom Profile: Raises PTSF-lossAnnounce.

  Parameters:
   [in]   pTimer  Stream ANNOUNCE_RECEIPT_TIMER that expired.

*******************************************************************************/
static void zl303xx_PtpTimerAnncRx(zl303xx_PtpTimerS *pTimer)
{
   zl303xx_ClockStreamS *stream = ZL303XX_PTP_TIMER_TO_STREAM(pTimer, ANNOUNCE_RECEIPT_TIMER);
   zl303xx_PtpClockS *pClock = stream->portData->clock;

   /* An Announce interval has timed out so disqualify any records associated
    * with the stream and re-evaluate the table. */
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TIMER, 2,
         "Stream %s: State %d: Expired: interval: " PTPD_FORMAT_TIME_STR " using pTimer=%p",
         pTimer->descr, stream->streamSubstate,
         (Uint32T) formatTimeStringSign(&pTimer->interval),
         formatTimeStringSec(&pTimer->interval),
         formatTimeStringNs(&pTimer->interval), pTimer);

   /* Disqualify only the FMT entry of the selected ParentDS. */
   (void)zl303xx_PtpFmtEntryDisqualifyForeignPort(&pClock->foreignMasterDS, stream,
         &stream->portData->clock->parentDS.parentPortIdentity);

   /* If using non-revertive BMCA, this will determine the selected master.
    * Note that Telecom Profile uses a different non-reversion mechanism. */
   if (pClock->config.bmca.revertiveEn == ZL303XX_FALSE &&
         pClock->config.profile != ZL303XX_PTP_PROFILE_TELECOM_G8265_1)
   {
      pClock->bmca.selectedFmtEntry = zl303xx_PtpFmtSelectedGet(
                                         &pClock->foreignMasterDS,
                                         &pClock->bmca.selectedAddr);
   }

   /* Run the BMCA to determine if the 'best' has changed if a disqualification occurred. */
   zl303xx_PtpEventClockBmcaUpdate(pClock);

   /* Cancel all the message contracts for this stream. The state machine should
    * attempt to re-start the Announce contract (if still applicable). */
   if (stream->config.unicast && stream->portData->config.unicastNegotiation.enabled)
   {
      zl303xx_PtpV2StopUnicastStream(stream);
   }

   /* Telecom Profile: raise the PTSF-lossAnnounce flag. */
   if ((pClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1) ||
       (pClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_2))
   {
      zl303xx_PtsfFlagSet(stream->clockStreamHandle, ZL303XX_PTSF_LOSS_ANNOUNCE, ZL303XX_TRUE);
   }

   /* Since the clockClass is no longer known, clear the PTSF-QLTooLow flag. */
   zl303xx_PtsfFlagSet(stream->clockStreamHandle, ZL303XX_PTSF_QL_TOO_LOW, ZL303XX_FALSE);
}

/* zl303xx_PtpTimerSyncTx */
/** 

   Callback for zl303xx_ClockStreamS::itimer[SYNC_INTERVAL_TIMER]::callback().
   Sends 1 or more Sync messages.

  Parameters:
   [in]   pTimer  Stream SYNC_INTERVAL_TIMER that expired.

*******************************************************************************/
static void zl303xx_PtpTimerSyncTx(zl303xx_PtpTimerS *pTimer)
{
   zl303xx_ClockStreamS *stream = ZL303XX_PTP_TIMER_TO_STREAM(pTimer, SYNC_INTERVAL_TIMER);

   zl303xx_PtpTimerMsgTx(stream, ZL303XX_MSG_ID_SYNC, pTimer->expiredCount,
                         zl303xx_PtpV2IssueSync);
}

/* zl303xx_PtpTimerAnncTx */
/** 

   Callback for zl303xx_ClockStreamS::itimer[ANNOUNCE_INTERVAL_TIMER]::callback().
   Sends 1 or more Announce messages.

  Parameters:
   [in]   pTimer  Stream ANNOUNCE_INTERVAL_TIMER that expired.

*******************************************************************************/
static void zl303xx_PtpTimerAnncTx(zl303xx_PtpTimerS *pTimer)
{
   zl303xx_ClockStreamS *stream = ZL303XX_PTP_TIMER_TO_STREAM(pTimer, ANNOUNCE_INTERVAL_TIMER);

   zl303xx_PtpTimerMsgTx(stream, ZL303XX_MSG_ID_ANNOUNCE, pTimer->expiredCount,
                         zl303xx_PtpV2IssueAnnounce);
}

/* zl303xx_PtpTimerQualification */
/** 

   Callback for zl303xx_ClockStreamS::itimer[QUALIFICATION_TIMER]::callback().

   Updates qualification counters for this stream (in port and clock FMTs).
   Updates the selected master if non-revertive is enabled and the current
   master gets disqualified.
   Telecom Profile: raises the SQUELCH event if there is no best master.

  Parameters:
   [in]   pTimer  Stream QUALIFICATION_TIMER that expired.

*******************************************************************************/
static void zl303xx_PtpTimerQualification(zl303xx_PtpTimerS *pTimer)
{
   zl303xx_ClockStreamS *stream = ZL303XX_PTP_TIMER_TO_STREAM(pTimer, QUALIFICATION_TIMER);
   zl303xx_PtpClockS *pClock = stream->portData->clock;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TIMER, 2,
         "Stream %s: Expired: interval: " PTPD_FORMAT_TIME_STR " using pTimer=%p",
         pTimer->descr,
         (Uint32T) formatTimeStringSign(&pTimer->interval),
         formatTimeStringSec(&pTimer->interval),
         formatTimeStringNs(&pTimer->interval), pTimer, 0);

   /* Logging for possible race condition:
    * - The ANNC-RX-TIMER should never be INACTIVE and expired.   */
   if ((stream->itimer[ANNOUNCE_RECEIPT_TIMER].expiredCount != 0) &&
       (zl303xx_PtpTimerActive(&stream->itimer[ANNOUNCE_RECEIPT_TIMER]) == ZL303XX_FALSE))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TIMER, 1,
            "Stream %s: ANNC RX Timer: INACTIVE with expired count %d",
            stream->itimer[ANNOUNCE_RECEIPT_TIMER].expiredCount, 0,0,0,0,0);
   }

   /* Don't do any FMT operations if the ANNOUNCE_RECEIPT_TIMER has expired in
    * this call to zl303xx_PtpTimerExecAllExpired(). Otherwise, this entry will
    * get qualified and immediately disqualified, causing toggling behavior. */
   if ((zl303xx_PtpTimerActive(&stream->itimer[ANNOUNCE_RECEIPT_TIMER]) == ZL303XX_FALSE) ||
       (stream->itimer[ANNOUNCE_RECEIPT_TIMER].expiredCount == 0))
   {
      /* End of a PTP_FOREIGN_MASTER_TIME_WINDOW period so update the
       * qualification counters and record states for any records associated
       * with this stream.         */
      (void)zl303xx_PtpFmtEntryQualify(&pClock->foreignMasterDS, stream);

      /* If using non-revertive BMCA, this will determine the selected master.
       * Note that Telecom Profile uses a different non-reversion mechanism. */
      if (pClock->config.bmca.revertiveEn == ZL303XX_FALSE &&
            pClock->config.profile != ZL303XX_PTP_PROFILE_TELECOM_G8265_1)
      {
         pClock->bmca.selectedFmtEntry = zl303xx_PtpFmtSelectedGet(
                                            &pClock->foreignMasterDS,
                                            &pClock->bmca.selectedAddr);
      }

      /* If the FMT has no records (and there is no Virtual Port) yet the
       * QUALIFICATION_TIMER for this stream has expired, then we must have an
       * M2 condition at the least (maybe even M1). Force this stream to
       * MASTER state.  */
      if ((pClock->foreignMasterDS.numActiveRecords == 0) &&
          (pClock->gmSource.location == ZL303XX_PTP_GM_SRC_LOCAL_CLOCK))
      {
         if (pClock->config.defaultDS.clockQuality.clockClass <= 127)
         {
            zl303xx_PtpStreamStateToM1(stream, NULL);
         }
         else if (pClock->config.defaultDS.clockQuality.clockClass == 255)
         {
            zl303xx_PtpStreamStateToS0(stream, NULL);
         }
         else
         {
            zl303xx_PtpStreamStateToM2(stream);
         }
      }

      /* Report the clock's BMC Table to the management system if updated. */
      (void)zl303xx_PtpEventClockBmcaUpdate(pClock);
   }

   /* Telecom Profile: Generate the SQUELCH event, if necessary. */
   if (pClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1)
   {
      zl303xx_PtpEventSquelch(pClock, ((pClock->foreignMasterDS.bestRecord == 0) ||
                                     (zl303xx_PtpTimerActive(&pClock->profileDS.telecom.qlHoldOffTimer))));
   }
}

/* zl303xx_PtpTimerPreMaster */
/** 

   Callback for zl303xx_ClockStreamS::itimer[PRE_MASTER_TIMER]::callback().

   Indicates that the Timer has expired and moves the stream into MASTER Mode.

  Parameters:
   [in]   pTimer  Stream PRE_MASTER_TIMER that expired.

*******************************************************************************/
static void zl303xx_PtpTimerPreMaster(zl303xx_PtpTimerS *pTimer)
{
   zl303xx_ClockStreamS *stream = ZL303XX_PTP_TIMER_TO_STREAM(pTimer, PRE_MASTER_TIMER);

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TIMER, 2,
         "Stream %s: Expired: State %d: interval: " PTPD_FORMAT_TIME_STR " using pTimer=%p",
         pTimer->descr, stream->streamSubstate,
         (Uint32T) formatTimeStringSign(&pTimer->interval),
         formatTimeStringSec(&pTimer->interval),
         formatTimeStringNs(&pTimer->interval), pTimer);

   /* If Stream is no longer in the PRE_MASTER State then something else has
    * caused a state change while waiting for the PRE_MASTER hold-off to expire.
    * In this case don't mess with the stream state, just log the scenario.   */
   if (stream->streamSubstate != PTP_STREAM_SUBSTATE_PRE_MASTER)
   {
      ZL303XX_TRACE_ALWAYS(
            "zl303xx_PtpTimerPreMaster: State changed (to %u) while waiting for PRE_MASTER expiry.",
            stream->streamSubstate, 0,0,0,0,0);
   }
   else
   {
      zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_MASTER, stream);
   }

   /* This is a 1-shot timer so stop it now. */
   zl303xx_PtpTimerStop(&stream->itimer[PRE_MASTER_TIMER]);
}

/* zl303xx_PtpTimerUncalibrated */
/** 

   Callback for zl303xx_ClockStreamS::itimer[UNCALIBRATED_TIMER]::callback().

   Indicates that the Timer has expired and sets the 'expiredOnce' flag to
   TRUE until any custom conditions for the UNCALIBRATED transition are cleared.

  Parameters:
   [in]   pTimer  Stream UNCALIBRATED_TIMER that expired.

*******************************************************************************/
static void zl303xx_PtpTimerUncalibrated(zl303xx_PtpTimerS *pTimer)
{
   zl303xx_ClockStreamS *stream = ZL303XX_PTP_TIMER_TO_STREAM(pTimer, UNCALIBRATED_TIMER);

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TIMER, 2,
         "Stream %s: Expired: State %d: interval: " PTPD_FORMAT_TIME_STR " using pTimer=%p",
         pTimer->descr, stream->streamSubstate,
         (Uint32T) formatTimeStringSign(&pTimer->interval),
         formatTimeStringSec(&pTimer->interval),
         formatTimeStringNs(&pTimer->interval), pTimer);

   /* If Stream is no longer in the PRE_MASTER State then something else has
    * caused a state change while waiting for the PRE_MASTER hold-off to expire.
    * In this case don't mess with the stream state, just log the scenario.   */
   if (stream->streamSubstate != PTP_STREAM_SUBSTATE_UNCALIBRATED)
   {
      ZL303XX_TRACE_ALWAYS(
            "zl303xx_PtpTimerUncalibrated: State changed (to %u) while waiting for UNCALIBRATED expiry.",
            stream->streamSubstate, 0,0,0,0,0);
   }
   else
   {
      /* Mark the Timer as having expired. The next time the Stream-DO routine
       * runs it will continue to the custom checks. */
      stream->uncalTimerExpired = ZL303XX_TRUE;
   }

   /* This is a 1-shot timer so stop it now. */
   zl303xx_PtpTimerStop(&stream->itimer[UNCALIBRATED_TIMER]);
}

/* zl303xx_PtpTimerDelayReqTx */
/** 

   Callback for zl303xx_ClockStreamS::itimer[DELAY_REQ_TIMER]::callback().
   Sends 1 or more Delay_Req messages.

  Parameters:
   [in]   pTimer  Stream DELAY_REQ_TIMER that expired.

*******************************************************************************/
static void zl303xx_PtpTimerDelayReqTx(zl303xx_PtpTimerS *pTimer)
{
   zl303xx_ClockStreamS *stream = ZL303XX_PTP_TIMER_TO_STREAM(pTimer, DELAY_REQ_TIMER);

   /* Detect changes in the rate of transmitted Delay_Req messages. This will
    * detect multicast and unicast negotiation changes. */
   zl303xx_PtpStreamIntvlDetect(stream, ZL303XX_MSG_ID_DELAY_REQ,
                                stream->msgParams[ZL303XX_MSG_ID_DELAY_REQ].log2intervalAct);

   zl303xx_PtpTimerMsgTx(stream, ZL303XX_MSG_ID_DELAY_REQ, pTimer->expiredCount,
                         zl303xx_PtpV2IssueDelayReq);
}

/* zl303xx_PtpTimerPeerDelayReqTx */
/** 

   Callback for zl303xx_ClockStreamS::itimer[PEER_DELAY_REQ_TIMER]::callback().
   Sends 1 or more PDelay_Req messages.

  Parameters:
   [in]   pTimer  Stream PEER_DELAY_REQ_TIMER that expired.

*******************************************************************************/
static void zl303xx_PtpTimerPeerDelayReqTx(zl303xx_PtpTimerS *pTimer)
{
   zl303xx_ClockStreamS *stream = ZL303XX_PTP_TIMER_TO_STREAM(pTimer, PEER_DELAY_REQ_TIMER);

   /* Detect changes in the rate of transmitted PDelay_Req messages. This will
    * detect multicast and unicast negotiation changes. */
   zl303xx_PtpStreamIntvlDetect(stream, ZL303XX_MSG_ID_PEER_DELAY_REQ,
                                stream->msgParams[ZL303XX_MSG_ID_PEER_DELAY_REQ].log2intervalAct);

   zl303xx_PtpTimerMsgTx(stream, ZL303XX_MSG_ID_PEER_DELAY_REQ, pTimer->expiredCount,
                         zl303xx_PtpV2IssuePeerDelayReq);
}

/* zl303xx_PtpTimerUcastQueryIntvl */
/** 

   Callback for zl303xx_ClockStreamS::itimer[UNICAST_QUERY_INTERVAL]::callback().
   Unicast Negotiation: Sends contract requests for all messages in the REQUESTING
      state.

  Parameters:
   [in]   pTimer  Stream UNICAST_QUERY_INTERVAL timer that expired.

*******************************************************************************/
static void zl303xx_PtpTimerUcastQueryIntvl(zl303xx_PtpTimerS *pTimer)
{
   zl303xx_ClockStreamS *stream = ZL303XX_PTP_TIMER_TO_STREAM(pTimer, UNICAST_QUERY_INTERVAL);
   zl303xx_PtpV2UnicastQueryInterval(stream);
}

/* zl303xx_PtpTimerOneSec */
/** 

   Callback for zl303xx_ClockStreamS::itimer[ONE_SEC_TIMER]::callback().

   Unicast Negotiation: Runs once-per-second functions.
   Checks stream alarms.
   Telecom Profile: Checks PTSF-lossSync and -lossDelayResp flags.

  Parameters:
   [in]   pTimer  Stream ONE_SEC_TIMER that expired.

*******************************************************************************/
static void zl303xx_PtpTimerOneSec(zl303xx_PtpTimerS *pTimer)
{
   zl303xx_ClockStreamS *stream = ZL303XX_PTP_TIMER_TO_STREAM(pTimer, ONE_SEC_TIMER);

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TIMER, 3,
         "Stream %s: Expired: State %d: interval: " PTPD_FORMAT_TIME_STR " using pTimer=%p",
         pTimer->descr, stream->streamSubstate,
         (Uint32T) formatTimeStringSign(&pTimer->interval),
         formatTimeStringSec(&pTimer->interval),
         formatTimeStringNs(&pTimer->interval), pTimer);

   /* Print PTP version number */
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,"PTP API release version: %s",
         zl303xx_PtpReleaseVersion, 0, 0, 0, 0, 0);

   /* Do once per second updates */
   if (stream->config.unicast &&
       stream->portData->config.unicastNegotiation.enabled)
   {
      (void)zl303xx_PtpV2UnicastOncePerSecond(stream);
   }

   /* Check alarm conditions and generate events for them. */
   (void)zl303xx_PtpStreamAlarmsCheck(stream);

   /* Raise/clear any PTSF flags. */
   zl303xx_PtpStreamPtsfCheck(stream);

   /* Calculate the drift (phase change) in ns/s. The result is stored in
    * the observedPhaseChangeRateInternal variable. */
   zl303xx_PtpStreamCalculateDrift(stream);

   /* Output some once-per-second status information if required */
   if ((stream->portData->clock->displayOffsetFromMaster == ZL303XX_TRUE) &&
       ((stream->streamSubstate == PTP_STREAM_SUBSTATE_SLAVE) ||
        (stream->streamSubstate == PTP_STREAM_SUBSTATE_UNCALIBRATED)))
   {
      /* Output the offset and drift */
      ZL303XX_TRACE_V2_TIMESTAMP(ZL303XX_MOD_ID_PTP_ENGINE, 2,
            stream->clockStreamHandle, "offset from master: ",
            &stream->farEndCurrentDS.offset_from_master);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,  "         drift: %5ld ns/s ",
            stream->drift.observedPhaseChangeRateInternal,
            0,0,0,0,0);
   }

   zl303xx_PtpStreamTsLogOneSec(stream);
}

/* zl303xx_PtpTimerMsgTx */
/** 

   Sends one or more PTP messages based on a timer expiry.

  Parameters:
   [in]   pStream    Stream to send the message(s) from.
   [in]   type       Message type to send.
   [in]   numToSend  Number of messages to send.
   [in]   msgTxFn    Function to call to send this type of message.

*******************************************************************************/
static void zl303xx_PtpTimerMsgTx(zl303xx_ClockStreamS *pStream,
                                  zl303xx_MessageTypeE type,
                                  Uint32T numToSend,
                                  void (*msgTxFn)(zl303xx_ClockStreamS *))
{
   const char *msgStr = zl303xx_PtpMessageTypeToStr(type);

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5,
         "Stream %u: %s TX expiry (%lu to send)",
         pStream->clockStreamHandle, msgStr, numToSend, 0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PKT_SCHED, 5,
         "Stream %u: %s TX expiry (%lu to send)",
         pStream->clockStreamHandle, msgStr, numToSend, 0,0,0);

   while (numToSend--)
   {
      msgTxFn(pStream);
   }
}

/* zl303xx_PtpStreamCalculateDrift */
/** 

   Calculate the drift of this slave against the master in ns/s.

  Parameters:
   [in]   pPtpStream  Pointer to a stream instance.

  Notes:
   The drift calculation is over the time period since this function was last
   called. This function is designed to be called periodically, once per second.

*******************************************************************************/
void zl303xx_PtpStreamCalculateDrift(zl303xx_ClockStreamS *pPtpStream)
{
   zl303xx_TimeStamp deltaOffset, deltaInterval;
   Uint64S tempStruct;
   Sint32T negSign = 0;
   Sint64T interval64, offset64, drift64;
   Sint32T temp32;

   if ((pPtpStream->drift.latestSyncRxTime.secondsField.hi == pPtpStream->drift.prevSyncRxTime.secondsField.hi) &&
       (pPtpStream->drift.latestSyncRxTime.secondsField.lo == pPtpStream->drift.prevSyncRxTime.secondsField.lo) &&
       (pPtpStream->drift.latestSyncRxTime.nanosecondsField == pPtpStream->drift.prevSyncRxTime.nanosecondsField))
   {
      return;
   }

   /* Calculate the time interval since last calculated */
   subTime(&deltaInterval, &pPtpStream->drift.latestSyncRxTime, &pPtpStream->drift.prevSyncRxTime);
   /* Calculate delta offset since last time */
   subTime(&deltaOffset, &pPtpStream->farEndCurrentDS.offset_from_master, &pPtpStream->drift.prevOffsetFromMaster);

   /* We want ns drift per second. Use 64 bit maths to make it easier */
   /* According to the spec this only needs to be "an estimate of the drift" */
   if (isNegativeTime(&deltaOffset))
   {
      negSign = negSign ^ 1;
      negateTime(&deltaOffset);
   }

   if (isNegativeTime(&deltaInterval))
   {
      negSign = negSign ^ 1;
      negateTime(&deltaInterval);
   }

   /* Now have two positive numbers.
      Ignore the "high seconds" value since we assume that the delta times will never be > 136 years */
   offset64 = (Sint64T)deltaOffset.secondsField.lo * TEN_e9 + (Sint64T)deltaOffset.nanosecondsField;
   interval64 = (Sint64T)deltaInterval.secondsField.lo * TEN_e9 + (Sint64T)deltaInterval.nanosecondsField;

   drift64 = (offset64*TEN_e9)/interval64;

   ZL303XX_CONVERT_FROM_64(tempStruct, drift64);

   if ((tempStruct.hi > 0) || (tempStruct.lo > (Sint32T)0x7fffffff))
   {
      /* number is too large */
      temp32 = (Sint32T)0x7fffffff;
   }
   else
   {
      temp32 = (Sint32T)tempStruct.lo;
   }

   if (negSign != 0)
   {
      /* negate and subtract an extra ns to force the overflow value to go to 0x80000000 */
      temp32 = -temp32 - 1;
   }

   /* Store into the internal variable, this is so the externally visible variable will always be zero
      as required by the standard */
   pPtpStream->drift.observedPhaseChangeRateInternal = temp32;

   /* Store values for next time */
   pPtpStream->drift.prevOffsetFromMaster = pPtpStream->farEndCurrentDS.offset_from_master;
   pPtpStream->drift.prevSyncRxTime = pPtpStream->drift.latestSyncRxTime;
}  /* END zl303xx_PtpStreamCalculateDrift */

/*****************   INTERNAL ROUTINES FOR STREAM STATE CONTROL  **************/

/* zl303xx_PtpStreamStateToS0 */
/** 

   Executes stream state transitions for streams on SLAVE_ONLY clocks. This is
   a flag set on the clock (Refer to Figures 24 & 26 of the IEEE 1588-2008
   standard). In this case,
   - the streams must be in SLAVE, LISTENING or UNCALIBRATED modes.
   - any MASTER_ONLY stream is considered a configuration ERROR which is
     logged but allowed to continue in case of a mode over-ride.

  Parameters:
   [in]   pPtpStream  Pointer to the stream to set in SLAVE_ONLY state.
   [in]   streamBest  Pointer to the Stream with the best network server or
                            NULL if none exists.

  Notes:
   Assumes all pointers are valid.

*******************************************************************************/
void zl303xx_PtpStreamStateToS0(zl303xx_ClockStreamS *pPtpStream,
                                zl303xx_ClockStreamS *streamBest)
{
   const char *fnName = "zl303xx_PtpStreamStateToS0";

   /* Log the input */
   const char *streamModeStr =  zl303xx_PtpStreamModeToStr(pPtpStream->config.mode);
   const char *streamStateStr = zl303xx_PtpStreamSubStateToStr(pPtpStream->streamSubstate);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
         "%s: Stream (%d): state %d(%s): mode %d(%s)",
         fnName, pPtpStream->clockStreamHandle,
         pPtpStream->streamSubstate, streamStateStr,
         pPtpStream->config.mode, streamModeStr);

   /* Since this is on a SLAVE_ONLY clock there cannot be any MASTER_ONLY
    * streams. If this is, log a message before setting the stream to MASTER. */
   if (pPtpStream->config.mode == ZL303XX_PTP_STREAM_MASTER_ONLY)
   {
      if ((pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_PRE_MASTER) &&
          (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_MASTER))
      {
         /* This is an ERRORed configuration that should never occur. */
         ZL303XX_TRACE_ALWAYS("%s ERROR: MASTER_ONLY stream (%d) on SLAVE_ONLY clock to GRANDMASTER mode",
               fnName,
               pPtpStream->clockStreamHandle, 0,0,0,0);

         zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_PRE_MASTER, pPtpStream);
         zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_GRANDMASTER);
      }
   }

   /* Set any other non-BEST streams to UNCALIBRATED if they are qualified, or
    * LISTENING otherwise. */
   else if (pPtpStream != streamBest)  /* streamBest may be NULL as well. */
   {
      if (pPtpStream->qualFmtEntry == ZL303XX_TRUE)
      {
         if (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_UNCALIBRATED)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
                  "%s: Requesting Stream (%d) To UNCALIBRATED state",
                  fnName,
                  pPtpStream->clockStreamHandle, 0,0,0,0);

            zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_UNCALIBRATED, pPtpStream);
            zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_MONITORING);
         }
      }
      else
      {
         if (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_LISTENING)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
                  "%s: Requesting Stream (%d) To LISTENING state",
                  fnName,
                  pPtpStream->clockStreamHandle, 0,0,0,0);

            zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_LISTENING, pPtpStream);
            zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_LISTENING);
         }
      }
   }

   else /* (pPtpStream == streamBest) */
   {
      if (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_SLAVE)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
               "%s: Requesting Stream (%d) To SLAVE state",
               fnName,
               pPtpStream->clockStreamHandle, 0,0,0,0);

         /* Move the stream to UNCALIBRATED first and let the state machine
          * move it to slave.    */
         zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_UNCALIBRATED, pPtpStream);
         zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_SLAVE);
      }
   }
}  /* END zl303xx_PtpStreamStateToS0 */

/* zl303xx_PtpStreamStateToM1 */
/** 

   Executes stream state transitions for streams on MASTER_ONLY clocks. This is
   defined as a Clock with clockClass <= 127. (Refer to Figures 23 & 26 of the
   IEEE 1588-2008 standard). In this case,
   - the stream cannot be set to SLAVE mode.
   - the streams must be in MASTER or PASSIVE modes.
   - a SLAVE_ONLY stream is considered a configuration ERROR and is placed in
     UNCALIBRATED state.

  Parameters:
   [in]  pPtpStream  Pointer to the stream to set in MASTER_ONLY (M1) state.
   [in]  streamBest  Pointer to the Stream with the best network server. In
                           the M1 scenario, we expect it to be NULL but may not
                           be if another Master exists in the network (in which
                           case this MASTER should be going PASSIVE soon).

  Notes:
   Assumes all pointers are valid.

*******************************************************************************/
void zl303xx_PtpStreamStateToM1(zl303xx_ClockStreamS *pPtpStream,
                                zl303xx_ClockStreamS *streamBest)
{
   const char *fnName = "zl303xx_PtpStreamStateToM1";

   /* Pointer to this Stream's clock. */
   zl303xx_PtpClockS *pPtpClock = pPtpStream->portData->clock;

   /* Log the input */
   const char *streamModeStr =  zl303xx_PtpStreamModeToStr(pPtpStream->config.mode);
   const char *streamStateStr = zl303xx_PtpStreamSubStateToStr(pPtpStream->streamSubstate);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
         "%s: Stream (%d): state %d(%s): mode %d(%s)",
         fnName, pPtpStream->clockStreamHandle,
         pPtpStream->streamSubstate, streamStateStr,
         pPtpStream->config.mode, streamModeStr);

   /* For M1 Clocks and Streams there should be no BEST but if there is then
    * this Clock should go to PASSIVE Mode. */
   if (streamBest != NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
            "%s: Best Stream(%d) is NOT NULL on M1 Clock: Expect PASSIVE",
            fnName, streamBest->clockStreamHandle, 0,0,0,0);
   }

   /* Since this is a MASTER_ONLY clock, no stream should be configured for
    * SLAVE_ONLY operation. If it is, leave UNCALIBRATED or LISTENING streams
    * alone. Otherwise, force it to LISTENING and log a message. */
   if (pPtpStream->config.mode == ZL303XX_PTP_STREAM_SLAVE_ONLY)
   {
      if ((pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_UNCALIBRATED) &&
          (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_LISTENING))
      {
         /* This is an ERRORed configuration that should never occur. */
         ZL303XX_TRACE_ALWAYS(
               "%s ERROR: SLAVE_ONLY stream (%d) on MASTER_ONLY clock to LISTENING",
               fnName,
               pPtpStream->clockStreamHandle, 0,0,0,0);

         zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_LISTENING, pPtpStream);
         zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_LISTENING);
      }
   }

   /* Stream Mode of BMC or Master-Only uses the same logic. */
   /* -> (BMC || Master-Only) with Local Clock as the Best. */
   else if (((pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_LOCAL_CLOCK) ||
             (pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_LOCAL_PORT)) &&
                /* Port must be allowed to be a MASTER. */
                (pPtpStream->portData->config.unicastNegotiation.denyServiceRequests == ZL303XX_FALSE))
   {
      /* Set all streams to PRE_MASTER. */
      if ((pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_PRE_MASTER) &&
          (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_MASTER))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
               "%s: Requesting Stream (%d) To PRE_MASTER state",
               fnName,
               pPtpStream->clockStreamHandle, 0,0,0,0);

         /* Request the stream go to MASTER. M1 Clocks have no PRE_MASTER state. */
         zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_MASTER, pPtpStream);
         zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_GRANDMASTER);
      }
   }

   /* -> (BMC || Master-Only) but a better Server exists. */
   else
   {
      /* Set all streams to PASSIVE. */
      if (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_PASSIVE)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
               "%s: Requesting Stream (%d) To PASSIVE state",
               fnName,
               pPtpStream->clockStreamHandle, 0,0,0,0);

         /* Request the stream go to PASSIVE.  */
         zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_PASSIVE, pPtpStream);
         zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_PASSIVE_MASTER);
      }
   }
}  /* END zl303xx_PtpStreamStateToM1 */

/* zl303xx_PtpStreamStateToM2 */
/** 

   Executes stream state transitions for streams on BMC clocks that are the
   Grandmaster in the network. This is defined as a Clock with clockClass > 127
   (Refer to Figures 23 & 26 of the IEEE 1588-2008 standard). In this case,
   - the stream cannot be set to SLAVE mode.
   - the streams must be in MASTER/BMC mode.
   - a SLAVE_ONLY stream is considered a configuration ERROR and is placed in
     UNCALIBRATED state.

  Parameters:
   [in]  pPtpStream  Pointer to the stream to set in Grandmaster (M2) state.

  Notes:
   Assumes all pointers are valid.

*******************************************************************************/
void zl303xx_PtpStreamStateToM2(zl303xx_ClockStreamS *pPtpStream)
{
   const char *fnName = "zl303xx_PtpStreamStateToM2";

   /* Log the input */
   const char *streamModeStr =  zl303xx_PtpStreamModeToStr(pPtpStream->config.mode);
   const char *streamStateStr = zl303xx_PtpStreamSubStateToStr(pPtpStream->streamSubstate);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
         "%s: Stream (%d): state %d(%s): mode %d(%s)",
         fnName, pPtpStream->clockStreamHandle,
         pPtpStream->streamSubstate, streamStateStr,
         pPtpStream->config.mode, streamModeStr);

   /* If this Clock is going to M2 Grandmaster, any SLAVE_ONLY stream are
    * either not receiving ANNOUNCE (i.e. LISTENING) or are receiving ANNOUNCE
    * from a lesser server. Or for Unicast Negotiated streams that are Monitoring.
    * - If it is currently LISTENING, let it continue.
    * - If UNCALIBRATED, let it continue.
    * - Otherwise, force it to LISTENING and log a message. */
   if ((pPtpStream->config.mode == ZL303XX_PTP_STREAM_SLAVE_ONLY) ||

       /* A UniNeg Stream that is NOT transmitting Announce Messages. */
       ((pPtpStream->config.unicast == ZL303XX_TRUE) &&
        (pPtpStream->portData->config.unicastNegotiation.enabled == ZL303XX_TRUE) &&
        (pPtpStream->contractTx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].state != ZL303XX_PTP_CONTRACT_TX_ACTIVE)))
   {
      if (pPtpStream->config.mode == ZL303XX_PTP_STREAM_SLAVE_ONLY)
      {
         /* This is potentially an ERRORed configuration since the Clock is
          * configured for BMC mode but the stream is SLAVE_ONLY. */
         ZL303XX_TRACE_ALWAYS("%s CONFLICT: SLAVE_ONLY stream (%d) on BMC clock",
               fnName, pPtpStream->clockStreamHandle, 0,0,0,0);
      }

      if ((pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_UNCALIBRATED) &&
          (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_LISTENING))
      {
         zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_LISTENING, pPtpStream);
         zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_LISTENING);
      }
   }

   /* If this is not a SLAVE_ONLY stream make sure it is in the PRE_MASTER or
    * MASTER state (unless it is a unicast negotiated stream in which case just
    * leave this alone). */
   else
   {
      /* If the Port is blocked from being a MASTER then force it to PASSIVE. */
      if (pPtpStream->portData->config.unicastNegotiation.denyServiceRequests == ZL303XX_TRUE)
      {
         if (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_PASSIVE)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
                  "%s: Requesting Stream (%d) To PASSIVE state on Port that denies MASTER Service",
                  fnName,
                  pPtpStream->clockStreamHandle, 0,0,0,0);

            zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_PASSIVE, pPtpStream);
            zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_PASSIVE_MASTER);
         }

         return;
      }

      /* Check if the stream is looped back to another port on the same clock.
       * In this case, set the stream to passive. */
      /* See IEEE-1588-2008 Section 9.5.2.3 */
      if ((pPtpStream->farEndParentDS.u.v2.parentPortIdentity.portNumber != 0) &&
          (pPtpStream->portData->portDS.portIdentity.portNumber >
                pPtpStream->farEndParentDS.u.v2.parentPortIdentity.portNumber) &&
           zl303xx_PtpV2EqualClockIdentities(
                pPtpStream->farEndParentDS.u.v2.parentPortIdentity.clockIdentity,
                pPtpStream->portData->clock->config.defaultDS.clockIdentity))
      {
         if (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_PASSIVE)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
                  "%s: Requesting Stream (%d) To PASSIVE state",
                  fnName,
                  pPtpStream->clockStreamHandle, 0,0,0,0);

            zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_PASSIVE, pPtpStream);
            zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_PASSIVE_MASTER);
         }

         /* Return early to simplify code:
          * - If the stream was Better-by-Topology it is set to PASSIVE and we can return.
          * - If NOT Better-by-Topology, we are not within this IF statement and the stream
          *   will be processed below. */
         return;
      }

      if ((pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_PRE_MASTER) &&
          (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_MASTER))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
               "%s: Requesting Stream (%d) To PRE_MASTER state",
               fnName,
               pPtpStream->clockStreamHandle, 0,0,0,0);

         /* Request the stream go to MASTER. M2 Clocks have no PRE_MASTER state. */
         zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_MASTER, pPtpStream);
         zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_MASTER);
      }
   }
}  /* END zl303xx_PtpStreamStateToM2 */

/* zl303xx_PtpStreamStateToM3 */
/** 

   Executes stream state transitions for streams on Client clocks. This is
   defined as a Clock with clockClass > 127 that is being served by another
   clock (the grandmaster) elsewhere in the network. (Refer to Figures 23 & 26
   of the IEEE 1588-2008 standard). In this case,
   - there should be a single SLAVE stream on the clock.
   - the secondary streams must be in MASTER or PASSIVE mode.
   - any secondary streams that are set to SLAVE_ONLY is considered a
     configuration ERROR and is placed in UNCALIBRATED state.

  Parameters:
   [in]  pPtpStream  Pointer to the stream to set in Client (M3) operation.
   [in]  streamBest  Pointer to the Stream with the best network server. This
                           may be a local stream or an EXTERNAL stream with data
                           saved in the clock ParentDS. (streamBest cannot be NULL).

  Notes:
   Assumes all pointers are valid.

*******************************************************************************/
void zl303xx_PtpStreamStateToM3(zl303xx_ClockStreamS *pPtpStream,
                                zl303xx_ClockStreamS *streamBest)
{
   const char *fnName = "zl303xx_PtpStreamStateToM3";

   /* This routine is called only if gmSource.location is LOCAL_STREAM,
    * LOCAL_PORT or EXTERNAL. In all cases, *streamBest should not be NULL. */
   if ((pPtpStream == NULL) ||
       (streamBest == NULL))
   {
      ZL303XX_TRACE_ALWAYS("%s: NULL pointer (pStrm: %p) (pBest: %p)",
            fnName, pPtpStream, streamBest, 0,0,0);
      return;
   }
   else
   {
      /* Log the input */
      const char *streamModeStr =  zl303xx_PtpStreamModeToStr(pPtpStream->config.mode);
      const char *streamStateStr = zl303xx_PtpStreamSubStateToStr(pPtpStream->streamSubstate);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
            "%s: Stream (%d): state %d(%s): mode %d(%s)",
            fnName, pPtpStream->clockStreamHandle,
            pPtpStream->streamSubstate, streamStateStr,
            pPtpStream->config.mode, streamModeStr);
   }

   /* If this Clock is going to M3 (Slave/BC), at least 1 local stream is
    * receiving ANNOUNCE Messages. If a stream is configured for SLAVE_ONLY
    * operations and:
    * - it is the BEST, force it to UNCALIBRATED if not already SLAVE.
    * - it is not the BEST, then:
    *   - If it is currently LISTENING or UNCALIBRATED, let it continue.
    *   - ELSE, force it to UNCALIBRATED since it is likely intended for
    *     MONITORING Mode. */
    /* NOTE: This is also consistent with the actions taken in the S0 case. */
   if (pPtpStream->config.mode == ZL303XX_PTP_STREAM_SLAVE_ONLY)
   {
      /* This stream should go to MASTER but is in SO mode. */
      if (pPtpStream != streamBest)
      {
         if ((pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_UNCALIBRATED) &&
             (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_LISTENING))
         {
            /* This is possibly an ERRORed configuration . */
            ZL303XX_TRACE_ALWAYS("%s ERROR: SLAVE_ONLY stream (%d) on BMC clock to UNCALIBRATED state ",
                  fnName,
                  pPtpStream->clockStreamHandle, 0,0,0,0);
            zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_UNCALIBRATED, pPtpStream);
            zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_MONITORING);
         }
      }

      else  /* This SO stream is the actual best. */
      {
         if (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_SLAVE)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
                  "%s: Requesting Stream (%d) To SLAVE state",
                  fnName,
                  pPtpStream->clockStreamHandle, 0,0,0,0);

            /* Move the stream to UNCALIBRATED first and let the state machine
             * move it to slave.    */
            zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_UNCALIBRATED, pPtpStream);
            zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_SLAVE);
         }
      }
   }

   else /* All MASTER_ONLY and BMC Streams that are not best go to MASTER or PASSIVE mode. */
   {
      /* Set the best stream to SLAVE. */
      if (pPtpStream != streamBest)
      {
         /* If the Port is blocked from being a MASTER then force it to PASSIVE. */
         if (pPtpStream->portData->config.unicastNegotiation.denyServiceRequests == ZL303XX_TRUE)
         {
            if (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_PASSIVE)
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
                     "%s: Requesting Stream (%d) To PASSIVE state on Port that denies MASTER Service",
                     fnName,
                     pPtpStream->clockStreamHandle, 0,0,0,0);

               zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_PASSIVE, pPtpStream);
               zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_PASSIVE_MASTER);

               /* Return early to simplify code: */
               return;
            }
         }

         /* If the best stream is only topologically better than the current
          * stream, the current stream must go to the PASSIVE state. */

         /* Topology check is different for the 8275 profiles. */
         if ((pPtpStream->portData->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_1v1) ||
             (pPtpStream->portData->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_1v2) ||
             (pPtpStream->portData->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_2))
         {
            /* Use the farEndData of each to do the comparison. */
            zl303xx_PtpG8275ProfileCompareDataS bestDataA;
            zl303xx_PtpG8275ProfileCompareDataS compDataB;

            /* Convert the Stream data to BMC data. */
            (void)zl303xx_PtpG8275BmcaStreamDataAdd(streamBest, &bestDataA);
            (void)zl303xx_PtpG8275BmcaStreamDataAdd(pPtpStream, &compDataB);

            if (zl303xx_PtpBmcaG8275ProfileComparison(&bestDataA, &compDataB, NULL) != ZL303XX_PTP_BMC_A_BETTER_THAN_B)
            {
               if (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_PASSIVE)
               {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
                        "%s: Requesting Stream (%d) To PASSIVE state",
                        fnName,
                        pPtpStream->clockStreamHandle, 0,0,0,0);

                  zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_PASSIVE, pPtpStream);
                  zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_PASSIVE_MASTER);
               }

               /* Return early to simplify code:
                * - If the stream was Better-by-Topology it is set to PASSIVE and we can return.
                * - If NOT Better-by-Topology, we are not within this IF statement and the stream
                *   will be processed below. */
               return;
            }
         }

         /* Only a Qualified Stream can go to PASSIVE ... */
         /* Un-Qualified ones will go to MASTER (below). */
         else if ((pPtpStream->qualFmtEntry == ZL303XX_TRUE) &&

            /* With the same Grandmaster... */
            zl303xx_PtpV2EqualClockIdentities(
                   pPtpStream->farEndParentDS.u.v2.grandmasterIdentity,
                   streamBest->farEndParentDS.u.v2.grandmasterIdentity)   &&

             /* That is only topologically worse (B better than A topologically = -2) */
             zl303xx_PtpV2BmcDataSetCompareSameGM(
                   pPtpStream->farEndCurrentDS.stepsRemoved,
                   &pPtpStream->farEndParentDS.u.v2.parentPortIdentity,
                   &pPtpStream->portData->portDS.portIdentity,
                   streamBest->farEndCurrentDS.stepsRemoved,
                   &streamBest->farEndParentDS.u.v2.parentPortIdentity,
                   &streamBest->portData->portDS.portIdentity) != ZL303XX_PTP_BMC_B_BETTER_THAN_A)
         {
            if (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_PASSIVE)
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
                     "%s: Requesting Stream (%d) To PASSIVE state",
                     fnName,
                     pPtpStream->clockStreamHandle, 0,0,0,0);

               zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_PASSIVE, pPtpStream);
               zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_PASSIVE_MASTER);
            }

            /* Return early to simplify code:
             * - If the stream was Better-by-Topology it is set to PASSIVE and we can return.
             * - If NOT Better-by-Topology, we are not within this IF statement and the stream
             *   will be processed below. */
            return;
         }

         /* Check if the stream is looped back to another port on the same clock.
          * In this case, set the stream to passive. */
         /* See IEEE-1588-2008 Section 9.5.2.3 */
         if ((pPtpStream->farEndParentDS.u.v2.parentPortIdentity.portNumber != 0) &&
             (pPtpStream->portData->portDS.portIdentity.portNumber >
                   pPtpStream->farEndParentDS.u.v2.parentPortIdentity.portNumber) &&
              zl303xx_PtpV2EqualClockIdentities(
                   pPtpStream->farEndParentDS.u.v2.parentPortIdentity.clockIdentity,
                   pPtpStream->portData->clock->config.defaultDS.clockIdentity))
         {
            if (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_PASSIVE)
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
                     "%s: Requesting Stream (%d) To PASSIVE state",
                     fnName,
                     pPtpStream->clockStreamHandle, 0,0,0,0);

               zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_PASSIVE, pPtpStream);
               zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_PASSIVE_MASTER);
            }

            /* Return early to simplify code:
             * - If the stream was Better-by-Topology it is set to PASSIVE and we can return.
             * - If NOT Better-by-Topology, we are not within this IF statement and the stream
             *   will be processed below. */
            return;
         }

         if ((pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_PRE_MASTER) &&
             (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_MASTER))
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
                  "%s: Requesting Stream (%d) To PRE_MASTER state",
                  fnName,
                  pPtpStream->clockStreamHandle, 0,0,0,0);

            /* Request the stream go to PRE_MASTER. The transition to MASTER
             * will be handled by the Stream State Machine.  */
            zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_PRE_MASTER, pPtpStream);
            zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_REPEATER);
         }
      }

      else /* The Slave Stream */
      {
         if (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_SLAVE)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2,
                  "%s: Requesting Stream (%d) To SLAVE state",
                  fnName,
                  pPtpStream->clockStreamHandle, 0,0,0,0);

            /* Move the stream to UNCALIBRATED first and let the state machine
             * move it to slave.    */
            zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_UNCALIBRATED, pPtpStream);
            zl303xx_PtpEventStreamStateChange(pPtpStream, ZL303XX_PTP_STREAM_OPER_STATE_SLAVE);
         }
      }
   }
}  /* END zl303xx_PtpStreamStateToM3 */

/*****************   END   ****************************************************/
