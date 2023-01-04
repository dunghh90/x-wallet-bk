

/*****************************************************************************
*
*  $Id: zl303xx_PtpV1Bmc.c 14109 2016-09-02 17:27:33Z DP $
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
*     Best Master Clock algorithm for IEEE-1588-2002 version 1
*
*******************************************************************************/

/****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_ErrTrace.h"
#include "zl303xx_Trace.h"

#include "zl303xx_Ptpd.h"

#ifdef ZL303XX_INCLUDE_PTP_V1   /* Version 1 message formats */

/****************   DEFINES     **********************************************/


/****************   STATIC FUNCTION DECLARATIONS   ***************************/

_ZL303XX_LOCAL Sint32T zl303xx_GetV1IdentifierOrder(Uint8T identifier[PTP_CODE_STRING_LENGTH]);

_ZL303XX_LOCAL void zl303xx_PtpV1CopyD0(zl303xx_MsgHeaderS *header, MsgV1Sync *sync, zl303xx_ClockStreamS *clockStream);

_ZL303XX_LOCAL zl303xx_PtpStreamSubStateE zl303xx_PtpV1BmcStateDecision(zl303xx_MsgHeaderS *header, MsgV1Sync *sync, zl303xx_ClockStreamS *clockStream);

_ZL303XX_LOCAL Sint8T zl303xx_PtpV1BmcDataSetComparison(zl303xx_MsgHeaderS *headerA, MsgV1Sync *syncA,
                     zl303xx_MsgHeaderS *headerB, MsgV1Sync *syncB, zl303xx_ClockStreamS *clockStream);


/****************   STATIC GLOBAL VARIABLES   ********************************/


/****************   EXPORTED GLOBAL VARIABLES   ******************************/


/****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/*

  Function Name:
    zl303xx_PtpV1DoM1Update

  Details:
   Performs data set updates "M1" as defined in 1588 spec v1, table 18

  Parameters:
       clockStream    Pointer to the clock stream instance data structure

  Return Value:
   Nothing

  Notes:
   M1 actions occur when a port is just entering the Master state. In other words
   its own local clock will be becoming its parent

   Also applies when update "M2" occurs

*******************************************************************************/
void zl303xx_PtpV1DoM1Update(zl303xx_ClockStreamS *clockStream)
{
   zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

   /* Clear the Clock's Current data set */
   OS_MEMSET(&pPtpClock->currentDSInt, 0, sizeof(pPtpClock->currentDSInt));

   /* Clear the Port's Current data set. */
   OS_MEMSET(&clockStream->farEndCurrentDS, 0, sizeof(clockStream->farEndCurrentDS));

   /* Parent data set */
   OS_MEMCPY(&clockStream->farEndParentDS.u.v1.parentIdentity,
          &pPtpClock->defaultDSInt.u.v1.clockIdentity,
          sizeof(V1PortIdentityS));
   clockStream->farEndParentDS.u.v1.parent_two_step_clock = pPtpClock->defaultDSInt.u.v1.clock_followup_capable;
   clockStream->farEndParentDS.u.v1.parent_last_sync_sequence_number = 0;
   clockStream->farEndParentDS.u.v1.parent_external_timing = pPtpClock->defaultDSInt.u.v1.external_timing;
   clockStream->farEndParentDS.u.v1.parent_log_variance = pPtpClock->defaultDSInt.u.v1.clock_variance;
   OS_MEMCPY(&clockStream->farEndParentDS.u.v1.grandmasterIdentity,
          &pPtpClock->defaultDSInt.u.v1.clockIdentity,
          sizeof(V1PortIdentityS));
   clockStream->farEndParentDS.u.v1.grandmaster_stratum = pPtpClock->defaultDSInt.u.v1.clock_stratum;
   OS_MEMCPY(clockStream->farEndParentDS.u.v1.grandmaster_identifier,
          pPtpClock->defaultDSInt.u.v1.clock_identifier,
          PTP_CODE_STRING_LENGTH);
   clockStream->farEndParentDS.u.v1.grandmaster_variance = pPtpClock->defaultDSInt.u.v1.clock_variance;
   clockStream->farEndParentDS.u.v1.grandmaster_preferred  = pPtpClock->defaultDSInt.u.v1.preferred;
   clockStream->farEndParentDS.u.v1.grandmaster_is_boundary_clock  = pPtpClock->defaultDSInt.u.v1.is_boundary_clock;

   /* Report M1 state to clock switch task */
   zl303xx_PtpEventStreamStateChange(clockStream, ZL303XX_PTP_STREAM_OPER_STATE_GRANDMASTER);
}

/*

  Function Name:
    zl303xx_PtpV1DoS1Update

  Details:
   Performs data set updates "S1" as defined in 1588 spec v1, table 21

  Parameters:
   [in]    header         Pointer to the header structure for the best clock
   [in]    sync           Pointer to the sync message body last received from
                  the best clock
   [in]    clockStream    Pointer to the clock stream instance data structure


  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpV1DoS1Update(zl303xx_MsgHeaderS *header, MsgV1Sync *sync, zl303xx_ClockStreamS *clockStream)
{
   zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

   /* Current data set */
   pPtpClock->currentDSInt.stepsRemoved = sync->localStepsRemoved + 1;

   /* Parent data set */
   OS_MEMCPY(&clockStream->farEndParentDS.u.v1.parentIdentity, &header->u.v1.sourceIdentity, sizeof(V1PortIdentityS));
   clockStream->farEndParentDS.u.v1.parent_two_step_clock = getFlag(header->flagField, PTP1_FLAGS_ASSIST);
   clockStream->farEndParentDS.u.v1.parent_last_sync_sequence_number = header->sequenceId;
   clockStream->farEndParentDS.u.v1.parent_external_timing = getFlag(header->flagField, PTP1_FLAGS_EXT_SYNC);
   clockStream->farEndParentDS.u.v1.parent_log_variance = sync->localClockVariance;
   OS_MEMCPY(&clockStream->farEndParentDS.u.v1.grandmasterIdentity, &sync->grandmasterIdentity, sizeof(V1PortIdentityS));
   clockStream->farEndParentDS.u.v1.grandmaster_stratum = sync->grandmasterClockStratum;
   OS_MEMCPY(clockStream->farEndParentDS.u.v1.grandmaster_identifier, sync->grandmasterClockIdentifier, PTP_CODE_STRING_LENGTH);
   clockStream->farEndParentDS.u.v1.grandmaster_variance = sync->grandmasterClockVariance;
   clockStream->farEndParentDS.u.v1.grandmaster_preferred = sync->grandmasterPreferred;
   clockStream->farEndParentDS.u.v1.grandmaster_is_boundary_clock = sync->grandmasterIsBoundaryClock;
   clockStream->farEndParentDS.u.v1.grandmaster_sequence_number = sync->grandmasterSequenceId;

   /* Global time properties data set */
   pPtpClock->timePropertiesDS.currentUtcOffset = sync->currentUtcOffset;
   pPtpClock->timePropertiesDS.leap59 = getFlag(header->flagField, PTP1_FLAGS_LI_59);
   pPtpClock->timePropertiesDS.leap61 = getFlag(header->flagField, PTP1_FLAGS_LI_61);
   pPtpClock->timePropertiesDS.epochNumber = sync->epochNumber;
}

/*

  Function Name:
    zl303xx_PtpV1CopyD0

  Details:
   Copies data into a temporary sync message from the local clock's data structure
   to allow it to be compared by the BMC set comparison algorithm.

  Parameters:
   [in]    header         Pointer to the header structure for the sync message
   [in]    sync           Pointer to the sync message body to populate
   [in]    clockStream    Pointer to the clock stream instance data structure


  Return Value:
   Nothing

  Notes:
   Creates a received sync message as if it had received from the current
   clock port. Only the data members that are required for the data set comparison
   are populated.

*******************************************************************************/
void zl303xx_PtpV1CopyD0(zl303xx_MsgHeaderS *header, MsgV1Sync *sync, zl303xx_ClockStreamS *clockStream)
{
   zl303xx_PtpClockS *pPtpClock = clockStream->portData->clock;

  OS_MEMCPY(&sync->grandmasterIdentity,
         &clockStream->portData->portConfig.u.v1.portIdentity,
         sizeof(V1PortIdentityS));
  sync->grandmasterClockStratum = pPtpClock->defaultDSInt.u.v1.clock_stratum;
  OS_MEMCPY(sync->grandmasterClockIdentifier,
         pPtpClock->defaultDSInt.u.v1.clock_identifier,
         PTP_CODE_STRING_LENGTH);
  sync->grandmasterClockVariance = pPtpClock->defaultDSInt.u.v1.clock_variance;
  sync->grandmasterIsBoundaryClock = pPtpClock->defaultDSInt.u.v1.is_boundary_clock;
  sync->grandmasterPreferred = pPtpClock->defaultDSInt.u.v1.preferred;
  sync->localStepsRemoved = pPtpClock->currentDSInt.stepsRemoved;
  OS_MEMCPY(&header->u.v1.sourceIdentity,
         &clockStream->portData->portConfig.u.v1.portIdentity,
         sizeof(V1PortIdentityS));
  sync->grandmasterSequenceId = clockStream->farEndParentDS.u.v1.grandmaster_sequence_number;
  header->sequenceId = clockStream->farEndParentDS.u.v1.grandmaster_sequence_number;
}

/*

  Function Name:
    zl303xx_GetV1dentifierOrder

  Details:
   Convert an ASCII identifier string from a PTP V1 packet into a priority order
   number

  Parameters:
   [in]    identifier     ASCII identifier string. One of the standard strings defined
                  in IEEE1588 v1 spec.

  Return Value:
   A priority order number in range 1 (highest) to 6 (lowest)

 *****************************************************************************/
Sint32T zl303xx_GetV1IdentifierOrder(Uint8T identifier[PTP_CODE_STRING_LENGTH])
{
  if (!memcmp(identifier, IDENTIFIER_ATOM, PTP_CODE_STRING_LENGTH))
    return 1;
  else if (!memcmp(identifier, IDENTIFIER_GPS, PTP_CODE_STRING_LENGTH))
    return 1;
  else if (!memcmp(identifier, IDENTIFIER_NTP, PTP_CODE_STRING_LENGTH))
    return 2;
  else if (!memcmp(identifier, IDENTIFIER_HAND, PTP_CODE_STRING_LENGTH))
    return 3;
  else if (!memcmp(identifier, IDENTIFIER_INIT, PTP_CODE_STRING_LENGTH))
    return 4;
  else if (!memcmp(identifier, IDENTIFIER_DFLT, PTP_CODE_STRING_LENGTH))
    return 5;

  return 6;
}

/*

  Function Name:
    zl303xx_PtpV1BmcDataSetComparison

  Details:
   Compares two data sets according to IEEE 1588 v1 spec.

  Parameters:
   [in]    headerA        Header structure from first received message to compare
   [in]    syncA          Sync message body from first received message to compare
   [in]    headerB        Header structure from second received message to compare
   [in]    syncB          Sync message body from second received message to compare
   [in]    clockStream    Pointer to the clock stream instance data structure


  Return Value:
   <0 if B is better than A
    0 if data sets are equal
   >0 if A is better than B

  Notes:
   communicationTechnology is ignored because if they differed they would not
   have made it here

*******************************************************************************/
Sint8T zl303xx_PtpV1BmcDataSetComparison(zl303xx_MsgHeaderS *headerA, MsgV1Sync *syncA,
                     zl303xx_MsgHeaderS *headerB, MsgV1Sync *syncB, zl303xx_ClockStreamS *clockStream)
{
  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,3, "bmcDataSetComparison: start", 0,0,0,0,0,0);
  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "syncA->grandmasterPortId = %d", syncA->grandmasterIdentity.portNumber, 0,0,0,0,0);
  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "syncA->grandmasterClockUuid = %02X:%02X:%02X:%02X:%02X:%02X",
              syncA->grandmasterIdentity.uuid[0], syncA->grandmasterIdentity.uuid[1], syncA->grandmasterIdentity.uuid[2],
              syncA->grandmasterIdentity.uuid[3], syncA->grandmasterIdentity.uuid[4], syncA->grandmasterIdentity.uuid[5]);
  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "syncB->grandmasterPortId = %d", syncB->grandmasterIdentity.portNumber, 0,0,0,0,0);
  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "syncB->grandmasterClockUuid = %02X:%02X:%02X:%02X:%02X:%02X",
              syncB->grandmasterIdentity.uuid[0], syncB->grandmasterIdentity.uuid[1], syncB->grandmasterIdentity.uuid[2],
              syncB->grandmasterIdentity.uuid[3], syncB->grandmasterIdentity.uuid[4], syncB->grandmasterIdentity.uuid[5]);
  if (memcmp(syncA->grandmasterIdentity.uuid, syncB->grandmasterIdentity.uuid, PTP_V1_UUID_LENGTH))
  { /* Grandmaster UUIDs are different */
    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "syncA->grandmasterClockStratum = %d \tsyncB->grandmasterClockStratum = %d",
                  syncA->grandmasterClockStratum, syncB->grandmasterClockStratum,0,0,0,0);
    if (syncA->grandmasterClockStratum < syncB->grandmasterClockStratum)
      goto A;
    else if (syncA->grandmasterClockStratum > syncB->grandmasterClockStratum)
      goto B;

    /* grandmasterClockStratums same */
    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "syncA->identifierOrder = %d \tsyncB->identifierOrder = %d",
               zl303xx_GetV1IdentifierOrder(syncA->grandmasterClockIdentifier), zl303xx_GetV1IdentifierOrder(syncB->grandmasterClockIdentifier),0,0,0,0);
    if (zl303xx_GetV1IdentifierOrder(syncA->grandmasterClockIdentifier) < zl303xx_GetV1IdentifierOrder(syncB->grandmasterClockIdentifier))
      goto A;
    if (zl303xx_GetV1IdentifierOrder(syncA->grandmasterClockIdentifier) > zl303xx_GetV1IdentifierOrder(syncB->grandmasterClockIdentifier))
      goto B;

    /* grandmasterClockIdentifiers same */
    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "syncA->grandmasterClockStratum = %d", syncA->grandmasterClockStratum, 0,0,0,0,0);
    if (syncA->grandmasterClockStratum > 2)
    {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "syncA->grandmasterClockVariance = %d \tsyncB->grandmasterClockVariance = %d",
                     syncA->grandmasterClockVariance, syncB->grandmasterClockVariance, 0,0,0,0);
      if (syncA->grandmasterClockVariance > syncB->grandmasterClockVariance + PTP_LOG_VARIANCE_THRESHOLD
        || syncA->grandmasterClockVariance < syncB->grandmasterClockVariance - PTP_LOG_VARIANCE_THRESHOLD)
      {
        /* grandmasterClockVariances are not similar */
        if (syncA->grandmasterClockVariance < syncB->grandmasterClockVariance)
          goto A;
        else
          goto B;
      }
      /* grandmasterClockVariances are similar */
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "syncA->grandmasterIsBoundaryClock = %d \tsyncB->grandmasterIsBoundaryClock = %d",
                        syncA->grandmasterIsBoundaryClock, syncB->grandmasterIsBoundaryClock,0,0,0,0);
      if (!syncA->grandmasterIsBoundaryClock != !syncB->grandmasterIsBoundaryClock) 
      {
        if (syncA->grandmasterIsBoundaryClock)
          goto A;
        else
          goto B;
      }
      /* neither is grandmasterIsBoundaryClock */
     ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "syncA->grandmasterClockUuid = %02X:%02X:%02X:%02X:%02X:%02X",
               syncA->grandmasterIdentity.uuid[0], syncA->grandmasterIdentity.uuid[1], syncA->grandmasterIdentity.uuid[2],
               syncA->grandmasterIdentity.uuid[3], syncA->grandmasterIdentity.uuid[4], syncA->grandmasterIdentity.uuid[5]);
     ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "syncB->grandmasterClockUuid = %02X:%02X:%02X:%02X:%02X:%02X",
               syncB->grandmasterIdentity.uuid[0], syncB->grandmasterIdentity.uuid[1], syncB->grandmasterIdentity.uuid[2],
               syncB->grandmasterIdentity.uuid[3], syncB->grandmasterIdentity.uuid[4], syncB->grandmasterIdentity.uuid[5]);
     if (memcmp(syncA->grandmasterIdentity.uuid, syncB->grandmasterIdentity.uuid, PTP_V1_UUID_LENGTH) < 0)
        goto A;
      else
        goto B;
    }

    /* syncA->grandmasterClockStratum <= 2 */
    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "syncA->grandmasterPreferred = %d \tsyncB->grandmasterPreferred = %d",
               syncA->grandmasterPreferred, syncB->grandmasterPreferred,0,0,0,0);
    if (!syncA->grandmasterPreferred != !syncB->grandmasterPreferred) 
    {
      if (syncA->grandmasterPreferred)
        return 1;  /* A1 */
      else
        return -1;  /* B1 */
    }
    /* neither or both grandmasterPreferred */
  }

  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,3, "bmcDataSetComparison: X", 0,0,0,0,0,0);
  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "syncA->localStepsRemoved = %d \tsyncB->localStepsRemoved = %d",
                                                syncA->localStepsRemoved, syncB->localStepsRemoved,0,0,0,0);
  if (syncA->localStepsRemoved > syncB->localStepsRemoved + 1
    || syncA->localStepsRemoved < syncB->localStepsRemoved -1)
  {
    /* localStepsRemoved not within 1 */
    if (syncA->localStepsRemoved < syncB->localStepsRemoved)
      return 1;  /* A1 */
    else
      return -1;  /* B1 */
  }

  /* localStepsRemoved within 1 */
  if (syncA->localStepsRemoved < syncB->localStepsRemoved)
  {
    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,3, "bmcDataSetComparison: A3", 0,0,0,0,0,0);
    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "pPtpClock->port_uuid_field = %02X:%02X:%02X:%02X:%02X:%02X",
               clockStream->portData->portConfig.u.v1.portIdentity.uuid[0], clockStream->portData->portConfig.u.v1.portIdentity.uuid[1],
               clockStream->portData->portConfig.u.v1.portIdentity.uuid[2], clockStream->portData->portConfig.u.v1.portIdentity.uuid[3],
               clockStream->portData->portConfig.u.v1.portIdentity.uuid[4], clockStream->portData->portConfig.u.v1.portIdentity.uuid[5]);
    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "headerB->sourceUuid = %02X:%02X:%02X:%02X:%02X:%02X",
               headerB->u.v1.sourceIdentity.uuid[0], headerB->u.v1.sourceIdentity.uuid[1],
               headerB->u.v1.sourceIdentity.uuid[2], headerB->u.v1.sourceIdentity.uuid[3],
               headerB->u.v1.sourceIdentity.uuid[4], headerB->u.v1.sourceIdentity.uuid[5]);

    if (memcmp(clockStream->portData->portConfig.u.v1.portIdentity.uuid, headerB->u.v1.sourceIdentity.uuid, PTP_V1_UUID_LENGTH) < 0)
      return 1;  /* A1 */
    else if (memcmp(clockStream->portData->portConfig.u.v1.portIdentity.uuid, headerB->u.v1.sourceIdentity.uuid, PTP_V1_UUID_LENGTH) > 0)
      return 2;  /* A2 */

    /* this port_uuid_field same as headerB->sourceUuid */
    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "pPtpClock->port_id_field = %d, headerB->sourcePortId = %d",
               clockStream->portData->portConfig.u.v1.portIdentity.portNumber, headerB->u.v1.sourceIdentity.portNumber, 0,0,0,0);
    if (clockStream->portData->portConfig.u.v1.portIdentity.portNumber < headerB->u.v1.sourceIdentity.portNumber)
      return 1;  /* A1 */
    else if (clockStream->portData->portConfig.u.v1.portIdentity.portNumber > headerB->u.v1.sourceIdentity.portNumber)
      return 2;  /* A2 */

    /* this port_id_field same as headerB->sourcePortId */
    return 0;  /* same */
  }
  if (syncA->localStepsRemoved > syncB->localStepsRemoved)
  {
    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,3, "bmcDataSetComparison: B3", 0,0,0,0,0,0);
    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "pPtpClock->port_uuid_field = %02X:%02X:%02X:%02X:%02X:%02X",
               clockStream->portData->portConfig.u.v1.portIdentity.uuid[0], clockStream->portData->portConfig.u.v1.portIdentity.uuid[1],
               clockStream->portData->portConfig.u.v1.portIdentity.uuid[2], clockStream->portData->portConfig.u.v1.portIdentity.uuid[3],
               clockStream->portData->portConfig.u.v1.portIdentity.uuid[4], clockStream->portData->portConfig.u.v1.portIdentity.uuid[5]);
    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "headerA->sourceUuid = %02X:%02X:%02X:%02X:%02X:%02X",
               headerA->u.v1.sourceIdentity.uuid[0], headerA->u.v1.sourceIdentity.uuid[1], headerA->u.v1.sourceIdentity.uuid[2],
               headerA->u.v1.sourceIdentity.uuid[3], headerA->u.v1.sourceIdentity.uuid[4], headerA->u.v1.sourceIdentity.uuid[5]);
    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "headerB->sourceUuid = %02X:%02X:%02X:%02X:%02X:%02X",
               headerB->u.v1.sourceIdentity.uuid[0], headerB->u.v1.sourceIdentity.uuid[1], headerB->u.v1.sourceIdentity.uuid[2],
               headerB->u.v1.sourceIdentity.uuid[3], headerB->u.v1.sourceIdentity.uuid[4], headerB->u.v1.sourceIdentity.uuid[5]);
    if (memcmp(clockStream->portData->portConfig.u.v1.portIdentity.uuid, headerA->u.v1.sourceIdentity.uuid, PTP_V1_UUID_LENGTH) < 0)
      return -1;  /* B1 */
    else if (memcmp(clockStream->portData->portConfig.u.v1.portIdentity.uuid, headerB->u.v1.sourceIdentity.uuid, PTP_V1_UUID_LENGTH) > 0)
      return -2;  /* B2 */

    /* this port_uuid_field same as headerA->sourceUuid */
    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "pPtpClock->port_id_field = %d, headerA->sourcePortId = %d",
                     clockStream->portData->portConfig.u.v1.portIdentity.portNumber, headerA->u.v1.sourceIdentity.portNumber, 0,0,0,0);
    if (clockStream->portData->portConfig.u.v1.portIdentity.portNumber < headerA->u.v1.sourceIdentity.portNumber)
      return -1;  /* B1 */
    else if (clockStream->portData->portConfig.u.v1.portIdentity.portNumber > headerA->u.v1.sourceIdentity.portNumber)
      return -2;  /* B2 */

    /* this port_id_field same as headerA->sourcePortId */
    return 0;  /* same */
  }

  /* localStepsRemoved same */
  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "headerA->sourceUuid = %02X:%02X:%02X:%02X:%02X:%02X",
               headerA->u.v1.sourceIdentity.uuid[0], headerA->u.v1.sourceIdentity.uuid[1], headerA->u.v1.sourceIdentity.uuid[2],
               headerA->u.v1.sourceIdentity.uuid[3], headerA->u.v1.sourceIdentity.uuid[4], headerA->u.v1.sourceIdentity.uuid[5]);
  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "headerB->sourceUuid = %02X:%02X:%02X:%02X:%02X:%02X",
               headerB->u.v1.sourceIdentity.uuid[0], headerB->u.v1.sourceIdentity.uuid[1], headerB->u.v1.sourceIdentity.uuid[2],
               headerB->u.v1.sourceIdentity.uuid[3], headerB->u.v1.sourceIdentity.uuid[4], headerB->u.v1.sourceIdentity.uuid[5]);
  if (memcmp(headerA->u.v1.sourceIdentity.uuid, headerB->u.v1.sourceIdentity.uuid, PTP_V1_UUID_LENGTH) < 0)
    return 2;  /* A2 */
  else if (memcmp(headerA->u.v1.sourceIdentity.uuid, headerB->u.v1.sourceIdentity.uuid, PTP_V1_UUID_LENGTH) > 0)
    return -2;  /* B2 */

  /* sourceUuid same */
  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,3, "bmcDataSetComparison: Z", 0,0,0,0,0,0);
  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "syncA->grandmasterSequenceId = %d \tsyncB->grandmasterSequenceId = %d",
                              syncA->grandmasterSequenceId, syncB->grandmasterSequenceId, 0,0,0,0);
  if (syncA->grandmasterSequenceId > syncB->grandmasterSequenceId)
    return 3;
  else if (syncA->grandmasterSequenceId < syncB->grandmasterSequenceId)
    return -3;

  /* grandmasterSequenceId same */
  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "headerA->sequenceId = %d,  headerB->sequenceId = %d",
                                                   headerA->sequenceId,  headerB->sequenceId, 0,0,0,0);
  if (headerA->sequenceId > headerB->sequenceId)
    return 3;
  else if (headerA->sequenceId < headerB->sequenceId)
    return -3;

  /* sequenceId same */
  return 0;  /* same */

  /* oh no, a goto label! the horror! */
A:
  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "syncA->grandmasterPreferred = %d,  syncB->grandmasterPreferred = %d",
                                         syncA->grandmasterPreferred,  syncB->grandmasterPreferred, 0,0,0,0);
  if (!syncA->grandmasterPreferred && syncB->grandmasterPreferred)
    return -1;  /* B1 */
  else
    return 1;  /* A1 */
B:
  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,5, "syncA->grandmasterPreferred = %d,  syncB->grandmasterPreferred = %d",
                                         syncA->grandmasterPreferred,  syncB->grandmasterPreferred, 0,0,0,0);
  if (syncA->grandmasterPreferred && !syncB->grandmasterPreferred)
    return 1;  /* A1 */
  else
    return -1;  /* B1 */
}

/*

  Function Name:
    zl303xx_PtpV1BmcStateDecision

  Details:
   Implements the state decision behaviour required by the Best Master Clock
   algorithm for IEEE 1588 v1 spec by comparing the contents of a received sync
   message with the configuration of the current port data set

  Parameters:
   [in]    header         Header structure from a received message to compare
   [in]    sync           Sync message body from first received message to compare
   [in]    clockStream    Pointer to the clock stream instance data structure


  Return Value:
   Recommended next state

*******************************************************************************/
zl303xx_PtpStreamSubStateE zl303xx_PtpV1BmcStateDecision(zl303xx_MsgHeaderS *header,
                                                    MsgV1Sync *sync,
                                                    zl303xx_ClockStreamS *clockStream)
{
   /* Temporary storage of the default data set (D0) for comparison. */
   zl303xx_MsgHeaderS d0Header;
   MsgV1Sync d0Sync;

  if (clockStream->config.mode == ZL303XX_PTP_STREAM_SLAVE_ONLY)
  {
    zl303xx_PtpV1DoS1Update(header, sync, clockStream);

    /* Report S1 state to clock switch task */
    zl303xx_PtpEventStreamStateChange(clockStream, ZL303XX_PTP_STREAM_OPER_STATE_SLAVE);

    return PTP_STREAM_SUBSTATE_SLAVE;
  }
  else if (clockStream->config.mode == ZL303XX_PTP_STREAM_MASTER_ONLY)
  {
    zl303xx_PtpV1DoM1Update(clockStream);
    return PTP_STREAM_SUBSTATE_MASTER;
  }

   /* Allow BMC algorithm to determine state. */
   /* Copy the clock data to temporary structures for comparison. */
   zl303xx_PtpV1CopyD0(&d0Header, &d0Sync, clockStream);

   if (d0Sync.grandmasterClockStratum < 3)
  {
      if (zl303xx_PtpV1BmcDataSetComparison(&d0Header, &d0Sync, header, sync, clockStream) > 0)
    {
      zl303xx_PtpV1DoM1Update(clockStream);
      return PTP_STREAM_SUBSTATE_MASTER;
    }
    zl303xx_PtpV1DoS1Update(header, sync, clockStream);

    /* Report P1 state to clock switch task */
    zl303xx_PtpEventStreamStateChange(clockStream, ZL303XX_PTP_STREAM_OPER_STATE_PASSIVE_MASTER);

    return PTP_STREAM_SUBSTATE_PASSIVE;
  }
   else if (zl303xx_PtpV1BmcDataSetComparison(&d0Header, &d0Sync, header, sync, clockStream) > 0)
  {
    zl303xx_PtpV1DoM1Update(clockStream);
    return PTP_STREAM_SUBSTATE_MASTER;
  }
  else
  {
    zl303xx_PtpV1DoS1Update(header, sync, clockStream);

    /* Report S1 state to clock switch task */
    zl303xx_PtpEventStreamStateChange(clockStream, ZL303XX_PTP_STREAM_OPER_STATE_SLAVE);

    return PTP_STREAM_SUBSTATE_SLAVE;
  }
}


/*

  Function Name:
    zl303xx_PtpV1Bmc

  Details:
   Implements the Best Master Clock algorithm for IEEE 1588 v1 spec by comparing
   all visible foreign masters and the current clock the determine the best master

  Parameters:
   [in]  fmTbl          Pointer to the Foreign Master Table for which an update
                              has been received and the BMC should be re-evaluated.
   [in]  clockStream    Pointer to the clock stream instance data structure


  Return Value:
   Recommended next state

 *****************************************************************************/
zl303xx_PtpStreamSubStateE zl303xx_PtpV1Bmc(zl303xx_ForeignMasterDS *fmTbl, zl303xx_ClockStreamS *clockStream)
{
  Uint16T tblIndex, best;
  Uint16T nQualifiedForeignRecords = 0; /* non-zero if we have found at least one qualifying clock */

  /* Find the best qualifying clock in the list of foreign records */
  /* We could enter this function if we have received a Sync message or if a
     management message has been received.
     It will only be entered from the following states:
      PTP_V1_LISTENING, PTP_V1_MASTER, PTP_V1_SLAVE */

  /* Find a qualifying clock */
  tblIndex = 0;
  while (tblIndex < fmTbl->numActiveRecords)
  {
    if (fmTbl->foreign[tblIndex].msgCount.previous >= ZL303XX_PTP_DEFAULT_FOREIGN_WINDOW_THRESHOLD)
    {
       /* Clock is "qualified" */
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,4,
                    "bmc: qualified clock record %d",
                    tblIndex, 0,0,0,0,0);

      nQualifiedForeignRecords++;
      break;
    }
    ++tblIndex;
  }
  best = tblIndex; /* This is so far the best clock we have seen if any */

  if (!nQualifiedForeignRecords)
  {
     /* No qualifying records to compare. We must be the best clock */
    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,4,
                  "bmc: Num foreign records = %d, Num qualifying = %d",
                  fmTbl->numActiveRecords,
                  nQualifiedForeignRecords, 0,0,0,0);

    if ((clockStream->streamSubstate == PTP_STREAM_SUBSTATE_LISTENING) ||
       (clockStream->streamSubstate == PTP_STREAM_SUBSTATE_MASTER))
    {
      /* Update current clock parameters from default record set */
      zl303xx_PtpV1DoM1Update(clockStream);
      return clockStream->streamSubstate;  /* no change */
    }
    else
    { /* currently in PTP_V1_SLAVE state but there are no qualifying foreign records. Therefore
         we must consider ourselves to be the "best" clock so switch to master mode
         if we are allowed */
      if (clockStream->config.mode != ZL303XX_PTP_STREAM_SLAVE_ONLY)
      {
        zl303xx_PtpV1DoM1Update(clockStream);
        return PTP_STREAM_SUBSTATE_MASTER;
      }
      else
      {
        return clockStream->streamSubstate;  /* no change */
      }
    }
  }
  else /* (nQualifiedForeignRecords > 0) */
  {
    /* Find another qualifying clock, if any, and compare to find the best */
    ++tblIndex;
    while (tblIndex < fmTbl->numActiveRecords)
    {
      if (fmTbl->foreign[tblIndex].msgCount.previous >= ZL303XX_PTP_DEFAULT_FOREIGN_WINDOW_THRESHOLD)
      {
         /* Clock is "qualified" */
        ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,4, ""
                      "bmc: qualified clock record %d",
                      tblIndex, 0,0,0,0,0);

        nQualifiedForeignRecords++;
        if (zl303xx_PtpV1BmcDataSetComparison(&fmTbl->foreign[tblIndex].v1.header, &fmTbl->foreign[tblIndex].v1.sync,
                                           &fmTbl->foreign[best].v1.header, &fmTbl->foreign[best].v1.sync,
                                           clockStream) > 0)
          best = tblIndex;
      }
      ++tblIndex;
    }
  }

  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,3,
                "bmc: Num foreign records = %d, Num qualifying = %d",
                fmTbl->numActiveRecords,
                nQualifiedForeignRecords, 0,0,0,0);

  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,3,
                "bmc: best record %d", best, 0,0,0,0,0);

  if (fmTbl->bestRecord != best)
  {
     ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,1,
                   "New best master detected.", 0,0,0,0,0,0);
     ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,1,
                   "  source uuid = %02X:%02X:%02X:%02X:%02X:%02X",
                   fmTbl->foreign[best].v1.foreignMasterIdentity.uuid[0],
                   fmTbl->foreign[best].v1.foreignMasterIdentity.uuid[1],
                   fmTbl->foreign[best].v1.foreignMasterIdentity.uuid[2],
                   fmTbl->foreign[best].v1.foreignMasterIdentity.uuid[3],
                   fmTbl->foreign[best].v1.foreignMasterIdentity.uuid[4],
                   fmTbl->foreign[best].v1.foreignMasterIdentity.uuid[5]);
     ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC,1,
                   "  grandmaster uuid = %02X:%02X:%02X:%02X:%02X:%02X",
                   fmTbl->foreign[best].v1.sync.grandmasterIdentity.uuid[0],
                   fmTbl->foreign[best].v1.sync.grandmasterIdentity.uuid[1],
                   fmTbl->foreign[best].v1.sync.grandmasterIdentity.uuid[2],
                   fmTbl->foreign[best].v1.sync.grandmasterIdentity.uuid[3],
                   fmTbl->foreign[best].v1.sync.grandmasterIdentity.uuid[4],
                   fmTbl->foreign[best].v1.sync.grandmasterIdentity.uuid[5]);
     fmTbl->bestRecord = best;
  }

  return zl303xx_PtpV1BmcStateDecision(&fmTbl->foreign[best].v1.header,
                                       &fmTbl->foreign[best].v1.sync,
                                       clockStream);
}

#endif   /* ZL303XX_INCLUDE_PTP_V1 */
