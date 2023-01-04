

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains functions to configure several PC37.238-2011 (Power
*     Profile) applications.
*
*******************************************************************************/

#ifndef _ZL303XX_EXAMPLE_PTP_POWER_PROFILE_H_
#define _ZL303XX_EXAMPLE_PTP_POWER_PROFILE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/* Default examples are Multicast Ordinary Clock (v1: 2011). */
zlStatusE examplePowerProfileMaster(void);
zlStatusE examplePowerProfileSlave(void);

/* Extra examples are Multicast Ordinary Clock IPv4 (v1: 2011). */
zlStatusE examplePowerProfileMaster_IPv4(void);
zlStatusE examplePowerProfileSlave_IPv4(void);

/* Version 2: examples are also provided although this revision is not finalized. */
zlStatusE examplePowerProfileMaster_v2(void);
zlStatusE examplePowerProfileSlave_v2(void);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
