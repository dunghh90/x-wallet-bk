

/*****************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Implementation of an event queue for the PTP engine
*
******************************************************************************/

/****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"
#include "zl303xx_ErrTrace.h"

#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpEventHandling.h"

/****************   DEFINES     **********************************************/

/****************   DATA TYPES   *********************************************/

/****************   DATA STRUCTURES   ****************************************/

/****************   STATIC FUNCTION DECLARATIONS   ***************************/
static zlStatusE zl303xx_PtpRxMsgDataCheck(Uint8T *buffer, Uint16T bufferLen,
                                           void *pMsgData);

/****************   STATIC GLOBAL VARIABLES   ********************************/

/****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   IMPORTED GLOBAL VARIABLES   ******************************/

/****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* zl303xx_PtpSendTimerTick */
/** 

   Creates an message to indicate a timer event and posts it into a clock's
   message queue.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   secondFlag   TRUE if this message corresponds to an (arbitrary)
                            seconds boundary.

*******************************************************************************/
zlStatusE zl303xx_PtpSendTimerTick(zl303xx_PtpClockS *pClock, zl303xx_BooleanE secondFlag)
{
   zl303xx_PtpTimerTickMsgS msg;
/*   fputs("zl303xx_PtpSendTimerMsg: ", stderr);*/ /* printf is non-reentrant - we are in an isr handler */

   /* Set the message type. */
   msg.type = PTP_TASK_MSG_TIMER_TICK;
   msg.secondFlag = secondFlag;

   return zl303xx_PtpClockMsgQSend(pClock, &msg, sizeof(msg));
}

/*

  Function Name:
   zl303xx_PtpSendControlMsg

  Details:
   Creates an event message for a control event and posts it into the event queue

  Parameters:
       clockStreamId     The PTP clock stream for which data has become available
       ctrlMsg           Pointer to buffer containing the packet
       ctrlMsgLength     The length of the packet

  Return Value:
   zlStatusE

 *****************************************************************************/
zlStatusE zl303xx_PtpSendControlMsg(zl303xx_PtpClockHandleT clockHandle,
                                    Uint32T clockStreamId,
                                    Uint8T *ctrlMsg,
                                    Uint32T ctrlMsgLength)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockMsgQueueS msg;
   zl303xx_PtpClockS *pPtpClock = NULL;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3, "zl303xx_PtpSendControlMsg:", 0,0,0,0,0,0);

   /* Get a pointer to the clock data structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
      if (status != ZL303XX_OK)
      {
          ZL303XX_TRACE_ALWAYS("zl303xx_PtpSendControlMsg: zl303xx_PtpClockDataGet() for clockHandle=%d failed with error = %d",
                   clockHandle, status, 0,0,0,0);
      }
   }

   if (status == ZL303XX_OK)
   {
      /* Set the message type */
      msg.taskMsgId = PTP_TASK_MSG_CONTROL_CMD;
      msg.clockStreamId = clockStreamId;

      if (((Uint32T *)(void*)ctrlMsg)[0] == CTRL_MSG_SET_PARENT_DATA)
      {
         msg.recvMsgInfo.clockPortHandle = ((Uint32T *)(void*)ctrlMsg)[3];
      }

      /* Copy the message into the buffer (better if we didn't have to copy but it is
         only a very short message) */
      if (ctrlMsgLength > sizeof(msg.msgBuf))
      {
         ctrlMsgLength = sizeof(msg.msgBuf);
      }
      OS_MEMCPY(msg.msgBuf, ctrlMsg, ctrlMsgLength);
      msg.pktLength = ctrlMsgLength;

      /* Send the message into the queue */
      status = zl303xx_PtpClockMsgQSend(pPtpClock, &msg, sizeof(msg));
   }

   return status;
}

/* zl303xx_PtpTsRecordProcess */
/**
   Processes a time stamp record generated by a hardware device. For a SYNC
   message, this will cause the PTP protocol to generate and transmit a FOLLOW_UP
   message with the accurate time stamp in it. For a DELAY_REQ message, this
   will store the egress time stamp to be recombined with the DELAY_RESP time
   stamp when it arrives.

  Parameters:
   [in]   pTsRecord  Time stamp record data to process.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      pTsRecord is NULL.
     ZL303XX_PARAMETER_INVALID    streamHandle is not valid.
     ZL303XX_RTOS_MSGQ_INVALID    The clock's message queue has not been initialized yet.
     ZL303XX_RTOS_MSGQ_SEND_FAIL  Error sending message to the clock's queue.
                                      The queue may be full.

*******************************************************************************/
zlStatusE zl303xx_PtpTsRecordProcess(zl303xx_PtpTsRecordS *pTsRecord)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *stream;
   zl303xx_PtpTsRecordMsgS msg;

   ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 2, "zl303xx_PtpTsRecordProcess:", 0,0,0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 2, "zl303xx_PtpTsRecordProcess:", 0,0,0,0,0,0);

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pTsRecord);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(pTsRecord->streamHandle, &stream);
   }

   if (status == ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 3,
            "zl303xx_PtpTsRecordProcess: Stream (%u), MsgType (%u), SeqId (%u)",
            pTsRecord->streamHandle,
            pTsRecord->messageType,
            pTsRecord->sequenceId, 0,0,0);

      msg.type = PTP_TASK_MSG_TS_RECORD_READY;
      msg.record = *pTsRecord;

      status = zl303xx_PtpClockMsgQSend(stream->portData->clock, &msg, sizeof(msg));
   }

   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_PtpTsRecordProcess FAILED! (Status: %d)",
            status, 0,0,0,0,0);

      status = ZL303XX_OK;
   }

   return status;
}

/* zl303xx_PtpSendManualCancel */
/** 

   Sends a MANUAL_CANCEL message to the PTP clock task.

  Parameters:
   [in]   pPort    Port the CANCEL will be sent from.
   [in]   pStream  (Optional) Stream associated with pDest.
   [in]   msgType  The messageType being cancelled.
   [in]   pDest    Destination address of the Signaling message.

*******************************************************************************/
zlStatusE zl303xx_PtpSendManualCancel(zl303xx_PortDataS *pPort,
                                      zl303xx_ClockStreamS *pStream,
                                      zl303xx_MessageTypeE msgType,
                                      zl303xx_PortAddress *pDest)
{
   zl303xx_PtpManualCancelS msg;

   msg.type = PTP_TASK_MSG_MANUAL_CANCEL;
   msg.portHandle = pPort->clockPortHandle;
   msg.streamHandle = (pStream != NULL) ? pStream->clockStreamHandle
                                        : (zl303xx_PtpStreamHandleT)ZL303XX_PTP_INVALID;
   msg.msgType = msgType;
   msg.destAddr = *pDest;

   return zl303xx_PtpClockMsgQSend(pPort->clock, &msg, sizeof(msg));
}

/* zl303xx_PtpRxMsgProcess */
/**
   Handles a PTP received PTP message. The source address of the message is
   used to send the message to the proper PTP clock task for processing.

  Parameters:
   [in]   buffer     Byte-buffer with the PTP message in network order.
   [in]   bufferLen  Length of the buffer.
   [in]   pMsgData   Extra information about the message.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    buffer or pMsgData is NULL.
     ZL303XX_PARAMETER_INVALID  bufLen == 0.

*******************************************************************************/
zlStatusE zl303xx_PtpRxMsgProcess(Uint8T *buffer, Uint16T bufferLen,
                                  zl303xx_PtpRxMsgDataS *pMsgData)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockMsgQueueS msg;
   zl303xx_ListS *clockList = zl303xx_PtpNodeClockListGet();
   zl303xx_PtpClockS *currClock = NULL, *temp = NULL;
   zl303xx_PortDataS *currPort, *matchPort;
   zl303xx_BooleanE matchedOnce = ZL303XX_FALSE;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3, "zl303xx_PtpRxMsgProcess:", 0,0,0,0,0,0);

   OS_MEMSET(&msg, 0, sizeof(msg));

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpRxMsgDataCheck(buffer, bufferLen, pMsgData);
   }

   if (!zl303xx_ListValid(clockList) || zl303xx_ListEmpty(clockList))
   {
       status = ZL303XX_STREAM_NOT_IN_USE;
   }

   /* Find the PTP port this message is intended for. */
   if (status == ZL303XX_OK && zl303xx_ListValid(clockList))
   {
      Uint16T portRet = 0;

      ZL303XX_PTP_FOR_EACH_CLOCK_SAFE(clockList, currClock, temp)
      {
         matchPort = NULL;
         if (zl303xx_PtpClockMutexLock(currClock) == ZL303XX_OK)
         {
            ZL303XX_PTP_FOR_EACH_PORT(&currClock->portList, currPort)
            {
               /* Check if the destination address of this packet matches the source
                * address of the current port. */
               if ((portRet = zl303xx_PtpComparePortAddress(&currPort->config.localAddr,
                                                 &pMsgData->destAddr)) == 0)
               {
                  /* No stream-specific data to send to the PTP clock task. Unicast
                   * stream lookup happens inside the clock task context. */
                  msg.clockStreamId = (Uint32T)ZL303XX_PTP_INVALID;
                  matchPort = currPort;
                  break;
               }

               /* Packet destination address does not match port source address.
                * Check if this port has a multicast stream on it and the packet
                * is destined to that multicast group. */
               if ((currPort->mcastStream != NULL) &&
                   ((portRet = zl303xx_PtpComparePortAddress(&currPort->mcastStream->config.destAddr,
                                                  &pMsgData->destAddr)) == 0))
               {
                  /* Multicast stream-specific data to send to the PTP clock task. */
                  msg.clockStreamId = currPort->mcastStream->clockStreamHandle;
                  matchPort = currPort;
                  break;
               }
               else if ((currPort->pdelayStream != NULL) &&
                        ((portRet = zl303xx_PtpComparePortAddress(&currPort->pdelayStream->config.destAddr,
                                                  &pMsgData->destAddr)) == 0))
               {
                  /* Multicast PDELAY stream-specific data to send to the PTP clock task. */
                  msg.clockStreamId = currPort->pdelayStream->clockStreamHandle;
                  matchPort = currPort;
                  break;
               }
   
            }  /* Ports */


            if (matchPort != NULL)
            {
               matchedOnce = ZL303XX_TRUE;

               /* Don't process if clock RX is disabled. This check is not at the
                * top of the clock loop because so the "no port match" warning
                * below can be suppressed. */
               if (currClock->enableRxTx == ZL303XX_FALSE)
               {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5,
                        "Clock %u: Discarded RX message: clock messaging disabled",
                        currClock->clockHandle, 0,0,0,0,0);
               }

               else
               {
                  msg.recvMsgInfo.clockPortHandle = matchPort->clockPortHandle;

                  /* Copy the source address and RX time stamp into the message. */
                  msg.recvMsgInfo.srcAddr = pMsgData->srcAddr;
                  msg.recvMsgInfo.destAddr = pMsgData->destAddr;
                  msg.recvMsgInfo.rxTs    = pMsgData->rxTs;

                  /* Copy the external data buffer. */
                  OS_MEMCPY(msg.recvMsgInfo.extData, pMsgData->extData, sizeof(msg.recvMsgInfo.extData));

                  /* Copy the message into the buffer (better if we didn't have to copy but at least
                     it is only the first part of the message, i.e. we truncate it here
                     if it is a big packet) */
                  msg.pktLength = min(bufferLen, sizeof(msg.msgBuf));
                  OS_MEMCPY(msg.msgBuf, buffer, msg.pktLength);

                  /* Set the message type */
                  msg.taskMsgId = PTP_TASK_MSG_RECV_PKT;

                  /* Send this message to the PTP clock task. */
                  status = zl303xx_PtpClockMsgQSend(matchPort->clock, &msg, sizeof(msg));
               }
            }

            /* Release the MUTEX. */
            (void)zl303xx_PtpClockMutexUnlock(currClock, status);
         }
      }  /* Clocks */

      if (matchedOnce == ZL303XX_FALSE)
      {
         /* The destination does not match anything on this clock. */
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpRxMsgProcess: (ERROR=%d ) No port matching destAddr=0x%08lX found.",
               portRet,  *(Uint32T *)(void*)pMsgData->destAddr.addressField, 0,0,0,0);
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
               "zl303xx_PtpRxMsgProcess: (ERROR=%d) No port matching destAddr=0x%08lX found.",
               portRet, *(Uint32T *)(void*)pMsgData->destAddr.addressField, 0,0,0,0);
      }
   }

   return status;
}

/* zl303xx_PtpPortRxMsgProcess */
/**
   Handles a PTP received PTP message. The port handle of the message is
   used to send the message to the proper PTP clock task for processing. This
   function can only be used to handle unicast messages.

  Parameters:
   [in]   buffer     Byte-buffer with the PTP message in network order.
   [in]   bufferLen  Length of the buffer.
   [in]   pMsgData   Extra information about the message.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    buffer or pMsgData is NULL.
     ZL303XX_INVALID_PARAMETER  bufLen == 0.

*******************************************************************************/
zlStatusE zl303xx_PtpPortRxMsgProcess(Uint8T *buffer, Uint16T bufferLen,
                                      zl303xx_PtpPortRxMsgDataS *pMsgData)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtpPortRxMsgProcess";
   zl303xx_PtpClockMsgQueueS msg;
   zl303xx_PortDataS *pPort;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4, "%s:", fnName, 0,0,0,0,0);

   OS_MEMSET(&msg, 0, sizeof(msg));

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpRxMsgDataCheck(buffer, bufferLen, pMsgData);
   }

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(pMsgData->portHandle, &pPort);
   }

   /* Lock the mutex */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPort->clock);
   }

   if (status == ZL303XX_OK)
   {
      /* Don't process if clock RX is disabled */
      if (pPort->clock->enableRxTx == ZL303XX_FALSE)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5,
               "Clock %u: Discarded RX message: clock messaging disabled",
               pPort->clock->clockHandle, 0,0,0,0,0);
      }
      else
      {
         msg.clockStreamId = (Uint32T)ZL303XX_PTP_INVALID;

         msg.recvMsgInfo.clockPortHandle = pMsgData->portHandle;
         msg.recvMsgInfo.srcAddr = pMsgData->srcAddr;
         msg.recvMsgInfo.destAddr = pMsgData->destAddr;
         msg.recvMsgInfo.rxTs = pMsgData->rxTs;
         OS_MEMCPY(msg.recvMsgInfo.extData, pMsgData->extData, sizeof(msg.recvMsgInfo.extData));

         /* Copy the message into the buffer (better if we didn't have to copy but at least
            it is only the first part of the message, i.e. we truncate it here
            if it is a big packet) */
         msg.pktLength = min(bufferLen, sizeof(msg.msgBuf));
         OS_MEMCPY(msg.msgBuf, buffer, msg.pktLength);

         /* Set the message type */
         msg.taskMsgId = PTP_TASK_MSG_RECV_PKT;

         /* Send the Packet to be processed. */
         status = zl303xx_PtpClockMsgQSend(pPort->clock, &msg, sizeof(msg));

         /* If we are here, we must have the Mutex.
          * Release the MUTEX (ignore the status of the release). */
         (void)zl303xx_PtpClockMutexUnlock(pPort->clock, status);
      }
   }

   return status;
}

/* zl303xx_PtpStreamRxMsgProcess */
/**
   Handles a PTP received PTP message. The stream handle of the message is
   used to send the message to the proper PTP clock task for processing.

   If providing a stream handle is more convenient, or the PTP connection
   cannot adequately be represented by a source/destination PortAddress pair,
   this function can be called instead of zl303xx_PtpRxMsgProcess().

   This function can only be used to handle unicast messages.

  Parameters:
   [in]   buffer     Byte-buffer with the PTP message in network order.
   [in]   bufferLen  Length of the buffer.
   [in]   pMsgData   Extra information about the message.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    buffer or pMsgData is NULL.
     ZL303XX_INVALID_PARAMETER  bufLen == 0.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamRxMsgProcess(Uint8T *buffer, Uint16T bufferLen,
                                        zl303xx_PtpStreamRxMsgDataS *pMsgData)
{
   zlStatusE status = ZL303XX_OK;
   const char * fnName = "zl303xx_PtpStreamRxMsgProcess";
   zl303xx_PtpClockMsgQueueS msg;
   zl303xx_ClockStreamS *pStream;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4, "%s:", fnName, 0,0,0,0,0);

   OS_MEMSET(&msg, 0, sizeof(msg));

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpRxMsgDataCheck(buffer, bufferLen, pMsgData);
   }

   /* Get a pointer to the stream data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(pMsgData->streamHandle, &pStream);
   }

   /* Lock the mutex */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pStream->portData->clock);
   }

   /* Find the PTP port this message is intended for. */
   if (status == ZL303XX_OK)
   {
      /* Don't process if clock RX is disabled */
      if (pStream->portData->clock->enableRxTx == ZL303XX_FALSE)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5, "Clock %u: Discarded RX message: clock messaging disabled",
               pStream->portData->clock->clockHandle, 0,0,0,0,0);
      }
      else
      {
         msg.clockStreamId = pMsgData->streamHandle;

         /* Assume the source address of the message is the destination address of the stream. */
         msg.recvMsgInfo.srcAddr = pStream->config.destAddr;
         msg.recvMsgInfo.clockPortHandle = pStream->portData->clockPortHandle;
         msg.recvMsgInfo.rxTs = pMsgData->rxTs;
         OS_MEMCPY(msg.recvMsgInfo.extData, pMsgData->extData, sizeof(msg.recvMsgInfo.extData));

         /* Copy the message into the buffer (better if we didn't have to copy but at least
            it is only the first part of the message, i.e. we truncate it here
            if it is a big packet). */
         msg.pktLength = min(bufferLen, sizeof(msg.msgBuf));
         OS_MEMCPY(msg.msgBuf, buffer, msg.pktLength);

         /* Set the message type */
         msg.taskMsgId = PTP_TASK_MSG_RECV_PKT;

         /* Send the Packet to be processed. */
         status = zl303xx_PtpClockMsgQSend(pStream->portData->clock, &msg, sizeof(msg));

         /* Release the MUTEX (ignore the status of the release). */
         (void)zl303xx_PtpClockMutexUnlock(pStream->portData->clock, status);
      }
   }

   return status;
}

/****************   STATIC FUNCTION DEFINITIONS   ****************************/

/* zl303xx_PtpRxMsgDataCheck */
/** 

   Checks the input parameters for the RxMsgProcess functions.

  Parameters:
   [in]   buffer     Byte-buffer with the PTP message in network order.
   [in]   bufferLen  Length of the buffer.
   [in]   pMsgData   Extra information about the message.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    buffer or pMsgData is NULL.
     ZL303XX_INVALID_PARAMETER  bufLen == 0.

*******************************************************************************/
static zlStatusE zl303xx_PtpRxMsgDataCheck(Uint8T *buffer, Uint16T bufferLen,
                                           void *pMsgData)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(buffer) |
                      ZL303XX_CHECK_POINTER(pMsgData);

   if (status == ZL303XX_OK)
   {
      if (bufferLen == 0)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Invalid bufferLen=%u.", bufferLen, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   return status;
}
