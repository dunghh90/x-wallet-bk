

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright (c) 2006 Zarlink Semiconductor Limited.
*
*  Module Description:
*     Support functions to allow local control of the running PTP task
*
*******************************************************************************/

#ifndef ZL303XX_PTP_CONTROL_H_
#define ZL303XX_PTP_CONTROL_H_

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_PtpInternalTypes.h"

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/
void handleLocalCtrlMessages(zl303xx_ClockStreamS *pPtpStream, zl303xx_PtpClockMsgQueueS *msg);
zlStatusE zl303xx_PtpCtrlClockParamSet(zl303xx_PtpClockHandleT clockHandle,
                                       zl303xx_PtpClockParamE param,
                                       const void *data);

/* User callable functions. These are really for interactive command line use and not
   generally expected to be called by an application */
zlStatusE zl303xx_SetClockParentData(zl303xx_PtpClockHandleT clockHandle, zl303xx_ParentDS *pParentDS, Uint16T rxPortNumber);


#endif /* MULTIPLE INCLUDE BARRIER */
