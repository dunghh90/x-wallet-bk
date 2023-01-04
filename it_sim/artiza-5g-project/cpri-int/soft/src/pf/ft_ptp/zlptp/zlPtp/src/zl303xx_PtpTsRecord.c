
/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This module contains the types and routines to interface to Timestamp
*     Record tables. These records are required to synchronize the timestamps
*     for the PTP EVENT messages. For example, storing T3 of a Delay_Request
*     until the T4 is received from a Response.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_Trace.h"

#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpDatatypes.h"
#include "zl303xx_PtpInternalTypes.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpFunctionExport.h"
#include "zl303xx_Ptp_dep.h"
#include "zl303xx_Ptpd.h"

/*****************   DEFINES     **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

static void zl303xx_PtpTsRecordReady(
      zl303xx_ClockStreamS *stream,
      zl303xx_PtpTsRecordTblEntryS *record);

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/* Defines which data fields are required for each PTP EVENT Message in order
 * to signal a "complete record". The Indexes of this table align with the first
 * 4 definitions of the zl303xx_MessageTypeE.  */
static const Uint8T TsRecordReadyFlags[] = {
      /* Sync: As soon as a TX is available, issue a FollowUp (if needed). */
      PTP_TS_RECORD_RESERVED | PTP_TS_RECORD_HAVE_TX,

      /* Delay: Need both the T3 and T4 values to complete the record. */
      PTP_TS_RECORD_RESERVED | PTP_TS_RECORD_HAVE_TX | PTP_TS_RECORD_HAVE_RX,

      /* PDelay_Req: Need both the T1 & T4 but also the residence time (which
       * may come from a 1-step RESP or multiple 2-step methods). */
      PTP_TS_RECORD_RESERVED | PTP_TS_RECORD_HAVE_TX | PTP_TS_RECORD_HAVE_RX | PTP_TS_RECORD_HAVE_RES,

      /* PDelay_Resp: Need both the T2 & T3 to compute the residence time. */
      PTP_TS_RECORD_RESERVED | PTP_TS_RECORD_HAVE_RX | PTP_TS_RECORD_HAVE_TX
};

/*****************   IMPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/*****************   INTERNAL PTP FUNCTION DEFINITIONS   **********************/

/* zl303xx_PtpTsRecordReserve */
/** 

   Reserves an entry in the stream's time stamp record array. This needs to be
   done when a Delay_Req message is sent, or a Sync message is sent from a
   two-step clock.

  Parameters:
   [in]   stream     Pointer to the stream
   [in]   msgType    Ptp Message type to reserve
   [in]   seqId      Value of the sequenceId field in the PTP message

  Notes:   Assumes all input values are valid.

*******************************************************************************/
void zl303xx_PtpTsRecordReserve(
      zl303xx_ClockStreamS *stream,
      zl303xx_MessageTypeE msgType,
      Uint16T seqId)
{
   zl303xx_PtpTsRecordTblEntryS *record = NULL;

   if (ZL303XX_CHECK_EVENT_MSG_TYPE_VALID(msgType) != ZL303XX_OK)
   {
      /* If this is an unknown type, then use '0' by default. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 1,
            "Unknown messageType (%u): Stream %u: TS record [--] seqId=%u reserved",
            msgType, stream->clockStreamHandle, seqId, 0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Unknown messageType (%u): Stream %u: TS record [--] seqId=%u reserved",
            msgType, stream->clockStreamHandle, seqId, 0,0,0);

      msgType = 0;
   }

   record = &(stream->tsRecordArray[msgType][seqId & stream->tsRecordIdxMask]);

   /* Start a new record */
   if (record->flags != PTP_TS_RECORD_EMPTY)
   {
      /* The next record is not empty. May indicate a problem or perhaps
         overload -- continue anyway. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 2,
            "Stream %u: TS record [--] seqId=%u reserved (overwrite seqId=%u, msgType=%d flags=0x%0X)",
            stream->clockStreamHandle, seqId, record->seqId, msgType, record->flags, 0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 2,
            "Stream %u: TS record [--] seqId=%u reserved (overwrite seqId=%u, msgType=%d flags=0x%0X)",
            stream->clockStreamHandle, seqId, record->seqId, msgType, record->flags, 0);
   }
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 2,
            "Stream %u: TS record [--] seqId=%u reserved",
            stream->clockStreamHandle, seqId, 0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
            "Stream %u: TS record [--] seqId=%u reserved",
            stream->clockStreamHandle, seqId, 0,0,0,0);
   }

   record->flags       = PTP_TS_RECORD_RESERVED;
   record->readyMask   = TsRecordReadyFlags[msgType];
   record->messageType = msgType;
   record->seqId       = seqId;
}

/* zl303xx_PtpTsRecordRegisterEgress */
/** 

   This function is called when the transmit time stamp of a Sync (from a two-
   step clock), Delay_Req, Peer_Delay_Req Peer_Delay_Resp is available from
   hardware. If the time stamp is from a:
      Sync message; a Follow_Up message is generated
      Delay_Req message; it is stored in the stream TsRecord Table until the
            matching Delay_Resp is received. If the Delay_Resp has already arrived,
            the time stamp pair gets registered with the timing recovery algorithm.
      Peer_Delay_Req message; actions are the same as for Delay_Req.
      Peer_Delay_Resp message; a Peer_Delay_Follow_Up message is generated.

  Parameters:
   [in]  stream      Pointer to the stream the PTP message was sent from
   [in]  msgType     PTP Message type transmitted.
   [in]  seqId       Value of the sequenceId field in the PTP message
   [in]  txTs        Transmit time stamp of the PTP message

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
void zl303xx_PtpTsRecordRegisterEgress(
      zl303xx_ClockStreamS *stream,
      zl303xx_MessageTypeE msgType,
      Uint16T seqId,
      zl303xx_TimeStamp *txTs)
{
   zl303xx_PtpTsRecordTblEntryS *record = NULL;

   if (ZL303XX_CHECK_EVENT_MSG_TYPE_VALID(msgType) != ZL303XX_OK)
   {
      /* If this is an unknown type, then use '0' by default. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 1,
            "Egress: Unknown messageType (%u): Stream %u: TS record [--] seqId=%u",
            msgType, stream->clockStreamHandle, seqId, 0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Egress: Unknown messageType (%u): Stream %u: TS record [--] seqId=%u",
            msgType, stream->clockStreamHandle, seqId, 0,0,0);

      msgType = 0;
   }

   record = &(stream->tsRecordArray[msgType][seqId & stream->tsRecordIdxMask]);

   /* Check if the record corresponding to this sequenceId has been reserved */
   if (!(record->flags & PTP_TS_RECORD_RESERVED) ||  /* record empty */
        (record->flags & PTP_TS_RECORD_HAVE_TX)  ||  /* record TX already filled */
        (record->seqId != seqId))                    /* record reserved with wrong seqId */
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 1,
            "Egress: Stream %u: TS record [TX] seqId=%u, msgType=%d, flags=0x%x error (not reserved, TX or seqId)",
            stream->clockStreamHandle, seqId, msgType, record->flags, 0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Egress: Stream %u: TS record [TX] seqId=%u, msgType=%d, flags=0x%x error (not reserved, TX or seqId)",
            stream->clockStreamHandle, seqId, msgType, record->flags, 0,0);

      /* Discard this timestamp */
      return;
   }

   /* Found a matching record; add the TX data. */
   record->txTs = *txTs;
   record->flags |= PTP_TS_RECORD_HAVE_TX;

   ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 2,
         "Egress: Stream %u: TS record [TX] seqId=%u added (%u:%u)",
         stream->clockStreamHandle, seqId,
         record->txTs.secondsField.lo, record->txTs.nanosecondsField, 0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
         "Egress: Stream %u: TS record [TX] seqId=%u added (%u:%u)",
         stream->clockStreamHandle, seqId,
         record->txTs.secondsField.lo, record->txTs.nanosecondsField, 0,0);

   /* If the record already had the RX data, then it is now complete. */
   if (record->flags == record->readyMask)
   {
      /* Send it to the event handler to be processed. */
      zl303xx_PtpTsRecordReady(stream, record);

      /* Mark this time stamp record as empty. */
      record->flags = PTP_TS_RECORD_EMPTY;
   }
}

/* zl303xx_PtpTsRecordRegisterIngress */
/** 

   This function is called when the RX time stamp of a Delay_Resp or
   Peer_Delay_Req message becomes available. The following actions are taken:
      Delay_Resp:
         - the original DELAY_REQ entry should have been previously RESERVED
           in the stream TsRecord table.
         - the RX timestamp (from within the received Delay_Resp message) is
           added to the Reserved DELAY_REQ record.
         - if both the TX and RX times are present, the timestamp pair gets
           registered with the timing recovery algorithm.
      Peer_Delay_Req:
         - 1-Step:
            - The Peer_Delay_Resp message header fields are updated and the
              response is issued.
         - 2-Step:
            - The receiver RESERVES a PEER_DELAY_RESP entry in the stream
              TsRecord table.
            - the RX timestamp and other header fields of the Peer_Delay_Req
              message are added to the reserved TsRecord entry.
            - The receiver issues a Peer_Delay_Resp message (the egress timestamp
              will be inserted into the record via the
              zl303xx_PtpTsRecordRegisterEgress() routine).

  Parameters:
   [in]  stream      Pointer to the stream the Delay_Resp was received on
   [in]  srcPortId   sourcePortIdentity of the sender (may be NULL)
   [in]  msgType     PTP Message type received.
   [in]  seqId       Value of the sequenceId field in the PTP message
                           (matching Delay_Req and Delay_Resp msgs have the same)
   [in]  rxTs        Value of the receiveTimestamp field in the Delay_Resp
   [in]  correction  Value of the correction field in the Delay_Resp

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
void zl303xx_PtpTsRecordRegisterIngress(
      zl303xx_ClockStreamS *stream,
      zl303xx_PortIdentity *srcPortId,
      zl303xx_MessageTypeE msgType,
      Uint16T seqId,
      zl303xx_TimeStamp *rxTs,
      Uint64S *correction)
{
   zl303xx_PtpTsRecordTblEntryS *record = NULL;

   if (ZL303XX_CHECK_EVENT_MSG_TYPE_VALID(msgType) != ZL303XX_OK)
   {
      /* If this is an unknown type, then use '0' by default. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 1,
            "Ingress: Unknown messageType (%u): Stream %u: TS record [--] seqId=%u reserved",
            msgType, stream->clockStreamHandle, seqId, 0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Ingress: Unknown messageType (%u): Stream %u: TS record [--] seqId=%u reserved",
            msgType, stream->clockStreamHandle, seqId, 0,0,0);

      msgType = 0;
   }

   record = &(stream->tsRecordArray[msgType][seqId & stream->tsRecordIdxMask]);

   /* Check if the record corresponding to this sequenceId has been reserved */
   if (!(record->flags & PTP_TS_RECORD_RESERVED) ||  /* record empty */
        (record->flags & PTP_TS_RECORD_HAVE_RX)  ||  /* record TX already filled */
        (record->seqId != seqId))                    /* record reserved with wrong seqId */
   {
      /* If the record has not been reserved, then this is an error because a
       * Delay_Resp cannot be received until a Delay_Req has been sent. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 1,
            "Ingress: Stream %u: TS record [RX] seqId=%u error (no reserved entry)",
            stream->clockStreamHandle, seqId, 0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Ingress: Stream %u: TS record [RX] seqId=%u error (no reserved entry)",
            stream->clockStreamHandle, seqId, 0,0,0,0);

      /* Discard this timestamp */
      return;
   }

   /* Found a matching record; add the RX data. */
   if (srcPortId)       {record->srcPortId = *srcPortId;}
   record->rxTs         = *rxTs;
   record->rxCorrection = *correction;
   record->flags |= PTP_TS_RECORD_HAVE_RX;

   ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 2,
         "Ingress: Stream %u: TS record [RX] seqId=%u added (%u:%u)",
         stream->clockStreamHandle, seqId,
         rxTs->secondsField.lo, rxTs->nanosecondsField, 0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
         "Ingress: Stream %u: TS record [RX] seqId=%u added (%u:%u)",
         stream->clockStreamHandle, seqId,
         rxTs->secondsField.lo, rxTs->nanosecondsField, 0,0);

   /* If the record already had the TX data, then it is now complete. */
   if (record->flags == record->readyMask)
   {
      /* Send it to the event handler to be processed. */
      zl303xx_PtpTsRecordReady(stream, record);

      /* Mark this time stamp record as empty. */
      record->flags = PTP_TS_RECORD_EMPTY;
   }
}

/* zl303xx_PtpTsRecordRegisterPdelayResp */
/** 

   This function is called when the RX time stamp of a Peer Delay_Req becomes
   available (which happens when a Peer Delay_Resp is received).

  Parameters:
   [in]  stream      Pointer to the stream the PDelay_Resp was received on
   [in]  pPtpMsgRx   Pointer to the received PDelay_Resp message structure.
                           Contains: seqId, bOneStep flag, correctionField,
                                     transmitting sourcePortIdentity,
                                     PDelay_Resp::requestingPortIdentity,
                                     PDelay_Resp::requestReceiptTimestamp
   [in]  rxTs        Value of the received timestamp of the PDelay_Resp

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
void zl303xx_PtpTsRecordRegisterPdelayResp(
      zl303xx_ClockStreamS *stream,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      zl303xx_TimeStamp *rxTs)
{
   /* Get some data from the received message header. */
      Uint16T        seqId    = pPtpMsgRx->header.sequenceId;
      zl303xx_BooleanE bOneStep = ((!(pPtpMsgRx->header.flagField & PTP2_FLAGS_TWO_STEP)) ? ZL303XX_TRUE : ZL303XX_FALSE);

   /* This PEER_DELAY_RESP was triggered due to an original PEER_DELAY_REQ. */
   zl303xx_PtpTsRecordTblEntryS *record =
      &(stream->tsRecordArray[ZL303XX_MSG_ID_PEER_DELAY_REQ][seqId & stream->tsRecordIdxMask]);

   /* Check if the record corresponding to this sequenceId has been reserved */
   if (!(record->flags & PTP_TS_RECORD_RESERVED) ||  /* record empty */
        (record->flags & PTP_TS_RECORD_HAVE_RX)  ||  /* record RX already filled */
        (record->flags & PTP_TS_RECORD_HAVE_RES) ||  /* record Resident Time already filled */
        (record->seqId != seqId))                    /* record reserved with wrong seqId */
   {
      /* If the record has not been reserved, then this is an error because a
       * PDelay_Resp cannot be received until a PDelay_Req has been sent. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 1,
            "Stream %u: TS record [PD-RX] seqId=%u error (no reserved entry)",
            stream->clockStreamHandle, seqId, 0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %u: TS record [PD-RX] seqId=%u error (no reserved entry)",
            stream->clockStreamHandle, seqId, 0,0,0,0);

      /* Assume the item is no longer reserved since a previous response has
       * already been received and processed. Raise the MULTIPLE_PEER_RESPONSE
       * Event. */
      if (!(record->flags & PTP_TS_RECORD_RESERVED))
      {
         (void)zl303xx_PtpEventMultiplePeerResp(
                                 stream, seqId,
                                 rxTs,
                                 &pPtpMsgRx->header.sourcePortIdentity,
                                 &pPtpMsgRx->data.pdelayResp.requestingPortIdentity);
      }

      /* Discard this timestamp */
      return;
   }

   /* Found a matching record; add the RX data. */
   record->rxTs = *rxTs;
   record->flags |= PTP_TS_RECORD_HAVE_RX;

   /* May be 0 if a follow-up is pending but fill anyway. */
   record->rxCorrection = pPtpMsgRx->header.correctionField;

   /* For a 1-step response, the correctionField contains the far-end
    * resident time, so the record will be complete. */
   if (bOneStep == ZL303XX_TRUE)
   {
      record->flags |= PTP_TS_RECORD_HAVE_RES;
   }

   /* 2-step : we will calculate it or receive it in the followup. */
   else
   {
      record->rxT2 = pPtpMsgRx->data.pdelayResp.requestReceiptTimestamp;
   }

   ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 2,
         "Stream %u: TS record [RX] seqId=%u added (%u:%u)",
         stream->clockStreamHandle, seqId,
         rxTs->secondsField.lo, rxTs->nanosecondsField, 0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
         "Stream %u: TS record [RX] seqId=%u added (%u:%u)",
         stream->clockStreamHandle, seqId,
         rxTs->secondsField.lo, rxTs->nanosecondsField, 0,0);

   /* If the record has all the data, then it is now complete. */
   if (record->flags == record->readyMask)
   {
      /* Send it to the event handler to be processed. */
      zl303xx_PtpTsRecordReady(stream, record);

      /* Mark this time stamp record as empty. */
      record->flags = PTP_TS_RECORD_EMPTY;
   }
}

/* zl303xx_PtpTsRecordRegisterPdelayFollowup */
/** 

   This function is called when the FollowUp message of a Peer Delay_Resp is
   received, marking the end of the record.

  Parameters:
   [in]  stream      Pointer to the stream the PDelay_FollowUp was received on
   [in]  seqId       Value of the sequenceId field in the PTP message
                           (PDelay_Req, _Resp and _FollowUp msgs have the same)
   [in]  correction  Value of the correction field in the PDelay_FollowUp
   [in]  pktTsField  Value of the receive T3 Timestamp from the far end.
                           (may be NULL).

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
void zl303xx_PtpTsRecordRegisterPdelayFollowup(
      zl303xx_ClockStreamS *stream,
      Uint16T            seqId,
      Uint64S            *correction,
      zl303xx_TimeStamp    *pktTsField)
{
   /* This PEER_DELAY_FOLLOWUP was triggered due to an original PEER_DELAY_REQ. */
   zl303xx_PtpTsRecordTblEntryS *record =
         &(stream->tsRecordArray[ZL303XX_MSG_ID_PEER_DELAY_REQ][seqId & stream->tsRecordIdxMask]);

   /* Warning cleanup. */
   if (pktTsField) {;}

   /* Check if the record corresponding to this sequenceId has been reserved */
   if (!(record->flags & PTP_TS_RECORD_RESERVED) ||  /* record empty */
        (record->flags & PTP_TS_RECORD_HAVE_RES) ||  /* record Resident Time already filled */
        (record->seqId != seqId))                    /* record reserved with wrong seqId */
   {
      /* If the record has not been reserved, then this is an error because a
       * PDelay_Resp cannot be received until a PDelay_Req has been sent. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 1,
            "Stream %u: TS record [RX] seqId=%u error (no reserved entry):(0x%02X)",
            stream->clockStreamHandle, seqId, record->flags, 0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %u: TS record [RX] seqId=%u error (no reserved entry):(0x%02X)",
            stream->clockStreamHandle, seqId, record->flags, 0,0,0);

      /* Discard this timestamp */
      return;
   }
   else if (!(record->flags & PTP_TS_RECORD_HAVE_RX) /* FollowUp received prior to Resp */)
   {
      /* If the record has not been reserved, then this is an error because a
       * PDelay_Resp cannot be received until a PDelay_Req has been sent. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 1,
            "Stream %u: TS record [RX] seqId=%u error (followup rx prior to pdResp):(0x%02X)",
            stream->clockStreamHandle, seqId, record->flags, 0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %u: TS record [RX] seqId=%u error (followup rx prior to pdResp):(0x%02X)",
            stream->clockStreamHandle, seqId, record->flags, 0,0,0);

      /* Discard this timestamp */
      return;
   }

   /* Add the correctionField to the record.  */
   record->rxCorrection = *correction;

   /* 2-step : we will calculate resident time or receive it in the corrField. */
   /* If T3 is == 0 or NULL, then resident time method was used. */
   /* This means that the correctionField is already complete. */
   /* Otherwise, compute (T3-T2) and add it to the correction field. */

   /* If there was no T3, then all the data is in the correctionField. */
   record->flags |= PTP_TS_RECORD_HAVE_RES;

   ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 2,
         "Stream %u: TS record [RES] seqId=%u added (%u:%u): flags (0x%02X)(0x%02X)",
         stream->clockStreamHandle, seqId,
         record->rxCorrection.hi, record->rxCorrection.lo,
         record->flags, record->readyMask);
   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
         "Stream %u: TS record [RES] seqId=%u added (%u:%u): flags (0x%02X)(0x%02X)",
         stream->clockStreamHandle, seqId,
         record->rxCorrection.hi, record->rxCorrection.lo,
         record->flags, record->readyMask);

   /* If the record already had the TX data, then it is now complete. */
   if (record->flags == record->readyMask)
   {
      ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_TS_RECORD_MGR, 3)
      {
         /* Get rid of the fractional nanoseconds for display purposes. */
         Uint64S corrNS = RShift_U64S(record->rxCorrection, 16);

         ZL303XX_TRACE_ALWAYS(
               "Stream %3u: TS record Complete:  seqId=%5u",
               stream->clockStreamHandle, seqId, 0,0,0,0);
         ZL303XX_TRACE_ALWAYS(
               "             TX : 0x%08X:%08X ",
               record->txTs.secondsField.lo, record->txTs.nanosecondsField, 0,0,0,0);
         ZL303XX_TRACE_ALWAYS(
               "             RX : 0x%08X:%08X ",
               record->rxTs.secondsField.lo, record->rxTs.nanosecondsField, 0,0,0,0);
         ZL303XX_TRACE_ALWAYS(
               "       Corr(NS) : 0x%08X:%08X  (%10u:%10u)",
               corrNS.hi, corrNS.lo, corrNS.hi, corrNS.lo, 0,0);
      }

      /* Send it to the event handler to be processed. */
      zl303xx_PtpTsRecordReady(stream, record);

      /* Mark this time stamp record as empty. */
      record->flags = PTP_TS_RECORD_EMPTY;
   }
}

/* zl303xx_PtpStreamTwoStepRegisterTx */
/** 

   Registers the preciseOriginTimestamp (txTs) of a Follow_Up message. If the
   corresponding Sync <syncEventIngressTimestamp> (rxTs) already exists, the
   time stamp pair is processed. Otherwise, the txTs is saved in the stream's
   two-step table to be matched later.

  Parameters:
   [in]   stream      Pointer to the stream the Follow_Up was received on.
   [in]   seqId       sequenceId from the Follow_Up
   [in]   txTs        preciseOriginTimestamp from the Follow_Up
   [in]   correction  correctionField from the Follow_Up
   [in]   srcPortId   sourcePortIdentity from the Follow_Up

*******************************************************************************/
void zl303xx_PtpStreamTwoStepRegisterTx(zl303xx_ClockStreamS *stream, Uint16T seqId,
                                        zl303xx_TimeStamp *txTs, Uint64S *correction,
                                        zl303xx_PortIdentity *srcPortId)
{
   Uint64S totalCorrection;
   zl303xx_PtpTwoStepRecordS *record =
         &stream->twoStepRecordArray[seqId & stream->twoStepRecordIdxMask];

   if (record->flags & PTP_TS_RECORD_HAVE_RX)
   {
      if ((record->seqId == seqId) &&
          (zl303xx_PtpV2EqualPortIdentities(&record->u.v2.srcPortId, srcPortId)))
      {
         /* Telecom Profile: Any matched TS record resets the PTSF-lossSyncOrFollowUp timer. */
         zl303xx_PtpTelecomSofTimerReset(stream);

         totalCorrection = Add_U64S(record->correction, *correction, NULL);
         zl303xx_PtpUpdateOffset(stream, txTs, &record->rxTs, &totalCorrection, seqId, srcPortId);
         record->flags = PTP_TS_RECORD_EMPTY;
         return;
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
   record->correction = *correction;
   record->u.v2.srcPortId = *srcPortId;
   record->flags = PTP_TS_RECORD_HAVE_TX;
}

/* zl303xx_PtpStreamTwoStepRegisterRx */
/** 

   Registers the <syncEventIngressTimestamp> (rxTs) of a two-step Sync message.
   If the corresponding Follow_Up's preciseOriginTimestamp (txTs) already
   exists, the time stamp pair is processed. Otherwise, the rxTs is saved in the
   stream's two-step table to be matched later.

  Parameters:
   [in]   stream      Pointer to the stream the Follow_Up was received on.
   [in]   seqId       sequenceId from the Follow_Up
   [in]   rxTs        preciseOriginTimestamp from the Follow_Up
   [in]   correction  correctionField from the Follow_Up
   [in]   srcPortId   sourcePortIdentity from the Follow_Up

*******************************************************************************/
void zl303xx_PtpStreamTwoStepRegisterRx(zl303xx_ClockStreamS *stream, Uint16T seqId,
                                        zl303xx_TimeStamp *rxTs, Uint64S *correction,
                                        zl303xx_PortIdentity *srcPortId)
{
   Uint64S totalCorrection;
   zl303xx_PtpTwoStepRecordS *record =
         &stream->twoStepRecordArray[seqId & stream->twoStepRecordIdxMask];

   if (record->flags & PTP_TS_RECORD_HAVE_TX)
   {
      if ((record->seqId == seqId) &&
          (zl303xx_PtpV2EqualPortIdentities(&record->u.v2.srcPortId, srcPortId)))
      {
         /* Telecom Profile: Any matched TS record resets the PTSF-lossSyncOrFollowUp timer. */
         zl303xx_PtpTelecomSofTimerReset(stream);

         totalCorrection = Add_U64S(record->correction, *correction, NULL);
         zl303xx_PtpUpdateOffset(stream, &record->txTs, rxTs, &totalCorrection, seqId, srcPortId);
         record->flags = PTP_TS_RECORD_EMPTY;
         return;
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
   record->correction = *correction;
   record->u.v2.srcPortId = *srcPortId;
   record->flags = PTP_TS_RECORD_HAVE_RX;
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/


/* zl303xx_PtpTsRecordReady */
/** 

   Passes time stamp record data to the proper handler routine.

  Parameters:
   [in]   stream  Pointer to a stream.
   [in]   record  Pointer to time stamp record data.

*******************************************************************************/
static void zl303xx_PtpTsRecordReady(
      zl303xx_ClockStreamS *stream,
      zl303xx_PtpTsRecordTblEntryS *record)
{
   ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 2,
         "Stream %u: TS record [--] seqId=%u complete",
         stream->clockStreamHandle, record->seqId, 0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
         "Stream %u: TS record [--] seqId=%u complete",
         stream->clockStreamHandle, record->seqId, 0,0,0,0);

   zl303xx_PtpV2HandleTsRecordReady(stream, record);
}

/*****************   END   ****************************************************/
