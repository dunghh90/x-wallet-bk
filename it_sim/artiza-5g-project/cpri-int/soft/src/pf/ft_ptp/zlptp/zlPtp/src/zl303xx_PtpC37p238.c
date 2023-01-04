

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains the data types and functions required to support
*     the PC37.238-2011 standard (Power Profile).
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"
#include "zl303xx_Error.h"

#include "zl303xx_PtpC37p238Api.h"
#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpConstants.h"
#include "zl303xx_PtpStrings.h"

#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpV2MsgParse.h"
#include "zl303xx_PtpFunctionExport.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpForeignMasterTbl.h"

#include "zl303xx_Ptsf.h"

/*****************   DEFINES     **********************************************/

/* Version 2014 TLV Extension definitions. (Currently only 1 defined). */
#define ZL303XX_PTP_C38_238_EXT_LOCAL_TIME_ZONE               (Uint8T)(0x01)

/* Minimum size of the LOCAL TIME ZONE EXTENSION. May be larger due to the
 * 2 time zone name fields. This definition marks the start of the
 * stdTimeZoneName field. */
#define ZL303XX_PTP_C38_238_EXT_LOCAL_TIME_ZONE_LENGTH_MIN    18

/*****************   STATIC DATA STRUCTURES   *********************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/* Gets the clock data pointer for the specified clock handle and verifies that
 * the clock is operating with the required C37.238 Profile. */
zlStatusE zl303xx_PtpClockDataGetC37p238(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpClockS **ppPtpClock);
/* Same as above for the Stream. */
zlStatusE zl303xx_PtpStreamDataGetC37p238(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_ClockStreamS **ppPtpStream);

/* Routine to set Profile clock default information. */
zlStatusE zl303xx_PtpC37p238ClockDefaults (void *pClockInitS,  Uint32T profileVer);
zlStatusE zl303xx_PtpC37p238PortDefaults  (void *pPortInitS,   Uint32T profileVer);
zlStatusE zl303xx_PtpC37p238StreamDefaults(void *pStreamInitS, Uint32T profileVer);

/* Routines to Pack/Unpack the Profile Specific TLV(s). */
Uint16T zl303xx_PtpC37p238v1TlvPack(
      zl303xx_PtpC37p238v1TlvS *pTlvDataS,
      Uint8T *pOutTlvBuf);
Uint16T zl303xx_PtpC37p238v1TlvUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_PtpC37p238v1TlvS *pTlvDataS);
Uint16T zl303xx_PtpC37p238v2TlvPack(
      zl303xx_PtpC37p238v2TlvS *pTlvDataS,
      Uint8T *pOutTlvBuf);
Uint16T zl303xx_PtpC37p238v2TlvUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_PtpC37p238v2TlvS *pTlvDataS);

/* Routines to Pack & Unpack profile TLV extensions. */
Uint16T zl303xx_PtpC37p238LocalTimeZoneExtUnpack(
      Uint8T *pInExtBuf,
      zl303xx_PtpC37p238LocalTimeZoneS *localTimeZoneS);
Uint16T zl303xx_PtpC37p238LocalTimeZoneExtPack(
      zl303xx_PtpC37p238LocalTimeZoneS *localTimeZoneS,
      Uint8T *pOutExtBuf);

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/* PROFILE OUI. */
Uint8T Zl303xx_PtpC37p238_OUI[] = {0x1C, 0x12, 0x9D};

/* Profile Information for each revision. */
/*** 2011 */
const zl303xx_PtpProfileS Zl303xx_PtpC37_238_2011ProfileInfo = {
   "IEEE Standard Profile for Use of IEEE 1588 Precision Time Protocol in Power System Applications",
   "Version 1.0",
   {0x1C, 0x12, 0x9D, 0x00, 0x00, 0x00},
   "This profile is specified by the IEEE 1588 Profile for Power System Applications Working Group of the IEEE Power System Relaying Committee and IEEE Power System Substation Committee",
   "For more information, refer to http://standards.ieee.org."
};

/*** 2014 */
const zl303xx_PtpProfileS Zl303xx_PtpC37_238_2014ProfileInfo = {
   "IEEE Standard Profile for Use of IEEE 1588 Precision Time Protocol in Power System Applications",
   "Version 2.0",
   {0x1C, 0x12, 0x9D, 0x00, 0x00, 0x00},
   "This profile is specified by the IEEE 1588 Profile for Power System Applications Working Group of the IEEE Power System Relaying Committee and IEEE Power System Substation Committee",
   "For more information, refer to http://standards.ieee.org."
};

/* There are 2 supported TLV sub-types (1 for 2011 and the other for 2014). */
Uint8T Zl303xx_PtpC37_238_2011TlvSubType[] = {0x00, 0x00, 0x01};
Uint8T Zl303xx_PtpC37_238_2014TlvSubType[] = {0x00, 0x00, 0x02};

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/*
  Function Name:
    zl303xx_PtpC37p238StreamProfileTlvCfgSet

  Details:
   Configures the requirements for Profile Specific TLV.

  Parameters:
   [in]  streamHandle   Handle to an existing stream.
   [in]  required       Determines if the TLV is required to be appended to
                              received Announce messages.
                              If == TRUE: any received Announce message with the
                                 TLV missing is discarded. Also, all transmitted
                                 Announce messages will append the TLV regardless
                                 of the 'append' setting.
   [in]  append         Determines if the TLV is required to be appended to
                              transmitted Announce messages.

  Return Value:
     ZL303XX_OK                      Success.
     ZL303XX_PARAMETER_INVALID       streamHandle is invalid.
     ZL303XX_INVALID_OPERATION       The specified stream is not running the correct profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL    Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL    Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238StreamProfileTlvCfgSet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_BooleanE required,
      zl303xx_BooleanE append)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Make sure the inputs are proper booleans. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(required) |
               ZL303XX_CHECK_BOOLEAN(append);
   }

   /* Get a pointer to the stream data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGetC37p238(streamHandle, &pPtpStream);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpStream->portData->clock);
   }

   /* Set the TLV configuration values. */
   if (status == ZL303XX_OK)
   {
      pPtpStream->config.profileCfg.power.profileTlvRequired = required;
      pPtpStream->config.profileCfg.power.profileTlvAppend = append;

      status = zl303xx_PtpClockMutexUnlock(pPtpStream->portData->clock, status);
   }

   return status;
}  /* END zl303xx_PtpC37p238StreamProfileTlvCfgSet */

/*
  Function Name:
    zl303xx_PtpC37p238StreamProfileTlvVersionSet

  Details:
   Configures the version of the Profile Specific TLV to use.

  Parameters:
   [in]  streamHandle   Handle to an existing stream.
   [in]  profileVer     The Profile TLV version to use (2001 or 2014).

  Return Value:
     ZL303XX_OK                      Success.
     ZL303XX_PARAMETER_INVALID       streamHandle or tlvVersion is invalid.
     ZL303XX_INVALID_OPERATION       The specified stream is not running the correct profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL    Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL    Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238StreamProfileTlvVersionSet(
      zl303xx_PtpStreamHandleT streamHandle,
      Uint32T profileVer)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Get a pointer to the stream data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGetC37p238(streamHandle, &pPtpStream);
   }

   /* Make sure the inputs are proper booleans. */
   if (status == ZL303XX_OK)
   {
      if ((profileVer != ZL303XX_PTP_C37_238_VERSION_2011) &&
          (profileVer != ZL303XX_PTP_C37_238_VERSION_2014))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpC37p238StreamProfileTlvVersionSet: Undefined Profile Version(%u)",
               profileVer, 0,0,0,0,0);

         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpStream->portData->clock);
   }

   /* Set the TLV configuration values. */
   if (status == ZL303XX_OK)
   {
      pPtpStream->config.profileCfg.power.profileTlvVersion = profileVer;

      status = zl303xx_PtpClockMutexUnlock(pPtpStream->portData->clock, status);
   }

   return status;
}  /* END zl303xx_PtpC37p238StreamProfileTlvVersionSet */

/*
  Function Name:
    zl303xx_PtpC37p238StreamAltTimeTlvCfgSet

  Details:
   Configures the requirements for Alternate Time Offset TLV.

  Parameters:
   [in]  streamHandle   Handle to an existing stream.
   [in]  required       Determines if the TLV is required to be appended to
                              received Announce messages.
                              If == TRUE: any received Announce message with the
                                 TLV missing is discarded. Also, all transmitted
                                 Announce messages will append the TLV regardless
                                 of the 'append' setting.
   [in]  append         Determines if the TLV is required to be appended to
                              transmitted Announce messages.

  Return Value:
     ZL303XX_OK                      Success.
     ZL303XX_PARAMETER_INVALID       streamHandle is invalid.
     ZL303XX_INVALID_OPERATION       The specified stream is not running the correct profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL    Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL    Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238StreamAltTimeTlvCfgSet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_BooleanE required,
      zl303xx_BooleanE append)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Get a pointer to the stream data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGetC37p238(streamHandle, &pPtpStream);
   }

   /* Make sure the inputs are proper booleans. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(required) |
               ZL303XX_CHECK_BOOLEAN(append);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpStream->portData->clock);
   }

   /* Set the TLV configuration values. */
   if (status == ZL303XX_OK)
   {
      pPtpStream->config.profileCfg.power.altTimeOffsetTlvRequired = required;
      pPtpStream->config.profileCfg.power.altTimeOffsetTlvAppend = append;

      status = zl303xx_PtpClockMutexUnlock(pPtpStream->portData->clock, status);
   }

   return status;
}  /* END zl303xx_PtpC37p238StreamAltTimeTlvCfgSet */

/**
    zl303xx_PtpC37p238v1ProfileInfoGet

  Details:
   Retrieves the profile description information for the profile.

  Parameters:
   [in]  profileInfo    Pointer to the structure to store the profile information.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

 *****************************************************************************/
zlStatusE zl303xx_PtpC37p238v1ProfileInfoGet(const zl303xx_PtpProfileS **profileInfo)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(profileInfo);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      *profileInfo = &Zl303xx_PtpC37_238_2011ProfileInfo;
   }

   return status;
}

/**
    zl303xx_PtpC37p238v2ProfileInfoGet

  Details:
   Retrieves the profile description information for the profile.

  Parameters:
   [in]  profileInfo    Pointer to the structure to store the profile information.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

 *****************************************************************************/
zlStatusE zl303xx_PtpC37p238v2ProfileInfoGet(const zl303xx_PtpProfileS **profileInfo)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(profileInfo);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      *profileInfo = &Zl303xx_PtpC37_238_2014ProfileInfo;
   }

   return status;
}

/*

  Function Name:
    zl303xx_PtpC37p238v1ClockDefaults

  Details:
   Routine to set the PC37.238-2011, (Power Profile) profile specific clock
   parameters to their default values.

  Parameters:
   [in]  pClockInitS    Pointer to the PTP creation data structure for the
                              clock (from the PtpApiTypes.h module). This is
                              a void type so that all PTP Object types can use
                              the same interface.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238v1ClockDefaults(void *pClockInitS)
{
   return zl303xx_PtpC37p238ClockDefaults(pClockInitS, ZL303XX_PTP_C37_238_VERSION_2011);
}  /* END zl303xx_PtpC37p238v1ClockDefaults */

/*

  Function Name:
    zl303xx_PtpC37p238v2ClockDefaults

  Details:
   Routine to set the PC37.238-2014, (Power Profile) profile specific clock
   parameters to their default values.

  Parameters:
   [in]  pClockInitS    Pointer to the PTP creation data structure for the
                              clock (from the PtpApiTypes.h module). This is
                              a void type so that all PTP Object types can use
                              the same interface.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238v2ClockDefaults(void *pClockInitS)
{
   return zl303xx_PtpC37p238ClockDefaults(pClockInitS, ZL303XX_PTP_C37_238_VERSION_2014);
}  /* END zl303xx_PtpC37p238v2ClockDefaults */

/*

  Function Name:
    zl303xx_PtpC37p238ClockDefaults

  Details:
   Routine to set the PC37.238, (Power Profile) profile specific clock
   parameters to their default values for the profile revision.

  Parameters:
   [in]  pClockInitS    Pointer to the PTP creation data structure for the
                              clock (from the PtpApiTypes.h module). This is
                              a void type so that all PTP Object types can use
                              the same interface.
   [in]  profileVer     The Profile Version.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238ClockDefaults(void *pClockInitS, Uint32T profileVer)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pClockInitS);

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpClockCreateS *pClockCreateS = (zl303xx_PtpClockCreateS *)pClockInitS;

      /* Common PTP Clock Attributes. */
      /********************************/
      /* priority1 & priority1 */
      if (pClockCreateS->defaultDS.clockQuality.clockClass == DEFAULT_C37_238_CLK_CLASS_SLAVE)
      {
         pClockCreateS->defaultDS.slaveOnly = DEFAULT_C37_238_SLAVE_ONLY_SLAVE;
         pClockCreateS->defaultDS.priority1 = DEFAULT_C37_238_P1_SLAVE;
         pClockCreateS->defaultDS.priority2 = DEFAULT_C37_238_P2_SLAVE;
      }
      else
      {
         pClockCreateS->defaultDS.slaveOnly = DEFAULT_C37_238_SLAVE_ONLY_MASTER;
         pClockCreateS->defaultDS.priority1 = DEFAULT_C37_238_P1_MASTER;
         pClockCreateS->defaultDS.priority2 = DEFAULT_C37_238_P2_MASTER;
      }

      /* domainNumber */
      if (profileVer == ZL303XX_PTP_C37_238_VERSION_2014)
      {
         pClockCreateS->defaultDS.domainNumber = DEFAULT_C37_238_2014_DOMAIN;
      }
      else  /* 2011 or any unknown version. */
      {
         pClockCreateS->defaultDS.domainNumber = DEFAULT_C37_238_2011_DOMAIN;
      }

      /* Power Profile Clock Attributes. */
      /***********************************/
      /* grandmasterID */
      if (pClockCreateS->defaultDS.clockQuality.clockClass == DEFAULT_C37_238_CLK_CLASS_SLAVE)
      {
         pClockCreateS->profileCfg.power.clockId = 0;
      }
      else
      {
         pClockCreateS->profileCfg.power.clockId = pClockCreateS->requestedHandle;
      }

      /* localTimeInaccuracy */
      pClockCreateS->profileCfg.power.localTimeInaccuracy = 0;
      /* networkTimeInaccuracy */
      pClockCreateS->profileCfg.power.networkTimeInaccuracy = 0;
   }

   return status;
}  /* END zl303xx_PtpC37p238ClockDefaults */

/*

  Function Name:
    zl303xx_PtpC37p238v1PortDefaults

  Details:
   Routine to set the PC37.238-2011, (Power Profile) profile specific port
   parameters to their default values.

  Parameters:
   [in]  pPortInitS     Pointer to the PTP creation data structure for the
                              port (from the PtpApiTypes.h module). This is
                              a void type so that all PTP Object types can use
                              the same interface.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238v1PortDefaults(void *pPortInitS)
{
   return zl303xx_PtpC37p238PortDefaults(pPortInitS, ZL303XX_PTP_C37_238_VERSION_2011);
}  /* END zl303xx_PtpC37p238v1PortDefaults */

/*

  Function Name:
    zl303xx_PtpC37p238v2PortDefaults

  Details:
   Routine to set the PC37.238-2014, (Power Profile) profile specific port
   parameters to their default values.

  Parameters:
   [in]  pPortInitS     Pointer to the PTP creation data structure for the
                              port (from the PtpApiTypes.h module). This is
                              a void type so that all PTP Object types can use
                              the same interface.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238v2PortDefaults(void *pPortInitS)
{
   return zl303xx_PtpC37p238PortDefaults(pPortInitS, ZL303XX_PTP_C37_238_VERSION_2014);
}  /* END zl303xx_PtpC37p238v2PortDefaults */

/*

  Function Name:
    zl303xx_PtpC37p238PortDefaults

  Details:
   Routine to set the PC37.238, (Power Profile) profile specific port
   parameters to their default values for the profile revision.

  Parameters:
   [in]  pPortInitS     Pointer to the PTP creation data structure for the
                              port (from the PtpApiTypes.h module). This is
                              a void type so that all PTP Object types can use
                              the same interface.
   [in]  profileVer     The Profile Version.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238PortDefaults(void *pPortInitS, Uint32T profileVer)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pPortInitS);

   /* Warning elimination. */
   if (profileVer) {;}

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpPortCreateS *pPortCreateS = (zl303xx_PtpPortCreateS *)pPortInitS;

      /* Common PTP Port Attributes. */
      /*******************************/
      /* delayMechanism */
      pPortCreateS->delayMechanism = ZL303XX_DELAY_MECHANISM_P2P;
      /* logMinPdelayReqInterval */
      pPortCreateS->logMinPdelayReqInterval = DEFAULT_C37_238_PDELAY_INTERVAL;


      /* Power Profile Port Attributes. */
      /**********************************/

   }

   return ZL303XX_OK;
}  /* END zl303xx_PtpC37p238PortDefaults */

/*

  Function Name:
    zl303xx_PtpC37p238v1StreamDefaults

  Details:
   Routine to set the PC37.238-2011, (Power Profile) profile specific stream
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
zlStatusE zl303xx_PtpC37p238v1StreamDefaults(void *pStreamInitS)
{
   return zl303xx_PtpC37p238StreamDefaults(pStreamInitS, ZL303XX_PTP_C37_238_VERSION_2011);
}  /* END zl303xx_PtpC37p238v1StreamDefaults */

/*

  Function Name:
    zl303xx_PtpC37p238v2StreamDefaults

  Details:
   Routine to set the PC37.238-2014, (Power Profile) profile specific stream
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
zlStatusE zl303xx_PtpC37p238v2StreamDefaults(void *pStreamInitS)
{
   return zl303xx_PtpC37p238StreamDefaults(pStreamInitS, ZL303XX_PTP_C37_238_VERSION_2014);
}  /* END zl303xx_PtpC37p238v2StreamDefaults */

/*

  Function Name:
    zl303xx_PtpC37p238StreamDefaults

  Details:
   Routine to set the PC37.238, (Power Profile) profile specific stream
   parameters to their default values for the profile revision.

  Parameters:
   [in]  pStreamInitS   Pointer to the PTP creation data structure for the
                              stream (from the PtpApiTypes.h module). This is
                              a void type so that all PTP Object types can use
                              the same interface.
   [in]  profileVer     The Profile Version.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238StreamDefaults(void *pStreamInitS, Uint32T profileVer)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pStreamInitS);

   /* Warning elimination. */
   if (profileVer) {;}

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpStreamCreateS *pStreamCreateS = (zl303xx_PtpStreamCreateS *)pStreamInitS;

      /* Common PTP Stream Attributes. */
      /*********************************/
      pStreamCreateS->logAnnounceInterval    = DEFAULT_C37_238_ANNC_INTERVAL;
      pStreamCreateS->logSyncInterval        = DEFAULT_C37_238_SYNC_INTERVAL;
      pStreamCreateS->announceReceiptTimeout = DEFAULT_C37_238_ANNC_TIMEOUT;

      /* Power Profile Stream Attributes. */
      /************************************/
      /* Configuration settings: the 2011 profile requires both TLVs while the
       * 2014 version does not. */
      if (profileVer == ZL303XX_PTP_C37_238_VERSION_2014)
      {
         pStreamCreateS->profileCfg.power.profileTlvRequired       = ZL303XX_FALSE;
         pStreamCreateS->profileCfg.power.profileTlvAppend         = ZL303XX_FALSE;
         pStreamCreateS->profileCfg.power.profileTlvVersion        = ZL303XX_PTP_C37_238_VERSION_2014;
         pStreamCreateS->profileCfg.power.altTimeOffsetTlvRequired = ZL303XX_FALSE;
         pStreamCreateS->profileCfg.power.altTimeOffsetTlvAppend   = ZL303XX_FALSE;
      }
      else  /* By default, set 2011 defaults. */
      {
         pStreamCreateS->profileCfg.power.profileTlvRequired       = ZL303XX_TRUE;
         pStreamCreateS->profileCfg.power.profileTlvAppend         = ZL303XX_TRUE;
         pStreamCreateS->profileCfg.power.profileTlvVersion        = ZL303XX_PTP_C37_238_VERSION_2011;
         pStreamCreateS->profileCfg.power.altTimeOffsetTlvRequired = ZL303XX_TRUE;
         pStreamCreateS->profileCfg.power.altTimeOffsetTlvAppend   = ZL303XX_TRUE;
      }

      /* localTimeInaccuracyMax */
      pStreamCreateS->profileCfg.power.localTimeInaccuracyMax = ZL303XX_PTP_C37_238_LOCAL_TIME_INNAC_MAX;

      /* networkTimeInaccuracyMax */
      pStreamCreateS->profileCfg.power.networkTimeInaccuracyMax = ZL303XX_PTP_C37_238_NETWORK_TIME_INNAC_MAX;
   }

   return status;
}  /* END zl303xx_PtpC37p238StreamDefaults */

/*

  Function Name:
    zl303xx_PtpC37p238ClockCreateActions

  Details:
   Routine to perform PC37.238 (Power Profile) actions related to clock creation.
   This routine is BIND'ed (bound) to the ProfileDef list and gets called at the
   end of the core Clock Creation process. (See zl303xx_PtpClockCreate()).

   For this profile, the PC37.238 profile specific TLV must be added to the
   clock's custom processing table.

  Parameters:
   [in]  pClockS  Pointer to the PTP clock data structure (as a void).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pClockS is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238ClockCreateActions(void *pClockS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pClockS);

   /* De-reference the clock pointer (if NULL, we skip using it). */
   zl303xx_PtpClockS *pPtpClockS = (zl303xx_PtpClockS *)pClockS;

   Uint32T oui32;

   /* Add the Profile OUI to the Custom TLV handler for the Clock */
   if (status == ZL303XX_OK)
   {
      /* Convert the OUI to an 'ID' for the lookup table. */
      (void)zl303xx_PtpTlvConvert3ByteFieldTo32Bit(Zl303xx_PtpC37p238_OUI, &oui32);

      /* Add the custom TLV handler for this profile. */
      status = zl303xx_PtpTlvCustomHandlerSet(
                                    pPtpClockS->clockHandle,
                                    PTP_TLV_VENDOR_EXTENSION,
                                    oui32,
                                    zl303xx_PtpC37p238TlvHandle);
   }

   return status;
}  /* END zl303xx_PtpC37p238ClockCreateActions */

/*

  Function Name:
    zl303xx_PtpC37p238AnncRxCheck

  Details:
   Checks that profile specific TLVs are appended if they are required.

  Parameters:
   [in]  pPtpStream     Pointer to the stream on which the message was received.
   [in]  pPtpMsgRx      Pointer the the received message structure (contains
                              all header and announce fields).
   [in]  pInTlvBuf      Pointer to any appended TLV buffer.

  Return Value:
     ZL303XX_OK                  Success.
     ZL303XX_INVALID_POINTER     pPtpStream or pPtpMsgRx is NULL.
     ZL303XX_PARAMETER_INVALID   Expected TLVs are not appended.
     OTHER                       As specified by the Profile specific check.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238AnncRxCheck(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf)
{
   /* Default return value. */
   zlStatusE status = ZL303XX_OK;

   /* Build the required minimum length of the Announce message based on the
    * TLVs that are required (or not).   */
   Uint16T requiredMsgLenMin = ZL303XX_PTP_V2_ANNC_MSG_LEN;

   /* Eliminate warnings for unused parameter. */
   if (pInTlvBuf) {;}

   /* Check input pointers. */
   if ((pPtpStream == NULL) || (pPtpMsgRx == NULL))
   {
      return ZL303XX_INVALID_POINTER;
   }

   /* If the Profile specific TLV is required, expect the ORG_EXT header length
    * and the Profile TLV length */
   if (pPtpStream->config.profileCfg.power.profileTlvRequired == ZL303XX_TRUE)
   {
      requiredMsgLenMin += (ZL303XX_PTP_TLV_ORG_EXT_HEADER_LEN + ZL303XX_PTP_C37_238_2011_TLV_LEN);
   }

   /* If the Alternate Time Offset TLV is required then expect at least the data
    * fields (the displayName may be empty). */
   if (pPtpStream->config.profileCfg.power.altTimeOffsetTlvRequired == ZL303XX_TRUE)
   {
      requiredMsgLenMin += (ZL303XX_PTP_TLV_ALT_TIME_OFFSET_MIN_LEN);
   }

   /* Check the expected length against the packet message length. */
   if (pPtpMsgRx->header.messageLength < requiredMsgLenMin)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "zl303xx_PtpC37p238AnncRxCheck: Received message too short (Req:%u)(Act:%u)",
            requiredMsgLenMin, pPtpMsgRx->header.messageLength,0 ,0,0,0);

      status = ZL303XX_PARAMETER_INVALID;
   }

   return status;
}  /* END zl303xx_PtpC37p238AnncRxCheck */

/*

  Function Name:
    zl303xx_PtpC37p238TlvAppend

  Details:
   Appends all necessary PC37.238-2011, Power Profile specific TLVs into
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
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpC37p238TlvAppend(
      zl303xx_ClockStreamS *pPtpStream,
      Uint32T messageType,
      Uint8T *pOutMsgBuf,
      Uint8T *pOutTlvBuf)
{
   /* Default return value. */
   Uint16T tlvBytes = 0;

   /* Dereference the profile specific Clock Data just to make coding cleaner. */
   zl303xx_PtpC37p238ClockConfigS *pProfileClockData = NULL;

   /* Warning removal */
   if (pOutMsgBuf) {;}

   /* Verify the Stream Data pointer.
    * Verify the messageType is Announce.
    * Verify the Power Profile Version. */
   if ((pPtpStream == NULL) ||
       (messageType != ZL303XX_MSG_ID_ANNOUNCE) ||
       ((pPtpStream->config.profileCfg.power.profileTlvVersion != ZL303XX_PTP_C37_238_VERSION_2011) &&
        (pPtpStream->config.profileCfg.power.profileTlvVersion != ZL303XX_PTP_C37_238_VERSION_2014)))
   {
      /* Return immediately to avoid complicated code. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "zl303xx_PtpC37p238TlvAppend: ERROR: ", 0,0,0,0,0,0);
       return 0;
   }

   /*** No failure scenario beyond this. */
   pProfileClockData = &pPtpStream->portData->clock->config.profileCfg.power;

   /* Pack the Profile specific TLV first (ORG_EXTENSION). */
   if ((pPtpStream->config.profileCfg.power.profileTlvRequired == ZL303XX_TRUE) ||
       (pPtpStream->config.profileCfg.power.profileTlvAppend == ZL303XX_TRUE))
   {
      /* Temp variable to avoid compiler warning. */
      Uint8T profileIdent[PTP_PROFILE_IDENTITY_LENGTH];

      /* Determine which Profile version should be used. */
      if (pPtpStream->config.profileCfg.power.profileTlvVersion == ZL303XX_PTP_C37_238_VERSION_2011)
      {
         /* Pack the Organization Extension Header */
         OS_MEMCPY(profileIdent, Zl303xx_PtpC37_238_2011ProfileInfo.profileIdentifier, PTP_PROFILE_IDENTITY_LENGTH);
         tlvBytes = zl303xx_PtpTlvOrganizationHeaderPack(
                           profileIdent,
                           Zl303xx_PtpC37_238_2011TlvSubType,
                           pOutTlvBuf);

         /* Add the TLV Data. */
         {
            zl303xx_PtpC37p238v1TlvS txTlvData;

            txTlvData.clockId = pProfileClockData->clockId;
            txTlvData.grandmasterTimeInaccuracy = pProfileClockData->localTimeInaccuracy;
            txTlvData.networkTimeInaccuracy = pProfileClockData->networkTimeInaccuracy;

            tlvBytes += zl303xx_PtpC37p238v1TlvPack(&txTlvData, (pOutTlvBuf + tlvBytes));
         }

      }
      else if (pPtpStream->config.profileCfg.power.profileTlvVersion == ZL303XX_PTP_C37_238_VERSION_2014)
      {
         /* Pack the Organization Extension Header */
         OS_MEMCPY(profileIdent, Zl303xx_PtpC37_238_2014ProfileInfo.profileIdentifier, PTP_PROFILE_IDENTITY_LENGTH);
         tlvBytes = zl303xx_PtpTlvOrganizationHeaderPack(
                           profileIdent,
                           Zl303xx_PtpC37_238_2014TlvSubType,
                           pOutTlvBuf);

         /* Add the TLV Data. */
         {
            zl303xx_PtpC37p238v2TlvS txTlvData;

            txTlvData.clockId = pProfileClockData->clockId;
            txTlvData.grandmasterTimeInaccuracy = pProfileClockData->localTimeInaccuracy;
            txTlvData.networkTimeInaccuracy = pProfileClockData->networkTimeInaccuracy;
            txTlvData.localTimeZoneInfo = pProfileClockData->localTimeZoneInfo;

            tlvBytes += zl303xx_PtpC37p238v2TlvPack(&txTlvData, (pOutTlvBuf + tlvBytes));
         }
      }

      /* Add the length within this ORG_EXT TLV if a proper version was specified. */
      /* length = (Ttl - 4 bytes of Type & Length) */
      if (tlvBytes > 0)
      {
         zl303xx_PtpTlvLengthSet((tlvBytes - ZL303XX_PTP_TLV_VALUE_OFFSET), pOutTlvBuf);
      }
   }

   /* Pack the ALTERNATE_TIME_OFFSET_INDICATOR TLV next if:
    *    1) It is REQUIRED or configured to be APPENDED
    *    2) AND either:
    *       a) It is a MASTER then append all ENABLED alternates.
    *       a) It is a BC or SLAVE then append the learned alternate. */
   if ((pPtpStream->config.profileCfg.power.altTimeOffsetTlvRequired == ZL303XX_TRUE) ||
       (pPtpStream->config.profileCfg.power.altTimeOffsetTlvAppend == ZL303XX_TRUE))
   {
      /* If this clock is MASTER, add all enabled ALT TIME OFFSET entries...  */
      if (zl303xx_PtpClockIsOwnMaster(pPtpStream->portData->clock))
      {
         Uint32T tblEntry = 0;
         for (tblEntry = 0; tblEntry < ZL303XX_PTP_ALT_TIME_OFFSET_TBL_ENTRIES; tblEntry++)
         {
            if (pPtpStream->portData->clock->altTimeOffset.entryTbl[tblEntry].enabled == ZL303XX_TRUE)
            {
               tlvBytes += zl303xx_PtpTlvAltTimeOffsetPack(
                                 &pPtpStream->portData->clock->altTimeOffset.entryTbl[tblEntry],
                                 (pOutTlvBuf + tlvBytes));
            }
         }
      }

      /* ... Otherwise Pack any learned parent data. */
      /* tlvType = PTP_TLV_ALTERNATE_TIME_OFFSET_INDICATOR */
      else if (pPtpStream->portData->clock->altTimeOffset.parentEntry.enabled == ZL303XX_TRUE)
      {
         tlvBytes += zl303xx_PtpTlvAltTimeOffsetPack(
                                 &pPtpStream->portData->clock->altTimeOffset.parentEntry,
                                 (pOutTlvBuf + tlvBytes));
      }
   }

   return tlvBytes;
}  /* END zl303xx_PtpC37p238TlvAppend */

/*

  Function Name:
    zl303xx_PtpC37p238TlvHandle

  Details:
   Processes the PC37.238-2011 or 2014 Power Profile specific TLVs.

  Parameters:
   [in]  streamHandle   Handle of the stream associated with this TLV buffer.
   [in]  pPtpMsgRx      Pointer the the received message structure (contains
                              all header and announce fields).
   [in]  pInTlvBuf      Pointer to the appended TLV buffer.

   [out] pOutTlvBuf     Pointer to the data portion of the output TLV buffer.
                              In this case no output buffer is packed. However,
                              this prototype must follow the definition of the
                              zl303xx_PtpV2TlvCustomHandlerT type so the handler
                              can be bound to the main PTP stack.

  Return Value:
   Uint16T  Number of bytes packed into the response TLV (in this case = 0).
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpC37p238TlvHandle(
      Uint32T streamHandle,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf,
      Uint8T *pOutTlvBuf)
{
   /* Default return value. */
   Uint16T tlvTxBytes = 0;

   zl303xx_BooleanE localTimeInaccExceeded = ZL303XX_FALSE;
   zl303xx_BooleanE networkTimeInaccExceeded = ZL303XX_FALSE;

   /* Pointer to the stream Data. */
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Warning elimination */
   if ((pPtpMsgRx) || (pOutTlvBuf)) {;}

   /* Get a pointer to the stream data. */
   if (zl303xx_PtpStreamDataGetC37p238(streamHandle, &pPtpStream) != ZL303XX_OK)
   {
      return 0;
   }

   /* Copy the profile data to the stream far-end profile data set. */
   if (pPtpStream->portData->clock->config.profile == ZL303XX_PTP_PROFILE_POWER_C37_238_2014)
   {
      zl303xx_PtpC37p238v2TlvUnpack(pInTlvBuf + ZL303XX_PTP_TLV_ORG_EXT_HEADER_LEN,
                                    &pPtpStream->profileDS.power);
   }
   else  /* Assume 2011 */
   {
      /* V1 fields are data aligned with V2. */
      zl303xx_PtpC37p238v1TlvUnpack(pInTlvBuf + ZL303XX_PTP_TLV_ORG_EXT_HEADER_LEN,
                                    (zl303xx_PtpC37p238v1TlvS *)(&pPtpStream->profileDS.power));
   }

   /* Check PTSF threshold values. */
   if (pPtpStream->profileDS.power.grandmasterTimeInaccuracy >
                     pPtpStream->config.profileCfg.power.localTimeInaccuracyMax)
   {
      localTimeInaccExceeded = ZL303XX_TRUE;
   }
   zl303xx_PtsfMaskSet(pPtpStream->clockStreamHandle,
                       ZL303XX_PTSF_LOCAL_TIME_INACC_EXCEEDED,
                       localTimeInaccExceeded);

   if (pPtpStream->profileDS.power.networkTimeInaccuracy >
                     pPtpStream->config.profileCfg.power.networkTimeInaccuracyMax)
   {
      networkTimeInaccExceeded = ZL303XX_TRUE;
   }
   zl303xx_PtsfMaskSet(pPtpStream->clockStreamHandle,
                       ZL303XX_PTSF_NETWORK_TIME_INACC_EXCEEDED,
                       networkTimeInaccExceeded);

   /* Disqualify the stream if PTSF is raised. */
   if ((localTimeInaccExceeded == ZL303XX_TRUE) ||
       (networkTimeInaccExceeded == ZL303XX_TRUE))
   {
      /* Disqualify any entries in the Foreign Master Table. */
      (void)zl303xx_PtpFmtEntryDisqualify(&pPtpStream->portData->clock->foreignMasterDS, pPtpStream);
   }

   return tlvTxBytes;
}  /* END zl303xx_PtpC37p238TlvHandle */

/*

  Function Name:
    zl303xx_PtpC37p238ClockDebugShow

  Details:
   Routine to display PC37.238-2014, (Power Profile) profile specific clock
   parameters.

  Parameters:
   [in]  pClockS  Pointer to the PTP clock data structure (as a void).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238ClockDebugShow(void *pClockS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pClockS);
   Uint32T profileVersion;

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpClockS *pPtpClockS = (zl303xx_PtpClockS *)pClockS;

      if (pPtpClockS->config.profile == ZL303XX_PTP_PROFILE_POWER_C37_238_2014)
      {
         profileVersion = ZL303XX_PTP_C37_238_VERSION_2014;
      }
      else  /* Assume 2011 */
      {
         profileVersion = ZL303XX_PTP_C37_238_VERSION_2011;
      }

      printf ("\nPC37.238(v%u) Profile: Clock Configured Data\n", profileVersion);
      printf ("-----------------------------------------------------\n");
      printf ("                    clockId: %u \n",
            pPtpClockS->config.profileCfg.power.clockId);
      printf ("        localTimeInaccuracy: %u \n",
            pPtpClockS->config.profileCfg.power.localTimeInaccuracy);
      printf ("      networkTimeInaccuracy: %u \n",
            pPtpClockS->config.profileCfg.power.networkTimeInaccuracy);

      if (profileVersion == ZL303XX_PTP_C37_238_VERSION_2014)
      {
         printf ("LOCAL TIME ZONE INFORMATION\n");
         printf ("                    keyField: %u \n",
                     pPtpClockS->config.profileCfg.power.localTimeZoneInfo.keyField);
         printf ("             stdTimeZoneName: %s \n",
                     pPtpClockS->config.profileCfg.power.localTimeZoneInfo.stdTimeZoneName);
         printf ("          stdTimeOffsetToUtc: %d \n",
                     pPtpClockS->config.profileCfg.power.localTimeZoneInfo.stdTimeOffsetToUtc);
         printf ("             stdTimeStartsAt: %u:%u:%u:%u (M:W:D:H) \n",
                     pPtpClockS->config.profileCfg.power.localTimeZoneInfo.stdTimeStartsAt.month,
                     pPtpClockS->config.profileCfg.power.localTimeZoneInfo.stdTimeStartsAt.week,
                     pPtpClockS->config.profileCfg.power.localTimeZoneInfo.stdTimeStartsAt.day,
                     pPtpClockS->config.profileCfg.power.localTimeZoneInfo.stdTimeStartsAt.hour);
         printf ("                 includesDst: %u \n",
                     pPtpClockS->config.profileCfg.power.localTimeZoneInfo.includesDst);
         printf ("             dstTimeZoneName: %s \n",
                     pPtpClockS->config.profileCfg.power.localTimeZoneInfo.dstTimeZoneName);
         printf ("          dstTimeOffsetToUtc: %d \n",
                     pPtpClockS->config.profileCfg.power.localTimeZoneInfo.dstTimeOffsetToUtc);
         printf ("             dstTimeStartsAt: %u:%u:%u:%u (M:W:D:H) \n",
                     pPtpClockS->config.profileCfg.power.localTimeZoneInfo.dstTimeStartsAt.month,
                     pPtpClockS->config.profileCfg.power.localTimeZoneInfo.dstTimeStartsAt.week,
                     pPtpClockS->config.profileCfg.power.localTimeZoneInfo.dstTimeStartsAt.day,
                     pPtpClockS->config.profileCfg.power.localTimeZoneInfo.dstTimeStartsAt.hour);
      }
   }

   return status;
}  /* END zl303xx_PtpC37p238ClockDebugShow */

/*

  Function Name:
    zl303xx_PtpC37p238PortDebugShow

  Details:
   Routine to display PC37.238-2014, (Power Profile) profile specific port
   parameters.

  Parameters:
   [in]  pPortS  Pointer to the PTP port data structure (as a void).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238PortDebugShow(void *pPortS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pPortS);
   Uint32T profileVersion;

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PortDataS *pPtpPortS = (zl303xx_PortDataS *)pPortS;

      if (pPtpPortS->clock->config.profile == ZL303XX_PTP_PROFILE_POWER_C37_238_2014)
      {
         profileVersion = ZL303XX_PTP_C37_238_VERSION_2014;
      }
      else  /* Assume 2011 */
      {
         profileVersion = ZL303XX_PTP_C37_238_VERSION_2011;
      }

      printf ("\nPC37.238(v%u) Profile Port Data\n", profileVersion);
      printf ("--------------------------------------\n");
      printf ("  >> No data to display <<\n");
   }

   return status;
}  /* END zl303xx_PtpC37p238PortDebugShow */

/*

  Function Name:
    zl303xx_PtpC37p238StreamDebugShow

  Details:
   Routine to display PC37.238-2014, (Power Profile) profile specific Stream
   parameters.

  Parameters:
   [in]  pStreamS  Pointer to the PTP Stream data structure (as a void).

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238StreamDebugShow(void *pStreamS)
{
   /* Check input parameters. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pStreamS);
   Uint32T profileVersion;

   /* Get the data. */
   if (status == ZL303XX_OK)
   {
      zl303xx_ClockStreamS *pPtpStreamS = (zl303xx_ClockStreamS *)pStreamS;

      if (pPtpStreamS->portData->clock->config.profile == ZL303XX_PTP_PROFILE_POWER_C37_238_2014)
      {
         profileVersion = ZL303XX_PTP_C37_238_VERSION_2014;
      }
      else  /* Assume 2011 */
      {
         profileVersion = ZL303XX_PTP_C37_238_VERSION_2011;
      }

      printf ("\nPC37.238(v%u) Profile Stream Data\n", profileVersion);
      printf ("--------------------------------------\n");
      printf ("           Profile TLV configuration: Req(%u) App(%u) Version(%u) \n",
            pPtpStreamS->config.profileCfg.power.profileTlvRequired,
            pPtpStreamS->config.profileCfg.power.profileTlvAppend,
            pPtpStreamS->config.profileCfg.power.profileTlvVersion);
      printf ("   ALT_TIME_OFFSET TLV configuration: Req(%u) App(%u)  \n",
            pPtpStreamS->config.profileCfg.power.altTimeOffsetTlvRequired,
            pPtpStreamS->config.profileCfg.power.altTimeOffsetTlvAppend);
      printf ("              localTimeInaccuracyMax: %u \n",
            pPtpStreamS->config.profileCfg.power.localTimeInaccuracyMax);
      printf ("            networkTimeInaccuracyMax: %u \n",
            pPtpStreamS->config.profileCfg.power.networkTimeInaccuracyMax);
   }

   return status;
}  /* END zl303xx_PtpC37p238StreamDebugShow */


/******************************************************************************/
/*******   USER API FUNCTION DEFINITIONS   ************************************/
/******************************************************************************/

/* zl303xx_PtpC37p238ClockPerformanceGet */
/**
   Retrieves the clock's profile specific performance data parameters.

  Parameters:
   [in]  clockHandle       Handle to an existing clock.
   [in]  clockPerfDataS    Pointer to the Profile specific clock performance
                                 information to return.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or clockPerfDataS are invalid or NULL.
     ZL303XX_INVALID_OPERATION     The specified clock is not running the correct profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238ClockPerformanceGet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpC37p238ClockPerformanceS *clockPerfDataS)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Get a pointer to the clock data. */
   /* This verifies the proper profile is set on the clock. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGetC37p238(clockHandle, &pPtpClock);
   }

   /* Check the data pointer. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(clockPerfDataS);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   /* Set the profile specific clockId parameter based on the version. */
   if (status == ZL303XX_OK)
   {
      /* Retrieve the Data. */
      clockPerfDataS->localClockId = pPtpClock->config.profileCfg.power.clockId;

      /* Set to the local value for now. If a Grand Master Stream exists, it
       * will be updated. */
      clockPerfDataS->grandmasterClockId = clockPerfDataS->localClockId;

      /* On the Master: timeInaccuracy = localTimeInaccuracy + networkTimeInaccuracy
       * On the Master: timeInaccuracy = TimeInaccuracy from the MASTER (in TLV) +
       *                                     localTimeInaccuracy + networkTimeInaccuracy   */
      /* For now, add the local values: if this is a slave and the MASTER info
       * is known, it will be added below.    */
      clockPerfDataS->timeInaccuracy = pPtpClock->config.profileCfg.power.localTimeInaccuracy +
                                       pPtpClock->config.profileCfg.power.networkTimeInaccuracy;

      /* All from the Clock data sets. */
      OS_MEMCPY(clockPerfDataS->grandmasterIdentity, pPtpClock->parentDS.grandmasterIdentity, 8);
      clockPerfDataS->timeTraceable = pPtpClock->timePropertiesDS.timeTraceable;
      zl303xx_PtpConvTimeStampToTimeInterval(&pPtpClock->currentDSInt.offset_from_master,
                                             &clockPerfDataS->offsetFromMaster);

      /* Retrieve the rest of the data from the active SLAVE stream. */
      if (pPtpClock->gmSource.pPtpStream != NULL)
      {
         clockPerfDataS->grandmasterClockId = pPtpClock->gmSource.pPtpStream->profileDS.power.clockId;

         /* The local Time Inaccuracy components were added (above). Now add the
          * far end components. */
         clockPerfDataS->timeInaccuracy += pPtpClock->gmSource.pPtpStream->profileDS.power.grandmasterTimeInaccuracy +
                                           pPtpClock->gmSource.pPtpStream->profileDS.power.networkTimeInaccuracy;
      }

      /* free the Mutex */
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* END zl303xx_PtpC37p238ClockPerformanceGet */

/* zl303xx_PtpC37p238ClockConfigGet */
/**
   Retrieves the clock's profile specific data parameter.

  Parameters:
   [in]  clockHandle       Handle to an existing clock.
   [in]  clockProfileCfgS  Pointer to the Profile specific clock information.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or clockProfileCfgS are invalid or NULL.
     ZL303XX_INVALID_OPERATION     The specified clock is not running the correct profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238ClockConfigGet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpC37p238ClockConfigS *clockProfileCfgS)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGetC37p238(clockHandle, &pPtpClock);
   }

   /* Check the localTimeZoneInfoS pointer. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(clockProfileCfgS);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   /* Set the profile specific clockId parameter based on the version. */
   if (status == ZL303XX_OK)
   {
      /* Retrieve the Data. */
      *clockProfileCfgS = pPtpClock->config.profileCfg.power;

      /* free the Mutex */
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* END zl303xx_PtpC37p238ClockConfigGet */

/* zl303xx_PtpC37p238ClockIdSet */
/**
   Updates a clock's profile specific clockId parameter. Even though the profile
   (or version) may have a VALID range, this parameter will be set regardless
   (in case the the clock id being turned OFF). If outside the range, a TRACE
   log is generated.

  Parameters:
   [in]  clockHandle      Handle to an existing clock.
   [in]  C37p238clockId   Profile specific ClockId to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid.
     ZL303XX_INVALID_OPERATION     The specified clock is not running the correct profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238ClockIdSet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint16T C37p238clockId)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;
   zl303xx_BooleanE outsideRange = ZL303XX_FALSE;

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGetC37p238(clockHandle, &pPtpClock);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   /* Set the profile specific clockId parameter based on the version. */
   if (status == ZL303XX_OK)
   {
      if (pPtpClock->config.profile == ZL303XX_PTP_PROFILE_POWER_C37_238_2011)
      {
         if ((C37p238clockId < DEFAULT_C37_238_2011_CLK_ID_MIN) ||
             (C37p238clockId > DEFAULT_C37_238_2011_CLK_ID_MAX))
         {
            outsideRange = ZL303XX_TRUE;
         }
      }
      else if (pPtpClock->config.profile == ZL303XX_PTP_PROFILE_POWER_C37_238_2011)
      {
         if ((C37p238clockId < DEFAULT_C37_238_2014_CLK_ID_MIN) ||
             (C37p238clockId > DEFAULT_C37_238_2014_CLK_ID_MAX))
         {
            outsideRange = ZL303XX_TRUE;
         }
      }

      /* TRACE if not valid but apply */
      if (outsideRange == ZL303XX_TRUE)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpC37p238ClockIdSet: ID(%u) out of range for profile(%u)",
               C37p238clockId, pPtpClock->config.profile, 0,0,0,0);
      }

      /* Apply the value. */
      pPtpClock->config.profileCfg.power.clockId = C37p238clockId;

      /* free the Mutex */
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* END zl303xx_PtpC37p238ClockIdSet */

/* zl303xx_PtpC37p238LocalTimeInaccSet */
/**
   Sets a clock's profile specific Local Time Inaccuracy value. On master
   clocks, the localTimeInaccuracy value is written to the
   grandmasterTimeInaccuracy field of the IEEE_C37_238 TLV.

  Parameters:
   [in]  clockHandle          Handle to an existing clock.
   [in]  localTimeInaccuracy  Local Time Inaccuracy value to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid.
     ZL303XX_INVALID_OPERATION     The specified clock is not running the correct profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238LocalTimeInaccSet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint32T localTimeInaccuracy)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGetC37p238(clockHandle, &pPtpClock);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   /* Set the profile specific clockId parameter based on the version. */
   if (status == ZL303XX_OK)
   {
      /* Apply the value. */
      pPtpClock->config.profileCfg.power.localTimeInaccuracy = localTimeInaccuracy;

      /* free the Mutex */
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* END zl303xx_PtpC37p238LocalTimeInaccSet */

/* zl303xx_PtpC37p238NetworkTimeInaccSet */
/**
   Sets a clock's profile specific Network Time Inaccuracy value.

  Parameters:
   [in]  clockHandle             Handle to an existing clock.
   [in]  networkTimeInaccuracy   Network Time Inaccuracy value to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid.
     ZL303XX_INVALID_OPERATION     The specified clock is not running the correct profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238NetworkTimeInaccSet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint32T networkTimeInaccuracy)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGetC37p238(clockHandle, &pPtpClock);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   /* Set the profile specific clockId parameter based on the version. */
   if (status == ZL303XX_OK)
   {
      /* Apply the value. */
      pPtpClock->config.profileCfg.power.networkTimeInaccuracy = networkTimeInaccuracy;

      /* free the Mutex */
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* END zl303xx_PtpC37p238NetworkTimeInaccSet */

/* zl303xx_PtpC37p238LocalTimeZoneSet */
/**
   Updates a clock's profile specific LOCAL TIME ZONE parameter.

  Parameters:
   [in]  clockHandle          Handle to an existing clock.
   [in]  localTimeZoneInfoS   Profile specific LOCAL TIME ZONE information.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle or localTimeZoneInfoS are invalid or NULL.
     ZL303XX_INVALID_OPERATION     The specified clock is not running the correct profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238LocalTimeZoneSet(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpC37p238LocalTimeZoneS *localTimeZoneInfoS)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Get a pointer to the clock data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDataGetC37p238(clockHandle, &pPtpClock);
   }

   /* Check the localTimeZoneInfoS pointer. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(localTimeZoneInfoS);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpClock);
   }

   /* Set the profile specific clockId parameter based on the version. */
   if (status == ZL303XX_OK)
   {
      /* Apply the Data. */
      pPtpClock->config.profileCfg.power.localTimeZoneInfo = *localTimeZoneInfoS;

      /* free the Mutex */
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}  /* END zl303xx_PtpC37p238LocalTimeZoneSet */

/* zl303xx_PtpC37p238StreamLocalTimeInaccMaxSet */
/**
   Sets a stream's profile specific Local Time Inaccuracy MAX value. Beyond this
   value, an Alarm / PTSF will be raised by the stream.

  Parameters:
   [in]  clockHandle             Handle to an existing stream.
   [in]  localTimeInaccuracyMax  Maximum allowed Local Time Inaccuracy value.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle is invalid.
     ZL303XX_INVALID_OPERATION     The specified stream is not running the correct profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238StreamLocalTimeInaccMaxSet(
      zl303xx_PtpStreamHandleT streamHandle,
      Uint32T localTimeInaccuracyMax)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Get a pointer to the stream data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGetC37p238(streamHandle, &pPtpStream);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpStream->portData->clock);
   }

   /* Set the profile specific localTimeInaccuracyMax parameter based on the version. */
   if (status == ZL303XX_OK)
   {
      /* Apply the value. */
      pPtpStream->config.profileCfg.power.localTimeInaccuracyMax = localTimeInaccuracyMax;

      /* free the Mutex */
      status = zl303xx_PtpClockMutexUnlock(pPtpStream->portData->clock, status);
   }

   return status;
}  /* END zl303xx_PtpC37p238StreamLocalTimeInaccMaxSet */

/* zl303xx_PtpC37p238StreamLocalTimeInaccMaxSet */
/**
   Sets a stream's profile specific Network Time Inaccuracy MAX value. Beyond this
   value, an Alarm / PTSF will be raised by the stream.

  Parameters:
   [in]  clockHandle                Handle to an existing stream.
   [in]  networkTimeInaccuracyMax   Maximum allowed Network Time Inaccuracy value.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle is invalid.
     ZL303XX_INVALID_OPERATION     The specified stream is not running the correct profile.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpC37p238StreamNetworkTimeInaccMaxSet(
      zl303xx_PtpStreamHandleT streamHandle,
      Uint32T networkTimeInaccuracyMax)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pPtpStream = NULL;

   /* Get a pointer to the stream data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGetC37p238(streamHandle, &pPtpStream);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpStream->portData->clock);
   }

   /* Set the profile specific networkTimeInaccuracyMax parameter based on the version. */
   if (status == ZL303XX_OK)
   {
      /* Apply the value. */
      pPtpStream->config.profileCfg.power.networkTimeInaccuracyMax = networkTimeInaccuracyMax;

      /* free the Mutex */
      status = zl303xx_PtpClockMutexUnlock(pPtpStream->portData->clock, status);
   }

   return status;
}  /* END zl303xx_PtpC37p238StreamNetworkTimeInaccMaxSet */


/******************************************************************************/
/*******   INTERNAL FUNCTION DEFINITIONS   ************************************/
/******************************************************************************/

/*

  Function Name:
    zl303xx_PtpC37p238v1TlvUnpack

  Details:
   Unpacks the PC37.238-2011, Power Profile specific TLV into a compatible data
   type for local use.

  Parameters:
   [in]  pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out] pTlvDataS      Pointer to the unpacked Power Profile specific TLV.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpC37p238v1TlvUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_PtpC37p238v1TlvS *pTlvDataS)
{
   /* PC37.238-2011, Power Profile specific TLV Composition
    * (PC37.238-2011: Section 5.12.2):
    * -------------------------------------------------------
    * Field                      Length    Offset
    * ------------------------------------------------------
    * grandmasterID                2         0
    * grandmasterTimeInaccuracy    4         2
    * networkTimeInaccuracy        4         6
    * RESERVED                     2        10  (all 0's)
    * ------------------------------------------------------
    * Total Length                12    */

   /* grandmasterID */
   pTlvDataS->clockId = UNPACK_16BIT(pInTlvBuf + 0);

   /* grandmasterTimeInaccuracy */
   pTlvDataS->grandmasterTimeInaccuracy = UNPACK_32BIT(pInTlvBuf + 2);

   /* networkTimeInaccuracy */
   pTlvDataS->networkTimeInaccuracy = UNPACK_32BIT(pInTlvBuf + 6);

   return ZL303XX_PTP_C37_238_2011_TLV_LEN;
}  /* END zl303xx_PtpC37p238v1TlvUnpack */

/*

  Function Name:
    zl303xx_PtpC37p238v1TlvPack

  Details:
   Packs the PC37.238-2011, Power Profile specific structure into a TLV buffer.

  Parameters:
   [in]  pTlvDataS      Pointer to the Power Profile structure to pack.

   [out] pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpC37p238v1TlvPack(
      zl303xx_PtpC37p238v1TlvS *pTlvDataS,
      Uint8T *pOutTlvBuf)
{
   /* Refer to the UNPACK routine (above) for TLV detailed layout.    */

   /* Clear the output data buffer. */
   /* Will set the RESERVED fields to 0's. */
   OS_MEMSET(pOutTlvBuf, 0x00, ZL303XX_PTP_C37_238_2011_TLV_LEN);

   /* grandmasterID */
   PACK_16BIT((pOutTlvBuf + 0), pTlvDataS->clockId);

   /* grandmasterTimeInaccuracy */
   PACK_32BIT((pOutTlvBuf + 2), pTlvDataS->grandmasterTimeInaccuracy);

   /* networkTimeInaccuracy */
   PACK_32BIT((pOutTlvBuf + 6), pTlvDataS->networkTimeInaccuracy);

   return ZL303XX_PTP_C37_238_2011_TLV_LEN;
}  /* END zl303xx_PtpC37p238v1TlvPack */

/*

  Function Name:
    zl303xx_PtpC37p238v2TlvUnpack

  Details:
   Unpacks the PC37.238-2014, Power Profile specific TLV into a compatible data
   type for local use.

  Parameters:
   [in]  pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out] pTlvDataS      Pointer to the unpacked Power Profile specific TLV.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpC37p238v2TlvUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_PtpC37p238v2TlvS *pTlvDataS)
{
   /* PC37.238-2014, Power Profile specific TLV Composition
    * (PC37.238-2014: Section 5.12.2):
    * -------------------------------------------------------
    * Field                      Length    Offset
    * ------------------------------------------------------
    * RESERVED                     3         0
    * grandmasterID                2         3
    * grandmasterTimeInaccuracy    4         5
    * networkTimeInaccuracy        4         9
    * numberExtensions             1        13
    * ------------------------------------------------------
    * Total Length                14    */

   /* Number of extension fields appended to the TLV. */
   Uint8T numberExtensions = 0;
   Uint8T extId = 0;

   Uint8T extType;
   Uint8T extLen;

   /* The minimum bytes processed will be this (14 bytes). */
   Uint8T bytesProcessed = ZL303XX_PTP_C37_238_2014_TLV_LEN_MIN;

   /* grandmasterID */
   pTlvDataS->clockId = UNPACK_16BIT(pInTlvBuf + 3);

   /* grandmasterTimeInaccuracy */
   pTlvDataS->grandmasterTimeInaccuracy = UNPACK_32BIT(pInTlvBuf + 5);

   /* networkTimeInaccuracy */
   pTlvDataS->networkTimeInaccuracy = UNPACK_32BIT(pInTlvBuf + 9);

   /* Process any TLV Extensions */
   numberExtensions = UNPACK_8BIT(pInTlvBuf + 13);

   /* bytesProcessed was set above */
   for (extId = 0; extId < numberExtensions; extId++)
   {
      /* Get the extension Type and Length. */
      extType = pInTlvBuf[bytesProcessed];
      extLen  = pInTlvBuf[bytesProcessed + 1];

      /* Currently one 1 extension supported. */
      if (extType == ZL303XX_PTP_C38_238_EXT_LOCAL_TIME_ZONE)
      {
         (void)zl303xx_PtpC37p238LocalTimeZoneExtUnpack(
                                    (pInTlvBuf + bytesProcessed),
                                    &pTlvDataS->localTimeZoneInfo);
      }
      else
      {
         ZL303XX_TRACE_ALWAYS("zl303xx_PtpC37p238v2TlvUnpack: Unknown extension(%u)",
               extType, 0,0,0,0,0);
      }

      bytesProcessed += extLen;
   }

   return bytesProcessed;
}  /* END zl303xx_PtpC37p238v2TlvUnpack */

/*

  Function Name:
    zl303xx_PtpC37p238v2TlvPack

  Details:
   Packs the PC37.238-2014, Power Profile specific structure into a TLV buffer.

  Parameters:
   [in]  pTlvDataS      Pointer to the Power Profile structure to pack.

   [out] pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpC37p238v2TlvPack(
      zl303xx_PtpC37p238v2TlvS *pTlvDataS,
      Uint8T *pOutTlvBuf)
{
   /* PC37.238-2014, Power Profile specific TLV Composition
    * (PC37.238-2014: Section 5.12.2):
    * -------------------------------------------------------
    * Field                      Length    Offset
    * ------------------------------------------------------
    * RESERVED                     3         0
    * grandmasterID                2         3
    * grandmasterTimeInaccuracy    4         5
    * networkTimeInaccuracy        4         9
    * numberExtensions             1        13
    * ------------------------------------------------------
    * Total Length                14    */

   /* The minimum bytes processed will be this (14 bytes). */
   Uint8T bytesProcessed = ZL303XX_PTP_C37_238_2014_TLV_LEN_MIN;

   /* Only add the extension if either stdTimeOffsetToUtc or dstTimeOffsetToUtc
    * is non-zero. */
   Uint8T numberExtensions = 0;
   if ((pTlvDataS->localTimeZoneInfo.stdTimeOffsetToUtc != 0) ||
       (pTlvDataS->localTimeZoneInfo.dstTimeOffsetToUtc != 0))
   {
      numberExtensions = 1;
   }

   /* Clear the output data buffer. */
   /* Will set the RESERVED fields to 0's. */
   OS_MEMSET(pOutTlvBuf, 0x00, ZL303XX_PTP_C37_238_2014_TLV_LEN_MIN);

   /* 3 RESERVED bytes. */

   /* grandmasterID */
   PACK_16BIT((pOutTlvBuf + 3), pTlvDataS->clockId);

   /* grandmasterTimeInaccuracy */
   PACK_32BIT((pOutTlvBuf + 5), pTlvDataS->grandmasterTimeInaccuracy);

   /* networkTimeInaccuracy */
   PACK_32BIT((pOutTlvBuf + 9), pTlvDataS->networkTimeInaccuracy);

   /* Add any TLV Extensions: we would only support adding 1 */
   PACK_8BIT((pOutTlvBuf + 13), numberExtensions);

   /* Add the extension (if necessary). */
   if (numberExtensions != 0)
   {
      bytesProcessed += zl303xx_PtpC37p238LocalTimeZoneExtPack(
                                       &pTlvDataS->localTimeZoneInfo,
                                       (pOutTlvBuf + bytesProcessed));
   }

   return bytesProcessed;
}  /* END zl303xx_PtpC37p238v2TlvPack */

/*

  Function Name:
    zl303xx_PtpC37p238LocalTimeZoneExtUnpack

  Details:
   Unpacks the Power Profile specific Local Time Zone extension.

  Parameters:
   [in]  pInExtBuf         Pointer to the start of the extension buffer.

   [out] localTimeZoneS    Pointer to the unpacked Local Time Zone structure.

  Return Value:
   Uint16T  Number of bytes unpacked from the Extension buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpC37p238LocalTimeZoneExtUnpack(
      Uint8T *pInExtBuf,
      zl303xx_PtpC37p238LocalTimeZoneS *localTimeZoneS)
{
   /* PC37.238-2014, Local Time Zone Extension
    * (PC37.238-2014: Section 5.12.2):
    * -------------------------------------------------------
    * Field                      Length    Offset
    * ------------------------------------------------------
    * extensionType                1         0
    * extensionLength              1         1
    * keyField                     1         2
    * flagField                    1         3
    *    includesDst[bit-0]
    * RESERVED                     2         4
    * stdTimeOffsetToUtc           4         6
    * dstTimeOffsetToUtc           4        10
    * stdTimeStartsAt              2        14
    *    month [bits 15:12]
    *    week  [bits 11: 9]
    *    day   [bits  8: 6]
    *    hour  [bits  5: 1]
    *    reserved [bit   0]
    * dstTimeStartsAt              2        16
    *    month [bits 15:12]
    *    week  [bits 11: 9]
    *    day   [bits  8: 6]
    *    hour  [bits  5: 1]
    *    reserved [bit   0]
    * stdTimeZoneName              X        18
    * dstTimeZoneName              Y        18 + X
    * pad                          M        18 + X + Y
    * ------------------------------------------------------
    * Total Length         18 + X + Y + M             */

   Uint8T extensionType;
   Uint8T extensionLength;
   Uint16T bytesProcessed;

   /* Check the actual extension type. */
   extensionType   = UNPACK_8BIT(pInExtBuf + 0);
   extensionLength = UNPACK_8BIT(pInExtBuf + 1);

   if (extensionType != ZL303XX_PTP_C38_238_EXT_LOCAL_TIME_ZONE)
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_PtpC37p238LocalTimeZoneExtUnpack: wrong ext value (%u:%u)",
            extensionType, extensionLength, 0,0,0,0);
      return 0;
   }

   /*** No expected failures beyond here. ****/

   /* keyField */
   localTimeZoneS->keyField = UNPACK_8BIT(pInExtBuf + 2);

   /* flagField */
   {
      /*                                                               Bit */
      localTimeZoneS->includesDst = (UNPACK_8BIT(pInExtBuf + 3) & (1 << 0));
   }

   /* 2 Reserved bytes here. */

   /* stdTimeOffsetToUtc */
   localTimeZoneS->stdTimeOffsetToUtc = UNPACK_32BIT(pInExtBuf + 6);

   /* dstTimeOffsetToUtc */
   localTimeZoneS->dstTimeOffsetToUtc = UNPACK_32BIT(pInExtBuf + 10);

   /* stdTimeStartsAt */
   /**********************************************************************************
    *  15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
    **********************************************************************************
    *        Month       |  Week of Mth |  Day of Week |       Hour of Day      |  0 |
    **********************************************************************************/
   {
      Uint16T stdTimeStartsAt = UNPACK_16BIT(pInExtBuf + 14);
      /*                                                        LSBit        MSBit  LSBit  */
      localTimeZoneS->stdTimeStartsAt.month = (stdTimeStartsAt >> 12) & ((1 << (15 - 12 + 1)) - 1);
      localTimeZoneS->stdTimeStartsAt.week  = (stdTimeStartsAt >>  9) & ((1 << (11 -  9 + 1)) - 1);
      localTimeZoneS->stdTimeStartsAt.day   = (stdTimeStartsAt >>  6) & ((1 << ( 8 -  6 + 1)) - 1);
      localTimeZoneS->stdTimeStartsAt.hour  = (stdTimeStartsAt >>  1) & ((1 << ( 5 -  1 + 1)) - 1);
   }

   /* dstTimeStartsAt */
   /**********************************************************************************
    *  15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
    **********************************************************************************
    *        Month       |  Week of Mth |  Day of Week |       Hour of Day      |  0 |
    **********************************************************************************/
   {
      Uint16T dstTimeStartsAt = UNPACK_16BIT(pInExtBuf + 16);
      /*                                                        LSBit        MSBit  LSBit  */
      localTimeZoneS->dstTimeStartsAt.month = (dstTimeStartsAt >> 12) & ((1 << (15 - 12 + 1)) - 1);
      localTimeZoneS->dstTimeStartsAt.week  = (dstTimeStartsAt >>  9) & ((1 << (11 -  9 + 1)) - 1);
      localTimeZoneS->dstTimeStartsAt.day   = (dstTimeStartsAt >>  6) & ((1 << ( 8 -  6 + 1)) - 1);
      localTimeZoneS->dstTimeStartsAt.hour  = (dstTimeStartsAt >>  1) & ((1 << ( 5 -  1 + 1)) - 1);
   }

   /* At this point, we are sure to have processed 18 bytes (type + length + data). */
   bytesProcessed = ZL303XX_PTP_C38_238_EXT_LOCAL_TIME_ZONE_LENGTH_MIN;

   /* stdTimeZoneName */
   bytesProcessed += zl303xx_PtpV2MsgTextUnpack(
                              (pInExtBuf + bytesProcessed),
                              localTimeZoneS->stdTimeZoneName,
                              ZL303XX_PTP_C38_238_TIME_ZONE_NAME_MAX_LEN);

   /* stdTimeZoneName */
   bytesProcessed += zl303xx_PtpV2MsgTextUnpack(
                              (pInExtBuf + bytesProcessed),
                              localTimeZoneS->dstTimeZoneName,
                              ZL303XX_PTP_C38_238_TIME_ZONE_NAME_MAX_LEN);

   /* Return the packed value so that the calling routine knows where the TLV ends
    * within a larger buffer. */
   return extensionLength;
}

/*

  Function Name:
    zl303xx_PtpC37p238LocalTimeZoneExtPack

  Details:
   Unpacks the Power Profile specific Local Time Zone extension.

  Parameters:
   [in]  localTimeZoneS    Pointer to the Local Time Zone structure to pack.

   [in]  pOutExtBuf        Pointer to the start of the extension buffer
                                 into which the data will be packed.

  Return Value:
   Uint16T  Number of bytes packed into the Extension buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpC37p238LocalTimeZoneExtPack(
      zl303xx_PtpC37p238LocalTimeZoneS *localTimeZoneS,
      Uint8T *pOutExtBuf)
{
   /* Refer to the zl303xx_PtpC37p238LocalTimeZoneExtUnpack() routine for
    * extension composition. */

   Uint16T bytesProcessed;

   /* Pack extension type. */
   PACK_8BIT((pOutExtBuf + 0), ZL303XX_PTP_C38_238_EXT_LOCAL_TIME_ZONE);

   /* Extension Length will be packed later (is data length NOT including the
    * 2 bytes of type and length. */

   /* keyField */
   PACK_8BIT((pOutExtBuf + 2), localTimeZoneS->keyField);

   /* flagField */
   {
      Uint8T flagField = 0;

      /* includesDst */
      flagField |= ((localTimeZoneS->includesDst & 0x01) << 0);

      PACK_8BIT((pOutExtBuf + 3), flagField);
   }

   /* 2 Reserved bytes here. */

   /* stdTimeOffsetToUtc */
   PACK_32BIT((pOutExtBuf + 6),localTimeZoneS->stdTimeOffsetToUtc);

   /* dstTimeOffsetToUtc */
   PACK_32BIT((pOutExtBuf + 10),localTimeZoneS->dstTimeOffsetToUtc);

   /* stdTimeStartsAt */
   {
      Uint16T stdTimeStartsAt = 0;

      /**********************************************************************************
       *  15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
       **********************************************************************************
       *        Month       |  Week of Mth |  Day of Week |       Hour of Day      |  0 |
       **********************************************************************************/
      /*                                                               MSBit  LSBit          LSBit*/
      stdTimeStartsAt |= (localTimeZoneS->stdTimeStartsAt.month & ((1 << (15 - 12 + 1)) - 1)) << 12;
      stdTimeStartsAt |= (localTimeZoneS->stdTimeStartsAt.week  & ((1 << (11 -  9 + 1)) - 1)) <<  9;
      stdTimeStartsAt |= (localTimeZoneS->stdTimeStartsAt.day   & ((1 << ( 8 -  6 + 1)) - 1)) <<  6;
      stdTimeStartsAt |= (localTimeZoneS->stdTimeStartsAt.hour  & ((1 << ( 5 -  1 + 1)) - 1)) <<  1;

      PACK_16BIT((pOutExtBuf + 14), stdTimeStartsAt);
   }

   /* dstTimeStartsAt */
   {
      Uint16T dstTimeStartsAt = 0;

      /**********************************************************************************
       *  15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
       **********************************************************************************
       *        Month       |  Week of Mth |  Day of Week |       Hour of Day      |  0 |
       **********************************************************************************/
      /*                                                               MSBit  LSBit          LSBit*/
      dstTimeStartsAt |= (localTimeZoneS->dstTimeStartsAt.month & ((1 << (15 - 12 + 1)) - 1)) << 12;
      dstTimeStartsAt |= (localTimeZoneS->dstTimeStartsAt.week  & ((1 << (11 -  9 + 1)) - 1)) <<  9;
      dstTimeStartsAt |= (localTimeZoneS->dstTimeStartsAt.day   & ((1 << ( 8 -  6 + 1)) - 1)) <<  6;
      dstTimeStartsAt |= (localTimeZoneS->dstTimeStartsAt.hour  & ((1 << ( 5 -  1 + 1)) - 1)) <<  1;

      PACK_16BIT((pOutExtBuf + 16), dstTimeStartsAt);
   }

   /* At this point, we are sure to have processed 18 bytes of data. */
   bytesProcessed = ZL303XX_PTP_C38_238_EXT_LOCAL_TIME_ZONE_LENGTH_MIN;

   /* stdTimeZoneName */
   bytesProcessed += zl303xx_PtpV2MsgTextPack(
                              localTimeZoneS->stdTimeZoneName,
                              (pOutExtBuf + bytesProcessed),
                              ZL303XX_PTP_C38_238_TIME_ZONE_NAME_MAX_LEN);

   /* stdTimeZoneName */
   bytesProcessed += zl303xx_PtpV2MsgTextPack(
                              localTimeZoneS->dstTimeZoneName,
                              (pOutExtBuf + bytesProcessed),
                              ZL303XX_PTP_C38_238_TIME_ZONE_NAME_MAX_LEN);

   /* Add a PAD byte to make it even if necessary. */
   if ((bytesProcessed % 2) != 0)
   {
      OS_MEMSET((pOutExtBuf + bytesProcessed), 0x00, 1);
      bytesProcessed += 1;
   }

   /* Set the length in the extension (is data length NOT including the 2 bytes
    * of type and length). */
   PACK_8BIT((pOutExtBuf + 1), (bytesProcessed - 2));

   /* Return the total bytes of the extension which includes the 2 bytes of
    * type and length. */
   return bytesProcessed;
}

/**

  Function Name:
   zl303xx_PtpClockDataGetC37p238

  Details:
   Gets the clock data pointer for the specified clock handle and verifies that
   the clock is operating with the required C37.238 Profile.

  Parameters:
   [in]   clockHandle    Handle to a specific PTP clock
   [out]  ppPtpClock     Pointer to a zl303xx_PtpClockS structure

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     clockHandle is invalid.
     ZL303XX_INVALID_OPERATION     The specified clock is not running the correct profile.

*******************************************************************************/
zlStatusE zl303xx_PtpClockDataGetC37p238(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpClockS **ppPtpClock)
{
   zlStatusE status = zl303xx_PtpClockDataGet(clockHandle, ppPtpClock);

   if (status == ZL303XX_OK)
   {
      if (((*ppPtpClock)->config.profile != ZL303XX_PTP_PROFILE_POWER_C37_238_2011) &&
          ((*ppPtpClock)->config.profile != ZL303XX_PTP_PROFILE_POWER_C37_238_2014))
      {
         const char * profileDescrStr = zl303xx_PtpProfileIdToStr((*ppPtpClock)->config.profile);
         ZL303XX_TRACE_ALWAYS(
               "Clock(%u): Power Profile not in use (%u)(%s).",
               clockHandle, (*ppPtpClock)->config.profile,
               profileDescrStr, 0,0,0);

         status = ZL303XX_INVALID_OPERATION;
      }
   }

   return status;
}

/**

  Function Name:
   zl303xx_PtpStreamDataGetC37p238

  Details:
   Gets the stream data pointer for the specified stream handle and verifies that
   the associated clock is operating with the required C37.238 Profile.

  Parameters:
   [in]   streamHandle   Handle to a specific PTP clock
   [out]  ppPtpStream    Pointer to a zl303xx_PtpClockS structure

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     streamHandle is invalid.
     ZL303XX_INVALID_OPERATION     The specified stream is not running the correct profile.

*******************************************************************************/
zlStatusE zl303xx_PtpStreamDataGetC37p238(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_ClockStreamS **ppPtpStream)
{
   zlStatusE status = zl303xx_PtpStreamDataGet(streamHandle, ppPtpStream);

   if (status == ZL303XX_OK)
   {
      if (((*ppPtpStream)->portData->clock->config.profile != ZL303XX_PTP_PROFILE_POWER_C37_238_2011) &&
          ((*ppPtpStream)->portData->clock->config.profile != ZL303XX_PTP_PROFILE_POWER_C37_238_2014))
      {
         const char * profileDescrStr =
               zl303xx_PtpProfileIdToStr((*ppPtpStream)->portData->clock->config.profile);

         ZL303XX_TRACE_ALWAYS(
               "Clock(%u): Power Profile not in use (%u)(%s).",
               streamHandle, (*ppPtpStream)->portData->clock->config.profile,
               profileDescrStr, 0,0,0);

         status = ZL303XX_INVALID_OPERATION;
      }
   }

   return status;
}




