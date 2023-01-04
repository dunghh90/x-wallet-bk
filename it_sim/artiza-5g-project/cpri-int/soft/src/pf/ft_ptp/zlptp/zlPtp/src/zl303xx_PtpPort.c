

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains interface for a PTP port data structure
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"
#include "zl303xx_Error.h"
#include "zl303xx_ErrTrace.h"
#include "zl303xx_List.h"

#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpSetup.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpFunctionExport.h"
#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpV2UnicastDiscovery.h"

#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpV2AcceptMaster.h"
#include "zl303xx_PtpForeignMasterTbl.h"
#include "zl303xx_PtpProfileDef.h"

/*****************   DEFINES     **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/
static zlStatusE zl303xx_PtpPortCreateStructCheck(zl303xx_PtpPortCreateS *portCreate, zl303xx_PtpClockS *pPtpClock);
static zlStatusE zl303xx_PtpPortNumberGenerate(zl303xx_PtpClockS *pPtpClock, Uint16T *portNumber);

static zlStatusE zl303xx_PtpGrantTblCreate(Uint16T maxEntries, zl303xx_PtpTableS *grantTbl);
static void zl303xx_PtpGrantTblDelete(zl303xx_PtpTableS *grantTbl);
static void zl303xx_PtpGrantEntryInit(const void *key, void *entry);
static zl303xx_BooleanE zl303xx_PtpGrantEntryMatch(const void *key, const void *entry);

/*****************   STATIC GLOBAL VARIABLES   ********************************/
static const zl303xx_PtpTableOpsS Zl303xx_PtpGrantTblOps = {
      zl303xx_PtpGrantEntryInit, zl303xx_PtpGrantEntryMatch
};

static zl303xxnumRateRatioPktsE zl303xx_PtpValidNumRateRatioPktsValues[ZL303XX_PTP_VALID_VALUES_NUM_RATE_RATIO_PKTS_MAX] = {
   ZL303XX_PTP_NUM_RATE_RATIO_1,ZL303XX_PTP_NUM_RATE_RATIO_2,ZL303XX_PTP_NUM_RATE_RATIO_4,ZL303XX_PTP_NUM_RATE_RATIO_8,
   ZL303XX_PTP_NUM_RATE_RATIO_16};

/*****************   IMPORTED GLOBAL VARIABLES   ******************************/
extern zl303xx_PortDataS *Zl303xx_PtpPortArray[ZL303XX_PTP_NUM_PORTS_MAX];
extern zl303xx_ClockStreamS *Zl303xx_PtpStreamArray[ZL303XX_PTP_NUM_STREAMS_MAX];

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* zl303xx_PtpPortCreateStructInit */
/**
   Fills a zl303xx_PtpPortCreateS structure with the default configuration for
   initializing a PTP port.

  Parameters:
   [out]  pPortCreate  Default parameters for creating a PTP port.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pPortCreate is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpPortCreateStructInit(zl303xx_PtpPortCreateS *pPortCreate)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpPortCreateStructInit:", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pPortCreate);
   }

   /* Assign default values. */
   if (status == ZL303XX_OK)
   {
      OS_MEMSET(pPortCreate, 0, sizeof(*pPortCreate));

      pPortCreate->clockHandle = 0;
      pPortCreate->portNumber  = 0;  /* Auto-generate port number. */

      pPortCreate->delayMechanism               = ZL303XX_PTP_DEFAULT_PORT_DELAY_MECH;
      pPortCreate->pdRespTxMethod               = ZL303XX_PTP_DEFAULT_PORT_PDELAY_RESP_METHOD;
      pPortCreate->logMinPdelayReqInterval      = ZL303XX_PTP_DEFAULT_STREAM_LOG_PEER_DELAY_INTVL;
      pPortCreate->ptpVersion                   = ZL303XX_PTP_DEFAULT_PORT_PTP_VERSION;
      pPortCreate->acceptableMasterTableEnabled = ZL303XX_PTP_DEFAULT_PORT_ACC_MASTER_TBL;
      pPortCreate->acceptableMasterTableLength  = ZL303XX_PTP_DEFAULT_PORT_ACC_MASTER_TBL_LEN;
      pPortCreate->acceptableSlaveTableEnabled  = ZL303XX_PTP_DEFAULT_PORT_ACC_SLAVE_TBL;
      pPortCreate->mixedMode                    = ZL303XX_PTP_DEFAULT_PORT_MIXED_MODE;
      pPortCreate->alternateMaster              = ZL303XX_PTP_DEFAULT_PORT_ALT_MASTER;

      /* Unicast negotiation parameters. */
      pPortCreate->unicastNegotiation.enabled               = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_ENABLED;
      pPortCreate->unicastNegotiation.maxAnnounceRate       = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_ANNC_RATE_MAX;
      pPortCreate->unicastNegotiation.maxAnnounceGrantSecs  = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_ANNC_GRANT_MAX;
      pPortCreate->unicastNegotiation.maxAnnounceCount      = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_ANNC_MAX;
      pPortCreate->unicastNegotiation.maxSyncRate           = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_SYNC_RATE_MAX;
      pPortCreate->unicastNegotiation.maxSyncGrantSecs      = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_SYNC_GRANT_MAX;
      pPortCreate->unicastNegotiation.maxSyncCount          = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_SYNC_MAX;
      pPortCreate->unicastNegotiation.maxDelayRespRate      = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_DELAY_RATE_MAX;
      pPortCreate->unicastNegotiation.maxDelayRespGrantSecs = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_DELAY_GRANT_MAX;
      pPortCreate->unicastNegotiation.maxDelayRespCount     = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_DELAY_MAX;
      pPortCreate->unicastNegotiation.maxPdelayRespRate     = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_PDELAY_RATE_MAX;
      pPortCreate->unicastNegotiation.maxPdelayRespGrantSecs = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_PDELAY_GRANT_MAX;
      pPortCreate->unicastNegotiation.maxPdelayRespCount    = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_PDELAY_MAX;
      pPortCreate->unicastNegotiation.stackedTlvEn          = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_STACKED_TLV_EN;
      pPortCreate->unicastNegotiation.denyServiceRequests   = ZL303XX_PTP_DEFAULT_PORT_DENY_UNICAST_REQUESTS;
      pPortCreate->unicastNegotiation.grantTableLength      = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_GRANT_TBL_LEN;

      /* Service limits. */
      pPortCreate->unicastNegotiation.totalPpsMax[ZL303XX_MSG_ID_SYNC]            = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_LIMIT_SYNC;
      pPortCreate->unicastNegotiation.totalPpsMax[ZL303XX_MSG_ID_ANNOUNCE]        = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_LIMIT_ANNC;
      pPortCreate->unicastNegotiation.totalPpsMax[ZL303XX_MSG_ID_DELAY_RESP]      = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_LIMIT_DELAY;
      pPortCreate->unicastNegotiation.totalPpsMax[ZL303XX_MSG_ID_PEER_DELAY_RESP] = ZL303XX_PTP_DEFAULT_PORT_UNI_NEG_LIMIT_DELAY;

      /* Unicast Master Table parameters. */
      pPortCreate->unicastMasterTable.maxTableSize     = ZL303XX_PTP_DEFAULT_PORT_UMT_MAX_TABLE_SIZE;
      pPortCreate->unicastMasterTable.logQueryInterval = ZL303XX_PTP_DEFAULT_PORT_UMT_LOG_QUERY_INTVL;

      pPortCreate->extData         = NULL;
      pPortCreate->requestedHandle = (zl303xx_PtpPortHandleT)ZL303XX_PTP_INVALID;

      /* Deprecated */
      pPortCreate->maxForeignRecords               = 0;
      pPortCreate->unicastNegotiation.grantDefault = !ZL303XX_PTP_DEFAULT_PORT_DENY_UNICAST_REQUESTS;
      
      /* rateRatioPkt attributes */
      pPortCreate->rateRatioEnabled = ZL303XX_FALSE;
      pPortCreate->numRateRatioPkts = ZL303XX_PTP_NUM_RATE_RATIO_1;

      /* Let the port automatically go to init if there is a fault */
      pPortCreate->faultPassThroughEn = ZL303XX_TRUE;

   }

   return status;
}

/* zl303xx_PtpPortCreate */
/**
   Creates a PTP port instance associated with an existing PTP clock.

   A PTP port represents logical access point of a clock for PTP communications
   to the communications network.

  Parameters:
   [in]   pPortCreate  Port configuration parameters.

   [out]  pPortHandle  Handle assigned to this port. Used to reference it
                            when calling other functions.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       pPortCreate or pPortHandle is NULL.
     ZL303XX_PARAMETER_INVALID     A member of pPortCreate is invalid.
     ZL303XX_RTOS_MEMORY_FAIL      Failed to allocate memory for port data.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.
     ZL303XX_TABLE_FULL            Maximum number of ports already created.

*******************************************************************************/
zlStatusE zl303xx_PtpPortCreate(zl303xx_PtpPortCreateS *pPortCreate,
                                zl303xx_PtpPortHandleT *pPortHandle)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_PtpClockS *pPtpClock = NULL;
   zl303xx_PortDataS *pPtpPort = NULL;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpPortCreate:", 0,0,0,0,0,0);

   /* Check input pointers. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pPortCreate) |
               ZL303XX_CHECK_POINTER(pPortHandle);
   }

   /* Get a pointer to the clock data structure. */
   if (status == ZL303XX_OK)
   {
      /* An error is returned if the clock is NULL. */
      status = zl303xx_PtpClockDataGet(pPortCreate->clockHandle, &pPtpClock);
   }

   /* Check the port creation parameters. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortCreateStructCheck(pPortCreate, pPtpClock);
   }

   /* Perform any Profile specific checks on the port creation parameters. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpProfilePortPreCreate(pPortCreate, pPtpClock);
   }

   /* Allocate memory for the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortStructAllocate(pPortCreate, &pPtpPort);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pPtpClock);
      status = mStatus;
   }

   /* If portNumber is 0, generate a valid and unique one for this clock. */
   if (status == ZL303XX_OK)
   {
      if (pPortCreate->portNumber == 0)
      {
         status = zl303xx_PtpPortNumberGenerate(pPtpClock, &pPortCreate->portNumber);
      }
   }

   /* Initialize the port data structure. */
   if (status == ZL303XX_OK)
   {
      /* Save the user's configuration. */
      pPtpPort->config = *pPortCreate;

      /* Store a pointer to the associated clock. */
      pPtpPort->clock = pPtpClock;

      /* Stream list. */
      zl303xx_ListInit(&pPtpPort->streamList);
      pPtpPort->streamCount = 0;

      /* The following code copies the Port Creation Configuration values to
       * similar structure (that is in the process of being deprecated).       */
      {  /* Version 2 */
         /* Copy the clock identity structure to the port identity of the
          * portDS (port Data set).  */
         OS_MEMCPY(pPtpPort->portDS.portIdentity.clockIdentity,
                pPtpClock->config.defaultDS.clockIdentity,
                PTP_V2_CLK_IDENTITY_LENGTH);

         /* Set the PTP port number in the portDS. This may be a user-specified
          * value or auto-generated. */
         pPtpPort->portDS.portIdentity.portNumber = pPtpPort->config.portNumber;

         /* Set the PTP version in the portDS. This may be a user-specified
          * value or auto-generated. */
         pPtpPort->portDS.versionNumber = pPtpPort->config.ptpVersion;

         /* Set the PTP port delayMechanism. This may be a user-specified
          * value or auto-generated. */
         pPtpPort->portDS.delayMechanism = pPtpPort->config.delayMechanism;

         /* Set the PTP port rate ratio valid to default. */
         pPtpPort->portDS.neighbourRateRatio.rateRatioValid =
                              ZL303XX_PTP_DEFAULT_PORT_NEIGH_RATE_RATIO_VALID;

         /* Set the unicast parameters */
         if (pPtpPort->config.unicastNegotiation.enabled == ZL303XX_TRUE)
         {
            pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].minIntvl = pPortCreate->unicastNegotiation.maxAnnounceRate;
            pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].maxDurationSec = pPortCreate->unicastNegotiation.maxAnnounceGrantSecs;
            pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].maxCount = pPortCreate->unicastNegotiation.maxAnnounceCount;
            pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_SYNC].minIntvl = pPortCreate->unicastNegotiation.maxSyncRate;
            pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_SYNC].maxDurationSec = pPortCreate->unicastNegotiation.maxSyncGrantSecs;
            pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_SYNC].maxCount = pPortCreate->unicastNegotiation.maxSyncCount;
            pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].minIntvl = pPortCreate->unicastNegotiation.maxDelayRespRate;
            pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].maxDurationSec = pPortCreate->unicastNegotiation.maxDelayRespGrantSecs;
            pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].maxCount = pPortCreate->unicastNegotiation.maxDelayRespCount;
            pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].minIntvl = pPortCreate->unicastNegotiation.maxPdelayRespRate;
            pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].maxDurationSec = pPortCreate->unicastNegotiation.maxPdelayRespGrantSecs;
            pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].maxCount = pPortCreate->unicastNegotiation.maxPdelayRespCount;
         }

         /* Initialize the acceptable master table, regardless of the acceptable master status. */
         if (status == ZL303XX_OK)
         {
            status = zl303xx_PtpAccMasterTblCreate(
                        pPortCreate->acceptableMasterTableLength,
                        &pPtpPort->accMasterTbl);

            /* Note: The dropped-packet counters for this table are set to '0'
             *       at port creation since OS_CALLOC was used to initialize
             *       the memory. We force it here for completeness. */
            OS_MEMSET(&pPtpPort->amtDroppedMsgCount, 0x00,
                   sizeof(pPtpPort->amtDroppedMsgCount)); /* sizeof the declared array */
         }

         /* Initialize the acceptable slave table, regardless of the acceptable slave status. */
         if (status == ZL303XX_OK)
         {
            status = zl303xx_PtpAccSlaveTblCreate(
                        pPortCreate->acceptableMasterTableLength,
                        &pPtpPort->accSlaveTbl);
         }

         /* Initialize the grant table */
         if (status == ZL303XX_OK)
         {
            status = zl303xx_PtpGrantTblCreate(
                        pPortCreate->unicastNegotiation.grantTableLength,
                        &pPtpPort->grantTbl);
         }
      }
   }

   if (status == ZL303XX_OK)
   {
      /* Ensure this port will be initialized the first time it is processed
       * by the protocol engine */
      pPtpPort->portSuperState = PTP_PORT_INT_STATE_INIT;

      /* Initialize the Unicast Master Table for this port. */
      zl303xx_PtpUmtStructInit(&pPtpPort->unicastMasterTable);
      status = zl303xx_PtpUmtCreate(&pPtpPort->unicastMasterTable,
                                    pPortCreate->unicastMasterTable.maxTableSize);
   }
   if (status == ZL303XX_OK)
   {
      /* Set the query interval for setting up unicast contracts for use when the master
         does not respond immediately */
      pPtpPort->unicastMasterTable.logQueryInterval = pPortCreate->unicastMasterTable.logQueryInterval;
   }

   /* Note that variables that are not explicitly initialized will be set to
    * zero because the structure was initialized to zero by OS_CALLOC(). */

   /* Add the port to the clock's portList. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockAttachPort(pPtpClock, pPtpPort);
   }

   if (status == ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
                    "Created PTP clock port for %d", pPortCreate->portNumber, 0,0,0,0,0);

      /* set the output value. This is a unique handle to this clock port. */
      pPtpPort->unicastMasterTable.portHandle = pPtpPort->clockPortHandle;
      *pPortHandle = pPtpPort->clockPortHandle;
   }
   else
       ZL303XX_TRACE_ALWAYS("zl303xx_PtpPortCreate: failure=%d", status, 0,0,0,0,0);

   /* Execute any Profile Specific actions related to the port. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpProfilePortPostCreate(pPtpPort);
   }

   /* Release the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   /* The Port failed to create at some point along the way. Attempt to FREE any
    * associated memory and delete the object from the Clock.  */
   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS(
            "   PTP port creation FAILED: status = %d.", status, 0,0,0,0,0);

      /* Ignore any return code from the DELETE but return the original status. */
      if (pPtpPort != NULL)
      {
         zl303xx_PtpPortDeleteS portDelete;
         (void)zl303xx_PtpPortDeleteStructInit(&portDelete);
         (void)zl303xx_PtpPortDeleteInternal(pPtpPort, &portDelete);
      }

      /* We expect the above call to free the memory but in case it had an
       * error, Free the memory manually. */
      if (pPtpPort != NULL)
      {
         OS_FREE(pPtpPort);
         pPtpPort = NULL;
      }
   }

   return status;
}

/* zl303xx_PtpPortDeleteStructInit */
/**
   Fills a zl303xx_PtpPortDeleteS structure with the default configuration for
   deleting a PTP port.

  Notes:
   Does not need to be called since zl303xx_PtpPortDeleteS does not currently
   have any members.

  Parameters:
   [out]  pPortDelete  Default parameters to shut down a PTP port.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pPortDelete is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpPortDeleteStructInit(zl303xx_PtpPortDeleteS *pPortDelete)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpPortDeleteStructInit:", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pPortDelete);
   }

   /* Initialize the port shutdown structure. */
   if (status == ZL303XX_OK)
   {
      /* Zero-fill the structure; it does not contain any members. */
      OS_MEMSET(pPortDelete, 0, sizeof(*pPortDelete));
   }

   return status;
}

/* zl303xx_PtpPortDelete */
/**
   Deletes a PTP port.

  Parameters:
   [in]   portHandle   Handle to an existing PTP port.
   [in]   pPortDelete  (Optional) Parameters to shut down this PTP port.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpPortDelete(zl303xx_PtpPortHandleT portHandle,
                                zl303xx_PtpPortDeleteS *pPortDelete)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_PortDataS *port = NULL;
   zl303xx_PtpClockS *pClock = NULL;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpPortDelete:", 0,0,0,0,0,0);

   /* Don't bother checking pPortDelete, since it does not contain any members. */

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      if ((status = ZL303XX_CHECK_PORT_HANDLE(portHandle)) != ZL303XX_OK)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Invalid portHandle=%lu.", portHandle, 0,0,0,0,0);
      }
   }

   if (status == ZL303XX_OK)
   {
      /* Get a pointer to the port data. Don't call zl303xx_PtpPortDataGet()
       * because we want to exit the function with ZL303XX_OK if the port does
       * not exist. */
      port = Zl303xx_PtpPortArray[portHandle];

      if (port == NULL)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "   No port with handle=%lu exists.",
               portHandle, 0,0,0,0,0);

         return status;
      }

      /* Lock the clock mutex. */
      pClock = port->clock;
      mStatus = zl303xx_PtpClockMutexLock(pClock);
      status = mStatus;
   }

   /* Call the internal port delete function. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDeleteInternal(port, pPortDelete);
   }

   /* Release the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pClock, status);
   }

   return status;
}

/* zl303xx_PtpPortConfigGet */
/**
   Returns the current configuration parameters of a PTP port.

  Parameters:
   [in]   portHandle   Handle to an existing port.
   [out]  pPortConfig  A copy of the port's current configuration.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pPortConfig is NULL.
     ZL303XX_PARAMETER_INVALID  portHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpPortConfigGet(zl303xx_PtpPortHandleT portHandle,
                                   zl303xx_PtpPortCreateS *pPortConfig)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *portData = NULL;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpPortConfigGet:", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pPortConfig);
   }

   /* Get a pointer to the port data structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &portData);
   }

   /* Return a copy of the configuration structure. */
   if (status == ZL303XX_OK)
   {
      *pPortConfig = portData->config;
   }

   return status;
}

/* zl303xx_PtpPortExtDataGet */
/**
   Returns the external data pointer associated with a port.

  Parameters:
   [in]   portHandle   Handle to an existing port.
   [out]  ppExtData    Pointer to user-specified data structure.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    ppExtData is NULL.
     ZL303XX_PARAMETER_INVALID  portHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpPortExtDataGet(zl303xx_PtpPortHandleT portHandle,
                                    void **ppExtData)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *port = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(ppExtData);
   }

   /* Get a pointer to the port data structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &port);
   }

   /* Return the pointer to user data. */
   if (status == ZL303XX_OK)
   {
      *ppExtData = port->config.extData;
   }

   return status;
}

/* zl303xx_PtpPortExtDataSet */
/**
   Sets the external data pointer associated with a port.

  Parameters:
   [in]   portHandle  Handle to an existing port.
   [in]   pExtData    User-specified data structure.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpPortExtDataSet(zl303xx_PtpPortHandleT portHandle,
                                    void *pExtData)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort;

   /* Don't bother checking pExtData, as the user may want to set it to NULL. */

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPort->clock);
   }

   if (status == ZL303XX_OK)
   {
      pPort->config.extData = pExtData;
      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }

   return status;
}

/* zl303xx_PtpPortPdelayRespHandleMethodSet */
/**
   Sets the method by which Peer-Delay Response messages are handled by a PTP
   Port. IEEE-1588-2008: Clause 11.4.3 outlines 3 methods.

  Parameters:
   [in]  portHandle        Handle to an existing port.
   [in]  pdelayRespMethod  Method of handling Peer-Delay Responses.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     pdelayRespMethod is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpPortPdelayRespHandleMethodSet(
      zl303xx_PtpPortHandleT portHandle,
      zl303xx_PtpPdelayRespMethodE pdelayRespMethod)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPtpPort;

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPtpPort);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpPort->clock);
   }

   if (status == ZL303XX_OK)
   {
      /* Set the method indicator. */
      pPtpPort->config.pdRespTxMethod = pdelayRespMethod;

      /* Release the clock mutex. */
      status = zl303xx_PtpClockMutexUnlock(pPtpPort->clock, status);
   }

   return status;
}

/* zl303xx_PtpPortPeerMeanPathDelayGet */
/**
   Returns the current peer-mean-path-delay value for the specified port.

  Parameters:
   [in]   portHandle          Handle to an existing port.
   [out]  peerMeanPathDelay   Pointer to current peer-mean-path-delay value.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    ppExtData is NULL.
     ZL303XX_PARAMETER_INVALID  portHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpPortPeerMeanPathDelayGet(zl303xx_PtpPortHandleT portHandle,
                                              zl303xx_TimeInterval *peerMeanPathDelay)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *port = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(peerMeanPathDelay);
   }

   /* Get a pointer to the port data structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &port);
   }

   /* Return the pointer to user data. */
   if (status == ZL303XX_OK)
   {
      *peerMeanPathDelay = port->portDS.peerMeanPathDelay;
   }

   return status;
}


/* zl303xx_PtpPortNeighbourRateRatioGet */
/**
   Returns the current neighbour rate ratio value for the specified port.

  Parameters:
   [in]   portHandle          Handle to an existing port.
   [out]  neighbourRateRatio  A copy of the current neighbour rate ratio.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pNeighbourRateRatio is NULL.
     ZL303XX_PARAMETER_INVALID  portHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpPortNeighbourRateRatioGet(zl303xx_PtpPortHandleT portHandle,
                                             zl303xx_RateRatio *pNeighbourRateRatio)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *port = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pNeighbourRateRatio);
   }

   /* Get a pointer to the port data structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &port);
   }

   /* Return a copy of the current neighbour rate ratio */
   if (status == ZL303XX_OK)
   {
      OS_MEMSET(pNeighbourRateRatio, 0, sizeof(*pNeighbourRateRatio));
      *pNeighbourRateRatio = port->portDS.neighbourRateRatio;
   }

   return status;
}
/* zl303xx_PtpPortLogQueryIntervalSet */
/**
   Updates the logQueryInterval value of a port's Unicast Master Table.

  Parameters:
   [in]   portHandle        Handle to an existing port.
   [in]   logQueryInterval  log2 of the interval value in seconds.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle or logQueryInterval is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpPortLogQueryIntervalSet(zl303xx_PtpPortHandleT portHandle,
                                             Sint8T logQueryInterval)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
   zl303xx_PortDataS *pPort = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      if ((logQueryInterval < UNICAST_DISC_INTERVAL_MIN) ||
          (logQueryInterval > UNICAST_DISC_INTERVAL_MAX))
      {
         ZL303XX_TRACE_ALWAYS("Invalid logQueryInterval=%d (valid range %d to %d).",
            logQueryInterval, UNICAST_DISC_INTERVAL_MIN, UNICAST_DISC_INTERVAL_MAX, 0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   /* Check the table validity. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpUmtCheck(&pPort->unicastMasterTable);
   }

   /* Additional Profile specific range checking. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpProfileQueryIntervalCheck(
                                          pPort->clock->config.profile,
                                          pPort->clock->config.profileStrict,
                                          &logQueryInterval);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pPort->clock);
      status = mStatus;
   }

   /* If it is already running, restart the UNICAST_QUERY_INTERVAL timer of
    * every stream on this port. */
   if (status == ZL303XX_OK)
   {
      zl303xx_ClockStreamS *pStream;
      /* renewTime is the number of seconds before the contract
       * expires when reissue requests will begin transmitting */
      zl303xx_TimeStamp timeInterval, renewTime;

      zl303xx_PtpConvertLog2IntervalToTime(logQueryInterval, &timeInterval);
      mulTime(&renewTime, &timeInterval, ZL303XX_PTP_DEFAULT_STREAM_UNI_NEG_RENEW);

      ZL303XX_PTP_FOR_EACH_STREAM(&pPort->streamList, pStream)
      {
         /* Assume the high bits of the second field will not apply, they are too high */
         status = zl303xx_PtpStreamUniNegRenewSet(pStream->clockStreamHandle, renewTime.secondsField.lo);
         if (status != ZL303XX_OK)
         {
            /* A failure occurred when updating the renewal time.
             * Return an error. If the renewal time was bad, an error
             * will be returned immediately.
             * Note: No repair actions are made if
             * a local control message failure occurs */
            break;
         }
         if (zl303xx_PtpTimerActive(&pStream->itimer[UNICAST_QUERY_INTERVAL]))
         {
            zl303xx_PtpStreamTimerStart(pStream, UNICAST_QUERY_INTERVAL);
         }
      }
   }

   if (status == ZL303XX_OK)
   {
      /* Update the port data field if the renewal times were successfully set */
      pPort->unicastMasterTable.logQueryInterval = logQueryInterval;
   }

   /* Release the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }

   return status;
}

/* zl303xx_PtpPortUnicastStackedTlvEnSet */
/**
   Sets a port's unicast negotiation stacked TLV enabled setting.

  Parameters:
   [in]   portHandle     Handle to an existing port.
   [in]   stackedTlvEn   ZL303XX_TRUE to enable sending stacked TLVs from this
                              port. ZL303XX_FALSE to send single TLVs.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle or stackedTlvEn is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpPortUnicastStackedTlvEnSet(zl303xx_PtpPortHandleT portHandle,
                                                zl303xx_BooleanE stackedTlvEn)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(stackedTlvEn);
   }

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPort->clock);
   }

   /* Update the stackedTlvEn port setting. */
   if (status == ZL303XX_OK)
   {
      pPort->config.unicastNegotiation.stackedTlvEn = stackedTlvEn;
      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }

   return status;
}

/* zl303xx_PtpPortTotalPpsMaxSet */
/**
   Sets a port's total PPS service limit for a specific message type. Note that
   this will not affect any existing unicast negotiation contracts. Only new
   or renewed contracts will use the new value.

  Parameters:
   [in]   portHandle   Handle to an existing port.
   [in]   msgType      The message type limit to modify.
   [in]   totalPpsMax  New PPS service limit. Setting to 0 will reject all
                            contracts.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle or msgType is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpPortTotalPpsMaxSet(zl303xx_PtpPortHandleT portHandle,
                                        zl303xx_MessageTypeE msgType,
                                        Uint32T totalPpsMax)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort = NULL;

   /* Check input parameters. */
   if ((status == ZL303XX_OK) &&
       ((status = ZL303XX_CHECK_MESSAGE_TYPE(msgType)) != ZL303XX_OK))
   {
      ZL303XX_TRACE_ALWAYS("Invalid msgType=%d", msgType, 0,0,0,0,0);
   }

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPort->clock);
   }

   /* Update the PPS limit. Unlock the mutex. */
   if (status == ZL303XX_OK)
   {
      pPort->config.unicastNegotiation.totalPpsMax[msgType] = totalPpsMax;
      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }

   return status;
}

/* zl303xx_PtpPortTotalPpsGet */
/**
   Returns the total PPS counters for all unicast negotiated message types
   (ANNOUNCE, SYNC, and DELAY_RESP) on a specified port.

  Parameters:
   [in]   portHandle  Handle to a previously created port.
   [out]  pTotal      Returned PPS counters.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       pTotal is NULL.
     ZL303XX_PARAMETER_INVALID     portHandle is invalid.
     ZL303XX_INVALID_MODE          Unicast negotiation not enabled on the port.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpPortTotalPpsGet(zl303xx_PtpPortHandleT portHandle,
                                     zl303xx_PtpTotalPpsS *pTotal)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtpPortTotalPpsGet";
   zl303xx_PortDataS *pPort = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pTotal);
   }

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   /* Check if unicast negotiation is enabled on this port. */
   if (status == ZL303XX_OK)
   {
      if (pPort->config.unicastNegotiation.enabled == ZL303XX_FALSE)
      {
         ZL303XX_TRACE_ALWAYS("%s: Unicast Negotiation is disabled on port %lu.",
                              fnName, portHandle, 0,0,0,0);
         status = ZL303XX_INVALID_MODE;
      }
   }

   /* Get the total PPS counters. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPort->clock);
   }

   if (status == ZL303XX_OK)
   {
      OS_MEMSET(pTotal, 0, sizeof(*pTotal));

      pTotal->msg[ZL303XX_MSG_ID_SYNC]            = zl303xx_PtpV2GetMsgTypePortTotalPps(pPort, NULL, PTP_NEGOTIATED_MSG_ID_SYNC);
      pTotal->msg[ZL303XX_MSG_ID_ANNOUNCE]        = zl303xx_PtpV2GetMsgTypePortTotalPps(pPort, NULL, PTP_NEGOTIATED_MSG_ID_ANNOUNCE);
      pTotal->msg[ZL303XX_MSG_ID_DELAY_RESP]      = zl303xx_PtpV2GetMsgTypePortTotalPps(pPort, NULL, PTP_NEGOTIATED_MSG_ID_DELAY_RESP);
      pTotal->msg[ZL303XX_MSG_ID_PEER_DELAY_RESP] = zl303xx_PtpV2GetMsgTypePortTotalPps(pPort, NULL, PTP_NEGOTIATED_MSG_ID_PDELAY_RESP);

      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }

   return status;
}

/* zl303xx_PtpGrantDefaultSet */
/**
   Sets the default behavior for unicast negotiation requests.

  Parameters:
   [in]   portHandle    Handle to a previously create port.
   [in]   grantDefault  Default behavior to set.

  (Deprecated)    Use zl303xx_PtpPortDenyServiceRequestsSet() instead.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle or grantDefault is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpGrantDefaultSet(zl303xx_PtpPortHandleT portHandle,
                                     zl303xx_BooleanE grantDefault)
{
   ZL303XX_TRACE_ALWAYS("zl303xx_PtpGrantDefaultSet: This function is deprecated.",
         0,0,0,0,0,0);

   /* Call the new routine directly.
    * The logic is Flipped (i.e GRANT in the old vs DENY in the new). */
   return zl303xx_PtpPortDenyServiceRequestsSet(portHandle, !grantDefault);
}

/* zl303xx_PtpGrantConfigStructInit */
/**
   Initializes a zl303xx_PtpGrantConfigS structure with default values.

  Parameters:
   [in]   pConfig  Grant configuration to initialize.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pConfig is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpGrantConfigStructInit(zl303xx_PtpGrantConfigS *pConfig)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pConfig);
   }

   if (status == ZL303XX_OK)
   {
      OS_MEMSET(pConfig, 0, sizeof(*pConfig));
      pConfig->portAddress.networkProtocol = ZL303XX_NETWORK_PROTOCOL_INVALID;
   }

   return status;
}

/* zl303xx_PtpGrantConfigSet */
/**
   Sets the override bit-mask for contracts that can be granted to a far-end
   port.

  Parameters:
   [in]   portHandle  Handle to a previously created port.
   [in]   pConfig     Grant message types configuration.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       pConfig is NULL.
     ZL303XX_PARAMETER_INVALID     portHandle or pConfig->portAddress is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.
     ZL303XX_TABLE_FULL            Grant configuration table is full.

*******************************************************************************/
zlStatusE zl303xx_PtpGrantConfigSet(zl303xx_PtpPortHandleT portHandle,
                                    zl303xx_PtpGrantConfigS *pConfig)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_PortDataS *pPort;
   zl303xx_PtpGrantConfigS *grantEntry = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pConfig);
   }
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortAddressCheck(&pConfig->portAddress);
   }

   /* Get a pointer to the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pPort->clock);
      status = mStatus;
   }

   /* Add or update this data entry in the grant table. */
   if (status == ZL303XX_OK)
   {
      grantEntry = zl303xx_PtpTableAdd(&pPort->grantTbl, &pConfig->portAddress);
      if (grantEntry == NULL)
      {
         ZL303XX_TRACE_ERROR("zl303xx_PtpGrantConfigSet: Table full (port %lu).",
                             portHandle, 0,0,0,0,0);
         status = ZL303XX_TABLE_FULL;
      }
   }

   if (status == ZL303XX_OK)
   {
      grantEntry->msgTypeMask = pConfig->msgTypeMask;
   }

   /* Unlock the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }

   return status;
}

/* zl303xx_PtpGrantConfigGet */
/**
   Returns the override bit-mask for contracts that can be granted to a far-end
   port.

  Parameters:
   [in]   portHandle  Handle to an existing port.
   [in]   pAddress    Port address to search for.
   [out]  pConfig     Previously set grant configuration.

  Return Value:
     ZL303XX_OK                     Success.
     ZL303XX_INVALID_POINTER        pAddress or pConfig is NULL.
     ZL303XX_PARAMETER_INVALID      portHandle is invalid.
     ZL303XX_TABLE_ENTRY_NOT_FOUND  No config associated with pAddress.

*******************************************************************************/
zlStatusE zl303xx_PtpGrantConfigGet(zl303xx_PtpPortHandleT portHandle,
                                    zl303xx_PortAddress *pAddress,
                                    zl303xx_PtpGrantConfigS *pConfig)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort;
   zl303xx_PtpGrantConfigS *grantEntry = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pAddress) |
               ZL303XX_CHECK_POINTER(pConfig);
   }

   /* Get a pointer to the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   /* Get a pointer to the grant entry structure. */
   if (status == ZL303XX_OK)
   {
      grantEntry = zl303xx_PtpTableFind(&pPort->grantTbl, pAddress);
      if (grantEntry == NULL)
      {
         ZL303XX_TRACE_ERROR("zl303xx_PtpGrantConfigGet: Address not found (portHandle=%lu).",
                             portHandle, 0,0,0,0,0);
         status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
      }
   }

   /* Return the data in the grant entry. */
   if (status == ZL303XX_OK)
   {
      *pConfig = *grantEntry;
   }

   return status;
}

/* zl303xx_PtpGrantConfigDelete */
/**
   Removes the override bit-mask table entry for contracts that can be granted
   to a far-end port.

  Parameters:
   [in]   portHandle  Handle to a previously created port.
   [in]   pAddress    Port address to remove from the table.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       pAddress is NULL.
     ZL303XX_PARAMETER_INVALID     portHandle is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpGrantConfigDelete(zl303xx_PtpPortHandleT portHandle,
                                       zl303xx_PortAddress *pAddress)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pAddress);
   }

   /* Get a pointer to the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPort->clock);
   }

   if (status == ZL303XX_OK)
   {
      /* Remove this entry in the grant table. */
      zl303xx_PtpTableRemove(&pPort->grantTbl, pAddress);

      /* Unlock the clock mutex. */
      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }

   return status;
}

/* zl303xx_PtpGrantConfigDeleteAll */
/**
   Removes all entries (bit-masks) in the table for granting contracts to a
   far-end port.

  Parameters:
   [in]   portHandle  Handle to a previously created port.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpGrantConfigDeleteAll(zl303xx_PtpPortHandleT portHandle)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort;

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPort->clock);
   }

   if (status == ZL303XX_OK)
   {
      zl303xx_PtpTableRemoveAll(&pPort->grantTbl);
      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }

   return status;
}

/* zl303xx_PtpVirtualPortCreateStructInit */
/**
   Initializes a zl303xx_PtpVirtualPortConfigS structure with default values.

  Parameters:
   [in]  pVirtualPortCreate   Virtual Port configuration to initialize.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pVirtualPortCreate is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpVirtualPortCreateStructInit(
      zl303xx_PtpVirtualPortConfigS *pVirtualPortCreate)
{
   zlStatusE status = ZL303XX_OK;

   /* Pointer to the targeted clock (may or may not exist). */
   zl303xx_PtpClockS * pPtpClock = NULL;

   /* Check the input pointer. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pVirtualPortCreate);
   }

   if (status == ZL303XX_OK)
   {
      /* Save the passed in clock handle and then clear the input structure. */
      zl303xx_PtpClockHandleT origClockHandle = pVirtualPortCreate->clockHandle;
      OS_MEMSET(pVirtualPortCreate, 0, sizeof(*pVirtualPortCreate));
      pVirtualPortCreate->clockHandle = origClockHandle;

      /* Get the associated Clock data (if it exists). */
      if (zl303xx_PtpClockDataGet(origClockHandle, &pPtpClock) != ZL303XX_OK)
      {
         /* The specified clock does not exist. */
         /* status will still be OK so continue. */
         pPtpClock = NULL;
      }

      /* By Default, let the system choose the portNumber. */
      pVirtualPortCreate->portNumber = (Uint16T)0;

      /* By default, disable the VP. */
      pVirtualPortCreate->ptsf = ZL303XX_TRUE;

      /* Assume no PRTC source. */
      pVirtualPortCreate->prtcConnected = ZL303XX_FALSE;

      /* Set the default GM parameters. */
      {
         /* If a good parent clock was specified, use its clockID by default. */
         if (pPtpClock != NULL)
         {
            OS_MEMCPY(pVirtualPortCreate->clockIdentity,
                   pPtpClock->config.defaultDS.clockIdentity,
                   PTP_V2_CLK_IDENTITY_LENGTH);
         }
         else
         {
            OS_MEMSET(pVirtualPortCreate->clockIdentity,
                   0x00, PTP_V2_CLK_IDENTITY_LENGTH);
         }

         /* Use worse case quality parameters. */
         pVirtualPortCreate->clockQuality.clockClass = PTP_MASTER_ONLY_CLOCK_CLASS_MAX;
         pVirtualPortCreate->clockQuality.clockAccuracy = ZL303XX_CLOCK_ACCURACY_UNKNOWN;
         pVirtualPortCreate->clockQuality.offsetScaledLogVariance = 0xFFFF;

         /* Set to the lowest value; force the user to set as needed. */
         pVirtualPortCreate->priority1 = 0;
         pVirtualPortCreate->priority2 = 0;

         /* Assume the GM is this local clock. */
         pVirtualPortCreate->stepsRemoved = 0;
      }

      /* Get the default Time Properties values. */
      zl303xx_PtpTimePropertiesDefaultParams(&pVirtualPortCreate->timeProperties);
      /* Depreciated member */
      pVirtualPortCreate->timeSource = pVirtualPortCreate->timeProperties.timeSource;

      /* Assume the ToD is not set. */
      pVirtualPortCreate->timeSetFlag = ZL303XX_FALSE;

      /* Some profiles use a Local Priority value. Use the same as the default Clock
       * Priority values for now. */
      pVirtualPortCreate->localPriority = ZL303XX_PTP_DEFAULT_CLOCK_PRI_1;

      /* Pointer to some external user data. */
      pVirtualPortCreate->extData = NULL;

   }

   return status;
}

/* zl303xx_PtpVirtualPortGet */
/**
   Returns the configuration data associated with the specified virtual port.

  Parameters:
   [in]   portHandle  Handle to an existing port.

   [out]  pVirtualPortConfig  Existing virtual port configuration.

  Return Value:
     ZL303XX_OK                     Success.
     ZL303XX_INVALID_POINTER        pVirtualPortConfig is NULL.
     ZL303XX_PARAMETER_INVALID      portHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpVirtualPortGet(
      zl303xx_PtpPortHandleT portHandle,
      zl303xx_PtpVirtualPortConfigS *pVirtualPortConfig)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pVirtualPortConfig);
   }

   /* Get a pointer to the Virtual port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpVirtualPortDataGet(portHandle, &pPort);
   }

   /* Get a pointer to the grant entry structure. */
   if (status == ZL303XX_OK)
   {
      *pVirtualPortConfig = pPort->virtualData.config;
   }

   return status;
}

/* zl303xx_PtpVirtualPortCreate */
/**
   Creates a PTP port instance associated with an existing PTP clock.

   A PTP port represents logical access point of a clock for PTP communications
   to the communications network.

  Parameters:
   [in]  pVirtualPortCreate  Virtual Port configuration parameters.

   [out] pPortHandle    Handle assigned to this port. Used to reference it
                              when calling other functions.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       Either input pointer is NULL.
     ZL303XX_PARAMETER_INVALID     A member of pVirtualPortCreate is invalid.
     ZL303XX_RTOS_MEMORY_FAIL      Failed to allocate memory for port data.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.
     ZL303XX_TABLE_FULL            Maximum number of ports already created.

*******************************************************************************/
zlStatusE zl303xx_PtpVirtualPortCreate(
      zl303xx_PtpVirtualPortConfigS *pVirtualPortCreate,
      zl303xx_PtpPortHandleT *pPortHandle)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;

   /* To create a virtual port, we fake a regular PTP Port creation with most
    * functionality disabled and then flag the port as a Virtual Port and add
    * the Virtual port configuration. */
   zl303xx_PtpPortCreateS ptpPortCreateS;

   zl303xx_PtpClockS *pPtpClock = NULL;
   zl303xx_PortDataS *pPtpPort = NULL;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpVirtualPortCreate: Start...", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pVirtualPortCreate) |
               ZL303XX_CHECK_POINTER(pPortHandle);
   }

   /* Make sure the clock identity that the VP represents is valid. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2ClockIdentityCheck(pVirtualPortCreate->clockIdentity);
   }

   /* Get a pointer to the clock data structure. */
   if (status == ZL303XX_OK)
   {
      /* An error is returned if the clock is NULL. */
      status = zl303xx_PtpClockDataGet(pVirtualPortCreate->clockHandle, &pPtpClock);
   }

   /* Create some fake PTP Port data. */
   if (status == ZL303XX_OK)
   {
      /* Set the defaults. */
      (void)zl303xx_PtpPortCreateStructInit(&ptpPortCreateS);

      /* Copy the data we need. */
      ptpPortCreateS.clockHandle = pVirtualPortCreate->clockHandle;
      ptpPortCreateS.portNumber = pVirtualPortCreate->portNumber;
      ptpPortCreateS.extData = pVirtualPortCreate->extData;

      /* Disable just about everything else. */
      {
         /* No Delay Mechanism. */
         ptpPortCreateS.delayMechanism = ZL303XX_DELAY_MECHANISM_DISABLED;

         /* Disable the acceptable master/slave tables. */
         ptpPortCreateS.acceptableMasterTableEnabled = ZL303XX_FALSE;
         ptpPortCreateS.acceptableSlaveTableEnabled = ZL303XX_FALSE;
         ptpPortCreateS.acceptableMasterTableLength = 0;
         ptpPortCreateS.mixedMode = ZL303XX_FALSE;

         /* Unicast negotiation parameters. */
         ptpPortCreateS.unicastNegotiation.enabled = ZL303XX_FALSE;
         ptpPortCreateS.unicastNegotiation.grantDefault = ZL303XX_FALSE;
         ptpPortCreateS.unicastNegotiation.denyServiceRequests = ZL303XX_TRUE;
         ptpPortCreateS.unicastNegotiation.grantTableLength = 0;

         /* Unicast Master Table parameters. */
         ptpPortCreateS.unicastMasterTable.maxTableSize = 0;

         /* Alternate master. */
         ptpPortCreateS.alternateMaster = ZL303XX_FALSE;

         /** Function used to transmit PTP messages. */
         ptpPortCreateS.txMsgFn = NULL;
      }

      /* Create an actual PTP Port. */
     /*  status = zl303xx_PtpPortCreate(&ptpPortCreateS, pPortHandle); */

   }

   /* Allocate memory for the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortStructAllocate(&ptpPortCreateS, &pPtpPort);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pPtpClock);
      status = mStatus;
   }

   /* If portNumber is 0, generate a valid and unique one for this clock. */
   if (status == ZL303XX_OK)
   {
      if (ptpPortCreateS.portNumber == 0)
      {
         status = zl303xx_PtpPortNumberGenerate(pPtpClock, &ptpPortCreateS.portNumber);

         /* Copy this to the Virtual Port config. */
         if (status == ZL303XX_OK)
         {
            pVirtualPortCreate->portNumber = ptpPortCreateS.portNumber;
         }
      }
   }

   /* Initialize the port data structure. */
   if (status == ZL303XX_OK)
   {
      /* Tag this as a Virtual Port. */
      pPtpPort->virtualData.isVirtualPort = ZL303XX_TRUE;
      pPtpPort->virtualData.config = *pVirtualPortCreate;

      /* Save the empty configuration. */
      pPtpPort->config = ptpPortCreateS;

      /* Store a pointer to the associated clock. */
      pPtpPort->clock = pPtpClock;

      /* Stream list. */
      zl303xx_ListInit(&pPtpPort->streamList);
      pPtpPort->streamCount = 0;

      /* Copy the clock identity structure to the port identity of the portDS. */
      OS_MEMCPY(pPtpPort->portDS.portIdentity.clockIdentity,
             pPtpClock->config.defaultDS.clockIdentity,
             PTP_V2_CLK_IDENTITY_LENGTH);

      /* Set the PTP port number in the portDS. This may be a user-specified
       * value or auto-generated. */
      pPtpPort->portDS.portIdentity.portNumber = pPtpPort->config.portNumber;

      /* Set the PTP version in the portDS. This may be a user-specified value
       * or auto-generated. */
      pPtpPort->portDS.versionNumber = pPtpPort->config.ptpVersion;

      /* Set the PTP port delayMechanism. */
      pPtpPort->portDS.delayMechanism = pPtpPort->config.delayMechanism;

      /* Set the unicast parameters */
      if (pPtpPort->config.unicastNegotiation.enabled == ZL303XX_TRUE)
      {
         pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].minIntvl = ptpPortCreateS.unicastNegotiation.maxAnnounceRate;
         pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].maxDurationSec = ptpPortCreateS.unicastNegotiation.maxAnnounceGrantSecs;
         pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_ANNOUNCE].maxCount = ptpPortCreateS.unicastNegotiation.maxAnnounceCount;
         pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_SYNC].minIntvl = ptpPortCreateS.unicastNegotiation.maxSyncRate;
         pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_SYNC].maxDurationSec = ptpPortCreateS.unicastNegotiation.maxSyncGrantSecs;
         pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_SYNC].maxCount = ptpPortCreateS.unicastNegotiation.maxSyncCount;
         pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].minIntvl = ptpPortCreateS.unicastNegotiation.maxDelayRespRate;
         pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].maxDurationSec = ptpPortCreateS.unicastNegotiation.maxDelayRespGrantSecs;
         pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_DELAY_RESP].maxCount = ptpPortCreateS.unicastNegotiation.maxDelayRespCount;
         pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].minIntvl = ptpPortCreateS.unicastNegotiation.maxPdelayRespRate;
         pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].maxDurationSec = ptpPortCreateS.unicastNegotiation.maxPdelayRespGrantSecs;
         pPtpPort->contractLimits[PTP_NEGOTIATED_MSG_ID_PDELAY_RESP].maxCount = ptpPortCreateS.unicastNegotiation.maxPdelayRespCount;
      }

      /* Initialize the acceptable master table, regardless of the acceptable master status. */
      if (status == ZL303XX_OK)
      {
         status = zl303xx_PtpAccMasterTblCreate(
                     ptpPortCreateS.acceptableMasterTableLength,
                     &pPtpPort->accMasterTbl);

         /* Note: The dropped-packet counters for this table are set to '0'
          *       at port creation since OS_CALLOC was used to initialize
          *       the memory. We force it here for completeness. */
         OS_MEMSET(&pPtpPort->amtDroppedMsgCount, 0x00,
                sizeof(pPtpPort->amtDroppedMsgCount)); /* sizeof the declared array */
      }

      /* Initialize the acceptable slave table, regardless of the acceptable slave status. */
      if (status == ZL303XX_OK)
      {
         status = zl303xx_PtpAccSlaveTblCreate(
                     ptpPortCreateS.acceptableMasterTableLength,
                     &pPtpPort->accSlaveTbl);
      }

      /* Initialize the grant table */
      if (status == ZL303XX_OK)
      {
         status = zl303xx_PtpGrantTblCreate(
                     ptpPortCreateS.unicastNegotiation.grantTableLength,
                     &pPtpPort->grantTbl);
      }
   }

   if (status == ZL303XX_OK)
   {
      /* Since this is not a packet stream, DISABLE it. */
      pPtpPort->portSuperState = PTP_PORT_INT_STATE_DISABLED;

      /* Initialize the Unicast Master Table for this port. */
      zl303xx_PtpUmtStructInit(&pPtpPort->unicastMasterTable);
      status = zl303xx_PtpUmtCreate(&pPtpPort->unicastMasterTable,
                                    ptpPortCreateS.unicastMasterTable.maxTableSize);
   }

   if (status == ZL303XX_OK)
   {
      /* Set the query interval for setting up unicast contracts for use when the master
         does not respond immediately */
      pPtpPort->unicastMasterTable.logQueryInterval = ptpPortCreateS.unicastMasterTable.logQueryInterval;
   }

   /* Note that variables that are not explicitly initialized will be set to
    * zero because the structure was initialized to zero by OS_CALLOC(). */

   /* Add the port to the clock's portList. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockAttachPort(pPtpClock, pPtpPort);
   }

   if (status == ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
                    "Created PTP clock port for %d", ptpPortCreateS.portNumber, 0,0,0,0,0);

      /* set the output value. This is a unique handle to this clock port. */
      pPtpPort->unicastMasterTable.portHandle = pPtpPort->clockPortHandle;
      *pPortHandle = pPtpPort->clockPortHandle;
   }
   else
       ZL303XX_TRACE_ALWAYS("zl303xx_PtpPortCreate: failure=%d", status, 0,0,0,0,0);

   /* Execute any Profile Specific actions related to the port. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpProfilePortPostCreate(pPtpPort);
   }

   /* Release the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   /* The Virtual Port is collected with the BMCA information like any other
    * data collected from a server. Typically a Master may disable the BMCA_UPLOAD
    * Event since it does not collect data from packet servers. However, it needs
    * to be enabled in order to force the VP data to be uploaded. */
   if ((status == ZL303XX_OK) &&
       (pPtpPort->clock->config.bmca.updateEventSec == 0))
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_PtpVirtualPortCreate:", 0,0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("   >> Enabling BMCA_UPLOAD Event Timer for Virtual Port",
            0,0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("   >> Setting default Timer value to %d",
            ZL303XX_PTP_DEFAULT_CLOCK_EVT_BMCA_UPDATE_SEC, 0,0,0,0,0);

      /* Set the BMCA Update Interval time to 1 second. */
      status = zl303xx_PtpClockBmcaUpdateIntervalSet(
                                 pPtpPort->clock->clockHandle,
                                 ZL303XX_PTP_DEFAULT_CLOCK_EVT_BMCA_UPDATE_SEC);
   }

   /* Force a BMCA Upload to re-evaluate states.   */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpClockBmcaUpdateInitiate(pPtpClock);
   }

   /* The Port failed to create at some point along the way. Attempt to FREE any
    * associated memory and delete the object from the Clock.  */
   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS(
            "   PTP port creation FAILED: status = %d.", status, 0,0,0,0,0);

      /* Ignore any return code from the DELETE but return the original status. */
      if (pPtpPort != NULL)
      {
         zl303xx_PtpPortDeleteS portDelete;
         (void)zl303xx_PtpPortDeleteStructInit(&portDelete);
         (void)zl303xx_PtpPortDeleteInternal(pPtpPort, &portDelete);
      }

      /* We expect the above call to free the memory but in case it had an
       * error, Free the memory manually. */
      if (pPtpPort != NULL)
      {
         OS_FREE(pPtpPort);
         pPtpPort = NULL;
      }
   }

   return status;
}  /* END zl303xx_PtpVirtualPortCreate */

/* zl303xx_PtpVirtualPortDelete */
/**
   Deletes a Virtual PTP port.

  Parameters:
   [in]   portHandle   Handle to an existing PTP Virtual port.
   [in]   pPortDelete  (Optional) Parameters to shut down this PTP port.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle is invalid or not a Virtual Port.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpVirtualPortDelete(
      zl303xx_PtpPortHandleT portHandle,
      zl303xx_PtpVirtualPortDeleteS *pPortDelete)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_PortDataS *pPtpPort = NULL;
   zl303xx_PtpClockS *pPtpClock = NULL;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpVirtualPortDelete:", 0,0,0,0,0,0);

   /* Don't bother checking pPortDelete, since it does not contain any members. */
   if (pPortDelete) {;}

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      if ((status = ZL303XX_CHECK_PORT_HANDLE(portHandle)) != ZL303XX_OK)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Invalid portHandle=%lu.", portHandle, 0,0,0,0,0);
      }
   }

   if (status == ZL303XX_OK)
   {
      /* Get a pointer to the port data. Don't call zl303xx_PtpPortDataGet()
       * because we want to exit the function with ZL303XX_OK if the port does
       * not exist. */
      pPtpPort = Zl303xx_PtpPortArray[portHandle];

      if (pPtpPort == NULL)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "   No port with handle=%lu exists.",
               portHandle, 0,0,0,0,0);

         return ZL303XX_OK;
      }

      else if (pPtpPort->virtualData.isVirtualPort == ZL303XX_FALSE)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "   Port=%lu not a Virtual Port.",
               portHandle, 0,0,0,0,0);
         return ZL303XX_PARAMETER_INVALID;
      }

      /* Lock the clock mutex. */
      pPtpClock = pPtpPort->clock;
      mStatus = zl303xx_PtpClockMutexLock(pPtpClock);
      status = mStatus;
   }

   /* Call the internal port delete function. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDeleteInternal(pPtpPort, NULL);
   }

   /* Release the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   return status;
}

/* zl303xx_PtpVirtualPortPtsfSet */
/**
   Enabled or disables the data of a virtual port by setting its PTSF flag. When
   enabled (PTSF = FALSE), the static configuration is used in the BMCA
   selection; when disabled (PTSF = TRUE), it is not uploaded for BMCA
   selection. By default, virtual ports are DISABLED when created and must be
   ENABLED by the user.

  Parameters:
   [in]  portHandle  Handle to an existing port.
   [in]  ptsf        Flag indicating if the virtual port is failed.

  Return Value:
     ZL303XX_OK                     Success.
     ZL303XX_PARAMETER_INVALID      portHandle or ptsf parameters invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpVirtualPortPtsfSet(
      zl303xx_PtpPortHandleT portHandle,
      zl303xx_BooleanE ptsf)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPtpPort;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(ptsf);
   }

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpVirtualPortDataGet(portHandle, &pPtpPort);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpPort->clock);
   }

   /* Update the enabled setting. */
   if (status == ZL303XX_OK)
   {
      pPtpPort->virtualData.config.ptsf = ptsf;
      status = zl303xx_PtpClockMutexUnlock(pPtpPort->clock, status);
   }

   /* Any time a Port configuration is changed, the Clock State-Decision
    * algorithm should be run to re-evaluate its state (and that of any
    * associated ports & streams).   */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpClockBmcaUpdateInitiate(pPtpPort->clock);
   }

   return status;
}  /* END zl303xx_PtpVirtualPortPtsfSet */

/* zl303xx_PtpVirtualPortSet */
/**
   Sets the configuration data associated with the specified virtual port.

  Parameters:
   [in]  portHandle           Handle to an existing port.
   [in]  pVirtualPortConfig   Virtual port configuration Data to set.

  Return Value:
     ZL303XX_OK                     Success.
     ZL303XX_INVALID_POINTER        pVirtualPortConfig is NULL.
     ZL303XX_PARAMETER_INVALID      portHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpVirtualPortSet(
      zl303xx_PtpPortHandleT portHandle,
      zl303xx_PtpVirtualPortConfigS *pVirtualPortConfig)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPtpPort;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pVirtualPortConfig);
   }

   /* Get a pointer to the Virtual port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpVirtualPortDataGet(portHandle, &pPtpPort);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpPort->clock);
   }

   /* Update the enabled setting. */
   if (status == ZL303XX_OK)
   {
      pPtpPort->virtualData.config = *pVirtualPortConfig;
      status = zl303xx_PtpClockMutexUnlock(pPtpPort->clock, status);
   }

   /* Any time a Virtual Port configuration is changed, the Clock State-Decision
    * algorithm should be run to re-evaluate all clock, port and stream states. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtpClockBmcaUpdateInitiate(pPtpPort->clock);
   }

   return status;
}  /* END zl303xx_PtpVirtualPortSet */

/*****************   INTERNAL PTP FUNCTION DEFINITIONS   **********************/

/* zl303xx_PtpPortDeleteInternal */
/** 

   Internal function to delete a port. Will either be called via the user API
   port delete function or by the clock delete function.

  Parameters:
   [in]   port         Pointer to a port data structure.
   [in]   pPortDelete  Optional parameters to shut down the port.

  Notes:   Assumes all input parameters are valid. Assumes the clock mutex has
        already been taken.

*******************************************************************************/
zlStatusE zl303xx_PtpPortDeleteInternal(zl303xx_PortDataS *port,
                                        zl303xx_PtpPortDeleteS *pPortDelete)
{
   zlStatusE status = ZL303XX_OK;

   /* Warning removal: the pPortDelete parameter is currently not required. */
   (void)pPortDelete;

   if (status == ZL303XX_OK)
   {
      /* Delete all streams attached to this port. */
      zl303xx_PtpPortDeleteAllStreams(port);
   }

   if (status == ZL303XX_OK)
   {
      /* Delete all port address tables created on this port. */
      /* We do not clear stats counters here since the Port is about to be
       * deleted. */
      zl303xx_PtpAccMasterTblDelete(&port->accMasterTbl);
      zl303xx_PtpAccSlaveTblDelete(&port->accSlaveTbl);
      zl303xx_PtpGrantTblDelete(&port->grantTbl);

      /* Delete the unicast master table. */
      zl303xx_PtpUmtDelete(&port->unicastMasterTable);

      /* If the clock has selected this port as the source of the grandMaster
       * data, then detach the pointer before deleting. */
      if (port->clock->gmSource.pPtpPort == port)
      {
         port->clock->gmSource.pPtpPort = NULL;
      }

      /* Detach the port from the clock's portList. */
      status = zl303xx_PtpClockDetachPort(port->clock, port);
   }

   /* Free the port structure memory. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortStructFree(&port);
   }

   return status;
}

/* zl303xx_PtpPortDeleteAllStreams */
/** 

   Deletes all streams attached to a port.

  Parameters:
   [in]   port  Pointer to a port data structure.

  Notes:   Assumes all input parameters are valid. Assumes the clock mutex has
        already been taken.

*******************************************************************************/
zlStatusE zl303xx_PtpPortDeleteAllStreams(zl303xx_PortDataS *port)
{
   zlStatusE status = ZL303XX_OK;

   Uint32T streamHandle;
   zl303xx_ClockStreamS *pPtpStream;
   zl303xx_PtpEventStreamDeleteS eventData;

   /* Delete any streams attached to this port. */
   if (status == ZL303XX_OK)
   {
      /* Loop through the entire stream array to delete all stream created on
       * this port.    */
      /* We could follow the linked list of streams attached to the port but
       * since this is the only time all streams are deleted, the inefficiency
       * of looping the entire stream array is a safe and acceptable approach. */
      for (streamHandle = 0; streamHandle < ZL303XX_PTP_NUM_STREAMS_MAX; streamHandle++)
      {
         pPtpStream = Zl303xx_PtpStreamArray[streamHandle];
         if ((pPtpStream != NULL) &&
             (pPtpStream->portData == port))
         {
            /* Gather data for the STREAM_DELETE event. The event handler will be
             * invoked because this is an automatic stream delete. */
            if (port->clock->config.eventCallback != NULL)
            {
               eventData.streamHandle = pPtpStream->clockStreamHandle;
               eventData.clockHandle = pPtpStream->portData->clock->clockHandle;
               eventData.streamExtData = pPtpStream->config.extData;
               eventData.streamConfig = pPtpStream->config;
            }

            status = zl303xx_PtpStreamDeleteInternal(pPtpStream, NULL);

            /* Call the event handler, if it exists. */
            if (status == ZL303XX_OK)
            {
               if (port->clock->config.eventCallback != NULL)
               {
                  port->clock->config.eventCallback(
                     ZL303XX_PTP_EVENT_STREAM_DELETE, &eventData);
               }
            }
            else
            {
               ZL303XX_TRACE_ALWAYS("zl303xx_PtpPortDeleteAllStreams: ERROR removing stream (%d) on port (%d)",
                     streamHandle, port->clockPortHandle,0,0,0,0);
               break;
            }
         }
      }
   }

   return status;
}

/* zl303xx_PtpPortStructAllocate */
/** 

   Allocates memory for a PTP port data structure.

  Parameters:
   [in]  pPortCreate    Structure containing the Port creation parameters.

   [out] ppPort      Pointer to the allocated structure.

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
zlStatusE zl303xx_PtpPortStructAllocate(
      zl303xx_PtpPortCreateS *pPortCreate,
      zl303xx_PortDataS **ppPort)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *newPort = NULL;

   /* Warning removal: the PortCreateS is unused but keep it as a parameter. */
   if (pPortCreate) {;}

   /* Allocate memory for the clock structure. */
   if (status == ZL303XX_OK)
   {
      newPort = (zl303xx_PortDataS *)OS_CALLOC(1, sizeof(zl303xx_PortDataS));

      if (newPort == NULL)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Port memory allocate failed.",
                             0,0,0,0,0,0);
         status = ZL303XX_RTOS_MEMORY_FAIL;
      }
   }

   /* Return the port pointer to the caller. */
   if (status == ZL303XX_OK)
   {
      *ppPort = newPort;
   }

   return status;
}

/* zl303xx_PtpPortStructFree */
/** 

   Frees memory used by a PTP port data structure.

  Parameters:
   [in,out]  ppPort  Pointer to the previously allocated structure. It will
                          point to NULL after this function completes.

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
zlStatusE zl303xx_PtpPortStructFree(zl303xx_PortDataS **ppPort)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      /* Free the port object. */
      OS_FREE(*ppPort);
      *ppPort = NULL;
   }

   return status;
}

/* zl303xx_PtpPortAttachStream */
/** 

   Adds a stream to a port's streamList.

  Parameters:
   [in]      port    Pointer to the port that the stream will be attached to.
   [in,out]  stream  Pointer to the stream to attach. This function modifies
                          the zl303xx_ClockStreamS::clockStreamHandle member.

  Notes:   Assumes all input parameters are valid. Assumes the clock mutex has
        already been taken prior to calling this function.

*******************************************************************************/
zlStatusE zl303xx_PtpPortAttachStream(zl303xx_PortDataS *port, zl303xx_ClockStreamS *stream)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   Uint32T n = 0;

   /* Lock the node mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpNodeMutexLock();
      status = mStatus;
   }

   /* requestedHandle must be a valid array index and not already in use, or -1 */
   if (status == ZL303XX_OK)
   {
      /* These checks are not done in zl303xx_PtpStreamCreateStructCheck() because there
       * is a (very unlikely) race condition where two streams could be created with
       * the same requested handle. So, only do checks when holding the node mutex. */
      if (stream->config.requestedHandle != (zl303xx_PtpStreamHandleT)ZL303XX_PTP_INVALID)
      {
         n = stream->config.requestedHandle;

         if (ZL303XX_CHECK_STREAM_HANDLE(n) != ZL303XX_OK)
         {
            ZL303XX_TRACE_ERROR("   Port (ERROR) requestedHandle=%u is out of range.",
                                n, 0,0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
         }
         else if (Zl303xx_PtpStreamArray[n] != NULL)
         {
            ZL303XX_TRACE_ERROR("   Port (ERROR) requestedHandle=%u is already in use.",
                                n, 0,0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
         }
      }
      /* requestedHandle is -1, so find an empty stream array entry */
      else
      {
         for (n = 0; n < ZL303XX_PTP_NUM_STREAMS_MAX; n++)
         {
            if (Zl303xx_PtpStreamArray[n] == NULL)
               break;
         }

         if (n == ZL303XX_PTP_NUM_STREAMS_MAX)
         {
            ZL303XX_TRACE_ERROR("   (ERROR) Maximum number of streams attached (%lu).",
                                ZL303XX_PTP_NUM_STREAMS_MAX, 0,0,0,0,0);
            status = ZL303XX_TABLE_FULL;
         }
      }
   }

   if (status == ZL303XX_OK)
   {
      Zl303xx_PtpStreamArray[n] = stream;
      stream->clockStreamHandle = n;
   }

   /* Release the node mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpNodeMutexUnlock(status);
   }

   /* Add the stream to the port's streamList. */
   if (status == ZL303XX_OK)
   {
      zl303xx_ListAdd(&port->streamList, &stream->listEntry);
      port->streamCount++;
   }

   return status;
}

/* zl303xx_PtpPortDetachStream */
/** 

   Removes a stream from a port's streamList.

  Parameters:
   [in]      port    Pointer to the port that the stream will be detached from.
   [in,out]  stream  Pointer to the stream to detach. This function modifies
                          the zl303xx_ClockStreamS::index member.

  Notes:   Assumes all input parameters are valid. Assumes the clock mutex has
        already been taken prior to calling this function.

*******************************************************************************/
zlStatusE zl303xx_PtpPortDetachStream(zl303xx_PortDataS *port, zl303xx_ClockStreamS *stream)
{
   /* Lock the node mutex. */
   zlStatusE status = ZL303XX_OK;

   /* Remove the stream from the port's streamList. */
   if (status == ZL303XX_OK)
   {
      zl303xx_ListRemove(&stream->listEntry);
      port->streamCount--;

      status = zl303xx_PtpNodeMutexLock();
   }

   /* Remove the stream from the global node array. */
   if (status == ZL303XX_OK)
   {
      Zl303xx_PtpStreamArray[stream->clockStreamHandle] = NULL;
      status = zl303xx_PtpNodeMutexUnlock(status);
   }

   return status;
}

/* zl303xx_PtpPortDataGet */
/** 

   Returns a pointer to an internal port data structure.

  Parameters:
   [in]   portHandle  Handle to an existing port.
   [out]  ppPort      Pointer to the port data structure.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    ppPort is NULL.
     ZL303XX_PARAMETER_INVALID  portHandle is out of range or references a
                                    port that does not exist.

  Notes:
   For internal API use only. Should not be called by an application.

*******************************************************************************/
zlStatusE zl303xx_PtpPortDataGet(zl303xx_PtpPortHandleT portHandle,
                                 zl303xx_PortDataS **ppPort)
{
   zlStatusE status = ZL303XX_OK;
   const char *fnName = "zl303xx_PtpPortDataGet";

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(ppPort);
   }

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_PORT_HANDLE(portHandle);
      if (status != ZL303XX_OK)
      {
         ZL303XX_TRACE_ALWAYS("%s: (ERROR) portHandle=%u out of range (0-%u)",
               fnName, portHandle, ZL303XX_PTP_NUM_PORTS_MAX - 1, 0,0,0);
      }
   }

   if (status == ZL303XX_OK)
   {
      *ppPort = Zl303xx_PtpPortArray[portHandle];
      if (*ppPort == NULL)
      {
         ZL303XX_TRACE_ALWAYS("%s: portHandle=%u not initialized",
               fnName, portHandle, 0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   return status;
}

/* zl303xx_PtpPortDataGetForProfile */
/** 

   Returns a pointer to an internal port data structure ONLY IF the port is
   operating the indicated PTP Profile.

   With the addition of PTP Profiles and unions to store profile specific data,
   a port must be running the indicated profile in order for certain data to
   map correctly to the profile specific structure.

  Parameters:
   [in]  portHandle     Handle to an existing port.
   [in]  ptpProfile     Required PTP Profile.

   [out]  ppPort        Pointer to the Port data structure.

  Return Value:
    ZL303XX_OK                 Success.
    ZL303XX_INVALID_POINTER    ppPort is NULL.
    ZL303XX_PARAMETER_INVALID  portHandle is out of range, references a
                                    port that does not exist.
    ZL303XX_INVALID_MODE       portHandle references a port that is not
                                    running the required PTP profile.

  Notes:
   For internal API use only. Should not be called by an application.

*******************************************************************************/
zlStatusE zl303xx_PtpPortDataGetForProfile(
      zl303xx_PtpPortHandleT portHandle,
      zl303xx_PtpProfileE ptpProfile,
      zl303xx_PortDataS **ppPort)
{
   zlStatusE status = ZL303XX_OK;

   /* First, see if the port even exists. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, ppPort);
   }

   /* Check the Profile value. */
   if (status == ZL303XX_OK)
   {
      if ((*ppPort)->clock->config.profile != ptpProfile)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpPortDataGetForProfile: Port(%u) Profile(%u)(%u expected)",
               portHandle, (*ppPort)->clock->config.profile, ptpProfile, 0,0,0);
         status = ZL303XX_INVALID_MODE;
      }
   }

   return status;
}

/* zl303xx_PtpVirtualPortDataGet */
/** 

   Returns a pointer to an internal port data structure ONLY IF it is
   configured as a Virtual PTP Port.

  Parameters:
   [in]   portHandle  Handle to an existing port.
   [out]  ppPort      Pointer to the port data structure.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    ppPort is NULL.
     ZL303XX_PARAMETER_INVALID  portHandle is out of range, references a
                                    port that does not exist or is not set up
                                    as a Virtual PTP Port.

  Notes:
   For internal API use only. Should not be called by an application.

*******************************************************************************/
zlStatusE zl303xx_PtpVirtualPortDataGet(
      zl303xx_PtpPortHandleT portHandle,
      zl303xx_PortDataS **ppPort)
{
   zlStatusE status = ZL303XX_OK;

   /* Use the regular routine to get the data for the portHandle. It will check
    * pointers, ranges, etc. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, ppPort);
   }

   /* Make sure this is a Virtual Port */
   if (status == ZL303XX_OK)
   {
      if ((*ppPort)->virtualData.isVirtualPort == ZL303XX_FALSE)
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpVirtualPortDataGet: portHandle=%u not Virtual Port",
               portHandle, 0,0,0,0,0);

         *ppPort = NULL;
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   return status;
}

/* zl303xx_PtpPortDataFromPortNumber */
/** 

   Retrieves the Port data instance structure for a particular portNumber on a
   specified Clock (since multiple clocks can exist on a single CPU, the
   portNumber may be reused on each clock).

  Parameters:
   [in]   pPtpClock   Target clock to look for the specified port.
   [in]   portNumber  Port number.

   [out]  portData    Corresponding port data structure. NULL is returned if
                           no matching port is found.

  Notes:   For internal use only. Should not be called by an application

*******************************************************************************/
zlStatusE zl303xx_PtpPortDataFromPortNumber(
      zl303xx_PtpClockS *pPtpClock,
      Uint16T portNumber,
      zl303xx_PortDataS **portData)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T portId;   /* FOR Loop */

   /* Check the Pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pPtpClock) |
               ZL303XX_CHECK_POINTER(portData);
   }

   if (status == ZL303XX_OK)
   {
      /* Return NULL if specified port number has not been created */
      *portData = NULL;

      /* Assume not found. */
      status = ZL303XX_TABLE_ENTRY_NOT_FOUND;

      /* Loop through all created ports */
      for (portId = 0; portId < ZL303XX_PTP_NUM_PORTS_MAX; portId++)
      {
         if (Zl303xx_PtpPortArray[portId] != NULL)
         {
            if ((Zl303xx_PtpPortArray[portId]->config.portNumber == portNumber) &&
                (Zl303xx_PtpPortArray[portId]->clock == pPtpClock))
            {
               *portData = Zl303xx_PtpPortArray[portId];

               /* Mark the entry as found. */
               status = ZL303XX_OK;
               break;
            }
         }
      }
   }

   return status;
}

/* zl303xx_PtpPortDataFromPortIdentity */
/** 

   Retrieves the Port data instance structure for a specified portIdentity
   (assumes that if 2 Clocks exist on the same CPU that the clockIdentities
   are unique).

  Parameters:
   [in]   pPortIdentity    Port Identity to look for on this Node.

   [out]  portData    Corresponding port data structure. NULL is returned if
                           no matching port is found.

  Notes:   For internal use only. Should not be called by an application

*******************************************************************************/
zlStatusE zl303xx_PtpPortDataFromPortIdentity(
      zl303xx_PortIdentity *pPortIdentity,
      zl303xx_PortDataS **portData)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T portId;   /* FOR Loop */

   /* Check the Pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pPortIdentity) |
               ZL303XX_CHECK_POINTER(portData);
   }

   /* We don't really care if the portIdentity is INVALID (i.e portNumber == 0xFFFE),
    * the search will just fail and return. */
   /* However, if the portNumber == 0 or the ALL-Port's value (0xFFFF) then get
    * the first port on the Clock. */
   if (status == ZL303XX_OK)
   {
      /* Return NULL if specified port number has not been created */
      *portData = NULL;

      if ((ZL303XX_TRUE == zl303xx_PtpV2IsAllClockIdentity(pPortIdentity->clockIdentity)) ||
          (pPortIdentity->portNumber == (Uint16T)(0xFFFE)))
      {
         status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
      }
      else if ((pPortIdentity->portNumber == 0) ||
               (pPortIdentity->portNumber == (Uint16T)(0xFFFF)))
      {
         /* Find the first port on this clock. */

         /* Assume not found. */
         status = ZL303XX_TABLE_ENTRY_NOT_FOUND;

         /* Loop through all created ports */
         for (portId = 0; portId < ZL303XX_PTP_NUM_PORTS_MAX; portId++)
         {
            if (Zl303xx_PtpPortArray[portId] != NULL)
            {
               if (ZL303XX_TRUE == zl303xx_PtpV2EqualClockIdentities(
                                       Zl303xx_PtpPortArray[portId]->portDS.portIdentity.clockIdentity,
                                       pPortIdentity->clockIdentity))
               {
                  *portData = Zl303xx_PtpPortArray[portId];

                  /* Mark the entry as found. */
                  status = ZL303XX_OK;
                  break;
               }
            }
         }
      }
      else /* Search for the port Identity. */
      {
         /* Assume not found. */
         status = ZL303XX_TABLE_ENTRY_NOT_FOUND;


         /* Loop through all created ports */
         for (portId = 0; portId < ZL303XX_PTP_NUM_PORTS_MAX; portId++)
         {
            if (Zl303xx_PtpPortArray[portId] != NULL)
            {
               if (ZL303XX_TRUE == zl303xx_PtpV2EqualPortIdentities(
                                       &Zl303xx_PtpPortArray[portId]->portDS.portIdentity,
                                       pPortIdentity))
               {
                  *portData = Zl303xx_PtpPortArray[portId];

                  /* Mark the entry as found. */
                  status = ZL303XX_OK;
                  break;
               }
            }
         }
      }
   }

   return status;
}

/* zl303xx_PtpGrantTblCheck */
/** 

   Determines if a contract will be granted or rejected based on the grant
   table. If an entry for this port address exists, the messageType bit-mask
   determines whether the contract will be granted. Otherwise, the default grant
   behaviour is used.

  Parameters:
   [in]   pPort    Pointer to an existing port.
   [in]   pAddr    The port address to search for.
   [in]   msgType  The message type of the contract being requested.

  Return Value:
     ZL303XX_FALSE  The contract is denied due to override, or default setting.
     ZL303XX_TRUE   The contract can be granted.

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtpGrantTblCheck(zl303xx_PortDataS *pPort,
                                          zl303xx_PortAddress *pAddr,
                                          zl303xx_MessageTypeE msgType)
{
   zl303xx_PtpGrantConfigS *grantEntry;

   grantEntry = zl303xx_PtpTableFind(&pPort->grantTbl, pAddr);
   if (grantEntry == NULL)
   {
   	/* At this point denyServiceRequests should be FALSE since it was checked
   	 * for TRUE at a higher layer in zl303xx_PtpV2CheckUnicastRequest.
   	 * Still, let it do the inversion below in case something is rearranged
   	 * in the future.  	 */
      return !(pPort->config.unicastNegotiation.denyServiceRequests);
   }

   /* Convert non-zero value to 1 (ZL303XX_TRUE). */
   return !!ZL303XX_BIT_TEST(grantEntry->msgTypeMask, msgType);
}

/* zl303xx_PtpGrantEntryUpdate */
/** 

   Updates a single message type for a port address in a port's grant table.

  Parameters:
   [in]   pPort    Pointer to an existing port.
   [in]   addr     The port address to search for.
   [in]   msgType  The message type to update.
   [in]   val      ZL303XX_TRUE to grant contracts for msgType. ZL303XX_FALSE
                        to deny.

*******************************************************************************/
void zl303xx_PtpGrantEntryUpdate(zl303xx_PortDataS *pPort,
                                 zl303xx_PortAddress *addr,
                                 zl303xx_MessageTypeE msgType,
                                 zl303xx_BooleanE val)
{
   zl303xx_PtpGrantConfigS *grantEntry;

   grantEntry = zl303xx_PtpTableAdd(&pPort->grantTbl, addr);
   if (grantEntry == NULL)
   {
      return;
   }

   if (val)
      ZL303XX_BIT_SET(grantEntry->msgTypeMask, msgType);
   else
      ZL303XX_BIT_CLEAR(grantEntry->msgTypeMask, msgType);
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/* zl303xx_PtpPortCreateStructCheck */
/** 

   Verifies that all of the members of a zl303xx_PtpPortCreateS have been
   set correctly.

  Parameters:
   [in]   portCreate  Structure to verify.

*******************************************************************************/
static zlStatusE zl303xx_PtpPortCreateStructCheck(zl303xx_PtpPortCreateS *portCreate,
                                                  zl303xx_PtpClockS *pPtpClock)
{
   zlStatusE status = ZL303XX_OK;

   /* portNumber cannot be all ones -- this value is reserved by the standard. */
   if (portCreate->portNumber == (Uint16T)0xFFFF)
   {
      ZL303XX_TRACE_ALWAYS("   (ERROR) portNumber=0xFFFF is reserved.", 0,0,0,0,0,0);
      status = ZL303XX_PARAMETER_INVALID;
   }

   /* portNumber should not be greater than the maximum number of ports supported
    * on the clock. */
   else if (portCreate->portNumber > pPtpClock->defaultDS.numberPorts)
   {
      ZL303XX_TRACE_ALWAYS(
         "   portNumber=%u is greater than DefaultDS::numberPorts=%u.",
         portCreate->portNumber,
         pPtpClock->defaultDS.numberPorts, 0,0,0,0);

      /* Do not return an error code, as Microsemi PTP implementation allows
       * an arbitrary portNumber to be set, even if it violates the standard. */
   }

   /* Check if a port with this portNumber is already attached to the clock. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PortDataS *portData;

      ZL303XX_PTP_FOR_EACH_PORT(&pPtpClock->portList, portData)
      {
         if (portCreate->portNumber == portData->config.portNumber)
         {
            ZL303XX_TRACE_ALWAYS("   A port with portNumber=%u already exists on this clock.",
                                 portCreate->portNumber, 0,0,0,0,0);
            ZL303XX_TRACE_ALWAYS("   A unique portNumber will be generated.", 0,0,0,0,0,0);

            /* Set portNumber to 0 to have it auto-generated later during creation. */
            portCreate->portNumber = 0;
         }
      }
   }

   /* Range check on delayMechanism. Current implementation does not support P2P. */
   if (status == ZL303XX_OK)
   {
      if ((portCreate->delayMechanism != ZL303XX_DELAY_MECHANISM_E2E) &&
          (portCreate->delayMechanism != ZL303XX_DELAY_MECHANISM_P2P) &&
          (portCreate->delayMechanism != ZL303XX_DELAY_MECHANISM_DISABLED))
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Invalid delayMechanism=0x%02X.",
                             portCreate->delayMechanism, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Range check on booleans. */
   if (status == ZL303XX_OK)
   {
      status |= ZL303XX_CHECK_BOOLEAN(portCreate->acceptableMasterTableEnabled) |
                ZL303XX_CHECK_BOOLEAN(portCreate->acceptableSlaveTableEnabled)  |
                ZL303XX_CHECK_BOOLEAN(portCreate->mixedMode) |
                ZL303XX_CHECK_BOOLEAN(portCreate->unicastNegotiation.enabled) |
                ZL303XX_CHECK_BOOLEAN(portCreate->unicastNegotiation.stackedTlvEn) |
                ZL303XX_CHECK_BOOLEAN(portCreate->unicastNegotiation.denyServiceRequests) |
                ZL303XX_CHECK_BOOLEAN(portCreate->rateRatioEnabled);

      status |= zl303xx_PtpPortAddressCheck(&portCreate->localAddr);
   }

   if (status == ZL303XX_OK)
   {
      /* Warn if configured to never grant any contracts. */
      if ((portCreate->unicastNegotiation.denyServiceRequests == ZL303XX_TRUE) &&
          (portCreate->unicastNegotiation.grantTableLength == 0))
      {
         ZL303XX_TRACE_ALWAYS(
            "   Warning: defaultGrant=FALSE and maxContractTableSize=0. No contracts can be granted!",
            0,0,0,0,0,0);
      }
   }
   
   /* Check that the value of numRateRatioPkts is within range. */
   if(status == ZL303XX_OK)
   {
      
      status = zl303xx_PtpNumRateRatioPktsValueCheck(portCreate->numRateRatioPkts); 
       
   }

   return status;
}

/* zl303xx_PtpPortNumberGenerate */
/** 

   Generates a value for zl303xx_PortDS::portIdentity::portNumber.

  Parameters:
   [in]   pPtpClock   The clock this port will be attached to.
   [out]  portNumber  A unique port number for this clock.

*******************************************************************************/
static zlStatusE zl303xx_PtpPortNumberGenerate(zl303xx_PtpClockS *pPtpClock,
                                               Uint16T *portNumber)
{
   zlStatusE status = ZL303XX_OK;
   Uint16T n;
   zl303xx_BooleanE nAvailable;
   zl303xx_PortDataS *portData = NULL;

   /* Try portNumbers 1..n where n is the maximum number of supported ports. */
   /* Note: port[0] is not used, so if maxPorts is 2 then the indexes must run
    *       from 0 to 2 (thus <=). */
   for (n = 1; n <= ZL303XX_PTP_NUM_PORTS_MAX; n++)
   {
      nAvailable = ZL303XX_TRUE;

      /* Check all ports attached to this clock to see if that portNumber is
       * already in use. */
      ZL303XX_PTP_FOR_EACH_PORT(&pPtpClock->portList, portData)
      {
         if (n == portData->config.portNumber)
         {
            nAvailable = ZL303XX_FALSE;
            break;
         }
      }

      /* If the above loop did not break, then this portNumber is not in use. */
      if (nAvailable)
      {
         *portNumber = n;
         break;
      }
   }

   /* Make sure a port number has been generated. */
   if (*portNumber == 0)
   {
      status = ZL303XX_TABLE_FULL;
   }

   return status;
}

/* zl303xx_PtpGrantTblCreate */
/** 

   Allocates memory and initializes a grant table used to check contract
   requests.

  Parameters:
   [in]   maxEntries  Number of entries to allocate for the table.
   [out]  grantTbl    Initialized grant table.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_RTOS_MEMORY_FAIL  Failed to allocate memory.

*******************************************************************************/
static zlStatusE zl303xx_PtpGrantTblCreate(Uint16T maxEntries,
                                           zl303xx_PtpTableS *grantTbl)
{
   return zl303xx_PtpTableCreate(maxEntries, sizeof(zl303xx_PtpGrantConfigS),
                                 &Zl303xx_PtpGrantTblOps, grantTbl);
}

/* zl303xx_PtpGrantTblDelete */
/** 

   Allocates memory and initializes a grant table used to check contract
   requests.

  Parameters:
   [in,out]  grantTbl  Pointer to a grant table to delete.

*******************************************************************************/
static void zl303xx_PtpGrantTblDelete(zl303xx_PtpTableS *grantTbl)
{
   zl303xx_PtpTableDestroy(grantTbl);
}

/* zl303xx_PtpGrantEntryInit */
/** 

   Callback function for zl303xx_PtpTableOpsS::init().

  Parameters:
   [in]   entry  Pointer to the grant table entry structure.
   [in]   key    Pointer to the portAddress passed to zl303xx_PtpTableAdd().

*******************************************************************************/
static void zl303xx_PtpGrantEntryInit(const void *key, void *entry)
{
   const zl303xx_PortAddress *addr = key;
   zl303xx_PtpGrantConfigS *grantEntry = entry;

   grantEntry->portAddress = *addr;

   /* Initialize the mask to all negotiable message types so internal table
    * adds don't affect existing contracts. */
   grantEntry->msgTypeMask = ZL303XX_PTP_V2_VALID_UNI_NEG_MSG_TYPES_MASK;
}

/* zl303xx_PtpGrantEntryMatch */
/** 

   Callback function for zl303xx_PtpTableOpsS::keyMatch(). Determines if a grant
   table entry matches the portAddress (key).

  Parameters:
   [in]   entry  Pointer to the grant table entry structure.
   [in]   key    Pointer to the portAddress passed to zl303xx_PtpTableFind().

*******************************************************************************/
static zl303xx_BooleanE zl303xx_PtpGrantEntryMatch(const void *key, const void *entry)
{
   const zl303xx_PtpGrantConfigS *grantEntry = entry;
   return !zl303xx_PtpComparePortAddress(&grantEntry->portAddress, key);
}

/* zl303xx_PtpPortContractCountGet */
/**
   Gets the number of active contracts of specified type on a given port.

  Parameters:
   [in]   portHandle  Handle of the port to get the limit for.
   [in]   type        The contract type to get.
   [out]  count       Pointer to variable that will accept the count result.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle or type is not valid.

*******************************************************************************/
zlStatusE zl303xx_PtpPortContractCountGet(zl303xx_PtpPortHandleT portHandle,
                                          zl303xx_MessageTypeE msgType,
                                          Uint32T *count)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort;
   zl303xx_V2UnicastNegotiatedMsgTypeE type;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(count);
   }

   /* Get a pointer to the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MessageTypeToIndex(msgType, &type);
   }

   if (status == ZL303XX_OK)
   {
      *count = zl303xx_PtpV2GetPortContractCount(pPort, NULL, type);
   }

   return status;
}

/* zl303xx_PtpPortContractCountMaxGet */
/**
   Gets the maximum number of contracts of specified type allowed on a given port.

  Parameters:
   [in]   portHandle  Handle of the port to get the limit for.
   [in]   type        The contract type to get.
   [out]  limit       Pointer to variable that will accept the limit result
                           (-1 is unlimited).

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle or type is not valid.

*******************************************************************************/
zlStatusE zl303xx_PtpPortContractCountMaxGet(
      zl303xx_PtpPortHandleT portHandle,
      zl303xx_MessageTypeE msgType,
      Uint32T *limit)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort;
   zl303xx_V2UnicastNegotiatedMsgTypeE type;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(limit);
   }

   /* Get a pointer to the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MessageTypeToIndex(msgType, &type);
   }

   if (status == ZL303XX_OK)
   {
      *limit = pPort->contractLimits[type].maxCount;
   }

   return status;
}

/* zl303xx_PtpPortContractCountMaxSet */
/**
   Sets the maximum number of contracts of specified type allowed on a given port.

   In the event that this parameter is lowered at run-time, to a value less than
   the current number of existing contracts, then those existing contracts will
   be permitted to expire normally. (Since the IEEE-1588 Std suggests that once
   a contract is GRANTED it should be honored for the duration of the contract
   and it would be unfair to cancel one contract but allow another to continue).

  Parameters:
   [in]   portHandle  Handle of the port to set the limit for.
   [in]   type        The contract type to set.
   [in]   limit       Limit value to set (-1 is unlimited).

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle or type is not valid.

*******************************************************************************/
zlStatusE zl303xx_PtpPortContractCountMaxSet(
      zl303xx_PtpPortHandleT portHandle,
      zl303xx_MessageTypeE msgType,
      Uint32T limit)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort;
   zl303xx_V2UnicastNegotiatedMsgTypeE type;

   /* Get a pointer to the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MessageTypeToIndex(msgType, &type);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPort->clock);
   }

   /* Set the limit; Unlock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      pPort->contractLimits[type].maxCount = limit;

      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }

   return status;
}

/* zl303xx_PtpPortContractDurationMaxGet */
/**
   Returns the maximum duration of unicast negotiation contracts that can be
   granted by this port for a specific messageType.

  Parameters:
   [in]  portHandle     Handle to an existing port.
   [in]  msgType        The messageType to query.
   [out] durationMax    Maximum duration of a contract that can be granted for
                              messageType on this port.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    durationMax is NULL.
     ZL303XX_PARAMETER_INVALID  portHandle or msgType is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpPortContractDurationMaxGet(
      zl303xx_PtpPortHandleT portHandle,
      zl303xx_MessageTypeE msgType,
      Uint32T *durationMax)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPtpPort;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(durationMax);
   }

   /* Get a pointer to the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPtpPort);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MessageTypeToIndex(msgType, &uniNegMsgType);
   }

   if (status == ZL303XX_OK)
   {
      *durationMax = pPtpPort->contractLimits[uniNegMsgType].maxDurationSec;
   }

   return status;
}

/* zl303xx_PtpPortContractDurationMaxSet */
/**
   Sets the maximum duration of unicast negotiation contracts that can be
   granted by this port for a specific messageType.

   In the event that this parameter is lowered at run-time, to a value less than
   the current duration of any existing contract, then those contracts will be
   permitted to expire normally. (Since the IEEE-1588 Std suggests that once a
   contract is GRANTED it should be honored for the duration).

  Parameters:
   [in]  portHandle     Handle to an existing port.
   [in]  msgType        The messageType to limit.
   [in]  durationMax    Maximum duration of a contract that can be granted for
                              messageType on this port.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  portHandle or msgType is invalid OR maxDuration
                                    exceeds the allowed PTP range.

*******************************************************************************/
zlStatusE zl303xx_PtpPortContractDurationMaxSet(
      zl303xx_PtpPortHandleT portHandle,
      zl303xx_MessageTypeE msgType,
      Uint32T durationMax)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPtpPort;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgtype;

   /* Get a pointer to the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPtpPort);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MessageTypeToIndex(msgType, &uniNegMsgtype);
   }

   /* Check the limit for the Message-Type specified. */
   if (status == ZL303XX_OK)
   {
      if ((durationMax < ZL303XX_PTP_UNI_NEG_DURATION_MIN) ||
          (durationMax > ZL303XX_PTP_UNI_NEG_DURATION_MAX))
      {
         ZL303XX_TRACE_ALWAYS(
               "zl303xx_PtpPortContractDurationMaxSet: duration(%d) outside allowed Range (%u - %u)",
               durationMax,
               ZL303XX_PTP_UNI_NEG_DURATION_MIN,
               ZL303XX_PTP_UNI_NEG_DURATION_MAX, 0,0,0);

         if (pPtpPort->clock->config.profileStrict == ZL303XX_TRUE)
         {
            status = ZL303XX_PARAMETER_INVALID;
         }
      }
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpPort->clock);
   }

   /* Update the contract duration limit; unlock mutex. */
   if (status == ZL303XX_OK)
   {
      pPtpPort->contractLimits[uniNegMsgtype].maxDurationSec = durationMax;
      status = zl303xx_PtpClockMutexUnlock(pPtpPort->clock, status);
   }

   return status;
}

/* zl303xx_PtpPortContractIntervalMinGet */
/**
   Returns the minimum packet interval of unicast negotiation contracts that can
   be granted by this port for a specific messageType.

  Parameters:
   [in]  portHandle     Handle to an existing port.
   [in]  msgType        The messageType to query.
   [out] intervalMin    Minimum packet interval of a contract that can be
                              granted for messageType on this port.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    intervalMin is NULL.
     ZL303XX_PARAMETER_INVALID  portHandle or msgType is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpPortContractIntervalMinGet(
      zl303xx_PtpPortHandleT portHandle,
      zl303xx_MessageTypeE msgType,
      Sint8T *intervalMin)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPtpPort;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(intervalMin);
   }

   /* Get a pointer to the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPtpPort);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MessageTypeToIndex(msgType, &uniNegMsgType);
   }

   if (status == ZL303XX_OK)
   {
      *intervalMin = pPtpPort->contractLimits[uniNegMsgType].minIntvl;
   }

   return status;
}

/* zl303xx_PtpPortContractIntervalMinSet */
/**
   Sets the minimum packet interval (i.e. max PPS) of unicast negotiation
   contracts that can be granted by this port for a specific messageType.

   In the event that this parameter is raised at run-time, to a value more than
   the current interval of an existing contract, then that existing contract
   will be permitted to expire normally. (Since the IEEE-1588 Std suggests that
   once a contract is GRANTED it should be honored for the duration).

  Parameters:
   [in]  portHandle     Handle to an existing port.
   [in]  msgType        The messageType to limit.
   [in]  intervalMin    Minimum packet interval of a contract that can be
                              granted for  messageType on this port.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_PARAMETER_INVALID  portHandle or msgType is invalid; intervalMin
                                    can be any value since the purpose may be to
                                    disable GRANTING of msgType (set MIN = 128).

*******************************************************************************/
zlStatusE zl303xx_PtpPortContractIntervalMinSet(
      zl303xx_PtpPortHandleT portHandle,
      zl303xx_MessageTypeE msgType,
      Sint8T intervalMin)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPtpPort;
   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgtype;

   /* Get a pointer to the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPtpPort);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpV2MessageTypeToIndex(msgType, &uniNegMsgtype);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpPort->clock);
   }

   /* Update the contract duration limit; unlock mutex. */
   if (status == ZL303XX_OK)
   {
      pPtpPort->contractLimits[uniNegMsgtype].minIntvl = intervalMin;
      status = zl303xx_PtpClockMutexUnlock(pPtpPort->clock, status);
   }

   return status;
}

/* zl303xx_PtpPortMasterOnlySet */
/**
   Sets a port's masterOnly value; TRUE prohibits the Port from entering the
   SLAVE or PASSIVE modes.

  Parameters:
   [in]  portHandle     Handle to an existing port.
   [in]  bMasterOnly    ZL303XX_TRUE to prohibit the Port from becoming the
                              SLAVE (or PASSIVE).
                           ZL303XX_FALSE allows normal BMC operation on the Port.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle or bMasterOnly is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpPortMasterOnlySet(
      zl303xx_PtpPortHandleT portHandle,
      zl303xx_BooleanE bMasterOnly)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_PortDataS *pPtpPort = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(bMasterOnly);
   }

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPtpPort);
   }

   /* Lock the clock mutex since we go into the binding routines. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pPtpPort->clock);
      status = mStatus;
   }

   /* Perform any PROFILE specific checks on the port setting. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpProfilePortMasterOnlyCheck(pPtpPort, &bMasterOnly);
   }

   /* Update the masterOnly setting. */
   /* Any time a Port configuration is changed, the Clock State-Decision
    * algorithm should be run to re-evaluate its state (and that of any
    * associated ports & streams).   */
   if (status == ZL303XX_OK)
   {
      pPtpPort->config.masterOnly = bMasterOnly;

      zl303xx_PtpClockBmcaUpdateInitiate(pPtpPort->clock);
   }

   /* Unlock the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPtpPort->clock, status);
   }

   return status;
}  /* END zl303xx_PtpPortMasterOnlySet */

/* zl303xx_PtpPortDenyServiceRequestsSet */
/**
   Sets a port::config::unicast::denyServiceRequests value. TRUE prohibits the
   Port from Granting any Unicast contracts.

   Previously, this was the port::config::unicast::grantDefault value (which
   is now deprecated).

  Parameters:
   [in]  portHandle           Handle to an existing port.
   [in]  bDenyServiceRequest  ZL303XX_TRUE to prohibit the Port from Granting
                                    Unicast contracts.
                                 ZL303XX_FALSE to allow Granting of contracts.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle or bDenyServiceRequest is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpPortDenyServiceRequestsSet(
      zl303xx_PtpPortHandleT portHandle,
      zl303xx_BooleanE bDenyServiceRequest)
{
   zlStatusE status = ZL303XX_OK;
   zlStatusE mStatus = ZL303XX_ERROR;
   zl303xx_PortDataS *pPort;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(bDenyServiceRequest);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   /* Since we go down into the PROFILE binding, it is best Take the MUTEX */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pPort->clock);
      status = mStatus;
   }

   /* Perform any PROFILE specific checks on the GRANT setting. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpProfileUnicastGrantDenyCheck(pPort->clock, &bDenyServiceRequest);
   }

   /* Set the Port configuration. */
   if (status == ZL303XX_OK)
   {
      pPort->config.unicastNegotiation.denyServiceRequests = bDenyServiceRequest;

      /* Set the deprecated member as well */
      pPort->config.unicastNegotiation.grantDefault = !(bDenyServiceRequest);
   }

   /* Release the Mutex (if we ever had it). */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }

   /* This Port configuration change does not affect BMCA so do not initiate
    * a BMCA UPDATE event.   */

   return status;
}  /* END zl303xx_PtpPortDenyServiceRequestsSet */


/* zl303xx_PtpPortRateRatioEnabledGet */
/**
   Returns rateRatioEnabled attribute associated with a port.

  Parameters:
   [in]   portHandle   Handle to an existing port.
   [out]  pRateRatioEnabled    Returned rateRatioEnabled.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pRateRatioEnabled is NULL.
     ZL303XX_PARAMETER_INVALID  portHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpPortRateRatioEnabledGet(zl303xx_PtpPortHandleT portHandle,
                                    zl303xx_BooleanE *pRateRatioEnabled)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *port = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pRateRatioEnabled);
   }

   /* Get a pointer to the port data structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &port);
   }

   /* Return the rateRatioEnabled. */
   if (status == ZL303XX_OK)
   {
      *pRateRatioEnabled = port->config.rateRatioEnabled;
   }

   return status;
}


/* zl303xx_PtpPortRateRatioEnabledSet */
/**
   Sets the rateRatioEnabled attribute associated with a port.

  Parameters:
   [in]   portHandle  Handle to an existing port.
   [in]   rateRatioEnabled    rateRatioEnabled value to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle/rateRatioEnabled is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpPortRateRatioEnabledSet(zl303xx_PtpPortHandleT portHandle,
                                    zl303xx_BooleanE rateRatioEnabled)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort;

   /* Check that the value of rateRatioEnabled is within range. */
   if(status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(rateRatioEnabled);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPort->clock);
   }

   if (status == ZL303XX_OK)
   {
      pPort->config.rateRatioEnabled = rateRatioEnabled;
      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }

   return status;
}


/* zl303xx_PtpPortNumRateRatioPktsGet */
/**
   Returns numRateRatioPkts attribute associated with a port.

  Parameters:
   [in]   portHandle   Handle to an existing port.
   [out]  pNumRateRatioPkts    Returned numRateRatioPkts.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pNumRateRatioPkts is NULL.
     ZL303XX_PARAMETER_INVALID  portHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpPortNumRateRatioPktsGet(zl303xx_PtpPortHandleT portHandle,
                                    zl303xxnumRateRatioPktsE *pNumRateRatioPkts)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *port = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pNumRateRatioPkts);
   }

   /* Get a pointer to the port data structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &port);
   }

   /* Return the numRateRatioPkts. */
   if (status == ZL303XX_OK)
   {
      *pNumRateRatioPkts = port->config.numRateRatioPkts;
   }

   return status;
}


/* zl303xx_PtpPortNumRateRatioPktsSet */
/**
   Sets the numRateRatioPkts attribute associated with a port.

  Parameters:
   [in]   portHandle  Handle to an existing port.
   [in]   numRateRatioPkts    numRateRatioPkts value to set.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle/numRateRatioPkts is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpPortNumRateRatioPktsSet(zl303xx_PtpPortHandleT portHandle,
                                    zl303xxnumRateRatioPktsE numRateRatioPkts)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort;

   /* Check that the value of numRateRatioPkts is within range. */
   if(status == ZL303XX_OK)
   {
      
      status = zl303xx_PtpNumRateRatioPktsValueCheck(numRateRatioPkts); 
       
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPort->clock);
   }

   if (status == ZL303XX_OK)
   {
      pPort->config.numRateRatioPkts = numRateRatioPkts;
      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }

   return status;
}



/* zl303xx_PtpNumRateRatioPktsValueCheck */
/**
   Check whether the value of numRateRatioPkts is valid.

  Parameters:
   [in]   numRateRatioPkts    value to check.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     numRateRatioPkts is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpNumRateRatioPktsValueCheck(zl303xxnumRateRatioPktsE numRateRatioPkts)
{
   zlStatusE status = ZL303XX_PARAMETER_INVALID;
   Uint8T idx = 0;

   while (status != ZL303XX_OK && idx < ZL303XX_PTP_VALID_VALUES_NUM_RATE_RATIO_PKTS_MAX)
   {
      if(numRateRatioPkts == zl303xx_PtpValidNumRateRatioPktsValues[idx])
      {
         status = ZL303XX_OK;
         
      }
      
      idx += 1;
   }

   
   if (status == ZL303XX_PARAMETER_INVALID)
   {
      ZL303XX_TRACE_ERROR("   (ERROR) Invalid numRateRatioPkts=%u. Must be one of 1, 2, 4, 8 or 16.",
                          numRateRatioPkts, 0,0,0,0,0);
   } 

   return status;
}


/* zl303xx_PtpPortForceFaultySet */
/**
   Forces a port to go into faulty state

  Parameters:
   [in]   portHandle     Handle to an existing port.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpPortForceFaultySet(zl303xx_PtpPortHandleT portHandle)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort;

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPort->clock);
   }

   if (status == ZL303XX_OK)
   {
      zl303xx_PtpPortLogFaultyState(ZL303XX_PTP_TX_USER_ENFORCED_ERROR,
                                 ZL303XX_PTP_PORT_FAULT_USER_ENFORCED, pPort);
      zl303xx_PtpV2ToPortState(pPort, PTP_PORT_INT_STATE_FAULTY);
      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);

   }

   return status;
}

/* zl303xx_PtpPortForceFaultyClear */
/**
   Brings a port out of faulty state

  Parameters:
   [in]   portHandle     Handle to an existing port.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpPortForceFaultyClear(zl303xx_PtpPortHandleT portHandle)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort;

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPort->clock);
   }

   if (status == ZL303XX_OK)
   {
      /* Make sure the port is still in faulty */
      if(pPort->portSuperState == PTP_PORT_INT_STATE_FAULTY)
      {
         zl303xx_PtpV2ToPortState(pPort, PTP_PORT_INT_STATE_INIT);
      }
      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }
   return status;
}


/* zl303xx_PtpPortGetLastFaultType */
/**
   Returns last fault type incurred on a port

  Parameters:
   [in]   portHandle        Handle to an existing port.
   [out]  pLastFaultType    Returned FaultType.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pLastFaultType is NULL.
     ZL303XX_PARAMETER_INVALID  portHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpPortGetLastFaultType(zl303xx_PtpPortHandleT portHandle,
                                          zl303xx_PtpPortFaultTypeE *pLastFaultType)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *port = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pLastFaultType);
   }

   /* Get a pointer to the port data structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &port);
   }

   /* Return the last fault type */
   if (status == ZL303XX_OK)
   {
      *pLastFaultType = port->lastFaultType;
   }

   return status;
}

/* zl303xx_PtpPortFaultPassThroughEnSet */
/**
   Sets a port's Fault Pass through Enable setting.

  Parameters:
   [in]   portHandle     Handle to an existing port.
   [in]   passThroughEn   ZL303XX_TRUE to enable the port going from faulty to init.
                             ZL303XX_FALSE to stay in faulty state.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle or passThroughEn is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpPortFaultPassThroughEnSet(zl303xx_PtpPortHandleT portHandle,
                                             zl303xx_BooleanE passThroughEn)
{

   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(passThroughEn);
   }

   /* Get a pointer to the port data. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPort->clock);
   }

   /* Update the passThroughEn port setting. */
   if (status == ZL303XX_OK)
   {
      pPort->config.faultPassThroughEn = passThroughEn;
      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }

   return status;
}

/* zl303xx_PtpPortFaultCounterGet */
/**
   Gets the fault counter for a certain port

  Parameters:
   [in]   portHandle     Handle to an existing port.
   [out]  pFaultCounter   Returned Fault counter

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     portHandle or passThroughEn is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpPortFaultCounterGet(zl303xx_PtpPortHandleT portHandle,
                                             Uint32T *pFaultCounter)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *port = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pFaultCounter);
   }

   /* Get a pointer to the port data structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &port);
   }

   /* Return the last fault type */
   if (status == ZL303XX_OK)
   {
      *pFaultCounter = port->faultyStateCount;
   }

   return status;
}

/* zl303xx_PtpPortGetLastTxError */
/**
   Returns last transmit error incurred on a port

  Parameters:
   [in]   portHandle      Handle to an existing port.
   [out]  pLastTxError    Returned Trasmit Error.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pLastTxError is NULL.
     ZL303XX_PARAMETER_INVALID  portHandle is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpPortGetLastTxError(zl303xx_PtpPortHandleT portHandle,
                                       zl303xx_PtpTxStatusE *pLastTxError)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *port = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pLastTxError);
   }

   /* Get a pointer to the port data structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &port);
   }

   /* Return the last fault type */
   if (status == ZL303XX_OK)
   {
      *pLastTxError = port->lastTxError;
   }

   return status;
}
/*****************   END   ****************************************************/
