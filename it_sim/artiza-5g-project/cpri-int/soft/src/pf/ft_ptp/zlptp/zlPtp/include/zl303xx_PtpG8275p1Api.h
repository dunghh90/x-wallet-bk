

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This header file contains the data types and functions required to support
*     the G.8275.1 standard (Telecom Profile for Phase).
*
*******************************************************************************/

#ifndef ZL303XX_PTP_G8275_1_API_H_
#define ZL303XX_PTP_G8275_1_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"        /* Basic ZL datatypes. */
#include "zl303xx_Trace.h"         /* TRACE and logging functionality. */
#include "zl303xx_Error.h"         /* For zlStatusE return type. */

#include "zl303xx_PtpG8275p1Types.h"  /* Types for this profile. */
#include "zl303xx_PtpStdTypes.h"      /* Native PTP types (standard types). */
#include "zl303xx_PtpApiTypes.h"      /* User Implementation specific types. */
#include "zl303xx_PtpDatatypes.h"     /* Internal Implementation specific types. */

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   PROFILE BINDING FUNCTION DECLARATIONS   ******************/

/* Routines to get data about the Profile or execute profile specific actions.
 * Ideally, these would be bound to the parent stack.  */
zlStatusE zl303xx_PtpG8275p1v1ProfileInfoGet(const zl303xx_PtpProfileS **profileInfo);
zlStatusE zl303xx_PtpG8275p1v2ProfileInfoGet(const zl303xx_PtpProfileS **profileInfo);
zlStatusE zl303xx_PtpG8275p2ProfileInfoGet  (const zl303xx_PtpProfileS **profileInfo);

/* Routines to set Profile specific default initialization values for PTP objects.
 * Ideally, these would be bound to the parent stack. */
/* G.8275.1 */
zlStatusE zl303xx_PtpG8275p1ClockDefaults (void *pClockInitS);
zlStatusE zl303xx_PtpG8275p1PortDefaults  (void *pPortInitS);
zlStatusE zl303xx_PtpG8275p1StreamDefaults(void *pStreamInitS);
/* G.8275.2 */
zlStatusE zl303xx_PtpG8275p2ClockDefaults (void *pClockInitS);
zlStatusE zl303xx_PtpG8275p2PortDefaults  (void *pPortInitS);
zlStatusE zl303xx_PtpG8275p2StreamDefaults(void *pStreamInitS);

/* Routines to do so Profile specific actions and checks prior to the objects
 * being created. This may include final range checking and/or ensuring
 * consistency amongst members (especially deprecated ones). */
zlStatusE zl303xx_PtpG8275ClockPreCreate (void *pClockCreateS,  void *pClockParentS);
zlStatusE zl303xx_PtpG8275PortPreCreate  (void *pPortCreateS,   void *pPortParentS);
zlStatusE zl303xx_PtpG8275StreamPreCreate(void *pStreamCreateS, void *pStreamParentS);

/* Routines to execute any Post-PTP Object creation actions for the profile.
 * For example, BIND a custom TLV handler to the clock or set internal parameters. */
/* Currently nothing defined for the Port and Stream. */
zlStatusE zl303xx_PtpG8275ClockCreateActions(void *pClockS);

/* Routines to exercise Profile specific Data Set updates. */
zlStatusE zl303xx_PtpG8275ClockDataUpdate(void *pClock);

/* Routines to do common parameter checks against Profile ranges, etc. */
zlStatusE zl303xx_PtpG8275TransportSpecificCheck(
      Uint8T *transportSpecific,
      zl303xx_BooleanE strictFlag);
zlStatusE zl303xx_PtpG8275p1v1ClockQualityCheck(
      zl303xx_ClockQuality *clockQuality,
      zl303xx_BooleanE strictFlag);
zlStatusE zl303xx_PtpG8275PriorityCheck(
      Uint8T *priority1, Uint8T *priority2, zl303xx_BooleanE slaveOnly,
      zl303xx_BooleanE strictFlag);
zlStatusE zl303xx_PtpG8275GrantDenyCheck(
      zl303xx_PtpClockS *pPtpClock,
      void *bInGrantDeny);
zlStatusE zl303xx_PtpG8275AnncRxCheck(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf);
zlStatusE zl303xx_PtpG8275StreamModeCheck(
      zl303xx_ClockStreamS *pPtpStream,
      void *pInStreamMode);
zlStatusE zl303xx_PtpG8275PortMasterOnlyCheck(
      zl303xx_PortDataS *pPtpPort,
      void *bInMasterOnly);

/* Routine to gather the proper data values of egress PTP header fields for
 * messages transmitted from clocks running this Profile.  */
zlStatusE zl303xx_PtpG8275PtpMsgHeaderGet(
      zl303xx_ClockStreamS *pPtpStream,
      void *pOutPtpMsgHeader);

/* Routine to gather the proper data values of egress Announce data for clocks
 * running this Profile.  */
zlStatusE zl303xx_PtpG8275AnncDataGet(
      zl303xx_ClockStreamS *pPtpStream,
      void *pOutAnncData);

/* Routine to Handle Profile specific BMCA data.  */
zlStatusE zl303xx_PtpG8275BmcaDataAdd(          /* Get BMCA data from the FMT record. */
      zl303xx_ForeignMasterRecordS *pFmtEntry,
      zl303xx_PtpEventClockBmcaEntryS *pBmcaDataS);
zlStatusE zl303xx_PtpG8275BmcaStreamDataAdd(    /* Get BMCA data from the Stream data structure. */
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpG8275ProfileCompareDataS *pBmcaDataS);

/* Routine to Handle Profile specific BMCA data for Virtual PTP Ports.  */
zlStatusE zl303xx_PtpG8275BmcaVpDataAdd(
      zl303xx_PortDataS *pPtpVirtualPort,
      void *pOutBmcaDataS);

/* Routines to check PtpMessage rates for the profile. */
zlStatusE zl303xx_PtpG8275p1MsgIntervalCheck(
      Uint32T messageType,
      Sint8T *log2MsgInterval,
      zl303xx_BooleanE strictFlag);
zlStatusE zl303xx_PtpG8275p2MsgIntervalCheck(
      Uint32T messageType,
      Sint8T *log2MsgInterval,
      zl303xx_BooleanE strictFlag);

/* Routine to add necessary TLVs to transmitted Announce messages. */
Uint16T zl303xx_PtpG8275TlvAppend(
      zl303xx_ClockStreamS *pPtpStream,
      Uint32T messageType,
      Uint8T *pOutMsgBuf,
      Uint8T *pOutTlvBuf);
/* Routine to process the profile specific TLV. */
Uint16T zl303xx_PtpG8275TlvHandle(
      Uint32T streamHandle,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf,
      Uint8T *pOutTlvBuf);

/* Routines to print profile specific debug information for each of the PTP objects. */
zlStatusE zl303xx_PtpG8275ClockDebugShow (void *pClockS);
zlStatusE zl303xx_PtpG8275PortDebugShow  (void *pPortS);
zlStatusE zl303xx_PtpG8275StreamDebugShow(void *pStreamS);

/*****************   API FUNCTIONS FOR PROFILE ATTRIBUTES   *******************/

/* Telecom Profile for Phase-specific functions. */

/*** CLOCK Related ***/
zlStatusE zl303xx_PtpG8275ClockStateGet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpG8275p1ClockStateE *clkState,
      zl303xx_PtpG8275p1ClockClassDescE *g8275ClassDesc);
zlStatusE zl303xx_PtpG8275ClockLocalPrioritySet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint8T localPriority);
zlStatusE zl303xx_PtpG8275p1ClockClassEvalMethodSet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpG8275p1ClassEvalMethodE evalMethodId);
zlStatusE zl303xx_PtpG8275p1ClockEquipmentClassSet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint8T equipmentClass);
zlStatusE zl303xx_PtpG8275p1ClockBypassEnable(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_BooleanE enableBypass);
zlStatusE zl303xx_PtpG8275ClockHoldoverSupportedSet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_BooleanE holdoverSupported);
zlStatusE zl303xx_PtpG8275ClockTimestampIfRateTlvEn(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_BooleanE tsIfRateTlvEn);

/*** PORT Related ***/
zlStatusE zl303xx_PtpG8275PortLocalPrioritySet(
      zl303xx_PtpPortHandleT portHandle,
      Uint8T localPriority);
zlStatusE zl303xx_PtpG8275PortTimestampIfRateSet(
      zl303xx_PtpPortHandleT portHandle,
      zl303xx_PtpG8275TimestampIfRateTlvS *tsIfRateS);
zlStatusE zl303xx_PtpG8275PortTimestampIfRateClear(
      zl303xx_PtpPortHandleT portHandle);

/*** STREAM Related ***/
zlStatusE zl303xx_PtpG8275StreamLocalPrioritySet(
      zl303xx_PtpStreamHandleT streamHandle,
      Uint8T localPriority);
zlStatusE zl303xx_PtpG8275StreamMasterOnlySet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_BooleanE bMasterOnly);

zlStatusE zl303xx_PtpG8275StreamTimestampIfRateGet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_PtpG8275TimestampIfRateTlvS *tsIfRateS);

/** Deprecated:
 *  The notSlave definition was changed by Ammendment-1 of G.8275.1 to the
 *  port::masterOnly member. The following definitions were added for the
 *  purpose of backwards compatibility but should not be used in new
 *  applications. */
#define zl303xx_PtpG8275p1PortNotSlaveSet    zl303xx_PtpPortMasterOnlySet
#define zl303xx_PtpG8275p1StreamNotSlaveSet  zl303xx_PtpG8275StreamMasterOnlySet

/*****************   UTILITY FUNCTION DECLARATIONS   **************************/

const char *zl303xx_PtpG8275p1ClockStateToStr(zl303xx_PtpG8275p1ClockStateE clkState);
const char *zl303xx_PtpG8275p1ClockClassDescToStr(zl303xx_PtpG8275p1ClockClassDescE clkClassDescr);
const char *zl303xx_PtpG8275p1ClassEvalMethodToStr(zl303xx_PtpG8275p1ClassEvalMethodE classEvalMethod);

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */


