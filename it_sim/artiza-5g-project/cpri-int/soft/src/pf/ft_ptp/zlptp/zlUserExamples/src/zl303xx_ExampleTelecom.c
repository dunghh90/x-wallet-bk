

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Example code to initialize the device as an PTP Telecom Profile
*     master or slave.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_ExamplePtpGlobals.h"
#include "zl303xx_Ptpd.h"
#include "zl303xx_ExampleUtils.h"
#if defined APR_INCLUDED
#include "zl303xx_ExampleAprGlobals.h"
#endif

/*****************   DEFINES     **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   IMPORTED GLOBAL VARIABLES   ******************************/
extern Uint8T exampleMaster2MacAddr[];
extern Uint8T exampleMaster1IPv6Addr[];
extern Uint8T exampleMaster2IPv6Addr[];
extern Uint8T exampleSlave1IPv6Addr[];
extern Uint8T exampleSlave2IPv6Addr[];
extern Uint8T const *exampleDestMacAddr;

#if defined ZLS30390_INCLUDED
extern Uint8T TARGET_DPLL;
#endif

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

#if defined ZLS30390_INCLUDED
/* examplePtpTelecomMaster */
/**
   Configures the system for PTP Telecom Profile master mode operation. This
   uses unicast negotiation and a clock class of 90.

*******************************************************************************/
zlStatusE examplePtpTelecomMaster(void)
{
   zlStatusE status = ZL303XX_OK;

   const char *srcIpv4Addr = "10.0.0.240";

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      /* Set the Profile so that appropriate default values are initialized. */
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_TELECOM_G8265_1);
      status = exampleAppStructInit(1, 1, 0, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
      status = examplePtpLocalIpv4Set(srcIpv4Addr, ZL303XX_FALSE, &zlExampleApp.port[0].ptp);
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8265_1;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      /*zlExampleApp.clock[TARGET_DPLL].ptp.config.bmca.revertiveEn = ZL303XX_TRUE;*/
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.domainNumber = 4;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = 90;

      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_TRUE;

      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}

/* examplePtpTelecomMaster2 */
/**
   Configures the system for PTP Telecom Profile master mode operation. This
   uses unicast negotiation and a clock class of 90.

*******************************************************************************/
zlStatusE examplePtpTelecomMaster2(void)
{
   zlStatusE status = ZL303XX_OK;

   const char *srcIpv4Addr = "10.0.0.241";

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      /* Set the Profile so that appropriate default values are initialized. */
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_TELECOM_G8265_1);
      status = exampleAppStructInit(1, 1, 0, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
      status = examplePtpLocalIpv4Set(srcIpv4Addr, ZL303XX_FALSE, &zlExampleApp.port[0].ptp);
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8265_1;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.domainNumber = 4;
    /*  zlExampleApp.clock[TARGET_DPLL].ptp.config.bmca.revertiveEn = ZL303XX_TRUE; */
      /* Set the clock class of this master the same as examplePtpTelecomMaster. The
       * slave will select a master to lock to based on local priority value. */
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = 90;

      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_TRUE;

      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}


/* examplePtpTelecomSlave */
/**
   Configures the system for PTP Telecom Profile slave mode operation. This
   uses unicast negotiation and enables BMCA to select the best master.
   Two potential masters are added to the slave's Unicast Master Table.

*******************************************************************************/
zlStatusE examplePtpTelecomSlave(void)
{
   zlStatusE status = ZL303XX_OK;

   const char *srcIpv4Addr = "10.0.0.249";
   const char *destIpv4Addr[2] = {"10.0.0.240", "10.0.0.241"};

   const Uint8T priorityStream0 = 50;
   const Uint8T priorityStream1 = 40;

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
      /* Set the Profile so that appropriate default values are initialized. */
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_TELECOM_G8265_1);
      status = exampleAppStructInit(1, 1, 2, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
      status = examplePtpLocalIpv4Set(srcIpv4Addr, ZL303XX_FALSE, &zlExampleApp.port[0].ptp);
   }

   if (status == ZL303XX_OK)
   {
      Uint32T count;

      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8265_1;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.domainNumber = 4;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = 255;

      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_TRUE;

      for (count = 0; count < 2; count++)
      {
         zlExampleApp.stream[count].ptp.config.mode = ZL303XX_PTP_STREAM_SLAVE_ONLY;
         zlExampleApp.stream[count].ptp.config.unicast = ZL303XX_TRUE;

         status |= examplePtpDestIpv4Set(destIpv4Addr[count], ZL303XX_TRUE,
                                         &zlExampleApp.stream[count].ptp);
      }
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStart(&zlExampleApp);
   }

   /* Request Announce, Sync, and Delay_Resp service from both masters. */
   if (status == ZL303XX_OK)
   {
      status = examplePtpContractsRequest(&zlExampleApp.stream[0].ptp,
                  (1 << ZL303XX_MSG_ID_ANNOUNCE) | (1 << ZL303XX_MSG_ID_SYNC) | (1 << ZL303XX_MSG_ID_DELAY_RESP));
   }

   if (status == ZL303XX_OK)
   {
      status = examplePtpContractsRequest(&zlExampleApp.stream[1].ptp,
                  (1 << ZL303XX_MSG_ID_ANNOUNCE) | (1 << ZL303XX_MSG_ID_SYNC) | (1 << ZL303XX_MSG_ID_DELAY_RESP));
   }

   /* Give master_1 a higher priority than master_0 because their clock classes
    * are equal. */
   if (status == ZL303XX_OK)
   {
      status = examplePtpUmtEntryPrioritySet(&zlExampleApp.stream[0].ptp, priorityStream0) |
               examplePtpUmtEntryPrioritySet(&zlExampleApp.stream[1].ptp, priorityStream1);
   }

   return status;
}


/* examplePtpTelecomBCMaster */
/**
   Configures the system for PTP Telecom Profile master mode operation. This
   uses unicast negotiation and a clock class of 90. This example is intended to
   be used with examplePtpTelecomBC().

*******************************************************************************/
zlStatusE examplePtpTelecomBCMaster(void)
{
   zlStatusE status = ZL303XX_OK;
   const char *srcIpv4Addr = "10.0.0.246";

   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      /* Set the Profile so that appropriate default values are initialized. */
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_TELECOM_G8265_1);
      status = exampleAppStructInit(1, 1, 0, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
      status = examplePtpLocalIpv4Set(srcIpv4Addr, ZL303XX_FALSE, &zlExampleApp.port[0].ptp);
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8265_1;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.domainNumber = 4;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = 90;

      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_TRUE;

      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}

/* examplePtpTelecomBC */
/**
   Configures the system for PTP Telecom Profile boundary clock operation.
   Boundary clock is a prohibited node type in the G.8265.1 standard, but the
   API does not restrict this mode from being used. The G.8265.1 BMCA is used
   to select the best master.

   This example is intended to be used with examplePtpTelecomBCMaster() and
   examplePtpTelecomBCSlave().

*******************************************************************************/
zlStatusE examplePtpTelecomBC(void)
{
   zlStatusE status = ZL303XX_OK;
   const char *srcIpv4Addr[2]  = {"10.0.0.247", "10.0.0.248"};
   const char *destIpv4Addr[2] = {"10.0.0.246", "10.0.0.249"};
   Uint32T count;
   Uint8T priority = 50;

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
      /* Set the Profile so that appropriate default values are initialized. */
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_TELECOM_G8265_1);
      status = exampleAppStructInit(1, 2, 2, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8265_1;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.domainNumber = 4;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = 150;

      for (count = 0; count < 2; count++)
      {
         zlExampleApp.port[count].ptp.config.unicastNegotiation.enabled = ZL303XX_TRUE;
         status |= examplePtpLocalIpv4Set(srcIpv4Addr[count], ZL303XX_TRUE,
                                          &zlExampleApp.port[count].ptp);

         zlExampleApp.stream[count].ptp.config.portHandle = count;
         zlExampleApp.stream[count].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
         zlExampleApp.stream[count].ptp.config.unicast = ZL303XX_TRUE;
         status |= examplePtpDestIpv4Set(destIpv4Addr[count], ZL303XX_TRUE,
                                         &zlExampleApp.stream[count].ptp);
      }
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStart(&zlExampleApp);
   }


   /* Request Announce, Sync, and Delay_Resp service from both potential masters. */
   if (status == ZL303XX_OK)
   {
      status = examplePtpContractsRequest(&zlExampleApp.stream[0].ptp,
                  (1 << ZL303XX_MSG_ID_ANNOUNCE) | (1 << ZL303XX_MSG_ID_SYNC) | (1 << ZL303XX_MSG_ID_DELAY_RESP));
   }

   if (status == ZL303XX_OK)
   {
      status = examplePtpContractsRequest(&zlExampleApp.stream[1].ptp,
                  (1 << ZL303XX_MSG_ID_ANNOUNCE) | (1 << ZL303XX_MSG_ID_SYNC) | (1 << ZL303XX_MSG_ID_DELAY_RESP));
   }

   /* Give both entries the same priority, so the boundary clock self-configures
    * based on clockClass. */
   if (status == ZL303XX_OK)
   {
      status = examplePtpUmtEntryPrioritySet(&zlExampleApp.stream[0].ptp, priority) |
               examplePtpUmtEntryPrioritySet(&zlExampleApp.stream[1].ptp, priority);
   }

   return status;
}

/* examplePtpTelecomBCSlave */
/**
   Configures the system for PTP Telecom Profile slave mode operation. This
   uses the non-standard (for G.8265.1) clockClass of 160, so it will self-
   configure to become a slave.

   This example is intended to be used with examplePtpTelecomBC().

*******************************************************************************/
zlStatusE examplePtpTelecomBCSlave(void)
{
   zlStatusE status = ZL303XX_OK;
   const char *srcIpv4Addr  = "10.0.0.249";
   const char *destIpv4Addr = "10.0.0.248";

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
      /* Set the Profile so that appropriate default values are initialized. */
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_TELECOM_G8265_1);
      status = exampleAppStructInit(1, 1, 1, &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
      status = examplePtpLocalIpv4Set(srcIpv4Addr, ZL303XX_FALSE, &zlExampleApp.port[0].ptp);
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profile = ZL303XX_PTP_PROFILE_TELECOM_G8265_1;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.domainNumber = 4;
      /* Use a clock class reserved in IEEE 1588-2008 for alternate PTP profiles. */
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass = 160;

      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_TRUE;

      zlExampleApp.stream[0].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
      zlExampleApp.stream[0].ptp.config.unicast = ZL303XX_TRUE;

      status = examplePtpDestIpv4Set(destIpv4Addr, ZL303XX_FALSE,
                                     &zlExampleApp.stream[0].ptp);
   }

   if (status == ZL303XX_OK)
   {
      status = exampleAppStart(&zlExampleApp);
   }


   /* Request Announce, Sync, and Delay_Resp service from the master. */
   if (status == ZL303XX_OK)
   {
      status = examplePtpContractsRequest(&zlExampleApp.stream[0].ptp,
                  (1 << ZL303XX_MSG_ID_ANNOUNCE) | (1 << ZL303XX_MSG_ID_SYNC) | (1 << ZL303XX_MSG_ID_DELAY_RESP));
   }

   return status;
}
#endif

/*****************   END   ****************************************************/
