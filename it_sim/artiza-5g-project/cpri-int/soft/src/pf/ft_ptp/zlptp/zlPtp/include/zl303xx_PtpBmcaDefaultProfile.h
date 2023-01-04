

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This module contains types and definitions for the IEEE Std. 1588-2008
*     PTP Default Profile Best-Master Clock (BMCA) comparison routines.
*
*******************************************************************************/

#ifndef _ZL303XX_PTP_BMCA_DEFAULT_PROFILE_H_
#define _ZL303XX_PTP_BMCA_DEFAULT_PROFILE_H_

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"        /* Basic ZL datatypes. */
#include "zl303xx_Trace.h"         /* TRACE and logging functionality. */
#include "zl303xx_Error.h"         /* For zlStatusE return type. */

#include "zl303xx_PtpStdTypes.h"   /* Native PTP types (standard types). */

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/** Implementation specific data structure used for comparison of data for
 *  Best-Master-Clock algorithms. These members are derived from Table 12 of
 *  the IEEE-1588-2008 standard with some implementation specific additions. */
typedef struct zl303xx_PtpBmcCompareDataS
{
   /* ANNOUNCE Message Data Fields. */
   /* Grandmaster members. */
   /** Grandmaster Priority1. */
   Uint8T                grandmasterPriority1;
   /** Grandmaster Priority2. */
   Uint8T                grandmasterPriority2;
   /** Grandmaster Clock Identity. */
   zl303xx_ClockIdentity grandmasterIdentity;
   /** Grandmaster Clock Quality. */
   zl303xx_ClockQuality  grandmasterClockQuality;

   /* Network parameters. */
   /** Network Hops (i.e. steps removed). */
   Uint16T stepsRemoved;

   /* Time Properties parameters. */
   zl303xx_TimePropertiesDS timeProperties;

   /* End point Data. */
   /** The Port Identity of the transmitting port. This is also referred to as
    *  the Foreign Master Identity in the IEEE-1588 standards. */
   zl303xx_PortIdentity foreignPortIdentity;
   /** The Port Identity of the receiving port. */
   zl303xx_PortIdentity receivingPortIdentity;
} zl303xx_PtpBmcCompareDataS;

/** Implementation specific data structure used to pass extra
 *  configuration data required by the Default Profile BMCA.  */
typedef struct zl303xx_PtpBmcaDefaultProfileExtraDataS
{
   /* Empty at the moment. */
   /** An Empty Element. */
   Uint32T  emptyMember;
} zl303xx_PtpBmcaDefaultProfileExtraDataS;


/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/*** BMCA DATA COMPARISON FUNCTIONS ***/
/* This is the new routine that should be used going forward. It adds the 'extra'
 * config data structure (which is actually unused at the moment. */
Sint8T zl303xx_PtpBmcaDefaultProfileComparison(
      zl303xx_PtpBmcCompareDataS *compareDataA,
      zl303xx_PtpBmcCompareDataS *compareDataB,
      zl303xx_PtpBmcaDefaultProfileExtraDataS *extraConfig);

/* This is the original routine which should be deprecated. */
Sint8T zl303xx_PtpV2DefaultProfileBmcaComparison(
      zl303xx_PtpBmcCompareDataS *compareDataA,
      zl303xx_PtpBmcCompareDataS *compareDataB);

/* BMC Comparison Routines    */
/* -------------------------- */
/* Routine to compare 2 data sets based on the IEEE-1588 default algorithm.
 * This comparison is divided into 2 parts based on GM identity; the 2 associated
 * parts are contained in the 2 routines listed below*/
Sint8T zl303xx_PtpV2BmcDataSetComparison(
      zl303xx_PtpBmcCompareDataS *compareDataA,
      zl303xx_PtpBmcCompareDataS *compareDataB);
/* Routine to compare 2 data sets with different grandmasterIdentity. */
Sint8T zl303xx_PtpV2BmcDataSetCompareDiffGM(
      zl303xx_PtpBmcCompareDataS *compareDataA,
      zl303xx_PtpBmcCompareDataS *compareDataB);
/* Routine to compare 2 data sets with the same grandmasterIdentity. */
Sint8T zl303xx_PtpV2BmcDataSetCompareSameGM(
      Uint16T stepsRemovedA, zl303xx_PortIdentity *senderIdA, zl303xx_PortIdentity *receiverIdA,
      Uint16T stepsRemovedB, zl303xx_PortIdentity *senderIdB, zl303xx_PortIdentity *receiverIdB);

/* A General Trace Routine to log a Default BMCA data set. */
void zl303xx_PtpTraceComparisonData(zl303xx_PtpBmcCompareDataS *pCompareData);

/* BMC Comparison Data Set Management Routines    */
/* ---------------------------------------------- */
/* Routine to collect BMCA comparison data from a Clock's DefaultDS. */
void zl303xx_PtpConvertDefaultDsToCompareData(
      zl303xx_DefaultDS *pDefaultDs,
      zl303xx_PtpBmcCompareDataS *compareData);
void zl303xx_PtpConvertCompareDataToParentDs(
      zl303xx_PtpBmcCompareDataS *compareData,
      zl303xx_ParentDS *pParentDs);


#endif
