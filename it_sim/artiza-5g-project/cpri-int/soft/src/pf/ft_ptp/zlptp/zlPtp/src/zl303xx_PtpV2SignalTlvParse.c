

/*****************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*  Module Description:
*     Definitions and utility functions for parsing (packing/unpacking)
*     Signaling TLV structures to/from buffers. These are covered under
*     Clause 14 of the IEEE-1588-2008 Standard with associated Signaling TLV
*     definitions listed in Table 34.
*
******************************************************************************/


/****************   INCLUDE FILES   *******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"
#include "zl303xx_Error.h"

#include "zl303xx_PtpStrings.h"
#include "zl303xx_PtpV2MsgParse.h"    /* Include all message Parsing definitions. */
#include "zl303xx_PtpV2MgmtTlvHandle.h"
#include "zl303xx_PtpConstants.h"

/****************   DEFINES     ***********************************************/

/* MACRO to check that a Signaling TLV is within the range of supported
 * UNICAST NEGOTIATION TLVs. */
#define ZL303XX_CHECK_UNICAST_NEG_TLV(signalTlvId) \
      ((((signalTlvId) >= PTP_TLV_REQUEST_UNICAST_TX) && ((signalTlvId) <= PTP_TLV_CANCEL_UNICAST_TX_ACK)) \
               ? ZL303XX_OK  \
               : ZL303XX_UNSUPPORTED_OPERATION)

#define SIGNAL_TLV_COMMON_LEN   4

#define SIGNAL_TLV_MSG_TYPE_PACK(msg, buf)  PACK_HI_NIBBLE((buf) + 4, msg)
#define SIGNAL_TLV_PERIOD_PACK(per, buf)    PACK_8BIT((buf) + 5, per)
#define SIGNAL_TLV_DURATION_PACK(dur, buf)  PACK_32BIT((buf) + 6, dur)
#define SIGNAL_TLV_RENEWAL_PACK(ren, buf)   PACK_8BIT((buf) + 11, ren)

#define SIGNAL_TLV_MSG_TYPE_UNPACK(buf)  (zl303xx_MessageTypeE)UNPACK_HI_NIBBLE((buf) + 4)
#define SIGNAL_TLV_PERIOD_UNPACK(buf)    (Sint8T)UNPACK_8BIT((buf) + 5)
#define SIGNAL_TLV_DURATION_UNPACK(buf)  UNPACK_32BIT((buf) + 6)
#define SIGNAL_TLV_RENEWAL_UNPACK(buf)   (zl303xx_BooleanE)(UNPACK_8BIT((buf) + 11) & 0x01)

/****************   DATA TYPES   **********************************************/
typedef struct
{
   /* Shortened version of the tlvType string. */
   const char *strBrief;

   /* Expected value of lengthField. */
   Uint16T dataLen;

   /* Function to pack the data portion of the TLV. */
   void (*pack)(zl303xx_PtpV2SignalTlvDataU *data, Uint8T *buf);

   /* Function to unpack the data portion of the TLV. */
   void (*unpack)(Uint8T *buf, zl303xx_PtpV2SignalTlvDataU *data);
} SignalTlvDefS;

/****************   STATIC FUNCTION DECLARATIONS   ****************************/
static void zl303xx_PtpV2SignalReqPack(zl303xx_PtpV2SignalTlvDataU *data, Uint8T *buf);
static void zl303xx_PtpV2SignalGrantPack(zl303xx_PtpV2SignalTlvDataU *data, Uint8T *buf);
static void zl303xx_PtpV2SignalCancelPack(zl303xx_PtpV2SignalTlvDataU *data, Uint8T *buf);

static void zl303xx_PtpV2SignalReqUnpack(Uint8T *buf, zl303xx_PtpV2SignalTlvDataU *data);
static void zl303xx_PtpV2SignalGrantUnpack(Uint8T *buf, zl303xx_PtpV2SignalTlvDataU *data);
static void zl303xx_PtpV2SignalCancelUnpack(Uint8T *buf, zl303xx_PtpV2SignalTlvDataU *data);

/****************   STATIC GLOBAL VARIABLES   *********************************/
static const SignalTlvDefS SignalTlvDef[] = {
/*   strBrief   dataLen  pack()                       unpack()  */
   { "REQUEST",    6,    zl303xx_PtpV2SignalReqPack,    zl303xx_PtpV2SignalReqUnpack    },
   { "GRANT",      8,    zl303xx_PtpV2SignalGrantPack,  zl303xx_PtpV2SignalGrantUnpack  },
   { "CANCEL",     2,    zl303xx_PtpV2SignalCancelPack, zl303xx_PtpV2SignalCancelUnpack },
   { "ACK_CANCEL", 2,    zl303xx_PtpV2SignalCancelPack, zl303xx_PtpV2SignalCancelUnpack }
};

#define SIGNAL_TLV_TYPE_TO_INDEX(type)  ((type) - PTP_TLV_REQUEST_UNICAST_TX)

/****************   EXPORTED GLOBAL VARIABLES   *******************************/

/****************   EXPORTED FUNCTION DEFINITIONS   ***************************/

/* zl303xx_PtpV2SignalTlvPack */
/** 

   Packs a Signaling TLV structure into a byte buffer.

  Parameters:
   [in]   tlv  Signaling TLV to pack.
   [out]  buf  Buffer to pack the structure into.

  Return Value:
     Uint16T  Number of bytes packed into buf.
     0        An error occurred.

*******************************************************************************/
Uint16T zl303xx_PtpV2SignalTlvPack(zl303xx_PtpV2SignalTlvS *tlv, Uint8T *buf)
{
   const char *fnName = "zl303xx_PtpV2SignalTlvPack";
   Uint32T defIdx = SIGNAL_TLV_TYPE_TO_INDEX(tlv->tlvType);

   if (defIdx >= ZL303XX_ARRAY_SIZE(SignalTlvDef))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                    "%s: Invalid tlvType=0x%04X (expected 0x%04X-0x%04X).",
                    fnName, tlv->tlvType, PTP_TLV_REQUEST_UNICAST_TX,
                    PTP_TLV_CANCEL_UNICAST_TX_ACK, 0,0);
      return 0;
   }

   OS_MEMSET(buf, 0, SIGNAL_TLV_COMMON_LEN + SignalTlvDef[defIdx].dataLen);

   /* Pack the common section. Fill the lengthField with the value defined in
    * SignalTlvDef because the calling function may not have data-filled tlv. */
   PACK_16BIT(buf, tlv->tlvType);
   PACK_16BIT(buf + 2, SignalTlvDef[defIdx].dataLen);

   /* Pack the data section */
   SignalTlvDef[defIdx].pack(&tlv->data, buf);

   return SIGNAL_TLV_COMMON_LEN + SignalTlvDef[defIdx].dataLen;
}

/* zl303xx_PtpV2SignalTlvUnpack */
/** 

   Unpacks a Signaling TLV into a structure format.

  Parameters:
   [in]   buf     Buffer at the beginning of the TLV.
   [in]   bufLen  Length of buf, in bytes.
   [out]  tlv     Unpacked TLV structure.

  Return Value:
     Uint16T  Number of bytes consumed by this function.
           0  An error occurred.

*******************************************************************************/
Uint16T zl303xx_PtpV2SignalTlvUnpack(Uint8T *buf, Uint16T bufLen,
                                     zl303xx_PtpV2SignalTlvS *tlv)
{
   Uint32T defIdx;
   const char *fnName = "zl303xx_PtpV2SignalTlvUnpack";
   const char * tlvTypeStr;

   /* Check the minimum buffer length. */
   if (bufLen < SIGNAL_TLV_COMMON_LEN)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                    "%s: Invalid bufLen=%u (min=%u).",
                    fnName, bufLen, SIGNAL_TLV_COMMON_LEN, 0,0,0);
      return 0;
   }

   /* Clear the output structure. */
   OS_MEMSET(tlv, 0, sizeof(*tlv));

   /* Unpack tlvType and lengthField. */
   zl303xx_PtpTlvTypeLengthGet(buf, &tlv->tlvType, &tlv->lengthField);
   tlvTypeStr = zl303xx_PtpV2MsgTlvTypeToStr(tlv->tlvType);

   /* Check if tlvType is in range for a Signaling message. */
   defIdx = SIGNAL_TLV_TYPE_TO_INDEX(tlv->tlvType);
   if (defIdx >= ZL303XX_ARRAY_SIZE(SignalTlvDef))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                    "%s: Invalid tlvType=0x%04X (expected 0x%04X-0x%04X).",
                    fnName, tlv->tlvType, PTP_TLV_REQUEST_UNICAST_TX,
                    PTP_TLV_CANCEL_UNICAST_TX_ACK, 0,0);
      return 0;
   }

   /* Check if buffer is long enough to hold the entire TLV. */
   if (bufLen < SIGNAL_TLV_COMMON_LEN + SignalTlvDef[defIdx].dataLen)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                    "%s: Invalid %s (0x%04X) bufLen=%u (min=%u).",
                    fnName, tlvTypeStr, tlv->tlvType, bufLen,
                    SIGNAL_TLV_COMMON_LEN + SignalTlvDef[defIdx].dataLen, 0);
      return 0;
   }

   /* Check if lengthField is correct. */
   if (tlv->lengthField != SignalTlvDef[defIdx].dataLen)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                    "%s: Invalid %s (%04X) lengthField=%u (expected %u).",
                    fnName, tlvTypeStr, tlv->tlvType, tlv->lengthField,
                    SignalTlvDef[defIdx].dataLen, 0);
      return 0;
   }

   /* tlvType, lengthField, and buffer length are good. Unpack the data section. */
   SignalTlvDef[defIdx].unpack(buf, &tlv->data);

   return SIGNAL_TLV_COMMON_LEN + tlv->lengthField;
}

/* zl303xx_PtpV2SignalTlvTypeStrBrief */
/** 

   Converts a tlvType value to a shortened string representation (e.g.,
   REQUEST_UNICAST_TRANSMISSION is returned as "REQUEST").

  Parameters:
   [in]   tlvType  Value to convert to a string.

  Return Value:
     (const char *)  String representation
     "INVALID"       tlvType is out of range.

*******************************************************************************/
const char *zl303xx_PtpV2SignalTlvTypeStrBrief(Uint16T tlvType)
{
   Uint32T defIdx = SIGNAL_TLV_TYPE_TO_INDEX(tlvType);

   if (defIdx < ZL303XX_ARRAY_SIZE(SignalTlvDef))
      return SignalTlvDef[defIdx].strBrief;

   return "INVALID";
}

/****************   STATIC FUNCTION DEFINITIONS   *****************************/

/* zl303xx_PtpV2SignalReqPack */
/** 

   Packs a REQUEST_UNICAST_TRANSMISSION structure into a byte buffer.

  Parameters:
   [in]   data  REQUEST structure to pack (data->request).
   [out]  buf   Buffer pointing to the beginning of the entire TLV.

*******************************************************************************/
static void zl303xx_PtpV2SignalReqPack(zl303xx_PtpV2SignalTlvDataU *data, Uint8T *buf)
{
   SIGNAL_TLV_MSG_TYPE_PACK(data->request.messageType, buf);
   SIGNAL_TLV_PERIOD_PACK(data->request.logInterMessagePeriod, buf);
   SIGNAL_TLV_DURATION_PACK(data->request.durationField, buf);
}

/* zl303xx_PtpV2SignalGrantPack */
/** 

   Packs a GRANT_UNICAST_TRANSMISSION structure into a byte buffer.

  Parameters:
   [in]   data  GRANT structure to pack (data->grant).
   [out]  buf   Buffer pointing to the beginning of the entire TLV.

*******************************************************************************/
static void zl303xx_PtpV2SignalGrantPack(zl303xx_PtpV2SignalTlvDataU *data, Uint8T *buf)
{
   SIGNAL_TLV_MSG_TYPE_PACK(data->grant.messageType, buf);
   SIGNAL_TLV_PERIOD_PACK(data->grant.logInterMessagePeriod, buf);
   SIGNAL_TLV_DURATION_PACK(data->grant.durationField, buf);
   SIGNAL_TLV_RENEWAL_PACK(data->grant.renewalInvited, buf);
}

/* zl303xx_PtpV2SignalCancelPack */
/** 

   Packs a CANCEL_UNICAST_TRANSMISSION or ACKNOWLEDGE_CANCEL_UNICAST_TRANSMISSION
   structure into a byte buffer.

  Parameters:
   [in]   data  CANCEL or ACK_CANCEL structure to pack (data->cancel or
                     data->ackCancel).
   [out]  buf   Buffer pointing to the beginning of the entire TLV.

*******************************************************************************/
static void zl303xx_PtpV2SignalCancelPack(zl303xx_PtpV2SignalTlvDataU *data, Uint8T *buf)
{
   SIGNAL_TLV_MSG_TYPE_PACK(data->cancel.messageType, buf);
}

/* zl303xx_PtpV2SignalReqUnpack */
/** 

   Unpacks the data section of a REQUEST_UNICAST_TRANSMISSION Signaling TLV.

  Parameters:
   [in]   buf   Buffer containing the entire TLV.
   [out]  data  Unpacked REQUEST structure (data->request).

*******************************************************************************/
static void zl303xx_PtpV2SignalReqUnpack(Uint8T *buf, zl303xx_PtpV2SignalTlvDataU *data)
{
   data->request.messageType           = SIGNAL_TLV_MSG_TYPE_UNPACK(buf);
   data->request.logInterMessagePeriod = SIGNAL_TLV_PERIOD_UNPACK(buf);
   data->request.durationField         = SIGNAL_TLV_DURATION_UNPACK(buf);
}

/* zl303xx_PtpV2SignalGrantUnpack */
/** 

   Unpacks the data section of a GRANT_UNICAST_TRANSMISSION Signaling TLV.

  Parameters:
   [in]   buf   Buffer containing the entire TLV.
   [out]  data  Unpacked GRANT structure (data->grant).

*******************************************************************************/
static void zl303xx_PtpV2SignalGrantUnpack(Uint8T *buf, zl303xx_PtpV2SignalTlvDataU *data)
{
   data->grant.messageType           = SIGNAL_TLV_MSG_TYPE_UNPACK(buf);
   data->grant.logInterMessagePeriod = SIGNAL_TLV_PERIOD_UNPACK(buf);
   data->grant.durationField         = SIGNAL_TLV_DURATION_UNPACK(buf);
   data->grant.renewalInvited        = SIGNAL_TLV_RENEWAL_UNPACK(buf);
}

/* zl303xx_PtpV2SignalCancelUnpack */
/** 

   Unpacks the data section of a CANCEL_UNICAST_TRANSMISSION or
   ACKNOWLEDGE_CANCEL_UNICAST_TRANSMISSION Signaling TLV.

  Parameters:
   [in]   buf   Buffer containing the entire TLV.
   [out]  data  Unpacked CANCEL or ACK_CANCEL structure (data->cancel or
                     data->ackCancel).

*******************************************************************************/
static void zl303xx_PtpV2SignalCancelUnpack(Uint8T *buf, zl303xx_PtpV2SignalTlvDataU *data)
{
   data->cancel.messageType = SIGNAL_TLV_MSG_TYPE_UNPACK(buf);
}

/****************   END   ****************************************************/
