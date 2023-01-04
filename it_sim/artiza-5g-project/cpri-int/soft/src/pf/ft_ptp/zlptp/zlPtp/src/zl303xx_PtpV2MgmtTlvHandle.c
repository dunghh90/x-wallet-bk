

/*****************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Definitions and utility functions for handling (processing)
*     Type-Length-Value (TLV) structures/buffers for management TLVs.
*
******************************************************************************/

/****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"

#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpDatatypes.h"
#include "zl303xx_PtpFunctionExport.h"
#include "zl303xx_PtpConstants.h"
#include "zl303xx_PtpV2MsgParse.h"
#include "zl303xx_PtpV2MgmtTlvParse.h"
#include "zl303xx_PtpV2MgmtTlvHandle.h"
#include "zl303xx_PtpV2MgmtTlvDef.h"

/****************   DEFINES     **********************************************/

/****************   DATA TYPES   *********************************************/

/****************   STATIC FUNCTION DECLARATIONS   ***************************/

/* Default Data Set Action Routines. */
Uint16T zl303xx_PtpMgmtTlvHandleDefaultDSMembersGet(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpV2MgmtTlvIdE memberId,
      Uint8T *pOutTlvDataBuf);
Uint16T zl303xx_PtpMgmtTlvHandleDefaultDSMembersSet(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpV2MgmtTlvIdE memberId,
      Uint8T *pInTlvDataBuf);

/****************   STATIC GLOBAL VARIABLES   ********************************/

/* Strings corresponding to the zl303xx_PtpV2MgmtActionE type */
const char *Zl303xx_ManagementErrorStr[] =
{
   "NO ERROR",                   /* ZL303XX_MGMT_ERR_NO_ERROR */
   "RESPONSE TOO BIG",           /* ZL303XX_MGMT_ERR_RESPONSE_TOO_BIG */
   "NO SUCH MGMT ID",            /* ZL303XX_MGMT_ERR_NO_SUCH_ID */
   "WRONG LENGTH",               /* ZL303XX_MGMT_ERR_WRONG_LENGTH */
   "WRONG VALUE",                /* ZL303XX_MGMT_ERR_WRONG_VALUE */
   "NOT SETABLE",                /* ZL303XX_MGMT_ERR_NOT_SETABLE */
   "NOT SUPPORTED",              /* ZL303XX_MGMT_ERR_NOT_SUPPORTED */
   "GENERAL or UNKNOWN ERROR",   /* ZL303XX_MGMT_ERR_GENERAL_ERROR */
};

#define Zl303xx_MgmtErrorTlvStr(mgmtErrId)  \
   ((mgmtErrId <= ZL303XX_MGMT_ERR_NOT_SUPPORTED) \
         ? Zl303xx_ManagementErrorStr[mgmtErrId] \
         : Zl303xx_ManagementErrorStr[ZL303XX_MGMT_ERR_NOT_SUPPORTED + 1])

/****************   EXPORTED GLOBAL VARIABLES   ******************************/

/****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/*

  Function Name:
   zl303xx_PtpV2MgmtErrorTlvHandle

  Details:
   Routine to process the TLV of a MANAGEMENT ERROR message. If the processing
   has reached this level then all sanity checks at higher levels is assumed to
   have passed (as per the following hierarchy):
   - PTP Header processing             (completed)
   - Management Data processing        (completed)
   - Management ERROR TLV processing   (current)

   This routine currently prints the ERROR TLV.

  Parameters:
   [in]  pPtpRxQueueData Pointer to the PTP message queue structure. Contains
                              the type info & message buffer, including appended
                              TLVs, etc. (often the same as the packet buffer).
   [in]  pPtpMsgRx      The received message unpacked into a PTP Header and
                              Data structure. In this case, the Data portion is
                              of zl303xx_PtpV2MsgManagementS type.
   [in]  pPtpStream     Pointer to the clock stream instance structure (may
                              be NULL since TLVs are Port oriented.
   [in]  pRxPtpPort     Pointer to the PTP Port on which the message was
                              received. Cannot be NULL.

  Return Value:
   On SUCCESS: The number of appended TLV bytes handled by this function.
   Otherwise:  0

  Notes:
   Assumes all input pointers are valid (for this internal routine).
   Assumes all processing (sanity) of upper message layers is complete and
      successful.

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtErrorTlvHandle(
      zl303xx_PtpClockMsgQueueS *pPtpRxQueueData,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PortDataS *pRxPtpPort)
{
   Uint16T tlvType, tlvLengthField;
   zl303xx_ManagementErrorIdE mgmtErrorId;
   zl303xx_PtpV2MgmtTlvIdE mgmtTlvId;
   char errorMsg[256];

   /* Pointer to the start of the TLV portion of the Received Error Buffer. */
   Uint8T *pInTlvBuf = pPtpRxQueueData->msgBuf + ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN;

   /* Get rid of compiler warnings */
   if (pPtpStream) {;}

   /* Extract the tlvType and length from the received TLV. */
   zl303xx_PtpV2MgmtTlvTypeLengthIdGet(pInTlvBuf, &tlvType, &tlvLengthField, NULL);

   /* Extract the other MANAGEMENT ERROR Fields. */
   zl303xx_PtpV2MgmtTlvErrorUnpack(pInTlvBuf, &mgmtErrorId, &mgmtTlvId, errorMsg);

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1, "=================================================",
         0,0,0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1, "MANAGEMENT ERROR (%u) received on Port (%u)",
         tlvType, pRxPtpPort->portDS.portIdentity.portNumber, 0,0,0,0);
   ZL303XX_TRACE_V2_PORT_ID(ZL303XX_MOD_ID_PTP_TLV, 1,
         "    from portIdentity: ", &pPtpMsgRx->header.sourcePortIdentity);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1, "=================================================",
         0,0,0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1, "   managementErrorId: (0x%04X):(%s)",
         mgmtErrorId, Zl303xx_MgmtErrorTlvStr(mgmtErrorId), 0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1, "        managementId: (0x%04X):(%s)",
         mgmtTlvId, zl303xx_PtpV2MgmtTlvDescr(mgmtTlvId), 0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1, "       error message: %s",
         errorMsg, 0,0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1, "=================================================",
         0,0,0,0,0,0);

   return 4 + tlvLengthField;
}  /* END zl303xx_PtpV2MgmtErrorTlvHandle */

/*

  Function Name:
   zl303xx_PtpV2MgmtTlvHandle

  Details:
   Routine to process the TLV of a MANAGEMENT message. If the processing has
   reached this level then all sanity checks at higher levels is assumed to have
   passed (as per the following hierarchy):
   - PTP Header processing       (completed)
   - Management Data processing  (completed)
   - Management TLV processing   (current)

   This routine will scan the appended Management TLVs (only type allowed) and
   process as required (send responses, perform commands, send errors, etc.).

  Parameters:
   [in]  pPtpRxQueueData Pointer to the PTP message queue structure. Contains
                              the type info & message buffer, including appended
                              TLVs, etc. (often the same as the packet buffer).
   [in]  pPtpMsgRx      The received message unpacked into a PTP Header and
                              Data structure. In this case, the Data portion is
                              of zl303xx_PtpV2MsgManagementS type.
   [in]  pPtpStream     Pointer to the clock stream instance structure (may
                              be NULL since TLVs are Port oriented.
   [in]  pRxPtpPort     Pointer to the PTP Port on which the message was
                              received. Cannot be NULL.

  Return Value:
   On SUCCESS: The number of appended TLV bytes handled by this function.
   Otherwise:  0

  Notes:
   Assumes all input pointers are valid (for this internal routine).
   Assumes all processing (sanity) of upper message layers is complete and
      successful.

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvHandle(
      zl303xx_PtpClockMsgQueueS *pPtpRxQueueData,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PortDataS *pRxPtpPort)
{
   const char *fnName = "zl303xx_PtpV2MgmtTlvHandle";

   /* Set the Default return value. */
   Uint16T numTlvBytesProcessed = 0;

   /* De-reference the header & management data to make code more readable. */
   zl303xx_PtpV2MsgHeaderS      *pPtpHdrS = &pPtpMsgRx->header;
   zl303xx_PtpV2MsgManagementS  *pMgmtHdrS = &pPtpMsgRx->data.management;

   /* Get the Clock pointer for the port. */
   zl303xx_PtpClockS *pPtpClock = pRxPtpPort->clock;

   /* Common variables of the TLV Header. */
   Uint16T tlvType;
   Uint16T tlvLength = 0;
   Uint16T managementId = 0;

   /*** INPUT TLV Processing Variables */
   /* Variables to track the processing progress of the TLV buffer. */
   /* Set the remaining size of the TLV area and set the current processing
    * point to the first octet after the MGMT message. */
   Uint16T tlvBytesRemaining = pPtpHdrS->messageLength - ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN;
   Uint8T *pInTlvBuf = pPtpRxQueueData->msgBuf + ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN;

   /*** RESPONSE Variables */
   /* As a starter, pack the base reply buffer so that it will not need to be
    * done multiple times at lower layers (i.e. ALL_PORT/CLOCKS). Since only one
    * message was received, multiple replies will have most of the same PTP
    * Header fields and Management Data (except pRxPtpPort->sourcePortIdentity).  */
   /* Variable for any Response message. */
   zl303xx_PtpV2MsgS pPtpMsgTx;
   /* Pointer to the Output (Response) message Buffer. */
   Uint8T *pOutMsgBuffer = pPtpClock->msgObuf;
   /* Pointer to the start of the TLV portion of the Response Buffer. */
   Uint8T *pOutTlvBuf = pOutMsgBuffer + ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN;
   /* respMsgBytes = ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN; from this routine. */
   Uint16T respMsgBytes = zl303xx_PtpV2MgmtTlvPrepareResponse(pPtpMsgRx, pRxPtpPort, &pPtpMsgTx, pOutMsgBuffer);
   /* This is the number of bytes appended to a message for transmission. */
   Uint16T respTlvBytes = 0;
   /* Number of local Ports (or Clocks) that match the targetPortIdentity. */
   Uint16T respPortCount = 0;

   /* STACKED TLV Variables. */
   /* Although MGMT messages DO NOT support stacked TLVs, this code is structured
    * in such a way as to allow it (some day if required). By default, keep it off.  */
   Uint16T stackedTlvCount = 0;
   zl303xx_BooleanE stackedTlvEnabled = ZL303XX_FALSE;

   /* RIGHT AWAY, ONLY CONTINUE IF THE TARGETED managementId PROCESSING HAS BEEN
    * MIGRATED TO THE NEW HANDLER MODULE.   */
   /* This can be removed when migration is complete. */
   /******************************************************************/
   {
      Uint16T mgmtId = 0;
      zl303xx_PtpV2MgmtTlvTypeLengthIdGet(pInTlvBuf, &tlvType, &tlvLength, &mgmtId);
      if (zl303xx_PtpV2MgmtTlvIdMigrated(mgmtId) == ZL303XX_FALSE)
      {
         /* Log entry into the TLV processing. */
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
               "%s: managementId(0x%04X) Not Migrated",
               fnName, mgmtId, 0,0,0,0);
         return 0;
      }
   }
   /******************************************************************/

   /* Log entry into the TLV processing. */
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
         "%s: Rx TLV Buffer, total length (%u) on Port (%u)",
         fnName, tlvBytesRemaining, pRxPtpPort->portDS.portIdentity.portNumber, 0,0,0);

   /* Begin Processing each TLV. */
   /* Although stacked TLV processing is not allowed for MANAGEMENT Messages,
    * the following loop allows it (if enabled). */
   /* Do not check status :although zlStatusE may fail within the loop for a
    * single TLV block, attempt to continue to the next TLV. For major errors,
    * use BREAK or set tlvBytesRemaining = 0. */
   /* A MGMT TLV is at least 6 octets long (Type + Length + ID).  */
   stackedTlvCount = 0;
   while ((tlvBytesRemaining >= ZL303XX_PTP_V2_MGMT_TLV_HEADER_LEN) &&
          ((stackedTlvCount == 0) || (stackedTlvEnabled == ZL303XX_TRUE)))
   {
      /* Count the number of Management TLVs processed. */
      stackedTlvCount++;

      /* Get the type, length and managementId fields of the current TLV. */
      zl303xx_PtpV2MgmtTlvTypeLengthIdGet(pInTlvBuf, &tlvType, &tlvLength, &managementId);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 3,
            "           : tlvType(%u), tlvLength(%u), managementId(0x%04X)",
            tlvType, tlvLength, managementId, 0,0,0);

      /* This routine is meant for MANAGEMENT TLVs. Therefore, only valid
       * MANAGEMENT or MANAGEMENT ERROR tlvType is allowed. */
      if (tlvType == PTP_TLV_MANAGEMENT_ERROR_STATUS)
      {
         numTlvBytesProcessed += zl303xx_PtpV2MgmtErrorTlvHandle(
                                 pPtpRxQueueData, pPtpMsgRx, pPtpStream, pRxPtpPort);
      }

      /* Do the IF statement a bit backwards to keep things together. */
      else if (tlvType != PTP_TLV_MANAGEMENT)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
               "%s: ERROR: PTP MANAGEMENT Message with conflicting tlvType(%u)",
               fnName, tlvType, 0,0,0,0);

         /* Pack a General Error Message and send it. */
         respMsgBytes = zl303xx_PtpV2MgmtErrorTlvRespond(
                              pPtpRxQueueData, &pPtpMsgTx, pRxPtpPort,
                              ZL303XX_MGMT_ERR_GENERAL_ERROR, managementId,
                              "Wrong tlvType for Management Message");

         /* Eat this TLV since we are unable to process. */
         /* Assumes the length is correct. */
         numTlvBytesProcessed += (4 + tlvLength);
      }

      /*** MANAGEMENT TLV (managementId) PROCESSING FROM HERE DOWN ****/
      /****************************************************************/

      /* Check that the managementId is supported. */
      else if ((zl303xx_PtpV2MgmtTlvIdSupported(managementId) != ZL303XX_TRUE) &&
               (zl303xx_PtpTlvCustomHandlerExists(
                     pRxPtpPort->clock->customTlvTable, PTP_TLV_MANAGEMENT, managementId) != ZL303XX_TRUE))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
               "%s: WARNING: Non-Supported managementId received(%u)",
               fnName, managementId, 0,0,0,0);

         /* Pack an Error Message and send it. */
         respTlvBytes = zl303xx_PtpV2MgmtTlvErrorPack_UnsupportedMgmtId(pOutTlvBuf, managementId);
         respMsgBytes = zl303xx_PtpV2MgmtTlvRespond(
                              pPtpRxQueueData,
                              &pPtpMsgTx,
                              pRxPtpPort,
                              pOutMsgBuffer, ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN + respTlvBytes);

         /* Eat this TLV since we are unable to process. */
         numTlvBytesProcessed += (4 + tlvLength);
      }
      /* Check that the managementId supports the specified ACTION. */
      else if (zl303xx_PtpV2MgmtTlvActionSupported(managementId, pMgmtHdrS->actionField) != ZL303XX_TRUE)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
               "%s: WARNING: Non-Supported action (%u) for managementId (%u)",
               fnName, pMgmtHdrS->actionField, managementId, 0,0,0);

         /* If the received action was RSP or ACK, do not reply. */
         if ((pMgmtHdrS->actionField != PTP_MANAGEMENT_ACTION_RSP) &&
             (pMgmtHdrS->actionField != PTP_MANAGEMENT_ACTION_ACK))
         {
            /* Pack an Error Message and send it. */
            respTlvBytes = zl303xx_PtpV2MgmtTlvErrorPack_InvalidAction(pOutTlvBuf, managementId);
            respMsgBytes = zl303xx_PtpV2MgmtTlvRespond(
                                 pPtpRxQueueData,
                                 &pPtpMsgTx,
                                 pRxPtpPort,
                                 pOutMsgBuffer, ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN + respTlvBytes);
         }

         /* Eat this TLV since we are unable to process. */
         numTlvBytesProcessed += (4 + tlvLength);
      }

      /* Check that the message is not longer or shorter than we expect. */
      else if (((4 + tlvLength) > tlvBytesRemaining) ||   /* length corruption */
               (tlvLength < 2))  /* Must be at least 2 bytes for the managementId. */
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
               "zl303xx_PtpV2MgmtTlvHandle: ERROR: lengthField Invalid: tlvType(%u),tlvLen(%u),managementId(0x%04X),rem(%u)",
               tlvType, tlvLength, managementId, tlvBytesRemaining, 0,0);

         /* Pack a General Error Message and send it. */
         respTlvBytes = zl303xx_PtpV2MgmtTlvErrorPack_InvalidLength(pOutTlvBuf, managementId);
         respMsgBytes = zl303xx_PtpV2MgmtTlvRespond(
                              pPtpRxQueueData,
                              &pPtpMsgTx,
                              pRxPtpPort,
                              pOutMsgBuffer, ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN + respTlvBytes);

         /* Eat this TLV since it is corrupt. */
         /* Assumes the length is correct. */
         numTlvBytesProcessed += tlvBytesRemaining;
      }

      /* Process the MANAGEMENT TLV. */
      /*******************************/
      else if (zl303xx_PtpV2MgmtTlvProcessFuncExists(managementId))
      {
         /* NOTE: The pPtpClock->msgObuf already contains a packed PTP Header
          *       Management Data section. (Some fields may need to be updated).   */

         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
               "           : Processing... managementId(0x%04X):%s",
               managementId, zl303xx_PtpV2MgmtTlvDescr(managementId), 0,0,0,0);

         /* For CLOCK targeted TLV */
         if (zl303xx_PtpV2MgmtTlvTargetObject(managementId, ZL303XX_PTP_MGMT_TLV_TARGET_OBJ_CLOCK))
         {
            /* This Clock must match since it was checked by the parent routine. */
            respPortCount++;

            /* At the zl303xx_PtpV2MsgMgmtHandle level (parent routine to this one),
             * it was verified that the targetPortIdentity matched this local
             * clock. Therefore, process the TLV using the managementId handler. */
            respTlvBytes = zl303xx_PtpV2MgmtTlvProcessFunc(managementId)(
                              pRxPtpPort,    /* Contains the clockIdentity & pointer of this clock. */
                              &pMgmtHdrS->actionField,
                              pInTlvBuf,
                              pOutTlvBuf);

            if (respTlvBytes != 0)
            {
               respMsgBytes = zl303xx_PtpV2MgmtTlvRespond(
                                    pPtpRxQueueData,
                                    &pPtpMsgTx,
                                    pRxPtpPort,
                                    pOutMsgBuffer, ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN + respTlvBytes);
            }
         }

         /* NO 'ELSE IF' since may be targeted for either. */
         if (zl303xx_PtpV2MgmtTlvTargetObject(managementId, ZL303XX_PTP_MGMT_TLV_TARGET_OBJ_PORT))
         {
            /* Pointer to the target Ports of this loop. */
            zl303xx_PortDataS *pLoopPtpPort = NULL;

            /* Loop through every available Port on this clock to gather data as
             * required and send responses (or ERRORS). */
            ZL303XX_PTP_FOR_EACH_PORT(&pPtpClock->portList, pLoopPtpPort)
            {
               /* Make sure the Port matches the targetPortIdentity. */
               if (zl303xx_PtpPortIdentityMatchesTarget(
                              &pLoopPtpPort->portDS.portIdentity,
                              &pPtpMsgRx->data.management.targetPortIdentity))
               {
                  /* Found a (another) Port that matches the target. */
                  respPortCount++;

                  respTlvBytes = zl303xx_PtpV2MgmtTlvProcessFunc(managementId)(
                                    pLoopPtpPort,     /* Handle the TLV on this Port. */
                                    &pMgmtHdrS->actionField,
                                    pInTlvBuf,
                                    pOutTlvBuf);

                  if (respTlvBytes != 0)
                  {
                     respMsgBytes = zl303xx_PtpV2MgmtTlvRespond(
                                          pPtpRxQueueData,
                                          &pPtpMsgTx,
                                          pLoopPtpPort,
                                          pOutMsgBuffer, ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN + respTlvBytes);
                  }
               }
            }  /* Port Loop. */
         }  /* PORT TARGET */

         /* If no Success response was sent, send a 'No Match' */
         if (respPortCount == 0)
         {
            respMsgBytes = zl303xx_PtpV2MgmtErrorTlvRespond(
                                 pPtpRxQueueData, &pPtpMsgTx, pRxPtpPort,
                                 ZL303XX_MGMT_ERR_GENERAL_ERROR, managementId,
                                 "targetPortIdentity not found");
         }

         /* This TLV has been handled one way or another. */
         numTlvBytesProcessed += (4 + tlvLength);

      }  /* Process Standard MANAGEMENT TLV or ERROR. */

      else
      {
      }

      /* We've processed this TLV (whether success or failure). */
      tlvBytesRemaining -= numTlvBytesProcessed;
      pInTlvBuf += numTlvBytesProcessed;
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
            "%s: Processed TLV bytes(%u), remaining(%u), Port matches(%u)",
            fnName, numTlvBytesProcessed, tlvBytesRemaining, respPortCount, 0,0);
   }  /* WHILE LOOP */

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
         "%s: MGMT TLV Message (numTLVs:%u)(bytes:%u)(Response:%u)",
         fnName, stackedTlvCount, numTlvBytesProcessed, respMsgBytes, 0,0);

   if (tlvBytesRemaining != 0)
   {
      /* Extra bytes left over for some reason. */
      /* Not an error but unusual. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
            "%s: WARNING: Mgmt TLV bytes unprocessed (%u)",
            fnName, tlvBytesRemaining, 0,0,0,0);
   }

   return numTlvBytesProcessed;
}  /* END zl303xx_PtpV2MgmtTlvHandle */

/*

  Function Name:
   zl303xx_PtpV2MgmtTlvInitiate

  Details:
   Initiates a Management TLV transmission (tlvType = MANAGEMENT (0x0001)).

   As per the IEEE-1588 Standard, Clause 15.4.1.7:
      Management messages shall be suffixed with zero or one TLV.

   The Unicast Negotiation TLV portion of the message must be provided in
   Stacked-TLV format. These will be transmitted individually or in Stacked-TLV
   format depending on:
      - The PTP port's unicastNegotiation.stackedTlvEn setting.
      - Combination of the Ports stackedTlvEn and the stackAllServicesOn1stRequest
        for initial contract startup.

   The PTP header & Signal Data portions are generated internally from the
   PtpStream configuration.

  Parameters:
   [in]  destAddr       The destination address to send the message to.
                              (May be NULL as long as pPtpStream is provided).
   [in]  pPtpStream     Pointer to the clock stream structure associated with
                              this connection. (May be NULL as long as destAddr
                              is provided).
   [in]  pTargetPortId  Port Identity of the targeted far-end port, if known.
                              If not provided, it is taken from the stream
                              history (if available, otherwise use all 1's).
   [in]  actionField    The Management action to perform (GET, SET, etc.).
   [in]  pMgmtTlvBuf    Pointer to the Management TLV buffer to send.
   [in]  mgmtTlvBufLen  Length of the Management TLV buffer (which is
                              redundant since it should be embedded in the buffer).

  Return Value:
   zlStatusE

  Notes:
   Assumes all input pointers are valid (for this internal routine).

*******************************************************************************/
zlStatusE zl303xx_PtpV2MgmtTlvInitiate(
      zl303xx_PortAddress *destAddr,
      zl303xx_ClockStreamS *pPtpStream,
      const zl303xx_PortIdentity *pTargetPortId,
      zl303xx_PtpV2MgmtActionE actionField,
      Uint8T *pMgmtTlvBuf, Uint8T mgmtTlvBufLen)
{
   zlStatusE status = ZL303XX_OK;
   Uint16T seqId;
   zl303xx_PtpTxStatusE txStatus = ZL303XX_PTP_TX_OK;

   const char *fnName = "zl303xx_PtpV2MgmtTlvInitiate";

   /* Pointer to the port & clock associated with this stream. */
   zl303xx_PortDataS *pPtpPort = NULL;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* To initiate a Management TLV, the stream must exist. */
   if (pPtpStream == NULL)
   {
      ZL303XX_TRACE_ALWAYS("%s:: Stream (%p) (== NULL)",
            fnName, (pPtpStream), 0,0,0,0);

      status = ZL303XX_INVALID_POINTER;
   }
   else  /* Set the Port & Clock data for this stream. */
   {
      pPtpPort = pPtpStream->portData;
      pPtpClock = pPtpPort->clock;
   }

   /* Build the PTP message Header. */
   if (status == ZL303XX_OK)
   {
      /* We have to pass a non-NULL pointer to the 'Pack' routine in order to
       * avoid the sequenceId from being incremented by mistake.    */
      seqId = pPtpStream->config.nextTxSequenceId[ZL303XX_MSG_ID_MANAGEMENT];

      /* Pack the PTP Message Header for a Management message.
       * The input TLV buffer may have multiple TLVs but, the PTP header will
       * always be the same (except for the messageLength & sequenceId fields
       * which will be set properly just before the buffer is transmitted).    */
      if (ZL303XX_PTP_V2_HEADER_LEN != zl303xx_PtpV2MsgHeaderPackForStream(
                                          pPtpStream,
                                          ZL303XX_MSG_ID_MANAGEMENT,
                                          &seqId,
                                          pPtpClock->msgObuf))
      {
         /* The Header was not constructed correctly. Display an error and quit. */
         ZL303XX_TRACE_ALWAYS("%s: Header Pack ERROR!", fnName, 0,0,0,0,0);

         status = ZL303XX_DATA_CORRUPTION;
      }
   }

   /* Add the Core Management Section. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpV2MsgManagementS mgmtDataS;

      /* For the targetPortIdentity member of the Management Data structure
       * if a port identity was passed into this function, use it. Otherwise,
       * use the parentIdentity from the stream's parentDS. */
      if (pTargetPortId != NULL)
      {
         mgmtDataS.targetPortIdentity = *pTargetPortId;
      }
      else
      {
         /* If a stream has the local clockIdentity as its far-end, then it does
          * not know the targetPortIdentity yet. In that case use all ones. */
         if (zl303xx_PtpV2EqualClockIdentities(
               pPtpStream->farEndParentDS.u.v2.parentPortIdentity.clockIdentity,
               pPtpStream->portData->clock->config.defaultDS.clockIdentity) == ZL303XX_TRUE)
         {
            OS_MEMSET(&mgmtDataS.targetPortIdentity, 0xFF, sizeof(mgmtDataS.targetPortIdentity));
         }
         else
         {
            mgmtDataS.targetPortIdentity = pPtpStream->farEndParentDS.u.v2.parentPortIdentity;
         }
      }

      /* Hops:
       *    When initiating a Management TLV, set the startingBoundaryHops = 255
       *    and the re-transmit value (boundaryHops) to the MAX as well.   */
      mgmtDataS.startingBoundaryHops = 255;
      mgmtDataS.boundaryHops = 255;
      mgmtDataS.actionField = actionField;

      /* Pack this data right after the header. */
      if (ZL303XX_PTP_V2_MANAGEMENT_DATA_LEN != zl303xx_PtpV2MsgDataMgmtPack(
                                                &mgmtDataS,
                                                (pPtpClock->msgObuf + ZL303XX_PTP_V2_HEADER_LEN)))
      {
         /* The Management Data was not constructed correctly. Display an error and quit. */
         ZL303XX_TRACE_ALWAYS("%s: Management Data Pack ERROR!", fnName, 0,0,0,0,0);

         status = ZL303XX_DATA_CORRUPTION;
      }
   }

   /* Add the Management TLV & send. */
   if (status == ZL303XX_OK)
   {
      /* Number of bytes contained in the current packet (HDR + SIG + TLV). */
      Uint16T pktBufferLength = ZL303XX_PTP_V2_HEADER_LEN + ZL303XX_PTP_V2_MANAGEMENT_DATA_LEN;

      /* Some managementId TLV have no TLV buffer in the transmission. */
      if ((pMgmtTlvBuf != NULL) &&
          (mgmtTlvBufLen > 0))
      {
         /* Copy the TLV buffer to the packet. */
         OS_MEMCPY((pPtpClock->msgObuf + pktBufferLength),
                (pMgmtTlvBuf),
                (mgmtTlvBufLen));

         /* Update the length of the buffer for this packet. */
         pktBufferLength += mgmtTlvBufLen;
      }

      /* Insert the total length of the message into the packet. */
      zl303xx_PtpV2MsgPktLengthSet(pktBufferLength, pPtpClock->msgObuf);

      /* Send the actual buffer. */
      {
         /* By specifying the destination address explicitly (passed in
          * parameter) this will override any value contained in the general
          * port connection structure */
         txStatus = zl303xx_PtpTxMsg(pPtpClock->msgObuf, pktBufferLength,
                     ZL303XX_PTP_INTERFACE_GENERAL, pPtpPort, destAddr,
                     pPtpStream, ZL303XX_MSG_ID_MANAGEMENT);

         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 2,
               "Stream %d: Initiating Management TLV (Seg:%d): Total (%d): TLV (%d); ",
               pPtpStream->clockStreamHandle, seqId,
               pktBufferLength, mgmtTlvBufLen, 0,0);

         if (txStatus != ZL303XX_PTP_TX_OK)
         {
            zl303xx_PtpPortLogFaultyState(txStatus, ZL303XX_PTP_PORT_FAULT_MGMT_SEND,
                                          pPtpPort);
            status = ZL303XX_PROTOCOL_ENGINE_ERROR;
            /* zl303xx_PtpV2ToPortState(pPtpPort, PTP_PORT_INT_STATE_FAULTY); */
         }
         else
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
                  "Stream %u event: Send %s Message",
                  pPtpStream->clockStreamHandle,
                  (const char*)"PTP_MANAGEMENT_MESSAGE",
                  0,0,0,0);
         }
      }
   }

   return status;
}  /* END zl303xx_PtpV2MgmtTlvInitiate */

/*

  Function Name:
   zl303xx_PtpV2MgmtTlvPrepareResponse

  Details:
   Routine to prepare a Management Response to a received Management message
   The received message may contain a TLV which is not processed as a part of
   this function (TLVs must be added by the main TLV processing routine).

   This routine prepares a response message buffer using the data contained in
   the received message.

  Parameters:
   [in]  pPtpMsgRx      PTP V2 received message header and management data
                              used to prepare the response message.
   [in]  pPtpPort       Pointer to the PTP Port structure associated with
                              this connection.

   [out] pPtpMsgTx      PTP V2 message header and management data of the
                              preliminary response message.
   [out] pOutMsgBuf     Pointer to a PTP message buffer (always the start of
                              the PTP message header).

  Return Value:
     Uint16T  Number of bytes packed (processed) into the buffer.
     0        An error occurred.

  Notes:
   Assumes all input pointers are valid.
   Packs only Header & Management Data, not any appended TLVs.

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvPrepareResponse(
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      zl303xx_PortDataS *pPtpPort,
      zl303xx_PtpV2MsgS *pPtpMsgTx,
      Uint8T *pOutMsgBuf)
{
   /* In general, copy the received Header and Management Data values into the
    * response structures. Most will be OK but some will be changed. */
   /* Will copy the following:
    *    - domainNumber
    *    - sequenceId   */
   *pPtpMsgTx = *pPtpMsgRx;

   /* PTP Header Specific Content. */
   {
      /* Force the following Header fields. */
      pPtpMsgTx->header.messageType = ZL303XX_MSG_ID_MANAGEMENT;
      pPtpMsgTx->header.versionPTP = ZL303XX_PTP_VERSION_2;
      pPtpMsgTx->header.messageLength = ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN;   /* Will be updated externally. */
      OS_MEMSET(&pPtpMsgTx->header.correctionField, 0x00, sizeof(pPtpMsgTx->header.correctionField));
      pPtpMsgTx->header.controlField = PTP_MANAGEMENT_MESSAGE;
      pPtpMsgTx->header.logMessageInterval = PTP_V2_LMMI_NULL_VALUE;

      /* flags:
       *    If MULTICAST Received, Respond MULTICAST, else UNICAST. */
      pPtpMsgTx->header.flagField &= PTP2_FLAGS_UNICAST;

      /* portIdentity:
       *    The PortId in the header is the ID of the port sending the response. */
      pPtpMsgTx->header.sourcePortIdentity = pPtpPort->portDS.portIdentity;
   }

   /* Management Data Specific Content. */
   {
      /* targetPortIdentity.
       *    The reply target is the source of the originator. */
      pPtpMsgTx->data.management.targetPortIdentity = pPtpMsgRx->header.sourcePortIdentity;

      /* startingBoundaryHops:
       *    Original (start) - (minus) the number remaining (boundaryHops) */
      pPtpMsgTx->data.management.startingBoundaryHops -= pPtpMsgTx->data.management.boundaryHops;

      /* boundaryHops:
       *    The number of remaining re-transmissions. This was set by the
       *    originator and decremented each time a message is re-transmitted. */
      /* Don't underflow or it will look like 255 more transmissions. */
      /* The zl303xx_PtpV2MsgMgmtHandle() routine checks if a reply can be re-issued. */
      if (pPtpMsgTx->data.management.boundaryHops > 0)
      {
         pPtpMsgTx->data.management.boundaryHops -= 1;
      }

      /* action field:
       *    Make the following assumptions about the action:
       *       - if RX = GET/SET; sent RSP
       *       - if RX = CMD; sent ACK
       *       - if RX = RSP/ACK; sent MAX (invalid value).  */
      switch (pPtpMsgRx->data.management.actionField)
      {
         case PTP_MANAGEMENT_ACTION_GET :
         case PTP_MANAGEMENT_ACTION_SET :
         {
            pPtpMsgTx->data.management.actionField = PTP_MANAGEMENT_ACTION_RSP;
            break;
         }
         case PTP_MANAGEMENT_ACTION_CMD :
         {
            pPtpMsgTx->data.management.actionField = PTP_MANAGEMENT_ACTION_ACK;
            break;
         }
         case PTP_MANAGEMENT_ACTION_RSP :
         case PTP_MANAGEMENT_ACTION_ACK :
         default :
         {
            pPtpMsgTx->data.management.actionField = PTP_MANAGEMENT_ACTION_MAX;
            break;
         }
      }
   }

   /* Pack the Response buffer. */
   /* return value should be ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN */
   return zl303xx_PtpV2MsgMgmtPack(&pPtpMsgTx->header, &pPtpMsgTx->data.management, pOutMsgBuf);
}  /* END zl303xx_PtpV2MgmtTlvPrepareResponse */

/*

  Function Name:
   zl303xx_PtpV2MgmtTlvRespond

  Details:
   Routine to send a Management Response buffer. The buffer includes the packed
   PTP Header, Management Data and appended TLV.

   Routine:
   - Checks the packed TLV length against the message buffer length for consistency.
   - Updates the packed PTP Message length.
   - Updates the packed PTP Message sourcePortIdentity.

  Parameters:
   [in]  pPtpMsgHeader  PTP V2 message header data used to build the packet.
   [in]  pInMgmtData    PTP Management Data used to build the packet. This
                              does not handle TLVs that may need to be appended.
   [in]  pPtpPort       Pointer to the PTP Port issuing the Response.
   [in]  pOutMsgBuf     Pointer to a PTP message buffer (always the start of
                              the PTP message header).
   [in]  respMsgBufLen  The response buffer length (bytes).

  Return Value:
     Uint16T  Number of bytes transmitted.
     0        An error occurred.

  Notes:
   Assumes all input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvRespond(
      zl303xx_PtpClockMsgQueueS *pPtpRxQueueData,
      zl303xx_PtpV2MsgS *pPtpTxMsgS,
      zl303xx_PortDataS *pPtpPort,
      Uint8T *pOutMsgBuf, Uint16T respMsgBufLen)
{
   /* Pointer to any appended TLV. */
   Uint8T *pOutTlvBuf = pOutMsgBuf + ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN;

   /* Variable for validating the expected TLV length and packed value. */
   Uint16T checkedTlvLen;
   zl303xx_PtpV2MgmtTlvTypeLengthIdGet(pOutTlvBuf, NULL, &checkedTlvLen, NULL);

   /* Check that the supplied TLV is of minimum length & the same as the packed value. */
   if ((respMsgBufLen < (ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN + ZL303XX_PTP_V2_MGMT_TLV_HEADER_LEN)) ||
       (respMsgBufLen != (ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN + checkedTlvLen + ZL303XX_PTP_TLV_BASE_LEN)))
   {
      ZL303XX_TRACE_ALWAYS(
            "zl303xx_PtpV2MgmtTlvRespond: ERROR: respMsgBufLen(%u): Packed TLV Length(%u)",
            respMsgBufLen, checkedTlvLen, 0,0,0,0);

      respMsgBufLen = 0;
   }

   /* Handle any Responses. */
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 3,
            "zl303xx_PtpV2MgmtTlvRespond: Response portNumber(%u): TLV Buffer Length(%u)",
            pPtpPort->portDS.portIdentity.portNumber, checkedTlvLen+ZL303XX_PTP_TLV_BASE_LEN, 0,0,0,0);

      /* Set the overall Reply message length. */
      pPtpTxMsgS->header.messageLength = respMsgBufLen;
      zl303xx_PtpV2MsgPktLengthSet(respMsgBufLen, pOutMsgBuf);

      /* Pack the sourcePortIdentity for this Port:
       * - the 'PrepareResponse' routine packs the Rx PortId which may not be
       *   source of the response in the ALL_PORTS case. */
      pPtpTxMsgS->header.sourcePortIdentity = pPtpPort->portDS.portIdentity;
      (void)zl303xx_PtpV2MsgPortIdentityPack(
                           &pPtpPort->portDS.portIdentity,
                           pOutMsgBuf + ZL303XX_PTP_V2_HDR_SRC_PORT_OFST);

      /* Send the message. */
      if (ZL303XX_PTP_TX_OK != zl303xx_PtpTxMsg(pOutMsgBuf, respMsgBufLen,
                           ZL303XX_PTP_INTERFACE_GENERAL, pPtpPort,
                           &pPtpRxQueueData->recvMsgInfo.srcAddr,
                           NULL, ZL303XX_MSG_ID_MANAGEMENT))
      {
         respMsgBufLen = 0;
      }
   }

   return respMsgBufLen;
}  /* END zl303xx_PtpV2MgmtTlvRespond */

/*

  Function Name:
   zl303xx_PtpV2MgmtErrorTlvRespond

  Details:
   Routine to send a Management Error Response buffer. The buffer includes the
   packed PTP Header, Management Data and appended TLV.

   Routine:
   - Packs all portions of the Management ERROR TLV Response.
   - Updates the packed PTP Message length.
   - Updates the packed PTP Message sourcePortIdentity.

  Parameters:
   [in]  pPtpRxQueueData Pointer to the PTP message queue structure. Contains
                              the type info & message buffer, including appended
                              TLVs, etc. (often the same as the packet buffer).
   [in]  pPtpMsgTx      The transmit message PTP Header and Data structure.
                              In this case, the Data portion is of
                              zl303xx_PtpV2MsgManagementS type.
   [in]  pPtpPort       Pointer to the PTP Port on which the message is to be
                              transmitted. Cannot be NULL.
   [in]  mgmtErrorId    The error value to return.
   [in]  mgmtTlvId      The managementID that caused the error.
   [in]  displayMsg     Pointer to a message string to be displayed.

  Return Value:
     Uint16T  Number of bytes transmitted.
     0        An error occurred.

  Notes:
   Assumes all input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtErrorTlvRespond(
      zl303xx_PtpClockMsgQueueS *pPtpRxQueueData,
      zl303xx_PtpV2MsgS *pPtpMsgTx,
      zl303xx_PortDataS *pPtpPort,
      zl303xx_ManagementErrorIdE mgmtErrorId,
      zl303xx_PtpV2MgmtTlvIdE mgmtTlvId,
      const char *displayMsg)
{
   /* Pointers to the message Buffer and the appended Error TLV. */
   Uint8T *pOutMsgBuf = pPtpPort->clock->msgObuf;
   Uint8T *pOutTlvBuf = pOutMsgBuf + ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN;

   /* Length variable for the message and appended TLV . */
   Uint16T msgLen = 0;
   Uint16T tlvLen = 0;

   /* Pack the Error TLV first so we know the overall message length. */
   tlvLen = zl303xx_PtpV2MgmtTlvErrorPack(pOutTlvBuf, mgmtErrorId, mgmtTlvId,
                     ((displayMsg == NULL) ? Zl303xx_MgmtErrorTlvStr(mgmtErrorId) : displayMsg));

   /* Pack the PTP Header and Management Data portion of the reply. */
   /* - Set the sourcePortIdentity of the reply with the PTP portIdentity
    *   provided. It is possible that the pPtpMsgTx contains the RX portIdentity
    *   but the transmitting Port is different (in the case of ALL_PORTs response). */
   pPtpMsgTx->header.sourcePortIdentity = pPtpPort->portDS.portIdentity;
   pPtpMsgTx->header.messageLength = ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN + tlvLen;

   /* Pack the rest of the message. */
   msgLen = zl303xx_PtpV2MsgMgmtPack(&pPtpMsgTx->header, &pPtpMsgTx->data.management, pOutMsgBuf);

   /* TRACE for logging */
   ZL303XX_TRACE_V2_PORT_ID(ZL303XX_MOD_ID_PTP_TLV, 1,
         "zl303xx_PtpV2MgmtErrorTlvRespond:  Tx Port", &pPtpPort->portDS.portIdentity);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
         "       : mgmtId(%u), mgmtErrorId(0x%04X)(%s), Tx TLV length(%u), msgLength(%d)",
         mgmtTlvId, mgmtErrorId, Zl303xx_MgmtErrorTlvStr(mgmtErrorId), tlvLen, msgLen, 0);

   /* Send the error Message. */
   return zl303xx_PtpV2MgmtTlvRespond(pPtpRxQueueData, pPtpMsgTx, pPtpPort, pOutMsgBuf, pPtpMsgTx->header.messageLength);
}  /* END zl303xx_PtpV2MgmtErrorTlvRespond */


/* zl303xx_PtpV2MgmtTlvIssue */
/** 

   Send a basic Management TLV with just the tlvType, length and managementId
   fields (which covers most issued Management TLV messages).

  Parameters:
   [in]  streamHandle   Handle of the stream to issue the message on.
   [in]  action         Management action to send with the TLV.
   [in]  managementId   Management TLV to issue.

  Return Value:
   zlStatusE

*******************************************************************************/
/* zlStatusE zl303xx_PtpV2MgmtTlvToAddrSend(
      zl303xx_PortAddress *destAddr,
      Uint8T *pMgmtTlvBuf, Uint8T mgmtTlvBufLen);
; */
zlStatusE zl303xx_PtpV2MgmtTlvIssue(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_PortIdentity *targetPortIdentity,
      zl303xx_PtpV2MgmtActionE action,
      Uint16T managementId)
{
   /* Basic Management TLV Buffer requires just 6 bytes. */
   Uint16T mgmtTlvBufLen = ZL303XX_PTP_V2_MGMT_TLV_HEADER_LEN;
   Uint8T pMgmtTlvBuf[ZL303XX_PTP_V2_MGMT_TLV_HEADER_LEN];

   /* tlvType = MANAGEMENT */
   PACK_16BIT((pMgmtTlvBuf + 0), PTP_TLV_MANAGEMENT);

   /* Although the buffer is 6-bytes, the lengthField stores only the number
    * of bytes after the lengthField.    */
   PACK_16BIT((pMgmtTlvBuf + 2), 2);

   /* The managementId provided. */
   PACK_16BIT((pMgmtTlvBuf + 4), managementId);

   return zl303xx_PtpV2MgmtTlvToStreamSend(
         streamHandle, targetPortIdentity, action, pMgmtTlvBuf, mgmtTlvBufLen);
}


/* zl303xx_PtpV2MgmtTlvToStreamSend */
/** 

   Send a Management TLV buffer with the specified action on the stream provided.

  Parameters:
   [in]  streamHandle   Handle of the stream to issue the message on.
   [in]  actionField    Management action (GET, SET, etc.).
   [in]  pMgmtTlvBuf    Management buffer to send.
   [in]  mgmtTlvBufLen  Length of the Management buffer to send.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_PtpV2MgmtTlvToStreamSend(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_PortIdentity *targetPortIdentity,
      zl303xx_PtpV2MgmtActionE actionField,
      Uint8T *pMgmtTlvBuf, Uint16T mgmtTlvBufLen)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtpV2MgmtTlvToStreamSend";

   /* Pointer to the Stream. */
   zl303xx_ClockStreamS * pPtpStream = NULL;

   /* Get the Stream Pointer. */
   if ((status = zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream)) != ZL303XX_OK )
   {
      ZL303XX_TRACE_ALWAYS("%s:: Stream (%p), does not exist)",
            fnName, streamHandle, 0,0,0,0);
   }
   /* To initiate a Unicast TLV, the stream must exist. */
   else if (pPtpStream == NULL)
   {
      ZL303XX_TRACE_ALWAYS("%s:: Stream (%p) (== NULL)",
            fnName, (pPtpStream), 0,0,0,0);

      status = ZL303XX_INVALID_POINTER;
   }
   /* Make sure the Management Action is correct. */
   else if (actionField >= PTP_MANAGEMENT_ACTION_MAX)
   {
      ZL303XX_TRACE_ALWAYS("%s:: Unsupported ACTION(%u)",
            fnName, actionField, 0,0,0,0);
      status = ZL303XX_UNSUPPORTED_OPERATION;
   }

   /* Send the buffer. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MgmtTlvInitiate(NULL, pPtpStream, targetPortIdentity,
                        actionField,
                        pMgmtTlvBuf, mgmtTlvBufLen);
   }

   return status;
}  /* END zl303xx_PtpV2MgmtTlvToStreamSend */

/*

  Function Name:
   zl303xx_PtpMgmtTlvHandleDefaultDSMembers

  Details:
   Processes a PTP Management Message related to the CLOCK's Default Data Set
   or one of its members and formating the appropriate response.
   - verifies that the requested ACTION is supported.
   - verifies the length of the received TLV for the given ACTION targeted for
     the specified member.
   - performs the required action by calling associated sub-routines:
     GET:   zl303xx_PtpMgmtTlvHandleDefaultDSMembersGet
     SET:   zl303xx_PtpMgmtTlvHandleDefaultDSMembersSet
   - returns any management errors generated.

  Parameters:
   [in]  pPtpClock      Pointer to the PTP Clock instance structure for which
                              the data is being read.
   [in]  actionField    The management action requested (GET, SET, etc.)
   [in]  memberId       The management ID of the data set member to get (may
                              be the entire data set as well).
   [in]  pInTlvDataBuf  The beginning of the data portion of the input TLV
                              buffer with the data to set.
   [in]  inTlvDataLen   The length of the data portion of the received TLV.

   [out] pOutTlvBuf     The packed outputTLV buffer with the required data.
                              This routine packs only the TLV data portion if
                              successful. If management errors are generated,
                              then the TLV header is also data-filled.
   [out] mgmtErrorLen   Length of the buffer of any management error return
                              message that may have been generated. This message
                              is already packed into the pOutTlvBuf.

  Return Value:
   Uint16T     Length of the prepared outTlvBuf (output TLV buffer) on success.
               0 if an error occurs.

  Notes:
   This routine should only be called if one of the following TLVs is received:
   - PTP2_MGMT_ID_DEFAULT_DATA_SET    (0x2000)
   - PTP2_MGMT_ID_PRIORITY1           (0x2005)
   - PTP2_MGMT_ID_PRIORITY2           (0x2006)
   - PTP2_MGMT_ID_DOMAIN              (0x2007)
   - PTP2_MGMT_ID_SLAVE_ONLY          (0x2008)
   - PTP2_MGMT_ID_CLOCK_ACCURACY      (0x2009)

   Assumes all input pointers are valid (for this local routine).

*******************************************************************************/
Uint16T zl303xx_PtpMgmtTlvHandleDefaultDSMembers(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpV2MgmtActionE actionField,
      zl303xx_PtpV2MgmtTlvIdE memberId,
      Uint8T *pInTlvDataBuf,
      Uint16T inTlvDataLen,
      Uint8T *pOutTlvBuf,
      Uint16T *mgmtErrorLen)
{
   /* Default return value. */
   Uint16T outTlvDataLen = 0;
   Uint16T inTlvDataLenRequired = 0;

   /* By default, assume no error will occur. */
   /* Used like the zlStatusE variable in this routine. */
   zl303xx_ManagementErrorIdE mgmtErrorId = ZL303XX_MGMT_ERR_NO_ERROR;

   *mgmtErrorLen = 0;

   /* Scope:
    * These parameters are CLOCK related so the PORT scope is ignored.
    * If we've made it here, the CLOCK scope is local or broadcast so continue. */

   /* Action:
    * The individual member parameters are allowed to GET or SET.
    * The entire Default Data Set is only allowed to GET according to
    * the standard but we allow SETs for some applications. */
   /* Data Length:
    * All GETs should have a Length of 0.
    * A SET of the entire Default Data Set should have a Length of 20.
    * All supported member SETs should have a Length of 2. */
   if (actionField == PTP_MANAGEMENT_ACTION_GET)
   {
      /* Data Length: All GETs should have a Length of 0. */
      inTlvDataLenRequired = 0;
   }
   else if (actionField == PTP_MANAGEMENT_ACTION_SET)
   {
      /* Data Length: Each SET has a define length. */
      if (memberId == PTP2_MGMT_ID_DEFAULT_DATA_SET)
      {
         inTlvDataLenRequired = 20;
      }
      else
      {
         inTlvDataLenRequired = 2;   /* All other member TLVs are 2 bytes. */
      }
   }
   else
   {
      /* Invalid request. Send an error response. */
      mgmtErrorId = ZL303XX_MGMT_ERR_NOT_SUPPORTED;
      *mgmtErrorLen = zl303xx_PtpV2MgmtTlvErrorPack(pOutTlvBuf, mgmtErrorId, memberId, "Unsupported Action for TLV");
   }

   /* Test the received TLV Length. */
   if (mgmtErrorId == ZL303XX_MGMT_ERR_NO_ERROR)
   {
      if (inTlvDataLen != inTlvDataLenRequired)
      {
         mgmtErrorId = ZL303XX_MGMT_ERR_WRONG_LENGTH;
         *mgmtErrorLen = zl303xx_PtpV2MgmtTlvErrorPack(pOutTlvBuf, mgmtErrorId, memberId, "Invalid TLV Length");
      }
   }

   /* Execute the requested Action. */
   if (mgmtErrorId == ZL303XX_MGMT_ERR_NO_ERROR)
   {
      if (actionField == PTP_MANAGEMENT_ACTION_SET)
      {
         outTlvDataLen = zl303xx_PtpMgmtTlvHandleDefaultDSMembersSet(pPtpClock, memberId, pInTlvDataBuf);

         /* On a successful SET, a GET operation will automatically follow. */
         if (outTlvDataLen == 0)
         {
            mgmtErrorId = ZL303XX_MGMT_ERR_NOT_SETABLE;
            *mgmtErrorLen = zl303xx_PtpV2MgmtTlvErrorPack_SetFailed(pOutTlvBuf, memberId);
         }
      }
   }

   /* Always do a GET as long as there have been no errors.
    * (Action can only be GET or SET and we do a GET for both operations). */
   if (mgmtErrorId == ZL303XX_MGMT_ERR_NO_ERROR)
   {
      /* The GET routine packs only the DATA portion of the output TLV so set
       * a dataBuffer just past the TLV header.  */
      Uint8T *pOutTlvDataBuf = (pOutTlvBuf + ZL303XX_PTP_V2_MGMT_TLV_HEADER_LEN);

      outTlvDataLen = zl303xx_PtpMgmtTlvHandleDefaultDSMembersGet(pPtpClock, memberId, pOutTlvDataBuf);

      /* Check if any error occurred. */
      if (outTlvDataLen == 0)
      {
        mgmtErrorId = ZL303XX_MGMT_ERR_NO_SUCH_ID;
        *mgmtErrorLen = zl303xx_PtpV2MgmtTlvErrorPack(pOutTlvBuf, mgmtErrorId, memberId, "Invalid TLV Management ID");
      }
   }

   return outTlvDataLen;
}  /* END zl303xx_PtpMgmtTlvHandleDefaultDSMembers */

/****************   STATIC FUNCTION DEFINITIONS   ****************************/

/*

  Function Name:
   zl303xx_PtpMgmtTlvHandleDefaultDSMembersGet

  Details:
   Processes a PTP Management Message related to getting the CLOCK's Default
   Data Set or one of its members and formatting the appropriate response.

  Parameters:
   [in]  pPtpClock      Pointer to the PTP Clock instance structure for which
                              the data is being read.
   [in]  memberId       The management ID of the data set member to get (may
                              be the entire data set as well).

   [out] pOutTlvDataBuf    The beginning of the data portion of the output
                                 TLV buffer with the required data.

  Return Value:
   Uint16T     Length of the prepared outTlvBuf (output TLV buffer) on success.
               0 if an error occurs.

  Notes:
   This routine should only be called if one of the following TLVs is received:
   - PTP2_MGMT_ID_DEFAULT_DATA_SET    (0x2000)
   - PTP2_MGMT_ID_PRIORITY1           (0x2005)
   - PTP2_MGMT_ID_PRIORITY2           (0x2006)
   - PTP2_MGMT_ID_DOMAIN              (0x2007)
   - PTP2_MGMT_ID_SLAVE_ONLY          (0x2008)
   - PTP2_MGMT_ID_CLOCK_ACCURACY      (0x2009)

   Assumes all input pointers are valid (for this local routine).

*******************************************************************************/
Uint16T zl303xx_PtpMgmtTlvHandleDefaultDSMembersGet(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpV2MgmtTlvIdE memberId,
      Uint8T *pOutTlvDataBuf)
{
   /* Default return value. */
   Uint16T outTlvBufLength = 0;

   /* If the entire Default Data Set is required... */
   if (memberId == PTP2_MGMT_ID_DEFAULT_DATA_SET)
   {
      /* Pack the output TLV buffer with the clock default Data Set. */
      outTlvBufLength = zl303xx_PtpV2MgmtTlvDefaultDSPack(&pPtpClock->config.defaultDS, pOutTlvDataBuf);
   }

   /* A single member is required */
   else
   {
      /* Clear the output buffer. All these TLVs are 2 bytes long with the
       * required data in the first byte and the second byte reserved. */
      outTlvBufLength = 2;
      OS_MEMSET (pOutTlvDataBuf, 0x00, outTlvBufLength);

      /* Do Not use a switch{} here since the case list has to include all possible
       * MGMT TLV names to avoid a compiler warning. */
      if (memberId == PTP2_MGMT_ID_PRIORITY1)
      {
         *pOutTlvDataBuf = pPtpClock->config.defaultDS.priority1;
      }
      else if (memberId == PTP2_MGMT_ID_PRIORITY2)
      {
         *pOutTlvDataBuf = pPtpClock->config.defaultDS.priority2;
      }
      else if (memberId == PTP2_MGMT_ID_DOMAIN)
      {
         *pOutTlvDataBuf = pPtpClock->config.defaultDS.domainNumber;
      }
      else if (memberId == PTP2_MGMT_ID_SLAVE_ONLY)
      {
         *pOutTlvDataBuf = (Uint8T)(pPtpClock->config.defaultDS.slaveOnly & 0x01);
      }
      else if (memberId == PTP2_MGMT_ID_CLOCK_ACCURACY)
      {
         *pOutTlvDataBuf = pPtpClock->config.defaultDS.clockQuality.clockAccuracy;
      }
      else
      {
         /* Can never happen but cover it anyway. */
         ZL303XX_TRACE_ALWAYS("Unknown DefaultDS memberId (managementId %d)",
               memberId, 0,0,0,0,0);

         outTlvBufLength = 0;
      }
   }

   return outTlvBufLength;
}  /* END zl303xx_PtpMgmtTlvHandleDefaultDSMembersGet */

/*

  Function Name:
   zl303xx_PtpMgmtTlvHandleDefaultDSMembersSet

  Details:
   Processes a PTP Management Message related to setting the CLOCK's Default
   Data Set or one of its members and formatting the appropriate response.

  Parameters:
   [in]  pPtpClock      Pointer to the PTP Clock instance structure for which
                              the data is being set.
   [in]  memberId       The management ID of the data set member to set (may
                              be the entire data set as well).
   [in]  pInTlvDataBuf  The beginning of the data portion of the input TLV
                              buffer with the data to set.

  Return Value:
   Uint16T     The length of the processed inTlvBuf (input TLV buffer).

  Notes:
   This routine should only be called if one of the following TLVs is received:
   - PTP2_MGMT_ID_DEFAULT_DATA_SET    (0x2000)
   - PTP2_MGMT_ID_PRIORITY1           (0x2005)
   - PTP2_MGMT_ID_PRIORITY2           (0x2006)
   - PTP2_MGMT_ID_DOMAIN              (0x2007)
   - PTP2_MGMT_ID_SLAVE_ONLY          (0x2008)
   - PTP2_MGMT_ID_CLOCK_ACCURACY      (0x2009)

   Assumes all input pointers are valid (for this local routine).

*******************************************************************************/
Uint16T zl303xx_PtpMgmtTlvHandleDefaultDSMembersSet(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpV2MgmtTlvIdE memberId,
      Uint8T *pInTlvDataBuf)
{
   /* Default return value. */
   Uint16T processedTlvBufLength = 0;

   /* Local parameter for setting clock data. */
   zl303xx_DefaultDS newDefaultDS;

   /* If an entire Default Data Set update is required... */
   if (memberId == PTP2_MGMT_ID_DEFAULT_DATA_SET)
   {
      /* Unpack the input TLV buffer into the local default Data Set structure. */
      processedTlvBufLength = zl303xx_PtpV2MgmtTlvDefaultDSUnpack(pInTlvDataBuf, &newDefaultDS);
   }

   /* Update the single member. */
   else
   {
      /* Copy the clock's current default data set to the local structure. */
      newDefaultDS = pPtpClock->config.defaultDS;

      /* All these TLVs are 2 bytes long with the required data in the first
       * byte and the second byte reserved. */
      processedTlvBufLength = 2;

      /* Do Not use a switch{} here since the case list has to include all possible
       * MGMT TLV names to avoid a compiler warning. */
      if (memberId == PTP2_MGMT_ID_PRIORITY1)
      {
         newDefaultDS.priority1 = *pInTlvDataBuf;
      }
      else if (memberId == PTP2_MGMT_ID_PRIORITY2)
      {
         newDefaultDS.priority2 = *pInTlvDataBuf;
      }
      else if (memberId == PTP2_MGMT_ID_DOMAIN)
      {
         newDefaultDS.domainNumber = *pInTlvDataBuf;
      }
      else if (memberId == PTP2_MGMT_ID_SLAVE_ONLY)
      {
         newDefaultDS.slaveOnly = (zl303xx_BooleanE)(*pInTlvDataBuf & 0x01);
      }
      else if (memberId == PTP2_MGMT_ID_CLOCK_ACCURACY)
      {
         newDefaultDS.clockQuality.clockAccuracy = *pInTlvDataBuf;
      }
      else
      {
         /* Can never happen but cover it anyway. */
         ZL303XX_TRACE_ALWAYS("Unknown DefaultDS memberId (managementId %d)",
               memberId, 0,0,0,0,0);

         processedTlvBufLength = 0;
      }
   }

   if (processedTlvBufLength != 0)
   {
      if (ZL303XX_OK != zl303xx_PtpClockConfigureDefaultDataSet(pPtpClock, &newDefaultDS, ZL303XX_FALSE))
      {
         /* For errors, set the processed length to 0 so an error TLV will be sent.  */
         processedTlvBufLength = 0;
      }
   }

   return processedTlvBufLength;
}  /* END zl303xx_PtpMgmtTlvHandleDefaultDSMembersSet */


/**************************************************************************/
/**** UTILITY ROUTINES used for gathering the required data used in the ***/
/**** reply to a MANAGEMENT TLVs request.                               ***/
/**************************************************************************/

/**************** NULL MANAGEMENT Utility Routines ************************/

/* zl303xx_PtpV2MgmtTlvNullHandle */
/** 

   Routine for handling the NULL management TLV. This routine will process
   the TLV based on the received action and call sub-processing routines
   if needed.

  Parameters:
   [in]  pPtpPort       Pointer to the PTP Port for which to process the
                              TLV (not necessarily the received Port).
   [in]  actionField    The management action to perform on the TLV.
   [in]  pInTlvBuf      Pointer to the received TLV buffer (begins at the
                              tlvType field in the TLV Header).

   [out] pOutTlvBuf     Pointer to any response TLV buffer (if response, or
                              ERROR is to be sent back).

  Return Value:  Uint16T  Size (in bytes) of the response TLV or '0' if no response
                  is required.

  Notes:
   Assumes input pointers and parameters have already been checked.
   Assumes the PTP Header, Management Data and TLV header (type, length and
      managementId) sections of the reply message have already been data filled.
      This routines packs only the data section of the TLV buffer and updates
      the tlvLength field (it also has free reign over the entire buffer as
      required, in case of other required modifications or ERRORs).

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvNullHandle(
      zl303xx_PortDataS *pPtpPort,
      zl303xx_PtpV2MgmtActionE *actionField,
      Uint8T *pInTlvBuf,
      Uint8T *pOutTlvBuf)
{
   /* Default return value. */
   Uint16T tlvRespBufferLen = 0;
   const char *fnName = "zl303xx_PtpV2MgmtTlvNullHandle";

   /* TLV Header Fields. */
   Uint16T tlvType = ZL303XX_MSG_ID_MANAGEMENT;
   Uint16T tlvLength = 0;
   Uint16T managementId = PTP2_MGMT_ID_NULL;

   /* Get the type, length and managementId fields for verification purposes. */
   zl303xx_PtpV2MgmtTlvTypeLengthIdGet(pInTlvBuf, &tlvType, &tlvLength, &managementId);

   /* Log the entry to the routine with all relevant data to reduce logging below.
    * (Just log errors below). */
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
         "%s: Port(%u): action(%u): T/L/ID(%u/%u/0x%04X)",
         fnName, pPtpPort->portDS.portIdentity.portNumber,
         *actionField,
         tlvType, tlvLength, managementId);

   /* Process any Request. */
   if ((*actionField == PTP_MANAGEMENT_ACTION_GET) ||
       (*actionField == PTP_MANAGEMENT_ACTION_SET) ||
       (*actionField == PTP_MANAGEMENT_ACTION_CMD))
   {
      /* Check the received tlvLength field. */
      if (zl303xx_PtpV2MgmtTlvCheckRxLength(managementId, tlvLength) == ZL303XX_FALSE)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
               "%s: ERROR: managementId(0x%04X), length(%u), expect(%u)",
               fnName, managementId, tlvLength,
               zl303xx_PtpV2MgmtTlvExpectedRxLength(managementId), 0,0);

         /* The lengthField is set internally. */
         tlvRespBufferLen = zl303xx_PtpV2MgmtTlvErrorPack_InvalidLength(pOutTlvBuf, managementId);
      }

      /* Pack the required data (or ERROR) for the targeted Port. */
      else
      {
         /* For the NULL Management response, there is no data in the DATA
          * section of the TLV, just the TLV Header is required. */
         tlvRespBufferLen = ZL303XX_PTP_V2_MGMT_TLV_HEADER_LEN;

         /* Set the TLV lengthField = packedLength + 2-byte managementId field. */
         tlvLength = 2;
         zl303xx_PtpV2MgmtTlvTypeLengthIdSet(&tlvType, &tlvLength, &managementId, pOutTlvBuf);
      }
   }

   /* Process any Reply */
   else if ((*actionField == PTP_MANAGEMENT_ACTION_RSP) ||
            (*actionField == PTP_MANAGEMENT_ACTION_ACK))
   {
      /* Routine entry log is sufficient, no reply is required. */
      tlvRespBufferLen = 0;
   }

   else
   {
      /* This should have been checked in zl303xx_PtpV2MgmtTlvHandle(). If we
       * have hit this point then perhaps the definitions array (MgmtTlvDef) in
       * zl303xx_PtpV2MgmtTlvDef.h needs to be updated. */
      ZL303XX_TRACE_ALWAYS(
            "%s: Unsupported Action(%u) for managementId(0x%04X)",
            fnName, *actionField, managementId, 0,0,0);

      /* Invalid Action. Return an error response. */
      tlvRespBufferLen = zl303xx_PtpV2MgmtTlvErrorPack_InvalidAction(pOutTlvBuf, managementId);
   }

   return tlvRespBufferLen;
}  /* END zl303xx_PtpV2MgmtTlvNullHandle */

/**************** CLOCK DESCRIPTION Utility Routines **********************/

/* zl303xx_PtpV2MgmtTlvClockDescrHandle */
/** 

   Routine for handling the CLOCK DESCRIPTION management TLV. This routine will
   process the TLV based on the received action and call sub-processing routines
   if needed.

  Parameters:
   [in]  pPtpPort       Pointer to the PTP Port for which the TLV will be
                              applied (data gathered, configured or command
                              executed).
   [in]  actionField    Action to perform with the TLV.
   [in]  pInTlvBuf      Pointer to the received TLV buffer (begins at the
                              tlvType field in the TLV Header).

   [out] pOutTlvBuf     Pointer to any response TLV buffer (if response, or
                              ERROR is to be sent back).

  Return Value:  Uint16T  Size (in bytes) of the response TLV or '0' if no response
                  is required.

  Notes:
   Assumes input pointers and parameters have already been checked.
   Assumes the PTP Header, Management Data and TLV header (type, length and
      managementId) sections of the reply message have already been data filled.
      This routines packs only the data section of the TLV buffer and updates
      the tlvLength field (it also has free reign over the entire buffer as
      required, in case of other required modifications or ERRORs).

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvClockDescrHandle(
      zl303xx_PortDataS *pPtpPort,
      zl303xx_PtpV2MgmtActionE *actionField,
      Uint8T *pInTlvBuf,
      Uint8T *pOutTlvBuf)
{
   /* Default return value. */
   Uint16T tlvRespBufferLen = 0;
   const char *fnName = "zl303xx_PtpV2MgmtTlvClockDescrHandle";

   /* TLV Header Fields. */
   Uint16T tlvType = ZL303XX_MSG_ID_MANAGEMENT;
   Uint16T tlvLength = 0;
   Uint16T managementId = PTP2_MGMT_ID_CLOCK_DESCRIPTION;

   /* Pointer to the first octet of the TLV data buffer. */
   Uint8T *pOutTlvBufData = pOutTlvBuf + ZL303XX_PTP_V2_MGMT_TLV_HEADER_LEN;

   /* Data type for the CLOCK_DESCRIPTION data. */
   zl303xx_ClockDescriptionS clkDescrS;

   /* Get the type, length and managementId fields for verification purposes. */
   zl303xx_PtpV2MgmtTlvTypeLengthIdGet(pInTlvBuf, &tlvType, &tlvLength, &managementId);

   /* Log the entry to the routine with all relevant data to reduce logging below.
    * (Just log errors below). */
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
         "%s: Port(%u): action(%u): T/L/ID(%u/%u/0x%04X)",
         fnName, pPtpPort->portDS.portIdentity.portNumber,
         *actionField,
         tlvType, tlvLength, managementId);

   if (*actionField == PTP_MANAGEMENT_ACTION_GET)
   {
      /* Check the received tlvLength field. */
      if (zl303xx_PtpV2MgmtTlvCheckRxLength(managementId, tlvLength) == ZL303XX_FALSE)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
               "%s: ERROR: managementId(0x%04X), length(%u), expect(%u)",
               fnName, managementId, tlvLength,
               zl303xx_PtpV2MgmtTlvExpectedRxLength(managementId), 0,0);

         /* The lengthField is set internally. */
         tlvRespBufferLen = zl303xx_PtpV2MgmtTlvErrorPack_InvalidLength(pOutTlvBuf, managementId);
      }

      /* Pack the required data (or ERROR) for the targeted Port. */
      else
      {
         if (zl303xx_PtpV2MgmtTlvClockDescrGather(pPtpPort, &clkDescrS) == ZL303XX_OK)
         {
            /* This Pack routine will always return >0 bytes. */
            tlvRespBufferLen = zl303xx_PtpV2MgmtTlvClockDescrPack(&clkDescrS, pOutTlvBufData);
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 3,
                  "%s: portNumber(%u): Packed TLV Data size(%u)",
                  fnName, pPtpPort->portDS.portIdentity.portNumber, tlvRespBufferLen, 0,0,0);

            /* Set the TLV lengthField = packedLength + 2-byte managementId field. */
            tlvLength = tlvRespBufferLen + 2;
            zl303xx_PtpV2MgmtTlvTypeLengthIdSet(&tlvType, &tlvLength, &managementId, pOutTlvBuf);

            /* Return the full length of the TLV buffer. */
            tlvRespBufferLen += ZL303XX_PTP_V2_MGMT_TLV_HEADER_LEN;
         }

         /* An error occurred when gathering the data. */
         else
         {
            /* The lengthField is set internally. */
            tlvRespBufferLen = zl303xx_PtpV2MgmtTlvErrorPack(pOutTlvBuf,
                                          ZL303XX_MGMT_ERR_WRONG_VALUE,
                                          managementId,
                                          "GET Clock Description ERROR: Port Data unavailable");
         }
      }
   }

   else if (*actionField == PTP_MANAGEMENT_ACTION_RSP)
   {
      /* Since only the GET action is allowed for this TLV, a RESPONSE should
       * have at least 22 bytes of data (roughly based on Table 41 of the 1588 Std).
       * If the message looks too short, display a local error (do not respond);
       * otherwise display the appended data.  */
      if (tlvLength > 2)
      {
         ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_TLV, 2)
         {
            zl303xx_PtpV2MgmtTlvClockDescrUnpack(pInTlvBuf + ZL303XX_PTP_V2_MGMT_TLV_HEADER_LEN, &clkDescrS);
            zl303xx_PtpV2MgmtTlvClockDescrTrace(&clkDescrS);
         }
      }
      else
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
               "%s: ERROR: portNumber(%u): RSP TLV Data size too short(%u)",
               fnName, pPtpPort->portDS.portIdentity.portNumber, tlvLength, 0,0,0);
      }

      /* No Reply required. */
      tlvRespBufferLen = 0;
   }

   else
   {
      /* This should have been checked in zl303xx_PtpV2MgmtTlvHandle(). If we
       * have hit this point then perhaps the definitions array (MgmtTlvDef) in
       * zl303xx_PtpV2MgmtTlvDef.h needs to be updated. */
      ZL303XX_TRACE_ALWAYS(
            "%s: Unsupported Action(%u) for managementId(0x%04X)",
            fnName, *actionField, managementId, 0,0,0);

      /* Invalid Action. Return an error response. */
      tlvRespBufferLen = zl303xx_PtpV2MgmtTlvErrorPack_InvalidAction(pOutTlvBuf, managementId);
   }

   return tlvRespBufferLen;
}  /* END zl303xx_PtpV2MgmtTlvClockDescrHandle */

/* zl303xx_PtpV2MgmtTlvClockDescrGather */
/** 

   Gathers the required data for a reply to the CLOCK DESCRIPTION management
   TLV (Port based but requires some Clock & Node data).

  Parameters:
   [in]  pPtpPort    Pointer to the PTP Port for which to gather the data.

   [out] pClkDescrS  Pointer to the CLOCK DESCRIPTION data structure to
                           return the gathered data.

  Return Value:  ZL303XX_OK

  Notes:
   Assumes input pointers have already been checked.

*******************************************************************************/
zlStatusE zl303xx_PtpV2MgmtTlvClockDescrGather(
               zl303xx_PortDataS *pPtpPort,
               zl303xx_ClockDescriptionS *pClkDescrS)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = pPtpPort->clock;

   if (status == ZL303XX_OK)
   {
      /* Clear the data structure before filling (since it contains some string arrays). */
      OS_MEMSET(pClkDescrS, 0x00, sizeof(zl303xx_ClockDescriptionS));

      /* clockType */
      pClkDescrS->clockType = pPtpClock->clockType;

      /* Physical address information */
      status = zl303xx_PtpPortPhysicalAddrGet(pPtpPort,
                                             pClkDescrS->physicalLayerProtocol,
                                             &pClkDescrS->physicalAddressLength,
                                             pClkDescrS->physicalAddress);
   }

   if (status == ZL303XX_OK)
   {
      /* protocolAddress */
      pClkDescrS->protocolAddress = pPtpPort->config.localAddr;

      /* Other product data. */
      {
         Uint8T *manufacturerIdentity = NULL;
         char *productDescription = NULL, *revisionData = NULL, *userDescription = NULL;

         zl303xx_PtpNodeDescriptionGet(&manufacturerIdentity,
                                       &productDescription,
                                       &revisionData,
                                       &userDescription);

         /* manufacturerIdentity */
         OS_MEMCPY(pClkDescrS->manufacturerIdentity,
                manufacturerIdentity,
                ZL303XX_PTP_NODE_OUI_LEN);

         /* productDescription */
         OS_MEMCPY(pClkDescrS->productDescription,
                productDescription,
                MIN_LEN(OS_STRLEN(productDescription), ZL303XX_PTP_NODE_PROD_DESC_LEN));

         /* revisionData */
         OS_MEMCPY(pClkDescrS->revisionData,
                revisionData,
                MIN_LEN(OS_STRLEN(revisionData), ZL303XX_PTP_NODE_REV_DATA_LEN));

         /* userDescription */
         OS_MEMCPY(pClkDescrS->userDescription,
                userDescription,
                MIN_LEN(OS_STRLEN(userDescription), ZL303XX_PTP_NODE_USER_DESC_LEN));
      }

      /* profileIdentity */
      if (pPtpClock->profile != NULL)
      {
         OS_MEMCPY(pClkDescrS->profileIdentity,
                pPtpClock->profile->profileIdentifier,
                PTP_PROFILE_IDENTITY_LENGTH);
      }
   }

   ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_TLV, ((status == ZL303XX_OK) ? 3 : 1))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
            "zl303xx_PtpV2MgmtTlvClockDescrGather: Port(%u): Status(%u)",
            pPtpPort->portDS.portIdentity.portNumber, status, 0,0,0,0);
      zl303xx_PtpV2MgmtTlvClockDescrTrace(pClkDescrS);
   }

   return status;
}  /* END zl303xx_PtpV2MgmtTlvClockDescrGather */

/* zl303xx_PtpV2MgmtTlvClockDescrTrace */
/** 

   Traces (Logs) the data associated with the CLOCK DESCRIPTION management TLV.

  Parameters:
   [in]  pClkDescrS  Pointer to the CLOCK DESCRIPTION data structure to TRACE.

  Return Value:  ZL303XX_OK

  Notes:
   Assumes input pointer has already been checked.

*******************************************************************************/
zlStatusE zl303xx_PtpV2MgmtTlvClockDescrTrace(
               zl303xx_ClockDescriptionS *pClkDescrS)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE_ALWAYS("==============================================", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("zl303xx_PtpV2MgmtTlvClockDescrTrace:", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("==============================================", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("               clockType: 0x%04X", pClkDescrS->clockType, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("   physicalLayerProtocol: %s", pClkDescrS->physicalLayerProtocol, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("   physicalAddressLength: %u", pClkDescrS->physicalAddressLength, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("         physicalAddress: %02X:%02X:%02X:%02X:%02X:%02X",
                                    pClkDescrS->physicalAddress[0],
                                    pClkDescrS->physicalAddress[1],
                                    pClkDescrS->physicalAddress[2],
                                    pClkDescrS->physicalAddress[3],
                                    pClkDescrS->physicalAddress[4],
                                    pClkDescrS->physicalAddress[5]);
   ZL303XX_TRACE_ALWAYS("         protocolAddress: networkProtocol %u", pClkDescrS->protocolAddress.networkProtocol, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("         protocolAddress:   addressLength %u", pClkDescrS->protocolAddress.addressLength, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("         protocolAddress:    addressField %02X:%02X:%02X:%02X",
                                    pClkDescrS->protocolAddress.addressField[0],
                                    pClkDescrS->protocolAddress.addressField[1],
                                    pClkDescrS->protocolAddress.addressField[2],
                                    pClkDescrS->protocolAddress.addressField[3], 0,0);
   ZL303XX_TRACE_ALWAYS("    manufacturerIdentity: %02X:%02X:%02X",
                                    pClkDescrS->manufacturerIdentity[0],
                                    pClkDescrS->manufacturerIdentity[1],
                                    pClkDescrS->manufacturerIdentity[2], 0,0,0);
   ZL303XX_TRACE_ALWAYS("      productDescription: %s", pClkDescrS->productDescription, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("            revisionData: %s", pClkDescrS->revisionData, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("         userDescription: %s", pClkDescrS->userDescription, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("         profileIdentity: %02X:%02X:%02X:%02X:%02X:%02X",
                                    pClkDescrS->profileIdentity[0],
                                    pClkDescrS->profileIdentity[1],
                                    pClkDescrS->profileIdentity[2],
                                    pClkDescrS->profileIdentity[3],
                                    pClkDescrS->profileIdentity[4],
                                    pClkDescrS->profileIdentity[5]);
   ZL303XX_TRACE_ALWAYS("==============================================", 0,0,0,0,0,0);

   return status;
}  /* END zl303xx_PtpV2MgmtTlvClockDescrTrace */

/**************** CLOCK DESCRIPTION Utility Routines **********************/

/* zl303xx_PtpV2MgmtTlvUserDescrHandle */
/** 

   Routine for handling the USER DESCRIPTION management TLV. This routine will
   process the TLV based on the received action and call sub-processing routines
   if needed.

  Parameters:
   [in]  pPtpPort       Pointer to the PTP Port for which to process the
                              TLV (not necessarily the received Port).
   [in]  actionField    The management action to perform on the TLV.
   [in]  pInTlvBuf      Pointer to the received (raw) TLV data.

   [out] pOutTlvBuf     Pointer to any response TLV buffer (if response, or
                              ERROR is to be sent back).

  Return Value:  Uint16T  Size (in bytes) of the response TLV or '0' if no response
                  is required.

  Notes:
   Assumes input pointers and parameters have already been checked.
   Assumes the PTP Header, Management Data and TLV header (type, length and
      managementId) sections of the reply message have already been data filled.
      This routines packs only the data section of the TLV buffer and updates
      the tlvLength field (it also has free reign over the entire buffer as
      required, in case of other required modifications or ERRORs).

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvUserDescrHandle(
      zl303xx_PortDataS *pPtpPort,
      zl303xx_PtpV2MgmtActionE *actionField,
      Uint8T *pInTlvBuf,
      Uint8T *pOutTlvBuf)
{
   /* Default return value. */
   Uint16T tlvRespBufferLen = 0;
   const char *fnName = "zl303xx_PtpV2MgmtTlvUserDescrHandle";

   /* TLV Header Fields. */
   Uint16T tlvType = ZL303XX_MSG_ID_MANAGEMENT;
   Uint16T tlvLength = 0;
   Uint16T managementId = PTP2_MGMT_ID_USER_DESCRIPTION;

   /* Pointer to the first octet of the TLV data buffer (In/Out). */
   Uint8T *pOutTlvBufData = pOutTlvBuf + ZL303XX_PTP_V2_MGMT_TLV_HEADER_LEN;
   Uint8T *pInTlvBufData = pInTlvBuf + ZL303XX_PTP_V2_MGMT_TLV_HEADER_LEN;

   /* Data type for the USER_DESCRIPTION data. */
   char userDescription[ZL303XX_PTP_NODE_USER_DESC_LEN];

   /* Get the type, length and managementId fields for verification purposes. */
   zl303xx_PtpV2MgmtTlvTypeLengthIdGet(pInTlvBuf, &tlvType, &tlvLength, &managementId);

   /* Log the entry to the routine with all relevant data to reduce logging below.
    * (Just log errors below). */
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
         "%s: Port(%u): action(%u): T/L/ID(%u/%u/0x%04X)",
         fnName, pPtpPort->portDS.portIdentity.portNumber,
         *actionField,
         tlvType, tlvLength, managementId);

   if (*actionField == PTP_MANAGEMENT_ACTION_GET)
   {
      /* Check the received tlvLength field. */
      if (zl303xx_PtpV2MgmtTlvCheckRxLength(managementId, tlvLength) == ZL303XX_FALSE)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
               "%s: ERROR: managementId(0x%04X), length(%u), expect(%u)",
               fnName, managementId, tlvLength,
               zl303xx_PtpV2MgmtTlvExpectedRxLength(managementId), 0,0);

         /* The lengthField is set internally. */
         tlvRespBufferLen = zl303xx_PtpV2MgmtTlvErrorPack_InvalidLength(pOutTlvBuf, managementId);
      }

      /* Pack the required data (or ERROR) for the targeted Port. */
      else
      {
         char *userDescriptionTx = NULL;

         /* GATHER the data. */
         zl303xx_PtpNodeDescriptionGet(NULL, NULL, NULL, &userDescriptionTx);

         /* Pack the string into the TX TLV. */
         /* This string should always be >0 bytes since it has 1 semi-colon ';' character. */
         tlvRespBufferLen = zl303xx_PtpV2MsgTextPack(userDescriptionTx, pOutTlvBufData,
                                                     ZL303XX_PTP_NODE_USER_DESC_LEN);

         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 3,
               "%s: portNumber(%u): Packed TLV Data size(%u)",
               fnName, pPtpPort->portDS.portIdentity.portNumber, tlvRespBufferLen, 0,0,0);

         /* Set the TLV lengthField = packedLength + 2-byte managementId field. */
         tlvLength = tlvRespBufferLen + 2;
         zl303xx_PtpV2MgmtTlvTypeLengthIdSet(&tlvType, &tlvLength, &managementId, pOutTlvBuf);

         /* Return the full length of the TLV buffer. */
         tlvRespBufferLen += ZL303XX_PTP_V2_MGMT_TLV_HEADER_LEN;
      }
   }

   else if (*actionField == PTP_MANAGEMENT_ACTION_SET)
   {
      tlvRespBufferLen = 0;
   }

   else if (*actionField == PTP_MANAGEMENT_ACTION_RSP)
   {
      /* Extract the string from the RX TLV. */
      /* This string should always be >0 bytes since it has 1 semi-colon ';' character. */
      tlvRespBufferLen = zl303xx_PtpV2MsgTextUnpack(pInTlvBufData, userDescription,
                                                  ZL303XX_PTP_NODE_USER_DESC_LEN);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
            "%s: Received on portNumber(%u): (%s)",
            fnName, pPtpPort->portDS.portIdentity.portNumber, userDescription, 0,0,0);

      /* No Reply required. */
      tlvRespBufferLen = 0;
   }

   else
   {
      /* This should have been checked in zl303xx_PtpV2MgmtTlvHandle(). If we
       * have hit this point then perhaps the definitions array (MgmtTlvDef) in
       * zl303xx_PtpV2MgmtTlvDef.h needs to be updated. */
      ZL303XX_TRACE_ALWAYS(
            "%s: Unsupported Action(%u) for managementId(0x%04X)",
            fnName, *actionField, managementId, 0,0,0);

      /* Invalid Action. Return an error response. */
      tlvRespBufferLen = zl303xx_PtpV2MgmtTlvErrorPack_InvalidAction(pOutTlvBuf, managementId);
   }

   return tlvRespBufferLen;
}  /* END zl303xx_PtpV2MgmtTlvUserDescrHandle */


/**************************************************************************/
/**** UTILITY ROUTINES used for processing MANAGEMENT TLVs (and in some ***/
/**** cases, Signaling TLVs).                                           ***/
/**************************************************************************/

/* zl303xx_PtpPortIdentityMatchesTarget */
/** 

   Utility routine to check if a particular Port Identity meets the conditions
   of a provided targetPortIdentity (from a Signaling or Management Message) as
   per Tables 32 & 36 of IEEE-1588-2008.

  Parameters:
   [in]  pPortIdentity        Pointer to the Port Identity to test.
   [in]  pTargetPortIdentity  Pointer to the received targetPortIdentity from
                                    a Signaling or Management Message.

  Return Value:  ZL303XX_TRUE: Conditions are met.
         ZL303XX_FALSE: Otherwise.

  Notes:
   Assumes input pointers have already been checked.

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtpPortIdentityMatchesTarget(
      zl303xx_PortIdentity *pPortIdentity,
      zl303xx_PortIdentity *pTargetPortIdentity)
{
   /* Default return value. */
   zl303xx_BooleanE bFitsTargetPortMask = ZL303XX_FALSE;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 5,
         "zl303xx_PtpPortIdentityMatchesTarget: ", 0,0,0,0,0,0);
   ZL303XX_TRACE_V2_PORT_ID(ZL303XX_MOD_ID_PTP_TLV, 5,
         "        pPortIdentity:", pPortIdentity);
   ZL303XX_TRACE_V2_PORT_ID(ZL303XX_MOD_ID_PTP_TLV, 5,
         "  pTargetPortIdentity:", pTargetPortIdentity);

   /* Row 1 & 2 (of either of the IEEE Tables) */
   if ((zl303xx_PtpV2EqualClockIdentities(pPortIdentity->clockIdentity, pTargetPortIdentity->clockIdentity)) ||
       (zl303xx_PtpV2IsAllClockIdentity(pTargetPortIdentity->clockIdentity)))
   {
      if ((pTargetPortIdentity->portNumber == (Uint16T)(-1)) ||   /* ALL_PORTS */
          (pTargetPortIdentity->portNumber == pPortIdentity->portNumber))
      {
         bFitsTargetPortMask = ZL303XX_TRUE;
      }
   }

   return bFitsTargetPortMask;
}  /* END zl303xx_PtpPortIdentityMatchesTarget */

/* zl303xx_PtpClockIdentityMatchesTarget */
/** 

   Utility routine to check if a particular Clock Identity meets the conditions
   of a provided targetPortIdentity (from a Signaling or Management Message) as
   per Tables 32 & 36 of IEEE-1588-2008.

  Parameters:
   [in]  pClockIdentity       Pointer to the Clock Identity to test.
   [in]  pTargetPortIdentity  Pointer to the received targetPortIdentity from
                                    a Signaling or Management Message.

  Return Value:  ZL303XX_TRUE: Conditions are met.
         ZL303XX_FALSE: Otherwise.

  Notes:
   Assumes input pointers have already been checked.

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtpClockIdentityMatchesTarget(
      zl303xx_ClockIdentity pClockIdentity,
      zl303xx_PortIdentity *pTargetPortIdentity)
{
   /* Default return value. */
   zl303xx_BooleanE bFitsTargetPortMask = ZL303XX_FALSE;

   /* Row 1 & 2 (of either of the IEEE Tables) */
   if ((zl303xx_PtpV2EqualClockIdentities(pClockIdentity, pTargetPortIdentity->clockIdentity)) ||
       (zl303xx_PtpV2IsAllClockIdentity(pTargetPortIdentity->clockIdentity)))
   {
      bFitsTargetPortMask = ZL303XX_TRUE;
   }

   return bFitsTargetPortMask;
}  /* END zl303xx_PtpClockIdentityMatchesTarget */


/****************   END   ****************************************************/
