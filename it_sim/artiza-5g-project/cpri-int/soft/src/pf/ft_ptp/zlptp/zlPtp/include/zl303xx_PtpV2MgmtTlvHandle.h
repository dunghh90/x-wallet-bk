

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Definitions and utility functions for handling (processing)
*     Type-Length-Value (TLV) structures/buffers for management TLVs.
*
******************************************************************************/

#ifndef _ZL303XX_PTP_V2_MGMT_TLV_HANDLE_H_
#define _ZL303XX_PTP_V2_MGMT_TLV_HANDLE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"

#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpV2MgmtTlvParse.h"
#include "zl303xx_PtpDatatypes.h"

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/* Routine to handle Management ERROR TLVs appended to a Management Message. */
/* Just prints the fields. */
Uint16T zl303xx_PtpV2MgmtErrorTlvHandle(
      zl303xx_PtpClockMsgQueueS *pPtpRxQueueData,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PortDataS *pRxPtpPort);

/* Routine to handle Management TLVs appended to a Management Message. */
Uint16T zl303xx_PtpV2MgmtTlvHandle(
      zl303xx_PtpClockMsgQueueS *pPtpRxQueueData,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PortDataS *pPtpPort);

/* Initiates a Management TLV Message with the buffer provided. */
zlStatusE zl303xx_PtpV2MgmtTlvInitiate(
      zl303xx_PortAddress *destAddr,
      zl303xx_ClockStreamS *pPtpStream,
      const zl303xx_PortIdentity *pTargetPortId,
      zl303xx_PtpV2MgmtActionE actionField,
      Uint8T *pMgmtTlvBuf, Uint8T mgmtTlvBufLen);

/* Prepares a Management Response (Header and Management Data) from received
 * Management message data. */
Uint16T zl303xx_PtpV2MgmtTlvPrepareResponse(
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      zl303xx_PortDataS *pPtpPort,
      zl303xx_PtpV2MsgS *pPtpMsgTx,
      Uint8T *pOutMsgBuf);

/* Send the actual response buffer. */
Uint16T zl303xx_PtpV2MgmtTlvRespond(
      zl303xx_PtpClockMsgQueueS *pPtpRxQueueData,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      zl303xx_PortDataS *pPtpPort,
      Uint8T *pOutMsgBuf, Uint16T respMsgBufLen);

/* Send a Management Error TLV response. */
Uint16T zl303xx_PtpV2MgmtErrorTlvRespond(
      zl303xx_PtpClockMsgQueueS *pPtpRxQueueData,
      zl303xx_PtpV2MsgS *pPtpMsgTx,
      zl303xx_PortDataS *pPtpPort,
      zl303xx_ManagementErrorIdE mgmtErrorId,
      zl303xx_PtpV2MgmtTlvIdE mgmtTlvId,
      const char *displayMsg);

/* Send a basic Management TLV with just the tlvType, length and managementId fields. */
zlStatusE zl303xx_PtpV2MgmtTlvIssue(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_PortIdentity *targetPortIdentity,
      zl303xx_PtpV2MgmtActionE actionField,
      Uint16T managementId);
/* Send a supplied Management TLV buffer from the specified stream. */
zlStatusE zl303xx_PtpV2MgmtTlvToStreamSend(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_PortIdentity *targetPortIdentity,
      zl303xx_PtpV2MgmtActionE actionField,
      Uint8T *pMgmtTlvBuf, Uint16T mgmtTlvBufLen);

/**** DEFAULT DATA SET TLV handling routines ***/
Uint16T zl303xx_PtpMgmtTlvHandleDefaultDSMembers(zl303xx_PtpClockS *pPtpClock,
                                          zl303xx_PtpV2MgmtActionE actionField,
                                          zl303xx_PtpV2MgmtTlvIdE memberId,
                                          Uint8T *pInTlvBuf,
                                          Uint16T inTlvDataLen,
                                          Uint8T *pOutTlvBuf,
                                          Uint16T *mgmtErrorLen);

Uint16T zl303xx_PtpV2MgmtTlvVersionNumberHandle(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpV2MgmtActionE *actionField,
      Uint8T *pInTlvBuf,
      Uint8T *pOutTlvBuf);


/**************************************************/
/**** HANDLERS for the various MANAGEMENT TLVs  ***/
/**** List of Routines for each managementId can be:
 *    - zl303xx_PtpV2MgmtTlv<managementId>Handle()
 *    - zl303xx_PtpV2MgmtTlv<managementId>Gather()
 *    - zl303xx_PtpV2MgmtTlv<managementId>HandleGet(),Set(),Cmd(),Rsp(),Ack()
 *    - zl303xx_PtpV2MgmtTlv<managementId>Trace()
 *    - The Pack & Unpack routines are contained in Parse.h/c
 */
/**************************************************/

/*** NULL MANAGEMENT Utility Routines **/
Uint16T zl303xx_PtpV2MgmtTlvNullHandle(
      zl303xx_PortDataS *pPtpPort,
      zl303xx_PtpV2MgmtActionE *actionField,
      Uint8T *pInTlvBuf,
      Uint8T *pOutTlvBuf);

/*** CLOCK DESCRIPTION Utility Routines **/
Uint16T zl303xx_PtpV2MgmtTlvClockDescrHandle(
      zl303xx_PortDataS *pPtpPort,
      zl303xx_PtpV2MgmtActionE *actionField,
      Uint8T *pInTlvBuf,
      Uint8T *pOutTlvBuf);
zlStatusE zl303xx_PtpV2MgmtTlvClockDescrGather(
   zl303xx_PortDataS *pPtpPort,
   zl303xx_ClockDescriptionS *pClkDescrS);
zlStatusE zl303xx_PtpV2MgmtTlvClockDescrTrace(
   zl303xx_ClockDescriptionS *pClkDescrS);

/*** USER DESCRIPTION Utility Routines **/
Uint16T zl303xx_PtpV2MgmtTlvUserDescrHandle(
      zl303xx_PortDataS *pPtpPort,
      zl303xx_PtpV2MgmtActionE *actionField,
      Uint8T *pInTlvBuf,
      Uint8T *pOutTlvBuf);

/***********************************************/
/**** EXTERNAL UTILITY FUNCTION DECLARATIONS ***/
/***********************************************/

/* Utility routines to check if a particular Clock or Port Identity meets the
 * conditions of a provided targetPortIdentity (from a Signaling or Management
 * Message).  Refer to IEEE-1588-2008: Tables 32 & 36 for filter details. */
zl303xx_BooleanE zl303xx_PtpPortIdentityMatchesTarget(
      zl303xx_PortIdentity *pPortIdentity,
      zl303xx_PortIdentity *pTargetPortIdentity);
zl303xx_BooleanE zl303xx_PtpClockIdentityMatchesTarget(
      zl303xx_ClockIdentity pClockIdentity,
      zl303xx_PortIdentity *pTargetPortIdentity);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
