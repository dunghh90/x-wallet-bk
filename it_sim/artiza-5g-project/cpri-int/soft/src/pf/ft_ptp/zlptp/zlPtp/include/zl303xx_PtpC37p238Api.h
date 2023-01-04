

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This header file contains the API required to support the PC37.238-2011
*     standard (Power Profile).
*
*******************************************************************************/

#ifndef ZL303XX_PTP_C37_238_API_H_
#define ZL303XX_PTP_C37_238_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"        /* Basic ZL datatypes. */
#include "zl303xx_Trace.h"         /* TRACE and logging functionality. */
#include "zl303xx_Error.h"         /* For zlStatusE return type. */

#include "zl303xx_PtpC37p238Types.h"  /* Types for this profile. */
#include "zl303xx_PtpStdTypes.h"      /* Native PTP types (standard types). */
#include "zl303xx_PtpApiTypes.h"      /* User Implementation specific types. */
#include "zl303xx_PtpDatatypes.h"     /* Internal Implementation specific types. */

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   INTERNAL PTP FUNCTION DECLARATIONS   *********************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/******************************************************************************/
/*******   PROFILE SPECIFIC API FUNCTIONS   ***********************************/
/******************************************************************************/

/* Routines to set the expected TLV behavior. */
zlStatusE zl303xx_PtpC37p238StreamProfileTlvCfgSet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_BooleanE required,
      zl303xx_BooleanE append);
zlStatusE zl303xx_PtpC37p238StreamProfileTlvVersionSet(
      zl303xx_PtpStreamHandleT streamHandle,
      Uint32T profileVer);
zlStatusE zl303xx_PtpC37p238StreamAltTimeTlvCfgSet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_BooleanE required,
      zl303xx_BooleanE append);


/******************************************************************************/
/*******   PROFILE DEFINITION FUNCTION BINDINGS (See PtpProfileDef.c)   *******/
/******************************************************************************/

/* Routines to get data about the Profile or execute profile specific actions.
 * Ideally, these would be bound to the parent stack.  */
zlStatusE zl303xx_PtpC37p238v1ProfileInfoGet(const zl303xx_PtpProfileS **profileInfo);
zlStatusE zl303xx_PtpC37p238v2ProfileInfoGet(const zl303xx_PtpProfileS **profileInfo);

/* Routines to set Profile specific default initialization values for PTP objects.
 * Ideally, these would be bound to the parent stack. */
zlStatusE zl303xx_PtpC37p238v1ClockDefaults (void *pClockInitS);
zlStatusE zl303xx_PtpC37p238v2ClockDefaults (void *pClockInitS);

zlStatusE zl303xx_PtpC37p238v1PortDefaults  (void *pPortInitS);
zlStatusE zl303xx_PtpC37p238v2PortDefaults  (void *pPortInitS);

zlStatusE zl303xx_PtpC37p238v1StreamDefaults(void *pStreamInitS);
zlStatusE zl303xx_PtpC37p238v2StreamDefaults(void *pStreamInitS);

/* Routines to execute any Post-PTP Object creation actions for the profile.
 * For example, BIND a custom TLV handler to the clock, etc. */
/* Currently nothing defined for the Port or Stream. */
zlStatusE zl303xx_PtpC37p238ClockCreateActions(void *pClockS);

/* Routine to check received Announce messages for valid TLVs, etc. */
zlStatusE zl303xx_PtpC37p238AnncRxCheck(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf);

/* Routine to add necessary TLVs to transmitted Announce messages. */
Uint16T zl303xx_PtpC37p238TlvAppend(
      zl303xx_ClockStreamS *pPtpStream,
      Uint32T messageType,
      Uint8T *pOutMsgBuf,
      Uint8T *pOutTlvBuf);
/* Routine to process the profile specific TLV. */
Uint16T zl303xx_PtpC37p238TlvHandle(
      Uint32T streamHandle,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf,
      Uint8T *pOutTlvBuf);

/* Routines to print profile specific debug information for each of the PTP objects. */
zlStatusE zl303xx_PtpC37p238ClockDebugShow(void *pClockS);
zlStatusE zl303xx_PtpC37p238PortDebugShow(void *pPortS);
zlStatusE zl303xx_PtpC37p238StreamDebugShow(void *pStreamS);


/******************************************************************************/
/*******   USER API FUNCTION DEFINITIONS   ************************************/
/******************************************************************************/

/* Updates a clock's profile specific clockId parameter.  */
zlStatusE zl303xx_PtpC37p238ClockPerformanceGet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpC37p238ClockPerformanceS *clockPerfDataS);

/* Retrieves a clock's profile specific configured parameters.  */
zlStatusE zl303xx_PtpC37p238ClockConfigGet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpC37p238ClockConfigS *clockProfileCfgS);

/* Updates a clock's profile specific parameters.  */
zlStatusE zl303xx_PtpC37p238ClockIdSet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint16T C37p238clockId);
zlStatusE zl303xx_PtpC37p238LocalTimeInaccSet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint32T gmTimeInaccuracy);
zlStatusE zl303xx_PtpC37p238NetworkTimeInaccSet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint32T networkTimeInaccuracy);

/* Sets a clock's profile specific LOCAL TIME ZONE information.  */
zlStatusE zl303xx_PtpC37p238LocalTimeZoneSet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpC37p238LocalTimeZoneS *localTimeZoneInfoS);

/* Updates a stream's profile specific parameters.  */
zlStatusE zl303xx_PtpC37p238StreamLocalTimeInaccMaxSet(
      zl303xx_PtpStreamHandleT streamHandle,
      Uint32T localTimeInaccuracyMax);
zlStatusE zl303xx_PtpC37p238StreamNetworkTimeInaccMaxSet(
      zl303xx_PtpStreamHandleT streamHandle,
      Uint32T networkTimeInaccuracyMax);

#endif







