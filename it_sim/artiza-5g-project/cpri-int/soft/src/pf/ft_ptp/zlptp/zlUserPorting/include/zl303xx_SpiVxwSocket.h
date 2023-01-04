

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Header for remote SPI read / write operations
*
******************************************************************************/

#ifndef _ZL303XX_SPI_VXW_SOCKET_H_
#define _ZL303XX_SPI_VXW_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Os.h"
#include "zl303xx_Error.h"

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/
extern OS_SEM_ID spiIsrSemId;

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/
/* Provide OS Exclusion for the interface */
zlStatusE spiLock(void);
zlStatusE spiUnlock(void);

zlStatusE zl303xx_SpiInitInterrupts(Uint32T spi_irq);
zlStatusE zl303xx_SpiDisableInterrupts(Uint32T spi_irq);

zlStatusE zl303xx_SpiSocketCreate(void);
zlStatusE zl303xx_SpiSocketClose(void);

zlStatusE zl303xx_SpiSocketRead(void *par, Uint32T regAddr, Uint32T *data, Uint16T bufLen);
zlStatusE zl303xx_SpiSocketWrite(void *par, Uint32T regAddr, Uint32T *data, Uint16T bufLen);

#ifdef __cplusplus
}
#endif

#endif
