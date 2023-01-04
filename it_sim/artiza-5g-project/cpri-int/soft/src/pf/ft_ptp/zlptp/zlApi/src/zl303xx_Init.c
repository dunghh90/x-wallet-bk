

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     ZL303XX_ initialization functions.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Init.h"
#include "zl303xx_RdWr.h"
#include "zl303xx_Trace.h"

#include "zl303xx_Var.h"
#include "zl303xx_Params.h"

#if defined ZLS30341_INCLUDED
#include "zl303xx_ApiInterrupt.h"
#include "zl303xx_Interrupt.h"
#include "zl303xx_Dpll34xDpllLow.h"
#include "zl303xx_Dpll34xDpllConfigs.h"
#endif

#if defined ZLS30751_INCLUDED
#include "zl303xx_Dpll75x.h"
#endif

#if defined ZLS30721_INCLUDED
#include "zl303xx_Dpll72x.h"
#endif

#if defined ZLS30701_INCLUDED
#include "zl303xx_Dpll70x.h"
#endif

#if defined ZLS30771_INCLUDED
#include "zl303xx_Dpll77x.h"
#endif

#include <string.h>


/*****************   DEFINES     **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   IM/EXPORTED GLOBAL VARIABLES   ***************************/
extern Uint8T TARGET_DPLL;

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/*****************   FWD FUNCTION DEFINITIONS   **************************/

/**

  Function Name:
   zl303xx_InitAPI

  Details:
   Performs global initialization of the API for use. This must be called once
   on application startup

  Parameters:
        None

  Return Value:
   ZlStatusE

*******************************************************************************/

zlStatusE zl303xx_InitApi(void)
{
   zlStatusE status = ZL303XX_OK;

#if defined ZLS30341_INCLUDED
   if(( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE )
     )
   {
      if (status == ZL303XX_OK)
      {
         status = zl303xx_ApiInterruptInit();
      }
   }
#endif

   if (status == ZL303XX_OK)
   {
      /* call the initialize function for read / write functions. This is an
         application-wide action, so will only do anything for the 1st call */
      status = zl303xx_ReadWriteInit();
   }

   return status;
}

/**

  Function Name:
   zl303xx_CloseApi

  Details:
   Performs global closedown of the API for use. If this is called it must only
   be called once on application shutdown which must be after each individual
   device has been closed down.

  Parameters:
        None

  Return Value:
   ZlStatusE

*******************************************************************************/

zlStatusE zl303xx_CloseApi(void)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      /* call the closedown function for the Read/Write system */
      status = zl303xx_ReadWriteClose();
   }

   return status;
}

/**

  Function Name:
   zl303xx_InitDeviceStructInit

  Details:
   Initializes the parameter structure for the zl303xx_InitDevice function

  Parameters:
   [in]    zl303xx_Params   Pointer to the structure for this device instance.
   [in]    par            Pointer to the parameter structure for the function
                  For details see zl303xx_InitDevice()

  Return Value:
   ZlStatusE

*******************************************************************************/

zlStatusE zl303xx_InitDeviceStructInit(zl303xx_ParamsS *zl303xx_Params, zl303xx_InitDeviceS *par, zl303xx_DeviceModeE deviceMode)
{
   zlStatusE status = ZL303XX_OK;
#if defined ZLS30341_INCLUDED
   zl303xx_DpllConfigS dpllConfig;
#endif

   ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 2,
         "zl303xx_InitDeviceStructInit",
         0,0,0,0,0,0);

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(zl303xx_Params) |
               ZL303XX_CHECK_POINTER(par);
   }

#if defined ZLS30341_INCLUDED
   if(( zl303xx_Params->deviceType == ZL3034X_DEVICETYPE )
     )
   {
      if (status == ZL303XX_OK)
      {
         memset(&dpllConfig, 0, sizeof(zl303xx_DpllConfigS));

         if (status == ZL303XX_OK)
         {
            /* System frequency */
            par->sysClockFreqHz = (Uint32T)(ZL303XX_PLL_SYSTEM_CLK_KHZ * 1000);

            /* DCO frequency */
            par->dcoClockFreqHz = (Uint32T)(ZL303XX_PLL_INTERNAL_FREQ_KHZ * 1000);
         }

         if (status == ZL303XX_OK && par->pllInit.pllId == TARGET_DPLL) /* TsEng Only on NCO device */
         {
            /* Initialize the TsEngine structure */
            status = zl303xx_TsEngineInitStructInit(zl303xx_Params, &par->tsEngInit);
         }

         if (status == ZL303XX_OK)
         {
            if (par->pllInit.TopClientMode == 0) /* DPLL is not already configured (assume memory is zeroed) so default it */
            {
               if (status == ZL303XX_OK)
               {
                  /* Initialize the Pll structure */
                  status = zl303xx_PllInitStructInit(zl303xx_Params, &par->pllInit);
               }

               if (deviceMode == ZL303XX_MODE_REF_EXTERNAL)
                  par->pllInit.TopClientMode = ZL303XX_FALSE;
               else
                  par->pllInit.TopClientMode = ZL303XX_TRUE;

               if (status == ZL303XX_OK)
               {
                  /* Set up the default Dpll Config data structure */
                  status = zl303xx_DpllConfigStructInit(zl303xx_Params, &dpllConfig);
               }

            #if !defined _USE_DEV_AS_TS_DEV
               if (status == ZL303XX_OK)
               {
                  /* Set up the defaults for Dpll1 and Dpll2 */
                  status = zl303xx_DpllConfigDefaults(zl303xx_Params, &dpllConfig, par->pllInit.TopClientMode);
               }
/* Original
 *          #else
 *             #warning _USE_DEV_AS_TS_DEV
 */
            #endif
             }
             else
             {
                ZL303XX_TRACE_ALWAYS("zl303xx_InitDeviceStructInit(): using existing DPLL config info rather than defaults", 0,0,0,0,0,0);
             }
         }
      }
   }
#else
   if (status == ZL303XX_OK)
   {
      par->pllInit.TopClientMode = (deviceMode == ZL303XX_MODE_REF_TOP) ? 1 : 0;
   }
#endif

   return status;
}

/**

  Function Name:
   zl303xx_InitDevice

  Details:
   Initialize the zl303xx device

  Parameters:
   [in]    zl303xx_Params      Pointer to the structure for this device instance.
   [in]    driverMsgRouter   Pointer to the driver routine.
   [in]    par               Parameter structure

Structure inputs:

       sysClockFreqHz    The system clock rate

       tsEngInit         Structure containing initialization parameters for the
                     timestamp engine. See function zl303xx_TsEngInit() in file
                     zl303xx_TsEng.c for details
       pllInit           Structure containing initialization parameters for the
                     PLL. See function zl303xx_PllInit() in file zl303xx_Dpll34xDpllConfigs.h for details

  Return Value:
   ZlStatusE

*******************************************************************************/

zlStatusE zl303xx_InitDevice(zl303xx_ParamsS *zl303xx_Params,
                hwFuncPtrDriverMsgRouter driverMsgRouter, zl303xx_InitDeviceS *par)
{
   zlStatusE status = ZL303XX_OK;
   driverMsgRouter = driverMsgRouter;

   ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 1,
         "zl303xx_InitDevice",
         0,0,0,0,0,0);

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(zl303xx_Params) |
               ZL303XX_CHECK_POINTER(par);
   }

   /* Check device not previously initialized */
   if (status == ZL303XX_OK)
   {
      if (zl303xx_Params->initState != ZL303XX_INIT_STATE_NONE)
      {
         status = ZL303XX_INIT_ERROR;
         ZL303XX_ERROR_TRAP("Device already initialized");
      }
   }

#if defined ZLS30341_INCLUDED
   if(( zl303xx_Params->deviceType == ZL3034X_DEVICETYPE )
     )
   {
      if (status == ZL303XX_OK)
      {
         if ((par->sysClockFreqHz == (Uint32T)ZL303XX_INVALID) ||
             (par->sysClockFreqHz == 0))
         {
            status = ZL303XX_PARAMETER_INVALID;
            ZL303XX_ERROR_TRAP("Invalid system clock rate parameter");
         }
      }

      /* Set the top level interrupt mask registers so that all device interrupts are masked */
      if (status == ZL303XX_OK)
      {
         /* Hi priority interrupt */
         status = zl303xx_InterruptSetTopMask(zl303xx_Params, ZL303XX_HI_PRI_INT_NUM, 0, 0xff);
      }

      /* Fill Device and Revision ID members */
      if (status == ZL303XX_OK)
      {
          status = zl303xx_InitDeviceIdAndRev(zl303xx_Params);
      }
   }
#endif

#if defined ZLS30341_INCLUDED
   if(( zl303xx_Params->deviceType == ZL3034X_DEVICETYPE )
     )
   {
      if (status == ZL303XX_OK)
      {
         status = ZL303XX_LOCK_DEV_PARAMS(zl303xx_Params);

         if (status == ZL303XX_OK)
         {
            /* Store the SYSTEM frequency and period */
            zl303xx_Params->sysClockFreqHz = par->sysClockFreqHz;
            zl303xx_Params->sysClockPeriod = ClockPeriod_ScaledNs(par->sysClockFreqHz);

            /* Store the SYSTEM frequency and period */
            zl303xx_Params->dcoClockFreqHz = par->dcoClockFreqHz;
            zl303xx_Params->dcoClockPeriod = ClockPeriod_ScaledNs(par->dcoClockFreqHz);

            zl303xx_Params->initState = ZL303XX_INIT_STATE_DEV_RESET;

            ZL303XX_UNLOCK_DEV_PARAMS(zl303xx_Params);

         }
      }
   }
#endif

#if defined ZLS30341_INCLUDED
   if(( zl303xx_Params->deviceType == ZL3034X_DEVICETYPE )
     )
   {
      if (status == ZL303XX_OK)
      {
         /* Initialize the timestamp engine */
         status = zl303xx_PllInit(zl303xx_Params, &par->pllInit);
      }
   }
#endif

#if defined ZLS30341_INCLUDED
   if(( zl303xx_Params->deviceType == ZL3034X_DEVICETYPE )
     )
   {
#if defined ZLS30341_INCLUDED
      if (status == ZL303XX_OK && par->pllInit.pllId == TARGET_DPLL) /* TsEng Only on NCO device */
#else
      if (status == ZL303XX_OK)
#endif
      {
         /* Initialize the timestamp engine */
         status = zl303xx_TsEngineInit(zl303xx_Params, &par->tsEngInit);
      }
   }
#endif

#if defined ZLS30361_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30721_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
   if (status == ZL303XX_OK)
   {
      if(( zl303xx_Params->deviceType == ZL3036X_DEVICETYPE ) ||
         ( zl303xx_Params->deviceType == ZL3070X_DEVICETYPE ) ||
         ( zl303xx_Params->deviceType == ZL3072X_DEVICETYPE ) ||
         ( zl303xx_Params->deviceType == ZL3075X_DEVICETYPE ) ||
         ( zl303xx_Params->deviceType == ZL3077X_DEVICETYPE ))
      {
         zl303xx_DriverMsgInDataS in;
         zl303xx_DriverMsgOutDataS out;

         if( status == ZL303XX_OK )
         {
            in.dpllMsgType = ZL303XX_DPLL_DRIVER_MSG_DEVICE_PARAMS_INIT;
         }
         if( status == ZL303XX_OK )
         {
            if( driverMsgRouter != NULL )
            {
               status = driverMsgRouter(zl303xx_Params, &in, &out);
            }
         }
      }
   }
#endif

#if defined ZLS30721_INCLUDED
   if (status == ZL303XX_OK)
   {
      if( zl303xx_Params->deviceType == ZL3072X_DEVICETYPE )
      {
         status = zl303xx_Dpll72xTaskStart();
      }
   }
#endif

   if (status == ZL303XX_OK)
   {
      /* Initialization complete */
      if (zl303xx_Params->initState == (Uint32T)ZL303XX_INIT_STATE_DONE -1)
      {
         zl303xx_Params->initState = ZL303XX_INIT_STATE_DONE;
      }
   }

   return status;
}

/**

  Function Name:
   zl303xx_CloseDeviceStructInit

  Details:
   Initializes the parameter structure for the zl303xx_CloseDevice function

  Parameters:
   [in]    zl303xx_Params   Pointer to the structure for this device instance.
   [in]    par            Pointer to the parameter structure for the function
                  For details see zl303xx_CloseDevice()

  Return Value:
   ZlStatusE

*******************************************************************************/

zlStatusE zl303xx_CloseDeviceStructInit(zl303xx_ParamsS *zl303xx_Params, zl303xx_CloseDeviceS *par)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 2,
         "zl303xx_CloseDeviceStructInit",
         0,0,0,0,0,0);

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(zl303xx_Params);
   }

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(par);
   }

#if defined ZLS30341_INCLUDED
   if(( zl303xx_Params->deviceType == ZL3034X_DEVICETYPE )
     )
   {
      if (status == ZL303XX_OK)
      {
         status = zl303xx_TsEngineCloseStructInit(zl303xx_Params, &par->tsEngClose);
      }
   }
#endif

   return status;
}

/**

  Function Name:
   zl303xx_CloseDevice

  Details:
   Close down the zl303xx device

  Parameters:
   [in]    zl303xx_Params   Pointer to the structure for this device instance.
   [in]    par            Pointer to the parameter structure for the function

  Return Value:
   ZlStatusE

*******************************************************************************/

zlStatusE zl303xx_CloseDevice(zl303xx_ParamsS *zl303xx_Params, zl303xx_CloseDeviceS *par)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 1,
         "zl303xx_CloseDevice",
         0,0,0,0,0,0);

   /* Check any locally referenced input pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(zl303xx_Params) |
               ZL303XX_CHECK_POINTER(par);
   }

   /* Shutdown the Tx Packet scheduler */
   if (status == ZL303XX_OK)
   {
      /* Update the device state */
      zl303xx_Params->initState = ZL303XX_INIT_STATE_CLOSE_DOWN;
   }

#if defined ZLS30701_INCLUDED
   if (status == ZL303XX_OK)
   {
      if( zl303xx_Params->deviceType == ZL3070X_DEVICETYPE )
      {
          status = zl303xx_Dpll70xParamsClose(zl303xx_Params);
      }
   }
#endif
#if defined ZLS30751_INCLUDED
   if (status == ZL303XX_OK)
   {
      if( zl303xx_Params->deviceType == ZL3075X_DEVICETYPE )
      {
          status = zl303xx_Dpll75xParamsClose(zl303xx_Params);
      }
   }
#endif

#if defined ZLS30341_INCLUDED
   if(( zl303xx_Params->deviceType == ZL3034X_DEVICETYPE )
     )
   {
      Sint32T idx;

      /* Disable all interrupts in the top level interrupt register */
      for (idx = 0; (idx < ZL303XX_NUM_DEVICE_IRQS) && (status == ZL303XX_OK); idx++)
      {
         status = zl303xx_InterruptSetTopMask(zl303xx_Params, (Uint32T)idx,
                                              0,             /* register value */
                                              (Uint8T)0xFF); /* mask */
      }
   }
#endif
#if defined ZLS30771_INCLUDED
   if (status == ZL303XX_OK)
   {
      if( zl303xx_Params->deviceType == ZL3077X_DEVICETYPE )
      {
          status = zl303xx_Dpll77xParamsClose(zl303xx_Params);
      }
   }
#endif

#if defined ZLS30341_INCLUDED
   if(( zl303xx_Params->deviceType == ZL3034X_DEVICETYPE )
     )
   {
      /* Close down the timestamp engine */
      if (status == ZL303XX_OK)
      {
         status = zl303xx_TsEngineClose(zl303xx_Params, &par->tsEngClose);
      }
   }
#endif

   /* Update the device state if everything was successful. Otherwise, leave the
      state as CLOSE_DOWN */
   if (status == ZL303XX_OK)
   {
      zl303xx_Params->initState = ZL303XX_INIT_STATE_NONE;
   }

   return status;
}

/*****************   END   ****************************************************/

