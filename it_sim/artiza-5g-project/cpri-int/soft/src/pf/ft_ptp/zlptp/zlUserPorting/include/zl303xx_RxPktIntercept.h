

/******************************************************************************
*
*  $Id: zl303xx_RxPktIntercept.h 6058 2011-06-10 14:37:16Z AW $
*
*  Copyright 2006-2017 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Functions to intercept raw received packets
*
******************************************************************************/

#ifndef _ZL303XX_RX_PKT_INTERCEPT_H_
#define _ZL303XX_RX_PKT_INTERCEPT_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"

/*****************   DEFINES     **********************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/
zlStatusE zl303xx_InitRxPktIntercept(char const *ifName);
zlStatusE zl303xx_DestroyRxPktIntercept(char const *ifName);

#ifdef OS_LINUX
    #define PKT_CHAR_DEV "/dev/zlTsPkts1"
    extern Sint32T charDevEtherHookFd;  /* Opened for read by zl303xx_LnxPktRxTask() and write by zl303xx_NewLinuxEtherFilter() */
#endif

#ifdef __cplusplus
}
#endif

#endif

