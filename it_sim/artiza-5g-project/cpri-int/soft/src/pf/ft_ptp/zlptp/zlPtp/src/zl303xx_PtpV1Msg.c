

/*****************************************************************************
*
*  $Id: zl303xx_PtpV1Msg.c 14109 2016-09-02 17:27:33Z DP $
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
*     This file contains the message packing/unpacking functions for the IEEE-1588-2002 version 1
*     messages
*
*******************************************************************************/

/* see IEEE 1588-2002 spec annex d */

/****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_ErrTrace.h"
#include "zl303xx_Trace.h"

#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpV2MsgParse.h"


/****************   DEFINES     **********************************************/

/****************   DATA TYPES   *********************************************/

/****************   DATA STRUCTURES   ****************************************/

/****************   STATIC FUNCTION DECLARATIONS   ***************************/

/****************   STATIC GLOBAL VARIABLES   ********************************/

/****************   EXPORTED GLOBAL VARIABLES   ******************************/

/****************   EXPORTED FUNCTION DEFINITIONS   **************************/

#ifdef ZL303XX_INCLUDE_PTP_V1   /* Version 1 message formats */

/*

  Function Name:
    zl303xx_MsgV1UnpackHeader

  Details:
   Unpacks the common header from a PTP packet into a structure

  Parameters:
       buf         Pointer to a buffer

       header      structure will be populated from the supplied packet

  Return Value:
   None

 *****************************************************************************/
void zl303xx_MsgV1UnpackHeader(Uint8T *buf, zl303xx_MsgHeaderS *header)
{
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3, "zl303xx_MsgV1UnpackHeader:", 0,0,0,0,0,0);

   header->versionPTP = zl303xx_PtpMsgVersionGet(buf);
   header->u.v1.versionNetwork = UNPACK_16BIT(buf + 2);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5, "   versionPTP %d", header->versionPTP, 0,0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5, "   versionNetwork %d", header->u.v1.versionNetwork, 0,0,0,0,0);

   OS_MEMCPY(header->u.v1.subdomain, (buf + 4), 16);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 6, "   subdomain %s", header->u.v1.subdomain, 0,0,0,0,0);

   header->messageType = *(Uint8T*)(buf + 20);
   header->u.v1.sourceIdentity.communicationTechnology = *(Uint8T*)(buf + 21);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5, "   messageType %d", header->messageType, 0,0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 7, "   sourceCommunicationTechnology %d", header->u.v1.sourceIdentity.communicationTechnology, 0,0,0,0,0);

   OS_MEMCPY(header->u.v1.sourceIdentity.uuid, (buf + 22), PTP_V1_UUID_LENGTH);
   header->u.v1.sourceIdentity.portNumber = UNPACK_16BIT(buf + 28);
   ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_ENGINE, 5)
   {
      zl303xx_DisplayV1DeviceIdentity("   sourceIdentity ", &header->u.v1.sourceIdentity);
   }

   header->sequenceId = UNPACK_16BIT(buf + 30);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5, "   sequenceId %d", header->sequenceId, 0,0,0,0,0);

   header->controlField = *(Uint8T*)(buf + 32);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5, "   control %d", header->controlField, 0,0,0,0,0);

   header->flagField = UNPACK_16BIT(buf + 34);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 6, "   flags 0x%04hx", header->flagField, 0,0,0,0,0);
}


/*

  Function Name:
    zl303xx_MsgV1UnpackSync

  Details:
   Unpacks a sync message from a PTP packet into a structure

  Parameters:
       buf         Pointer to a buffer

       sync        structure will be populated from the supplied packet

  Return Value:
   None

 *****************************************************************************/
void zl303xx_MsgV1UnpackSync(Uint8T *buf, MsgV1Sync *sync)
{
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE,3,"zl303xx_MsgV1UnpackSync:", 0,0,0,0,0,0);

   sync->originTimestamp.secondsField.hi = 0;
   sync->originTimestamp.secondsField.lo = UNPACK_32BIT(buf + 40);
   sync->originTimestamp.nanosecondsField = networkNsToInternalNs(UNPACK_32BIT(buf + 44));
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 7, "   originTimestamp.seconds %lu", sync->originTimestamp.secondsField.lo, 0,0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 7, "   originTimestamp.nanoseconds %ld", sync->originTimestamp.nanosecondsField, 0,0,0,0,0);

   sync->epochNumber = UNPACK_16BIT(buf + 48);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 7, "   epochNumber %d", sync->epochNumber, 0,0,0,0,0);

   sync->currentUtcOffset = UNPACK_16BIT(buf + 50);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 7, "   currentUtcOffset %d", sync->currentUtcOffset, 0,0,0,0,0);

   sync->grandmasterIdentity.communicationTechnology = *(Uint8T*)(buf + 53);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 8, "   grandmasterCommunicationTechnology %d", sync->grandmasterIdentity.communicationTechnology, 0,0,0,0,0);

   OS_MEMCPY(sync->grandmasterIdentity.uuid, (buf + 54), PTP_V1_UUID_LENGTH);
   sync->grandmasterIdentity.portNumber = UNPACK_16BIT(buf + 60);
   ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_ENGINE, 8)
   {
      zl303xx_DisplayV1DeviceIdentity("   grandmasterIdentity ", &sync->grandmasterIdentity);
   }

   sync->grandmasterSequenceId = UNPACK_16BIT(buf + 62);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 7, "   grandmasterSequenceId %d", sync->grandmasterSequenceId, 0,0,0,0,0);

   sync->grandmasterClockStratum = *(Uint8T*)(buf + 67);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 8, "   grandmasterClockStratum %d", sync->grandmasterClockStratum, 0,0,0,0,0);

   OS_MEMCPY(sync->grandmasterClockIdentifier, (buf + 68), 4);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 8, "   grandmasterClockIdentifier %c%c%c%c",
         sync->grandmasterClockIdentifier[0], sync->grandmasterClockIdentifier[1],
         sync->grandmasterClockIdentifier[2], sync->grandmasterClockIdentifier[3], 0,0);

   sync->grandmasterClockVariance = UNPACK_16BIT(buf + 74);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 9, "   grandmasterClockVariance %d", sync->grandmasterClockVariance, 0,0,0,0,0);

   sync->grandmasterPreferred = (zl303xx_BooleanE)(*(Uint8T*)(buf + 77));
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 9, "   grandmasterPreferred %d", (Sint8T)sync->grandmasterPreferred, 0,0,0,0,0);

   sync->grandmasterIsBoundaryClock = (zl303xx_BooleanE)(*(Uint8T*)(buf + 79));
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 91, "   grandmasterIsBoundaryClock %d", (Sint8T)sync->grandmasterIsBoundaryClock, 0,0,0,0,0);

   sync->logSyncInterval = *(Sint8T*)(buf + 83);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 9, "   logSyncInterval %d", sync->logSyncInterval, 0,0,0,0,0);

   sync->localClockVariance = UNPACK_16BIT(buf + 86);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 9, "   localClockVariance %d", sync->localClockVariance, 0,0,0,0,0);

   sync->localStepsRemoved = UNPACK_16BIT(buf + 90);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 9, "   localStepsRemoved %d", sync->localStepsRemoved, 0,0,0,0,0);

   sync->localClockStratum = *(Uint8T*)(buf + 95);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 9, "   localClockStratum %d", sync->localClockStratum, 0,0,0,0,0);

   OS_MEMCPY(sync->localClockIdentifer, (buf + 96), PTP_CODE_STRING_LENGTH);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 8, "   localClockIdentifer %c%c%c%c",
         sync->localClockIdentifer[0], sync->localClockIdentifer[1],
         sync->localClockIdentifer[2], sync->localClockIdentifer[3], 0,0);

   sync->parentIdentity.communicationTechnology = *(Uint8T*)(buf + 101);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 9, "   parentCommunicationTechnology %d", sync->parentIdentity.communicationTechnology, 0,0,0,0,0);

   OS_MEMCPY(sync->parentIdentity.uuid, (buf + 102), PTP_V1_UUID_LENGTH);
   sync->parentIdentity.portNumber = UNPACK_16BIT(buf + 110);
   ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_ENGINE, 9)
   {
      zl303xx_DisplayV1DeviceIdentity("   parentIdentity ", &sync->parentIdentity);
   }
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 9, "   parentPortField %d", sync->parentIdentity.portNumber, 0,0,0,0,0);

   sync->estimatedMasterVariance = UNPACK_16BIT(buf + 114);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 10, "   estimatedMasterVariance %d", sync->estimatedMasterVariance, 0,0,0,0,0);

   sync->estimatedMasterDrift = UNPACK_32BIT(buf + 116);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 8, "   estimatedMasterDrift %ld", sync->estimatedMasterDrift, 0,0,0,0,0);

   sync->utcReasonable = (zl303xx_BooleanE)(*(Uint8T*)(buf + 123));
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 9, "   utcReasonable %d", (Sint8T)sync->utcReasonable, 0,0,0,0,0);
}

/*

  Function Name:
    zl303xx_MsgV1UnpackDelayReq

  Details:
   Unpacks a delay request message from a PTP packet into a structure

  Parameters:
       buf         Pointer to a buffer

       req         structure will be populated from the supplied packet

  Return Value:
   None

  Notes:
   This is a null operation

*******************************************************************************/
void zl303xx_MsgV1UnpackDelayReq(Uint8T *buf, MsgV1DelayReq *req)
{
   if (!buf || !req)
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_MsgV1UnpackDelayReq: either buf or req is NULL", 0,0,0,0,0,0);
   }

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3, "zl303xx_MsgV1UnpackDelayReq:", 0,0,0,0,0,0);
}

/*

  Function Name:
    zl303xx_MsgUnpackFollowUp

  Details:
   Unpacks a follow-up message from a PTP packet into a structure

  Parameters:
       buf         Pointer to a buffer

       follow      structure will be populated from the supplied packet

  Return Value:
   None

 *****************************************************************************/
void zl303xx_MsgV1UnpackFollowUp(Uint8T *buf, MsgV1FollowUp *follow)
{
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 7, "zl303xx_MsgUnpackFollowUp", 0,0,0,0,0,0);

   follow->associatedSequenceId = UNPACK_16BIT(buf + 42);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 7, "   associatedSequenceId %u", follow->associatedSequenceId, 0,0,0,0,0);

   follow->preciseOriginTimestamp.secondsField.hi = 0;
   follow->preciseOriginTimestamp.secondsField.lo = UNPACK_32BIT(buf + 44);
   follow->preciseOriginTimestamp.nanosecondsField = networkNsToInternalNs(UNPACK_32BIT(buf + 48));
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 7, "   preciseOriginTimestamp.seconds %lu", follow->preciseOriginTimestamp.secondsField.lo, 0,0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 7, "   preciseOriginTimestamp.nanoseconds %ld", follow->preciseOriginTimestamp.nanosecondsField, 0,0,0,0,0);
}

/*

  Function Name:
    zl303xx_MsgV1UnpackDelayResp

  Details:
   Unpacks a delay-response message from a PTP packet into a structure

  Parameters:
       buf         Pointer to a buffer

       resp        structure will be populated from the supplied packet

  Return Value:
   None

 *****************************************************************************/
void zl303xx_MsgV1UnpackDelayResp(Uint8T *buf, MsgV1DelayResp *resp)
{
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 7, "zl303xx_MsgUnpackDelayResp", 0,0,0,0,0,0);

   resp->delayReceiptTimestamp.secondsField.hi = 0;
   resp->delayReceiptTimestamp.secondsField.lo = UNPACK_32BIT(buf + 40);
   resp->delayReceiptTimestamp.nanosecondsField = networkNsToInternalNs(UNPACK_32BIT(buf + 44));
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 7, "   delayReceiptTimestamp.seconds %lu", resp->delayReceiptTimestamp.secondsField.lo, 0,0,0,0,0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 7, "   delayReceiptTimestamp.nanoseconds %ld", resp->delayReceiptTimestamp.nanosecondsField, 0,0,0,0,0);

   resp->requestingSourceIdentity.communicationTechnology = *(Uint8T*)(buf + 49);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 9, "   requestingSourceCommunicationTechnology %d", resp->requestingSourceIdentity.communicationTechnology, 0,0,0,0,0);

   OS_MEMCPY(resp->requestingSourceIdentity.uuid, (buf + 50), PTP_V1_UUID_LENGTH);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 8, "   requestingSourceUuid %02hx:%02hx:%02hx:%02hx:%02hx:%02hx",
         resp->requestingSourceIdentity.uuid[0], resp->requestingSourceIdentity.uuid[1], resp->requestingSourceIdentity.uuid[2],
         resp->requestingSourceIdentity.uuid[3], resp->requestingSourceIdentity.uuid[4], resp->requestingSourceIdentity.uuid[5]);

   resp->requestingSourceIdentity.portNumber = UNPACK_16BIT(buf + 56);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 8, "   requestingSourcePortId %d", resp->requestingSourceIdentity.portNumber, 0,0,0,0,0);

   resp->requestingSourceSequenceId = UNPACK_16BIT(buf + 58);
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 7, "   requestingSourceSequenceId %d", resp->requestingSourceSequenceId, 0,0,0,0,0);
}

/*

  Function Name:
    zl303xx_MsgV1UnpackManagement

  Details:
   Unpacks the initial part of a management message from a PTP packet into a structure

  Parameters:
       buf         Pointer to a buffer

       manage      structure will be populated from the supplied packet

  Return Value:
   None

 *****************************************************************************/
void zl303xx_MsgV1UnpackManagement(Uint8T *buf, MsgV1Management *manage)
{
   manage->targetIdentity.communicationTechnology = *(Uint8T*)(buf + 41);
   OS_MEMCPY(manage->targetIdentity.uuid, (buf + 42), PTP_V1_UUID_LENGTH);
   manage->targetIdentity.portNumber = UNPACK_16BIT(buf + 48);
   manage->startingBoundaryHops = UNPACK_16BIT(buf + 50);
   manage->boundaryHops = UNPACK_16BIT(buf + 52);
   manage->managementMessageKey = *(Uint8T*)(buf + 55);
   manage->parameterLength = UNPACK_16BIT(buf + 58);

   if (manage->managementMessageKey == PTP_MM_GET_FOREIGN_DATA_SET)
      manage->recordKey = UNPACK_16BIT(buf + 62);
}

/*

  Function Name:
    zl303xx_MsgV1UnloadManagement

  Details:
   Once the type of a management packet has been identified this function
   performs the operations necessary to handle this message

  Parameters:
       buf         Pointer to a buffer
       manage      A previously populated management message data structure
       clockStream Pointer to the clock stream instance structure

       clockStream Contents may be modified depending on the message type

  Return Value:
   The next PTP state to enter or the current state if no change is required

 *****************************************************************************/
zl303xx_PtpPortInternalStateE zl303xx_MsgV1UnloadManagement(Uint8T *buf,
                                       MsgV1Management *manage,
                                       zl303xx_ClockStreamS *clockStream)
{
   zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

   switch(manage->managementMessageKey)
   {
   case PTP_MM_INITIALIZE_CLOCK:
      if(pPtpClock->defaultDSInt.u.v1.initializable)
         return PTP_PORT_INT_STATE_INIT;
      break;

   case PTP_MM_GOTO_FAULTY_STATE:
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3, "event FAULT_DETECTED (forced by management message)",0,0,0,0,0,0);
      return PTP_PORT_INT_STATE_FAULTY;

   case PTP_MM_DISABLE_PORT:
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3, "event DESIGNATED_DISABLED",0,0,0,0,0,0);
      return PTP_PORT_INT_STATE_DISABLED;

   case PTP_MM_ENABLE_PORT:
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3, "event DESIGNATED_ENABLED", 0,0,0,0,0,0);
      return PTP_PORT_INT_STATE_INIT;

   case PTP_MM_CLEAR_DESIGNATED_PREFERRED_MASTER:
      pPtpClock->defaultDSInt.u.v1.preferred = ZL303XX_FALSE;
      break;

   case PTP_MM_SET_DESIGNATED_PREFERRED_MASTER:
      pPtpClock->defaultDSInt.u.v1.preferred = ZL303XX_TRUE;
      break;

   case PTP_MM_DISABLE_BURST:
      if (clockStream)
      {
         clockStream->portData->portConfig.u.v1.burst_enabled = ZL303XX_FALSE;
      }
      break;

   case PTP_MM_ENABLE_BURST:
      if (clockStream)
      {
         clockStream->portData->portConfig.u.v1.burst_enabled = ZL303XX_TRUE;
      }
      break;

   case PTP_MM_SET_SYNC_INTERVAL:
      if (clockStream)
      {
         clockStream->config.logSyncInterval = *(Sint8T*)(buf + 63);
      }
      break;

   case PTP_MM_SET_SUBDOMAIN:
      OS_MEMCPY(pPtpClock->defaultDSInt.u.v1.subdomain_name,
             (buf + 60),
             PTP_SUBDOMAIN_NAME_LENGTH);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5,
            "set subdomain to %s",
            pPtpClock->defaultDSInt.u.v1.subdomain_name, 0,0,0,0,0);

      break;

   case PTP_MM_SET_TIME:
      if (clockStream)
      {
         Uint32T nanosecondsField;
         Uint64S secondsField;
         zl303xx_TimeStamp clockTime = {{0}};

         secondsField.hi = 0;
         secondsField.lo = UNPACK_32BIT(buf + 60);
         nanosecondsField = UNPACK_32BIT(buf + 64);  /* Unused */

         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "Setting local PTP clock time to %lus.%09luns (in epoch %lu)",
               secondsField.hi, secondsField.lo, 0,0,0,0);

         clockTime.secondsField = secondsField;
         clockTime.nanosecondsField = 0;

         (void)zl303xx_PtpClockTimeSet(pPtpClock, &clockTime);

      }
      break;

   case PTP_MM_UPDATE_DEFAULT_DATA_SET:
      pPtpClock->defaultDSInt.u.v1.clock_stratum = *(Uint8T*)(buf + 63);
      OS_MEMCPY(pPtpClock->defaultDSInt.u.v1.clock_identifier,
             (buf + 64),
             PTP_CODE_STRING_LENGTH);
      pPtpClock->defaultDSInt.u.v1.clock_variance = UNPACK_16BIT(buf + 70);
      pPtpClock->defaultDSInt.u.v1.preferred = (zl303xx_BooleanE)(*(Uint8T*)(buf + 75));
      clockStream->config.logSyncInterval = *(Sint8T*)(buf + 79);
      OS_MEMCPY(pPtpClock->defaultDSInt.u.v1.subdomain_name,
             (buf + 80),
             PTP_SUBDOMAIN_NAME_LENGTH);
      break;

   case PTP_MM_UPDATE_GLOBAL_TIME_PROPERTIES:
      pPtpClock->timePropertiesDS.currentUtcOffset = UNPACK_16BIT(buf + 62);
      pPtpClock->timePropertiesDS.leap59 = (zl303xx_BooleanE)(*(Uint8T*)(buf + 67));
      pPtpClock->timePropertiesDS.leap61 = (zl303xx_BooleanE)(*(Uint8T*)(buf + 71));
      pPtpClock->timePropertiesDS.epochNumber = UNPACK_16BIT(buf + 74);
      break;

   default:
      break;
   }

   return PTP_PORT_INT_STATE_ACTIVE;
}

/*

  Function Name:
    zl303xx_MsgV1PackHeader

  Details:
   Packs the common part of a PTP message from the currently stored PTP clock
   information

  Parameters:
       clockStream Pointer to the clock stream instance structure

       buf         Pointer to a buffer in which to assemble the packet

  Return Value:
   None

 *****************************************************************************/
void zl303xx_MsgV1PackHeader(Uint8T *buf, zl303xx_ClockStreamS *clockStream)
{
   zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

   PACK_16BIT((buf + 0), ZL303XX_PTP_VERSION_1);
   PACK_16BIT((buf + 2), VERSION_NETWORK1);

   OS_MEMCPY((buf + 4), pPtpClock->defaultDSInt.u.v1.subdomain_name, 16);
   *(Uint8T*)(buf + 20 + 1) = clockStream->portData->portConfig.u.v1.portIdentity.communicationTechnology;

   OS_MEMCPY((buf + 22), clockStream->portData->portConfig.u.v1.portIdentity.uuid, PTP_V1_UUID_LENGTH);

   /* flags */
   {
      Uint16T flags16 = 0;

      if(pPtpClock->defaultDSInt.u.v1.external_timing)
         setFlag(flags16, PTP1_FLAGS_EXT_SYNC);
      if(pPtpClock->defaultDSInt.u.v1.clock_followup_capable)
         setFlag(flags16, PTP1_FLAGS_ASSIST);
      if(pPtpClock->defaultDSInt.u.v1.is_boundary_clock)
         setFlag(flags16, PTP1_FLAGS_BOUNDARY_CLOCK);

      PACK_16BIT((buf + 34), flags16);
   }
}

/*

  Function Name:
    zl303xx_MsgV1PackSync

  Details:
   Packs a sync message from the currently stored PTP clock
   information

  Parameters:
       clockStream Pointer to the clock stream instance structure
       burst       ZL303XX_TRUE if this sync packet has the burst flag set

       buf         Pointer to a buffer in which to assemble the packet

  Return Value:
   None

 *****************************************************************************/
void zl303xx_MsgV1PackSync(Uint8T *buf, zl303xx_BooleanE burst, zl303xx_ClockStreamS *clockStream)
{
   zl303xx_TimeStamp txTime;
   zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

   *(Uint8T*)(buf +20) = 1;  /* messageType */
   PACK_16BIT((buf + 28), clockStream->portData->portConfig.u.v1.portIdentity.portNumber);
   PACK_16BIT((buf + 30), clockStream->u.v1.last_sync_event_sequence_number);
   *(Uint8T*)(buf +32) = PTP_SYNC_MESSAGE;  /* controlField */

   /* flags */
   {
      Uint16T flags16 = 0;

      if(burst)
         setFlag(flags16, PTP1_FLAGS_SYNC_BURST);
      else
         clearFlag(flags16, PTP1_FLAGS_SYNC_BURST);

      if (pPtpClock->parentDS.parentStats == ZL303XX_TRUE)
         setFlag(flags16, PTP1_FLAGS_PARENT_STATS);
      else
         clearFlag(flags16, PTP1_FLAGS_PARENT_STATS);

      PACK_16BIT((buf + 34), flags16);
   }

   /* The returned txTime value should already be adjusted for latency.  */
   (void)zl303xx_PtpClockTimeGet(pPtpClock, ZL303XX_PTP_INTERFACE_EVENT, &txTime);

   PACK_32BIT((buf + 40), txTime.secondsField.lo);
   PACK_32BIT((buf + 44), internalNsToNetworkNs(txTime.nanosecondsField));
   PACK_16BIT((buf + 48), pPtpClock->timePropertiesDS.epochNumber);
   PACK_16BIT((buf + 50), pPtpClock->timePropertiesDS.currentUtcOffset);
   PACK_8BIT((buf + 52 + 1), clockStream->farEndParentDS.u.v1.grandmasterIdentity.communicationTechnology);
   OS_MEMCPY((buf + 54), clockStream->farEndParentDS.u.v1.grandmasterIdentity.uuid, PTP_V1_UUID_LENGTH);
   PACK_16BIT((buf + 60), clockStream->farEndParentDS.u.v1.grandmasterIdentity.portNumber);
   PACK_16BIT((buf + 62), clockStream->farEndParentDS.u.v1.grandmaster_sequence_number);
   PACK_8BIT((buf + 64 + 3), clockStream->farEndParentDS.u.v1.grandmaster_stratum);
   OS_MEMCPY((buf + 68), clockStream->farEndParentDS.u.v1.grandmaster_identifier, 4);
   PACK_16BIT((buf + 72 + 2), clockStream->farEndParentDS.u.v1.grandmaster_variance);
   PACK_16BIT((buf + 76), clockStream->farEndParentDS.u.v1.grandmaster_preferred);
   PACK_16BIT((buf + 78), clockStream->farEndParentDS.u.v1.grandmaster_is_boundary_clock);
   PACK_8BIT((buf + 80 + 3), (Uint8T)clockStream->config.logSyncInterval);
   PACK_16BIT((buf + 84 + 2), pPtpClock->defaultDSInt.u.v1.clock_variance);
   PACK_16BIT((buf + 88 + 2), pPtpClock->currentDSInt.stepsRemoved);

   /* IEEE-1588-2002 requires that a slave only clock always present a stratum of 255. However slave-only clocks
     do not send Sync messages so always use the ZL303XX_TRUE stratum here */
   PACK_8BIT((buf + 92 + 3), pPtpClock->defaultDSInt.u.v1.clock_stratum);
   OS_MEMCPY((buf + 96), pPtpClock->defaultDSInt.u.v1.clock_identifier, 4);
   *(Uint8T*)(buf + 100 + 1) = clockStream->farEndParentDS.u.v1.parentIdentity.communicationTechnology;
   OS_MEMCPY((buf + 102), clockStream->farEndParentDS.u.v1.parentIdentity.uuid, PTP_V1_UUID_LENGTH);
   PACK_16BIT((buf + 108 + 2), clockStream->farEndParentDS.u.v1.parentIdentity.portNumber);

   PACK_16BIT((buf + 112 + 2), pPtpClock->parentDS.observedParentOffsetScaledLogVariance);
   PACK_32BIT((buf + 116), pPtpClock->parentDS.observedParentClockPhaseChangeRate);
   *(Uint8T*)(buf + 120 + 3) = clockStream->farEndParentDS.u.v1.utc_reasonable;
}

/*

  Function Name:
    zl303xx_MsgV1PackDelayReq

  Details:
   Packs a delay_request message from the currently stored PTP clock
   information

  Parameters:
       clockStream Pointer to the clock stream instance structure
       burst       ZL303XX_TRUE if this message should have the burst flag set
               i.e. to request a burst of sync messages

       buf         Pointer to a buffer in which to assemble the packet

  Return Value:
   None

 *****************************************************************************/
void zl303xx_MsgV1PackDelayReq(Uint8T *buf, zl303xx_BooleanE burst, zl303xx_ClockStreamS *clockStream)
{
   zl303xx_TimeStamp txTime;
   zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

   *(Uint8T*)(buf + 20) = 1;  /* messageType */
   PACK_16BIT((buf + 28), clockStream->portData->portConfig.u.v1.portIdentity.portNumber);
   PACK_16BIT((buf + 30), ++clockStream->u.v1.last_sync_event_sequence_number);
   *(Uint8T*)(buf + 32) = PTP_DELAY_REQ_MESSAGE;  /* controlField */

   /* flags */
   {
      Uint16T flags16 = 0;

      if(burst)
         setFlag(flags16, PTP1_FLAGS_SYNC_BURST);
      else
         clearFlag(flags16, PTP1_FLAGS_SYNC_BURST);

      if (pPtpClock->parentDS.parentStats == ZL303XX_TRUE)
         setFlag(flags16, PTP1_FLAGS_PARENT_STATS);
      else
         clearFlag(flags16, PTP1_FLAGS_PARENT_STATS);

      PACK_16BIT((buf + 34), flags16);
   }

   /* The returned txTime value should already be adjusted for latency.  */
   (void)zl303xx_PtpClockTimeGet(pPtpClock, ZL303XX_PTP_INTERFACE_EVENT, &txTime);

   PACK_32BIT((buf + 40), txTime.secondsField.lo);
   PACK_32BIT((buf + 44), internalNsToNetworkNs((Sint32T)txTime.nanosecondsField));

   PACK_16BIT((buf + 48), pPtpClock->timePropertiesDS.epochNumber);
   PACK_16BIT((buf + 50), pPtpClock->timePropertiesDS.currentUtcOffset);
   PACK_16BIT((buf + 52), clockStream->farEndParentDS.u.v1.grandmasterIdentity.communicationTechnology);
   OS_MEMCPY((buf + 54), clockStream->farEndParentDS.u.v1.grandmasterIdentity.uuid, PTP_V1_UUID_LENGTH);

   PACK_16BIT((buf + 60), clockStream->farEndParentDS.u.v1.grandmasterIdentity.portNumber);
   PACK_16BIT((buf + 62), clockStream->farEndParentDS.u.v1.grandmaster_sequence_number);
   PACK_8BIT((buf + 64 + 3), clockStream->farEndParentDS.u.v1.grandmaster_stratum);
   OS_MEMCPY((buf + 68), clockStream->farEndParentDS.u.v1.grandmaster_identifier, 4);
   PACK_16BIT((buf + 72 + 2), clockStream->farEndParentDS.u.v1.grandmaster_variance);
   PACK_16BIT((buf + 76), clockStream->farEndParentDS.u.v1.grandmaster_preferred);
   PACK_16BIT((buf + 78), clockStream->farEndParentDS.u.v1.grandmaster_is_boundary_clock);
   PACK_8BIT((buf + 80 + 3), (Uint8T)clockStream->config.logSyncInterval);
   PACK_16BIT((buf + 84 + 2), pPtpClock->defaultDSInt.u.v1.clock_variance);
   PACK_32BIT((buf + 88), pPtpClock->currentDSInt.stepsRemoved);

   if (clockStream->config.mode == ZL303XX_PTP_STREAM_SLAVE_ONLY)
   {   /* IEEE-1588-2002 requires that a slave only clock always present a stratum of 255 */
      PACK_8BIT((buf + 92 + 3), 255);
   }
   else /* Use the user specified stratum */
   {
      PACK_8BIT((buf + 92 + 3), pPtpClock->defaultDSInt.u.v1.clock_stratum);
   }

   OS_MEMCPY((buf + 96), pPtpClock->defaultDSInt.u.v1.clock_identifier, 4);
   PACK_16BIT((buf + 100), clockStream->farEndParentDS.u.v1.parentIdentity.communicationTechnology);
   OS_MEMCPY((buf + 102), clockStream->farEndParentDS.u.v1.parentIdentity.uuid, PTP_V1_UUID_LENGTH);

   PACK_32BIT((buf + 108), clockStream->farEndParentDS.u.v1.parentIdentity.portNumber);
   PACK_16BIT((buf + 112 + 2), pPtpClock->parentDS.observedParentOffsetScaledLogVariance);
   PACK_32BIT((buf + 116), pPtpClock->parentDS.observedParentClockPhaseChangeRate);
   PACK_32BIT((buf + 120), clockStream->farEndParentDS.u.v1.utc_reasonable);
}

/*

  Function Name:
    zl303xx_MsgV1PackFollowUp

  Details:
   Packs a follow-up message from the currently stored PTP clock
   information

  Parameters:
       associatedSequenceId The sequence number of the sync message associated
               with this follow-up
       preciseOriginTimestamp  The accurate timestamp to insert into the message
       burst       ZL303XX_TRUE if this message should have the burst flag set
               i.e. it is part of a requested burst of sync+follow-up messages
       clockStream Pointer to the clock stream instance structure

       buf         Pointer to a buffer in which to assemble the packet

  Return Value:
   None

 *****************************************************************************/
void zl303xx_MsgV1PackFollowUp(Uint8T *buf, Uint16T associatedSequenceId,
                               zl303xx_TimeStamp *preciseOriginTimestamp,
                               zl303xx_BooleanE burst, zl303xx_ClockStreamS *clockStream)
{
   zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

   *(Uint8T*)(buf + 20) = 2;  /* messageType */
   PACK_16BIT((buf + 28), clockStream->portData->portConfig.u.v1.portIdentity.portNumber);
   PACK_16BIT((buf + 30), clockStream->u.v1.last_general_event_sequence_number);
   *(Uint8T*)(buf + 32) = PTP_FOLLOWUP_MESSAGE;  /* controlField */

   /* flags */
   {
      Uint16T flags16 = 0;

      if(burst)
         setFlag(flags16, PTP1_FLAGS_SYNC_BURST);
      else
         clearFlag(flags16, PTP1_FLAGS_SYNC_BURST);

      if (pPtpClock->parentDS.parentStats == ZL303XX_TRUE)
         setFlag(flags16, PTP1_FLAGS_PARENT_STATS);
      else
         clearFlag(flags16, PTP1_FLAGS_PARENT_STATS);

      PACK_16BIT((buf + 34), flags16);
   }

   PACK_32BIT((buf + 40), associatedSequenceId);
   PACK_32BIT((buf + 44), preciseOriginTimestamp->secondsField.lo);
   PACK_32BIT((buf + 48), internalNsToNetworkNs(preciseOriginTimestamp->nanosecondsField));
}

/*

  Function Name:
    zl303xx_MsgV1PackDelayResp

  Details:
   Packs a delay response message from the currently stored PTP clock
   information

  Parameters:
       header      The header from the received delay request message which is
               triggering this delay response
       delayReceiptTimestamp  The received timestamp of the delay request
       clockStream Pointer to the clock stream instance structure

       buf         Pointer to a buffer in which to assemble the packet

  Return Value:
   None

 *****************************************************************************/
void zl303xx_MsgV1PackDelayResp(Uint8T *buf, zl303xx_MsgHeaderS *header,
                                zl303xx_TimeStamp *delayReceiptTimestamp,
                                zl303xx_ClockStreamS *clockStream)
{
   zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

   *(Uint8T*)(buf + 20) = 2;  /* messageType */
   PACK_16BIT((buf + 28), clockStream->portData->portConfig.u.v1.portIdentity.portNumber);
   PACK_16BIT((buf + 30), clockStream->u.v1.last_general_event_sequence_number);
   *(Uint8T*)(buf + 32) = PTP_DELAY_RESP_MESSAGE;  /* controlField */

   /* flags */
   {
      Uint16T flags16 = 0;

      clearFlag(flags16, PTP1_FLAGS_SYNC_BURST);

      if (pPtpClock->parentDS.parentStats == ZL303XX_TRUE)
         setFlag(flags16, PTP1_FLAGS_PARENT_STATS);
      else
         clearFlag(flags16, PTP1_FLAGS_PARENT_STATS);

      PACK_16BIT((buf + 34), flags16);
   }

   PACK_32BIT((buf + 40), delayReceiptTimestamp->secondsField.lo);
   PACK_32BIT((buf + 44), internalNsToNetworkNs(delayReceiptTimestamp->nanosecondsField));
   PACK_16BIT((buf + 48), header->u.v1.sourceIdentity.communicationTechnology);
   OS_MEMCPY(buf + 50, header->u.v1.sourceIdentity.uuid, PTP_V1_UUID_LENGTH);
   PACK_16BIT((buf + 56), header->u.v1.sourceIdentity.portNumber);
   PACK_16BIT((buf + 58), header->sequenceId);
}

/*

  Function Name:
    zl303xx_MsgV1PackManagement

  Details:
   Packs a management message from the currently stored PTP clock
   information

  Parameters:
       header      Where appropriate contains the header from the last received management
               message to which this is a response. Not applicable if this is a management
               message initiated by this node.
       manage      Where appropriate contains the body of the last received management
               message to which this is a response. Not applicable if this is a management
               message initiated by this node.
       clockStream Pointer to the clock stream instance structure

       buf         Pointer to a buffer in which to assemble the packet

  Return Value:
   The length of the assembled packet

 *****************************************************************************/
Uint16T zl303xx_MsgV1PackManagement(Uint8T *buf, zl303xx_MsgHeaderS *header,
                                    MsgV1Management *manage,
                                    zl303xx_ClockStreamS *clockStream)
{
   zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

   *(Uint8T*)(buf + 20) = 2;  /* messageType */
   PACK_16BIT((buf + 28), clockStream->portData->portConfig.u.v1.portIdentity.portNumber);
   PACK_16BIT((buf + 30), clockStream->u.v1.last_general_event_sequence_number);
   *(Uint8T*)(buf + 32) = PTP_MANAGEMENT_MESSAGE;  /* controlField */

   /* flags */
   {
      Uint16T flags16 = 0;

      clearFlag(flags16, PTP1_FLAGS_SYNC_BURST);

      if (pPtpClock->parentDS.parentStats == ZL303XX_TRUE)
         setFlag(flags16, PTP1_FLAGS_PARENT_STATS);
      else
         clearFlag(flags16, PTP1_FLAGS_PARENT_STATS);

      PACK_16BIT((buf + 34), flags16);
   }

   PACK_16BIT((buf + 40), header->u.v1.sourceIdentity.communicationTechnology);
   OS_MEMCPY(buf + 42, header->u.v1.sourceIdentity.uuid, PTP_V1_UUID_LENGTH);

   /* Set max boundary hops to however many boundary hops the original management message suffered */
   PACK_16BIT((buf + 48), header->u.v1.sourceIdentity.portNumber);
   PACK_16BIT((buf + 50), manage->startingBoundaryHops - manage->boundaryHops);
   PACK_16BIT((buf + 52), (manage->startingBoundaryHops - manage->boundaryHops));
   PACK_16BIT((buf + 54), header->u.v1.sourceIdentity.communicationTechnology);

   switch(manage->managementMessageKey)
   {
   case PTP_MM_OBTAIN_IDENTITY:
      *(Uint8T*)(buf + 55) = PTP_MM_CLOCK_IDENTITY;
      PACK_32BIT((buf + 56), 64);
      PACK_32BIT((buf + 60), pPtpClock->defaultDSInt.u.v1.clockIdentity.communicationTechnology);
      OS_MEMCPY(buf + 64, pPtpClock->defaultDSInt.u.v1.clockIdentity.uuid, PTP_V1_UUID_LENGTH);
      PACK_32BIT((buf + 72), pPtpClock->defaultDSInt.u.v1.clockIdentity.portNumber);
      strncpy((char *)(buf + 76), PtpManufacturerID, 48);
      return 124;

   case PTP_MM_GET_DEFAULT_DATA_SET:
      *(Uint8T*)(buf + 55) = PTP_MM_DEFAULT_DATA_SET;
      PACK_32BIT((buf + 56), 76);
      PACK_32BIT((buf + 60), pPtpClock->defaultDSInt.u.v1.clockIdentity.communicationTechnology);
      OS_MEMCPY(buf + 64, pPtpClock->defaultDSInt.u.v1.clockIdentity.uuid, PTP_V1_UUID_LENGTH);
      PACK_32BIT((buf + 72), pPtpClock->defaultDSInt.u.v1.clockIdentity.portNumber);
      if (clockStream->config.mode == ZL303XX_PTP_STREAM_SLAVE_ONLY)
      {
         /* Spec section 9.2.2 requires that a slave only node must return a stratum level of 255 */
         PACK_8BIT((buf + 76 + 3), 255);
      }
      else /* respond with the user specified stratum level value */
      {
         PACK_8BIT((buf + 76 + 3), pPtpClock->defaultDSInt.u.v1.clock_stratum);
      }
      OS_MEMCPY(buf + 80, pPtpClock->defaultDSInt.u.v1.clock_identifier, 4);
      PACK_16BIT((buf + 84 + 2), pPtpClock->defaultDSInt.u.v1.clock_variance);
      PACK_32BIT((buf + 88), pPtpClock->defaultDSInt.u.v1.clock_followup_capable);
      PACK_32BIT((buf + 92), pPtpClock->defaultDSInt.u.v1.preferred);
      PACK_32BIT((buf + 96), pPtpClock->defaultDSInt.u.v1.initializable);
      PACK_32BIT((buf + 100), pPtpClock->defaultDSInt.u.v1.external_timing);
      PACK_32BIT((buf + 104), pPtpClock->defaultDSInt.u.v1.is_boundary_clock);
      PACK_8BIT((buf + 108 + 3), (Uint8T)clockStream->config.logSyncInterval);
      OS_MEMCPY(buf + 112, pPtpClock->defaultDSInt.u.v1.subdomain_name, 16);
      PACK_32BIT((buf + 129), pPtpClock->defaultDS.numberPorts);
      PACK_32BIT((buf + 132), pPtpClock->foreignMasterDS.numActiveRecords);
      return 136;

   case PTP_MM_GET_CURRENT_DATA_SET:
      *(Uint8T*)(buf + 55) = PTP_MM_CURRENT_DATA_SET;
      PACK_32BIT((buf + 56), 20);
      PACK_32BIT((buf + 60), pPtpClock->currentDSInt.stepsRemoved);
      PACK_32BIT((buf + 64), pPtpClock->currentDSInt.offset_from_master.secondsField.lo);
      PACK_32BIT((buf + 68), internalNsToNetworkNs(pPtpClock->currentDSInt.offset_from_master.nanosecondsField));
      PACK_32BIT((buf + 72), pPtpClock->currentDSInt.mean_path_delay.secondsField.lo);
      PACK_32BIT((buf + 76), internalNsToNetworkNs(pPtpClock->currentDSInt.mean_path_delay.nanosecondsField));
      return 80;

   case PTP_MM_GET_PARENT_DATA_SET:
      *(Uint8T*)(buf + 55) = PTP_MM_PARENT_DATA_SET;
      PACK_32BIT((buf + 56), 90);
      PACK_32BIT((buf + 60), clockStream->farEndParentDS.u.v1.parentIdentity.communicationTechnology);
      OS_MEMCPY(buf + 64, clockStream->farEndParentDS.u.v1.parentIdentity.uuid, PTP_V1_UUID_LENGTH);

      PACK_32BIT((buf + 72), clockStream->farEndParentDS.u.v1.parentIdentity.portNumber);
      PACK_32BIT((buf + 76), clockStream->farEndParentDS.u.v1.parent_last_sync_sequence_number);
      PACK_16BIT((buf + 80), clockStream->farEndParentDS.u.v1.parent_two_step_clock);
      PACK_32BIT((buf + 84), clockStream->farEndParentDS.u.v1.parent_external_timing);
      PACK_16BIT((buf + 88 + 2), clockStream->farEndParentDS.u.v1.parent_log_variance);
      PACK_32BIT((buf + 92), pPtpClock->parentDS.parentStats);
      PACK_16BIT((buf + 96 + 2), pPtpClock->parentDS.observedParentOffsetScaledLogVariance);
      PACK_32BIT((buf + 100), pPtpClock->parentDS.observedParentClockPhaseChangeRate);
      PACK_32BIT((buf + 104), clockStream->farEndParentDS.u.v1.utc_reasonable);
      PACK_32BIT((buf + 108), clockStream->farEndParentDS.u.v1.grandmasterIdentity.communicationTechnology);
      OS_MEMCPY(buf + 112, clockStream->farEndParentDS.u.v1.grandmasterIdentity.uuid, PTP_V1_UUID_LENGTH);
      PACK_32BIT((buf + 120), clockStream->farEndParentDS.u.v1.grandmasterIdentity.portNumber);
      PACK_32BIT((buf + 124), clockStream->farEndParentDS.u.v1.grandmaster_stratum);
      OS_MEMCPY(buf + 128, clockStream->farEndParentDS.u.v1.grandmaster_identifier, 4);
      PACK_16BIT((buf + 132 + 2), clockStream->farEndParentDS.u.v1.grandmaster_variance);
      PACK_32BIT((buf + 136), clockStream->farEndParentDS.u.v1.grandmaster_preferred);
      PACK_32BIT((buf + 140), clockStream->farEndParentDS.u.v1.grandmaster_is_boundary_clock);
      PACK_32BIT((buf + 144), clockStream->farEndParentDS.u.v1.grandmaster_sequence_number);
      return 148;

   case PTP_MM_GET_PORT_DATA_SET:
      if (clockStream)
      {
         zl303xx_PortStateE portState = zl303xx_GetV1PortState(clockStream->portData);

         if(manage->targetIdentity.portNumber != clockStream->portData->portConfig.u.v1.portIdentity.portNumber)
         {
            *(Uint8T*)(buf + 55) = PTP_MM_NULL;
            PACK_32BIT((buf + 56), 0);
            return 0;
         }

         *(Uint8T*)(buf + 55) = PTP_MM_PORT_DATA_SET;
         PACK_32BIT((buf + 56), 52);
         PACK_32BIT((buf + 60), manage->targetIdentity.portNumber);
         PACK_8BIT((buf + 64 + 3), portState);
         PACK_32BIT((buf + 68), clockStream->u.v1.last_sync_event_sequence_number);
         PACK_32BIT((buf + 72), clockStream->u.v1.last_general_event_sequence_number);
         PACK_32BIT((buf + 76), clockStream->portData->portConfig.u.v1.portIdentity.communicationTechnology);
         OS_MEMCPY(buf + 80, clockStream->portData->portConfig.u.v1.portIdentity.uuid, PTP_V1_UUID_LENGTH);
         PACK_32BIT((buf + 88), clockStream->portData->portConfig.u.v1.portIdentity.portNumber);
         PACK_32BIT((buf + 92), clockStream->portData->portConfig.u.v1.burst_enabled);
         PACK_32BIT((buf + 96), 0x00040202);
         OS_MEMCPY(buf + 100, pPtpClock->defaultDSInt.u.v1.subdomain_address, 4);
         OS_MEMCPY(buf + 106, clockStream->portData->portConfig.u.v1.event_port_address, 2);
         OS_MEMCPY(buf + 110, clockStream->portData->portConfig.u.v1.general_port_address, 2);
         return 112;
      }
      else
      {
         return 0;
      }

   case PTP_MM_GET_GLOBAL_TIME_DATA_SET:
      *(Uint8T*)(buf + 55) = PTP_MM_GLOBAL_TIME_DATA_SET;
      PACK_32BIT((buf + 56), 24);

      {
         zl303xx_TimeStamp clockTime = {{0}};
         (void)zl303xx_PtpClockTimeGet(pPtpClock, ZL303XX_PTP_INTERFACE_GENERAL, &clockTime);

         PACK_32BIT((buf + 60), clockTime.secondsField.lo);
         PACK_32BIT((buf + 64), clockTime.nanosecondsField);
      }

      PACK_16BIT((buf + 68 + 2), pPtpClock->timePropertiesDS.currentUtcOffset);
      PACK_32BIT((buf + 72), pPtpClock->timePropertiesDS.leap59);
      PACK_32BIT((buf + 76), pPtpClock->timePropertiesDS.leap61);
      PACK_32BIT((buf + 80), pPtpClock->timePropertiesDS.epochNumber);
      return 84;

   case PTP_MM_GET_FOREIGN_DATA_SET:
      if((manage->targetIdentity.portNumber &&
            (manage->targetIdentity.portNumber != clockStream->portData->portConfig.u.v1.portIdentity.portNumber)) ||
            !manage->recordKey || (manage->recordKey > pPtpClock->foreignMasterDS.numActiveRecords))
      {
         *(Uint8T*)(buf + 55) = PTP_MM_NULL;
         PACK_32BIT((buf + 56), 0);
         return 0;
      }

      *(Uint8T*)(buf + 55) = PTP_MM_FOREIGN_DATA_SET;
      PACK_32BIT((buf + 56), 28);

      PACK_32BIT((buf + 60), clockStream->portData->portConfig.u.v1.portIdentity.portNumber);
      PACK_32BIT((buf + 64), manage->recordKey);
      PACK_32BIT((buf + 68), pPtpClock->foreignMasterDS.foreign[manage->recordKey - 1].v1.foreignMasterIdentity.communicationTechnology);
      OS_MEMCPY(buf + 72, pPtpClock->foreignMasterDS.foreign[manage->recordKey - 1].v1.foreignMasterIdentity.uuid, PTP_V1_UUID_LENGTH);
      PACK_32BIT((buf + 80), pPtpClock->foreignMasterDS.foreign[manage->recordKey - 1].v1.foreignMasterIdentity.portNumber);
      PACK_32BIT((buf + 84), pPtpClock->foreignMasterDS.foreign[manage->recordKey - 1].msgCount.previous);
      return 88;

   default:
      return 0;
   }
}

/***************** END OF VERSION 1 MESSAGE FORMATS **************************/
#endif

