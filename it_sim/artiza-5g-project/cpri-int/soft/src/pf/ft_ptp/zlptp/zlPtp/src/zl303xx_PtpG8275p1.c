

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This module contains the functions required to support the G.8275.1
*     standard (Telecom Profile for Phase).
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"
#include "zl303xx_Error.h"
#include "zl303xx_Ptsf.h"

#include "zl303xx_PtpG8275p1Api.h"
#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpFunctionExport.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_Ptp_dep.h"
#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpProfileDef.h"
#include "zl303xx_PtpV2TlvApi.h"
#include "zl303xx_PtpV2MsgParse.h"

/*****************   DEFINES     **********************************************/

/* Macro for checking Profile ID. */
#define ZL303XX_CHECK_G8275_PROFILE(fnName, profileId) \
   ( ((profileId) >= ZL303XX_PTP_PROFILE_NUM_TYPES)    \
         ?  (ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "%s: Invalid profileId (%u)(expected %u)", \
                           fnName, profileId, ZL303XX_PTP_PROFILE_TELECOM_G8275_1, 0,0,0), \
           ZL303XX_INVALID_OPERATION) \
         : ZL303XX_OK )

#define ZL303XX_PTP_G8275_LOCAL_PRI_CHECK(lPriority) \
           (((lPriority) < ZL303XX_PTP_G8275_LOCAL_PRI_MIN) \
                        ? ZL303XX_PARAMETER_INVALID : ZL303XX_OK)

/* Port masterOnly is a Boolean but Stream::mode is zl303xx_PtpStreamModeE
 * type. Use these MACROs to convert back-and-forth for masterOnly stream
 * within the context of this profile.   */
#define ZL303XX_PTP_G8275_STREAM_MODE_AS_MASTER_ONLY(streamMode) \
           (((streamMode) == ZL303XX_PTP_STREAM_MASTER_ONLY) \
                        ? ZL303XX_TRUE : ZL303XX_FALSE)
#define ZL303XX_PTP_G8275_MASTER_ONLY_AS_STREAM_MODE(bMasterOnly) \
           (((bMasterOnly) == ZL303XX_TRUE \
                 ? (ZL303XX_PTP_STREAM_MASTER_ONLY) \
                 : (ZL303XX_PTP_STREAM_USE_BMC)))


/* Profile Specific TLV Extension definitions. (Currently only 1 defined). */
#define ZL303XX_PTP_G8275_TLV_TS_IF_RATE             (Uint8T)(0x02)

/* Size of the TIMESTAMP INTERFACE RATE TLV. This is the required value in the
 * lengthField of the TLV (starts with the organizationId field). */
#define ZL303XX_PTP_G8275_TLV_TS_IF_RATE_TTL_LEN     18
#define ZL303XX_PTP_G8275_TLV_TS_IF_RATE_DATA_LEN    12

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/* Routine to gather ANNOUNCE Header fields. */
zlStatusE zl303xx_PtpG8275AnncHeaderGet(zl303xx_ClockStreamS *pPtpStream, zl303xx_PtpV2MsgHeaderS *pOutPtpMsgHeader);

/* Routines that initialize common parameter values for PTP Objects across
 * multiple profile versions / editions. */
zlStatusE zl303xx_PtpG8275CommonClockDefaults (zl303xx_PtpClockCreateS  *pClockCreateS);
zlStatusE zl303xx_PtpG8275CommonPortDefaults  (zl303xx_PtpPortCreateS   *pPortCreateS);
zlStatusE zl303xx_PtpG8275CommonStreamDefaults(zl303xx_PtpStreamCreateS *pStreamCreateS);

/* Gets the data pointer for the specified PTP object and verifies that it is
 * operating with the required G.8275 Profile. */
zlStatusE zl303xx_PtpClockDataGetG8275 (zl303xx_PtpClockHandleT clockHandle,   zl303xx_PtpClockS **ppClock);
zlStatusE zl303xx_PtpPortDataGetG8275  (zl303xx_PtpPortHandleT portHandle,     zl303xx_PortDataS **ppPort);
zlStatusE zl303xx_PtpStreamDataGetG8275(zl303xx_PtpStreamHandleT streamHandle, zl303xx_ClockStreamS **ppStream);

/* Local routine to check Message Intervals. */
zlStatusE zl303xx_PtpG8275MsgIntervalCheck(
      zl303xx_PtpProfileE profileId,
      Uint32T messageType,
      Sint8T *msgInterval,
      zl303xx_BooleanE returnErr,
      Sint8T *intervalMin,
      Sint8T *intervalMax,
      Sint8T *intervalDef);

/* Routine to evaluate the Clock-State of this Clock. */
zlStatusE zl303xx_PtpG8275p1ClockStateEvaluate(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PortDataS *pPtpVirtualPort,
      zl303xx_PtpPllPerformanceDataS *pPllPerfData);

/* Routines to evaluate the Clock-Class of this Clock. */
zlStatusE zl303xx_PtpG8275p1ClockClassEvaluate(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PortDataS *pPtpVirtualPort,
      zl303xx_PtpPllPerformanceDataS *pPllPerfData);
void zl303xx_PtpG8275p1ClockClassEvaluate_Method1(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PortDataS *pPtpVirtualPort,
      zl303xx_PtpPllPerformanceDataS *pllPerfData,
      Uint8T *pOutClockClass);
void zl303xx_PtpG8275p1ClockClassEvaluate_Method2(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PortDataS *pPtpVirtualPort,
      zl303xx_PtpPllPerformanceDataS *pllPerfData,
      Uint8T *pOutClockClass);
void zl303xx_PtpG8275p1ClockClassEvaluate_Method3(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PortDataS *pPtpVirtualPort,
      zl303xx_PtpPllPerformanceDataS *pllPerfData,
      Uint8T *pOutClockClass);

/* Routine to evaluate the Egress Frequency Traceable flag of this Clock. */
zlStatusE zl303xx_PtpG8275p1FreqTraceableEvaluate(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpPllPerformanceDataS *pPllPerfData);

/* Evaluate if a VP qualifies as a GM. */
zl303xx_BooleanE zl303xx_PtpG8275p1VirtualPortIsGM(
      zl303xx_PortDataS *pPtpVirtualPort,
      zl303xx_PtpPllPerformanceDataS *pllPerfData);

void zl303xx_PtpG8275p1ClockClassEvalGM_Method1(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpPllPerformanceDataS *pllPerfData,
      Uint8T *pOutClockClass);
void zl303xx_PtpG8275p1ClockClassEvalBC_Method1(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpPllPerformanceDataS *pllPerfData,
      Uint8T *pOutClockClass);

/* Local routines to evaluate the PHASE & FREQUENCY LOCK State of an
 * electrical reference. Used to determine Clock-State. */
zl303xx_PtpG8275p1ClockStateE zl303xx_PtpG8275p1ClockStateElecPhase(
      zl303xx_PtpPllPerformanceDataS *pPllPerfData);
zl303xx_PtpG8275p1ClockStateE zl303xx_PtpG8275p1ClockStateElecFreq(
      zl303xx_PtpPllPerformanceDataS *pPllPerfData);

/* Local routine to evaluate the PHASE LOCK State of a packet reference.
 * Used to determine Clock-State in Hybrid and pure-Packet modes. */
zl303xx_PtpG8275p1ClockStateE zl303xx_PtpG8275p1ClockStatePktPhase(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpPllPerformanceDataS *pPllPerfData);

/* Local routine to update the Clock State's description. */
void zl303xx_PtpG8275p1ClockStateDescrSet(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpG8275p1ClockStateE g8275p1State);

/* Local routines to Unpack/Pack Profile specific TLVs. */
Uint16T zl303xx_PtpG8275TimestampIfRateTlvUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_PtpG8275TimestampIfRateTlvS *pOutTlvDataS);
Uint16T zl303xx_PtpG8275TimestampIfRateTlvPack(
      zl303xx_PtpG8275TimestampIfRateTlvS *pInTlvDataS,
      Uint8T *pOutTlvBuf);

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/* PROFILE OUI. */
Uint8T Zl303xx_PtpG8275_OUI[] = {0x00, 0x19, 0xA7};

const zl303xx_PtpProfileS Zl303xx_PtpG8275p1v1ProfileInfo = {
   "ITU-T PTP profile for phase/time distribution with full timing support from the network",
   "Version 1.0",
   {0x00, 0x19, 0xA7, 0x01, 0x01, 0x00},
   "This profile is specified by the ITU-T",
   "A copy may be obtained from www.itu.int"
};

const zl303xx_PtpProfileS Zl303xx_PtpG8275p1v2ProfileInfo = {
   "ITU-T PTP profile for phase/time distribution with full timing support from the network",
   "Version 2.0",
   {0x00, 0x19, 0xA7, 0x01, 0x02, 0x00},
   "This profile is specified by the ITU-T",
   "A copy may be obtained from www.itu.int"
};

const zl303xx_PtpProfileS Zl303xx_PtpG8275p2ProfileInfo = {
   "ITU-T PTP profile for time distribution with partial timing support from the network (unicast mode)",
   "Version 1.0",
   {0x00, 0x19, 0xA7, 0x02, 0x01, 0x00},
   "This profile is specified by the ITU-T",
   "A copy may be obtained from www.itu.int"
};

Uint8T Zl303xx_PtpG8275TsIfRateTlvSubType[] = {0x00, 0x00, 0x02};

/* Strings for the zl303xx_PtpG8275p1ClockStateE (see the
 *                      zl303xx_PtpG8275p1ClockStateToStr() routine. */
static const char *zl303xx_PtpG8275p1ClockStateStr[] = {
    "FREERUN",             /* ZL303XX_PTP_G8275p1_CLOCK_STATE_FREERUN           */
    "ACQUIRING",           /* ZL303XX_PTP_G8275p1_CLOCK_STATE_ACQUIRING         */
    "HOLDOVER (OUT SPEC)", /* ZL303XX_PTP_G8275p1_CLOCK_STATE_HOLDOVER_OUT_SPEC */
    "HOLDOVER (IN SPEC)",  /* ZL303XX_PTP_G8275p1_CLOCK_STATE_HOLDOVER_IN_SPEC  */
    "LOCKED",              /* ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED            */
    "UNKNOWN"              /* ZL303XX_PTP_G8275p1_CLOCK_STATE_UNKNOWN.          */
};

/* Strings for the zl303xx_PtpG8275p1ClockClassDescE (see the
 *                      zl303xx_PtpG8275p1ClockClassDescToStr() routine.  */
static const char *zl303xx_PtpG8275p1ClockClassDescStr[] = {
    "FREERUN",       /* ZL303XX_PTP_G8275p1_CLOCK_CLASS_DESC_FREERUN      */
    "HOLDOVER",      /* ZL303XX_PTP_G8275p1_CLOCK_CLASS_DESC_HOLDOVER     */
    "LOCKED",        /* ZL303XX_PTP_G8275p1_CLOCK_CLASS_DESC_LOCKED       */
    "UNKNOWN"        /* ZL303XX_PTP_G8275p1_CLOCK_CLASS_DESC_UNKNOWN      */
};

/* Strings for the zl303xx_PtpG8275p1ClassEvalMethodE (see the
 *                      zl303xx_PtpG8275p1ClassEvalMethodToStr() routine. */
static const char *zl303xx_PtpG8275p1ClockClassEvalMethodStr[] = {
    "G.8275.1: Table 6.2",    /* ZL303XX_PTP_G8275p1_CLASS_EVAL_DEFAULT   */
    "G.8275.1: Table 6.4",    /* ZL303XX_PTP_G8275p1_CLASS_EVAL_METHOD2   */
    "China Mobile",           /* ZL303XX_PTP_G8275p1_CLASS_EVAL_METHOD3   */
    "UNKNOWN"                 /* UNKNOWN.                               */
};

/* There are 3 CLASS evaluation methods and each uses different values for some scenarios. */
Uint8T CLASS_GM_OUT_HLD_CAT1[3]  = { ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT1, ZL303XX_PTP_G8275p1_GM_CLASS_IN_HOLDOVER, ZL303XX_PTP_G8275p1_GM_CLASS_IN_HOLDOVER };
Uint8T CLASS_GM_OUT_HLD_CAT2[3]  = { ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT2, ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_C2_ALT, ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_C2_ALT };
Uint8T CLASS_GM_OUT_HLD_CAT3[3]  = { ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT3, ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_C3_ALT, ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_C3_ALT };
Uint8T CLASS_BC_OUT_HLD[3]       = { ZL303XX_PTP_G8275p1_BC_CLASS_OUT_HOLDOVER, ZL303XX_PTP_G8275p1_BC_CLASS_OUT_HOLDOVER_ALT, ZL303XX_PTP_G8275p1_BC_CLASS_OUT_HOLDOVER_ALT };

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/**
    zl303xx_PtpG8275p1v1ProfileInfoGet

  Details:
   Retrieves the profile description information for the G.8275.1 (edition 1)
   profile.

  Parameters:
   [in]  profileInfo    Pointer to the structure to store the profile information.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

 *****************************************************************************/
zlStatusE zl303xx_PtpG8275p1v1ProfileInfoGet(const zl303xx_PtpProfileS **profileInfo)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(profileInfo);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      *profileInfo = &Zl303xx_PtpG8275p1v1ProfileInfo;
   }

   return status;
}

/**
    zl303xx_PtpG8275p1v2ProfileInfoGet

  Details:
   Retrieves the profile description information for the G.8275.1 (edition 2)
   profile.

  Parameters:
   [in]  profileInfo    Pointer to the structure to store the profile information.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

 *****************************************************************************/
zlStatusE zl303xx_PtpG8275p1v2ProfileInfoGet(const zl303xx_PtpProfileS **profileInfo)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(profileInfo);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      *profileInfo = &Zl303xx_PtpG8275p1v2ProfileInfo;
   }

   return status;
}

/**
    zl303xx_PtpG8275p2ProfileInfoGet

  Details:
   Retrieves the profile description information for the G.8275.2 profile.

  Parameters:
   [in]  profileInfo    Pointer to the structure to store the profile information.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

 *****************************************************************************/
zlStatusE zl303xx_PtpG8275p2ProfileInfoGet(const zl303xx_PtpProfileS **profileInfo)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(profileInfo);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      *profileInfo = &Zl303xx_PtpG8275p2ProfileInfo;
   }

   return status;
}

/*

  Function Name:
    zl303xx_PtpG8275p1ClockDefaults

  Details:
   Routine to set the G.8275.1 (TelecomPhase Profile) specific clock parameters
   to their default values.

  Parameters:
   [in]  pClockInitS    Pointer to the PTP creation data structure for the
                              clock (from the PtpApiTypes.h module). This is
                              a void type so that all PTP Object types can use
                              the same interface.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275p1ClockDefaults(void *pClockInitS)
{
   /* Input pointer is verified in the COMMON routine. */
   zl303xx_PtpClockCreateS *pPtpClockCreateS = (zl303xx_PtpClockCreateS *)pClockInitS;
   zlStatusE status = zl303xx_PtpG8275CommonClockDefaults(pPtpClockCreateS);

   /* Set G.8275.1 specific values. */
   if (status == ZL303XX_OK)
   {
      /* domainNumber */
      pPtpClockCreateS->defaultDS.domainNumber = ZL303XX_PTP_G8275p1_DOMAIN_NUM_DEF;
   }

   return status;
}  /* END zl303xx_PtpG8275p1ClockDefaults */

/*

  Function Name:
    zl303xx_PtpG8275p1PortDefaults

  Details:
   Routine to set the G.8275.1 (TelecomPhase Profile) specific port parameters
   to their default values.

  Parameters:
   [in]  pPortInitS     Pointer to the PTP creation data structure for the
                              port (from the PtpApiTypes.h module). This is
                              a void type so that all PTP Object types can use
                              the same interface.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   profileInfo is NULL.
     ZL303XX_INVALID_MODE      The associated clock is not running the
                                    G.8275 PTP profile.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275p1PortDefaults(void *pPortInitS)
{
   /* Input pointer is verified in the COMMON routine. */
   zl303xx_PtpPortCreateS *pPortCreateS = (zl303xx_PtpPortCreateS *)pPortInitS;
   zlStatusE status = zl303xx_PtpG8275CommonPortDefaults(pPortCreateS);

   /* Set G.8275.1 specific values. */
   if (status == ZL303XX_OK)
   {
      /* None at the moment */
   }

   return status;
}  /* END zl303xx_PtpG8275p1PortDefaults */

/*

  Function Name:
    zl303xx_PtpG8275p1StreamDefaults

  Details:
   Routine to set the G.8275.1 (TelecomPhase Profile) specific stream parameters
   to their default values.

  Parameters:
   [in]  pStreamInitS   Pointer to the PTP creation data structure for the
                              stream (from the PtpApiTypes.h module). This is
                              a void type so that all PTP Object types can use
                              the same interface.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275p1StreamDefaults(void *pStreamInitS)
{
   /* Input pointer is verified in the COMMON routine. */
   zl303xx_PtpStreamCreateS *pStreamCreateS = (zl303xx_PtpStreamCreateS *)pStreamInitS;
   zlStatusE status = zl303xx_PtpG8275CommonStreamDefaults(pStreamCreateS);

   /* Set G.8275.1 specific values. */
   if (status == ZL303XX_OK)
   {
      pStreamCreateS->logAnnounceInterval    = ZL303XX_PTP_G8275p1_LOG_ANNC_DEF;
      pStreamCreateS->logSyncInterval        = ZL303XX_PTP_G8275p1_LOG_SYNC_DEF;
      pStreamCreateS->logMinDelayReqInterval = ZL303XX_PTP_G8275p1_LOG_DELAY_DEF;
      pStreamCreateS->announceReceiptTimeout = ZL303XX_PTP_G8275p1_ANNC_TIMEOUT_DEF;
   }

   return status;
}  /* END zl303xx_PtpG8275p1StreamDefaults */

/*

  Function Name:
    zl303xx_PtpG8275p2ClockDefaults

  Details:
   Routine to set the G.8275.2 (TelecomPhase Profile) specific clock parameters
   to their default values.

  Parameters:
   [in]  pClockInitS    Pointer to the PTP creation data structure for the
                              clock (from the PtpApiTypes.h module). This is
                              a void type so that all PTP Object types can use
                              the same interface.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275p2ClockDefaults(void *pClockInitS)
{
   /* Input pointer is verified in the COMMON routine. */
   zl303xx_PtpClockCreateS *pPtpClockCreateS = (zl303xx_PtpClockCreateS *)pClockInitS;
   zlStatusE status = zl303xx_PtpG8275CommonClockDefaults(pPtpClockCreateS);

   /* Set G.8275.2 specific values. */
   if (status == ZL303XX_OK)
   {
      /* domainNumber */
      pPtpClockCreateS->defaultDS.domainNumber = ZL303XX_PTP_G8275p2_DOMAIN_NUM_DEF;
   }

   return status;
}  /* END zl303xx_PtpG8275p2ClockDefaults */

/*

  Function Name:
    zl303xx_PtpG8275p2PortDefaults

  Details:
   Routine to set the G.8275.2 (TelecomPhase Profile) specific port parameters
   to their default values.

  Parameters:
   [in]  pPortInitS     Pointer to the PTP creation data structure for the
                              port (from the PtpApiTypes.h module). This is
                              a void type so that all PTP Object types can use
                              the same interface.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   profileInfo is NULL.
     ZL303XX_INVALID_MODE      The associated clock is not running the
                                    G.8275 PTP profile.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275p2PortDefaults(void *pPortInitS)
{
   /* Input pointer is verified in the COMMON routine. */
   zl303xx_PtpPortCreateS *pPortCreateS = (zl303xx_PtpPortCreateS *)pPortInitS;
   zlStatusE status = zl303xx_PtpG8275CommonPortDefaults(pPortCreateS);

   /* Set G.8275.2 specific values. */
   if (status == ZL303XX_OK)
   {
      /* None at the moment */
   }

   return status;
}  /* END zl303xx_PtpG8275p2PortDefaults */

/*

  Function Name:
    zl303xx_PtpG8275p2StreamDefaults

  Details:
   Routine to set the G.8275.2 (TelecomPhase Profile) specific stream parameters
   to their default values.

  Parameters:
   [in]  pStreamInitS   Pointer to the PTP creation data structure for the
                              stream (from the PtpApiTypes.h module). This is
                              a void type so that all PTP Object types can use
                              the same interface.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275p2StreamDefaults(void *pStreamInitS)
{
   /* Input pointer is verified in the COMMON routine. */
   zl303xx_PtpStreamCreateS *pStreamCreateS = (zl303xx_PtpStreamCreateS *)pStreamInitS;
   zlStatusE status = zl303xx_PtpG8275CommonStreamDefaults(pStreamCreateS);

   /* Set G.8275.2 specific values. */
   if (status == ZL303XX_OK)
   {
      /* Packet Rates */
      pStreamCreateS->logAnnounceInterval    = ZL303XX_PTP_G8275p2_LOG_ANNC_DEF;
      pStreamCreateS->logSyncInterval        = ZL303XX_PTP_G8275p2_LOG_SYNC_DEF;
      pStreamCreateS->logMinDelayReqInterval = ZL303XX_PTP_G8275p2_LOG_DELAY_DEF;
      pStreamCreateS->announceReceiptTimeout = ZL303XX_PTP_G8275p2_ANNC_TIMEOUT_DEF;

      /* Contract Retry */
      pStreamCreateS->unicastNegotiation.retry.denial.logQueryCount
                                          = ZL303XX_PTP_TELECOM_UN_RETRIES;
      pStreamCreateS->unicastNegotiation.retry.denial.lockoutSec
                                          = ZL303XX_PTP_TELECOM_UN_LOCKOUT_SEC;
   }

   return status;
}  /* END zl303xx_PtpG8275p2StreamDefaults */

/*

  Function Name:
    zl303xx_PtpG8275PortPreCreate

  Details:
   Routine to do G.8275 (TelecomPhase Profile) specific actions and checks
   prior to the PTP Port being created. This may include final range checking
   and/or ensuring consistency amongst members (especially deprecated ones).

  Parameters:
   [in]  pPortInitS     Pointer to the PTP creation data structure for the
                              port (this is a void type so that all PTP Object
                              types can use the same interface.
   [in]  pPortParentS   Pointer to an existing PTP Clock Object to which the
                              newly created Port will belong.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   pPortCreateS or pPortParentS are NULL.
     ZL303XX_INVALID_MODE      The associated clock is not running the
                                    G.8275 PTP profile.

  Notes:    Since this call is made by way of the ProfileDef binding, it is assumed
            that input Pointers have already been checked.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275PortPreCreate(
      void *pPortInitS,
      void *pPortParentS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_OK;

   /* Cast the inputs to their proper types. */
   zl303xx_PtpPortCreateS *pPortCreateS = (zl303xx_PtpPortCreateS *)pPortInitS;
   zl303xx_PtpClockS *pPtpClock         = (zl303xx_PtpClockS *)pPortParentS;

   /* Backwards Compatibility checks. */
   if (status == ZL303XX_OK)
   {
      /* Port::Profile::notSlave */
      /* If the old application code set a non-Default value in this deprecated
       * member, then use it. Otherwise, just keep the new member's value. */
      if (pPortCreateS->profileCfg.g8275p1.notSlave == ZL303XX_TRUE)
      {
         pPortCreateS->masterOnly = pPortCreateS->profileCfg.g8275p1.notSlave;
      }
      else
      {
         pPortCreateS->profileCfg.g8275p1.notSlave = pPortCreateS->masterOnly;
      }

      /* Port::Unicast::grantDefault */
      /* Again, use the old application value in this deprecated member if it is
       * not the default. Otherwise, keep the new member's value. */
      if (pPortCreateS->unicastNegotiation.grantDefault == ZL303XX_FALSE)
      {
         pPortCreateS->unicastNegotiation.denyServiceRequests = ZL303XX_TRUE;
      }
      else
      {
         pPortCreateS->unicastNegotiation.grantDefault =
                        !(pPortCreateS->unicastNegotiation.denyServiceRequests);
      }
   }

   /* Re-check some of the Profile specific values. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpG8275GrantDenyCheck(pPtpClock,
                        &pPortCreateS->unicastNegotiation.denyServiceRequests);
   }


   return ZL303XX_OK;
}  /* END zl303xx_PtpG8275PortPreCreate */

/*

  Function Name:
    zl303xx_PtpG8275ClockCreateActions

  Details:
   Routine to perform G.8275 (Telecom Profile - Phase) actions related to clock
   creation. This routine is BIND'ed (bound) to the ProfileDef list and gets
   called at the end of the core Clock Creation process.
   (See zl303xx_PtpClockCreate()).

   For this profile, the Clock is configured to used Stacked TLVs on initial
   contract request.

  Parameters:
   [in]  pClockS  Pointer to the PTP clock data structure (as a void).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pClockS is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275ClockCreateActions(void *pClockS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pClockS);

   /* De-reference the clock pointer (if NULL, we skip using it). */
   zl303xx_PtpClockS *pPtpClockS = (zl303xx_PtpClockS *)pClockS;

   /* Set the Clock to used Stacked TLVs on initial unicast contract request. */
   if (status == ZL303XX_OK)
   {
      /* For G.8275, use stack TLV on initial contract request regardless of the
       * stacked TLV setting. This is not a CONFIG parameter so cannot be put
       * into the clock defaults routine. */
      pPtpClockS->stackAllServicesOn1stRequest = ZL303XX_TRUE;

      /* For G.8275.1, set the clockState parameter to FREERUN. */
      pPtpClockS->profileDS.g8275.clockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_FREERUN;
      pPtpClockS->profileDS.g8275.clockClassDesc = ZL303XX_PTP_G8275p1_CLOCK_CLASS_DESC_FREERUN;

      /* For G.8275.1, set the highest Class that this clock has achieved. */
      pPtpClockS->profileDS.g8275.fallbackClass = pPtpClockS->config.defaultDS.clockQuality.clockClass;

      /* For G.8275.1, set the highest Class that this clock has achieved. */
      pPtpClockS->profileDS.g8275.prtcHoldover = ZL303XX_FALSE;

      /* Keep the equipmentClass for METHOD2 in the dynamic array. */
      CLASS_GM_OUT_HLD_CAT2[ZL303XX_PTP_G8275p1_CLASS_EVAL_METHOD2] = pPtpClockS->config.profileCfg.g8275p1.equipmentClass;
   }

   return status;
}  /* END zl303xx_PtpG8275ClockCreateActions */

/* zl303xx_PtpG8275ClockDataUpdate */
/** 

   Only applicable to Telecom Profile for Phase. Evaluates and updates any
   profile specific data.

  Parameters:
   [in]   pClock   Pointer to a clock.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275ClockDataUpdate(void *pClock)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pClock);

   /* De-reference the clock pointer. */
   zl303xx_PtpClockS *pPtpClock = (zl303xx_PtpClockS *)pClock;

   /* Pointer to a Virtual Port (may not be selected. */
   zl303xx_PortDataS *pPtpVirtualPort = NULL;

   /* Structure for the PLL Performance Data. */
   zl303xx_PtpPllPerformanceDataS pllPerfData;

   /* Check the Profile */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_G8275_PROFILE("zl303xx_PtpG8275ClockDataUpdate",
                                          pPtpClock->config.profile);
   }

   /* Get the PLL performance & state data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockPerformanceGet(pPtpClock, &pllPerfData);
   }


   /* Get the pointer to the Virtual PTP Port (if selected:: NULL otherwise). */
   if (status == ZL303XX_OK)
   {
      (void)zl303xx_PtpClockHasSelectedVirtualPort(pPtpClock, &pPtpVirtualPort);
      if (pPtpVirtualPort != NULL)
      {
         /* If a Virtual Port has been selected AND the VP is configured as a GM
          * (PRTC or ePRTC) then remember this if we go to holdover later. */
         pPtpClock->profileDS.g8275.prtcHoldover =
               zl303xx_PtpG8275p1VirtualPortIsGM(pPtpVirtualPort, &pllPerfData);
      }
      else if (pPtpClock->gmSource.location != ZL303XX_PTP_GM_SRC_LOCAL_CLOCK)
      {
         /* If a Packet Port has been selected then do not use PRTC if we go to
          * holdover later. */
         pPtpClock->profileDS.g8275.prtcHoldover = ZL303XX_FALSE;
      }
      /* ELSE: if local clock (D0) then Holdover is activated; use the saved
       *       prtcHoldover flag whild in holdover.  */
   }

   /* Evaluate the Clock State. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpG8275p1ClockStateEvaluate(pPtpClock, pPtpVirtualPort, &pllPerfData);
   }

   /* Update the advertised Class and other flags. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpG8275p1ClockClassEvaluate(pPtpClock, pPtpVirtualPort, &pllPerfData);
   }

   /* Update the frequencyTraceable flag. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpG8275p1FreqTraceableEvaluate(pPtpClock, &pllPerfData);
   }

   /* Check TS flag */
   /* If the Time has not been set, then ignore the stuff we did above. */
   if (status == ZL303XX_OK)
   {
      /* If the clock is bypassing the G.8275.1 hardware interface, then set the
       * timeSet flag to the timePropertiesDS::timeTraceable value.  */
      if (pPtpClock->config.profileCfg.g8275p1.bypassEnabled == ZL303XX_TRUE)
      {
         pPtpClock->profileDS.g8275.todSet = pPtpClock->timePropertiesDS.timeTraceable;
      }

      /* If a Virtual port is selected, use its configuration. */
      else if (pPtpVirtualPort != NULL)
      {
         pPtpClock->profileDS.g8275.todSet = pPtpVirtualPort->virtualData.config.timeSetFlag;
      }

      /* Otherwise, check if the ToD has been set via the HW interface. */
      else
      {
         status = zl303xx_PtpClockTimeSetStatusGet(pPtpClock, &pPtpClock->profileDS.g8275.todSet);
      }
   }

   if (status == ZL303XX_OK)
   {
      /* If the time has not been set then advertise Freerun. */
      if (pPtpClock->profileDS.g8275.todSet == ZL303XX_FALSE)
      {
         pPtpClock->profileDS.g8275.pllClockClass = ZL303XX_PTP_G8275p1_CLASS_FREERUN;
      }
   }
   return ZL303XX_OK;
}  /* END zl303xx_PtpG8275ClockDataUpdate */

/**
    zl303xx_PtpG8275TransportSpecificCheck

  Details:
   Checks if the transportSpecific value provided is within the range of the
   profile.

  Parameters:
   [in]  transportSpecific Pointer to the transportSpecific value to check.
                                 (Uses a pointer to allow profiles to 'reset'
                                 the value if out of range and
                                 strictFlag == FALSE).
   [in]  strictFlag        Flag indicating whether to over-write erroneous
                                 values with the default value or return the
                                 ZL303XX_PARAMETER_INVALID error.
                                 Unused for this profile.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      transportSpecific is NULL.
     ZL303XX_PARAMETER_INVALID    transportSpecific is out of range

 *****************************************************************************/
zlStatusE zl303xx_PtpG8275TransportSpecificCheck(
      Uint8T *transportSpecific,
      zl303xx_BooleanE strictFlag)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(transportSpecific);

   /* Warning removal */
   if (strictFlag) {;}

   /* Check the data. */
   if (status == ZL303XX_OK)
   {
      /* Check that the transportSpecific nibble has bit-0 set to 0 and ALWAYS
       * return ERROR in G.8275 regardless of the strictFlag setting. */
      if (*transportSpecific != 0)
      {
          status = ZL303XX_PARAMETER_INVALID;
      }
   }

   return status;
}  /* END zl303xx_PtpG8275TransportSpecificCheck */

/* zl303xx_PtpG8275p1v1ClockQualityCheck */
/** 

   Determines if the PTP clockQuality values are valid for the profile.

  Parameters:
   [in]  clockQuality   PTP clockQuality structure to check. (Values are
                              'reset' to defaults if incorrect and
                              strictFlag == FALSE).
   [in]  strictFlag     Flag indicating whether to over-write erroneous values
                              with the default value or return the
                              ZL303XX_PARAMETER_INVALID error.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      clockQuality is NULL.
     ZL303XX_PARAMETER_INVALID    clockQuality member is out of range and
                                       strictFlag == TRUE.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275p1v1ClockQualityCheck(
      zl303xx_ClockQuality *clockQuality,
      zl303xx_BooleanE strictFlag)
{
   zlStatusE status = ZL303XX_OK;

   /* The clockClass must be one of the allowed values when strictFlag is True. */
   if (status == ZL303XX_OK)
   {
      if ((clockQuality->clockClass != ZL303XX_PTP_G8275p1_GM_CLASS_LOCK) &&
          (clockQuality->clockClass != ZL303XX_PTP_G8275p1_GM_CLASS_IN_HOLDOVER) &&
          (clockQuality->clockClass != ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT1) &&
          (clockQuality->clockClass != ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT2) &&
          (clockQuality->clockClass != ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT3) &&
          (clockQuality->clockClass != ZL303XX_PTP_G8275p1_BC_CLASS_IN_HOLDOVER) &&
          (clockQuality->clockClass != ZL303XX_PTP_G8275p1_BC_CLASS_OUT_HOLDOVER) &&
          (clockQuality->clockClass != ZL303XX_PTP_G8275p1_CLASS_FREERUN) &&
          (clockQuality->clockClass != ZL303XX_PTP_G8275p1_CLASS_SLAVE) &&
          (clockQuality->clockClass != ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_C2_ALT) &&
          (clockQuality->clockClass != ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_C3_ALT) &&
          (clockQuality->clockClass != ZL303XX_PTP_G8275p1_BC_CLASS_OUT_HOLDOVER_ALT))
      {
         if (strictFlag == ZL303XX_TRUE)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                  "   (ERROR) G.8275.1 (edition 1) clockclass(%d) Non-Standard value.",
                  clockQuality->clockClass, 0,0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
         }
         else
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                  "   (WARNING) G.8275.1 (edition 1) clockclass(%d) Using Non-Standard value.",
                  clockQuality->clockClass, 0,0,0,0,0);
         }
      }
   }

   /* The clockAccuracy must be one of the allowed values when strictFlag is True. */
   if (status == ZL303XX_OK)
   {
      if ((clockQuality->clockAccuracy != ZL303XX_PTP_G8275p1_ACCURACY_LOCK_ePRTC) &&
          (clockQuality->clockAccuracy != ZL303XX_PTP_G8275p1_ACCURACY_LOCK) &&
          (clockQuality->clockAccuracy != ZL303XX_PTP_G8275p1_ACCURACY_UNLOCK))
      {
         if (strictFlag == ZL303XX_TRUE)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                  "   (ERROR) G.8275.1 (edition 1) clockAccuracy(0x%02X) Non-Standard value.",
                  clockQuality->clockAccuracy, 0,0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
         }
         else
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                  "   (WARNING) G.8275.1 (edition 1) clockAccuracy(0x%02X) Using Non-Standard value.",
                  clockQuality->clockAccuracy, 0,0,0,0,0);
         }
      }
   }

   /* The offsetScaledLogVariance must be one of the allowed values when strictFlag is True. */
   if (status == ZL303XX_OK)
   {
      if ((clockQuality->offsetScaledLogVariance != ZL303XX_PTP_G8275p1_VARIANCE_LOCK_ePRTC) &&
          (clockQuality->offsetScaledLogVariance != ZL303XX_PTP_G8275p1_VARIANCE_LOCK) &&
          (clockQuality->offsetScaledLogVariance != ZL303XX_PTP_G8275p1_VARIANCE_UNLOCK))
      {
         if (strictFlag == ZL303XX_TRUE)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                  "   (ERROR) G.8275.1 (edition 1) offsetScaledLogVariance(0x%04X) Non-Standard value.",
                  clockQuality->offsetScaledLogVariance, 0,0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
         }
         else
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                  "   (WARNING) G.8275.1 (edition 1) offsetScaledLogVariance(0x%04X) Using Non-Standard value.",
                  clockQuality->offsetScaledLogVariance, 0,0,0,0,0);
         }
      }
   }

   return status;
}  /* END zl303xx_PtpG8275p1v1ClockQualityCheck */

/**
    zl303xx_PtpG8275PriorityCheck

  Details:
   Checks if the priority1 and/or priority2 values provided is/are within the
   range of the profile.

  Parameters:
   [in]  priority1      Pointer to the Clock priority1 value being used.
   [in]  priority2      Pointer to the Clock priority2 value being used.
                              (Each uses a pointer to allow profiles to 'reset'
                              the value if out of range and strictFlag == FALSE).
   [in]  slaveOnly      Flag indicating if this is a slave-only clock
                              (limitations differ for GM, BC and Slave clocks).
   [in]  strictFlag     Flag indicating whether to over-write erroneous values
                              with the default value or return the
                              ZL303XX_PARAMETER_INVALID error.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    priority1 or priority2 value is out of
                                       range and strictFlag == TRUE.

 *****************************************************************************/
zlStatusE zl303xx_PtpG8275PriorityCheck(
      Uint8T *priority1,
      Uint8T *priority2,
      zl303xx_BooleanE slaveOnly,
      zl303xx_BooleanE strictFlag)
{
   /* Set default return value. */
   zlStatusE status = ZL303XX_OK;

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      /* Priority1 check */
      if (priority1 != NULL)
      {
         /* Priority1 must be 128 for Strict Profile enforcement. */
         if (*priority1 != ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF)
         {
            /* If the STRICT flag is enabled, apply the Profile Specific limits. */
            if (strictFlag == ZL303XX_FALSE)
            {
               ZL303XX_TRACE_ALWAYS(
                     "   (WARNING) G.8275 priority1=%u is invalid. Defaulting to %u.",
                     *priority1, ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF, 0,0,0,0);

               /* Set default, return no error. */
               *priority1 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;
            }
            else
            {
               ZL303XX_TRACE_ALWAYS(
                     "   (ERROR) G.8275 priority1(%u) is invalid. Only allowed(%u)",
                     *priority1, ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF, 0,0,0,0);

               status = ZL303XX_PARAMETER_INVALID;
            }
         }
      }

      /* Priority2 check:
       * - Slave-Only clocks must be 255.
       * - All others have a range of {0,255} so no check required on a Uint8T. */
      if ((priority2 != NULL) &&
          (slaveOnly == ZL303XX_TRUE))
      {
         /* For Slave-Only clocks, only 255 is permitted. */
         if (*priority2 != ZL303XX_PTP_G8275p1_CLOCK_PRI2_SLAVE)
         {
            /* If the STRICT flag is enabled, apply the Profile Specific limits. */
            if (strictFlag == ZL303XX_FALSE)
            {
               ZL303XX_TRACE_ALWAYS(
                     "   (WARNING) G.8275 priority2=%u is invalid for Slave-Only Clock. Defaulting to %u.",
                     *priority2, ZL303XX_PTP_G8275p1_CLOCK_PRI2_SLAVE, 0,0,0,0);

               /* Set default, return no error. */
               *priority2 = ZL303XX_PTP_G8275p1_CLOCK_PRI2_SLAVE;
            }
            else
            {
               ZL303XX_TRACE_ALWAYS(
                     "   (ERROR) G.8275 priority2(%u) is invalid for Slave-Only Clock. Only allowed(%u)",
                     *priority2, ZL303XX_PTP_G8275p1_CLOCK_PRI2_SLAVE, 0,0,0,0);

               status = ZL303XX_PARAMETER_INVALID;
            }
         }
      }
   }

   return status;
}  /* END zl303xx_PtpG8275PriorityCheck */

/**
    zl303xx_PtpG8275GrantDenyCheck

  Details:
   Checks if the specified Port is allowed to GRANT Unicast contracts. (SLAVE
   Ports are not permitted but MASTER Ports must allow them).

  Parameters:
   [in]  pPtpClock      Pointer to an existing Profile Clock structure on
                              which the Port parameter is being set.
   [in]  bInGrantDeny   Boolean indicating the desired setting of the
                              GRANT-DENY setting of the Port.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      bGrantDeny is NULL.
     ZL303XX_PARAMETER_INVALID    bGrantDeny is out of range and strictFlag == TRUE.

 *****************************************************************************/
zlStatusE zl303xx_PtpG8275GrantDenyCheck(
      zl303xx_PtpClockS *pPtpClock,
      void *bInGrantDeny)
{
   /* Check input parameters. */
   zl303xx_BooleanE *bGrantDeny = (zl303xx_BooleanE *)bInGrantDeny;
   zlStatusE status = ZL303XX_CHECK_POINTER(bGrantDeny);

   /* Check the bGrantDeny setting is valid for the clockType. */
   if (status == ZL303XX_OK)
   {
      /* SLAVE - bGrantDeny value can only be TRUE  */
      if ((pPtpClock->config.clockType == ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_TSC) &&
          (*bGrantDeny != ZL303XX_TRUE))
      {
         if (pPtpClock->config.profileStrict == ZL303XX_FALSE)
         {
            /* Allow the setting. */
            ZL303XX_TRACE_ALWAYS(
                  "   (WARNING) G.8275 Port on clockType (SLAVE) should set denyServiceRequests = TRUE",
                  0,0,0,0,0,0);
            status = ZL303XX_OK;
         }
         else
         {
            ZL303XX_TRACE_ALWAYS(
                  "   (ERROR) G.8275 Port on clockType (SLAVE) must set denyServiceRequests = TRUE",
                  0,0,0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
         }
      }

      /* GM - bGrantDeny value can only be FALSE */
      else if ((pPtpClock->config.clockType == ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_GM) &&
               (*bGrantDeny != ZL303XX_FALSE))
      {
         if (pPtpClock->config.profileStrict == ZL303XX_FALSE)
         {
            /* Allow the setting. */
            ZL303XX_TRACE_ALWAYS(
                  "   (WARNING) G.8275 Port on clockType (MASTER) should set denyServiceRequests = FALSE",
                  0,0,0,0,0,0);
            status = ZL303XX_OK;
         }
         else
         {
            ZL303XX_TRACE_ALWAYS(
                  "   (ERROR) G.8275 Port on clockType (MASTER) must set denyServiceRequests = FALSE",
                  0,0,0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
         }
      }
   }

   return status;
}  /* END zl303xx_PtpG8275GrantDenyCheck */

/*

  Function Name:
    zl303xx_PtpG8275AnncRxCheck

  Details:
   Checks that Announce fields contain values permitted by the profile.

  Parameters:
   [in]  pPtpStream     Pointer to the stream on which the message was received.
   [in]  pPtpMsgRx      Pointer the the received message structure (contains
                              all header and announce fields).
   [in]  pInTlvBuf      Pointer to any appended TLV buffer.

  Return Value:
     ZL303XX_OK                  Success.
     ZL303XX_PARAMETER_INVALID   Announce values are outside the allowed range.
     OTHER                     As specified by the Profile specific check.

  Notes:    If the associated clock's 'strictFlag' == FALSE, this check is ignored.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275AnncRxCheck(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf)
{
   /* Default return value. */
   zlStatusE status = ZL303XX_OK;

   /* Declare local parameters to keep the code manageable. */
   zl303xx_BooleanE strictFlag = pPtpStream->portData->clock->config.profileStrict;
   zl303xx_PtpProfileE profileId = pPtpStream->portData->clock->config.profile;

   /* Eliminate warnings for unused parameters. */
   if (pInTlvBuf) {;}

   /* If there is no STRICT checking, just return. */
   if (strictFlag == ZL303XX_TRUE)
   {
      /* messageType */
      if (pPtpMsgRx->header.messageType != ZL303XX_MSG_ID_ANNOUNCE)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpG8275AnncRxCheck: messageType != ANNOUNCE (Act:%u)",
               pPtpMsgRx->header.messageType, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }

      /* transportSpecific */
      else if (pPtpMsgRx->header.transportSpecific != 0)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpG8275AnncRxCheck: INVALID transportSpecific value (Act:%u)",
               pPtpMsgRx->header.transportSpecific, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }

      /* versionPTP */
      else if (pPtpMsgRx->header.versionPTP != ZL303XX_PTP_VERSION_2)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpG8275AnncRxCheck: INVALID versionPTP (Act:%u)",
               pPtpMsgRx->header.versionPTP, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* clockQuality */
   if (status == ZL303XX_OK)
   {
      if (profileId == ZL303XX_PTP_PROFILE_TELECOM_G8275_1v1)
      {
         status = zl303xx_PtpG8275p1v1ClockQualityCheck(
                              &pPtpMsgRx->data.announce.grandmasterClockQuality,
                              strictFlag);
      }

      /* For G.8275.1 (v2) and G.8275.2 any class, Accuracy or Variance is allowed. */
      else if ((profileId == ZL303XX_PTP_PROFILE_TELECOM_G8275_1v2) ||
               (profileId == ZL303XX_PTP_PROFILE_TELECOM_G8275_2))
      {
         status = ZL303XX_OK;
      }

      else
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpG8275AnncRxCheck: INVALID profile ID (Act:%u)",
               profileId, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* priority1 & priority2 */
   /* priority1 is unused and can be ignored (retransmit 128 no matter what).
    * priority2 full range of 0-255 is allowed so check required. */
   pPtpMsgRx->data.announce.grandmasterPriority1 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;

   /* domainNumber */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpProfileDomainNumberCheck(profileId, strictFlag,
                                          &pPtpMsgRx->header.domainNumber);
   }

   return status;
}  /* END zl303xx_PtpG8275AnncRxCheck */

/*

  Function Name:
    zl303xx_PtpG8275StreamModeCheck

  Details:
   Checks that the Stream::Mode parameter value is permitted by the profile.

  Parameters:
   [in]  pPtpStream     Pointer to the target stream structure.
   [in]  pInStreamMode	The new mode value intended to be set.

  Return Value:
     ZL303XX_OK                  Success.
     ZL303XX_PARAMETER_INVALID   pInStreamMode value is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275StreamModeCheck(
      zl303xx_ClockStreamS *pPtpStream,
      void *pInStreamMode)
{
   /* Default return value. */
   zlStatusE status = ZL303XX_OK;

   /* Cast the input type. */
   zl303xx_PtpStreamModeE overrideMode = *(zl303xx_PtpStreamModeE *)pInStreamMode;

   /* declare local parameters to keep the code manageable. */
   zl303xx_BooleanE strictFlag = pPtpStream->portData->clock->config.profileStrict;

   /* Check the overrideMode setting is valid for the Profile clockType. */
   if (status == ZL303XX_OK)
   {
      /* SLAVE Clock -
       * - STRICT: Streams should only be slaveOnly but allow BMC (since there
       *           is no equivalent SlaveOnly setting for the PORT, it is
       *           effectively BMC when not MASTER_ONLY). */
      if ((pPtpStream->portData->clock->config.clockType == ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_TSC) &&
          (overrideMode == ZL303XX_PTP_STREAM_MASTER_ONLY))
      {
         if (strictFlag == ZL303XX_TRUE)
         {
            ZL303XX_TRACE_ALWAYS(
                  "   (ERROR) G.8275.1 Stream on clockType (SLAVE) must set masterOnly = FALSE.",
                  0,0,0,0,0,0);

            status = ZL303XX_PARAMETER_INVALID;
         }
         else
         {
            ZL303XX_TRACE_ALWAYS(
                  "   (WARNING) G.8275.1 Stream on clockType (SLAVE) should set masterOnly = FALSE.",
                  0,0,0,0,0,0);
         }
      }

      /* GM - masterOnly value can only be TRUE */
      /* GRANDMASTER Clock -
       * - STRICT: Streams should only be masterOnly (but allow BMC). */
      else if ((pPtpStream->portData->clock->config.clockType == ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_GM) &&
               (overrideMode == ZL303XX_PTP_STREAM_SLAVE_ONLY))
      {
         if (strictFlag == ZL303XX_TRUE)
         {
            ZL303XX_TRACE_ALWAYS(
                  "   (ERROR) G.8275.1 Stream on clockType (GM) must set masterOnly = TRUE.",
                  0,0,0,0,0,0);

            status = ZL303XX_PARAMETER_INVALID;
         }
         else
         {
            ZL303XX_TRACE_ALWAYS(
                  "   (WARNING) G.8275.1 Stream on clockType (GM) should set masterOnly = TRUE.",
                  0,0,0,0,0,0);
         }
      }
   }

   return status;
}  /* END zl303xx_PtpG8275StreamModeCheck */

/*

  Function Name:
    zl303xx_PtpG8275PortMasterOnlyCheck

  Details:
   Checks that the Port::masterOnly parameter value is permitted by the profile.

  Parameters:
   [in]  pPtpPort       Pointer to the target port structure.
   [in]  bInMasterOnly  The new masterOnly value intended to be set.

  Return Value:
     ZL303XX_OK                  Success.
     ZL303XX_PARAMETER_INVALID   bInMasterOnly value is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275PortMasterOnlyCheck(
      zl303xx_PortDataS *pPtpPort,
      void *bInMasterOnly)
{
   /* Default return value. */
   zlStatusE status = ZL303XX_OK;

   /* Cast the input type. */
   zl303xx_BooleanE bMasterOnly = *(zl303xx_BooleanE *)bInMasterOnly;

   /* declare local parameters to keep the code manageable. */
   zl303xx_BooleanE strictFlag = pPtpPort->clock->config.profileStrict;

   /* Check the masterOnly setting is valid for the Profile clockType. */
   if (status == ZL303XX_OK)
   {
      /* SLAVE Clock -
       * - STRICT: Ports should never be masterOnly. */
      if ((pPtpPort->clock->config.clockType == ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_TSC) &&
          (bMasterOnly == ZL303XX_TRUE))
      {
         if (strictFlag == ZL303XX_TRUE)
         {
            ZL303XX_TRACE_ALWAYS(
                  "   (ERROR) G.8275.1 Port on clockType (SLAVE) must set masterOnly = FALSE.",
                  0,0,0,0,0,0);

            status = ZL303XX_PARAMETER_INVALID;
         }
         else
         {
            ZL303XX_TRACE_ALWAYS(
                  "   (WARNING) G.8275.1 Port on clockType (SLAVE) should set masterOnly = FALSE.",
                  0,0,0,0,0,0);
         }
      }

      /* GM - masterOnly value can only be TRUE */
      /* GRANDMASTER Clock -
       * - STRICT: Ports should only be masterOnly (but allow BMC). */
      else if ((pPtpPort->clock->config.clockType == ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_GM) &&
               (bMasterOnly == ZL303XX_FALSE))
      {
         if (strictFlag == ZL303XX_TRUE)
         {
            ZL303XX_TRACE_ALWAYS(
                  "   (ERROR) G.8275.1 Port on clockType (GM) must set masterOnly = TRUE.",
                  0,0,0,0,0,0);

            status = ZL303XX_PARAMETER_INVALID;
         }
      }
   }

   /* Check that no pre-existing Stream has a conflicting MODE setting */
   if ((status == ZL303XX_OK) &&
       (bMasterOnly == ZL303XX_TRUE))
   {
      zl303xx_ClockStreamS *pPtpStream;

      ZL303XX_PTP_FOR_EACH_STREAM(&pPtpPort->streamList, pPtpStream)
      {
         if (pPtpStream->config.mode == ZL303XX_PTP_STREAM_SLAVE_ONLY)
         {
            if (strictFlag == ZL303XX_TRUE)
            {
               ZL303XX_TRACE_ALWAYS(
                     "   (ERROR) G.8275.1 Attempt to set Port(%u) to masterOnly but Stream(%u) is SLAVE_ONLY.",
                     pPtpPort->clockPortHandle, pPtpStream->clockStreamHandle, 0,0,0,0);

               status = ZL303XX_PARAMETER_INVALID;
            }
            else
            {
               ZL303XX_TRACE_ALWAYS(
                     "   (WARNING) G.8275.1 Setting Port(%u) to masterOnly with Stream(%u) as SLAVE_ONLY.",
                     pPtpPort->clockPortHandle, pPtpStream->clockStreamHandle, 0,0,0,0);
            }
         }
      }
   }

   /* Update the Deprecated PORT member as well (doesn't matter if this value
    * remains consistent since it is used nowhere. */
   if (status == ZL303XX_OK)
   {
      pPtpPort->config.masterOnly = bMasterOnly;
   }

   return status;
}  /* END zl303xx_PtpG8275PortMasterOnlyCheck */

/**
    zl303xx_PtpG8275PtpMsgHeaderGet

  Details:
   Routine to gather the proper data values of egress PTP header fields for
   messages transmitted from clocks running this Profile.

  Parameters:
   [in]  pPtpStream  Pointer to the PTP Stream data structure.

   [out] pOutMsgHeader  Structure containing the PTP message Header values
                              for this profile.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      pPtpStream or pPtpMsgHeader is NULL.
     ZL303XX_PARAMETER_INVALID    The clockState value of the associated
                                       G.8275 clock is out of range.

  Notes:    The default Header fields for ANNOUNCE messages are gathered prior to
            this Profile specific routine. Therefore, profiles may update only
            the fields required (OR update everything just to be safe).

 *****************************************************************************/
zlStatusE zl303xx_PtpG8275PtpMsgHeaderGet(
      zl303xx_ClockStreamS *pPtpStream,
      void *pOutPtpMsgHeader)
{
   zlStatusE status = ZL303XX_OK;

   /* Cast the input type. */
   zl303xx_PtpV2MsgHeaderS *pPtpMsgHeader = (zl303xx_PtpV2MsgHeaderS *)pOutPtpMsgHeader;

   /* Check input pointers. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pPtpStream) |
               ZL303XX_CHECK_POINTER(pPtpMsgHeader);
   }

   /* Call the proper routine for the PROFILE and messageType. */
   if (status == ZL303XX_OK)
   {
      /* The alternateMaster flag applies to ANNOUNCE, SYNC, FOLLOWUP &
       * DELAY_RESP Messages for 8275.2 only. */
      if (pPtpStream->portData->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_2)
      {
         if ((pPtpMsgHeader->messageType == ZL303XX_MSG_ID_SYNC) ||
             (pPtpMsgHeader->messageType == ZL303XX_MSG_ID_ANNOUNCE) ||
             (pPtpMsgHeader->messageType == ZL303XX_MSG_ID_FOLLOWUP) ||
             (pPtpMsgHeader->messageType == ZL303XX_MSG_ID_DELAY_RESP))
         {
            if (pPtpStream->streamSubstate != PTP_STREAM_SUBSTATE_MASTER)
               setFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_ALTERNATE_MASTER);
            else
               clearFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_ALTERNATE_MASTER);
         }
      }

      /* The synchronizationUncertain flag applies only to ANNOUNCE Messages
       * for all G.8275 profiles. */
      if (pPtpMsgHeader->messageType == ZL303XX_MSG_ID_ANNOUNCE)
      {
         if (pPtpStream->portData->clock->config.synchronizationUncertain.enabled)
         {
            if (pPtpStream->portData->clock->profileDS.g8275.clockState != ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED)
               setFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_SYNCHRO_UNCERTAIN);
            else
               clearFlag(pPtpMsgHeader->flagField, PTP2_FLAGS_SYNCHRO_UNCERTAIN);
         }
      }

      /* The ANNOUNCE message has a lot more fields to update. */
      if (pPtpMsgHeader->messageType == ZL303XX_MSG_ID_ANNOUNCE)
      {
         zl303xx_PtpG8275AnncHeaderGet(pPtpStream, pPtpMsgHeader);
      }
   }

   return status;
}  /* END zl303xx_PtpG8275PtpMsgHeaderGet */

/**
    zl303xx_PtpG8275AnncHeaderGet

  Details:
   Routine to gather the proper HEADER values of egress Announce messages for
   clocks running this Profile.

  Parameters:
   [in]  pPtpStream  Pointer to the PTP Stream data structure (as a void).

   [out] pOutMsgHeader  Structure containing the PTP message Header values
                              for this profile.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      pPtpStream or pPtpMsgHeader is NULL.
     ZL303XX_PARAMETER_INVALID    The clockState value of the associated
                                       G.8275 clock is out of range.

  Notes:    The default Header fields for ANNOUNCE messages are gathered prior to
            this Profile specific routine. Therefore, profiles may update only
            the fields required (OR update everything just to be safe).

 *****************************************************************************/
zlStatusE zl303xx_PtpG8275AnncHeaderGet(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgHeaderS *pOutPtpMsgHeader)
{
   zlStatusE status = ZL303XX_OK;

   /* Local pointer to dereference the PTP objects. */
   zl303xx_PtpClockS *pPtpClock  = NULL;

   /* A local variable to track Clock State. */
   zl303xx_PtpG8275p1ClockStateE currClkState;

   /* Check input pointers. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pPtpStream) |
               ZL303XX_CHECK_POINTER(pOutPtpMsgHeader);
   }

   /* Check that this is an ANNOUNCE message. */
   if (status == ZL303XX_OK)
   {
      if (pOutPtpMsgHeader->messageType != ZL303XX_MSG_ID_ANNOUNCE)
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* If the bypassEnabled flag is TRUE, this routine updates none of the
    * advertised ANNOUNCE members. RETURN immediately. */
   if (status == ZL303XX_OK)
   {
      /* Dereference the Clock object. */
      pPtpClock = pPtpStream->portData->clock;

      if (pPtpClock->config.profileCfg.g8275p1.bypassEnabled == ZL303XX_TRUE)
      {
         return status;
      }
   }

   /* This routine only modifies the Header fields of ANNOUNCE messages. */
   if (status == ZL303XX_OK)
   {
      /* Check current clockState. */
      if (pPtpClock->profileDS.g8275.clockState >= ZL303XX_PTP_G8275p1_CLOCK_STATE_UNKNOWN)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpG8275AnncHeaderGet: (WARNING): UNKNOWN Clock State (%d).",
               pPtpClock->profileDS.g8275.clockState, 0,0,0,0,0);
         ZL303XX_TRACE_ALWAYS(
               "                              >>> Assuming FREERUN...", 0,0,0,0,0,0);

         currClkState = ZL303XX_PTP_G8275p1_CLOCK_STATE_FREERUN;
      }
      else
      {
         currClkState = pPtpClock->profileDS.g8275.clockState;
      }


      /* Set the Announce HEADER fields based on the State of the G.8275.1 clock. */
      /****************************************************************************/

      /** The following fields are not modified... ***/
      /* messageType */
      /* transportSpecific */
      /* versionPTP */
      /* messageLength */
      /* domainNumber */
      /* correctionField */
      /* sequenceId */
      /* control */
      /* logMessageInterval */

      /* sourcePortIdentity */
      /* We advertise our own portIdentity (this should already be filled in but
       * update again just to be sure.  */
      pOutPtpMsgHeader->sourcePortIdentity = pPtpStream->portData->portDS.portIdentity;


      /* flagField */
      /*************/
      {
         /* leap59 & leap61 */
         /*******************/
         /* First Clear the current values. */
         clearFlag(pOutPtpMsgHeader->flagField, PTP2_FLAGS_LI_61);
         clearFlag(pOutPtpMsgHeader->flagField, PTP2_FLAGS_LI_59);

         /* Update based on clockState. */
         if ((currClkState == ZL303XX_PTP_G8275p1_CLOCK_STATE_ACQUIRING) ||
             (currClkState == ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED))
         {
            if (pPtpClock->timePropertiesDS.leap61)
               setFlag(pOutPtpMsgHeader->flagField, PTP2_FLAGS_LI_61);
            if (pPtpClock->timePropertiesDS.leap59)
               setFlag(pOutPtpMsgHeader->flagField, PTP2_FLAGS_LI_59);
         }


         /* currentUtcOffsetValid */
         /*************************/
         /* First Clear the current values. */
         clearFlag(pOutPtpMsgHeader->flagField, PTP2_FLAGS_CURRENT_UTC_OFFSET_VALID);

         /* Update based on clockType and clockState. */
         if (pPtpClock->config.clockType == ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_GM)
         {
            /* FREERUN == FALSE */
            if (currClkState == ZL303XX_PTP_G8275p1_CLOCK_STATE_FREERUN)
            {
               clearFlag(pOutPtpMsgHeader->flagField, PTP2_FLAGS_CURRENT_UTC_OFFSET_VALID);
            }

            /* LOCKED or HOLDOVER_IN_SPEC == TRUE */
            else if ((currClkState == ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED) ||
                     (currClkState == ZL303XX_PTP_G8275p1_CLOCK_STATE_HOLDOVER_IN_SPEC))
            {
               setFlag(pOutPtpMsgHeader->flagField, PTP2_FLAGS_CURRENT_UTC_OFFSET_VALID);
            }

            /* ACQUIRING or HOLDOVER_OUT_SPEC == From active Time Properties */
            else
            {
               if (pPtpClock->timePropertiesDS.currentUtcOffsetValid)
               {
                  setFlag(pOutPtpMsgHeader->flagField, PTP2_FLAGS_CURRENT_UTC_OFFSET_VALID);
               }
            }
         }
         else  /* ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_BC */
         {
            /* FREERUN == FALSE */
            if (currClkState == ZL303XX_PTP_G8275p1_CLOCK_STATE_FREERUN)
            {
               clearFlag(pOutPtpMsgHeader->flagField, PTP2_FLAGS_CURRENT_UTC_OFFSET_VALID);
            }

            /* ACQUIRING or HOLDOVER_IN_SPEC == TRUE */
            else if ((currClkState == ZL303XX_PTP_G8275p1_CLOCK_STATE_ACQUIRING) ||
                     (currClkState == ZL303XX_PTP_G8275p1_CLOCK_STATE_HOLDOVER_IN_SPEC))
            {
               setFlag(pOutPtpMsgHeader->flagField, PTP2_FLAGS_CURRENT_UTC_OFFSET_VALID);
            }

            /* LOCKED or HOLDOVER_OUT_SPEC == From active Time Properties */
            else
            {
               if (pPtpClock->timePropertiesDS.currentUtcOffsetValid)
               {
                  setFlag(pOutPtpMsgHeader->flagField, PTP2_FLAGS_CURRENT_UTC_OFFSET_VALID);
               }
            }
         }

         /* ptpTimescale */
         /*******************/
         /* Assume PTP Time Scale and Clear where necessary. */
         setFlag(pOutPtpMsgHeader->flagField, PTP2_FLAGS_PTP_TIMESCALE);

         /* Only the LOCKED-BC case has to be handled. */
         if ((pPtpClock->config.clockType != ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_GM) &&
             (currClkState == ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED))
         {
            if (pPtpClock->timePropertiesDS.ptpTimescale == ZL303XX_FALSE)
            {
               clearFlag(pOutPtpMsgHeader->flagField, PTP2_FLAGS_PTP_TIMESCALE);
            }
         }

         /* timeTraceable */
         /*******************/
         /* Assume Time Traceable and CLEAR where needed. */
         setFlag(pOutPtpMsgHeader->flagField, PTP2_FLAGS_TIME_TRACEABLE);

         /* For both GM and BC:
          * - FREERUN & HOLDOVER_OUT_SPEC = FALSE.
          * - ACQUIRING & HOLDOVER_IN_SPEC = TRUE. */
         if ((currClkState == ZL303XX_PTP_G8275p1_CLOCK_STATE_FREERUN) ||
             (currClkState == ZL303XX_PTP_G8275p1_CLOCK_STATE_HOLDOVER_OUT_SPEC) ||
             (currClkState == ZL303XX_PTP_G8275p1_CLOCK_STATE_UNKNOWN))
         {
            clearFlag(pOutPtpMsgHeader->flagField, PTP2_FLAGS_TIME_TRACEABLE);
         }

         /* Only the LOCKED-BC case is left. */
         if ((pPtpClock->config.clockType != ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_GM) &&
             (currClkState == ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED))
         {
            if (pPtpClock->timePropertiesDS.timeTraceable == ZL303XX_FALSE)
            {
               clearFlag(pOutPtpMsgHeader->flagField, PTP2_FLAGS_TIME_TRACEABLE);
            }
         }

         /* frequencyTraceable */
         /*******************/
         /* Assume NOT Frequency Traceable and SET where needed. */
         /* For both GM and BC: FREERUN = FALSE */
         clearFlag(pOutPtpMsgHeader->flagField, PTP2_FLAGS_FREQ_TRACEABLE);

         /* This was evaluated previously */
         if (pPtpClock->profileDS.g8275.pllFrequencyTraceable == ZL303XX_TRUE)
         {
            setFlag(pOutPtpMsgHeader->flagField, PTP2_FLAGS_FREQ_TRACEABLE);
         }
      }
   }

   return status;
}  /* END zl303xx_PtpG8275AnncHeaderGet */

/**
    zl303xx_PtpG8275AnncDataGet

  Details:
   Routine to gather the proper DATA values of egress Announce messages for
   clocks running this Profile.

  Parameters:
   [in]  pPtpStream  Pointer to the PTP Stream data structure (as a void).

   [out] pOutAnncData   Structure containing the Announce data values for
                              this profile.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      pPtpStream or pAnncData is NULL.
     ZL303XX_PARAMETER_INVALID    The clockState value of the associated
                                       G.8275 clock is out of range.

  Notes:    The default Announce fields are gathered prior to this Profile
            specific routine. Therefore, profiles may update only the fields
            required (OR update everything just to be safe).
         If the bypassEnabled flag is TRUE, this routine updates none of the
            advertised ANNOUNCE members.

 *****************************************************************************/
zlStatusE zl303xx_PtpG8275AnncDataGet(
      zl303xx_ClockStreamS *pPtpStream,
      void *pOutAnncData)
{
   zlStatusE status = ZL303XX_OK;

   /* Local pointer to dereference the PTP objects. */
   zl303xx_PtpClockS *pPtpClock  = NULL;

   /* A local variable to track Clock State. */
   zl303xx_PtpG8275p1ClockStateE currClkState;

   /* Cast the input type. */
   zl303xx_PtpV2MsgAnnounceS *pAnncData = (zl303xx_PtpV2MsgAnnounceS *)pOutAnncData;

   /* Check input pointers. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pPtpStream) |
               ZL303XX_CHECK_POINTER(pAnncData);
   }

   /* If the bypassEnabled flag is TRUE, this routine updates none of the
    * advertised ANNOUNCE members. RETURN immediately. */
   if (status == ZL303XX_OK)
   {
      /* Dereference the Clock object. */
      pPtpClock  = pPtpStream->portData->clock;

      if (pPtpClock->config.profileCfg.g8275p1.bypassEnabled == ZL303XX_TRUE)
      {
         return status;
      }
   }

   if (status == ZL303XX_OK)
   {
      /* Check current clockState. */
      if (pPtpClock->profileDS.g8275.clockState >= ZL303XX_PTP_G8275p1_CLOCK_STATE_UNKNOWN)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpG8275AnncDataGet: (WARNING): UNKNOWN Clock State (%d).",
               pPtpClock->profileDS.g8275.clockState, 0,0,0,0,0);
         ZL303XX_TRACE_ALWAYS(
               "                           >>> Assuming FREERUN...", 0,0,0,0,0,0);

         currClkState = ZL303XX_PTP_G8275p1_CLOCK_STATE_FREERUN;
      }
      else
      {
         currClkState = pPtpClock->profileDS.g8275.clockState;
      }


      /* Set the Announce fields based on the State of the G.8275.1 clock.    */
      /************************************************************************/

      /* currentUtcOffset */
      if (currClkState == ZL303XX_PTP_G8275p1_CLOCK_STATE_FREERUN)
      {
         /* FREERUN state advertises the LOCAL hardware setting. */
         pAnncData->currentUtcOffset = pPtpClock->config.localTimeProperties.currentUtcOffset;
      }
      else
      {
         /* Everything else advertises the current ParentDS value. */
         pAnncData->currentUtcOffset = pPtpClock->timePropertiesDS.currentUtcOffset;
      }

      /* Priority1 / Priority2 */
      /* For T-GM: always advertise its own priority 1 & 2 values (priority1
       *           should be 128 while priority2 should be the same for both
       *           the defaultDS and parentDS).
       * For T-BC: advertise its own priority 2 if UNLOCKED otherwise use the
       *           parentDS value. Priority1 will always be 128 when UNLOCKED. */
      pAnncData->grandmasterPriority1 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;
      if (currClkState == ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED)
      {
         pAnncData->grandmasterPriority2 = pPtpClock->parentDS.grandmasterPriority2;
      }
      else
      {
         /* If not LOCKED, always advertise the local priority2 value. */
         pAnncData->grandmasterPriority2 = pPtpClock->config.defaultDS.priority2;
      }

      /* Clock Quality */
      {
         /* Clock Class */
         pAnncData->grandmasterClockQuality.clockClass = pPtpClock->profileDS.g8275.pllClockClass;

         /* Accuracy & Variance */
         if (currClkState != ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED)
         {
            pAnncData->grandmasterClockQuality.clockAccuracy = ZL303XX_PTP_G8275p1_ACCURACY_UNLOCK;
            pAnncData->grandmasterClockQuality.offsetScaledLogVariance = ZL303XX_PTP_G8275p1_VARIANCE_UNLOCK;
         }

         else  /* This should have already been filled in prior to calling this profile binding. */
         {
            pAnncData->grandmasterClockQuality.clockAccuracy = pPtpClock->parentDS.grandmasterClockQuality.clockAccuracy;
            pAnncData->grandmasterClockQuality.offsetScaledLogVariance = pPtpClock->parentDS.grandmasterClockQuality.offsetScaledLogVariance;
         }
      }

      /* GrandmasterIdentity */
      /* For T-GM: always advertise its own ClockIdentity (should be both the
       *           defaultDS and parentDS value.
       * For T-BC: advertise its own ClockIdentity if UNLOCKED otherwise use the
       *           parentDS value.    */
      if (currClkState == ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED)
      {
         OS_MEMCPY(pAnncData->grandmasterIdentity,
                pPtpClock->parentDS.grandmasterIdentity,
                PTP_V2_CLK_IDENTITY_LENGTH);
      }
      else
      {
         /* If not LOCKED, always advertise the local hardware value. */
         OS_MEMCPY(pAnncData->grandmasterIdentity,
                pPtpClock->config.defaultDS.clockIdentity,
                PTP_V2_CLK_IDENTITY_LENGTH);
      }

      /* Steps Removed */
      if (currClkState == ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED)
      {
         /* CurrentDS members. */
         pAnncData->stepsRemoved = pPtpClock->currentDSInt.stepsRemoved;
      }
      else
      {
         /* If not LOCKED, always advertise 0. */
         pAnncData->stepsRemoved = 0;
      }

      /* TIME SOURCE. */
      if (currClkState == ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED)
      {
         pAnncData->timeSource = pPtpClock->timePropertiesDS.timeSource;
      }
      else
      {
         /* If not LOCKED, always advertise OSC. */
         pAnncData->timeSource = ZL303XX_TIME_SOURCE_INTERNAL_OSCILLATOR;
      }
   }

   return status;
}  /* END zl303xx_PtpG8275AnncDataGet */

/**
    zl303xx_PtpG8275BmcaDataAdd

  Details:
   Routine to extract Profile specific BMCA data from a Foreign Master Table
   entry and insert it into the BmcaCompareS structure.

  Parameters:
   [in]  pFmtEntry   Pointer to a Foreign Master Table entry from which to
                           extract the Profile specific data.

   [out] pBmcaDataS  Pointer to the BMCA structure.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      Either of the input pointers is invalid.
     ZL303XX_PARAMETER_INVALID    profileId of the entry is not G.8275.
     ZL303XX_STREAM_NOT_IN_USE    The Stream has a Signal Fail

 *****************************************************************************/
zlStatusE zl303xx_PtpG8275BmcaDataAdd(
      zl303xx_ForeignMasterRecordS *pFmtEntry,
      zl303xx_PtpEventClockBmcaEntryS *pBmcaDataS)
{
   zlStatusE status = ZL303XX_OK;

   /* Local pointer to keep the code more manageable. */
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Check input pointers. */
   /* Make sure the stream, port and clock pointers are OK as well. */
   /* If called from ProfileDef() module, this was done preciously. */
   if (status == ZL303XX_OK)
   {
      if ((pBmcaDataS == NULL) ||
          (pFmtEntry == NULL) ||
          (pFmtEntry->streamData == NULL) ||
          (pFmtEntry->streamData->portData == NULL) ||
          (pFmtEntry->streamData->portData->clock == NULL))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpG8275BmcaDataAdd: NULL Pointer", 0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
      else
      {
         pPtpStream = pFmtEntry->streamData;
      }
   }

   /* Make sure the specified profile of the entry is G.8275. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_G8275_PROFILE("zl303xx_PtpG8275p1BmcaDataAdd",
                              pPtpStream->portData->clock->config.profile);
   }

   /* If all is well, fill in the BMCA Compare structure with the FMT data. */
   if (status == ZL303XX_OK)
   {
      /* Port-Stream masterOnly value */
      /* For the masterOnly attribute, if the Stream is masterOnly then follow that
       * behavior. Otherwise, use the Port masterOnly value. */
      if (pPtpStream->config.mode == ZL303XX_PTP_STREAM_MASTER_ONLY)
      {
         pBmcaDataS->tpPhaseData.masterOnly = ZL303XX_TRUE;
      }
      else
      {
         /* Report the parent port value. */
         pBmcaDataS->tpPhaseData.masterOnly = pPtpStream->portData->config.masterOnly;
      }

      /* Port-Stream localPriority value */
      /* For the localPriority attribute, if the Stream has a value configured then
       * use it. Otherwise, use the Port localPriority value. */
      if (pPtpStream->config.profileCfg.g8275p1.localPriority != ZL303XX_PTP_G8275_LOCAL_PRI_DISABLED)
      {
         pBmcaDataS->tpPhaseData.localPriority =
                  pPtpStream->config.profileCfg.g8275p1.localPriority;
      }
      else
      {
         pBmcaDataS->tpPhaseData.localPriority =
                  pPtpStream->portData->config.profileCfg.g8275p1.localPriority;
      }

      pBmcaDataS->tpPhaseData.rxPortIdentity = pPtpStream->portData->portDS.portIdentity;

      /* Grandmaster values. */
      pBmcaDataS->tpPhaseData.grandmasterClockQuality = pFmtEntry->compareData.grandmasterClockQuality;
      pBmcaDataS->tpPhaseData.grandmasterPriority2 = pFmtEntry->compareData.grandmasterPriority2;
      OS_MEMCPY(pBmcaDataS->tpPhaseData.grandmasterIdentity,
             pFmtEntry->compareData.grandmasterIdentity,
             PTP_V2_CLK_IDENTITY_LENGTH);

      /* Other fields. */
      pBmcaDataS->tpPhaseData.stepsRemoved = pFmtEntry->compareData.stepsRemoved;
      pBmcaDataS->tpPhaseData.txPortIdentity = pFmtEntry->compareData.foreignPortIdentity;

      /* PTSF value */
      /* If the stream is FAILED, it should not be uploaded for BMCA selection.
       * In this case return an ERROR code so the entry is skipped for that
       * purpose; the data structure is still filled in.  */
      if (ZL303XX_TRUE == zl303xx_PtsfFlagGetAndMask(pPtpStream->clockStreamHandle, ZL303XX_PTSF_MAIN))
      {
         status = ZL303XX_STREAM_NOT_IN_USE;
      }
   }

   return status;
}  /* END zl303xx_PtpG8275BmcaDataAdd */

/**
    zl303xx_PtpG8275BmcaStreamDataAdd

  Details:
   Routine to extract Profile specific BMCA data from a PTP Stream Data Structure
   and insert it into a Profile Specific BMCA Comparison structure.

  Parameters:
   [in]  pPtpStream   Pointer to a Stream Data Structure from which to
                           extract the Profile specific data.

   [out] pBmcaDataS  Pointer to the BMCA structure for the Profile.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      Either of the input pointers is invalid.
     ZL303XX_PARAMETER_INVALID    profileId of the entry is not G.8275.
     ZL303XX_STREAM_NOT_IN_USE    The Stream has a Signal Fail

 *****************************************************************************/
zlStatusE zl303xx_PtpG8275BmcaStreamDataAdd(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpG8275ProfileCompareDataS *pBmcaDataS)
{
   zlStatusE status = ZL303XX_OK;

   /* Check input pointers. */
   /* Make sure the stream, port and clock pointers are OK as well. */
   /* If called from ProfileDef() module, this was done preciously. */
   if (status == ZL303XX_OK)
   {
      if ((pBmcaDataS == NULL) ||
          (pPtpStream == NULL))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpG8275BmcaStreamDataAdd: NULL Pointer", 0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
   }

   /* Make sure the specified profile of the entry is G.8275. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_G8275_PROFILE("zl303xx_PtpG8275BmcaStreamDataAdd",
                                 pPtpStream->portData->clock->config.profile);
   }

   /* If all is well, fill in the BMCA Compare structure with the FMT data. */
   if (status == ZL303XX_OK)
   {
      /* Port-Stream masterOnly value */
      /* For the masterOnly attribute, if the Stream is masterOnly then follow that
       * behavior. Otherwise, use the Port masterOnly value. */
      if (pPtpStream->config.mode == ZL303XX_PTP_STREAM_MASTER_ONLY)
      {
         pBmcaDataS->masterOnly = ZL303XX_TRUE;
      }
      else
      {
         /* Report the parent port value. */
         pBmcaDataS->masterOnly = pPtpStream->portData->config.masterOnly;
      }

      /* Port-Stream localPriority value */
      /* For the localPriority attribute, if the Stream has a value configured then
       * use it. Otherwise, use the Port localPriority value. */
      if (pPtpStream->config.profileCfg.g8275p1.localPriority != ZL303XX_PTP_G8275_LOCAL_PRI_DISABLED)
      {
         pBmcaDataS->localPriority = pPtpStream->config.profileCfg.g8275p1.localPriority;
      }
      else
      {
         pBmcaDataS->localPriority = pPtpStream->portData->config.profileCfg.g8275p1.localPriority;
      }

      pBmcaDataS->rxPortIdentity = pPtpStream->portData->portDS.portIdentity;

      /* Grandmaster values. */
      pBmcaDataS->grandmasterClockQuality = pPtpStream->farEndParentDS.u.v2.grandmasterClockQuality;
      pBmcaDataS->grandmasterPriority2 = pPtpStream->farEndParentDS.u.v2.grandmasterPriority2;
      OS_MEMCPY(pBmcaDataS->grandmasterIdentity,
             pPtpStream->farEndParentDS.u.v2.grandmasterIdentity,
             PTP_V2_CLK_IDENTITY_LENGTH);

      /* Other fields. */
      pBmcaDataS->stepsRemoved = pPtpStream->farEndCurrentDS.stepsRemoved;
      pBmcaDataS->txPortIdentity = pPtpStream->farEndParentDS.u.v2.parentPortIdentity;

      /* PTSF value */
      /* If the stream is FAILED, it should not be uploaded for BMCA selection.
       * In this case return an ERROR code so the entry is skipped for that
       * purpose; the data structure is still filled in.  */
      if (ZL303XX_TRUE == zl303xx_PtsfFlagGetAndMask(pPtpStream->clockStreamHandle, ZL303XX_PTSF_MAIN))
      {
         status = ZL303XX_STREAM_NOT_IN_USE;
      }
   }

   return status;
}  /* END zl303xx_PtpG8275BmcaStreamDataAdd */

/**
    zl303xx_PtpG8275BmcaVpDataAdd

  Details:
   Routine to extract Profile specific BMCA data from a Virtual PTP Port and
   insert it into the BmcaCompareS structure.

  Parameters:
   [in]  pPtpVirtualPort   Pointer to a Virtual PTP Port from which to
                                 extract the Profile specific data.

   [out] pBmcaDataS  Pointer to the BMCA structure.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      Either of the input pointers is invalid.
     ZL303XX_PARAMETER_INVALID    profileId of the entry is not G.8275 or the
                                       port is not a Virtual port.
     ZL303XX_STREAM_NOT_IN_USE    The Port has a Signal Fail

 *****************************************************************************/
zlStatusE zl303xx_PtpG8275BmcaVpDataAdd(
      zl303xx_PortDataS *pPtpVirtualPort,
      void *pOutBmcaDataS)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpEventClockBmcaEntryS *pBmcaDataS = (zl303xx_PtpEventClockBmcaEntryS *)pOutBmcaDataS;

   /* Check input pointers. */
   if (status == ZL303XX_OK)
   {
      if ((pPtpVirtualPort == NULL) || (pBmcaDataS == NULL))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpG8275BmcaVpDataAdd: NULL Pointer", 0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
   }

   /* Make sure the specified profile of the entry is G.8275.1. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_G8275_PROFILE("zl303xx_PtpG8275BmcaVpDataAdd",
                                       pPtpVirtualPort->clock->config.profile);
   }

   /* Make sure the Port is configured as a Virtual Port. */
   if (status == ZL303XX_OK)
   {
      if (pPtpVirtualPort->virtualData.isVirtualPort != ZL303XX_TRUE)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpG8275BmcaVpDataAdd: Port(%d) NOT configured for Virtual operations",
               pPtpVirtualPort->clockPortHandle, 0,0,0,0,0);

         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* If all is well, fill in the BMCA Compare structure with the Virtual Port data. */
   if (status == ZL303XX_OK)
   {
      /* Virtual Ports can always be 'SLAVE'. Their whole purpose is to provide
       * a reference; they never send anything so never a MASTER.  */
      pBmcaDataS->tpPhaseData.masterOnly = ZL303XX_FALSE;

      pBmcaDataS->tpPhaseData.localPriority = pPtpVirtualPort->virtualData.config.localPriority;
      pBmcaDataS->tpPhaseData.rxPortIdentity = pPtpVirtualPort->portDS.portIdentity;

      /* Grandmaster values. */
      pBmcaDataS->tpPhaseData.grandmasterClockQuality = pPtpVirtualPort->virtualData.config.clockQuality;
      pBmcaDataS->tpPhaseData.grandmasterPriority2 = pPtpVirtualPort->virtualData.config.priority2;
      OS_MEMCPY(pBmcaDataS->tpPhaseData.grandmasterIdentity,
             pPtpVirtualPort->virtualData.config.clockIdentity,
             PTP_V2_CLK_IDENTITY_LENGTH);

      /* Other fields. */
      pBmcaDataS->tpPhaseData.stepsRemoved = pPtpVirtualPort->virtualData.config.stepsRemoved;
      pBmcaDataS->tpPhaseData.txPortIdentity = pBmcaDataS->tpPhaseData.rxPortIdentity;

      /* PTSF value */
      /* If the VP is FAILED, it should not be uploaded for BMCA selection.
       * In this case return an ERROR code so the entry is skipped for that
       * purpose; the data structure is still filled in.  */
      if (ZL303XX_TRUE == pPtpVirtualPort->virtualData.config.ptsf)
      {
         status = ZL303XX_STREAM_NOT_IN_USE;
      }
   }

   return status;
}  /* END zl303xx_PtpG8275BmcaVpDataAdd */

/* zl303xx_PtpG8275p1MsgIntervalCheck */
/** 

   Checks if an specified message Interval value is within the range supported
   by the Profile for that messageType.

  Parameters:
   [in]  messageType       PTP messageType for which to perform the profile
                                 specific check.
   [in]  log2MsgInterval   Message Interval to verify.
   [in]  strictFlag        Flag indicating whether to over-write erroneous
                                 values with the default value or return the
                                 ZL303XX_PARAMETER_INVALID error.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      log2MsgInterval pointer is NULL.
     ZL303XX_PARAMETER_INVALID    log2MsgInterval is outside the permitted
                                       range for the messageType and profile.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275p1MsgIntervalCheck(
      Uint32T messageType,
      Sint8T *log2MsgInterval,
      zl303xx_BooleanE strictFlag)
{
   /* Check Input pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(log2MsgInterval);

   /* Both G.8275.1 v1 and v2 have the same message rate constraints so use either
    * profile ID.  */
   zl303xx_PtpProfileE profileId = ZL303XX_PTP_PROFILE_TELECOM_G8275_1v2;

   Sint8T intervalMin, intervalMax, intervalDef = 0;

   if (status ==ZL303XX_OK)
   {
      if (messageType == ZL303XX_MSG_ID_ANNOUNCE)
      {
         intervalMin = ZL303XX_PTP_G8275p1_LOG_ANNC_MIN;
         intervalMax = ZL303XX_PTP_G8275p1_LOG_ANNC_MAX;
         intervalDef = ZL303XX_PTP_G8275p1_LOG_ANNC_DEF;
      }
      else if (messageType == ZL303XX_MSG_ID_SYNC)
      {
         intervalMin = ZL303XX_PTP_G8275p1_LOG_SYNC_MIN;
         intervalMax = ZL303XX_PTP_G8275p1_LOG_SYNC_MAX;
         intervalDef = ZL303XX_PTP_G8275p1_LOG_SYNC_DEF;
      }
      else if ((messageType == ZL303XX_MSG_ID_DELAY_RESP) ||
               (messageType == ZL303XX_MSG_ID_DELAY_REQ))
      {
         intervalMin = ZL303XX_PTP_G8275p1_LOG_DELAY_MIN;
         intervalMax = ZL303XX_PTP_G8275p1_LOG_DELAY_MAX;
         intervalDef = ZL303XX_PTP_G8275p1_LOG_DELAY_DEF;
      }
      else
      {
         /* There are no restrictions so return OK to simplify the code. */
         return ZL303XX_OK;
      }

      /* Perform any checks for any restricted messageTypes for this profile. */
      status = zl303xx_PtpG8275MsgIntervalCheck(profileId, messageType,
                                       log2MsgInterval, strictFlag,
                                       &intervalMin, &intervalMax, &intervalDef);
   }

   return status;
}  /* END zl303xx_PtpG8275p1MsgIntervalCheck */

/* zl303xx_PtpG8275p2MsgIntervalCheck */
/** 

   Checks if an specified message Interval value is within the range supported
   by the Profile for that messageType.

  Parameters:
   [in]  messageType       PTP messageType for which to perform the profile
                                 specific check.
   [in]  log2MsgInterval   Message Interval to verify.
   [in]  strictFlag        Flag indicating whether to over-write erroneous
                                 values with the default value or return the
                                 ZL303XX_PARAMETER_INVALID error.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      log2MsgInterval pointer is NULL.
     ZL303XX_PARAMETER_INVALID    log2MsgInterval is outside the permitted
                                       range for the messageType and profile.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275p2MsgIntervalCheck(
      Uint32T messageType,
      Sint8T *log2MsgInterval,
      zl303xx_BooleanE strictFlag)
{
   /* Check Input pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(log2MsgInterval);

   /* Both G.8275.1 v1 and v2 have the same message rate constraints so use either
    * profile ID.  */
   zl303xx_PtpProfileE profileId = ZL303XX_PTP_PROFILE_TELECOM_G8275_2;

   Sint8T intervalMin, intervalMax, intervalDef = 0;

   if (status ==ZL303XX_OK)
   {
      if (messageType == ZL303XX_MSG_ID_ANNOUNCE)
      {
         intervalMin = ZL303XX_PTP_G8275p2_LOG_ANNC_MIN;
         intervalMax = ZL303XX_PTP_G8275p2_LOG_ANNC_MAX;
         intervalDef = ZL303XX_PTP_G8275p2_LOG_ANNC_DEF;
      }
      else if (messageType == ZL303XX_MSG_ID_SYNC)
      {
         intervalMin = ZL303XX_PTP_G8275p2_LOG_SYNC_MIN;
         intervalMax = ZL303XX_PTP_G8275p2_LOG_SYNC_MAX;
         intervalDef = ZL303XX_PTP_G8275p2_LOG_SYNC_DEF;
      }
      else if ((messageType == ZL303XX_MSG_ID_DELAY_RESP) ||
               (messageType == ZL303XX_MSG_ID_DELAY_REQ))
      {
         intervalMin = ZL303XX_PTP_G8275p2_LOG_DELAY_MIN;
         intervalMax = ZL303XX_PTP_G8275p2_LOG_DELAY_MAX;
         intervalDef = ZL303XX_PTP_G8275p2_LOG_DELAY_DEF;
      }
      else
      {
         /* There are no restrictions so return OK to simplify the code. */
         return ZL303XX_OK;
      }

      /* Perform any checks for any restricted messageTypes for this profile. */
      status = zl303xx_PtpG8275MsgIntervalCheck(profileId, messageType,
                                       log2MsgInterval, strictFlag,
                                       &intervalMin, &intervalMax, &intervalDef);
   }

   return status;
}  /* END zl303xx_PtpG8275p2MsgIntervalCheck */

/* zl303xx_PtpG8275p1AnncIntervalCheck */
/** 

   Checks if an ANNOUNCE message Interval value is within the range supported by
   the Profile.

  Parameters:
   [in]  anncInterval   log2 of the Announce message interval.
   [in]  strictFlag     ZL303XX_TRUE to return an error status. Otherwise,
                              ZL303XX_OK is returned with extra logging.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275MsgIntervalCheck(
      zl303xx_PtpProfileE profileId,
      Uint32T messageType,
      Sint8T *msgInterval,
      zl303xx_BooleanE strictFlag,
      Sint8T *intervalMin,
      Sint8T *intervalMax,
      Sint8T *intervalDef)
{
   zlStatusE status = ZL303XX_OK;
   const char *msgStr = zl303xx_PtpMessageTypeToStr(messageType);
   const char *profileStr = (profileId == ZL303XX_PTP_PROFILE_TELECOM_G8275_2)
                                 ? "G.8275.2"
                                 : "G.8275.1";

   /* Check the range. */
   if (status ==ZL303XX_OK)
   {
      if ((*msgInterval < *intervalMin) ||
          (*msgInterval > *intervalMax))
      {
         /* If the STRICT flag is enabled, apply the Profile Specific limits. */
         if (strictFlag == ZL303XX_FALSE)
         {
            ZL303XX_TRACE_ALWAYS(
                  "   (WARNING) %s: %s Interval=%d is invalid. Range {%d:%d} Defaulting to %d.",
                  profileStr, msgStr, *msgInterval,
                  *intervalMin, *intervalMax, *intervalDef);

            /* Set default, return no error. */
            *msgInterval = *intervalDef;
         }
         else
         {
            ZL303XX_TRACE_ALWAYS(
                  "   (ERROR) %s: %s Interval(%d) is invalid. Range {%d:%d}",
                  profileStr, msgStr, *msgInterval, *intervalMin, *intervalMax, 0);
            status = ZL303XX_PARAMETER_INVALID;
         }
      }
   }

   return status;
}  /* END zl303xx_PtpG8275MsgIntervalCheck */

/*

  Function Name:
    zl303xx_PtpG8275TlvAppend

  Details:
   Appends all necessary G.8275 specific TLVs into the buffer provided based
   on transmit settings.

  Parameters:
   [in]  pPtpStream     Pointer to the stream associated with this buffer.
   [in]  messageType    PTP messageType being sent.
   [in]  pOutMsgBuf     Pointer to the start of the PTP Message to be sent
                              (byte-0 of the TX buffer). Provided in case the
                              profile requires additional message data.

   [out] pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes added into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpG8275TlvAppend(
      zl303xx_ClockStreamS *pPtpStream,
      Uint32T messageType,
      Uint8T *pOutMsgBuf,
      Uint8T *pOutTlvBuf)
{
   /* Default return value. */
   Uint16T tlvBytes = 0;

   /* Verify the Stream Data pointer and buffers. */
   if ((pPtpStream == NULL) || (pOutMsgBuf == NULL) ||(pOutTlvBuf == NULL))
   {
      /* Return immediately to avoid complicated code. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "zl303xx_PtpG8275TlvAppend: ERROR: NULL Pointer (%p, %p, %p)",
            pPtpStream, pOutMsgBuf, pOutTlvBuf, 0,0,0);
       return 0;
   }

   /* The only TLV to attach is for G.8175.2 GRANT Messages. */
   if ((pPtpStream->portData->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_2) &&
       (messageType == ZL303XX_MSG_ID_SIGNALING))
   {
      /* We only care about GRANT TLVs */
      Uint16T tlvType, tlvLength;
      (void)zl303xx_PtpTlvTypeLengthGet(
            pOutMsgBuf + ZL303XX_PTP_V2_HEADER_LEN + ZL303XX_PTP_V2_SIGNAL_DATA_LEN,
            &tlvType, &tlvLength);

      /* We only care about GRANT TLVs */
      /* Only send if the Rate members are provisioned. */
      if ((tlvType == PTP_TLV_GRANT_UNICAST_TX) &&
           !( (pPtpStream->portData->config.profileCfg.g8275p1.tsIfRate.interfaceBitPeriod.hi == 0) &&
              (pPtpStream->portData->config.profileCfg.g8275p1.tsIfRate.interfaceBitPeriod.lo == 0) &&
              (pPtpStream->portData->config.profileCfg.g8275p1.tsIfRate.numberBitsBeforeTimestamp == 0) &&
              (pPtpStream->portData->config.profileCfg.g8275p1.tsIfRate.numberBitsAfterTimestamp == 0)))
      {
         /* Temp variable to avoid compiler warning. */
         Uint8T profileIdent[PTP_PROFILE_IDENTITY_LENGTH];
         OS_MEMCPY(profileIdent, Zl303xx_PtpG8275p2ProfileInfo.profileIdentifier, PTP_PROFILE_IDENTITY_LENGTH);

         tlvBytes = zl303xx_PtpTlvOrganizationHeaderPack(
                           profileIdent,
                           Zl303xx_PtpG8275TsIfRateTlvSubType,
                           pOutTlvBuf);

         tlvBytes += zl303xx_PtpG8275TimestampIfRateTlvPack(
               &pPtpStream->portData->config.profileCfg.g8275p1.tsIfRate,
               (pOutTlvBuf + tlvBytes));

         /* Add the length within this ORG_EXT TLV if a proper version was specified. */
         /* length = (Ttl - 4 bytes of Type & Length) */
         if (tlvBytes > 0)
         {
            zl303xx_PtpTlvLengthSet((tlvBytes - ZL303XX_PTP_TLV_VALUE_OFFSET), pOutTlvBuf);
         }
      }
   }

   return tlvBytes;
}  /* END zl303xx_PtpG8275TlvAppend */

/*

  Function Name:
    zl303xx_PtpG8275TlvHandle

  Details:
   Processes the G.8275 Telecom Phase Profile specific TLVs.

  Parameters:
   [in]  streamHandle   Handle of the stream associated with this TLV buffer.
   [in]  pPtpMsgRx      Pointer the the received message structure (contains
                              all header and announce fields).
   [in]  pInTlvBuf      Pointer to the appended TLV buffer.

   [out] pOutTlvBuf     Pointer to the data portion of any output TLV buffer
                              (in response to the Rx TLV - In this case no
                              output buffer is packed). Note: this prototype
                              must follow the definition of the
                              zl303xx_PtpV2TlvCustomHandlerT type so the handler
                              can be bound to the main PTP stack.

  Return Value:
   Uint16T  Number of bytes packed into the response TLV (in this case = 0).
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpG8275TlvHandle(
      Uint32T streamHandle,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf,
      Uint8T *pOutTlvBuf)
{
   /* Default return value. */
   Uint16T tlvTxBytes = 0;

   /* Vendor TLV Header parameters. */
   Uint16T tlvType, tlvLength;
   Uint32T oui32, subtype32;

   /* Pointer to the stream Data. */
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Warning elimination */
   if ((pPtpMsgRx) || (pOutTlvBuf)) {;}

   /* Get a pointer to the stream data. */
   if (zl303xx_PtpStreamDataGetG8275(streamHandle, &pPtpStream) != ZL303XX_OK)
   {
      return 0;
   }

   /* Get the VENDER TLV fields. */
   /* -------------------------------------------------------
    * Field                      Length    Offset
    * ------------------------------------------------------
    * TLV Type (ORG_EXT)           2         0
    * lengthField                  2         2  (depends on subType)
    * OUI                          3         4  (0x00, 0x19, 0xA7)
    * subType                      3         7  (depends on subType)
    * ------------------------------------------------------
    * Total Length                10  (ZL303XX_PTP_TLV_ORG_EXT_HEADER_LEN)  */

   /* Type and Length */
   (void)zl303xx_PtpTlvTypeLengthGet(pInTlvBuf, &tlvType, &tlvLength);
   /* The OUI and Sub-Type fields are only 24-bit but for look-up, convert it to 32-bit. */
   (void)zl303xx_PtpTlvConvert3ByteFieldTo32Bit(
               (pInTlvBuf + ZL303XX_PTP_TLV_ORG_EXT_OUI_OFFSET), &oui32);
   (void)zl303xx_PtpTlvConvert3ByteFieldTo32Bit(
               (pInTlvBuf + ZL303XX_PTP_TLV_ORG_EXT_SUBTYPE_OFFSET), &subtype32);

   /* Handle the TLV based on the TLV subType. */
   if (subtype32 == ZL303XX_PTP_G8275_TLV_TS_IF_RATE)
   {
      if (tlvLength != ZL303XX_PTP_G8275_TLV_TS_IF_RATE_TTL_LEN)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpG8275TlvHandle: ERROR - TLV Length (expected %d: actusl %d)",
               ZL303XX_PTP_G8275_TLV_TS_IF_RATE_TTL_LEN, tlvLength, 0,0,0,0);
      }
      else
      {
         zl303xx_PtpG8275TimestampIfRateTlvUnpack(
                     pInTlvBuf + ZL303XX_PTP_TLV_ORG_EXT_HEADER_LEN,
                     &pPtpStream->profileDS.g8275.tsIfRate);
      }
   }

   return tlvTxBytes;
}  /* END zl303xx_PtpG8275TlvHandle */

/*

  Function Name:
    zl303xx_PtpG8275ClockDebugShow

  Details:
   Routine to display G.8275 (Telecom Phase Profile) specific clock parameters.

  Parameters:
   [in]  pClockS  Pointer to the PTP clock data structure (as a void).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pClockS is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275ClockDebugShow(void *pClockS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pClockS);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpClockS *pPtpClockS = (zl303xx_PtpClockS *)pClockS;
      const char *clockStateStr =
         zl303xx_PtpG8275p1ClockStateToStr(pPtpClockS->profileDS.g8275.clockState);
      const char *clockClassDescStr =
         zl303xx_PtpG8275p1ClockClassDescToStr(pPtpClockS->profileDS.g8275.clockClassDesc);
      const char *classEvalStr =
         zl303xx_PtpG8275p1ClassEvalMethodToStr(pPtpClockS->config.profileCfg.g8275p1.classEvalMethod);

      printf ("\nG.8275 Profile: Clock Data\n");
      printf ("-----------------------------------------------------\n");
      printf ("                 clockState: %u (%s) \n",
            pPtpClockS->profileDS.g8275.clockState,
            clockStateStr);
      printf ("             clockClassDesc: %u (%s) \n",
            pPtpClockS->profileDS.g8275.clockClassDesc,
            clockClassDescStr);
      printf ("           Advertised Class: %u \n",
            pPtpClockS->profileDS.g8275.pllClockClass);
      printf ("             Fallback Class: %u \n",
            pPtpClockS->profileDS.g8275.fallbackClass);
      printf ("             PRTC Holdover: %s \n",
            ((pPtpClockS->profileDS.g8275.prtcHoldover) ? "TRUE" : "FALSE"));
      printf ("        Frequency Traceable: %s \n",
            ((pPtpClockS->profileDS.g8275.pllFrequencyTraceable) ? "TRUE" : "FALSE"));
      printf ("                   Time Set: %s \n",
            ((pPtpClockS->profileDS.g8275.todSet) ? "TRUE" : "FALSE"));
      printf ("              localPriority: %u \n",
            pPtpClockS->config.profileCfg.g8275p1.localPriority);
      printf ("    class Evaluation Method: %u (%s) \n",
            pPtpClockS->config.profileCfg.g8275p1.classEvalMethod,
            classEvalStr);
      printf ("            Equipment Class: %u \n",
            pPtpClockS->config.profileCfg.g8275p1.equipmentClass);
      printf ("      G.8275 Bypass Enabled: %s \n",
            ((pPtpClockS->config.profileCfg.g8275p1.bypassEnabled) ? "TRUE" : "FALSE"));
      printf ("   T-TSC Holdover Supported: %s \n",
            ((pPtpClockS->config.profileCfg.g8275p1.holdoverSupported) ? "TRUE" : "FALSE"));
   }

   return status;
}  /* END zl303xx_PtpG8275ClockDebugShow */

/*

  Function Name:
    zl303xx_PtpG8275PortDebugShow

  Details:
   Routine to display G.8275 (Telecom Phase Profile) specific port parameters.

  Parameters:
   [in]  pPortS  Pointer to the PTP port data structure (as a void).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pPortS is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275PortDebugShow(void *pPortS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pPortS);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PortDataS *pPtpPortS = (zl303xx_PortDataS *)pPortS;

      printf ("\nG.8275 Profile: Port Configured Data\n");
      printf ("-----------------------------------------------------\n");
      printf ("              localPriority: %u \n",
            pPtpPortS->config.profileCfg.g8275p1.localPriority);
      printf ("                 masterOnly: %u \n",
            pPtpPortS->config.masterOnly);
      printf ("        denyServiceRequests: %s \n",
            (pPtpPortS->config.unicastNegotiation.denyServiceRequests) ? "TRUE" : "FALSE");

      if (pPtpPortS->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_2)
      {
         printf ("Timestamp Interface Parameters:\n");
         printf ("          bit period (attoSec): 0x%08X:%08X \n",
                     pPtpPortS->config.profileCfg.g8275p1.tsIfRate.interfaceBitPeriod.hi,
                     pPtpPortS->config.profileCfg.g8275p1.tsIfRate.interfaceBitPeriod.lo);
         printf ("         bits before timestamp: %u \n",
                     pPtpPortS->config.profileCfg.g8275p1.tsIfRate.numberBitsBeforeTimestamp);
         printf ("          bits after timestamp: %u \n",
                     pPtpPortS->config.profileCfg.g8275p1.tsIfRate.numberBitsAfterTimestamp);
      }
   }

   return status;
}  /* END zl303xx_PtpG8275PortDebugShow */

/*

  Function Name:
    zl303xx_PtpG8275StreamDebugShow

  Details:
   Routine to display G.8275 (Telecom Phase Profile) specific stream parameters.

  Parameters:
   [in]  pStreamS  Pointer to the PTP stream data structure (as a void).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pStreamS is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275StreamDebugShow(void *pStreamS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pStreamS);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      zl303xx_ClockStreamS *pPtpStreamS = (zl303xx_ClockStreamS *)pStreamS;

      printf ("\nG.8275 Profile: Stream Configured Data\n");
      printf ("-----------------------------------------------------\n");
      printf ("              localPriority: %u \n",
            pPtpStreamS->config.profileCfg.g8275p1.localPriority);
      printf ("                 masterOnly: %s \n",
            pPtpStreamS->config.mode == ZL303XX_PTP_STREAM_MASTER_ONLY ? "TRUE" : "FALSE");

      /* Clock UNCALIBRATED configuration*/
      printf ("     UNCALIBRATED behaviour:\n");
      printf ("             Use Previous Class: %d (%s)\n",
            pPtpStreamS->portData->clock->config.uncalibrated.usePreviousClass,
            (pPtpStreamS->portData->clock->config.uncalibrated.usePreviousClass) ? "TRUE" : "FALSE");
      printf ("                 Previous Class: %d\n",
            pPtpStreamS->portData->clock->uncalibrated.previousClockClass);
      printf ("                      Intervals: %d\n",
            pPtpStreamS->config.uncalibrated.anncIntervals);
      printf ("                  Lock Required: %d\n",
            pPtpStreamS->config.uncalibrated.lockRequired);
   }

   return status;
}  /* END zl303xx_PtpG8275StreamDebugShow */

/******************************************************************************/
/*******   USER API FUNCTION DEFINITIONS   ************************************/
/******************************************************************************/

/* zl303xx_PtpG8275ClockStateGet */
/**
   Telecom Profile for Phase only. Gets a clock's state based on the combination
   of local stream states.

  Parameters:
   [in]  clockHandle       Handle to an existing clock.
   [in]  g8275p1State      Parameter to return the G8275.1 clock state.
   [in]  g8275p1ClassDesc  Parameter to return the G8275.1 clock class Description.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      g8275p1State is NULL.
     ZL303XX_PARAMETER_INVALID    clockHandle is invalid.
     ZL303XX_INVALID_MODE         The clock is not running the G.8275.1 PTP profile.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275ClockStateGet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpG8275p1ClockStateE *g8275p1State,
      zl303xx_PtpG8275p1ClockClassDescE *g8275p1ClassDesc)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check the input pointer. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(g8275p1State) |
               ZL303XX_CHECK_POINTER(g8275p1ClassDesc);
   }

   /* Get the associated profile clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGetG8275(clockHandle, &pPtpClock);
   }

   /* Evaluate the state of every Stream on this clock to get the overall
    * ClockState. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpG8275ClockDataUpdate(pPtpClock);
   }

   /* Return the State value. */
   if (status == ZL303XX_OK)
   {
      *g8275p1State = pPtpClock->profileDS.g8275.clockState;
      *g8275p1ClassDesc = pPtpClock->profileDS.g8275.clockClassDesc;
   }

   return status;
}  /* END zl303xx_PtpG8275ClockStateGet */

/* zl303xx_PtpG8275p1ClockLocalPrioritySet */
/**
   Telecom Profile for Phase only. Sets a clock's localPriority parameter.

  Parameters:
   [in]  clockHandle    Handle to an existing clock.
   [in]  localPriority  localPriority value to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or localPriority is invalid.
     ZL303XX_INVALID_MODE          The clock is not running the G.8275.1 PTP profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275ClockLocalPrioritySet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint8T localPriority)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check the input parameter. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_PTP_G8275_LOCAL_PRI_CHECK(localPriority);
   }

   /* Get the associated profile clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGetG8275(clockHandle, &pPtpClock);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   /* Update the clock profile data parameter and trigger an event that the
    * clock configuration has changed. */
   if (status == ZL303XX_OK)
   {
      pPtpClock->config.profileCfg.g8275p1.localPriority = localPriority;

      /* free the Mutex */
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   /* Any time a Clock configuration is changed, the Clock State-Decision
    * algorithm should be run to re-evaluate its state (and that of any
    * associated ports & streams).   */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpClockBmcaUpdateInitiate(pPtpClock);
   }

   return status;
}  /* END zl303xx_PtpG8275ClockLocalPrioritySet */

/* zl303xx_PtpG8275p1ClockClassEvalMethodSet */
/**
   Telecom Profile for Phase only. Sets the method to use for determining which
   Clock Class evaluation method to use on this clock.

  Parameters:
   [in]  clockHandle    Handle to an existing clock.
   [in]  evalMethodId   Clock Class Evaluation Method to use..

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or evalMethodId is invalid.
     ZL303XX_INVALID_MODE          The clock is not running the G.8275.1 PTP profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275p1ClockClassEvalMethodSet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpG8275p1ClassEvalMethodE evalMethodId)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check the input parameter. */
   if (status == ZL303XX_OK)
   {
      if (evalMethodId > ZL303XX_PTP_G8275p1_CLASS_EVAL_METHOD3)
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Get the associated profile clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGetG8275(clockHandle, &pPtpClock);
   }

   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pPtpClock);
      status = mStatus;
   }

   /* Update the clock profile data parameter and trigger and re-evaluate the
    * egress clock class. */
   if (status == ZL303XX_OK)
   {
      pPtpClock->config.profileCfg.g8275p1.classEvalMethod = evalMethodId;

      /* Re-evaluate the egress clock Class. */
      status = zl303xx_PtpG8275ClockDataUpdate(pPtpClock);
   }

   /* free the Mutex (pass the active status). */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* END zl303xx_PtpG8275p1ClockClassEvalMethodSet */

/* zl303xx_PtpG8275p1ClockEquipmentClassSet */
/**
   Telecom Profile for Phase only. Equipment Class value to use for Table 6.4
   of G.8275.1 (T-GM; holdover - out-of-spec, Category 2).

  Parameters:
   [in]  clockHandle       Handle to an existing clock.
   [in]  equipmentClass    Clock Class to advertise for this scenario.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid.
     ZL303XX_INVALID_MODE          The clock is not running the G.8275.1 PTP profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275p1ClockEquipmentClassSet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint8T equipmentClass)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Get the associated profile clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGetG8275(clockHandle, &pPtpClock);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   /* Update the clock profile data parameter. */
   if (status == ZL303XX_OK)
   {
      /* Stored in 2 places. */
      pPtpClock->config.profileCfg.g8275p1.equipmentClass = equipmentClass;
      CLASS_GM_OUT_HLD_CAT2[ZL303XX_PTP_G8275p1_CLASS_EVAL_METHOD2] = equipmentClass;

      /* free the Mutex (pass the active status). */
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* END zl303xx_PtpG8275p1ClockEquipmentClassSet */

/* zl303xx_PtpG8275p1ClockBypassEnable */
/**
   Telecom Profile for Phase only. Allows the implementation to bypass the
   G.8275.1 restrictions on advertised Announce parameters and allow the user to
   set the advertised values via the ParentDS. (On a T-GM, the
     ParentDS = DefaultDS                  + TimePropertiesDS
                (class, accuracy, variance)  (frequencyTraceable, timeTraceable)

  Parameters:
   [in]  clockHandle    Handle to an existing clock.
   [in]  enableBypass   FALSE: use G.8275.1 restrictions - default setting
                           TRUE: bypass mode - ignore G.8275.1 restrictions

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or enableBypass is invalid.
     ZL303XX_INVALID_MODE          The clock is not running the G.8275.1 PTP profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275p1ClockBypassEnable(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_BooleanE enableBypass)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(enableBypass);
   }

   /* Get the associated profile clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGetG8275(clockHandle, &pPtpClock);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   /* Update the clock profile data parameter. */
   if (status == ZL303XX_OK)
   {
      /* Stored in 2 places. */
      pPtpClock->config.profileCfg.g8275p1.bypassEnabled = enableBypass;

      /* If Bypass is being enabled, then ignore the UNCALIBRATED class delay
       * as well. Otherwise, use it. */
      pPtpClock->config.uncalibrated.usePreviousClass = !(enableBypass);

      /* free the Mutex (pass the active status). */
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* END zl303xx_PtpG8275p1ClockBypassEnable */

/* zl303xx_PtpG8275ClockHoldoverSupportedSet */
/**
   Telecom Profile for Phase only. Allows T-TSC clocks to maintain holdover
   clock classes and use them in BMCA selection.

  Parameters:
   [in]  clockHandle          Handle to an existing clock.
   [in]  holdoverSupported    FALSE (default): The T-TSC clock always uses
                                       class 255 in BMCA selection.
                                 TRUE:  Allow T-TSC clocks to use HOLDOVER
                                        Classes (135/165) in BMCA selection.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or holdoverSupported is invalid.
     ZL303XX_INVALID_MODE          The clock is not running the G.8275 PTP profile.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275ClockHoldoverSupportedSet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_BooleanE holdoverSupported)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(holdoverSupported);
   }

   /* Get the associated profile clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGetG8275(clockHandle, &pPtpClock);
   }

   /* No MUTEX required. */

   /* Update the clock profile data parameter. */
   if (status == ZL303XX_OK)
   {
      /* Stored in 2 places. */
      pPtpClock->config.profileCfg.g8275p1.holdoverSupported = holdoverSupported;
   }

   return status;
}  /* END zl303xx_PtpG8275ClockHoldoverSupportedSet */

/* zl303xx_PtpG8275ClockTimestampIfRateTlvEn */
/**
   Telecom Profile for Phase only. Allows the implementation to append the
   profile specific Timestamp Interface Rate TLV to unicast GRANT messages
   (allows the client to correct for asymmetry).

  Parameters:
   [in]  clockHandle    Handle to an existing clock.
   [in]  tsIfRateTlvEn  TRUE: append the Timestamp Interface Rate TLV
                           FALSE: do not append

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or tsIfRateTlvEn is invalid.
     ZL303XX_INVALID_MODE          The clock is not running the G.8275 profile.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275ClockTimestampIfRateTlvEn(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_BooleanE tsIfRateTlvEn)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(tsIfRateTlvEn);
   }

   /* Get the associated profile clock data structure.
    * (An error is returned if the clock is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGetG8275(clockHandle, &pPtpClock);
   }

   /* Take the MUTEX to configure a custom TLV. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   /* Enable / Disable the TLV. */
   if (status == ZL303XX_OK)
   {
      /* Convert the OUI to an 'ID' for the lookup table. */
      Uint32T oui32;
      (void)zl303xx_PtpTlvConvert3ByteFieldTo32Bit(Zl303xx_PtpG8275_OUI, &oui32);

      if (tsIfRateTlvEn == ZL303XX_TRUE)
      {
         /* Add the custom TLV handler for this profile. */
         status = zl303xx_PtpTlvCustomHandlerSet(pPtpClock->clockHandle,
                                               PTP_TLV_VENDOR_EXTENSION,
                                               oui32,
                                               zl303xx_PtpG8275TlvHandle);
      }

      else
      {
         /* Delete the custom TLV handler for this profile. */
         status = zl303xx_PtpTlvCustomHandlerDel(pPtpClock->clockHandle,
                                               PTP_TLV_VENDOR_EXTENSION,
                                               oui32);
      }

      /* Regardless of the Handler status, release the MUTEX
       * (pass the active status). */
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* END zl303xx_PtpG8275ClockTimestampIfRateTlvEn */

/* zl303xx_PtpG8275PortLocalPrioritySet */
/**
   Telecom Profile for Phase only. Sets a port's localPriority parameter.

  Parameters:
   [in]  portHandle     Handle to an existing port.
   [in]  localPriority  Local Priority value of the Port used for the G.8275
                              Profile. Range: 1-255 for operational; 0 disables
                              this parameter for the port and uses the
                              associated stream values instead.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle or localPriority is invalid.
     ZL303XX_INVALID_MODE          The port is not running the G.8275 profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275PortLocalPrioritySet(
      zl303xx_PtpPortHandleT portHandle,
      Uint8T localPriority)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPtpPort = NULL;

   /* Check the input parameter. */
   if (status == ZL303XX_OK)
   {
      /* 0 is not permitted on the Port (on the stream, 0 means disabled) */
      status = ZL303XX_PTP_G8275_LOCAL_PRI_CHECK(localPriority);
   }

   /* Get the associated profile port data structure.
    * (An error is returned if the port is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGetG8275(portHandle, &pPtpPort);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpPort->clock);
   }

   /* Update the localPriority port setting for the Port. */
   if (status == ZL303XX_OK)
   {
      pPtpPort->config.profileCfg.g8275p1.localPriority = localPriority;
      status = zl303xx_PtpClockMutexUnlock(pPtpPort->clock, status);
   }

   /* Any time a Port configuration is changed, the Clock State-Decision
    * algorithm should be run to re-evaluate its state (and that of any
    * associated ports & streams).   */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpClockBmcaUpdateInitiate(pPtpPort->clock);
   }

   return status;
}  /* END zl303xx_PtpG8275PortLocalPrioritySet */

/* zl303xx_PtpG8275PortTimestampIfRateSet */
/**
   Telecom Profile for Phase only. Sets a port's Timestamp Interface
   configuration parameters (used by the Timestamp Interface RAte TLV of the
   G.8275.2 profile).

  Parameters:
   [in]  portHandle  Handle to an existing port.
   [in]  tsIfRateS   Attributes of the timestamp interface associated with
                           this PTP port. If NULL, the current configuration is
                           cleared. Refer to the following data structure:
                           zl303xx_PtpG8275TimestampIfRateTlvS

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle is invalid.
     ZL303XX_INVALID_MODE          The port is not running the G.8275 PTP profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275PortTimestampIfRateSet(
      zl303xx_PtpPortHandleT portHandle,
      zl303xx_PtpG8275TimestampIfRateTlvS *tsIfRateS)
{
   zlStatusE status = ZL303XX_OK;

   zl303xx_PtpG8275TimestampIfRateTlvS tsIfRateSet = {{0,0}, 0,0};
   zl303xx_PortDataS *pPtpPort = NULL;

   /* Get the associated profile port data structure.
    * (An error is returned if the port is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGetG8275(portHandle, &pPtpPort);
   }

   /* Check the input values. */
   if (status == ZL303XX_OK)
   {
      if (tsIfRateS != NULL)
      {
         tsIfRateSet = *tsIfRateS;
      }

      /* Lock the Clock Mutex to update the structure. */
      status = zl303xx_PtpClockMutexLock(pPtpPort->clock);
   }

   /* Update the timestamp interface attributes for the Port. */
   if (status == ZL303XX_OK)
   {
      pPtpPort->config.profileCfg.g8275p1.tsIfRate = tsIfRateSet;
      status = zl303xx_PtpClockMutexUnlock(pPtpPort->clock, status);
   }

   /* Since this attributes do not impact the BMCA selection, there is no need
    * to trigger the Clock State-Decision algorithm.  */

   return status;
}  /* END zl303xx_PtpG8275PortTimestampIfRateSet */

/* zl303xx_PtpG8275PortTimestampIfRateClear */
/**
   Telecom Profile for Phase only. Clears a port's Timestamp Interface
   configuration parameters to the empty set (meaning the Timestamp Interface
   Rate TLV of the G.8275.2 profile will not be appended).

  Parameters:
   [in]  portHandle  Handle to an existing port.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle is invalid.
     ZL303XX_INVALID_MODE          The port is not running the G.8275 PTP profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275PortTimestampIfRateClear(
      zl303xx_PtpPortHandleT portHandle)
{
   return zl303xx_PtpG8275PortTimestampIfRateSet(portHandle , NULL);
}  /* END zl303xx_PtpG8275PortTimestampIfRateClear */

/* zl303xx_PtpG8275StreamMasterOnlySet */
/**
   Telecom Profile for Phase Only.
   Sets a stream's masterOnly value; TRUE prohibits the stream from becoming the
   SLAVE.

  Parameters:
   [in]  streamHandle   Handle to an existing stream.
   [in]  bMasterOnly    ZL303XX_TRUE prohibits the Stream from becoming the SLAVE.
                           ZL303XX_FALSE allows normal BMC operation on the Stream and
                              allows individual Streams to set States according to
                              their own configurations.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or bMasterOnly is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275StreamMasterOnlySet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_BooleanE bMasterOnly)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(bMasterOnly);
   }

   /* Get a pointer to the stream data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream);
   }

   /* Send this to the Stream::config::mode attribute which will check the
    * profile-specific constraints and force the Clock State-Decision
    * algorithm Event to re-evaluate its state (and that of any associated
    * ports & streams).   */
   if (status == ZL303XX_OK)
   {
      if (bMasterOnly == ZL303XX_TRUE)
      {
         status = zl303xx_PtpStreamOverrideModeSet(streamHandle, ZL303XX_PTP_STREAM_MASTER_ONLY);
      }
      else
      {
         status = zl303xx_PtpStreamOverrideModeSet(streamHandle, ZL303XX_PTP_STREAM_USE_BMC);
      }
   }

   /* Update the Deprecated member as well. */
   if (status == ZL303XX_OK)
   {
      pPtpStream->config.profileCfg.g8275p1.notSlave = bMasterOnly;
   }

   return status;
}  /* END zl303xx_PtpG8275StreamMasterOnlySet */

/* zl303xx_PtpG8275StreamLocalPrioritySet */
/**
   Telecom Profile for Phase only. Sets a stream's localPriority parameter.

  Parameters:
   [in]  streamHandle   Handle to an existing stream.
   [in]  localPriority  Local Priority value of the Stream used for the
                              G.8275.1 Profile. This value takes priority over
                              the associated Port localPriority value.
                              Range: 1-255 for operational; 0 disables this
                              parameter for the stream and uses the associated
                              port values instead.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle is invalid.
     ZL303XX_INVALID_MODE          The stream is not running the G.8275.1 PTP profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275StreamLocalPrioritySet(
      zl303xx_PtpStreamHandleT streamHandle,
      Uint8T localPriority)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* No check the input parameter is required:
    *   1 - 255 are valid (active)
    *   0 = DISABLED. */

   /* Get the associated profile port data structure.
    * (An error is returned if the port is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGetG8275(streamHandle, &pPtpStream);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpStream->portData->clock);
   }

   /* Update the localPriority setting for the Stream. */
   if (status == ZL303XX_OK)
   {
      pPtpStream->config.profileCfg.g8275p1.localPriority = localPriority;
      status = zl303xx_PtpClockMutexUnlock(pPtpStream->portData->clock, status);
   }

   /* Any time a Stream configuration is changed, the Clock State-Decision
    * algorithm should be run to re-evaluate its state (and that of any
    * associated ports & streams).   */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpClockBmcaUpdateInitiate(pPtpStream->portData->clock);
   }

   return status;
}  /* END zl303xx_PtpG8275StreamLocalPrioritySet */

/* zl303xx_PtpG8275StreamTimestampIfRateGet */
/**
   Telecom Profile for Phase only. Retrieves the Timestamp Interface Rate
   parameters learned by this stream (from the the Timestamp Interface Rate TLV
   transmitted by the far end port).

  Parameters:
   [in]  streamHandle   Handle to an existing stream.
   [in]  tsIfRateS      The Timestamp Interface Rate parameters learned by
                              this stream. Refer to the following data
                              structure: zl303xx_PtpG8275TimestampIfRateTlvS

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle is invalid.
     ZL303XX_INVALID_POINTER       tsIfRateS is NULL.
     ZL303XX_INVALID_MODE          The stream is not running the G.8275 PTP profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275StreamTimestampIfRateGet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_PtpG8275TimestampIfRateTlvS *tsIfRateS)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Check the input pointer. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tsIfRateS);
   }

   /* Get the associated profile stream data structure.
    * (An error is returned if the stream is NULL). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGetG8275(streamHandle, &pPtpStream);
   }

   /* Lock the Clock Mutex to retrieve the structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpStream->portData->clock);
   }

   /* Update the timestamp interface attributes for the Port. */
   if (status == ZL303XX_OK)
   {
      *tsIfRateS = pPtpStream->profileDS.g8275.tsIfRate;
      status = zl303xx_PtpClockMutexUnlock(pPtpStream->portData->clock, status);
   }

   return status;
}  /* END zl303xx_PtpG8275StreamTimestampIfRateGet */

/*****************   UTILITY FUNCTION DEFINITIONS   ***************************/

/* zl303xx_PtpG8275p1ClockStateToStr */
/**

   Converts a G8275.1 Clock State ENUM value to a string.

  Parameters:
   [in]  clkState    G8275.1 Clock State to convert.

  Return Value:  (const char *)  The G8275.1 Clock State as a string.

*******************************************************************************/
const char *zl303xx_PtpG8275p1ClockStateToStr(zl303xx_PtpG8275p1ClockStateE clkState)
{
   /* If the state is not valid, use the last index of the array. */
   if (clkState >= ZL303XX_PTP_G8275p1_CLOCK_STATE_UNKNOWN)
   {
      clkState = ZL303XX_PTP_G8275p1_CLOCK_STATE_UNKNOWN;
   }

   return zl303xx_PtpG8275p1ClockStateStr[clkState];
}

/* zl303xx_PtpG8275p1ClockClassDescToStr */
/**

   Converts a G8275.1 Clock Class Description ENUM value to a string.

  Parameters:
   [in]  clkClassDescr    G8275.1 Clock Class Description to convert.

  Return Value:  (const char *)  The G8275.1 Clock Class Description as a string.

*******************************************************************************/
const char *zl303xx_PtpG8275p1ClockClassDescToStr(
      zl303xx_PtpG8275p1ClockClassDescE clkClassDescr)
{
   /* If the state is not valid, use the last index of the array. */
   if (clkClassDescr >= ZL303XX_PTP_G8275p1_CLOCK_CLASS_DESC_UNKNOWN)
   {
      clkClassDescr = ZL303XX_PTP_G8275p1_CLOCK_CLASS_DESC_UNKNOWN;
   }

   return zl303xx_PtpG8275p1ClockClassDescStr[clkClassDescr];
}


/* zl303xx_PtpG8275p1ClassEvalMethodToStr */
/**

   Converts a G8275.1 Clock Class Evaluation Method ENUM value to a string.

  Parameters:
   [in]  clkState    G8275.1 Clock Class Evaluation Method.

  Return Value:  (const char *)  The G8275.1 Clock Class Evaluation Method as a string.

*******************************************************************************/
const char *zl303xx_PtpG8275p1ClassEvalMethodToStr(
      zl303xx_PtpG8275p1ClassEvalMethodE classEvalMethod)
{
   /* If the state is not valid, use the last index of the array. */
   if (classEvalMethod > ZL303XX_PTP_G8275p1_CLASS_EVAL_METHOD3)
   {
      classEvalMethod = ZL303XX_PTP_G8275p1_CLASS_EVAL_METHOD3 + 1;
   }

   return zl303xx_PtpG8275p1ClockClassEvalMethodStr[classEvalMethod];
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/*

  Function Name:
    zl303xx_PtpG8275CommonClockDefaults

  Details:
   Routine to set common G.8275 (TelecomPhase Profile) clock parameters to
   default values. This routine is called first to initialize parameters that
   are common across all G.8275 versions and/or editions. Any settings specific
   to a version is then set in each individual initialization routine.

  Parameters:
   [in]  pClockCreateS  Pointer to the PTP creation data structure for the
                              clock (from the PtpApiTypes.h module).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275CommonClockDefaults(zl303xx_PtpClockCreateS *pClockCreateS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pClockCreateS);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      /* Common PTP Clock Attributes. */
      /********************************/
      /* priority1: static and set to 128 for all clocks with this profile. */
      pClockCreateS->defaultDS.priority1 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;

      /* Assume UNLOCKED condition for all Clock types. */
      pClockCreateS->defaultDS.clockQuality.clockAccuracy = ZL303XX_PTP_G8275p1_ACCURACY_UNLOCK;
      pClockCreateS->defaultDS.clockQuality.offsetScaledLogVariance = ZL303XX_PTP_G8275p1_VARIANCE_UNLOCK;

      /* Set defaults based on clockType */
      /* SLAVE */
      if (pClockCreateS->clockType == ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_TSC)
      {
         pClockCreateS->defaultDS.slaveOnly = ZL303XX_TRUE;
         pClockCreateS->defaultDS.priority2 = ZL303XX_PTP_G8275p1_CLOCK_PRI2_SLAVE;
         pClockCreateS->defaultDS.clockQuality.clockClass = ZL303XX_PTP_G8275p1_CLASS_SLAVE;
      }

      /* GM or BC*/
      else if (pClockCreateS->clockType == ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_GM)
      {
         pClockCreateS->defaultDS.slaveOnly = ZL303XX_FALSE;
         pClockCreateS->defaultDS.priority2 = ZL303XX_PTP_G8275p1_CLOCK_PRI2_MASTER;
         pClockCreateS->defaultDS.clockQuality.clockClass = ZL303XX_PTP_G8275p1_CLASS_FREERUN;
      }

      /* Synchronization Uncertain */
      pClockCreateS->synchronizationUncertain.enabled = ZL303XX_TRUE;

      /* domainNumber */
      /* Needs to be set by individual profile Version routines. */

      /* Telecom Phase Profile Clock Attributes. */
      /*******************************************/
      /* bypassEnabled */
      pClockCreateS->profileCfg.g8275p1.bypassEnabled = ZL303XX_FALSE;
      /* localPriority */
      pClockCreateS->profileCfg.g8275p1.localPriority = ZL303XX_PTP_G8275_LOCAL_PRI_DEF;
      /* classEvalMethod */
      pClockCreateS->profileCfg.g8275p1.classEvalMethod = ZL303XX_PTP_G8275p1_CLASS_EVAL_DEFAULT;
      /* equipmentClass */
      pClockCreateS->profileCfg.g8275p1.equipmentClass = ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_C2_ALT;
      /* holdoverSupported */
      pClockCreateS->profileCfg.g8275p1.holdoverSupported = ZL303XX_FALSE;

      /* Advertise the PREVIOUS class while in the UNCALIBRATED state.  */
      /* Refer to the Note in ITU G.8275.1: Appendix V, Section 1. */
      pClockCreateS->uncalibrated.usePreviousClass = ZL303XX_TRUE;
   }

   return status;
}  /* END zl303xx_PtpG8275CommonClockDefaults */

/*

  Function Name:
    zl303xx_PtpG8275CommonPortDefaults

  Details:
   Routine to set common G.8275 (TelecomPhase Profile) port parameters to
   default values. This routine is called first to initialize parameters that
   are common across all G.8275 versions and/or editions. Any settings specific
   to a version is then set in each individual initialization routine.

  Parameters:
   [in]  pPortCreateS   Pointer to the PTP creation data structure for the
                              port (from the PtpApiTypes.h module).

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   profileInfo is NULL.
     ZL303XX_INVALID_MODE      The associated clock is not running the
                                    G.8275.1 PTP profile.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275CommonPortDefaults(zl303xx_PtpPortCreateS *pPortCreateS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pPortCreateS);

   /* Several parameters in the Port set their default based on the parent
    * Clock's TYPE (GM, BC, TSC, etc). */
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      /* Attempt to get the parent Clock structure and determine if this is a
       * GM, BC or Slave. (An error is returned if pPtpClock is NULL). */
      /* Not getting a parent clock is not necessarily an error (yet). However,
       * some assumptions will be made for some parameters. */
      if (ZL303XX_OK != zl303xx_PtpClockDataGetG8275(pPortCreateS->clockHandle, &pPtpClock))
      {
         pPtpClock = NULL;
      }

      /* Common PTP Port Attributes. */
      /*******************************/
      /* delayMechanism */
      pPortCreateS->delayMechanism = ZL303XX_DELAY_MECHANISM_E2E;


      /* Phase Profile Port Attributes. */
      /**********************************/
      /* localPriority */
      pPortCreateS->profileCfg.g8275p1.localPriority = ZL303XX_PTP_G8275_LOCAL_PRI_DEF;

      /* tsIfRate - Clear all attributes */
      OS_MEMSET(&pPortCreateS->profileCfg.g8275p1.tsIfRate, 0x00,
             sizeof(pPortCreateS->profileCfg.g8275p1.tsIfRate));

      /* denyServiceRequests */
      if ((pPtpClock != NULL) &&
          (pPtpClock->config.clockType == ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_TSC))
      {
         /* Disable GRANTs on Slave clocks. */
         pPortCreateS->unicastNegotiation.denyServiceRequests = ZL303XX_TRUE;
      }
      else
      {
         /* Disable GRANTs on Slave clocks. */
         pPortCreateS->unicastNegotiation.denyServiceRequests = ZL303XX_FALSE;
      }

      /* masterOnly */
      if ((pPtpClock != NULL) &&
          (pPtpClock->config.clockType == ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_GM))
      {
         pPortCreateS->masterOnly = ZL303XX_TRUE;
      }
      else
      {
         /* Assume this is not a GM... */
         pPortCreateS->masterOnly = ZL303XX_FALSE;
      }

      /* Update any DEPRECATED fields. */
      pPortCreateS->unicastNegotiation.grantDefault =
         !(pPortCreateS->unicastNegotiation.denyServiceRequests);
      pPortCreateS->profileCfg.g8275p1.notSlave = pPortCreateS->masterOnly;
   }

   return ZL303XX_OK;
}  /* END zl303xx_PtpG8275CommonPortDefaults */

/*

  Function Name:
    zl303xx_PtpG8275CommonStreamDefaults

  Details:
   Routine to set common G.8275 (TelecomPhase Profile) stream parameters to
   default values. This routine is called first to initialize parameters that
   are common across all G.8275 versions and/or editions. Any settings specific
   to a version is then set in each individual initialization routine.

  Parameters:
   [in]  pStreamCreateS Pointer to the PTP creation data structure for the
                              stream (from the PtpApiTypes.h module).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275CommonStreamDefaults(zl303xx_PtpStreamCreateS *pStreamCreateS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pStreamCreateS);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      /* Do not allow the PTP stream to enter SLAVE state until the Timing
       * Algorithm has achieved the LOCKED state.   */
      pStreamCreateS->uncalibrated.lockRequired = ZL303XX_TRUE;

      /* Enable Bi-Directional monitoring for all G.8275 Unicast negotiated
       * configurations.   */
      pStreamCreateS->unicastMonitorTiming = ZL303XX_TRUE;

      /* Phase Profile Stream Attributes. */
      /************************************/
      /* masterOnly */
      /* Set the stream default to the same value as the parent Port for now. */
      {
         /* Get the associated parent Port data structure.
          * (An error is returned if the port is NULL). */
         zl303xx_PortDataS *pPtpPort = NULL;
         if (ZL303XX_OK == zl303xx_PtpPortDataGetG8275(pStreamCreateS->portHandle, &pPtpPort))
         {
            pStreamCreateS->mode =
               ZL303XX_PTP_G8275_MASTER_ONLY_AS_STREAM_MODE(pPtpPort->config.masterOnly);
         }
         else
         {
            /* This is not necessarily an error so return OK. */
            /* Set a default for now... */
            pStreamCreateS->mode = ZL303XX_PTP_STREAM_USE_BMC;
         }
      }  /* masterOnly determination */

      /* Update this DEPRECATED field. */
      pStreamCreateS->profileCfg.g8275p1.notSlave =
         ZL303XX_PTP_G8275_STREAM_MODE_AS_MASTER_ONLY(pStreamCreateS->mode);

      /* localPriority */
      /* By default, use the Port level localPriority value. This will force
       * the user to turn on this level explicitly.  */
      pStreamCreateS->profileCfg.g8275p1.localPriority = ZL303XX_PTP_G8275_LOCAL_PRI_DISABLED;
   }

   return status;
}  /* END zl303xx_PtpG8275CommonStreamDefaults */

/* zl303xx_PtpClockDataGetG8275 */
/**
   Telecom Profile for Phase only. Retrieves the specified Clock Object and
   verifies that any of the G.8275 profiles are configured.

  Parameters:
   [in]  clockHandle    Clock handle of an existing Clock

   [out] pPtpClock      Pointer to an existing clock data set running a
                              G.8275 profile.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    clockHandle is invalid.
     ZL303XX_INVALID_MODE         The profile is not running on the clock.

*******************************************************************************/
zlStatusE zl303xx_PtpClockDataGetG8275(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpClockS **ppClock)
{
   zlStatusE status = ZL303XX_OK;

   /* First, see if the clock even exists. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGet(clockHandle, ppClock);
   }

   /* Check the Profile value. */
   if (status == ZL303XX_OK)
   {
      if (((*ppClock)->config.profile != ZL303XX_PTP_PROFILE_TELECOM_G8275_1v1) &&
          ((*ppClock)->config.profile != ZL303XX_PTP_PROFILE_TELECOM_G8275_1v2) &&
          ((*ppClock)->config.profile != ZL303XX_PTP_PROFILE_TELECOM_G8275_2))
      {
         ZL303XX_TRACE_ALWAYS("zl303xx_PtpClockDataGetG8275: Clock(%u) Profile(%u)(any 8275 expected)",
               clockHandle, (*ppClock)->config.profile, 0,0,0,0);
         status = ZL303XX_INVALID_MODE;
      }
   }

   return status;
}  /* END zl303xx_PtpClockDataGetG8275 */

/* zl303xx_PtpPortDataGetG8275 */
/**
   Telecom Profile for Phase only. Retrieves the specified Port Object and
   verifies that any of the G.8275 profiles are configured.

  Parameters:
   [in]  portHandle  Port handle of an existing Port

   [out] ppPort      Pointer to an existing port data set running a
                           G.8275 profile.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    portHandle is invalid.
     ZL303XX_INVALID_MODE         The profile is not running on the port.

*******************************************************************************/
zlStatusE zl303xx_PtpPortDataGetG8275(
      zl303xx_PtpPortHandleT portHandle,
      zl303xx_PortDataS **ppPort)
{
   zlStatusE status = ZL303XX_OK;

   /* First, see if the port even exists. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, ppPort);
   }

   /* Check the Profile value. */
   if (status == ZL303XX_OK)
   {
      if (((*ppPort)->clock->config.profile != ZL303XX_PTP_PROFILE_TELECOM_G8275_1v1) &&
          ((*ppPort)->clock->config.profile != ZL303XX_PTP_PROFILE_TELECOM_G8275_1v2) &&
          ((*ppPort)->clock->config.profile != ZL303XX_PTP_PROFILE_TELECOM_G8275_2))
      {
         ZL303XX_TRACE_ALWAYS("zl303xx_PtpPortDataGetG8275: Port(%u) Profile(%u)(any 8275 expected)",
               portHandle, (*ppPort)->clock->config.profile, 0,0,0,0);
         status = ZL303XX_INVALID_MODE;
      }
   }

   return status;
}  /* END zl303xx_PtpPortDataGetG8275 */

/* zl303xx_PtpStreamDataGetG8275 */
/**
   Telecom Profile for Phase only. Retrieves the specified Stream Object and
   verifies that any of the G.8275 profiles are configured.

  Parameters:
   [in]  streamHandle   Stream handle of an existing Stream

   [out] ppStream       Pointer to an existing stream data set running a
                              G.8275 profile.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    streamHandle is invalid.
     ZL303XX_INVALID_MODE         The profile is not running on the stream.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamDataGetG8275(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_ClockStreamS **ppStream)
{
   zlStatusE status = ZL303XX_OK;

   /* First, see if the stream even exists. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, ppStream);
   }

   /* Check the Profile value. */
   if (status == ZL303XX_OK)
   {
      if (((*ppStream)->portData->clock->config.profile != ZL303XX_PTP_PROFILE_TELECOM_G8275_1v1) &&
          ((*ppStream)->portData->clock->config.profile != ZL303XX_PTP_PROFILE_TELECOM_G8275_1v2) &&
          ((*ppStream)->portData->clock->config.profile != ZL303XX_PTP_PROFILE_TELECOM_G8275_2))
      {
         ZL303XX_TRACE_ALWAYS("zl303xx_PtpStreamDataGetG8275: Stream(%u) Profile(%u)(any 8275 expected)",
               streamHandle, (*ppStream)->portData->clock->config.profile, 0,0,0,0);
         status = ZL303XX_INVALID_MODE;
      }
   }

   return status;
}  /* END zl303xx_PtpStreamDataGetG8275 */

/* zl303xx_PtpG8275p1ClockStateEvaluate */
/**
   Telecom Profile for Phase only. Evaluates a clock's state based on the
   combination of local stream states.

  Parameters:
   [in]  pPtpClock         Pointer to an existing clock.
   [in]  pPtpVirtualPort   Pointer to the virtual port selected by the clock.
                                 (May be NULL if no VP selected).
   [in]  pPllPerfData      PLL performance data (may be required to make the
                                 clock state decision).

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      g8275p1State is NULL.
     ZL303XX_PARAMETER_INVALID    clockHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275p1ClockStateEvaluate(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PortDataS *pPtpVirtualPort,
      zl303xx_PtpPllPerformanceDataS *pPllPerfData)
{
   zlStatusE status = ZL303XX_OK;

   /* Set a default Return Value: . */
   zl303xx_PtpG8275p1ClockStateE g8275p1State = ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED;

   /* Eliminate warning (consider removing). */
   if (pPtpVirtualPort) {;}

   /* Evaluate the Clock State. */
   if (status == ZL303XX_OK)
   {
      /* If BYPASS mode is on then ignore all Port state and PLL Performance data. */
      if (pPtpClock->config.profileCfg.g8275p1.bypassEnabled == ZL303XX_TRUE)
      {
         g8275p1State = ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED;

         /* Set all performance values to match the fake LOCK condition. */
         /* Ignore the MODE. */
         pPllPerfData->hwPllOperatingState = ZL303XX_PTP_PLL_STATE_PHASE_ACQUIRED;
         pPllPerfData->swPllOperatingState = ZL303XX_PTP_PLL_STATE_PHASE_ACQUIRED;
         pPllPerfData->pllHoldoverQuality = ZL303XX_PTP_PLL_HOLDOVER_QUALITY_CAT3;
         pPllPerfData->bHoldoverInSpec = ZL303XX_TRUE;
         pPllPerfData->synceTraceable = ZL303XX_TRUE;
      }

      /* For any ClockType, if in electrical mode, evaluate based on the
       * Electrical PLL performance. */
      else if (pPllPerfData->pllOperatingMode == ZL303XX_PTP_PLL_MODE_ELEC_REF_SYNC)
      {
         g8275p1State = zl303xx_PtpG8275p1ClockStateElecPhase(pPllPerfData);
      }

      /* For any ClockType, if in Hybrid mode, evaluate the Frequency portion
       * based on the Electrical PLL performance but use the Packet performance
       * for the overall Phase performance. */
      else if (pPllPerfData->pllOperatingMode == ZL303XX_PTP_PLL_MODE_HYBRID_SYNCE)
      {
         /* Evaluate the Electrical Frequency portion first. */
         g8275p1State = zl303xx_PtpG8275p1ClockStateElecFreq(pPllPerfData);

         /* If the Electrical Frequency is LOCKED, then check the Packet-Phase
          * status, otherwise just continue until the Electrical Frequency
          * portion is complete. */
         if (g8275p1State == ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED)
         {
            /* The Electrical-Frequency Portion is complete so evaluate the
             * Packet-Phase portion.    */
            g8275p1State = zl303xx_PtpG8275p1ClockStatePktPhase(pPtpClock, pPllPerfData);
         }
         /* ELSE:
          * The Electrical portion is ACQUIRING or less so report its State. */
      }

      /* For a PURE-PACKET mode, just evaluate the PTP states. */
      else
      {
         g8275p1State = zl303xx_PtpG8275p1ClockStatePktPhase(pPtpClock, pPllPerfData);
      }
   }

   /* Update the dynamic data values. */
   pPtpClock->profileDS.g8275.clockState = g8275p1State;

   /* Update the Clock Class Description as well (info only). */
   (void)zl303xx_PtpG8275p1ClockStateDescrSet(pPtpClock, g8275p1State);

   return status;
}  /* END zl303xx_PtpG8275p1ClockStateEvaluate */


/* zl303xx_PtpG8275p1ClockClassEvaluate */
/**
   Telecom Profile for Phase only. Determines a clock's advertised class and
   frequency Traceable flag based on certain performance parameters of the local
   hardware PLL or software algorithm.

   Several methods for doing this evaluation exists. Refer to the
   zl303xx_PtpG8275p1ClassEvalMethodE enumeration for descriptions.

   This routine requires:
   - Interface for the zl303xx_PtpPllPerformanceDataS parameters.
   - Proper mapping of PLL performance metrics to the members of the structure.
     Refer to the ZL303XX_PTP_HW_CMD_PLL_PERF_DATA_GET interface command.

  Parameters:
   [in]  pPtpClock         Pointer to an existing clock.
   [in]  pPtpVirtualPort   Pointer to the virtual port selected by the clock.
                                 (May be NULL if no VP selected).
   [in]  pPllPerfData      PLL performance data  required to make the class
                                 and frequency traceable decision.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      pPtpClock is NULL.
     ZL303XX_EXT_API_CALL_FAIL    Hardware interface command failed.

*******************************************************************************/
zlStatusE zl303xx_PtpG8275p1ClockClassEvaluate(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PortDataS *pPtpVirtualPort,
      zl303xx_PtpPllPerformanceDataS *pPllPerfData)
{
   zlStatusE status = ZL303XX_OK;

   /* Evaluate Class and Frequency Traceability based on HW/SW PLL status. */
   if (status == ZL303XX_OK)
   {
      if (pPtpClock->config.profileCfg.g8275p1.classEvalMethod == ZL303XX_PTP_G8275p1_CLASS_EVAL_DEFAULT)
      {
         (void)zl303xx_PtpG8275p1ClockClassEvaluate_Method1(pPtpClock, pPtpVirtualPort, pPllPerfData,
               &pPtpClock->profileDS.g8275.pllClockClass);
      }
      else if (pPtpClock->config.profileCfg.g8275p1.classEvalMethod == ZL303XX_PTP_G8275p1_CLASS_EVAL_METHOD2)
      {
         (void)zl303xx_PtpG8275p1ClockClassEvaluate_Method2(pPtpClock, pPtpVirtualPort, pPllPerfData,
               &pPtpClock->profileDS.g8275.pllClockClass);
      }
      else if (pPtpClock->config.profileCfg.g8275p1.classEvalMethod == ZL303XX_PTP_G8275p1_CLASS_EVAL_METHOD3)
      {
         (void)zl303xx_PtpG8275p1ClockClassEvaluate_Method3(pPtpClock, pPtpVirtualPort, pPllPerfData,
               &pPtpClock->profileDS.g8275.pllClockClass);
      }
      else
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Some value of clockClass and frequency traceability must be assigned. As
    * long as the clock pointer is valid use parentDS information.  */
   if (status != ZL303XX_OK)
   {
      pPtpClock->profileDS.g8275.pllClockClass = pPtpClock->parentDS.grandmasterClockQuality.clockClass;
   }

   return status;
}  /* END zl303xx_PtpG8275p1ClockClassEvaluate */

/* zl303xx_PtpG8275p1ClockClassEvaluate_Method1 */
/**
   Telecom Profile for Phase only. Determines a clock's advertised class and
   frequency Traceable flag based on G.8275.1 - Table 6.2.

   Refer to zl303xx_PtpPllPerformanceDataS and zl303xx_PtpG8275p1ClassEvalMethodE.

  Parameters:
   [in]  pPtpClock         Pointer to an existing clock.
   [in]  pPtpVirtualPort   Pointer to the virtual port selected by the clock.
                                 (May be NULL if no VP selected).
   [in]  pllPerfData       PLL performance data required to make the evaluation.

   [out]  pOutClockClass   The evaluated clock Class value.

  Return Value:  ZL303XX_OK                   Success.

*******************************************************************************/
void zl303xx_PtpG8275p1ClockClassEvaluate_Method1(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PortDataS *pPtpVirtualPort,
      zl303xx_PtpPllPerformanceDataS *pllPerfData,
      Uint8T *pOutClockClass)
{
   /* Evaluate for configured GM and PRTC Virtual Port. */
   if ((pPtpClock->config.clockType == ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_GM) ||
       (zl303xx_PtpG8275p1VirtualPortIsGM(pPtpVirtualPort, pllPerfData)) ||

       /* In Holdover mode from a previous PRTC Virtual Port */
       ((pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_LOCAL_CLOCK) &&
        (pPtpClock->profileDS.g8275.prtcHoldover == ZL303XX_TRUE)))
   {
      zl303xx_PtpG8275p1ClockClassEvalGM_Method1(pPtpClock, pllPerfData, pOutClockClass);
   }

   /* If a T-TSC that does not support HOLDOVER then force the class. */
   else if ((pPtpClock->config.clockType == ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_TSC) &&
            (pPtpClock->config.profileCfg.g8275p1.holdoverSupported == ZL303XX_FALSE))
   {
      *pOutClockClass = ZL303XX_PTP_G8275p1_CLASS_SLAVE;
   }
   else
   {
      zl303xx_PtpG8275p1ClockClassEvalBC_Method1(pPtpClock, pllPerfData, pOutClockClass);
   }
}  /* END zl303xx_PtpG8275p1ClockClassEvaluate_Method1 */


/* zl303xx_PtpG8275p1ClockClassEvaluate_Method2 */
/**
   Telecom Profile for Phase only. Determines a clock's advertised class and
   frequency Traceable flag based on G.8275.1 - Table 6.4.

   Refer to zl303xx_PtpPllPerformanceDataS and zl303xx_PtpG8275p1ClassEvalMethodE.

  Parameters:
   [in]  pPtpClock         Pointer to an existing clock.
   [in]  pPtpVirtualPort   Pointer to the virtual port selected by the clock.
                                 (May be NULL if no VP selected).
   [in]  pllPerfData       PLL performance data required to make the evaluation.

   [out]  pOutClockClass       The evaluated clock Class value.

  Return Value:  ZL303XX_OK                   Success.

*******************************************************************************/
void zl303xx_PtpG8275p1ClockClassEvaluate_Method2(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PortDataS *pPtpVirtualPort,
      zl303xx_PtpPllPerformanceDataS *pllPerfData,
      Uint8T *pOutClockClass)
{
   /* First, evaluate the Default Method. */
   (void)zl303xx_PtpG8275p1ClockClassEvaluate_Method1(pPtpClock, pPtpVirtualPort, pllPerfData, pOutClockClass);

   /* Replace just the affected values. */
   if (*pOutClockClass == ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT1)
      *pOutClockClass = ZL303XX_PTP_G8275p1_GM_CLASS_IN_HOLDOVER;
   else if (*pOutClockClass == ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT2)
      *pOutClockClass = pPtpClock->config.profileCfg.g8275p1.equipmentClass;
   else if (*pOutClockClass == ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT3)
      *pOutClockClass = ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_C3_ALT;
   else if (*pOutClockClass == ZL303XX_PTP_G8275p1_BC_CLASS_OUT_HOLDOVER)
      *pOutClockClass = ZL303XX_PTP_G8275p1_BC_CLASS_OUT_HOLDOVER_ALT;

   /* Replace just the affected values. */
   if (pPtpClock->profileDS.g8275.fallbackClass == ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT1)
      pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_GM_CLASS_IN_HOLDOVER;
   else if (pPtpClock->profileDS.g8275.fallbackClass == ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT2)
      pPtpClock->profileDS.g8275.fallbackClass = pPtpClock->config.profileCfg.g8275p1.equipmentClass;
   else if (pPtpClock->profileDS.g8275.fallbackClass == ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT3)
      pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_C3_ALT;
   else if (pPtpClock->profileDS.g8275.fallbackClass == ZL303XX_PTP_G8275p1_BC_CLASS_OUT_HOLDOVER)
      pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_BC_CLASS_OUT_HOLDOVER_ALT;

}  /* END zl303xx_PtpG8275p1ClockClassEvaluate_Method2 */

/* zl303xx_PtpG8275p1ClockClassEvaluate_Method3 */
/**
   Telecom Profile for Phase only. Determines a clock's advertised class and
   frequency Traceable flag based on G.8275.1 - CM

   Refer to zl303xx_PtpPllPerformanceDataS and zl303xx_PtpG8275p1ClassEvalMethodE.

  Parameters:
   [in]  pPtpClock         Pointer to an existing clock.
   [in]  pPtpVirtualPort   Pointer to the virtual port selected by the clock.
                                 (May be NULL if no VP selected).
   [in]  pllPerfData       PLL performance data required to make the evaluation.

   [out]  pOutClockClass   The evaluated clock Class value.

  Return Value:  ZL303XX_OK                   Success.

*******************************************************************************/
void zl303xx_PtpG8275p1ClockClassEvaluate_Method3(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PortDataS *pPtpVirtualPort,
      zl303xx_PtpPllPerformanceDataS *pllPerfData,
      Uint8T *pOutClockClass)
{
   /* First, evaluate the Method 1 to get most values. */
   (void)zl303xx_PtpG8275p1ClockClassEvaluate_Method1(pPtpClock, pPtpVirtualPort, pllPerfData, pOutClockClass);

   /* Replace just the affected value. */
   if (*pOutClockClass == ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT1)
      *pOutClockClass = ZL303XX_PTP_G8275p1_GM_CLASS_IN_HOLDOVER;
   else if (*pOutClockClass == ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT2)
      *pOutClockClass = ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_C2_ALT;
   else if (*pOutClockClass == ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT3)
      *pOutClockClass = ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_C3_ALT;
   else if (*pOutClockClass == ZL303XX_PTP_G8275p1_BC_CLASS_OUT_HOLDOVER)
      *pOutClockClass = ZL303XX_PTP_G8275p1_BC_CLASS_OUT_HOLDOVER_ALT;

   /* Replace just the affected values. */
   if (pPtpClock->profileDS.g8275.fallbackClass == ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT1)
      pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_GM_CLASS_IN_HOLDOVER;
   else if (pPtpClock->profileDS.g8275.fallbackClass == ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT2)
      pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_C2_ALT;
   else if (pPtpClock->profileDS.g8275.fallbackClass == ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT3)
      pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_C3_ALT;
   else if (pPtpClock->profileDS.g8275.fallbackClass == ZL303XX_PTP_G8275p1_BC_CLASS_OUT_HOLDOVER)
      pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_BC_CLASS_OUT_HOLDOVER_ALT;

}  /* END zl303xx_PtpG8275p1ClockClassEvaluate_Method3 */

/* zl303xx_PtpG8275p1FreqTraceableEvaluate */
/**
   Telecom Profile for Phase only. Determines the egress value of the clock's
   advertised frequencyTraceable flag based on certain performance parameters
   of the local hardware PLL or software algorithm.

   This routine requires:
   - Interface for the zl303xx_PtpPllPerformanceDataS parameters.
   - Proper mapping of PLL performance metrics to the members of the structure.
     Refer to the ZL303XX_PTP_HW_CMD_PLL_PERF_DATA_GET interface command.

  Parameters:
   [in]  pPtpClock         Pointer to an existing clock.
   [in]  pPllPerfData      PLL performance data  required to make the class
                                 and frequency traceable decision.

  Return Value:  ZL303XX_OK                   Success.

  Notes:    Assumes the Clock & Data pointers are valid for this internal routine.
*******************************************************************************/
zlStatusE zl303xx_PtpG8275p1FreqTraceableEvaluate(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpPllPerformanceDataS *pPllPerfData)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      /* If the clock is bypassing the G.8275.1 performance data and hardware
       * interface, then get the frequencyTraceable flag straight from the
       * parentDS.   */
      if (pPtpClock->config.profileCfg.g8275p1.bypassEnabled == ZL303XX_TRUE)
      {
         pPtpClock->profileDS.g8275.pllFrequencyTraceable = pPtpClock->timePropertiesDS.frequencyTraceable;
      }

      /* For any ClockType, if in electrical mode, the flag is derived from the
       * PLL Lock status and existence of a Category1 SyncE. */
      else if (pPllPerfData->pllOperatingMode == ZL303XX_PTP_PLL_MODE_ELEC_REF_SYNC)
      {
         /* Must be at least Frequency Locked with CAT1 SyncE */
         if (((pPllPerfData->hwPllOperatingState == ZL303XX_PTP_PLL_STATE_PHASE_ACQUIRED) ||
               (pPllPerfData->hwPllOperatingState == ZL303XX_PTP_PLL_STATE_FREQ_ACQUIRED))
             &&
             (pPllPerfData->synceTraceable) &&
             (pPllPerfData->pllHoldoverQuality == ZL303XX_PTP_PLL_HOLDOVER_QUALITY_CAT1))
         {
            pPtpClock->profileDS.g8275.pllFrequencyTraceable = ZL303XX_TRUE;
         }
         else
         {
            pPtpClock->profileDS.g8275.pllFrequencyTraceable = ZL303XX_FALSE;
         }
      }

      /* A Pure-Packet Mode or Hybrid clock:
       * Requires Frequency Lock to advertise the ParentDS value. */
      else
      {
         if ((pPllPerfData->swPllOperatingState == ZL303XX_PTP_PLL_STATE_PHASE_ACQUIRED) ||
             (pPllPerfData->swPllOperatingState == ZL303XX_PTP_PLL_STATE_FREQ_ACQUIRED))
         {
            pPtpClock->profileDS.g8275.pllFrequencyTraceable = pPtpClock->timePropertiesDS.frequencyTraceable;
         }
         else
         {
            pPtpClock->profileDS.g8275.pllFrequencyTraceable = ZL303XX_FALSE;
         }
      }
   }

   return status;
}  /* END zl303xx_PtpG8275p1FreqTraceableEvaluate */

/* zl303xx_PtpG8275p1VirtualPortIsGM */
/**
   Telecom Profile for Phase only. Determines if the Virtual Port specified
   meets the criteria of a GM source (see conditions in the routine).

   Refer to zl303xx_PtpPllPerformanceDataS and zl303xx_PtpG8275p1ClassEvalMethodE.

  Parameters:
   [in]  pPtpVirtualPort   Pointer to the virtual port selected by the clock.
                                 (May be NULL if no VP selected).
   [in]  pllPerfData       PLL performance data required to make the evaluation.

  Return Value:
     ZL303XX_TRUE     A VP is specified and meets all criteria.
     ZL303XX_FALSE    Otherwise.

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtpG8275p1VirtualPortIsGM(
      zl303xx_PortDataS *pPtpVirtualPort,
      zl303xx_PtpPllPerformanceDataS *pllPerfData)
{
   if ((pPtpVirtualPort != NULL) &&                                              /* Virtual Port Selected. */
       (pPtpVirtualPort->virtualData.isVirtualPort == ZL303XX_TRUE) &&             /* Is in VP mode */
       (pPtpVirtualPort->virtualData.config.prtcConnected) &&                    /* PRTC Traceable */
       (pPtpVirtualPort->virtualData.config.ptsf == ZL303XX_FALSE) &&              /* Is Enabled (not Failed) */

       ((pllPerfData->pllOperatingMode == ZL303XX_PTP_PLL_MODE_ELEC_REF_SYNC) ||   /* PLL in Electrical OR ... */
         (pllPerfData->pllOperatingMode == ZL303XX_PTP_PLL_MODE_HYBRID_SYNCE)))    /* ... Hybrid Mode */

   {
      return ZL303XX_TRUE;
   }
   else
   {
      return ZL303XX_FALSE;
   }
}

/* zl303xx_PtpG8275p1ClockClassEvalGM_Method1 */
/**
   Telecom Profile for Phase only. Determines a clock's advertised class and
   frequency Traceable flag based on G.8275.1 - Table 6.2.

   Refer to zl303xx_PtpPllPerformanceDataS and zl303xx_PtpG8275p1ClassEvalMethodE.

  Parameters:
   [in]  pPtpClock         Pointer to an existing clock.
   [in]  pllPerfData       PLL performance data required to make the evaluation.

   [out]  pOutClockClass       The evaluated clock Class value.

  Return Value:  ZL303XX_OK                   Success.

*******************************************************************************/
void zl303xx_PtpG8275p1ClockClassEvalGM_Method1(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpPllPerformanceDataS *pllPerfData,
      Uint8T *pOutClockClass)
{
   /* For Electric or HYBRID mode, use the Hardware PLL Status to evaluate the class. */
   if ((pllPerfData->pllOperatingMode == ZL303XX_PTP_PLL_MODE_ELEC_REF_SYNC) ||
       (pllPerfData->pllOperatingMode == ZL303XX_PTP_PLL_MODE_HYBRID_SYNCE))
   {
      /* If PHASE LOCK then use the ParentDS values. */
      if ((pllPerfData->hwPllOperatingState == ZL303XX_PTP_PLL_STATE_PHASE_ACQUIRED) ||
          (pllPerfData->settlingTimeActive == ZL303XX_TRUE))
      {
         *pOutClockClass = pPtpClock->parentDS.grandmasterClockQuality.clockClass;
      }

      /* HOLDOVER or ACQUIRING Conditions (FREQ_ACQUIRED means NOT PHASE_LOCKED) */
      else if ((pllPerfData->hwPllOperatingState == ZL303XX_PTP_PLL_STATE_FREQ_ACQUIRED)   ||
               (pllPerfData->hwPllOperatingState == ZL303XX_PTP_PLL_STATE_HOLDOVER)        ||
               (pllPerfData->hwPllOperatingState == ZL303XX_PTP_PLL_STATE_REF_FAILED)      ||
               (pllPerfData->hwPllOperatingState == ZL303XX_PTP_PLL_STATE_FREQ_ACQUIRING)  ||
               (pllPerfData->hwPllOperatingState == ZL303XX_PTP_PLL_STATE_PHASE_ACQUIRING))
      {
         /* Default frequencyTraceable setting (above) applies whether IN or OUT of
          * HOLDOVER Specification. */

         /* Within Holdover SPEC. */
         if (pllPerfData->bHoldoverInSpec == ZL303XX_TRUE)
         {
            *pOutClockClass = ZL303XX_PTP_G8275p1_GM_CLASS_IN_HOLDOVER;
         }

         else  /* Out of Spec */
         {
            if (pllPerfData->pllHoldoverQuality == ZL303XX_PTP_PLL_HOLDOVER_QUALITY_CAT1)
            {
               *pOutClockClass = CLASS_GM_OUT_HLD_CAT1[pPtpClock->config.profileCfg.g8275p1.classEvalMethod];
            }
            else if (pllPerfData->pllHoldoverQuality == ZL303XX_PTP_PLL_HOLDOVER_QUALITY_CAT2)
            {
               *pOutClockClass = CLASS_GM_OUT_HLD_CAT2[pPtpClock->config.profileCfg.g8275p1.classEvalMethod];
            }
            else
            {
               *pOutClockClass = CLASS_GM_OUT_HLD_CAT3[pPtpClock->config.profileCfg.g8275p1.classEvalMethod];
            }
         }

         /* No matter what, the only way to improve our class while in these
          * states is if we are FREERUN but TOD is set.
          * For example suppose we were HLD-OUT-SPEC clock on the last cycle but
          * the IN-SPEC Flag is good, we cannot go to class 7; that is only set
          * once we achieve LOCK.      */
         if (*pOutClockClass < pPtpClock->profileDS.g8275.fallbackClass)
         {
            /* Trying to push our class to a better level. Only do so if:
             * - current fallback is FREERUN.
             * - TOD is set.
             * - new class is no better than 140.       */
            if ((pPtpClock->profileDS.g8275.fallbackClass == ZL303XX_PTP_G8275p1_CLASS_FREERUN) &&
                (pPtpClock->profileDS.g8275.todSet == ZL303XX_TRUE))
            {
               if (*pOutClockClass < CLASS_GM_OUT_HLD_CAT1[pPtpClock->config.profileCfg.g8275p1.classEvalMethod])
               {
                  /* With TOD set, never go above 140 */
                  *pOutClockClass = CLASS_GM_OUT_HLD_CAT1[pPtpClock->config.profileCfg.g8275p1.classEvalMethod];
               }
            }
            else
            {
               *pOutClockClass = pPtpClock->profileDS.g8275.fallbackClass;
            }
         }
      }

      /* Some form of Initializing or FREERUN. */
      else
      {
         *pOutClockClass = ZL303XX_PTP_G8275p1_CLASS_FREERUN;
      }
   }

   /* For PACKET mode, use the Software PLL Status to evaluate the class and
    * traceability. */
   else if (pllPerfData->pllOperatingMode == ZL303XX_PTP_PLL_MODE_PKT_REF_SYNC)
   {
      /* A T-GM should never be operating in Packet Mode */
      ZL303XX_TRACE_ALWAYS(
            "zl303xx_PtpG8275p1ClockClassEvalGM_Method1: T-GM clock operating in packet mode",
            0,0,0,0,0,0);

      /* The clock is in Packet mode, so determine the Class to advertise. */
      zl303xx_PtpClockUncalibratedClassEvaluate(pPtpClock, pOutClockClass);
   }

   /* Set the fallbackClass of the T-GM. */
   /* PHASE LOCKED to a PRTC traceable clock. Maintain HLD-IN-SPEC on degrade. */
   /* HOLDOVER-IN-SPEC clock. Maintain HLD-IN-SPEC on degrade. */
   if (*pOutClockClass <= ZL303XX_PTP_G8275p1_GM_CLASS_IN_HOLDOVER)
   {
      pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_GM_CLASS_IN_HOLDOVER;
   }

   /* HLD-OUT-SPEC clocks. Maintain the current HLD-OUT-SPEC on degrade. */
   else if (*pOutClockClass <= ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT1)
   {
      pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT1;
   }
   else if (*pOutClockClass <= ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT2)
   {
      pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT2;
   }
   else if (*pOutClockClass <= ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT3)
   {
      pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT3;
   }

   /* Clock above HLD-OUT-SPEC so fall back to FREERUN on degrade. */
   else
   {
      pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_CLASS_FREERUN;
   }

}  /* END zl303xx_PtpG8275p1ClockClassEvalGM_Method1 */

/* zl303xx_PtpG8275p1ClockClassEvalBC_Method1 */
/**
   Telecom Profile for Phase only. Determines a clock's advertised class and
   frequency Traceable flag based on G.8275.1 - Table 6.2.

   Refer to zl303xx_PtpPllPerformanceDataS and zl303xx_PtpG8275p1ClassEvalMethodE.

  Parameters:
   [in]  pPtpClock         Pointer to an existing clock.
   [in]  pllPerfData       PLL performance data required to make the evaluation.

   [out]  pOutClockClass       The evaluated clock Class value.

  Return Value:  ZL303XX_OK                   Success.

*******************************************************************************/
void zl303xx_PtpG8275p1ClockClassEvalBC_Method1(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpPllPerformanceDataS *pllPerfData,
      Uint8T *pOutClockClass)
{
   /* Create a temporary variable to keep the code shorter. */
   Uint8T BC_CLASS_OUT_HOLDOVER = CLASS_BC_OUT_HLD[pPtpClock->config.profileCfg.g8275p1.classEvalMethod];

   /* If the local clock is PHASE LOCK'ed then:
    * - advertise the GM clockClass to downstream clients.
    * - if the GM is PRTC-locked, keep the BC Holdover-IN-SPEC if we lose lock,
    *   or if the GM is disconnected or degrades. */
   if ((pPtpClock->profileDS.g8275.clockState == ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED) ||
       (pllPerfData->settlingTimeActive == ZL303XX_TRUE))
   {
      if (pllPerfData->settlingTimeActive == ZL303XX_TRUE)
      {
         *pOutClockClass = pPtpClock->parentDS.grandmasterClockQuality.clockClass;
      }
      else
      {
         /* In case byPass is enabled, report the class provided via the clock
          * UNCALIBRATED filter. */
         zl303xx_PtpClockUncalibratedClassEvaluate(pPtpClock, pOutClockClass);
      }

      /* PHASE LOCKED to a PRTC traceable clock. Maintain HLD-IN-SPEC on degrade. */
      if (*pOutClockClass <= ZL303XX_PTP_G8275p1_GM_CLASS_LOCK)
      {
         pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_BC_CLASS_IN_HOLDOVER;
      }
      /* PHASE LOCKED to a HLD-OUT-SPEC clock. Maintain HLD-OUT-SPEC on degrade. */
      else if (*pOutClockClass <= BC_CLASS_OUT_HOLDOVER)
      {
         pPtpClock->profileDS.g8275.fallbackClass = BC_CLASS_OUT_HOLDOVER;
      }
      /* PHASE LOCKED to clock above HLD-OUT-SPEC so fall back to FREERUN on degrade. */
      else
      {
         pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_CLASS_FREERUN;
      }
   }

   /* If we are FREQ LOCK'ed then:
    * - If we have improved to this stage then perhaps we can advertise a better class.
    * - If we have degraded from PHASE_LOCK then perhaps we need to degrade our
    *   advertised class. */
   else if (pPtpClock->profileDS.g8275.clockState == ZL303XX_PTP_G8275p1_CLOCK_STATE_ACQUIRING)
   {
      /* If we were in FREERUN before but are moving closer towards PHASE_LOCK. */
      if (pPtpClock->profileDS.g8275.fallbackClass > BC_CLASS_OUT_HOLDOVER)
      {
         /* If the TOD is set and the clock we are acquiring with is better than
          * 165 then we are allowed to advertise 165 (at most). */
         if ((pPtpClock->profileDS.g8275.todSet == ZL303XX_TRUE) &&
             (pPtpClock->parentDS.grandmasterClockQuality.clockClass <= BC_CLASS_OUT_HOLDOVER))
         {
            pPtpClock->profileDS.g8275.fallbackClass = BC_CLASS_OUT_HOLDOVER;
         }
         /* ELSE: leave it at 248 until we achieve PHASE_LOCK */
         else
         {
            pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_CLASS_FREERUN;
         }
      }

      /* The case where we were LOCKED to a PRTC-GM at one point and have
       * degraded to a 'HOLDOVER-IN-SPEC' equivalent. */
      else if ((pPtpClock->profileDS.g8275.fallbackClass <= ZL303XX_PTP_G8275p1_BC_CLASS_IN_HOLDOVER) &&
               (pllPerfData->bHoldoverInSpec == ZL303XX_TRUE))
      {
         pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_BC_CLASS_IN_HOLDOVER;
      }

      else /* > 135 but <= 165 */
      {
         pPtpClock->profileDS.g8275.fallbackClass = BC_CLASS_OUT_HOLDOVER;
      }

      *pOutClockClass = pPtpClock->profileDS.g8275.fallbackClass;
   }

   /* If we are HOLDOVER_IN_SPEC from a reference point-of-view:
    * - If we never achieved 'IN_SPEC' from the Packet point-of-view, we have to
    *   be careful not to report a better class. (EX:, if we were ACQUIRING
    *   (class = 248) but then the reference went to HLD_IN_SPEC without ever
    *   achieving LOCK, we should continue reporting 248 and not 135). */
   else if (pPtpClock->profileDS.g8275.clockState == ZL303XX_PTP_G8275p1_CLOCK_STATE_HOLDOVER_IN_SPEC)
   {
      /* The case where we were LOCKED to a PRTC-GM at one point and have
       * degraded to a 'HOLDOVER-IN-SPEC' equivalent. */
      if (pPtpClock->profileDS.g8275.fallbackClass <= ZL303XX_PTP_G8275p1_BC_CLASS_IN_HOLDOVER)
      {
         pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_BC_CLASS_IN_HOLDOVER;
      }

      /* References are reporting IN_SPEC but we had never achieved an 'IN_SPEC'
       * state from the Packet point-of-view.  */
      else if (pPtpClock->profileDS.g8275.fallbackClass <= BC_CLASS_OUT_HOLDOVER)
      {
         pPtpClock->profileDS.g8275.fallbackClass = BC_CLASS_OUT_HOLDOVER;
      }
      /* ELSE: over 165 just advertise whatever it is. */
      else
      {
         pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_CLASS_FREERUN;
      }

      *pOutClockClass = pPtpClock->profileDS.g8275.fallbackClass;
   }

   /* If we are HOLDOVER_OUT_SPEC from a reference point-of-view:
    * - Do a similar check as was done with HOLDOVER_IN_SPEC (above) */
   else if (pPtpClock->profileDS.g8275.clockState == ZL303XX_PTP_G8275p1_CLOCK_STATE_HOLDOVER_OUT_SPEC)
   {
      /* The case where we were better than 'HOLDOVER-OUT-SPEC' but have now
       * degraded. */
      if (pPtpClock->profileDS.g8275.fallbackClass <= BC_CLASS_OUT_HOLDOVER)
      {
         pPtpClock->profileDS.g8275.fallbackClass = BC_CLASS_OUT_HOLDOVER;
      }

      /* ELSE: We never left FREERUN so keep reporting 248. */
      else
      {
         pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_CLASS_FREERUN;
      }

      *pOutClockClass = pPtpClock->profileDS.g8275.fallbackClass;
   }

   /* Some form of Initializing or FREERUN. */
   else
   {
      pPtpClock->profileDS.g8275.fallbackClass = ZL303XX_PTP_G8275p1_CLASS_FREERUN;
      *pOutClockClass = pPtpClock->profileDS.g8275.fallbackClass;
   }

}  /* END zl303xx_PtpG8275p1ClockClassEvalBC_Method1 */

/* zl303xx_PtpG8275p1ClockStateElecPhase */
/**
   Telecom Profile for Phase only. Determines the Clock-State of a node in
   Electrical mode (Phase + Frequency).

   Refer to zl303xx_PtpPllPerformanceDataS & zl303xx_PtpG8275p1ClockStateE.

  Parameters:
   [in]  pPtpClock      Pointer to an existing clock.
   [in]  pllPerfData    PLL performance data required to make the evaluation.

  Return Value:  zl303xx_PtpG8275p1ClockStateE

*******************************************************************************/
zl303xx_PtpG8275p1ClockStateE zl303xx_PtpG8275p1ClockStateElecPhase(
      zl303xx_PtpPllPerformanceDataS *pPllPerfData)
{
   /* Set a default Return Value. */
   zl303xx_PtpG8275p1ClockStateE phaseClockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_FREERUN;
   zl303xx_PtpG8275p1ClockStateE freqClockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_FREERUN;

   /* First, evaluate the Frequency Lock state of the HW PLL. */
   freqClockState = zl303xx_PtpG8275p1ClockStateElecFreq(pPllPerfData);

   /* If the FREQUENCY is LOCKED, but the PHASE is not LOCKED, return
    * 'PHASE ACQUIRING'. Otherwise, return the current FREQUENCY State.  */
   if ((freqClockState == ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED) &&
       (pPllPerfData->hwPllOperatingState != ZL303XX_PTP_PLL_STATE_PHASE_ACQUIRED))
   {
      phaseClockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_ACQUIRING;
   }
   else
   {
      phaseClockState = freqClockState;
   }

   return phaseClockState;
}  /* END zl303xx_PtpG8275p1ClockStateElecPhase */

/* zl303xx_PtpG8275p1ClockStateElecFreq */
/**
   Telecom Profile for Phase only. Determines the Clock-State of the Frequency
   portion of an Electrical source. Used when a node is in Electrical
   (Phase + Frequency) mode and Hybrid mode.

   Refer to zl303xx_PtpPllPerformanceDataS & zl303xx_PtpG8275p1ClockStateE.

  Parameters:
   [in]  pPtpClock         Pointer to an existing clock.
   [in]  pllPerfData       PLL performance data required to make the evaluation.

  Return Value:  zl303xx_PtpG8275p1ClockStateE

*******************************************************************************/
zl303xx_PtpG8275p1ClockStateE zl303xx_PtpG8275p1ClockStateElecFreq(
      zl303xx_PtpPllPerformanceDataS *pPllPerfData)
{
   /* Set a default Return Value: . */
   zl303xx_PtpG8275p1ClockStateE freqClockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_FREERUN;

   if ((pPllPerfData->hwPllOperatingState == ZL303XX_PTP_PLL_STATE_PHASE_ACQUIRED) ||
       (pPllPerfData->hwPllOperatingState == ZL303XX_PTP_PLL_STATE_FREQ_ACQUIRED) ||
       (pPllPerfData->hwPllOperatingState == ZL303XX_PTP_PLL_STATE_PHASE_ACQUIRING))
   {
      freqClockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED;
   }
   else if (pPllPerfData->hwPllOperatingState == ZL303XX_PTP_PLL_STATE_FREQ_ACQUIRING)
   {
      freqClockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_ACQUIRING;
   }
   else if ((pPllPerfData->hwPllOperatingState == ZL303XX_PTP_PLL_STATE_HOLDOVER) ||
            (pPllPerfData->hwPllOperatingState == ZL303XX_PTP_PLL_STATE_REF_FAILED))
   {
      if (pPllPerfData->bHoldoverInSpec == ZL303XX_FALSE)
      {
         freqClockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_HOLDOVER_OUT_SPEC;
      }
      else
      {
         freqClockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_HOLDOVER_IN_SPEC;
      }
   }

   return freqClockState;
}  /* END zl303xx_PtpG8275p1ClockStateElecFreq */

/* zl303xx_PtpG8275p1ClockStatePktPhase */
/**
   Telecom Profile for Phase only. Determines the Clock-State of the Phase
   portion of a Packet source. Used when a node is in pure Packet or Hybrid mode.

   Refer to zl303xx_PtpPllPerformanceDataS & zl303xx_PtpG8275p1ClockStateE.

  Parameters:
   [in]  pPtpClock         Pointer to an existing clock.
   [in]  pllPerfData       PLL performance data required to make the evaluation.

  Return Value:  zl303xx_PtpG8275p1ClockStateE

*******************************************************************************/
zl303xx_PtpG8275p1ClockStateE zl303xx_PtpG8275p1ClockStatePktPhase(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpPllPerformanceDataS *pPllPerfData)
{
   /* Set a default Return Value: . */
   zl303xx_PtpG8275p1ClockStateE phaseClockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_FREERUN;

   /* Variables for looping through the Clock Object's linked lists. */
   zl303xx_PortDataS *pPtpPort = NULL;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Stream state counters. */
   Uint16T streamState[PTP_STREAM_SUBSTATE_SLAVE + 1];
   OS_MEMSET(streamState, 0x00, sizeof(Uint16T)*(PTP_STREAM_SUBSTATE_SLAVE + 1));

   /* First, evaluate the PLL performance.
    * If the PLL is LOCKED then we evaluate the software state. */
   if ((pPllPerfData->swPllOperatingState == ZL303XX_PTP_PLL_STATE_UNKNOWN) ||
       (pPllPerfData->swPllOperatingState == ZL303XX_PTP_PLL_STATE_FREERUN) ||
       (pPllPerfData->swPllOperatingState >= ZL303XX_PTP_PLL_STATE_REF_FAILED))
   {
      phaseClockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_FREERUN;
   }
   else if ((pPllPerfData->swPllOperatingState == ZL303XX_PTP_PLL_STATE_FREQ_ACQUIRING) ||
            (pPllPerfData->swPllOperatingState == ZL303XX_PTP_PLL_STATE_FREQ_ACQUIRED) ||
            (pPllPerfData->swPllOperatingState == ZL303XX_PTP_PLL_STATE_PHASE_ACQUIRING))
   {
      phaseClockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_ACQUIRING;
   }
   else if (pPllPerfData->swPllOperatingState == ZL303XX_PTP_PLL_STATE_HOLDOVER)
   {
      if (pPllPerfData->bHoldoverInSpec == ZL303XX_FALSE)
      {
         phaseClockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_HOLDOVER_OUT_SPEC;
      }
      else
      {
         phaseClockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_HOLDOVER_IN_SPEC;
      }
   }

   /* PLL is ZL303XX_PTP_PLL_STATE_PHASE_ACQUIRED */
   /* So evaluate the PTP Stream state. */
   /* In BYPASS mode, swPllOperatingState is forced to LOCK so the PLL status
    * is skipped (bypassed) and only PTP is evaluated.   */
   else
   {
      /* LOOP through each Port & Stream of the clock and count the Stream States. */
      ZL303XX_PTP_FOR_EACH_PORT(&pPtpClock->portList, pPtpPort)
      {
         /* Check the Virtual PTP Port state. */
         if (pPtpPort->virtualData.isVirtualPort == ZL303XX_TRUE)
         {
            if (zl303xx_GetV2PortState(pPtpPort) ==ZL303XX_PORT_STATE_SLAVE)
            {
               streamState[PTP_STREAM_SUBSTATE_SLAVE]++;
            }
            else
            {
               streamState[PTP_STREAM_SUBSTATE_PASSIVE]++;
            }
         }
         else /* Count the stream states. */
         {
            ZL303XX_PTP_FOR_EACH_STREAM(&pPtpPort->streamList, pPtpStream)
            {
               if (pPtpStream->streamSubstate <= PTP_STREAM_SUBSTATE_SLAVE)
               {
                  streamState[pPtpStream->streamSubstate]++;
               }
               else
               {
                  streamState[PTP_STREAM_SUBSTATE_INIT]++;
               }
            }
         }
      }

      /* If no stream or port 'appears to be in SLAVE state then check the
       * grandmasterSource value in case the local clock is getting GM data from an
       * external source (specifically, in the case of a distributed clock). */
      if ((streamState[PTP_STREAM_SUBSTATE_SLAVE] == 0) &&
          (pPtpClock->gmSource.location == ZL303XX_PTP_GM_SRC_EXTERNAL))
      {
         streamState[PTP_STREAM_SUBSTATE_SLAVE]++;
      }

      /* Check for Each condition (G.8275.1 - Appendix V) */
      if ((streamState[PTP_STREAM_SUBSTATE_MASTER] == 0) &&
          (streamState[PTP_STREAM_SUBSTATE_PASSIVE] == 0) &&
          (streamState[PTP_STREAM_SUBSTATE_UNCALIBRATED] == 0) &&
          (streamState[PTP_STREAM_SUBSTATE_SLAVE] == 0) &&

          /* To get here we are PLL PHASE LOCKED but have a stream in UNINIT or
           * LISTENING state. It is most likely a bypass state but if not then
           * this is more likely HOLDOVER. (i.e. we were PLL LOCKED but now
           * have lost the PTP Stream). */
          (pPtpClock->profileDS.g8275.fallbackClass >= ZL303XX_PTP_G8275p1_CLASS_FREERUN))
      {
         phaseClockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_FREERUN;
      }

      else if (streamState[PTP_STREAM_SUBSTATE_UNCALIBRATED] > 0)
      {
         phaseClockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_ACQUIRING;
      }

      else if (streamState[PTP_STREAM_SUBSTATE_SLAVE] > 0)
      {
         phaseClockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED;
      }

      /* Holdover state */
      else
      {
         if (pPllPerfData->bHoldoverInSpec == ZL303XX_FALSE)
         {
            phaseClockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_HOLDOVER_OUT_SPEC;
         }
         else
         {
            phaseClockState = ZL303XX_PTP_G8275p1_CLOCK_STATE_HOLDOVER_IN_SPEC;
         }
      }
   }

   return phaseClockState;
}  /* END zl303xx_PtpG8275p1ClockStatePktPhase */

/* zl303xx_PtpG8275p1ClockStateDescrSet */
/**
   Telecom Profile for Phase only. Local sub-routine to update the Clock-State's
   description parameter.

   Refer to zl303xx_PtpG8275p1ClockStateE.

  Parameters:
   [in]  pPtpClock      Pointer to an existing clock.
   [in]  g8275p1State   Last evaluated Clock-State.

  Return Value:  void

*******************************************************************************/
void zl303xx_PtpG8275p1ClockStateDescrSet(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpG8275p1ClockStateE g8275p1State)
{
   switch (g8275p1State)
   {
      case ZL303XX_PTP_G8275p1_CLOCK_STATE_FREERUN :
      {
         pPtpClock->profileDS.g8275.clockClassDesc = ZL303XX_PTP_G8275p1_CLOCK_CLASS_DESC_FREERUN;
         break;
      }
      case ZL303XX_PTP_G8275p1_CLOCK_STATE_ACQUIRING :
      case ZL303XX_PTP_G8275p1_CLOCK_STATE_HOLDOVER_IN_SPEC :
      case ZL303XX_PTP_G8275p1_CLOCK_STATE_HOLDOVER_OUT_SPEC :
      {
         pPtpClock->profileDS.g8275.clockClassDesc = ZL303XX_PTP_G8275p1_CLOCK_CLASS_DESC_HOLDOVER;
         break;
      }
      case ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED :
      {
         pPtpClock->profileDS.g8275.clockClassDesc = ZL303XX_PTP_G8275p1_CLOCK_CLASS_DESC_LOCKED;
         break;
      }
      case ZL303XX_PTP_G8275p1_CLOCK_STATE_UNKNOWN :
      default :
      {
         pPtpClock->profileDS.g8275.clockClassDesc = ZL303XX_PTP_G8275p1_CLOCK_CLASS_DESC_UNKNOWN;
         break;
      }
   }
}  /* END zl303xx_PtpG8275p1ClockStateDescrSet */

/******************************************************************************/
/*******   INTERNAL FUNCTION DEFINITIONS   ************************************/
/******************************************************************************/

/*

  Function Name:
    zl303xx_PtpG8275TimestampIfRateTlvUnpack

  Details:
   Unpacks a G8275 Timestamp Interface Rate TLV, into a compatible data type.

  Parameters:
   [in]  pInTlvBuf      Pointer to the data portion of the input TLV buffer.
                              (Begins at the interfaceBitPeriod field).

   [out] pOutTlvDataS   Pointer to a compatible TLV data structure.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpG8275TimestampIfRateTlvUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_PtpG8275TimestampIfRateTlvS *pOutTlvDataS)
{
   /* -------------------------------------------------------
    * Field                      Length    Offset
    * ------------------------------------------------------
    * interfaceBitPeriod            8         0
    * numberBitsBeforeTimestamp     2         8
    * numberBitsAfterTimestamp      2         10
    * ------------------------------------------------------
    * Total Length                 12                                      */

   /* interfaceBitPeriod */
   UNPACK_64BIT(pInTlvBuf + 0, pOutTlvDataS->interfaceBitPeriod);

   /* numberBitsBeforeTimestamp */
   pOutTlvDataS->numberBitsBeforeTimestamp = UNPACK_16BIT(pInTlvBuf + 8);

   /* numberBitsAfterTimestamp */
   pOutTlvDataS->numberBitsAfterTimestamp = UNPACK_16BIT(pInTlvBuf + 10);

   return ZL303XX_PTP_G8275_TLV_TS_IF_RATE_DATA_LEN;
}  /* END zl303xx_PtpG8275TimestampIfRateTlvUnpack */

/*

  Function Name:
    zl303xx_PtpG8275TimestampIfRateTlvPack

  Details:
   Packs a G8275 Timestamp Interface Rate data type into a compatible TLV.

  Parameters:
   [in]  pInTlvDataS    Pointer to the TLV structure to pack.

   [out] pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpG8275TimestampIfRateTlvPack(
      zl303xx_PtpG8275TimestampIfRateTlvS *pInTlvDataS,
      Uint8T *pOutTlvBuf)
{
   /* Refer to the UNPACK routine (above) for TLV detailed layout.    */
   /* No need to clear the output buffer since there are no reserved fields. */

   /* interfaceBitPeriod */
   PACK_64BIT(pOutTlvBuf + 0, pInTlvDataS->interfaceBitPeriod);

   /* numberBitsBeforeTimestamp */
   PACK_16BIT(pOutTlvBuf + 8, pInTlvDataS->numberBitsBeforeTimestamp);

   /* numberBitsAfterTimestamp */
   PACK_16BIT(pOutTlvBuf + 10, pInTlvDataS->numberBitsAfterTimestamp);

   return ZL303XX_PTP_G8275_TLV_TS_IF_RATE_DATA_LEN;
}  /* END _zl303xx_PtpG8275TimestampIfRateTlvPack */



