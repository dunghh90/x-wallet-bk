

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Definitions and utility functions for parsing (packing/unpacking) TLV
*     structures to/from buffers. These include generic TLV definitions and any
*     other TLVs not covered by its own module (i.e. Alternate Time Offset
*     Indicator is covered here but Signaling and Management have their own
*     modules).
*
******************************************************************************/

#ifndef _ZL303XX_PTP_TLV_PARSE_H_
#define _ZL303XX_PTP_TLV_PARSE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"

/* Exported inclusions */
#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpConstants.h"

/* Internal inclusions */
#include "zl303xx_PtpV2MsgTypes.h"
#include "zl303xx_PtpDatatypes.h"

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/* Handles TLVs attached to GENERAL Messages */
Uint16T zl303xx_PtpTlvAppendedHandle(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf);
/* Handles TLVs attached to EVENT Messages */
Uint16T zl303xx_PtpTlvEventMsgdHandle(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf);

/* Routines to Pack the TYPE & LENGTH fields of a generic TLV Buffer. */
Uint16T zl303xx_PtpTlvTypeLengthGet(
      Uint8T  *pInTlvBuf,
      Uint16T *tlvType,
      Uint16T *tlvLength);
Uint16T zl303xx_PtpTlvTypeSet(
      Uint16T tlvType,
      Uint8T *pOutTlvBuf);
Uint16T zl303xx_PtpTlvLengthSet(
      Uint16T tlvLength,
      Uint8T *pOutTlvBuf);

/* Unpacks/Packs the Organization Extension Header TLV to/from a compatible
 * data type. */
Uint16T zl303xx_PtpTlvOrganizationHeaderUnpack(
      Uint8T *pInTlvBuf,
      Uint8T *orgId,
      Uint8T *orgSubType);
Uint16T zl303xx_PtpTlvOrganizationHeaderPack(
      Uint8T *orgId,
      Uint8T *orgSubType,
      Uint8T *pOutTlvBuf);
/* Gets a 3-Byte Field (ex: OUI or Vender Sub-Type) as a 32-bit value (MSB = 0).
 * Also used for custom TLV lookup.  */
Uint16T zl303xx_PtpTlvConvert3ByteFieldTo32Bit(
      Uint8T *pInOuiBuf,
      Uint32T *val32);

/* Alternate Time Offset Indicator TLV. */
/* Unpacks/Packs the Alternate Time Offset Indicator TLV to/from a compatible
 * data type. */
Uint16T zl303xx_PtpTlvAltTimeOffsetUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_PtpTlvAltTimeOffsetS *altTimeS);
Uint16T zl303xx_PtpTlvAltTimeOffsetPack(
      zl303xx_PtpTlvAltTimeOffsetS *altTimeS,
      Uint8T *pOutTlvBuf);

Uint16T zl303xx_PtpTlvAltTimeOffsetHandle(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf);

zlStatusE zl303xx_PtpTlvAltTimeOffsetEntryAdd(
      zl303xx_PtpClockHandleT clockHandle,
      zl303xx_PtpTlvAltTimeOffsetS *altTimeS);
zlStatusE zl303xx_PtpTlvAltTimeOffsetEntryDel(
      zl303xx_PtpClockHandleT clockHandle,
      Uint8T keyField);
zlStatusE zl303xx_PtpTlvAltTimeOffsetEntryEnable(
      zl303xx_PtpClockHandleT clockHandle,
      Uint8T keyField,
      zl303xx_BooleanE enable);

Uint8T * zl303xx_PtpTlvPathTraceFind(
      zl303xx_PtpV2MsgS *pPtpMessage,
      Uint8T *msgBuffer);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
