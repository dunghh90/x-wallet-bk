

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains the unicast protocol state machines for the
*     IEEE-1588-2008 version 2 PTP protocol
*
*******************************************************************************/

/****************   INCLUDE FILES   *******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"

#include "zl303xx_Ptsf.h"

#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpStrings.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpV2MsgParse.h"
#include "zl303xx_PtpV2MgmtTlvHandle.h"
#include "zl303xx_PtpV2AcceptMaster.h"
#include "zl303xx_PtpProfileDef.h"
#include "zl303xx_PtpG8265p1Api.h"

/****************   DEFINES     ***********************************************/
/* In the event of receiving no acknowledge for a cancel message then try a total
   of 5 times */
#define MAX_NUM_CANCEL_ATTEMPTS 5

/* A unicast contract should be renewed before it runs out. The following constant
   is the time in seconds to start the renewal process before the contract expires.
   It is recommended to be at least 3 x the renewal timeout period which in this
   implementation is 1s. */
#define CONTRACT_RENEW_ADVANCE_MIN      (Uint32T)2
#define CONTRACT_RENEW_ADVANCE_MAX      (Uint32T)5  /* Actually (duration - 5) */

/* Trace macro that logs to module ZL303XX_MOD_ID_PTP_UNICAST, and prepends the
 * string "Stream x:" or "Port x:" if the stream pointer is NULL. Up to 5
 * parameters can be used in fmt. */
#define UNI_NEG_TRACE(lvl, port, stream, fmt, arg1, arg2, arg3, arg4, arg5) \
   do { \
      if ((stream) != NULL) { \
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, lvl, "Stream %lu: " fmt, \
            (stream)->clockStreamHandle, arg1, arg2, arg3, arg4, arg5); \
      } else { \
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, lvl, "Port %lu:   " fmt, \
            (port)->clockPortHandle, arg1, arg2, arg3, arg4, arg5); \
      } \
   } while (0)

/* Trace macro that logs contract-specific info to module ZL303XX_MOD_ID_PTP_UNICAST,
 * and prepends the Up to 4 parameters can be used in fmt. */
#define UNI_NEG_CONTRACT_TRACE(lvl, pStream, conType, msgType, fmt, arg1, arg2, arg3, arg4) \
           ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, lvl, \
              "Stream %lu: Contract [" conType "] %-14s : " fmt, \
              pStream->clockStreamHandle, GET_UNICAST_MSG_STR(msgType), \
              arg1, arg2, arg3, arg4)

/****************   DATA TYPES   **********************************************/

/* Structure to define properties and routines for individual unicast
 * negotiation TLVs. */
typedef struct
{
   /* If a stream is required, and NULL is passed into zl303xx_PtpV2SignalUnicastNegTlvHandle(),
    * the process() binding will not be called. */
   zl303xx_BooleanE streamRequired;

   /* Function to process a received TLV. Must not be NULL. */
   zlStatusE (*process)(zl303xx_PtpClockMsgQueueS *ptpData,
                        zl303xx_PtpV2SignalTlvS *rxTlv,
                        zl303xx_PortDataS *pPort,
                        zl303xx_ClockStreamS **ppStream,
                        zl303xx_PtpV2SignalTlvS *txTlv);
} UniNegTlvDefS;

/****************   STATIC FUNCTION DECLARATIONS   ****************************/
static zlStatusE zl303xx_PtpUniNegReqHandle(zl303xx_PtpClockMsgQueueS *ptpData,
                                            zl303xx_PtpV2SignalTlvS *rxTlv,
                                            zl303xx_PortDataS *pPort,
                                            zl303xx_ClockStreamS **ppStream,
                                            zl303xx_PtpV2SignalTlvS *txTlv);
static zlStatusE zl303xx_PtpUniNegGrantHandle(zl303xx_PtpClockMsgQueueS *ptpData,
                                              zl303xx_PtpV2SignalTlvS *rxTlv,
                                              zl303xx_PortDataS *pPort,
                                              zl303xx_ClockStreamS **ppStream,
                                              zl303xx_PtpV2SignalTlvS *txTlv);
static zlStatusE zl303xx_PtpUniNegCancelHandle(zl303xx_PtpClockMsgQueueS *ptpData,
                                               zl303xx_PtpV2SignalTlvS *rxTlv,
                                               zl303xx_PortDataS *pPort,
                                               zl303xx_ClockStreamS **ppStream,
                                               zl303xx_PtpV2SignalTlvS *txTlv);
static zlStatusE zl303xx_PtpUniNegAckCancelHandle(zl303xx_PtpClockMsgQueueS *ptpData,
                                                  zl303xx_PtpV2SignalTlvS *rxTlv,
                                                  zl303xx_PortDataS *pPort,
                                                  zl303xx_ClockStreamS **ppStream,
                                                  zl303xx_PtpV2SignalTlvS *txTlv);

zlStatusE zl303xx_PtpV2StartUnicastMsgFlow(
      zl303xx_ClockStreamS *pPtpStream,
      Uint32T uniNegMsgTypeList);

zlStatusE zl303xx_PtpV2StopUnicastMsgFlow(
      zl303xx_ClockStreamS *pPtpStream,
      Uint32T uniNegMsgTypeList);

void zl303xx_PtpV2DoUnicastStreamOverallState(zl303xx_ClockStreamS *stream);

static void zl303xx_PtpV2DoUnicastMsgState(
      zl303xx_ClockStreamS *pPtpStream);

/* Summary function to check the validity of the requested contract */
static void zl303xx_PtpV2CheckUnicastRequest(
                    zl303xx_PtpV2SignalRequestS *request,
                    zl303xx_PortDataS *pPort,
                    zl303xx_ClockStreamS *pStream,
                    zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType,
                    zl303xx_PortAddress *srcAddr,
                    zl303xx_PtpV2SignalGrantS *grant);

zl303xx_BooleanE zl303xx_PtpV2CheckPortSupportsNegotiation(
      zl303xx_PortDataS *pPtpPort,
      zl303xx_V2UnicastNegotiatedMsgTypeE unicastMessageType);

static zl303xx_BooleanE zl303xx_PtpV2StreamAllServicesRequired(
      zl303xx_ClockStreamS *pPtpStream);

/* Routines to check Unicast Service Limits on the Clock and Port. */
static zlStatusE zl303xx_PtpV2CheckMsgTypeClockLimits(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_V2UnicastNegotiatedMsgTypeE uniMsgType,
      Sint8T log2intvl);
static zlStatusE zl303xx_PtpV2CheckMsgTypePortLimits(
      zl303xx_PortDataS *pPtpPort,
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_V2UnicastNegotiatedMsgTypeE uniMsgType,
      Sint8T log2intvl);

/* Convert a log2 packet interval to a PPS value (rates below 1PPS return 0). */
static Uint32T zl303xx_PtpV2UnicastIntvlToPps(Sint8T intvl);

static void zl303xx_PtpV2UniNegIntvlDetectInit(zl303xx_ClockStreamS *pStream,
                                               zl303xx_MessageTypeE type,
                                               Sint8T intvl);
static void zl303xx_PtpUniNegPtsfRejectedSet(zl303xx_PtpStreamHandleT streamHandle,
                              zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType,
                              zl303xx_BooleanE val);
static void zl303xx_PtpUniNegPtsfIntvlTooHighSet(zl303xx_ClockStreamS *stream,
                              zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType,
                              Sint8T *intvl);

static void zl303xx_PtpContractRxStateRun(zl303xx_ClockStreamS *stream,
                                          Uint32T *cancelList);
static void zl303xx_PtpContractTxStateRun(zl303xx_ClockStreamS *stream,
                                          Uint32T *cancelList);

/****************   STATIC GLOBAL VARIABLES   *********************************/

const zl303xx_PortIdentity Zl303xx_PtpAllPorts = {{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}, 0xFFFF};

/* Strings correspond to zl303xx_PtpContractRxStateE */
static const char *ContractRxStateStr[] =
{
   "IDLE",
   "REQUESTING",
   "ACTIVE",
   "CANCELLING"
};

#define CONTRACT_RX_STATE_TO_STR(state) \
           ((state) < ZL303XX_ARRAY_SIZE(ContractRxStateStr) \
               ? ContractRxStateStr[state] \
               : "INVALID")

/* Strings correspond to zl303xx_PtpContractTxStateE */
static const char *ContractTxStateStr[] =
{
   "IDLE",
   "ACTIVE",
   "CANCELLING"
};

#define CONTRACT_TX_STATE_TO_STR(state) \
           ((state) < ZL303XX_ARRAY_SIZE(ContractTxStateStr) \
               ? ContractTxStateStr[state] \
               : "INVALID")

/* Strings corresponding to the zl303xx_V2UnicastNegotiatedMsgTypeE type. */
static const char *Zl303xx_UnicastMsgIndexStr[] =
{
   "SYNC",
   "ANNOUNCE",
   "DELAY RESPONSE",
   "PDELAY_RESP",
   "UNKNOWN"
};

/* MACRO to get the appropriate unicast message type string and do
 * range checking. */
#define GET_UNICAST_MSG_STR(msgIndex)                                   \
   (((msgIndex) < PTP_NEGOTIATED_NUM_MSG_TYPES)                         \
         ? Zl303xx_UnicastMsgIndexStr[(msgIndex)]                         \
         : Zl303xx_UnicastMsgIndexStr[(PTP_NEGOTIATED_NUM_MSG_TYPES)])

/* Converts unicast negotiated message index to a PTP messageType. Use this in
 * loops instead of zl303xx_PtpV2IndexToMessageType(). */
static const zl303xx_MessageTypeE Zl303xx_UnicastIdxToMsgType[PTP_NEGOTIATED_NUM_MSG_TYPES] =
{
   ZL303XX_MSG_ID_SYNC,
   ZL303XX_MSG_ID_ANNOUNCE,
   ZL303XX_MSG_ID_DELAY_RESP,
   ZL303XX_MSG_ID_PEER_DELAY_RESP
};

static const UniNegTlvDefS UniNegTlvDef[] = {
/* streamRequired    process() */
   { ZL303XX_FALSE, zl303xx_PtpUniNegReqHandle,      },
   { ZL303XX_TRUE,  zl303xx_PtpUniNegGrantHandle,    },
   { ZL303XX_TRUE,  zl303xx_PtpUniNegCancelHandle,   },
   { ZL303XX_TRUE,  zl303xx_PtpUniNegAckCancelHandle }
};

/* A note on why a CANCEL TLV requires a stream to process it:
 * IEEE 1588-2008 section 16.1.1 states: "A grantor receiving a
 * CANCEL_UNICAST_TRANSMISSION TLV shall always respond with an
 * ACKNOWLEDGE_CANCEL_UNICAST_TRANSMISSION TLV and may immediately
 * cease to provide the indicated service." Since no stream exists,
 * this port cannot be considered a grantor to the far-end port, and
 * no message should be transmitted back. */

#define UNI_NEG_TLV_TYPE_TO_INDEX(type)  ((type) - PTP_TLV_REQUEST_UNICAST_TX)


/****************   EXPORTED GLOBAL VARIABLES   *******************************/

/****************   DEPRECATED FUNCTION DEFINITIONS   *************************/

/* zl303xx_PtpV2UniNegContractStart */
/** 

   API routine to Start the specified Unicast Message Contract on the
   stream provided.

  (Deprecated)  This function has been replaced by zl303xx_PtpContractRequest().

  Parameters:
   [in]  streamHandle   Stream Id on which to start the specified contract.
   [in]  messageType    PTP message contract to initiate.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_PtpV2UniNegContractStart(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_MessageTypeE messageType)
{
   return zl303xx_PtpContractRequest(streamHandle, messageType, NULL);
}

/*

  Function Name:
    zl303xx_PtpV2UniNegContractCancel

  Details:
   API routine to Cancel the specified Unicast Message Contract on the
   stream provided.

  (Deprecated)  The function has been replaced by zl303xx_PtpContractCancel().

  Parameters:
   [in]  streamHandle   Stream Id on which to cancel the specified contract.
   [in]  messageType    PTP message contract to cancel.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_PtpV2UniNegContractCancel(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_MessageTypeE messageType)
{
   return zl303xx_PtpContractCancel(streamHandle, messageType);
}

/****************   INTERNAL PTP FUNCTION DEFINITIONS   ***********************/

/* zl303xx_PtpV2UnicastOncePerSecond */
/** 

   Performs unicast mode actions in response to the once per second timer for
   the specified stream.

  Parameters:
       pPtpStream     Pointer to the clock stream instance structure.

  Return Value:
   zlStatusE

  Notes:
   Note that the once-per-second timer is asynchronous to when the "duration"
   counter starts, so the first second of a granted period might be "short".
   We make the transmit grant duration longer by one second to compensate.
   Also the once-per-second timer is asynchronous to when a cancel request
   is received so there could be two cancel messages (the first and second) sent
   very close together. This should not cause a problem as the second should be
   ignored.

*******************************************************************************/
zlStatusE zl303xx_PtpV2UnicastOncePerSecond(zl303xx_ClockStreamS *pPtpStream)
{
   zlStatusE status = ZL303XX_OK;
   /* Used for Slaves & Masters that are canceling. */
   Uint32T uniNegCancelList = 0;
   const char *fnName = "zl303xx_PtpV2UnicastOncePerSecond";

   /* Check the function input Parameter Pointer */
   if (status == ZL303XX_OK)
   {
       if (pPtpStream == NULL)
       {
           ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                 fnName, 0,0,0,0,0);

           return ZL303XX_INVALID_POINTER;
       }
   }

   if (status == ZL303XX_OK)
   {
      /*  Decrement the lockout timer. */
      if (pPtpStream->unicast.lockout.remainingSec > 0)
      {
         pPtpStream->unicast.lockout.remainingSec--;
         if (pPtpStream->unicast.lockout.remainingSec == 0)
         {
            zl303xx_PtsfFlagSet(pPtpStream->clockStreamHandle, ZL303XX_PTSF_RETRY_ATTEMPTS_FAIL, ZL303XX_FALSE);
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 1,
                          "Stream %lu: Master lockout has ended.",
                          pPtpStream->clockStreamHandle, 0,0,0,0,0);
         }
      }

      /* Decrement the wait to restore timer. */
      if ((pPtpStream->pUmtEntry) &&
          (pPtpStream->portData->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1))
      {
         zl303xx_PtpTelecomWtrUpdate(pPtpStream->clockStreamHandle,
                                     &pPtpStream->pUmtEntry->telecom);
      }

      zl303xx_PtpContractRxStateRun(pPtpStream, &uniNegCancelList);
      zl303xx_PtpContractTxStateRun(pPtpStream, &uniNegCancelList);

      /* If the stream has not sent an update for some time, check if a
       * keep-alive event is required.        */
      if (pPtpStream->config.keepAliveSec != 0)
      {
         if ((--pPtpStream->keepAliveRemSec) == 0)
         {
            /* Keep-Alive period timeout. The counter is reset within the Event generator. */
            zl303xx_PtpEventStreamKeepAlive(pPtpStream);
         }
      }

      /* Re-issue the Cancel Request (if any are required). */
      /* Here we do not force ALL messageTypes to cancel based on the
       * Stacked-TLV setting since the state-machine may require just 1 to be canceled. */
      if (uniNegCancelList != 0)
      {
         status = zl303xx_PtpV2StopUnicastMsgFlow(pPtpStream, uniNegCancelList);
      }
   }

   return status;
}

/* _zl303xx_PtpV2DoUnicastState */
/** 

   Perform unicast mode state actions according to V2 protocol

  Parameters:
       pPtpStream    Pointer to the clock stream to process

  Return Value:
   Nothing

*******************************************************************************/
void _zl303xx_PtpV2DoUnicastState(zl303xx_ClockStreamS *pPtpStream)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "_zl303xx_PtpV2DoUnicastState";

   /* Check the function input Parameter Pointer */
   if (status == ZL303XX_OK)
   {
       if (pPtpStream == NULL)
       {
           ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                 fnName, 0,0,0,0,0);

           return;
       }
   }

   if (status == ZL303XX_OK)
   {  /* Execute the overall stream state machine */
      zl303xx_PtpV2DoUnicastStreamOverallState(pPtpStream);

      /* Execute the individual sub packet flow state machines for the stream */
      zl303xx_PtpV2DoUnicastMsgState(pPtpStream);
   }
}

/* zl303xx_PtpV2SignalUnicastNegTlvHandle */
/** 

   Handle a received unicast contract negotiation TLV

  Parameters:
   [in]   ptpData     Pointer to the PTP event message buffer (the message
                                as received from the PTP task message queue).
   [in]   rxTlv  The Received Unicast Negotiation TLV contents.
   [in]   ppStream      Address of a pointer to the clock stream instance
                              structure (may be NULL if there is no clock stream
                              associated with this received packet). If a stream
                              is created for this message, it is returned to the
                              calling routine.
   [in]    pPort         Pointer to the port data structure. (Cannot be NULL).

   [in]    txTlv  The Response Unicast Negotiation TLV contents.

  Return Value:
   zlStatusE

  Notes:
   This routine only handles the Signaling TLVs related to Unicast Negotiation
   (REQUEST_TX, GRANT_TX, CANCEL_TX, & CANCEL_TX_ACK). All other TLVs result
   in an ERROR.

*******************************************************************************/
zlStatusE zl303xx_PtpV2SignalUnicastNegTlvHandle(
             zl303xx_PtpClockMsgQueueS *ptpData,
             zl303xx_PtpV2SignalTlvS *rxTlv,
             zl303xx_ClockStreamS **ppStream,
             zl303xx_PortDataS *pPort,
             zl303xx_PtpV2SignalTlvS *txTlv)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtpV2SignalUnicastNegTlvHandle";
   const char *rxTlvStr     = NULL;
   const char *rxTlvTypeStr = NULL;
   const char *msgTypeStr   = NULL;
   Uint32T defIdx;

   /* Check the input pointers. */
   if (status == ZL303XX_OK)
   {
       if ((ptpData == NULL) ||
           (rxTlv == NULL) ||
           (ppStream == NULL) ||
           (pPort == NULL) ||
           (txTlv == NULL))
       {
          ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                fnName, 0,0,0,0,0);

          return ZL303XX_INVALID_POINTER;
       }
   }

   rxTlvStr     = zl303xx_PtpV2SignalTlvTypeStrBrief(rxTlv->tlvType);
   rxTlvTypeStr = zl303xx_PtpV2MsgTlvTypeToStr(rxTlv->tlvType);
   msgTypeStr   = zl303xx_PtpMessageTypeToStr(rxTlv->data.messageType);

   /* Initialize the return TLV type to the "Reserved" value so it will not be
    * used by the calling function. If a TLV needs to be transmitted in response,
    * the handler function will set this value correctly. */
   txTlv->tlvType = 0;

   /* Check that only UNICAST NEGOTATION TLVs are passed. */
   defIdx = UNI_NEG_TLV_TYPE_TO_INDEX(rxTlv->tlvType);
   if (defIdx >= ZL303XX_ARRAY_SIZE(UniNegTlvDef))
   {
      UNI_NEG_TRACE(1, pPort, *ppStream,
                    "TLV type %s (0x%04X) not related to unicast negotiation.",
                    rxTlvTypeStr, rxTlv->tlvType, 0,0,0);
      return ZL303XX_ERROR;
   }

   /* As per IEEE1588 16.1.2: If Unicast Negotiation is Disabled, do not:
      - RESPOND to a GRANT REQUEST
      - TRANSMIT a GRANT REQUEST
      - TRANSMIT a GRANT
      - RESPOND to a CANCEL
      we do:
      - RESPOND to UNICAST ENABLE MANAGEMENT messages. */
   if ((pPort->config.unicastNegotiation.enabled != ZL303XX_TRUE) &&
       (rxTlv->tlvType != PTP_TLV_CANCEL_UNICAST_TX) &&
       (rxTlv->tlvType != PTP_TLV_CANCEL_UNICAST_TX_ACK))
   {
      /* We have received a Unicast TLV that we cannot process while in
         a DISABLED state */
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 1,
         "Port %lu: Received a %s TLV with unicast negotiation disabled.",
         pPort->clockPortHandle, rxTlvStr, 0,0,0,0);
      return ZL303XX_NOT_RUNNING;
   }

   if ((UniNegTlvDef[defIdx].streamRequired == ZL303XX_TRUE) &&
       (*ppStream == NULL))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 1,
         "Port %lu: Discarded %s TLV: no associated stream.",
         pPort->clockPortHandle, rxTlvStr, 0,0,0,0);
      return ZL303XX_OK;  /* Not an error */
   }

   if (ZL303XX_CHECK_UNI_NEG_MSG_TYPE_VALID(rxTlv->data.messageType) != ZL303XX_OK)
   {
      UNI_NEG_TRACE(1, pPort, *ppStream,
         "Discarded %s TLV: messageType %s (0x%X) is invalid.",
         rxTlvStr, msgTypeStr, rxTlv->data.messageType, 0,0);
      return ZL303XX_OK;  /* Not an error */
   }

   return UniNegTlvDef[defIdx].process(ptpData, rxTlv, pPort, ppStream, txTlv);
}

/* zl303xx_PtpV2StopUnicastStream */
/** 

   Attempt to stop all message types associated with an active unicast stream.

  Parameters:
   [in]   stream  Pointer to an existing stream.

*******************************************************************************/
void zl303xx_PtpV2StopUnicastStream(zl303xx_ClockStreamS *stream)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;
   const char *fnName = "zl303xx_PtpV2StopUnicastStream";

   /* Check the function input Parameter Pointer */
   if (status == ZL303XX_OK)
   {
       if (stream == NULL)
       {
           ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                 fnName, 0,0,0,0,0);

           return;
       }
   }

   for (uniNegMsgType = PTP_NEGOTIATED_MSG_ID_SYNC;
        uniNegMsgType < PTP_NEGOTIATED_NUM_MSG_TYPES;
        uniNegMsgType++)
   {
      if (stream->contractRx[uniNegMsgType].state != ZL303XX_PTP_CONTRACT_RX_IDLE)
      {
         UNI_NEG_CONTRACT_TRACE(2, stream, "RX", uniNegMsgType,
               "CANCELLING: current %d: next %d",
               stream->contractRx[uniNegMsgType].state,ZL303XX_PTP_CONTRACT_RX_CANCELLING,0,0);
         zl303xx_PtpContractRxStateSet(stream, uniNegMsgType,
                                       ZL303XX_PTP_CONTRACT_RX_CANCELLING);
      }
   }
}

/* zl303xx_PtpV2UnicastNegTlvInitiate */
/** 

   Initiates a Unicast TLV transmission. Generally used for REQUEST (including
   renewals) & CANCEL messages when the STREAM already exists.

   The Unicast Negotiation TLV portion of the message must be provided in
   Stacked-TLV format. These will be transmitted individually or in Stacked-TLV
   format depending on:
      - The PTP port's unicastNegotiation.stackedTlvEn setting.
      - Combination of the Ports stackedTlvEn and the stackAllServicesOn1stRequest
        for initial contract startup.

   The PTP header & Signal Data portions are generated internally from the
   PtpStream configuration.

  Parameters:
   [in]  destAddr          The destination address to send the message to.
                                (May be NULL).
   [in]  pPtpStream        Pointer to the clock stream structure associated
                                with this connection. (Can not be NULL).
   [in]  pTargetPortId     Port Identity of the targeted far-end port, if
                                 known. If not provided, it is taken from the
                                 stream history (which may be all 1's).
   [in]  pUniNegTlvBuf     Pointer to the Unicast TLV buffer to send.
   [in]  uniNegTlvBufLen   Length of the Unicast TLV buffer.


  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_PtpV2UnicastNegTlvInitiate(
      zl303xx_PortAddress *destAddr,
      zl303xx_ClockStreamS *pPtpStream,
      const zl303xx_PortIdentity *pTargetPortId,
      Uint8T *pUniNegTlvBuf, Uint8T uniNegTlvBufLen)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpTxStatusE txStatus = ZL303XX_PTP_TX_OK;
   const char *fnName = "zl303xx_PtpV2UnicastNegTlvInitiate";
   /* Pointer to the port & clock associated with this stream. */
   zl303xx_PortDataS *pPtpPort = NULL;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Many pointers may be NULL, they are checked later */
   if (pPtpStream == NULL)
   {
      ZL303XX_TRACE_ALWAYS("%s:: Stream (%p) (== NULL)",
            fnName, (pPtpStream), 0,0,0,0);

      status = ZL303XX_INVALID_POINTER;
   }
   else  /* Set the Port & Clock data for this stream. */
   {
      /* To initiate a Unicast TLV, the stream must exist. */
      pPtpPort = pPtpStream->portData;
      pPtpClock = pPtpPort->clock;
   }

   /* Build the PTP message Header. */
   if (status == ZL303XX_OK)
   {
      /* We have to pass a non-NULL pointer to the 'Pack' routine in order to
       * avoid the sequenceId from being incremented by mistake.    */
      Uint16T seqId = pPtpStream->config.nextTxSequenceId[ZL303XX_MSG_ID_SIGNALING];

      /* Pack the PTP Message Header for a Signaling message.
       * The input TLV buffer may have multiple TLVs but, the PTP header will
       * always be the same (except for the messageLength & sequenceId fields
       * which will be set properly just before the buffer is transmitted.    */
      if (ZL303XX_PTP_V2_HEADER_LEN != zl303xx_PtpV2MsgHeaderPackForStream(
                                             pPtpStream,
                                             ZL303XX_MSG_ID_SIGNALING,
                                             &seqId,    /* Filler for now, sequenceId is set properly later. */
                                             pPtpClock->msgObuf))
      {
         /* The Header was not constructed correctly. Display an error and quit. */
         ZL303XX_TRACE_ALWAYS("%s: Header Pack ERROR!", fnName, 0,0,0,0,0);

         status = ZL303XX_DATA_CORRUPTION;
      }
   }

   /* Add the Signaling Data. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpV2MsgSignalingS signalDataS;

      /* The only item in the Signal Data structure is the targetPortIdentity.
       * If a port identity was passed into this function, use it. Otherwise,
       * use the parentIdentity from the stream's parentDS. */
      if (pTargetPortId != NULL)
      {
         signalDataS.targetPortIdentity = *pTargetPortId;
      }
      else
      {
         /* If a stream has the local clockIdentity as its far-end, then it does
          * not know the targetPortIdentity yet. In that case use all ones. */
         if (zl303xx_PtpV2EqualClockIdentities(
               pPtpStream->farEndParentDS.u.v2.parentPortIdentity.clockIdentity,
               pPtpStream->portData->clock->config.defaultDS.clockIdentity) == ZL303XX_TRUE)
         {
            OS_MEMSET(&signalDataS.targetPortIdentity, 0xFF, sizeof(signalDataS.targetPortIdentity));
         }

         /* For Client operations:
          * - If a previous contract exists, use the learned targetPortIdentity.
          * - If no contract exists or has expired, use all 1's again for the
          *   targetPortIdentity.       */
         else
         {
            /* Use the ANNOUNCE contract as the key here for all others. */
            if (pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].remainingSec > 0)
            {
               signalDataS.targetPortIdentity = pPtpStream->farEndParentDS.u.v2.parentPortIdentity;
            }
            else
            {
               OS_MEMSET(&signalDataS.targetPortIdentity, 0xFF, sizeof(signalDataS.targetPortIdentity));
            }
         }
      }

      /* Pack this data right after the header. */
      if (ZL303XX_PTP_V2_SIGNAL_DATA_LEN != zl303xx_PtpV2MsgDataSignalPack(
                                             &signalDataS,
                                             (pPtpClock->msgObuf + ZL303XX_PTP_V2_HEADER_LEN)))
      {
         /* The Signal Data was not constructed correctly. Display an error and quit. */
         ZL303XX_TRACE_ALWAYS("%s: Signal Data Pack ERROR!", fnName, 0,0,0,0,0);

         status = ZL303XX_DATA_CORRUPTION;
      }
   }

   /* Add the Signaling TLVs (based on unicastNegotiation.stackedTlvEn) & send
    * all at once or individually. */
   if (status == ZL303XX_OK && pUniNegTlvBuf)
   {
      Uint16T tlvBytesSent = 0;        /* Track the number of bytes processed from the TLV buffer.    */
      Uint16T thisTlvLength = 0;       /* The number of TLV bytes to be sent in the current packet.   */
      Uint16T pktBufferLength = 0;     /* Number of bytes contained in the current packet (HDR + SIG + TLV). */
      Uint16T pktSequenceNum = 0;      /* Sequence number of the current packet. */

      /* STATUS is checked inside the loop. */
      while ((status == ZL303XX_OK) &&                     /* Last loop was good. */
             (tlvBytesSent < uniNegTlvBufLen) &&         /* More TLV bytes remaining. */
             ((uniNegTlvBufLen - tlvBytesSent) > 4))     /* At least a TLV header remaining. */
      {
         /* Each packet buffer sent will have the PTP header & Signal Data
          * (already packed outside this loop). */
         pktBufferLength = ZL303XX_PTP_V2_HEADER_LEN + ZL303XX_PTP_V2_SIGNAL_DATA_LEN;

         /* Grab the next Signaling sequence number for this stream. */
         pktSequenceNum = pPtpStream->config.nextTxSequenceId[ZL303XX_MSG_ID_SIGNALING];

         /* If Stacked-TLVs is enabled, or 'allServices' are required on
          * initial timing contract request, send the entire buffer. */
         if ((pPtpPort->config.unicastNegotiation.stackedTlvEn == ZL303XX_TRUE) ||
             (pPtpStream->unicast.requestAllServices == ZL303XX_TRUE))
         {
            thisTlvLength = uniNegTlvBufLen;
         }

         else  /* Send the TLVs individually. */
         {
            /* We should check that there are at least 4 bytes remaining in the
             * buffer but given that this is a LOCAL routine, we are safe to
             * assume that this is the case. */
            /* Ignore the TLV TYPE field. */
            (void)zl303xx_PtpTlvTypeLengthGet((pUniNegTlvBuf + tlvBytesSent), NULL, &thisTlvLength);

            /* The LENGTH field of the TLV does not include the 4 bytes needed
             * for the TYPE and LENGTH fields so add these 4 bytes.         */
            thisTlvLength += ZL303XX_PTP_TLV_BASE_LEN;
         }

         /* Copy the required portion of the TLV buffer to the packet. */
         OS_MEMCPY((pPtpClock->msgObuf + pktBufferLength),
                (pUniNegTlvBuf + tlvBytesSent),
                (thisTlvLength));

         /* The length of the buffer for this packet. */
         pktBufferLength += thisTlvLength;

         /* The number of bytes of the TLV buffer used. */
         tlvBytesSent += thisTlvLength;

         /* Insert the total length of the message into the packet. */
         zl303xx_PtpV2MsgPktLengthSet(pktBufferLength, pPtpClock->msgObuf);

         /* Insert the sequenceId of this message into the packet. */
         zl303xx_PtpV2MsgSequenceNumSet(pktSequenceNum, pPtpClock->msgObuf);

         /* Send the actual buffer. */
         {
            /* By specifying the destination address explicitly (passed in
             * parameter) this will override any value contained in the general
             * port connection structure */
            txStatus = zl303xx_PtpTxMsg(pPtpClock->msgObuf, pktBufferLength,
                        ZL303XX_PTP_INTERFACE_GENERAL, pPtpPort, destAddr,
                        pPtpStream, ZL303XX_MSG_ID_SIGNALING);

            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 2,
                  "Stream %d: Initiating Unicast Request (Seg:%d): Total (%d): This (%d); Accumulated(%d)",
                  pPtpStream->clockStreamHandle, pktSequenceNum,
                  uniNegTlvBufLen, thisTlvLength, tlvBytesSent, 0);

            if (txStatus != ZL303XX_PTP_TX_OK)
            {
               zl303xx_PtpPortLogFaultyState(txStatus, ZL303XX_PTP_PORT_FAULT_SIGNALLING_SEND, pPtpPort);
               zl303xx_PtpV2ToPortState(pPtpPort, PTP_PORT_INT_STATE_FAULTY);
               status = ZL303XX_PROTOCOL_ENGINE_ERROR;
            }
            else
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
                     "Stream %u event: Send %s Message",
                     pPtpStream->clockStreamHandle,
                     (const char*)"PTP_SIGNALING_MESSAGE",
                     0,0,0,0);
            }
         }
      } /* while */
   }    /* !ZL303XX_OK || !pUniNegTlvBuf */

   return status;
}  /* END zl303xx_PtpV2UnicastNegTlvInitiate */

/* zl303xx_PtpV2UnicastNegTlvRespond */
/** 

   Respond to a received Unicast TLV. Generally used for GRANTS (including DENY)
   & CANCEL_ACK messages. The STREAM may not exists in the case of a GRANT DENY.

   The Unicast Negotiation TLV portion of the message must be provided in
   Stacked-TLV format. These will be transmitted in the same format in which
   they were received. If there are multiple TLVs, then a Stacked-TLV must have
   been received.

   The PTP header & Signal Data portions are generated from the receive message.
   PtpStream configuration.

  Parameters:
   [in]  destAddr          The destination address which to send the response
                                to. (May be NULL).
   [in]  pPtpStream        Pointer to the clock stream structure associated
                                with this connection. (May be NULL).
   [in]  pPtpPort          Pointer to the PTP port structure associated with
                                this connection. (Can not be NULL).
   [in]  pPtpV2MsgHdrRx    Pointer to the received PTP message header
                                structure to which the responds applies.
   [in]  pUniNegTlvBuf     Pointer to the Unicast TLV buffer to send.
   [in]  uniNegTlvBufLen   Length of the Unicast TLV buffer.


  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_PtpV2UnicastNegTlvRespond(
      zl303xx_PortAddress *destAddr,
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PortDataS *pPtpPort,
      zl303xx_PtpV2MsgHeaderS *pPtpV2MsgHdrRx,
      Uint8T *pUniNegTlvBuf, Uint8T uniNegTlvBufLen)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpTxStatusE txStatus = ZL303XX_PTP_TX_OK;
   const char *fnName = "zl303xx_PtpV2UnicastNegTlvRespond";
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* pPtpStream - MAY BE NULL */
   /* Check the input pointers. */
   if (status == ZL303XX_OK)
   {
       if ((pPtpPort == NULL) ||
           (pPtpV2MsgHdrRx == NULL) ||
           (pUniNegTlvBuf == NULL) ||
           (pPtpPort->clock == NULL))
       {
          ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                fnName, 0,0,0,0,0);

          return ZL303XX_INVALID_POINTER;
       }
   }

   if (status == ZL303XX_OK)
   {
      /* Pointer to the clock associated with this port. */
      pPtpClock = pPtpPort->clock;

   /* Build the PTP message Header. */

      /* The response message is based on the received message, not the stream. */
      zl303xx_PtpV2MsgHeaderS ptpV2MsgHdrTx;

      /* Clear the local header structure. */
      OS_MEMSET(&ptpV2MsgHdrTx, 0x00, sizeof(ptpV2MsgHdrTx));

      /* Assign the message fields in the common header */
      /**************************************************/
      ptpV2MsgHdrTx.messageType = ZL303XX_MSG_ID_SIGNALING;
      ptpV2MsgHdrTx.transportSpecific = (Uint4T)0;
      ptpV2MsgHdrTx.versionPTP = (Uint4T)ZL303XX_PTP_VERSION_2;
      ptpV2MsgHdrTx.messageLength = ZL303XX_PTP_V2_HEADER_LEN;  /* Updated later. */
      ptpV2MsgHdrTx.domainNumber = pPtpClock->config.defaultDS.domainNumber;

      /* For SIGNAL messages, only the Unicast flag applies (always UNICAST). */
      setFlag(ptpV2MsgHdrTx.flagField, PTP2_FLAGS_UNICAST);

      /* sourcePortIdentity */
      /* We advertise our own portIdentity */
      ptpV2MsgHdrTx.sourcePortIdentity = pPtpPort->portDS.portIdentity;

      /* sequenceId */
      /* Signaling messages always use the local sequenceID pool (even for Responses). */
      if (pPtpStream != NULL)
      {
         ptpV2MsgHdrTx.sequenceId = pPtpStream->config.nextTxSequenceId[ZL303XX_MSG_ID_SIGNALING];
      }
      else /* We don't have a stream for this connection so use the received sequenceID. */
      {
         ptpV2MsgHdrTx.sequenceId = pPtpV2MsgHdrRx->sequenceId;
      }

      ptpV2MsgHdrTx.controlField = (Uint8T)PTP_OTHER_MESSAGE;
      ptpV2MsgHdrTx.logMessageInterval = PTP_V2_LMMI_NULL_VALUE;

      /* Pack the PTP Message Header for For the response.  */
      if (ZL303XX_PTP_V2_HEADER_LEN != zl303xx_PtpV2MsgHeaderPack(&ptpV2MsgHdrTx, pPtpClock->msgObuf))
      {
         /* The Header was not constructed correctly. Display an error and quit. */
         ZL303XX_TRACE_ALWAYS("%s: Header Pack ERROR!", fnName, 0,0,0,0,0);

         status = ZL303XX_DATA_CORRUPTION;
      }
   }

   /* Add the Signaling Data. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpV2MsgSignalingS signalDataS;

      /* The only item in the Signal Data structure is the targetPortIdentity.
       * This will be the same as the received sourcePortIdentity. */
      signalDataS.targetPortIdentity = pPtpV2MsgHdrRx->sourcePortIdentity;

      /* Pack this data right after the header. */
      if (ZL303XX_PTP_V2_SIGNAL_DATA_LEN != zl303xx_PtpV2MsgDataSignalPack(
                                             &signalDataS,
                                             (pPtpClock->msgObuf + ZL303XX_PTP_V2_HEADER_LEN)))
      {
         /* The Signal Data was not constructed correctly. Display an error and quit. */
         ZL303XX_TRACE_ALWAYS("%s: Signal Data Pack ERROR!", fnName, 0,0,0,0,0);

         status = ZL303XX_DATA_CORRUPTION;
      }
   }

   /* Add the Signaling TLV(s) & send all at once. */
   if (status == ZL303XX_OK)
   {
      /* The response packet buffer sent will have the PTP header & Signal Data
       * (already packed above). */
      Uint16T pktBufferLength = ZL303XX_PTP_V2_HEADER_LEN + ZL303XX_PTP_V2_SIGNAL_DATA_LEN;

      /* Copy the entire TLV buffer to the packet. */
      OS_MEMCPY((pPtpClock->msgObuf + pktBufferLength),
             (pUniNegTlvBuf),
             (uniNegTlvBufLen));

      /* The length of the buffer for this packet. */
      pktBufferLength += uniNegTlvBufLen;

      /* Append any custom TLVs to the end of the Announce Message. */
      if (pPtpStream != NULL)
      {
         pktBufferLength += zl303xx_PtpProfileTlvAppend(pPtpStream,
                                       ZL303XX_MSG_ID_SIGNALING,
                                       pPtpClock->msgObuf,
                                       pPtpClock->msgObuf + pktBufferLength);
      }

      /* Update the total length of the message into the packet. */
      zl303xx_PtpV2MsgPktLengthSet(pktBufferLength, pPtpClock->msgObuf);

      /* Send the actual buffer. */
      {
         /* By specifying the destination address explicitly (passed in
          * parameter) this will override any value contained in the general
          * port connection structure */
         txStatus = zl303xx_PtpTxMsg(pPtpClock->msgObuf, pktBufferLength,
                     ZL303XX_PTP_INTERFACE_GENERAL, pPtpPort, destAddr,
                     pPtpStream, ZL303XX_MSG_ID_SIGNALING);

         if (txStatus != ZL303XX_PTP_TX_OK)
         {
            zl303xx_PtpPortLogFaultyState(txStatus, ZL303XX_PTP_PORT_FAULT_SIGNALLING_SEND, pPtpPort);
            zl303xx_PtpV2ToPortState(pPtpPort, PTP_PORT_INT_STATE_FAULTY);
            status = ZL303XX_PROTOCOL_ENGINE_ERROR;
         }
         else if (pPtpStream != NULL)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 2,
                  "Stream %u event: Send %s Message",
                  pPtpStream->clockStreamHandle,
                  (const char*)"PTP_SIGNALING_MESSAGE",
                  0,0,0,0);
         }
         else
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 2,
                  "Port %u event: Send %s Message",
                  pPtpPort->clockPortHandle,
                  (const char*)"PTP_SIGNALING_MESSAGE",
                  0,0,0,0);
         }
      }
   }

   return status;
}  /* END zl303xx_PtpV2UnicastNegTlvRespond */

/* zl303xx_PtpV2CheckUnicastSlaveData */
/** 

  Details:
   Checks the parameters for the slave to use when negotiating a unicast contract

  Parameters:
   [in]   desiredAnnounceGrantSecs   The maximum duration to request an announce contract
   [in]   desiredSyncGrantSecs       The maximum duration to request a sync contract
   [in]   desiredDelayRespGrantSecs  The maximum duration to request a delay response contract
   [in]   desiredPdelayRespGrantSecs The maximum duration to request a peer delay response contract
   [in]   renewSecs                  Time before contract expiry to renew, in seconds.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_PtpV2CheckUnicastSlaveData(
      Uint32T desiredAnnounceGrantSecs,
      Uint32T desiredSyncGrantSecs,
      Uint32T desiredDelayRespGrantSecs,
      Uint32T desiredPdelayRespGrantSecs,
      Uint32T renewSecs)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T minGrant;

   /* Check that the grant period is > 0 for all message types. */
   if (status == ZL303XX_OK)
   {
      if (desiredAnnounceGrantSecs == 0)
      {
         ZL303XX_TRACE_ERROR("Invalid desiredAnnouncGrantSecs=%lu.",
                             desiredAnnounceGrantSecs, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
      if (desiredSyncGrantSecs == 0)
      {
         ZL303XX_TRACE_ERROR("Invalid desiredSyncGrantSecs=%lu.",
                             desiredSyncGrantSecs, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
      if (desiredDelayRespGrantSecs == 0)
      {
         ZL303XX_TRACE_ERROR("Invalid desiredDelayRespGrantSecs=%lu.",
                             desiredDelayRespGrantSecs, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
      if (desiredPdelayRespGrantSecs == 0)
      {
         ZL303XX_TRACE_ERROR("Invalid desiredPdelayRespGrantSecs=%lu.",
                              desiredPdelayRespGrantSecs, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      minGrant = min(desiredAnnounceGrantSecs, min(desiredSyncGrantSecs, min(desiredDelayRespGrantSecs, desiredPdelayRespGrantSecs)));

      /* ANNOUNCE, SYNC, and DELAY_RESP intervals are checked elsewhere, but renewSecs
       * needs to be compared to all of these. */
      if ((renewSecs < CONTRACT_RENEW_ADVANCE_MIN) ||
          (renewSecs > (minGrant - CONTRACT_RENEW_ADVANCE_MAX)))
      {
         ZL303XX_TRACE_ALWAYS("Invalid renewSecs=%lu (valid range: %lu - %lu).",
            renewSecs, CONTRACT_RENEW_ADVANCE_MIN, (minGrant - CONTRACT_RENEW_ADVANCE_MAX),
            0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   return status;
}

/*

  Function Name:
    zl303xx_PtpV2SetUnicastSlaveData

  Details:
   Sets the parameters for the slave to use when negotiating a unicast contract

  Parameters:
   [in]   pPtpStream                Pointer to the clock stream of interest
   [in]   desiredAnnounceIntvl      The maximum announce interval to request
   [in]   desiredAnnounceGrantSec   The maximum duration to request an announce contract
   [in]   desiredSyncIntvl          The maximum sync interval to request
   [in]   desiredSyncGrantSec       The maximum duration to request a sync contract
   [in]   desiredDelayRespIntvl     The maximum delay response interval to request
   [in]   desiredDelayRespGrantSec  The maximum duration to request a delay response contract
   [in]   desiredPdelayRespIntvl    The maximum peer delay response interval to request
   [in]   desiredPdelayRespGrantSec The maximum duration to request a peer delay response contract
   [in]   renewSecs                 Time before contract expiry to renew, in seconds.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_PtpV2SetUnicastSlaveData(
             zl303xx_ClockStreamS *pPtpStream,
             Sint8T desiredAnnounceIntvl,
             Uint32T desiredAnnounceGrantSec,
             Sint8T desiredSyncIntvl,
             Uint32T desiredSyncGrantSec,
             Sint8T desiredDelayRespIntvl,
             Uint32T desiredDelayRespGrantSec,
             Sint8T desiredPdelayRespIntvl,
             Uint32T desiredPdelayRespGrantSec,
             Uint32T renewSec)
{
   zlStatusE status = ZL303XX_OK;
   Uint16T msgId;
   zl303xx_PtpContractS *req;
   const char *fnName = "zl303xx_PtpV2SetUnicastSlaveData";

   /* Check the function input Parameter Pointer */
   if (status == ZL303XX_OK)
   {
       if (pPtpStream == NULL)
       {
           ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                 fnName, 0,0,0,0,0);

           return ZL303XX_INVALID_POINTER;
       }
   }

   if (status == ZL303XX_OK)
   {
      if (pPtpStream->portData->config.mixedMode == ZL303XX_FALSE)
      {
         req = &pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].req;
         req->interval    = desiredAnnounceIntvl;
         req->durationSec = desiredAnnounceGrantSec;

         req = &pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].req;
         req->interval    = desiredSyncIntvl;
         req->durationSec = desiredSyncGrantSec;
      }

      req = &pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].req;
      req->interval    = desiredDelayRespIntvl;
      req->durationSec = desiredDelayRespGrantSec;
      req = &pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].req;
      req->interval    = desiredPdelayRespIntvl;
      req->durationSec = desiredPdelayRespGrantSec;

      /* Initialize the renewSec value. */
      for (msgId = PTP_NEGOTIATED_MSG_ID_SYNC; msgId < PTP_NEGOTIATED_NUM_MSG_TYPES; msgId++)
      {
         pPtpStream->contractRx[msgId].renewSec = renewSec;
      }
   }

   return status;
}

/* zl303xx_PtpV2InitUnicastStreamData */
/** 

   Initializes the unicast stream data for a clock port

  Parameters:
   [in]   pPtpStream   Pointer to the clock stream of interest.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_PtpV2InitUnicastStreamData(zl303xx_ClockStreamS *pPtpStream)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_V2UnicastNegotiatedMsgTypeE msgType;
   zl303xx_PtpContractRxS *contractRx;
   zl303xx_PtpContractTxS *contractTx;
   const char *fnName = "zl303xx_PtpV2InitUnicastStreamData";

   /* Check the function input Parameter Pointer */
   if (status == ZL303XX_OK)
   {
       if (pPtpStream == NULL)
       {
           ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                 fnName, 0,0,0,0,0);

           return ZL303XX_INVALID_POINTER;
       }
   }

   if (status == ZL303XX_OK)
   {
      for (msgType = PTP_NEGOTIATED_MSG_ID_SYNC;
           msgType < PTP_NEGOTIATED_NUM_MSG_TYPES;
           msgType++)
      {
         contractRx = &pPtpStream->contractRx[msgType];
         contractTx = &pPtpStream->contractTx[msgType];

         OS_MEMSET(contractRx, 0, sizeof(*contractRx));
         OS_MEMSET(contractTx, 0, sizeof(*contractTx));

         contractRx->state = ZL303XX_PTP_CONTRACT_RX_IDLE;
         contractRx->renewSec = CONTRACT_RENEW_ADVANCE_MIN;
         zl303xx_PtpContractMsgRetryStatusInit(&contractRx->retry,
                                               &pPtpStream->config.unicastNegotiation.retry);

         contractTx->state = ZL303XX_PTP_CONTRACT_TX_IDLE;
      }
   }

   return status;
}

/****************   STATIC FUNCTION DEFINITIONS   *****************************/

/* zl303xx_PtpUniNegReqHandle */
/** 

   Handler function for the REQUEST_UNICAST_TRANSMISSION Signaling TLV.

  Parameters:
   [in]      ptpData   Pointer to the PTP event message queue structure.
   [in]      rxTlv     Received TLV unpacked into a structure.
   [in]      pPort     Port that the Signaling message was received on.
   [in,out]  ppStream  Stream that the Signaling message was received on. May
                            be NULL. Used to return a pointer to a created stream.
   [out]     txTlv     TLV response to rxTlv.

  Notes:       ppStream is assumed to be valid since this routine is called
               internally. However,*ppStream may be NULL if a stream was not
               previously created.

*******************************************************************************/
static zlStatusE zl303xx_PtpUniNegReqHandle(zl303xx_PtpClockMsgQueueS *ptpData,
                                            zl303xx_PtpV2SignalTlvS *rxTlv,
                                            zl303xx_PortDataS *pPort,
                                            zl303xx_ClockStreamS **ppStream,
                                            zl303xx_PtpV2SignalTlvS *txTlv)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;
   zl303xx_PtpV2SignalRequestS *reqData = NULL;
   zl303xx_PtpV2SignalGrantS *grantData = NULL;
   const char *msgTypeStr = NULL;
   const char *fnName = "zl303xx_PtpV2SignalUnicastNegTlvHandle";

   /* *ppStream - MAY BE NULL */
   /* Check the input pointers. */
   if (status == ZL303XX_OK)
   {
       if ((ptpData == NULL) ||
           (rxTlv == NULL) ||
           (pPort == NULL) ||
           (ppStream == NULL) ||
           (txTlv == NULL))
       {
          ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                fnName, 0,0,0,0,0);

          return ZL303XX_INVALID_POINTER;
       }
   }

   reqData = &rxTlv->data.request;
   grantData = &txTlv->data.grant;
   msgTypeStr = zl303xx_PtpMessageTypeToStr(reqData->messageType);

   /* Determine the message type of the REQUEST TLV.  Ignore the return code
    * for now since we may need to respond. */
   (void)zl303xx_PtpV2MessageTypeToIndex(reqData->messageType, &uniNegMsgType);

   /* If the stream already exists, then it must be past the INITIALIZATION/IDLE
    * stage to accept requests. */
   if (*ppStream != NULL)
   {
      if (((*ppStream)->streamSubstate == PTP_STREAM_SUBSTATE_INIT) ||
          ((*ppStream)->streamSubstate == PTP_STREAM_SUBSTATE_IDLE))
      {
         UNI_NEG_TRACE(2, pPort, *ppStream,
            "Contract [TX] %-14s : received REQUEST while STATE = %d",
            GET_UNICAST_MSG_STR(uniNegMsgType),
            (*ppStream)->streamSubstate, 0,0,0);

         status = ZL303XX_UNSUPPORTED_OPERATION;
      }
      else
      {
         UNI_NEG_TRACE(2, pPort, *ppStream,
            "Contract [TX] %-14s : received REQUEST",
            GET_UNICAST_MSG_STR(uniNegMsgType), 0,0,0,0);
         UNI_NEG_TRACE(3, pPort, *ppStream,
            "Contract [TX] %-14s :    period=%d, duration=%u",
            GET_UNICAST_MSG_STR(uniNegMsgType), reqData->logInterMessagePeriod,
            reqData->durationField, 0,0);
      }
   }

   if (status == ZL303XX_OK)
   {
      /* Set the type attribute for the GRANT response (length is set by the
       * pack function automatically). */
      txTlv->tlvType = PTP_TLV_GRANT_UNICAST_TX;

      /* Create the GRANT for this REQUEST (durationField=0 if denied). */
      /* Static Analysis Note: dereferenced of NULL pointer (*ppStream)
       * - The following call does reference *ppStream only to check if the
       *   pointer is to be ignored; it does not dereference any member.
       * - The chain of calls eventually lead to these 2 routines where the
       *   check is performed:
       *   -> PtpV2GetMsgTypePortTotalPps
       *   -> PtpV2GetPortContractCount                           */
      {
       zl303xx_PtpV2CheckUnicastRequest(reqData, pPort, *ppStream, uniNegMsgType,
                                      &ptpData->recvMsgInfo.srcAddr, grantData);
      }
   }


   /* If the request was granted and no stream exists for this destination
    * address, create one. */
   if (status == ZL303XX_OK)
   {
      if (grantData->durationField > 0)
      {
         if (*ppStream == NULL)
         {
            status = zl303xx_PtpUniNegStreamCreate(pPort, &ptpData->recvMsgInfo.srcAddr,
                                                   grantData->messageType,
                                                   grantData->logInterMessagePeriod,
                                                   ptpData->recvMsgInfo.extData, ppStream);
         }
         else /* Update the message interval (in case it changed). */
         {
            zl303xx_PtpStreamConfigIntvlSet(*ppStream,
                                            grantData->messageType,
                                            grantData->logInterMessagePeriod);
         }
      }
   }

   /* The Signal Handler will issue the response even if the request was denied */
   if (status == ZL303XX_OK)
   {
      /* Set a local stream pointer to keep the code more manageable.
       * NOTE: pStream may still be NULL if the request was not granted. */
      zl303xx_ClockStreamS *pStream = *ppStream;

      /* If the request was granted then activate it */
      if (grantData->durationField > 0)
      {
         /* Request was granted, so the stream pointer must be good. Save the
          * known REQUEST and GRANT TLV data. */
         pStream->contractTx[uniNegMsgType].req.interval    = reqData->logInterMessagePeriod;
         pStream->contractTx[uniNegMsgType].req.durationSec = reqData->durationField;

         pStream->contractTx[uniNegMsgType].grant.interval    = grantData->logInterMessagePeriod;
         pStream->contractTx[uniNegMsgType].grant.durationSec = grantData->durationField;
         pStream->contractTx[uniNegMsgType].remainingSec      = grantData->durationField;

         UNI_NEG_CONTRACT_TRACE(2, pStream, "TX", uniNegMsgType, "sent GRANT", 0,0,0,0);
         UNI_NEG_CONTRACT_TRACE(3, pStream, "TX", uniNegMsgType, "   period=%d, duration=%u, renewal=%s",
                                grantData->logInterMessagePeriod,
                                grantData->durationField,
                                ZL303XX_BOOL_TO_STR(grantData->renewalInvited), 0);

         zl303xx_PtpContractTxStateSet(pStream, uniNegMsgType,
                                       ZL303XX_PTP_CONTRACT_TX_ACTIVE);

         /* Fire the TX contract granted event */
         zl303xx_PtpEventContractGranted(pStream, grantData, ZL303XX_PTP_CONTRACT_TX);
      }
      else
      {
         UNI_NEG_TRACE(1, pPort, pStream,
                       "REQUEST for %s (0x%X) contract DENIED.",
                       msgTypeStr, grantData->messageType, 0,0,0);

         if (pStream != NULL)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
               "Stream=%d; Interval request for uniNegMsgType=%d - %s DENIED!",
               pStream->clockStreamHandle, uniNegMsgType,
               GET_UNICAST_MSG_STR(uniNegMsgType), 0,0,0);
         }
      }
   }

   return status;
}

/* zl303xx_PtpUniNegGrantHandle */
/** 

   Handler function for the GRANT_UNICAST_TRANSMISSION Signaling TLV.

  Parameters:
   [in]      ptpData   Unused.
   [in]      rxTlv     Received TLV unpacked into a structure.
   [in]      pPort     Port that the Signaling message was received on.
   [in,out]  ppStream  Stream that the Signaling message was received on. May
                            be NULL. Used to return a pointer to a created stream.
   [out]     txTlv     Unused.

*******************************************************************************/
static zlStatusE zl303xx_PtpUniNegGrantHandle(zl303xx_PtpClockMsgQueueS *ptpData,
                                              zl303xx_PtpV2SignalTlvS *rxTlv,
                                              zl303xx_PortDataS *pPort,
                                              zl303xx_ClockStreamS **ppStream,
                                              zl303xx_PtpV2SignalTlvS *txTlv)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtpUniNegGrantHandle";
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;
   zl303xx_ClockStreamS *pStream = NULL;
   zl303xx_PtpV2SignalGrantS *grantData = NULL;
   Sint8T maxIntvl;

   /* Check the input pointers. */
   if (status == ZL303XX_OK)
   {
       if ((ptpData == NULL) ||
           (rxTlv == NULL) ||
           (pPort == NULL) ||
           (ppStream == NULL) ||
           (txTlv == NULL))
       {
          ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                fnName, 0,0,0,0,0);

          return ZL303XX_INVALID_POINTER;
       }
   }

   /* Simplify the code by renaming the GRANT TLV. */
   grantData = &rxTlv->data.grant;

   pStream = *ppStream;

   /* Warning removal */
   (void)ptpData; (void)txTlv;

   if (status == ZL303XX_OK)
   {
      /* Convert the PTP messageType to UniNeg messageType. */
      status = zl303xx_PtpV2MessageTypeToIndex(grantData->messageType, &uniNegMsgType);
   }

   if (status == ZL303XX_OK && pStream != NULL)
   {
      UNI_NEG_CONTRACT_TRACE(2, pStream, "RX", uniNegMsgType, "received GRANT", 0,0,0,0);
      UNI_NEG_CONTRACT_TRACE(3, pStream, "RX", uniNegMsgType, "   period=%d, duration=%u, renewal=%s",
                             grantData->logInterMessagePeriod,
                             grantData->durationField,
                             ZL303XX_BOOL_TO_STR(grantData->renewalInvited), 0);

      if (pStream->contractRx[uniNegMsgType].state == ZL303XX_PTP_CONTRACT_RX_REQUESTING)
      {
         /* Check if the request was granted. */
         if (grantData->durationField > 0)
         {
            pStream->contractRx[uniNegMsgType].grant.interval    = grantData->logInterMessagePeriod;
            pStream->contractRx[uniNegMsgType].grant.durationSec = grantData->durationField;
            pStream->contractRx[uniNegMsgType].remainingSec      = grantData->durationField;

            if (grantData->messageType == ZL303XX_MSG_ID_DELAY_RESP)
            {
               /* Update configured Delay_Req interval. */
               pStream->config.logMinDelayReqInterval = grantData->logInterMessagePeriod;
               pStream->msgParams[ZL303XX_MSG_ID_DELAY_REQ].log2intervalAct = grantData->logInterMessagePeriod;
            }

            zl303xx_PtpContractRxStateSet(pStream, uniNegMsgType, ZL303XX_PTP_CONTRACT_RX_ACTIVE);

            /* Detect if this interval is different compared to the
             * previous granted contract. */
            zl303xx_PtpStreamIntvlDetect(
               pStream, grantData->messageType, grantData->logInterMessagePeriod);

            /* Sets PTSF-[announce|sync|delayResp]IntvlTooHigh */
            maxIntvl = grantData->logInterMessagePeriod;
            zl303xx_PtpUniNegPtsfIntvlTooHighSet(pStream, uniNegMsgType, &maxIntvl);
            if (grantData->logInterMessagePeriod > maxIntvl)
            {
               UNI_NEG_CONTRACT_TRACE(2, pStream, "RX", uniNegMsgType, "period=%d > configured max=%d",
                                      grantData->logInterMessagePeriod, maxIntvl, 0,0);
            }

            /* Telecom Profile: Ensure that the timeout used to raise PTSF-loss*
             * is longer than message interval. */
            if (pPort->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1 &&
                pStream->pUmtEntry != NULL)
            {
               zl303xx_PtpTelecomMsgTimeoutCheck(&pStream->pUmtEntry->telecom,
                                                 grantData->messageType,
                                                 grantData->logInterMessagePeriod,
                                                 &pStream->pUmtEntry->telecomConfig);
            }

            /* Fire the RX contract granted event */
            zl303xx_PtpEventContractGranted(pStream, grantData, ZL303XX_PTP_CONTRACT_RX);

            /* Whether enabled or not, just reset this value so that at the
             * start of the next contract cycle, the full rate is requested. */
            pStream->unicast.bestEffort[grantData->messageType].retryCount = 0;
         }

         else if ((pStream->config.unicastNegotiation.bestEffort[grantData->messageType].enabled == ZL303XX_TRUE) &&
                   ((pStream->contractRx[uniNegMsgType].req.interval +
                     pStream->unicast.bestEffort[grantData->messageType].retryCount) <
                        pStream->config.unicastNegotiation.bestEffort[grantData->messageType].intervalMax))
         {
            pStream->unicast.bestEffort[grantData->messageType].retryCount++;

            ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
                  "%s: PTP_TLV_REQUEST_UNICAST_TX- DENIED! (Stream %d: messageType %d: requested interval %d: retry interval %d )",
                  fnName,
                  pStream->clockStreamHandle,
                  grantData->messageType,
                  grantData->logInterMessagePeriod,
                  pStream->unicast.bestEffort[grantData->messageType].retryCount, 0);
            UNI_NEG_CONTRACT_TRACE(2, pStream, "RX", uniNegMsgType, "DENIED! Interval %d: BestEffort retry interval %d",
                  pStream->contractRx[uniNegMsgType].req.interval,
                  (pStream->contractRx[uniNegMsgType].req.interval +
                  pStream->unicast.bestEffort[grantData->messageType].retryCount), 0,0);
         }
         else
         {
            /* The request has been denied for some reason (maybe the
             * interval we requested is too low or the server was too
             * busy, etc.) Continue trying after some timeout period.
             * If renewalInvited = FALSE we should quit, but we retry. */
            UNI_NEG_CONTRACT_TRACE(1, pStream, "RX", uniNegMsgType, "DENIED (period=%d)(renewal=%s)",
                                   grantData->logInterMessagePeriod,
                                   ((grantData->renewalInvited == ZL303XX_TRUE) ? "T" : "F"), 0,0);

            ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
                  "%s: PTP_TLV_REQUEST_UNICAST_TX- DENIED! (Stream %d: messageType %d: requested interval %d)",
                  fnName,
                  pStream->clockStreamHandle,
                  grantData->messageType,
                  grantData->logInterMessagePeriod, 0,0);

            /* Generate a PTP Event for the Contract DENIAL. */
            zl303xx_PtpEventContractRejected(pPort->clock, pStream->clockStreamHandle,
                                             grantData->messageType,
                                             ZL303XX_PTP_CONTRACT_REQUEST_DENIED);

            {
               /* Increment the GRANT denied counter; lockout master if necessary */
               zl303xx_PtpContracRequestDenied(
                  &pStream->unicast.lockout,
                  &pStream->contractRx[uniNegMsgType].retry,
                  &pStream->config.unicastNegotiation.retry,
                  pStream->clockStreamHandle,
                  GET_UNICAST_MSG_STR(uniNegMsgType));
            }

            /* Whether enabled or not, just reset this value so that at the
             * start of the next contract cycle, the full rate is requested. */
            pStream->unicast.bestEffort[grantData->messageType].retryCount = 0;
         }

         /* Sets PTSF-[announce|sync|delayResp]Rejected */
         zl303xx_PtpUniNegPtsfRejectedSet(pStream->clockStreamHandle, uniNegMsgType,
                                          !grantData->durationField);
      }
      /* Cancel any contract received while in the IDLE state. */
      else if ((pStream->contractRx[uniNegMsgType].state == ZL303XX_PTP_CONTRACT_RX_IDLE) &&
               (grantData->durationField > 0))
      {
         zl303xx_PtpContractRxStateSet(pStream, uniNegMsgType, ZL303XX_PTP_CONTRACT_RX_CANCELLING);
      }
      /* Ignore GRANTs received in the ACTIVE or CANCELLING states. */
   }

   return status;
}

/* zl303xx_PtpUniNegCancelHandle */
/** 

   Handler function for the CANCEL_UNICAST_TRANSMISSION Signaling TLV.

  Parameters:
   [in]      ptpData   Unused.
   [in]      rxTlv     Received TLV unpacked into a structure.
   [in]      pPort     Port that the Signaling message was received on.
   [in,out]  ppStream  Stream that the Signaling message was received on. May
                            be NULL. Used to return a pointer to a created stream.
   [out]     txTlv     TLV response to rxTlv.

*******************************************************************************/
static zlStatusE zl303xx_PtpUniNegCancelHandle(zl303xx_PtpClockMsgQueueS *ptpData,
                                               zl303xx_PtpV2SignalTlvS *rxTlv,
                                               zl303xx_PortDataS *pPort,
                                               zl303xx_ClockStreamS **ppStream,
                                               zl303xx_PtpV2SignalTlvS *txTlv)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;
   zl303xx_ClockStreamS *pStream = NULL;
   zl303xx_PtpV2SignalCancelS *cancelData = NULL;
   const char *fnName = "zl303xx_PtpUniNegCancelHandle";

   /* Check the input pointers. */
   if (status == ZL303XX_OK)
   {
       if ((ptpData == NULL) ||
           (rxTlv == NULL) ||
           (pPort == NULL) ||
           (ppStream == NULL) ||
           (txTlv == NULL))
       {
          ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                fnName, 0,0,0,0,0);

          return ZL303XX_INVALID_POINTER;
       }
   }

   pStream = *ppStream;
   cancelData = &rxTlv->data.cancel;

   /* Warning removal */
   (void)ptpData; (void)pPort;

   if (status == ZL303XX_OK)
   {
      /* Convert the PTP messageType to UniNeg messageType. */
      status = zl303xx_PtpV2MessageTypeToIndex(cancelData->messageType, &uniNegMsgType);
   }

   if (status == ZL303XX_OK && pStream != NULL)
   {
      /* Set type for the ACK_CANCEL Signaling TLV. Length is set by pack function. */
      txTlv->tlvType = PTP_TLV_CANCEL_UNICAST_TX_ACK;

      /* The messageType should be the same as that received. */
      txTlv->data.ackCancel.messageType = cancelData->messageType;

      /* If the stream is transmitting and receiving this message type, it is
       * unknown which contract the CANCEL applies to. Is the far-end trying to
       * stop the us from transmitting, or is it informing us that it can no
       * longer provide the current service? In this case, assume that the
       * cancel is to stop us from transmitting. */
      if (pStream->contractTx[uniNegMsgType].state == ZL303XX_PTP_CONTRACT_TX_ACTIVE)
      {
         UNI_NEG_CONTRACT_TRACE(2, pStream, "TX", uniNegMsgType, "received CANCEL", 0,0,0,0);
         UNI_NEG_CONTRACT_TRACE(2, pStream, "TX", uniNegMsgType, "sent ACK_CANCEL", 0,0,0,0);

         zl303xx_PtpContractTxStateSet(pStream, uniNegMsgType, ZL303XX_PTP_CONTRACT_TX_IDLE);
      }
      else if (pStream->contractRx[uniNegMsgType].state == ZL303XX_PTP_CONTRACT_RX_ACTIVE)
      {
         UNI_NEG_CONTRACT_TRACE(2, pStream, "RX", uniNegMsgType, "received CANCEL", 0,0,0,0);
         UNI_NEG_CONTRACT_TRACE(2, pStream, "RX", uniNegMsgType, "sent ACK_CANCEL", 0,0,0,0);

         zl303xx_PtpContractRxStateSet(pStream, uniNegMsgType, ZL303XX_PTP_CONTRACT_RX_IDLE);
      }
   }

   return status;
}

/* zl303xx_PtpUniNegAckCancelHandle */
/** 

   Handler function for the ACKNOWLEDGE_CANCEL_UNICAST_TRANSMISSION Signaling
   TLV.

  Parameters:
   [in]      ptpData   Unused.
   [in]      rxTlv     Received TLV unpacked into a structure.
   [in]      pPort     Port that the Signaling message was received on.
   [in,out]  ppStream  Stream that the Signaling message was received on. May
                            be NULL.
   [out]     txTlv     Unused.

*******************************************************************************/
static zlStatusE zl303xx_PtpUniNegAckCancelHandle(zl303xx_PtpClockMsgQueueS *ptpData,
                                                  zl303xx_PtpV2SignalTlvS *rxTlv,
                                                  zl303xx_PortDataS *pPort,
                                                  zl303xx_ClockStreamS **ppStream,
                                                  zl303xx_PtpV2SignalTlvS *txTlv)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;
   zl303xx_ClockStreamS *pStream = NULL;
   zl303xx_PtpV2SignalAckCancelS *ackData = NULL;
   const char *fnName = "zl303xx_PtpUniNegAckCancelHandle";

   /* Check the input pointers. */
   if (status == ZL303XX_OK)
   {
       if ((ptpData == NULL) ||
           (rxTlv == NULL) ||
           (pPort == NULL) ||
           (ppStream == NULL) ||
           (*ppStream == NULL) ||
           (txTlv == NULL))
       {
          ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                fnName, 0,0,0,0,0);

          return ZL303XX_INVALID_POINTER;
       }
   }

   pStream = *ppStream;
   ackData = &rxTlv->data.ackCancel;

   /* Warning removal */
   (void)ptpData; (void)pPort; (void)txTlv;

   if (status == ZL303XX_OK)
   {
      /* Convert the PTP messageType to UniNeg messageType. */
      status = zl303xx_PtpV2MessageTypeToIndex(ackData->messageType, &uniNegMsgType);
   }

   if (status == ZL303XX_OK)
   {
      /* If this stream is cancelling its contracts to transmit and receive this
       * messageType, assume that the ACK_CANCEL for the transmit contract. */
      if (pStream->contractTx[uniNegMsgType].state == ZL303XX_PTP_CONTRACT_TX_CANCELLING)
      {
         UNI_NEG_CONTRACT_TRACE(2, pStream, "TX", uniNegMsgType, "received ACK_CANCEL", 0,0,0,0);
         zl303xx_PtpContractTxStateSet(pStream, uniNegMsgType, ZL303XX_PTP_CONTRACT_TX_IDLE);
      }
      else if (pStream->contractRx[uniNegMsgType].state == ZL303XX_PTP_CONTRACT_RX_CANCELLING)
      {
         UNI_NEG_CONTRACT_TRACE(2, pStream, "RX", uniNegMsgType, "received ACK_CANCEL", 0,0,0,0);
         zl303xx_PtpContractRxStateSet(pStream, uniNegMsgType, ZL303XX_PTP_CONTRACT_RX_IDLE);
      }
   }

   return status;
}

/* zl303xx_PtpUniNegStreamCreate */
/** 

   Creates a new stream needed to service a unicast negotiation contract
   REQUEST. This function will also generate the STREAM_CREATE event.

  Parameters:
   [in]   pPort      Port this stream will be created on.
   [in]   pDestAddr  Destination address of the new stream.
   [in]   msgType    Service request that triggered this stream creation.
   [in]   intvl      Requested log2 interval.
   [in]   extData    External data associated with the received REQUEST TLV.
   [out]  ppStream   Pointer to the created stream.

*******************************************************************************/
zlStatusE zl303xx_PtpUniNegStreamCreate(zl303xx_PortDataS *pPort,
                                        zl303xx_PortAddress *pDestAddr,
                                        zl303xx_MessageTypeE msgType,
                                        Sint8T intvl, void *extData,
                                        zl303xx_ClockStreamS **ppStream)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpStreamCreateS streamCreate;
   const char *msgTypeStr = zl303xx_PtpMessageTypeToStr(msgType);
   const char *fnName = "zl303xx_PtpUniNegStreamCreate";

   /* Check the input pointers. */
   if (status == ZL303XX_OK)
   {
       if ((pPort == NULL) ||
           (pDestAddr == NULL) ||
           (extData == NULL) ||
           (ppStream == NULL))
       {
          ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                fnName, 0,0,0,0,0);

          return ZL303XX_INVALID_POINTER;
       }
   }

   /* Add any Profile specific default values. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpProfileStreamStructInit(pPort->clock->config.profile, &streamCreate);
   }

   if (status == ZL303XX_OK)
   {
      streamCreate.portHandle = pPort->clockPortHandle;
      streamCreate.unicast = ZL303XX_TRUE;
      streamCreate.mode = ZL303XX_PTP_STREAM_USE_BMC;
      streamCreate.destAddr = *pDestAddr;
      streamCreate.createFlags |= ZL303XX_PTP_STREAM_CREATE_FIRE_EVENT;

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 2,
              "Port %u: Creating stream for %s TX service",
              pPort->clockPortHandle, msgTypeStr, 0,0,0,0);

      status = zl303xx_PtpStreamCreateInternal(&streamCreate, pPort, extData,
                                               ppStream);
   }

   if (status == ZL303XX_OK)
   {
      /* Update message counters now that the stream exists */
      (*ppStream)->counters.msg[ZL303XX_MSG_ID_SIGNALING].rx++;
      (*ppStream)->counters.msg[ZL303XX_MSG_ID_SIGNALING].rxProcessed++;
      (*ppStream)->countersInt.msg[ZL303XX_MSG_ID_SIGNALING].rx++;

      zl303xx_PtpStreamConfigIntvlSet(*ppStream, msgType, intvl);
   }

   return status;
}

/* zl303xx_PtpV2StartUnicastMsgFlow */
/** 

   Sends a Unicast Negotiation Contract Request for the specified stream.
   This may be a new request for service or a renewal of an existing service.

  Parameters:
   [in]  pPtpStream         Pointer to the clock stream structure associated
                                 with this connection. (Can not be NULL).
   [in]  uniNegMsgTypeList  A bit-mask list of the unicast message types for
                                 which to request message transmission.

  Return Value:
   Nothing

*******************************************************************************/
zlStatusE zl303xx_PtpV2StartUnicastMsgFlow(
      zl303xx_ClockStreamS *pPtpStream,
      Uint32T uniNegMsgTypeList)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtpV2StartUnicastMsgFlow";
   zl303xx_PtpV2SignalTlvS txTlv;                       /* TLV for the message. */
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;   /* Index for the loop. */
   /* Assemble the Unicast TLVs into a local buffer.
    * The largest number of TLVs will be the number of supported messageTypes. */
   Uint8T pUniNegTlvBuf[PTP_NEGOTIATED_NUM_MSG_TYPES * ZL303XX_PTP_V2_UNI_NEG_REQUEST_TLV_LEN];
   Uint16T packed, uniNegTlvBufLen = 0;

   /* Check the function input Parameter Pointer */
   if (status == ZL303XX_OK)
   {
       if (pPtpStream == NULL)
       {
        ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
              fnName, 0,0,0,0,0);
        
        return ZL303XX_INVALID_POINTER;
      }
   }


   if (status == ZL303XX_OK)
   {
      /* Send a REQUEST TLV for each message in the list. */
      for ((uniNegMsgType = PTP_NEGOTIATED_MSG_ID_SYNC);
           (uniNegMsgType < PTP_NEGOTIATED_NUM_MSG_TYPES);
           (uniNegMsgType++))
      {
         /* Check every message type against the bitMask-list. */
         if (uniNegMsgTypeList & (1 << uniNegMsgType))
         {
            /* Set the type attribute for the REQUEST TLV every time (in case
             * it is updated by the SEND routine. */
            txTlv.tlvType = PTP_TLV_REQUEST_UNICAST_TX;

            /* Convert internal type to external message type */
            txTlv.data.request.messageType = Zl303xx_UnicastIdxToMsgType[uniNegMsgType];

            /* Add the desired packet interval & duration. */
            txTlv.data.request.logInterMessagePeriod = pPtpStream->contractRx[uniNegMsgType].req.interval;

            /* Adjust if Best-Effort Negotiation is enabled. */
            {
               zl303xx_MessageTypeE ptpMsgType;
               (void)zl303xx_PtpV2IndexToMessageType(uniNegMsgType, &ptpMsgType);

               /* Adjust the Rate for Best-Effort Negotiation if enabled. */
               if (pPtpStream->config.unicastNegotiation.bestEffort[ptpMsgType].enabled == ZL303XX_TRUE)
               {
                  txTlv.data.request.logInterMessagePeriod += pPtpStream->unicast.bestEffort[ptpMsgType].retryCount;
               }
            }

            txTlv.data.request.durationField = pPtpStream->contractRx[uniNegMsgType].req.durationSec;

            /* Add the REQUEST TLV to the buffer. */
            packed = zl303xx_PtpV2SignalTlvPack(&txTlv, pUniNegTlvBuf + uniNegTlvBufLen);
            if (packed == 0)
            {
               /* The TLV was not constructed correctly.
                * Display an error and quit.         */
               ZL303XX_TRACE_ALWAYS("%s: TLV ERROR!",
                     fnName, 0,0,0,0,0);

               uniNegTlvBufLen = 0;
               status = ZL303XX_DATA_CORRUPTION;

               break;   /* No need to continue. */
            }

            /* Only allow a certain number of REQUESTs to be sent
             * before locking out this master. */
            {
               Uint8T prevRemainingTimeoutRequests = pPtpStream->contractRx[uniNegMsgType].retry.timeout.remainingRequests;

               zl303xx_PtpContractRequestUnicastTx(
                     &pPtpStream->unicast.lockout,
                     &pPtpStream->contractRx[uniNegMsgType].retry,
                     &pPtpStream->config.unicastNegotiation.retry,
                     pPtpStream->clockStreamHandle,
                     GET_UNICAST_MSG_STR(uniNegMsgType));

               /* If there are no more requests allowed for the stream,
                * do not process any more requests. */
               if (prevRemainingTimeoutRequests == 0)
               {
                  uniNegTlvBufLen = 0;
                  break;
               }
            }

            uniNegTlvBufLen += packed;

            UNI_NEG_CONTRACT_TRACE(2, pPtpStream, "RX", uniNegMsgType, "sent REQUEST", 0,0,0,0);
            UNI_NEG_CONTRACT_TRACE(3, pPtpStream, "RX", uniNegMsgType, "   period=%d, duration=%u",
                                   txTlv.data.request.logInterMessagePeriod,
                                   txTlv.data.request.durationField, 0,0);

            /* This will be sent so set the renegotiate flag to FALSE so this
             * function doesn't get called more than once by the state
             * machine, within a single state evaluation. */
            pPtpStream->contractRx[uniNegMsgType].renegotiate = ZL303XX_FALSE;

            /* Initialize the data used to detect interval changes. */
            zl303xx_PtpV2UniNegIntvlDetectInit(
               pPtpStream, txTlv.data.request.messageType,
               txTlv.data.request.logInterMessagePeriod);
         }
      }  /* FOR loop */

      /* Send the buffer if there is anything to send. */
      if (uniNegTlvBufLen != 0)
      {
         status = zl303xx_PtpV2UnicastNegTlvInitiate(NULL, pPtpStream, NULL,
                                                     pUniNegTlvBuf, uniNegTlvBufLen);
      }
   }

   return status;
}  /* END zl303xx_PtpV2StartUnicastMsgFlow */

/* zl303xx_PtpV2StopUnicastMsgFlow */
/** 

   Cancels the Unicast Negotiation Contracts for the specified messageTypes
   on the stream provided.

  Parameters:
   [in]  pPtpStream         Pointer to the clock stream structure associated
                                 with this connection. (Can not be NULL).
   [in]  uniNegMsgTypeList  A bit-mask list of the unicast message types for
                                 which to cancel message transmission.

  Return Value:
   Nothing

*******************************************************************************/
zlStatusE zl303xx_PtpV2StopUnicastMsgFlow(
      zl303xx_ClockStreamS *pPtpStream,
      Uint32T uniNegMsgTypeList)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtpV2StopUnicastMsgFlow";
   zl303xx_PtpV2SignalTlvS txTlv;                       /* TLV for the message. */
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;   /* Index for the loop. */
   /* Assemble the Unicast TLVs into a local buffer.
    * The largest number of TLVs will be the number of supported messageTypes. */
   Uint8T pUniNegTlvBuf[PTP_NEGOTIATED_NUM_MSG_TYPES * ZL303XX_PTP_V2_UNI_NEG_CANCEL_TLV_LEN];
   Uint16T packed, uniNegTlvBufLen = 0;     /* Overall packed TLV buffer length. */

   /* Check the function input Parameter Pointer */
   if (status == ZL303XX_OK)
   {
       if (pPtpStream == NULL)
       {
           ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                 fnName, 0,0,0,0,0);

           return ZL303XX_INVALID_POINTER;
       }
   }


   /* Confirm that we support the message type */
   if (status == ZL303XX_OK)
   {
      for (uniNegMsgType = PTP_NEGOTIATED_MSG_ID_SYNC;
           uniNegMsgType < PTP_NEGOTIATED_NUM_MSG_TYPES;
           uniNegMsgType++)
      {
         /* Check every message type against the bitMask-list. */
         if (uniNegMsgTypeList & (1 << uniNegMsgType))
         {
            /* Set the type attribute for the CANCEL TLV every time (in
             * case it is updated by the SEND routine). */
            txTlv.tlvType = PTP_TLV_CANCEL_UNICAST_TX;

            /* Convert internal type to external message type */
            txTlv.data.cancel.messageType = Zl303xx_UnicastIdxToMsgType[uniNegMsgType];

            /* Add the Cancel TLV to the buffer. */
            packed = zl303xx_PtpV2SignalTlvPack(&txTlv,
                                                pUniNegTlvBuf + uniNegTlvBufLen);
            if (packed == 0)
            {
               /* The TLV was not constructed correctly. Display an error and quit. */
               ZL303XX_TRACE_ALWAYS("%s: TLV ERROR!", fnName, 0,0,0,0,0);

               status = ZL303XX_DATA_CORRUPTION;
               break;   /* No need to continue. */
            }

            uniNegTlvBufLen += packed;

            if (pPtpStream->contractRx[uniNegMsgType].state == ZL303XX_PTP_CONTRACT_RX_CANCELLING)
            {
               UNI_NEG_CONTRACT_TRACE(2, pPtpStream, "RX", uniNegMsgType, "sent CANCEL", 0,0,0,0);
            }
            else
            {
               UNI_NEG_CONTRACT_TRACE(2, pPtpStream, "TX", uniNegMsgType, "sent CANCEL", 0,0,0,0);

               /* Don't allow this contract type to be granted again, unless
                * explicitly allowed. */
               zl303xx_PtpGrantEntryUpdate(pPtpStream->portData,
                                           &pPtpStream->config.destAddr,
                                           txTlv.data.cancel.messageType,
                                           ZL303XX_FALSE);
            }
         }
      }  /* FOR LOOP */
   }

   /* Send the entire Cancel TLV buffer to the Initiate Routine (if one exists).
    * It will send the TLVs based on the Stacked-TLVs setting of the clock. */
   if (status == ZL303XX_OK)
   {
      if (uniNegTlvBufLen > 0)
      {
         status = zl303xx_PtpV2UnicastNegTlvInitiate(NULL, pPtpStream,
                                    &pPtpStream->unicast.farEndPortIdentity,
                                    pUniNegTlvBuf, uniNegTlvBufLen);
      }
   }

   return status;
}  /* END zl303xx_PtpV2StopUnicastMsgFlow */

/* zl303xx_PtpV2DoUnicastStreamOverallState */
/** 

   Performs the operations to bring up a clock stream from the current node
   (as slave) to the specified master device.

  Parameters:
   [in]   stream  Pointer to an existing stream.

  Return Value:
   Nothing

  Notes:
   A clock stream is fully established when it has announce, sync and delay
   response flows all successfully negotiated with the far end.

*******************************************************************************/
void zl303xx_PtpV2DoUnicastStreamOverallState(zl303xx_ClockStreamS *stream)
{
    zlStatusE status = ZL303XX_OK;
   zl303xx_PtpContractRxS *anncRx, *syncRx, *drespRx, *peerRespRx;
   const char *fnName = "zl303xx_PtpV2DoUnicastStreamOverallState";

   /* Check the function input Parameter Pointer */
   if (status == ZL303XX_OK)
   {
       if (stream == NULL)
       {
           ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                 fnName, 0,0,0,0,0);

           return;
       }
   }

   /* Conditions that mean this state machine may not run. */
   if ((stream->config.mode == ZL303XX_PTP_STREAM_MASTER_ONLY) ||
       (stream->pUmtEntry == NULL))
   {
      return;
   }

   /* Makes the code easier to read. */
   anncRx     = &stream->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE];
   syncRx     = &stream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC];
   drespRx    = &stream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP];
   peerRespRx = &stream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP];

   /* By default, assume all contracts are off. */
   anncRx->negotiate     = ZL303XX_FALSE;
   syncRx->negotiate     = ZL303XX_FALSE;
   drespRx->negotiate    = ZL303XX_FALSE;
   peerRespRx->negotiate = ZL303XX_FALSE;

   /* Conditions that cause all active contracts to be canceled. */
   if ((stream->portData->portSuperState != PTP_PORT_INT_STATE_ACTIVE) ||
       (stream->portData->config.unicastNegotiation.enabled == ZL303XX_FALSE) ||
       (stream->unicast.lockout.remainingSec > 0))
   {
      /* All are set to OFF above so just return. */
      return;
   }

   /* Determine which contracts should be negotiated. */
   if (stream->portData->config.mixedMode == ZL303XX_FALSE)
   {
      anncRx->negotiate = stream->pUmtEntry->entryConfig.reqAnnc;

      /* Return early if the Announce contract is not being requested. This will
       * cancel all existing contracts. */
      if (anncRx->negotiate == ZL303XX_FALSE)
      {
         return;
      }

      /* If the Announce contract has been successfully negotiated, determine
       * which other contracts to negotiate. */
      if (anncRx->remainingSec > 0)
      {
         /* For SLAVE and UNCALIBRATED, negotiate based on user config */
         if ((stream->streamSubstate == PTP_STREAM_SUBSTATE_UNCALIBRATED) ||
             (stream->streamSubstate == PTP_STREAM_SUBSTATE_SLAVE) ||
             (stream->config.unicastMonitorTiming == ZL303XX_TRUE))
         {
            syncRx->negotiate  = stream->pUmtEntry->entryConfig.reqSync;

            if (stream->portData->config.delayMechanism == ZL303XX_DELAY_MECHANISM_E2E)
            {
               drespRx->negotiate = stream->pUmtEntry->entryConfig.reqDelayResp;
            }
            else if (stream->portData->config.delayMechanism == ZL303XX_DELAY_MECHANISM_P2P)
            {
               peerRespRx->negotiate = stream->pUmtEntry->entryConfig.reqPdelayResp;
            }
         }
         /* For LISTENING, all Timing contracts will be cancelled (in line with the
          * definition of LISTENING in the IEEE-1588-2008 Std) unless unicastMonitorTiming
          * is enabled. */
      }
   }
   else
   {
      if (stream->portData->config.delayMechanism == ZL303XX_DELAY_MECHANISM_E2E)
      {
         drespRx->negotiate = stream->pUmtEntry->entryConfig.reqDelayResp;
      }
      else if (stream->portData->config.delayMechanism == ZL303XX_DELAY_MECHANISM_P2P)
      {
         peerRespRx->negotiate = stream->pUmtEntry->entryConfig.reqPdelayResp;
      }
   }
}

/* zl303xx_PtpV2DoUnicastMsgState */
/** 

   Performs the operations to bring up a clock stream from the current node (as slave)
   to the specified master device.

  Parameters:
   [in]   pPtpStream   Pointer to a clock stream structure

  Return Value:
   Nothing

  Notes:
   A clock stream is fully established when it has announce, sync and delay response
   flows all successfully negotiated with the far end.

*******************************************************************************/
void zl303xx_PtpV2DoUnicastMsgState(zl303xx_ClockStreamS *pPtpStream)
{
    zlStatusE status = ZL303XX_OK;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;
   const char *fnName = "zl303xx_PtpV2DoUnicastMsgState";

   /* Check the function input Parameter Pointer */
   if (status == ZL303XX_OK)
   {
       if (pPtpStream == NULL)
       {
           ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                 fnName, 0,0,0,0,0);

           return;
       }
   }

   /* If the overall port state is not active then all streams must go idle immediately */
   if (pPtpStream->portData->portSuperState != PTP_PORT_INT_STATE_ACTIVE)
   {
      for (uniNegMsgType = PTP_NEGOTIATED_MSG_ID_SYNC; uniNegMsgType < PTP_NEGOTIATED_NUM_MSG_TYPES; uniNegMsgType++)
      {
         zl303xx_PtpContractRxStateSet(pPtpStream, uniNegMsgType, ZL303XX_PTP_CONTRACT_RX_IDLE);
         zl303xx_PtpContractTxStateSet(pPtpStream, uniNegMsgType, ZL303XX_PTP_CONTRACT_TX_IDLE);
      }
   }
}

/* zl303xx_PtpV2UnicastQueryInterval */
/** 

   Sends unicast contract requests for all message types in the REQUESTING state.

  Parameters:
   [in,out]  pStream  Stream to start message flow for.

*******************************************************************************/
void zl303xx_PtpV2UnicastQueryInterval(zl303xx_ClockStreamS *pStream)
{
    zlStatusE status = ZL303XX_OK;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;
   /* Bit-masked list of unicast message types. */
   Uint32T uniNegMsgTypeList = 0;
   const char *fnName = "zl303xx_PtpV2UnicastQueryInterval";

   /* Check the function input Parameter Pointer */
   if (status == ZL303XX_OK)
   {
       if (pStream == NULL)
       {
           ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                 fnName, 0,0,0,0,0);

           return;
       }
   }

   for (uniNegMsgType = PTP_NEGOTIATED_MSG_ID_SYNC;
        uniNegMsgType < PTP_NEGOTIATED_NUM_MSG_TYPES;
        uniNegMsgType++)
   {
      if (pStream->contractRx[uniNegMsgType].state == ZL303XX_PTP_CONTRACT_RX_REQUESTING)
      {
         /* Increment the failed request count */
         if (pStream->portData->failedUnicastRequestCount < 0xFFFFFFFF)
         {
            pStream->portData->failedUnicastRequestCount++;
         }

         uniNegMsgTypeList |= 1 << uniNegMsgType;
      }
   }

   /* Send a unicast request for these messages */
   if (uniNegMsgTypeList != 0)
   {
      (void)zl303xx_PtpV2StartUnicastMsgFlow(pStream, uniNegMsgTypeList);
   }
}

/* zl303xx_PtpV2GetMsgTypePortTotalPps */
/** 

   Calculates a port's totalPps counter for a specific message type. Optionally,
   disregards a stream's contribution to the totalPps.

  Parameters:
   [in]  pPtpPort           The port to calculate totalPps for.
   [in]  pPtpStreamIgnore   The stream to ignore when calculating totalPps (Optional).
   [in]  uniMsgType         The specific unicast message type to update.

  Return Value:  Uint32T  The total Port PPS rate.

*******************************************************************************/
Uint32T zl303xx_PtpV2GetMsgTypePortTotalPps(
      zl303xx_PortDataS *pPtpPort,
      zl303xx_ClockStreamS *pPtpStreamIgnore,
      zl303xx_V2UnicastNegotiatedMsgTypeE uniMsgType)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T totalPps = 0;
   zl303xx_ClockStreamS *currStream;
   const char *fnName = "zl303xx_PtpV2GetMsgTypePortTotalPps";

   /* Check the input pointers. */
   if (status == ZL303XX_OK)
   {
       if (pPtpPort == NULL) 
       {
          ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                fnName, 0,0,0,0,0);

          return 0;
       }
   }

   /* For every stream in the transmitting state, add the negotiated PPS to the
    * port's total PPS. Ignore packet rates < 1. */
   ZL303XX_PTP_FOR_EACH_STREAM(&pPtpPort->streamList, currStream)
   {
      /* Do not add the provided stream's negotiated PPS into the port's total
       * PPS because this function is only called when a contract is being renewed. */
      if (currStream == pPtpStreamIgnore)
      {
         continue;
      }

      if (currStream->contractTx[uniMsgType].remainingSec > 0)
      {
         /* Ignores rates below 1PPS. */
         totalPps += zl303xx_PtpV2UnicastIntvlToPps(currStream->contractTx[uniMsgType].grant.interval);
      }
   }

   return totalPps;
}

/* zl303xx_PtpV2GetMsgTypeClockTotalPps */
/** 

   Calculates a clock's totalPps counter for a specific message type. Optionally,
   disregards a stream's contribution to the totalPps.

  Parameters:
   [in]  pPtpClock          The port to calculate totalPps for.
   [in]  pPtpStreamIgnore   The stream to ignore when calculating totalPps (Optional).
   [in]  uniMsgType         The specific unicast message type to update.

  Return Value:  Uint32T  The total Clock PPS rate.

*******************************************************************************/
Uint32T zl303xx_PtpV2GetMsgTypeClockTotalPps(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_ClockStreamS *pPtpStreamIgnore,
      zl303xx_V2UnicastNegotiatedMsgTypeE uniMsgType)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T totalClockPps = 0;
   zl303xx_PortDataS *pPtpPort = NULL;
   const char *fnName = "zl303xx_PtpV2GetMsgTypeClockTotalPps";

   /* Check the input pointers. */
   if (status == ZL303XX_OK)
   {
       if (pPtpClock == NULL)
       {
          ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                fnName, 0,0,0,0,0);

          return 0;
       }
   }

   /* For every port on the clock, add the negotiated PPS to the clock's total.
    * Ignore packet rates < 1.
    * Exclude the PPS of the stream (if provided). */
   ZL303XX_PTP_FOR_EACH_PORT(&pPtpClock->portList, pPtpPort)
   {
      totalClockPps += zl303xx_PtpV2GetMsgTypePortTotalPps(pPtpPort, pPtpStreamIgnore, uniMsgType);
   }

   return totalClockPps;
}

/* zl303xx_PtpV2GetPortContractCount */
/** 

   Calculates a port's total active contracts for a specific message type.
   Optionally, disregards a provided stream's contribution to the active count.

  Parameters:
   [in]   pPort    The port to calculate total contracts for.
   [in]   pStream  (Optional and it may be NULL) The stream to disregard when calculating
                        total contract count.
   [in]   type     The specific unicast message type to count.

  Return Value:  Uint32T  The total active contract count.

*******************************************************************************/
Uint32T zl303xx_PtpV2GetPortContractCount(zl303xx_PortDataS *pPort,
                                        zl303xx_ClockStreamS *pStream,
                                        zl303xx_V2UnicastNegotiatedMsgTypeE type)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T totalContracts = 0;
   zl303xx_ClockStreamS *currStream;
  const char *fnName = "zl303xx_PtpV2GetPortContractCount";

   /* Check the input pointers. */
   if (status == ZL303XX_OK)
   {
       if (pPort == NULL)
       {
          ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                fnName, 0,0,0,0,0);

          return 0;
       }
   }

   /* For every stream attached to the port count streams that have time
    * remaining in the contract.
    */
   ZL303XX_PTP_FOR_EACH_STREAM(&pPort->streamList, currStream)
   {
      /* Do not count the given stream if provided */
      if (currStream == pStream)
      {
         continue;
      }

      if (currStream->contractTx[type].remainingSec > 0)
      {
         ++totalContracts;
      }
   }

   return totalContracts;
}

/* zl303xx_PtpV2GetClockContractCount */
/** 

   Calculates a clock's total active contracts for a specific message type.
   Optionally, disregards a provided stream's contribution to the active count.

  Parameters:
   [in]   pClock    The clock to calculate total contracts.
   [in]   pStream  (Optional) The stream to disregard when calculating
                        total contract count.
   [in]   type     The specific unicast message type to count.

  Return Value:  Uint32T  The total active contract count.

*******************************************************************************/
Uint32T zl303xx_PtpV2GetClockContractCount(zl303xx_PtpClockS *pClock,
                                         zl303xx_ClockStreamS *pStream,
                                         zl303xx_V2UnicastNegotiatedMsgTypeE type)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T totalContracts = 0;
   zl303xx_PortDataS *currPort;
   const char *fnName = "zl303xx_PtpV2GetClockContractCount";

   /* Check the input pointers. */
   if (status == ZL303XX_OK)
   {
       if (pClock == NULL)
       {
          ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                fnName, 0,0,0,0,0);

          return 0;
       }
   }

   /* For every port attached to the clock count streams that have time
    * remaining in the contract.
    */
   ZL303XX_PTP_FOR_EACH_PORT(&pClock->portList, currPort)
   {
      totalContracts += zl303xx_PtpV2GetPortContractCount(currPort, pStream, type);
   }

   return totalContracts;
}

/* zl303xx_PtpContractRxStateSet */
/** 

   Performs any leaving- and entering-state actions for a specific unicast
   negotiation contract that a stream has been granted by another port (RX).

  Parameters:
   [in]   stream   Pointer to an existing stream.
   [in]   msgType  Message type contract to set the state for.
   [in]   state    New state of this contract.

*******************************************************************************/
void zl303xx_PtpContractRxStateSet(zl303xx_ClockStreamS *stream,
                                   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType,
                                   zl303xx_PtpContractRxStateE state)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpContractRxS *contractRx = &stream->contractRx[uniNegMsgType];
   const char *fnName = "zl303xx_PtpContractRxStateSet";

   /* Check the input pointers. */
   if (status == ZL303XX_OK)
   {
       if (stream == NULL)
       {
           ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                 fnName, 0,0,0,0,0);

           return;
       }
   }

   /* Do nothing if attempting to set the same state. */
   if (state == contractRx->state)
      return;

   /* Leaving state actions */
   if (contractRx->state == ZL303XX_PTP_CONTRACT_RX_REQUESTING)
   {
      /* Initialize the per-message RETRY data. If the stream is entering the
       * receiving state, wipe all records of previous request failures such
       * that if a stream attempts to renew or request a new contract, it will
       * send three requests on its initial attempt. */
      if (state == ZL303XX_PTP_CONTRACT_RX_ACTIVE)
      {
         contractRx->retry.denial.prevFailed = ZL303XX_FALSE;
         contractRx->retry.timeout.prevFailed = ZL303XX_FALSE;
      }

      zl303xx_PtpContractMsgRetryStatusInit(&contractRx->retry,
                                            &stream->config.unicastNegotiation.retry);
   }

   /* Entering state actions */
   switch (state)
   {
      case ZL303XX_PTP_CONTRACT_RX_IDLE :
      {
         contractRx->negotiate = ZL303XX_FALSE;
         contractRx->remainingSec = 0;
         contractRx->renegotiate = ZL303XX_FALSE;
         contractRx->remainingCancelMsgs = 0;

         zl303xx_PtpV2SetRequiredTimers(stream);
         zl303xx_PtpUniNegPtsfRejectedSet(stream->clockStreamHandle, uniNegMsgType, ZL303XX_FALSE);
         zl303xx_PtpUniNegPtsfIntvlTooHighSet(stream, uniNegMsgType, NULL);
         break;
      }

      case ZL303XX_PTP_CONTRACT_RX_REQUESTING :
      {
         /* Initialize the per-message RETRY data. */
         zl303xx_PtpContractMsgRetryStatusInit(&contractRx->retry,
                                               &stream->config.unicastNegotiation.retry);
         break;
      }

      case ZL303XX_PTP_CONTRACT_RX_ACTIVE :
      {
         contractRx->renegotiate = ZL303XX_FALSE;
         zl303xx_PtpV2SetRequiredTimers(stream);
         break;
      }

      case ZL303XX_PTP_CONTRACT_RX_CANCELLING :
      {
         contractRx->remainingCancelMsgs = MAX_NUM_CANCEL_ATTEMPTS;
         break;
      }

      default :
         break;
   }

   UNI_NEG_CONTRACT_TRACE(4, stream, "RX", uniNegMsgType, "state %s to %s",
                          CONTRACT_RX_STATE_TO_STR(contractRx->state),
                          CONTRACT_RX_STATE_TO_STR(state), 0,0);

   contractRx->state = state;
}

/* zl303xx_PtpContractTxStateSet */
/** 

   Performs any leaving- and entering-state actions for a specific unicast
   negotiation contract that a stream has granted (TX).

  Parameters:
   [in]   stream   Pointer to an existing stream.
   [in]   msgType  Message type contract to set the state for.
   [in]   state    New state of this contract.

*******************************************************************************/
void zl303xx_PtpContractTxStateSet(zl303xx_ClockStreamS *stream,
                                   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType,
                                   zl303xx_PtpContractTxStateE state)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpContractTxS *contractTx;
   const char *fnName = "zl303xx_PtpContractTxStateSet";

   /* Check the function input Parameter Pointer */
   if (status == ZL303XX_OK)
   {
       if (stream == NULL)
       {
           ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                 fnName, 0,0,0,0,0);

           return;
       }
   }
   
   contractTx = &stream->contractTx[uniNegMsgType];

   /* Do nothing if attempting to set the same state. The TX_ACTIVE state is
    * an exception because a new message rate can be renegotiated. */
   if (state == contractTx->state && state != ZL303XX_PTP_CONTRACT_TX_ACTIVE)
      return;

   /* Entering state actions */
   switch (state)
   {
      case ZL303XX_PTP_CONTRACT_TX_IDLE :
      {
         contractTx->grant.interval = 0;
         contractTx->grant.durationSec = 0;
         contractTx->remainingSec = 0;
         contractTx->remainingCancelMsgs = 0;

         zl303xx_PtpV2SetRequiredTimers(stream);
         break;
      }

      case ZL303XX_PTP_CONTRACT_TX_ACTIVE :
      {
         zl303xx_PtpV2SetRequiredTimers(stream);
         break;
      }

      case ZL303XX_PTP_CONTRACT_TX_CANCELLING :
      {
         contractTx->remainingCancelMsgs = MAX_NUM_CANCEL_ATTEMPTS;
         break;
      }

      default :
         break;
   }

   UNI_NEG_CONTRACT_TRACE(4, stream, "TX", uniNegMsgType, "state %s to %s",
                          CONTRACT_TX_STATE_TO_STR(contractTx->state),
                          CONTRACT_TX_STATE_TO_STR(state), 0,0);

   contractTx->state = state;
}

/* zl303xx_PtpV2UnicastRateToPps */
/** 

   Converts a log2 interval to PPS. If the interval is greater than 1 second,
   0 is returned.

  Parameters:
   [in]   intvl  Log2 interval to convert.

  Return Value:
     Uint32T  intvl converted to PPS.
     0        intvl is > 0.

*******************************************************************************/
static Uint32T zl303xx_PtpV2UnicastIntvlToPps(Sint8T intvl)
{
   if (intvl <= 0)
   {
      return (Uint32T)1 << -intvl;
   }

   /* We make the assumption that any contract below 1-PPS adds no load to the
    * CPU. The number of contracts on a clock/port will be the limiting factor
    * in this case, not the PPS. */
   return 0;
}

/* zl303xx_PtpV2CheckMsgTypeClockLimits */
/** 

   Checks if the log2 interval in a contract request will exceed a clock's
   totalPpsMax service limit for the requested message type.

  Parameters:
   [in]  pPtpClock   The clock the contract request is associated with.
   [in]  pPtpStream  Pointer to the stream for the contract request.
                           If NULL: this is a new contract request.
                              ELSE: this is a contract renewal so ignore the
                                    existing PPS of this contract.
   [in]  uniMsgType  The unicast message type of the contract request.
   [in]  log2intvl   Log2 interval in the contract request.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  log2intvl would cause totalPps to exceed totalPpsMax.

  Notes:    Assumes the Clock & Stream pointers and uniMsgType are valid.

*******************************************************************************/
static zlStatusE zl303xx_PtpV2CheckMsgTypeClockLimits(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_V2UnicastNegotiatedMsgTypeE uniMsgType,
      Sint8T log2intvl)
{
   const char *fnName = "zl303xx_PtpV2CheckMsgTypeClockLimits";

   /* Convert the 'unicast' message type to a standard definition. */
   zl303xx_MessageTypeE ptpMsgType;
   zlStatusE status = zl303xx_PtpV2IndexToMessageType(uniMsgType, &ptpMsgType);

   /* Verify the Clock service Limits */
   if (status == ZL303XX_OK)
   {
      /* If totalPpsMax == 0, reject all contracts for this message type. */
      if (pPtpClock->config.unicastNegotiation.totalPpsMax[ptpMsgType] == 0)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 1,
               "%s: Unicast Request for message(%u) disabled for Clock(%lu)::",
               fnName,
               ptpMsgType,
               pPtpClock->config.unicastNegotiation.totalPpsMax[ptpMsgType],
               0,0,0);

         status = ZL303XX_PARAMETER_INVALID;
      }

      /* Determine if the new PPS exceeds the Clock Limits. */
      else
      {
         /* Rates below 1PPS are set to 0.    */
         Uint32T newPps = zl303xx_PtpV2UnicastIntvlToPps(log2intvl);

         /* Get the total PPS count active for this clock and message type.
          * In the case of a renewal, ignore the PPS of the active stream (if
          * pPtpStream is NULL, then this is a first-time request). */
         Uint32T activeClockPps = zl303xx_PtpV2GetMsgTypeClockTotalPps(pPtpClock, pPtpStream, uniMsgType);

         /* Adding the requested PPS rate must not exceed the totalPpsMax (or rollover). */
         if (((newPps + activeClockPps) > pPtpClock->config.unicastNegotiation.totalPpsMax[ptpMsgType]) ||
             ((newPps + activeClockPps) < newPps)) /* Rollover case */
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 1,
               "%s: Clock (%d): Unicast Request for message(%u) exceeds Clock Limit(%lu):: (activePps=%lu, newPps=%lu).",
               fnName, pPtpClock->clockHandle,
               ptpMsgType,
               pPtpClock->config.unicastNegotiation.totalPpsMax[ptpMsgType],
               activeClockPps, newPps);

            status = ZL303XX_PARAMETER_INVALID;
         }
      }
   }

   return status;
}

/* zl303xx_PtpV2CheckMsgTypePortLimits */
/** 

   Checks if the log2 interval in a contract request will exceed a port's
   totalPpsMax service limit for the requested message type.

  Parameters:
   [in]  pPtpPort    The port the contract request arrived on.
   [in]  pPtpStream  Pointer to the stream for the contract request.
                           If NULL: this is a new contract request.
                           ELSE: this is a contract renewal so ignore the
                                 existing PPS of this contract.
   [in]  uniMsgType  The unicast message type of the contract request.
   [in]  log2intvl   Log2 interval in the contract request.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  log2intvl would cause totalPps to exceed totalPpsMax.

  Notes:    Assumes the Port & Stream pointers and uniMsgType are valid.

*******************************************************************************/
static zlStatusE zl303xx_PtpV2CheckMsgTypePortLimits(
      zl303xx_PortDataS *pPtpPort,
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_V2UnicastNegotiatedMsgTypeE uniMsgType,
      Sint8T log2intvl)
{
   const char *fnName = "zl303xx_PtpV2CheckMsgTypePortLimits";

   /* Convert the 'unicast' message type to a standard definition. */
   zl303xx_MessageTypeE ptpMsgType;
   zlStatusE status = zl303xx_PtpV2IndexToMessageType(uniMsgType, &ptpMsgType);

   /* Rates below 1PPS are set to 0. We still check that the Port has
    * enabled Grants of the requested message type.    */
   Uint32T newPps = zl303xx_PtpV2UnicastIntvlToPps(log2intvl);

   /* Verify the Port service Limits */
   if (status == ZL303XX_OK)
   {
      /* Get the total PPS count for this port and message type.
       * In the case of a renewal, ignore the PPS of the active stream (if
       * pPtpStream is NULL, then this is a first-time request). */
      Uint32T activePortPps = zl303xx_PtpV2GetMsgTypePortTotalPps(pPtpPort, pPtpStream, uniMsgType);

          /* If totalPpsMax == 0, reject all contracts for this message type. */
      if ((pPtpPort->config.unicastNegotiation.totalPpsMax[ptpMsgType] == 0) ||
          /* Adding the requested PPS rate must not exceed the totalPpsMax. */
          ((newPps + activePortPps) > pPtpPort->config.unicastNegotiation.totalPpsMax[ptpMsgType]))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 1,
            "%s: Port(%d): Unicast Request for message(%u) exceeds Port Limit(%lu):: (activePps=%lu, newPps=%lu).",
            fnName, pPtpPort->clockPortHandle,
            ptpMsgType,
            pPtpPort->config.unicastNegotiation.totalPpsMax[ptpMsgType],
            activePortPps, newPps);

         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   return status;
}

/* zl303xx_PtpUniNegRequestValid */
/** 

   Checks if the parameters of a unicast service request are valid.

  Parameters:
   [in]   pPort       Port that the service will be created on.
   [in]   msgType     messageType service requested.
   [in]   duration    Contract duration, in seconds.
   [out]  pMalformed  (Optional) TRUE is returned if a parameter check fails
                           because the REQUEST TLV contained invalid data.

*******************************************************************************/
zlStatusE zl303xx_PtpUniNegRequestValid(zl303xx_PortDataS *pPort,
                                        zl303xx_MessageTypeE msgType,
                                        Uint32T duration,
                                        zl303xx_BooleanE *pMalformed)
{
   zlStatusE status = ZL303XX_PARAMETER_INVALID;
   zl303xx_BooleanE malformed = ZL303XX_FALSE;
   const char *msgTypeStr = zl303xx_PtpMessageTypeToStr(msgType);
   const char *fnName = "zl303xx_PtpUniNegRequestValid";

   /* Check the function input Parameter Pointer */
   /* pMalformed CAN BE NULL */
   if (pPort == NULL)
   {
       ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
             fnName, 0,0,0,0,0);

       return ZL303XX_INVALID_POINTER;
   }
   

   if (ZL303XX_CHECK_UNI_NEG_MSG_TYPE_VALID(msgType) != ZL303XX_OK)
   {
      /* Unicast negotiation does not support the requested message type.
         The standard requires us to send a denial */
      malformed = ZL303XX_TRUE;
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 1,
            "Port %u: Cannot create TX service: unknown messageType=%d",
            pPort->clockPortHandle, msgType, 0,0,0,0);
   }
   else if (pPort->clock->config.defaultDS.slaveOnly == ZL303XX_TRUE)
   {
      /* Cannot grant a contract if we are in SLAVE only mode */
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 1,
            "Port %u: Cannot create %s TX service: clock is slave-only",
            pPort->clockPortHandle, msgTypeStr, 0,0,0,0);
   }
   else if (duration == 0)
   {
      /* The standard is unclear about what to do in this case. We will
         respond by denying the request */
      /* We could remove this clause since the success path would respond
         with duration = 0 anyway. However, this is good for logging. */
      malformed = ZL303XX_TRUE;
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 1,
            "Port %u: Cannot create %s TX service: durationField=0",
            pPort->clockPortHandle, msgTypeStr, 0,0,0,0);
   }
   else
   {
       status = ZL303XX_OK;
   }

   if (pMalformed != NULL)
   {
      *pMalformed = malformed;
   }

   return status;
}

/* zl303xx_PtpV2CheckUnicastRequest */
/** 

   Verifies that the received request for a Unicast Tx contract is valid.
   The contract is denied if:
      - the request is of a Path_Delay_Resp message type
      - the request is other than Sync, Announce or Delay_Resp message type
      - the current node is not in MASTER mode
      - the requested contract duration = 0 (see comments below)
      - the requested messageType is disabled on the current node
      - the requested message rate exceeds the port's service limits
      - the requested messageType exceeds the port's service limits
      - the requested messageType exceeds the clocks's service limits

  Parameters:
   [in]   request        The received Unicast-Request-Tx message
   [in]   pPort          Info on the configuration of the port
   [in]   pStream        Info on the configuration of the stream (may be NULL
                            if stream not created yet)
   [in]   uniNegMsgType  Local unicast message type used for array indexing
   [in]   srcAddr        The source port address of the request TLV.

   [out]  grant          The granted Unicast-Request-Tx message. A grant
                              duration of 0 indicates the request is denied.

*******************************************************************************/
static void zl303xx_PtpV2CheckUnicastRequest(
                  zl303xx_PtpV2SignalRequestS *request,
                  zl303xx_PortDataS *pPort,
                  zl303xx_ClockStreamS *pStream,
                  zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType,
                  zl303xx_PortAddress *srcAddr,
                  zl303xx_PtpV2SignalGrantS *grant)
{
   const char *fnName = "zl303xx_PtpV2CheckUnicastRequest";
   zl303xx_BooleanE malformed;
   zl303xx_PtpContractLimitsS *portLimits, *clockLimits;
   Uint32T totalContracts, totalClockContracts;
   const char *msgTypeStr = zl303xx_PtpMessageTypeToStr(request->messageType);

   portLimits = &pPort->contractLimits[uniNegMsgType];
   clockLimits = &pPort->clock->contractLimits[uniNegMsgType];

   /* Check the requested contract parameters & local config */

   /* Only grant the request if the proper conditions are met */
   /* The duration field != 0 will indicate if the contract was granted */
   /* Initialization of grant structure*/
   grant->durationField  = 0;
   grant->renewalInvited = ZL303XX_FALSE;
   /* Grant has the same logInterMessagePeriod as request */
   grant->logInterMessagePeriod = request->logInterMessagePeriod;

   /* A GRANT is the same message type as a REQUEST */
   grant->messageType = request->messageType;

   /* ERROR scenarios that we will respond to */
   /******************************************/
   if (zl303xx_PtpUniNegRequestValid(pPort, request->messageType,
                                     request->durationField,
                                     &malformed) != ZL303XX_OK)
   {
      if (malformed)
      {
         zl303xx_IncrementMalformedPktCount(pPort);
      }
   }
   else if (portLimits->maxDurationSec == 0)
   {
      /* There is nothing in the standard concerning when to deny a grant.
       * For now we can use this flag for each individual message type. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 1,
            "Port %u: Cannot create %s TX service: configured with maxDurationSec=0",
            pPort->clockPortHandle, msgTypeStr, 0,0,0,0);
   }

   /* Check port's PPS limit */
   else if (zl303xx_PtpV2CheckMsgTypePortLimits(pPort, pStream,
                           uniNegMsgType, request->logInterMessagePeriod) != ZL303XX_OK)
   {
      /* A TRACE message is printed within the routine above. */

      /* Generate a PTP Event for the Contract DENIAL. */
      zl303xx_PtpEventContractRejected(pPort->clock, pPort->clockPortHandle,
                                       grant->messageType,
                                       ZL303XX_PTP_CONTRACT_RATE_REJECTED);
   }

   /* Check clock's PPS limit */
   else if (zl303xx_PtpV2CheckMsgTypeClockLimits(pPort->clock, pStream,
                           uniNegMsgType, request->logInterMessagePeriod) != ZL303XX_OK)
   {
      /* A TRACE message is printed within the routine above. */
      zl303xx_PtpEventContractRejected(pPort->clock, pPort->clockPortHandle,
                                       grant->messageType,
                                       ZL303XX_PTP_CONTRACT_RATE_REJECTED);
   }

   /* Check if the Port has been configured to Deny all contracts. */
   else if (pPort->config.unicastNegotiation.denyServiceRequests == ZL303XX_TRUE)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 1,
            "Port %u: Cannot create %s TX service: denyServiceRequests configured to reject",
            pPort->clockPortHandle, msgTypeStr, 0,0,0,0);

      zl303xx_PtpEventContractRejected(pPort->clock, pPort->clockPortHandle,
                                       grant->messageType,
                                       ZL303XX_PTP_CONTRACT_PORT_DENY_REJECTED);
   }

   /* Check port's grant table config */
   else if (zl303xx_PtpGrantTblCheck(pPort, srcAddr, request->messageType) == ZL303XX_FALSE)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 1,
            "Port %u: Cannot create %s TX service: grant table configured to reject",
            pPort->clockPortHandle, msgTypeStr, 0,0,0,0);

      zl303xx_PtpEventContractRejected(pPort->clock, pPort->clockPortHandle,
                                       grant->messageType,
                                       ZL303XX_PTP_CONTRACT_GRANT_TBL_REJECTED);
   }

   /* Check the AST (if enabled). */
   /******************************************/
   else if ((pPort->config.acceptableSlaveTableEnabled == ZL303XX_TRUE) &&
            (NULL == zl303xx_PtpTableFind(&pPort->accSlaveTbl, srcAddr)))
   {
      char srcAddrStr[ZL303XX_PTP_ADDR_STR_LEN_MAX];

      (void)zl303xx_PtpConvertPortAddressToStr(srcAddr, srcAddrStr);

      /* Discarding received event message from node not in the acceptable
       * slave table. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "DENYING Request from node not in AST: %s",
            srcAddrStr, 0,0,0,0,0);

      zl303xx_PtpEventContractRejected(pPort->clock, pPort->clockPortHandle,
                                       grant->messageType,
                                       ZL303XX_PTP_CONTRACT_RATE_REJECTED);
   }

   /* Possible successful Grant conditions   */
   /******************************************/
   else
   {
      /* Get port and clock contract counts, making sure to discount the
       * current stream since this may be a contract renewal. */
      totalContracts = zl303xx_PtpV2GetPortContractCount(pPort, pStream,
                                                         uniNegMsgType);

      totalClockContracts = zl303xx_PtpV2GetClockContractCount(pPort->clock,
                                                               pStream,
                                                               uniNegMsgType);

      /* Check port-level contract limits */
      if (portLimits->maxCount != (Uint32T) -1 &&
          totalContracts >= portLimits->maxCount)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 1,
               "Port %u: Cannot create %s TX service: max port contracts reached (%u)",
               pPort->clockPortHandle, msgTypeStr, portLimits->maxCount, 0,0,0);

         zl303xx_PtpEventContractRejected(pPort->clock, pPort->clockPortHandle,
                                          uniNegMsgType,
                                          ZL303XX_PTP_CONTRACT_COUNT_MAX_REACHED);
      }

      /* Check clock-level contract limits */
      else if (clockLimits->maxCount != (Uint32T) -1 &&
               totalClockContracts >= clockLimits->maxCount)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 1,
               "Port %u: Cannot create %s TX service: max clock contracts reached (%u)",
               pPort->clockPortHandle, msgTypeStr, clockLimits->maxCount, 0,0,0);

         zl303xx_PtpEventContractRejected(pPort->clock, pPort->clockPortHandle,
                                          uniNegMsgType,
                                          ZL303XX_PTP_CONTRACT_COUNT_MAX_REACHED);
      }

      /* Check Port-Level Duration Limits and Profile Specific Duration ranges. */
      else if ((request->durationField > portLimits->maxDurationSec) ||
               (zl303xx_PtpProfileUnicastDurationCheck(
                                    pPort->clock->config.profile,
                                    pPort->clock->config.profileStrict,
                                    &request->durationField)  != ZL303XX_OK))
      {
         ZL303XX_TRACE_ALWAYS(
               "%s: Port=%d: Unicast Request duration REJECTED (%s)!",
               fnName, pPort->clockPortHandle,
               (pPort->clock->config.profileStrict) ? "STRICT":"NOT STRICT", 0,0,0);

         zl303xx_PtpEventContractRejected(pPort->clock, pPort->clockPortHandle,
                                          grant->messageType,
                                          ZL303XX_PTP_CONTRACT_DURATION_REJECTED);
      }

      /* Check Port-Level Packet Rate Limits. */
      else if (request->logInterMessagePeriod < portLimits->minIntvl)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 3,
               "%s: Port=%d Unicast Request interval %d for msgType %s REJECTED!",
               fnName, pPort->clockPortHandle,
               request->logInterMessagePeriod,
               GET_UNICAST_MSG_STR(uniNegMsgType), 0,0);

         /* Recommend the lower rate to the client.   */
         grant->logInterMessagePeriod = portLimits->minIntvl;
         grant->renewalInvited = ZL303XX_TRUE;

         zl303xx_PtpEventContractRejected(pPort->clock, pPort->clockPortHandle,
                                          grant->messageType,
                                          ZL303XX_PTP_CONTRACT_RATE_REJECTED);
      }

      else if (pPort->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1)
      {
         /* Check message interval */
         if (/* Telecom STRICT: DENY intervals too high or too low */
               zl303xx_PtpTelecomLogIntvlCheck(
                     request->messageType,
                     &request->logInterMessagePeriod,
                     pPort->clock->config.profileStrict) != ZL303XX_OK ||

               /* Telecom Non-strict: DENY intervals only if they are too low */
               request->logInterMessagePeriod < portLimits->minIntvl)
         {
            request->durationField = 0;

            ZL303XX_TRACE_ALWAYS("%s: Port=%d (G.8265.1) Unicast Request REJECTED (%s)!",
                                 fnName, pPort->clockPortHandle,
                                 (pPort->clock->config.profileStrict) ? "STRICT":"NOT STRICT",
                                 0, 0, 0);

            zl303xx_PtpEventContractRejected(pPort->clock, pPort->clockPortHandle,
                                             grant->messageType,
                                             ZL303XX_PTP_CONTRACT_RATE_REJECTED);
         }

         /* Set the interval and duration of the contract */
         grant->logInterMessagePeriod = request->logInterMessagePeriod;
         grant->durationField         = request->durationField;
      }

      /* Request looks good */
      else
      {
         /* The message interval was checked above so must be good. */
         grant->logInterMessagePeriod = request->logInterMessagePeriod;

         /* The duration was checked above and must have passed. Therefore GRANT
          * the requested contract duration. */
         grant->durationField = request->durationField;

         /* Indicate that renewal of the contract is likely */
         grant->renewalInvited = ZL303XX_TRUE;
      }
   }
}  /* END zl303xx_PtpV2CheckUnicastRequest */

/* zl303xx_PtpV2CheckPortSupportsNegotiation */
/** 

   Verifies that the target Port should support a Unicast Negotiation request.

  Parameters:
   [in]   pPtpPort        Info on the configuration of the port
   [in]   uniNegMsgType   Local unicast message type used for array indexing

  Return Value:
     ZL303XX_TRUE   if the support conditions are met.
     ZL303XX_FALSE  otherwise

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtpV2CheckPortSupportsNegotiation(zl303xx_PortDataS *pPtpPort,
                                 zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType)
{
   zlStatusE status = ZL303XX_OK;
   /* Set the default return value. */
   zl303xx_BooleanE uniNegSupported = ZL303XX_FALSE;
   zl303xx_PortStateE bPortState;
   const char *fnName = "zl303xx_PtpV2CheckPortSupportsNegotiation";

   /* Check the function input Parameter Pointer */
   if (status == ZL303XX_OK)
   {
       if (pPtpPort == NULL)
       {
           ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                 fnName, 0,0,0,0,0);

           return ZL303XX_FALSE;
       }
   }

   /* Get the current state of the port. */
   bPortState = zl303xx_GetV2PortState(pPtpPort);

   /* Support conditions:
    * Mixed mode:
    *    - requires the Port to be in MASTER state.
    *    - only Delay_Requests messages are allowed.
    * Otherwise:
    *    - if the Port is MASTER then all message types are OK
    *    - if the Port is LISTENING then only ANNOUNCE types are OK.  */
   /*
    *                        Mixed Mode
    *                +----------+-------------+
    *                |  TRUE    |    FALSE    |
    *                +----------+-------------+
    *    P   MASTER  |  D_RESP  |   ALL MSGs  |
    *    O           +----------+-------------+
    *    R   LISTEN  |   FAIL   |  ANNC ONLY  |
    *    T           +----------+-------------+
    *        ELSE    |          FAIL          |
    *                +------------------------+      */

   if (pPtpPort->config.mixedMode == ZL303XX_TRUE)
   {
      if ((bPortState == ZL303XX_PORT_STATE_MASTER) &&
          (uniNegMsgType == PTP_NEGOTIATED_MSG_ID_DELAY_RESP))
      {
         uniNegSupported = ZL303XX_TRUE;
      }
   }
   else  /* Not Mixed mode. */
   {
      if (bPortState == ZL303XX_PORT_STATE_MASTER)
      {
         uniNegSupported = ZL303XX_TRUE;
      }
      else if((bPortState == ZL303XX_PORT_STATE_LISTENING) &&
              (uniNegMsgType == PTP_NEGOTIATED_MSG_ID_ANNOUNCE))
      {
         uniNegSupported = ZL303XX_TRUE;
      }
   }

   return uniNegSupported;
}  /* END zl303xx_PtpV2CheckPortSupportsNegotiation */

/* zl303xx_PtpV2StreamAllServicesRequired */
/** 

   Determines if the request for Unicast Contract Negotiation meets the conditions
   to request all message contracts that comprise the stream.

   The conditions to determine if this is the initial timing request are:
   - one or both timing services will be negotiated.
   - any timing service to be negotiated was not previously running (not a
     renewal or re-negotiation so the remainingSec duration is 0).
   - the Announce flow associated with this stream is ACTIVE.

   As well, in order to require all services in a request:
   - the stacked TLV setting of the Port must be enabled
   OR
   - the 'allServicesOn1stRequest' override for the clock must be enabled.

  Parameters:
   [in]  pPtpStream         Pointer to the clock stream structure associated
                                 with this connection. (Can not be NULL).

  Return Value:
     ZL303XX_TRUE   if the required conditions are met.
     ZL303XX_FALSE  otherwise

  Notes:
   Assumes all input pointers are valid.

*******************************************************************************/
static zl303xx_BooleanE zl303xx_PtpV2StreamAllServicesRequired(
      zl303xx_ClockStreamS *pPtpStream)
{
   /* Set the default return value. */
   zl303xx_BooleanE allServicesRequired = ZL303XX_FALSE;

   /* Message type loop counter. */
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;

   /* Pointer to the port & clock associated with this stream. */
   zl303xx_PortDataS *pPtpPort = pPtpStream->portData;
   zl303xx_PtpClockS *pPtpClock = pPtpPort->clock;

   if (pPtpPort->config.delayMechanism == ZL303XX_DELAY_MECHANISM_P2P)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 3,
            "Evaluate all-services:: AllEn (%d): StkEn (%d): SYNC(En=%d:Sec=%d): PDelay-SEC(En=%d:Sec=%d)",
            pPtpClock->stackAllServicesOn1stRequest,
            pPtpPort->config.unicastNegotiation.stackedTlvEn,
            pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].negotiate,
            pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].remainingSec,
            pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].negotiate,
            pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].remainingSec);
   }
   else  /* E2E or disabled */
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 3,
            "Evaluate all-services:: AllEn (%d): StkEn (%d): SYNC(En=%d:Sec=%d): Delay-SEC(En=%d:Sec=%d)",
            pPtpClock->stackAllServicesOn1stRequest,
            pPtpPort->config.unicastNegotiation.stackedTlvEn,
            pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].negotiate,
            pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].remainingSec,
            pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].negotiate,
            pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].remainingSec);
   }

   /* CONDITION 1: Stacked TLV or the allServices override has to be ENABLED. */
   if ((pPtpStream->portData->clock->stackAllServicesOn1stRequest == ZL303XX_TRUE) ||
       (pPtpStream->portData->config.unicastNegotiation.stackedTlvEn == ZL303XX_TRUE))
   {
      /* CONDITION 2: Either of the TIMING flows:
       *              a) must be required.
       *              b) can not have been already up. */
      if (
          /* Check if a SYNC contract will be included but is not active. */
         ((pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].negotiate == ZL303XX_TRUE) &&
          (pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].remainingSec == 0))
          ||
         /* Check if a DELAY contract will be included but is not active. */
         ((pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].negotiate == ZL303XX_TRUE) &&
          (pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].remainingSec == 0))
          ||
         /* Check if a PEER-DELAY contract will be included but is not active. */
         ((pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].negotiate == ZL303XX_TRUE) &&
          (pPtpStream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].remainingSec == 0)))
      {
         allServicesRequired = ZL303XX_TRUE;
      }
   }

   /* If 'allServices' are required, flag all active flows to renegotiate.  */
   if (allServicesRequired == ZL303XX_TRUE)
   {
      for (uniNegMsgType = PTP_NEGOTIATED_MSG_ID_SYNC;
           uniNegMsgType < PTP_NEGOTIATED_NUM_MSG_TYPES;
           uniNegMsgType++)
      {
         if ((pPtpStream->contractRx[uniNegMsgType].negotiate == ZL303XX_TRUE) &&
             (pPtpStream->contractRx[uniNegMsgType].remainingSec > 0))
         {
            pPtpStream->contractRx[uniNegMsgType].renegotiate = ZL303XX_TRUE;
         }
      }
   }

   return allServicesRequired;
}  /* END zl303xx_PtpV2StreamAllServicesRequired */

/* zl303xx_PtpV2MessageTypeToIndex */
/** 

   Verifies that the received message type is a valid unicast negotiation
   message type as per section 16.1 of the IEEE 1588 V2 spec. It also converts
   the message type to a local type for more convenient internal use.

  Parameters:
   [in]   messageType     The received message type field
   [out]  uniNegMsgType   Local unicast message type for array indexing

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_PtpV2MessageTypeToIndex(zl303xx_MessageTypeE messageType,
                           zl303xx_V2UnicastNegotiatedMsgTypeE *uniNegMsgType)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtpV2MessageTypeToIndex";

   /* Check the function input Parameter Pointer */
   if (status == ZL303XX_OK)
   {
       if (uniNegMsgType == NULL)
       {
           ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                 fnName, 0,0,0,0,0);

           return ZL303XX_PARAMETER_INVALID;
       }
   }

   switch (messageType)
   {
      case ZL303XX_MSG_ID_SYNC :
      {
         *uniNegMsgType = PTP_NEGOTIATED_MSG_ID_SYNC;
         break;
      }

      case ZL303XX_MSG_ID_ANNOUNCE :
      {
         *uniNegMsgType = PTP_NEGOTIATED_MSG_ID_ANNOUNCE;
         break;
      }

      case ZL303XX_MSG_ID_DELAY_RESP :
      {
         *uniNegMsgType = PTP_NEGOTIATED_MSG_ID_DELAY_RESP;
         break;
      }

      case ZL303XX_MSG_ID_PEER_DELAY_RESP :
      {
         /* We do not support this but we have to respond */
         *uniNegMsgType = PTP_NEGOTIATED_MSG_ID_PDELAY_RESP;
         break;
      }

      /* fall-through for removal of switch-enum warning */
      case ZL303XX_MSG_ID_DELAY_REQ :
      case ZL303XX_MSG_ID_PEER_DELAY_REQ :
      case ZL303XX_MSG_ID_FOLLOWUP :
      case ZL303XX_MSG_ID_PEER_DELAY_FOLLOWUP :
      case ZL303XX_MSG_ID_SIGNALING :
      case ZL303XX_MSG_ID_MANAGEMENT :
      default :
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 1,
               "Unicast TLV message check error: Invalid message type request, type 0x%x",
               messageType, 0,0,0,0,0);

         /* Set an invalid value that will cause an error if used later */
         *uniNegMsgType = PTP_NEGOTIATED_NUM_MSG_TYPES;
         status = ZL303XX_PARAMETER_INVALID;
         break;
      }
   }

   return status;
}  /* END zl303xx_PtpV2MessageTypeToIndex */

/* zl303xx_PtpV2IndexToMessageType */
/** 

   Verifies that the message index is a valid local unicast negotiation type and
   converts it to the corresponding unicast message type as per section 16.1 of
   the IEEE 1588 V2 spec.

  Parameters:
   [in]   uniNegMsgType   Local unicast message type for array indexing
   [out]  messageType     The transmit message type field

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_PtpV2IndexToMessageType(zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType,
                                          zl303xx_MessageTypeE *messageType)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtpV2IndexToMessageType";

   /* Check the function input Parameter Pointer */
   if (status == ZL303XX_OK)
   {
       if (messageType == NULL)
       {
           ZL303XX_TRACE_ALWAYS("%s:: Input Pointer == NULL)",
                 fnName, 0,0,0,0,0);

           return ZL303XX_PARAMETER_INVALID;
       }
   }

   if (uniNegMsgType >= PTP_NEGOTIATED_NUM_MSG_TYPES)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 1,
            "Invalid unicast message type request, index %d",
            uniNegMsgType,0,0,0,0,0);

      /* Set a reserved value that will cause an error if used later */
      *messageType = (zl303xx_MessageTypeE)ZL303XX_PTP_INVALID;
      return ZL303XX_PARAMETER_INVALID;
   }

   *messageType = Zl303xx_UnicastIdxToMsgType[uniNegMsgType];
   return ZL303XX_OK;
}  /* END zl303xx_PtpV2IndexToMessageType */

/* zl303xx_PtpV2UniNegIntvlDetectInit */
/** 

   Initializes data used to detect a message interval change when configured for
   unicast negotiation.

  Parameters:
   [in]   pStream  Pointer to an existing stream.
   [in]   type     Message type to initialize.
   [in]   intvl    The interval being requested.

*******************************************************************************/
static void zl303xx_PtpV2UniNegIntvlDetectInit(zl303xx_ClockStreamS *pStream,
                                               zl303xx_MessageTypeE type,
                                               Sint8T intvl)
{
   if (pStream->msgParams[type].prevIntvl == PTP_V2_LMMI_NULL_VALUE)
   {
      pStream->msgParams[type].prevIntvl = intvl;
   }
}

/* zl303xx_PtpUniNegPtsfRejectedSet */
/** 

   Sets the appropriate PTSF-*Rejected flag.

  Parameters:
   [in]   streamHandle   Handle to an existing stream.
   [in]   uniNegMsgType  Message type flag to set.
   [in]   rejected       PTSF value to set.

*******************************************************************************/
static void zl303xx_PtpUniNegPtsfRejectedSet(zl303xx_PtpStreamHandleT streamHandle,
                              zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType,
                              zl303xx_BooleanE val)
{
   zl303xx_PtsfFlagE flag;

   if (uniNegMsgType == PTP_NEGOTIATED_MSG_ID_SYNC)
      flag = ZL303XX_PTSF_SYNC_REJECTED;
   else if (uniNegMsgType == PTP_NEGOTIATED_MSG_ID_DELAY_RESP)
      flag = ZL303XX_PTSF_DELAY_RESP_REJECTED;
   else if (uniNegMsgType == PTP_NEGOTIATED_MSG_ID_PDELAY_RESP)
      flag = ZL303XX_PTSF_PEER_DELAY_RESP_REJECTED;
   else if (uniNegMsgType == PTP_NEGOTIATED_MSG_ID_ANNOUNCE)
      flag = ZL303XX_PTSF_ANNOUNCE_REJECTED;
   else
      return;

   zl303xx_PtsfFlagSet(streamHandle, flag, val);
}

/* zl303xx_PtpUniNegPtsfIntvlTooHighSet */
/** 

   Sets the appropriate PTSF-*IntvlTooHigh flag.

  Parameters:
   [in]      streamHandle   Handle to an existing stream.
   [in]      uniNegMsgType  Message type flag to set.
   [in,out]  intvl          Message interval to check. The maximum acceptable
                                 interval is passed back. Set to NULL to
                                 for the PTSF flag to FALSE.

*******************************************************************************/
static void zl303xx_PtpUniNegPtsfIntvlTooHighSet(zl303xx_ClockStreamS *stream,
                              zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType,
                              Sint8T *intvl)
{
   Sint8T *maxIntvl = NULL;
   zl303xx_PtsfFlagE flag;

   if (stream->pUmtEntry == NULL)
      return;

   if (uniNegMsgType == PTP_NEGOTIATED_MSG_ID_SYNC)
   {
      maxIntvl = &stream->pUmtEntry->entryConfig.maxSyncIntvl;
      flag = ZL303XX_PTSF_SYNC_RATE_TOO_LOW;
   }
   else if (uniNegMsgType == PTP_NEGOTIATED_MSG_ID_DELAY_RESP)
   {
      maxIntvl = &stream->pUmtEntry->entryConfig.maxDelayRespIntvl;
      flag = ZL303XX_PTSF_DELAY_RESP_RATE_TOO_LOW;
   }
   else if (uniNegMsgType == PTP_NEGOTIATED_MSG_ID_PDELAY_RESP)
   {
      maxIntvl = &stream->pUmtEntry->entryConfig.maxPdelayRespIntvl;
      flag = ZL303XX_PTSF_PEER_DELAY_RESP_RATE_TOO_LOW;
   }
   else if (uniNegMsgType == PTP_NEGOTIATED_MSG_ID_ANNOUNCE)
   {
      maxIntvl = &stream->pUmtEntry->entryConfig.maxAnncIntvl;
      flag = ZL303XX_PTSF_ANNOUNCE_RATE_TOO_LOW;
   }
   else
   {
      /* Should never reach this point but to eliminate compiler warnings. */
      return;
   }

   if (intvl != NULL)
   {
      zl303xx_PtsfFlagSet(stream->clockStreamHandle, flag, (*intvl > *maxIntvl));
      *intvl = *maxIntvl;
   }
   else
   {
      zl303xx_PtsfFlagSet(stream->clockStreamHandle, flag, ZL303XX_FALSE);
   }
}

/* zl303xx_PtpContractRxStateRun */
/** 

   Performs any in-state actions for each unicast negotiation contract that
   a stream has been granted by another port (RX). Must be called once per
   second.

  Parameters:
   [in]   stream      Pointer to an existing stream.
   [out]  cancelList  Bit-masked list of message types to cancel. This list
                           is not initialized to 0 by this function.

*******************************************************************************/
static void zl303xx_PtpContractRxStateRun(zl303xx_ClockStreamS *stream,
                                          Uint32T *cancelList)
{
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;
   zl303xx_PtpContractRxS *contractRx;
   zl303xx_PtpContractRxStateE nextState;
   zl303xx_BooleanE expired;

   /* Have to check if 'allServices' are required for this stream. If so, then
    * all active flows are flagged for re-negotiation within the following routine.*/
   stream->unicast.requestAllServices = zl303xx_PtpV2StreamAllServicesRequired(stream);

   for (uniNegMsgType = PTP_NEGOTIATED_MSG_ID_SYNC;
        uniNegMsgType < PTP_NEGOTIATED_NUM_MSG_TYPES;
        uniNegMsgType++)
   {
      contractRx = &stream->contractRx[uniNegMsgType];
      nextState = contractRx->state;
      expired = ZL303XX_FALSE;

      if (contractRx->remainingSec > 0)
      {
         if (--contractRx->remainingSec == 0)
         {
            expired = ZL303XX_TRUE;
            UNI_NEG_CONTRACT_TRACE(2, stream, "RX", uniNegMsgType, "expired", 0,0,0,0);
         }
         else
         {
            UNI_NEG_CONTRACT_TRACE(5, stream, "RX", uniNegMsgType, "%lu seconds remaining",
                                   contractRx->remainingSec, 0,0,0);
         }
      }

      switch (contractRx->state)
      {
         case ZL303XX_PTP_CONTRACT_RX_IDLE :
         {
            if (contractRx->negotiate)
            {
               nextState = ZL303XX_PTP_CONTRACT_RX_REQUESTING;
            }
            break;
         }

         case ZL303XX_PTP_CONTRACT_RX_REQUESTING :
         {
            /* Check if this contract is still required. */
            if (contractRx->negotiate == ZL303XX_FALSE)
            {
               if (contractRx->remainingSec > 0)
               {
                  nextState = ZL303XX_PTP_CONTRACT_RX_CANCELLING;
               }
               else
               {
                  nextState = ZL303XX_PTP_CONTRACT_RX_IDLE;
               }
            }
            /* Check if this contract has expired while requesting */
            else if (expired)
            {
               zl303xx_PtpEventContractExpired(
                     stream, Zl303xx_UnicastIdxToMsgType[uniNegMsgType],
                     ZL303XX_PTP_CONTRACT_RX);

               nextState = ZL303XX_PTP_CONTRACT_RX_IDLE;
            }
            break;
         }

         /* Evaluate the current contract (both SLAVE & MASTER):
          * - A SLAVE may require a renewal of an existing contract. */
         /* For CONTRACT Renewal & Expiry, all available messageTypes are grouped if
          * the Stacked-TLV mode is enabled. A contract will renew
          * only if there is not a cancel pending for that message type. */
         case ZL303XX_PTP_CONTRACT_RX_ACTIVE :
         {
            /* Check if this contract is still required. */
            if (contractRx->negotiate == ZL303XX_FALSE)
            {
               nextState = ZL303XX_PTP_CONTRACT_RX_CANCELLING;
            }
            /* Check if this contract needs to be renewed. */
            else if ((contractRx->remainingSec <= contractRx->renewSec) ||
                     (contractRx->renegotiate == ZL303XX_TRUE))
            {
               nextState = ZL303XX_PTP_CONTRACT_RX_REQUESTING;
            }
            break;
         }

         case ZL303XX_PTP_CONTRACT_RX_CANCELLING :
         {
            /* If we have started cancelling, send another cancel message if our
             * cancel time out has not expired. */
            if (contractRx->remainingCancelMsgs > 0)
            {
               contractRx->remainingCancelMsgs--;
               *cancelList |= 1 << uniNegMsgType;
            }
            else
            {
               nextState = ZL303XX_PTP_CONTRACT_RX_IDLE;
            }
            break;
         }

         default :
            break;
      }

      /* Does nothing if nextState == contractRx->state */
      zl303xx_PtpContractRxStateSet(stream, uniNegMsgType, nextState);
   }
}

/* zl303xx_PtpContractTxStateRun */
/** 

   Performs any in-state actions for each unicast negotiation contract that
   a stream has granted (TX). Must be called once per second.

  Parameters:
   [in]   stream      Pointer to an existing stream.
   [out]  cancelList  Bit-masked list of message types to cancel. This list
                           is not initialized to 0 by this function.

*******************************************************************************/
static void zl303xx_PtpContractTxStateRun(zl303xx_ClockStreamS *stream,
                                          Uint32T *cancelList)
{
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;
   zl303xx_PtpContractTxS *contractTx;
   zl303xx_PtpContractTxStateE nextState;
   zl303xx_BooleanE expired;

   for (uniNegMsgType = PTP_NEGOTIATED_MSG_ID_SYNC;
        uniNegMsgType < PTP_NEGOTIATED_NUM_MSG_TYPES;
        uniNegMsgType++)
   {
      contractTx = &stream->contractTx[uniNegMsgType];
      nextState = contractTx->state;
      expired = ZL303XX_FALSE;

      if (contractTx->remainingSec > 0)
      {
         if (--contractTx->remainingSec == 0)
         {
            expired = ZL303XX_TRUE;
            UNI_NEG_CONTRACT_TRACE(2, stream, "TX", uniNegMsgType, "expired", 0,0,0,0);
         }
         else
         {
            UNI_NEG_CONTRACT_TRACE(5, stream, "TX", uniNegMsgType, "%lu seconds remaining",
                                   contractTx->remainingSec, 0,0,0);
         }
      }

      switch (contractTx->state)
      {
         case ZL303XX_PTP_CONTRACT_TX_ACTIVE :
         {
            if (expired)
            {
               /* This is the only case that implies a TX contract has expired.
                * This event doesn't need to be generated for the CANCELLING
                * state because that can only be reached with user intervention. */
               zl303xx_PtpEventContractExpired(
                     stream, Zl303xx_UnicastIdxToMsgType[uniNegMsgType],
                     ZL303XX_PTP_CONTRACT_TX);

               nextState = ZL303XX_PTP_CONTRACT_TX_IDLE;
            }
            break;
         }

         case ZL303XX_PTP_CONTRACT_TX_CANCELLING :
         {
            /* If we have started cancelling, send another cancel message if our
             * cancel time out has not expired. */
            if (contractTx->remainingCancelMsgs > 0)
            {
               contractTx->remainingCancelMsgs--;
               *cancelList |= 1 << uniNegMsgType;
            }
            else
            {
               nextState = ZL303XX_PTP_CONTRACT_TX_IDLE;
            }

            break;
         }

         case ZL303XX_PTP_CONTRACT_TX_IDLE :
         default :
            break;
      }

      /* Does nothing if nextState == contractTx->state */
      zl303xx_PtpContractTxStateSet(stream, uniNegMsgType, nextState);
   }
}

/****************   END   *****************************************************/
