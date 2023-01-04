

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Data types that only apply to the C.37.238 standard (Power Profile).
*     Currently version 1 (2011) but may be expanded to other versions eventually.
*
*******************************************************************************/

#ifndef ZL303XX_PTP_C37_238_TYPES_H_
#define ZL303XX_PTP_C37_238_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"

#include "zl303xx_PtpStdTypes.h"

/*****************   DEFINES   ************************************************/

/* Current Versions of the Power Profile */
#define ZL303XX_PTP_C37_238_VERSION_2011   (Uint32T)(2011)
#define ZL303XX_PTP_C37_238_VERSION_2014   (Uint32T)(2014)

/* Maximum length of the NAME strings in the LOCAL TIME ZONE TLV extension. */
#define ZL303XX_PTP_C38_238_TIME_ZONE_NAME_MAX_LEN      8

/* Default Settings for the C37.238 profile (V1 and V2) */
#define DEFAULT_C37_238_ANNC_PPS              1
#define DEFAULT_C37_238_ANNC_INTERVAL         0
#define DEFAULT_C37_238_SYNC_PPS              1
#define DEFAULT_C37_238_SYNC_INTERVAL         0
#define DEFAULT_C37_238_PDELAY_PPS            1
#define DEFAULT_C37_238_PDELAY_INTERVAL       0
#define DEFAULT_C37_238_ANNC_TIMEOUT          4  /* 2 for GM Preferred Clocks */
#define DEFAULT_C37_238_P1_MASTER             128
#define DEFAULT_C37_238_P2_MASTER             128
#define DEFAULT_C37_238_P1_SLAVE              255
#define DEFAULT_C37_238_P2_SLAVE              255
#define DEFAULT_C37_238_SLAVE_ONLY_MASTER     ZL303XX_FALSE
#define DEFAULT_C37_238_SLAVE_ONLY_SLAVE      ZL303XX_TRUE

#define DEFAULT_C37_238_CLK_CLASS_GM_SYNC     6
#define DEFAULT_C37_238_CLK_CLASS_GM_HOLDOVER 7
#define DEFAULT_C37_238_CLK_CLASS_GM_FREERUN  187
#define DEFAULT_C37_238_CLK_CLASS_SLAVE       255

/* Default Settings for some specific 2011 profile parameters. */
#define DEFAULT_C37_238_2011_DOMAIN                0
#define DEFAULT_C37_238_2011_CLK_ID_MIN            0x03
#define DEFAULT_C37_238_2011_CLK_ID_MAX            0xFE

/* Default Settings for some specific 2014 profile parameters. */
/* The real value for the Domain should be 238 but the 1588 Std. lists this as a
 * reserved value. Expect this to change in the final 2014 power profile version.  */
#define DEFAULT_C37_238_2014_DOMAIN                38

#define DEFAULT_C37_238_2014_CLK_ID_MIN            0x05
#define DEFAULT_C37_238_2014_CLK_ID_MAX            0xFE

/* Set the maximum Inaccuracy values for the stream at creation time.  */
/* See C37.238: Annex B for constraints. */
#define ZL303XX_PTP_C37_238_LOCAL_TIME_INNAC_MAX     (Uint32T)(200) /* See C37.238: Annex B */
#define ZL303XX_PTP_C37_238_NETWORK_TIME_INNAC_MAX   (Uint32T)(1000)

/* TLV Definitions. */
/* 2011 has a definite length */
#define ZL303XX_PTP_C37_238_2011_TLV_LEN          12 /* 2 + 4 + 4 + 2 (reserved) */
/* 2014 has some definite fields but may have extensions.  */
#define ZL303XX_PTP_C37_238_2014_TLV_LEN_MIN      14 /* 3 (reserved) + 2 + 4 + 4 + 1 reserved */

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/** Profile specific Extension Data type for communicating LOCAL TIME ZONE. */
typedef struct zl303xx_PtpC37p238LocalTimeZoneS
{
   /* Instance of this extension. */
   Uint8T keyField;

   /* Indicates if this local time zone extension supports DST/summer time. */
   zl303xx_BooleanE includesDst;

   /* Offset to UTC in seconds. */
   Sint32T stdTimeOffsetToUtc;

   /* Offset to UTC in seconds (shall be set to zero if includesDst is FALSE) */
   Sint32T dstTimeOffsetToUtc;

   /* stdTimeStartsAt */
   /* Date/time when standard time starts (and daylight saving time ends) */
   struct
   {
      Uint8T month;
      Uint8T week;
      Uint8T day;
      Uint8T hour;
   } stdTimeStartsAt;

   /* dstTimeStartsAt */
   /* Date/time when daylight saving time starts (and standard time ends) */
   struct
   {
      Uint8T month;
      Uint8T week;
      Uint8T day;
      Uint8T hour;
   } dstTimeStartsAt;

   char stdTimeZoneName[ZL303XX_PTP_C38_238_TIME_ZONE_NAME_MAX_LEN];
   char dstTimeZoneName[ZL303XX_PTP_C38_238_TIME_ZONE_NAME_MAX_LEN];
} zl303xx_PtpC37p238LocalTimeZoneS;

/** Profile specific TLV structure for the 2011 revision. */
typedef struct zl303xx_PtpC37p238v1TlvS
{
   /** Clock ID: Range 0x0003 - 0x00FE (else assume unconfigured) */
   Uint16T clockId;

   /** Grandmaster Time Inaccuracy (ns) of the server.
    *  (0xFFFFFFFF indicates overflow value). */
   Uint32T grandmasterTimeInaccuracy;

   /** Network Time Inaccuracy (ns) configured at the server.
    *  (0xFFFFFFFF indicates overflow value). */
   Uint32T networkTimeInaccuracy;
} zl303xx_PtpC37p238v1TlvS;

/** Profile specific TLV structure for the 2014 revision. */
typedef struct zl303xx_PtpC37p238v2TlvS
{
   /** Clock ID: Range 0x0003 - 0x00FE (else assume unconfigured) */
   Uint16T clockId;

   /** Grandmaster Time Inaccuracy (ns) of the server.
    *  (0xFFFFFFFF indicates overflow value). */
   Uint32T grandmasterTimeInaccuracy;

   /** Network Time Inaccuracy (ns) configured at the server.
    *  (0xFFFFFFFF indicates overflow value). */
   Uint32T networkTimeInaccuracy;

   /* Local Time Zone TLV Extension. */
   zl303xx_PtpC37p238LocalTimeZoneS localTimeZoneInfo;
} zl303xx_PtpC37p238v2TlvS;

/** Profile specific clock data required for proper profile operation. */
typedef struct zl303xx_PtpC37p238ClockConfigS
{
   /** Clock ID: Range 0x0003 - 0x00FE (else assume unconfigured) */
   Uint16T clockId;

   /** Local Time Inaccuracy (ns) of this clock. On master clocks, this value is
    *  written to the grandmasterTimeInaccuracy field of the IEEE_C37_238 TLV.
    *  (0xFFFFFFFF indicates overflow value). */
   Uint32T localTimeInaccuracy;

   /** Network Time Inaccuracy (ns). This value is calculated by the system
    *  designer by summing the individual localTimeInaccuracy values for each TC
    *  in the cascade from the grandmaster clock. This value is set at either:
    *    a) the grandmaster clock and transmitted in the IEEE_C37_238 TLV as the
    *       networkTimeInaccuracy field.
    *    b) the end device clock and added to received grandmasterTimeInaccuracy
    *       value in the IEEE_C37_238 TLV to compute the TimeInaccuracy value.
    *  (0xFFFFFFFF indicates overflow value). */
   Uint32T networkTimeInaccuracy;

   zl303xx_PtpC37p238LocalTimeZoneS localTimeZoneInfo;
} zl303xx_PtpC37p238ClockConfigS;

/** Profile specific port data required for proper profile operation. */
typedef struct zl303xx_PtpC37p238PortConfigS
{
   /** No parameters at the moment. */
   Uint32T empty;
} zl303xx_PtpC37p238PortConfigS;

/** Profile specific stream data required for proper profile operation. */
typedef struct zl303xx_PtpC37p238StreamConfigS
{
   /** TLV Configuration settings for the stream. */
   zl303xx_BooleanE profileTlvRequired;
   zl303xx_BooleanE profileTlvAppend;
   Uint32T        profileTlvVersion;
   zl303xx_BooleanE altTimeOffsetTlvRequired;
   zl303xx_BooleanE altTimeOffsetTlvAppend;

   /** Local Time Inaccuracy Max for this Stream (ns). */
   Uint32T localTimeInaccuracyMax;

   /** Network Time Inaccuracy Max for this Stream (ns). */
   Uint32T networkTimeInaccuracyMax;
} zl303xx_PtpC37p238StreamConfigS;

/** Profile specific stream data learned from the far End Server. */
/*  Use the Profile's V2 TLV format. */
typedef zl303xx_PtpC37p238v2TlvS zl303xx_PtpC37p238StreamDS;

/** Profile specific clock data reported for the profile. */
typedef struct zl303xx_PtpC37p238PerformanceS
{
   /** Local Clock ID: Range 0x0003 - 0x00FE (else assume unconfigured) */
   Uint16T localClockId;

   /** Grandmaster Clock ID: Range 0x0003 - 0x00FE (else assume unconfigured) */
   Uint16T grandmasterClockId;
   /** Grandmaster Clock Identity: */
   zl303xx_ClockIdentity grandmasterIdentity;

   /** Time Inaccuracy (ns) value of the selected Master (0xFFFFFFFF indicates
    *  overflow value). */
   Uint32T timeInaccuracy;

   /** This member is ZL303XX_TRUE if the timescale is traceable to standard
    *  recognized time source. Otherwise, it is ZL303XX_FALSE. */
   zl303xx_BooleanE timeTraceable;

   /** The current value of the time difference between a master and a slave as
    *  computed by the slave. */
   zl303xx_TimeInterval offsetFromMaster;
} zl303xx_PtpC37p238ClockPerformanceS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */

