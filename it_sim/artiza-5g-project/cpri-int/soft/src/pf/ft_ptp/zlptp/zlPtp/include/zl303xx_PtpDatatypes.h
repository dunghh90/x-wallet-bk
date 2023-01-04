

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
* Copyright 2006-2018 Microsemi Semiconductor Limited.
* All rights reserved.
*
*  Module Description:
*     Implementation specific and internal IEEE-1588 related datatypes
*
*******************************************************************************/

#ifndef _ZL303XX_PTP_DATATYPES_H_
#define _ZL303XX_PTP_DATATYPES_H_

#include "zl303xx_Global.h"
#include "zl303xx_Os.h"
#include "zl303xx_List.h"

#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpSetup.h"
#include "zl303xx_PtpTimer.h"
#include "zl303xx_PtpV2TlvTypes.h"
#include "zl303xx_PtpInternalTypes.h"
#include "zl303xx_PtpV2MsgTypes.h"
#include "zl303xx_PtpV2TlvCustom.h"
#include "zl303xx_PtpV2UnicastDiscovery.h"
#include "zl303xx_PtpTable.h"
#include "zl303xx_PtpG8265p1Types.h"
#include "zl303xx_Ptp802p1Types.h"

/*****************   DEFINES   ************************************************/
/******************************************************************************/


/***********************************************************************
 * PTP Port Identity Structure (for internal use).
 * This is a union of the various Port Identity Types for generic use.
 ***********************************************************************/
typedef union
{

   /* PTP Version 2. */
   zl303xx_PortIdentity v2;
} zl303xx_PtpPortIdentityS;

/***********************************************************************
 * PTP Stream Identity Structure (for internal use).
 * This is an extension of the Port Identity structure for Streams.
 ***********************************************************************/
typedef union
{
   zl303xx_PtpPortIdentityS portId;
   Uint32T streamNumber;
} zl303xx_PtpStreamIdentityS;

/***********************************************************************
 * PTP Message Header Structure (for internal use).
 * This incorporates both the V1 & V2 message header fields.
 ***********************************************************************/
typedef struct
{
   /* V2: Transport Specific data. */
   Uint4T   transportSpecific;

   /* V1 & V2: Message Type.
    * Used for both V1 & V2 protocol but with different definitions:
    * - V1: Event messages = 1; General Messages = 2. (more like a messageClass).
    * - V2: Actual message type (SYNC/ANNC. etc. ).*/
   Uint4T   messageType;

   /* V1 & V2: PTP Version of the message.
    * For both protocols, this field is located 1 byte from the beginning of
    * the packet & is 4-bits long (lower 4-bits of the byte). */
   Uint4T   versionPTP;

   /* V2: Length (in bytes) of the PTP message. */
   Uint16T  messageLength;

   /* V2: Clock Domain. */
   Uint8T   domainNumber;

   /* The 2-octets of packet header flags. Stored as a Uint16T type but packed
    * and unpacked in network order in the packets.   */
   Uint16T   flagField;

   /* V2: Transparent Clock Only. */
   Uint64S  correctionField;

   /* V1 & V2: Message sequence number.
    * For both protocols, this field is located 30 bytes from the beginning of
    * the packet & is 2 bytes long. */
   Uint16T  sequenceId;

   /* V1 & V2: Control Value for V1 backwards-compatibility.
    * For both protocols, this field is located 32 bytes from the beginning of
    * the packet & is 1 byte long. */
   Uint8T   controlField;

   /* V2: The Message interval is determined by the messageType. */
   Sint8T   logMessageInterval;

   /* Version specific header fields */
   union
   {

      struct
      {  /* Version 2 only header fields */
         Uint8T empty;
      } v2;
   } u;
} zl303xx_MsgHeaderS;

/************************************************************************************/
/* End of Message formats                                                           */
/************************************************************************************/

/* Foreign master record */
typedef struct
{
   /* The Stream information on which this foreign record was received. Also,
    * used to mark records that are not in use by setting this to NULL. */
   struct zl303xx_ClockStreamS *streamData;

   /* Indicates if the record is qualified. Several things determine this state.
    * When disqualified, a BMC cycle may be required. */
   zl303xx_BooleanE valid;

   /* Indicates if the entry was received from an Alternate Master (i.e. a port
    * sending ANNOUNCE but is not in the MASTER state). */
   zl303xx_BooleanE alternateMaster;

   /* The number of SYNC (v1) or Announce (v2) messages received from this
    * master in the previous & current FOREIGN_MASTER_TIME_WINDOWs*/
   struct
   {
      Uint16T previous;
      Uint16T current;
   } msgCount;

   /* Port address of the foreign master. */
   zl303xx_PortAddress portAddress;

   /* Stores all of the parameters needed by the BMC to do a data set comparison
    * to determine the 'best' clock (including the foreignMasterIdentity of the
    * source port). This is also the data set that is forwarded to the next level
    * decision point in a Distributed PTP hierarchy (forwarded either via the
    * Event Mechanism or directly linked to a local clock task). */
   zl303xx_PtpBmcCompareDataS compareData;

   /* PATH TRACE list for this connection. For Unicast, this is updated for each
    * ANNOUNCE message received with the TLV Attached. In Multicast, it is only
    * updated for ANNC from the selected server.  */
   zl303xx_PtpPathTraceListS pathTraceList;

} zl303xx_ForeignMasterRecordS;

/* A record of a timestamp received or transmitted in a message */
typedef struct
{
   Uint8T            syncTsRecordState;
   union
   {
      struct
      {
         zl303xx_PortIdentity sourceIdentity;
         Uint64S            correctionField;
      } v2;
   } u;
   Uint16T           sequenceId;
   zl303xx_TimeStamp   syncTimeStamp;
} zl303xx_SyncTimestampRecordS;


/* Default data set */
typedef struct
{
   /* Version specific parameters */
   union
   {

      struct
      {
         /* No implementation specific V2 data. */
         Uint32T empty6;
      } v2;
   } u;
} zl303xx_DefaultDSInternal;

/* Internal version of the current data set. This structure differs from zl303xx_CurrentDS
 * in that it uses a zl303xx_TimeStamp structure to store offsets instead of a
 * zl303xx_TimeInterval structure. This is done to speed up calculations that occur for
 * every packet received. */
typedef struct
{
   /* PTP CurrentDS properties. */
   Uint16T  stepsRemoved;
   zl303xx_TimeStamp offset_from_master;
   zl303xx_TimeStamp mean_path_delay;

   /* Internal properties of the clock stream */
   zl303xx_TimeStamp master_to_slave_delay;
   zl303xx_TimeStamp slave_to_master_delay;

   /* Offset from master and Mean Path delay are calculated from the 2 values
    * above. However, if the GM Port changes then the calculation will be wrong.
    * Consider: Sync[A], Delay[A] then all valid.
    *           Sync[B] received so that m2s is invalid
    *           Delay[B] received so that s2m is valid
    *           However, MPD and OFM will be invalid since m2s was from a mismatched path.
    *           OFM & MPD are valid only if m2s and s2m are all on the same GM port. */
   zl303xx_BooleanE m2sValid;
   zl303xx_BooleanE s2mValid;

   /* Source of the Delay-Request and Sync messages: used to determine if the
    * offset_from_master calculation is valid. In case of a switch in masters
    * the SYNC may be from one source but the Delay-Request from another.   */
   zl303xx_PortIdentity syncSrcPort;
   zl303xx_PortIdentity delaySrcPort;
} zl303xx_CurrentDSInternal;

/* Parent data set */
typedef struct
{
   /* Version specific parameters */
   union
   {
      zl303xx_ParentDS v2;   /* Version 2 parameters */
   } u;
} zl303xx_ParentDSInternal;

/* Port configuration data set */
typedef struct
{
   /* Version specific data members */
   union
   {

      struct
      {
         /* PTP V2 port specific data members */
         /* Empty right now. */
         Uint32T empty;
      } v2;
   } u;

} zl303xx_PortConfigDataSetS;

typedef struct
{
   Uint16T  maxRecords;          /* Maximum size of the table. */
   Uint16T  numActiveRecords;    /* Current active records in the table. */
   Uint16T  bestRecord;          /* Index of the 'best' entry in the table. */

   /* Flags used for internal control; indicates a change in the table. */
   zl303xx_BooleanE  tableUpdated;

   /* The table of discovered server entries (that have received ANNC messages. */
   zl303xx_ForeignMasterRecordS *foreign;
} zl303xx_ForeignMasterDS;

/* Even though we don't support parent statistics we do actually calculate the
 * phase change (= drift) but to ensure the strict requirement of V1 for the
 * variables to be zero we store it in a separate variable outside the clock
 * parent data set on a per stream bases. */
typedef struct zl303xx_PtpClockDriftS
{
   /* Calculated Drift Value. */
   Sint32T  observedPhaseChangeRateInternal;

   /* Other internal values required for the drift calculation */
   zl303xx_TimeStamp  prevSyncRxTime;
   zl303xx_TimeStamp  latestSyncRxTime;
   zl303xx_TimeStamp  prevOffsetFromMaster;
} zl303xx_PtpClockDriftS;

/* Main clock program data structure. There is one of these structures for
 * every clock on the device. Each Port is 'driven' by a single clock.  */
typedef struct zl303xx_PtpClockS
{
   /* Handle of this clock instance. Should represent the index of each clock
    * instance in a Clock Array (if one exists). */
   zl303xx_PtpClockHandleT clockHandle;

   /* Configuration of the Clock. Elements in this member are set during
    * initialization but some can be changed during operation.    */
   zl303xx_PtpClockCreateS config;

   /* OS members. */
   OS_MUTEX_ID  mutexId;
   OS_MSG_Q_ID  msgQId;
   OS_TASK_ID   taskId;

   /* Standard Data Sets */
   zl303xx_DefaultDS         defaultDS;
   zl303xx_ParentDS          parentDS;
   zl303xx_TimePropertiesDS  timePropertiesDS;
   zl303xx_ForeignMasterDS   foreignMasterDS;

   /* A Union of all PROFILE SPECIFIC learned data related to the clock. */
   union
   {
      /* Variables for Telecom Profile only */
      struct
      {
         zl303xx_BooleanE enableOutput;
         const zl303xx_PtpTelecomQualityLevelS *pQuality;
         zl303xx_PtpTimerS qlHoldOffTimer;
      } telecom;

      /* The G.8275 (Telecom Phase) Profile. */
      zl303xx_PtpG8275ClockDS g8275;

      /* The 802.1AS (TSN) Profile. */
      zl303xx_Ptp802p1ClockDS _802p1;
   } profileDS;

   /* ALTERNATE TIME OFFSET INDICATOR parameters. */
   struct
   {
      zl303xx_BooleanE enabled; /* Unused right now. */

      /* Permitted ALTERNATE TIME OFFSET INDICATOR values. */
      zl303xx_PtpTlvAltTimeOffsetS entryTbl[ZL303XX_PTP_ALT_TIME_OFFSET_TBL_ENTRIES];

      /* Learned ALTERNATE TIME OFFSET INDICATOR values. */
      zl303xx_PtpTlvAltTimeOffsetS parentEntry;
   } altTimeOffset;

   /* Implementation Specific Data Sets. */
   zl303xx_DefaultDSInternal defaultDSInt;  /* Currently only V1 items. */
   zl303xx_CurrentDSInternal currentDSInt;

   /* Variables for managing the UNCALIBRATED state. */
   struct
   {
      Uint8T previousClockClass;
   } uncalibrated;

   /* The following members are used to manage the source of the best PTP Server. */
   /******************************************/
   struct
   {
      /* Internal field (ENUM) indicating where the best GM originates from. */
      zl303xx_PtpGmSourceTypeE location;
      /* Pointer to the current grandmaster clock stream (NULL if no Stream is
       * currently selected (i.e CLOCK, External or Virtual PORT is selected. */
      struct zl303xx_ClockStreamS *pPtpStream;
      /* Pointer to the current grandmaster clock stream (NULL if no PORT
       * is currently selected (i.e CLOCK or External is selected. */
      struct zl303xx_PortDataS *pPtpPort;
      /* When an EXTERNAL ParentDS is provided use this supplied data to build
       * transmitted ANNOUNCE messages.  */
      zl303xx_PtpBmcCompareDataS extComparisonData;
      /* When an EXTERNAL ParentDS is provided use this supplied list when
       * appending a PATH_TRACE TLV to transmitted ANNOUNCE messages.  */
      zl303xx_PtpPathTraceListS extPathTraceList;
   } gmSource;
   /******************************************/

   /* PTP profile in use. */
   const zl303xx_PtpProfileS *profile;

   /* clockType is an array of flags used for the CLOCK_DESCRIPTION.
    * (Valid for V2 only). There is also a clockType in the ClockCreate structure
    * which will over-write this value. However, if the value in the ClockCreate
    * structure is '0' then zl303xx_PtpClockTypeEvaluate() will set this whenever
    * a port is added or removed. */
   Uint16T clockType;


   /* Transmit packet buffer */
   Uint8T msgObuf[PTP_MSG_SIZE_MAX];

   /* Application configurable options.  These are initialized to FALSE when the
     clock is created but will not subsequently be reset so that if an application
     changes them they will be "sticky" */
   /* Variables controlling display of information from this clock. */
   zl303xx_BooleanE  displayOffsetFromMaster;
   zl303xx_BooleanE  utcTimeDisplay;

   zl303xx_BooleanE syncUsesSwTs;    /* TRUE uses inaccurate software timestamps in sync messages (followup message
                                      contains accurate timestamp), FALSE uses hardware timestamp in sync message. */


   Uint32T     random_seed;          /* A seed for use in V1 to generate pseudo-random intervals for delay_request messages */

   zl303xx_ListS  listEntry;   /* Linked list anchor (for node's clockList). */

   zl303xx_ListS  portList;    /* Linked list of ports attached to this clock. */
   Uint16T      portCount;   /* Number of ports attached to this clock. */

   /* Contract parameters for Unicast Negotiation mode (clock level limits)
    * NOTE: Only the maxCount is currently enforced at the clock level */
   zl303xx_PtpContractLimitsS contractLimits[PTP_NEGOTIATED_NUM_MSG_TYPES];

   /* Variables for BMCA */
   struct
   {
      zl303xx_PortAddress selectedAddr;     /* Manually selected address. */
      Uint16T selectedFmtEntry;           /* Selected FMT entry. */
      zl303xx_PtpTimerS updateTimer;        /* Interval for BMCA Updates. */
   } bmca;

   /* Boolean to indicate whether the initial request for Timing service should
    * include all required services (including ANNOUNCE) regardless of the
    * stacked-TLV setting. Managed internally but can can at runtime using API. */
   zl303xx_BooleanE stackAllServicesOn1stRequest;

   /* Used to enable/disable all RX/TX packet processing from this clock */
   zl303xx_BooleanE enableRxTx;

   /* Table of custom TLV Handlers */
   zl303xx_PtpV2TlvCustomS customTlvTable[ZL303XX_PTP_TLV_CUSTOM_HANDLERS_MAX];

   /* Dynamic parameters related to the optional PATH-TRACE functionality. */
   zl303xx_PtpPathTraceListS pathTraceList;

	/* Individual bits are used in BMCA decision logic */
	Uint32T  bmcaFlags;                     

} zl303xx_PtpClockS;

/* Main port data structure. There is one of these structures per port */
typedef struct zl303xx_PortDataS
{
   /* The handle for this clock port. Should represent the index of each port
    * instance in a Port Array (if one exists). */
   zl303xx_PtpPortHandleT clockPortHandle;

   /* The internal Port State for this port. This is the short list (INITIALIZING,
    * ACTIVE, DISABLED, or FAULTY) as opposed to the Standard defined list (that
    * includes SLAVE, MASTER, etc.) which is maintained in portDs:portState. */
   zl303xx_PtpPortInternalStateE portSuperState;
   const char *lastFaultString;

   /* Latest reason for the port to go into faulty state */
   zl303xx_PtpPortFaultTypeE lastFaultType;

   /* Latest Transmit Error */
   zl303xx_PtpTxStatusE lastTxError;

   /* Pointer to the clock this port is attached to. */
   zl303xx_PtpClockS *clock;

   /* The configuration of the port at the time of creation. */
   zl303xx_PtpPortCreateS config;

   /* Structure for data if this is a Virtual PTP Port */
   struct
   {
      zl303xx_BooleanE              isVirtualPort;
      zl303xx_PtpVirtualPortConfigS config;
   } virtualData;

   /*** PTP defined data members ***/
   zl303xx_PortDS portDS;

   /* A Union of all PROFILE SPECIFIC learned data for the port. */
   union
   {
      /** Currently Empty. */
      Uint32T empty;
   } profileDS;

   /* Configuration data for protocols other than V2 */
   zl303xx_PortConfigDataSetS portConfig;

   /* Unicast Master Table used for Unicast Discovery. */
   zl303xx_PtpUmtS unicastMasterTable;

   /* Acceptable master table and filter counters for the port. */
   zl303xx_PtpTableS accMasterTbl;
   Uint32T amtDroppedMsgCount[ZL303XX_PTP_V2_NUM_MSG_IDS];

   /* Acceptable slave table for the port (no filter counters implemented). */
   zl303xx_PtpTableS accSlaveTbl;

   /* Contract parameters for Unicast Negotiation mode (port level limits) */
   zl303xx_PtpContractLimitsS contractLimits[PTP_NEGOTIATED_NUM_MSG_TYPES];
   zl303xx_PtpTableS grantTbl;

   /* Internal data members */
   /* Pointer to the clock stream that corresponds to the port's state machine
      which supports the standard complaint multicast operation */
   struct zl303xx_ClockStreamS *mcastStream;

   /* Pointer to the stream that collects the port's peer-delay data. Only one
    * stream per-port should operate the peer-delay mechanism. */
   struct zl303xx_ClockStreamS *pdelayStream;

   Uint32T malformedPktCount;
   Uint32T failedUnicastRequestCount;

   /* Number of times this port went into faulty state */
   Uint32T faultyStateCount;

   zl303xx_ListS  listEntry;     /* Linked list anchor (for clock's portList). */

   zl303xx_ListS  streamList;    /* Linked list of streams attached to this port. */
   Uint16T      streamCount;   /* Number of streams attached to this port. */

} zl303xx_PortDataS;


typedef struct zl303xx_ClockStreamS
{
   /* The handle for this clock stream. */
   Uint32T clockStreamHandle;

#if defined MUX_PTP_STREAMS_TO_APR_SLOTS
   Uint32T aprSlotHandle;
#endif

   /* The configuration data used to create this stream. */
   zl303xx_PtpStreamCreateS config;

   /* Dynamic states for the stream (internal & external). These are the result
    * of the configuration state of the stream combined with the affects of
    * dynamic changes generated by the system's server selection evaluation. */
   zl303xx_PtpStreamOperatingStateE operState;
   zl303xx_PtpStreamSubStateE       streamSubstate;

   /* Reference back to the port to which this stream is associated.
    * This member in turn contains a reference back to its associated clock. */
   zl303xx_PortDataS *portData;

   /*** Stream Data Set Information ***/
   /* Each stream will collect and maintain information about its own far-end
    * connection in the data sets below.
    * When a stream transmits packets, it uses data from the clock's data sets.
    * Whenever a stream is connected to the 'best' server in the network, the
    * data in the stream's data sets is copied to the clocks's data sets for
    * all other local streams to access. */

   /* Far-End Parent Data Set information: Info from ANNC messages. */
   zl303xx_ParentDSInternal farEndParentDS;
   /* Per stream Current Data Set: Used to calculate offset_from_master &
    * mean_path_delay values per stream. */
   zl303xx_CurrentDSInternal farEndCurrentDS;
   /* Per stream Time Properties. */
   zl303xx_TimePropertiesDS farEndTimePropertiesDS;

   /* A Union of all PROFILE SPECIFIC learned data for the stream. */
   union
   {
      /** Power Profile-specific stream data members. */
      zl303xx_PtpC37p238StreamDS power;

      /* The G.8275.1 (Telecom Phase) Profile. */
      zl303xx_PtpG8275StreamDS g8275;
   } profileDS;

   /* Even though we don't support parent statistics we do actually calculate the
    * phase change (= drift) but to ensure the strict requirement of V1 for the
    * variables to be zero we store it in a separate variable outside the clock
    * parent data set on a per stream bases. */
   zl303xx_PtpClockDriftS drift;

   /* Commonly used message parameters (one for each messageType). */
   struct
   {
      Sint8T            log2intervalCfg;
      Sint8T            log2intervalAct;
      Sint8T            prevIntvl;
      zl303xx_TimeStamp   timeInterval;
      Uint16T           lastSequenceIdRx;
      zl303xx_BooleanE    lastSequenceIdRxInit;
      Uint32T           errPkts;
   } msgParams[ZL303XX_PTP_V2_NUM_MSG_IDS];

   /* Version specific PTP defined data members */
   union
   {

      /* PTP V2 stream specific data members */
      struct
      {
         /* Peer delay parameters */
         zl303xx_TimeInterval peer_mean_path_delay;
         Sint8T log_min_mean_pdelay_req_interval;
      } v2;

   } u;

   /* Timers for this clock stream */
   /* Timers for various tasks including message intervals and receipts */
   /* (see zl303xx_TimerIdE in zl303xx_PtpConstants.h for the timer types). */
   /* The timers required for any stream depends on the stream configuration
    * (Unicast/multicast; bi-directional; master/slave; etc.). */
   zl303xx_PtpTimerS  itimer[TIMER_ARRAY_SIZE];

   /* Unicast contract negotiation structures */
   zl303xx_PtpContractRxS contractRx[PTP_NEGOTIATED_NUM_MSG_TYPES];
   zl303xx_PtpContractTxS contractTx[PTP_NEGOTIATED_NUM_MSG_TYPES];

   /* Dynamic UNICAST Negotiation parameters. */
   struct
   {
      /* Populated when the contract is GRANTED (both server and client). */
      /* Used for the PortIdentity when canceling a contract. */
      zl303xx_PortIdentity farEndPortIdentity;

      /* Flag to indicate if all required services should be included in a
       * contract request.   */
      zl303xx_BooleanE requestAllServices;

      /* Dynamic parameters that track the Best-Effort Unicast Negotiation
       * state of a stream. */
      struct
      {
         /* The number of times the configured rate has been reduced by the
          * requester. After successful negotiation, this value is set back to
          * zero (0) so that on contract renewal, the actual configured value is
          * attempted again.  */
         Uint8T retryCount;
      } bestEffort[ZL303XX_PTP_V2_NUM_MSG_IDS];

      /* Unicast Lockout Status of the entire stream. */
      zl303xx_PtpContractLockoutStatusS lockout;
   } unicast;

   /* If enabled, the remaining seconds before sending a keep-alive Event.   */
   Uint32T keepAliveRemSec;

   /* There are several conditions that prohibit a stream from moving to the
    * SLAVE state from UNCALIBRATED. This flag determines if the Timer has
    * already expired.  */
   zl303xx_BooleanE uncalTimerExpired;

   /* Nominal packet rate and calculated packet delta time used for sequence number clock recovery */
   Uint32T pktRate;
   Uint32T pktDeltaTimeNs;

   /* This is a Dynamically allocated, 4 x numTs Table indexed on:
    * - messageType (EVENT type)
    * - sequenceId % numRecords.  */
   zl303xx_PtpTsRecordTblEntryS *tsRecordArray[ZL303XX_PTP_V2_MSG_NUM_EVENT_TYPES];
   Uint16T tsRecordIdxMask;   /* SequenceId mask used to index into the array. */

   zl303xx_PtpTwoStepRecordS *twoStepRecordArray;
   Uint16T twoStepRecordIdxMask;

   /* Various counters for each message type. */
   zl303xx_PtpStreamCountersS     counters;     /* Visible to applications */
   zl303xx_PtpStreamCountersIntS  countersInt;  /* Internal to PTP */

   /* Flag indicating that a SYNC has been received on this stream therefore
    * allowing a Delay-Request to be issued (if applicable). Each time a SYNC is
    * received, this value is set to TRUE.  */
   /* Set to FALSE by default during structure initialization. */
   zl303xx_BooleanE firstSyncReceived;

   /* Pointer to the Unicast Master Table entry this stream was created from. */
   zl303xx_PtpUmtEntryIntS *pUmtEntry;

   zl303xx_BooleanE qualFmtEntry;  /* Stream has at least one qualified FMT entry */

   zl303xx_ListS listEntry;   /* Linked list anchor (for port's streamList). */

   struct
   {
      Uint32T remainingSec;
      swFuncPtrTSLogging callback;
   } tsLogging;

} zl303xx_ClockStreamS;

#endif
