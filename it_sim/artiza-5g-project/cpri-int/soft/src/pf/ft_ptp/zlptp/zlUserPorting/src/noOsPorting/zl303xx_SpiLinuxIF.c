

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Implements a Linux API to SPI bus interface for the SDK.
*
*******************************************************************************/


/*****************   INCLUDE FILES   ******************************************/

#ifdef OS_LINUX


#include "zl303xx_Os.h"
#include "zl303xx_ErrTrace.h"
#include "zl303xx_Porting.h"
#include "zl303xx_SpiPort.h"
#include "zl303xx_OsHeaders.h"
#include "zl303xx_RdWr.h"
#include "zl303xx_SpiZLDevLimits.h"
#include "zl303xx_Macros.h"

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


#include <endian.h>
#include <stdio.h>
#include <fcntl.h>

/*****************   DEFINES   ************************************************/
/*#define DEBUG*/    /* Logging */
/*#define DEBUG2*/

#if defined (DEBUG) || defined (DEBUG2)
    #define debugf(fmt,__VARARGS_) printf(fmt ,##args)
#else
    #define debugf(fmt,__VARARGS_)
#endif


/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   GLOBAL VARIABLE ******************/

/* API is protected by a mutex in zl303xx_RdWr.c */
static Uint16T *currPage = NULL;

#if defined ZLS30361_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
static void strReverse(Uint8T* begin, Uint8T* end);
#endif

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

static SINT_T readDeviceOnSPI(void *params, SINT_T fdOfDev, SINT_T address,
                              char page, SINT_T numBytes, char *value_arr);
static SINT_T writeDeviceOnSPI(void *params, SINT_T fdOfDev, SINT_T address,
                               char page, SINT_T numBytes, char *value_arr);

/*****************  FUNCTION PROTOTYPES ***************************************/

/*

  Function Name:
   readDeviceOnSPI

  Details:
   API interface to the ZL303XX_ Device on the SPI using the Linux driver

  Parameters:
   [in]   params          Pointer to the device instance parameter structure
   [in]   fdOfDev         File descriptor number of open /dev/blahXX
   [in]   address         Device register address to access
   [in]   page            Device page to use
   [in]   numBytes        Number of bytes to access
   [in]   value_arr       Pointer to value_arr for result

  Return Value:
   signed int

*******************************************************************************/
static SINT_T readDeviceOnSPI(void *params, SINT_T fdOfDev, SINT_T address,
                              char page, SINT_T numBytes, char *value_arr)
{
    int bytes = 0;
    int indx = 0;
    char bytes2Move;
    char buffer[DRV_LIMIT + 3] = {0};   /* Limit in driver + num address bytes
                                           of our most demanding device */
    int max_pages, min_paged_addr, max_paged_addr, page_sel_addr, spi_w_bit_mask, spi_r_bit_mask;
    int num_addr_bytes;
#if defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30721_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
    zl303xx_ParamsS *zl303xx_Params = params;
#endif
#if defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
    int pageChange;
#endif

    if (!params)
    {
        fprintf(stderr, "params is NULL!\n");
        return -ENODEV;
    }

    /* Configure settings for different devices */

    /* Set default values */
    max_pages = MAX_PAGES;
    min_paged_addr = MIN_PAGED_ADDR;
    max_paged_addr = MAX_PAGED_ADDR;
    spi_w_bit_mask = SPI_W_BIT;
    spi_r_bit_mask = SPI_R_BIT;
    page_sel_addr = (char)DEVICE_PAGE_SEL_REG_ADDR;
    num_addr_bytes = 1;

#if defined ZLS30361_INCLUDED
    if(zl303xx_Params->deviceType == ZL3036X_DEVICETYPE)
    {
        /* ZL3036X device */
        max_pages = MAX_PAGES_36X;
        min_paged_addr = MIN_PAGED_ADDR_36X;
        max_paged_addr = MAX_PAGED_ADDR_36X;
        spi_w_bit_mask = SPI_W_BIT;
        spi_r_bit_mask = SPI_R_BIT;
        page_sel_addr = (char)DEVICE_PAGE_SEL_REG_ADDR_36X;
        num_addr_bytes = 1;
    }
#endif
#if defined ZLS30721_INCLUDED
    if(zl303xx_Params->deviceType == ZL3072X_DEVICETYPE)
    {
        /* ZL3072X device */
        max_pages = 0;
        min_paged_addr = 0;
        max_paged_addr = 0;
        spi_w_bit_mask = SPI_W_BIT;
        spi_r_bit_mask = SPI_R_BIT;
        page_sel_addr = 0;
        num_addr_bytes = 3;
    }
#endif
#if defined ZLS30701_INCLUDED
    if(zl303xx_Params->deviceType == ZL3070X_DEVICETYPE)
    {
        /* ZL3070X device */
        max_pages = MAX_PAGES_70X;
        min_paged_addr = MIN_PAGED_ADDR_70X;
        max_paged_addr = MAX_PAGED_ADDR_70X;
        spi_w_bit_mask = SPI_W_BIT;
        spi_r_bit_mask = SPI_R_BIT;
        page_sel_addr = (char)DEVICE_PAGE_SEL_REG_ADDR_70X;
        num_addr_bytes = 1;
    }
#endif
#if defined ZLS30751_INCLUDED
    if(zl303xx_Params->deviceType == ZL3075X_DEVICETYPE)
    {
        /* ZL3075X device */
        max_pages = MAX_PAGES_75X;
        min_paged_addr = MIN_PAGED_ADDR_75X;
        max_paged_addr = MAX_PAGED_ADDR_75X;
        spi_w_bit_mask = SPI_W_BIT;
        spi_r_bit_mask = SPI_R_BIT;
        page_sel_addr = (char)DEVICE_PAGE_SEL_REG_ADDR_75X;
        num_addr_bytes = 1;
    }
#endif
#if defined ZLS30771_INCLUDED
    if(zl303xx_Params->deviceType == ZL3077X_DEVICETYPE)
    {
        /* ZL3077X device */
        max_pages = MAX_PAGES_77X;
        min_paged_addr = MIN_PAGED_ADDR_77X;
        max_paged_addr = MAX_PAGED_ADDR_77X;
        spi_w_bit_mask = SPI_W_BIT;
        spi_r_bit_mask = SPI_R_BIT;
        page_sel_addr = (char)DEVICE_PAGE_SEL_REG_ADDR_77X;
        num_addr_bytes = 1;
    }
#endif

    if (fdOfDev <= 0)
    {
        fprintf(stderr, "File descriptor <= 0!\n");
        return -ENODEV;
    }

    if (numBytes > DRV_LIMIT)
    {
        fprintf(stderr, "numBytes=%d exceeds DRV_LIMIT maximum of %d!\n", numBytes, DRV_LIMIT);
        return -EFAULT;
    }

    if (page > 0)
    {
        if (page > max_pages)
        {
            fprintf(stderr, "Page requested=%d exceeds maximum of %d!\n", page, max_pages);
            return -EINVAL;
        }

        /* if is not bridge-configurable data */
        if ((address < min_paged_addr || address > max_paged_addr) && page != 0xB)
        {
            fprintf(stderr, "Addr=0x%x, Exceeds valid range of paged addresses (0x%x - 0x%x)!\n", address, min_paged_addr, max_paged_addr);
            return -EINVAL;
        }
    }

#if defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
    /* Assume the page does not need to be changed. */
    pageChange = 0;

#if defined ZLS30341_INCLUDED
    if(( zl303xx_Params->deviceType == ZL3034X_DEVICETYPE )
      )
    {
        if (page != *currPage && page <= max_pages)
        {
            /* Make a page change */
            pageChange = 1;
        }
    }
#endif
#if defined ZLS30361_INCLUDED
    if(zl303xx_Params->deviceType == ZL3036X_DEVICETYPE)
    {
        /* Always force a page change */
        if(page <= max_pages)
        {
            pageChange = 1;
        }
    }
#endif
#if defined ZLS30701_INCLUDED
    if(zl303xx_Params->deviceType == ZL3070X_DEVICETYPE)
    {
        /* Always force a page change */
        if(page <= max_pages)
        {
            pageChange = 1;
        }
    }
#endif
#if defined ZLS30751_INCLUDED
    if(zl303xx_Params->deviceType == ZL3075X_DEVICETYPE)
    {
        /* Always force a page change */
        if(page <= max_pages)
        {
            pageChange = 1;
        }
    }
#endif
#if defined ZLS30771_INCLUDED
    if(zl303xx_Params->deviceType == ZL3077X_DEVICETYPE)
    {
        /* Always force a page change */
        if(page <= max_pages)
        {
            pageChange = 1;
        }
    }
#endif

    if( pageChange == 1 )
    {
        *currPage = page;
        buffer[indx] = page_sel_addr & spi_w_bit_mask;                         /* Tell Device to write */
        memcpy((void*)&buffer[++indx], (void*)&page, 1);
        bytes2Move = indx+1;
#ifdef DEBUG2
        {
            int i;

            debugf("buffer[i]=0x ");
            for (i = 0; i < bytes2Move; i++)
            {
                debugf("%.2X ", buffer[i]);
            }
            debugf("\n");
        }
#endif
        if ((bytes = write(fdOfDev, buffer, bytes2Move)) < 0)
        {
            fprintf(stderr, "Paged write on SPI read failed!\n");
            return -EIO;
        }
    }
#endif

    indx = 0;                               /* Normal case */
    memset(&buffer, 0, sizeof(buffer));

#if defined ZLS30721_INCLUDED
    if(zl303xx_Params->deviceType == ZL3072X_DEVICETYPE)
    {
        buffer[indx++] = (char)SPI_R_BYTE_72X;                           /* Tell Device to read */
        buffer[indx++] = (char)(address >> 8);                           /* specify the address MSB to read */
        buffer[indx++] = (char)(address & 0xFF);                         /* specify the address LSB to read */
    }
#endif
    if(zl303xx_Params->deviceType != ZL3072X_DEVICETYPE)
    {
        buffer[indx++] = (char)address | spi_r_bit_mask;                 /* Tell Device to read */
    }

    indx += numBytes;

    bytes2Move = indx;

#ifdef DEBUG2
    {
        int i;

        printf("Send buffer[i]=0x ");
        for (i = 0; i < bytes2Move; i++)
        {
            printf("%.2X ", buffer[i]);
        }
        printf("\n");
    }
#endif

    if ((bytes = read(fdOfDev, &buffer[0], bytes2Move)) != bytes2Move)
    {
        fprintf(stderr, "Read Device on SPI failed! Errno=%d\n", errno);
        return bytes;   /* Return code */
    }

    memset((void*)value_arr, 0, (Uint32T)numBytes);           /* Clean answer */
    memcpy((void*)value_arr, (void*)&buffer[num_addr_bytes],
                              (Uint32T)bytes-num_addr_bytes); /* Ignore address */

#ifdef DEBUG2
    {
        int i;

        printf("Returned value_arr[i]=0x ");
        for (i = 0; i < bytes-num_addr_bytes; i++)
        {
            printf("%.2X ", value_arr[i]);
        }
        printf("\n");
    }
#endif

    return bytes-num_addr_bytes;   /* Ignore address */
}

/*

  Function Name:
   writeDeviceOnSPI

  Details:
   API interface to the ZL303XX_ Device on the SPI using the Linux driver

  Parameters:
   [in]   params          Pointer to the device instance parameter structure
   [in]   fdOfDev         File descriptor number of open /dev/blahXX
   [in]   address         Device register address to access
   [in]   page            Device page to use
   [in]   numBytes        Number of bytes to access
   [out]  value_arr       Pointer to value_arr for result

  Return Value:
   signed int

*******************************************************************************/
static SINT_T writeDeviceOnSPI(void *params, SINT_T fdOfDev, SINT_T address,
                               char page, SINT_T numBytes, char *value_arr)
{
    Sint32T bytes = 0;
    Sint32T indx = 0;
    char bytes2Move;
    char buffer[DRV_LIMIT + 3] = {0};   /* Limit in driver + num address bytes
                                           of our most demanding device */
    int max_pages, min_paged_addr, max_paged_addr, page_sel_addr, spi_w_bit_mask, spi_r_bit_mask;
    int num_addr_bytes;
#if defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30721_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
    zl303xx_ParamsS *zl303xx_Params = params;
#endif
#if defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
    int pageChange;
#endif

    if (!params)
    {
        fprintf(stderr, "params is NULL!\n");
        return -ENODEV;
    }

    /* Configure settings for different devices */

    /* Set default values */
    max_pages = MAX_PAGES;
    min_paged_addr = MIN_PAGED_ADDR;
    max_paged_addr = MAX_PAGED_ADDR;
    spi_w_bit_mask = SPI_W_BIT;
    spi_r_bit_mask = SPI_R_BIT;
    page_sel_addr = (char)DEVICE_PAGE_SEL_REG_ADDR;
    num_addr_bytes = 1;
    currPage = &zl303xx_Params->spiParams.currentPage;

#if defined ZLS30361_INCLUDED
    if(zl303xx_Params->deviceType == ZL3036X_DEVICETYPE)
    {
        /* ZL3036X device */
        max_pages = MAX_PAGES_36X;
        min_paged_addr = MIN_PAGED_ADDR_36X;
        max_paged_addr = MAX_PAGED_ADDR_36X;
        spi_w_bit_mask = SPI_W_BIT;
        spi_r_bit_mask = SPI_R_BIT;
        page_sel_addr = (char)DEVICE_PAGE_SEL_REG_ADDR_36X;
        num_addr_bytes = 1;
    }
#endif
#if defined ZLS30721_INCLUDED
    if(zl303xx_Params->deviceType == ZL3072X_DEVICETYPE)
    {
        /* ZL3072X device */
        max_pages = 0;
        min_paged_addr = 0;
        max_paged_addr = 0;
        spi_w_bit_mask = SPI_W_BIT;
        spi_r_bit_mask = SPI_R_BIT;
        page_sel_addr = 0;
        num_addr_bytes = 3;
    }
#endif
#if defined ZLS30701_INCLUDED
    if(zl303xx_Params->deviceType == ZL3070X_DEVICETYPE)
    {
        /* ZL3070X device */
        max_pages = MAX_PAGES_70X;
        min_paged_addr = MIN_PAGED_ADDR_70X;
        max_paged_addr = MAX_PAGED_ADDR_70X;
        spi_w_bit_mask = SPI_W_BIT;
        spi_r_bit_mask = SPI_R_BIT;
        page_sel_addr = (char)DEVICE_PAGE_SEL_REG_ADDR_70X;
        num_addr_bytes = 1;
    }
#endif
#if defined ZLS30751_INCLUDED
    if(zl303xx_Params->deviceType == ZL3075X_DEVICETYPE)
    {
        /* ZL3075X device */
        max_pages = MAX_PAGES_75X;
        min_paged_addr = MIN_PAGED_ADDR_75X;
        max_paged_addr = MAX_PAGED_ADDR_75X;
        spi_w_bit_mask = SPI_W_BIT;
        spi_r_bit_mask = SPI_R_BIT;
        page_sel_addr = (char)DEVICE_PAGE_SEL_REG_ADDR_75X;
        num_addr_bytes = 1;
    }
#endif
#if defined ZLS30771_INCLUDED
    if(zl303xx_Params->deviceType == ZL3077X_DEVICETYPE)
    {
        /* ZL3077X device */
        max_pages = MAX_PAGES_77X;
        min_paged_addr = MIN_PAGED_ADDR_77X;
        max_paged_addr = MAX_PAGED_ADDR_77X;
        spi_w_bit_mask = SPI_W_BIT;
        spi_r_bit_mask = SPI_R_BIT;
        page_sel_addr = (char)DEVICE_PAGE_SEL_REG_ADDR_77X;
        num_addr_bytes = 1;
    }
#endif

    if (fdOfDev <= 0)
    {
        fprintf(stderr, "File descriptor <= 0!\n");
        return -ENODEV;
    }

    if (numBytes > DRV_LIMIT)
    {
        fprintf(stderr, "numBytes=%d exceeds DRV_LIMIT maximum of %d!\n", numBytes, DRV_LIMIT);
        return -EFAULT;
    }

    if (page > 0)
    {
        if (page > max_pages)
        {
            fprintf(stderr, "Page requested=%d exceeds maximum of %d!\n", page, max_pages);
            return -EINVAL;
        }

         /* if is not bridge-configurable data */
        if ((address < min_paged_addr || address > max_paged_addr) && page != 0xB)
        {
            fprintf(stderr, "Addr=0x%x, Exceeds valid range of paged addresses (0x%x - 0x%x)!\n", address, min_paged_addr, max_paged_addr);
            return -EINVAL;
        }
    }

#if defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
    /* Assume the page does not need to be changed. */
    pageChange = 0;

#if defined ZLS30341_INCLUDED
    if(( zl303xx_Params->deviceType == ZL3034X_DEVICETYPE )
      )
    {
        if (page != *currPage && page <= max_pages)
        {
            /* Make a page change */
            pageChange = 1;
        }
    }
#endif
#if defined ZLS30361_INCLUDED
    if(zl303xx_Params->deviceType == ZL3036X_DEVICETYPE)
    {
        /* Always force a page change */
        if(page <= max_pages)
        {
            pageChange = 1;
        }
    }
#endif
#if defined ZLS30701_INCLUDED
    if(zl303xx_Params->deviceType == ZL3070X_DEVICETYPE)
    {
        /* Always force a page change */
        if(page <= max_pages)
        {
            pageChange = 1;
        }
    }
#endif
#if defined ZLS30751_INCLUDED
    if(zl303xx_Params->deviceType == ZL3075X_DEVICETYPE)
    {
        /* Always force a page change */
        if(page <= max_pages)
        {
            pageChange = 1;
        }
    }
#endif
#if defined ZLS30771_INCLUDED
    if(zl303xx_Params->deviceType == ZL3077X_DEVICETYPE)
    {
        /* Always force a page change */
        if(page <= max_pages)
        {
            pageChange = 1;
        }
    }
#endif

    if( pageChange == 1 )
    {
        *currPage = page;
        buffer[indx] = (char)page_sel_addr & spi_w_bit_mask;       /* Tell Device to write */
        memcpy((void*)&buffer[++indx], (void*)&page, 1);
        bytes2Move = indx+1;
#ifdef DEBUG2
        {
            Sint32T i;

            debugf("buffer[i]=0x ");
            for (i = 0; i <bytes2Move; i++)
            {
                debugf("%.2X ", buffer[i]);
            }
            debugf("\n");
        }
#endif
        if ((bytes = write(fdOfDev, buffer, bytes2Move)) < 0)
        {
            fprintf(stderr, "Paged write on SPI write failed!\n");
            return -EIO;
        }
    }
#endif

    indx = 0;                                       /* Normal case */
    memset(&buffer, 0, sizeof(buffer));
    buffer[indx] = (char)address & spi_w_bit_mask;  /*  Tell Device to write */
#if defined ZLS30721_INCLUDED
    if(zl303xx_Params->deviceType == ZL3072X_DEVICETYPE)
    {
        buffer[indx++] = (char)SPI_W_BYTE_72X;      /* Tell Device to write */
        buffer[indx++] = (char)(address >> 8);      /* specify the address MSB to read */
        buffer[indx] = (char)(address & 0xFF);      /* specify the address LSB to read */
    }
#endif
    memcpy((void*)&buffer[++indx], (void*)value_arr, (Uint32T)numBytes);
    indx += numBytes;

    bytes2Move = indx;

#ifdef DEBUG2
    {
        Sint32T i;

        printf("write buffer[i]=0x ");
        for (i = 0; i < bytes2Move; i++)
        {
            printf("%.2X ", buffer[i]);
        }
        printf("\n");
    }
#endif


    if ((bytes = write(fdOfDev, &buffer[0], bytes2Move)) != bytes2Move)
    {
        fprintf(stderr, "Write Device on SPI failed! Errno=%d\n", errno);
        return bytes;   /* Return code */
    }

    return bytes-num_addr_bytes; /* Ignore address */
}

/*

  Function Name:
   cpuSpiRead

  Details:
   Linux wrapper API for low level spi read of the ZL303XX_ Device using the Linux driver.
   Code that does not use this wrapper could be more efficient if it opened the device once and
   then did multiple operations.

  Parameters:
   [in]   params          Pointer to the device instance parameter structure
   [in]   addr            Device register address to access
   [in]   readValue       Pointer to the result
   [in]   bytes           Number of bytes to access

  Return Value:
   cpuStatusE

*******************************************************************************/

cpuStatusE cpuSpiRead(void *params, Uint32T addr, Uint8T *readValue, Uint16T bytes)
{
    zl303xx_ParamsS *zl303xx_Params = params;
    Sint32T fdOfDevCS, status = 0;
    char  page = 0, overlay = 0, regSize = 0;
/*    char value_arr[DRV_LIMIT] = {0}; */
    Uint32T address = 0;


#ifdef DEBUG2
    if (zl303xx_Params == NULL)
    {
        fprintf(stderr, "cpuSpiRead: Error (zl303xx_Params) when trying to write address=0x%X\n", addr);
    }
    if (readValue == NULL)
    {
        fprintf(stderr, "cpuSpiRead: Error (data) when trying to write address=0x%X\n", addr);
    }
    if (DRV_LIMIT < bytes)
    {
        fprintf(stderr, "cpuSpiRead: Error (too many bytes) when trying to write address=0x%X\n", addr);
        return (-EFAULT);
    }
#endif

    if (ZL303XX_CHECK_POINTERS(zl303xx_Params, readValue))
    {
        fprintf(stderr, "cpuSpiRead(): invalid pointer!\n");
        return ((signed int)ZL303XX_INVALID_POINTER);
    }

    /* Convert address encoding! */
/*    debugf("addr was=0x%x ", addr); */

    /* Set default values */
    address = addr & 0xFF;              /* 8 bits @0 */
    page = (addr & 0xF00) >>8;          /* 4 bits @8 */

#if defined ZLS30361_INCLUDED
    if(zl303xx_Params->deviceType == ZL3036X_DEVICETYPE)
    {
        /* 36x Page and address decoding from virtual address */
        address = ZL303XX_MEM_ADDR_EXTRACT_36X(addr);
        page = ZL303XX_MEM_PAGE_EXTRACT_36X(addr);
  #ifdef DEBUG
        printf("cpuSpiRead: 36X addr=0x%x == address=0x%x, page=0x%x\n", addr, address, page);
  #endif
    }
#endif
#if defined ZLS30721_INCLUDED
    if(zl303xx_Params->deviceType == ZL3072X_DEVICETYPE)
    {
        address = ZL303XX_MEM_ADDR_EXTRACT_72X(addr);
        page = 0;
    }
#endif
#if defined ZLS30701_INCLUDED
    if(zl303xx_Params->deviceType == ZL3070X_DEVICETYPE)
    {
        /* 70x Page and address decoding from virtual address */
        address = ZL303XX_MEM_ADDR_EXTRACT_70X(addr);
        page = ZL303XX_MEM_PAGE_EXTRACT_70X(addr);
  #ifdef DEBUG
        printf("cpuSpiRead: 36X addr=0x%x == address=0x%x, page=0x%x\n", addr, address, page);
  #endif
    }
#endif
#if defined ZLS30751_INCLUDED
    if(zl303xx_Params->deviceType == ZL3075X_DEVICETYPE)
    {
        /* 75x Page and address decoding from virtual address */
        address = ZL303XX_MEM_ADDR_EXTRACT_75X(addr);
        page = ZL303XX_MEM_PAGE_EXTRACT_75X(addr);
    }
#endif
#if defined ZLS30771_INCLUDED
    if(zl303xx_Params->deviceType == ZL3077X_DEVICETYPE)
    {
        /* 77x Page and address decoding from virtual address */
        address = ZL303XX_MEM_ADDR_EXTRACT_77X(addr);
        page = ZL303XX_MEM_PAGE_EXTRACT_77X(addr);
    }
#endif

    regSize = (addr & 0x3F00) >>12;     /* 6 bits @ 12 */
    overlay =  (addr & 0xC0000) >>18;   /* 2 bits @ 18*/

    if ((fdOfDevCS = open((const char *) zl303xx_Params->spiParams.linuxChipSelectDevName, O_RDWR)) < 0)
    {
        fprintf(stderr, "cpuSpiRead(): open failed on %s; errno=%d\n", zl303xx_Params->spiParams.linuxChipSelectDevName, errno);
        return -ENOENT;
    }

//printf( "OPEN chip select success on %s for address=%x bytes=%d\n", zl303xx_Params->spiParams.linuxChipSelectDevName, address, bytes);

    currPage = &zl303xx_Params->spiParams.currentPage;

    if ((status = readDeviceOnSPI(params, fdOfDevCS, (Sint32T)address, page, bytes, (char*)readValue)) < 0)
    {
        fprintf(stderr, "cpuSpiRead(): call to readDeviceOnSPI() failed! return=%d\n", bytes);
        switch (status)  /* Mimic current behaviour */
        {
            case -SPI_SEM_TAKE_FAIL:
                status = SPI_SEM_TAKE_FAIL;
                break;
            case -SPI_SEM_GIVE_FAIL:
                status = SPI_SEM_GIVE_FAIL;
                break;
            default:
                ZL303XX_TRACE_ALWAYS("cpuSpiRead: Unhandled status return=%d", status, 0,0,0,0,0);
        }
    }
    else
    {
        bytes = (Uint16T) status;       /* status contains num bytes read */
        status = ZL303XX_OK;

#if defined ZLS30361_INCLUDED
        if(zl303xx_Params->deviceType == ZL3036X_DEVICETYPE)
        {
            /*debugf("Read page=0x%x, addr=0x%x, regSize=0x%x overlay=0x%x, %p\n", page, address, regSize, overlay, addr); */
            strReverse(readValue, readValue+bytes-1);   /* Reverse the byte write order for MSB device */
        }
#endif
#if defined ZLS30701_INCLUDED
        if(zl303xx_Params->deviceType == ZL3070X_DEVICETYPE)
        {
            /*debugf("Read page=0x%x, addr=0x%x, regSize=0x%x overlay=0x%x, %p\n", page, address, regSize, overlay, addr); */
            strReverse(readValue, readValue+bytes-1);   /* Reverse the byte write order for MSB device */
        }
#endif
#if defined ZLS30751_INCLUDED
        if(zl303xx_Params->deviceType == ZL3075X_DEVICETYPE)
        {
            /*debugf("Read page=0x%x, addr=0x%x, regSize=0x%x overlay=0x%x, %p\n", page, address, regSize, overlay, addr); */
            strReverse(readValue, readValue+bytes-1);   /* Reverse the byte write order for MSB device */
        }
#endif
#if defined ZLS30771_INCLUDED
        if(zl303xx_Params->deviceType == ZL3077X_DEVICETYPE)
        {
            /*debugf("Read page=0x%x, addr=0x%x, regSize=0x%x overlay=0x%x, %p\n", page, address, regSize, overlay, addr); */
            strReverse(readValue, readValue+bytes-1);   /* Reverse the byte write order for MSB device */
        }
#endif
    }

    if (close(fdOfDevCS) < 0)
    {
        fprintf(stderr, "cpuSpiRead(): close failed on %s!\n", zl303xx_Params->spiParams.linuxChipSelectDevName);
        return -ENOENT;
    }

#ifdef DEBUG2
    {
        Sint32T i;

        printf("LSB readValue[i]=0x ");
        for (i = 0; i < bytes; i++)
        {
            printf("%.2X ", readValue[i]);
        }
        printf("\n\n");
    }
#endif

    return status;
}

/*

  Function Name:
   cpuSpiWrite

  Details:
   Linux wrapper API for low level spi write of the ZL303XX_ Device using the Linux driver.
   Code that does not use this wrapper could be more efficient if it opened the device once and
   then did multiple operations.

  Parameters:
   [in]   params          Pointer to the device instance parameter structure
   [in]   addr            Device register address to access
   [in]   data            Pointer to the data
   [in]   bytes           Number of bytes to access

  Return Value:
   cpuStatusE

*******************************************************************************/
cpuStatusE cpuSpiWrite(void *params, Uint32T addr, Uint8T *data, Uint16T bytes)
{
    zl303xx_ParamsS *zl303xx_Params = params;
    Sint32T fdOfDevCS, status = 0;
    char page = 0, overlay = 0, regSize = 0;
/*    char value_arr[DRV_LIMIT] = {0}; */
    Uint32T address = 0;

#ifdef DEBUG2
    if (zl303xx_Params == NULL)
    {
        fprintf(stderr, "cpuSpiWrite: Error (zl303xx_Params) when trying to write address=0x%X\n", addr);
    }
    if (data == NULL)
    {
        fprintf(stderr, "cpuSpiWrite: Error (data) when trying to write address=0x%X\n", addr);
    }
    if (DRV_LIMIT < bytes)
    {
        fprintf(stderr, "cpuSpiWrite: Error (too many bytes) when trying to write address=0x%X\n", addr);
        return (-EFAULT);
    }
#endif

    if (ZL303XX_CHECK_POINTERS(zl303xx_Params, data))
    {
        fprintf(stderr, "cpuSpiWrite(): invalid pointer!\n");
        return ((signed int)ZL303XX_INVALID_POINTER);
    }


#ifdef DEBUG2
    {
        Sint32T i;
        char buffer[32];

        memcpy(&buffer, data, bytes);

        printf("LSB data[i]=0x ");
        for (i = 0; i < bytes; i++)
        {
            printf("%.2X ", buffer[i]);
        }
        printf("\n");
    }
#endif

    /* Convert address encoding! */
/*    debugf("addr was=0x%x ", addr); */

    /* Set default address and page */
    address = addr & 0xFF;              /* 8 bits @0 */
    page = (addr & 0xF00) >>8;          /* 4 bits @8 */

#if defined ZLS30361_INCLUDED
    if(zl303xx_Params->deviceType == ZL3036X_DEVICETYPE)
    {
        /* 36x Page and address decoding from virtual address */
        address = ZL303XX_MEM_ADDR_EXTRACT_36X(addr);
        page = ZL303XX_MEM_PAGE_EXTRACT_36X(addr);
    }
#endif
#if defined ZLS30721_INCLUDED
    if(zl303xx_Params->deviceType == ZL3072X_DEVICETYPE)
    {
        /* 36x Page and address decoding from virtual address */
        address = ZL303XX_MEM_ADDR_EXTRACT_72X(addr);
        page = 0;
    }
#endif
#if defined ZLS30701_INCLUDED
    if(zl303xx_Params->deviceType == ZL3070X_DEVICETYPE)
    {
        /* 70x Page and address decoding from virtual address */
        address = ZL303XX_MEM_ADDR_EXTRACT_70X(addr);
        page = ZL303XX_MEM_PAGE_EXTRACT_70X(addr);
    }
#endif
#if defined ZLS30751_INCLUDED
    if(zl303xx_Params->deviceType == ZL3075X_DEVICETYPE)
    {
        /* 75x Page and address decoding from virtual address */
        address = ZL303XX_MEM_ADDR_EXTRACT_75X(addr);
        page = ZL303XX_MEM_PAGE_EXTRACT_75X(addr);
    }
#endif
#if defined ZLS30771_INCLUDED
    if(zl303xx_Params->deviceType == ZL3077X_DEVICETYPE)
    {
        /* 77x Page and address decoding from virtual address */
        address = ZL303XX_MEM_ADDR_EXTRACT_77X(addr);
        page = ZL303XX_MEM_PAGE_EXTRACT_77X(addr);
    }
#endif

    regSize = (addr & 0x3F00) >>12;     /* 6 bits @ 12 */
    overlay =  (addr & 0xC0000) >>18;   /* 2 bits @ 18*/
    /*debugf("wri page=0x%x, addr=0x%x, regSize=0x%x overlay=0x%x, %p\n", page, address, regSize, overlay, addr); */

    if ((fdOfDevCS = open((const char *)zl303xx_Params->spiParams.linuxChipSelectDevName, O_RDWR)) < 0)
    {
        fprintf(stderr, "cpuSpiWrite(): open failed on %s; errno=%d\n", zl303xx_Params->spiParams.linuxChipSelectDevName, errno);
        return -ENOENT;
    }

    currPage = &zl303xx_Params->spiParams.currentPage;

#if defined ZLS30361_INCLUDED
    if(zl303xx_Params->deviceType == ZL3036X_DEVICETYPE)
    {
        /*debugf("Write3 page=0x%x, addr=0x%x, regSize=0x%x overlay=0x%x, %p\n", page, address, regSize, overlay, addr); */
        strReverse(data, data+bytes-1);   /* Reverse the byte write order for MSB device */
    }
#endif
#if defined ZLS30701_INCLUDED
    if(zl303xx_Params->deviceType == ZL3070X_DEVICETYPE)
    {
        /*debugf("Write3 page=0x%x, addr=0x%x, regSize=0x%x overlay=0x%x, %p\n", page, address, regSize, overlay, addr); */
        strReverse(data, data+bytes-1);   /* Reverse the byte write order for MSB device */
    }
#endif
#if defined ZLS30751_INCLUDED
    if(zl303xx_Params->deviceType == ZL3075X_DEVICETYPE)
    {
        /*debugf("Write3 page=0x%x, addr=0x%x, regSize=0x%x overlay=0x%x, %p\n", page, address, regSize, overlay, addr); */
        strReverse(data, data+bytes-1);   /* Reverse the byte write order for MSB device */
    }
#endif
#if defined ZLS30771_INCLUDED
    if(zl303xx_Params->deviceType == ZL3077X_DEVICETYPE)
    {
        /*debugf("Write3 page=0x%x, addr=0x%x, regSize=0x%x overlay=0x%x, %p\n", page, address, regSize, overlay, addr); */
        strReverse(data, data+bytes-1);   /* Reverse the byte write order for MSB device */
    }
#endif

    if ((status = writeDeviceOnSPI(params, fdOfDevCS, (Sint32T)address, page, bytes, (char*)data)) < 0)  /* Do it */
    {
        fprintf(stderr, "cpuSpiWrite(): call to writeDeviceOnSPI() failed! return=%d\n", status);
        switch (status)  /* Mimic current behaviour */
        {
            case -SPI_SEM_TAKE_FAIL:
                status = SPI_SEM_TAKE_FAIL;
                break;
            case -SPI_SEM_GIVE_FAIL:
                status = SPI_SEM_GIVE_FAIL;
                break;
            default:
                ZL303XX_TRACE_ALWAYS("cpuSpiWrite: Unhandled status return=%d", status, 0,0,0,0,0);
        }
    }
    else
        status = ZL303XX_OK;

    if (close(fdOfDevCS) < 0)
    {
        fprintf(stderr, "cpuSpiWrite(): close failed on %s!\n", zl303xx_Params->spiParams.linuxChipSelectDevName);
        return -ENOENT;
    }

    return status;
}


/**
*
  Function Name:*
*
*
*  cpuConfigSpi

  Details:*
*
*  Initialises the SPI interface
*
  Parameters:
        None

  Return Value: 0 (OK) or -1 if an error occurs


*******************************************************************************/
cpuStatusE cpuConfigSpi( void )
{
    /* SPI init code must be done in the Linux kernel! (for example see kernel/drivers/spi/spi_mpc83xx.c) */
    return CPU_OK;
}

/**
*
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
    /* SPI init code must be done in the Linux kernel! (for example see kernel/drivers/spi/spi_mpc83xx.c) */
    return CPU_OK;
}  /* END cpuSpiCleanUp */



#if defined ZLS30361_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
static void strReverse(Uint8T* begin, Uint8T* end)
{

    char aux;

    while(end>begin)
        aux=*end, *end--=*begin, *begin++=aux;
}
#endif

/*****************   END   ****************************************************/

#else
    /* #warning This file should only be included for Linux */
#endif  /* OS_LINUX */



