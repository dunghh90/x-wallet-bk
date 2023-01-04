

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This header file contains the data types and functions required to support
*     the G.8265.1 standard (Telecom Profile for Frequency).
*
*******************************************************************************/

#ifndef ZL303XX_PTP_G8265_1_H_
#define ZL303XX_PTP_G8265_1_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Error.h"

#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpG8265p1Types.h"
#include "zl303xx_PtpDatatypes.h"

/*****************   DEFINES   ************************************************/

/* Packet rate ranges allowed by the Telecom Profile for Frequency. */
#define ZL303XX_PTP_TELECOM_LOG_SYNC_MIN   (Sint8T)(-7)  /* 128 pps  */
#define ZL303XX_PTP_TELECOM_LOG_SYNC_MAX   (Sint8T)(4)   /* 1/16 pps */

#define ZL303XX_PTP_TELECOM_LOG_DELAY_MIN  (Sint8T)(-7)  /* 128 pps  */
#define ZL303XX_PTP_TELECOM_LOG_DELAY_MAX  (Sint8T)(4)   /* 1/16 pps */

#define ZL303XX_PTP_TELECOM_LOG_ANNC_MIN   (Sint8T)(-3)  /* 8 pps    */
#define ZL303XX_PTP_TELECOM_LOG_ANNC_MAX   (Sint8T)(4)   /* 1/16 pps */
#define ZL303XX_PTP_TELECOM_LOG_ANNC_DEF   (Sint8T)(1)   /* 2 pps (default) */

/* Unicast negotiation contract length range in seconds. */
#define ZL303XX_PTP_TELECOM_UN_DURATION_MIN   (Uint32T)(60)
#define ZL303XX_PTP_TELECOM_UN_DURATION_MAX   (Uint32T)(1000)
#define ZL303XX_PTP_TELECOM_UN_DURATION_DEF   (Uint32T)(300)   /* default */

/* Unicast Master Table logQueryInterval. Must be greater than 0 to ensure
 * there is at least 1 second between contract REQUESTs. */
#define ZL303XX_PTP_TELECOM_LQI_MIN  (Sint8T)0

/* Domain number range */
#define ZL303XX_PTP_TELECOM_DOMAIN_NUM_MIN   23			/* 4->23 */
#define ZL303XX_PTP_TELECOM_DOMAIN_NUM_MAX   43
#define ZL303XX_PTP_TELECOM_DOMAIN_NUM_DEF   ZL303XX_PTP_TELECOM_DOMAIN_NUM_MIN
#define ZL303XX_PTP_TELECOM_DOMAIN_NUM_CHECK(domain) \
           ((((domain) < ZL303XX_PTP_TELECOM_DOMAIN_NUM_MIN) || \
             ((domain) > ZL303XX_PTP_TELECOM_DOMAIN_NUM_MAX))   \
                ? ZL303XX_PARAMETER_INVALID : ZL303XX_OK)

/* Master clock class */
#define ZL303XX_PTP_TELECOM_CLOCK_CLASS_DEF   (Uint8T)110
#define ZL303XX_PTP_TELECOM_CLOCK_CLASS_CHECK(c)  \
           ((((c) & 1) || ((c) < 80) || ((c) > 110)) ? (ZL303XX_PARAMETER_INVALID)   \
                                                     : (ZL303XX_OK))

/* Master SSM QL */
#define ZL303XX_PTP_TELECOM_SSM_QL_INVALID  (Uint8T)0xFF
#define ZL303XX_PTP_TELECOM_SSM_QL_CHECK(q) (((q) <= 0xF) ? (ZL303XX_OK)   \
                                                        : (ZL303XX_PARAMETER_INVALID))

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   PROFILE BINDING FUNCTION DECLARATIONS   ******************/

/* Routines to get data about the Profile or execute profile specific actions.
 * Ideally, these would be bound to the parent stack.  */
zlStatusE zl303xx_PtpG8265p1ProfileInfoGet(const zl303xx_PtpProfileS **profileInfo);

/* Routine to set profile specific values for PTP Objects. */
zlStatusE zl303xx_PtpG8265p1StreamDefaults(void *pStreamInitS);

/* Routines to execute any Post-PTP Object creation actions for the profile.
 * For example, BIND a custom TLV handler to the clock or set internal parameters. */
/* Currently nothing defined for the Port or Stream. */
zlStatusE zl303xx_PtpG8265p1ClockCreateActions(void *pClockS);

/* Routines to exercise Profile specific Data Set updates. */
zlStatusE zl303xx_PtpG8265p1ClockQLUpdate(void *pClock);

/* Routines to do common parameter checks against Profile ranges, etc. */
zlStatusE zl303xx_PtpG8265p1TransportSpecificCheck(Uint8T *transportSpecific, zl303xx_BooleanE strictFlag);
zlStatusE zl303xx_PtpG8265p1ClockQualityCheck(zl303xx_ClockQuality *clockQuality, zl303xx_BooleanE retError);
zlStatusE zl303xx_PtpG8265p1QueryIntervalCheck(Sint8T *logQueryInterval, zl303xx_BooleanE retError);
zlStatusE zl303xx_PtpG8265p1UniNegDurationCheck(Uint32T *duration, zl303xx_BooleanE retError);

/* Routine to Handle Profile specific BMCA data for stream entries.  */
zlStatusE zl303xx_PtpG8265p1BmcaDataAdd(
      zl303xx_ForeignMasterRecordS *pFmtEntry,
      zl303xx_PtpEventClockBmcaEntryS *pBmcaDataS);
/* Routine to Handle Profile specific BMCA data for Virtual Ports.  */
zlStatusE zl303xx_PtpG8265p1BmcaVpDataAdd(
      zl303xx_PortDataS *pPtpVirtualPort,
      void *pOutBmcaDataS);

/* Routines to print profile specific debug information for the PTP objects. */
zlStatusE zl303xx_PtpG8265p1ClockDebugShow(void *pClockS);


/*****************   API FUNCTIONS FOR PROFILE ATTRIBUTES   ********************/

/* Telecom Profile for Frequency-specific API functions. */
zlStatusE zl303xx_PtpG8265p1ClockQLGet(zl303xx_PtpClockHandleT clockHandle,
                                       zl303xx_PtpTelecomQualityLevelS *ql);
zlStatusE zl303xx_PtpClockWaitToRestoreSet(zl303xx_PtpClockHandleT clockHandle, Uint32T wtrSec);
zlStatusE zl303xx_PtpClockEvtSquelchEnSet(zl303xx_PtpClockHandleT clockHandle, zl303xx_BooleanE enabled);
zlStatusE zl303xx_PtpClockQLHoldOffSet(zl303xx_PtpClockHandleT clockHandle, Uint32T holdOffSec);

zlStatusE zl303xx_DebugTelecomBMCA(zl303xx_PtpClockHandleT clockHandle);

/*****************   INTERNAL PTP FUNCTION DECLARATIONS   *********************/

/* SSM Quality Level <-> PTP Clock Class conversion functions. */
const zl303xx_PtpTelecomQualityLevelS *zl303xx_PtpClockClassToQL(Uint8T clockClass);
zlStatusE zl303xx_PtpSsmQLToClockClass(Uint8T ssmQL, Uint8T *pClockClass);
void zl303xx_PtpTelecomBmcaDataUpdate(zl303xx_PtpTelecomUmtEntryIntS *pEntry,
                                      Uint8T clockClass, Uint16T stepsRemoved);

/* Parameter range check functions. */
zlStatusE zl303xx_PtpTelecomLogIntvlCheck(zl303xx_MessageTypeE type, Sint8T *logIntvl,
                                          zl303xx_BooleanE retError);

/* Unicast negotiation functions. */
void zl303xx_PtpTelecomMsgTimeoutCheck(zl303xx_PtpTelecomUmtEntryIntS *pEntry,
                                       zl303xx_MessageTypeE msgType,
                                       Sint8T intvl,
                                       zl303xx_PtpTelecomUmtEntryS *pConfig);

/* UMT extension functions. */
void      zl303xx_PtpTelecomUmtEntryStructInit(zl303xx_PtpTelecomUmtEntryS *pAdd);
zlStatusE zl303xx_PtpTelecomUmtEntryStructCheck(zl303xx_PtpTelecomUmtEntryS *pAdd);
void      zl303xx_PtpTelecomUmtEntryIntAdd(zl303xx_PtpTelecomUmtEntryS *pAdd,
                                           zl303xx_PtpTelecomUmtEntryIntS *pEntry);

/* PTSF related functions. */
void zl303xx_PtpTelecomPtsfEval(Uint32T streamHandle, zl303xx_BooleanE reqAnnc,
                                zl303xx_BooleanE reqSync, zl303xx_BooleanE reqDelayResp,
                                zl303xx_BooleanE reqPdelayResp);

/* Wait to restore timer functions. */
void zl303xx_PtpTelecomStreamBestMasterOnce(zl303xx_PtpTelecomUmtEntryIntS *pEntry);
void zl303xx_PtpTelecomWtrStart(Uint32T streamHandle,
                                zl303xx_PtpTelecomUmtEntryIntS *pEntry,
                                Uint32T wtrSec);
void zl303xx_PtpTelecomWtrStop(zl303xx_PtpTelecomUmtEntryIntS *pEntry);
zl303xx_BooleanE zl303xx_PtpTelecomWtrIsActive(zl303xx_PtpTelecomUmtEntryIntS *pEntry);
zl303xx_BooleanE zl303xx_PtpTelecomWtrExpired(zl303xx_PtpTelecomUmtEntryIntS *pEntry);
void zl303xx_PtpTelecomWtrUpdate(Uint32T streamHandle,
                                 zl303xx_PtpTelecomUmtEntryIntS *pEntry);

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */


