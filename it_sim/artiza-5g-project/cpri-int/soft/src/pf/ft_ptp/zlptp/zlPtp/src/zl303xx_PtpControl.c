

/*****************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Support functions to allow local control of the running PTP task
*     All PTP actions occur in a single thread and this file contains functions
*     to pass asynchronous messages into that thread to control its behaviour.
*     These operations would normally be performed from the command line
*
*******************************************************************************/

/****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"
#include "zl303xx_ErrTrace.h"

#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpControl.h"
#include "zl303xx_PtpInternalTypes.h"
#include "zl303xx_PtpForeignMasterTbl.h"
#include "zl303xx_PtpDeprecated.h"

#ifdef __VXWORKS_54
#include "zl303xx_OsHeaders.h"
#endif
#if defined OS_LINUX || defined OS_FREERTOS
#include <time.h>
#endif

/****************   # DEFINES   **********************************************/

/* Maximum buffer Size. */
/* Currently the largest Control Message is for the ParentDS. */
#define PTP_MSGQ_MAX_MSG_LEN_WORDS (4 + sizeof(zl303xx_ParentDS)/4 + 1)

/****************   TYPEDEFS    **********************************************/

/* A Structure for sending local Control Messages to a PTP Object. */
typedef struct
{
   Uint32T clockStreamId;  /* The clock port to which to send this command */
   Uint32T buffer[PTP_MSGQ_MAX_MSG_LEN_WORDS];
   Uint32T length;      /* The length in 32-bit words */
} zl303xx_PtpCtrlMsgS;

/****************   GLOBAL VARIABLES   ****************************************/

extern zl303xx_ClockStreamS *Zl303xx_PtpStreamArray[ZL303XX_PTP_NUM_STREAMS_MAX];

/******************   STATIC FUNCTION DEFINITIONS   ***************************/

static zlStatusE sendLocalControlMessage(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpCtrlMsgS *msg);

/****************   EXPORTED FUNCTION DEFINITIONS   ***************************/

/*

  Function Name:
   handleLocalCtrlMessages

  Details:
   Reads the local control message queue and processes any pending messages

  Parameters:
       pPtpStream  Pointer to the clock stream data structure for this message
       ptpMsg      A received PTP control message

  Return Value:
   Nothing

*******************************************************************************/
void handleLocalCtrlMessages(zl303xx_ClockStreamS *pStream,
                               zl303xx_PtpClockMsgQueueS *pMsg)
{
   Uint32T ctrlMsg[PTP_MSGQ_MAX_MSG_LEN_WORDS];

   /* Copy the control words from the message buffer. */
   OS_MEMCPY(ctrlMsg, pMsg->msgBuf, sizeof(ctrlMsg));

   if (pStream == NULL)
   {
      /* If the clock stream is not specified then we assume we cannot continue
       * to process this request. The only exceptions to this is setting a new
       * time on a PTP clock or updating the parent data set. */
      if ((ctrlMsg[0] != CTRL_MSG_SET_PARENT_DATA) &&
          (ctrlMsg[0] != CTRL_MSG_SET_CLOCK_PARAM))
      {
         ZL303XX_ERROR_NOTIFY("handleLocalCtrlMessages(): NULL clock stream");
         return;
      }
   }

   switch (ctrlMsg[0])
   {
      case CTRL_MSG_SET_PARENT_DATA:
      {
         zlStatusE status = ZL303XX_OK;

         zl303xx_PtpClockS *pPtpClock = NULL;
         zl303xx_PortDataS *pPtpPort = NULL;
         zl303xx_ClockStreamS *pServerStream = NULL;    /* Stream associated with this GM Data. */

         /* Local copy of the Message parameters Data Set. */
         zl303xx_ParentDS parentDS;
         zl303xx_PtpClockHandleT clockHandle = ctrlMsg[1];
         Uint16T rxPortNumber = ctrlMsg[2];

         status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);

         if (status == ZL303XX_OK)
         {
            OS_MEMCPY(&parentDS, &ctrlMsg[4], sizeof(zl303xx_ParentDS));

            if (rxPortNumber == 0)
            {
               /* This may indicate that the ParentDS is the local DefaultDS
                * OR a local Virtual Port is providing the ParentDS.    */
               if (pPtpClock->gmSource.extComparisonData.receivingPortIdentity.portNumber != 0)
               {
                  /* This VP may not be local (in the case of distributed
                   * architectures) but check anyway. */
                  (void)zl303xx_PtpPortDataFromPortNumber(
                              pPtpClock,
                              pPtpClock->gmSource.extComparisonData.receivingPortIdentity.portNumber,
                              &pPtpClock->gmSource.pPtpPort);

                  /* No matter what, update the location flag. */
                  pPtpClock->gmSource.location = ZL303XX_PTP_GM_SRC_LOCAL_PORT;
                  pPtpClock->gmSource.pPtpStream = NULL;
               }
               else
               {
                  /* The local clock is the best Server so far. */
                  /* Update the clock's dynamic parameters. */
                  pPtpClock->gmSource.location = ZL303XX_PTP_GM_SRC_LOCAL_CLOCK;
                  pPtpClock->gmSource.pPtpStream = NULL;
               }

               /* Clear any existing active Path Trace information. */
               if (pPtpClock->pathTraceList.listLength > 0)
               {
                  status = zl303xx_PtpClockPathTraceListUpdate(&pPtpClock->pathTraceList, 0, NULL);
               }
            }

            else if (rxPortNumber == (Uint16T)ZL303XX_PTP_INVALID)
            {
               /* Do not overwrite the ParentDS if this a MASTER_ONLY clock. */
               if (pPtpClock->config.defaultDS.clockQuality.clockClass <= PTP_MASTER_ONLY_CLOCK_CLASS_MAX)
               {
                  zl303xx_PtpConvertDefaultDsToParentDs(&pPtpClock->config.defaultDS, &parentDS);
               }
               else
               {
                  /* Check the provided data set for potential errors.  */
                  status = zl303xx_PtpClockCheckParentDS(pPtpClock, &parentDS);
               }

               if (status == ZL303XX_OK)
               {
                  /* An external Port is the best Server. */
                  pPtpClock->gmSource.location = ZL303XX_PTP_GM_SRC_EXTERNAL;
                  pPtpClock->gmSource.pPtpStream = NULL;
                  pPtpClock->gmSource.pPtpPort = NULL;

                  /* Copy the External Path Trace information to the dynamic clock list. */
                  if (pPtpClock->config.pathTrace.enabled == ZL303XX_TRUE)
                  {
                     status = zl303xx_PtpClockPathTraceListUpdate(
                                    &pPtpClock->pathTraceList,
                                    pPtpClock->gmSource.extPathTraceList.listLength,
                                    pPtpClock->gmSource.extPathTraceList.pClockIdList);
                  }
               }
            }

            else
            {
               /* A local portNumber with a foreign ParentDS. */
               /* Make sure the port still exists. */
               if ((status = zl303xx_PtpPortDataFromPortNumber(pPtpClock, rxPortNumber, &pPtpPort)) != ZL303XX_OK)
               {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 1,
                        "SetClockParentDS: rxPortNumber (%d) no longer exists.",
                        rxPortNumber, 0,0,0,0,0);
               }
               else
               {
                  /* Do not overwrite the ParentDS if this a MASTER_ONLY clock. */
                  if (pPtpClock->config.defaultDS.clockQuality.clockClass <= PTP_MASTER_ONLY_CLOCK_CLASS_MAX)
                  {
                     zl303xx_PtpConvertDefaultDsToParentDs(&pPtpClock->config.defaultDS, &parentDS);
                  }
                  else
                  {
                     /* Search the entire clock for the local stream associated with this GM data. */
                     pServerStream = zl303xx_PtpPortFindStreamForParentDS(pPtpPort, &parentDS);

                     if ((pServerStream == NULL) ||
                         (rxPortNumber != pServerStream->portData->config.portNumber))
                     {
                        ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 1,
                              "SetClockParentDS: rxPortNumber (%d) does not match receiving Stream (%d)",
                              rxPortNumber,
                              pServerStream ? pServerStream->portData->config.portNumber : 0, 0,0,0,0);

                        status = ZL303XX_ERROR;
                     }
                     else
                     {
                        /* Check the provided data set for potential errors.  */
                        status = zl303xx_PtpClockCheckParentDS(pPtpClock, &parentDS);
                     }

                     if (status == ZL303XX_OK)
                     {
                        /* A local Port is the best Server. */
                        pPtpClock->gmSource.location = ZL303XX_PTP_GM_SRC_LOCAL_STREAM;
                        pPtpClock->gmSource.pPtpStream = pServerStream;
                        pPtpClock->gmSource.pPtpPort = pServerStream->portData;

                        /* Get the Path Trace Data for this stream from the FMT
                         * (recorded when the ANNOUNCE packet arrived. */
                        if (pPtpClock->config.pathTrace.enabled == ZL303XX_TRUE)
                        {
                           status = zl303xx_PtpFmtPathTraceToActive(
                                 &pPtpClock->foreignMasterDS,
                                 pServerStream,
                                 &parentDS.parentPortIdentity);
                        }
                     }
                  }
               }
            }
         }
         else
         {
             ZL303XX_TRACE_ALWAYS("handleLocalCtrlMessages: zl303xx_PtpClockDataGet() failed with error = %d ",
                   status, 0,0,0,0,0);
         }

         if (status == ZL303XX_OK)
         {
            /* Pass the data to the core Parent Data Set update routine. */
            status = zl303xx_PtpClockConfigureParentDataSet(pPtpClock, &parentDS, ZL303XX_FALSE);
         }

         break;
      }

      case CTRL_MSG_SET_CLOCK_PARAM:
      {
         zlStatusE status = ZL303XX_OK;
         zl303xx_PtpClockS *pPtpClock;

         if (status == ZL303XX_OK)
         {
            /* Clock handle stored in clockStreamId by zl303xx_PtpCtrlClockParamSet() */
            status = zl303xx_PtpClockDataGet(pMsg->clockStreamId, &pPtpClock);
         }

         if (status == ZL303XX_OK)
         {
            /* ctrlMsg[1] = param type, ctrlMsg[2] = data */
            zl303xx_PtpClockParamSet(pPtpClock, ctrlMsg[1], &ctrlMsg[2]);
         }

         break;
      }

      case CTRL_MSG_SET_KEEP_ALIVE_DURATION :
      {
         /* Only update if it's different (API should have verified). */
         if (ctrlMsg[1] != pStream->config.keepAliveSec)
         {
            pStream->config.keepAliveSec = ctrlMsg[1];

            /* If the count has been changed, reset the remaining count to the beginning.
             * (It's possible the new value is larger or smaller than original which
             * would require some math to keep an even window. Resetting is simpler). */
            pStream->keepAliveRemSec = pStream->config.keepAliveSec;
         }

         break;
      }

      default:
      {
         /* Unrecognised message */
         ZL303XX_ERROR_NOTIFY("Unrecognised local control message");
         break;
      }
   }
}

/*

  Function Name:
   zl303xx_SetCurrentPTPTime

  Details:
   Sends a command to set the current time of the PTP clock

  Parameters:
   [in]  clockPortHandle   Index into the port data array
   [in]  secondsLo    The number of seconds to set the clock relative to the PTP epoch
                        (lowest 32-bits)
   [in]  secondsHi    The highest 16-bits of the number of seconds to set the clock

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetCurrentPTPTime(zl303xx_PtpPortHandleT portHandle,
                                    Uint32T secondsLo, Uint32T secondsHi)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_PortDataS *port = NULL;
   zl303xx_TimeStamp tod;

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &port);
   }

   if (status == ZL303XX_OK)
   {
      /* Truncate secondsField to 48 bits */
      tod.secondsField.hi = secondsHi & 0xFFFF;
      tod.secondsField.lo = secondsLo;
      tod.nanosecondsField = 0;

      mStatus = zl303xx_PtpClockMutexLock(port->clock);
      status = mStatus;
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockTimeSet(port->clock, &tod);
   }

   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(port->clock, status);
   }

   return status;
}

/**

  Function Name:
   zl303xx_SetCurrentUTCTime

  Details:
   Sets the current time of a PTP clock to the specified UTC time

  Parameters:
   [in]  clockPortHandle  Index into the port data array
   [in]  dateTimeStr A date and time string in ISO-8601 format.
                  The specific format supported is "YYYY-MM-DD hh:mm:ss"

  Return Value:
   zlStatusE

  Notes:
   Does not support the setting of any seconds values above 32-bits

*******************************************************************************/
zlStatusE zl303xx_SetCurrentUTCTime(zl303xx_PtpPortHandleT clockPortHandle, char *dateTimeStr)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *portData = NULL;
   struct tm tm_time;
   time_t secs;
   Sint32T conversions;

   OS_MEMSET(&tm_time, 0, sizeof(struct tm));
   OS_MEMSET(&secs, 0, sizeof(time_t));

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      /* Returns an error if the port data is NULL */
      status = zl303xx_PtpPortDataGet(clockPortHandle, &portData);
   }

   if (status == ZL303XX_OK)
   {
      conversions = sscanf(dateTimeStr, "%4d-%2d-%2d %2d:%2d:%2d",
                  &tm_time.tm_year, &tm_time.tm_mon, &tm_time.tm_mday,
                  &tm_time.tm_hour, &tm_time.tm_min, &tm_time.tm_sec);

      if (conversions != 6)
      {
         ZL303XX_ERROR_NOTIFY("Invalid date/time format.\nUse format \"YYYY-MM-DD hh:mm:ss\"");
         ZL303XX_ERROR_NOTIFY("Example: 2006-04-28 13:04:59");
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      /* Make adjustments into the format required by mktime */
      tm_time.tm_year -= 1900;  /* Years since 1900 */
      tm_time.tm_mon -= 1;      /* Months start from 0 */

      /* Convert to UTC seconds value */
      secs = mktime(&tm_time);

      if (secs == (time_t)-1)
      {
         ZL303XX_ERROR_NOTIFY("Cannot convert date/time");
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      /* Adjust for leap seconds which are included in UTC but not PTP time.
         UTC time is behind PTP time */
      if (portData->portDS.versionNumber == ZL303XX_PTP_VERSION_1)
      {
         secs += UTC_OFFSET;    /* Add leap seconds offset */
      }
      else
      {  /* For version 2 the UTC offset may have been configured either manually, or may
            have been obtained from a remote clock. So use that value if there is one */
         if (portData->clock->timePropertiesDS.currentUtcOffsetValid == ZL303XX_TRUE)
         {
            secs += portData->clock->timePropertiesDS.currentUtcOffset;    /* Add leap seconds offset */
         }
         else
         {  /* Since the UTC is not configured use the compiled in value */
            secs += UTC_OFFSET;    /* Add leap seconds offset */
         }
      }

      /* Always set the high seconds value to 0, this will limit the usability of this system in 136 years */
      status = zl303xx_SetCurrentPTPTime(clockPortHandle, (Uint32T) secs, 0);
   }

   return status;
 }

/*

  Function Name:
   zl303xx_EnableTsPacketTransmit

  Details:
   Sends a command to allow/disallow the PTP clock to transmit timestamped packets

  Parameters:
       clockStreamHandle  The clock stream to which to send this command or -1 for all clock streams
       enable   1 to enable, 0 to disable the transmission of timestamped packets

  Return Value:
   zlStatusE)

  Notes:
   Only affects the transmission of timestamped packets (not all packets)

  (Deprecated)  This function no longer does anything.

*******************************************************************************/
zlStatusE zl303xx_EnableTsPacketTransmit(zl303xx_PtpStreamHandleT clockStreamHandle, zl303xx_BooleanE enable)
{
   (void)clockStreamHandle;
   (void)enable;

   ZL303XX_TRACE_ALWAYS("zl303xx_EnableTsPacketTransmit: This function is deprecated.",
                        0,0,0,0,0,0);
   return ZL303XX_OK;
 }

/* zl303xx_SetSyncSecondsPerPacket */
/**
   Set the Sync interval for 1 or more seconds per packet. Only powers of 2 are
   supported (1, 2, 4, 8, etc.)

  (Deprecated)  This function has been replaced by zl303xx_PtpStreamLogSyncIntvlSet().

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   secsPerPkt    The number of seconds between Sync packets.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetSyncSecondsPerPacket(zl303xx_PtpStreamHandleT streamHandle,
                                          Sint16T secsPerPkt)
{
   zlStatusE status = ZL303XX_OK;

   /* Check it's within a sensible range. Greater than 0 and up to 1024 seconds
    * per packet */
   if (status == ZL303XX_OK)
   {
      if (secsPerPkt <= 0)
      {
         ZL303XX_ERROR_NOTIFY("Seconds per packet must be >= 1");
         status = ZL303XX_PARAMETER_INVALID;
      }
      else if (secsPerPkt > 1024)
      {
         ZL303XX_ERROR_NOTIFY("Seconds per packet must be <= 1024");
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamLogSyncIntvlSet(
                     streamHandle, log2SecondsPerPacket(secsPerPkt));
   }

   return status;
 }

/* zl303xx_SetSyncPacketsPerSecond */
/**
   Set the Sync interval for 1 or more packets per second. Only powers of 2 are
   supported (1, 2, 4, 8, etc.)

  (Deprecated)  This function has been replaced by zl303xx_PtpStreamLogSyncIntvlSet().

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   pktsPerSec    The number of Sync packets to send per second.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetSyncPacketsPerSecond(zl303xx_PtpStreamHandleT streamHandle,
                                          Sint16T pktsPerSec)
{
   zlStatusE status = ZL303XX_OK;

   /* Check it's within a sensible range. Greater than 0 and up to 1024 packets
    * per second */
   if (status == ZL303XX_OK)
   {
      if (pktsPerSec <= 0)
      {
         ZL303XX_ERROR_NOTIFY("Packets per second must be >= 1 per sec");
         status = ZL303XX_PARAMETER_INVALID;
      }
      else if (pktsPerSec > 1024)
      {
         ZL303XX_ERROR_NOTIFY("Packets per second must be <= 1024 per sec");
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamLogSyncIntvlSet(
                     streamHandle, log2PacketsPerSecond(pktsPerSec));
   }

   return status;
 }


/* zl303xx_SetDelayReqSecondsPerPacket */
/**
   Set the Delay_Req interval for 1 or more seconds per packet. Only powers of
   2 are supported (1, 2, 4, 8, etc.)

  (Deprecated)  This function has been replaced by zl303xx_PtpStreamLogDelayIntvlSet().

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   secsPerPkt    The number of seconds between Delay_Req packets.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetDelayReqSecondsPerPacket(zl303xx_PtpStreamHandleT streamHandle,
                                              Sint16T secsPerPkt)
{
   zlStatusE status = ZL303XX_OK;

   /* Check it's within a sensible range. Greater than 0 and up to 1024 seconds
    * per packet */
   if (status == ZL303XX_OK)
   {
      if (secsPerPkt <= 0)
      {
         ZL303XX_ERROR_NOTIFY("Seconds per packet must be >= 1");
         status = ZL303XX_PARAMETER_INVALID;
      }
      else if (secsPerPkt > 1024)
      {
         ZL303XX_ERROR_NOTIFY("Seconds per packet must be <= 1024");
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamLogDelayIntvlSet(
                     streamHandle, log2SecondsPerPacket(secsPerPkt));
   }

   return status;
}

/* zl303xx_SetDelayReqPacketsPerSecond */
/**
   Set the Delay_Req interval for 1 or more packets per second. Only powers of
   2 are supported (1, 2, 4, 8, etc.)

  (Deprecated)  This function has been replaced by zl303xx_PtpStreamLogDelayIntvlSet().

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   pktsPerSec    The number of Delay_Req packets to send per second.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetDelayReqPacketsPerSecond(zl303xx_PtpStreamHandleT streamHandle,
                                              Sint16T pktsPerSec)
{
   zlStatusE status = ZL303XX_OK;

   /* Check it's within a sensible range. Greater than 0 and up to 1024 packets
    * per second */
   if (status == ZL303XX_OK)
   {
      if (pktsPerSec <= 0)
      {
         ZL303XX_ERROR_NOTIFY("Packets per second must be >= 1 per sec");
         status = ZL303XX_PARAMETER_INVALID;
      }
      else if (pktsPerSec > 1024)
      {
         ZL303XX_ERROR_NOTIFY("Packets per second must be <= 1024 per sec");
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamLogDelayIntvlSet(
                     streamHandle, log2PacketsPerSecond(pktsPerSec));
   }

   return status;
}

/*

  Function Name:
      zl303xx_ForcePtpMode

  Details:
   Forces this clock into a particular mode

  Parameters:
       clockStreamHandle  The clock stream to which to send this command or -1 for all
               clock streams
       mode        The mode of the clock
               Values: slave only                    (mode = 1),
                       master only                   (mode = 2),
                       automatic mode based on BMC   (mode = 0) (default)

  Return Value:
   zlStatusE

  (Deprecated)  This function no longer does anything.

*******************************************************************************/
zlStatusE zl303xx_ForcePtpMode(zl303xx_PtpStreamHandleT clockStreamHandle, zl303xx_PtpStreamModeE mode)
{
   (void)clockStreamHandle;
   (void)mode;

   ZL303XX_TRACE_ALWAYS("zl303xx_ForcePtpMode: This function is deprecated.",
                        0,0,0,0,0,0);

   return ZL303XX_OK;
}

/*

  Function Name:
      zl303xx_SetClockParentData

  Details:
   Sends a message to the specified clock to set the ParentDS of the clock.

  Parameters:
   [in]  clockHandle    The PTP Clock to which to send this command.
   [in]  pParentDS      The Parent Data Set to apply on the specified clock.
   [in]  rxPortNumber   The PTP Port on which the Parent Data Set was received:
                              -  0: The DefaultDS of the clock.
                              - -1: An external Port of another node.
                              - ELSE: A local Port on this clock.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetClockParentData(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_ParentDS *pParentDS,
      Uint16T rxPortNumber)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpCtrlMsgS msg;

   zl303xx_PtpClockS *pPtpClock = NULL;
   zl303xx_PortDataS *pPtpPort = NULL;

   OS_MEMSET(&msg, 0, sizeof(zl303xx_PtpCtrlMsgS));

   if (zl303xx_PtpClockDataGet(clockHandle, &pPtpClock) != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
            "zl303xx_SetClockParentData: zl303xx_PtpPortDataGet() failed with error = %d ",
             status, 0,0,0,0,0);
      status = ZL303XX_PARAMETER_INVALID;
      return status;
   }

   /* Assemble the message. A command byte and a time value */
   msg.buffer[0] = CTRL_MSG_SET_PARENT_DATA;
   msg.buffer[1] = clockHandle;
   msg.buffer[2] = rxPortNumber;
   msg.clockStreamId = (Uint32T)ZL303XX_PTP_INVALID;

   /* Get the first valid port on this clock. */
   /* This is so the Ptpd will not fail.
    * CAN BE REMOVED if the Ptpd is ever cleaned up. */
   if (zl303xx_ListEmpty(&pPtpClock->portList) == ZL303XX_TRUE)
   {
      /* This Clock has no Ports so cannot continue. */
       ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
             "zl303xx_SetClockParentData: Clock with no ports ",
             0,0,0,0,0,0);
      status = ZL303XX_PARAMETER_INVALID;
      return status;
   }
   else
   {
      /* Get the first port handle associated with this Clock. */
      ZL303XX_PTP_FOR_EACH_PORT(&pPtpClock->portList, pPtpPort)
      {
         msg.buffer[3] = pPtpPort->clockPortHandle;
         break;
      }
   }

   OS_MEMCPY(&msg.buffer[4], pParentDS, sizeof(zl303xx_ParentDS));
   msg.length = 4 + sizeof(zl303xx_ParentDS)/4 + 1;

   if (sendLocalControlMessage(clockHandle, &msg) != ZL303XX_OK)
   {
      ZL303XX_ERROR_NOTIFY("sendLocalControlMessage() failed.");
   }

   return status;
}

/* zl303xx_PtpCtrlClockParamSet */
/** 

   Sends a message to the clock task to set a clock parameter. Setting a
   parameter inside the context of a PTP task removes the need for the calling
   task to take the PTP mutex.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   param        The clock parameter to set.
   [in]   data         Data value to set (must be <= 4 bytes).

*******************************************************************************/
zlStatusE zl303xx_PtpCtrlClockParamSet(zl303xx_PtpClockHandleT clockHandle,
                                       zl303xx_PtpClockParamE param,
                                       const void *data)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpCtrlMsgS msg;
   Uint32T len;

   if (status == ZL303XX_OK)
   {
      OS_MEMSET(&msg, 0, sizeof(msg));

      msg.clockStreamId = clockHandle;
      msg.buffer[0] = CTRL_MSG_SET_CLOCK_PARAM;
      msg.buffer[1] = param;

      len = zl303xx_PtpClockParamPack(param, data, (Uint8T *)&msg.buffer[2]);
      if (len == 0 || len > 4)
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      msg.length = 3;
      status = sendLocalControlMessage(clockHandle, &msg);
   }

   return status;
}


/* zl303xx_PtpStreamKeepAliveDurationSet */
/**
   Sets the duration, in seconds, for the Keep-Alive Event.

  Parameters:
   [in]   streamHandle   Handle to a previously created stream.
   [in]   keepAliveSec   Keep-Alive event duration (in Seconds). Setting a
                               value of '0' disables the Event.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_PtpStreamKeepAliveDurationSet(
      zl303xx_PtpStreamHandleT streamHandle,
      Uint32T keepAliveSec)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pStream = NULL;
   zl303xx_PtpCtrlMsgS msg;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pStream);
   }

   /* Assemble the control message and send it to PTP. */
   if (status == ZL303XX_OK)
   {
      OS_MEMSET(&msg, 0, sizeof(msg));

      msg.clockStreamId = streamHandle;
      msg.buffer[0] = CTRL_MSG_SET_KEEP_ALIVE_DURATION;
      msg.buffer[1] = keepAliveSec;
      msg.length = 2;

      status = sendLocalControlMessage(pStream->portData->clock->clockHandle, &msg);
   }

   return status;
}

/****************   STATIC FUNCTION DEFINITIONS   ****************************/

/*

  Function Name:
   sendLocalControlMessage

  Details:
   Sends a command to the local control queue

  Parameters:
       msg      The message to send
       length   The length of the message in 32-bit words

  Return Value:
   Nothing

*******************************************************************************/
static zlStatusE sendLocalControlMessage(zl303xx_PtpClockHandleT clockHandle,
                                           zl303xx_PtpCtrlMsgS *msg)
{
   zlStatusE status = ZL303XX_OK;

   if (msg->length > PTP_MSGQ_MAX_MSG_LEN_WORDS)
   {
      ZL303XX_TRACE_ALWAYS(
            "SEND local control message length ERROR: len(%d):max(%d)",
            msg->length, PTP_MSGQ_MAX_MSG_LEN_WORDS, 0,0,0,0);
      status = ZL303XX_DATA_CORRUPTION;
   }


   else if ((status = zl303xx_PtpSendControlMsg(
                    clockHandle,
                    msg->clockStreamId,
                    (Uint8T *)msg->buffer,
                    msg->length * sizeof(Uint32T))) != ZL303XX_OK)
   {
      ZL303XX_ERROR_NOTIFY("Problem sending local control message");
   }

   return status;
}
