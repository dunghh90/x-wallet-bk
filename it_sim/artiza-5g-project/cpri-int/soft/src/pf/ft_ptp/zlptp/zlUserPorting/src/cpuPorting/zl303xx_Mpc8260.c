

/******************************************************************************
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
*     This file is the MPC8260 implementation of the abstract interface defined
*     in zl303xx_Porting.h
*
******************************************************************************/

#ifdef OS_VXWORKS
#include "zl303xx_M8260.h"
#include "zl303xx_Porting.h"
#include "zl303xx_SpiPort.h"
#include "zl303xx_AddressMap.h" /* from API directory */
#include "zl303xx_RdWr.h" /* from API directory */

#if defined (MPC8260)
#include "zl303xx_M8260PinDefs.h"
    const Uint32T ZL303XX_CHIP_SELECT_MASK      = (Uint32T)M8260_PIN(28);  /* Port D GPIO pin 28 */
#else
   /* #warning MSCC: Must specify bsp-specific chip select mask in ZL303XX_CHIP_SELECT_MASK */
   const Uint32T ZL303XX_CHIP_SELECT_MASK = 255;
#endif

/*****************   # DEFINES   **********************************************/

#define HI_PRI_IRQ_PIN_MASK   (Uint32T)M8260_PIN(11)  /* Port C data pin 11 */
#define LO_PRI_IRQ_PIN_MASK   (Uint32T)M8260_PIN(15)  /* Port C data pin 15 */

#define RESET_PIN_MASK        (Uint32T)M8260_PIN(24)  /* Port B GPIO pin 24 */

/* Note that in older versions of the MPC8260 user manual, table 4-3 contains an
   incorrect listing of the interrupt numbers. This current example is intended to
   run on an Embedded Planet EP8260 and the interrupt support in this BSP uses
   the old numbering scheme. Care should be taken when choosing which interrupt
   number to use to ensure this is consistent with whatever BSP is used and that it will
   be mapped to the correct processor interrupt number and vector */

/* Device interrupt 0 (high priority) is attached to CPU pin PC11 */
const Uint32T ZL303XX_DEVICE_HI_PRI_CPU_IRQ    = 44;    /* Old interrupt number = 44. Mapped to correct
                                                          internal interrupt number 52 by the BSP */
const Uint32T ZL303XX_DEVICE_HI_PRI_CPU_VECTOR = 52;

/* Device interrupt 1 (lower priority) is attached to CPU pin PC15 */
const Uint32T ZL303XX_DEVICE_LO_PRI_CPU_IRQ    = 40;    /* Old interrupt number = 40. Mapped to correct
                                                          internal interrupt number 48 by the BSP */
const Uint32T ZL303XX_DEVICE_LO_PRI_CPU_VECTOR = 48;


/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/**

  Function Name:
   cpuConfigInterrupts

  Details:
   Performs any configuration necessary on the MPC8260 to allow external
   interrupts to be routed to the interrupt controller.

   In this example, general purpose parallel port C pins 11 and 15 are
   used as interrupt sources.

  Parameters:
        None

  Return Value:
   cpuStatusE

*******************************************************************************/

cpuStatusE cpuConfigInterrupts(void)
{
    Uint32T immrAddress = OS_IMMR_GET();

    /* configure PC11 as i/p */
    *M8260_PCDDR(immrAddress) &= ~HI_PRI_IRQ_PIN_MASK;
    *M8260_PCPAR(immrAddress) &= ~HI_PRI_IRQ_PIN_MASK;
    /* configure PC11 as -ve edge int */
    *M8260_SIEXR(immrAddress) |= HI_PRI_IRQ_PIN_MASK;


    /* configure PC15 as i/p */
    *M8260_PCDDR(immrAddress) &= ~LO_PRI_IRQ_PIN_MASK;
    *M8260_PCPAR(immrAddress) &= ~LO_PRI_IRQ_PIN_MASK;
    /* configure PC15 as -ve edge int */
    *M8260_SIEXR(immrAddress) |= LO_PRI_IRQ_PIN_MASK;

    return CPU_OK;
}

/**

  Function Name:
   zl303xx_MaskInterruptFn

  Details:
   Masks or unmasks the specified interrupt from the device. The method of doing
   this is system and/or processor specific.
   This example is for an MPC8260 where the device high priority interrupt is
   connected to CPU interrupt source PC11 and the low priority interrupt to CPU
   interrupt source PC15

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

    (void) cpuIntNum; /* warning removal for unused parameter */

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
      *M8260_SIPNR_H(immrAddress) = intBit;
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
   Mpc8260ChipSelectFn

  Details:
   Selects or release the specified Spi chip select pin.

  Parameters:
   [in]    par      Predefined Spi chip select pin.
   [in]    enable   Boolean value to select (true) or release (false) the csPin.

  Return Value:
   zlStatusE

*******************************************************************************/
/* typedef zlStatusE (*zl303xx_SpiChipSelectT)(Uint32T par, zl303xx_BooleanE enable); */
zlStatusE Mpc8260ChipSelectFn(Uint32T, zl303xx_BooleanE);
zlStatusE Mpc8260ChipSelectFn(Uint32T par, zl303xx_BooleanE enable)
{
   Uint32T immrAddress = OS_IMMR_GET();

   if (enable)
   {
      *M8260_PDDTR(immrAddress) &= ~(par);
   }
   else
   {
      *M8260_PDDTR(immrAddress) |= (par);
   }

   return ZL303XX_OK;
}

/**

  Function Name:
   cpuConfigChipSelect

  Details:
   Performs any configuration necessary on the CPU to handle the SPI
   chip select for the ZL303XX_ device.

   In this example, general purpose parallel port D pin 28 is used as the chip
   select pin.

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

   /* configure PD28 as output */
   *M8260_PDDDR(immrAddress) |= (csPin);
   *M8260_PDPAR(immrAddress) &= ~(csPin);
   *M8260_PDSOR(immrAddress) &= ~(csPin);

   /* Set the initial state of the pin as inactive (select = low) */
   status = Mpc8260ChipSelectFn(csPin, ZL303XX_FALSE);

   /* Assign the Chip Select function and Pin Id to this Device */
   if (status == ZL303XX_OK)
   {
      zl303xx_Params->spiParams.chipSelect.csFuncPtr = Mpc8260ChipSelectFn;
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

cpuStatusE cpuSoftwareResetFn(void *hwParams)
{
   Uint32T immrAddress = OS_IMMR_GET();

   if (hwParams) {;}   /* Eliminates warnings. */
   /* #warning MSCC: cpuSoftwareResetFn() not implemented yet! */
   return CPU_OK;
}

/**

  Function Name:
   cpuConfigSoftwareReset

  Details:
   Performs any configuration necessary on the CPU to handle a software
   reset of the ZL303XX_ device.

   In this example, general purpose parallel port B pin 24 is used as the device
   reset pin.

  Parameters:
   [in]    zl303xx_Params   Pointer to the device specific parameters in case it is required

  Return Value:
   cpuStatusE

*******************************************************************************/

cpuStatusE cpuConfigSoftwareReset( void *hwParams )
{
    Uint32T immrAddress;

    immrAddress = OS_IMMR_GET();

   if (hwParams) {;}   /* Eliminates warnings. */
   /* #warning MSCC: cpuConfigSoftwareReset() not implemented yet! */
 
   return CPU_OK;
}
#endif /* OS_VXWORKS */



#ifdef OS_LINUX

#include "zl303xx_Porting.h"
#include "zl303xx_AddressMap.h" /* from API directory */
#include "zl303xx_RdWr.h" /* from API directory */

#include <string.h>

/* #warning MSCC: Pathname specific to the BSP environment. */
#ifdef MPC8260
  #include <signal.h>
  #include "../../zlUserPorting/LinuxPorting/MPC8260/drivers/spi/cpm_spi.h"
#endif

extern zl303xx_ParamsS *zl303xx_Params0;

   /* #warning MSCC: Must specify bsp-specific chip select mask in ZL303XX_CHIP_SELECT_MASK */
   const Uint32T ZL303XX_CHIP_SELECT_MASK = 255;

/*****************   FWD FUNCTION DECLARATIONS   ***************************/
void itoa(SINT_T value, Uint8T* str, SINT_T base);


/*****************   # DEFINES   **********************************************/

/* Note that in older versions of the MPC8260 user manual, table 4-3 contains an
   incorrect listing of the interrupt numbers. This current example is intended to
   run on an Embedded Planet EP8260 and the interrupt support in this BSP uses
   the old numbering scheme. Care should be taken when choosing which interrupt
   number to use to ensure this is consistent with whatever BSP is used and that it will
   be mapped to the correct processor interrupt number and vector */

/* Device interrupt 0 (high priority) is attached to CPU pin PC11 */
const Uint32T ZL303XX_DEVICE_HI_PRI_CPU_IRQ    = 11;

const Uint32T ZL303XX_DEVICE_HI_PRI_CPU_VECTOR = 52;


/* Device interrupt 1 (lower priority) is attached to CPU pin PC15 */
const Uint32T ZL303XX_DEVICE_LO_PRI_CPU_IRQ    = 15;

const Uint32T ZL303XX_DEVICE_LO_PRI_CPU_VECTOR = 48;

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/*****************   GLOBAL VARIABLES   ********************************/
    /* #warning MSCC: SPI configuration info. required */
    Uint8T devNameBase[32] = {"/dev/YourDevNameHere"};   /* This is the start of the name## (e.g.,  /dev/cpm_spi28) */
    Uint32T chipSelectPin = 0;      /* Port Y GPIO pin XX */


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
    zl303xx_BooleanE bogus;

    if (ZL303XX_CHECK_POINTER(zl303xx_Params))      /* Warning removal */
    {
        ZL303XX_TRACE_ALWAYS("Null pointer", 0,0,0,0,0,0);
    }
    if (!(deviceIntNum || cpuIntNum))    /* Warning removal */
    {
        ZL303XX_TRACE_ALWAYS("deviceIntNum and cpuIntNum both 0", 0,0,0,0,0,0);
    }
    bogus = bMasking;    /* Warning removal */


    /* Done by the Linux SPI Interrupt handler if ZL_INTR_MASK_OFF_ON turned on */
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

    /* #warning MSCC: NO ZL303XX_ Chip Select defined for Linux */
    printf("MSCC: No ZL303XX chip select and interrupt dev names defined for Linux!\n");
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
   if (hwParams) {;}   /* Eliminates warnings. */
   /* #warning MSCC: cpuSoftwareResetFn() not implemented yet! */
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
   /* #warning MSCC: cpuConfigSoftwareReset() not implemented yet! */
   return CPU_OK;
}

#endif /* OS_LINUX */

/*****************   END   ****************************************************/

