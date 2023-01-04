

/*****************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*   Implementation of the Unicast Discovery functionality.
*
******************************************************************************/

/****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Os.h"
#include "zl303xx_ErrTrace.h"

#include "zl303xx_Ptsf.h"

#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpDatatypes.h"
#include "zl303xx_PtpV2UnicastDiscovery.h"
#include "zl303xx_PtpFunctionExport.h"
#include "zl303xx_PtpG8265p1Api.h"

#ifdef OS_FREERTOS
#include "def.h"
#endif

#ifndef ntohl
  #include <netinet/in.h>
#endif

/****************   DEFINES     **********************************************/

/****************   STATIC DATA TYPES   **************************************/

/* All parameters stored in a Unicast Master Table entry. Used to access
 * parameters in a generic way to reduce code duplication. */
typedef enum
{
   UMT_PARAM_IN_USE = 0,
   UMT_PARAM_STREAM_HANDLE,
   UMT_PARAM_DEST_ADDR,
   UMT_PARAM_BMCA_ALLOW,
   UMT_PARAM_ANNC_INTVL,
   UMT_PARAM_SYNC_INTVL,             /*  5 */
   UMT_PARAM_DELAY_INTVL,
   UMT_PARAM_ANNC_TIMEOUT,
   UMT_PARAM_NEG_DURATION,
   UMT_PARAM_NEG_RENEW,
   UMT_PARAM_MAX_INTVL,              /* 10 */
   UMT_PARAM_TELECOM_PRIORITY,
   UMT_PARAM_TELECOM_SYNC_TIMEOUT,
   UMT_PARAM_TELECOM_FOLLOW_TIMEOUT,
   UMT_PARAM_TELECOM_SYNC_FOLLOW_TIMEOUT,
   UMT_PARAM_TELECOM_DELAY_TIMEOUT,  /* 15 */
   UMT_PARAM_TELECOM_PDELAY_TIMEOUT,
   UMT_PARAM_TELECOM_PDELAY_FOLLOW_TIMEOUT,
   UMT_PARAM_EXT_DATA,
   UMT_PARAM_TELECOM_SSM_QL,
   UMT_PARAM_TELECOM_RETRY,         /* 20 */
   UMT_PARAM_TELECOM_WTR_SET,

   /* Can only GET */
   UMT_PARAM_STATUS,
   UMT_NUM_PARAMS
} zl303xx_PtpUmtEntryParamE;

typedef struct
{
   zl303xx_MessageTypeE msgType;
   Sint8T intvl;
} umtEntryMaxIntvlS;

/****************   STATIC FUNCTION DECLARATIONS   ***************************/

static void ConvertUniDiscoveryToGeneric(zl303xx_MasterAddressS *maPar,
                                         zl303xx_GenericPortInfoS *gpaPar);
static void ConvertGenericToUniDiscovery(zl303xx_GenericPortInfoS *gpaPar,
                                         zl303xx_MasterAddressS *maPar);

static zlStatusE zl303xx_PtpUmtEntryStructCheck(zl303xx_PtpUmtEntryS *pEntry,
                                                zl303xx_PortDataS *pPort);
static void      zl303xx_PtpUmtEntryForceStreamConfig(zl303xx_PtpStreamCreateS *config);
static zlStatusE zl303xx_PtpUmtEntryIntAdd(zl303xx_PortDataS *pPort, zl303xx_PtpUmtEntryS *pEntry,
                                           zl303xx_BooleanE startStream,
                                           zl303xx_PtpUmtEntryIntS **ppEntryInt);
static zlStatusE zl303xx_PtpUmtEntryFind(zl303xx_PtpUmtS *pUmt, zl303xx_PortAddress *pPortAddr,
                                         zl303xx_BooleanE *pFound, zl303xx_PtpUmtEntryIntS **ppEntry);
static zlStatusE zl303xx_PtpUnicastMasterParamOper(zl303xx_PtpStreamHandleT streamHandle,
                                       zl303xx_PtpUmtEntryParamE param, zl303xx_BooleanE setParam,
                                       void *pVal, zl303xx_PtpUmtEntryIntS **ppEntry);
static zlStatusE zl303xx_PtpUmtEntryParamOper(zl303xx_PtpUmtEntryIntS *pEntry,
                                              zl303xx_PtpUmtEntryParamE param,
                                              zl303xx_BooleanE setParam,
                                              void *pVal,
                                              zl303xx_ClockStreamS *pStream);
static void zl303xx_PtpUmtEntryStatusFill(zl303xx_ClockStreamS *pStream,
                                           zl303xx_PtpUmtEntryStatusS *pStatus);
static zlStatusE zl303xx_PtpUmtEntryStartStream(zl303xx_PortDataS *pPort,
                                                zl303xx_PtpStreamCreateS *pCreate,
                                                Uint8T *extData,
                                                zl303xx_PtpUmtEntryIntS *pEntry);

/****************   STATIC GLOBAL VARIABLES   ********************************/

/****************   EXPORTED GLOBAL VARIABLES   ******************************/
extern zl303xx_PortDataS *Zl303xx_PtpPortArray[ZL303XX_PTP_NUM_PORTS_MAX];

/****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* zl303xx_PtpUmtActSizeGet */
/**
   Returns the number of entries in a port's Unicast Master Table.

  Parameters:
   [in]   portHandle   Handle to a previously created port.
   [out]  size         UMT's actualTableSize (number of entries).

*******************************************************************************/
zlStatusE zl303xx_PtpUmtActSizeGet(zl303xx_PtpPortHandleT portHandle, Uint16T *size)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(size);
   }

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   /* Return the actual size of the UMT. */
   if (status == ZL303XX_OK)
   {
      *size = pPort->unicastMasterTable.actualTableSize;
   }

   return status;
}

/* zl303xx_PtpUmtShow */
/**
   Prints a PTP port's Unicast Master Table to the console or logging stream.

  Parameters:
   [in]   portHandle     Handle to a previously created PTP port.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtShow(zl303xx_PtpPortHandleT portHandle)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort = NULL;

   /* Get a pointer to the port data. */
   if ((status == ZL303XX_OK) &&
       ((status = zl303xx_PtpPortDataGet(portHandle, &pPort)) != ZL303XX_OK))
   {
      ZL303XX_TRACE_ALWAYS("Port %lu: (ERROR) Port does not exist.",
            portHandle, 0,0,0,0,0);
   }

   /* Check if the UMT is initialized. */
   if ((status == ZL303XX_OK) &&
       ((status = zl303xx_PtpUmtCheck(&pPort->unicastMasterTable)) != ZL303XX_OK))
   {
      ZL303XX_TRACE_ALWAYS("Port %lu: (ERROR) Unicast Master Table not initialized yet.",
            portHandle,0,0,0,0,0);
   }

   /* Print out the UMT. */
   if (status == ZL303XX_OK)
   {
      Uint16T count = 0;
      zl303xx_PtpUmtEntryIntS *entry = NULL;
      zl303xx_PtpContractRetryStatusS *pMsgRetry = NULL;
      zl303xx_BooleanE telecomProfileEn = ZL303XX_FALSE;
      zl303xx_ClockStreamS *pStream = NULL;
      const char *str0, *str1, *str2;
      zl303xx_BooleanE flag, mask;
      zl303xx_TimeStamp timeStamp;

      if ((pPort->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1) ||
          (pPort->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_2))
      {
         telecomProfileEn = ZL303XX_TRUE;
      }

      ZL303XX_TRACE_ALWAYS("Port %lu: Unicast Master Table", portHandle,0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("   actualTableSize: %u", pPort->unicastMasterTable.actualTableSize, 0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("   maxTableSize:    %u\n", pPort->unicastMasterTable.maxTableSize, 0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("   Selected Stream: %ld (for entire CLOCK) ",
            ((pPort->clock->gmSource.pPtpStream)
                  ? (UnativeT)(pPort->clock->gmSource.pPtpStream->clockStreamHandle)
                  : (UnativeT)(-1)), 0,0,0,0,0);

      for (count = 0, entry = pPort->unicastMasterTable.master;
           count < pPort->unicastMasterTable.maxTableSize;
           count++, entry++)
      {
         if (entry->inUse == ZL303XX_TRUE)
         {
            ZL303XX_TRACE_ALWAYS("", 0,0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS("   Entry %u:", count, 0,0,0,0,0);

            if (zl303xx_PtpStreamDataGet(entry->streamHandle, &pStream) != ZL303XX_OK)
            {
               ZL303XX_TRACE_ALWAYS("      Error retrieving stream data (streamHandle=%lu).", entry->streamHandle, 0,0,0,0,0);
               continue;
            }

            ZL303XX_TRACE_ALWAYS("      Stream Handle:     %lu", entry->streamHandle, 0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS("      BMCA Select Allow: %s", ZL303XX_BOOL_TO_STR(entry->entryConfig.bmcaAllow), 0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS("      GM Clock Class:    %u", pStream->farEndParentDS.u.v2.grandmasterClockQuality.clockClass, 0,0,0,0,0);

            if (telecomProfileEn == ZL303XX_TRUE)
            {
               if (pPort->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1)
               {
                  Uint8T ssmQL = entry->telecom.pQuality->ssm;

                  if (entry->telecom.wtr > 0)
                     ZL303XX_TRACE_ALWAYS("      Wait to Restore:   %lu sec", entry->telecom.wtr, 0,0,0,0,0);

                  if (pStream->unicast.lockout.remainingSec > 0)
                     ZL303XX_TRACE_ALWAYS("      LOCKED OUT (%lu sec remaining in timeout)", pStream->unicast.lockout.remainingSec, 0,0,0,0,0);

                  ZL303XX_TRACE_ALWAYS("      Total Lockouts:    %lu", pStream->unicast.lockout.previousCount, 0,0,0,0,0);
                  ZL303XX_TRACE_ALWAYS("      Local Priority:    %u", entry->telecom.priority, 0,0,0,0,0);

                  if (ZL303XX_PTP_TELECOM_SSM_QL_CHECK(ssmQL) == ZL303XX_OK)
                     ZL303XX_TRACE_ALWAYS("      SSM Quality Level: %u%u%u%u", (ssmQL & 0x08) >> 3, (ssmQL & 0x04) >> 2,
                           (ssmQL & 0x02) >> 1, (ssmQL & 0x01), 0,0);
                  else
                     ZL303XX_TRACE_ALWAYS("      SSM Quality Level: INVALID (bad clock class)", 0,0,0,0,0,0);

                  str0 = zl303xx_PtpG781QLToStr(entry->telecom.pQuality->g781[0]);
                  str1 = zl303xx_PtpG781QLToStr(entry->telecom.pQuality->g781[1]);
                  str2 = zl303xx_PtpG781QLToStr(entry->telecom.pQuality->g781[2]);

                  ZL303XX_TRACE_ALWAYS("      G.781 QL (Opt I):  %s", str0, 0,0,0,0,0);
                  ZL303XX_TRACE_ALWAYS("              (Opt II):  %s", str1, 0,0,0,0,0);
                  ZL303XX_TRACE_ALWAYS("             (Opt III):  %s", str2, 0,0,0,0,0);
               }

               ZL303XX_TRACE_ALWAYS("      Overall PTSF:      %s", zl303xx_PtsfFlagGetAndMask(pStream->clockStreamHandle, ZL303XX_PTSF_MAIN) ? "FAILED" : "OK", 0,0,0,0,0);

               flag = zl303xx_PtsfFlagGet(entry->streamHandle, ZL303XX_PTSF_LOSS_ANNOUNCE);
               mask = zl303xx_PtsfMaskGet(entry->streamHandle, ZL303XX_PTSF_LOSS_ANNOUNCE);
               ZL303XX_TRACE_ALWAYS("      PTSF-lossAnnounce:  %d & mask: %d = %d",
                     flag, mask, flag & mask, 0,0,0);

               flag = zl303xx_PtsfFlagGet(entry->streamHandle, ZL303XX_PTSF_LOSS_SYNC);
               mask = zl303xx_PtsfMaskGet(entry->streamHandle, ZL303XX_PTSF_LOSS_SYNC);
               ZL303XX_TRACE_ALWAYS("      PTSF-lossSync:      %d & mask: %d = %d",
                     flag, mask, flag & mask, 0,0,0);

               flag = zl303xx_PtsfFlagGet(entry->streamHandle, ZL303XX_PTSF_LOSS_FOLLOW_UP);
               mask = zl303xx_PtsfMaskGet(entry->streamHandle, ZL303XX_PTSF_LOSS_FOLLOW_UP);
               ZL303XX_TRACE_ALWAYS("      PTSF-lossFollowUp:  %d & mask: %d = %d",
                     flag, mask, flag & mask, 0,0,0);

               flag = zl303xx_PtsfFlagGet(entry->streamHandle, ZL303XX_PTSF_LOSS_DELAY_RESP);
               mask = zl303xx_PtsfMaskGet(entry->streamHandle, ZL303XX_PTSF_LOSS_DELAY_RESP);
               ZL303XX_TRACE_ALWAYS("      PTSF-lossDelayResp: %d & mask: %d = %d",
                     flag, mask, flag & mask, 0,0,0);

               flag = zl303xx_PtsfFlagGet(entry->streamHandle, ZL303XX_PTSF_LOSS_PEER_DELAY_RESP);
               mask = zl303xx_PtsfMaskGet(entry->streamHandle, ZL303XX_PTSF_LOSS_PEER_DELAY_RESP);
               ZL303XX_TRACE_ALWAYS("      PTSF-lossPdelayResp: %d & mask: %d = %d",
                     flag, mask, flag & mask, 0,0,0);

               flag = zl303xx_PtsfFlagGet(entry->streamHandle, ZL303XX_PTSF_LOSS_PEER_DELAY_FOLLOW_UP);
               mask = zl303xx_PtsfMaskGet(entry->streamHandle, ZL303XX_PTSF_LOSS_PEER_DELAY_FOLLOW_UP);
               ZL303XX_TRACE_ALWAYS("      PTSF-lossPdelayFollowUp: %d & mask: %d = %d",
                     flag, mask, flag & mask, 0,0,0);

               flag = zl303xx_PtsfFlagGet(entry->streamHandle, ZL303XX_PTSF_UNUSABLE);
               mask = zl303xx_PtsfMaskGet(entry->streamHandle, ZL303XX_PTSF_UNUSABLE);
               ZL303XX_TRACE_ALWAYS("      PTSF-unusable:      %d & mask: %d = %d",
                     flag, mask, flag & mask, 0,0,0);
            }

            /* ANNOUNCE Messages */
            pMsgRetry = &pStream->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].retry;
            zl303xx_PtpGetAnnounceReceiptTimeout(pStream, &timeStamp);
            ZL303XX_TRACE_ALWAYS("",0,0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS("      [ANNOUNCE Messages]%s",
                  ((pMsgRetry->denial.failed || pMsgRetry->timeout.failed) ? " - FAILED to negotiate" : ""), 0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS("      Requested: %s", ZL303XX_BOOL_TO_STR(entry->entryConfig.reqAnnc), 0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS("      Timeout:   %u sec",
                  timeStamp.secondsField.lo, 0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS("      Best effort: %s (intervalMax %d, retryCount %d)",
                  pStream->config.unicastNegotiation.bestEffort[ZL303XX_MSG_ID_ANNOUNCE].enabled ? "ENABLED" : "DISABLED", 
                  pStream->config.unicastNegotiation.bestEffort[ZL303XX_MSG_ID_ANNOUNCE].intervalMax,
                  pStream->unicast.bestEffort[ZL303XX_MSG_ID_ANNOUNCE].retryCount,0,0,0);

            if (telecomProfileEn == ZL303XX_TRUE)
               ZL303XX_TRACE_ALWAYS("      Remaining Requests: denied=%u timeout=%u (total grants denied=%lu, total timeouts=%lu)",
                     (pStream->unicast.lockout.remainingSec > 0 && pMsgRetry->denial.remainingRequests == 1 ? 0 : pMsgRetry->denial.remainingRequests),
                     (pStream->unicast.lockout.remainingSec > 0 && pMsgRetry->timeout.remainingRequests == 1 ? 0 : pMsgRetry->timeout.remainingRequests),
                     pMsgRetry->denial.count, pMsgRetry->timeout.count, 0,0);

            /* SYNC Messages */
            pMsgRetry = &pStream->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].retry;
            ZL303XX_TRACE_ALWAYS("",0,0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS("      [SYNC Messages]%s",
                  ((pMsgRetry->denial.failed || pMsgRetry->timeout.failed) ? " - FAILED to negotiate" : ""), 0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS("      Requested: %s", ZL303XX_BOOL_TO_STR(entry->entryConfig.reqSync), 0,0,0,0,0);
            if (telecomProfileEn == ZL303XX_TRUE)
            {
               ZL303XX_TRACE_ALWAYS("      Timeout:   %u sec", entry->telecom.syncReceiptTimeoutSec, 0,0,0,0,0);
               ZL303XX_TRACE_ALWAYS("      Remaining Requests: denied=%u timeout=%u (total grants denied=%lu, total timeouts=%lu)",
                     (pStream->unicast.lockout.remainingSec > 0 && pMsgRetry->denial.remainingRequests == 1 ? 0 : pMsgRetry->denial.remainingRequests),
                     (pStream->unicast.lockout.remainingSec > 0 && pMsgRetry->timeout.remainingRequests == 1 ? 0 : pMsgRetry->timeout.remainingRequests),
                     pMsgRetry->denial.count, pMsgRetry->timeout.count, 0,0);
            }
            ZL303XX_TRACE_ALWAYS("      Best effort: %s (intervalMax %d, retryCount %d)",
                  pStream->config.unicastNegotiation.bestEffort[ZL303XX_MSG_ID_SYNC].enabled ? "ENABLED" : "DISABLED", 
                  pStream->config.unicastNegotiation.bestEffort[ZL303XX_MSG_ID_SYNC].intervalMax,
                  pStream->unicast.bestEffort[ZL303XX_MSG_ID_SYNC].retryCount,0,0,0);
                  
            /* DELAY_RESP Messages */
            pMsgRetry = &pStream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].retry;
            ZL303XX_TRACE_ALWAYS("",0,0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS("      [DELAY_RESP Messages]%s",
                  ((pMsgRetry->denial.failed || pMsgRetry->timeout.failed) ? " - FAILED to negotiate" : ""), 0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS("      Requested: %s", ZL303XX_BOOL_TO_STR(entry->entryConfig.reqDelayResp), 0,0,0,0,0);
            if (telecomProfileEn == ZL303XX_TRUE)
            {
               ZL303XX_TRACE_ALWAYS("      Timeout:   %u sec", entry->telecom.delayReceiptTimeoutSec, 0,0,0,0,0);
               ZL303XX_TRACE_ALWAYS("      Remaining Requests: denied=%u timeout=%u (total grants denied=%lu, total timeouts=%lu)",
                     (pStream->unicast.lockout.remainingSec > 0 && pMsgRetry->denial.remainingRequests == 1 ? 0 : pMsgRetry->denial.remainingRequests),
                     (pStream->unicast.lockout.remainingSec > 0 && pMsgRetry->timeout.remainingRequests == 1 ? 0 : pMsgRetry->timeout.remainingRequests),
                     pMsgRetry->denial.count, pMsgRetry->timeout.count, 0,0);
            }
            ZL303XX_TRACE_ALWAYS("      Best effort: %s (intervalMax %d, retryCount %d)",
                  pStream->config.unicastNegotiation.bestEffort[ZL303XX_MSG_ID_DELAY_RESP].enabled ? "ENABLED" : "DISABLED", 
                  pStream->config.unicastNegotiation.bestEffort[ZL303XX_MSG_ID_DELAY_RESP].intervalMax,
                  pStream->unicast.bestEffort[ZL303XX_MSG_ID_DELAY_RESP].retryCount,0,0,0);
                  
            /* PEER_DELAY_RESP Messages */
            pMsgRetry = &pStream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].retry;
            ZL303XX_TRACE_ALWAYS("",0,0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS("      [PEER_DELAY_RESP Messages]%s",
                  ((pMsgRetry->denial.failed || pMsgRetry->timeout.failed) ? " - FAILED to negotiate" : ""), 0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS("      Requested: %s", ZL303XX_BOOL_TO_STR(entry->entryConfig.reqPdelayResp), 0,0,0,0,0);
            if (telecomProfileEn == ZL303XX_TRUE)
            {
               ZL303XX_TRACE_ALWAYS("      Timeout:   %u sec", entry->telecom.pdelayReceiptTimeoutSec, 0,0,0,0,0);
               ZL303XX_TRACE_ALWAYS("      Remaining Requests: denied=%u timeout=%u (total grants denied=%lu, total timeouts=%lu)",
                     (pStream->unicast.lockout.remainingSec > 0 && pMsgRetry->denial.remainingRequests == 1 ? 0 : pMsgRetry->denial.remainingRequests),
                     (pStream->unicast.lockout.remainingSec > 0 && pMsgRetry->timeout.remainingRequests == 1 ? 0 : pMsgRetry->timeout.remainingRequests),
                     pMsgRetry->denial.count, pMsgRetry->timeout.count, 0,0);
            }
            ZL303XX_TRACE_ALWAYS("      Best effort: %s (intervalMax %d, retryCount %d)",
                  pStream->config.unicastNegotiation.bestEffort[ZL303XX_MSG_ID_PEER_DELAY_RESP].enabled ? "ENABLED" : "DISABLED", 
                  pStream->config.unicastNegotiation.bestEffort[ZL303XX_MSG_ID_PEER_DELAY_RESP].intervalMax,
                  pStream->unicast.bestEffort[ZL303XX_MSG_ID_PEER_DELAY_RESP].retryCount,0,0,0);
                  
            ZL303XX_TRACE_ALWAYS("_________________________________________________________", 0,0,0,0,0,0);
            OS_TASK_DELAY(0);
         }
      }
      ZL303XX_TRACE_ALWAYS("", 0,0,0,0,0,0);
   }

   return status;
}

/* zl303xx_PtpUmtEntryStructInit */
/**

   Initializes a zl303xx_PtpUmtEntryS structure with default values.

  Parameters:
   [out]  pEntry  The initialized structure.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryStructInit(zl303xx_PtpUmtEntryS *pEntry)
{
   zlStatusE status = ZL303XX_OK;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pEntry);
   }

   /* Initialization the stream creation parameters. */
   if (status == ZL303XX_OK)
   {
      OS_MEMSET(pEntry, 0, sizeof(*pEntry));
      status = zl303xx_PtpStreamCreateStructInit(&pEntry->streamConfig);
   }

   if (status == ZL303XX_OK)
   {
      /* General UMT entry parameter defaults. */
      pEntry->entryConfig.reqAnnc           = ZL303XX_PTP_DEFAULT_UMT_ENTRY_REQ_ANNC;
      /* reqSync, reqDelayResp & reqPdelayResp must be initialized to FALSE to keep the
       * behavior of reqTiming backward compatible with old example code. */
      pEntry->entryConfig.reqSync            = ZL303XX_FALSE;
      pEntry->entryConfig.reqDelayResp       = ZL303XX_FALSE;
      pEntry->entryConfig.reqPdelayResp      = ZL303XX_FALSE;
      pEntry->entryConfig.reqTiming          = ZL303XX_PTP_DEFAULT_UMT_ENTRY_REQ_TIMING;  /* Deprecated */
      pEntry->entryConfig.maxAnncIntvl       = ZL303XX_PTP_DEFAULT_UMT_ENTRY_MAX_ANNC_INTVL;
      pEntry->entryConfig.maxSyncIntvl       = ZL303XX_PTP_DEFAULT_UMT_ENTRY_MAX_SYNC_INTVL;
      pEntry->entryConfig.maxDelayRespIntvl  = ZL303XX_PTP_DEFAULT_UMT_ENTRY_MAX_DRESP_INTVL;
      pEntry->entryConfig.maxPdelayRespIntvl = ZL303XX_PTP_DEFAULT_UMT_ENTRY_MAX_DRESP_INTVL; /* Same as any Delay mechanism */
      pEntry->entryConfig.bmcaAllow          = ZL303XX_PTP_DEFAULT_UMT_ENTRY_BMCA_ALLOW;

      /* Override some of the stream creation defaults. */
      zl303xx_PtpUmtEntryForceStreamConfig(&pEntry->streamConfig);

      /* Telecom Profile-specific members. */
      zl303xx_PtpTelecomUmtEntryStructInit(&pEntry->telecomConfig);
   }

   return status;
}

/* zl303xx_PtpUmtEntryAdd */
/**
   Adds an entry to a PTP port's Unicast Master Table.

  Parameters:
   [in]   pEntry         Data required to negotiate a timing connection with
                              a master.
   [out]  pStreamHandle  (Optional) Can be NULL. The handle to the newly
                              created stream.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryAdd(zl303xx_PtpUmtEntryS *pEntry,
                                 zl303xx_PtpStreamHandleT *pStreamHandle)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
   const char *fnName = "zl303xx_PtpUmtEntryAdd";
   zl303xx_PortDataS *pPort = NULL;
   zl303xx_PtpUmtEntryIntS *pEntryInt;

   /* Check input parameters before passing them on. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pEntry);
   }

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(pEntry->streamConfig.portHandle, &pPort);
   }

   if (status == ZL303XX_OK)
   {
      /* Override some of the stream config members. */
      zl303xx_PtpUmtEntryForceStreamConfig(&pEntry->streamConfig);

      /* Check the UMT entry add structure. */
      status = zl303xx_PtpUmtEntryStructCheck(pEntry, pPort);
   }

   /* Check that unicast negotiation is enabled on this port. */
   if (status == ZL303XX_OK)
   {
      if (pPort->config.unicastNegotiation.enabled == ZL303XX_FALSE)
      {
         ZL303XX_TRACE_ALWAYS("%s: Unicast negotation disabled on port %lu.",
            fnName, pPort->clockPortHandle, 0,0,0,0);
         status = ZL303XX_INVALID_OPERATION;
      }
   }

   /* Take the PTP clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pPort->clock);
      status = mStatus;
   }

   /* Add the data to the port's Unicast Master Table. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpUmtEntryIntAdd(pPort, pEntry, ZL303XX_TRUE, &pEntryInt);
   }

   /* Release the PTP data mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }

   /* Return the stream handle, if requested. */
   if (status == ZL303XX_OK)
   {
      if (pStreamHandle)
      {
         *pStreamHandle = pEntryInt->streamHandle;
      }
   }

   return status;
}

/* zl303xx_PtpUmtEntryRemove */
/**
   Removes an entry from a PTP port's Unicast Master Table. A
   ZL303XX_PTP_EVENT_STREAM_DELETE will be generated if there was a stream
   associated with this UMT entry.

  Parameters:
   [in]   portHandle   Handle to a previously created PTP port.
   [in]   pPortAddr    The port address of the master to remove from the table.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryRemove(zl303xx_PtpPortHandleT portHandle,
                                    zl303xx_PortAddress *pPortAddr)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
   zl303xx_PortDataS *pPort = NULL;
   zl303xx_BooleanE found = ZL303XX_FALSE;
   zl303xx_PtpUmtEntryIntS *pEntryInt;

   /* Check input parameters before passing them on. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pPortAddr);
   }

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   /* Find the UMT entry corresponding to portAddress. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpUmtEntryFind(&pPort->unicastMasterTable, pPortAddr, &found, &pEntryInt);
   }

   if (status == ZL303XX_OK)
   {
      /* Exit function early if the UMT entry was not found. Do not return an
       * error code. */
      if (found == ZL303XX_FALSE)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "No Unicast Master entry matching portAddress found.", 0,0,0,0,0,0);
         return status;
      }

      /* Take the clock mutex. */
      mStatus = zl303xx_PtpClockMutexLock(pPort->clock);
      status = mStatus;
   }

   /* This will delete the stream associated with this entry. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpUmtEntryIntRemove(pPort, pEntryInt);
   }

   /* Release the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }

   return status;
}

/* zl303xx_PtpUmtEntryRemoveAll */
/**
   Removes all entries in a PTP port's Unicast Master Table. A
   ZL303XX_PTP_EVENT_STREAM_DELETE will be generated for each UMT entry that
   has a stream associated with it.

  Parameters:
   [in]   portHandle    Handle to a previously created PTP port.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryRemoveAll(zl303xx_PtpPortHandleT portHandle)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
   zl303xx_PortDataS *pPort = NULL;

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   /* Check if the UMT is valid. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpUmtCheck(&pPort->unicastMasterTable);
   }

   /* Take the clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pPort->clock);
      status = mStatus;
   }

   /* Delete all entries in the UMT. */
   if (status == ZL303XX_OK)
   {
      Uint16T count;

      for (count = 0;
           (count < pPort->unicastMasterTable.maxTableSize) && (status == ZL303XX_OK);
           count++)
      {
         status = zl303xx_PtpUmtEntryIntRemove(pPort,
                     &pPort->unicastMasterTable.master[count]);
      }
   }

   /* Release the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }

   return status;
}

/* zl303xx_PtpUmtEntryExists */
/**
   Checks if a master entry exists in any PTP port's Unicast Master Table.

  Parameters:
   [in,out] pEntry         The master entry to search for. Only the portAddress
                                member needs to be filled in. If the entry exists
                                all other structure members will be data filled.
   [out]    pPortHandle    (Optional) Can be NULL. Returns the handle of the
                                first port that this UMT entry was found on.
                                Otherwise, returns ZL303XX_PTP_INVALID.
   [out]    pStreamHandle  (Optional) Can be NULL. Returns the handle to the
                                stream associated with this UMT entry, if one
                                has been created. Otherwise, returns ZL303XX_PTP_INVALID.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryExists(zl303xx_PtpUmtEntryS *pEntry,
                                    zl303xx_PtpPortHandleT *pPortHandle,
                                    zl303xx_PtpStreamHandleT *pStreamHandle)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T count;
   zl303xx_BooleanE exists = ZL303XX_FALSE;
   zl303xx_PtpUmtEntryIntS *pEntryInt;
   zl303xx_PtpPortHandleT portHandle = (zl303xx_PtpPortHandleT)ZL303XX_PTP_INVALID;
   zl303xx_PtpStreamHandleT streamHandle = (zl303xx_PtpStreamHandleT)ZL303XX_PTP_INVALID;
   zl303xx_ClockStreamS *pStream;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pEntry);
   }

   /* Search all ports for a matching master address. */
   if (status == ZL303XX_OK)
   {
      for (count = 0; count < ZL303XX_PTP_NUM_PORTS_MAX; count++)
      {
         if ((Zl303xx_PtpPortArray[count] != NULL) &&
             (zl303xx_PtpUmtEntryFind(&Zl303xx_PtpPortArray[count]->unicastMasterTable,
                                      &pEntry->streamConfig.destAddr,
                                      &exists, &pEntryInt)       == ZL303XX_OK) &&
             (exists))
         {
            /* If the entry was found, fill in the UMT entry structure. */
            pEntry->entryConfig   = pEntryInt->entryConfig;
            pEntry->telecomConfig = pEntryInt->telecomConfig;

            portHandle   = Zl303xx_PtpPortArray[count]->clockPortHandle;
            streamHandle = pEntryInt->streamHandle;

            if (zl303xx_PtpStreamDataGet(streamHandle, &pStream) == ZL303XX_OK)
            {
               pEntry->streamConfig = pStream->config;
            }

            break;
         }
      }

      if (pPortHandle)
         *pPortHandle = portHandle;

      if (pStreamHandle)
         *pStreamHandle = streamHandle;
   }

   return status;
}

/* zl303xx_PtpUmtEntryReqAnncSet */
/**
   Allows a slave to enable/disable unicast negotiation requests for Announce
   messages.

  (Deprecated)
   zl303xx_PtpContractRequest() and zl303xx_PtpContractCancel() should be used
   instead to manage individual contracts.

  Parameters:
   [in]  streamHandle  Handle to a previously created PTP stream.
   [in]  reqAnnc       Value to set.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryReqAnncSet(zl303xx_PtpStreamHandleT streamHandle,
                                        zl303xx_BooleanE reqAnnc)
{
   zlStatusE status;

   if (reqAnnc)
   {
      status = zl303xx_PtpContractRequest(streamHandle, ZL303XX_MSG_ID_ANNOUNCE, NULL);
   }
   else
   {
      status = zl303xx_PtpContractCancel(streamHandle, ZL303XX_MSG_ID_ANNOUNCE);
   }

   return status;
}

/* zl303xx_PtpUmtEntryReqTimingSet */
/**
   Allows a slave to enable/disable unicast negotiation requests for timing
   (Sync and Delay_Resp) messages.

  (Deprecated)
   zl303xx_PtpContractRequest() and zl303xx_PtpContractCancel() should be used
   instead to manage individual contracts.

  Parameters:
   [in]  streamHandle  Handle to a previously created PTP stream.
   [in]  reqTiming     Value to set.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryReqTimingSet(zl303xx_PtpStreamHandleT streamHandle,
                                          zl303xx_BooleanE reqTiming)
{
   zlStatusE status = ZL303XX_OK;

   if (reqTiming)
   {
      if (status == ZL303XX_OK)
      {
         status = zl303xx_PtpContractRequest(streamHandle, ZL303XX_MSG_ID_SYNC, NULL);
      }
      if (status == ZL303XX_OK)
      {
         status = zl303xx_PtpContractRequest(streamHandle, ZL303XX_MSG_ID_DELAY_RESP, NULL);
      }
   }
   else
   {
      if (status == ZL303XX_OK)
      {
         status = zl303xx_PtpContractCancel(streamHandle, ZL303XX_MSG_ID_SYNC);
      }
      if (status == ZL303XX_OK)
      {
         status = zl303xx_PtpContractCancel(streamHandle, ZL303XX_MSG_ID_DELAY_RESP);
      }
   }

   return status;
}

/* zl303xx_PtpUmtEntryMaxIntvlSet */
/**
   Sets the maximum acceptable log2 interval for a specified messageType.

  Parameters:
   [in]   streamHandle  Handle to an existing stream.
   [in]   msgType       The messageType to set the max interval of.
   [in]   intvl         The maximum acceptable log2 interval.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryMaxIntvlSet(zl303xx_PtpStreamHandleT streamHandle,
                                         zl303xx_MessageTypeE msgType,
                                         Sint8T intvl)
{
   zlStatusE status = ZL303XX_OK;
   umtEntryMaxIntvlS max;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_UNI_NEG_MSG_TYPE_VALID(msgType);
   }

   if (status == ZL303XX_OK)
   {
      max.msgType = msgType;
      max.intvl = intvl;

      status =  zl303xx_PtpUnicastMasterParamOper(streamHandle,
                   UMT_PARAM_MAX_INTVL, ZL303XX_TRUE, &max, NULL);
   }

   return status;
}

/* zl303xx_PtpUmtEntrySyncTimeoutSet */
/**
   Sets the syncReceiptTimeoutSec value for a Unicast Master which is used to
   generate the PTSF-lossSync flag.

  Parameters:
   [in]  streamHandle    Handle to a previously created PTP stream.
   [in]  syncTimeoutSec  Value to set. Set to 0 to disable.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntrySyncTimeoutSet(zl303xx_PtpStreamHandleT streamHandle,
                                            Uint8T syncTimeoutSec)
{
   return zl303xx_PtpUnicastMasterParamOper(
                streamHandle, UMT_PARAM_TELECOM_SYNC_TIMEOUT,
                ZL303XX_TRUE, &syncTimeoutSec, NULL);
}

/* zl303xx_PtpUmtEntryFollowUpTimeoutSet */
/**
   Sets the followUpReceiptTimeoutSec value for a Unicast Master which is used
   to generate the PTSF-lossFollowUp flag.

  Parameters:
   [in]  streamHandle  Handle to a previously created PTP stream.
   [in]  timeoutSec    Value to set. Set to 0 to disable.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryFollowUpTimeoutSet(zl303xx_PtpStreamHandleT streamHandle,
                                                Uint8T followUpTimeoutSec)
{
   return zl303xx_PtpUnicastMasterParamOper(
                streamHandle, UMT_PARAM_TELECOM_FOLLOW_TIMEOUT,
                ZL303XX_TRUE, &followUpTimeoutSec, NULL);
}

/* zl303xx_PtpUmtEntryFollowUpTimeoutSet */
/**
   Sets the syncOrFollowUpReceiptTimeoutSec value for a Unicast Master which is
   used to generate the PTSF-lossSyncOrFollowUp flag.

  Parameters:
   [in]  streamHandle  Handle to a previously created PTP stream.
   [in]  timeoutSec    Value to set. Set to 0 to disable.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntrySyncOrFollowUpTimeoutSet(zl303xx_PtpStreamHandleT streamHandle,
                                                      Uint8T timeoutSec)
{
   return zl303xx_PtpUnicastMasterParamOper(
                streamHandle, UMT_PARAM_TELECOM_SYNC_FOLLOW_TIMEOUT,
                ZL303XX_TRUE, &timeoutSec, NULL);
}

/* zl303xx_PtpUmtEntryDelayTimeoutSet */
/**
   Sets the delayRespReceiptTimeoutSec value for a Unicast Master which is used
   to generate the PTSF-lossDelayResp flag.

  Parameters:
   [in]  streamHandle     Handle to a previously created PTP stream.
   [in]  delayTimeoutSec  Value to set. Set to 0 to disable.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryDelayTimeoutSet(zl303xx_PtpStreamHandleT streamHandle,
                                             Uint8T delayTimeoutSec)
{
   return zl303xx_PtpUnicastMasterParamOper(
                streamHandle, UMT_PARAM_TELECOM_DELAY_TIMEOUT,
                ZL303XX_TRUE, &delayTimeoutSec, NULL);
}

/* zl303xx_PtpUmtEntryPdelayTimeoutSet */
/**
   Sets the pdelayRespReceiptTimeoutSec value for a Unicast Master which is used
   to generate the PTSF-lossPdelayResp flag.

  Parameters:
   [in]  streamHandle      Handle to a previously created PTP stream.
   [in]  pdelayTimeoutSec  Value to set. Set to 0 to disable.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryPdelayTimeoutSet(zl303xx_PtpStreamHandleT streamHandle,
                                              Uint8T pdelayTimeoutSec)
{
   return zl303xx_PtpUnicastMasterParamOper(
                streamHandle, UMT_PARAM_TELECOM_PDELAY_TIMEOUT,
                ZL303XX_TRUE, &pdelayTimeoutSec, NULL);
}

/* zl303xx_PtpUmtEntryPdelayFollowTimeoutSet */
/**
   Sets the pdelayFollowUpReceiptTimeoutSec value for a Unicast Master which is used
   to generate the PTSF-lossPdelayFollow flag.

  Parameters:
   [in]  streamHandle      Handle to a previously created PTP stream.
   [in]  pdelayFollowTimeoutSec  Value to set. Set to 0 to disable.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryPdelayFollowTimeoutSet(zl303xx_PtpStreamHandleT streamHandle,
                                              Uint8T pdelayFollowTimeoutSec)
{
   return zl303xx_PtpUnicastMasterParamOper(
                streamHandle, UMT_PARAM_TELECOM_PDELAY_FOLLOW_TIMEOUT,
                ZL303XX_TRUE, &pdelayFollowTimeoutSec, NULL);
}

/* zl303xx_PtpUmtEntryPtsfMaskSet */
/**
   Performs no action.

  (Deprecated)  This function has been replaced by zl303xx_PtsfMaskSet().

  Parameters:
   [in]  streamHandle   Unused.
   [in]  ptsfMask       Unused.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryPtsfMaskSet(zl303xx_PtpStreamHandleT streamHandle,
                                         Sint32T ptsfMask)
{
   (void)streamHandle; (void)ptsfMask;
   return ZL303XX_OK;
}

/* zl303xx_PtpUmtEntryPtsfUserSet */
/**
   Sets the PTSF-user flag which will be evaluated based on the current
   PTSF mask.

  (Deprecated)  This function has been replaced by zl303xx_PtsfFlagSet().

  Parameters:
   [in]   streamHandle  Handle to a previously created PTP stream.
   [in]   setFlag       ZL303XX_TRUE will raise the PTSF-user flag. ZL303XX_FALSE
                             will clear it.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryPtsfUserSet(zl303xx_PtpStreamHandleT streamHandle,
                                         zl303xx_BooleanE setFlag)
{
   zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_USER, setFlag);
   return ZL303XX_OK;
}

/* zl303xx_PtpUmtEntryPtsfNotLockedSet */
/**
   Sets the PTSF-notFreqLock and PTSF-notPhaseLock flags which will be evaluated
   based on the current PTSF mask.

  (Deprecated)  This function has been replaced by zl303xx_PtsfFlagSet().

  Parameters:
   [in]   streamHandle  Handle to a previously created PTP stream.
   [in]   setFlag       ZL303XX_TRUE will raise PTSF flags. ZL303XX_FALSE will
                             clear them.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryPtsfNotLockedSet(zl303xx_PtpStreamHandleT streamHandle,
                                              zl303xx_BooleanE setFlag)
{
   zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_NOT_FREQ_LOCK, setFlag);
   zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_NOT_PHASE_LOCK, setFlag);
   return ZL303XX_OK;
}

/* zl303xx_PtpUmtEntryBmcaAllowSet */
/**
   Sets the flag that allows this Unicast Master entry to be used in the BMCA
   selection process.

  Parameters:
   [in]  streamHandle   Handle to a previously created PTP stream.
   [in]  bmcaAllow      Set to ZL303XX_FALSE to not allow this master to be
                             selected by BMCA. Set to ZL303XX_TRUE to allow it.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryBmcaAllowSet(zl303xx_PtpStreamHandleT streamHandle,
                                          zl303xx_BooleanE bmcaAllow)
{
   zlStatusE status = ZL303XX_OK;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(bmcaAllow);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpUnicastMasterParamOper(
                  streamHandle, UMT_PARAM_BMCA_ALLOW,
                  ZL303XX_TRUE, &bmcaAllow, NULL);
   }

   return status;
}

/* zl303xx_PtpUmtEntryPrioritySet */
/**
   Sets the local priority for a Unicast Master entry to be used in the BMCA
   selection process.

  Parameters:
   [in]  streamHandle   Handle to a previously created PTP stream.
   [in]  priority       Local priority value to use in BMCA selection.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryPrioritySet(zl303xx_PtpStreamHandleT streamHandle,
                                         Uint8T priority)
{
   return zl303xx_PtpUnicastMasterParamOper(
             streamHandle, UMT_PARAM_TELECOM_PRIORITY,
             ZL303XX_TRUE, &priority, NULL);
}

/* zl303xx_PtpUmtEntryRetryReset */
/**
   Resets the number of retry attempts for a Unicast Master. If the master
   is already locked out, that timer is reset and the server is retried the
   next time the state machine runs.

  Parameters:
   [in]  streamHandle   Handle to a previously created PTP stream.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryRetryReset(zl303xx_PtpStreamHandleT streamHandle)
{
   Sint32T empty = 0;

   return zl303xx_PtpUnicastMasterParamOper(streamHandle, UMT_PARAM_TELECOM_RETRY,
                                            ZL303XX_TRUE, &empty, NULL);
}

/* zl303xx_PtpUmtEntryWtrSet */
/**
   Resets the wait to restore timer for a Unicast Master. This master will be
   available immediately for selection by the Telecom Profile BMCA.

  Parameters:
   [in]  streamHandle   Handle to a previously created PTP stream.
   [in]  sec            Time, in seconds, to set the WTR timer to.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryWtrSet(zl303xx_PtpStreamHandleT streamHandle,
                                    Uint32T sec)
{
   return zl303xx_PtpUnicastMasterParamOper(
             streamHandle, UMT_PARAM_TELECOM_WTR_SET,
             ZL303XX_TRUE, &sec, NULL);
}

/* zl303xx_PtpUmtEntryStatusGet */
/**
   Fills a zl303xx_PtpUmtEntryStatusS structure with data from a PTP stream.

  Parameters:
   [in]   streamHandle  Handle to a previously created PTP stream.
   [out]  pStatus       Filled status structure.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryStatusGet(zl303xx_PtpStreamHandleT streamHandle,
                                       zl303xx_PtpUmtEntryStatusS *pStatus)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pStatus);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpUnicastMasterParamOper(streamHandle, UMT_PARAM_STATUS,
                                                 ZL303XX_FALSE, pStatus, NULL);
   }

   return status;
}

/****************   INTERNAL PTP FUNCTION DEFINITIONS   **********************/

/* zl303xx_PtpUmtStructInit */
/** 

   Initializes a Unicast Master Table structure. Called during port creation.

  Parameters:
   [in,out]  pUmt   Unicast Master Table structure to initialize.

*******************************************************************************/
void zl303xx_PtpUmtStructInit(zl303xx_PtpUmtS *pUmt)
{
   /* Check the UMT pointer is good. */
   if(pUmt)
   {
      OS_MEMSET(pUmt, 0, sizeof(*pUmt));
      pUmt->master = NULL;
   }
}

/* zl303xx_PtpUmtCreate */
/** 

   Allocates memory for a Unicast Master Table structure. Called during port
   creation.

  Parameters:
   [in,out]  pUmt   Unicast Master Table structure to initialize.
   [in]      size   Number of master entries to allocate for this table.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtCreate(zl303xx_PtpUmtS *pUmt, Uint16T size)
{
   zlStatusE status = ZL303XX_OK;

   /* Assume UMT pointer is good. */

   /* Check if the table is already initialized. */
   if (status == ZL303XX_OK)
   {
      if (pUmt->master != NULL)
      {
         ZL303XX_TRACE_ERROR("Unicast master table already initialized.", 0,0,0,0,0,0);
         status = ZL303XX_MULTIPLE_INIT_ATTEMPT;
      }
   }

   /* Allocate memory for the master entries. */
   if (status == ZL303XX_OK)
   {
      if (size > 0)
      {
         pUmt->master = (zl303xx_PtpUmtEntryIntS *)OS_CALLOC(size, sizeof(*(pUmt->master)));

         if (pUmt->master == NULL)
         {
            ZL303XX_TRACE_ERROR("Memory allocate for UMT failed.", 0,0,0,0,0,0);
            status = ZL303XX_RTOS_MEMORY_FAIL;
         }
      }
   }

   /* Fill in the rest of the table parameters. */
   if (status == ZL303XX_OK)
   {
      pUmt->maxTableSize    = size;
      pUmt->actualTableSize = 0;
   }

   return status;
}

/* zl303xx_PtpUmtDelete */
/** 

   Frees memory used by a Unicast Master Table structure. Called during port
   deletion.

  Parameters:
   [in,out]  pUmt   Unicast Master Table structure to free memory from.

*******************************************************************************/
void zl303xx_PtpUmtDelete(zl303xx_PtpUmtS *pUmt)
{
   /* Free memory allocated for the master entries and zero-fill the structure. */
   zl303xx_PtpUmtFree(pUmt);
   zl303xx_PtpUmtStructInit(pUmt);
}

/* zl303xx_PtpUmtCheck */
/** 

   Checks a Unicast Master Table structure to ensure it is valid.

  Parameters:
   [in]   pUmt   Pointer to an existing Unicast Master Table.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtCheck(zl303xx_PtpUmtS *pUmt)
{
   zlStatusE status = ZL303XX_OK;

   if (pUmt->master == NULL)
   {
      ZL303XX_TRACE_ERROR("Unicast Master Table not initialized.", 0,0,0,0,0,0);
      status = ZL303XX_INIT_ERROR;
   }

   return status;
}

/* zl303xx_PtpUmtStartAllStreams */
/** 

   Attempt to start all streams in a port's Unicast Master Table.

  Parameters:
   [in]   pUmt   Pointer to a Unicast Master Table structure.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtStartAllStreams(zl303xx_PtpUmtS *pUmt)
{
   zlStatusE status = ZL303XX_OK;
   Uint16T count;
   zl303xx_PortDataS *pPort = NULL;
   zl303xx_PtpStreamCreateS streamCreate;

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(pUmt->portHandle, &pPort);
   }

   /* Initialize the stream config and force any UMT specific members. */
   if (status == ZL303XX_OK)
   {
      /* Add any Profile specific default values. */
      status = zl303xx_PtpProfileStreamStructInit(pPort->clock->config.profile, &streamCreate);

      zl303xx_PtpUmtEntryForceStreamConfig(&streamCreate);
   }

   /* Create streams for any entries that do not have them yet. */
   if (status == ZL303XX_OK)
   {
      for (count = 0; (count < pUmt->maxTableSize) && (status == ZL303XX_OK); count++)
      {
         if ((pUmt->master[count].inUse == ZL303XX_TRUE) &&
             (pUmt->master[count].streamHandle == (Uint32T)ZL303XX_PTP_INVALID))
         {
            status = zl303xx_PtpUmtEntryStartStream(pPort, &streamCreate, NULL, &pUmt->master[count]);
         }
      }
   }

   return status;
}

/* zl303xx_PtpUmtTlvPack */
/** 

   Packs a Unicast Master Table into a TLV data field as described in IEEE
   1588-2008 section 17.5.3.

  Parameters:
   [in]   pUmt        Pointer to a Unicast Master Table.
   [out]  buffer      Byte buffer to pack the TLV data field in.
   [out]  pBufferLen  Number of bytes packed in buffer.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtTlvPack(zl303xx_PtpUmtS *pUmt, Uint8T *buffer, Uint16T *pBufferLen)
{
   zlStatusE status = ZL303XX_OK;
   Uint16T len = 0, portLen, count, packedEntries = 0;

   /* Assume pointers are OK. */

   /* Pack the UMT into TLV format. */
   if (status == ZL303XX_OK)
   {
      /* logQueryInterval... */
      *(buffer + len) = (Uint8T)pUmt->logQueryInterval;
      len += sizeof(pUmt->logQueryInterval);

      /* actualTableSize... */
      *(Uint16T *)(void *)(buffer + len) = htons(pUmt->actualTableSize);
      len += sizeof(pUmt->actualTableSize);

      /* Pack all of the port addresses. */
      for (count = 0;
           (count < pUmt->maxTableSize) && (status == ZL303XX_OK);
           count++)
      {
         if (pUmt->master[count].inUse)
         {
            status = zl303xx_V2PackPortAddrTlvFromIeee(
                       &pUmt->master[count].portAddress, buffer + len, &portLen);

            packedEntries++;
            len += portLen;
         }
      }
   }

   /* Check that the number of packed entries is equal to the table size. */
   if (status == ZL303XX_OK)
   {
      if (packedEntries != pUmt->actualTableSize)
      {
         ZL303XX_TRACE_ALWAYS("Error packing UMT TLV. # entries packed=%u != actualTableSize=%u",
                              packedEntries, pUmt->actualTableSize, 0,0,0,0);
         status = ZL303XX_DATA_CORRUPTION;
      }
   }

   /* Return the number of bytes packed. */
   if (status == ZL303XX_OK)
   {
      *pBufferLen = len;
   }
   /* If this function failed somewhere, clear any data already written. */
   else
   {
      OS_MEMSET(buffer, 0, len);
      *pBufferLen = 0;
   }

   return status;
}

/* zl303xx_PtpUmtTlvUnpack */
/** 

   Unpacks a Unicast Master Table in TLV format as described in IEEE
   1588-2008 section 17.5.3 into an internal zl303xx_PtpUmtS structure.

  Parameters:
   [in]   buffer      Byte buffer containing the UNICAST_MASTER_TABLE TLV.
   [in]   bufferLen   Length of buffer, in bytes.
   [out]  pUmt        Pointer to an uninitialized Unicast Master Table.

  Notes:
   Memory for the UMT entries will be allocated by this function.
   zl303xx_PtpUmtFree() must be called sometime after returning from this
   function.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtTlvUnpack(Uint8T *buffer, Uint16T bufferLen, zl303xx_PtpUmtS *pUmt)
{
   zlStatusE status = ZL303XX_OK;
   Uint16T len = 0, count, portLen;
   Sint8T logQueryInterval;
   Uint16T tableSize;

   /* Assume pointers are OK. */

   if (status == ZL303XX_OK)
   {
      zl303xx_PtpUmtStructInit(pUmt);

      /* Unpack the logQueryInterval and tableSize fields. */
      logQueryInterval = (Sint8T)(*(buffer + len));
      len += sizeof(Uint8T);

      OS_MEMCPY(&tableSize, buffer + len, sizeof(tableSize));
      tableSize = ntohs(tableSize);
      len += sizeof(Uint16T);

      /* Allocate memory for the master entries. */
      status = zl303xx_PtpUmtCreate(pUmt, tableSize);
   }

   /* Unpack all port addresses. */
   if (status == ZL303XX_OK)
   {
      for (count = 0;
           (count < tableSize) && (status == ZL303XX_OK);
           count++)
      {
         if ((status = zl303xx_V2UnpackPortAddrTlvToIeee(
                          &pUmt->master[count].portAddress,
                          buffer + len, &portLen))               == ZL303XX_OK)
         {
            pUmt->master[count].inUse = ZL303XX_TRUE;
            len += portLen;

            if (len > bufferLen)
            {
               ZL303XX_TRACE_ALWAYS(
                  "UNICAST_MASTER_TABLE TLV data missing! Attempt to unpack %u of %u bytes.",
                  len, bufferLen, 0,0,0,0);
               status = ZL303XX_DATA_CORRUPTION;
            }
         }
      }
   }

   /* Store the other unpacked parameters. */
   if (status == ZL303XX_OK)
   {
      pUmt->logQueryInterval = logQueryInterval;
      pUmt->actualTableSize  = tableSize;
   }
   /* If there was a problem unpacking the TLV, free any allocated memory. */
   else
   {
      zl303xx_PtpUmtDelete(pUmt);
   }

   return status;
}

/* zl303xx_PtpUmtReplace */
/** 

   Replaces an existing UMT with a new one. If the same entry exists in both
   tables, it will remain at the same index.

  Parameters:
   [in,out]  pCurrUmt   Pointer to an existing Unicast Master Table.
   [in]      pNewUmt    The new UMT to use. It is generated from a TLV and
                             should have its memory freed by zl303xx_PtpUmtDelete()
                             after calling this function.

  Notes:
   Assumes the clock mutex has already been locked prior to calling.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtReplace(zl303xx_PtpUmtS *pCurrUmt, zl303xx_PtpUmtS *pNewUmt)
{
   zlStatusE status = ZL303XX_OK;
   Uint16T count;
   zl303xx_PortDataS *pPort;
   zl303xx_PtpUmtEntryIntS *currEntry, *newEntry;

   /* Assume table pointers are OK. */

   /* Check if the new table will fit into the old table. */
   if (status == ZL303XX_OK)
   {
      if (pNewUmt->actualTableSize > pCurrUmt->maxTableSize)
      {
         ZL303XX_TRACE_ALWAYS("New UMT size=%u greater than current max size=%u.",
               pNewUmt->actualTableSize, pCurrUmt->maxTableSize, 0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(pCurrUmt->portHandle, &pPort);
   }

   /* Find entries that exist in both tables. */
   if (status == ZL303XX_OK)
   {
      zl303xx_BooleanE exists;

      for (count = 0, currEntry = pCurrUmt->master;
           (count < pCurrUmt->maxTableSize) && (status == ZL303XX_OK);
           count++, currEntry++)
      {
         exists = ZL303XX_FALSE;

         if (currEntry->inUse && (pNewUmt->actualTableSize > 0))
         {
            status = zl303xx_PtpUmtEntryFind(pNewUmt, &currEntry->portAddress,
                                             &exists, &newEntry);
         }

         if (status == ZL303XX_OK)
         {
            if (exists)
            {
               /* Mark the new entry as not in use so it does not get added to
                * the current table later. */
               newEntry->inUse = ZL303XX_FALSE;
            }
            else
            {
               /* This entry does not exist in the new table, so delete it. */
               status = zl303xx_PtpUmtEntryIntRemove(pPort, currEntry);
            }
         }
      }
   }

   /* Add all new entries to the current table. */
   {
      zl303xx_PtpUmtEntryS addEntry;

      if (status == ZL303XX_OK)
      {
         for (count = 0, newEntry = pNewUmt->master;
              (count < pNewUmt->maxTableSize) && (status == ZL303XX_OK);
              count++, newEntry++)
         {
            /* inUse was marked FALSE above if the entry already exists in the current table. */
            if (newEntry->inUse)
            {
               if (status == ZL303XX_OK)
               {
                  status = zl303xx_PtpUmtEntryStructInit(&addEntry);
               }

               if (status == ZL303XX_OK)
               {
                  addEntry.streamConfig.destAddr = newEntry->portAddress;
                  status = zl303xx_PtpUmtEntryIntAdd(pPort, &addEntry, ZL303XX_TRUE, NULL);
               }
            }
         }
      }
   }

   /* Set the new logQueryInterval value. */
   if (status == ZL303XX_OK)
   {
      pCurrUmt->logQueryInterval = pNewUmt->logQueryInterval;
   }

   return status;
}

/* zl303xx_PtpUmtFree */
/** 

   Frees any memory allocated for master entries inside a zl303xx_PtpUmtS
   structure. Should only be called for a zl303xx_PtpUmtS known to be initialized
   by another function.

   Must be called when finished using the table returned by
   zl303xx_PtpUmtTlvUnpack().

  Parameters:
   [in]   pUmt   Pointer to a UMT structure to free memory from.

*******************************************************************************/
void zl303xx_PtpUmtFree(zl303xx_PtpUmtS *pUmt)
{
   /* Check the pointers are good. */
   if((pUmt) && (pUmt->master))
   {
      OS_FREE(pUmt->master);
      pUmt->master = NULL;
   }
}

/* zl303xx_PtpUmtEntryIntRemove */
/** 

   Destroys the stream associated with an entry in a Unicast Master Table.

  Parameters:
   [in]   pPort      Pointer to an existing port.
   [in]   pEntryInt  Pointer to the UMT internal entry.

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
zlStatusE zl303xx_PtpUmtEntryIntRemove(zl303xx_PortDataS *pPort,
                                       zl303xx_PtpUmtEntryIntS *pEntryInt)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pStream;
   zl303xx_PtpEventStreamDeleteS eventData;

   /* Return early if the entry is empty. */
   if (status == ZL303XX_OK)
   {
      if ((pEntryInt == NULL) || (pEntryInt->inUse == ZL303XX_FALSE))
      {
         return status;
      }

      /* G.8265.1 & G.8275.2:
       * Remove this address from the acceptable master table. */
      if ((pPort->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1) ||
          (pPort->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_2))
      {
         zl303xx_PtpTableRemove(&pPort->accMasterTbl, &pEntryInt->portAddress);
      }

      if (pEntryInt->streamHandle != (Uint32T)ZL303XX_PTP_INVALID)
      {
         /* Get a pointer to the stream. */
         status = zl303xx_PtpStreamDataGet(pEntryInt->streamHandle, &pStream);

         /* Delete the stream, if one exists. */
         if (status == ZL303XX_OK)
         {
            /* zl303xx_PtpStreamDeleteInternal() trashes the stream pointer, so get
             * a pointer to the clock before that. */
            zl303xx_PtpClockS *pClock = pStream->portData->clock;

            /* Gather data for the STREAM_DELETE event. The event handler will be
             * invoked because this is an automatic stream delete. */
            if (pClock->config.eventCallback != NULL)
            {
               eventData.streamHandle = pStream->clockStreamHandle;
               eventData.clockHandle = pClock->clockHandle;
               eventData.streamExtData = pStream->config.extData;
               eventData.streamConfig = pStream->config;
            }

            status = zl303xx_PtpStreamDeleteInternal(pStream, NULL);

            /* Call the event handler, if it exists. */
            if (status == ZL303XX_OK)
            {
               if (pClock->config.eventCallback != NULL)
               {
                  pClock->config.eventCallback(
                     ZL303XX_PTP_EVENT_STREAM_DELETE, &eventData);
               }
            }
         }
      }
   }

   /* Mark the table entry as empty. */
   if (status == ZL303XX_OK)
   {
      pEntryInt->inUse = ZL303XX_FALSE;
      pPort->unicastMasterTable.actualTableSize--;
   }

   return status;
}

/* zl303xx_PtpUmtEntryServicesCheck */
/** 

   Checks the sanity of the internal request flags for Announce, Sync, and
   Delay_Resp contracts. Logs any warnings or modifies the flags as needed.

  Parameters:
   [in]   pPort   Pointer to the port pEntry is stored on.
   [in]   pEntry  Pointer to an existing UMT entry.

*******************************************************************************/
void zl303xx_PtpUmtEntryServicesCheck(zl303xx_PortDataS *pPort,
                                      zl303xx_PtpUmtEntryIntS *pEntry)
{
   if ((pEntry->entryConfig.reqAnnc == ZL303XX_FALSE) &&
       ((pEntry->entryConfig.reqSync == ZL303XX_TRUE) ||
        (pEntry->entryConfig.reqDelayResp == ZL303XX_TRUE) ||
        (pEntry->entryConfig.reqPdelayResp == ZL303XX_TRUE)))
   {
      ZL303XX_TRACE_ALWAYS("Stream %lu: Timing service will not start until Announce service requested.",
         pEntry->streamHandle, 0,0,0,0,0);
   }

   if ((pPort->config.delayMechanism != ZL303XX_DELAY_MECHANISM_E2E) &&
       (pEntry->entryConfig.reqDelayResp == ZL303XX_TRUE))
   {
      ZL303XX_TRACE_ALWAYS("Stream %lu: Delay_Resp contract cannot be requested (port not E2E).",
                           pEntry->streamHandle, 0,0,0,0,0);
      pEntry->entryConfig.reqDelayResp = ZL303XX_FALSE;
   }

   if ((pPort->config.delayMechanism != ZL303XX_DELAY_MECHANISM_P2P) &&
       (pEntry->entryConfig.reqPdelayResp == ZL303XX_TRUE))
   {
      ZL303XX_TRACE_ALWAYS("Stream %lu: Peer_Delay_Resp contract cannot be requested (port not P2P).",
                           pEntry->streamHandle, 0,0,0,0,0);
      pEntry->entryConfig.reqPdelayResp = ZL303XX_FALSE;
   }
}

/****************   STATIC FUNCTION DEFINITIONS   ****************************/

/* zl303xx_PtpUmtEntryStructCheck */
/** 

   Checks the validity of the members of a unicast master structure.

  Parameters:
   [in]   pEntry   Pointer to a unicast master structure.
   [in]   pPort    Port this UMT entry will be added on.

*******************************************************************************/
static zlStatusE zl303xx_PtpUmtEntryStructCheck(zl303xx_PtpUmtEntryS *pEntry,
                                                zl303xx_PortDataS *pPort)
{
   zlStatusE status = ZL303XX_OK;

   /* Assume pointer is valid. */

   /* Check boolean parameters. */
   status = ZL303XX_CHECK_BOOLEAN(pEntry->entryConfig.reqAnnc) |
            ZL303XX_CHECK_BOOLEAN(pEntry->entryConfig.reqSync) |
            ZL303XX_CHECK_BOOLEAN(pEntry->entryConfig.reqDelayResp) |
            ZL303XX_CHECK_BOOLEAN(pEntry->entryConfig.reqPdelayResp) |
            ZL303XX_CHECK_BOOLEAN(pEntry->entryConfig.reqTiming) |
            ZL303XX_CHECK_BOOLEAN(pEntry->entryConfig.bmcaAllow);

   /* Check the stream configuration. */
   status |= zl303xx_PtpStreamCreateStructCheck(&pEntry->streamConfig, pPort);

   /* Check Telecom Profile specific members. */
   status |= zl303xx_PtpTelecomUmtEntryStructCheck(&pEntry->telecomConfig);

   if (status == ZL303XX_OK)
   {
      if ((pEntry->entryConfig.reqSync || pEntry->entryConfig.reqDelayResp ||
           pEntry->entryConfig.reqPdelayResp || pEntry->entryConfig.reqTiming) &&
           !pEntry->entryConfig.reqAnnc)
      {
         ZL303XX_TRACE_ALWAYS(
            "Requested timing data but not announce data. Defaulting reqAnnc to ZL303XX_TRUE.",
            0,0,0,0,0,0);

         pEntry->entryConfig.reqAnnc = ZL303XX_TRUE;
      }
   }

   return status;
}

/* zl303xx_PtpUmtEntryForceStreamConfig */
/** 

   Forces some members of the zl303xx_PtpStreamCreateS structure for unicast
   negotiation slave mode.

  Parameters:
   [in,out]  config   Stream configuration to update.

*******************************************************************************/
static void zl303xx_PtpUmtEntryForceStreamConfig(zl303xx_PtpStreamCreateS *config)
{
   config->unicast = ZL303XX_TRUE;
}

/* zl303xx_PtpUmtEntryIntAdd */
/** 

   Adds an entry to the Unicast Master Table.

  Parameters:
   [in]   pPort        Pointer an existing port.
   [in]   pEntry       Unicast master data provided by user.
   [in]   startStream  Flag to indicate if a stream should be started
   [out]  ppEntryInt   (Optional) A pointer to the newly created internal
                            UMT entry.

  Notes:   Assumes all input parameters are valid.
        The PTP data mutex must be taken before calling this function.

*******************************************************************************/
static zlStatusE zl303xx_PtpUmtEntryIntAdd(zl303xx_PortDataS *pPort,
                                           zl303xx_PtpUmtEntryS *pEntry,
                                           zl303xx_BooleanE startStream,
                                           zl303xx_PtpUmtEntryIntS **ppEntryInt)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpUmtEntryIntS *entryInt = NULL;
   zl303xx_BooleanE found;

   /* Find an empty entry in the UMT. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpUmtEntryFind(&pPort->unicastMasterTable,
                  &pEntry->streamConfig.destAddr, &found, &entryInt);
   }

   /* If the table entry is not found, add the new master data. */
   if (status == ZL303XX_OK)
   {
      if (!found)
      {
         pPort->unicastMasterTable.actualTableSize++;

         entryInt->inUse         = ZL303XX_TRUE;
         entryInt->portAddress   = pEntry->streamConfig.destAddr;
         entryInt->streamHandle  = (Uint32T)ZL303XX_PTP_INVALID;  /* Gets set later when stream is created. */
         entryInt->entryConfig   = pEntry->entryConfig;
         entryInt->telecomConfig = pEntry->telecomConfig;

         /* Telecom Profile: Determine which message types to request. */
         if (pPort->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1)
         {
            /* Internally, only reqSync and reqDelayResp are used. If both are
             * FALSE, but the deprecated reqTiming is TRUE, set both to TRUE. */
            if ((entryInt->entryConfig.reqSync == ZL303XX_FALSE) &&
                (entryInt->entryConfig.reqDelayResp == ZL303XX_FALSE) &&
                (entryInt->entryConfig.reqPdelayResp == ZL303XX_FALSE) &&
                (entryInt->entryConfig.reqTiming == ZL303XX_TRUE))
            {
               entryInt->entryConfig.reqSync      = ZL303XX_TRUE;

               if (pPort->config.delayMechanism == ZL303XX_DELAY_MECHANISM_E2E)
               {
                  entryInt->entryConfig.reqDelayResp = ZL303XX_TRUE;
               }
               else if (pPort->config.delayMechanism == ZL303XX_DELAY_MECHANISM_P2P)
               {
                  entryInt->entryConfig.reqPdelayResp = ZL303XX_TRUE;
               }
            }
         }
         /* Telecom Profile is not used -- request all message types. */
         else
         {
            entryInt->entryConfig.reqAnnc      = ZL303XX_TRUE;
            entryInt->entryConfig.reqSync      = ZL303XX_TRUE;

            if (pPort->config.delayMechanism == ZL303XX_DELAY_MECHANISM_E2E)
            {
               entryInt->entryConfig.reqDelayResp  = ZL303XX_TRUE;
               entryInt->entryConfig.reqPdelayResp = ZL303XX_FALSE;
            }
            else if (pPort->config.delayMechanism == ZL303XX_DELAY_MECHANISM_P2P)
            {
               entryInt->entryConfig.reqDelayResp  = ZL303XX_FALSE;
               entryInt->entryConfig.reqPdelayResp = ZL303XX_TRUE;
            }
         }

         /* Deprecated: Never allow this value to go to ZL303XX_TRUE internally. */
         entryInt->entryConfig.reqTiming = ZL303XX_FALSE;

         /* Set Telecom Profile specific members. */
         zl303xx_PtpTelecomUmtEntryIntAdd(&pEntry->telecomConfig, &entryInt->telecom);

         /* Return a pointer to the new UMT entry, if requested. */
         if (ppEntryInt)
         {
            *ppEntryInt = entryInt;
         }
      }
      else
      {
         ZL303XX_TRACE_ERROR("Entry (0x%x) already exists in Unicast Master Table.", pEntry->streamConfig.destAddr.addressField, 0,0,0,0,0);
         status = ZL303XX_TABLE_ENTRY_DUPLICATE;
      }
   }

   /* Start a stream to the new master. */
   if (status == ZL303XX_OK)
   {
      if (startStream)
      {
         status = zl303xx_PtpUmtEntryStartStream(pPort, &pEntry->streamConfig,
                                                 pEntry->entryConfig.extData, entryInt);
      }
   }

   if (status == ZL303XX_OK)
   {
      zl303xx_PtpUmtEntryServicesCheck(pPort, entryInt);

      /* G.8265.1 & G.8275.2:
       * Add this entry to the acceptable master table also. */
      if ((pPort->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1) ||
          (pPort->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_2))
      {
         if (zl303xx_PtpTableAdd(&pPort->accMasterTbl,
                                 &pEntry->streamConfig.destAddr) == NULL)
         {
            ZL303XX_TRACE_ERROR("Acceptable master table full.", 0,0,0,0,0,0);
            status = ZL303XX_TABLE_FULL;
         }
      }
   }

   return status;
}

/* zl303xx_PtpUmtEntryFind */
/** 

   Finds an entry in the Unicast Master Table, based on port address.

  Parameters:
   [in]   pUmt       Pointer a port's Unicast Master Table.
   [in]   pPortAddr  Port address to search for.
   [out]  pFound     Returns ZL303XX_TRUE if a match is found.
   [out]  ppEntry    (Optional) A pointer to the table entry matching pPortAddr.
                          If no match is found, this will return the first empty
                          table entry is returned instead.

  Return Value:  zlStatusE

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
static zlStatusE zl303xx_PtpUmtEntryFind(zl303xx_PtpUmtS *pUmt,
                                         zl303xx_PortAddress *pPortAddr,
                                         zl303xx_BooleanE *pFound,
                                         zl303xx_PtpUmtEntryIntS **ppEntry)
{
   zlStatusE status = ZL303XX_OK;
   Uint16T count = 0;
   zl303xx_PtpUmtEntryIntS *emptyEntry  = NULL;
   zl303xx_BooleanE found = ZL303XX_FALSE;

   /* Assume pUmt, pPortAddr, and pFound pointers are valid. */

   /* Check the table validity. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpUmtCheck(pUmt);
   }

   /* Search the table for a matching entry. */
   if (status == ZL303XX_OK)
   {
      for (count = 0; (count < pUmt->maxTableSize) && !found; count++)
      {
         if (pUmt->master[count].inUse == ZL303XX_TRUE)
         {
            if (zl303xx_PtpComparePortAddress(pPortAddr, &pUmt->master[count].portAddress) == 0)
            {
               /* Return a pointer to the matching entry, if requested. */
               if (ppEntry)
               {
                  *ppEntry = &pUmt->master[count];
               }

               /* Set found flag to break out of loop. */
               found = ZL303XX_TRUE;
            }
         }

         /* Store the first empty entry found in the table. */
         else if (emptyEntry == NULL)
         {
            emptyEntry = &pUmt->master[count];
         }
      }

      /* Return a pointer to the first empty entry, if requested. */
      if (!found && ppEntry)
      {
         *ppEntry = emptyEntry;
      }

      /* Always return the found flag. */
      *pFound = found;
   }

   return status;
}

/* zl303xx_PtpUnicastMasterParamOper */
/** 

   Gathers data for a GET/SET operation on a parameter of a Unicast Master Table
   entry and executes the operation.

  Parameters:
   [in]   streamHandle  Handle to a previously create PTP stream.
   [in]   param         The UMT entry parameter to modify.
   [in]   setParam      Flag to indicate that the contents of pVal are to be
                             written to param.
   [in,out] pVal        If setParam == ZL303XX_TRUE, this holds the value of
                             the param to set. If setParam == ZL303XX_FALSE, the
                             value of param is returned in this variable.
   [out]  ppEntry       (Optional) Returns a pointer to the unicast master
                             entry matching pPortAddr.

*******************************************************************************/
static zlStatusE zl303xx_PtpUnicastMasterParamOper(zl303xx_PtpStreamHandleT streamHandle,
                                                   zl303xx_PtpUmtEntryParamE param,
                                                   zl303xx_BooleanE setParam,
                                                   void *pVal,
                                                   zl303xx_PtpUmtEntryIntS **ppEntry)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
   zl303xx_ClockStreamS *pStream = NULL;

   /* Get a pointer to the stream data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamDataGet(streamHandle, &pStream);
   }

   /* Check that the stream has a UMT entry associated with it. */
   if (status == ZL303XX_OK)
   {
      if (pStream->pUmtEntry == NULL)
      {
         ZL303XX_TRACE_ALWAYS("No Unicast Master entry for streamHandle=%lu.", streamHandle, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Take the clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pStream->portData->clock);
      status = mStatus;
   }

   /* Set the parameter for this entry. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpUmtEntryParamOper(pStream->pUmtEntry, param,
                                            setParam, pVal, pStream);
   }

   /* Release the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pStream->portData->clock, status);
   }

   /* Return a pointer to the table entry, if requested. */
   if (status == ZL303XX_OK)
   {
      if (ppEntry)
      {
         *ppEntry = pStream->pUmtEntry;
      }
   }

   return status;
}

/* zl303xx_PtpUmtEntryParamOper */
/** 

   Performs a GET/SET operation on a parameter of a Unicast Master Table entry.

  Parameters:
   [in]      pEntry    Pointer to a UMT entry to perform GET/SET on.
   [in]      param     The UMT entry parameter to modify.
   [in]      setParam  GET/SET flag.
   [in,out]  pVal      Returned value or value to set, depending on setParam.
   [in,out]  pStream   (Optional) Pointer to a PTP stream structure.

*******************************************************************************/
static zlStatusE zl303xx_PtpUmtEntryParamOper(zl303xx_PtpUmtEntryIntS *pEntry,
                                              zl303xx_PtpUmtEntryParamE param,
                                              zl303xx_BooleanE setParam,
                                              void *pVal,
                                              zl303xx_ClockStreamS *pStream)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      if (setParam == ZL303XX_TRUE)
      {
         switch (param)
         {
            case UMT_PARAM_BMCA_ALLOW :
            {
               /* Just set the boolean. Any change will get picked up the next
                * time BMCA runs. */
               OS_MEMCPY(&pEntry->entryConfig.bmcaAllow, pVal, sizeof(pEntry->entryConfig.bmcaAllow));
               break;
            }

            case UMT_PARAM_MAX_INTVL :
            {
               umtEntryMaxIntvlS *max = pVal;

               if (max->msgType == ZL303XX_MSG_ID_SYNC)
                  pEntry->entryConfig.maxSyncIntvl = max->intvl;
               else if (max->msgType == ZL303XX_MSG_ID_DELAY_RESP)
                  pEntry->entryConfig.maxDelayRespIntvl = max->intvl;
               else if (max->msgType == ZL303XX_MSG_ID_PEER_DELAY_RESP)
                  pEntry->entryConfig.maxPdelayRespIntvl = max->intvl;
               else if (max->msgType == ZL303XX_MSG_ID_ANNOUNCE)
                  pEntry->entryConfig.maxAnncIntvl = max->intvl;

               break;
            }

            case UMT_PARAM_TELECOM_SYNC_TIMEOUT :
            {
               OS_MEMCPY(&pEntry->telecom.syncReceiptTimeoutSec, pVal,
                      sizeof(pEntry->telecom.syncReceiptTimeoutSec));

               if (pEntry->telecom.syncTimeoutCounter > pEntry->telecom.syncReceiptTimeoutSec)
               {
                  pEntry->telecom.syncTimeoutCounter = pEntry->telecom.syncReceiptTimeoutSec;
               }
               break;
            }

            case UMT_PARAM_TELECOM_FOLLOW_TIMEOUT :
            {
               OS_MEMCPY(&pEntry->telecom.followReceiptTimeoutSec, pVal,
                      sizeof(pEntry->telecom.followReceiptTimeoutSec));

               if (pEntry->telecom.followTimeoutCounter > pEntry->telecom.followReceiptTimeoutSec)
               {
                  pEntry->telecom.followTimeoutCounter = pEntry->telecom.followReceiptTimeoutSec;
               }
               break;
            }

            case UMT_PARAM_TELECOM_SYNC_FOLLOW_TIMEOUT :
            {
               OS_MEMCPY(&pEntry->telecom.sofReceiptTimeoutSec, pVal,
                      sizeof(pEntry->telecom.sofReceiptTimeoutSec));

               if (pEntry->telecom.sofTimeoutCounter > pEntry->telecom.sofReceiptTimeoutSec)
               {
                  pEntry->telecom.sofTimeoutCounter = pEntry->telecom.sofReceiptTimeoutSec;
               }
               break;
            }

            case UMT_PARAM_TELECOM_DELAY_TIMEOUT :
            {
               OS_MEMCPY(&pEntry->telecom.delayReceiptTimeoutSec, pVal,
                      sizeof(pEntry->telecom.delayReceiptTimeoutSec));

               if (pEntry->telecom.delayTimeoutCounter > pEntry->telecom.delayReceiptTimeoutSec)
               {
                  pEntry->telecom.delayTimeoutCounter = pEntry->telecom.delayReceiptTimeoutSec;
               }
               break;
            }

            case UMT_PARAM_TELECOM_PDELAY_TIMEOUT :
            {
               OS_MEMCPY(&pEntry->telecom.pdelayReceiptTimeoutSec, pVal,
                      sizeof(pEntry->telecom.pdelayReceiptTimeoutSec));

               if (pEntry->telecom.pdelayTimeoutCounter > pEntry->telecom.pdelayReceiptTimeoutSec)
               {
                  pEntry->telecom.pdelayTimeoutCounter = pEntry->telecom.pdelayReceiptTimeoutSec;
               }
               break;
            }

            case UMT_PARAM_TELECOM_PDELAY_FOLLOW_TIMEOUT :
            {
               OS_MEMCPY(&pEntry->telecom.pdelayFollowReceiptTimeoutSec, pVal,
                      sizeof(pEntry->telecom.pdelayFollowReceiptTimeoutSec));

               if (pEntry->telecom.pdelayFollowTimeoutCounter > pEntry->telecom.pdelayFollowReceiptTimeoutSec)
               {
                  pEntry->telecom.pdelayFollowTimeoutCounter = pEntry->telecom.pdelayFollowReceiptTimeoutSec;
               }
               break;
            }

            case UMT_PARAM_TELECOM_PRIORITY :
            {
               OS_MEMCPY(&pEntry->telecom.priority, pVal, sizeof(pEntry->telecom.priority));
               OS_MEMCPY(&pEntry->telecomConfig.priority, pVal, sizeof(pEntry->telecomConfig.priority));
               break;
            }

            case UMT_PARAM_TELECOM_RETRY :
            {
               Uint8T msgIdx;

               for (msgIdx = PTP_NEGOTIATED_MSG_ID_SYNC; msgIdx < PTP_NEGOTIATED_NUM_MSG_TYPES; msgIdx++)
               {
                  zl303xx_PtpContractMsgRetryStatusInit(&pStream->contractRx[msgIdx].retry,
                                                        &pStream->config.unicastNegotiation.retry);
               }

               zl303xx_PtpContractLockoutStatusInit(&pStream->unicast.lockout);

               break;
            }

            case UMT_PARAM_TELECOM_WTR_SET :
            {
               Uint32T wtr;
               OS_MEMCPY(&wtr, pVal, sizeof(wtr));

               zl303xx_PtpTelecomWtrStart(pEntry->streamHandle,
                                          &pEntry->telecom,
                                          wtr);
               break;
            }

            /* fall-through for removal of switch-enum warning */
            case UMT_PARAM_IN_USE :
            case UMT_PARAM_STREAM_HANDLE :
            case UMT_PARAM_DEST_ADDR :
            case UMT_PARAM_EXT_DATA :
            case UMT_PARAM_ANNC_INTVL :
            case UMT_PARAM_SYNC_INTVL :
            case UMT_PARAM_DELAY_INTVL :
            case UMT_PARAM_ANNC_TIMEOUT :
            case UMT_PARAM_NEG_DURATION :
            case UMT_PARAM_NEG_RENEW :
            case UMT_PARAM_TELECOM_SSM_QL :
            case UMT_PARAM_STATUS :
            case UMT_NUM_PARAMS :
            default :
               ZL303XX_TRACE_ALWAYS("Unable to SET unicast master param %u.",
                                    param, 0,0,0,0,0);
               status = ZL303XX_PARAMETER_INVALID;
               break;
         }
      }
      else
      {
         switch (param)
         {
            case UMT_PARAM_TELECOM_PRIORITY :
               OS_MEMCPY(pVal, &pEntry->telecom.priority, sizeof(pEntry->telecom.priority));
               break;

            case UMT_PARAM_TELECOM_SSM_QL :
               OS_MEMCPY(pVal, &pEntry->telecom.pQuality->ssm, sizeof(pEntry->telecom.pQuality->ssm));
               break;

            case UMT_PARAM_STATUS :
               zl303xx_PtpUmtEntryStatusFill(pStream, pVal);
               break;

            /* fall-through for removal of switch-enum warning */
            case UMT_PARAM_IN_USE :
            case UMT_PARAM_STREAM_HANDLE :
            case UMT_PARAM_DEST_ADDR :
            case UMT_PARAM_EXT_DATA :
            case UMT_PARAM_BMCA_ALLOW :
            case UMT_PARAM_ANNC_INTVL :
            case UMT_PARAM_SYNC_INTVL :
            case UMT_PARAM_DELAY_INTVL :
            case UMT_PARAM_ANNC_TIMEOUT :
            case UMT_PARAM_NEG_DURATION :
            case UMT_PARAM_NEG_RENEW :
            case UMT_PARAM_MAX_INTVL :
            case UMT_PARAM_TELECOM_SYNC_TIMEOUT :
            case UMT_PARAM_TELECOM_FOLLOW_TIMEOUT :
            case UMT_PARAM_TELECOM_SYNC_FOLLOW_TIMEOUT :
            case UMT_PARAM_TELECOM_DELAY_TIMEOUT :
            case UMT_PARAM_TELECOM_PDELAY_TIMEOUT :
            case UMT_PARAM_TELECOM_PDELAY_FOLLOW_TIMEOUT :
            case UMT_PARAM_TELECOM_RETRY :
            case UMT_PARAM_TELECOM_WTR_SET :
            case UMT_NUM_PARAMS :
            default :
               ZL303XX_TRACE_ALWAYS("Unable to GET unicast master param %u.",
                                    param, 0,0,0,0,0);
               status = ZL303XX_PARAMETER_INVALID;
               break;
         }
      }
   }

   return status;
}

/* zl303xx_PtpUmtEntryStatusFill */
/** 

   Fills a zl303xx_PtpUmtEntryStatusS structure with data from a PTP stream.

  Parameters:
   [in]   pStream   Pointer to a PTP stream structure.
   [out]  pStatus   Filled status structure.

*******************************************************************************/
static void zl303xx_PtpUmtEntryStatusFill(zl303xx_ClockStreamS *pStream,
                                           zl303xx_PtpUmtEntryStatusS *pStatus)
{
   Uint8T msgIdx;

   OS_MEMSET(pStatus, 0, sizeof(*pStatus));

   pStatus->clockClass = pStream->farEndParentDS.u.v2.grandmasterClockQuality.clockClass;

   /* Stats that only apply for Telecom Profile. */
   if (pStream->portData->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1)
   {
      pStatus->telecom.ptsf = 0;  /* Deprecated */
      if (pStream->pUmtEntry)
      {
         pStatus->telecom.quality = *(pStream->pUmtEntry->telecom.pQuality);
         pStatus->telecom.waitToRestoreSec = pStream->pUmtEntry->telecom.wtr;
      }

      /* Per message stats. */
      for (msgIdx = PTP_NEGOTIATED_MSG_ID_SYNC; msgIdx < PTP_NEGOTIATED_NUM_MSG_TYPES; msgIdx++)
      {
         pStatus->telecom.msgType[msgIdx] = pStream->contractRx[msgIdx].retry;
      }

      /* Per stream stats. */
      pStatus->telecom.stream = pStream->unicast.lockout;
   }
}

/* zl303xx_PtpUmtEntryStartStream */
/** 

   Creates a stream for an entry in the Unicast Master Table.

  Parameters:
   [in]      pPort     Pointer to the port this stream will be created on.
   [in]      pCreate   Parameters to create the stream.
   [in]      extData   External data for the STREAM_CREATE event handler.
   [in,out]  pEntry    Pointer to the internal UMT entry. streamHandle is
                            set by this function.

  Return Value:  zlStatusE

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
static zlStatusE zl303xx_PtpUmtEntryStartStream(zl303xx_PortDataS *pPort,
                                                zl303xx_PtpStreamCreateS *pCreate,
                                                Uint8T *extData,
                                                zl303xx_PtpUmtEntryIntS *pEntry)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ClockStreamS *pStream;

   /* Make sure the table is initialized. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpUmtCheck(&pPort->unicastMasterTable);
   }

   /* Check if a connection to this master already exists. */
   if (status == ZL303XX_OK)
   {
      pStream = zl303xx_PtpStreamExistsOnPort(pPort, &pCreate->destAddr);

      /* Return the stream handle to the existing stream and exit function early. */
      if (pStream)
      {
         pEntry->streamHandle = pStream->clockStreamHandle;

         /*Check if the connection exists but UMT entry to this
          * master does not exist */
         if(pStream->pUmtEntry == NULL)
         {
            /* Save a pointer to the UMT entry in the stream struct. */
            pStream->pUmtEntry = pEntry;
         }
         return status;
      }

      /* Fire the stream created event. */
      pCreate->createFlags |= ZL303XX_PTP_STREAM_CREATE_FIRE_EVENT;

      /* Stream does not yet exist, so create it. */
      status = zl303xx_PtpStreamCreateInternal(pCreate, pPort, extData, &pStream);
   }

   if (status == ZL303XX_OK)
   {
      pEntry->streamHandle = pStream->clockStreamHandle;

      /* Initialize the stream (& place it in an IDLE state). */
      zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_INIT, pStream);

      /* Save a pointer to the UMT entry in the stream struct. */
      pStream->pUmtEntry = pEntry;
   }

   return status;
}

/****************   STATIC FUNCTION DEFINITIONS   ****************************/

/* ConvertUniDiscoveryToGeneric */
/** 

   Simple function to copy the necessary members of the zl303xx_MasterAddressS
   type to the associated members of the zl303xx_GenericPortInfoS type prior
   to calling a generic port address function.

  Parameters:
   [in]   maPar    Pointer to the zl303xx_MasterAddressS data structure.
   [out]  gpaPar   Pointer to the equivalent zl303xx_GenericPortInfoS data structure.

  Notes:
   Does not check the pointer status since the function should only be called
   locally and they should have already been verified.

*******************************************************************************/
static void ConvertUniDiscoveryToGeneric(zl303xx_MasterAddressS *maPar,
                                         zl303xx_GenericPortInfoS *gpaPar)
{
   gpaPar->portAddressHandle = maPar->portAddressHandle;
   gpaPar->portAddress = maPar->portAddress;
   gpaPar->empty = ZL303XX_FALSE;
}  /* END ConvertUniDiscoveryToGeneric */

/* ConvertUniDiscoveryToGeneric */
/** 

   Simple function to copy the necessary members of the zl303xx_GenericPortInfoS
   type to the associated members of the zl303xx_MasterAddressS type after
   calling a generic port address function and prior to returning the data from
   a Unicast Discovery function.

  Parameters:
   [in]   gpaPar   Pointer to the equivalent zl303xx_GenericPortInfoS data structure.
   [out]  maPar    Pointer to the zl303xx_MasterAddressS data structure.

  Notes:
   Does not check the pointer status since the function should only be called
   locally and they should have already been verified.

*******************************************************************************/
static void ConvertGenericToUniDiscovery(zl303xx_GenericPortInfoS *gpaPar,
                                         zl303xx_MasterAddressS *maPar)
{
   maPar->portAddressHandle = gpaPar->portAddressHandle;
   maPar->portAddress = gpaPar->portAddress;
}  /* END ConvertGenericToUniDiscovery */

/****************   DEPRECATED FUNCTION DEFINITIONS   *************************/

/* zl303xx_PtpUniDiscoveryMasterExists */
/**
   Checks a PTP port's unicast discovery table to see if a master's address
   exists in it.

  Parameters:
   [in]   portHandle    Handle to a previously created port.
   [in]   pMasterAddr   The master address to check for.
   [out]  pExists       ZL303XX_TRUE if the master address exists, ZL303XX_FALSE
                             otherwise.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pMasterAddr or pExists is NULL.
     ZL303XX_PARAMETER_INVALID  portHandle is invalid.
     ZL303XX_INIT_ERROR         The port's unicast discovery table has not
                                    been initialized.

  (Deprecated)
   This function has been replaced by zl303xx_PtpUmtEntryExists().

*******************************************************************************/
zlStatusE zl303xx_PtpUniDiscoveryMasterExists(zl303xx_PtpPortHandleT portHandle,
                                              zl303xx_PortAddress *pMasterAddr,
                                              zl303xx_BooleanE *pExists)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpUmtEntryS entry;
   zl303xx_PtpPortHandleT ph;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpUniDiscoveryMasterExists: (Deprecated)", 0,0,0,0,0,0);

   (void)portHandle;  /* Warning removal */

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pMasterAddr) |
               ZL303XX_CHECK_POINTER(pExists);
   }

   /* Call the new function. */
   if (status == ZL303XX_OK)
   {
      entry.streamConfig.destAddr = *pMasterAddr;
      status = zl303xx_PtpUmtEntryExists(&entry, &ph, NULL);
   }

   if (status == ZL303XX_OK)
   {
      *pExists = (ph != (zl303xx_PtpPortHandleT)ZL303XX_PTP_INVALID)
                    ? ZL303XX_TRUE
                    : ZL303XX_FALSE;
   }

   return status;
}

/* zl303xx_PtpUniDiscoveryMasterAdd */
/**
   Adds a master's address to a port's unicast discovery table.

  Parameters:
   [in]   portHandle    Handle to a previous created port.
   [in]   pMasterAddr   Port address of the master to add.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       pMasterAddr is NULL.
     ZL303XX_INIT_ERROR            portHandle is invalid, or unicast negotiation
                                       not enabled for this port.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Unable to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Unable to release mutex.
     ZL303XX_TABLE_ENTRY_DUPLICATE The master's port address has already been
                                       added to the table.
     ZL303XX_TABLE_FULL            No space left in the table.

  (Deprecated)
   This function has been replaced by zl303xx_PtpUmtEntryAdd().

*******************************************************************************/
zlStatusE zl303xx_PtpUniDiscoveryMasterAdd(zl303xx_PtpPortHandleT portHandle,
                                           zl303xx_PortAddress *pMasterAddr)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
   zl303xx_PtpUmtEntryS entry;
   zl303xx_PortDataS *pPort;

   ZL303XX_TRACE_ALWAYS("zl303xx_PtpUniDiscoveryMasterAdd: (Deprecated)", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pMasterAddr);
   }

   /* Initialize the new UMT entry. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpUmtEntryStructInit(&entry);
   }

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   /* Take the PTP clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pPort->clock);
      status = mStatus;
   }

   /* Call the new function. */
   if (status == ZL303XX_OK)
   {
      entry.streamConfig.destAddr = *pMasterAddr;
      status = zl303xx_PtpUmtEntryIntAdd(pPort, &entry, ZL303XX_FALSE, NULL);
   }

   /* Release the PTP data mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }

   return status;
}

/* zl303xx_MasterAddressStructInit */
/**
   Initializes a zl303xx_MasterAddressS data structure.

  Parameters:
   [in]   maPar   Pointer to the structure to be initialized.

  (Deprecated)
   The zl303xx_MasterAddressS structure is no longer supported.

*******************************************************************************/
zlStatusE zl303xx_MasterAddressStructInit(zl303xx_MasterAddressS *maPar)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE_ALWAYS("zl303xx_MasterAddressStructInit: (Deprecated)", 0,0,0,0,0,0);

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(maPar);
   }

   /* Use the default initialization routine */
   if (status == ZL303XX_OK)
   {
      maPar->portAddressHandle = (Uint32T)ZL303XX_PTP_INVALID;
      maPar->portAddress.networkProtocol = (zl303xx_NetworkProtocolE)ZL303XX_PTP_INVALID;
      maPar->portAddress.addressLength = 0;
      OS_MEMSET(maPar->portAddress.addressField, 0, sizeof(maPar->portAddress.addressField));
   }

   return status;
}

/* zl303xx_UniDiscoveryTblClearAllEntries */
/**  (Deprecated)

   Clears all of the masterAddress entries in the Unicast Discovery Table but
   does not free its allocated memory

  Parameters:
   [in]   udt   Pointer to a port's unicast discovery table.

*******************************************************************************/
zlStatusE zl303xx_UniDiscoveryTblClearAllEntries(zl303xx_UniDiscoveryTblS *udt)
{
   ZL303XX_TRACE_ALWAYS("zl303xx_UniDiscoveryTblClearAllEntries: (Deprecated)", 0,0,0,0,0,0);

   /* Use the default clear routine */
   return zl303xx_GenericPortAddrTblClearEntries(udt);
}

/* zl303xx_UniDiscoveryTblMaxSize */
/**  (Deprecated)

   Returns the maximum masterAddress entries attribute of a Unicast Discovery
   Table. This is needed for ACCEPTABLE_MASTER_MAX_TABLE_SIZE messages.

  Parameters:
   [in]   udt       Pointer to a port's unicast discovery table.
   [in]   maxSize   Pointer to return the maxTableSize attribute of the table.

*******************************************************************************/
zlStatusE zl303xx_UniDiscoveryTblMaxSize(zl303xx_UniDiscoveryTblS *udt,
                                         Uint16T *maxSize)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE_ALWAYS("zl303xx_UniDiscoveryTblMaxSize: (Deprecated)", 0,0,0,0,0,0);

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(maxSize);
   }

   /* Return the maxTableSize attribute */
   if (status == ZL303XX_OK)
   {
      *maxSize = udt->maxTableSize;
   }

   return status;
}

/* zl303xx_UniDiscoveryTblActSize */
/**  (Deprecated)

   Returns the actual number of masterAddress entries in a Unicast Discovery
   Table. Currently, this is not needed for any ANNOUNCE message.

  Parameters:
   [in]   udt       Pointer to a port's unicast discovery table.
   [in]   actSize   Pointer to return the maxTableSize attribute of the table.

*******************************************************************************/
zlStatusE zl303xx_UniDiscoveryTblActSize(zl303xx_UniDiscoveryTblS *udt,
                                         Uint16T *actSize)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE_ALWAYS("zl303xx_UniDiscoveryTblActSize: (Deprecated)", 0,0,0,0,0,0);

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(actSize);
   }

   /* Return the actualTableSize attribute */
   if (status == ZL303XX_OK)
   {
      *actSize = udt->actualTableSize;
   }

   return status;
}

/* zl303xx_UniDiscoveryMasterExists */
/**  (Deprecated)

   Determines if the Unicast Discovery Table already contains the specified
   masterAddress entry.

  Parameters:
   [in]   udt       Pointer to a port's unicast discovery table.
   [in]   maPar     Pointer to masterAddress info to look for in the Table.
   [in]   present   Pointer indicating if the specified masterAddress entry exists.

*******************************************************************************/
zlStatusE zl303xx_UniDiscoveryMasterExists(zl303xx_UniDiscoveryTblS *udt,
                                           zl303xx_MasterAddressS *maPar,
                                           zl303xx_BooleanE *present)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_GenericPortInfoS *entry = NULL;
   zl303xx_GenericPortInfoS maInput;

   ZL303XX_TRACE_ALWAYS("zl303xx_UniDiscoveryMasterExists: (Deprecated)", 0,0,0,0,0,0);

   /* Look for the entry in the Table */
   if (status == ZL303XX_OK)
   {
      ConvertUniDiscoveryToGeneric(maPar, &maInput);

      status = zl303xx_GenericPortAddrTblEntryExists(udt, &maInput, &entry);

      *present = ( (entry) ? ZL303XX_TRUE : ZL303XX_FALSE );
   }

   return status;
}

/* zl303xx_UniDiscoveryMasterIndex */
/**  (Deprecated)

   Gets the masterAddress entry specified by Index. It counts only non-empty
   entries so that passing an index value of 0 will return the first entry
   regardless of where it is in table. If the index value is too big, NULL is
   passed back.

  Parameters:
   [in]   udt        Pointer to a port's unicast discovery table.
   [in]   maPar      Pointer to masterAddress info to look for in the Table.
   [in]   tblIndex   Index of the entry to return.

  Notes:
   Index starts at 0 and goes up to 1 less than the maximum table size.

*******************************************************************************/
zlStatusE zl303xx_UniDiscoveryMasterIndex(zl303xx_UniDiscoveryTblS *udt,
                                          zl303xx_MasterAddressS *maPar,
                                          Uint16T tblIndex)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_GenericPortInfoS gpaPar;

   ZL303XX_TRACE_ALWAYS("zl303xx_UniDiscoveryMasterIndex: (Deprecated)", 0,0,0,0,0,0);

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(maPar);
   }

   /* Pass off to our Generic equivalent */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_GenericPortAddrTblEntryIndex(udt, &gpaPar, tblIndex);

      /* Regardless of the status, copy the data to the maPar. Since status is
         passed back up the chain, the caller will know if the data is OK */
      ConvertGenericToUniDiscovery(&gpaPar, maPar);
   }

   return status;
}

/* zl303xx_UniDiscoveryAddMaster */
/**  (Deprecated)

   Adds a masterAddress entry to the Unicast Discovery Table if it does not
   already exist and space allows

  Parameters:
   [in]   udt     Pointer to a port's unicast discovery table.
   [in]   maPar   Pointer to masterAddress info to add.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_UniDiscoveryAddMaster(zl303xx_UniDiscoveryTblS *udt,
                                        zl303xx_MasterAddressS *maPar)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_GenericPortInfoS maInput;

   ZL303XX_TRACE_ALWAYS("zl303xx_UniDiscoveryAddMaster: (Deprecated)", 0,0,0,0,0,0);

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(maPar);
   }

   /* Attempt to add the entry to the Table */
   if (status == ZL303XX_OK)
   {
      ConvertUniDiscoveryToGeneric(maPar, &maInput);

      status = zl303xx_GenericPortAddrTblAddEntry(udt, &maInput);
   }

   return status;
}

/* zl303xx_UniDiscoveryDelMaster */
/**  (Deprecated)

   Deletes a masterAddress entry from the Unicast Discovery Table if it exists

  Parameters:
   [in]   udt     Pointer to a port's unicast discovery table.
   [in]   maPar   Pointer to masterAddress info to delete.

*******************************************************************************/
zlStatusE zl303xx_UniDiscoveryDelMaster(zl303xx_UniDiscoveryTblS *udt,
                                        zl303xx_MasterAddressS *maPar)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_GenericPortInfoS maInput;

   ZL303XX_TRACE_ALWAYS("zl303xx_UniDiscoveryDelMaster: (Deprecated)", 0,0,0,0,0,0);

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(maPar);
   }

   /* Attempt to remove the entry from the Table */
   if (status == ZL303XX_OK)
   {
      ConvertUniDiscoveryToGeneric(maPar, &maInput);

      status = zl303xx_GenericPortAddrTblDelEntry(udt, &maInput);
   }

   return status;
}

/* zl303xx_UniDiscoveryUpdateMaster */
/**  (Deprecated)

   Updates info for the specified masterAddress entry in the Unicast Discovery
   Table if it exists.

  Parameters:
   [in]   udt     Pointer to a port's unicast discovery table.
   [in]   maPar   Pointer to masterAddress info to update. The portAddress
                       portion of the masterAddress structure must contain the
                       portAddress of interest.

*******************************************************************************/
zlStatusE zl303xx_UniDiscoveryUpdateMaster(zl303xx_UniDiscoveryTblS *udt,
                                           zl303xx_MasterAddressS *maPar)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_GenericPortInfoS maInput;
   zl303xx_GenericPortInfoS *present = NULL;

   ZL303XX_TRACE_ALWAYS("zl303xx_UniDiscoveryUpdateMaster: (Deprecated)", 0,0,0,0,0,0);

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(maPar);
   }

   /* Look for the entry in the Table */
   if (status == ZL303XX_OK)
   {
      ConvertUniDiscoveryToGeneric(maPar, &maInput);

      status = zl303xx_GenericPortAddrTblEntryExists(udt, &maInput, &present);
   }

   if (status == ZL303XX_OK)
   {
      /* If the entry exists, update it with our new parameters */
      if (present != NULL)
      {
         ConvertUniDiscoveryToGeneric(maPar, present);
      }
      else
      {
         status = ZL303XX_STREAM_NOT_IN_USE;
      }
   }

   return status;
}

/* zl303xx_UniDiscoveryPktPerSecGet */
/**  (Deprecated)

   Converts the logQueryInterval member of the Unicast Discovery Table to
   a corresponding Packet/Second value

  Parameters:
   [in]   udt   Pointer to a port's unicast discovery table.
   [in]   pps   Pointer to a Pkt/Sec value converted from logQueryInterval.

  Notes:
   The logQueryInterval member must be -ve or 0 (interval <= 1 sec) in order for
   the Packet/Second value to be greater than 1.

   This function passes all parameters to zl303xx_GenericPortAddrTblPpsGet()
   for appropriate error checking.

*******************************************************************************/
zlStatusE zl303xx_UniDiscoveryPktPerSecGet(zl303xx_UniDiscoveryTblS *udt,
                                           Uint32T *pps)
{
   ZL303XX_TRACE_ALWAYS("zl303xx_UniDiscoveryPktPerSecGet: (Deprecated)", 0,0,0,0,0,0);

   /* The generic function will check any pointers */
   return zl303xx_GenericPortAddrTblPpsGet(udt, pps);
}

/* zl303xx_UniDiscoveryPktPerSecSet */
/**  (Deprecated)

   Sets the logQueryInterval member of the Unicast Discovery Table by converting
   the supplied Packet/Second value to a log2 equivalent

  Parameters:
   [in]   udt   Pointer to a port's unicast discovery table.
   [in]   pps   Pointer to the Pkt/Sec value to convert to logQueryInterval.

  Notes:
   If the supplied Packet/Second Rate is not an even log2 value, then
   logQueryInterval will be set so that the Rate is the fastest possible that
   is LESS-THAN the Rate requested.
   The actual Packet/Second Rate set is passed back via the pps pointer.
   Ex:   requested PPS = 15 (0000 1111)
            actual PPS = 8  (0000 1000)
         logQueryInterval = -3

   This function passes all parameters to zl303xx_GenericPortAddrTblPpsSet()
   for appropriate error checking.

*******************************************************************************/
zlStatusE zl303xx_UniDiscoveryPktPerSecSet(zl303xx_UniDiscoveryTblS *udt,
                                           Uint32T *pps)
{
   ZL303XX_TRACE_ALWAYS("zl303xx_UniDiscoveryPktPerSecSet: (Deprecated)", 0,0,0,0,0,0);

   /* The generic function will check any pointers */
   return zl303xx_GenericPortAddrTblPpsSet(udt, pps);
}

/* zl303xx_UniDiscoverySecPerPktGet */
/**  (Deprecated)

   Converts the logQueryInterval member of the Unicast Discovery Table to
   a corresponding Seconds/Packet value (ie. time between query packets)

  Parameters:
   [in]   udt   Pointer to a port's unicast discovery table.
   [in]   spp   Pointer to a Sec/Pkt value converted from logQueryInterval.

  Notes:
   The logQueryInterval member must be a +ve or 0 (interval >= 1 sec) in order
   for the Seconds/Packet value to be greater than 1.

   This function passes all parameters to zl303xx_GenericPortAddrTblSppGet()
   for appropriate error checking.

*******************************************************************************/
zlStatusE zl303xx_UniDiscoverySecPerPktGet(zl303xx_UniDiscoveryTblS *udt,
                                           Uint32T *spp)
{
   ZL303XX_TRACE_ALWAYS("zl303xx_UniDiscoverySecPerPktGet: (Deprecated)", 0,0,0,0,0,0);

   /* The generic function will check any pointers */
   return zl303xx_GenericPortAddrTblSppGet(udt, spp);
}

/* zl303xx_UniDiscoverySecPerPktSet */
/**  (Deprecated)

   Sets the logQueryInterval member of the Unicast Discovery Table by converting
   the corresponding Seconds/Packet value

  Parameters:
   [in]   udt   Pointer to a port's unicast discovery table.
   [in]   spp   Pointer to the Secs/Pkt value to convert to logQueryInterval.

  Notes:
   If the passed in Seconds/Packet Interval is not an even log2 value, then
   logQueryInterval will be set so that the Interval is the largest possible
   that is GREATER-THAN the Interval requested
   The actual Seconds/Packet Interval set is passed back via the spp pointer.
   Ex:   requested SPP = 15 (0000 1111)
            actual SPP = 16 (0001 0000) (Thus, a slower pps).
         logQueryInterval = 4

   This function passes all parameters to zl303xx_GenericPortAddrTblSppSet()
   for appropriate error checking.

*******************************************************************************/
zlStatusE zl303xx_UniDiscoverySecPerPktSet(zl303xx_UniDiscoveryTblS *udt,
                                           Uint32T *spp)
{
   ZL303XX_TRACE_ALWAYS("zl303xx_UniDiscoverySecPerPktGet: (Deprecated)", 0,0,0,0,0,0);

   /* The generic function will check any pointers */
   return zl303xx_GenericPortAddrTblSppSet(udt, spp);
}

/* zl303xx_UniDiscoveryLogIntervalCheck */
/**  (Deprecated)

   Checks that the logQueryInterval value and limits meet the limits defined by
   the Unicast Discovery Table. Also does some other simple limit/range
   consistency checks.

  Parameters:
   [in]   udt   Pointer to a port's unicast discovery table.

  Notes:
   This function passes all parameters to zl303xx_GenericPortAddrTblLogIntervalChk
   for appropriate error checking.

*******************************************************************************/
zlStatusE zl303xx_UniDiscoveryLogIntervalCheck(zl303xx_UniDiscoveryTblS *udt)
{
   ZL303XX_TRACE_ALWAYS("zl303xx_UniDiscoveryLogIntervalCheck: (Deprecated)", 0,0,0,0,0,0);

   return zl303xx_GenericPortAddrTblLogIntervalChk(udt);
}

/* zl303xx_UniDiscoveryLogIntervalSet */
/** 

   Sets the logQueryInterval value of the Unicast Discovery Table in raw format

  Parameters:
   [in]   udt           Pointer to a port's unicast discovery table.
   [in]   logInterval   The raw logQueryInterval value.

  Notes:
   This function passes all parameters to zl303xx_GenericPortAddrTblLogIntervalSet
   for appropriate error checking.

*******************************************************************************/
zlStatusE zl303xx_UniDiscoveryLogIntervalSet(zl303xx_UniDiscoveryTblS *udt,
                                             Sint32T logInterval)
{
   ZL303XX_TRACE_ALWAYS("zl303xx_UniDiscoveryLogIntervalSet: (Deprecated)", 0,0,0,0,0,0);

   /* The generic function will check any parameters */
   return zl303xx_GenericPortAddrTblLogIntervalSet(udt, logInterval);
}

/* zl303xx_PtpV2BmcUmtQualified */
/** 

   Determines if the UMT entry associated with an FMT entry is qualified. This
   is used as an input to determine if the FMT entry can be qualified.

  Parameters:
   [in]   pStream   Stream to evaluate.

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtpV2BmcUmtQualified(zl303xx_ClockStreamS *pStream)
{
       /* A stream with no UMT entry is always considered qualified. */
   if ((pStream->pUmtEntry == NULL) ||
        /* UMT entry exists, bmcaAllow must be TRUE. */
       ((pStream->pUmtEntry->entryConfig.bmcaAllow) &&
           ((pStream->portData->clock->config.profile != ZL303XX_PTP_PROFILE_TELECOM_G8265_1) ||
            /* Telecom Profile: Check conditions specific to this profile. */
            (zl303xx_PtpTelecomUmtEntryQualified(pStream)))))
   {
      return ZL303XX_TRUE;
   }

   return ZL303XX_FALSE;
}
