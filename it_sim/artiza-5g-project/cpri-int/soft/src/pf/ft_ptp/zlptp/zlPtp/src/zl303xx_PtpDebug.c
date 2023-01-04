

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Debugging functions for PTP
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Error.h"
#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpDebug.h"

/*****************   # DEFINES   **********************************************/

/*****************   TYPEDEFS    **********************************************/

/*****************   GLOBAL VARIABLES   ****************************************/

/*******************   STATIC FUNCTION DEFINITIONS   ***************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   ***************************/

/*******************************************************************************

  Function Name:   setCurrentUTCTime

  Details:  Sets the current time of the PTP clock to the specified UTC time

  Parameters:
   [in]  dateTimeStr A date and time string in ISO-8601 format.
            The specific format supported is "YYYY-MM-DD hh:mm:ss"

  Return Value:  zero if OK or non-zero if an error occurs

  Notes: Assumes that PTP clock ID 0 is active

*******************************************************************************/
Sint32T setCurrentUTCTime(char *dateTimeStr)
{
   return zl303xx_SetCurrentUTCTime(0, dateTimeStr);
}

/*******************************************************************************

  Function Name:  forcePacketTransmission

  Details:  Forces specified stream to begin sending messages of the type specified.

  Parameters:
   [in]  streamHandle   Stream Handle of the stream to update the message transmission for.
   [in]   messageType    The message type to affect.
   [in]   enable         Flag to start or stop the transmission.

  Notes:  zero (just so a defined value will be displayed by the shell)

*******************************************************************************/
Sint32T forcePacketTransmission(Uint32T streamHandle,
                                Uint32T messageType,
                                zl303xx_BooleanE enable)
{
   zlStatusE status = ZL303XX_OK;

   zl303xx_ClockStreamS *stream = NULL;

   const char *Zl303xx_MsgTypeStr[] =
   {
      "ANNOUNCE_RECEIPT_TIMER",
      "SYNC_INTERVAL_TIMER",
      "ANNOUNCE_INTERVAL_TIMER",
      "QUALIFICATION_TIMER",
      "PRE_MASTER_TIMER",
      "UNCALIBRATED_TIMER"
      "DELAY_REQ_TIMER",
      "PEER_DELAY_REQ_TIMER",
      "unused",
      "ONE_SEC_TIMER",
      "MAX_TIMER_SIZE"
   };

   if ((messageType > ONE_SEC_TIMER) ||
       (messageType == SYNC_BURST_DELAY_TIMER))
   {
      printf("Invalid or Unused Message Type: (%u)\n", (UINT_T)messageType);
      status = ZL303XX_PARAMETER_INVALID;
   }
   else if (enable & 0xFFFFFFFE)
   {
      printf("Invalid ENABLE flag: (%d)\n", enable);
      status = ZL303XX_PARAMETER_INVALID;
   }

   /* Get a pointer to the stream structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &stream);
   }

   if (status == ZL303XX_OK)
   {
      /* If already in the desired state, print a message). */
      if (zl303xx_PtpTimerActive(&stream->itimer[messageType]) == enable)
      {
         printf("Message Type %d:(%s) already %s\n",
                 (SINT_T)messageType,
                 Zl303xx_MsgTypeStr[messageType],
                 (enable == 1 ? "Enabled" : "Disabled"));
      }

      if (enable == ZL303XX_FALSE)
      {
         printf("Disabling Message Type %d:(%s) for stream %d\n",
                  (SINT_T)messageType, Zl303xx_MsgTypeStr[messageType],
                  (SINT_T)stream->clockStreamHandle);

         zl303xx_PtpTimerStop(&stream->itimer[messageType]);
      }
      else
      {
         /* Default for 1 second timer. */
         zl303xx_TimeStamp timeInterval = {{0, 1}, 0};
         zl303xx_V2UnicastNegotiatedMsgTypeE unicastMessageType = PTP_NEGOTIATED_MSG_ID_INVALID;

         /* Determine the packet rate. */
         switch (messageType)
         {
            case ANNOUNCE_RECEIPT_TIMER :
            {
               zl303xx_PtpGetAnnounceReceiptTimeout(stream, &timeInterval);
               break;
            }

            case SYNC_INTERVAL_TIMER :
            {
               zl303xx_PtpGetSyncIntervalTimeout(stream, &timeInterval);
               unicastMessageType = PTP_NEGOTIATED_MSG_ID_SYNC; /* 0 */
               break;
            }

            case ANNOUNCE_INTERVAL_TIMER :
            {
               zl303xx_PtpGetAnnounceIntervalTimeout(stream, &timeInterval);
               unicastMessageType = PTP_NEGOTIATED_MSG_ID_ANNOUNCE; /* 1 */
               break;
            }

            case QUALIFICATION_TIMER :
            {
               zl303xx_PtpGetQualificationTimeout(stream, &timeInterval);
               break;
            }

            case PRE_MASTER_TIMER :
            {
               zl303xx_PtpGetPreMasterTimeout(stream, &timeInterval);
               break;
            }

            case UNCALIBRATED_TIMER :
            {
               zl303xx_PtpGetUncalibratedTimeout(stream, &timeInterval);
               break;
            }

            case DELAY_REQ_TIMER :
            {
               zl303xx_PtpGetDelayReqIntervalTimeout(stream, &timeInterval);
               unicastMessageType = PTP_NEGOTIATED_MSG_ID_DELAY_RESP; /* 2 */
               break;
            }

            case PEER_DELAY_REQ_TIMER :
            {
               zl303xx_PtpGetPeerDelayReqIntervalTimeout(stream, &timeInterval);
               unicastMessageType = PTP_NEGOTIATED_MSG_ID_PDELAY_RESP; /* 3 */
               break;
            }

            case ONE_SEC_TIMER :
            default:
            {
               /* Default for 1-Second timer is set above. */
               /* Can never be an error at this point because we checked messageType above. */
               break;
            }
         }

         if (unicastMessageType != (Uint32T)(-1))
         {
            printf("Enabling Negotiated Unicast message flow: Type :(%s) for stream %d:\n",
                     Zl303xx_MsgTypeStr[messageType], (SINT_T)stream->clockStreamHandle);
         }

         printf("Enabling Message Type %d:(%s) for stream %d: Interval %u:%u\n",
                  (SINT_T)messageType, Zl303xx_MsgTypeStr[messageType],
                  (SINT_T)stream->clockStreamHandle,
                  (SINT_T)timeInterval.secondsField.lo, (SINT_T)timeInterval.nanosecondsField);

         /* Start the requested timer. */
         (void)zl303xx_PtpStreamTimerStart(stream, messageType);
      }
   }

   return (Sint32T)status;
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

