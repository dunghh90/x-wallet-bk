

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Debug functions for the ZL3077X DPLL.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_RdWr.h"
#include "zl303xx_Dpll77x.h"
#include "zl303xx_AddressMap77x.h"
#include "zl303xx_DebugDpll77x.h"
#include <string.h>


/*****************   DEFINES   ************************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/
static zlStatusE zl303xx_DebugRefPrint(zl303xx_ParamsS *zl303xx_Params, Uint32T refId, Uint32T syncId);
static void zl303xx_DebugRefFreqPrint(Uint32T freqHz);
static void zl303xx_DebugRefInfoPrint(Uint32T freqHz, Uint32T failBits, Uint32T priority);
static zlStatusE zl303xx_DebugRefConfigPrint(zl303xx_ParamsS *zl303xx_Params, Uint32T refId, Uint32T syncId);
static zlStatusE zl303xx_DebugRefToSyncGet(zl303xx_ParamsS *zl303xx_Params, Uint32T refToSync[]);

/*****************   STATIC GLOBAL VARIABLES   ********************************/
static const char *dpllModeStr[] = {
      "Freerun",         /* 000 */
      "Holdover",        /* 001 */
      "Reference Lock",  /* 010 */
      "Automatic",       /* 011 */
      "NCO"              /* 100 */
};

/* Also used for CFM limits */
static const char *scmLimitStr[] = {
      "+/- 0.1%",  /* 000 */
      "+/- 0.5%",  /* 001 */
      "+/- 1%",    /* 010 */
      "+/- 2%",    /* 011 */
      "+/- 5%",    /* 100 */
      "+/- 10%",   /* 101 */
      "+/- 20%",   /* 110 */
      "+/- 50%"    /* 111 */
};

static const char *bandwidthStr[] = {
      "14 Hz",     /* 000 */
      "29 Hz",     /* 001 */
      "56 Hz",     /* 010 */
      "130 Hz",    /* 011 */
      "380 Hz",    /* 100 */
      "470 Hz",    /* 101 */
      "470 Hz",    /* 110 */
      "Variable",  /* 111 */
};

static const char *edgeSelStr[] = {
      "Rising edge",   /* 00 */
      "Falling edge",  /* 01 */
      "Low pulse",     /* 10 */
      "High pulse"     /* 11 */
};

static const char *dpllRefStr[] = {
      "REF0P",          /* ref 0 */
      "REF0N",          /* ref 1 */
      "REF1P",          /* ref 2 */
      "REF1N",          /* ref 3 */
      "REF2P",          /* ref 4 */
      "REF2N",          /* ref 5 */
      "REF3P",          /* ref 6 */
      "REF3N",          /* ref 7 */
      "REF4P",          /* ref 8 */
      "REF4N",          /* ref 9 */
};

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* zl303xx_DebugPllStatus77x */
/**
   Display PLL status information.

  Parameters:
   [in]   zl303xx_Params  Device instance parameter structure.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_DebugPllStatus77x(zl303xx_ParamsS *zl303xx_Params)
{
    zlStatusE status = ZL303XX_OK;
    zlStatusE stickyStatus = ZL303XX_ERROR;
    Uint32T count;
    Uint32T monStickyBits;
    Uint32T currRef;
    Uint32T pllId;
    Uint32T refToSync[ZLS3077X_REF_MAX];


    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(zl303xx_Params);
    }

    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

#if !defined ZLS30771_INCLUDED
    /* Read sticky registers needed by this function */
    if (status == ZL303XX_OK)
    {
        /* Read DPLL sticky bits */
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HOLDOVER_LOCK_STATUS_REG(pllId), &monStickyBits);
    }
#endif
    /* Lock all sticky registers */
    if (status == ZL303XX_OK)
    {
        stickyStatus = zl303xx_Dpll77xStickyLockSet(zl303xx_Params, ZLS3077X_STICKY_LOCK);
        status = stickyStatus;
    }

    /* Clear all sticky registers needed by this function */

    /* Clear reference monitor sticky bits */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Ref77xMonBitsClear(zl303xx_Params, (Uint32T)ZL303XX_INVALID);
    }

    /* Unlock sticky registers */
    if (stickyStatus == ZL303XX_OK)
    {
        stickyStatus = zl303xx_Dpll77xStickyLockSet(zl303xx_Params, ZLS3077X_STICKY_UNLOCK);
        status = (status == ZL303XX_OK) ? stickyStatus : status;
    }

    /* Clear DPLL holdover and lock status bits */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Write(zl303xx_Params, NULL, ZLS3077X_HOLDOVER_LOCK_STATUS_REG(pllId), 0);
    }

#if defined ZLS30771_INCLUDED
    /* Read sticky registers needed by this function */
    if (status == ZL303XX_OK)
    {
        /* Wait for 77x to update registers with new data */
        OS_TASK_DELAY(ZLS3077X_STICKY_UPDATE_DELAY_MS);

        /* Read DPLL sticky bits */

        /*************** alpha release note ***************/
        /* For the alpha release, we do NOT look at the sticky bits - we look
           at the instantaneous state. This code will change for the final
           release. */
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HOLDOVER_LOCK_STATUS_REG(pllId), &monStickyBits);
    }
#endif

    if (status == ZL303XX_OK)
    {
        status = zl303xx_DebugRefToSyncGet(zl303xx_Params, refToSync);
    }

    if (status == ZL303XX_OK)
    {
        printf("\n");

        for (count = 0; count < ZLS3077X_REF_MAX; count++)
        {
            if (status == ZL303XX_OK)
            {
                status = zl303xx_DebugRefPrint(zl303xx_Params, count, refToSync[count]);
            }
        }
    }

    /* This function will verify pllId (used below) */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xCurrRefGet(zl303xx_Params, &currRef);
    }
    if (status == ZL303XX_OK)
    {
        printf("\n");
        printf("DPLL %u: freqLock:%u hold:%u ref:",
             pllId,
             ZLS3077X_LOSS_OF_LOCK_STATUS_DPLL_GET(monStickyBits),
             ZLS3077X_HOLDOVER_STATUS_DPLL_GET(monStickyBits));

        if (currRef != (Uint32T)ZL303XX_INVALID)
        {
            printf("%u\n", currRef);
        }
        else
        {
            ZLS3077X_DpllHWModeE mode = 0;

            if ((status = zl303xx_Dpll77xModeGet(zl303xx_Params, &mode)) == ZL303XX_OK)
            {
                if (mode == ZLS3077X_DPLL_MODE_NCO)
                    printf(" NCO\n");
                else
                    printf(" NONE\n");
            }
            else
                printf(" MODE FAILURE=%d\n", status);
        }
    }

    return status;
}


/* zl303xx_DebugHwRefStatus77x */
/**
   Display hardware reference input status information.

  Parameters:
   [in]   zl303xx_Params  Device instance parameter structure.
   [in]   refId         Reference number to display status for (0-9). Any
                             other value will display info for all references.
                             If refId has an associated sync input, it will also
                             be displayed.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_DebugHwRefStatus77x(zl303xx_ParamsS *zl303xx_Params, Uint32T refId)
{
    zlStatusE status = ZL303XX_OK;
    zlStatusE stickyStatus = ZL303XX_ERROR;
    Uint32T start;
    Uint32T end;
    Uint32T count;
    Uint32T refToSync[ZLS3077X_REF_MAX];


    if (status == ZL303XX_OK)
    {
        if (refId < ZLS3077X_REF_MAX)
        {
            start = refId;
            end = refId;
        }
        else
        {
            start = 0;
            end = ZLS3077X_REF_MAX - 1;
        }

        status = zl303xx_DebugRefToSyncGet(zl303xx_Params, refToSync);
    }

    if (status == ZL303XX_OK)
    {
        printf("\n");
        for (count = start; count <= end; count++)
        {
            if (status == ZL303XX_OK)
            {
                status = zl303xx_DebugRefPrint(zl303xx_Params, count, refToSync[count]);
            }
        }
    }

    /* If only one reference was printed, print the sync if it exists */
    if (status == ZL303XX_OK)
    {
        if (start == end && refToSync[start] != (Uint32T)ZL303XX_INVALID)
        {
            status = zl303xx_DebugRefPrint(zl303xx_Params, refToSync[start], (Uint32T)ZL303XX_INVALID);
        }
    }

    /* Lock all sticky registers */
    if (status == ZL303XX_OK)
    {
        stickyStatus = zl303xx_Dpll77xStickyLockSet(zl303xx_Params, ZLS3077X_STICKY_LOCK);
        status = stickyStatus;
    }

    /* Clear reference monitor sticky bits */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Ref77xMonBitsClear(zl303xx_Params, refId);
    }

    /* Unlock sticky registers */
    if (stickyStatus == ZL303XX_OK)
    {
        stickyStatus = zl303xx_Dpll77xStickyLockSet(zl303xx_Params, ZLS3077X_STICKY_UNLOCK);
        status = (status == ZL303XX_OK) ? stickyStatus : status;
    }

    if (status == ZL303XX_OK)
    {
        /* Wait for 77X to update registers with new data */
        OS_TASK_DELAY(ZLS3077X_STICKY_UPDATE_DELAY_MS);

        printf("\n");
        for (count = start; count <= end; count++)
        {
            if (status == ZL303XX_OK)
            {
                status = zl303xx_DebugRefPrint(zl303xx_Params, count, refToSync[count]);
            }
        }
    }

    /* If only one reference was printed, print the sync if it exists */
    if (status == ZL303XX_OK)
    {
        if (start == end && refToSync[start] != (Uint32T)ZL303XX_INVALID)
        {
            status = zl303xx_DebugRefPrint(zl303xx_Params, refToSync[start], (Uint32T)ZL303XX_INVALID);
        }
    }

    return status;
}


/* zl303xx_DebugHwRefCfg77x */
/**
   Display hardware reference input configuration.

  Parameters:
   [in]   zl303xx_Params  Device instance parameter structure
   [in]   refId         Reference number to display configuration for (0-10).
                             Any other value will display all references. If
                             refId has an associated sync input, it will also
                             be displayed.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_DebugHwRefCfg77x(zl303xx_ParamsS *zl303xx_Params, Uint32T refId)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T start;
    Uint32T end;
    Uint32T count;
    Uint32T refToSync[ZLS3077X_REF_MAX];


    if (status == ZL303XX_OK)
    {
        if (refId < ZLS3077X_REF_MAX)
        {
            start = refId;
            end = refId;
        }
        else
        {
            start = 0;
            end = ZLS3077X_REF_MAX - 1;
        }

        status = zl303xx_DebugRefToSyncGet(zl303xx_Params, refToSync);
    }

    if (status == ZL303XX_OK)
    {
        for (count = start; count <= end; count++)
        {
            if (status == ZL303XX_OK)
            {
            status = zl303xx_DebugRefConfigPrint(zl303xx_Params, count, refToSync[count]);
            }
        }
    }

    /* If only one reference was printed, print the sync if it exists */
    if (status == ZL303XX_OK)
    {
        if (start == end && refToSync[start] != (Uint32T)ZL303XX_INVALID)
        {
            status = zl303xx_DebugRefConfigPrint(zl303xx_Params, refToSync[start], refToSync[refToSync[start]]);
        }
    }

    return status;
}


/* zl303xx_DebugDpllConfig77x */
/**
   Display hardware DPLL configuration.

  Parameters:
   [in]   zl303xx_Params   Device instance parameter structure.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_DebugDpllConfig77x(zl303xx_ParamsS *zl303xx_Params)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T pllId = 0;
    Uint32T currRef = -1;
    Uint32T bw;
    Uint32T tieClear;
    Uint32T psl;
    Uint32T pir;
    Uint32T edge;
    ZLS3077X_DpllHWModeE mode;
    Uint32T data;


    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(zl303xx_Params);
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xModeGet(zl303xx_Params, &mode);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xCurrRefGet(zl303xx_Params, &currRef);
    }

    /* Get DPLL mailbox data*/
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetupMailboxForRead(zl303xx_Params, ZLS3077X_MB_DPLL, pllId);
    }
    if( status == ZL303XX_OK )
    {
        status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_BW_FIXED_REG, &data);
    }
    if( status == ZL303XX_OK )
    {
        bw = ZLS3077X_DPLL_BANDWIDTH(data);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_RANGE_REG, &data);
    }
    if (status == ZL303XX_OK)
    {
        pir = ZLS3077X_DPLL_PULL_IN_GET(data);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_CONFIG_REG, &data);
    }
    if (status == ZL303XX_OK)
    {
        edge = ZLS3077X_DPLL_EDGE_SEL(data);
    }
    /* End mailbox data read */

    if( status == ZL303XX_OK )
    {
        status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_PSL_REG, &psl);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLL_TIE_CLEAR_REG(pllId), &data);
    }
    if (status == ZL303XX_OK)
    {
        tieClear = ZLS3077X_DPLL_TIE_CLEAR_EN(data);
    }

    if (status == ZL303XX_OK)
    {
        printf("\n");
        printf("DPLL %u Configuration:\n", pllId);
        printf("   Mode:               %s (%d)\n" \
               "   Current Ref:        ",
                dpllModeStr[mode], mode);
        if (currRef != (Uint32T)ZL303XX_INVALID)
        {
            printf("%s (%u)\n", dpllRefStr[currRef], currRef);
        }
        else
        {
            printf("NONE\n");
        }
        printf("   Bandwidth:          %s (%u)\n" \
               "   TIE clear enabled:  %s (%u)\n" \
               "   Phase slope limit:  %s %u ns/s\n"  \
               "   Pull-in range:      %u.%u ppm\n"     \
               "   Edge select:        %s (%u)\n",
               bandwidthStr[bw], bw,
               ZL303XX_BOOL_TO_STR(tieClear), tieClear,
               (psl == 0) ? "Unlimited -" : "", psl,
               pir/10, pir%10,
               edgeSelStr[edge], edge);
    }

    return status;
}


/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/* zl303xx_DebugRefPrint */
/** 

   Prints reference input status information to the logging file.

  Parameters:
   [in]   zl303xx_Params   Device instance parameter structure.
   [in]   refId          Reference input to print status for.
   [in]   syncId         Sync input associated with refId. Pass in
                              ZL303XX_INVALID if no sync input exists.

*******************************************************************************/
static zlStatusE zl303xx_DebugRefPrint( zl303xx_ParamsS *zl303xx_Params, Uint32T refId,
                                            Uint32T syncId)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T pllId;
    Uint32T freqHz;
    Uint32T failBits;
    Uint32T priority;
    Uint32T data;


    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Ref77xFreqDetectedGet(zl303xx_Params, refId, &freqHz);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REF_MON_FAIL_REG(refId), &failBits);
        failBits &= ZLS3077X_REF_MON_FAIL_MASK;
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetupMailboxForRead(zl303xx_Params, ZLS3077X_MB_DPLL, pllId);
    }
    if( status == ZL303XX_OK )
    {
        switch( refId )
        {
            case 0:
            case 1:
                status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_0_REG, &data);
                break;
            case 2:
            case 3:
                status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_1_REG, &data);
                break;
            case 4:
            case 5:
                status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_2_REG, &data);
                break;
            case 6:
            case 7:
                status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_3_REG, &data);
                break;
            case 8:
            case 9:
                status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_4_REG, &data);
                break;
            default:
                break;
        }
    }
    if( status == ZL303XX_OK )
    {
        priority = ZLS3077X_DPLL_REF_PRIORITY_GET(data, refId);
    }
    if (status == ZL303XX_OK)
    {
        printf("Ref %s(%d)", dpllRefStr[refId], refId);
        if (syncId != (Uint32T)ZL303XX_INVALID)
        {
            printf("+%s(%d): ", dpllRefStr[syncId], syncId);
        }
        else
        {
            printf("         : ");
        }

        zl303xx_DebugRefInfoPrint(freqHz, failBits, priority);
        printf("\n");
    }

   return status;
}


/* zl303xx_DebugRefFreqPrint */
/** 

   Prints a reference's detected frequency (in Hz, kHz, or MHz), and right-pads
   with spaces to 14 characters (to fit "999.999999 MHz"). No newlines are
   printed in this function.

  Parameters:
   [in]   freqHz  Frequency in Hz.

*******************************************************************************/
static void zl303xx_DebugRefFreqPrint(Uint32T freqHz)
{
    const char *units = NULL;
    Uint32T divider = 0;
    Uint32T maxdec = 0;
    Uint32T whole;
    Uint32T frac;
    Uint32T count = 0;


    if (freqHz < 1000)
    {
        units = "Hz";
        divider = 1;
        maxdec = 0;
    }
    else if (freqHz < 1000000)
    {
        units = "kHz";
        divider = 1000;
        maxdec = 3;
    }
    else if (freqHz < 1000000000)
    {
        units = "MHz";
        divider = 1000000;
        maxdec = 6;
    }

    whole = freqHz / divider;
    frac = freqHz % divider;

    count += snprintf(NULL, 0, "%u", whole); /* Get character count */
    printf("%u", whole);

    if (frac > 0)
    {
        while (frac % 10 == 0)  /* Strip trailing zeros of fractional part */
        {
            frac /= 10;
            maxdec--;
        }

        count += snprintf(NULL, 0, ".%0*u", maxdec, frac); /* Get character count */
        printf(".%0*u", maxdec, frac);
    }

    /* 14 chars total, minus 1 for the space between number and units */
    printf(" %-*s", 14 - 1 - count, units);
}


/* zl303xx_DebugRefInfoPrint */
/** 

   Prints a reference's detected frequency (in Hz, kHz, or MHz), overall status,
   and individual fail bits. No newlines are printed in this function.

  Parameters:
   [in]   freqHz    Frequency in Hz.
   [in]   failBits  Failure bits from the ref_mon_fail_x register.
   [in]   priority  Priority of this reference

*******************************************************************************/
static void zl303xx_DebugRefInfoPrint(Uint32T freqHz, Uint32T failBits, Uint32T priority)
{
    char priorityStr[16];


    memset(&priorityStr, 0, 16);
    if (priority == ZLS3077X_DPLL_REF_PRIORITY_NEVER_LOCK)
        snprintf(priorityStr, 16, "never");
    else
        snprintf(priorityStr, 16, "%u", priority);

    zl303xx_DebugRefFreqPrint(freqHz);
    printf(" %s -- Flags: LOS:%u SCM:%u CFM:%u GST:%u PFM:%u -- Priority: %s",
          failBits ? "[FAIL]" : "[ OK ]",
          !!(failBits & ZLS3077X_REF_MON_FAIL_LOS),
          !!(failBits & ZLS3077X_REF_MON_FAIL_SCM),
          !!(failBits & ZLS3077X_REF_MON_FAIL_CFM),
          !!(failBits & ZLS3077X_REF_MON_FAIL_GST),
          !!(failBits & ZLS3077X_REF_MON_FAIL_PFM),
          priorityStr);
}


/* zl303xx_DebugRefConfigPrint */
/** 

   Prints a reference input's configuration to the logging file.

  Parameters:
   [in]   zl303xx_Params  Device instance parameter structure.
   [in]   refId         Reference input to print the configuration for.
   [in]   syncId        Sync input associated with refId, or ZL303XX_INVALID
                             if none exists.

*******************************************************************************/
static zlStatusE zl303xx_DebugRefConfigPrint(zl303xx_ParamsS *zl303xx_Params,
                                             Uint32T refId, Uint32T syncId)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T pllId;
    Uint32T freqHz, gstDisqual, gstQual = 0;
    Uint32T pfmDisqual;
    Uint32T pfmQual;
    Uint32T scmLimit, cfmLimit, diff, priority = 0;
    Uint32T data;
    Uint32T refPfmCtrl;
    Uint32T resolution;


    #define ZL303XX_PRIORITY_STR_SIZE (32)
    char priorityStr[ZL303XX_PRIORITY_STR_SIZE];

    memset(&priorityStr, 0, ZL303XX_PRIORITY_STR_SIZE);

    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Ref77xFreqDetectedGet(zl303xx_Params, refId, &freqHz);
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetupMailboxForRead(zl303xx_Params, ZLS3077X_MB_ref, refId);
    }
    if( status == ZL303XX_OK )
    {
        status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_GST_DISQUAL_REG, &gstDisqual);
    }
    if( status == ZL303XX_OK )
    {
        status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_GST_QUAL_REG, &gstQual);
    }
    if( status == ZL303XX_OK )
    {
        status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_SCM_REG, &data);
    }
    if( status == ZL303XX_OK )
    {
        scmLimit =  ZLS3077X_SCM_LIMIT_GET(data);
    }
    if( status == ZL303XX_OK )
    {
        status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_CFM_REG, &data);
    }
    if( status == ZL303XX_OK )
    {
        cfmLimit =  ZLS3077X_CFM_LIMIT_GET(data);
    }
    if( status == ZL303XX_OK )
    {
        status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_PFM_CTRL_REG, &refPfmCtrl);
    }
    if( status == ZL303XX_OK )
    {
        if( ZLS3077X_PFM_CTRL_RESOLUTION_GET_REG(refPfmCtrl) == 0 )
        {
            resolution = 5; /* 0.005ppm * 1000 */
        }
        else
        {
            resolution = 50; /* 0.05ppm * 1000 */
        }
    }
    if( status == ZL303XX_OK )
    {
        status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_PFM_DISQUALIFY_REG, &pfmDisqual);
    }
    if( status == ZL303XX_OK )
    {
        status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_PFM_QUALIFY_REG, &pfmQual);
    }
    if (status == ZL303XX_OK)
    {
        if (refId < ZLS3077X_REF_MAX)
        {
            status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_CONFIG_REG, &data);
            diff = ZLS3077X_DIFF_EN_GET(data);
        }
        else
        {
            diff = 0;
        }
    }
    /* Read reference priority bits (based on given pll) */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetupMailboxForRead(zl303xx_Params, ZLS3077X_MB_DPLL, pllId);
    }
    if( status == ZL303XX_OK )
    {
        switch( refId )
        {
            case 0:
            case 1:
                status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_0_REG, &data);
                break;
            case 2:
            case 3:
                status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_1_REG, &data);
                break;
            case 4:
            case 5:
                status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_2_REG, &data);
                break;
            case 6:
            case 7:
                status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_3_REG, &data);
                break;
            case 8:
            case 9:
                status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_4_REG, &data);
                break;
            default:

                status = ZL303XX_ERROR;
                break;
        }
    }
    if( status == ZL303XX_OK )
    {
        priority = ZLS3077X_DPLL_REF_PRIORITY_GET(data, refId);

        if (priority == ZLS3077X_DPLL_REF_PRIORITY_NEVER_LOCK)
        {
            snprintf(priorityStr, ZL303XX_PRIORITY_STR_SIZE, "never (%u)", priority);
        }
        else
        {
            snprintf(priorityStr, ZL303XX_PRIORITY_STR_SIZE, "%u", priority);
        }
    }

    if (status == ZL303XX_OK)
    {
        printf("\n");
        printf("Ref %s (%u) Configuration\n", dpllRefStr[refId], refId);
        printf("   Frequency:         ");
        zl303xx_DebugRefFreqPrint(freqHz);
        printf("\n" \
                "   GST Disqual Time:  %u ms\n" \
                "   GST Qualify Mult:  %u ms\n" \
                "   SCM Limit:         %s (%u)\n" \
                "   CFM Limit:         %s (%u)\n" \
                "   PFM Disqual:       %u ppm\n" \
                "   PFM Qualify:       %u ppm\n" \
                "   Diff. Input:       %s (%u)\n" \
                "   Priority:          %s\n",
                gstDisqual * 10,
                gstQual * 10,
                scmLimitStr[scmLimit], scmLimit,
                scmLimitStr[cfmLimit], cfmLimit,
                pfmDisqual * resolution / 1000,
                pfmQual * resolution / 1000,
                ZL303XX_BOOL_TO_STR(diff), diff,
                priorityStr);

        if (syncId != (Uint32T)ZL303XX_INVALID)
        {
            printf("   Sync Input:        Ref %s (%u)\n", dpllRefStr[syncId], syncId);
        }
    }
    else
    {
        printf("   Error %d querying reference configuration.\n", status);
    }

    return status;
}


/* zl303xx_DebugRefToSyncGet */
/**
   Returns an array that associates reference with sync inputs. syncArray[refId]
   will be the sync input associated with refId, or ZL303XX_INVALID if none
   exists.

  Parameters:
   [in]   zl303xx_Params  Device instance parameter structure.
   [out]  refToSync     Array that associates references with sync inputs.

*******************************************************************************/
static zlStatusE zl303xx_DebugRefToSyncGet(zl303xx_ParamsS *zl303xx_Params,
                                           Uint32T refToSync[])
{
    zlStatusE status = ZL303XX_OK;
    Uint32T refId;
    Uint32T data;
    ZLS3077X_RefSynchPairModeE refSyncMode;


    if (status == ZL303XX_OK)
    {
        for (refId = 0; refId < ZLS3077X_REF_MAX; refId++)
        {
            status = zl303xx_Dpll77xSetupMailboxForRead(zl303xx_Params, ZLS3077X_MB_ref, refId);

            if( status == ZL303XX_OK )
            {
                status =  zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_SYNC_REG, &data);
            }
            if( status == ZL303XX_OK )
            {
                refToSync[refId] = (Uint32T)ZL303XX_INVALID;

                refSyncMode = ZLS3077X_REF_CLK_SYNC_MODE_GET(data);

                if( refSyncMode == ZLS3077X_RSPM_enabled )
                {
                    refToSync[refId] = ZLS3077X_REF_CLK_SYNC_PAIR_GET(data);
                }
            }
        }
    }

    return status;
}


/* zl303xx_DebugPrintRefMailbox77x */
/**
   Prints the given mailbox

  Parameters:
   [in]   zl303xx_Params   Device instance parameter structure.
   [in]   mb             The mailbox type
   [in]   mbNum          The mailbox number

*******************************************************************************/
zlStatusE zl303xx_DebugPrintMailbox77x(zl303xx_ParamsS *zl303xx_Params,
                                        ZLS3077X_MailboxE mb, Uint32T mbNum)
{
    zlStatusE status = ZL303XX_OK;


    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(zl303xx_Params);
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetupMailboxForRead(zl303xx_Params, mb, mbNum);
    }
    if (status == ZL303XX_OK)
    {
        switch( mb )
        {
            case ZLS3077X_MB_ref:
                {
                    zl303xx77xRefMailBoxS mbData;
                    zl303xx77xRefMailBoxS *p = &mbData;

                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_FREQ_BASE_REG,        &(p->ref_freq_base));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_FREQ_MULT_REG,        &(p->ref_freq_mult));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_RATIO_M_REG,          &(p->ref_ratio_m));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_RATIO_N_REG,          &(p->ref_ratio_n));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_CONFIG_REG,           &(p->ref_config));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_SCM_REG,              &(p->ref_scm));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_CFM_REG,              &(p->ref_cfm));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_GST_DISQUAL_REG,      &(p->ref_gst_disqual));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_GST_QUAL_REG,         &(p->ref_gst_qual));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_PFM_CTRL_REG,         &(p->ref_pfm_ctrl));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_PFM_DISQUALIFY_REG,   &(p->ref_pfm_disqualify));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_PFM_QUALIFY_REG,      &(p->ref_pfm_qualify));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_PFM_PERIOD_REG,       &(p->ref_pfm_period));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_PFM_FILTER_LIMIT_REG, &(p->ref_pfm_filter_limit));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_SYNC_REG,             &(p->ref_sync));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_SYNC_MISC_REG,        &(p->ref_sync_misc));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_SYNC_OFFSET_COMP_REG, &(p->ref_sync_offset_comp));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_PHASE_OFFSET_COMP_REG,&(p->ref_phase_offset_compensation));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_SCM_FINE_REG,         &(p->ref_scm_fine));

                    printf( "Ref mailbox: %d\n", mbNum );
                    printf( "  p->ref_freq_base:                %3d  (0x%x)\n" , p->ref_freq_base        , p->ref_freq_base        );
                    printf( "  p->ref_freq_mult:                %3d  (0x%x)\n" , p->ref_freq_mult        , p->ref_freq_mult        );
                    printf( "  p->ref_ratio_m:                  %3d  (0x%x)\n" , p->ref_ratio_m          , p->ref_ratio_m          );
                    printf( "  p->ref_ratio_n:                  %3d  (0x%x)\n" , p->ref_ratio_n          , p->ref_ratio_n          );
                    printf( "  p->ref_config:                   %3d  (0x%x)\n" , p->ref_config           , p->ref_config           );
                    printf( "  p->ref_scm:                      %3d  (0x%x)\n" , p->ref_scm              , p->ref_scm              );
                    printf( "  p->ref_cfm:                      %3d  (0x%x)\n" , p->ref_cfm              , p->ref_cfm              );
                    printf( "  p->ref_gst_disqual:              %3d  (0x%x)\n" , p->ref_gst_disqual      , p->ref_gst_disqual      );
                    printf( "  p->ref_gst_qual:                 %3d  (0x%x)\n" , p->ref_gst_qual         , p->ref_gst_qual         );
                    printf( "  p->ref_pfm_ctrl:                 %3d  (0x%x)\n" , p->ref_pfm_ctrl         , p->ref_pfm_ctrl         );
                    printf( "  p->ref_pfm_disqualify:           %3d  (0x%x)\n" , p->ref_pfm_disqualify   , p->ref_pfm_disqualify   );
                    printf( "  p->ref_pfm_qualify:              %3d  (0x%x)\n" , p->ref_pfm_qualify      , p->ref_pfm_qualify      );
                    printf( "  p->ref_pfm_period:               %3d  (0x%x)\n" , p->ref_pfm_period       , p->ref_pfm_period       );
                    printf( "  p->ref_pfm_filter_limit:         %3d  (0x%x)\n" , p->ref_pfm_filter_limit , p->ref_pfm_filter_limit );
                    printf( "  p->ref_sync:                     %3d  (0x%x)\n" , p->ref_sync             , p->ref_sync             );
                    printf( "  p->ref_sync_misc:                %3d  (0x%x)\n" , p->ref_sync_misc        , p->ref_sync_misc        );
                    printf( "  p->ref_sync_offset_comp:         %3d  (0x%x)\n" , p->ref_sync_offset_comp , p->ref_sync_offset_comp );
                    printf( "  p->ref_phase_offset_compensation:%3d  (0x%x)\n" , p->ref_phase_offset_compensation , p->ref_phase_offset_compensation );
                    printf( "  p->ref_scm_fine:                 %3d  (0x%x)\n" , p->ref_scm_fine         , p->ref_scm_fine         );
                    printf( "  " );
                }
                break;

            case ZLS3077X_MB_DPLL:
                {
                    zl303xx77xDpllMailboxS mbData;
                    zl303xx77xDpllMailboxS *p = &mbData;

                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_BW_FIXED_REG,              &(p->dpll_bw_fixed));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_BW_VAR_REG,                &(p->dpll_bw_var));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_CONFIG_REG,                &(p->dpll_config));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_PSL_REG,                   &(p->dpll_psl));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_PSL_MAX_PHASE_REG,         &(p->dpll_psl_max_phase));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_PSL_SCALING_REG,           &(p->dpll_psl_scaling));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_PSL_DECAY_REG,             &(p->dpll_psl_decay));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_RANGE_REG,                 &(p->dpll_range));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_SW_MASK_REG,           &(p->dpll_ref_sw_mask));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_HO_MASK_REG,           &(p->dpll_ref_ho_mask));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_DPER_SW_MASK_REG,          &(p->dpll_dper_sw_mask));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_DPER_HO_MASK_REG,          &(p->dpll_dper_ho_mask));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_0_REG,            &(p->dpll_ref_prio_0));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_1_REG,            &(p->dpll_ref_prio_1));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_2_REG,            &(p->dpll_ref_prio_2));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_3_REG,            &(p->dpll_ref_prio_3));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_4_REG,            &(p->dpll_ref_prio_4));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_DPER_PRIO_1_0_REG,         &(p->dpll_dper_prio_1_0));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_DPER_PRIO_3_2_REG,         &(p->dpll_dper_prio_3_2));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_HO_FILTER_REG,             &(p->dpll_ho_filter));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_HO_DELAY_REG,              &(p->dpll_ho_delay));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_SPLIT_XO_CONFIG_REG,       &(p->dpll_split_xo_config));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FAST_LOCK_CTRL_REG,        &(p->dpll_fast_lock_ctrl));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FAST_LOCK_PHASE_ERR_REG,   &(p->dpll_fast_lock_phase_err));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FAST_LOCK_FREQ_ERR_REG,    &(p->dpll_fast_lock_freq_err));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FAST_LOCK_IDEAL_TIME_REG,  &(p->dpll_fast_lock_ideal_time));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FAST_LOCK_NOTIFY_TIME_REG, &(p->dpll_fast_lock_notify_time));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FAST_LOCK_FCL_REG,         &(p->dpll_fast_lock_fcl));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FCL_REG,                   &(p->dpll_fcl));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_DAMPING_REG,               &(p->dpll_damping));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_PHASE_BAD_REG,             &(p->dpll_phase_bad));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_PHASE_GOOD_REG,            &(p->dpll_phase_good));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_GOOD_DURATION_REG,         &(p->dpll_duration_good));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_LOCK_DELAY_REG,            &(p->dpll_lock_delay));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_TIE_REG,                   &(p->dpll_tie));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_TIE_THRESH_REG,            &(p->dpll_tie_wr_thresh));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FP_FIRST_REALIGN_REG,      &(p->dpll_fp_first_realign));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FP_REALIGN_INTVL_REG,      &(p->dpll_fp_align_intvl));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FP_LOCK_THRESH_REG,        &(p->dpll_fp_lock_thresh));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_STEP_TIME_THRESH_REG,      &(p->dpll_step_time_thresh));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_STEP_TIME_RESO_REG,        &(p->dpll_step_time_reso));

                    printf( "DPLL mailbox: %d\n", mbNum );
                    printf( "  p->dpll_bw_fixed:            %3d  (0x%x)\n" , p->dpll_bw_fixed             , p->dpll_bw_fixed             );
                    printf( "  p->dpll_bw_var:              %3d  (0x%x)\n" , p->dpll_bw_var               , p->dpll_bw_var               );
                    printf( "  p->dpll_config:              %3d  (0x%x)\n" , p->dpll_config               , p->dpll_config               );
                    printf( "  p->dpll_psl:                 %3d  (0x%x)\n" , p->dpll_psl                  , p->dpll_psl                  );
                    printf( "  p->dpll_psl_max_phase:       %3d  (0x%x)\n" , p->dpll_psl_max_phase        , p->dpll_psl_max_phase        );
                    printf( "  p->dpll_psl_scaling:         %3d  (0x%x)\n" , p->dpll_psl_scaling          , p->dpll_psl_scaling          );
                    printf( "  p->dpll_psl_decay:           %3d  (0x%x)\n" , p->dpll_psl_decay            , p->dpll_psl_decay            );
                    printf( "  p->dpll_range:               %3d  (0x%x)\n" , p->dpll_range                , p->dpll_range                );
                    printf( "  p->dpll_ref_sw_mask:         %3d  (0x%x)\n" , p->dpll_ref_sw_mask          , p->dpll_ref_sw_mask          );
                    printf( "  p->dpll_ref_ho_mask:         %3d  (0x%x)\n" , p->dpll_ref_ho_mask          , p->dpll_ref_ho_mask          );
                    printf( "  p->dpll_dper_sw_mask:        %3d  (0x%x)\n" , p->dpll_dper_sw_mask         , p->dpll_dper_sw_mask         );
                    printf( "  p->dpll_dper_ho_mask:        %3d  (0x%x)\n" , p->dpll_dper_ho_mask         , p->dpll_dper_ho_mask         );
                    printf( "  p->dpll_ho_filter:           %3d  (0x%x)\n" , p->dpll_ref_prio_0           , p->dpll_ref_prio_0           );
                    printf( "  p->dpll_ho_delay:            %3d  (0x%x)\n" , p->dpll_ref_prio_1           , p->dpll_ref_prio_1           );
                    printf( "  p->dpll_priority_1_0:        %3d  (0x%x)\n" , p->dpll_ref_prio_2           , p->dpll_ref_prio_2           );
                    printf( "  p->dpll_priority_3_2:        %3d  (0x%x)\n" , p->dpll_ref_prio_3           , p->dpll_ref_prio_3           );
                    printf( "  p->dpll_priority_5_4:        %3d  (0x%x)\n" , p->dpll_ref_prio_4           , p->dpll_ref_prio_4           );
                    printf( "  p->dpll_priority_7_6:        %3d  (0x%x)\n" , p->dpll_dper_prio_1_0        , p->dpll_dper_prio_1_0        );
                    printf( "  p->dpll_priority_9_8:        %3d  (0x%x)\n" , p->dpll_dper_prio_3_2        , p->dpll_dper_prio_3_2        );
                    printf( "  p->dpll_lock_phase:          %3d  (0x%x)\n" , p->dpll_ho_filter            , p->dpll_ho_filter            );
                    printf( "  p->dpll_lock_period:         %3d  (0x%x)\n" , p->dpll_ho_delay             , p->dpll_ho_delay             );
                    printf( "  p->dpll_fast_lock_ctrl:      %3d  (0x%x)\n" , p->dpll_split_xo_config      , p->dpll_split_xo_config      );
                    printf( "  p->dpll_fast_lock_phase_err: %3d  (0x%x)\n" , p->dpll_fast_lock_ctrl       , p->dpll_fast_lock_ctrl       );
                    printf( "  p->dpll_fast_lock_freq_err:  %3d  (0x%x)\n" , p->dpll_fast_lock_phase_err  , p->dpll_fast_lock_phase_err  );
                    printf( "  p->dpll_damping:             %3d  (0x%x)\n" , p->dpll_fast_lock_freq_err   , p->dpll_fast_lock_freq_err   );
                    printf( "  p->dpll_dual_config:         %3d  (0x%x)\n" , p->dpll_fast_lock_ideal_time , p->dpll_fast_lock_ideal_time );
                    printf( "  p->dpll_pbo:                 %3d  (0x%x)\n" , p->dpll_fast_lock_notify_time, p->dpll_fast_lock_notify_time);
                    printf( "  p->dpll_pbo_jitter_thresh:   %3d  (0x%x)\n" , p->dpll_fast_lock_fcl        , p->dpll_fast_lock_fcl        );
                    printf( "  p->dpll_pbo_min_slope:       %3d  (0x%x)\n" , p->dpll_fcl                  , p->dpll_fcl                  );
                    printf( "  p->dpll_pbo_end_interval:    %3d  (0x%x)\n" , p->dpll_damping              , p->dpll_damping              );
                    printf( "  p->dpll_pbo_time_out:        %3d  (0x%x)\n" , p->dpll_phase_bad            , p->dpll_phase_bad            );
                    printf( "  p->dpll_lock_delay           %3d  (0x%x)\n" , p->dpll_phase_good           , p->dpll_phase_good           );
                    printf( "  p->dpll_fp_first_realign     %3d  (0x%x)\n" , p->dpll_duration_good        , p->dpll_duration_good        );
                    printf( "  p->dpll_fp_realign_intvl     %3d  (0x%x)\n" , p->dpll_lock_delay           , p->dpll_lock_delay           );
                    printf( "  p->dpll_fp_lock_thresh       %3d  (0x%x)\n" , p->dpll_tie                  , p->dpll_tie                  );
                    printf( "  p->dpll_fp_lock_thresh       %3d  (0x%x)\n" , p->dpll_tie_wr_thresh        , p->dpll_tie_wr_thresh        );
                    printf( "  p->dpll_fp_lock_thresh       %3d  (0x%x)\n" , p->dpll_fp_first_realign     , p->dpll_fp_first_realign     );
                    printf( "  p->dpll_fp_lock_thresh       %3d  (0x%x)\n" , p->dpll_fp_align_intvl       , p->dpll_fp_align_intvl       );
                    printf( "  p->dpll_fp_lock_thresh       %3d  (0x%x)\n" , p->dpll_fp_lock_thresh       , p->dpll_fp_lock_thresh       );
                    printf( "  p->dpll_fp_lock_thresh       %3d  (0x%x)\n" , p->dpll_step_time_thresh     , p->dpll_step_time_thresh     );
                    printf( "  p->dpll_fp_lock_thresh       %3d  (0x%x)\n" , p->dpll_step_time_reso       , p->dpll_step_time_reso       );
                }
                break;

            case ZLS3077X_MB_last:
            default:
                break;
        }
    }

    return status;
}

/*****************   END   ****************************************************/
