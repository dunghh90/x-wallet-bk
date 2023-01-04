

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Stub functions for the SPI bus interface.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Os.h"
#include "zl303xx_ErrTrace.h"
#include "zl303xx_Error.h"
#include "zl303xx_Porting.h"

#if defined ZLS30341_INCLUDED
#include "zl303xx_AddressMap34x.h"
#endif
#if defined ZLS30361_INCLUDED
#include "zl303xx_AddressMap36x.h"
#endif
#if defined ZLS30701_INCLUDED
#include "zl303xx_AddressMap70x.h"
#endif
#if defined ZLS30721_INCLUDED
#include "zl303xx_AddressMap72x.h"
#endif
#if defined ZLS30751_INCLUDED
#include "zl303xx_AddressMap75x.h"
#endif
#if defined ZLS30771_INCLUDED
#include "zl303xx_AddressMap77x.h"
#endif
#include "zl303xx_AddressMap.h"


/*****************   DEFINES   ************************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/
#if defined ZLS30361_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
static void strReverse(Uint8T *begin, Uint8T *end);
#endif

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DECLARATIONS   *************************/
zlStatusE cpuChipSelectFn(Uint32T par, zl303xx_BooleanE enable);

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

#if !defined(ZL_I2C_RDWR)
/* cpuSpiRead */
/**
   Stub API to perform a low level SPI read from the ZL303XX_ device. This
   function is called by zl303xx_ReadLow(), which expects multi-byte registers to
   be returned in least significant byte (LSB) order.

  Parameters:
   [in]   params     Pointer to the device instance parameter structure
   [in]   addr       Device register address to access
   [out]  readValue  Buffer to return read data (LSB)
   [in]   bytes      Number of bytes to access

  Return Value:  cpuStatusE

*******************************************************************************/
cpuStatusE cpuSpiRead(void *params, Uint32T addr, Uint8T *readValue, Uint16T bytes)
{
   Uint32T address = 0;
   Uint32T page;
   page = 0;

#ifdef OS_LINUX
   zl303xx_ParamsS *zl303xx_Params = params;
#else
   if (params) {;} /* warning removal */
#endif

   (void) addr; /* warning removal */
   (void) readValue; /* warning removal */
   (void) bytes; /* warning removal */

   /* Page and address decoding from virtual address */
#if defined ZLS30721_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_72X(addr);
      }
#endif
#if defined ZLS30361_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_36X(addr);
         page = ZL303XX_MEM_PAGE_EXTRACT_36X(addr);
      }
#endif
#if defined ZLS30701_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_70X(addr);
      }
#endif
#if defined ZLS30751_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_75X(addr);
      }
#endif
#if defined ZLS30341_INCLUDED
      if(( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE )
        )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT(addr);
         page = ZL303XX_MEM_PAGE_EXTRACT(addr);
      }
#endif
#if defined ZLS30771_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_77X(addr);
      }
#endif


#ifdef OS_LINUX
   if (page != zl303xx_Params->spiParams.currentPage)
   {
      /* >>> Write value of 'page' to register 0x7F <<< */
      zl303xx_Params->spiParams.currentPage = page;
   }
#endif

   /* >>> Read 'bytes' starting at register 'address' into 'readValue' <<< */
   (void)address;   /* Warning removal */
   /* #warning MSCC: cpuSpiRead() missing */

#if defined ZLS30361_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
      {
         strReverse(readValue, readValue + bytes - 1);
      }
#endif
#if defined ZLS30701_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
      {
         strReverse(readValue, readValue + bytes - 1);
      }
#endif
#if defined ZLS30751_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
      {
         strReverse(readValue, readValue + bytes - 1);
      }
#endif
#if defined ZLS30771_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
      {
         strReverse(readValue, readValue + bytes - 1);
      }
#endif

   return CPU_OK;
}

/* cpuSpiWrite */
/**
   Stub API to perform a low level SPI write to the ZL303XX_ device. This function
   is called by zl303xx_WriteLow(), which passes multi-byte registers in least
   significant byte (LSB) order.

  Parameters:
   [in]   params  Pointer to the device instance parameter structure
   [in]   addr    Device register address to access
   [in]   data    Buffer to write to the device (LSB)
   [in]   bytes   Number of bytes to access

  Return Value:  cpuStatusE

*******************************************************************************/
cpuStatusE cpuSpiWrite(void *params, Uint32T addr, Uint8T *data, Uint16T bytes)
{
   Uint32T address = 0;
   Uint32T page;
   page = 0;

#ifdef OS_LINUX
   zl303xx_ParamsS *zl303xx_Params = params;
#else
   if (params) {;} /* warning removal */
#endif

   (void) addr; /* warning removal */
   (void) data; /* warning removal */
   (void) bytes; /* warning removal */

   /* Page and address decoding from virtual address */
#if defined ZLS30721_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_72X(addr);
      }
#endif
#if defined ZLS30361_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_36X(addr);
         page = ZL303XX_MEM_PAGE_EXTRACT_36X(addr);
      }
#endif
#if defined ZLS30701_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_70X(addr);
      }
#endif
#if defined ZLS30751_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_75X(addr);
      }
#endif
#if defined ZLS30341_INCLUDED
      if(( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE )
        )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT(addr);
         page = ZL303XX_MEM_PAGE_EXTRACT(addr);
      }
#endif
#if defined ZLS30771_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_77X(addr);
      }
#endif

#if defined ZLS30361_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
      {
         strReverse(data, data + bytes - 1);
      }
#endif
#if defined ZLS30701_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
      {
         strReverse(data, data + bytes - 1);
      }
#endif
#if defined ZLS30751_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
      {
         strReverse(data, data + bytes - 1);
      }
#endif
#if defined ZLS30771_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
      {
         strReverse(data, data + bytes - 1);
      }
#endif

#ifdef OS_LINUX
   if (page != zl303xx_Params->spiParams.currentPage)
   {
      /* >>> Write value of 'page' to register 0x7F <<< */
      zl303xx_Params->spiParams.currentPage = page;
   }
#endif

   /* >>> Write 'data' buffer to register 'address' <<< */
   (void)address;   /* Warning removal */
   /* #warning MSCC: cpuSpiWrite() missing */

   return CPU_OK;
}

/* cpuConfigSpi */
/**
   Stub API to initialises the SPI interface.

  Parameters:
      None

  Return Value:  cpuStatusE

*******************************************************************************/
cpuStatusE cpuConfigSpi(void)
{
   /* SPI init code may be done in the kernel! */
   /* #warning MSCC: cpuConfigSpi() missing */
   return CPU_OK;
}

/* cpuSpiCleanUp */
/**
   Stub API to clean up the SPI interface (includes related structures and
   interrupts).

  Parameters:
      None

  Return Value:  cpuStatusE

*******************************************************************************/
cpuStatusE cpuSpiCleanUp(void)
{
   /* SPI init code may be done in the kernel! */
   /* #warning MSCC: cpuSpiCleanUp() missing */
   return CPU_OK;
}

#ifdef OS_VXWORKS
/* cpuSpiIsrHandler */
/**
   When an SPI packet is sent or received, an interrupt is generated in the
   <blank> register. The corresponding ISR clears the interrupt and gives a
   semaphore so any task waiting on a Rx or Tx can continue.

  Parameters:
      None

  Return Value:  None

*******************************************************************************/
void cpuSpiIsrHandler(void)
{
   /* #warning MSCC: cpuSpiIsrHandler() missing */
   return;
}
#endif

#else   /* !ZL_I2C_RDWR */
/* cpuI2CRead */
/**
   Stub API to perform a low level I2C read from the ZL303XX_ device. This
   function is called by zl303xx_ReadLow(), which expects multi-byte registers to
   be returned in least significant byte (LSB) order.

  Parameters:
   [in]   params     Pointer to the device instance parameter structure
   [in]   addr       Device register address to access
   [out]  readValue  Buffer to return read data (LSB)
   [in]   bytes      Number of bytes to access

  Return Value:  cpuStatusE

*******************************************************************************/
cpuStatusE cpuI2CRead(void *params, Uint32T addr, Uint8T *readValue, Uint16T bytes)
{
   /* #warning MSCC: cpuI2CRead() missing */

   Uint32T address, page = 0;
#ifdef OS_LINUX
   zl303xx_ParamsS *zl303xx_Params = params;
#endif

   /* Page and address decoding from virtual address */
#if defined ZLS30721_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_72X(addr);
      }
#endif
#if defined ZLS30361_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_36X(addr);
         page = ZL303XX_MEM_PAGE_EXTRACT_36X(addr);
      }
#endif
#if defined ZLS30701_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_70X(addr);
      }
#endif
#if defined ZLS30751_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_75X(addr);
      }
#endif
#if defined ZLS30341_INCLUDED
      if(( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE )
        )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT(addr);
         page = ZL303XX_MEM_PAGE_EXTRACT(addr);
      }
#endif
#if defined ZLS30771_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_77X(addr);
      }
#endif

#ifdef OS_LINUX
   if (page != zl303xx_Params->spiParams.currentPage)
   {
      /* >>> Write value of 'page' to register 0x7F <<< */
      zl303xx_Params->spiParams.currentPage = page;
   }
#endif

   /* >>> Read 'bytes' starting at register 'address' into 'readValue' <<< */

#if defined ZLS30361_INCLUDED
   if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
   {
      /* The ZL3036x is an MSB device, so reverse the bytes because zl303xx_ReadLow()
       * expects LSB order. */
      strReverse(readValue, readValue + bytes - 1);
   }
#endif
#if defined ZLS30701_INCLUDED
   if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
   {
      /* The ZL3070x is an MSB device, so reverse the bytes because zl303xx_ReadLow()
       * expects LSB order. */
      strReverse(readValue, readValue + bytes - 1);
   }
#endif
#if defined ZLS30751_INCLUDED
   if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
   {
      /* The ZL3075x is an MSB device, so reverse the bytes because zl303xx_ReadLow()
       * expects LSB order. */
      strReverse(readValue, readValue + bytes - 1);
   }
#endif
#if defined ZLS30771_INCLUDED
   if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
   {
      /* The ZL3077x is an MSB device, so reverse the bytes because zl303xx_ReadLow()
       * expects LSB order. */
      strReverse(readValue, readValue + bytes - 1);
   }
#endif

   return CPU_OK;
}

/* cpuI2CWrite */
/**
   Stub API to perform a low level I2C write to the ZL303XX_ device. This function
   is called by zl303xx_WriteLow(), which passes multi-byte registers in least
   significant byte (LSB) order.

  Parameters:
   [in]   params  Pointer to the device instance parameter structure
   [in]   addr    Device register address to access
   [in]   data    Pointer to the result
   [in]   bytes   Number of bytes to access

  Return Value:  cpuStatusE

*******************************************************************************/
cpuStatusE cpuI2CWrite(void *params, Uint32T addr, Uint8T *data, Uint16T bytes)
{
   /* #warning MSCC: cpuI2CWrite() missing */

   Uint32T address, page = 0;
#ifdef OS_LINUX
   zl303xx_ParamsS *zl303xx_Params = params;
#endif

   /* Page and address decoding from virtual address */
#if defined ZLS30721_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_72X(addr);
      }
#endif
#if defined ZLS30361_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_36X(addr);
         page = ZL303XX_MEM_PAGE_EXTRACT_36X(addr);
      }
#endif
#if defined ZLS30701_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_70X(addr);
      }
#endif
#if defined ZLS30751_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_75X(addr);
      }
#endif
#if defined ZLS30341_INCLUDED
      if(( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE )
        )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT(addr);
         page = ZL303XX_MEM_PAGE_EXTRACT(addr);
      }
#endif
#if defined ZLS30771_INCLUDED
      if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
      {
         address = ZL303XX_MEM_ADDR_EXTRACT_77X(addr);
      }
#endif

#if defined ZLS30361_INCLUDED
   if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
   {
      /* The ZL3036x is an MSB device, so reverse the bytes because zl303xx_WriteLow()
       * provides data in LSB order. */
      strReverse(data, data + bytes - 1);
   }
#endif
#if defined ZLS30701_INCLUDED
   if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
   {
      /* The ZL3070x is an MSB device, so reverse the bytes because zl303xx_WriteLow()
       * provides data in LSB order. */
      strReverse(data, data + bytes - 1);
   }
#endif
#if defined ZLS30751_INCLUDED
   if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
   {
      /* The ZL3075x is an MSB device, so reverse the bytes because zl303xx_WriteLow()
       * provides data in LSB order. */
      strReverse(data, data + bytes - 1);
   }
#endif
#if defined ZLS30771_INCLUDED
   if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
   {
      /* The ZL3077x is an MSB device, so reverse the bytes because zl303xx_WriteLow()
       * provides data in LSB order. */
      strReverse(data, data + bytes - 1);
   }
#endif

#ifdef OS_LINUX
   if (page != zl303xx_Params->spiParams.currentPage)
   {
      /* >>> Write value of 'page' to register 0x7F <<< */
      zl303xx_Params->spiParams.currentPage = page;
   }
#endif

   /* >>> Write 'data' buffer to register 'address' <<< */

   return CPU_OK;
}

#endif  /* ZL_I2C_RDWR */

/* cpuChipSelectFn */
/**
   Callback function to select or release the specified SPI chip select pin.
   Associated with a device in cpuConfigChipSelect().

  Parameters:
   [in]   par     Predefined SPI chip select pin.
   [in]   enable  Boolean value to select (ZL303XX_TRUE) or release
                       (ZL303XX_FALSE) the csPin.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE cpuChipSelectFn(Uint32T par, zl303xx_BooleanE enable)
{
   /* #warning MSCC: cpuChipSelectFn() missing */

   (void)par; (void)enable;  /* Warning removal */
   return ZL303XX_ERROR;
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/
#if defined ZLS30361_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
static void strReverse(Uint8T *begin, Uint8T *end)
{
   Uint8T aux;

   while(end > begin)
   {
      aux = *end;
      *end-- = *begin;
      *begin++ = aux;
   }
}
#endif

/*****************   END   ****************************************************/
