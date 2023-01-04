

/*****************************************************************************
* 
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
* Copyright 2006-2018 Microsemi Semiconductor Limited.
* All rights reserved.
*
*  Module Description:
*     This file contains the protocol state machines for the
*     IEEE-1588-2008 ptp protocol
*
*******************************************************************************/

/****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Os.h"
#include "zl303xx_Trace.h"
#include "zl303xx_ErrTrace.h"

#include "zl303xx_Macros.h"
#include "zl303xx_Ptsf.h"

#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpV2MsgParse.h"      /* Include all message Parsing definitions. */
#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpControl.h"
#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpV2AcceptMaster.h"
#include "zl303xx_PtpForeignMasterTbl.h"
#include "zl303xx_PtpV2MgmtTlvHandle.h"
#include "zl303xx_PtpProfileDef.h"
#include "zl303xx_PtpG8265p1Api.h"
#include "zl303xx_PtpStrings.h"

#include "zl303xx_PtpG8275p1Types.h"
#include "zl303xx_ExamplePtpGlobals.h"
#include "f_rrh_def.h"
#include "f_rrh_inc.h"
#include "BPF_RU_IPCM.h"
#include "BPF_COM_LOG.h"
#include "BPF_HM_DEVC.h"
#include "BPF_COM_CMD.h"
#include "f_com_inc.h"

/****************   DEFINES     **********************************************/

/****************   TYPEDEFS    **********************************************/

/* Generic format for the PTP V2 packet handlers. Not all parameters are used
 * for all handlers but they are included to keep things uniform. The function
 * must return number of bytes processed (usually messageLength) or 0 if an
 * error occurs. */
typedef Uint16T (*zl303xx_PktHandlerFnT)(zl303xx_PtpClockMsgQueueS *ptpData,
                                          zl303xx_PtpV2MsgS *msg,
                                          zl303xx_ClockStreamS *streamData,
                                          zl303xx_PortDataS *portData);

/* Information about the parameters for handling each of the PTP packets. */
typedef struct
{
   /* Short description (for logging, etc. ) */
   const char *desc;

   /* Some PTP messages (ANNC, SYNC, etc.) required that a Stream exist in order
    * that the packet be processed. Management & Signaling do not have this
    * requirement since it may be a request to negotiate a stream (signaling)
    * or a non-stream related management request (TLV). */
   zl303xx_BooleanE streamRequired;

   /* Minimum length required, INCLUDING the common header (header is
    * currently 44 bytes). */
   Uint16T minLength;

   /* Function to unpack the data section for a this messageType. */
   Uint16T (*unpack)(Uint8T *buf, zl303xx_PtpV2MsgDataU *data);

   /* What to do when a packet of a certain type is received. */
   /* If this is set to NULL, it indicates that the PTP message type is not
    * implemented (or supported in the case of RESERVED types). */
   zl303xx_PktHandlerFnT rxHandlerFn;
} zl303xx_PtpV2PktProcessingDefS;

/****************   STATIC FUNCTION DECLARATIONS   ***************************/

/* Called by zl303xx_PtpV2handleRecvMsg to checks all of the necessary PTP
 * packet parameters before processing. (Moved to a separate routine since it
 * was getting rather long in the process handler itself).  */
_ZL303XX_LOCAL zlStatusE zl303xx_PtpV2CheckRecvPktParameters(zl303xx_PtpClockMsgQueueS *ptpMsg, zl303xx_PtpV2MsgHeaderS *pktHdr, zl303xx_PortDataS *portData);
static zlStatusE zl303xx_PtpV2CheckRecvStream(zl303xx_MessageTypeE msgType, zl303xx_PortDataS *portData, zl303xx_ClockStreamS *streamData);

/* Local Routines called by zl303xx_PtpV2handleRecvMsg to handle each of the
 * individual received PTP packet (message) types. */
_ZL303XX_LOCAL Uint16T zl303xx_PtpV2handleSync             (zl303xx_PtpClockMsgQueueS *ptpData, zl303xx_PtpV2MsgS *msg, zl303xx_ClockStreamS *streamData, zl303xx_PortDataS *portData);
_ZL303XX_LOCAL Uint16T zl303xx_PtpV2handleDelayReq         (zl303xx_PtpClockMsgQueueS *ptpData, zl303xx_PtpV2MsgS *msg, zl303xx_ClockStreamS *streamData, zl303xx_PortDataS *portData);
_ZL303XX_LOCAL Uint16T zl303xx_PtpV2handlePeerDelayReq     (zl303xx_PtpClockMsgQueueS *ptpData, zl303xx_PtpV2MsgS *msg, zl303xx_ClockStreamS *streamData, zl303xx_PortDataS *portData);
_ZL303XX_LOCAL Uint16T zl303xx_PtpV2handlePeerDelayResp    (zl303xx_PtpClockMsgQueueS *ptpData, zl303xx_PtpV2MsgS *msg, zl303xx_ClockStreamS *streamData, zl303xx_PortDataS *portData);
_ZL303XX_LOCAL Uint16T zl303xx_PtpV2handleFollowUp         (zl303xx_PtpClockMsgQueueS *ptpData, zl303xx_PtpV2MsgS *msg, zl303xx_ClockStreamS *streamData, zl303xx_PortDataS *portData);
_ZL303XX_LOCAL Uint16T zl303xx_PtpV2handleDelayResp        (zl303xx_PtpClockMsgQueueS *ptpData, zl303xx_PtpV2MsgS *msg, zl303xx_ClockStreamS *streamData, zl303xx_PortDataS *portData);
_ZL303XX_LOCAL Uint16T zl303xx_PtpV2handlePeerDelayFollowUp(zl303xx_PtpClockMsgQueueS *ptpData, zl303xx_PtpV2MsgS *msg, zl303xx_ClockStreamS *streamData, zl303xx_PortDataS *portData);
_ZL303XX_LOCAL Uint16T zl303xx_PtpV2handleAnnounce         (zl303xx_PtpClockMsgQueueS *ptpData, zl303xx_PtpV2MsgS *msg, zl303xx_ClockStreamS *streamData, zl303xx_PortDataS *portData);
_ZL303XX_LOCAL Uint16T zl303xx_PtpV2handleSignaling        (zl303xx_PtpClockMsgQueueS *ptpData, zl303xx_PtpV2MsgS *msg, zl303xx_ClockStreamS *streamData, zl303xx_PortDataS *portData);
_ZL303XX_LOCAL Uint16T zl303xx_PtpV2handleManagement       (zl303xx_PtpClockMsgQueueS *ptpData, zl303xx_PtpV2MsgS *msg, zl303xx_ClockStreamS *streamData, zl303xx_PortDataS *portData);

_ZL303XX_LOCAL Uint16T zl303xx_PtpV2MsgMgmtHandle          (zl303xx_PtpClockMsgQueueS *pPtpRxQueueData, zl303xx_PtpV2MsgS *pPtpMsgRx, zl303xx_ClockStreamS *pPtpStream, zl303xx_PortDataS *pPtpPort);


_ZL303XX_LOCAL void zl303xx_PtpV2DoInitStream(zl303xx_ClockStreamS *streamData);
_ZL303XX_LOCAL void zl303xx_PtpV2DoInitPort(zl303xx_PortDataS *portData);
_ZL303XX_LOCAL void zl303xx_PtpV2InitStreamData(zl303xx_ClockStreamS *streamData);
_ZL303XX_LOCAL void zl303xx_PtpV2DoStreamState(zl303xx_ClockStreamS *streamData);

_ZL303XX_LOCAL void zl303xx_PtpV2IssueDelayResp(
      zl303xx_TimeStamp *delayReqRecvTime,
      zl303xx_PtpV2MsgHeaderS *header,
      zl303xx_ClockStreamS *streamData);

static zl303xx_BooleanE zl303xx_PtpV2MixedModePktHandler(zl303xx_PtpClockMsgQueueS *ptpMsg,
                                                       zl303xx_PtpV2MsgHeaderS *pktHdr,
                                                       zl303xx_PortDataS *port,
                                                       zl303xx_ClockStreamS **ppStream);

static zl303xx_PtpV2MgmtMsgScopeE zl303xx_PtpV2MgmtMsgEvaluateScope(zl303xx_PortIdentity *targetPortIdentity);

static void zl303xx_PtpV2McastIntvlDetect(zl303xx_ClockStreamS *pStream, zl303xx_MessageTypeE type,
                                          Sint8T intvl);

static zl303xx_BooleanE zl303xx_PtpV2StreamStateChangeValid(
                              zl303xx_ClockStreamS *pStream,
                              zl303xx_PtpStreamSubStateE nextState);

static Uint16T zl303xx_PtpMsgAppendEventPadding(
      zl303xx_ClockStreamS *streamData,
      zl303xx_MessageTypeE messageType,
      Uint16T currMessageLen);


/****************   STATIC GLOBAL VARIABLES   ********************************/

/* Strings corresponding to the zl303xx_PtpV2MgmtActionE type */
const char *Zl303xx_ManagementActionStr[] = {
      "GET", "SET", "RSP", "CMD", "ACK"
};

/* Strings corresponding to the zl303xx_PtpPortInternalStateE type */
static const char *Zl303xx_PtpPortInternalStateStr[] = {
      "INITIALIZING", "ACTIVE", "DISABLED", "FAULTY"
};

zl303xx_PtpV2PktProcessingDefS ptpV2PktProcess[ZL303XX_PTP_V2_NUM_MSG_IDS] = {
      /* Description       stream-Required?  Minimum Packet Length                              Unpack Function                         Receive Packet Handler     */
      { "SYNC",                 ZL303XX_TRUE,  ZL303XX_PTP_V2_SYNC_MSG_LEN,                         zl303xx_PtpV2MsgDataSyncUnpack,           zl303xx_PtpV2handleSync              },
      { "DELAY REQUEST",        ZL303XX_TRUE,  ZL303XX_PTP_V2_DREQ_MSG_LEN,                         zl303xx_PtpV2MsgDataDelayReqUnpack,       zl303xx_PtpV2handleDelayReq          },
      { "PEER DELAY REQUEST",   ZL303XX_TRUE,  ZL303XX_PTP_V2_PDREQ_MSG_LEN,                        zl303xx_PtpV2MsgDataPDelayReqUnpack,      zl303xx_PtpV2handlePeerDelayReq      },
      { "PEER DELAY RESPONSE",  ZL303XX_TRUE,  ZL303XX_PTP_V2_PDRESP_MSG_LEN,                       zl303xx_PtpV2MsgDataPDelayRespUnpack,     zl303xx_PtpV2handlePeerDelayResp     },
      { "RESERVED",             ZL303XX_FALSE, 0,                                                 NULL,                                   NULL  /* Reserved */               },
      { "RESERVED",             ZL303XX_FALSE, 0,                                                 NULL,                                   NULL  /* Reserved */               },
      { "RESERVED",             ZL303XX_FALSE, 0,                                                 NULL,                                   NULL  /* Reserved */               },
      { "RESERVED",             ZL303XX_FALSE, 0,                                                 NULL,                                   NULL  /* Reserved */               },
      { "FOLLOW UP",            ZL303XX_TRUE,  ZL303XX_PTP_V2_FOLLOW_UP_MSG_LEN,                    zl303xx_PtpV2MsgDataFollowUpUnpack,       zl303xx_PtpV2handleFollowUp          },
      { "DELAY RESPONSE",       ZL303XX_TRUE,  ZL303XX_PTP_V2_DRESP_MSG_LEN,                        zl303xx_PtpV2MsgDataDelayRespUnpack,      zl303xx_PtpV2handleDelayResp         },
      { "PEER DELAY FOLLOW UP", ZL303XX_TRUE,  ZL303XX_PTP_V2_PDRESP_FUP_MSG_LEN,                   zl303xx_PtpV2MsgDataPDelayFollowUpUnpack, zl303xx_PtpV2handlePeerDelayFollowUp },
      { "ANNOUNCE",             ZL303XX_TRUE,  ZL303XX_PTP_V2_ANNC_MSG_LEN,                         zl303xx_PtpV2MsgDataAnncUnpack,           zl303xx_PtpV2handleAnnounce          },
                                             /* Signaling packets MUST have at least 1 TLV. */
      { "SIGNALING",            ZL303XX_FALSE, ZL303XX_PTP_V2_SIGNAL_MSG_LEN + 2*sizeof(Uint16T),   zl303xx_PtpV2MsgDataSignalUnpack,         zl303xx_PtpV2handleSignaling         },
                                             /* Management packets may have 0 or 1 TLVs. */
      { "MANAGEMENT",           ZL303XX_FALSE, ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN,                   zl303xx_PtpV2MsgDataMgmtUnpack,           zl303xx_PtpV2handleManagement        } };

/* Announceメッセージ上のClockClass値保持用 */
int	keep_announce_clockclass = 0;		/* 初期値0 */

/****************   EXPORTED GLOBAL VARIABLES   ******************************/

/****************   EXTERNAL GLOBAL VARIABLES   ******************************/
/* EEPROMから取得したaccepted-clock-classes */
/* Announce解析時(zl303xx_PtpV2handleAnnounce)に使用 */
extern int accepted_clock_classes;

/****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* zl303xx_PtpV2Protocol */
/** 

   Runs port and stream state machines for IEEE 1588-2008 (PTPv2).

  Parameters:
   [in]   portData    Pointer to a port data structure.
   [in]   streamData  Pointer to a stream data structure. If non-NULL, only
                           the state machine for this stream will run. If NULL,
                           the state machine for all streams will run.

*******************************************************************************/
void zl303xx_PtpV2Protocol(zl303xx_PortDataS *portData,
                           zl303xx_ClockStreamS *streamData)
{
   /* Always execute the port state machine */
   zl303xx_PtpDoPortState(portData);

   /* Run the state machine for the stream, if it was provided. If it wasn't,
    * run it for all streams. */
   if (streamData != NULL)
   {
      zl303xx_PtpV2DoStreamState(streamData);
   }
   else
   {
      ZL303XX_PTP_FOR_EACH_STREAM(&portData->streamList, streamData)
      {
         zl303xx_PtpV2DoStreamState(streamData);
      }
   }
}

/* zl303xx_PtpDoPortState */
/** 

   Function to implement the port superstate state machine for IEEE 1588.

  Parameters:
   [in]   portData  Pointer to the data for the port

  Notes:
   This function handles the overall state of the port and is generic to
   both v1 and v2.

*******************************************************************************/
void zl303xx_PtpDoPortState(zl303xx_PortDataS *portData)
{
   switch (portData->portSuperState)
   {
      case PTP_PORT_INT_STATE_INIT :
         zl303xx_PtpV2ToPortState(portData, PTP_PORT_INT_STATE_ACTIVE);
         break;

      case PTP_PORT_INT_STATE_FAULTY :
         /* Only let the port go to init if faultPassThroughEn is ZL303XX_TRUE
          * The port will loop in faulty if faultPassThroughEn is ZL303XX_FALSE */
        if(portData->config.faultPassThroughEn == ZL303XX_TRUE)
        {
           /* imaginary troubleshooting here */
           ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                 "Port number %d, event FAULT_CLEARED",
                 portData->config.portNumber, 0,0,0,0,0);

           zl303xx_PtpV2ToPortState(portData, PTP_PORT_INT_STATE_INIT);
        }
         break;

      case PTP_PORT_INT_STATE_DISABLED :
      case PTP_PORT_INT_STATE_ACTIVE :
      default :
          break;
   }
}

/* zl303xx_PtpV2ToPortState */
/** 

   Function to implement a port state transition from the current state into
   into the specified state

  Parameters:
   [in,out]  portData   Pointer to the data for the port
   [in]      nextState  The state to enter

*******************************************************************************/
void zl303xx_PtpV2ToPortState(zl303xx_PortDataS *portData,
                              zl303xx_PtpPortInternalStateE nextState)
{
   zl303xx_ClockStreamS *stream;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "Port number %u: state %s",
         portData->config.portNumber,
         Zl303xx_PtpPortInternalStateStr[nextState], 0,0,0,0);

   /* No leaving state tasks */
   /* Entering state tasks */
   switch (nextState)
   {
      case PTP_PORT_INT_STATE_INIT :
         zl303xx_PtpV2DoInitPort(portData);

         ZL303XX_PTP_FOR_EACH_STREAM(&portData->streamList, stream)
         {
            zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_INIT, stream);
         }
         break;

      case PTP_PORT_INT_STATE_DISABLED :
      case PTP_PORT_INT_STATE_FAULTY :
         if(portData->faultyStateCount < 0xFFFFFFFF)
         {
            portData->faultyStateCount++;
         }
         ZL303XX_PTP_FOR_EACH_STREAM(&portData->streamList, stream)
         {
            zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_IDLE, stream);
         }
         break;

      case PTP_PORT_INT_STATE_ACTIVE :
      default :
         break;
   }

   portData->portSuperState = nextState;
}

/* zl303xx_PtpMessageTypeToStr */
/**
   Converts a zl303xx_MessageTypeE enum value to a (const char *).

  Parameters:
   [in]   msgType   Message type to convert.

  Return Value:   (const char *)

*******************************************************************************/
const char *zl303xx_PtpMessageTypeToStr(zl303xx_MessageTypeE msgType)
{
   if (msgType < ZL303XX_PTP_V2_NUM_MSG_IDS)
   {
      return ptpV2PktProcess[msgType].desc;
   }

   return "INVALID";
}

/****************   STATIC FUNCTION DEFINITIONS   *****************************/

/*

  Function Name:
    zl303xx_PtpV2DoInitPort

  Details:
   Initializes a version 2 PTP clock port

  Parameters:
   [in]    portData    Pointer to the clock port instance structure

  Return Value:
   Nothing

  Notes:
   This is called on startup and for each subsequent re-initialization

*******************************************************************************/

void zl303xx_PtpV2DoInitPort(zl303xx_PortDataS *portData)
{
   zl303xx_PtpClockS *pPtpClock = portData->clock;
   char *desc = NULL, *rev = NULL, *user = NULL;

   zl303xx_PtpNodeDescriptionGet(NULL, &desc, &rev, &user);

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "Manufacturer Identity: %s. Revision %s",
         desc, rev, 0,0,0,0);

   if (OS_STRLEN(user) > 0)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "User Description: %s", user, 0,0,0,0,0);
   }

   /* Reset the malformed packet count. This should only be cleared when the
    * port is truly initialized */
   portData->malformedPktCount = 0;

   /* Also the unicast failure count. */
   portData->failedUnicastRequestCount = 0;

   /* Display debug output, if enabled */
   ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_ENGINE, 2)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "Clock port %u initial configuration:  ", portData->config.portNumber, 0,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4, "  class: %d  ", pPtpClock->config.defaultDS.clockQuality.clockClass, 0,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4, "  clockAccuracy: 0x%02X  ", pPtpClock->config.defaultDS.clockQuality.clockAccuracy, 0,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 5, "  scaledLogVariance: 0x%04X  ",  pPtpClock->config.defaultDS.clockQuality.offsetScaledLogVariance, 0,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3, "  priority1: %d, priority2: %d  ", pPtpClock->config.defaultDS.priority1, pPtpClock->config.defaultDS.priority2, 0,0,0,0);
      ZL303XX_TRACE_V2_PORT_ID(ZL303XX_MOD_ID_PTP_ENGINE, 2, "  portIdentity: ", &portData->portDS.portIdentity);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3, "PTP domain_number : %d  ", pPtpClock->config.defaultDS.domainNumber, 0,0,0,0,0);
   }  /* ZL303XX_DEBUG_FILTER() */
}

/*

  Function Name:
    zl303xx_PtpV2DoInitStream

  Details:
   Initializes a version 2 PTP clock port stream

  Parameters:
   [in]    streamData    Pointer to the clock stream instance structure

  Return Value:
   Nothing

  Notes:
   This is called on startup and for each subsequent re-initialization

*******************************************************************************/
void zl303xx_PtpV2DoInitStream(zl303xx_ClockStreamS *streamData)
{
   /* Reset the clock stream calculated (dynamic) values. */
   zl303xx_PtpV2InitStreamData(streamData);

   /* Initialize version specific members */

   /* Display debug output, if enabled */
   ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_ENGINE, 2)
   {
      zl303xx_TimeStamp interval;
      const char *streamModeStr = zl303xx_PtpStreamModeToStr(streamData->config.mode);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
            "Stream %u initial configuration:  ",
            streamData->clockStreamHandle, 0,0,0,0,0);

      zl303xx_PtpGetAnnounceReceiptTimeout(streamData, &interval);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
            "  ANNOUNCE message timeout: " PTPD_FORMAT_TIME_STR,
             formatTimeStringSign(&interval),
            formatTimeStringSec(&interval),
            formatTimeStringNs(&interval), 0,0,0);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
            "  logSyncInterval: %d  ",
            streamData->config.logSyncInterval, 0,0,0,0,0);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
            "  logAnnounceInterval: %d  ",
            streamData->config.logAnnounceInterval, 0,0,0,0,0);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4,
            "  logMinDelayReqInterval: %d  ",
            streamData->config.logMinDelayReqInterval, 0,0,0,0,0);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4,
            "  log_min_mean_pdelay_req_interval: %d  ",
            streamData->u.v2.log_min_mean_pdelay_req_interval, 0,0,0,0,0);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
            "  requested stream mode: %s", streamModeStr, 0,0,0,0,0);
   }  /* ZL303XX_DEBUG_FILTER() */
}


/* zl303xx_PtpV2DoStreamState */
/** 

   Perform clock stream specific state actions according to V2 protocol

  Parameters:
   [in]   streamData  Pointer to clock stream instance structure

*******************************************************************************/
void zl303xx_PtpV2DoStreamState(zl303xx_ClockStreamS *streamData)
{
    /* Force any state transitions that are necessary */
    const char *fnName = "zl303xx_PtpV2DoStreamState";

   ZL303XX_CHECK_POINTERS(streamData, streamData->portData);
   if (!streamData || !streamData->portData)
       return;  /* Too early? */

   /* Unicast portions of mixed streams, should track the state of the
    * multicast path.  */
   if ((streamData->portData->config.mixedMode == ZL303XX_TRUE) &&
       (streamData->config.unicast == ZL303XX_TRUE) &&
       (streamData->portData->mcastStream != NULL))
   {
      /* If the stream's parent is the same as the clock parent (i.e. the
       * stream is getting DREQ from the server) synchronize its state to
       * that of the multicast stream.
       * Otherwise, kill the reverse path for this unneeded stream.   */
      if (zl303xx_PtpV2EqualPortIdentities(&streamData->farEndParentDS.u.v2.parentPortIdentity,
                        &streamData->portData->clock->parentDS.parentPortIdentity) == ZL303XX_TRUE)
      {
         if (streamData->streamSubstate != streamData->portData->mcastStream->streamSubstate)
         {
            zl303xx_PtpV2ToStreamState(streamData->portData->mcastStream->streamSubstate, streamData);
         }
      }

      /* Remove this stream */
      else if (streamData->streamSubstate != PTP_STREAM_SUBSTATE_PASSIVE)
      {
         zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_PASSIVE, streamData);
      }
   }

   switch (streamData->streamSubstate)
   {
      case PTP_STREAM_SUBSTATE_INIT :
      {
         zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_IDLE, streamData);
         break;
      }

      case PTP_STREAM_SUBSTATE_IDLE :
      {
         /* This is not treated the same as DISABLED for this implementation.
          * Any streams that are disabled should move to a PASSIVE state. */

         /* For SLAVE_ONLY stream or a BMC stream on a slaveOnly clock, move
          * immediately to LISTENING. */
         /* If a UMT entry exists, only move to LISTENING if an ANNOUNCE
          * flow is being requested. */
         if (((streamData->config.mode != ZL303XX_PTP_STREAM_SLAVE_ONLY) &&
              (streamData->portData->clock->config.defaultDS.slaveOnly != ZL303XX_TRUE))
              ||
              (streamData->pUmtEntry == NULL) ||
              (streamData->pUmtEntry->entryConfig.reqAnnc == ZL303XX_TRUE))
         {
            zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_LISTENING, streamData);
         }
         /* ELSE: Do nothing, Remain in IDLE until an ANNC request is issued. */

         break;
      }

      case PTP_STREAM_SUBSTATE_PRE_MASTER :
         /* If the Timer is already active then do nothing. The PTP Task routine
          * zl303xx_PtpHandleTimerTick() will handle the Expiry when necessary. */
         /* Otherwise, start the timer if necessary (M3) or go straight to MASTER
          * if M1 or M2.    */
         if (zl303xx_PtpTimerActive(&streamData->itimer[PRE_MASTER_TIMER]) == ZL303XX_FALSE)
         {
			 
            /* For M1 & M2 masters:
             * - no timer will be started (active timers will be stopped).
             * - move immediately to MASTER. */
            if ((zl303xx_PtpClockIsOwnMaster(streamData->portData->clock) == ZL303XX_TRUE) ||
                (streamData->portData->clock->currentDSInt.stepsRemoved == 0)) /* For misconfiguration */
            {
               /* Stop any active timer and go to MASTER. */
               zl303xx_PtpTimerStop(&streamData->itimer[PRE_MASTER_TIMER]);
               zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_MASTER, streamData);
            }

            /* M3: start the required Timer if able to do so. */
            else if (zl303xx_PtpStreamTimerStart(streamData, PRE_MASTER_TIMER) != ZL303XX_OK)
            {
               /* Jump to MASTER State if the Timer failed to start. */
               zl303xx_PtpTimerStop(&streamData->itimer[PRE_MASTER_TIMER]);
               zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_MASTER, streamData);
            }
         }
         break;

      case PTP_STREAM_SUBSTATE_MASTER :
      {
         /* SLAVE_ONLY streams should never be in this state so move back to
          * LISTENING state and let the state machine figure it out. */
         if (streamData->config.mode == ZL303XX_PTP_STREAM_SLAVE_ONLY)
         {
            ZL303XX_TRACE_ALWAYS("%s: SLAVE-ONLY Stream (%u), in state %d",
                  fnName,
                  streamData->clockStreamHandle,
                  streamData->streamSubstate, 0,0,0);

            zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_LISTENING, streamData);
         }

         break;
      }

      case PTP_STREAM_SUBSTATE_UNCALIBRATED :
	  {
		  
		 /* If the Timer is required and active then wait for it to expire. */
		 /* If the Timer is required and inactive then it is possible it has not
		  * yet started (so initiate) OR it has already expired (continue to
		  * custom checks). */
		 /* If the Timer is not required then just evaluate the custom trigger. */
		 if (streamData->config.uncalibrated.anncIntervals > 0)
		 {
			if (zl303xx_PtpTimerActive(&streamData->itimer[UNCALIBRATED_TIMER]) == ZL303XX_FALSE)
			{
			   /* If this required timer was never started then start it now. */
			   if (streamData->uncalTimerExpired == ZL303XX_FALSE)
			   {
				  (void)zl303xx_PtpStreamTimerStart(streamData, UNCALIBRATED_TIMER);
				  break;
			   }
			   else
			   {
				  /* The required Timer has already expired so do nothing here.
				   * Let it drop to evaluate custom triggers. */
			   }
			}

			/* Timer has not yet expired so do nothing further. */
			else
			{
			   break;
			}
		 }

		 /* Timer requirements are met so evaluate the LOCK status (if required). */
		 if (streamData->config.uncalibrated.lockRequired == ZL303XX_TRUE)
		 {
			Uint32T lockStatus;

			/* Continue in the current state if the Hardware is still uncalibrated. */
			if ((ZL303XX_OK == zl303xx_PtpClockLockStatusGet(streamData->portData->clock, &lockStatus)) &&
				(lockStatus == ZL303XX_FALSE))
			{
			   break;
			}
		 }
			 
		  

         /* If we make it here then both Timer and Custom triggers are clear. */
         {
            streamData->uncalTimerExpired = ZL303XX_FALSE;

            if (streamData == streamData->portData->clock->gmSource.pPtpStream)
            {
               zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_SLAVE, streamData);
            }
         }

         break;
      }

      case PTP_STREAM_SUBSTATE_SLAVE :
      {
         /* If LOCK is lost while in SLAVE state, re-evaluate the Stream state. */
         if (streamData->config.uncalibrated.lockRequired == ZL303XX_TRUE)
         {
            Uint32T lockStatus;

            /* Continue in the current state if the Hardware is still uncalibrated. */
            if ((ZL303XX_OK == zl303xx_PtpClockLockStatusGet(streamData->portData->clock, &lockStatus)) &&
                (lockStatus == ZL303XX_FALSE))
            {
               zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_UNCALIBRATED, streamData);
            }
         }

         /* FALL-THROUGH */
      }

      case PTP_STREAM_SUBSTATE_LISTENING :
      {
         /* MASTER_ONLY streams should enter any of these states so move back
          * to PRE_MASTER state and let the state machine figure it out. */
         if (streamData->config.mode == ZL303XX_PTP_STREAM_MASTER_ONLY)
         {
            /* Perhaps a glitch in the State machine that needs fixing?
             * If this ever gets hit, investigate. */
            ZL303XX_TRACE_ALWAYS("%s: MASTER-ONLY Stream (%u), in state %d",
                  fnName,
                  streamData->clockStreamHandle,
                  streamData->streamSubstate, 0,0,0);

            zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_PRE_MASTER, streamData);
         }
      }

      /* Fall-through intentional */
      case PTP_STREAM_SUBSTATE_PASSIVE :
      {
         if ((streamData->config.unicast == ZL303XX_TRUE) &&
             (streamData->pUmtEntry != NULL) &&
             (!streamData->pUmtEntry->entryConfig.reqAnnc))
         {
            if (streamData->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].remainingSec == 0)
            {
               zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_IDLE, streamData);
            }
         }
         break;
      }

      default:
      {
         break;
      }
   }

   /* Unicast stream state machine must come after the overall state machine,
    * so changes in state will be handled immediately. */
   if ((streamData->config.unicast == ZL303XX_TRUE) &&
       (streamData->portData->config.unicastNegotiation.enabled))
   {
      _zl303xx_PtpV2DoUnicastState(streamData);
   }
}  /* END zl303xx_PtpV2DoStreamState */

/* zl303xx_PtpV2ToStreamState */
/** 

   Perform actions on a state transition for the stream state machine.
   i.e., when leaving current state and entering 'state', according to the V2
   protocol definition.

  Parameters:
   [in]   nextState   The state to enter
   [in]   streamData  Pointer to the stream data instance structure

*******************************************************************************/
void zl303xx_PtpV2ToStreamState(zl303xx_PtpStreamSubStateE nextState,
                                zl303xx_ClockStreamS *streamData)
{
   zl303xx_BooleanE bmcaFlagStatus = ZL303XX_TRUE;
   const char *fnName = "zl303xx_PtpV2ToStreamState";
   zlStatusE status = ZL303XX_OK;
   
   /* check for NULL pointer */
   if (streamData == NULL)
   {
      ZL303XX_TRACE_ALWAYS("%s:: pointer to ClockStream == NULL",
            fnName,0, 0,0,0,0);

      return;
   }
   
   /* Get status of PP_FLAG */
   status = zl303xx_PtpClockBmcaFlagStatusGet(streamData->portData->clock->clockHandle, PP_FLAG, &bmcaFlagStatus);
   if (status == ZL303XX_OK && !bmcaFlagStatus && 
   nextState == PTP_STREAM_SUBSTATE_PRE_MASTER) /* Do not use PRE-MASTER state */
   {
       ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "%s:: Immediately moving to MASTER state, and skipping PRE_MASTER state because PP_FLAG is false.",
         fnName,0,0,0,0,0);
	   nextState = PTP_STREAM_SUBSTATE_MASTER;   
   }
   
   /* Get status of PU_FLAG */
   bmcaFlagStatus = ZL303XX_TRUE;
   status = zl303xx_PtpClockBmcaFlagStatusGet(streamData->portData->clock->clockHandle, PU_FLAG, &bmcaFlagStatus);
   if (status == ZL303XX_OK && !bmcaFlagStatus && 
   nextState == PTP_STREAM_SUBSTATE_UNCALIBRATED) /* Do not use UNCALIBRATED state */
   {
       ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "%s:: Immediately moving to SLAVE state, and skipping UNCALIBRATED state because PU_FLAG is false.",
         fnName,0,0,0,0,0);
	   nextState = PTP_STREAM_SUBSTATE_SLAVE;
   }
		
   if (!zl303xx_PtpV2StreamStateChangeValid(streamData, nextState))
   {
      return;
   }

   ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_STATE_UPD, 1)  /* 18 */
   {
      const char *currStateStr = zl303xx_PtpStreamSubStateToStr(streamData->streamSubstate);
      const char *nextStateStr = zl303xx_PtpStreamSubStateToStr(nextState);

      ZL303XX_TRACE_ALWAYS(
            "zl303xx_PtpV2ToStreamState: Stream %u: state change from %s (%d) to %s (%d)",
            streamData->clockStreamHandle,
            currStateStr, streamData->streamSubstate,
            nextStateStr, nextState, 0);
   }

   /* Leaving state tasks */
   if (streamData->streamSubstate == PTP_STREAM_SUBSTATE_INIT)
   {
      /* Reset the stream counters and calculated members for the stream.
       * All configured data is unaffected. */
      zl303xx_PtpV2DoInitStream(streamData);
   }

   /* Entering state tasks */
   if (nextState == PTP_STREAM_SUBSTATE_IDLE)
   {
      /* Set all PTP-specific PTSF flags to FALSE. */
      zl303xx_PtpStreamPtsfReset(streamData);
   }

   streamData->streamSubstate = nextState;

   /* Start/stop timers based on the new state. */
   zl303xx_PtpV2SetRequiredTimers(streamData);
}  /* END zl303xx_PtpV2ToStreamState */

/* zl303xx_PtpV2StreamStateChangeValid */
/** 

   Helper function for zl303xx_PtpV2ToStreamState(). Determines if the requested
   state change is valid.

  Parameters:
   [in]   pStream    Stream to evaluate.
   [in]   nextState  Requested state transition

*******************************************************************************/
static zl303xx_BooleanE zl303xx_PtpV2StreamStateChangeValid(
                              zl303xx_ClockStreamS *pStream,
                              zl303xx_PtpStreamSubStateE nextState)
{
   /* Do not allow transition to same state */
   if (pStream->streamSubstate == nextState)
   {
      return ZL303XX_FALSE;
   }

   /* INIT -> IDLE is the only allowed transition (from INIT) */
   if (pStream->streamSubstate == PTP_STREAM_SUBSTATE_INIT &&
       nextState != PTP_STREAM_SUBSTATE_IDLE)
   {
      return ZL303XX_FALSE;
   }

   /* Only allow transition out of INIT or IDLE when port state is active */
   if ((pStream->streamSubstate == PTP_STREAM_SUBSTATE_INIT ||
        pStream->streamSubstate == PTP_STREAM_SUBSTATE_IDLE)  &&
       (pStream->portData->portSuperState != PTP_PORT_INT_STATE_ACTIVE))
   {
      return ZL303XX_FALSE;
   }

   return ZL303XX_TRUE;
}

/*

  Function Name:
   zl303xx_GetV2PortState

  Details:
   Determines the state of the specified port based on the state(s) of its
   associated streams (if any).

  Parameters:
   [in]    portData    Pointer to the port data structure

  Return Value:
   zl303xx_PortStateE

 *****************************************************************************/
zl303xx_PortStateE zl303xx_GetV2PortState(zl303xx_PortDataS *portData)
{
   zl303xx_PortStateE v2StateValue;

   if (portData == NULL)
   {
      return ZL303XX_PORT_STATE_FAULTY;
   }

   /* Special case for Virtual Ports. */
   if (portData->virtualData.isVirtualPort == ZL303XX_TRUE)
   {
      /* If this is the selected port of the ParentDS then it is considered SLAVE.
       * If not, return PASSIVE since UNCALIBRATED interfere with the clockState
       * decision (and since there are no packets, PASSIVE makes sense).     */
      if(zl303xx_PtpV2EqualPortIdentities(
                      &portData->portDS.portIdentity,
                      &portData->clock->gmSource.extComparisonData.receivingPortIdentity))
      {
         return ZL303XX_PORT_STATE_SLAVE;
      }
      else
      {
         return ZL303XX_PORT_STATE_PASSIVE;
      }
   }

   switch (portData->portSuperState)
   {
      /* For each of the possible "INACTIVE" situations just return the actual
       * Port State.    */
      case PTP_PORT_INT_STATE_INIT :
      {
         v2StateValue = ZL303XX_PORT_STATE_INITIALIZING;
         break;
      }
      case PTP_PORT_INT_STATE_FAULTY :
      {
         v2StateValue = ZL303XX_PORT_STATE_FAULTY;
         break;
      }
      case PTP_PORT_INT_STATE_DISABLED :
      {
         v2StateValue = ZL303XX_PORT_STATE_DISABLED;
         break;
      }

      /* For this implementation, the individual Streams associated with the
       * Port track their own individual States. The Port is ACTIVE in either
       * case. The following case determines the Port state for the Stream
       * states using the following rules:
       * - If a Multicast stream is present (multicast & mixed mode): the
       *   state of the multicast stream is returned as the Port State.
       * - Only Unicast Streams present: Base the decision on the primary
       *   stream's sub-state.
       * - No Stream is present: return ZL303XX_PORT_STATE_DISABLED
       * */
      case PTP_PORT_INT_STATE_ACTIVE :
      {
         /* Set the default to the lowest possible Port state. */
         zl303xx_PtpStreamSubStateE primaryStreamState = PTP_STREAM_SUBSTATE_INIT;

         /* If a Multicast stream is present, use it. */
         if (portData->mcastStream != NULL)
         {
            primaryStreamState = portData->mcastStream->streamSubstate;
         }

         /* If no streams are present, the Port is effectively in-effective. */
         else if (portData->streamCount == 0)
         {
            /* If there are no Streams on the Port then set the stream State to INIT. */
            primaryStreamState = PTP_STREAM_SUBSTATE_INIT;
         }

         /* No Multicast but Unicast streams abound. */
         else  /* (portData->streamCount > 0) */
         {
            zl303xx_ClockStreamS *pPtpStream = NULL;

            ZL303XX_PTP_FOR_EACH_STREAM(&portData->streamList, pPtpStream)
            {
               if (pPtpStream->streamSubstate > primaryStreamState)
               {
                  primaryStreamState = pPtpStream->streamSubstate;
               }
            }
         }

         switch (primaryStreamState)    /* The STDs port state enums are used by higher level s/w */
         {
            case PTP_STREAM_SUBSTATE_INIT :
            case PTP_STREAM_SUBSTATE_IDLE :
            {
               /* Either no stream created yet or the existing stream hasn't finished Initializing. */
               v2StateValue = ZL303XX_PORT_STATE_INITIALIZING;
               break;
            }
            case PTP_STREAM_SUBSTATE_PASSIVE :
            {
               v2StateValue = ZL303XX_PORT_STATE_PASSIVE;
               break;
            }
            case PTP_STREAM_SUBSTATE_LISTENING :
            {
               v2StateValue = ZL303XX_PORT_STATE_LISTENING;
               break;
            }
            case PTP_STREAM_SUBSTATE_PRE_MASTER :
            {
               v2StateValue = ZL303XX_PORT_STATE_PRE_MASTER;
               break;
            }
            case PTP_STREAM_SUBSTATE_UNCALIBRATED :
            {
               v2StateValue = ZL303XX_PORT_STATE_UNCALIBRATED;
               break;
            }
            case PTP_STREAM_SUBSTATE_MASTER :
            {
               v2StateValue = ZL303XX_PORT_STATE_MASTER;
               break;
            }
            case PTP_STREAM_SUBSTATE_SLAVE :
            {
               v2StateValue = ZL303XX_PORT_STATE_SLAVE;
               break;
            }
            default :
            {
               ZL303XX_TRACE_ALWAYS(
                     "zl303xx_GetV2PortState: Invalid sub-stream state (%d): Port raw state (%d)",
                     primaryStreamState,
                     portData->portSuperState, 0,0,0,0);

               v2StateValue = ZL303XX_PORT_STATE_FAULTY;
               break;
            }
         }

         /* Telecom Profile: A slave clock should always report the slave state. */
         if ((portData->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1) &&
             (portData->clock->config.defaultDS.slaveOnly == ZL303XX_TRUE))
         {
            v2StateValue = ZL303XX_PORT_STATE_SLAVE;
         }

         break;
      }

      default :
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_GetV2PortState: Invalid Port state (%d)",
               portData->portSuperState, 0,0,0,0,0);

         v2StateValue = ZL303XX_PORT_STATE_FAULTY;
         break;
      }
   }

   return v2StateValue;
}  /* END zl303xx_GetV2PortState */

/*

  Function Name:
    zl303xx_PtpV2handleRecvMsg

  Details:
   Perform actions on a received packet using the v2 protocol

  Parameters:
   [in]  streamData  Pointer to the clock stream instance structure.
                           Could be NULL if there is no clock stream identified
                           yet (as in the case of some management TLVs &
                           Unicast Negotiation signaling).
   [in]  portData    Pointer to the port data instance structure.
                           Can never be NULL as the message must have been
                           received on a physical port.
   [in]  ptpData     The received message (from the task message queue)
                           which contains the received packet.

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpV2handleRecvMsg(zl303xx_ClockStreamS *streamData,
                                zl303xx_PortDataS *portData,
                                zl303xx_PtpClockMsgQueueS *ptpData)
{
   Uint16T offset, processed;

   /* A local area to unpack the packet for processing. */
   zl303xx_PtpV2MsgS msg;

   if (portData == NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
            "Port not provided", 0,0,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Port not provided", 0,0,0,0,0,0);
      return;
   }

   if (portData->portSuperState != PTP_PORT_INT_STATE_ACTIVE)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
            "Port %u inactive -> disregarding received message",
            portData->config.portNumber, 0,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Port %u inactive -> disregarding received message",
            portData->config.portNumber, 0,0,0,0,0);
      return;
   }

   /* Unpack the message (packet) Header to get access to the data needed.
    * The ptpMsg pointer must be valid so do not check it here (see the
    * reasoning in the CHECK routine, below). */
   /* Note: The following zl303xx_PtpV2MsgHeaderUnpack call assumes the packet is of
    *       V2 type. Even if it is not, the parsing for the version number will
    *       still work since it is in the same place for both V1 & V2.
    *       An alternative is to call the GetVersionNumber routine before
    *       parsing but it is unlikely we will ever receive V1 packets (& the
    *       check below will work in either case).  */
   offset = zl303xx_PtpV2MsgHeaderUnpack(ptpData->msgBuf, &msg.header);

   /* Check the following items related to the received packet (PTP message):
    * ------------------------------------------------------------------------
    * - PTP Version: must be compatible with the portData (expect v2).
    * - domain: must be compatible with the clock configuration.
    * - messageType:
    *    - range: must be between 0 (SYNC) & 13 (Management).
    *    - implemented: Peer-Delay message are not implemented. there are also
    *          some reserved values that should be ignored.
    * - sourcePortIdentity: ignore packets that somehow originated from the
    *       local port.
    * - malformed packet:
    *    - packet check: verifies the length set in the packet header field with
    *          the minimum required for the message type
    *    - task message queue check: verifies the length set in the task message
    *          queue bufLen member with the packet header field to make sure the
    *          packet wasn't truncated
    * - alternate master configuration: check for a mismatch in the local and far
    *       end ALTERNATE_MASTER configuration
    * ------------------------------------------------------------------------ */
   if (zl303xx_PtpV2CheckRecvPktParameters(ptpData, &msg.header, portData) != ZL303XX_OK)
   {
      /* Return immediately since there is no STATUS variable locally.
       * Any logging would have already occurred inside the CHECK routine. */
      return;
   }

   if (portData->config.mixedMode == ZL303XX_TRUE)
   {
      (void)zl303xx_PtpV2MixedModePktHandler(ptpData, &msg.header, portData, &streamData);
   }

   /* Check that a stream exists, if applicable to this message type */
   if (zl303xx_PtpV2CheckRecvStream(msg.header.messageType, portData, streamData) != ZL303XX_OK)
   {
      return;
   }

   /* At this point, the message has passed the preliminary verification. */
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
         "Received PTP: message type %d :  ",
         msg.header.messageType, 0,0,0,0,0);
   ZL303XX_TRACE_V2_PORT_ID(ZL303XX_MOD_ID_PTP_ENGINE, 4,
         "                                 from portIdentity ",
         &msg.header.sourcePortIdentity);

   /* Update the RX counters for this message type, if the stream data is available. */
   if (streamData != NULL)
   {
      streamData->counters.msg[msg.header.messageType].rx++;
      streamData->countersInt.msg[msg.header.messageType].rx++;

      zl303xx_PtpStreamLostMsgCheck(streamData, &msg);
   }

   /* Determine if we have to filter packets of acceptable masters only.
    * This implementation allows filtering for multicast & unicast streams. */
   if (portData->config.acceptableMasterTableEnabled == ZL303XX_TRUE)
   {
      zl303xx_PtpAccMasterEntryS *amtEntry;

      amtEntry = zl303xx_PtpTableFind(&portData->accMasterTbl,
                                      &ptpData->recvMsgInfo.srcAddr);
      if (amtEntry != NULL)
      {
         /* If this is an ANNOUNCE message and the alternatePriority1 value is
          * not 0, then the grandmasterPriority1 value in the pack needs to be
          * replaced with the local value. (Refer to IEEE-1588-2008, Table 91). */
         if ((msg.header.messageType == ZL303XX_MSG_ID_ANNOUNCE) &&
             (amtEntry->alternatePriority1 != 0))
         {
            Uint8T *anncData = ptpData->msgBuf + ZL303XX_PTP_V2_HEADER_LEN;

            /* This MACRO requires the beginning of the ANNOUNCE data portion
             * of the received packet. */
            anncData[ZL303XX_PTP_ANNC_GM_PRIORITY1_OFST] = amtEntry->alternatePriority1;
         }

         /* Update the stream handle of this entry. */
         amtEntry->streamHandle = (streamData != NULL) ? streamData->clockStreamHandle
                                                       : (zl303xx_PtpStreamHandleT)ZL303XX_PTP_INVALID;
      }
      else
      {
         /* Increment the dropped packet counter. */
         portData->amtDroppedMsgCount[msg.header.messageType]++;

         /* Discarding received event message from node not in the acceptable
            master table  */
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "Discarding packet from PROTOCOL %lu, Addr Len %lu, Address %02X.%02X.%02X.%02X",
               ptpData->recvMsgInfo.srcAddr.networkProtocol,
               ptpData->recvMsgInfo.srcAddr.addressLength,
               ptpData->recvMsgInfo.srcAddr.addressField[0],
               ptpData->recvMsgInfo.srcAddr.addressField[1],
               ptpData->recvMsgInfo.srcAddr.addressField[2],
               ptpData->recvMsgInfo.srcAddr.addressField[3]);

         return;
      }
   }

   /* Determine if we have to filter packets of acceptable slaves only. */
   /* The logic here is that only DELAY-REQUESTS are unique from a client node.
    * An incoming ANNCOUNCE or SYNC could be from a potential MASTER and MGMT
    * messages should not be blocked. SIGNALING messages will be handled
    * elsewhere (with just REQUESTS being filtered / DENIED).
    * G.8275 GMs have class=248 so assuming <=127 as a Master is not valid.  */
   if ((portData->config.acceptableSlaveTableEnabled == ZL303XX_TRUE) &&
       (msg.header.messageType == ZL303XX_MSG_ID_DELAY_REQ))
   {
      zl303xx_PtpAccSlaveEntryS *astEntry;

      astEntry = zl303xx_PtpTableFind(&portData->accSlaveTbl,
                                      &ptpData->recvMsgInfo.srcAddr);
      if (astEntry != NULL)
      {
         /* Update the stream handle of this entry. */
         astEntry->streamHandle = (streamData != NULL) ? streamData->clockStreamHandle
                                                       : (zl303xx_PtpStreamHandleT)ZL303XX_PTP_INVALID;
      }
      else
      {
         /* Discarding received event message from node not in the acceptable
          * slave table. */
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "Discarding packet from PROTOCOL %lu, Addr Len %lu, Address %02X.%02X.%02X.%02X",
               ptpData->recvMsgInfo.srcAddr.networkProtocol,
               ptpData->recvMsgInfo.srcAddr.addressLength,
               ptpData->recvMsgInfo.srcAddr.addressField[0],
               ptpData->recvMsgInfo.srcAddr.addressField[1],
               ptpData->recvMsgInfo.srcAddr.addressField[2],
               ptpData->recvMsgInfo.srcAddr.addressField[3]);
         return;
      }
   }

   /* Unpack the data section of the received message. This function pointer is
    * checked by zl303xx_PtpV2CheckRecvPktParameters(). */
   (void)ptpV2PktProcess[msg.header.messageType].unpack(
            ptpData->msgBuf + offset, &msg.data);

   /* The rxHandlerFn pointer was verified previously but check it again here
    * in case the code was modified somehow.  */
   /* For ANNOUNCE, FOLLOWUP, DELAY_RESP the streamData must be present and has
    * already been checked previously (above). */
   if (ptpV2PktProcess[msg.header.messageType].rxHandlerFn != NULL)
   {
      processed = ptpV2PktProcess[msg.header.messageType].rxHandlerFn(
                     ptpData, &msg, streamData, portData);

      /* Update the processed message counter if all bytes in the PTP message
       * have been processed. */
      if ((streamData != NULL) && (processed == msg.header.messageLength))
      {
         streamData->counters.msg[msg.header.messageType].rxProcessed++;

         zl303xx_PtpV2McastIntvlDetect(streamData, msg.header.messageType,
                                       msg.header.logMessageInterval);
      }
   }

   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "Received Msg: No rxHandlerFn for PTP messageType=%u.",
            msg.header.messageType, 0,0,0,0,0);
   }

   /* Run the port and stream state machines */
   zl303xx_PtpV2Protocol(portData, streamData);
}

/*

  Function Name:
    zl303xx_PtpV2HandleTsRecordReady

  Details:
   Handles a Timestamp Record Ready event

  Parameters:
   [in]    msg            The received message

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpV2HandleTsRecordReady(
      zl303xx_ClockStreamS *streamData,
      zl303xx_PtpTsRecordTblEntryS *tsRecord)
{
   zl303xx_PtpClockS *pPtpClock = streamData->portData->clock;
   zl303xx_PtpTxStatusE txStatus = ZL303XX_PTP_TX_OK;

   /* Message contains a Delay Request/Response timestamp pair, so process the delay data */
   if (tsRecord->messageType == ZL303XX_MSG_ID_DELAY_REQ)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
            "   zl303xx_PtpV2HandleTsRecordReady - Have DReq/DRes TS pair.", 0,0,0,0,0,0);
      ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_ENGINE, 3)
      {
         ZL303XX_TRACE_ALWAYS("Stream %u: Delay Req msg tx " PTPD_FORMAT_TIME_STR,
               streamData->clockStreamHandle,
               formatTimeStringSign(&tsRecord->txTs),
               formatTimeStringSec(&tsRecord->txTs),
               formatTimeStringNs(&tsRecord->txTs), 0,0);
         ZL303XX_TRACE_ALWAYS("                        rx " PTPD_FORMAT_TIME_STR,
               formatTimeStringSign(&tsRecord->rxTs),
               formatTimeStringSec(&tsRecord->rxTs),
               formatTimeStringNs(&tsRecord->rxTs), 0,0,0);
      }

      zl303xx_PtpUpdateDelay(streamData, &tsRecord->txTs, &tsRecord->rxTs,
            &tsRecord->rxCorrection, tsRecord->seqId, &tsRecord->srcPortId);
   }

   else if (tsRecord->messageType == ZL303XX_MSG_ID_PEER_DELAY_REQ)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 2,
            "   zl303xx_PtpV2HandleTsRecordReady - Have Peer DReq/DRes TS pair for seqId=%d.", tsRecord->seqId, 0,0,0,0,0);

      zl303xx_PtpUpdatePeerDelay(streamData, &tsRecord->txTs, &tsRecord->rxTs,
                                   &tsRecord->rxCorrection, tsRecord->seqId);
   }

   /* Record contains T2/T3 Peer Delay data so issue a FollowUp. */
   else if (tsRecord->messageType == ZL303XX_MSG_ID_PEER_DELAY_RESP)
   {
      zl303xx_PtpV2MsgHeaderS origHdr;
      zl303xx_TimeStamp deltaTs = {{0,0},0};
      zl303xx_TimeStamp respTxTs = {{0,0},0};
      zl303xx_TimeInterval deltaInterval;

      /* Calculate the delta. */
      /************************/
      /* If the seconds portion of the RX time is 0 then assume only 30-bit NS are present. */
      if (tsRecord->rxTs.secondsField.lo == 0)
      {
         deltaTs.nanosecondsField = (((tsRecord->txTs.nanosecondsField + 1000000000) - tsRecord->rxTs.nanosecondsField) % 1000000000);
      }
      else
      {
         deltaTs.secondsField.lo = tsRecord->txTs.secondsField.lo - tsRecord->rxTs.secondsField.lo;
         deltaTs.nanosecondsField = tsRecord->txTs.nanosecondsField - tsRecord->rxTs.nanosecondsField;

         /* Check for -ve nanoseconds. */
         /* Since ns only goes to 1 billion, anything larger is actually negative. */
         while ((deltaTs.nanosecondsField >= 1000000000) &&
                (deltaTs.secondsField.lo > 0))
         {
            --deltaTs.secondsField.lo;
            deltaTs.nanosecondsField += 1000000000;
         }
      }

      /* Create a temporary Header with the original information. */
      OS_MEMSET(&origHdr, 0x00, sizeof(origHdr));           /* Clear correctionField, etc. */
      origHdr.sequenceId = tsRecord->seqId;              /* Add original sequenceId.    */
      origHdr.sourcePortIdentity = tsRecord->srcPortId;  /* Add original sourcePortIdentity.    */

      /* Resident Time method:
       * - Save the arrival time (T2) and rxHeader information (done above).
       * - Issue a PEER_DELAY_RESP message with correctionField &
       *   requestReceiptTimestamp both == 0
       * - Record PEER_DELAY_RESP launch time (T3).
       * - Issue a PDELAY_FOLLOWUP with:
       *     - responseOriginTimestamp == 0
       *     - correctionField += (T3 - T2) */
      if (streamData->portData->config.pdRespTxMethod == ZL303XX_PTP_PDELAY_METHOD_2_STEP_RESIDENT)
      {
         /* The correctionField is of TimeInterval type. */
         (void)zl303xx_PtpConvTimeStampToTimeInterval(&deltaTs, &deltaInterval);

         ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_TS_RECORD_MGR, 1)
         {
            ZL303XX_TRACE_ALWAYS("Stream %u: Delay Req msg tx " PTPD_FORMAT_TIME_STR,
                  streamData->clockStreamHandle,
                  formatTimeStringSign(&tsRecord->txTs),
                  formatTimeStringSec(&tsRecord->txTs),
                  formatTimeStringNs(&tsRecord->txTs), 0,0);
            ZL303XX_TRACE_ALWAYS("                        rx " PTPD_FORMAT_TIME_STR,
                  formatTimeStringSign(&tsRecord->rxTs),
                  formatTimeStringSec(&tsRecord->rxTs),
                  formatTimeStringNs(&tsRecord->rxTs), 0,0,0);
            ZL303XX_TRACE_ALWAYS(
                  "   zl303xx_PtpV2HandleTsRecordReady - Have PDReq/PDRes TS pair: Delta (%u:%u)(%08X:%08X)(%u:%u)",
                  deltaTs.secondsField.lo, deltaTs.nanosecondsField,
                  deltaInterval.scaledNanoseconds.hi, deltaInterval.scaledNanoseconds.lo,
                  tsRecord->rxCorrection.hi, tsRecord->rxCorrection.lo);
         }

         origHdr.correctionField = Add_U64S(deltaInterval.scaledNanoseconds, tsRecord->rxCorrection, NULL);

         ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 1,
               " origHdr.correctionField : (%u:%u)(%08X:%08X)   tsRecord->rxCorrection(%08X:%08X)",
               origHdr.correctionField.hi, origHdr.correctionField.lo,
               origHdr.correctionField.hi, origHdr.correctionField.lo,
               tsRecord->rxCorrection.hi, tsRecord->rxCorrection.lo);
      }

      /* Raw Timestamp method:
       * - Save the arrival time (T2) and rxHeader information (done above).
       * - Issue a PEER_DELAY_RESP message with correctionField == 0 and
       *   requestReceiptTimestamp both == T2
       * - Record PEER_DELAY_RESP launch time (T3).
       * - Issue a PDELAY_FOLLOWUP with:
       *     - responseOriginTimestamp == T3
       *     - correctionField = original correctionField -T2 fractional nSec +T3 fractional nSec */
      else if (streamData->portData->config.pdRespTxMethod == ZL303XX_PTP_PDELAY_METHOD_2_STEP_RAW)
      {
         /* correctionField 0'ed out above.  */
         origHdr.correctionField = tsRecord->rxCorrection;
         respTxTs = tsRecord->txTs;
      }

      /* Pack the Follow-Up message. */
      zl303xx_PtpV2MsgPDelayFollowUpPackForStream(streamData, &origHdr, &respTxTs, pPtpClock->msgObuf);


      txStatus = zl303xx_PtpTxMsg(pPtpClock->msgObuf,
            zl303xx_PtpV2MsgPktLengthGet(pPtpClock->msgObuf),
            ZL303XX_PTP_INTERFACE_GENERAL,
            streamData->portData,
            NULL,
            streamData,
            ZL303XX_MSG_ID_PEER_DELAY_FOLLOWUP);


      if (txStatus != ZL303XX_PTP_TX_OK)
      {
         zl303xx_PtpPortLogFaultyState(txStatus, ZL303XX_PTP_PORT_FAULT_FOLLOW_UP_SEND, streamData->portData);
         zl303xx_PtpV2ToPortState(streamData->portData, PTP_PORT_INT_STATE_FAULTY);
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                 "zl303xx_PtpV2HandleTsRecordReady: failed with txStatus = %lu.",
                 txStatus, 0,0,0,0,0);
           ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
                 "zl303xx_PtpV2HandleTsRecordReady: failed with txStatus = %lu.",
                 txStatus, 0,0,0,0,0);
         return;
      }

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4,
            "Stream %u: Sent PTP_PEER_FOLLOWUP_MESSAGE message  ",
            streamData->clockStreamHandle, 0,0,0,0,0);
   }

   /* Message contains SYNC message actual transmit timestamp, so generate a follow-up */
   else if (tsRecord->messageType == ZL303XX_MSG_ID_SYNC)
   {
      /* Create a temporary Header with the original Sync information. */
      zl303xx_PtpV2MsgHeaderS origHdr;
      OS_MEMSET(&origHdr, 0x00, sizeof(origHdr));    /* Clear correctionField, etc. */
      origHdr.sequenceId = tsRecord->seqId;       /* Add original sequenceId.    */

      /* Pack the Follow-Up message. */
      zl303xx_PtpV2MsgFollowUpPackForStream(streamData, &origHdr, &tsRecord->txTs, pPtpClock->msgObuf);

      txStatus = zl303xx_PtpTxMsg(pPtpClock->msgObuf, zl303xx_PtpV2MsgPktLengthGet(pPtpClock->msgObuf),
            ZL303XX_PTP_INTERFACE_GENERAL, streamData->portData,
            NULL, streamData, ZL303XX_MSG_ID_FOLLOWUP);

      if (txStatus != ZL303XX_PTP_TX_OK)
      {
         zl303xx_PtpPortLogFaultyState(txStatus, ZL303XX_PTP_PORT_FAULT_FOLLOW_UP_SEND, streamData->portData);
         zl303xx_PtpV2ToPortState(streamData->portData, PTP_PORT_INT_STATE_FAULTY);
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpV2HandleTsRecordReady: failed with txStatus = %lu.",
               txStatus, 0,0,0,0,0);
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
               "zl303xx_PtpV2HandleTsRecordReady: failed with txStatus = %lu.",
               txStatus, 0,0,0,0,0);
         return;
      }

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4,
            "Stream %u: Sent PTP_FOLLOWUP_MESSAGE message  ",
            streamData->clockStreamHandle, 0,0,0,0,0);
   }

}   /* End of zl303xx_PtpV2HandleTsRecordReady */


/*

  Function Name:
   zl303xx_PtpV2InitStreamData

  Details:
   Initializes the data structure for a specific stream on the PTP Clock

  Parameters:
   [in]    streamData    Pointer to the clock stream instance structure

  Return Value:
   Nothing

  Notes:
   Only initializes the data that must be reinitialized on a warm start.
   Cold-start data is initialized by zl303xx_PtpStreamCreate

*******************************************************************************/
void zl303xx_PtpV2InitStreamData(zl303xx_ClockStreamS *streamData)
{
   Uint32T count;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
         "zl303xx_PtpV2InitStreamData: Stream %u:",
         streamData->clockStreamHandle, 0,0,0,0,0);

   /* Clear the version independent variables */
   zl303xx_PtpStreamInitGenericCalcValues(streamData);

   for (count = 0; count < ZL303XX_ARRAY_SIZE(streamData->msgParams); count++)
   {
      streamData->msgParams[count].lastSequenceIdRxInit = ZL303XX_FALSE;
   }
}

/*

  Function Name:
   zl303xx_PtpV2IssueAnnounce

  Details:
   Issues an ANNOUNCE message for the specified clock stream

  Parameters:
   [in]    streamData    Pointer to the clock stream instance structure for which to
                  issue the ANNOUNCE message

  Return Value:
   Nothing

*******************************************************************************/
void zl303xx_PtpV2IssueAnnounce(zl303xx_ClockStreamS *streamData)
{
   /* Local status only */
   zl303xx_PtpTxStatusE txStatus = ZL303XX_PTP_TX_OK;

   /* Pack the ANNOUNCE data into the packet buffer */
   Uint16T pktLen = zl303xx_PtpV2MsgAnncPack(streamData,
                                     streamData->portData->clock->msgObuf,
                                     NULL);

   /* Append any custom TLVs to the end of the Announce Message. */
   pktLen += zl303xx_PtpProfileTlvAppend(streamData,
                                    ZL303XX_MSG_ID_ANNOUNCE,
                                    streamData->portData->clock->msgObuf,
                                    streamData->portData->clock->msgObuf + pktLen);

   /* Append the PATH_TRACE TLV (if enabled). */
   if (streamData->portData->clock->config.pathTrace.enabled == ZL303XX_TRUE)
   {
      /* Add the learned list... */
      Uint8T * pathTraceTlvStart = streamData->portData->clock->msgObuf + pktLen;
      Uint8T * pathTraceBufStart = streamData->portData->clock->msgObuf + pktLen + 2 + 2;
      Uint16T ptLen = PTP_V2_CLK_IDENTITY_LENGTH * streamData->portData->clock->pathTraceList.listLength;
      /* Add the PATH_TRACE TLV value. */
      PACK_16BIT((pathTraceTlvStart + 0), PTP_TLV_PATH_TRACE);

      /* Add the Final PATH_TRACE TLV added length (list entries + local clock id). */
      PACK_16BIT((pathTraceTlvStart + 2), PTP_V2_CLK_IDENTITY_LENGTH + ptLen);

      /* Add any learned Rx values */
      OS_MEMCPY(pathTraceBufStart,
             streamData->portData->clock->pathTraceList.pClockIdList,
             ptLen);

      /* Add the local node ClockIdentity */
      OS_MEMCPY(pathTraceBufStart + ptLen,
             streamData->portData->clock->config.defaultDS.clockIdentity,
             PTP_V2_CLK_IDENTITY_LENGTH);

      pktLen += (2 + 2 + ptLen + PTP_V2_CLK_IDENTITY_LENGTH);
   }

   /* Update the PTP message length in the header. */
   zl303xx_PtpV2MsgPktLengthSet(pktLen, streamData->portData->clock->msgObuf);

   txStatus = zl303xx_PtpTxMsg(streamData->portData->clock->msgObuf, pktLen,
                             ZL303XX_PTP_INTERFACE_GENERAL, streamData->portData,
                             NULL, streamData, ZL303XX_MSG_ID_ANNOUNCE);

   if (txStatus != ZL303XX_PTP_TX_OK)
   {
      zl303xx_PtpPortLogFaultyState(txStatus, ZL303XX_PTP_PORT_FAULT_ANNC_SEND, streamData->portData);
      zl303xx_PtpV2ToPortState(streamData->portData, PTP_PORT_INT_STATE_FAULTY);
   }
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
            "Stream %u event: send PTP_ANNOUNCE_MESSAGE Message  ",
            streamData->clockStreamHandle, 0,0,0,0,0);
   }
}

/*

  Function Name:
   zl303xx_PtpV2IssueSync

  Details:
   Issues a SYNC message for the specified clock stream

  Parameters:
   [in]    streamData    Pointer to the clock stream instance structure for which to
                  issue the SYNC message
   [in]    followUp       ZL303XX_TRUE if the SYNC message uses follow up timestamping

  Return Value:
   Nothing

*******************************************************************************/
void zl303xx_PtpV2IssueSync(zl303xx_ClockStreamS *streamData)
{
   /* Local status only */
   zl303xx_PtpTxStatusE txStatus = ZL303XX_PTP_TX_OK;
   zl303xx_BooleanE twoStepFlag = streamData->portData->clock->config.defaultDS.twoStepFlag;
   Uint16T pktLen = 0;
   Uint16T padLen = 0;

   pktLen = zl303xx_PtpV2MsgSyncPackForStream(streamData,
                                    NULL,    /* Get ToD from the hardware. */
                                    streamData->portData->clock->msgObuf);

   /* Update the PTP message length in the header if padding is required. */
   if ((padLen = zl303xx_PtpMsgAppendEventPadding(streamData, ZL303XX_MSG_ID_SYNC, pktLen)) > 0)
   {
      pktLen += padLen;
      zl303xx_PtpV2MsgPktLengthSet(pktLen, streamData->portData->clock->msgObuf);
   }

   txStatus = zl303xx_PtpTxMsg(streamData->portData->clock->msgObuf,
                             pktLen,
                             ZL303XX_PTP_INTERFACE_EVENT,
                             streamData->portData,
                             NULL,
                             streamData,
                             ZL303XX_MSG_ID_SYNC);

   if (txStatus == ZL303XX_PTP_TX_OK)
   {
      if (twoStepFlag == ZL303XX_TRUE)
      {
          zl303xx_PtpTsRecordReserve(streamData, ZL303XX_MSG_ID_SYNC,
                     zl303xx_PtpV2MsgSequenceNumGet(streamData->portData->clock->msgObuf));
      }
   }
   else
   {
      /* Go to faulty state */
      zl303xx_PtpPortLogFaultyState(txStatus ,ZL303XX_PTP_PORT_FAULT_SYNC_SEND, streamData->portData);
      zl303xx_PtpV2ToPortState(streamData->portData, PTP_PORT_INT_STATE_FAULTY);
   }
}

/**********************    DELAY-RESPONSE MECHANISM     ***********************/

/*

  Function Name:
   zl303xx_PtpV2IssueDelayReq

  Details:
   Issues a delay request message for the specified clock stream.

  Parameters:
   [in]  pPtpStream    Pointer to the clock stream instance structure for which to
                  issue the Delay Request message

  Return Value:
   Nothing

*******************************************************************************/
void zl303xx_PtpV2IssueDelayReq(zl303xx_ClockStreamS *pPtpStream)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpTxStatusE txStatus = ZL303XX_PTP_TX_OK;

   /* Local variables and references. */
   Uint16T pktLen = 0;
   Uint16T padLen = 0;

   /* Check any locally referenced input pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pPtpStream);
   }

   /* Only issue a delay requests if a SYNC has been received on this Stream. */
   if (status == ZL303XX_OK)
   {
      if (pPtpStream->firstSyncReceived == ZL303XX_FALSE)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpV2IssueDelayReq: Stream %u: First SYNC not received.",
               pPtpStream->clockStreamHandle, 0,0,0,0,0);
         status = ZL303XX_STREAM_NOT_IN_USE;
      }
   }

   /* Only issue a delay requests if the Time has been set on the local clock
    * (if required). */
   if (status == ZL303XX_OK)
   {
      if (pPtpStream->config.delayReqTimeSetRequired == ZL303XX_TRUE)
      {
         zl303xx_BooleanE timeStatus = ZL303XX_FALSE;

         if ((status = zl303xx_PtpClockTimeSetStatusGet(pPtpStream->portData->clock,
                                                        &timeStatus)) == ZL303XX_OK)
         {
            if (timeStatus == ZL303XX_FALSE)
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                     "zl303xx_PtpV2IssueDelayReq: Stream %u: Time Required but not set.",
                     pPtpStream->clockStreamHandle, 0,0,0,0,0);
               status = ZL303XX_INIT_ERROR;
            }
         }
      }
   }

   /* Only issue delay requests if the port is configured for a delay mechanism. */
   if ((status == ZL303XX_OK) &&
       (pPtpStream->portData->config.delayMechanism == ZL303XX_DELAY_MECHANISM_E2E))
   {
      pktLen = zl303xx_PtpV2MsgDelayReqPackForStream(pPtpStream, NULL, pPtpStream->portData->clock->msgObuf);

      /* Update the PTP message length in the header if padding is required. */
      if ((padLen = zl303xx_PtpMsgAppendEventPadding(pPtpStream, ZL303XX_MSG_ID_DELAY_REQ, pktLen)) > 0)
      {
         pktLen += padLen;
         zl303xx_PtpV2MsgPktLengthSet(pktLen, pPtpStream->portData->clock->msgObuf);
      }

      txStatus = zl303xx_PtpTxMsg(pPtpStream->portData->clock->msgObuf, pktLen,
                                ZL303XX_PTP_INTERFACE_EVENT, pPtpStream->portData,
                                NULL, pPtpStream, ZL303XX_MSG_ID_DELAY_REQ);

      if (txStatus == ZL303XX_PTP_TX_OK)
      {
         zl303xx_PtpTsRecordReserve(pPtpStream, ZL303XX_MSG_ID_DELAY_REQ,
               zl303xx_PtpV2MsgSequenceNumGet(pPtpStream->portData->clock->msgObuf));
      }
      else
      {
         /* Unable to send the Packet:: Go to faulty state */
         zl303xx_PtpPortLogFaultyState(txStatus, ZL303XX_PTP_PORT_FAULT_DELAY_REQ_SEND, pPtpStream->portData);
         zl303xx_PtpV2ToPortState(pPtpStream->portData, PTP_PORT_INT_STATE_FAULTY);
      }
   }
}

/*

  Function Name:
   zl303xx_PtpV2IssueDelayResp

  Details:
   Issues a delay response message

  Parameters:
   [in]    delayReqRecvTime     The receive time of the corresponding delay_req message
   [in]    header               The header of the corresponding delay_req message
   [in]    streamData          The clock stream instance structure

  Return Value:
   Nothing

*******************************************************************************/
void zl303xx_PtpV2IssueDelayResp(zl303xx_TimeStamp *delayReqRecvTime,
                                 zl303xx_PtpV2MsgHeaderS *header,
                                 zl303xx_ClockStreamS *streamData)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpTxStatusE txStatus = ZL303XX_PTP_TX_OK;
   const char *fnName = "zl303xx_PtpV2IssueDelayResp";

   /* Check any locally referenced input pointers */
   status = ZL303XX_CHECK_POINTER(header) |
         ZL303XX_CHECK_POINTER(streamData);

   if(status == ZL303XX_OK)
   {
      /* Only issue delay requests if the port is configured for a delay mechanism. */
      if (streamData->portData->config.delayMechanism != ZL303XX_DELAY_MECHANISM_DISABLED)
      {
         Uint16T ptpBufLen = 0;

         /* Pack the response TLV and keep the buffer size. Since the stream may
         be configured to send larger packets than the standard delay request
         size, check if padding is needed */
         ptpBufLen = zl303xx_PtpV2MsgDelayRespPackForStream(
               streamData, header, delayReqRecvTime,
               streamData->portData->clock->msgObuf);

         txStatus = zl303xx_PtpTxMsg(streamData->portData->clock->msgObuf, ptpBufLen,
               ZL303XX_PTP_INTERFACE_GENERAL, streamData->portData,
               NULL, streamData, ZL303XX_MSG_ID_DELAY_RESP);
      }

      if (txStatus != ZL303XX_PTP_TX_OK)
      {
         zl303xx_PtpPortLogFaultyState(txStatus, ZL303XX_PTP_PORT_FAULT_DELAY_RESP_SEND, streamData->portData);
         zl303xx_PtpV2ToPortState(streamData->portData, PTP_PORT_INT_STATE_FAULTY);
         return;
      }

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
            "Stream %u event: Send PTP_DELAY_RESP_MESSAGE Message",
            streamData->clockStreamHandle, 0,0,0,0,0);
   }
   else
   {
      ZL303XX_TRACE_ERROR("(ERROR) NULL pointer passed to %s.", fnName, 0, 0, 0, 0, 0);
   }
}

/**********************    PEER-DELAY MECHANISM     ***************************/

/*

  Function Name:
   zl303xx_PtpV2IssuePeerDelayReq

  Details:
   Issues a peer-delay request message for the specified clock stream.

  Parameters:
   [in]  pPtpStream     Pointer to the clock stream instance structure for
                              which to issue the Peer-Delay Request message.

  Return Value:
   Nothing

*******************************************************************************/
void zl303xx_PtpV2IssuePeerDelayReq(zl303xx_ClockStreamS *pPtpStream)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpTxStatusE txStatus = ZL303XX_PTP_TX_OK;

   /* Local variables and references. */
   Uint16T pktLen = 0;
   Uint16T padLen = 0;

   /* Check any locally referenced input pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pPtpStream);
   }

   /* Only issue peer-delay requests if the port is configured for the peer-delay mechanism. */
   if ((status == ZL303XX_OK) &&
       (pPtpStream->portData->config.delayMechanism != ZL303XX_DELAY_MECHANISM_P2P))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "zl303xx_PtpV2IssuePeerDelayReq: Stream(%u): Port(%u) not configured for P2P(%u)",
            pPtpStream->clockStreamHandle, pPtpStream->portData->clockPortHandle,
            pPtpStream->portData->config.delayMechanism, 0,0,0);
      status = ZL303XX_INVALID_OPERATION;
   }

   /* Attempt to send the Message */
   if (status == ZL303XX_OK)
   {
      pktLen = zl303xx_PtpV2MsgPDelayReqPackForStream(pPtpStream, NULL, pPtpStream->portData->clock->msgObuf);

      /* Update the PTP message length in the header if padding is required. */
      if ((padLen = zl303xx_PtpMsgAppendEventPadding(pPtpStream, ZL303XX_MSG_ID_PEER_DELAY_REQ, pktLen)) > 0)
      {
         pktLen += padLen;
         zl303xx_PtpV2MsgPktLengthSet(pktLen, pPtpStream->portData->clock->msgObuf);
      }

      txStatus = zl303xx_PtpTxMsg(pPtpStream->portData->clock->msgObuf, pktLen,
                                ZL303XX_PTP_INTERFACE_EVENT, pPtpStream->portData,
                                NULL, pPtpStream, ZL303XX_MSG_ID_PEER_DELAY_REQ);

      if (txStatus == ZL303XX_PTP_TX_OK)
      {
         zl303xx_PtpTsRecordReserve(pPtpStream, ZL303XX_MSG_ID_PEER_DELAY_REQ,
               zl303xx_PtpV2MsgSequenceNumGet(pPtpStream->portData->clock->msgObuf));
      }
      else
      {
         /* Go to faulty state */
         zl303xx_PtpPortLogFaultyState(txStatus, ZL303XX_PTP_PORT_FAULT_PDELAY_REQ_SEND, pPtpStream->portData);
         zl303xx_PtpV2ToPortState(pPtpStream->portData, PTP_PORT_INT_STATE_FAULTY);
         status = ZL303XX_PROTOCOL_ENGINE_ERROR;
      }
   }

   /* Log any error. */
   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "Stream %u: Error with PEER_DELAY_REQ event, status=%d",
            pPtpStream->clockStreamHandle, status, 0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %u: Error with PEER_DELAY_REQ event, status=%d",
            pPtpStream->clockStreamHandle, status, 0,0,0,0);
   }
}

/*

  Function Name:
   zl303xx_PtpV2IssuePeerDelayResp

  Details:
   Issues a peer delay response message from a received peer delay request.

  Parameters:
   [in]  streamData           The clock stream instance structure
   [in]  rxHeader             The received header of the corresponding p
                                    delay_req message
   [in]  pdelayReqRecvTime    The receive time (T2) of the corresponding
                                    pdelay_req message

  Return Value:
   Nothing

*******************************************************************************/
void zl303xx_PtpV2IssuePeerDelayResp(zl303xx_ClockStreamS *streamData,
                                     zl303xx_PtpV2MsgHeaderS *rxHeader,
                                     zl303xx_TimeStamp *pdelayReqRecvTime)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpTxStatusE txStatus = ZL303XX_PTP_TX_OK;

   /* Check any locally referenced input pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(streamData) |
               ZL303XX_CHECK_POINTER(rxHeader) |
               ZL303XX_CHECK_POINTER(pdelayReqRecvTime);
   }

   /* Issue peer delay responses regardless of the port's delay mechanism. */
   if (status == ZL303XX_OK)
   {
      Uint16T ptpBufLen = 0;
      Uint16T padLen = 0;

      /* If this is any of the 1-step Peer-Delay Resp Methods, just send.
       * Put the T2 value in the requestReceiptTimestamp field for now. The hardware
       * will need to subtract it from the generated T3 value. */
      if (streamData->portData->config.pdRespTxMethod & ZL303XX_PTP_PDELAY_METHOD_1_STEP_FLAG)
      {
         /* If DROP is set, then we received this packet just to update stats.
          * The RESP is turned-around at the hardware level.   */
         if (streamData->portData->config.pdRespTxMethod == ZL303XX_PTP_PDELAY_METHOD_1_STEP_DROP)
         {
            return;
         }
         else if (streamData->portData->config.pdRespTxMethod == ZL303XX_PTP_PDELAY_METHOD_1_STEP_CORR)
         {
            /* The transmitted correctionField should be = rxCorr + (T2-T2) so
             * subtract the T2 of the received REQ (in scaled-ns) from the
             * received correctionField. T3 will be added by the hardware. */
            zl303xx_TimeInterval t2Rx;
            (void)zl303xx_PtpConvTimeStampToTimeInterval(pdelayReqRecvTime, &t2Rx);

            rxHeader->correctionField = Diff_U64S(rxHeader->correctionField, t2Rx.scaledNanoseconds, NULL);
            OS_MEMSET(pdelayReqRecvTime, 0x00, sizeof(zl303xx_TimeStamp));

            /* Packet is issued below. */
         }
      }

      /* If any 2-step: Method(7) is used, then store T2 for now and wait for T3 to
       * be generated.   */
      else /* if (streamData->portData->config.pdRespTxMethod == ZL303XX_PTP_PDELAY_METHOD_2_STEP_RESIDENT)*/
      {
         /* Reserve a Peer-Delay Response Record entry until T3 is ready. */
         zl303xx_PtpTsRecordReserve(streamData, ZL303XX_MSG_ID_PEER_DELAY_RESP, rxHeader->sequenceId);

         /* Add the received Timestamp and other rxHeader data to the reserved record. */
         zl303xx_PtpTsRecordRegisterIngress(
                     streamData, &rxHeader->sourcePortIdentity,
                     ZL303XX_MSG_ID_PEER_DELAY_RESP,
                     rxHeader->sequenceId,
                     pdelayReqRecvTime,
                     &rxHeader->correctionField);

         /* There are 2 possible methods of implementing the 2-step PD_RESP.
          * Both cases 0-out the correctionField. */
         OS_MEMSET(&rxHeader->correctionField, 0x00, sizeof(rxHeader->correctionField));

         /* Resident Time method:
          * - Save the arrival time (T2) and rxHeader information (done above).
          * - Issue a PEER_DELAY_RESP message with correctionField &
          *   requestReceiptTimestamp both == 0
          * - Record PEER_DELAY_RESP launch time (T3).
          * - Issue a PDELAY_FOLLOWUP with:
          *     - responseOriginTimestamp == 0
          *     - correctionField += (T3 - T2) */
         if (streamData->portData->config.pdRespTxMethod == ZL303XX_PTP_PDELAY_METHOD_2_STEP_RESIDENT)
         {
            /* Clear the launched value of the timestamp & correctionField.
             * (Both values have already been recorded above).*/
            OS_MEMSET(pdelayReqRecvTime, 0x00, sizeof(zl303xx_TimeStamp));
         }

         /* Raw Timestamp method:
          * - Save the arrival time (T2) and rxHeader information (done above).
          * - Issue a PEER_DELAY_RESP message with correctionField == 0 and
          *   requestReceiptTimestamp both == T2
          * - Record PEER_DELAY_RESP launch time (T3).
          * - Issue a PDELAY_FOLLOWUP with:
          *     - responseOriginTimestamp == T3
          *     - correctionField = original correctionField -T2 fractional nSec +T3 fractional nSec */
         else if (streamData->portData->config.pdRespTxMethod == ZL303XX_PTP_PDELAY_METHOD_2_STEP_RAW)
         {
            /* correctionField 0'ed out above.  */
            /* pdelayReqRecvTime already == T2. */
         }
      }

      /* Pack the PEER_DELAY_RESP and send it. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 2,
            "Stream %u: Build PEER_DELAY_RESP event - seqId(%u):(%u:%u)",
            streamData->clockStreamHandle, rxHeader->sequenceId,
            pdelayReqRecvTime->secondsField.lo, pdelayReqRecvTime->nanosecondsField, 0,0);
      ptpBufLen = zl303xx_PtpV2MsgPDelayRespPackForStream(
                                 streamData, rxHeader, pdelayReqRecvTime,
                                 streamData->portData->clock->msgObuf);

      /* Update the PTP message length in the header if padding is required. */
      if ((padLen = zl303xx_PtpMsgAppendEventPadding(streamData, ZL303XX_MSG_ID_PEER_DELAY_REQ, ptpBufLen)) > 0)
      {
         ptpBufLen += padLen;
         zl303xx_PtpV2MsgPktLengthSet(ptpBufLen, streamData->portData->clock->msgObuf);
      }

      txStatus = zl303xx_PtpTxMsg(streamData->portData->clock->msgObuf, ptpBufLen,
                                 ZL303XX_PTP_INTERFACE_EVENT, streamData->portData,
                                 NULL, streamData, ZL303XX_MSG_ID_PEER_DELAY_RESP);

      if (txStatus != ZL303XX_PTP_TX_OK)
      {
         zl303xx_PtpPortLogFaultyState(txStatus, ZL303XX_PTP_PORT_FAULT_PDELAY_RESP_SEND, streamData->portData);
         zl303xx_PtpV2ToPortState(streamData->portData, PTP_PORT_INT_STATE_FAULTY);
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                     "Stream %u: Error with PEER_DELAY_RESP event - seqId(%u), txStatus=%d",
                     streamData->clockStreamHandle, rxHeader->sequenceId, txStatus, 0,0,0);
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
                     "Stream %u: Error with PEER_DELAY_RESP event - seqId(%u), txStatus=%d",
                     streamData->clockStreamHandle, rxHeader->sequenceId, txStatus, 0,0,0);
      }
      else
      {
         /* Packet has been issued. */
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
               "Stream %u: Issued PEER_DELAY_RESP event - seqId(%u)",
               streamData->clockStreamHandle, rxHeader->sequenceId, 0,0,0,0);
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 2,
               "Stream %u: Issued PEER_DELAY_RESP event - seqId(%u)",
               streamData->clockStreamHandle, rxHeader->sequenceId, 0,0,0,0);
      }
   }
}


/****************   STATIC PACKET CHECKING ROUTINE  **************************/

/*

  Function Name:
    zl303xx_PtpV2CheckRecvPktParameters

  Details:
   Check the following items related to the received packet (PTP message):
   - PTP Version: must be compatible with the portData (expect v2).
   - domain: must be compatible with the clock configuration.
   - messageType:
      - range: must be between 0 (SYNC) & 13 (Management).
      - implemented: Peer-Delay message are not implemented. there are also
            some reserved values that should be ignored.
   - sourcePortIdentity: ignore packets that somehow originated from the
         local port.
   - malformed packet:
      - packet check: verifies the length set in the packet header field with
            the minimum required for the message type
      - task message queue check: verifies the length set in the task message
            queue bufLen member with the packet header field to make sure the
            packet wasn't truncated
   - alternate master configuration: check for a mismatch in the local and far
         end ALTERNATE_MASTER configuration

   Some things that are NOT checked:
   - UDP Port: the UDP port and PTP message types are not cross-referenced. In
         general, PTP event messages use UDP 319 and general messages use 320.
         However, this implementation assumes that the UDP configuration is
         handled at the transport layer and thus the PTP flags only on the
         message type, regardless of the actual UDP port.

  Parameters:
   [in]    ptpMsg       Pointer to the PTP event message queue structure.
                              Contains the type info, the message buffer (in
                              this case, the same as the packet buffer), etc.
   [in]    pktHdr       The PTP header structure of the received message.
   [in]    portData     Pointer to the port data structure. Cannot be NULL.

  Return Value:
   zlStatusE

 *****************************************************************************/
zlStatusE zl303xx_PtpV2CheckRecvPktParameters(zl303xx_PtpClockMsgQueueS *ptpMsg,
                                              zl303xx_PtpV2MsgHeaderS *pktHdr,
                                              zl303xx_PortDataS *portData)
{
   zlStatusE status = ZL303XX_OK;

   const char *fnName = "zl303xx_PtpV2CheckRecvPktParameters";

   /* Determine the sourcePortIdentity as a string if need for logging. */
   static char srcPortIdStr[64];
   (void)zl303xx_PtpV2PortIdentityToString(&pktHdr->sourcePortIdentity, srcPortIdStr);

   /* Skip messages that are not version compatible */
   if (pktHdr->versionPTP != portData->portDS.versionNumber)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "%s: Stream %d: Src(%s):MsgType(%u):Version : v%d received but configured for v%d",
            fnName, (Sint32T)ptpMsg->clockStreamId, srcPortIdStr,
            pktHdr->messageType, pktHdr->versionPTP,
            portData->portDS.versionNumber);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "%s: Stream %d: Src(%s):MsgType(%u) Version : v%d received but configured for v%d",
            fnName, (Sint32T)ptpMsg->clockStreamId, srcPortIdStr,
            pktHdr->messageType, pktHdr->versionPTP,
            portData->portDS.versionNumber);

      status = ZL303XX_PARAMETER_INVALID;
   }

   /* Skip messages that are not for the current domain */
   else if (pktHdr->domainNumber != portData->clock->config.defaultDS.domainNumber)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "%s: Stream %d: Src(%s):MsgType(%u) Domain : %d received but configured for %d",
            fnName, (Sint32T)ptpMsg->clockStreamId, srcPortIdStr,
            pktHdr->messageType, pktHdr->domainNumber,
            portData->clock->config.defaultDS.domainNumber);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "%s: Stream %d: Src(%s):MsgType(%u) Domain : %d received but configured for %d",
            fnName, (Sint32T)ptpMsg->clockStreamId, srcPortIdStr,
            pktHdr->messageType, pktHdr->domainNumber,
            portData->clock->config.defaultDS.domainNumber);

      status = ZL303XX_PARAMETER_INVALID;
   }

   /* Skip messages of types that are RESERVED or outside the range. */
   else if ((pktHdr->messageType >= ZL303XX_PTP_V2_NUM_MSG_IDS) ||
            (ptpV2PktProcess[pktHdr->messageType].minLength == 0))
   {
      /* This is an unknown message type. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "%s: Stream %d: Src(%s):MsgType    : %d Unknown or Invalid",
            fnName,
            (Sint32T)ptpMsg->clockStreamId, srcPortIdStr, pktHdr->messageType, 0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "%s: Stream %d: Src(%s):MsgType    : %d Unknown or Invalid",
            fnName,
            (Sint32T)ptpMsg->clockStreamId, srcPortIdStr, pktHdr->messageType, 0,0);

      status = ZL303XX_PARAMETER_INVALID;
   }

   /* Refer to IEEE-1588: 9.5.2 - Ignore self-originated messages of a Clock */
   /* 9.5.2.1 implies messages from the same Clock must be dropped.
    * 9.5.2.2 implies messages from the same Port must be dropped.
    * 9.5.2.3 states that a port receiving ANNC from another port on the same
    *    clock must go to PASSIVE Mode (see zl303xx_PtpStreamStateToM2) */
   else if (zl303xx_PtpV2EqualClockIdentities(pktHdr->sourcePortIdentity.clockIdentity,
                           portData->portDS.portIdentity.clockIdentity) == ZL303XX_TRUE)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "%s: Stream %d: Port %u : Ignoring message from self",
            fnName,
            (Sint32T)ptpMsg->clockStreamId, portData->config.portNumber, 0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "%s: Stream %d: Port %u : Ignoring message from self",
            fnName,
            (Sint32T)ptpMsg->clockStreamId, portData->config.portNumber, 0,0,0);

      status = ZL303XX_ERROR;
   }

   /* Check for malformed packets... */
   /* 1) check the buffer is as large as the PTP Header thinks it is... */
   /* 2) check that the PTP length field is at least as long as the minimum for
    *    respective messageType (TLVs may make some longer but never shorter. */
   else if ((ptpMsg->pktLength < pktHdr->messageLength) ||
            (pktHdr->messageLength < ptpV2PktProcess[pktHdr->messageType].minLength))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "%s: Stream %d: Protocol mis-configuration or TS not appended to pkt!",
            fnName,
			(Sint32T)ptpMsg->clockStreamId,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "%s: Stream %d: Protocol mis-configuration or TS not appended to pkt!",
            fnName,
			(Sint32T)ptpMsg->clockStreamId,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "Stream %d: msgId %d (%s), embeddedMsgLen %d, bufferLen=%d, expected=%d",
			(Sint32T)ptpMsg->clockStreamId,
            pktHdr->messageType,
            (ptpV2PktProcess[pktHdr->messageType].desc),
            pktHdr->messageLength,
            ptpMsg->pktLength,
            ptpV2PktProcess[pktHdr->messageType].minLength);

      /* Increment the malformed packet count if possible */
      zl303xx_IncrementMalformedPktCount(portData);

      status = ZL303XX_DATA_CORRUPTION;
   }

   /* Do Profile specific checks on the transportSpecific field. */
   else if ((status = zl303xx_PtpProfileTransportSpecificCheck(
                                 portData->clock->config.profile,
                                 portData->clock->config.profileStrict,
                                 &pktHdr->transportSpecific)) != ZL303XX_OK)
   {
      const char * profileDescrStr = zl303xx_PtpProfileIdToStr(portData->clock->config.profile);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "%s: Stream %d: transportSpecific(0x%X) invalid for profile (%d)(%s): %s; dropped",
            fnName,
			(Sint32T)ptpMsg->clockStreamId, pktHdr->transportSpecific,
            portData->clock->config.profile, profileDescrStr,
            (ptpV2PktProcess[pktHdr->messageType].desc));
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "%s: Stream %d: transportSpecific(0x%X) invalid for profile (%d)(%s): %s; dropped",
            fnName,
			(Sint32T)ptpMsg->clockStreamId,
			pktHdr->transportSpecific,
            portData->clock->config.profile, profileDescrStr,
            (ptpV2PktProcess[pktHdr->messageType].desc));

      zl303xx_IncrementMalformedPktCount(portData);
   }

   /* Check for a mismatch in the local and far end ALTERNATE_MASTER configuration. */
   else if (getFlag(pktHdr->flagField, PTP2_FLAGS_ALTERNATE_MASTER) == ZL303XX_TRUE)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
            "%s: Stream %d: ALTERNATE_MASTER : disqualifying message type %d:%s",
            fnName,
            (Sint32T)ptpMsg->clockStreamId, pktHdr->messageType,
            (ptpV2PktProcess[pktHdr->messageType].desc), 0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 2,
            "%s: Stream %d: ALTERNATE_MASTER : disqualifying message type %d:%s",
            fnName,
            (Sint32T)ptpMsg->clockStreamId, pktHdr->messageType,
            (ptpV2PktProcess[pktHdr->messageType].desc), 0,0);
   }

   return status;
}  /* END zl303xx_PtpV2CheckRecvPktParameters */

/* zl303xx_PtpV2CheckRecvStream */
/** 

   Verifies the input STREAM pointer.
   In order to call this LOCAL routine,
      1) the MSG is verified by the TASK and passed to V2Protocol which calls
         this routine (so MSG can't be NULL).
      2) the PORT parameter is established at the TASK level (so can't be NULL).

   In some cases, it is OK for the streamData to be NULL (MGMT/Signaling)
   but for many it is not, so check it here (once) to avoid having to do it
   in all of the individual packet (message) handler routines.

  Parameters:
   [in]    streamData   Pointer to the clock stream instance structure (may
                              be NULL if there is no clock stream associated
                              with this received message, ex: CONTROL).
   [in]    portData     Pointer to the port data structure. Cannot be NULL.

*******************************************************************************/
static zlStatusE zl303xx_PtpV2CheckRecvStream(zl303xx_MessageTypeE msgType,
                                              zl303xx_PortDataS *portData,
                                              zl303xx_ClockStreamS *streamData)
{
   const char *fnName = "zl303xx_PtpV2CheckRecvStream";

   if (ptpV2PktProcess[msgType].streamRequired == ZL303XX_TRUE)
   {
      if (streamData == NULL)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
               "%s: Stream  : NULL stream for message type %d:%s",
               fnName, msgType, ptpV2PktProcess[msgType].desc, 0,0,0);
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
               "%s: Stream  : NULL stream for message type %d:%s",
               fnName, msgType, ptpV2PktProcess[msgType].desc, 0,0,0);

         return ZL303XX_INVALID_POINTER;
      }

      /* The following check is not an error but may indicate a configuration
       * problem somewhere. Produce a log to indicate the condition. */
      else if (portData != streamData->portData)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "%s: Packet received on a port other that the one configured for it...portData=%p != streamData->portData=%p",
               fnName, portData, streamData->portData, 0,0,0);
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
               "%s: Packet received on a port other that the one configured for it...portData=%p != streamData->portData=%p",
               fnName, portData, streamData->portData, 0,0,0);
      }
   }

   return ZL303XX_OK;
}

/****************   STATIC PACKET HANDLING ROUTINES **************************/

/*

  Function Name:
    zl303xx_PtpV2handleSync

  Details:
   Handle a received PTP v2 SYNC message

  Parameters:
   [in]    ptpData      Pointer to the PTP event message queue structure.
                              Contains the type info, the message buffer (often
                              the same as the packet buffer), etc.
   [in]    msg          The received message unpacked into a structure.
   [in]    streamData   Pointer to the clock stream instance structure (may
                              be NULL if there is no clock stream associated
                              with this received message, ex: CONTROL).
   [in]    portData     Pointer to the port data structure. Cannot be NULL.

  Return Value:
   The number of message bytes handled by this function on SUCCESS.
   0 otherwise.

 *****************************************************************************/
Uint16T zl303xx_PtpV2handleSync(zl303xx_PtpClockMsgQueueS *ptpData,
                                zl303xx_PtpV2MsgS *msg,
                                zl303xx_ClockStreamS *streamData,
                                zl303xx_PortDataS *portData)
{
   /* Notes:
    * - the streamData pointer was checked by the parent, 'V2handleRecvMsg'
    * - all other pointers are generated locally or verified at the TASK,
    *   PROTOCOL or HANDLER levels.
    * - the message & packet lengths were verified by 'V2handleRecvMsg'
    * - the alternate master configuration was verified by 'V2handleRecvMsg'
    */

   zl303xx_PtpClockS *pPtpClock = portData->clock;
   zl303xx_BooleanE bTwoStepSync = getFlag(msg->header.flagField, PTP2_FLAGS_TWO_STEP);

   (void)exampleSetTwoStepFlag(bTwoStepSync);

   /* The T1 value inside the SYNC message may not be valid in the case of a
    * 2-step clock. Show all necessary info in the log. */
   {
      static char t1[TIMESTAMP_STR_LEN];
      static char t2[TIMESTAMP_STR_LEN];

      /* Convert both Timestamps to strings. */
      zl303xx_PtpV2TimestampToString(&msg->data.sync.originTimestamp, t1);
      zl303xx_PtpV2TimestampToString(&ptpData->recvMsgInfo.rxTs, t2);

      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %u: Received SYNC event - seqId[%u]: T2(%s), T1(%s)(%s-STEP)",
            streamData->clockStreamHandle, msg->header.sequenceId,
            t2, t1,
            ((bTwoStepSync == ZL303XX_TRUE) ? "2" : "1"), 0);
   }

   /* Ensure we do not issue DELAY_REQUEST unless this SYNC is accepted. */
   streamData->firstSyncReceived = ZL303XX_FALSE;

   /* Multicast stream: accept the Sync message based on the sourcePortIdentity
    * in the header.*/
   /* Multicast & unicast non-negotiated stream: accept the Sync message based
    * on conditions of IEEE-1588-2008: Figure 30:
    * - the sourcePortIdentity in the header must match the local ParentDS.
    * - the current state of the stream must be SLAVE or UNCAL. */
   if ((streamData->config.unicast == ZL303XX_FALSE) ||
       (portData->config.unicastNegotiation.enabled == ZL303XX_FALSE))
   {
      /* If this stream is not being monitored, discard SYNC messages from ports
       * other than the parent. If the node is not SYNCed to another server, the
       * parent data is the local default values and the incoming SYNCs are
       * still dropped.*/
      if ((streamData->config.unicastMonitorTiming == ZL303XX_FALSE) &&
          (zl303xx_PtpV2EqualPortIdentities(&msg->header.sourcePortIdentity,
                        &pPtpClock->parentDS.parentPortIdentity) == ZL303XX_FALSE))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
               "Stream %u: Ignored Sync (not from current master).",
               streamData->clockStreamHandle, 0,0,0,0,0);
         return 0;
      }

      /* Unicast non-negotiated stream: accept the Sync message based on the
       * current state: 
       * - if UNCALIBRATED or SLAVE, accept the packet
       * - if PASSIVE the monitor Timing must be enabled. */
      else if ((streamData->streamSubstate != PTP_STREAM_SUBSTATE_UNCALIBRATED) &&
               (streamData->streamSubstate != PTP_STREAM_SUBSTATE_SLAVE))
      {
         if  ((streamData->streamSubstate != PTP_STREAM_SUBSTATE_PASSIVE) ||
              (streamData->config.unicastMonitorTiming == ZL303XX_FALSE))
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
                  "Stream %lu: Ignored Sync (stream not UNCALIBRATED or SLAVE)=%d.",
                  streamData->clockStreamHandle, streamData->streamSubstate, 0,0,0,0);
            return 0;
         }
      }
   }

   /* Unicast negotiated stream: accept the Sync message if we have an active
    * contract to receive. */
   else if (portData->config.unicastNegotiation.enabled)
   {
      if (streamData->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].remainingSec == 0)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
               "Stream %u: Ignored unicastNeg Sync (no contract).",
               streamData->clockStreamHandle, 0,0,0,0,0);
         return 0;
      }
   }

   /* We should never receive a 1-step SYNC packet with a TS of 0. This may
    * indicate a configuration error of the transmit end.   */
   if ((bTwoStepSync == ZL303XX_FALSE) &&
       (isZeroTime(&msg->data.sync.originTimestamp) == ZL303XX_TRUE))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %u: Ignored 1-Step SYNC with T1 timestamp of (0).",
            0,0,0,0,0,0);
      return 0;
   }

   /* Packet has been accepted. */
   /* Set a flag so Delay Request may be set on this stream (if applicable). */
   streamData->firstSyncReceived = ZL303XX_TRUE;

   /* Add any local delay asymmetry value to the correctionField of the
    * received event message. */
   {
      zl303xx_TimeInterval rxCorr;
      rxCorr.scaledNanoseconds = msg->header.correctionField;
      msg->header.correctionField = zl303xx_PtpTimeIntervalAdd(
                                          streamData->config.delayAsymmetry.ingress,
                                          rxCorr).scaledNanoseconds;
   }

   if ((msg->header.logMessageInterval != PTP_V2_LMMI_NULL_VALUE) &&
       (msg->header.logMessageInterval != streamData->config.logSyncInterval))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 2,
            "Stream %u: message's SYNC interval is %d, but clock's is %d  ",
            streamData->clockStreamHandle,
            msg->header.logMessageInterval,
            streamData->config.logSyncInterval, 0,0,0);

      /* Spec. recommends a SYNC interval discrepancy should be a fault.
         However, this implementation can tolerate asymmetrical SYNC
         rates without problem. */
      /* (So, the 'else' is removed below). */
   }

   if (bTwoStepSync == ZL303XX_FALSE)
   {
      /* There is no follow-up so we have all the information we
         need to process the timestamps */

      /* G.8265.1 and G.8275.2 only:
       * Disable PTSF-lossFollowUp and PTSF-lossSyncOrFollowUp timeouts. These
       * timeouts will get enabled on the next received GRANT TLV for Sync
       * messages (see zl303xx_PtpTelecomMsgTimeoutCheck). */
      if (((pPtpClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1) ||
           (pPtpClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_2)) &&
          (streamData->pUmtEntry != NULL))
      {
         streamData->pUmtEntry->telecom.followReceiptTimeoutSec = 0;
         streamData->pUmtEntry->telecom.sofReceiptTimeoutSec = 0;
      }

      zl303xx_PtpUpdateOffset(streamData, &msg->data.sync.originTimestamp,
                              &ptpData->recvMsgInfo.rxTs,
                              &msg->header.correctionField,
                              msg->header.sequenceId, &msg->header.sourcePortIdentity);

   }
   else
   {
      zl303xx_PtpStreamTwoStepRegisterRx(streamData, msg->header.sequenceId,
                                         &ptpData->recvMsgInfo.rxTs,
                                         &msg->header.correctionField,
                                         &msg->header.sourcePortIdentity);
   }

   /* Store the latest receipt time for the drift calculation */
   streamData->drift.latestSyncRxTime = ptpData->recvMsgInfo.rxTs;

   /* Handle any appending TLVs (Only padding supported) */
   if ( msg->header.messageLength > ZL303XX_PTP_V2_SYNC_MSG_LEN)
   {
      (void)zl303xx_PtpTlvEventMsgdHandle(streamData, msg, (ptpData->msgBuf + ZL303XX_PTP_V2_SYNC_MSG_LEN));
   }

   return msg->header.messageLength;
}

/*

  Function Name:
    zl303xx_PtpV2handleDelayReq

  Details:
   Handle a received PTP v2 delay request message

  Parameters:
   [in]    ptpData      Pointer to the PTP event message queue structure.
                              Contains the type info, the message buffer (often
                              the same as the packet buffer), etc.
   [in]    msg          The received message unpacked into a structure.
   [in]    streamData   Pointer to the clock stream instance structure (may
                              be NULL if there is no clock stream associated
                              with this received message, ex: CONTROL).
   [in]    portData     Pointer to the port data structure. Cannot be NULL.


  Return Value:
   The number of message bytes handled by this function on SUCCESS.
   0 otherwise.

 *****************************************************************************/
Uint16T zl303xx_PtpV2handleDelayReq(zl303xx_PtpClockMsgQueueS *ptpData,
                                    zl303xx_PtpV2MsgS *msg,
                                    zl303xx_ClockStreamS *streamData,
                                    zl303xx_PortDataS *portData)
{
   /* Notes:
    * - streamData pointer may be NULL in the case of a mixed mode master.
    * - all other pointers are generated locally or verified at the TASK,
    *   PROTOCOL or HANDLER levels.
    * - the message & packet lengths were verified by 'V2handleRecvMsg'
    * - the alternate master configuration was verified by 'V2handleRecvMsg'
    */

   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
         "Stream %u: Received DELAY_REQ event - seqId[%u]",
         streamData->clockStreamHandle,
         msg->header.sequenceId, 0,0,0,0);

   /* Multicast & unicast non-negotiated stream: accept the Delay_Req message
    * based on the current state (must be MASTER). */
   if ((streamData->config.unicast == ZL303XX_FALSE) ||
       (portData->config.unicastNegotiation.enabled == ZL303XX_FALSE))
   {
      if (streamData->streamSubstate != PTP_STREAM_SUBSTATE_MASTER)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %u: Ignored Delay_Req (stream not in MASTER state).",
            streamData->clockStreamHandle, 0,0,0,0,0);
         return 0;
      }
   }
   else
   {
      /* Unicast negotiated stream: accept the Delay_Req message if we have an
       * active contract to transmit Delay_Resp. */
      if (streamData->contractTx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].remainingSec == 0)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %lu: Ignored unicast Delay_Req (no contract).",
            streamData->clockStreamHandle, 0,0,0,0,0);
         return 0;
      }
   }

   /* Packet has been accepted - Send a Response. */
   zl303xx_PtpV2IssueDelayResp(&ptpData->recvMsgInfo.rxTs, &msg->header, streamData);


   /* Handle any appending TLVs (Only padding supported) */
   if ( msg->header.messageLength > ZL303XX_PTP_V2_DREQ_MSG_LEN)
   {
      (void)zl303xx_PtpTlvEventMsgdHandle(streamData, msg, (ptpData->msgBuf + ZL303XX_PTP_V2_DREQ_MSG_LEN));
   }

   return msg->header.messageLength;
}  /* END zl303xx_PtpV2handleDelayReq */

/*

  Function Name:
    zl303xx_PtpV2handlePeerDelayReq

  Details:
   Handle a received PTP v2 peer-delay request message

  Parameters:
   [in]    ptpData      Pointer to the PTP event message queue structure.
                              Contains the type info, the message buffer (often
                              the same as the packet buffer), etc.
   [in]    msg          The received message unpacked into a structure.
   [in]    streamData   Pointer to the clock stream instance structure (may
                              be NULL if there is no clock stream associated
                              with this received message, ex: CONTROL).
   [in]    portData     Pointer to the port data structure. Cannot be NULL.


  Return Value:
   The number of message bytes handled by this function on SUCCESS.
   0 otherwise.

 *****************************************************************************/
Uint16T zl303xx_PtpV2handlePeerDelayReq(zl303xx_PtpClockMsgQueueS *ptpData,
                                        zl303xx_PtpV2MsgS *msg,
                                        zl303xx_ClockStreamS *streamData,
                                        zl303xx_PortDataS *portData)
{
   /* Notes:
    * - streamData pointer may be NULL in the case of a mixed mode master.
    * - all other pointers are generated locally or verified at the TASK,
    *   PROTOCOL or HANDLER levels.
    * - the message & packet lengths were verified by 'V2handleRecvMsg'
    * - the alternate master configuration was verified by 'V2handleRecvMsg'
    */

   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
         "Stream %u: Received PEER_DELAY_REQ event - seqId[%u]",
         streamData->clockStreamHandle,
         msg->header.sequenceId, 0,0,0,0);

   /* All Received Peer-Delay Request messages should be responded to regardless
    * of the Port / Stream state. The only exception is for unicast negotiation
    * and no contract exists. */
   if ((streamData->config.unicast == ZL303XX_TRUE) &&
       (portData->config.unicastNegotiation.enabled == ZL303XX_TRUE) &&
       (streamData->contractTx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].remainingSec == 0))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
         "Stream %lu: Ignored unicast Peer_Delay_Req (no contract).",
         streamData->clockStreamHandle, 0,0,0,0,0);
      return 0;
   }

   /* Packet has been accepted. */

   /* There are any number of Response scenarios (1-step or 2 formats for 2-step). */
   zl303xx_PtpV2IssuePeerDelayResp(streamData, &msg->header, &ptpData->recvMsgInfo.rxTs);


   /* Handle any appending TLVs (Only padding supported) */
   if ( msg->header.messageLength > ZL303XX_PTP_V2_PDREQ_MSG_LEN)
   {
      (void)zl303xx_PtpTlvEventMsgdHandle(streamData, msg, (ptpData->msgBuf + ZL303XX_PTP_V2_PDREQ_MSG_LEN));
   }

   return msg->header.messageLength;
}  /* END zl303xx_PtpV2handlePeerDelayReq */

/*

  Function Name:
    zl303xx_PtpV2handlePeerDelayResp

  Details:
   Handle a received PTP v2 peer delay response message

  Parameters:
   [in]  ptpData     Pointer to the PTP event message queue structure.
                           Contains the type info, the message buffer (often
                            the same as the packet buffer), etc.
   [in]  msg         The received message unpacked into a structure.
   [in]  streamData  Pointer to the clock stream instance structure (may
                           be NULL if there is no clock stream associated
                           with this received message, ex: CONTROL).
   [in]  portData    Pointer to the port data structure. Cannot be NULL.


  Return Value:
   The number of message bytes handled by this function on SUCCESS.
   0 otherwise.

 *****************************************************************************/
Uint16T zl303xx_PtpV2handlePeerDelayResp(
      zl303xx_PtpClockMsgQueueS *ptpData,
      zl303xx_PtpV2MsgS *msg,
      zl303xx_ClockStreamS *streamData,
      zl303xx_PortDataS *portData)
{
   /* Notes:
    * - the streamData pointer was checked by the parent, 'V2handleRecvMsg'
    * - all other pointers are generated locally or verified at the TASK,
    *   PROTOCOL or HANDLER levels.
    * - the message & packet lengths were verified by 'V2handleRecvMsg'
    * - the alternate master configuration was verified by 'V2handleRecvMsg'
    */
   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
         "Stream %u: Received PTP_PEER_DELAY_RESP event - seqId[%u]",
         streamData->clockStreamHandle,
         msg->header.sequenceId, 0,0,0,0);

   /* Accept the PDelay_Resp message only if issued by this port.
    * Other Port / Stream states do not matter. */
   if (ZL303XX_FALSE == zl303xx_PtpV2EqualPortIdentities(
                              &msg->data.delayResp.requestingPortIdentity,
                              &portData->portDS.portIdentity))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
         "Stream %u: Ignored PDelay_Resp (wrong requestingPortIdentity).",
         streamData->clockStreamHandle, 0,0,0,0,0);
      return 0;
   }

   /* Unicast mode must have an active contract. */
   else if ((streamData->config.unicast == ZL303XX_TRUE) &&
            (portData->config.unicastNegotiation.enabled == ZL303XX_TRUE) &&
            (streamData->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].remainingSec == 0))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
         "Stream %lu: Ignored unicastNeg PDelay_Resp (no active contract).",
         streamData->clockStreamHandle, 0,0,0,0,0);
      return 0;
   }

   /* Packet has been accepted. */
   /* Add any local delay asymmetry value to the correctionField of the
    * received event message. */
   {
      zl303xx_TimeInterval rxCorr;
      rxCorr.scaledNanoseconds = msg->header.correctionField;
      msg->header.correctionField = zl303xx_PtpTimeIntervalAdd(
                                          streamData->config.delayAsymmetry.ingress,
                                          rxCorr).scaledNanoseconds;
   }

   /* Register the peer delay response information with the timestamp record
    * handler module. A complete record requires the TX for the corresponding
    * peer delay request, this RX value and a far-end resident time (or T2/T3
    * values). If all 3 parts are available then this will trigger a timestamp
    * record ready event which will cause the complete request/response cycle to
    * be subsequently processed. */

   /* There are 3 formats to distinguish here:
    * - 1-step: this packet TS = 0; resident time in corrField; T4 will come from HW.
    * - 2-step RESIDENT: this packet TS = 0 ; corrField = 0; T4 will come from HW.
    * - 2-step RAW TS:   this packet TS = T2; corrField = 0; T4 will come from HW. */
   zl303xx_PtpTsRecordRegisterPdelayResp(streamData, msg, &ptpData->recvMsgInfo.rxTs);

   /* Table 24 of the IEEE-1588-2008 Std. specifies that the logMessageInterval
    * used in all PEER_DELAY messages must be the NULL_VALUE (unlike E2E DELAY
    * messages). If we receive otherwise, then log the error'ed value but do not
    * change our own transmit message rate.  */
   if (msg->header.logMessageInterval != PTP_V2_LMMI_NULL_VALUE)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
         "Stream %lu: Ignored PDelay_Resp logMessageInterval value(0x%02X).",
         streamData->clockStreamHandle, msg->header.logMessageInterval, 0,0,0,0);
   }

   /* Handle any appending TLVs (Only padding supported) */
   if ( msg->header.messageLength > ZL303XX_PTP_V2_PDRESP_MSG_LEN)
   {
      (void)zl303xx_PtpTlvEventMsgdHandle(streamData, msg, (ptpData->msgBuf + ZL303XX_PTP_V2_PDRESP_MSG_LEN));
   }

   return msg->header.messageLength;
}

/*

  Function Name:
    zl303xx_PtpV2handlePeerDelayFollowUp

  Details:
   Handle a received PTP v2 peer delay follow up message

  Parameters:
   [in]  ptpData     Pointer to the PTP event message queue structure.
                           Contains the type info, the message buffer (often
                            the same as the packet buffer), etc.
   [in]  msg         The received message unpacked into a structure.
   [in]  streamData  Pointer to the clock stream instance structure (may
                           be NULL if there is no clock stream associated
                           with this received message, ex: CONTROL).
   [in]  portData    Pointer to the port data structure. Cannot be NULL.


  Return Value:
   The number of message bytes handled by this function on SUCCESS.
   0 otherwise.

 *****************************************************************************/
Uint16T zl303xx_PtpV2handlePeerDelayFollowUp(
      zl303xx_PtpClockMsgQueueS *ptpData,
      zl303xx_PtpV2MsgS *msg,
      zl303xx_ClockStreamS *streamData,
      zl303xx_PortDataS *portData)
{
   /* Notes:
    * - the streamData pointer was checked by the parent, 'V2handleRecvMsg'
    * - all other pointers are generated locally or verified at the TASK,
    *   PROTOCOL or HANDLER levels.
    * - the message & packet lengths were verified by 'V2handleRecvMsg'
    * - the alternate master configuration was verified by 'V2handleRecvMsg'
    */

   /* Eliminate the 'unused' compiler warning. */
   (void)ptpData;

   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
         "Stream %u: Received PTP_PEER_DELAY_FOLLOWUP event - seqId[%u]",
         streamData->clockStreamHandle,
         msg->header.sequenceId, 0,0,0,0);

   /* Accept the PDelay_Resp_FollowUp message only if issued by this port.
    * Other Port / Stream states do not matter. */
   if (ZL303XX_FALSE == zl303xx_PtpV2EqualPortIdentities(
                              &msg->data.pdelayFollowUp.requestingPortIdentity,
                              &portData->portDS.portIdentity))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
         "Stream %u: Ignored PDelay_FollowUp (wrong requestingPortIdentity).",
         streamData->clockStreamHandle, 0,0,0,0,0);
      return 0;
   }

   /* In Unicast mode, PDELAY_RESP must have an active contract. */
   else if ((streamData->config.unicast == ZL303XX_TRUE) &&
            (portData->config.unicastNegotiation.enabled == ZL303XX_TRUE) &&
            (streamData->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].remainingSec == 0))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
         "Stream %lu: Ignored unicast PDelay_Resp (no active contract).",
         streamData->clockStreamHandle, 0,0,0,0,0);
      return 0;
   }

   /* Packet has been accepted. */

   /* Register the peer delay response information with the timestamp record
    * handler module. A complete record requires the TX for the corresponding
    * peer delay request, this RX value and a far-end resident time (or T2/T3
    * values). If all 3 parts are available then this will trigger a timestamp
    * record ready event which will cause the complete request/response cycle to
    * be subsequently processed. */

   zl303xx_PtpTsRecordRegisterPdelayFollowup(
         streamData,
         msg->header.sequenceId,
         &msg->header.correctionField,
         &msg->data.pdelayFollowUp.responseOriginTimestamp);

   return msg->header.messageLength;
}

/*

  Function Name:
    zl303xx_PtpV2handleFollowUp

  Details:
   Handle a received PTP v2 follow-up message

  Parameters:
   [in]    ptpData      Pointer to the PTP event message queue structure.
                              Contains the type info, the message buffer (often
                              the same as the packet buffer), etc.
   [in]    msg          The received message unpacked into a structure.
   [in]    streamData   Pointer to the clock stream instance structure (may
                              be NULL if there is no clock stream associated
                              with this received message, ex: CONTROL).
   [in]    portData     Pointer to the port data structure. Cannot be NULL.


  Return Value:
   The number of message bytes handled by this function on SUCCESS.
   0 otherwise.

 *****************************************************************************/
Uint16T zl303xx_PtpV2handleFollowUp(zl303xx_PtpClockMsgQueueS *ptpData,
                                    zl303xx_PtpV2MsgS *msg,
                                    zl303xx_ClockStreamS *streamData,
                                    zl303xx_PortDataS *portData)
{
   /* Notes:
    * - the streamData pointer was checked by the parent, 'V2handleRecvMsg'
    * - all other pointers are generated locally or verified at the TASK,
    *   PROTOCOL or HANDLER levels.
    * - the message & packet lengths were verified by 'V2handleRecvMsg'
    * - the alternate master configuration was verified by 'V2handleRecvMsg'
    */

   zl303xx_PtpClockS *pPtpClock = portData->clock;

   /* Eliminate the 'unused' compiler warning. */
   (void)ptpData;

   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
         "Stream %u: Received PTP_FOLLOWUP event - seqId[%u]",
         streamData->clockStreamHandle,
         msg->header.sequenceId, 0,0,0,0);

   /* Multicast stream: accept the Follow_Up message based on the current state
    * and the sourcePortIdentity in the header. */
   if (streamData->config.unicast == ZL303XX_FALSE)
   {
      if (((streamData->streamSubstate != PTP_STREAM_SUBSTATE_UNCALIBRATED) &&
           (streamData->streamSubstate != PTP_STREAM_SUBSTATE_SLAVE)) ||
          (zl303xx_PtpV2EqualPortIdentities(&msg->header.sourcePortIdentity,
                                            &pPtpClock->parentDS.parentPortIdentity) == ZL303XX_FALSE))
      {
         /* Packet not accepted in this mode */
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %u: Ignored multicast Follow_Up.",
            streamData->clockStreamHandle, 0,0,0,0,0);
         return 0;
      }
   }
   /* Unicast negotiated stream: accept the Follow_Up message if we have an
    * active contract to receive Sync. */
   else if (portData->config.unicastNegotiation.enabled)
   {
      if (streamData->contractRx[PTP_NEGOTIATED_MSG_ID_SYNC].remainingSec == 0)
      {
         /* Packet not accepted in this mode */
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %u: Ignored unicast Follow_Up (no contract).",
            streamData->clockStreamHandle, 0,0,0,0,0);
         return 0;
      }
   }
   /* Unicast non-negotiated stream: accept the Follow_Up message based on the
    * current state (must be UNCALIBRATED or SLAVE). */
   else
   {
      if ((streamData->streamSubstate != PTP_STREAM_SUBSTATE_UNCALIBRATED) &&
          (streamData->streamSubstate != PTP_STREAM_SUBSTATE_SLAVE))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %lu: Ignored unicast Follow_Up (stream not UNCALIBRATED or SLAVE).",
            streamData->clockStreamHandle, 0,0,0,0,0);
         return 0;
      }
   }

   /* Packet has been accepted. */
   zl303xx_PtpStreamTwoStepRegisterTx(streamData, msg->header.sequenceId,
                                      &msg->data.followUp.preciseOriginTimestamp,
                                      &msg->header.correctionField,
                                      &msg->header.sourcePortIdentity);

   return msg->header.messageLength;
}

/*

  Function Name:
    zl303xx_PtpV2handleDelayResp

  Details:
   Handle a received PTP v2 delay response message

  Parameters:
   [in]    ptpData      Pointer to the PTP event message queue structure.
                              Contains the type info, the message buffer (often
                              the same as the packet buffer), etc.
   [in]    msg          The received message unpacked into a structure.
   [in]    streamData   Pointer to the clock stream instance structure (may
                              be NULL if there is no clock stream associated
                              with this received message, ex: CONTROL).
   [in]    portData     Pointer to the port data structure. Cannot be NULL.


  Return Value:
   The number of message bytes handled by this function on SUCCESS.
   0 otherwise.

 *****************************************************************************/
Uint16T zl303xx_PtpV2handleDelayResp(zl303xx_PtpClockMsgQueueS *ptpData,
                                      zl303xx_PtpV2MsgS *msg,
                                      zl303xx_ClockStreamS *streamData,
                                      zl303xx_PortDataS *portData)
{
   /* Notes:
    * - the streamData pointer was checked by the parent, 'V2handleRecvMsg'
    * - all other pointers are generated locally or verified at the TASK,
    *   PROTOCOL or HANDLER levels.
    * - the message & packet lengths were verified by 'V2handleRecvMsg'
    * - the alternate master configuration was verified by 'V2handleRecvMsg'
    */

   zl303xx_PtpClockS *pPtpClock = portData->clock;

   /* Eliminate the 'unused' compiler warning. */
   (void)ptpData;

   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
         "Stream %u: Received PTP_DELAY_RESP event - seqId[%u]",
         streamData->clockStreamHandle,
         msg->header.sequenceId, 0,0,0,0);

   /* Multicast stream: accept the Delay_Resp message based on the current state
    * and soucePortIdentity in the header. */
   if (streamData->config.unicast == ZL303XX_FALSE)
   {
      if (((streamData->streamSubstate != PTP_STREAM_SUBSTATE_UNCALIBRATED) &&
           (streamData->streamSubstate != PTP_STREAM_SUBSTATE_SLAVE)) ||
          /* Is the msg not from the current master? */
          (zl303xx_PtpV2EqualPortIdentities(&msg->header.sourcePortIdentity,
                                            &pPtpClock->parentDS.parentPortIdentity) == ZL303XX_FALSE))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %lu: Ignored multicast Delay_Resp.",
            streamData->clockStreamHandle, 0,0,0,0,0);
         return 0;
      }
   }
   /* Unicast negotiated stream: accept the Delay_Resp message if we have an
    * active contract to receive. */
   else if (portData->config.unicastNegotiation.enabled)
   {
      if (streamData->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].remainingSec == 0)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %lu: Ignored unicast Delay_Resp (no contract).",
            streamData->clockStreamHandle, 0,0,0,0,0);
         return 0;
      }
   }
   /* Unicast non-negotiated stream: accept the Delay_Resp message based on the
    * current state (must be UNCALIBRATED or SLAVE). */
   else
   {
      if ((streamData->streamSubstate != PTP_STREAM_SUBSTATE_UNCALIBRATED) &&
          (streamData->streamSubstate != PTP_STREAM_SUBSTATE_SLAVE))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %lu: Ignored unicast Delay_Resp (stream not UNCALIBRATED or SLAVE).",
            streamData->clockStreamHandle, 0,0,0,0,0);
         return 0;
      }
   }

   /* Check the message is in response to our own delay_response. We could also check the sequence
      number to ensure it is the latest but this is not an important check for us */
   if (zl303xx_PtpV2EqualPortIdentities(&msg->data.delayResp.requestingPortIdentity,
                                        &portData->portDS.portIdentity) == ZL303XX_FALSE)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
         "Stream %u: Ignored Delay_Resp (wrong requestingPortIdentity).",
         streamData->clockStreamHandle, 0,0,0,0,0);
      return 0;
   }

   /* Packet has been accepted. */

   /* Register the delay response information with the timestamp record handler module.
      If there is transmit timestamp information available for a corresponding delay request
      then this will trigger a timestamp record ready event which will cause the complete
      request/response cycle to be subsequently processed */
   zl303xx_PtpTsRecordRegisterIngress(
            streamData, &msg->header.sourcePortIdentity,
            ZL303XX_MSG_ID_DELAY_REQ, /* This RESP should already have a REQ record. */
            msg->header.sequenceId,
            &msg->data.delayResp.receiveTimestamp,
            &msg->header.correctionField);

   /* If the latest delay response specifies a minimum message period
    * that is greater than the current logMinDelayReqInterval, restart
    * the DELAY_REQ timer with this new (slower) value. */
   if (msg->header.logMessageInterval != PTP_V2_LMMI_NULL_VALUE)
   {
      Sint8T newIntvl = max(msg->header.logMessageInterval, streamData->config.logMinDelayReqInterval);
      if (newIntvl != streamData->msgParams[ZL303XX_MSG_ID_DELAY_REQ].log2intervalAct)
      {
         /* Restart the timer with the new value */
         streamData->msgParams[ZL303XX_MSG_ID_DELAY_REQ].log2intervalAct = newIntvl;
         zl303xx_PtpStreamTimerStart(streamData, DELAY_REQ_TIMER);
      }
   }

   return msg->header.messageLength;
}

/*

  Function Name:
    zl303xx_PtpV2handleAnnounce

  Details:
   Handle a received PTP v2 ANNOUNCE message

  Parameters:
   [in]    ptpData      Pointer to the PTP event message queue structure.
                              Contains the type info, the message buffer (often
                              the same as the packet buffer), etc.
   [in]    msg          The received message unpacked into a structure.
   [in]    streamData   Pointer to the clock stream instance structure (may
                              be NULL if there is no clock stream associated
                              with this received message, ex: CONTROL).
   [in]    portData     Pointer to the port data structure. Cannot be NULL.


  Return Value:
   The number of message bytes handled by this function on SUCCESS.
   0 otherwise.

 *****************************************************************************/
Uint16T zl303xx_PtpV2handleAnnounce(
      zl303xx_PtpClockMsgQueueS *ptpData,
      zl303xx_PtpV2MsgS *msg,
      zl303xx_ClockStreamS *streamData,
      zl303xx_PortDataS *portData)
{
   /* Notes:
    * - the streamData pointer was checked by the parent, 'V2handleRecvMsg'
    * - all other pointers are generated locally or verified at the TASK,
    *   PROTOCOL or HANDLER levels.
    * - the message & packet lengths were verified by 'V2handleRecvMsg'
    * - the alternate master configuration was verified by 'V2handleRecvMsg'
    */

   const char *fnName = "zl303xx_PtpV2handleAnnounce";
   zl303xx_PtpClockS *pClock = portData->clock;

   zl303xx_PtpV2MsgAnnounceS *announce = &msg->data.announce;
   zl303xx_BooleanE anncOverride = ZL303XX_TRUE;

   /* PATH TRACE varables */
   Uint8T * ptListStart = NULL;
   Uint8T * ptTlvStart = NULL;

   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
         "%s: Stream %u: Received PTP_ANNOUNCE event - seqId[%u]",
         fnName, streamData->clockStreamHandle,
         msg->header.sequenceId, 0,0,0);

   /* If this stream uses an ingress clockClass over-ride value replace it now
    * so that all checks are done with the value that will be used by BMC. The
    * farEndParentDS will also store this value (if enabled).    */
   if (streamData->config.ingressClockClass != PTP_CLOCK_CLASS_OVERRIDE_DISABLED)
   {
      announce->grandmasterClockQuality.clockClass = streamData->config.ingressClockClass;
   }
   else if (pClock->config.ingressClockClass != PTP_CLOCK_CLASS_OVERRIDE_DISABLED)
   {
      announce->grandmasterClockQuality.clockClass = pClock->config.ingressClockClass;
   }
   else
   {
      anncOverride = ZL303XX_FALSE;
   }

   /* If PATH_TRACE is enabled, check if this ANNOUNCE message is in a loop. */
   /* TODO: sub-routine */
   if (portData->clock->config.pathTrace.enabled == ZL303XX_TRUE)
   {
      zl303xx_ClockIdentity ptClock;
      Uint16T tlvType, tlvLength, listCount;

      ptTlvStart = zl303xx_PtpTlvPathTraceFind(msg, ptpData->msgBuf);

      if (ptTlvStart != NULL)
      {
         /* Get the Type & Length of this TLV */
         zl303xx_PtpTlvTypeLengthGet(ptTlvStart, &tlvType, &tlvLength);

         /* Assign the start of the Path Trace List. */
         /* The TYPE and Length take up 2 bytes each. */
         ptListStart = ptTlvStart + ZL303XX_PTP_TLV_BASE_LEN;

         for (listCount = 0;
              listCount < (tlvLength / PTP_V2_CLK_IDENTITY_LENGTH);
              listCount++)
         {
            /* Copy the clock Id to a local structure to avoid byte
             * alignment warnings. */
            OS_MEMCPY(ptClock,
                   ptListStart + (listCount * PTP_V2_CLK_IDENTITY_LENGTH),
                   PTP_V2_CLK_IDENTITY_LENGTH);

            /* Check if this path already includes the local clock. */
            if (zl303xx_PtpV2EqualClockIdentities(ptClock,
                           pClock->config.defaultDS.clockIdentity))
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
                  "Stream %u: Dropping Announce (PATH_TRACE_LOOP detected).",
                  streamData->clockStreamHandle, 0,0,0,0,0);

               return 0;
            }
         }
      }
   }

   /* Multicast & unicast non-negotiated stream: accept the Announce message
    * based on the current state (cannot be INIT or IDLE). */
   if ((streamData->config.unicast == ZL303XX_FALSE) ||
       (portData->config.unicastNegotiation.enabled == ZL303XX_FALSE))
   {
      if ((streamData->streamSubstate == PTP_STREAM_SUBSTATE_INIT) ||
          (streamData->streamSubstate == PTP_STREAM_SUBSTATE_IDLE))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %u: Ignored Announce (stream in INIT or IDLE state).",
            streamData->clockStreamHandle, 0,0,0,0,0);
         return 0;
      }
   }
   else
   {
      /* Unicast negotiated stream: accept the Announce message if we have an
       * active contract to receive. */
      if (streamData->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].remainingSec == 0)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %u: Ignored unicast Announce (no contract).",
            streamData->clockStreamHandle, 0,0,0,0,0);
         return 0;
      }
   }

   /* Do not accept announce messages if the stream is running in MASTER_ONLY
    * mode. Adding foreign master entries for a stream which can only run in
    * master mode can corrupt the BMC algorithm. */
   if ((streamData->config.mode == ZL303XX_PTP_STREAM_MASTER_ONLY) ||
       (streamData->portData->config.masterOnly == ZL303XX_TRUE))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
         "Stream %u: Ignored Announce (received on MASTER_ONLY stream).",
         streamData->clockStreamHandle, 0,0,0,0,0);

      return 0;
   }

   /* Check if this is a "rogue frame" as per section 9.3.2.5 subsection (d) of
    * the standard. */
   if (announce->stepsRemoved >= pClock->config.optionalDefaultDS.maxStepsRemoved)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %u: Ignored Announce (stepsRemoved >= Clock Maximum (%d)).",
            streamData->clockStreamHandle,
            pClock->config.optionalDefaultDS.maxStepsRemoved, 0,0,0,0);

      /* Trigger an event notification to the user. */
      (void)zl303xx_PtpEventRxStepsRemovedExceeded(portData, msg);

      return 0;
   }

   /* grandmasterClockQuality.clockClassがEEPROMから取得したaccepted_clock_classesより大きい場合停波を行う */
   /* 連続で停波を投げないように前回値と比較して差分が合った場合のみ実施する */
   if(announce->grandmasterClockQuality.clockClass != keep_announce_clockclass)
   {
	  if(announce->grandmasterClockQuality.clockClass > accepted_clock_classes)
	  {
		/* 停波 */
		ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Announce Checks failed. stop wave. coming clockclass =%u, accepted_clock_classes = %u",
            announce->grandmasterClockQuality.clockClass, accepted_clock_classes,0,0,0,0);
        f_com_SVCtl(E_DU_SVCTL_OPE_STP_WAV, D_DU_SVCTL_ON, D_DU_SVCTL_ON_WAVE_STOP_ALL_PTP);
	  }
	  else
	  {
		/* 再開 */
		ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Announce Checks recovered. restart wave. coming clockclass =%u, accepted_clock_classes = %u",
            announce->grandmasterClockQuality.clockClass, accepted_clock_classes,0,0,0,0);
        f_com_SVCtl(E_DU_SVCTL_OPE_STP_WAV, D_DU_SVCTL_OFF, D_DU_SVCTL_ON_WAVE_STOP_ALL_PTP);

	  }
	  /* 値保持 */
	  keep_announce_clockclass = announce->grandmasterClockQuality.clockClass;
     BPF_COM_CMD_SET_CUR_CLK_CLASS((UCHAR)keep_announce_clockclass);	/* FUJITSU added for DU command */

   }

   /* Execute any Profile specific checks. */
   if (ZL303XX_OK != zl303xx_PtpProfileAnncRxCheck(streamData, msg, ptpData->msgBuf + ZL303XX_PTP_V2_ANNC_MSG_LEN))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %u: Profile Specific  Announce Checks failed.",
            streamData->clockStreamHandle, 0,0,0,0,0);
      return 0;
   }

   /* Check that an Appropriate level of ClockClass is received.
    * If not, raise the PTSF here but DO NOT return here. Instead, allow the
    * ANNC-RX-TIMER to restart below and disqualify the entry in the FMT. */
   if (ZL303XX_STREAM_MAX_CLOCK_CLASS_OK(
            streamData,
            announce->grandmasterClockQuality.clockClass) == ZL303XX_FALSE)
   {
      /* This Class is outside our acceptable range so discard it. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            "Stream %u: maxClockClass(%d) exceeded by %s(%d).",
            streamData->clockStreamHandle,
            streamData->config.maxClockClass,
            (anncOverride) ? "Overide Value" : "RX Announce Class",
            announce->grandmasterClockQuality.clockClass, 0,0);

      /* PTSF-QLTooLow is currently only generated for unicast streams. */
      zl303xx_PtsfFlagSet(streamData->clockStreamHandle,
                          ZL303XX_PTSF_QL_TOO_LOW,
                          streamData->config.unicast);
   }

   /* For a multicast stream using BMCA, only overwrite the stream's data sets
    * under certain conditions. */
   if ((streamData->config.unicast == ZL303XX_TRUE) ||
       (streamData->config.mode == ZL303XX_PTP_STREAM_SLAVE_ONLY) ||
       (zl303xx_PtpV2EqualClockIdentities(streamData->farEndParentDS.u.v2.parentPortIdentity.clockIdentity,
                                          msg->header.sourcePortIdentity.clockIdentity) == ZL303XX_TRUE) ||

               /* If revertive mode is enabled, only overwrite data sets if this
                * announce message was from the best master.
                * Note: Telecom Profile will use best master regardless of
                * revertive mode being used. */
          ((   (pClock->config.bmca.revertiveEn ||
                pClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1) &&
               (pClock->foreignMasterDS.bestRecord == 0) &&
               (zl303xx_PtpV2EqualClockIdentities(
                   pClock->foreignMasterDS.foreign[0].compareData.grandmasterIdentity,
                   announce->grandmasterIdentity))) ||

            /* Else (non-revertive mode), only overwrite data sets if this
             * announce message was from the selected master. */
        (   (pClock->config.bmca.revertiveEn == ZL303XX_FALSE) &&
            (pClock->bmca.selectedFmtEntry != ZL303XX_PTP_FMT_INDEX_INVALID) &&
            (zl303xx_PtpV2EqualClockIdentities(
               pClock->foreignMasterDS.foreign[pClock->bmca.selectedFmtEntry].compareData.grandmasterIdentity,
               announce->grandmasterIdentity)))))
   {
      /* The Data Sets of the Streams store the receive-side data only.
       * When a stream issues an ANNOUNCE message, it uses the Clock Data Set
       * information for advertising.
       * Update the Stream's receive data sets first. */

      /* Stream's Current data set */
      streamData->farEndCurrentDS.stepsRemoved = announce->stepsRemoved + 1;

      /* Stream's Parent data set */
      streamData->farEndParentDS.u.v2.parentPortIdentity = msg->header.sourcePortIdentity;

      OS_MEMCPY(streamData->farEndParentDS.u.v2.grandmasterIdentity,
             announce->grandmasterIdentity,
             PTP_V2_CLK_IDENTITY_LENGTH);

      streamData->farEndParentDS.u.v2.grandmasterClockQuality = announce->grandmasterClockQuality;

      streamData->farEndParentDS.u.v2.grandmasterPriority1 = announce->grandmasterPriority1;
      streamData->farEndParentDS.u.v2.grandmasterPriority2 = announce->grandmasterPriority2;

      /* Update the Streams's Time Properties data set from the received message.
       * The stream does not have a Time-Properties Data Set. */
      streamData->farEndTimePropertiesDS.currentUtcOffset = announce->currentUtcOffset;
      streamData->farEndTimePropertiesDS.timeSource = announce->timeSource;
      streamData->farEndTimePropertiesDS.currentUtcOffsetValid = getFlag(msg->header.flagField, PTP2_FLAGS_CURRENT_UTC_OFFSET_VALID);
      streamData->farEndTimePropertiesDS.leap59 = getFlag(msg->header.flagField, PTP2_FLAGS_LI_59);
      streamData->farEndTimePropertiesDS.leap61 = getFlag(msg->header.flagField, PTP2_FLAGS_LI_61);
      streamData->farEndTimePropertiesDS.timeTraceable = getFlag(msg->header.flagField, PTP2_FLAGS_TIME_TRACEABLE);
      streamData->farEndTimePropertiesDS.frequencyTraceable = getFlag(msg->header.flagField, PTP2_FLAGS_FREQ_TRACEABLE);
      streamData->farEndTimePropertiesDS.ptpTimescale = getFlag(msg->header.flagField, PTP2_FLAGS_PTP_TIMESCALE);
      streamData->farEndTimePropertiesDS.synchronizationUncertain = getFlag(msg->header.flagField, PTP2_FLAGS_SYNCHRO_UNCERTAIN);

      /* Telecom Profile: Convert the clock class to a SSM Quality Level. */
      if ((pClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1) &&
          (streamData->pUmtEntry))
      {
         zl303xx_PtpTelecomBmcaDataUpdate(&streamData->pUmtEntry->telecom,
                                          streamData->farEndParentDS.u.v2.grandmasterClockQuality.clockClass,
                                          streamData->farEndCurrentDS.stepsRemoved);
      }
   }

   /* FOR Mixed Mode:
    * ***************
    * Update parent data set */
   {
      if ((streamData->streamSubstate == PTP_STREAM_SUBSTATE_SLAVE) &&
          (portData->config.mixedMode == ZL303XX_TRUE))
      {
         zl303xx_ClockStreamS *delayStream = zl303xx_PtpStreamExistsOnPort(portData, &ptpData->recvMsgInfo.srcAddr);

         if (delayStream != NULL)
         {
            delayStream->farEndParentDS.u.v2.parentPortIdentity = msg->header.sourcePortIdentity;
         }
      }
   }

   /* A new ANNOUNCE record has been received. Do the following:
    * - Determine the new clockBest for the clock.
    * - Update any data sets (if necessary).
    * - Invoke any port/stream transitions (if necessary).  */

   /* Update the Clock Foreign Master Table. */
   {
      (void)zl303xx_PtpFmtEntryUpdate(&pClock->foreignMasterDS,
                                      &msg->header, announce, streamData,
                                      &ptpData->recvMsgInfo.srcAddr);

      /* If using non-revertive BMCA, this will determine the selected master. */
      if (pClock->config.bmca.revertiveEn == ZL303XX_FALSE)
      {
         pClock->bmca.selectedFmtEntry = zl303xx_PtpFmtSelectedGet(
                                            &pClock->foreignMasterDS,
                                            &pClock->bmca.selectedAddr);
      }
   }

   /* Reset the ANNOUNCE_RECEIPT_TIMER back to 0 if the received message was
    * from the selected ParentDS server. If this is the first ANNOUNCE message
    * for this stream then initialize and start the timer from the
    * log2AnnounceInterval configured for the stream. */
   /* The same is true for PATH TRACE info in the Foreign Master Table. */
   {
      zlStatusE tmStatus = ZL303XX_OK;

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4,
            "%s: Stream %u: ANNOUNCE_RECEIPT_TIMER reset interval",
            fnName,
            streamData->clockStreamHandle, 0,0,0,0);

      /* In the multicast OR static unicast case, ANNOUNCE messages may be
       * received on the same stream from multiple sources. In that case,
       * determine if this message is from the ParentDS source, and if so,
       * restart the ANNOUNCE_RECEIPT_TIMER. */
      /* In the unicast-negotiated case, always restart the timer. */
      if (
            /* Unicast-Negotiated case: restart */
            ((streamData->config.unicast == ZL303XX_TRUE) &&
             (portData->config.unicastNegotiation.enabled == ZL303XX_TRUE)) ||
            /* Multicast and Static Unicast case. */
            (zl303xx_PtpV2EqualPortIdentities(
                     &msg->header.sourcePortIdentity,
                     &pClock->parentDS.parentPortIdentity) == ZL303XX_TRUE))
      {
         /* To avoid having to constantly calculate the ANNOUNCE interval from the
          * log2 ANNOUNCE setting, just restart the timer if it is already active. */
         if (zl303xx_PtpTimerActive(&streamData->itimer[ANNOUNCE_RECEIPT_TIMER]))
         {
            tmStatus = zl303xx_PtpTimerRestart(&streamData->itimer[ANNOUNCE_RECEIPT_TIMER]);
         }
         else  /* This is the first ANNOUNCE message so start the timer. */
         {
            tmStatus = zl303xx_PtpStreamTimerStart(streamData, ANNOUNCE_RECEIPT_TIMER);
         }

         if (tmStatus != ZL303XX_OK)
         {
            ZL303XX_TRACE_ALWAYS("%s: Announce Timer Failed: Status %lu",
                  fnName,
                  tmStatus, 0,0,0,0);
         }

         /* Add the Path-Trace data to the FMT record. ANY TLV was found earlier
          * if the functionality was enabled. */
         else if (ptTlvStart != NULL)
         {
            if (ZL303XX_OK != zl303xx_PtpFmtEntryPathTraceUpdate(&pClock->foreignMasterDS,
                              &msg->header.sourcePortIdentity, streamData, ptTlvStart))
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                     "%s: Update of Clock FMT Failed: PATH-TRACE",
                     fnName, 0,0,0,0,0);
            }
         }
      }
   }  /* END of ANNOUNCE_RX timer reset. */

   /* Telecom Profile: Check for the PTSF-lossAnnounce condition. */
   if (((pClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1) ||
        (pClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_2)) &&
       (streamData->pUmtEntry))
   {
      zl303xx_PtsfFlagSet(streamData->clockStreamHandle, ZL303XX_PTSF_LOSS_ANNOUNCE, ZL303XX_FALSE);
   }

   /* Check if there are any TLVs attached to the end of the ANNC message. */
   if (msg->header.messageLength > ZL303XX_PTP_V2_ANNC_MSG_LEN)
   {
      (void)zl303xx_PtpTlvAppendedHandle(streamData, msg, (ptpData->msgBuf + ZL303XX_PTP_V2_ANNC_MSG_LEN));
   }

   /* Mark the entire received message as processed. */
   return msg->header.messageLength;
}  /*END zl303xx_PtpV2handleAnnounce */

/*

  Function Name:
    zl303xx_PtpV2handleSignaling

  Details:
   Handle a received PTP V2 signaling message.

  Parameters:
   [in]    ptpData      Pointer to the PTP event message queue structure.
                              Contains the type info, the message buffer (often
                              the same as the packet buffer), etc.
   [in]    msg          The received message unpacked into a structure.
   [in]    streamData   Pointer to the clock stream instance structure (may
                              be NULL if there is no clock stream associated
                              with this received message, ex: CONTROL).
   [in]    portData     Pointer to the port data structure. Cannot be NULL.


  Return Value:
   The number of message bytes handled by this function on SUCCESS.
   0 otherwise.

 *****************************************************************************/
Uint16T zl303xx_PtpV2handleSignaling(zl303xx_PtpClockMsgQueueS *ptpData,
                                     zl303xx_PtpV2MsgS *msg,
                                     zl303xx_ClockStreamS *streamData,
                                     zl303xx_PortDataS *portData)
{
   /* Notes:
    * - the streamData pointer was checked by the parent, 'V2handleRecvMsg'
    * - all other pointers are generated locally or verified at the TASK,
    *   PROTOCOL or HANDLER levels.
    * - the message & packet lengths were verified by 'V2handleRecvMsg'
    * - the alternate master configuration was verified by 'V2handleRecvMsg'
    */

   /* Maintain a status for internal purposes. */
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtpV2handleSignaling";

   /* Counters to control the processing of the packet. */
   /* This routine only handles the Signal data; the PTP header portion has
    * already been unpacked so adjust the buffer counters accordingly. */
   Uint16T msgBytesConsumed = ZL303XX_PTP_V2_SIGNAL_MSG_LEN;
   Uint16T msgBytesRemaining = msg->header.messageLength - msgBytesConsumed;

   /* The scope of application of the message (single clock OR all clocks). */
   zl303xx_PtpV2MgmtMsgScopeE msgScope = PTPV2_MGMT_SCOPE_NONE;

   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
         "Received PTP_SIGNALING event - seqId[%u]: Input Buffer length=%d: Ptp msgLen %d",
         msg->header.sequenceId,
         ptpData->pktLength, msg->header.messageLength, 0,0,0);

   /* Ignore unicast messages on the multicast stream. This is not an error,
    * just return immediately. */
   if (status == ZL303XX_OK)
   {
      if (!(msg->header.flagField & PTP2_FLAGS_UNICAST))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "Port %lu: Received %s TLV from multicast address.",
                       portData->clockPortHandle, 0,0,0,0,0);
         /* Return status = ZL303XX_OK since this is not a fatal error */
         return status;
      }

      /* Determine the message SCOPE of any attached TLVs. */

      /* The message should be accepted by this clock (IEEE 1588 v2 section 13.12)
       * if either:
       * - a. the target clock identity matches the local clock identity or
       * - b. the target clock identify is all 1's in which case the message
       *      is for all clocks in the domain. */
      /* The port scope is defined as:
       *    a.) the portNumber is all 0's: indicates the internal clock port.
       *    b.) the portNumber is all 1's: indicates all ports within the clock scope.
       *    c.) otherwise: indicates a single port within the clock scope.    */
      msgScope = zl303xx_PtpV2MgmtMsgEvaluateScope(&msg->data.signaling.targetPortIdentity);

      if (msgScope == PTPV2_MGMT_SCOPE_NONE)
      {
         /* Message did not contain a valid port specifier */
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "Port %u: Received malformed SIGNALING packet",
               portData->config.portNumber, 0,0,0,0,0);

         /* Count this as a malformed packet? */
         zl303xx_IncrementMalformedPktCount(portData);

         /* Closest error to this scenario. */
         status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
      }
   }

   /* Process any attached TLVs. */
   if (status == ZL303XX_OK)
   {
      /* Message needs to be handled by this clock */
      Uint8T *nextTlvBuf = NULL;      /* The start of the next TLV to process. */
      Uint16T tlvConsumedBytes = 0;   /* Bytes processed by a TLV. */
      Uint16T tlvCount = 0;           /* Number of TLVs processed (for logging). */
      Uint16T tlvType, tlvLen;        /* Confirm TLV Type. */
      zl303xx_PtpV2SignalTlvS rxTlv;    /* Received UnicastNeg TLV structure. */
      zl303xx_PtpV2SignalTlvS txTlv;    /* Response UnicastNeg TLV structure. */

      /* Response TLV Buffer. */
      Uint8T pUniNegResponseTlvBuf[PTP_NEGOTIATED_NUM_MSG_TYPES * ZL303XX_PTP_V2_UNI_NEG_TLV_LEN_MAX];
      Uint8T uniNegResponseTlvBufLen = 0;

      /* Continue processing TLVs as long as another is present. */
      while ((status == ZL303XX_OK) &&
             (msgBytesRemaining > ZL303XX_PTP_TLV_BASE_LEN))
      {
         /* Start of the next TLV... */
         nextTlvBuf = ptpData->msgBuf + msgBytesConsumed;

         /* Get the TLV type to determine how to proceed. */
         /* We don't expect MGMT TLVs but VENDOR extensions are possible. */
         (void)zl303xx_PtpTlvTypeLengthGet(nextTlvBuf, &tlvType, &tlvLen);

         if ((tlvType < PTP_TLV_REQUEST_UNICAST_TX) ||
             (tlvType > PTP_TLV_CANCEL_UNICAST_TX_ACK))
         {
            /* Pass the TLV off to the general MGMT Handler since this routine
             * expect SIGNALING TLVs */
            (void)zl303xx_PtpTlvAppendedHandle(streamData, msg, nextTlvBuf);

            /* We ignore how many bytes the MGMT handler actually used and chew
             * off all bytes in the TLV to keep things aligned. */
            tlvConsumedBytes = tlvLen + ZL303XX_PTP_TLV_BASE_LEN;
            msgBytesConsumed += tlvConsumedBytes;
            msgBytesRemaining -= tlvConsumedBytes;
            tlvCount++;

            /* Skip to the next TLV. */
            continue;
         }

         /* Unpack this TLV. */
         tlvConsumedBytes = zl303xx_PtpV2SignalTlvUnpack(
                               nextTlvBuf, msgBytesRemaining, &rxTlv);

         /* Determine if there was an error unpacking the TLV. The following
          * scenarios will cause an error (tlvConsumedBytes == 0):
          * - the msgBytesRemaining < 4 (2 needed for TYPE, 2 needed for LENGTH).
          * - the TLV type is not recognized.
          * - msgBytesRemaining is too short for the tlvType.
          * - the embedded tlvLength in the TLV is too short for the tlvType. */
         if (tlvConsumedBytes == 0)
         {
            /* Log and exit. */
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                  "%s: Error unpacking SIGNALING TLV: TLVs processed %d:",
                  fnName,
                  tlvCount, 0,0,0,0);

            /* Use a distinct error code. */
            status = ZL303XX_PARAMETER_INVALID;
         }

         else  /* TLV was successfully parsed so process it. */
         {
            status = zl303xx_PtpV2SignalUnicastNegTlvHandle(
                        ptpData, &rxTlv, &streamData, portData, &txTlv);

            /* If processing was successful update the counts for the next
             * TLV (if any). */
            if (status == ZL303XX_OK)
            {
               msgBytesConsumed += tlvConsumedBytes;
               msgBytesRemaining -= tlvConsumedBytes;
               tlvCount++;

               /* Save the Far-End Port Identity for future messages. */
               if (streamData != NULL)
               {
                  streamData->unicast.farEndPortIdentity = msg->header.sourcePortIdentity;
               }

               /* IF RESPONSE is required, PACK it to the buffer now. */
               if (txTlv.tlvType != 0)
               {
                  /* Pack the Unicast Negotiation TLV. */
                  uniNegResponseTlvBufLen += zl303xx_PtpV2SignalTlvPack(
                                                &txTlv,
                                                pUniNegResponseTlvBuf + uniNegResponseTlvBufLen);
               }
            }

            else /* handling ERROR */
            {
               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                     "%s: UnicastNeg TLV Error: bufLen %d: ptpMsgLen %d: processed %d remaining %d: TLV count %d:",
                     fnName,
                     ptpData->pktLength,
                     msg->header.messageLength,
                     msgBytesConsumed,
                     msgBytesRemaining, tlvCount);
            }
         }
      }

      if (uniNegResponseTlvBufLen > 0)
      {
         status = zl303xx_PtpV2UnicastNegTlvRespond(
                                 &ptpData->recvMsgInfo.srcAddr,
                                 streamData, portData,
                                 &msg->header,
                                 pUniNegResponseTlvBuf, uniNegResponseTlvBufLen);
      }

      /* If a single TLV processing fails, count the packet as malformed. */
      if (status != ZL303XX_OK)
      {
         zl303xx_IncrementMalformedPktCount(portData);
      }

      /* We expect no bytes to be left over.       */
      if (msgBytesRemaining != 0)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
               "%s: Unprocessed UnicastNeg TLV bytes: origLen %d: remainingLen %d: TLVs processed %d:",
               fnName,
               ptpData->pktLength, msgBytesRemaining, tlvCount, 0,0);
      }

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4,
            "%s: Processed UnicastNeg TLV: Response bufLen %d: Rx ptpMsgLen %d: TLVs count %d:",
            fnName,
            uniNegResponseTlvBufLen,
            msg->header.messageLength, tlvCount, 0,0);
   }

   return msgBytesConsumed;
}   /* END zl303xx_PtpV2handleSignaling */

/*

  Function Name:
    zl303xx_PtpV2handleManagement

  Details:
   Handle a received PTP v2 management message

  Parameters:
   [in]  pPtpRxQueueData   Pointer to the PTP event message queue structure.
                                 Contains the type info, the message buffer
                                 (often the same as the packet buffer), etc.
   [in]  pPtpMsgRx         The received message unpacked into a structure.
   [in]  pPtpStream        Pointer to the clock stream instance structure (may
                                 be NULL if there is no clock stream associated
                                 with this received message, ex: CONTROL).
   [in]  pPtpPort          Pointer to the port data structure. Cannot be NULL.


  Return Value:
   The number of message bytes handled by this function on SUCCESS.
   0 otherwise.

 *****************************************************************************/
Uint16T zl303xx_PtpV2MsgMgmtHandle(
      zl303xx_PtpClockMsgQueueS *pPtpRxQueueData,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PortDataS *pPtpPort)
{
   /* Set the Default return value. */
   Uint16T totalConsumedMsgBytes = 0;

   /* De-reference the header & management data to make code more readable. */
   zl303xx_PtpV2MsgHeaderS      *pPtpHdrS = &pPtpMsgRx->header;
   zl303xx_PtpV2MsgManagementS  *pMgmtHdrS = &pPtpMsgRx->data.management;

   /* Get the Clock pointer for the port. */
   zl303xx_PtpClockS *pPtpClock = pPtpPort->clock;

   /* CHECKS */
   /**********/
   /* - the streamData pointer was checked by the parent, 'V2handleRecvMsg'
    * - all other pointers are generated locally or verified at the TASK,
    *   PROTOCOL or HANDLER levels.
    * - the message & packet lengths were verified by 'V2handleRecvMsg'
    * - the alternate master configuration was verified by 'V2handleRecvMsg' */

   /* The targetPortIdentity must be for this clock (or ALL CLOCKS).
    * The search for a matching portNumber is done at lower layers. */
   if (ZL303XX_FALSE == zl303xx_PtpClockIdentityMatchesTarget(
                              pPtpClock->config.defaultDS.clockIdentity,
                              &pMgmtHdrS->targetPortIdentity))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
            "zl303xx_PtpV2MsgMgmtHandle:WARN: targetClock != this clock::", 0,0,0,0,0,0);
      ZL303XX_TRACE_V2_CLOCK_ID(ZL303XX_MOD_ID_PTP_TLV, 1,
                                    "       clockIdentity: ",
                                    pPtpClock->config.defaultDS.clockIdentity);
      ZL303XX_TRACE_V2_PORT_ID(ZL303XX_MOD_ID_PTP_TLV, 1,
                                    "  targetPortIdentity: ",
                                    &pMgmtHdrS->targetPortIdentity);

      /* Eat the entire packet since it isn't meant for this Clock. */
      /* No Error Response is required. */
      totalConsumedMsgBytes = pPtpHdrS->messageLength;
   }

   /* The boundaryHops value indicates how many more re-transmissions are
    * permitted. If this message requires a reply and boundaryHops is already
    * at 0, then we cannot reply. (Technically, we could process the SET and
    * CMD actions and send no reply, but Table 38 of IEEE-1588-2008 states that
    * a reply 'shall' be sent). A RSP or ACK with boundaryHops == 0 is OK since
    * a re-transmit is not required. */
   else if ((pMgmtHdrS->boundaryHops == 0) &&
            ((pMgmtHdrS->actionField == PTP_MANAGEMENT_ACTION_GET) ||
             (pMgmtHdrS->actionField == PTP_MANAGEMENT_ACTION_SET) ||
             (pMgmtHdrS->actionField == PTP_MANAGEMENT_ACTION_CMD)))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
            "zl303xx_PtpV2MsgMgmtHandle:WARN: Unable to Reply to action-%s(%u), boundaryHops == %u:",
            Zl303xx_ManagementActionStr[pMgmtHdrS->actionField], pMgmtHdrS->actionField,
            pMgmtHdrS->boundaryHops, 0,0,0);

      /* Eat the entire packet since it cannot be responded to. */
      totalConsumedMsgBytes = pPtpHdrS->messageLength;
   }

   /* If the PTP Message does not contain any appended TLVs, there is nothing
    * to process. */
   else if (pPtpHdrS->messageLength == ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
            "zl303xx_PtpV2MsgMgmtHandle:WARN: No MGMT TLV appended; messageLength == %u::",
            pPtpHdrS->messageLength, 0,0,0,0,0);

      /* Eat the entire packet since there's nothing we can do. */
      totalConsumedMsgBytes = pPtpHdrS->messageLength;
      zl303xx_IncrementMalformedPktCount(pPtpPort);
   }

   /* Verify a valid Action. */
   else if (pMgmtHdrS->actionField >= PTP_MANAGEMENT_ACTION_MAX)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
            "zl303xx_PtpV2MsgMgmtHandle:ERROR: Invalid actionField == %u::",
            pMgmtHdrS->actionField, 0,0,0,0,0);

      /* Eat the entire packet since it isn't meant for this Clock. */
      totalConsumedMsgBytes = pPtpHdrS->messageLength;
      zl303xx_IncrementMalformedPktCount(pPtpPort);
   }

   /* Verify a valid targetPortIdentity for the specified action. */
   /* RSP and ACK messages must be targeted at the sourcePortIdentity, they
    * cannot specify the ALL_CLOCK or ALL_PORT. */
   else if (((pMgmtHdrS->actionField == PTP_MANAGEMENT_ACTION_RSP) ||
                  (pMgmtHdrS->actionField == PTP_MANAGEMENT_ACTION_ACK)) &&
            (zl303xx_PtpV2IsAllClockIdentity(pMgmtHdrS->targetPortIdentity.clockIdentity) ||
                  (pMgmtHdrS->targetPortIdentity.portNumber == (Uint16T)(0xFFFF))))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
            "zl303xx_PtpV2MsgMgmtHandle:ERROR: Invalid targetPortIdentity for action-%s(%u)",
            Zl303xx_ManagementActionStr[pMgmtHdrS->actionField], pMgmtHdrS->actionField, 0,0,0,0);
      ZL303XX_TRACE_V2_PORT_ID(ZL303XX_MOD_ID_PTP_TLV, 1,
            "    targetPortIdentity:", &pMgmtHdrS->targetPortIdentity);

      /* Eat the entire packet since it cannot be processed. */
      /* DO Not reply to a RSP or ACK */
      totalConsumedMsgBytes = pPtpHdrS->messageLength;
      zl303xx_IncrementMalformedPktCount(pPtpPort);
   }

   /* Continue with processing of the appended TLVs. */
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
            "zl303xx_PtpV2MsgMgmtHandle:Port %u: Received MANAGEMENT %s message.",
            pPtpPort->config.portNumber,
            Zl303xx_ManagementActionStr[pMgmtHdrS->actionField], 0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 2,
            "zl303xx_PtpV2MsgMgmtHandle:Port %u: Received MANAGEMENT %s message.",
            pPtpPort->config.portNumber,
            Zl303xx_ManagementActionStr[pMgmtHdrS->actionField], 0,0,0,0);

      /* At this point, the PTP Header and Management Data checks have passed. */
      totalConsumedMsgBytes = ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN;

      /* Call the next level routine to process any appended TLVs. */
      totalConsumedMsgBytes += zl303xx_PtpV2MgmtTlvHandle(pPtpRxQueueData, pPtpMsgRx, pPtpStream, pPtpPort);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
            "zl303xx_PtpV2MsgMgmtHandle: Total Processed Message bytes(%u)",
            totalConsumedMsgBytes, 0,0,0,0,0);
   }

   return totalConsumedMsgBytes;

}
Uint16T zl303xx_PtpV2handleManagement(zl303xx_PtpClockMsgQueueS *ptpData,
                                      zl303xx_PtpV2MsgS *msg,
                                      zl303xx_ClockStreamS *streamData,
                                      zl303xx_PortDataS *portData)
{
   /* Notes:
    * - the streamData pointer was checked by the parent, 'V2handleRecvMsg'
    * - all other pointers are generated locally or verified at the TASK,
    *   PROTOCOL or HANDLER levels.
    * - the message & packet lengths were verified by 'V2handleRecvMsg'
    * - the alternate master configuration was verified by 'V2handleRecvMsg' */

   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpTxStatusE txStatus = ZL303XX_PTP_TX_OK;

   /* De-reference the management data to make code more readable. */
   zl303xx_PtpV2MsgManagementS *pMgmtHdrS = &msg->data.management;

   /* Response Message Data. */
   zl303xx_PtpV2MsgS msgTx;

   /* Default return value.  */
   Uint16T totalConsumedMsgBytes = 0;

   /* The message should be accepted by this clock if either:
    *    a.) the target clock identity matches the local clock identity or
    *    b.) the target clock identify is all ones in which case the message is
    *        for all clocks in the domain.
    * The port scope is defined as:
    *    a.) the portNumber is all 0's: indicates the internal clock port.
    *    b.) the portNumber is all 1's: indicates all ports within the clock scope.
    *    c.) otherwise: indicates a single port within the clock scope.    */
   zl303xx_PtpV2MgmtMsgScopeE msgScope = zl303xx_PtpV2MgmtMsgEvaluateScope(&pMgmtHdrS->targetPortIdentity);

   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 3,
         "Stream %u: Received PTP_MANAGEMENT event - seqId[%u]: Received Management Message on Port (%u)",
         (streamData == NULL) ? (Uint32T)(-1) : streamData->clockStreamHandle,
         msg->header.sequenceId,
         portData->portDS.portIdentity.portNumber, 0,0,0);

   /* Intercept Management TLVs and execute the new processing first. If this
    * fails (the 'new' method is not yet implemented for the received TLV) then
    * proceed with the old method.
    * Eventually this will become the main Management Message Handler. */
   {
      Uint16T numPktBytesProcessed = zl303xx_PtpV2MsgMgmtHandle(ptpData, msg, streamData, portData);
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 3,
            "MGMT TLV TEST new processing: (%u)",
            numPktBytesProcessed, 0,0,0,0,0);

      if (numPktBytesProcessed > ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN)
      {
         return numPktBytesProcessed;
      }
   }

   if (msgScope != PTPV2_MGMT_SCOPE_NONE)
   {
      Uint16T respMsgLength;
      zl303xx_BooleanE doResponse = ZL303XX_FALSE;

      /* The PTP Header and MGMT Data are unpacked so set the counter of bytes
       * processed to the first octet of the MGMT TLV.  */
      totalConsumedMsgBytes = ZL303XX_PTP_V2_MANAGEMENT_MSG_LEN;

      switch (pMgmtHdrS->actionField)
      {
         case PTP_MANAGEMENT_ACTION_GET :
         case PTP_MANAGEMENT_ACTION_SET :
         case PTP_MANAGEMENT_ACTION_CMD :
         {
            doResponse = ZL303XX_TRUE;

            /* Prepare a response packet */
            respMsgLength = zl303xx_PtpV2MgmtTlvPrepareResponse(
                                 msg, portData, &msgTx,
                                 portData->clock->msgObuf);
            break;
         }

         case PTP_MANAGEMENT_ACTION_RSP :
         {
            /* Ignore any response messages we receive as these should be the response to management messages we have sent out
               which is currently not supported. Note: if we do process response messages we need to step along to the last
               TLV and work backwards through the TLVs because if there is an error this will be indicated AFTER the
               actual response. */
            Uint16T tlvType;
            Uint16T tlvLength = 0;
            Uint16T managementId = 0;

            /* Get the type and managementId fields */
            zl303xx_PtpV2MgmtTlvTypeLengthIdGet(ptpData->msgBuf + totalConsumedMsgBytes,
                  &tlvType, &tlvLength, &managementId);

            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
                  "Management TLV Received - RSP:: (tlvType:%d)(mgmtId:%d)",
                  tlvType, managementId, 0,0,0,0);

            if ((tlvType = 0x01) && (managementId == 0xC401) && (tlvLength >= 2))
            {
               Uint8T statusFieldId, statusFieldType;
               Uint16T statusFieldLength;
               Uint32T statusFieldValue;

               /* TLV Length is the full length minus the managementId field. */
               Uint8T *remainingBuffer = ptpData->msgBuf + totalConsumedMsgBytes + 6;
               tlvLength -= 2;

               /* The magic number takes up 4 bytes. */
               remainingBuffer += 4;
               tlvLength -= 4;

               while (tlvLength > 3)
               {
                  statusFieldValue = 0;
                  statusFieldId = remainingBuffer[0];
                  statusFieldType = remainingBuffer[1];
                  statusFieldLength = remainingBuffer[2];

                  if (statusFieldLength == 1)
                  {
                     statusFieldValue = remainingBuffer[3];
                  }
                  else if (statusFieldLength == 4)
                  {
                     statusFieldValue = UNPACK_32BIT(&remainingBuffer[3]);
                  }

                  tlvLength -= (3 + statusFieldLength);
                  remainingBuffer += (3 + statusFieldLength);

                  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
                        "Field (%d): Type (%d): Length (%d): Value (%lX): remaining (%d)",
                        statusFieldId, statusFieldType, statusFieldLength,
                        statusFieldValue,
                        tlvLength, 0);

               }
            }
            /* Swallow the whole message (for now). */
            totalConsumedMsgBytes = msg->header.messageLength;
            respMsgLength = 0;      /* No response */
            break;
         }

         case PTP_MANAGEMENT_ACTION_ACK :
         {
            /* Ignore any acknowledge messages as these should be the response
             * to any command messages we have sent out */
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 4,
                  "Received MANAGEMENT %s message. Ignored",
                  Zl303xx_ManagementActionStr[pMgmtHdrS->actionField], 0,0,0,0,0);

            /* Swallow the whole message */
            totalConsumedMsgBytes = msg->header.messageLength;
            respMsgLength = 0;      /* No response */
            break;
         }

         default:
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
                  "Unidentified MANAGEMENT ACTION (%u) ignored",
                  pMgmtHdrS->actionField, 0,0,0,0,0);

            totalConsumedMsgBytes = 0; /* Indicate an error to caller */
            respMsgLength = 0;      /* No response */
            break;
         }
      }

      if (doResponse == ZL303XX_TRUE)
      {
         Uint16T respTlvMessageBytes = 0;
         Uint16T consumedTlvBytes;

         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
               "Port %u: Received MANAGEMENT %s message.",
               portData->config.portNumber,
               Zl303xx_ManagementActionStr[pMgmtHdrS->actionField], 0,0,0,0);
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 2,
               "Port %u: Received MANAGEMENT %s message.",
               portData->config.portNumber,
               Zl303xx_ManagementActionStr[pMgmtHdrS->actionField], 0,0,0,0);

         /* Now handle the management TLV suffix */
         consumedTlvBytes = zl303xx_V2HandleManagementTLV(msgScope, msg,
                                                          (ptpData->msgBuf + totalConsumedMsgBytes)   /* Start of the first received TLV */,
                                                          (msg->header.messageLength - totalConsumedMsgBytes),   /* Length of received message
                                                                                             following the management message carrier */
                                                          (portData->clock->msgObuf + respMsgLength), /* Pointer to the start of the output response TLV buffer */
                                                          portData,   /* This is the data structure for the physical port that received the message */
                                                          &respTlvMessageBytes     /* Number of response bytes added */
                                                       );
         if (consumedTlvBytes == 0)
         {
            /* There was an error parsing the input packet */
            totalConsumedMsgBytes = 0;
         }
         else
         {
            if (respTlvMessageBytes > 0)
            {
               /* Update the overall message length */
               respMsgLength += respTlvMessageBytes;
               zl303xx_PtpV2MsgPktLengthSet(respMsgLength, portData->clock->msgObuf);

               /* Send the response message */
               /* Note that streamData could be NULL but only in a unicast system */
               if (!(msg->header.flagField & PTP2_FLAGS_UNICAST))
               {
                  /* Message was received multicast so there must be multicast clock stream configured.
                        Therefore use the clock stream information to determine the multicast destination address */
                  if (streamData == NULL)
                  {
                     ZL303XX_ERROR_NOTIFY("Multicast message received but no multicast stream configured");
                  }
               }

               if (status == ZL303XX_OK)
               {
                  txStatus = zl303xx_PtpTxMsg(portData->clock->msgObuf,
                                            zl303xx_PtpV2MsgPktLengthGet(portData->clock->msgObuf),
                                            ZL303XX_PTP_INTERFACE_GENERAL, portData,
                                            (streamData != NULL) ? NULL : &ptpData->recvMsgInfo.srcAddr,
                                            streamData, ZL303XX_MSG_ID_MANAGEMENT);
               }
               if (txStatus != ZL303XX_PTP_TX_OK)
               {
                  if (streamData != NULL)
                  {
                     zl303xx_PtpPortLogFaultyState(txStatus, ZL303XX_PTP_PORT_FAULT_MGMT_SEND, streamData->portData);
                  } 
                  zl303xx_PtpV2ToPortState(portData, PTP_PORT_INT_STATE_FAULTY);
               }
            }
            totalConsumedMsgBytes += consumedTlvBytes;
         }
      }
      else
      {
         /* No response */
      }
   }
   else
   {
      /* Message not destined for this port. */
      /* Swallow the whole message */
      totalConsumedMsgBytes = msg->header.messageLength;
   }

   return totalConsumedMsgBytes;
}

/**

  Function Name:
    zl303xx_PtpV2ClockIdentityCheck

  Details:
   Checks that the specified clock identity is not all 0's or all 1's.

  Parameters:
   [in]  clockIdentity   Pointer to the clock identity to evaluate.

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE zl303xx_PtpV2ClockIdentityCheck(zl303xx_ClockIdentity clockIdentity)
{
   /* Set a default return value. */
   zlStatusE status = ZL303XX_OK;

   /* Create a test structure for the ClockId portion. */
   zl303xx_ClockIdentity testClock;


   /* Test the ClockId against all 0's first. */
   OS_MEMSET(&testClock, 0x00, PTP_V2_CLK_IDENTITY_LENGTH);
   if (zl303xx_PtpV2EqualClockIdentities(clockIdentity, testClock) == ZL303XX_TRUE)
   {
      ZL303XX_TRACE_ERROR("ClockIdentityCheck:  ALL-ZEROS", 0,0,0,0,0,0);
      status = ZL303XX_PARAMETER_INVALID;
   }
   else
   {
      /* Test for all 1's. */
      OS_MEMSET(&testClock, 0xFF, PTP_V2_CLK_IDENTITY_LENGTH);
      if (zl303xx_PtpV2EqualClockIdentities(clockIdentity, testClock) == ZL303XX_TRUE)
      {
         ZL303XX_TRACE_ERROR("ClockIdentityCheck:  ALL-ONES", 0,0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   return status;
}

/**

  Function Name:
    zl303xx_PtpV2PortIdentityCheck

  Details:
   Checks that the specified port identity:
   - does not have all 0's or all 1's in the clockIdentity field.
   - does not have all 1's in the portNumber field.

  Parameters:
   [in]  portIdentity   Pointer to port identity to evaluate.

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE zl303xx_PtpV2PortIdentityCheck(zl303xx_PortIdentity *portIdentity)
{
   /* Set a default return value. */
   zlStatusE status = ZL303XX_OK;

   /* clockIdentity */
   /*---------------*/
   status = zl303xx_PtpV2ClockIdentityCheck(portIdentity->clockIdentity);

   /* portNumber */
   /*------------*/
   /* Check the portNumber portion of the portIdentity. */
   if (status == ZL303XX_OK)
   {
      if (portIdentity->portNumber == (Uint16T)(0xFFFF))
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

    return status;
}

/* zl303xx_PtpV2MixedModePktHandler */
/** 

   Performs additional processing on packets received on a port in mixed mode:
      - Discard any unicast SYNC packets.
      - Discard any multicast DELAY_REQ packets.
      - If a multicast SYNC is received by a slave, start a unicast stream back
          to the master to send DELAY_REQ.
      - If a unicast DELAY_REQ is received by a master in non-negotiated mode,
          start a unicast stream to send DELAY_RESP back.

  Parameters:
   [in]      ptpMsg    Information about the received PTP packet.
   [in]      pktHdr    The PTP message header unpacked into a structure.
   [in]      port      Pointer to the port this packet was received on.
   [in,out]  ppStream  Pointer to stream this message was received on. NULL
                            if no stream exists, and the new stream pointer is
                            returned.

*******************************************************************************/
static zl303xx_BooleanE zl303xx_PtpV2MixedModePktHandler(zl303xx_PtpClockMsgQueueS *ptpMsg,
                                                       zl303xx_PtpV2MsgHeaderS *pktHdr,
                                                       zl303xx_PortDataS *port,
                                                       zl303xx_ClockStreamS **ppStream)
{
   switch (pktHdr->messageType)
   {
      case ZL303XX_MSG_ID_SYNC :
      {
         /* Do not process any unicast SYNC packets. */
         if (getFlag(pktHdr->flagField, PTP2_FLAGS_UNICAST) == ZL303XX_TRUE)
         {
            return ZL303XX_FALSE;
         }

         /* If this is a non-negotiated slave, start a new unicast stream to send
          * DELAY_REQ messages to the master that this SYNC message was received
          * from. Also, ensure that the multicast stream has been set up to
          * receive packets. */
         if ((port->mcastStream != NULL) &&
             (port->mcastStream->streamSubstate == PTP_STREAM_SUBSTATE_SLAVE) &&
             (*ppStream == NULL))
         {
            zlStatusE status = ZL303XX_OK;
            zl303xx_PtpStreamCreateS streamCreate;

            /* Initialize Default stream values for the configured profile. */
            status = zl303xx_PtpProfileStreamStructInit(port->clock->config.profile, &streamCreate);

            if (status == ZL303XX_OK)
            {
               streamCreate.portHandle = (zl303xx_PtpPortHandleT)port->clockPortHandle;
               streamCreate.unicast = ZL303XX_TRUE;
               streamCreate.mode = ZL303XX_PTP_STREAM_SLAVE_ONLY;
               streamCreate.logMinDelayReqInterval = port->mcastStream->config.logMinDelayReqInterval;
               streamCreate.unicastNegotiationDuration = port->mcastStream->config.unicastNegotiationDuration;
               streamCreate.destAddr = ptpMsg->recvMsgInfo.srcAddr;
               streamCreate.createFlags |= ZL303XX_PTP_STREAM_CREATE_FIRE_EVENT;

               status = zl303xx_PtpStreamCreateInternal(&streamCreate, port,
                                                        ptpMsg->recvMsgInfo.extData,
                                                        ppStream);
            }

            /* Initialize the stream (& place it in an IDLE state). */
            if (status == ZL303XX_OK)
            {
               zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_INIT, *ppStream);
            }
         }
         break;
      }

      case ZL303XX_MSG_ID_DELAY_REQ :
      {
         /* Do not process any multicast DELAY_REQ packets. */
         if (getFlag(pktHdr->flagField, PTP2_FLAGS_UNICAST) == ZL303XX_FALSE)
         {
            return ZL303XX_FALSE;
         }

         /* If this is a non-negotiated unicast master, start a new unicast stream
          * up to send DELAY_RESP messages back to the slave (if one does not
          * already exist). Also, ensure that the multicast stream has been
          * set up to receive packets. */
         if ((port->mcastStream != NULL) &&
             (port->mcastStream->streamSubstate == PTP_STREAM_SUBSTATE_MASTER) &&
             (*ppStream == NULL))
         {
            zlStatusE status = ZL303XX_OK;
            zl303xx_PtpStreamCreateS streamCreate;

            /* Initialize Default stream values for the configured profile. */
            status = zl303xx_PtpProfileStreamStructInit(port->clock->config.profile, &streamCreate);

            if (status == ZL303XX_OK)
            {
               streamCreate.portHandle = (zl303xx_PtpPortHandleT)port->clockPortHandle;
               streamCreate.unicast = ZL303XX_TRUE;
               streamCreate.mode = ZL303XX_PTP_STREAM_MASTER_ONLY;
               streamCreate.unicastNegotiationDuration = port->mcastStream->config.unicastNegotiationDuration;
               streamCreate.destAddr = ptpMsg->recvMsgInfo.srcAddr;
               streamCreate.createFlags |= ZL303XX_PTP_STREAM_CREATE_FIRE_EVENT;

               status = zl303xx_PtpStreamCreateInternal(&streamCreate, port,
                                                        ptpMsg->recvMsgInfo.extData,
                                                        ppStream);
            }

            /* Initialize the stream (& place it in an IDLE state). */
            if (status == ZL303XX_OK)
            {
               zl303xx_PtpV2ToStreamState(PTP_STREAM_SUBSTATE_INIT, *ppStream);
            }
         }
         break;
      }

      default :
         break;
   }

   return ZL303XX_TRUE;
}

/*

  Function Name:
   zl303xx_IncrementMalformedPktCount

  Details:
   Increments the malformed packet count for the specified port instance

  Parameters:
   [in]  portData    The port instance on which the malformed packet was
                           received

  Return Value:
   Nothing

*******************************************************************************/
void zl303xx_IncrementMalformedPktCount(zl303xx_PortDataS *portData)
{
   if ((portData != NULL) && (portData->malformedPktCount < 0xFFFFFFFF))
   {
      portData->malformedPktCount++;
   }
}

/******************************************************************************
   Version independent helper functions
 *******************************************************************************/

/**

  Function Name:
    zl303xx_PtpConvertLog2IntervalToTime

  Details:
   Converts an IEEE-1588 "log2 interval" value to a time value in
   Seconds:Nanoseconds format.

  Parameters:
   [in]  log2Interval   The log2 interval value to convert.

   [out] intervalTime   logInterval converted to an equivalent time value.

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpConvertLog2IntervalToTime(Sint8T log2Interval,
                                          zl303xx_TimeStamp *intervalTime)
{
   intervalTime->secondsField.hi = 0;
   intervalTime->secondsField.lo = 0;
   intervalTime->nanosecondsField = 0;

   /* The highest value supported is +64 since the intervalTime->secondsField is
    * a Uint64S type. Therefore if log2Interval is >=64 then set the highest
    * possible value but log a message.   */
   if (log2Interval >= 64)
   {
      intervalTime->secondsField.hi = 0x80000000;

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
            "zl303xx_PtpConvertLog2IntervalToTime:  Setting MAXIMUM Time interval (1<<63) for log2Interval=%d",
            log2Interval, 0,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TS_PKT_STRM, 2,
            "zl303xx_PtpConvertLog2IntervalToTime:  Setting MAXIMUM Time interval (1<<63) for log2Interval=%d",
            log2Interval, 0,0,0,0,0);
   }
   /* Interval is greater than 1 second so logInterval is a log2 multiplier. */
   else if (log2Interval >= 0)
   {
      intervalTime->secondsField.lo = 1;
      intervalTime->secondsField = LShift_U64S(intervalTime->secondsField, log2Interval);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
            "zl303xx_PtpConvertLog2IntervalToTime:  secondsField=%d",
            intervalTime->secondsField.lo, 0,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TS_PKT_STRM, 3,
            "zl303xx_PtpConvertLog2IntervalToTime:  secondsField=%d",
            intervalTime->secondsField.lo, 0,0,0,0,0);
   }
   else /* log2Interval < 0 */
#ifndef _SUPPORT_SUB_SECOND_SYNC
   {
      /* This is true for PTP V1. V2 does support values less than 1s. */
      ERROR_MSG("logInterval message period < 1s is not supported.  ");
      intervalTime->secondsField.lo = 1;
      intervalTime->nanosecondsField = 0;
   }
#else
   {
      /* The lowest value supported is -29 since 1,000,000,000 shifted left by
       * 29 would give just 1ns in the 32-bit nanosecondsField. In the case,
       * set the lowest possible value and log a message (we don't set 0ns since
       * this would disable any timer).   */
      if (log2Interval <= -30)
      {
         intervalTime->nanosecondsField = 1;
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
               "zl303xx_PtpConvertLog2IntervalToTime:  Setting MINIMUM Time interval (1ns) for log2Interval=%d",
               log2Interval, 0,0,0,0,0);
         ZL303XX_TRACE(ZL303XX_MOD_ID_TS_PKT_STRM, 2,
               "zl303xx_PtpConvertLog2IntervalToTime:  Setting MINIMUM Time interval (1ns) for log2Interval=%d",
               log2Interval, 0,0,0,0,0);
      }
      else /* (-1 to -28) */
      {
         intervalTime->nanosecondsField = TEN_e9 >> (-log2Interval);

         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
               "zl303xx_PtpConvertLog2IntervalToTime:  nanosecondsField=%d",
               intervalTime->nanosecondsField, 0,0,0,0,0);
         ZL303XX_TRACE(ZL303XX_MOD_ID_TS_PKT_STRM, 3,
               "zl303xx_PtpConvertLog2IntervalToTime:  nanosecondsField=%d",
               intervalTime->nanosecondsField, 0,0,0,0,0);
      }
   }
#endif
}

/**

  Function Name:
    zl303xx_PtpGetSyncIntervalTimeout

  Details:
   Converts an IEEE-1588 "log2 SYNC interval" value to a time value in
   Seconds:Nanoseconds format.

  Parameters:
   [in]  streamData  The stream instance data structure of the stream to
                           calculate the SYNC Interval Timeout.

   [out] timeout     The logSyncInterval member of the stream
                           converted to an equivalent time.

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpGetSyncIntervalTimeout(zl303xx_ClockStreamS *streamData,
                                       zl303xx_TimeStamp *timeout)
{
    ZL303XX_CHECK_POINTERS (streamData, streamData->portData);

    if ((streamData->config.unicast == ZL303XX_TRUE) &&
        (streamData->portData->config.unicastNegotiation.enabled == ZL303XX_TRUE))
    {
       zl303xx_PtpConvertLog2IntervalToTime(
          streamData->contractTx[PTP_NEGOTIATED_MSG_ID_SYNC].grant.interval, timeout);
    }
    else
    {
        zl303xx_PtpConvertLog2IntervalToTime(streamData->config.logSyncInterval, timeout);
    }
}

/**

  Function Name:
    zl303xx_PtpGetSyncReceiptTimeout

  Details:
   Converts an IEEE-1588 "log2 SYNC interval" value to a SYNC receipt timeout
   value in Seconds:Nanoseconds format. (Used for V1 Only).

  Parameters:
   [in]  streamData  The stream instance data structure of the stream to
                           calculate the SYNC Receipt Timeout value.

   [out] timeout     log2SyncInterval converted to an equivalent SYNC receipt
                           timeout value (with a minimum).

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpGetSyncReceiptTimeout(zl303xx_ClockStreamS *streamData,
                                      zl303xx_TimeStamp *timeout)
{
   zl303xx_TimeStamp tempTime;

   /* Calculate a single SYNC Interval. */
   zl303xx_PtpGetSyncIntervalTimeout(streamData, &tempTime);

   /* For V1 the minimum SYNC Receipt Timeout is 10 SYNC Intervals. */
   mulTime(timeout, &tempTime, ZL303XX_PTP_DEFAULT_STREAM_SYNC_TIMEOUT);
}

/**

  Function Name:
    zl303xx_PtpGetDelayReqIntervalTimeout

  Details:
   Converts an IEEE-1588 "log2 Delay Request interval" value to a time value in
   Seconds:Nanoseconds format.

  Parameters:
   [in]  streamData  The stream instance data structure of the stream to
                           calculate the Delay Request Interval Timeout.

   [out] timeout     The logMinDelayReqInterval member of the stream
                           converted to an equivalent time.

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpGetDelayReqIntervalTimeout(zl303xx_ClockStreamS *streamData,
                                           zl303xx_TimeStamp *timeout)
{
   ZL303XX_CHECK_POINTERS (streamData, streamData->portData);

   zl303xx_PtpConvertLog2IntervalToTime(
         streamData->msgParams[ZL303XX_MSG_ID_DELAY_REQ].log2intervalAct, timeout);
}

/**

  Function Name:
    zl303xx_PtpGetPeerDelayReqIntervalTimeout

  Details:
   Converts an IEEE-1588 "log2 Peer Delay Request interval" value to a time
   value in Seconds:Nanoseconds format.

  Parameters:
   [in]  streamData  The stream instance data structure of the stream to
                           calculate the Peer Delay Request Interval Timeout.

   [out] timeout     The logMinPeerDelayReqInterval member of the stream
                           converted to an equivalent time.

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpGetPeerDelayReqIntervalTimeout(zl303xx_ClockStreamS *streamData,
                                               zl303xx_TimeStamp *timeout)
{
   ZL303XX_CHECK_POINTERS (streamData, streamData->portData);

   zl303xx_PtpConvertLog2IntervalToTime(
         streamData->msgParams[ZL303XX_MSG_ID_PEER_DELAY_REQ].log2intervalAct, timeout);
}

/**

  Function Name:
    zl303xx_PtpGetAnnounceIntervalTimeout

  Details:
   Converts an IEEE-1588 "log2 ANNOUNCE interval" value to a time value in
   Seconds:Nanoseconds format.

  Parameters:
   [in]  streamData  The stream instance data structure of the stream to
                           calculate the ANNOUNCE Interval Timeout.

   [out] timeout     The logAnnounceInterval member of the stream
                           converted to an equivalent time.

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpGetAnnounceIntervalTimeout(zl303xx_ClockStreamS *streamData,
                                           zl303xx_TimeStamp *timeout)
{
   if (streamData->contractTx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].remainingSec > 0)
   {
      zl303xx_PtpConvertLog2IntervalToTime(
         streamData->contractTx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].grant.interval, timeout);
   }
   else
   {
      zl303xx_PtpConvertLog2IntervalToTime(streamData->config.logAnnounceInterval, timeout);
   }
}

/**

  Function Name:
    zl303xx_PtpGetAnnounceReceiptTimeout

  Details:
   Converts an IEEE-1588 "log2 ANNOUNCE interval" value to an ANNOUNCE receipt
   timeout value in Seconds:Nanoseconds format.

  Parameters:
   [in]  streamData  The stream instance data structure of the stream to
                           calculate the ANNOUNCE Receipt Timeout value.

   [out] timeout     The logAnnounceInterval member converted to an
                           equivalent ANNOUNCE receipt timeout value.

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpGetAnnounceReceiptTimeout(zl303xx_ClockStreamS *streamData,
                                          zl303xx_TimeStamp *timeout)
{
   zl303xx_TimeStamp tempTime = {{0,0}, 0};

   /* The time for a single ANNOUNCE interval. */
   if (streamData->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].remainingSec > 0)
   {
      zl303xx_PtpConvertLog2IntervalToTime(
         streamData->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].grant.interval, &tempTime);
   }
   else
   {
      zl303xx_PtpConvertLog2IntervalToTime(streamData->config.logAnnounceInterval, &tempTime);
   }

   /* Spec section 8.2.5.5.2 specifies a minimum value of 3 */
   if (streamData->config.announceReceiptTimeout < ZL303XX_PTP_DEFAULT_STREAM_ANNC_TIMEOUT)
   {
      streamData->config.announceReceiptTimeout = ZL303XX_PTP_DEFAULT_STREAM_ANNC_TIMEOUT;
   }

   /* Scale by the announceTimeout value */
   mulTime(timeout, &tempTime, streamData->config.announceReceiptTimeout);
}

/*

  Function Name:
    zl303xx_PtpGetQualificationTimeout

  Details:
   Converts an IEEE-1588 "log2 ANNOUNCE(V2) or SYNC(V1) interval" value into a
   qualification expiry time value (Seconds:Nanoseconds).

  Parameters:
   [in]  streamData  The stream instance data structure of the stream to
                           calculate the qualification window timeout value.

   [out] timeout     The applicable ANNOUNCE/SYNC interval converted to an
                           equivalent timeout value.

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpGetQualificationTimeout(zl303xx_ClockStreamS *streamData,
                                        zl303xx_TimeStamp *timeout)
{
   zl303xx_TimeStamp tempTime;

   /* The interval for V1 is based on the SYNC rate.
    * The interval for V2 is based on the ANNOUNCE rate.   */
   if (streamData->portData->portDS.versionNumber == ZL303XX_PTP_VERSION_1)
   {
      /* The time for a single SYNC interval. */
      zl303xx_PtpGetSyncIntervalTimeout(streamData, &tempTime);
   }
   else
   {
      /* The time for a single ANNOUNCE interval. */
      if (streamData->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].remainingSec > 0)
      {
         zl303xx_PtpConvertLog2IntervalToTime(
            streamData->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].grant.interval, &tempTime);
      }
      else
      {
         zl303xx_PtpConvertLog2IntervalToTime(streamData->config.logAnnounceInterval, &tempTime);
      }
   }

   /* Scale by the Foreign Master Qualification Window value */
   mulTime(timeout, &tempTime, ZL303XX_PTP_DEFAULT_FOREIGN_WINDOW_TIMEOUT);
}

/*

  Function Name:
    zl303xx_PtpGetPreMasterTimeout

  Details:
   Determines the required interval that a steam must remain in the PRE_MASTER
   state. According to Clause 9.2.6.10:
   - for M1 (CLASS <= 127) this interval = 0;
   - for M2 (CLASS > 127 but DefaultDS is the BEST) this interval = 0;
   - for M3 (Slave Stream exists but it is not THIS stream)
       interval = (announceInterval * (1 + currentDS.stepsRemoved));

  Parameters:
   [in]  streamData  The stream instance data structure of the stream to
                           calculate the PRE_MASTER hold out interval.

   [out] timeout     The applicable PRE_MASTER interval converted to an
                           equivalent timeout value.

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpGetPreMasterTimeout(zl303xx_ClockStreamS *streamData,
                                    zl303xx_TimeStamp *timeout)
{
   zl303xx_TimeStamp tempTime;
   zl303xx_PtpClockS *pPtpClock = streamData->portData->clock;

   /* M1 or M2 conditions */
   if (zl303xx_PtpClockIsOwnMaster(pPtpClock) == ZL303XX_TRUE)
   {
      OS_MEMSET(timeout, 0x00, sizeof(zl303xx_TimeStamp));
   }

   /* If going to PRE_MASTER, we should not yet be sending ANNOUNCE. */
   else
   {
      zl303xx_PtpConvertLog2IntervalToTime(streamData->config.logAnnounceInterval, &tempTime);

      /* Scale by the Steps Removed of the current Master. */
      mulTime(timeout, &tempTime, pPtpClock->currentDSInt.stepsRemoved);
   }
}

/*

  Function Name:
    zl303xx_PtpGetUncalibratedTimeout

  Details:
   Determines the UNCALIBRATED Timer interval for a stream based on the
   Announce Rate.

  Parameters:
   [in]  streamData  The stream instance data structure of the stream to
                           calculate the UNCALIBRATED Timer interval.

   [out] timeout     The computed UNCALIBRATED Timer interval.

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpGetUncalibratedTimeout(zl303xx_ClockStreamS *streamData,
                                        zl303xx_TimeStamp *timeout)
{
   zl303xx_TimeStamp tempTime;

   /* M1 or M2 conditions */
   if (streamData->config.uncalibrated.anncIntervals == 0)
   {
      OS_MEMSET(timeout, 0x00, sizeof(zl303xx_TimeStamp));
   }

   /* Get the ANNOUNCE rate and multiply. */
   else
   {
      zl303xx_PtpConvertLog2IntervalToTime(streamData->config.logAnnounceInterval, &tempTime);

      /* Scale by the Steps Removed of the current Master. */
      mulTime(timeout, &tempTime, streamData->config.uncalibrated.anncIntervals);
   }
}

/* zl303xx_PtpV2SetRequiredTimers */
/** 

   Starts or stops stream timers based on the configured modes and operating
   state of the stream. These timers will drive the state machine for the given
   stream. Once activated, the timers determine what packets are sent or
   received as well as the in-state operations to execute.

  Parameters:
   [in]   streamData  The stream instance data structure of the stream to
                           set the required timers for.

*******************************************************************************/
void zl303xx_PtpV2SetRequiredTimers(zl303xx_ClockStreamS *stream)
{
   /* This Routine is called only once at the beginning of a state change.
    * Therefore, there should be no contention issues (even with the
    * QUERY_INTERVAL timer). */
   Uint32T timerList = 0, count;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TIMER, 2,
         "zl303xx_PtpV2SetRequiredTimers: Stream %d;",
         stream->clockStreamHandle, 0,0,0,0,0);

   /* The ONE_SECOND TIMER is needed for all Modes. */
   ZL303XX_BIT_SET(timerList, ONE_SEC_TIMER);

   /* All 'active' states require the stream QUALIFICATION timer, even if the
    * stream also contains the selected stream, since multiple masters may
    * multicast to the same stream:
    * - the qualification timer will only qualify/disqualify unselected streams.
    * - the ANNOUNCE Rx Timer monitors the selected entry.  */
   /* If the stream is in the INIT or IDLE state then no timer is required. */
   if ((stream->streamSubstate != PTP_STREAM_SUBSTATE_INIT) &&
       (stream->streamSubstate != PTP_STREAM_SUBSTATE_IDLE))
   {
      ZL303XX_BIT_SET(timerList, QUALIFICATION_TIMER);      /* All 'active' states require the QUALIFICATION timer except PDelay. */
      if ((stream->portData->config.delayMechanism == ZL303XX_DELAY_MECHANISM_P2P) &&
          (stream == stream->portData->pdelayStream))
      {
         /* If Static Unicast, Multicast or Negotiated with time remaining. */
         if ((stream->portData->config.unicastNegotiation.enabled == ZL303XX_FALSE) ||
             (stream->config.unicast == ZL303XX_FALSE) ||
             (stream->contractRx[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].remainingSec > 0))
         {
            ZL303XX_BIT_SET(timerList, PEER_DELAY_REQ_TIMER);
         }
      }

      /* For unicast negotiated streams, the required timers are based on the
       * active RX and TX contracts. */
      else if ((stream->portData->config.unicastNegotiation.enabled == ZL303XX_TRUE) &&
               (stream->config.unicast == ZL303XX_TRUE))
      {
         ZL303XX_BIT_SET(timerList, UNICAST_QUERY_INTERVAL);

         if (stream->contractTx[PTP_NEGOTIATED_MSG_ID_SYNC].remainingSec > 0)
            ZL303XX_BIT_SET(timerList, SYNC_INTERVAL_TIMER);

         if (stream->contractTx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].remainingSec > 0)
            ZL303XX_BIT_SET(timerList, ANNOUNCE_INTERVAL_TIMER);

         if (stream->contractRx[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].remainingSec > 0)
            ZL303XX_BIT_SET(timerList, ANNOUNCE_RECEIPT_TIMER);

         /* Turn on Reverse Path based on Delay Mechanism. */
         if (stream->portData->config.delayMechanism == ZL303XX_DELAY_MECHANISM_E2E)
         {
            if (stream->contractRx[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].remainingSec > 0)
               ZL303XX_BIT_SET(timerList, DELAY_REQ_TIMER);
         }
      }

      /* For non-negotiated streams, the required timers are based on the
       * current stream state. */
      else
      {
         switch (stream->streamSubstate)
         {
            case PTP_STREAM_SUBSTATE_INIT :        /* Only ONE-SECOND (above). */
            case PTP_STREAM_SUBSTATE_IDLE :        /* Only ONE-SECOND (above). */
            case PTP_STREAM_SUBSTATE_PRE_MASTER :  /* ONE-SECOND & QUALIFICATION (above). */
            {
               break;
            }

            case PTP_STREAM_SUBSTATE_PASSIVE :     /* ONE-SECOND & QUALIFICATION (above). */
            {
               if (stream->config.unicastMonitorTiming == ZL303XX_TRUE)
               {
                  if (stream->portData->config.delayMechanism == ZL303XX_DELAY_MECHANISM_E2E)
                  {
                     ZL303XX_BIT_SET(timerList, DELAY_REQ_TIMER);
                  }
               }
               break;
            }

            /* These are the 3 valid SLAVE (quasi-SLAVE) modes. Typically receiving
             * ANNOUNCE & SYNC messages while transmitting DELAY_REQ. However, this
             * may not always be the case...
             *
             *                    !Mixed                                 Mixed
             *             --------------------------------------------------------------------
             *            | ANNC_RX  (ON)                   | ANNC_RX  (ON)                    |
             * Multicast  | DELAY_TX (ON - for SLAVE/UNCAL) | DELAY_TX (OFF - for All 3 modes) |
             *             --------------------------------------------------------------------
             *            | ANNC_RX  (ON)                   | ANNC_RX  (OFF)                   |
             *  Unicast   | DELAY_TX (ON - for SLAVE/UNCAL) | DELAY_TX (ON - for SLAVE/UNCAL)  |
             *             --------------------------------------------------------------------
             */
            case PTP_STREAM_SUBSTATE_UNCALIBRATED :
               {
                  if (stream->config.uncalibrated.anncIntervals > 0)
                  {
                     ZL303XX_BIT_SET(timerList, UNCALIBRATED_TIMER);
                  }

                  /* FALL through to next case */
               }
            case PTP_STREAM_SUBSTATE_SLAVE :
               /* Turn on Reverse Path based on Delay Mechanism. */
               if (stream->portData->config.delayMechanism == ZL303XX_DELAY_MECHANISM_E2E)
               {
                  ZL303XX_BIT_SET(timerList, DELAY_REQ_TIMER);
               }

               /* Fall-through intentional */

            case PTP_STREAM_SUBSTATE_LISTENING :
            {
               /* Easiest is just to set the defaults and shut any OFF based on the
                * stream configuration. */
               ZL303XX_BIT_SET(timerList, ANNOUNCE_RECEIPT_TIMER);

               /* Mixed mode limitations. */
               if (stream->portData->config.mixedMode == ZL303XX_TRUE)
               {
                  if (stream->config.unicast == ZL303XX_TRUE)
                  {
                     ZL303XX_BIT_CLEAR(timerList, ANNOUNCE_RECEIPT_TIMER);
                  }
                  else  /* Mixed-Multicast */
                  {
                     ZL303XX_BIT_CLEAR(timerList, DELAY_REQ_TIMER);
                     ZL303XX_BIT_CLEAR(timerList, PEER_DELAY_REQ_TIMER);
                  }
               }

               /* Bi-Directional limitations. */
               if (stream->portData->config.delayMechanism == ZL303XX_DELAY_MECHANISM_DISABLED)
               {
                  ZL303XX_BIT_CLEAR(timerList, DELAY_REQ_TIMER);
                  ZL303XX_BIT_CLEAR(timerList, PEER_DELAY_REQ_TIMER);
               }

               break;
            }

            case PTP_STREAM_SUBSTATE_MASTER:
            {
               /* Typically transmitting ANNOUNCE & SYNC messages while responding to
                * DELAY_REQ. However, this may not always be the case... */

               /*                   !Mixed            Mixed
                *             -----------------------------------
                *            |  ANNC_TX (ON)    |  ANNC_TX (ON)  |
                * Multicast  |  SYNC_TX (ON)    |  SYNC_TX (ON)  |
                *             -----------------------------------
                *            |  ANNC_TX (ON)    |  ANNC_TX (OFF) |
                *  Unicast   |  SYNC_TX (ON)    |  SYNC_TX (OFF) |
                *             -----------------------------------
                */

               /* Mixed mode limitations. */
               if ((stream->portData->config.mixedMode == ZL303XX_FALSE) ||
                   (stream->config.unicast == ZL303XX_FALSE))
               {
                  ZL303XX_BIT_SET(timerList, SYNC_INTERVAL_TIMER);
                  ZL303XX_BIT_SET(timerList, ANNOUNCE_INTERVAL_TIMER);
               }

               break;
            }

            default:
            {
               ZL303XX_TRACE_ALWAYS("zl303xx_PtpV2GetRequiredTimers: Stream %u: unrecognized state: %d",
                     stream->clockStreamHandle,
                     stream->streamSubstate, 0,0,0,0);
               break;
            }
         }
      }
   }

   /* The only Timer not handle above is the PRE_MASTER Timer which is managed
    * outside this routine. If active then leave it alone.  */
   if (zl303xx_PtpTimerActive(&stream->itimer[PRE_MASTER_TIMER]) == ZL303XX_TRUE)
   {
      ZL303XX_BIT_SET(timerList, PRE_MASTER_TIMER);
   }

   /* Start all timers in timerList. Stop all other timers. */
   for (count = 0; count < ZL303XX_ARRAY_SIZE(stream->itimer); count++)
   {
      if (ZL303XX_BIT_TEST(timerList, count))
      {
         zl303xx_PtpStreamTimerStart(stream, count);
      }
      else
      {
         zl303xx_PtpTimerStop(&stream->itimer[count]);
      }
   }
}  /* END zl303xx_PtpV2SetRequiredTimers */

/*

  Function Name:
    zl303xx_PtpV2MgmtMsgEvaluateScope

  Details:
   Determines the targeted scope of a management (or signaling) message based
   on the targetPortIdentity field. (Refer to Tables 32 & 36 of IEEE-1588-2008).

  Parameters:
   [in]  targetPortIdentity   Pointer to the target port identity.

  Return Value:
   zl303xx_PtpV2MgmtMsgScopeE:
      - PTPV2_MGMT_SCOPE_NONE
      - PTPV2_MGMT_SCOPE_SINGLE_CLOCK
      - PTPV2_MGMT_SCOPE_SINGLE_CLOCK_PORT
      - PTPV2_MGMT_SCOPE_SINGLE_CLOCK_ALL_PORTS
      - PTPV2_MGMT_SCOPE_ALL_CLOCKS
      - PTPV2_MGMT_SCOPE_ALL_CLOCKS_SINGLE_PORT
      - PTPV2_MGMT_SCOPE_ALL_CLOCKS_ALL_PORTS

  Notes:
   Assumes the input pointer has been verified.

*******************************************************************************/
zl303xx_PtpV2MgmtMsgScopeE zl303xx_PtpV2MgmtMsgEvaluateScope(zl303xx_PortIdentity *targetPortIdentity)
{
   /* Default in the case when the portNumber is Invalid (0xFFFE). */
   zl303xx_PtpV2MgmtMsgScopeE targetScope = PTPV2_MGMT_SCOPE_NONE;

   if (targetPortIdentity->portNumber != (Uint16T)(0xFFFE))
   {
      /* Check the CLOCK portion of the Scope. */
      if (ZL303XX_TRUE == zl303xx_PtpV2IsAllClockIdentity(targetPortIdentity->clockIdentity))
      {
         targetScope = PTPV2_MGMT_SCOPE_ALL_CLOCKS;
      }
      else
      {
         targetScope = PTPV2_MGMT_SCOPE_SINGLE_CLOCK;
      }

      /* Set the PORT portion of the Scope. */
      if (targetPortIdentity->portNumber == (Uint16T)(0xFFFF))
      {
         /* Add all Ports to the CLock Scope */
         targetScope |= PTPV2_MGMT_SCOPE_ALL_PORTS;
      }
      else if (targetPortIdentity->portNumber != 0)
      {
         /* Intended for a single Port. */
         targetScope |= PTPV2_MGMT_SCOPE_SINGLE_PORT;
      }

      /* else : portNumber == 0 indicates a Clock only Scope. */
   }

   return targetScope;
}  /* END zl303xx_PtpV2MgmtMsgEvaluateScope */

/*

  Function Name:
   zl303xx_PtpStreamInitGenericCalcValues

  Details:
   Initializes the version independent, dynamic members of the stream data
   structure for the stream provided. These are predominantly related to the
   calculation of clock offsets & path delays.

  Parameters:
      streamData   The PTP clock stream instance structure

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_PtpStreamInitGenericCalcValues(zl303xx_ClockStreamS *streamData)
{
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
         "zl303xx_PtpStreamInitGenericCalcValues: Clock stream %u:",
         streamData->clockStreamHandle, 0,0,0,0,0);

   /* Clear the following Internal Current Data Set values:
    * - stepsRemoved
    * - offset_from_master
    * - mean_path_delay
    * - master_to_slave_delay
    * - slave_to_master_delay */
   OS_MEMSET(&streamData->farEndCurrentDS, 0x00, sizeof(streamData->farEndCurrentDS));

   /* Clear the historic drift values. */
   OS_MEMSET(&streamData->drift, 0x00, sizeof(streamData->drift));
}

/* zl303xx_PtpV2McastIntvlDetect */
/** 

   Detects a message interval change for multicast streams.

  Parameters:
   [in]   pStream  Pointer to an existing stream.
   [in]   type     Received message type.
   [in]   intvl    logMessageInterval from the common header.

*******************************************************************************/
static void zl303xx_PtpV2McastIntvlDetect(zl303xx_ClockStreamS *pStream,
                                          zl303xx_MessageTypeE type,
                                          Sint8T intvl)
{
   if ((pStream->config.unicast == ZL303XX_FALSE) &&
       (pStream->streamSubstate == PTP_STREAM_SUBSTATE_SLAVE))
   {
      zl303xx_PtpStreamIntvlDetect(pStream, type, intvl);
   }
}

/* zl303xx_PtpV2MsgLengthOfType */
/** 

   Get the Minimum Length of a PTP Message (Header + DATA + tlvType (if any)).

  Parameters:
   [in]  messageType   Message type to query the length of.

  Return Value:
   Uint16T  Length of the specified messageType; 0 if undefined.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgLengthOfType(zl303xx_MessageTypeE messageType)
{
   Uint16T msgTypeLength = 0;

   if (messageType < ZL303XX_PTP_V2_NUM_MSG_IDS)
   {
      msgTypeLength = ptpV2PktProcess[messageType].minLength;
   }

   return msgTypeLength;
}

/* zl303xx_PtpMsgAppendEventPadding */
/** 

   Adds padding to the end of a PTP EVENT message if configured. This may be the
   PAD TLV or simply clearing 0s at the end of the message.

  Parameters:
   [in]  messageType       Message type to add padding.
   [in]  padBufferStart    Location to start the padding (first byte after
                                 existing message).

  Return Value:
   Uint16T  Padding added to the specified messageType; 0 if none.

*******************************************************************************/
Uint16T zl303xx_PtpMsgAppendEventPadding(
      zl303xx_ClockStreamS *streamData,
      zl303xx_MessageTypeE messageType,
      Uint16T currMessageLen)
{
   Uint16T ttlTlvLen = 0;
   Uint16T padLength = 0;
   Uint16T ttlBufSpaceRemaining = 0;

   /* Append any Padding to EVENT messages only. */
   if (ZL303XX_OK == ZL303XX_CHECK_EVENT_MSG_TYPE_VALID(messageType))
   {
      /* Some minor error checking that shoulf never be hit. */
      if (currMessageLen >= PTP_MSG_SIZE_MAX)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpMsgAppendEventPadding: currMessageLen(%d) exceeds buffer(%d)",
               currMessageLen, PTP_MSG_SIZE_MAX, 0,0,0,0);
      }
      else
      {
         ttlBufSpaceRemaining = PTP_MSG_SIZE_MAX - currMessageLen;

         /* PAD TLV Configured */
         /* Must have enough room left for 4 TYPE-LENGTH bytes. */
         if ((streamData->config.padTlvLen[messageType] >= ZL303XX_PTP_TLV_BASE_LEN) &&
             (ttlBufSpaceRemaining >= ZL303XX_PTP_TLV_BASE_LEN))
         {
            if (streamData->config.padTlvLen[messageType] <= ttlBufSpaceRemaining)
            {
               ttlTlvLen = streamData->config.padTlvLen[messageType];
            }
            else
            {
               ttlTlvLen = ttlBufSpaceRemaining;
            }

            /* Clear the allowed TLV Space. */
            memset(&streamData->portData->clock->msgObuf[currMessageLen], 0x00, ttlTlvLen);

            /* Set the TYPE & LENGTH Fields. */
            (void)zl303xx_PtpTlvTypeSet(PTP_TLV_PAD,
                        &streamData->portData->clock->msgObuf[currMessageLen]);
            (void)zl303xx_PtpTlvLengthSet(ttlTlvLen - ZL303XX_PTP_TLV_BASE_LEN,
                        &streamData->portData->clock->msgObuf[currMessageLen]);

            /* Update the currMessageLen in case there is zero-padding (next). */
            currMessageLen += ttlTlvLen;
         }

         /* In case a TLV was added. */
         ttlBufSpaceRemaining -= ttlTlvLen;

         /* Zero-Padding is configured */
         if ((streamData->config.msgLenPadding[messageType] > 0) &&
             (ttlBufSpaceRemaining > 0))
         {
            if (streamData->config.msgLenPadding[messageType] <= ttlBufSpaceRemaining)
            {
               padLength = streamData->config.msgLenPadding[messageType];
            }
            else
            {
               padLength = ttlBufSpaceRemaining;
            }

            /* Clear the allowed pad. */
            memset(&streamData->portData->clock->msgObuf[currMessageLen], 0x00, padLength);
         }
      }
   }

   return (ttlTlvLen + padLength);
}  /* END zl303xx_PtpMsgAppendEventPadding */

