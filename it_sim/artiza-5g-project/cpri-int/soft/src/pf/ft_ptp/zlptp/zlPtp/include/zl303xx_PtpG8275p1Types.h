

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This header file contains the data types required to support the G.8275.1
*     standard (Telecom Profile for Phase).
*
*******************************************************************************/

#ifndef ZL303XX_PTP_G8275_1_TYPES_H_
#define ZL303XX_PTP_G8275_1_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"

/*****************   DEFINES   ************************************************/

/* Packet rate ranges allowed by the Telecom Profile for Phase. */
/* G.8275.1 */
#define ZL303XX_PTP_G8275p1_PPS_SYNC_DEF       (Sint8T)(16)   /* 16 pps  (default) */
#define ZL303XX_PTP_G8275p1_LOG_SYNC_DEF       (Sint8T)(-4)   /* 16 pps  (default) */
#define ZL303XX_PTP_G8275p1_LOG_SYNC_MIN       (Sint8T)(-4)   /* 16 pps  */
#define ZL303XX_PTP_G8275p1_LOG_SYNC_MAX       (Sint8T)(-4)   /* 16 pps  */

#define ZL303XX_PTP_G8275p1_PPS_DELAY_DEF      (Sint8T)(16)   /* 16 pps  (default) */
#define ZL303XX_PTP_G8275p1_LOG_DELAY_DEF      (Sint8T)(-4)   /* 16 pps  (default) */
#define ZL303XX_PTP_G8275p1_LOG_DELAY_MIN      (Sint8T)(-4)   /* 16 pps  */
#define ZL303XX_PTP_G8275p1_LOG_DELAY_MAX      (Sint8T)(-4)   /* 16 pps  */

#define ZL303XX_PTP_G8275p1_PPS_ANNC_DEF       (Sint8T)(8)    /* 8 pps  (default) */
#define ZL303XX_PTP_G8275p1_LOG_ANNC_DEF       (Sint8T)(-3)   /* 8 pps  (default) */
#define ZL303XX_PTP_G8275p1_LOG_ANNC_MIN       (Sint8T)(-3)   /* 8 pps  */
#define ZL303XX_PTP_G8275p1_LOG_ANNC_MAX       (Sint8T)(-3)   /* 8 pps  */

#define ZL303XX_PTP_G8275p1_ANNC_TIMEOUT_MIN   3
#define ZL303XX_PTP_G8275p1_ANNC_TIMEOUT_MAX   10
#define ZL303XX_PTP_G8275p1_ANNC_TIMEOUT_DEF   ZL303XX_PTP_G8275p1_ANNC_TIMEOUT_MIN

/* G.8275.2 */
#define ZL303XX_PTP_G8275p2_PPS_SYNC_DEF       (Sint8T)(64)   /*  64 pps  (default) */
#define ZL303XX_PTP_G8275p2_LOG_SYNC_DEF       (Sint8T)(-6)   /*  64 pps  (default) */
#define ZL303XX_PTP_G8275p2_LOG_SYNC_MIN       (Sint8T)(-7)   /* 128 pps  */
#define ZL303XX_PTP_G8275p2_LOG_SYNC_MAX       (Sint8T)(0)    /*   1 pps  */

#define ZL303XX_PTP_G8275p2_PPS_DELAY_DEF      (Sint8T)(64)   /*  64 pps  (default) */
#define ZL303XX_PTP_G8275p2_LOG_DELAY_DEF      (Sint8T)(-6)   /*  64 pps  (default) */
#define ZL303XX_PTP_G8275p2_LOG_DELAY_MIN      (Sint8T)(-7)   /* 128 pps  */
#define ZL303XX_PTP_G8275p2_LOG_DELAY_MAX      (Sint8T)(0)    /*   1 pps  */

#define ZL303XX_PTP_G8275p2_PPS_ANNC_DEF       (Sint8T)(1)    /* 1 pps  (default) */
#define ZL303XX_PTP_G8275p2_LOG_ANNC_DEF       (Sint8T)(0)    /* 1 pps  (default) */
#define ZL303XX_PTP_G8275p2_LOG_ANNC_MIN       (Sint8T)(-3)   /* 8 pps  */
#define ZL303XX_PTP_G8275p2_LOG_ANNC_MAX       (Sint8T)(0)    /* 1 pps  */

#define ZL303XX_PTP_G8275p2_ANNC_TIMEOUT_MIN   2
#define ZL303XX_PTP_G8275p2_ANNC_TIMEOUT_MAX   255
#define ZL303XX_PTP_G8275p2_ANNC_TIMEOUT_DEF   3

/* Domain number range */
/* G.8275.1 */
#define ZL303XX_PTP_G8275p1_DOMAIN_NUM_MIN     24
#define ZL303XX_PTP_G8275p1_DOMAIN_NUM_MAX     43
#define ZL303XX_PTP_G8275p1_DOMAIN_NUM_DEF     ZL303XX_PTP_G8275p1_DOMAIN_NUM_MIN
/* G.8275.2 */
#define ZL303XX_PTP_G8275p2_DOMAIN_NUM_MIN     44
#define ZL303XX_PTP_G8275p2_DOMAIN_NUM_MAX     63
#define ZL303XX_PTP_G8275p2_DOMAIN_NUM_DEF     ZL303XX_PTP_G8275p2_DOMAIN_NUM_MIN

/* Clock Class Ranges. */
#define ZL303XX_PTP_G8275p1_GM_CLASS_LOCK                    6
#define ZL303XX_PTP_G8275p1_GM_CLASS_IN_HOLDOVER             7
#define ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT1     140
#define ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT2     150
#define ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_CAT3     160

#define ZL303XX_PTP_G8275p1_BC_CLASS_IN_HOLDOVER           135
#define ZL303XX_PTP_G8275p1_BC_CLASS_OUT_HOLDOVER          165

#define ZL303XX_PTP_G8275p1_CLASS_FREERUN                  248
#define ZL303XX_PTP_G8275p1_CLASS_SLAVE                    255

#define ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_C2_ALT   8
#define ZL303XX_PTP_G8275p1_GM_CLASS_OUT_HOLDOVER_C3_ALT   52
#define ZL303XX_PTP_G8275p1_BC_CLASS_OUT_HOLDOVER_ALT      187

/* Clock Accuracy Ranges. */
#define ZL303XX_PTP_G8275p1_ACCURACY_LOCK_ePRTC      ZL303XX_CLOCK_ACCURACY_WITHIN_25ns    /* 0x20 */
#define ZL303XX_PTP_G8275p1_ACCURACY_LOCK            ZL303XX_CLOCK_ACCURACY_WITHIN_100ns   /* 0x21 */
#define ZL303XX_PTP_G8275p1_ACCURACY_UNLOCK          ZL303XX_CLOCK_ACCURACY_UNKNOWN        /* 0xFE */

/* Clock Variance Ranges. */
#define ZL303XX_PTP_G8275p1_VARIANCE_LOCK_ePRTC      0x4B32
#define ZL303XX_PTP_G8275p1_VARIANCE_LOCK            0x4E5D
#define ZL303XX_PTP_G8275p1_VARIANCE_UNLOCK          0xFFFF

/* Local Priority */
#define ZL303XX_PTP_G8275_LOCAL_PRI_DEF              128
#define ZL303XX_PTP_G8275_LOCAL_PRI_MIN              1
#define ZL303XX_PTP_G8275_LOCAL_PRI_MAX              255
/* Allowed to disable on the stream only. */
#define ZL303XX_PTP_G8275_LOCAL_PRI_DISABLED         0

#define ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF           128
#define ZL303XX_PTP_G8275p1_CLOCK_PRI2_MASTER        128
#define ZL303XX_PTP_G8275p1_CLOCK_PRI2_SLAVE         255

/*****************   DATA TYPES   *********************************************/

/** G.8275.1 uses 3 types of clocks (as defined below). These have been defined
 *  in such as way as to be compatible with Table 42 of the IEEE-1588 Std.
 *  (It uses the upper-nibble of the 16-bit clockType field to store the G.8275
 *  clockType while maintaining the standard value in their usual positions so
 *  that proper data is sent in TLV messages, etc.) */
typedef enum zl303xx_PtpG8275p1ClockTypeE
{
   /** The node implements a T-GM according to the G.8275 architecture. */
   ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_GM   = (Uint16T)0x1001,   /* T-GM + OC */

   /** The node implements a T-BC according to G.8275 architecture. */
   ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_BC   = (Uint16T)0x2002,   /* T-BC + BC */

   /** The node implements a T-TSC according to the G.8275 architecture. */
   ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_TSC  = (Uint16T)0x4001    /* T-TSC + OC */
} zl303xx_PtpG8275p1ClockTypeE;

/** G.8275.1 provides information related to T-GM and T-BC clock states in
 *  order to provide a high-level indication of the operational status of the
 *  entire clock as opposed to just individual PTP ports (the clock state is
 *  dependent on the combination of local port states). The clock state itself
 *  determines the content of the Announce message fields. */
typedef enum zl303xx_PtpG8275p1ClockStateE
{
   /** The PTP clock has never been synchronized to a time source and is not in
    *  the process of synchronizing to a time source. As it relates to the PTP
    *  port state defined in [IEEE 1588], a clock is in Free-Run state if there
    *  are no PTP ports in: MASTER, PRE-MASTER, PASSIVE, UNCALIBRATED or SLAVE
    *  states. */
   ZL303XX_PTP_G8275p1_CLOCK_STATE_FREERUN = 0,

   /** The PTP clock is in process of synchronizing to a time source. The
    *  duration and functionality of this state is implementation specific. This
    *  state is not required in an implementation. As it relates to the PTP port
    *  state defined in [IEEE 1588], a clock is in Acquiring state if there is a
    *  PTP port in UNCALIBRATED state. */
   ZL303XX_PTP_G8275p1_CLOCK_STATE_ACQUIRING,

   /** The PTP clock is no longer synchronized to a time source and, while it
    *  may be using information obtained while it was previously synchronized or
    *  other information sources were still available, it is unable to maintain
    *  performance within desired specification. As it relates to the PTP port
    *  state defined in [IEEE 1588], a clock is in Holdover-Of-Out-Specification
    *  state if there are no PTP ports in: INITIALIZING, LISTENING, UNCALIBRATED
    *  or SLAVE states, and performance is not within desired specification. */
   ZL303XX_PTP_G8275p1_CLOCK_STATE_HOLDOVER_OUT_SPEC,

   /** The PTP clock is no longer synchronized to a time source and is using
    *  information obtained while it was previously synchronized or other
    *  information sources were still available, to maintain performance within
    *  desired specification. The node may be relying solely on its own
    *  facilities for holdover or may use something like a frequency input from
    *  the network to achieve a holdover of time and/or phase. As it relates to
    *  the PTP port state defined in [IEEE 1588], a clock is in
    *  Holdover-In-Specification state if there are no PTP ports in:
    *  INITIALIZING, LISTENING, UNCALIBRATED or SLAVE states, and performance is
    *  within desired specification. */
   ZL303XX_PTP_G8275p1_CLOCK_STATE_HOLDOVER_IN_SPEC,

   /** The PTP clock is synchronized to a time source and is within some
    *  internal acceptable accuracy. As it relates to the PTP port state defined
    *  in [IEEE 1588], a clock is in Locked state if there is a PTP port in
    *  SLAVE state. */
   ZL303XX_PTP_G8275p1_CLOCK_STATE_LOCKED,

   /** The Clock state is UNKNOWN (has not been evaluated or is beyond the range
    *  of this ENUM definition. This may signal an error in the logic).  */
   ZL303XX_PTP_G8275p1_CLOCK_STATE_UNKNOWN
} zl303xx_PtpG8275p1ClockStateE;

/** G.8275.1 Clock Class Description (maps the Clock-State to a description
 *  that closer resembles Table 6-2 of the profile. For informational
 *  purposes only. */
typedef enum zl303xx_PtpG8275p1ClockClassDescE
{
   /** The PTP clock has never been synchronized to a time source and is not in
    *  the process of synchronizing to a time source. */
   ZL303XX_PTP_G8275p1_CLOCK_CLASS_DESC_FREERUN = 0,

   /** The PTP clock is no longer synchronized to a time source and is using
    *  information obtained while it was previously synchronized or other
    *  information sources were still available, to maintain performance within
    *  desired specification. */
   ZL303XX_PTP_G8275p1_CLOCK_CLASS_DESC_HOLDOVER,

   /** The PTP clock is synchronized to a time source and is within some
    *  internal acceptable accuracy. */
   ZL303XX_PTP_G8275p1_CLOCK_CLASS_DESC_LOCKED,

   /** The Clock is UNKNOWN.  */
   ZL303XX_PTP_G8275p1_CLOCK_CLASS_DESC_UNKNOWN
} zl303xx_PtpG8275p1ClockClassDescE;

/** G.8275.1 method used to update the egress clockClass.  */
typedef enum zl303xx_PtpG8275p1ClassEvalMethodE
{
   /** The node implements Table 6.2 of the ITU G.8275.1 standard. */
   ZL303XX_PTP_G8275p1_CLASS_EVAL_DEFAULT = 0,

   /** The node implements Table 6.2 of the ITU G.8275.1 standard along with
    *  Table 6.4 (for earlier deployments. */
   ZL303XX_PTP_G8275p1_CLASS_EVAL_METHOD2,

   /** The node implements a 3rd evaluation method. */
   ZL303XX_PTP_G8275p1_CLASS_EVAL_METHOD3
} zl303xx_PtpG8275p1ClassEvalMethodE;

/*****************   DATA STRUCTURES   ****************************************/

/** Profile specific TLV to communicate Timestamp Interface Rate data.
 *  (Attached to GRANT Signaling TLVs). */
typedef struct zl303xx_PtpG8275TimestampIfRateTlvS
{
   /* The period of 1-bit of the transmitting PTP timestamp interface, excluding
    * line encoding.  The value is encoded as an unsigned integer in units of
    * attoseconds (10^-18) to accommodate interface bit periods less than 1 ns. */
   Uint64S interfaceBitPeriod;
   /* The length of the packet prior to the timestamp point, in bits.  */
   Uint16T numberBitsBeforeTimestamp;
   /* The length of the packet after the timestamp point, in bits. */
   Uint16T numberBitsAfterTimestamp;
} zl303xx_PtpG8275TimestampIfRateTlvS;

/** Profile specific clock data required for proper profile operation. */
typedef struct zl303xx_PtpG8275ClockConfigS
{
   /** A configurable member allowing the implementation to bypass the
    *  G.8275.1 restrictions on advertised Announce parameters and allow the
    *  user to set the advertised values via the ParentDS. (On a T-GM, the
    *  ParentDS = DefaultDS (class, accuracy, variance) + TimePropertiesDS
    *  (frequencyTraceable, timeTraceable).
    *  Unused for other profiles.
    *  Values: 0 (use G.8275.1 restrictions - default setting)
    *          1 (bypass mode - ignore G.8275.1 restrictions) */
   zl303xx_BooleanE bypassEnabled;
   /** A configurable member defining the localPriority attribute of the
    *  local clock. Used in the Best Master Clock Algorithm of the G.8275.1
    *  profile when comparing the local clock data set (D0) to that of a
    *  received potential grandmasters. Unused for other profiles.
    *  Values: 1 (highest priority) - 255 (lowest priority). */
   Uint8T localPriority;
   /** A configurable member defining which method is used to evaluate the
    *  clockClass that is advertised from the Clock. Refer to the
    *  zl303xx_PtpG8275p1ClassEvalMethodE enumeration for descriptions.  */
   zl303xx_PtpG8275p1ClassEvalMethodE classEvalMethod;
   /** A configurable member defining the alternate equipment class to use
    *  for T-GM clocks in holdover (Out-of-spec) for Table 6.4 of G.8275.1.  */
   Uint8T equipmentClass;
   /** A configurable member applicable to T-TSC clocks only.
    *  - TRUE:  Allows the T-TSC clock to use HOLDOVER Classes (135/165) in
    *           BMCA selection. (Like a T-BC).
    *  - FALSE: The T-TSC clock always uses class 255 in BMCA selection.
    *  Values: FALSE (default) or TRUE */
   zl303xx_BooleanE holdoverSupported;
} zl303xx_PtpG8275ClockConfigS;

/** Profile specific clock data learned or dynamically maintained during
 *  profile operation. */
typedef struct zl303xx_PtpG8275ClockDS
{
   /** A dynamic member to store the zl303xx_PtpG8275p1ClockStateE value
    *  (refer to the ENUM definition).  */
   zl303xx_PtpG8275p1ClockStateE clockState;
   /** A dynamic member to store the zl303xx_PtpG8275p1ClockClassDescE value
    *  (refer to the ENUM definition).  */
   zl303xx_PtpG8275p1ClockClassDescE clockClassDesc;
   /** A dynamic member to store the clockClass value advertised (based on
    *  PLL performance information). */
   Uint8T pllClockClass;
   /** A dynamic member to store the frequencyTraceable value advertised
    *  (based on PLL performance information). */
   zl303xx_BooleanE pllFrequencyTraceable;
   /** A dynamic member to store whether the TOD has been set. */
   zl303xx_BooleanE todSet;
   /** A dynamic member indicating the clockClass that this BC may advertise
    *  if it falls out of PHASE_LOCK or a higher class level. As the BC
    *  approaches LOCK or degrades, this value is updated by dynamically. */
   Uint8T fallbackClass;
   /** A dynamic member indicating if the clock was previously locked to a
    *  PRTC source. This is needed on the T-BC with a PRTC Virtual Port since
    *  it should behave like a T-GM in that case. */
   zl303xx_BooleanE prtcHoldover;
} zl303xx_PtpG8275ClockDS;

/** Profile specific port data required for proper profile operation. */
typedef struct zl303xx_PtpG8275PortConfigS
{
   /** A configurable member defining the localPriority attribute of the
    *  local port. Used in the Best Master Clock Algorithm of the G.8275
    *  profile. Unused for other profiles.
    *  Values: 1 (highest priority) - 255 (lowest priority). */
   Uint8T localPriority;

   /** A configurable member defining the Timestamp Interface attributes
    *  associated with this local PTP port. This data is used to issue the
    *  Timestamp Interface Rate TLV of the G.8275.2 profile. Unused for other
    *  profiles.   */
   zl303xx_PtpG8275TimestampIfRateTlvS tsIfRate;

   /** Deprecated: The notSlave definition was changed by Ammendment-1 of
    *  G.8275.1. The following member value is ignored in favor of the
    *  optionalPortDS::masterOnly definition. It is retained for the purpose of
    *  backward compatibility. */
    zl303xx_BooleanE notSlave;
} zl303xx_PtpG8275PortConfigS;

/** Profile specific stream data required for proper profile operation. */
typedef struct zl303xx_PtpG8275StreamConfigS
{
   /** A configurable member defining the localPriority attribute of the
    *  local stream. Takes priority over the associated Port localPriority. Used
    *  in the Best Master Clock Algorithm of the G.8275.1 profile. Unused for
    *  other profiles.
    *  Values: 1 (highest priority) - 255 (lowest priority). */
   Uint8T localPriority;

   /** Deprecated: The notSlave definition was changed by Ammendment-1 of
    *  G.8275.1. The following member value is ignored in favor of the
    *  combination of optionalPortDS::masterOnly and stream::config::operMode
    *  definitions. It is retained for the purpose of backward compatibility. */
   zl303xx_BooleanE notSlave;
} zl303xx_PtpG8275StreamConfigS;

/** Profile specific stream data learned or dynamically maintained during
 *  profile operation. */
typedef struct zl303xx_PtpG8275StreamDS
{
   /** A dynamic member defining the Timestamp Interface attributes
    *  associated with the remote PTP port that is issuing timing messages to
    *  this stream (via the Timestamp Interface Rate TLV of the G.8275.2
    *  profile). Can be used for Asymmetry correction calculations. Can be
    *  queried using zl303xx_PtpG8275StreamTimestampIfRateGet() */
   zl303xx_PtpG8275TimestampIfRateTlvS tsIfRate;
} zl303xx_PtpG8275StreamDS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */

