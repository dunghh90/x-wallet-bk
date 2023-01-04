

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This header file contains the data types required to support the G.8265.1
*     standard (Telecom Profile).
*
*******************************************************************************/

#ifndef ZL303XX_PTP_TELECOM_TYPES_H_
#define ZL303XX_PTP_TELECOM_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/** Various Quality Level values listed in G.781, Annex A. */
typedef enum zl303xx_PtpG781QualityLevelE
{
   ZL303XX_G781_QL_UNDEF = 0,

   /* Option I */
   ZL303XX_G781_QL_PRC,
   ZL303XX_G781_QL_SSU_A,
   ZL303XX_G781_QL_SSU_B,
   ZL303XX_G781_QL_SEC,   /* QL-SEC is also in Option III. */
   ZL303XX_G781_QL_EEC1 = ZL303XX_G781_QL_SEC,
   ZL303XX_G781_QL_DNU,

   /* Option II */
   ZL303XX_G781_QL_PRS,
   ZL303XX_G781_QL_STU,
   ZL303XX_G781_QL_ST2,
   ZL303XX_G781_QL_TNC,
   ZL303XX_G781_QL_ST3E,
   ZL303XX_G781_QL_ST3,
   ZL303XX_G781_QL_EEC2 = ZL303XX_G781_QL_ST3,
   ZL303XX_G781_QL_SMC,
   ZL303XX_G781_QL_ST4,
   ZL303XX_G781_QL_PROV,
   ZL303XX_G781_QL_DUS,

   /* Option III */
   ZL303XX_G781_QL_UNK,

   /* Option I, II and III */
   ZL303XX_G781_QL_INV0,
   ZL303XX_G781_QL_INV1,
   ZL303XX_G781_QL_INV2,
   ZL303XX_G781_QL_INV3,
   ZL303XX_G781_QL_INV4,
   ZL303XX_G781_QL_INV5,
   ZL303XX_G781_QL_INV6,
   ZL303XX_G781_QL_INV7,
   ZL303XX_G781_QL_INV8,
   ZL303XX_G781_QL_INV9,
   ZL303XX_G781_QL_INV10,
   ZL303XX_G781_QL_INV11,
   ZL303XX_G781_QL_INV12,
   ZL303XX_G781_QL_INV13,
   ZL303XX_G781_QL_INV14,
   ZL303XX_G781_QL_INV15,
   ZL303XX_G781_QL_FAILED,

   ZL303XX_G781_QL_NUM_VALUES
} zl303xx_PtpG781QualityLevelE;

/*****************   DATA STRUCTURES   ****************************************/

/** Structure to store the conversion between various Quality Levels. */
typedef struct zl303xx_PtpTelecomQualityLevelS
{
   /** SSM Quality Level converted from clockClass. Will be 0xFF if clockClass
    *  cannot be converted to a QL. */
   Uint8T ssm;

   /** QL values listed in G.781. Each SSM QL maps to G.781 QL values for
    * Option I, II, and III synchronization networks. ZL303XX_G781_QL_UNDEF is
    * used to represent no mapping from SSM QL. */
   zl303xx_PtpG781QualityLevelE g781[3];
} zl303xx_PtpTelecomQualityLevelS;

/** Extensions to the zl303xx_PtpClockCreateS structure. */
typedef struct zl303xx_PtpTelecomClockCreateS
{
   /** Wait to restore time prevents a master from being selected by the BMCA.
    *  When a master fails and is later requalified, this timer starts. Set to 0
    *  to disable this functionality. */
   Uint32T waitToRestoreSec;

   /** If no masters are qualified, this determines how long the previous QL
    *  value will be retained before reporting QL-DNU/QL-DUS. */
   Uint32T qlHoldOffSec;

   /** Determines if the ZL303XX_PTP_EVENT_SQUELCH is generated. Should not be set
    *  to ZL303XX_TRUE if qlHoldOffSec is greater than 0 (a warning message will
    *  be logged). */
   zl303xx_BooleanE evtSquelchEn;
} zl303xx_PtpTelecomClockCreateS;

/** Telecom Profile extensions to the zl303xx_PtpUmtEntryS structure. */
typedef struct zl303xx_PtpTelecomUmtEntryS
{
   /** Local priority value for this master. */
   Uint8T  priority;
   /** Minimum time to declare PTSF-lossSync, in seconds. Set to 0 to disable. */
   Uint8T  syncReceiptTimeoutSec;
   /** Minimum time to declare PTSF-lossFollowUp, in seconds. Set to 0 to disable. */
   Uint8T  followUpReceiptTimeoutSec;
   /** Minimum time to declare PTSF-lossSyncOrFollowUp, in seconds. Set to 0 to disable. */
   Uint8T  syncOrFollowUpReceiptTimeoutSec;
   /** Minimum time to declare PTSF-lossDelayResp, in seconds. Set to 0 to disable. */
   Uint8T  delayRespReceiptTimeoutSec;
   /** Minimum time to declare PTSF-lossPdelayResp, in seconds. Set to 0 to disable. */
   Uint8T  pdelayRespReceiptTimeoutSec;
   /** Minimum time to declare PTSF-lossPdelayFollow, in seconds. Set to 0 to disable. */
   Uint8T  pdelayFollowUpReceiptTimeoutSec;

   /**  (Deprecated) Any value in this member is ignored. Masks for PTSF flags are
    *  now set using zl303xx_PtsfMaskSet(). */
   Sint32T  ptsfMask;
} zl303xx_PtpTelecomUmtEntryS;

/* This structure is used to implement the Telecom Profile extensions to Unicast
 * Discovery. These are extra members for each entry in a port's Unicast Master
 * Table. */
typedef struct
{
   Uint8T  priority;                /* Local priority value for this master. */
   const zl303xx_PtpTelecomQualityLevelS *pQuality; /* Various Quality Levels */
   Uint8T  clockClass;
   Uint16T stepsRemoved;

   Uint8T  syncReceiptTimeoutSec;   /* Time to declare PTSF-lossSync, in seconds. */
   Uint32T syncPrevCount;           /* Previous count of received Sync messages. */
   Uint8T  syncTimeoutCounter;      /* Countdown timer. */

   Uint8T  followReceiptTimeoutSec; /* Time to declare PTSF-lossFollowUp, in seconds. */
   Uint32T followPrevCount;         /* Previous count of received Follow_Up messages. */
   Uint8T  followTimeoutCounter;    /* Countdown timer. */

   Uint8T  sofReceiptTimeoutSec;    /* Time to declare PTSF-lossSyncOrFollowUp, in seconds. */
   Uint8T  sofTimeoutCounter;       /* Countdown timer. */

   Uint8T  delayReceiptTimeoutSec;  /* Time to declare PTSF-lossDelayResp, in seconds. */
   Uint32T delayPrevCount;          /* Previous count of received Delay_Resp messages. */
   Uint8T  delayTimeoutCounter;     /* Countdown timer. */

   Uint8T  pdelayReceiptTimeoutSec; /* Time to declare PTSF-lossPdelayResp, in seconds. */
   Uint32T pdelayPrevCount;         /* Previous count of received Peer_Delay_Resp messages. */
   Uint8T  pdelayTimeoutCounter;    /* Countdown timer. */

   Uint8T  pdelayFollowReceiptTimeoutSec; /* Time to declare PTSF-lossPdelayResp, in seconds. */
   Uint32T pdelayFollowPrevCount;         /* Previous count of received Peer_Delay_FollowUp messages. */
   Uint8T  pdelayFollowTimeoutCounter;    /* Countdown timer. */

   Uint8T  unusableTimeoutSec;      /* Time to declare PTSF-unusable, in seconds. */
   Uint8T  unusableTimeoutCounter;  /* Countdown timer. */

   Uint32T        wtr;              /* Wait to restore countdown timer. */
   zl303xx_BooleanE wtrActive;        /* WTR timer active flag. */
   zl303xx_BooleanE bestMasterOnce;   /* Don't use the WTR timer if never previously best master. */
} zl303xx_PtpTelecomUmtEntryIntS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */

