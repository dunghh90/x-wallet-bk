

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This module contains types and definitions for Telecom Profile for
*        Phase Best-Master Clock (BMCA) comparison routines.
*
*******************************************************************************/

#ifndef _ZL303XX_PTP_BMCA_G8275p1_PROFILE_H_
#define _ZL303XX_PTP_BMCA_G8275p1_PROFILE_H_

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"        /* Basic ZL datatypes. */
#include "zl303xx_Trace.h"         /* TRACE and logging functionality. */
#include "zl303xx_Error.h"         /* For zlStatusE return type. */

#include "zl303xx_PtpStdTypes.h"   /* Native PTP types (standard types). */

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/** Implementation specific data structure used for comparison of data for
 *  the Telecom Profile for Phase BMCA.  */
typedef struct zl303xx_PtpG8275ProfileCompareDataS
{
   /** PortDS - masterOnly configuration */
   zl303xx_BooleanE masterOnly;
   /** PortDS - localPriority */
   Uint8T localPriority;
   /** PortDS - Receiver Port Identity */
   zl303xx_PortIdentity rxPortIdentity;

   /** Grandmaster Clock Quality (Class, Accuracy & Precision) */
   zl303xx_ClockQuality grandmasterClockQuality;
   /** Grandmaster priority2 */
   Uint8T grandmasterPriority2;
   /** Grandmaster clockIdentity */
   zl303xx_ClockIdentity grandmasterIdentity;

   /** Steps Removed */
   Uint16T stepsRemoved;
   /** PortDS - Sender Port Identity */
   zl303xx_PortIdentity txPortIdentity;

   /** Deprecated: Removed by Ammendment-1 of G.8275.1.
    *  Use the masterOnly member of this structure (above) instead. */
   /** PortDS - notSlave configuration */
   zl303xx_BooleanE notSlave;
} zl303xx_PtpG8275ProfileCompareDataS;

/** Implementation specific data structure used to pass extra
 *  configuration data required by the Telecom Profile for Phase BMCA.  */
typedef struct zl303xx_PtpBmcG8275ProfileExtraDataS
{
   /** None currently. */
   Uint32T empty;
} zl303xx_PtpBmcG8275ProfileExtraDataS;

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/* Telecom Profile for Phase BMCA Comparison Routine    */
/* -------------------------------------------------------- */
Sint8T zl303xx_PtpBmcaG8275ProfileComparison(
      zl303xx_PtpG8275ProfileCompareDataS *compareDataA,
      zl303xx_PtpG8275ProfileCompareDataS *compareDataB,
      zl303xx_PtpBmcG8275ProfileExtraDataS *extraConfig);


#endif


