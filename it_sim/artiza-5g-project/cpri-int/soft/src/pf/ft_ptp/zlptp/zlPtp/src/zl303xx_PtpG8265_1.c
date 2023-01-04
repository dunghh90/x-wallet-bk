

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Contains functions required to implement the G.8265.1 (Telecom Profile)
*     standard.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"

#include "zl303xx_Error.h"
#include "zl303xx_Ptsf.h"
#include "zl303xx_Ptpd.h"

#include "zl303xx_PtpG8265p1Api.h"
#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpFunctionExport.h"

/*****************   DEFINES     **********************************************/
/* Seconds to declare PTSF-unusable. This is to prevent toggling on startup
 * because time stamps are only passed to the servo algorithm in the SLAVE and
 * UNCALIBRATED states. */
#define PTSF_UNUSABLE_TIMEOUT_SEC  30

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/* Wrapper to error if Telecom Profile is not enabled. */
#define zl303xx_PtpClockDataGetG8265p1(clockHandle, ppPtpClock) \
               zl303xx_PtpClockDataGetForProfile((clockHandle), (ZL303XX_PTP_PROFILE_TELECOM_G8265_1), (ppPtpClock))

static zlStatusE zl303xx_PtpTelecomLogRangeError(zl303xx_BooleanE retError);

/*****************   STATIC GLOBAL VARIABLES   ********************************/
/* Index 0 = clockClass 80
 * Index 1 = clockClass 82, etc... */
static const zl303xx_PtpTelecomQualityLevelS Zl303xx_PtpClockClassToQL[] =
     /* SSM    Option I              Option II             Option III        */
      {{0x1,  {ZL303XX_G781_QL_INV1,   ZL303XX_G781_QL_PRS,    ZL303XX_G781_QL_INV1 }},
       {0x0,  {ZL303XX_G781_QL_INV0,   ZL303XX_G781_QL_STU,    ZL303XX_G781_QL_UNK  }},
       {0x2,  {ZL303XX_G781_QL_PRC,    ZL303XX_G781_QL_INV2,   ZL303XX_G781_QL_INV2 }},
       {0x7,  {ZL303XX_G781_QL_INV7,   ZL303XX_G781_QL_ST2,    ZL303XX_G781_QL_INV7 }},
       {0x3,  {ZL303XX_G781_QL_INV3,   ZL303XX_G781_QL_INV3,   ZL303XX_G781_QL_INV3 }},
       {0x4,  {ZL303XX_G781_QL_SSU_A,  ZL303XX_G781_QL_TNC,    ZL303XX_G781_QL_INV4 }},
       {0x5,  {ZL303XX_G781_QL_INV5,   ZL303XX_G781_QL_INV5,   ZL303XX_G781_QL_INV5 }} ,
       {0x6,  {ZL303XX_G781_QL_INV6,   ZL303XX_G781_QL_INV6,   ZL303XX_G781_QL_INV6 }},
       {0x8,  {ZL303XX_G781_QL_SSU_B,  ZL303XX_G781_QL_INV8,   ZL303XX_G781_QL_INV8 }},
       {0x9,  {ZL303XX_G781_QL_INV9,   ZL303XX_G781_QL_INV9,   ZL303XX_G781_QL_INV9 }},
       {0xD,  {ZL303XX_G781_QL_INV13,  ZL303XX_G781_QL_ST3E,   ZL303XX_G781_QL_INV13}},
       {0xA,  {ZL303XX_G781_QL_INV10,  ZL303XX_G781_QL_ST3,    ZL303XX_G781_QL_INV10}},
       {0xB,  {ZL303XX_G781_QL_SEC,    ZL303XX_G781_QL_INV11,  ZL303XX_G781_QL_SEC  }},
       {0xC,  {ZL303XX_G781_QL_INV12,  ZL303XX_G781_QL_SMC,    ZL303XX_G781_QL_INV12}},
       {0xE,  {ZL303XX_G781_QL_INV14,  ZL303XX_G781_QL_PROV,   ZL303XX_G781_QL_INV14}},
       {0xF,  {ZL303XX_G781_QL_DNU,    ZL303XX_G781_QL_DUS,    ZL303XX_G781_QL_INV15}},

       /* Returned when clock class = 255 (slave is using local datasets). */
       {ZL303XX_PTP_TELECOM_SSM_QL_INVALID, {ZL303XX_G781_QL_DNU,   ZL303XX_G781_QL_DNU, ZL303XX_G781_QL_DNU}},

       /* Returned when clock class is invalid. */
       {ZL303XX_PTP_TELECOM_SSM_QL_INVALID, {ZL303XX_G781_QL_UNDEF, ZL303XX_G781_QL_UNDEF}}};

/* Array indexes for special cases. */
#define QL_SLAVE_INDEX   ((sizeof(Zl303xx_PtpClockClassToQL) / sizeof(*Zl303xx_PtpClockClassToQL)) - 2)
#define QL_INVALID_INDEX ((sizeof(Zl303xx_PtpClockClassToQL) / sizeof(*Zl303xx_PtpClockClassToQL)) - 1)

/* SSM QL is in the range 0x0-0xF. */
static Uint8T Zl303xx_PtpQLToClockClass[16] =
      {82, 80,  84,  88,  90,  92,  94,  86,
       96, 98, 102, 104, 106, 100, 108, 110};

/* Array of description strings for G.781 QL values. Must match the
 * zl303xx_PtpTelecomQualityLevelE enum. */
static const char *Zl303xx_PtpQualityLevelStr[ZL303XX_G781_QL_NUM_VALUES + 1] = {
      "QL-undef",

      "QL-PRC",
      "QL-SSU-A",
      "QL-SSU-B",
      "QL-SEC",
      "QL-DNU",

      "QL-PRS",
      "QL-STU",
      "QL-ST2",
      "QL-TNC",
      "QL-ST3E",
      "QL-ST3",
      "QL-SMC",
      "QL-ST4",
      "QL-PROV",
      "QL-DUS",

      "QL-UNK",

      "QL-INV0", "QL-INV1", "QL-INV2",  "QL-INV3",  "QL-INV4",  "QL-INV5",  "QL-INV6",  "QL-INV7",
      "QL-INV8", "QL-INV9", "QL-INV10", "QL-INV11", "QL-INV12", "QL-INV13", "QL-INV14", "QL-INV15",
      "QL-FAILED"

      "ERROR"
};

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

const zl303xx_PtpProfileS Zl303xx_PtpG8265ProfileInfo = {
   "ITU-T PTP Profile for Frequency distribution without timing support from the network (Unicast mode)",
   "Version 1.0",
   {0x00, 0x19, 0xA7, 0x00, 0x01, 0x00},
   "This profile is specified by the ITU-T",
   "A copy may be obtained from www.itu.int"
};

/*****************   PROFILE BINDING FUNCTION DECLARATIONS   ******************/

/**
    zl303xx_PtpG8265p1ProfileInfoGet

  Details:
   Retrieves the profile description information for the profile.

  Parameters:
   [in]  profileInfo    Pointer to the structure to store the profile information.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

 *****************************************************************************/
zlStatusE zl303xx_PtpG8265p1ProfileInfoGet(const zl303xx_PtpProfileS **profileInfo)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(profileInfo);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      *profileInfo = &Zl303xx_PtpG8265ProfileInfo;
   }

   return status;
}  /* END zl303xx_PtpG8265p1ProfileInfoGet */

/*

  Function Name:
    zl303xx_PtpG8265p1StreamDefaults

  Details:
   Routine to set the G.8265.1 (Telecom Frequency Profile) specific stream
   parameters to their default values.

  Parameters:
   [in]  pStreamInitS   Pointer to the PTP creation data structure for the
                              stream (from the PtpApiTypes.h module). This is
                              a void type so that all PTP Object types can use
                              the same interface.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpG8265p1StreamDefaults(void *pStreamInitS)
{
   /* Input pointer is verified in the COMMON routine. */
   zl303xx_PtpStreamCreateS *pStreamCreateS = (zl303xx_PtpStreamCreateS *)pStreamInitS;
   zlStatusE status = ZL303XX_OK;

   /* Set G.8265.1 specific values. */
   if (status == ZL303XX_OK)
   {
      /* Contract Retry (Denial) */
      pStreamCreateS->unicastNegotiation.retry.denial.logQueryCount
                                          = ZL303XX_PTP_TELECOM_UN_RETRIES;
      pStreamCreateS->unicastNegotiation.retry.denial.lockoutSec
                                          = ZL303XX_PTP_TELECOM_UN_LOCKOUT_SEC;

      /* Contract Retry (Timeout) */
      pStreamCreateS->unicastNegotiation.retry.timeout.logQueryCount
                                          = ZL303XX_PTP_TELECOM_UN_RETRIES;
      pStreamCreateS->unicastNegotiation.retry.timeout.lockoutSec
                                          = ZL303XX_PTP_TELECOM_UN_LOCKOUT_SEC;
   }

   return status;
}  /* END zl303xx_PtpG8265p1StreamDefaults */

/*

  Function Name:
    zl303xx_PtpG8265p1ClockCreateActions

  Details:
   Routine to perform G.8265.1 (Telecom Profile - Frequency) actions related to
   clock creation. This routine is BIND'ed (bound) to the ProfileDef list and
   gets called at the end of the core Clock Creation process.
   (See zl303xx_PtpClockCreate()).

   For this profile, the Clock is configured to used Stacked TLVs on initial
   contract request.

  Parameters:
   [in]  pClockS  Pointer to the PTP clock data structure (as a void).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pClockS is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpG8265p1ClockCreateActions(void *pClockS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pClockS);

   /* De-reference the clock pointer (if NULL, we skip using it). */
   zl303xx_PtpClockS *pPtpClockS = (zl303xx_PtpClockS *)pClockS;

   /* Set the Clock to used Stacked TLVs on initial unicast contract request. */
   if (status == ZL303XX_OK)
   {
      /* For G.8265.1, use stack TLV on initial contract request regardless of
       * the stacked TLV setting. */
      pPtpClockS->stackAllServicesOn1stRequest = ZL303XX_TRUE;

      /* Telecom Profile parameters. */
      pPtpClockS->profileDS.telecom.pQuality =
               zl303xx_PtpClockClassToQL(pPtpClockS->config.defaultDS.clockQuality.clockClass);
      pPtpClockS->profileDS.telecom.enableOutput = ZL303XX_TRUE;
   }

   return status;
}  /* END zl303xx_PtpG8265p1ClockCreateActions */

/* zl303xx_PtpG8265p1ClockQLUpdate */
/** 

   Only applicable to Telecom Profile. Sets the clock's QL values based on the
   clock's parentDS and the QL hold off timer.

  Parameters:
   [in]   pClock   Pointer to a clock.

*******************************************************************************/
zlStatusE zl303xx_PtpG8265p1ClockQLUpdate(void *pClock)
{
   zl303xx_PtpClockS *pPtpClock = (zl303xx_PtpClockS *)pClock;

   if ((pPtpClock->config.telecom.qlHoldOffSec == 0) ||
       (pPtpClock->gmSource.pPtpStream != NULL) ||
       (zl303xx_PtpTimerExpired(&pPtpClock->profileDS.telecom.qlHoldOffTimer)))
   {
      pPtpClock->profileDS.telecom.pQuality =
         zl303xx_PtpClockClassToQL(pPtpClock->parentDS.grandmasterClockQuality.clockClass);
      zl303xx_PtpTimerStop(&pPtpClock->profileDS.telecom.qlHoldOffTimer);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 2, "Clock %lu: QL Hold off expired.",
                    pPtpClock->clockHandle, 0,0,0,0,0);
   }
   else if ((!zl303xx_PtpTimerActive(&pPtpClock->profileDS.telecom.qlHoldOffTimer)) &&
            (pPtpClock->profileDS.telecom.pQuality->ssm != ZL303XX_PTP_TELECOM_SSM_QL_INVALID))
   {
      zl303xx_TimeStamp interval = {{0,0},0};

      interval.secondsField.lo = pPtpClock->config.telecom.qlHoldOffSec;
      zl303xx_PtpTimerStart(pPtpClock->clockHandle,
                           &pPtpClock->profileDS.telecom.qlHoldOffTimer,
                           &interval,
                           ZL303XX_FALSE);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 1, "Clock %lu: QL Hold off timer started (%lu sec).",
                    pPtpClock->clockHandle, interval.secondsField.lo, 0,0,0,0);
   }

   return ZL303XX_OK;
}  /* END zl303xx_PtpG8265p1ClockQLUpdate */

/**
    zl303xx_PtpG8265p1TransportSpecificCheck

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
zlStatusE zl303xx_PtpG8265p1TransportSpecificCheck(
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
       * return ERROR in G.8265.1 regardless of the strictFlag setting. */
      if (*transportSpecific & 0x1)
      {
          status = ZL303XX_PARAMETER_INVALID;
      }
   }

   return status;
}  /* END zl303xx_PtpG8265p1TransportSpecificCheck */

/* zl303xx_PtpG8265p1ClockQualityCheck */
/** 

   Determines if a PTP clockQuality::clockClass value can be converted to a
   SSM quality level.

  Parameters:
   [in]  clockQuality   PTP clockQuality to check.
   [in]  retError       ZL303XX_TRUE to return an error status. Otherwise,
                             ZL303XX_OK is returned with extra logging.

*******************************************************************************/
zlStatusE zl303xx_PtpG8265p1ClockQualityCheck(
      zl303xx_ClockQuality *clockQuality,
      zl303xx_BooleanE retError)
{
   /* For G.8265.1, clockClass must be 255 or an even number in the range
    * 80-110, inclusive. */
   if ((clockQuality->clockClass != 255) &&
       ((clockQuality->clockClass & 1) || (clockQuality->clockClass < 80) || (clockQuality->clockClass > 110)))
   {
      ZL303XX_TRACE_ALWAYS("(G.8265.1) Invalid clockClass=%u.",
            clockQuality->clockClass, 0,0,0,0,0);

      if (retError == ZL303XX_TRUE)
      {
         return ZL303XX_PARAMETER_INVALID;
      }
      else
      {
         ZL303XX_TRACE_ALWAYS("(G.8265.1) Continuing with unsupported mode.", 0,0,0,0,0,0);
      }
   }

   return ZL303XX_OK;
}  /* END zl303xx_PtpG8265p1ClockQualityCheck */

/* zl303xx_PtpG8265p1QueryIntervalCheck */
/** 

   Checks if a Unicast Master Table logQueryInterval value is within the range
   supported by the Telecom Profile.

  Parameters:
   [in]   logQueryInterval  log2 of the query interval in seconds.
   [in]   retError          ZL303XX_TRUE to return an error status. Otherwise,
                                 ZL303XX_OK is returned with extra logging.

*******************************************************************************/
zlStatusE zl303xx_PtpG8265p1QueryIntervalCheck(Sint8T *logQueryInterval,
                                               zl303xx_BooleanE retError)
{
   zlStatusE status = ZL303XX_OK;

   if (*logQueryInterval < ZL303XX_PTP_TELECOM_LQI_MIN)
   {
      ZL303XX_TRACE_ALWAYS("Invalid logQueryInterval=%d (must be >%d).",
                           *logQueryInterval, ZL303XX_PTP_TELECOM_LQI_MIN, 0,0,0,0);
      status = zl303xx_PtpTelecomLogRangeError(retError);
   }

   return status;
}  /* END zl303xx_PtpG8265p1QueryIntervalCheck */

/* zl303xx_PtpG8265p1UniNegDurationCheck */
/** 

   Checks if a unicast negotiation contract duration value is within the range
   supported by the Telecom Profile.

  Parameters:
   [in]   duration   Contract duration in seconds.
   [in]   retError   ZL303XX_TRUE to return an error status. Otherwise,
                          ZL303XX_OK is returned with extra logging.

*******************************************************************************/
zlStatusE zl303xx_PtpG8265p1UniNegDurationCheck(Uint32T *duration, zl303xx_BooleanE retError)
{
   zlStatusE status = ZL303XX_OK;

   if ((*duration < ZL303XX_PTP_TELECOM_UN_DURATION_MIN) ||
       (*duration > ZL303XX_PTP_TELECOM_UN_DURATION_MAX))
   {
      ZL303XX_TRACE_ALWAYS("Invalid uni-neg duration=%lu (valid range: %lu-%lu).",
         *duration, ZL303XX_PTP_TELECOM_UN_DURATION_MIN, ZL303XX_PTP_TELECOM_UN_DURATION_MAX, 0,0,0);
      status = zl303xx_PtpTelecomLogRangeError(retError);
   }

   return status;
}

/**
    zl303xx_PtpG8265p1BmcaDataAdd

  Details:
   Routine to extract Profile specific BMCA data from a Foreign Master Table entry
   and insert it into the BmcaCompareS structure.

  Parameters:
   [in]  pFmtEntry   Pointer to a Foreign Master Table entry from which to
                           extract the Profile specific data.

   [out] pBmcaDataS  Pointer to the BMCA structure.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      Either of the input pointers is invalid.
     ZL303XX_PARAMETER_INVALID    profileId of the entry is not G.8265.1.

 *****************************************************************************/
zlStatusE zl303xx_PtpG8265p1BmcaDataAdd(
      zl303xx_ForeignMasterRecordS *pFmtEntry,
      zl303xx_PtpEventClockBmcaEntryS *pBmcaDataS)
{
   zlStatusE status = ZL303XX_OK;

   /* PTSF Flag counter (needed by FOR loop). */
   zl303xx_PtsfFlagE ptsfFlag;

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
               "zl303xx_PtpG8265p1BmcaDataAdd: NULL Pointer", 0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
   }

   /* Make sure the specified profile of the entry is G.8265.1. */
   if (status == ZL303XX_OK)
   {
      if (pFmtEntry->streamData->portData->clock->config.profile != ZL303XX_PTP_PROFILE_TELECOM_G8265_1)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpG8265p1BmcaDataAdd: Invalid Profile (%d: Expected %d)",
               pFmtEntry->streamData->portData->clock->config.profile,
               ZL303XX_PTP_PROFILE_TELECOM_G8265_1, 0,0,0,0);

         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* If all is well, fill in the BMCA Compare structure with the FMT data. */
   if (status == ZL303XX_OK)
   {
      /* PTSF value */
      pBmcaDataS->telecomData.ptsf = zl303xx_PtsfFlagGetAndMask(pFmtEntry->streamData->clockStreamHandle, ZL303XX_PTSF_MAIN);

      /* Data-fill every PTSF-Flag and mask for the stream. */
      pBmcaDataS->telecomData.ptsfFlags[0] = 0;
      pBmcaDataS->telecomData.ptsfMasks[0] = 0;
      for (ptsfFlag = 0; ptsfFlag < ZL303XX_PTSF_NUM_FLAGS; ptsfFlag++)
      {
         pBmcaDataS->telecomData.ptsfFlags[0] |= (zl303xx_PtsfFlagGet(pFmtEntry->streamData->clockStreamHandle, ptsfFlag) << ptsfFlag);
         pBmcaDataS->telecomData.ptsfMasks[0] |= (zl303xx_PtsfMaskGet(pFmtEntry->streamData->clockStreamHandle, ptsfFlag) << ptsfFlag);
      }

      /* From the UMT */
      if (pFmtEntry->streamData->pUmtEntry != NULL)
      {
         pBmcaDataS->telecomData.priority = pFmtEntry->streamData->pUmtEntry->telecom.priority;
         pBmcaDataS->telecomData.clockClass = pFmtEntry->streamData->pUmtEntry->telecom.clockClass;
         pBmcaDataS->telecomData.stepsRemoved = pFmtEntry->streamData->pUmtEntry->telecom.stepsRemoved;
      }
      else
      {
         /* G.8265.1 expects Unicast Neg */
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpEventClockBmcaUpdate: Error: No UMT entry for stream %lu",
               pFmtEntry->streamData->clockStreamHandle, 0,0,0,0,0);
         pBmcaDataS->telecomData.priority = 0;
         pBmcaDataS->telecomData.clockClass = 0;
         pBmcaDataS->telecomData.stepsRemoved = 0;
      }

      /* Copied from generic data */
      OS_MEMCPY(pBmcaDataS->telecomData.grandmasterIdentity,
             pFmtEntry->compareData.grandmasterIdentity,
             PTP_V2_CLK_IDENTITY_LENGTH);
   }

   return status;
}  /* END zl303xx_PtpG8265p1BmcaDataAdd */

/**
    zl303xx_PtpG8265p1BmcaDataAdd

  Details:
   Routine to extract Profile specific BMCA data from a Virtual PTP Port and
   insert it into the BmcaCompareS structure.

  Parameters:
   [in]  pPtpVirtualPort   Pointer to a Virtual PTP Port from which to
                                 extract the Profile specific data.

   [out] pOutBmcaDataS  Pointer to the BMCA structure.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      Either of the input pointers is invalid.
     ZL303XX_PARAMETER_INVALID    profileId of the entry is not G.8265.1 or
                                    Port is not configured for Virtual operations.

 *****************************************************************************/
zlStatusE zl303xx_PtpG8265p1BmcaVpDataAdd(
      zl303xx_PortDataS *pPtpVirtualPort,
      void *pOutBmcaDataS)
{
   zlStatusE status = ZL303XX_OK;

   /* Cast the local pointer to the proper type. */
   zl303xx_PtpEventClockBmcaEntryS *pBmcaDataS = (zl303xx_PtpEventClockBmcaEntryS *)pOutBmcaDataS;

   /* Check input pointers. */
   if (status == ZL303XX_OK)
   {
      if ((pPtpVirtualPort == NULL) || (pBmcaDataS == NULL))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpG8265p1BmcaVpDataAdd: NULL Pointer", 0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
   }

   /* Check Port configuration */
   if (status == ZL303XX_OK)
   {
      /* Make sure the specified profile of the entry is G.8265.1. */
      if (pPtpVirtualPort->clock->config.profile != ZL303XX_PTP_PROFILE_TELECOM_G8265_1)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpG8265p1BmcaVpDataAdd: Invalid Profile (%d: Expected %d)",
               pPtpVirtualPort->clock->config.profile,
               ZL303XX_PTP_PROFILE_TELECOM_G8265_1, 0,0,0,0);

         status = ZL303XX_PARAMETER_INVALID;
      }

      /* Make sure the Port is configured as a Virtual Port. */
      else if (pPtpVirtualPort->virtualData.isVirtualPort != ZL303XX_TRUE)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpG8265p1BmcaVpDataAdd: Port(%d) NOT configured for Virtual operations",
               pPtpVirtualPort->clockPortHandle, 0,0,0,0,0);

         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* If all is well, fill in the BMCA Compare structure with the VP data. */
   if (status == ZL303XX_OK)
   {
      pBmcaDataS->telecomData.ptsf = pPtpVirtualPort->virtualData.config.ptsf;
      pBmcaDataS->telecomData.priority = pPtpVirtualPort->virtualData.config.localPriority;
      pBmcaDataS->telecomData.clockClass = pPtpVirtualPort->virtualData.config.clockQuality.clockClass;
      pBmcaDataS->telecomData.stepsRemoved = pPtpVirtualPort->virtualData.config.stepsRemoved;

      /* Copied from generic data */
      OS_MEMCPY(pBmcaDataS->telecomData.grandmasterIdentity,
             pPtpVirtualPort->virtualData.config.clockIdentity,
             PTP_V2_CLK_IDENTITY_LENGTH);
   }

   return status;
}  /* END zl303xx_PtpG8265p1BmcaVpDataAdd */

/*

  Function Name:
    zl303xx_PtpG8265p1ClockDebugShow

  Details:
   Routine to display G.8265.1, (Telecom Profile: Frequency) profile specific
   clock parameters.

  Parameters:
   [in]  pClockS  Pointer to the PTP clock data structure (as a void).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpG8265p1ClockDebugShow(void *pClockS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pClockS);

   /* De-reference the void pointer. Even if NULL, the status check below will fail. */
   zl303xx_PtpClockS *pPtpClock = (zl303xx_PtpClockS *)pClockS;

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
       zl303xx_PtpTelecomQualityLevelS ql;

       printf("\nG.8265.1 (Telecom Profile - FREQUENCY) information:\n");
       printf("----------------------------------------------------------\n");

       if (zl303xx_PtpG8265p1ClockQLGet(pPtpClock->clockHandle, &ql) == ZL303XX_OK)
       {
           printf("QL (Option I):   %s\n", zl303xx_PtpG781QLToStr(ql.g781[0]));
           printf("   (Option II):  %s\n", zl303xx_PtpG781QLToStr(ql.g781[1]));
           printf("   (Option III): %s\n", zl303xx_PtpG781QLToStr(ql.g781[2]));
       }

       if (zl303xx_PtpTimerActive(&pPtpClock->profileDS.telecom.qlHoldOffTimer))
           printf(" [HOLD OFF ACTIVE (%u/%u sec remaining)]",
                  (pPtpClock->profileDS.telecom.qlHoldOffTimer.interval.secondsField.lo -
                   pPtpClock->profileDS.telecom.qlHoldOffTimer.carryOver.secondsField.lo),
                  pPtpClock->profileDS.telecom.qlHoldOffTimer.interval.secondsField.lo);

       printf("Squelch event: %s\n", (pPtpClock->config.telecom.evtSquelchEn) ? "ENABLED" : "DISABLED");
       printf("Output clock(s): %s\n", (pPtpClock->profileDS.telecom.enableOutput) ? "ENABLED" : "SQUELCHED");
   }

   return status;
}  /* END zl303xx_PtpG8265p1ClockDebugShow */

/*****************   PROFILE BINDING FUNCTION DECLARATIONS (END) **************/

/*****************   API FUNCTIONS FOR PROFILE ATTRIBUTES   ********************/

/* zl303xx_PtpG8265p1ClockQLGet */
/**
   Telecom Profile only. Returns a quality level structure based on a clock's
   parentDS::grandmasterClockQuality::clockClass. If this clock is a slave with
   no qualified grandmasters, ZL303XX_G781_QL_DNU and ZL303XX_G781_QL_DUS will be
   returned in the structure.

  Parameters:
   [in]   clockHandle   Handle to an existing clock.
   [in]   ql            Returned QL structure.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    ql is NULL.
     ZL303XX_PARAMETER_INVALID  clockHandle is invalid.
     ZL303XX_INVALID_MODE       The clock is not running the G.8265.1 PTP profile.

*******************************************************************************/
zlStatusE zl303xx_PtpG8265p1ClockQLGet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpTelecomQualityLevelS *ql)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pClock;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(ql);
   }

   /* Get a pointer to the clock data (error if not using Telecom Profile). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGetG8265p1(clockHandle, &pClock);
   }

   /* Return the clock's QL value(s). */
   if (status == ZL303XX_OK)
   {
      *ql = *(pClock->profileDS.telecom.pQuality);
   }

   return status;
}  /* END zl303xx_PtpG8265p1ClockQLGet */

/* zl303xx_PtpClockWaitToRestoreSet */
/**
   Telecom Profile only. Sets a clock's wait-to-restore time.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   wtrSec       Wait-to-restore time, in seconds.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid.
     ZL303XX_INVALID_MODE          The clock is not running the G.8265.1 PTP profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockWaitToRestoreSet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint32T wtrSec)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pClock = NULL;
   zl303xx_PortDataS *pPort = NULL;
   zl303xx_ClockStreamS *pStream = NULL;

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGetG8265p1(clockHandle, &pClock);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pClock);
   }

   if (status == ZL303XX_OK)
   {
      /* Update the wait-to-restore value. */
      pClock->config.telecom.waitToRestoreSec = wtrSec;

      /* Update any active timers. */
      ZL303XX_PTP_FOR_EACH_PORT(&pClock->portList, pPort)
      {
         ZL303XX_PTP_FOR_EACH_STREAM(&pPort->streamList, pStream)
         {
            if ((pStream->pUmtEntry != NULL) &&
                (zl303xx_PtpTelecomWtrIsActive(&pStream->pUmtEntry->telecom)))
            {
               /* Stop all timers if wtrSec is 0. */
               if (wtrSec == 0)
               {
                  zl303xx_PtpTelecomWtrStop(&pStream->pUmtEntry->telecom);
               }
               /* Restart any timers that have > wtrSec time remaining. */
               else if (pStream->pUmtEntry->telecom.wtr > wtrSec)
               {
                  ZL303XX_TRACE_ALWAYS("Stream %lu: Reducing current WTR from %lu to %lu.",
                     pStream->clockStreamHandle, pStream->pUmtEntry->telecom.wtr, wtrSec, 0,0,0);

                  zl303xx_PtpTelecomWtrStart(pStream->clockStreamHandle,
                                             &pStream->pUmtEntry->telecom,
                                             wtrSec);
               }
            }
         }
      }

      status = zl303xx_PtpClockMutexUnlock(pClock, status);
   }

   return status;
}  /* END zl303xx_PtpClockWaitToRestoreSet */

/* zl303xx_PtpClockEvtSquelchEnSet */
/**
   Telecom Profile only. Sets the enabled flag do determine if the SQUELCH
   event will be generated.

  Parameters:
   [in]   clockHandle  Handle to an existing clock.
   [in]   enabled      Enabled flag to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid.
     ZL303XX_INVALID_MODE          The clock is not running the G.8265.1 PTP profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockEvtSquelchEnSet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_BooleanE enabled)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pClock;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(enabled);
   }

   /* Get a pointer to the clock data (error if not using Telecom Profile). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGetG8265p1(clockHandle, &pClock);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pClock);
   }

   if (status == ZL303XX_OK)
   {
      /* If the enabled flag is FALSE and the output clocks are currently disabled,
       * generate a SQUELCH event to re-enable them. */
      if ((enabled == ZL303XX_FALSE) && (pClock->profileDS.telecom.enableOutput == ZL303XX_FALSE))
      {
         zl303xx_PtpEventSquelch(pClock, ZL303XX_TRUE);
      }

      /* Set the enabled flag. */
      pClock->config.telecom.evtSquelchEn = enabled;

      /* Release the clock mutex. */
      status = zl303xx_PtpClockMutexUnlock(pClock, status);
   }

   return status;
}  /* END zl303xx_PtpClockEvtSquelchEnSet */

/* zl303xx_PtpClockQLHoldOffSet */
/**
   Telecom Profile only. Sets the QL hold off time for a clock when a previously
   failed master is restored. Does not affect the timer if this function is called
   while a QL hold off is in effect.

  Parameters:
   [in]   clockHandle   Handle to an existing clock.
   [in]   holdOffSec    QL hold off time, in seconds.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or enabled is invalid.
     ZL303XX_INVALID_MODE          The clock is not running the G.8265.1 PTP profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpClockQLHoldOffSet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint32T holdOffSec)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pClock;

   /* Get a pointer to the clock data (error if not using Telecom Profile). */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGetG8265p1(clockHandle, &pClock);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pClock);
   }

   if (status == ZL303XX_OK)
   {
      /* Set the QL hold off time. */
      pClock->config.telecom.qlHoldOffSec = holdOffSec;

      /* Release the clock mutex. */
      status = zl303xx_PtpClockMutexUnlock(pClock, status);
   }

   return status;
}  /* END  zl303xx_PtpClockQLHoldOffSet */

/**

  Function Name:
   zl303xx_DebugTelecomBMCA

  Details:
   Displays Tlecom BMCA clock data set statistics for the specified Clock

  Parameters:
   [in]
   clockHandle    Handle to the PTP clock for which the data sets are required.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugTelecomBMCA(zl303xx_PtpClockHandleT clockHandle)
{
   zlStatusE status = ZL303XX_OK;

   /* Pointer to the targeted clock. */
   zl303xx_PtpClockS *pPtpClock = NULL;
   zl303xx_DefaultDS *defaultDS = NULL;    /* 1588 Standard Default Data Set. */

   zl303xx_PtpTelecomQualityLevelS ql;

   if ((status = zl303xx_PtpClockDataGetG8265p1(clockHandle, &pPtpClock)) == ZL303XX_OK )
   {
      defaultDS = &(pPtpClock->config.defaultDS);

      printf("\nTelecom Profile information:\n");

      if (zl303xx_PtpG8265p1ClockQLGet(pPtpClock->clockHandle, &ql) == ZL303XX_OK)
      {
          printf("QL (Option I):   %s\n", zl303xx_PtpG781QLToStr(ql.g781[0]));
          printf("   (Option II):  %s\n", zl303xx_PtpG781QLToStr(ql.g781[1]));
          printf("   (Option III): %s\n", zl303xx_PtpG781QLToStr(ql.g781[2]));
      }

      if (zl303xx_PtpTimerActive(&pPtpClock->profileDS.telecom.qlHoldOffTimer))
          printf(" [HOLD OFF ACTIVE (%u/%u sec remaining)]",
                 (pPtpClock->profileDS.telecom.qlHoldOffTimer.interval.secondsField.lo -
                  pPtpClock->profileDS.telecom.qlHoldOffTimer.carryOver.secondsField.lo),
                 pPtpClock->profileDS.telecom.qlHoldOffTimer.interval.secondsField.lo);

      printf("Squelch event: %s\n", (pPtpClock->config.telecom.evtSquelchEn) ? "ENABLED" : "DISABLED");
      printf("Output clock(s): %s\n", (pPtpClock->profileDS.telecom.enableOutput) ? "ENABLED" : "SQUELCHED");

      printf ("\nSlave Only: %s\n", ZL303XX_BOOL_TO_STR(defaultDS->slaveOnly));
      printf ("\nRevertive BMCA: %s\n", ZL303XX_BOOL_TO_STR(pPtpClock->config.bmca.revertiveEn));
      printf ("\nBMCA Update Trigger: %d", pPtpClock->config.bmca.updateTrigger);
      printf ("\nAuto BMCA Update: %s (%d seconds)\n", ZL303XX_BOOL_TO_STR(pPtpClock->config.bmca.updateEventSec != 0), pPtpClock->config.bmca.updateEventSec);
      printf ("\nStream State Update Mode: %s\n", (pPtpClock->config.autoUpdateStreamStates) ? "AUTO" : "MANUAL");

      /* Ingress/Egress clockClass configuration.  */
      printf ("\nClock Ingress/Egress clockClass (QL) over-ride configuration:\n");
      printf (" Ingress clockClass over-ride: %d %s\n",
              pPtpClock->config.ingressClockClass,
              ((pPtpClock->config.ingressClockClass == 0) ? ":DISABLED" : " "));
      printf ("  Egress clockClass over-ride: %d %s\n",
              pPtpClock->config.egressClockClass,
              ((pPtpClock->config.egressClockClass == 0) ? ":DISABLED" : " "));
      printf ("     Egress clockClass update: %d (UPDATE WHEN %s)\n",
              pPtpClock->config.egressClassUpdateTrigger,
              ((pPtpClock->config.egressClassUpdateTrigger == ZL303XX_EGRESS_QL_UPDATE_ON_QUALIFY) ? "QUALIFIED" : "LOCKED"));
    }

    return status;
}  /* END zl303xx_DebugTelecomBMCA */

/*****************   API FUNCTIONS FOR PROFILE ATTRIBUTES (END)   *************/


/* zl303xx_PtpTelecomPtsfToStr */
/**
   Converts a PTSF enum value to a string.

  (Deprecated)  This function has been replaced by zl303xx_PtsfFlagToStr().

  Parameters:
   [in]   ptsf   PTSF enum value to convert.

  Return Value:  (const char *) The PTSF value as a string.

*******************************************************************************/
const char *zl303xx_PtpTelecomPtsfToStr(Sint32T ptsf)
{
   return zl303xx_PtsfFlagToStr(ptsf);
}

/* zl303xx_PtpG781QLToStr */
/**
   Converts a G.781 Quality Level enum value to a string.

  Parameters:
   [in]   ql   Quality Level to convert.

  Return Value:  (const char *) The G.781 QL value as a string.

*******************************************************************************/
const char *zl303xx_PtpG781QLToStr(zl303xx_PtpG781QualityLevelE ql)
{
   if (ql < ZL303XX_G781_QL_NUM_VALUES)
   {
      return Zl303xx_PtpQualityLevelStr[ql];
   }

   return Zl303xx_PtpQualityLevelStr[ZL303XX_G781_QL_NUM_VALUES];   /* Index of "ERROR" string. */
}

/*****************   INTERNAL PTP FUNCTION DEFINITIONS   **********************/

/* zl303xx_PtpTelecomUmtEntryStructInit */
/** 

   Initializes the Telecom Profile specific members of a zl303xx_PtpUmtEntryS
   structure (zl303xx_PtpTelecomUmtEntryS).

  Parameters:
   [out]  pAdd   The initialized structure.

*******************************************************************************/
void zl303xx_PtpTelecomUmtEntryStructInit(zl303xx_PtpTelecomUmtEntryS *pAdd)
{
   pAdd->priority = 255;
   pAdd->syncReceiptTimeoutSec = ZL303XX_PTP_DEFAULT_UMT_ENTRY_SYNC_RX_TIMEOUT;
   pAdd->followUpReceiptTimeoutSec = ZL303XX_PTP_DEFAULT_UMT_ENTRY_FOLLOW_RX_TIMEOUT;
   pAdd->syncOrFollowUpReceiptTimeoutSec = ZL303XX_PTP_DEFAULT_UMT_ENTRY_SOF_RX_TIMEOUT;
   pAdd->delayRespReceiptTimeoutSec = ZL303XX_PTP_DEFAULT_UMT_ENTRY_DRESP_RX_TIMEOUT;
   pAdd->pdelayRespReceiptTimeoutSec = ZL303XX_PTP_DEFAULT_UMT_ENTRY_PDRESP_RX_TIMEOUT;
   pAdd->pdelayFollowUpReceiptTimeoutSec = ZL303XX_PTP_DEFAULT_UMT_ENTRY_PDRESP_RX_TIMEOUT;

   pAdd->ptsfMask = 0;   /* Deprecated */
}

/* zl303xx_PtpTelecomUmtEntryStructCheck */
/** 

   Checks the Telecom Profile specific members of a zl303xx_PtpUmtEntryS
   structure (zl303xx_PtpTelecomUmtEntryS).

  Parameters:
   [in]   pEntry   The structure to check.

*******************************************************************************/
zlStatusE zl303xx_PtpTelecomUmtEntryStructCheck(zl303xx_PtpTelecomUmtEntryS *pEntry)
{
   /* No members are currently checked. */
   (void)pEntry;
   return ZL303XX_OK;
}

/* zl303xx_PtpTelecomUmtEntryAdd */
/** 

   Initializes the members of a zl303xx_PtpTelecomUmtEntryIntS structure.

  Parameters:
   [in]   pAdd    Telecom Profile data used to initialize the UMT entry.
   [out]  pEntry  Initialized UMT entry.

*******************************************************************************/
void zl303xx_PtpTelecomUmtEntryIntAdd(zl303xx_PtpTelecomUmtEntryS *pAdd,
                                      zl303xx_PtpTelecomUmtEntryIntS *pEntry)
{
   OS_MEMSET(pEntry, 0, sizeof(*pEntry));

   pEntry->priority                = pAdd->priority;
   pEntry->pQuality                = &Zl303xx_PtpClockClassToQL[QL_INVALID_INDEX];
   pEntry->syncReceiptTimeoutSec   = pAdd->syncReceiptTimeoutSec;
   pEntry->syncTimeoutCounter      = pAdd->syncReceiptTimeoutSec;
   pEntry->followReceiptTimeoutSec = pAdd->followUpReceiptTimeoutSec;
   pEntry->followTimeoutCounter    = pAdd->followUpReceiptTimeoutSec;
   pEntry->sofReceiptTimeoutSec    = pAdd->syncOrFollowUpReceiptTimeoutSec;
   pEntry->sofTimeoutCounter       = pAdd->syncOrFollowUpReceiptTimeoutSec;
   pEntry->delayReceiptTimeoutSec  = pAdd->delayRespReceiptTimeoutSec;
   pEntry->delayTimeoutCounter     = pAdd->delayRespReceiptTimeoutSec;
   pEntry->pdelayReceiptTimeoutSec = pAdd->pdelayRespReceiptTimeoutSec;
   pEntry->pdelayTimeoutCounter    = pAdd->pdelayRespReceiptTimeoutSec;
   pEntry->pdelayFollowReceiptTimeoutSec = pAdd->pdelayFollowUpReceiptTimeoutSec;
   pEntry->pdelayFollowTimeoutCounter    = pAdd->pdelayFollowUpReceiptTimeoutSec;
   pEntry->unusableTimeoutSec      = PTSF_UNUSABLE_TIMEOUT_SEC;
   pEntry->unusableTimeoutCounter  = PTSF_UNUSABLE_TIMEOUT_SEC;

   /* Allow a master to be selected immediately when first seen. */
   pEntry->wtr            = 0;
   pEntry->wtrActive      = ZL303XX_FALSE;
   pEntry->bestMasterOnce = ZL303XX_FALSE;
}

/* zl303xx_PtpClockClassToQL */
/** 

   Converts a PTP clockClass value to a quality level structure.

  Parameters:
   [in]   clockClass   PTP clockClass to convert.

*******************************************************************************/
const zl303xx_PtpTelecomQualityLevelS *zl303xx_PtpClockClassToQL(Uint8T clockClass)
{
   Uint8T arrayIndex;

   /* Bad clock classes: odd numbers, less than 80, greater than 110 */
   if (ZL303XX_PTP_TELECOM_CLOCK_CLASS_CHECK(clockClass) == ZL303XX_OK)
   {
      arrayIndex = (clockClass - 80) / 2;
   }
   else if (clockClass == PTP_SLAVE_ONLY_CLOCK_CLASS)
   {
      arrayIndex = QL_SLAVE_INDEX;
   }
   else
   {
      arrayIndex = QL_INVALID_INDEX;
   }

   return &Zl303xx_PtpClockClassToQL[arrayIndex];
}


/* zl303xx_PtpTelecomBmcaDataUpdate */
/** 

   Updates the Telecom Profile quality level pointer based on PTP clockClass.

  Parameters:
   [in]   pEntry        Pointer to Telecom Profile data from a UMT entry.
   [in]   clockClass    PTP clockClass received in an ANNOUNCE message.
   [in]   stepsRemoved  PTP stepsRemoved received in an ANNOUNCE message.

*******************************************************************************/
void zl303xx_PtpTelecomBmcaDataUpdate(zl303xx_PtpTelecomUmtEntryIntS *pEntry,
                                      Uint8T clockClass, Uint16T stepsRemoved)
{
   Uint8T arrayIndex = (ZL303XX_PTP_TELECOM_CLOCK_CLASS_CHECK(clockClass) == ZL303XX_OK)
                          ? ((clockClass - 80) / 2)
                          : (Uint8T)(QL_INVALID_INDEX);

   pEntry->pQuality = &Zl303xx_PtpClockClassToQL[arrayIndex];
   pEntry->clockClass = clockClass;
   pEntry->stepsRemoved = stepsRemoved;
}

/* zl303xx_PtpSsmQLToClockClass */
/** 

   Converts an SSM quality level value to a PTP clockClass.

  Parameters:
   [in]   ssmQL         SSM QL to convert.
   [out]  pClockClass   Equivalent PTP clockClass.

*******************************************************************************/
zlStatusE zl303xx_PtpSsmQLToClockClass(Uint8T ssmQL, Uint8T *pClockClass)
{
   if (ssmQL > 0xF)
   {
      ZL303XX_TRACE_ALWAYS("Could not convert SSM QL=%u to a clockClass.",
                           ssmQL, 0,0,0,0,0);
      return ZL303XX_PARAMETER_INVALID;
   }

   *pClockClass = Zl303xx_PtpQLToClockClass[ssmQL];

   return ZL303XX_OK;
}

/* zl303xx_PtpTelecomLogIntvlCheck */
/** 

   Checks if the log2 interval value for a given message type is within the
   range supported by the Telecom Profile.

  Parameters:
   [in]   type      Message type to check.
   [in]   logIntvl  Pointer to the log2 of the interval to check.
   [in]   retError  ZL303XX_TRUE to return an error status if logIntvl is out
                         of range. Otherwise, ZL303XX_OK is returned with extra
                         logging.

  Notes:  An error status will always be returned when message type is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpTelecomLogIntvlCheck(zl303xx_MessageTypeE type,
                                          Sint8T *logIntvl,
                                          zl303xx_BooleanE retError)
{
   /* Check Input pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(logIntvl);

   const char *fnName = "zl303xx_PtpTelecomLogIntvlCheck";
   const char *msgTypeStr = zl303xx_PtpMessageTypeToStr(type);

   /* Set the default range to the outer limits for messageTypes that have no
    * constraints.  */
   Sint8T min = PTP_MESSAGE_INTERVAL_MIN, max = PTP_MESSAGE_INTERVAL_MAX;

   if (status == ZL303XX_OK)
   {
      switch (type)
      {
         case ZL303XX_MSG_ID_SYNC :
            min = ZL303XX_PTP_TELECOM_LOG_SYNC_MIN;
            max = ZL303XX_PTP_TELECOM_LOG_SYNC_MAX;
            break;

         case ZL303XX_MSG_ID_DELAY_REQ :
         case ZL303XX_MSG_ID_DELAY_RESP :
            min = ZL303XX_PTP_TELECOM_LOG_DELAY_MIN;
            max = ZL303XX_PTP_TELECOM_LOG_DELAY_MAX;
            break;

         case ZL303XX_MSG_ID_ANNOUNCE :
            min = ZL303XX_PTP_TELECOM_LOG_ANNC_MIN;
            max = ZL303XX_PTP_TELECOM_LOG_ANNC_MAX;
            break;

         /* There are no limits on the following messageTypes. */
         case ZL303XX_MSG_ID_PEER_DELAY_REQ :
         case ZL303XX_MSG_ID_PEER_DELAY_RESP :
         case ZL303XX_MSG_ID_FOLLOWUP :
         case ZL303XX_MSG_ID_PEER_DELAY_FOLLOWUP :
         case ZL303XX_MSG_ID_SIGNALING :
         case ZL303XX_MSG_ID_MANAGEMENT :
            break;

         default :
            ZL303XX_TRACE_ERROR("%s: Message type %s (0x%X) is invalid.",
                                fnName, msgTypeStr, type, 0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
            break;
      }
   }

   if (status == ZL303XX_OK)
   {
      if ((*logIntvl < min) || (*logIntvl > max))
      {
         ZL303XX_TRACE_ERROR("%s: Invalid log2 %s interval=%d (G.8265.1 valid range: %d to %d).",
                             fnName, msgTypeStr, *logIntvl, min, max, 0);
         status = zl303xx_PtpTelecomLogRangeError(retError);
      }
   }

   return status;
}

/* zl303xx_PtpTelecomMsgTimeoutCheck */
/** 

   Ensures that the minimum timeout value used to raise a PTSF-loss* flag is at
   least twice the negotiated interval.

  Parameters:
   [in]   pEntry   Pointer to UMT entry Telecom data.
   [in]   msgType  Negotiated message type.
   [in]   intvl    Negotiated log2 message interval.
   [in]   pConfig  Pointer to UMT entry Telecom configuration.

*******************************************************************************/
void zl303xx_PtpTelecomMsgTimeoutCheck(zl303xx_PtpTelecomUmtEntryIntS *pEntry,
                                       zl303xx_MessageTypeE msgType,
                                       Sint8T intvl,
                                       zl303xx_PtpTelecomUmtEntryS *pConfig)
{
   Uint32T minTimeout = 0;

   /* Ignore intervals less than 1 second */
   if (intvl >= 0)
      minTimeout = 2 << intvl;

   if (msgType == ZL303XX_MSG_ID_SYNC)
   {
      pEntry->syncReceiptTimeoutSec = max(pConfig->syncReceiptTimeoutSec, minTimeout);
      pEntry->sofReceiptTimeoutSec = max(pConfig->syncOrFollowUpReceiptTimeoutSec, minTimeout);
      pEntry->followReceiptTimeoutSec = max(pConfig->followUpReceiptTimeoutSec, minTimeout);

      /* If the timer has not yet expired, reset the timeout counter */
      if (pEntry->syncTimeoutCounter > 0)
         pEntry->syncTimeoutCounter = pEntry->syncReceiptTimeoutSec;
      if (pEntry->sofTimeoutCounter > 0)
         pEntry->sofTimeoutCounter = pEntry->sofReceiptTimeoutSec;
      if (pEntry->followTimeoutCounter > 0)
         pEntry->followTimeoutCounter = pEntry->followReceiptTimeoutSec;

   }
   else if (msgType == ZL303XX_MSG_ID_DELAY_RESP)
   {
      pEntry->delayReceiptTimeoutSec = max(pConfig->delayRespReceiptTimeoutSec, minTimeout);

      if (pEntry->delayTimeoutCounter > 0)
         pEntry->delayTimeoutCounter = pEntry->delayReceiptTimeoutSec;
   }

   else if (msgType == ZL303XX_MSG_ID_PEER_DELAY_RESP)
   {
      pEntry->pdelayReceiptTimeoutSec = max(pConfig->pdelayRespReceiptTimeoutSec, minTimeout);
      pEntry->pdelayFollowReceiptTimeoutSec = max(pConfig->pdelayFollowUpReceiptTimeoutSec, minTimeout);

      if (pEntry->pdelayTimeoutCounter > 0)
         pEntry->pdelayTimeoutCounter = pEntry->pdelayReceiptTimeoutSec;
      if (pEntry->pdelayFollowTimeoutCounter > 0)
         pEntry->pdelayFollowTimeoutCounter = pEntry->pdelayFollowReceiptTimeoutSec;
   }
}

/* zl303xx_PtpTelecomPtsfEval */
/** 

   Evaluates the PTSF member of a Unicast Master Table entry and makes any
   changes based on the request-flags.

  Parameters:
   [in]   streamHandle  Handle to a previously created stream.
   [in]   reqAnnc       UMT entry requesting Announce contract.
   [in]   reqSync       UMT entry requesting Sync contract.
   [in]   reqDelayResp  UMT entry requesting Delay_Resp contract.
   [in]   reqPdelayResp UMT entry requesting Delay_Resp contract.

*******************************************************************************/
void zl303xx_PtpTelecomPtsfEval(Uint32T streamHandle, zl303xx_BooleanE reqAnnc,
                                zl303xx_BooleanE reqSync, zl303xx_BooleanE reqDelayResp,
                                zl303xx_BooleanE reqPdelayResp)
{
   if (!reqAnnc)
   {
      /* Clear all PTSF-timeOut flags. */
      zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_LOSS_ANNOUNCE,             ZL303XX_FALSE);
      zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_LOSS_SYNC,                 ZL303XX_FALSE);
      zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_LOSS_FOLLOW_UP,            ZL303XX_FALSE);
      zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_LOSS_SYNC_OR_FOLLOW_UP,    ZL303XX_FALSE);
      zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_LOSS_DELAY_RESP,           ZL303XX_FALSE);
      zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_LOSS_PEER_DELAY_RESP,      ZL303XX_FALSE);
      zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_LOSS_PEER_DELAY_FOLLOW_UP, ZL303XX_FALSE);
   }
   else
   {
      if (!reqSync)
      {
         zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_LOSS_SYNC,              ZL303XX_FALSE);
         zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_LOSS_FOLLOW_UP,         ZL303XX_FALSE);
         zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_LOSS_SYNC_OR_FOLLOW_UP, ZL303XX_FALSE);
      }
      if (!reqDelayResp)
      {
         zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_LOSS_DELAY_RESP, ZL303XX_FALSE);
      }
      if (!reqPdelayResp)
      {
         zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_LOSS_PEER_DELAY_RESP,      ZL303XX_FALSE);
         zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_LOSS_PEER_DELAY_FOLLOW_UP, ZL303XX_FALSE);
      }
   }
}

zl303xx_BooleanE zl303xx_PtpTelecomUmtEntryQualified(zl303xx_ClockStreamS *pStream)
{
   if (pStream->pUmtEntry)
   {
      return ((!zl303xx_PtsfFlagGetAndMask(pStream->clockStreamHandle, ZL303XX_PTSF_MAIN)) &&
              (pStream->pUmtEntry->telecom.pQuality->g781[0] != ZL303XX_G781_QL_DNU) &&
              (pStream->pUmtEntry->telecom.pQuality->g781[0] != ZL303XX_G781_QL_DUS));
   }
   else
   {
      return (!zl303xx_PtsfFlagGetAndMask(pStream->clockStreamHandle, ZL303XX_PTSF_MAIN));
   }
}

/* zl303xx_PtpTelecomStreamBestMasterOnce */
/** 

   Marks a stream as having been a best master at least once by the BMCA module.

  Parameters:
   [in]   pEntry   Pointer to UMT entry Telecom data.

*******************************************************************************/
void zl303xx_PtpTelecomStreamBestMasterOnce(zl303xx_PtpTelecomUmtEntryIntS *pEntry)
{
   pEntry->bestMasterOnce = ZL303XX_TRUE;
}

/* zl303xx_PtpTelecomWtrStart */
/** 

   Starts the wait to restore timer for a stream, indicating it cannot be
   selected by the BMCA.

  Parameters:
   [in]   streamHandle   Handle to an existing stream.
   [in]   pEntry         Pointer to UMT entry Telecom data.
   [in]   wtrSec         Countdown timer duration.

*******************************************************************************/
void zl303xx_PtpTelecomWtrStart(Uint32T streamHandle,
                                zl303xx_PtpTelecomUmtEntryIntS *pEntry,
                                Uint32T wtrSec)
{
   if (pEntry->bestMasterOnce)
   {
      pEntry->wtr = wtrSec;
      pEntry->wtrActive = ZL303XX_TRUE;

      if (pEntry->wtr > 0)
      {
         zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_WAIT_TO_RESTORE_ACTIVE, ZL303XX_TRUE);

         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 1,
               "Stream=%lu: (G.8265.1) Wait to Restore timer started (%lu sec).",
               streamHandle, wtrSec, 0,0,0,0);
      }
   }
}

/* zl303xx_PtpTelecomWtrStop */
/** 

   Stops the WTR timer for a UMT entry.

  Parameters:
   [in]   pEntry   Pointer to UMT entry Telecom data.

*******************************************************************************/
void zl303xx_PtpTelecomWtrStop(zl303xx_PtpTelecomUmtEntryIntS *pEntry)
{
   pEntry->wtrActive = ZL303XX_FALSE;
}

/* zl303xx_PtpTelecomWtrIsActive */
/** 

   Returns TRUE if the WTR timer for a UMT entry is active.

  Parameters:
   [in]   pEntry   Pointer to UMT entry Telecom data.

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtpTelecomWtrIsActive(zl303xx_PtpTelecomUmtEntryIntS *pEntry)
{
   return pEntry->wtrActive;
}

/* zl303xx_PtpTelecomWtrExpired */
/** 

   Returns TRUE if the WTR timer for a UMT entry is inactive or is active and
   has expired.

  Parameters:
   [in]   pEntry   Pointer to UMT entry Telecom data.

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtpTelecomWtrExpired(zl303xx_PtpTelecomUmtEntryIntS *pEntry)
{
   return (!pEntry->wtrActive || (pEntry->wtr == 0));
}

/* zl303xx_PtpTelecomWtrUpdate */
/** 

   Updates the wait to restore timer for a stream. Must be called once per second.

  Parameters:
   [in]   streamHandle   Handle to an existing stream.
   [in]   pEntry         Pointer to UMT entry Telecom data.

*******************************************************************************/
void zl303xx_PtpTelecomWtrUpdate(Uint32T streamHandle,
                                 zl303xx_PtpTelecomUmtEntryIntS *pEntry)
{
   /* Don't update the timer if it is stopped. */
   if ((pEntry->wtr > 0) && pEntry->wtrActive)
   {
      pEntry->wtr--;

      /* Print out WTR time remaining every 10 sec. */
      if ((pEntry->wtr % 10) == 0)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 2,
               "Stream %lu: (G.8265.1) Wait to Restore expires in %lu sec.",
               streamHandle, pEntry->wtr, 0,0,0,0);
      }
   }
   else
   {
      zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_WAIT_TO_RESTORE_ACTIVE, ZL303XX_FALSE);
   }
}

/* zl303xx_PtpTelecomSofTimerReset */
/** 

   Resets the timer used to generate PTSF-lossSyncOrFollowUp.

  Parameters:
   [in]   stream  Pointer to an existing stream.

*******************************************************************************/
void zl303xx_PtpTelecomSofTimerReset(zl303xx_ClockStreamS *stream)
{
   if (stream->pUmtEntry != NULL)
   {
      stream->pUmtEntry->telecom.sofTimeoutCounter =
         stream->pUmtEntry->telecom.sofReceiptTimeoutSec;
   }
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/* zl303xx_PtpTelecomLogRangeError */
/** 

   Performs extra logging if no error return code is desired.

  Parameters:
   [in]  retError  ZL303XX_FALSE if the calling function should not return an
                        error code.

*******************************************************************************/
static zlStatusE zl303xx_PtpTelecomLogRangeError(zl303xx_BooleanE retError)
{
   if (retError == ZL303XX_FALSE)
   {
      ZL303XX_TRACE_ALWAYS(
         "Parameter range violated. Continuing with unsupported mode.",
         0,0,0,0,0,0);
      return ZL303XX_OK;
   }

   return ZL303XX_PARAMETER_INVALID;
}

/*****************   END   ****************************************************/
