

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Header file for example files
*
*******************************************************************************/

#ifndef ZL303XX_EXAMPLE_TELECOM_G8275_H_
#define ZL303XX_EXAMPLE_TELECOM_G8275_H_

#ifdef __cplusplus
extern "C" {
#endif


/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_PtpApiTypes.h"

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/* G8275.1 (edition 1) */
zlStatusE examplePtpTelecomPhaseG8275p1Master(void);
zlStatusE examplePtpTelecomPhaseG8275p1Bc(void);
zlStatusE examplePtpTelecomPhaseG8275p1BcSlave(void);
zlStatusE examplePtpTelecomPhaseG8275p1Slave(void);
zlStatusE examplePtpTelecomPhaseG8275VirtualPortAdd(zl303xx_PtpClockHandleT clockHandle);
zlStatusE examplePtpTelecomPhaseG8275p1Bc2Ifs(void);

/* G8275.1 (edition 2) */
zlStatusE examplePtpTelecomPhaseG8275p1v2Master(void);
zlStatusE examplePtpTelecomPhaseG8275p1v2Bc(void);
zlStatusE examplePtpTelecomPhaseG8275p1v2BcSlave(void);
zlStatusE examplePtpTelecomPhaseG8275p1v2Slave(void);

/* G8275.2 */
zlStatusE examplePtpTelecomPhaseG8275p2Master(void);
zlStatusE examplePtpTelecomPhaseG8275p2Bc(void);
zlStatusE examplePtpTelecomPhaseG8275p2Slave(void);

zlStatusE examplePtpTelecomPhaseMasterIPv4(void);
zlStatusE examplePtpTelecomPhaseBcIPv4(void);
zlStatusE examplePtpTelecomPhaseSlaveIPv4(void);
zlStatusE examplePtpTelecomPhaseBcSlaveIPv4(void);

zlStatusE examplePtpTelecomPhaseG8275p2Bc1Port(void);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */

