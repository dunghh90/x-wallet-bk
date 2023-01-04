

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     The NODE interface is a manager / data control point for creating,
*     deleting, accessing, etc. other PTP entities. The node itself must be
*     initialized before other entities can be provisioned.
*
*******************************************************************************/


/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Os.h"
#include "zl303xx_Trace.h"
#include "zl303xx_ErrTrace.h"

#include "zl303xx_Error.h"
#include "zl303xx_List.h"
#include "zl303xx_Ptsf.h"

#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_Ptpd.h"

/*****************   DEFINES     **********************************************/
#define PTP_NODE_DEFAULT_INIT_STATE    ZL303XX_PTP_NODE_STATE_ENABLED

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/
static zlStatusE zl303xx_PtpNodeCreate(void);
static zlStatusE zl303xx_PtpNodeDelete(void);
static zlStatusE zl303xx_PtpNodeStringCopy(const char *src, Uint16T srcLen, char *dest);

/*****************   STATIC GLOBAL VARIABLES   ********************************/
static zl303xx_PtpNodeS PtpNode = {OS_MUTEX_INVALID,
                                 ZL303XX_PTP_NODE_STATE_UNINIT,
                                 ZL303XX_LIST_HEAD_INIT(PtpNode.clockList),
                                 0,
                                 {0x00,0xA0,0x87}, /* OUI */
                                 "",
                                 "",
                                 ""};

/* Pointer to the most recently created clock. */
zl303xx_PtpClockS *Zl303xx_PtpLastClock = NULL;

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/* The main clock structure, one per device */
zl303xx_PtpClockS *Zl303xx_PtpClockArray[ZL303XX_PTP_NUM_CLOCKS_MAX] = {NULL};

/* An array of structures; one per port, but in this case only one port is supported so it
   is a simple variable */
zl303xx_PortDataS *Zl303xx_PtpPortArray[ZL303XX_PTP_NUM_PORTS_MAX] = {NULL};

/* An array of structures; one per clock stream */
zl303xx_ClockStreamS *Zl303xx_PtpStreamArray[ZL303XX_PTP_NUM_STREAMS_MAX] = {NULL};

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* zl303xx_PtpInit */
/**
   Initializes any global data used by PTP.

  Parameters:
   [in]  None

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_PtpInit(void)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtsfConfigS ptsfCfg;

   /* Initialize node description strings. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpNodeStringsInit();
   }

   /* Initialize the PTSF module. */
   if (status == ZL303XX_OK)
   {
      zl303xx_PtsfConfigStructInit(&ptsfCfg);
   }

   if (status == ZL303XX_OK)
   {
      ptsfCfg.maxStreams = ZL303XX_PTP_NUM_STREAMS_MAX;
      status = zl303xx_PtsfInit(&ptsfCfg);
   }

   /* Create the PTP timer task. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpTimerTaskStart();
   }

   return ZL303XX_OK;
}

/* zl303xx_PtpShutdown */
/**
   Shuts down the PTP capabilities of the CPU. This will delete any existing
   clocks and their associated ports and streams. The PTP functionality is
   restarted automatically when a new clock is created (or re-created).

  Parameters:
   [in]  None

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Mutex take failed.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Mutex release failed.

*******************************************************************************/
zlStatusE zl303xx_PtpShutdown(void)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T clockHandle;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpShutdown:", 0,0,0,0,0,0);

   /* Stop the timer task. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpTimerTaskStop();
   }

   /* The best way to shut down PTP is to delete each of the existing Clocks.
    * When the last Clock is removed, the Node is shut down automatically.
    * Starting this from the Node level will cause a 'loop of contention' since
    * deleting the last Clock tries to delete the Node and the Node tries to
    * delete the attached clocks, and so on!  */
   if (status == ZL303XX_OK)
   {
      if (PtpNode.state == ZL303XX_PTP_NODE_STATE_UNINIT)
      {
         ZL303XX_TRACE_ALWAYS("zl303xx_PtpShutdown: Node already removed or not initialized.", 0,0,0,0,0,0);
      }
      else
      {
          if (zl303xx_ListValid(&PtpNode.clockList) && !zl303xx_ListEmpty(&PtpNode.clockList))
          {
             /* LOOP through each CLOCK associated with the Node and delete it. */
             /* We could follow the linked list of clocks attached to the node but
              * the inefficiency of looping the entire clock array is a safe and
              * acceptable approach at this point. */
             for (clockHandle = 0; clockHandle < ZL303XX_PTP_NUM_CLOCKS_MAX; clockHandle++)
             {
                /* Call the API routine so that there is just one code path that
                 * ever deletes the clock. The API will handle the mutex and any
                 * PTP Events that need to be generated.   */
                status |= zl303xx_PtpClockDelete(clockHandle, NULL);
             }
          }
      }
   }

   /* Close the PTSF module. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtsfClose();
   }

   return status;
}

/* zl303xx_PtpNodeEnable */
/**
   Enable/disable the PTP Node for this CPU. Updates the Node status.

  Parameters:
   [in]  nodeEnable  PTP Node enable parameters.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       nodeShutdown is NULL.
     ZL303XX_PARAMETER_INVALID     A parameter in nodeShutdown is out of range.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Mutex take failed.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Mutex release failed.

*******************************************************************************/
zlStatusE zl303xx_PtpNodeEnable(zl303xx_PtpNodeEnableS *nodeEnable)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpNodeEnable:", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(nodeEnable);
   }
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(nodeEnable->enable);
   }

   /* Check if the node has been initialized yet. */
   if (status == ZL303XX_OK)
   {
      if (PtpNode.state == ZL303XX_PTP_NODE_STATE_UNINIT)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) PTP node has not been initialized.",
               0,0,0,0,0,0);

         status = ZL303XX_NOT_RUNNING;
      }
   }

   /* Take the node mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpNodeMutexLock();
      status = mStatus;
   }

   /* Update the node state. */
   if (status == ZL303XX_OK)
   {
      if (nodeEnable->enable == ZL303XX_FALSE)
      {
         PtpNode.state = ZL303XX_PTP_NODE_STATE_DISABLED;
      }
      else
      {
         PtpNode.state = ZL303XX_PTP_NODE_STATE_ENABLED;
      }

   }

   /* Release the node mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpNodeMutexUnlock(status);
   }

   return status;
}

/* zl303xx_PtpNodeManufacturerIdentitySet */
/**
   Sets the manufacturerIdentity for the PTP node. The manufacturerIdentity
   shall be an OUI owned by the manufacturer of the node.

  Parameters:
   [in]   pManufacturerId   3 byte OUI.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pManufacturerId is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpNodeManufacturerIdentitySet(Uint8T *pManufacturerId)
{
   zlStatusE status = ZL303XX_OK;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pManufacturerId);
   }

   /* Copy the ID into the node structure. */
   if (status == ZL303XX_OK)
   {
      OS_MEMCPY(PtpNode.manufacturerIdentity, pManufacturerId,
             sizeof(PtpNode.manufacturerIdentity));
   }

   return status;
}

/* zl303xx_PtpNodeManufacturerIdentityGet */
/**
   Returns the manufacturerIdentity for the PTP node.

  Parameters:
   [out]  pManufacturerId   Output buffer. Must be at least 3 bytes.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pManufacturerId is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpNodeManufacturerIdentityGet(Uint8T *pManufacturerId)
{
   zlStatusE status = ZL303XX_OK;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pManufacturerId);
   }

   /* Copy the node's ID into the supplied buffer. */
   if (status == ZL303XX_OK)
   {
      OS_MEMCPY(pManufacturerId, PtpNode.manufacturerIdentity, ZL303XX_PTP_NODE_OUI_LEN);
   }

   return status;
}

/* zl303xx_PtpNodeProductDescriptionStructInit */
/**
   Initializes a zl303xx_PtpNodeProductDescriptionS structure with default values.

  Parameters:
   [out]   pDesc   Pointer to a product description structure.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pDesc is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpNodeProductDescriptionStructInit(zl303xx_PtpNodeProductDescriptionS *pDesc)
{
   zlStatusE status = ZL303XX_OK;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pDesc);
   }

   /* Set default values. */
   if (status == ZL303XX_OK)
   {
      OS_MEMSET(pDesc, 0, sizeof(*pDesc));

      pDesc->manufacturerName   = ZL303XX_PTP_DEFAULT_NODE_MANUFAC_NAME;
      pDesc->modelNumber        = ZL303XX_PTP_DEFAULT_NODE_MODEL_NUM;
      pDesc->instanceIdendifier = ZL303XX_PTP_DEFAULT_NODE_INSTANCE_ID;
   }

   return status;
}

/* zl303xx_PtpNodeProductDescriptionSet */
/**
   Sets the productDescription for the PTP node. It is formatted as:
      "manufacturerName;modelNumber;instanceIdendifier".

  Parameters:
   [in]   pDesc   Pointer to a product description structure.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pDesc is NULL.
     ZL303XX_PARAMETER_INVALID  The total length of the fomatted string
                                    exceeds 64.

*******************************************************************************/
zlStatusE zl303xx_PtpNodeProductDescriptionSet(zl303xx_PtpNodeProductDescriptionS *pDesc)
{
   zlStatusE status = ZL303XX_OK;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pDesc);
   }
   if (status == ZL303XX_OK)
   {
      if ((OS_STRLEN(pDesc->manufacturerName)   +
           OS_STRLEN(pDesc->modelNumber)        +
           OS_STRLEN(pDesc->instanceIdendifier) +
           3)  /* +2 for semicolons, +1 for NULL terminator */
             > sizeof(PtpNode.productDescription))
      {
         ZL303XX_TRACE_ALWAYS("   (ERROR) Length of concatenated strings > %lu",
                              sizeof(PtpNode.productDescription), 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Concatenate the provided strings. */
   if (status == ZL303XX_OK)
   {
      OS_SNPRINTF(PtpNode.productDescription, sizeof(PtpNode.productDescription), "%s;%s;%s",
              pDesc->manufacturerName, pDesc->modelNumber, pDesc->instanceIdendifier);
   }

   return status;
}

/* zl303xx_PtpNodeProductDescriptionGet */
/**
   Returns the productDescription for the PTP node. It is formatted as:
      "manufacturerName;modelNumber;instanceIdendifier".

  Parameters:
   [out]  productDescription   Pointer to a character array. The maximum size
                                    of the returned string is 65 characters.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    productDescription is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpNodeProductDescriptionGet(char *productDescription)
{
   return zl303xx_PtpNodeStringCopy(PtpNode.productDescription,
                                    sizeof(PtpNode.productDescription),
                                    productDescription);
}

/* zl303xx_PtpNodeRevisionDataStructInit */
/**
   Initializes a zl303xx_PtpNodeRevisionDataS structure with default values.

  Parameters:
   [out]   pRev   Pointer to a revision data structure.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pRev is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpNodeRevisionDataStructInit(zl303xx_PtpNodeRevisionDataS *pRev)
{
   zlStatusE status = ZL303XX_OK;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pRev);
   }

   /* Set default values. */
   if (status == ZL303XX_OK)
   {
      OS_MEMSET(pRev, 0, sizeof(*pRev));

      pRev->hardwareRevision = ZL303XX_PTP_DEFAULT_NODE_HW_REV;
      pRev->firmwareRevision = ZL303XX_PTP_DEFAULT_NODE_FW_REV;
      pRev->softwareRevision = ZL303XX_PTP_DEFAULT_NODE_SW_REV;
   }

   return status;
}

/* zl303xx_PtpNodeRevisionDataSet */
/**
   Sets the revisionData for the PTP node. It is formatted as:
      "hardwareRevision;firmwareRevision;softwareRevision".

  Parameters:
   [in]   pRev   Pointer to a revision data structure.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pRev is NULL.
     ZL303XX_PARAMETER_INVALID  The total length of the fomatted string
                                    exceeds 32.

*******************************************************************************/
zlStatusE zl303xx_PtpNodeRevisionDataSet(zl303xx_PtpNodeRevisionDataS *pRev)
{
   zlStatusE status = ZL303XX_OK;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pRev);
   }
   if (status == ZL303XX_OK)
   {
      if((OS_STRLEN(pRev->hardwareRevision) +
          OS_STRLEN(pRev->firmwareRevision) +
          OS_STRLEN(pRev->softwareRevision) +
          3)  /* +2 for semicolons, +1 for NULL terminator */
            > sizeof(PtpNode.revisionData))
      {
         ZL303XX_TRACE_ALWAYS("   (ERROR) Length of concatenated strings > %lu",
                              sizeof(PtpNode.revisionData), 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Concatenate the provided strings. */
   if (status == ZL303XX_OK)
   {
      OS_SNPRINTF(PtpNode.revisionData, sizeof(PtpNode.revisionData), "%s;%s;%s",
                pRev->hardwareRevision, pRev->firmwareRevision, pRev->softwareRevision);
   }

   return status;
}

/* zl303xx_PtpNodeRevisionDataGet */
/**
   Returns the revisionData for the PTP node. It is formatted as:
      "hardwareRevision;firmwareRevision;softwareRevision".

  Parameters:
   [out]  revisionData   Pointer to a character array. The maximum size of
                              the returned string is 33 characters.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    revisionData is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpNodeRevisionDataGet(char *revisionData)
{
   return zl303xx_PtpNodeStringCopy(PtpNode.revisionData,
                                    sizeof(PtpNode.revisionData),
                                    revisionData);
}

/* zl303xx_PtpNodeUserDescriptionStructInit */
/**
   Initializes a zl303xx_PtpNodeUserDescriptionS structure with default values.

  Parameters:
   [out]   pUser   Pointer to a user description structure.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pUser is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpNodeUserDescriptionStructInit(zl303xx_PtpNodeUserDescriptionS *pUser)
{
   zlStatusE status = ZL303XX_OK;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pUser);
   }

   /* Set default values. */
   if (status == ZL303XX_OK)
   {
      OS_MEMSET(pUser, 0, sizeof(*pUser));

      pUser->deviceName       = ZL303XX_PTP_DEFAULT_NODE_DEV_NAME;
      pUser->physicalLocation = ZL303XX_PTP_DEFAULT_NODE_PHYS_LOC;
   }

   return status;
}

/* zl303xx_PtpNodeUserDescriptionSet */
/**
   Sets the userDescription for the PTP node. It is formatted as:
      "deviceName;physicalLocation".

  Parameters:
   [in]   pUser   Pointer to a revision data structure.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    pUser is NULL.
     ZL303XX_PARAMETER_INVALID  The total length of the fomatted string
                                    exceeds 128.

*******************************************************************************/
zlStatusE zl303xx_PtpNodeUserDescriptionSet(zl303xx_PtpNodeUserDescriptionS *pUser)
{
   zlStatusE status = ZL303XX_OK;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pUser);
   }
   if (status == ZL303XX_OK)
   {
      if ((OS_STRLEN(pUser->deviceName)       +
           OS_STRLEN(pUser->physicalLocation) +
           2)
             > sizeof(PtpNode.userDescription))
      {
         ZL303XX_TRACE_ALWAYS("   (ERROR) Length of concatenated strings > %lu",
                              sizeof(PtpNode.userDescription), 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Concatenate the provided strings. */
   if (status == ZL303XX_OK)
   {
      if (OS_STRLEN(pUser->physicalLocation) > 0)
      {
         OS_SNPRINTF(PtpNode.userDescription, sizeof(PtpNode.userDescription), "%s;%s",
                 pUser->deviceName, pUser->physicalLocation);
      }
      else
      {
         OS_SNPRINTF(PtpNode.userDescription, sizeof(PtpNode.userDescription), "%s", pUser->deviceName);
      }
   }

   return status;
}

/* zl303xx_PtpNodeUserDescriptionGet */
/**
   Returns the userDescription for the PTP node. It is formatted as:
      "deviceName;physicalLocation".

  Parameters:
   [out]  userDescription   Pointer to a character array. The maximum size of
                                 the returned string is 129 characters.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    userDescription is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpNodeUserDescriptionGet(char *userDescription)
{
   return zl303xx_PtpNodeStringCopy(PtpNode.userDescription,
                                 sizeof(PtpNode.userDescription),
                                 userDescription);
}

/*****************   INTERNAL PTP FUNCTION DEFINITIONS   **********************/

/* zl303xx_PtpNodeClockListGet */
/** 

   Returns the list of clocks attached to this node.

  Parameters:
      None.

*******************************************************************************/
zl303xx_ListS *zl303xx_PtpNodeClockListGet(void)
{
   return &PtpNode.clockList;
}

/* zl303xx_PtpNodeMutexLock */
/** 

   Locks the PTP node mutex.

  Parameters:
      None.

*******************************************************************************/
zlStatusE zl303xx_PtpNodeMutexLock(void)
{
   if (OS_MUTEX_TAKE(PtpNode.mutexId) != OS_OK)
   {
      ZL303XX_TRACE_ALWAYS("   (ERROR) Node mutex take failed.",
            0,0,0,0,0,0);

      return ZL303XX_RTOS_MUTEX_TAKE_FAIL;
   }

   return ZL303XX_OK;
}

/* zl303xx_PtpNodeMutexUnlock */
/** 

   Releases the PTP node mutex. Does not overwrite an existing error code.

  Parameters:
   [in]   status   Current status.

*******************************************************************************/
zlStatusE zl303xx_PtpNodeMutexUnlock(zlStatusE status)
{
   zlStatusE newStatus = ZL303XX_OK;

   if (OS_MUTEX_GIVE(PtpNode.mutexId) != OS_OK)
   {
      ZL303XX_TRACE_ALWAYS("   (ERROR) Node mutex release failed.",
            0,0,0,0,0,0);

      newStatus = ZL303XX_RTOS_MUTEX_GIVE_FAIL;
   }

   /* Do not overwrite an existing error code. */
   return (status != ZL303XX_OK) ? status : newStatus;
}

/* zl303xx_PtpNodeAttachClock */
/** 

   Associates a clock data structure with the PTP node. If this is the first
   clock being attached, the node data structures will be automatically
   initialized, and the PTP timer task will be started.

  Parameters:
   [in,out]  pPtpClock  Pointer to clock data structure to attach.

  Notes:   Assumes all input parameters are valid.

*******************************************************************************/
zlStatusE zl303xx_PtpNodeAttachClock(zl303xx_PtpClockS *pPtpClock)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   Uint32T n = 0;

   /* Create the node, if necessary. This needs to be done for the first clock
    * that gets attached. */
   if (status == ZL303XX_OK)
   {
      if (PtpNode.state == ZL303XX_PTP_NODE_STATE_UNINIT)
      {
         status = zl303xx_PtpNodeCreate();
      }
   }

   /* Lock the node mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpNodeMutexLock();
      status = mStatus;
   }

   /* requestedHandle must be a valid array index and not already in use, or -1 */
   if (status == ZL303XX_OK)
   {
      /* These checks are not done in zl303xx_PtpClockCreateStructCheck() because there
       * is a (very unlikely) race condition where two clocks could be created with
       * the same requested handle. So, only do checks when holding the node mutex. */
      if (pPtpClock->config.requestedHandle != (zl303xx_PtpClockHandleT)ZL303XX_PTP_INVALID)
      {
         n = pPtpClock->config.requestedHandle;

         if (ZL303XX_CHECK_CLOCK_HANDLE(n) != ZL303XX_OK)
         {
            ZL303XX_TRACE_ERROR("   Node (ERROR) requestedHandle=%u is out of range.",
                                n, 0,0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
         }
         else if (Zl303xx_PtpClockArray[n] != NULL)
         {
            ZL303XX_TRACE_ERROR("   Node (ERROR) requestedHandle=%u is already in use.",
                                n, 0,0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
         }
      }
      /* requestedHandle is -1, so find an empty clock array entry */
      else
      {
         for (n = 0; n < ZL303XX_PTP_NUM_CLOCKS_MAX; n++)
         {
            if (Zl303xx_PtpClockArray[n] == NULL)
               break;
         }

         if (n == ZL303XX_PTP_NUM_CLOCKS_MAX)
         {
            ZL303XX_TRACE_ERROR("   (ERROR) Maximum number of clocks attached (%lu).",
                  ZL303XX_PTP_NUM_CLOCKS_MAX, 0,0,0,0,0);
            status = ZL303XX_TABLE_FULL;
         }
      }
   }

   if (status == ZL303XX_OK)
   {
      Zl303xx_PtpClockArray[n] = pPtpClock;
      pPtpClock->clockHandle = n;

      /* Add the clock to the node's clockList. */
      zl303xx_ListAdd(&PtpNode.clockList, &pPtpClock->listEntry);
      PtpNode.clockCount++;

      /* Keep track of the most recently created clock. */
      Zl303xx_PtpLastClock = pPtpClock;
   }

   /* Unlock the node mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpNodeMutexUnlock(status);
   }

   return status;
}

/* zl303xx_PtpNodeDetachClock */
/** 

   Removes a clock data structure from the PTP node. If this is the last clock
   being detached, the node will be deleted, and the PTP timer task will stop.

  Parameters:
   [in]   pPtpClock   Pointer to clock data structure to detach.

*******************************************************************************/
zlStatusE zl303xx_PtpNodeDetachClock(zl303xx_PtpClockS *pPtpClock)
{
   /* Lock the node mutex. */
   zlStatusE status = zl303xx_PtpNodeMutexLock();

   if (status == ZL303XX_OK)
   {
      /* Remove the clock from the node's clockList. */
      zl303xx_ListRemove(&pPtpClock->listEntry);
      PtpNode.clockCount--;

      /* Delete the clock array entry. */
      Zl303xx_PtpClockArray[pPtpClock->clockHandle] = NULL;
      /* Unlock the node mutex. */
      status = zl303xx_PtpNodeMutexUnlock(status);
   }

   /* If there are no longer any clocks attached to the node, shut it down. */
   if (status == ZL303XX_OK)
   {
      if (PtpNode.clockCount == 0)
      {
         status = zl303xx_PtpNodeDelete();
      }
   }

   return status;
}

/* zl303xx_PtpNodeLastClockGet */
/** 

   Returns a pointer to the most recently created clock.

  Parameters:
   [out]   pPtpClock   Pointer to the clock structure.

*******************************************************************************/
zlStatusE zl303xx_PtpNodeLastClockGet(zl303xx_PtpClockS **ppPtpClock)
{
   zlStatusE mStatus = zl303xx_PtpNodeMutexLock(), status = mStatus;

   if (status == ZL303XX_OK)
   {
      if (Zl303xx_PtpLastClock == NULL)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) No clock has been created recently.",
               0,0,0,0,0,0);

         status = ZL303XX_INIT_ERROR;
      }
   }

   if (status == ZL303XX_OK)
   {
      *ppPtpClock = Zl303xx_PtpLastClock;
      Zl303xx_PtpLastClock = NULL;
   }

   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpNodeMutexUnlock(status);
   }

   return status;
}

/* zl303xx_PtpNodeLastClockExists */
/**
   Determines if the last created clock has been queried by the PTP clock
   task yet.

  Parameters:
      None.

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtpNodeLastClockExists(void)
{
   return (Zl303xx_PtpLastClock != NULL) ? ZL303XX_TRUE : ZL303XX_FALSE;
}

/* zl303xx_PtpNodeLastClockExists */
/** 

   Determines if a clock exists with the provided clockIdentity.

  Parameters:
   [in]   clockId  clockIdentity to search for.

  Return Value:
     NULL                 No matching clock.
     (zl303xx_PtpClockS *)  Matching clock found.

*******************************************************************************/
zl303xx_PtpClockS *zl303xx_PtpNodeClockIdentityExists(zl303xx_ClockIdentity clockId)
{
   if (PtpNode.state != ZL303XX_PTP_NODE_STATE_UNINIT)
   {
      zl303xx_PtpClockS *pClock = NULL;

      if (zl303xx_ListValid(&PtpNode.clockList) && !zl303xx_ListEmpty(&PtpNode.clockList))
      {
         ZL303XX_PTP_FOR_EACH_CLOCK(&PtpNode.clockList, pClock)
         {
            if (zl303xx_PtpV2EqualClockIdentities(clockId, pClock->config.defaultDS.clockIdentity))
            {
               return pClock;
            }
         }
      }
   }

   return NULL;
}

/* zl303xx_PtpNodeStringsInit */
/** 

   Initializes the PTP node strings. For backward compatibility, if
   PtpManufacturerID, PtpRevisionData, or PtpUserDescription are not zero-length,
   those strings will be used instead of the defaults in zl303xx_PtpSetup.h

  Parameters:
      None.

*******************************************************************************/
zlStatusE zl303xx_PtpNodeStringsInit(void)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpNodeProductDescriptionS desc;
   zl303xx_PtpNodeRevisionDataS rev;
   zl303xx_PtpNodeUserDescriptionS user;

   /* NUL-fill the PTP node strings. */
   OS_MEMSET(PtpNode.productDescription, '\0', sizeof(PtpNode.productDescription));
   OS_MEMSET(PtpNode.revisionData,       '\0', sizeof(PtpNode.revisionData));
   OS_MEMSET(PtpNode.userDescription,    '\0', sizeof(PtpNode.userDescription));

   /* Initialize structures. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpNodeProductDescriptionStructInit(&desc) |
               zl303xx_PtpNodeRevisionDataStructInit(&rev)        |
               zl303xx_PtpNodeUserDescriptionStructInit(&user);
   }

   /* Backward compatibility: If PtpManufacturerID is set, copy that into the
    * productDescription buffer. Otherwise, use the default values from zl303xx_PtpSetup.h */
   if (status == ZL303XX_OK)
   {
      if (OS_STRLEN(PtpManufacturerID) > 0)
      {
         strncpy(PtpNode.productDescription, PtpManufacturerID,
                 sizeof(PtpNode.productDescription));
      }
      else
      {
         status = zl303xx_PtpNodeProductDescriptionSet(&desc);
      }
   }

   /* Backward compatibility: If PtpRevisionData is set, copy that into the
    * revisionData buffer. Otherwise, use the default values from zl303xx_PtpSetup.h */
   if (status == ZL303XX_OK)
   {
      if (OS_STRLEN(PtpRevisionData) > 0)
      {
         strncpy(PtpNode.revisionData, PtpRevisionData,
                 sizeof(PtpNode.revisionData));
      }
      else
      {
         status = zl303xx_PtpNodeRevisionDataSet(&rev);
      }
   }

   /* Backward compatibility: If PtpUserDescription is set, copy that into the
    * userDescription buffer. Otherwise, use the default values from zl303xx_PtpSetup.h */
   if (status == ZL303XX_OK)
   {
      if (OS_STRLEN(PtpUserDescription) > 0)
      {
         strncpy(PtpNode.userDescription, PtpUserDescription,
                 sizeof(PtpNode.userDescription));
      }
      else
      {
         status = zl303xx_PtpNodeUserDescriptionSet(&user);
      }
   }

   return status;
}

/* zl303xx_PtpNodeDescriptionSet */
/**
   Sets the node description strings. For internal use by management TLV
   handling.

  Parameters:
   [in]   mId    Manufacturer identity.
   [in]   desc   Product description.
   [in]   rev    Revision data.
   [in]   user   User description.

*******************************************************************************/
void zl303xx_PtpNodeDescriptionSet(Uint8T *mId, char *desc, char *rev, char *user)
{
   if (mId  != NULL) OS_MEMCPY(PtpNode.manufacturerIdentity, mId, sizeof(PtpNode.manufacturerIdentity));
   if (desc != NULL) strncpy(PtpNode.productDescription, desc, sizeof(PtpNode.productDescription) - 1);
   if (rev  != NULL) strncpy(PtpNode.revisionData, rev, sizeof(PtpNode.revisionData) - 1);
   if (user != NULL) strncpy(PtpNode.userDescription, user, sizeof(PtpNode.userDescription) - 1);
}

/* zl303xx_PtpNodeDescriptionGet */
/**
   Returns the node description strings. For internal use by management TLV
   handling.

  Parameters:
   [out]  mId    Manufacturer identity.
   [out]  desc   Product description.
   [out]  rev    Revision data.
   [out]  user   User description.

*******************************************************************************/
void zl303xx_PtpNodeDescriptionGet(Uint8T **mId, char **desc, char **rev, char **user)
{
   if (mId  != NULL) *mId  = PtpNode.manufacturerIdentity;
   if (desc != NULL) *desc = PtpNode.productDescription;
   if (rev  != NULL) *rev  = PtpNode.revisionData;
   if (user != NULL) *user = PtpNode.userDescription;
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/* zl303xx_PtpNodeCreate */
/** 

   Starts the PTP Node for this CPU. Initializes all arrays, tables, data, etc.
   Updates the PTP Node status. The node itself must be initialized before other
   PTP entities can be provisioned.

   This also initializes the PTP timer task used to transmit PTP messages.

  Parameters:
      None.

*******************************************************************************/
static zlStatusE zl303xx_PtpNodeCreate(void)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
         "zl303xx_PtpNodeCreate:", 0,0,0,0,0,0);

   /* Check if the node is already initialized. */
   if (status == ZL303XX_OK)
   {
      if (PtpNode.state != ZL303XX_PTP_NODE_STATE_UNINIT)
      {
         /* The node is already initialized. This is not an error, so just
          * report and return from function. */
         ZL303XX_TRACE_ERROR(
               "zl303xx_PtpNodeCreate:   PTP node has already been initialized.", 0,0,0,0,0,0);

         return status;
      }
   }

   /* Check if the node mutex has already been created. */
   if (status == ZL303XX_OK)
   {
      if (PtpNode.mutexId != OS_MUTEX_INVALID)
      {
         /* This indicates some kind of data corruption has occurred. */
         ZL303XX_TRACE_ERROR("   (ERROR) Node mutex has already been created.",
               0,0,0,0,0,0);

         status = ZL303XX_INIT_ERROR;
      }
   }

   /* Create the node mutex. */
   if (status == ZL303XX_OK)
   {
      PtpNode.mutexId = OS_MUTEX_CREATE();

      if (PtpNode.mutexId == OS_MUTEX_INVALID)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) Node mutex create failed.",
               0,0,0,0,0,0);

         status = ZL303XX_RTOS_MUTEX_CREATE_FAIL;
      }
   }

   /* Take the node mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpNodeMutexLock();
      status = mStatus;
   }

   /* Initialize node data. */
   if (status == ZL303XX_OK)
   {
      /* Data arrays. */
      OS_MEMSET(Zl303xx_PtpClockArray, 0, sizeof(Zl303xx_PtpClockArray));
      OS_MEMSET(Zl303xx_PtpPortArray, 0, sizeof(Zl303xx_PtpPortArray));
      OS_MEMSET(Zl303xx_PtpStreamArray, 0, sizeof(Zl303xx_PtpStreamArray));

      /* Clock list. */
      zl303xx_ListInit(&PtpNode.clockList);
      PtpNode.clockCount = 0;
   }

   /* Update the node state. */
   if (status == ZL303XX_OK)
   {
      PtpNode.state = PTP_NODE_DEFAULT_INIT_STATE;
   }

   /* Release the node mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpNodeMutexUnlock(status);
   }

   return status;
}

/* zl303xx_PtpNodeDelete */
/** 

   Stops the PTP node for this CPU. Updates the node Status. Calls all necessary
   shutdown/delete commands for all other PTP entities and deletes any existing
   provisioning data.

  Parameters:
   [in]  None.

*******************************************************************************/
static zlStatusE zl303xx_PtpNodeDelete(void)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
         "zl303xx_PtpNodeDelete:", 0,0,0,0,0,0);

   /* Check that the node has been initialized. */
   if (status == ZL303XX_OK)
   {
      if (PtpNode.state == ZL303XX_PTP_NODE_STATE_UNINIT)
      {
         /* The node has not been initialized yet. This is not an error, so just
          * report it and return from function. */
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2,
               "   PTP node has not been initialized yet.", 0,0,0,0,0,0);

         return status;
      }
   }

   /* Take the node mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpNodeMutexLock();
   }

   /* Delete any Clocks remaining on this Node. By design, there should not be
    * any Clocks remaining since the only time zl303xx_PtpNodeDelete is called
    * is after the last clock is deleted (but check anyway). */
   if (status == ZL303XX_OK)
   {
      Uint32T clockHandle;
      zl303xx_PtpClockDeleteS clockDeleteParams = { ZL303XX_FALSE };

      /* LOOP through each CLOCK associated with the Node and delete it. */
      /* We could follow the linked list of clocks attached to the node but
       * the inefficiency of looping the entire clock array is a safe and
       * acceptable approach at this point. */
      for (clockHandle = 0; clockHandle < ZL303XX_PTP_NUM_CLOCKS_MAX; clockHandle++)
      {
         (void)zl303xx_PtpClockDelete(clockHandle, &clockDeleteParams);
      }

      /* Initialize the clock list to the empty set. */
      zl303xx_ListInit(&PtpNode.clockList);
   }

   /* Delete the node mutex. */
   if (status == ZL303XX_OK)
   {
      /* Detaching the last Clock from the node should have cleaned-up the
       * Node control parameters. If not, do it now.  */
      if (PtpNode.mutexId != OS_MUTEX_INVALID)
      {
         if (OS_MUTEX_DELETE(PtpNode.mutexId) == OS_OK)
         {
            PtpNode.mutexId = OS_MUTEX_INVALID;
         }
         else
         {
            ZL303XX_TRACE_ERROR("   (ERROR) Node mutex delete failed. %d: %d",
                  0,0,0,0,0,0);

            status = ZL303XX_RTOS_MUTEX_DELETE_FAIL;
         }
      }
   }

   /* Set the node state to uninitialized. */
   if (status == ZL303XX_OK)
   {
      PtpNode.state = ZL303XX_PTP_NODE_STATE_UNINIT;
   }

   return status;
}

/* zl303xx_PtpNodeStringCopy */
/** 

   Copies a PTP node string into a character buffer.

  Parameters:
   [in]   src     Pointer the the beginning of the PTP node string.
   [in]   srcLen  Length of src.
   [out]  dest    Pointer to the destination character buffer.

*******************************************************************************/
static zlStatusE zl303xx_PtpNodeStringCopy(const char *src, Uint16T srcLen, char *dest)
{
   zlStatusE status = ZL303XX_OK;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(dest);
   }

   /* Copy the string into the provided array. */
   if (status == ZL303XX_OK)
   {
      strncpy(dest, src, srcLen);
   }

   return status;
}

/*****************   END   ****************************************************/
