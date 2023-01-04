

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Definitions and utility functions for parsing (packing/unpacking) PTP
*     messages to/from buffers. These include PTP headers, messageType data
*     blocks (i.e. SYNC data, SIGNALING data, etc.) as well as individual data
*     field parsing.
*
******************************************************************************/

#ifndef _ZL303XX_PTP_V2_MSG_PARSE_H_
#define _ZL303XX_PTP_V2_MSG_PARSE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"

#include "zl303xx_PtpV2MsgTypes.h"
#include "zl303xx_PtpDatatypes.h"

/* Definitions for Signaling and Management TLVs */
#include "zl303xx_PtpV2TlvApi.h"            /* For General TLV Parsing + Other TLVs. */
#include "zl303xx_PtpV2SignalTlvParse.h"    /* For Signal TLV Parsing. */
#include "zl303xx_PtpV2MgmtTlvParse.h"      /* For Management TLV Parsing. */

/*****************   DEFINES   ************************************************/

/******** General PACK macros for fields of standard sizes. *********/
/******************************************************************************/
#ifdef OS_VXWORKS
#define PACK_4BIT(addr,val,shift)               \
   do {                                         \
      /* Clear the target bits first */         \
      *(addr) &= ~((Uint8T)(0x0F << shift));    \
                                                \
      /* Fill in the new value */               \
      *(addr) |= (val & 0x0F) << shift;         \
   } while (0)

#else

#define PACK_4BIT(addr,val,shift)               \
   do {                                         \
      /* Clear the target bits first */         \
      *(addr) &= (Uint8T)(~((Uint8T)(0x0F << shift)));    \
      /* Fill in the new value */               \
      *(addr) = (Uint8T)(*(addr) | (Uint8T)((val & 0x0F) << shift)); /* long winded way to do it to silence conversion warnings for |= */ \
      } while (0)
#endif

#define PACK_8BIT(addr,val)               (*(Uint8T*)(addr) = (Uint8T)(val))

#define PACK_16BIT(addr,val)  do { PACK_8BIT((addr),     ((Uint16T)(val) >>  8) & 0xFF); \
                                   PACK_8BIT((addr) + 1,  (Uint16T)(val)        & 0xFF); } while (0)

#define PACK_32BIT(addr, val) do { PACK_8BIT((addr),     ((Uint32T)(val) >> 24) & 0xFF); \
                                   PACK_8BIT((addr) + 1, ((Uint32T)(val) >> 16) & 0xFF); \
                                   PACK_8BIT((addr) + 2, ((Uint32T)(val) >>  8) & 0xFF); \
                                   PACK_8BIT((addr) + 3,  (Uint32T)(val)        & 0xFF); } while (0)

#define PACK_64BIT(addr,val)                           \
   do {                                                \
      PACK_32BIT((addr), val.hi);                      \
      PACK_32BIT((addr + sizeof(Uint32T)), val.lo);    \
   } while (0)

#define PACK_LO_NIBBLE(addr,val)          PACK_4BIT(addr,val,0)
#define PACK_HI_NIBBLE(addr,val)          PACK_4BIT(addr,val,4)

/**** General UNPACK macros ****/

#define UNPACK_4BIT(addr,shift)     ((*(Uint8T*)(addr) >> shift) & 0x0F)
#define UNPACK_8BIT(addr)           (*(Uint8T*)(addr))
#define UNPACK_16BIT(addr)          (((Uint16T)*(Uint8T*)(addr) <<  8) | ((Uint16T)*(Uint8T*)((addr) + 1)))
#define UNPACK_32BIT(addr)          (((Uint32T)*(Uint8T*)(addr) << 24) | ((Uint32T)*(Uint8T*)((addr) + 1) << 16) | \
                                     ((Uint32T)*(Uint8T*)((addr) + 2) <<  8) | ((Uint32T)*(Uint8T*)((addr) + 3)))

#define UNPACK_64BIT(addr,val)                           \
   do {                                                  \
      (val).hi = UNPACK_32BIT(addr);                     \
      (val).lo = UNPACK_32BIT(addr + sizeof(Uint32T));   \
   } while (0)

#define UNPACK_LO_NIBBLE(addr)      UNPACK_4BIT(addr,0)
#define UNPACK_HI_NIBBLE(addr)      UNPACK_4BIT(addr,4)
/******************************************************************************/

#define MAX_LEN(A,B)       ((A) > (B) ? (A) : (B))
#define MIN_LEN(A,B)       ((A) > (B) ? (B) : (A))

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/*****************   PTP MESSAGE PARSING FUNCTION DECLARATIONS   **************/

/**** PTP MESSAGE HEADER Parsing Routines. ****/
/**********************************************/
Uint16T zl303xx_PtpV2MsgHeaderUnpack(
      Uint8T *pInHdrBuf,
      zl303xx_PtpV2MsgHeaderS *pPtpMsgHeader);
Uint16T zl303xx_PtpV2MsgHeaderPack(
      zl303xx_PtpV2MsgHeaderS *pPtpMsgHeader,
      Uint8T *pOutHdrBuf);

/* Prepare a Header for a specific stream. */
Uint16T zl303xx_PtpV2MsgHeaderPackForStream(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_MessageTypeE messageType,
      Uint16T *sequenceId,
      Uint8T *pOutHdrBuf);

/**** SYNC Message & DATA Parsing Routines. ****/
/***********************************************/
/* Routines for only the Data portion of a SYNC message. */
/* (Map to the generic Event Data parse routines). */
#define zl303xx_PtpV2MsgDataSyncUnpack         zl303xx_PtpV2MsgDataTimestampUnpack

/* Prepare a SYNC message for a specific stream. */
#define zl303xx_PtpV2MsgSyncPackForStream(pPtpStream, pSyncTs, pOutMsgBuf) \
            zl303xx_PtpV2MsgTimePortIdPackForStream((pPtpStream),(NULL),(NULL),(ZL303XX_MSG_ID_SYNC),(pOutMsgBuf))

/**** DELAY_REQUEST Message & DATA Parsing Routines. ****/
/********************************************************/
/* Routines for only the Data portion of a DELAY_REQUEST message. */
/* (Map to the generic Event Data parse routines). */
#define zl303xx_PtpV2MsgDataDelayReqUnpack     zl303xx_PtpV2MsgDataTimestampUnpack

/* Prepare a DELAY_REQUEST message for a specific stream. */
#define zl303xx_PtpV2MsgDelayReqPackForStream(pPtpStream, pDReqTs, pOutMsgBuf)  \
           zl303xx_PtpV2MsgTimePortIdPackForStream((pPtpStream),(NULL),(NULL),(ZL303XX_MSG_ID_DELAY_REQ),(pOutMsgBuf))

/**** PEER_DELAY_REQ Message & Data Parsing Routines ****/
/***************************************************/
/* Routines for the data portion of a PEER_DELAY_REQ message. */
/* (Map to the generic TIMESTAMP + PORT_ID Data parse routines). */
#define zl303xx_PtpV2MsgDataPDelayReqUnpack    zl303xx_PtpV2MsgDataTimestampUnpack

/* Prepare a PEER_DELAY_REQ message for a specific stream. */
#define zl303xx_PtpV2MsgPDelayReqPackForStream(pPtpStream, pPDelayReqTs, pOutMsgBuf)  \
           zl303xx_PtpV2MsgTimePortIdPackForStream((pPtpStream),(NULL),(NULL),(ZL303XX_MSG_ID_PEER_DELAY_REQ),(pOutMsgBuf))

/**** PEER_DELAY_RESP Message & Data Parsing Routines ****/
/***************************************************/
/* Routines for the data portion of a PEER_DELAY_RESP message. */
/* (Map to the generic TIMESTAMP + PORT_ID Data parse routines). */
#define zl303xx_PtpV2MsgDataPDelayRespUnpack   zl303xx_PtpV2MsgDataTsPortIdUnpack

/* Prepare a PEER_DELAY_RESP message for a specific stream. */
#define zl303xx_PtpV2MsgPDelayRespPackForStream(pPtpStream, pRxHdr, pPDelayRespTs, pOutMsgBuf)  \
           zl303xx_PtpV2MsgTimePortIdPackForStream((pPtpStream),(pRxHdr),(pPDelayRespTs),(ZL303XX_MSG_ID_PEER_DELAY_RESP),(pOutMsgBuf))

/**** Follow_Up Message & Data Parsing Routines ****/
/***************************************************/
/* Routines for the data portion of a Follow_Up message. */
/* (Map to the generic Event Data parse routines). */
#define zl303xx_PtpV2MsgDataFollowUpUnpack     zl303xx_PtpV2MsgDataTimestampUnpack

/* Create a Follow-up Message for a particular Stream.  */
#define zl303xx_PtpV2MsgFollowUpPackForStream(pPtpStream, pRxHdr, pFollowUpTs, pOutMsgBuf) \
            zl303xx_PtpV2MsgTimePortIdPackForStream((pPtpStream),(pRxHdr),(pFollowUpTs),ZL303XX_MSG_ID_FOLLOWUP,(pOutMsgBuf))

/**** Delay_Resp Message & Data Parsing Routines ****/
/****************************************************/
/* Routines for the data portion of a Delay_Resp message. */
#define zl303xx_PtpV2MsgDataDelayRespUnpack    zl303xx_PtpV2MsgDataTsPortIdUnpack

/* Pack a DELAY_RESP packet. */
#define zl303xx_PtpV2MsgDelayRespPackForStream(pPtpStream, pRxHdr, pDelayReqRxTs, pOutMsgBuf) \
            zl303xx_PtpV2MsgTimePortIdPackForStream((pPtpStream),(pRxHdr),(pDelayReqRxTs),ZL303XX_MSG_ID_DELAY_RESP,(pOutMsgBuf))

/**** PEER_DELAY_RESP_FOLLOWUP Message & Data Parsing Routines ****/
/******************************************************************/
/* Routines for the data portion of a PEER_DELAY_RESP_FOLLOWUP message. */
/* (Map to the generic TIMESTAMP + PORT_ID Data parse routines). */
#define zl303xx_PtpV2MsgDataPDelayFollowUpUnpack  zl303xx_PtpV2MsgDataTsPortIdUnpack

/* Prepare a PEER_DELAY_RESP_FOLLOWUP message for a specific stream. */
#define zl303xx_PtpV2MsgPDelayFollowUpPackForStream(pPtpStream, pRxHdr, pTimestamp, pOutMsgBuf)  \
           zl303xx_PtpV2MsgTimePortIdPackForStream((pPtpStream),(pRxHdr),(pTimestamp),(ZL303XX_MSG_ID_PEER_DELAY_FOLLOWUP),(pOutMsgBuf))

/**** Announce Message & Data Parsing Routines ****/
/**************************************************/
/* Routines for only the data portion of an Announce message. */
Uint16T zl303xx_PtpV2MsgDataAnncPack(
      zl303xx_PtpV2MsgAnnounceS *pAnncDataIn,
      Uint8T *pAnncDataBufOut);
Uint16T zl303xx_PtpV2MsgDataAnncUnpack(
      Uint8T *pAnncDataBufIn,
      zl303xx_PtpV2MsgDataU *pAnncDataOut);

/* Routines for both the header & data portion of an Announce message. */
Uint16T zl303xx_PtpV2MsgAnncPack(
      zl303xx_ClockStreamS *pClock,
      Uint8T *buf,
      zl303xx_PtpV2MsgAnnounceS *pAnncData);

/**** SIGNAL DATA Parsing Routines. ****/
/***************************************/
Uint16T zl303xx_PtpV2MsgDataSignalUnpack(
      Uint8T *pSignalDataBuf,
      zl303xx_PtpV2MsgDataU *pSignalData);
Uint16T zl303xx_PtpV2MsgDataSignalPack(
      zl303xx_PtpV2MsgSignalingS *pSignalData,
      Uint8T *pSignalDataBuf);


/**** MANAGEMENT Message & DATA Parsing Routines. ****/
/*****************************************************/
Uint16T zl303xx_PtpV2MsgDataMgmtUnpack(
          Uint8T *pMgmtDataBuf,
           zl303xx_PtpV2MsgDataU *pMgmtData);
Uint16T zl303xx_PtpV2MsgDataMgmtPack(
          zl303xx_PtpV2MsgManagementS *pMgmtData,
          Uint8T *pMgmtDataBuf);

Uint16T zl303xx_PtpV2MsgMgmtPack(
      zl303xx_PtpV2MsgHeaderS *pPtpMsgHeader,
      zl303xx_PtpV2MsgManagementS *pInMgmtData,
      Uint8T *pOutMgmtMsgBuf);

/****************   GENERIC PARSING FUNCTION DEFINITIONS FOR MESSAGES WITH SAME FORMAT  ******/

/* Routines for only the Data portion of a TIMESTAMP message. These have only
 * an 'originTimestamp' as a data member. (Used for SYNC, DELAY_REQ & FOLLOW_UP). */
Uint16T zl303xx_PtpV2MsgDataTimestampUnpack(
      Uint8T                *pInTsDataBuf,
      zl303xx_PtpV2MsgDataU   *pOutTsDataS);

/* Routines for only the Data portion of a TIMESTAMP and Port Identity message.
 * These have a 'timestamp' and 'portIdentity' as data members. (Used for
 * DELAY_RESP, PDELAY_REQ, PDELAY_RESP & PDELAY_FOLLOW_UP). */
Uint16T zl303xx_PtpV2MsgDataTsPortIdUnpack(
      Uint8T               *pInTsPortDataBuf,
      zl303xx_PtpV2MsgDataU  *pOutTsPortDataS);

Uint16T zl303xx_PtpV2MsgTimePortIdPackForStream(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgHeaderS *pRxMsgHdr,      /* May be NULL */
      zl303xx_TimeStamp *pInTimestamp,         /* May be NULL */
      zl303xx_MessageTypeE ptpMsgType,
      Uint8T *pOutTsPortIdMsgBuf);

/*****************   PTP MESSAGE FRAGMENT PARSING FUNCTION DECLARATIONS   *****/

/* Individual fields within message data blocks.
 * Separate routines are created to take advantage of code reuse. */

/**** CLOCK IDENTITY utilities ****/
Uint16T zl303xx_PtpV2MsgClockIdentityUnpack(
      Uint8T *pClockIdBuf,
      zl303xx_ClockIdentity clockIdentity);
Uint16T zl303xx_PtpV2MsgClockIdentityPack(
      zl303xx_ClockIdentity clockIdentity,
      Uint8T *pClockIdBuf);

/**** CLOCK QUALITY utilities ****/
Uint16T zl303xx_PtpV2MsgClockQualityUnpack(
           Uint8T *pClockQualityBuf,
           zl303xx_ClockQuality *pClockQuality);
Uint16T zl303xx_PtpV2MsgClockQualityPack(
           zl303xx_ClockQuality *pClockQuality,
           Uint8T *pClockQualityBuf);

/**** PORT IDENTITY utilities ****/
Uint16T zl303xx_PtpV2MsgPortIdentityUnpack(
      Uint8T *pPortIdBuf,
      zl303xx_PortIdentity *pPortIdentity);
Uint16T  zl303xx_PtpV2MsgPortIdentityPack(
      zl303xx_PortIdentity *pPortIdentity,
      Uint8T *pPortIdBuf);

/**** PTP TIMESTAMP utilities ***/
Uint16T zl303xx_PtpV2MsgTimestampUnpack(
      Uint8T          *pTimestampBuf,
      zl303xx_TimeStamp *pOutTimestamp);
Uint16T zl303xx_PtpV2MsgTimestampPack(
      zl303xx_TimeStamp *pInTimestamp,
      Uint8T          *pTimestampBuf);

/**** PROTOCOL ADDRESS utilities ****/
Uint16T zl303xx_PtpV2MsgProtocolAddressUnpack(
      Uint8T *pInProtoAddrBuf,
      zl303xx_PortAddress *pOutProtoAddrS);
Uint16T zl303xx_PtpV2MsgProtocolAddressPack(
      zl303xx_PortAddress *pInProtoAddrS,
      Uint8T *pOutProtoAddrBuf);

/*****************   OTHER PARSING UTILITY FUNCTION DECLARATIONS   ************/

/* Routine to extract the PTP version from a message header buffer.
 * This is useful when:
 *    - GET: the PTP message is initially received and a decision on whether
 *           V1 or V2 handling is required.   */
zl303xx_PtpVersionE zl303xx_PtpMsgVersionGet(Uint8T *pInHdrBuf);

/* Extract or Set the messageLength field in a packed PTP header buffer.
 * This is useful when:
 *    - GET: only the length of a Rx packet is required for error checking, etc.
 *    - SET: the PTP packet length is variable (Signaling & Management messages)
 *           and the final length is not known until completely built. */
Uint16T zl303xx_PtpV2MsgPktLengthGet(Uint8T *pInHdrBuf);
Uint16T zl303xx_PtpV2MsgPktLengthSet(Uint16T messageLength, Uint8T *pOutHdrBuf);

/* Extract or Set the sequenceId field in a packed PTP header buffer.
 * This is useful when:
 *    - GET: only the sequenceId of a Rx packet is required to verify it was
 *           sent by the local node OR to reply to the sending node.
 *    - SET: the sequenceId is set by some external process/routine and is not
 *           known at the time of initial packing.             */
Uint16T zl303xx_PtpV2MsgSequenceNumGet(Uint8T *pInHdrBuf);
Uint16T zl303xx_PtpV2MsgSequenceNumSet(Uint16T sequenceId, Uint8T *pOutHdrBuf);

/* Set the correctionField value in a packed PTP header buffer.
 * This is useful when:
 *    - SET: replying to a Peer/Delay_Req with a RESP and the received
 *           correctionField value must be inserted into the RESP.    */
Uint16T zl303xx_PtpV2MsgCorrectionFieldSet(Uint64S correctionField, Uint8T *pOutHdrBuf);

/*****************   OTHER PTP MESSAGE UTILITY FUNCTION DECLARATIONS   ********/

/* Get the Minimum Length of a PTP Message (Header + DATA + tlvType (if any)). */
Uint16T zl303xx_PtpV2MsgLengthOfType(zl303xx_MessageTypeE messageType);

/* Displays all the fields of a PTP header structure. */
void zl303xx_PtpV2MsgHeaderTrace(zl303xx_PtpV2MsgHeaderS *pPtpMsgHeader, Uint16T modId, Uint8T level);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
