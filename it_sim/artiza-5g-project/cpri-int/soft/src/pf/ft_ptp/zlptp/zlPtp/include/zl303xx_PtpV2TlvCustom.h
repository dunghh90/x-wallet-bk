

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Definitions and utility functions for interfacing to custom Management
*     TLV handlers.
*
******************************************************************************/

#ifndef _ZL303XX_PTP_V2_TLV_CUSTOM_H_
#define _ZL303XX_PTP_V2_TLV_CUSTOM_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Error.h"

/* Need the Message Types */
#include "zl303xx_PtpV2MsgTypes.h"

/*****************   DEFINES   ************************************************/

/* ERROR return values from the custom handlers.
 * For success conditions, the minimum return value should be 6-bytes (tlvType,
 * length, tlvId); anything below 6 is handled as an error. If none of the
 * values below are used, then a generic ZL303XX_MGMT_ERR_NO_SUCH_ID Management
 * Error Response is sent. */
typedef enum zl303xx_PtpTlvCustomErrorE
{
   ZL303XX_PTP_TLV_CUSTOM_RSP_NO_HANDLER = 0,   /* generates ZL303XX_MGMT_ERR_NO_SUCH_ID */
   ZL303XX_PTP_TLV_CUSTOM_RSP_UNSUPPORTED,      /* generates ZL303XX_MGMT_ERR_NOT_SUPPORTED */
   ZL303XX_PTP_TLV_CUSTOM_RSP_PARAM_ERROR,      /* generates ZL303XX_MGMT_ERR_WRONG_VALUE */
   ZL303XX_PTP_TLV_CUSTOM_RSP_PROVIDED,         /* User provided ERROR TLV */
   ZL303XX_PTP_TLV_CUSTOM_RSP_NONE,             /* No ERROR; do not reply (Perhaps a RSP or ACK) */

   ZL303XX_PTP_TLV_CUSTOM_RSP_SUCCESS  = 6      /* No ERROR; Successfully Handled. */
} zl303xx_PtpTlvCustomErrorE;

/* Custom TLV buffers should restrict themselves to this limit. */
#define ZL303XX_PTP_TLV_CUSTOM_BUF_LENGTH_MAX   (Uint16T)(128)

/* Maximum Number of Handlers to support.  */
#define ZL303XX_PTP_TLV_CUSTOM_HANDLERS_MAX     8

/*****************   DATA TYPES   *********************************************/

/* Define a TLV handler function type for processing custom TLVs              */
/* Returns:
 *    On Success, the length of the entire output buffer response; this includes
 *                the tlvType, length and managementId fields. So, the returned
 *                value should be at least 6-bytes and NOT the same as the value
 *                in the lengthField of the pOutCustomTlvBuf.
 *    On Failure, One of the ERROR codes listed above.
 */
typedef Uint16T (*zl303xx_PtpV2TlvCustomHandlerT)(
      /* Stream handle of the stream receiving the TLV. In the case when no
       * stream exists, the clock will set this field to INVALID (-1)
       * representing a general received TLV at the clock. */
      Uint32T streamHandle,

      /* The entire parsed, received message as input. As an output, the message
       * to send with fields updated. */
      zl303xx_PtpV2MsgS *pPtpMsgRx,

      /* Received TLV Buffer; starts at tlvType. */
      Uint8T *pInCustomTlvBuf,

      /* TLV Buffer to reply; starts at tlvType. */
      Uint8T *pOutCustomTlvBuf);

/*****************   DATA STRUCTURES   ****************************************/

/* A data type that can be used to lookup/store custom handlers. */
typedef struct zl303xx_PtpV2TlvCustomS
{
   /** The tlvType under which this Custom message is classified. Currently
    * only MGMT and VENDOR_EXT TLVs can have customized handlers.*/
   Uint16T tlvType;

   /** The tlvId of the implementation specific TLV.
    *  If tlvType == MGMT then tlvId is the managementId field as a 32-bit value.
    *  If tlvType == ORG_EXT then tlvId is the 3-byte OUI as a 32-bit value
    *                (with MSB = 0). Any organizational sub-type is dealt with
    *                in the same handler. */
   Uint32T tlvId;

   /** The handler routine for the implementation specific MGMT TLV. */
   zl303xx_PtpV2TlvCustomHandlerT fnTlvHandler;
} zl303xx_PtpV2TlvCustomS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/* Attempts to processes custom implemented TLVs by searching through a table
 * of configured custom TLV handlers (provided by the Clock). Triggered when a
 * normal TLV message handler cannot resolve a received TLV message.
 * The user can set the handler of non-standard Management TLVs via the
 * zl303xx_PtpTlvCustomHandlerSet() routine (see API for details).  */
Uint16T zl303xx_PtpV2TlvCustomHandler(
      zl303xx_PtpV2TlvCustomS *pPtpCustomTbl,
      Uint32T streamHandle,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf,
      Uint8T *pOutTlvBuf);

/* Searches a Custom TLV Table for the specified entry. */
zl303xx_BooleanE zl303xx_PtpTlvCustomHandlerExists(
      zl303xx_PtpV2TlvCustomS *pPtpCustomTbl,
      Uint16T tlvType,
      Uint32T tlvId);

/* Allows the user to add a custom TLV handler. */
zlStatusE zl303xx_PtpTlvCustomHandlerSet(
      Uint32T clockHandle,
      Uint16T tlvType,
      Uint32T tlvId,
      zl303xx_PtpV2TlvCustomHandlerT fnTlvHandler);

/* Allows the user to delete a custom TLV handler. */
zlStatusE zl303xx_PtpTlvCustomHandlerDel(
      Uint32T clockHandle,
      Uint16T tlvType,
      Uint32T tlvId);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
