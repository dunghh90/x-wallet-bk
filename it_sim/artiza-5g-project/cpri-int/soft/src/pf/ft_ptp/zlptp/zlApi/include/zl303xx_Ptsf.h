

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains the packet timing signal fail (PTSF) API.
*
*     A PTSF flag represents a specific type of failure for each timing stream used
*     to perform clock recovery. Each PTSF flag can be individually masked, and
*     flags may be used to generate higher-level flags. The following diagram
*     represents the entire PTSF tree:
*
*     Level 1
*     =======
*     PTSF-user ------------------> & mask ---> |
*     PTSF-QLTooLow --------------> & mask ---> |
*     PTSF-LOSFromPHY ------------> & mask ---> | ---> PTSF-misc
*     PTSF-waitToRestoreActive ---> & mask ---> |
*
*     PTSF-retryAttemptsFail -----> & mask ---> |
*     PTSF-announceRejected ------> & mask ---> |
*     PTSF-syncRejected ----------> & mask ---> |
*     PTSF-delayRespRejected -----> & mask ---> | ---> PTSF-contract
*     PTSF-pdelayRespRejected ----> & mask ---> |
*     PTSF-announceRateTooLow ----> & mask ---> |
*     PTSF-syncRateTooLow --------> & mask ---> |
*     PTSF-delayRespRateTooLow ---> & mask ---> |
*     PTSF-pdelayRespRateTooLow --> & mask ---> |
*
*     PTSF-lossAnnounce ----------> & mask ---> |
*     PTSF-lossSync --------------> & mask ---> |
*     PTSF-lossFollowUp ----------> & mask ---> | ---> PTSF-timeOut
*     PTSF-lossSyncOrFollowUp ----> & mask ---> |
*     PTSF-lossDelayResp ---------> & mask ---> |
*     PTSF-lossPdelayResp --------> & mask ---> |
*     PTSF-lossPdelayFollowUp ----> & mask ---> |
*
*     PTSF-packetRejectReset --------------> & mask ---> |
*     PTSF-lossTimeStamps -----------------> & mask ---> | ---> PTSF-unusable
*     PTSF-localTimeInaccuracyExceeded ----> & mask ---> |
*     PTSF-networkTimeInaccuracyExceeded --> & mask ---> |
*
*     PTSF-pullInRangeOOR --------> & mask ---> |
*     PTSF-PDVFreqMetricFail -----> & mask ---> |
*     PTSF-freqVarTooHigh --------> & mask ---> | ---> PTSF-frequency
*     PTSF-notFreqLock -----------> & mask ---> |
*
*     PTSF-pullInRangeOOR --------> & mask ---> |
*     PTSF-PDVPhaseMetricFail ----> & mask ---> |
*     PTSF-phaseVarTooHigh -------> & mask ---> | ---> PTSF-phase
*     PTSF-notPhaseLock ----------> & mask ---> |
*     PTSF-frequency -------------> & mask ---> |
*
*     PTSF-pDelayMultipleResponsesRecieved --> & mask --> |
*     PTSF-PDelayAboveThreshold -------------> & mask --> | PTSF-asCapable
*     PTSF-pDelayAllowedLostResponses -------> & mask --> |
*
*     Level 2
*     =======
*     PTSF-misc --------> & mask ---> |
*     PTSF-contract ----> & mask ---> |
*     PTSF-timeout -----> & mask ---> |
*     PTSF-unusable ----> & mask ---> | ---> PSTF (overall)
*     PTSF-frequency ---> & mask ---> |
*     PTSF-phase -------> & mask ---> |
*     PTSF-asCapable ---> & mask ---> |
*
*     The following diagram shows how PTSF data can accessed or modified at each
*     point in the tree:
*
*         +--- zl303xx_PtsfFlagSet()
*         |                          +--- zl303xx_PtsfMaskSet()
*         |                          |
*         v                          v
*     PTSF-example --------+---> & mask -----+--->
*                          |         |       |
*                          |         |       +--- zl303xx_PtsfFlagGetAndMask() + callback
*                          |         +--- zl303xx_PtsfMaskGet()
*                          +--- zl303xx_PtsfFlagGet()
*
*     A callback function can be registered using zl303xx_PtsfCallbackSet(). This
*     function will be executed when the value of the PTSF flag changes after
*     the mask has been applied.
*
*     Only the following flags should be modified by the user. All other flags
*     are set automatically by other API modules.
*        PTSF-user
*        PTSF-LOSFromPHY
*        PTSF-PDVFreqMetricFail
*        PTSF-freqVarTooHigh
*        PTSF-PDVPhaseMetricFail
*        PTSF-phaseVarTooHigh
*
*  Trace information (ZL303XX_MOD_ID_PTSF):
*     ALWAYS: Initialization errors
*          1: Run-time errors (in getter/setter functions)
*          2: (reserved)
*          3: PTSF flag changes (after mask applied) & mask changes
*          4: User callback executed
*          5: PTSF flag changes (before mask applied)
*
*******************************************************************************/

#ifndef ZL303XX_PTSF_H_
#define ZL303XX_PTSF_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"

/*****************   DEFINES   ************************************************/
#define ZL303XX_PTSF_DEFAULT_MAX_STREAMS   128

/*****************   DATA TYPES   *********************************************/

/** Enum of all possible PTSF flags. */
typedef enum zl303xx_PtsfFlagE
{
   /* misc */
   ZL303XX_PTSF_USER = 0,
   ZL303XX_PTSF_QL_TOO_LOW,
   ZL303XX_PTSF_LOS_FROM_PHY,
   ZL303XX_PTSF_WAIT_TO_RESTORE_ACTIVE,

   /* contract */
   ZL303XX_PTSF_RETRY_ATTEMPTS_FAIL,
   ZL303XX_PTSF_ANNOUNCE_REJECTED,
   ZL303XX_PTSF_SYNC_REJECTED,
   ZL303XX_PTSF_DELAY_RESP_REJECTED,
   ZL303XX_PTSF_PEER_DELAY_RESP_REJECTED,
   ZL303XX_PTSF_ANNOUNCE_RATE_TOO_LOW,
   ZL303XX_PTSF_SYNC_RATE_TOO_LOW,
   ZL303XX_PTSF_DELAY_RESP_RATE_TOO_LOW,
   ZL303XX_PTSF_PEER_DELAY_RESP_RATE_TOO_LOW,

   /* timeout */
   ZL303XX_PTSF_LOSS_ANNOUNCE,
   ZL303XX_PTSF_LOSS_SYNC,
   ZL303XX_PTSF_LOSS_FOLLOW_UP,
   ZL303XX_PTSF_LOSS_SYNC_OR_FOLLOW_UP,
   ZL303XX_PTSF_LOSS_DELAY_RESP,
   ZL303XX_PTSF_LOSS_PEER_DELAY_RESP,
   ZL303XX_PTSF_LOSS_PEER_DELAY_FOLLOW_UP,

   /* unusable */
   ZL303XX_PTSF_PACKET_REJECT_RESET,
   ZL303XX_PTSF_LOSS_TIME_STAMPS,
   ZL303XX_PTSF_LOCAL_TIME_INACC_EXCEEDED,
   ZL303XX_PTSF_NETWORK_TIME_INACC_EXCEEDED,

   /* frequency */
   ZL303XX_PTSF_PULL_IN_RANGE_OOR,
   ZL303XX_PTSF_PDV_FREQ_METRIC_FAIL,
   ZL303XX_PTSF_FREQ_VAR_TOO_HIGH,
   ZL303XX_PTSF_NOT_FREQ_LOCK,

   /* phase */
   ZL303XX_PTSF_PDV_PHASE_METRIC_FAIL,
   ZL303XX_PTSF_PHASE_VAR_TOO_HIGH,
   ZL303XX_PTSF_NOT_PHASE_LOCK,

   ZL303XX_PTSF_MISC,
   ZL303XX_PTSF_CONTRACT,
   ZL303XX_PTSF_TIMEOUT,
   ZL303XX_PTSF_UNUSABLE,
   ZL303XX_PTSF_FREQUENCY,
   ZL303XX_PTSF_PHASE,
   ZL303XX_PTSF_MAIN,

   /* As-Capable */
   ZL303XX_PTSF_PEER_DELAY_MULTI_RESP,
   ZL303XX_PTSF_PEER_DELAY_ABOVE_THRESHOLD,
   ZL303XX_PTSF_PEER_DELAY_ALLOWED_LOST_RESPONSE,
   ZL303XX_PTSF_AS_CAPABLE,


   /* Must always be the last member. */
   ZL303XX_PTSF_NUM_FLAGS
} zl303xx_PtsfFlagE;

typedef enum {
    UP_PTSF_FALSE = 0,
    DOWN_PTSF_TRUE,
    PTFS_UNKNOWN
} zl303xx_UpDownPtsfE;

/*****************   DATA STRUCTURES   ****************************************/

/** Parameters used to initialize the PTSF module. */
typedef struct zl303xx_PtsfConfigS
{
   /** Maximum number of streams (APR/PTP) that can be created on the system. */
   Uint32T  maxStreams;
} zl303xx_PtsfConfigS;

/** Data passed to a callback function when a PTSF flag changes state. */
typedef struct zl303xx_PtsfEventS
{
   /** Handle of the stream this flag changed on. */
   Uint32T  streamHandle;
   /** The PTSF flag that changed. */
   zl303xx_PtsfFlagE  flag;
   /** The new value of the PTSF flag with mask applied. */
   zl303xx_BooleanE  value;
} zl303xx_PtsfEventS;

/* zl303xx_PtsfCallbackT */
/**
   Type definition of a PTSF callback function. A callback can be registered
   with the PTSF module using zl303xx_PtsfCallbackSet(). It will be executed every
   time the PTSF flag after the mask has been applied changes state.

  Parameters:
   [in]   pEvent  PTSF change event data.

  Return Value:  N/A

*******************************************************************************/
typedef void (*zl303xx_PtsfCallbackT)(zl303xx_PtsfEventS *pEvent);

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/
zlStatusE zl303xx_PtsfConfigStructInit(zl303xx_PtsfConfigS *pConfig);
zlStatusE zl303xx_PtsfInit(zl303xx_PtsfConfigS *pConfig);
zlStatusE zl303xx_PtsfClose(void);

void zl303xx_TSAPtsfFlagSet(Sint32T aprServerNum, zl303xx_PtsfFlagE flag, zl303xx_BooleanE value);
void zl303xx_PtsfFlagSet(Uint32T streamHandle, zl303xx_PtsfFlagE flag, zl303xx_BooleanE value);
void zl303xx_PtsfFlagSetAll(Uint32T streamHandle, zl303xx_BooleanE value);
zl303xx_BooleanE zl303xx_PtsfFlagGet(Uint32T streamHandle, zl303xx_PtsfFlagE flag);
zl303xx_BooleanE zl303xx_PtsfFlagGetAndMask(Uint32T streamHandle, zl303xx_PtsfFlagE flag);

void zl303xx_PtsfMaskSet(Uint32T streamHandle, zl303xx_PtsfFlagE flag, Uint32T mask);
void zl303xx_PtsfMaskSetAll(Uint32T streamHandle, Uint32T mask);
Uint32T zl303xx_PtsfMaskGet(Uint32T streamHandle, zl303xx_PtsfFlagE flag);

zlStatusE zl303xx_PtsfCallbackSet(zl303xx_PtsfFlagE flag, zl303xx_PtsfCallbackT callback);

const char *zl303xx_PtsfFlagToStr(zl303xx_PtsfFlagE flag);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
