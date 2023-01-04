

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains compile time defines used by the Zarlink PTP API.
*
*******************************************************************************/

#ifndef _ZL303XX_PTP_SETUP_H_
#define _ZL303XX_PTP_SETUP_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_TSALimits.h"

/*****************   DEFINES   ************************************************/
/** Maximum number of PTP clock objects per node. */

#if !defined ZL303XX_PTP_NUM_CLOCKS_MAX
#define ZL303XX_PTP_NUM_CLOCKS_MAX                  4
#endif
/** Maximum number of PTP port objects per node. */

#if !defined ZL303XX_PTP_NUM_PORTS_MAX
#define ZL303XX_PTP_NUM_PORTS_MAX                   8
#endif
/** Maximum number of PTP stream objects per node. */

#if !defined ZL303XX_PTP_NUM_STREAMS_MAX
#define ZL303XX_PTP_NUM_STREAMS_MAX                 128
#endif

/* Timer task configuration. */
#define ZL303XX_PTP_TIMER_TASK_NAME                 "tPtpTimerzl303xx"
#define ZL303XX_PTP_TIMER_TASK_STACK_SIZE           9000

/* ZL303XX_PTP_TIMER_TASK_INTVL_MS is somewhat of a "magic number" and should
 * not be changed. This number is chosen such that the 1588-2008 requirement
 * that messages must be sent with an interval that is +/-30% of the
 * configured value with 90% confidence is met up to 128pps.
 *
 * At 128pps, the desired mean message interval is ~7.8ms. The error allowed
 * is +/- 2.34ms. Using an 8ms timer to wake up the task, the message interval
 * error is ~0.2ms. To keep the mean interval at 7.8ms, occasionally two
 * messages must be sent back-to-back. However, this happens for less than 10%
 * of the messages, so the 90% confidence requirement will be met.
 *
 * Note: The OS kernel must be configured with a tick rate of 250Hz = 4ms
 * tick interval. This ensures that the PTP timer task is runs every 2 kernel
 * ticks. */
#define ZL303XX_PTP_TIMER_TASK_INTVL_MS             8

#ifdef OS_LINUX
   #define ZL303XX_PTP_TIMER_TASK_PRIORITY          96
#elif OS_FREERTOS
   #define ZL303XX_PTP_TIMER_TASK_PRIORITY        18   //ML_INTEGRATION not in Danns version
#else
   #define ZL303XX_PTP_TIMER_TASK_PRIORITY        64
#endif

/* Node initialization defaults. */
#define ZL303XX_PTP_DEFAULT_NODE_MANUFAC_NAME       ZL303XX_MICROSEMI_NAME
#define ZL303XX_PTP_DEFAULT_NODE_MODEL_NUM          ""
#define ZL303XX_PTP_DEFAULT_NODE_INSTANCE_ID        ""
#define ZL303XX_PTP_DEFAULT_NODE_HW_REV             ""
#define ZL303XX_PTP_DEFAULT_NODE_FW_REV             ""
#define ZL303XX_PTP_DEFAULT_NODE_SW_REV             zl303xx_PtpReleaseVersion
#define ZL303XX_PTP_DEFAULT_NODE_DEV_NAME           ""
#define ZL303XX_PTP_DEFAULT_NODE_PHYS_LOC           ""

/* Clock initialization defaults. */
#define ZL303XX_PTP_DEFAULT_CLOCK_TASK_NAME         "tPtpClock"
#define ZL303XX_PTP_DEFAULT_CLOCK_TASK_STACK_SIZE   9000

#if defined OS_VXWORKS
   #define ZL303XX_PTP_DEFAULT_CLOCK_TASK_PRIORITY   ZL303XX_PTP_TIMER_TASK_PRIORITY + 1
#elif OS_LINUX
   #define ZL303XX_PTP_DEFAULT_CLOCK_TASK_PRIORITY   ZL303XX_PTP_TIMER_TASK_PRIORITY
#elif defined(OS_FREERTOS)
   #define ZL303XX_PTP_DEFAULT_CLOCK_TASK_PRIORITY   ZL303XX_PTP_TIMER_TASK_PRIORITY + 2
#else
    #error ZL303XX_PTP_DEFAULT_CLOCK_TASK_PRIORITY NOT DEFINED for unknown OS
#endif

#define ZL303XX_PTP_DEFAULT_CLOCK_MSG_Q_LENGTH      128
#define ZL303XX_PTP_DEFAULT_CLOCK_TYPE              0       /* Auto fill */
#define ZL303XX_PTP_DEFAULT_CLOCK_PROFILE_STRICT    ZL303XX_FALSE
#define ZL303XX_PTP_DEFAULT_CLOCK_SLAVE_ONLY        ZL303XX_FALSE
#define ZL303XX_PTP_DEFAULT_CLOCK_TWO_STEP          ZL303XX_FALSE
#define ZL303XX_PTP_DEFAULT_CLOCK_PRI_1             128     /* From IEEE 1588 version 2, Annex J */
#define ZL303XX_PTP_DEFAULT_CLOCK_PRI_2             128     /* From IEEE 1588 version 2, Annex J */
#define ZL303XX_PTP_DEFAULT_CLOCK_DOMAIN_NUM        0
#define ZL303XX_PTP_DEFAULT_CLOCK_NUM_PORTS         0       /* Auto fill */
#define ZL303XX_PTP_DEFAULT_CLOCK_CLASS             13
#define ZL303XX_PTP_DEFAULT_CLOCK_ACCURACY          ZL303XX_CLOCK_ACCURACY_WORSE_THAN_10s
#define ZL303XX_PTP_DEFAULT_CLOCK_SCALED_LOG_VAR    0xCD70
#define ZL303XX_PTP_DEFAULT_CLOCK_REVERTIVE_EN      ZL303XX_TRUE
#define ZL303XX_PTP_DEFAULT_CLOCK_WTR               0
#define ZL303XX_PTP_DEFAULT_CLOCK_QL_HOLD_OFF       0

#define ZL303XX_PTP_DEFAULT_CLOCK_EVT_SQUELCH_EN        ZL303XX_FALSE
#define ZL303XX_PTP_DEFAULT_CLOCK_EVT_BMCA_UPDATE_SEC   8  /* 4 * 2 Seconds */
#define ZL303XX_PTP_DEFAULT_CLOCK_EVT_BMCA_TRIGGER      ZL303XX_BMCA_TRIGGER_MIN_ANNC_INTVL

#define ZL303XX_PTP_DEFAULT_CLOCK_MAX_FOREIGN_REC       8     /* Foreign Master Table Size */
#define ZL303XX_PTP_DEFAULT_CLOCK_STEPS_REMOVED_MAX     255
#define ZL303XX_PTP_DEFAULT_CLOCK_PATH_TRACE_ENABLED    ZL303XX_FALSE
#define ZL303XX_PTP_DEFAULT_CLOCK_SYNCHRO_UNCERTAIN_EN  ZL303XX_FALSE
#define ZL303XX_PTP_DEFAULT_CLOCK_UNI_NEG_ANNC_MAX      -1    /* Max number of announce contracts */
#define ZL303XX_PTP_DEFAULT_CLOCK_UNI_NEG_SYNC_MAX      -1    /* Max number of sync contracts */
#define ZL303XX_PTP_DEFAULT_CLOCK_UNI_NEG_DELAY_MAX     -1    /* Max number of delay resp contracts */
#define ZL303XX_PTP_DEFAULT_CLOCK_UNI_NEG_PDELAY_MAX    -1    /* Max number of peer delay resp contracts */
#define ZL303XX_PTP_DEFAULT_CLOCK_UNI_NEG_LIMIT_SYNC    ZL303XX_PTP_PPS_UNLIMITED
#define ZL303XX_PTP_DEFAULT_CLOCK_UNI_NEG_LIMIT_ANNC    ZL303XX_PTP_PPS_UNLIMITED
#define ZL303XX_PTP_DEFAULT_CLOCK_UNI_NEG_LIMIT_DELAY   ZL303XX_PTP_PPS_UNLIMITED
#define ZL303XX_PTP_DEFAULT_CLOCK_UNI_NEG_LIMIT_PDELAY  ZL303XX_PTP_PPS_UNLIMITED

#define ZL303XX_PTP_DEFAULT_CLOCK_FORCE_DELETE      ZL303XX_TRUE

/* Port initialization defaults. */
#define ZL303XX_PTP_DEFAULT_PORT_DELAY_MECH          ZL303XX_DELAY_MECHANISM_E2E
#define ZL303XX_PTP_DEFAULT_PORT_PDELAY_RESP_METHOD  ZL303XX_PTP_PDELAY_METHOD_2_STEP_RESIDENT
#define ZL303XX_PTP_DEFAULT_PORT_PTP_VERSION         2
#define ZL303XX_PTP_DEFAULT_PORT_MAX_FOREIGN_REC     8                    /* Should align with ZL303XX_APR_MAX_NUM_MASTERS in Apr.h */
#define ZL303XX_PTP_DEFAULT_PORT_ACC_MASTER_TBL      ZL303XX_FALSE
#define ZL303XX_PTP_DEFAULT_PORT_ACC_MASTER_TBL_LEN  8                    /* Should align with ZL303XX_APR_MAX_NUM_MASTERS in Apr.h */
#define ZL303XX_PTP_DEFAULT_PORT_ACC_SLAVE_TBL       ZL303XX_FALSE
#define ZL303XX_PTP_DEFAULT_PORT_MSG_TRANSPORT       ZL303XX_PTP_MSG_TRANSPORT_MULTICAST
#define ZL303XX_PTP_DEFAULT_PORT_MIXED_MODE          ZL303XX_FALSE
#define ZL303XX_PTP_DEFAULT_PORT_ALT_MASTER          ZL303XX_FALSE

#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_ENABLED          ZL303XX_FALSE

#if defined ZL303XX_TEST_THROUGHPUT
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_ANNC_RATE_MAX    -4    /* Max of 16 announce msgs/sec */
#else
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_ANNC_RATE_MAX    -3    /* Max of 8 announce msgs/sec */
#endif
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_ANNC_GRANT_MAX   1000
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_ANNC_MAX         -1    /* Max number of announce contracts */
#if defined ZL303XX_TEST_THROUGHPUT
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_SYNC_RATE_MAX    -13    /* Max of 8192 sync msgs/sec */
#else
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_SYNC_RATE_MAX    -7     /* Max of 128 sync msgs/sec */
#endif

#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_SYNC_GRANT_MAX   1000
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_SYNC_MAX         -1    /* Max number of sync contracts */
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_DELAY_RATE_MAX   -7    /* Max of 128 delay_resp msgs/sec */
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_DELAY_GRANT_MAX  1000
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_DELAY_MAX        -1    /* Max number of delay resp contracts */
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_PDELAY_RATE_MAX  -7    /* Max of 128 pdelay_resp msgs/sec */
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_PDELAY_GRANT_MAX 1000
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_PDELAY_MAX       -1    /* Max number of pdelay resp contracts */
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_STACKED_TLV_EN   ZL303XX_FALSE  /* Use single TLVs by default */
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_LIMIT_SYNC       ZL303XX_PTP_PPS_UNLIMITED
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_LIMIT_ANNC       ZL303XX_PTP_PPS_UNLIMITED
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_LIMIT_DELAY      ZL303XX_PTP_PPS_UNLIMITED
#define ZL303XX_PTP_DEFAULT_PORT_DENY_UNICAST_REQUESTS    ZL303XX_FALSE
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_GRANT_TBL_LEN    5
#define ZL303XX_PTP_DEFAULT_PORT_NEIGH_RATE_RATIO_VALID   ZL303XX_FALSE
/* DEPRECATED */
#define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_GRANT            !(ZL303XX_PTP_DEFAULT_PORT_DENY_UNICAST_REQUESTS)

#define ZL303XX_PTP_DEFAULT_PORT_UMT_MAX_TABLE_SIZE       8                        /* Should align with ZL303XX_APR_MAX_NUM_MASTERS in Apr.h */
#define ZL303XX_PTP_DEFAULT_PORT_UMT_LOG_QUERY_INTVL      2

#define ZL303XX_PTP_DEFAULT_UMT_ENTRY_REQ_ANNC            ZL303XX_FALSE
#define ZL303XX_PTP_DEFAULT_UMT_ENTRY_REQ_TIMING          ZL303XX_FALSE  /* Deprecated */
#define ZL303XX_PTP_DEFAULT_UMT_ENTRY_MAX_ANNC_INTVL      PTP_V2_LMMI_NULL_VALUE
#define ZL303XX_PTP_DEFAULT_UMT_ENTRY_MAX_SYNC_INTVL      PTP_V2_LMMI_NULL_VALUE
#define ZL303XX_PTP_DEFAULT_UMT_ENTRY_MAX_DRESP_INTVL     PTP_V2_LMMI_NULL_VALUE
#define ZL303XX_PTP_DEFAULT_UMT_ENTRY_BMCA_ALLOW          ZL303XX_TRUE
#define ZL303XX_PTP_DEFAULT_UMT_ENTRY_SYNC_RX_TIMEOUT     8
#define ZL303XX_PTP_DEFAULT_UMT_ENTRY_FOLLOW_RX_TIMEOUT   8
#define ZL303XX_PTP_DEFAULT_UMT_ENTRY_SOF_RX_TIMEOUT      8  /* Sync or Follow_Up */
#define ZL303XX_PTP_DEFAULT_UMT_ENTRY_DRESP_RX_TIMEOUT    8
#define ZL303XX_PTP_DEFAULT_UMT_ENTRY_PDRESP_RX_TIMEOUT   8  /* PDelay & Follow_Up */

/* Currently unsupported. */
/* #define ZL303XX_PTP_DEFAULT_PORT_LOG_PDELAY_INTVL          0
   #define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_PDELAY_RATE_MAX   (Sint8T)0x7F
   #define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_PDEALY_GRANT_MAX  0
   #define ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_LIMIT_PDELAY      ZL303XX_PTP_PPS_UNLIMITED  */

/* Stream initialization defaults. */
#define ZL303XX_PTP_DEFAULT_STREAM_MODE                 ZL303XX_PTP_STREAM_USE_BMC
#define ZL303XX_PTP_DEFAULT_STREAM_UNICAST              ZL303XX_FALSE
#define ZL303XX_PTP_DEFAULT_STREAM_LOG_ANNC_INTVL       1
#define ZL303XX_PTP_DEFAULT_STREAM_ANNC_TIMEOUT         3   /* Number of Announce Intervals */
#define ZL303XX_PTP_DEFAULT_STREAM_SYNC_TIMEOUT         10  /* Number of Sync Intervals (V1). */
#define ZL303XX_PTP_DEFAULT_STREAM_LOG_SYNC_INTVL       0
#define ZL303XX_PTP_DEFAULT_STREAM_LOG_DELAY_INTVL      0
#define ZL303XX_PTP_DEFAULT_STREAM_LOG_PEER_DELAY_INTVL 0

/* The next 2 parameters are Scaled-Nanoseconds MS-48bit nSec; LS-16Bit fractional nSec */
#define ZL303XX_PTP_DEFAULT_STREAM_DELAY_ASYM_INGRESS   ((0/*ns*/ << 16) | (0x0000))
#define ZL303XX_PTP_DEFAULT_STREAM_DELAY_ASYM_EGRESS    ((0/*ns*/ << 16) | (0x0000))

/* The next parameters set the messageLength extension ('0' padding), in bytes,
 * for each EVENT messageType (use the zl303xx_PtpStreamEventMsgExtensionSet()
 * API to change at run-time).  */
#define ZL303XX_PTP_DEFAULT_MSG_LEN_EXTENSION_SYNC         0
#define ZL303XX_PTP_DEFAULT_MSG_LEN_EXTENSION_DELAY_REQ    0
#define ZL303XX_PTP_DEFAULT_MSG_LEN_EXTENSION_PDELAY_REQ   0
#define ZL303XX_PTP_DEFAULT_MSG_LEN_EXTENSION_PDELAY_RESP  0

/* The next parameters set the PAD TLV length, in bytes, for each EVENT
 * messageType (use the zl303xx_PtpStreamEventTlvPadLengthSet() API to change
 * at run-time).  */
#define ZL303XX_PTP_DEFAULT_PAD_TLV_LEN_SYNC               0
#define ZL303XX_PTP_DEFAULT_PAD_TLV_LEN_DELAY_REQ          0
#define ZL303XX_PTP_DEFAULT_PAD_TLV_LEN_PDELAY_REQ         0
#define ZL303XX_PTP_DEFAULT_PAD_TLV_LEN_PDELAY_RESP        0

#define ZL303XX_PTP_DEFAULT_STREAM_UNI_NEG_DUR          300
#define ZL303XX_PTP_DEFAULT_STREAM_UNI_NEG_RENEW        3     /* Number of Query Intervals */
#define ZL303XX_PTP_DEFAULT_STREAM_UNI_MONITOR_TIMING   ZL303XX_FALSE
#define ZL303XX_PTP_DEFAULT_STREAM_UNI_NEG_RETRIES_MAX  0xFFFFFFFF
#define ZL303XX_PTP_DEFAULT_STREAM_UNI_NEG_LOCKOUT_SEC  0
#define ZL303XX_PTP_DEFAULT_STREAM_MAX_CLOCK_CLASS      0     /* Disabled */
#define ZL303XX_PTP_DEFAULT_STREAM_NUM_TS_RECORDS       32    /* MUST be a power of 2. */
#define ZL303XX_PTP_DEFAULT_STREAM_NUM_TWO_STEP_REC     32    /* MUST be a power of 2. */
#define ZL303XX_PTP_DEFAULT_STREAM_KEEP_ALIVE_SEC       60    /* Keep-Alive Event period */
#define ZL303XX_PTP_DEFAULT_STREAM_UNCALIBRATED_INTVL    0    /* UNCALIBRATED State Intervals */

/* Foreign Master Qualification Defaults. */
#define ZL303XX_PTP_DEFAULT_FOREIGN_WINDOW_TIMEOUT      PTP_FOREIGN_MASTER_WINDOW_MIN
#define ZL303XX_PTP_DEFAULT_FOREIGN_WINDOW_THRESHOLD    PTP_FOREIGN_MASTER_THRESHOLD

/* Number of valid values for numRateRatioPkts attribute of zl303xx_PtpPortCreateS structure */
#define ZL303XX_PTP_VALID_VALUES_NUM_RATE_RATIO_PKTS_MAX           5

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
