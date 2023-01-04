

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Interface functions for packet send/receive
*
*
*******************************************************************************/

#ifndef ZL303XX_PKT_H_
#define ZL303XX_PKT_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"

/*****************   DEFINES   ************************************************/

/* Ethernet packet specific defines */
#define ZL303XX_PACKET_MIN_PKT_SIZE         64    /* Minimum number of bytes in ethernet packet
                                                   including FCS */
#define ZL303XX_PACKET_MAX_PKT_SIZE_STD     1518  /* Max Ethernet pkt size including FCS */
#define ZL303XX_PACKET_MAX_PKT_SIZE_VLAN    (ZL303XX_PACKET_MAX_PKT_SIZE_STD + ZL303XX_PKT_VLAN_HDR_LEN)
#define ZL303XX_PACKET_ETHERNET_FCS         4     /* Size of the Ethernet Frame Check Sequence */
#define ZL303XX_PACKET_INTER_PKT_GAP_BITS   96    /* Inter packet gap in bits */
#define ZL303XX_PACKET_ETHERNET_PREAMBLE_BITS  64 /* Ethernet preamble in bits */

/* Protocol specific defines */
/* Timestamp sizes in bytes */
#define ZL303XX_TIMESTAMP_SIZE_PTP 8
#define ZL303XX_TIMESTAMP_SIZE_NTP 8

/* Ethernet header defines */
#define ZL303XX_MAC_SIZE 6      /* The size of a MAC address */
#define ZL303XX_PKT_ETHERTYPE_POS     12    /* The offset to start of the Ethertype field */
   /* Ethertypes */
#define ZL303XX_PKT_ETHERTYPE_LEN     (2)  /* Number of bytes */
#define ZL303XX_PKT_ETHERTYPE_VLAN    ((Uint16T)0x8100)
#define ZL303XX_PKT_ETHERTYPE_IPV4    ((Uint16T)0x0800)
#define ZL303XX_PKT_ETHERTYPE_IPV6    ((Uint16T)0x86DD)
#define ZL303XX_PKT_ETHERTYPE_MPLS    ((Uint16T)0x8847)

/* IP protocol */
#define ZL303XX_PKT_IP_VERSION_POS    ((Uint16T)0)    /* Offset from start of IP header */
#define ZL303XX_PKT_IP4_VERSION       ((Uint8T)0x45)  /* IPv4, normal length pkt (with no optional fields) */
#define ZL303XX_PKT_IP4_DOT_FORMAT_LENGTH      16     /* The length of an IP4 address in dot format with a
                                                       terminating NULL, e.g. "255.255.255.255" */
#define ZL303XX_PKT_IPV6_STR_LENGTH   46              /* The length of an IPv6 address with prefix length
                                                       appended (e.g., "1234::5678:ABCD/64"). */
/* IP4 next-protocol fields */
#define ZL303XX_PKT_IP_PROTOCOL_POS   ((Uint16T)9)
#define ZL303XX_PKT_IP_PROTOCOL_UDP   ((Uint16T)17)

/* Lengths of various headers */
#define ZL303XX_PKT_ETHERNET_HDR_LEN  14 /* Number of bytes in ethernet header     */
#define ZL303XX_PKT_ETHERSNAP_HDR_LEN (ZL303XX_PKT_ETHERNET_HDR_LEN + ZL303XX_PKT_SNAP_FIELD_LEN)
#define ZL303XX_PKT_SNAP_FIELD_LEN    8  /* Number of bytes in SNAP field          */
#define ZL303XX_PKT_VLAN_HDR_LEN      4  /* Number of bytes in VLAN header         */
#define ZL303XX_PKT_MPLS_HDR_LEN      4  /* Number of bytes in MPLS header         */
#define ZL303XX_PKT_IPV4_HDR_LEN      20 /* Number of bytes in IP v4 header (with no options) */
#define ZL303XX_PKT_IPV6_HDR_LEN      40 /* Number of bytes in IP v6 header        */
#define ZL303XX_PKT_UDP_HDR_LEN       8  /* Number of bytes in UDP header          */
#define ZL303XX_PKT_PW_HDR_LEN        4  /* Number of bytes in PW header           */
#define ZL303XX_MAX_TIMING_PACKET     ZL303XX_PACKET_MAX_PKT_SIZE_STD - (ZL303XX_PKT_ETHERNET_HDR_LEN + \
                                                                     ZL303XX_PKT_IPV4_HDR_LEN + \
                                                                     ZL303XX_PKT_UDP_HDR_LEN + \
                                                                     ZL303XX_PACKET_ETHERNET_FCS)

/* Protocol specific field positions/sizes */
/* UDP protocol */
#define ZL303XX_PKT_UDP_DEST_POS      2  /* -from start of UDP hdr (8-bit bytes)   */
#define ZL303XX_PKT_UDP_CS_POS        6  /* -from start of UDP hdr (8-bit bytes)   */
#define ZL303XX_PKT_PTP_EVENT_PORT   319    /* The UDP port for PTP event pkts */
#define ZL303XX_PKT_PTP_GENERAL_PORT 320    /* The UDP port for PTP general pkts */

/* PTP protocol */
#define ZL303XX_PKT_PTP_VER_POS       1     /* The offset within the PTP messages of the version number byte */
#define ZL303XX_PKT_PTP_VER1_NUM      0x01  /* The PTP version number */
#define ZL303XX_PKT_PTP_VER2_NUM      0x02  /* The PTP version number */
#define ZL303XX_PKT_PTP_SEQ_NUM_SIZE  2     /* Size of the sequence number field */

/* PTP protocol V1 */
#define ZL303XX_PKT_PTP_V1_SYNC_LEN      124 /* Number of PTP bytes in a PTP v1 SYNC message */
#define ZL303XX_PKT_PTP_V1_SEQ_NUM_POS   30    /* The offset within the PTP sync and delay_req messages
                                             of the pkt sequence number */

#define ZL303XX_PKT_PTP_V1_MSG_TYPE_POS  20    /* The offset within the PTP messages of the message type byte */
#define ZL303XX_PKT_PTP_V1_SYNC_TYPE_NUM 0x01  /* The expected message type byte for sync and delay_req messages */

#define ZL303XX_PKT_PTP_V1_UUID_POS      22    /* The offset of the UUID field in a sync or delay_req message */
#define ZL303XX_PKT_PTP_V1_UUID_SIZE     (6+2) /* The number of bytes in the PTP UUID field, including port number */

#define ZL303XX_PKT_PTP_V1_TS_POS        40    /* Offset of the 'origin timestamp' field */
#define ZL303XX_PKT_PTP_V1_TS_SIZE        8    /* Number of bytes in PTP timestamp */

/* PTP protocol V2 */
#define ZL303XX_PKT_PTP_V2_SYNC_LEN      256    /* Max Number of PTP bytes in a PTP v2 SYNC message */
#define ZL303XX_PKT_PTP_V2_SEQ_NUM_POS   30    /* The offset within the PTP sync and delay_req messages
                                             of the pkt sequence number */

#define ZL303XX_PKT_PTP_V2_MSG_TYPE_POS   0    /* The offset within the PTP messages of the message type byte */
#define ZL303XX_PKT_PTP_V2_EVENT_TYPE_MASK 0x08  /* The MS bit of the LS nibble is set in the msg type field for
                                                   timestamped events */

#define ZL303XX_PKT_PTP_V2_IDENTITY_POS  20    /* The offset of the identity field in a sync, delay_req or pdelay message */
#define ZL303XX_PKT_PTP_V2_IDENTITY_SIZE     (8+2) /* The number of bytes in the PTP identity field, including port number */

#define ZL303XX_PKT_PTP_V2_TS_POS        34    /* Offset of the 'origin timestamp' field. This is the start of the
                                                48-bit seconds and 32-bit nanoseconds fields */
#define ZL303XX_PKT_PTP_V2_TS_SIZE        8    /* Number of bytes in the 32-bit seconds and 32-bit nanoseconds parts of the
                                                timestamp field. This starts 2 bytes into the packet timestamp field */

/**********************************************************************************************************************/
/* The number of bytes in the actual received timestamp which may be inserted in/appended
   to the packet */
#define ZL303XX_PKT_RX_TS_SIZE           (Uint32T)4

/* Packet Per Second - PPS */
#define ZL303XX_MAX_PKT_PER_SEC          (Uint32T)8192
#define ZL303XX_DEFAULT_PPS              (Uint32T)64
#define ZL303XX_DEFAULT_DELAY_PPS        (Uint32T)16
#define ZL303XX_DEFAULT_ANN_PPS          (Sint32T)-2
#define ZL303XX_DEFAULT_PKT_INTERVAL     (Sint16T)4

#define ZL303XX_DEFAULT_PW_FIELD           (Uint16T)0xDEAD
#define ZL303XX_DEFAULT_MPLS_LABEL         (Uint32T)0xEDCBA000
#define ZL303XX_DEFAULT_INITIAL_MPLS_LABEL (Uint32T)0xABCDE000
/* Normal VLAN tags : 16 bits = Tagged Protocol Identifier - 3 bits = PCP - 1 bit = CFI - 12 bits = VLAN Identifier */
/* Our VLAN tags  : 16 bits = VLAN Ethertype - 3 bits = PCP - 1 bit = CFI - 12 bits = VLAN Identifier */
#define ZL303XX_DEFAULT_VLAN_TAG           (Uint32T)0x81000000
#define ZL303XX_DEFAULT_DSA_TAG            (Uint32T)0xDADA0000
#define ZL303XX_DEFAULT_QNQ_TAG            (Uint32T)0x91000000


/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */

