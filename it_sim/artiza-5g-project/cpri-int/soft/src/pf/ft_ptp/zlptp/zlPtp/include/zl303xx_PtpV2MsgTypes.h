

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Definitions and utility functions for Standard PTP Messages and On-the-Wire
*     formats. These are covered under Clause 13 of the IEEE-1588-2008 Standard.
*
******************************************************************************/

#ifndef _ZL303XX_PTP_V2_MSG_TYPES_H_
#define _ZL303XX_PTP_V2_MSG_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"

#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpConstants.h"

/*****************   DEFINES   ************************************************/

/* Based on the zl303xx_MessageTypeE enum definitions, a 'valid messageType'
 * check can be quickly done by bit-shifting and masking a supplied messageType
 * against the mask below.  */
/* {
      ZL303XX_MSG_ID_SYNC                = 0x0,    (1 << 0)  = 0x00000001
      ZL303XX_MSG_ID_DELAY_REQ           = 0x1,    (1 << 1)  = 0x00000002
      ZL303XX_MSG_ID_PEER_DELAY_REQ      = 0x2,    (1 << 2)  = 0x00000004
      ZL303XX_MSG_ID_PEER_DELAY_RESP     = 0x3,    (1 << 3)  = 0x00000008
      ZL303XX_MSG_ID_FOLLOWUP            = 0x8,    (1 << 8)  = 0x00000100
      ZL303XX_MSG_ID_DELAY_RESP          = 0x9,    (1 << 9)  = 0x00000200
      ZL303XX_MSG_ID_PEER_DELAY_FOLLOWUP = 0xA,    (1 << 10) = 0x00000400
      ZL303XX_MSG_ID_ANNOUNCE            = 0xB,    (1 << 11) = 0x00000800
      ZL303XX_MSG_ID_SIGNALING           = 0xC,    (1 << 12) = 0x00001000
      ZL303XX_MSG_ID_MANAGEMENT          = 0xD,    (1 << 13) = 0x00002000
   }  zl303xx_MessageTypeE;  */
/* The following MACRO's can be used to check for valid PTP messageTypes. */
#define ZL303XX_PTP_V2_VALID_MESSAGE_TYPE_MASK          (Uint32T)(0x00003F0F)
#define ZL303XX_CHECK_MESSAGE_TYPE(messageType)   \
               ((ZL303XX_PTP_V2_VALID_MESSAGE_TYPE_MASK & (1 << (messageType))) \
                     ? ZL303XX_OK  \
                     : ZL303XX_PARAMETER_INVALID)

/* Likewise, Unicast Negotiation only supports SYNC, PEER_DELAY_RESP,
 * ANNOUNCE & DELAY_RESP so define the following mask & macro.  */
#define ZL303XX_PTP_V2_VALID_UNI_NEG_MSG_TYPES_MASK     (Uint32T)(0x00000A09)
#define ZL303XX_CHECK_UNI_NEG_MSG_TYPE_VALID(messageType)   \
               ((ZL303XX_PTP_V2_VALID_UNI_NEG_MSG_TYPES_MASK & (1 << (messageType))) \
                     ? ZL303XX_OK  \
                     : ZL303XX_PARAMETER_INVALID)

/* PTP Messages are either EVENT or GENERAL. */
#define ZL303XX_PTP_V2_MSG_NUM_EVENT_TYPES     (ZL303XX_MSG_ID_PEER_DELAY_RESP + 1)
#define ZL303XX_CHECK_EVENT_MSG_TYPE_VALID(messageType)   \
               ((messageType < ZL303XX_PTP_V2_MSG_NUM_EVENT_TYPES) \
                     ? ZL303XX_OK  \
                     : ZL303XX_PARAMETER_INVALID)


/****  GENERAL PTP MESSAGE FIELD LENGTHS ****/
/********************************************/
/** PTP Message FLAGS field length. */
#define ZL303XX_PTP_MSG_FLAGS_LEN                 (Uint16T)(2)
/** PTP Message SequenceId field length. */
#define ZL303XX_PTP_MSG_SEQ_ID_LEN                (Uint16T)(2)
/** PTP Message Clock Identity field length. */
#define ZL303XX_PTP_MSG_CLOCK_IDENTITY_LEN        (Uint16T)(8)
/** PTP Message Port Identity field length. */
#define ZL303XX_PTP_MSG_PORT_NUM_LEN              (Uint16T)(2)
#define ZL303XX_PTP_MSG_PORT_IDENTITY_LEN         (Uint16T)(10)
/** PTP Message Clock Quality field length. */
#define ZL303XX_PTP_MSG_CLOCK_QUALITY_LEN         (Uint16T)(4)
/* PTP Message Timestamp field length. */
#define ZL303XX_PTP_MSG_TIMESTAMP_LEN             (Uint16T)(10)

/******************************************/
/****  COMMON MESSAGE OVERLAY FORMATS  ****/
/******************************************/

/****** PTP V2 Timestamp Message Overlay Composition  ******/
/* This Overlay contains both a Timestamp and Port Identity Field.
 * It contains common data overlays for 7 of the 10 supported PTP message types:
 *    Timestamp only (SYNC, DELAY_REQ and FOLLOW_UP Messages).
 *    Timestamp and PortIdentity (DELAY_RESP, PDELAY_REQ, PDELAY_RESP and
 *       PDELAY_RESP_FOLLOWUP).                                              */
/* The following Definitions / Offsets are relative to the start of the
 * Overlay Data, not from the beginning of the PTP packet. */
/* Refer to IEEE-1588-2008: Tables 26 to 31 for exact field names.  */
/* ------------------------------------------*/
/* Field             Length      Offset      */
/* ------------------------------------------*/
/* timestamp            10          0        */
/* portIdentity         10         10        */
/* ------------------------------------------*/
/* Total Length         20                   */
#define ZL303XX_PTP_OVERLAY_TIMESTAMP_LEN         ZL303XX_PTP_MSG_TIMESTAMP_LEN
#define ZL303XX_PTP_OVERLAY_PORT_ID_LEN           ZL303XX_PTP_MSG_PORT_IDENTITY_LEN

#define ZL303XX_PTP_OVERLAY_TIMESTAMP_OFST        (Uint8T)(0)
#define ZL303XX_PTP_OVERLAY_PORT_ID_OFST          ZL303XX_PTP_OVERLAY_TIMESTAMP_LEN

#define ZL303XX_PTP_OVERLAY_TS_PORTID_LEN         (ZL303XX_PTP_MSG_TIMESTAMP_LEN + ZL303XX_PTP_MSG_PORT_IDENTITY_LEN)
#define ZL303XX_PTP_OVERLAY_TS_PORTID_MSG_LEN     (Uint16T)(ZL303XX_PTP_OVERLAY_TS_PORTID_LEN + ZL303XX_PTP_V2_HEADER_LEN)


/****  ON-THE-WIRE MESSAGE FORMAT DEFINITIONS  ****/
/**************************************************/

/****** PTP V2 Message Header Composition *****/
/**********************************************/
                                                      /*         IEEE-1588-2008: Table 18        */
                                                      /* ----------------------------------------*/
                                                      /* Field                 Length     Offset */
                                                      /* ----------------------------------------*/
#define ZL303XX_PTP_V2_HDR_TRANS_OFST     (Uint16T)(0)  /* transportSpecific    4-bits (HI)   0    */
#define ZL303XX_PTP_V2_HDR_MSG_TYPE_OFST  (Uint16T)(0)  /* messageType          4-bits (LO)   0    */
                                                      /* RESERVED             4-bits (HI)   1    */
#define ZL303XX_PTP_V2_HDR_VERSION_OFST   (Uint16T)(1)  /* versionPTP           4-bits (LO)   1    */
#define ZL303XX_PTP_V2_HDR_MSG_LEN_OFST   (Uint16T)(2)  /* messageLength            2         2    */
#define ZL303XX_PTP_V2_HDR_DOMAIN_OFST    (Uint16T)(4)  /* domainNumber             1         4    */
                                                      /* RESERVED                 1         5    */
#define ZL303XX_PTP_V2_HDR_FLAGS_OFST     (Uint16T)(6)  /* flagField                2         6    */
#define ZL303XX_PTP_V2_HDR_CORR_OFST      (Uint16T)(8)  /* correctionField          8         8    */
                                                      /* RESERVED                 4        16    */
#define ZL303XX_PTP_V2_HDR_SRC_PORT_OFST  (Uint16T)(20) /* sourcePortIdentity      10        20    */
#define ZL303XX_PTP_V2_HDR_SEQ_ID_OFST    (Uint16T)(30) /* sequenceId               2        30    */
#define ZL303XX_PTP_V2_HDR_CTRL_OFST      (Uint16T)(32) /* controlField             1        32    */
#define ZL303XX_PTP_V2_HDR_INTERVAL_OFST  (Uint16T)(33) /* logMessageInterval       1        33    */
                                                      /* ----------------------------------------*/
#define ZL303XX_PTP_V2_HEADER_LEN         (Uint16T)(34) /* Total Length            34              */


/****** PTP V2 Announce Data Composition  ******/
/* The following Definitions/Offsets are relative to the start of the Announce Data,
 * not from the beginning of the PTP packet. */
                                                        /*         IEEE-1588-2008: Table 25        */
                                                        /* ----------------------------------------*/
                                                        /* Field                Length    Offset   */
                                                        /* ----------------------------------------*/
#define ZL303XX_PTP_ANNC_TIMESTAMP_OFST     (Uint8T)(0)   /* originTimestamp         10         0    */
#define ZL303XX_PTP_ANNC_UTC_OFFSET_OFST    (Uint8T)(10)  /* currentUtcOffset         2        10    */
                                                        /* RESERVED                 1        12    */
#define ZL303XX_PTP_ANNC_GM_PRIORITY1_OFST  (Uint8T)(13)  /* grandmasterPriority1     1        13    */
#define ZL303XX_PTP_ANNC_GM_QUALITY_OFST    (Uint8T)(14)  /* grandmasterClockQuality  4        14    */
#define ZL303XX_PTP_ANNC_GM_PRIORITY2_OFST  (Uint8T)(18)  /* grandmasterPriority2     1        18    */
#define ZL303XX_PTP_ANNC_GM_ID_OFST         (Uint8T)(19)  /* grandmasterIdentity      8        19    */
#define ZL303XX_PTP_ANNC_STEPS_REMOVE_OFST  (Uint8T)(27)  /* stepsRemoved             2        27    */
#define ZL303XX_PTP_ANNC_TIME_SOURCE_OFST   (Uint8T)(29)  /* timeSource               1        29    */
                                                        /* ----------------------------------------*/
#define ZL303XX_PTP_V2_ANNC_DATA_LEN        (Uint8T)(30)  /* Total Length            30              */

/* Announce Message Length (including PTP Header).  */
#define ZL303XX_PTP_V2_ANNC_MSG_LEN         (Uint8T)(ZL303XX_PTP_V2_ANNC_DATA_LEN + ZL303XX_PTP_V2_HEADER_LEN)


/****** PTP V2 SYNC Data Composition  ******/
/* Uses the general Timestamp Overlay Data composition (defined above). */
/* Refer to IEEE-1588-2008: Table 26        */
#define ZL303XX_PTP_V2_SYNC_DATA_LEN        ZL303XX_PTP_OVERLAY_TIMESTAMP_LEN
#define ZL303XX_PTP_V2_SYNC_MSG_LEN         (Uint16T)(ZL303XX_PTP_V2_SYNC_DATA_LEN + ZL303XX_PTP_V2_HEADER_LEN)

/****** PTP V2 Delay Request Data Composition  ******/
/* Uses the general Timestamp Overlay Data composition (defined above). */
/* Refer to IEEE-1588-2008: Table 26        */
#define ZL303XX_PTP_V2_DREQ_DATA_LEN        ZL303XX_PTP_OVERLAY_TIMESTAMP_LEN
#define ZL303XX_PTP_V2_DREQ_MSG_LEN         (Uint16T)(ZL303XX_PTP_V2_DREQ_DATA_LEN + ZL303XX_PTP_V2_HEADER_LEN)

/****** PTP V2 Peer Delay Request Data Composition  ******/
/* Uses the general Timestamp & PortId Overlay Data composition (defined above). */
/* Refer to IEEE-1588-2008: Table 29        */
#define ZL303XX_PTP_V2_PDREQ_DATA_LEN       ZL303XX_PTP_OVERLAY_TS_PORTID_LEN
#define ZL303XX_PTP_V2_PDREQ_MSG_LEN        (Uint16T)(ZL303XX_PTP_V2_PDREQ_DATA_LEN + ZL303XX_PTP_V2_HEADER_LEN)

/****** PTP V2 Peer Delay Response Data Composition  ******/
/* Uses the general Timestamp & PortId Overlay Data composition (defined above). */
/* Refer to IEEE-1588-2008: Table 30        */
#define ZL303XX_PTP_V2_PDRESP_DATA_LEN      ZL303XX_PTP_OVERLAY_TS_PORTID_LEN
#define ZL303XX_PTP_V2_PDRESP_MSG_LEN       (Uint16T)(ZL303XX_PTP_V2_PDRESP_DATA_LEN + ZL303XX_PTP_V2_HEADER_LEN)

/****** PTP V2 Follow-Up Data Composition  ******/
/* Follow-Up messages are the same as the SYNC in terms of composition.
 * However, its data member is 'preciseOriginTimestamp' instead of
 * 'originTimestamp' (Refer to IEEE-1588-2008: Table 27). */
#define ZL303XX_PTP_V2_FOLLOW_UP_DATA_LEN   ZL303XX_PTP_OVERLAY_TIMESTAMP_LEN
#define ZL303XX_PTP_V2_FOLLOW_UP_MSG_LEN    (Uint16T)(ZL303XX_PTP_V2_FOLLOW_UP_DATA_LEN + ZL303XX_PTP_V2_HEADER_LEN)

/****** PTP V2 Delay Response Data Composition  ******/
/* Uses the general Timestamp & PortId Overlay Data composition (defined above). */
/* Refer to IEEE-1588-2008: Table 28        */
#define ZL303XX_PTP_V2_DRESP_DATA_LEN       ZL303XX_PTP_OVERLAY_TS_PORTID_LEN
#define ZL303XX_PTP_V2_DRESP_MSG_LEN        (Uint16T)(ZL303XX_PTP_V2_DRESP_DATA_LEN + ZL303XX_PTP_V2_HEADER_LEN)

/****** PTP V2 Peer Delay Response Follow Up Data Composition  ******/
/* Uses the general Timestamp & PortId Overlay Data composition (defined above). */
/* Refer to IEEE-1588-2008: Table 31        */
#define ZL303XX_PTP_V2_PDRESP_FUP_DATA_LEN  ZL303XX_PTP_OVERLAY_TS_PORTID_LEN
#define ZL303XX_PTP_V2_PDRESP_FUP_MSG_LEN   (Uint16T)(ZL303XX_PTP_V2_PDRESP_FUP_DATA_LEN + ZL303XX_PTP_V2_HEADER_LEN)

/****** PTP V2 Signaling Message Data Composition  ******/
/* The following Definitions/Offsets are relative to the start of the Signaling Data,
 * not from the beginning of the PTP packet. */

/*         IEEE-1588-2008: Table 33        */
/* ----------------------------------------*/
/* Field                Length    Offset   */
/* ----------------------------------------*/
/* targetPortIdentity      10         0    */
/* One or more TLV's        M        10    */
/* ----------------------------------------*/
/* Total Length          10 + M            */

#define ZL303XX_PTP_V2_SIGNAL_DATA_LEN      (Uint16T)(ZL303XX_PTP_MSG_PORT_IDENTITY_LEN)
#define ZL303XX_PTP_V2_SIGNAL_MSG_LEN       (Uint16T)(ZL303XX_PTP_V2_SIGNAL_DATA_LEN + ZL303XX_PTP_V2_HEADER_LEN)

/****** PTP V2 Management Message Data Composition  ******/
/* The following Definitions/Offsets are relative to the start of the Management Data,
 * not from the beginning of the PTP packet. */

/*         IEEE-1588-2008: Table 37        */
/* ----------------------------------------*/
/* Field                Length    Offset   */
/* ----------------------------------------*/
/* targetPortIdentity      10         0    */
/* startingBoundaryHops     1        10    */
/* boundaryHops             1        11    */
/* RESERVED             4-bits (HI)  12    */
/* actionField          4-bits (LO)  12    */
/* RESERVED                 1        13    */
/* One or more TLV's        M        14    */
/* ----------------------------------------*/
/* Total Length          14 + M            */

#define ZL303XX_PTP_V2_MANAGEMENT_DATA_LEN     (Uint16T)(14)
#define ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN      (Uint16T)(ZL303XX_PTP_V2_MANAGEMENT_DATA_LEN + ZL303XX_PTP_V2_HEADER_LEN)



/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/***********************************************************************
 * PTP V2 Message Header Structure (Standard).
 ***********************************************************************/
typedef struct zl303xx_PtpV2MsgHeaderS
{
   /* Transport Specific data. */
   Uint4T   transportSpecific;

   /* Message Type (SYNC/ANNC. etc. ). */
   Uint4T   messageType;

   /* V1 & V2: PTP Version of the message.
    * For both protocols, this field is located 1 byte from the beginning of
    * the packet & is 4-bits long (lower 4-bits of the byte). */
   Uint4T   versionPTP;

   /* Length (in bytes) of the entire PTP message. */
   Uint16T  messageLength;

   /* Clock Domain. */
   Uint8T   domainNumber;

   /* The 2-octets of packet header flags. Stored as a Uint16T type but packed
    * and unpacked in network order in the packets.   */
   Uint16T   flagField;

   /* Transparent Clock Only. */
   Uint64S  correctionField;

   /* Source Port Identity. */
   zl303xx_PortIdentity sourcePortIdentity;

   /* Message sequence number. */
   Uint16T  sequenceId;

   /* Control Value for V1 backwards-compatibility.
    * For both protocols, this field is located 32 bytes from the beginning of
    * the packet & is 1 byte long. */
   Uint8T   controlField;

   /* The Message interval is determined by the messageType. */
   Sint8T   logMessageInterval;
} zl303xx_PtpV2MsgHeaderS;


/***********************************************************************
 * PTP V2 Message Definition Structures (Refer to the Standard
 * zl303xx_MessageTypeE definition).
 ***********************************************************************/
/* Several message data portions have just a Timestamp value so create
 * a generic type so that function parameters are compatible.  */
/* Event message Data format */
typedef struct
{
   zl303xx_TimeStamp  originTimestamp;
} zl303xx_PtpV2MsgDataTsS;

/* Several Message types all have a Timestamp and Port Identity entry.
 * Create this generic type so that a single function will be compatible to
 * handle all these messages.  */
typedef struct
{
   zl303xx_TimeStamp      timestamp;
   zl303xx_PortIdentity   portIdentity;
} zl303xx_PtpV2MsgDataTsPortIdS;

/* SYNC message format */
typedef zl303xx_PtpV2MsgDataTsS zl303xx_PtpV2MsgSyncS;

/* DELAY_REQ message format */
typedef zl303xx_PtpV2MsgDataTsS zl303xx_PtpV2MsgDelayReqS;

/* PDelay_Req has just a timestamp (but also 10 reserved bytes on-the-wire). */
/* Choose to structure it as below since we want to clear the extra bits. */
typedef struct
{
   zl303xx_TimeStamp      originTimestamp;
   zl303xx_PortIdentity   reservedPortId;
} zl303xx_PtpV2MsgPdelayReqS;

/* PDelay_Resp message (same layout as zl303xx_PtpV2MsgDataTsPortIdS). */
typedef struct
{
   zl303xx_TimeStamp      requestReceiptTimestamp;
   zl303xx_PortIdentity   requestingPortIdentity;
} zl303xx_PtpV2MsgPdelayRespS;

/* FOLLOW_UP message */
typedef struct
{
   zl303xx_TimeStamp      preciseOriginTimestamp;
} zl303xx_PtpV2MsgFollowUpS;

/* DELAY_RESP message */
typedef struct
{
   zl303xx_TimeStamp      receiveTimestamp;
   zl303xx_PortIdentity   requestingPortIdentity;
} zl303xx_PtpV2MsgDelayRespS;

/* PDelay_Resp_Follow_Up message (same layout as zl303xx_PtpV2MsgDataTsPortIdS). */
typedef struct
{
   zl303xx_TimeStamp      responseOriginTimestamp;
   zl303xx_PortIdentity   requestingPortIdentity;
} zl303xx_PtpV2MsgPdelayFollowUpS;

/* ANNOUNCE message */
typedef struct
{
   zl303xx_TimeStamp      originTimestamp;
   Sint16T              currentUtcOffset;
   Uint8T               grandmasterPriority1;
   zl303xx_ClockQuality   grandmasterClockQuality;
   Uint8T               grandmasterPriority2;
   zl303xx_ClockIdentity  grandmasterIdentity;
   Uint16T              stepsRemoved;
   Uint8T               timeSource;
} zl303xx_PtpV2MsgAnnounceS;

/* Signaling message */
typedef struct
{
   zl303xx_PortIdentity   targetPortIdentity;
} zl303xx_PtpV2MsgSignalingS;

/* Management message */
typedef struct
{
   zl303xx_PortIdentity      targetPortIdentity;
   Uint8T                  startingBoundaryHops;
   Uint8T                  boundaryHops;
   zl303xx_PtpV2MgmtActionE  actionField;
} zl303xx_PtpV2MsgManagementS;


/***********************************************************************
 * PTP V2 Message Definition union of all message structures.
 ***********************************************************************/
typedef union
{
   /* EVENT Messages */
   zl303xx_PtpV2MsgSyncS        sync;
   zl303xx_PtpV2MsgDelayReqS    delayReq;
   zl303xx_PtpV2MsgPdelayReqS   pdelayReq;
   zl303xx_PtpV2MsgPdelayRespS  pdelayResp;

   /* GENERAL Messages */
   zl303xx_PtpV2MsgFollowUpS       followUp;
   zl303xx_PtpV2MsgDelayRespS      delayResp;
   zl303xx_PtpV2MsgPdelayFollowUpS pdelayFollowUp;
   zl303xx_PtpV2MsgAnnounceS       announce;
   zl303xx_PtpV2MsgSignalingS      signaling;
   zl303xx_PtpV2MsgManagementS     management;

   /* Used as overlays for messages with multiple common members. */
   zl303xx_PtpV2MsgDataTsS         tStamp;
   zl303xx_PtpV2MsgDataTsPortIdS   tStampAndPortId;
} zl303xx_PtpV2MsgDataU;


/***********************************************************************
 * General PTP V2 message structure.
 ***********************************************************************/
typedef struct
{
   zl303xx_PtpV2MsgHeaderS   header;
   zl303xx_PtpV2MsgDataU     data;
} zl303xx_PtpV2MsgS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/*****************   PTP MESSAGE FUNCTION DECLARATIONS   **********************/


#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
