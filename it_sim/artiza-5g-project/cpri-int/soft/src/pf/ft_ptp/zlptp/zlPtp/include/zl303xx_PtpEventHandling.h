

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Implementation of an event queue for the PTP engine
*
******************************************************************************/

#ifndef _ZL303XX_PTP_EVENT_HANDLING_H_
#define _ZL303XX_PTP_EVENT_HANDLING_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"

#include "zl303xx_PtpPortAddrQueryTbl.h"
#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpInternalTypes.h"

/*****************   DEFINES     **********************************************/

/*****************   DATA TYPES   *********************************************/

/* Local control message types */
typedef enum
{
   CTRL_MSG_INVALID = 0,
   CTRL_MSG_SET_PARENT_DATA,              /* Targets Clock. */
   CTRL_MSG_SET_CLOCK_PARAM,              /* Targets Clock. */
   CTRL_MSG_SET_KEEP_ALIVE_DURATION       /* Targets Stream. */
} localCtrlCommand_t;

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/* Specific message types */
zlStatusE zl303xx_PtpSendControlMsg(zl303xx_PtpClockHandleT clockHandle,
                                    Uint32T clockStreamId,
                                    Uint8T *ctrlMsg,
                                    Uint32T ctrlMsgLength);

#ifdef __cplusplus
}
#endif

#endif
