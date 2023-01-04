

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     ZL30771 DPLL functions and registers.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Int64.h"
#include "zl303xx_Error.h"
#include "zl303xx_Trace.h"
#include "zl303xx_AddressMap77x.h"
#include "zl303xx_DeviceSpec.h"
#include "zl303xx_RdWr.h"
#include "zl303xx_Macros.h"
#include "zl303xx_Dpll77x.h"
#include "zl303xx_Dpll771.h"

/*****************   DEFINES     **********************************************/
#if !defined TEN_e9
  #define TEN_e9 1000000000
#endif

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   IMPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************    GLOBAL VARIABLES   ********************************/

/* Set to ZL303XX_TRUE if the chip stores accumulated phase */
zl303xx_BooleanE accumulatePhaseForUser77x = ZL303XX_FALSE;

/*****************   IMPORTED GLOBAL VARIABLES   ******************************/

/*****************   FWD FUNCTION DECLARATIONS   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/


/* zl303xx_DpllNsMod1Sec */
/** 

   Removes the whole seconds portion of a signed nanoseconds value.

  Parameters:
   [in]   ns  Ns value to mod.

  Return Value:  Sint32T

*******************************************************************************/
static Sint32T zl303xx_DpllNsMod1Sec(Sint32T ns)
{
   if (ns >= TEN_e9)
   {
      ns %= TEN_e9;
   }
   else if (ns <= -TEN_e9)
   {
      ns = -ns;
      ns %= TEN_e9;
      ns = -ns;
   }

   return ns;
}


/* zl303xx_Dpll77xWaitForDfCtrlReadSem */
/**

   Wait for the df read semaphore.

  Parameters:
   [in]   hwParams  Pointer to a device structure.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
static zlStatusE zl303xx_Dpll77xWaitForDfCtrlReadSem(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T pllId;
    Uint32T data;
    Uint32T attempts = 0;
    zl303xx_BooleanE bReady =ZL303XX_FALSE;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xWaitForDfCtrlReadSem:", 0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        do
        {
            status = zl303xx_Read(zl303xx_Params, NULL,
                                ZLS3077X_DPLL_DF_CTRL(pllId),
                                &data);

            if (status == ZL303XX_OK && (ZLS3077X_DPLL_DF_CTRL_SEM_GET(data) == 0))
            {
                bReady = ZL303XX_TRUE;
            }
            else
            {
                /* Device is not ready, wait a few ms */
                OS_TASK_DELAY(ZLS3077X_REG_READ_INTERVAL);
            }

            if (++attempts > Dpll77xShortSanityTO/ZLS3077X_REG_READ_INTERVAL)
            {
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xWaitForDfCtrlReadSem: timeout!",
                        0,0,0,0,0,0);
                status = ZL303XX_TIMEOUT;
            }
        } while (status == ZL303XX_OK && bReady == ZL303XX_FALSE);

    }

    return status;
}


/* zl303xx_Dpll77xRequestFreqRead */
/**

   Request a 77X device read of the current frequency offset.

  Parameters:
   [in]   hwParams  Pointer to a device structure.
   [in]   memPart   Enum for the i-part, i-mem, sum-p+i or p-part.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xRequestFreqRead(void *hwParams, ZLS3077X_MemPartE memPart)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xRequestFreqRead: memPart=%d",
            memPart, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        zl303xx_Dpll77xWaitForDfCtrlReadSem(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        /* Request a read */
        status = zl303xx_Write(zl303xx_Params, NULL, ZLS3077X_DPLL_DF_CTRL(pllId),
                        ZLS3077X_DPLL_DF_CTRL_SEM_SET |
                        ZLS3077X_DPLL_DF_CTRL_CMD_SET(memPart));
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Dpll77xWaitForDfCtrlReadSem(hwParams);
    }

   return status;
}


/* zl303xx_Dpll77xGetFreqGeneral */
/*
    General routine to get the frequency.

    This routine may return:
    1) a dynamic value while in normal mode,
    2) a latched value while in NCO mode, or
    3) the previously written value if the caller is the algorithm.

  Parameters:
   [in]   hwParams        Pointer to the device parameter structure.
   [in]   memPart         Enum for the i-part, i-mem, sump+i or p-part.
   [in]   alwaysReadMemPart  TRUE if we always want the value from the device
   [out]  freqOffsetInppt Pointer to the current frequency in ppt.

  Notes:
   This function may block for some time.

  Return Value:  ZL303XX_OK          Success.
         ZL303XX_TIMEOUT     Failure.

*******************************************************************************/
static Sint32T zl303xx_Dpll77xGetFreqGeneral(void *hwParams,
                    ZLS3077X_MemPartE memPart, zl303xx_BooleanE alwaysReadMemPart,
                    Sint32T *freqOffsetInppt)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T pllId;
    Uint32T dcoOffset = 0;
    zl303xx_BooleanE isNegative = ZL303XX_FALSE;
    Uint8T round;



    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xGetFreq: memPart:%d", memPart,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
       status = ZL303XX_CHECK_POINTERS(hwParams, freqOffsetInppt);
    }
    if (status == ZL303XX_OK)
    {
       status = ZL303XX_CHECK_BOOLEAN(alwaysReadMemPart);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        if( alwaysReadMemPart == ZL303XX_TRUE )
        {
            /* We always want to get the df from the device */
            status = zl303xx_Dpll77xRequestFreqRead(hwParams, memPart);
        }
        else if (zl303xx_Dpll77xInNCOMode(hwParams))
        {
            /* While in NCO mode, we want to return what we wrote, not one
               of the latched values when the device enterend NCO mode. */
        }
        else
        {
            /* Get the df from the device. */
            status = zl303xx_Dpll77xRequestFreqRead(hwParams, memPart);
        }
        if( status != ZL303XX_OK )
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetFreq: Call to zl303xx_Dpll77xRequestFreqRead() failed=%d",
                    status,0,0,0,0,0);
        }
    }
    if (status == ZL303XX_OK)  /* read the current df from H/W */
    {
        /* NOTE: The following assumes the dcoOffset will not exceed 32 bits */

        /* Read the DCO offset from the device register (MSB byte 5) */

        status = zl303xx_Read(hwParams, NULL,
                           ZLS3077X_NCO_FREQ_OFFSET_WR_HI_DPLL(pllId),
                           &dcoOffset);
        if( status != ZL303XX_OK )
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetFreq: read (1) failed=%d",
                    status, 0,0,0,0,0);
        }
    }
    if (status == ZL303XX_OK)
    {
        /* Check the high order bit to determine sign */
        if (dcoOffset & 0x8000)
        {
            isNegative = ZL303XX_TRUE;
        }

        /* Read the DCO offset from the device register */
        status = zl303xx_Read(hwParams, NULL,
                         ZLS3077X_NCO_FREQ_OFFSET_WR_LO_DPLL(pllId),
                         &dcoOffset);
        if( status != ZL303XX_OK )
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetFreq: read (2) failed=%d",
                    status, 0,0,0,0,0);
        }
    }
    if (status == ZL303XX_OK)
    {
        /* Convert to positive numbers for the math library */
        if (isNegative)
        {
            dcoOffset = ~dcoOffset + 1;
        }

        *freqOffsetInppt = Mult_Mult_Div_U32((Uint32T)dcoOffset,
                         1000000,  /* ONE_MILLION */
                         ZLS3077X_DCO_COUNT_PER_PPM,
                         &round, NULL);
        *freqOffsetInppt += round;

        /* Convert for the original signage of the input variable */
        /* For 77X dco offset has a reversed sign, so this logic is
           inverted */
        if (isNegative != ZL303XX_TRUE)
        {
            *freqOffsetInppt = -(*freqOffsetInppt);
        }
    }
    if (status == ZL303XX_OK)
    {
        /* If in NCO mode, then a value, else 0 */
        if (*freqOffsetInppt)
        {
            zl303xx_Params->pllParams.dcoFreq = *freqOffsetInppt;
        }

        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                "zl303xx_Dpll77xGetFreq: DPLL=%d freqOffsetInppt=0x%x",
                pllId, *freqOffsetInppt, 0,0,0,0);
    }

    return (Sint32T)status;
}


/* zl303xx_Dpll77xGetFreq */
/*
    Callback function for zl303xx_AprAddDeviceS::getHwFreqOffset().

    Wrapper for zl303xx_Dpll77xGetFreqGeneral(). By passing TRUE if we are in
    NCO mode, we return the value that we previously wrote, not a latched
    value in the device.

*******************************************************************************/
Sint32T zl303xx_Dpll77xGetFreq(void *hwParams, Sint32T *freqOffsetInppt,
                                            ZLS3077X_MemPartE memPart)
{
    return zl303xx_Dpll77xGetFreqGeneral(hwParams, memPart, ZL303XX_FALSE,
                                        freqOffsetInppt);
}


/* zl303xx_Dpll77xGetDeviceFreq */
/*
    Wrapper for zl303xx_Dpll77xGetFreqGeneral().

*******************************************************************************/
Sint32T zl303xx_Dpll77xGetDeviceFreq(void *hwParams, Sint32T *freqOffsetInppt,
                                            ZLS3077X_MemPartE memPart)
{
    return zl303xx_Dpll77xGetFreqGeneral(hwParams, memPart, ZL303XX_TRUE,
                                        freqOffsetInppt);
}


/* zl303xx_Dpll77xInputPhaseErrorThresholdSet */
/**

   Sets the TIE write complete threshold for a dpll.
   Setting < 5ns will disable this mechanism. Settings > 2550 ns will be limited
   to 2550 ns.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [in]   thresNs   The threshold to set, in nanoseconds.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xInputPhaseErrorThresholdSet(void *hwParams, Uint32T threshNs)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T pllId;
    Uint32T regVal;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xInputPhaseErrorThresholdSet: threshNs=%d",
            threshNs, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetupMailboxForWrite(hwParams,
                                            ZLS3077X_MB_DPLL, pllId);
    }
    if (status == ZL303XX_OK)
    {
        regVal = (threshNs + 5) / 10;  /* regVal = round(threshNs / 10) */
        if (regVal > 0xFF)
        {
            regVal = 0xFF;
        }
        status = zl303xx_Write(zl303xx_Params, NULL,
                                ZLS3077X_DPLLX_TIE_WR_THRESH,
                                regVal);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetMailboxSemWrite(hwParams, ZLS3077X_MB_DPLL);
    }
    if (status == ZL303XX_OK)
    {
        /* Update the copy we have in memory */
        status = zl303xx_Dpll77xUpdateMailboxCopy(hwParams, ZLS3077X_MB_DPLL, pllId);
    }

    return status;
}


/* zl303xx_Dpll77xInputPhaseErrorClearSet */
/**

   Sets the TIE write clear on switch bit for a DPLL.

  Parameters:
   [in]   hwParams    Pointer to a device parameter structure.
   [in]   tieWrClear  TIE write clear bit.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xInputPhaseErrorClearSet(void *hwParams, zl303xx_BooleanE tieWrClear)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xInputPhaseErrorClearSet: tieWrClear=%d",
            tieWrClear, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_BOOLEAN(tieWrClear);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetupMailboxForWrite(hwParams,
                                            ZLS3077X_MB_DPLL, pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_ReadModWrite(zl303xx_Params, NULL, ZLS3077X_DPLLX_TIE,
                                    tieWrClear, 0x1, NULL);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetMailboxSemWrite(hwParams, ZLS3077X_MB_DPLL);
    }
    if (status == ZL303XX_OK)
    {
        /* Update the copy we have in memory */
        status = zl303xx_Dpll77xUpdateMailboxCopy(hwParams, ZLS3077X_MB_DPLL, pllId);
    }

    return status;
}

/* zl303xx_Dpll77xTieCtrlSet */
/** 
   Helper function to write to the dpll_tie_ctrl register.

  Parameters:
   [in]   zl303xx_Params  Pointer to a device parameter structure.
   [in]   oper          Operation to write to the register.

  Notes:  Assumes input parameters are valid.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xTieCtrlSet(void *hwParams, ZLS3077X_DpllTieCtrlE oper)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xTieCtrlSet: oper=%d", oper, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_Write(zl303xx_Params, NULL,
                                ZLS3077X_DPLL_TIE_CTRL_REG(pllId),
                                (oper & ZLS3077X_DPLL_TIE_CTRL_MASK));
    }

    return status;
}

/* zl303xx_Dpll77xTieCtrlReady */
/**
   Determines if the ZL3077X device is ready to perform an operation.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [out]  ready     ZL303XX_TRUE if the device is ready. ZL303XX_FALSE otherwise.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xTieCtrlReady(void *hwParams, zl303xx_BooleanE *ready)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T data;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xTieCtrlReady:",
            0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTERS(hwParams, ready);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL,
                            ZLS3077X_DPLL_TIE_CTRL_REG(pllId),
                            &data);
    }

    if (status == ZL303XX_OK)
    {
        data = data & ZLS3077X_DPLL_TIE_CTRL_MASK;
        *ready = (data == ZLS3077X_DPLL_TIE_COMPLETE);

        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xTieCtrlReady: *ready=%d ",
                *ready, 0,0,0,0,0);
    }

    return status;
}


/* zl303xx_Dpll77xWaitForTieCtrlReady */
/**

   Wait for the TIE control register to become ready

  Parameters:
   [in]   hwParams  Pointer to a device structure.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
static zlStatusE zl303xx_Dpll77xWaitForTieCtrlReady(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T pllId;
    Uint32T attempts = 0;
    zl303xx_BooleanE bReady =ZL303XX_FALSE;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xWaitForTieCtrlReady:", 0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        do
        {
            status = zl303xx_Dpll77xTieCtrlReady(zl303xx_Params, &bReady);

            if (status == ZL303XX_OK && (bReady == ZL303XX_TRUE))
            {
                bReady = ZL303XX_TRUE;
            }
            else
            {
                /* Device is not ready, wait a few ms */
                OS_TASK_DELAY(ZLS3077X_REG_READ_INTERVAL);
            }

            if (++attempts > Dpll77xShortSanityTO/ZLS3077X_REG_READ_INTERVAL)
            {
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xWaitForTieCtrlReady: timeout!",
                        0,0,0,0,0,0);
                status = ZL303XX_TIMEOUT;
            }
        } while (status == ZL303XX_OK && bReady == ZL303XX_FALSE);

    }

    return status;
}


/* zl303xx_Dpll77xInputPhaseErrorWrite */
/**

   Writes the TIE data for a DPLL.

   zl303xx_Dpll77xTieCtrlReady() must be called before this function to ensure
   that the device is ready.

   This function clears the sticky bit in dpll_tie_wr_status (TIE write
   complete) for the DPLL.

  Parameters:
   [in]   hwParams     Pointer to a device parameter structure.
   [in]   tieNs        The input-to-output TIE data to write, in nanoseconds.
   [in]   snapAlign    The flag to be "hitless".

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xInputPhaseErrorWrite(void *hwParams, Sint32T tieNs,
                                                zl303xx_BooleanE snapAlign)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T pllId;
    Sint32T AccuInputPhaseError1588HostReg;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xInputPhaseErrorWrite: tieNs=%d  snapAlign=%d",
            tieNs, snapAlign, 0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    /* Remove whole seconds from TIE value */
    if (status == ZL303XX_OK)
    {
        tieNs = zl303xx_DpllNsMod1Sec(tieNs);
    }
    /* Write the TIE size into the register */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Write(zl303xx_Params, NULL,
                                    ZLS3077X_DPLL_TIE_DATA_REG(pllId),
                                    tieNs);
    }
    /* Store a software version of the writes. */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xAccuInputPhaseError1588HostRegGet(hwParams,
                                                    &AccuInputPhaseError1588HostReg);
        AccuInputPhaseError1588HostReg = AccuInputPhaseError1588HostReg + tieNs;
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xAccuInputPhaseError1588HostRegSet(hwParams,
                                                    AccuInputPhaseError1588HostReg);
    }

    /* Do the write */
    if (status == ZL303XX_OK)
    {
        if (snapAlign)
        {
            status = zl303xx_Write(zl303xx_Params, NULL,
                                        ZLS3077X_DPLL_TIE_CTRL_REG(pllId),
                                        ZLS3077X_DPLL_MTIE_SNAP);
        }
        else
        {
            status = zl303xx_Write(zl303xx_Params, NULL,
                                        ZLS3077X_DPLL_TIE_CTRL_REG(pllId),
                                        ZLS3077X_DPLL_TIE_WRITE);
        }
    }

    return status;
}


/* zl303xx_Dpll77xInputPhaseErrorRead */
/**
    Reads the TIE data for a DPLL. This routine waits until the device is
    ready before returning.

  Parameters:
   [in]   hwParams   Pointer to a device parameter structure.
   [in]   timeoutMs  unused
   [out]  tieNs      The input-to-output TIE data, in nanoseconds.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xInputPhaseErrorRead(void *hwParams, Uint32T timeoutMs,
                                                            Sint32T *tieNs)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xInputPhaseErrorRead:", 0,0,0,0,0,0);

    if( timeoutMs ) {}  /* Warning removal */

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTERS(hwParams, tieNs);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xWaitForTieCtrlReady(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xTieCtrlSet(hwParams, ZLS3077X_DPLL_TIE_READ);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xWaitForTieCtrlReady(hwParams);
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLL_TIE_DATA_REG(pllId),
                            (Uint32T *)tieNs);

        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                "zl303xx_Dpll77xInputPhaseErrorRead: *tieNs=%d", *tieNs, 0,0,0,0,0);
    }

    return status;
}


/* zl303xx_Dpll77xInputPhaseErrorClear */
/**

   Clears the input-output phase offset of the PLL to the active reference.

  Parameters:
   [in]   hwParams    Pointer to a device parameter structure.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xInputPhaseErrorClear(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T pllId;
    Sint32T tieNs;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xInputPhaseErrorClear: ",
            0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_ParamsS *zl303xx_Params;

        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xInputPhaseErrorRead(hwParams, 0 /* unused */, &tieNs);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xInputPhaseErrorWrite(hwParams, -tieNs, ZL303XX_FALSE);
    }

    return status;
}


/* zl303xx_Dpll77xInputPhaseErrorStatusSet */
/**

   Sets the TIE write complete sticky status bit for a DPLL.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [in]   v         What to set the bit to: either 0 or 1

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xInputPhaseErrorStatusSet(void *hwParams, Uint32T v)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T mask;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xInputPhaseErrorStatusSet: v=%d", v, 0, 0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        if( v > 1 )
        {
            status = ZL303XX_PARAMETER_INVALID;
        }
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        mask = (1 << pllId) & ZLS3077X_TIE_WR_STATUS_MASK;


        status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                                    ZLS3077X_TIE_WR_STATUS_REG,
                                    (v << pllId), mask, NULL);
    }

    return status;
}


/* zl303xx_Dpll77xInputPhaseErrorStatusGet */
/**

   Returns the TIE write complete status bit for a DPLL.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [out]  complete  TIE write complete bit.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xInputPhaseErrorStatusGet(void *hwParams,
                                                zl303xx_BooleanE *complete)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T mask;
    Uint32T data;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xInputPhaseErrorStatusGet: ", 0, 0, 0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTERS(hwParams, complete);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        *complete = ZL303XX_FALSE;
    }
    if (status == ZL303XX_OK)
    {
        mask = (1 << pllId) & ZLS3077X_TIE_WR_STATUS_MASK;
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_TIE_WR_STATUS_REG, &data);
    }

    if (status == ZL303XX_OK)
    {
        *complete = ZL303XX_INT_TO_BOOL(data & mask);

        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                "zl303xx_Dpll77xInputPhaseErrorStatusGet: *complete=%d",
                *complete, 0, 0,0,0,0);
    }

    return status;
}


/* zl303xx_Dpll77xAdjustIfHitlessCompensationBeingUsed */
/**
   Decision whether to apply phase using hitless compensation.

  Parameters:
   [in]   hwParams     Pointer to a device parameter structure.
   [in]   hitlessType  Configured hitless compensation type.
   [out]  apply        Return TRUE to apply the compensate for the cumulative
                            stepTime() operations before switching to electrical
                            mode.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
Sint32T zl303xx_Dpll77xAdjustIfHitlessCompensationBeingUsed(void *hwParams,
                                           zl303xx_HitlessCompE hitlessType,
                                           zl303xx_BooleanE *apply)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T data;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xAdjustIfHitlessCompensationBeingUsed: hitlessType=%d",
            hitlessType,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        /* Convert HITLESS_COMP_FALSE/TRUE to boolean FALSE/TRUE */
        if (hitlessType == ZL303XX_HITLESS_COMP_FALSE)
        {
            *apply = ZL303XX_FALSE;
        }
        else if (hitlessType == ZL303XX_HITLESS_COMP_TRUE)
        {
            *apply = ZL303XX_TRUE;
        }
        else
        {
            /* HITLESS_COMP_AUTO: Return TRUE if TIE clear is enabled,
               FALSE otherwise */

            status = zl303xx_Read(zl303xx_Params, NULL,
                               ZLS3077X_DPLL_TIE_CLEAR_REG(pllId),
                               &data);
            data >>= ZLS3077X_DPLL_TIE_CLEAR_EN_SHIFT;
            *apply = data & ZLS3077X_DPLL_TIE_CLEAR_EN_MASK;
            /* The register is tie_clear=0|1. This is backwards from what apply
               is returning so flip the register value */
            *apply = !(*apply);

            ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                "zl303xx_Dpll77xAdjustIfHitlessCompensationBeingUsed: *apply=%d",
                *apply,0,0,0,0,0);
        }
    }

    return (Sint32T)status;
}


/* zl303xx_Dpll77xAdjustPhaseUsingHitlessCompensation */
/**

   Adjust the phase offset using hitless compensation by performing a TIE
   write of the negated cumulative NCO stepTime() phase.

  Parameters:
   [in]   hwParams    Pointer to the device data structure.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_Dpll77xAdjustPhaseUsingHitlessCompensation(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_BooleanE apply = ZL303XX_FALSE;
    Sint32T ncoPhase = 0;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xAdjustPhaseUsingHitlessCompensation: ", 0, 0, 0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_ParamsS *zl303xx_Params;

        zl303xx_Params= hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    apply = accumulatePhaseForUser77x;

    if (status == ZL303XX_OK)
    {
        if (apply)
        {
            status = zl303xx_Dpll77xAccuStepTimePhase1588HostRegGet(hwParams, &ncoPhase);
            if (status != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xAdjustPhaseUsingHitlessCompensation: Call to zl303xx_Dpll77xAccuStepTimePhase1588HostRegGet() failed=%d",
                        status, 0,0,0,0,0);
            }
            else
            {
                if (ncoPhase != 0)
                {
                    #define NO_SNAP_ALIGN ZL303XX_FALSE /* SnapAlign=FALSE */

                    /* Request TIE write of the negated cumulative stepTime
                       phase (no Snap Align) */
                    status = zl303xx_Dpll77xInputPhaseErrorWrite(hwParams,
                                                    -ncoPhase, NO_SNAP_ALIGN);
                    if (status != ZL303XX_OK)
                    {
                        ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xAdjustPhaseUsingHitlessCompensation: Call to zl303xx_Dpll77xInputPhaseErrorWrite() failed=%d",
                                status, 0,0,0,0,0);
                    }
                }
            }
        }
    }

    return status;
}


/* zl303xx_Dpll77xTakeHwNcoControl */
/**
   An example of how to take software control of the NCO for a ZL3077X device.

  Parameters:
   [in]   hwParams   Pointer to the device instance parameter structure.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
Sint32T zl303xx_Dpll77xTakeHwNcoControl(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T pllId = 0;
    ZLS3077X_DpllHWModeE *priorMode = NULL;
    ZLS3077X_DpllHWModeE oldPriorMode;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xTakeHwNcoControl: ", 0, 0, 0, 0, 0, 0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
        priorMode = &zl303xx_Params->pllParams.d77x.pllPriorMode;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
       /* Save current DPLL mode */
        status = zl303xx_Dpll77xModeGet(hwParams, &oldPriorMode);
    }

    /* Request a read of the current frequency offset, but don't actually read
       the dpllx_df_offset register. It only needs to be populated, so when the
       switch to NCO mode happens, the value already in dpllx_df_offset is
       reasonably close to the electrical reference.

       We perform this only if the current mode is electrical or automatic mode.
       We can't determine the validity of the I-memory value in other modes.
    */
    if ( (status == ZL303XX_OK) &&
         (oldPriorMode == ZLS3077X_DPLL_MODE_REFLOCK || oldPriorMode == ZLS3077X_DPLL_MODE_AUTO_LOCK) )
    {
        /* This will populate dplln_df_in */
        status = zl303xx_Dpll77xRequestFreqRead(zl303xx_Params, ZLS3077X_NCO_I_PART);
    }

    /* Enable NCO mode */
    if (status == ZL303XX_OK)
    {
        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xTakeHwNcoControl: Failed @ %u. Status=%d",
                    pllId, status, 0,0,0,0);
        }
        if (status == ZL303XX_OK)
        {
            if (oldPriorMode == ZLS3077X_DPLL_MODE_NCO)
            {
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xTakeHwNcoControl: current mode is already NCO in mode, do nothing",
                        0,0,0,0,0,0);
            }
            else
            {
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xTakeHwNcoControl: Setting DPLL mode to NCO mode",
                        0,0,0,0,0,0);
                *priorMode = oldPriorMode;
                status = zl303xx_Dpll77xModeSet(hwParams, ZLS3077X_DPLL_MODE_NCO);
            }

            if (status != ZL303XX_OK)
            {
                /* Failed to set DPLL_X into NCO mode */
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xTakeHwNcoControl: Failed for pllId=%d. Status=%d",
                        pllId, status, 0,0,0,0);
            }
            else
            {
                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xTakeHwNcoControl: hwParams=%p, pllId=%d",
                        hwParams, pllId, 0,0,0,0);
            }
        }
   }

   return (Sint32T)status;
}


/* zl303xx_Apr77xReturnHwNcoControl */
/**
   An example of how to return hw control of the NCO for a ZL3077X device.

  Parameters:
   [in]   hwParams   Pointer to the device instance parameter structure.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
Sint32T zl303xx_Dpll77xReturnHwNcoControl(void *hwParams)
{

    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T pllId;
    ZLS3077X_DpllHWModeE *priorMode = NULL;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xReturnHwNcoControl: ", 0, 0, 0, 0, 0, 0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
        priorMode = &zl303xx_Params->pllParams.d77x.pllPriorMode;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xModeSet(zl303xx_Params, *priorMode);
        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xReturnHwNcoControl: Failed @ %u. Status %d",
                    pllId, status, 0,0,0,0);
        }
        else
        {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xReturnHwNcoControl: hwParams=%p, pllId=%d",
                    hwParams, pllId, 0,0,0,0);
        }
    }

    return (Sint32T)status;
}


/**  zl303xx_Dpll77xBCHybridActionPhaseLock

   This routine is a simple callback for actions to perform when entering phase lock during Hybrid Boundary Clock operations (ACI9).

  Parameters:
   [in]   zl303xx_Params     Pointer to the device structure

  Return Value: Sint32T
*****************************************************************************/
Sint32T  zl303xx_Dpll77xBCHybridActionPhaseLock(void *hwParams)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ParamsS *paramsNCOAssist = NULL;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(hwParams);
   }

   /* Retrieve the ParamS structure of the NCO Assist DPLL Device */
   if (status == ZL303XX_OK)
   {
      (void)zl303xx_Dpll77xGetNCOAssistParamsSAssociation(hwParams,&paramsNCOAssist);
   }

   /* Due to 8273.2 compliance, it is necessary to reduce the PSL setting
      when the hybrid server status is in PHASE_LOCK.  When we exit PHASE_LOCK,
      we restore the normal PSL settings. */
   if( status == ZL303XX_OK )
   {
      status = zl303xx_Dpll77xPhaseSlopeLimitSet(hwParams, 750);
      if (status != ZL303XX_OK)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xBCHybridActionPhaseLock: hwParams=%p, failure with status=%d",
            hwParams,status,0,0,0,0);
      }

      if (paramsNCOAssist != NULL)
      {
         /* Also set the NCO-Assist DPLL if it is valid */
         status = zl303xx_Dpll77xPhaseSlopeLimitSet(paramsNCOAssist, 750);
         if (status != ZL303XX_OK)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xBCHybridActionPhaseLock: paramsNCOAssist=%p, failure with status=%d",
               paramsNCOAssist,status,0,0,0,0);
         }
      }
   }
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xBCHybridActionPhaseLock: hwParams=%p, failure with status=%d",
         hwParams,status,0,0,0,0);
   }

   return (Sint32T)status;
} /*  zl303xx_Dpll77xBCHybridActionPhaseLock */


/**   zl303xx_Dpll77xBCHybridActionOutOfLock

   This routine is a simple callback for actions to perform when exiting phase lock during Hybrid Boundary Clock operations (ACI9).

  Parameters:
   [in]   zl303xx_Params     Pointer to the device structure

  Return Value: Sint32T
*****************************************************************************/
Sint32T  zl303xx_Dpll77xBCHybridActionOutOfLock(void *hwParams)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ParamsS *paramsNCOAssist = NULL;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(hwParams);
   }

   /* Retrieve the ParamS structure of the NCO Assist DPLL Device */
   if (status == ZL303XX_OK)
   {
      (void)zl303xx_Dpll77xGetNCOAssistParamsSAssociation(hwParams,&paramsNCOAssist);
   }

   /* When going out of phase lock, restore original user settings */
   /* In this case we assume the suggesting setting of 7.5 us/s */
   if( status == ZL303XX_OK )
   {
      /* In this case we assume the suggesting setting of 7.5 us/s */
      status = zl303xx_Dpll77xPhaseSlopeLimitSet(hwParams, 7500);
      if (status != ZL303XX_OK)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xBCHybridActionOutOfLock: hwParams=%p, failure with status=%d",
            hwParams,status,0,0,0,0);
      }

      if (paramsNCOAssist != NULL)
      {
         /* Also set the NCO-Assist DPLL if it is valid */
         status = zl303xx_Dpll77xPhaseSlopeLimitSet(paramsNCOAssist, 7500);
         if (status != ZL303XX_OK)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xBCHybridActionOutOfLock: paramsNCOAssist=%p, failure with status=%d",
               paramsNCOAssist,status,0,0,0,0);
         }
      }
   }
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xBCHybridActionOutOfLock: hwParams=%p, failure with status=%d",
         hwParams,status,0,0,0,0);
   }

   return (Sint32T)status;
} /*  zl303xx_Dpll77xBCHybridActionOutOfLock */


/**   zl303xx_Dpll77xSetActiveElecActionsInitial

   This routine performs the actions needed when switching to given electrical
   reference.

   In particular, this routine:
    1) determines if we are switching to one of the following:
        a) a ref-sync pair
        b) an ePPS reference
        c) a reference that has a frequency of 1pps with TIE-clear enabled
    2) if switching to one of the above:
        a) writes device registers (which are restored later) to facilitate the
           switch
        b) determines the total phase steps (ToD and TIE-writes) taken so far
        c) writes a negative of the total phase steps as a TIE-write
        d) waits a little while
    3) sets the new reference
    4) sets the new sync if specified
    5) switches modes to ZLS3077X_DPLL_MODE_REFLOCK

  Parameters:
   [in]   hwParams         Pointer to the device structure
   [in]   refId            RefId of the target electrical reference
   [in]   syncId           SyncId of the target Sync pulse input (-1 for disabled)

  Return Value: Sint32T
*****************************************************************************/
Sint32T zl303xx_Dpll77xSetActiveElecActionsInitial(void *hwParams, Sint32T refId, Sint32T syncId)
{
    zlStatusE status = ZL303XX_OK;
    zlStatusE stickyStatus = ZL303XX_ERROR;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T pllId;
    Uint32T chipSyncId = 0;
    Uint32T targetRefFreq = 0;
    Sint32T existingNcoPhase = 0;
    Sint32T existingTIEPhase = 0;
    Sint32T finalCompValue = 0;
    zl303xx_BooleanE applyHitlessCompensationMechanism = ZL303XX_FALSE;
    zl303xx_BooleanE bSwitchingToRefSyncPair = ZL303XX_FALSE;
    zl303xx_BooleanE bEPPS = ZL303XX_FALSE;

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_REF_ID((Uint32T)refId);
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_SYNC_ID(syncId);
    }

    /* Get pllId for the given hwParams */
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    /* Since this function will only be called from customer space, we
       may clear the reference valid sticky bits. */
    if (status == ZL303XX_OK)
    {
        if (status == ZL303XX_OK) /* Lock sticky bits */
        {
           stickyStatus = zl303xx_Dpll77xStickyLockSet(hwParams, ZLS3077X_STICKY_LOCK);
           status = stickyStatus;
        }

        if (status == ZL303XX_OK) /* Clear DPLL sticky bits */
        {
           status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                                ZLS3077X_HOLDOVER_LOCK_TH_STICKY_REG(pllId),
                                0x00,ZLS3077X_HOLDOVER_LOCK_STATUS_MASK_DPLL,0);
        }
        if (status == ZL303XX_OK) /* Clear DPLL sticky bits */
        {
            status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                                ZLS3077X_HOLDOVER_LOCK_TL_STICKY_REG(pllId),
                                0x00,ZLS3077X_HOLDOVER_LOCK_STATUS_MASK_DPLL,0);
        }
        if (status == ZL303XX_OK) /* Clear Ref sticky bits */
        {
            status = zl303xx_Ref77xMonBitsClear(hwParams, refId);
        }
        if (status == ZL303XX_OK && syncId >= 0 ) /* Clear Sync sticky bits if valid */
        {
            status = zl303xx_Ref77xMonBitsClear(hwParams, syncId);
        }

        if (stickyStatus == ZL303XX_OK) /* Unlock sticky bits */
        {
           stickyStatus = zl303xx_Dpll77xStickyLockSet(hwParams, ZLS3077X_STICKY_UNLOCK);
           status = (status == ZL303XX_OK) ? stickyStatus : status;
        }

        /* Wait for 77x to update registers with new data */
        OS_TASK_DELAY(ZLS3077X_STICKY_UPDATE_DELAY_MS);
    }

    /* Determine whether to perform compensation based upon the DPLL TIE clear setting */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xAdjustIfHitlessCompensationBeingUsed(hwParams,
                                            ZL303XX_HITLESS_COMP_AUTO,
                                            &applyHitlessCompensationMechanism);
    }
    if (status == ZL303XX_OK)
    {
        if( applyHitlessCompensationMechanism == ZL303XX_TRUE )
        {
            /* Get the reference frequency: Used if we are a 1pps input. */
            if (status == ZL303XX_OK)
            {
                status = zl303xx_Ref77xFreqDetectedGet( hwParams, refId, &targetRefFreq );
            }
            /* Check if the reference we are switching to is a ref-sync pair or ePPS. */
            if (status == ZL303XX_OK)
            {
                status = zl303xx_Dpll77xGetRefSyncPairMode( hwParams, refId, &chipSyncId,
                                                        &bSwitchingToRefSyncPair, &bEPPS );
            }
            if (status == ZL303XX_OK)
            {
                if ( syncId >= 0 )
                {
                    /* User sync is valid, switch to ref sync */
                    bSwitchingToRefSyncPair = ZL303XX_TRUE;

                    ZL303XX_TRACE_ALWAYS( "Setting electrical Ref+Sync pair with refId=%d, syncId=%d",
                                           refId, syncId, 0,0,0,0);
                }
                else if ( bSwitchingToRefSyncPair == ZL303XX_TRUE )
                {
                    /* User sync is invalid but the ref we are switching to is a ref
                       sync pair */

                    /* Override invalid user value with the SyncId from chip */
                    syncId = chipSyncId;

                    ZL303XX_TRACE_ALWAYS( "Setting electrical Ref+Sync pair with refId=%d, syncId=%d",
                                           refId, chipSyncId, 0,0,0,0);
                }
                else
                {
                    /* Sync is invalid, switch to ref only (ref could still be a sync
                       pulse or ePPS) */
                    bSwitchingToRefSyncPair = ZL303XX_FALSE;

                    ZL303XX_TRACE_ALWAYS( "Setting electrical Ref with refId=%d",
                                           refId, 0,0,0,0,0);
                }
            }

            /* We have enough data, now perform the actions. */
            if(( bSwitchingToRefSyncPair == ZL303XX_TRUE ) ||
               ( bEPPS == ZL303XX_TRUE ) ||
               ( targetRefFreq == 1 ))
            {
                /* Write device registers to facilitate hitless switching.
                   Once the switch is done and after a timeout period, these registers
                   are restored. These registers MUST NOT be modified during the
                   timeout period. The current timeout period is 30s. These registers
                   are restored by zl303xx_Dpll77xSetActiveElecActionsTransientStage2().
                */
                if (status == ZL303XX_OK)
                {
                    if( targetRefFreq == 1 )
                    {
                        /* For the firmware to work, TIE-clear must be 1. */
                        status = zl303xx_ReadModWrite(hwParams, NULL,
                                    ZLS3077X_DPLL_TIE_CLEAR_REG(pllId),
                                    0x1, /* tie_clear=1, tod_step_reset=0 */
                                    (ZLS3077X_DPLL_TOD_STEP_RESET_EN_MASK | ZLS3077X_DPLL_TIE_CLEAR_EN_MASK),
                                    &(zl303xx_Params->pllParams.d77x.prev_ZLS3077X_DPLL_CTRL_x_REG));
                    }
                    else
                    {
                        status = zl303xx_ReadModWrite(hwParams, NULL,
                                    ZLS3077X_DPLL_TIE_CLEAR_REG(pllId),
                                    0x0, /* tod_step_reset=0 */
                                    ZLS3077X_DPLL_TOD_STEP_RESET_EN_MASK,
                                    &(zl303xx_Params->pllParams.d77x.prev_ZLS3077X_DPLL_CTRL_x_REG));
                    }
                }
                if (status == ZL303XX_OK)
                {
                    status = zl303xx_Dpll77xSetupMailboxForWrite(hwParams,
                                                        ZLS3077X_MB_DPLL, pllId);
                }
                if (status == ZL303XX_OK)
                {
                    status = zl303xx_ReadModWrite(hwParams, NULL,
                                ZLS3077X_DPLLX_TIE_REG,
                                0x2, /* switch_clear_en=0, ref_sync_tie_wr=1 */
                                (ZLS3077X_DPLL_TIE_SWITCH_CLEAR_EN_MASK | ZLS3077X_DPLL_TIE_REF_SYNC_TIE_WR_MASK),
                                &(zl303xx_Params->pllParams.d77x.prev_ZLS3077X_DPLLX_TIE_REG));
                }
                if (status == ZL303XX_OK)
                {
                    status = zl303xx_Dpll77xSetMailboxSemWrite(hwParams, ZLS3077X_MB_DPLL);
                }
                if (status == ZL303XX_OK)
                {
                    /* Update the copy we have in memory */
                    status = zl303xx_Dpll77xUpdateMailboxCopy(hwParams, ZLS3077X_MB_DPLL, pllId);
                }
                if (status == ZL303XX_OK)
                {
                    /* Make sure we restore above registers when we are done. */
                    zl303xx_Params->pllParams.d77x.restoreRegValues = ZL303XX_TRUE;
                }
                if (status == ZL303XX_OK)
                {
                    /* Set the device mode to holdover while we are switching */
                    status = zl303xx_Dpll77xModeSet(hwParams, ZLS3077X_DPLL_MODE_HOLDOVER);
                }
                if (status == ZL303XX_OK)
                {
                    /* Disable NCO-assist mode. The device only looks at this bit while in
                       NCO mode. Now, we are in HOLDOVER so it is safe to maniplulate the
                       bit. */
                    status = zl303xx_Dpll77xSetNCOAssistEnable(hwParams, ZL303XX_FALSE);
                }
                if (status == ZL303XX_OK)
                {
                    /* Get accumulated StepTime since last switch from host registers */
                    status = zl303xx_Dpll77xAccuStepTimePhase1588HostRegGet(hwParams,
                                                                    &existingNcoPhase);
                }
                if (status == ZL303XX_OK)
                {
                    /* Get accumulated input phase error offset from host registers */
                    status = zl303xx_Dpll77xAccuInputPhaseError1588HostRegGet(hwParams,
                                                                    &existingTIEPhase);
                }
                if (status == ZL303XX_OK)
                {
                    /* Compensate for the current offsets via a input phase error write
                       operation */
                    finalCompValue = - existingTIEPhase - existingNcoPhase;
                    if (finalCompValue != 0)
                    {
                        status = zl303xx_Dpll77xInputPhaseErrorWrite(hwParams,
                                                            finalCompValue, ZL303XX_FALSE);
                    }
                }
                if (status == ZL303XX_OK)
                {
                    OS_TASK_DELAY(100);
                }
                if (status == ZL303XX_OK)
                {
                    /* Clear accumulated StepTime host register */
                    status = zl303xx_Dpll77xAccuStepTimePhase1588HostRegSet(hwParams, 0);
                }
                if (status == ZL303XX_OK)
                {
                    /* Clear accumulated input phase error offset host register */
                    status = zl303xx_Dpll77xAccuInputPhaseError1588HostRegSet(hwParams, 0);
                }
            }
        }
    }
    if (status == ZL303XX_OK)
    {
        if (bSwitchingToRefSyncPair == ZL303XX_TRUE)
        {
            /* Make sure the sync reference has been programmed */
            status = zl303xx_Dpll77xSetRefSyncPair(hwParams, refId, syncId);
        }
    }
    if (status == ZL303XX_OK)
    {
        /* Set ref ID */
        status = zl303xx_Dpll77xCurrRefSet(hwParams, refId);
    }
    if (status == ZL303XX_OK)
    {
        /* Set the chip to electrical mode */
        status = zl303xx_Dpll77xModeSet(hwParams, ZLS3077X_DPLL_MODE_REFLOCK);
    }


    return (Sint32T)status;

} /*  zl303xx_Dpll77xSetActiveElecActionsInitial */


/**   zl303xx_Dpll77xSetActiveElecActionsTransientStage1

   This routine is currently unused (but it is currently called).

  Parameters:
   [in]   zl303xx_Params     Pointer to the device structure
   [in]   refId            RefId of the target electrical reference

  Return Value: Sint32T
*****************************************************************************/
Sint32T zl303xx_Dpll77xSetActiveElecActionsTransientStage1(void *hwParams, Uint32T refId)
{
    zlStatusE status = ZL303XX_OK;

    if( hwParams ) {}  /* Warning removal */
    if( refId ) {}  /* Warning removal */

    return (Sint32T)status;

} /* zl303xx_Dpll77xSetActiveElecActionsTransientStage1 */


/**   zl303xx_Dpll77xSetActiveElecActionsTransientStage2

   This routine restores the values previously manipulated when switching to
   and electrical reference.

  Parameters:
   [in]   zl303xx_Params     Pointer to the device structure
   [in]   refId            RefId of the target electrical reference

  Return Value: Sint32T
*****************************************************************************/
Sint32T  zl303xx_Dpll77xSetActiveElecActionsTransientStage2(void *hwParams, Uint32T refId)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T pllId;


    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_REF_ID(refId);
    }

    /* Get pllId for the given hwParams */
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        if( zl303xx_Params->pllParams.d77x.restoreRegValues == ZL303XX_TRUE )
        {
            zl303xx_Params->pllParams.d77x.restoreRegValues = ZL303XX_FALSE;

            if (status == ZL303XX_OK)
            {
                /* Put tod_step_reset back to its original value */
                status = zl303xx_ReadModWrite(hwParams, NULL,
                            ZLS3077X_DPLL_TIE_CLEAR_REG(pllId),
                            zl303xx_Params->pllParams.d77x.prev_ZLS3077X_DPLL_CTRL_x_REG,
                            (ZLS3077X_DPLL_TOD_STEP_RESET_EN_MASK | ZLS3077X_DPLL_TIE_CLEAR_EN_MASK),
                            NULL);
            }
            if (status == ZL303XX_OK)
            {
                status = zl303xx_Dpll77xSetupMailboxForWrite(hwParams,
                                                    ZLS3077X_MB_DPLL, pllId);
            }
            if (status == ZL303XX_OK)
            {
                /* Put switch_clear_en and ref_sync_tie_wr back to their original value */
                status = zl303xx_ReadModWrite(hwParams, NULL,
                            ZLS3077X_DPLLX_TIE_REG,
                            zl303xx_Params->pllParams.d77x.prev_ZLS3077X_DPLLX_TIE_REG,
                            (ZLS3077X_DPLL_TIE_SWITCH_CLEAR_EN_MASK | ZLS3077X_DPLL_TIE_REF_SYNC_TIE_WR_MASK),
                            NULL);
            }
            if (status == ZL303XX_OK)
            {
                status = zl303xx_Dpll77xSetMailboxSemWrite(hwParams, ZLS3077X_MB_DPLL);
            }
            if (status == ZL303XX_OK)
            {
                /* Update the copy we have in memory */
                status = zl303xx_Dpll77xUpdateMailboxCopy(hwParams, ZLS3077X_MB_DPLL, pllId);
            }
        }
    }

    return (Sint32T)status;

} /*  zl303xx_Dpll77xSetActiveElecActionsTransientStage2 */


/** zl303xx_Dpll77xGetDeviceInfo

   Get device information.

  Parameters:
   [in]   zl303xx_Params     Pointer to the device structure
   [out]  devType          Pointer to output value of current device

  Return Value: Sint32T
*****************************************************************************/
Sint32T zl303xx_Dpll77xGetDeviceInfo(void *hwParams, zl303xx_DevTypeE *devType, zl303xx_DeviceIdE *devId, Uint32T *devRev)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(hwParams);
   }
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(devType);
   }
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(devId);
   }
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(devRev);
   }

   if (status == ZL303XX_OK)
   {
      hwParams = hwParams;
      *devType = ZL3077X_DEVICETYPE;
      *devId = 0;
      *devRev = 0;
   }

   return (Sint32T)status;
} /* zl303xx_Dpll77xGetDeviceInfo */


/** zl303xx_Dpll77xSetNCOAssistEnable

   Enable/disable the NCO-Assist DPLL for the given hwParams

  Parameters:
   [in]   hwParams   Pointer to the device structure
   [in]   enable     enable/disable

  Return Value: Sint32T
*****************************************************************************/
zlStatusE zl303xx_Dpll77xSetNCOAssistEnable(void *hwParams, zl303xx_BooleanE enable)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xSetNCOAssistEnable: enable=%d", enable, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                            ZLS3077X_DPLL_NCO_ASSIST_EN_REG(pllId),
                            enable << ZLS3077X_DPLL_NCO_ASSIST_EN_SHIFT,
                            ZLS3077X_DPLL_NCO_ASSIST_EN_MASK << ZLS3077X_DPLL_NCO_ASSIST_EN_SHIFT,
                            NULL);
    }

    return status;
}


/** zl303xx_Dpll77xGetNCOAssistEnable

   Enable/disable the NCO-Assist DPLL for the given hwParams

  Parameters:
   [in]   hwParams   Pointer to the device structure
   [in]   enable     enable/disable

  Return Value: Sint32T
*****************************************************************************/
zlStatusE zl303xx_Dpll77xGetNCOAssistEnable(void *hwParams, zl303xx_BooleanE *enable)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params;
    Uint32T pllId;
    Uint32T data;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xGetNCOAssistEnable:", 0, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(enable);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params = hwParams;
        pllId = zl303xx_Params->pllParams.pllId;
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL,
                                ZLS3077X_DPLL_NCO_ASSIST_EN_REG(pllId),
                                &data);
    }
    if (status == ZL303XX_OK)
    {
        data >>= ZLS3077X_DPLL_NCO_ASSIST_EN_SHIFT;
        data &= ZLS3077X_DPLL_NCO_ASSIST_EN_MASK;
        *enable = data;
    }

    return status;
}


/* zl303xx_Dpll77xMsgRouterLog */
/**

   Logs for the mesage router

  Parameters:
   [in]   hwParams  Pointer to the device parameter structure.
   [in]   inData    In data
   [in]   outData   Out data
   [in]   statusToLog   Status data

*******************************************************************************/
static Sint32T zl303xx_Dpll77xMsgRouterLog(void *hwParams, void *inData, void *outData, zlStatusE statusToLog)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_DriverMsgInDataS *in;
    zl303xx_DriverMsgOutDataS *out;


    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(inData);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(outData);
    }

    if (status == ZL303XX_OK)
    {
        in  = (zl303xx_DriverMsgInDataS *)inData;
        out = (zl303xx_DriverMsgOutDataS *)outData;
    }
    if (status == ZL303XX_OK)
    {
        switch( in->dpllMsgType )
        {
            case ZL303XX_DPLL_DRIVER_MSG_GET_FREQ_I_OR_P:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (GET_FREQ_I_OR_P)  f=%d  mp=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_GET_FREQ_I_OR_P,
                        out->d.getFreq.freqOffsetInPpt, in->d.getFreq.memPart, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_SET_FREQ:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (SET_FREQ)  f=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_SET_FREQ,
                        in->d.setFreq.freqOffsetInPpt, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_LOCK_STATUS:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (GET_HW_LOCK_STATUS)  ls=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_GET_HW_LOCK_STATUS,
                        out->d.getHWLockStatus.lockStatus, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_TAKE_HW_NCO_CONTROL:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (TAKE_HW_NCO_CONTROL)",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_TAKE_HW_NCO_CONTROL,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_RETURN_HW_NCO_CONTROL:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (RETURN_HW_NCO_CONTROL)",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_RETURN_HW_NCO_CONTROL,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_SET_TIME:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (SET_TIME)",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_SET_TIME,
                        0, 0, 0);
                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "       s=0x%x.%x  ns=%d  ba=%d",
                        in->d.setTime.seconds.hi, in->d.setTime.seconds.lo, in->d.setTime.nanoSeconds,
                        in->d.setTime.bBackwardAdjust,
                        0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_STEP_TIME:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (STEP_TIME)  deltaTime=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_STEP_TIME,
                        in->d.stepTime.deltaTime, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_JUMP_ACTIVE_CGU:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (JUMP_TIME_CGU)",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_JUMP_ACTIVE_CGU,
                        0, 0, 0);
                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "       s=0x%x.%x  ns=%d  ba=%d",
                        in->d.jumpActiveCGU.seconds.hi, in->d.jumpActiveCGU.seconds.lo,
                        in->d.jumpActiveCGU.nanoSeconds, in->d.jumpActiveCGU.bBackwardAdjust,
                        0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_MANUAL_FREERUN_STATUS:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (GET_HW_MANUAL_FREERUN_STATUS)  mfs=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_GET_HW_MANUAL_FREERUN_STATUS,
                        out->d.getHWManualFreerun.status, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_MANUAL_HOLDOVER_STATUS:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (GET_HW_MANUAL_HOLDOVER_STATUS)  mhs=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_GET_HW_MANUAL_HOLDOVER_STATUS,
                        out->d.getHWManualHoldover.status, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_SYNC_INPUT_EN_STATUS:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (GET_HW_SYNC_INPUT_EN_STATUS)  sies=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_GET_HW_SYNC_INPUT_EN_STATUS,
                        out->d.getHWSyncInputEn.status, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_OUT_OF_RANGE_STATUS:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (GET_HW_OUT_OF_RANGE_STATUS)  oors=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_GET_HW_OUT_OF_RANGE_STATUS,
                        out->d.getHWOutOfRange.status, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_DEVICE_PARAMS_INIT:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (DEVICE_PARAMS_INIT)",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_DEVICE_PARAMS_INIT,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_CURRENT_REF_GET:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (CURRENT_REF_GET)  ref=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_CURRENT_REF_GET,
                        out->d.getCurrRef.ref, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_CURRENT_REF_SET:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (CURRENT_REF_SET)  ref=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_CURRENT_REF_SET,
                        in->d.setCurrRef.ref, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_INPUT_PHASE_ERROR_WRITE:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (INPUT_PHASE_ERROR_WRITE)  tieNs=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_INPUT_PHASE_ERROR_WRITE,
                        in->d.inputPhaseErrorWrite.tieNs, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_INPUT_PHASE_ERROR_WRITE_CTRL_READY:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (INPUT_PHASE_ERROR_WRITE_CTRL_READY)  ready=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_INPUT_PHASE_ERROR_WRITE_CTRL_READY,
                        out->d.inputPhaseErrorWriteCtrlReady.ready, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_REGISTER_TOD_DONE_FUNC:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (REGISTER_TOD_DONE_FUNC)  TODdoneFuncPtr=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_REGISTER_TOD_DONE_FUNC,
                        in->d.setTODDoneFunc.TODdoneFuncPtr, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_CONFIRM_HW_CNTRL:

                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_DEVICE_INFO:

                break;

            case ZL303XX_DPLL_DRIVER_MSG_BC_HYBRID_ACTION_PHASE_LOCK:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_BC_HYBRID_ACTION_PHASE_LOCK,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_BC_HYBRID_ACTION_OUT_OF_LOCK:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_BC_HYBRID_ACTION_OUT_OF_LOCK,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_INITIAL:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_INITIAL,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_STAGE1:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_STAGE1,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_STAGE2:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_STAGE2,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_SET_NCO_ASSIST_ENABLE:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (SET_NCO_ASSIST_ENABLE)  enable=%d",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_SET_NCO_ASSIST_ENABLE,
                        in->d.NCOAssistEnable.enable, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_ENABLE:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (GET_NCO_ASSIST_ENABLE)",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_ENABLE,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_PARAMS:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (GET_NCO_ASSIST_PARAMS)",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_PARAMS,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_FREQ_OFFSET:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (GET_NCO_ASSIST_FREQ_OFFSET)",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_FREQ_OFFSET,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_HW_LOCK_STATUS:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (GET_NCO_ASSIST_HW_LOCK_STATUS)",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_HW_LOCK_STATUS,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_SYNC_INPUT_EN_STATUS:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (GET_NCO_ASSIST_SYNC_INPUT_EN_STATUS)",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_SYNC_INPUT_EN_STATUS,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_OUT_OF_RANGE_STATUS:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (GET_NCO_ASSIST_OUT_OF_RANGE_STATUS)",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_OUT_OF_RANGE_STATUS,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_MANUAL_HOLDOVER_STATUS:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (GET_NCO_ASSIST_MANUAL_HOLDOVER_STATUS)",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_MANUAL_HOLDOVER_STATUS,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_MANUAL_FREERUN_STATUS:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (GET_NCO_ASSIST_MANUAL_FREERUN_STATUS)",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_MANUAL_FREERUN_STATUS,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_MODIFY_STEP_TIME_NS:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (GET_MODIFY_STEP_TIME_NS)",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_GET_MODIFY_STEP_TIME_NS,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_PHASE_STEP_OUTPUT_IS_HP:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (PHASE_STEP_OUTPUT_IS_HP)",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_PHASE_STEP_OUTPUT_IS_HP,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_SET_MODE_HOLDOVER:

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: s=%d  hwP=%p  msg=%d (SET_MODE_HOLDOVER)",
                        statusToLog, hwParams, ZL303XX_DPLL_DRIVER_MSG_SET_MODE_HOLDOVER,
                        0, 0, 0);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_CLEAR_HOLDOVER_FFO:
            case ZL303XX_DPLL_DRIVER_MSG_SET_AFBDIV:
            case ZL303XX_DPLL_DRIVER_MSG_DETERMINE_MAX_STEP_SIZE_PER_ADJUSTMENT:
            case ZL303XX_DPLL_DRIVER_MSG_GET_STEP_TIME_CURR_MAX_STEP_SIZE:
            case ZL303XX_DPLL_DRIVER_MSG_ADJUST_TIME:
            case ZL303XX_DPLL_DRIVER_MSG_ADJUST_TIME_DCO_READABLE:
            default:
                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR: UNSUPPORTED s=%d  hwP=%p  msg=%d",
                        statusToLog, hwParams, in->dpllMsgType,
                        0, 0, 0);
                break;
        }
    }

    return status;
}


/* zl303xx_Dpll77xMsgRouter */
/**

   Mesage router for 77x driver.

  Parameters:
   [in]   hwParams  Pointer to the device parameter structure.
   [in]   inData    In data
   [in]   outData   Out data

*******************************************************************************/
Sint32T zl303xx_Dpll77xMsgRouter(void *hwParams, void *inData, void *outData)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_DriverMsgInDataS *in;
    zl303xx_DriverMsgOutDataS *out;


    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(inData);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(outData);
    }

    if (status == ZL303XX_OK)
    {
        in  = (zl303xx_DriverMsgInDataS *)inData;
        out = (zl303xx_DriverMsgOutDataS *)outData;
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_UNSUPPORTED_MSG_ROUTER_OPERATION;

        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "771MR Call: hwP=%p  msg=%d",
            hwParams, in->dpllMsgType, 0, 0, 0, 0);

        switch( in->dpllMsgType )
        {
            case ZL303XX_DPLL_DRIVER_MSG_GET_DEVICE_INFO:

                status = zl303xx_Dpll77xGetDeviceInfo(hwParams,
                                        &(out->d.getDeviceInfo.devType),
                                        &(out->d.getDeviceInfo.devId),
                                        &(out->d.getDeviceInfo.devRev));
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_FREQ_I_OR_P:

                status = zl303xx_Dpll77xGetFreq(hwParams,
                                        &(out->d.getFreq.freqOffsetInPpt),
                                        in->d.getFreq.memPart);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_SET_FREQ:

                status = zl303xx_Dpll77xSetFreq(hwParams,
                                        in->d.setFreq.freqOffsetInPpt);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_TAKE_HW_NCO_CONTROL:

                status = zl303xx_Dpll77xTakeHwNcoControl(hwParams);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_RETURN_HW_NCO_CONTROL:

                status = zl303xx_Dpll77xReturnHwNcoControl(hwParams);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_SET_TIME:

                status = zl303xx_Dpll77xToDWrite(hwParams,
                                        ZLS3077X_TWT_writeOnInternal1Hz,
                                        ZL303XX_TRUE,
                                        in->d.setTime.seconds,
                                        in->d.setTime.nanoSeconds,
                                        in->d.setTime.bBackwardAdjust);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_STEP_TIME:

                status = zl303xx_Dpll77xOutputPhaseStepWrite(hwParams,
                                        in->d.stepTime.deltaTime,
                                        in->d.stepTime.inCycles,
                                        in->d.stepTime.clockFreq);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_JUMP_ACTIVE_CGU:

                status = zl303xx_Dpll77xjumpActiveCGU(hwParams,
                                        in->d.jumpActiveCGU.seconds,
                                        in->d.jumpActiveCGU.nanoSeconds,
                                        in->d.jumpActiveCGU.bBackwardAdjust);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_MANUAL_FREERUN_STATUS:

                status = zl303xx_Dpll77xGetHwManualFreerunStatus(hwParams,
                                        &(out->d.getHWManualFreerun.status));
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_MANUAL_HOLDOVER_STATUS:

                status = zl303xx_Dpll77xGetHwManualHoldoverStatus(hwParams,
                                        &(out->d.getHWManualHoldover.status));
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_SYNC_INPUT_EN_STATUS:

                status = zl303xx_Dpll77xGetHwSyncInputEnStatus(hwParams,
                                        &(out->d.getHWSyncInputEn.status));
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_OUT_OF_RANGE_STATUS:

                status = zl303xx_Dpll77xGetHwOutOfRangeStatus(hwParams,
                                        &(out->d.getHWOutOfRange.status));
                break;

            case ZL303XX_DPLL_DRIVER_MSG_DEVICE_PARAMS_INIT:

                status = zl303xx_Dpll77xParamsInit(hwParams);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_CURRENT_REF_GET:

                status = zl303xx_Dpll77xCurrRefGet(hwParams,
                                        &(out->d.getCurrRef.ref));
                break;

            case ZL303XX_DPLL_DRIVER_MSG_CURRENT_REF_SET:

                status = zl303xx_Dpll77xCurrRefSet(hwParams,
                                        in->d.setCurrRef.ref);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_INPUT_PHASE_ERROR_WRITE:

                status = zl303xx_Dpll77xInputPhaseErrorWrite(hwParams,
                                        in->d.inputPhaseErrorWrite.tieNs,
                                        ZL303XX_FALSE);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_INPUT_PHASE_ERROR_WRITE_CTRL_READY:

                status = zl303xx_Dpll77xTieCtrlReady(hwParams,
                                        &(out->d.inputPhaseErrorWriteCtrlReady.ready));
                break;

            case ZL303XX_DPLL_DRIVER_MSG_REGISTER_TOD_DONE_FUNC:

                status = zl303xx_Dpll77xRegisterStepDoneFunc(hwParams,
                                        in->d.setTODDoneFunc.TODdoneFuncPtr);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_CONFIRM_HW_CNTRL:

                status = zl303xx_Read(hwParams, NULL,
                                        in->d.confirmHwCntrl.addr,
                                        &(out->d.confirmHwCntrl.data));
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_HW_LOCK_STATUS:

                status = zl303xx_Dpll77xGetHwLockStatus(hwParams,
                                        &(out->d.getHWLockStatus.lockStatus));
                break;

            case ZL303XX_DPLL_DRIVER_MSG_BC_HYBRID_ACTION_PHASE_LOCK:

                status = zl303xx_Dpll77xBCHybridActionPhaseLock(hwParams);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_BC_HYBRID_ACTION_OUT_OF_LOCK:

                status = zl303xx_Dpll77xBCHybridActionOutOfLock(hwParams);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_INITIAL:

                status = zl303xx_Dpll77xSetActiveElecActionsInitial(hwParams,
                                        in->d.setActiveElecActionsInitial.refId,
                                        in->d.setActiveElecActionsInitial.syncId);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_STAGE1:

                status = zl303xx_Dpll77xSetActiveElecActionsTransientStage1(hwParams,
                                        in->d.setActiveElecActionsStage1.refId);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_STAGE2:

                status = zl303xx_Dpll77xSetActiveElecActionsTransientStage2(hwParams,
                                        in->d.setActiveElecActionsStage2.refId);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_SET_NCO_ASSIST_ENABLE:

                status = zl303xx_Dpll77xSetNCOAssistEnable(hwParams,
                                        in->d.NCOAssistEnable.enable);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_ENABLE:

                status = zl303xx_Dpll77xGetNCOAssistEnable(hwParams,
                                        &(out->d.NCOAssistEnable.enable));
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_PARAMS:

                status = zl303xx_Dpll77xGetNCOAssistParamsSAssociation(hwParams,
                                    (zl303xx_ParamsS **)&(out->d.getNCOAssistParams.NCOAssistParams));
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_FREQ_OFFSET:
                {
                    zl303xx_ParamsS *NCOAssistPairHwParams;

                    status = zl303xx_Dpll77xGetNCOAssistParamsSAssociation(hwParams,
                                        &NCOAssistPairHwParams);
                    if( status == ZL303XX_OK )
                    {
                        status = zl303xx_Dpll77xGetFreq(NCOAssistPairHwParams,
                                        &(out->d.getNCOAssistPairFreqOffset.freqOffsetInPpt),
                                        in->d.getNCOAssistPairFreqOffset.memPart);
                    }
                }
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_HW_LOCK_STATUS:
                {
                    zl303xx_ParamsS *NCOAssistPairHwParams;

                    status = zl303xx_Dpll77xGetNCOAssistParamsSAssociation(hwParams,
                                        &NCOAssistPairHwParams);
                    if( status == ZL303XX_OK )
                    {
                        status = zl303xx_Dpll77xGetNCOAssistHwLockStatus(NCOAssistPairHwParams,
                                        &(out->d.getNCOAssistPairHWLockStatus.lockStatus));
                    }
                }
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_SYNC_INPUT_EN_STATUS:
                {
                    zl303xx_ParamsS *NCOAssistPairHwParams;

                    status = zl303xx_Dpll77xGetNCOAssistParamsSAssociation(hwParams,
                                        &NCOAssistPairHwParams);
                    if( status == ZL303XX_OK )
                    {
                        status = zl303xx_Dpll77xGetHwSyncInputEnStatus(NCOAssistPairHwParams,
                                        &(out->d.getNCOAssistSyncInputEn.status));
                    }
                }
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_OUT_OF_RANGE_STATUS:
                {
                    zl303xx_ParamsS *NCOAssistPairHwParams;

                    status = zl303xx_Dpll77xGetNCOAssistParamsSAssociation(hwParams,
                                        &NCOAssistPairHwParams);
                    if( status == ZL303XX_OK )
                    {
                        status = zl303xx_Dpll77xGetHwOutOfRangeStatus(NCOAssistPairHwParams,
                                        &(out->d.getNCOAssistOutOfRange.status));
                    }
                }
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_MANUAL_HOLDOVER_STATUS:
                {
                    zl303xx_ParamsS *NCOAssistPairHwParams;

                    status = zl303xx_Dpll77xGetNCOAssistParamsSAssociation(hwParams,
                                        &NCOAssistPairHwParams);
                    if( status == ZL303XX_OK )
                    {
                        status = zl303xx_Dpll77xGetHwManualHoldoverStatus(NCOAssistPairHwParams,
                                        &(out->d.getNCOAssistManualHoldover.status));
                    }
                }
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_MANUAL_FREERUN_STATUS:
                {
                    zl303xx_ParamsS *NCOAssistPairHwParams;

                    status = zl303xx_Dpll77xGetNCOAssistParamsSAssociation(hwParams,
                                        &NCOAssistPairHwParams);
                    if( status == ZL303XX_OK )
                    {
                        status = zl303xx_Dpll77xGetHwManualFreerunStatus(NCOAssistPairHwParams,
                                        &(out->d.getNCOAssistManualFreerun.status));
                    }
                }
                break;

            case ZL303XX_DPLL_DRIVER_MSG_GET_MODIFY_STEP_TIME_NS:

                status = zl303xx_Dpll77xModifyStepTimeNs(hwParams,
                                        in->d.getModifiedStepTimeValue.stepTimeValueIn,
                                        &(out->d.getModifiedStepTimeValue.stepTimeValueOut));
                break;

            case ZL303XX_DPLL_DRIVER_MSG_PHASE_STEP_OUTPUT_IS_HP:
                {
                    Uint32T phaseStepMaskGp;
                    Uint32T phaseStepMaskHp;
                    ZLS3077X_OutputTypesE outputTypeThatDrivesTimeStamper;
                    ZLS3077X_OutputsE outputNumThatDrivesTimeStamper;

                    status = zl303xx_Dpll77xPhaseStepMaskGet(hwParams, &phaseStepMaskGp, &phaseStepMaskHp,
                                &outputTypeThatDrivesTimeStamper, &outputNumThatDrivesTimeStamper);
                    if( status == ZL303XX_OK )
                    {
                        out->d.isOutputTypeThatDrivesTimeStamperHP.b = (outputTypeThatDrivesTimeStamper ==
                                                                ZLS3077X_OUTPUT_TYPE_HP);
                    }
                }
                break;

            case ZL303XX_DPLL_DRIVER_MSG_SET_MODE_HOLDOVER:

                status = zl303xx_Dpll77xModeSet(hwParams, ZLS3077X_DPLL_MODE_HOLDOVER);
                break;

            case ZL303XX_DPLL_DRIVER_MSG_CLEAR_HOLDOVER_FFO:
            case ZL303XX_DPLL_DRIVER_MSG_SET_AFBDIV:
            case ZL303XX_DPLL_DRIVER_MSG_DETERMINE_MAX_STEP_SIZE_PER_ADJUSTMENT:
            case ZL303XX_DPLL_DRIVER_MSG_GET_STEP_TIME_CURR_MAX_STEP_SIZE:
            case ZL303XX_DPLL_DRIVER_MSG_ADJUST_TIME:
            case ZL303XX_DPLL_DRIVER_MSG_ADJUST_TIME_DCO_READABLE:
            default:
                break;
        }

        zl303xx_Dpll77xMsgRouterLog(hwParams, inData, outData, status);
    }

    return status;
}

/*****************   END   ****************************************************/


