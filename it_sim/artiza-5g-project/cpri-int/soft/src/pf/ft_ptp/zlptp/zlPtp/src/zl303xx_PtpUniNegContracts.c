

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Contains functions required to implement and manage Unicast Negotiation
*     Contracts.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"
#include "zl303xx_Error.h"

#include "zl303xx_PtpUniNegContractTypes.h"
#include "zl303xx_Ptsf.h"

/*
#include "zl303xx_Ptpd.h"

#include "zl303xx_PtpG8265p1Api.h"
#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpFunctionExport.h" */

/*****************   DEFINES     **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

static void zl303xx_PtpContractLockoutStart(
      zl303xx_PtpContractLockoutStatusS *pStreamLockout,
      zl303xx_PtpContractRetryStatusS *pMsgRetry,
      zl303xx_PtpContractRetryConfigS *pStreamRetryCfg,
      Uint32T streamHandle,
      const char * msgTypeStr);

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   PROFILE BINDING FUNCTION DECLARATIONS (END) **************/


/* zl303xx_PtpContractLockoutStatusInit */
/** 

   Initializes a zl303xx_PtpContractLockoutStatusS structure by clearing the
   remainingSec counter to 0 (it does not reset the statistics counters.
   This should be done at stream creation (unless CALLOC is used) and after a
   contract is successfully negotiated.

  Parameters:
   [out] pStreamLockout    Pointer to the Lockout status member of the
                                    stream structure to initialize.

*******************************************************************************/
void zl303xx_PtpContractLockoutStatusInit(
      zl303xx_PtpContractLockoutStatusS *pStreamLockout)
{
   pStreamLockout->remainingSec = 0;
}

/* zl303xx_PtpContractMsgRetryStatusInit */
/** 

   Initializes a zl303xx_PtpContractRetryStatusS structure, but does not reset
   the statistics counters (grant denied counter and timeout counter).

  Parameters:
   [out]    pMsgRetry   Message Retry Status structure to initialize.

*******************************************************************************/
void zl303xx_PtpContractMsgRetryStatusInit(
      zl303xx_PtpContractRetryStatusS *pMsgRetry,
      zl303xx_PtpContractRetryConfigS *pStreamRetryCfg)
{
   /* Denial */
   pMsgRetry->denial.prevFailed = pMsgRetry->denial.prevFailed || pMsgRetry->denial.failed;
   pMsgRetry->denial.failed = ZL303XX_FALSE;
   pMsgRetry->denial.remainingRequests = (pMsgRetry->denial.prevFailed == ZL303XX_TRUE)
                                             ? 1
                                             : pStreamRetryCfg->denial.logQueryCount;
   pMsgRetry->lastGrantDenied = ZL303XX_FALSE;
   
   /* Timeout */
   pMsgRetry->timeout.prevFailed = pMsgRetry->timeout.prevFailed || pMsgRetry->timeout.failed;
   pMsgRetry->timeout.failed = ZL303XX_FALSE;
   pMsgRetry->timeout.remainingRequests = (pMsgRetry->timeout.prevFailed == ZL303XX_TRUE)
                                             ? 2
                                             : pStreamRetryCfg->timeout.logQueryCount;
}

/* zl303xx_PtpContractRequestUnicastTx */
/** 

   Function to be called when the Unicast state machine attempts to start a
   message flow. Determines if a message type has exceeded the maximum number of
   attempts to negotiate a contract. If it has, the lockout timer for this
   master is started.

  Parameters:
   [in,out] pStreamLockout    Pointer to the Lockout status member of the
                                    stream structure.
   [in]     pMsgRetry         Pointer to the Message Retry Status structure.
   [in]     streamHandle      Handle to a previously created stream.
   [in]     msgTypeStr        Unicast message type string.

*******************************************************************************/
void zl303xx_PtpContractRequestUnicastTx(
      zl303xx_PtpContractLockoutStatusS *pStreamLockout,
      zl303xx_PtpContractRetryStatusS *pMsgRetry,
      zl303xx_PtpContractRetryConfigS *pStreamRetryCfg,
      Uint32T streamHandle,
      const char * msgTypeStr)
{
   if (pStreamRetryCfg->timeout.logQueryCount != (Uint32T)(-1))
   {
      if (pMsgRetry->timeout.failed == ZL303XX_FALSE)
      {
         /* Either the Grant was Denied or experienced Timeout
          * The DENIAL counter is explicitly handled elsewhere.*/
         if (pMsgRetry->lastGrantDenied == ZL303XX_FALSE) /* TIMEOUT */
         {
            /* Increment the timeout counter if the first REQUEST has not been GRANTed
             * yet. The DENIAL counter is explicitly handled elsewhere. */
            if (pMsgRetry->timeout.remainingRequests < pStreamRetryCfg->timeout.logQueryCount)
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 2,
                             "Stream=%d: (TIMEOUT) REQUEST for %s contract timed out.",
                             streamHandle, msgTypeStr, 0,0,0,0);

               pMsgRetry->timeout.count++;
            }

            /* Decrement the message-specific failed request counter. */
            if (pMsgRetry->timeout.remainingRequests > 0)
            {
               pMsgRetry->timeout.remainingRequests--;

               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 2,
                        "Stream %u, uniNegMsgType %s: (TIMEOUT) request sent- %u requests remaining",
                        streamHandle,
                        msgTypeStr,
                        pMsgRetry->timeout.remainingRequests, 0,0,0);
            }
         }

         else  /* Grant was DENIED */
         {
            /* The DENIAL counter is explicitly handled elsewhere.*/
            pMsgRetry->lastGrantDenied = ZL303XX_FALSE;
         }

         if (pMsgRetry->timeout.remainingRequests == 0)
         {
            pMsgRetry->timeout.failed = ZL303XX_TRUE;
            zl303xx_PtpContractLockoutStart(pStreamLockout, pMsgRetry, pStreamRetryCfg, streamHandle, msgTypeStr);
         }
      }
   }
}

/* zl303xx_PtpContracRequestDenied */
/** 

   Function to be called when a unicast master denies a contract from a
   REQUEST_UNICAST_TRANSMISSION TLV. Determines if a message type has exceeded
   the maximum number of attempts to negotiate a contract. If it has, the lockout
   timer for this master is started.

  Parameters:
   [in,out] pStreamLockout    Pointer to the Lockout status member of the
                                    stream structure.
   [in]     pMsgRetry         Pointer to the Message Retry Status structure.
   [in]     streamHandle      Handle to a previously created stream.
   [in]     msgTypeStr        Unicast message type string.

*******************************************************************************/
void zl303xx_PtpContracRequestDenied(
      zl303xx_PtpContractLockoutStatusS *pStreamLockout,
      zl303xx_PtpContractRetryStatusS *pMsgRetry,
      zl303xx_PtpContractRetryConfigS *pStreamRetryCfg,
      Uint32T streamHandle,
      const char * msgTypeStr)
{
   if (pStreamRetryCfg->denial.logQueryCount != (Uint32T)(-1))
   {
      if (pMsgRetry->denial.failed == ZL303XX_FALSE)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 2,
                       "Stream=%d: (G.8265.1) REQUEST for %s contract DENIED.",
                       streamHandle, msgTypeStr, 0,0,0,0);

         /* Increment the grant denied counter. */
         pMsgRetry->denial.count++;
         pMsgRetry->lastGrantDenied = ZL303XX_TRUE;

         /* Decrement the message-specific failed request counter. */
         if (pMsgRetry->denial.remainingRequests > 0)
         {
            pMsgRetry->denial.remainingRequests--;

            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 2,
                     "Stream %u, uniNegMsgType %s: (DENIED) request sent- %u requests remaining",
                     streamHandle,
                     msgTypeStr,
                     pMsgRetry->denial.remainingRequests, 0,0,0);
         }

         /* Mark this message type as failed if there are no more retries left. */
         if (pMsgRetry->denial.remainingRequests == 0)
         {
            pMsgRetry->denial.failed = ZL303XX_TRUE;
            zl303xx_PtpContractLockoutStart(pStreamLockout, pMsgRetry, pStreamRetryCfg, streamHandle, msgTypeStr);
         }
      }
   }
}


/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/* zl303xx_PtpContractLockoutStart */
/** 

   Marks the message type as failed and starts the lockout counter.

  Parameters:
   [in,out] pStreamLockout    Pointer to the Lockout status member of the
                                    stream structure.
   [in]     pMsgRetry         Pointer to the Message Retry Status structure.
   [in]     streamHandle      Handle to a previously created stream.
   [in]     msgTypeStr        Unicast message type string.

*******************************************************************************/
static void zl303xx_PtpContractLockoutStart(
      zl303xx_PtpContractLockoutStatusS *pStreamLockout,
      zl303xx_PtpContractRetryStatusS *pMsgRetry,
      zl303xx_PtpContractRetryConfigS *pStreamRetryCfg,
      Uint32T streamHandle,
      const char * msgTypeStr)
{
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 1,
      "Stream %lu: REQUEST for %s contract failed too many times :Deny(%u):Timeout(%u)",
      streamHandle, msgTypeStr, pMsgRetry->denial.failed, pMsgRetry->timeout.failed, 0,0);

   if (pStreamLockout->remainingSec == 0)
   {
      /* Initialize the lockout counter. */
      if (pMsgRetry->denial.failed == ZL303XX_TRUE)
      {
         pStreamLockout->remainingSec = pStreamRetryCfg->denial.lockoutSec;
      }
      else
      {
         pStreamLockout->remainingSec = pStreamRetryCfg->timeout.lockoutSec;
      }

      zl303xx_PtsfFlagSet(streamHandle, ZL303XX_PTSF_RETRY_ATTEMPTS_FAIL, ZL303XX_TRUE);

      /* Increment the lockout stat. */
      pStreamLockout->previousCount++;

      /* Log info */
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_UNICAST, 1,
            "Stream %lu: Master locked out. No contract REQUESTs for %lu sec.",
            streamHandle, pStreamLockout->remainingSec, 0,0,0,0);
   }
}

/*****************   END   ****************************************************/
