

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This provides an abstraction for various functions provided by a CPU. The
*     aim is to make it easier to port a system to a different 'host'
*
*     This file is the MPC8313 implementation of the abstract interface defined
*     in zl303xx_Porting.h
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_M8313.h"
#include "zl303xx_Porting.h"
#include "zl303xx_RdWr.h"

#include "zl303xx_Os.h"
#include "zl303xx_Error.h"
#include "zl303xx_LnxVariants.h"

#include <string.h>

/*****************   DEFINES     **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/


#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD

    extern zl303xx_ParamsS * zl303xx_Params0;
  #if defined _ZL303XX_ZLE1588_BOARD
    extern zl303xx_ParamsS * zl303xx_Params1;
    #warning MSCC: CHASSIS
    #if defined THREE_DEVICES_ON_DB
      extern zl303xx_ParamsS * zl303xx_Params2;
    #endif
  #endif

  #if defined OS_VXWORKS
      const Uint32T ZL303XX_CHIP_SELECT_MASK = (0x80000000 >> 0);
      const Uint32T APLL_CHIP_SELECT_MASK  = (0x80000000 >> 1);
  #endif

  #if defined OS_LINUX
      Uint8T devNameBase[32] = {"/dev/zl_spi"};   /* This is the start of the name## (e.g.,  /dev/zl_spi17) */
  #endif

  #if defined _ZL303XX_ZLE30360_BOARD
    #if defined OS_VXWORKS
      const Uint32T ZL303XX_DEVICE_HI_PRI_CPU_VECTOR = 0;   /* Unused on 36X */
      const Uint32T ZL303XX_DEVICE_LO_PRI_CPU_VECTOR = 0;   /* Unused on 36X */
    #endif

    #if defined OS_LINUX
      const Uint32T ZL303XX_CHIP_SELECT_MASK = 0;   /* GPIO 0 - the zl3036x */
    #endif
    const Uint32T ZL303XX_DEVICE_HI_PRI_CPU_IRQ = 0;   /* Unused on 36X */
    const Uint32T ZL303XX_DEVICE_LO_PRI_CPU_IRQ = 0;   /* Unused on 36X */

  #elif defined _ZL303XX_ZLE1588_BOARD
    #if defined OS_LINUX
      const Uint32T ZL303XX_CHIP_SELECT_MASK = 0;     /* GPIO 0 - ZL303XX_A = 1588   = Params0 */
      const Uint32T CSB2_CHIP_SELECT_MASK  = 1;     /* GPIO 1 - ZL303XX_B = SynchE = Params1 */
      const Uint32T CSB3_CHIP_SELECT_MASK  = 2;     /* GPIO 2 - ZL303XX_C = ?????? = Params2 */
    #else
     #error NO non-Linux support for ZLE1588 board configuration
    #endif
    const Uint32T ZL303XX_DEVICE_HI_PRI_CPU_IRQ = 0;   /* Unused */
    const Uint32T ZL303XX_DEVICE_LO_PRI_CPU_IRQ = 0;   /* Unused */

  #elif defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30701_INCLUDED  || defined ZLS30721_INCLUDED  || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED || defined ZLS3038X_INCLUDED
      #warning MSCC These are all faked (just to get started)
      const Uint32T ZL303XX_CHIP_SELECT_MASK = 0;     /* ALL FAKE!!!! */
      const Uint32T CSB2_CHIP_SELECT_MASK  = 0;     
      const Uint32T CSB3_CHIP_SELECT_MASK  = 0;    

      const Uint32T ZL303XX_DEVICE_HI_PRI_CPU_IRQ = 0;   /* ALL FAKE!!!! */
      const Uint32T ZL303XX_DEVICE_LO_PRI_CPU_IRQ = 0;
  #else
     #error Something is amiss in your board/sw_variant configuration
  #endif

#else
      #warning MSCC: SPI configuration info. required
      Uint8T devNameBase[32] = {"/dev/YourDevNameHere"};   /* This is the start of the name## (e.g.,  /dev/cpm_spi28) */
      Uint32T chipSelectPin = 0;      /* Port Y GPIO pin XX */

      #warning  MSCC: Chip Select and Interrupt indications are missing!
      /* Device interrupt 0 (high priority) */
      const Uint32T ZL303XX_DEVICE_HI_PRI_CPU_IRQ    = 28;
      /* Device interrupt 1 (lower priority) */
      const Uint32T ZL303XX_DEVICE_LO_PRI_CPU_IRQ    = 0;

      #warning  MSCC: Must specify bsp-specific chip select mask in ZL303XX_CHIP_SELECT_MASK!
      const Uint32T ZL303XX_CHIP_SELECT_MASK = 255;
#endif


/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/



#ifndef OS_LINUX

zlStatusE Mpc8313ChipSelectFn(Uint32T par, zl303xx_BooleanE enable);

/*****************   FUNCTION DEFINITIONS   **********************************/

/**

  Function Name:
   cpuConfigInterrupts

  Details:
   Performs any configuration necessary on the MPC8313 to allow external
   interrupts to be routed to the interrupt controller.

  Parameters:
        None

  Return Value:
   cpuStatusE

*******************************************************************************/
cpuStatusE cpuConfigInterrupts(void)
{
    Uint32T immrAddress = OS_IMMR_GET();

    /* configure IRQ1 as -ve edge int */
    *M8313_SECNR(immrAddress) |= IRQ1_SECNR_EDGE_MASK;
    *M8313_SEMSR(immrAddress) |= HI_PRI_IRQ_PIN_MASK;

    /* configure IRQ2 as -ve edge int */
    *M8313_SECNR(immrAddress) |= IRQ2_SECNR_EDGE_MASK;
    *M8313_SEMSR(immrAddress) |= LO_PRI_IRQ_PIN_MASK;

/*    printf("Intr - Mask: %08lx Control: %08lx\n", (Uint32T)(*M8313_SEMSR(immrAddress)),(Uint32T)(*M8313_SECNR(immrAddress))); */

    return CPU_OK;
}

/**

  Function Name:
   zl303xx_MaskInterruptFn

  Details:
   Masks or unmasks the specified interrupt from the device. The method of doing
   this is system and/or processor specific.
   This example is for an MPC8313 where the device high priority interrupt is
   connected to CPU interrupt source IRQ1 and the low priority interrupt to CPU
   interrupt source IRQ2

  Parameters:
   [in]    zl303xx_Params   Pointer to the device specific parameters in case it is required
                  for identifying the specific device. (Unused in this example)
   [in]    cpuIntNum      CPU interrupt number to mask off (Unused in this example)
   [in]    deviceIntNum   Device interrupt number to mask off. Values 0 or 1.
   [in]    bMasking       ZL303XX_TRUE if the interrupt is to be masked. ZL303XX_FALSE to
                  unmask the interrupt

  Return Value:
   None

*******************************************************************************/
void zl303xx_MaskInterruptFn(zl303xx_ParamsS * zl303xx_Params, Uint8T deviceIntNum, Uint8T cpuIntNum, zl303xx_BooleanE bMasking)
{
    Uint32T maskReg;
    Uint32T intBit;
    Uint32T immrAddress = OS_IMMR_GET();
    zl303xx_BooleanE gotMutex = ZL303XX_FALSE;
    zlStatusE status = ZL303XX_OK;

   /* This platform does not use the cpuIntNum so to avoid a warning we simply increment the unused valus */
   cpuIntNum++;

    /* In this implementation the IRQ pins used are edge triggered. No operation is
       required to mask the interrupt as it stays low as long as the device detects
       the interrupt condition.
       When the deferred handler calls this function to unmask the interrupt
       the interrupt source is masked off to ensure that the IRQ pin goes high
       ready for next -ve edge triggered interrupt */

    if (deviceIntNum == 0)
    {
       /* High priority device interrupt */
       intBit = HI_PRI_IRQ_PIN_MASK;
       maskReg = ZL303XX_TOP_ISR0_MASK_REG;
    }
    else
    {
       intBit = LO_PRI_IRQ_PIN_MASK;
       maskReg = ZL303XX_TOP_ISR1_MASK_REG;
    }

    if (bMasking == ZL303XX_TRUE)
    {
       /* Clear the pending bit for this interrupt */
       *M8313_SEPNR(immrAddress) |= intBit;
    }
    else
    {
       zl303xx_ReadWriteS rdWr;
       Uint32T readValue;

       /* Deferred interrupt handler re-enabling the interrupts. Since it may be that
          interrupts have come in before all were cleared, the int signal may have stayed
          low. To force an edge, mask and then unmask the interrupts from the device. */
       status = zl303xx_ReadWriteLock();

       if (status == ZL303XX_OK)
       {
          gotMutex = ZL303XX_TRUE;
       }

       if (status == ZL303XX_OK)
       {
          /* clear the mask to set o/p to 1. Also get the current mask value */
          rdWr.osExclusionEnable = ZL303XX_FALSE;

          status =  zl303xx_ReadModWrite(zl303xx_Params, &rdWr, maskReg, 0, 0xFF, &readValue);
       }

       if (status == ZL303XX_OK)
       {
          /* rewrite the previous mask */
          rdWr.osExclusionEnable = ZL303XX_FALSE;

          status =  zl303xx_Write(zl303xx_Params, &rdWr, maskReg, readValue);
       }

       if (gotMutex == ZL303XX_TRUE)
       {
          (void)zl303xx_ReadWriteUnlock();
       }
    }
}

/**

  Function Name:
   Mpc8313ChipSelectFn

  Details:
   Selects or release the specified Spi chip select pin.

  Parameters:
   [in]    par      Predefined Spi chip select pin.
   [in]    enable   Boolean value to select (true) or release (false) the csPin.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE Mpc8313ChipSelectFn(Uint32T par, zl303xx_BooleanE enable)
{
   Uint32T immrAddress = OS_IMMR_GET();

   if (enable)
   {
      *M8313_GP1DAT(immrAddress) &= ~(par);
   }
   else
   {
      *M8313_GP1DAT(immrAddress) |= (par);
   }

   return ZL303XX_OK;
}

/**

  Function Name:
   cpuConfigChipSelect

  Details:
   Performs any configuration necessary on the CPU to handle the SPI
   chip select for the ZL303XX_ device.

   In this example, general purpose pin is used as the chip select pin.

  Parameters:
   [in]    zl303xx_Params   Pointer to the device specific parameters which include the
                  Spi chip select structure.

  Return Value:
   cpuStatusE

*******************************************************************************/

cpuStatusE cpuConfigChipSelect(zl303xx_ParamsS * zl303xx_Params, Uint32T csPin)
{
    cpuStatusE cpuStatus = CPU_OK;
    zlStatusE status = ZL303XX_OK;
    Uint32T immrAddress = OS_IMMR_GET();

    /* configure GPIO1 as output */
    *M8313_GP1DIR(immrAddress) |= (csPin);  /* direction set to output */
    *M8313_GP1ODR(immrAddress) &= ~(csPin); /* open drain reg cleared */

    /* Set the initial state of the pin as inactive (select = low) */
    status = Mpc8313ChipSelectFn(csPin, ZL303XX_FALSE);

    /* Assign the Chip Select function and Pin Id to this Device */
    if (status == ZL303XX_OK)
    {
        zl303xx_Params->spiParams.chipSelect.csFuncPtr = Mpc8313ChipSelectFn;
        zl303xx_Params->spiParams.chipSelect.csFuncPar = csPin;
    }
   else
   {
      cpuStatus = CPU_ERROR;
   }

   return cpuStatus;
}

/**

  Function Name:
   Mpc8313SoftwareResetFn

  Details:
   Forces the ZL303XX_ reset pin low and then re-asserts it high. The
   requirement is for the pin to remain low for a minimum of 300ns. This example
   uses a more coarse delay in order to avoid using a nano second timer.

  Parameters:
   [in]    zl303xx_Params   Pointer to the device specific parameters in case it is required

  Return Value:
   None

*******************************************************************************/

cpuStatusE cpuSoftwareResetFn( void *hwParams )
{
   Uint32T immrAddress = OS_IMMR_GET();

   if (hwParams) {;}   /* Eliminates warnings. */
   #warning MSCC: cpuSoftwareResetFn() not implemented yet!

   return CPU_OK;
}

/**

  Function Name:
   cpuConfigSoftwareReset

  Details:
   Performs any configuration necessary on the CPU to handle a software
   reset of the ZL303XX_ device.

   In this example, general purpose pin is used as the device reset pin.

  Parameters:
   [in]    zl303xx_Params   Pointer to the device specific parameters in case it is required

  Return Value:
   cpuStatusE

*******************************************************************************/

cpuStatusE cpuConfigSoftwareReset( void *hwParams )
{
   Uint32T immrAddress = OS_IMMR_GET();

   if (hwParams) {;}   /* Eliminates warnings. */
   #warning MSCC: cpuConfigSoftwareReset() not implemented yet!
   return CPU_OK;
}
#endif  /* !OS_LINUX */



#ifdef OS_LINUX

#include "zl303xx_Global.h"
#include "zl303xx_Porting.h"
#include "zl303xx_RdWr.h" /* from API directory */

/*****************   FWD FUNCTION DECLARATIONS   ***************************/

/*****************   # DEFINES   **********************************************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/*****************   GLOBAL VARIABLES   ********************************/

/**

  Function Name:
   cpuConfigInterrupts

  Details:
   Performs any configuration necessary on the CPU to allow external
   interrupts to be routed to the interrupt controller.

  Parameters:
        None

  Return Value:
   cpuStatusE

*******************************************************************************/

cpuStatusE cpuConfigInterrupts(void)
{
    /* Done by the Linux Kernel */
    return CPU_OK;
}

/**

  Function Name:
   zl303xx_MaskInterruptFn

  Details:
   Masks or unmasks the specified interrupt from the device. The method of doing
   this is system and/or processor specific.

  Parameters:
   [in]    zl303xx_Params   Pointer to the device specific parameters in case it is required
                  for identifying the specific device. (Unused in this example)
   [in]    cpuIntNum      CPU interrupt number to mask off (Unused in this example)
   [in]    deviceIntNum   Device interrupt number to mask off. Values 0 or 1.
   [in]    bMasking       ZL303XX_TRUE if the interrupt is to be masked. ZL303XX_FALSE to
                  unmask the interrupt

  Return Value:
   None

*******************************************************************************/

void zl303xx_MaskInterruptFn(zl303xx_ParamsS * zl303xx_Params, Uint8T deviceIntNum, Uint8T cpuIntNum, zl303xx_BooleanE bMasking)
{
    /* Done by the Linux SPI Interrupt handler if ZL_INTR_MASK_OFF_ON turned on */
    if (zl303xx_Params && deviceIntNum && cpuIntNum && bMasking) {;}   /* Eliminates warnings. */
}

/**

  Function Name:
   cpuConfigChipSelect

  Details:
   Performs any configuration necessary on the CPU to handle the SPI
   chip select for the ZL303XX_ device.

  Parameters:
   [in]    zl303xx_Params   Pointer to the device specific parameters which include the
                  Spi chip select structure.

  Return Value:
   cpuStatusE

*******************************************************************************/

cpuStatusE cpuConfigChipSelect(zl303xx_ParamsS * zl303xx_Params, Uint32T csPin)
{


    if (zl303xx_Params && csPin) {;}   /* Eliminates warnings. */

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD

    if (ZL303XX_CHECK_POINTER(zl303xx_Params))
    {
        return ((signed int)ZL303XX_INVALID_POINTER);
    }

    /* Setup the first ZL303XX chip select */
	Sint8T deviceName[64] = {0};
    snprintf((char*)deviceName, sizeof(deviceName), "%s%d", devNameBase,  (SINT_T)csPin);
    strncpy((char*)zl303xx_Params->spiParams.linuxChipSelectDevName, (char*)deviceName, sizeof(zl303xx_Params->spiParams.linuxChipSelectDevName) -1);

        if (zl303xx_Params == zl303xx_Params0)
        {

      #if defined ZLS30341_INCLUDED
            ZL303XX_TRACE_ALWAYS("Using %s (for CS); %s(for Low) and %s(for High) interrupt devices for zl303xx_Params",
                               zl303xx_Params->spiParams.linuxChipSelectDevName,
                               zl303xx_Params->spiParams.linuxChipLowIntrDevName,
                               zl303xx_Params->spiParams.linuxChipHighIntrDevName,
                               0,0,0);
      #endif
        }

#else
    #warning MSCC: NO ZL303XX_ Chip Select defined for Linux and non-ZLE30320 board
    #warning MSCC: Chip select function missing
    printf("MSCC:  NO ZL303XX chip select and/or interrupt dev names defined for Linux !!!!\n");
    return CPU_ERROR;
#endif

    return CPU_OK;
}

/**

  Function Name:
   cpuSoftwareResetFn

  Details:
   Forces the ZL303XX_ reset pin low and then re-asserts it high. The
   requirement is for the pin to remain low for a minimum of 300ns. This example
   uses a more coarse delay in order to avoid using a nano second timer.

  Parameters:
   [in]    zl303xx_Params   Pointer to the device specific parameters in case it is required

  Return Value:
   None

*******************************************************************************/

cpuStatusE cpuSoftwareResetFn( void *hwParams )
{
   if (hwParams){;} /* warning removal */

   #warning MSCC: cpuSoftwareResetFn() not implemented yet!
   return CPU_OK;
}

/**

  Function Name:
   cpuConfigSoftwareReset

  Details:
   Performs any configuration necessary on the CPU to handle a software
   reset of the ZL303XX_ device.

  Parameters:
   [in]    zl303xx_Params   Pointer to the device specific parameters in case it is required

  Return Value:
   cpuStatusE

*******************************************************************************/

cpuStatusE cpuConfigSoftwareReset( void *hwParams )
{
   if (hwParams) {;}   /* Eliminates warnings. */
   #warning MSCC: cpuConfigSoftwareReset() not implemented yet!
   return CPU_OK;
}

#endif /* OS_LINUX */


/*****************   END   ****************************************************/


