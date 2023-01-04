

/*******************************************************************************
 *
 *  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
 *
 *  Copyright 2006-2018 Microsemi Semiconductor Limited.
 *  All rights reserved.
 *
 *  Module Description:
 *     Example file showing how to configure a device as a slave or master for
 *     Peer-Delay operation.
 *
 ******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Error.h"

#include "zl303xx_PtpApi.h"
#include "zl303xx_ExamplePtpGlobals.h"
#include "zl303xx_ExampleUtils.h"

#include "zl303xx_ExamplePeerDelay.h"


/*****************   DEFINES     **********************************************/

/* IP Address catalog. */
#define DEFAULT_PEER_MASTER_1_IP       "10.0.0.121"
#define DEFAULT_PEER_MASTER_2_IP       "10.0.0.122"

#define DEFAULT_PEER_BC_1_IP           "10.0.0.141"
#define DEFAULT_PEER_BC_2_IP           "10.0.0.142"

#define DEFAULT_PEER_SLAVE_1_IP        "10.0.0.161"
#define DEFAULT_PEER_SLAVE_2_IP        "10.0.0.162"

/*****************   DATATYPES   **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   EXTERN GLOBAL VARIABLES   ********************************/
#if defined ZLS30390_INCLUDED
extern Uint8T TARGET_DPLL;
#endif

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/


#if defined ZLS30390_INCLUDED 

/* examplePtpPeerDelayMaster */
/**
   Configures the system for PTP Peer Delay Profile Master mode operation.
   This uses multicast transmission with 802_3 (ETH LAyer 2).

*******************************************************************************/
zlStatusE examplePtpPeerDelayMaster(void)
{
   zlStatusE status = ZL303XX_OK;
   Uint8T count;

   const char *srcIpv4Addr = "10.0.0.121";
   const char *destIpv4Addr[2] = {(const char *)Zl303xx_PtpPdelayDomainAddr802_3,
                                  (const char *)Zl303xx_PtpDefaultDomainAddr802_3};

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      /* Set the Profile so that appropriate default values are initialized. */
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_DEFAULT_P2P);
      status = exampleAppStructInit(1,    /* 1 Clock  */
                                    1,    /* 1 Port   */
                                    2,    /* 1 Stream */
                                    &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
      /* Local Port address is IPv4 */
      status = examplePtpLocalIpv4Set(srcIpv4Addr, ZL303XX_FALSE, &zlExampleApp.port[0].ptp);
   }

   if (status == ZL303XX_OK)
   {
      /* Set profile and use defaults. */
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profile = ZL303XX_PTP_PROFILE_DEFAULT_P2P;
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_FALSE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;

      zlExampleApp.port[0].ptp.config.delayMechanism = ZL303XX_DELAY_MECHANISM_P2P;
      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_FALSE;

      /* Stream Config */
      for (count = 0; count < 2; count++)
      {
         zlExampleApp.stream[count].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
         zlExampleApp.stream[count].ptp.config.unicast = ZL303XX_FALSE;
         zlExampleApp.stream[count].ptp.config.portHandle = 0;  /* 2 streams on 1 port. */

         /* Destination addresses are the 802.3 values. */
         status |= examplePtpDestEthL2Set(destIpv4Addr[count], ZL303XX_TRUE,
                                            &zlExampleApp.stream[count].ptp);
      }

      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}

/* examplePtpPeerDelaySlave */
/**
   Configures the system for PTP Peer Delay Slave mode operation.
   This uses multicast transmission with 802_3 (ETH LAyer 2).

  examplePtpPeerDelayMaster()         examplePtpPeerDelaySlave()

       +-------------+            Timestamped          +-------------+
       |             |            (SYNC,ANNC)          |             |
       |  Multicast  |           Routable 802.3        |  Multicast  |
       |   Master    |<------------------------------->|    Slave    |
       | 10.0.0.121  |                                 | 10.0.0.161  |
       | Class = 187 |<------------------------------->| Class = 255 |
       |             |            (PEER-DELAY)         |             |
       +-------------+         Non-Routable 802.3      +-------------+



*******************************************************************************/
zlStatusE examplePtpPeerDelaySlave(void)
{
   zlStatusE status = ZL303XX_OK;
   Uint8T count;

   const char *srcIpv4Addr = "10.0.0.161";
   const char *destIpv4Addr[2] = {(const char *)Zl303xx_PtpPdelayDomainAddr802_3,
                                  (const char *)Zl303xx_PtpDefaultDomainAddr802_3};

   /* Initialize global environment variables. */
   if (status == ZL303XX_OK)
   {
      status = exampleEnvInit();
   }

   if (status == ZL303XX_OK)
   {
      /* Set the Profile so that appropriate default values are initialized. */
      zl303xx_SetPtpProfile(ZL303XX_PTP_PROFILE_DEFAULT_P2P);
      status = exampleAppStructInit(1,    /* 1 Clock  */
                                    1,    /* 1 Port   */
                                    2,    /* 2 Stream */
                                    &zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
      /* Local Port address is IPv4 */
      status = examplePtpLocalIpv4Set(srcIpv4Addr, ZL303XX_FALSE, &zlExampleApp.port[0].ptp);
   }

   if (status == ZL303XX_OK)
   {
      zlExampleApp.clock[TARGET_DPLL].ptp.config.profile = ZL303XX_PTP_PROFILE_DEFAULT_P2P;
      zlExampleApp.clock[TARGET_DPLL].pktRef = ZL303XX_TRUE;
      zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.slaveOnly = ZL303XX_TRUE;

      zlExampleApp.port[0].ptp.config.delayMechanism = ZL303XX_DELAY_MECHANISM_P2P;
      zlExampleApp.port[0].ptp.config.unicastNegotiation.enabled = ZL303XX_FALSE;

      /* Stream Config */
      for (count = 0; count < 2; count++)
      {
         zlExampleApp.stream[count].ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
         zlExampleApp.stream[count].ptp.config.unicast = ZL303XX_FALSE;
         zlExampleApp.stream[count].ptp.config.portHandle = 0;  /* 2 streams on 1 port. */

         /* Destination addresses are the 802.3 values. */
         status |= examplePtpDestEthL2Set(destIpv4Addr[count], ZL303XX_TRUE,
                                            &zlExampleApp.stream[count].ptp);
      }

      status = exampleAppStart(&zlExampleApp);
   }

   return status;
}
#endif

/*****************   END   ****************************************************/


