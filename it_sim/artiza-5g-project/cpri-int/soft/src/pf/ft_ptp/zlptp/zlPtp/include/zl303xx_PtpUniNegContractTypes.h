

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This header file contains the data types and functions required to support
*     general Unicast Negotiation contracts.
*
*******************************************************************************/

#ifndef ZL303XX_PTP_UNI_NEG_CONTRACT_TYPES_H_
#define ZL303XX_PTP_UNI_NEG_CONTRACT_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Error.h"

#include "zl303xx_PtpStdTypes.h"

/*****************   DEFINES   ************************************************/

/* Unicast negotiation retry attempts. */
#define ZL303XX_PTP_TELECOM_UN_RETRIES        (Uint8T)(3)
#define ZL303XX_PTP_TELECOM_UN_LOCKOUT_SEC    (Uint32T)(60)

/*****************   DATA TYPES   *********************************************/

/** Used to reference unicast negotiation RX and TX contracts. A stream may
 *  have both active at the same time. */
typedef enum zl303xx_PtpContractFlowE
{
   ZL303XX_PTP_CONTRACT_RX,
   ZL303XX_PTP_CONTRACT_TX,
   ZL303XX_PTP_CONTRACT_NUM_TYPES
} zl303xx_PtpContractFlowE;

typedef enum zl303xx_PtpContractRxStateE
{
   ZL303XX_PTP_CONTRACT_RX_IDLE,
   ZL303XX_PTP_CONTRACT_RX_REQUESTING,
   ZL303XX_PTP_CONTRACT_RX_ACTIVE,
   ZL303XX_PTP_CONTRACT_RX_CANCELLING
} zl303xx_PtpContractRxStateE;

typedef enum zl303xx_PtpContractTxStateE
{
   ZL303XX_PTP_CONTRACT_TX_IDLE,
   ZL303XX_PTP_CONTRACT_TX_ACTIVE,
   ZL303XX_PTP_CONTRACT_TX_CANCELLING
} zl303xx_PtpContractTxStateE;

/*****************   DATA STRUCTURES   ****************************************/

/** Parameters for individual unicast negotiation contract requests. */
typedef struct zl303xx_PtpContractS
{
   /** The log2 of the inter-message interval. */
   Sint8T  interval;
   /** Duration of the contract, in seconds. */
   Uint32T durationSec;
} zl303xx_PtpContractS;

/** Status of an individual unicast negotiation contract. */
typedef struct zl303xx_PtpContractStatusS
{
   /** The log2 of the inter-message interval. */
   Sint8T logMessageInterval;
   /** Duration of the contract, in seconds. */
   Uint32T durationSec;
   /** Remaining time of the contract, in seconds. */
   Uint32T remainingSec;
   /** Port address the contract has been negotiated with. */
   zl303xx_PortAddress farEndPortAddr;
   /** Next sequenceId to be transmitted, or expected to be received. */
   Uint16T nextSequenceId;
} zl303xx_PtpContractStatusS;

/* Used to store unicast negotiation limits per message type */
typedef struct zl303xx_PtpContractLimitsS
{
   Sint8T  minIntvl;        /* Min log2 interval (i.e., max message rate) */
   Uint32T maxDurationSec;  /* Max duration, in seconds */
   Uint32T maxCount;        /* Max number of contracts that can be granted */
} zl303xx_PtpContractLimitsS;

/** Contract Lockout status parameters for the clock stream. */
typedef struct
{
   /** Number of seconds this stream will remain locked out (after this the
    *  stream will retry to request service from the master). */
   Uint32T remainingSec;

   /** Number of times this server has already been locked out. */
   Uint32T previousCount;
} zl303xx_PtpContractLockoutStatusS;

/** Retry statistics & counters for a single failure type (per messageType). */
typedef struct
{
   /** Number of remaining times a unicast REQUEST can be retried before
    *  the master is locked out. */
   Uint8T remainingRequests;

   /** Flag is set if the maximum number of retries for this failure type
    *  id reached. */
   zl303xx_BooleanE failed;
   /** Flag is set if a unicast REQUEST attempt has previously failed for this
    *  failure type. */
   zl303xx_BooleanE prevFailed;

   /** Number of times this Request has been denied (per failure type). */
   Uint32T count;
} zl303xx_PtpContractRetryStatsS;

/** Retry status of an individual unicast negotiation contract (per messageType). */
typedef struct
{
   /* Retry status for DENIAL of service failures. */
   zl303xx_PtpContractRetryStatsS denial;

   /* Retry status for Timeout of request failures. */
   zl303xx_PtpContractRetryStatsS timeout;

   /* Indicates that the last request failed due to a Denial; required so that
    * both timers do no expire.   */
   zl303xx_BooleanE lastGrantDenied;
} zl303xx_PtpContractRetryStatusS;

/* Contract Retry Configuration */
typedef struct
{
   /* Number of times to retry at the logQueryInterval.
    * Default = 0xFFFFFFFF */
   Uint32T logQueryCount;
   /* Lockout period before retrying again.
    * Default = 0 */
   Uint32T lockoutSec;
} zl303xx_PtpContractRetryParamS;

/** The retry configuration when a negotiation attempt is DENIED or a TIMEOUT
 *  occurs (each configured independently). A client sends 'logQueryCount'
 *  retries before it puts the request into a 'lockout' for some number of
 *  seconds. After the lockout expires, a single retry will occur before another
 *  lockout of the same period. */
typedef struct
{
   /* Configuration for DENIAL of Contract. */
   zl303xx_PtpContractRetryParamS denial;

   /* Configuration for TIMEOUT of Contract. */
   zl303xx_PtpContractRetryParamS timeout;
} zl303xx_PtpContractRetryConfigS;

/* Request Contract Data. */
typedef struct
{
   zl303xx_BooleanE negotiate;          /* Request a contract for this message type */
   zl303xx_PtpContractRxStateE  state;  /* Message flow state */
   zl303xx_PtpContractS         req;    /* Local request values */
   zl303xx_PtpContractS         grant;  /* Far-end grant values */

   Uint32T remainingSec;              /* Time remaining in contract (seconds) */
   Uint32T renewSec;                  /* Time before expiry that renewal(s) will be sent (seconds) */
   Uint16T remainingCancelMsgs;       /* Used in CANCELLING state before forcing to IDLE */
   zl303xx_BooleanE renegotiate;        /* Request new contract after dynamic parameter change */
   zl303xx_PtpContractRetryStatusS retry;   /* Contract Retry specific data */
} zl303xx_PtpContractRxS;

/* Granted Contract Data. */
typedef struct
{
   zl303xx_PtpContractTxStateE  state;  /* Message flow state */
   zl303xx_PtpContractS         req;    /* Far-end request values */
   zl303xx_PtpContractS         grant;  /* Local grant values */

   Uint32T remainingSec;              /* Time remaining in contract (seconds) */
   Uint16T remainingCancelMsgs;       /* Used in CANCELLING state before forcing to IDLE */
} zl303xx_PtpContractTxS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   INTERNAL PTP FUNCTION DECLARATIONS   *********************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

void zl303xx_PtpContractLockoutStatusInit(
      zl303xx_PtpContractLockoutStatusS *pStreamLockout);
void zl303xx_PtpContractMsgRetryStatusInit(
      zl303xx_PtpContractRetryStatusS *pMsgRetry,
      zl303xx_PtpContractRetryConfigS *pStreamRetryCfg);

/* Unicast Negotiation state machine functions. */
void zl303xx_PtpContractRequestUnicastTx(
      zl303xx_PtpContractLockoutStatusS *pStreamLockout,
      zl303xx_PtpContractRetryStatusS *pMsgRetry,
      zl303xx_PtpContractRetryConfigS *pStreamRetryCfg,
      Uint32T streamHandle,
      const char * msgTypeStr);
void zl303xx_PtpContracRequestDenied(
      zl303xx_PtpContractLockoutStatusS *pStreamLockout,
      zl303xx_PtpContractRetryStatusS *pMsgRetry,
      zl303xx_PtpContractRetryConfigS *pStreamRetryCfg,
      Uint32T streamHandle,
      const char * msgTypeStr);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
