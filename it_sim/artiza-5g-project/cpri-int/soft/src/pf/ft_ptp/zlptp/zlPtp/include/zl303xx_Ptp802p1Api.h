

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This header file contains the data types and functions required to support
*     the IEEE802.1AS standard (TSN Profile).
*
*******************************************************************************/
#ifndef ZL303XX_PTP_802_P1_API_H_
#define ZL303XX_PTP_802_P1_API_H_

#ifdef __cplusplus
extern "C" {
#endif
/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"        /* Basic ZL datatypes. */

#include "zl303xx_Ptp802p1Types.h"  /* Types for this profile. */
#include "zl303xx_PtpDatatypes.h"     /* Internal Implementation specific types. */

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   INTERNAL PTP FUNCTION DECLARATIONS   *********************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/*****************   PROFILE BINDING FUNCTION DECLARATIONS   ******************/

/* Routines to get data about the Profile or execute profile specific actions.
 * Ideally, these would be bound to the parent stack.  */
zlStatusE zl303xx_Ptp802p1v1ProfileInfoGet  (const zl303xx_PtpProfileS **profileInfo);

/* Routines to set Profile specific default initialization values for PTP objects.
 * Ideally, these would be bound to the parent stack. */
zlStatusE zl303xx_Ptp802p1v1ClockDefaults (void *pClockInitS);
zlStatusE zl303xx_Ptp802p1v1PortDefaults (void *pPortInitS);
zlStatusE zl303xx_Ptp802p1v1StreamDefaults (void *pStreamInitS);

/* Routines to execute any Post-PTP Object creation actions for the profile.
 * For example, BIND a custom TLV handler to the clock or set internal parameters. */
/* Currently nothing defined for the Port and Stream. */
zlStatusE zl303xx_Ptp802p1ClockCreateActions(void *pClockS);

/* Routines to print profile specific debug information for each of the PTP objects. */
zlStatusE zl303xx_Ptp802p1ClockDebugShow (void *pClockS);
zlStatusE zl303xx_Ptp802p1PortDebugShow  (void *pPortS);
zlStatusE zl303xx_Ptp802p1StreamDebugShow(void *pStreamS);

/******************************************************************************/
/*******   PROFILE SPECIFIC API FUNCTIONS   ***********************************/
/******************************************************************************/
/* Routine to add necessary TLVs to transmitted Signaling messages. */
Uint16T zl303xx_Ptp802p1TlvAppend(
      zl303xx_ClockStreamS *pPtpStream,
      Uint32T messageType,
      Uint8T *pOutMsgBuf,
      Uint8T *pOutTlvBuf);
/* Routine to process the profile specific TLV. */
Uint16T zl303xx_Ptp802p1TlvHandle(
      Uint32T streamHandle,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf,
      Uint8T *pOutTlvBuf);

#ifdef __cplusplus
}
#endif
#endif /* ZL303XX_PTP_802_P1_API_H_ */
