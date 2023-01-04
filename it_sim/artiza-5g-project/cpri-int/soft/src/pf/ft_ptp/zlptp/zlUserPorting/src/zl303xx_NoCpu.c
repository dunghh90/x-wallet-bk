

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This provides an abstraction for various functions provided by a CPU. The
*     aim is to make it easier to port a system to a different 'host'.
*
*     This file is the NoCpu implementation of the abstract interface defined
*     in zl303xx_Porting.h.
*
*******************************************************************************/
/* #warning MSCC: Using zl303xx_NoCpu.c because the CPU arch. specified has NO SUPPORTING FILES in zlUserPorting/src/! */

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Porting.h"
#include "zl303xx_AddressMap.h" /* from API directory */

#include <string.h>

/*****************   DEFINES   ************************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   IMPORTED FUNCTION DECLARATIONS   *************************/
extern zlStatusE cpuChipSelectFn(Uint32T par, zl303xx_BooleanE enable);

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/
/* #warning MSCC: These are all faked (just to get started) */

/* #warning MSCC: Must specify BSP-specific chip select mask with ZL303XX_CHIP_SELECT_MASK */
const Uint32T ZL303XX_CHIP_SELECT_MASK = 255;
const Uint32T CSB2_CHIP_SELECT_MASK  = 0;     
const Uint32T CSB3_CHIP_SELECT_MASK  = 0;    

/* #warning MSCC: Must specify BSP-specific HI and LO IRQs */
/* Device 0 interrupt 0 (high priority) is attached to CPU IRQ# */
const Uint32T ZL303XX_DEVICE_HI_PRI_CPU_IRQ = 0;
/* Device 0 interrupt 1 (lower priority) is attached to CPU IRQ# */
const Uint32T ZL303XX_DEVICE_LO_PRI_CPU_IRQ = 0;




/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* cpuConfigInterrupts */
/**
   Performs any configuration necessary on the CPU to allow external
   interrupts to be routed to the interrupt controller.

  Parameters:
      None

  Return Value:  cpuStatusE

*******************************************************************************/
cpuStatusE cpuConfigInterrupts(void)
{
   /* #warning MSCC: cpuConfigInterrupts() missing */
   return CPU_OK;
}

/* zl303xx_MaskInterruptFn */
/**
   Masks or unmasks the specified interrupt from the device. The method of doing
   this is system and/or processor specific.

  Parameters:
   [in]   zl303xx_Params  Pointer to the device specific parameters in case it
                             is required for identifying the specific device
                             (unused in this example).
   [in]   cpuIntNum     CPU interrupt number to mask off (unused in this
                             example).
   [in]   deviceIntNum  Device interrupt number to mask off. Values 0 or 1.
   [in]   bMasking      ZL303XX_TRUE if the interrupt is to be masked.
                             ZL303XX_FALSE to unmask the interrupt.

  Return Value:  None

*******************************************************************************/
void zl303xx_MaskInterruptFn(zl303xx_ParamsS *zl303xx_Params, Uint8T deviceIntNum,
                             Uint8T cpuIntNum, zl303xx_BooleanE bMasking)
{
   /* #warning MSCC: zl303xx_MaskInterruptFn() missing */

   /* Warning removal */
   (void)zl303xx_Params; (void)deviceIntNum; (void)cpuIntNum; (void)bMasking;
}

/* cpuConfigChipSelect */
/**
   Performs any configuration necessary on the CPU to handle the SPI chip select
   for the ZL303XX_ device.

  Parameters:
   [in]   zl303xx_Params  Pointer to the device specific parameters which
                             include the SPI chip select structure.
   [in]   csPin         Chip select pin number.

  Return Value:  cpuStatusE

*******************************************************************************/
cpuStatusE cpuConfigChipSelect(zl303xx_ParamsS * zl303xx_Params, Uint32T csPin)
{
    /* #warning MSCC: cpuConfigChipSelect() missing */

#if defined OS_VXWORKS
   zl303xx_Params->spiParams.chipSelect.csFuncPtr = cpuChipSelectFn;
   zl303xx_Params->spiParams.chipSelect.csFuncPar = csPin;
#elif defined OS_LINUX 
   (void)csPin;  /* Warning removal */
   strncpy((char *)zl303xx_Params->spiParams.linuxChipSelectDevName,   "NULL", sizeof(zl303xx_Params->spiParams.linuxChipSelectDevName));
   return CPU_OK;
#endif

   printf("MSCC: SHOULD NOT BE CALLING STUB cpuConfigChipSelect()\n");
   return CPU_ERROR;
}

/* cpuSoftwareResetFn */
/**
   Forces the ZL303XX_ reset pin low and then re-asserts it high. The requirement
   is for the pin to remain low for a minimum of 300ns. This example uses a more
   coarse delay in order to avoid using a nanosecond timer.

  Parameters:
   [in]   hwParams  Hardware parameter structure.

  Return Value:
   None

*******************************************************************************/
cpuStatusE cpuSoftwareResetFn(void *hwParams)
{
   /* #warning MSCC: cpuSoftwareResetFn() missing */

   (void)hwParams;  /* Warning removal */
   return CPU_ERROR;
}

/* cpuConfigSoftwareReset */
/**
   Performs any configuration necessary on the CPU to handle a software reset of
   the ZL303XX_ device.

  Parameters:
   [in]   hwParams  Hardware parameter structure.

  Return Value:  cpuStatusE

*******************************************************************************/
cpuStatusE cpuConfigSoftwareReset(void *hwParams)
{
   /* #warning MSCC: cpuConfigSoftwareReset() missing */

   (void)hwParams;  /* Warning removal */
   return CPU_ERROR;
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/*****************   END   ****************************************************/
