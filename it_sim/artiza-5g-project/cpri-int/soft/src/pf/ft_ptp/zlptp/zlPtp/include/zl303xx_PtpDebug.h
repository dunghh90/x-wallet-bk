

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright (c) 2006 Zarlink Semiconductor Limited.
*
*  Module Description:
*     Debug functions to allow local control of the running PTP task
*
*******************************************************************************/

#ifndef ZL303XX_PTP_DEBUG_H__
#define ZL303XX_PTP_DEBUG_H__

/*****************   # DEFINES   **********************************************/

/*****************   TYPEDEFS    **********************************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   ***************************/
/* Application callable functions */
Sint32T setCurrentUTCTime(char *dateTimeStr);

Sint32T forcePacketTransmission(Uint32T streamHandle,
                                Uint32T messageType,
                                zl303xx_BooleanE enable);
#endif

