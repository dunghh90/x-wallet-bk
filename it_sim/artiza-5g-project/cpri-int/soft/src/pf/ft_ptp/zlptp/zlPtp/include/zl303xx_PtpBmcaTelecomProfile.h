

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This module contains types and definitions for Telecom Profile for
*        Frequency Best-Master Clock (BMCA) comparison routines.
*
*******************************************************************************/

#ifndef _ZL303XX_PTP_BMCA_TELECOM_PROFILE_H_
#define _ZL303XX_PTP_BMCA_TELECOM_PROFILE_H_

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"        /* Basic ZL datatypes. */
#include "zl303xx_Trace.h"         /* TRACE and logging functionality. */
#include "zl303xx_Error.h"         /* For zlStatusE return type. */

#include "zl303xx_PtpStdTypes.h"   /* Native PTP types (standard types). */

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/** Implementation specific data structure used for comparison of data for
 *  the Telecom Profile for Frequency BMCA.  */
typedef struct zl303xx_PtpTelecomCompareDataS
{
   /** PTSF - Overall value */
   zl303xx_BooleanE ptsf;
   /** PTSF - individual flags */
   Uint32T ptsfFlags[1];
   /** PTSF - individual masks */
   Uint32T ptsfMasks[1];

   /** Server Priority */
   Uint8T priority;
   /** Server Class (QL) */
   Uint8T clockClass;
   /** Grandmaster Clock Identity */
   zl303xx_ClockIdentity grandmasterIdentity;
   /** Steps Removed */
   Uint16T stepsRemoved;
} zl303xx_PtpTelecomCompareDataS;

/** Implementation specific data structure used to pass extra
 *  configuration data required by the Telecom Profile for Frequency BMCA.  */
typedef struct zl303xx_PtpBmcTelecomProfileExtraDataS
{
   /** Indicates if the system is operating in Revertive Switching Mode. */
   zl303xx_BooleanE revertiveEn;
} zl303xx_PtpBmcTelecomProfileExtraDataS;

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/* Telecom Profile for Frequency BMCA Comparison Routine    */
/* -------------------------------------------------------- */
/* This is the new routine that should be used going forward. It adds the 'extra'
 * config data structure (right now contains only the revertiveEn flag but makes
 * expansion easier in the future and interface compatible). */
Sint8T zl303xx_PtpBmcaTelecomProfileComparison(
      zl303xx_PtpTelecomCompareDataS *telecomDataA,
      zl303xx_PtpTelecomCompareDataS *telecomDataB,
      zl303xx_PtpBmcTelecomProfileExtraDataS *extraConfig);

/* This is the original routine which should be deprecated. */
Sint8T zl303xx_PtpTelecomProfileBmcaComparison(
      zl303xx_PtpTelecomCompareDataS *telecomDataA,
      zl303xx_PtpTelecomCompareDataS *telecomDataB,
      zl303xx_BooleanE revertiveEn);


#endif

