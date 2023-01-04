

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains functions to configure, start, and stop a PTP
*     application. Also included are example function bindings for various PTP
*     events.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"
#include "zl303xx_Error.h"
#include "zl303xx_Os.h"
#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpStrings.h"
#include "zl303xx_PtpSetup.h"
#include "zl303xx_PtpV2MsgParse.h"
#include "zl303xx_ExamplePtp.h"
#include "zl303xx_ExampleMain.h"
#include "zl303xx_ExampleUtils.h"
#include "zl303xx_ExamplePtpGlobals.h"
#include "zl303xx_Pkt.h"
#include "zl303xx_PortingFunctions.h"

#include "f_du_reg_eep.h"

#include "BPF_RU_IPCM.h"
#include "BPF_COM_LOG.h"
#include "BPF_HM_DEVC.h"
#include "BPF_COM_CMD.h"
#include "f_com_inc.h"

/* Timestamp対応 examplePtpSocketSを使う */
#include "zl303xx_ExamplePtpSocket.h"

/* #include <net/if.h> */           /* for ifreq (may need __USE_MISC from _BSD_SOURCE) */
#include <linux/if.h>
#include <string.h>				/* Add */

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
#if  defined OS_LINUX
#include "zl303xx_ZLE30360.h"
#endif
#include "zl303xx_PortingFunctions.h"
#endif

#if defined ZL303XX_REFERENCE_SELECTION
   #include "zl303xx_ExampleRefSelectApi.h"
#endif

#if defined APR_INCLUDED
#include "zl303xx_Apr1Hz.h"
#include "zl303xx_Apr.h"
#include "zl303xx_ExampleAprGlobals.h"
#endif

#if defined APR_INCLUDED
#include "zl303xx_HoldoverUtils.h"
#endif

#if defined OS_LINUX && defined OHLY_TIMESTAMPS
#include "zl303xx_LinuxOhlyTS.h"
#endif

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
#include "zl303xx_ApiConfig.h"
#endif

/*****************   DEFINES     **********************************************/

/* Formats a PTP Clock Identity into a string for printing. */
#define ZL303XX_PTP_CLK_TO_STRING(clkId, string)                              \
            OS_SPRINTF(string, "0x%02hx%02hx%02hx%02hx%02hx%02hx%02hx%02hx", \
                    clkId[0], clkId[1], clkId[2], clkId[3],                    \
                    clkId[4], clkId[5], clkId[6], clkId[7])




#if defined ZLS30390_INCLUDED
/* Use the same src and dest addresses in all examples */
#define DEFAULT_SLAVE_IPV4_ADDRESS  "10.0.0.235"
#define DEFAULT_MASTER_IPV4_ADDRESS "10.0.0.230"

#endif

#if !(defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD)
/* Generate software time stamps for initial debug purposes. These software
 * time stamps are unsuitable for a real application because they are generated
 * from the CPU clock. */
/* #define EXAMPLE_PTP_SW_TIMESTAMPS  1 */
#endif

#if (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD )
/* Call the example socket functions for initial debug purposes. These socket
 * functions are NOT INTENDED to be a full-featured transport layer. */
#define EXAMPLE_PTP_SOCKET_TEST  1
#else
#if defined EXAMPLE_PTP_SOCKET_TEST
/* #warning MSCC: The EXAMPLE_PTP_SOCKET_TEST interface is NOT INTENDED to be a full-featured transport layer. Use at your own risk. This feature is not supported */
#endif
#endif

/*****************   DATATYPES                      ***************************/
#if defined APR_INCLUDED
#if defined MUX_PTP_STREAMS_TO_APR_SLOTS
typedef struct
{
    zl303xx_BooleanE aprSlotCurrentActiveRef;
    Sint16T aprServerSlot;
    zl303xx_PtpStreamHandleT streamHandle;
} muxToAprSlotsS;

muxToAprSlotsS muxPtpStreamsToAprSlotsS[ZL303XX_PTP_NUM_STREAMS_MAX];
#endif
#endif





/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/* Individual event handlers. */
static zlStatusE examplePtpStreamExtDataAlloc(examplePtpStreamExtDataS **ppExt);
static void examplePtpStreamExtDataFree(examplePtpStreamExtDataS **ppExt);
static void examplePtpEventContractRejected(zl303xx_PtpEventContractRejectedS *eventData);
static void examplePtpEventClockCreate(zl303xx_PtpEventClockCreateS *eventData);
static void examplePtpEventClockDelete(zl303xx_PtpEventClockDeleteS *eventData);
static void examplePtpEventPortDelete(zl303xx_PtpEventPortDeleteS *eventData);
static void examplePtpEventStreamCreate(zl303xx_PtpEventStreamCreateS *eventData);
static void examplePtpEventStreamDelete(zl303xx_PtpEventStreamDeleteS *eventData);
static void examplePtpEventStreamStateChange(zl303xx_PtpEventStreamStateChangeS *eventData);
static void examplePtpEventStreamKeepAlive(zl303xx_PtpEventStreamKeepAliveS *eventData);
static void examplePtpEventClockBmcaUpdate(zl303xx_PtpEventClockBmcaUpdateS *eventData);
static void examplePtpEventServoData(zl303xx_PtpEventServoDataS *eventData);
static void examplePtpEventCounterAlarm(zl303xx_PtpEventCounterAlarmS *eventData);
static void examplePtpEventSquelch(zl303xx_PtpEventSquelchS *eventData);
static void examplePtpEventMsgIntvlChange(zl303xx_PtpEventMsgIntvlChangeS *eventData);
static void examplePtpEventContractGranted(zl303xx_PtpEventContractGrantedS *eventData);
static void examplePtpEventContractExpired(zl303xx_PtpEventContractExpiredS *eventData);
static void examplePtpEventMultiplePeerResp(zl303xx_PtpEventMultiplePeerRespS *eventData);
static void examplePtpEventUnknownTlv(zl303xx_PtpEventUnknownTlvS *eventData);
static void examplePtpEventLeapSecondsFlagChange(zl303xx_PtpEventLeapSecondsFlagChangeS *eventData);
static void examplePtpEventUtcOffsetChange(zl303xx_PtpEventUtcOffsetChangeS *eventData);
static void examplePtpEventRxStepsRemovedExceeded(zl303xx_PtpEventRxStepsRemovedExceededS *eventData);
static void examplePtpEventParentDsUpdated(zl303xx_PtpEventParentDsChangeS *eventData);
static void examplePtpEventPortFaulty(zl303xx_PtpEventPortFaultS *eventData);
static void examplePtpEventStreamDisqualified(zl303xx_PtpEventStreamDisqualifiedS *eventData);

/* Individual hardware command handlers. */
static zlStatusE examplePtpHwClockTimeGet(zl303xx_PtpHwClockTimeGetS *cmdParams);
static zlStatusE examplePtpHwClockTimeSet(zl303xx_PtpHwClockTimeSetS *cmdParams);
static zlStatusE examplePtpHwLockStatusGet(zl303xx_PtpHwLockStatusGetS *cmdParams);
static zlStatusE examplePtpHwClockStabilityGet(zl303xx_PtpHwClockStabilityGetS *cmdParams);
static zlStatusE examplePtpHwTimeSetStatusGet(zl303xx_PtpHwTimeStatusGetS *cmdParams);
static zlStatusE examplePtpHwPllPerformanceGet(zl303xx_PtpHwPllPerformanceGetS *cmdParams);

#ifdef EXAMPLE_PTP_SW_TIMESTAMPS
static void examplePtpSwTimeStampGet(zl303xx_TimeStamp *pTimeStamp);
#endif

/* Timestamp */
static void Get25gTimestamps(Sint32T fd, Uint16T seq, zl303xx_TimeStamp *pTimeStamp,Uint16T trx);


typedef struct{
	int kind;
	int ptp_tag;
	int ptp_time[4];
}e25g_tstamp;


/*****************   STATIC GLOBAL VARIABLES   ********************************/
char	getPtpParameter_str[64];
static	zl303xx_PtpStreamOperatingStateE f_ptpw_pre_operState = -1;

/*****************   IMPORTED GLOBAL VARIABLES   ******************************/
extern exampleAppS zlExampleApp;
extern zl303xx_BooleanE exampleAppRunning;

#if defined ZLS30390_INCLUDED
extern Uint8T TARGET_DPLL;
#endif

/* An array of structures; one per clock stream */
extern zl303xx_ClockStreamS *Zl303xx_PtpStreamArray[ZL303XX_PTP_NUM_STREAMS_MAX];

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/* Handles to the most recently created clock, port, and stream. */
zl303xx_PtpClockHandleT  PtpClockHandle  = (zl303xx_PtpClockHandleT) (-1);
zl303xx_PtpPortHandleT   PtpPortHandle   = (zl303xx_PtpPortHandleT)  (-1);
zl303xx_PtpStreamHandleT PtpStreamHandle = (zl303xx_PtpStreamHandleT)(-1);


/* Configurable PTP port parameters. */
char examplePtpLocalIpv4[ZL303XX_PKT_IPV6_STR_LENGTH] = "\0";
zl303xx_PortAddress examplePtpLocalAddr[2] = {{ZL303XX_NETWORK_PROTOCOL_UNKNOWN, 0, {0}},{ZL303XX_NETWORK_PROTOCOL_UNKNOWN, 0, {0}}};
zl303xx_BooleanE examplePtpUseDelayMechanism = ZL303XX_TRUE;
zl303xx_DelayMechanismE examplePtpDelayMechanism = ZL303XX_DELAY_MECHANISM_E2E;

/* Configurable PTP stream parameters. */
char examplePtpDestIpv4[ZL303XX_PKT_IPV6_STR_LENGTH] = "\0";
zl303xx_PortAddress examplePtpDestAddr[2] = {{ZL303XX_NETWORK_PROTOCOL_UNKNOWN, 0, {0}}, {ZL303XX_NETWORK_PROTOCOL_UNKNOWN, 0, {0}}};
Sint8T examplePtpLogAnnounceInterval    = ZL303XX_PTP_DEFAULT_STREAM_LOG_ANNC_INTVL; /* 1pp2s */
Uint8T examplePtpAnnounceReceiptTimeout = ZL303XX_PTP_DEFAULT_STREAM_ANNC_TIMEOUT;   /* x3 */
#if defined APR_INCLUDED
/* Use zl303xx_AprSetPktRate() to configure event packet rates */
#else
Sint8T examplePtpLogSyncInterval        = -6;  /* 64 pps */
Sint8T examplePtpLogMinDelayReqInterval = -4;  /* 16 pps */
#endif

extern int keep_announce_clockclass;			/* AnnounceClockClass現在値 */

/****************   EXTERNAL GLOBAL VARIABLES   ******************************/
/* zl303xx_PtpV2handleAnnounceに使用する変数をM-plane edit時に置き換え */
extern int accepted_clock_classes;

/* M-plane Edit変化確認用グローバル変数 */
extern	unsigned char	*f_ptp_d_num;
extern	unsigned char	*f_ptp_m_macaddr;
extern	unsigned char	*f_ptp_accepted_clockclass;

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/
int BPF_HM_DEVC_EEPROM_READ(unsigned short offset, unsigned char *data_p);

/* examplePtpEnvInit */
/**
   Initializes the PTP environment and the global variables used in this
   example code.

*******************************************************************************/
zlStatusE examplePtpEnvInit(void)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpInit();
   }

   /* Setup the PTSF default callback routines. */
   if (status == ZL303XX_OK)
   {
      Uint32T ptsfFlag;

      for (ptsfFlag = 0; ptsfFlag < ZL303XX_PTSF_NUM_FLAGS; ptsfFlag++)
      {
         (void)zl303xx_PtsfCallbackSet(ptsfFlag, examplePtsfDefaultCallback);
      }
   }

   if (status == ZL303XX_OK)
   {
      if (strlen(examplePtpLocalIpv4) == 0)
      {
         memset(&examplePtpLocalAddr[0], 0, sizeof(examplePtpLocalAddr));
         examplePtpLocalAddr[0].networkProtocol = ZL303XX_NETWORK_PROTOCOL_INVALID;
         memset(&examplePtpLocalAddr[1], 0, sizeof(examplePtpLocalAddr));
         examplePtpLocalAddr[1].networkProtocol = ZL303XX_NETWORK_PROTOCOL_INVALID;
      }

      if (strlen(examplePtpDestIpv4) == 0)
      {
         memset(&examplePtpDestAddr[0], 0, sizeof(examplePtpDestAddr));
         examplePtpDestAddr[0].networkProtocol  = ZL303XX_NETWORK_PROTOCOL_INVALID;
         memset(&examplePtpDestAddr[1], 0, sizeof(examplePtpDestAddr));
         examplePtpDestAddr[1].networkProtocol  = ZL303XX_NETWORK_PROTOCOL_INVALID;
      }

   #ifdef EXAMPLE_PTP_SOCKET_TEST
      #if !(defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD )
      /* #warning MSCC: The EXAMPLE_PTP_SOCKET_TEST interface was NOT INTENDED to be a full-featured transport layer. */
      #endif
      status = examplePtpSocketEnvInit();
   #endif
   }

   return status;
}

/* examplePtpEnvClose */
/**
   Closes the PTP environment and resets global variables used in this
   example code, as necessary.

*******************************************************************************/
 zlStatusE examplePtpEnvClose(void)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK && exampleAppRunning == ZL303XX_TRUE)
   {
#ifdef EXAMPLE_PTP_SOCKET_TEST
       if (status == ZL303XX_OK)
       {
          status = examplePtpSocketEnvClose();
       }
#endif

       if (status == ZL303XX_OK)
       {
           status = examplePtpStop();
       }
   }

   /* Reset the global handles. */
   if (status == ZL303XX_OK)
   {
      PtpClockHandle  = (zl303xx_PtpClockHandleT) (-1);
      PtpPortHandle   = (zl303xx_PtpPortHandleT)  (-1);
      PtpStreamHandle = (zl303xx_PtpStreamHandleT)(-1);
   }

   return status;
}

/* examplePtpClockCreateStructInit */
/**
   An example of how to initialize a PTP clock configuration structure,
   including necessary function bindings.

*******************************************************************************/
zlStatusE examplePtpClockCreateStructInit(examplePtpClockCreateS *pClock)
{
   zlStatusE status = ZL303XX_OK;

   /* Initialize the clock config structure with default values. */
   if (status == ZL303XX_OK)
   {
      /* Clear the structure. */
      memset(pClock, 0, sizeof(*pClock));

      /* Set the clockType member in case a profile wishes to do initialization
       * based on expected clock operation.    */
      pClock->config.clockType = zl303xx_GetClockType();

      status = zl303xx_PtpProfileClockStructInit(zl303xx_GetPtpProfile(), &pClock->config);
   }

   /* Overwrite defaults with values set through example globals. */
   if (status == ZL303XX_OK)
   {
      pClock->config.defaultDS.twoStepFlag = exampleGetTwoStepFlag();
      pClock->config.egressClassUpdateTrigger = exampleGetEgressClassUpdateTrigger();

      /* Attach an event handler. */
      pClock->config.eventCallback = examplePtpEventHandler;

      /* Attach a hardware command handler. */
      pClock->config.hwCmdFn = examplePtpHwCmdHandler;

      pClock->handle = (zl303xx_PtpClockHandleT)(-1);
   }

   return status;
}

/* examplePtpClockCreate */
/**
   An example of how to start a PTP clock. A PTP clock represents a local
   oscillator that provides time information. Each clock runs as its own OS task.

*******************************************************************************/
zlStatusE examplePtpClockCreate(examplePtpClockCreateS *pClock)
{
   zlStatusE status = ZL303XX_OK;

   int				i;
   unsigned short	addr = D_DU_EEP_MAC_ADDR_PORT1_H1;				/* EEPROMのMACアドレス格納番地 */
   unsigned char	data = 0;

   /*** TASK parameters ***/
   /* Each PTP clock runs as its own task, with the following parameters:
    *    taskName      - Default is "zlPtpClock0"
    *    taskPriority  - Default is OS dependent
    *    taskStackSize - In bytes
    *    msgQLength    - Task communication is handled by a message queue
    *                    (e.g., timer tick notifications, received packets). */

   /*** PTP STANDARD parameters ***/
   /* A PTP clock maintains certain properties specified in IEEE1588-2008 related
    * to the underlying hardware, network configuration, and connected timing
    * references:
    *    defaultDS
    *    localTimeProperties
    *
    * Selected important properties:
    *    defaultDS.twoStepFlag - Set to ZL303XX_TRUE if the time stamping hardware
    *       is unable to insert egress time into packets on-the-fly.
    *
    *    defaultDS.clockIdentity - A standard-compliant way to generate this is
    *       to take the MAC address from a physical interface attached to the
    *       PTP clock and insert 0xFFFE into the middle of it (e.g., if MAC = 00:11:22:33:44:55,
    *       the clockIdentity is 0x001122FFFE334455).
    *
    * The clock's localTimeProperties can be changed at run time based on the currently
    * selected timing reference signal. DefaultDS cannot be changed after the clock is created. */

   /*** USER CALLBACK parameters ***/
   /* Callback functions can be provided to handle events generated by the PTP
    * protocol, and to query/set the underlying hardware device:
    * eventCallback - Called any time a PTP event happens on this clock.
    * hwCmdFn       - Used to query/set hardware parameters.
    * extData       - A void pointer to some user data related to the clock
    *                 (e.g., hardware parameter structure). This pointer will be
    *                 provided in relevant callback event structures. User is
    *                 responsible for memory management. */

   /* Modify values as needed and create the clock. */
   if (status == ZL303XX_OK)
   {
       /* Make a unique Clock Identity using the MAC address */
#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD

       if ((status = osGetMacAddr(zl303xx_GetExampleInterfaceStr(zl303xx_GetExampleInterface()), (Uint8T*)&pClock->config.defaultDS.clockIdentity)) != ZL303XX_OK)
       {
           ZL303XX_TRACE_ALWAYS("osGetMacAddr() failed with status = %d",
           status, 0,0,0,0,0);
       }
       pClock->config.defaultDS.clockIdentity[7] = pClock->config.defaultDS.clockIdentity[5];
       pClock->config.defaultDS.clockIdentity[6] = pClock->config.defaultDS.clockIdentity[4];
       pClock->config.defaultDS.clockIdentity[5] = pClock->config.defaultDS.clockIdentity[3];
       pClock->config.defaultDS.clockIdentity[4] = 0xFE;
       pClock->config.defaultDS.clockIdentity[3] = 0xFF;
#else
      /* #warning MSCC: Code required for unique Clock Identity. */
      /* INSERT CODE here to generate an actual clock identity (usually from the MAC address). */
      {
         /* EEPROMからMACアドレス情報を取得 */
         Uint8T fakeClockId[] = {0};

         for( i = 0; i < 6; i++, addr++ ) {
             BPF_HM_DEVC_EEPROM_READ( addr, &data );
             fakeClockId[i] = data;
         }
         pClock->config.defaultDS.clockIdentity[7] = fakeClockId[5];
         pClock->config.defaultDS.clockIdentity[6] = fakeClockId[4];
         pClock->config.defaultDS.clockIdentity[5] = fakeClockId[3];
         pClock->config.defaultDS.clockIdentity[4] = 0xFE;
         pClock->config.defaultDS.clockIdentity[3] = 0xFF;
         pClock->config.defaultDS.clockIdentity[2] = fakeClockId[2];
         pClock->config.defaultDS.clockIdentity[1] = fakeClockId[1];
         pClock->config.defaultDS.clockIdentity[0] = fakeClockId[0];

         /* Uint8T fakeClockId[] = {0x00, 0x11, 0x22, 0xFF, 0xFE, 0x33, 0x00, 0x00}; */
         /* fakeClockId[6] = pClock->config.defaultDS.clockQuality.clockClass; */
         /* fakeClockId[7] = (Uint8T)((pClock->handle) & 0x000000FF); */    /* Unique */
         /* memcpy(pClock->config.defaultDS.clockIdentity, fakeClockId, */
         /*       sizeof(pClock->config.defaultDS.clockIdentity)); */
      }
#endif

      pClock->config.defaultDS.twoStepFlag = exampleGetTwoStepFlag();

   }

   if (status == ZL303XX_OK && (status = zl303xx_PtpClockCreate(&pClock->config, &pClock->handle)) == ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS("PTP CLOCK created. handle=%lu", pClock->handle, 0,0,0,0,0);
      PtpClockHandle = pClock->handle;
   }
   else
       ZL303XX_TRACE_ALWAYS("PTP CLOCK created for handle=%d failed=%lu", pClock->handle, status, 0,0,0,0);

   return status;
}

/* examplePtpPortCreateStructInit */
/**
   An example of how to initialize a PTP port configuration structure,
   including necessary function bindings.

*******************************************************************************/
zlStatusE examplePtpPortCreateStructInit(examplePtpPortCreateS *pPort,int count)
{
   zlStatusE status = ZL303XX_OK;
   int				i;
   unsigned short	addr;
   unsigned char	data = 0;

   /* Initialize the port config structure with default values. */
   if (status == ZL303XX_OK)
   {
      memset(pPort, 0, sizeof(*pPort));
      status = zl303xx_PtpProfilePortStructInit(zl303xx_GetPtpProfile(), &pPort->config);
   }

	if(count == 0){
		addr = D_DU_EEP_MAC_ADDR_PORT1_H1;	/* port0 */
	}
	else{
		status = ZL303XX_ERROR;
		return status;
	}

   ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 2, "examplePtpPortCreateStructInit: count=%d, addr=%04x", count, addr,0,0,0,0);

   /* Overwrite defaults with values set through example globals. */
   if (status == ZL303XX_OK)
   {
      pPort->config.delayMechanism = examplePtpDelayMechanism;

      	examplePtpLocalAddr[count].networkProtocol = ZL303XX_NETWORK_PROTOCOL_802_3;
      	examplePtpLocalAddr[count].addressLength = 6;
      	for( i = 0; i < examplePtpLocalAddr[count].addressLength; i++, addr++ ) {

         	BPF_HM_DEVC_EEPROM_READ( addr, &data );
         	examplePtpLocalAddr[count].addressField[i] = data;
      	}

      	/* Initialize the structure with the global example port address,
       	 * if it has been set */
      	if (examplePtpLocalAddr[count].networkProtocol != ZL303XX_NETWORK_PROTOCOL_INVALID)
      	{
         	memcpy(&pPort->config.localAddr, &examplePtpLocalAddr[count], sizeof(pPort->config.localAddr));
      	}
	  
      /* Attach a message transmit handler. */
      pPort->config.txMsgFn = examplePtpTx;

      pPort->handle = (zl303xx_PtpPortHandleT)(-1);
   }

   return status;
}

/* examplePtpPortCreate */
/**
   An example of how to start a PTP port. A PTP port represents logical access
   point of a clock to a communications network.

   This also can be used to create linkages to any necessary external data.

*******************************************************************************/
zlStatusE examplePtpPortCreate(examplePtpPortCreateS *pPort)
{
   zlStatusE status = ZL303XX_OK;

   /*** PORT parameters ***/
   /* clockHandle    - Handle of the clock this port will be attached to. Must be changed.
    * portNumber     - Set to 0 to auto-generate this value.
    * delayMechanism - End-to-end (E2E) to support DELAY_REQ/RESP messages or
    *    Peer-to-peer to support PEER_DELAY_REQ/RESP messages, either of which
    *    allows for phase recovery. Disabled to only use SYNC messages for
    *    frequency recovery.
    * ptpVersion     - Only the default (2) is currently supported.
    * acceptableMasterTableEnabled - Enables the AMT on this port.
    * acceptableMasterTableLength - Size of AMT to allocate.
    * localAddr - The network address of this port.
    * mixedMode - Enabled mixed multicast (SYNC) / unicast (DELAY_REQ/RESP) mode.
    * unicastNegotiation - Parameters to set unicast negotiation packet rates.
    * alternateMaster - Specifies this port as an alternate master.
    * extData - A void pointer to some user data related to the port (e.g., socket
    *           information). This pointer will be provided in relevant
    *           callback event structures. User is responsible for memory management.
    * txMsgFn - Callout function used to transmit egress messages. */

   /* INSERT CODE to initialize transport layer structures here (e.g., create sockets
    * and bind to the source address on ports 319 and 320. */

#ifdef EXAMPLE_PTP_SOCKET_TEST
   {
      examplePtpSocketT ptpSkt;

      memset(&ptpSkt, 0, sizeof(ptpSkt));

   /* #warning MSCC: examplePtpPortCreate(): port->ptp.config.localAddr will be used for socket */

      /* Open unicast sockets for TX/RX. Memory for the socket structure is
       * allocated inside this function. */
      if (status == ZL303XX_OK)
      {
          status = examplePtpSocketUcastOpen(&pPort->config.localAddr, &ptpSkt);
           if (status != ZL303XX_OK)
               ZL303XX_TRACE_ALWAYS("examplePtpPortCreate: examplePtpSocketUcastOpen() failed=%lu", status, 0,0,0,0,0);
      }

      /* Save the structure pointer in the port's extData. */
      if (status == ZL303XX_OK)
      {
         pPort->config.extData = ptpSkt;
      }
   }
#endif

   /* Create the PTP port. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortCreate(&pPort->config, &pPort->handle);
      if (status == ZL303XX_OK)
      {
         ZL303XX_TRACE_ALWAYS("PTP PORT created. handle=%lu", pPort->handle, 0,0,0,0,0);
         PtpPortHandle = pPort->handle;

#if defined EXAMPLE_PTP_SOCKET_TEST
        /* Provide socket with backreference to PTP port */
         if (pPort->config.extData)
         {
             if ((status = examplePtpSocketPortHandleSet(pPort->config.extData, pPort->handle)) != ZL303XX_OK)
             {
                ZL303XX_TRACE_ALWAYS("examplePtpPortCreate: examplePtpSocketPortHandleSet() failed=%lu", status, 0,0,0,0,0);
             }
         }
#endif
      }
      else
          ZL303XX_TRACE_ALWAYS("examplePtpPortCreate: zl303xx_PtpPortCreate() failed=%lu", status, 0,0,0,0,0);
   }


   return status;
}

/* examplePtpStreamCreateStructInit */
/**
   An example of how to initialize a PTP stream configuration structure.

*******************************************************************************/
zlStatusE examplePtpStreamCreateStructInit(examplePtpStreamCreateS *pStream, int count)
{
   zlStatusE status = ZL303XX_OK;
	unsigned char	data = 0;			/* 適用MACアドレス識別フラグ(from EEPROM) */
	
   /* Initialize the stream config structure with default values. */
   if (status == ZL303XX_OK)
   {
      memset(pStream, 0, sizeof(*pStream));
      status = zl303xx_PtpProfileStreamStructInit(zl303xx_GetPtpProfile(), &pStream->config);
   }

   if(count > 1){
	    status = ZL303XX_ERROR;
		return status;
	}

	ZL303XX_TRACE_ALWAYS("examplePtpStreamCreateStructInit: count=%d",count, 0, 0,0,0,0);

   /* Overwrite defaults with values set through example globals. */
   if (status == ZL303XX_OK)
   {
      pStream->config.logAnnounceInterval    = examplePtpLogAnnounceInterval;
      pStream->config.announceReceiptTimeout = examplePtpAnnounceReceiptTimeout;
#if defined APR_INCLUDED
      pStream->config.logSyncInterval        = log2PacketsPerSecond(zl303xx_AprGetPktRate(ZL303XX_TRUE));
      pStream->config.logMinDelayReqInterval = log2PacketsPerSecond(zl303xx_AprGetPktRate(ZL303XX_FALSE));
      pStream->config.logMinPdelayReqInterval = log2PacketsPerSecond(zl303xx_AprGetPktRate(ZL303XX_FALSE));
#else
      pStream->config.logSyncInterval         = examplePtpLogSyncInterval;
      pStream->config.logMinDelayReqInterval  = examplePtpLogMinDelayReqInterval;
      pStream->config.logMinPdelayReqInterval = examplePtpLogMinDelayReqInterval;
#endif

	  pStream->config.portHandle = count;		/* 0 or 1 */

	  /* PTP用Mac Address設定 (Non-forwardable MAC address 固定) */
      examplePtpDestAddr[count].networkProtocol = ZL303XX_NETWORK_PROTOCOL_802_3;
      examplePtpDestAddr[count].addressLength = 6;
	  
      /* EEPROMからmac-address設定値を取得 */
      BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_VID_MULTI_MACADDR, &data );
	  if( data == 0x01 ) {
      	examplePtpDestAddr[count].addressField[0] = 0x01;
      	examplePtpDestAddr[count].addressField[1] = 0x80;
      	examplePtpDestAddr[count].addressField[2] = 0xC2;
      	examplePtpDestAddr[count].addressField[3] = 0x00;
      	examplePtpDestAddr[count].addressField[4] = 0x00;
      	examplePtpDestAddr[count].addressField[5] = 0x0E;
      }
      else{
      	examplePtpDestAddr[count].addressField[0] = 0x01;
      	examplePtpDestAddr[count].addressField[1] = 0x1B;
      	examplePtpDestAddr[count].addressField[2] = 0x19;
      	examplePtpDestAddr[count].addressField[3] = 0x00;
      	examplePtpDestAddr[count].addressField[4] = 0x00;
      	examplePtpDestAddr[count].addressField[5] = 0x00;
	  }
      /* Initialize the structure with the global example port address,
       * if it has been set */
      if (examplePtpDestAddr[count].networkProtocol != ZL303XX_NETWORK_PROTOCOL_INVALID)
      {
         memcpy(&pStream->config.destAddr, &examplePtpDestAddr[count], sizeof(pStream->config.destAddr));
      }

      pStream->config.createFlags |= ZL303XX_PTP_STREAM_CREATE_FIRE_EVENT;

      pStream->handle = (zl303xx_PtpStreamHandleT)count;

   }

#if defined EXAMPLE_PTP_SOCKET_TEST
   if (status == ZL303XX_OK)
   {
      /* Allocate memory for the external stream data - there is no link
         between this memory and PTP's dynamically allocated streams
         until examplePtpStreamCreate() connects a socket */
      examplePtpStreamExtDataS *pExtData  = NULL;

      status = examplePtpStreamExtDataAlloc(&pExtData);
      pStream->config.extData = pExtData;
      
      if (status != ZL303XX_OK || pExtData == NULL)
      {
         ZL303XX_TRACE_ALWAYS("   (ERROR) External stream data memory allocate failed.",
                              0,0,0,0,0,0);
         status = ZL303XX_RTOS_MEMORY_FAIL;
      }
      else
          pStream->config.extData = pExtData;

   }
#endif

   return status;
}

/* examplePtpStreamCreate */
/**
   An example of how to start a PTP stream. A PTP stream is a Microsemi-specific
   implementation used to represent bidirectional connection between two endpoints.

   This also can be used to create linkages to any necessary external data.

*******************************************************************************/
zlStatusE examplePtpStreamCreate(examplePtpStreamCreateS *pStream)
{
   zlStatusE status = ZL303XX_OK;
   static const char fnName[] = "examplePtpStreamCreate";
   zl303xx_PtpPortCreateS portConfig;
   examplePtpStreamExtDataS *pExtData = NULL;

   status = ZL303XX_CHECK_POINTER(pStream);
   if (status == ZL303XX_OK)
   {
      if (pStream->config.extData == NULL)
      {
         ZL303XX_TRACE_ERROR("%s: Stream external data not allocated.", fnName, 0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
      else
          pExtData = pStream->config.extData;
   }

   /* Get a copy of the port's configuration. */
   if (status == ZL303XX_OK)
   {
      if ((status = zl303xx_PtpPortConfigGet(pStream->config.portHandle, &portConfig)) != ZL303XX_OK)
          ZL303XX_TRACE_ERROR("%s: Call to zl303xx_PtpPortConfigGet() with portHandle=%d failed=%d.",
                            fnName, pStream->config.portHandle, status, 0,0,0);
   }

   /*** STREAM parameters ***/
   /* portHandle - Handle of the port this stream will be attached to. Must be changed.
    * destAddr - Destination address (multicast group or unicast endpoint).
    * mode - BMC (master or slave is determined by automatically), SLAVE_ONLY, or
    *        MASTER_ONLY (non-standard mode).
    * unicast - ZL303XX_TRUE if destAddr is a unicast address.
    * logAnnounceInterval - time between egress ANNOUNCE messages when in master state.
    * announceReceiptTimeout - number of ANNOUNCE intervals that must elapse before
    *                          declaring a timeout when in slave state.
    * logSyncInterval - time between egress SYNC messages when in master state.
    * logMinDelayReqInterval - time between egress DELAY_REQ messages when in slave state.
    * unicastNegotiationDuration - Duration value in seconds for unicast contracts.
    * numTsRecords - Length of queue used to store egress DELAY_REQ and SYNC (FOLLOW_UPs
    *                are used) time stamps.
    * extData - A void pointer to some user data related to the port (e.g., transport
    *           layer information). This pointer will be provided in relevant
    *           callback event structures. User is responsible for memory management. */

   /* INSERT CODE to initialize multicast transport layer structures here, if
    * necessary (e.g., create sockets and bind to the source address on ports
    * 319 and 320). */

#ifdef EXAMPLE_PTP_SOCKET_TEST
   if (status == ZL303XX_OK)
   {
      if (pStream->config.unicast == ZL303XX_FALSE)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "examplePtpSocketMcastOpen() will start.", 0,0,0,0,0,0);
         
         if(portConfig.extData != NULL)
         {
            examplePtpSocketT ptpSkt;

            /* Check if the port already has an open unicast socket */
            status = zl303xx_PtpPortExtDataGet(pStream->config.portHandle, (void **)&ptpSkt);
            if (status != ZL303XX_OK)
            {
               ZL303XX_TRACE_ALWAYS("examplePtpStreamCreate: zl303xx_PtpPortExtDataGet() failed=%lu", status, 0,0,0,0,0);
            }
            /* Close the Unicast socket opened by the port before opening a multicast socket*/
            status = examplePtpSocketClose(&ptpSkt);
            if(status != ZL303XX_OK)
            {
               ZL303XX_TRACE_ALWAYS("examplePtpStreamCreate: examplePtpSocketClose() failed=%lu", status, 0,0,0,0,0);
            }

            status = zl303xx_PtpPortExtDataSet(pStream->config.portHandle, NULL);
            if (status != ZL303XX_OK)
               ZL303XX_TRACE_ALWAYS("examplePtpStreamCreate: zl303xx_PtpPortExtDataSet() failed=%lu", status, 0,0,0,0,0);
         }
         /* Open multicast sockets for TX/RX. Memory for the socket structure is
          * allocated inside this function. */
         status = examplePtpSocketMcastOpen(&portConfig.localAddr,
                                            &pStream->config.destAddr,
                                            &pExtData->skt);
         if (status != ZL303XX_OK)
         {
             ZL303XX_TRACE_ALWAYS("examplePtpStreamCreate: examplePtpSocketMcastOpen() failed=%lu", status, 0,0,0,0,0);
         }
      }
   }
#endif

   if (status == ZL303XX_OK)
   {
      /* If unicast negotiation is being used, add a UMT entry rather than
       * directly creating a stream. */
      if (portConfig.unicastNegotiation.enabled == ZL303XX_TRUE)
      {
         zl303xx_PtpUmtEntryS umtEntry;

         status = zl303xx_PtpUmtEntryStructInit(&umtEntry);

         if (status == ZL303XX_OK)
         {
            umtEntry.streamConfig = pStream->config;
            status = zl303xx_PtpUmtEntryAdd(&umtEntry, &pStream->handle);
         }
      }

      /* Otherwise, just create a stream. */
      else
      {
         status = zl303xx_PtpStreamCreate(&pStream->config, &pStream->handle);
      }
   }

#if defined EXAMPLE_PTP_SOCKET_TEST
    if (status == ZL303XX_OK)
    {
        pExtData = pStream->config.extData;

        /* Assign the port's socket, to stream extData */
        if (pExtData)
        {
            if (pExtData->skt == NULL)
            {
                pExtData->skt = portConfig.extData;

                ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 2, "examplePtpStreamCreate: pExtData->skt=%p", pExtData->skt, 0,0,0,0,0);
            }

            if (pExtData->skt != NULL)
            {
               if ((status = examplePtpSocketStreamHandleSet(pExtData->skt, pStream->handle)) != ZL303XX_OK)
               {
                  ZL303XX_TRACE_ALWAYS("examplePtpStreamCreate: examplePtpSocketStreamHandleSet() failed=%lu, stream %d",
                                      status, pStream->handle,0,0,0,0);
               }
               if ((status = examplePtpSocketPortHandleSet(pExtData->skt, pStream->config.portHandle)) != ZL303XX_OK)
               {
                  ZL303XX_TRACE_ALWAYS("examplePtpStreamCreate: examplePtpSocketPortHandleSet() failed=%lu, stream %d, port %d",
                                     status, pStream->handle, pStream->config.portHandle, 0,0,0);
               }
            }
        }
    }
#endif

   return status;
}

/* examplePtpStreamCguIdSet */
/**
   An example of how to associate a timing recovery mechanism (clock generation
   unit if using ZLS3038x package) to a PTP stream.

*******************************************************************************/
zlStatusE examplePtpStreamCguIdSet(examplePtpStreamCreateS *pStream, void *cguId)
{
   zlStatusE status = ZL303XX_OK;
   static const char fnName[] = "examplePtpStreamCguIdSet";
   examplePtpStreamExtDataS *pExtData = pStream->config.extData;

   if (status == ZL303XX_OK)
   {
      if (pStream->config.extData == NULL)
      {
         ZL303XX_TRACE_ALWAYS("%s: Stream external data not yet allocated.", fnName, 0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
      else
          pExtData->cguId = cguId;
   }

#if defined EXAMPLE_PTP_SOCKET_TEST && defined OHLY_TIMESTAMPS
    if (status == ZL303XX_OK)
    {
        /* Assign the socket backreference handles, if stream socket created */
        if (pExtData && pExtData->skt)
        {
             if ((status = examplePtpSocketStreamHandleSet(pExtData->skt, pStream->handle)) != ZL303XX_OK)
             {
                ZL303XX_TRACE_ALWAYS("examplePtpStreamCreate: examplePtpSocketStreamHandleSet() failed=%lu, stream %d",
                                    status, pStream->handle,0,0,0,0);
             }
             if ((status = examplePtpSocketPortHandleSet(pExtData->skt, pStream->config.portHandle)) != ZL303XX_OK)
             {
                ZL303XX_TRACE_ALWAYS("examplePtpStreamCreate: examplePtpSocketPortHandleSet() failed=%lu, stream %d, port %d",
                                    status, pStream->handle, pStream->config.portHandle,0,0,0);
             }
        }
    }
#endif

   return status;
}

/* examplePtpLocalIpv4SetDefault */
/**
   Sets the local IPv4 address to use for the next created PTP port. This
   function sets the examplePtpLocalAddr structure.

  Parameters:
   [in]   ipv4Addr   IPv4 address in dot-notation.

*******************************************************************************/
zlStatusE examplePtpLocalIpv4SetDefault(const char *ipv4Addr)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      if (ipv4Addr != NULL)
      {
         strncpy(examplePtpLocalIpv4, ipv4Addr, sizeof(examplePtpLocalIpv4) - 1);
         status = zl303xx_PtpConvertIPv4ToPortAddress(examplePtpLocalIpv4, &examplePtpLocalAddr[0]);
         status = zl303xx_PtpConvertIPv4ToPortAddress(examplePtpLocalIpv4, &examplePtpLocalAddr[1]);
      }
      else
      {
         memset(examplePtpLocalIpv4, '\0', sizeof(examplePtpLocalIpv4));
         memset(&examplePtpLocalAddr[0], 0, sizeof(examplePtpLocalAddr));
         examplePtpLocalAddr[0].networkProtocol = ZL303XX_NETWORK_PROTOCOL_INVALID;
         memset(&examplePtpLocalAddr[1], 0, sizeof(examplePtpLocalAddr));
         examplePtpLocalAddr[1].networkProtocol = ZL303XX_NETWORK_PROTOCOL_INVALID;

      }
   }

   return status;
}

/* examplePtpDestIpv4SetDefault */
/**
   Sets the destination IPv4 address to use for the next created PTP stream.
   This function sets the examplePtpDestAddr structure.

  Parameters:
   [in]   ipv4Addr   IPv4 address in dot-notation.

*******************************************************************************/
zlStatusE examplePtpDestIpv4SetDefault(const char *ipv4Addr)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      if (ipv4Addr != NULL)
      {
         strncpy(examplePtpDestIpv4, ipv4Addr, sizeof(examplePtpDestIpv4) - 1);
         status = zl303xx_PtpConvertIPv4ToPortAddress(examplePtpDestIpv4, &examplePtpDestAddr[0]);
         status = zl303xx_PtpConvertIPv4ToPortAddress(examplePtpDestIpv4, &examplePtpDestAddr[1]);

      }
      else
      {
         memset(examplePtpDestIpv4, '\0', sizeof(examplePtpDestIpv4));
         memset(&examplePtpDestAddr[0], 0, sizeof(examplePtpDestAddr));
         examplePtpDestAddr[0].networkProtocol = ZL303XX_NETWORK_PROTOCOL_INVALID;
         memset(&examplePtpDestAddr[1], 0, sizeof(examplePtpDestAddr));
         examplePtpDestAddr[1].networkProtocol = ZL303XX_NETWORK_PROTOCOL_INVALID;
      }
   }

   return status;
}

/* examplePtpLocalIpv4Set */
/**
   Internal function used to set the local address of an examplePtpPortCreateS
   structure.

  Parameters:
   [in]   ipv4Addr   Address string (can be non-IPv4)
   [in]   forceAddr  If ZL303XX_FALSE and the examplePtpLocalAddr structure has
                          been previously set by examplePtpLocalIpv4SetDefault(),
                          that address structure will be used instead of ipv4Addr.
                          If ZL303XX_TRUE or the examplePtpLocalAddr structure has
                          not been setup, pPort will be filled with ipv4Addr.
   [out]  pPort      The localAddr member of the config structure is modified
                          by this function.

*******************************************************************************/
zlStatusE examplePtpLocalIpv4Set(
      const char *ipv4Addr,
      zl303xx_BooleanE forceAddr,
      examplePtpPortCreateS *pPort)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      /* Force the source IPv4 address or if the address is not set. */
      if ((forceAddr == ZL303XX_TRUE) ||
          (examplePtpLocalAddr[0].networkProtocol == ZL303XX_NETWORK_PROTOCOL_INVALID))

      {
         /*status = zl303xx_PtpConvertIPv4ToPortAddress(ipv4Addr, &pPort->config.localAddr);*/ /* Old method IPv4 only */
         status = examplePtpSocketStrToPortAddress(ipv4Addr, &pPort->config.localAddr); /* Accepts non-IPv4 strings */
      }
      else
      {
         if (pPort)
         {
            strncpy((char*)&pPort->config.localAddr, (char*)&examplePtpLocalAddr, sizeof(pPort->config.localAddr));
         }
         else
         {
            ZL303XX_ERROR_NOTIFY("examplePtpLocalIpv4Set: Port was null");
            status = ZL303XX_PARAMETER_INVALID;
         }
      }
   }

   return status;
}

/* examplePtpDestIpv4Set */
/**
   Internal function used to set the local address of an examplePtpStreamCreateS
   structure.

  Parameters:
   [in]   ipv4Addr   Address string (can be non-IPv4)
   [in]   forceAddr  If ZL303XX_FALSE and the examplePtpDestAddr structure has
                          been previously set by examplePtpDestIpv4SetDefault(),
                          that address structure will be used instead of ipv4Addr.
                          If ZL303XX_TRUE or the examplePtpDestAddr structure has
                          not been setup, pStream will be filled with ipv4Addr.
   [out]  pStream    The destAddr member of the config structure is modified
                          by this function.

*******************************************************************************/
zlStatusE examplePtpDestIpv4Set(
      const char *ipv4Addr,
      zl303xx_BooleanE forceAddr,
      examplePtpStreamCreateS *pStream)
{
   zlStatusE status = ZL303XX_CHECK_POINTERS(ipv4Addr, pStream);
   const char *ipAddr = NULL;

   if (status == ZL303XX_OK)
   {
      /* Only use the default destination IPv4 address if the global variable is not set. */
      if ((forceAddr == ZL303XX_TRUE) ||
            (examplePtpDestAddr[0].networkProtocol == ZL303XX_NETWORK_PROTOCOL_INVALID))
      {
         ipAddr = ipv4Addr;
         /*status = zl303xx_PtpConvertIPv4ToPortAddress(ipAddr, &pStream->config.destAddr);*/ /* Old method IPv4 only */
         status = examplePtpSocketStrToPortAddress(ipAddr, &pStream->config.destAddr); /* Accepts non-IPv4 strings */
      }
      else
      {
         ipAddr = (const char *)examplePtpDestAddr[0].addressField;
         memcpy(&pStream->config.destAddr, &examplePtpDestAddr[0], sizeof(pStream->config.destAddr));
      }

   #if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD || defined _ZL303XX_FMC_BOARD
      if (status == ZL303XX_OK)
      {
          setExampleMulticastIpDest(ipAddr);
      }
   #endif
   }

   return status;
}

/* examplePtpSrcEthL2Set */
/**
   Internal function used to set the source Ethernet Layer2 address of an
   examplePtpPortCreateS structure.

  Parameters:
   [in]   eth2Addr   Ethernet Layer2 address.
   [in]   forceAddr  If ZL303XX_FALSE and the examplePtpLocalAddr structure has
                          been previously set by examplePtpLocalIpv4SetDefault(),
                          that address structure will be used instead of ipv4Addr.
                          If ZL303XX_TRUE or the examplePtpLocalAddr structure has
                          not been setup, pPort will be filled with ipv4Addr.
   [out]  pPort      The localAddr member of the config structure is modified
                          by this function.

*******************************************************************************/
zlStatusE examplePtpSrcEthL2Set(const char *eth2Addr, zl303xx_BooleanE forceAddr,
                                examplePtpPortCreateS *pPort);
zlStatusE examplePtpSrcEthL2Set(
      const char *eth2Addr,
      zl303xx_BooleanE forceAddr,
      examplePtpPortCreateS *pPort)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      /* Only use the default destination IPv4 address if the global variable is not set. */
      if ((forceAddr == ZL303XX_TRUE) ||
          (examplePtpDestAddr[0].networkProtocol == ZL303XX_NETWORK_PROTOCOL_INVALID))
      {
         status = zl303xx_PtpConvertEth2ToPortAddress(eth2Addr, &pPort->config.localAddr);
      }
      else
      {
         memcpy(&pPort->config.localAddr, &examplePtpDestAddr[0], sizeof(pPort->config.localAddr));
      }
   }

   return status;
}
/* examplePtpDestEthL2Set */
/**
   Internal function used to set the destination Ethernet Layer2 address of an
   examplePtpPortCreateS structure.

  Parameters:
   [in]   eth2Addr   Ethernet Layer2 address.
   [in]   forceAddr  If ZL303XX_FALSE and the examplePtpLocalAddr structure has
                          been previously set by examplePtpLocalIpv4SetDefault(),
                          that address structure will be used instead of ipv4Addr.
                          If ZL303XX_TRUE or the examplePtpDestAddr structure has
                          not been setup, pPort will be filled with ipv4Addr.
   [out]  pStream    The localAddr member of the config structure is modified
                          by this function.

*******************************************************************************/
zlStatusE examplePtpDestEthL2Set(
      const char *eth2Addr,
      zl303xx_BooleanE forceAddr,
      examplePtpStreamCreateS *pStream)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      /* Only use the default destination IPv4 address if the global variable is not set. */
      if ((forceAddr == ZL303XX_TRUE) ||
          (examplePtpDestAddr[0].networkProtocol == ZL303XX_NETWORK_PROTOCOL_INVALID))
      {
         status = zl303xx_PtpConvertEth2ToPortAddress(eth2Addr, &pStream->config.destAddr);
      }
      else
      {
         memcpy(&pStream->config.destAddr, &examplePtpDestAddr[0], sizeof(pStream->config.destAddr));
      }
   }

   return status;
}

/* examplePtpStop */
/**
   Destroys previously created PTP clocks, ports, and streams.

*******************************************************************************/
zlStatusE examplePtpStop(void)
{
   zlStatusE status = ZL303XX_OK;

   /* The PTP clock(s), port(s), and stream(s) have event callouts that will
    * run when they are automatically deleted. */

   /* Node shut down will delete all PTP tasks and destroy all associated structures. */
   if (status == ZL303XX_OK && exampleAppRunning == ZL303XX_TRUE)
   {
       if (status == ZL303XX_OK)
       {
           status = zl303xx_PtpShutdown();    /* Needs msgQ to shutdown cleanly (and it waits 250ms)*/
           OS_TASK_DELAY(500);
       }

#ifdef EXAMPLE_PTP_SOCKET_TEST
       if (status == ZL303XX_OK)
       {
          status = examplePtpSocketEnvClose();
       }
#endif

   }

   return status;
}

/* examplePtpClockDelete */
/**
   Destroys a previously created PTP clock.

*******************************************************************************/
zlStatusE examplePtpClockDelete(zl303xx_PtpClockHandleT clockHandle)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpClockDeleteS clockDel;

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDeleteStructInit(&clockDel);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockDelete(clockHandle, &clockDel);
   }

   return status;
}

/* examplePtpPortDelete */
/**
   Destroys a previously created PTP port.

*******************************************************************************/
zlStatusE examplePtpPortDelete(zl303xx_PtpPortHandleT portHandle)
{
   zlStatusE status = ZL303XX_OK, xStatus = ZL303XX_OK;
   zl303xx_PtpPortDeleteS portDel;
   examplePtpSocketT ptpSkt;

   memset(&ptpSkt, 0, sizeof(examplePtpSocketT));

   if (status == ZL303XX_OK)
   {
      xStatus = zl303xx_PtpPortExtDataGet(portHandle, (void **)&ptpSkt);
   }
	/* なぜかxStatus使ってるのでwarning対応 */
   if (xStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDeleteStructInit(&portDel);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDelete(portHandle, &portDel);
   }

   if (status == ZL303XX_OK)
   {
       zlExampleApp.port->started = ZL303XX_FALSE;
   }


   return status;
}

/* examplePtpStreamDelete */
/**
   This function removes a PTP stream from a running system by:
   calling the event handler, clearing the UMT entry then clears
   the comms entry and the stream allocation array.

  Parameters:
   [in]   streamHandle   The PTP stream handle that was created.

  Return Value:   zlStatusE -   ZL303XX_OK or
                        ZL303XX_INVALID_POINTER
                        ZL303XX_PARAMETER_INVALID
                        return from zl303xx_PtpUmtEntryRemove()

*******************************************************************************/
zlStatusE examplePtpStreamDelete(zl303xx_PtpStreamHandleT streamHandle)
{
    const char *fnName = "examplePtpStreamDelete";
    zlStatusE status = ZL303XX_OK;
    Uint16T indx;
    zl303xx_ClockStreamS *pStream = NULL;
    zl303xx_PtpEventStreamDeleteS eventData;
    zl303xx_PtpStreamDeleteS streamDel;
    examplePtpStreamExtDataS *pStreamExtData = NULL;

    for (indx = 0; indx < ZL303XX_PTP_NUM_STREAMS_MAX; indx++)
    {
        if (Zl303xx_PtpStreamArray[indx] && Zl303xx_PtpStreamArray[indx]->config.requestedHandle == streamHandle)
        {
            zlStatusE mStatus = ZL303XX_ERROR;

            if (status == ZL303XX_OK)
            {
               mStatus = zl303xx_PtpNodeMutexLock();
               status = mStatus;
            }

            streamHandle = Zl303xx_PtpStreamArray[indx]->clockStreamHandle;

            if (mStatus == ZL303XX_OK)
            {
               status = zl303xx_PtpNodeMutexUnlock(status);
            }

            break;
        }
    }

    if (indx == ZL303XX_PTP_NUM_STREAMS_MAX)
    {
        status = ZL303XX_OK;
        ZL303XX_TRACE_ALWAYS("%s: streamHandle=%d was not found; Continue; status =%d",
                           fnName, streamHandle, status, 0,0,0);
    }

    if (status == ZL303XX_OK)
    {
        /* Get a pointer to the stream. */
        if ((status = zl303xx_PtpStreamDataGet(streamHandle, &pStream)) != ZL303XX_OK)
        {
            if (status == ZL303XX_PARAMETER_INVALID || status == ZL303XX_TABLE_ENTRY_NOT_FOUND)
            {
                ZL303XX_TRACE_ALWAYS("%s: stream=%lu was ALREADY REMOVED; returning status=%lu", fnName, streamHandle, status, 0,0,0);
                status = ZL303XX_OK;
            }
        }
        else
        {
            /* Delete the stream, if one exists. */
            if (status == ZL303XX_OK)
            {
                zl303xx_PtpClockS *pClock = pStream->portData->clock;

                /* Gather data for the STREAM_DELETE event. The event handler will be
                 * invoked because this is an automatic stream delete. */
                if (pClock->config.eventCallback != NULL)
                {
                    eventData.streamHandle =  pStream->clockStreamHandle;
                    eventData.clockHandle =   pClock->clockHandle;
                    eventData.streamExtData = pStream->config.extData;
                    eventData.streamConfig =  pStream->config;
                }

                /* Call the event handler, if it exists. */
                if (status == ZL303XX_OK)
                {
                    if (pClock->config.eventCallback != NULL)
                    {
                       pClock->config.eventCallback(
                          ZL303XX_PTP_EVENT_STREAM_DELETE, &eventData);
                    }
                }
            }
        }
    }

   /* Prep to remove the stream's data and comms entry */
   if (status == ZL303XX_OK &&
       (status = zl303xx_PtpStreamExtDataGet(streamHandle, (void **)&pStreamExtData)) != ZL303XX_OK)
   {
       ZL303XX_TRACE_ALWAYS("%s: ERROR zl303xx_PtpStreamExtDataGet=%lu", fnName, status, 0,0,0,0);
   }
   else
   {
   #ifdef EXAMPLE_PTP_SOCKET_TEST
       /* Delete the sockets, if they exist. */
       if (status == ZL303XX_OK)
       {
          if (pStreamExtData != NULL)
          {
             status = examplePtpSocketClose(&pStreamExtData->skt);
          }
       }
   #endif

       if (status == ZL303XX_OK && pStreamExtData)
       {
           examplePtpStreamExtDataFree(&pStreamExtData);
           /* Housekeeping */
           zlExampleApp.stream[streamHandle].ptp.config.extData = NULL;
       }
   }

   /* Housekeeping */
   if (status == ZL303XX_OK)
   {
      zlExampleApp.stream[streamHandle].started = ZL303XX_FALSE;
   }

   if (status == ZL303XX_OK &&
       (status = zl303xx_PtpStreamDeleteStructInit(&streamDel)) != ZL303XX_OK)
   {
       ZL303XX_TRACE_ALWAYS("examplePtpStreamDelete:  zl303xx_PtpStreamDeleteStructInit=%lu", status, 0,0,0,0,0);
   }

   if (status == ZL303XX_OK &&
       (status = zl303xx_PtpStreamDelete(streamHandle, &streamDel)) != ZL303XX_OK)
   {
       ZL303XX_TRACE_ALWAYS("examplePtpStreamDelete:  zl303xx_PtpStreamDelete=%lu", status, 0,0,0,0,0);
   }


   if (status == ZL303XX_OK)
       ZL303XX_TRACE_ALWAYS("%s: streamHandle=%lu was REMOVED.", fnName, streamHandle, 0,0,0,0);

   return status;
}

/* examplePtpContractsRequest */
/**
   Requests contracts for multiple message types. Can only be called after
   exampleAppStart().

  Parameters:
   [in]   pStream      Pointer to an example stream structure.
   [in]   msgTypeList  Bitmap of PTP message types to request.

*******************************************************************************/
zlStatusE examplePtpContractsRequest(examplePtpStreamCreateS *pStream,
                                     Uint32T msgTypeList)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_MessageTypeE msgType;

   for (msgType = 0;
        status == ZL303XX_OK && msgTypeList != 0;
        msgType++, msgTypeList >>= 1)
   {
      if (msgTypeList & 1)
      {
         status = zl303xx_PtpContractRequest(pStream->handle, msgType, NULL);
      }
   }

   return status;
}

/* examplePtpUmtEntryPrioritySet */
/**
   Sets the priority of a UMT entry. This only affects Telecom Profile mode.
   Can only be called after exampleAppStart().

  Parameters:
   [in]   pStream   Pointer to an example stream structure.
   [in]   priority  Priority value to set.

*******************************************************************************/
zlStatusE examplePtpUmtEntryPrioritySet(examplePtpStreamCreateS *pStream,
                                        Uint8T priority)
{
   return zl303xx_PtpUmtEntryPrioritySet(pStream->handle, priority);
}

/* examplePtpRxMsgProcess */
/**
   Sends a received message to the PTP task for processing. Searches entire
   PTP Clock for the first Stream that either:
   a) Has stream destAddr == *pSrcAddr
   b) Has stream destAddr == *pDestAddr (for multicast)

*******************************************************************************/
zlStatusE examplePtpRxMsgProcess(Uint8T *buffer, Uint16T bufferLen,
                                 zl303xx_PortAddress *pSrcAddr,
                                 zl303xx_PortAddress *pDestAddr)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpRxMsgDataS rxMsgData;

   if (status == ZL303XX_OK)
   {
      rxMsgData.srcAddr  = *pSrcAddr;
      rxMsgData.destAddr = *pDestAddr;
   }

   if (status == ZL303XX_OK)
   {
#if defined EXAMPLE_PTP_SW_TIMESTAMPS
/* #warning MSCC: Not a normal path through the code! */
      examplePtpSwTimeStampGet(&rxMsgData.rxTs);
#endif

/*
    {
        int i;

        printf("examplePtpRxMsgProcess: ");
        for (i=0; i<bufferLen; i++)
            printf(" %x", buffer[i]);
        printf("\n");
    }
*/

#if (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD) && defined OS_LINUX
      status = ZLE30360_PtpRxMsgProcess(buffer, bufferLen, &rxMsgData);
#endif

   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpRxMsgProcess(buffer, bufferLen, &rxMsgData);
      if (status != ZL303XX_OK)
      {
          ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "examplePtpRxMsgProcess: zl303xx_PtpRxMsgProcess() failed=%lu", status, 0,0,0,0,0);
          ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1, "examplePtpRxMsgProcess: zl303xx_PtpRxMsgProcess() failed=%lu", status, 0,0,0,0,0);
      }
   }

   return status;
}

/* examplePtpPortRxMsgProcess */
/**
   Sends a received message to the PTP task for processing on a specific
   PTP Port. Searches single PTP Port for the first Stream that either:
   a) Has Stream::destAddr == pSrcAddr
   b) Has Stream::destAddr == pDestAddr (for multicast)
   
   プロト版でIF変更

*******************************************************************************/
zlStatusE examplePtpPortRxMsgProcess(Sint32T fd,
                                 zl303xx_PtpPortHandleT portHandle,
                                 Uint8T *buffer, Uint16T bufferLen,
                                 zl303xx_PortAddress *pSrcAddr,
                                 zl303xx_PortAddress *pDestAddr)
{
	
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpPortRxMsgDataS rxMsgData;

   if (status == ZL303XX_OK)
   {
      rxMsgData.portHandle = portHandle;
      rxMsgData.srcAddr  = *pSrcAddr;
      rxMsgData.destAddr = *pDestAddr;
   }

   if (status == ZL303XX_OK)
   {
/*
#if defined EXAMPLE_PTP_SW_TIMESTAMPS
#warning MSCC: Not a normal path through the code!
      examplePtpSwTimeStampGet(&rxMsgData.rxTs);
#endif
*/

		/* SYNCの場合だけTimestamp取得を実施 */
		Uint4T   messageType = UNPACK_LO_NIBBLE(buffer + ZL303XX_PTP_V2_HDR_MSG_TYPE_OFST);
		if( messageType == ZL303XX_MSG_ID_SYNC)
		{
			Uint16T sequenceId = zl303xx_PtpV2MsgSequenceNumGet(buffer);
			Get25gTimestamps(fd , sequenceId, &rxMsgData.rxTs,0);
		}

#if (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD) && defined OS_LINUX
      status = ZLE30360_PtpPortRxMsgProcess(buffer, bufferLen, &rxMsgData);
#endif

   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortRxMsgProcess(buffer, bufferLen, &rxMsgData);
      if (status != ZL303XX_OK)
      {
          ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "examplePtpPortRxMsgProcess: zl303xx_PtpPortRxMsgProcess() failed=%lu", status, 0,0,0,0,0);
          ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1, "examplePtpPortRxMsgProcess: zl303xx_PtpPortRxMsgProcess() failed=%lu", status, 0,0,0,0,0);
      }
   }

   return status;
}

/* examplePtpEventHandler */
/**
   Function that is called for any PTP event output. It calls the appropriate
   handler based on the type of event.

  Parameters:
   [in]   eventType  Type of PTP event that occurred.
   [in]   eventData  Pointer to a data structure specific to the event type.
                          May be NULL.

  Return Value:   N/A

*******************************************************************************/
void examplePtpEventHandler(zl303xx_PtpEventE eventType, void *eventData)
{
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
         "examplePtpEventHandler: eventType=%lu",
         (Uint32T)eventType, 0,0,0,0,0);

   switch (eventType)
   {
      case ZL303XX_PTP_EVENT_CONTRACT_REJECTED :
         examplePtpEventContractRejected(eventData);
         break;

      case ZL303XX_PTP_EVENT_CLOCK_CREATE :
         examplePtpEventClockCreate(eventData);
         break;

      case ZL303XX_PTP_EVENT_CLOCK_DELETE :
         examplePtpEventClockDelete(eventData);
         break;

      case ZL303XX_PTP_EVENT_PORT_DELETE :
         examplePtpEventPortDelete(eventData);
         break;

      case ZL303XX_PTP_EVENT_STREAM_CREATE :
         examplePtpEventStreamCreate(eventData);
         break;

      case ZL303XX_PTP_EVENT_STREAM_DELETE :
         examplePtpEventStreamDelete(eventData);
         break;

      case ZL303XX_PTP_EVENT_STREAM_STATE_CHANGE :
         examplePtpEventStreamStateChange(eventData);
         break;

      case ZL303XX_PTP_EVENT_STREAM_KEEP_ALIVE :
         examplePtpEventStreamKeepAlive(eventData);
         break;

      case ZL303XX_PTP_EVENT_CLOCK_BMCA_UPDATE :
         examplePtpEventClockBmcaUpdate(eventData);
         break;

      case ZL303XX_PTP_EVENT_SERVO_DATA :
         examplePtpEventServoData(eventData);
         break;

      case ZL303XX_PTP_EVENT_COUNTER_ALARM :
         examplePtpEventCounterAlarm(eventData);
         break;

      case ZL303XX_PTP_EVENT_SQUELCH :
         examplePtpEventSquelch(eventData);
         break;

      case ZL303XX_PTP_EVENT_MULTIPLE_PEER_RESP :
         examplePtpEventMultiplePeerResp(eventData);
         break;

      case ZL303XX_PTP_EVENT_MSG_INTVL_CHANGE :
         examplePtpEventMsgIntvlChange(eventData);
         break;

      case ZL303XX_PTP_EVENT_CONTRACT_GRANTED :
         examplePtpEventContractGranted(eventData);
         break;

      case ZL303XX_PTP_EVENT_CONTRACT_EXPIRED :
         examplePtpEventContractExpired(eventData);
         break;

      case ZL303XX_PTP_EVENT_UNKNOWN_TLV :
         examplePtpEventUnknownTlv(eventData);
         break;

      case ZL303XX_PTP_EVENT_LEAP_SECONDS_FLAG_CHANGE :
         examplePtpEventLeapSecondsFlagChange(eventData);
         break;

      case ZL303XX_PTP_EVENT_UTC_OFFSET_CHANGE :
         examplePtpEventUtcOffsetChange(eventData);
         break;

      case ZL303XX_PTP_EVENT_RX_STEPS_REMOVED_EXCEEDED :
         examplePtpEventRxStepsRemovedExceeded(eventData);
         break;

      case ZL303XX_PTP_EVENT_PORT_FAULTY:
         examplePtpEventPortFaulty(eventData);
         break;

      case ZL303XX_PTP_EVENT_STREAM_DISQUALIFIED:
         examplePtpEventStreamDisqualified(eventData);
         break;

      /* This is generated periodically. */
      case ZL303XX_PTP_EVENT_PARENT_DATA_SET_CHANGE :
         examplePtpEventParentDsUpdated(eventData);
         break;

      /* Deprecated events (no longer generated) */
      case ZL303XX_PTP_EVENT_SERVO_STATUS_GET :
      case ZL303XX_PTP_EVENT_PTSF_CHANGE :

      /* Default cases - fall through for switch-enum warning */
      case ZL303XX_PTP_EVENT_NONE :
      case ZL303XX_PTP_EVENT_NUM_TYPES :
      default :
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
               "   Unknown or unhandled event=%d.",
               eventType, 0,0,0,0,0);
         break;
   }
}

/* examplePtpHwCmdHandler */
/**
   Function that is called when PTP needs to query information from the hardware
   device or perform some hardware function. It calls the appropriate handler
   based on the type of command.

  Parameters:
   [in]   cmdType     Type of command PTP is attempting.
   [in]   cmdParams   Pointer to a data structure specific to the command type.

  Return Value:   0   Success. Otherwise, return some error code.

*******************************************************************************/
Sint32T examplePtpHwCmdHandler(zl303xx_PtpHwCmdE cmdType, void *cmdParams)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 4,
         "examplePtpHwCmdHandle: cmdType=%lu",
         (Uint32T)cmdType, 0,0,0,0,0);

   switch (cmdType)
   {
      case ZL303XX_PTP_HW_CMD_CLOCK_TIME_GET :
         status = examplePtpHwClockTimeGet(cmdParams);
         break;

      case ZL303XX_PTP_HW_CMD_CLOCK_TIME_SET :
         /* Function to set the PTP time of the underlying hardware clock. This
          * function is called for an ingress TIME SET management TLV or when
          * zl303xx_PtpSetLocalUtcTime() is called. */
         status = examplePtpHwClockTimeSet(cmdParams);
         break;

      case ZL303XX_PTP_HW_CMD_PHYS_ADDR_GET :
         /* Function to retrieve data about the physical address of a hardware
          * network port. */
         break;

      case ZL303XX_PTP_HW_CMD_LOCK_STATUS_GET :
         /* Function to get the local clock LOCK status. */
         status = examplePtpHwLockStatusGet(cmdParams);
         break;

      case ZL303XX_PTP_HW_CMD_CLOCK_STABILITY_GET :
         /* Function to get the local clock stability data. */
         status = examplePtpHwClockStabilityGet(cmdParams);
         break;

      case ZL303XX_PTP_HW_CMD_TIMESET_STATUS_GET :
         /* Function to get the local clock TIME status. */
         status = examplePtpHwTimeSetStatusGet(cmdParams);
         break;

      case ZL303XX_PTP_HW_CMD_PLL_PERF_DATA_GET :
         /* Function to get the local PLL Performance Status. */
         status = examplePtpHwPllPerformanceGet(cmdParams);
         break;

      /* Default cases - fall through for switch-enum warning */
      case ZL303XX_PTP_HW_CMD_NUM_TYPES :
      default :
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "   No handler exists for this hardware command.",
               0,0,0,0,0,0);

         status = ZL303XX_ERROR;
         break;
   }

   return (Sint32T)status;
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/* examplePtpEventContractRejected */
/**
   An example callout function called during a failed negotiated contract.

  Parameters:
   [in]   eventData  Pointer to the data associated with
                          ZL303XX_PTP_EVENT_CONTRACT_REJECTED.

  Return Value:   N/A

  Notes:   This function is expected to be modified by the end-user.

*******************************************************************************/
static void examplePtpEventContractRejected(zl303xx_PtpEventContractRejectedS *eventData)
{
   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3,
         "examplePtpEventContractRejected:", 0,0,0,0,0,0);

   if (eventData == NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "   (ERROR) eventData is NULL.", 0,0,0,0,0,0);

      return;
   }

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "Stream %u: Contract rejected: messageId=%u, errorType=%d",
         eventData->handle,
         eventData->messageId,
         eventData->errorType, 0,0,0);
}

/* examplePtpStreamExtDataAlloc */
/**
   Allocates memory for an examplePtpStreamExtDataS structure and returns a
   pointer to it.

*******************************************************************************/
static zlStatusE examplePtpStreamExtDataAlloc(examplePtpStreamExtDataS **ppExt)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      *ppExt = OS_CALLOC(1, sizeof(examplePtpStreamExtDataS));

      if (*ppExt == NULL)
      {
         ZL303XX_TRACE_ALWAYS("Error allocating memory for external stream data.", 0,0,0,0,0,0);
         status = ZL303XX_RTOS_MEMORY_FAIL;
      }
   }
   return status;
}

/* examplePtpStreamExtDataFree */
/**
   Frees memory for an examplePtpStreamExtDataS structure and sets the pointer
   to NULL.

*******************************************************************************/
void examplePtpStreamExtDataFree(examplePtpStreamExtDataS **ppExt)
{
    if (ppExt && *ppExt)
    {
        if (*ppExt)
        {
            OS_FREE(*ppExt);
            *ppExt = NULL;
        }
    }
}


/* examplePtpTx */
/**
   Function to bind to a PTP port's message transmit callout. This code should
   be modified to call the system's transport layer.

  Parameters:
   [in]   buffer      Byte-buffer with the PTP message in network order.
   [in]   bufLen      Length of the buffer.
   [in]   pTxMsgData  Extra information about the message.

  Return Value:   0   Success. Otherwise, return an error code.

*******************************************************************************/
Sint32T examplePtpTx(Uint8T *buffer, Uint16T bufferLen,
                     zl303xx_PtpTxMsgDataS *pTxMsgData)
{
    zlStatusE status = ZL303XX_OK;
    examplePtpSocketT skt;
    zl303xx_PtpInterfaceE inf;
   
   #define MAX_CONTENT_LEN 1500
    if (!(pTxMsgData && buffer))
    {
        ZL303XX_TRACE_ALWAYS("examplePtpTx: NULL pointer - pTxMsgData: %p buffer: %p ; RETURNED -1", pTxMsgData, buffer, 0,0,0,0);
        return -1;
    }

    if (bufferLen > MAX_CONTENT_LEN)
    {
        ZL303XX_TRACE_ALWAYS("examplePtpTx: bufferLen=%d > 1500; RETURNED -1", bufferLen, 0,0,0,0,0);
        return -1;
    }

#if (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD) && defined OS_LINUX
    if (status == ZL303XX_OK)
    {
        status = ZLE30360_PtpTxMsgProcess(buffer, bufferLen, pTxMsgData);
    }
#endif

   /* INSERT CODE here to transmit the PTP message. */

  #if defined EXAMPLE_PTP_SOCKET_TEST || defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
   if (status == ZL303XX_OK)
   {

        examplePtpStreamExtDataS *pExtData = pTxMsgData->streamExtData;
        /* examplePtpSocketT skt; */

        if (pExtData && pExtData->skt)
        {
            /* Use stream's socket if it has one (for egress TS) */
            skt = pExtData->skt;
        }
        else
        {
            /* Otherwise use port's socket */
            skt = pTxMsgData->portExtData;
        }

        status = examplePtpSocketTx(skt,
                                  buffer, bufferLen,
                                  &pTxMsgData->destAddr,
                                  pTxMsgData->interface);
   }
  #else
   if (buffer && bufferLen && pTxMsgData) {;}   /* Unused variable warning removal */
  #endif

   /* INSERT CODE here to associate the transmit time stamp with the PTP message
    * with this sequenceId. */

  /* #ifdef EXAMPLE_PTP_SW_TIMESTAMPS */
  /* #warning MSCC: S/W timestamps are not accurate enough for timing recovery - socket test mode only! */
   if (status == ZL303XX_OK)
   {
      if (((pTxMsgData->messageId == ZL303XX_MSG_ID_SYNC) && (pTxMsgData->flagField & PTP2_FLAGS_TWO_STEP)) ||
          (pTxMsgData->messageId  == ZL303XX_MSG_ID_DELAY_REQ) ||
          (pTxMsgData->messageId  == ZL303XX_MSG_ID_PEER_DELAY_REQ) ||
          ((pTxMsgData->messageId == ZL303XX_MSG_ID_PEER_DELAY_RESP) && (pTxMsgData->flagField & PTP2_FLAGS_TWO_STEP)))
      {
         zl303xx_PtpTsRecordS tsRecord;

         tsRecord.streamHandle = pTxMsgData->streamHandle;
         tsRecord.messageType  = pTxMsgData->messageId;
         tsRecord.sequenceId   = pTxMsgData->sequenceId;
         /* examplePtpSwTimeStampGet(&tsRecord.timeStamp); */

        if (pTxMsgData->messageId  == ZL303XX_MSG_ID_DELAY_REQ)
		{
			inf = pTxMsgData->interface;
			Get25gTimestamps(skt->socket[inf].fd ,pTxMsgData->sequenceId, &tsRecord.timeStamp,1);
		}
		else
		{
			inf = pTxMsgData->interface;
			ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2," ELSE(0x%x) Message : Get Timestamp. seq=%d", pTxMsgData->messageId,pTxMsgData->sequenceId,0,0,0,0);
		}
		
        status = zl303xx_PtpTsRecordProcess(&tsRecord);
      }
   }
  /* #endif */

   return (Sint32T)status;
}

/* examplePtpEventClockCreate */
/**
   A function to handle the creation of a new PTP Clock.

  Parameters:
   [in]   eventData   Data about the clock creation.

  Return Value:   N/A

*******************************************************************************/
static void examplePtpEventClockCreate(zl303xx_PtpEventClockCreateS *eventData)
{
   const char *fnName = "examplePtpEventClockCreate";

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3, "%s:", fnName, 0,0,0,0,0);

   if (eventData == NULL)
   {
      ZL303XX_TRACE_ALWAYS("%s: (ERROR) eventData is NULL.",
            fnName, 0,0,0,0,0);
      return;
   }

#if defined ZL303XX_REFERENCE_SELECTION
   /* Send an update for this PTP Clock to the Server Selection Handler. */
   {
      /***************************************************************************/
      /* Translate the PTP Event message into a type for transmission over the
       * distributed backplane.                                                  */
      /***************************************************************************/
      exampleRefSelectBpMsgS txMsg;

      /* First, create a Telecom data set from the Clock to attach to the message. */
      zl303xx_PtpTelecomCompareDataS telecomData;
      {
         /* Assume the hardware is valid as a last resort. */
         telecomData.ptsf = ZL303XX_FALSE;
         telecomData.priority = 0;     /* the local clock is lowest priority. */
         telecomData.stepsRemoved = 0;

         /* Other configuration values. */
         telecomData.clockClass = eventData->clockDefault.grandmasterClockQuality.clockClass;
         memcpy(telecomData.grandmasterIdentity,
                eventData->clockDefault.grandmasterIdentity,
                sizeof(telecomData.grandmasterIdentity));
      }

      /* UPDATE message from the Application. */
      txMsg.source = REF_SELECT_SRC_MOD_PROT_CLOCK;
      txMsg.action = REF_SELECT_ACTION_UPDATE;

      /* Translate the message data. */
      txMsg.data.clockUpdate.clockHandle = eventData->clockHandle;
      txMsg.data.clockUpdate.profile = eventData->profile;
      txMsg.data.clockUpdate.bmcaData = eventData->clockDefault;
      txMsg.data.clockUpdate.telecomData = telecomData;

   #if defined APR_INCLUDED
      /***************************************************************************/
      /* Ordinarily, the data would be sent over the backplane at this point and
       * the next command executed within the context of the Timing Card CPU.
       * Here, both run on a single CPU.                                         */
      /***************************************************************************/

      /* Send the Data to the Server Database. */
      (void)exampleRefSelectSendMsgToBackplane(&txMsg);
   #else
   /* #warning MSCC: Translation of PTP message to a Backplane message is required. */
      ZL303XX_TRACE_ALWAYS("Line Card Event ready for BP transmission (%d)",
            ZL303XX_PTP_EVENT_CLOCK_BMCA_UPDATE, 0,0,0,0,0);
   #endif
   }
#endif
   return;
}

/* examplePtpEventClockDelete */
/**
   A function to handle the deletion of a PTP Clock.

  Parameters:
   [in]   eventData   Data about the deleted clock.

  Return Value:   N/A

*******************************************************************************/
static void examplePtpEventClockDelete(zl303xx_PtpEventClockDeleteS *eventData)
{
   const char *fnName = "examplePtpEventClockDelete";

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3, "%s:", fnName, 0,0,0,0,0);

   if (eventData == NULL)
   {
      ZL303XX_TRACE_ALWAYS("%s: (ERROR) eventData is NULL.",
            fnName, 0,0,0,0,0);
      return;
   }

#if defined ZL303XX_REFERENCE_SELECTION
   /* Send an update for this PTP Clock to the Server Selection Handler. */
   {
      /***************************************************************************/
      /* Translate the PTP Event message into a type for transmission over the
       * distributed backplane.                                                  */
      /***************************************************************************/
      exampleRefSelectBpMsgS txMsg;

      /* UPDATE message from the Application. */
      txMsg.source = REF_SELECT_SRC_MOD_PROT_CLOCK;
      txMsg.action = REF_SELECT_ACTION_DELETE;

      /* Translate the message data. */
      txMsg.data.clockDelete.clockHandle = eventData->clockHandle;

   #if defined APR_INCLUDED
      /***************************************************************************/
      /* Ordinarily, the data would be sent over the backplane at this point and
       * the next command executed within the context of the Timing Card CPU.
       * Here, both run on a single CPU.                                         */
      /***************************************************************************/

      /* Send the Data to the Server Database. */
      (void)exampleRefSelectSendMsgToBackplane(&txMsg);
   #else
   /* #warning MSCC: Translation of PTP message to a Backplane message is required. */
      ZL303XX_TRACE_ALWAYS("Line Card Event ready for BP transmission (%d)",
            ZL303XX_PTP_EVENT_CLOCK_DELETE, 0,0,0,0,0);
   #endif
   }
#endif
   return;
}

/* examplePtpEventPortDelete */
/**
   This function deletes the associated transport structures when a PTP task
   automatically deletes a port.

  Parameters:
   [in]   eventData   Data about the port being deleted.

  Return Value:   N/A

*******************************************************************************/
static void examplePtpEventPortDelete(zl303xx_PtpEventPortDeleteS *eventData)
{
   /* INSERT CODE here to delete/modify transport layer structures as necessary. */

#ifdef EXAMPLE_PTP_SOCKET_TEST
   {
      zlStatusE status = ZL303XX_OK;

      /* Delete the unicast sockets. */
      if (status == ZL303XX_OK)
      {
         eventData->portExtData = NULL;
      }
   }
#else
   if (eventData) {;}   /* Unused variable warning removal */
#endif
}

#if defined APR_INCLUDED
#if defined MUX_PTP_STREAMS_TO_APR_SLOTS

void zl303xx_InitPtpStreamMuxToAprSlots(void)
{
    const char *fnName = "zl303xx_InitPtpStreamMuxToAprSlots";
    Sint16T indx;
    Uint16T maxPtpStreams = ZL303XX_PTP_NUM_STREAMS_MAX;

    for (indx = 0; indx < maxPtpStreams; indx++)
    {
        muxPtpStreamsToAprSlotsS[indx].aprSlotCurrentActiveRef = ZL303XX_FALSE;
        muxPtpStreamsToAprSlotsS[indx].aprServerSlot = -1;
        muxPtpStreamsToAprSlotsS[indx].streamHandle = -1;
    }

    ZL303XX_TRACE_ALWAYS("%s: STREAMS_PER_SLOT_MUX=%d = (ZL303XX_PTP_NUM_STREAMS_MAX=%d / NUM_APR_SLOTS_MUX=%d)",
                       fnName, STREAMS_PER_SLOT_MUX, ZL303XX_PTP_NUM_STREAMS_MAX, NUM_APR_SLOTS_MUX, 0,0);
}


/* zl303xx_MuxPtpStreamToAprSlots */
/**
   This function muxes a number of PTP streams into a few APR slots.
    BMCA (or an alternative) will pick the best server from all
    active PTP streams and that stream will be used to drive APR (See
    zl303xx_NewBestPtpStreamToAprSlot() ).

   During runtime, PTP STREAM HANDLES are replaced by the concept of APR SERVER SLOTS
    (aprServerSlot) during stream creation, deletion and when BMCA selects a different
    stream.

   ALL streams in a slot must be CONFIGURED THE SAME to ensure proper APR operation
    (i.e., configure the SAME CIDs TOGETHER into slots).

   During configuration, the logic used is to fill the lowest empty
    PTP stream index therefore all streams should be configured together into slots.
    Use BOGUS entries to fill up a slot and configure a different CID into the next slot.

   The first stream in each slot will be the DEFAULT ACTIVE SERVER FOR
    THAT SLOT and will be dynamically created and then monitored by APR.

   There are (ZL303XX_PTP_NUM_STREAMS_MAX / NUM_APR_SLOTS_MUX) streams per slot.
    The default configuration is:
    128/8 = 16 ptp streams muxed to each APR slot.
    Modify ZL303XX_PTP_NUM_STREAMS_MAX and/or NUM_APR_SLOTS_MUX to define your
    strategy (see zl303xx_TSALimits.h).


  Parameters:
   [in]   aprStream   Pointer to the APR stream create struct.

  Return Value:   zlStatusE -   ZL303XX_OK or
                        ZL303XX_PARAMETER_INVALID
                        ZL303XX_INVALID_POINTER
                        returns from exampleAprStreamCreate()

*******************************************************************************/
static zlStatusE zl303xx_MuxPtpStreamToAprSlots(exampleAprStreamCreateS *aprStream);
static zlStatusE zl303xx_MuxPtpStreamToAprSlots(exampleAprStreamCreateS *aprStream)
{
    const char *fnName = "zl303xx_MuxPtpStreamToAprSlots";
    zlStatusE status = ZL303XX_OK;
    Sint16T indx, firstOpenIndex = -1;
    Uint16T maxPtpStreams = ZL303XX_PTP_NUM_STREAMS_MAX;
    Sint16T aprServerSlot, muxServerSlot, muxActiveIndex = -1;
    Uint32T requestedHandle = 0;
    zl303xx_BooleanE anotherOnMuxSlot = ZL303XX_FALSE;

    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 2, "%s:", fnName, 0,0,0,0,0);

    if (STREAMS_PER_SLOT_MUX == 0)
    {
        ZL303XX_TRACE_ALWAYS("%s: (ERROR) STREAMS_PER_SLOT_MUX is 0.",
                           fnName, 0,0,0,0,0);
        return ZL303XX_PARAMETER_INVALID;
    }
    if (NUM_APR_SLOTS_MUX == 0)
    {
        ZL303XX_TRACE_ALWAYS("%s: (ERROR) NUM_APR_SLOTS_MUX is 0.",
                           fnName, 0,0,0,0,0);
        return ZL303XX_PARAMETER_INVALID;
    }
    if (NUM_APR_SLOTS_MUX > ZL303XX_APR_MAX_NUM_MASTERS)
    {
        ZL303XX_TRACE_ALWAYS("%s: (ERROR) NUM_APR_SLOTS_MUX cannot exceed %d.",
                           fnName, ZL303XX_APR_MAX_NUM_MASTERS, 0,0,0,0);
        return ZL303XX_PARAMETER_INVALID;
    }

    status = ZL303XX_CHECK_POINTER(aprStream);
    if (status == ZL303XX_OK)
    {
        requestedHandle = aprStream->server.serverId;

        /* Find first unused PTP slot */
        for (indx = 0; indx < maxPtpStreams; indx++)
        {
            aprServerSlot = (indx / STREAMS_PER_SLOT_MUX);

            if (muxPtpStreamsToAprSlotsS[indx].streamHandle == (zl303xx_PtpStreamHandleT)-1 && firstOpenIndex == -1)
            {
                firstOpenIndex = indx;
                muxServerSlot = aprServerSlot;
                muxPtpStreamsToAprSlotsS[firstOpenIndex].streamHandle =  aprStream->server.serverId;
                muxPtpStreamsToAprSlotsS[firstOpenIndex].aprServerSlot = aprServerSlot; /* Mux STREAMS_PER_SLOT_MUX into slots */
                continue;
            }

            /* Check for Dups */
            if (muxPtpStreamsToAprSlotsS[indx].streamHandle == aprStream->server.serverId)
            {
                status = ZL303XX_TABLE_ENTRY_DUPLICATE;
                ZL303XX_TRACE_ALWAYS("%s: Entry for serverId=%d ALREADY EXISTS; status =%d",
                                   fnName,  aprStream->server.serverId, status, 0,0,0);
                break;
            }
        }

        if (status == ZL303XX_OK && firstOpenIndex >= 0)
        {
            for (indx = 0; indx < maxPtpStreams; indx++)
            {
                aprServerSlot = (indx / STREAMS_PER_SLOT_MUX);

                /* Find any others on the same mux and which is active on the same slot */
                if (muxPtpStreamsToAprSlotsS[indx].streamHandle != (Uint32T)-1 && indx != firstOpenIndex && aprServerSlot == muxServerSlot)
                {
                    anotherOnMuxSlot = ZL303XX_TRUE;

                    if (muxPtpStreamsToAprSlotsS[indx].aprSlotCurrentActiveRef)
                    {
                        muxActiveIndex = indx;
                        break;
                    }
                }
            }
        }

        if (status == ZL303XX_OK)
        {
            /* Add ptp stream to a slot */
            if (firstOpenIndex != -1)
            {
                ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3, "%s: ADDED requestedHandle = %d at ptpIndex = %d to mux slot = %d",
                            fnName,  aprStream->server.serverId, firstOpenIndex, muxPtpStreamsToAprSlotsS[firstOpenIndex].aprServerSlot, 0,0);

                /* Create new APR stream if no others for this slot */
                if (!anotherOnMuxSlot)
                {
                    aprStream->server.serverId = muxPtpStreamsToAprSlotsS[firstOpenIndex].aprServerSlot;   /* ServerId is remapped to slotId */

                    if ((status = exampleAprStreamCreate(aprStream)) != ZL303XX_OK)
                    {
                        ZL303XX_TRACE_ALWAYS("%s: Call to exampleAprStreamCreate() for aprServerSlot = %d failed =%d",
                                           fnName, muxPtpStreamsToAprSlotsS[firstOpenIndex].aprServerSlot, status, 0,0,0);
                    }
                    else
                    {
                        muxPtpStreamsToAprSlotsS[firstOpenIndex].aprSlotCurrentActiveRef = ZL303XX_TRUE;
                        ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3, "%s: ADDED NEW APR SLOT = %d on device=%p",
                                    fnName,  muxPtpStreamsToAprSlotsS[firstOpenIndex].aprServerSlot, aprStream->cguId, 0,0,0);
                    }
                }
            }
            else    /* Broken! */
            {
                status = ZL303XX_TABLE_FULL;
                ZL303XX_TRACE_ALWAYS("%s: NO EMPTY STREAMS in muxPtpStreamsToAprSlotsS; status = %d",
                                   fnName, status, 0,0,0,0);
            }
        }

        /* Find the original PTP requestedHandle in Zl303xx_PtpStreamArray and store the matching aprSlotHandle */
        if (status == ZL303XX_OK)
        {
            for (indx = 0; indx < maxPtpStreams; indx++)
            {
                if (Zl303xx_PtpStreamArray[indx] &&
                    Zl303xx_PtpStreamArray[indx]->clockStreamHandle == requestedHandle)
                {
                    zlStatusE mStatus = ZL303XX_ERROR;

                    if (status == ZL303XX_OK)
                    {
                       mStatus = zl303xx_PtpNodeMutexLock();
                       status = mStatus;
                    }

                    Zl303xx_PtpStreamArray[indx]->aprSlotHandle = muxPtpStreamsToAprSlotsS[firstOpenIndex].aprServerSlot;

                    if (mStatus == ZL303XX_OK)
                    {
                       status = zl303xx_PtpNodeMutexUnlock(status);
                    }

                        ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3, "%s: MAPPED APR SLOT HANDLE = %d to requestedHandle = %d at index = %d",
                                    fnName,  muxPtpStreamsToAprSlotsS[firstOpenIndex].aprServerSlot, requestedHandle, indx, 0,0);

                        break;
                }
            }

            if (indx >= maxPtpStreams)   /* Broken! */
            {
                status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
                ZL303XX_TRACE_ALWAYS("%s: PTP STREAM NOT FOUND in Zl303xx_PtpStreamArray; status = %d",
                                   fnName, status, 0,0,0,0);
            }
        }
    }

    return status;
}

/* zl303xx_DemuxPtpStreamFromAprSlots */
/**
   This function will de-muxes a PTP stream from an APR slot.

    If this stream was the current server for APR then the
    first other stream on the same APR slot (firstOtherOnSameMux) will become
    the active reference for APR.
    If there are no others left on the same slot then the slot is removed from
    APR.

  Parameters:
   [in]   eventData   Pointer to the Event stream delete struct.

  Return Value:   zlStatusE -   ZL303XX_OK or
                        ZL303XX_PARAMETER_INVALID
                        ZL303XX_INVALID_POINTER
                        return from exampleAprStreamRemove()

*******************************************************************************/
static zlStatusE zl303xx_DemuxPtpStreamFromAprSlots(zl303xx_PtpEventStreamDeleteS *eventData);
static zlStatusE zl303xx_DemuxPtpStreamFromAprSlots(zl303xx_PtpEventStreamDeleteS *eventData)
{
    const char *fnName = "zl303xx_DemuxPtpStreamFromAprSlots";
    zlStatusE status = ZL303XX_OK;
    Sint16T indx, remIndx;
    Uint16T maxPtpStreams = ZL303XX_PTP_NUM_STREAMS_MAX;
    Sint16T aprServerSlot = -1;

    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 2, "%s:", fnName, 0,0,0,0,0);

    if (eventData == NULL)
    {
       ZL303XX_TRACE_ALWAYS("%s: (ERROR) eventData is NULL.",
                          fnName, 0,0,0,0,0);
       return ZL303XX_INVALID_POINTER;
    }

    if (STREAMS_PER_SLOT_MUX == 0)
    {
        ZL303XX_TRACE_ALWAYS("%s: (ERROR) STREAMS_PER_SLOT_MUX is 0.",
                           fnName, 0,0,0,0,0);
        return ZL303XX_PARAMETER_INVALID;
    }
    if (NUM_APR_SLOTS_MUX == 0)
    {
        ZL303XX_TRACE_ALWAYS("%s: (ERROR) NUM_APR_SLOTS_MUX is 0.",
                           fnName, 0,0,0,0,0);
        return ZL303XX_PARAMETER_INVALID;
    }
    if (NUM_APR_SLOTS_MUX > ZL303XX_APR_MAX_NUM_MASTERS)
    {
        ZL303XX_TRACE_ALWAYS("%s: (ERROR) NUM_APR_SLOTS_MUX cannot exceed %d.",
                           fnName, ZL303XX_APR_MAX_NUM_MASTERS, 0,0,0,0);
        return ZL303XX_PARAMETER_INVALID;
    }

    status = ZL303XX_CHECK_POINTER(eventData);
    if (status == ZL303XX_OK)
    {
        /* Find the streamHandle to remove */
        for (remIndx = 0; remIndx < maxPtpStreams; remIndx++)
        {
            /* Remove entry */
            if (muxPtpStreamsToAprSlotsS[remIndx].streamHandle == eventData->streamHandle)
            {
                aprServerSlot = (remIndx / STREAMS_PER_SLOT_MUX);
                muxPtpStreamsToAprSlotsS[remIndx].aprSlotCurrentActiveRef = ZL303XX_FALSE;
                muxPtpStreamsToAprSlotsS[remIndx].aprServerSlot = -1;
                muxPtpStreamsToAprSlotsS[remIndx].streamHandle = (zl303xx_PtpStreamHandleT)-1;
                ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3, "%s: Will DELETE streamHandle = %d at ptpIndex = %d",
                            fnName, eventData->streamHandle, remIndx, 0,0,0);
                break;
            }
        }

        if (remIndx == maxPtpStreams)
        {
            status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
            ZL303XX_TRACE_ALWAYS("%s: Entry for streamHandle=%d DOES NOT EXIST; status=%d",
                               fnName, eventData->streamHandle, status, 0,0,0);
        }
        else
        {
            Sint16T firstOtherOnSameMux = -1;
            Sint16T activeAtIndex = -1;

            /* Identify others (and whether active) on the same mux */
            for (indx = 0; indx < maxPtpStreams; indx++)
            {
                /* Others on the same mux? */
                if (muxPtpStreamsToAprSlotsS[indx].aprServerSlot == aprServerSlot)
                {
                    if (firstOtherOnSameMux == -1)
                        firstOtherOnSameMux = indx;

                    /* Any active? */
                    if (muxPtpStreamsToAprSlotsS[indx].aprSlotCurrentActiveRef)
                    {
                        activeAtIndex = indx;
                        ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3, "%s: Found ACTIVE MUX on same aprServerSlot = %d at activeAtIndex = %d",
                                    fnName, aprServerSlot, activeAtIndex, 0,0,0);
                    }
                }
            }

            /* None active */
            if (activeAtIndex == -1)
            {
                /* Select another (firstOtherOnSameMux) as active */
                if (firstOtherOnSameMux >= 0)
                {
                    examplePtpStreamExtDataS *pExtData = eventData->streamExtData;

                    muxPtpStreamsToAprSlotsS[firstOtherOnSameMux].aprSlotCurrentActiveRef = ZL303XX_TRUE;

                    /* Could warm start APR here if required */
                    if ((status = zl303xx_AprSetActiveRef(pExtData->cguId, muxPtpStreamsToAprSlotsS[firstOtherOnSameMux].aprServerSlot)) != ZL303XX_OK)
                    {
                        ZL303XX_TRACE_ALWAYS("%s: Call to zl303xx_AprSetActiveRef() for aprServerSlot=%d on device=%p failed=%d",
                                           fnName, muxPtpStreamsToAprSlotsS[firstOtherOnSameMux].aprServerSlot, pExtData->cguId, status, 0,0);
                    }
                    else
                    {
                        ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3, "%s: REPLACED ACTIVE SLOT = %d with NEW streamHandle = %d for device = %p",
                                    fnName, muxPtpStreamsToAprSlotsS[firstOtherOnSameMux].aprServerSlot,
                                    muxPtpStreamsToAprSlotsS[firstOtherOnSameMux].streamHandle, pExtData->cguId, 0,0);
                    }
                }
                else    /* No others so remove slot from APR */
                {
                    examplePtpStreamExtDataS *pExtData = eventData->streamExtData;
                    exampleAprStreamCreateS aprStream;

                    aprStream.cguId = pExtData->cguId;
                    aprStream.server.serverId = aprServerSlot;

                    if ((status = exampleAprStreamRemove(&aprStream)) != ZL303XX_OK)
                    {
                        ZL303XX_TRACE_ALWAYS("%s: Call to exampleAprStreamRemove() failed=%d\n", fnName, status, 0,0,0,0);
                    }
                    else
                    {
                        ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3, "%s: REMOVED APR SLOT = %d at remIndx = %d for device=%p",
                                    fnName, aprServerSlot, remIndx, aprStream.cguId, 0,0);
                    }
                }
            }
        }
    }

    return status;
}


/* zl303xx_NewBestPtpStreamToAprSlot */
/*
   This function is called by BMCA (or an alternative) for inform APR
   of the new best timing stream from all active streams.

    APR may reference switch (or may warm start) if on another slot.

  Parameters:
   [in]   eventData   Pointer to the BMCA update struct.

  Return Value:   zlStatusE -   ZL303XX_OK or
                        ZL303XX_INVALID_POINTER
                        return from zl303xx_AprSetActiveRef()

*******************************************************************************/
static zlStatusE zl303xx_NewBestPtpStreamToAprSlot(zl303xx_PtpEventClockBmcaUpdateS *eventData, zl303xx_PtpStreamHandleT bestStreamHandle);
static zlStatusE zl303xx_NewBestPtpStreamToAprSlot(zl303xx_PtpEventClockBmcaUpdateS *eventData, zl303xx_PtpStreamHandleT bestStreamHandle)
{
    const char *fnName = "zl303xx_NewBestPtpStreamToAprSlot";
    zlStatusE status = ZL303XX_OK;
    Uint16T indx;
    Uint16T maxPtpStreams = ZL303XX_PTP_NUM_STREAMS_MAX;
    Sint16T newBestStreamIndex = -1;
    static zl303xx_PtpStreamHandleT lastBestStreamHandle = -1;
    static Uint16T lastBestStreamIndex = 0;
    static Sint16T lastAprServerSlot = 0;

    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 2, "%s:", fnName, 0,0,0,0,0);

    if (eventData == NULL)
    {
       ZL303XX_TRACE_ALWAYS("%s: (ERROR) eventData is NULL.",
                          fnName, 0,0,0,0,0);
       return ZL303XX_INVALID_POINTER;
    }

    status = ZL303XX_CHECK_POINTER(eventData);
    if (status == ZL303XX_OK)
    {
        if (lastBestStreamHandle == bestStreamHandle)   /* Same, so nothing to do */
            return ZL303XX_OK;

        ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 2, "%s: NEW BEST STREAM HANDLE = %d", fnName, bestStreamHandle, 0,0,0,0);

        /* Find index for new BEST */
        for (indx = 0; indx < maxPtpStreams; indx++)
        {
            if (muxPtpStreamsToAprSlotsS[indx].streamHandle == bestStreamHandle)
            {
                newBestStreamIndex = indx;
                break;
            }
        }

        if (newBestStreamIndex == -1)  /* Broken! */
        {
            status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
            ZL303XX_TRACE_ALWAYS("%s: newBestStreamIndex for bestStreamHandle=%d NOT FOUND; status=%d", fnName, bestStreamHandle, status, 0,0,0);
        }
        else
        {
            void *zl303xx_ParamsP = eventData->clockExtData;

            /* Replace lastBest with BEST */
            muxPtpStreamsToAprSlotsS[lastBestStreamIndex].aprSlotCurrentActiveRef = ZL303XX_FALSE;
            muxPtpStreamsToAprSlotsS[newBestStreamIndex].aprSlotCurrentActiveRef = ZL303XX_TRUE;

            /* Perform APR reference switch to BEST if on a different slot */
            if (muxPtpStreamsToAprSlotsS[newBestStreamIndex].aprServerSlot != lastAprServerSlot)
            {
                /* Could warm start APR here if required */
                if ((status = zl303xx_AprSetActiveRef(zl303xx_ParamsP, muxPtpStreamsToAprSlotsS[newBestStreamIndex].aprServerSlot)) != ZL303XX_OK)
                {
                    ZL303XX_TRACE_ALWAYS("%s: Call to zl303xx_AprSetActiveRef() for aprServerSlot = %d failed =%d",
                                       fnName, muxPtpStreamsToAprSlotsS[newBestStreamIndex].aprServerSlot, status, 0,0,0);
                }
                else
                {
                    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3, "%s: CHANGED ActiveRef to bestStreamHandle = %d for aprServerSlot = %d at ptpIndex = %d",
                                fnName, bestStreamHandle, muxPtpStreamsToAprSlotsS[newBestStreamIndex].aprServerSlot, newBestStreamIndex, 0,0);
                }
            }
        }

        /* Store the current BEST */
        lastBestStreamHandle = bestStreamHandle;
        lastBestStreamIndex  = newBestStreamIndex;
        lastAprServerSlot  = muxPtpStreamsToAprSlotsS[newBestStreamIndex].aprServerSlot;
    }

    return status;
}


zlStatusE zl303xx_DisplayPtpStreamToAprSlots(void);
zlStatusE zl303xx_DisplayPtpStreamToAprSlots(void)
{
    const char *fnName = "zl303xx_DisplayPtpStreamToAprSlots";
    Sint16T maxPtpStreams = ZL303XX_PTP_NUM_STREAMS_MAX;
    Sint16T indx;

    printf("%s:\n\n", fnName);
    printf("     requestedHandle aprSlotHandle \n");
    printf("     _______________ _____________ \n");

    for (indx = 0; indx < maxPtpStreams; indx++)
    {
        if (Zl303xx_PtpStreamArray[indx])
            printf("%3d\t   %d \t\t %d \n",
                   indx, Zl303xx_PtpStreamArray[indx]->clockStreamHandle, Zl303xx_PtpStreamArray[indx]->aprSlotHandle);
    }

    printf("\n\n");

    return ZL303XX_OK;
}

#endif  /* MUX_PTP_STREAMS_TO_APR_SLOTS */
#endif  /* APR_INCLUDED */



/* examplePtpEventStreamCreate */
/**
   This function creates the transport structures necessary to send/receive
   PTP packets when a PTP task automatically creates a stream.

  Parameters:
   [in]   eventData   Data about the newly created stream.

  Return Value:   N/A

*******************************************************************************/
static void examplePtpEventStreamCreate(zl303xx_PtpEventStreamCreateS *eventData)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 3, "examplePtpEventStreamCreate:", 0,0,0,0,0,0);

   if (status == ZL303XX_OK)
   {
      if (eventData == NULL)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "   (ERROR) eventData is NULL.", 0,0,0,0,0,0);
         return;
      }
   }

#ifdef SYNC_FORCE_MIN_INTVL
   /* Force the interval between Sync message transmissions to never be less than
      the configured logSyncInterval value. This behavior is NON-STANDARD. */
   if (status == ZL303XX_OK)
   {
      eventData->streamConfig->createFlags |= ZL303XX_PTP_SYNC_FORCE_MIN_INTVL;
   }
#endif

#ifdef APR_INCLUDED
   {
      exampleAprStreamCreateS aprStream;
      zl303xx_PtpPortCreateS portConfig;

      if (status == ZL303XX_OK)
      {
         if (eventData->streamConfig->mode != ZL303XX_PTP_STREAM_MASTER_ONLY)
         {
            status = zl303xx_PtpPortConfigGet(eventData->portHandle, &portConfig);

            if (status == ZL303XX_OK)
            {
               status = exampleAprStreamCreateStructInit(&aprStream);
            }

            if (status == ZL303XX_OK)
            {
               aprStream.cguId = eventData->clockExtData;

               /* Override defaults based on event data or existing PTP config */
               aprStream.server.serverId = (Uint16T)eventData->streamHandle;
               aprStream.server.fwdPacketRateType = log2IntvlToPps(eventData->streamConfig->logSyncInterval);
               aprStream.server.revPacketRateType = log2IntvlToPps(eventData->streamConfig->logMinDelayReqInterval);
               /* For uni-directional PTP (delayMechanism disabled), the reverse path
                * cannot be used because Delay_Req/_Resp messages are not sent. For
                * bi-directional PTP, use the configured path direction. */
               aprStream.server.bUseRevPath = portConfig.delayMechanism != ZL303XX_DELAY_MECHANISM_DISABLED &&
                                              zl303xx_AprGetUseReversePath();

#if defined MUX_PTP_STREAMS_TO_APR_SLOTS
              if (status == ZL303XX_OK && (status = zl303xx_MapPtpStreamToAprSlot(&aprStream, -1)) != ZL303XX_OK)
              {
                  ZL303XX_TRACE_ALWAYS("examplePtpEventStreamCreate: Call to zl303xx_MuxPtpStreamToAprSlots() failed=%d\n", status, 0,0,0,0,0);
              }
#else
               status = exampleAprStreamCreate(&aprStream);
#endif
            }
         }
      }
   }
#endif  /* APR_INCLUDED */


#if defined EXAMPLE_PTP_SOCKET_TEST && defined OHLY_TIMESTAMPS
    /* For Ohly Timestamping, create unique per-stream socket for unicast mode egress timestamps (see examplePtpTx).
       Note: Multicast mode stream does not require unique socket
     */
    if (status == ZL303XX_OK && eventData->streamConfig->unicast == ZL303XX_TRUE)
    {
        zl303xx_PtpPortCreateS portConfig;
        examplePtpStreamExtDataS *pExtData;
        zl303xx_PortAddress localAddr;

        status = zl303xx_PtpStreamExtDataGet(eventData->streamHandle, (void**)&pExtData);

        if (status == ZL303XX_OK && pExtData == NULL)
        {
            /* Allocate memory for the external stream data if not provided already. */
            status = examplePtpStreamExtDataAlloc(&pExtData);

            /* Associate external data with stream */
            status = zl303xx_PtpStreamExtDataSet(eventData->streamHandle, pExtData);
        }

        /* Get localAddr from PTP port */
        if (status == ZL303XX_OK)
        {
            if ((status = zl303xx_PtpPortConfigGet(eventData->portHandle, &portConfig)) != ZL303XX_OK)
            {
               ZL303XX_TRACE_ALWAYS("examplePtpEventStreamCreate: zl303xx_PtpPortConfigGet() failed=%lu", status, 0,0,0,0,0);
            }
            else
            {
                memcpy(&localAddr, &portConfig.localAddr, sizeof(localAddr));
            }
        }

        /* Open unicast sockets for TX/RX. Memory for the socket structure is
        * allocated inside this function. */
        if (status == ZL303XX_OK)
        {
            status = examplePtpSocketUcastOpen(&localAddr, &pExtData->skt);
            if (status != ZL303XX_OK)
            {
              ZL303XX_TRACE_ALWAYS("examplePtpEventStreamCreate: examplePtpSocketUcastOpen() failed=%lu streamHandle=%d", status, eventData->streamHandle,0,0,0,0);
            }
            else
            {
                /* Associate socket with stream and port */
                if ((status = examplePtpSocketStreamHandleSet(pExtData->skt, eventData->streamHandle)) != ZL303XX_OK)
                {
                    ZL303XX_TRACE_ALWAYS("examplePtpEventStreamCreate: examplePtpSocketStreamHandleSet() failed=%lu streamHandle=%d", status, eventData->streamHandle,0,0,0,0);
                }

                if ((status = examplePtpSocketPortHandleSet(pExtData->skt, eventData->portHandle)) != ZL303XX_OK)
                {
                    ZL303XX_TRACE_ALWAYS("examplePtpEventStreamCreate: examplePtpSocketPortHandleSet() failed=%lu streamHandle=%d", status, eventData->streamHandle,0,0,0,0);
                }
            }
        }

        if (status == ZL303XX_OK)
        {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "examplePtpEventStreamCreate: created stream %d Tx socket, status %d", eventData->streamHandle, status,0,0,0,0);
        }
        else
        {
           /* Error creating socket. Maybe ran out of socket table space, see EXAMPLE_PTP_SOCKETS_MAX. */
           ZL303XX_TRACE_ALWAYS("examplePtpEventStreamCreate: ERROR creating port %d stream %d Tx socket, status %d", eventData->portHandle, eventData->streamHandle, status, 0,0,0);
        }
    }
#else
    if (status == ZL303XX_OK)
    {
        examplePtpStreamExtDataS *pExtData;

        status = zl303xx_PtpStreamExtDataGet(eventData->streamHandle, (void**)&pExtData);

        if (status == ZL303XX_OK && pExtData == NULL)
        {
            /* Allocate memory for the external stream data if not provided already. */
            status = examplePtpStreamExtDataAlloc(&pExtData);

        }

        /* Associate external data with stream */
        if (status == ZL303XX_OK && (status = zl303xx_PtpStreamExtDataSet(eventData->streamHandle, pExtData)) != ZL303XX_OK)
            ZL303XX_TRACE_ALWAYS("examplePtpEventStreamCreate: ERROR calling zl303xx_PtpStreamExtDataSet); status=%d", status, 0,0,0,0,0);

    }
#endif

#if defined ZL303XX_REFERENCE_SELECTION
   /* Send an update for this PTP Stream to the Server Selection Handler. */
   {
      /***************************************************************************/
      /* Translate the PTP Event message into a type for transmission over the
       * distributed backplane.                                                  */
      /***************************************************************************/
      exampleRefSelectBpMsgS txMsg;

      /* UPDATE message from the Application. */
      txMsg.source = REF_SELECT_SRC_MOD_PROT_STREAM;
      txMsg.action = REF_SELECT_ACTION_ADD;

      /* Translate the message data. */
      /***************************************************************************/
      /* Note:
       * - this example assumes a 1-to-1 correlation between the Protocol
       *   'streamHandle' and the Time-Sync 'serverId'. If this is not the case,
       *   a mapping / look-up should be done in place of the following ID
       *   assignment.                                                           */
      /***************************************************************************/
      txMsg.data.streamAdd.streamHandle = eventData->streamHandle;
      txMsg.data.streamAdd.clockHandle = eventData->clockHandle;
      txMsg.data.streamAdd.serverId = eventData->streamHandle;

   #if defined APR_INCLUDED
      /***************************************************************************/
      /* Ordinarily, the data would be sent over the backplane at this point and
       * the next command executed within the context of the Timing Card CPU.
       * Here, both run on a single CPU.                                         */
      /***************************************************************************/

      /* Send the Data to the Server Database. */
      (void)exampleRefSelectSendMsgToBackplane(&txMsg);
   #else
   /* #warning MSCC: Translation of PTP message to a Backplane message is required. */
      ZL303XX_TRACE_ALWAYS("Line Card Event ready for BP transmission (%d)",
            ZL303XX_PTP_EVENT_STREAM_CREATE, 0,0,0,0,0);
   #endif
   }
#endif
   return;
}

/* examplePtpEventStreamDelete */
/**
   This function deletes the associated transport and clock recovery structures
   when a PTP task automatically deletes a stream.

  Parameters:
   [in]   eventData   Data about the stream being deleted.

  Return Value:   N/A

*******************************************************************************/
static void examplePtpEventStreamDelete(zl303xx_PtpEventStreamDeleteS *eventData)
{
   zlStatusE status = ZL303XX_OK;

   if (eventData == NULL)
   {
      status = ZL303XX_INVALID_POINTER;
      ZL303XX_TRACE_ALWAYS(
            "examplePtpEventStreamDelete: eventData == NULL; return status = %d",
            status, 0,0,0,0,0);
      return;
   }

#ifdef EXAMPLE_PTP_SOCKET_TEST

   /* Delete the sockets, if they exist. */
   if (status == ZL303XX_OK)
   {
      examplePtpStreamExtDataS *pStreamExtData = eventData->streamExtData;
      if (pStreamExtData != NULL)
      {
         status = examplePtpSocketClose(&pStreamExtData->skt);
      }

   }
#else
   /* INSERT CODE here to delete/modify transport layer structures as necessary. */
#endif


#if defined ZL303XX_REFERENCE_SELECTION
   /* Send an update for this PTP Stream to the Server Selection Handler. */
   {
      /***************************************************************************/
      /* Translate the PTP Event message into a type for transmission over the
       * distributed backplane.                                                  */
      /***************************************************************************/
      exampleRefSelectBpMsgS txMsg;

      /* UPDATE message from the Application. */
      txMsg.source = REF_SELECT_SRC_MOD_PROT_STREAM;
      txMsg.action = REF_SELECT_ACTION_DELETE;

      /* Translate the message data. */
      txMsg.data.streamDelete.streamHandle = eventData->streamHandle;
      txMsg.data.streamDelete.clockHandle = eventData->clockHandle;

   #if defined APR_INCLUDED
      /***************************************************************************/
      /* Ordinarily, the data would be sent over the backplane at this point and
       * the next command executed within the context of the Timing Card CPU.
       * Here, both run on a single CPU.                                         */
      /***************************************************************************/

      /* Send the Data to the Server Database. */
      (void)exampleRefSelectSendMsgToBackplane(&txMsg);
   #else
   /* #warning MSCC: Translation of PTP message to a Backplane message is required. */
      ZL303XX_TRACE_ALWAYS("Line Card Event ready for BP transmission (%d)",
            ZL303XX_PTP_EVENT_STREAM_DELETE, 0,0,0,0,0);
   #endif
   }
#endif

#if defined MUX_PTP_STREAMS_TO_APR_SLOTS
   if ((status = zl303xx_DemuxPtpStreamFromAprSlots(eventData)) != ZL303XX_OK)
   {
       ZL303XX_TRACE_ALWAYS("examplePtpEventStreamDelete: Call to zl303xx_DemuxPtpStreamFromAprSlots() failed=%d\n", status, 0,0,0,0,0);
   }
#endif

   ZL303XX_TRACE_ALWAYS("examplePtpEventStreamDelete: return status = %d", status, 0,0,0,0,0);
   return;
}

/* examplePtpEventStreamStateChange */
/**
   A function to handle a state change by the Best Master Clock Algorithm.
   This can be used to switch the active reference of a clock recovery
   mechanism.

  Parameters:
   [in]   eventData   Data about the stream operating state change.

  Return Value:   N/A

*******************************************************************************/
static void examplePtpEventStreamStateChange(zl303xx_PtpEventStreamStateChangeS *eventData)
{
   const char *stateStr = zl303xx_PtpStreamStateToStr(eventData->operState);

	/* ログが大量になるため、変化時のみ残す */
	const char *stateStr_pre;
	if(f_ptpw_pre_operState != eventData->operState){
		if(f_ptpw_pre_operState >= 0){
			stateStr_pre = zl303xx_PtpStreamStateToStr(f_ptpw_pre_operState);
		}else{
			stateStr_pre = "INITIAL STATE";
		}
		/* If we've made it here, the event is valid  */
		ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
		   "Stream %d to State %d (%s) pre State %d (%s)",
		   eventData->streamHandle, eventData->operState, stateStr, f_ptpw_pre_operState,stateStr_pre,0);
		   f_ptpw_pre_operState = eventData->operState;
	}

   switch (eventData->operState)
   {
      /* Stale modes. */
      case ZL303XX_PTP_STREAM_OPER_STATE_UNKNOWN :
      case ZL303XX_PTP_STREAM_OPER_STATE_DISABLED :

      /* Master modes */
      case ZL303XX_PTP_STREAM_OPER_STATE_GRANDMASTER:    /* Valid states but do nothing (for now).  */
      case ZL303XX_PTP_STREAM_OPER_STATE_MASTER:
      case ZL303XX_PTP_STREAM_OPER_STATE_REPEATER:
      case ZL303XX_PTP_STREAM_OPER_STATE_PASSIVE_MASTER:

      /* Slave Modes. */
      case ZL303XX_PTP_STREAM_OPER_STATE_LISTENING:
      case ZL303XX_PTP_STREAM_OPER_STATE_MONITORING:
      case ZL303XX_PTP_STREAM_OPER_STATE_SLAVE:
         break;

      default :
      {
         /* Can never reach this since we return (above) for unknown states. */
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "(ERROR) Bad stream operating state (%d).",
               eventData->operState, 0,0,0,0,0);

         break;
      }
   }
}

/* examplePtpEventStreamKeepAlive */
/**
   A function to handle the STream Keep-Alive Event.
   This event merely notifies the system that an existing stream is still active.

  Parameters:
   [in]   eventData   Data about the stream BMCA information.

  Return Value:   N/A

*******************************************************************************/
static void examplePtpEventStreamKeepAlive(zl303xx_PtpEventStreamKeepAliveS *eventData)
{
   if (eventData == NULL)
   {
      ZL303XX_TRACE_ALWAYS("examplePtpEventStreamKeepAlive: (ERROR) eventData is NULL.",
            0,0,0,0,0,0);

      return;
   }

#if defined ZL303XX_REFERENCE_SELECTION
   /* Send an update for this PTP Stream to the Server Selection Handler. */
   {
      /***************************************************************************/
      /* Translate the PTP Event message into a type for transmission over the
       * distributed backplane.                                                  */
      /***************************************************************************/
      exampleRefSelectBpMsgS txMsg;

      /* UPDATE message from the Application. */
      txMsg.source = REF_SELECT_SRC_MOD_PROT_STREAM;
      txMsg.action = REF_SELECT_ACTION_UPDATE;

      /* Translate the PTP Stream data to a compatible Backplane Message. */
      txMsg.data.streamUpdate.streamHandle = eventData->bmcaEntry.streamHandle;
      txMsg.data.streamUpdate.clockHandle = eventData->clockHandle;
      txMsg.data.streamUpdate.streamDataValid = ZL303XX_TRUE;
      txMsg.data.streamUpdate.profile = eventData->clockProfile;
      txMsg.data.streamUpdate.bmcaData = eventData->bmcaEntry.bmcaData;
      txMsg.data.streamUpdate.telecomData = eventData->bmcaEntry.telecomData;

   #if defined APR_INCLUDED
      /***************************************************************************/
      /* Ordinarily, the data would be sent over the backplane at this point and
       * the next command executed within the context of the Timing Card CPU.
       * Here, both run on a single CPU.                                         */
      /***************************************************************************/

      /* Send the Data to the Server Database. */
      (void)exampleRefSelectSendMsgToBackplane(&txMsg);
   #else
   /* #warning MSCC: Translation of PTP message to a Backplane message is required. */
      ZL303XX_TRACE_ALWAYS("Line Card Event ready for BP transmission (%d)",
            ZL303XX_PTP_EVENT_STREAM_KEEP_ALIVE, 0,0,0,0,0);
   #endif
   }

   return;
#endif

   return;
}

/* examplePtpEventMultiplePeerResp */
/**
   A function to handle Multiple Peer Response Messages received from a single
   request.

  Parameters:
   [in]   eventData   Data about the stream Peer Response Message.

  Return Value:   N/A

*******************************************************************************/
static void examplePtpEventMultiplePeerResp(zl303xx_PtpEventMultiplePeerRespS *eventData)
{
   static char txPortIdStr[64];
   static char rxPortIdStr[64];
   static char reqPortIdStr[64];

   ZL303XX_TRACE_ALWAYS(
         "examplePtpEventMultiplePeerResp: Multiple Peer Response Notification.",
         0,0,0,0,0,0);

   if (eventData == NULL)
   {
      ZL303XX_TRACE_ALWAYS(
            "       >>>: (ERROR) eventData is NULL.",
            0,0,0,0,0,0);
      return;
   }

   ZL303XX_TRACE_ALWAYS("   streamHandle: %u", eventData->streamHandle,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("     sequenceId: %u", eventData->sequenceId,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("           rxTs: %010u:%010u",
         eventData->rxTs.secondsField.lo, eventData->rxTs.nanosecondsField, 0,0,0,0);

   (void)zl303xx_PtpV2PortIdentityToString(&eventData->txPortIdentity, txPortIdStr);
   (void)zl303xx_PtpV2PortIdentityToString(&eventData->rxPortIdentity, rxPortIdStr);
   (void)zl303xx_PtpV2PortIdentityToString(&eventData->reqPortIdentity, reqPortIdStr);

   ZL303XX_TRACE_ALWAYS("      TX PortId: %s", txPortIdStr, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("     REQ PortId: %s", reqPortIdStr, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("      RX PortId: %s", rxPortIdStr, 0,0,0,0,0);

   return;
}

/* examplePtpEventClockBmcaUpdate */
/**
   A function to handle a change in the BMCA table for a clock.
   This update is sent when the information changes.

  Parameters:
   [in]   eventData   Data about the clock BMCA change.

  Return Value:   N/A

*******************************************************************************/
static void examplePtpEventClockBmcaUpdate(zl303xx_PtpEventClockBmcaUpdateS *eventData)
{
   const char *fnName = "examplePtpEventClockBmcaUpdate";
#if defined APR_INCLUDED
   void *zl303xx_ParamsP = NULL;
#endif

   /* Set the selected stream value to be the Clock Default. */
   zl303xx_PtpStreamHandleT bestPktStream = (zl303xx_PtpStreamHandleT)(-1);
   zl303xx_PtpBmcCompareDataS bestPktServerData;
   zl303xx_BooleanE bestIsVirtualPort = ZL303XX_FALSE;
   zl303xx_PtpStreamHandleT standbyPktStream = (zl303xx_PtpStreamHandleT)(-1); /* Second best */
   zl303xx_PtpBmcCompareDataS standbyPktServerData;
   zl303xx_BooleanE standbyIsVirtualPort = ZL303XX_FALSE;

   /* Counter to loop through the server table. */
   Uint16T entryId;

   /* For retrieving the Clock's configuration since some of its data is required. */
   zl303xx_PtpClockCreateS clockConfig;

   /* Table of additional servers found. */
   zl303xx_PtpEventClockBmcaEntryS *bmcaEntries = NULL;
   zl303xx_PtpBmcCompareDataS clockDefaultData;
   zl303xx_PtpTelecomCompareDataS telecomData;
   zl303xx_PtpG8275ProfileCompareDataS tpPhaseData;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3, "%s:", fnName, 0,0,0,0,0);

   if (eventData == NULL)
   {
      ZL303XX_TRACE_ALWAYS("%s: (ERROR) eventData is NULL.",
            fnName, 0,0,0,0,0);
      return;
   }

#if defined APR_INCLUDED
   zl303xx_ParamsP = eventData->clockExtData;
#endif

   /* Get the Clock's configuration since some of its data is required. */
   if (zl303xx_PtpClockConfigGet(eventData->clockHandle, &clockConfig) != ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS("%s: (ERROR): Unable to retrieve Data for Clock (%d)",
            fnName, eventData->clockHandle, 0,0,0,0);
      return;
   }

   /* Unpack the received data, then loop through all the data for all of the
    * qualified servers discovered and determine the source of the best one. */

   /* Assume the best server is the clock that sent the data. */
   clockDefaultData = eventData->clockDefault;
   bestPktServerData = clockDefaultData;
   standbyPktServerData = clockDefaultData;

   /* Table of additional servers found. */
   bmcaEntries = eventData->bmcaEntry;

   /* Create a Telecom-Frequency data set from the Clock config if the G.8265 Profile is used. */
   if (eventData->profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1)
   {
      /* Assume the hardware is valid as a last resort. */
      telecomData.ptsf = ZL303XX_FALSE;

      /* Other configuration values. */
      telecomData.priority = 0;
      telecomData.clockClass = clockDefaultData.grandmasterClockQuality.clockClass;
      telecomData.stepsRemoved = 0;
      memcpy(telecomData.grandmasterIdentity,
             clockDefaultData.grandmasterIdentity,
             sizeof(telecomData.grandmasterIdentity));
   }

   /* Create a Telecom-Phase data set from the Clock config if the G.8275 Profile is used. */
	else if ((eventData->profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_1v1) ||
             (eventData->profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_1v2) ||
             (eventData->profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_2))
	{
  		/* Port-Stream value */
  		if (clockDefaultData.grandmasterClockQuality.clockClass <= 127)
  		{
  			tpPhaseData.masterOnly = ZL303XX_TRUE;
  			tpPhaseData.notSlave = ZL303XX_TRUE;
		}
		else
		{
  			tpPhaseData.masterOnly = ZL303XX_FALSE;
  			tpPhaseData.notSlave = ZL303XX_FALSE;
		}

   	tpPhaseData.localPriority = clockDefaultData.grandmasterPriority1;
   	tpPhaseData.rxPortIdentity = clockDefaultData.receivingPortIdentity;

   	/* Grandmaster values. */
   	tpPhaseData.grandmasterClockQuality = clockDefaultData.grandmasterClockQuality;
   	tpPhaseData.grandmasterPriority2 = clockDefaultData.grandmasterPriority2;
    	memcpy(tpPhaseData.grandmasterIdentity,
      		 clockDefaultData.grandmasterIdentity,
       		 sizeof(tpPhaseData.grandmasterIdentity));

 		/* Other fields. */
    	tpPhaseData.stepsRemoved = 0;
 		tpPhaseData.txPortIdentity = clockDefaultData.foreignPortIdentity;
	}

#if defined ZL303XX_REFERENCE_SELECTION
   /* Send an update for this PTP Clock and all STreams to the Server Selection Handler. */
   {
      /***************************************************************************/
      /* Translate the PTP Event message into a type for transmission over the
       * distributed backplane.                                                  */
      /***************************************************************************/
      exampleRefSelectBpMsgS txMsg;

      /* Clock UPDATE message from the Application. */
      {
         txMsg.source = REF_SELECT_SRC_MOD_PROT_CLOCK;
         txMsg.action = REF_SELECT_ACTION_UPDATE;

         /* Translate the PTP Clock data to a compatible Backplane Message. */
         txMsg.data.clockUpdate.clockHandle = eventData->clockHandle;
         txMsg.data.clockUpdate.profile = eventData->profile;
         txMsg.data.clockUpdate.bmcaData = clockDefaultData;
         txMsg.data.clockUpdate.telecomData = telecomData;
         txMsg.data.clockUpdate.tpPhaseData = tpPhaseData;

   #if defined APR_INCLUDED
      /***************************************************************************/
      /* Ordinarily, the data would be sent over the backplane at this point and
       * the next command executed within the context of the Timing Card CPU.
       * Here, both run on a single CPU.                                         */
      /***************************************************************************/

      /* Send the Data to the Server Database. */
      (void)exampleRefSelectSendMsgToBackplane(&txMsg);
   #else
   /* #warning MSCC: Translation of PTP message to a Backplane message is required. */
      ZL303XX_TRACE_ALWAYS("Line Card Event ready for BP transmission (%d)",
            ZL303XX_PTP_EVENT_CLOCK_BMCA_UPDATE, 0,0,0,0,0);
   #endif
      }

      /* Stream UPDATE messages from the Application. */
      {
         txMsg.source = REF_SELECT_SRC_MOD_PROT_STREAM;
         txMsg.action = REF_SELECT_ACTION_UPDATE;

         /* Loop through the table entries and send updates to the Server data base. */
         for (entryId = 0; entryId < eventData->numBmcaEntries; entryId++, bmcaEntries++)
         {
            /* Translate the PTP Stream data to a compatible Backplane Message. */
            txMsg.data.streamUpdate.streamHandle = bmcaEntries->streamHandle;
            txMsg.data.streamUpdate.clockHandle = eventData->clockHandle;
            txMsg.data.streamUpdate.streamDataValid = ZL303XX_TRUE;
            txMsg.data.streamUpdate.profile = eventData->profile;
            txMsg.data.streamUpdate.bmcaData = bmcaEntries->bmcaData;
            txMsg.data.streamUpdate.telecomData = bmcaEntries->telecomData;
            txMsg.data.streamUpdate.tpPhaseData = bmcaEntries->tpPhaseData;

   #if defined APR_INCLUDED
         /***************************************************************************/
         /* Ordinarily, the data would be sent over the backplane at this point and
          * the next command executed within the context of the Timing Card CPU.
          * Here, both run on a single CPU.                                         */
         /***************************************************************************/

         /* Send the Data to the Server Database. */
         (void)exampleRefSelectSendMsgToBackplane(&txMsg);
   #else
   /* #warning MSCC: Translation of PTP message to a Backplane message is required. */
         ZL303XX_TRACE_ALWAYS("Line Card Event ready for BP transmission (%d)",
               ZL303XX_PTP_EVENT_CLOCK_BMCA_UPDATE, 0,0,0,0,0);
   #endif

         } /* For */
      }
   }

   /* When using the Server Database, exit once all messages are sent. */
   return;
#endif

   /* Loop through the table entries and do the appropriate comparison on the data. */
   for (entryId = 0; entryId < eventData->numBmcaEntries; entryId++, bmcaEntries++)
   {
      if ((eventData->profile == ZL303XX_PTP_PROFILE_DEFAULT_E2E) ||
          (eventData->profile == ZL303XX_PTP_PROFILE_DEFAULT_P2P) ||
          (eventData->profile == ZL303XX_PTP_PROFILE_POWER_C37_238_2011))
      {
         if (zl303xx_PtpV2DefaultProfileBmcaComparison(&bestPktServerData, &bmcaEntries->bmcaData) < 0)
         {
            /* This entry is better than the existing one so select it. */
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3,"%s: PTP server %d worse than %d (default profile)",
                fnName, (Uint16T)bestPktStream, (Uint16T)bmcaEntries->streamHandle,0,0,0);

            standbyPktStream = bestPktStream;           /* Save 2nd best */
            standbyPktServerData = bestPktServerData;

            bestPktStream = bmcaEntries->streamHandle;
            bestPktServerData = bmcaEntries->bmcaData;
         }
      }

      else if (eventData->profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1)
      {
         if (zl303xx_PtpTelecomProfileBmcaComparison(&telecomData,
                                                     &bmcaEntries->telecomData,
                                                     clockConfig.bmca.revertiveEn) < 0)
         {
            /* This entry is better than the existing one so select it. */
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3, "%s: PTP server %d worse than %d (G8265.1 profile)",
                fnName, (Uint16T)bestPktStream, (Uint16T)bmcaEntries->streamHandle,0,0,0);

            standbyPktStream = bestPktStream;           /* Save 2nd best */
            standbyPktServerData = bestPktServerData;

            bestPktStream = bmcaEntries->streamHandle;
            bestPktServerData = bmcaEntries->bmcaData;
            telecomData = bmcaEntries->telecomData;
         }
      }

      else if ((eventData->profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_1v1) ||
               (eventData->profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_1v2) ||
               (eventData->profile == ZL303XX_PTP_PROFILE_TELECOM_G8275_2))
      {
         if (zl303xx_PtpBmcaG8275ProfileComparison(&tpPhaseData,
                                                   &bmcaEntries->tpPhaseData,
                                                   NULL) < 0)
         {
            /* This entry is better than the existing one so select it. */
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3, "%s: PTP server %d worse than %d (G8275.1 profile)",
                fnName, (Uint16T)bestPktStream, (Uint16T)bmcaEntries->streamHandle,0,0,0);

            standbyPktStream = bestPktStream;           /* Save 2nd best */
            standbyPktServerData = bestPktServerData;

            bestPktStream = bmcaEntries->streamHandle;
            bestPktServerData = bmcaEntries->bmcaData;
            tpPhaseData = bmcaEntries->tpPhaseData;
         }
      }

      else
      {
         ZL303XX_TRACE_ALWAYS("%s: Unknown CLOCK PROFILE:: Custom Comparison Required.)",
               fnName, 0,0,0,0,0);

         return;
      }
   }

   if (bestPktStream == (Uint32T)(-1))
   {
       if (bestPktServerData.receivingPortIdentity.portNumber == 0)
       {
          /* Clock defaultDS is always portNumber 0 */
          ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3, "%s: Clock defaultDS selected as the best", fnName,0,0,0,0,0);
          bestIsVirtualPort = ZL303XX_FALSE;
       }
       else
       {
          /* Must be a virtual port */
          ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3, "%s: Virtual Port selected as the best", fnName,0,0,0,0,0);
          bestIsVirtualPort = ZL303XX_TRUE;
       }
   }

   if (standbyPktStream == (Uint32T)(-1))
   {
       if (standbyPktServerData.receivingPortIdentity.portNumber == 0)
       {
          /* Clock defaultDS is always portNumber 0 */
          standbyIsVirtualPort = ZL303XX_FALSE;
       }
       else
       {
          /* Must be a virtual port */
          standbyIsVirtualPort = ZL303XX_TRUE;
       }
   }
	 ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3, "Best is Virtual Port:%d , bestPktStream:%d, standbyPktStream:%d", bestIsVirtualPort,
		bestPktStream, standbyPktStream,0,0,0);
   (void) bestIsVirtualPort;    /* warning removal if not used */
   (void) standbyIsVirtualPort; /* warning removal if not used */

#if defined APR_INCLUDED

#if defined MUX_PTP_STREAMS_TO_APR_SLOTS
    if (bestPktStream != (Uint32T)(-1))
    {
        zl303xx_NewBestPtpStreamToAprSlot(eventData, (zl303xx_PtpStreamHandleT)bestPktStream/*, zl303xx_AprDeviceRefModeE refModeType*/);
    }
#else
   if (bestPktStream != (zl303xx_PtpStreamHandleT)(-1))
   {
      zl303xx_AprLockStatusE aprLockStatus;
      zl303xx_AprDeviceRefModeE devRefMode;

      /* Retrieve the Reference Mode. */
      (void)zl303xx_AprGetDeviceRefMode(zl303xx_ParamsP, &devRefMode);

      if (devRefMode == ZL303XX_PACKET_MODE)
      {
         /* In Packet Mode so select the best packet server. */
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 2, "%s: ZL303XX_PACKET_MODE selecting active server %d",
                     fnName, (Uint16T)bestPktStream, 0,0,0,0);

         (void)zl303xx_AprSetActiveRef(zl303xx_ParamsP, (Uint16T)bestPktStream);
      }

      else if (devRefMode == ZL303XX_HYBRID_MODE)
      {
         /* APR in hybrid mode, select best packet server for hybrid operation? */
        /* #warning MSCC: Do hybrid mode BMCA action */
        (void)zl303xx_AprSetActiveRef(zl303xx_ParamsP, (Uint16T)bestPktStream);
        ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 1, "%s: ZL303XX_HYBRID_MODE selected ... TODO best server %d",
            fnName, (Uint16T)bestPktStream, 0,0,0,0);
      }

      else if (devRefMode == ZL303XX_ELECTRIC_MODE)
      {
          /* In Electric reference mode, if the electric reference fails, change to packet reference */
          (void)zl303xx_AprGetDeviceLockStatus(zl303xx_ParamsP, &aprLockStatus);

          if (aprLockStatus == ZL303XX_LOCK_STATUS_REF_FAILED)
          {
             /* Force APR into packet mode with best packet server available */
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 2, "%s: ZL303XX_ELECTRIC_MODE REF_FAILED, picking packet server %d",
                fnName, (Uint16T)bestPktStream, 0,0,0,0);

             (void)zl303xx_AprSetActiveRef((void*)zl303xx_ParamsP, (Uint16T)bestPktStream);
          }
          else
          {
             /* There is a PTP server but if the electrical one is better then use it. */
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 2, "%s: ZL303XX_ELECTRIC_MODE, monitoring packet server %d",
                fnName, (Uint16T)bestPktStream, 0,0,0,0);

             bestPktStream = (zl303xx_PtpStreamHandleT)(-1);
             bestPktServerData = clockDefaultData;
          }
      }
   }

   else
   {
      /* At this point, there is no valid packet server. */
      /* Take any appropriate action such as switching to HOLDOVER, etc. */
      /* #warning MSCC: Do no packet server BMCA action */
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 1, "%s: no valid packet server available, TODO ...", fnName,0,0,0,0,0);
   }
#endif /* !MUX_PTP_STREAMS_TO_APR_SLOTS */
#else /* !APR_INCLUDED */
   /* INSERT CODE here to switch the active reference on the clock recovery
    * mechanism to this PTP stream. */
    /* #warning MSCC: Configure servo for BMCA selection */
    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 1, "%s: TODO servo select server %d", fnName, bestPktStream,0,0,0,0);
#endif

   /* Check how the Clock egress clockClass should be updated. */
   if (clockConfig.egressClassUpdateTrigger == ZL303XX_EGRESS_QL_UPDATE_ON_LOCK)
   {
#if defined APR_INCLUDED
      zl303xx_AprLockStatusE aprLockStatus;
      zl303xx_AprLockStatusE targetAprLockStatus;
      Uint16T serverId;
      zl303xx_AprConfigure1HzS fwd;
      zl303xx_AprConfigure1HzS rev;

      /* Determine which lock state to wait for: If 1Hz is enabled, then wait
         for phase to lock. Otherwise, wait for frequency lock. */
      targetAprLockStatus = ZL303XX_LOCK_STATUS_LOCKED; /* frequency lock */
      if( zl303xx_AprGetCurrent1HzConfigData(zl303xx_ParamsP, &serverId, &fwd, &rev) == ZL303XX_OK )
      {
         if(( fwd.disabled == ZL303XX_FALSE ) && ( rev.disabled == ZL303XX_FALSE ))
         {
            targetAprLockStatus = ZL303XX_LOCK_STATUS_PHASE_LOCKED; /* phase lock */
         }
      }

      /* Only use the selected server's QL if the local hardware is equal to
         targetAprLockStatus as determined above. */
      (void)zl303xx_AprGetDeviceLockStatus(zl303xx_ParamsP, &aprLockStatus);
      if (aprLockStatus == targetAprLockStatus)
      {
         /* Use the parentDS clockClass and disable any over-ride value. */
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 1, "%s: APR locked, disabled egress clockClass override", fnName,0,0,0,0,0);

         (void)zl303xx_PtpClockEgressClockClassSet(eventData->clockHandle, 0);
      }
      else
      {
         /* Transmit the client's default clockClass until the hardware locks. */

         /* The normal case would be to send the default clockClass. However,
            in boundary clock, using the default clockClass causes the slave
            to lock to the previous node immediately since the previous node's
            clockClass is better then the slaves. So send clockClass = 255
            to make sure the slave does not lock until frequency or phase lock. */
         if (zl303xx_AprGetAlgTypeMode() == ZL303XX_BOUNDARY_CLK)
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 1, "%s: APR not locked, BC egress clockClass override 255", fnName,0,0,0,0,0);

            (void)zl303xx_PtpClockEgressClockClassSet(
                                 eventData->clockHandle,
                                 255);
         }
         else
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 1, "%s: APR not locked, egress clockClass override %u",
                                 fnName, eventData->clockDefault.grandmasterClockQuality.clockClass,0,0,0,0);

            (void)zl303xx_PtpClockEgressClockClassSet(
                                 eventData->clockHandle,
                                 eventData->clockDefault.grandmasterClockQuality.clockClass);
         }
      }
#else
   /* INSERT CODE here to check the CGU LOCK status and update the ParentDS
    * clockClass accordingly. */
#endif
   }

   /* Send the selected Server Data Set back to the clock for state updates. */
   (void)zl303xx_PtpClockApplyCompareData(eventData->clockHandle, &bestPktServerData);
}

/* examplePtpEventServoData */
/**
   Function to pass a TX/RX time stamp pair to a clock recovery (servo)
   algorithm.

  Parameters:
   [in]   eventData   Data about the time stamp pair.

  Return Value:   N/A

*******************************************************************************/
static void examplePtpEventServoData(zl303xx_PtpEventServoDataS *eventData)
{
#if defined APR_INCLUDED
   #if defined EXAMPLE_PTP_SW_TIMESTAMPS
      /* #warning EXAMPLE_PTP_SW_TIMESTAMPS still defined, bypassing servo */
      /* printf("examplePtpEventServoData: In EXAMPLE_PTP_SW_TIMESTAMPS: doing nothing with ingress TS\n"); */
      /* Eliminate warnings. */
      if (eventData) {;}
   #else
   {
      zlStatusE status = ZL303XX_OK;
      examplePtpStreamExtDataS *pStreamExtData = eventData->streamExtData;
      zl303xx_AprTimestampS aprTs;

	  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2," examplePtpEventServoData start\n", 0, 0, 0, 0, 0, 0);

      memset(&aprTs, 0, sizeof(aprTs));

      aprTs.serverId = (Uint16T)eventData->streamHandle;

      aprTs.rxTs.second    = eventData->rxTs.secondsField;
      aprTs.rxTs.subSecond = eventData->rxTs.nanosecondsField;
      aprTs.txTs.second    = eventData->txTs.secondsField;
      aprTs.txTs.subSecond = eventData->txTs.nanosecondsField;
      memcpy(&aprTs.corr, &eventData->correctionField, sizeof(aprTs.corr));

      aprTs.bPeerDelay = ZL303XX_FALSE;
      aprTs.bForwardPath = ZL303XX_TRUE;

      if (eventData->tsPairType == ZL303XX_PTP_TS_PAIR_DELAY)
      {
          aprTs.bForwardPath = ZL303XX_FALSE;
      }
      else
      {
          if (eventData->tsPairType == ZL303XX_PTP_TS_PAIR_SYNC_PEER)
          {
              aprTs.bPeerDelay = ZL303XX_TRUE;
              memcpy(&aprTs.peerMeanDelay, &eventData->peerMeanPathDelay, sizeof(aprTs.peerMeanDelay));
          }
      }

      /* Offset From Master */
      aprTs.offsetFromMasterValid = eventData->offsetFromMasterValid;
      aprTs.offsetFromMaster.hi = eventData->offsetFromMaster.secondsField.lo;
      aprTs.offsetFromMaster.lo = eventData->offsetFromMaster.nanosecondsField;

      if (status == ZL303XX_OK)
      {
          if (pStreamExtData != NULL && pStreamExtData->cguId != NULL)
          {
              if ((status = zl303xx_AprProcessTimestamp(pStreamExtData->cguId, &aprTs)) != ZL303XX_OK)
              {
                 ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                       "zl303xx_AprProcessTimestamp() failed with status=%lu.",
                        status, 0,0,0,0,0);
              }
              else{
				 /* 正常ルート 確認用 */
				 ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,"zl303xx_AprProcessTimestamp() success with status=%lu.",
                        status, 0,0,0,0,0);
              }
          }
          else
          {
             ZL303XX_TRACE_ALWAYS("examplePtpEventServoData: pStreamExtData=%p or pStreamExtData->cguId was NULL.",
                                pStreamExtData, 0,0,0,0,0);
             status =  ZL303XX_INVALID_POINTER;
          }
      }
   }
   #endif /* EXAMPLE_PTP_SW_TIMESTAMPS */
#else
   /* INSERT CODE here to register the time stamp pair with the clock recovery
    * mechanism. */
   /* #warning MSCC: INSERT CODE here to register the time stamp pair */

   /* Eliminate warnings. */
   if (eventData) {;}
#endif
}

/* examplePtpEventCounterAlarm */
/**
   Logs info to the console if SYNC, DELAY_RESP or ANNOUNCE message rates fall
   below 50% of the expected rate.

  Parameters:
   [in]   eventData   Data about the message counters.

  Return Value:   N/A

*******************************************************************************/
static void examplePtpEventCounterAlarm(zl303xx_PtpEventCounterAlarmS *eventData)
{
   /* Eliminates warning */
   const char *msgTypeStr = zl303xx_PtpMessageTypeToStr(eventData->messageType);

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                 "Stream %lu: [PTP ALARM] %s rxCount=%lu, expected=%lu, period=%u sec.",
                 eventData->streamHandle, msgTypeStr, eventData->rxCount,
                 eventData->rxCountExpected, eventData->periodSec, 0);
   ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 2,
                 "Stream %lu: [PTP ALARM] %s rxCount=%lu, expected=%lu, period=%u sec.",
                 eventData->streamHandle, msgTypeStr, eventData->rxCount,
                 eventData->rxCountExpected, eventData->periodSec, 0);
                 
   if (eventData->messageType == ZL303XX_MSG_ID_ANNOUNCE)
   {
		if(( eventData->rxCount == 0) && (keep_announce_clockclass != 0))
		{
			/* 停波処理を行う */
			ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
            	"Announce Checks failed. stop wave. because Announce timeout.(keep_announce_clockclass=%d)",keep_announce_clockclass,0,0,0,0,0);
        	f_com_SVCtl(E_DU_SVCTL_OPE_STP_WAV, D_DU_SVCTL_ON, D_DU_SVCTL_ON_WAVE_STOP_ALL_PTP);
        	
        	/* Announceの前回値を0クリア */
			keep_announce_clockclass = 0;

        	BPF_COM_CMD_SET_CUR_CLK_CLASS((UCHAR)keep_announce_clockclass);	/* FUJITSU added for DU command */

        }
	}
}

/* examplePtpEventSquelch */
/**
   Enables/disables all output clocks based on the SQUELCH event data.

  Parameters:
   [in]   eventData   Data about the SQUELCH event.

  Return Value:  N/A

*******************************************************************************/
static void examplePtpEventSquelch(zl303xx_PtpEventSquelchS *eventData)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE_ALWAYS("Clock %lu: SQUELCH event - Output clocks %s.",
      eventData->clockHandle, (eventData->enableOutput) ? "ENABLED" : "DISABLED",
      0,0,0,0);

   if (status == ZL303XX_OK)
   {
       /* Change all output clocks based on enableOutput. */
   }

   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE_ERROR("Clock %lu: Failed to disable all clock outputs (status = %ld).",
                          eventData->clockHandle, status, 0,0,0,0);
   }
}

/* examplePtpEventMsgIntvlChange */
/**
   Notifies APR of a message rate interval change.

  Parameters:
   [in]   eventData  Data about the interval change event.

  Return Value:  N/A

*******************************************************************************/
static void examplePtpEventMsgIntvlChange(zl303xx_PtpEventMsgIntvlChangeS *eventData)
{
    const char *fnName = "examplePtpEventMsgIntvlChange";

    ZL303XX_TRACE_ALWAYS("%s: EVENT Notification", fnName, 0,0,0,0,0);

#if defined APR_INCLUDED
   zlStatusE status = ZL303XX_OK;
   Uint16T serverId = eventData->streamHandle;
   examplePtpStreamExtDataS *streamExtData = eventData->streamExtData;
   zl303xx_AprConfigure1HzS config1Hz;
   void *cguId;
   zl303xx_BooleanE bForwardPath = ZL303XX_FALSE;

   status = ZL303XX_CHECK_POINTER(streamExtData);
   if (status == ZL303XX_OK)
   {
       cguId = streamExtData->cguId;
       status = ZL303XX_CHECK_POINTER(cguId);
   }

   if (status == ZL303XX_OK)
   {
       Sint16T indx;
       Uint16T maxPtpStreams = ZL303XX_PTP_NUM_STREAMS_MAX;

       for (indx = 0; indx < maxPtpStreams; indx++)
       {
           if (Zl303xx_PtpStreamArray[indx] &&
               Zl303xx_PtpStreamArray[indx]->clockStreamHandle == eventData->streamHandle)
           {
           #if defined MUX_PTP_STREAMS_TO_APR_SLOTS
               serverId = Zl303xx_PtpStreamArray[indx]->aprSlotHandle;

               ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_BMC, 3, "%s: Using APR SLOT HANDLE = %d for requestedHandle = %d at index = %d",
                           fnName, serverId, eventData->streamHandle, indx, 0,0);
           #else
               serverId = Zl303xx_PtpStreamArray[indx]->clockStreamHandle;
           #endif

               break;
           }
       }

       if (indx >= maxPtpStreams)   /* Broken! */
       {
           status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
           ZL303XX_TRACE_ALWAYS("%s: PTP STREAM NOT FOUND in Zl303xx_PtpStreamArray; status = %d",
                              fnName, status, 0,0,0,0);
       }
   }

   if (status == ZL303XX_OK)
   {
       /* Return early if the message interval change isn't Sync, Delay_Req/Resp or Peer_Req/Resp. */
       if ((eventData->messageType != ZL303XX_MSG_ID_SYNC) &&
           (eventData->messageType != ZL303XX_MSG_ID_DELAY_REQ) &&
           (eventData->messageType != ZL303XX_MSG_ID_PEER_DELAY_REQ))
       {
          return;
       }

       bForwardPath = eventData->messageType == ZL303XX_MSG_ID_SYNC ? ZL303XX_TRUE : ZL303XX_FALSE;   /* Evaluate to boolean */

       status = zl303xx_AprNotifyServerPktRateChange(
                   cguId,
                   serverId,
                   bForwardPath,
                   log2IntvlToPps(eventData->currIntvl));

       if (status != ZL303XX_OK)
       {
          ZL303XX_TRACE_ERROR("Stream %lu: Call to zl303xx_AprNotifyServerPktRateChange() failed with status=%d.",
                              serverId, status, 0,0,0,0);
       }
   }

   if (status == ZL303XX_OK)
   {
       if (bForwardPath || zl303xx_AprGetUseReversePath())
       {
           status = zl303xx_AprGetServer1HzConfigData(cguId,
                                                    serverId,
                                                    bForwardPath,
                                                    &config1Hz);
           if (status != ZL303XX_OK)
           {
              ZL303XX_TRACE_ERROR("Server %lu: Call to zl303xx_AprGetServer1HzConfigData() failed with status=%d.",
                                  serverId, status, 0,0,0,0);
           }

           if (status == ZL303XX_OK && !config1Hz.disabled)
           {
               status = exampleConfigure1Hz(
                                cguId,
                                ZL303XX_TRUE,                              /* enable 1Hz */
                                serverId,                 /* server ID */
                                config1Hz.realignmentType,               /* realignment type */
                                config1Hz.realignmentInterval,           /* realignment interval */
                                bForwardPath,                            /* forward */
                                log2IntvlToPps(eventData->currIntvl),    /* new packet rate */
                                ZL303XX_FALSE);                            /* startupModifier */

               if (status != ZL303XX_OK)
               {
                   ZL303XX_TRACE_ERROR("Server %u: Call to exampleConfigure1Hz() %s failed with status=%d.",
                                       serverId, bForwardPath ? "forward" : "reverse",
                                       status, 0,0,0);
               }
           }
       }
   }
#else
   /* INSERT CODE here to handle a message rate change. */
   (void)eventData;  /* Warning removal */
#endif
}

/* examplePtpEventContractGranted */
/**
   Logs info when a PTP contract is granted.

  Parameters:
   [in]   eventData  Data about the granted contract.

*******************************************************************************/
static void examplePtpEventContractGranted(zl303xx_PtpEventContractGrantedS *eventData)
{
   (void)eventData;  /* Warning removal */

   /* Info already logged on ZL303XX_MOD_ID_PTP_UNICAST, level 3 */
   /*const char *msgTypeStr = zl303xx_PtpMessageTypeToStr(eventData->msgType);
   const char *flowStr = eventData->contractFlow == ZL303XX_PTP_CONTRACT_RX ? "RX" : "TX";

   ZL303XX_TRACE_ALWAYS("Stream %u: %s %s contract granted (intvl=%d, dur=%u, r=%d)",
                        eventData->streamHandle, msgTypeStr, flowStr,
                        eventData->logMessageInterval, eventData->durationSec,
                        eventData->renewalInvited);*/
}

/* examplePtpEventContractExpired */
/**
   Logs info when a PTP contract expires.

  Parameters:
   [in]   eventData  Data about the expired contract.

*******************************************************************************/
static void examplePtpEventContractExpired(zl303xx_PtpEventContractExpiredS *eventData)
{
   const char *msgTypeStr = zl303xx_PtpMessageTypeToStr(eventData->msgType);
   const char *flowStr = eventData->contractFlow == ZL303XX_PTP_CONTRACT_RX ? "RX" : "TX";

   ZL303XX_TRACE_ALWAYS("Stream %u: %s %s contract expired",
                        eventData->streamHandle, msgTypeStr, flowStr, 0,0,0);
}

/* examplePtpEventUnknownTlv */
/**
   Handles the PTP Unknown TLV Event.

  Parameters:
   [in]   eventData  Data about the Unknown TLV.

*******************************************************************************/
static void examplePtpEventUnknownTlv(zl303xx_PtpEventUnknownTlvS *eventData)
{
   ZL303XX_TRACE_ALWAYS("Unknown TLV: Port %u: ", eventData->portHandle, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("=======================================", 0,0,0,0,0,0);
   zl303xx_PtpV2MsgHeaderTrace(
         (zl303xx_PtpV2MsgHeaderS*)&(eventData->ptpMsgRx), ZL303XX_MOD_ID_PTP_TLV, 2);
}

/* examplePtpEventLeapSecondsFlagChange */
/**
   Handles the change in a clock's leap59 or leap61 flags (of the Time
   Properties data set).

  Parameters:
   [in]   eventData  Data about the Time Properties data set and previous leap
                           seconds flags values.

*******************************************************************************/
static void examplePtpEventLeapSecondsFlagChange(
      zl303xx_PtpEventLeapSecondsFlagChangeS *eventData)
{
   ZL303XX_TRACE_ALWAYS("LEAP Seconds Change Event: Clock %u: ",
         eventData->clockHandle, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("======================================", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("  Parameter  |  current  |  previous  |", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("======================================", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("   leap59    |     %d     |     %d      |",
         eventData->currentTimeProperties.leap59, eventData->previousLeap59, 0,0,0,0);
   ZL303XX_TRACE_ALWAYS("   leap61    |     %d     |     %d      |",
         eventData->currentTimeProperties.leap61, eventData->previousLeap61, 0,0,0,0);
   ZL303XX_TRACE_ALWAYS("======================================", 0,0,0,0,0,0);
}

/* examplePtpEventUtcOffsetChange */
/**
   Handles a change in a clock's UTC Offset value or UTC Valid flag (of the Time
   Properties data set).

  Parameters:
   [in]   eventData  Data about the Time Properties data set and previous UTC
                           Offset and UTC Valid flag.

*******************************************************************************/
static void examplePtpEventUtcOffsetChange(
      zl303xx_PtpEventUtcOffsetChangeS *eventData)
{
   ZL303XX_TRACE_ALWAYS("UTC Offset Change Event: Clock %u: ",
         eventData->clockHandle, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("======================================", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("  Parameter  |  current  |  previous  |", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("======================================", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("  utcOffset  |     %3d   |     %3d    |",
         eventData->currentTimeProperties.currentUtcOffset,
         eventData->previousUtcOffset, 0,0,0,0);
   ZL303XX_TRACE_ALWAYS("  utcValid   |       %d   |       %d    |",
         eventData->currentTimeProperties.currentUtcOffsetValid,
         eventData->previousUtcOffsetValid, 0,0,0,0);
   ZL303XX_TRACE_ALWAYS("======================================", 0,0,0,0,0,0);

   /* Update the Clocks data if the UTC OFFSET is valid. */
   if (eventData->currentTimeProperties.currentUtcOffsetValid == ZL303XX_TRUE)
   {
      eventData->localTimeProperties.currentUtcOffset = eventData->currentTimeProperties.currentUtcOffset;

      if (ZL303XX_OK != zl303xx_PtpCurrentUtcOffsetSetLocal(
                              eventData->clockHandle,
                              eventData->currentTimeProperties.currentUtcOffset))
      {
         ZL303XX_TRACE_ALWAYS(
               "examplePtpEventUtcOffsetChange: ERROR Setting Local Current UTC Offset",
               0,0,0,0,0,0);
      }
   }
}

/* examplePtpEventRxStepsRemovedExceeded */
/**
   Receives a message that a 'rogue' ANNOUNCE message may have been received
   (and discarded) by a clock.

  Parameters:
   [in]   eventData  Data about the potential rogue ANNOUNCE message.

*******************************************************************************/
static void examplePtpEventRxStepsRemovedExceeded(
      zl303xx_PtpEventRxStepsRemovedExceededS *eventData)
{
   static char txPortIdStr[64];
   static char rxPortIdStr[64];

   (void)zl303xx_PtpV2PortIdentityToString(&eventData->ptpHeader.sourcePortIdentity, txPortIdStr);
   (void)zl303xx_PtpV2PortIdentityToString(&eventData->rxPortIdentity, rxPortIdStr);

   ZL303XX_TRACE_ALWAYS("MAX Steps Removed Event message: Clock %u: ",
         eventData->clockHandle, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("======================================", 0,0,0,0,0,0);

   ZL303XX_TRACE_ALWAYS("               Source Port:  %s",
         txPortIdStr, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("            Receiving Port:  %s",
         rxPortIdStr, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("  Transmitted stepsRemoved:  %d",
         eventData->announceData.stepsRemoved, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("    Max stepsRemoved Limit:  %d",
         eventData->maxStepsRemoved, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("======================================", 0,0,0,0,0,0);
}

/* examplePtpEventParentDsUpdated */
/**
   Notification that the Parent Data Set on a clock has been updated.

  Parameters:
   [in]   eventData  Data about the potential rogue ANNOUNCE message.

*******************************************************************************/
static void examplePtpEventParentDsUpdated(
      zl303xx_PtpEventParentDsChangeS *eventData)
{
   static char localClockIdStr[32];
   static char gmClockIdStr[32];
   static char parentPortIdStr[64];

   (void)zl303xx_PtpV2ClockIdentityToString(eventData->clockIdentity, localClockIdStr);
   (void)zl303xx_PtpV2ClockIdentityToString(eventData->parentDS.grandmasterIdentity, gmClockIdStr);
   (void)zl303xx_PtpV2PortIdentityToString(&eventData->parentDS.parentPortIdentity, parentPortIdStr);

   ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_PTP_STATE_UPD, 2)
   {
      ZL303XX_TRACE_ALWAYS("Parent Data Set Update Event message: Clock %u:(%s) ",
            eventData->clockHandle, localClockIdStr, 0,0,0,0);
      ZL303XX_TRACE_ALWAYS("================================================================",
            0,0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("    GM Identity:  %s", gmClockIdStr, 0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("    Source Port:  %s", parentPortIdStr, 0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("     GM Quality:  %d:0x%02X:0x%04X",
            eventData->parentDS.grandmasterClockQuality.clockClass,
            eventData->parentDS.grandmasterClockQuality.clockAccuracy,
            eventData->parentDS.grandmasterClockQuality.offsetScaledLogVariance, 0,0,0);
      ZL303XX_TRACE_ALWAYS(" GM Priority1/2:  %d:%d",
            eventData->parentDS.grandmasterPriority1,
            eventData->parentDS.grandmasterPriority2, 0,0,0,0);
      ZL303XX_TRACE_ALWAYS("================================================================",
            0,0,0,0,0,0);
   }
}

/* examplePtpEventPortFaulty */
/**
   Notification that a port went into faulty state

  Parameters:
   [in]   eventData  Information about the port and fault type */
static void examplePtpEventPortFaulty(zl303xx_PtpEventPortFaultS *eventData)
{
   zlStatusE status = ZL303XX_CHECK_POINTER(eventData);
   if(status == ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS("Port Faulty: Port %u:", eventData->portHandle, 0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("Port Faulty: Fault Type %u:", eventData->portFaultType, 0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("=======================================", 0,0,0,0,0,0);
   }
}

/* examplePtpEventStreamDisqualified */
/**
   Notification that a stream has been disqualified.

  Parameters:
   [in]   eventData  Information about the stream and clock */
static void examplePtpEventStreamDisqualified(zl303xx_PtpEventStreamDisqualifiedS *eventData)
{
   if (eventData == NULL)
   {
      ZL303XX_TRACE_ALWAYS("examplePtpEventStreamKeepAlive: (ERROR) eventData is NULL.",
            0,0,0,0,0,0);

      return;
   }
#if defined ZL303XX_REFERENCE_SELECTION
   {
      /* Send an update for this stream to the Server Selection Handler. */
      /***************************************************************************/
      /* Translate the PTP Event message into a type for transmission over the
       * distributed backplane.                                                  */
      /***************************************************************************/
      exampleRefSelectBpMsgS txMsg;

      /* Stream UPDATE messages from the Application. */
      txMsg.source = REF_SELECT_SRC_MOD_PROT_STREAM;
      txMsg.action = REF_SELECT_ACTION_UPDATE;

      txMsg.data.streamUpdate.streamHandle = eventData->streamHandle;
      txMsg.data.streamUpdate.clockHandle = eventData->clockHandle;
      txMsg.data.streamUpdate.streamDataValid = ZL303XX_FALSE;
#if defined APR_INCLUDED
      /***************************************************************************/
      /* Ordinarily, the data would be sent over the backplane at this point and
       * the next command executed within the context of the Timing Card CPU.
       * Here, both run on a single CPU.                                         */
      /***************************************************************************/

      /* Send the Data to the Server Database. */
      (void)exampleRefSelectSendMsgToBackplane(&txMsg);
#else
/* #warning MSCC: Translation of PTP message to a Backplane message is required. */
      ZL303XX_TRACE_ALWAYS("Line Card Event ready for BP transmission (%d)",
            ZL303XX_PTP_EVENT_STREAM_DISQUALIFIED, 0,0,0,0,0);
#endif
      return;
   }
#endif

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_STATE_UPD, 2, "Stream Disqualified Event Message: streamHandle: %u",
         eventData->streamHandle, 0, 0,0,0,0);

}
/* examplePtpHwClockTimeGet */
/**
   Function to query the current estimated PTP time of the underlying hardware
   clock. This function is called for every egress PTP EVENT or ANNOUNCE message
   and if a TIME GET management TLV is received.

  Parameters:
   [in,out]   cmdParams   Data about the time to query. The PTP time is
                               returned in this structure.

  Return Value:  ZL303XX_OK  Success. Otherwise, return an error code.

*******************************************************************************/
static zlStatusE examplePtpHwClockTimeGet(zl303xx_PtpHwClockTimeGetS *cmdParams)
{
    zlStatusE status = ZL303XX_OK;

#if defined EXAMPLE_PTP_SW_TIMESTAMPS
   if (status == ZL303XX_OK)
   {
      examplePtpSwTimeStampGet(&cmdParams->output.ptpTime);
   }
#elif (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD) && defined OS_LINUX
   if (status == ZL303XX_OK)
   {
      status = zl303xx_VTSS_get_time(
                     &cmdParams->output.ptpTime.secondsField,
                     &cmdParams->output.ptpTime.nanosecondsField);
   }
#elif defined ZLS30341_INCLUDED
   {
      Uint16T epoch;
      Uint64S tod;
      zl303xx_BooleanE getTod;

      if (status == ZL303XX_OK)
      {
         getTod = (cmdParams->input.interface == ZL303XX_PTP_INTERFACE_GENERAL) ||
                  (cmdParams->input.twoStepFlag == ZL303XX_TRUE);

         status = example34xClockTimeGet(cmdParams->input.clockExtData,  /* zl303xx_Params */
                                           &epoch,
                                           getTod ? &tod : NULL);
      }

      if (status == ZL303XX_OK)
      {
         cmdParams->output.ptpTime.secondsField.hi  = epoch;
         cmdParams->output.ptpTime.secondsField.lo  = getTod ? tod.hi : 0;
         cmdParams->output.ptpTime.nanosecondsField = getTod ? tod.lo : 0;
      }
   }
#else
   /* INSERT CODE here to retrieve the estimated PTP time of the hardware clock. */
   if (status == ZL303XX_OK)
   {
      if (cmdParams->input.interface == ZL303XX_PTP_INTERFACE_EVENT)
      {
         /* This function is called for every egress EVENT message:
          *
          *   - For one-step clocks, the underlying harware may be incapable of
          *     inserting the full 80-bit PTP time stamp on the fly. This function
          *     can be used to fill in the remaining bits in software.
          *
          *   - For two-step clocks, the estimated time stamp inserted in the message
          *     must be within +/- 1 second of the actual hardware time, or must
          *     be zero-filled.
          */
         memset(&cmdParams->output.ptpTime, 0, sizeof(cmdParams->output.ptpTime));
      }
      else
      {
         /* Handle egress ANNOUNCE messages and ingress TIME GET management TLVs.
          * This above requirement for two-step clocks applies here. */
         memset(&cmdParams->output.ptpTime, 0, sizeof(cmdParams->output.ptpTime));
      }
   }
#endif

   return status;
}

/* examplePtpHwClockTimeSet */
/**
   Function to set the PTP time of a hardware clock.

  Parameters:
   [in]   cmdParams   Data about the time to set.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  Time to set is invalid.

*******************************************************************************/
static zlStatusE examplePtpHwClockTimeSet(zl303xx_PtpHwClockTimeSetS *cmdParams)
{
   zlStatusE status = ZL303XX_OK;

   /* Check that seconds field fits into 48 bits. */
   if (status == ZL303XX_OK)
   {
      if (cmdParams->input.ptpTime.secondsField.hi & 0xFFFF0000)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Invalid secondsField.hi (%lu).",
                             cmdParams->input.ptpTime.secondsField.hi, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

#if defined ZLS30341_INCLUDED
   if (status == ZL303XX_OK)
   {
      status = example34xClockTimeSet(
                     cmdParams->input.clockExtData,  /* zl303xx_Params */
                     cmdParams->input.ptpTime.secondsField.hi,
                     cmdParams->input.ptpTime.secondsField.lo);
   }
#elif (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD) && defined OS_LINUX
   if (status == ZL303XX_OK)
   {
      status = zl303xx_VTSS_set_time(
                     cmdParams->input.ptpTime.secondsField,
                     cmdParams->input.ptpTime.nanosecondsField);
   }
#else
    /* INSERT CODE here to set the time of the hardware clock. */
    (void)cmdParams;  /* Warning removal */
#endif

   return status;
}

/* examplePtpHwLockStatusGet */
/**
   Function to query the current LOCK state of the hardware clock. Whether
   LOCK is achieved may be determined by the mode of the clock or timing
   algorithm. For example, if in PHASE mode but only a FREQUENCY lock has been
   achieved then FALSE should be returned.

  Parameters:
   [in,out] cmdParams      Data about the clock LOCK status.

  Return Value:  ZL303XX_OK  Success. Otherwise, return an error code.

*******************************************************************************/
static zlStatusE examplePtpHwLockStatusGet(zl303xx_PtpHwLockStatusGetS *cmdParams)
{
   zlStatusE status = ZL303XX_OK;

   /* Set a default return value. */
   cmdParams->output.lockStatus = ZL303XX_FALSE;

#if defined APR_INCLUDED
   if (status == ZL303XX_OK)
   {
      void *zl303xx_ParamsP = cmdParams->input.clockExtData;

      /* Query the Lock Status. */
      zl303xx_AprLockStatusE currentLockStatus  = ZL303XX_LOCK_STATUS_UNKNOWN;
      status = zl303xx_AprGetDeviceLockStatus(zl303xx_ParamsP, &currentLockStatus);

      /* If we expect FREQ and the current is FREQ or PHASE then return TRUE. */
      /* If we expect PHASE and the current is PHASE then return TRUE. */
      if (status == ZL303XX_OK)
      {
         if ((currentLockStatus == ZL303XX_LOCK_STATUS_PHASE_LOCKED) ||
             (currentLockStatus == ZL303XX_LOCK_STATUS_LOCKED &&
                   cmdParams->input.phaseRequired == ZL303XX_FALSE)) /* For FREQ */
         {
            /* Expected LOCK state is achieved. */
            cmdParams->output.lockStatus = ZL303XX_TRUE;
         }
      }
   }
#else
    /* #warning MSCC TODO: Report PLL lock status to PTP */
#endif

   return status;
}

/* examplePtpHwClockStabilityGet */
/**
   Function to query the current stability of the hardware clock.
   This function is called certain custom management TLVs.

  Parameters:
   [in,out]   cmdParams   Data about the values to query. The stability
                              parameters are returned in this structure.

  Return Value:  ZL303XX_OK  Success. Otherwise, return an error code.

*******************************************************************************/
static zlStatusE examplePtpHwClockStabilityGet(zl303xx_PtpHwClockStabilityGetS *cmdParams)
{
    zlStatusE status = ZL303XX_OK;

#if defined APR_INCLUDED
   if (status == ZL303XX_OK)
   {
      Uint32T freqStability = (Uint32T)(-1);
      Sint32T phaseStability = (0);
      Uint16T serverId;

      status = zl303xx_AprGetDeviceCurrActiveRef (cmdParams->input.clockExtData, &serverId);
      if (status == ZL303XX_OK)
      {
         (void)zl303xx_AprGetServerClkStabilityIndicator (cmdParams->input.clockExtData, serverId, &freqStability);
         (void)zl303xx_AprGetDevicePhaseStabilityIndicator (cmdParams->input.clockExtData, &phaseStability);
      }

      if (status == ZL303XX_OK)
      {
         cmdParams->output.freqStability = freqStability;
         cmdParams->output.phaseStability = phaseStability;
      }
   }
#else
   /* #warning MSCC TODO: Report PLL lock stability to PTP */
   if (status == ZL303XX_OK)
   {
      cmdParams->output.freqStability = -1;
      cmdParams->output.phaseStability = -1;
   }
#endif

   return status;
}

/* examplePtpHwTimeSetStatusGet */
/**
   Function to query whether clock's Time has been set. This function may be
   called prior to a stream sending Delay_Requests.

  Parameters:
   [in,out] cmdParams   Data about the values to query. The TIME-SET
                              condition is returned in this structure.
                              -> TRUE  = Time has been set.
                              -> FALSE = Time has never been set.

  Return Value:  ZL303XX_OK  Success. Otherwise, return an error code.

*******************************************************************************/
static zlStatusE examplePtpHwTimeSetStatusGet(zl303xx_PtpHwTimeStatusGetS *cmdParams)
{
    zlStatusE status = ZL303XX_OK;

    /* Set a default return value. */
    cmdParams->output.bTimeSet = ZL303XX_FALSE;

#if defined APR_INCLUDED
    if (status == ZL303XX_OK)
    {
       void *zl303xx_ParamsP = cmdParams->input.clockExtData;
       zl303xx_BooleanE bTimeSetFlag = ZL303XX_FALSE;

       /* Get the ToD Set Status status. */
       status = zl303xx_AprGetDeviceTimeSetFlag(zl303xx_ParamsP, &bTimeSetFlag);

       if (status == ZL303XX_OK)
       {
          cmdParams->output.bTimeSet = bTimeSetFlag;
       }
    }
#else
   /* #warning MSCC TODO: Report timestamper time set status */
#endif


    ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "examplePtpHwTimeSetStatusGet: %d",  cmdParams->output.bTimeSet, 0,0,0,0,0);

   return status;
}


/* examplePtpHwPllPerformanceGet */
/**
   Function to query the PLL performance parameters of local clock.

  Parameters:
   [in,out] cmdParams   Data about the values to query. Refer to the
                              zl303xx_PtpHwPllPerformanceGetS data structure
                              for details.

  Return Value:  ZL303XX_OK  Success. Otherwise, return an error code.

*******************************************************************************/
static zlStatusE examplePtpHwPllPerformanceGet(zl303xx_PtpHwPllPerformanceGetS *cmdParams)
{
   zlStatusE status = ZL303XX_OK;

#if defined APR_INCLUDED
   /* Dereference the device parameter. */
   void *zl303xx_ParamsP = cmdParams->input.clockExtData;

   /* Query the performance data via the existing interface. */
   zl303xx_HoldoverQualityParamsS holdoverQualityParams;

   /* Query the Hardware LOCK status. */
   zl303xx_AprLockStatusE hwLockStatus;

   /* Query the performance data via the existing interface. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_GetHoldoverQuality(zl303xx_ParamsP, &holdoverQualityParams);
   }

   /* Query the hardware status via the existing interface. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_AprGetDeviceLockStatus(zl303xx_ParamsP, &hwLockStatus);
   }

   /* Map hardware specific values to PTP equivalents. */
   if (status == ZL303XX_OK)
   {
      /* Operating Mode */
      if (holdoverQualityParams.devMode == ZL303XX_ELECTRIC_MODE)
         cmdParams->output.pllPerfData.pllOperatingMode = ZL303XX_PTP_PLL_MODE_ELEC_REF_SYNC;
      else if (holdoverQualityParams.devMode == ZL303XX_PACKET_MODE)
         cmdParams->output.pllPerfData.pllOperatingMode = ZL303XX_PTP_PLL_MODE_PKT_REF_SYNC;
      else if (holdoverQualityParams.devMode == ZL303XX_HYBRID_MODE)
         cmdParams->output.pllPerfData.pllOperatingMode = ZL303XX_PTP_PLL_MODE_HYBRID_SYNCE;
      else
      {
         ZL303XX_TRACE_ALWAYS(
               "examplePtpHwPllPerformanceGet: (WARNING) Cannot Map Operating Mode (%d)",
               holdoverQualityParams.devMode, 0,0,0,0,0);
         cmdParams->output.pllPerfData.pllOperatingMode = ZL303XX_PTP_PLL_MODE_UNKNOWN;
      }

      /* Hardware Lock State */
      if (hwLockStatus == ZL303XX_LOCK_NO_ACTIVE_SERVER)
      {
         cmdParams->output.pllPerfData.hwPllOperatingState = ZL303XX_PTP_PLL_STATE_FREERUN;
      }
      else if ((hwLockStatus == ZL303XX_LOCK_STATUS_HOLDOVER) ||
               (hwLockStatus == ZL303XX_LOCK_STATUS_REF_FAILED))
      {
         cmdParams->output.pllPerfData.hwPllOperatingState = ZL303XX_PTP_PLL_STATE_HOLDOVER;
      }
      else if (hwLockStatus == ZL303XX_LOCK_STATUS_ACQUIRING)
      {
         cmdParams->output.pllPerfData.hwPllOperatingState = ZL303XX_PTP_PLL_STATE_FREQ_ACQUIRING;
      }
      else if (hwLockStatus == ZL303XX_LOCK_STATUS_LOCKED)
      {
         cmdParams->output.pllPerfData.hwPllOperatingState = ZL303XX_PTP_PLL_STATE_FREQ_ACQUIRED;
      }
      else if (hwLockStatus == ZL303XX_LOCK_STATUS_PHASE_LOCKED)
      {
         cmdParams->output.pllPerfData.hwPllOperatingState = ZL303XX_PTP_PLL_STATE_PHASE_ACQUIRED;
      }
      else
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_AprGetDeviceLockStatus: (WARNING) Cannot Map Hardware Operating State (%d)",
               hwLockStatus, 0,0,0,0,0);
         cmdParams->output.pllPerfData.hwPllOperatingState = ZL303XX_PTP_PLL_STATE_UNKNOWN;
      }

      /* Software Lock State */
      if ((holdoverQualityParams.aprState == ZL303XX_MANUAL_FREERUN) ||
          (holdoverQualityParams.aprState == ZL303XX_NO_ACTIVE_SERVER))
      {
         cmdParams->output.pllPerfData.swPllOperatingState = ZL303XX_PTP_PLL_STATE_FREERUN;
      }
      else if ((holdoverQualityParams.aprState == ZL303XX_HOLDOVER) ||
               (holdoverQualityParams.aprState == ZL303XX_MANUAL_HOLDOVER)  ||
               (holdoverQualityParams.aprState == ZL303XX_MANUAL_SERVO_HOLDOVER))
      {
         cmdParams->output.pllPerfData.swPllOperatingState = ZL303XX_PTP_PLL_STATE_HOLDOVER;
      }
      else if (holdoverQualityParams.aprState == ZL303XX_FREQ_LOCK_ACQUIRING)
      {
         cmdParams->output.pllPerfData.swPllOperatingState = ZL303XX_PTP_PLL_STATE_FREQ_ACQUIRING;
      }
      else if (holdoverQualityParams.aprState == ZL303XX_FREQ_LOCK_ACQUIRED)
      {
         cmdParams->output.pllPerfData.swPllOperatingState = ZL303XX_PTP_PLL_STATE_FREQ_ACQUIRED;
      }
      else if (holdoverQualityParams.aprState == ZL303XX_PHASE_LOCK_ACQUIRED)
      {
         cmdParams->output.pllPerfData.swPllOperatingState = ZL303XX_PTP_PLL_STATE_PHASE_ACQUIRED;
      }
      else if (holdoverQualityParams.aprState == ZL303XX_REF_FAILED)
      {
         cmdParams->output.pllPerfData.swPllOperatingState = ZL303XX_PTP_PLL_STATE_REF_FAILED;
      }
      else
      {
         ZL303XX_TRACE_ALWAYS(
               "examplePtpHwPllPerformanceGet: (WARNING) Cannot Map Software Operating State (%d)",
               holdoverQualityParams.aprState, 0,0,0,0,0);
         cmdParams->output.pllPerfData.swPllOperatingState = ZL303XX_PTP_PLL_STATE_UNKNOWN;
      }

      /* Holdover Quality */
      if (holdoverQualityParams.currentHoldoverCategory == ZL303XX_FREQ_CATX_UNKNOWN)
         cmdParams->output.pllPerfData.pllHoldoverQuality = ZL303XX_PTP_PLL_HOLDOVER_QUALITY_UNKNOWN;
      else if (holdoverQualityParams.currentHoldoverCategory == ZL303XX_FREQ_CAT1)
         cmdParams->output.pllPerfData.pllHoldoverQuality = ZL303XX_PTP_PLL_HOLDOVER_QUALITY_CAT1;
      else if (holdoverQualityParams.currentHoldoverCategory == ZL303XX_FREQ_CAT2)
         cmdParams->output.pllPerfData.pllHoldoverQuality = ZL303XX_PTP_PLL_HOLDOVER_QUALITY_CAT2;
      else if (holdoverQualityParams.currentHoldoverCategory == ZL303XX_FREQ_CAT3)
         cmdParams->output.pllPerfData.pllHoldoverQuality = ZL303XX_PTP_PLL_HOLDOVER_QUALITY_CAT3;
      else
      {
         cmdParams->output.pllPerfData.pllHoldoverQuality = ZL303XX_PTP_PLL_HOLDOVER_QUALITY_OTHER;
      }

      /* Holdover In or Out of Spec */
      if ((holdoverQualityParams.holdoverQuality == HOLDOVER_QUALITY_IN_SPEC) ||
          (holdoverQualityParams.holdoverQuality == HOLDOVER_QUALITY_LOCKED_TO_SYNCE))
      {
         cmdParams->output.pllPerfData.bHoldoverInSpec = ZL303XX_TRUE;
      }
      else
      {
         cmdParams->output.pllPerfData.bHoldoverInSpec = ZL303XX_FALSE;
      }

      /* SyncE Traceability */
      if (holdoverQualityParams.syncEStatus == HOLDOVER_SYNCE_TRACEABLE)
      {
         cmdParams->output.pllPerfData.synceTraceable = ZL303XX_TRUE;
      }
      else
      {
         cmdParams->output.pllPerfData.synceTraceable = ZL303XX_FALSE;
      }

      /* Settling Time Active */
      cmdParams->output.pllPerfData.settlingTimeActive = exampleGetPtpPLLSettlingTimeOverride(cmdParams->input.clockHandle);
   }

#else
    /* #warning MSCC TODO: Populate PLL performance data */
   (void)cmdParams;     /* Warning removal */
#endif

   return status;
}

#ifdef EXAMPLE_PTP_SW_TIMESTAMPS
/* examplePtpSwTimeStampGet */
/**
   A function used to convert the OS tick counter to a PTP-format time stamp.

*******************************************************************************/
static void examplePtpSwTimeStampGet(zl303xx_TimeStamp *pTimeStamp)
{
   /* CPU tick counter extended to 64 bits. */
   static Uint64S currCpuTicks = {0,0};

   Uint32T readCpuTicks = OS_TICK_GET();
   Uint32T mod;

   /* Update the CPU tick counter. */
   if (readCpuTicks < currCpuTicks.lo)
   {
      currCpuTicks.hi++;
   }
   currCpuTicks.lo = readCpuTicks;

   /* Convert the CPU tick counter to PTP time stamp format. */
   pTimeStamp->secondsField = Div_U64S_U32(currCpuTicks, OS_TICK_RATE_GET(), &mod);
   pTimeStamp->nanosecondsField = RatioConvert_U32(mod, OS_TICK_RATE_GET(), ONE_BILLION);
}
#endif

#define E25G_RTSTAMP 0 /* 受信タイムスタンプ取得 */
#define E25G_TTSTAMP 1 /* 送信タイムスタンプ取得 */
//#include "f_rrh_def.h"
#include "f_du_reg_eth.h"
#include "BPF_HM_DEVC.h"

extern INT f_ptp_t3_adj_val;
extern INT f_ptp_t2_adj_val;

/* Timestamp取得関数                                                           */
/* FPGAから時刻情報を取得する                                                  */
/* 2018/08/22 ハソ7-3-6-4 PTPタイムスタンプ補正対応追加                        */
/* 2018/10/05 seq番号を送受信情報に使用していたが、引数に持たせる (0:Rx 1:Tx)  */
/*******************************************************************************/
static void Get25gTimestamps(Sint32T fd , Uint16T seq, zl303xx_TimeStamp *pTimeStamp, Uint16T trx)
{
	struct ifreq ifr;
	
	e25g_tstamp tstamp;
	e25g_tstamp res_tstamp;
	Uint48T sec; 
	Uint32T nano;
	int	ret;
	unsigned int reg_data;
	unsigned int reg_addr;

	Uint32T portNo = zl303xx_GetExampleInterface();
	zlStatusE status = ZL303XX_OK;
	
	Sint32T adj;
	Sint32T tmp_nano;
	long long tmp_sec;
	
	char *ifName = zl303xx_GetExampleInterfaceStr(portNo);
	const char* msg_name;

	/* parameter set */
	if (trx == 0)
	{
		tstamp.kind = E25G_RTSTAMP;
		tstamp.ptp_tag = seq;
		msg_name = "SYNC";
	}
	else
	{
		tstamp.kind = E25G_TTSTAMP;
		tstamp.ptp_tag = seq;
		msg_name = "DELAY_REQ";
	}

	memset(&ifr, 0, sizeof(ifr));		/* 0 clear */
	strncpy(ifr.ifr_name, ifName, IFNAMSIZ-1);
	ifr.ifr_data = &tstamp;
	
	/* ファイルディスクリプタ情報は上位から渡すのでここではopenしないように変更 */
	/* if (open( pTblEntry->fd ,O_RDONLY))<0 */
	/* if ((fd = open( name , O_RDONLY)) < 0) */
	/* { */
		/* error */
	/*	ZL303XX_TRACE_ALWAYS("Error opening Discripter.", 0, 0,0,0,0,0); */
	/*	status = ZL303XX_FALSE; */
	/* } */

	/* ioctl */
	if((ret = ioctl(fd, SIOCGHWTSTAMP, &ifr)) < 0)
	{
		/* error */
		ZL303XX_TRACE_ALWAYS("Get25gTimestamps:%s seq=%d,ioctl error(%d),fhethS%d,fd=%d",
								msg_name,
								seq,
								ret,
								portNo,
								fd,
								0);

		status = ZL303XX_ERROR;
		/* fail safe for TAG ERROR */
		if(tstamp.kind == E25G_TTSTAMP){
			reg_addr = M_DU_REG_ETH_PORT0(D_DU_REG_ETH_MACSYSRST);
			reg_data=1;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,reg_addr,&reg_data);
			reg_data=0;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,reg_addr,&reg_data);
		}
	}

	if (status == ZL303XX_OK)
	{
		/* timestamp詰め替え										*/
		/*	   ptp_time[0] 下位16bit分がsecondsFieldの上位16bit分	*/
		/*	   ptp_time[1] 32bit分がsecondsFieldの下位32bit分		*/
		/*	   ptp_time[2] 32bit分がnanosecondsField				*/
	
		memcpy(&res_tstamp, ifr.ifr_data, sizeof(e25g_tstamp));
	
		sec.hi = res_tstamp.ptp_time[0];
		sec.lo = res_tstamp.ptp_time[1];
		nano = res_tstamp.ptp_time[2] ;
		tmp_sec = (long long)(((long long)(sec.hi) << 32) | ((long long)(sec.lo) & 0xFFFFFFFF));

		/* ハードソフト仕様書7-3-6-4章対応 */
		/* T3には+84.93ns、T2には-533.37nsの補正を加える */
		/* 小数点は誤差レベルなので切り捨てる */
		if (trx == 0)						/* T2 */
		{
			adj = (Sint32T)(-533 + f_ptp_t2_adj_val);		/* -533+(-512~512) */
		}
		else								/* T3 */
		{
			adj = (Sint32T)(84 + f_ptp_t3_adj_val);		/* 84+(-512~512) */
		}
		tmp_nano = nano + adj;
		if( tmp_nano < 0 ){
			tmp_sec -= 1;
			if(tmp_sec < 0){
				ZL303XX_TRACE_ALWAYS("Get25gTimestamps:%s seq=%d Strange under flow %d%010d.%010d",
								msg_name,
								seq,
								sec.hi,sec.lo,nano,0);
			}
			nano = (Uint32T)(1000000000 + tmp_nano);
		}
		else
		{
			nano = (Uint32T)tmp_nano;
		}
		sec.hi = (Uint32T)((tmp_sec >> 32) & 0xFFFFFFFF);
		sec.lo = (Uint32T)(tmp_sec & 0xFFFFFFFF);

		pTimeStamp->secondsField = sec;
		pTimeStamp->nanosecondsField = nano;

		ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,"Get25gTimestamps:%s seq=%d pre %d%010d.%010d DBG:0x%08x",
													msg_name,
													seq,
													res_tstamp.ptp_time[0], res_tstamp.ptp_time[1], res_tstamp.ptp_time[2], res_tstamp.ptp_time[3]);
		ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,"Get25gTimestamps:%s seq=%d updated %d%010d.%d adj:%010d",
													msg_name,
													seq,
													sec.hi, sec.lo, nano, adj);
	}
}



/* exampleSetSrcDestAddressesToStructs */
/**
   A function used to convert the local and dest addresses into correct structs.
   Assigns them to zlExampleApp.port[0] and zlExampleApp.stream[0]

*******************************************************************************/
zlStatusE exampleSetSrcDestAddressesToStructs(const char *srcAddr, const char *destAddr)
{
    zlStatusE status = ZL303XX_OK;

    /* Assume we are associated with clock[0] (this is a normal assumption throughout the example code) */

     if (strlen(srcAddr) == 0)
     {
        memset(&examplePtpLocalAddr[0], 0, sizeof(examplePtpLocalAddr));
        examplePtpLocalAddr[0].networkProtocol = ZL303XX_NETWORK_PROTOCOL_INVALID;

        memset(&examplePtpLocalAddr[1], 0, sizeof(examplePtpLocalAddr));
        examplePtpLocalAddr[1].networkProtocol = ZL303XX_NETWORK_PROTOCOL_INVALID;


        status = ZL303XX_PARAMETER_INVALID;
        ZL303XX_TRACE_ALWAYS("exampleSetSrcDestAddressesToStructs: No SOURCE IPV4 address; status=%d", status, 0,0,0,0,0);
     }

     if (strlen(destAddr) == 0)
     {
        memset(&examplePtpDestAddr[0], 0, sizeof(examplePtpDestAddr));
        examplePtpDestAddr[0].networkProtocol  = ZL303XX_NETWORK_PROTOCOL_INVALID;
        memset(&examplePtpDestAddr[1], 0, sizeof(examplePtpDestAddr));
        examplePtpDestAddr[1].networkProtocol  = ZL303XX_NETWORK_PROTOCOL_INVALID;

        ZL303XX_TRACE_ALWAYS("exampleSetSrcDestAddressesToStructs: No DEST IPV4 address (OK for UniNegMaster only); status=%d", status, 0,0,0,0,0);
     }

     if (status == ZL303XX_OK)
     {
         if (strlen(srcAddr) != 0)
             status = examplePtpLocalIpv4Set(srcAddr, ZL303XX_TRUE, &zlExampleApp.port[0].ptp);
         if (strlen(destAddr) != 0)
             status = examplePtpDestIpv4Set(destAddr, ZL303XX_TRUE, &zlExampleApp.stream[0].ptp);
     }

    return status;
}


/*****************   START OF CANNED EXAMPLES   *******************************/

#if defined ZLS30390_INCLUDED

extern exampleAppS zlExampleApp;

extern zl303xx_ParamsS *zl303xx_Params;

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/


/* examplePtpMultiMaster */
/**
   An example of how to configure a PTP node that uses multicast transport.

*******************************************************************************/
zlStatusE examplePtpMultiMaster(void)
{
   zlStatusE status = ZL303XX_OK;

   const char *srcIpv4Addr  = DEFAULT_MASTER_IPV4_ADDRESS;
   const char *destIpv4Addr = DEFAULT_PTP_DOMAIN_ADDRESS_IPV4;

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStructInit(1, 1, 1, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
       status = exampleSetSrcDestAddressesToStructs(srcIpv4Addr, destIpv4Addr);
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = 13;

      zlExampleApp.stream[0].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
      zlExampleApp.stream[0].ptp.config.unicast = ZL303XX_FALSE;

      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}

/* examplePtpMultiSlave */
/**
   An example of how to configure a PTP node that uses multicast transport and
   the Best Master Clock Algorithm.

*******************************************************************************/
zlStatusE examplePtpMultiSlave(void)
{
   zlStatusE status = ZL303XX_OK;

   const char *srcIpv4Addr  = DEFAULT_SLAVE_IPV4_ADDRESS;
   const char *destIpv4Addr = DEFAULT_PTP_DOMAIN_ADDRESS_IPV4;

#if defined APR_INCLUDED
   if (status == ZL303XX_OK)
   {
      /* Setup global configuration options */
      status |= zl303xx_AprSetPktRate(64, ZL303XX_TRUE);
      status |= zl303xx_AprSetPktRate(16, ZL303XX_FALSE);

      status |= exampleAprSetConfigParameters(ACI_DEFAULT );
   }
#endif

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStructInit(1, 1, 1, &zlExampleApp);    /* 1 clock, 1 port, 1 stream(server) */
   }

   if (status == ZL303XX_OK)
   {
       status = exampleSetSrcDestAddressesToStructs(srcIpv4Addr, destIpv4Addr);
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = 248;

      zlExampleApp.stream[0].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
      zlExampleApp.stream[0].ptp.config.unicast = ZL303XX_FALSE;

      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}

/* examplePtpUniNegMaster */
/**
   An example of how to configure a PTP master that uses unicast negotiation.

*******************************************************************************/
zlStatusE examplePtpUniNegMaster(void)
{
   zlStatusE status = ZL303XX_OK;

   const char *srcIpv4Addr = DEFAULT_MASTER_IPV4_ADDRESS;
   const char *destIpv4Addr = "";

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStructInit(1, 1, 0, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
       exampleSetSrcDestAddressesToStructs(srcIpv4Addr, destIpv4Addr);  /* Ignore error return for dest addr */
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = 13;

      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_TRUE;

      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}

/* examplePtpUniNegSlave */
/**
   An example of how to configure a PTP slave that uses unicast negotiation.

*******************************************************************************/
zlStatusE examplePtpUniNegSlave(void)
{
   zlStatusE status = ZL303XX_OK;

   const char *srcIpv4Addr  = DEFAULT_SLAVE_IPV4_ADDRESS;
   const char *destIpv4Addr = DEFAULT_MASTER_IPV4_ADDRESS;

#if defined APR_INCLUDED
   if (status == ZL303XX_OK)
   {
      /* Setup global configuration options */
      status |= zl303xx_AprSetPktRate(64, ZL303XX_TRUE);
      status |= zl303xx_AprSetPktRate(16, ZL303XX_FALSE);

      status |= exampleAprSetConfigParameters(ACI_DEFAULT );
   }
#endif

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStructInit(1, 1, 1, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
       status = exampleSetSrcDestAddressesToStructs(srcIpv4Addr, destIpv4Addr);
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = 248;

      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_TRUE;

      zlExampleApp.stream[0].ptp.config.mode = ZL303XX_PTP_STREAM_SLAVE_ONLY;
      zlExampleApp.stream[0].ptp.config.unicast = ZL303XX_TRUE;

      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}

/* examplePtpUniNegSlaveHybrid */
/**
   An example of how to configure a PTP slave that uses unicast negotiation and
   with a hybrid mode APR.

*******************************************************************************/
zlStatusE examplePtpUniNegSlaveHybrid(void)
{
   zlStatusE status = ZL303XX_OK;

   const char *srcIpv4Addr  = "10.0.0.205";
   const char *destIpv4Addr = "10.0.0.230";

#if defined APR_INCLUDED
   if (status == ZL303XX_OK)
   {
      /* Setup global configuration options */
      status |= zl303xx_AprSetPktRate(64, ZL303XX_TRUE);
      status |= zl303xx_AprSetPktRate(16, ZL303XX_FALSE);

      status |= exampleAprSetConfigParameters(ACI_DEFAULT );
   }

   if (status == ZL303XX_OK)
   {
      /* Set the default device reference mode to hybrid */
      status = zl303xx_AprSetDeviceOptMode(ZL303XX_HYBRID_MODE);
   }

   if (status == ZL303XX_OK)
   {
      /* Set the default hybrid configuration for the server to be registered */
      status = zl303xx_AprSetHybridServerFlag(ZL303XX_TRUE);
   }
#endif

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStructInit(1, 1, 1, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
       status = exampleSetSrcDestAddressesToStructs(srcIpv4Addr, destIpv4Addr);
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = 248;

      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_TRUE;

      zlExampleApp.stream[0].ptp.config.mode = ZL303XX_PTP_STREAM_SLAVE_ONLY;
      zlExampleApp.stream[0].ptp.config.unicast = ZL303XX_TRUE;

      status = exampleAppStart(&zlExampleApp);
   }

#if defined APR_INCLUDED
   if (status == ZL303XX_OK)
   {
       zl303xx_ParamsS *zl303xx_ParamsP = NULL;

      /* Use the first clock params */
#if defined ZLS30361_INCLUDED
       if (zl303xx_ParamsP == NULL)
       {
         zl303xx_ParamsP = zlExampleApp.clock[TARGET_DPLL].zl3036x.zl303xx_Params;
       }
#endif
#if defined ZLS30721_INCLUDED
       if (zl303xx_ParamsP == NULL)
       {
         zl303xx_ParamsP = zlExampleApp.clock[TARGET_DPLL].zl3072x.zl303xx_Params;
       }
#endif
#if defined ZLS30701_INCLUDED
       if (zl303xx_ParamsP == NULL)
       {
         zl303xx_ParamsP = zlExampleApp.clock[TARGET_DPLL].zl3070x.zl303xx_Params;
       }
#endif
#if defined ZLS30751_INCLUDED
       if (zl303xx_ParamsP == NULL)
       {
         zl303xx_ParamsP = zlExampleApp.clock[TARGET_DPLL].zl3075x.zl303xx_Params;
       }
#endif
#if defined ZLS30341_INCLUDED
       if (zl303xx_ParamsP == NULL)
       {
         zl303xx_ParamsP = zlExampleApp.clock[TARGET_DPLL].zl3034x.zl303xx_Params;
       }
#endif
#if defined ZLS30771_INCLUDED
       if (zl303xx_ParamsP == NULL)
       {
         zl303xx_ParamsP = zlExampleApp.clock[TARGET_DPLL].zl3077x.zl303xx_Params;
       }
#endif

      /* Enable 1Hz for the device */
      status = zl303xx_AprSetDevice1HzEnabled(zl303xx_ParamsP, ZL303XX_TRUE);

   }

#endif

   return status;
}

/* examplePtpUniNegMaster2 */
/**
   An example of how to configure a PTP master that uses unicast negotiation.
   This master has a higher priority1 value than examplePtpUniNegMaster().

*******************************************************************************/
zlStatusE examplePtpUniNegMaster2(void)
{
   zlStatusE status = ZL303XX_OK;

   const char *srcIpv4Addr = "10.0.0.106";
   const char *destIpv4Addr = "";

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStructInit(1, 1, 0, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
       exampleSetSrcDestAddressesToStructs(srcIpv4Addr, destIpv4Addr);  /* Ignore error return for dest addr */
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = 13;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.priority1 = 150;

      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_TRUE;

      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}

/* examplePtpUniNegMaster3 */
/**
   An example of how to configure a PTP master that uses unicast negotiation.
   This master has a higher priority1 value than examplePtpUniNegMaster2().

*******************************************************************************/
zlStatusE examplePtpUniNegMaster3(void)
{
   zlStatusE status = ZL303XX_OK;

   const char *srcIpv4Addr = "10.0.0.107";
   const char *destIpv4Addr = "";

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStructInit(1, 1, 0, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
       exampleSetSrcDestAddressesToStructs(srcIpv4Addr, destIpv4Addr);  /* Ignore error return for dest addr */
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = 13;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.priority1 = 160;

      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_TRUE;

      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}

/* examplePtpUniNegMaster4 */
/**
   An example of how to configure a PTP master that uses unicast negotiation.
   This master has a higher priority1 value than examplePtpUniNegMaster3().

*******************************************************************************/
zlStatusE examplePtpUniNegMaster4(void)
{
   zlStatusE status = ZL303XX_OK;

   const char *srcIpv4Addr = "10.0.0.108";
   const char *destIpv4Addr = "";

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStructInit(1, 1, 0, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
       exampleSetSrcDestAddressesToStructs(srcIpv4Addr, destIpv4Addr);  /* Ignore error return for dest addr */
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = 13;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.priority1 = 170;

      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_TRUE;

      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}

/* examplePtpUniNegSlave_4Streams */
/**
   An example of how to configure a PTP slave that uses unicast negotiation.
   This slave will lock to 1 of 4 potential masters.

*******************************************************************************/
zlStatusE examplePtpUniNegSlave_4Streams(void)
{
   zlStatusE status = ZL303XX_OK;

   const char *srcIpv4Addr = "10.0.0.210";
   const char *destIpv4Addr[4] = {"10.0.0.105", "10.0.0.106",
                                  "10.0.0.107", "10.0.0.108"};

#if defined APR_INCLUDED
   if (status == ZL303XX_OK)
   {
      /* Setup global configuration options */
      status |= zl303xx_AprSetPktRate(64, ZL303XX_TRUE);
      status |= zl303xx_AprSetPktRate(16, ZL303XX_FALSE);

      status |= exampleAprSetConfigParameters(ACI_DEFAULT );
   }
#endif

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStructInit(1, 1, 4, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
      status = examplePtpLocalIpv4Set(srcIpv4Addr, ZL303XX_FALSE, &zlExampleApp.port[0].ptp);
   }

   if (status == ZL303XX_OK)
   {
      Uint32T count;

      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_TRUE;

      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_TRUE;

      for (count = 0; count < 4; count++)
      {
         zlExampleApp.stream[count].ptp.config.mode = ZL303XX_PTP_STREAM_SLAVE_ONLY;
         zlExampleApp.stream[count].ptp.config.unicast = ZL303XX_TRUE;

         status |= examplePtpDestIpv4Set(destIpv4Addr[count], ZL303XX_TRUE, &zlExampleApp.stream[count].ptp);
      }
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}

/* examplePtpMultiBC */
/**
   An example of how to configure a PTP boundary clock that uses multicast
   transport. 2 PTP ports are created, each with a single stream.



 examplePtpMultiMaster()       examplePtpMultiBC()             examplePtpMultiBCSlave()

 +-------------+               +-----------------------+                +-------------+
 |  Multicast  |  Timestamped  |    Boundary Clock     |  Timestamped   |  Multicast  |
 |   Master    |<------------->|-----------------------|<-------------->|    Slave    |
 | 10.0.0.100  |    Packets    | Multicast | Multicast |    Packets     | 10.0.0.210  |
 | Class = 10  |  (Multicast)  |   Slave   |   Master  |  (Multicast)   | Class = 248 |
 +-------------+    ( .129 )   |10.0.0.205 | 10.0.0.206|    ( .130 )    +-------------+
                               |     Class = 187       |
                               +-----------+-----------+
                                  Port 1       Port 2


*******************************************************************************/
zlStatusE examplePtpMultiBC(void)
{
   zlStatusE status = ZL303XX_OK;

   const char *srcIpv4Addr[2]  = {"10.0.0.205", "10.0.0.206"};
   const char *destIpv4Addr[2] = {DEFAULT_PTP_DOMAIN_ADDRESS_IPV4,
                                  ALTERNATE_PTP_DOMAIN1_ADDRESS_IPV4};

#if defined APR_INCLUDED
   if (status == ZL303XX_OK)
   {
      /* Setup global configuration options */
      status |= zl303xx_AprSetPktRate(64, ZL303XX_TRUE);
      status |= zl303xx_AprSetPktRate(16, ZL303XX_FALSE);

      status |= exampleAprSetConfigParameters(ACI_BC_FULL_ON_PATH_PHASE);
   }
#endif

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStructInit(1, 2, 2, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
      Uint32T count;

      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = 187;

      for (count = 0; count < 2; count++)
      {
         status |= examplePtpLocalIpv4Set(srcIpv4Addr[count], ZL303XX_TRUE,
                                          &zlExampleApp.port[count].ptp);
      }

      for (count = 0; count < 2; count++)
      {
         zlExampleApp.stream[count].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
         zlExampleApp.stream[count].ptp.config.unicast = ZL303XX_FALSE;
         zlExampleApp.stream[count].ptp.config.portHandle = count;  /* One stream per port. */

         status |= examplePtpDestIpv4Set(destIpv4Addr[count], ZL303XX_TRUE,
                                         &zlExampleApp.stream[count].ptp);
      }
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStart(&zlExampleApp);
   }


   return status;
}

/* examplePtpMultiBCSlave */
/**
   An example of how to configure a PTP node that uses multicast transport and
   the Best Master Clock Algorithm. This example is designed to run with
   examplePtpMultiBC().

*******************************************************************************/
zlStatusE examplePtpMultiBCSlave(void)
{
   zlStatusE status = ZL303XX_OK;

   const char *srcIpv4Addr = "10.0.0.210";
   const char *destIpv4Addr = ALTERNATE_PTP_DOMAIN1_ADDRESS_IPV4;

#if defined APR_INCLUDED
   if (status == ZL303XX_OK)
   {
      /* Setup global configuration options */
      status |= zl303xx_AprSetPktRate(64, ZL303XX_TRUE);
      status |= zl303xx_AprSetPktRate(16, ZL303XX_FALSE);

      status |= exampleAprSetConfigParameters(ACI_BC_FULL_ON_PATH_PHASE);
   }
#endif

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStructInit(1, 1, 1, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
      status = examplePtpLocalIpv4Set(srcIpv4Addr, ZL303XX_FALSE, &zlExampleApp.port[0].ptp) |
               examplePtpDestIpv4Set(destIpv4Addr, ZL303XX_FALSE, &zlExampleApp.stream[0].ptp);
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = 248;

      zlExampleApp.stream[0].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
      zlExampleApp.stream[0].ptp.config.unicast = ZL303XX_FALSE;
   }

   if(status == ZL303XX_OK)
   {
      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}
#endif  /* ZLS30390_INCLUDED */


/* Get PTP Status */
zlStatusE getPtpStatus(void)
{
	unsigned int port = 0;						/* #12560 初期化対応 */
	port = zl303xx_GetExampleInterface();
	
	zl303xx_ClockStreamS* pPtpStream = Zl303xx_PtpStreamArray[port];
	
    const char *streamModeStr =  zl303xx_PtpStreamModeToStr(pPtpStream->config.mode);
    const char *streamStateStr = zl303xx_PtpStreamSubStateToStr(pPtpStream->streamSubstate);

	printf("getPtpStatus: Stream (%d): state %d(%s): mode %d(%s) \n",
         pPtpStream->clockStreamHandle,
         pPtpStream->streamSubstate, streamStateStr,
         pPtpStream->config.mode, streamModeStr);
	return ZL303XX_OK;
}

void getPtpParameter(void)
{
	union udata {
		Uint64S		ldata;
		Uint32T		idata[2];
	} data;

	Uint32T					port = zl303xx_GetExampleInterface();
	zl303xx_ClockStreamS*	pPtpStream = Zl303xx_PtpStreamArray[port] ;


	data.idata[0] = 0 ;
	data.idata[1] = 0 ;
	printf("******************* \n");
	printf("*****  clock  ***** \n");
	printf("******************* \n");
	printf("*** PTP Clock Create Parameters (Task & Misc) *** \n");
	printf("  taskName                    : %s \n",   zlExampleApp.clock[TARGET_DPLL].ptp.config.taskName);
	printf("  taskPriority                : %08x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.taskPriority);
	printf("  taskStackSize               : %08x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.taskStackSize);
	printf("  msgQLength                  : %08x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.msgQLength);
	printf("  clockType                   : %04x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.clockType);
	printf("  requestedHandle             : %d \n",   zlExampleApp.clock[TARGET_DPLL].ptp.config.requestedHandle);
	printf("  maxForeignRecords           : %08x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.maxForeignRecords);
	printf("  updateTrigger               : %s \n",   getPtpParameter_strUpdateTrigger((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.bmca.updateTrigger));	/* enum */
	printf("  updateEventSec              : %08x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.bmca.updateEventSec);
	printf("  autoUpdateStreamStates      : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.autoUpdateStreamStates));	/* bool */

	printf("\n");
	printf("*** PTP Clock Create Parameters (Local defaultDS) *** \n");
	printf("  defaultDS \n");
	printf("    => twoStepFlag            : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.twoStepFlag));	/* bool */
	printf("    => clockIdentity          : %02x%02x%02x%02x%02x%02x%02x%02x \n",
						zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockIdentity[0],
						zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockIdentity[1],
						zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockIdentity[2],
						zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockIdentity[3],
						zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockIdentity[4],
						zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockIdentity[5],
						zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockIdentity[6],
						zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockIdentity[7] );
	printf("    => numberPorts            : %08x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.numberPorts);
	printf("    => Quality.Class          : %02x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass);
	printf("    => Quality.Accuracy       : %s \n",   getPtpParameter_strClockAccuracy((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockAccuracy));	/* enum */
	printf("    => Quality.Variance       : %04x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.offsetScaledLogVariance);
	printf("    => priority1              : %02x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.priority1);
	printf("    => priority2              : %02x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.priority2);
	printf("    => domainNumber           : %02x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.domainNumber);
	printf("    => slaveOnly              : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly));	/* bool */
	printf("  optionalDefaultDS \n");
	printf("    => maxStepsRemoved        : %04x\n\n",  zlExampleApp.clock[TARGET_DPLL].ptp.config.optionalDefaultDS.maxStepsRemoved);

	printf("*** PTP Clock Create Parameters (Local timePropertiesDS) *** \n");
	printf("  localTimeProperties \n");
	printf("    => currentUtcOffset       : %04x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.localTimeProperties.currentUtcOffset);
	printf("    => currentUtcOffsetValid  : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.localTimeProperties.currentUtcOffsetValid));	/* bool */
	printf("    => leap59                 : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.localTimeProperties.leap59));	/* bool */
	printf("    => leap61                 : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.localTimeProperties.leap61));	/* bool */
	printf("    => timeTraceable          : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.localTimeProperties.timeTraceable));	/* bool */
	printf("    => frequencyTraceable     : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.localTimeProperties.frequencyTraceable));	/* bool */
	printf("    => ptpTimescale           : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.localTimeProperties.ptpTimescale));	/* bool */
	printf("    => timeSource             : %s \n",   getPtpParameter_strTimeSource((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.localTimeProperties.timeSource));	/* enum */
	printf("    => sync_Uncertain         : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.localTimeProperties.synchronizationUncertain));	/* bool */

	printf("\n");
	printf("*** PTP Clock Create Parameters (Profile & BMCA) *** \n");
	printf("  profile                     : %s \n",   getPtpParameter_strProfile((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.profile));	/* enum */
	printf("  profileStrict               : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.profileStrict));	/* bool */
	printf("  power.clockId               : %04x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.clockId);
	printf("  power.localTimeInaccuracy   : %08x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.localTimeInaccuracy);
	printf("  power.networkTimeInaccuracy : %08x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.networkTimeInaccuracy);
	printf("  usePreviousClass            : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.uncalibrated.usePreviousClass));	/* bool */
	printf("  egressClassUpdateTrigger    : %s \n",   getPtpParameter_strEgressQlUpdate((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.egressClassUpdateTrigger));	/* enum */
	printf("  pathTrace.enabled           : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.pathTrace.enabled));	/* bool */
	printf("  syncUncertain.enabled       : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.synchronizationUncertain.enabled));	/* bool */
	printf("  revertiveEn                 : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.bmca.revertiveEn));	/* bool */

	printf("\n");
	printf("*** Clock Settings related to ITU-T G.8275.x *** \n");
	printf("  zl303xx_PtpG8275ClockConfigS \n");
	printf("    => bypassEnabled          : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.g8275p1.bypassEnabled));	/* bool */
	printf("    => localPriority          : %02x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.g8275p1.localPriority);
	printf("    => classEvalMethod        : %s \n",   getPtpParameter_strEvalMethod((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.g8275p1.classEvalMethod));	/* enum */
	printf("    => equipmentClass         : %02x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.g8275p1.equipmentClass);
	printf("    => holdoverSupported      : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.g8275p1.holdoverSupported));	/* bool */

	printf("\n");
	printf("*** Clock Settings related to ITU-T G.8265.1 *** \n");
	printf("  zl303xx_PtpTelecomClockCreateS \n");
	printf("    => waitToRestoreSec       : %08x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.telecom.waitToRestoreSec);
	printf("    => qlHoldOffSec           : %08x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.telecom.qlHoldOffSec);
	printf("    => evtSquelchEn           : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.telecom.evtSquelchEn));	/* bool */

	printf("\n");
	printf("*** Clock Settings related to IEEE-C37.238 *** \n");
	printf("  zl303xx_PtpC37p238ClockConfigS \n");
	printf("    => clockId                : %04x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.clockId);
	printf("    => localTimeInaccuracy    : %04x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.localTimeInaccuracy);
	printf("    => networkTimeInaccuracy  : %04x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.networkTimeInaccuracy);
	printf("    => localTimeZoneInfo      : %s \n",   zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.localTimeZoneInfo.dstTimeZoneName);

	printf("\n");
	printf("*** Clock Settings related to IEEE-C37.238 zl303xx_PtpC37p238LocalTimeZoneS *** \n");
	printf("  zl303xx_PtpC37p238LocalTimeZoneS \n");
	printf("    => keyField               : %02x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.localTimeZoneInfo.keyField);
	printf("    => includesDst            : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.localTimeZoneInfo.includesDst));	/* bool */
	printf("    => stdTimeOffsetToUtc     : %08x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.localTimeZoneInfo.stdTimeOffsetToUtc);
	printf("    => dstTimeOffsetToUtc     : %08x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.localTimeZoneInfo.dstTimeOffsetToUtc);
	printf("    => stdTimeStartsAt \n");
	printf("         month : %02d / week:%02d / day:%02d / hour:%02d \n",
						zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.localTimeZoneInfo.stdTimeStartsAt.month,
						zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.localTimeZoneInfo.stdTimeStartsAt.week,
						zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.localTimeZoneInfo.stdTimeStartsAt.day,
						zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.localTimeZoneInfo.stdTimeStartsAt.hour);
	printf("    => dstTimeStartsAt \n");
	printf("         month : %02d / week:%02d / day:%02d / hour:%02d \n",
						zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.localTimeZoneInfo.dstTimeStartsAt.month,
						zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.localTimeZoneInfo.dstTimeStartsAt.week,
						zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.localTimeZoneInfo.dstTimeStartsAt.day,
						zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.localTimeZoneInfo.dstTimeStartsAt.hour);
	printf("    => stdTimeZoneName        : %s \n",   zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.localTimeZoneInfo.stdTimeZoneName);
	printf("    => dstTimeZoneName        : %s \n",   zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.power.localTimeZoneInfo.dstTimeZoneName);

	printf("\n");
	printf("*** PTP Clock Create Parameters (Unicast Negotiation Capacity) *** \n");
	printf("  unicastNegotiation \n");
	printf("    => maxAnnounceCount       : %08x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.unicastNegotiation.maxAnnounceCount);
	printf("    => maxSyncCount           : %08x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.unicastNegotiation.maxSyncCount);
	printf("    => maxDelayRespCount      : %08x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.unicastNegotiation.maxDelayRespCount);
	printf("    => maxPdelayRespCount     : %08x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.unicastNegotiation.maxPdelayRespCount);
	printf("    => totalPpsMax            : %08x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.unicastNegotiation.totalPpsMax);

	printf("\n");
	printf("*** PTP Clock Create Parameters (User Overrides) *** \n");
	printf("  override->enabled \n");
	printf("    => [CLASS]                : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.enabled[0]));	/* bool */
	printf("    => [ACCURACY]             : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.enabled[1]));	/* bool */
	printf("    => [OFFSET_SCALED_LOG_VAR]: %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.enabled[2]));	/* bool */
	printf("    => [PRIORITY1]            : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.enabled[3]));	/* bool */
	printf("    => [PRIORITY2]            : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.enabled[4]));	/* bool */
	printf("    => [DOMAIN]               : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.enabled[5]));	/* bool */
	printf("    => [CUR_UTC_OFFSET]       : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.enabled[6]));	/* bool */
	printf("    => [CUR_UTC_OFFSET_VALID] : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.enabled[7]));	/* bool */
	printf("    => [TIME_TRACEABLE]       : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.enabled[8]));	/* bool */
	printf("    => [FREQUENCY_TRACE]      : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.enabled[9]));	/* bool */
	printf("    => [SYNC_UNCERTAIN]       : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.enabled[10]));	/* bool */
	printf("    => [TIME_SOURCE]          : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.enabled[11]));	/* bool */
	printf("    => [STEPS_REMOVED]        : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.enabled[12]));	/* bool */
	printf("  override->defaultDS \n");
	printf("    => twoStepFlag            : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.defaultDS.twoStepFlag));	/* bool */
	printf("    => clockIdentity          : %02x%02x%02x%02x%02x%02x%02x%02x \n",
						zlExampleApp.clock[TARGET_DPLL].ptp.config.override.defaultDS.clockIdentity[0],
						zlExampleApp.clock[TARGET_DPLL].ptp.config.override.defaultDS.clockIdentity[1],
						zlExampleApp.clock[TARGET_DPLL].ptp.config.override.defaultDS.clockIdentity[2],
						zlExampleApp.clock[TARGET_DPLL].ptp.config.override.defaultDS.clockIdentity[3],
						zlExampleApp.clock[TARGET_DPLL].ptp.config.override.defaultDS.clockIdentity[4],
						zlExampleApp.clock[TARGET_DPLL].ptp.config.override.defaultDS.clockIdentity[5],
						zlExampleApp.clock[TARGET_DPLL].ptp.config.override.defaultDS.clockIdentity[6],
						zlExampleApp.clock[TARGET_DPLL].ptp.config.override.defaultDS.clockIdentity[7]);
	printf("    => numberPorts            : %04x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.override.defaultDS.numberPorts);
	printf("    => Quality.Class          : %02x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.override.defaultDS.clockQuality.clockClass);
	printf("    => Quality.Accuracy       : %s \n",   getPtpParameter_strClockAccuracy((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.defaultDS.clockQuality.clockAccuracy));	/* enum */
	printf("    => Quality.Variance       : %04x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.override.defaultDS.clockQuality.offsetScaledLogVariance);
	printf("    => priority1              : %02x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.override.defaultDS.priority1);
	printf("    => priority2              : %02x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.override.defaultDS.priority2);
	printf("    => domainNumber           : %02x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.override.defaultDS.domainNumber);
	printf("    => slaveOnly              : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.defaultDS.slaveOnly));	/* bool */
	printf("  override->timePropertiesDS \n");
	printf("    => currentUtcOffset       : %04x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.override.timePropertiesDS.currentUtcOffset);
	printf("    => currentUtcOffsetValid  : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.timePropertiesDS.currentUtcOffsetValid));	/* bool */
	printf("    => leap59                 : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.timePropertiesDS.leap59));	/* bool */
	printf("    => leap61                 : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.timePropertiesDS.leap61));	/* bool */
	printf("    => timeTraceable          : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.timePropertiesDS.timeTraceable));	/* bool */
	printf("    => frequencyTraceable     : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.timePropertiesDS.frequencyTraceable));	/* bool */
	printf("    => ptpTimescale           : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.timePropertiesDS.ptpTimescale));	/* bool */
	printf("    => timeSource             : %s \n",   getPtpParameter_strTimeSource((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.timePropertiesDS.timeSource));	/* enum */
	printf("    => syncUncertain          : %s \n",   getPtpParameter_strBool((int)zlExampleApp.clock[TARGET_DPLL].ptp.config.override.timePropertiesDS.synchronizationUncertain));	/* bool */
	printf("  override->currentDS \n");
	printf("    => stepsRemoved           : %04x \n", zlExampleApp.clock[TARGET_DPLL].ptp.config.override.currentDS.stepsRemoved);
	printf("\n");

	printf("\n");
	printf("******************* \n");
	printf("*****  port   ***** \n");
	printf("******************* \n");
	printf("*** PTP Port Create Parameters (General & Misc) *** \n");
	printf("  clockHandle                 : %08x \n", zlExampleApp.port[0].ptp.config.clockHandle);
	printf("  portNumber                  : %04x \n", zlExampleApp.port[0].ptp.config.portNumber);
	printf("  ptpVersion                  : %04x \n", zlExampleApp.port[0].ptp.config.ptpVersion);
	printf("  localAddr.networkProtocol   : %s \n",   getPtpParameter_strNetworkProtocol((int)zlExampleApp.port[0].ptp.config.localAddr.networkProtocol));	/* enum */
	printf("  localAddr.addressLength     : %04x \n", zlExampleApp.port[0].ptp.config.localAddr.addressLength);
	printf("  localAddr.addressField      : %02x-%02x-%02x-%02x-%02x-%02x \n", 
						zlExampleApp.port[0].ptp.config.localAddr.addressField[0],
						zlExampleApp.port[0].ptp.config.localAddr.addressField[1],
						zlExampleApp.port[0].ptp.config.localAddr.addressField[2],
						zlExampleApp.port[0].ptp.config.localAddr.addressField[3],
						zlExampleApp.port[0].ptp.config.localAddr.addressField[4],
						zlExampleApp.port[0].ptp.config.localAddr.addressField[5]);

	printf("\n");
	printf("*** PTP Port Create Parameters (Profile-Specific) *** \n");
	printf("  delayMechanism              : %s \n",   getPtpParameter_strDelayMechanism((int)zlExampleApp.port[0].ptp.config.delayMechanism));	/* enum */
	printf("  pdRespTxMethod              : %s \n",   getPtpParameter_strPdelayRespMethod((int)zlExampleApp.port[0].ptp.config.pdRespTxMethod));	/* enum */
	printf("  logMinPdelayReqInterval     : %02x \n", zlExampleApp.port[0].ptp.config.logMinPdelayReqInterval);
	printf("  masterOnly                  : %s \n",   getPtpParameter_strBool((int)zlExampleApp.port[0].ptp.config.masterOnly));	/* bool */
	printf("  alternateMaster             : %s \n",   getPtpParameter_strBool((int)zlExampleApp.port[0].ptp.config.alternateMaster));	/* bool */

	printf("\n");
	printf("*** Port Settings related to ITU-T G.8275.x *** \n");
	printf("  profileCfg->g8275p1 \n");
	printf("    => localPriority          : %02x \n", zlExampleApp.port[0].ptp.config.profileCfg.g8275p1.localPriority);
	printf("    => notSlave               : %s \n",   getPtpParameter_strBool((int)zlExampleApp.port[0].ptp.config.profileCfg.g8275p1.notSlave));	/* bool */

	printf("\n");
	printf("*** Settings related to Interface Rate TLV of the G.8275.2 *** \n");
	printf("  profileCfg->g8275p1 \n");
	data.ldata = zlExampleApp.port[0].ptp.config.profileCfg.g8275p1.tsIfRate.interfaceBitPeriod;
	printf("    => interfaceBitPeriod     : %08x%08x \n", data.idata[0], data.idata[1]);
	printf("    => numBitsBeforeTimestamp : %04x \n", zlExampleApp.port[0].ptp.config.profileCfg.g8275p1.tsIfRate.numberBitsBeforeTimestamp);
	printf("    => numBitsAfterTimestamp  : %04x \n", zlExampleApp.port[0].ptp.config.profileCfg.g8275p1.tsIfRate.numberBitsAfterTimestamp);

	printf("\n");
	printf("*** Port Settings related IEEE C37.238 *** \n");
	printf("  profileCfg->power \n");
	printf("    => empty                  : %08x \n", zlExampleApp.port[0].ptp.config.profileCfg.power.empty);

	printf("\n");
	printf("*** PTP Port Create Parameters (Acceptable Master) *** \n");
	printf("  acceptableMasterTableEnabled: %d \n",   zlExampleApp.port[0].ptp.config.acceptableMasterTableEnabled);
	printf("  acceptableSlaveTableEnabled : %d \n",   zlExampleApp.port[0].ptp.config.acceptableSlaveTableEnabled);
	printf("  acceptableMasterTableLength : %04x \n", zlExampleApp.port[0].ptp.config.acceptableMasterTableLength);

	printf("\n");
	printf("*** PTP Port Create Parameters (Unicast Negotiation) *** \n");
	printf("  unicastNegotiation \n");
	printf("    => enabled                : %s \n",   getPtpParameter_strBool((int)zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled));	/* bool */
	printf("    => stackedTlvEn           : %s \n",   getPtpParameter_strBool((int)zlExampleApp.port[0].ptp.config.unicastNegotiation.stackedTlvEn));	/* bool */
	printf("    => maxAnnounceRate        : %02x \n", zlExampleApp.port[0].ptp.config.unicastNegotiation.maxAnnounceRate);
	printf("    => maxAnnounceGrantSecs   : %08x \n", zlExampleApp.port[0].ptp.config.unicastNegotiation.maxAnnounceGrantSecs);
	printf("    => maxAnnounceCount       : %08x \n", zlExampleApp.port[0].ptp.config.unicastNegotiation.maxAnnounceCount);
	printf("    => maxSyncRate            : %02x \n", zlExampleApp.port[0].ptp.config.unicastNegotiation.maxSyncRate);
	printf("    => maxSyncGrantSecs       : %08x \n", zlExampleApp.port[0].ptp.config.unicastNegotiation.maxSyncGrantSecs);
	printf("    => maxSyncCount           : %08x \n", zlExampleApp.port[0].ptp.config.unicastNegotiation.maxSyncCount);
	printf("    => maxDelayRespRate       : %02x \n", zlExampleApp.port[0].ptp.config.unicastNegotiation.maxDelayRespRate);
	printf("    => maxDelayRespGrantSecs  : %08x \n", zlExampleApp.port[0].ptp.config.unicastNegotiation.maxDelayRespGrantSecs);
	printf("    => maxDelayRespCount      : %08x \n", zlExampleApp.port[0].ptp.config.unicastNegotiation.maxDelayRespCount);
	printf("    => maxPdelayRespRate      : %02x \n", zlExampleApp.port[0].ptp.config.unicastNegotiation.maxPdelayRespRate);
	printf("    => maxPdelayRespGrantSecs : %08x \n", zlExampleApp.port[0].ptp.config.unicastNegotiation.maxPdelayRespGrantSecs);
	printf("    => maxPdelayRespCount     : %08x \n", zlExampleApp.port[0].ptp.config.unicastNegotiation.maxPdelayRespCount);
	printf("    => totalPpsMax            : %08x \n", zlExampleApp.port[0].ptp.config.unicastNegotiation.totalPpsMax);
	printf("    => denyServiceRequests    : %s \n",   getPtpParameter_strBool((int)zlExampleApp.port[0].ptp.config.unicastNegotiation.denyServiceRequests));	/* bool */
	printf("    => grantTableLength       : %04x \n", zlExampleApp.port[0].ptp.config.unicastNegotiation.grantTableLength);
	printf("    => maxTableSize           : %04x \n", zlExampleApp.port[0].ptp.config.unicastMasterTable.maxTableSize);
	printf("    => logQueryInterval       : %02x \n", zlExampleApp.port[0].ptp.config.unicastMasterTable.logQueryInterval);

	printf("\n");
	printf("******************* \n");
	printf("***** stream  ***** \n");
	printf("******************* \n");
	printf("*** PTP Stream Create Parameters (General & Misc) *** \n");
	printf("  portHandle                  : %08x \n", pPtpStream->config.portHandle);
	printf("  destAddr.networkProtocol    : %s \n",   getPtpParameter_strNetworkProtocol((int)pPtpStream->config.destAddr.networkProtocol));	/* enum */
	printf("  destAddr.addressLength      : %04x \n", pPtpStream->config.destAddr.addressLength);
	printf("  destAddr.addressField       : %02x-%02x-%02x-%02x-%02x-%02x \n", 
						pPtpStream->config.destAddr.addressField[0],
						pPtpStream->config.destAddr.addressField[1],
						pPtpStream->config.destAddr.addressField[2],
						pPtpStream->config.destAddr.addressField[3],
						pPtpStream->config.destAddr.addressField[4],
						pPtpStream->config.destAddr.addressField[5]);
	printf("  requestedHandle             : %08x \n", pPtpStream->config.requestedHandle);
	printf("  numTsRecords                : %04x \n", pPtpStream->config.numTsRecords);
	printf("  numTwoStepRecords           : %04x \n", pPtpStream->config.numTwoStepRecords);
	printf("  createFlags                 : %08x \n", pPtpStream->config.createFlags);
	printf("  delayReqTimeSetRequired     : %d \n",   pPtpStream->config.delayReqTimeSetRequired);	/* bool	*/
	printf("  nextTxSequenceId            : %04x \n", pPtpStream->config.nextTxSequenceId);
	printf("  delayAsymmetry \n");
	printf("    => ingress                : %08x.%08x \n", 
						pPtpStream->config.delayAsymmetry.ingress.scaledNanoseconds.hi, 
						pPtpStream->config.delayAsymmetry.ingress.scaledNanoseconds.lo);
	printf("    => egress                 : %08x.%08x \n", 
						pPtpStream->config.delayAsymmetry.egress.scaledNanoseconds.hi, 
						pPtpStream->config.delayAsymmetry.egress.scaledNanoseconds.lo);


	printf("\n");
	printf("*** PTP Stream Create Parameters (Message Rates, Timeouts & Intervals) *** \n");
	printf("  logAnnounceInterval         : %02x \n", pPtpStream->config.logAnnounceInterval);
	printf("  announceReceiptTimeout      : %02x \n", pPtpStream->config.announceReceiptTimeout);
	printf("  logSyncInterval             : %02x \n", pPtpStream->config.logSyncInterval);
	printf("  logMinDelayReqInterval      : %02x \n", pPtpStream->config.logMinDelayReqInterval);
	printf("  logMinPdelayReqInterval     : %02x \n", pPtpStream->config.logMinPdelayReqInterval);

	printf("\n");
	printf("*** PTP Stream Create Parameters (Profile-Specific) *** \n");
	printf("  mode                        : %s \n",   getPtpParameter_strStreamMode((int)pPtpStream->config.mode));	/* enum */
	printf("  padTlvLen : SYNC            : %04x \n", pPtpStream->config.padTlvLen[0]);
	printf("  padTlvLen : DELAY_REQ       : %04x \n", pPtpStream->config.padTlvLen[1]);
	printf("  padTlvLen : DELAY_REQ       : %04x \n", pPtpStream->config.padTlvLen[2]);
	printf("  padTlvLen : DELAY_RESP      : %04x \n", pPtpStream->config.padTlvLen[3]);
	printf("  padTlvLen : FOLLOWUP        : %04x \n", pPtpStream->config.padTlvLen[8]);
	printf("  padTlvLen : DELAY_RESP      : %04x \n", pPtpStream->config.padTlvLen[9]);
	printf("  padTlvLen : DELAY_FOLLOWUP  : %04x \n", pPtpStream->config.padTlvLen[10]);
	printf("  padTlvLen : ANNOUNCE        : %04x \n", pPtpStream->config.padTlvLen[11]);
	printf("  padTlvLen : SIGNALING       : %04x \n", pPtpStream->config.padTlvLen[12]);
	printf("  padTlvLen : MANAGEMENT      : %04x \n", pPtpStream->config.padTlvLen[13]);
	printf("  maxClockClass               : %02x \n", pPtpStream->config.maxClockClass);
	printf("  uncalibrated.anncIntervals  : %02x \n", pPtpStream->config.uncalibrated.anncIntervals);
	printf("  uncalibrated.lockRequired   : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.uncalibrated.lockRequired));	/* bool */
	printf("  keepAliveSec                : %08x \n", pPtpStream->config.keepAliveSec);

	printf("\n");
	printf("*** Stream Settings related IEEE C37.238 *** \n");
	printf("  g8275p1 \n");
	printf("    => localPriority          : %02x \n", pPtpStream->config.profileCfg.g8275p1.localPriority);
	printf("    => notSlave               : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.profileCfg.g8275p1.notSlave));	/* bool */

	printf("\n");
	printf("*** Stream Settings related IEEE C37.238 *** \n");
	printf("  power \n");
	printf("    => profileTlvRequired     : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.profileCfg.power.profileTlvRequired));	/* bool */
	printf("    => profileTlvAppend       : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.profileCfg.power.profileTlvAppend));	/* bool */
	printf("    => profileTlvVersion      : %08x \n", pPtpStream->config.profileCfg.power.profileTlvVersion);
	printf("    => TimeOffsetTlvRequired  : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.profileCfg.power.altTimeOffsetTlvRequired));	/* bool */
	printf("    => TimeOffsetTlvAppend    : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.profileCfg.power.altTimeOffsetTlvAppend));	/* bool */
	printf("    => localTimeInaccuracyMax : %08x \n", pPtpStream->config.profileCfg.power.localTimeInaccuracyMax);
	printf("    => nwTimeInaccuracyMax    : %08x \n", pPtpStream->config.profileCfg.power.networkTimeInaccuracyMax);

	printf("\n");
	printf("*** PTP Stream Create Parameters (Unicast Negotiation) *** \n");
	printf("  unicast                     : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.unicast));	/* bool */
	printf("  unicastNegotiationDuration  : %08x \n", pPtpStream->config.unicastNegotiationDuration);
	printf("  unicastMonitorTiming        : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.unicastMonitorTiming));	/* bool */
	printf("  unicastNegotiationRenew     : %08x \n", pPtpStream->config.unicastNegotiationRenew);
	printf("  unicastNegotiation->bestEffort \n");
	printf("    => enabled[SYNC]          : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.unicastNegotiation.bestEffort[0].enabled));	/* bool */
	printf("    => enabled[DELAY_REQ]     : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.unicastNegotiation.bestEffort[1].enabled));	/* bool */
	printf("    => enabled[PEER_DELAY_REQ]: %s \n",   getPtpParameter_strBool((int)pPtpStream->config.unicastNegotiation.bestEffort[2].enabled));	/* bool */
	printf("    => enabled[PEER_DELAY_RSP]: %s \n",   getPtpParameter_strBool((int)pPtpStream->config.unicastNegotiation.bestEffort[3].enabled));	/* bool */
	printf("    => enabled[FOLLOWUP]      : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.unicastNegotiation.bestEffort[8].enabled));	/* bool */
	printf("    => enabled[DELAY_RESP]    : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.unicastNegotiation.bestEffort[9].enabled));	/* bool */
	printf("    => enabled[DELAY_FOLLOWUP]: %s \n",   getPtpParameter_strBool((int)pPtpStream->config.unicastNegotiation.bestEffort[10].enabled));	/* bool */
	printf("    => enabled[ANNOUNCE]      : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.unicastNegotiation.bestEffort[11].enabled));	/* bool */
	printf("    => enabled[SIGNALING]     : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.unicastNegotiation.bestEffort[12].enabled));	/* bool */
	printf("    => enabled[MANAGEMENT]    : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.unicastNegotiation.bestEffort[13].enabled));	/* bool */
	printf("    => intervalMax            : %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n", 
						pPtpStream->config.unicastNegotiation.bestEffort[0].intervalMax,
						pPtpStream->config.unicastNegotiation.bestEffort[1].intervalMax,
						pPtpStream->config.unicastNegotiation.bestEffort[2].intervalMax,
						pPtpStream->config.unicastNegotiation.bestEffort[3].intervalMax,
						pPtpStream->config.unicastNegotiation.bestEffort[4].intervalMax,
						pPtpStream->config.unicastNegotiation.bestEffort[5].intervalMax,
						pPtpStream->config.unicastNegotiation.bestEffort[6].intervalMax,
						pPtpStream->config.unicastNegotiation.bestEffort[7].intervalMax,
						pPtpStream->config.unicastNegotiation.bestEffort[8].intervalMax,
						pPtpStream->config.unicastNegotiation.bestEffort[9].intervalMax,
						pPtpStream->config.unicastNegotiation.bestEffort[10].intervalMax,
						pPtpStream->config.unicastNegotiation.bestEffort[11].intervalMax,
						pPtpStream->config.unicastNegotiation.bestEffort[12].intervalMax,
						pPtpStream->config.unicastNegotiation.bestEffort[13].intervalMax,
						pPtpStream->config.unicastNegotiation.bestEffort[14].intervalMax);

	printf("\n");
	printf("*** PTP Stream Create Parameters (User Overrides) *** \n");
	printf("  override \n");
	printf("    => transportSpecific      : %02x \n", pPtpStream->config.override.ptpHeader.transportSpecific);
	printf("  override->anncOvrdEn \n");
	printf("    => [TRANSPORT]            : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.anncOvrdEn[0]));	/* bool */
	printf("    => [DOMAIN]               : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.anncOvrdEn[1]));	/* bool */
	printf("    => [CUR_UTC_OFFSET_VALID] : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.anncOvrdEn[2]));	/* bool */
	printf("    => [TIME_TRACEABLE]       : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.anncOvrdEn[3]));	/* bool */
	printf("    => [FREQUENCY_TRACEABLE]  : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.anncOvrdEn[4]));	/* bool */
	printf("    => [SYNCHRO_UNCERTAIN]    : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.anncOvrdEn[5]));	/* bool */
	printf("    => [CORRECTION]           : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.anncOvrdEn[6]));	/* bool */
	printf("    => [SRC_PORT_ID]          : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.anncOvrdEn[7]));	/* bool */
	printf("    => [ORIGIN_TIMESTAMP]     : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.anncOvrdEn[8]));	/* bool */
	printf("    => [CURRENT_UTC_OFFSET]   : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.anncOvrdEn[9]));	/* bool */
	printf("    => [GM_PRIORITY1]         : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.anncOvrdEn[10]));	/* bool */
	printf("    => [GM_CLASS]             : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.anncOvrdEn[11]));	/* bool */
	printf("    => [GM_ACCURACY]          : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.anncOvrdEn[12]));	/* bool */
	printf("    => [GM_VARIANCE]          : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.anncOvrdEn[13]));	/* bool */
	printf("    => [GM_PRIORITY2]         : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.anncOvrdEn[14]));	/* bool */
	printf("    => [GM_IDENTITY]          : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.anncOvrdEn[15]));	/* bool */
	printf("    => [STEPS_REMOVED]        : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.anncOvrdEn[16]));	/* bool */
	printf("    => [TIME_SOURCE]          : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.anncOvrdEn[17]));	/* bool */
	printf("  override->ptpHeader \n");
	printf("    => transportSpecific      : %02x \n", pPtpStream->config.override.ptpHeader.transportSpecific);
	printf("    => messageType            : %02x \n", pPtpStream->config.override.ptpHeader.messageType);
	printf("    => versionPTP             : %02x \n", pPtpStream->config.override.ptpHeader.versionPTP);
	printf("    => messageLength          : %04x \n", pPtpStream->config.override.ptpHeader.messageLength);
	printf("    => domainNumber           : %02x \n", pPtpStream->config.override.ptpHeader.domainNumber);
	printf("    => flagField              : %04x \n", pPtpStream->config.override.ptpHeader.flagField);
	data.ldata = pPtpStream->config.override.ptpHeader.correctionField;
	printf("    => correctionField        : %08x%08x \n", data.idata[0], data.idata[1]);
	printf("    => clockIdentity          : %02x%02x%02x%02x%02x%02x%02x%02x \n",
						 pPtpStream->config.override.ptpHeader.sourcePortIdentity.clockIdentity[0],
						 pPtpStream->config.override.ptpHeader.sourcePortIdentity.clockIdentity[1],
						 pPtpStream->config.override.ptpHeader.sourcePortIdentity.clockIdentity[2],
						 pPtpStream->config.override.ptpHeader.sourcePortIdentity.clockIdentity[3],
						 pPtpStream->config.override.ptpHeader.sourcePortIdentity.clockIdentity[4],
						 pPtpStream->config.override.ptpHeader.sourcePortIdentity.clockIdentity[5],
						 pPtpStream->config.override.ptpHeader.sourcePortIdentity.clockIdentity[6],
						 pPtpStream->config.override.ptpHeader.sourcePortIdentity.clockIdentity[7]);
	printf("    => portNumber             : %04x \n", pPtpStream->config.override.ptpHeader.sourcePortIdentity.portNumber);
	printf("    => sequenceId             : %04x \n", pPtpStream->config.override.ptpHeader.sequenceId);
	printf("    => controlField           : %02x \n", pPtpStream->config.override.ptpHeader.controlField);
	printf("    => logMessageInterval     : %02x \n", pPtpStream->config.override.ptpHeader.logMessageInterval);
	printf("  override->timeProperties \n");
	printf("    => currentUtcOffset       : %04x \n", pPtpStream->config.override.timeProperties.currentUtcOffset);
	printf("    => currentUtcOffsetValid  : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.timeProperties.currentUtcOffsetValid));	/* bool */
	printf("    => leap59                 : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.timeProperties.leap59));	/* bool */
	printf("    => leap61                 : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.timeProperties.leap61));	/* bool */
	printf("    => timeTraceable          : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.timeProperties.timeTraceable));	/* bool */
	printf("    => frequencyTraceable     : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.timeProperties.frequencyTraceable));	/* bool */
	printf("    => ptpTimescale           : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.timeProperties.ptpTimescale));	/* bool */
	printf("    => timeSource             : %s \n",   getPtpParameter_strTimeSource((int)pPtpStream->config.override.timeProperties.timeSource));	/* enum */
	printf("    => syncUncertain          : %s \n",   getPtpParameter_strBool((int)pPtpStream->config.override.timeProperties.synchronizationUncertain));	/* bool */
	printf("  override->anncData \n");
	data.ldata = pPtpStream->config.override.anncData.originTimestamp.secondsField;
	printf("    => originTimestamp.sec    : %08x%08x \n", data.idata[0], data.idata[1]);
	printf("    => originTimestamp.nsec   : %08x \n", pPtpStream->config.override.anncData.originTimestamp.nanosecondsField);
	printf("    => currentUtcOffset       : %04x \n", pPtpStream->config.override.anncData.currentUtcOffset);
	printf("    => grandmasterPriority1   : %02x \n", pPtpStream->config.override.anncData.grandmasterPriority1);
	printf("    => clockClass             : %02x \n", pPtpStream->config.override.anncData.grandmasterClockQuality.clockClass);
	printf("    => clockAccuracy          : %s \n",   getPtpParameter_strClockAccuracy((int)pPtpStream->config.override.anncData.grandmasterClockQuality.clockAccuracy));	/* enum */
	printf("    => offsetScaledLogVariance: %04x \n", pPtpStream->config.override.anncData.grandmasterClockQuality.offsetScaledLogVariance);
	printf("    => grandmasterPriority2   : %02x \n", pPtpStream->config.override.anncData.grandmasterPriority2);
	printf("    => grandmasterIdentity    : %02x%02x%02x%02x%02x%02x%02x%02x \n",
						 pPtpStream->config.override.anncData.grandmasterIdentity[0],
						 pPtpStream->config.override.anncData.grandmasterIdentity[1],
						 pPtpStream->config.override.anncData.grandmasterIdentity[2],
						 pPtpStream->config.override.anncData.grandmasterIdentity[3],
						 pPtpStream->config.override.anncData.grandmasterIdentity[4],
						 pPtpStream->config.override.anncData.grandmasterIdentity[5],
						 pPtpStream->config.override.anncData.grandmasterIdentity[6],
						 pPtpStream->config.override.anncData.grandmasterIdentity[7]);
	printf("    => stepsRemoved           : %04x \n", pPtpStream->config.override.anncData.stepsRemoved);
	printf("    => timeSource             : %02x \n", pPtpStream->config.override.anncData.timeSource);

	return;
}


char*	getPtpParameter_strBool(int	pram)
{
	/****************************************************************************/
	/***** typedef enum zl303xx_BooleanE									*****/
	/****************************************************************************/
	memset(getPtpParameter_str, 0, 64);

	switch(pram) {
		case	ZL303XX_FALSE:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_FALSE", pram);
			break;
		case	ZL303XX_TRUE:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_TRUE", pram);
			break;
		default:
			sprintf(getPtpParameter_str, "%02x : Bool Invalid Value!", pram);
			break;
	}
	return	getPtpParameter_str;
} 

char*	getPtpParameter_strUpdateTrigger(int	pram)
{
	/****************************************************************************/
	/***** typedef enum zl303xx_PtpClockBmcaTriggerE						*****/
	/****************************************************************************/
	memset(getPtpParameter_str, 0, 64);

	switch(pram) {
		case	ZL303XX_BMCA_TRIGGER_MIN_ANNC_INTVL:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_BMCA_TRIGGER_MIN_ANNC_INTVL", pram);
			break;
		case	ZL303XX_BMCA_TRIGGER_ON_RX_ANNC_DELTA:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_BMCA_TRIGGER_ON_RX_ANNC_DELTA", pram);
			break;
		case	ZL303XX_BMCA_TRIGGER_TIMER:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_BMCA_TRIGGER_TIMER", pram);
			break;
		case	ZL303XX_BMCA_TRIGGER_DISABLED:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_BMCA_TRIGGER_DISABLE", pram);
			break;
		default:
			sprintf(getPtpParameter_str, "%02x : enum Invalid Value!", pram);
			break;
	}
	return	getPtpParameter_str;
} 

char*	getPtpParameter_strClockAccuracy(int	pram)
{
	/****************************************************************************/
	/***** typedef enum zl303xx_ClockAccuracyE								*****/
	/****************************************************************************/
	memset(getPtpParameter_str, 0, 64);

	switch(pram) {
		case	ZL303XX_CLOCK_ACCURACY_WITHIN_25ns:	/* 0x20	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_WITHIN_25ns", pram);
			break;
		case	ZL303XX_CLOCK_ACCURACY_WITHIN_100ns:	/* 0x21	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_WITHIN_100ns", pram);
			break;
		case	ZL303XX_CLOCK_ACCURACY_WITHIN_250ns:	/* 0x22	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_WITHIN_250ns", pram);
			break;
		case	ZL303XX_CLOCK_ACCURACY_WITHIN_1us:	/* 0x23	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_WITHIN_1us", pram);
			break;
		case	ZL303XX_CLOCK_ACCURACY_WITHIN_2P5us:	/* 0x24	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_WITHIN_2P5us", pram);
			break;
		case	ZL303XX_CLOCK_ACCURACY_WITHIN_10us:	/* 0x25	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_WITHIN_10us", pram);
			break;
		case	ZL303XX_CLOCK_ACCURACY_WITHIN_25us:	/* 0x26	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_WITHIN_25us", pram);
			break;
		case	ZL303XX_CLOCK_ACCURACY_WITHIN_100us:	/* 0x27	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_WITHIN_100us", pram);
			break;
		case	ZL303XX_CLOCK_ACCURACY_WITHIN_250us:	/* 0x28	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_WITHIN_250us", pram);
			break;
		case	ZL303XX_CLOCK_ACCURACY_WITHIN_1ms:	/* 0x29	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_WITHIN_1ms", pram);
			break;
		case	ZL303XX_CLOCK_ACCURACY_WITHIN_2P5ms:	/* 0x2A	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_WITHIN_2P5ms", pram);
			break;
		case	ZL303XX_CLOCK_ACCURACY_WITHIN_10ms:	/* 0x2B	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_WITHIN_10ms", pram);
			break;
		case	ZL303XX_CLOCK_ACCURACY_WITHIN_25ms:	/* 0x2C	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_WITHIN_25ms", pram);
			break;
		case	ZL303XX_CLOCK_ACCURACY_WITHIN_100ms:	/* 0x2D	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_WITHIN_100ms", pram);
			break;
		case	ZL303XX_CLOCK_ACCURACY_WITHIN_250ms:	/* 0x2E	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_WITHIN_250ms", pram);
			break;
		case	ZL303XX_CLOCK_ACCURACY_WITHIN_1s:	/* 0x2F	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_WITHIN_1s", pram);
			break;
		case	ZL303XX_CLOCK_ACCURACY_WITHIN_10s:	/* 0x30	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_WITHIN_10s", pram);
			break;
		case	ZL303XX_CLOCK_ACCURACY_WORSE_THAN_10s:	/* 0x31	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_WORSE_THAN_10s", pram);
			break;
		case	ZL303XX_CLOCK_ACCURACY_UNKNOWN:	/* 0xF	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_CLOCK_ACCURACY_UNKNOWN", pram);
			break;
		default:
			sprintf(getPtpParameter_str, "enum Invalid Value!");
			break;
	}
	return	getPtpParameter_str;
}

char*	getPtpParameter_strTimeSource(int	pram)
{
	/****************************************************************************/
	/***** typedef enum zl303xx_TimeSourceE									*****/
	/****************************************************************************/
	memset(getPtpParameter_str, 0, 64);

	switch(pram) {
		case	ZL303XX_TIME_SOURCE_ATOMIC_CLOCK:	/* 0x10	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_TIME_SOURCE_ATOMIC_CLOCK", pram);
			break;
		case	ZL303XX_TIME_SOURCE_GPS:	/* 0x20	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_TIME_SOURCE_GPS", pram);
			break;
		case	ZL303XX_TIME_SOURCE_TERRESTRIAL_RADIO:	/* 0x30	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_TIME_SOURCE_TERRESTRIAL_RADIO", pram);
			break;
		case	ZL303XX_TIME_SOURCE_PTP:	/* 0x40	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_TIME_SOURCE_PTP", pram);
			break;
		case	ZL303XX_TIME_SOURCE_NTP:	/* 0x50	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_TIME_SOURCE_NTP", pram);
			break;
		case	ZL303XX_TIME_SOURCE_HAND_SET:	/* 0x60	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_TIME_SOURCE_HAND_SET", pram);
			break;
		case	ZL303XX_TIME_SOURCE_OTHER:	/* 0x90	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_TIME_SOURCE_OTHER", pram);
			break;
		case	ZL303XX_TIME_SOURCE_INTERNAL_OSCILLATOR:	/* 0xA	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_TIME_SOURCE_INTERNAL_OSCILLATOR", pram);
			break;
		default:
			sprintf(getPtpParameter_str, "%02x : enum Invalid Value!", pram);
			break;
	}
	return	getPtpParameter_str;
} 

char*	getPtpParameter_strProfile(int	pram)
{
	/****************************************************************************/
	/***** typedef enum zl303xx_PtpProfileE									*****/
	/****************************************************************************/
	memset(getPtpParameter_str, 0, 64);

	switch(pram) {
		case	ZL303XX_PTP_PROFILE_DEFAULT_E2E:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_PROFILE_DEFAULT_E2E", pram);
			break;
		case	ZL303XX_PTP_PROFILE_TELECOM_G8265_1:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_PROFILE_TELECOM_G8265_1", pram);
			break;
		case	ZL303XX_PTP_PROFILE_TELECOM_G8275_1v1:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_PROFILE_TELECOM_G8275_1v1", pram);
			break;
		case	ZL303XX_PTP_PROFILE_TELECOM_G8275_1v2:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_PROFILE_TELECOM_G8275_1v2", pram);
			break;
		case	ZL303XX_PTP_PROFILE_TELECOM_G8275_2:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_PROFILE_TELECOM_G8275_2", pram);
			break;
		case	ZL303XX_PTP_PROFILE_POWER_C37_238_2011:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_PROFILE_POWER_C37_238_2011", pram);
			break;
		case	ZL303XX_PTP_PROFILE_POWER_C37_238_2014:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_PROFILE_POWER_C37_238_2014", pram);
			break;
		case	ZL303XX_PTP_PROFILE_NUM_TYPES:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_PROFILE_NUM_TYPES", pram);
			break;
		default:
			sprintf(getPtpParameter_str, "%02x : enum Invalid Value!", pram);
			break;
	}
	return	getPtpParameter_str;
} 

char*	getPtpParameter_strEgressQlUpdate(int	pram)
{
	/****************************************************************************/
	/***** typedef enum zl303xx_PtpClockEgressQlUpdateE						*****/
	/****************************************************************************/
	memset(getPtpParameter_str, 0, 64);

	switch(pram) {
		case	ZL303XX_EGRESS_QL_UPDATE_ON_QUALIFY:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_EGRESS_QL_UPDATE_ON_QUALIFY", pram);
			break;
		case	ZL303XX_EGRESS_QL_UPDATE_ON_LOCK:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_EGRESS_QL_UPDATE_ON_LOCK", pram);
			break;
		default:
			sprintf(getPtpParameter_str, "%02x : enum Invalid Value!", pram);
			break;
	}
	return	getPtpParameter_str;
} 

char*	getPtpParameter_strEvalMethod(int	pram)
{
	/****************************************************************************/
	/***** typedef enum zl303xx_PtpG8275p1ClassEvalMethodE					*****/
	/****************************************************************************/
	memset(getPtpParameter_str, 0, 64);

	switch(pram) {
		case	ZL303XX_PTP_G8275p1_CLASS_EVAL_DEFAULT:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_G8275p1_CLASS_EVAL_DEFAULT", pram);
			break;
		case	ZL303XX_PTP_G8275p1_CLASS_EVAL_METHOD2:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_G8275p1_CLASS_EVAL_METHOD2", pram);
			break;
		case	ZL303XX_PTP_G8275p1_CLASS_EVAL_METHOD3:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_G8275p1_CLASS_EVAL_METHOD3", pram);
			break;
		default:
			sprintf(getPtpParameter_str, "%02x : enum Invalid Value!", pram);
			break;
	}
	return	getPtpParameter_str;
} 

char*	getPtpParameter_strNetworkProtocol(int	pram)
{
	/****************************************************************************/
	/***** typedef enum zl303xx_NetworkProtocolE							*****/
	/****************************************************************************/
	memset(getPtpParameter_str, 0, 64);

	switch(pram) {
		case	ZL303XX_NETWORK_PROTOCOL_UDP_IPv4:	/* 0x01	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_NETWORK_PROTOCOL_UDP_IPv4", pram);
			break;
		case	ZL303XX_NETWORK_PROTOCOL_UDP_IPv6:	/* 0x02	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_NETWORK_PROTOCOL_UDP_IPv6", pram);
			break;
		case	ZL303XX_NETWORK_PROTOCOL_802_3:	/* 0x03	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_NETWORK_PROTOCOL_802_3", pram);
			break;
		case	ZL303XX_NETWORK_PROTOCOL_DEVICENET:	/* 0x04	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_NETWORK_PROTOCOL_DEVICENET", pram);
			break;
		case	ZL303XX_NETWORK_PROTOCOL_CONTROLNET:	/* 0x05	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_NETWORK_PROTOCOL_CONTROLNET", pram);
			break;
		case	ZL303XX_NETWORK_PROTOCOL_PROFINET:	/* 0x06	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_NETWORK_PROTOCOL_PROFINET", pram);
			break;
		case	ZL303XX_NETWORK_PROTOCOL_UNKNOWN:	/* 0xFFFE	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_NETWORK_PROTOCOL_UNKNOWN", pram);
			break;
/*
 *		case	ZL303XX_NETWORK_PROTOCOL_INVALID:
 *			sprintf(getPtpParameter_str, "%02x : ZL303XX_NETWORK_PROTOCOL_INVALID", pram);
 *			break;
 */
 		default:
			sprintf(getPtpParameter_str, "%02x : enum Invalid Value!", pram);
			break;
	}
	return	getPtpParameter_str;
} 

char*	getPtpParameter_strDelayMechanism(int	pram)
{
	/****************************************************************************/
	/***** typedef enum zl303xx_DelayMechanismE								*****/
	/****************************************************************************/
	memset(getPtpParameter_str, 0, 64);

	switch(pram) {
		case	ZL303XX_DELAY_MECHANISM_E2E:	/* 0x01	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_DELAY_MECHANISM_E2E", pram);
			break;
		case	ZL303XX_DELAY_MECHANISM_P2P:	/* 0x02	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_DELAY_MECHANISM_P2P", pram);
			break;
		case	ZL303XX_DELAY_MECHANISM_DISABLED:	/* 0xFE	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_DELAY_MECHANISM_DISABLED", pram);
			break;
		default:
			sprintf(getPtpParameter_str, "%02x : enum Invalid Value!", pram);
			break;
	}
	return	getPtpParameter_str;
} 

char*	getPtpParameter_strPdelayRespMethod(int	pram)
{
	/****************************************************************************/
	/***** typedef enum zl303xx_PtpPdelayRespMethodE						*****/
	/****************************************************************************/
	memset(getPtpParameter_str, 0, 64);

	switch(pram) {
		case	ZL303XX_PTP_PDELAY_METHOD_1_STEP_FLAG:	/* 0x10	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_PDELAY_METHOD_1_STEP_FLAG", pram);
			break;
		case	ZL303XX_PTP_PDELAY_METHOD_1_STEP_DROP:	/* 0x11	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_PDELAY_METHOD_1_STEP_DROP", pram);
			break;
		case	ZL303XX_PTP_PDELAY_METHOD_1_STEP_TS:	/* 0x12	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_PDELAY_METHOD_1_STEP_TS", pram);
			break;
		case	ZL303XX_PTP_PDELAY_METHOD_1_STEP_CORR:	/* 0x13	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_PDELAY_METHOD_1_STEP_CORR", pram);
			break;
		case	ZL303XX_PTP_PDELAY_METHOD_2_STEP_RESIDENT:	/* 0x01	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_PDELAY_METHOD_2_STEP_RESIDENT", pram);
			break;
		case	ZL303XX_PTP_PDELAY_METHOD_2_STEP_RAW:	/* 0x02	*/
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_PDELAY_METHOD_2_STEP_RAW", pram);
			break;
		default:
			sprintf(getPtpParameter_str, "%02x : enum Invalid Value!", pram);
			break;
	}
	return	getPtpParameter_str;
} 

char*	getPtpParameter_strStreamMode(int	pram)
{
	/****************************************************************************/
	/***** typedef enum zl303xx_PtpgetPtpParameter_StreamModeE								*****/
	/****************************************************************************/
	memset(getPtpParameter_str, 0, 64);

	switch(pram) {
		case	ZL303XX_PTP_STREAM_USE_BMC:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_STREAM_USE_BMC", pram);
			break;
		case	ZL303XX_PTP_STREAM_SLAVE_ONLY:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_STREAM_SLAVE_ONLY", pram);
			break;
/*
 *		case	ZL303XX_PTP_STREAM_MASTER_ONLY:
 *			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_STREAM_MASTER_ONLY", pram);
 *			break;
 */
		case	ZL303XX_PTP_STREAM_NOT_SLAVE:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_STREAM_NOT_SLAVE", pram);
			break;
		case	ZL303XX_PTP_STREAM_MODE_NUM_TYPES:
			sprintf(getPtpParameter_str, "%02x : ZL303XX_PTP_STREAM_MODE_NUM_TYPES", pram);
			break;
		default:
			sprintf(getPtpParameter_str, "%02x : enum Invalid Value!", pram);
			break;
	}
	return	getPtpParameter_str;
} 

/*****************   END   ****************************************************/
