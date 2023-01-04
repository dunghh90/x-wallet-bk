

/*****************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
* Copyright 2006-2018 Microsemi Semiconductor Limited.
* All rights reserved.
*
*  Module Description:
*     This file contains the main application for implementing the
*     IEEE-1588 PTP protocol.
*
*******************************************************************************/

/****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_ErrTrace.h"

#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpControl.h"

#include "zl303xx_OsHeaders.h"

#ifdef SOCPG_PORTING
#ifdef OS_FREERTOS
#include <time.h>
#endif
#endif

#ifdef OS_LINUX
#ifdef __STRICT_ANSI__
    #define __STRICT_ANSI_TMP__ __STRICT_ANSI__
    #undef __STRICT_ANSI__  /* Undefine __STRICT_ANSI__ so gmtime_r() is visible */
#endif
#include <time.h>
#ifdef __STRICT_ANSI_TMP__
    #define __STRICT_ANSI__ __STRICT_ANSI_TMP__ /* Redefine __STRICT_ANSI__ as per above dance */
    #undef __STRICT_ANSI_TMP__
#endif
#endif

/****************   DEFINES     **********************************************/

/****************   STATIC FUNCTION DECLARATIONS   ***************************/
_ZL303XX_LOCAL void zl303xx_PtpDisplayUtcTime(zl303xx_TimeStamp *timeRep);

static zlStatusE zl303xx_PtpHandleTimerTick(zl303xx_PtpClockS *pClock, zl303xx_PtpTaskMsgU *pMsg);
static zlStatusE zl303xx_PtpHandleRecvPkt(zl303xx_PtpClockS *pClock, zl303xx_PtpTaskMsgU *pMsg);
static zlStatusE zl303xx_PtpHandleTsRecordReady(zl303xx_PtpClockS *pClock, zl303xx_PtpTaskMsgU *pMsg);
static zlStatusE zl303xx_PtpHandleControlCmd(zl303xx_PtpClockS *pClock, zl303xx_PtpTaskMsgU *pMsg);
static zlStatusE zl303xx_PtpHandleManualCancel(zl303xx_PtpClockS *pClock, zl303xx_PtpTaskMsgU *pMsg);

/****************   STATIC GLOBAL VARIABLES   ********************************/
typedef struct zl303xx_PtpTaskMsgProcessS
{
   const char *name;
   zlStatusE (*func)(zl303xx_PtpClockS *pClock, zl303xx_PtpTaskMsgU *pMsg);
} zl303xx_PtpTaskMsgProcessS;

static const zl303xx_PtpTaskMsgProcessS Zl303xx_PtpTaskMsgProcess[PTP_TASK_MSG_NUM_TYPES] = {
      {"NONE",            NULL},
      {"QUIT",            NULL},
      {"TIMER_TICK",      zl303xx_PtpHandleTimerTick},
      {"RECV_PKT",        zl303xx_PtpHandleRecvPkt},
      {"TS_RECORD_READY", zl303xx_PtpHandleTsRecordReady},
      {"CONTROL_CMD",     zl303xx_PtpHandleControlCmd},
      {"MANUAL_CANCEL",   zl303xx_PtpHandleManualCancel}
};

/****************   EXPORTED GLOBAL VARIABLES   ******************************/

/* Manufacturer's ID. This should be set by the application */
/* The version 1 manufacturer ID string format is name;modelNumber
   The version 2 manufacturer ID string format is name;modelNumber;serialNumber */
char PtpManufacturerID[PTP_MANUFACTURER_ID_LENGTH] = "";

/* Strings used for Version 2 only */
/* Revision ID. This should be set by the application */
/* The format is HardwareVersion;FirmwareVersion;SoftwareVersion. Individual fields can be of zero length */
char PtpRevisionData[ZL303XX_PTP_NODE_REV_DATA_LEN] = "";

/* User Description (optional). This should be set by the application if required */
/* The format is userDefinedName;physicalLocation.
   Individual fields can be of zero length, or the whole string may be omitted if not required. */
char PtpUserDescription[ZL303XX_PTP_NODE_USER_DESC_LEN] = "";     /* Initialize to the empty string */

/* Delay Request-Response Default PTP profile, from IEEE 1588-2008 Annex J.3 */
const zl303xx_PtpProfileS Zl303xx_PtpE2EDefaultProfileInfo = {
   "Default PTP profile for use with the delay request-response mechanism.",
   "Version 1.0",
   {0x00, 0x1B, 0x19, 0x00, 0x01, 0x00},
   "This profile is specified by the Precise Networked Clock Synchronization Working Group of the IM/ST Committee.",
   "A copy may be obtained by ordering IEEE Std 1588-2008 from the IEEE Standards Organization http://standards.ieee.org/."
};

/* Peer-to-Peer Default PTP profile, from IEEE 1588-2008 Annex J.4 */
const zl303xx_PtpProfileS Zl303xx_PtpP2PDefaultProfileInfo = {
   "Default PTP profile for use with the peer delay mechanism.",
   "Version 2.0",
   {0x00, 0x1B, 0x19, 0x00, 0x02, 0x00},
   "This profile is specified by the Precise Networked Clock Synchronization Working Group of the IM/ST Committee.",
   "A copy may be obtained by ordering IEEE Std 1588-2008 from the IEEE Standards Organization http://standards.ieee.org/."
};

/****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* zl303xx_PtpTask */
/** 

   Main PTP stack engine task.

  Return Value:
      0  OK
     -1  An error occurred

*******************************************************************************/
Sint32T zl303xx_PtpTask(void)
{
   zlStatusE status;
   const char * fnName = "zl303xx_PtpTask";
   zl303xx_PtpClockS *pClock;
   zl303xx_PtpTaskMsgU msg;
   const zl303xx_PtpTaskMsgProcessS *taskMsgProc;

   /* Get a pointer to the most recently created clock. */
   status = zl303xx_PtpNodeLastClockGet(&pClock);
   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE_ERROR("%s: Failed to get most recent clock", fnName, 0,0,0,0,0);
      return -1;  /* Fatal error */
   }

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "event POWERUP", 0,0,0,0,0,0);

   while (1)
   {
      /* Wait until an event message is received */
      status = zl303xx_PtpClockMsgQRecv(pClock, &msg);
      if (status != ZL303XX_OK)
      {
         /* MSG Q receive failed. Wait and continue. */
         ZL303XX_TRACE_ERROR("Clock %u: Message queue received failed (%d)",
                             pClock->clockHandle, status, 0,0,0,0);
         OS_TASK_DELAY(100);
         continue;
      }

      if (msg.type == PTP_TASK_MSG_QUIT)
      {
          ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "event POWERDOWN", 0,0,0,0,0,0);
          OS_TASK_DELAY(2000);
          break;
      }

      {
         OS_TASKMON_FUNC_START();

         if (msg.type >= ZL303XX_ARRAY_SIZE(Zl303xx_PtpTaskMsgProcess))
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                  "Clock %u: Received invalid task message type %d",
                  pClock->clockHandle, msg.type, 0,0,0,0);
            continue;
         }

         taskMsgProc = &Zl303xx_PtpTaskMsgProcess[msg.type];

         /* If no function defined for this message type, do nothing */
         if (taskMsgProc->func == NULL)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                  "Clock %u: No handler for task message %s (%d)",
                  pClock->clockHandle, taskMsgProc->name, msg.type, 0,0,0);
            continue;
         }

         status = zl303xx_PtpClockMutexLock(pClock);
         if (status != ZL303XX_OK)
         {
            continue;  /* Mutex lock failed--try again next message */
         }

         status = taskMsgProc->func(pClock, &msg);
         if (status != ZL303XX_OK)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                  "Clock %u: Handler for task message %s (%d) failed with status %d",
                  pClock->clockHandle, taskMsgProc->name, msg.type, status, 0,0);
            status = ZL303XX_OK;
         }

         status = zl303xx_PtpClockMutexUnlock(pClock, status);
         if (status != ZL303XX_OK)
         {
            break;  /* Mutex unlock failed--fatal error */
         }

         OS_TASKMON_FUNC_END();
      }
   }

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "ptpTask shutdown", 0,0,0,0,0,0);

   return -1;   /* Should not happen - task should be killed during delay */
}

/*

  Function Name:
   zl303xx_PtpGetLocalUtcTime

  Details:
   Gets the current UTC time.

  Parameters:
   [in]   clockPortHandle   Index into the port data array.
   [out]  utcTime           The local UTC time.

  Return Value:
   nothing

  (Deprecated)
   This function has been replaced by zl303xx_PtpClockUtcGet().

*******************************************************************************/
void zl303xx_PtpGetLocalUtcTime(zl303xx_PtpPortHandleT clockPortHandle, zl303xx_TimeStamp *utcTime)
{
   zlStatusE status = zl303xx_PtpClockUtcGet(clockPortHandle, utcTime);

   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE_ERROR("zl303xx_PtpClockUtcGet() failed with status %d.", status, 0,0,0,0,0);
   }
}

/*

  Function Name:
   zl303xx_PtpPrintLocalUtcTime

  Details:
   Prints the the current local UTC time

  Parameters:
   [in]   pClock  Pointer to an existing clock.

  Return Value:
   nothing

*******************************************************************************/
void zl303xx_PtpPrintLocalUtcTime(zl303xx_PtpClockS *pClock)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_TimeStamp utc;

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockUtcGetInternal(pClock, &utc, NULL);
   }

   if (status == ZL303XX_OK)
   {
      zl303xx_PtpDisplayUtcTime(&utc);
   }
}

/*

  Function Name:
   zl303xx_PtpPrintMasterUtcTime

  Details:
   Prints the the current Master UTC time for the first stream found

  Parameters:
        None

  Return Value:
   nothing

 *****************************************************************************/
void zl303xx_PtpPrintMasterUtcTime(void)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_TimeStamp timeRep;
   zl303xx_ClockStreamS *clockStream;

   /* Find the first valid clock stream */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpFindFirstActiveStream(&clockStream);
   }

   /* Get the time string for this clock */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockUtcGetInternal(clockStream->portData->clock, &timeRep, NULL);
   }

   /* Convert to the master's epoch */
   if (status == ZL303XX_OK)
   {
      subTime(&timeRep, &timeRep, &clockStream->farEndCurrentDS.offset_from_master);
      zl303xx_PtpDisplayUtcTime(&timeRep);
   }
}


/*

  Function Name:
   zl303xx_PtpSetDebugLevel

  Details:
   Set the amount of debug for the PTP engine

  Parameters:
       newLevel    The debug level to set. Zero = off

  Return Value:
   zlStatusE

 *****************************************************************************/
zlStatusE zl303xx_PtpSetDebugLevel(Uint32T newLevel)
{
   zl303xx_TraceSetLevel(ZL303XX_MOD_ID_PTP_ENGINE, newLevel);

   return ZL303XX_OK;
}

/*

  Function Name:
   zl303xx_PtpEnableUtcTimeDisplay

  Details:
   Externally callable function to toggle the display of the UTC time every second

  Parameters:
   [in]  clockHandle  Index into the clock data array
   [in]  enable       1 to enable Utc time display, 0 to disable

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_PtpEnableUtcTimeDisplay(zl303xx_PtpClockHandleT clockHandle, Uint32T enable)
{
    zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* An error is returned if the clock is NULL. */
   status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   if (status == ZL303XX_OK)
   {
      if (enable)
      {
         pPtpClock->utcTimeDisplay = ZL303XX_TRUE;
      }
      else
      {
         pPtpClock->utcTimeDisplay = ZL303XX_FALSE;
      }

      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}

/**
    zl303xx_PtpDefaultProfileE2EInfoGet

  Details:
   Retrieves the profile description information for the Default(E2E) profile.

  Parameters:
   [in]  profileInfo    Pointer to the structure to store the profile information.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

 *****************************************************************************/
zlStatusE zl303xx_PtpDefaultProfileE2EInfoGet(const zl303xx_PtpProfileS **profileInfo)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(profileInfo);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      *profileInfo = &Zl303xx_PtpE2EDefaultProfileInfo;
   }

   return status;
}

/**
    zl303xx_PtpDefaultProfileP2PInfoGet

  Details:
   Retrieves the profile description information for the Default(P2P) profile.

  Parameters:
   [in]  profileInfo    Pointer to the structure to store the profile information.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

 *****************************************************************************/
zlStatusE zl303xx_PtpDefaultProfileP2PInfoGet(const zl303xx_PtpProfileS **profileInfo)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(profileInfo);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      *profileInfo = &Zl303xx_PtpP2PDefaultProfileInfo;
   }

   return status;
}

/**
    zl303xx_PtpDefaultProfileDurationCheck

  Details:
   Checks if the unicast contract duration provided is within the range of the
   Default profile.

  Parameters:
   [in]  duration       Pointer to the contract duration value to check. (Uses a
                              pointer to allow profiles to 'reset' the value if
                              out of range and strictFlag == FALSE).
   [in]  strictFlag     Flag indicating whether to over-write erroneous values
                              with the default value or return the
                              ZL303XX_PARAMETER_INVALID error.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      duration is NULL.
     ZL303XX_PARAMETER_INVALID    duration is out of range and strictFlag == TRUE.

 *****************************************************************************/
zlStatusE zl303xx_PtpDefaultProfileDurationCheck(
      Uint32T *duration,
      zl303xx_BooleanE strictFlag)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(duration);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      if ((*duration < ZL303XX_PTP_UNI_NEG_DURATION_MIN) ||
          (*duration > ZL303XX_PTP_UNI_NEG_DURATION_MAX))
      {
         ZL303XX_TRACE_ALWAYS(
               "   (ERROR) duration(%d) outside allowed Range (%u - %u)",
               *duration, ZL303XX_PTP_UNI_NEG_DURATION_MIN,
               ZL303XX_PTP_UNI_NEG_DURATION_MAX, 0,0,0);

         if (strictFlag == ZL303XX_TRUE)
         {
            ZL303XX_TRACE_ALWAYS(
                  "   (WARNING) Resetting to default (%u)",
                  *duration, ZL303XX_PTP_UNI_NEG_DURATION_DEF, 0,0,0,0);

            /* Set default, return no error. */
            *duration = ZL303XX_PTP_UNI_NEG_DURATION_DEF;
         }
      }
   }

   return status;
}  /* END zl303xx_PtpDefaultProfileDurationCheck */



/****************   STATIC FUNCTION DEFINITIONS   ****************************/

/* zl303xx_PtpHandleTimerTick */
/** 

   Handler for the TIMER_TICK task message. Executes all expired PTP timers and
   runs the state machine for all ports and streams.

*******************************************************************************/
static zlStatusE zl303xx_PtpHandleTimerTick(zl303xx_PtpClockS *pClock,
                                            zl303xx_PtpTaskMsgU *pMsg)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *port;

   zl303xx_PtpTimerExecAllExpired(pClock->clockHandle);


   /* Do low priority, global once-per-second actions. */
   if (pMsg->timerTick.secondFlag)
   {
      if (pClock->utcTimeDisplay)
      {
         /* Output the local time for this clock. */
         zl303xx_PtpPrintLocalUtcTime(pClock);
      }

      ZL303XX_PTP_FOR_EACH_PORT(&pClock->portList, port)
      {
         zl303xx_PtpV2Protocol(port, NULL);
      }
   }

   return status;
}

/* zl303xx_PtpHandleRecvPkt */
/** 

   Handler for the RECV_PKT task message. Determines the correct port structure
   pointer (must have) and stream structure pointer (may be NULL) and calls the
   state machine protocol.

*******************************************************************************/
static zlStatusE zl303xx_PtpHandleRecvPkt(zl303xx_PtpClockS *pClock,
                                          zl303xx_PtpTaskMsgU *pMsg)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockMsgQueueS *clockMsg = &pMsg->clockMsg;
   zl303xx_PortDataS *port = NULL;
   zl303xx_ClockStreamS *stream = NULL;

   (void)pClock;  /* Warning removal */

   if (status == ZL303XX_OK)
   {
      if (clockMsg->clockStreamId == (Uint32T)ZL303XX_PTP_INVALID)
      {
         status = zl303xx_PtpPortDataGet(clockMsg->recvMsgInfo.clockPortHandle, &port);
      }
      else
      {
         status = zl303xx_PtpStreamDataGet(clockMsg->clockStreamId, &stream);
      }
   }

   if (status == ZL303XX_OK)
   {
      if (clockMsg->clockStreamId == (Uint32T)ZL303XX_PTP_INVALID)
      {
         if (port->mcastStream != NULL)
         {
            /* If the port has a multicast stream configured, then check the
             * stream destination address first. */
            stream = zl303xx_PtpStreamExists(port, &clockMsg->recvMsgInfo.destAddr);
         }

         if (stream == NULL)
         {
            /* Check if there is a Unicast stream configured on the port. */
            stream = zl303xx_PtpStreamExists(port, &clockMsg->recvMsgInfo.srcAddr);
         }
      }

      if (stream != NULL)
      {
         /* It's possible that the matching stream is not attached to
          * the port provided in the PTP message. Use the matching
          * stream's port data for consistency. */
         port = stream->portData;
      }
      zl303xx_PtpV2handleRecvMsg(stream, port, clockMsg);

   }

   return status;
}

/* zl303xx_PtpHandleTsRecordReady */
/** 

   Handler for the TS_RECORD_READY task message. Determines the correct stream
   structure pointer (must have) and calls the state machine protocol.

*******************************************************************************/
static zlStatusE zl303xx_PtpHandleTsRecordReady(zl303xx_PtpClockS *pClock,
                                                zl303xx_PtpTaskMsgU *pMsg)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpTsRecordS *record = &pMsg->tsRecord.record;
   zl303xx_ClockStreamS *stream;

   (void)pClock;  /* Warning removal */

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(record->streamHandle, &stream);
   }

   if (status == ZL303XX_OK)
   {
      zl303xx_PtpTsRecordRegisterEgress(stream, record->messageType,
                                          record->sequenceId,
                                          &record->timeStamp);
   }

   return status;
}

/* zl303xx_PtpHandleControlCmd */
/** 

   Handler for the CONTROL_CMD task message. Determines the correct port
   structure pointer (must have) and stream structure pointer (may be NULL) and
   calls the state machine protocol.

*******************************************************************************/
static zlStatusE zl303xx_PtpHandleControlCmd(zl303xx_PtpClockS *pClock,
                                             zl303xx_PtpTaskMsgU *pMsg)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockMsgQueueS *clockMsg = &pMsg->clockMsg;
   zl303xx_ClockStreamS *stream = NULL;

   (void)pClock;  /* Warning removal */

   if (status == ZL303XX_OK)
   {
      if (clockMsg->clockStreamId != (Uint32T)ZL303XX_PTP_INVALID)
      {
         status = zl303xx_PtpStreamDataGet(clockMsg->clockStreamId, &stream);
      }
   }

   if (status == ZL303XX_OK)
   {
      handleLocalCtrlMessages(stream, clockMsg);
   }

   return status;
}

/* zl303xx_PtpHandleManualCancel */
/** 

   Handler for the MANUAL_CANCEL task message. Determines the correct port
   structure pointer (must have) and stream structure pointer (may be NULL) and
   sends a Signaling message with a CANCEL TLV appended.

*******************************************************************************/
static zlStatusE zl303xx_PtpHandleManualCancel(zl303xx_PtpClockS *pClock,
                                               zl303xx_PtpTaskMsgU *pMsg)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *port;
   zl303xx_ClockStreamS *stream = NULL;
   zl303xx_PtpManualCancelS *mc = &pMsg->manualCancel;
   zl303xx_PtpV2SignalTlvS tlv;
   Uint8T tlvBuf[6];
   Uint16T tlvBufLen = 0;
   zl303xx_PtpV2MsgHeaderS fakeRxHdr;

   (void)pClock;  /* Warning removal */

   /* Ensure the port still exists */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(mc->portHandle, &port);
   }

   if (status == ZL303XX_OK)
   {
      if (mc->streamHandle != (zl303xx_PtpStreamHandleT)ZL303XX_PTP_INVALID)
      {
         /* stream will only be modified if the stream still exists. If it has
          * been deleted, continue with stream=NULL and keep status=OK. */
         (void)zl303xx_PtpStreamDataGet(mc->streamHandle, &stream);
      }

      tlv.tlvType = PTP_TLV_CANCEL_UNICAST_TX;
      tlv.data.cancel.messageType = mc->msgType;
      tlvBufLen = zl303xx_PtpV2SignalTlvPack(&tlv, tlvBuf);
      if (tlvBufLen == 0)
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      fakeRxHdr.sequenceId = 0;
      OS_MEMSET(&fakeRxHdr.sourcePortIdentity, 0xFF, PTP_V2_CLK_IDENTITY_LENGTH);

      status = zl303xx_PtpV2UnicastNegTlvRespond(&mc->destAddr, stream, port,
                                                 &fakeRxHdr, tlvBuf, tlvBufLen);
   }

   return status;
}

/*

  Function Name:
   zl303xx_PtpDisplayUtcTime

  Details:
   Displays utc time value

  Parameters:
       timeRep     Current time value

  Return Value:
   nothing

 *****************************************************************************/
void zl303xx_PtpDisplayUtcTime(zl303xx_TimeStamp *timeRep)
{
   struct tm utcTime;
   time_t timeStamp;

   /* OS_MEMSET to eliminate warnings. */
   OS_MEMSET(&utcTime, 0, sizeof(struct tm));

   if (isNegativeTime(timeRep))
   {
      ZL303XX_ERROR_NOTIFY("Cannot process negative times");
      timeRep->secondsField.hi = 0;
      timeRep->secondsField.lo = 0;
      timeRep->nanosecondsField = 0;
   }

   /* Convert to broken down time */
   timeStamp = timeRep->secondsField.lo;

   {
      utcTime = *(gmtime(&timeStamp));
   }

   /* Note we cannot pass a non constant string to DBG() so have to let DBG
    * do the formatting */
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
         "UTC time (epoch %d)",
         timeRep->secondsField.hi, 0,0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
         "UTC time %4d-%02d-%02d %02d:%02d:%02d",
         (Uint32T) utcTime.tm_year+1900, (Uint32T) utcTime.tm_mon+1, (Uint32T) utcTime.tm_mday,
         (Uint32T) utcTime.tm_hour, (Uint32T) utcTime.tm_min, (Uint32T) utcTime.tm_sec);
}

