

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains functions to configure several PTP Peer-Delay
*     applications.
*
*******************************************************************************/

#ifndef _ZL303XX_EXAMPLE_PTP_PEER_DELAY_H_
#define _ZL303XX_EXAMPLE_PTP_PEER_DELAY_H_

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

/* Default examples are Multicast Ordinary Clock. (IPv4/UDP) */
zlStatusE examplePeerDelayMaster(void);
zlStatusE examplePeerDelaySlave(void);

/* Unicast examples (Multicast PDelay + UniNeg Sync/Annc). */
zlStatusE exampleUniNegPeerDelayMaster(void);
zlStatusE exampleUniNegPeerDelaySlave(void);

/* Default examples are Multicast Ordinary Clock. (ETH L2) */
zlStatusE examplePeerDelayMasterL2(void);
zlStatusE examplePeerDelaySlaveL2(void);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
