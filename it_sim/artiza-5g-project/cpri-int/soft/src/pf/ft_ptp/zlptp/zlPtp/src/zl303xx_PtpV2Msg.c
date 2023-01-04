

/*****************************************************************************
* 
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
* Copyright 2006-2018 Microsemi Semiconductor Limited.
* All rights reserved.
*
*  Module Description:
*     This file contains the message packing/unpacking functions for the IEEE-1588 v2 messages
*
*******************************************************************************/

/* see IEEE 1588-2002 spec annex d */

/****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"

#include "zl303xx_Ptpd.h"

#include "zl303xx_PtpV2MsgParse.h"       /* Include all message Parsing definitions. */
#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpV2SignalTlvParse.h"

#include "zl303xx_PtpV2MgmtTlvParse.h"
#include "zl303xx_PtpV2MgmtTlvHandle.h"

#include "zl303xx_PtpV2UnicastDiscovery.h"
#include "zl303xx_PtpProfileDef.h"

/****************   DEFINES     **********************************************/

/****************   DATA TYPES   *********************************************/

/****************   DATA STRUCTURES   ****************************************/

/****************   STATIC FUNCTION DECLARATIONS   ***************************/
_ZL303XX_LOCAL zlStatusE zl303xx_HandleUnicastEnableGet(zl303xx_PortDataS *portData, zl303xx_BooleanE *uniNegEnabled);
_ZL303XX_LOCAL zlStatusE zl303xx_HandleUnicastEnableSet(zl303xx_PortDataS *portData, zl303xx_BooleanE uniNegEnabled);

/* LOCAL functions to gather the Announce message info (header & data) and
   then pack them into IEEE-1588 message format */
/******************************************************************/
static void zl303xx_PtpV2MsgDataAnncGet(zl303xx_PtpClockS *pClock, zl303xx_PtpV2MsgAnnounceS *pAnncData);
static void zl303xx_PtpV2MsgDataAnncOverridesGet(zl303xx_ClockStreamS *pPtpStream, zl303xx_PtpV2MsgAnnounceS *pAnncData);

/****************   STATIC GLOBAL VARIABLES   ********************************/

static const char *errStrInvalidTgtId  = "Invalid targetPortIdentity";
static const char *errStrInvalidAction = "Invalid action value";
static const char *errStrInvalidLength = "Invalid TLV length";



/****************   EXPORTED GLOBAL VARIABLES   ******************************/

/****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* zl303xx_PtpStreamEgressAnncDataGet */
/**
   Routine to query the Announce DATA that will be transmitted by a stream.
   The data is built in the following order:
     1) The default data based on the associated clock.
     2) Data based on the profile being executed by the clock; this will replace
        and data from step (1).
     3) Configured override values (see zl303xx_PtpStreamEgressOverrideAnncFieldSet).
        This will replace any data from steps (1) & (2).

  Parameters:
   [in]  streamHandle   Handle to an existing stream.

   [out] pEgressAnncData   Egress Announce DATA that would be transmitted
                                 from this stream.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle is invalid.
     ZL303XX_INVALID_POINTER       pEgressAnncData is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamEgressAnncDataGet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_PtpV2MsgAnnounceS *pEgressAnncData)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Check the input data pointer. */
   if (status == ZL303XX_OK)
   {
      if (pEgressAnncData == NULL)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpStreamEgressAnncDataGet: Invalid pointer",
               0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
   }

   /* Get the associated stream data structure.
    * (An error is returned if the stream is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream);
   }

   /* Get the Announce data. */
   if (status == ZL303XX_OK)
   {
      /* Gather the Default Announce data section of the message from the
       * associated clock. */
      zl303xx_PtpV2MsgDataAnncGet(pPtpStream->portData->clock, pEgressAnncData);

      /* Insert any Profile specific values. */
      zl303xx_PtpProfileAnncEgressDataGet(pPtpStream, pEgressAnncData);

      /* Replace any Over-ride values. */
      zl303xx_PtpV2MsgDataAnncOverridesGet(pPtpStream, pEgressAnncData);
   }

   return status;
}  /* END zl303xx_PtpStreamEgressAnncDataGet */

/**********************  ANNOUNCE MESSAGES  **********************************/

/* zl303xx_PtpV2MsgAnncPack */
/** 

   Packs an Announce message based on PTP stream and clock information.

  Parameters:
   [in]   pPtpStream    Pointer to the PTP Stream transmitting this message.

   [out]  pAnncMsgBuf   Announce message buffer into which the required
                              announce message will be constructed.
   [out]  pAnncDataS    Optional: If provided, the Announce data structure
                              will be returned.

  Return Value:
     Uint16T  Size in bytes of the packed Announce message (header & data).
     0        An error occurred gathering or packing data.

  Notes:  Assumes pPtpStream and pAnncMsgBuf are already validated.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgAnncPack(
      zl303xx_ClockStreamS *pPtpStream,
      Uint8T *pAnncMsgBuf,
      zl303xx_PtpV2MsgAnnounceS *pAnncDataS)
{
   Uint16T bytesPacked = 0;
   zl303xx_PtpV2MsgAnnounceS lAnncData;  /* Local announce structure. */

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4,
         "zl303xx_PtpV2MsgAnncPack:", 0,0,0,0,0,0);

   /* Pack the common message header. NULL is used to generate sequenceId
    * automatically. */
   bytesPacked = zl303xx_PtpV2MsgHeaderPackForStream(
                                 pPtpStream,
                                 ZL303XX_MSG_ID_ANNOUNCE,
                                 NULL,    /* NULL seqId triggers an automatic insertion. */
                                 pAnncMsgBuf);

   /* Make sure the Header was packed correctly. */
   if (bytesPacked !=ZL303XX_PTP_V2_HEADER_LEN)
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_PtpV2MsgAnncPack: Header Pack ERROR! Size %d",
            bytesPacked, 0,0,0,0,0);
   }

   /* Pack the rest of the message. */
   else
   {
      /* Gather the Default Announce data section of the message from the
       * associated clock. */
      zl303xx_PtpV2MsgDataAnncGet(pPtpStream->portData->clock, &lAnncData);

      /* Insert any Profile specific values. */
      zl303xx_PtpProfileAnncEgressDataGet(pPtpStream, &lAnncData);

      /* Replace any Over-ride values. */
      zl303xx_PtpV2MsgDataAnncOverridesGet(pPtpStream, &lAnncData);

      /* Pack the final Announce data section of the message. */
      /* Note: this routine cannot fail so ignore checking it. */
      bytesPacked += zl303xx_PtpV2MsgDataAnncPack(&lAnncData, pAnncMsgBuf + bytesPacked);
   }

   /* Insert the total length of the message into the packet regardless of
    * success. 0 indicates an error. */
   zl303xx_PtpV2MsgPktLengthSet(bytesPacked, pAnncMsgBuf);

   /* Return the values gathered for the data portion if requested and the
    * operation was successful. */
   if ((pAnncDataS != NULL) &&
       (bytesPacked > ZL303XX_PTP_V2_HEADER_LEN))
   {
      *pAnncDataS = lAnncData;
   }

   return bytesPacked;
}

/**********************  MANAGEMENT MESSAGE HANDLING  *************************/

/*

  Function Name:
    zl303xx_V2HandleManagementTLV

  Details:
   Handles a single management TLV with action get/set

  Parameters:
       msgScope      The scope of the message (refer to the definition of
                           zl303xx_PtpV2MgmtMsgScopeE
       pPtpMsgRx     The PTP received message structure containing the common
                           header and the contents of the management base message
       iBuf           Pointer to the start of the received TLV
       ibufLength     The unprocessed length of the received message
       oBuf           Pointer to the start of a buffer to receive the response TLV.
                  (This points after the standard Management Message data)
       portData       Pointer to the clock port instance structure for the port
                  that received this message. Cannot be NULL.

       addedBytes     The number of response bytes (if any) added by this function

  Return Value:
   the number of input message bytes consumed by this function or zero if there
   was a parsing error

 *****************************************************************************/
Uint16T zl303xx_V2HandleManagementTLV(zl303xx_PtpV2MgmtMsgScopeE msgScope,
                                      zl303xx_PtpV2MsgS *pPtpMsgRx,
                                      Uint8T *iBuf,
                                      Uint16T ibufLength,
                                      Uint8T *oBuf,
                                      zl303xx_PortDataS *portData,
                                      Uint16T *addedBytes)
{
   Uint16T tlvInDataLength;
   Uint16T tlvType;
   Uint16T msgLength = 0;        /* The length of the current TLV */
   Uint16T errorMsgLength = 0;   /* The length of the error message TLV, if an error occurs */
   Uint16T tlvOutDataLength = 0; /* The length of the data in the current TLV */
   Uint8T *pTlvDataOut = NULL;
   Uint8T *pTlvDataIn;

   zl303xx_PtpV2MsgHeaderS *header = &pPtpMsgRx->header;
   zl303xx_PtpV2MsgManagementS *manage = &pPtpMsgRx->data.management;

   /* Check that the buffer contains at least a type and length field */
   if (ibufLength < ZL303XX_PTP_TLV_BASE_LEN)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
            "Port %u: TLV packet too short, Type=%d",
            portData->config.portNumber, header->messageType, 0,0,0,0);

      zl303xx_IncrementMalformedPktCount(portData);
      return 0;  /* Indicate an error to the caller */
   }

   /* Get the type and length fields */
   zl303xx_PtpV2MgmtTlvTypeLengthIdGet(iBuf, &tlvType, &tlvInDataLength, NULL);

   /* Check that the TLV includes at least the required amount of data.
      The total length of the TLV in the buffer includes the type field and the length field */
   if (ibufLength < (tlvInDataLength + ZL303XX_PTP_TLV_VALUE_OFFSET))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
            "Port %u: TLV packet too short",
            portData->config.portNumber, 0,0,0,0,0);

      zl303xx_IncrementMalformedPktCount(portData);
      return 0;  /* Indicate an error to the caller */
   }

   if ((tlvInDataLength % 2) != 0)   /* Length must be an even number */
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
            "Port %u: TLV packet invalid length",
            portData->config.portNumber, 0,0,0,0,0);

      zl303xx_IncrementMalformedPktCount(portData);
      return 0;  /* Indicate an error to the caller */
   }

   /* For these, we look at the port scope. */
   msgScope &= PTPV2_MGMT_SCOPE_ALL_PORTS;

   if (tlvType == PTP_TLV_MANAGEMENT)
   {  /* Only handle messages of type MANAGEMENT */
      Uint16T managementId = 0;
      Uint16T managementTlvInDataLength = 0; /* The length of the data field in a management TLV. This excludes the management ID */

      /* Management TLVs must have at least a management ID, so check the length */
      if (tlvInDataLength < 2)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
               "Port %u: TLV packet too short",
               portData->config.portNumber, 0,0,0,0,0);

         zl303xx_IncrementMalformedPktCount(portData);
         return 0;  /* Indicate an error to the caller */
      }

      /* Find the true length of the data in the management TLV */
      managementTlvInDataLength = tlvInDataLength - 2;

      /* Extract the management ID of the request */
      managementId = UNPACK_16BIT(iBuf + 4);

      /* Start to compose a response TLV. The returned message length is the total length of the TLV */
      msgLength = zl303xx_PtpV2MgmtTlvTypeLengthIdSet(&tlvType, NULL, &managementId, oBuf);

      /* Set up a pointer to the start of the TLV data proper */
      pTlvDataOut = (oBuf + ZL303XX_PTP_V2_MGMT_TLV_HEADER_LEN);
      pTlvDataIn = (iBuf + ZL303XX_PTP_V2_MGMT_TLV_HEADER_LEN);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
            "zl303xx_V2HandleManagementTLV: Management TLV Received:: (tlvType:%d)(mgmtId:%d)(action:%d)",
            tlvType, managementId, manage->actionField, 0,0,0);

      switch (managementId)
      {
         case PTP2_MGMT_ID_NULL :
         case PTP2_MGMT_ID_CLOCK_DESCRIPTION :
         {
            /* Major changes have been made to the Management TLV processing hierarchy.
             * This work is not yet complete but is ongoing.
             * The TLVs in this list have been moved from this MSG module to the
             * more appropriate PtpV2MgmtTlvHandle module.  */
            /* If this code is still being hit, then something is wrong. */
            break;
         }

         case PTP2_MGMT_ID_USER_DESCRIPTION :
         {
            if (msgScope != PTPV2_MGMT_SCOPE_ALL_PORTS)
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_VALUE, managementId,
                                    errStrInvalidTgtId);
            }
            else
            {
               switch (manage->actionField)
               {
                  case PTP_MANAGEMENT_ACTION_SET :
                     /* Check that there is enough data in the received packet firstly to read the text length field
                        and then to read the text itself. */
                     {
                        Uint8T textLength = 0;
                        zl303xx_BooleanE lengthOK = ZL303XX_TRUE;
                        if (managementTlvInDataLength < 1)
                        {
                           lengthOK = ZL303XX_FALSE;
                        }
                        if (lengthOK == ZL303XX_TRUE)
                        {
                           textLength = *(Uint8T*)(pTlvDataIn + 0);
                           /* The total length of the text field in the packet is (textLength + 1) */
                           if ((textLength +1) > ZL303XX_PTP_NODE_USER_DESC_LEN)
                           {
                              lengthOK = ZL303XX_FALSE;
                           }
                        }
                        if (lengthOK == ZL303XX_TRUE)
                        {
                           /* There may be a padding byte at the end of text to make the packet
                              an even number of bytes so round up to the next  */
                           if (managementTlvInDataLength < ((textLength + 1) + ((textLength + 1) % 2)))
                           {
                              lengthOK = ZL303XX_FALSE;
                           }
                        }

                        if (lengthOK == ZL303XX_FALSE)
                        {
                           zl303xx_IncrementMalformedPktCount(portData);
                           errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_LENGTH, managementId,
                                             errStrInvalidLength);   /* Packet is too short */
                           break;
                        }
                        else  /* Length seems OK so now process the data */
                        {
                           char userDescription[ZL303XX_PTP_NODE_USER_DESC_LEN + 1];
                           (void)zl303xx_PtpV2MsgTextUnpack(pTlvDataIn, userDescription, ZL303XX_PTP_NODE_USER_DESC_LEN);

                           zl303xx_PtpNodeDescriptionSet(NULL, NULL, NULL, userDescription);
                        }
                     }
                     /* Fallthrough intentional */
                  case PTP_MANAGEMENT_ACTION_GET :
                     {
                        char *userDescription = NULL;
                        zl303xx_PtpNodeDescriptionGet(NULL, NULL, NULL, &userDescription);

                        /* Store the userDescription string into the response packet. It is possible that the
                           userDescription string is an empty string. In this case we will just return a PTPtext
                           field with length set to zero*/
                        tlvOutDataLength += zl303xx_PtpV2MsgTextPack(userDescription, pTlvDataOut, ZL303XX_PTP_NODE_USER_DESC_LEN);

                        if (tlvOutDataLength % 2)
                        {
                           /* Add a padding byte to make the length an even number */
                           *(Uint8T*)(pTlvDataOut + tlvOutDataLength) = 0;
                           tlvOutDataLength++;
                        }
                     }
                     break;

                  case PTP_MANAGEMENT_ACTION_RSP:
                  case PTP_MANAGEMENT_ACTION_CMD:
                  case PTP_MANAGEMENT_ACTION_ACK:
                  default :
                     /* Invalid request. Return an error response. */
                     errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_NOT_SUPPORTED, managementId,
                                          errStrInvalidAction);
                     break;
               }
            }
            break;
         }

         case PTP2_MGMT_ID_SAVE_IN_NVS :
         case PTP2_MGMT_ID_RESET_NVS :
         {
            errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                       ZL303XX_MGMT_ERR_NOT_SUPPORTED,
                                       managementId,
                                       "Non-volatile storage not supported");
            break;
         }

         case PTP2_MGMT_ID_INITIALIZE :
         {
            /* The standard assumes that if one port is in the INITIALIZING state then all ports must be in the INITIALIZING state.
               This requirement comes from boundary clocks where clearly the whole boundary clock will initialize as one entity.
               However, in our case we have one physical port with pseudo-independent clock streams running through it
               and it suits our purposes, under some circumstances, to allow each clock stream to be reset independently.
               So we support the ability to either re-initialize all clock streams or just a single one, depending on the
               targetPortIdentity field and the msgScope variable.
               It is possible but extremely unlikely that this will cause a problem in the field since if the INITIALIZE management
               message is used in its standard form then it should logically be addressed to all ports anyway which we will handle
               in the standard conformant manner. However, the standard is not rigorous about requiring that the INITIALIZE message
               be addressed to all ports. */
            if ((manage->actionField == PTP_MANAGEMENT_ACTION_CMD) && (managementTlvInDataLength != 2))
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_LENGTH, managementId,
                                    errStrInvalidLength);
            }
            else
            {
               switch (manage->actionField)
               {
                  case PTP_MANAGEMENT_ACTION_CMD :
                     {
                        Uint16T initKey = UNPACK_16BIT(pTlvDataIn);

                        if (initKey == PTP2_INITIALIZATION_KEY_INIT_EVENT)
                        {  /* The initialization key indicates we should perform a re-initialization */
                           zl303xx_PortDataS *requestedPortData;

                           if (msgScope == PTPV2_MGMT_SCOPE_ALL_PORTS)
                           {
                              /* Apply the request to all ports */
                              zl303xx_PtpPortHandleT portId;

                              for (portId = 0;
                                   ZL303XX_CHECK_PORT_HANDLE(portId) == ZL303XX_OK;
                                   portId++)
                              {
                                 if (zl303xx_PtpPortDataGet(portId, &requestedPortData) == ZL303XX_OK)
                                 {
                                    zl303xx_PtpV2ToPortState(requestedPortData, PTP_PORT_INT_STATE_INIT);
                                 }
                              }
                           }
                           else
                           {
                              /* Apply to the specific port only */
                              ZL303XX_TRACE_ALWAYS("Executing non-standard single port initialization. Requested port %d",
                                    manage->targetPortIdentity.portNumber, 0,0,0,0,0);

                              (void)zl303xx_PtpPortDataFromPortIdentity(&manage->targetPortIdentity, &requestedPortData);
                              if (requestedPortData == NULL)
                              {  /* No matching port found. Return a management error TLV */
                                 errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_VALUE, managementId,
                                                      "INITIALIZE command: invalid port number");
                              }
                              else
                              {
                                 zl303xx_PtpV2ToPortState(requestedPortData, PTP_PORT_INT_STATE_INIT);
                              }
                           }
                        }
                        else
                        {  /* A different init key which we do not support*/
                           errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_NOT_SUPPORTED, managementId,
                                          "Unsupported initialization key value");
                        }
                     }
                     break;

                  case PTP_MANAGEMENT_ACTION_ACK:
                  case PTP_MANAGEMENT_ACTION_RSP:
                  case PTP_MANAGEMENT_ACTION_SET:
                  case PTP_MANAGEMENT_ACTION_GET:
                  default :
                     /* Invalid request. Return an error response. */
                     errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_NOT_SUPPORTED, managementId,
                                          errStrInvalidAction);
                     break;
               }
            }
            break;
         }

         case PTP2_MGMT_ID_FAULT_LOG :
         case PTP2_MGMT_ID_FAULT_LOG_RESET :
         {
            /* These operations are not currently supported in this node */
            errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                       ZL303XX_MGMT_ERR_NOT_SUPPORTED,
                                       managementId,
                                       "Fault log operations not supported");
            break;
         }

         /* The following IDs apply to the DEFAULT_DATA_SET or one of its
          * members. These all end up at the same internal function call so
          * handle them together. */
         case PTP2_MGMT_ID_DEFAULT_DATA_SET :
         case PTP2_MGMT_ID_PRIORITY1 :
         case PTP2_MGMT_ID_PRIORITY2 :
         case PTP2_MGMT_ID_DOMAIN :
         case PTP2_MGMT_ID_SLAVE_ONLY :
         case PTP2_MGMT_ID_CLOCK_ACCURACY :
         {
            tlvOutDataLength = zl303xx_PtpMgmtTlvHandleDefaultDSMembers(
                                       portData->clock,              /* Target Clock */
                                       manage->actionField,               /* GET or SET for these TLVs */
                                       managementId,                 /* TLV Id */
                                       pTlvDataIn,                   /* Rx TLV data (no TLV header) */
                                       managementTlvInDataLength,    /* Rx TLV data length */
                                       oBuf,                         /* Tx TLV buffer (TLV header + data) */
                                       &errorMsgLength);             /* Output error length (if generated) */

            break;
         }

         case PTP2_MGMT_ID_CURRENT_DATA_SET :
         {
            /* This management message operates on the clock itself so should be addressed to all ports */
            if (msgScope != PTPV2_MGMT_SCOPE_ALL_PORTS)
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_VALUE, managementId,
                                    errStrInvalidTgtId);
            }
            else if ((manage->actionField == PTP_MANAGEMENT_ACTION_GET) && (managementTlvInDataLength != 0))
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_LENGTH, managementId,
                                    errStrInvalidLength);
            }
            else
            {
               switch (manage->actionField)
               {
                  case PTP_MANAGEMENT_ACTION_GET :
                  {
                     zl303xx_TimeInterval timeInterval;
                     zl303xx_CurrentDSInternal *currentDS = &portData->clock->currentDSInt;

                     /* stepsRemoved */
                     PACK_16BIT((pTlvDataOut + 0), currentDS->stepsRemoved);

                     /* offset_from_master */
                     /* Convert to a time interval value */
                     zl303xx_PtpConvTimeStampToTimeInterval(&currentDS->offset_from_master, &timeInterval);
                     PACK_32BIT((pTlvDataOut + 2), timeInterval.scaledNanoseconds.hi);
                     PACK_32BIT((pTlvDataOut + 6), timeInterval.scaledNanoseconds.lo);

                     /* mean_path_delay */
                     /* Convert to a time interval value */
                     zl303xx_PtpConvTimeStampToTimeInterval(&currentDS->mean_path_delay, &timeInterval);
                     PACK_32BIT((pTlvDataOut + 10), timeInterval.scaledNanoseconds.hi);
                     PACK_32BIT((pTlvDataOut + 14), timeInterval.scaledNanoseconds.lo);

                     tlvOutDataLength = 18;

                     break;
                  }

                  case PTP_MANAGEMENT_ACTION_ACK:
                  case PTP_MANAGEMENT_ACTION_RSP:
                  case PTP_MANAGEMENT_ACTION_SET:
                  case PTP_MANAGEMENT_ACTION_CMD:
                  default :
                  {
                     /* Invalid request. Return an error response. */
                     errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_NOT_SUPPORTED, managementId,
                                          errStrInvalidAction);
                     break;
                  }
               }
            }
            break;
         }

         case PTP2_MGMT_ID_PARENT_DATA_SET :
         {
            /* This management message operates on the clock itself so should be addressed to all ports */
            if (msgScope != PTPV2_MGMT_SCOPE_ALL_PORTS)
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_VALUE, managementId,
                                    errStrInvalidTgtId);
            }
            else if ((manage->actionField == PTP_MANAGEMENT_ACTION_GET) && (managementTlvInDataLength != 0))
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_LENGTH, managementId,
                                    errStrInvalidLength);
            }
            else
            {
               switch (manage->actionField)
               {
                  case PTP_MANAGEMENT_ACTION_GET :
                  {
                     tlvOutDataLength = zl303xx_PtpV2MgmtTlvParentDSPack(&portData->clock->parentDS, pTlvDataOut);
                     break;
                  }

                  case PTP_MANAGEMENT_ACTION_ACK:
                  case PTP_MANAGEMENT_ACTION_RSP:
                  case PTP_MANAGEMENT_ACTION_SET:
                  case PTP_MANAGEMENT_ACTION_CMD:
                  default :
                     /* Invalid request. Return an error response. */
                     errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_NOT_SUPPORTED, managementId,
                                          errStrInvalidAction);
                     break;
               }
            }
            break;
         }

         case PTP2_MGMT_ID_TIME_PROPERTIES_DATA_SET :
         {
            /* This management message operates on the clock itself so should be addressed to all ports */
            if (msgScope != PTPV2_MGMT_SCOPE_ALL_PORTS)
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_VALUE, managementId,
                                    errStrInvalidTgtId);
            }
            else if ((manage->actionField == PTP_MANAGEMENT_ACTION_GET) && (managementTlvInDataLength != 0))
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_LENGTH, managementId,
                                    errStrInvalidLength);
            }
            else
            {
               switch (manage->actionField)
               {
                  case PTP_MANAGEMENT_ACTION_GET :
                     {
                        Uint8T flagBits = 0;
                        zl303xx_PtpClockS *pPtpClock = portData->clock;

                        PACK_16BIT((pTlvDataOut + 0), pPtpClock->timePropertiesDS.currentUtcOffset);

                        flagBits = (Uint8T)((pPtpClock->timePropertiesDS.leap59 & 0x01)                       << 0 |
                                            (pPtpClock->timePropertiesDS.leap61 & 0x01)                       << 1 |
                                            (pPtpClock->timePropertiesDS.frequencyTraceable & 0x01)      << 2 |
                                            (pPtpClock->timePropertiesDS.timeTraceable & 0x01)           << 3 |
                                            (pPtpClock->timePropertiesDS.ptpTimescale & 0x01)            << 4 |
                                            (pPtpClock->timePropertiesDS.currentUtcOffsetValid & 0x01) << 5);
                        *(Uint8T*)(pTlvDataOut + 2) = (Uint8T)flagBits;
                        *(Uint8T*)(pTlvDataOut + 3) = (Uint8T)pPtpClock->timePropertiesDS.timeSource;
                        tlvOutDataLength = 4;
                     }
                     break;

                  case PTP_MANAGEMENT_ACTION_ACK:
                  case PTP_MANAGEMENT_ACTION_RSP:
                  case PTP_MANAGEMENT_ACTION_SET:
                  case PTP_MANAGEMENT_ACTION_CMD:
                  default :
                     /* Invalid request. Return an error response. */
                     errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_NOT_SUPPORTED, managementId,
                                          errStrInvalidAction);
                     break;
               }
            }
            break;
         }

         case PTP2_MGMT_ID_PORT_DATA_SET :
         {
            if ((manage->actionField == PTP_MANAGEMENT_ACTION_GET) &&
                (managementTlvInDataLength != 0))
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                       ZL303XX_MGMT_ERR_WRONG_LENGTH,
                                       managementId,
                                       errStrInvalidLength);
            }
            else
            {
               switch (manage->actionField)
               {
                  case PTP_MANAGEMENT_ACTION_GET :
                  {
                     zl303xx_PortDataS *requestedPortData;

                     /* We only have 1 port but the TLV may be requesting ALL.
                     * Our ALL is port 1...  */
                     if ((manage->targetPortIdentity.portNumber == 0) ||
                         (manage->targetPortIdentity.portNumber == (Uint16T)(-1)))
                     {
                        manage->targetPortIdentity.portNumber = 1;
                     }

                     /* Identify the port to which the management message is targeted */
                     (void)zl303xx_PtpPortDataFromPortIdentity(&manage->targetPortIdentity, &requestedPortData);

                     if (requestedPortData == NULL)
                     {  /* No matching port found. Return a management error TLV */
                        errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_VALUE, managementId,
                                             "GET port data set error: invalid port number");
                     }
                     else if ((requestedPortData->mcastStream == NULL))
                     {  /* There is no Multicast stream. Return a management error TLV */
                        errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_NOT_SUPPORTED, managementId,
                                             "GET port data set error: multicast not enabled, port data set invalid");
                     }
                     else
                     {
                        zl303xx_PtpV2MsgPortIdentityPack(&requestedPortData->portDS.portIdentity, (oBuf + 0));
                        *(Uint8T*)(pTlvDataOut + 10) = zl303xx_GetV2PortState(requestedPortData);
                        *(Uint8T*)(pTlvDataOut + 11) = (Uint8T)(requestedPortData->mcastStream->config.logMinDelayReqInterval);

                        PACK_32BIT((pTlvDataOut + 12), requestedPortData->mcastStream->u.v2.peer_mean_path_delay.scaledNanoseconds.hi);
                        PACK_32BIT((pTlvDataOut + 16), requestedPortData->mcastStream->u.v2.peer_mean_path_delay.scaledNanoseconds.lo);

                        *(Uint8T*)(pTlvDataOut + 20) = (Uint8T)(requestedPortData->mcastStream->config.logAnnounceInterval);
                        *(Uint8T*)(pTlvDataOut + 21) = (Uint8T)(requestedPortData->mcastStream->config.announceReceiptTimeout);
                        *(Uint8T*)(pTlvDataOut + 22) = (Uint8T)(requestedPortData->mcastStream->config.logSyncInterval);
                        *(Uint8T*)(pTlvDataOut + 23) = (Uint8T)(requestedPortData->config.delayMechanism);

                        *(Uint8T*)(pTlvDataOut + 24) = (Uint8T)(requestedPortData->mcastStream->u.v2.log_min_mean_pdelay_req_interval);

                        /* For the version number we return the overall version number of the clock since all the ports
                           must support the same version number in this implementation */
                        *(Uint8T*)(pTlvDataOut + 25) = (Uint8T)(requestedPortData->portDS.versionNumber & 0x0F);
                        tlvOutDataLength = 26;
                     }

                     break;
                  }

                  case PTP_MANAGEMENT_ACTION_ACK:
                  case PTP_MANAGEMENT_ACTION_RSP:
                  case PTP_MANAGEMENT_ACTION_SET:
                  case PTP_MANAGEMENT_ACTION_CMD:
                  default :
                  {
                     /* Invalid request. Return an error response. */
                     errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                             ZL303XX_MGMT_ERR_NOT_SUPPORTED,
                                             managementId,
                                             errStrInvalidAction);
                     break;
                  }
               }
            }
            break;
         }

         case PTP2_MGMT_ID_LOG_ANNOUNCE_INTERVAL :
         case PTP2_MGMT_ID_ANNOUNCE_RECEIPT_TIMEOUT :
         case PTP2_MGMT_ID_LOG_SYNC_INTERVAL :
         {
            /* These management messages have the following properties in common:
             * - They operate on a single variable which is of type Uint8T,
             * - The format of the received management message and its response is identical in each case.
             * - As a SET operation these management message could operate on one or more ports in the current
                     clock but as a GET operation they only operate on a specified port.
               Therefore their processing is performed through a common block. Which is an attempt to reduce code duplication
               which may or may not turn out to be a good thing.
               Note that in the future, if we implement fuller range checking on SET operations then this is likely to be
               different for each variable.
               Note also that although the variable may be updated in response to a SET operation the timers are not immediately
               restarted with the new value so it may not take effect until then end of the current interval. */
            Uint16T strId = managementId - PTP2_MGMT_ID_LOG_ANNOUNCE_INTERVAL;
            const char *varNameStr[] = {"logAnnounceInterval",
                                        "announceReceiptTimeout",
                                        "logSyncInterval"};

            if ((manage->actionField == PTP_MANAGEMENT_ACTION_GET) && (msgScope != PTPV2_MGMT_SCOPE_SINGLE_PORT))
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_VALUE, managementId,
                                    errStrInvalidTgtId);
            }
            else if (((manage->actionField == PTP_MANAGEMENT_ACTION_GET) && (managementTlvInDataLength != 0)) ||
                     ((manage->actionField == PTP_MANAGEMENT_ACTION_SET) && (managementTlvInDataLength != 2)))
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_LENGTH, managementId,
                                    errStrInvalidLength);
            }
            else
            {
               zl303xx_PortDataS *requestedPortData = NULL;
               Uint8T dataValue = 0;

               /* Get the port data to operate on. Can only perform the operation if multicast is enabled */
               (void)zl303xx_PtpPortDataFromPortIdentity(&manage->targetPortIdentity, &requestedPortData);
               if ((requestedPortData) &&
                   (requestedPortData->mcastStream == NULL))
               {  /* There is no Multicast stream. Return a management error TLV */
                  errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_NOT_SUPPORTED, managementId,
                                       "Port data set member error: multicast not enabled, port data set invalid");
               }
               else
               {
                  switch (manage->actionField)
                  {
                     case PTP_MANAGEMENT_ACTION_SET :
                        dataValue = *(Uint8T *)(pTlvDataIn + 0);

                        if (msgScope == PTPV2_MGMT_SCOPE_ALL_PORTS)
                        {  /* Apply the request to all ports */
                           zl303xx_PtpPortHandleT portId;

                           for (portId = 0;
                                ZL303XX_CHECK_PORT_HANDLE(portId) == ZL303XX_OK;
                                portId++)
                           {
                              (void)zl303xx_PtpPortDataGet(portId, &requestedPortData);
                              if (requestedPortData)
                              {
                                 if (managementId == PTP2_MGMT_ID_LOG_ANNOUNCE_INTERVAL)
                                    requestedPortData->mcastStream->config.logAnnounceInterval = dataValue;
                                 else if (managementId == PTP2_MGMT_ID_ANNOUNCE_RECEIPT_TIMEOUT)
                                    requestedPortData->mcastStream->config.announceReceiptTimeout = dataValue;
                                 else /* (managementId == PTP2_MGMT_ID_LOG_SYNC_INTERVAL) */
                                    requestedPortData->mcastStream->config.logSyncInterval = dataValue;
                              }
                           }

                           ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV,3,
                                 "Setting %s for all ports to %d",
                                 varNameStr[strId],
                                 (Sint8T)dataValue, 0,0,0,0);
                        }
                        else
                        {  /* Apply to the specific port only */
                           (void)zl303xx_PtpPortDataFromPortIdentity(&manage->targetPortIdentity, &requestedPortData);
                           if (requestedPortData == NULL)
                           {  /* No matching port found. Return a management error TLV */
                              errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_VALUE, managementId,
                                                   errStrInvalidTgtId);
                           }
                           else
                           {
                              if (managementId == PTP2_MGMT_ID_LOG_ANNOUNCE_INTERVAL)
                                 requestedPortData->mcastStream->config.logAnnounceInterval = dataValue;
                              else if (managementId == PTP2_MGMT_ID_ANNOUNCE_RECEIPT_TIMEOUT)
                                 requestedPortData->mcastStream->config.announceReceiptTimeout = dataValue;
                              else /* (managementId == PTP2_MGMT_ID_LOG_SYNC_INTERVAL) */
                                 requestedPortData->mcastStream->config.logSyncInterval = dataValue;

                              ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV,3,
                                    "Setting %s for port %u to %u",
                                    varNameStr[strId],
                                    requestedPortData->config.portNumber,
                                    dataValue, 0,0,0);
                           }
                        }
                        /* Now respond to the SET operation with a GET operation. Only respond once with the value set
                           even if it was set in all ports */
                        *(Uint8T *)(pTlvDataOut + 0) = dataValue;
                        *(Uint8T *)(pTlvDataOut + 1) = 0;    /* reserved */
                        tlvOutDataLength = 2;
                        break;

                     case PTP_MANAGEMENT_ACTION_GET :
                        /* Return the current value */
                        {  /* Apply to the specified port only */
                           (void)zl303xx_PtpPortDataFromPortIdentity(&manage->targetPortIdentity, &requestedPortData);
                           if (requestedPortData == NULL)
                           {  /* No matching port found. Return a management error TLV */
                              errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_VALUE, managementId,
                                                   errStrInvalidTgtId);
                           }
                           else
                           {
                              if (managementId == PTP2_MGMT_ID_LOG_ANNOUNCE_INTERVAL)
                                 dataValue = requestedPortData->mcastStream->config.logAnnounceInterval;
                              else if (managementId == PTP2_MGMT_ID_ANNOUNCE_RECEIPT_TIMEOUT)
                                 dataValue = requestedPortData->mcastStream->config.announceReceiptTimeout;
                              else /* (managementId == PTP2_MGMT_ID_LOG_SYNC_INTERVAL) */
                                 dataValue = requestedPortData->mcastStream->config.logSyncInterval;

                              *(Uint8T *)(pTlvDataOut + 0) = dataValue;
                              *(Uint8T *)(pTlvDataOut + 1) = 0;    /* reserved */
                              tlvOutDataLength = 2;
                           }
                        }
                        break;

                     case PTP_MANAGEMENT_ACTION_ACK:
                     case PTP_MANAGEMENT_ACTION_RSP:
                     case PTP_MANAGEMENT_ACTION_CMD:
                     default :
                        /* Invalid request. Return an error response. */
                        errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_NOT_SUPPORTED, managementId,
                                             errStrInvalidAction);
                        break;
                  }
               }
            }

            break;
         }

         case PTP2_MGMT_ID_VERSION_NUMBER :
         {
            /* Only support the GET operation since this clock does not allow setting the version number */
            if ((manage->actionField == PTP_MANAGEMENT_ACTION_GET) && (managementTlvInDataLength != 0))
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_LENGTH, managementId,
                                    errStrInvalidLength);
            }
            else
            {
               switch (manage->actionField)
               {
                  case PTP_MANAGEMENT_ACTION_SET :
                     errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack_SetFailed(oBuf, managementId);
                     break;

                  case PTP_MANAGEMENT_ACTION_GET :
                  {
                     zl303xx_PortDataS *requestedPortData;

                     /* We only have 1 port but the TLV may be requesting ALL.
                      * Our ALL is port 1...  */
                     if ((manage->targetPortIdentity.portNumber == 0) ||
                         (manage->targetPortIdentity.portNumber == (Uint16T)(-1)))
                     {
                        manage->targetPortIdentity.portNumber = 1;
                     }

                     /* Identify the port to which the management message is targeted */
                     (void)zl303xx_PtpPortDataFromPortIdentity(&manage->targetPortIdentity, &requestedPortData);

                     /* Always return the version number of the overall clock regardless of which port is requested */
                     {  /* Get the value for the specified port */
                        *(Uint8T *)(pTlvDataOut + 0) = (Uint8T)(requestedPortData->portDS.versionNumber & 0x0F);
                        *(Uint8T *)(pTlvDataOut + 1) = 0;    /* reserved */
                        tlvOutDataLength = 2;
                     }
                     break;
                  }

                  case PTP_MANAGEMENT_ACTION_ACK:
                  case PTP_MANAGEMENT_ACTION_RSP:
                  case PTP_MANAGEMENT_ACTION_CMD:
                  default :
                     /* Invalid request. Return an error response. */
                     errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_NOT_SUPPORTED, managementId,
                                          errStrInvalidAction);
                     break;
               }
            }
            break;
         }

         case PTP2_MGMT_ID_ENABLE_PORT :
         {
            if (managementTlvInDataLength != 0)
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_LENGTH, managementId,
                                    "Invalid data length");
            }
            else
            {
               zl303xx_PortDataS *requestedPortData = NULL;
               switch (manage->actionField)
               {
                  case PTP_MANAGEMENT_ACTION_CMD :
                     if (msgScope == PTPV2_MGMT_SCOPE_ALL_PORTS)
                     {  /* Apply the request to all ports */
                        zl303xx_PtpPortHandleT portId;

                        for (portId = 0;
                             ZL303XX_CHECK_PORT_HANDLE(portId) == ZL303XX_OK;
                             portId++)
                        {
                           (void)zl303xx_PtpPortDataGet(portId, &requestedPortData);
                           if (requestedPortData)
                           {
                              zl303xx_PtpV2ToPortState(requestedPortData, PTP_PORT_INT_STATE_INIT);
                           }
                        }
                     }
                     else
                     {  /* Apply to the specific port only */
                        (void)zl303xx_PtpPortDataFromPortIdentity(&manage->targetPortIdentity, &requestedPortData);
                        if (requestedPortData == NULL)
                        {  /* No matching port found. Return a management error TLV */
                           errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_VALUE, managementId,
                                                "ENABLE command: invalid port number");
                        }
                        else
                        {
                           zl303xx_PtpV2ToPortState(requestedPortData, PTP_PORT_INT_STATE_INIT);
                        }
                     }
                     break;

                  case PTP_MANAGEMENT_ACTION_RSP:
                  case PTP_MANAGEMENT_ACTION_ACK:
                  case PTP_MANAGEMENT_ACTION_SET:
                  case PTP_MANAGEMENT_ACTION_GET:
                  default :
                     errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_NOT_SUPPORTED, managementId,
                                          errStrInvalidAction);
                     break;
               }
            }
            break;
         }

         case PTP2_MGMT_ID_DISABLE_PORT :
         {
            if (managementTlvInDataLength != 0)
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_LENGTH, managementId,
                                    "Invalid data length");
            }
            else
            {
               zl303xx_PortDataS *requestedPortData = NULL;
               switch (manage->actionField)
               {
                  case PTP_MANAGEMENT_ACTION_CMD :
                     if (msgScope == PTPV2_MGMT_SCOPE_ALL_PORTS)
                     {  /* Apply the request to all ports */
                        zl303xx_PtpPortHandleT portId;

                        for (portId = 0;
                             ZL303XX_CHECK_PORT_HANDLE(portId) == ZL303XX_OK;
                             portId++)
                        {
                           (void)zl303xx_PtpPortDataGet(portId, &requestedPortData);
                           if (requestedPortData)
                           {
                              zl303xx_PtpV2ToPortState(requestedPortData, PTP_PORT_INT_STATE_DISABLED);
                           }
                        }
                     }
                     else
                     {  /* Apply to the specific port only */
                        (void)zl303xx_PtpPortDataFromPortIdentity(&manage->targetPortIdentity, &requestedPortData);
                        if (requestedPortData == NULL)
                        {  /* No matching port found. Return a management error TLV */
                           errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_VALUE, managementId,
                                                "DISABLE command: invalid port number");
                        }
                        else
                        {
                           zl303xx_PtpV2ToPortState(requestedPortData, PTP_PORT_INT_STATE_DISABLED);
                        }
                     }
                     break;

                  case PTP_MANAGEMENT_ACTION_ACK:
                  case PTP_MANAGEMENT_ACTION_RSP:
                  case PTP_MANAGEMENT_ACTION_SET:
                  case PTP_MANAGEMENT_ACTION_GET:
                  default :
                     errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_NOT_SUPPORTED, managementId,
                                          errStrInvalidAction);
                     break;
               }
            }
            break;
         }

         case PTP2_MGMT_ID_TIME :
         {
            /* Check the message is correctly addressed */
            if (msgScope != PTPV2_MGMT_SCOPE_ALL_PORTS)
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_VALUE, managementId,
                                    "Invalid port value");
            }
            else if (managementTlvInDataLength != 10)
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_LENGTH, managementId,
                                    "Invalid data length");
            }
            else
            {
               zl303xx_TimeStamp todTime;

               switch (manage->actionField)
               {
                  case PTP_MANAGEMENT_ACTION_SET :
                     /* The standard recommends that if an attempt is made to set the time
                      * on a non-grandmaster clock that an error TLV should be returned.
                      * However, knowing if the clock is actually a grandmaster is tricky
                      * because it depends, among other things, as to whether the clock has
                      * a local external reference. Here we use a simplified condition which
                      * we know will not work in all cases but which may be adequate in normal use. */
                     if (zl303xx_PtpClockHasSlavePorts(portData->clock) == ZL303XX_TRUE)
                     {
                        /* Device has at least one slave port so it is probably unwise to set the time */
                        errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_NOT_SUPPORTED, managementId,
                                          "Clock has ports in slave mode");
                     }
                     else
                     {  /* Go ahead and set the time.  */

                        zl303xx_PtpV2MsgTimestampUnpack(pTlvDataIn, &todTime);

                        /* The nanoseconds value is ignored as the sub-seconds
                         * is determined by the position of the 1Hz reference */
                        if (zl303xx_PtpClockTimeSet(portData->clock, &todTime) != 0)
                        {
                           /* Time could not be set */
                           errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(
                                               oBuf, ZL303XX_MGMT_ERR_WRONG_VALUE,
                                               managementId,
                                               "Time could not be set");
                           break;
                        }
                     }

                     /* Fallthrough */
                  case PTP_MANAGEMENT_ACTION_GET :
                     /* Return the current value that has been set */
                     {
                        (void)zl303xx_PtpClockTimeGet(portData->clock, ZL303XX_PTP_INTERFACE_GENERAL, &todTime);
                        zl303xx_PtpV2MsgTimestampPack(&todTime, pTlvDataOut);
                        tlvOutDataLength = 10;
                     }
                     break;

                  case PTP_MANAGEMENT_ACTION_ACK:
                  case PTP_MANAGEMENT_ACTION_RSP:
                  case PTP_MANAGEMENT_ACTION_CMD:
                  default :
                     errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(
                                         oBuf,
                                         ZL303XX_MGMT_ERR_NOT_SUPPORTED,
                                         managementId,
                                         errStrInvalidAction);
                     break;
               }
            }
            break;
         }

         case PTP2_MGMT_ID_UTC_PROPERTIES :
         {
            /* This management message operates on the clock itself so should be addressed to all ports */
            if (msgScope != PTPV2_MGMT_SCOPE_ALL_PORTS)
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_VALUE, managementId,
                                    errStrInvalidTgtId);
            }
            else if (((manage->actionField == PTP_MANAGEMENT_ACTION_GET) && (managementTlvInDataLength != 0)) ||
                     ((manage->actionField == PTP_MANAGEMENT_ACTION_SET) && (managementTlvInDataLength != 4)))
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_LENGTH, managementId,
                                    errStrInvalidLength);
            }
            else
            {
               Uint8T flagBits;
               zl303xx_PtpClockS *pPtpClock = portData->clock;

               switch (manage->actionField)
               {
                  case PTP_MANAGEMENT_ACTION_SET :
                     pPtpClock->timePropertiesDS.currentUtcOffset = UNPACK_16BIT(pTlvDataIn + 0);

                     flagBits = *(Uint8T*)(pTlvDataIn + 2);
                     pPtpClock->timePropertiesDS.leap59  = (zl303xx_BooleanE)((flagBits >> 0) & 0x01);
                     pPtpClock->timePropertiesDS.leap61  = (zl303xx_BooleanE)((flagBits >> 1) & 0x01);
                     pPtpClock->timePropertiesDS.currentUtcOffsetValid  = (zl303xx_BooleanE)((flagBits >> 2) & 0x01);
                    /* (pTlvDataIn + 3) is reserved */
                    /* fall through */
                  case PTP_MANAGEMENT_ACTION_GET :
                     PACK_16BIT((pTlvDataOut + 0), pPtpClock->timePropertiesDS.currentUtcOffset);

                     flagBits = (Uint8T)((pPtpClock->timePropertiesDS.leap59 & 0x01)                       << 0 |
                                         (pPtpClock->timePropertiesDS.leap61 & 0x01)                       << 1 |
                                         (pPtpClock->timePropertiesDS.currentUtcOffsetValid & 0x01) << 2);
                     *(Uint8T*)(pTlvDataOut + 2) = (Uint8T)flagBits;
                     *(Uint8T*)(pTlvDataOut + 3) = 0; /* reserved */
                     tlvOutDataLength = 4;
                     break;

                  case PTP_MANAGEMENT_ACTION_ACK:
                  case PTP_MANAGEMENT_ACTION_RSP:
                  case PTP_MANAGEMENT_ACTION_CMD:
                  default :
                     /* Invalid request. Return an error response. */
                     errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_NOT_SUPPORTED, managementId,
                                          errStrInvalidAction);
                     break;
               }
            }
            break;
         }

         case PTP2_MGMT_ID_TRACEABILITY_PROPERTIES :
         {
            /* This management message operates on the clock itself so should be addressed to all ports */
            if (msgScope != PTPV2_MGMT_SCOPE_ALL_PORTS)
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_VALUE, managementId,
                                    errStrInvalidTgtId);
            }
            else if (((manage->actionField == PTP_MANAGEMENT_ACTION_GET) && (managementTlvInDataLength != 0)) ||
                     ((manage->actionField == PTP_MANAGEMENT_ACTION_SET) && (managementTlvInDataLength != 2)))
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_LENGTH, managementId,
                                    errStrInvalidLength);
            }
            else
            {
               Uint8T flagBits;
               zl303xx_PtpClockS *pPtpClock = portData->clock;

               switch (manage->actionField)
               {
                  case PTP_MANAGEMENT_ACTION_SET :
                     flagBits = *(Uint8T*)(pTlvDataIn + 0);
                     pPtpClock->timePropertiesDS.frequencyTraceable  = (zl303xx_BooleanE)((flagBits >> 2) & 0x01);
                     pPtpClock->timePropertiesDS.timeTraceable  = (zl303xx_BooleanE)((flagBits >> 3) & 0x01);
                     /* (pTlvDataIn + 1) is reserved padding */
                    /* fallthrough */
                  case PTP_MANAGEMENT_ACTION_GET :
                     flagBits = (Uint8T)((pPtpClock->timePropertiesDS.frequencyTraceable & 0x01) << 2 |
                                         (pPtpClock->timePropertiesDS.timeTraceable & 0x01)      << 3);
                     *(Uint8T*)(pTlvDataOut + 0) = (Uint8T)flagBits;
                     *(Uint8T*)(pTlvDataOut + 1) = 0; /* reserved */
                     tlvOutDataLength = 2;
                     break;

                  case PTP_MANAGEMENT_ACTION_ACK:
                  case PTP_MANAGEMENT_ACTION_RSP:
                  case PTP_MANAGEMENT_ACTION_CMD:
                  default :
                     /* Invalid request. Return an error response. */
                     errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_NOT_SUPPORTED, managementId,
                                          errStrInvalidAction);
                     break;
               }
            }
            break;
         }

         case PTP2_MGMT_ID_TIMESCALE_PROPERTIES :
         {
            /* This management message operates on the clock itself so should be addressed to all ports */
            if (msgScope != PTPV2_MGMT_SCOPE_ALL_PORTS)
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_VALUE, managementId,
                                    errStrInvalidTgtId);
            }
            else if (((manage->actionField == PTP_MANAGEMENT_ACTION_GET) && (managementTlvInDataLength != 0)) ||
                     ((manage->actionField == PTP_MANAGEMENT_ACTION_SET) && (managementTlvInDataLength != 2)))
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_LENGTH, managementId,
                                    errStrInvalidLength);
            }
            else
            {
               Uint8T flagBits;
               zl303xx_PtpClockS *pPtpClock = portData->clock;

               switch (manage->actionField)
               {
                  case PTP_MANAGEMENT_ACTION_SET :
                     flagBits = *(Uint8T*)(pTlvDataIn + 0);
                     pPtpClock->timePropertiesDS.ptpTimescale  = (zl303xx_BooleanE)((flagBits >> 4) & 0x01);
                     pPtpClock->timePropertiesDS.timeSource = *(Uint8T*)(pTlvDataIn + 1);
                    /* fall-through */
                  case PTP_MANAGEMENT_ACTION_GET :
                     flagBits = (Uint8T)((pPtpClock->timePropertiesDS.ptpTimescale & 0x01) << 4);
                     *(Uint8T*)(pTlvDataOut + 0) = (Uint8T)flagBits;
                     *(Uint8T*)(pTlvDataOut + 1) = pPtpClock->timePropertiesDS.timeSource;
                     tlvOutDataLength = 2;
                     break;

                  case PTP_MANAGEMENT_ACTION_ACK:
                  case PTP_MANAGEMENT_ACTION_RSP:
                  case PTP_MANAGEMENT_ACTION_CMD:
                  default :
                     /* Invalid request. Return an error response. */
                     errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_NOT_SUPPORTED, managementId,
                                          errStrInvalidAction);
                     break;
               }
            }
            break;
         }

         case PTP2_MGMT_ID_UNICAST_NEGOTIATION_ENABLE :
         {
            if (((manage->actionField == PTP_MANAGEMENT_ACTION_GET) && (managementTlvInDataLength != 0)) ||
                ((manage->actionField == PTP_MANAGEMENT_ACTION_SET) && (managementTlvInDataLength != 2)))
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_LENGTH, managementId,
                                    errStrInvalidLength);
            }
            else
            {
               zl303xx_BooleanE uniTlv;
               zlStatusE status = ZL303XX_OK;

               switch (manage->actionField)    /* Not fully implemented. */
               {
                  case PTP_MANAGEMENT_ACTION_SET :
                  {
                     /* Check if the request is a change in the current setting */
                     if (0 != zl303xx_PtpV2MgmtTlvUnicastNegEnUnpack(pTlvDataIn, &uniTlv))
                     {
                        /* Trigger any state changes */
                        status = zl303xx_HandleUnicastEnableSet(portData, uniTlv);
                     }
                     else
                     {
                        status = ZL303XX_ERROR;
                     }
                  }

                  /* NO BREAK
                     Fall-through since a SET operation requires a response */
                  case PTP_MANAGEMENT_ACTION_GET :
                  {
                     /* Evaluate whether Unicast Negotiation is actually disabled */
                     if (status == ZL303XX_OK)
                     {
                        status = zl303xx_HandleUnicastEnableGet(portData, &uniTlv);
                     }

                     /* Pack the response to every SET or GET request */
                     if (status == ZL303XX_OK)
                     {
                        tlvOutDataLength = zl303xx_PtpV2MgmtTlvUnicastNegEnPack(&uniTlv, pTlvDataOut);
                     }

                     break;
                  }

                  case PTP_MANAGEMENT_ACTION_ACK:
                  case PTP_MANAGEMENT_ACTION_RSP:
                  case PTP_MANAGEMENT_ACTION_CMD:
                  default :
                  {
                     /* Invalid request. Return an error response. */
                     errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                          ZL303XX_MGMT_ERR_NOT_SUPPORTED,
                                          managementId,
                                          errStrInvalidAction);
                     break;
                  }
               }
            }

            break;
         }

         case PTP2_MGMT_ID_PATH_TRACE_LIST :
         case PTP2_MGMT_ID_PATH_TRACE_ENABLE :
         {
            errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                       ZL303XX_MGMT_ERR_NOT_SUPPORTED,
                                       managementId,
                                       "Path trace option not supported");
            break;
         }

         case PTP2_MGMT_GRANDMASTER_CLUSTER_TABLE :
         {
            errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                       ZL303XX_MGMT_ERR_NOT_SUPPORTED,
                                       managementId,
                                       "Master cluster option not supported");
            break;
         }

         case PTP2_MGMT_UNICAST_MASTER_TABLE :
         {
            if (((manage->actionField == PTP_MANAGEMENT_ACTION_GET) && (managementTlvInDataLength != 0)) ||
                ((manage->actionField == PTP_MANAGEMENT_ACTION_SET) && (managementTlvInDataLength < 3)))
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                      ZL303XX_MGMT_ERR_WRONG_LENGTH,
                                      managementId,
                                      errStrInvalidLength);
            }
            else
            {
               switch (manage->actionField)
               {
                  case PTP_MANAGEMENT_ACTION_SET :
                  {
                     /* Check the scope of the request is for a specific port */
                     if (msgScope != PTPV2_MGMT_SCOPE_SINGLE_PORT)
                     {
                        errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                               ZL303XX_MGMT_ERR_WRONG_VALUE,
                                               managementId,
                                               errStrInvalidTgtId);
                        break;
                     }
                     else
                     {  /* Get the table for the specified port. */
                        zl303xx_PortDataS *requestedPortData;

                        (void)zl303xx_PtpPortDataFromPortIdentity(
                                       &manage->targetPortIdentity,
                                       &requestedPortData);

                        if (requestedPortData == NULL)
                        {  /* No matching port found. Return a management error TLV */
                           errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                                  ZL303XX_MGMT_ERR_WRONG_VALUE,
                                                  managementId,
                                                  errStrInvalidTgtId);
                           break;
                        }
                        else
                        {
                           zl303xx_PtpUmtS newUmt;

                           /* Unpack the Unicast Master Table TLV into a UMT structure. */
                           zlStatusE status = zl303xx_PtpUmtTlvUnpack(pTlvDataIn, tlvInDataLength, &newUmt);

                           /* Replace the current UMT in the port with the unpacked one. */
                           if (status == ZL303XX_OK)
                           {
                              status = zl303xx_PtpUmtReplace(&requestedPortData->unicastMasterTable, &newUmt);
                           }

                           if (status != ZL303XX_OK)
                           {
                              errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                                     ZL303XX_MGMT_ERR_GENERAL_ERROR,
                                                     managementId,
                                                     "Unicast Master Table SET failed");
                              break;
                           }

                           /* Free any memory allocated by the Unpack function. */
                           zl303xx_PtpUmtFree(&newUmt);
                        }
                     }

                     /* Fall-through intentional so a RESPONSE TLV is sent. */
                  }

                  case PTP_MANAGEMENT_ACTION_GET :
                  {
                     /* Check the scope of the request is for a specific port */
                     if (msgScope != PTPV2_MGMT_SCOPE_SINGLE_PORT)
                     {
                        errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                               ZL303XX_MGMT_ERR_WRONG_VALUE,
                                               managementId,
                                               errStrInvalidTgtId);
                     }
                     else
                     {  /* Get the table for the specified port. */
                        zl303xx_PortDataS *requestedPortData;

                        (void)zl303xx_PtpPortDataFromPortIdentity(
                                       &manage->targetPortIdentity,
                                       &requestedPortData);

                        if (requestedPortData == NULL)
                        {  /* No matching port found. Return a management error TLV */
                           errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                                  ZL303XX_MGMT_ERR_WRONG_VALUE,
                                                  managementId,
                                                  errStrInvalidTgtId);
                        }
                        else
                        {
                           if (zl303xx_PtpUmtTlvPack(&requestedPortData->unicastMasterTable,
                                                     pTlvDataOut, &tlvOutDataLength) == ZL303XX_OK)
                           {
                              /* Add a pad byte if the TLV data is an odd number of bytes. */
                              if (tlvOutDataLength & 1)
                              {
                                 pTlvDataOut[tlvOutDataLength] = 0;
                                 tlvOutDataLength++;
                              }
                           }
                           else
                           {
                              errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                                     ZL303XX_MGMT_ERR_GENERAL_ERROR,
                                                     managementId,
                                                     "Unicast Master Table GET failed");
                           }
                        }
                     }
                     break;
                  }

                  case PTP_MANAGEMENT_ACTION_ACK:
                  case PTP_MANAGEMENT_ACTION_RSP:
                  case PTP_MANAGEMENT_ACTION_CMD:
                  default :
                     break;
               }
            }

            break;
         }

         case PTP2_MGMT_UNICAST_MASTER_MAX_TABLE_SIZE :
         {
            if ((manage->actionField == PTP_MANAGEMENT_ACTION_GET) && (managementTlvInDataLength != 0))
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                      ZL303XX_MGMT_ERR_WRONG_LENGTH,
                                      managementId,
                                      errStrInvalidLength);
            }
            else
            {
               switch (manage->actionField)
               {
                  case PTP_MANAGEMENT_ACTION_GET :
                     /* Check the scope of the request is for a specific port */
                     if (msgScope != PTPV2_MGMT_SCOPE_SINGLE_PORT)
                     {
                        errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                               ZL303XX_MGMT_ERR_WRONG_VALUE,
                                               managementId,
                                               errStrInvalidTgtId);
                     }
                     else
                     {  /* Get the value for the specified port */
                        zl303xx_PortDataS *requestedPortData;
                        (void)zl303xx_PtpPortDataFromPortIdentity(&manage->targetPortIdentity, &requestedPortData);
                        if (requestedPortData == NULL)
                        {  /* No matching port found. Return a management error TLV */
                           errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                                   ZL303XX_MGMT_ERR_WRONG_VALUE,
                                                   managementId,
                                                   errStrInvalidTgtId);
                        }
                        else
                        {
                           PACK_16BIT(pTlvDataOut + 0, requestedPortData->unicastMasterTable.maxTableSize);
                           tlvOutDataLength = 2;
                        }
                     }
                     break;

                  case PTP_MANAGEMENT_ACTION_ACK:
                  case PTP_MANAGEMENT_ACTION_RSP:
                  case PTP_MANAGEMENT_ACTION_SET:
                  case PTP_MANAGEMENT_ACTION_CMD:
                  default :
                     /* Invalid request. Return an error response. */
                     errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                            ZL303XX_MGMT_ERR_NOT_SUPPORTED,
                                            managementId,
                                            errStrInvalidAction);
                     break;
               }
            }

            break;
         }

         case PTP2_MGMT_ACCEPTABLE_MASTER_TABLE :
         case PTP2_MGMT_ACCEPTABLE_MASTER_TABLE_ENABLED :
         case PTP2_MGMT_ACCEPTABLE_MASTER_MAX_TABLE_SIZE :
         {
            errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                       ZL303XX_MGMT_ERR_NOT_SUPPORTED,
                                       managementId,
                                       "Acceptable master table option not supported");
            break;
         }

         case PTP2_MGMT_ALTERNATE_MASTER :
         {
            errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                       ZL303XX_MGMT_ERR_NOT_SUPPORTED,
                                       managementId,
                                       "Alternate master option not supported");
            break;
         }

         case PTP2_MGMT_ALTERNATE_TIME_OFFSET_ENABLE :
         case PTP2_MGMT_ALTERNATE_TIME_OFFSET_NAME :
         case PTP2_MGMT_ALTERNATE_TIME_OFFSET_MAX_KEY :
         case PTP2_MGMT_ALTERNATE_TIME_OFFSET_PROPERTIES :
         {
            errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                       ZL303XX_MGMT_ERR_NOT_SUPPORTED,
                                       managementId,
                                       "Alternate timescale option not supported");
            break;
         }

         /* Management IDs applicable to transparent clocks */
         case PTP2_MGMT_ID_TRANSPARENT_CLOCK_DEFAULT_DATA_SET :
         case PTP2_MGMT_ID_TRANSPARENT_CLOCK_PORT_DATA_SET :
         case PTP2_MGMT_ID_PRIMARY_DOMAIN :
         {
            errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                       ZL303XX_MGMT_ERR_NOT_SUPPORTED,
                                       managementId,
                                       "Transparent clock data sets not supported");
            break;
         }

         case PTP2_MGMT_ID_DELAY_MECHANISM :
            /* Only support the GET operation since this clock only implements the End-to-End mechanism */
            if ((manage->actionField == PTP_MANAGEMENT_ACTION_GET) && (managementTlvInDataLength != 0))
            {
               errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_LENGTH, managementId,
                                    errStrInvalidLength);
            }
            else
            {
               switch (manage->actionField)
               {
                  case PTP_MANAGEMENT_ACTION_SET :
                     errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack_SetFailed(oBuf, managementId);
                     break;

                  case PTP_MANAGEMENT_ACTION_GET :
                     /* Check the scope of the request is for a specific port */
                     if (msgScope != PTPV2_MGMT_SCOPE_SINGLE_PORT)
                     {
                        errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_VALUE, managementId,
                                             errStrInvalidTgtId);
                     }
                     else
                     {  /* Get the value for the specified port */
                        zl303xx_PortDataS *requestedPortData;
                        (void)zl303xx_PtpPortDataFromPortIdentity(&manage->targetPortIdentity, &requestedPortData);
                        if (requestedPortData == NULL)
                        {  /* No matching port found. Return a management error TLV */
                           errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_WRONG_VALUE, managementId,
                                                errStrInvalidTgtId);
                        }
                        else
                        {
                           *(Uint8T *)(pTlvDataOut + 0) = requestedPortData->config.delayMechanism;
                           *(Uint8T *)(pTlvDataOut + 1) = 0;    /* reserved */
                           tlvOutDataLength = 2;
                        }
                     }
                     break;

                  case PTP_MANAGEMENT_ACTION_ACK:
                  case PTP_MANAGEMENT_ACTION_RSP:
                  case PTP_MANAGEMENT_ACTION_CMD:
                  default :
                     /* Invalid request. Return an error response. */
                     errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf, ZL303XX_MGMT_ERR_NOT_SUPPORTED, managementId,
                                          errStrInvalidAction);
                     break;
               }
            }
            break;

         case PTP2_MGMT_ID_LOG_MIN_PDELAY_REQ_INTERVAL :
         {
            errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                       ZL303XX_MGMT_ERR_NOT_SUPPORTED,
                                       managementId,
                                       "PDelay measurements not supported");
            break;
         }

         default :
         {
            /* Exercise the Custom Handler Table for a match. */
            Uint16T tlvOutTotalLength = zl303xx_PtpV2TlvCustomHandler(
                                 portData->clock->customTlvTable, portData->clock->clockHandle,
                                 pPtpMsgRx, iBuf, oBuf);

            /* The output buffer must include the tlvType, length and managementId
             * fields (6-bytes), so anything less than this is considered an error.
             * (i.e. no Custom Handler exists). */
            if (tlvOutTotalLength >= ZL303XX_PTP_TLV_CUSTOM_RSP_SUCCESS)
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
                     "Custom MANAGEMENT message. Type = %u, managementID = %u",
                     tlvType, managementId, 0,0,0,0);

               /* Since custom routines return the length of the entire oBuf, adjust
                * for the extra 3 fields (6-bytes).       */
               tlvOutDataLength = tlvOutTotalLength - ZL303XX_PTP_V2_MGMT_TLV_HEADER_LEN;

               /* Make sure dataField is even. */
               if ((tlvOutDataLength % 2) != 0)
               {
                  *(Uint8T *)(pTlvDataOut + tlvOutDataLength) = 0;
                  tlvOutDataLength += 1;
               }
            }

            /* ELSE: fall-through to the default Error message. */
            else
            {
               /* If the Handler provided a response, then use it. */
               if (tlvOutTotalLength == ZL303XX_PTP_TLV_CUSTOM_RSP_PROVIDED)
               {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
                        "Custom MANAGEMENT TLV Error. managementID = %u",
                        managementId, 0,0,0,0,0);

                  /* Extract the MANAGEMENT ERROR TLV Data length from the buffer.
                   * The oBuf is already packed. */
                  zl303xx_PtpV2MgmtTlvTypeLengthIdGet(oBuf, NULL, &errorMsgLength, NULL);
               }

               /* If the Handler didn't support the action. */
               else if (tlvOutTotalLength == ZL303XX_PTP_TLV_CUSTOM_RSP_UNSUPPORTED)
               {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
                        "Custom MANAGEMENT TLV Unsupported Action. managementID(%u), action(%u)",
                        managementId, manage->actionField, 0,0,0,0);

                  errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack(oBuf,
                                             ZL303XX_MGMT_ERR_NOT_SUPPORTED,
                                             managementId,
                                             "Unsupported custom Action");
               }

               /* If a Handler exists but no response is required. */
               else if (tlvOutTotalLength == ZL303XX_PTP_TLV_CUSTOM_RSP_NONE)
               {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
                        "Custom MANAGEMENT TLV  Processed. managementID(%u), action(%u)",
                        managementId, manage->actionField, 0,0,0,0);

                  /* No data is to be replied. */
                  errorMsgLength = 0;
                  tlvOutDataLength = 0;
                  msgLength = 0;
               }

               /* No Handler (or Default handling). */
               else
               {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
                        "Unidentified MANAGEMENT message. Type = %u, managementID = %u",
                        tlvType,
                        managementId, 0,0,0,0);

                  errorMsgLength = zl303xx_PtpV2MgmtTlvErrorPack_InvalidMgmtId(oBuf, managementId);
               }
            }

            break;
         }
      }

      /* If the msgLength has not been set explicitly then calculate it. */
      if (errorMsgLength > 0)
      {
         msgLength = errorMsgLength;
      }
      else
      {
         /* Total message consists of tlvType, length and managementId  plus any TLV data */
         msgLength += tlvOutDataLength;
      }
   }

   /* Insert the length field into the output TLV (+2 for managementId) */
   tlvOutDataLength += 2;
   zl303xx_PtpV2MgmtTlvTypeLengthIdSet(NULL, &tlvOutDataLength, NULL, oBuf);

   /* Return the number of added bytes. */
   *addedBytes = msgLength;

   /* Return the number of input bytes processed. Either the original TLV will
    * be completely processed or there is an error, i.e. tlvInDataLength == 0 */
   if (tlvInDataLength > 0)
   {
      /* Whole of input TLV was processed */
      tlvInDataLength += 4;
   }

   return tlvInDataLength;
}  /* END zl303xx_V2HandleManagementTLV */

/****************   STATIC FUNCTION DEFINITIONS   ****************************/

/**********************  PACK & UNPACK HELPER FUNCTIONS ***********************/

/*

  Function Name:
   zl303xx_HandleUnicastEnableGet

  Details:
   Determines the current state of the nodes Unicast Negotiation and data fills
   the zl303xx_PtpV2MgmtTlvUnicastNegEnS structure so that a response can be issued.

  Parameters:
       portData    Pointer to the Port Data structure associated with the stream

       par         Pointer to a Unicast Management TLV structure in with to return
                  the results of the analysis

  Return Value:
   zlStatusE

  Notes:
   If Unicast Enabled = TRUE for the clock stream but we have no active message
   streams, then we send a response indicating that we are Enabled.
   If Unicast Enabled = FALSE for the clock stream it does not mean that all
   individual message streams (ANNOUNCE/SYNC/etc) have finished CANCELLING
   existing GRANTS. Therefore, each individual stream stream must be tested to
   see if they are not IDLE.

*******************************************************************************/
zlStatusE zl303xx_HandleUnicastEnableGet(zl303xx_PortDataS *portData, zl303xx_BooleanE *uniNegEnabled)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *clockStream = NULL;

   status = ZL303XX_CHECK_POINTER(portData) |
            ZL303XX_CHECK_POINTER(uniNegEnabled);

   if (status == ZL303XX_OK)
   {
      /* Initially, set the Unicast Response to the overall setting */
      *uniNegEnabled = portData->config.unicastNegotiation.enabled;

      /* If Unicast Enabled = FALSE it does not mean that all streams have
         finished CANCELLING existing GRANTS. Therefore, see if any message
         streams are still active. */
      if (*uniNegEnabled == ZL303XX_FALSE)
      {
         /* We do not need to check that the Clock and Clock-Port are
            initialized since all ClockStreams will be NULL if initialization
            has been done properly & consistent. */

         ZL303XX_PTP_FOR_EACH_STREAM(&portData->streamList, clockStream)
         {
            if ((clockStream->config.unicast == ZL303XX_TRUE)  &&
                ((clockStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].remainingSec > 0) ||
                 (clockStream->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].remainingSec > 0) ||
                 (clockStream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].remainingSec > 0) ||
                 (clockStream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].remainingSec > 0)))
            {
               /* At least 1 negotiated stream is not yet IDLE so respond
                  that Unicast Negotiation is still Enabled (16.1.4.5) */
               *uniNegEnabled = ZL303XX_TRUE;
               break;
            }
         }
      }
   }

   return status;
}  /* END zl303xx_HandleUnicastEnableGet */

/*

  Function Name:
   zl303xx_HandleUnicastEnableSet

  Details:
   Sets the state of the Unicast Negotiation functionality from the info in the
   zl303xx_PtpV2MgmtTlvUnicastNegEnS structure and triggers any necessary state machine
   transitions.

  Parameters:
       portData    Pointer to the Port Data structure associated with the stream
       par         The Unicast Management TLV structure containing the new Unicast
                  Negotiation state

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_HandleUnicastEnableSet(zl303xx_PortDataS *portData, zl303xx_BooleanE uniNegEnabled)
{
   zl303xx_ClockStreamS *clockStream = NULL;

   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(portData);

   if (status == ZL303XX_OK)
   {
      /* Determine if any change in state is even needed */
      if (uniNegEnabled != portData->config.unicastNegotiation.enabled)
      {
         /* A change in state of Unicast negotiation has been requested */
         if (uniNegEnabled == ZL303XX_TRUE)
         {
            /* We were previously disabled so start up any streams that are
               meant to Negotiate a GRANT */
            ZL303XX_PTP_FOR_EACH_STREAM(&portData->streamList, clockStream)
            {
               if (clockStream->config.mode != ZL303XX_PTP_STREAM_MASTER_ONLY)
               {
                  status = zl303xx_PtpUmtStartAllStreams(&portData->unicastMasterTable);
               }
            }
            /* MASTER: Nothing special required */
         }
         /* Setting to FALSE is handled in the uni-neg state machine. */

         /* Store the change to the set value in the port data */
         portData->config.unicastNegotiation.enabled = uniNegEnabled;
      }
   }

   return status;
}

/********   STATIC FUNCTIONS FOR ANNOUNCE MESSAGE HANDLING   *****************/

/* zl303xx_PtpV2MsgDataAnncGet */
/** 

   Fills an Announce data structure based on a clock structure. This structure
   is used to pack the data section of an Announce message.

  Parameters:
   [in]   pClock     PTP clock used to fill the Announce data structure.
   [out]  pAnncData  Filled Announce data structure.

*******************************************************************************/
static void zl303xx_PtpV2MsgDataAnncGet(
      zl303xx_PtpClockS *pClock,
      zl303xx_PtpV2MsgAnnounceS *pAnncData)
{
   /* CurrentDS members. */
   pAnncData->stepsRemoved = pClock->currentDSInt.stepsRemoved;

   /* TimePropertiesDS members. */
   pAnncData->currentUtcOffset = pClock->timePropertiesDS.currentUtcOffset;
   pAnncData->timeSource = pClock->timePropertiesDS.timeSource;

   /* ParentDS members */
   OS_MEMCPY(pAnncData->grandmasterIdentity,
          pClock->parentDS.grandmasterIdentity,
          PTP_V2_CLK_IDENTITY_LENGTH);
   pAnncData->grandmasterClockQuality = pClock->parentDS.grandmasterClockQuality;
   pAnncData->grandmasterPriority1 = pClock->parentDS.grandmasterPriority1;
   pAnncData->grandmasterPriority2 = pClock->parentDS.grandmasterPriority2;

   /* Clock class behavior (if UNCALIBRATED state). */
   zl303xx_PtpClockUncalibratedClassEvaluate(pClock, &pAnncData->grandmasterClockQuality.clockClass);


   /* Get an inaccurate time stamp (+/- 1 sec). The returned time stamp is
    * already adjusted for latency. */
   if (zl303xx_PtpClockTimeGet(pClock, ZL303XX_PTP_INTERFACE_GENERAL,
                               &pAnncData->originTimestamp) != ZL303XX_OK)
   {
      /* If there is an error retrieving the clock time, zero-fill the
       * originTimestamp field, so the message will contain standard-compliant
       * data. The error is logged inside zl303xx_PtpClockTimeGet(). */
      OS_MEMSET(&pAnncData->originTimestamp, 0, sizeof(pAnncData->originTimestamp));
   }
}

/* zl303xx_PtpV2MsgDataAnncOverridesGet */
/** 

   Replaces any fields of the Announce message structure with configured
   Over-ride values. All other fields are left untouched, therefore, this
   routine should only be called after zl303xx_PtpV2MsgDataAnncGet() to
   ensure default egress values are selected first.

   In the case where both a Clock and a Stream over-ride value exists, the
   stream value will take priority.

  Parameters:
   [in]   pPtpStream    PTP stream (and clock) to use over-ride values for.
   [out]  pAnncData     Filled Announce data structure with over-ride values.

*******************************************************************************/
static void zl303xx_PtpV2MsgDataAnncOverridesGet(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgAnnounceS *pAnncData)
{
   /* Dereference this Streams Clock data. */
   zl303xx_PtpClockS *pPtpClock = pPtpStream->portData->clock;

   /* grandmasterIdentity */
   if (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_GM_IDENTITY] == ZL303XX_TRUE)
   {
      OS_MEMCPY(pAnncData->grandmasterIdentity,
             pPtpStream->config.override.anncData.grandmasterIdentity,
             PTP_V2_CLK_IDENTITY_LENGTH);
   }

   /* ParentDS::grandmasterClockQuality::clockClass */
   /* We have an existing interface here so do the following:
    * - Give priority to the new interface (stream) first.
    * - Give priority to the new interface (clock) next.
    * - Give priority to the OLD interface (stream) next.
    * - Give priority to the OLD interface (clock) last.
    * Eventually, deprecate the OLD interface.     */
   if (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_GM_CLASS] == ZL303XX_TRUE)
   {
      pAnncData->grandmasterClockQuality.clockClass = pPtpStream->config.override.anncData.grandmasterClockQuality.clockClass;
   }
   else if (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_CLASS] == ZL303XX_TRUE)
   {
      pAnncData->grandmasterClockQuality.clockClass = pPtpClock->config.override.defaultDS.clockQuality.clockClass;
   }
   /** OLD INTERFACE **/
   /* Use the stream egress clockClass over-ride value if configured.   */
   else if (pPtpStream->config.egressClockClass != PTP_CLOCK_CLASS_OVERRIDE_DISABLED)
   {
      pAnncData->grandmasterClockQuality.clockClass = pPtpStream->config.egressClockClass;
   }
   /* Otherwise, use the clock egress clockClass over-ride value if configured.   */
   else if (pPtpStream->portData->clock->config.egressClockClass != PTP_CLOCK_CLASS_OVERRIDE_DISABLED)
   {
      pAnncData->grandmasterClockQuality.clockClass = pPtpStream->portData->clock->config.egressClockClass;
   }

   /* ParentDS::grandmasterClockQuality::clockAccuracy */
   if (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_GM_ACCURACY] == ZL303XX_TRUE)
   {
      pAnncData->grandmasterClockQuality.clockAccuracy = pPtpStream->config.override.anncData.grandmasterClockQuality.clockAccuracy;
   }
   else if (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_ACCURACY] == ZL303XX_TRUE)
   {
      pAnncData->grandmasterClockQuality.clockAccuracy = pPtpClock->config.override.defaultDS.clockQuality.clockAccuracy;
   }

   /* ParentDS::grandmasterClockQuality::offsetScaledLogVariance */
   if (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_GM_VARIANCE] == ZL303XX_TRUE)
   {
      pAnncData->grandmasterClockQuality.offsetScaledLogVariance = pPtpStream->config.override.anncData.grandmasterClockQuality.offsetScaledLogVariance;
   }
   else if (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_OFFSET_SCALED_LOG_VARIANCE] == ZL303XX_TRUE)
   {
      pAnncData->grandmasterClockQuality.offsetScaledLogVariance = pPtpClock->config.override.defaultDS.clockQuality.offsetScaledLogVariance;
   }

   /* ParentDS::grandmasterPriority1 */
   if (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_GM_PRIORITY1] == ZL303XX_TRUE)
   {
      pAnncData->grandmasterPriority1 = pPtpStream->config.override.anncData.grandmasterPriority1;
   }
   else if (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_PRIORITY1] == ZL303XX_TRUE)
   {
      pAnncData->grandmasterPriority1 = pPtpClock->config.override.defaultDS.priority1;
   }

   /* ParentDS::grandmasterPriority2 */
   if (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_GM_PRIORITY2] == ZL303XX_TRUE)
   {
      pAnncData->grandmasterPriority2 = pPtpStream->config.override.anncData.grandmasterPriority2;
   }
   else if (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_PRIORITY2] == ZL303XX_TRUE)
   {
      pAnncData->grandmasterPriority2 = pPtpClock->config.override.defaultDS.priority2;
   }

   /* TimePropertiesDS::currentUtcOffset. */
   if (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_CURRENT_UTC_OFFSET] == ZL303XX_TRUE)
   {
      pAnncData->currentUtcOffset = pPtpStream->config.override.anncData.currentUtcOffset;
   }
   else if (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_CURRENT_UTC_OFFSET] == ZL303XX_TRUE)
   {
      pAnncData->currentUtcOffset = pPtpClock->config.override.timePropertiesDS.currentUtcOffset;
   }

   /* TimePropertiesDS::timeSource. */
   if (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_TIME_SOURCE] == ZL303XX_TRUE)
   {
      pAnncData->timeSource = pPtpStream->config.override.anncData.timeSource;
   }
   else if (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_TIME_SOURCE] == ZL303XX_TRUE)
   {
      pAnncData->timeSource = pPtpClock->config.override.timePropertiesDS.timeSource;
   }

   /* CurrentDS::stepsRemoved. */
   if (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_STEPS_REMOVED] == ZL303XX_TRUE)
   {
      pAnncData->stepsRemoved = pPtpStream->config.override.anncData.stepsRemoved;
   }
   else if (pPtpClock->config.override.enabled[ZL303XX_PTP_CLOCK_OVERRIDE_STEPS_REMOVED] == ZL303XX_TRUE)
   {
      pAnncData->stepsRemoved = pPtpClock->config.override.currentDS.stepsRemoved;
   }

   /* originTimestamp */
   if (pPtpStream->config.override.anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_ORIGIN_TIMESTAMP] == ZL303XX_TRUE)
   {
      pAnncData->originTimestamp = pPtpStream->config.override.anncData.originTimestamp;
   }
}

/* zl303xx_PtpV2MsgDataAnncPack */
/** 

   Packs the Announce data section of an Announce message.

  Parameters:
   [in]   pAnncDataIn   Structure containing the Announce data to be packed.

   [out]  pAnncDataBufOut  Message buffer containing the packed Announce data.
                                 (Must point to the beginning of the announce data,
                                 the first byte after the message header).

  Return Value:
     Uint16T  Length of Announce data packed into pAnncDataBufOut.
     0        An error occurred.

  Notes:  Assumes all input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgDataAnncPack(
      zl303xx_PtpV2MsgAnnounceS *pAnncDataIn,
      Uint8T *pAnncDataBufOut)
{
   /* originTimestamp */
   (void)zl303xx_PtpV2MsgTimestampPack(&pAnncDataIn->originTimestamp, (pAnncDataBufOut + ZL303XX_PTP_ANNC_TIMESTAMP_OFST));

   /* currentUtcOffset */
   PACK_16BIT((pAnncDataBufOut + ZL303XX_PTP_ANNC_UTC_OFFSET_OFST), pAnncDataIn->currentUtcOffset);

   /* timeSource */
   pAnncDataBufOut[ZL303XX_PTP_ANNC_TIME_SOURCE_OFST] = pAnncDataIn->timeSource;

   /* stepsRemoved */
   PACK_16BIT((pAnncDataBufOut + ZL303XX_PTP_ANNC_STEPS_REMOVE_OFST), pAnncDataIn->stepsRemoved);

   /* grandmasterIdentity */
   zl303xx_PtpV2MsgClockIdentityPack(pAnncDataIn->grandmasterIdentity, (pAnncDataBufOut + ZL303XX_PTP_ANNC_GM_ID_OFST));

   /* grandmasterClockQuality */
   zl303xx_PtpV2MsgClockQualityPack(&pAnncDataIn->grandmasterClockQuality, (pAnncDataBufOut + ZL303XX_PTP_ANNC_GM_QUALITY_OFST));

   /* grandmasterPriority1 and grandmasterPriority2 */
   pAnncDataBufOut[ZL303XX_PTP_ANNC_GM_PRIORITY1_OFST] = pAnncDataIn->grandmasterPriority1;
   pAnncDataBufOut[ZL303XX_PTP_ANNC_GM_PRIORITY2_OFST] = pAnncDataIn->grandmasterPriority2;

   return ZL303XX_PTP_V2_ANNC_DATA_LEN;
}  /* END zl303xx_PtpV2MsgDataAnncPack */

/* zl303xx_PtpV2MsgDataAnncUnpack */
/** 

   Unpacks the Announce data section from a PTP message into a structure.

  Parameters:
   [in]   pAnncDataBufIn   Buffer containing the Announce data received in
                                 a PTP packet (Must point to the beginning of
                                 the announce data, the first byte after the
                                 message header)

   [out]  pAnncDataOut     Pointer to an Announce data structure (in this case,
                                 the generic union type).

  Return Value:
     Uint16T  Size in bytes of the Announce data buffer that is unpacked.
     0        An error occurred.

  Notes:  Assumes all input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgDataAnncUnpack(
      Uint8T *pAnncDataBufIn,
      zl303xx_PtpV2MsgDataU *pAnncDataOut)
{
   zl303xx_PtpV2MsgAnnounceS *pAnncDataS = &pAnncDataOut->announce;


   /* Origin Timestamp */
   zl303xx_PtpV2MsgTimestampUnpack(pAnncDataBufIn, &pAnncDataS->originTimestamp);

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
                 "   originTimestamp " PTPD_FORMAT_TIME_STR,
                 formatTimeStringSign(&pAnncDataS->originTimestamp),
                 formatTimeStringSec(&pAnncDataS->originTimestamp),
                 formatTimeStringNs(&pAnncDataS->originTimestamp),
                 0,0,0);

   /* Other Announce data. */
   pAnncDataS->currentUtcOffset = UNPACK_16BIT(pAnncDataBufIn + ZL303XX_PTP_ANNC_UTC_OFFSET_OFST);
   pAnncDataS->timeSource       = pAnncDataBufIn[ZL303XX_PTP_ANNC_TIME_SOURCE_OFST];
   pAnncDataS->stepsRemoved     = UNPACK_16BIT(pAnncDataBufIn + ZL303XX_PTP_ANNC_STEPS_REMOVE_OFST);

   /* Received grandmaster parameters */
   zl303xx_PtpV2MsgClockIdentityUnpack((pAnncDataBufIn + ZL303XX_PTP_ANNC_GM_ID_OFST), pAnncDataS->grandmasterIdentity);
   zl303xx_PtpV2MsgClockQualityUnpack((pAnncDataBufIn + ZL303XX_PTP_ANNC_GM_QUALITY_OFST), &pAnncDataS->grandmasterClockQuality);
   pAnncDataS->grandmasterPriority1 = pAnncDataBufIn[ZL303XX_PTP_ANNC_GM_PRIORITY1_OFST];
   pAnncDataS->grandmasterPriority2 = pAnncDataBufIn[ZL303XX_PTP_ANNC_GM_PRIORITY2_OFST];

   return ZL303XX_PTP_V2_ANNC_DATA_LEN;
}  /* END zl303xx_PtpV2MsgDataAnncUnpack */

/***************** END OF VERSION 2 MESSAGE FORMATS **************************/



