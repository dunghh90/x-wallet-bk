

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Definitions and utility functions for parsing (packing/unpacking)
*     Signaling TLV structures to/from buffers. These are covered under
*     Clause 14 of the IEEE-1588-2008 Standard with associated Signaling TLV
*     definitions listed in Table 34.
*
******************************************************************************/

#ifndef _ZL303XX_PTP_V2_SIGNAL_TLV_PARSE_H_
#define _ZL303XX_PTP_V2_SIGNAL_TLV_PARSE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"

#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpConstants.h"

/*****************   DEFINES   ************************************************/

/* The required length of the Unicast Negotiation Signaling TLVs.
 * These lengths include 2 bytes for each of the TYPE and LENGTH fields (but
 * excludes the base signaling message carrier length). */
#define ZL303XX_PTP_V2_UNI_NEG_REQUEST_TLV_LEN             (Uint16T)(10)
#define ZL303XX_PTP_V2_UNI_NEG_GRANT_TLV_LEN               (Uint16T)(12)
#define ZL303XX_PTP_V2_UNI_NEG_CANCEL_TLV_LEN              (Uint16T)(6)
#define ZL303XX_PTP_V2_UNI_NEG_CANCEL_ACK_TLV_LEN          (Uint16T)(6)
#define ZL303XX_PTP_V2_UNI_NEG_TLV_LEN_MAX                 (Uint16T)(ZL303XX_PTP_V2_UNI_NEG_GRANT_TLV_LEN)

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

typedef struct
{
   zl303xx_MessageTypeE messageType;
   Sint8T logInterMessagePeriod;
   Uint32T durationField;
} zl303xx_PtpV2SignalRequestS;

typedef struct
{
   zl303xx_MessageTypeE messageType;
   Sint8T logInterMessagePeriod;
   Uint32T durationField;
   zl303xx_BooleanE renewalInvited;
} zl303xx_PtpV2SignalGrantS;

typedef struct
{
   zl303xx_MessageTypeE  messageType;
} zl303xx_PtpV2SignalCancelS, zl303xx_PtpV2SignalAckCancelS;

typedef union
{
   zl303xx_MessageTypeE           messageType;
   zl303xx_PtpV2SignalRequestS    request;
   zl303xx_PtpV2SignalGrantS      grant;
   zl303xx_PtpV2SignalCancelS     cancel;
   zl303xx_PtpV2SignalAckCancelS  ackCancel;
} zl303xx_PtpV2SignalTlvDataU;

/* Structure containing a parsed Signaling TLV */
typedef struct
{
   Uint16T tlvType;
   Uint16T lengthField;
   zl303xx_PtpV2SignalTlvDataU data;
} zl303xx_PtpV2SignalTlvS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/* Unicast Negotiation Message Parsing Routines */
Uint16T zl303xx_PtpV2SignalTlvPack(zl303xx_PtpV2SignalTlvS *tlv, Uint8T *buf);
Uint16T zl303xx_PtpV2SignalTlvUnpack(Uint8T *buf, Uint16T bufLen,
                                     zl303xx_PtpV2SignalTlvS *tlv);

const char *zl303xx_PtpV2SignalTlvTypeStrBrief(Uint16T tlvType);

/* Unicast negotiation TLV specific routines */
zlStatusE zl303xx_PtpV2SignalUnicastNegTlvHandle(
      zl303xx_PtpClockMsgQueueS *ptpData,
      zl303xx_PtpV2SignalTlvS *rxTlv,
      zl303xx_ClockStreamS **ppStream,
      zl303xx_PortDataS *pPort,
      zl303xx_PtpV2SignalTlvS *txTlv);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
