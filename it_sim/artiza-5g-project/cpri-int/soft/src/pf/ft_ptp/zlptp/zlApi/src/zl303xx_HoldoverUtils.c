

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     G8275.1 APR operating modes have holdover in-spec and out-of-spec thresholds
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Int64.h"
#include "zl303xx_Trace.h"
#include "zl303xx_HoldoverUtils.h"

#include "zl303xx_Apr.h"


#include <string.h>

/*****************   DEFINES     **********************************************/

/*****************   TYPES       **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   IMPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/



/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/


/** zl303xx_GetAccumulatedTIE
   Calculates the accumulated TIE (in ns) based on seconds in holdover.

  Parameters:
   [in]   hwParams                Pointer to the device parameter structure.
   [out]  holdoverQualityParamsP  Pointer to the holdoverQualityParams struct.

  Return Value:
   ZL303XX_OK, or status failure.


*******************************************************************************/
static zlStatusE zl303xx_GetAccumulatedTIE(void *hwParams, zl303xx_HoldoverQualityParamsS *holdoverQualityParamsP)
{
   #if !defined ONE_PPB
   #define ONE_PPB (100000000000LL)
   #endif

   zlStatusE status = ZL303XX_OK;

    ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 2,
                "zl303xx_GetAccumulatedTIE: ", 0,0,0,0,0,0);

   status = ZL303XX_CHECK_POINTERS(hwParams, holdoverQualityParamsP);
   if (status != ZL303XX_OK)
   {
       return status;
   }

   holdoverQualityParamsP->holdSecs =  holdoverQualityParamsP->holdoverIncrementCount * holdoverQualityParamsP->holdoverSecsPerIncrement;

   /* Calculate the TIE */
   holdoverQualityParamsP->holdoverTIEns = (Uint32T)( (Uint64T)( (Uint64T)( (Uint64T) holdoverQualityParamsP->holdSecs *
                                                      (Uint64T)( (Uint64T) holdoverQualityParamsP->currentHoldoverAccuracyPpt *
                                                               ONE_PPB ) ) / ONE_PPB ) /1000 );  /* ppb to ns */

   ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 3,
               "zl303xx_GetAccumulatedTIE: holdoverTIEns=%d = holdoverIncrementCount=%d * holdoverSecsPerIncrement=%d for holdSecs=%d using currentHoldoverAccuracyPpt=%d ",
               holdoverQualityParamsP->holdoverTIEns, holdoverQualityParamsP->holdoverIncrementCount, holdoverQualityParamsP->holdoverSecsPerIncrement, holdoverQualityParamsP->holdSecs, holdoverQualityParamsP->currentHoldoverAccuracyPpt, 0);

   return status;
}


/** zl303xx_GetHoldoverQuality
   Queries APR for all holdover data and calculates the accumulated TIE then
   decides IN or OUT of SPEC.

  Parameters:
   [in]   hwParams                Pointer to the device parameter structure.
   [out]  holdoverQualityParamsP  Pointer to the holdoverQualityParams struct.

  Return Value:
   ZL303XX_OK, or status failure.

  Notes:
   In packet mode without a valid Alg. avg. holdover, then holdoverQualityParamsP->holdoverQuality is
   HOLDOVER_QUALITY_UNKNOWN.

*******************************************************************************/
zlStatusE zl303xx_GetHoldoverQuality(void *hwParams, zl303xx_HoldoverQualityParamsS *holdoverQualityParamsP)
{
    zlStatusE status = ZL303XX_OK;

    ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 2,
                "zl303xx_GetHoldoverQuality: ", 0,0,0,0,0,0);

   status = ZL303XX_CHECK_POINTERS(hwParams, holdoverQualityParamsP);
   if (status == ZL303XX_OK)
   {
       memset(holdoverQualityParamsP, 0, sizeof(zl303xx_HoldoverQualityParamsS));

       if ((status = zl303xx_AprGetCurrentHoldoverQualityParameters(hwParams, holdoverQualityParamsP)) != ZL303XX_OK)
       {
           if (status != ZL303XX_OK)
           {
               ZL303XX_TRACE_ALWAYS("zl303xx_GetHoldoverQuality: zl303xx_AprGetCurrentHoldoverQualityParameters failure=%d", status, 0,0,0,0,0);
           }
       }

       /* Set quality UNKNOWN */
       holdoverQualityParamsP->holdoverQuality = HOLDOVER_QUALITY_UNKNOWN;

       if (status == ZL303XX_OK && holdoverQualityParamsP->currentAlgHoldover == 0 && holdoverQualityParamsP->devMode == ZL303XX_PACKET_MODE)
       {
           /* No valid holdover history yet in pkt mode */
           ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 2, "zl303xx_GetHoldoverQuality: Device=%p; No holdover history yet.", hwParams, 0,0,0,0,0);
       }

       if (status == ZL303XX_OK && (status = zl303xx_GetAccumulatedTIE(hwParams, holdoverQualityParamsP)) != ZL303XX_OK)
       {
           ZL303XX_TRACE_ALWAYS("zl303xx_GetHoldoverQuality: zl303xx_GetAccumulatatedTIE failure=%d", status, 0,0,0,0,0);
       }
       else
       {
           /* Decide if we are in spec. */
           if (holdoverQualityParamsP->holdoverTIEns <= holdoverQualityParamsP->inSpecHoldoverLimit)
           {
               /* IN spec */
               holdoverQualityParamsP->holdoverQuality = HOLDOVER_QUALITY_IN_SPEC;

               /* If tracable then syncE in spec. */
               if (holdoverQualityParamsP->syncEStatus == HOLDOVER_SYNCE_TRACEABLE)
               {
                   holdoverQualityParamsP->holdoverQuality = HOLDOVER_QUALITY_LOCKED_TO_SYNCE;
               }
           }
           else /* OUT of spec */
           {
               holdoverQualityParamsP->holdoverQuality = HOLDOVER_QUALITY_OUT_OF_SPEC;
           }
       }

       /* Declare quality UNKNOWN if no active server being used */
       if (status == ZL303XX_OK && holdoverQualityParamsP->aprState == ZL303XX_NO_ACTIVE_SERVER)
       {
           holdoverQualityParamsP->holdoverQuality = HOLDOVER_QUALITY_UNKNOWN;
       }

       /* Store any changes to accumulation (even if ZL303XX_INVALID_MODE) */
       if ((status = zl303xx_AprStoreHoldoverAccum(hwParams, holdoverQualityParamsP->holdoverQuality, holdoverQualityParamsP->holdoverTIEns,
                                                 holdoverQualityParamsP->holdoverIncrementCount, holdoverQualityParamsP->holdoverAction)) != ZL303XX_OK)
       {
           ZL303XX_TRACE_ALWAYS("zl303xx_GetHoldoverQuality: Call to zl303xx_AprStoreHoldoverAccum() failed= %d\n", status, 0,0,0,0,0);
       }
       else
           ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 2,
                       "zl303xx_GetHoldoverQuality: Returns: holdoverTIEns=%d gives holdoverQuality=%d (0=UNKNOWN, 1=IN, 2=SYNCE, 3=OUT) and currentAlgHoldover=%d  ",
                       holdoverQualityParamsP->holdoverTIEns, holdoverQualityParamsP->holdoverQuality, holdoverQualityParamsP->currentAlgHoldover, 0,0,0);
   }

   /* Decide on SyncE Tracability based on mode and state */
   if (status == ZL303XX_OK)
   {
       holdoverQualityParamsP->syncEStatus = HOLDOVER_SYNCE_NOT_TRACEABLE;

       if ( (holdoverQualityParamsP->aprState == ZL303XX_PHASE_LOCK_ACQUIRED || holdoverQualityParamsP->aprState == ZL303XX_FREQ_LOCK_ACQUIRED) &&
             (holdoverQualityParamsP->devMode == ZL303XX_ELECTRIC_MODE || holdoverQualityParamsP->devMode == ZL303XX_HYBRID_MODE) )
       {
           holdoverQualityParamsP->syncEStatus = HOLDOVER_SYNCE_TRACEABLE;
           ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 2, "zl303xx_GetHoldoverQuality: SYNCE TRACEABLE", 0,0,0,0,0,0);
       }
       else
           ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 2, "zl303xx_GetHoldoverQuality: SYNCE NOT TRACEABLE", 0,0,0,0,0,0);
   }

   if (status == ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 3,
            "zl303xx_GetHoldoverQuality: serverId=%u devMode=%u aprState=%u",
            holdoverQualityParamsP->serverId,
            holdoverQualityParamsP->devMode,
            holdoverQualityParamsP->aprState,
            0,0,0);
   }
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 1,
            "zl303xx_GetHoldoverQuality: failure status=%u",
            status, 0,0,0,0,0);
   }

   return status;
}

/* zl303xx_DpllHoldoverStateChange
   Clear or increment the accumulated TIE data.

  Parameters:
   [in]   hwParams          Pointer to the device parameter structure.
   [in]   holdoverAction    Action to take.

  Return Value:
   ZL303XX_OK, or failure status value.

*******************************************************************************/
Sint32T zl303xx_DpllHoldoverStateChange(void *hwParams, zl303xx_HoldoverActionE holdoverAction)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_HoldoverQualityParamsS holdoverQualityParams;

    ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 2,
                "zl303xx_DpllHoldoverStateChange: holdoverAction=%u (0=CLEAR, 1=INCREMENT, 2=FREEZE)", holdoverAction, 0,0,0,0,0);

    status = ZL303XX_CHECK_POINTER(hwParams);
    if (status == ZL303XX_OK)
    {
        if ((status = zl303xx_GetHoldoverQuality(hwParams, &holdoverQualityParams)) != ZL303XX_OK)
        {
            if (status == ZL303XX_TABLE_ENTRY_NOT_FOUND)   /* Electric mode or Master */
            {
                status = ZL303XX_OK;
            }
            else
            {
                ZL303XX_TRACE_ALWAYS("zl303xx_DpllHoldoverStateChange: Call to zl303xx_GetHoldoverQuality() failed= %d\n", status, 0,0,0,0,0);
            }
        }
    }

    /* Store (or clear) based on holdoverAction */
    if (status == ZL303XX_OK)
    {
        if (holdoverAction == ZL303XX_HO_CLEAR)
        {
            /* CLEAR holdover accumulation */
            if (holdoverQualityParams.aprState == ZL303XX_PHASE_LOCK_ACQUIRED)
            {
                /* Clear holdover debris */
                if ((status = zl303xx_AprClearHoldoverAccum(hwParams)) != ZL303XX_OK)
                {
                    ZL303XX_TRACE_ALWAYS("zl303xx_DpllHoldoverStateChange: Call to zl303xx_AprClearHoldoverAccum() failed= %d\n", status, 0,0,0,0,0);
                }
                else
                    ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 4, "zl303xx_DpllHoldoverStateChange: CLEARED Holdover accumulaton data", 0,0,0,0,0,0);
            }
        }
        else if (holdoverAction == ZL303XX_HO_INCREMENT)
        {
            holdoverQualityParams.holdoverAction = ZL303XX_HO_INCREMENT;
            /* Store action and ++holdoverIncrementCount holdover accumulation */
            if ((status = zl303xx_AprStoreHoldoverAccum(hwParams, holdoverQualityParams.holdoverQuality, holdoverQualityParams.holdoverTIEns,
                                                      ++holdoverQualityParams.holdoverIncrementCount, holdoverQualityParams.holdoverAction)) != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("zl303xx_DpllHoldoverStateChange: Call to zl303xx_AprStoreHoldoverAccum() failed= %d (INCREMENT)\n", status, 0,0,0,0,0);
            }
            else
                ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 4, "zl303xx_DpllHoldoverStateChange: INCREMENT holdoverIncrementCount=%u", holdoverQualityParams.holdoverIncrementCount, 0,0,0,0,0);
        }
        else if (holdoverAction == ZL303XX_HO_FREEZE)
        {
            holdoverQualityParams.holdoverAction = ZL303XX_HO_FREEZE;
            /* Store action */
            if ((status = zl303xx_AprStoreHoldoverAccum(hwParams, holdoverQualityParams.holdoverQuality, holdoverQualityParams.holdoverTIEns,
                                                      holdoverQualityParams.holdoverIncrementCount, holdoverQualityParams.holdoverAction)) != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("zl303xx_DpllHoldoverStateChange: Call to zl303xx_AprStoreHoldoverAccum() failed= %d (FREEZE)\n", status, 0,0,0,0,0);
            }
            else
                ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 4, "zl303xx_DpllHoldoverStateChange: FREEZE", 0,0,0,0,0,0);
        }
        else/* Catch errors */
        {
            status = ZL303XX_PARAMETER_INVALID;
            ZL303XX_TRACE_ALWAYS("zl303xx_DpllHoldoverStateChange: Unhandled holdoverAction=%d; status = %d\n", holdoverAction, status, 0,0,0,0);
        }

        if (status == ZL303XX_OK)
        {

            ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 2, "zl303xx_DpllHoldoverStateChange: Returns: syncEStatus=%u (0=NOT_Traceable, 1=Traceable); currentSyncEAccuracyPpt=%u; currentHoldoverCategory=%u",
                        holdoverQualityParams.syncEStatus, holdoverQualityParams.currentSyncEAccuracyPpt, holdoverQualityParams.currentHoldoverCategory, 0,0,0);

            ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 3, "zl303xx_DpllHoldoverStateChange: holdoverTIEns=%u currentAlgHoldover=%u holdoverQuality=%u currentHoldoverAccuracyPpt=%u inSpecHoldoverLimit=%u holdoverIncrementCount=%u",
                        holdoverQualityParams.holdoverTIEns, holdoverQualityParams.currentAlgHoldover, holdoverQualityParams.holdoverQuality,
                        holdoverQualityParams.currentHoldoverAccuracyPpt, holdoverQualityParams.inSpecHoldoverLimit, holdoverQualityParams.holdoverIncrementCount);

            ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 3, "                               holdSecs=%u holdoverAction=%u",
                         holdoverQualityParams.holdSecs, holdoverQualityParams.holdoverAction, 0,0,0,0);

            ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 3, "                               devMode=%u (PKY=0 HYB=1 ELEC=2) aprState=%u (FREQ_ACQ=0 FL=1 PL=2 H=3 RF=4 NAS=5 UK=6 MF=7 MH=8 SH=9)",
                        holdoverQualityParams.devMode, holdoverQualityParams.aprState, 0,0,0,0);
        }
    }

    return status;
}


