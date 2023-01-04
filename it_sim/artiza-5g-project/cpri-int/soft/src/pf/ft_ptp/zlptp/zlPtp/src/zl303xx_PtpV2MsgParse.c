

/*****************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Definitions and utility functions for parsing (packing/unpacking) PTP
*     messages to/from buffers. These include PTP headers, messageType data
*     blocks (i.e. SYNC data, SIGNALING data, etc.) as well as individual data
*     field parsing.
*
*******************************************************************************/

/****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"

#include "zl303xx_PtpV2MsgParse.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_Ptp_dep.h"    /* for the 'setflag' functions and others */
#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpProfileDef.h"

/****************   DEFINES     **********************************************/

/****************   DATA TYPES   *********************************************/

/****************   DATA STRUCTURES   ****************************************/

/****************   STATIC FUNCTION DECLARATIONS   ***************************/

/* Computes the 16-bit flag field in a PTP message header based on stream
 * configuration and messageType. */
static Uint16T zl303xx_PtpV2MsgHeaderFlagsCompute(zl303xx_ClockStreamS *pPtpStream,
                                                  zl303xx_MessageTypeE messageType,
                                                  Uint16T *msgHdrFlags);
static void zl303xx_PtpV2MsgHeaderOverridesGet(zl303xx_ClockStreamS *pPtpStream,
                                               zl303xx_PtpV2MsgHeaderS *pPtpMsgHeader);

/****************   STATIC GLOBAL VARIABLES   ********************************/

/****************   EXPORTED GLOBAL VARIABLES   ******************************/

/****************   PTP MESSAGE PARSING FUNCTION DEFINITIONS   ***************/

/* zl303xx_PtpV2MsgHeaderUnpack */
/** 

   Unpacks a PTP message header (generally a received packet) into an associated
   data structure (refer to IEEE 1588-2008: Table 18).

  Parameters:
   [in]   pInHdrBuf      Pointer to the beginning of a buffer containing the
                              PTP V2 message header (in IEEE 1588 TLV format).
   [out]  pPtpMsgHeader  PTP V2 message structure to populate with the data
                              from the supplied header buffer.

  Return Value:  Uint16T  Size (in bytes) of the packet TLV header that was unpacked
                  0 if either of the input pointers is NULL

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgHeaderUnpack(
      Uint8T *pInHdrBuf,
      zl303xx_PtpV2MsgHeaderS *pPtpMsgHeader)
{
   Uint16T bytesProcessed = 0;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4,
         "zl303xx_PtpV2MsgHeaderUnpack:", 0,0,0,0,0,0);

   /* Make sure we have valid buffer and structure pointers. */
   if ((pInHdrBuf != NULL) && (pPtpMsgHeader != NULL))
   {
      /* transportSpecific */
      pPtpMsgHeader->transportSpecific = UNPACK_HI_NIBBLE(pInHdrBuf + ZL303XX_PTP_V2_HDR_TRANS_OFST);

      /* messageType */
      pPtpMsgHeader->messageType = UNPACK_LO_NIBBLE(pInHdrBuf + ZL303XX_PTP_V2_HDR_MSG_TYPE_OFST);

      /* versionPTP */
      pPtpMsgHeader->versionPTP = UNPACK_LO_NIBBLE(pInHdrBuf + ZL303XX_PTP_V2_HDR_VERSION_OFST);

      /* messageLength */
      pPtpMsgHeader->messageLength = zl303xx_PtpV2MsgPktLengthGet(pInHdrBuf);

      /* domainNumber */
      pPtpMsgHeader->domainNumber = UNPACK_8BIT(pInHdrBuf + ZL303XX_PTP_V2_HDR_DOMAIN_OFST);

      /* flagField */
      pPtpMsgHeader->flagField = UNPACK_16BIT(pInHdrBuf + ZL303XX_PTP_V2_HDR_FLAGS_OFST);

      /* correctionField */
      UNPACK_64BIT((pInHdrBuf + ZL303XX_PTP_V2_HDR_CORR_OFST), pPtpMsgHeader->correctionField);

      /* sourcePortIdentity */
      (void)zl303xx_PtpV2MsgPortIdentityUnpack(
                           (pInHdrBuf + ZL303XX_PTP_V2_HDR_SRC_PORT_OFST),
                           &pPtpMsgHeader->sourcePortIdentity);

      /* sequenceId */
      pPtpMsgHeader->sequenceId = zl303xx_PtpV2MsgSequenceNumGet(pInHdrBuf);

      /* controlField */
      pPtpMsgHeader->controlField = UNPACK_8BIT(pInHdrBuf + ZL303XX_PTP_V2_HDR_CTRL_OFST);

      /* logMessageInterval */
      pPtpMsgHeader->logMessageInterval = (Sint8T)UNPACK_8BIT(pInHdrBuf + ZL303XX_PTP_V2_HDR_INTERVAL_OFST);

      /* Assign the total bytes processed in case the calling routine wants
       * to adjust its buffer pointer */
      bytesProcessed = ZL303XX_PTP_V2_HEADER_LEN;

      /* If logging is enabled, display some (or all) of the above values. */
      (void)zl303xx_PtpV2MsgHeaderTrace(pPtpMsgHeader, ZL303XX_MOD_ID_PTP_ENGINE, 4);
   }

   return bytesProcessed;
}

/* zl303xx_PtpV2MsgHeaderPack */
/** 

   Packs a PTP message header (generally for transmission) into an associated
   network buffer (refer to IEEE 1588-2008: Table 18).

  Parameters:
   [in]   pPtpMsgHeader  The header structure containing the PTP V2 data to
                              pack into the message buffer.
   [out]  pOutHdrBuf     Pointer to the beginning of a buffer in which to
                              assemble the PTP packet header.

  Return Value:  Uint16T  Size (in bytes) of the packet header if successful.
                  0 if either of the input pointers is NULL or an error occurs.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgHeaderPack(
      zl303xx_PtpV2MsgHeaderS *pPtpMsgHeader,
      Uint8T *pOutHdrBuf)
{
   Uint16T bytesProcessed = 0;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4,
         "zl303xx_PtpV2MsgHeaderPack:", 0,0,0,0,0,0);

   /* Make sure we have valid buffer and structure pointers. */
   if ((pOutHdrBuf != NULL) && (pPtpMsgHeader != NULL))
   {
      /* Clear the header portion of the buffer */
      OS_MEMSET(pOutHdrBuf, 0, ZL303XX_PTP_V2_HEADER_LEN);

      /* transportSpecific */
      PACK_HI_NIBBLE(pOutHdrBuf + ZL303XX_PTP_V2_HDR_TRANS_OFST, pPtpMsgHeader->transportSpecific);

      /* messageType */
      PACK_LO_NIBBLE(pOutHdrBuf + ZL303XX_PTP_V2_HDR_MSG_TYPE_OFST, pPtpMsgHeader->messageType);

      /* versionPTP */
      PACK_LO_NIBBLE(pOutHdrBuf + ZL303XX_PTP_V2_HDR_VERSION_OFST, pPtpMsgHeader->versionPTP);

      /* messageLength */
      zl303xx_PtpV2MsgPktLengthSet(pPtpMsgHeader->messageLength, pOutHdrBuf);

      /* domainNumber */
      PACK_8BIT(pOutHdrBuf + ZL303XX_PTP_V2_HDR_DOMAIN_OFST, pPtpMsgHeader->domainNumber);

      /* flagField */
      PACK_16BIT(pOutHdrBuf + ZL303XX_PTP_V2_HDR_FLAGS_OFST, pPtpMsgHeader->flagField);

      /* correctionField */
      PACK_64BIT(pOutHdrBuf + ZL303XX_PTP_V2_HDR_CORR_OFST, pPtpMsgHeader->correctionField);

      /* sourcePortIdentity */
      (void)zl303xx_PtpV2MsgPortIdentityPack(
                           &pPtpMsgHeader->sourcePortIdentity,
                           pOutHdrBuf + ZL303XX_PTP_V2_HDR_SRC_PORT_OFST);

      /* sequenceId */
      zl303xx_PtpV2MsgSequenceNumSet(pPtpMsgHeader->sequenceId, pOutHdrBuf);

      /* controlField */
      PACK_8BIT(pOutHdrBuf + ZL303XX_PTP_V2_HDR_CTRL_OFST, pPtpMsgHeader->controlField);

      /* logMessageInterval */
      PACK_8BIT(pOutHdrBuf + ZL303XX_PTP_V2_HDR_INTERVAL_OFST, pPtpMsgHeader->logMessageInterval);

      /* Assign the total bytes processed in case the calling routine wants
       * to adjust its buffer pointer */
      bytesProcessed = ZL303XX_PTP_V2_HEADER_LEN;

      /* If logging is enabled, display some (or all) of the above values. */
      (void)zl303xx_PtpV2MsgHeaderTrace(pPtpMsgHeader, ZL303XX_MOD_ID_PTP_ENGINE, 4);
   }

   return bytesProcessed;
}

/* zl303xx_PtpV2MsgHeaderPackForStream */
/** 

   Utility function to pack a PTP message HEADER (only) for a particular stream
   based on the messageType specified (refer to IEEE 1588-2008: Table 18).

  Parameters:
   [in]   pPtpStream     Pointer to the PTP stream for which to build the
                              message buffer.
   [in]   messageType    Pointer to the messageType of the packet.
   [in]   sequenceId     Pointer to the sequenceId value to insert into the
                              packet header if specified. If set to NULL, this
                              is automatically generated from the stream's pool
                              of sequence numbers based on the messageType.
   [out]  pOutHdrBuf     Pointer to the packed output buffer.

  Return Value:  Uint16T  Length (in bytes) of the packed buffer on success.
                  0 Otherwise.

  Notes:
   As this is an internal routine, all input parameters are assumed to be valid.
   The pOutHdrBuf is assumed to be large enough for the constructed message.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgHeaderPackForStream(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_MessageTypeE messageType,
      Uint16T *sequenceId,
      Uint8T *pOutHdrBuf)
{
   zlStatusE status = ZL303XX_OK;

   const char *fnName = "zl303xx_PtpV2MsgHeaderPackForStream";

   /* A Blank header structure. */
   zl303xx_PtpV2MsgHeaderS ptpMsgHeader;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 6,
         "%s: Stream %d: Message %d",
         fnName, pPtpStream->clockStreamHandle, messageType, 0,0,0);

   /* Check for valid STREAM pointer. */
   if ((status = ZL303XX_CHECK_POINTER(pPtpStream)) != ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS("%s: Invalid Stream Pointer!",
            fnName, 0,0,0,0,0);
   }
   /* ELSE: the pOutHdrBuf pointer is checked later. */

   /* Check for valid messageTypes. */
   else if ((status = ZL303XX_CHECK_MESSAGE_TYPE(messageType)) != ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS("%s: Invalid messageType (%d)!",
            fnName, messageType, 0,0,0,0);
   }

   /* Get the pointer to the clock associated with this stream. */
   else if ((pPtpStream->portData == NULL) || (pPtpStream->portData->clock == NULL))
   {
      ZL303XX_TRACE_ALWAYS("%s: No parent CLOCK (%p) or PORT (%p)!",
               fnName,
               pPtpStream->portData,
               ((pPtpStream->portData != NULL) ? pPtpStream->portData->clock : NULL), 0,0,0);

      status = ZL303XX_INIT_ERROR;
   }

   /* Violates normal coding rules but avoids indentations below. */
   if (status != ZL303XX_OK)
   {
      return 0;
   }

   /* Clear the local header structure. */
   OS_MEMSET(&ptpMsgHeader, 0x00, sizeof(ptpMsgHeader));

   /* Assign the message fields in the common header */
   /**************************************************/

   /* messageType */
   ptpMsgHeader.messageType = messageType;
   /* transportSpecific */
   ptpMsgHeader.transportSpecific = (Uint4T)0;
   /* versionPTP */
   ptpMsgHeader.versionPTP = ZL303XX_PTP_VERSION_2;

   /* messageLength */
   /* For now, set it to the length of the PTP header since this is all that
    * this routine packs. As more data is concatenated to the end, the
    * messageLength field should be updated by the calling routine.   */
   ptpMsgHeader.messageLength = ZL303XX_PTP_V2_HEADER_LEN;

   /* domainNumber */
   ptpMsgHeader.domainNumber = pPtpStream->portData->clock->config.defaultDS.domainNumber;

   /* flagField */
   /* FLAGS are updated based on messageType. */
   (void)zl303xx_PtpV2MsgHeaderFlagsCompute(pPtpStream, messageType, &ptpMsgHeader.flagField);

   /* correctionField */
   /* This has to be set for delay asymmetry corrections (Clause 11.6). For
    * SYNC and PEER_DELAY_RESP it done at the receive handler: For DELAY_REQUEST
    * and PEER_DELAY_REQ it done on the transmit side.  */
   if ((messageType == ZL303XX_MSG_ID_DELAY_REQ) ||
       (messageType == ZL303XX_MSG_ID_PEER_DELAY_REQ))
   {
      /* Per Clause 11.6.3.b or 11.6.4.b, subtract the egress delay asymmetry
       * correction value from the correctionField. */
      ptpMsgHeader.correctionField =
         zl303xx_PtpTimeIntervalNegate(pPtpStream->config.delayAsymmetry.egress).scaledNanoseconds;
   }

   /* sourcePortIdentity */
   /* We advertise our own portIdentity */
   ptpMsgHeader.sourcePortIdentity = pPtpStream->portData->portDS.portIdentity;

   /* sequenceId */
   /* - If this is a response to a received message, then use the sequenceId
    *   from the received message.
    * - Otherwise (we are initiating the message) generate the sequenceId from
    *   the local stream's sequenceId counter for the messageType specified.  */
   if (sequenceId != NULL)
   {
      ptpMsgHeader.sequenceId = *sequenceId;
   }
   else
   {
      ptpMsgHeader.sequenceId = pPtpStream->config.nextTxSequenceId[messageType];
   }

   /* control */
   /* logMessageInterval */
   /* Assign the controlField & logMessageInterval based on messageType. */
   switch (messageType)
   {
      case ZL303XX_MSG_ID_SYNC :
      {
         ptpMsgHeader.controlField = (Uint8T)PTP_SYNC_MESSAGE;

         /* Only send the Sync rate for Multicast. */
         if (pPtpStream->config.unicast == ZL303XX_FALSE)
            ptpMsgHeader.logMessageInterval = pPtpStream->config.logSyncInterval;
         else
            ptpMsgHeader.logMessageInterval = PTP_V2_LMMI_NULL_VALUE;

         break;
      }
      case ZL303XX_MSG_ID_DELAY_REQ :
      {
         ptpMsgHeader.controlField = (Uint8T)PTP_DELAY_REQ_MESSAGE;
         ptpMsgHeader.logMessageInterval = PTP_V2_LMMI_NULL_VALUE;
         break;
      }

      case ZL303XX_MSG_ID_FOLLOWUP :
      {
         ptpMsgHeader.controlField = (Uint8T)PTP_FOLLOWUP_MESSAGE;

         /* Only send the Sync rate for Multicast. */
         if (pPtpStream->config.unicast == ZL303XX_FALSE)
            ptpMsgHeader.logMessageInterval = pPtpStream->config.logSyncInterval;
         else
            ptpMsgHeader.logMessageInterval = PTP_V2_LMMI_NULL_VALUE;

         break;
      }
      case ZL303XX_MSG_ID_DELAY_RESP :
      {
         ptpMsgHeader.controlField = (Uint8T)PTP_DELAY_RESP_MESSAGE;

         /* Only send the Delay Request rate for Multicast. */
         if (pPtpStream->config.unicast == ZL303XX_FALSE)
            ptpMsgHeader.logMessageInterval = pPtpStream->config.logMinDelayReqInterval;
         else
            ptpMsgHeader.logMessageInterval = PTP_V2_LMMI_NULL_VALUE;

         break;
      }

      case ZL303XX_MSG_ID_ANNOUNCE :
      {
         ptpMsgHeader.controlField = (Uint8T)PTP_OTHER_MESSAGE;
         ptpMsgHeader.logMessageInterval = pPtpStream->config.logAnnounceInterval;
         break;
      }

      case ZL303XX_MSG_ID_MANAGEMENT :
      {
         ptpMsgHeader.controlField = (Uint8T)PTP_MANAGEMENT_MESSAGE;
         ptpMsgHeader.logMessageInterval = PTP_V2_LMMI_NULL_VALUE;
         break;
      }

      case ZL303XX_MSG_ID_SIGNALING :
      case ZL303XX_MSG_ID_PEER_DELAY_REQ :
      case ZL303XX_MSG_ID_PEER_DELAY_RESP :
      case ZL303XX_MSG_ID_PEER_DELAY_FOLLOWUP :
      default :
      {
         ptpMsgHeader.controlField = (Uint8T)PTP_OTHER_MESSAGE;
         ptpMsgHeader.logMessageInterval = PTP_V2_LMMI_NULL_VALUE;
         break;
      }
   }

   /* Insert any Profile specific header values.    */
   /* The pPtpStream will have the profileId & Clock/Port info.
    * The ptpMsgHeader has the messageType.  */
   status = zl303xx_PtpProfileEgressPktHeaderGet(pPtpStream, &ptpMsgHeader);

   /* Get any Over-ride values configured on this Stream/Clock */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpV2MsgHeaderOverridesGet(pPtpStream, &ptpMsgHeader);

      /* Now that all of the header fields of the structure are populated,
       * pack the data into the supplied buffer. */
      return zl303xx_PtpV2MsgHeaderPack(&ptpMsgHeader, pOutHdrBuf);
   }

   /* If status was not OK, return 0. */
   return 0;
}

/* zl303xx_PtpV2MsgDataSignalUnpack */
/** 

   Unpacks the signaling data portion of a received PTP packet into a
   compatible data structure.

  Parameters:
   [in]  pSignalDataBuf    Pointer to the start of the signaling data portion
                                 of the received PTP packet buffer (the first
                                 byte after the PTP packet header).
   [out] pMsgData          Unpacked Signaling data section (pMsgData->signaling).

  Return Value:
     Uint16T  Size in bytes of the Signaling data buffer that is unpacked.
     0        An error occurred.

  Notes:
   This only unpacks the common part of the signaling message (the
   targetPortIdentity). Any attached TLVs must be decoded separately.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgDataSignalUnpack(
      Uint8T *pSignalDataBuf,
      zl303xx_PtpV2MsgDataU *pMsgData)
{
   /* The input buffer starts at the Signal Data. */
   return zl303xx_PtpV2MsgPortIdentityUnpack(
             pSignalDataBuf, &pMsgData->signaling.targetPortIdentity);
}

/* zl303xx_PtpV2MsgDataSignalPack */
/** 

   Packs the signaling data portion of a buffer from a compatible data structure.

  Parameters:
   [in]   pSignalData     Pointer to a Signaling structure from which to unpack
                               the Signaling data portion of the buffer.
   [out]  pSignalDataBuf  Pointer to the start of the signaling data portion
                               of the packet buffer to fill (the first byte
                               after the PTP packet header).

  Return Value:  Uint16T  The number of bytes processed.

  Notes:
   This only packs the common part of the signaling message (the
   targetPortIdentity). Any TLVs to be attached must be added separately.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgDataSignalPack(
      zl303xx_PtpV2MsgSignalingS *pSignalData,
      Uint8T *pSignalDataBuf)
{
   /* Pack the signaling data specific items. */

   /* targetPortIdentity */
   return  zl303xx_PtpV2MsgPortIdentityPack(&pSignalData->targetPortIdentity, pSignalDataBuf);
}

/* zl303xx_PtpV2MsgDataMgmtUnpack */
/** 

   Unpacks the data portion of a received PTP packet into a compatible Management
   data structure.

  Parameters:
   [in]  pMgmtDataBuf   Pointer to the start of the Management data portion
                              of the received PTP packet buffer (the first byte
                              after the PTP packet header).
   [out] pMsgData       Unpacked Management data section (pMsgData->management).

  Return Value:
     Uint16T  Size in bytes of the Management data buffer that is unpacked.
     0        An error occurred.

  Notes:  Assumes all input pointers are valid. This only unpacks the common part
       of the management message. Any attached TLVs must be decoded separately.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgDataMgmtUnpack(
      Uint8T *pMgmtDataBuf,
      zl303xx_PtpV2MsgDataU *pMsgData)
{
   zl303xx_PtpV2MsgManagementS *pMgmtData = NULL;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5,
         "zl303xx_PtpV2MsgDataMgmtUnpack:", 0,0,0,0,0,0);

   /* Assign a local reference. */
   pMgmtData = &pMsgData->management;

   /* targetPortIdentity */
   zl303xx_PtpV2MsgPortIdentityUnpack(pMgmtDataBuf, &pMgmtData->targetPortIdentity);
   ZL303XX_TRACE_V2_PORT_ID(ZL303XX_MOD_ID_PTP_ENGINE, 8,
         "   targetPortIdentity ",
         &pMgmtData->targetPortIdentity);

   /* Hops */
   pMgmtData->startingBoundaryHops = *(Uint8T *)(pMgmtDataBuf + 10);
   pMgmtData->boundaryHops = *(Uint8T *)(pMgmtDataBuf + 11);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5,
         "   startingBoundaryHops %u, boundaryHops %u",
         pMgmtData->startingBoundaryHops, pMgmtData->boundaryHops, 0,0,0,0);

   /* Action */
   pMgmtData->actionField = *(pMgmtDataBuf + 12) & 0x0F;
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
         "   action %u", pMgmtData->actionField, 0,0,0,0,0);

   /* reserved at (pMgmtDataBuf + 13) */

   return ZL303XX_PTP_V2_MANAGEMENT_DATA_LEN;
}

/* zl303xx_PtpV2MsgDataMgmtPack */
/** 

   Unpacks the data portion of a received PTP packet into a compatible Management
   data structure.

  Parameters:
   [in]  pMgmtDataBuf   Pointer to the start of the Management data portion
                              of the received PTP packet buffer (the first byte
                              after the PTP packet header).
   [out] pMsgData       Unpacked Management data section (pMsgData->management).

  Return Value:
     Uint16T  Size in bytes of the Management data buffer that is unpacked.
     0        An error occurred.

  Notes:  Assumes all input pointers are valid. This only unpacks the common part
       of the management message. Any attached TLVs must be decoded separately.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgDataMgmtPack(
          zl303xx_PtpV2MsgManagementS *pMgmtData,
          Uint8T *pMgmtDataBuf)
{
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5,
         "zl303xx_PtpV2MsgDataMgmtPack:", 0,0,0,0,0,0);

   /* targetPortIdentity */
   zl303xx_PtpV2MsgPortIdentityPack(&pMgmtData->targetPortIdentity, pMgmtDataBuf);

   /* Hops */
   *(Uint8T *)(pMgmtDataBuf + 10) = pMgmtData->startingBoundaryHops;
   *(Uint8T *)(pMgmtDataBuf + 11) = pMgmtData->boundaryHops;

   /* Action */
   *(Uint8T *)(pMgmtDataBuf + 12) = (pMgmtData->actionField & 0x0F);

   /* reserved at (pMgmtDataBuf + 13) */
   *(Uint8T *)(pMgmtDataBuf + 13) = 0;

   return ZL303XX_PTP_V2_MANAGEMENT_DATA_LEN;
}

/* zl303xx_PtpV2MsgMgmtPack */
/** 

   Packs a PTP Management Message (generally a transmit packet) from the PTP
   Header & Management data structures provided.

  Parameters:
   [in]  pPtpMsgHeader     PTP V2 message header data to use for building
                                 the packet. (If = NULL this portion is ignored).
   [in]  pInMgmtData       PTP Management Data to use for building the packet.
                                 (If = NULL this portion is ignored). This does
                                 not handle TLVs that may need to be appended.

   [out] pOutMgmtMsgBuf    Pointer to a PTP message buffer (always the start
                                 of the PTP message header regardless of whether
                                 either data structure is NULL).

  Return Value:
     Uint16T  Number of bytes packed (processed) into the buffer.
     0        An error occurred.

  Notes:
   Assumes all input pointers are valid.
   Packs only Header & Management Data, not any appended TLVs.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgMgmtPack(
      zl303xx_PtpV2MsgHeaderS *pPtpMsgHeader,
      zl303xx_PtpV2MsgManagementS *pInMgmtData,
      Uint8T *pOutMgmtMsgBuf)
{
   Uint16T bytesProcessed = 0;

   /* Should be no failure paths. */

   /* PTP Header */
   if (pPtpMsgHeader != NULL)
   {
      bytesProcessed = zl303xx_PtpV2MsgHeaderPack(pPtpMsgHeader, pOutMgmtMsgBuf);
   }

   /* Management Data */
   if (pInMgmtData != NULL)
   {
      /* The Management data begins after the header. */
      bytesProcessed += zl303xx_PtpV2MsgDataMgmtPack(
                              pInMgmtData,
                              (pOutMgmtMsgBuf + ZL303XX_PTP_V2_HEADER_LEN));
   }

   /* Add the packet length (if the header is requested). */
   if (pPtpMsgHeader != NULL)
   {
      zl303xx_PtpV2MsgPktLengthSet(bytesProcessed, pOutMgmtMsgBuf);
   }

   return bytesProcessed;
}  /* END zl303xx_PtpV2MsgMgmtPack */

/****************   GENERIC PARSING FUNCTION DEFINITIONS FOR MESSAGES WITH SAME FORMAT  ******/

/* zl303xx_PtpV2MsgDataTimestampUnpack */
/** 

   Unpacks the 'originTimestamp' from the Data portion of a compatible PTP
   message (SYNC, DELAY_REQ or FOLLOW_UP) into an associated data structure
   (refer to IEEE 1588-2008: Table 26 & 27).

  Parameters:
   [in]  pInTsDataBuf   Pointer to the Data portion of a PTP Timestamp
                              message (1st byte after the PTP header).

   [out] pOutTsDataS    Unpacked Timestamp data section.

  Return Value:
     Uint16T  Number of bytes unpacked (processed) from the buffer.
     0        An error occurred.

  Notes:  Assumes all input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgDataTimestampUnpack(
      Uint8T               *pInTsDataBuf,
      zl303xx_PtpV2MsgDataU  *pOutTsDataS)
{
   /* Since this message data contains only a Timestamp member, pass the
    * structure to the member type routine. */

   /* originTimestamp */
   /* Since 'originTimestamp' is the only (& 1st) member, just cast the data structure. */
   return zl303xx_PtpV2MsgTimestampUnpack(pInTsDataBuf, (zl303xx_TimeStamp *)pOutTsDataS);
}

/* zl303xx_PtpV2MsgDataTsPortIdUnpack */
/** 

   Unpacks the 'Timestamp' and 'Port Identity' from the Data portion of
   certain PTP message into an associated data structure.

  Parameters:
   [in]  pInTsPortDataBuf  Pointer to the Data portion of a PTP message
                                 (1st byte after the PTP header).

   [out] pOutTsPortDataS   Unpacked data values (Timestamp and PortIdentity).

  Return Value:
     Uint16T  Number of bytes unpacked (processed) from the buffer.
     0        An error occurred.

  Notes:  Assumes all input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgDataTsPortIdUnpack(
      Uint8T               *pInTsPortDataBuf,
      zl303xx_PtpV2MsgDataU  *pOutTsPortDataS)
{
   /* Use the generic (overlay) form of the structure. */
   zl303xx_PtpV2MsgDataTsPortIdS *pTsPortData = NULL;


   /* Assign a local reference. */
   pTsPortData = &pOutTsPortDataS->tStampAndPortId;

   /* Unpack each of the fields in this message. */

   /* timestamp */
   zl303xx_PtpV2MsgTimestampUnpack(pInTsPortDataBuf + ZL303XX_PTP_OVERLAY_TIMESTAMP_OFST,
                                   &pTsPortData->timestamp);

   /* portIdentity */
   zl303xx_PtpV2MsgPortIdentityUnpack(pInTsPortDataBuf + ZL303XX_PTP_OVERLAY_PORT_ID_OFST,
                                      &pTsPortData->portIdentity);

   /* Return the full size. */
   return ZL303XX_PTP_OVERLAY_TS_PORTID_LEN;
}

/* zl303xx_PtpV2MsgTimePortIdPackForStream */
/** 

   Packs a PTP Timestamp & Port ID Message as an Initial packet or a Reply (or
   secondary action) to some earlier message (or action).

   Can be used for any of the following messages:
   - PDELAY_REQ    : Initial Action  : No RxHeader Required
   - PDELAY_RESP   : Secondary Action: No RxHeader Required
   - DELAY_RESP    : Secondary Action: RxHeader Required
   - PDELAY_RESP_FU: Secondary Action: No RxHeader Required

   These messages may require some data from a previous message (sequenceId,
   sourcePortIdentity, correctionField), an optional Timestamp and an optional
   target portIdentity (generally the sourcePortIdentity is from a previous
   action).

  Parameters:
   [in]  pPtpStream     Pointer to the PTP clock stream structure for the
                              stream from which this message will be issued.
   [in]  pRxMsgHdr      The header from the received DELAY_REQ or PDELAY_REQ
                              message associated with this packet being built.
                              (NULL for initial PDELAY_REQ message).
   [in]  pInTimestamp   Optional pointer to a provided Timestamp.
                              (If == NULL then a +/- 1 sec estimate is inserted).
   [in]  ptpMsgType     The PTP message type to generate the packet buffer for.

   [out] pOutTsPortIdMsgBuf   Pointer to a packed Timestamp / PortIdentity
                                    message buffer (that may not contain the
                                    PortIdentity).

  Return Value:
     Uint16T  Number of bytes packed (processed) into the buffer.
     0        An error occurred.

  Notes:  Assumes all input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgTimePortIdPackForStream(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgHeaderS *pRxMsgHdr,      /* May be NULL */
      zl303xx_TimeStamp *pInTimestamp,         /* May be NULL */
      zl303xx_MessageTypeE ptpMsgType,
      Uint8T *pOutTsPortIdMsgBuf)
{
   Uint16T bytesPacked = 0;
   Uint16T msgBytesMin = zl303xx_PtpV2MsgLengthOfType(ptpMsgType);

   Uint16T *sequenceId = NULL;

   /* Should be no failure paths. */

   /* Zero-fill the entire PTP message buffer. */
   OS_MEMSET(pOutTsPortIdMsgBuf, 0, msgBytesMin);

   /* Pack the Header portion of the buffer. */
   {
      /* Use the sequenceId from the received Request if provided. */
      if (pRxMsgHdr != NULL)
      {
         sequenceId = &pRxMsgHdr->sequenceId;
      }

      bytesPacked = zl303xx_PtpV2MsgHeaderPackForStream(
                              pPtpStream, ptpMsgType, sequenceId,
                              pOutTsPortIdMsgBuf);
   }

   /* If the header was created properly, add the Timestamp. */
   /* timestamp */
   /* Every one of the intended messageTypes for this routine has the Timestamp
    * field as the first entry. If NULL it will be left as 0, but always count
    * the bytes in the processed length.   */
   if (bytesPacked == ZL303XX_PTP_V2_HEADER_LEN)
   {
      /* Set a reference to the Timestamp data portion of the message. */
      Uint8T *pOutTimestampBuf = pOutTsPortIdMsgBuf + bytesPacked;

      /* In case we need to generate the Timestamp. */
      zl303xx_TimeStamp originTimestamp = {{0,0}, 0};

      /* Insert the Timestamp if provided (otherwise it is generated). */
      if (pInTimestamp != NULL)
      {
         originTimestamp = *pInTimestamp;
      }
      else
      {
         /* If no Origin-Timestamp is provided, call the hardware interface routine
          * to get an estimate of the local time (+/- 1 sec OR 0) for 2-step clocks.
          * Otherwise, the actual time will be inserted on egress. (The returned
          * value is already adjusted for latency; if any).  */
         if (ZL303XX_OK != zl303xx_PtpClockTimeGet(pPtpStream->portData->clock,
                                                  ZL303XX_PTP_INTERFACE_EVENT,
                                                  &originTimestamp))
         {
            OS_MEMSET(&originTimestamp, 0x00, sizeof(originTimestamp));
         }
      }

      /* Add the originTimestamp to the message */
      bytesPacked += zl303xx_PtpV2MsgTimestampPack(&originTimestamp, pOutTimestampBuf);
   }

   /* Pack the Port ID data and other associated fields. */
   if (bytesPacked == (ZL303XX_PTP_V2_HEADER_LEN + ZL303XX_PTP_MSG_TIMESTAMP_LEN))
   {
      /* Set a reference to the PortIdentity data portion of the message. */
      Uint8T *pOutPortIdBuf = pOutTsPortIdMsgBuf + bytesPacked;

      /* portIdentity */
      /* Set the Port Identity as follows:
       * - for SYNC, DELAY_REQ, and FOLLOW_UP: Ignore (leave bytesPacked)
       * - for PDELAY_REQ: : fill with 0's (add bytesPacked)
       * - for PDELAY_RESP, DELAY_RESP and PDELAY_RESP_FU: Use Rx Header.  */
      if (ptpMsgType == ZL303XX_MSG_ID_PEER_DELAY_REQ)
      {
         /* 0's were set above with OS_MEMSET() so just increment bytes processed. */
         bytesPacked += ZL303XX_PTP_MSG_PORT_IDENTITY_LEN;
      }
      else if ((ptpMsgType == ZL303XX_MSG_ID_PEER_DELAY_RESP) ||
               (ptpMsgType == ZL303XX_MSG_ID_DELAY_RESP) ||
               (ptpMsgType == ZL303XX_MSG_ID_PEER_DELAY_FOLLOWUP))
      {
         if (pRxMsgHdr != NULL)
         {
            bytesPacked += zl303xx_PtpV2MsgPortIdentityPack(
                                    &pRxMsgHdr->sourcePortIdentity,
                                    pOutPortIdBuf);
         }
      }

      /* The correction field is taken directly from the received delay request
       * (spec section 11.3.2) since we are not contributing to it. */
      /* correctionField */

      if (pRxMsgHdr != NULL)
      {
         (void)zl303xx_PtpV2MsgCorrectionFieldSet(pRxMsgHdr->correctionField, pOutTsPortIdMsgBuf);
      }

      /* Set the message length. */
      zl303xx_PtpV2MsgPktLengthSet(bytesPacked, pOutTsPortIdMsgBuf);
   }


   return bytesPacked;
}

/****************   PTP MESSAGE FRAGMENT PARSING FUNCTION DEFINITIONS   ******/

/* zl303xx_PtpV2MsgClockIdentityUnpack */
/** 

   Unpacks a PTP V2 Clock Identity data type from network format.

  Parameters:
   [in]   pClockIdBuf     Pointer to the input buffer containing the
                               unpacked data.
   [out]  pClockIdentity  Pointer to the clockIdentity data value.

  Return Value:  Uint16T  Number of bytes unpacked from the input buffer on success.
                  0 otherwise

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgClockIdentityUnpack(
      Uint8T *pClockIdBuf,
      zl303xx_ClockIdentity clockIdentity)
{
   OS_MEMCPY(clockIdentity, pClockIdBuf, PTP_V2_CLK_IDENTITY_LENGTH);

   return ZL303XX_PTP_MSG_CLOCK_IDENTITY_LEN;
}

/* zl303xx_PtpV2MsgClockIdentityPack */
/** 

   Packs a PTP V2 Clock Identity data type into network format.

  Parameters:
   [in]   pClockIdentity  Structure containing the clock identity to pack.
   [out]  pClockIdBuf     Pointer to the output buffer containing the
                               packed data.

  Return Value:  Uint16T  Number of bytes packed into the output buffer on success.
                  0 otherwise

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgClockIdentityPack(
      zl303xx_ClockIdentity clockIdentity,
      Uint8T *pClockIdBuf)
{
   OS_MEMCPY(pClockIdBuf, clockIdentity, PTP_V2_CLK_IDENTITY_LENGTH);

   return ZL303XX_PTP_MSG_CLOCK_IDENTITY_LEN;
}

/* zl303xx_PtpV2MsgClockQualityUnpack */
/** 

   Unpacks a Clock Quality data type from network format.

  Parameters:
   [in]  pClockQualityBuf  Pointer to the input buffer containing the unpacked
                                 data.
   [out] pClockQuality     Pointer to the unpacked data structure.

  Return Value:  Uint16T  Number of bytes unpacked from the input buffer on success.
                  0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgClockQualityUnpack(
      Uint8T *pClockQualityBuf,
      zl303xx_ClockQuality *pClockQuality)
{
   pClockQuality->clockClass              = UNPACK_8BIT (pClockQualityBuf);
   pClockQuality->clockAccuracy           = UNPACK_8BIT (pClockQualityBuf + 1);
   pClockQuality->offsetScaledLogVariance = UNPACK_16BIT(pClockQualityBuf + 2);

   return ZL303XX_PTP_MSG_CLOCK_QUALITY_LEN;
}

/* zl303xx_PtpV2MsgClockQualityPack */
/** 

   Packs a clock quality value from a structure into a PTP message

  Parameters:
   [in]  clockQuality      Structure to insert into the message
   [out] pClockQualityBuf  Pointer to a buffer in network order to contain
                                 the clock quality octets.

  Return Value:  None

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgClockQualityPack(
      zl303xx_ClockQuality *pClockQuality,
      Uint8T *pClockQualityBuf)
{
   PACK_8BIT (pClockQualityBuf,     pClockQuality->clockClass);
   PACK_8BIT (pClockQualityBuf + 1, pClockQuality->clockAccuracy);
   PACK_16BIT(pClockQualityBuf + 2, pClockQuality->offsetScaledLogVariance);

   return ZL303XX_PTP_MSG_CLOCK_QUALITY_LEN;
}

/* zl303xx_PtpV2MsgPortIdentityUnpack */
/** 

   Unpacks a PTP V2 Port Identity data type from network format.

  Parameters:
   [in]   pPortIdBuf     Pointer to the input buffer containing the
                              unpacked data.
   [out]  pPortIdentity  Pointer to the portIdentity data value.

  Return Value:  Uint16T  Number of bytes unpacked from the input buffer on success.
                  0 otherwise

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgPortIdentityUnpack(
      Uint8T *pPortIdBuf,
      zl303xx_PortIdentity *pPortIdentity)
{
   /* IEEE-1588 is not 100% explicit about the ordering of the clockIdentity and
    * portNumber bytes within the sourcePortIdentity. Here we assume the most
    * obvious ordering */

  /* Unpack the Clock Identity */
  zl303xx_PtpV2MsgClockIdentityUnpack(pPortIdBuf, pPortIdentity->clockIdentity);

  /* Unpack the Port Identity */
  pPortIdentity->portNumber = UNPACK_16BIT(pPortIdBuf + PTP_V2_CLK_IDENTITY_LENGTH);

  return ZL303XX_PTP_MSG_PORT_IDENTITY_LEN;
}

/* zl303xx_PtpV2MsgPortIdentityPack */
/** 

   Packs a PTP V2 Port Identity data type into network format.

  Parameters:
   [in]   pPortIdentity  Structure containing the port identity to pack.
   [out]  pPortIdBuf     Pointer to the output buffer containing the
                              packed data.

  Return Value:  Uint16T  Number of bytes packed into the output buffer on success.
                  0 otherwise

*******************************************************************************/
Uint16T  zl303xx_PtpV2MsgPortIdentityPack(
      zl303xx_PortIdentity *pPortIdentity,
      Uint8T *pPortIdBuf)
{

   /* Pack the Clock Identity */
   zl303xx_PtpV2MsgClockIdentityPack(pPortIdentity->clockIdentity, pPortIdBuf);

   /* Pack the Port Identity */
   PACK_16BIT((pPortIdBuf + PTP_V2_CLK_IDENTITY_LENGTH), pPortIdentity->portNumber);

   return ZL303XX_PTP_MSG_PORT_IDENTITY_LEN;
}

/*

  Function Name:
   zl303xx_PtpV2MsgTimestampUnpack

  Details:
   Unpacks a PTP V2 zl303xx_TimeStamp data type from network format.

  Parameters:
   [in]  pTimestampBuf  Pointer to the input buffer containing the unpacked data.

   [out] pOutTimestamp  Pointer to the zl303xx_TimeStamp data value.

  Return Value:
     Uint16T  Number of bytes unpacked from the input buffer on success.
     0        Otherwise

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgTimestampUnpack(
      Uint8T *pTimestampBuf,
      zl303xx_TimeStamp *pOutTimestamp)
{

   /* Seconds is a 48-bit field. */
   pOutTimestamp->secondsField.hi = UNPACK_16BIT((pTimestampBuf + 0));
   pOutTimestamp->secondsField.lo = UNPACK_32BIT((pTimestampBuf + 2));

   /* Nanoseconds is a 32-bit field */
   pOutTimestamp->nanosecondsField = UNPACK_32BIT((pTimestampBuf + 6));

   return ZL303XX_PTP_MSG_TIMESTAMP_LEN;
}  /* END zl303xx_PtpV2MsgTimestampUnpack */

/*

  Function Name:
   zl303xx_PtpV2MsgTimestampPack

  Details:
   Packs a PTP V2 zl303xx_TimeStamp data type into network format.

  Parameters:
   [in]  pInTimestamp   Structure containing the zl303xx_TimeStamp to pack.

   [out] pTimestampBuf  Pointer to the output buffer containing the packed data.

  Return Value:
     Uint16T  Number of bytes packed into the output buffer on success.
     0        Otherwise

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgTimestampPack(
      zl303xx_TimeStamp *pInTimestamp,
      Uint8T *pTimestampBuf)
{

   /* Seconds is a 48-bit field. */
   PACK_16BIT((pTimestampBuf + 0), (Uint16T)(pInTimestamp->secondsField.hi));
   PACK_32BIT((pTimestampBuf + 2), pInTimestamp->secondsField.lo);

   /* Nanoseconds is a 32-bit field */
   PACK_32BIT((pTimestampBuf + 6), pInTimestamp->nanosecondsField);

   return ZL303XX_PTP_MSG_TIMESTAMP_LEN;
}  /* END zl303xx_PtpV2MsgTimestampPack */

/* zl303xx_PtpV2MsgProtocolAddressUnpack */
/** 

   Unpacks a Protocol Address data type from network format.

  Parameters:
   [in]  pInProtoAddrBuf   Pointer to the input buffer containing the unpacked
                                 data.
   [out] pOutProtoAddrS    Pointer to the unpacked data structure.

  Return Value:  Uint16T  Number of bytes unpacked from the input buffer on success.
                  0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgProtocolAddressUnpack(
      Uint8T *pInProtoAddrBuf,
      zl303xx_PortAddress *pOutProtoAddrS)
{
   /* Set a default return value. */
   Uint16T copyStringLen = 0;

   pOutProtoAddrS->networkProtocol = UNPACK_16BIT(pInProtoAddrBuf + 0);
   pOutProtoAddrS->addressLength   = UNPACK_16BIT(pInProtoAddrBuf + 2);

   /* Set the length of the string to copy. */
   copyStringLen = ((pOutProtoAddrS->addressLength <= ZL303XX_PORT_ADDRESS_LENGTH_MAX)
                        ? pOutProtoAddrS->addressLength
                        : ZL303XX_PORT_ADDRESS_LENGTH_MAX);

   /* Clear the addressField value first. */
   OS_MEMSET(pOutProtoAddrS->addressField, 0x00, ZL303XX_PORT_ADDRESS_LENGTH_MAX);
   OS_MEMCPY(pOutProtoAddrS->addressField, (pInProtoAddrBuf + 4), copyStringLen);

   /* Return the length of the TLV field processed regardless of the number of
    * bytes actually copied (this is so that the pointer to the next field will
    * be incremented correctly by the parent function).  */
   /* 2 + 2 + string length */
   return pOutProtoAddrS->addressLength + 4;
}  /* END zl303xx_PtpV2MsgProtocolAddressUnpack */

/* zl303xx_PtpV2MsgProtocolAddressPack */
/** 

   Packs a Protocol Address value from a structure into a PTP message

  Parameters:
   [in]  pInProtoAddrS     Structure to insert into the message
   [out] pOutProtoAddrBuf  Pointer to a buffer in network order to insert
                                 the Protocol Address in network form.

  Return Value:  Uint16T  Number of bytes packed into the output buffer on success.
                  0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgProtocolAddressPack(
      zl303xx_PortAddress *pInProtoAddrS,
      Uint8T *pOutProtoAddrBuf)
{
   PACK_16BIT((pOutProtoAddrBuf + 0), pInProtoAddrS->networkProtocol);
   PACK_16BIT((pOutProtoAddrBuf + 2), pInProtoAddrS->addressLength);
   OS_MEMCPY(    (pOutProtoAddrBuf + 4), pInProtoAddrS->addressField, pInProtoAddrS->addressLength);

   /* Return the length of the packed TLV field.  */
   /* 2 + 2 + string length */
   return pInProtoAddrS->addressLength + 4;
}  /* END zl303xx_PtpV2MsgProtocolAddressPack */


/*****************   OTHER PARSING UTILITY FUNCTION DECLARATIONS   ************/

/* zl303xx_PtpMsgVersionGet */
/** 

   Extracts the PTP version number from a PTP message header buffer

   For both protocols (V1 & V2), this field is located 1 byte from the beginning
   of the header & is 4-bits long (lower 4-bits of the byte).

  Parameters:
   [in]  pInHdrBuf   Pointer to the beginning of a buffer containing a PTP
                           message header (in IEEE 1588 format).

  Return Value:
   zl303xx_PtpVersionE    The PTP version number.

*******************************************************************************/
zl303xx_PtpVersionE zl303xx_PtpMsgVersionGet(Uint8T *pInHdrBuf)
{
   /* - In PTP V1, the version number was stored as a 16-bit value at offset 0,
    *   with the only valid value being 0x0001.
    * - In PTP V2, the version number is stored as the LS 4-bits of the octet at
    *   offset 1. There is additional information in octet 0 and the MS 4-bits
    *   of octet 1. */

   /* In either version, the Version number is in the same 4-bits */
   return (*(pInHdrBuf + 1) & (Uint8T)0x0F);
}  /* END zl303xx_PtpMsgVersionGet */


/* zl303xx_PtpV2MsgPktLengthGet */
/** 

   Extracts the messageLength field from a PTP V2 packet header buffer.

  Parameters:
   [in]  pInHdrBuf   Pointer to the beginning of the PTP V2 packet buffer
                          containing the Ptp V2 message header.

  Return Value:  Uint16T  The messageLength value extracted in the PTP message header if
                  successful.
                  0 otherwise (for error checking).

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgPktLengthGet(
      Uint8T *pInHdrBuf)
{
   Uint16T messageLength = 0;

   if (pInHdrBuf != NULL)
   {
      messageLength = UNPACK_16BIT(pInHdrBuf + ZL303XX_PTP_V2_HDR_MSG_LEN_OFST);
   }

   return messageLength;
}  /* END zl303xx_PtpV2MsgPktLengthGet */

/* zl303xx_PtpV2MsgPktLengthSet */
/** 

   Inserts the specified length value into the messageLength field of a PTP V2
   message buffer.

  Parameters:
   [in]  messageLength  The message length value to insert into the packet.
   [out] pOutHdrBuf     Pointer to the beginning of the PTP V2 packet buffer
                             in which to insert the messageLength value.

  Return Value:  Uint16T  The messageLength value inserted in the PTP message header if
                  successful.
                  0 otherwise (for error checking).

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgPktLengthSet(
      Uint16T messageLength,
      Uint8T *pOutHdrBuf)
{
   Uint16T insertedLength = 0;

   if (pOutHdrBuf != NULL)
   {
      PACK_16BIT((pOutHdrBuf + ZL303XX_PTP_V2_HDR_MSG_LEN_OFST), messageLength);
      insertedLength = messageLength;
   }

   return insertedLength;
}  /* END zl303xx_PtpV2MsgPktLengthSet */

/* zl303xx_PtpV2MsgSequenceNumGet */
/** 

   Extracts the sequenceId field from a PTP V2 packet header buffer.

  Parameters:
   [in]  pInHdrBuf   Pointer to the beginning of the PTP V2 packet buffer
                          containing the Ptp V2 message header.

  Return Value:  Uint16T  The sequenceId value extracted in the PTP message header.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgSequenceNumGet(
      Uint8T *pInHdrBuf)
{
   Uint16T sequenceId = 0;

   if (pInHdrBuf != NULL)
   {
      sequenceId = UNPACK_16BIT(pInHdrBuf + ZL303XX_PTP_V2_HDR_SEQ_ID_OFST);
   }

   return sequenceId;
}  /* END zl303xx_PtpV2MsgSequenceNumGet */

/* zl303xx_PtpV2MsgSequenceNumSet */
/** 

   Inserts the specified value into the sequenceId field of a PTP V2 message buffer.

  Parameters:
   [in]   sequenceId  The sequence number to insert into the packet.
   [out]  pOutHdrBuf  Pointer to the beginning of the PTP V2 packet buffer
                           in which to insert the sequenceId value.

  Return Value:  Uint16T  The sequenceId value inserted in the PTP message header if
                  successful.
                  The inverse otherwise (for error checking).

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgSequenceNumSet(
      Uint16T sequenceId,
      Uint8T *pOutHdrBuf)
{
   if (pOutHdrBuf != NULL)
   {
      PACK_16BIT((pOutHdrBuf + ZL303XX_PTP_V2_HDR_SEQ_ID_OFST), sequenceId);
   }
   else
   {
      /* Do something to indicate an error. */
      sequenceId = (Uint16T)~sequenceId;
   }

   return sequenceId;
}  /* END zl303xx_PtpV2MsgSequenceNumSet */

/* zl303xx_PtpV2MsgCorrectionFieldSet */
/** 

   Inserts the specified correctionField value into the proper field of a PTP V2
   message buffer.

  Parameters:
   [in]  correctionField   The correctionField value to insert into the packet.
   [out] pOutHdrBuf        Pointer to the beginning of the PTP V2 packet buffer
                                 in which to insert the correctionField value.

  Return Value:  Uint16T  The number of bytes inserted in the PTP message header if
                  successful.
                  0 otherwise (for error checking).

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgCorrectionFieldSet(
      Uint64S correctionField,
      Uint8T *pOutHdrBuf)
{
   Uint16T insertedBytes = 0;

   if (pOutHdrBuf != NULL)
   {
      PACK_64BIT(pOutHdrBuf + ZL303XX_PTP_V2_HDR_CORR_OFST, correctionField);
      insertedBytes = sizeof(Uint64S);
   }

   return insertedBytes;
}  /* END zl303xx_PtpV2MsgCorrectionFieldSet */

/* zl303xx_PtpV2MsgHeaderTrace */
/** 

   Utility routine to log the PTP V2 data that has been packed/unpacked

  Parameters:
   [in]  pPtpMsgHeader  Structure containing the PTP V2 header data to log.

  Return Value:  None

*******************************************************************************/
void zl303xx_PtpV2MsgHeaderTrace(
      zl303xx_PtpV2MsgHeaderS *pPtpMsgHeader, Uint16T modId, Uint8T level)
{
   ZL303XX_DEBUG_FILTER(modId, level)
   {
     ZL303XX_TRACE_ALWAYS(
            "   messageType 0x%x",
            pPtpMsgHeader->messageType, 0,0,0,0,0);

     ZL303XX_TRACE_ALWAYS(
            "   transportSpecific %d",
            pPtpMsgHeader->transportSpecific, 0,0,0,0,0);

     ZL303XX_TRACE_ALWAYS(
            "   versionPTP %d",
            pPtpMsgHeader->versionPTP, 0,0,0,0,0);

     ZL303XX_TRACE_ALWAYS(
            "   messageLength %d",
            pPtpMsgHeader->messageLength, 0,0,0,0,0);

     ZL303XX_TRACE_ALWAYS(
            "   domainNumber %d",
            pPtpMsgHeader->domainNumber, 0,0,0,0,0);

     ZL303XX_TRACE_ALWAYS(
            "   flagField 0x%04x",
            pPtpMsgHeader->flagField, 0,0,0,0,0);

     ZL303XX_TRACE_ALWAYS(
            "   correctionField 0x%04X%04X",
            pPtpMsgHeader->correctionField.hi,
            pPtpMsgHeader->correctionField.lo,
            0,0,0,0);

      ZL303XX_TRACE_V2_PORT_ID(modId, level, "   sourcePortIdentity ",
                               &pPtpMsgHeader->sourcePortIdentity);

     ZL303XX_TRACE_ALWAYS(
            "   sequenceId %d",
            pPtpMsgHeader->sequenceId, 0,0,0,0,0);

     ZL303XX_TRACE_ALWAYS(
            "   controlField %d",
            pPtpMsgHeader->controlField, 0,0,0,0,0);

     ZL303XX_TRACE_ALWAYS(
            "   logMessageInterval %d",
            pPtpMsgHeader->logMessageInterval, 0,0,0,0,0);
   }
}


/****************   STATIC FUNCTION DEFINITIONS   *****************************/

/* zl303xx_PtpV2MsgHeaderFlagsCompute */
/** 

   Utility routine to compute the PTP header flagField based on the stream
   configuration and message type.

  Parameters:
   [in]   pPtpStream   Pointer to the target stream structure.
   [in]   messageType  PTP messageType to compute the flagField for.
   [out]  msgHdrFlags  16-bit packed flagField value.

  Return Value:  Uint16T  Always 2 (i.e. number of bytes packed(.

*******************************************************************************/
static Uint16T zl303xx_PtpV2MsgHeaderFlagsCompute(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_MessageTypeE messageType,
      Uint16T *msgHdrFlags)
{
   Uint16T flags = 0;

   /* For all message types. */
   if (pPtpStream->config.unicast == ZL303XX_TRUE)
      setFlag(flags, PTP2_FLAGS_UNICAST);

   /* The alternateMaster flag applies to ANNOUNCE, SYNC, FOLLOWUP &
    * DELAY_RESP Messages. */
   if ((messageType == ZL303XX_MSG_ID_SYNC) ||
       (messageType == ZL303XX_MSG_ID_ANNOUNCE) ||
       (messageType == ZL303XX_MSG_ID_FOLLOWUP) ||
       (messageType == ZL303XX_MSG_ID_DELAY_RESP))
   {
      if ((pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_MASTER) &&
          (pPtpStream->portData->config.alternateMaster))
      {
         setFlag(flags, PTP2_FLAGS_ALTERNATE_MASTER);
      }
   }

   /* The twoStepFlag only applies to SYNC & PDELAY Messages. */
   /* The 2-step flag of SYNC messages is determined by the Clock::DefaultDS. */
   if (messageType == ZL303XX_MSG_ID_SYNC)
   {
      if (pPtpStream->portData->clock->config.defaultDS.twoStepFlag)
         setFlag(flags, PTP2_FLAGS_TWO_STEP);
   }
   /* The 2-step flag of PDELAY_RESP messages is determined by the Port setting. */
   else if (messageType == ZL303XX_MSG_ID_PEER_DELAY_RESP)
   {
      if (!(pPtpStream->portData->config.pdRespTxMethod & ZL303XX_PTP_PDELAY_METHOD_1_STEP_FLAG))
         setFlag(flags, PTP2_FLAGS_TWO_STEP);
   }

   /* The timeProperties flags apply only to ANNOUNCE Messages. */
   if (messageType == ZL303XX_MSG_ID_ANNOUNCE)
   {
      if (pPtpStream->portData->clock->timePropertiesDS.leap61)
         setFlag(flags, PTP2_FLAGS_LI_61);
      if (pPtpStream->portData->clock->timePropertiesDS.leap59)
         setFlag(flags, PTP2_FLAGS_LI_59);
      if (pPtpStream->portData->clock->timePropertiesDS.currentUtcOffsetValid)
         setFlag(flags, PTP2_FLAGS_CURRENT_UTC_OFFSET_VALID);
      if (pPtpStream->portData->clock->timePropertiesDS.ptpTimescale)
         setFlag(flags, PTP2_FLAGS_PTP_TIMESCALE);
      if (pPtpStream->portData->clock->timePropertiesDS.timeTraceable)
         setFlag(flags, PTP2_FLAGS_TIME_TRACEABLE);
      if (pPtpStream->portData->clock->timePropertiesDS.frequencyTraceable)
         setFlag(flags, PTP2_FLAGS_FREQ_TRACEABLE);

      /* Sets only if this optional functionality is enabled */
      {
         zl303xx_BooleanE synchroUncertain = ZL303XX_FALSE;
         zl303xx_PtpClockSynchronizationUncertainEvaluate(pPtpStream->portData->clock, &synchroUncertain);

         if (synchroUncertain)
            setFlag(flags, PTP2_FLAGS_SYNCHRO_UNCERTAIN);
      }
   }

   *msgHdrFlags = flags;

   return ZL303XX_PTP_MSG_FLAGS_LEN;
}

/* zl303xx_PtpV2MsgHeaderOverridesGet */
/** 

   Replaces any fields of the PTP Header structure with configured Over-ride
   values. All other fields are left untouched, therefore, this routine should
   only be called prior to calling zl303xx_PtpV2MsgHeaderPack().

   In the case where both a Clock and a Stream over-ride value exists, the
   stream value will take priority.

  Parameters:
   [in]  pPtpStream     Pointer to the PTP stream (and clock) for which to
                              get over-ride values.
   [in]  pPtpMsgHeader  Pointer to a PTP Header structure with default values.

   [out] pPtpMsgHeader  Pointer to a PTP Header structure with configured
                              over-ride values.

  Notes:
   As this is an internal routine, all input parameters are assumed to be valid.
   It is expected that the messageType is filled correctly as an input.

*******************************************************************************/
void zl303xx_PtpV2MsgHeaderOverridesGet(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgHeaderS *pPtpMsgHeader)
{
   zl303xx_PtpClockS *pPtpClock = pPtpStream->portData->clock;

   /* Assign the Over-ride message fields in the common header */
   /************************************************************/

   /* transportSpecific (check Stream Over-ride settings) */
   if (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_TRANSPORT] == ZL303XX_TRUE)
   {
      pPtpMsgHeader->transportSpecific =
         pPtpStream->config.override.ptpHeader.transportSpecific;
   }

   /* domainNumber (check Stream & Clock Over-ride settings) */
   if (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_DOMAIN] == ZL303XX_TRUE)
   {
      pPtpMsgHeader->domainNumber =
         pPtpStream->config.override.ptpHeader.domainNumber;
   }
   else if (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_DOMAIN] == ZL303XX_TRUE)
   {
      pPtpMsgHeader->domainNumber = pPtpClock->config.override.defaultDS.domainNumber;
   }

   /* flagField */
   /* The timeProperties flags apply only to ANNOUNCE Messages. */
   if (pPtpMsgHeader->messageType == ZL303XX_MSG_ID_ANNOUNCE)
   {
      /* currentUtcOffsetValid */
      if (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_CURRENT_UTC_OFFSET_VALID] == ZL303XX_TRUE)
      {
         if (pPtpStream->config.override.timeProperties.currentUtcOffsetValid == ZL303XX_TRUE)
         {
            setFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_CURRENT_UTC_OFFSET_VALID);
         }
         else
         {
            clearFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_CURRENT_UTC_OFFSET_VALID);
            
         }
      }
      else if (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_CURRENT_UTC_OFFSET_VALID] == ZL303XX_TRUE)
      {
         if (pPtpClock->config.override.timePropertiesDS.currentUtcOffsetValid == ZL303XX_TRUE)
         {
            setFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_CURRENT_UTC_OFFSET_VALID);
         }
         else
         {
            clearFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_CURRENT_UTC_OFFSET_VALID); 
         }
      }

      /* timeTraceable */
      if (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_TIME_TRACEABLE] == ZL303XX_TRUE)
      {
         if (pPtpStream->config.override.timeProperties.timeTraceable == ZL303XX_TRUE)
         {
            setFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_TIME_TRACEABLE);
         }
         else
         {
            clearFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_TIME_TRACEABLE);    
         }
      }
      else if (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_TIME_TRACEABLE] == ZL303XX_TRUE)
      {
         if (pPtpClock->config.override.timePropertiesDS.timeTraceable == ZL303XX_TRUE)
         {
            setFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_TIME_TRACEABLE);
         }
         else
         {
            clearFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_TIME_TRACEABLE);  
         }
      }

      /* frequencyTraceable */
      if (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_FREQUENCY_TRACEABLE] == ZL303XX_TRUE)
      {
         if (pPtpStream->config.override.timeProperties.frequencyTraceable == ZL303XX_TRUE)
         {
            setFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_FREQ_TRACEABLE);
         }
         else
         {
            clearFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_FREQ_TRACEABLE);   
         }
      }
      else if (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_FREQUENCY_TRACEABLE] == ZL303XX_TRUE)
      {
         if (pPtpClock->config.override.timePropertiesDS.frequencyTraceable == ZL303XX_TRUE)
         {
            setFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_FREQ_TRACEABLE);
         }
         else
         {
            clearFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_FREQ_TRACEABLE); 
         }
      }

      /* synchronizationUncertain */
      /* We transmit any over-ride value even if this optional functionality is disabled. */
      if (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_SYNCHRO_UNCERTAIN] == ZL303XX_TRUE)
      {
         if (pPtpStream->config.override.timeProperties.synchronizationUncertain == ZL303XX_TRUE)
         {
            setFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_SYNCHRO_UNCERTAIN);
         }
         else
         {
            clearFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_SYNCHRO_UNCERTAIN);
            
         }
      }
      else if (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_SYNCHRO_UNCERTAIN] == ZL303XX_TRUE)
      {
         if (pPtpClock->config.override.timePropertiesDS.synchronizationUncertain == ZL303XX_TRUE)
         {
            setFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_SYNCHRO_UNCERTAIN);
         }
         else
         {
            clearFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_SYNCHRO_UNCERTAIN);  
         }
      }
   }

   /* correctionField (check Stream Over-ride settings) */
   /* This has to be set for delay asymmetry corrections (Clause 11.6). For
    * SYNC and PEER_DELAY_RESP it done at the receive handler: For DELAY_REQUEST
    * and PEER_DELAY_REQ it done on the transmit side.  */
   if (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_CORRECTION] == ZL303XX_TRUE)
   {
      pPtpMsgHeader->correctionField = pPtpStream->config.override.ptpHeader.correctionField;
   }

   /* sourcePortIdentity (check Stream Over-ride settings) */
   if (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_SRC_PORT_ID] == ZL303XX_TRUE)
   {
      pPtpMsgHeader->sourcePortIdentity = pPtpStream->config.override.ptpHeader.sourcePortIdentity;
   }

   return;
}

/****************   END OF VERSION 2 MESSAGE FORMATS **************************/
