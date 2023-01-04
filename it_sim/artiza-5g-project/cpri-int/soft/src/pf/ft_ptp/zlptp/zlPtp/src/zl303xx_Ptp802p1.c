

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This module contains functions required to implement the
*     IEEE802.1AS Standard (TSN Profile).
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"
#include "zl303xx_Error.h"
#include "zl303xx_Ptsf.h"

#include "zl303xx_Ptp802p1Api.h"
#include "zl303xx_Ptp802p1Types.h"
#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpFunctionExport.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_Ptp_dep.h"
#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpProfileDef.h"
#include "zl303xx_PtpV2TlvApi.h"
#include "zl303xx_PtpV2MsgParse.h"



/*****************   DEFINES     **********************************************/

/* Profile Specific TLV Extension definitions. */
#define ZL303XX_PTP_802_P1_TLV_MSG_INTVL_REQ             (Uint8T)(0x02)

/* Size of the MESSAGE INTERVAL REQ TLV. This is the required value in the
 * lengthField of the TLV (starts with the organizationId field). */
#define ZL303XX_PTP_802_P1_TLV_MSG_INTVL_REQ_TTL_LEN     12
#define ZL303XX_PTP_802_P1_TLV_MSG_INTVL_REQ_DATA_LEN    6

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/* PROFILE OUI. */
Uint8T Zl303xx_Ptp802p1_OUI[] = {0x00, 0x80, 0xC2};

const zl303xx_PtpProfileS Zl303xx_Ptp802p1v1ProfileInfo = {
   "IEEE 802.1AS PTP profile for transport of timing over full-duplex, point-to-point links",
   "Version 1.0",
   {0x00, 0x80, 0xC2, 0x00, 0x01, 0x00},
   "This profile is specified by IEEE",
   "A copy may be obtained from www.ieee.org"
};

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/
/* Local routines to Unpack/Pack Profile specific TLVs. */
static Uint16T zl303xx_Ptp802p1MsgIntvlReqTlvUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_Ptp802p1MsgIntvlReqTlvS *pOutTlvDataS);

Uint16T zl303xx_Ptp802p1MsgIntvlReqTlvPack(
      zl303xx_Ptp802p1MsgIntvlReqTlvS *pInTlvDataS,
      Uint8T *pOutTlvBuf);

/* Local Routines to set all the time intervals sent in the message
 * interval request TLV
 */
static Sint8T zl303xx_Ptp802p1LogMsgIntvlDecode(Sint8T log2Intvl, Sint8T log2DefIntvl);

static zlStatusE zl303xx_Ptp802p1LogPdelayIntvlSet(zl303xx_PtpStreamHandleT streamHandle,
                                                 Sint8T logPdelayIntvl);

static zlStatusE zl303xx_Ptp802p1LogAnnounceIntvlSet(zl303xx_PtpStreamHandleT streamHandle,
                                                   Sint8T logAnncIntvl);

static zlStatusE zl303xx_Ptp802p1LogSyncIntvlSet(zl303xx_PtpStreamHandleT streamHandle,
                                                Sint8T logSyncIntvl);


/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/
/**
    zl303xx_Ptp802p1v1ProfileInfoGet

  Details:
   Retrieves the profile description information for the 802.1AS profile version 1 (2011).

  Parameters:
   [in,out]  profileInfo    Pointer to the structure to store the profile information.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

 *****************************************************************************/
zlStatusE zl303xx_Ptp802p1v1ProfileInfoGet(const zl303xx_PtpProfileS **profileInfo)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(profileInfo);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      *profileInfo = &Zl303xx_Ptp802p1v1ProfileInfo;
   }

   return status;
}

/*
  Function Name:
    zl303xx_Ptp802p1v1ClockDefaults

  Details:
   Routine to set the 802.1AS-2011, (TSN Profile) profile specific clock
   parameters to their default values.

  Parameters:
   [in]  pClockInitS    Pointer to the PTP creation data structure for the
                              clock (from the PtpApiTypes.h module).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pClockInitS is NULL.

*******************************************************************************/
zlStatusE zl303xx_Ptp802p1v1ClockDefaults(void *pClockInitS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pClockInitS);
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpClockCreateS *pClockCreate = (zl303xx_PtpClockCreateS *)pClockInitS;
      /* if gmCapable is true then clock class is set to 802.1 default.
       * if gmCapable is false then clock class should be set to slave only
       * clock class
       */
      if(pClockCreate->profileCfg._802p1.gmCapable == ZL303XX_TRUE)
      {
         pClockCreate->defaultDS.clockQuality.clockClass = ZL303XX_PTP_802P1_GM_CAPABLE_CLASS;
         /* Priority 1 is set to Network infrastructure time-aware systems by default */
         pClockCreate->defaultDS.priority1 = ZL303XX_PTP_802P1_CLOCK_PRI1_NITS;
      }
      else
      {
         pClockCreate->defaultDS.clockQuality.clockClass = ZL303XX_PTP_802P1_NOT_GM_CAPABLE_CLASS;
         /* Priority 1 is set to indicate the system is not gmCapable */
         pClockCreate->defaultDS.priority1 = ZL303XX_PTP_802P1_CLOCK_PRI1_NO_GM;
      }
      /* Domain Number */
      pClockCreate->defaultDS.domainNumber = ZL303XX_PTP_802P1_DOMAIN_NUM_DEF;
      /* Priority 2 is static and set at the start */
      pClockCreate->defaultDS.priority2 = ZL303XX_PTP_802P1_CLOCK_PRI2_DEF;
      /* All clocks are two step clocks */
      pClockCreate->defaultDS.twoStepFlag = ZL303XX_TRUE;
      /* Path Trace Functionality is enabled*/
      pClockCreate->pathTrace.enabled = ZL303XX_TRUE;
      /* Set the clockAccuracy and Variance to profile default */
      pClockCreate->defaultDS.clockQuality.clockAccuracy = ZL303XX_PTP_802P1_ACCURACY_DEF;
      pClockCreate->defaultDS.clockQuality.offsetScaledLogVariance = ZL303XX_PTP_802P1_VARIANCE_DEF;
   }

   return status;
}  /* END zl303xx_Ptp802p1v1ClockDefaults */

/*
  Function Name:
    zl303xx_Ptp802p1v1PortDefaults

  Details:
   Routine to set the 802.1AS (TSN Profile) specific port parameters
   to their default values.

  Parameters:
   [in]  pPortInitS     Pointer to the PTP creation data structure for the
                              port (from the PtpApiTypes.h module). This is
                              a void type so that all PTP Object types can use
                              the same interface.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   pPortInitS is NULL.

*******************************************************************************/
zlStatusE zl303xx_Ptp802p1v1PortDefaults (void *pPortInitS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pPortInitS);
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpPortCreateS *pPortCreate = (zl303xx_PtpPortCreateS *)pPortInitS;

      /* delayMechanism */
      pPortCreate->delayMechanism = ZL303XX_DELAY_MECHANISM_P2P;
      /* logMinPdelayReqInterval */
      pPortCreate->logMinPdelayReqInterval = ZL303XX_PTP_802P1_LOG_PDELAY_DEF;
      /* Raw T2 in Response, Raw T3 in Follow_up */
      pPortCreate->pdRespTxMethod = ZL303XX_PTP_PDELAY_METHOD_2_STEP_RAW;
      /* Turn unicast neg off */
      pPortCreate->mixedMode  = ZL303XX_FALSE;
      pPortCreate->unicastNegotiation.enabled = ZL303XX_FALSE;
      /* Rate Ratio Calculation should be enabled for this profile*/
      pPortCreate->rateRatioEnabled = ZL303XX_TRUE;
      /* Start with 1 packet for Rate ratio calculations */
      pPortCreate->numRateRatioPkts = ZL303XX_PTP_NUM_RATE_RATIO_1;
      /* Set allowed lost responses to default */
      pPortCreate->allowedLostResponses = ZL303XX_PTP_802P1_LOST_RESP_DEF;
   }
   return status;
} /* zl303xx_Ptp802p1v1PortDefaults */

/*
  Function Name:
    zl303xx_Ptp802p1v1StreamDefaults

  Details:
   Routine to set the 802.1AS (TSN Profile) specific stream parameters
   to their default values.

  Parameters:
   [in]  pStreamInitS   Pointer to the PTP creation data structure for the
                              stream (from the PtpApiTypes.h module). This is
                              a void type so that all PTP Object types can use
                              the same interface.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pStreamInitS is NULL.

*******************************************************************************/
zlStatusE zl303xx_Ptp802p1v1StreamDefaults (void *pStreamInitS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pStreamInitS);

   if (status == ZL303XX_OK)
   {
      zl303xx_PtpStreamCreateS *pStreamCreate = (zl303xx_PtpStreamCreateS *)pStreamInitS;

      /* Common PTP Stream Attributes. */
      /*********************************/
      pStreamCreate->logAnnounceInterval     = ZL303XX_PTP_802P1_LOG_ANNC_DEF;
      pStreamCreate->logSyncInterval         = ZL303XX_PTP_802P1_LOG_SYNC_DEF;
      pStreamCreate->logMinPdelayReqInterval = ZL303XX_PTP_802P1_LOG_PDELAY_DEF;
      pStreamCreate->announceReceiptTimeout  = ZL303XX_PTP_802P1_ANNC_TIMEOUT_DEF;
   }

   return status;
}

/*

  Function Name:
    zl303xx_Ptp802p1ClockCreateActions

  Details:
   Routine to perform 802.1AS (TSN Profile) actions related to clock
   creation. This routine is BIND'ed (bound) to the ProfileDef list and gets
   called at the end of the core Clock Creation process.
   (See zl303xx_PtpClockCreate()).

   For this profile, the Clock is configured to have a custom tlv handler.
   Certain BMCA Flags are also turned off.

  Parameters:
   [in]  pClockS  Pointer to the PTP clock data structure (as a void).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pClockS is NULL.

*******************************************************************************/
zlStatusE zl303xx_Ptp802p1ClockCreateActions(void *pClockS)
{
   Uint32T oui32;
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pClockS);


   if (status == ZL303XX_OK)
   {
      zl303xx_PtpClockS *pPtpClock = (zl303xx_PtpClockS *)pClockS;
      /* Set the Profile Specific Clock Parameters to defaults */
      pPtpClock->profileDS._802p1.lastGmPhaseChange.hi = 0;
      pPtpClock->profileDS._802p1.lastGmPhaseChange.lo = 0;
      pPtpClock->profileDS._802p1.lastGmFreqChange = 0;
      pPtpClock->profileDS._802p1.gmTimeBaseIndicator = 0;
      pPtpClock->profileDS._802p1.gmRateRatio = 1;
      pPtpClock->profileDS._802p1.syntonizedPll = ZL303XX_FALSE;

      /* Convert the OUI to an 'ID' for the lookup table. */
      (void)zl303xx_PtpTlvConvert3ByteFieldTo32Bit(Zl303xx_Ptp802p1_OUI, &oui32);

      /* Add the custom TLV handler for this profile. */
      status = zl303xx_PtpTlvCustomHandlerSet(
            pPtpClock->clockHandle,
            PTP_TLV_VENDOR_EXTENSION,
            oui32,
            zl303xx_Ptp802p1TlvHandle);

      /* Clear the BMCA Flags not needed for this profile */
      ZL303XX_BIT_CLEAR(pPtpClock->bmcaFlags,STATE_FM_QUALIFICATION_FLAG);
      ZL303XX_BIT_CLEAR(pPtpClock->bmcaFlags,PP_FLAG);
      ZL303XX_BIT_CLEAR(pPtpClock->bmcaFlags,PU_FLAG);
   }

   return status;
}

/*

  Function Name:
    zl303xx_Ptp802p1TlvAppend

  Details:
   Appends all necessary IEEE802.1, TSN Profile specific TLVs into
   the buffer provided based on transmit settings.

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

*******************************************************************************/
Uint16T zl303xx_Ptp802p1TlvAppend(
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
            "zl303xx_Ptp802p1TlvAppend: ERROR: NULL Pointer (%p, %p, %p)",
            pPtpStream, pOutMsgBuf, pOutTlvBuf, 0,0,0);
       return 0;
   }

   if (pPtpStream->portData->clock->config.profile == ZL303XX_PTP_PROFILE_TSN_802_1_2011)
   {
      switch (messageType)
      {
         case ZL303XX_MSG_ID_SIGNALING:
         {
            Uint8T Zl303xx_Ptp802p1MsgIntvlReqTlvSubType[] = {0x00, 0x00, 0x02};
            /* Temp variable to avoid compiler warning. */
            Uint8T profileIdent[PTP_PROFILE_IDENTITY_LENGTH];
            OS_MEMCPY(profileIdent, Zl303xx_Ptp802p1v1ProfileInfo.profileIdentifier, PTP_PROFILE_IDENTITY_LENGTH);

            tlvBytes = zl303xx_PtpTlvOrganizationHeaderPack(
                  profileIdent,
                  Zl303xx_Ptp802p1MsgIntvlReqTlvSubType,
                  pOutTlvBuf);

            tlvBytes += zl303xx_Ptp802p1MsgIntvlReqTlvPack(
                  &pPtpStream->portData->config.profileCfg._802p1.msgIntvlReq,
                  (pOutTlvBuf + tlvBytes));

            /* Add the length within this ORG_EXT TLV if a proper version was specified. */
            /* length = (Ttl - 4 bytes of Type & Length) */
            if (tlvBytes > 0)
            {
               zl303xx_PtpTlvLengthSet((tlvBytes - ZL303XX_PTP_TLV_VALUE_OFFSET), pOutTlvBuf);
            }
            break;
         }
         case ZL303XX_MSG_ID_PEER_DELAY_FOLLOWUP:
         {
            /* TO DO: Add code once support for follow up information
             * tlv is added. Just set the tlvbytes to zero for now.
             */
            tlvBytes = 0;
            break;
         }
         default:
         {
            /* Do not do anything for other message types */
            break;
         }
      }

   }

   return tlvBytes;
}

/*

  Function Name:
    zl303xx_Ptp802p1ClockDebugShow

  Details:
   Routine to display IEEE802.1AS (TSN Profile) specific clock parameters.

  Parameters:
   [in]  pClockS  Pointer to the PTP clock data structure (as a void).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pClockS is NULL.

*******************************************************************************/
zlStatusE zl303xx_Ptp802p1ClockDebugShow(void *pClockS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pClockS);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpClockS *pPtpClockS = (zl303xx_PtpClockS *)pClockS;

      printf ("\nIEEE802.1AS (TSN) Profile: Clock Data\n");
      printf ("-----------------------------------------------------\n");
      printf ("             GM Capable: %s \n",
              (pPtpClockS->config.profileCfg._802p1.gmCapable ? "TRUE" : "FALSE"));
      printf ("             lastGmPhaseChange: 0x%016x%08x \n",
            pPtpClockS->profileDS._802p1.lastGmPhaseChange);
      printf ("             lastGmFreqChange: %li \n",
            pPtpClockS->profileDS._802p1.lastGmFreqChange);
      printf ("             gmTimeBaseIndicator: %u \n",
            pPtpClockS->profileDS._802p1.gmTimeBaseIndicator);
      printf ("             gmRateRatio: %lu \n",
                  pPtpClockS->profileDS._802p1.gmRateRatio);
      printf ("             Syntonized Pll: %s \n",
              (pPtpClockS->profileDS._802p1.syntonizedPll ? "TRUE" : "FALSE"));
   }
   return status;
}  /* END zl303xx_Ptp802p1ClockDebugShow */

/*

  Function Name:
    zl303xx_Ptp802p1PortDebugShow

  Details:
   Routine to display IEEE802.1AS (TSN Profile) specific port parameters.

  Parameters:
   [in]  pPortS  Pointer to the PTP port data structure (as a void).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pPortS is NULL.

*******************************************************************************/
zlStatusE zl303xx_Ptp802p1PortDebugShow(void *pPortS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pPortS);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PortDataS *pPtpPortS = (zl303xx_PortDataS *)pPortS;

      printf ("\nMessage Interval Request Parameters:\n");
      printf ("          Link Delay Interval: %i \n",
            pPtpPortS->config.profileCfg._802p1.msgIntvlReq.linkDelayIntvl);
      printf ("          Sync Interval: %i \n",
                  pPtpPortS->config.profileCfg._802p1.msgIntvlReq.timeSyncIntvl);
      printf ("          Announce Interval: %i \n",
                      pPtpPortS->config.profileCfg._802p1.msgIntvlReq.announceIntvl);
      printf ("          Flag bits: 0x%02x \n",
                      pPtpPortS->config.profileCfg._802p1.msgIntvlReq.flags);
   }

   return status;
}  /* END zl303xx_Ptp802p1PortDebugShow */

/*

  Function Name:
    zl303xx_Ptp802p1StreamDebugShow

  Details:
   Routine to display IEEE802.1AS (TSN Profile) profile specific Stream
   parameters.

  Parameters:
   [in]  pStreamS  Pointer to the PTP Stream data structure (as a void).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pStreamS is NULL.

*******************************************************************************/
zlStatusE zl303xx_Ptp802p1StreamDebugShow(void *pStreamS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pStreamS);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      printf ("\nIEEE802.1AS (TSN) Profile Stream Data\n");
      printf ("--------------------------------------\n");
      printf ("  >> No data to display <<\n");
   }

   return status;
}  /* END zl303xx_Ptp802p1StreamDebugShow */

/*

  Function Name:
    zl303xx_Ptp802p1TlvHandle

  Details:
   Processes the 802.1AS TSN Profile TLVs.

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

*******************************************************************************/
Uint16T zl303xx_Ptp802p1TlvHandle(
      Uint32T streamHandle,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf,
      Uint8T *pOutTlvBuf)
{
   zlStatusE status = ZL303XX_CHECK_POINTER(pPtpMsgRx) |
                      ZL303XX_CHECK_POINTER(pInTlvBuf) |
                      ZL303XX_CHECK_POINTER(pOutTlvBuf);

   /* Default return value. */
   Uint16T tlvTxBytes = 0;

   if(status == ZL303XX_OK)
   {

      /* Vendor TLV Header parameters. */
      Uint16T tlvType, tlvLength;
      Uint32T oui32, subtype32;

      /* Pointer to the stream Data. */
      zl303xx_ClockStreamS *pPtpStream = NULL;

      /* Get a pointer to the stream data. */
      if (zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream) != ZL303XX_OK)
      {
         return 0;
      }

      /* Get the VENDER TLV fields. */
      /* -------------------------------------------------------
       * Field                      Length    Offset
       * ------------------------------------------------------
       * TLV Type (ORG_EXT)           2         0
       * lengthField                  2         2  (depends on subType)
       * OUI                          3         4  (0x00, 0x80, 0xC2)
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
      if (subtype32 == ZL303XX_PTP_802_P1_TLV_MSG_INTVL_REQ)
      {
         if (tlvLength != ZL303XX_PTP_802_P1_TLV_MSG_INTVL_REQ_TTL_LEN)
         {
            ZL303XX_TRACE_ERROR(
                  "zl303xx_Ptp802p1TlvHandle: ERROR - TLV Length (expected %d: actual %d)",
                  ZL303XX_PTP_802_P1_TLV_MSG_INTVL_REQ_TTL_LEN, tlvLength, 0,0,0,0);
         }
         else
         {
            /* Structure to hold values from tlv*/
            zl303xx_Ptp802p1MsgIntvlReqTlvS decodedTlv;

            /*Unpack the TLV and save all values */
            zl303xx_Ptp802p1MsgIntvlReqTlvUnpack(
                  pInTlvBuf + ZL303XX_PTP_TLV_ORG_EXT_HEADER_LEN,
                  &decodedTlv);

            /* Process the decoded values and set new intervals accordingly */
            status = zl303xx_Ptp802p1LogSyncIntvlSet(streamHandle, decodedTlv.timeSyncIntvl);
            if(status == ZL303XX_OK)
            {
               status = zl303xx_Ptp802p1LogAnnounceIntvlSet(streamHandle, decodedTlv.announceIntvl);
            }

            if(status == ZL303XX_OK)
            {
               status = zl303xx_Ptp802p1LogPdelayIntvlSet(streamHandle, decodedTlv.linkDelayIntvl);
            }

            ZL303XX_TRACE_ALWAYS(
                  "zl303xx_Ptp802p1TlvHandle: TLV decode completed for stream %lu: status %lu",
                  streamHandle, status, 0,0,0,0);
         }
      }
      else
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1, "zl303xx_Ptp802p1TlvHandle: Received TLV with unsupported type: %lu",
                     subtype32,0,0,0,0,0);
      }
   }

   return tlvTxBytes;
}  /* END zl303xx_Ptp802p1TlvHandle */


/*

  Function Name:
    zl303xx_Ptp802p1MsgIntvlReqTlvUnpack

  Details:
   Unpacks a 802.1AS Message Interval Req TLV, converting to a compatible data structure.

  Parameters:
   [in]  pInTlvBuf      Pointer to the data portion of the input TLV buffer.
                              (Begins at the linkDelayInterval field).

   [out] pOutTlvDataS   Pointer to a compatible TLV data structure.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_Ptp802p1MsgIntvlReqTlvUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_Ptp802p1MsgIntvlReqTlvS *pOutTlvDataS)
{
   zlStatusE status = ZL303XX_CHECK_POINTER(pInTlvBuf) |
                      ZL303XX_CHECK_POINTER(pOutTlvDataS);
   /* -------------------------------------------------------
    * Field                      Length    Offset
    * ------------------------------------------------------
    * linkDelayInterval             1         0
    * timeSyncInterval              1         1
    * announceInterval              1         2
    * flags                         1         3
    * reserved                      2         4
    * ------------------------------------------------------
    * Total Length                  6                                      */

   if(status == ZL303XX_OK)
   {
      /* linkDelayInterval */
      pOutTlvDataS->linkDelayIntvl = UNPACK_8BIT(pInTlvBuf + 0);

      /* timeSyncInterval */
      pOutTlvDataS->timeSyncIntvl = UNPACK_8BIT(pInTlvBuf + 1);

      /* announceInterval */
      pOutTlvDataS->announceIntvl = UNPACK_8BIT(pInTlvBuf + 2);

      /* flags */
      pOutTlvDataS->flags = UNPACK_8BIT(pInTlvBuf + 3);

      return ZL303XX_PTP_802_P1_TLV_MSG_INTVL_REQ_DATA_LEN;
   }
   else
   {
      return 0;
   }
}  /* END zl303xx_Ptp802p1MsgIntvlReqTlvUnpack */


/*

  Function Name:
    zl303xx_Ptp802p1MsgIntvlReqTlvPack

  Details:
   Packs a 802.1AS Message Interval Req TLV, converting from a compatible data structure.

  Parameters:
   [in]  pInTlvDataS      Pointer to a compatible TLV data structure

   [out] pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise


  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_Ptp802p1MsgIntvlReqTlvPack(
      zl303xx_Ptp802p1MsgIntvlReqTlvS *pInTlvDataS,
      Uint8T *pOutTlvBuf)
{
   /* Refer to the UNPACK routine (above) for TLV detailed layout.    */

   zlStatusE status = ZL303XX_CHECK_POINTER(pInTlvDataS) |
                         ZL303XX_CHECK_POINTER(pOutTlvBuf);
   if(status == ZL303XX_OK)
   {
      /* Clear the output data buffer. */
      /* Will set the RESERVED fields to 0's. */
      OS_MEMSET(pOutTlvBuf, 0x00, ZL303XX_PTP_802_P1_TLV_MSG_INTVL_REQ_DATA_LEN);


      /* linkDelayInterval */
      PACK_8BIT(pOutTlvBuf + 0, pInTlvDataS->linkDelayIntvl);

      /* timeSyncInterval */
      PACK_8BIT(pOutTlvBuf + 1, pInTlvDataS->timeSyncIntvl);

      /* announceInterval */
      PACK_8BIT(pOutTlvBuf + 2, pInTlvDataS->announceIntvl);

      /* flags */
      PACK_8BIT(pOutTlvBuf + 3, pInTlvDataS->flags);

      return ZL303XX_PTP_802_P1_TLV_MSG_INTVL_REQ_DATA_LEN;
   }
   else
   {
      return 0;
   }
}  /* END zl303xx_Ptp802p1MsgIntvlReqTlvUnpack */


/* zl303xx_Ptp802p1LogPdelayIntvlSet */
/**
   Sets the log2 Peer Delay_Req/Resp interval for a stream received
   in a message interval request TLV

  Parameters:
   [in]  streamHandle      Handle to an existing stream.
   [in]  logPdelayIntvl    Peer Delay_Req/Resp interval to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or logPdelayIntvl is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.
*/
zlStatusE zl303xx_Ptp802p1LogPdelayIntvlSet(zl303xx_PtpStreamHandleT streamHandle,
                                          Sint8T logPdelayIntvl)
{
   zlStatusE status = ZL303XX_OK;
   Sint8T reqLogPdelayIntvl;

   /* Check if a change is actually requested in the interval */
   if(logPdelayIntvl != ZL303XX_PTP_802P1_LOG_INTVL_NO_CHANGE)
   {
      /* Decode the passed value for special requests i.e. Reset to default */
      reqLogPdelayIntvl = zl303xx_Ptp802p1LogMsgIntvlDecode(logPdelayIntvl,
                                                         ZL303XX_PTP_802P1_LOG_PDELAY_DEF);
      /* Set the Interval*/
      status = zl303xx_PtpStreamLogPdelayIntvlSet(streamHandle, reqLogPdelayIntvl);
   }
   return status;
}


/* zl303xx_Ptp802p1LogAnnounceIntvlSet */
/**
   Sets the log2 Announce interval for a stream received
   in a message interval request TLV

  Parameters:
   [in]  streamHandle        Handle to an existing stream.
   [in]  logAnnounceIntvl    Announce interval to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or logAnnounceIntvl is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.
*/
zlStatusE zl303xx_Ptp802p1LogAnnounceIntvlSet(zl303xx_PtpStreamHandleT streamHandle,
                                             Sint8T logAnncIntvl)
{
   zlStatusE status = ZL303XX_OK;
   Sint8T reqLogAnncIntvl;

   /* Check if a change is actually requested in the interval */
   if(logAnncIntvl != ZL303XX_PTP_802P1_LOG_INTVL_NO_CHANGE)
   {
      /* Decode the passed value for special requests i.e. Reset to default */
      reqLogAnncIntvl = zl303xx_Ptp802p1LogMsgIntvlDecode(logAnncIntvl,
                                                         ZL303XX_PTP_802P1_LOG_ANNC_DEF);
      /* Set the Interval*/
      status = zl303xx_PtpStreamLogAnnounceIntvlSet(streamHandle, reqLogAnncIntvl);
   }
   return status;
}


/* zl303xx_Ptp802p1LogSyncIntvlSet */
/**
   Sets the log2 Sync interval for a stream received
   in a message interval request TLV

  Parameters:
   [in]  streamHandle    Handle to an existing stream.
   [in]  logSyncIntvl    Sync interval to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle or logSyncIntvl is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.
*/
zlStatusE zl303xx_Ptp802p1LogSyncIntvlSet(zl303xx_PtpStreamHandleT streamHandle,
                                          Sint8T logSyncIntvl)
{
   zlStatusE status = ZL303XX_OK;
   Sint8T reqLogSyncIntvl;

   /* Check if a change is actually requested in the interval */
   if(logSyncIntvl != ZL303XX_PTP_802P1_LOG_INTVL_NO_CHANGE)
   {
      /* Decode the passed value for special requests i.e. Reset to default */
      reqLogSyncIntvl = zl303xx_Ptp802p1LogMsgIntvlDecode(logSyncIntvl,
                                                         ZL303XX_PTP_802P1_LOG_SYNC_DEF);
      /* Set the Interval*/
      status = zl303xx_PtpStreamLogSyncIntvlSet(streamHandle, reqLogSyncIntvl);
   }
   return status;
}


/* zl303xx_Ptp802p1LogMsgIntvlDecode */
/**
   Decodes the passed Message Interval for
   a given message type.
   126 is a special value which means the interval should
   be reset to default.

  Parameters:
   [in]  log2Intvl       New message interval.
   [in]  log2DefIntvl    Default Interval for a particular message.

  Return Value:  The decoded message interval
*/
Sint8T zl303xx_Ptp802p1LogMsgIntvlDecode(Sint8T log2Intvl, Sint8T log2DefIntvl)
{
   Sint8T  logMsgIntvl = log2Intvl;
   if(log2DefIntvl == ZL303XX_PTP_802P1_LOG_INTVL_RESET)
   {
      logMsgIntvl = log2DefIntvl;
   }

   return logMsgIntvl;
}
