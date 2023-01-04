

/*******************************************************************************
 *
 *  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
 *
 *  Copyright 2006-2018 Microsemi Semiconductor Limited.
 *  All rights reserved. 
 *
 *  Module Description:
 *     Supporting code for the 77x examples
 *
 ******************************************************************************/


/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_Os.h"
#include "zl303xx_Trace.h"
#include "zl303xx_Macros.h"
#include "zl303xx_Var.h"
#include "zl303xx_Porting.h"
#include "zl303xx_SpiPort.h"
#include "zl303xx_ErrTrace.h"
#include "zl303xx_Init.h"
#include "zl303xx_ExampleMain.h"

#if defined APR_INCLUDED
#include "zl303xx_ExampleAprGlobals.h"
#include "zl303xx_ExampleUtils.h"
#endif


#include "zl303xx_Params.h"
#include "zl303xx_Example77x.h"
#include "zl303xx_AddressMap77x.h"
#include "zl303xx_Dpll771.h"
#include "zl303xx_Dpll77x.h"
#include "zl303xx_HWTimer.h"


#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
#if defined OS_LINUX
#include "zl303xx_ZLE30360.h"
#endif
#endif

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
#include "zl303xx_ApiConfig.h"
#endif

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#if defined ZL_LNX_DENX

extern char *strtok_r (char *, __const char *, char **);
#endif

/*****************   DEFINES     **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

extern Uint8T TARGET_DPLL;

/*****************   STATIC FUNCTION DEFINITIONS   **************************/

zlStatusE ZLE_GPIOSet(void *params, Uint32T gpio, Uint8T value);

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* example77xStickyLockCallout */
/**
   Example for a 77x sticky register callout handler, currently bound within
   exampleAppStructInit().

  Parameters:
   [in]   hwParams   Pointer to the device instance parameter structure.
   [in]   pllId      Associated PLL.
   [in]   lockFlag   Flag indicating lock or unlock.

  Return Value:  ZL303XX_OK          Success.

*******************************************************************************/
zlStatusE example77xStickyLockCallout(void *hwParams, zl303xx_DpllIdE pllId, zl303xx_BooleanE lockFlag)
{
    /* Every call to zl303xx_Dpll77xStickyLockSet will callout to here */

    /* In hybrid/elec mode, APR will get here every 125ms */
    /* DON'T BLOCK LONG on a mutex or APR will have issues in hybrid/elec mode! */

    zlStatusE status = ZL303XX_OK;

    if( hwParams ) {}  /* Warning removal */
    if( pllId ) {}  /* Warning removal */

    if (status == ZL303XX_OK)
    {
        /* This log generates a lot of output that is usually not usefull.
           However, if you do need it, un-comment the line. */
        /*
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                    "example77xStickyLockCallout: called with hwParams=%p, pllId=%d and lockFlag=%d",
                            hwParams, pllId, lockFlag, 0,0,0);
        */
        if (lockFlag)
        {
            ;   /* Lock take by another consumer - take some mutex? */
        }
        if (!lockFlag)
        {
            ;   /* Lock give by another consumer - give some mutex? */
        }
    }

    return status;
}

/* example77xEnvInit */
/**
   Initializes the 77x environment and the global variables used in this
   example code.

*******************************************************************************/
zlStatusE example77xEnvInit(void)
{
   zlStatusE status = ZL303XX_OK;

   /********* Generic API initialization. Called once per application *********/
   /* Initialize the API */
   if ((status == ZL303XX_OK) && (status = zl303xx_InitApi()) != ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_InitApi() failed with status = %lu",
            status, 0,0,0,0,0);
   }

   return status;
}


/* example77xEnvClose */
/**
   Closes the 77x environment and resets global variables used in this
   example code, as necessary.

*******************************************************************************/
zlStatusE example77xEnvClose(void)
{
   zlStatusE status = ZL303XX_OK;

   #if !defined(ZL_I2C_RDWR)
      cpuSpiCleanUp();
/* Original
 * #else
 *    #warning Add I2C shutdown function here
 */
   #endif    /* !ZL_I2C_RDWR */

   /* Perform a total shutdown on the API */
   (void)zl303xx_CloseApi();

   ZL303XX_TRACE_ALWAYS("example77xEnvClose() completed status = %lu",
         status, 0,0,0,0,0);


   return status;
}


/* example77xClockCreateStructInit */
/**
   An example of how to initialize a 77x clock/device configuration structure,
   including necessary function bindings.

*******************************************************************************/
zlStatusE example77xClockCreateStructInit(example77xClockCreateS *pClock)
{
   zlStatusE status = ZL303XX_OK;

   /* Check input parameters */
   status = ZL303XX_CHECK_POINTER(pClock);

   /********* Generic device creation. Called once per attached device ********/
   /* Create the device instance structure */
   if ((status == ZL303XX_OK) &&
       (status = zl303xx_CreateDeviceInstance(&pClock->zl303xx_Params)) != ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS("example77xClockCreateStructInit: zl303xx_CreateDeviceInstance() failed with status = %lu or OS_CALLOC failed",
                         status, 0,0,0,0,0);
   }

   if (status == ZL303XX_OK)
   {
      pClock->deviceMode = ZL303XX_MODE_UNKNOWN;
      pClock->zl303xx_Params->pllParams.d77x.std.phaseStepMaskGp = 0;
      pClock->zl303xx_Params->pllParams.d77x.std.phaseStepMaskHp = 0;
      pClock->zl303xx_Params->pllParams.d77x.std.outputTypeThatDrivesTimeStamper = ZLS3077X_HPOUT0;
      pClock->zl303xx_Params->pllParams.d77x.std.outputNumThatDrivesTimeStamper = ZLS3077X_OUTPUT_TYPE_HP;
      pClock->zl303xx_Params->pllParams.d77x.std.stepDoneFuncPtr = NULL;
      pClock->zl303xx_Params->pllParams.d77x.std.guardStartTick = 0;
      pClock->zl303xx_Params->pllParams.d77x.std.totalStep = 0;
      pClock->zl303xx_Params->pllParams.d77x.std.STState = ZLS3077X_STS_IDLE;
      pClock->zl303xx_Params->pllParams.d77x.std.sanityStartTick = 0;

      pClock->zl303xx_Params->pllParams.d77x.drvInitialized = ZL303XX_FALSE;

   }

   return status;
}


/* example77xClockCreate */
/**
   An example of how to start the 77x APR clock/device. An APR clock represents a
   local DCO.

*******************************************************************************/
zlStatusE example77xClockCreate(example77xClockCreateS *pClock)
{
   zlStatusE status = ZL303XX_OK;
   static zl303xx_BooleanE spiInitialized = ZL303XX_FALSE;
   static cpuStatusE spiStatus;

      /* Check input parameters */
   status = ZL303XX_CHECK_POINTERS(pClock, pClock->zl303xx_Params);

   /********* SPI initialization. Called once per attached device *************/
#if !defined(ZL_I2C_RDWR)
   /* Initialize the CPU SPI interface and any device Chip Select */
   if (status == ZL303XX_OK && !spiInitialized)
   {
      spiStatus = cpuConfigSpi();
      spiInitialized = ZL303XX_TRUE;
   }
   /* Initialize the processor's SPI interface and configure the Chip Select */
   if (status == ZL303XX_OK && (spiStatus != CPU_OK || spiStatus == CPU_SPI_MULTIPLE_INIT))
   {
      if (spiStatus == CPU_SPI_MULTIPLE_INIT)
      {
         ZL303XX_ERROR_NOTIFY("Multiple attempts to initialize the SPI, ignoring the error");
      }
      else
      {
          ZL303XX_ERROR_NOTIFY("example77xClockCreate: Failed to initialize the SPI");
          status = ZL303XX_HARDWARE_ERROR;
      }
   }

   /* If the SPI init was OK, add the Spi chip select functionality. */
   if (status == ZL303XX_OK && spiStatus == CPU_OK &&
       (spiStatus = cpuConfigChipSelect(pClock->zl303xx_Params, ZL303XX_CHIP_SELECT_MASK)) != CPU_OK)
   {
      ZL303XX_TRACE_ALWAYS(" example77xClockCreate: cpuConfigChipSelect() failed with status = %lu",
                         spiStatus, 0,0,0,0,0);
      status = ZL303XX_HARDWARE_ERROR;
   }
/* Original
 * #else
 * #warning Add I2C config and chip select functions here
 */
#endif    /* !ZL_I2C_RDWR */


   if (status == ZL303XX_OK)
   {
      pClock->zl303xx_Params->deviceType = ZL3077X_DEVICETYPE;
      /* Set the PLL ID */
      pClock->zl303xx_Params->pllParams.pllId = pClock->pllId;
   }


   /********* Device Info Collection  *****************************************/
   /* Collect the device Id and Revision */
   if (status == ZL303XX_OK && (status = zl303xx_InitDeviceIdAndRev(pClock->zl303xx_Params)) != ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS(" example77xClockCreate: zl303xx_InitDeviceIdAndRev() failed with status = %lu",
            status, 0,0,0,0,0);
   }

   if (pClock->pllId == TARGET_DPLL)    /* Only one DPLL has a valid NCO clock to control the TSU */
   {
#if defined PROGRAM_TXT_ONTHEFLY
       /* Configure the ZL3077X device. */
       if (status == ZL303XX_OK)
       {
           if (pClock->zl303xx_Params)
           {
#if defined ZLE30360_770_FRANKEN_BOARD
    ZL303XX_TRACE_ALWAYS(" example77xClockCreate: BYPASSING Reset the ZL3077X device", 0,0,0,0,0,0);
#else
             /* Reset the ZL3077X device */
               ZL303XX_TRACE_ALWAYS(" example77xClockCreate: Reset the ZL3077X device", 0,0,0,0,0,0);

           #if defined _ZL303XX_ZLE1588_BOARD
               #define GPIO_ZL3077X_ENABLE  6
               #define waitMs 1000              /* Needs ~550ms */

               ZLE_GPIOSet(pClock->zl303xx_Params, GPIO_ZL3077X_ENABLE, 0);
               OS_TASK_DELAY(30);
               ZLE_GPIOSet(pClock->zl303xx_Params, GPIO_ZL3077X_ENABLE, 1);
               OS_TASK_DELAY(waitMs);
           #endif
#endif
           }

       #if defined _ZL303XX_ZLE1588_BOARD
           ZL303XX_TRACE_ALWAYS(" example77xClockCreate: Call example77xLoadConfigFile() to load all DPLL parameters", 0,0,0,0,0,0);
           status = example77xLoadConfigFile(pClock->zl303xx_Params, "ZLE30770.txt");
           if (status != ZL303XX_OK)
           {
               ZL303XX_TRACE_ALWAYS("example77xClockCreate: Check the ZLE1588 DUT SPI jumper block (JP13) for correct settings (DUT-CPU)",
                                  0,0,0,0,0,0);
           }
       #else
/* Original (元々コメント)
           ZL303XX_TRACE_ALWAYS(" example77xClockCreate: Call example77xLoadConfigDefaults() to load all DPLL parameters", 0,0,0,0,0,0);
           #warning example77xLoadConfigDefaults function call loading register values for ZL3077x device from function defaults, not from configuration file
           ZL303XX_TRACE_ALWAYS(" example77xClockCreate: example77xLoadConfigDefaults() function call loading register values for ZL3077x device from function defaults, not from configuration file", 0,0,0,0,0,0);
           status = example77xLoadConfigDefaults(pClock->zl303xx_Params);
           if (status != ZL303XX_OK)
           {
               ZL303XX_TRACE_ALWAYS("example77xClockCreate: Failed to load 77x configuration using INTERNAL DEFAULTS",
                                  0,0,0,0,0,0);
           }
 */
           /* #warning MSCC: To load config file, use the following piece of code: */

            ZL303XX_TRACE_ALWAYS(" example77xClockCreate: Call example77xLoadConfigFile() to load all DPLL parameters", 0,0,0,0,0,0);
            /* status = example77xLoadConfigFile(pClock->zl303xx_Params, "ZLE30770.txt"); */
                        
			status = example77xLoadConfigFile(pClock->zl303xx_Params, "/etc/ptp/PTP_reg_set.txt");
						
           if (status != ZL303XX_OK)
           {
               ZL303XX_TRACE_ALWAYS("example77xClockCreate: Failed to load 77x configuration using Config file",
                                  0,0,0,0,0,0);
           }
			
           #endif

           if ( exampleReset1588HostRegisterOnClockCreateGet() == ZL303XX_TRUE )
           {
               /* #warning MSCC: zl303xx_Dpll77xReset1588HostRegisters() resets the device host registers for the specific DPLL */
               status = zl303xx_Dpll77xReset1588HostRegisters(pClock->zl303xx_Params);
               if (status != ZL303XX_OK)
               {
                   ZL303XX_TRACE_ALWAYS("example77xClockCreate: Failed to reset device host registers",
                                      0,0,0,0,0,0);
               }
           }

           if (status != ZL303XX_OK)
           {
               ZL303XX_TRACE_ALWAYS("example77xClockCreate: ZL3077x failed to initialize (status %d).", status, 0,0,0,0,0);
           }

           {
               #undef waitMs
               #define waitMs 10000
               printf("\n ... WAITING %d seconds for ZL3077x to align clocks ...\n", waitMs / 1000);
               OS_TASK_DELAY(waitMs);
           }

           /* Reset the PHY device */
           /* #warning MSCC: Clocks available; Hard reset the PHY if required */
       }
#else
       ZL303XX_TRACE_ALWAYS("example77xClockCreate: Using the DPLL configuration done in uBoot", 0,0,0,0,0,0);
#endif
    }

    /* Create the mutex for mailbox data copies. This is done once per device. */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xParamsMutexInit(pClock->zl303xx_Params);
        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("example77xClockCreate: zl303xx_Dpll77xParamsMutexInit() faild: %d", status,0,0,0,0,0);
        }
    }

    /* Get a copy of the device mailbox registers; APR will use these copies instead
        of accessing the device. */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xUpdateAllMailboxCopies(pClock->zl303xx_Params);
        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("example77xClockCreate: zl303xx_Dpll77xUpdateAllMailboxCopies() failed=%d", status,0,0,0,0,0);
        }
    }


   /********* Device initialization. Called once per attached device **********/
   /* Initialize the device */
   if (status == ZL303XX_OK)
   {
      zl303xx_InitDeviceS initDevice;

      memset(&initDevice, 0, sizeof(zl303xx_InitDeviceS));

      if ((status == ZL303XX_OK) &&
          ((status = zl303xx_InitDeviceStructInit(pClock->zl303xx_Params, &initDevice, pClock->deviceMode)) != ZL303XX_OK))
      {
         ZL303XX_TRACE_ALWAYS("zl303xx_InitDeviceStructinit() failed with status = %d",
                              status, 0,0,0,0,0);
      }

      if ((status == ZL303XX_OK) &&
          (status = zl303xx_InitDevice(pClock->zl303xx_Params, zl303xx_Dpll77xMsgRouter, &initDevice)) != ZL303XX_OK)
      {
         ZL303XX_TRACE_ALWAYS("zl303xx_InitDevice() failed with status = %lu",
               status, 0,0,0,0,0);
      }
   }

   /* Setup which post dividers will apply a phase_step operation. */
   {
   #if defined _ZL303XX_ZLE1588_BOARD
      if (status == ZL303XX_OK && pClock->pllId == TARGET_DPLL)
      {
        #if defined ZLE30360_770_FRANKEN_BOARD
         status = zl303xx_Dpll77xPhaseStepMaskSet( pClock->zl303xx_Params, ZLS3077X_OUTPUT_TYPE_HP, ZLS3077X_HPOUT0, 1 );
         if (status != ZL303XX_OK)
         {
             ZL303XX_TRACE_ALWAYS("example77xClockCreate: zl303xx_Dpll77xPhaseStepMaskSet (1) failed to initialize (status %d).", status, 0,0,0,0,0);
         }
         /* Franken board uses output 0 to drive the timestamper */
         status = zl303xx_Dpll77xOutputThatDrivesTimeStamperSet(pClock->zl303xx_Params, ZLS3077X_OUTPUT_TYPE_HP, ZLS3077X_HPOUT0);
         if (status != ZL303XX_OK)
         {
             ZL303XX_TRACE_ALWAYS("example77xClockCreate: zl303xx_Dpll77xLSDIVThatDrivesTimeStamperSet failed (status %d).", status, 0,0,0,0,0);
         }
        #else
         /* 1588 board uses output 6 to drive the timestamper */
         status = zl303xx_Dpll77xPhaseStepMaskSet( pClock->zl303xx_Params, ZLS3077X_OUTPUT_TYPE_HP, ZLS3077X_HPOUT6, 1 );
         if (status != ZL303XX_OK)
         {
             ZL303XX_TRACE_ALWAYS("example77xClockCreate: zl303xx_Dpll77xPhaseStepMaskSet (3) failed to initialize (status %d).", status, 0,0,0,0,0);
         }
         status = zl303xx_Dpll77xOutputThatDrivesTimeStamperSet(pClock->zl303xx_Params, ZLS3077X_OUTPUT_TYPE_HP, ZLS3077X_HPOUT6);
         if (status != ZL303XX_OK)
         {
             ZL303XX_TRACE_ALWAYS("example77xClockCreate: zl303xx_Dpll77xLSDIVThatDrivesTimeStamperSet failed (status %d).", status, 0,0,0,0,0);
         }
        #endif
      }
   #else
      if (status == ZL303XX_OK)
      {
         /* Assume output 0 drives the timestamper */
         /* 1pps出力先はGPOUT1 */
         status = zl303xx_Dpll77xPhaseStepMaskSet( pClock->zl303xx_Params, ZLS3077X_OUTPUT_TYPE_GP, ZLS3077X_GPOUT1, 1 );
         if (status != ZL303XX_OK)
         {
         	ZL303XX_TRACE_ALWAYS("example77xClockCreate: zl303xx_Dpll77xPhaseStepMaskSet (4) GPOUT1 failed to initialize (status %d).", status, 0,0,0,0,0);
         }
         
         status = zl303xx_Dpll77xOutputThatDrivesTimeStamperSet(pClock->zl303xx_Params, ZLS3077X_OUTPUT_TYPE_GP, ZLS3077X_GPOUT1);
         if (status != ZL303XX_OK)
         {
             ZL303XX_TRACE_ALWAYS("example77xClockCreate: zl303xx_Dpll77xLSDIVThatDrivesTimeStamperSet GPOUT1 failed (status %d).", status, 0,0,0,0,0);
         }
         
         /* HPOUT5への出力も連続で実施する */
         
         status = zl303xx_Dpll77xPhaseStepMaskSet( pClock->zl303xx_Params, ZLS3077X_OUTPUT_TYPE_HP, ZLS3077X_HPOUT5, 1 );
         if (status != ZL303XX_OK)
         {
             ZL303XX_TRACE_ALWAYS("example77xClockCreate: zl303xx_Dpll77xPhaseStepMaskSet (4) HPOUT5 failed to initialize (status %d).", status, 0,0,0,0,0);
         }
         
         status = zl303xx_Dpll77xOutputThatDrivesTimeStamperSet(pClock->zl303xx_Params, ZLS3077X_OUTPUT_TYPE_HP, ZLS3077X_HPOUT5);
         if (status != ZL303XX_OK)
         {
             ZL303XX_TRACE_ALWAYS("example77xClockCreate: zl303xx_Dpll77xLSDIVThatDrivesTimeStamperSet HPOUT5 failed (status %d).", status, 0,0,0,0,0);
         }
      }
      
   #endif
   }

   /* Set the clock into the initialized mode */
   if ((status == ZL303XX_OK) && (pClock->pllId == TARGET_DPLL))
   {
       ZLS3077X_DpllHWModeE dpllMode;

       /* Convert device mode to DPLL mode */
       switch (pClock->deviceMode)
       {
          case ZL303XX_MODE_REF_EXTERNAL :        dpllMode = ZLS3077X_DPLL_MODE_AUTO_LOCK;     break;
          case ZL303XX_MODE_REF_EXTERNAL_FORCED : dpllMode = ZLS3077X_DPLL_MODE_REFLOCK;  break;
          case ZL303XX_MODE_FREERUN :             dpllMode = ZLS3077X_DPLL_MODE_FREERUN;  break;
          case ZL303XX_MODE_HOLDOVER :            dpllMode = ZLS3077X_DPLL_MODE_HOLDOVER; break;

          case ZL303XX_MODE_UNKNOWN :
          case ZL303XX_MODE_REF_BC :
          case ZL303XX_MODE_REF_TOP :
          case ZL303XX_MODE_INVALID :
          default :
             dpllMode = ZLS3077X_DPLL_MODE_NCO; break;
       }

       #if defined APR_INCLUDED
       if (!(zl303xx_AprGetDeviceOptMode() == ZL303XX_HYBRID_MODE))
       {
           status = zl303xx_Dpll77xModeSet(pClock->zl303xx_Params, dpllMode);
           ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
                   "example77xClockCreate: DPLL mode set to %d", dpllMode, 0,0,0,0,0);
       }
      #endif


       if (status != ZL303XX_OK)
       {
           ZL303XX_TRACE_ALWAYS("example77xClockCreate: zl303xx_Dpll77xModeSet(%d) failed with status=%d", dpllMode, status, 0,0,0,0);
       }
   }

   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS("example77xClockCreate() failed somewhere with status = %lu",
            status, 0,0,0,0,0);
   }

   return status;
}


#if (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD) && defined OS_LINUX
/* example77xCheckPhyTSClockFreq */
/**
   Routine to check the clock frequency goin to the Vitess.

*******************************************************************************/
zlStatusE example77xCheckPhyTSClockFreq(zl303xx_ParamsS *zl303xx_Params)
{
   zlStatusE status = ZL303XX_OK;

    /* Check DUT 1588 clock is configured the same as PHY, error if not */
   if (status == ZL303XX_OK)
   {
       const Uint32T phy1588freqs[5] = {
            125000000, /* VTSS_PHY_TS_CLOCK_FREQ_125M */
            156250000, /* VTSS_PHY_TS_CLOCK_FREQ_15625M */
            200000000, /* VTSS_PHY_TS_CLOCK_FREQ_200M */
            250000000, /* VTSS_PHY_TS_CLOCK_FREQ_250M */
            500000000, /* VTSS_PHY_TS_CLOCK_FREQ_500M */
       };
       Uint32T phy1588freq = phy1588freqs[zl303xx_VTSS_config.clockFreq % 5];
       zl303xx_Dpll77xOutputConfS clk1588conf;

       status = zl303xx_Dpll77xGetOutputClockConfig(zl303xx_Params,
                                         &clk1588conf);

       if (status == ZL303XX_OK)
       {
            /* On the 770 daughter board, HPOUT4P/HPOUT4N, MSDIV are routed
               to the Vitess so look at freq[4].msdivFreq */
            if( clk1588conf.freq[4].msdivFreq != phy1588freq )
            {
                ZL303XX_TRACE_ALWAYS("example77xCheckPhyTSClockFreq: ERROR PHY expecting %u Hz, HPOUT4P output %u Hz",
                                    phy1588freq, clk1588conf.freq[4].msdivFreq, 0,0,0,0);
                status = ZL303XX_ERROR;
            }
       }
   }

   if (status == ZL303XX_OK)
   {
       zl303xx_VTSS_config.twoStep = exampleGetTwoStepFlag(); /* Use the two step flag from PTP */
   }

   if (status == ZL303XX_OK)
   {
        /* Align TSU PPS to the ZL30362 1Hz Pulse */
        ZL303XX_TRACE_ALWAYS("Aligning CGU and TSU 1 PPS signals (Timeout 30 seconds)",
                            0,0,0,0,0,0);

        /* Sync to external pulse, 10 second timeout */
        if ((status = zl303xx_VTSS_sync_to_ext_pulse(30000)) != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("example77xCheckPhyTSClockFreq: Aligning CGU and TSU 1 PPS signals, ERROR status= %d",
                                status, 0,0,0,0,0);
        }
   }

   return status;
}
#endif


/* example77xClockRemove */
/**
   An example of how to remove an 77x clock/device

*******************************************************************************/
zlStatusE example77xClockRemove(example77xClockCreateS *pClock)
{
   zlStatusE status = ZL303XX_OK;

   /* Delete the mutex for mailbox data copies */
   if (status == ZL303XX_OK)
   {
       status = zl303xx_Dpll77xParamsMutexDelete(pClock->zl303xx_Params);
       if (status != ZL303XX_OK)
       {
          ZL303XX_TRACE_ALWAYS("example77xClockRemove: zl303xx_Dpll77xParamsMutexDelete() failed with status = %lu",
             status, 0,0,0,0,0);
       }
   }

   /* Close down the device cleanly */
   if (status == ZL303XX_OK)
   {
      zl303xx_CloseDeviceS closeDevice;
      if ((status = zl303xx_CloseDeviceStructInit(pClock->zl303xx_Params, &closeDevice)) != ZL303XX_OK)
      {
         ZL303XX_TRACE_ALWAYS("example77xClockRemove: zl303xx_CloseDeviceStructInit() failed with status = %lu",
               status, 0,0,0,0,0);
      }

      else if ((status = zl303xx_CloseDevice(pClock->zl303xx_Params, &closeDevice)) != ZL303XX_OK)
      {
         ZL303XX_TRACE_ALWAYS("example77xClockRemove: zl303xx_CloseDevice() failed with status = %lu",
               status, 0,0,0,0,0);
      }
   }

   /* Free up the memory allocated for the device instance */
   if(status == ZL303XX_OK)
   {
      if((status = zl303xx_FreeDeviceInstance(&pClock->zl303xx_Params)) != ZL303XX_OK)
      {
         ZL303XX_TRACE_ALWAYS("example77xClockRemove: zl303xx_FreeDeviceInstance() failed with status = %lu",
                  status, 0,0,0,0,0);
      }
   }

   return status;
}


typedef enum {
    zl303xx_WOV_write,
    zl303xx_WOV_verify
} zl303xxexample77xWriteOrVerifyE;


typedef enum {
    zl303xxfile,
    zl303xxdataStructure
} zl303xxexample77xConfigDataInputTypeS;

typedef struct {
    zl303xxexample77xConfigDataInputTypeS type;
    int fileDesc;
    example77xStructConfigData_t *structPtr;
    int lineIdx;
} zl303xxexample77xConfigDataSourceS;


#define ZLS3077X_SEPARATORS ", \t\n\r"
#define ZLS3077X_LINE_BUFF_LEN 132


/* readLineFromSource */
/*
    Returns the next line from the given data source. The data source could
    be a file or a data structure.

  Parameters:
   [in]   zl303xx_Params   Pointer to the device instance parameter structure.
   [in]  dS.type         The type of input - either file or data structure
   [in]  dS.fileDesc     If file, the this is the file descriptor
   [in]  dS.structPtr    If data structure, then this is a pointer to the
                            data structure
   [in]  dS.lineIdx      If data structure, then this is the line number
                            in dS.structPtr that should be read next

   [out] lineBuffer      The read line

  Return Value:     ZL303XX_DATA_CORRUPTION if we reach the end of file or an error
            occurs

*******************************************************************************/
static int readLineFromSource
            (
            zl303xxexample77xConfigDataSourceS *dS,
            char *lineBuffer
            )
{
    zlStatusE status = ZL303XX_OK;
    int fBytes = 0;
    zl303xx_BooleanE endOfLine = ZL303XX_FALSE;


    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(dS);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(lineBuffer);
    }

    if (status == ZL303XX_OK)
    {
        switch( dS->type )
        {
            case zl303xxfile:
            {
                char *b = lineBuffer;

                do
                {
                    fBytes += read(dS->fileDesc, (void *)b, 1);
                    if( (*b == '\n') || (*b == '\r') || (*b == '\0') )
                    {
                        if( fBytes != 1 )
                        {
                            /* Terminate the string */
                            *b = '\0';
                            endOfLine = ZL303XX_TRUE;
                        }
                    }
                    else
                    {
                        b++;
                    }
                } while ( (endOfLine == ZL303XX_FALSE) &&
                          (status == ZL303XX_OK) );
                break;
            }
            case zl303xxdataStructure:
            {
                fBytes = strlen( dS->structPtr->lines[dS->lineIdx].line );
                strncpy(lineBuffer, dS->structPtr->lines[dS->lineIdx].line, fBytes+1);
                dS->lineIdx++;
                break;
            }
            default:
            {
                status = ZL303XX_DATA_CORRUPTION;
                break;
            }
        }
    }

    return fBytes;
}


#define ZL303XX_MAX_DO_NOT_CHECK_REGISTERS (9)
Uint32T example77xDoNotCheckRegisters[ZL303XX_MAX_DO_NOT_CHECK_REGISTERS] = {
    0x1,
    0x2,
    0x3,
    0x5,
    0x6,
    0x40,
    0x41,
    0x480,
    0x4B0
};

#define ZL303XX_MAX_MAILBOX_SELECTION_REGISTERS (6)
Uint32T example77xMailboxSelectionRegisters[ZL303XX_MAX_MAILBOX_SELECTION_REGISTERS] = {
    /* ref mailbox */
    0x582,
    0x583,
    0x584,
    /* dpll mailbox */
    0x602,
    0x603,
    0x604,
};

/* example77xLoadConfig */
/*
    Load the ZL30770 device configuration registers from the given file.

  Parameters:
   [in]   zl303xx_Params   Pointer to the device instance parameter structure.
   [in]   dS             The data source
   [in]   wov            'write' or 'verify' indicator

  Return Value:  The result of the calculation

*******************************************************************************/
static zlStatusE example77xLoadConfig
            (
            zl303xx_ParamsS *zl303xx_Params,
            zl303xxexample77xConfigDataSourceS *dS,
            zl303xxexample77xWriteOrVerifyE wov
            )
{
    zlStatusE status = ZL303XX_OK;
    Uint32T idx;
    int fBytes = 0;
    char *cmd;
    Uint16T regAddr = 0;
    Uint8T regValue = 0;
    Uint32T readRegValue;
    Uint32T waitUsec = 0;
    char *dummy;
    zl303xx_BooleanE endOfFile = ZL303XX_FALSE;
    zl303xx_BooleanE mbsr = ZL303XX_FALSE;
    zl303xx_BooleanE readOnlyReg = ZL303XX_FALSE;
    char lineBuffer[ZLS3077X_LINE_BUFF_LEN];
    char lineBufferCopy[ZLS3077X_LINE_BUFF_LEN];


    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(zl303xx_Params);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(dS);
    }

    if (status == ZL303XX_OK)
    {
        char *strContext;

        do
        {
            memset(lineBuffer, 0, sizeof(lineBuffer));
            memset(lineBufferCopy, 0, sizeof(lineBufferCopy));

            fBytes = readLineFromSource(dS, lineBuffer);

            if( fBytes < 1 )
            {
                /* We reached the end of the config file but did not detect
                   the termination string (the next check). */
                ZL303XX_TRACE_ALWAYS("example77xLoadConfig: Unusual termination of config file: fBytes: %d",
                          fBytes, 0,0,0,0,0);
                endOfFile = ZL303XX_TRUE;
            }
            else if( strstr(lineBuffer, "; Register Configuration End") != NULL )
            {
                /* End of config file */
                endOfFile = ZL303XX_TRUE;
            }
            else
            {
                /* Copy the line so we can search it easier later */
                memset( lineBufferCopy, 0x00, ZLS3077X_LINE_BUFF_LEN );
                (void)strncpy( lineBufferCopy, lineBuffer, ZLS3077X_LINE_BUFF_LEN-1 );

                /* Get command: ';', 'X', or 'W'  */
                cmd = strtok_r(lineBuffer, ZLS3077X_SEPARATORS, &strContext );

                if ( cmd == NULL )
                {
                    /* Blank line - disacrd */
                }
                else if (cmd[0] == ';')
                {
                    /* Comment line - discard */
                }
                else if ( cmd[0] == 'X' )
                {
                    /* convert our hex string to an int */
                    regAddr = strtol( strtok_r(NULL, ZLS3077X_SEPARATORS, &strContext ), &dummy, 16);
                    regValue = strtol( strtok_r(NULL, ZLS3077X_SEPARATORS, &strContext ), &dummy, 16);

                    if( wov == zl303xx_WOV_write )
                    {
                        status = zl303xx_Write(zl303xx_Params, NULL,
                                          ZL303XX_MAKE_MEM_ADDR_77X(regAddr, ZL303XX_MEM_SIZE_1_BYTE),
                                          regValue);
                    }
                    if( wov == zl303xx_WOV_verify )
                    {
                        /* If we are reading from a mailbox, then we need to populate
                           the mailbox before we can read it. To populate the mailbox
                           we must write some registers.

                           BUT: we should not write the xxx_semaphore registers to 1
                                since that would store the entire mailbox again.
                        */
                        mbsr = ZL303XX_FALSE;
                        for( idx = 0; idx < ZL303XX_MAX_MAILBOX_SELECTION_REGISTERS; idx++)
                        {
                            if( regAddr == example77xMailboxSelectionRegisters[idx] )
                            {
                                /* We may need to write this register */
                                mbsr = ZL303XX_TRUE;
                            }
                        }
                        if( mbsr == ZL303XX_TRUE )
                        {
                            if( ( ( regAddr == 0x584 ) || ( regAddr == 0x604 ) ) &&
                                ( regValue == 1 ) )
                            {
                                /* Do not set the mailbox wr bit (write bit) to 1 */
                            }
                            else
                            {
                                /* This is either the mb_mask or the mb_sem rd bit
                                   (read bit). Write them to populate the mailbox. */
                                status = zl303xx_Write(zl303xx_Params, NULL,
                                              ZL303XX_MAKE_MEM_ADDR_77X(regAddr, ZL303XX_MEM_SIZE_1_BYTE),
                                              regValue);
                            }
                        }
                        else
                        {
                            /* Check if this is a read-only register */
                            readOnlyReg = ZL303XX_FALSE;
                            for( idx = 0; idx < ZL303XX_MAX_DO_NOT_CHECK_REGISTERS; idx++)
                            {
                                if( regAddr == example77xDoNotCheckRegisters[idx] )
                                {
                                    readOnlyReg = ZL303XX_TRUE;
                                }
                            }
                            if( readOnlyReg == ZL303XX_FALSE )
                            {
                                status = zl303xx_Read(zl303xx_Params, NULL,
                                              ZL303XX_MAKE_MEM_ADDR_77X(regAddr, ZL303XX_MEM_SIZE_1_BYTE),
                                              &readRegValue);

                                if( status == ZL303XX_OK )
                                {
                                    if( regValue != readRegValue )
                                    {
                                        printf( "programming verfiy failed: register: %x  expected: %x  read : %x\n",
                                                 regAddr, regValue, readRegValue );
                                    }
                                }
                            }
                        }
                    }
                }

                /* Parse wait commands */
                else if (cmd[0] == 'W')
                {
                    /* Only wait if we are currently processing register data. */
                    waitUsec = atoi( strtok_r(NULL, ZLS3077X_SEPARATORS, &strContext ));

                    if ( waitUsec > 2000000 )
                    {
                        /* Sanity check - do not wait more than 2s */
                        waitUsec = 2000000;
                    }
                    if( waitUsec < 1000 )
                    {
                        /* Force a delay of at least 1ms */
                        waitUsec = 1000;
                    }
                    OS_TASK_DELAY(waitUsec / 1000);
                }
            }
        } while ((status == ZL303XX_OK) && (endOfFile == ZL303XX_FALSE));
    }

#if defined ZLE30360_770_FRANKEN_BOARD
    /* Flash LED0 on the Redwood board to indicate life. */
    (void)zl303xx_Write(zl303xx_Params, NULL, ZL303XX_MAKE_MEM_ADDR_77X(0x88, ZL303XX_MEM_SIZE_1_BYTE), 0x70);
    (void)zl303xx_Write(zl303xx_Params, NULL, ZL303XX_MAKE_MEM_ADDR_77X(0x89, ZL303XX_MEM_SIZE_1_BYTE), 0x41);
    (void)zl303xx_Write(zl303xx_Params, NULL, ZL303XX_MAKE_MEM_ADDR_77X(0x8A, ZL303XX_MEM_SIZE_1_BYTE), 0x03);
#endif

    return status;
}

/* example77xLoadConfigFile */
/**
   Parses and loads the ZLS3077X using a config file generated by the 77x chip GUI.

  Parameters:
   [in]   zl303xx_Params   Pointer to the device instance parameter structure.
   [in]   filename       Pointer to string containing filename to read config from.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE example77xLoadConfigFile
            (
            zl303xx_ParamsS *zl303xx_Params,
            const char *filename
            )
{
    zlStatusE status = ZL303XX_OK;
    zl303xxexample77xConfigDataSourceS dS;
    int fInput = 0;
    zl303xxexample77xWriteOrVerifyE wov;

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(zl303xx_Params);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(filename);
    }

    if (status == ZL303XX_OK)
    {
        for( wov = zl303xx_WOV_write; wov <= zl303xx_WOV_verify; wov++ )
        {
            printf("%s: ZLS3077x config from CONFIG FILE=\"%s\"\n",
                        (wov == zl303xx_WOV_write) ? "Writing" : "Verifying", filename);

            #if defined OS_VXWORKS
            if ((fInput = open(filename, O_RDONLY, 0666)) == ERROR)
            #else
            if ((fInput = open(filename, O_RDONLY)) == -1)
            #endif
            {
                status = ZL303XX_IO_ERROR;
                printf("example77xLoadConfigFile: Error opening CONFIG FILE=\"%s\" status=%d\n", filename, status);
            }

            if (status == ZL303XX_OK)
            {
                dS.type = zl303xxfile;
                dS.fileDesc = fInput;

                status = example77xLoadConfig(zl303xx_Params, &dS, wov);
            }

            #if defined OS_VXWORKS
            if( fInput != ERROR )
            #else
            if( fInput >= 0 )
            #endif
            {
                close(fInput);

                #if defined OS_VXWORKS
                fInput = ERROR;
                #else
                fInput = -1;
                #endif
            }
        }
    }

    return status;
}



/* example77xLoadConfigStruct */
/*
   Parses and loads the ZLS3077X using a given structure.

  Parameters:
   [in]   zl303xx_Params   Pointer to the device instance parameter structure.
   [in]   cData          Pointer to the configuration in memory.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE example77xLoadConfigStruct
            (
            zl303xx_ParamsS *zl303xx_Params,
            example77xStructConfigData_t *cData
            )
{
    zlStatusE status = ZL303XX_OK;
    zl303xxexample77xConfigDataSourceS dS;


    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(zl303xx_Params);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(cData);
    }
    if (status == ZL303XX_OK)
    {
        dS.type = zl303xxdataStructure;
        dS.structPtr = cData;
        dS.lineIdx = 0;
        printf("Writing: ZLS3077x config from data structure at 0x%p\n", cData);

        status = example77xLoadConfig(zl303xx_Params, &dS, zl303xx_WOV_write);
    }
    if (status == ZL303XX_OK)
    {
        dS.type = zl303xxdataStructure;
        dS.structPtr = cData;
        dS.lineIdx = 0;
        printf("Verifying: ZLS3077x config from data structure at 0x%p\n", cData);
        status = example77xLoadConfig(zl303xx_Params, &dS, zl303xx_WOV_verify);
    }

    return status;
}


example77xStructConfigData_t example77xConfigData = {
    {
        {"; Device Id            : ZL30773"},
        {"; GUI Version          : 1.1"},
        {"; File Generation Date : September-27-17 11:41:55 AM"},
        {";======================================================================"},

        {"; NOTE:"},
        {"; This is an incremental configuration script."},
        {"; For proper device operation, all register write and wait commands in"},
        {"; this file must be performed in the sequence listed."},

        {";======================================================================"},

        {"; Configuration script commands"},

        {"; 1.  Register Write Command:"},
        {";        X , <register_address> , <data_bytes>"},
        {";        Both <register_address> and <data_bytes> are in hexadecimal"},
        {";        format and must have the 0x prefix."},

        {"; 2.  Wait Command:"},
        {";        W , <time_microseconds>"},
        {";        The wait time is specified in microseconds."},

        {";======================================================================"},

        {"; The following lines are used only for the evaluation board GUI configuration:"},

        {"; Master Clock Nominal Freq MHz = 125"},

        {";======================================================================"},
        {"; Register Configuration Start"},

        {"X , 0x0480  , 0x10                        ;  hp_ctrl_1                   "},
        {"X , 0x04B0  , 0x20                        ;  hp_ctrl_2                   "},

        {"X , 0x0582  , 0x00                        ;  ref_mb_mask                 "},
        {"X , 0x0583  , 0x01                        ;  ref_mb_mask                 "},
        {"X , 0x0584  , 0x02                        ;  ref_mb_sem                  "},
        {"W , 20000 "},
        {"X , 0x0585  , 0x1F                        ;  ref0_freq_base              "},
        {"X , 0x0586  , 0x40                        ;  ref0_freq_base              "},
        {"X , 0x0587  , 0x00                        ;  ref0_freq_mult              "},
        {"X , 0x0588  , 0x01                        ;  ref0_freq_mult              "},
        {"X , 0x0589  , 0x00                        ;  ref0_ratio_m                "},
        {"X , 0x058A  , 0x01                        ;  ref0_ratio_m                "},
        {"X , 0x058B  , 0x00                        ;  ref0_ratio_n                "},
        {"X , 0x058C  , 0x01                        ;  ref0_ratio_n                "},
        {"X , 0x058D  , 0x01                        ;  ref0_config                 "},
        {"X , 0x0590  , 0x05                        ;  ref0_scm                    "},
        {"X , 0x0591  , 0x05                        ;  ref0_cfm                    "},
        {"X , 0x0592  , 0x00                        ;  ref0_gst_disqual            "},
        {"X , 0x0593  , 0x05                        ;  ref0_gst_disqual            "},
        {"X , 0x0594  , 0x00                        ;  ref0_gst_qual               "},
        {"X , 0x0595  , 0x14                        ;  ref0_gst_qual               "},
        {"X , 0x0597  , 0x00                        ;  ref0_pfm_ctrl               "},
        {"X , 0x0598  , 0x65                        ;  ref0_pfm_disqualify         "},
        {"X , 0x0599  , 0x68                        ;  ref0_pfm_disqualify         "},
        {"X , 0x059A  , 0x4E                        ;  ref0_pfm_qualify            "},
        {"X , 0x059B  , 0x48                        ;  ref0_pfm_qualify            "},
        {"X , 0x059C  , 0x00                        ;  ref0_pfm_period             "},
        {"X , 0x059D  , 0x00                        ;  ref0_pfm_period             "},
        {"X , 0x059E  , 0x28                        ;  ref0_pfm_filter_limit       "},
        {"X , 0x05A0  , 0x00                        ;  ref0_sync                   "},
        {"X , 0x05A1  , 0x00                        ;  ref0_sync_misc              "},
        {"X , 0x05A2  , 0x00                        ;  ref0_sync_offset_comp       "},
        {"X , 0x05A3  , 0x00                        ;  ref0_phase_offset_compensation"},
        {"X , 0x05A4  , 0x00                        ;  ref0_phase_offset_compensation"},
        {"X , 0x05A5  , 0x00                        ;  ref0_phase_offset_compensation"},
        {"X , 0x05A6  , 0x00                        ;  ref0_phase_offset_compensation"},
        {"X , 0x05A8  , 0x00                        ;  ref0_scm_fine               "},
        {"X , 0x05A9  , 0x00                        ;  ref0_scm_fine               "},
        {"X , 0x05AA  , 0x00                        ;  ref0_scm_fine               "},
        {"X , 0x05AB  , 0x00                        ;  ref0_scm_fine               "},
        {"X , 0x0584  , 0x01                        ;  ref_mb_sem                  "},
        {"W , 20000 "},

        {"X , 0x0582  , 0x00                        ;  ref_mb_mask                 "},
        {"X , 0x0583  , 0x02                        ;  ref_mb_mask                 "},
        {"X , 0x0584  , 0x02                        ;  ref_mb_sem                  "},
        {"W , 20000 "},
        {"X , 0x0585  , 0x1F                        ;  ref1_freq_base              "},
        {"X , 0x0586  , 0x40                        ;  ref1_freq_base              "},
        {"X , 0x0587  , 0x00                        ;  ref1_freq_mult              "},
        {"X , 0x0588  , 0x01                        ;  ref1_freq_mult              "},
        {"X , 0x0589  , 0x00                        ;  ref1_ratio_m                "},
        {"X , 0x058A  , 0x01                        ;  ref1_ratio_m                "},
        {"X , 0x058B  , 0x00                        ;  ref1_ratio_n                "},
        {"X , 0x058C  , 0x01                        ;  ref1_ratio_n                "},
        {"X , 0x058D  , 0x01                        ;  ref1_config                 "},
        {"X , 0x0590  , 0x05                        ;  ref1_scm                    "},
        {"X , 0x0591  , 0x05                        ;  ref1_cfm                    "},
        {"X , 0x0592  , 0x00                        ;  ref1_gst_disqual            "},
        {"X , 0x0593  , 0x05                        ;  ref1_gst_disqual            "},
        {"X , 0x0594  , 0x00                        ;  ref1_gst_qual               "},
        {"X , 0x0595  , 0x14                        ;  ref1_gst_qual               "},
        {"X , 0x0597  , 0x00                        ;  ref1_pfm_ctrl               "},
        {"X , 0x0598  , 0x65                        ;  ref1_pfm_disqualify         "},
        {"X , 0x0599  , 0x68                        ;  ref1_pfm_disqualify         "},
        {"X , 0x059A  , 0x4E                        ;  ref1_pfm_qualify            "},
        {"X , 0x059B  , 0x48                        ;  ref1_pfm_qualify            "},
        {"X , 0x059C  , 0x00                        ;  ref1_pfm_period             "},
        {"X , 0x059D  , 0x00                        ;  ref1_pfm_period             "},
        {"X , 0x059E  , 0x28                        ;  ref1_pfm_filter_limit       "},
        {"X , 0x05A0  , 0x00                        ;  ref1_sync                   "},
        {"X , 0x05A1  , 0x00                        ;  ref1_sync_misc              "},
        {"X , 0x05A2  , 0x00                        ;  ref1_sync_offset_comp       "},
        {"X , 0x05A3  , 0x00                        ;  ref1_phase_offset_compensation"},
        {"X , 0x05A4  , 0x00                        ;  ref1_phase_offset_compensation"},
        {"X , 0x05A5  , 0x00                        ;  ref1_phase_offset_compensation"},
        {"X , 0x05A6  , 0x00                        ;  ref1_phase_offset_compensation"},
        {"X , 0x05A8  , 0x00                        ;  ref1_scm_fine               "},
        {"X , 0x05A9  , 0x00                        ;  ref1_scm_fine               "},
        {"X , 0x05AA  , 0x00                        ;  ref1_scm_fine               "},
        {"X , 0x05AB  , 0x00                        ;  ref1_scm_fine               "},
        {"X , 0x0584  , 0x01                        ;  ref_mb_sem                  "},
        {"W , 20000 "},

        {"X , 0x0582  , 0x00                        ;  ref_mb_mask                 "},
        {"X , 0x0583  , 0x04                        ;  ref_mb_mask                 "},
        {"X , 0x0584  , 0x02                        ;  ref_mb_sem                  "},
        {"W , 20000 "},
        {"X , 0x0585  , 0x00                        ;  ref2_freq_base              "},
        {"X , 0x0586  , 0x01                        ;  ref2_freq_base              "},
        {"X , 0x0587  , 0x00                        ;  ref2_freq_mult              "},
        {"X , 0x0588  , 0x01                        ;  ref2_freq_mult              "},
        {"X , 0x0589  , 0x00                        ;  ref2_ratio_m                "},
        {"X , 0x058A  , 0x01                        ;  ref2_ratio_m                "},
        {"X , 0x058B  , 0x00                        ;  ref2_ratio_n                "},
        {"X , 0x058C  , 0x01                        ;  ref2_ratio_n                "},
        {"X , 0x058D  , 0x01                        ;  ref2_config                 "},
        {"X , 0x0590  , 0x05                        ;  ref2_scm                    "},
        {"X , 0x0591  , 0x05                        ;  ref2_cfm                    "},
        {"X , 0x0592  , 0x00                        ;  ref2_gst_disqual            "},
        {"X , 0x0593  , 0x05                        ;  ref2_gst_disqual            "},
        {"X , 0x0594  , 0x00                        ;  ref2_gst_qual               "},
        {"X , 0x0595  , 0x14                        ;  ref2_gst_qual               "},
        {"X , 0x0597  , 0x00                        ;  ref2_pfm_ctrl               "},
        {"X , 0x0598  , 0x65                        ;  ref2_pfm_disqualify         "},
        {"X , 0x0599  , 0x68                        ;  ref2_pfm_disqualify         "},
        {"X , 0x059A  , 0x4E                        ;  ref2_pfm_qualify            "},
        {"X , 0x059B  , 0x48                        ;  ref2_pfm_qualify            "},
        {"X , 0x059C  , 0x00                        ;  ref2_pfm_period             "},
        {"X , 0x059D  , 0x00                        ;  ref2_pfm_period             "},
        {"X , 0x059E  , 0x28                        ;  ref2_pfm_filter_limit       "},
        {"X , 0x05A0  , 0x00                        ;  ref2_sync                   "},
        {"X , 0x05A1  , 0x00                        ;  ref2_sync_misc              "},
        {"X , 0x05A2  , 0x00                        ;  ref2_sync_offset_comp       "},
        {"X , 0x05A3  , 0x00                        ;  ref2_phase_offset_compensation"},
        {"X , 0x05A4  , 0x00                        ;  ref2_phase_offset_compensation"},
        {"X , 0x05A5  , 0x00                        ;  ref2_phase_offset_compensation"},
        {"X , 0x05A6  , 0x00                        ;  ref2_phase_offset_compensation"},
        {"X , 0x05A8  , 0x00                        ;  ref2_scm_fine               "},
        {"X , 0x05A9  , 0x00                        ;  ref2_scm_fine               "},
        {"X , 0x05AA  , 0x00                        ;  ref2_scm_fine               "},
        {"X , 0x05AB  , 0x00                        ;  ref2_scm_fine               "},
        {"X , 0x0584  , 0x01                        ;  ref_mb_sem                  "},
        {"W , 20000 "},

        {"X , 0x0582  , 0x00                        ;  ref_mb_mask                 "},
        {"X , 0x0583  , 0x08                        ;  ref_mb_mask                 "},
        {"X , 0x0584  , 0x02                        ;  ref_mb_sem                  "},
        {"W , 20000 "},
        {"X , 0x0585  , 0x1F                        ;  ref3_freq_base              "},
        {"X , 0x0586  , 0x40                        ;  ref3_freq_base              "},
        {"X , 0x0587  , 0x00                        ;  ref3_freq_mult              "},
        {"X , 0x0588  , 0x01                        ;  ref3_freq_mult              "},
        {"X , 0x0589  , 0x00                        ;  ref3_ratio_m                "},
        {"X , 0x058A  , 0x01                        ;  ref3_ratio_m                "},
        {"X , 0x058B  , 0x00                        ;  ref3_ratio_n                "},
        {"X , 0x058C  , 0x01                        ;  ref3_ratio_n                "},
        {"X , 0x058D  , 0x01                        ;  ref3_config                 "},
        {"X , 0x0590  , 0x05                        ;  ref3_scm                    "},
        {"X , 0x0591  , 0x05                        ;  ref3_cfm                    "},
        {"X , 0x0592  , 0x00                        ;  ref3_gst_disqual            "},
        {"X , 0x0593  , 0x05                        ;  ref3_gst_disqual            "},
        {"X , 0x0594  , 0x00                        ;  ref3_gst_qual               "},
        {"X , 0x0595  , 0x14                        ;  ref3_gst_qual               "},
        {"X , 0x0597  , 0x00                        ;  ref3_pfm_ctrl               "},
        {"X , 0x0598  , 0x65                        ;  ref3_pfm_disqualify         "},
        {"X , 0x0599  , 0x68                        ;  ref3_pfm_disqualify         "},
        {"X , 0x059A  , 0x4E                        ;  ref3_pfm_qualify            "},
        {"X , 0x059B  , 0x48                        ;  ref3_pfm_qualify            "},
        {"X , 0x059C  , 0x00                        ;  ref3_pfm_period             "},
        {"X , 0x059D  , 0x00                        ;  ref3_pfm_period             "},
        {"X , 0x059E  , 0x28                        ;  ref3_pfm_filter_limit       "},
        {"X , 0x05A0  , 0x00                        ;  ref3_sync                   "},
        {"X , 0x05A1  , 0x00                        ;  ref3_sync_misc              "},
        {"X , 0x05A2  , 0x00                        ;  ref3_sync_offset_comp       "},
        {"X , 0x05A3  , 0x00                        ;  ref3_phase_offset_compensation"},
        {"X , 0x05A4  , 0x00                        ;  ref3_phase_offset_compensation"},
        {"X , 0x05A5  , 0x00                        ;  ref3_phase_offset_compensation"},
        {"X , 0x05A6  , 0x00                        ;  ref3_phase_offset_compensation"},
        {"X , 0x05A8  , 0x00                        ;  ref3_scm_fine               "},
        {"X , 0x05A9  , 0x00                        ;  ref3_scm_fine               "},
        {"X , 0x05AA  , 0x00                        ;  ref3_scm_fine               "},
        {"X , 0x05AB  , 0x00                        ;  ref3_scm_fine               "},
        {"X , 0x0584  , 0x01                        ;  ref_mb_sem                  "},
        {"W , 20000 "},

        {"X , 0x0582  , 0x00                        ;  ref_mb_mask                 "},
        {"X , 0x0583  , 0x10                        ;  ref_mb_mask                 "},
        {"X , 0x0584  , 0x02                        ;  ref_mb_sem                  "},
        {"W , 20000 "},
        {"X , 0x0585  , 0x1F                        ;  ref4_freq_base              "},
        {"X , 0x0586  , 0x40                        ;  ref4_freq_base              "},
        {"X , 0x0587  , 0x00                        ;  ref4_freq_mult              "},
        {"X , 0x0588  , 0x01                        ;  ref4_freq_mult              "},
        {"X , 0x0589  , 0x00                        ;  ref4_ratio_m                "},
        {"X , 0x058A  , 0x01                        ;  ref4_ratio_m                "},
        {"X , 0x058B  , 0x00                        ;  ref4_ratio_n                "},
        {"X , 0x058C  , 0x01                        ;  ref4_ratio_n                "},
        {"X , 0x058D  , 0x01                        ;  ref4_config                 "},
        {"X , 0x0590  , 0x05                        ;  ref4_scm                    "},
        {"X , 0x0591  , 0x05                        ;  ref4_cfm                    "},
        {"X , 0x0592  , 0x00                        ;  ref4_gst_disqual            "},
        {"X , 0x0593  , 0x05                        ;  ref4_gst_disqual            "},
        {"X , 0x0594  , 0x00                        ;  ref4_gst_qual               "},
        {"X , 0x0595  , 0x14                        ;  ref4_gst_qual               "},
        {"X , 0x0597  , 0x00                        ;  ref4_pfm_ctrl               "},
        {"X , 0x0598  , 0x65                        ;  ref4_pfm_disqualify         "},
        {"X , 0x0599  , 0x68                        ;  ref4_pfm_disqualify         "},
        {"X , 0x059A  , 0x4E                        ;  ref4_pfm_qualify            "},
        {"X , 0x059B  , 0x48                        ;  ref4_pfm_qualify            "},
        {"X , 0x059C  , 0x00                        ;  ref4_pfm_period             "},
        {"X , 0x059D  , 0x00                        ;  ref4_pfm_period             "},
        {"X , 0x059E  , 0x28                        ;  ref4_pfm_filter_limit       "},
        {"X , 0x05A0  , 0x00                        ;  ref4_sync                   "},
        {"X , 0x05A1  , 0x00                        ;  ref4_sync_misc              "},
        {"X , 0x05A2  , 0x00                        ;  ref4_sync_offset_comp       "},
        {"X , 0x05A3  , 0x00                        ;  ref4_phase_offset_compensation"},
        {"X , 0x05A4  , 0x00                        ;  ref4_phase_offset_compensation"},
        {"X , 0x05A5  , 0x00                        ;  ref4_phase_offset_compensation"},
        {"X , 0x05A6  , 0x00                        ;  ref4_phase_offset_compensation"},
        {"X , 0x05A8  , 0x00                        ;  ref4_scm_fine               "},
        {"X , 0x05A9  , 0x00                        ;  ref4_scm_fine               "},
        {"X , 0x05AA  , 0x00                        ;  ref4_scm_fine               "},
        {"X , 0x05AB  , 0x00                        ;  ref4_scm_fine               "},
        {"X , 0x0584  , 0x01                        ;  ref_mb_sem                  "},
        {"W , 20000 "},

        {"X , 0x0582  , 0x00                        ;  ref_mb_mask                 "},
        {"X , 0x0583  , 0x20                        ;  ref_mb_mask                 "},
        {"X , 0x0584  , 0x02                        ;  ref_mb_sem                  "},
        {"W , 20000 "},
        {"X , 0x0585  , 0x1F                        ;  ref5_freq_base              "},
        {"X , 0x0586  , 0x40                        ;  ref5_freq_base              "},
        {"X , 0x0587  , 0x00                        ;  ref5_freq_mult              "},
        {"X , 0x0588  , 0x01                        ;  ref5_freq_mult              "},
        {"X , 0x0589  , 0x00                        ;  ref5_ratio_m                "},
        {"X , 0x058A  , 0x01                        ;  ref5_ratio_m                "},
        {"X , 0x058B  , 0x00                        ;  ref5_ratio_n                "},
        {"X , 0x058C  , 0x01                        ;  ref5_ratio_n                "},
        {"X , 0x058D  , 0x01                        ;  ref5_config                 "},
        {"X , 0x0590  , 0x05                        ;  ref5_scm                    "},
        {"X , 0x0591  , 0x05                        ;  ref5_cfm                    "},
        {"X , 0x0592  , 0x00                        ;  ref5_gst_disqual            "},
        {"X , 0x0593  , 0x05                        ;  ref5_gst_disqual            "},
        {"X , 0x0594  , 0x00                        ;  ref5_gst_qual               "},
        {"X , 0x0595  , 0x14                        ;  ref5_gst_qual               "},
        {"X , 0x0597  , 0x00                        ;  ref5_pfm_ctrl               "},
        {"X , 0x0598  , 0x65                        ;  ref5_pfm_disqualify         "},
        {"X , 0x0599  , 0x68                        ;  ref5_pfm_disqualify         "},
        {"X , 0x059A  , 0x4E                        ;  ref5_pfm_qualify            "},
        {"X , 0x059B  , 0x48                        ;  ref5_pfm_qualify            "},
        {"X , 0x059C  , 0x00                        ;  ref5_pfm_period             "},
        {"X , 0x059D  , 0x00                        ;  ref5_pfm_period             "},
        {"X , 0x059E  , 0x28                        ;  ref5_pfm_filter_limit       "},
        {"X , 0x05A0  , 0x00                        ;  ref5_sync                   "},
        {"X , 0x05A1  , 0x00                        ;  ref5_sync_misc              "},
        {"X , 0x05A2  , 0x00                        ;  ref5_sync_offset_comp       "},
        {"X , 0x05A3  , 0x00                        ;  ref5_phase_offset_compensation"},
        {"X , 0x05A4  , 0x00                        ;  ref5_phase_offset_compensation"},
        {"X , 0x05A5  , 0x00                        ;  ref5_phase_offset_compensation"},
        {"X , 0x05A6  , 0x00                        ;  ref5_phase_offset_compensation"},
        {"X , 0x05A8  , 0x00                        ;  ref5_scm_fine               "},
        {"X , 0x05A9  , 0x00                        ;  ref5_scm_fine               "},
        {"X , 0x05AA  , 0x00                        ;  ref5_scm_fine               "},
        {"X , 0x05AB  , 0x00                        ;  ref5_scm_fine               "},
        {"X , 0x0584  , 0x01                        ;  ref_mb_sem                  "},
        {"W , 20000 "},

        {"X , 0x0582  , 0x00                        ;  ref_mb_mask                 "},
        {"X , 0x0583  , 0x40                        ;  ref_mb_mask                 "},
        {"X , 0x0584  , 0x02                        ;  ref_mb_sem                  "},
        {"W , 20000 "},
        {"X , 0x0585  , 0x1F                        ;  ref6_freq_base              "},
        {"X , 0x0586  , 0x40                        ;  ref6_freq_base              "},
        {"X , 0x0587  , 0x00                        ;  ref6_freq_mult              "},
        {"X , 0x0588  , 0x01                        ;  ref6_freq_mult              "},
        {"X , 0x0589  , 0x00                        ;  ref6_ratio_m                "},
        {"X , 0x058A  , 0x01                        ;  ref6_ratio_m                "},
        {"X , 0x058B  , 0x00                        ;  ref6_ratio_n                "},
        {"X , 0x058C  , 0x01                        ;  ref6_ratio_n                "},
        {"X , 0x058D  , 0x01                        ;  ref6_config                 "},
        {"X , 0x0590  , 0x05                        ;  ref6_scm                    "},
        {"X , 0x0591  , 0x05                        ;  ref6_cfm                    "},
        {"X , 0x0592  , 0x00                        ;  ref6_gst_disqual            "},
        {"X , 0x0593  , 0x05                        ;  ref6_gst_disqual            "},
        {"X , 0x0594  , 0x00                        ;  ref6_gst_qual               "},
        {"X , 0x0595  , 0x14                        ;  ref6_gst_qual               "},
        {"X , 0x0597  , 0x00                        ;  ref6_pfm_ctrl               "},
        {"X , 0x0598  , 0x65                        ;  ref6_pfm_disqualify         "},
        {"X , 0x0599  , 0x68                        ;  ref6_pfm_disqualify         "},
        {"X , 0x059A  , 0x4E                        ;  ref6_pfm_qualify            "},
        {"X , 0x059B  , 0x48                        ;  ref6_pfm_qualify            "},
        {"X , 0x059C  , 0x00                        ;  ref6_pfm_period             "},
        {"X , 0x059D  , 0x00                        ;  ref6_pfm_period             "},
        {"X , 0x059E  , 0x28                        ;  ref6_pfm_filter_limit       "},
        {"X , 0x05A0  , 0x00                        ;  ref6_sync                   "},
        {"X , 0x05A1  , 0x00                        ;  ref6_sync_misc              "},
        {"X , 0x05A2  , 0x00                        ;  ref6_sync_offset_comp       "},
        {"X , 0x05A3  , 0x00                        ;  ref6_phase_offset_compensation"},
        {"X , 0x05A4  , 0x00                        ;  ref6_phase_offset_compensation"},
        {"X , 0x05A5  , 0x00                        ;  ref6_phase_offset_compensation"},
        {"X , 0x05A6  , 0x00                        ;  ref6_phase_offset_compensation"},
        {"X , 0x05A8  , 0x00                        ;  ref6_scm_fine               "},
        {"X , 0x05A9  , 0x00                        ;  ref6_scm_fine               "},
        {"X , 0x05AA  , 0x00                        ;  ref6_scm_fine               "},
        {"X , 0x05AB  , 0x00                        ;  ref6_scm_fine               "},
        {"X , 0x0584  , 0x01                        ;  ref_mb_sem                  "},
        {"W , 20000 "},

        {"X , 0x0582  , 0x00                        ;  ref_mb_mask                 "},
        {"X , 0x0583  , 0x80                        ;  ref_mb_mask                 "},
        {"X , 0x0584  , 0x02                        ;  ref_mb_sem                  "},
        {"W , 20000 "},
        {"X , 0x0585  , 0x00                        ;  ref7_freq_base              "},
        {"X , 0x0586  , 0x01                        ;  ref7_freq_base              "},
        {"X , 0x0587  , 0x00                        ;  ref7_freq_mult              "},
        {"X , 0x0588  , 0x01                        ;  ref7_freq_mult              "},
        {"X , 0x0589  , 0x00                        ;  ref7_ratio_m                "},
        {"X , 0x058A  , 0x01                        ;  ref7_ratio_m                "},
        {"X , 0x058B  , 0x00                        ;  ref7_ratio_n                "},
        {"X , 0x058C  , 0x01                        ;  ref7_ratio_n                "},
        {"X , 0x058D  , 0x01                        ;  ref7_config                 "},
        {"X , 0x0590  , 0x05                        ;  ref7_scm                    "},
        {"X , 0x0591  , 0x05                        ;  ref7_cfm                    "},
        {"X , 0x0592  , 0x00                        ;  ref7_gst_disqual            "},
        {"X , 0x0593  , 0x05                        ;  ref7_gst_disqual            "},
        {"X , 0x0594  , 0x00                        ;  ref7_gst_qual               "},
        {"X , 0x0595  , 0x14                        ;  ref7_gst_qual               "},
        {"X , 0x0597  , 0x00                        ;  ref7_pfm_ctrl               "},
        {"X , 0x0598  , 0x65                        ;  ref7_pfm_disqualify         "},
        {"X , 0x0599  , 0x68                        ;  ref7_pfm_disqualify         "},
        {"X , 0x059A  , 0x4E                        ;  ref7_pfm_qualify            "},
        {"X , 0x059B  , 0x48                        ;  ref7_pfm_qualify            "},
        {"X , 0x059C  , 0x00                        ;  ref7_pfm_period             "},
        {"X , 0x059D  , 0x00                        ;  ref7_pfm_period             "},
        {"X , 0x059E  , 0x28                        ;  ref7_pfm_filter_limit       "},
        {"X , 0x05A0  , 0x00                        ;  ref7_sync                   "},
        {"X , 0x05A1  , 0x00                        ;  ref7_sync_misc              "},
        {"X , 0x05A2  , 0x00                        ;  ref7_sync_offset_comp       "},
        {"X , 0x05A3  , 0x00                        ;  ref7_phase_offset_compensation"},
        {"X , 0x05A4  , 0x00                        ;  ref7_phase_offset_compensation"},
        {"X , 0x05A5  , 0x00                        ;  ref7_phase_offset_compensation"},
        {"X , 0x05A6  , 0x00                        ;  ref7_phase_offset_compensation"},
        {"X , 0x05A8  , 0x00                        ;  ref7_scm_fine               "},
        {"X , 0x05A9  , 0x00                        ;  ref7_scm_fine               "},
        {"X , 0x05AA  , 0x00                        ;  ref7_scm_fine               "},
        {"X , 0x05AB  , 0x00                        ;  ref7_scm_fine               "},
        {"X , 0x0584  , 0x01                        ;  ref_mb_sem                  "},
        {"W , 20000 "},

        {"X , 0x0582  , 0x01                        ;  ref_mb_mask                 "},
        {"X , 0x0583  , 0x00                        ;  ref_mb_mask                 "},
        {"X , 0x0584  , 0x02                        ;  ref_mb_sem                  "},
        {"W , 20000 "},
        {"X , 0x0585  , 0x1F                        ;  ref8_freq_base              "},
        {"X , 0x0586  , 0x40                        ;  ref8_freq_base              "},
        {"X , 0x0587  , 0x00                        ;  ref8_freq_mult              "},
        {"X , 0x0588  , 0x01                        ;  ref8_freq_mult              "},
        {"X , 0x0589  , 0x00                        ;  ref8_ratio_m                "},
        {"X , 0x058A  , 0x01                        ;  ref8_ratio_m                "},
        {"X , 0x058B  , 0x00                        ;  ref8_ratio_n                "},
        {"X , 0x058C  , 0x01                        ;  ref8_ratio_n                "},
        {"X , 0x058D  , 0x01                        ;  ref8_config                 "},
        {"X , 0x0590  , 0x05                        ;  ref8_scm                    "},
        {"X , 0x0591  , 0x05                        ;  ref8_cfm                    "},
        {"X , 0x0592  , 0x00                        ;  ref8_gst_disqual            "},
        {"X , 0x0593  , 0x05                        ;  ref8_gst_disqual            "},
        {"X , 0x0594  , 0x00                        ;  ref8_gst_qual               "},
        {"X , 0x0595  , 0x14                        ;  ref8_gst_qual               "},
        {"X , 0x0597  , 0x00                        ;  ref8_pfm_ctrl               "},
        {"X , 0x0598  , 0x65                        ;  ref8_pfm_disqualify         "},
        {"X , 0x0599  , 0x68                        ;  ref8_pfm_disqualify         "},
        {"X , 0x059A  , 0x4E                        ;  ref8_pfm_qualify            "},
        {"X , 0x059B  , 0x48                        ;  ref8_pfm_qualify            "},
        {"X , 0x059C  , 0x00                        ;  ref8_pfm_period             "},
        {"X , 0x059D  , 0x00                        ;  ref8_pfm_period             "},
        {"X , 0x059E  , 0x28                        ;  ref8_pfm_filter_limit       "},
        {"X , 0x05A0  , 0x00                        ;  ref8_sync                   "},
        {"X , 0x05A1  , 0x00                        ;  ref8_sync_misc              "},
        {"X , 0x05A2  , 0x00                        ;  ref8_sync_offset_comp       "},
        {"X , 0x05A3  , 0x00                        ;  ref8_phase_offset_compensation"},
        {"X , 0x05A4  , 0x00                        ;  ref8_phase_offset_compensation"},
        {"X , 0x05A5  , 0x00                        ;  ref8_phase_offset_compensation"},
        {"X , 0x05A6  , 0x00                        ;  ref8_phase_offset_compensation"},
        {"X , 0x05A8  , 0x00                        ;  ref8_scm_fine               "},
        {"X , 0x05A9  , 0x00                        ;  ref8_scm_fine               "},
        {"X , 0x05AA  , 0x00                        ;  ref8_scm_fine               "},
        {"X , 0x05AB  , 0x00                        ;  ref8_scm_fine               "},
        {"X , 0x0584  , 0x01                        ;  ref_mb_sem                  "},
        {"W , 20000 "},

        {"X , 0x0582  , 0x02                        ;  ref_mb_mask                 "},
        {"X , 0x0583  , 0x00                        ;  ref_mb_mask                 "},
        {"X , 0x0584  , 0x02                        ;  ref_mb_sem                  "},
        {"W , 20000 "},
        {"X , 0x0585  , 0x61                        ;  ref9_freq_base              "},
        {"X , 0x0586  , 0xA8                        ;  ref9_freq_base              "},
        {"X , 0x0587  , 0x01                        ;  ref9_freq_mult              "},
        {"X , 0x0588  , 0x90                        ;  ref9_freq_mult              "},
        {"X , 0x0589  , 0x00                        ;  ref9_ratio_m                "},
        {"X , 0x058A  , 0x01                        ;  ref9_ratio_m                "},
        {"X , 0x058B  , 0x00                        ;  ref9_ratio_n                "},
        {"X , 0x058C  , 0x01                        ;  ref9_ratio_n                "},
        {"X , 0x058D  , 0x01                        ;  ref9_config                 "},
        {"X , 0x0590  , 0x05                        ;  ref9_scm                    "},
        {"X , 0x0591  , 0x05                        ;  ref9_cfm                    "},
        {"X , 0x0592  , 0x00                        ;  ref9_gst_disqual            "},
        {"X , 0x0593  , 0x05                        ;  ref9_gst_disqual            "},
        {"X , 0x0594  , 0x00                        ;  ref9_gst_qual               "},
        {"X , 0x0595  , 0x14                        ;  ref9_gst_qual               "},
        {"X , 0x0597  , 0x00                        ;  ref9_pfm_ctrl               "},
        {"X , 0x0598  , 0x65                        ;  ref9_pfm_disqualify         "},
        {"X , 0x0599  , 0x68                        ;  ref9_pfm_disqualify         "},
        {"X , 0x059A  , 0x4E                        ;  ref9_pfm_qualify            "},
        {"X , 0x059B  , 0x48                        ;  ref9_pfm_qualify            "},
        {"X , 0x059C  , 0x00                        ;  ref9_pfm_period             "},
        {"X , 0x059D  , 0x00                        ;  ref9_pfm_period             "},
        {"X , 0x059E  , 0x28                        ;  ref9_pfm_filter_limit       "},
        {"X , 0x05A0  , 0x70                        ;  ref9_sync                   "},
        {"X , 0x05A1  , 0x00                        ;  ref9_sync_misc              "},
        {"X , 0x05A2  , 0x00                        ;  ref9_sync_offset_comp       "},
        {"X , 0x05A3  , 0x00                        ;  ref9_phase_offset_compensation"},
        {"X , 0x05A4  , 0x00                        ;  ref9_phase_offset_compensation"},
        {"X , 0x05A5  , 0x00                        ;  ref9_phase_offset_compensation"},
        {"X , 0x05A6  , 0x00                        ;  ref9_phase_offset_compensation"},
        {"X , 0x05A8  , 0x00                        ;  ref9_scm_fine               "},
        {"X , 0x05A9  , 0x00                        ;  ref9_scm_fine               "},
        {"X , 0x05AA  , 0x00                        ;  ref9_scm_fine               "},
        {"X , 0x05AB  , 0x00                        ;  ref9_scm_fine               "},
        {"X , 0x0584  , 0x01                        ;  ref_mb_sem                  "},
        {"W , 20000 "},

        {"X , 0x0602  , 0x00                        ;  dpll_mb_mask                "},
        {"X , 0x0603  , 0x01                        ;  dpll_mb_mask                "},
        {"X , 0x0604  , 0x02                        ;  dpll_mb_sem                 "},
        {"W , 20000 "},
        {"X , 0x0605  , 0x00                        ;  dpll0_bw_fixed              "},
        {"X , 0x0606  , 0x00                        ;  dpll0_bw_var                "},
        {"X , 0x0607  , 0x00                        ;  dpll0_config                "},
        {"X , 0x0608  , 0x00                        ;  dpll0_psl                   "},
        {"X , 0x0609  , 0x00                        ;  dpll0_psl                   "},
        {"X , 0x060A  , 0x00                        ;  dpll0_psl_max_phase         "},
        {"X , 0x060B  , 0x64                        ;  dpll0_psl_max_phase         "},
        {"X , 0x060C  , 0xE7                        ;  dpll0_psl_scaling           "},
        {"X , 0x060D  , 0x08                        ;  dpll0_psl_decay             "},
        {"X , 0x060E  , 0x00                        ;  dpll0_range                 "},
        {"X , 0x060F  , 0x78                        ;  dpll0_range                 "},
        {"X , 0x0610  , 0x08                        ;  dpll0_ref_sw_mask           "},
        {"X , 0x0611  , 0x17                        ;  dpll0_ref_ho_mask           "},
        {"X , 0x0614  , 0x10                        ;  dpll0_ref_prio_0            "},
        {"X , 0x0615  , 0x32                        ;  dpll0_ref_prio_1            "},
        {"X , 0x0616  , 0x54                        ;  dpll0_ref_prio_2            "},
        {"X , 0x0617  , 0x76                        ;  dpll0_ref_prio_3            "},
        {"X , 0x0618  , 0x98                        ;  dpll0_ref_prio_4            "},
        {"X , 0x061C  , 0x00                        ;  dpll0_ho_filter             "},
        {"X , 0x061D  , 0x4C                        ;  dpll0_ho_delay              "},
        {"X , 0x061E  , 0x00                        ;  dpll0_split_xo_config       "},
        {"X , 0x0620  , 0x01                        ;  dpll0_fast_lock_ctrl        "},
        {"X , 0x0621  , 0xFF                        ;  dpll0_fast_lock_phase_err   "},
        {"X , 0x0622  , 0x04                        ;  dpll0_fast_lock_freq_err    "},
        {"X , 0x0623  , 0x00                        ;  dpll0_fast_lock_ideal_time  "},
        {"X , 0x0624  , 0x00                        ;  dpll0_fast_lock_ideal_time  "},
        {"X , 0x0626  , 0x07                        ;  dpll0_fast_lock_fol         "},
        {"X , 0x0627  , 0xD0                        ;  dpll0_fast_lock_fol         "},
        {"X , 0x062E  , 0x00                        ;  dpll0_damping               "},
        {"X , 0x0634  , 0x02                        ;  dpll0_phase_good            "},
        {"X , 0x0635  , 0x25                        ;  dpll0_phase_good            "},
        {"X , 0x0636  , 0x51                        ;  dpll0_phase_good            "},
        {"X , 0x0637  , 0x00                        ;  dpll0_phase_good            "},
        {"X , 0x0638  , 0x09                        ;  dpll0_duration_good         "},
        {"X , 0x0639  , 0x00                        ;  dpll0_lock_delay            "},
        {"X , 0x063A  , 0x00                        ;  dpll0_tie                   "},
        {"X , 0x063B  , 0x00                        ;  dpll0_tie_wr_thresh         "},
        {"X , 0x063C  , 0x7F                        ;  dpll0_fp_first_realign      "},
        {"X , 0x063D  , 0x00                        ;  dpll0_fp_realign_intvl      "},
        {"X , 0x063E  , 0x00                        ;  dpll0_fp_lock_thresh        "},
        {"X , 0x0640  , 0x77                        ;  dpll0_step_time_thresh      "},
        {"X , 0x0641  , 0x35                        ;  dpll0_step_time_thresh      "},
        {"X , 0x0642  , 0x94                        ;  dpll0_step_time_thresh      "},
        {"X , 0x0643  , 0x00                        ;  dpll0_step_time_thresh      "},
        {"X , 0x0644  , 0x77                        ;  dpll0_step_time_reso        "},
        {"X , 0x0645  , 0x35                        ;  dpll0_step_time_reso        "},
        {"X , 0x0646  , 0x94                        ;  dpll0_step_time_reso        "},
        {"X , 0x0647  , 0x00                        ;  dpll0_step_time_reso        "},
        {"X , 0x0648  , 0x40                        ;  dpll0_bw_thresh_speedup     "},
        {"X , 0x0604  , 0x01                        ;  dpll_mb_sem                 "},
        {"W , 20000 "},

        {"X , 0x0602  , 0x00                        ;  dpll_mb_mask                "},
        {"X , 0x0603  , 0x02                        ;  dpll_mb_mask                "},
        {"X , 0x0604  , 0x02                        ;  dpll_mb_sem                 "},
        {"W , 20000 "},
        {"X , 0x0605  , 0x00                        ;  dpll1_bw_fixed              "},
        {"X , 0x0606  , 0x00                        ;  dpll1_bw_var                "},
        {"X , 0x0607  , 0x00                        ;  dpll1_config                "},
        {"X , 0x0608  , 0x00                        ;  dpll1_psl                   "},
        {"X , 0x0609  , 0x00                        ;  dpll1_psl                   "},
        {"X , 0x060A  , 0x00                        ;  dpll1_psl_max_phase         "},
        {"X , 0x060B  , 0x64                        ;  dpll1_psl_max_phase         "},
        {"X , 0x060C  , 0xE7                        ;  dpll1_psl_scaling           "},
        {"X , 0x060D  , 0x08                        ;  dpll1_psl_decay             "},
        {"X , 0x060E  , 0x00                        ;  dpll1_range                 "},
        {"X , 0x060F  , 0x78                        ;  dpll1_range                 "},
        {"X , 0x0610  , 0x08                        ;  dpll1_ref_sw_mask           "},
        {"X , 0x0611  , 0x17                        ;  dpll1_ref_ho_mask           "},
        {"X , 0x0614  , 0x10                        ;  dpll1_ref_prio_0            "},
        {"X , 0x0615  , 0x32                        ;  dpll1_ref_prio_1            "},
        {"X , 0x0616  , 0x54                        ;  dpll1_ref_prio_2            "},
        {"X , 0x0617  , 0x76                        ;  dpll1_ref_prio_3            "},
        {"X , 0x0618  , 0x98                        ;  dpll1_ref_prio_4            "},
        {"X , 0x061C  , 0x00                        ;  dpll1_ho_filter             "},
        {"X , 0x061D  , 0x4C                        ;  dpll1_ho_delay              "},
        {"X , 0x061E  , 0x00                        ;  dpll1_split_xo_config       "},
        {"X , 0x0620  , 0x01                        ;  dpll1_fast_lock_ctrl        "},
        {"X , 0x0621  , 0xFF                        ;  dpll1_fast_lock_phase_err   "},
        {"X , 0x0622  , 0x04                        ;  dpll1_fast_lock_freq_err    "},
        {"X , 0x0623  , 0x00                        ;  dpll1_fast_lock_ideal_time  "},
        {"X , 0x0624  , 0x00                        ;  dpll1_fast_lock_ideal_time  "},
        {"X , 0x0626  , 0x07                        ;  dpll1_fast_lock_fol         "},
        {"X , 0x0627  , 0xD0                        ;  dpll1_fast_lock_fol         "},
        {"X , 0x062E  , 0x00                        ;  dpll1_damping               "},
        {"X , 0x0634  , 0x02                        ;  dpll1_phase_good            "},
        {"X , 0x0635  , 0x25                        ;  dpll1_phase_good            "},
        {"X , 0x0636  , 0x51                        ;  dpll1_phase_good            "},
        {"X , 0x0637  , 0x00                        ;  dpll1_phase_good            "},
        {"X , 0x0638  , 0x09                        ;  dpll1_duration_good         "},
        {"X , 0x0639  , 0x00                        ;  dpll1_lock_delay            "},
        {"X , 0x063A  , 0x00                        ;  dpll1_tie                   "},
        {"X , 0x063B  , 0x00                        ;  dpll1_tie_wr_thresh         "},
        {"X , 0x063C  , 0x7F                        ;  dpll1_fp_first_realign      "},
        {"X , 0x063D  , 0x00                        ;  dpll1_fp_realign_intvl      "},
        {"X , 0x063E  , 0x00                        ;  dpll1_fp_lock_thresh        "},
        {"X , 0x0640  , 0x77                        ;  dpll1_step_time_thresh      "},
        {"X , 0x0641  , 0x35                        ;  dpll1_step_time_thresh      "},
        {"X , 0x0642  , 0x94                        ;  dpll1_step_time_thresh      "},
        {"X , 0x0643  , 0x00                        ;  dpll1_step_time_thresh      "},
        {"X , 0x0644  , 0xEE                        ;  dpll1_step_time_reso        "},
        {"X , 0x0645  , 0x6B                        ;  dpll1_step_time_reso        "},
        {"X , 0x0646  , 0x28                        ;  dpll1_step_time_reso        "},
        {"X , 0x0647  , 0x00                        ;  dpll1_step_time_reso        "},
        {"X , 0x0648  , 0x40                        ;  dpll1_bw_thresh_speedup     "},
        {"X , 0x0604  , 0x01                        ;  dpll_mb_sem                 "},
        {"W , 20000 "},

        {"X , 0x0602  , 0x00                        ;  dpll_mb_mask                "},
        {"X , 0x0603  , 0x04                        ;  dpll_mb_mask                "},
        {"X , 0x0604  , 0x02                        ;  dpll_mb_sem                 "},
        {"W , 20000 "},
        {"X , 0x0605  , 0x00                        ;  dpll2_bw_fixed              "},
        {"X , 0x0606  , 0x00                        ;  dpll2_bw_var                "},
        {"X , 0x0607  , 0x00                        ;  dpll2_config                "},
        {"X , 0x0608  , 0x00                        ;  dpll2_psl                   "},
        {"X , 0x0609  , 0x00                        ;  dpll2_psl                   "},
        {"X , 0x060A  , 0x00                        ;  dpll2_psl_max_phase         "},
        {"X , 0x060B  , 0x64                        ;  dpll2_psl_max_phase         "},
        {"X , 0x060C  , 0xE7                        ;  dpll2_psl_scaling           "},
        {"X , 0x060D  , 0x08                        ;  dpll2_psl_decay             "},
        {"X , 0x060E  , 0x00                        ;  dpll2_range                 "},
        {"X , 0x060F  , 0x78                        ;  dpll2_range                 "},
        {"X , 0x0610  , 0x08                        ;  dpll2_ref_sw_mask           "},
        {"X , 0x0611  , 0x17                        ;  dpll2_ref_ho_mask           "},
        {"X , 0x0614  , 0x10                        ;  dpll2_ref_prio_0            "},
        {"X , 0x0615  , 0x32                        ;  dpll2_ref_prio_1            "},
        {"X , 0x0616  , 0x54                        ;  dpll2_ref_prio_2            "},
        {"X , 0x0617  , 0x76                        ;  dpll2_ref_prio_3            "},
        {"X , 0x0618  , 0x98                        ;  dpll2_ref_prio_4            "},
        {"X , 0x061C  , 0x00                        ;  dpll2_ho_filter             "},
        {"X , 0x061D  , 0x4C                        ;  dpll2_ho_delay              "},
        {"X , 0x061E  , 0x00                        ;  dpll2_split_xo_config       "},
        {"X , 0x0620  , 0x01                        ;  dpll2_fast_lock_ctrl        "},
        {"X , 0x0621  , 0xFF                        ;  dpll2_fast_lock_phase_err   "},
        {"X , 0x0622  , 0x04                        ;  dpll2_fast_lock_freq_err    "},
        {"X , 0x0623  , 0x00                        ;  dpll2_fast_lock_ideal_time  "},
        {"X , 0x0624  , 0x00                        ;  dpll2_fast_lock_ideal_time  "},
        {"X , 0x0626  , 0x07                        ;  dpll2_fast_lock_fol         "},
        {"X , 0x0627  , 0xD0                        ;  dpll2_fast_lock_fol         "},
        {"X , 0x062E  , 0x00                        ;  dpll2_damping               "},
        {"X , 0x0634  , 0x02                        ;  dpll2_phase_good            "},
        {"X , 0x0635  , 0x25                        ;  dpll2_phase_good            "},
        {"X , 0x0636  , 0x51                        ;  dpll2_phase_good            "},
        {"X , 0x0637  , 0x00                        ;  dpll2_phase_good            "},
        {"X , 0x0638  , 0x09                        ;  dpll2_duration_good         "},
        {"X , 0x0639  , 0x00                        ;  dpll2_lock_delay            "},
        {"X , 0x063A  , 0x00                        ;  dpll2_tie                   "},
        {"X , 0x063B  , 0x00                        ;  dpll2_tie_wr_thresh         "},
        {"X , 0x063C  , 0x7F                        ;  dpll2_fp_first_realign      "},
        {"X , 0x063D  , 0x00                        ;  dpll2_fp_realign_intvl      "},
        {"X , 0x063E  , 0x00                        ;  dpll2_fp_lock_thresh        "},
        {"X , 0x0640  , 0x77                        ;  dpll2_step_time_thresh      "},
        {"X , 0x0641  , 0x35                        ;  dpll2_step_time_thresh      "},
        {"X , 0x0642  , 0x94                        ;  dpll2_step_time_thresh      "},
        {"X , 0x0643  , 0x00                        ;  dpll2_step_time_thresh      "},
        {"X , 0x0644  , 0xEE                        ;  dpll2_step_time_reso        "},
        {"X , 0x0645  , 0x6B                        ;  dpll2_step_time_reso        "},
        {"X , 0x0646  , 0x28                        ;  dpll2_step_time_reso        "},
        {"X , 0x0647  , 0x00                        ;  dpll2_step_time_reso        "},
        {"X , 0x0648  , 0x40                        ;  dpll2_bw_thresh_speedup     "},
        {"X , 0x0604  , 0x01                        ;  dpll_mb_sem                 "},
        {"W , 20000 "},

        {"X , 0x0602  , 0x00                        ;  dpll_mb_mask                "},
        {"X , 0x0603  , 0x08                        ;  dpll_mb_mask                "},
        {"X , 0x0604  , 0x02                        ;  dpll_mb_sem                 "},
        {"W , 20000 "},
        {"X , 0x0605  , 0x00                        ;  dpll3_bw_fixed              "},
        {"X , 0x0606  , 0x00                        ;  dpll3_bw_var                "},
        {"X , 0x0607  , 0x00                        ;  dpll3_config                "},
        {"X , 0x0608  , 0x00                        ;  dpll3_psl                   "},
        {"X , 0x0609  , 0x00                        ;  dpll3_psl                   "},
        {"X , 0x060A  , 0x00                        ;  dpll3_psl_max_phase         "},
        {"X , 0x060B  , 0x64                        ;  dpll3_psl_max_phase         "},
        {"X , 0x060C  , 0xE7                        ;  dpll3_psl_scaling           "},
        {"X , 0x060D  , 0x08                        ;  dpll3_psl_decay             "},
        {"X , 0x060E  , 0x00                        ;  dpll3_range                 "},
        {"X , 0x060F  , 0x78                        ;  dpll3_range                 "},
        {"X , 0x0610  , 0x08                        ;  dpll3_ref_sw_mask           "},
        {"X , 0x0611  , 0x17                        ;  dpll3_ref_ho_mask           "},
        {"X , 0x0614  , 0x10                        ;  dpll3_ref_prio_0            "},
        {"X , 0x0615  , 0x32                        ;  dpll3_ref_prio_1            "},
        {"X , 0x0616  , 0x54                        ;  dpll3_ref_prio_2            "},
        {"X , 0x0617  , 0x76                        ;  dpll3_ref_prio_3            "},
        {"X , 0x0618  , 0x98                        ;  dpll3_ref_prio_4            "},
        {"X , 0x061C  , 0x00                        ;  dpll3_ho_filter             "},
        {"X , 0x061D  , 0x4C                        ;  dpll3_ho_delay              "},
        {"X , 0x061E  , 0x00                        ;  dpll3_split_xo_config       "},
        {"X , 0x0620  , 0x01                        ;  dpll3_fast_lock_ctrl        "},
        {"X , 0x0621  , 0xFF                        ;  dpll3_fast_lock_phase_err   "},
        {"X , 0x0622  , 0x04                        ;  dpll3_fast_lock_freq_err    "},
        {"X , 0x0623  , 0x00                        ;  dpll3_fast_lock_ideal_time  "},
        {"X , 0x0624  , 0x00                        ;  dpll3_fast_lock_ideal_time  "},
        {"X , 0x0626  , 0x07                        ;  dpll3_fast_lock_fol         "},
        {"X , 0x0627  , 0xD0                        ;  dpll3_fast_lock_fol         "},
        {"X , 0x062E  , 0x00                        ;  dpll3_damping               "},
        {"X , 0x0634  , 0x02                        ;  dpll3_phase_good            "},
        {"X , 0x0635  , 0x25                        ;  dpll3_phase_good            "},
        {"X , 0x0636  , 0x51                        ;  dpll3_phase_good            "},
        {"X , 0x0637  , 0x00                        ;  dpll3_phase_good            "},
        {"X , 0x0638  , 0x09                        ;  dpll3_duration_good         "},
        {"X , 0x0639  , 0x00                        ;  dpll3_lock_delay            "},
        {"X , 0x063A  , 0x00                        ;  dpll3_tie                   "},
        {"X , 0x063B  , 0x00                        ;  dpll3_tie_wr_thresh         "},
        {"X , 0x063C  , 0x7F                        ;  dpll3_fp_first_realign      "},
        {"X , 0x063D  , 0x00                        ;  dpll3_fp_realign_intvl      "},
        {"X , 0x063E  , 0x00                        ;  dpll3_fp_lock_thresh        "},
        {"X , 0x0640  , 0x77                        ;  dpll3_step_time_thresh      "},
        {"X , 0x0641  , 0x35                        ;  dpll3_step_time_thresh      "},
        {"X , 0x0642  , 0x94                        ;  dpll3_step_time_thresh      "},
        {"X , 0x0643  , 0x00                        ;  dpll3_step_time_thresh      "},
        {"X , 0x0644  , 0xEE                        ;  dpll3_step_time_reso        "},
        {"X , 0x0645  , 0x6B                        ;  dpll3_step_time_reso        "},
        {"X , 0x0646  , 0x28                        ;  dpll3_step_time_reso        "},
        {"X , 0x0647  , 0x00                        ;  dpll3_step_time_reso        "},
        {"X , 0x0648  , 0x40                        ;  dpll3_bw_thresh_speedup     "},
        {"X , 0x0604  , 0x01                        ;  dpll_mb_sem                 "},
        {"W , 20000 "},

        {"X , 0x0602  , 0x00                        ;  dpll_mb_mask                "},
        {"X , 0x0603  , 0x10                        ;  dpll_mb_mask                "},
        {"X , 0x0604  , 0x02                        ;  dpll_mb_sem                 "},
        {"W , 20000 "},
        {"X , 0x0605  , 0x00                        ;  dpll4_bw_fixed              "},
        {"X , 0x0606  , 0x00                        ;  dpll4_bw_var                "},
        {"X , 0x0607  , 0x00                        ;  dpll4_config                "},
        {"X , 0x0608  , 0x00                        ;  dpll4_psl                   "},
        {"X , 0x0609  , 0x00                        ;  dpll4_psl                   "},
        {"X , 0x060A  , 0x00                        ;  dpll4_psl_max_phase         "},
        {"X , 0x060B  , 0x64                        ;  dpll4_psl_max_phase         "},
        {"X , 0x060C  , 0xE7                        ;  dpll4_psl_scaling           "},
        {"X , 0x060D  , 0x08                        ;  dpll4_psl_decay             "},
        {"X , 0x060E  , 0x00                        ;  dpll4_range                 "},
        {"X , 0x060F  , 0x78                        ;  dpll4_range                 "},
        {"X , 0x0610  , 0x08                        ;  dpll4_ref_sw_mask           "},
        {"X , 0x0611  , 0x17                        ;  dpll4_ref_ho_mask           "},
        {"X , 0x0614  , 0x10                        ;  dpll4_ref_prio_0            "},
        {"X , 0x0615  , 0x32                        ;  dpll4_ref_prio_1            "},
        {"X , 0x0616  , 0x54                        ;  dpll4_ref_prio_2            "},
        {"X , 0x0617  , 0x76                        ;  dpll4_ref_prio_3            "},
        {"X , 0x0618  , 0x98                        ;  dpll4_ref_prio_4            "},
        {"X , 0x061C  , 0x00                        ;  dpll4_ho_filter             "},
        {"X , 0x061D  , 0x4C                        ;  dpll4_ho_delay              "},
        {"X , 0x061E  , 0x00                        ;  dpll4_split_xo_config       "},
        {"X , 0x0620  , 0x01                        ;  dpll4_fast_lock_ctrl        "},
        {"X , 0x0621  , 0xFF                        ;  dpll4_fast_lock_phase_err   "},
        {"X , 0x0622  , 0x04                        ;  dpll4_fast_lock_freq_err    "},
        {"X , 0x0623  , 0x00                        ;  dpll4_fast_lock_ideal_time  "},
        {"X , 0x0624  , 0x00                        ;  dpll4_fast_lock_ideal_time  "},
        {"X , 0x0626  , 0x07                        ;  dpll4_fast_lock_fol         "},
        {"X , 0x0627  , 0xD0                        ;  dpll4_fast_lock_fol         "},
        {"X , 0x062E  , 0x00                        ;  dpll4_damping               "},
        {"X , 0x0634  , 0x02                        ;  dpll4_phase_good            "},
        {"X , 0x0635  , 0x25                        ;  dpll4_phase_good            "},
        {"X , 0x0636  , 0x51                        ;  dpll4_phase_good            "},
        {"X , 0x0637  , 0x00                        ;  dpll4_phase_good            "},
        {"X , 0x0638  , 0x09                        ;  dpll4_duration_good         "},
        {"X , 0x0639  , 0x00                        ;  dpll4_lock_delay            "},
        {"X , 0x063A  , 0x00                        ;  dpll4_tie                   "},
        {"X , 0x063B  , 0x00                        ;  dpll4_tie_wr_thresh         "},
        {"X , 0x063C  , 0x7F                        ;  dpll4_fp_first_realign      "},
        {"X , 0x063D  , 0x00                        ;  dpll4_fp_realign_intvl      "},
        {"X , 0x063E  , 0x00                        ;  dpll4_fp_lock_thresh        "},
        {"X , 0x0640  , 0x77                        ;  dpll4_step_time_thresh      "},
        {"X , 0x0641  , 0x35                        ;  dpll4_step_time_thresh      "},
        {"X , 0x0642  , 0x94                        ;  dpll4_step_time_thresh      "},
        {"X , 0x0643  , 0x00                        ;  dpll4_step_time_thresh      "},
        {"X , 0x0644  , 0xEE                        ;  dpll4_step_time_reso        "},
        {"X , 0x0645  , 0x6B                        ;  dpll4_step_time_reso        "},
        {"X , 0x0646  , 0x28                        ;  dpll4_step_time_reso        "},
        {"X , 0x0647  , 0x00                        ;  dpll4_step_time_reso        "},
        {"X , 0x0648  , 0x40                        ;  dpll4_bw_thresh_speedup     "},
        {"X , 0x0604  , 0x01                        ;  dpll_mb_sem                 "},
        {"W , 20000 "},

        {"X , 0x0602  , 0x00                        ;  dpll_mb_mask                "},
        {"X , 0x0603  , 0x20                        ;  dpll_mb_mask                "},
        {"X , 0x0604  , 0x02                        ;  dpll_mb_sem                 "},
        {"W , 20000 "},
        {"X , 0x0605  , 0x00                        ;  dpll5_bw_fixed              "},
        {"X , 0x0606  , 0x00                        ;  dpll5_bw_var                "},
        {"X , 0x0607  , 0x00                        ;  dpll5_config                "},
        {"X , 0x0608  , 0x00                        ;  dpll5_psl                   "},
        {"X , 0x0609  , 0x00                        ;  dpll5_psl                   "},
        {"X , 0x060A  , 0x00                        ;  dpll5_psl_max_phase         "},
        {"X , 0x060B  , 0x64                        ;  dpll5_psl_max_phase         "},
        {"X , 0x060C  , 0xE7                        ;  dpll5_psl_scaling           "},
        {"X , 0x060D  , 0x08                        ;  dpll5_psl_decay             "},
        {"X , 0x060E  , 0x00                        ;  dpll5_range                 "},
        {"X , 0x060F  , 0x78                        ;  dpll5_range                 "},
        {"X , 0x0610  , 0x08                        ;  dpll5_ref_sw_mask           "},
        {"X , 0x0611  , 0x17                        ;  dpll5_ref_ho_mask           "},
        {"X , 0x0614  , 0x10                        ;  dpll5_ref_prio_0            "},
        {"X , 0x0615  , 0x32                        ;  dpll5_ref_prio_1            "},
        {"X , 0x0616  , 0x54                        ;  dpll5_ref_prio_2            "},
        {"X , 0x0617  , 0x76                        ;  dpll5_ref_prio_3            "},
        {"X , 0x0618  , 0x98                        ;  dpll5_ref_prio_4            "},
        {"X , 0x061C  , 0x00                        ;  dpll5_ho_filter             "},
        {"X , 0x061D  , 0x4C                        ;  dpll5_ho_delay              "},
        {"X , 0x061E  , 0x00                        ;  dpll5_split_xo_config       "},
        {"X , 0x0620  , 0x01                        ;  dpll5_fast_lock_ctrl        "},
        {"X , 0x0621  , 0xFF                        ;  dpll5_fast_lock_phase_err   "},
        {"X , 0x0622  , 0x04                        ;  dpll5_fast_lock_freq_err    "},
        {"X , 0x0623  , 0x00                        ;  dpll5_fast_lock_ideal_time  "},
        {"X , 0x0624  , 0x00                        ;  dpll5_fast_lock_ideal_time  "},
        {"X , 0x0626  , 0x07                        ;  dpll5_fast_lock_fol         "},
        {"X , 0x0627  , 0xD0                        ;  dpll5_fast_lock_fol         "},
        {"X , 0x062E  , 0x00                        ;  dpll5_damping               "},
        {"X , 0x0634  , 0x02                        ;  dpll5_phase_good            "},
        {"X , 0x0635  , 0x25                        ;  dpll5_phase_good            "},
        {"X , 0x0636  , 0x51                        ;  dpll5_phase_good            "},
        {"X , 0x0637  , 0x00                        ;  dpll5_phase_good            "},
        {"X , 0x0638  , 0x09                        ;  dpll5_duration_good         "},
        {"X , 0x0639  , 0x00                        ;  dpll5_lock_delay            "},
        {"X , 0x063A  , 0x00                        ;  dpll5_tie                   "},
        {"X , 0x063B  , 0x00                        ;  dpll5_tie_wr_thresh         "},
        {"X , 0x063C  , 0x7F                        ;  dpll5_fp_first_realign      "},
        {"X , 0x063D  , 0x00                        ;  dpll5_fp_realign_intvl      "},
        {"X , 0x063E  , 0x00                        ;  dpll5_fp_lock_thresh        "},
        {"X , 0x0640  , 0x77                        ;  dpll5_step_time_thresh      "},
        {"X , 0x0641  , 0x35                        ;  dpll5_step_time_thresh      "},
        {"X , 0x0642  , 0x94                        ;  dpll5_step_time_thresh      "},
        {"X , 0x0643  , 0x00                        ;  dpll5_step_time_thresh      "},
        {"X , 0x0644  , 0xEE                        ;  dpll5_step_time_reso        "},
        {"X , 0x0645  , 0x6B                        ;  dpll5_step_time_reso        "},
        {"X , 0x0646  , 0x28                        ;  dpll5_step_time_reso        "},
        {"X , 0x0647  , 0x00                        ;  dpll5_step_time_reso        "},
        {"X , 0x0648  , 0x40                        ;  dpll5_bw_thresh_speedup     "},
        {"X , 0x0604  , 0x01                        ;  dpll_mb_sem                 "},
        {"W , 20000 "},
        {"X , 0x0000  , 0xA0                        ;  info                        "},
        {"X , 0x0001  , 0x00                        ;  id                          "},
        {"X , 0x0002  , 0x00                        ;  id                          "},
        {"X , 0x0003  , 0x00                        ;  revision                    "},
        {"X , 0x0004  , 0x00                        ;  boot_status                 "},
        {"X , 0x0005  , 0x00                        ;  fw_ver                      "},
        {"X , 0x0006  , 0x00                        ;  fw_ver                      "},
        {"X , 0x0007  , 0xFF                        ;  custom_config_ver           "},
        {"X , 0x0008  , 0xFF                        ;  custom_config_ver           "},
        {"X , 0x0009  , 0xFF                        ;  custom_config_ver           "},
        {"X , 0x000A  , 0xFF                        ;  custom_config_ver           "},
        {"X , 0x000B  , 0x18                        ;  central_freq_offset         "},
        {"X , 0x000C  , 0x00                        ;  central_freq_offset         "},
        {"X , 0x000D  , 0x72                        ;  central_freq_offset         "},
        {"X , 0x000E  , 0xB0                        ;  central_freq_offset         "},
        {"X , 0x000F  , 0x00                        ;  hw_record_ver               "},
        {"X , 0x0010  , 0x00                        ;  init_fail                   "},
        {"X , 0x0011  , 0x03                        ;  auto_config_sel             "},
        {"X , 0x0012  , 0x00                        ;  warm_start                  "},
        {"X , 0x0013  , 0x00                        ;  trap_status                 "},
        {"X , 0x0014  , 0x00                        ;  trap_mask                   "},
        {"X , 0x0015  , 0x00                        ;  trap_cnt                    "},
        {"X , 0x0016  , 0x00                        ;  internal_error              "},
        {"X , 0x0017  , 0x00                        ;  internal_error              "},
        {"X , 0x0018  , 0x00                        ;  reset_status                "},
        {"X , 0x0019  , 0x00                        ;  gpio_at_startup             "},
        {"X , 0x001A  , 0x00                        ;  gpio_at_startup             "},
        {"X , 0x001B  , 0x00                        ;  patch_status                "},
        {"X , 0x001C  , 0x00                        ;  patch_control               "},
        {"X , 0x001D  , 0x00                        ;  patch_control               "},
        {"X , 0x001E  , 0x00                        ;  patch_control               "},
        {"X , 0x001F  , 0x00                        ;  patch_control               "},
        {"X , 0x0020  , 0x00                        ;  reset_ctrl                  "},
        {"X , 0x002C  , 0x02                        ;  osci_ctrl                   "},
        {"X , 0x002D  , 0x00                        ;  osci_ctrl2                  "},
        {"X , 0x002E  , 0x00                        ;  osci_amp                    "},
        {"X , 0x002F  , 0x00                        ;  osci_cap                    "},
        {"X , 0x0040  , 0x00                        ;  max_isr_count               "},
        {"X , 0x0041  , 0x00                        ;  lospeed_count               "},
        {"X , 0x0080  , 0x00                        ;  gpio_irq_config             "},
        {"X , 0x0082  , 0x00                        ;  gpio_out_2_0                "},
        {"X , 0x0084  , 0x00                        ;  gpio_freeze_2_0             "},
        {"X , 0x0088  , 0x00                        ;  gpio_select_0               "},
        {"X , 0x0089  , 0x00                        ;  gpio_select_0               "},
        {"X , 0x008A  , 0x00                        ;  gpio_config_0               "},
        {"X , 0x008B  , 0x00                        ;  gpio_select_1               "},
        {"X , 0x008C  , 0x00                        ;  gpio_select_1               "},
        {"X , 0x008D  , 0x00                        ;  gpio_config_1               "},
        {"X , 0x008E  , 0x00                        ;  gpio_select_2               "},
        {"X , 0x008F  , 0x00                        ;  gpio_select_2               "},
        {"X , 0x0090  , 0x00                        ;  gpio_config_2               "},
        {"X , 0x0097  , 0x00                        ;  gpio_select_5               "},
        {"X , 0x0098  , 0x00                        ;  gpio_config_5               "},
        {"X , 0x0099  , 0x00                        ;  gpio_select_6               "},
        {"X , 0x009A  , 0x00                        ;  gpio_config_6               "},
        {"X , 0x009B  , 0x00                        ;  gpio_select_7               "},
        {"X , 0x009C  , 0x00                        ;  gpio_config_7               "},
        {"X , 0x009D  , 0x00                        ;  gpio_select_8               "},
        {"X , 0x009E  , 0x00                        ;  gpio_config_8               "},
        {"X , 0x00A8  , 0x00                        ;  ref_irq_mask_3_0            "},
        {"X , 0x00A9  , 0x00                        ;  ref_irq_mask_4              "},
        {"X , 0x00AB  , 0x00                        ;  dpll_irq_mask               "},
        {"X , 0x00AC  , 0x00                        ;  synth_irq_mask              "},
        {"X , 0x00AD  , 0x00                        ;  hp_out_irq_mask             "},
        {"X , 0x00B0  , 0x00                        ;  ref_mon_th_mask_0P          "},
        {"X , 0x00B1  , 0x00                        ;  ref_mon_tl_mask_0P          "},
        {"X , 0x00B2  , 0x00                        ;  ref_mon_th_mask_0N          "},
        {"X , 0x00B3  , 0x00                        ;  ref_mon_tl_mask_0N          "},
        {"X , 0x00B4  , 0x00                        ;  ref_mon_th_mask_1P          "},
        {"X , 0x00B5  , 0x00                        ;  ref_mon_tl_mask_1P          "},
        {"X , 0x00B6  , 0x00                        ;  ref_mon_th_mask_1N          "},
        {"X , 0x00B7  , 0x00                        ;  ref_mon_tl_mask_1N          "},
        {"X , 0x00B8  , 0x00                        ;  ref_mon_th_mask_2P          "},
        {"X , 0x00B9  , 0x00                        ;  ref_mon_tl_mask_2P          "},
        {"X , 0x00BA  , 0x00                        ;  ref_mon_th_mask_2N          "},
        {"X , 0x00BB  , 0x00                        ;  ref_mon_tl_mask_2N          "},
        {"X , 0x00BC  , 0x00                        ;  ref_mon_th_mask_3P          "},
        {"X , 0x00BD  , 0x00                        ;  ref_mon_tl_mask_3P          "},
        {"X , 0x00BE  , 0x00                        ;  ref_mon_th_mask_3N          "},
        {"X , 0x00BF  , 0x00                        ;  ref_mon_tl_mask_3N          "},
        {"X , 0x00C0  , 0x00                        ;  ref_mon_th_mask_4P          "},
        {"X , 0x00C1  , 0x00                        ;  ref_mon_tl_mask_4P          "},
        {"X , 0x00C2  , 0x00                        ;  ref_mon_th_mask_4N          "},
        {"X , 0x00C3  , 0x00                        ;  ref_mon_tl_mask_4N          "},
        {"X , 0x00D0  , 0x00                        ;  dpll_mon_th_mask_0          "},
        {"X , 0x00D1  , 0x00                        ;  dpll_mon_tl_mask_0          "},
        {"X , 0x00D2  , 0x00                        ;  dpll_mon_th_mask_1          "},
        {"X , 0x00D3  , 0x00                        ;  dpll_mon_tl_mask_1          "},
        {"X , 0x00D4  , 0x00                        ;  dpll_mon_th_mask_2          "},
        {"X , 0x00D5  , 0x00                        ;  dpll_mon_tl_mask_2          "},
        {"X , 0x00D6  , 0x00                        ;  dpll_mon_th_mask_3          "},
        {"X , 0x00D7  , 0x00                        ;  dpll_mon_tl_mask_3          "},
        {"X , 0x00D8  , 0x00                        ;  dpll_mon_th_mask_4          "},
        {"X , 0x00D9  , 0x00                        ;  dpll_mon_tl_mask_4          "},
        {"X , 0x00DA  , 0x00                        ;  dpll_mon_th_mask_5          "},
        {"X , 0x00DB  , 0x00                        ;  dpll_mon_tl_mask_5          "},
        {"X , 0x00E0  , 0x00                        ;  gp_mon_th_mask              "},
        {"X , 0x00E1  , 0x00                        ;  gp_mon_tl_mask              "},
        {"X , 0x00E2  , 0x00                        ;  hp_mon_th_mask_1            "},
        {"X , 0x00E3  , 0x00                        ;  hp_mon_tl_mask_1            "},
        {"X , 0x00E4  , 0x00                        ;  hp_mon_th_mask_2            "},
        {"X , 0x00E5  , 0x00                        ;  hp_mon_tl_mask_2            "},
        {"X , 0x00E6  , 0x00                        ;  hp_out_th_mask_0            "},
        {"X , 0x00E7  , 0x00                        ;  hp_out_tl_mask_0            "},
        {"X , 0x00E8  , 0x00                        ;  hp_out_th_mask_1            "},
        {"X , 0x00E9  , 0x00                        ;  hp_out_tl_mask_1            "},
        {"X , 0x00EA  , 0x00                        ;  hp_out_th_mask_2            "},
        {"X , 0x00EB  , 0x00                        ;  hp_out_tl_mask_2            "},
        {"X , 0x00EC  , 0x00                        ;  hp_out_th_mask_3            "},
        {"X , 0x00ED  , 0x00                        ;  hp_out_tl_mask_3            "},
        {"X , 0x00EE  , 0x00                        ;  hp_out_th_mask_4            "},
        {"X , 0x00EF  , 0x00                        ;  hp_out_tl_mask_4            "},
        {"X , 0x00F0  , 0x00                        ;  hp_out_th_mask_5            "},
        {"X , 0x00F1  , 0x00                        ;  hp_out_tl_mask_5            "},
        {"X , 0x00F2  , 0x00                        ;  hp_out_th_mask_6            "},
        {"X , 0x00F3  , 0x00                        ;  hp_out_tl_mask_6            "},
        {"X , 0x00F4  , 0x00                        ;  hp_out_th_mask_7            "},
        {"X , 0x00F5  , 0x00                        ;  hp_out_tl_mask_7            "},
        {"X , 0x0200  , 0x00                        ;  ref_los_3_0                 "},
        {"X , 0x0201  , 0x00                        ;  ref_los_4                   "},
        {"X , 0x0208  , 0x03                        ;  dpll_enable                 "},
        {"X , 0x020A  , 0x88                        ;  split_xo_refsel             "},
        {"X , 0x020B  , 0x00                        ;  ext_fb_ctrl                 "},
        {"X , 0x020C  , 0x00                        ;  ext_fb_sel                  "},
        {"X , 0x0211  , 0x02                        ;  dpll_ctrl_0                 "},
        {"X , 0x0212  , 0x00                        ;  dpll_cmd_0                  "},
        {"X , 0x0215  , 0x02                        ;  dpll_ctrl_1                 "},
        {"X , 0x0216  , 0x00                        ;  dpll_cmd_1                  "},
        {"X , 0x0219  , 0x02                        ;  dpll_ctrl_2                 "},
        {"X , 0x021A  , 0x00                        ;  dpll_cmd_2                  "},
        {"X , 0x021D  , 0x00                        ;  dpll_ctrl_3                 "},
        {"X , 0x021E  , 0x00                        ;  dpll_cmd_3                  "},
        {"X , 0x0221  , 0x00                        ;  dpll_ctrl_4                 "},
        {"X , 0x0222  , 0x00                        ;  dpll_cmd_4                  "},
        {"X , 0x0225  , 0x00                        ;  dpll_ctrl_5                 "},
        {"X , 0x0226  , 0x00                        ;  dpll_cmd_5                  "},
        {"X , 0x0234  , 0x00                        ;  phase_step_data             "},
        {"X , 0x0235  , 0x00                        ;  phase_step_data             "},
        {"X , 0x0236  , 0x00                        ;  phase_step_data             "},
        {"X , 0x0237  , 0x00                        ;  phase_step_data             "},
        {"X , 0x0238  , 0x00                        ;  phase_step_mask_gp          "},
        {"X , 0x0239  , 0x00                        ;  phase_step_mask_hp          "},
        {"X , 0x023A  , 0x00                        ;  step_time_mask_gp           "},
        {"X , 0x023E  , 0x31                        ;  phase_step_max              "},
        {"X , 0x023F  , 0x14                        ;  phase_step_margin           "},
        {"X , 0x0250  , 0x00                        ;  pherr_meas_ctrl             "},
        {"X , 0x0251  , 0x98                        ;  pherr_meas_refsel           "},
        {"X , 0x0252  , 0x00                        ;  pherr_data                  "},
        {"X , 0x0253  , 0x00                        ;  pherr_data                  "},
        {"X , 0x0254  , 0x00                        ;  pherr_data                  "},
        {"X , 0x0255  , 0x00                        ;  pherr_data                  "},
        {"X , 0x0256  , 0x00                        ;  pherr_data                  "},
        {"X , 0x0257  , 0x00                        ;  pherr_data                  "},
        {"X , 0x025F  , 0x00                        ;  pherr_read_rqst             "},
        {"X , 0x0260  , 0x00                        ;  dpll_phase_err_read_mask    "},
        {"X , 0x0261  , 0x00                        ;  dpll_phase_err_data_0       "},
        {"X , 0x0262  , 0x00                        ;  dpll_phase_err_data_0       "},
        {"X , 0x0263  , 0x00                        ;  dpll_phase_err_data_0       "},
        {"X , 0x0264  , 0x00                        ;  dpll_phase_err_data_0       "},
        {"X , 0x0265  , 0x00                        ;  dpll_phase_err_data_0       "},
        {"X , 0x0266  , 0x00                        ;  dpll_phase_err_data_0       "},
        {"X , 0x0267  , 0x00                        ;  dpll_phase_err_data_1       "},
        {"X , 0x0268  , 0x00                        ;  dpll_phase_err_data_1       "},
        {"X , 0x0269  , 0x00                        ;  dpll_phase_err_data_1       "},
        {"X , 0x026A  , 0x00                        ;  dpll_phase_err_data_1       "},
        {"X , 0x026B  , 0x00                        ;  dpll_phase_err_data_1       "},
        {"X , 0x026C  , 0x00                        ;  dpll_phase_err_data_1       "},
        {"X , 0x026D  , 0x00                        ;  dpll_phase_err_data_2       "},
        {"X , 0x026E  , 0x00                        ;  dpll_phase_err_data_2       "},
        {"X , 0x026F  , 0x00                        ;  dpll_phase_err_data_2       "},
        {"X , 0x0270  , 0x00                        ;  dpll_phase_err_data_2       "},
        {"X , 0x0271  , 0x00                        ;  dpll_phase_err_data_2       "},
        {"X , 0x0272  , 0x00                        ;  dpll_phase_err_data_2       "},
        {"X , 0x0273  , 0x00                        ;  dpll_phase_err_data_3       "},
        {"X , 0x0274  , 0x00                        ;  dpll_phase_err_data_3       "},
        {"X , 0x0275  , 0x00                        ;  dpll_phase_err_data_3       "},
        {"X , 0x0276  , 0x00                        ;  dpll_phase_err_data_3       "},
        {"X , 0x0277  , 0x00                        ;  dpll_phase_err_data_3       "},
        {"X , 0x0278  , 0x00                        ;  dpll_phase_err_data_3       "},
        {"X , 0x0300  , 0x00                        ;  dpll_df_offset_0            "},
        {"X , 0x0301  , 0x00                        ;  dpll_df_offset_0            "},
        {"X , 0x0302  , 0x00                        ;  dpll_df_offset_0            "},
        {"X , 0x0303  , 0x00                        ;  dpll_df_offset_0            "},
        {"X , 0x0304  , 0x00                        ;  dpll_df_offset_0            "},
        {"X , 0x0305  , 0x00                        ;  dpll_df_offset_0            "},
        {"X , 0x0306  , 0x00                        ;  dpll_df_ctrl_0              "},
        {"X , 0x0307  , 0x00                        ;  dpll_df_manual_0            "},
        {"X , 0x0308  , 0x00                        ;  dpll_df_manual_0            "},
        {"X , 0x0309  , 0x00                        ;  dpll_df_manual_0            "},
        {"X , 0x030A  , 0x00                        ;  dpll_df_manual_0            "},
        {"X , 0x030B  , 0x00                        ;  dpll_df_manual_0            "},
        {"X , 0x0313  , 0x00                        ;  dpll_tie_data_0             "},
        {"X , 0x0314  , 0x00                        ;  dpll_tie_data_0             "},
        {"X , 0x0315  , 0x00                        ;  dpll_tie_data_0             "},
        {"X , 0x0316  , 0x00                        ;  dpll_tie_data_0             "},
        {"X , 0x0317  , 0x00                        ;  dpll_tie_ctrl_0             "},
        {"X , 0x0320  , 0x00                        ;  dpll_df_offset_1            "},
        {"X , 0x0321  , 0x00                        ;  dpll_df_offset_1            "},
        {"X , 0x0322  , 0x00                        ;  dpll_df_offset_1            "},
        {"X , 0x0323  , 0x00                        ;  dpll_df_offset_1            "},
        {"X , 0x0324  , 0x00                        ;  dpll_df_offset_1            "},
        {"X , 0x0325  , 0x00                        ;  dpll_df_offset_1            "},
        {"X , 0x0326  , 0x00                        ;  dpll_df_ctrl_1              "},
        {"X , 0x0327  , 0x00                        ;  dpll_df_manual_1            "},
        {"X , 0x0328  , 0x00                        ;  dpll_df_manual_1            "},
        {"X , 0x0329  , 0x00                        ;  dpll_df_manual_1            "},
        {"X , 0x032A  , 0x00                        ;  dpll_df_manual_1            "},
        {"X , 0x032B  , 0x00                        ;  dpll_df_manual_1            "},
        {"X , 0x0333  , 0x00                        ;  dpll_tie_data_1             "},
        {"X , 0x0334  , 0x00                        ;  dpll_tie_data_1             "},
        {"X , 0x0335  , 0x00                        ;  dpll_tie_data_1             "},
        {"X , 0x0336  , 0x00                        ;  dpll_tie_data_1             "},
        {"X , 0x0337  , 0x00                        ;  dpll_tie_ctrl_1             "},
        {"X , 0x0340  , 0x00                        ;  dpll_df_offset_2            "},
        {"X , 0x0341  , 0x00                        ;  dpll_df_offset_2            "},
        {"X , 0x0342  , 0x00                        ;  dpll_df_offset_2            "},
        {"X , 0x0343  , 0x00                        ;  dpll_df_offset_2            "},
        {"X , 0x0344  , 0x00                        ;  dpll_df_offset_2            "},
        {"X , 0x0345  , 0x00                        ;  dpll_df_offset_2            "},
        {"X , 0x0346  , 0x00                        ;  dpll_df_ctrl_2              "},
        {"X , 0x0347  , 0x00                        ;  dpll_df_manual_2            "},
        {"X , 0x0348  , 0x00                        ;  dpll_df_manual_2            "},
        {"X , 0x0349  , 0x00                        ;  dpll_df_manual_2            "},
        {"X , 0x034A  , 0x00                        ;  dpll_df_manual_2            "},
        {"X , 0x034B  , 0x00                        ;  dpll_df_manual_2            "},
        {"X , 0x0353  , 0x00                        ;  dpll_tie_data_2             "},
        {"X , 0x0354  , 0x00                        ;  dpll_tie_data_2             "},
        {"X , 0x0355  , 0x00                        ;  dpll_tie_data_2             "},
        {"X , 0x0356  , 0x00                        ;  dpll_tie_data_2             "},
        {"X , 0x0357  , 0x00                        ;  dpll_tie_ctrl_2             "},
        {"X , 0x0380  , 0x00                        ;  dpll_tod_sec_0              "},
        {"X , 0x0381  , 0x00                        ;  dpll_tod_sec_0              "},
        {"X , 0x0382  , 0x00                        ;  dpll_tod_sec_0              "},
        {"X , 0x0383  , 0x00                        ;  dpll_tod_sec_0              "},
        {"X , 0x0384  , 0x00                        ;  dpll_tod_sec_0              "},
        {"X , 0x0385  , 0x00                        ;  dpll_tod_sec_0              "},
        {"X , 0x0386  , 0x00                        ;  dpll_tod_ns_0               "},
        {"X , 0x0387  , 0x00                        ;  dpll_tod_ns_0               "},
        {"X , 0x0388  , 0x00                        ;  dpll_tod_ns_0               "},
        {"X , 0x0389  , 0x00                        ;  dpll_tod_ns_0               "},
        {"X , 0x038A  , 0x00                        ;  dpll_tod_ctrl_0             "},
        {"X , 0x038C  , 0x00                        ;  dpll_tod_sec_1              "},
        {"X , 0x038D  , 0x00                        ;  dpll_tod_sec_1              "},
        {"X , 0x038E  , 0x00                        ;  dpll_tod_sec_1              "},
        {"X , 0x038F  , 0x00                        ;  dpll_tod_sec_1              "},
        {"X , 0x0390  , 0x00                        ;  dpll_tod_sec_1              "},
        {"X , 0x0391  , 0x00                        ;  dpll_tod_sec_1              "},
        {"X , 0x0392  , 0x00                        ;  dpll_tod_ns_1               "},
        {"X , 0x0393  , 0x00                        ;  dpll_tod_ns_1               "},
        {"X , 0x0394  , 0x00                        ;  dpll_tod_ns_1               "},
        {"X , 0x0395  , 0x00                        ;  dpll_tod_ns_1               "},
        {"X , 0x0396  , 0x00                        ;  dpll_tod_ctrl_1             "},
        {"X , 0x0398  , 0x00                        ;  dpll_tod_sec_2              "},
        {"X , 0x0399  , 0x00                        ;  dpll_tod_sec_2              "},
        {"X , 0x039A  , 0x00                        ;  dpll_tod_sec_2              "},
        {"X , 0x039B  , 0x00                        ;  dpll_tod_sec_2              "},
        {"X , 0x039C  , 0x00                        ;  dpll_tod_sec_2              "},
        {"X , 0x039D  , 0x00                        ;  dpll_tod_sec_2              "},
        {"X , 0x039E  , 0x00                        ;  dpll_tod_ns_2               "},
        {"X , 0x039F  , 0x00                        ;  dpll_tod_ns_2               "},
        {"X , 0x03A0  , 0x00                        ;  dpll_tod_ns_2               "},
        {"X , 0x03A1  , 0x00                        ;  dpll_tod_ns_2               "},
        {"X , 0x03A2  , 0x00                        ;  dpll_tod_ctrl_2             "},
        {"X , 0x0400  , 0x00                        ;  gp_ctrl                     "},
        {"X , 0x0401  , 0x00                        ;  gp_cmd                      "},
        {"X , 0x0404  , 0x1F                        ;  gp_freq_base                "},
        {"X , 0x0405  , 0x40                        ;  gp_freq_base                "},
        {"X , 0x0406  , 0x01                        ;  gp_freq_mult                "},
        {"X , 0x0407  , 0x7B                        ;  gp_freq_mult                "},
        {"X , 0x0408  , 0xB0                        ;  gp_freq_mult                "},
        {"X , 0x0409  , 0x00                        ;  gp_freq_m                   "},
        {"X , 0x040A  , 0x01                        ;  gp_freq_m                   "},
        {"X , 0x040B  , 0x00                        ;  gp_freq_n                   "},
        {"X , 0x040C  , 0x01                        ;  gp_freq_n                   "},
        {"X , 0x040D  , 0x00                        ;  gp_fine_shift               "},
        {"X , 0x040E  , 0x00                        ;  gp_fine_shift               "},
        {"X , 0x040F  , 0x01                        ;  gp_fine_shift_intvl         "},
        {"X , 0x0410  , 0x00                        ;  gp_df_offset_manual         "},
        {"X , 0x0411  , 0x00                        ;  gp_df_offset_manual         "},
        {"X , 0x0412  , 0x00                        ;  gp_df_offset_manual         "},
        {"X , 0x0413  , 0x00                        ;  gp_df_offset_manual         "},
        {"X , 0x0414  , 0x00                        ;  gp_df_offset_manual         "},
        {"X , 0x0415  , 0x00                        ;  gp_ctrl_int                 "},
        {"X , 0x0416  , 0x00                        ;  gp_dco_ratio                "},
        {"X , 0x0417  , 0x00                        ;  gp_dco_ratio                "},
        {"X , 0x0418  , 0x00                        ;  gp_dco_ratio                "},
        {"X , 0x0419  , 0x00                        ;  gp_dco_ratio                "},
        {"X , 0x041A  , 0x00                        ;  gp_frl                      "},
        {"X , 0x041B  , 0x00                        ;  gp_frl                      "},
        {"X , 0x041C  , 0x00                        ;  gp_fdl                      "},
        {"X , 0x041D  , 0x00                        ;  gp_fdl                      "},
        {"X , 0x041E  , 0x00                        ;  gp_lockmon                  "},
        {"X , 0x041F  , 0x00                        ;  gp_lockmon                  "},
        {"X , 0x0420  , 0x00                        ;  gp_out_ctrl_0               "},
        {"X , 0x0422  , 0x00                        ;  gp_out_div_0                "},
        {"X , 0x0423  , 0x00                        ;  gp_out_div_0                "},
        {"X , 0x0424  , 0x00                        ;  gp_out_div_0                "},
        {"X , 0x0425  , 0x05                        ;  gp_out_div_0                "},
        {"X , 0x0426  , 0x00                        ;  gp_out_width_0              "},
        {"X , 0x0427  , 0x00                        ;  gp_out_width_0              "},
        {"X , 0x0428  , 0x00                        ;  gp_out_width_0              "},
        {"X , 0x0429  , 0x00                        ;  gp_out_width_0              "},
        {"X , 0x042C  , 0x00                        ;  gp_out_shift_0              "},
        {"X , 0x042D  , 0x00                        ;  gp_out_shift_0              "},
        {"X , 0x042E  , 0x00                        ;  gp_out_shift_0              "},
        {"X , 0x042F  , 0x00                        ;  gp_out_shift_0              "},
        {"X , 0x0430  , 0x00                        ;  gp_out_ctrl_1               "},
        {"X , 0x0432  , 0x00                        ;  gp_out_div_1                "},
        {"X , 0x0433  , 0x00                        ;  gp_out_div_1                "},
        {"X , 0x0434  , 0x00                        ;  gp_out_div_1                "},
        {"X , 0x0435  , 0x28                        ;  gp_out_div_1                "},
        {"X , 0x0436  , 0x00                        ;  gp_out_width_1              "},
        {"X , 0x0437  , 0x00                        ;  gp_out_width_1              "},
        {"X , 0x0438  , 0x00                        ;  gp_out_width_1              "},
        {"X , 0x0439  , 0x00                        ;  gp_out_width_1              "},
        {"X , 0x043C  , 0x00                        ;  gp_out_shift_1              "},
        {"X , 0x043D  , 0x00                        ;  gp_out_shift_1              "},
        {"X , 0x043E  , 0x00                        ;  gp_out_shift_1              "},
        {"X , 0x043F  , 0x00                        ;  gp_out_shift_1              "},
        {"X , 0x0450  , 0x00                        ;  gp_out_en                   "},
        {"X , 0x0451  , 0x0F                        ;  gp_out_drive                "},
        {"X , 0x0481  , 0x03                        ;  hp_src_1                    "},
        {"X , 0x0484  , 0xDF                        ;  hp_freq_base_1              "},
        {"X , 0x0485  , 0x84                        ;  hp_freq_base_1              "},
        {"X , 0x0486  , 0x75                        ;  hp_freq_base_1              "},
        {"X , 0x0487  , 0x80                        ;  hp_freq_base_1              "},
        {"X , 0x0488  , 0x00                        ;  hp_freq_m_1                 "},
        {"X , 0x0489  , 0x00                        ;  hp_freq_m_1                 "},
        {"X , 0x048A  , 0x00                        ;  hp_freq_m_1                 "},
        {"X , 0x048B  , 0x01                        ;  hp_freq_m_1                 "},
        {"X , 0x048C  , 0x00                        ;  hp_freq_n_1                 "},
        {"X , 0x048D  , 0x00                        ;  hp_freq_n_1                 "},
        {"X , 0x048E  , 0x00                        ;  hp_freq_n_1                 "},
        {"X , 0x048F  , 0x01                        ;  hp_freq_n_1                 "},
        {"X , 0x0490  , 0x02                        ;  hp_hsdiv_1                  "},
        {"X , 0x0491  , 0x0B                        ;  hp_fdiv_base_1              "},
        {"X , 0x0492  , 0xEB                        ;  hp_fdiv_base_1              "},
        {"X , 0x0493  , 0xC2                        ;  hp_fdiv_base_1              "},
        {"X , 0x0494  , 0x00                        ;  hp_fdiv_base_1              "},
        {"X , 0x0495  , 0x00                        ;  hp_fdiv_num_1               "},
        {"X , 0x0496  , 0x00                        ;  hp_fdiv_num_1               "},
        {"X , 0x0497  , 0x00                        ;  hp_fdiv_num_1               "},
        {"X , 0x0498  , 0x01                        ;  hp_fdiv_num_1               "},
        {"X , 0x0499  , 0x00                        ;  hp_fdiv_den_1               "},
        {"X , 0x049A  , 0x00                        ;  hp_fdiv_den_1               "},
        {"X , 0x049B  , 0x00                        ;  hp_fdiv_den_1               "},
        {"X , 0x049C  , 0x01                        ;  hp_fdiv_den_1               "},
        {"X , 0x049D  , 0x00                        ;  hp_sprd_ctrl_1              "},
        {"X , 0x049E  , 0x00                        ;  hp_sprd_cnt_1               "},
        {"X , 0x049F  , 0x00                        ;  hp_sprd_cnt_1               "},
        {"X , 0x04A0  , 0x00                        ;  hp_sprd_incdec_1            "},
        {"X , 0x04A1  , 0x00                        ;  hp_sprd_incdec_1            "},
        {"X , 0x04A2  , 0x00                        ;  hp_sprd_incdec_1            "},
        {"X , 0x04A3  , 0x00                        ;  hp_sprd_incdec_1            "},
        {"X , 0x04A4  , 0x00                        ;  hp_fine_shift_1             "},
        {"X , 0x04A5  , 0x00                        ;  hp_fine_shift_1             "},
        {"X , 0x04A6  , 0x00                        ;  hp_fine_shift_1             "},
        {"X , 0x04A7  , 0x00                        ;  hp_fine_shift_1             "},
        {"X , 0x04B1  , 0x03                        ;  hp_src_2                    "},
        {"X , 0x04B4  , 0xDF                        ;  hp_freq_base_2              "},
        {"X , 0x04B5  , 0x84                        ;  hp_freq_base_2              "},
        {"X , 0x04B6  , 0x75                        ;  hp_freq_base_2              "},
        {"X , 0x04B7  , 0x80                        ;  hp_freq_base_2              "},
        {"X , 0x04B8  , 0x00                        ;  hp_freq_m_2                 "},
        {"X , 0x04B9  , 0x00                        ;  hp_freq_m_2                 "},
        {"X , 0x04BA  , 0x00                        ;  hp_freq_m_2                 "},
        {"X , 0x04BB  , 0x01                        ;  hp_freq_m_2                 "},
        {"X , 0x04BC  , 0x00                        ;  hp_freq_n_2                 "},
        {"X , 0x04BD  , 0x00                        ;  hp_freq_n_2                 "},
        {"X , 0x04BE  , 0x00                        ;  hp_freq_n_2                 "},
        {"X , 0x04BF  , 0x01                        ;  hp_freq_n_2                 "},
        {"X , 0x04C0  , 0x02                        ;  hp_hsdiv_2                  "},
        {"X , 0x04C1  , 0x0B                        ;  hp_fdiv_base_2              "},
        {"X , 0x04C2  , 0xEB                        ;  hp_fdiv_base_2              "},
        {"X , 0x04C3  , 0xC2                        ;  hp_fdiv_base_2              "},
        {"X , 0x04C4  , 0x00                        ;  hp_fdiv_base_2              "},
        {"X , 0x04C5  , 0x00                        ;  hp_fdiv_num_2               "},
        {"X , 0x04C6  , 0x00                        ;  hp_fdiv_num_2               "},
        {"X , 0x04C7  , 0x00                        ;  hp_fdiv_num_2               "},
        {"X , 0x04C8  , 0x05                        ;  hp_fdiv_num_2               "},
        {"X , 0x04C9  , 0x00                        ;  hp_fdiv_den_2               "},
        {"X , 0x04CA  , 0x00                        ;  hp_fdiv_den_2               "},
        {"X , 0x04CB  , 0x00                        ;  hp_fdiv_den_2               "},
        {"X , 0x04CC  , 0x04                        ;  hp_fdiv_den_2               "},
        {"X , 0x04CD  , 0x00                        ;  hp_sprd_ctrl_2              "},
        {"X , 0x04CE  , 0x00                        ;  hp_sprd_cnt_2               "},
        {"X , 0x04CF  , 0x00                        ;  hp_sprd_cnt_2               "},
        {"X , 0x04D0  , 0x00                        ;  hp_sprd_incdec_2            "},
        {"X , 0x04D1  , 0x00                        ;  hp_sprd_incdec_2            "},
        {"X , 0x04D2  , 0x00                        ;  hp_sprd_incdec_2            "},
        {"X , 0x04D3  , 0x00                        ;  hp_sprd_incdec_2            "},
        {"X , 0x04D4  , 0x00                        ;  hp_fine_shift_2             "},
        {"X , 0x04D5  , 0x00                        ;  hp_fine_shift_2             "},
        {"X , 0x04D6  , 0x00                        ;  hp_fine_shift_2             "},
        {"X , 0x04D7  , 0x00                        ;  hp_fine_shift_2             "},
        {"X , 0x04E0  , 0xFF                        ;  hp_out_en                   "},
        {"X , 0x04E1  , 0x00                        ;  hp_out_mux                  "},
        {"X , 0x04E2  , 0x00                        ;  hp_stop_ctrl                "},
        {"X , 0x04E3  , 0x00                        ;  hp_stopall_ctrl             "},
        {"X , 0x0500  , 0x4B                        ;  hp_out_msdiv_0              "},
        {"X , 0x0501  , 0x00                        ;  hp_out_lsdiv_0              "},
        {"X , 0x0502  , 0x98                        ;  hp_out_lsdiv_0              "},
        {"X , 0x0503  , 0x96                        ;  hp_out_lsdiv_0              "},
        {"X , 0x0504  , 0x80                        ;  hp_out_lsdiv_0              "},
        {"X , 0x0505  , 0x04                        ;  hp_out_ctrl_0               "},
        {"X , 0x0506  , 0x05                        ;  hp_out_diff_0               "},
        {"X , 0x0507  , 0x00                        ;  hp_out_reg_0                "},
        {"X , 0x0508  , 0x20                        ;  hp_out_lsctrl_0             "},
        {"X , 0x0509  , 0x00                        ;  hp_out_width_0              "},
        {"X , 0x050A  , 0x00                        ;  hp_out_shift_0              "},
        {"X , 0x050B  , 0x0C                        ;  hp_out_stop_0               "},
        {"X , 0x0510  , 0x00                        ;  hp_out_msdiv_1              "},
        {"X , 0x0511  , 0x00                        ;  hp_out_lsdiv_1              "},
        {"X , 0x0512  , 0x00                        ;  hp_out_lsdiv_1              "},
        {"X , 0x0513  , 0x00                        ;  hp_out_lsdiv_1              "},
        {"X , 0x0514  , 0x00                        ;  hp_out_lsdiv_1              "},
        {"X , 0x0515  , 0x00                        ;  hp_out_ctrl_1               "},
        {"X , 0x0516  , 0x05                        ;  hp_out_diff_1               "},
        {"X , 0x0517  , 0x00                        ;  hp_out_reg_1                "},
        {"X , 0x0518  , 0x00                        ;  hp_out_lsctrl_1             "},
        {"X , 0x0519  , 0x00                        ;  hp_out_width_1              "},
        {"X , 0x051A  , 0x00                        ;  hp_out_shift_1              "},
        {"X , 0x051B  , 0x0C                        ;  hp_out_stop_1               "},
        {"X , 0x0520  , 0x4B                        ;  hp_out_msdiv_2              "},
        {"X , 0x0521  , 0x00                        ;  hp_out_lsdiv_2              "},
        {"X , 0x0522  , 0x98                        ;  hp_out_lsdiv_2              "},
        {"X , 0x0523  , 0x96                        ;  hp_out_lsdiv_2              "},
        {"X , 0x0524  , 0x80                        ;  hp_out_lsdiv_2              "},
        {"X , 0x0525  , 0x04                        ;  hp_out_ctrl_2               "},
        {"X , 0x0526  , 0x05                        ;  hp_out_diff_2               "},
        {"X , 0x0527  , 0x00                        ;  hp_out_reg_2                "},
        {"X , 0x0528  , 0x20                        ;  hp_out_lsctrl_2             "},
        {"X , 0x0529  , 0x00                        ;  hp_out_width_2              "},
        {"X , 0x052A  , 0x00                        ;  hp_out_shift_2              "},
        {"X , 0x052B  , 0x0C                        ;  hp_out_stop_2               "},
        {"X , 0x0530  , 0x00                        ;  hp_out_msdiv_3              "},
        {"X , 0x0531  , 0x00                        ;  hp_out_lsdiv_3              "},
        {"X , 0x0532  , 0x00                        ;  hp_out_lsdiv_3              "},
        {"X , 0x0533  , 0x00                        ;  hp_out_lsdiv_3              "},
        {"X , 0x0534  , 0x00                        ;  hp_out_lsdiv_3              "},
        {"X , 0x0535  , 0x00                        ;  hp_out_ctrl_3               "},
        {"X , 0x0536  , 0x05                        ;  hp_out_diff_3               "},
        {"X , 0x0537  , 0x00                        ;  hp_out_reg_3                "},
        {"X , 0x0538  , 0x00                        ;  hp_out_lsctrl_3             "},
        {"X , 0x0539  , 0x00                        ;  hp_out_width_3              "},
        {"X , 0x053A  , 0x00                        ;  hp_out_shift_3              "},
        {"X , 0x053B  , 0x0C                        ;  hp_out_stop_3               "},
        {"X , 0x0540  , 0x03                        ;  hp_out_msdiv_4              "},
        {"X , 0x0541  , 0x00                        ;  hp_out_lsdiv_4              "},
        {"X , 0x0542  , 0x00                        ;  hp_out_lsdiv_4              "},
        {"X , 0x0543  , 0x00                        ;  hp_out_lsdiv_4              "},
        {"X , 0x0544  , 0x00                        ;  hp_out_lsdiv_4              "},
        {"X , 0x0545  , 0x02                        ;  hp_out_ctrl_4               "},
        {"X , 0x0546  , 0x05                        ;  hp_out_diff_4               "},
        {"X , 0x0547  , 0x00                        ;  hp_out_reg_4                "},
        {"X , 0x0548  , 0x00                        ;  hp_out_lsctrl_4             "},
        {"X , 0x0549  , 0x00                        ;  hp_out_width_4              "},
        {"X , 0x054A  , 0x00                        ;  hp_out_shift_4              "},
        {"X , 0x054B  , 0x0C                        ;  hp_out_stop_4               "},
        {"X , 0x0550  , 0x00                        ;  hp_out_msdiv_5              "},
        {"X , 0x0551  , 0x00                        ;  hp_out_lsdiv_5              "},
        {"X , 0x0552  , 0x00                        ;  hp_out_lsdiv_5              "},
        {"X , 0x0553  , 0x00                        ;  hp_out_lsdiv_5              "},
        {"X , 0x0554  , 0x00                        ;  hp_out_lsdiv_5              "},
        {"X , 0x0555  , 0x00                        ;  hp_out_ctrl_5               "},
        {"X , 0x0556  , 0x05                        ;  hp_out_diff_5               "},
        {"X , 0x0557  , 0x00                        ;  hp_out_reg_5                "},
        {"X , 0x0558  , 0x00                        ;  hp_out_lsctrl_5             "},
        {"X , 0x0559  , 0x00                        ;  hp_out_width_5              "},
        {"X , 0x055A  , 0x00                        ;  hp_out_shift_5              "},
        {"X , 0x055B  , 0x0C                        ;  hp_out_stop_5               "},
        {"X , 0x0560  , 0x32                        ;  hp_out_msdiv_6              "},
        {"X , 0x0561  , 0x00                        ;  hp_out_lsdiv_6              "},
        {"X , 0x0562  , 0xE4                        ;  hp_out_lsdiv_6              "},
        {"X , 0x0563  , 0xE1                        ;  hp_out_lsdiv_6              "},
        {"X , 0x0564  , 0xC0                        ;  hp_out_lsdiv_6              "},
        {"X , 0x0565  , 0x04                        ;  hp_out_ctrl_6               "},
        {"X , 0x0566  , 0x05                        ;  hp_out_diff_6               "},
        {"X , 0x0567  , 0x00                        ;  hp_out_reg_6                "},
        {"X , 0x0568  , 0x10                        ;  hp_out_lsctrl_6             "},
        {"X , 0x0569  , 0x00                        ;  hp_out_width_6              "},
        {"X , 0x056A  , 0x00                        ;  hp_out_shift_6              "},
        {"X , 0x056B  , 0x0C                        ;  hp_out_stop_6               "},
        {"X , 0x0570  , 0x00                        ;  hp_out_msdiv_7              "},
        {"X , 0x0571  , 0x00                        ;  hp_out_lsdiv_7              "},
        {"X , 0x0572  , 0x00                        ;  hp_out_lsdiv_7              "},
        {"X , 0x0573  , 0x00                        ;  hp_out_lsdiv_7              "},
        {"X , 0x0574  , 0x00                        ;  hp_out_lsdiv_7              "},
        {"X , 0x0575  , 0x00                        ;  hp_out_ctrl_7               "},
        {"X , 0x0576  , 0x05                        ;  hp_out_diff_7               "},
        {"X , 0x0577  , 0x00                        ;  hp_out_reg_7                "},
        {"X , 0x0578  , 0x00                        ;  hp_out_lsctrl_7             "},
        {"X , 0x0579  , 0x00                        ;  hp_out_width_7              "},
        {"X , 0x057A  , 0x00                        ;  hp_out_shift_7              "},
        {"X , 0x057B  , 0x0C                        ;  hp_out_stop_7               "},
        {"X , 0x0700  , 0x00                        ;  gp_align_query              "},
        {"X , 0x0701  , 0x00                        ;  gp_align_data               "},
        {"X , 0x0702  , 0x00                        ;  gp_align_data               "},
        {"X , 0x0703  , 0x00                        ;  gp_align_data               "},
        {"X , 0x0704  , 0x00                        ;  gp_align_ofst               "},
        {"X , 0x0705  , 0x00                        ;  gp_align_ofst               "},
        {"X , 0x0706  , 0x00                        ;  psrg_ctrl                   "},
        {"X , 0x0480  , 0x11                        ;  hp_ctrl_1                   "},
        {"X , 0x04B0  , 0x20                        ;  hp_ctrl_2                   "},

        {"W , 2000000 "},
        {"X , 0x0210  , 0x00                        ;  dpll_mode_refsel_0          "},
        {"X , 0x0214  , 0x52                        ;  dpll_mode_refsel_1          "},
        {"X , 0x0218  , 0x90                        ;  dpll_mode_refsel_2          "},
        {"X , 0x021C  , 0x00                        ;  dpll_mode_refsel_3          "},
        {"X , 0x0220  , 0x00                        ;  dpll_mode_refsel_4          "},
        {"X , 0x0224  , 0x00                        ;  dpll_mode_refsel_5          "},
        {"X , 0x0209  , 0x02                        ;  split_xo_ctrl               "},
        {"W , 2000000 "},

        {"; Register Configuration End"},
        {"; Register Write Count = 1211"},

        {";======================================================================"},
    }
};


/* example77xLoadConfigDefaults */
/*
   Parses and loads the ZLS3077X using a static structure.

  Parameters:
   [in]   zl303xx_Params   Pointer to the device instance parameter structure.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE example77xLoadConfigDefaults(zl303xx_ParamsS *zl303xx_Params)
{
    zlStatusE status = ZL303XX_OK;


    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(zl303xx_Params);
    }
    if (status == ZL303XX_OK)
    {
        status = example77xLoadConfigStruct( zl303xx_Params, &example77xConfigData );
    }

    return status;
}



/* CANNED EXAMPLES */

/**

  Function Name:
   example77xSlave

  Details:
   Configures the system for slave mode operation

  Parameters:
        None

  Return Value:
   zlStatusE

 *****************************************************************************/
#if defined ZLS30771_INCLUDED && !(defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD)
zlStatusE example77x(void)
#else
zlStatusE example77xSlave(void)
#endif
{
   zlStatusE status = ZL303XX_OK;
#if defined ZLS30390_INCLUDED
   const char *srcIpv4Addr = "10.0.0.101";
   const char *dstIpv4Addr = "10.0.0.230";
#endif
   Uint32T numClocks = 4;

   /* Set the default clock class, accuracy, and slaveOnly */
#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
   zl303xx_SetExampleInterface(1);
   zl303xx_SetUseUniNegFlag(ZL303XX_TRUE);
   zl303xx_SetDefaultClockClass(PTP_SLAVE_ONLY_CLOCK_CLASS);
   zl303xx_SetDefaultClockAccuracy(ZL303XX_CLOCK_ACCURACY_WITHIN_1s);
#endif

#if defined APR_INCLUDED
   if (status == ZL303XX_OK)
   {
      /* Setup global configuration options */
      status |= zl303xx_AprSetPktRate(64, ZL303XX_TRUE);
      status |= zl303xx_AprSetPktRate(16, ZL303XX_FALSE);

      status |= exampleAprSetConfigParameters(ACI_DEFAULT );
   }
#endif

   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
       /* setup with numClocks clocks, 1 port, 1 streams (servers) */
       status = exampleAppStructInit(numClocks, 1, 1, &zlExampleApp);
   }

#if defined ZLS30390_INCLUDED
   if (status == ZL303XX_OK)
   {
       status = examplePtpLocalIpv4Set(srcIpv4Addr, ZL303XX_FALSE, &zlExampleApp.port[0].ptp);
       if (status == ZL303XX_OK)
       {
          status = examplePtpDestIpv4Set(dstIpv4Addr, ZL303XX_FALSE, &zlExampleApp.stream[0].ptp);
       }
   #if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
       setExampleIpSrc(srcIpv4Addr);

       if (zl303xx_GetUseUniNegFlag())
       {
           zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_TRUE;
       }
       zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = zl303xx_GetDefaultClockClass();
       zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockAccuracy = zl303xx_GetDefaultClockAccuracy();
       zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_TRUE;
   #else
       /* #warning MSCC: Set Source IPV4 address on port here */
       zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = PTP_MASTER_ONLY_CLOCK_CLASS_MAX;
       zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockAccuracy = ZL303XX_CLOCK_ACCURACY_WITHIN_1s;
       zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
   #endif
   }
#endif

#if defined ZLS30390_INCLUDED
   zlExampleApp.stream[0].ptp.config.unicast = ZL303XX_TRUE;
#endif

   if (status == ZL303XX_OK)
   {
      /* Set all CGUs to ToP mode */
      /* Set all DPLLs to start in AUTO with REF_AUTO */
      if (status == ZL303XX_OK)
      {
         Uint32T count;

         for (count = 0; count < numClocks; count++)
         {
            zlExampleApp.clock[count].pktRef = ZL303XX_TRUE;
            zlExampleApp.clock[count].zl3077x.deviceMode = ZL303XX_MODE_REF_TOP;
         }

         status = exampleAppStart(&zlExampleApp);

         for (count = 0; count < numClocks; count++)
         {
            zlExampleApp.clock[count].zl3077x.zl303xx_Params->pllParams.d77x.pllPriorMode = ZLS3077X_DPLL_MODE_AUTO_LOCK;
            zlExampleApp.clock[count].zl3077x.zl303xx_Params->pllParams.selectedRef = ZL303XX_REF_AUTO;
         }

      }
   }

   return status;
}



/*****************   END   ****************************************************/
