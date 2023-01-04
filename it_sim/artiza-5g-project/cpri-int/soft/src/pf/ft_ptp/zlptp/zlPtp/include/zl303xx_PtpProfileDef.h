

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Interface for defining and accessing profile specific routines that need
*     to executed at various points in the PTP stack processing beyond the
*     standard behavior.
*
*******************************************************************************/
#ifndef ZL303XX_PTP_PROFILE_DEF_H_
#define ZL303XX_PTP_PROFILE_DEF_H_

/****************   INCLUDE FILES   ******************************************/

/* Typical Porting headers */
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"

/* Common PTP headers */
#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpDatatypes.h"
#include "zl303xx_PtpV2MsgTypes.h"

/****************   DEFINES     **********************************************/

/* Macro for checking Profile ID. */
#define ZL303XX_CHECK_PROFILE(fnName, profileId) \
   ( ((profileId) >= ZL303XX_PTP_PROFILE_NUM_TYPES) ?  \
     (ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "%s: Invalid profileId (%u)(%u)", \
           fnName, profileId, ZL303XX_PTP_PROFILE_NUM_TYPES, 0,0,0), \
           ZL303XX_PARAMETER_INVALID) : \
           ZL303XX_OK )

/****************   TYPEDEFS    **********************************************/

/* Routine to append a TLV to the end of a transmitted general message.  */
typedef Uint16T (*zl303xx_PtpTlvAppendFnT)(
      zl303xx_ClockStreamS *pPtpStream, Uint32T messageType,
      Uint8T *pOutMsgBuf, Uint8T *pOutTlvBuf);

/****************   STATIC FUNCTION DECLARATIONS   ***************************/

/****************   STATIC GLOBAL VARIABLES   ********************************/

/****************   EXPORTED GLOBAL VARIABLES   ******************************/

/****************   EXTERNAL GLOBAL VARIABLES   ******************************/

/****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* BINDING ROUTINES (Called within the PTP Stack). */
/***************************************************/
/* Binding routine to get data about the Profile.  */
zlStatusE zl303xx_PtpProfileInfoGet(
      zl303xx_PtpProfileE profileId,
      const zl303xx_PtpProfileS **profileInfo);

/* Binding routines to set profile specific values for PTP Objects. It
 * automatically calls the regular StructInit() routine first before calling
 * the profile specific function (the latter may set only a sub-set of the
 * overall configuration).  */
zlStatusE zl303xx_PtpProfileClockStructInit(
      zl303xx_PtpProfileE profileId,
      zl303xx_PtpClockCreateS *pClockCreateS);
zlStatusE zl303xx_PtpProfilePortStructInit(
      zl303xx_PtpProfileE profileId,
      zl303xx_PtpPortCreateS *pPortCreateS);
zlStatusE zl303xx_PtpProfileStreamStructInit(
      zl303xx_PtpProfileE profileId,
      zl303xx_PtpStreamCreateS *pStreamCreateS);

/* Pre PTP Object Creation Actions */
/* For example, final checks on creation parameters.  */
zlStatusE zl303xx_PtpProfileClockPreCreate(
      zl303xx_PtpClockCreateS *pClockCreateS);
zlStatusE zl303xx_PtpProfilePortPreCreate(
      zl303xx_PtpPortCreateS *pPortCreateS,
      zl303xx_PtpClockS *pPtpParentClock);
zlStatusE zl303xx_PtpProfileStreamPreCreate(
      zl303xx_PtpStreamCreateS *pStreamCreateS,
      zl303xx_PortDataS *pPtpParentPort);

/* Post PTP Object Creation Actions */
/* For example, Profiles with custom TLV handling will need to add their OUI to
 * the VENDOR_EXTENSION custom TLV handling table of the clock.  */
zlStatusE zl303xx_PtpProfileClockPostCreate(
      zl303xx_PtpClockS *pPtpClock);
zlStatusE zl303xx_PtpProfilePortPostCreate(
      zl303xx_PortDataS *pPtpPort);
zlStatusE zl303xx_PtpProfileStreamPostCreate(
      zl303xx_ClockStreamS *pPtpStream);

/* Profile Data Set Update routines for each PTP Object */
/* For example, when the Clock ParentDS changes, certain Profile specific pieces
 * of data may need to be updated as well. Similar profile data sets exist for
 * the ports and streams as well. */
zlStatusE zl303xx_PtpProfileClockDsUpdate(
      zl303xx_PtpClockS *pPtpClock);
zlStatusE zl303xx_PtpProfilePortDsUpdate(
      zl303xx_PortDataS *pPtpPort);
zlStatusE zl303xx_PtpProfileStreamDsUpdate(
      zl303xx_ClockStreamS *pPtpStream);

/* Binding routines to Check common parameters.  */
zlStatusE zl303xx_PtpProfileDomainNumberCheck(
      zl303xx_PtpProfileE profileId,
      zl303xx_BooleanE strictFlag,
      Uint8T *domainNumber);
zlStatusE zl303xx_PtpProfileTransportSpecificCheck(
      zl303xx_PtpProfileE profileId,
      zl303xx_BooleanE strictFlag,
      Uint8T *transportSpecific);
zlStatusE zl303xx_PtpProfileClockQualityCheck(
      zl303xx_PtpProfileE profileId,
      zl303xx_BooleanE strictFlag,
      zl303xx_ClockQuality *clockQuality);
zlStatusE zl303xx_PtpProfileClockPriorityCheck(
      zl303xx_PtpProfileE profileId,
      zl303xx_BooleanE strictFlag,
      Uint8T *priority1, Uint8T *priority2,
      zl303xx_BooleanE bSlaveOnly);
zlStatusE zl303xx_PtpProfileAnncReceiptTimeoutCheck(
      zl303xx_PtpProfileE profileId,
      zl303xx_BooleanE strictFlag,
      Uint8T *anncReceiptTimeout);

/* Binding routines to Check various Interval (Packet Rate, Query Interval)  */
zlStatusE zl303xx_PtpProfileQueryIntervalCheck(
      zl303xx_PtpProfileE profileId,
      zl303xx_BooleanE strictFlag,
      Sint8T *queryInterval);
zlStatusE zl303xx_PtpProfileMsgIntervalCheck(
      zl303xx_PtpProfileE profileId,
      zl303xx_BooleanE strictFlag,
      zl303xx_MessageTypeE msgType,
      Sint8T *logInterval);

/* Binding routine to Check Unicast Contract Duration .  */
zlStatusE zl303xx_PtpProfileUnicastDurationCheck(
      zl303xx_PtpProfileE profileId,
      zl303xx_BooleanE strictFlag,
      Uint32T *duration);
/* Binding routine to Check if Unicast GRANTS are allowed on a specified Port. */
zlStatusE zl303xx_PtpProfileUnicastGrantDenyCheck(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_BooleanE *bGrantDeny);
/* Binding routine to Check if the Master-Only setting is allowed on a specified Port. */
zlStatusE zl303xx_PtpProfilePortMasterOnlyCheck(
      zl303xx_PortDataS *pPtpPort,
      zl303xx_BooleanE *bMasterOnly);

/* Binding routine to process Received Announce messages.  */
zlStatusE zl303xx_PtpProfileAnncRxCheck(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf);
/* Binding routine to Check a Stream Over-ride MODE Setting. */
zlStatusE zl303xx_PtpProfileStreamModeCheck(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpStreamModeE *streamMode);

/* Binding routine to update transmitted PTP messages header fields with Profile
 * Specific data.
 * - pPtpStream has the profileId + Clock/Port info.
 * - ptpMsgHeader has the messageType.  */
zlStatusE zl303xx_PtpProfileEgressPktHeaderGet(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgHeaderS *pPtpMsgHeader);

/* Binding routine to update transmitted Announce data with Profile Specific
 * values (ANNC Header is handled by the routine above).  */
zlStatusE zl303xx_PtpProfileAnncEgressDataGet(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgAnnounceS *pAnncData);

/* Binding routine to Handle Profile specific BMCA data from Packet streams.  */
zlStatusE zl303xx_PtpProfileBmcaDataAdd(
      zl303xx_ForeignMasterRecordS *pFmtEntry,
      zl303xx_PtpEventClockBmcaEntryS *pBmcaDataS);

/* Binding routine to Handle Profile specific BMCA data from Virtual PTP Ports.  */
zlStatusE zl303xx_PtpProfileBmcaVirtualPortAdd(
      zl303xx_PortDataS *pPtpVirtualPort,
      zl303xx_PtpEventClockBmcaEntryS *pBmcaDataS);

/* Executes the BMC comparison algorithm for the specified  profile. */
zlStatusE zl303xx_PtpProfileBmcaComparison(
      zl303xx_PtpProfileE profileId,
      zl303xx_PtpEventClockBmcaEntryS *pCompDataA,
      zl303xx_PtpEventClockBmcaEntryS *pCompDataB,
      void *pExtraData,
      Sint8T *result);

/* Binding routine to append Profile specific TLVs.  */
Uint16T zl303xx_PtpProfileTlvAppend(
      zl303xx_ClockStreamS *pPtpStream,
      Uint32T messageType,
      Uint8T *pOutMsgBuf,
      Uint8T *pOutTlvBuf);

/* Debug routines */
zlStatusE zl303xx_PtpProfileClockDataShow(
      zl303xx_PtpClockS *pPtpClock);
zlStatusE zl303xx_PtpProfilePortDataShow(
      zl303xx_PortDataS *pPtpPort);
zlStatusE zl303xx_PtpProfileStreamDataShow(
      zl303xx_ClockStreamS *pPtpStream);

/* API Routines to allow users to change the binding definitions for a profile. */
/********************************************************************************/
zlStatusE zl303xx_PtpProfileTlvAppendFnSet(
      zl303xx_PtpProfileE profileId,
      zl303xx_PtpTlvAppendFnT tlvAppendFn);

#endif /* ZL303XX_PTP_PROFILE_DEF_H_ */
