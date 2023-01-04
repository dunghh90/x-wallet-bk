

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Implements the SPI bus interface for the Freescale 8313.
*
*******************************************************************************/

#ifndef OS_LINUX
/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Os.h"
#include "zl303xx_ErrTrace.h"
#include "zl303xx_Porting.h"
#include "zl303xx_SpiPort.h"
#include "zl303xx_SpiVxwSocket.h"

/*****************   DEFINES   ************************************************/

 /* The SPI Interrupt on the 8313. */
 #define SPI_IRQ           (16)

 /* base address of the SPI registers */
 #define M8313_SPI(base)   ((volatile Sint32T *)((base) + (Uint32T)0x7000))

 #define SPI_MODE_LOOP     (0x80000000 >> 1)  /* Loopback mode */
 #define SPI_MODE_CI       (0x80000000 >> 2)  /* Clock inactive - 0 low, 1 high */
 #define SPI_MODE_CP       (0x80000000 >> 3)  /* Clock phase - 0 toggle in middle, 1 toggle at beginning */
 #define SPI_MODE_REV      (0x80000000 >> 5)  /* Reverse mode - MSB first */
 #define SPI_MODE_MS       (0x80000000 >> 6)  /* Always master */
 #define SPI_MODE_EN       (0x80000000 >> 7)  /* Enable interface */

 #define SPI_EV_NE         (0x80000000 >> 22) /* Receiver Not Empty */
 #define SPI_EV_NF         (0x80000000 >> 23) /* Transmitter Not Full */
 #define SPI_EV_LT         (0x80000000 >> 17) /* Last Char transmitted */

 #define SPI_COM_LST       (0x80000000 >> 9)  /* Last char. */

 /* Length of Receive and Single Transmit Data Buffers */
 #define SPI_BUFF_MAX_LEN  (32)

 #define SPI_R_BIT         0x80
 #define SPI_W_BIT         0x7F

 #define SPI_TIMEOUT       (1000)

/*****************   STATIC DATA TYPES  ***************************************/

typedef struct spi8xxx {
    Uint8T res0[0x20];  /* 0x0-0x01f reserved */
    Uint32T mode;       /* mode register  */
    Uint32T event;      /* event register */
    Uint32T mask;       /* mask register  */
    Uint32T com;        /* command register */
    Uint32T tx;         /* transmit register */
    Uint32T rx;         /* receive register */
    Uint8T res1[0xFC8]; /* fill up to 0x1000 */
} spi8xxx_t;

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/* Initialize the individual parts of the SPI interface */
static void spiInit8313Registers(void);

/* Execute a SPI IO service (read or write) */
static cpuStatusE spi8313Io(void *par, Uint32T regAddr, Uint8T *dataBuf,
                            Uint16T bufLen, spiServiceE rdWrService);

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/* Local SPI data structures needed for initializing the SPI interface */
Uint8T M8313_SpiTxBuffer[SPI_BUFF_MAX_LEN];
Uint8T M8313_SpiRxBuffer[SPI_BUFF_MAX_LEN];

/* Data buffers needed for storing Rx/Tx SPI data */
static Uint8T *pM8313_SpiTxBuffer = NULL;
static Uint8T *pM8313_SpiRxBuffer = NULL;

/*****************   EXPORTED FUNCTION DEFINTIONS   ***************************/

/**

  Function Name:
   cpuConfigSpi

  Details:
   Initialises the SPI interface (includes related structures and interrupts)

  Parameters:
        None

  Return Value:
   cpuStatusE

*******************************************************************************/
cpuStatusE cpuConfigSpi( void )
{
    cpuStatusE cpuStatus = CPU_OK;

    /* Initialize the SPI structures */
    spiInit8313Registers();

    if ((pM8313_SpiTxBuffer != NULL) && (pM8313_SpiRxBuffer != NULL))
    {
        cpuStatus = CPU_SPI_MULTIPLE_INIT;
    }
    else
    {
        /* Data buffers needed for storing Rx/Tx SPI data */
        pM8313_SpiTxBuffer = &M8313_SpiTxBuffer[0];
        pM8313_SpiRxBuffer = &M8313_SpiRxBuffer[0];
    }

    if (cpuStatus == CPU_OK)
    {
        /* Initialize the SPI interrupts */
        if ((cpuStatus = zl303xx_SpiInitInterrupts(SPI_IRQ)) != CPU_OK)
        {
            (void) cpuSpiCleanUp();
        }
    }

    return cpuStatus;
}  /* END cpuConfigSpi */

/**

  Function Name:
    cpuSpiIsrHandler

  Details:
   When an SPI packet is sent or received, an interrupt is generated in the
   <blank> register.  The corresponding ISR clears the interrupt and gives a
   Semaphore so any task waiting on a Rx or Tx can continue.

  Parameters:
        None

  Return Value:
   None

*******************************************************************************/
void cpuSpiIsrHandler(void)
{
   /* Clear the Event that triggered the Interrupt. */

   /* Give the Tx/Rx Sema4 so the calling task can continue. */
   (void)OS_SEMA4_GIVE(spiIsrSemId);
}  /* END cpuSpiIsrHandler */

/**

  Function Name:
   cpuSpiCleanUp

  Details:
   Clean up the SPI interface (includes related structures and interrupts)

  Parameters:
        None

  Return Value:
   cpuStatusE

*******************************************************************************/
cpuStatusE cpuSpiCleanUp( void )
{
   Uint32T immrAddress = OS_IMMR_GET();
   volatile spi8xxx_t* spi = &(*(spi8xxx_t *) M8313_SPI(immrAddress));

   (void) zl303xx_SpiDisableInterrupts(SPI_IRQ);

   /* Clear data buffers */
   pM8313_SpiTxBuffer = NULL;
   pM8313_SpiRxBuffer = NULL;

   /* disable SPI */
   spi->mode &= ~SPI_MODE_EN;

   return CPU_OK;
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
   cpuStatusE

*******************************************************************************/
cpuStatusE cpuSpiRead(void *par, Uint32T regAddr,
                      Uint8T *dataBuf, Uint16T bufLen)
{
   return spi8313Io(par, regAddr, dataBuf, bufLen, SPI_SERVICE_READ);
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
   cpuStatusE

*******************************************************************************/
cpuStatusE cpuSpiWrite(void *par, Uint32T regAddr,
                       Uint8T *dataBuf, Uint16T bufLen)
{
   return spi8313Io(par, regAddr, dataBuf, bufLen, SPI_SERVICE_WRITE);;
}  /* END cpuSpiWrite */


/******************************************************************************/
/*****************   STATIC FUNCTION DEFINITIONS   ****************************/
/******************************************************************************/

/**

  Function Name:
    spiInit8313Registers

  Details:
   Initializes the SPI data structures, dedicated hardware pins and registers
   for SPI control.

  Parameters:
        None

  Return Value:
   cpuStatusE

*******************************************************************************/

void spiInit8313Registers(void)
{
   Uint32T immrAddress = OS_IMMR_GET();

   volatile spi8xxx_t* spi = &(*(spi8xxx_t *) M8313_SPI(immrAddress));

   /*
    * SPI pins on the MPC8313 are not muxed, so all we do is initialize
    * some registers
    */

   /* disable during init */
   spi->mode &= ~SPI_MODE_EN;

   spi->mode = SPI_MODE_REV | SPI_MODE_MS | SPI_MODE_CP;

   /* spi clock setup (baudrate) = sysclk / (DIV16 * (4 * (PM + 1))) where sysclk on trd is 133.33M */
   /* no divide by 16, PM is 5: sysclk / 24 = 6.9MHz.  device requires < 8MHz */
   spi->mode |= (5 << 16);

   /* enable after init */
   spi->mode |= SPI_MODE_EN;

   spi->event = 0xffffffff; /* Clear all SPI events */
   spi->mask = 0x00000000;  /* Mask  all SPI interrupts */
   spi->com = SPI_COM_LST;  /* LST bit doesn't do anything, so disregard */
}

/**

  Function Name:
   spi8313Io

  Details:
   Executes the specified SPI IO request on the 8313 processor.

  Parameters:
       par            device structure pointer that is unused in this function
       regAddr        direct SPI address to read from
       dataBuf        the data to write to the register (for write requests)
       bufLen         number of bytes to read from the device
       rdWrService    type of IO service (read or write)

       dataBuf  the data read from the register (for read requests)

  Return Value:
   cpuStatusE

  Notes:
   This function assumes that any Spi chip select operation has already been
   handled.

*******************************************************************************/
static cpuStatusE spi8313Io(void *par, Uint32T regAddr, Uint8T *dataBuf,
                            Uint16T bufLen, spiServiceE rdWrService)
{
   Uint32T immrAddress = OS_IMMR_GET();

   volatile spi8xxx_t* spi = &(*(spi8xxx_t *) M8313_SPI(immrAddress));
   cpuStatusE cpuStatus = CPU_OK;
   zl303xx_BooleanE gotMutex = ZL303XX_FALSE;
   Uint32T tmpdout, event, tmpdin = 0;
   Uint32T numBlks = 0;
   Uint32T bitLen = 0;
   Uint32T tm, isRead = 0;
   Uint8T charSize = 8;

   (void)par;  /* Warning removal */

   if (cpuStatus == CPU_OK)
   {
       /* Block other calls to the Driver until we are finished. */
       cpuStatus = spiLock();
   }

   if (cpuStatus == CPU_OK)
   {
       /* We locked so set a flag indicating we have the Spi Mutex */
       gotMutex = ZL303XX_TRUE;

       /* Clear all SPI interrupts */
       spi->event = 0xffffffff;
       spi->mask = 0x00000000;

       pM8313_SpiTxBuffer = &M8313_SpiTxBuffer[0];
       pM8313_SpiRxBuffer = &M8313_SpiRxBuffer[0];

      if (rdWrService == SPI_SERVICE_WRITE)
      {
         /* Send the Write bit and the Register Address... */
         pM8313_SpiTxBuffer[0] = regAddr & SPI_W_BIT;

         /* ...copy the write data as well */
         memcpy(&(pM8313_SpiTxBuffer[1]), dataBuf, bufLen);
         tmpdout = 0;
      }
      else
      {
         /* Send the Read bit and the Start Register Address.
            Since the TX here is just to keep the SPI_CLK going, we don't care
            about the rest of the bytes in the TX buffer. */
         pM8313_SpiTxBuffer[0] = regAddr | SPI_R_BIT;
         tmpdin = 0;
      }

      /* add on the address byte */
      bufLen++;

      /* calculate the number of blocks */

      bitLen = bufLen * 8;
      numBlks = bitLen / 8 + (bitLen % 8 ? 1 : 0);

      /* OS_LOG_MSG("bufLen: %d bitLen: %d numBlks: %d",bufLen,bitLen,numBlks,0,0,0); */

      /* restore bufLen */
      bufLen--;

      /* handle data in 16-bit chunks because the spi mode can't
       * handle 24-bit chunks */
      while (numBlks--) {
          tmpdout = 0;
          charSize = (bitLen >= 8 ? 8 : bitLen);

          /* Shift data so it's msb-justified */
          tmpdout = *(Uint32T *) (void *) pM8313_SpiTxBuffer >> (32 - charSize);

          /* The LEN field of the SPMODE register is set as follows:
           *
           * Bit length             setting
           * len <= 4               3
           * 4 < len <= 16          len - 1
           * len > 16               0
           */

         /* NOTE: original driver changed LEN on the fly.
          * This did not work.  Switching from 16 bit to 8 bit length
          * caused erratic behaviour - 8 bit write clocked out
          * 40 bits!
          */

         /* Set up the next iteration if sending > 8 bits */
         spi->mode = ((spi->mode & 0xff0fffff) | ((charSize - 1) << 20));
         bitLen -= 8;
         pM8313_SpiTxBuffer += 1;

          spi->tx = tmpdout;    /* Write the data out */

          /*
           * Wait for SPI transmit to get out
           * or time out (1 second = 1000 ms)
           * The NE event must be read and cleared first
           */


          for (tm = 0, isRead = 0; tm < SPI_TIMEOUT; ++tm)
          {
              event = spi->event;
              if (event & SPI_EV_NE) {
                  while(spi->event & SPI_EV_NE){
                     tmpdin = spi->rx;
                     spi->event |= SPI_EV_NE;
                     isRead = 1;
                  }

                  *(Uint32T *) (void *) pM8313_SpiRxBuffer = (tmpdin << (24));
                  if(charSize == 8)
                  {
                      /* Advance output buffer */

                      /* on multi byte reads/writes (16-bit chunks):
                       * 1st 'block' is address and 1 byte data,
                       * 2nd 'block' is either 1 or 2 bytes data, (
                       * 3rd 'block' is either 1 or 2 bytes data,
                       * 4th 'block' is always 2 bytes data,
                       * 5th 'block' is always 1 byte of data
                       */
                     pM8313_SpiRxBuffer += 1;
                  }
              }
              /*
               * Only bail when we've had both NE and NF events.
               * This will cause timeouts on RO devices, so maybe
               * in the future put an arbitrary delay after writing
               * the device.  Arbitrary delays suck, though...
               */
              if (isRead && (event & SPI_EV_NF)){
                 /* OS_LOG_MSG("break event: %08lX numBlks: %d tmpdout: %08lX tmpdin: %08lX",spi->event,numBlks,tmpdout,tmpdin,0,0); */
                 break;
              }
         }
         if(tm >= SPI_TIMEOUT){
            OS_LOG_MSG("TIMEOUT",0,0,0,0,0,0);
         }
      } /* end while */

      /* On READs, copy the received data into the return buffer */
      if (rdWrService == SPI_SERVICE_READ)
      {
         if (cpuStatus == CPU_OK)
         {
/*
            OS_LOG_MSG("rx[0]: 0x%x rx[1]: 0x%x rx[2]: 0x%x rx[3]: 0x%x rx[4]: 0x%x rx[5]: 0x%x",
               M8313_SpiRxBuffer[0],
               M8313_SpiRxBuffer[1],
               M8313_SpiRxBuffer[2],
               M8313_SpiRxBuffer[3],
               M8313_SpiRxBuffer[4],
               M8313_SpiRxBuffer[5]);
*/
            memcpy(dataBuf, &(M8313_SpiRxBuffer[1]), bufLen);
         }
      }

   }
   /* Release any Mutex */
   if (gotMutex == ZL303XX_TRUE)
   {
       (void)spiUnlock();
   }

   return cpuStatus;
}  /* END spi8313Io */

/*****************   END   ****************************************************/
#endif /* !OS_LINUX*/
