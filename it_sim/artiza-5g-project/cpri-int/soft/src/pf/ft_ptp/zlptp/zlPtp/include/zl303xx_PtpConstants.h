

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
* Copyright 2006-2018 Microsemi Semiconductor Limited.
* All rights reserved.
*
*  Module Description:
*     The IEEE-1588 PTP Protocol Constants
*
*******************************************************************************/

#ifndef ZL303XX_PTP_CONSTANTS_H
#define ZL303XX_PTP_CONSTANTS_H

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"

/********************** Compile control flags *********************************/

/* Define the following constant if you want outgoing PTP messages to represent all time
   fields using the modification to the TimeRepresentation type which was subsequent
   to IEEE-1588-2002. Otherwise leave it undefined for standard compliant
   behaviour */
#ifndef _EXTERNAL_TRANSMIT_TR_NANOSECONDS_ARE_UNSIGNED32
/* #define _EXTERNAL_TRANSMIT_TR_NANOSECONDS_ARE_UNSIGNED32 */
#endif

/* Define the following constant if incoming PTP messages represent their time
   fields using the modification to the TimeRepresentation type which was subsequent
   to IEEE-1588-2002. Whether this is enabled will depend on remote system behaviour.
   Leave it undefined for standard compliant behaviour.
   This should really be runtime selectable on a per remote host basis */
#ifndef _EXTERNAL_RECEIVE_TR_NANOSECONDS_ARE_UNSIGNED32
/* #define _EXTERNAL_RECEIVE_TR_NANOSECONDS_ARE_UNSIGNED32 */
#endif

/* Define the following constant if you want to use sync periods < 1s (this
 * behaviour is not supported in PTP V1: IEEE-1588-2002). Leave it undefined
 * for standard compliant behaviour. */
#ifndef _SUPPORT_SUB_SECOND_SYNC
/* #define _SUPPORT_SUB_SECOND_SYNC */
#endif

/* Define the following constant if you want to use a reduced period between delay_req cycles
   (this behaviour is not supported in IEEE-1588-2002). Leave it undefined for standard compliant
   behaviour. Only applies to PTP v1 */
#ifndef _DEFAULT_TO_SHORT_DELAY_REQ_CYCLE
   #define _DEFAULT_TO_SHORT_DELAY_REQ_CYCLE
#endif

/* Define the following constant to switch on all Microsemi extensions to IEEE 1588 */
#ifndef _ZL_PTP_EXTENSIONS
   #define _ZL_PTP_EXTENSIONS
#endif

/* For version 2 external nanoseconds are always sign-magnitude format */
#ifndef _EXTERNAL_TRANSMIT_TR_NANOSECONDS_ARE_UNSIGNED32
   #define _EXTERNAL_TRANSMIT_TR_NANOSECONDS_ARE_UNSIGNED32
#endif
#ifndef _EXTERNAL_RECEIVE_TR_NANOSECONDS_ARE_UNSIGNED32
   #define _EXTERNAL_RECEIVE_TR_NANOSECONDS_ARE_UNSIGNED32
#endif
/* End of version 2 specifics */

#ifdef _ZL_PTP_EXTENSIONS
/* Turn on all the Microsemi extensions to IEEE 1588 */
   #ifndef _EXTERNAL_TRANSMIT_TR_NANOSECONDS_ARE_UNSIGNED32
      #define _EXTERNAL_TRANSMIT_TR_NANOSECONDS_ARE_UNSIGNED32
   #endif
   #ifndef _EXTERNAL_RECEIVE_TR_NANOSECONDS_ARE_UNSIGNED32
      #define _EXTERNAL_RECEIVE_TR_NANOSECONDS_ARE_UNSIGNED32
   #endif
   #ifndef _SUPPORT_SUB_SECOND_SYNC
      #define _SUPPORT_SUB_SECOND_SYNC
   #endif

#endif

/* implementation specific constants */

#define DEFAULT_CLOCK_VARIANCE       (-12944)   /* A conservative guestimate of variance. */
#define DEFAULT_CLOCK_STRATUM        3
#define DEFAULT_INBOUND_LATENCY      0       /* in nsec */
#define DEFAULT_OUTBOUND_LATENCY     0       /* in nsec */
#define DEFAULT_CLOCK_CLASS          13      /* Synchronised to an application source of time and not in holdover */
#define DEFAULT_SLAVE_CLOCK_CLASS    248
#define DEFAULT_RESET_CLOCK          ZL303XX_FALSE
#define DEFAULT_MAX_FOREIGN_RECORDS  5
#define DEFAULT_SYNC_BURST_IN_DELAY_REQ   ZL303XX_FALSE /* ZL303XX_TRUE to enable sync burst requests in every delay_req by default */
   #define DEFAULT_USE_STANDARD_DELAY_REQ ZL303XX_TRUE

/* MIN and MAX packet intervals */
#define PTP_MESSAGE_INTERVAL_MIN         -128
#define PTP_MESSAGE_INTERVAL_MAX          127

/* Clocks with CLASS less than 128 operate as Master Only  */
#define PTP_MASTER_ONLY_CLOCK_CLASS_MAX   127

/* SLAVE_ONLY Clocks must have the following Clock Class  */
#define PTP_SLAVE_ONLY_CLOCK_CLASS        255

/* Clock or Stream clockClass override disabled definition.  */
#define PTP_CLOCK_CLASS_OVERRIDE_DISABLED 0

/* Default DOMAIN Range  */
#define PTP_CLOCK_DOMAIN_NUMBER_MIN       (Uint8T)0
#define PTP_CLOCK_DOMAIN_NUMBER_MAX       (Uint8T)127
#define PTP_CLOCK_DOMAIN_NUMBER_DEF       (Uint8T)0

/* Range of Announce Receipt Timeout values */
#define PTP_CLOCK_ANNC_RX_TIMEOUT_MIN     2
#define PTP_CLOCK_ANNC_RX_TIMEOUT_MAX     255
#define PTP_CLOCK_ANNC_RX_TIMEOUT_DEF     3

/* features, only change to reflect changes in implementation */

/* This is the current difference between UTC and TAI times
 * (Refer to IEEE 1588-2008: Annex B).
 * On June 30, 2015: the value became 36.
 * On December 31, 2016: the value became 37.
 */
#define UTC_OFFSET        37

/* V1 Only */
#define VERSION_NETWORK1  1

/* spec defined constants. Note that these are defined as strings which places a NUL terminator on the end
   making the length out by one */
#define DEFAULT_PTP_DOMAIN_NAME      "_DFLT\0\0\0\0\0\0\0\0\0\0\0"
#define ALTERNATE_PTP_DOMAIN1_NAME   "_ALT1\0\0\0\0\0\0\0\0\0\0\0"
#define ALTERNATE_PTP_DOMAIN2_NAME   "_ALT2\0\0\0\0\0\0\0\0\0\0\0"
#define ALTERNATE_PTP_DOMAIN3_NAME   "_ALT3\0\0\0\0\0\0\0\0\0\0\0"


/* ptp constants */

/* The maximum size of a PTP packet received on a stream */
#define PTP_MSG_SIZE_MAX                    1500

/* Clock Identity Type Length (V2 specific) */
#define PTP_V2_CLK_IDENTITY_LENGTH           8

/* Maximum lengths for some CLOCK_DESCRIPTION TLV fields. */
#define ZL303XX_PTP_NODE_PHYSICAL_PROTO_LEN    128
#define ZL303XX_PTP_NODE_PHYSICAL_ADDR_LEN      16
#define ZL303XX_PTP_NODE_OUI_LEN                 3
#define ZL303XX_PTP_NODE_OUI_SUBTYPE_LEN         3
#define ZL303XX_PTP_NODE_PROD_DESC_LEN          64
#define ZL303XX_PTP_NODE_REV_DATA_LEN           32
#define ZL303XX_PTP_NODE_USER_DESC_LEN         128
#define PTP_PROFILE_IDENTITY_LENGTH            6


#define PTP_FOREIGN_MASTER_WINDOW_MIN       4
#define PTP_FOREIGN_MASTER_THRESHOLD        2

#define PTP_DELAY_REQ_INTERVAL              30
#define PTP_RANDOMIZING_SLOTS               18
#define PTP_LOG_VARIANCE_THRESHOLD          256
#define PTP_LOG_VARIANCE_HYSTERESIS         128
#define PTP_V2_LMMI_NULL_VALUE              0x7F   /* Value used to represent a NULL value in the logMessageInterval field. */

/* ptp data enums */
/* Length of various PTP packets */

/* Unicast negotiation contract length range in seconds. */
#define ZL303XX_PTP_UNI_NEG_DURATION_MIN   (Uint32T)(10)
#define ZL303XX_PTP_UNI_NEG_DURATION_MAX   (Uint32T)(1000)
#define ZL303XX_PTP_UNI_NEG_DURATION_DEF   (Uint32T)(300)   /* default */

/* PTP version number definitions. */
typedef enum
{
   ZL303XX_PTP_VERSION_UNKNOWN  = 0,
   ZL303XX_PTP_VERSION_1        = 1,
   ZL303XX_PTP_VERSION_2        = 2,

   ZL303XX_PTP_VERSION_MAX      = ZL303XX_PTP_VERSION_2
} zl303xx_PtpVersionE;

/* Values for the communication technology */
typedef enum {
  PTP_CLOSED,
  PTP_ETHER,
  PTP_RESERVED_1,
  PTP_RESERVED_2,
  PTP_FFBUS,
  PTP_PROFIBUS,
  PTP_LON,
  PTP_DNET,
  PTP_SDS,
  PTP_CONTROLNET,
  PTP_CANOPEN,
  PTP_IEEE1394=243,
  PTP_IEEE802_11A,
  PTP_IEEE_WIRELESS,
  PTP_INFINIBAND,
  PTP_BLUETOOTH,
  PTP_IEEE802_15_1,
  PTP_IEEE1451_2,
  PTP_IEEE1451_5,
  PTP_USB,
  PTP_ISA,
  PTP_PCI,
  PTP_VXI,
  PTP_DEFAULT
} zl303xx_CommunicationIdE;

/* Value of control field in PTP messages (common field for V1 and V2) */
/* Refer to Table 23 of the IEEE-1588-2008 Standard. */
typedef enum {
  PTP_SYNC_MESSAGE         = 0x00,
  PTP_DELAY_REQ_MESSAGE    = 0x01,
  PTP_FOLLOWUP_MESSAGE     = 0x02,
  PTP_DELAY_RESP_MESSAGE   = 0x03,
  PTP_MANAGEMENT_MESSAGE   = 0x04,
  PTP_OTHER_MESSAGE        = 0x05
} zl303xx_PtpMsgControlTypeE;

/* Flag mask bits */
/* version 2 flags */
#define PTP2_FLAGS_LI_61                     (Uint16T)0x0001
#define PTP2_FLAGS_LI_59                     (Uint16T)0x0002
#define PTP2_FLAGS_CURRENT_UTC_OFFSET_VALID  (Uint16T)0x0004
#define PTP2_FLAGS_PTP_TIMESCALE             (Uint16T)0x0008
#define PTP2_FLAGS_TIME_TRACEABLE            (Uint16T)0x0010
#define PTP2_FLAGS_FREQ_TRACEABLE            (Uint16T)0x0020
#define PTP2_FLAGS_SYNCHRO_UNCERTAIN         (Uint16T)0x0040
#define PTP2_FLAGS_ALTERNATE_MASTER          (Uint16T)0x0100
#define PTP2_FLAGS_TWO_STEP                  (Uint16T)0x0200
#define PTP2_FLAGS_UNICAST                   (Uint16T)0x0400
#define PTP2_FLAGS_PROFILE_SPECIFIC_1        (Uint16T)0x1000
#define PTP2_FLAGS_PROFILE_SPECIFIC_2        (Uint16T)0x2000

/* General messages are determined by upper bit of the messageType value.
 * The following definition and MACROs may be of some use. */
#define ZL303XX_PTP_MSG_ID_GENERAL_FLAG     0x8   /* b1000 */

#define ZL303XX_PTP_MSG_ID_IS_GENERAL(msgID)   (((msgID) & ZL303XX_PTP_MSG_ID_GENERAL_FLAG) ? (ZL303XX_TRUE) : (ZL303XX_FALSE))
#define ZL303XX_PTP_MSG_ID_IS_EVENT(msgID)     (!((msgID) & ZL303XX_PTP_MSG_ID_GENERAL_FLAG) ? (ZL303XX_TRUE) : (ZL303XX_FALSE))

/* The highest V2 management message ID number in use */
#define ZL303XX_PTP_V2_NUM_MSG_IDS    (ZL303XX_MSG_ID_MANAGEMENT + 1)
#define ZL303XX_PTP_V2_MAX_EVENT_ID   0x07


/******************************************************************************/
/*******************  MANAGEMENT MESSAGE CONSTANTS  ***************************/
/******************************************************************************/

/* ENUM to define 1588 TLV types (Table 34 of the v2 Standard) */
typedef enum
{
   /* Reserved Value */
   PTP_TLV_RESERVED                          = 0x0000,

   /* Standard TLVs */
   PTP_TLV_MANAGEMENT                        = 0x0001,
   PTP_TLV_MANAGEMENT_ERROR_STATUS           = 0x0002,
   PTP_TLV_VENDOR_EXTENSION                  = 0x0003,

   /* Optional unicast message negotiation TLVs */
   PTP_TLV_REQUEST_UNICAST_TX                = 0x0004,
   PTP_TLV_GRANT_UNICAST_TX                  = 0x0005,
   PTP_TLV_CANCEL_UNICAST_TX                 = 0x0006,
   PTP_TLV_CANCEL_UNICAST_TX_ACK             = 0x0007,

   /* Optional path trace mechanism TLV */
   PTP_TLV_PATH_TRACE                        = 0x0008,

   /* Optional alternate timescale TLV */
   PTP_TLV_ALTERNATE_TIME_OFFSET_INDICATOR   = 0x0009,

   /* IEEE-1588-2017 Optional TLVs */
   PTP_TLV_CUMULATIVE_RATE_RATIO             = 0x000A,
   PTP_TLV_ENHANCED_ACCURACY_METRICS         = 0x000B,
   PTP_TLV_PAD                               = 0x000C,

   /* There are also ranges of numbers for experimental purpose which are
    * ignored in this implementation */
   /* Received messages beyond this value must provide implementation specific
    * handlers.    */
   PTP_TLV_TYPE_MAX
} zl303xx_PtpV2TlvTypeE;

/* ENUM to define Management TLV message IDs (managementId from Table 40 of the
 * V2 Standard) */
typedef enum
{
   /* Management IDs applicable to all node types */
   PTP2_MGMT_ID_NULL                         = 0x0000,
   PTP2_MGMT_ID_CLOCK_DESCRIPTION            = 0x0001,
   PTP2_MGMT_ID_USER_DESCRIPTION             = 0x0002,
   PTP2_MGMT_ID_SAVE_IN_NVS                  = 0x0003,
   PTP2_MGMT_ID_RESET_NVS                    = 0x0004,
   PTP2_MGMT_ID_INITIALIZE                   = 0x0005,
   PTP2_MGMT_ID_FAULT_LOG                    = 0x0006,
   PTP2_MGMT_ID_FAULT_LOG_RESET              = 0x0007,

   /* Management IDs applicable to ordinary and boundary clocks */
   PTP2_MGMT_ID_DEFAULT_DATA_SET             = 0x2000,      /* defaultDS */
   PTP2_MGMT_ID_CURRENT_DATA_SET             = 0x2001,
   PTP2_MGMT_ID_PARENT_DATA_SET              = 0x2002,
   PTP2_MGMT_ID_TIME_PROPERTIES_DATA_SET     = 0x2003,
   PTP2_MGMT_ID_PORT_DATA_SET                = 0x2004,
   PTP2_MGMT_ID_PRIORITY1                    = 0x2005,      /* defaultDS */
   PTP2_MGMT_ID_PRIORITY2                    = 0x2006,      /* defaultDS */
   PTP2_MGMT_ID_DOMAIN                       = 0x2007,      /* defaultDS */
   PTP2_MGMT_ID_SLAVE_ONLY                   = 0x2008,      /* defaultDS */
   PTP2_MGMT_ID_LOG_ANNOUNCE_INTERVAL        = 0x2009,
   PTP2_MGMT_ID_ANNOUNCE_RECEIPT_TIMEOUT     = 0x200A,
   PTP2_MGMT_ID_LOG_SYNC_INTERVAL            = 0x200B,
   PTP2_MGMT_ID_VERSION_NUMBER               = 0x200C,

   PTP2_MGMT_ID_ENABLE_PORT                  = 0x200D,
   PTP2_MGMT_ID_DISABLE_PORT                 = 0x200E,
   PTP2_MGMT_ID_TIME                         = 0x200F,
   PTP2_MGMT_ID_CLOCK_ACCURACY               = 0x2010,      /* defaultDS */
   PTP2_MGMT_ID_UTC_PROPERTIES               = 0x2011,
   PTP2_MGMT_ID_TRACEABILITY_PROPERTIES      = 0x2012,
   PTP2_MGMT_ID_TIMESCALE_PROPERTIES         = 0x2013,
   PTP2_MGMT_ID_UNICAST_NEGOTIATION_ENABLE   = 0x2014,

   PTP2_MGMT_ID_PATH_TRACE_LIST              = 0x2015,
   PTP2_MGMT_ID_PATH_TRACE_ENABLE            = 0x2016,

   PTP2_MGMT_GRANDMASTER_CLUSTER_TABLE       = 0x2017,

   PTP2_MGMT_UNICAST_MASTER_TABLE            = 0x2018,
   PTP2_MGMT_UNICAST_MASTER_MAX_TABLE_SIZE   = 0x2019,

   PTP2_MGMT_ACCEPTABLE_MASTER_TABLE         = 0x201A,
   PTP2_MGMT_ACCEPTABLE_MASTER_TABLE_ENABLED = 0x201B,
   PTP2_MGMT_ACCEPTABLE_MASTER_MAX_TABLE_SIZE = 0x201C,

   PTP2_MGMT_ALTERNATE_MASTER                = 0x201D,

   PTP2_MGMT_ALTERNATE_TIME_OFFSET_ENABLE    = 0x201E,
   PTP2_MGMT_ALTERNATE_TIME_OFFSET_NAME      = 0x201F,
   PTP2_MGMT_ALTERNATE_TIME_OFFSET_MAX_KEY   = 0x2020,
   PTP2_MGMT_ALTERNATE_TIME_OFFSET_PROPERTIES = 0x2021,

   /* Management IDs applicable to transparent clocks */
   PTP2_MGMT_ID_TRANSPARENT_CLOCK_DEFAULT_DATA_SET = 0x4000,
   PTP2_MGMT_ID_TRANSPARENT_CLOCK_PORT_DATA_SET    = 0x4001,
   PTP2_MGMT_ID_PRIMARY_DOMAIN                     = 0x4002,

   /* Management IDs applicable to ordinary, boundary and transparent clocks */
   PTP2_MGMT_ID_DELAY_MECHANISM              = 0x6000,
   PTP2_MGMT_ID_LOG_MIN_PDELAY_REQ_INTERVAL  = 0x6001,

   /* There are also ranges of reserved numbers and those for profile and
    * implementation-specific purposes that are ignored. */
   /* Received messages beyond this value must provide implementation specific
    * handlers.    */
   PTP_MGMT_ID_MAX
} zl303xx_PtpV2MgmtTlvIdE;

/* Management action values */
typedef enum
{
   PTP_MANAGEMENT_ACTION_GET = 0,
   PTP_MANAGEMENT_ACTION_SET = 1,
   PTP_MANAGEMENT_ACTION_RSP = 2,
   PTP_MANAGEMENT_ACTION_CMD = 3,
   PTP_MANAGEMENT_ACTION_ACK = 4
} zl303xx_PtpV2MgmtActionE;
/* The limit of the value in this ENUM. */
#define PTP_MANAGEMENT_ACTION_MAX   (PTP_MANAGEMENT_ACTION_ACK + 1)

/* The scope for handling a Management or Signaling message */
typedef enum
{
   PTPV2_MGMT_SCOPE_NONE                     = 0x0000,
   PTPV2_MGMT_SCOPE_SINGLE_CLOCK             = 0x0100,
   PTPV2_MGMT_SCOPE_SINGLE_CLOCK_PORT        = 0x0101,
   PTPV2_MGMT_SCOPE_SINGLE_CLOCK_ALL_PORTS   = 0x01FF,
   PTPV2_MGMT_SCOPE_ALL_CLOCKS               = 0xFF00,
   PTPV2_MGMT_SCOPE_ALL_CLOCKS_SINGLE_PORT   = 0xFF01,
   PTPV2_MGMT_SCOPE_ALL_CLOCKS_ALL_PORTS     = 0xFFFF,

   /* Extra Masks */
   PTPV2_MGMT_SCOPE_ALL_PORTS                = 0x00FF,
   PTPV2_MGMT_SCOPE_SINGLE_PORT              = 0x0001
} zl303xx_PtpV2MgmtMsgScopeE;

/* Field value for initialization */
#define PTP2_INITIALIZATION_KEY_INIT_EVENT (Uint16T)0x0000

/* enum used by this implementation to identify timers */
typedef enum
{
   SYNC_RECEIPT_TIMER      = 0,  /* Used for version 1 only */
   ANNOUNCE_RECEIPT_TIMER  = 0,  /* Used for version 2 only */
   SYNC_INTERVAL_TIMER     = 1,  /* Both version 1 and version 2 */
   ANNOUNCE_INTERVAL_TIMER = 2,  /* For version 2 only */
   QUALIFICATION_TIMER     = 3,  /* qualifying sync (v1) or announce (v2) messages */
   PRE_MASTER_TIMER        = 4,  /* Pre-Master Qualification Timer (v2) */
   UNCALIBRATED_TIMER      = 5,  /* Uncalibrated Timer (v2) */
   Q_TIMER                 = 6,  /* For version 1 only. Used for delay request timing (as per spec) */
   DELAY_REQ_TIMER         = 6,  /* For version 2 only */
   PEER_DELAY_REQ_TIMER    = 7,  /* For version 2 only */
   SYNC_BURST_DELAY_TIMER  = 8,  /* For version 1 only. Used for sync bursts */
   UNICAST_QUERY_INTERVAL  = 8,  /* version 2 unicast negotiation only */
   ONE_SEC_TIMER           = 9,  /* Both version 1 and version 2. All modes */

   TIMER_ARRAY_SIZE              /* Must be the last entry */
} zl303xx_TimerIdE;

/* For clarification the software timer allocation in each mode is as follows:
For version 1:
   timer #   purpose
    0       SYNC_RECEIPT_TIMER
    1       SYNC_INTERVAL_TIMER
    2       unused
    3       QUALIFICATION_TIMER
    6       Q_TIMER
    8       SYNC_BURST_DELAY_TIMER
    9       ONE_SEC_TIMER
For version 2 multicast:
   timer #   purpose
    0       ANNOUNCE_RECEIPT_TIMER
    1       SYNC_INTERVAL_TIMER
    2       ANNOUNCE_INTERVAL_TIMER
    3       QUALIFICATION_TIMER
    4       PRE_MASTER_TIMER
    5       UNCALIBRATED_TIMER
    6       DELAY_REQ_TIMER
    7       PEER_DELAY_REQ_TIMER
    8       unused
    9       ONE_SEC_TIMER
For version 2 unicast negotiation:
   timer #   purpose
    0       ANNOUNCE_RECEIPT_TIMER
    1       SYNC_INTERVAL_TIMER
    2       ANNOUNCE_INTERVAL_TIMER
    3       unused (reserved for possible QUALIFICATION_TIMER use if required)
    4       PRE_MASTER_TIMER
    5       UNCALIBRATED_TIMER
    6       DELAY_REQ_TIMER
    7       PEER_DELAY_REQ_TIMER
    8       UNICAST_QUERY_INTERVAL
    9       ONE_SEC_TIMER
*/

/* Internal type for maintaining the state of a timestamp record. Determines the meaning of the msgTimeStamp field */
typedef enum
{
   MSG_TS_RECORD_EMPTY     = 0x0,
   MSG_TS_RECORD_SYNC      = 0x1,   /* Timestamp record contains a sync receive timestamp */
   MSG_TS_RECORD_FOLLOW_UP = 0x2    /* Timestamp record contains an accurate origin timestamp from a received followup message */
} zl303xx_MsgTsRecordStateE;


/*******************************************************************************
   Constants and datatypes for unicast contract negotiation
********************************************************************************/

/* V2 Message types that can negotiate unicast contracts. The values of this
   local enum datatype does not match the values in the IEEE 1588 messageType
   enum. This local type makes array access easier. */
typedef enum
{
   PTP_NEGOTIATED_MSG_ID_SYNC = 0,
   PTP_NEGOTIATED_MSG_ID_ANNOUNCE,
   PTP_NEGOTIATED_MSG_ID_DELAY_RESP,
   PTP_NEGOTIATED_MSG_ID_PDELAY_RESP,
   PTP_NEGOTIATED_NUM_MSG_TYPES,          /* Must be the last member of the set */
   PTP_NEGOTIATED_MSG_ID_INVALID =  0xFE  /* PTP INVALID code is often 0xFE. */
}  zl303xx_V2UnicastNegotiatedMsgTypeE;

#endif   /* ZL303XX_PTP_CONSTANTS_H */
