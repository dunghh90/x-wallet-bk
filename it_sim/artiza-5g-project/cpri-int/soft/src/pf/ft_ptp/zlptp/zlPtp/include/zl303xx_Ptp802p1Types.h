

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This header file contains the data types required to support the
*     IEEE802.1AS Standard (TSN Profile).
*
*******************************************************************************/
#ifndef ZL303XX_PTP_802_P1_TYPES_H_
#define ZL303XX_PTP_802_P1_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_PtpStdTypes.h"

/*****************   DEFINES   ************************************************/

#define ZL303XX_PTP_802P1_LOG_ANNC_DEF     (Sint8T)(0)   /* 1 pps  (default) */
#define ZL303XX_PTP_802P1_LOG_SYNC_DEF     (Sint8T)(-3)  /* 8 pps  (default) */
#define ZL303XX_PTP_802P1_LOG_PDELAY_DEF   (Sint8T)(0)   /* 1 pps  (default) */
#define ZL303XX_PTP_802P1_ANNC_TIMEOUT_DEF (3)

/* Special Interval types for MESSAGE INTERVAL REQ TLV */
#define ZL303XX_PTP_802P1_LOG_INTVL_NO_CHANGE (Sint8T)(-128) /* Do not change the interval*/
#define ZL303XX_PTP_802P1_LOG_INTVL_RESET     (Sint8T)(126)  /* Reset the interval value to default*/

/* Default Settings for some specific 2011 profile parameters. */
#define ZL303XX_PTP_802P1_DOMAIN_NUM_DEF       0
#define ZL303XX_PTP_802P1_CLOCK_PRI2_DEF       248
#define ZL303XX_PTP_802P1_CLOCK_PRI1_NITS      246 /* Network infrastructure time-aware system */
#define ZL303XX_PTP_802P1_CLOCK_PRI1_PORTABLE  250 /* Portable time-aware system */
#define ZL303XX_PTP_802P1_CLOCK_PRI1_OTHER     248 /* Other time-aware systems */
#define ZL303XX_PTP_802P1_CLOCK_PRI1_NO_GM     255 /* Not Grandmaster-capable */

/* Default Clock Class values */
#define ZL303XX_PTP_802P1_GM_CAPABLE_CLASS        248
#define ZL303XX_PTP_802P1_NOT_GM_CAPABLE_CLASS    PTP_SLAVE_ONLY_CLOCK_CLASS  /* 255 */

/* Default Clock Accuracy */
#define ZL303XX_PTP_802P1_ACCURACY_DEF    ZL303XX_CLOCK_ACCURACY_UNKNOWN    /* 0xFE */

/* Default Clock Variance */
#define ZL303XX_PTP_802P1_VARIANCE_DEF      0x4100
#define ZL303XX_PTP_802P1_LOST_RESP_DEF     3

/* Masks for Message Intvl Req TLV Flags */
#define ZL303XX_PTP_802P1_COMPUTE_RATE_RATIO_FLAG  (0x01)
#define ZL303XX_PTP_802P1_COMPUTE_PROP_DELAY_FLAG  (ZL303XX_PTP_802P1_COMPUTE_RATE_RATIO_FLAG << 1)
/*****************   DATA STRUCTURES   ****************************************/

/** Profile specific TLV to communicate Message Intervals.
 *  (Attached to Signaling message). */
typedef struct zl303xx_Ptp802p1MsgIntvlReqTlvS
{
   /*The value is the logarithm to base 2 of the mean time interval,
    * desired by the sending port between successive Pdelay_Req messages */
   Sint8T linkDelayIntvl;
   /* The value is the logarithm to base 2 of the mean time interval,
    * desired by the sending port between successive time-synchronization event messages */
   Sint8T timeSyncIntvl;
   /*The value is the logarithm to base 2 of the mean time interval,
    * by the sending port between successive Announce messages. */
   Sint8T announceIntvl;
   /*Definitions of bits of flags field of message interval request TLV
   Bit      Name
   1        computeNeighborRateRatio
   2        computeNeighborPropDelay */
   Uint8T flags;

   Uint16T reserved;
} zl303xx_Ptp802p1MsgIntvlReqTlvS;

/** Profile specific clock data required for proper profile operation. */
typedef struct zl303xx_Ptp802p1ClockConfigS
{
   /** A configurable member which indicates if the time-aware system
    * is capable of being a grandmaster and dictates the value of clockClass
    * in defaultDs.If the value is TRUE the time-aware system is capable
    * of being a grandmaster and if FALSE the timeaware
    * system is not capable of being a grandmaster.
    * Unused for other profiles */
   zl303xx_BooleanE gmCapable;
} zl303xx_Ptp802p1ClockConfigS;

/** Profile specific clock data learned or dynamically maintained during
 *  profile operation. */
typedef struct zl303xx_Ptp802p1ClockDS
{
   /* A dynamic member which represents the phase change (i.e., change in sourceTime)
    * that occurred on the most recent change in timeBaseIndicator*/
   ScaledNs96T lastGmPhaseChange;

   /* The fractional frequency offset of the current grandmaster relative to the previous
      grandmaster, at the time that the current grandmaster became grandmaster,
      or relative to itself prior to the last change in gmTimeBaseIndicator. */
   Sint64T lastGmFreqChange;

   /* The timeBaseIndicator of the current grandmaster. */
   Uint16T gmTimeBaseIndicator;

   /* The measured ratio of the frequency of the grandmaster to the frequency of
   the LocalClock. */
   Uint64T gmRateRatio;

   /* Dynamic member to indicate that gmRateRatio is 1.
    * APR enabled then TRUE, APR disabled then FALSE
    */
   zl303xx_BooleanE syntonizedPll;
} zl303xx_Ptp802p1ClockDS;

/** Profile specific port data required for proper profile operation. */
typedef struct zl303xx_Ptp802p1PortConfigS
{

   /** A configurable member defining the Message Interval attributes
    *  associated with this local PTP port. This data is used to issue the
    *  Message Interval Request TLV of the IEEE802.1AS profile. Unused for other
    *  profiles.   */
   zl303xx_Ptp802p1MsgIntvlReqTlvS msgIntvlReq;
} zl303xx_Ptp802p1PortConfigS;

/** Profile specific stream data learned or dynamically maintained during
 *  profile operation. */
typedef struct zl303xx_Ptp802p1StreamDS
{
   /** A dynamic member defining the Message Interval attributes
    *  associated with the remote PTP port that is issuing timing messages to
    *  this stream (via the Message Interval Request TLV of the IEEE802.1AS
    *  profile). It is used to set announce, sync and delay request intervals. */
   zl303xx_Ptp802p1MsgIntvlReqTlvS msgIntvlReq;
} zl303xx_Ptp802p1StreamDS;

#ifdef __cplusplus
}
#endif
#endif /* ZL303XX_PTP_802_P1_TYPES_H_ */
