

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Contains routines to display common PTP values as strings; typically
*     ENUM definitions used in logging or TRACE routines.
*
*     This module should contain ONLY COMMON PTP strings.
*     The API for each routine must be contained in the zl303xx_PtpApi.h
*        header module (so that this module can be ported easily).
*     This module MUST NOT contain strings for internal values so that internal
*        header modules will not need to be included.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_PtpStrings.h"

/*****************   DEFINES     **********************************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/* String corresponding to the Profile ID. */
static const char *Zl303xx_PtpProfileDecrStr[] = {
      "DEFAULT PROFILE (E2E)",         /* ZL303XX_PTP_PROFILE_DEFAULT_E2E */
      "DEFAULT PROFILE (P2P)",         /* ZL303XX_PTP_PROFILE_DEFAULT_P2P */
      "G.8265 (Telecom Frequency)",    /* ZL303XX_PTP_PROFILE_TELECOM_G8265_1 */
      "G.8275.1v1 (Telecom Phase)",    /* ZL303XX_PTP_PROFILE_TELECOM_G8275_1v1 */
      "G.8275.1v2 (Telecom Phase)",    /* ZL303XX_PTP_PROFILE_TELECOM_G8275_1v2 */
      "G.8275.2 (Telecom Phase)",      /* ZL303XX_PTP_PROFILE_TELECOM_G8275_2 */
      "C27.238 (Power Profile:2011)",  /* ZL303XX_PTP_PROFILE_POWER_C37_238_2011 */
      "C27.238 (Power Profile:2014)",   /* ZL303XX_PTP_PROFILE_POWER_C37_238_2014 */
      "802.1AS (TSN Profile:2011)"       /* ZL303XX_PTP_PROFILE_TSN_802_1_2011 */
};

/* String corresponding to the BMCA Result.  This mappings is not 1-to-1;
 * 2 needs to be added to the value to get the proper index. */
static const char *Zl303xx_PtpBmcaResultsStr[] = {
      "B TOPOLOGICALLY BETTER THAN A", /* (-2) ZL303XX_PTP_BMC_B_TOPOLOGICALLY_BETTER_THAN_A */
      "B BETTER THAN A",               /* (-1) ZL303XX_PTP_BMC_B_BETTER_THAN_A */
      "A EQUALS B",                    /*  (0) ZL303XX_PTP_BMC_A_EQUALS_B    */
      "A BETTER THAN B",               /*  (1) ZL303XX_PTP_BMC_A_BETTER_THAN_B */
      "A TOPOLOGICALLY BETTER THAN B"  /*  (2) ZL303XX_PTP_BMC_A_TOPOLOGICALLY_BETTER_THAN_B */
};

/* Strings corresponding to the zl303xx_PtpStreamOperatingStateE TLV type. */
static const char *Zl303xx_PtpStreamOperStateStr[] = {
      "UNKNOWN",        /* ZL303XX_PTP_STREAM_OPER_STATE_UNKNOWN        */
      "DISABLED",       /* ZL303XX_PTP_STREAM_OPER_STATE_DISABLED       */
      "GRANDMASTER",    /* ZL303XX_PTP_STREAM_OPER_STATE_GRANDMASTER    */
      "MASTER",         /* ZL303XX_PTP_STREAM_OPER_STATE_MASTER         */
      "REPEATER",       /* ZL303XX_PTP_STREAM_OPER_STATE_REPEATER       */
      "PASSIVE",        /* ZL303XX_PTP_STREAM_OPER_STATE_PASSIVE_MASTER */
      "SLAVE",          /* ZL303XX_PTP_STREAM_OPER_STATE_SLAVE          */
      "LISTENING",      /* ZL303XX_PTP_STREAM_OPER_STATE_LISTENING      */
      "MONITORING"      /* ZL303XX_PTP_STREAM_OPER_STATE_MONITORING     */
};

/* Strings corresponding to the zl303xx_PtpV2TlvTypeE TLV type. */
static const char *Zl303xx_TlvTypeStr[] =
{
      "RESERVED",
      "MANAGEMENT",
      "MANAGEMENT_ERROR_STATUS",
      "VENDOR_EXTENSION",
      "REQUEST_UNICAST_TX",
      "GRANT_UNICAST_TX",
      "CANCEL_UNICAST_TX",
      "CANCEL_UNICAST_TX_ACK",
      "PATH_TRACE",
      "ALTERNATE_TIME_OFFSET_INDICATOR",
      "CUMULATIVE_RATE_RATIO",
      "ENHANCED_ACCURACY_METRICS",
      "PAD"
};

/* Strings corresponding to the zl303xx_PtpStreamModeE ENUM type. */
static const char *Zl303xx_StreamModeStr[] =
{
      "USE_BMC",
      "SLAVE_ONLY",
      "MASTER_ONLY"
};

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   IMPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* zl303xx_PtpStreamStateToStr */
/**

   Converts a Profile Identity ENUM to a string.

  Parameters:
   [in]  profileId   Profile ID to convert.

  Return Value:
     (const char *)  The Profile Identity as a string.
     "INVALID"       Profile Identity is out of range.

*******************************************************************************/
const char *zl303xx_PtpProfileIdToStr(zl303xx_PtpProfileE profileId)
{
   /* If the profileId is not valid, set ("INVALID") of some sort. */
   if (profileId < ZL303XX_PTP_PROFILE_NUM_TYPES)
   {
      return Zl303xx_PtpProfileDecrStr[profileId];
   }

   return "INVALID PROFILE";
}

/* zl303xx_PtpBmcaResultToStr */
/**

   Converts a BMCA Comparison Result value to a string.
   BMCA comparison results range from -2 to +2 so the value has to be adjusted
   to line up with the string array index (which goes from 0 to 4).

  Parameters:
   [in]  bmcaResult     BMCA Comparison Result to convert.

  Return Value:
     (const char *)  The BMCA Comparison Result as a string.
     "INVALID"       BMCA Comparison Result is out of range.

*******************************************************************************/
const char *zl303xx_PtpBmcaResultToStr(Sint8T bmcaResult)
{
   /* If the state is not valid, use the last index of the array ("INVALID"). */
   if ((bmcaResult <= ZL303XX_PTP_BMC_A_TOPOLOGICALLY_BETTER_THAN_B) &&
       (bmcaResult >= ZL303XX_PTP_BMC_B_TOPOLOGICALLY_BETTER_THAN_A))
   {
      /* Add 2 to the input to get the proper array index. */
      return Zl303xx_PtpBmcaResultsStr[bmcaResult + 2];
   }

   return "INVALID BMCA RESULT";
}

/* zl303xx_PtpStreamStateToStr */
/**

   Converts a stream operating state ENUM value to a string.

  Parameters:
   [in]  state    Stream State to convert.

  Return Value:
     (const char *)  The stream operating state as a string.
     "INVALID"       Stream state is out of range.

*******************************************************************************/
const char *zl303xx_PtpStreamStateToStr(zl303xx_PtpStreamOperatingStateE state)
{
   /* If the state is not valid, use the last index of the array ("INVALID"). */
   if (state <= ZL303XX_PTP_STREAM_OPER_STATE_MONITORING)
   {
      return Zl303xx_PtpStreamOperStateStr[state];
   }

   return "INVALID OPERATING STATE";
}

/* zl303xx_PtpV2MsgTlvTypeToStr */
/** 

   Converts a tlvType field to a string.

  Parameters:
   [in]  tlvType     Value to convert to string.

  Return Value:
     (const char *)  String representation
     "INVALID"       tlvType is out of range.

*******************************************************************************/
const char *zl303xx_PtpV2MsgTlvTypeToStr(zl303xx_PtpV2TlvTypeE tlvType)
{
   if (tlvType < PTP_TLV_TYPE_MAX)
   {
      return Zl303xx_TlvTypeStr[tlvType];
   }

   return "INVALID TLV TYPE";
}

/* zl303xx_PtpStreamModeToStr */
/** 

   Converts a zl303xx_PtpStreamModeE ENUM value to a string.

  Parameters:
   [in]  streamMode  Value to convert to string.

  Return Value:
     (const char *)  The stream mode as a string.
     "INVALID"       Stream mode is out of range.

*******************************************************************************/
const char *zl303xx_PtpStreamModeToStr(zl303xx_PtpStreamModeE streamMode)
{
   if (streamMode < ZL303XX_PTP_STREAM_MODE_NUM_TYPES)
   {
      return Zl303xx_StreamModeStr[streamMode];
   }

   return "UNKNOWN STREAM MODE";
}

/* zl303xx_PtpV2ClockIdentityToString */
/** 

   Formats a Clock Identity To a String of format "00:00:00:00:00:00:00:00"
   The provided string buffer requires a minimum length of 24 bytes.

  Parameters:
   [in]  clockIdentity  The clock identity to display.

   [out] clkIdStr       The formatted clock identity string.

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpV2ClockIdentityToString(zl303xx_ClockIdentity clockIdentity, char *clkIdStr)
{
   Uint8T *clkId = (Uint8T *)clockIdentity;

   /* Assumes a minimum of 24 bytes ("00:00:00:00:00:00:00:00")  */
   OS_MEMSET(clkIdStr, 0x00, CLOCK_ID_STR_LEN);
   OS_SPRINTF(clkIdStr,
           "%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx",
           clkId[0], clkId[1], clkId[2], clkId[3],
           clkId[4], clkId[5], clkId[6], clkId[7]);
}

/* zl303xx_PtpV2PortIdentityToString */
/** 

   Formats a Port Identity To a String of format "00:00:00:00:00:00:00:00 (port: 0x0000)".
   The provided string buffer requires a minimum length of 39 bytes.

  Parameters:
   [in]  portIdentity   The port identity to display.

   [out] portIdStr      The formatted port identity string.


  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpV2PortIdentityToString(zl303xx_PortIdentity *portIdentity, char *portIdStr)
{
   char clockIdStr[CLOCK_ID_STR_LEN];

   /* Format the Clock Identity portion of the port ID. */
   zl303xx_PtpV2ClockIdentityToString(portIdentity->clockIdentity, clockIdStr);

   /* Assumes a minimum of 39 bytes ("00:00:00:00:00:00:00:00 (port: 0x0000)")  */
   OS_MEMSET(portIdStr, 0x00, PORT_ID_STR_LEN);
   OS_SPRINTF(portIdStr, "%s (port: 0x%04hx)", clockIdStr, portIdentity->portNumber);
}

/* zl303xx_PtpV2TimestampToString */
/** 

   Formats a Time Stamp structure To a String of format "Sec:Ns", ignoring
   the epoch. The provided string buffer requires a minimum length of 21 bytes.

  Parameters:
   [in]  timeStamp   The Time Stamp to display.

   [out] tsStr       The formatted Time Stamp string.


  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpV2TimestampToString(zl303xx_TimeStamp *timeStamp, char *tsStr)
{
   /* Assumes a minimum of 21 bytes ("4294967295:999999999")  */
   OS_MEMSET(tsStr, 0x00, TIMESTAMP_STR_LEN);
   OS_SNPRINTF(tsStr, TIMESTAMP_STR_LEN, "%10lu:%9lu",
            (long unsigned int)timeStamp->secondsField.lo, (long unsigned int)timeStamp->nanosecondsField);
}


/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/*****************   END   ****************************************************/
