

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     The IEEE-1588-2008 PTP Protocol standard datatypes.
*
*******************************************************************************/

#ifndef _ZL303XX_PTP_STD_TYPES_H_
#define _ZL303XX_PTP_STD_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif


/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Int64.h"

/*****************   DEFINES   ************************************************/

/** The maximum size of the zl303xx_PortAddress::addressField array.
 * 
 *  Field sizes for transport protocols:
 *               UDP/IPv4 = 4
 *               UDP/IPv6 = 16
 *    IEEE 802.3/Ethernet = 6
 *              DeviceNET = 2
 *             ControlNET = 2
 *               PROFINET = 6
 *  */
#define ZL303XX_PORT_ADDRESS_LENGTH_MAX 16

/* Number of entries in the zl303xx_PortAddressQueryTable structure (defined below). */
#define ZL303XX_PORT_ADDRESS_QUERY_TABLE_SIZE_MAX  4
/* Number of entries in the zl303xx_AcceptableMasterTable structure (defined below). */
#define ZL303XX_ACCEPTABLE_MASTER_TABLE_SIZE_MAX   4

/* Return definitions for BMC Comparison results. */
#define ZL303XX_PTP_BMC_A_TOPOLOGICALLY_BETTER_THAN_B      (Sint8T)(2)
#define ZL303XX_PTP_BMC_A_BETTER_THAN_B                    (Sint8T)(1)
#define ZL303XX_PTP_BMC_A_EQUALS_B                         (Sint8T)(0)
#define ZL303XX_PTP_BMC_B_BETTER_THAN_A                    (Sint8T)(-1)
#define ZL303XX_PTP_BMC_B_TOPOLOGICALLY_BETTER_THAN_A      (Sint8T)(-2)

/*****************   DATA TYPES   *********************************************/

/** Type define for a 4-bit nibble. */
typedef unsigned char Uint4T;

/** Type define for the 6-byte seconds field of a PTP time stamp. */
typedef Uint64S Uint48T;

/* Structure used to support 96-bit data types */
typedef struct Sint96S
{
   Sint32T hi;
   Uint64T lo;
} Sint96S;

/* Define a type in which the upper 80-bits of a 96-bit variable represents
   nanoseconds and the lower 16-bits represents fractional nanoseconds. */
/* This new type will be needed to represent some time intervals for the 802.1AS Profile */
typedef Sint96S ScaledNs96T;

/** An IEEE-1588 defined octet array used to uniquely represent the identity
 *  of a PTP clock. The most significant octet is assigned to byte 0. */
typedef Uint8T zl303xx_ClockIdentity[8];

/** The IEEE-1588 defined Network protocol used by a port for communication.
 *  There are other reserved values which are not itemised here. 0007-EFFF is
 *  reserved for Precise Network Clock. */
typedef enum zl303xx_NetworkProtocolE
{  /** UDP//IPv4 */
   ZL303XX_NETWORK_PROTOCOL_UDP_IPv4   = (Uint16T)0x0001,
   /** UDP//IPv6 */
   ZL303XX_NETWORK_PROTOCOL_UDP_IPv6   = (Uint16T)0x0002,
   /** IEEE 802.3 */
   ZL303XX_NETWORK_PROTOCOL_802_3      = (Uint16T)0x0003,
   /** DeviceNet */
   ZL303XX_NETWORK_PROTOCOL_DEVICENET  = (Uint16T)0x0004,
   /** ControlNet */
   ZL303XX_NETWORK_PROTOCOL_CONTROLNET = (Uint16T)0x0005,
   /** PROFINET */
   ZL303XX_NETWORK_PROTOCOL_PROFINET   = (Uint16T)0x0006,
   /** Unknown Protocol*/
   ZL303XX_NETWORK_PROTOCOL_UNKNOWN    = (Uint16T)0xFFFE,
   /** A non-standard definition used for this implementation. */
   ZL303XX_NETWORK_PROTOCOL_INVALID    = ZL303XX_NETWORK_PROTOCOL_UNKNOWN
} zl303xx_NetworkProtocolE;

/** The IEEE-1588 defined accuracy of a clock. */
typedef enum zl303xx_ClockAccuracyE
{  /** The time is accurate to within 25ns */
   ZL303XX_CLOCK_ACCURACY_WITHIN_25ns    = (Uint8T)0x20,
   /** The time is accurate to within 100ns */
   ZL303XX_CLOCK_ACCURACY_WITHIN_100ns   = (Uint8T)0x21,
   /** The time is accurate to within 250ns */
   ZL303XX_CLOCK_ACCURACY_WITHIN_250ns   = (Uint8T)0x22,
   /** The time is accurate to within 1us */
   ZL303XX_CLOCK_ACCURACY_WITHIN_1us     = (Uint8T)0x23,
   /** The time is accurate to within 2.5us */
   ZL303XX_CLOCK_ACCURACY_WITHIN_2P5us   = (Uint8T)0x24,
   /** The time is accurate to within 10us */
   ZL303XX_CLOCK_ACCURACY_WITHIN_10us    = (Uint8T)0x25,
   /** The time is accurate to within 25us */
   ZL303XX_CLOCK_ACCURACY_WITHIN_25us    = (Uint8T)0x26,
   /** The time is accurate to within 100us */
   ZL303XX_CLOCK_ACCURACY_WITHIN_100us   = (Uint8T)0x27,
   /** The time is accurate to within 250us */
   ZL303XX_CLOCK_ACCURACY_WITHIN_250us   = (Uint8T)0x28,
   /** The time is accurate to within 1ms */
   ZL303XX_CLOCK_ACCURACY_WITHIN_1ms     = (Uint8T)0x29,
   /** The time is accurate to within 2.5ms */
   ZL303XX_CLOCK_ACCURACY_WITHIN_2P5ms   = (Uint8T)0x2A,
   /** The time is accurate to within 10ms */
   ZL303XX_CLOCK_ACCURACY_WITHIN_10ms    = (Uint8T)0x2B,
   /** The time is accurate to within 25ms */
   ZL303XX_CLOCK_ACCURACY_WITHIN_25ms    = (Uint8T)0x2C,
   /** The time is accurate to within 100ms */
   ZL303XX_CLOCK_ACCURACY_WITHIN_100ms   = (Uint8T)0x2D,
   /** The time is accurate to within 250ms */
   ZL303XX_CLOCK_ACCURACY_WITHIN_250ms   = (Uint8T)0x2E,
   /** The time is accurate to within 1s */
   ZL303XX_CLOCK_ACCURACY_WITHIN_1s      = (Uint8T)0x2F,
   /** The time is accurate to within 10s */
   ZL303XX_CLOCK_ACCURACY_WITHIN_10s     = (Uint8T)0x30,
   /** The time is accurate to > 10s */
   ZL303XX_CLOCK_ACCURACY_WORSE_THAN_10s = (Uint8T)0x31,
   /** The time accuracy is unknown */
   ZL303XX_CLOCK_ACCURACY_UNKNOWN        = (Uint8T)0xFE
} zl303xx_ClockAccuracyE;

/** The IEEE-1588 defined sources of time distributed by a grandmaster clock. */
typedef enum zl303xx_TimeSourceE
{  /** An Atomic Clock */
   ZL303XX_TIME_SOURCE_ATOMIC_CLOCK         = (Uint8T)0x10,
   /** A GPS satellite system */
   ZL303XX_TIME_SOURCE_GPS                  = (Uint8T)0x20,
   /** A radio distribution system */
   ZL303XX_TIME_SOURCE_TERRESTRIAL_RADIO    = (Uint8T)0x30,
   /** PTP network that is not this PTP domain */
   ZL303XX_TIME_SOURCE_PTP                  = (Uint8T)0x40,
   /** An NTP or SNTP network */
   ZL303XX_TIME_SOURCE_NTP                  = (Uint8T)0x50,
   /** Time set via human interface */
   ZL303XX_TIME_SOURCE_HAND_SET             = (Uint8T)0x60,
   /** Some other (or unknown) source */
   ZL303XX_TIME_SOURCE_OTHER                = (Uint8T)0x90,
   /** Free-running oscillator */
   ZL303XX_TIME_SOURCE_INTERNAL_OSCILLATOR  = (Uint8T)0xA0
} zl303xx_TimeSourceE;

/** The IEEE-1588 defined states of a port protocol engine. */
typedef enum zl303xx_PortStateE
{  /** The port is INITIALIZING */
   ZL303XX_PORT_STATE_INITIALIZING = (Uint8T)0x01,
   /** The port has encountered a FAULT */
   ZL303XX_PORT_STATE_FAULTY       = (Uint8T)0x02,
   /** The port has been DISABLED */
   ZL303XX_PORT_STATE_DISABLED     = (Uint8T)0x03,
   /** The port is LISTENING for valid servers in the network */
   ZL303XX_PORT_STATE_LISTENING    = (Uint8T)0x04,
   /** The port is PREparing for MASTER operation */
   ZL303XX_PORT_STATE_PRE_MASTER   = (Uint8T)0x05,
   /** The port is operating as a MASTER */
   ZL303XX_PORT_STATE_MASTER       = (Uint8T)0x06,
   /** The port is in PASSIVE mode */
   ZL303XX_PORT_STATE_PASSIVE      = (Uint8T)0x07,
   /** The port is UNCALIBRATED (similar to a slave but not affecting the local clock) */
   ZL303XX_PORT_STATE_UNCALIBRATED = (Uint8T)0x08,
   /** The port is operating as a SLAVE (driving the local clock) */
   ZL303XX_PORT_STATE_SLAVE        = (Uint8T)0x09
} zl303xx_PortStateE;

/** The IEEE-1588 defined options for measuring propagation delay. */
typedef enum zl303xx_DelayMechanismE
{
   /** Port using end-to-end propagation delay measurement. */
   ZL303XX_DELAY_MECHANISM_E2E      = (Uint8T)0x01,
   /** Port using peer-to-peer propagation delay measurement. */
   ZL303XX_DELAY_MECHANISM_P2P      = (Uint8T)0x02,
   /** Port does not implement delay measurement. */
   ZL303XX_DELAY_MECHANISM_DISABLED = (Uint8T)0xFE
} zl303xx_DelayMechanismE;

/** The IEEE-1588 defined Management Error definitions. */
typedef enum
{
   /** Reserved value. */
   ZL303XX_MGMT_ERR_NO_ERROR             = 0x0000,
   /** The requested operation could not fit in a single response message. */
   ZL303XX_MGMT_ERR_RESPONSE_TOO_BIG     = 0x0001,
   /** The managementId is not recognized. */
   ZL303XX_MGMT_ERR_NO_SUCH_ID           = 0x0002,
   /** The managementId was identified but the length of the data was wrong. */
   ZL303XX_MGMT_ERR_WRONG_LENGTH         = 0x0003,
   /** The managementId and length were correct but one or more values were wrong. */
   ZL303XX_MGMT_ERR_WRONG_VALUE          = 0x0004,
   /** Some of the variables in the set command were not updated because they are not configurable. */
   ZL303XX_MGMT_ERR_NOT_SETABLE          = 0x0005,
   /** The requested operation is not supported in this node. */
   ZL303XX_MGMT_ERR_NOT_SUPPORTED        = 0x0006,
   /** An error occurred that is not covered by other managementErrorId values. */
   ZL303XX_MGMT_ERR_GENERAL_ERROR        = 0xFFFE
} zl303xx_ManagementErrorIdE;

/** The IEEE-1588 defined Message Type definitions. */
typedef enum zl303xx_MessageTypeE
{
   /* Event Messages */
   /** SYNC */
   ZL303XX_MSG_ID_SYNC                = (Uint4T)0x0,
   /** DELAY REQUEST */
   ZL303XX_MSG_ID_DELAY_REQ           = (Uint4T)0x1,
   /** PEER-DELAY REQUEST */
   ZL303XX_MSG_ID_PEER_DELAY_REQ      = (Uint4T)0x2,
   /** PEER-DELAY RESPONSE */
   ZL303XX_MSG_ID_PEER_DELAY_RESP     = (Uint4T)0x3,

   /* Values 4-7 are reserved */

   /* General Messages */
   /** FOLLOW-UP */
   ZL303XX_MSG_ID_FOLLOWUP            = (Uint4T)0x8,
   /** DELAY RESPONSE */
   ZL303XX_MSG_ID_DELAY_RESP          = (Uint4T)0x9,
   /** PEER-DELAY FOLLOW-UP */
   ZL303XX_MSG_ID_PEER_DELAY_FOLLOWUP = (Uint4T)0xA,
   /** ANNOUNCE */
   ZL303XX_MSG_ID_ANNOUNCE            = (Uint4T)0xB,
   /** SIGNALING */
   ZL303XX_MSG_ID_SIGNALING           = (Uint4T)0xC,
   /** MANAGEMENT */
   ZL303XX_MSG_ID_MANAGEMENT          = (Uint4T)0xD
} zl303xx_MessageTypeE;

/*****************   DATA STRUCTURES   ****************************************/

/** An IEEE-1588 defined representation of a time interval. */
typedef struct zl303xx_TimeInterval
{
   /** The time interval expressed in units of nanoseconds and multiplied by
    *  2<SUP>16</SUP> (e.g., fractional nanoseconds are stored in the bottom
    *  16 bits). */
   Uint64S scaledNanoseconds;
} zl303xx_TimeInterval;

/* An IEEE 802.1AS define to store the frequency ratio between local and remote clock */
typedef struct zl303xx_RateRatio
{
   /** The measured ratio of the frequency of the LocalClock entity of the timeaware
      system at the other end of the link attached to this port, to the frequency of the LocalClock entity of
      this time-aware system */
   Uint64S rateRatio;

   /* A Boolean variable that indicates whether or not
   we successfully computed RateRatio */
   zl303xx_BooleanE rateRatioValid;
} zl303xx_RateRatio;

/** An IEEE-1588 defined representation of a positive time with respect to the epoch. */
typedef struct zl303xx_TimeStamp
{
   /** Integer portion of the timestamp in units of seconds. */
   Uint48T secondsField;

   /** Fractional portion of the timestamp in units of nanoseconds. This field
    *  is always less than 10<SUP>9</SUP>. */
   Uint32T nanosecondsField;
} zl303xx_TimeStamp;

/** The IEEE-1588 defined representation of a PTP-V2 port identity. */
typedef struct zl303xx_PortIdentity
{
   /** Identity of the PTP clock this port is attached to. */
   zl303xx_ClockIdentity clockIdentity;

   /** A number that uniquely identifies this PTP port on the clock. This value is
    *  always 1 or greater. In general, for a PTP clock that supports N ports, this
    *  value is:
    *   1 <= portNumber <= N  */
   Uint16T portNumber;
} zl303xx_PortIdentity;

/** The IEEE-1588 defined protocol address of a PTP port. */
typedef struct zl303xx_PortAddress
{
   /** Identifies the protocol used by the communication path this PTP port is
    *  attached to. */
   zl303xx_NetworkProtocolE networkProtocol;

   /** The length of the address in octets. */
   Uint16T addressLength;

   /** The protocol address of a port in the format defined by the mapping annex
    *  of the protocol. The most significant octet of the address is mapped
    *  into index 0. */
   Uint8T addressField[ZL303XX_PORT_ADDRESS_LENGTH_MAX];
} zl303xx_PortAddress;

/** The IEEE-1588 defined quality parameters of a PTP clock. This structure
 *  is stored in the clock's zl303xx_DefaultDS::clockQuality field. The fields
 *  should be updated whenever the local hardware clock (i.e. electrical
 *  reference) changes or the PTP timing server changes. */
typedef struct zl303xx_ClockQuality
{
   /** Defines the traceability of the time or frequency distributed by the
    *  grandmaster clock. */
   Uint8T clockClass;

   /** Indicates the expected accuracy of a clock when it is the grandmaster,
    *  or in the event it becomes the grandmaster, for the purposes of the Best
    *  Master Clock Algorithm. */
   zl303xx_ClockAccuracyE clockAccuracy;

   /** The precision of the timestamps included in messages issued by the clock
    *  when it is not synchronized to another clock. */
   Uint16T offsetScaledLogVariance;
} zl303xx_ClockQuality;

/** The IEEE-1588 defined data set to describe the performance (or capability)
 *  attributes for the local PTP clock. */
typedef struct zl303xx_DefaultDS
{
   /*** STATIC MEMBERS ***/
   /** A static member defining if this clock provides timing information
    *  using event and subsequent general messages (i.e., SYNC and then a
    *  FOLLOW_UP message). */
   zl303xx_BooleanE twoStepFlag;
   /** A static member defining the identity of the local clock. Index 0 is
    *  the most significant byte.*/
   zl303xx_ClockIdentity clockIdentity;
   /** A static member defining the maximum number of PTP ports on the clock.
    * Set to 0 to automatically filled with the greatest value of
    * zl303xx_PortDS::portIdentity::portNumber on the clock. */
   Uint16T numberPorts;

   /*** DYNAMIC MEMBERS ***/
   /** A dynamic member defining the quality of the local clock. Refer to the
    *  zl303xx_ClockQuality structure definition for sub-member descriptions. */
   zl303xx_ClockQuality clockQuality;

   /*** CONFIGURABLE MEMBERS ***/
   /** A configurable member defining the priority1 attribute of the local
    *  clock, used in the Best Master Clock Algorithm.
    *  Values: 0 (highest priority) - 255 (lowest priority). */
   Uint8T priority1;
   /** A configurable member defining the priority2 attribute of the local
    *  clock, used in the Best Master Clock Algorithm.
    *  Values: 0 (highest priority) - 255 (lowest priority). */
   Uint8T priority2;
   /** A configurable member defining the domain attribute of the local
    *  clock. Messages from other domains will be ignored.
    *  Values: 0 - 127. */
   Uint8T domainNumber;
   /** A configurable member that defines if the clock can ever enter the
    *  MASTER state. */
   zl303xx_BooleanE slaveOnly;
} zl303xx_DefaultDS;

/** An IEEE-1588 defined data set containing attributes related to clock
 *  synchronization. All members of this data set are dynamic. */
typedef struct zl303xx_CurrentDS
{
   /** The number of communication paths traversed between the local clock and
    *  the grandmaster clock. */
   Uint16T stepsRemoved;

   /** An implementation-specific representation of the current value of the
    *  time difference between a master and a slave as computed by the slave. */
   zl303xx_TimeInterval offsetFromMaster;

   /** An implementation-specific representation of the current value of the
    *  mean propagation time between a master and slave clock as computed by
    *  the slave. */
   zl303xx_TimeInterval meanPathDelay;
} zl303xx_CurrentDS;

/** An IEEE-1588 defined data set containing attributes describing the parent
 *  (the clock to which the local clock synchronizes) and the grandmaster (the
 *  clock at the root of the master-slave hierarchy). All members of this data set
 *  are dynamic. */
typedef struct zl303xx_ParentDS
{
   /** The identity of the port on the master that issues the SYNC messages used
    *  in synchronizing this clock. */
   zl303xx_PortIdentity parentPortIdentity;

   /** This member is ZL303XX_TRUE if he local clock has a slave port and has computed
    *  statistically valid estimates of the zl303xx_ParentDS::observedParentOffsetScaledLogVariance
    *  and the zl303xx_ParentDS::observedParentClockPhaseChangeRate members. */
   zl303xx_BooleanE parentStats;

   /** An estimate of the parent clock's PTP variance as observed by the slave
    *  clock. */
   Uint16T observedParentOffsetScaledLogVariance;

   /** An estimate of the parent clock's phase change rate as observed by the
    *  slave clock. */
   Sint32T observedParentClockPhaseChangeRate;

   /** The identity of the grandmaster clock. */
   zl303xx_ClockIdentity grandmasterIdentity;

   /** The quality of the grandmaster clock. */
   zl303xx_ClockQuality grandmasterClockQuality;

   /** The priority1 attribute of the grandmaster clock, used in the Best Master Clock Algorithm. */
   Uint8T grandmasterPriority1;
   /** The priority2 attribute of the grandmaster clock, used in the Best Master Clock Algorithm. */
   Uint8T grandmasterPriority2;
} zl303xx_ParentDS;

/** An IEEE-1588 defined data set containing attributes about the PTP timescale
 *  being used. All members of this data set are dynamic. */
typedef struct zl303xx_TimePropertiesDS
{
   /** The offset between TAI and UTC. This values is only meaningful in PTP
    *  systems whose epoch is the PTP epoch. This value is in units of seconds. */
   Sint16T currentUtcOffset;

   /** This member is ZL303XX_TRUE if zl303xx_TimePropertiesDS::currentUtcOffset is known to
    *  be correct. Otherwise, it is ZL303XX_FALSE. */
   zl303xx_BooleanE currentUtcOffsetValid;

   /** This member is ZL303XX_TRUE if the epoch is the PTP epoch, and the last minute
    *  of the current UTC day contains 59 seconds. Otherwise, it is ZL303XX_FALSE. */
   zl303xx_BooleanE leap59;

   /** This member is ZL303XX_TRUE if the epoch is the PTP epoch, and the last minute
    *  of the current UTC day contains 61 seconds. Otherwise, it is ZL303XX_FALSE. */
   zl303xx_BooleanE leap61;

   /** This member is ZL303XX_TRUE if the timescale and the value of
    *  zl303xx_TimePropertiesDS::currentUtcOffset is traceable to a primary reference.
    *  Otherwise, it is ZL303XX_FALSE. */
   zl303xx_BooleanE timeTraceable;

   /** This member is ZL303XX_TRUE if the frequency determining the timescale is
    *  traceable to a primary reference. Otherwise, it is ZL303XX_FALSE. */
   zl303xx_BooleanE frequencyTraceable;

   /** This member is ZL303XX_TRUE if the clock timescale of the grandmaster clock
    *  is PTP. Otherwise, it is ZL303XX_FALSE. */
   zl303xx_BooleanE ptpTimescale;

   /** The source of time used by the grandmaster clock. */
   zl303xx_TimeSourceE timeSource;

   /** This is an optional member and is ZL303XX_TRUE if the clock is in the
    *  process of synchronizing to the time source. Otherwise, the synchronization
    *  is complete and this value is ZL303XX_FALSE. When not used, this value is
    *  also ZL303XX_FALSE. */
   zl303xx_BooleanE synchronizationUncertain;

} zl303xx_TimePropertiesDS;

/** An IEEE-1588 defined data set containing attributes related to a single
 *  port of a PTP clock. */
typedef struct zl303xx_PortDS
{
   /*** STATIC MEMBERS ***/
   /** A static member defining the identity of the local port. */
   zl303xx_PortIdentity portIdentity;

   /*** DYNAMIC MEMBERS ***/
   /** A dynamic member defining the current state of the protocol engine
    *  associated with this port. */
   zl303xx_PortStateE portState;
   /** A dynamic member defining the log2 of the minimum permitted mean time interval
    *  between successive DELAY_REQ messages sent by a slave to a specific port
    *  on a master. This value is determined and advertised by a master clock
    *  based on the ability of the master clock to process the DELAY_REQ message traffic. */
   Sint8T logMinDelayReqInterval;
   /** A dynamic member defining an estimate of the current one-way
    *  propagation delay on the link, if the value of zl303xx_PortDS::delayMechanism
    *  is DELAY_MECHANISM_P2P. Otherwise, this is 0.  */
   zl303xx_TimeInterval peerMeanPathDelay;
   /** A dynamic member defining an estimate of the current frequency ratio
       *  on the link, if the value of zl303xx_PortDS::delayMechanism
       *  is DELAY_MECHANISM_P2P and the current profile is TSN.
       *  Otherwise, this is 0.  */
   zl303xx_RateRatio neighbourRateRatio;

   /*** CONFIGURABLE MEMBERS ***/
   /** A configurable member defining the log2 of the mean time interval
    *  between successive ANNOUNCE messages. This value should be uniform
    *  throughout a domain.
    *  Notes: This value is only valid if the port is transmitting messages. */
   Sint8T logAnnounceInterval;
   /** A configurable member defining the number of announceIntervals that
    *  have to pass without receipt of an ANNOUNCE message before the generation
    *  of an ANNOUNCE_RECEIPT_TIMEOUT_EXPIRES event. Range: 2-255.
    *  Notes: This value is only valid if the port is transmitting messages. */
   Uint8T announceReceiptTimeout;
   /** A configurable member that specifies the mean time interval between
    *  successive SYNC messages when transmitted as multicast messages.
    *  Notes: This value is only valid if the port is transmitting messages. */
   Sint8T logSyncInterval;
   /** A configurable member that indicates the propagation delay measuring option. */
   zl303xx_DelayMechanismE delayMechanism;
   /** A configurable member that specifies the minimum permitted mean time
    *  interval between successive PDELAY_REQ messages.  */
   Sint8T logMinPdelayReqInterval;
   /** A configurable member indicating the PTP version in use on this port. */
   Uint16T versionNumber;
} zl303xx_PortDS;

/** The IEEE-1588 defined data set to describe configurable attributes for
 *  optional functionality of the local PTP clock. */
typedef struct zl303xx_OptionalDefaultDS
{
   /*** STATIC MEMBERS ***/

   /*** DYNAMIC MEMBERS ***/

   /*** CONFIGURABLE MEMBERS ***/
   /** Parameter to determine the maximum value of the stepsRemoved member of
    *  a received ANNOUNCE message that the local clock will accept. Received
    *  ANNOUNCE message with a stepsRemoved value less than the configured
    *  MAX will be accepted by the receiving node.
    *  By default, this is set to 255 as per IEEE-1588-2008. */
   Uint16T maxStepsRemoved;
} zl303xx_OptionalDefaultDS;

/** The IEEE-1588 defined data set to describe configurable attributes for
 *  optional functionality of the local PTP Port. */
typedef struct zl303xx_OptionalPortDS
{
   /*** STATIC MEMBERS ***/

   /*** DYNAMIC MEMBERS ***/
   /** Optional parameter indicating if a Port is experiencing a Signal Fail
    *  condition. Updating of this field is implementation specific (typically
    *  based on profile).
    *  Values:
    *  ZL303XX_FALSE (Port may participate in BMCA selection)
    *  ZL303XX_TRUE  (Port should not participate in BMCA selection) */
   zl303xx_BooleanE signalFail;

   /*** CONFIGURABLE MEMBERS ***/
   /** Optional parameter to prohibit a Port from entering the SLAVE or PASSIVE
    *  state. Any ANNOUNCE messages received on a masterOnly Port are not used
    *  in the Best Master Clock Algorithm of the profile.
    *  Values:
    *  ZL303XX_FALSE (default operation) or ZL303XX_TRUE (MASTER_ONLY mode). */
   zl303xx_BooleanE masterOnly;

   /** Optional parameter to prohibit a Port from Granting Unicast service
    *  requests. Set to ZL303XX_FALSE (default value) to grant a contract request;
    *  set to ZL303XX_TRUE to deny requests received by this Port. */
   zl303xx_BooleanE denyServiceRequests;
} zl303xx_OptionalPortDS;

/** An IEEE-1588 defined data set describing a local PTP transparent clock. */
typedef struct zl303xx_TransparentClockDefaultDS
{
   /*** STATIC MEMBERS ***/
   /** A static member defining the identity of the local clock. */
   zl303xx_ClockIdentity clockIdentity;
   /** A static member defining the number of PTP ports on the device. */
   Uint16T numberPorts;

   /*** CONFIGURABLE MEMBERS ***/
   /** A configurable member that indicates the propagation delay measuring option. */
   zl303xx_DelayMechanismE delayMechanism;
   /** A configurable member indicating the domainNumber of the primary
    *  syntonization domain. */
   Uint8T primaryDomain;
} zl303xx_TransparentClockDefaultDS;

/** An IEEE-1588 defined data set containing attributes related to each port
 *  of a PTP transparent clock. */
typedef struct zl303xx_TransparentClockPortDS
{
   /*** STATIC MEMBERS ***/
   /** A static member defining the identity of the local port. */
   zl303xx_PortIdentity portIdentity;

   /*** DYNAMIC MEMBERS ***/
   /** A dynamic member that specifies the minimum permitted mean time
    *  interval between successive PDELAY_REQ messages. */
   Sint8T logMinPdelayReqInterval;
   /** A dynamic member that is ZL303XX_TRUE if the port is faulty, or ZL303XX_FALSE
    *  if the port is operating normally. */
   zl303xx_BooleanE faultyFlag;
   /** A dynamic member defining an estimate of the current one-way
    *  propagation delay on the link, if the value of
    *  zl303xx_TransparentClockDefaultDS::delayMechanism is DELAY_MECHANISM_P2P.
    *  Otherwise, this is 0.*/
   zl303xx_TimeInterval peerMeanPathDelay;
} zl303xx_TransparentClockPortDS;

/** An IEEE-1588 defined table of port addresses along with the query interval. */
typedef struct zl303xx_PortAddressQueryTable
{
   /** Maximum table size. */
   Uint16T maxTableSize;
   /** Logarithm to the base 2 of the mean interval in seconds between unicast
    *  Announce messages. */
   Sint8T logQueryInterval;
   /** Number of entries in the table. */
   Uint16T actualTableSize;
   /** Array of port addresses. */
   zl303xx_PortAddress portAddress[ZL303XX_PORT_ADDRESS_QUERY_TABLE_SIZE_MAX];
} zl303xx_PortAddressQueryTable;

/** An IEEE-1588 defined data structure that describes an entry in an
 *  acceptable master table. */
typedef struct zl303xx_AcceptableMaster
{
   /** Port address (including protocol & network address) of an acceptable master. */
   zl303xx_PortAddress acceptableAddress;
   /** Replaces the value of the priority1 field in an ANNOUNCE message from
    *  this master in the Best Master Clock Algorithm. If set to 0, no replacement
    *  is done. */
   Uint8T alternatePriority1;
} zl303xx_AcceptableMaster;

/** An IEEE-1588 defined table used to store master addresses that messages
 *  can be received from. */
typedef struct zl303xx_AcceptableMasterTable
{
   /** Maximum table size. */
   Uint16T maxTableSize;
   /** Number of entries in the table. */
   Uint16T actualTableSize;
   /** Array of acceptable masters. */
   zl303xx_AcceptableMaster acceptableMaster[ZL303XX_ACCEPTABLE_MASTER_TABLE_SIZE_MAX];
} zl303xx_AcceptableMasterTable;


/** An IEEE-1588 defined structure to store standardized PTP profile info.  */
typedef struct zl303xx_PtpProfileS
{
   const char   *profileName;
   const char   *profileVersion;
   const Uint8T  profileIdentifier[6];
   const char   *organizationName;
   const char   *sourceIdentification;
} zl303xx_PtpProfileS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */

