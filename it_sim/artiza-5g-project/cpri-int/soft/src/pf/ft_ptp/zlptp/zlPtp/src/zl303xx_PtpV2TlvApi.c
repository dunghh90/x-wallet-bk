

/*****************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Definitions and utility functions for parsing (packing/unpacking) TLV
*     structures to/from buffers. These include generic TLV definitions and any
*     other TLVs not covered by its own module (i.e. Alternate Time Offset
*     Indicator is covered here but Signaling and Management have their own
*     modules).
*
******************************************************************************/

/****************   INCLUDE FILES   *******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"
#include "zl303xx_Error.h"

#include "zl303xx_PtpV2MsgParse.h"    /* Include all message Parsing definitions. */
#include "zl303xx_PtpConstants.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpFunctionExport.h"

/****************   DEFINES     ***********************************************/


/* Macro for checking Profile ID. */
#define ZL303XX_CHECK_ALT_TIME_KEY_FIELD(fnName, keyField) \
   ( ((keyField) == 255) ?  \
     (ZL303XX_TRACE_ALWAYS("%s: Invalid keyField (cannot == 255)", (fnName), 0,0,0,0,0), \
           ZL303XX_PARAMETER_INVALID) : \
           ZL303XX_OK )

/****************   DATA TYPES   **********************************************/

/****************   STATIC FUNCTION DECLARATIONS   ****************************/

/* Local routine for searching a Alternate Time Offset Table/Array. */
zlStatusE zl303xx_PtpTlvAltTimeOffsetEntrySearch(
      zl303xx_PtpTlvAltTimeOffsetS *altTimeTbl,
      Uint8T keyFieldValue,
      zl303xx_PtpTlvAltTimeOffsetS **altTimeMatchEntry);

/****************   STATIC GLOBAL VARIABLES   *********************************/

/****************   EXPORTED GLOBAL VARIABLES   *******************************/

/****************   EXPORTED FUNCTION DEFINITIONS   ***************************/

/* zl303xx_PtpTlvAppendedHandle */
/** 

   Processes any TLVs appended to PTP GENERAL message.

  Parameters:
   [in]  pPtpStream  Pointer to the stream on which the message was received.
   [in]  pPtpMsgRx   The PTP message data related to the received message.
   [in]  pInTlvBuf   Pointer to the remaining TLV buffer to process.

  Return Value:
   Uint16T  Number of bytes processed from the TLV buffer on success.
            0 otherwise.

  Notes:
   Assumes input pointers have already been checked.

*******************************************************************************/
Uint16T zl303xx_PtpTlvAppendedHandle(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf)
{
   Uint16T tlvBytesRemaining = 0;
   Uint16T tlvBytesProcessed = 0;
   Uint16T tlvType, tlvLength;
   Uint16T tlvCount = 0;

   Uint8T *nextInTlvStart = NULL;

   const char *fnName = "zl303xx_PtpTlvAppendedHandle";

   /* TLV buffer may be NULL: it is up to the profile to check its presence
    * and if valid data exists. */
   if ((pPtpStream == NULL) || (pPtpMsgRx == NULL) || (pInTlvBuf == NULL))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
            "%s: NULL Pointer", fnName, 0,0,0,0,0);
      return 0;
   }

   /* This routine should only process TLV appended to General messages. */
   /* FOR NOW: ONLY ANNOUNCE & SIGNALING MESSAGES ARE HANDLED */
   if (pPtpMsgRx->header.messageType == ZL303XX_MSG_ID_ANNOUNCE)
   {
      /* For ANNOUNCE Messages, the entire TLV buffer is passed in so the #bytes
       * is everything after the ANNC data.     */
      tlvBytesRemaining = pPtpMsgRx->header.messageLength - ZL303XX_PTP_V2_ANNC_MSG_LEN;
   }
   /* For SIGNALING Messages, single TLV buffers are passed in so the #bytes is
    * the lengthField inside the TLV itself (+ the 4 byte type-length header).  */
   else if (pPtpMsgRx->header.messageType == ZL303XX_MSG_ID_SIGNALING)
   {
      (void)zl303xx_PtpTlvTypeLengthGet(pInTlvBuf, &tlvType, &tlvLength);
      tlvBytesRemaining = tlvLength + ZL303XX_PTP_TLV_BASE_LEN;
   }
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
            "%s: Invalid Message Type (%u)",
            fnName, pPtpMsgRx->header.messageType, 0,0,0,0);
      return 0;
   }

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
         "%s: MsgLen(%u) : Appended Bytes(%u)",
         fnName, pPtpMsgRx->header.messageLength, tlvBytesRemaining, 0,0,0);

   /* Continue processing TLVs until the buffer is exhausted. */
   while (tlvBytesRemaining >= ZL303XX_PTP_TLV_BASE_LEN)
   {
      /* Track the number of TLVs appended for logging purposes. */
   	tlvCount++;
   	nextInTlvStart = pInTlvBuf + tlvBytesProcessed;

      /* Get the TYPE and LENGTH from the TLV. */
      (void)zl303xx_PtpTlvTypeLengthGet(nextInTlvStart, &tlvType, &tlvLength);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
            "%s: TLV#(%u): TYPE:LENGTH (%u:%u)",
            fnName, tlvCount, tlvType, tlvLength, 0,0);

      switch (tlvType)
      {
         case PTP_TLV_VENDOR_EXTENSION :
         {
            Uint8T custTlvReplyLen = 0;
            Uint8T pOutTlvBuf[256];

            custTlvReplyLen = zl303xx_PtpV2TlvCustomHandler(
                                 pPtpStream->portData->clock->customTlvTable,
                                 pPtpStream->portData->clock->clockHandle,
                                 pPtpMsgRx,
                                 nextInTlvStart,
                                 pOutTlvBuf);

            if ((pPtpMsgRx->data.management.actionField == PTP_MANAGEMENT_ACTION_ACK) ||
                (pPtpMsgRx->data.management.actionField == PTP_MANAGEMENT_ACTION_RSP))
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
                     "%s: VENDOR_EXT - expected to reply: action(%u):length(%u)",
                     fnName, pPtpMsgRx->data.management.actionField, custTlvReplyLen, 0,0,0);
            }

            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
                  "%s: VENDOR_EXT - tlvLen(%u), processed(%u)",
                  fnName, (tlvLength + ZL303XX_PTP_TLV_BASE_LEN), custTlvReplyLen, 0,0,0);

            break;
         }

         case PTP_TLV_PATH_TRACE :
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
                  "%s: PATH_TRACE TLV Received",
                  fnName, 0,0,0,0,0);

            break;
         }

         case PTP_TLV_ALTERNATE_TIME_OFFSET_INDICATOR :
         {
            zl303xx_PtpTlvAltTimeOffsetHandle(pPtpStream, pPtpMsgRx, nextInTlvStart);
            break;
         }

         case PTP_TLV_PAD :
         {
            /* Nothing to do with this. Just let the bytes be eaten (below). */
            break;
         }

         /* Nothing done with this */
         case PTP_TLV_RESERVED :

         /* Signaling TLVs can only be appended to Signaling Messages since the
          * targetPortIdentity is required from the message data. This TLV is
          * dropped here but should have been handled by the Signaling Message
          * handler. */
         case PTP_TLV_REQUEST_UNICAST_TX :
         case PTP_TLV_GRANT_UNICAST_TX :
         case PTP_TLV_CANCEL_UNICAST_TX :
         case PTP_TLV_CANCEL_UNICAST_TX_ACK :

         /* Management TLVs can only be appended to Management Messages since the
          * actionField is required from the message data. This TLV is dropped
          * here but should have been handled by the Management Message handler. */
         case PTP_TLV_MANAGEMENT :
         case PTP_TLV_MANAGEMENT_ERROR_STATUS :

         /* IEEE-1588-2017 */
         case PTP_TLV_CUMULATIVE_RATE_RATIO :
         case PTP_TLV_ENHANCED_ACCURACY_METRICS :

         default :
         {
            /* This TLV is unknown or can not be handled here so pass it off to
             * the Event Notification module. */
            ZL303XX_TRACE_ALWAYS(
                  "%s: Cannot process tlvType(%u):ignored bytes(%u)",
                  fnName, tlvType, (tlvLength + ZL303XX_PTP_TLV_BASE_LEN), 0,0,0);

            zl303xx_PtpEventUnknownTlv(pPtpStream->portData, pPtpStream,
                                       pPtpMsgRx,
                                       (tlvLength + ZL303XX_PTP_TLV_BASE_LEN), nextInTlvStart);
            break;
         }
      }  /* switch */

      tlvBytesProcessed += (tlvLength + ZL303XX_PTP_TLV_BASE_LEN);
      tlvBytesRemaining -= (tlvLength + ZL303XX_PTP_TLV_BASE_LEN);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 3,
            "%s: tlvType(%u) - processed(%u):remaining(%u)",
            fnName, tlvType, (tlvLength + ZL303XX_PTP_TLV_BASE_LEN),
            tlvBytesRemaining, 0,0);
   }  /* while */

   return tlvBytesProcessed;
}  /* END zl303xx_PtpTlvAppendedHandle */

/* zl303xx_PtpTlvEventMsgHandle */
/** 

   Processes TLVs appended to PTP EVENT message.

  Parameters:
   [in]  pPtpStream  Pointer to the stream on which the message was received.
   [in]  pPtpMsgRx   The PTP message data related to the received message.
   [in]  pInTlvBuf   Pointer to the remaining TLV buffer to process.

  Return Value:
   Uint16T  Number of bytes processed from the TLV buffer on success.
            0 otherwise.

  Notes:
   Assumes input pointers have already been checked.

*******************************************************************************/
Uint16T zl303xx_PtpTlvEventMsgdHandle(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf)
{
   Uint16T tlvBytesRemaining = 0;
   Uint16T tlvBytesProcessed = 0;
   Uint16T tlvType, tlvLength;
   Uint16T tlvCount = 0;

   Uint8T *nextInTlvStart = NULL;

   const char *fnName = "zl303xx_PtpTlvEventMsgHandle";

   /* TLV buffer may be NULL: it is up to the profile to check its presence
    * and if valid data exists. */
   if ((pPtpStream == NULL) || (pPtpMsgRx == NULL) || (pInTlvBuf == NULL))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
            "%s: NULL Pointer", fnName, 0,0,0,0,0);
      return 0;
   }

   /* Only EVENT Messages allowed. */
   if (ZL303XX_CHECK_EVENT_MSG_TYPE_VALID(pPtpMsgRx->header.messageType) != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
            "%s: Invalid messageType(%d): Only EVENT Messages permitted",
            fnName, pPtpMsgRx->header.messageType, 0,0,0,0);
      return 0;
   }
   /* The TLV #bytes is everything after the header + data.     */
   else
   {
      tlvBytesRemaining = pPtpMsgRx->header.messageLength - zl303xx_PtpV2MsgLengthOfType(pPtpMsgRx->header.messageType);
   }

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
         "%s: MsgType(%d) : MsgLen(%u) : Appended Bytes(%u)",
         fnName,
         pPtpMsgRx->header.messageType,
         pPtpMsgRx->header.messageLength,
         tlvBytesRemaining, 0,0);

   /* Continue processing TLVs until the buffer is exhausted. */
   while (tlvBytesRemaining >= ZL303XX_PTP_TLV_BASE_LEN)
   {
      /* Track the number of TLVs appended for logging purposes. */
      tlvCount++;
      nextInTlvStart = pInTlvBuf + tlvBytesProcessed;

      /* Get the TYPE and LENGTH from the TLV. */
      (void)zl303xx_PtpTlvTypeLengthGet(nextInTlvStart, &tlvType, &tlvLength);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
            "%s: TLV#(%u): TYPE:LENGTH (%u:%u)",
            fnName, tlvCount, tlvType, tlvLength, 0,0);

      switch (tlvType)
      {
         /* This may be the messageLength Extension scenario of
          * IEEE-1588-2017 (Clause 16.14)  */
         case PTP_TLV_RESERVED :
         {
            if (tlvLength != 0)
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
                     "%s: Dumping Extension TLV with non-Zero Length:  TLV#(%u): TYPE:LENGTH (%u:%u)",
                     fnName, tlvCount, tlvType, tlvLength, 0,0);
            }
            else /* tlvLength == 0 */
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
                     "%s: Padding Extension TLV :  TLV#(%u): TYPE:LENGTH (%u:%u)",
                     fnName, tlvCount, tlvType, tlvLength, 0,0);
            }

            /* Either way, throw away the remaining buffer. */
            tlvLength = tlvBytesRemaining - ZL303XX_PTP_TLV_BASE_LEN;

            break;
         }

         /* Nothing to do with this. Just let the bytes be eaten (below). */
         case PTP_TLV_PAD :
         {
            break;
         }

         case PTP_TLV_VENDOR_EXTENSION :

         /* The rest of the TLVs are not valid on EVENT messages. */
         case PTP_TLV_PATH_TRACE :
         case PTP_TLV_ALTERNATE_TIME_OFFSET_INDICATOR :
         case PTP_TLV_REQUEST_UNICAST_TX :
         case PTP_TLV_GRANT_UNICAST_TX :
         case PTP_TLV_CANCEL_UNICAST_TX :
         case PTP_TLV_CANCEL_UNICAST_TX_ACK :
         case PTP_TLV_MANAGEMENT :
         case PTP_TLV_MANAGEMENT_ERROR_STATUS :

         /* IEEE-1588-2017 */
         case PTP_TLV_CUMULATIVE_RATE_RATIO :
         case PTP_TLV_ENHANCED_ACCURACY_METRICS :

         default :
         {
            /* This TLV is unknown or can not be handled here so pass it off to
             * the Event Notification module. */
            ZL303XX_TRACE_ALWAYS(
                  "%s: Cannot process tlvType(%u):ignored bytes(%u)",
                  fnName, tlvType, (tlvLength + ZL303XX_PTP_TLV_BASE_LEN), 0,0,0);

            zl303xx_PtpEventUnknownTlv(pPtpStream->portData, pPtpStream,
                                       pPtpMsgRx,
                                       (tlvLength + ZL303XX_PTP_TLV_BASE_LEN), nextInTlvStart);
            break;
         }
      }  /* switch */

      tlvBytesProcessed += (tlvLength + ZL303XX_PTP_TLV_BASE_LEN);
      tlvBytesRemaining -= (tlvLength + ZL303XX_PTP_TLV_BASE_LEN);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 3,
            "%s: tlvType(%u) - processed(%u):remaining(%u)",
            fnName, tlvType, (tlvLength + ZL303XX_PTP_TLV_BASE_LEN),
            tlvBytesRemaining, 0,0);
   }  /* while */

   return tlvBytesProcessed;
}  /* END zl303xx_PtpTlvEventMsgHandle */

/* zl303xx_PtpTlvTypeLengthGet */
/** 

   Gets the TYPE and LENGTH fields of a general TLV buffer.

  Parameters:
   [in]  pInTlvBuf   Pointer to the start of the TLV buffer (starts with the
                           TYPE field).

   [out] tlvType     The TLV TYPE field extracted from the buffer (may be NULL).
   [out] tlvLength   The TLV LENGTH field extracted from the buffer (may be NULL).

  Return Value:  None

  Notes:
   Assumes input pointers have already been checked.

*******************************************************************************/
Uint16T zl303xx_PtpTlvTypeLengthGet(
      Uint8T  *pInTlvBuf,
      Uint16T *tlvType,
      Uint16T *tlvLength)
{
   return zl303xx_PtpV2MgmtTlvTypeLengthIdGet(pInTlvBuf, tlvType, tlvLength, NULL);
}  /* END zl303xx_PtpTlvTypeLengthGet */

/* zl303xx_PtpTlvTypeSet */
/** 

   Packs the TYPE fields of a general TLV buffer.

  Parameters:
   [in]  tlvType        The TLV TYPE field.

   [out] pOutTlvBuf     Pointer to a buffer containing a TLV (starts with
                              the TYPE field).

  Return Value:  None

  Notes:
   Assumes input pointers have already been checked.

*******************************************************************************/
Uint16T zl303xx_PtpTlvTypeSet(
      Uint16T tlvType,
      Uint8T *pOutTlvBuf)
{
   /* The TYPE is the first 2 bytes. */
   PACK_16BIT((pOutTlvBuf + 0), tlvType);

   return sizeof(tlvType);
}  /* END zl303xx_PtpTlvTypeSet */

/* zl303xx_PtpTlvLengthSet */
/** 

   Packs the LENGTH of a general TLV buffer.

  Parameters:
   [in]  tlvLength      The TLV LENGTH field.

   [out] pOutTlvBuf     Pointer to a buffer containing a TLV (starts with
                              the TYPE field).

  Return Value:  None

  Notes:
   Assumes input pointers have already been checked.

*******************************************************************************/
Uint16T zl303xx_PtpTlvLengthSet(
      Uint16T tlvLength,
      Uint8T *pOutTlvBuf)
{
   /* The LENGTH is the 3rd & 4th bytes. */
   PACK_16BIT((pOutTlvBuf + 2), tlvLength);

   return sizeof(tlvLength);
}  /* END zl303xx_PtpTlvLengthSet */

/*

  Function Name:
    zl303xx_PtpTlvOrganizationHeaderUnpack

  Details:
   Unpacks the Organization Extension TLV Header into a compatible data type
   for local use.

  Parameters:
   [in]  pInTlvBuf   Pointer to the header portion of the input TLV buffer.

   [out] orgId       Organization ID unpacked from the TLV Header.
   [out] orgSubType  Organization Sub-Type unpacked from the TLV Header.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpTlvOrganizationHeaderUnpack(
      Uint8T *pInTlvBuf,
      Uint8T *orgId,
      Uint8T *orgSubType)
{
   /* Organization Extension TLV Header Composition (Clause 14.3.2):
    * -------------------------------------------------------
    * Field                Length      Offset
    * ------------------------------------------------------
    * tlvType                 2           0
    * lengthField             2           2
    * organizationId          3           4
    * organizationSubType     3           7
    * dataField               N           10
    * ------------------------------------------------------
    * Total Length        (10 + N)    */

   /* Check that this is an Organization Extension TLV with the minimum
    * number of defined bytes.   */
   zl303xx_PtpV2TlvTypeE tlvType = UNPACK_16BIT(pInTlvBuf + 0);
   Uint16T tlvLength           = UNPACK_16BIT(pInTlvBuf + 2);

   if (tlvType != PTP_TLV_VENDOR_EXTENSION)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4,
            "zl303xx_PtpTlvOrganizationHeaderUnpack: Invalid tlvType (%u)",
            tlvType, 0,0,0,0,0);
      return 0;
   }
   else if (tlvLength < (ZL303XX_PTP_TLV_ORG_EXT_HEADER_LEN - ZL303XX_PTP_TLV_BASE_LEN))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4,
            "zl303xx_PtpTlvOrganizationHeaderUnpack: Invalid length (%u)",
            tlvLength, 0,0,0,0,0);
      return 0;
   }

   /* At this point, the TLV is accepted. */

   /* organizationId */
   OS_MEMCPY(orgId, (pInTlvBuf + 4), ZL303XX_PTP_NODE_OUI_LEN);

   /* organizationSubType */
   OS_MEMCPY(orgSubType, (pInTlvBuf + 7), ZL303XX_PTP_NODE_OUI_SUBTYPE_LEN);

   return tlvLength + ZL303XX_PTP_TLV_BASE_LEN;
}  /* END zl303xx_PtpTlvOrganizationHeaderUnpack */

/*

  Function Name:
    zl303xx_PtpTlvOrganizationHeaderPack

  Details:
   Packs the Organization Extension TLV Header data structure into a TLV buffer
   for transmission.

  Parameters:
   [in]  orgId       Organization ID to packed into the TLV Header.
   [in]  orgSubType  Organization Sub-Type to packed into the TLV Header.

   [out] pOutTlvBuf  Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpTlvOrganizationHeaderPack(
      Uint8T *orgId,
      Uint8T *orgSubType,
      Uint8T *pOutTlvBuf)
{
   /* Organization Extension TLV Header Composition (Clause 14.3.2):
    * -------------------------------------------------------
    * Field                Length      Offset
    * ------------------------------------------------------
    * tlvType                 2           0
    * lengthField             2           2
    * organizationId          3           4
    * organizationSubType     3           7
    * dataField               N           10
    * ------------------------------------------------------
    * Total Length        (10 + N)    */


   /* Pack the necessary TLV information. */
   OS_MEMSET(pOutTlvBuf, 0x00, ZL303XX_PTP_TLV_ORG_EXT_HEADER_LEN);

   /* tlvType = PTP_TLV_VENDOR_EXTENSION */
   PACK_16BIT((pOutTlvBuf + 0), PTP_TLV_VENDOR_EXTENSION);
   /* length = For now, pack the header length. If data is added, it is the
    *          responsibility of the Org-Specific routine to update the length. */
   PACK_16BIT((pOutTlvBuf + 2), (ZL303XX_PTP_TLV_ORG_EXT_HEADER_LEN - ZL303XX_PTP_TLV_BASE_LEN));

   /* organizationId */
   OS_MEMCPY((pOutTlvBuf + 4), orgId, ZL303XX_PTP_NODE_OUI_LEN);

   /* organizationSubType */
   OS_MEMCPY((pOutTlvBuf + 7), orgSubType, ZL303XX_PTP_NODE_OUI_SUBTYPE_LEN);

   return ZL303XX_PTP_TLV_ORG_EXT_HEADER_LEN;
}  /* END zl303xx_PtpTlvOrganizationHeaderPack */

/*

  Function Name:
    zl303xx_PtpTlvConvert3ByteFieldTo32Bit

  Details:
   Converts a 3-byte TLV Field (vendor OUI, and sub-type) from a CHAR array to
   a 32-bit value in the following way:

               index:     [0]   [1]   [2]
   Uint8T  vendorOUI[] = {0x0A, 0x0B, 0x0C};
   Uint32T out32bits   = 0x000A0B0C

   This is done in order to manage Custom TLV handling. The following is an
   example of how to add a custom handler:

   status = zl303xx_PtpTlvCustomHandlerSet(
                              Uint32T clockHandle,
                              Uint16T tlvType,
                              Uint32T tlvId,
                              zl303xx_PtpV2TlvCustomHandlerT fnTlvHandler);

  Parameters:
   [in]  pInByteBuf  Pointer to the start of the 3-byte buffer.

   [out] out32bits   3-Byte field unpacked from the buffer and returned
                           as a 32-bit value (with MSB = 0).

  Return Value:
   Uint16T  Number of bytes unpacked from the input buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpTlvConvert3ByteFieldTo32Bit(
      Uint8T  *pInByteBuf,
      Uint32T *out32bits)
{

   /* Clear the return value. */
   *out32bits = 0;

   /* Shift in each byte from the buffer. */
   /* Start by unpacking from the beginning of the buffer...
    * out32 = 0x000A0B0C */
   *out32bits = ((Uint32T)(pInByteBuf[0]) << 16) +
                ((Uint32T)(pInByteBuf[1]) << 8)  +
                 (Uint32T)(pInByteBuf[2]);

   return 3;
}  /* END zl303xx_PtpTlvConvert3ByteFieldTo32Bit */

/*

  Function Name:
    zl303xx_PtpTlvAltTimeOffsetUnpack

  Details:
   Unpacks the Alternate Time Offset Indicator TLV into a compatible data
   type for local use.

  Parameters:
   [in]  pInTlvBuf   Pointer to the data portion of the input TLV buffer.

   [out] altTimeS    Pointer to the unpacked Alternate Time Offset Indicator
                           specific data structure.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpTlvAltTimeOffsetUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_PtpTlvAltTimeOffsetS *altTimeS)
{
   /* Alternate Time Offset Indicator TLV Composition (Clause 16.3.3):
    * -------------------------------------------------------
    * Field                Length      Offset
    * ------------------------------------------------------
    * tlvType                 2           0
    * lengthField             2           2
    * keyField                1           4
    * currentOffset           4           5
    * jumpSeconds             4           9
    * timeOfNextJump          6           13
    * displayName             L           19
    * pad                     M           19+L
    * ------------------------------------------------------
    * Total Length       (19 + L + M)    */

   /* Check that this is an Alternate Time Offset Indicator TLV with the minimum
    * number of defined bytes.   */
   Uint16T tlvType   = UNPACK_16BIT(pInTlvBuf + 0);
   Uint16T tlvLength = UNPACK_16BIT(pInTlvBuf + 2);
   if (tlvType != PTP_TLV_ALTERNATE_TIME_OFFSET_INDICATOR)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "zl303xx_PtpTlvAltTimeOffsetUnpack: Invalid tlvType (%u)",
            tlvType, 0,0,0,0,0);
      return 0;
   }
   else if (tlvLength < ZL303XX_PTP_TLV_ALT_TIME_OFFSET_MIN_LEN)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4,
            "zl303xx_PtpTlvAltTimeOffsetUnpack: Invalid length (%u)",
            tlvLength, 0,0,0,0,0);
      return 0;
   }

   /* At this point, the TLV is accepted. */

   /* keyField */
   altTimeS->keyField = UNPACK_16BIT(pInTlvBuf + 4);
   /* If appended, then assume it was enabled. */
   altTimeS->enabled = ZL303XX_TRUE;

   /* currentOffset */
   altTimeS->currentOffset = UNPACK_32BIT(pInTlvBuf + 5);

   /* jumpSeconds */
   altTimeS->jumpSeconds = UNPACK_32BIT(pInTlvBuf + 9);

   /* timeOfNextJump */
   altTimeS->timeOfNextJump.hi = UNPACK_16BIT(pInTlvBuf + 13);
   altTimeS->timeOfNextJump.lo = UNPACK_32BIT(pInTlvBuf + 15);

   /* displayName */
   /* We don't do any length checks against tlvLength. */
   (void)zl303xx_PtpV2MsgTextUnpack(
                              (pInTlvBuf + ZL303XX_PTP_TLV_ALT_TIME_OFFSET_MIN_LEN),
                              altTimeS->displayName,
                              ZL303XX_PTP_TLV_ALT_TIME_OFFSET_DISPLAY_MAX_LEN);

   return tlvLength + ZL303XX_PTP_TLV_BASE_LEN;
}  /* END zl303xx_PtpTlvAltTimeOffsetUnpack */

/*

  Function Name:
    zl303xx_PtpTlvAltTimeOffsetPack

  Details:
   Packs the Alternate Time Offset Indicator data structure into a TLV buffer
   for transmission.

  Parameters:
   [in]  altTimeS    Pointer to the Alternate Time Offset Indicator data
                           structure to pack.

   [out] pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpTlvAltTimeOffsetPack(
      zl303xx_PtpTlvAltTimeOffsetS *altTimeS,
      Uint8T *pOutTlvBuf)
{
   /* Alternate Time Offset Indicator TLV Composition (Clause 16.3.3):
    * -------------------------------------------------------
    * Field                Length      Offset
    * ------------------------------------------------------
    * tlvType                 2           0
    * lengthField             2           2
    * keyField                1           4
    * currentOffset           4           5
    * jumpSeconds             4           9
    * timeOfNextJump          6           13
    * displayName             L           19
    * pad                     M           19+L
    * ------------------------------------------------------
    * Total Length       (19 + L + M)    */

   Uint16T bytesPacked = 0;
   Uint16T tlvType = PTP_TLV_ALTERNATE_TIME_OFFSET_INDICATOR;

   /* Pack the necessary TLV information. */
   /* tlvType = PTP_TLV_ALTERNATE_TIME_OFFSET_INDICATOR */
   PACK_16BIT((pOutTlvBuf + 0), tlvType);
   /* length = Packed below */

   /* keyField */
   PACK_8BIT((pOutTlvBuf + 4), altTimeS->keyField);

   /* currentOffset */
   PACK_32BIT((pOutTlvBuf + 5), altTimeS->currentOffset);

   /* jumpSeconds */
   PACK_32BIT((pOutTlvBuf + 9), altTimeS->jumpSeconds);

   /* timeOfNextJump */
   PACK_16BIT((pOutTlvBuf + 13), (Uint16T)altTimeS->timeOfNextJump.hi);
   PACK_32BIT((pOutTlvBuf + 15), altTimeS->timeOfNextJump.lo);

   bytesPacked = 19;  /* End of the Defined length fields */

   /* displayName */
   bytesPacked += zl303xx_PtpV2MsgTextPack(altTimeS->displayName,
                                          (pOutTlvBuf + bytesPacked),
                                          ZL303XX_PTP_TLV_ALT_TIME_OFFSET_DISPLAY_MAX_LEN);

   /* Add a PAD byte to make it even if necessary. */
   if ((bytesPacked % 2) != 0)
   {
      OS_MEMSET((pOutTlvBuf + bytesPacked), 0x00, 1);
      bytesPacked += 1;
   }

   /* Add the length within this TLV */
   /* length = (Ttl - 4 bytes of Type & Length) */
   zl303xx_PtpTlvLengthSet((bytesPacked - ZL303XX_PTP_TLV_BASE_LEN), pOutTlvBuf);

   return bytesPacked;
}  /* END zl303xx_PtpTlvAltTimeOffsetPack */

/* zl303xx_PtpTlvAltTimeOffsetHandle */
/** 

   Processes a received Alternate Time Offset Indicator TLV.

  Parameters:
   [in]  pPtpStream  Pointer to the stream on which the message was received.
   [in]  pPtpMsgRx   The PTP message data related to the received message.
   [in]  pInTlvBuf   Pointer to the TLV buffer to process.

  Return Value:
   Uint16T  Number of bytes processed from the TLV buffer on success.
            0 otherwise.

  Notes:
   Assumes input pointers have already been checked.

*******************************************************************************/
Uint16T zl303xx_PtpTlvAltTimeOffsetHandle(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf)
{
   Uint16T tlvBytes = 0;
   zl303xx_PtpTlvAltTimeOffsetS altTimeOffsetS;

   /* Alternate Time Offset Indicator TLV can only be appended to announce
    * messages according to Clause 16.3.1 */
   if (pPtpMsgRx->header.messageType != ZL303XX_MSG_ID_ANNOUNCE)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "zl303xx_PtpTlvAltTimeOffsetHandle: ERROR : messageType(%u) != ANNC",
            pPtpMsgRx->header.messageType, 0,0,0,0,0);
   }

   /* Unpack the TLV. */
   else if ((tlvBytes = zl303xx_PtpTlvAltTimeOffsetUnpack(pInTlvBuf, &altTimeOffsetS)) <
                                                    ZL303XX_PTP_TLV_ALT_TIME_OFFSET_MIN_LEN)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "zl303xx_PtpTlvAltTimeOffsetHandle: ERROR : messageLength(%u) too small",
            tlvBytes, 0,0,0,0,0);
   }

   /* Check that the ANNC message is from the current GM. */
   else if (zl303xx_PtpV2EqualClockIdentities(
                  pPtpStream->portData->clock->parentDS.grandmasterIdentity,
                  pPtpMsgRx->data.announce.grandmasterIdentity) == ZL303XX_TRUE)
   {
      /* Update the learned values so that any Tx ANNC on BCs will append it. */
      pPtpStream->portData->clock->altTimeOffset.parentEntry = altTimeOffsetS;
   }

   /* The LENGTH field of the TLV does not include the 4 bytes needed
    * for the TYPE and LENGTH fields so add these 4 bytes.         */
   return tlvBytes + ZL303XX_PTP_TLV_VALUE_OFFSET;
}

/*

  Function Name:
   zl303xx_PtpTlvAltTimeOffsetEntrySearch

  Details:
   Searches the specified Alternate Time Offset Table for a record with the
   keyField value specified.

  Parameters:
   [in]  altTimeTbl     Pointer to the Clock's Alternate Time Offset Table.
   [in]  keyFieldValue  keyField value to search for.

   [out] altTimeMatchEntry    Pointer to a matching record (if found).
                                 Pointer to an empty record if available.
                                 NULL if no match and TABLE FULL.

  Return Value:
    ZL303XX_TABLE_ENTRY_DUPLICATE    If a matching record was found.
    ZL303XX_TABLE_ENTRY_NOT_FOUND    If no match was found but empty space exists.
    ZL303XX_TABLE_FULL               If no match was found and the table is Full.

  Notes:
   Input pointers are assumed OK for this local routine.

 *****************************************************************************/
zlStatusE zl303xx_PtpTlvAltTimeOffsetEntrySearch(
      zl303xx_PtpTlvAltTimeOffsetS *altTimeTbl,
      Uint8T keyFieldValue,
      zl303xx_PtpTlvAltTimeOffsetS **altTimeMatchEntry)
{
   /* Ignore input pointer & parameter checking for this LOCAL routine. */
   zlStatusE status = ZL303XX_TABLE_ENTRY_DUPLICATE;    /* Most likely case. */

   /* Table index counter. */
   Uint16T tblIndex;

   /* Set the default return values. */
   zl303xx_PtpTlvAltTimeOffsetS *altTimeEmptyEntry = NULL;
   *altTimeMatchEntry = NULL;

   /* Search each entry in the table for the first KEY match. */
   for(tblIndex = 0; tblIndex < ZL303XX_PTP_ALT_TIME_OFFSET_TBL_ENTRIES; tblIndex++)
   {
      if (altTimeTbl[tblIndex].keyField == keyFieldValue)
      {
         /* Entry found so exit the search. */
         *altTimeMatchEntry = &altTimeTbl[tblIndex];
         break;
      }

      /* If not a match, check if it is an EMPTY entry and mark it for later. */
      else if ((altTimeTbl[tblIndex].keyField == 255) &&
               (altTimeEmptyEntry == NULL))
      {
         altTimeEmptyEntry = &altTimeTbl[tblIndex];
      }
   }

   /* Evaluate the search... */
   if (*altTimeMatchEntry == NULL)
   {
      /* If no match was found, try to return an empty entry. */
      if (altTimeEmptyEntry == NULL)
      {
         status = ZL303XX_TABLE_FULL;
      }

      /* No match was found for an expected entry. */
      else
      {
         *altTimeMatchEntry = altTimeEmptyEntry;
         status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
      }
   }

   return status;
}

/*

  Function Name:
   zl303xx_PtpTlvAltTimeOffsetEntryAdd

  Details:
   Adds (or replaces) the specified entry in the Alternate Time Offset Table of
   the specified clock.

  Parameters:
   [in]  clockHandle    Handle of the clock to add/update the entry.
   [in]  altTimeS       Pointer to the Alternate Time Offset entry to add/update.

  Return Value:
    ZL303XX_OK                 Entry added/Updated.
    ZL303XX_PARAMETER_INVALID  The keyField value of the entry is INVALID.
    ZL303XX_TABLE_FULL         If no match was found and the table is already Full.

 *****************************************************************************/
zlStatusE zl303xx_PtpTlvAltTimeOffsetEntryAdd(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpTlvAltTimeOffsetS *altTimeS)
{
   /* Input pointers are not verified locally (see note above). */
   zlStatusE status = ZL303XX_OK;

   /* Pointer to the targeted clock. */
   zl303xx_PtpClockS * pPtpClock = NULL;

   /* Pointer to a matching entry in the table. */
   zl303xx_PtpTlvAltTimeOffsetS *altTimeMatchEntry = NULL;

   /* Check Input parameters */
   if (status == ZL303XX_OK)
   {
      if (altTimeS == NULL)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpTlvAltTimeOffsetEntryAdd: altTimeS == NULL",
               0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
      else
      {
         status = ZL303XX_CHECK_ALT_TIME_KEY_FIELD("zl303xx_PtpTlvAltTimeOffsetEntryAdd", altTimeS->keyField);
      }
   }

   /* Get the specified Clock Data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Get a pointer to the entry or an empty entry.  */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpTlvAltTimeOffsetEntrySearch(
                        pPtpClock->altTimeOffset.entryTbl,
                        altTimeS->keyField,
                        &altTimeMatchEntry);

      /* DUPLICATE::  entry exists so replace it.
       * NOT_FOUND::  entry does not exist so add it to the empty record.
       * TABLE_FULL:: entry does not exist but there is no more space. */
      if (status == ZL303XX_TABLE_ENTRY_DUPLICATE)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpTlvAltTimeOffsetEntryAdd: Record Replaced (keyField: %u)",
               altTimeS->keyField, 0,0,0,0,0);

         *altTimeMatchEntry = *altTimeS;

         /* Update successful. */
         status = ZL303XX_OK;
      }
      else if (status == ZL303XX_TABLE_ENTRY_NOT_FOUND)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpTlvAltTimeOffsetEntryAdd: Record Added (keyField: %u)",
               altTimeS->keyField, 0,0,0,0,0);

         *altTimeMatchEntry = *altTimeS;

         /* Addition successful. */
         status = ZL303XX_OK;
      }
      else /* ZL303XX_TABLE_FULL */
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpTlvAltTimeOffsetEntryAdd: Table full",
               0,0,0,0,0,0);
      }
   }

   return status;
}

/*

  Function Name:
   zl303xx_PtpTlvAltTimeOffsetEntryDel

  Details:
   Removes the specified entry from the Alternate Time Offset Table of the
   specified clock.

  Parameters:
   [in]  clockHandle    Handle of the clock to add/update the entry.
   [in]  keyField       The Alternate Time Offset entry to remove.

  Return Value:
    ZL303XX_OK                 Entry added/Updated.
    ZL303XX_PARAMETER_INVALID  The keyField value of the entry is INVALID.

 *****************************************************************************/
zlStatusE zl303xx_PtpTlvAltTimeOffsetEntryDel(
      zl303xx_PtpClockHandleT clockHandle,
      Uint8T keyField)
{
   /* Input pointers are not verified locally (see note above). */
   zlStatusE status = ZL303XX_OK;

   /* Pointer to the targeted clock. */
   zl303xx_PtpClockS * pPtpClock = NULL;

   /* Pointer to a matching entry in the table. */
   zl303xx_PtpTlvAltTimeOffsetS *altTimeMatchEntry = NULL;

   /* Check Input parameters */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_ALT_TIME_KEY_FIELD("zl303xx_PtpTlvAltTimeOffsetEntryDel", keyField);
   }

   /* Get the specified Clock Data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Get a pointer to the entry.  */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpTlvAltTimeOffsetEntrySearch(
                        pPtpClock->altTimeOffset.entryTbl,
                        keyField,
                        &altTimeMatchEntry);

      /* DUPLICATE::  entry exists so delete it.
       * NOT_FOUND::  entry does not exist so do nothing.
       * TABLE_FULL:: entry does not exist so do nothing. */
      if (status == ZL303XX_TABLE_ENTRY_DUPLICATE)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpTlvAltTimeOffsetEntryDel: Record Removed (keyField: %u)",
               keyField, 0,0,0,0,0);

         altTimeMatchEntry->keyField = 255;

         /* Entry deleted. */
         status = ZL303XX_OK;
      }

      /* ZL303XX_TABLE_ENTRY_NOT_FOUND or ZL303XX_TABLE_FULL */
      else
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpTlvAltTimeOffsetEntryDel: Entry does not exist",
               0,0,0,0,0,0);

         /* Still return OK since this is the desired state. */
         status = ZL303XX_OK;
      }
   }

   return status;
}

/*

  Function Name:
   zl303xx_PtpTlvAltTimeOffsetEntryEnable

  Details:
   Enables / Disables the specified entry in the Alternate Time Offset Table
   of the specified clock.

  Parameters:
   [in]  clockHandle    Handle of the clock to add/update the entry.
   [in]  keyField       The Alternate Time Offset entry to remove.
   [in]  enable         Boolean to enable or Disable the entry.

  Return Value:
    ZL303XX_OK                 Entry added/Updated.
    ZL303XX_PARAMETER_INVALID  The keyField value of the entry is INVALID.

 *****************************************************************************/
zlStatusE zl303xx_PtpTlvAltTimeOffsetEntryEnable(
      zl303xx_PtpClockHandleT clockHandle,
      Uint8T keyField,
      zl303xx_BooleanE enable)
{
   /* Input pointers are not verified locally (see note above). */
   zlStatusE status = ZL303XX_OK;

   /* Pointer to the targeted clock. */
   zl303xx_PtpClockS * pPtpClock = NULL;

   /* Pointer to a matching entry in the table. */
   zl303xx_PtpTlvAltTimeOffsetS *altTimeMatchEntry = NULL;

   /* Check Input parameters */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_ALT_TIME_KEY_FIELD("zl303xx_PtpTlvAltTimeOffsetEntryDel", keyField);
   }

   /* Get the specified Clock Data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock);
   }

   /* Get a pointer to the entry.  */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpTlvAltTimeOffsetEntrySearch(
                        pPtpClock->altTimeOffset.entryTbl,
                        keyField,
                        &altTimeMatchEntry);

      /* DUPLICATE::  entry exists so update the enable field.
       * NOT_FOUND::  entry does not exist so do nothing.
       * TABLE_FULL:: entry does not exist so do nothing. */
      if (status == ZL303XX_TABLE_ENTRY_DUPLICATE)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpTlvAltTimeOffsetEntryEnable: Record (keyField: %u)(enabled = %u)",
               keyField, enable, 0,0,0,0);

         altTimeMatchEntry->enabled = enable;

         /* Return success; state updated. */
         status = ZL303XX_OK;
      }

      /* ZL303XX_TABLE_ENTRY_NOT_FOUND or ZL303XX_TABLE_FULL */
      else
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpTlvAltTimeOffsetEntryEnable: Entry does not exist (keyField: %u)",
               keyField,0,0,0,0,0);

         /* Either way, the entry does not exist. */
         status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
      }
   }

   return status;
}

/*

  Function Name:
   zl303xx_PtpTlvPathTraceFind

  Details:
   Determines the start of the PATH_TRACE TLV (byte-0 of the tlvType field) if
   one exists. If no such TLV exists, then NULL is returned.

  Parameters:
   [in]  pPtpMessage    Pointer to the received ANNOUNCE message (already
                              parsed into fields).
   [in]  msgBuffer      Pointer to the raw received ANNOUNCE message with
                              unprocessed TLVs still attached.

  Return Value:  Pointer the the start of the PATH_TRACE TLV (if it exists).
         NULL otherwise or if either input pointer is NULL.

 *****************************************************************************/
Uint8T * zl303xx_PtpTlvPathTraceFind(
      zl303xx_PtpV2MsgS *pPtpMessage,
      Uint8T *msgBuffer)
{
   /* Default Return Value */
   Uint8T *ptTlvStart = NULL;

   Sint32T tlvBytesRem;
   Sint32T tlvBytesProcessed;

   /* Pointer to the next TLV block */
   Uint8T *nextTlvBuf = NULL;
   Uint16T tlvType, tlvLength;

   if ((pPtpMessage == NULL) || (msgBuffer == NULL))
   {
      /* Return immediately. */
      return NULL;
   }

   /* A PATH_TRACE TLV can only exist if the ANNOUNCE message is larger than the
    * minimum required by a basic ANNOUNCE message.   */
   tlvBytesRem = pPtpMessage->header.messageLength - ZL303XX_PTP_V2_ANNC_MSG_LEN;
   tlvBytesProcessed = 0;

   while ((tlvBytesRem > 0) &&
          (ptTlvStart == NULL))
   {
      nextTlvBuf = &msgBuffer[ZL303XX_PTP_V2_ANNC_MSG_LEN + tlvBytesProcessed];

      /* Get the Type & Length of this TLV */
      zl303xx_PtpTlvTypeLengthGet(nextTlvBuf, &tlvType, &tlvLength);

      if (tlvType == PTP_TLV_PATH_TRACE)
      {
         ptTlvStart = nextTlvBuf;
      }
      else
      {
         tlvBytesRem -= (2 + 2 + tlvLength);
         tlvBytesProcessed += (2 + 2 + tlvLength);
      }
   }

   return ptTlvStart;
}  /* END zl303xx_PtpTlvPathTraceFind */

/****************   STATIC FUNCTION DEFINITIONS   *****************************/



/****************   END   ****************************************************/
