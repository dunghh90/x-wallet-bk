

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

#ifndef ZL303XX_PTP_STRINGS_H_
#define ZL303XX_PTP_STRINGS_H_

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpConstants.h"
#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpApi.h"

/*****************   DEFINES   ************************************************/

/* sizeof() will include the NUL terminator */
#define CLOCK_ID_STR_LEN  (sizeof("00:00:00:00:00:00:00:00"))
#define PORT_ID_STR_LEN   (sizeof("00:00:00:00:00:00:00:00 (port: 0x0000)"))
#define TIMESTAMP_STR_LEN (sizeof("4294967295:999999999"))


/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/* Converts a Profile Identity ENUM to a string. */
const char *zl303xx_PtpProfileIdToStr(zl303xx_PtpProfileE profileId);

/* Converts a BMCA Comparison Result value to a string. */
const char *zl303xx_PtpBmcaResultToStr(Sint8T bmcaResult);

/* Convert zl303xx_PtpStreamOperatingStateE ENUM value to a String. */
const char *zl303xx_PtpStreamStateToStr(zl303xx_PtpStreamOperatingStateE state);

/* Convert zl303xx_PtpV2TlvTypeE ENUM value to a String. */
const char *zl303xx_PtpV2MsgTlvTypeToStr(zl303xx_PtpV2TlvTypeE tlvType);

/* Convert zl303xx_PtpStreamModeE ENUM value to a String. */
const char *zl303xx_PtpStreamModeToStr(zl303xx_PtpStreamModeE streamMode);

#endif /* MULTIPLE INCLUDE BARRIER */
