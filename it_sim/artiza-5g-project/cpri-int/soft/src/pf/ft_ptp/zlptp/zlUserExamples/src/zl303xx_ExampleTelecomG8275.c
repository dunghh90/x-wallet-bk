

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Example code to initialize the device as an PTP Telecom Profile for Phase
*     (G.8275) master or slave.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Error.h"

#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpG8275p1Api.h"
#include "zl303xx_Ptpd.h"

#include "zl303xx_ExamplePtpGlobals.h"

#if defined APR_INCLUDED
   /* Setup global APR options */
#include "zl303xx_ExampleAprGlobals.h"
#endif

#include "zl303xx_ExampleUtils.h"
#include "zl303xx_ExampleTelecomG8275.h"

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
#include "zl303xx_ApiConfig.h"
#endif

#if defined ZLS30390_INCLUDED 
#include "zl303xx_PortingFunctions.h"
#endif

#include "f_du_reg_eep.h"
#include "BPF_COM_LOG.h"

/*****************   DEFINES     **********************************************/

/* IP Address catalog. */
#define DEFAULT_TELECOM_PHASE_MASTER_1_IP       "10.0.0.121"
#define DEFAULT_TELECOM_PHASE_MASTER_2_IP       "10.0.0.122"

#define DEFAULT_TELECOM_PHASE_SLAVE_1_IP        "10.0.0.161"
#define DEFAULT_TELECOM_PHASE_SLAVE_2_IP        "10.0.0.162"


/* Ethernet Interface catalog */
#define ZL303XX_ETH_0     "eth0"
#define ZL303XX_ETH_1_1   "eth1.1"
#define ZL303XX_ETH_1_2   "eth1.2"
#define ZL303XX_ETH_1_3   "eth1.3"
/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

zlStatusE examplePtpTelecomPhaseMasterBase(void);
zlStatusE examplePtpTelecomPhaseSlaveBase(void);
zlStatusE examplePtpTelecomPhaseG8275VirtualPortAdd(zl303xx_PtpClockHandleT clockHandle);
#if defined ZLS30390_INCLUDED 
static zlStatusE examplePtpTelecomPhaseEthInterfaceStrSet(exampleAppS *pZlExApp, const char *ifName, Uint32T portNum);
#endif
/*****************   STATIC GLOBAL VARIABLES   ********************************/
int accepted_clock_classes;		/* EEPROMから取り出したaccepted_clock_classes格納用 */


/*****************   IMPORTED GLOBAL VARIABLES   ******************************/
#if defined ZLS30390_INCLUDED
extern Uint8T TARGET_DPLL;
#endif


/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/
int BPF_HM_DEVC_EEPROM_READ(unsigned short offset, unsigned char *data_p);

Uint16T zl303xx_PtpV2handleAnnounce(
      zl303xx_PtpClockMsgQueueS *ptpData,
      zl303xx_PtpV2MsgS *msg,
      zl303xx_ClockStreamS *streamData,
      zl303xx_PortDataS *portData);


/* examplePtpTelecomPhaseG8275p1Master */
/**
   Configures the system for PTP Telecom Profile for Phase (G.8275.1) master
   mode operation. This uses multicast transmission with 802_3 (ETH LAyer 2).

*******************************************************************************/
zlStatusE examplePtpTelecomPhaseG8275p1Master(void)
{
   zlStatusE status = ZL303XX_OK;

   /* IP固定とはしない */
   /* const char *srcIpv4Addr = "10.0.0.121"; */
   const char *destIpv4Addr = (const char *)DEFAULT_PTP_DOMAIN_ADDRESS_802_3;

	/* Master運用時に必要。PktRateを設定しないとIntervalに異常値が設定される。(semi回答による対処) */
   if (status == ZL303XX_OK)
   {
      status |= zl303xx_AprSetPktRate(16, ZL303XX_TRUE);
      status |= zl303xx_AprSetPktRate(16, ZL303XX_FALSE);
   }

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      /* Set the Profile so that appropriate default values are initialized. */
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_TELECOM_G8275_1);
      status = exampleAppStructInit(1,    /* 1 Clock  */
                                    1,    /* 1 Port   */
                                    1,    /* 1 Stream */
                                    &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
               /* Local Port address is IPv4 */
      /* status = examplePtpLocalIpv4Set(srcIpv4Addr, ZL303XX_FALSE, &zlExampleApp.port[0].ptp) | */
               /* Destination address is the 802.3 Default. */
               /* examplePtpDestEthL2Set(destIpv4Addr, ZL303XX_FALSE, &zlExampleApp.stream[0].ptp); */
               status = examplePtpDestEthL2Set(destIpv4Addr, ZL303XX_FALSE, &zlExampleApp.stream[0].ptp);
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8275_1;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.clockType = ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_GM;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.domainNumber = ZL303XX_PTP_G8275p1_DOMAIN_NUM_DEF;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.priority1 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.priority2 = ZL303XX_PTP_G8275p1_CLOCK_PRI2_MASTER;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = ZL303XX_PTP_G8275p1_CLASS_FREERUN;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockAccuracy = ZL303XX_PTP_G8275p1_ACCURACY_UNLOCK;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.offsetScaledLogVariance = ZL303XX_PTP_G8275p1_VARIANCE_UNLOCK;
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profileStrict = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.g8275p1.bypassEnabled = ZL303XX_FALSE;

      zlExampleApp.port[0].ptp.config.delayMechanism = ZL303XX_DELAY_MECHANISM_E2E;
      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_FALSE;

      /* memo : AnnounceはRate設定するAPIが用意されていないため、直値設定 (microsemi回答による修正) */
      zlExampleApp.stream[0].ptp.config.logAnnounceInterval = -3;

      zlExampleApp.stream[0].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
      zlExampleApp.stream[0].ptp.config.unicast = ZL303XX_FALSE;

      status = exampleAppStart(&zlExampleApp);
   }

   /* Add a Virtual port to the G.8275.1 Master. */
   if (status == ZL303XX_OK)
   {
      status = examplePtpTelecomPhaseG8275VirtualPortAdd(zlExampleApp.clock[TARGET_DPLL].ptp.handle);
   }

   return status;
}

/* examplePtpTelecomPhaseG8275VirtualPortAdd */
/**
   Adds a Virtual PTP Port to the Phase Master.

  Parameters:
        clockHandle  Handle of the clock to which to add the port.

  Return Value:
   zlStatusE

  Notes:
   Should only be called by exampleTelecomPhaseMaster and master variants.

*******************************************************************************/
zlStatusE examplePtpTelecomPhaseG8275VirtualPortAdd(zl303xx_PtpClockHandleT clockHandle)
{
   zlStatusE status = ZL303XX_OK;

   /* Get the configured data for the associated clock. */
   zl303xx_PtpClockCreateS clockConfig;
   status = zl303xx_PtpClockConfigGet(clockHandle, &clockConfig);

   /* Add a Virtual port to the G.8275.1 Master. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpVirtualPortConfigS pVirtualPortCreate;
      zl303xx_PtpPortHandleT vpHandle;

      /* Get default Parameters. */
      pVirtualPortCreate.clockHandle = clockHandle;
      (void)zl303xx_PtpVirtualPortCreateStructInit(&pVirtualPortCreate);

      /* Configure the Virtual port as required. */
      memcpy(pVirtualPortCreate.clockIdentity,
             clockConfig.defaultDS.clockIdentity,
             PTP_V2_CLK_IDENTITY_LENGTH);

      pVirtualPortCreate.stepsRemoved = 0;

      pVirtualPortCreate.prtcConnected = ZL303XX_TRUE;

      pVirtualPortCreate.clockQuality.clockClass = ZL303XX_PTP_G8275p1_GM_CLASS_LOCK;
      pVirtualPortCreate.clockQuality.clockAccuracy = ZL303XX_PTP_G8275p1_ACCURACY_LOCK;
      pVirtualPortCreate.clockQuality.offsetScaledLogVariance = ZL303XX_PTP_G8275p1_VARIANCE_LOCK;
      pVirtualPortCreate.priority1 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;
      pVirtualPortCreate.priority2 = ZL303XX_PTP_G8275p1_CLOCK_PRI2_MASTER;
      pVirtualPortCreate.localPriority = ZL303XX_PTP_G8275p1_CLOCK_PRI2_MASTER;
      pVirtualPortCreate.timeSource = ZL303XX_TIME_SOURCE_GPS;

      pVirtualPortCreate.timeProperties.currentUtcOffsetValid = ZL303XX_TRUE;
      pVirtualPortCreate.timeProperties.timeTraceable = ZL303XX_TRUE;
      pVirtualPortCreate.timeProperties.frequencyTraceable = ZL303XX_TRUE;
      pVirtualPortCreate.timeProperties.ptpTimescale = ZL303XX_TRUE;
      pVirtualPortCreate.timeProperties.timeSource = ZL303XX_TIME_SOURCE_GPS;

      if (ZL303XX_OK == zl303xx_PtpVirtualPortCreate(&pVirtualPortCreate, &vpHandle))
      {
         /* Update Time Properties. */
         pVirtualPortCreate.timeSetFlag = ZL303XX_TRUE;
         (void)zl303xx_PtpVirtualPortSet(vpHandle, &pVirtualPortCreate);

         /* Enable the Port if created. */
         (void)zl303xx_PtpVirtualPortPtsfSet(vpHandle, ZL303XX_FALSE);
      }
   }

   return status;
}

/* examplePtpTelecomPhaseG8275p1Bc */
/**
   An example of how to configure a PTP boundary clock that uses PTP Telecom
   Profile for Phase (G.8275.1 - Edition 1). 2 PTP ports are created, each with
   a single stream. This uses multicast transmission with 802_3 (ETH Layer 2).



 examplePtpTelecomPhaseG8275p1Master()
                           examplePtpTelecomPhaseG8275p1Bc()
                                                         examplePtpTelecomPhaseG8275p1BcSlave()

 +-------------+               +-----------------------+                +-------------+
 |  Multicast  |  Timestamped  |    Boundary Clock     |  Timestamped   |  Multicast  |
 |   Master    |<------------->|-----------------------|<-------------->|    Slave    |
 | 10.0.0.121  |    Packets    | Multicast | Multicast |    Packets     | 10.0.0.161  |
 | Class = 10  |     (L2)      |   Slave   |   Master  |      (L2)      | Class = 255 |
 +-------------+               |10.0.0.205 | 10.0.0.206|                +-------------+
              (0x011B19000000) |     Class = 187       |  (0x011B19000001)
                               +-----------+-----------+
                                  Port 1       Port 2



*******************************************************************************/
zlStatusE examplePtpTelecomPhaseG8275p1Bc(void)
{
   zlStatusE status = ZL303XX_OK;

   const char *srcIpv4Addr[2]  = {"10.0.0.205", "10.0.0.206"};
   const char *destIpv4Addr[2] = {(const char *)DEFAULT_PTP_DOMAIN_ADDRESS_802_3,
                                  (const char *)ALTERNATE_PTP_DOMAIN1_ADDRESS_802_3};


   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      /* Set the Profile so that appropriate default values are initialized. */
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_TELECOM_G8275_1);
      status = exampleAppStructInit(1, 2, 2, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
      Uint32T count;

      zlExampleApp.clock[TARGET_DPLL].ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8275_1;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.clockType = ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_BC;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.domainNumber = ZL303XX_PTP_G8275p1_DOMAIN_NUM_DEF;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.priority1 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.priority2 = ZL303XX_PTP_G8275p1_CLOCK_PRI2_MASTER;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = ZL303XX_PTP_G8275p1_CLASS_FREERUN;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockAccuracy = ZL303XX_PTP_G8275p1_ACCURACY_UNLOCK;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.offsetScaledLogVariance = ZL303XX_PTP_G8275p1_VARIANCE_UNLOCK;
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profileStrict = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.g8275p1.bypassEnabled = ZL303XX_FALSE;

      /* Port Config */
      for (count = 0; count < 2; count++)
      {
         zlExampleApp.port[count].ptp.config.delayMechanism = ZL303XX_DELAY_MECHANISM_E2E;
         zlExampleApp.port[count].ptp.config.unicastNegotiation.enabled = ZL303XX_FALSE;

         /* Local Port address is IPv4 */
         status |= examplePtpLocalIpv4Set(srcIpv4Addr[count], ZL303XX_TRUE,
                                          &zlExampleApp.port[count].ptp);
      }

      /* Stream Config */
      for (count = 0; count < 2; count++)
      {
         zlExampleApp.stream[count].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
         zlExampleApp.stream[count].ptp.config.unicast = ZL303XX_FALSE;
         zlExampleApp.stream[count].ptp.config.portHandle = count;  /* One stream per port. */

         /* Destination addresses are the 802.3 values. */
         status |= examplePtpDestEthL2Set(destIpv4Addr[count], ZL303XX_TRUE,
                                            &zlExampleApp.stream[count].ptp);
      }
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStart(&zlExampleApp);
   }


   return status;
}

/*
  Function Name:
   examplePtpTelecomPhaseG8275p1Bc2Ifs
   An example of how to configure a PTP boundary clock that uses PTP Telecom
   Profile for Phase (G.8275.1 - Edition 1) on two different ethernet interfaces.
   2 PTP ports are created, each on a different interface with a single stream.
   This uses multicast transmission with 802_3 (ETH Layer 2).



 examplePtpTelecomPhaseG8275p1Master()
                           examplePtpTelecomPhaseG8275p1Bc2Ifs()
                                                         examplePtpTelecomPhaseG8275p1BcSlave()

 +-------------+               +-----------------------+                +-------------+
 |  Multicast  |  Timestamped  |    Boundary Clock     |  Timestamped   |  Multicast  |
 |   Master    |<------------->|-----------------------|<-------------->|    Slave    |
 | XX.0.0.XXX  |    Packets    | Multicast | Multicast |    Packets     | XX.0.0.XXX  |
 | Class = 10  |     (L2)      |   Slave   |   Master  |      (L2)      | Class = 255 |
 +-------------+               |XX.0.0.XXX | XX.0.0.XXX|                +-------------+
         (0x01:1B:19:00:00:00) |     Class = 187       |  (0x01:1B:19:00:00:00)
                               +-----------+-----------+
                                  Port 1       Port 2
                                 (eth1.1)     (eth1.2)

  Parameters:
      None.

  Return Value:  zlStatusE
*******************************************************************************/
zlStatusE examplePtpTelecomPhaseG8275p1Bc2Ifs(void)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T i;
   const Uint8T numPorts = 2;
#if  defined _ZL303XX_ZLE1588_BOARD
   const Uint8T *destAddrStr = DEFAULT_PTP_DOMAIN_ADDRESS_802_3;    /* All ports will use same destAddr, network is responsible to isolate ports */
#else
   const Uint8T *destAddrStr = PDELAY_PTP_DOMAIN_ADDRESS_802_3;
#endif

 #if defined APR_INCLUDED
    if (status == ZL303XX_OK)
    {
       /* Setup global configuration options */
       status |= zl303xx_AprSetPktRate(16, ZL303XX_TRUE);
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
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_TELECOM_G8275_1); /* For defaults */
   }

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
       if (status == ZL303XX_OK)
       {
          zl303xx_SetExampleInterface(2);
          zl303xx_SetDeviceMode(ZL303XX_MODE_REF_EXTERNAL);
           status = exampleAppStructInit(2,    /* 2 Clocks */
                                         numPorts,    /* N Ports   */
                                         numPorts,    /* N Streams */
                                         &zlExampleApp);

       }
#else
       status = exampleAppStructInit(1,    /* 1 Clock  */
                                     numPorts,    /* N Ports  */
                                     numPorts,    /* N Streams*/
                                     &zlExampleApp);

#endif

   if (status == ZL303XX_OK)
   {
      /* Start Port 0 on eth1.1 */
      status = examplePtpTelecomPhaseEthInterfaceStrSet(&zlExampleApp, ZL303XX_ETH_1_1, 0);
   }
   if (status == ZL303XX_OK)
   {
      /* Start Port 1 on eth1.2 */
      status = examplePtpTelecomPhaseEthInterfaceStrSet(&zlExampleApp, ZL303XX_ETH_1_2, 1);
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8275_1;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.clockType = ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_BC;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.domainNumber = ZL303XX_PTP_G8275p1_DOMAIN_NUM_DEF;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.priority1 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.priority2 = ZL303XX_PTP_G8275p1_CLOCK_PRI2_MASTER;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = ZL303XX_PTP_G8275p1_CLASS_FREERUN;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockAccuracy = ZL303XX_PTP_G8275p1_ACCURACY_UNLOCK;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.offsetScaledLogVariance = ZL303XX_PTP_G8275p1_VARIANCE_UNLOCK;
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profileStrict = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.g8275p1.bypassEnabled = ZL303XX_FALSE;

      for (i=0; i<numPorts; i++)
      {
         zlExampleApp.port[i].ptp.config.delayMechanism = ZL303XX_DELAY_MECHANISM_E2E;
         zlExampleApp.port[i].ptp.config.unicastNegotiation.enabled = ZL303XX_FALSE;
         zlExampleApp.port[i].ptp.config.requestedHandle = i;

         /* Destination address is the 802.3 Default and same for all streams (isolated ports). */
         examplePtpDestEthL2Set((const char *)destAddrStr, ZL303XX_TRUE, &zlExampleApp.stream[i].ptp);
         zlExampleApp.stream[i].ptp.config.logAnnounceInterval = -3;
         zlExampleApp.stream[i].ptp.config.logSyncInterval = -4;
         zlExampleApp.stream[i].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
         zlExampleApp.stream[i].ptp.config.unicast = ZL303XX_FALSE;
         zlExampleApp.stream[i].ptp.config.portHandle = i; /* 1:1 stream:port */

      }

      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}

/* examplePtpTelecomPhaseG8275p1Slave */
/**
   Configures the system for PTP Telecom Profile for Phase (G.8275.1) master
   mode operation. This uses multicast transmission with 802.3 Transport.

  examplePtpTelecomPhaseG8275p1Master()    examplePtpTelecomPhaseG8275p1Slave()

       +-------------+                            +-------------+
       |  Multicast  |         Timestamped        |  Multicast  |
       |   Master    |<-------------------------->|    Slave    |
       | 10.0.0.121  |          Packets           | 10.0.0.161  |
       | Class = 6   |          (802.3)           | Class = 255 |
       +-------------+    (0x01:1B:19:00:00:00)   +-------------+



*******************************************************************************/
zlStatusE examplePtpTelecomPhaseG8275p1Slave(void)
{
   zlStatusE status = ZL303XX_OK;
	unsigned char	domain_num = 0;		/* DomainNumber(from EEPROM) */
	unsigned char	eep_clockclass = 0;	
	

   zl303xx_SetDefaultDeviceType(ZL3077X_DEVICETYPE);

   if (status == ZL303XX_OK)
   {
      status |= zl303xx_AprSetPktRate(16, ZL303XX_TRUE);
      status |= zl303xx_AprSetPktRate(16, ZL303XX_FALSE);

      status |= exampleAprSetConfigParameters(ACI_BC_FULL_ON_PATH_PHASE_SYNCE);
      BPF_COM_LOG_DMESG( "PTP Start Process : Slave / SyncE assist Mode(%d) \n",status);
   }

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      /* Set the Profile so that appropriate default values are initialized. */
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_TELECOM_G8275_1);

      status = exampleAppStructInit(4,    /* 4 Clock  */
                                    1,    /* 1 Port   */
                                    1,    /* 1 Stream */
                                    &zlExampleApp);
   }


   if (status == ZL303XX_OK)
   {
		/* EEPROMからdomain-number設定値を取得 */
		BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_VID_DOMAIN_NO, &domain_num );
		/* 設定値が誤っている場合はデフォルト値(0x18)を設定 */
	  if( (domain_num < 0x18) || ( domain_num > 0x2B) ) {
          domain_num = 0x18;
      }
	
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8275_1;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.clockType = ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_TSC;
      /* zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.domainNumber = ZL303XX_PTP_G8275p1_DOMAIN_NUM_DEF; */
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.domainNumber = domain_num;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.priority1 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.priority2 = ZL303XX_PTP_G8275p1_CLOCK_PRI2_SLAVE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = ZL303XX_PTP_G8275p1_CLASS_SLAVE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockAccuracy = ZL303XX_PTP_G8275p1_ACCURACY_UNLOCK;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.offsetScaledLogVariance = ZL303XX_PTP_G8275p1_VARIANCE_UNLOCK;
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profileStrict = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.g8275p1.bypassEnabled = ZL303XX_FALSE;

      zlExampleApp.port[0].ptp.config.delayMechanism = ZL303XX_DELAY_MECHANISM_E2E;
      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_FALSE;

      zlExampleApp.port[1].ptp.config.delayMechanism = ZL303XX_DELAY_MECHANISM_E2E;
      zlExampleApp.port[1].ptp.config.unicastNegotiation.enabled = ZL303XX_FALSE;

      /* memo : AnnounceはRate設定するAPIが用意されていないため、直値設定 (microsemi回答による修正) */
      zlExampleApp.stream[0].ptp.config.logAnnounceInterval = -3;
      zlExampleApp.stream[1].ptp.config.logAnnounceInterval = -3;
      
      zlExampleApp.stream[1].ptp.config.announceReceiptTimeout = 3;
      zlExampleApp.stream[0].ptp.config.announceReceiptTimeout = 3;

	  BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_VID_ACPT_CLK_CLASS, &eep_clockclass );		/* EEPROM:0x372 Announce判定 */
	  accepted_clock_classes = eep_clockclass;

      zlExampleApp.stream[0].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
      zlExampleApp.stream[0].ptp.config.unicast = ZL303XX_FALSE;
      zlExampleApp.stream[0].ptp.config.maxClockClass = eep_clockclass;
      
      zlExampleApp.stream[1].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
      zlExampleApp.stream[1].ptp.config.unicast = ZL303XX_FALSE;
      zlExampleApp.stream[1].ptp.config.maxClockClass = eep_clockclass;

      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}

/* examplePtpTelecomPhaseG8275p1BcSlave */
/**
   Configures the system for PTP Telecom Profile for Phase (G.8275.1 - Edition 1)
   master mode operation. This uses multicast transmission with 802.3 Transport.

   To be used with the following examples:
   - examplePtpTelecomPhaseG8275p1BcMaster()
   - examplePtpTelecomPhaseG8275p1Bc()
   - examplePtpTelecomPhaseG8275p1BcSlave()

*******************************************************************************/
zlStatusE examplePtpTelecomPhaseG8275p1BcSlave(void)
{
   zlStatusE status = ZL303XX_OK;

   const char *srcIpv4Addr = "10.0.0.161";
   const char *destIpv4Addr = (const char *)ALTERNATE_PTP_DOMAIN1_ADDRESS_802_3;

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStructInit(1,    /* 1 Clock  */
                                    1,    /* 1 Port   */
                                    1,    /* 1 Stream */
                                    &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
               /* Local Port address is IPv4 */
      status = examplePtpLocalIpv4Set(srcIpv4Addr, ZL303XX_FALSE, &zlExampleApp.port[0].ptp) |
               /* Destination address is the 802.3 value. */
               examplePtpDestEthL2Set(destIpv4Addr, ZL303XX_FALSE, &zlExampleApp.stream[0].ptp);
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8275_1;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.clockType = ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_TSC;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.domainNumber = ZL303XX_PTP_G8275p1_DOMAIN_NUM_DEF;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.priority1 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.priority2 = ZL303XX_PTP_G8275p1_CLOCK_PRI2_SLAVE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = ZL303XX_PTP_G8275p1_CLASS_SLAVE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockAccuracy = ZL303XX_PTP_G8275p1_ACCURACY_UNLOCK;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.offsetScaledLogVariance = ZL303XX_PTP_G8275p1_VARIANCE_UNLOCK;
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profileStrict = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.g8275p1.bypassEnabled = ZL303XX_FALSE;

      zlExampleApp.port[0].ptp.config.delayMechanism = ZL303XX_DELAY_MECHANISM_E2E;
      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_FALSE;

      zlExampleApp.stream[0].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
      zlExampleApp.stream[0].ptp.config.unicast = ZL303XX_FALSE;

      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}

/* examplePtpTelecomPhaseG8275p2Master */
/**
   Configures the system for PTP Telecom Profile for Phase (G.8275.2) master
   mode operation. This uses unicast transmission with IPv4.

*******************************************************************************/
zlStatusE examplePtpTelecomPhaseG8275p2Master(void)
{
   zlStatusE status = ZL303XX_OK;
   exampleAppClockS *clck = NULL;
   const char *srcIpv4Addr = "10.0.0.240";

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
   zl303xx_SetExampleInterface(2);
   zl303xx_SetAnnouncePps(0,1);
   status = zl303xx_AprSetDeviceOptMode(ZL303XX_ELECTRIC_MODE);
#endif

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      /* Set the Profile so that appropriate default values are initialized. */
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_TELECOM_G8275_2);
#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
       if (status == ZL303XX_OK)
       {
           zl303xx_SetDeviceMode(ZL303XX_MODE_REF_EXTERNAL);
           status = exampleAppStructInit(2,    /* 2 Clocks */
                                         1,    /* 1 Port   */
                                         0,    /* Streams will be requested;  */
                                         &zlExampleApp);
           clck = &zlExampleApp.clock[TARGET_DPLL];
       }
#else
       status = exampleAppStructInit(1,    /* 1 Clock  */
                                     1,    /* 1 Port   */
                                     0,    /* Streams will be requested;  */
                                     &zlExampleApp);
       clck = &zlExampleApp.clock[0];
#endif
   }

   if (status == ZL303XX_OK)
   {
      status = examplePtpLocalIpv4Set(srcIpv4Addr, ZL303XX_FALSE, &zlExampleApp.port[0].ptp);
#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
      setExampleIpSrc(srcIpv4Addr);
#endif
   }

   if (status == ZL303XX_OK)
   {
      clck->ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8275_2;
      clck->ptp.config.clockType = ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_GM;
      clck->ptp.config.defaultDS.domainNumber = ZL303XX_PTP_G8275p2_DOMAIN_NUM_DEF;
      clck->ptp.config.defaultDS.priority1 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;
      clck->ptp.config.defaultDS.priority2 = ZL303XX_PTP_G8275p1_CLOCK_PRI2_MASTER;
      clck->ptp.config.defaultDS.clockQuality.clockClass = ZL303XX_PTP_G8275p1_CLASS_FREERUN;
      clck->ptp.config.defaultDS.clockQuality.clockAccuracy = ZL303XX_PTP_G8275p1_ACCURACY_UNLOCK;
      clck->ptp.config.defaultDS.clockQuality.offsetScaledLogVariance = ZL303XX_PTP_G8275p1_VARIANCE_UNLOCK;
      clck->pktRef = ZL303XX_FALSE;
      clck->ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      clck->ptp.config.profileStrict = ZL303XX_TRUE;
      clck->ptp.config.profileCfg.g8275p1.bypassEnabled = ZL303XX_FALSE;

      zlExampleApp.port[0].ptp.config.delayMechanism = ZL303XX_DELAY_MECHANISM_E2E;
      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_TRUE;

      status = exampleAppStart(&zlExampleApp);
   }

   /* Add a Virtual port to the G.8275.1 Master. */
   if (status == ZL303XX_OK)
   {
      status = examplePtpTelecomPhaseG8275VirtualPortAdd(zlExampleApp.clock[TARGET_DPLL].ptp.handle);
   }

   return status;
}

/* examplePtpTelecomPhaseG8275p2Slave */
/**
   Configures the system for PTP Telecom Profile for Phase (G.8275.2) slave
   mode operation. This uses unicast transmission with IPv4.

   examplePtpTelecomPhaseG8275p2Master_Draft()
                                    examplePtpTelecomPhaseG8275p2Slave_Draft()

       +-------------+                           +-------------+
       |   Unicast   |         Timestamped       |   Unicast   |
       |   Master    |<------------------------->|    Slave    |
       | 10.0.0.240  |          Packets          | 10.0.0.121  |
       | Class = 6   |         (Unicast)         | Class = 255 |
       +-------------+                           +-------------+



*******************************************************************************/
zlStatusE examplePtpTelecomPhaseG8275p2Slave(void)
{
   zlStatusE status = ZL303XX_OK;
   exampleAppClockS *clck = NULL;
   const char *srcIpv4Addr = "10.0.0.121";
   const char *dstIpv4Addr = "10.0.0.240";

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
   zl303xx_SetExampleInterface(1);
   zl303xx_SetAnnouncePps(0, 1);
#endif

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      /* Set the Profile so that appropriate default values are initialized. */
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_TELECOM_G8275_2);
#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
       status = exampleAppStructInit(2,    /* 2 Clocks */
                                     1,    /* 1 Port   */
                                     1,    /* 1 Stream */
                                     &zlExampleApp);
       clck = &zlExampleApp.clock[TARGET_DPLL];
#else
       status = exampleAppStructInit(1,    /* 1 Clock  */
                                     1,    /* 1 Port   */
                                     1,    /* 1 Stream */
                                     &zlExampleApp);
       clck = &zlExampleApp.clock[0];
#endif
   }

   if (status == ZL303XX_OK)
   {
      status = examplePtpLocalIpv4Set(srcIpv4Addr, ZL303XX_FALSE, &zlExampleApp.port[0].ptp);
      if (status == ZL303XX_OK)
      {
         status = examplePtpDestIpv4Set(dstIpv4Addr, ZL303XX_FALSE, &zlExampleApp.stream[0].ptp);
      }
#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
      setExampleIpSrc(srcIpv4Addr);

#else
    /* #warning MSCC: Set Source IPV4 address on port here */
#endif
   }

   if (status == ZL303XX_OK)
   {
      clck->ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8275_2;
      clck->ptp.config.clockType = ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_TSC;
      clck->ptp.config.defaultDS.domainNumber = ZL303XX_PTP_G8275p2_DOMAIN_NUM_DEF;
      clck->ptp.config.defaultDS.priority1 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;
      clck->ptp.config.defaultDS.priority2 = ZL303XX_PTP_G8275p1_CLOCK_PRI2_SLAVE;
      clck->ptp.config.defaultDS.clockQuality.clockClass = ZL303XX_PTP_G8275p1_CLASS_SLAVE;
      clck->ptp.config.defaultDS.clockQuality.clockAccuracy = ZL303XX_PTP_G8275p1_ACCURACY_UNLOCK;
      clck->ptp.config.defaultDS.clockQuality.offsetScaledLogVariance = ZL303XX_PTP_G8275p1_VARIANCE_UNLOCK;
      clck->pktRef = ZL303XX_TRUE;
      clck->ptp.config.defaultDS.slaveOnly = ZL303XX_TRUE;
      clck->ptp.config.profileStrict = ZL303XX_TRUE;
      clck->ptp.config.profileCfg.g8275p1.bypassEnabled = ZL303XX_FALSE;

      zlExampleApp.port[0].ptp.config.delayMechanism = ZL303XX_DELAY_MECHANISM_E2E;
      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_TRUE;

      zlExampleApp.stream[0].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
      zlExampleApp.stream[0].ptp.config.unicast = ZL303XX_TRUE;

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
      zlExampleApp.stream[0].ptp.config.logAnnounceInterval = log2PacketsPerSecond(zl303xx_GetAnnouncePps(0));
#else
/* #warning MSCC: Example stream config requires announce rate 1-8 pps */
      zlExampleApp.stream[0].ptp.config.logAnnounceInterval = 0;
#endif
      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}

/* examplePtpTelecomPhaseG8275p2Bc */
/**
   An example of how to configure a PTP boundary clock that uses unicast IPv4
   transport for G.8275.2.

   2 PTP ports are created;
   - the first creates a stream on which it will attempt to negotiate with the master;
   - the second port will create a stream when it receives a request from a slave.

 examplePtpTelecomPhaseG8275p2Master()
                           examplePtpTelecomPhaseG8275p2Bc()
                                                         examplePtpTelecomPhaseG8275p2Slave()

 +-------------+               +-----------------------+                +-------------+
 |   Unicast   |  Timestamped  |    Boundary Clock     |  Timestamped   |   Unicast   |
 |   Master    |<------------->|-----------------------|<-------------->|    Slave    |
 | 10.0.0.240  |    Packets    |  Unicast  |  Unicast  |    Packets     | 10.0.0.200  |
 | Class = 6   |   (Unicast)   |   Slave   |   Master  |   (Unicast)    | Class = 255 |
 +-------------+               |10.0.0.205 | 10.0.0.206|                +-------------+
                               |     Class = 187       |
                               +-----------+-----------+
                                  Port 1       Port 2

*******************************************************************************/
zlStatusE examplePtpTelecomPhaseG8275p2Bc(void)
{
   zlStatusE status = ZL303XX_OK;
   const char *srcIpv4Addr[2]  = {"10.0.0.205", "10.0.0.206"};
   const char *destIpv4Addr[1] = {"10.0.0.240"};

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
   zl303xx_SetPps(0, 16);
   zl303xx_SetAnnouncePps(0, 8);
#endif

#if defined APR_INCLUDED
   if (status == ZL303XX_OK)
   {
      /* Setup global configuration options */
      status |= zl303xx_AprSetPktRate(16, ZL303XX_TRUE);
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
      /* Set the Profile so that appropriate default values are initialized. */
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_TELECOM_G8275_2);
#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
       status = exampleAppStructInit(2,    /* 2 Clocks */
                                     2,    /* 2 Port   */
                                     1,    /* 1 Stream on Port 0 */
                                     &zlExampleApp);
#else
       status = exampleAppStructInit(1,    /* 1 Clock  */
                                     2,    /* 2 Port   */
                                     1,    /* 1 Stream on Port 0 */
                                     &zlExampleApp);
#endif
   }

   if (status == ZL303XX_OK)
   {
      Uint32T count;

      zlExampleApp.clock[0].ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8275_2;
      zlExampleApp.clock[0].ptp.config.clockType = ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_BC;
      zlExampleApp.clock[0].ptp.config.defaultDS.domainNumber = ZL303XX_PTP_G8275p2_DOMAIN_NUM_DEF;
      zlExampleApp.clock[0].ptp.config.defaultDS.priority1 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;
      zlExampleApp.clock[0].ptp.config.defaultDS.priority2 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;
      zlExampleApp.clock[0].ptp.config.defaultDS.clockQuality.clockClass = ZL303XX_PTP_G8275p1_CLASS_FREERUN;
      zlExampleApp.clock[0].ptp.config.defaultDS.clockQuality.clockAccuracy = ZL303XX_PTP_G8275p1_ACCURACY_UNLOCK;
      zlExampleApp.clock[0].ptp.config.defaultDS.clockQuality.offsetScaledLogVariance = ZL303XX_PTP_G8275p1_VARIANCE_UNLOCK;
      zlExampleApp.clock[0].pktRef = ZL303XX_TRUE;
      zlExampleApp.clock[0].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[0].ptp.config.profileStrict = ZL303XX_TRUE;
      zlExampleApp.clock[0].ptp.config.profileCfg.g8275p1.bypassEnabled = ZL303XX_FALSE;

      /* Port Config (2 ports) */
      for (count = 0; count < 2; count++)
      {
         zlExampleApp.port[count].ptp.config.delayMechanism = ZL303XX_DELAY_MECHANISM_E2E;
         zlExampleApp.port[count].ptp.config.unicastNegotiation.enabled = ZL303XX_TRUE;

         if (status == ZL303XX_OK)
         {
            status = examplePtpLocalIpv4Set(srcIpv4Addr[count], ZL303XX_FALSE, &zlExampleApp.port[count].ptp);
         }
      #if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
         if (count == 0)    /* Slave port */
             zl303xx_SetExampleInterface(1);
         else
             zl303xx_SetExampleInterface(zl303xx_GetExampleInterface() +1);

         setExampleIpSrc(srcIpv4Addr[count]);
      #else
          /* #warning MSCC: Set Source IPV4 address on ports here */
      #endif
      }

      /* Stream Config (only for the first stream - 2nd stream will be requested). */
      if (status == ZL303XX_OK )
      {
         zlExampleApp.stream[0].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
         zlExampleApp.stream[0].ptp.config.unicast = ZL303XX_TRUE;
         zlExampleApp.stream[0].ptp.config.portHandle = 0;  /* Slave stream on port 0. */

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
         zlExampleApp.stream[0].ptp.config.logSyncInterval = log2PacketsPerSecond(zl303xx_GetPps(0));
         zlExampleApp.stream[0].ptp.config.logAnnounceInterval = log2PacketsPerSecond(zl303xx_GetAnnouncePps(0));
#else
         /* #warning MSCC: Example stream config requires 16/16 and announce rate 8 pps */
         zlExampleApp.stream[0].ptp.config.logSyncInterval = -4;
         zlExampleApp.stream[0].ptp.config.logAnnounceInterval = -3;
#endif
         status = examplePtpDestIpv4Set(destIpv4Addr[0], ZL303XX_FALSE, &zlExampleApp.stream[0].ptp);
      }
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}

/* examplePtpTelecomPhaseG8275p2Bc1Port */
/**
   Configures the system for PTP Telecom Profile for Phase (G.8275.2) T-BC
   mode operation with a single PTP port and one Virtual Port. 
   This uses unicast transmission with IPv4.

   examplePtpTelecomPhaseG8275p2Master()
                                    examplePtpTelecomPhaseG8275p2Bc1Port()

       +-------------+                           +-------------+
       |   Unicast   |         Timestamped       |   Unicast   |
       |    T-GM     |<------------------------->|  T-BC Slave |<-- VP (GNSS)
       | 10.0.0.240  |          Packets          | 10.0.0.121  |
       | Class = 6   |         (Unicast)         | Class = 248 |
       +-------------+                           +-------------+

*******************************************************************************/
zlStatusE examplePtpTelecomPhaseG8275p2Bc1Port(void)
{
   zlStatusE status = ZL303XX_OK;
   exampleAppClockS *clck = NULL;
   const char *srcIpv4Addr = "10.0.0.121";
   const char *dstIpv4Addr = "10.0.0.240";

   zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_TELECOM_G8275_2); /* Used by exampleAppStructInit */

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
       status = exampleAppStructInit(1,    /* 1 Clock  */
                                     1,    /* 1 Port   */
                                     1,    /* 1 Stream */
                                     &zlExampleApp);
       clck = &zlExampleApp.clock[0];
   }

   if (status == ZL303XX_OK)
   {
      status = examplePtpLocalIpv4Set(srcIpv4Addr, ZL303XX_FALSE, &zlExampleApp.port[0].ptp);
      if (status == ZL303XX_OK)
      {
         status = examplePtpDestIpv4Set(dstIpv4Addr, ZL303XX_FALSE, &zlExampleApp.stream[0].ptp);
      }
    /* #warning MSCC: Set Source IPV4 address on port here */
   }

   if (status == ZL303XX_OK)
   {
      clck->ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8275_2;
      clck->ptp.config.clockType = ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_BC;
      clck->ptp.config.defaultDS.domainNumber = ZL303XX_PTP_G8275p2_DOMAIN_NUM_DEF;
      clck->ptp.config.defaultDS.priority1 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;
      clck->ptp.config.defaultDS.priority2 = ZL303XX_PTP_G8275p1_CLOCK_PRI2_MASTER;
      clck->ptp.config.defaultDS.clockQuality.clockClass = ZL303XX_PTP_G8275p1_CLASS_FREERUN;
      clck->ptp.config.defaultDS.clockQuality.clockAccuracy = ZL303XX_PTP_G8275p1_ACCURACY_UNLOCK;
      clck->ptp.config.defaultDS.clockQuality.offsetScaledLogVariance = ZL303XX_PTP_G8275p1_VARIANCE_UNLOCK;
      clck->pktRef = ZL303XX_TRUE;
      clck->ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      clck->ptp.config.profileStrict = ZL303XX_TRUE;
      clck->ptp.config.profileCfg.g8275p1.bypassEnabled = ZL303XX_FALSE;

      zlExampleApp.port[0].ptp.config.delayMechanism = ZL303XX_DELAY_MECHANISM_E2E;
      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_TRUE;

      zlExampleApp.stream[0].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
      zlExampleApp.stream[0].ptp.config.unicast = ZL303XX_TRUE;
      zlExampleApp.stream[0].ptp.config.logAnnounceInterval = 0;
      
      status = exampleAppStart(&zlExampleApp);
   }

   /* Add a Virtual port to the G.8275.1 Master. */
   if (status == ZL303XX_OK)
   {
      status = examplePtpTelecomPhaseG8275VirtualPortAdd(zlExampleApp.clock[TARGET_DPLL].ptp.handle);
   }
   
   return status;
}

/* examplePtpTelecomPhaseMasterIPv4 */
/**
   Configures the system for PTP Telecom Profile for Phase (G.8275.1) master
   mode operation. This uses multicast transmission with IPv4.

   Note: Multicast IPv4 is a non-standard mode of operation for this profile.

*******************************************************************************/
zlStatusE examplePtpTelecomPhaseMasterIPv4(void)
{
   zlStatusE status = ZL303XX_OK;
   exampleAppClockS *clck = NULL;
   const char *srcIpv4Addr = "10.0.0.121";
   const char *destIpv4Addr = DEFAULT_PTP_DOMAIN_ADDRESS_IPV4;

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
   status = zl303xx_AprSetDeviceOptMode(ZL303XX_ELECTRIC_MODE);
   zl303xx_SetExampleInterface(2);
   zl303xx_SetDeviceMode(ZL303XX_MODE_REF_EXTERNAL);
   zl303xx_SetPps(0, 16);
   zl303xx_SetAnnouncePps(0,8);
#endif
   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      /* Set the Profile so that appropriate default values are initialized. */
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_TELECOM_G8275_1);
#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
       status = exampleAppStructInit(2,    /* 2 Clocks */
                                     1,    /* 1 Port   */
                                     1,    /* 1 Stream */
                                     &zlExampleApp);
       clck = &zlExampleApp.clock[TARGET_DPLL];
#else
       status = exampleAppStructInit(1,    /* 1 Clock  */
                                     1,    /* 1 Port   */
                                     1,    /* 1 Stream */
                                     &zlExampleApp);
       clck = &zlExampleApp.clock[0];
#endif
   }

   if (status == ZL303XX_OK)
   {
      status = examplePtpLocalIpv4Set(srcIpv4Addr, ZL303XX_FALSE, &zlExampleApp.port[0].ptp);
      if (status == ZL303XX_OK)
      {
         status = examplePtpDestIpv4Set(destIpv4Addr, ZL303XX_FALSE, &zlExampleApp.stream[0].ptp);
      }
#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
      setExampleIpSrc(srcIpv4Addr);
#else
    /* #warning MSCC: Set Source IPV4 address on port here */
#endif
   }

   if (status == ZL303XX_OK)
   {
      clck->ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8275_1;
      clck->ptp.config.clockType = ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_GM;
      clck->ptp.config.defaultDS.domainNumber = ZL303XX_PTP_G8275p1_DOMAIN_NUM_DEF;
      clck->ptp.config.defaultDS.priority1 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;
      clck->ptp.config.defaultDS.priority2 = ZL303XX_PTP_G8275p1_CLOCK_PRI2_MASTER;
      clck->ptp.config.defaultDS.clockQuality.clockClass = ZL303XX_PTP_G8275p1_CLASS_FREERUN;
      clck->ptp.config.defaultDS.clockQuality.clockAccuracy = ZL303XX_PTP_G8275p1_ACCURACY_UNLOCK;
      clck->ptp.config.defaultDS.clockQuality.offsetScaledLogVariance = ZL303XX_PTP_G8275p1_VARIANCE_UNLOCK;
      clck->pktRef = ZL303XX_FALSE;
      clck->ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      clck->ptp.config.profileStrict = ZL303XX_TRUE;
      clck->ptp.config.profileCfg.g8275p1.bypassEnabled = ZL303XX_FALSE;

      zlExampleApp.port[0].ptp.config.delayMechanism = ZL303XX_DELAY_MECHANISM_E2E;
      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_FALSE;

      zlExampleApp.stream[0].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
      zlExampleApp.stream[0].ptp.config.unicast = ZL303XX_FALSE;

 #if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
      zlExampleApp.stream[0].ptp.config.logSyncInterval = log2PacketsPerSecond(zl303xx_GetPps(0));
      zlExampleApp.stream[0].ptp.config.logMinDelayReqInterval = log2PacketsPerSecond(zl303xx_GetDelayReqPps(0));
      zlExampleApp.stream[0].ptp.config.logAnnounceInterval = log2PacketsPerSecond(zl303xx_GetAnnouncePps(0));
#else
/* #warning MSCC: Example stream config requires 16/16 and announce rate 8 pps */
      zlExampleApp.stream[0].ptp.config.logSyncInterval = -4;
      zlExampleApp.stream[0].ptp.config.logAnnounceInterval = -3;
#endif
     status = exampleAppStart(&zlExampleApp);
   }

   /* Add a Virtual port to the G.8275.1 Master. */
   if (status == ZL303XX_OK)
   {
      status = examplePtpTelecomPhaseG8275VirtualPortAdd(zlExampleApp.clock[TARGET_DPLL].ptp.handle);
   }

   return status;
}

/* examplePtpTelecomPhaseBcIPv4 */
/**
   An example of how to configure a PTP boundary clock that uses multicast IPv4
   transport for G.8275.1. 2 PTP ports are created, each with a single stream.

   Note: Multicast IPv4 is a non-standard mode of operation for this profile.



 examplePtpTelecomPhaseMasterIPv4()
                             examplePtpTelecomPhaseBcIPv4()
                                                              examplePtpTelecomPhaseBcSlaveIPv4()

 +-------------+               +-----------------------+                +-------------+
 |  Multicast  |  Timestamped  |    Boundary Clock     |  Timestamped   |  Multicast  |
 |   Master    |<------------->|-----------------------|<-------------->|    Slave    |
 | 10.0.0.121  |    Packets    | Multicast | Multicast |    Packets     | 10.0.1.207  |
 | Class = 10  |  (Multicast)  |   Slave   |   Master  |  (Multicast)   | Class = 255 |
 +-------------+    ( .129 )   |10.0.0.205 | 10.0.1.206|    ( .130 )    +-------------+
                               |     Class = 187       |
                               +-----------+-----------+
                                  Port 1       Port 2


*******************************************************************************/
zlStatusE examplePtpTelecomPhaseBcIPv4(void)
{
   zlStatusE status = ZL303XX_OK;
   const char *srcIpv4Addr[2]  = {"10.0.0.205", "10.0.1.206"};
   const char *destIpv4Addr[2] = {DEFAULT_PTP_DOMAIN_ADDRESS_IPV4,
                                  ALTERNATE_PTP_DOMAIN1_ADDRESS_IPV4};

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
   zl303xx_SetPps(0, 16);
   zl303xx_SetAnnouncePps(0, 8);
#endif

#if defined APR_INCLUDED
   if (status == ZL303XX_OK)
   {
      /* Setup global configuration options */
      status |= zl303xx_AprSetPktRate(16, ZL303XX_TRUE);
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
      /* Set the Profile so that appropriate default values are initialized. */
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_TELECOM_G8275_1);
#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
       status = exampleAppStructInit(2,    /* 2 Clocks */
                                     2,    /* 2 Port   */
                                     2,    /* 2 Stream */
                                     &zlExampleApp);
#else
       status = exampleAppStructInit(1,    /* 1 Clock  */
                                     2,    /* 2 Port   */
                                     2,    /* 2 Stream */
                                     &zlExampleApp);
#endif
   }

   if (status == ZL303XX_OK)
   {
      Uint32T count;

      zlExampleApp.clock[TARGET_DPLL].ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8275_1;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.clockType = ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_BC;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.domainNumber = ZL303XX_PTP_G8275p1_DOMAIN_NUM_DEF;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.priority1 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.priority2 = ZL303XX_PTP_G8275p1_CLOCK_PRI2_MASTER;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = ZL303XX_PTP_G8275p1_CLASS_FREERUN;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockAccuracy = ZL303XX_PTP_G8275p1_ACCURACY_UNLOCK;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.offsetScaledLogVariance = ZL303XX_PTP_G8275p1_VARIANCE_UNLOCK;
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profileStrict = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profileCfg.g8275p1.bypassEnabled = ZL303XX_FALSE;

      /* Port Config */
      for (count = 0; count < 2; count++)
      {
         zlExampleApp.port[count].ptp.config.delayMechanism = ZL303XX_DELAY_MECHANISM_E2E;
         zlExampleApp.port[count].ptp.config.unicastNegotiation.enabled = ZL303XX_FALSE;

         if (status == ZL303XX_OK)
         {
            status = examplePtpLocalIpv4Set(srcIpv4Addr[count], ZL303XX_TRUE, &zlExampleApp.port[count].ptp);
         }
      #if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
         if (count == 0)    /* Slave port */
             zl303xx_SetExampleInterface(1);
         else
             zl303xx_SetExampleInterface(zl303xx_GetExampleInterface() +1);

         setExampleIpSrc(srcIpv4Addr[count]);
      #else
          /* #warning MSCC: Set Source IPV4 address on ports here */
      #endif
      }

      /* Stream Config */
      for (count = 0; count < 2; count++)
      {
         zlExampleApp.stream[count].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
         zlExampleApp.stream[count].ptp.config.unicast = ZL303XX_FALSE;
         zlExampleApp.stream[count].ptp.config.portHandle = count;  /* One stream per port. */

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
         zlExampleApp.stream[count].ptp.config.logSyncInterval = log2PacketsPerSecond(zl303xx_GetPps(0));
         zlExampleApp.stream[count].ptp.config.logAnnounceInterval = log2PacketsPerSecond(zl303xx_GetAnnouncePps(0));
#else
         /* #warning MSCC: Example stream config requires 16/16 and announce rate 8 pps */
         zlExampleApp.stream[count].ptp.config.logSyncInterval = -4;
         zlExampleApp.stream[count].ptp.config.logAnnounceInterval = -3;
#endif
         if (status == ZL303XX_OK )
         {
            status = examplePtpDestIpv4Set(destIpv4Addr[count], ZL303XX_TRUE, &zlExampleApp.stream[count].ptp);
         }
      }
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStart(&zlExampleApp);
   }


   return status;
}

/* examplePtpTelecomPhaseSlaveIPv4 */
/**
   Configures the system for PTP Telecom Profile for Phase (G.8275.1) master
   mode operation. This uses multicast transmission with IPv4.

   Note: Multicast IPv4 is a non-standard mode of operation for this profile.

  examplePtpTelecomPhaseMasterIPv4()      examplePtpTelecomPhaseSlaveIPv4()

       +-------------+                           +-------------+
       |  Multicast  |         Timestamped       |  Multicast  |
       |   Master    |<------------------------->|    Slave    |
       | 10.0.0.121  |          Packets          | 10.0.0.161  |
       | Class = 6   |        (Multicast)        | Class = 255 |
       +-------------+         ( .129 )          +-------------+



*******************************************************************************/
zlStatusE examplePtpTelecomPhaseSlaveIPv4(void)
{
   zlStatusE status = ZL303XX_OK;
   exampleAppClockS *clck = NULL;
   const char *srcIpv4Addr = "10.0.0.161";
   const char *destIpv4Addr = DEFAULT_PTP_DOMAIN_ADDRESS_IPV4;

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
   zl303xx_SetPps(0, 16);
   zl303xx_SetDelayReqPps(0, 16);
   zl303xx_SetAnnouncePps(0, 8);
#endif

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      /* Set the Profile so that appropriate default values are initialized. */
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_TELECOM_G8275_1);
#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
       status = exampleAppStructInit(2,    /* 2 Clocks */
                                     1,    /* 1 Port   */
                                     1,    /* 1 Stream */
                                     &zlExampleApp);
       clck = &zlExampleApp.clock[TARGET_DPLL];
#else
       status = exampleAppStructInit(1,    /* 1 Clock  */
                                     1,    /* 1 Port   */
                                     1,    /* 1 Stream */
                                     &zlExampleApp);
       clck = &zlExampleApp.clock[0];
#endif
   }

   if (status == ZL303XX_OK)
   {
      status = examplePtpLocalIpv4Set(srcIpv4Addr, ZL303XX_FALSE, &zlExampleApp.port[0].ptp);
      if (status == ZL303XX_OK)
      {
         status = examplePtpDestIpv4Set(destIpv4Addr, ZL303XX_FALSE, &zlExampleApp.stream[0].ptp);
      }
   #if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
      setExampleIpSrc(srcIpv4Addr);
   #else
      /* #warning MSCC: Set Source IPV4 address on port here */
   #endif
   }

   if (status == ZL303XX_OK)
   {
      clck->ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8275_1;
      clck->ptp.config.clockType = ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_TSC;
      clck->ptp.config.defaultDS.domainNumber = ZL303XX_PTP_G8275p1_DOMAIN_NUM_DEF;
      clck->ptp.config.defaultDS.priority1 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;
      clck->ptp.config.defaultDS.priority2 = ZL303XX_PTP_G8275p1_CLOCK_PRI2_SLAVE;
      clck->ptp.config.defaultDS.clockQuality.clockClass = ZL303XX_PTP_G8275p1_CLASS_SLAVE;
      clck->ptp.config.defaultDS.clockQuality.clockAccuracy = ZL303XX_PTP_G8275p1_ACCURACY_UNLOCK;
      clck->ptp.config.defaultDS.clockQuality.offsetScaledLogVariance = ZL303XX_PTP_G8275p1_VARIANCE_UNLOCK;
      clck->pktRef = ZL303XX_TRUE;
      clck->ptp.config.defaultDS.slaveOnly = ZL303XX_TRUE;
      clck->ptp.config.profileStrict = ZL303XX_TRUE;
      clck->ptp.config.profileCfg.g8275p1.bypassEnabled = ZL303XX_FALSE;

      zlExampleApp.port[0].ptp.config.delayMechanism = ZL303XX_DELAY_MECHANISM_E2E;
      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_FALSE;

      zlExampleApp.stream[0].ptp.config.unicast = ZL303XX_FALSE;
      zlExampleApp.stream[0].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
      zlExampleApp.stream[0].ptp.config.logSyncInterval = log2PacketsPerSecond(zl303xx_GetPps(0));
      zlExampleApp.stream[0].ptp.config.logMinDelayReqInterval = log2PacketsPerSecond(zl303xx_GetDelayReqPps(0));
      zlExampleApp.stream[0].ptp.config.logAnnounceInterval = log2PacketsPerSecond(zl303xx_GetAnnouncePps(0));
#else
/* #warning MSCC: Example stream config requires 16/16 and announce rate 8 pps */
      zlExampleApp.stream[0].ptp.config.logSyncInterval = -4;
      zlExampleApp.stream[0].ptp.config.logMinDelayReqInterval = -4;
      zlExampleApp.stream[0].ptp.config.logAnnounceInterval = -3;
#endif

      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}

/* examplePtpTelecomPhaseBcSlaveIPv4 */
/**
   An example of how to configure a PTP slave clock that uses multicast
   transport for G.8275.1 and connects to a Boundary Clock.

   Note: Multicast IPv4 is a non-standard mode of operation for this profile.

   To be used with the following examples:
   - examplePtpTelecomPhaseMasterIPv4()
   - examplePtpTelecomPhaseBcIPv4()
   - examplePtpTelecomPhaseBcSlaveIPv4()

*******************************************************************************/
zlStatusE examplePtpTelecomPhaseBcSlaveIPv4(void)
{
   zlStatusE status = ZL303XX_OK;
   exampleAppClockS *clck = NULL;
   const char *srcIpv4Addr = "10.0.1.207";
   const char *destIpv4Addr = ALTERNATE_PTP_DOMAIN1_ADDRESS_IPV4;

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
   zl303xx_SetPps(0, 16);
   zl303xx_SetDelayReqPps(0, 16);
   zl303xx_SetAnnouncePps(0, 8);
#endif

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      /* Set the Profile so that appropriate default values are initialized. */
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_TELECOM_G8275_1);
#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
       status = exampleAppStructInit(2,    /* 2 Clocks */
                                     1,    /* 1 Port   */
                                     1,    /* 1 Stream */
                                     &zlExampleApp);
       clck = &zlExampleApp.clock[TARGET_DPLL];
#else
       status = exampleAppStructInit(1,    /* 1 Clock  */
                                     1,    /* 1 Port   */
                                     1,    /* 1 Stream */
                                     &zlExampleApp);
       clck = &zlExampleApp.clock[0];
#endif
   }

   if (status == ZL303XX_OK)
   {
      status = examplePtpLocalIpv4Set(srcIpv4Addr, ZL303XX_FALSE, &zlExampleApp.port[0].ptp);
      if (status == ZL303XX_OK)
      {
         status = examplePtpDestIpv4Set(destIpv4Addr, ZL303XX_FALSE, &zlExampleApp.stream[0].ptp);
      }
   #if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
      setExampleIpSrc(srcIpv4Addr);
   #else
      /* #warning MSCC: Set Source IPV4 address on port here */
   #endif
   }

   if (status == ZL303XX_OK)
   {
      clck->ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8275_1;
      clck->ptp.config.clockType = ZL303XX_PTP_G8275p1_CLOCK_TYPE_T_TSC;
      clck->ptp.config.defaultDS.domainNumber = ZL303XX_PTP_G8275p1_DOMAIN_NUM_DEF;
      clck->ptp.config.defaultDS.priority1 = ZL303XX_PTP_G8275p1_CLOCK_PRI1_DEF;
      clck->ptp.config.defaultDS.priority2 = ZL303XX_PTP_G8275p1_CLOCK_PRI2_SLAVE;
      clck->ptp.config.defaultDS.clockQuality.clockClass = ZL303XX_PTP_G8275p1_CLASS_SLAVE;
      clck->ptp.config.defaultDS.clockQuality.clockAccuracy = ZL303XX_PTP_G8275p1_ACCURACY_UNLOCK;
      clck->ptp.config.defaultDS.clockQuality.offsetScaledLogVariance = ZL303XX_PTP_G8275p1_VARIANCE_UNLOCK;
      clck->pktRef = ZL303XX_TRUE;
      clck->ptp.config.defaultDS.slaveOnly = ZL303XX_TRUE;
      clck->ptp.config.profileStrict = ZL303XX_TRUE;
      clck->ptp.config.profileCfg.g8275p1.bypassEnabled = ZL303XX_FALSE;

      zlExampleApp.port[0].ptp.config.delayMechanism = ZL303XX_DELAY_MECHANISM_E2E;
      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_FALSE;

      zlExampleApp.stream[0].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
      zlExampleApp.stream[0].ptp.config.unicast = ZL303XX_FALSE;

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
      zlExampleApp.stream[0].ptp.config.logSyncInterval = log2PacketsPerSecond(zl303xx_GetPps(0));
      zlExampleApp.stream[0].ptp.config.logMinDelayReqInterval = log2PacketsPerSecond(zl303xx_GetDelayReqPps(0));
      zlExampleApp.stream[0].ptp.config.logAnnounceInterval = log2PacketsPerSecond(zl303xx_GetAnnouncePps(0));
#else
/* #warning MSCC: Example stream config requires 16/16 and announce rate 8 pps */
      zlExampleApp.stream[0].ptp.config.logSyncInterval = -4;
      zlExampleApp.stream[0].ptp.config.logMinDelayReqInterval = -4;
      zlExampleApp.stream[0].ptp.config.logAnnounceInterval = -3;
#endif
      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}


/* examplePtpTelecomPhaseEthInterfaceStrSet */
/**
   Configures the PTP port to be associated with a particular Ethernet Interface.

  Parameters:
   [in] pZlExApp - Pointer to the current example context
   [in] ifName- The name of the ethernet interface
   [in] portNum - The PTP port number to be associated with the interface

  Return Value:
   zlStatusE

  Notes:
   Should only be called by examplePtpTelecomPhaseG8275p1Bc2Ifs and BC variants.
 */
zlStatusE examplePtpTelecomPhaseEthInterfaceStrSet(exampleAppS *pZlExApp, const char *ifName, Uint32T portNum)
{
   zl303xx_PortAddress localAddr;
   char localAddrStr[32];
   zlStatusE status = ZL303XX_OK;

   /* Obtain Local Port address from OS */
   if((status = osGetMacAddr(ifName, localAddr.addressField)) != 0)
   {
      printf("ERROR: Could not get MAC address for interface \"%s\", status %d\n", ifName, status);
      return status;
   }
   localAddr.networkProtocol = ZL303XX_NETWORK_PROTOCOL_802_3;
   localAddr.addressLength = 6;

   if((status = examplePtpSocketPortAddressToStr(&localAddr, localAddrStr, sizeof(localAddrStr))) != ZL303XX_OK)
   {
      printf("ERROR: Could not convert MAC address for interface \"%s\", status %d\n", ifName, status);
      return status;
   }

   /* Setup the Port's local address */
   examplePtpLocalIpv4Set(localAddrStr, ZL303XX_FALSE, &pZlExApp->port[portNum].ptp);

   /* Setup the desired socket interface */
   pZlExApp->port[portNum].exampleInterface = ZL303XX_EXAMPLE_INTERFACE_CUSTOM_STR;

   strncpy(pZlExApp->port[portNum].exampleInterfaceCustomStr, ifName, sizeof(pZlExApp->port[portNum].exampleInterfaceCustomStr));
   pZlExApp->port[portNum].exampleInterfaceCustomStr[sizeof(pZlExApp->port[portNum].exampleInterfaceCustomStr)-1] = '\0';

   return status;
}

/*****************   END   ****************************************************/
