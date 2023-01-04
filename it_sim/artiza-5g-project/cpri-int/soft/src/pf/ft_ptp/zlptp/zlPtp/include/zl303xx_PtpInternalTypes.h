

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Implementation specific datatypes and structures.
*
*******************************************************************************/

#ifndef _ZL303XX_PTP_INTERNAL_TYPES_H_
#define _ZL303XX_PTP_INTERNAL_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif


/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Os.h"         /* For MUTEX & other OS definitions. */
#include "zl303xx_List.h"

/*****************   DEFINES   ************************************************/
/* Alarm generation */
#define ZL303XX_PTP_STREAM_ALARM_PERIOD_SEC    8

/* Time Interval Signage definitions */
#define PTP_TIME_INTERVAL_OVERFLOW     (Sint8T)2
#define PTP_TIME_INTERVAL_POSITIVE     (Sint8T)1
#define PTP_TIME_INTERVAL_ZERO         (Sint8T)0
#define PTP_TIME_INTERVAL_NEGATIVE     (Sint8T)(-1)
#define PTP_TIME_INTERVAL_UNDERFLOW    (Sint8T)(-2)

/*****************   DATA TYPES   *********************************************/

/** The possible states of a PTP NODE. */
typedef enum zl303xx_PtpNodeStateE
{
   /** The NODE has not been initialized yet. */
   ZL303XX_PTP_NODE_STATE_UNINIT = 0,

   /** The NODE has been initialized and is in the DISABLED state. When a NODE
       is DISABLED:
        -  No packets are transmitted
        -  Received packets are discarded
        -  PTP state machines are not executed */
   ZL303XX_PTP_NODE_STATE_DISABLED,

   /** The NODE has been initialized and is in the ENABLED state. */
   ZL303XX_PTP_NODE_STATE_ENABLED,

   ZL303XX_PTP_NODE_STATE_NUM_TYPES
} zl303xx_PtpNodeStateE;

/* Internal port state values. Also known as the super-state of the port.
 * This applies to unicast and multicast port. */
typedef enum {
  PTP_PORT_INT_STATE_INIT,
  PTP_PORT_INT_STATE_ACTIVE,
  PTP_PORT_INT_STATE_DISABLED,
  PTP_PORT_INT_STATE_FAULTY
} zl303xx_PtpPortInternalStateE;

/* The internal sub-states of a stream. */
/* These are arranged in order of increasing precedence for mapping the parent
 * port State. For example, if 2 streams exist on the same port with one
 * stream in SLAVE state and the other in MASTER state (Boundary Clock), then
 * the PORT STATE will be evaluated as SLAVE since SLAVE has higher precedence
 * in the list below. */
typedef enum {
  PTP_STREAM_SUBSTATE_INIT,            /* 0 */
  PTP_STREAM_SUBSTATE_IDLE,            /* 1 */
  PTP_STREAM_SUBSTATE_PASSIVE,         /* 2 */
  PTP_STREAM_SUBSTATE_LISTENING,       /* 3 */
  PTP_STREAM_SUBSTATE_PRE_MASTER,      /* 4 */
  PTP_STREAM_SUBSTATE_UNCALIBRATED,    /* 5 */
  PTP_STREAM_SUBSTATE_MASTER,          /* 6 */
  PTP_STREAM_SUBSTATE_SLAVE            /* 7 */
} zl303xx_PtpStreamSubStateE;

/* Values for the type field of a message */
typedef enum zl303xx_PtpTaskMsgE
{
   PTP_TASK_MSG_NONE = 0,
   PTP_TASK_MSG_QUIT,
   PTP_TASK_MSG_TIMER_TICK,
   PTP_TASK_MSG_RECV_PKT,
   PTP_TASK_MSG_TS_RECORD_READY,
   PTP_TASK_MSG_CONTROL_CMD,
   PTP_TASK_MSG_MANUAL_CANCEL,

   PTP_TASK_MSG_NUM_TYPES
} zl303xx_PtpTaskMsgE;

/** Used to indicate the source of the best GM in the network relative to the
 *  local clock. */
typedef enum zl303xx_PtpGmSourceTypeE
{
   /** The Grandmaster source is UNKNOWN (may represent an ERROR). */
   ZL303XX_PTP_GM_SRC_UNKNOWN        = 0,

   /** The Grandmaster is derived from the Clock DefaultDS. */
   ZL303XX_PTP_GM_SRC_LOCAL_CLOCK,

   /** The Grandmaster is recovered from a stream on the local Clock. */
   ZL303XX_PTP_GM_SRC_LOCAL_STREAM,

   /** The Grandmaster is recovered from a virtual port on the local Clock. */
   ZL303XX_PTP_GM_SRC_LOCAL_PORT,

   /** The Grandmaster is recovered from a stream on a different client Clock
    *  that is paired to this local clock (in the case of a distributed client
    *  system).  */
   ZL303XX_PTP_GM_SRC_EXTERNAL
} zl303xx_PtpGmSourceTypeE;

typedef enum
{
   CLOCK_PARAM_INGRESS_CLOCK_CLASS,
   CLOCK_PARAM_EGRESS_CLOCK_CLASS,
   CLOCK_PARAM_EGRESS_QL_UPDATE,

   CLOCK_PARAM_NUM_TYPES
} zl303xx_PtpClockParamE;

/*****************   DATA STRUCTURES   ****************************************/

/*** NODE-SPECIFIC STRUCTURES ***/

/** Implementation defined parameters used to indicate the status of a
 *  PTP NODE. The status can be retrieved by calling zl303xx_PtpNodeStatusGet(). */
typedef struct zl303xx_PtpNodeStatusS
{
   /** The number of PTP clocks that have been created on this NODE. */
   Uint32T numClocks;

   /** The number of PTP ports that have been created across all PTP clocks on
       this NODE. */
   Uint32T numPorts;

   /** The number of PTP connections that have been created across all PTP ports
       on this NODE. */
   Uint32T numStreams;
} zl303xx_PtpNodeStatusS;

/* Parameters used to enable/disable a PTP NODE. */
typedef struct zl303xx_PtpNodeEnableS
{
   /** Set to ZL303XX_TRUE to enable or ZL303XX_FALSE to disable this NODE. */
   zl303xx_BooleanE enable;
} zl303xx_PtpNodeEnableS;

/* The PTP node structure. A node represents the collection of PTP clocks
 * managed by a single CPU. */
typedef struct zl303xx_PtpNodeS
{
   OS_MUTEX_ID          mutexId;

   zl303xx_PtpNodeStateE  state;
   zl303xx_ListS          clockList;
   Uint16T              clockCount;

   Uint8T  manufacturerIdentity[ZL303XX_PTP_NODE_OUI_LEN];
   char    productDescription[ZL303XX_PTP_NODE_PROD_DESC_LEN + 1];
   char    revisionData[ZL303XX_PTP_NODE_REV_DATA_LEN + 1];
   char    userDescription[ZL303XX_PTP_NODE_USER_DESC_LEN + 1];
} zl303xx_PtpNodeS;

/* Task message structure for PTP_TASK_MSG_TIMER_TICK */
typedef struct
{
   zl303xx_PtpTaskMsgE type;
   zl303xx_BooleanE secondFlag;  /* Set to TRUE once per second */
} zl303xx_PtpTimerTickMsgS;

/* Task message structure for PTP_TASK_MSG_TS_RECORD_READY */
typedef struct
{
   zl303xx_PtpTaskMsgE type;
   zl303xx_PtpTsRecordS record;
} zl303xx_PtpTsRecordMsgS;

/* Task message structure for PTP_TASK_MSG_MANUAL_CANCEL */
typedef struct
{
   zl303xx_PtpTaskMsgE type;
   zl303xx_PtpPortHandleT portHandle;
   zl303xx_PtpStreamHandleT streamHandle;
   zl303xx_MessageTypeE msgType;
   zl303xx_PortAddress destAddr;
} zl303xx_PtpManualCancelS;

/* Structure containing additional information about received messages, supplied
 * in the zl303xx_PtpClockMsgQueueS structure when appropriate. */
typedef struct
{
   Uint32T  clockPortHandle;
   zl303xx_PortAddress srcAddr;
   zl303xx_PortAddress destAddr;
   zl303xx_TimeStamp rxTs;
   Uint8T extData[ZL303XX_PTP_RX_MSG_EXT_DATA_LEN];
} recvMsgInfoS;

/* A structure for passing received PTP messages back to the main task */
typedef struct
{
  zl303xx_PtpTaskMsgE  taskMsgId;
  Uint32T clockStreamId;
  Uint8T  msgBuf[PTP_MSG_SIZE_MAX];
  Uint16T pktLength;
  recvMsgInfoS recvMsgInfo;   /* Additional information structure;
                                 valid if taskMsgId is PTP_TASK_MSG_RECV_PKT */
} zl303xx_PtpClockMsgQueueS;

/* Generic PTP message sent to a clock's queue. */
typedef union zl303xx_PtpTaskMsgU
{
   zl303xx_PtpTaskMsgE type;
   zl303xx_PtpTimerTickMsgS timerTick;
   zl303xx_PtpTsRecordMsgS tsRecord;
   zl303xx_PtpClockMsgQueueS clockMsg;
   zl303xx_PtpManualCancelS manualCancel;
} zl303xx_PtpTaskMsgU;

/* A Timestamp Record TYPES and DEFINITIONS.  */
typedef struct zl303xx_PtpTsRecordTblEntryS
{
   Uint8T flags;        /* Current state of the record (determined by bit flags). */
   Uint8T readyMask;    /* What the bit flags of a completed record would look like. */

   zl303xx_MessageTypeE messageType;  /* Needed to pass info to 'complete handler' */
   Uint16T            seqId;        /* sequenceId field of this PTP message */
   zl303xx_PortIdentity srcPortId;    /* Sending sourcePortIdentity. */

   zl303xx_TimeStamp txTs;  /* TX time stamp of initiated message */
   zl303xx_TimeStamp rxTs;  /* RX time of received message */
   Uint64S rxCorrection;  /* correctionField of message */

   zl303xx_TimeStamp rxT2;  /* RX time stamp of PDelay_Req at far end. */
} zl303xx_PtpTsRecordTblEntryS;

/* Timestamp Record flags. */
#define PTP_TS_RECORD_EMPTY     0x00
#define PTP_TS_RECORD_RESERVED  0x01
#define PTP_TS_RECORD_HAVE_TX   0x02
#define PTP_TS_RECORD_HAVE_RX   0x04
#define PTP_TS_RECORD_HAVE_RES  0x08  /* For PEER_DELAY, the far-end resident time is needed. */

/* Information uniquely identifying a clock source */

typedef struct zl303xx_PtpTwoStepRecordS
{
   Uint8T flags;
   Uint16T seqId;

   zl303xx_TimeStamp txTs;  /* preciseOriginTimestamp field from Follow_Up */
   zl303xx_TimeStamp rxTs;  /* <syncEventIngressTimestamp> from Sync */
   Uint64S correction;    /* correctionField from Sync or Follow_Up */
   union
   {
      struct { zl303xx_PortIdentity srcPortId; } v2;
   } u;
} zl303xx_PtpTwoStepRecordS;

/* Internal stream counters used for alarms, etc. */
typedef struct zl303xx_PtpStreamCountersIntS
{
   struct
   {
      Uint32T rx;
   } msg[ZL303XX_PTP_V2_NUM_MSG_IDS];

   Uint16T currSec;
} zl303xx_PtpStreamCountersIntS;


/* Parameters related to management of the optional PATH-TRACE functionality. */
typedef struct
{
   /* Maximum number entries the the current Allocated memory can handle.
    * This will avoid frequent memory FREE & ALLOC calls if resizing needs to
    * take place.   */
   Uint16T listMax;

   /* Number of Clock Identities in the received PATH-TRACE list.  */
   Uint16T listLength;

   /* List of received Clock Identities tracing back to the GM. */
   zl303xx_ClockIdentity *pClockIdList;
} zl303xx_PtpPathTraceListS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
