

/*****************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Module Description:
*     This module contains the interface for the Telecom Profile for Phase
*     Best-Master Clock (BMCA) comparison routines.
*
*******************************************************************************/

/****************   INCLUDE FILES   ******************************************/

#include "zl303xx_PtpBmcaG8275p1Profile.h"
#include "zl303xx_PtpStrings.h"

#include <string.h>

/****************   DEFINES     **********************************************/

/* Compare 2 PTP Clock Identities (part of the BMCA process). */
#define ZL303XX_PTP_COMPARE_CLK_ID(clkIdA, clkIdB)                              \
            memcmp(clkIdA, clkIdB, sizeof(zl303xx_ClockIdentity))

/* Formats a PTP Clock Identity into a string for printing. */
#define ZL303XX_PTP_CLK_ID_TO_STRING(clkId, string)                              \
            OS_SPRINTF(string, "%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx", \
                    clkId[0], clkId[1], clkId[2], clkId[3],                    \
                    clkId[4], clkId[5], clkId[6], clkId[7])

/****************   STATIC FUNCTION DECLARATIONS   ***************************/

Sint8T zl303xx_PtpBmcaG8275ProfileComparisonP1(
      zl303xx_PtpG8275ProfileCompareDataS *compareDataA,
      zl303xx_PtpG8275ProfileCompareDataS *compareDataB,
      zl303xx_PtpBmcG8275ProfileExtraDataS *extraConfig);
Sint8T zl303xx_PtpBmcaG8275ProfileComparisonP2(
      zl303xx_PtpG8275ProfileCompareDataS *compareDataA,
      zl303xx_PtpG8275ProfileCompareDataS *compareDataB,
      zl303xx_PtpBmcG8275ProfileExtraDataS *extraConfig);

/* Routine to display all of the data used in this BMC Data Set Comparison.
 * (Purposely used CAPS to resemble other TRACE routines. */
void ZL303XX_TRACE_G8275_BMC_COMPARISON(
      Uint32T modId, Uint16T level,
      zl303xx_PtpG8275ProfileCompareDataS *compareDataA,
      zl303xx_PtpG8275ProfileCompareDataS *compareDataB);

/****************   STATIC GLOBAL VARIABLES   ********************************/

/****************   EXTERNAL GLOBAL VARIABLES   ******************************/

/****************   EXPORTED GLOBAL VARIABLES   ******************************/

/****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/**
    zl303xx_PtpBmcaG8275ProfileComparison

  Details:
    Compares two data sets according to G.8275.1 (Figure 2 and Figure 3 of the
    G.8275.1 Standard).

  Parameters:
   [in]  compareDataA   Telecom Data Set A.
   [in]  compareDataB   Telecom Data Set B.
   [in]  extraConfig    Pointer to a structure containing extra settings that
                              affect the BMCA decision (For example, tie-breaker
                              rules).

  Return Value:
      2   A is better than B by topology (ZL303XX_PTP_BMC_A_TOPOLOGICALLY_BETTER_THAN_B)
      1   A is better than B             (ZL303XX_PTP_BMC_A_BETTER_THAN_B)
      0   data sets are equal            (ZL303XX_PTP_BMC_A_EQUALS_B)
     -1   B is better than A             (ZL303XX_PTP_BMC_B_BETTER_THAN_A)
     -2   B is better than A by topology (ZL303XX_PTP_BMC_B_TOPOLOGICALLY_BETTER_THAN_A)

  Notes:   If one of the input data set pointers is NULL then the other is selected.
        If both are NULL, then '0' (ZL303XX_PTP_BMC_A_EQUALS_B) is returned.

 *****************************************************************************/
Sint8T zl303xx_PtpBmcaG8275ProfileComparison(
      zl303xx_PtpG8275ProfileCompareDataS *compareDataA,
      zl303xx_PtpG8275ProfileCompareDataS *compareDataB,
      zl303xx_PtpBmcG8275ProfileExtraDataS *extraConfig)
{
   /* Set a default return value. */
   Sint8T compareResult = ZL303XX_PTP_BMC_A_EQUALS_B;

   /* Compare input pointers first. */
   if (compareDataA == NULL)
   {
      if (compareDataB == NULL)
      {
         return ZL303XX_PTP_BMC_A_EQUALS_B;
      }
      else
      {
         return ZL303XX_PTP_BMC_B_BETTER_THAN_A;
      }
   }

   /* compareDataA is good */
   else if (compareDataB == NULL)  /* compareDataB is not */
   {
      return ZL303XX_PTP_BMC_A_BETTER_THAN_B;
   }

   /* At this point, both pointers are good and if not we would have returned. */
   /****************************************************************************/

   /* Pass the data to Part 1 (Figure 2 of the G.8275.1 Standard) of the internal
    * comparison routine. */
   compareResult = zl303xx_PtpBmcaG8275ProfileComparisonP1(compareDataA, compareDataB, extraConfig);

   /* If stage 1 could not resolve the comparison, initiate Stage 2 (Figure 2 of
    * the G.8275.1 Standard) of the internal comparison routine. */
   if (compareResult == ZL303XX_PTP_BMC_A_EQUALS_B)
   {
      compareResult = zl303xx_PtpBmcaG8275ProfileComparisonP2(compareDataA, compareDataB, extraConfig);
   }

   /* Log the comparison if required. */
   ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_BMC, 2)
   {
      const char * bmcaResultStr = zl303xx_PtpBmcaResultToStr(compareResult);

      ZL303XX_TRACE_G8275_BMC_COMPARISON(ZL303XX_MOD_ID_PTP_BMC, 2, compareDataA, compareDataB);
      ZL303XX_TRACE_ALWAYS(
            "zl303xx_PtpBmcaG8275ProfileComparison: Result (%d): %s",
            compareResult, bmcaResultStr, 0,0,0,0);
   }

   return compareResult;
}  /* END zl303xx_PtpBmcaG8275ProfileComparison */

/* zl303xx_PtpBmcaG8275ProfileComparisonP1 */
/** 

   Performs Part 1 of the G.8275.1 data set comparison (Figure 2 of the G.8275.1
   Standard). If no 'best' is selected, then Part 2 (Figure 3 of the G.8275.1
   Standard) will automatically be called.

  Parameters:
   [in]  compareDataA   Pointer to the first G.8275.1 data set.
   [in]  compareDataB   Pointer to the second G.8275.1 data set.
   [in]  extraConfig    Pointer to a structure containing extra settings that
                              affect the BMCA decision (For example, tie-breaker
                              rules).

  Return Value:
      1   A is better than B             (ZL303XX_PTP_BMC_A_BETTER_THAN_B)
      0   data sets are equal            (ZL303XX_PTP_BMC_A_EQUALS_B)
     -1   B is better than A             (ZL303XX_PTP_BMC_B_BETTER_THAN_A)

  Notes:    Assumes this routine is called from the parent routine,
            zl303xx_PtpBmcaG8275p1ProfileComparison() so pointers are not NULL.

*******************************************************************************/
Sint8T zl303xx_PtpBmcaG8275ProfileComparisonP1(
      zl303xx_PtpG8275ProfileCompareDataS *compareDataA,
      zl303xx_PtpG8275ProfileCompareDataS *compareDataB,
      zl303xx_PtpBmcG8275ProfileExtraDataS *extraConfig)
{
   /* Warning Removal of an unused parameter. */
   if (extraConfig) {;}

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3,
         "zl303xx_PtpBmcaG8275ProfileComparisonP1: (Stage 1)",
         0,0,0,0,0,0);

   /* Compare GM Clock Quality values. */
   {
      /* clockClass. */
      if (compareDataA->grandmasterClockQuality.clockClass <
          compareDataB->grandmasterClockQuality.clockClass)
      {
         return ZL303XX_PTP_BMC_A_BETTER_THAN_B;
      }
      else if (compareDataA->grandmasterClockQuality.clockClass >
               compareDataB->grandmasterClockQuality.clockClass)
      {
         return ZL303XX_PTP_BMC_B_BETTER_THAN_A;
      }

      /* clockAccuracy. */
      if (compareDataA->grandmasterClockQuality.clockAccuracy <
          compareDataB->grandmasterClockQuality.clockAccuracy)
      {
         return ZL303XX_PTP_BMC_A_BETTER_THAN_B;
      }
      else if (compareDataA->grandmasterClockQuality.clockAccuracy >
               compareDataB->grandmasterClockQuality.clockAccuracy)
      {
         return ZL303XX_PTP_BMC_B_BETTER_THAN_A;
      }

      /* offsetScaledLogVariance. */
      if (compareDataA->grandmasterClockQuality.offsetScaledLogVariance <
          compareDataB->grandmasterClockQuality.offsetScaledLogVariance)
      {
         return ZL303XX_PTP_BMC_A_BETTER_THAN_B;
      }
      else if (compareDataA->grandmasterClockQuality.offsetScaledLogVariance >
               compareDataB->grandmasterClockQuality.offsetScaledLogVariance)
      {
         return ZL303XX_PTP_BMC_B_BETTER_THAN_A;
      }
   }

   /* Compare Priority values. */
   {
      /* GM priority2. */
      if (compareDataA->grandmasterPriority2 <
          compareDataB->grandmasterPriority2)
      {
         return ZL303XX_PTP_BMC_A_BETTER_THAN_B;
      }
      else if (compareDataA->grandmasterPriority2 >
               compareDataB->grandmasterPriority2)
      {
         return ZL303XX_PTP_BMC_B_BETTER_THAN_A;
      }

      /* localPriority. */
      if (compareDataA->localPriority <
          compareDataB->localPriority)
      {
         return ZL303XX_PTP_BMC_A_BETTER_THAN_B;
      }
      else if (compareDataA->localPriority >
               compareDataB->localPriority)
      {
         return ZL303XX_PTP_BMC_B_BETTER_THAN_A;
      }
   }

   /* For Master-Only clockClass on both A & B, go straight to Part 2. */
   if (compareDataA->grandmasterClockQuality.clockClass <= 127)
   {
      /* Returning A == B will trigger Stage 2 of the comparison to execute in
       * the calling routine.   */
      return ZL303XX_PTP_BMC_A_EQUALS_B;
   }
   /* For non Master-Only clockClass on both A & B, compare grandmasterIdentity. */
   else
   {
      Sint32T gmIdComparison = ZL303XX_PTP_COMPARE_CLK_ID(compareDataA->grandmasterIdentity,
                                                        compareDataB->grandmasterIdentity);

      if (gmIdComparison < 0)
      {
         return ZL303XX_PTP_BMC_A_BETTER_THAN_B;
      }
      else if (gmIdComparison > 0)
      {
         return ZL303XX_PTP_BMC_B_BETTER_THAN_A;
      }
      else
      {
         /* Returning A == B will trigger Stage 2 of the comparison to execute in
          * the calling routine.   */
         return ZL303XX_PTP_BMC_A_EQUALS_B;
      }
   }

   /* Can never get to here as all the differences must have been resolved above. */
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 1,
         "zl303xx_PtpBmcaG8275ProfileComparisonP1: (Stage 1): NO BEST MASTER DETERMINED",
         0,0,0,0,0,0);

   return ZL303XX_PTP_BMC_A_EQUALS_B;  /* OR error, OR ignore */

}  /* END zl303xx_PtpBmcaG8275ProfileComparisonP1 */

/* zl303xx_PtpBmcaG8275ProfileComparisonP2 */
/** 

   Performs Part 2 of the G.8275.1 data set comparison (Figure 3 of the G.8275.1
   Standard). If no 'best' is selected, then a configured tie-breaker setting
   will be used.

  Parameters:
   [in]  compareDataA   Pointer to the first G.8275.1 data set.
   [in]  compareDataB   Pointer to the second G.8275.1 data set.
   [in]  extraConfig    Pointer to a structure containing extra settings that
                              affect the BMCA decision (For example, tie-breaker
                              rules).

  Return Value:
      2   A is better than B by topology (ZL303XX_PTP_BMC_A_TOPOLOGICALLY_BETTER_THAN_B)
      1   A is better than B             (ZL303XX_PTP_BMC_A_BETTER_THAN_B)
      0   data sets are equal            (ZL303XX_PTP_BMC_A_EQUALS_B)
     -1   B is better than A             (ZL303XX_PTP_BMC_B_BETTER_THAN_A)
     -2   B is better than A by topology (ZL303XX_PTP_BMC_B_TOPOLOGICALLY_BETTER_THAN_A)

  Notes:    Assumes this routine is called from the parent routine,
            zl303xx_PtpBmcaG8275p1ProfileComparison() so pointers are not NULL.

*******************************************************************************/
Sint8T zl303xx_PtpBmcaG8275ProfileComparisonP2(
      zl303xx_PtpG8275ProfileCompareDataS *compareDataA,
      zl303xx_PtpG8275ProfileCompareDataS *compareDataB,
      zl303xx_PtpBmcG8275ProfileExtraDataS *extraConfig)
{
   Sint32T cmpResult; /* Temporary result of a comparison */

   /* Warning Removal of an unused parameter. */
   if (extraConfig) {;}

   /*** Starts at Point X in flow diagram (Figure 2/3) ****/
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3,
         "zl303xx_PtpBmcaG8275ProfileComparisonP2: Stage 2",
         0,0,0,0,0,0);

   /* Compare the stepsRemoved values. A server with more steps, indicates more
    * hops (distance) in the network and thus less accuracy.
    * The 5 possibilities are:
    *    A >> B (by more than 1) : B is better (outright)
    *    A > B  (by exactly 1)   : B is better (but compare Sender-A with Receiver-A for topology)
    *    A == B (equal steps)    : compare Sender-A with Sender-B
    *    A < B  (by exactly 1)   : A is better (but compare Sender-B with Receiver-B for topology)
    *    A << B (by more than 1) : A is better (outright) */

   /* Check if 'A' is more distanced than 'B' */
   {
      /* 'A' is much further away than 'B' */
      if ((compareDataA->stepsRemoved - compareDataB->stepsRemoved) > 1)
      {
         return ZL303XX_PTP_BMC_B_BETTER_THAN_A;
      }
      /* 'A' must be exactly 1 hop further than 'B' */
      else if (compareDataA->stepsRemoved > compareDataB->stepsRemoved)
      {
         cmpResult = ZL303XX_PTP_COMPARE_CLK_ID(compareDataA->rxPortIdentity.clockIdentity,
                                                compareDataA->txPortIdentity.clockIdentity);

         if (cmpResult < 0)
            return ZL303XX_PTP_BMC_B_BETTER_THAN_A;
         else if (cmpResult > 0)
            return ZL303XX_PTP_BMC_B_TOPOLOGICALLY_BETTER_THAN_A;  /* 'B' topologically better than 'A'  */

         /* ClockIdentity portion of the portIdentity was the same so compare portNumber. */
         else
         {
            if (compareDataA->rxPortIdentity.portNumber < compareDataA->txPortIdentity.portNumber)
               return ZL303XX_PTP_BMC_B_BETTER_THAN_A;
            else if (compareDataA->rxPortIdentity.portNumber > compareDataA->txPortIdentity.portNumber)
               return ZL303XX_PTP_BMC_B_TOPOLOGICALLY_BETTER_THAN_A;
            else
            {
               /* Message from self will be ignored */
               return ZL303XX_PTP_BMC_A_EQUALS_B;
            }
         }
      }
   }

   /* Check if 'B' is more distanced than 'A' */
   {
      /* 'B' much further away than 'A' */
      if ((compareDataB->stepsRemoved - compareDataA->stepsRemoved) > 1)
      {
         return ZL303XX_PTP_BMC_A_BETTER_THAN_B;
      }
      /* 'B' must be exactly 1 hop further than 'A' */
      else if (compareDataB->stepsRemoved > compareDataA->stepsRemoved)
      {
         cmpResult = ZL303XX_PTP_COMPARE_CLK_ID(compareDataB->rxPortIdentity.clockIdentity,
                                                compareDataB->txPortIdentity.clockIdentity);

         if (cmpResult < 0)
            return ZL303XX_PTP_BMC_A_BETTER_THAN_B;
         else if (cmpResult > 0)
            return ZL303XX_PTP_BMC_A_TOPOLOGICALLY_BETTER_THAN_B;  /* 'A' topologically better than 'B' */

         /* ClockIdentity portion of the portIdentity was the same so compare portNumber. */
         else
         {
            if (compareDataB->rxPortIdentity.portNumber < compareDataB->txPortIdentity.portNumber)
               return ZL303XX_PTP_BMC_A_BETTER_THAN_B;
            else if (compareDataB->rxPortIdentity.portNumber > compareDataB->txPortIdentity.portNumber)
               return ZL303XX_PTP_BMC_A_TOPOLOGICALLY_BETTER_THAN_B;
            else
            {
               /* Message from self will be ignored */
               return ZL303XX_PTP_BMC_A_EQUALS_B;
            }
         }
      }
   }

   /* stepsRemoved are the same (stepsRemovedA == stepsRemovedB) */
   {
      /* Compare source (sender) clockIdentity values */
      cmpResult = ZL303XX_PTP_COMPARE_CLK_ID(compareDataA->txPortIdentity.clockIdentity,
                                             compareDataB->txPortIdentity.clockIdentity);

      if (cmpResult < 0)
         return ZL303XX_PTP_BMC_A_TOPOLOGICALLY_BETTER_THAN_B;  /* 'A' topologically better than 'B' */
      else if (cmpResult > 0)
         return ZL303XX_PTP_BMC_B_TOPOLOGICALLY_BETTER_THAN_A;  /* 'B' topologically better than 'A'  */

      /* ClockIdentity portion of the portIdentity was the same so compare portNumber. */
      else
      {
         if (compareDataA->txPortIdentity.portNumber < compareDataB->txPortIdentity.portNumber)
            return ZL303XX_PTP_BMC_A_TOPOLOGICALLY_BETTER_THAN_B;
         else if (compareDataA->txPortIdentity.portNumber > compareDataB->txPortIdentity.portNumber)
            return ZL303XX_PTP_BMC_B_TOPOLOGICALLY_BETTER_THAN_A;

         /* source (sender) clockIdentity are the same so...
          * ..compare receive portNumber values */
         else
         {
            if (compareDataA->rxPortIdentity.portNumber < compareDataB->rxPortIdentity.portNumber)
               return ZL303XX_PTP_BMC_A_TOPOLOGICALLY_BETTER_THAN_B;  /* 'A' topologically better than 'B'  */
            else if (compareDataA->rxPortIdentity.portNumber > compareDataB->rxPortIdentity.portNumber)
               return ZL303XX_PTP_BMC_B_TOPOLOGICALLY_BETTER_THAN_A;  /* 'B' topologically better than 'A'  */
            else
               return ZL303XX_PTP_BMC_A_EQUALS_B;  /* same, an error, ignore */
         }
      }
   }

   /* Can never get to here as all the differences must have been resolved */
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 1,
         "zl303xx_PtpBmcaG8275ProfileComparisonP2: Stage 2: NO BEST MASTER DETERMINED",
         0,0,0,0,0,0);

   return ZL303XX_PTP_BMC_A_EQUALS_B;  /* OR error, OR ignore */
}  /* END zl303xx_PtpBmcaG8275ProfileComparisonP2 */

/* ZL303XX_TRACE_G8275_BMC_COMPARISON */
/** 

   Displays all of the data used by the G.8275 BMC to compare 2 data sets.
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
void ZL303XX_TRACE_G8275_BMC_COMPARISON(
      Uint32T modId, Uint16T level,
      zl303xx_PtpG8275ProfileCompareDataS *compareDataA,
      zl303xx_PtpG8275ProfileCompareDataS *compareDataB)
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
      ZL303XX_PTP_CLK_ID_TO_STRING(compareDataA->rxPortIdentity.clockIdentity, rxIdA);
      ZL303XX_PTP_CLK_ID_TO_STRING(compareDataB->rxPortIdentity.clockIdentity, rxIdB);
      ZL303XX_PTP_CLK_ID_TO_STRING(compareDataA->txPortIdentity.clockIdentity, txIdA);
      ZL303XX_PTP_CLK_ID_TO_STRING(compareDataB->txPortIdentity.clockIdentity, txIdB);

      /* The Announce Data. */
      ZL303XX_TRACE_ALWAYS("G.8275 BMC Data Set Comparison", 0,0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("**********************************", 0,0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("   MEMBER                  Data Set-A                               Data Set-B", 0,0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("  ********                 ************                             ************", 0,0,0,0,0,0);
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
      ZL303XX_TRACE_ALWAYS("    grandmasterPriority2 : %-40d %-6d",
            compareDataA->grandmasterPriority2,
            compareDataB->grandmasterPriority2, 0,0,0,0);
      ZL303XX_TRACE_ALWAYS("           localPriority : %-40d %-6d",
            compareDataA->localPriority,
            compareDataB->localPriority, 0,0,0,0);
      ZL303XX_TRACE_ALWAYS("     grandmasterIdentity : %-23s (port 0x%04X)    %-23s (port 0x%04X)",
            gmIdA, 0, /* port = 0 for a clock */
            gmIdB, 0, /* port = 0 for a clock */    0,0);

      ZL303XX_TRACE_ALWAYS("**********************************", 0,0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("            stepsRemoved : %-40d %-6d",
            compareDataA->stepsRemoved,
            compareDataB->stepsRemoved, 0,0,0,0);
      ZL303XX_TRACE_ALWAYS("      senderPortIdentity : %-23s (port 0x%04X)    %-23s (port 0x%04X)",
            txIdA, compareDataA->txPortIdentity.portNumber,
            txIdB, compareDataB->txPortIdentity.portNumber, 0,0);
      ZL303XX_TRACE_ALWAYS("    receiverPortIdentity : %-23s (port 0x%04X)    %-23s (port 0x%04X)",
            rxIdA, compareDataA->rxPortIdentity.portNumber,
            rxIdB, compareDataB->rxPortIdentity.portNumber, 0,0);

      /* Blank Line for Separation. */
      ZL303XX_TRACE_ALWAYS("  ", 0,0,0,0,0,0);
   }
}




