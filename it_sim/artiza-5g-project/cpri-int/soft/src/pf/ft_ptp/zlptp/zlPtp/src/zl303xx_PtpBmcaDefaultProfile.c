

/*****************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
* Copyright 2006-2018 Microsemi Semiconductor Limited.
* All rights reserved.
*
*  Module Description:
*     This module contains the interface for the IEEE Std. 1588-2008 (PTP)
*     Default Profile Best-Master Clock (BMCA) comparison routines.
*
*******************************************************************************/

/****************   INCLUDE FILES   ******************************************/

#include "zl303xx_PtpBmcaDefaultProfile.h"
#include "zl303xx_PtpStrings.h"

#include <string.h>

/****************   DEFINES     **********************************************/

/* Formats a PTP Clock Identity into a string for printing. */
#define ZL303XX_PTP_CLK_ID_TO_STRING(clkId, string)                              \
            OS_SPRINTF(string, "%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx", \
                    clkId[0], clkId[1], clkId[2], clkId[3],                    \
                    clkId[4], clkId[5], clkId[6], clkId[7])

/* Compare 2 PTP Clock Identities (part of the BMCA process). */
#define ZL303XX_PTP_COMPARE_CLK_ID(clkIdA, clkIdB)                              \
            memcmp(clkIdA, clkIdB, sizeof(zl303xx_ClockIdentity))
            

#define ZL303XX_BMCA_SELECTION_STRING_SIZE  128

/****************   STATIC FUNCTION DECLARATIONS   ***************************/

/* Routine to determine if a Data Set originated from a Virtual Port. */
zl303xx_BooleanE zl303xx_PtpV2BmcDataSetIsVirtualPort(
      zl303xx_PtpBmcCompareDataS *compareData);

/* Debug Routine to display all of the data used in the BMC Data Set Comparison.
 * (Purposely used CAPS to resemble other TRACE routines. */
void ZL303XX_TRACE_BMC_COMPARISON(
      Uint32T modId, Uint16T level,
      zl303xx_PtpBmcCompareDataS *compareDataA,
      zl303xx_PtpBmcCompareDataS *compareDataB);

/****************   STATIC GLOBAL VARIABLES   ********************************/
/* criteria on which default BMCA selection was based e.g clockClass */
static char zl303xx_PtpDefaultBMCASelectionCriteria[ZL303XX_BMCA_SELECTION_STRING_SIZE] = "\0";
/****************   EXTERNAL GLOBAL VARIABLES   ******************************/

/****************   EXPORTED GLOBAL VARIABLES   ******************************/

/****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/**
    zl303xx_PtpV2DefaultProfileBmcaComparison

  Details:
   Compares two data sets according to IEEE 1588 v2 spec.

  Parameters:
   [in]  compareDataA   Data Set A comparison data (from Annc or DefaultDS).
   [in]  compareDataB   Data Set B comparison data (from Annc or DefaultDS).

  Return Value:
   -2 if 'B' is better by topology than 'A'
   -1 if 'B' is better than 'A'
    0 if data sets are equal (which is technically an error)
   +1 if 'A' is better than 'B'
   +2 if 'A' is better by topology than 'B'

  Notes:   If one of the input pointers is NULL then the other data is selected.
        If both are NULL, then '0' is returned.

 *****************************************************************************/
Sint8T zl303xx_PtpV2DefaultProfileBmcaComparison(
      zl303xx_PtpBmcCompareDataS *compareDataA,
      zl303xx_PtpBmcCompareDataS *compareDataB)
{
   /* Compare input pointers first. */
   if (compareDataA == NULL)
   {
      if (compareDataB == NULL)
      {
         return 0;
      }
      else
      {
         return -1;
      }
   }
   else  /* compareDataA is good */
   {
      if (compareDataB == NULL)
      {
         return 1;
      }
      else
      {
         return zl303xx_PtpV2BmcDataSetComparison(compareDataA, compareDataB);
      }
   }
}  /* END zl303xx_PtpV2DefaultProfileBmcaComparison */

/*

  Function Name:
    zl303xx_PtpV2BmcDataSetComparison

  Details:
   Compares two data sets according to IEEE 1588 v2 spec.

  Parameters:
   [in]  compareDataA   Data Set A comparison data (from Annc or DefaultDS).
   [in]  compareDataB   Data Set B comparison data (from Annc or DefaultDS).

  Return Value:
   -2 if 'B' is better by topology than 'A'
   -1 if 'B' is better than 'A'
    0 if data sets are equal (which is technically an error)
   +1 if 'A' is better than 'B'
   +2 if 'A' is better by topology than 'B'

  Notes:    Since this routine is called internally, it is assumed that all input
         pointers are valid and already verified.

 *****************************************************************************/
Sint8T zl303xx_PtpV2BmcDataSetComparison(
      zl303xx_PtpBmcCompareDataS *compareDataA,
      zl303xx_PtpBmcCompareDataS *compareDataB)
{
   /* Set a default return value. */
   Sint8T compareResult = ZL303XX_PTP_BMC_A_EQUALS_B;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 2,
         "BmcDataSetComparison: start", 0,0,0,0,0,0);

   ZL303XX_TRACE_BMC_COMPARISON(ZL303XX_MOD_ID_PTP_BMC, 3, compareDataA, compareDataB);

   /* If the grandmasterIdentity values are different, follow comparison
    * algorithm (part 1) of Figure 27 in the IEEE 1588 Std.  */
   /* If one of the data sets is from a Virtual Port start from the top as well. */
   if ((ZL303XX_PTP_COMPARE_CLK_ID(compareDataA->grandmasterIdentity, compareDataB->grandmasterIdentity) != 0) ||
       (zl303xx_PtpV2BmcDataSetIsVirtualPort(compareDataA) == ZL303XX_TRUE) ||
       (zl303xx_PtpV2BmcDataSetIsVirtualPort(compareDataB) == ZL303XX_TRUE))
   {
      compareResult = zl303xx_PtpV2BmcDataSetCompareDiffGM(compareDataA, compareDataB);
   }
   else
   {
      compareResult =  zl303xx_PtpV2BmcDataSetCompareSameGM(
                compareDataA->stepsRemoved, &compareDataA->foreignPortIdentity, &compareDataA->receivingPortIdentity,
                compareDataB->stepsRemoved, &compareDataB->foreignPortIdentity, &compareDataB->receivingPortIdentity);
   }

   /* Log the comparison if required. */
   ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
   {
      const char * bmcaResultStr = zl303xx_PtpBmcaResultToStr(compareResult);
      ZL303XX_TRACE_ALWAYS(
            "zl303xx_PtpV2BmcDataSetComparison: Result (%d): %s %s",
            compareResult, bmcaResultStr, zl303xx_PtpDefaultBMCASelectionCriteria,0,0,0);
			
	  strncpy(zl303xx_PtpDefaultBMCASelectionCriteria,"\0",ZL303XX_BMCA_SELECTION_STRING_SIZE - 1); /* set BMCA selection criteria to empty string after use */
   }

   return compareResult;
}  /* END zl303xx_PtpV2BmcDataSetComparison */

/* zl303xx_PtpV2BmcDataSetCompareDiffGM */
/** 

   Compares two data sets according to IEEE 1588-2008 Fig. 27. These comparisons
   are used when the two data sets have different grandmaster identities.

  Parameters:
   [in]  compareDataA   Data Set A comparison data (from Annc or DefaultDS).
   [in]  compareDataB   Data Set B comparison data (from Annc or DefaultDS).

  Return Value:
   -2 if 'B' is better by topology than 'A'
   -1 if 'B' is better than 'A'
    0 if data sets are equal (which is technically an error)
   +1 if 'A' is better than 'B'
   +2 if 'A' is better by topology than 'B'

  Notes:    Since this routine is called internally, it is assumed that all input
         pointers are valid and already verified.

*******************************************************************************/
Sint8T zl303xx_PtpV2BmcDataSetCompareDiffGM(
      zl303xx_PtpBmcCompareDataS *compareDataA,
      zl303xx_PtpBmcCompareDataS *compareDataB)
{
   Sint32T cmp; /* Temporary result of a comparison */

   /* The grandmasterIdentity values are equal. */
   /*** Point X in flow diagram (Figure 27/28) ****/
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3,
         "BmcDataSetCompareDiffGM: Figure 27 Algorithm (Part 1)",
         0,0,0,0,0,0);

   /* Begin by comparing the grandmasterPriority1 values. */
   if (compareDataA->grandmasterPriority1 < compareDataB->grandmasterPriority1)
   {
      ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
     {
         strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by grandmasterPriority1",ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
     }
     return ZL303XX_PTP_BMC_A_BETTER_THAN_B;
   }
   else if (compareDataB->grandmasterPriority1 < compareDataA->grandmasterPriority1)
   {
      ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
     {
         strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by grandmasterPriority1",ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
     }
     return ZL303XX_PTP_BMC_B_BETTER_THAN_A;
   }

   /* Priority1 fields are the same...
    * ..so compare the grandmasterClockQuality.clockClass */
   if (compareDataA->grandmasterClockQuality.clockClass < compareDataB->grandmasterClockQuality.clockClass)
   {
      ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
     {
         strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by clockClass", ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
     }
     return ZL303XX_PTP_BMC_A_BETTER_THAN_B;
   }
   else if (compareDataA->grandmasterClockQuality.clockClass > compareDataB->grandmasterClockQuality.clockClass)
   {
      ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
     {
         strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by clockClass", ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
     }
     return ZL303XX_PTP_BMC_B_BETTER_THAN_A;
   }

   /* "class" values are the same...
    * ..so compare the grandmasterClockQuality.clockAccuracy */
   if (compareDataA->grandmasterClockQuality.clockAccuracy < compareDataB->grandmasterClockQuality.clockAccuracy)
   {
      ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
     {
         strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by clockAccuracy", ZL303XX_BMCA_SELECTION_STRING_SIZE -1 );
     }
     return ZL303XX_PTP_BMC_A_BETTER_THAN_B;
   }
   else if (compareDataA->grandmasterClockQuality.clockAccuracy > compareDataB->grandmasterClockQuality.clockAccuracy)
   {
      ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
     {
         strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by clockAccuracy", ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
     }
     return ZL303XX_PTP_BMC_B_BETTER_THAN_A;
   }

   /* "clockAccuracy" values are the same...
    * ..so compare the grandmasterClockQuality.offsetScaledLogVariance */
   if (compareDataA->grandmasterClockQuality.offsetScaledLogVariance < compareDataB->grandmasterClockQuality.offsetScaledLogVariance)
   {
      ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
     {
         strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by offsetScaledLogVariance", ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
     }
     return ZL303XX_PTP_BMC_A_BETTER_THAN_B;
   }
   else if (compareDataA->grandmasterClockQuality.offsetScaledLogVariance > compareDataB->grandmasterClockQuality.offsetScaledLogVariance)
   {
      ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
     {
         strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by offsetScaledLogVariance", ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
     }
     return ZL303XX_PTP_BMC_B_BETTER_THAN_A;
   }

   /* grandmasterQuality.scaledLogVariance are same...
    * ..so compare the grandmasterPriority2 values */
   if (compareDataA->grandmasterPriority2 < compareDataB->grandmasterPriority2)
   {
      ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
     {
         strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by grandmasterPriority2", ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
     }
     return ZL303XX_PTP_BMC_A_BETTER_THAN_B;
   }
   else if (compareDataB->grandmasterPriority2 < compareDataA->grandmasterPriority2)
   {
      ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
     {
         strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by grandmasterPriority2", ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
     }
     return ZL303XX_PTP_BMC_B_BETTER_THAN_A;
   }

   /* Priority2 fields are the same...
    * ..so base the result on the grandmasterIdentity */
   cmp = ZL303XX_PTP_COMPARE_CLK_ID(compareDataA->grandmasterIdentity, compareDataB->grandmasterIdentity);

   if (cmp < 0)
   {
      ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
     {
         strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by grandmasterIdentity", ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
     }
     return ZL303XX_PTP_BMC_A_BETTER_THAN_B;
   }
   else if (cmp > 0)
   {
      ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
     {
         strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by grandmasterIdentity", ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
     }
     return ZL303XX_PTP_BMC_B_BETTER_THAN_A;
   }

   /* Can never get to here as all the differences must have been resolved */
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 1,
         "BmcDataSetCompareDiffGM: Figure 27 Algorithm (Part 1): NO BEST MASTER DETERMINED",
         0,0,0,0,0,0);

   return ZL303XX_PTP_BMC_A_EQUALS_B;  /* OR error, OR ignore */
}  /* END zl303xx_PtpV2BmcDataSetCompareDiffGM */

/* zl303xx_PtpV2BmcDataSetCompareSameGM */
/** 

   Compares two data sets according to IEEE 1588-2008 Fig. 28. These comparisons
   are necessary when the two data sets have the same grandmaster identity.

  Parameters:
   [in]   stepsRemovedA  Value of parentDS.stepsRemoved for A
   [in]   senderIdA      Sender's portIdentity for A
   [in]   receiverIdA    Receiver's portIdentity for A
   [in]   stepsRemovedB  Value of parentDS.stepsRemoved for B
   [in]   senderIdB      Sender's portIdentity for B
   [in]   receiverIdB    Receiver's portIdentity for B

  Return Value:
   -2 if 'B' is better by topology than 'A'
   -1 if 'B' is better than 'A'
    0 if data sets are equal (which is technically an error)
   +1 if 'A' is better than 'B'
   +2 if 'A' is better by topology than 'B'

  Notes:   Since this routine is called internally, it is assumed that all
         input pointers are valid and already verified.

*******************************************************************************/
Sint8T zl303xx_PtpV2BmcDataSetCompareSameGM(
      Uint16T stepsRemovedA, zl303xx_PortIdentity *senderIdA, zl303xx_PortIdentity *receiverIdA,
      Uint16T stepsRemovedB, zl303xx_PortIdentity *senderIdB, zl303xx_PortIdentity *receiverIdB)
{
   Sint32T cmp; /* Temporary result of a comparison */

   /* The grandmasterIdentity values are equal. */
   /*** Point X in flow diagram (Figure 27/28) ****/
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3,
         "BmcDataSetCompareSameGM: Figure 28 Algorithm (Part 2 - X)",
         0,0,0,0,0,0);

   /* Compare the stepsRemoved values. A server with more steps, indicates more
    * hops (distance) in the network and thus less accuracy.
    * The 5 possibilities are:
    *    A >> B (by more than 1) : B is better (outright)
    *    A > B  (by exactly 1)   : B is better (but compare Sender-A with Receiver-A for topology)
    *    A == B (equal steps)    : compare Sender-A with Sender-B
    *    A < B  (by exactly 1)   : A is better (but compare Sender-B with Receiver-B for topology)
    *    A << B (by more than 1) : A is better (outright) */

   /* stepsRemoved same */
   if (stepsRemovedA == stepsRemovedB)
   {
      /* Compare source (sender) clockIdentity values */
      cmp = ZL303XX_PTP_COMPARE_CLK_ID(senderIdA->clockIdentity, senderIdB->clockIdentity);

      if (cmp < 0)
	  {
        ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
       {
            strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by clockIdentity", ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
       }
       return ZL303XX_PTP_BMC_A_TOPOLOGICALLY_BETTER_THAN_B;  /* 'A' topologically better than 'B' */
	  }
      else if (cmp > 0)
	  {
        ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
       {
            strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by clockIdentity", ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
       }
       return ZL303XX_PTP_BMC_B_TOPOLOGICALLY_BETTER_THAN_A;  /* 'B' topologically better than 'A'  */
	  }

      /* source (sender) clockIdentity are the same so...
       * ..compare receive portNumber values */
      else
      {
         if (receiverIdA->portNumber < receiverIdB->portNumber)
		 {
          ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
         {
            strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by receive portNumber value", ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
         }
         return ZL303XX_PTP_BMC_A_TOPOLOGICALLY_BETTER_THAN_B;  /* 'A' topologically better than 'B'  */
		 }
         else if (receiverIdA->portNumber > receiverIdB->portNumber)
		 {
          ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
         {
            strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by receive portNumber value", ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
         }
         return ZL303XX_PTP_BMC_B_TOPOLOGICALLY_BETTER_THAN_A;  /* 'B' topologically better than 'A'  */
		 }
         else
            return ZL303XX_PTP_BMC_A_EQUALS_B;  /* same, an error, ignore */
      }
   }

   /* 'A' is more distanced than 'B' */
   else if (stepsRemovedA > stepsRemovedB)
   {
      if ((stepsRemovedA - stepsRemovedB) > 1)
      {
         /* 'A' much further away than 'B' */
          ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
         {
            strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by stepsRemoved", ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
         }
         return ZL303XX_PTP_BMC_B_BETTER_THAN_A;
      }
      else /* 'A' 1 hop further than 'B' */
      {
         cmp = ZL303XX_PTP_COMPARE_CLK_ID(receiverIdA->clockIdentity, senderIdA->clockIdentity);

         if (cmp < 0)
		 {
          ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
         {
            strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by stepsRemoved", ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
         }
         return ZL303XX_PTP_BMC_B_BETTER_THAN_A;
		 }
         else if (cmp > 0)
		 {
          ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
         {
            strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by stepsRemoved", ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
         }
         return ZL303XX_PTP_BMC_B_TOPOLOGICALLY_BETTER_THAN_A;  /* 'B' topologically better than 'A'  */
		 }
         else
         {
            /* Message from self, ignore */
            return ZL303XX_PTP_BMC_A_EQUALS_B;
         }
      }
   }

   /* 'B' is more distanced than 'A' */
   else if (stepsRemovedB > stepsRemovedA)
   {
      if ((stepsRemovedB - stepsRemovedA) > 1)
      {
         /* 'B' much further away than 'A' */
          ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
         {
            strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by stepsRemoved", ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
         }
         return ZL303XX_PTP_BMC_A_BETTER_THAN_B;
      }
      else /* 'B' 1 hop further than 'A' */
      {
         cmp = ZL303XX_PTP_COMPARE_CLK_ID(receiverIdB->clockIdentity, senderIdB->clockIdentity);

         if (cmp < 0)
		 {
          ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
         {
            strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by stepsRemoved",ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
         }
         return ZL303XX_PTP_BMC_A_BETTER_THAN_B;
		 }
         else if (cmp > 0)
		 {
          ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
         {
            strncpy(zl303xx_PtpDefaultBMCASelectionCriteria, "by stepsRemoved",ZL303XX_BMCA_SELECTION_STRING_SIZE - 1);
         }
         return ZL303XX_PTP_BMC_A_TOPOLOGICALLY_BETTER_THAN_B;  /* 'A' topologically better than 'B' */
		 }
         else
         {
            /* Message from self, an error, ignore */
            return ZL303XX_PTP_BMC_A_EQUALS_B;
         }
      }
   }

   /* Can never get to here as all the differences must have been resolved */
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 1,
         "BmcDataSetCompareSameGM: Figure 28 Algorithm (Part 2): NO BEST MASTER DETERMINED",
         0,0,0,0,0,0);

   return ZL303XX_PTP_BMC_A_EQUALS_B;  /* OR error, OR ignore */
}  /* END zl303xx_PtpV2BmcDataSetCompareSameGM */

/*

  Function Name:
    zl303xx_PtpConvertDefaultDsToCompareData

  Details:
   Collects the necessary data fields from the specified clock default data-set
   to be used to make a BMC comparison.

  Parameters:
   [in]  pDefaultDs     Pointer to a PTP clock's DefaultDs structure to
                              collect the comparison data from.

   [out] compareData    Pointer to the BMC comparison data structure to
                              data-fill from this received message.

  Return Value:
   None

  Notes:   Since this routine is called internally, it is assumed that all
         input pointers are valid and already verified.

 *****************************************************************************/
void zl303xx_PtpConvertDefaultDsToCompareData(
      zl303xx_DefaultDS *pDefaultDs,
      zl303xx_PtpBmcCompareDataS *compareData)
{
   /* Translate the necessary fields. */
   compareData->grandmasterPriority1 = pDefaultDs->priority1;
   compareData->grandmasterPriority2 = pDefaultDs->priority2;
   compareData->stepsRemoved = 0;

   OS_MEMCPY(compareData->grandmasterIdentity,
          pDefaultDs->clockIdentity,
          sizeof(compareData->grandmasterIdentity));

   compareData->grandmasterClockQuality = pDefaultDs->clockQuality;

   /* Received port is the local clock and port 0. */
   OS_MEMCPY(compareData->receivingPortIdentity.clockIdentity,
          pDefaultDs->clockIdentity,
          sizeof(compareData->receivingPortIdentity.clockIdentity));
   compareData->receivingPortIdentity.portNumber = 0;

   /* The foreignPortIdentity is the same as receivingPortIdentity */
   compareData->foreignPortIdentity = compareData->receivingPortIdentity;
}  /* END zl303xx_PtpConvertDefaultDsToCompareData */

/*

  Function Name:
    zl303xx_PtpConvertCompareDataToParentDs

  Details:
   Uses the necessary data fields from the comparison data set to construct
   a standard ParentDS.

  Parameters:
   [in]  compareData    Pointer to the BMC comparison data structure from
                              which to build a ParentDS.

   [in]  pParentDs      Pointer to the created PTP clock ParentDS structure.

  Return Value:
   None

  Notes:   Since this routine is called internally, it is assumed that all
         input pointers are valid and already verified.

 *****************************************************************************/
void zl303xx_PtpConvertCompareDataToParentDs(
      zl303xx_PtpBmcCompareDataS *compareData,
      zl303xx_ParentDS *pParentDs)
{
   /* Translate the necessary fields. */
   pParentDs->parentPortIdentity = compareData->foreignPortIdentity;
   OS_MEMCPY(pParentDs->grandmasterIdentity, compareData->grandmasterIdentity,
          sizeof(pParentDs->grandmasterIdentity));
   pParentDs->grandmasterClockQuality = compareData->grandmasterClockQuality;
   pParentDs->grandmasterPriority1 = compareData->grandmasterPriority1;
   pParentDs->grandmasterPriority2 = compareData->grandmasterPriority2;

   /* The following members are not supported so set to default values
    * as per IEEE-1588-2008: Section 8.2.3.4. */
   pParentDs->parentStats = ZL303XX_FALSE;
   pParentDs->observedParentOffsetScaledLogVariance = 0xFFFF;
   pParentDs->observedParentClockPhaseChangeRate = 0x7FFFFFFF;
}  /* END zl303xx_PtpConvertCompareDataToParentDs */

/****************   STATIC FUNCTION DEFINITIONS   ****************************/

/* Attempt to determine if either of these data sets originated from a
 * Virtual Port */
/* Determines that the data is from a VP if:
 * - RxClockID == TXClockID &&
 * - RxPortNumber != 0 &&
 * - TxPortNumber == 0 &&  (assumes sorcePortIdentity.portNumber is never 0 in the real world)
 * - RxClockID == GM ID      */
zl303xx_BooleanE zl303xx_PtpV2BmcDataSetIsVirtualPort(zl303xx_PtpBmcCompareDataS *compareData)
{
   zl303xx_BooleanE bVpComparison = ZL303XX_FALSE;

   if ((memcmp(compareData->receivingPortIdentity.clockIdentity,
               compareData->foreignPortIdentity.clockIdentity, 8) == 0) &&
       (memcmp(compareData->receivingPortIdentity.clockIdentity,
               compareData->grandmasterIdentity, 8) == 0) &&
       (compareData->receivingPortIdentity.portNumber != 0) &&
       (compareData->foreignPortIdentity.portNumber == 0))
   {
      bVpComparison = ZL303XX_TRUE;
   }

   return bVpComparison;
}

/* ZL303XX_TRACE_BMC_COMPARISON */
/** 

   Displays all of the data used by the BMC to compare 2 data set entities.
   (Intentionally used CAPS to resemble other TRACE calls).

  Parameters:
   [in]  modId          Module ID from which the trace is called. Used to
                              determine if the trace will be executed based on
                              the trace level.
   [in]  level          The trace level for the module specified.
   [in]  compareDataA   Comparison data for Clock A.
   [in]  compareDataB   Comparison data for Clock B.

  Return Value:
   Nothing

  Notes:
   Assumes all pointers are valid.

 *****************************************************************************/
void ZL303XX_TRACE_BMC_COMPARISON(
      Uint32T modId, Uint16T level,
      zl303xx_PtpBmcCompareDataS *compareDataA,
      zl303xx_PtpBmcCompareDataS *compareDataB)
{
   /* Unfortunate but necessary: since the TRACE output is so slow, this
    * function ends before the output is displayed and by that time any local
    * data is destroyed. So, keep a static string for each clockId needed. */
   static char gmIdA[] = "aa:bb:cc:dd:ee:ff:gg:hh\0";
   static char gmIdB[] = "a1:b1:c1:d1:e1:f1:g1:h1\0";
   static char rxIdA[] = "aa:bb:cc:dd:ee:ff:gg:hh\0";
   static char rxIdB[] = "a1:b1:c1:d1:e1:f1:g1:h1\0";
   static char txIdA[] = "aa:bb:cc:dd:ee:ff:gg:hh\0";
   static char txIdB[] = "a1:b1:c1:d1:e1:f1:g1:h1\0";

   /* Use the FILTER/TRACE_ALWAYS combination to avoid processing the string
    * if the trace level is set too low.    */
   ZL303XX_DEBUG_FILTER(modId, level)
   {
      /* Convert all necessary clockIdentities to strings. */
      ZL303XX_PTP_CLK_ID_TO_STRING(compareDataA->grandmasterIdentity, gmIdA);
      ZL303XX_PTP_CLK_ID_TO_STRING(compareDataB->grandmasterIdentity, gmIdB);
      ZL303XX_PTP_CLK_ID_TO_STRING(compareDataA->receivingPortIdentity.clockIdentity, rxIdA);
      ZL303XX_PTP_CLK_ID_TO_STRING(compareDataB->receivingPortIdentity.clockIdentity, rxIdB);
      ZL303XX_PTP_CLK_ID_TO_STRING(compareDataA->foreignPortIdentity.clockIdentity, txIdA);
      ZL303XX_PTP_CLK_ID_TO_STRING(compareDataB->foreignPortIdentity.clockIdentity, txIdB);

      /* The Announce Data. */
      ZL303XX_TRACE_ALWAYS("BMC Data Set Comparison", 0,0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("**********************************", 0,0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("   MEMBER                  ANNOUNCE-A                               ANNOUNCE-B", 0,0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("  ********                 ************                             ************", 0,0,0,0,0,0);

      ZL303XX_TRACE_ALWAYS(" grandmasterIdentity     : %-23s (port 0x%04X)    %-23s (port 0x%04X)",
            gmIdA, 0, /* port = 0 for a clock */
            gmIdB, 0, /* port = 0 for a clock */    0,0);

      ZL303XX_TRACE_ALWAYS(" grandmasterPriority1    : %-40d %-6d",
            compareDataA->grandmasterPriority1,
            compareDataB->grandmasterPriority1, 0,0,0,0);

      ZL303XX_TRACE_ALWAYS(" grandmasterClockQuality :", 0,0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("                 (Class) : %-40d %-6d",
            compareDataA->grandmasterClockQuality.clockClass,
            compareDataB->grandmasterClockQuality.clockClass, 0,0,0,0);
      ZL303XX_TRACE_ALWAYS("              (Accuracy) : 0x%-38X 0x%-4X",
            compareDataA->grandmasterClockQuality.clockAccuracy,
            compareDataB->grandmasterClockQuality.clockAccuracy, 0,0,0,0);
      ZL303XX_TRACE_ALWAYS("     (scaledLogVariance) : 0x%-38X 0x%-4X",
            compareDataA->grandmasterClockQuality.offsetScaledLogVariance,
            compareDataB->grandmasterClockQuality.offsetScaledLogVariance, 0,0,0,0);

      ZL303XX_TRACE_ALWAYS(" grandmasterPriority2    : %-40d %-6d",
            compareDataA->grandmasterPriority2,
            compareDataB->grandmasterPriority2, 0,0,0,0);

      ZL303XX_TRACE_ALWAYS(" stepsRemoved            : %-40d %-6d",
            compareDataA->stepsRemoved,
            compareDataB->stepsRemoved, 0,0,0,0);

      ZL303XX_TRACE_ALWAYS(" receivingPortIdentity   : %-23s (port 0x%04X)    %-23s (port 0x%04X)",
            rxIdA, compareDataA->receivingPortIdentity.portNumber,
            rxIdA, compareDataB->receivingPortIdentity.portNumber, 0,0);

      /* The Source Port Identity. */
      ZL303XX_TRACE_ALWAYS("  ", 0,0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("**********************************", 0,0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("   MEMBER                  HEADER-A                                 HEADER-B", 0,0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("  ********                 ************                             ************", 0,0,0,0,0,0);

      ZL303XX_TRACE_ALWAYS(" sourcePortIdentity      : %-23s (port 0x%04X)    %-23s (port 0x%04X)",
            txIdA, compareDataA->foreignPortIdentity.portNumber,
            txIdB, compareDataB->foreignPortIdentity.portNumber, 0,0);

      /* Blank Line for Separation. */
      ZL303XX_TRACE_ALWAYS("  ", 0,0,0,0,0,0);
   }
}

