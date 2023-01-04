

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Implements the SPI bus interface for the Motorola 8260.
*
*******************************************************************************/

#ifndef OS_LINUX
/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Os.h"
#include "zl303xx_ErrTrace.h"
#include "zl303xx_Porting.h"
#include "zl303xx_M8260.h"
#include "zl303xx_M8260PinDefs.h"
#include "zl303xx_SpiPort.h"
#include "zl303xx_SpiVxwSocket.h"

/*****************   DEFINES   ************************************************/

/* The SPI Interrupt on the 8260. */
#define SPI_IRQ   2

/******************************************************************************/
/* Define the SPI Parameter RAM. It stores the 8260 SPI data structure that   */
/* is configured during initialization and used by the CP during operation.   */
/******************************************************************************/
#define M8260_DPR_SPI(IMMBA)     ((volatile Sint16T *)((IMMBA) + 0x89FC))

/******************************************************************************/
/* Define the CLK, MISO, and MOSI SPI Pins on the 8260.                       */
/******************************************************************************/
#define SPI_MISO        M8260_PIN(16)     /* Input  PIN */
#define SPI_MOSI        M8260_PIN(17)     /* Output PIN */
#define SPI_CLK         M8260_PIN(18)     /* Output PIN */

/******************************************************************************/
/* MACROS to configure the SPI Pin Direction (Output or Input/BiDir).         */
/******************************************************************************/

/* Output PINs: (Port-D DIR value = 1). */
#define SPI_CFG_OUT(IMMBA, spi_pins)   *M8260_PDDDR(IMMBA) |= (spi_pins);

/* Input (& Bi-directional) PINs:  (Port-D DIR value = 0). */
#define SPI_CFG_IN(IMMBA, spi_pins)    *M8260_PDDDR(IMMBA) &= ~(spi_pins);

/******************************************************************************/
/* MACROS to configure the SPI Pins as a Dedicated Peripheral PIN or GPIO.    */
/******************************************************************************/

/* Dedicated Peripheral PINs: (Port-D PAR value = 1). These also need     */
/* their respective SO (Special Options) bits Set (Port-D SOR value = 1). */
#define SPI_CFG_4PRPH(IMMBA, spi_pins)      \
   {                                            \
      *M8260_PDPAR(IMMBA) |= (spi_pins);    \
      *M8260_PDSOR (IMMBA) |= (spi_pins);    \
   }

/* GPIO PINs: (Port-D PAR value = 0). These also need their respective    */
/* SO (Special Options) bit Cleared (Port-D SOR value = 0).  */
#define SPI_CFG_4GPIO(IMMBA, spi_pins)           \
   {                                            \
      *M8260_PDPAR(IMMBA) &= ~(spi_pins);   \
      *M8260_PDSOR(IMMBA) &= ~(spi_pins);   \
   }

/******************************************************************************/
/* MACROS & DEFINES to manage the SPI MODE Register.                          */
/******************************************************************************/

/* LoopBack Bit. */
#define SPI_MOD_LPBK_MSK      0x4000      /* Loopback Mask for MODE Reg.      */
#define SPI_MOD_LPBK_SHIFT    0xe         /* Loopback Shift for MODE Reg.     */
#define SPI_MOD_LPBK_EN       1           /* Set SPI in Loopback Mode.        */
#define SPI_MOD_LPBK_DS       0           /* Disable Loopback Mode on the SPI.*/
#define SPI_MOD_LPBK(x)       (((x) << SPI_MOD_LPBK_SHIFT) & SPI_MOD_LPBK_MSK)

/* Clock Polarity. */
#define SPI_MOD_CLKPL_MSK        0x2000   /* Clock Polarity Mask for MODE Reg.*/
#define SPI_MOD_CLKPL_SHIFT      0xd      /* Clock Polarity Shift for MODE Rg.*/
#define SPI_MOD_CLKPL_INACT_HI   1        /* Falling/Rising pol on Aardvark   */
#define SPI_MOD_CLKPL_INACT_LO   0        /* Rising/Falling pol on Aardvark   */
#define SPI_MOD_CLKPL(x)      (((x) << SPI_MOD_CLKPL_SHIFT) & SPI_MOD_CLKPL_MSK)

/* Clock Phase. */
#define SPI_MOD_CLKPH_MSK     0x1000      /* Clock Phase Mask for MODE Reg.   */
#define SPI_MOD_CLKPH_SHIFT   0xc         /* Clock Phase Shift for MODE Rg.   */
#define SPI_MOD_CLKPH_TRL     1           /* Setup/Sample phase on Aardvark   */
#define SPI_MOD_CLKPH_LD      0           /* Sample/Setup phase on Aardvark   */
#define SPI_MOD_CLKPH(x)      (((x) << SPI_MOD_CLKPH_SHIFT) & SPI_MOD_CLKPH_MSK)

/* Clock Source. */
#define SPI_MOD_CLKSRC_MSK    0x0800      /* Clock Source Mask for MODE Reg.  */
#define SPI_MOD_CLKSRC_SHIFT  0xb         /* Clock Source Shift for MODE Rg.  */
#define SPI_MOD_CLKSRC_D16    1           /* SPI CLK Input = BRG/16           */
#define SPI_MOD_CLKSRC_BRG    0           /* SPI CLK Input = BRG              */
#define SPI_MOD_CLKSRC(x)  (((x) << SPI_MOD_CLKSRC_SHIFT) & SPI_MOD_CLKSRC_MSK)

/* Bit order. */
#define SPI_MOD_BORDR_MSK     0x0400      /* Bit Order Mask for MODE Reg.     */
#define SPI_MOD_BORDR_SHIFT   0xa         /* Bit Order Shift for MODE Reg.    */
#define SPI_MOD_BORDR_MSB     1           /* Most Significant Bit First.      */
#define SPI_MOD_BORDR_LSB     0           /* Least Significant Bit First.     */
#define SPI_MOD_BORDR(x)      (((x) << SPI_MOD_BORDR_SHIFT) & SPI_MOD_BORDR_MSK)

/* Master / Slave Mode. */
#define SPI_MOD_MS_MSK        0x0200      /* Master/Slave Mask for MODE Reg.  */
#define SPI_MOD_MS_SHIFT      0x9         /* Master/Slave Shift for MODE Reg. */
#define SPI_MOD_MS_MSTR       1           /* SPI as a MASTER.                 */
#define SPI_MOD_MS_SLV        0           /* SPI as a SLAVE.                  */
#define SPI_MOD_MS(x)         (((x) << SPI_MOD_MS_SHIFT) & SPI_MOD_MS_MSK)

/* SPI Enable/Disable. */
#define SPI_MOD_EN_MSK        0x0100      /* Enable/Disable Mask for MODE Reg.*/
#define SPI_MOD_EN_SHIFT      0x8         /* Enable/Disable Shift for MODE Rg.*/
#define SPI_MOD_EN_ENABLE     1           /* Enable SPI.                      */
#define SPI_MOD_EN_DISABLE    0           /* Disable SPI.                     */
#define SPI_MOD_EN(x)         (((x) << SPI_MOD_EN_SHIFT) & SPI_MOD_EN_MSK)

/* Data Character Length */
#define SPI_MOD_CHRLN_MSK     0x00f0      /* Character Length Mask for MODE.  */
#define SPI_MOD_CHRLN_SHIFT   0x4         /* Character Length Shift for MODE. */
#define SPI_MOD_CHRLN_7       6           /* Length => 6 + 1 = charlen of 7   */
#define SPI_MOD_CHRLN_8       7           /* Length => 7 + 1 = charlen of 8   */
#define SPI_MOD_CHRLN(x)      (((x) << SPI_MOD_CHRLN_SHIFT) & SPI_MOD_CHRLN_MSK)

/* Prescale Modulus */
#define SPI_MOD_PRESCL_MSK    0x000f      /* Prescale Modulus Mask for MODE.  */
#define SPI_MOD_PRESCL_SHIFT  0x0         /* Prescale Modulus Shift for MODE. */
#define SPI_MOD_PRESCL(x)  (((x) << SPI_MOD_PRESCL_SHIFT) & SPI_MOD_PRESCL_MSK)

/* Pointer to the Mode Register. */
#define SPI_MOD_REG(IMMBA) ((volatile Uint16T *)(IMMBA + 0x11AA0))

/**************************************************************************/
/* MACROS & DEFINES to manage the SPI EVENTS and EVENTS MASK Registers.   */
/**************************************************************************/

/* SPI EVENT and MASK Definitions for the various pieces. */
#define SPI_EV_MME      0x20
#define SPI_EV_TXE      0x10
#define SPI_EV_BSY      0x04
#define SPI_EV_TXB      0x02
#define SPI_EV_RXB      0x01

/* READ requires us to Tx the request and then wait for the response. */
#define SPI_EV_ERROR    (SPI_EV_MME | SPI_EV_TXE | SPI_EV_BSY)
#define SPI_EV_ALL      (SPI_EV_ERROR | SPI_EV_TXB | SPI_EV_RXB)

#define SPI_INTR_ENABLE    SPI_ALL_EVENTS
#define SPI_INTR_DISABLE   0x00

#define SPI_EV_CLEAR       SPI_EV_ALL

/* Pointer to the EVENT and MASK Registers. */
#define SPI_EVT_REG(IMMBA)    ((volatile Uint8T *)(IMMBA + 0x11AA6))
#define SPI_MSK_REG(IMMBA)    ((volatile Uint8T *)(IMMBA + 0x11AAA))

/******************************************************************************/
/* MACROS & DEFINES to manage the SPI COMMAND Register.                       */
/******************************************************************************/

#define SPI_COM_TX_START      0x80

/* Length of Receive and Single Transmit Data Buffers */
#define SPI_BUFF_MAX_LEN      (32)

#define SPI_R_BIT       0x80
#define SPI_W_BIT       0x7F

/* Rx Buffer Descriptor status codes */
#define SPI_RX_BD_EMPTY          (Uint16T)0x8000
#define SPI_RX_BD_IS_LAST        (Uint16T)0x2000
#define SPI_RX_INT_ENABLE        (Uint16T)0x1000
#define SPI_RX_LAST_CHAR         (Uint16T)0x0800
#define SPI_RX_CONT_MODE         (Uint16T)0x0200
#define SPI_RX_OVERRUN           (Uint16T)0x0002
#define SPI_RX_MULTI_MASTER_ERR  (Uint16T)0x0001

#define SPI_RX_BD_INITIAL_STATUS    \
   (Uint16T)(SPI_RX_BD_EMPTY | SPI_RX_BD_IS_LAST | SPI_RX_INT_ENABLE)

/* Tx Buffer Descriptor status codes */
#define SPI_TX_BD_READY          (Uint16T)0x8000
#define SPI_TX_BD_IS_LAST        (Uint16T)0x2000
#define SPI_TX_INT_ENABLE        (Uint16T)0x1000
#define SPI_TX_LAST_CHAR         (Uint16T)0x0800
#define SPI_TX_CONT_MODE         (Uint16T)0x0200
#define SPI_TX_UNDERRUN          (Uint16T)0x0002
#define SPI_TX_MULTI_MASTER_ERR  (Uint16T)0x0001

#define SPI_TX_BD_INITIAL_STATUS    \
   (Uint16T)(SPI_RX_BD_EMPTY | SPI_RX_BD_IS_LAST |    \
             SPI_RX_INT_ENABLE | SPI_TX_LAST_CHAR)

/* SPI Command Reg */
#define  SPI_COM_REG(IMMBA)   ((volatile Uint8T *)(IMMBA + 0x11AAD))

/*****************   STATIC DATA TYPES  ***************************************/

/* M8260 SPI Buffer Descriptor Type */
typedef struct {
    Uint16T status;     /* status and control */
    Uint16T length;     /* length of data buffer in bytes */
    Uint8T  *data;      /* points to data buffer */
} M8260_BuffDescS;

/* M8260 SPI Parameter Type */
typedef struct {     /* offset description */
    volatile Sint16T    rbase;      /* 00 Rx buffer descriptors base address */
    volatile Sint16T    tbase;      /* 02 Tx buffer descriptors base address */
    volatile Sint8T     rfcr;       /* 04 Rx function code */
    volatile Sint8T     tfcr;       /* 05 Tx function code */
    volatile Sint16T    mrblr;      /* 06 maximum receive buffer length */
    volatile Sint32T    ris;        /* 08 Rx internal state */
    volatile Sint32T    ridp;       /* 0c Rx internal data pointer */
    volatile Sint16T    rbptr;      /* 10 Rx buffer descriptor pointer */
    volatile Sint16T    ribc;       /* 12 Rx internal byte count */
    volatile Sint32T    rt;         /* 14 Rx temp */
    volatile Sint32T    tis;        /* 18 Tx internal state */
    volatile Sint32T    tidp;       /* 1c Tx internal data pointer */
    volatile Sint16T    tbptr;      /* 20 Tx buffer descriptor pointer */
    volatile Sint16T    tibc;       /* 22 Tx internal byte count */
    volatile Sint32T    tt;         /* 24 Tx temp */
    volatile Sint32T    notused;    /* 28 not used */
    volatile Sint16T    rpbase;     /* 2c relocatable parameter RAM base */
    volatile Sint16T    reserved;   /* 2e reserved */
} M8260_SpiParamS;

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/* Initialize the individual parts of the SPI interface */
static spiStatusE spiInit8260Structures(void);
static spiStatusE spiInit8260CommProc(Uint32T cmd);

/* On any initialization error, free all allocated memory */
static void spi8260FreeMem(void);

/* Execute a SPI IO service (read or write) */
static spiStatusE spi8260Io(void *par, Uint32T regAddr, Uint8T *dataBuf,
                            Uint16T bufLen, spiServiceE rdWrService);

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/* Local SPI data structures needed for initializing the SPI interface */
static M8260_SpiParamS *pM8260_SpiParam = NULL;
static M8260_BuffDescS *pM8260_SpiRxBuffDesc = NULL;
static M8260_BuffDescS *pM8260_SpiTxBuffDesc = NULL;

/* Data buffers needed for storing Rx/Tx SPI data */
static Uint8T *pM8260_SpiTxBuffer = NULL;
static Uint8T *pM8260_SpiRxBuffer = NULL;

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINTIONS   ***************************/

/**

  Function Name:
   cpuConfigSpi

  Details:
   Initialises the SPI interface (includes related structures and interrupts)

  Parameters:
        None

  Return Value:
   0 (OK) or -1 if an error occurs

*******************************************************************************/
cpuStatusE cpuConfigSpi(void)
{
    cpuStatusE status = CPU_OK;

    /* #warning SPI initialisation code required */
    return status;
}  /* END cpuConfigSpi */

/**

  Function Name:
    cpuSpiIsrHandler

  Details:
   When an SPI packet is sent or received, an interrupt is generated at the CPM.
   The corresponding ISR clears the interrupt and gives a Semaphore so any task
   waiting on a Rx or Tx can continue.

  Parameters:
        None

  Return Value:
   None

*******************************************************************************/
void cpuSpiIsrHandler(void)
{
   /* Clear the Event that triggered the Interrupt. */
   *SPI_EVT_REG(OS_IMMR_GET()) = SPI_EV_CLEAR;

   /* Give the Tx/Rx Sema4 so the calling task can continue. */
   (void)OS_SEMA4_GIVE(spiIsrSemId);
}  /* END cpuSpiIsrHandler */

/**

  Function Name:
   cpuSpiCleanUp

  Details:
*  Shutdown the SPI interface (includes related structures and
interrupts)**

  Parameters:
        None

  Return Value:
   cpuStatusE

*******************************************************************************/
cpuStatusE cpuSpiCleanUp(void)
{
    spiStatusE spiStatus = CPU_OK;

    /* #warning SPI shutdown code required */
    return spiStatus;
}  /* END cpuSpiCleanUp */

/**

  Function Name:
   cpuSpiRead

  Details:
   Sends a read request to a Device.

  Parameters:
       par      device structure pointer that is unused in this function
       regAddr  direct SPI address to read from
       bufLen   number of bytes to read from the device

       dataBuf  the data read from the register (if successful).

  Return Value:
   0 (OK) or -1 if an error occurs

*******************************************************************************/
cpuStatusE cpuSpiRead(void *par, Uint32T regAddr,
                            Uint8T *dataBuf, Uint16T bufLen)
{
   return spi8260Io(par, regAddr, dataBuf, bufLen, SPI_SERVICE_READ);
}  /* END cpuSpiRead */

/**

  Function Name:
   cpuSpiWrite

  Details:
   Starting at the given address, a block of data (of a given length) is written
   to indicated device.

  Parameters:
       par      device structure pointer that is unused in this function
       regAddr  SPI address to write
       data     the data to write to the requested register.

  Return Value:
   0 (OK) or -1 if an error occurs

*******************************************************************************/
cpuStatusE cpuSpiWrite(void *par, Uint32T regAddr,
                             Uint8T *dataBuf, Uint16T bufLen)
{
   return spi8260Io(par, regAddr, dataBuf, bufLen, SPI_SERVICE_WRITE);
}  /* END cpuSpiWrite */

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/**

  Function Name:
    spiInit8260Structures

  Details:
   Initializes the SPI data structures, dedicated hardware pins and registers
   used by the CPM for SPI control.

  Parameters:
        None

  Return Value:
   spiStatusE

*******************************************************************************/
static spiStatusE spiInit8260Structures(void)
{
   spiStatusE spiStatus = SPI_OK;
   Uint32T immrVal = OS_IMMR_GET();

   /********************************************************************/
   /* Create an SPI data structure in DP RAM and assign it to the SPI  */
   /* parameter RAM pointer. The SPI parameter RAM is used by the CP   */
   /* to manage and control the SPI port.                              */
   /********************************************************************/

   if (pM8260_SpiParam != NULL)
   {
      /* The SPI is already initialized so indicate so... */
      spiStatus = SPI_MULTIPLE_INIT;
   }
   else
   {
      /* Allocate the SPI Data structure in DP RAM. */
      pM8260_SpiParam = (M8260_SpiParamS*)OS_DPRAM_MALLOC(sizeof(M8260_SpiParamS), 64);

      /* Check that allocation was successful */
      if (pM8260_SpiParam == NULL)
      {
         spiStatus = SPI_MALLOC_ERROR;
      }
   }

   /* If we managed to allocate the structure memory, continue configuration */
   if (spiStatus == SPI_OK)
   {
      /* Clear the allocated space. */
      memset(pM8260_SpiParam, 0, sizeof(M8260_SpiParamS));

      /* Assign the space to the SPI Parameter RAM pointer. */
      *M8260_DPR_SPI(immrVal) = (Sint16T)((Sint32T)pM8260_SpiParam);

      /* Define the SPI Input/Output Pins.    */
      SPI_CFG_OUT(immrVal, SPI_MOSI | SPI_CLK );
      SPI_CFG_IN(immrVal, SPI_MISO );

      /* Define Dedicated Peripheral Pins  */
      SPI_CFG_4PRPH(immrVal, SPI_MISO | SPI_MOSI | SPI_CLK);

      /* Disable SPI MODE Register prior to setting the other parameters.  */
      *SPI_MOD_REG(immrVal) = SPI_MOD_EN_DISABLE;

      /* Set all the other Bits in the SPI MODE Register. */
      *SPI_MOD_REG(immrVal) = (SPI_MOD_LPBK(SPI_MOD_LPBK_DS)
                             | SPI_MOD_CLKPL(SPI_MOD_CLKPL_INACT_LO)
                             | SPI_MOD_CLKPH(SPI_MOD_CLKPH_LD)
                             | SPI_MOD_CLKSRC(SPI_MOD_CLKSRC_BRG)
                             | SPI_MOD_BORDR(SPI_MOD_BORDR_MSB)
                             | SPI_MOD_MS(SPI_MOD_MS_MSTR)
                             | SPI_MOD_CHRLN(SPI_MOD_CHRLN_8)
                             | SPI_MOD_PRESCL(0));

      /***************************************************/
      /* Initialize the SPI RAM.                         */
      /***************************************************/
      pM8260_SpiParam->rfcr  = 0x10;
      pM8260_SpiParam->tfcr  = 0x10;
      pM8260_SpiParam->mrblr = SPI_BUFF_MAX_LEN;

      /* allocate the Tx/Rx Data Buffers */
      pM8260_SpiTxBuffer = (Uint8T *)OS_DPRAM_MALLOC(SPI_BUFF_MAX_LEN, 32);
      pM8260_SpiRxBuffer = (Uint8T *)OS_DPRAM_MALLOC(SPI_BUFF_MAX_LEN, 32);

      if ((pM8260_SpiTxBuffer == NULL) || (pM8260_SpiRxBuffer == NULL))
      {
         spiStatus = SPI_MALLOC_ERROR;
      }
      else /* allocate the Buffer Descriptors */
      {
         /* allocate the Buffer Descriptors. */
         /* Rx Buffer alignment must be on an even address so set 32-bit just to be safe */
         pM8260_SpiRxBuffDesc = (M8260_BuffDescS*)OS_DPRAM_MALLOC(sizeof(M8260_BuffDescS), 32);
         pM8260_SpiTxBuffDesc = (M8260_BuffDescS*)OS_DPRAM_MALLOC(sizeof(M8260_BuffDescS), 32);

         /* Make sure the BDs were allocated */
         if ((pM8260_SpiRxBuffDesc == NULL) || (pM8260_SpiTxBuffDesc == NULL))
         {
            spiStatus = SPI_MALLOC_ERROR;
         }
      }
   }

   /* Check all Buffer descriptor and buffer allocation */
   if (spiStatus == SPI_OK)
   {
      /* Memory Allocation was successful */

      /* assign the Rx & Tx buffers to the BD data pointers */
      pM8260_SpiRxBuffDesc->data = (Uint8T *)pM8260_SpiRxBuffer;
      pM8260_SpiTxBuffDesc->data = (Uint8T *)pM8260_SpiTxBuffer;

      /* assign the BDs to the SPI Structure. */
      pM8260_SpiParam->rbase = (Uint16T)((Sint32T)pM8260_SpiRxBuffDesc);
      pM8260_SpiParam->tbase = (Uint16T)((Sint32T)pM8260_SpiTxBuffDesc);

      /* Send the Init Command to the CP. */
      spiStatus = spiInit8260CommProc((Uint8T)M8260_CPCR_RT_INIT);
   }

   /* If everything went OK, enable the interface */
   if (spiStatus == SPI_OK)
   {
      /* Reset the SPI Event and Mask Registers */
      *SPI_EVT_REG(immrVal) = SPI_EV_CLEAR;     /* Clear existing SPI events */
      *SPI_MSK_REG(immrVal) = SPI_INTR_DISABLE;    /* Disable SPI interrupts */

      /* Finally, enable the SPI interface again.  */
      *SPI_MOD_REG(immrVal) |= SPI_MOD_EN(SPI_MOD_EN_ENABLE);
   }

   /* If things are already initialized, just return. Otherwise, clean up any
      memory that has been allocated */
   else if (spiStatus != SPI_MULTIPLE_INIT)
   {
      spi8260FreeMem();
   }

   return spiStatus;
}  /* END spiInit8260Structures */

/**

  Function Name:
    spiInit8260CommProc

  Details:
   Purpose is to execute the Communication Processor command passed. The channel
   command register is read prior to execution to verify that there are no
   outstanding commands pending. It is also read following the command execution
   to insure the CP command completed.

  Parameters:
       cmd         The Communication Processor Command to be executed.

  Return Value:
   spiStatusE

*******************************************************************************/
spiStatusE spiInit8260CommProc(Uint32T cmd)
{
   spiStatusE spiStatus = SPI_OK;
   Uint32T immrVal = OS_IMMR_GET();

   Uint32T cpcrVal;
   Sint32T intLevel;
   Uint32T waitLoop = 0;
   Uint32T timeoutTick = (Uint32T)(OS_TICK_RATE_GET()/4);

   /* we cannot be interrupted */
   intLevel = OS_INTERRUPT_LOCK();

   /* Wait until the CP is finished with any previous Command. */
   while ((*M8260_CPCR(immrVal) & M8260_CPCR_FLG) != 0)
   {
      /* Do not wait forever, it will be timeout after 250ms */
      if(waitLoop++ > timeoutTick)
      {
         /* Unlock the interrupts and return. */
         OS_INTERRUPT_UNLOCK(intLevel);

         spiStatus = SPI_CMD_CP_BUSY;
         ZL303XX_ERROR_TRAP("CP Command Timeout");
         break;
      }

      /* If we haven't timed-out yet, just delay a little.*/
      (void)OS_TICK_DELAY(1);
   }

   if (spiStatus == SPI_OK)
   {
      /* Create the command for the CP. */
      cpcrVal = (M8260_CPCR_OP(cmd)
               | M8260_CPCR_SBC(M8260_CPCR_SBC_SPI)
               | M8260_CPCR_PAGE(M8260_CPCR_PAGE_SPI)
               | M8260_CPCR_MCN(0)
               | M8260_CPCR_FLG);

      /* Issue the CP command */
      *M8260_CPCR(immrVal) = cpcrVal;
   }

   OS_INTERRUPT_UNLOCK(intLevel);

   return spiStatus;
}  /* END spiInit8260CommProc */

/**

  Function Name:
    spi8260FreeMem

  Details:
   De-alocates all allocated memory associated with the SPI buffer descriptors
   and Rx/Tx buffer space.

  Parameters:
        None

  Return Value:
   None

*******************************************************************************/
static void spi8260FreeMem(void)
{
   if (pM8260_SpiRxBuffer)
       OS_DPRAM_FREE(pM8260_SpiRxBuffer); /* DP Ram */
   if (pM8260_SpiTxBuffer)
       OS_DPRAM_FREE(pM8260_SpiTxBuffer);
   if (pM8260_SpiRxBuffDesc)
       OS_DPRAM_FREE(pM8260_SpiRxBuffDesc);
   if (pM8260_SpiTxBuffDesc)
       OS_DPRAM_FREE(pM8260_SpiTxBuffDesc);
   if (pM8260_SpiParam)
       OS_DPRAM_FREE(pM8260_SpiParam);

   pM8260_SpiParam = NULL;
}  /* END spi8260FreeMem */

/**

  Function Name:
   spi8260Io

  Details:
   Executes the specified SPI IO request on the 8260 processor.

  Parameters:
       par            device structure pointer that is unused in this function
       regAddr        direct SPI address to read from
       dataBuf        the data to write to the register (for write requests)
       bufLen         number of bytes to read from the device
       rdWrService    type of IO service (read or write)

       dataBuf  the data read from the register (for read requests)

  Return Value:
   spiStatusE

  Notes:
   This function assumes that any Spi chip select operation has already been
   handled.

*******************************************************************************/
static spiStatusE spi8260Io(void *par, Uint32T regAddr,
                            Uint8T *dataBuf, Uint16T bufLen,
                            spiServiceE rdWrService)
{
   spiStatusE spiStatus = SPI_OK;
   Uint32T immrVal = OS_IMMR_GET();
   zl303xx_BooleanE gotMutex = ZL303XX_FALSE;

   (void)par;  /* Warning removal */

   if (spiStatus == SPI_OK)
   {
      /* Block other calls to the Driver until we are finished. */
      spiStatus = spiLock();
   }

   if (spiStatus == SPI_OK)
   {
      /* Flag to check the status of any OS calls */
      OS_STATUS osStatus = OS_OK;

      /* We locked so set a flag indicating we have the Spi Mutex */
      gotMutex = ZL303XX_TRUE;

      /* Clear the Tx/Rx Interupt Sema4 */
      (void)OS_SEMA4_TAKE(spiIsrSemId, NO_WAIT);

      /* Clear old interrupts and set the RX interrupt */
      *SPI_MSK_REG(immrVal) = SPI_EV_RXB;
      *SPI_EVT_REG(immrVal) = SPI_EV_CLEAR;

      if (rdWrService == SPI_SERVICE_WRITE)
      {
         /* Send the Write bit and the Register Address... */
         pM8260_SpiTxBuffer[0] = regAddr & SPI_W_BIT;

         /* ...copy the write data as well */
         memcpy(&(pM8260_SpiTxBuffer[1]), dataBuf, bufLen);
      }
      else
      {
         /* Send the Read bit and the Start Register Address.
            Since the TX here is just to keep the SPI_CLK going, we don't care
            about the rest of the bytes in the TX buffer. */
         pM8260_SpiTxBuffer[0] = regAddr | SPI_R_BIT;
      }

      pM8260_SpiTxBuffDesc->length = 1 + bufLen;
      pM8260_SpiTxBuffDesc->status = SPI_TX_BD_INITIAL_STATUS;

      pM8260_SpiRxBuffDesc->length = 0;
      pM8260_SpiRxBuffDesc->status = SPI_TX_BD_INITIAL_STATUS;

      /* Transmit the package. */
      *SPI_COM_REG(immrVal) = SPI_COM_TX_START;

      /* Wait for the request to be acknowledged. */
      osStatus = OS_SEMA4_TAKE(spiIsrSemId, 1000);

      /* check that there wasn't an error - most likely a timeout */
      if (osStatus == OS_ERROR)
      {
         spiStatus = SPI_SEM_TAKE_FAIL;
         ZL303XX_ERROR_TRAP("SPI write timeout");
      }
   }

   /* On READs, copy the received data into the return buffer */
   if (rdWrService == SPI_SERVICE_READ)
   {
      if (spiStatus == SPI_OK)
      {
         memcpy(dataBuf, &(pM8260_SpiRxBuffer[1]), bufLen);
      }
   }

   /* Release any Mutex */
   if (gotMutex == ZL303XX_TRUE)
   {
      (void)spiUnlock();
   }

   return spiStatus;
}  /* END spi8260Io */
#else
   #define EMPTY_MODULE 1
   /* #warning MSCC: This file should not be included for Linux */
#endif

/*****************   END   ****************************************************/
