

/*****************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Module Description:
*     This module contains the interface for the Telecom Profile for Frequency
*     Best-Master Clock (BMCA) comparison routines.
*
*******************************************************************************/

/****************   INCLUDE FILES   ******************************************/

#include "zl303xx_PtpBmcaTelecomProfile.h"

#include <string.h>

/****************   DEFINES     **********************************************/

/****************   STATIC FUNCTION DECLARATIONS   ***************************/

/****************   STATIC GLOBAL VARIABLES   ********************************/

/****************   EXTERNAL GLOBAL VARIABLES   ******************************/

/****************   EXPORTED GLOBAL VARIABLES   ******************************/

/****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/**
    zl303xx_PtpTelecomProfileBmcaComparison

  Details:
    Compares two data sets according to G.8265.1.

  Parameters:
   [in]  telecomDataA   Telecom Data Set A (from Annc or DefaultDS).
   [in]  telecomDataB   Telecom Data Set B (from Annc or DefaultDS).
   [in]  revertiveEn    Revertive switching configuration of the associated
                              clock (required for comparison decisions).

  Return Value:
     -1   B is better than A
      0   data sets are equal
      1   A is better than B

  Notes:   If one of the input data set pointers is NULL then the other is selected.
         If both are NULL, then '0' is returned.

 *****************************************************************************/
Sint8T zl303xx_PtpTelecomProfileBmcaComparison(
      zl303xx_PtpTelecomCompareDataS *telecomDataA,
      zl303xx_PtpTelecomCompareDataS *telecomDataB,
      zl303xx_BooleanE revertiveEn)
{
   /* Compare input pointers first. */
   if (telecomDataA == NULL)
   {
      if (telecomDataB == NULL)
      {
         return 0;
      }
      else
      {
         return -1;
      }
   }
   else  /* telecomDataA is good */
   {
      if (telecomDataB == NULL)
      {
         return 1;
      }
      else
      {
         /* Structure for the 'extra' required configuration data. */
         zl303xx_PtpBmcTelecomProfileExtraDataS extraConfig;
         extraConfig.revertiveEn = revertiveEn;

         /* Pass the data to the internal comparison routine. */
         return zl303xx_PtpBmcaTelecomProfileComparison(telecomDataA, telecomDataB, &extraConfig);
      }
   }
}  /* END zl303xx_PtpTelecomProfileBmcaComparison */

/* zl303xx_PtpBmcaTelecomProfileComparison */
/** 

   Compares two data sets according to G.8265.1.

  Parameters:
   [in]  telecomDataA   Pointer to the first data set (contains PTSF flag,
                              grandmasterId, class, priority and steps removed).
   [in]  telecomDataB   Pointer to the second data set).
   [in]  extraConfig    Pointer to extra configuration data required by the
                              selection algorithm. In this case, the revertiveEn
                              setting is required.

  Return Value:
     -1   B is better than A
      0   data sets are equal
      1   A is better than B

*******************************************************************************/
Sint8T zl303xx_PtpBmcaTelecomProfileComparison(
      zl303xx_PtpTelecomCompareDataS *telecomDataA,
      zl303xx_PtpTelecomCompareDataS *telecomDataB,
      zl303xx_PtpBmcTelecomProfileExtraDataS *extraConfig)
{
   /* Compare PTSF values. */
   if (telecomDataA->ptsf && !telecomDataB->ptsf)
   {
      /*ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 2, "PTSF-A better than PTSF-B", 0,0,0,0,0,0);*/
      return -1;
   }
   else if (!telecomDataA->ptsf && telecomDataB->ptsf)
   {
      /*ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 2, "PTSF-B better than PTSF-A", 0,0,0,0,0,0);*/
      return 1;
   }

   /* PTSF values are both OK or both bad. If both PTSF values are bad, it's
    * still useful to sort these entries on the remaining criteria, but neither
    * will be selected because of the PTSF failure. */

   /* If both masters have the same grandmasterIdentity, don't use clockClass as
    * a selection criteria. This is to prevent a scenario with bidirectional
    * Announce messages when a master's clockClass degrades to the same level as
    * the current slave's. In this case the slave continues to send out the old
    * (better) clockClass in its Announce messages, and the master also goes to
    * slave mode because this clockClass in the message is now better than its own. */
   if (memcmp(telecomDataA->grandmasterIdentity,
              telecomDataB->grandmasterIdentity,
             sizeof(zl303xx_ClockIdentity)) != 0)
   {
      /* Compare PTP clock class values. */
      if (telecomDataA->clockClass < telecomDataB->clockClass)
      {
         /*ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 2, "clockClass-A better than clockClass-B", 0,0,0,0,0,0);*/
         return 1;
      }
      else if (telecomDataA->clockClass > telecomDataB->clockClass)
      {
         /*ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 2, "clockClass-B better than clockClass-A", 0,0,0,0,0,0);*/
         return -1;
      }
   }

   /* PTP clock class values are equal. */

   /* Only check the priority if revertive mode is used (as per ICAP test cases) */
   if (extraConfig->revertiveEn)
   {
      /* Compare Local Priority values. */
      if (telecomDataA->priority < telecomDataB->priority)
      {
         /*ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 2, "priority-A better than priority-B", 0,0,0,0,0,0);*/
         return 1;
      }
      else if (telecomDataA->priority > telecomDataB->priority)
      {
         /*ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 2, "priority-B better than priority-A", 0,0,0,0,0,0);*/
         return -1;
      }
   }

   /* Comparing stepsRemoved is not part of the G.8265.1 alternate BMCA, but it
    * helps the non-standard boundary clock operation to prevent cyclic timing paths. */
   if (telecomDataA->stepsRemoved < telecomDataB->stepsRemoved)
   {
      /*ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 2, "stepsRemoved-A better than stepsRemoved-B", 0,0,0,0,0,0);*/
      return 1;
   }
   else if (telecomDataA->stepsRemoved > telecomDataB->stepsRemoved)
   {
      /*ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 2, "stepsRemoved-B better than stepsRemoved-A", 0,0,0,0,0,0);*/
      return -1;
   }

   return 0;
}

