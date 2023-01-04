

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

#ifndef _ZL303XX_SPI_WIN_SOCKET_H_
#define _ZL303XX_SPI_WIN_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_SpiSocket.h"
#include "zl303xx_Error.h"

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

zlStatusE zl303xx_SpiInitialize(void);
zlStatusE zl303xx_SpiSetSocketIp(Uint8T ip0, Uint8T ip1, Uint8T ip2, Uint8T ip3);
zlStatusE  zl303xx_SpiSetTargetUdp(Uint16T portNum);
zlStatusE  zl303xx_SpiSetLocalUdp(Uint16T portNum);
zlStatusE  zl303xx_SpiSocketCreate(Uint8T ip0, Uint8T ip1, Uint8T ip2, Uint8T ip3);
zlStatusE  zl303xx_SpiSocketClose(void);
zlStatusE  zl303xx_SpiSocketRead(void *par, Uint32T regAddr, Uint32T *data, Uint16T bufLen);
zlStatusE  zl303xx_SpiSocketWrite(void *par, Uint32T regAddr, Uint32T *data, Uint16T bufLen);

#ifdef __cplusplus
}
#endif

#endif

