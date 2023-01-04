

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     PTP API
*
*******************************************************************************/

#ifndef _ZL303XX_PTP_API_TYPES_H_
#define _ZL303XX_PTP_API_TYPES_H_

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_TSDebug.h"

#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpV2MsgTypes.h"

#include "zl303xx_PtpG8265p1Types.h"
#include "zl303xx_PtpC37p238Types.h"
#include "zl303xx_PtpG8275p1Types.h"
#include "zl303xx_Ptp802p1Types.h"

#include "zl303xx_PtpConstants.h"
#include "zl303xx_PtpConstants_dep.h"

#include "zl303xx_PtpBmcaDefaultProfile.h"
#include "zl303xx_PtpBmcaTelecomProfile.h"
#include "zl303xx_PtpBmcaG8275p1Profile.h"

#include "zl303xx_PtpUniNegContractTypes.h"

/*****************   DEFINES   ************************************************/

#define ZL303XX_PTP_MAX_TASK_NAME_LEN 64
#define ZL303XX_PTP_MAX_IFACE_NAME_LEN 21

#define ZL303XX_PTP_RX_MSG_EXT_DATA_LEN    8
#define ZL303XX_PTP_ADDR_STR_LEN_MAX      40

#define ZL303XX_PTP_PPS_UNLIMITED       (Uint32T)(-1)

#define PTP_RESTORE_DEFAULT_DS        (Uint32T)(0xFFFFFFFE)

/*****************   DATA TYPES   *********************************************/

/** Used to configure the operating mode of a clock during initialization.
 *  These are also used in the CLOCK_DESCRIPTION management TLV. */
typedef enum zl303xx_PtpClockTypeBitE
{
   /** The node implements an ordinary clock. */
   ZL303XX_PTP_CLOCK_TYPE_ORDINARY        = (Uint16T)0x0001,

   /** The node implements a boundary clock. */
   ZL303XX_PTP_CLOCK_TYPE_BOUNDARY        = (Uint16T)0x0002,

   /** The node implements a peer-to-peer transparent clock. */
   ZL303XX_PTP_CLOCK_TYPE_P2P_TRANSPARENT = (Uint16T)0x0004,

   /** The node implements an end-to-end transparent clock. */
   ZL303XX_PTP_CLOCK_TYPE_E2E_TRANSPARENT = (Uint16T)0x0008,

   /** The node implements a management node. */
   ZL303XX_PTP_CLOCK_TYPE_MANAGEMENT_NODE = (Uint16T)0x0010,

   ZL303XX_PTP_CLOCK_TYPE_RESERVED        = (Uint16T)0xFFE0
} zl303xx_PtpClockTypeBitE;

/** Interfaces a PTP message can be sent on. */
typedef enum zl303xx_PtpInterfaceE
{
   /** Used to send and receive event messages, which are timestamped based on
    *  the value of the local clock. */
   ZL303XX_PTP_INTERFACE_EVENT = 0,

   /** Used to send and receive general (non-timestamped) messages. */
   ZL303XX_PTP_INTERFACE_GENERAL,

   ZL303XX_PTP_INTERFACE_NUM_TYPES
} zl303xx_PtpInterfaceE;

/** Operating mode of a stream. */
typedef enum zl303xx_PtpStreamModeE
{
   /** PTP uses BMC algorithm to determine if it is a slave or master. The clock
    *  must have zl303xx_DefaultDS::slaveOnly set to ZL303XX_FALSE. */
   ZL303XX_PTP_STREAM_USE_BMC     = 0,

   /** This stream will only ever act as a slave. This value will always be
    *  set if the clock has zl303xx_DefaultDS::slaveOnly set to ZL303XX_TRUE. */
   ZL303XX_PTP_STREAM_SLAVE_ONLY  = 1,

   /** This stream will only ever act as a master. The clock must have
    *  zl303xx_DefaultDS::slaveOnly set to ZL303XX_FALSE. */
   ZL303XX_PTP_STREAM_MASTER_ONLY = 2,
   ZL303XX_PTP_STREAM_NOT_SLAVE = 2,

   ZL303XX_PTP_STREAM_MODE_NUM_TYPES
} zl303xx_PtpStreamModeE;

/** A PTP profile specifies selections of attribute values and optional features
 *  of PTP. */
typedef enum zl303xx_PtpProfileE
{
   /** The default profile used for this implementation. Corresponds to the
    *  profile of IEEE Std. 1588-2008, Annex J.3. */
   ZL303XX_PTP_PROFILE_DEFAULT = 0,

   /** The default profile specified in IEEE Std. 1588-2008, Annex J.3. */
   ZL303XX_PTP_PROFILE_DEFAULT_E2E = 0,
   /** The default profile specified in IEEE Std. 1588-2008, Annex J.4. */
   ZL303XX_PTP_PROFILE_DEFAULT_P2P = 1,

   /** The ITU-T Telecom Profile for Frequency (G.8265.1). */
   ZL303XX_PTP_PROFILE_TELECOM_G8265_1,
   /** The original definition for the ITU-T Telecom Profile for Frequency
    *  (G.8265) kept for backwards compatibility. */
   ZL303XX_PTP_PROFILE_TELECOM = ZL303XX_PTP_PROFILE_TELECOM_G8265_1,
   ZL303XX_PTP_PROFILE_TELECOM_G8265 = ZL303XX_PTP_PROFILE_TELECOM_G8265_1,

   /** The ITU-T Telecom Profile for Phase (G.8275). */
   /** Edition 1 */
   ZL303XX_PTP_PROFILE_TELECOM_G8275_1,
   ZL303XX_PTP_PROFILE_TELECOM_G8275_1v1 = ZL303XX_PTP_PROFILE_TELECOM_G8275_1,
   /** Edition 2 */
   ZL303XX_PTP_PROFILE_TELECOM_G8275_1v2,
   /** G.8275.2 */
   ZL303XX_PTP_PROFILE_TELECOM_G8275_2,

   /** Cable DTI Profile */
   /* TODO: REMOVE - GF11603, GF12710  **/
   ZL303XX_PTP_PROFILE_CABLE_DTI = ZL303XX_PTP_PROFILE_TELECOM_G8275_2,

   /** The IEEE Power Profile (C.37.238 versions 2011 and 2014). */
   ZL303XX_PTP_PROFILE_POWER_C37_238_2011,
   ZL303XX_PTP_PROFILE_POWER_C37_238_2014,

   /** The IEEE TSN Profile (802.1AS Version 2011). */
   ZL303XX_PTP_PROFILE_TSN_802_1_2011,

   ZL303XX_PTP_PROFILE_NUM_TYPES
} zl303xx_PtpProfileE;

/** A list of the events that may need to be handled by an external function. */
typedef enum zl303xx_PtpEventE
{
   ZL303XX_PTP_EVENT_NONE = 0,

   /** Occurs when a clock is configured with ZL303XX_PTP_PROFILE_TELECOM_G8265_1
    *  and a slave stream is not granted the requested contract (rate or
    *  duration) for a message type.
    *     - pEventData must be cast to type (zl303xx_PtpEventContractRejectedS *).   */
   ZL303XX_PTP_EVENT_CONTRACT_REJECTED,

   /** Occurs when a PTP clock is created.
    *     - pEventData must be cast to type (zl303xx_PtpEventClockCreateS *).  */
   ZL303XX_PTP_EVENT_CLOCK_CREATE,

   /** Occurs when a PTP clock is automatically deleted. This event will fire for
    *  every clock when zl303xx_PtpShutdown() is called. Calling zl303xx_PtpClockDelete()
    *  will not trigger this event.
    *     - pEventData must be cast to type (zl303xx_PtpEventClockDeleteS *).  */
   ZL303XX_PTP_EVENT_CLOCK_DELETE,

   /** Occurs when a PTP port is automatically deleted. This event will fire for
    *  every port still attached to a clock when zl303xx_PtpClockDelete() is called.
    *  Calling zl303xx_PtpPortDelete() will not trigger this event.
    *     - pEventData must be cast to type (zl303xx_PtpEventPortDeleteS *).   */
   ZL303XX_PTP_EVENT_PORT_DELETE,

   /** Occurs when a PTP stream is automatically created. This happens when a
    *  unicast negotiation master receives a valid transmission request
    *  signaling message from a slave node. Calling zl303xx_PtpStreamCreate() will
    *  not trigger this event.
    *     - pEventData must be cast to type (zl303xx_PtpEventStreamCreateS *).   */
   ZL303XX_PTP_EVENT_STREAM_CREATE,

   /** Occurs when a PTP stream is automatically deleted. This can happen when a
    *  clock or port is deleted when streams are still associated with it. Calling
    *  zl303xx_PtpStreamDelete() will not trigger this event.
    *     - pEventData must be cast to type (zl303xx_PtpEventStreamDeleteS *).   */
   ZL303XX_PTP_EVENT_STREAM_DELETE,

   /** Occurs when the Operating State of a Stream changes as a result of a
    *  Best Master Clock evaluation.
    *     - pEventData must be cast to type (zl303xx_PtpEventStreamStateChangeS *). */
   ZL303XX_PTP_EVENT_STREAM_STATE_CHANGE,

   /** Occurs when no Stream related Event has been issued for the stream for
    *  an extended period of time.
    *     - pEventData must be cast to type (zl303xx_PtpEventStreamKeepAliveS *). */
   ZL303XX_PTP_EVENT_STREAM_KEEP_ALIVE,

   /** Occurs when there is a change in a clock's ordered list of best servers
    *  (evaluated by BMCA) or at periodic intervals (2 seconds).
    *     - pEventData must be cast to type (zl303xx_PtpEventClockBmcaUpdateS *).   */
   ZL303XX_PTP_EVENT_CLOCK_BMCA_UPDATE,

   /** A transmit/receive time stamp pair is available to be processed by a
    *  clock recovery algorithm.
    *     - pEventData must be cast to type (zl303xx_PtpEventServoDataS *).    */
   ZL303XX_PTP_EVENT_SERVO_DATA,

   /** An alarm generated when the rate of received messages drops below 50% of the
    *  expected rate.
    *     - pEventData must be cast to type (zl303xx_PtpEventCounterAlarmS *).    */
   ZL303XX_PTP_EVENT_COUNTER_ALARM,

   /** Will only be generated when using ZL303XX_PTP_PROFILE_TELECOM. Occurs when
    *  all grandmasters are in failure conditions.
    *     - pEventData must be cast to type (zl303xx_PtpEventSquelchS *).    */
   ZL303XX_PTP_EVENT_SQUELCH,

   /** A message interval change has been detected. This is used to change the
    *  parameters of a clock recovery algorithm.
    *     - pEventData must be cast to type (zl303xx_PtpEventMsgIntvlChangeS *). */
   ZL303XX_PTP_EVENT_MSG_INTVL_CHANGE,

   /** A message indicating that the Parent Data Set of the PTP Clock generating
    *  the event has been changed.
    *     - pEventData must be cast to type (zl303xx_PtpEventParentDsChangeS *).     */
   ZL303XX_PTP_EVENT_PARENT_DATA_SET_CHANGE,

   /** Occurs when a unicast negotiation contract is granted.
    *     - pEventData must be cast to type (zl303xx_PtpEventContractGrantedS *).   */
   ZL303XX_PTP_EVENT_CONTRACT_GRANTED,

   /** Occurs when a unicast negotiation contract expires (does not get renewed).
    *     - pEventData must be cast to type (zl303xx_PtpEventContractExpiredS *).   */
   ZL303XX_PTP_EVENT_CONTRACT_EXPIRED,

   /** Occurs when multiple PEER_DELAY_RESP messages are received to a single
    *  PEER_DELAY_REQ message.
    *     - pEventData must be cast to type (zl303xx_PtpEventMultiplePeerRespS *).   */
   ZL303XX_PTP_EVENT_MULTIPLE_PEER_RESP,

   /** Occurs when a TLV is received that is non-standard and for which no custom
    *  TLV handler has been configured. This can be for a non-standard tlvType,
    *  managementId or unhandled ORGANIZATION_EXTENSION tlvType.
    *     - pEventData must be cast to type (zl303xx_PtpEventUnknownTlvS *).   */
   ZL303XX_PTP_EVENT_UNKNOWN_TLV,

   /** Occurs when there is a change in either of the leap51 or leap61 flags of
    *  the clock's dynamic zl303xx_TimePropertiesDS.
    *     - pEventData must be cast to type (zl303xx_PtpEventLeapSecondsFlagChangeS *).   */
   ZL303XX_PTP_EVENT_LEAP_SECONDS_FLAG_CHANGE,

   /** Occurs when there is a change in UTC Offset value of the clock's dynamic
    *  zl303xx_TimePropertiesDS.
    *     - pEventData must be cast to type (zl303xx_PtpEventUtcOffsetChangeS *).   */
   ZL303XX_PTP_EVENT_UTC_OFFSET_CHANGE,

   /** Occurs when the local node receives an ANNOUNCE message in which the
    *  stepsRemoved field exceeds the local stepsRemoved limit configured.
    *     - pEventData must be cast to type (zl303xx_PtpEventRxStepsRemovedExceededS *).   */
   ZL303XX_PTP_EVENT_RX_STEPS_REMOVED_EXCEEDED,

   /*** DEPRECATED EVENTS ***/
   /**  (Deprecated) This event is no longer generated. Servo status must now be
    *  set directly, from inside the servo code. */
   ZL303XX_PTP_EVENT_SERVO_STATUS_GET,

   /**  (Deprecated) This event is no longer generated. Callback functions for PTSF
    *  flag changes are now registered using zl303xx_PtsfCallbackSet(). */
   ZL303XX_PTP_EVENT_PTSF_CHANGE,

   /** Occurs when a port goes into faulty state, pEventData must
    * be cast to type (zl303xx_PtpEventPortFaultS) */
   ZL303XX_PTP_EVENT_PORT_FAULTY,

   /** Occurs when a previously qualified stream has been
    * disqualified from taking part in the BMCA.
    * - pEventData must be cast to type (zl303xx_PtpEventStreamDisqualifiedS *). */
   ZL303XX_PTP_EVENT_STREAM_DISQUALIFIED,

   ZL303XX_PTP_EVENT_NUM_TYPES
} zl303xx_PtpEventE;

/** Time stamp pair type being passed to the servo algorithm. */
typedef enum zl303xx_PtpTsPairE
{
   /** SYNC pair: TX time is from server, RX time is from client. */
   ZL303XX_PTP_TS_PAIR_SYNC = 0,
   /** DELAY pair: TX time is from client, RX time is from server. */
   ZL303XX_PTP_TS_PAIR_DELAY,
   /** SYNC pair with Peer-Delay Mechanism:
    *             TX time is from server, RX time is from client.
    *             correctionField from the received SYNC packet.
    *             meanPeerDelay from PDelay mechanism for the Port. */
   ZL303XX_PTP_TS_PAIR_SYNC_PEER,

   ZL303XX_PTP_TS_PAIR_NUM_TYPES
} zl303xx_PtpTsPairE;

/** Time stamp pair type being passed to the servo algorithm. */
typedef enum zl303xx_PtpPdelayRespMethodE
{
   /** Peer-Delay Response uses 1-step (with no Follow-Up). The are a number of
    *  implementation specific options for sending the response.
    *  IEEE-1588-2008: Clause 11.4.3.b */
   ZL303XX_PTP_PDELAY_METHOD_1_STEP_FLAG = 0x10,
   ZL303XX_PTP_PDELAY_METHOD_1_STEP_DROP = 0x11,
   ZL303XX_PTP_PDELAY_METHOD_1_STEP_TS   = 0x12,
   ZL303XX_PTP_PDELAY_METHOD_1_STEP_CORR = 0x13,

   /** Peer-Delay Response uses 2-step (Resident time (T3-T2) in the Follow-Up).
    *  IEEE-1588-2008: Clause 11.4.3.c.7 */
   ZL303XX_PTP_PDELAY_METHOD_2_STEP_RESIDENT = 0x01,
   /** Peer-Delay Response uses 2-step (Raw T2 in the Response and raw T3 in
    *  the Follow-Up).
    *  IEEE-1588-2008: Clause 11.4.3.c.8 */
   ZL303XX_PTP_PDELAY_METHOD_2_STEP_RAW = 0x02
} zl303xx_PtpPdelayRespMethodE;

/** List of hardware commands that must be handled by an external porting function. */
typedef enum zl303xx_PtpHwCmdE
{
   /** A command to get the estimated time of a hardware clock. This time is
    *  used to fill the originTimestamp field of general packets and SYNC
    *  packets transmitted from a two-step clock. The returned time must be
    *  accurate to within +/- 1 second or set to all zeros.
    *     - pCmdParams must be cast to type (zl303xx_PtpHwClockTimeGetS *)  */
   ZL303XX_PTP_HW_CMD_CLOCK_TIME_GET = 0,

   /** A command to set the time of a hardware clock. Generally, this function
    *  is called to set the seconds portion of a time-of-day hardware register.
    *     - pCmdParams must be cast to type (zl303xx_PtpHwClockTimeSetS *)  */
   ZL303XX_PTP_HW_CMD_CLOCK_TIME_SET,

   /** A command to gather data when when a GET is requested for the
    *  CLOCK_DESCRIPTION management TLV.
    *     - pCmdParams must be cast to type (zl303xx_PtpHwPhysAddrGetS *)   */
   ZL303XX_PTP_HW_CMD_PHYS_ADDR_GET,

   /** A command to get the hardware clock LOCK status (may be from the actual
    *  hardware or timing algorithm).
    *     - pCmdParams must be cast to type (zl303xx_PtpHwLockStatusGetS *) */
   ZL303XX_PTP_HW_CMD_LOCK_STATUS_GET,

   /** A command to get the hardware clock STABILITY status (may be from the
    *  actual hardware or timing algorithm).
    *     - pCmdParams must be cast to type (zl303xx_PtpHwClockStabilityGetS *) */
   ZL303XX_PTP_HW_CMD_CLOCK_STABILITY_GET,

   /** A command to get information from the hardware clock about whether the
    *  Time has been set (may be from the actual hardware or timing algorithm).
    *     - pCmdParams must be cast to type (zl303xx_PtpHwTimeStatusGetS *)     */
   ZL303XX_PTP_HW_CMD_TIMESET_STATUS_GET,

   /** A command to get information from the hardware clock about the current
    *  operating status of the PLL (may be from the actual hardware or timing
    *  algorithm).
    *     - pCmdParams must be cast to type (zl303xx_PtpHwPllPerformanceGetS *)     */
   ZL303XX_PTP_HW_CMD_PLL_PERF_DATA_GET,

   ZL303XX_PTP_HW_CMD_NUM_TYPES
} zl303xx_PtpHwCmdE;

/** List of triggers used to update a Clock's egress clockClass. */
typedef enum zl303xx_PtpClockEgressQlUpdateE
{
   /** Updates the egress clockClass if the server is qualified (i.e. received
    *  ample Announce messages).  */
   ZL303XX_EGRESS_QL_UPDATE_ON_QUALIFY   = 0,

   /** Updates the egress clockClass if the server is locked by the algorithm.  */
   ZL303XX_EGRESS_QL_UPDATE_ON_LOCK      = 1
} zl303xx_PtpClockEgressQlUpdateE;

/** List of triggers used to upload a Clock's BMCA data as it relates to RX
 *  server information. BMCA data is also uploaded if any local configurations
 *  change. */
typedef enum zl303xx_PtpClockBmcaTriggerE
{
   /** Upload BMCA data every time any Announce message is received (default). */
   ZL303XX_BMCA_TRIGGER_MIN_ANNC_INTVL   = 0,
   /** Upload BMCA data only when Rx ANNC data changes. */
   ZL303XX_BMCA_TRIGGER_ON_RX_ANNC_DELTA,
   /** Upload BMCA data when a Timer expires (to set the timer seconds value
    *  refer to zl303xx_PtpClockBmcaUpdateIntervalSet()). */
   ZL303XX_BMCA_TRIGGER_TIMER,
   /** Disable BMCA updating. */
   ZL303XX_BMCA_TRIGGER_DISABLED
} zl303xx_PtpClockBmcaTriggerE;

/** List of all possible Clock Egress Override values. */
typedef enum zl303xx_PtpClockOverrideEgressE
{
   /** DefaultDS override values. */
   ZL303XX_PTP_CLOCK_OVERRIDE_CLASS = 0,
   ZL303XX_PTP_CLOCK_OVERRIDE_ACCURACY,
   ZL303XX_PTP_CLOCK_OVERRIDE_OFFSET_SCALED_LOG_VARIANCE,
   ZL303XX_PTP_CLOCK_OVERRIDE_PRIORITY1,
   ZL303XX_PTP_CLOCK_OVERRIDE_PRIORITY2,
   ZL303XX_PTP_CLOCK_OVERRIDE_DOMAIN,

   /** TimePropertiesDS override values. */
   ZL303XX_PTP_CLOCK_OVERRIDE_CURRENT_UTC_OFFSET,
   ZL303XX_PTP_CLOCK_OVERRIDE_CURRENT_UTC_OFFSET_VALID,
   ZL303XX_PTP_CLOCK_OVERRIDE_TIME_TRACEABLE,
   ZL303XX_PTP_CLOCK_OVERRIDE_FREQUENCY_TRACEABLE,
   ZL303XX_PTP_CLOCK_OVERRIDE_SYNCHRO_UNCERTAIN,
   ZL303XX_PTP_CLOCK_OVERRIDE_TIME_SOURCE,

   /** CurrentDS override values. */
   ZL303XX_PTP_CLOCK_OVERRIDE_STEPS_REMOVED,

   /** Number of entries for Clock Egress Override. */
   ZL303XX_PTP_CLOCK_OVERRIDE_NUM_ENTRIES
} zl303xx_PtpClockOverrideEgressE;

/** List of all possible Stream Egress Override Announce values. */
typedef enum zl303xx_PtpStreamOverrideAnncEgressE
{
   /** Header override values. */
   ZL303XX_PTP_STREAM_OVERRIDE_TRANSPORT = 0,
   ZL303XX_PTP_STREAM_OVERRIDE_DOMAIN,
   ZL303XX_PTP_STREAM_OVERRIDE_CURRENT_UTC_OFFSET_VALID,
   ZL303XX_PTP_STREAM_OVERRIDE_TIME_TRACEABLE,
   ZL303XX_PTP_STREAM_OVERRIDE_FREQUENCY_TRACEABLE,
   ZL303XX_PTP_STREAM_OVERRIDE_SYNCHRO_UNCERTAIN,
   ZL303XX_PTP_STREAM_OVERRIDE_CORRECTION,
   ZL303XX_PTP_STREAM_OVERRIDE_SRC_PORT_ID,

   /** Announce Data override values. */
   ZL303XX_PTP_STREAM_OVERRIDE_ORIGIN_TIMESTAMP,
   ZL303XX_PTP_STREAM_OVERRIDE_CURRENT_UTC_OFFSET,
   ZL303XX_PTP_STREAM_OVERRIDE_GM_PRIORITY1,
   ZL303XX_PTP_STREAM_OVERRIDE_GM_CLASS,
   ZL303XX_PTP_STREAM_OVERRIDE_GM_ACCURACY,
   ZL303XX_PTP_STREAM_OVERRIDE_GM_VARIANCE,
   ZL303XX_PTP_STREAM_OVERRIDE_GM_PRIORITY2,
   ZL303XX_PTP_STREAM_OVERRIDE_GM_IDENTITY,
   ZL303XX_PTP_STREAM_OVERRIDE_STEPS_REMOVED,
   ZL303XX_PTP_STREAM_OVERRIDE_TIME_SOURCE,

   /** Number of entries for Stream Egress Announce Overrides. */
   ZL303XX_PTP_STREAM_OVERRIDE_NUM_ANNC_ENTRIES
} zl303xx_PtpStreamOverrideAnncEgressE;

/** List of all possible PRTC Connection Levels. */
typedef enum zl303xx_PtpPrtcLevelE
{
   /** Header override values. */
   ZL303XX_PTP_PRTC_NOT_CONNECTED = 0,
   ZL303XX_PTP_PRTC_CONNECTED,
   ZL303XX_PTP_PRTC_ENHANCED,

   /** Number of entries for Stream Egress Announce Overrides. */
   ZL303XX_PTP_PRTC_LEVEL_MAX
} zl303xx_PtpPrtcLevelE;

/** List of all possible reasons PTP port could enter faulty state. */
typedef enum zl303xx_PtpPortFaultTypeE
{
   ZL303XX_PTP_PORT_FAULT_FOLLOW_UP_SEND = 0,
   ZL303XX_PTP_PORT_FAULT_SYNC_SEND,
   ZL303XX_PTP_PORT_FAULT_ANNC_SEND,
   ZL303XX_PTP_PORT_FAULT_DELAY_REQ_SEND,
   ZL303XX_PTP_PORT_FAULT_DELAY_RESP_SEND,
   ZL303XX_PTP_PORT_FAULT_PDELAY_REQ_SEND,
   ZL303XX_PTP_PORT_FAULT_PDELAY_RESP_SEND,
   ZL303XX_PTP_PORT_FAULT_SIGNALLING_SEND,
   ZL303XX_PTP_PORT_FAULT_MGMT_SEND,
   ZL303XX_PTP_PORT_FAULT_USER_ENFORCED,

   /* Total number of fault types */
   ZL303XX_PTP_PORT_FAULT_TYPE_MAX
} zl303xx_PtpPortFaultTypeE;

/** Handle type used to reference a previously created clock. */
typedef Uint32T zl303xx_PtpClockHandleT;
/** Handle type used to reference a previously created port. */
typedef Uint32T zl303xx_PtpPortHandleT;
/** Handle type used to reference a previously created stream. */
typedef Uint32T zl303xx_PtpStreamHandleT;

/** Implementation specific additional data that may be required to
 *  transmit a PTP message. */
typedef struct zl303xx_PtpTxMsgDataS
{
   /** Handle of the port this message is being transmitted from. */
   zl303xx_PtpPortHandleT portHandle;
   /** Handle of the stream this message is associated with. */
   zl303xx_PtpStreamHandleT streamHandle;

   /** Pointer to external clock data. */
   void *clockExtData;
   /** Pointer to external port data. */
   void *portExtData;
   /** Pointer to external stream data. */
   void *streamExtData;

   /** Source port address of this message. */
   zl303xx_PortAddress srcAddr;
   /** Destination port address of this message. */
   zl303xx_PortAddress destAddr;

   /** Interface (EVENT or GENERAL) of this message. */
   zl303xx_PtpInterfaceE interface;
   /** Value of the messageType field in the PTP header. */
   zl303xx_MessageTypeE messageId;
   /** Sequence ID of this message. */
   Uint16T sequenceId;
   /** flagField of this message. */
   Uint16T flagField;

} zl303xx_PtpTxMsgDataS;

#define ZL303XX_PTP_EVENT_CHECK(evt)   (((evt) >= ZL303XX_PTP_EVENT_NUM_TYPES) \
                                         ? (ZL303XX_PARAMETER_INVALID)       \
                                         : (ZL303XX_OK))

typedef void (* zl303xx_PtpEventFnT)(zl303xx_PtpEventE event, void *pEventData);
typedef Sint32T (* zl303xx_PtpHwCmdFnT)(zl303xx_PtpHwCmdE cmdType, void *pCmdParams);

/* Function binding definition for routines used to transmit PTP messages. */
typedef Sint32T (* zl303xx_PtpTxMsgFnT)(Uint8T *buffer, Uint16T bufLen,
                                        zl303xx_PtpTxMsgDataS *msgData);

/** The Acceptable Slave Table is non-standard, but its entries contain the same
 *  data as acceptable masters. */
typedef zl303xx_AcceptableMaster zl303xx_AcceptableSlave;

/*****************   DATA STRUCTURES   ****************************************/

/** Implementation specific parameters required to create a PTP clock. */
typedef struct zl303xx_PtpClockCreateS
{
   /** Task name for this PTP clock. It will be automatically appended with a
    *  unique clock ID and part number. */
   char taskName[24];

   /** Priority of the clock task. */
   Sint32T taskPriority;

   /** Stack size of the clock task. */
   Sint32T taskStackSize;

   /** Length of the message queue used by the clock. */
   Sint32T msgQLength;

   /** Value that will be placed in the clockType field of a CLOCK_DESCRIPTION
    *  management TLV. Set to 0 to have this parameter automatically filled
    *  based on the number of ports attached to the clock. */
   Uint16T clockType;

   /** The PTP profile that will be used on this clock. */
   zl303xx_PtpProfileE profile;

   /** Strictly adhere to parameter ranges specified in the profile. */
   zl303xx_BooleanE profileStrict;

   /** Default Data Set values of the local Clock. This structure is copied to
    *  the clock's zl303xx_DefaultDS::defaultDS field. This field should be
    *  updated whenever the local hardware clock (i.e. electrical reference)
    *  changes or the local configuration changes. */
   zl303xx_DefaultDS defaultDS;

   /** A structure for configuration of optional Default Data Set features
    *  related to the local Clock. */
   zl303xx_OptionalDefaultDS optionalDefaultDS;

   /** Time Property values of the local Clock. This structure is copied to the
    *  clock's zl303xx_TimePropertiesDS::timePropertiesDS field when the clock is
    *  operating in grandmaster mode. This field should be updated whenever the
    *  local hardware clock (i.e. electrical reference) changes. */
   zl303xx_TimePropertiesDS localTimeProperties;

   /* A Union of all PROFILE SPECIFIC configuration data for the clock. */
   union
   {
      /* Unused at the moment: in order to maintain backwards compatibility the
       * G.8265 data has to stay outside the union. Eventually when we pass a
       * major release transition it can be moved inside. */
      /** Telecom Profile-specific members. */
      /* zl303xx_PtpTelecomClockCreateS telecom; */

      /** Power Profile-specific clock creation members. */
      zl303xx_PtpC37p238ClockConfigS power;

      /** Telecom Phase Profile-specific clock creation members. */
      zl303xx_PtpG8275ClockConfigS g8275p1;

      /* TSN Profile specific clock creation parameters. */
      zl303xx_Ptp802p1ClockConfigS _802p1;
   } profileCfg;

   /** Telecom Profile-specific members. */
   zl303xx_PtpTelecomClockCreateS telecom;

   /** Static value: cannot be changed during runtime.
    *  The maximum number of foreign records that can be handled by this Clock.
    *  This should be determined by the number of remote master clocks that are
    *  expected to be discovered at any time on all associated Ports. This value
    *  must be at least 5 (see IEEE-1588-2008 section 9.3.2.4.5) */
   Uint16T maxForeignRecords;

   /* BMCA configuration. */
   struct
   {
      /* BMCA upload trigger. */
      zl303xx_PtpClockBmcaTriggerE updateTrigger;
      /* BMCA upload interval in seconds (generates the BMCA Update Event) */
      Uint32T updateEventSec;
      /* Applicable to Telecom Profile only. */
      zl303xx_BooleanE revertiveEn;
   } bmca;

   /** Parameters that determine how a clock issues its Class information while
    *  in the UNCALIBRATED state (i.e. Issue the new class immediately or wait
    *  until LOCK is achieved. */
   struct
   {
      /** Flag indicating if the clock will advertise the parentDS clockClass
       *  or the previously active clockClass while in the UNCALIBRATED state. */
      zl303xx_BooleanE usePreviousClass;
   } uncalibrated;

   /** Function to be called when a PTP event (see #zl303xx_PtpEventE) occurs.
    *  This binding is not required (may be NULL). */
   zl303xx_PtpEventFnT eventCallback;

   /** Function to be called when PTP needs to query or set some information
    *  in the hardware. This binding is required (must NOT be NULL). */
   zl303xx_PtpHwCmdFnT hwCmdFn;

   /** The clockClass override values assigned to this clock. When set to a
    *  non-zero value, this overrides the value of the clock's defaultDS or
    *  ParentDS as long as the associated Stream override values are disabled.
    *  This defaults to disabled (0) for normal operation. */
   Uint8T ingressClockClass;
   Uint8T egressClockClass;

   /* The group of all Clock parameters that can be over-written on egress. */
   struct
   {
      /* Boolean flag indicating if an Override is configured. */
      zl303xx_BooleanE enabled[ZL303XX_PTP_CLOCK_OVERRIDE_NUM_ENTRIES];

      /* Override parameters (not all fields are used). */
      zl303xx_DefaultDS defaultDS;
      zl303xx_TimePropertiesDS timePropertiesDS;
      zl303xx_CurrentDS currentDS;
   } override;

   /** Sets the trigger to use to update the Clock's egress class. Until the
    *  indicated event occurs, the Clock's Default Class (QL) is used. */
   zl303xx_PtpClockEgressQlUpdateE egressClassUpdateTrigger;

   /** Unicast negotiation parameters. */
   struct
   {
      /** The maximum number of ANNOUNCE contracts supported.*/
      Uint32T maxAnnounceCount;

      /** The maximum number of SYNC contracts supported.*/
      Uint32T maxSyncCount;

      /** The maximum number of DELAY_RESP contracts supported.*/
      Uint32T maxDelayRespCount;

      /** The maximum number of PEER_DELAY_RESP contracts supported.*/
      Uint32T maxPdelayRespCount;

      /** The combined Maximum PPS of all contracts of a single message type
       *  allowed on this Clock.   */
      Uint32T totalPpsMax[ZL303XX_PTP_V2_NUM_MSG_IDS];
   } unicastNegotiation;

   /** Pointer to some external user data. Memory management must be handled
    *  externally to PTP. */
   void *extData;

   /** Request clock to be created with a specific handle. Set to ZL303XX_PTP_INVALID
    *  to auto-generate a handle. */
   zl303xx_PtpClockHandleT requestedHandle;

   /** Flag indicating whether the Clock will automatically update the state of
    *  each local stream when a ParentDS update occurs or if the Clock will operate
    *  in a manual mode. */
   zl303xx_BooleanE autoUpdateStreamStates;

   /* Configuration parameters for to the optional PATH-TRACE functionality. */
   struct
   {
      /** Flag indicating whether the Clock has enabled the optional PATH_TRACE
       *  functionality. Default = FALSE */
      zl303xx_BooleanE enabled;
   } pathTrace;

   /* Configuration parameters for to the optional Synchronization-Uncertain
    * functionality. */
   struct
   {
      /** Flag indicating whether the Clock has enabled the optional
       * Synchronization-Uncertain functionality.
       * Default = FALSE */
      zl303xx_BooleanE enabled;
   } synchronizationUncertain;

} zl303xx_PtpClockCreateS;

/** Implementation specific parameters required to delete a PTP clock.
 *  For future expansion -- it does not currently contain any members. */
typedef struct zl303xx_PtpClockDeleteS
{
   /** Set to ZL303XX_FALSE to prevent deleting a clock that still has ports
    *  attached to it. */
   zl303xx_BooleanE forceDelete;
} zl303xx_PtpClockDeleteS;


/** Valid values for numRateRatioPkts attribute of zl303xx_PtpPortCreateS structure */
typedef enum {
ZL303XX_PTP_NUM_RATE_RATIO_1  =  1,
ZL303XX_PTP_NUM_RATE_RATIO_2  =  2,
ZL303XX_PTP_NUM_RATE_RATIO_4  =  4,
ZL303XX_PTP_NUM_RATE_RATIO_8  =  8,
ZL303XX_PTP_NUM_RATE_RATIO_16 =  16
} zl303xxnumRateRatioPktsE;


/** Enum used for BMCA flags individual bit fields */
typedef enum
{
   STATE_FM_QUALIFICATION_FLAG      = 0,  /* Indicates presence of Foreign Master Qualification */
   PD_FLAG,  /* Indicates whether the DISABLED state is used */
   PF_FLAG,  /* Indicates whether the FAULTY state is used*/
   PL_FLAG,  /* Indicates whether the LISTENING state is used */
   PP_FLAG,  /* Indicates whether the PRE-MASTER state is used */
   PU_FLAG,  /* Indicates whether the UNCALIBRATED state is used */
   ZL303XX_PTP_VALID_NUM_BMCA_FLAGS /*number of valid flags */
} zl303xx_BMCAFlagsE;


/** Implementation specific parameters required to create a PTP port. */
typedef struct zl303xx_PtpPortCreateS
{
   /** The handle to the clock that this port is attached to. */
   zl303xx_PtpClockHandleT clockHandle;

   /** Value that will be placed in the sourcePortIdentity field of egress PTP
    *  messages. Set to 0 to automatically generate a unique portNumber during
    *  port initialization. The identity of this port. This value is stored in
    *  the port's zl303xx_PortDS::portIdentity::portNumber field.
    *
    *  Notes:  Specifying a value that is greater than the maximum number of PTP
    *         ports on a node is allowed, but violates the standard. */
   Uint16T portNumber;

   /** Propagation delay measuring option. This value is stored in the port's
    *  zl303xx_PortDS::delayMechanism field. */
   zl303xx_DelayMechanismE delayMechanism;

   /** Indicates the method of issuing a Peer-Delay Response.
    *  IEEE-1588-2008: Clause 11.4.3 provides several options. */
   zl303xx_PtpPdelayRespMethodE pdRespTxMethod;

   /** The minimum permitted mean time interval between successive PDELAY_REQ
    *  messages. This value is stored in the port's
    *  zl303xx_PortDS::logMinPdelayReqInterval field. */
   Sint8T logMinPdelayReqInterval;

   /** The PTP version in use on this port. This value is stored in the port's
    *  zl303xx_PortDS::versionNumber field. */
   Uint16T ptpVersion;

   /** Enables the use of an acceptable master table. */
   zl303xx_BooleanE acceptableMasterTableEnabled;

   /** Enables the use of an acceptable slave table. */
   zl303xx_BooleanE acceptableSlaveTableEnabled;

   /** The number of acceptable master table entries to allocate. */
   Uint16T acceptableMasterTableLength;

   /** Added by ITU G.8275.2:
    *  Optional parameter to prohibit a Port from entering the SLAVE or PASSIVE
    *  state. Any ANNOUNCE messages received on a masterOnly Port are not used
    *  in the Best Master Clock Algorithm of the profile.  This value is stored
    *  in the port's zl303xx_OptionalPortDS::denyServiceRequests field.
    *  Values: ZL303XX_FALSE (default port operation)
    *          ZL303XX_TRUE (MASTER_ONLY mode). */
   zl303xx_BooleanE masterOnly;

   /** The local address of this port. All messages are transmitted from this address,
    *  and unicast messages destined to this address will be processed. */
   zl303xx_PortAddress localAddr;

   /** Enables a mixed multicast/unicast message transport system. */
   zl303xx_BooleanE mixedMode;

   /** Unicast negotiation parameters. */
   struct
   {
      /** Indicates unicast negotiation is used on this port. */
      zl303xx_BooleanE enabled;

      /** Enables sending of stacked unicast negotiation TLVs. */
      zl303xx_BooleanE stackedTlvEn;

      /** Log2 of 1/(max. ANNOUNCE rate). */
      Sint8T maxAnnounceRate;
      /** The maximum duration to grant an ANNOUNCE contract. */
      Uint32T maxAnnounceGrantSecs;
      /** The maximum number of ANNOUNCE contracts supported. */
      Uint32T maxAnnounceCount;

      /** Log2 of 1/(max. SYNC rate). */
      Sint8T maxSyncRate;
      /** The maximum duration to grant a SYNC contract. */
      Uint32T maxSyncGrantSecs;
      /** The maximum number of SYNC contracts supported.*/
      Uint32T maxSyncCount;

      /** Log2 of 1/(max. DELAY_RESP rate). */
      Sint8T maxDelayRespRate;
      /** The maximum duration to grant a DELAY_RESP contract. */
      Uint32T maxDelayRespGrantSecs;
      /** The maximum number of DELAY_RESP contracts supported.*/
      Uint32T maxDelayRespCount;

      /** Log2 of 1/(max. PEER_DELAY_RESP rate). */
      Sint8T maxPdelayRespRate;
      /** The maximum duration to grant a PEER_DELAY_RESP contract. */
      Uint32T maxPdelayRespGrantSecs;
      /** The maximum number of PEER_DELAY_RESP contracts supported.*/
      Uint32T maxPdelayRespCount;

      /** The combined Maximum PPS of all contracts of a single message type
       *  allowed on this Port.   */
      Uint32T totalPpsMax[ZL303XX_PTP_V2_NUM_MSG_IDS];

      /** Added by ITU G.8275.2:
       *  Replaces the grantDefault member.
       *  Set to ZL303XX_FALSE to grant a contract request by default unless
       *  explicitly set otherwise using zl303xx_PtpPortDenyServiceRequests().
       *  Set to ZL303XX_TRUE to deny requests by default. This value is stored
       *  in the port's zl303xx_OptionalPortDS::denyServiceRequests field.*/
      zl303xx_BooleanE denyServiceRequests;
      /** DEPRECATED
       *  Set to ZL303XX_TRUE to grant a contract request by default unless
       *  explicitly set otherwise using zl303xx_PtpGrantConfigSet(). Set to
       *  ZL303XX_FALSE to deny requests by default unless explicitly set otherwise. */
      zl303xx_BooleanE grantDefault;

      /** Maximum number of far-end PortAddresses that can be configured using
       *  zl303xx_PtpGrantConfigSet(). Must be set to a non-zero value if
       *  defaultGrant is set to ZL303XX_FALSE. */
      Uint16T grantTableLength;
   } unicastNegotiation;

   /** Unicast Master Table parameters. */
   struct
   {
      /** The maximum number of permitted entries in the Unicast Master Table. */
      Uint16T maxTableSize;

      /** The log2 of the mean interval, in seconds, between requests from a node
       *  for a unicast ANNOUNCE message contract. */
      Sint8T logQueryInterval;
   } unicastMasterTable;

   /** Specifies this port as an alternate master. The alternate master flag
    *  will not be set in transmitted messages if the port is not in MASTER mode. */
   zl303xx_BooleanE alternateMaster;

   /** Pointer to some external user data. This is passed to the packet-send
    *  porting functions. Memory management must be handled
    *  externally to PTP. */
   void *extData;

   /** Function used to transmit PTP messages. */
   zl303xx_PtpTxMsgFnT txMsgFn;

   /** Request port to be created with a specific handle. Set to ZL303XX_PTP_INVALID
    *  to auto-generate a handle. */
   zl303xx_PtpPortHandleT requestedHandle;

   /* A Union of all PROFILE SPECIFIC configuration data for the port. */
   union
   {
      /** Power Profile-specific port creation members. */
      zl303xx_PtpC37p238PortConfigS power;

      /** G.8275.1 (Telecom Phase) Profile-specific port creation members. */
      zl303xx_PtpG8275PortConfigS g8275p1;

      /** IEEE802.1AS (TSN) Profile-speicific port creation members. */
      zl303xx_Ptp802p1PortConfigS _802p1;
   } profileCfg;

   /** \Deprecated Previously used to set the size of the Port Foreign Master
    *  Table. All Foreign servers are now managed by the associated Clock,
    *  therefore this member is no longer used. */
   Uint16T maxForeignRecords;
   
   /** Indicates whether rateRatio is enabled or not. */
   zl303xx_BooleanE rateRatioEnabled;
   
   /** Indicates number of rateRatio packets. */
   zl303xxnumRateRatioPktsE numRateRatioPkts;

   /** Enables the port to go to init from faulty */
   zl303xx_BooleanE faultPassThroughEn;

   /* Indicates the allowed number of Peer-Delay reponse packets
    * dropped before PTSF-asCapable is declared
    */
   Uint8T allowedLostResponses;
} zl303xx_PtpPortCreateS;

/** Implementation specific parameters required to delete a PTP port.
 *  For future expansion -- it does not currently contain any members. */
typedef struct zl303xx_PtpPortDeleteS
{
   /** For future expansion. */
   Uint32T empty;
} zl303xx_PtpPortDeleteS;

/** Implementation specific parameters required to create a Virtual PTP port.
 *  (These are external phase/time input interfaces created to allow electrical
 *  sources to participate in the BMCA selection). */
typedef struct zl303xx_PtpVirtualPortConfigS
{
   /** The handle to the clock that this port is attached to. */
   zl303xx_PtpClockHandleT clockHandle;

   /** The identity of this port. This value is stored in the port's
    *  zl303xx_PortDS::portIdentity::portNumber field. Setting to INVALID (-1)
    *  will automatically generate a unique portNumber during initialization. */
   Uint16T portNumber;

   /** Flag indicating whether the Virtual Port is Failed. */
   zl303xx_BooleanE ptsf;

   /* Flag indicating if this port is connected to a PRTC source. */
   zl303xx_PtpPrtcLevelE prtcConnected;

   /** Virtual GM input to the virtual interface. */
   /***********************************************/
   zl303xx_ClockIdentity clockIdentity;
   zl303xx_ClockQuality clockQuality;
   Uint8T priority1;
   Uint8T priority2;
   Uint16T stepsRemoved;

   /* Time Properties associated with this virtual Input. */
   /********************************************/
   zl303xx_TimePropertiesDS timeProperties;

   /** DEPRECIATED: now in VP timeProperties member. */
   /** Time Source. */
   zl303xx_TimeSourceE timeSource;

   /** Member defining the localPriority of the virtual interface. */
   Uint8T localPriority;

   /* To be Depreciated. */
   zl303xx_BooleanE timeSetFlag;

   /** Pointer to some external user data. This is passed to the packet-send
    *  porting functions. Memory management must be handled
    *  externally to PTP. */
   void *extData;
} zl303xx_PtpVirtualPortConfigS;

/** Implementation specific parameters required to delete a Virtual PTP port.
 *  For future expansion -- it does not currently contain any members. */
typedef struct zl303xx_PtpVirtualPortDeleteS
{
   /** For future expansion. */
   Uint32T empty;
} zl303xx_PtpVirtualPortDeleteS;

/** Implementation specific parameters required to create a PTP stream. */
typedef struct zl303xx_PtpStreamCreateS
{
   /** The handle to the port that this stream is attached to. */
   zl303xx_PtpPortHandleT portHandle;

   /** The destination port address of this connection. */
   zl303xx_PortAddress destAddr;

   /** How this stream will determine if it is master or slave. This can be forced
    *  or automatically determined using BMCA. */
   zl303xx_PtpStreamModeE mode;

   /** Indicates that destAddr corresponds to a unicast transport layer protocol
    *  address (not multicast or broadcast). This member also sets unicastFlag in
    *  the flagField of the common PTP message header. */
   zl303xx_BooleanE unicast;

   /** The log2 of the mean time interval between successive ANNOUNCE messages
    *  sent from this port. This value should be uniform throughout a domain.
    *  This value is stored in the port's zl303xx_PortDS::logAnnounceInterval field
    *  if this stream is connected to a port. */
   Sint8T logAnnounceInterval;

   /** The number of announceIntervals that have to pass without receipt of an
    *  ANNOUNCE message before the occurrence of the event
    *  ANNOUNCE_RECEIPT_TIMEOUT_EXPIRES. Range: 2-255. This value is stored in
    *  the port's zl303xx_PortDS::announceReceiptTimeout field if this stream
    *  is connected to a port. */
   Uint8T announceReceiptTimeout;

   /** The mean time interval between successive SYNC messages. This value is
    *  stored in the port's zl303xx_PortDS::logSyncInterval field if this stream
    *  is connected to a port. */
   Sint8T logSyncInterval;

   /** When the port is in MASTER mode, this value is the minimum interval that
    *  a slave should use between delay request messages to this port. In MASTER
    *  mode, this value will be stored in the port's zl303xx_PortDS::logMinDelayReqInterval
    *  field. When the port is in SLAVE mode, this is the preferred interval to be used
    *  for transmitting delay request messages but is subject to any minimum
    *  specified by the master. */
   Sint8T logMinDelayReqInterval;

   /** When the port is in MASTER mode, this value is the minimum interval that
    *  a slave should use between peer delay request messages to this port. In MASTER
    *  mode, this value will be stored in the port's zl303xx_PortDS::logMinPdelayReqInterval
    *  field. When the port is in SLAVE mode, this is the preferred interval to be used
    *  for transmitting peer delay request messages but is subject to any minimum
    *  specified by the master. */
   Sint8T logMinPdelayReqInterval;

   /** According to the 1588-2008 Std., a SYNC message must be received before
    *  issuing a Delay Request. This parameter enforces a stricter condition
    *  that the Time (ToD) also needs to be set on the local hardware before a
    *  Delay Request message can be sent.  */
   zl303xx_BooleanE delayReqTimeSetRequired;

   /** Specifies the delay asymmetry of a stream (if known). These are added to,
    *  or subtracted from correctionField values of received or transmitted
    *  event messages as per Clause 11 of the IEEE-1588-2008 Std. */
   struct
   {
      /** Ingress values (added to received SYNC & PEER_DELAY_RESP). */
      zl303xx_TimeInterval ingress;
      /** Egress values (subtracted from transmitted DELAY_REQ & PEER_DELAY_REQ). */
      zl303xx_TimeInterval egress;
   } delayAsymmetry;

   /* A Union of all PROFILE SPECIFIC configuration data for the stream. */
   union
   {
      /** Power Profile-specific stream creation members. */
      zl303xx_PtpC37p238StreamConfigS power;

      /** G.8275.1 (Telecom Phase) Profile-specific stream creation members. */
      zl303xx_PtpG8275StreamConfigS g8275p1;
   } profileCfg;

   /** The duration in seconds for which to request unicast contracts. Only
    *  relevant for negotiated unicast streams. */
   Uint32T unicastNegotiationDuration;

   /** Flag indicating that a stream in the MASTER state that also has a UMT
    *  entry to a far-end-server will continue to request both ANNOUNCE and
    *  Timing service (contracts) with that server:
    *  - FALSE (default): request only ANNOUNCE service as long as the UMT
    *                     entry exists,
    *  - TRUE: request ANNOUNCE, SYNC and DELAY services as configured by the
    *          UMT (even in MASTER mode). */
   zl303xx_BooleanE unicastMonitorTiming;

   /** The time in seconds before unicastNegotiationDuration expires that the
    *  unicast negotiation contract will be renewed. */
   Uint32T unicastNegotiationRenew;

   struct
   {
      /** Enables a requester of a unicast contract to request a lower rate if
       *  its original request was denied. */
      struct
      {
         /** Boolean flags indicating if an Override is configured.
          *  - FALSE (default): no re-request is made for a lower packet rate,
          *  - TRUE: REQUEST is re-issued at half of the previous request rate
          *          if the previous was denied. */
         zl303xx_BooleanE enabled;

         /** The minimum packet interval to accept (intervalMax corresponds to a
          *  minimum PPS).
          *  Default = -128 (PTP_MESSAGE_INTERVAL_MIN); (so enabling this feature
          *                  without changing intervalMax will inhibit negotiation). */
         Sint8T intervalMax;
      } bestEffort[ZL303XX_PTP_V2_NUM_MSG_IDS];

      /** The retry configuration when a negotiation attempt is either DENIED or
       *  a TIMEOUT occurs (each configured independently). A client sends
       *  'logQueryCount' retries before it puts the request into a 'lockout'
       *  for some number of seconds. After the lockout expires, a single retry
       *  will occur before another lockout of the same period. If a contract
       *  exists then 'logQueryCount' retries will be used upon renewal. */
      zl303xx_PtpContractRetryConfigS retry;
   } unicastNegotiation;

   /** The number of bytes to pad at the end of each EVENT messageType. Required
    *  for messageLength extension feature (Clause 16.13 of IEEE-2017).
    *  Default: 0 for all EVENT messageTypes. */
   Uint16T msgLenPadding[ZL303XX_PTP_V2_MSG_NUM_EVENT_TYPES];

   /** The number of bytes to pad at the end of each EVENT messageType in the
    *  PAD TLV (Clause 16.13 of IEEE-2017). The length includes the TYPE and
    *  LENGTH fields so therefore must have a minimum pad length of 4.
    *  Default: 0 for all EVENT messageTypes. */
   Uint16T padTlvLen[ZL303XX_PTP_V2_MSG_NUM_EVENT_TYPES];

   /** The maximum acceptable clock class that will allow a master to be qualified.
    *  Set to 0 to disable. */
   Uint8T maxClockClass;

   /** The length of the time stamp record queue for this stream. The queue is
    *  used to store egress SYNC and DELAY_REQ time stamps, depending on the
    *  stream state (which determines what type of messages will be transmitted).
    *  Must be a power of 2. */
   Uint16T numTsRecords;

   /** The length of the two-step record queue for this stream. This queue is
    *  used to match time stamps for Sync and Follow_Up messages. Must be a
    *  power of 2. */
   Uint16T numTwoStepRecords;

   /** The clockClass override values assigned to this stream. When set to a
    *  non-zero value, this overrides the value of the associated clock or
    *  parent data set. This defaults to disabled (0) for normal operation. */
   Uint8T ingressClockClass;
   Uint8T egressClockClass;

   /* The group of all Stream parameters that can be over-written on egress. */
   struct
   {
      /* Boolean flags indicating if an Override is configured. */
      zl303xx_BooleanE anncOvrdEn[ZL303XX_PTP_STREAM_OVERRIDE_NUM_ANNC_ENTRIES];

      /* Override parameters (not all fields are used). */
      zl303xx_PtpV2MsgHeaderS ptpHeader;
      zl303xx_TimePropertiesDS timeProperties;
      zl303xx_PtpV2MsgAnnounceS anncData;
   } override;

   /** The duration in seconds for which to issue a 'keep-alive' event for the
    *  stream if no other related event has been issued (i.e BMCA UPDATE, STATE
    *  CHANGE, CONFIG CHANGE all count as a stream event). */
   Uint32T keepAliveSec;

   /** Parameters that determine how a stream transitions from the
    *  UNCALIBRATED to the SLAVE state and how it manages egress ANNC data. */
   struct
   {
      /** The number of Announce Intervals that have to pass before a stream in
       *  the UNCALIBRATED state can transition to the SLAVE state.
       *  Range: 0-255. A value of 0 disables this restriction.  */
      Uint8T anncIntervals;  /*uncalibratedIntervals; */
      /** Flag indicating if the clock must be in a LOCKED state before a stream
       *  can transition to the SLAVE State. */
      zl303xx_BooleanE lockRequired;
   } uncalibrated;

   /** Pointer to some external user data. Memory management must be handled
    *  externally to PTP. */
   void *extData;

   /** Request stream to be created with a specific handle. Set to ZL303XX_PTP_INVALID
    *  to auto-generate a handle. */
   zl303xx_PtpStreamHandleT requestedHandle;

   /** Boolean flags for various stream options. */
   Uint32T createFlags;

   /** Initial values for the next transmitted sequenceId. Used for warm start
    *  operation to prevent jumps in sequenceId. */
   Uint16T nextTxSequenceId[ZL303XX_PTP_V2_NUM_MSG_IDS];

} zl303xx_PtpStreamCreateS;

/* List of flags for zl303xx_PtpStreamCreateS::createFlags */
/** Also fire ZL303XX_PTP_EVENT_STREAM_CREATE when zl303xx_PtpStreamCreate() is called */
#define ZL303XX_PTP_STREAM_CREATE_FIRE_EVENT   (0x1 << 0)

/** Force the interval between Sync message transmissions to never be less than
 *  the configured logSyncInterval value. This behavior is NON-STANDARD because
    the actual mean message interval will be less than the configured interval. */
#define ZL303XX_PTP_SYNC_FORCE_MIN_INTVL       (0x1 << 1)

/** Force the interval between Delay_Req message transmissions to never be less
 *  than the configured logMinDelayReqInterval value. */
#define ZL303XX_PTP_DREQ_FORCE_MIN_INTVL       (0x1 << 2)

/** Force the interval between Peer_Delay_Req message transmissions to never be
 *  less than the configured logMinPdelayReqInterval value. */
#define ZL303XX_PTP_PDREQ_FORCE_MIN_INTVL      (0x1 << 3)

/** Implementation specific parameters required to delete a PTP stream. */
typedef struct zl303xx_PtpStreamDeleteS
{
   /** This array will be overwritten with the next transmit sequenceIds for
    *  stream that has been deleted. These values can be copied to
    *  zl303xx_PtpStreamCreateS::nextTsSequenceId[] for warm start operation. */
   Uint16T nextTxSequenceId[ZL303XX_PTP_V2_NUM_MSG_IDS];
} zl303xx_PtpStreamDeleteS;

/** Structure used to return message counters for debug purposes. */
typedef struct zl303xx_PtpStreamCountersS
{
   struct
   {
      /** Number of ingress packets. */
      Uint32T rx;
      /** Number of ingress packets successfully processed. */
      Uint32T rxProcessed;
      /** Number of lost packets (based on jumps in sequenceId). */
      Uint32T lost;
      /** Number of transmitted packets. */
      Uint32T tx;
   } msg[ZL303XX_PTP_V2_NUM_MSG_IDS];
} zl303xx_PtpStreamCountersS;

/** Structure used to return per message PPS rates. */
typedef struct zl303xx_PtpTotalPpsS
{
   /** Array of PPS counters. */
   Uint32T msg[ZL303XX_PTP_V2_NUM_MSG_IDS];
} zl303xx_PtpTotalPpsS;

/** Additional data required when registering a received PTP message. This
 * implementation specific data is not available in the PTP message itself. */
typedef struct zl303xx_PtpRxMsgDataS
{
   /** Source port address of this message. */
   zl303xx_PortAddress srcAddr;

   /** Destination port address of this message. */
   zl303xx_PortAddress destAddr;

   /** Receive time stamp of this message, if it arrived on the EVENT interface. */
   zl303xx_TimeStamp rxTs;
   /** Small buffer for any additional implementation-specific data. */
   Uint8T extData[ZL303XX_PTP_RX_MSG_EXT_DATA_LEN];

} zl303xx_PtpRxMsgDataS;

/** Additional data required when registering a received PTP message for a
 *  specific port. This implementation specific data is not available
 *  in the PTP message itself. */
typedef struct zl303xx_PtpPortRxMsgDataS
{
   /** Handle to a previously created port that this message was received on. */
   zl303xx_PtpPortHandleT portHandle;

   /** Source port address of this message. */
   zl303xx_PortAddress srcAddr;

   /** Destination port address of this message. */
   zl303xx_PortAddress destAddr;

   /** Receive time stamp of this message, if it arrived on the EVENT interface. */
   zl303xx_TimeStamp rxTs;

   /** Small buffer for any additional implementation-specific data. */
   Uint8T extData[ZL303XX_PTP_RX_MSG_EXT_DATA_LEN];
} zl303xx_PtpPortRxMsgDataS;

/** Additional data required when registering a received PTP message for a
 *  specific stream. This implementation specific data is not available
 *  in the PTP message itself. */
typedef struct zl303xx_PtpStreamRxMsgDataS
{
   /** Handle to a previously created stream. */
   zl303xx_PtpStreamHandleT streamHandle;

   /** Receive time stamp of this message, if it arrived on the EVENT interface. */
   zl303xx_TimeStamp rxTs;

   /** Small buffer for any additional implementation-specific data. */
   Uint8T extData[ZL303XX_PTP_RX_MSG_EXT_DATA_LEN];
} zl303xx_PtpStreamRxMsgDataS;

/** Implementation specific parameter structure for a time stamp record.
 *  A time stamp record represents the egress time of a SYNC message (in the
 *  case of a two-step clock) or a PEER / DELAY_REQ message. */
typedef struct zl303xx_PtpTsRecordS
{
   /** Handle to the stream that the egress message was transmitted on. */
   zl303xx_PtpStreamHandleT streamHandle;

   /** The PTP message type triggering the timestamp. */
   Uint16T messageType;

   /** Value of the sequenceId field in the egress PTP message. */
   Uint16T sequenceId;

   /** Egress time stamp recorded by a hardware device. */
   zl303xx_TimeStamp timeStamp;

} zl303xx_PtpTsRecordS;

/** Implementation specific structure used to configure UMT entry parameters
 *  that do not apply to all types of streams. */
typedef struct zl303xx_PtpUmtEntryConfigS
{
   /** Announce messages will be requested. This is a non-configurable parameter
    *  which is automatically updated internally based on clock profile and the
    *  port delayMechanism setting. */
   zl303xx_BooleanE  reqAnnc;
   /** Sync messages will be requested. This is a non-configurable parameter
    *  which is automatically updated internally based on clock profile and the
    *  port delayMechanism setting. */
   zl303xx_BooleanE  reqSync;
   /* Delay_Resp messages will be requested. This is a non-configurable parameter
    *  which is automatically updated internally based on clock profile and the
    *  port delayMechanism setting. */
   zl303xx_BooleanE  reqDelayResp;
   /* Peer_Delay_Resp messages will be requested. This is a non-configurable parameter
    *  which is automatically updated internally based on clock profile and the
    *  port delayMechanism setting. */
   zl303xx_BooleanE  reqPdelayResp;
   /**  (Deprecated) Sync and Delay_Resp messages will be requested. If reqSync
    *  or reqDelayResp is ZL303XX_TRUE, this parameter will be ignored. */
   zl303xx_BooleanE  reqTiming;

   /** The maximum acceptable log2 interval (minimum rate) for Announce messages. */
   Sint8T maxAnncIntvl;
   /** The maximum acceptable log2 interval (minimum rate) for Sync messages. */
   Sint8T maxSyncIntvl;
   /** The maximum acceptable log2 interval (minimum rate) for Delay_Resp messages. */
   Sint8T maxDelayRespIntvl;
   /** The maximum acceptable log2 interval (minimum rate) for Pdelay_Resp messages. */
   Sint8T maxPdelayRespIntvl;

   /** Allow this master to be selected by the BMCA. */
   zl303xx_BooleanE  bmcaAllow;

   /** Small buffer for any additional implementation-specific data. This data will
    *  be available to the ZL303XX_PTP_EVENT_STREAM_CREATE handler in
    *  zl303xx_PtpEventStreamCreateS::rxExtData */
   Uint8T extData[ZL303XX_PTP_RX_MSG_EXT_DATA_LEN];
} zl303xx_PtpUmtEntryConfigS;

/** Implementation specific structure used to add entries to a port's
 *  Unicast Master Table. */
typedef struct zl303xx_PtpUmtEntryS
{
   /** Parameters used to create the stream associated with this UMT entry. */
   zl303xx_PtpStreamCreateS    streamConfig;

   /** Additional parameters specific to unicast master entries. */
   zl303xx_PtpUmtEntryConfigS  entryConfig;

   /** Telecom Profile-specific members. */
   zl303xx_PtpTelecomUmtEntryS telecomConfig;
} zl303xx_PtpUmtEntryS;

/** Used to return run-time info about a Unicast Master entry. */
typedef struct zl303xx_PtpUmtEntryStatusS
{
   /** Clock class of this master. */
   Uint8T  clockClass;

   /* Status data that is only valid if the Telecom Profile is enabled on the clock. */
   struct
   {
      /**  (Deprecated) This member will always be zero. PTSF flags can be queried using
       *  zl303xx_PtsfFlagGetAndMask(). */
      Sint32T ptsf;

      /** SSM and G.781 Quality Levels converted from clockClass. SSM QL will be
       * 0xFF if clockClass cannot be converted to a QL. */
      zl303xx_PtpTelecomQualityLevelS    quality;

      /** If non-zero, this is the time remaining until this stream can be
       *  selected by the Telecom Profile BMCA. */
      Uint32T waitToRestoreSec;

      /** Lockout statistics that apply to the entire stream. */
      zl303xx_PtpContractLockoutStatusS  stream;
      /** Retry statistics that only apply to individual message contracts. */
      zl303xx_PtpContractRetryStatusS    msgType[PTP_NEGOTIATED_NUM_MSG_TYPES];
   } telecom;
} zl303xx_PtpUmtEntryStatusS;

/** Used to configure which contracts can be granted to another port. */
typedef struct zl303xx_PtpGrantConfigS
{
   /** Far-end port address. */
   zl303xx_PortAddress portAddress;
   /** PTP message type mask.
    *  e.g., (1 << ZL303XX_MSG_ID_ANNOUNCE) | (1 << ZL303XX_MSG_ID_SYNC) will grant
    *  Announce and Sync contract requests, but deny Delay_Resp contracts. This
    *  bit-mask will override a port's defaultGrant setting. */
   Uint32T msgTypeMask;
} zl303xx_PtpGrantConfigS;

/** Used to configure time stamp logging. */
typedef struct zl303xx_PtpTsLogConfigS
{
   /** How long time stamps should be logged for. */
   Uint32T durationSec;
   /** Function to be called for every time stamp pair. */
   swFuncPtrTSLogging callback;
} zl303xx_PtpTsLogConfigS;

/** Types of contract rejection that can occur in unicast negotiation. */
typedef enum zl303xx_PtpUniNegContractErrorE
{
   /** Requested message rate was rejected. */
   ZL303XX_PTP_CONTRACT_RATE_REJECTED = 0,
   /** Requested contract duration rejected. */
   ZL303XX_PTP_CONTRACT_DURATION_REJECTED,
   /** Port default behavior is to reject all requests. Setting the
    *  denyServiceRequests member of the Port to TRUE rejects all REQUESTS. */
   ZL303XX_PTP_CONTRACT_PORT_DENY_REJECTED,
   /** Grant table rejected the request. Either an entry exists that explicitly
    *  denied the request, or the default behavior is to deny. */
   ZL303XX_PTP_CONTRACT_GRANT_TBL_REJECTED,
   /** Contract count max limit reached for the request. */
   ZL303XX_PTP_CONTRACT_COUNT_MAX_REACHED,
   /** Received a GRANT TLV with durationField set to 0. This event is generated
    *  by a requesting (grantee) port. */
   ZL303XX_PTP_CONTRACT_REQUEST_DENIED,

   ZL303XX_PTP_CONTRACT_ERROR_MAX = ZL303XX_PTP_CONTRACT_DURATION_REJECTED
} zl303xx_PtpUniNegContractErrorE;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_CONTRACT_REJECTED event. */
typedef struct zl303xx_PtpEventContractRejectedS
{
   /** For a slave, this is the handle of the stream the rejection occurred
    *  on. For a master, this is the handle of the port the rejection occurred on. */
   Uint32T handle;

   /** The PTP message type whose request was rejected by the master. */
   zl303xx_MessageTypeE messageId;

   /** The type of rejection by the master. */
   zl303xx_PtpUniNegContractErrorE errorType;
} zl303xx_PtpEventContractRejectedS;

/** The Operating state of a stream state machine. This state is determined by
 *  a number of factors including the clock's operating mode, the port's BMCA
 *  state and the streams desired state as compared to its configuration.*/
typedef enum zl303xx_PtpStreamOperatingStateE
{
   /** Operating state is currently unknown. */
   ZL303XX_PTP_STREAM_OPER_STATE_UNKNOWN = 0,
   /** The stream has been disabled. */
   ZL303XX_PTP_STREAM_OPER_STATE_DISABLED = 1,

   /** The stream is operating on a clock with a clock class <= 127 and is
    *  currently grandmaster in the network. */
   ZL303XX_PTP_STREAM_OPER_STATE_GRANDMASTER = 2,
   /** The stream is operating as a master on a clock with a class >= 128
    *  because it is currently the best clock available. */
   ZL303XX_PTP_STREAM_OPER_STATE_MASTER = 3,
   /** The stream is operating as a master on a clock that is synchronized to
    *  a better server via another stream. This stream is repeating the timing
    *  information from the grandmaster. */
   ZL303XX_PTP_STREAM_OPER_STATE_REPEATER = 4,
   /** The stream is operating on a clock but is currently not the grandmaster
    * in the network and therefore is in the passive state. */
   ZL303XX_PTP_STREAM_OPER_STATE_PASSIVE_MASTER = 5,

   /** The stream is operating as a slave due to the BMC Algorithm determining
    *  that it is connected to the grandmaster in the network. */
   ZL303XX_PTP_STREAM_OPER_STATE_SLAVE = 6,
   /** The stream is listening to ANNOUNCE messages from a server but is not
    *  collecting any timing data. */
   ZL303XX_PTP_STREAM_OPER_STATE_LISTENING = 7,
   /** The stream is receiving ANNOUNCE messages and timing data (SYNC, etc.)
    *  from a server that is not the best server in the network. */
   ZL303XX_PTP_STREAM_OPER_STATE_MONITORING = 8
} zl303xx_PtpStreamOperatingStateE;


/************************************************************/
/* ENUMs used for retrieving hardware PLL performance data. */
/************************************************************/

/** The Operating Mode of the PLL. These definitions are a combination of bits
 *  reflecting the states of both the Frequency and Phase components of the
 *  local PLL. */
typedef enum zl303xx_PtpPllOperatingModeE
{
   ZL303XX_PTP_PLL_MODE_UNKNOWN        = 0x00,
   ZL303XX_PTP_PLL_MODE_ELEC_FREQUENCY = 0x01,
   ZL303XX_PTP_PLL_MODE_ELEC_PHASE     = 0x02,
   ZL303XX_PTP_PLL_MODE_ELEC_REF_SYNC  = 0x03,
   ZL303XX_PTP_PLL_MODE_PKT_FREQUENCY  = 0x10,
   ZL303XX_PTP_PLL_MODE_PKT_PHASE      = 0x20,
   ZL303XX_PTP_PLL_MODE_PKT_REF_SYNC   = 0x30,
   ZL303XX_PTP_PLL_MODE_HYBRID_SYNCE   = (ZL303XX_PTP_PLL_MODE_ELEC_FREQUENCY | ZL303XX_PTP_PLL_MODE_PKT_PHASE),
   ZL303XX_PTP_PLL_MODE_MAX_TYPES
} zl303xx_PtpPllOperatingModeE;

/** The current state of the PLL in its attempt to achieve LOCK. These
 *  definitions apply to both Hardware and Software PLLs. */
typedef enum zl303xx_PtpPllOperatingStateE
{
   ZL303XX_PTP_PLL_STATE_UNKNOWN = 0x00,
   ZL303XX_PTP_PLL_STATE_FREERUN,
   ZL303XX_PTP_PLL_STATE_HOLDOVER,
   ZL303XX_PTP_PLL_STATE_FREQ_ACQUIRING,
   ZL303XX_PTP_PLL_STATE_FREQ_ACQUIRED,
   ZL303XX_PTP_PLL_STATE_PHASE_ACQUIRING,
   ZL303XX_PTP_PLL_STATE_PHASE_ACQUIRED,
   ZL303XX_PTP_PLL_STATE_REF_FAILED,
   ZL303XX_PTP_PLL_STATE_MAX_TYPES
} zl303xx_PtpPllOperatingStateE;

/** The current quality of the PLL. These may be mapped to a variety of other
 *  application definitions. (For example, G.781 QLs).  */
typedef enum zl303xx_PtpPllFreqHoldoverQualityE
{
   ZL303XX_PTP_PLL_HOLDOVER_QUALITY_UNKNOWN = 0,
   ZL303XX_PTP_PLL_HOLDOVER_QUALITY_CAT1,
   ZL303XX_PTP_PLL_HOLDOVER_QUALITY_CAT2,
   ZL303XX_PTP_PLL_HOLDOVER_QUALITY_CAT3,
   ZL303XX_PTP_PLL_HOLDOVER_QUALITY_OTHER,
   ZL303XX_PTP_PLL_HOLDOVER_QUALITY_MAX_TYPES
} zl303xx_PtpPllFreqHoldoverQualityE;


/** The status of the PTP transmit function */
typedef enum zl303xx_PtpTxStatusE
{
   ZL303XX_PTP_TX_OK = 0,
   ZL303XX_PTP_TX_MSG_BINDING_NULL,
   ZL303XX_PTP_TX_MSG_BUFFER_DATA_NULL,
   ZL303XX_PTP_TX_PORT_NULL,
   ZL303XX_PTP_TX_MSG_TOO_BIG,
   ZL303XX_PTP_TX_STREAM_STATE_OUT_OF_RANGE,
   ZL303XX_PTP_TX_SOCKET_UNSUPPORTED_OPERATION,
   ZL303XX_PTP_TX_SOCKET_ERROR,
   ZL303XX_PTP_TX_USER_ENFORCED_ERROR,
   ZL303XX_PTP_TX_STATUS_MAX_TYPES
}zl303xx_PtpTxStatusE;
/*************************************************************/
/* Structure definitions for obtaining hardware information. */
/*************************************************************/

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_STREAM_STATE_CHANGE event. */
typedef struct zl303xx_PtpEventStreamStateChangeS
{
   /** Handle to the stream whose state changed. */
   zl303xx_PtpStreamHandleT streamHandle;
   /** Pointer to the stream's external data. */
   void *streamExtData;
   /** New Operating state of the stream. */
   zl303xx_PtpStreamOperatingStateE operState;
} zl303xx_PtpEventStreamStateChangeS;

/** Implementation specific data member for the
 *  zl303xx_PtpEventClockBmcaUpdateS structure. */
typedef struct zl303xx_PtpEventClockBmcaEntryS
{
   /** Handle to the stream on which the BMCA data was captured. */
   zl303xx_PtpStreamHandleT streamHandle;
   /** Port address of this far-end node. */
   zl303xx_PortAddress farEndPortAddress;
   /** The BMCA Comparison data that was used to make the BMCA determination. */
   zl303xx_PtpBmcCompareDataS bmcaData;
   /** The Telecom Profile for Frequency data used to make a BMCA determination. */
   zl303xx_PtpTelecomCompareDataS telecomData;
   /** The Telecom Profile for Phase data used to make a BMCA determination. */
   zl303xx_PtpG8275ProfileCompareDataS tpPhaseData;
} zl303xx_PtpEventClockBmcaEntryS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_STREAM_KEEP_ALIVE event. */
typedef struct zl303xx_PtpEventStreamKeepAliveS
{
   /** Handle to the clock associated with the stream event. */
   zl303xx_PtpClockHandleT clockHandle;
   /* Operating Profile of the Clock associated with the Stream. */
   zl303xx_PtpProfileE clockProfile;

   /** Pointer to the clock's external data. */
   void *clockExtData;

   /** Pointer to the Stream data in BMCA format. */
   zl303xx_PtpEventClockBmcaEntryS bmcaEntry;
} zl303xx_PtpEventStreamKeepAliveS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_CLOCK_BMCA_UPDATE event. */
typedef struct zl303xx_PtpEventClockBmcaUpdateS
{
   /** Handle to the clock whose BMCA data is being exported. */
   zl303xx_PtpClockHandleT clockHandle;
   /** The DefaultDS of the exporting clock in 'Comparison' format. */
   zl303xx_PtpBmcCompareDataS clockDefault;
   /* Operating Profile of the Clock issuing the update. */
   zl303xx_PtpProfileE profile;

   /** Pointer to the clock's external data. */
   void *clockExtData;

   /** The number of BMCA Entries that have been qualified by the Clock. */
   Uint16T numBmcaEntries;
   /** Pointer to an ordered list of BMCA entries (NULL if none exist). */
   zl303xx_PtpEventClockBmcaEntryS *bmcaEntry;
} zl303xx_PtpEventClockBmcaUpdateS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_LEAP_SECONDS_FLAG_CHANGE event. */
typedef struct zl303xx_PtpEventLeapSecondsFlagChangeS
{
   /** Handle to the clock for which the event occurred. */
   zl303xx_PtpClockHandleT clockHandle;
   /** The configured TimePropertiesDS of the exporting clock. */
   zl303xx_TimePropertiesDS localTimeProperties;
   /** The current, dynamic TimePropertiesDS of the exporting clock which
    *  includes the current leap59 & leap 61 values. */
   zl303xx_TimePropertiesDS currentTimeProperties;
   /* The previous values of the leap59 & leap61 flags. */
   zl303xx_BooleanE previousLeap59;
   zl303xx_BooleanE previousLeap61;
   /** The current PTP Time on the clock. */
   zl303xx_TimeStamp currentPtpTime;

   /** Pointer to the clock's external data. */
   void *clockExtData;
} zl303xx_PtpEventLeapSecondsFlagChangeS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_UTC_OFFSET_CHANGE event. */
typedef struct zl303xx_PtpEventUtcOffsetChangeS
{
   /** Handle to the clock for which the event occurred. */
   zl303xx_PtpClockHandleT clockHandle;
   /** The configured TimePropertiesDS of the exporting clock. */
   zl303xx_TimePropertiesDS localTimeProperties;
   /** The complete, current TimePropertiesDS of the exporting clock which
    *  includes the current utcOffset value. */
   zl303xx_TimePropertiesDS currentTimeProperties;
   /* The previous values of the UTC Offset and Valid flag. */
   Sint16T previousUtcOffset;
   zl303xx_BooleanE previousUtcOffsetValid;
   /** The current PTP Time on the clock. */
   zl303xx_TimeStamp currentPtpTime;

   /** Pointer to the clock's external data. */
   void *clockExtData;
} zl303xx_PtpEventUtcOffsetChangeS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_RX_STEPS_REMOVED_EXCEEDED event. */
typedef struct zl303xx_PtpEventRxStepsRemovedExceededS
{
   /** Handle to the clock for which the event occurred. */
   zl303xx_PtpClockHandleT clockHandle;

   /** Received Announce message header and data. */
   zl303xx_PtpV2MsgHeaderS   ptpHeader;
   zl303xx_PtpV2MsgAnnounceS announceData;

   /* The Port that received the message. */
   zl303xx_PortIdentity rxPortIdentity;

   /** Configured maximum value of the stepsRemoved parameter. */
   Uint16T maxStepsRemoved;

   /** Pointer to the clock's external data. */
   void *clockExtData;
} zl303xx_PtpEventRxStepsRemovedExceededS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_HW_CMD_CLOCK_TIME_GET command. */
typedef struct zl303xx_PtpHwClockTimeGetS
{
   /** Input parameters. */
   struct
   {
      /** Handle to the clock that is being queried. */
      zl303xx_PtpClockHandleT clockHandle;
      /** Two-step clock flag. If this is false (egress timestamps can be
       *  inserted on the fly), it may still be necessary to return part
       *  of the time stamp. E.g., a hardware device cannot write the top
       *  16 bits of the time stamp. */
      zl303xx_BooleanE twoStepFlag;
      /** Interface that the message will be sent on. */
      zl303xx_PtpInterfaceE interface;
      /** Pointer to the clock's external data. */
      void *clockExtData;
   } input;

   /** Output parameters. */
   struct
   {
      /** Return the current time of the clock in PTP format here. */
      zl303xx_TimeStamp ptpTime;
   } output;

} zl303xx_PtpHwClockTimeGetS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_HW_CMD_CLOCK_TIME_SET command. */
typedef struct zl303xx_PtpHwClockTimeSetS
{
   /** Input parameters. */
   struct
   {
      /** Handle to the clock that is being set. */
      zl303xx_PtpClockHandleT clockHandle;
      /** Pointer to the clock's external data. */
      void *clockExtData;
      /** Time to set in PTP format. */
      zl303xx_TimeStamp ptpTime;
   } input;
} zl303xx_PtpHwClockTimeSetS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_HW_CMD_PHYS_ADDR_GET command. */
typedef struct zl303xx_PtpHwPhysAddrGetS
{
   /** Input parameters. */
   struct
   {
      /** Handle to the port being queried. */
      zl303xx_PtpPortHandleT portHandle;
      /** Pointer to the port's external data. */
      void *portExtData;
   } input;

   /** Output parameters. */
   struct
   {
      /** Return the physical layer protocol string. */
      char physicalLayerProtocol[32];
      /** Return the length of the physical address string. */
      Uint16T physicalAddressLength;
      /** Return the physical address string. */
      Uint8T physicalAddress[16];
   } output;

} zl303xx_PtpHwPhysAddrGetS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_HW_CMD_LOCK_STATUS_GET command. */
typedef struct zl303xx_PtpHwLockStatusGetS
{
   /** Input parameters. */
   struct
   {
      /** Handle to the clock that is being set. */
      zl303xx_PtpClockHandleT clockHandle;
      /** Pointer to the clock's external data. */
      void *clockExtData;

      /* A minimum of FREQUENCY lock is expected. However, some profile require
       * PHASE lock as well. Provide this criteria so the driver can reply. */
      Uint32T phaseRequired;
      /* For compatibility purposes, provide the PTP profile in case future
       * profiles are added. */
      zl303xx_PtpProfileE profile;
   } input;

   /** Output parameters. */
   struct
   {
      /** Return the clock Lock Status (TRUE or FALSE). */
      Uint32T lockStatus;
   } output;

} zl303xx_PtpHwLockStatusGetS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_HW_CMD_CLOCK_STABILITY_GET command. */
typedef struct zl303xx_PtpHwClockStabilityGetS
{
   /** Input parameters. */
   struct
   {
      /** Handle to the clock that is being set. */
      zl303xx_PtpClockHandleT clockHandle;
      /** Pointer to the clock's external data. */
      void *clockExtData;
   } input;

   /** Output parameters. */
   struct
   {
      /** Return the clock Stability values. */
      Uint32T freqStability;
      Sint32T phaseStability;
   } output;

} zl303xx_PtpHwClockStabilityGetS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_HW_CMD_TIMESET_STATUS_GET command. */
typedef struct zl303xx_PtpHwTimeStatusGetS
{
   /** Input parameters. */
   struct
   {
      /** Handle to the clock that is being queried. */
      zl303xx_PtpClockHandleT clockHandle;
      /** Pointer to the clock's external data. */
      void *clockExtData;
   } input;

   /** Output parameters. */
   struct
   {
      /** Return the Time-of-Day Set Status. */
      zl303xx_BooleanE bTimeSet;
   } output;

} zl303xx_PtpHwTimeStatusGetS;

/** Implementation specific data structure to retrieve performance data
 *  from the local PLL. It is used with the ZL303XX_PTP_HW_CMD_PLL_PERF_DATA_GET
 *  hardware interface command. */
typedef struct zl303xx_PtpPllPerformanceDataS
{
   /** PLL Operating mode (ELEC, PKT, HYBRID, etc.). */
   zl303xx_PtpPllOperatingModeE       pllOperatingMode;
   /** Hardware PLL/Algorithm lock status (Freerun, Holdover, Acquiring,
    *  Locked, etc.). Valid when pllOperatingMode = ELECTRIC or HYBRID. */
   zl303xx_PtpPllOperatingStateE      hwPllOperatingState;
   /** Software PLL/Algorithm lock status (Freerun, Holdover, Acquiring,
    *  Locked, etc.). Valid when pllOperatingMode = PACKET_MODE. */
   zl303xx_PtpPllOperatingStateE      swPllOperatingState;
   /* Holdover quality category (Category 1,2,or 3). */
   zl303xx_PtpPllFreqHoldoverQualityE pllHoldoverQuality;
   /* Holdover IN/OUT of Specification. */
   zl303xx_BooleanE bHoldoverInSpec;
   /* Flag indicating if the SyncE is frequency traceable (for HYBRID mode). */
   zl303xx_BooleanE synceTraceable;
   /* Flag indicating that a reference switch or other event may have recently
    * occurred and that the current reference should be viewed as LOCKED. Once
    * this flag goes to FALSE, the actual state of the PLL will be used internally. */
   zl303xx_BooleanE settlingTimeActive;
} zl303xx_PtpPllPerformanceDataS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_HW_CMD_PLL_PERF_DATA_GET command. */
typedef struct zl303xx_PtpHwPllPerformanceGetS
{
   /** Input parameters. */
   struct
   {
      /** Handle to the clock that is being queried. */
      zl303xx_PtpClockHandleT clockHandle;
      /** Pointer to the clock's external data. */
      void *clockExtData;
   } input;

   /** Output parameters. */
   struct
   {
      /** Return the PLL Status parameters. */
      zl303xx_PtpPllPerformanceDataS pllPerfData;
   } output;

} zl303xx_PtpHwPllPerformanceGetS;


/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_CLOCK_CREATE event. */
typedef struct zl303xx_PtpEventClockCreateS
{
   /** Handle to the clock being created. */
   zl303xx_PtpClockHandleT clockHandle;
   /* Operating Profile of the Clock issuing the update. */
   zl303xx_PtpProfileE profile;

   /** Pointer to the clocks's external data. */
   void *clockExtData;

   /** The DefaultDS of the exporting clock in 'Comparison' format. */
   zl303xx_PtpBmcCompareDataS clockDefault;
} zl303xx_PtpEventClockCreateS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_CLOCK_DELETE event. */
typedef struct zl303xx_PtpEventClockDeleteS
{
   /** Handle to the clock being automatically deleted. */
   zl303xx_PtpClockHandleT clockHandle;
   /** Pointer to the port's external data (will likely need to be freed). */
   void *clockExtData;
} zl303xx_PtpEventClockDeleteS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_PORT_DELETE event. */
typedef struct zl303xx_PtpEventPortDeleteS
{
   /** Handle to the port being automatically deleted. */
   zl303xx_PtpPortHandleT portHandle;
   /** Pointer to the port's external data (will likely need to be freed). */
   void *portExtData;
   /** The port's local address. */
   zl303xx_PortAddress portAddr;
} zl303xx_PtpEventPortDeleteS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_STREAM_CREATE event. */
typedef struct zl303xx_PtpEventStreamCreateS
{
   /** Handle to the clock this stream is associated with. */
   zl303xx_PtpClockHandleT clockHandle;
   /** Handle to the port this stream is associated with. */
   zl303xx_PtpPortHandleT portHandle;
   /** Pointer to the clock's external data. */
   void *clockExtData;
   /** Pointer to the port's external data. */
   void *portExtData;
   /** The port's source address. */
   zl303xx_PortAddress portSrcAddr;

   /** Handle of the stream being automatically created. */
   zl303xx_PtpStreamHandleT streamHandle;
   /** Parameters generated by PTP used to initialize this stream. */
   zl303xx_PtpStreamCreateS *streamConfig;
   /** Implementation-specific data from the PTP message that triggered this event. */
   Uint8T rxExtData[ZL303XX_PTP_RX_MSG_EXT_DATA_LEN];
} zl303xx_PtpEventStreamCreateS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_STREAM_DELETE event. */
typedef struct zl303xx_PtpEventStreamDeleteS
{
   /** Handle to the stream being automatically deleted. */
   zl303xx_PtpStreamHandleT streamHandle;
   /** Handle to the clock this stream is associated with. */
   zl303xx_PtpClockHandleT clockHandle;
   /** Pointer to the stream's external data (will likely need to be freed). */
   void *streamExtData;
   /** Current stream configuration. */
   zl303xx_PtpStreamCreateS streamConfig;
} zl303xx_PtpEventStreamDeleteS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_SERVO_DATA event. */
typedef struct zl303xx_PtpEventServoDataS
{
   /** Handle to the stream the time stamp pair is associated with. */
   zl303xx_PtpStreamHandleT streamHandle;
   /** Pointer to the stream's external data. */
   void *streamExtData;

   /** The transmit time stamp of the event message in PTP format. */
   zl303xx_TimeStamp txTs;
   /** The receive time stamp of the event message in PTP format. */
   zl303xx_TimeStamp rxTs;
   /** The value of the correctionField in the PTP message. */
   Uint64S correctionField;
   /** The value of the peerMeanPathDelay calculated at the current node.
    *  I.e. An estimate of the one-way propagation delay to its Peer Node.
    *  If the value of zl303xx_PortDS::delayMechanism is DELAY_MECHANISM_P2P.
    *  Otherwise, this is 0.  */
   Uint64S peerMeanPathDelay;
   /** Whether this time stamp pair is from a SYNC or DELAY_REQ/DELAY_RESP. */
   zl303xx_PtpTsPairE tsPairType;
   /** SequenceId of the PTP message associated with these time stamps. */
   Uint16T sequenceId;
   /* Flag indicating if the offsetFromMaster value provided is valid. It may be
    * invalid if there was a correctionField overflow or the source of the
    * SYNC and Delay-Request messages are different.    */
   zl303xx_BooleanE offsetFromMasterValid;
   /* The offsetFromMaster value calculated from the last SYNC and Delay-Request
    * messages received. The offsetFromMasterValid flag should be checked to
    * determine if this value is accurate or may contain error.   */
   zl303xx_TimeStamp offsetFromMaster;
   /* Flag indicating if the meanPathDelay value provided is valid. It may be
    * invalid if there was a correctionField overflow or the source of the
    * SYNC and Delay-Request messages are different.    */
   zl303xx_BooleanE meanPathDelayValid;
   /* The meanPathDelay value calculated from the last SYNC and Delay-Request
    * messages received. The meanPathDelayValid flag should be checked to
    * determine if this value is accurate or may contain error.   */
   zl303xx_TimeStamp meanPathDelay;
} zl303xx_PtpEventServoDataS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_COUNTER_ALARM event. */
typedef struct zl303xx_PtpEventCounterAlarmS
{
   /** Handle to the stream the alarm was generated from. */
   zl303xx_PtpStreamHandleT streamHandle;
   /** Pointer to the stream's external data. */
   void *streamExtData;

   /** messageType the alarm applies to. */
   zl303xx_MessageTypeE messageType;
   /** Period of time used to measure packet counts, in seconds. */
   Uint16T periodSec;
   /** RX packet count over periodSec. */
   Uint32T rxCount;
   /** Expected RX packet count over periodSec. */
   Uint32T rxCountExpected;
} zl303xx_PtpEventCounterAlarmS;

/** Implementation specific data structure for the ZL303XX_PTP_EVENT_SQUELCH event. */
typedef struct zl303xx_PtpEventSquelchS
{
   /** Handle to the clock that should have its output signal squelched. */
   zl303xx_PtpClockHandleT clockHandle;
   /** Pointer to the clock's external data. */
   void *clockExtData;

   /** ZL303XX_FALSE indicates that the output timing signal(s) should be cut off.
    *  ZL303XX_TRUE indicates that they should be restored. This event is only
    *  generated when this flag changes state. */
   zl303xx_BooleanE enableOutput;
} zl303xx_PtpEventSquelchS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_MSG_INTVL_CHANGE event. */
typedef struct zl303xx_PtpEventMsgIntvlChangeS
{
   /** Handle to the stream the interval change was detected on. */
   zl303xx_PtpStreamHandleT streamHandle;
   /** Pointer to the stream's external data. */
   void *streamExtData;
   /** The message type that changed interval. */
   zl303xx_MessageTypeE messageType;
   /** Previous message interval. */
   Sint8T prevIntvl;
   /** New message interval. */
   Sint8T currIntvl;
} zl303xx_PtpEventMsgIntvlChangeS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_PARENT_DATA_SET_CHANGE event. */
typedef struct zl303xx_PtpEventParentDsChangeS
{
   /** Identity of the PTP Clock generating the event. */
   zl303xx_PtpClockHandleT clockHandle;
   zl303xx_ClockIdentity clockIdentity;

   /** Pointer to the clocks's external data. */
   void *clockExtData;

   /* The updated ParentDS information. */
   zl303xx_ParentDS parentDS;
} zl303xx_PtpEventParentDsChangeS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_CONTRACT_GRANTED event. */
typedef struct zl303xx_PtpEventContractGrantedS
{
   /** Stream the contract was granted on. */
   zl303xx_PtpStreamHandleT streamHandle;
   /** Pointer to the stream's external data. */
   void *streamExtData;
   /** The messageType of the contract that was granted. */
   zl303xx_MessageTypeE msgType;
   /** Indicates if the RX or TX flow expired. */
   zl303xx_PtpContractFlowE contractFlow;
   /** Value of logInterMessagePeriod from the GRANT TLV. */
   Sint8T logMessageInterval;
   /** Value of durationField from the GRANT TLV. */
   Uint32T durationSec;
   /** Value of R (Renewal Invited Boolean) from GRANT TLV. */
   zl303xx_BooleanE renewalInvited;
} zl303xx_PtpEventContractGrantedS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_CONTRACT_EXPIRED event. */
typedef struct zl303xx_PtpEventContractExpiredS
{
   /** Stream the contract expired on. */
   zl303xx_PtpStreamHandleT streamHandle;
   /** Pointer to the stream's external data. */
   void *streamExtData;
   /** The messageType of the contract that expired. */
   zl303xx_MessageTypeE msgType;
   /** Indicates if the RX or TX flow expired. */
   zl303xx_PtpContractFlowE contractFlow;
} zl303xx_PtpEventContractExpiredS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_MULTIPLE_PEER_RESP event. */
typedef struct zl303xx_PtpEventMultiplePeerRespS
{
   /** Stream the contract expired on. */
   zl303xx_PtpStreamHandleT streamHandle;
   /** Pointer to the stream's external data. */
   void *streamExtData;

   /** The receiving Port Identity (associated with the streamHandle). */
   zl303xx_PortIdentity rxPortIdentity;
   /** Pointer to the port's external data. */
   void *portExtData;

   /** The receive time stamp of the event message in PTP format. */
   zl303xx_TimeStamp rxTs;
   /** SequenceId of the PTP message associated with these time stamps. */
   Uint16T sequenceId;
   /** The transmitting Port Identity. */
   zl303xx_PortIdentity txPortIdentity;
   /** The requesting Port Identity (from within the RESP message). */
   zl303xx_PortIdentity reqPortIdentity;
} zl303xx_PtpEventMultiplePeerRespS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_UNKNOWN_TLV event. */
typedef struct zl303xx_PtpEventUnknownTlvS
{
   /** Clock Handle the Unknown TLV was received at. */
   zl303xx_PtpClockHandleT clockHandle;
   /** Port Handle the Unknown TLV was received at. */
   zl303xx_PtpPortHandleT portHandle;
   /** Stream Handle the Unknown TLV was received at
    *  (a value of All 1`s = INVALID. */
   zl303xx_PtpStreamHandleT streamHandle;

   /** Port Identity the Unknown TLV was received at. */
   zl303xx_PortIdentity receivedPortIdentity;

   /** Pointer to the clock's external data. */
   void *clockExtData;
   /** Pointer to the port's external data. */
   void *portExtData;

   /* The PTP message header and data related to the UNKNOWN TLV. */
   zl303xx_PtpV2MsgS ptpMsgRx;

   /** Length (in octets) of the received PTP message buffer. */
   Uint16T msgLength;
   /** Pointer to the received PTP message buffer. */
   void *msgBuffer;
} zl303xx_PtpEventUnknownTlvS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_PORT_FAULTY event. */
typedef struct zl303xx_PtpEventPortFaultS
{
   /** Handle to the port which went into faulty */
   zl303xx_PtpPortHandleT portHandle;
   /** Pointer to the port's external data */
   void *portExtData;
   /** The fault type which triggered this event */
   zl303xx_PtpPortFaultTypeE portFaultType;
   /** The transmit error which caused the fault */
   zl303xx_PtpTxStatusE txError;
} zl303xx_PtpEventPortFaultS;

/** Implementation specific data structure for the
 *  ZL303XX_PTP_EVENT_STREAM_DISQUALIFIED event. */
typedef struct zl303xx_PtpEventStreamDisqualifiedS
{
   /** Handle to the stream which was disqualified. */
   zl303xx_PtpStreamHandleT streamHandle;
   /** Handle to the clock where the stream currently resides. */
   zl303xx_PtpClockHandleT clockHandle;
} zl303xx_PtpEventStreamDisqualifiedS;

/** Implementation specific data structure required when setting the
 *  ParentDS of a PTP Clock. */
typedef struct zl303xx_PtpClockServerSourceS
{
   /* Keep this member 1st in the structure as the old function definition
    * only took this parameter.    */
   zl303xx_PortIdentity receivingPortIdentity;

   /* Other object handles associated with the receiving selected server. */
   zl303xx_PtpClockHandleT  receivingClockHandle;
   zl303xx_PtpStreamHandleT receivingStreamHandle;
} zl303xx_PtpClockServerSourceS;

/** Implementation specific structure used to set the productDescription. */
typedef struct zl303xx_PtpNodeProductDescriptionS
{
   /** Name of the manufacturer of the node. */
   const char *manufacturerName;
   /** The model number of the node. */
   const char *modelNumber;
   /** An identifier of the instance of this node (e.g., MAC address or serial number). */
   const char *instanceIdendifier;
} zl303xx_PtpNodeProductDescriptionS;

/** Implementation specific structure used to set the revisionData. */
typedef struct zl303xx_PtpNodeRevisionDataS
{
   /** Hardware revision number. */
   const char *hardwareRevision;
   /** Firmware revision number. */
   const char *firmwareRevision;
   /** Software revision number. */
   const char *softwareRevision;
} zl303xx_PtpNodeRevisionDataS;

/** Implementation specific structure used to set the userDescription. */
typedef struct zl303xx_PtpNodeUserDescriptionS
{
   /** A user-defined name of the device. */
   const char *deviceName;
   /** A user-defined physical location of the device. */
   const char *physicalLocation;
} zl303xx_PtpNodeUserDescriptionS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/
/* API build and release info strings */
extern const char zl303xx_PtpBuildDate[];
extern const char zl303xx_PtpBuildTime[];
extern const char zl303xx_PtpReleaseDate[];
extern const char zl303xx_PtpReleaseTime[];
extern const char zl303xx_PtpReleaseVersion[];
extern const char zl303xx_PtpReleaseSwId[];

#endif
