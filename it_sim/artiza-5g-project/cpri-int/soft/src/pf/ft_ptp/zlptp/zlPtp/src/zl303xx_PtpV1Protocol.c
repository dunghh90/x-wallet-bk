

/*****************************************************************************
*
*  $Id: zl303xx_PtpV1Protocol.c 13442 2016-05-03 16:49:21Z SW $
*
*  COPYRIGHT
*  The original version of this file is part of PTPd version 1b2 from
*  ptpd.sourceforge.net and is used here under the copyright terms
*  of that project.
*  Original file: Copyright (c) 2005 Kendall Correll.
*  Modifications in this version: Copyright (c) 2008 Zarlink Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains the protocol state machines for the
*     IEEE-1588-2002 ptp protocol
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"
#include "zl303xx_ErrTrace.h"
#include "zl303xx_Os.h"

#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpControl.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpV2MsgParse.h"
#include "zl303xx_PtpStrings.h"

/*****************   DEFINES     **********************************************/

/* Number of packets in a burst */
#define BURST_SIZE        8

/*****************   TYPEDEFS    **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

#ifdef ZL303XX_INCLUDE_PTP_V1
_ZL303XX_LOCAL void zl303xx_PtpV1DoInitStream(zl303xx_ClockStreamS *clockStream);
_ZL303XX_LOCAL void zl303xx_PtpV1DoInitPort(zl303xx_PortDataS *portData);
_ZL303XX_LOCAL void zl303xx_PtpV1InitStreamData(zl303xx_ClockStreamS *clockStream);
_ZL303XX_LOCAL void zl303xx_PtpV1DoStreamState(zl303xx_ClockStreamS *clockStream);

_ZL303XX_LOCAL void zl303xx_PtpV1handleRecvMsg(zl303xx_ClockStreamS *clockStream,  zl303xx_PortDataS *portData, zl303xx_PtpClockMsgQueueS *msg);
_ZL303XX_LOCAL void zl303xx_PtpV1handleSync(zl303xx_MsgHeaderS *header, Uint8T *msgIbuf,
                                             zl303xx_TimeStamp *rxTime, zl303xx_ClockStreamS *clockStream);
_ZL303XX_LOCAL void zl303xx_PtpV1handleFollowUp(zl303xx_MsgHeaderS *header, Uint8T *msgIbuf, zl303xx_ClockStreamS *clockStream);
_ZL303XX_LOCAL void zl303xx_PtpV1handleDelayReq(zl303xx_MsgHeaderS*, zl303xx_TimeStamp *, zl303xx_ClockStreamS *);
_ZL303XX_LOCAL void zl303xx_PtpV1handleDelayResp(zl303xx_MsgHeaderS *header, Uint8T *msgIbuf, zl303xx_ClockStreamS *clockStream);
_ZL303XX_LOCAL void zl303xx_PtpV1handleManagement(zl303xx_MsgHeaderS *header, Uint8T *msgIbuf, zl303xx_ClockStreamS *clockStream);

_ZL303XX_LOCAL void zl303xx_PtpV1IssueSync(zl303xx_ClockStreamS *clockStream, zl303xx_BooleanE burst, zl303xx_BooleanE followUp);
_ZL303XX_LOCAL void zl303xx_PtpV1IssueSyncBurst(zl303xx_ClockStreamS *clockStream);
_ZL303XX_LOCAL void zl303xx_PtpV1IssueDelayReq(zl303xx_ClockStreamS *,zl303xx_BooleanE burst);
_ZL303XX_LOCAL void zl303xx_PtpV1IssueDelayResp(zl303xx_TimeStamp *delayReqRecvTime, zl303xx_MsgHeaderS *header,
                                                   zl303xx_ClockStreamS *clockStream);
_ZL303XX_LOCAL void zl303xx_PtpV1IssueManagement(zl303xx_MsgHeaderS*,MsgV1Management*,zl303xx_ClockStreamS *);
_ZL303XX_LOCAL MsgV1Sync * zl303xx_PtpV1AddForeign(Uint8T *buf, zl303xx_MsgHeaderS *header, zl303xx_ClockStreamS *clockStream);

_ZL303XX_LOCAL Uint16T getRand(Uint32T *seed);

_ZL303XX_LOCAL zl303xx_BooleanE zl303xx_EqualV1Identities(V1PortIdentityS *first, V1PortIdentityS *second);

static void zl303xx_PtpV1TwoStepRegisterTx(zl303xx_ClockStreamS *stream, Uint16T seqId,
                                           zl303xx_TimeStamp *txTs,
                                           V1PortIdentityS *srcPortId);
static zl303xx_BooleanE zl303xx_PtpV1TwoStepRegisterRx(zl303xx_ClockStreamS *stream,
                                                       Uint16T seqId,
                                                       zl303xx_TimeStamp *rxTs,
                                                       V1PortIdentityS *srcPortId);

/* External routines */
void zl303xx_PtpStreamCalculateDrift(zl303xx_ClockStreamS *pPtpStream);
#endif


/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

#ifdef ZL303XX_INCLUDE_PTP_V1
/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/


/*

  Function Name:
    zl303xx_PtpV1Protocol

  Details:
   Main function to implement the protocol for IEEE 1588 version 1

  Parameters:
       clockStream    The PTP clock stream instance structure.
                  Could be NULL if no clock stream yet exists
       portData       Pointer to the data for the port on which this message was
                  received
       msg            A received event message

  Return Value:
   zlStatusE

 *****************************************************************************/

zlStatusE zl303xx_PtpV1Protocol(zl303xx_ClockStreamS *clockStream, zl303xx_PortDataS *portData, zl303xx_PtpClockMsgQueueS *msg)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   if (portData)
   {
      pPtpClock = portData->clock;
   }
   else
   {
      ZL303XX_ERROR_NOTIFY("zl303xx_PtpV1Protocol(): NULL port data");
      status = ZL303XX_PARAMETER_INVALID;
   }

   if (status == ZL303XX_OK)
   {
      if (portData->portSuperState == PTP_PORT_INT_STATE_INIT)
      {
         /* Force the port into the initialising state proper */
         status = zl303xx_PtpV1ToPortState(PTP_PORT_INT_STATE_INIT, portData);
      }
      else
      {
         if (msg->taskMsgId == PTP_TASK_MSG_RECV_PKT)
         {
            if (portData->portSuperState == PTP_PORT_INT_STATE_ACTIVE)
            {
               zl303xx_PtpV1handleRecvMsg(clockStream, portData, msg);
            }
            else
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "Port %u inactive -> disregarding received message", portData->config.portNumber,0,0,0,0,0);
            }
         }
         else if (msg->taskMsgId == PTP_TASK_MSG_CONTROL_CMD)
         {
            handleLocalCtrlMessages(clockStream, msg);   /* Process any locally generated control messages */
         }
         else if (msg->taskMsgId == PTP_TASK_MSG_TIMER_TICK)
         {
            /* Nothing special to do for timer messages. Just re-execute the state machine */
         }

         /* Always execute the port state machine */
         zl303xx_PtpDoPortState(portData);

         /* Execute the stream state machine as well if it exists */
         if (clockStream)
         {
            zl303xx_PtpV1DoStreamState(clockStream);
         }
         else
         {
            /* Use clockStream as a temp variable, then set it back to NULL */
            ZL303XX_PTP_FOR_EACH_STREAM(&portData->streamList, clockStream)
            {
               zl303xx_PtpV1DoStreamState(clockStream);
            }
            clockStream = NULL;
         }
      }
   }

   if (status == ZL303XX_OK)
   {
      if (clockStream)
      {
         if (zl303xx_PtpTimerExpired(&clockStream->itimer[QUALIFICATION_TIMER]) == ZL303XX_TRUE)
         {  /* End of a PTP_FOREIGN_MASTER_TIME_WINDOW period so latch all foreign record sync values for
               qualification purposes */
            Sint32T count;

            for(count = 0; count < pPtpClock->foreignMasterDS.numActiveRecords; ++count)
            {
               pPtpClock->foreignMasterDS.foreign[count].msgCount.previous = pPtpClock->foreignMasterDS.foreign[count].msgCount.current;
               pPtpClock->foreignMasterDS.foreign[count].msgCount.current = 0;
            }
         }

         if (zl303xx_PtpTimerExpired(&clockStream->itimer[ONE_SEC_TIMER]) == ZL303XX_TRUE)
         {
            /* Do once per second updates */

            /* Calculate the drift (phase change) in ns/s. The result is stored
             * in the observedPhaseChangeRateInternal variable */
            zl303xx_PtpStreamCalculateDrift(clockStream);

            /* Output some once-per-second status information if required */
            if ((pPtpClock->displayOffsetFromMaster) &&
                (clockStream->streamSubstate == PTP_STREAM_SUBSTATE_SLAVE))
            {
               /* Output the offset and drift of the stream for which the message was received. */
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,  "Clock stream %u: offset from master: " PTPD_FORMAT_TIME_STR,
                           clockStream->clockStreamHandle,
                           formatTimeStringSign(&clockStream->farEndCurrentDS.offset_from_master),
                           formatTimeStringSec(&clockStream->farEndCurrentDS.offset_from_master),
                           formatTimeStringNs(&clockStream->farEndCurrentDS.offset_from_master), 0,0);
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,  "         drift: %5ld ns/s ",
                           clockStream->drift.observedPhaseChangeRateInternal, 0,0,0,0,0);
            }
         }
      }
   }

   return ZL303XX_OK;
}

/*

  Function Name:
    zl303xx_PtpV1ToPortState

  Details:
   Function to implement a port state transition from the current state into
   into the specified state

  Parameters:
       nextState      The state to enter
       portData       Pointer to the data for the port

  Return Value:
   zlStatusE

 *****************************************************************************/

zlStatusE zl303xx_PtpV1ToPortState(zl303xx_PtpPortInternalStateE nextState,
                                   zl303xx_PortDataS *portData)
{
   zlStatusE status = ZL303XX_OK;
   Sint32T streamId;

   /* Leaving current state tasks */
   switch(portData->portSuperState)
   {
      case PTP_PORT_INT_STATE_INIT :
         break;
      default:
         /* Currently none */
         break;
   }

   /* Entering state tasks */
   switch (nextState)
   {
      case PTP_PORT_INT_STATE_INIT :
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "Port number %u: state INITIALIZING", portData->config.portNumber, 0,0,0,0,0);
         zl303xx_PtpV1DoInitPort(portData);
         break;
      case PTP_PORT_INT_STATE_FAULTY :
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "Port number %u: state FAULTY", portData->config.portNumber, 0,0,0,0,0);
         break;
      case PTP_PORT_INT_STATE_DISABLED :
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "Port number %u: state DISABLED", portData->config.portNumber, 0,0,0,0,0);
         break;
      case PTP_PORT_INT_STATE_ACTIVE :
         break;
   }

   portData->portSuperState = nextState;

   /* Iterate around all the active clock streams to allow them to re-evaluate their state */
   for (streamId = 0; (streamId < ZL303XX_PTP_NUM_STREAMS_MAX) && (status == ZL303XX_OK); streamId++)
   {
      zl303xx_ClockStreamS *clockStream = NULL;
      status = zl303xx_PtpStreamDataGet(streamId, &clockStream);

      if (status == ZL303XX_OK)
      {
         zl303xx_PtpV1DoStreamState(clockStream);
      }
   }

   if (portData->portSuperState == PTP_PORT_INT_STATE_INIT)
   {
      /* All the streams will have now initialised */
      status = zl303xx_PtpV1ToPortState(PTP_PORT_INT_STATE_ACTIVE, portData);
   }

   return status;
}

/*

  Function Name:
    zl303xx_PtpV1DoInitPort

  Details:
   Initialises a version 1 PTP clock port

  Parameters:
       portData    Pointer to the clock port instance structure

  Return Value:
   Nothing

  Notes:
   This is called on startup and for each subsequent reinitialisation

*******************************************************************************/

void zl303xx_PtpV1DoInitPort(zl303xx_PortDataS *portData)
{
   zl303xx_PtpClockS *pPtpClock = portData->clock;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "Manufacturer Identity: %s. Revision %s", PtpManufacturerID, PtpRevisionData,0,0,0,0);
   if (PtpUserDescription[0] != '\0')
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "User Descrription: %s  ", PtpUserDescription, 0,0,0,0,0);
   }

   /* Reset the malformed packet count. This should only be cleared when the port is truly initialised */
   portData->malformedPktCount = 0;

   /* Display debug output, if enabled */
   /* Display debug output, if enabled */
   ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_ENGINE, 2)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "Clock port %u initial configuration:  ", portData->config.portNumber, 0,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4, "  clock identifier: %s  ", pPtpClock->defaultDSInt.u.v1.clock_identifier, 0,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4, "  256*log2(clock variance): %d  ", pPtpClock->defaultDSInt.u.v1.clock_variance, 0,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "  clock stratum: %d  ", pPtpClock->defaultDSInt.u.v1.clock_stratum, 0,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3, pPtpClock->defaultDSInt.u.v1.preferred?"  clock preferred?: yes  ":"  clock preferred?: no  ", 0,0,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5, "  communication technology: %d  ", portData->portConfig.u.v1.portIdentity.communicationTechnology, 0,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "  uuid: %02hx:%02hx:%02hx:%02hx:%02hx:%02hx  ",
          portData->portConfig.u.v1.portIdentity.uuid[0], portData->portConfig.u.v1.portIdentity.uuid[1],
          portData->portConfig.u.v1.portIdentity.uuid[2], portData->portConfig.u.v1.portIdentity.uuid[3],
          portData->portConfig.u.v1.portIdentity.uuid[4], portData->portConfig.u.v1.portIdentity.uuid[5]);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "  port number: %u  ", portData->config.portNumber, 0,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3, "PTP subdomain name: %s  ", pPtpClock->defaultDSInt.u.v1.subdomain_name, 0,0,0,0,0);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4, "event port address: %hx %hx  ",
         portData->portConfig.u.v1.event_port_address[0], portData->portConfig.u.v1.event_port_address[1], 0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4, "general port address: %hx %hx  ",
         portData->portConfig.u.v1.general_port_address[0], portData->portConfig.u.v1.general_port_address[1], 0,0,0,0);
   }  /* ZL303XX_DEBUG_FILTER() */
}

/*

  Function Name:
    zl303xx_PtpV1DoInitStream

  Details:
   Initialises a version 1 PTP clock stream

  Parameters:
       clockStream    The PTP clock stream instance structure

  Return Value:
   Nothing

  Notes:
   This is called on startup and for each subsequent reinitialisation

*******************************************************************************/

void zl303xx_PtpV1DoInitStream(zl303xx_ClockStreamS *clockStream)
{
   zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

   /* initialize the clock stream */
   zl303xx_PtpV1InitStreamData(clockStream);

   /* Reset the sequence numbers */
   clockStream->u.v1.last_sync_event_sequence_number = 0;
   clockStream->u.v1.last_general_event_sequence_number = 0;

   /* Initialise the parent and grandmaster values from our own clock values */
   zl303xx_PtpV1DoM1Update(clockStream);

   /* For version 1 pack the common part of the header which is constant and therefore set only once */
   zl303xx_MsgV1PackHeader(pPtpClock->msgObuf, clockStream);

   /* Display debug output, if enabled */
   ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_ENGINE, 2)
   {
      zl303xx_TimeStamp interval;
      const char *streamModeStr = zl303xx_PtpStreamModeToStr(clockStream->config.mode);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
            "Stream %u initial configuration:  ",
            clockStream->clockStreamHandle, 0,0,0,0,0);

      zl303xx_PtpGetSyncIntervalTimeout(clockStream, &interval);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
            "  sync message timeout: " PTPD_FORMAT_TIME_STR,
            formatTimeStringSign(&interval),
            formatTimeStringSec(&interval),
            formatTimeStringNs(&interval), 0,0,0);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
            "  logSyncInterval: %d  ",
            clockStream->config.logSyncInterval, 0,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
            "  requested stream mode: %s", streamModeStr, 0,0,0,0,0);
   }  /* ZL303XX_DEBUG_FILTER() */
}

/*

  Function Name:
    zl303xx_PtpV1DoStreamState

  Details:
   Perform clock stream specific state actions according to V1 protocol

  Parameters:
       clockStream    The PTP clock stream instance structure

  Return Value:
   Nothing

 *****************************************************************************/

void zl303xx_PtpV1DoStreamState(zl303xx_ClockStreamS *clockStream)
{
   zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;
   Uint32T numPktsToNowSend;

   /* If the overall port state is not active then go idle */
   if (clockStream->portData->portSuperState == PTP_PORT_INT_STATE_INIT )
   {
      zl303xx_PtpV1ToStreamState(PTP_STREAM_SUBSTATE_INIT, clockStream);
   }
   else if (clockStream->portData->portSuperState != PTP_PORT_INT_STATE_ACTIVE )
   {
      if (clockStream->streamSubstate != PTP_STREAM_SUBSTATE_IDLE)
      {
         zl303xx_PtpV1ToStreamState(PTP_STREAM_SUBSTATE_IDLE, clockStream);
      }
   }
   else /* superstate is ACTIVE */
   {
      /* Force any state transitions that are necessary */
      switch (clockStream->streamSubstate)
      {
         case PTP_STREAM_SUBSTATE_INIT :
            /* This should not happen because the substate should go IDLE before the superstate goes ACTIVE */
            ZL303XX_TRACE_ALWAYS("Internal state error, superstate = ACTIVE, substate = INITIALIZING", 0,0,0,0,0,0);
            break;

         case PTP_STREAM_SUBSTATE_IDLE :
            if (clockStream->config.mode == ZL303XX_PTP_STREAM_MASTER_ONLY)
            {
               zl303xx_PtpV1ToStreamState(PTP_STREAM_SUBSTATE_MASTER, clockStream);
            }
            else
            {
               zl303xx_PtpV1ToStreamState(PTP_STREAM_SUBSTATE_LISTENING, clockStream);
            }
            break;

         default :

            if (clockStream->config.unicast == ZL303XX_TRUE)
            {  /* unicast stream */
               if ((clockStream->config.mode == ZL303XX_PTP_STREAM_MASTER_ONLY) &&
                   (clockStream->streamSubstate != PTP_STREAM_SUBSTATE_MASTER))
               {
                  zl303xx_PtpV1ToStreamState(PTP_STREAM_SUBSTATE_MASTER, clockStream);
               }
               else if ((clockStream->config.mode == ZL303XX_PTP_STREAM_SLAVE_ONLY) &&
                   (clockStream->streamSubstate != PTP_STREAM_SUBSTATE_SLAVE))
               {
                  zl303xx_PtpV1ToStreamState(PTP_STREAM_SUBSTATE_SLAVE, clockStream);
               }
            }
            else
            {  /* Multicast stream */
               /* If the operating mode has been forced to either master only or slave only
                  and this is not consistent with the current mode then apply the change now */
               if ((clockStream->config.mode == ZL303XX_PTP_STREAM_MASTER_ONLY) &&
                   (clockStream->streamSubstate != PTP_STREAM_SUBSTATE_MASTER))
               {
                  zl303xx_PtpV1ToStreamState(PTP_STREAM_SUBSTATE_MASTER, clockStream);
               }
               else if ((clockStream->config.mode == ZL303XX_PTP_STREAM_SLAVE_ONLY) &&
                        ((clockStream->streamSubstate == PTP_STREAM_SUBSTATE_MASTER) ||
                         (clockStream->streamSubstate == PTP_STREAM_SUBSTATE_PRE_MASTER) ||
                         (clockStream->streamSubstate == PTP_STREAM_SUBSTATE_PASSIVE)))
               {   /* Force to listening state (to allow it
                      time to determine the best visible master) */
                  zl303xx_PtpV1ToStreamState(PTP_STREAM_SUBSTATE_LISTENING, clockStream);
               }
               else if (pPtpClock->foreignMasterDS.tableUpdated)
               {  /* There is an updated record. For multicast streams apply the best master clock */
                  zl303xx_PtpStreamSubStateE nextState;

                  pPtpClock->foreignMasterDS.tableUpdated = ZL303XX_FALSE;
                  nextState = zl303xx_PtpV1Bmc(&pPtpClock->foreignMasterDS, clockStream);
                  if (nextState != clockStream->streamSubstate)
                     zl303xx_PtpV1ToStreamState(nextState, clockStream);
               }
            }
            break;
      }


      /* Now execute the in-state behaviour */
      switch(clockStream->streamSubstate)
      {
         case PTP_STREAM_SUBSTATE_INIT :
         case PTP_STREAM_SUBSTATE_IDLE :
            break;

         case PTP_STREAM_SUBSTATE_PRE_MASTER :
            zl303xx_PtpV1ToStreamState(PTP_STREAM_SUBSTATE_MASTER, clockStream);
            break;

         case PTP_STREAM_SUBSTATE_LISTENING :
         case PTP_STREAM_SUBSTATE_PASSIVE :
         case PTP_STREAM_SUBSTATE_UNCALIBRATED :
         case PTP_STREAM_SUBSTATE_SLAVE :
            /* To obtain packet rates faster than one second we sometimes need to send multiple
             * packets. We do this by sending a burst when necessary on the interval timeout.
             * We don't wait for the delay_resp to be received in each case so this could
             * cause a problem (although it shouldn't) for some server implementations, especially
             * if we also request a sync burst from each delay_req */
            numPktsToNowSend = zl303xx_PtpTimerExpired(&(clockStream->itimer[Q_TIMER]));

            if (numPktsToNowSend)
            {
               (void)zl303xx_PtpTimerStop(&clockStream->itimer[Q_TIMER]);

               /* Send all but the last packet without the burst request flag.
                * This is to reduce the loading on the server */
               numPktsToNowSend--;
               while (numPktsToNowSend--)
               {
                  zl303xx_PtpV1IssueDelayReq(clockStream,ZL303XX_FALSE);
               }
               /* Send the last packet with the user specified burst flag */
               zl303xx_PtpV1IssueDelayReq(clockStream, pPtpClock->useDelayReqSyncBurst);

               /* Calculate the interval until the next delay_req cycle */
               /* R is the number of sync cycles to wait before performing the next delay_req cycle */
               /* Q is the number of 1/18ths of a sync interval to wait after the Rth sync cycle */

               /* If useStandardDelayReq is set then use standard IEEE-1588-2002 behaviour for delay_req */
               if ( pPtpClock->useStandardDelayReq )
               {
                  /* Calculate the random interval until the next delay_req cycle as per IEEE-1588-2002 */
                  Uint32T seed = clockStream->drift.latestSyncRxTime.nanosecondsField;

                  clockStream->u.v1.R = (getRand(&seed)%(PTP_DELAY_REQ_INTERVAL - 2)) + 2;
                  clockStream->u.v1.Q = (getRand(&seed)%(PTP_RANDOMIZING_SLOTS - 2)) + 2;
                  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 9, "R = %d, Q = %d", clockStream->u.v1.R, clockStream->u.v1.Q, 0,0,0,0);
               }
               else
               {  /* Use non-standard (quick) delay_req cycle */
                  clockStream->u.v1.R = 0;  /* Don't wait for sync pulses */
                  (void)zl303xx_PtpStreamTimerStart(clockStream, Q_TIMER);
               }
            }

            if (zl303xx_PtpTimerExpired(&clockStream->itimer[SYNC_RECEIPT_TIMER]) == ZL303XX_TRUE)
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "Clock stream %u: event SYNC_RECEIPT_TIMEOUT_EXPIRES  ",clockStream->clockStreamHandle, 0,0,0,0,0);
               pPtpClock->foreignMasterDS.numActiveRecords = 0;
               /* Always service the sync receipt timer but don't allow this to cause a switch to master
                  mode if not allowed */
               if (clockStream->config.mode != ZL303XX_PTP_STREAM_SLAVE_ONLY)
               {
                  zl303xx_PtpV1DoM1Update(clockStream);
                  zl303xx_PtpV1ToStreamState(PTP_STREAM_SUBSTATE_MASTER, clockStream);
               }
            }

            break;

         case PTP_STREAM_SUBSTATE_MASTER:
         {
            numPktsToNowSend = zl303xx_PtpTimerExpired(&(clockStream->itimer[SYNC_INTERVAL_TIMER]));

            if (numPktsToNowSend)
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
                             "Stream %u: Event SYNC_INTERVAL_TIMEOUT_EXPIRES: #PKTs %lu ",
                             clockStream->clockStreamHandle, numPktsToNowSend, 0,0,0,0);

               /* Send the required number of sync packets */
               while (numPktsToNowSend--)
               {
                  /* Periodic sync messages never have the burst flag set,
                   * even though we may be sending several together */
                  zl303xx_PtpV1IssueSync(clockStream,
                                         ZL303XX_FALSE,
                                         pPtpClock->defaultDSInt.u.v1.clock_followup_capable);
               }
            }

            break;
         }

         default:
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                  "zl303xx_PtpV1DoStreamState: Stream %u, unrecognized state %d ",
                  clockStream->clockStreamHandle,
                  clockStream->streamSubstate,0,0,0,0);

            break;
      }
   }
}

/*

  Function Name:
    zl303xx_PtpV1ToStreamState

  Details:
   Perform actions on a state transition for the port state machine
   ie. when leaving current state and entering 'state', according to the V1
   protocol definition

  Parameters:
       nextState      The state to enter
       clockStream Pointer to the stream data instance structure

  Return Value:
   Nothing

 *****************************************************************************/

void zl303xx_PtpV1ToStreamState(zl303xx_PtpStreamSubStateE nextState, zl303xx_ClockStreamS *clockStream)
{
   zl303xx_TimeStamp timeInterval;

   /* leaving state tasks */
   switch(clockStream->streamSubstate)
   {
      case PTP_STREAM_SUBSTATE_PRE_MASTER:
      case PTP_STREAM_SUBSTATE_MASTER:
         /* Stop the sync transmit interval timer and start the sync receipt timeout timer */
         (void)zl303xx_PtpTimerStop(&clockStream->itimer[SYNC_INTERVAL_TIMER]);
         (void)zl303xx_PtpStreamTimerStart(clockStream, SYNC_RECEIPT_TIMER);
         break;

      case PTP_STREAM_SUBSTATE_SLAVE:
         /* Stop the delay request 'Q' timer */
         (void)zl303xx_PtpTimerStop(&clockStream->itimer[Q_TIMER]);
         zl303xx_PtpV1InitStreamData(clockStream);
         break;

      default:
         break;
   }

  /* entering state tasks */
  switch(nextState)
  {
      case PTP_STREAM_SUBSTATE_INIT :
      {
         /* Reset the dynamic counters and calculated members of the stream. */
         zl303xx_PtpV1DoInitStream(clockStream);

         {  /* Start a 1 second timer for once-per-second updating */
            (void)zl303xx_PtpStreamTimerStart(clockStream, ONE_SEC_TIMER);
         }

         zl303xx_PtpV1ToStreamState(PTP_STREAM_SUBSTATE_IDLE, clockStream);
         break;
      }

      case PTP_STREAM_SUBSTATE_IDLE:
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "Stream %u: state PTP_STREAM_SUBSTATE_IDLE",
               clockStream->clockStreamHandle, 0,0,0,0,0);

         (void)zl303xx_PtpTimerStop(&clockStream->itimer[SYNC_RECEIPT_TIMER]);

          /* Start the sync burst timer with a minimum period. This is to ensure that it will
             have expired at least once when it is first tested */
          timeInterval.secondsField.hi = 0;
          timeInterval.secondsField.lo = 0;
          timeInterval.nanosecondsField = 1; /* Shortest possible interval - actually will be one tick */
          (void)zl303xx_PtpTimerStart(clockStream->portData->clock->clockHandle,
                                      &clockStream->itimer[SYNC_BURST_DELAY_TIMER],
                                      &timeInterval, ZL303XX_TRUE);

          /* Start the sync qualification timer. This runs continually at a
           * multiple of the sync_interval for PTP_V1. (This multiple is
           * PTP_FOREIGN_MASTER_TIME_WINDOW = 4). */
          (void)zl303xx_PtpStreamTimerStart(clockStream, QUALIFICATION_TIMER);

         clockStream->streamSubstate = PTP_STREAM_SUBSTATE_IDLE;
         break;

      case PTP_STREAM_SUBSTATE_LISTENING:
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "Stream %u: state PTP_STREAM_SUBSTATE_LISTENING  ",
               clockStream->clockStreamHandle, 0,0,0,0,0);

          if (clockStream->config.mode == ZL303XX_PTP_STREAM_USE_BMC)
          {
             (void)zl303xx_PtpStreamTimerStart(clockStream, SYNC_RECEIPT_TIMER);
          }

         clockStream->streamSubstate = PTP_STREAM_SUBSTATE_LISTENING;
         break;

      case PTP_STREAM_SUBSTATE_PRE_MASTER:
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "Stream %u: state PTP_STREAM_SUBSTATE_PRE_MASTER  ",
               clockStream->clockStreamHandle, 0,0,0,0,0);

         clockStream->streamSubstate = PTP_STREAM_SUBSTATE_PRE_MASTER;
         break;

     case PTP_STREAM_SUBSTATE_MASTER:
       ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "Clock stream %u: state PTP_V1_MASTER  ",
             clockStream->clockStreamHandle, 0,0,0,0,0);

         (void)zl303xx_PtpStreamTimerStart(clockStream, SYNC_INTERVAL_TIMER);

         (void)zl303xx_PtpTimerStop(&clockStream->itimer[SYNC_RECEIPT_TIMER]);

         clockStream->streamSubstate = PTP_STREAM_SUBSTATE_MASTER;

         break;

      case PTP_STREAM_SUBSTATE_PASSIVE:
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "Stream %u: state PTP_STREAM_SUBSTATE_PASSIVE  ",
               clockStream->clockStreamHandle, 0,0,0,0,0);

         clockStream->streamSubstate = PTP_STREAM_SUBSTATE_PASSIVE;
         break;

      case PTP_STREAM_SUBSTATE_UNCALIBRATED:
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "Stream %u: state PTP_STREAM_SUBSTATE_UNCALIBRATED  ",
               clockStream->clockStreamHandle, 0,0,0,0,0);

         clockStream->streamSubstate = PTP_STREAM_SUBSTATE_UNCALIBRATED;
         break;

      case PTP_STREAM_SUBSTATE_SLAVE:
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "Stream %u: state PTP_STREAM_SUBSTATE_SLAVE  ",
               clockStream->clockStreamHandle, 0,0,0,0,0);

         zl303xx_PtpV1InitStreamData(clockStream);

         {
            zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

             /* R is chosen to allow a few syncs before we first get a one-way delay estimate */
             /* this is to allow the offset filter to fill for an accurate initial clock reset. */
             /* Q set to minimum value for start off (Note that the oiginal version of PTPd has now
                removed the Q timer but we still use it) */
             clockStream->u.v1.Q = 2;
             clockStream->u.v1.R = getRand(&pPtpClock->random_seed)%4 + 4;
             ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4, "Q = %d, R = %d  ",
                           clockStream->u.v1.Q, clockStream->u.v1.R, 0,0,0,0);
         }

         clockStream->streamSubstate = PTP_STREAM_SUBSTATE_SLAVE;
         break;

      default:
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "Stream %u: to unrecognized state  ", clockStream->clockStreamHandle, 0,0,0,0,0);
         break;
   }
}


/*

  Function Name:
   zl303xx_GetV1PortState

  Details:
   Converts the internal state of the port into an overall PTP V1 state
   value

  Parameters:
       portData    Pointer to the port data structure

  Return Value:
   the state

 *****************************************************************************/
zl303xx_PortStateE zl303xx_GetV1PortState(zl303xx_PortDataS *portData)
{
   zl303xx_PortStateE v1StateValue;

   switch (portData->portSuperState)
   {
      case PTP_PORT_INT_STATE_INIT :
         v1StateValue = ZL303XX_PORT_STATE_INITIALIZING;
         break;
      case PTP_PORT_INT_STATE_FAULTY :
         v1StateValue = ZL303XX_PORT_STATE_FAULTY;
         break;
      case PTP_PORT_INT_STATE_DISABLED :
         v1StateValue = ZL303XX_PORT_STATE_DISABLED;
         break;
      case PTP_PORT_INT_STATE_ACTIVE :
         if (portData->mcastStream != NULL)
         {
            switch (portData->mcastStream->streamSubstate)
            {
               case PTP_STREAM_SUBSTATE_IDLE :
                  /* This is an internal error condition, the port is enabled and has multicast enabled but the state
                     machine is invalid */
                  ZL303XX_ERROR_NOTIFY("Invalid state combination");
                  v1StateValue = ZL303XX_PORT_STATE_FAULTY;
                  break;
               case PTP_STREAM_SUBSTATE_LISTENING :
                  v1StateValue = ZL303XX_PORT_STATE_LISTENING;
                  break;
               case PTP_STREAM_SUBSTATE_PRE_MASTER :
                  v1StateValue = ZL303XX_PORT_STATE_PRE_MASTER;
                  break;
               case PTP_STREAM_SUBSTATE_MASTER :
                  v1StateValue = ZL303XX_PORT_STATE_MASTER;
                  break;
               case PTP_STREAM_SUBSTATE_PASSIVE :
                  v1StateValue = ZL303XX_PORT_STATE_PASSIVE;
                  break;
               case PTP_STREAM_SUBSTATE_UNCALIBRATED :
                  v1StateValue = ZL303XX_PORT_STATE_UNCALIBRATED;
                  break;
               case PTP_STREAM_SUBSTATE_SLAVE :
                  v1StateValue = ZL303XX_PORT_STATE_SLAVE;
                  break;
               default :
                  ZL303XX_ERROR_NOTIFY("Invalid substate value");
                  v1StateValue = ZL303XX_PORT_STATE_FAULTY;
                  break;
            }
         }
         else
         {
            /* The port itself is enabled but its multicast processing is disabled. The IEEE 1588 standard does not
               support this possibility since it is written assuming that multicast will be operational. But for unicast
               support according to the telecoms profile it is a requirement that multicast be able to be disabled.
               So there is no defined state in the standard for this condition so this implementation returns the
               closest sensible state which is probably LISTENING, even though this is not actually true. It should
               at least be benign to any remote equipment that queries it. */
            v1StateValue = ZL303XX_PORT_STATE_LISTENING;
         }
         break;
      default :
         ZL303XX_ERROR_NOTIFY("Invalid superstate value");
         v1StateValue = ZL303XX_PORT_STATE_FAULTY;
         break;
   }

   return v1StateValue;
}

/*

  Function Name:
    zl303xx_PtpV1handleRecvMsg

  Details:
   Perform actions on a received packet using the v1 protocol

  Parameters:
       clockStream    The PTP clock stream instance structure
       portData       The port data structure
       msg            The received message

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpV1handleRecvMsg(zl303xx_ClockStreamS *clockStream,
                                zl303xx_PortDataS *portData,
                                zl303xx_PtpClockMsgQueueS *msg)
{
   Uint16T versionNumber;

   /* A local area to unpack the packet header for processing. */
   zl303xx_MsgHeaderS rxMsgHeader;

   /* Skip messages that are not version compatible */
   versionNumber = zl303xx_PtpMsgVersionGet(msg->msgBuf);
   if (versionNumber != portData->portDS.versionNumber)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE,1,
            "Received PTP v%d messages but configured for v%d support",
            versionNumber,
            portData->portDS.versionNumber,
            0,0,0,0);
      return;
   }

   /* A clock stream must be configured */
   if (clockStream == NULL)
   {
      ZL303XX_TRACE_ALWAYS("No clock stream configured to handle received PTP messages",
            0,0,0,0,0,0);
      return;
   }

   zl303xx_MsgV1UnpackHeader(msg->msgBuf, &rxMsgHeader);

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4,
                 "Receipt of message type %d  ",
                 rxMsgHeader.controlField, 0,0,0,0,0);

   ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_ENGINE, 7)
   {
      zl303xx_DisplayV1DeviceIdentity("   event received from uuid ",
                                              &rxMsgHeader.u.v1.sourceIdentity);
   }

   /* Ignore self originated messages */
   if (zl303xx_EqualV1Identities(&rxMsgHeader.u.v1.sourceIdentity,
                                 &clockStream->portData->portConfig.u.v1.portIdentity) == ZL303XX_TRUE)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5,
                    "Clock stream %u: zl303xx_PtpV1HandleRecvMsg: ignoring message from self  ",
                    clockStream->clockStreamHandle, 0,0,0,0,0);
      return;
   }

   switch(rxMsgHeader.controlField)
   {
      case PTP_SYNC_MESSAGE:
      {
         zl303xx_PtpV1handleSync(&rxMsgHeader, msg->msgBuf, &msg->recvMsgInfo.rxTs, clockStream);
         break;
      }

      case PTP_DELAY_REQ_MESSAGE:
      {
         zl303xx_PtpV1handleDelayReq(&rxMsgHeader, &msg->recvMsgInfo.rxTs, clockStream);
         break;
      }

      case PTP_FOLLOWUP_MESSAGE:
      {
         zl303xx_PtpV1handleFollowUp(&rxMsgHeader, msg->msgBuf, clockStream);
         break;
      }

      case PTP_DELAY_RESP_MESSAGE:
      {
         zl303xx_PtpV1handleDelayResp(&rxMsgHeader, msg->msgBuf, clockStream);
         break;
      }

      case PTP_MANAGEMENT_MESSAGE:
      {
         zl303xx_PtpV1handleManagement(&rxMsgHeader, msg->msgBuf, clockStream);
         break;
      }

      default:
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                       "Clock stream %u: Unexpected message type %d, seq %d.",
                       clockStream->clockStreamHandle,
                       rxMsgHeader.controlField,
                       rxMsgHeader.sequenceId,
                       0,0,0);
         break;
      }
   }
}

/*

  Function Name:
    zl303xx_PtpV1HandleTsRecordReady

  Details:
   Handles a Timestamp Record Ready event

  Parameters:
       clockStream    The PTP clock stream instance structure
       msg            The received message

  Return Value:
   Nothing

*******************************************************************************/
void zl303xx_PtpV1HandleTsRecordReady(zl303xx_ClockStreamS *clockStream,
                                      zl303xx_PtpTsRecordTblEntryS *tsRecord)
{
   zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;
   zl303xx_TimeStamp txPtpTimestamp = {{0}}, rxPtpTimestamp = {{0}};

   /* Message contains a Delay Request/Response timestamp pair, so process the delay data */
   if (tsRecord->flags & PTP_TS_RECORD_HAVE_RX)
   {
      ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_ENGINE, 1)
      {
         ZL303XX_TRACE_ALWAYS("Stream %u: Delay Req msg tx " PTPD_FORMAT_TIME_STR,
            clockStream->clockStreamHandle, formatTimeStringSign(&tsRecord->txTs),
            formatTimeStringSec(&tsRecord->txTs), formatTimeStringNs(&tsRecord->txTs), 0,0);
         ZL303XX_TRACE_ALWAYS("                        rx " PTPD_FORMAT_TIME_STR,
            formatTimeStringSign(&tsRecord->rxTs), formatTimeStringSec(&tsRecord->rxTs),
            formatTimeStringNs(&tsRecord->rxTs), 0,0,0);
      }

      zl303xx_PtpUpdateDelay(clockStream, &txPtpTimestamp, &rxPtpTimestamp,
                             &tsRecord->rxCorrection, tsRecord->seqId);
   }

   /* Message contains SYNC message actual transmit timestamp, so generate a follow-up */
   else
   {
      /* Increment sequence number */
      ++clockStream->u.v1.last_general_event_sequence_number;

      zl303xx_MsgV1PackFollowUp(clockStream->portData->clock->msgObuf,
                                tsRecord->seqId,
                                &txPtpTimestamp,
                                clockStream->portData->portConfig.u.v1.burst_enabled,
                                clockStream);

      if (zl303xx_PtpTxMsg(pPtpClock->msgObuf, PTP_V1_FOLLOW_UP_PKT_LEN,
                           ZL303XX_PTP_INTERFACE_GENERAL, clockStream->portData,
                           NULL, clockStream, ZL303XX_MSG_ID_FOLLOWUP) != ZL303XX_OK)
      {
         clockStream->portData->lastFaultString = "Error sending Follow_Up packet";
         (void)zl303xx_PtpV1ToPortState(PTP_PORT_INT_STATE_FAULTY, clockStream->portData);
         return;
      }

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4, "Clock stream %u: Sent PTP_FOLLOWUP_MESSAGE message  ",
               clockStream->clockStreamHandle, 0,0,0,0,0);
   }
}

/*

  Function Name:
   zl303xx_PtpV1IssueSync

  Details:
   Issues a sync message

  Parameters:
       clockStream  The PTP clock stream instance structure
       burst        ZL303XX_TRUE if this sync message should have its burst flag set
       followUp     ZL303XX_TRUE if this message uses 2-step messaging

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpV1IssueSync(zl303xx_ClockStreamS *clockStream, zl303xx_BooleanE burst, zl303xx_BooleanE followUp)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_BooleanE bInsertTs;
   zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

   if (followUp)
   {  /* When a follow up is used, sync message should have timestamp set to zero */
      if (pPtpClock->syncUsesSwTs == ZL303XX_TRUE)
      {  /* Insert current PTP time as an inaccurate software timestamp */
         bInsertTs = ZL303XX_FALSE;
      }
      else
      {  /* ZL303XX_ hardware inserts timestamp */
         bInsertTs = ZL303XX_TRUE;
      }
   }
   else
   {
      bInsertTs = ZL303XX_TRUE;
   }

   ++clockStream->u.v1.last_sync_event_sequence_number;

   zl303xx_MsgV1PackSync(pPtpClock->msgObuf, burst, clockStream);

   status = zl303xx_PtpTxMsg(pPtpClock->msgObuf, PTP_V1_SYNC_PKT_LEN,
                             ZL303XX_PTP_INTERFACE_EVENT, clockStream->portData,
                             NULL, clockStream, ZL303XX_MSG_ID_SYNC);

   if (status == ZL303XX_OK)
   {
      if (followUp == ZL303XX_TRUE)
      {
         zl303xx_PtpTsRecordReserve(
                     clockStream,
                     clockStream->u.v1.last_sync_event_sequence_number,
                     zl303xx_PtpV2MsgSequenceNumGet(clockStream->portData->clock->msgObuf));
      }
   }
   else
   {
      /* Go to faulty state */
      clockStream->portData->lastFaultString = "Error sending Sync packet";
      (void)zl303xx_PtpV1ToPortState(PTP_PORT_INT_STATE_FAULTY, clockStream->portData);
   }
}

/*

  Function Name:
   zl303xx_PtpV1IssueSyncBurst

  Details:
   Issues a burst of sync messages

  Parameters:
       pPtpClock     The PTP clock instance structure

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpV1IssueSyncBurst(zl303xx_ClockStreamS *clockStream)
{
  Sint32T count;
  zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

  for (count=0; count<BURST_SIZE; count++)
  {
    zl303xx_PtpV1IssueSync(clockStream, ZL303XX_TRUE, pPtpClock->defaultDSInt.u.v1.clock_followup_capable);
  }
}

/*

  Function Name:
   zl303xx_PtpV1IssueDelayReq

  Details:
   Issues a delay request message

  Parameters:
       clockStream The PTP clock stream instance structure
       burst_req   ZL303XX_TRUE if this delay request should also request a burst of syncs

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpV1IssueDelayReq(zl303xx_ClockStreamS *clockStream, zl303xx_BooleanE burst_req)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_BooleanE burst = ZL303XX_FALSE;
   zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

   if (burst_req && clockStream->portData->portConfig.u.v1.burst_enabled)
   {
      burst = ZL303XX_TRUE;
   }

   zl303xx_MsgV1PackDelayReq(pPtpClock->msgObuf, burst, clockStream);

   status = zl303xx_PtpTxMsg(pPtpClock->msgObuf, PTP_V1_DELAY_REQ_PKT_LEN,
                             ZL303XX_PTP_INTERFACE_EVENT, clockStream->portData,
                             NULL, clockStream, ZL303XX_MSG_ID_PEER_DELAY_REQ);

   if (status == ZL303XX_OK)
   {
      zl303xx_PtpTsRecordReserve(
                  clockStream,
                  clockStream->u.v1.last_sync_event_sequence_number,
                  zl303xx_PtpV2MsgSequenceNumGet(clockStream->portData->clock->msgObuf));
   }
   /* Nothing else to do at this time. The timing information associated with the delay request will be processed when the
      delay response is received. */
   else
   {
      /* Go to faulty state */
      clockStream->portData->lastFaultString = "Error sending Delay_Req packet";
      (void)zl303xx_PtpV1ToPortState(PTP_PORT_INT_STATE_FAULTY, clockStream->portData);
   }
}

/*

  Function Name:
   zl303xx_PtpV1IssueDelayResp

  Details:
   Issues a delay response message

  Parameters:
       delayReqRecvTime  The receive time of the corresponding delay_req message
       header            The header of the corresponding delay_req message
       clockStream       The PTP clock stream instance structure

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpV1IssueDelayResp(zl303xx_TimeStamp *delayReqRecvTime, zl303xx_MsgHeaderS *header, zl303xx_ClockStreamS *clockStream)
{
   zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

  ++clockStream->u.v1.last_general_event_sequence_number;

  zl303xx_MsgV1PackDelayResp(pPtpClock->msgObuf, header, delayReqRecvTime, clockStream);

  if (zl303xx_PtpTxMsg(pPtpClock->msgObuf, PTP_V1_DELAY_RESP_PKT_LEN,
                       ZL303XX_PTP_INTERFACE_GENERAL, clockStream->portData,
                       NULL, clockStream, ZL303XX_MSG_ID_PEER_DELAY_RESP) != ZL303XX_OK)
  {
    clockStream->portData->lastFaultString = "Error sending Delay_Resp packet";
    (void)zl303xx_PtpV1ToPortState(PTP_PORT_INT_STATE_FAULTY, clockStream->portData);
    return;
  }

  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4, "Clock stream %u event: Send PTP_DELAY_RESP_MESSAGE Message",
                  clockStream->clockStreamHandle, 0,0,0,0,0);
}

/*

  Function Name:
   zl303xx_PtpV1IssueManagement

  Details:
   Issues a V1 management message

  Parameters:
       header        The header of the corresponding delay_req message
       manage        Management message structure
       clockStream   The PTP clock stream instance structure

  Return Value:
   Nothing

 *****************************************************************************/

void zl303xx_PtpV1IssueManagement(zl303xx_MsgHeaderS *header, MsgV1Management *manage,
                                  zl303xx_ClockStreamS *clockStream)
{
  Uint16T len;
  zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

  ++clockStream->u.v1.last_general_event_sequence_number;

  if ((len = zl303xx_MsgV1PackManagement(pPtpClock->msgObuf, header, manage, clockStream)) == 0)
    return;

  if (zl303xx_PtpTxMsg(pPtpClock->msgObuf, len,
                       ZL303XX_PTP_INTERFACE_GENERAL, clockStream->portData,
                       NULL, clockStream, ZL303XX_MSG_ID_MANAGEMENT) != ZL303XX_OK)
  {
    clockStream->portData->lastFaultString = "Error sending Management message";
    (void)zl303xx_PtpV1ToPortState(PTP_PORT_INT_STATE_FAULTY, clockStream->portData);
    return;
  }

  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5, "Clock stream %u: Sent management message type %d  ",
                                                clockStream->clockStreamHandle, manage->managementMessageKey, 0,0,0,0);
}


/*

  Function Name:
   zl303xx_PtpV1InitStreamData

  Details:
   Initialises the data structure for a specific port on the PTP Clock

  Parameters:
       clockStream    The PTP clock stream instance structure

  Return Value:
   Nothing

  Notes:
   Only initialises the data that must be reinitialised on a warm start.
   Cold-start data is initialised by zl303xx_PtpPortCreate.

*******************************************************************************/
void zl303xx_PtpV1InitStreamData(zl303xx_ClockStreamS *clockStream)
{
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
         "zl303xx_PtpV1InitStreamData: Clock stream %u:",
         clockStream->clockStreamHandle, 0,0,0,0,0);

   /* Clear the version independent variables */
   zl303xx_PtpStreamInitGenericCalcValues(clockStream);

   /* Initialise version specific members */
   {
      clockStream->farEndParentDS.u.v1.parent_last_sync_sequence_number = 0;

      /* Q and R timer values are 0 initially */
      clockStream->u.v1.Q = 0;
      clockStream->u.v1.R = 0;

      clockStream->farEndParentDS.u.v1.parent_two_step_clock = ZL303XX_FALSE;
      clockStream->farEndParentDS.u.v1.grandmaster_is_boundary_clock = ZL303XX_FALSE;
   }
}


/*

  Function Name:
   zl303xx_PtpV1AddForeign

  Details:
   Adds a foreign master to the list of visible masters (for PTP v1)

  Parameters:
       buf         Pointer to the raw packet buffer
       header      Pointer to the unpacked header contents
       clockStream    The PTP clock stream instance structure

  Return Value:
   Nothing

 *****************************************************************************/
MsgV1Sync * zl303xx_PtpV1AddForeign(Uint8T *buf, zl303xx_MsgHeaderS *header, zl303xx_ClockStreamS *clockStream)
{
  Sint32T count, j;
  Sint32T emptyRecord = ZL303XX_PTP_FMT_INDEX_INVALID;
  zl303xx_BooleanE found = ZL303XX_FALSE;

  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4, "Clock stream %u: updateForeign  ",clockStream->clockStreamHandle, 0,0,0,0,0);

  /* Start search with the best foreign record from last time since this is most likely to
      be the current master */
  if (pPtpClock->foreignMasterDS.bestRecord != ZL303XX_PTP_FMT_INDEX_INVALID)
  {
    j = pPtpClock->foreignMasterDS.bestRecord;
  }
  else
  {
    j = 0;
  }
  for(count = 0; count < pPtpClock->foreignMasterDS.numActiveRecords; ++count)
  {
    if (zl303xx_EqualV1Identities(&header->u.v1.sourceIdentity, &pPtpClock->foreignMasterDS.foreign[j].v1.foreignMasterIdentity))
      /* N.b. should really also ignore the incoming message if its older than the previous */
    {
      ++pPtpClock->foreignMasterDS.foreign[j].msgCount.current;
      found = ZL303XX_TRUE;
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4, "Clock stream %u: updateForeign: update record %d  ",clockStream->clockStreamHandle, j, 0,0,0,0);
      break;
    }
    else if ((pPtpClock->foreignMasterDS.foreign[j].msgCount.previous < ZL303XX_PTP_DEFAULT_FOREIGN_WINDOW_THRESHOLD) &&
             (pPtpClock->foreignMasterDS.foreign[j].msgCount.current == 0))
    { /* This entry is no longer qualified nor active and can be overwritten if required */
      if (emptyRecord == ZL303XX_PTP_FMT_INDEX_INVALID)
      {
        emptyRecord = j;
      }
    }

    j = (j + 1)%pPtpClock->foreignMasterDS.numActiveRecords;
  }

  if (!found)
  {
    if (pPtpClock->foreignMasterDS.numActiveRecords < pPtpClock->foreignMasterDS.maxRecords)
    { /* Add to end of array */
      j = pPtpClock->foreignMasterDS.numActiveRecords;
      ++pPtpClock->foreignMasterDS.numActiveRecords;
    }
    else
    { /* Overwrite an old record */
      if (emptyRecord == ZL303XX_PTP_FMT_INDEX_INVALID)
      {
        ZL303XX_ERROR_NOTIFY("No free foreign record entry. Discarding Sync");
        return NULL;
      }
      else
      {
        j = emptyRecord;
      }
    }


    pPtpClock->foreignMasterDS.foreign[j].v1.foreignMasterIdentity = header->u.v1.sourceIdentity;
    pPtpClock->foreignMasterDS.foreign[j].msgCount.previous = 0;
    pPtpClock->foreignMasterDS.foreign[j].msgCount.current = 1;

    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5, "Clock stream %u: updateForeign: new record (%d,%d) %d %d  ",
      clockStream->clockStreamHandle,
      j, pPtpClock->foreignMasterDS.numActiveRecords,
      pPtpClock->foreignMasterDS.foreign[j].v1.foreignMasterIdentity.communicationTechnology,
      pPtpClock->foreignMasterDS.foreign[j].v1.foreignMasterIdentity.portNumber, 0);

    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5, "   %02hx:%02hx:%02hx:%02hx:%02hx:%02hx  ",
      pPtpClock->foreignMasterDS.foreign[j].v1.foreignMasterIdentity.uuid[0], pPtpClock->foreignMasterDS.foreign[j].v1.foreignMasterIdentity.uuid[1],
      pPtpClock->foreignMasterDS.foreign[j].v1.foreignMasterIdentity.uuid[2], pPtpClock->foreignMasterDS.foreign[j].v1.foreignMasterIdentity.uuid[3],
      pPtpClock->foreignMasterDS.foreign[j].v1.foreignMasterIdentity.uuid[4], pPtpClock->foreignMasterDS.foreign[j].v1.foreignMasterIdentity.uuid[5]);
  }

  zl303xx_MsgV1UnpackHeader(buf, &pPtpClock->foreignMasterDS.foreign[j].v1.header);
  zl303xx_MsgV1UnpackSync(buf, &pPtpClock->foreignMasterDS.foreign[j].v1.sync);

  return &pPtpClock->foreignMasterDS.foreign[j].v1.sync;
}


/*

  Function Name:
    zl303xx_PtpV1handleSync

  Details:
   Handle a received PTP v1 sync message

  Parameters:
       header        The common header of the received message
       msgIbuf       The sync message content
       rxTime        The received timestamp of the sync message
       clockStream   Pointer to the clock stream instance structure

  Return Value:
   nothing

 *****************************************************************************/

void zl303xx_PtpV1handleSync(zl303xx_MsgHeaderS *header, Uint8T *msgIbuf, zl303xx_TimeStamp *rxTime, zl303xx_ClockStreamS *clockStream)
{
   MsgV1Sync *sync = NULL;
   zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3, "Clock stream %u: Received SYNC event",clockStream->clockStreamHandle,0,0,0,0,0);
   switch(clockStream->streamSubstate)
   {
      case PTP_STREAM_SUBSTATE_UNCALIBRATED:
      case PTP_STREAM_SUBSTATE_SLAVE:
         /* For version 1, disregard incoming sync messages forming part of a burst unless we have been enabled to support them */
         if (getFlag(header->flagField, PTP1_FLAGS_SYNC_BURST) && !clockStream->portData->portConfig.u.v1.burst_enabled)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3, "Clock stream %u: Disregarding burst sync message  ",clockStream->clockStreamHandle, 0,0,0,0,0);
            return;
         }
         /* addForeign() takes care of msgUnpackSync() and it returns a valid sync pointer if the message is qualified or NULL if not */
         pPtpClock->foreignMasterDS.tableUpdated = ZL303XX_TRUE;
         sync = zl303xx_PtpV1AddForeign(msgIbuf, header, clockStream);

         ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_ENGINE, 5)
         {
            zl303xx_DisplayV1DeviceIdentity("handleSync: looking for parent uuid ", &clockStream->farEndParentDS.u.v1.parentIdentity);
         }

         /* Is this a unicast clock stream in which we can accept all sync messages received?
            Or else is the sync msg from the current master?
            And in either case is this the latest message? */
         if (((clockStream->config.unicast == ZL303XX_TRUE) ||
              (zl303xx_EqualV1Identities(&header->u.v1.sourceIdentity , &clockStream->farEndParentDS.u.v1.parentIdentity) == ZL303XX_TRUE)) &&
             (header->sequenceId != clockStream->farEndParentDS.u.v1.parent_last_sync_sequence_number) &&
             (header->sequenceId >= (Uint16T)(clockStream->farEndParentDS.u.v1.parent_last_sync_sequence_number + 1)))
         {
            if (sync)
            {  /* sync is qualified so continue processing */
               if (sync->logSyncInterval != clockStream->config.logSyncInterval)
               {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4, "Clock stream %u: message's sync interval is %d, but clock's is %d  ",
                                    clockStream->clockStreamHandle,
                                    sync->logSyncInterval, clockStream->config.logSyncInterval, 0,0,0);
                  /* spec section 7.5.18 recommends a sync interval discrepancy should be a fault. */
                  /* spec section 6.2.5.5 says that "the behaviour of PTP sub-domains where clocks do not all have the
                    same value of sync interval is outside the scope of this standard."
                    However, this implementation can tolerate asymmetrical sync rates without problem. */
               }


               {
                  Uint64S zeroCorrection = {0,0};
                  zl303xx_BooleanE done = ZL303XX_FALSE;

                  if (!getFlag(header->flagField, PTP1_FLAGS_ASSIST))
                  {  /* There is no follow-up so we have all the information we need to process the timestamps */
                      zl303xx_PtpUpdateOffset(clockStream, &sync->originTimestamp, rxTime,
                                             &zeroCorrection, header->sequenceId);
                     done = ZL303XX_TRUE;
                  }
                  else
                  {  /* There should be a follow-up for this message */
                     /* Have we already received the follow-up? (since packets might arrive out of order) */
                     done = zl303xx_PtpV1TwoStepRegisterRx(clockStream, header->sequenceId,
                                                           rxTime, &header->u.v1.sourceIdentity);
                  }

                  if (done == ZL303XX_TRUE)
                  {  /* State change S1 */
                     zl303xx_PtpV1DoS1Update(header, sync, clockStream);
                  }

                  /* Store the latest receipt time for the drift calculation */
                  clockStream->drift.latestSyncRxTime = *rxTime;
               }

               /* If useStandardDelayReq flag is ZL303XX_TRUE then use standard IEEE-1588-2002 behaviour for delay_req
                  which does a count of R sync cycles before sending next delay_req. */
               if (pPtpClock->useStandardDelayReq)
               {
                  if (clockStream->u.v1.R > 0)
                     clockStream->u.v1.R--;

                  if (!clockStream->u.v1.R)
                  {
                     /* Have waited for R sync messages */
                     /* The spec requires us to wait a further interval after receiving the 'R'th sync message
                        which is given by Q * PTP_SYNC_INTERVAL_TIMEOUT / PTP_RANDOMIZING_SLOTS seconds */
                     /* The following calculation provides an approximation to this but due to the truncation it is
                        not precise. This is probably good enough since the intention of the spec is (I believe) to
                        spread the occurrence of delay_req messages over time when there are multiple slaves */
                     zl303xx_TimeStamp interval;
                     Uint32T divisor;

                     /* N.B. Present version of PTPd no longer uses the Q timer and so sends
                      the delay request immediately the R timer expires. However, we still
                      use the Q timer to provide a more conformant 1588 implementation */
                     if (clockStream->u.v1.Q < 2)
                     {
                        /* Spec requires Q >= 2 */
                        ZL303XX_ERROR_TRAP("Invalid Q value  ");
                     }
                     else
                     {
                        /* If the sync interval period is < 1s (which is not supported by the current spec anyway)
                           then this gets complicated */
                        zl303xx_PtpGetSyncIntervalTimeout(clockStream, &interval);
                        divisor = PTP_RANDOMIZING_SLOTS / clockStream->u.v1.Q;
                        if (divisor > 0)
                        {
                          divTime(&interval, &interval, PTP_RANDOMIZING_SLOTS);
                        }
                        /* Avoid setting a zero timer as this will never start and therefore never expire */
                        if (isZeroTime(&interval))
                        {  /* Set one system tick instead */
                          interval.nanosecondsField = TEN_e9 / OS_TICK_RATE_GET();
                        }

                        (void)zl303xx_PtpTimerStart(pPtpClock->clockHandle,
                                                    &clockStream->itimer[Q_TIMER],
                                                    &interval, ZL303XX_FALSE);
                     }
                  }
               }
               else /* Use shortened interval between delay_reqs */
               {
                  /* Wait initially until R sync messages have been seen */
                  if (clockStream->u.v1.R > 0)
                  {
                     clockStream->u.v1.R--;
                  }

                  if ((clockStream->u.v1.R == 0) &&
                      (zl303xx_PtpTimerActive(&clockStream->itimer[Q_TIMER]) == ZL303XX_FALSE))
                  {
                     (void)zl303xx_PtpStreamTimerStart(clockStream, Q_TIMER);
                  }
               }
            }

            /* Restart the sync receipt timer */
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4, "Clock stream %u: SYNC_RECEIPT_TIMER reset  ",clockStream->clockStreamHandle, 0,0,0,0,0);
            (void)zl303xx_PtpStreamTimerStart(clockStream, SYNC_RECEIPT_TIMER);
         }
         break;

      case PTP_STREAM_SUBSTATE_MASTER:
      default:
         if ( header->u.v1.sourceIdentity.communicationTechnology == pPtpClock->defaultDSInt.u.v1.clockIdentity.communicationTechnology
            || header->u.v1.sourceIdentity.communicationTechnology == PTP_DEFAULT
            || pPtpClock->defaultDSInt.u.v1.clockIdentity.communicationTechnology == PTP_DEFAULT )
         {
            pPtpClock->foreignMasterDS.tableUpdated = ZL303XX_TRUE;
            (void)zl303xx_PtpV1AddForeign(msgIbuf, header, clockStream);
         }
         break;
   }
}

/*

  Function Name:
    zl303xx_PtpV1handleFollowUp

  Details:
   Handle a received PTP v1 follow-up message

  Parameters:
       header         The common header of the received message
       msgIbuf        Pointer to the raw message buffer
       clockStream    The PTP clock stream instance structure

  Return Value:
   nothing

 *****************************************************************************/
void zl303xx_PtpV1handleFollowUp(zl303xx_MsgHeaderS *header,
                                 Uint8T *msgIbuf,
                                 zl303xx_ClockStreamS *clockStream)
{
   MsgV1FollowUp follow;
   zl303xx_BooleanE acceptPkt = ZL303XX_TRUE;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
                 "Clock stream %u: Received PTP_FOLLOW_UP msg",
                 clockStream->clockStreamHandle, 0,0,0,0,0);

   switch(clockStream->streamSubstate)
   {
      default:
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5,
                       "Stream %u: zl303xx_PtpV1handleFollowUp: disregard  ",
                       clockStream->clockStreamHandle, 0,0,0,0,0);
         return;
      }

      case PTP_STREAM_SUBSTATE_UNCALIBRATED:
      case PTP_STREAM_SUBSTATE_SLAVE:
      {
      /* Determine whether to accept the packet based on current state */
         if (clockStream->config.unicast == ZL303XX_FALSE)
         {
            /* Multicast stream */
            if ((clockStream->streamSubstate != PTP_STREAM_SUBSTATE_UNCALIBRATED) &&
                (clockStream->streamSubstate != PTP_STREAM_SUBSTATE_MASTER) &&
                (clockStream->streamSubstate != PTP_STREAM_SUBSTATE_SLAVE))
            {
               acceptPkt = ZL303XX_FALSE;
            }
         }
         else
         {  /* unicast stream. Can only accept follow-ups if we are receiving syncs or requesting renewal of an existing sync contract */
            if (clockStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].remainingSec > 0)
            {
               /* Accept the packet */
            }
            else
            {
               acceptPkt = ZL303XX_FALSE;
            }
         }

         if (acceptPkt == ZL303XX_TRUE)
         {
            /* Is this a unicast clock stream in which we can accept all sync messages received?
             * Or, is the msg from the current master? */
            if ((clockStream->config.unicast == ZL303XX_FALSE) &&
                (zl303xx_EqualV1Identities(&header->u.v1.sourceIdentity, &clockStream->farEndParentDS.u.v1.parentIdentity) != ZL303XX_TRUE))
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5,
                             "Clock stream %u: Disregard follow-up from unknown master  ",
                             clockStream->clockStreamHandle, 0,0,0,0,0);
               return;
            }

            /* Disregard incoming follow-up messages forming part of a burst unless we have been enabled to support them */
            if (getFlag(header->flagField, PTP1_FLAGS_SYNC_BURST) && !clockStream->portData->portConfig.u.v1.burst_enabled)
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
                             "Clock stream %u: Disregard burst follow-up msg  ",
                             clockStream->clockStreamHandle, 0,0,0,0,0);
               return;
            }

            zl303xx_MsgV1UnpackFollowUp(msgIbuf, &follow);

            zl303xx_PtpV1TwoStepRegisterTx(clockStream, follow.associatedSequenceId,
                                           &follow.preciseOriginTimestamp,
                                           &header->u.v1.sourceIdentity);
         }
            break;
         }
   }
}

/* zl303xx_PtpV1TwoStepRegisterTx */
/** 

   Registers the txTs of a Follow_Up message. If the corresponding Sync (rxTs)
   already exists, the time stamp pair is processed. Otherwise, the txTs is
   saved in the stream's two-step table to be matched later.

  Parameters:
   [in]   stream      Pointer to the stream the Follow_Up was received on.
   [in]   seqId       sequenceId from the Follow_Up
   [in]   txTs        preciseOriginTimestamp from the Follow_Up
   [in]   srcPortId   sourcePortIdentity from the Follow_Up

*******************************************************************************/
static void zl303xx_PtpV1TwoStepRegisterTx(zl303xx_ClockStreamS *stream, Uint16T seqId,
                                           zl303xx_TimeStamp *txTs,
                                           V1PortIdentityS *srcPortId)
{
   Uint64S zero64 = {0,0};
   zl303xx_PtpTwoStepRecordS *record =
         &stream->twoStepRecordArray[seqId & stream->twoStepRecordIdxMask];

   if (record->flags & PTP_TS_RECORD_HAVE_RX)
   {
      if ((record->seqId == seqId) &&
          (zl303xx_EqualV1Identities(&record->u.v1.srcPortId, srcPortId)))
      {
         zl303xx_PtpUpdateOffset(stream, txTs, &record->rxTs, &zero64, seqId);
         record->flags = PTP_TS_RECORD_EMPTY;
      }
   }

   if (record->flags != PTP_TS_RECORD_EMPTY)
   {
      /* The record is not empty, so overwrite it */
      ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 1,
            "Stream %u: Two-step record [TX] seqId=%u overwrite (prev: seqId=%u, flags=0x%0X)",
            stream->clockStreamHandle, seqId, record->seqId, record->flags, 0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %u: Two-step record [TX] seqId=%u overwrite (prev: seqId=%u, flags=0x%0X)",
            stream->clockStreamHandle, seqId, record->seqId, record->flags, 0,0);
   }
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 2,
            "Stream %u: Two-step record [TX] seqId=%u added (early Follow_Up)", stream->clockStreamHandle, seqId, 0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
            "Stream %u: Two-step record [TX] seqId=%u added (early Follow_Up)", stream->clockStreamHandle, seqId, 0,0,0,0);
   }

   record->seqId = seqId;
   record->txTs = *txTs;
   record->u.v1.srcPortId = *srcPortId;
   record->flags = PTP_TS_RECORD_HAVE_TX;
}

/* zl303xx_PtpStreamTwoStepRegisterRx */
/** 

   Registers the rxTs of a two-step Sync message. If the corresponding
   Follow_Up's txTs already exists, the time stamp pair is processed. Otherwise,
   the rxTs is saved in the stream's two-step table to be matched later.

  Parameters:
   [in]   stream      Pointer to the stream the Follow_Up was received on.
   [in]   seqId       sequenceId from the Follow_Up
   [in]   rxTs        preciseOriginTimestamp from the Follow_Up
   [in]   srcPortId   sourcePortIdentity from the Follow_Up

*******************************************************************************/
static zl303xx_BooleanE zl303xx_PtpV1TwoStepRegisterRx(zl303xx_ClockStreamS *stream,
                                                       Uint16T seqId,
                                                       zl303xx_TimeStamp *rxTs,
                                                       V1PortIdentityS *srcPortId)
{
   Uint64S zero64 = {0,0};
   zl303xx_PtpTwoStepRecordS *record =
         &stream->twoStepRecordArray[seqId & stream->twoStepRecordIdxMask];

   if (record->flags & PTP_TS_RECORD_HAVE_TX)
   {
      if ((record->seqId == seqId) &&
          (zl303xx_EqualV1Identities(&record->u.v1.srcPortId, srcPortId)))
      {
         zl303xx_PtpUpdateOffset(stream, &record->txTs, rxTs, &zero64, seqId);
         record->flags = PTP_TS_RECORD_EMPTY;
         return ZL303XX_TRUE;
      }
   }

   if (record->flags != PTP_TS_RECORD_EMPTY)
   {
      /* The record is not empty, so overwrite it */
      ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 1,
            "Stream %u: Two-step record [RX] seqId=%u overwrite (prev: seqId=%u, flags=0x%0X)",
            stream->clockStreamHandle, seqId, record->seqId, record->flags, 0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %u: Two-step record [RX] seqId=%u overwrite (prev: seqId=%u, flags=0x%0X)",
            stream->clockStreamHandle, seqId, record->seqId, record->flags, 0,0);
   }
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 2,
            "Stream %u: Two-step record [RX] seqId=%u added", stream->clockStreamHandle, seqId, 0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
            "Stream %u: Two-step record [RX] seqId=%u added", stream->clockStreamHandle, seqId, 0,0,0,0);
   }

   record->seqId = seqId;
   record->rxTs = *rxTs;
   record->u.v1.srcPortId = *srcPortId;
   record->flags = PTP_TS_RECORD_HAVE_RX;
   return ZL303XX_FALSE;
}

/*

  Function Name:
    zl303xx_PtpV1handleDelayReq

  Details:
   Handle a received PTP v1 delay request message

  Parameters:
       header         The common header of the received message
       rxTime         The received timestamp of the delay_req message
       clockStream    The PTP clock stream instance structure

  Return Value:
   nothing

 *****************************************************************************/

void zl303xx_PtpV1handleDelayReq(zl303xx_MsgHeaderS *header, zl303xx_TimeStamp *rxTime, zl303xx_ClockStreamS *clockStream)
{
   if (clockStream->streamSubstate == PTP_STREAM_SUBSTATE_MASTER)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
                    "Clock stream %u: Received DELAY_REQ event  ",
                    clockStream->clockStreamHandle, 0,0,0,0,0);

      zl303xx_PtpV1IssueDelayResp(rxTime, header, clockStream);

      if ((getFlag(header->flagField, PTP1_FLAGS_SYNC_BURST)) &&                 /* Requester has asked for a sync burst, */
         (clockStream->portData->portConfig.u.v1.burst_enabled) &&                        /* and we are enabled to support bursts, */
         (zl303xx_PtpTimerExpired(&clockStream->itimer[SYNC_BURST_DELAY_TIMER]) == ZL303XX_TRUE))  /* and minimum time between bursts has passed */
      {
         zl303xx_TimeStamp interval;

         /* send a burst now */
         zl303xx_PtpV1IssueSyncBurst(clockStream);

         /* Restart timer to ensure a gap before sending the next burst.
            In this implementation the gap will be one system tick */
         interval.secondsField.hi = 0;
         interval.secondsField.lo = 0;
         interval.nanosecondsField = TEN_e9 / OS_TICK_RATE_GET();   /* Time one tick */
         (void)zl303xx_PtpTimerStart(clockStream->portData->clock->clockHandle,
                                     &clockStream->itimer[SYNC_BURST_DELAY_TIMER],
                                     &interval, ZL303XX_TRUE);
      }
   }
}

/*

  Function Name:
    zl303xx_PtpV1handleDelayResp

  Details:
   Handle a received PTP v1 delay response message

  Parameters:
       header         The common header of the received message
       msgIbuf        Pointer to the raw received message buffer
       clockStream    The PTP clock stream instance structure

  Return Value:
   nothing

 *****************************************************************************/

void zl303xx_PtpV1handleDelayResp(zl303xx_MsgHeaderS *header,
                                  Uint8T *msgIbuf,
                                  zl303xx_ClockStreamS *clockStream)
{
   MsgV1DelayResp resp;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
                 "Clock stream %u: Received PTP_DELAY_RESP msg",
                 clockStream->clockStreamHandle, 0,0,0,0,0);

   switch(clockStream->streamSubstate)
   {
      default:
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5,
                       "Stream %u: zl303xx_PtpV1handleDelayResp: disregard  ",
                       clockStream->clockStreamHandle, 0,0,0,0,0);
         return;
      }

      case PTP_STREAM_SUBSTATE_UNCALIBRATED:
      case PTP_STREAM_SUBSTATE_SLAVE:
      {
         zl303xx_MsgV1UnpackDelayResp(msgIbuf, &resp);

            /* Check the message is from our parent and is in response to our own delay_response. We could also check the sequence
               number to ensure it is the latest but this is not an important check for us */
         if ((zl303xx_EqualV1Identities(&header->u.v1.sourceIdentity, &clockStream->farEndParentDS.u.v1.parentIdentity) == ZL303XX_TRUE) &&
               zl303xx_EqualV1Identities(&resp.requestingSourceIdentity, &clockStream->portData->portConfig.u.v1.portIdentity))
         {
            /* Register the delay response information with the timestamp record handler module.
               If there is transmit timestamp information available for a corresponding delay request
               then this will trigger a timestamp record ready event which will cause the complete
               request/response cycle to be subsequently processed */
            Uint64S zeroCorrection = {0,0};

            zl303xx_PtpTsRecordRegisterIngress(clockStream, NULL,
                                           resp.requestingSourceSequenceId,
                                           &resp.delayReceiptTimestamp,
                                           &zeroCorrection);
         }

         break;
   }
}
}

/*

  Function Name:
    zl303xx_PtpV1handleManagement

  Details:
   Handle a received PTP v1 management message

  Parameters:
       header         The common header of the received message
       msgIbuf        Pointer to the raw received management message
       clockStream    The PTP clock stream instance structure

  Return Value:
   nothing

 *****************************************************************************/

void zl303xx_PtpV1handleManagement(zl303xx_MsgHeaderS *header, Uint8T *msgIbuf, zl303xx_ClockStreamS *clockStream)
{
   MsgV1Management manage;
   zl303xx_PtpPortInternalStateE nextState;
   zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

   if (clockStream->streamSubstate == PTP_STREAM_SUBSTATE_INIT)
      return;

   zl303xx_MsgV1UnpackManagement(msgIbuf, &manage);

   if ((manage.targetIdentity.communicationTechnology == pPtpClock->defaultDSInt.u.v1.clockIdentity.communicationTechnology
        && !memcmp(manage.targetIdentity.uuid, pPtpClock->defaultDSInt.u.v1.clockIdentity.uuid, PTP_V1_UUID_LENGTH))
       || ((manage.targetIdentity.communicationTechnology == PTP_DEFAULT ||
            manage.targetIdentity.communicationTechnology == pPtpClock->defaultDSInt.u.v1.clockIdentity.communicationTechnology)
         && manage.targetIdentity.uuid[0] == '\0') )
   {
      switch(manage.managementMessageKey)
      {
      case PTP_MM_OBTAIN_IDENTITY:
      case PTP_MM_GET_DEFAULT_DATA_SET:
      case PTP_MM_GET_CURRENT_DATA_SET:
      case PTP_MM_GET_PARENT_DATA_SET:
      case PTP_MM_GET_PORT_DATA_SET:
      case PTP_MM_GET_GLOBAL_TIME_DATA_SET:
      case PTP_MM_GET_FOREIGN_DATA_SET:
         {
            zl303xx_PtpV1IssueManagement(header, &manage, clockStream);

         break;
         }

      default:
         {
         pPtpClock->foreignMasterDS.tableUpdated = ZL303XX_TRUE;
            nextState = zl303xx_MsgV1UnloadManagement(msgIbuf, &manage, clockStream);
         if (nextState != clockStream->portData->portSuperState)
            (void)zl303xx_PtpV1ToPortState(nextState, clockStream->portData);

         break;
      }
   }
}
}

/*

  Function Name:
    zl303xx_EqualV1Identities

  Details:
   Determines whether two clock identities are equal or not

  Parameters:
       first         Pointer to first clock identity
       second        Pointer to second clock identity


  Return Value:
   zl303xx_BooleanE

  Notes:
   As this is a local routine, it assumes that pointers have been verified.

 *****************************************************************************/
zl303xx_BooleanE zl303xx_EqualV1Identities(V1PortIdentityS *first,
                                         V1PortIdentityS *second)
{
   if ((first->communicationTechnology == second->communicationTechnology) &&
       (first->portNumber == second->portNumber) &&
      (memcmp(first->uuid, second->uuid, PTP_V1_UUID_LENGTH) == 0))
   {
      return ZL303XX_TRUE;  /* identities are equal */
   }
   else
   {
      return ZL303XX_FALSE;
   }
}

/*

  Function Name:
    zl303xx_DisplayV1DeviceIdentity

  Details:
   Displays a device identity value for PTP version 1

  Parameters:
       string         A preliminary string to display
       deviceIdentity The device identity to display


  Return Value:
   Nothing

 *****************************************************************************/

void zl303xx_DisplayV1DeviceIdentity(const char *string, V1PortIdentityS *deviceIdentity)
{
   ZL303XX_TRACE_ALWAYS("%s...", string, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("...uuid: %02hx:%02hx:%02hx:%02hx:%02hx:%02hx",
                         deviceIdentity->uuid[0], deviceIdentity->uuid[1], deviceIdentity->uuid[2],
                         deviceIdentity->uuid[3], deviceIdentity->uuid[4], deviceIdentity->uuid[5]);
   ZL303XX_TRACE_ALWAYS(" ...port number: %02hx:%02hx", deviceIdentity->portNumber >> 8, deviceIdentity->portNumber & 0xFF, 0,0,0,0);
}


/*

  Function Name:
   getRand

  Details:
   Returns a pseudo-random number based on the supplied seed

  Parameters:
       seed        Pointer to a seed

  Return Value:
   Pseudo random number

 *****************************************************************************/
Uint16T getRand(Uint32T *seed)
{
  (void)srand(*seed);
  return rand();
}

#endif   /* ZL303XX_INCLUDE_PTP_V1 */
