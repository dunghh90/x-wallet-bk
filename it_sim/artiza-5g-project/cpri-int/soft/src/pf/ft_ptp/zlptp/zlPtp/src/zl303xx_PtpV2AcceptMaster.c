

/*****************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Implementation of the V2 Acceptable Master functionality.
*
******************************************************************************/

/****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"

#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpFunctionExport.h"
#include "zl303xx_PtpV2AcceptMaster.h"
#include "zl303xx_PtpTable.h"

/****************   DEFINES     **********************************************/

/****************   DATA TYPES   *********************************************/

/****************   STATIC FUNCTION DECLARATIONS   ***************************/
static void zl303xx_PtpAmtEntryInit(const void *key, void *entry);
static zl303xx_BooleanE zl303xx_PtpAmtMatch(const void *key, const void *entry);

/****************   STATIC GLOBAL VARIABLES   ********************************/
static const zl303xx_PtpTableOpsS Zl303xx_PtpAmtOps = {
      zl303xx_PtpAmtEntryInit, zl303xx_PtpAmtMatch
};

/****************   EXPORTED GLOBAL VARIABLES   ******************************/

/****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* zl303xx_PtpAcceptableMasterStructInit */
/**
   Initializes the acceptable master data structure.

  Parameters:
   [out]  pAcceptableMaster  This initialized structure.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pAcceptableMaster is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpAcceptableMasterStructInit(zl303xx_AcceptableMaster *pAcceptableMaster)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "zl303xx_PtpAcceptableMasterStructInit:", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pAcceptableMaster);
   }

   /* Initialize the structure with default values. */
   if (status == ZL303XX_OK)
   {
      OS_MEMSET(pAcceptableMaster, 0, sizeof(*pAcceptableMaster));
      pAcceptableMaster->acceptableAddress.networkProtocol = ZL303XX_NETWORK_PROTOCOL_INVALID;
   }

   return status;
}

/* zl303xx_PtpAcceptableMasterAdd */
/**
   Adds an entry in a port's acceptable master table.

  Parameters:
   [in]   portHandle         Handle to a previous created port.
   [in]   pAcceptableMaster  Acceptable master information to add.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       pAcceptableMaster is NULL
     ZL303XX_PARAMETER_INVALID     The master's port address is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Unable to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Unable to release mutex.
     ZL303XX_TABLE_FULL            No space left in the table.

*******************************************************************************/
zlStatusE zl303xx_PtpAcceptableMasterAdd(zl303xx_PtpPortHandleT portHandle,
                                         zl303xx_AcceptableMaster *pAcceptableMaster)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_PortDataS *port = NULL;
   zl303xx_PtpAccMasterEntryS *amtEntry = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pAcceptableMaster);
   }
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortAddressCheck(&pAcceptableMaster->acceptableAddress);
   }

   /* Get a pointer to the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &port);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(port->clock);
      status = mStatus;
   }

   /* Add or update this entry in the Acceptable Master Table. */
   if (status == ZL303XX_OK)
   {
      amtEntry = zl303xx_PtpTableAdd(&port->accMasterTbl,
                       &pAcceptableMaster->acceptableAddress);

      if (amtEntry == NULL)
      {
         ZL303XX_TRACE_ALWAYS("zl303xx_PtpAcceptableMasterAdd: Table full (port %u).", portHandle, 0,0,0,0,0);
         status = ZL303XX_TABLE_FULL;
      }
   }

   /* Update the alternatePriority1 field even if the entry already exists. */
   if (status == ZL303XX_OK)
   {
      amtEntry->alternatePriority1 = pAcceptableMaster->alternatePriority1;
   }

   /* Unlock the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(port->clock, status);
   }

   return status;
}

/* zl303xx_PtpAcceptableMasterRemove */
/**
   Removes an entry in a port's acceptable master table.

  Parameters:
   [in]   portHandle    Handle to a previous created port.
   [in]   pPortAddress  Address of the acceptable master to remove.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       pPortAddress is NULL
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Unable to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Unable to release mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpAcceptableMasterRemove(zl303xx_PtpPortHandleT portHandle,
                                            zl303xx_PortAddress *pPortAddress)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *port = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pPortAddress);
   }

   /* Get a pointer to the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &port);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(port->clock);
   }

   if (status == ZL303XX_OK)
   {
      /* Remove this entry from the Acceptable Master Table. */
      zl303xx_PtpTableRemove(&port->accMasterTbl, pPortAddress);

      /* Unlock the clock mutex. */
      status = zl303xx_PtpClockMutexUnlock(port->clock, status);
   }

   return status;
}

/* zl303xx_PtpAcceptableMasterSetAltPriority */
/**
   Updates the alternatePriority1 value for an existing Acceptable Master entry.

  Parameters:
   [in]   portHandle         Handle to a previous created port.
   [in]   pAcceptableMaster  Acceptable master information to update.

  Return Value:
     ZL303XX_OK                     Success.
     ZL303XX_INVALID_POINTER        pAcceptableMaster is NULL
     ZL303XX_PARAMETER_INVALID      The master's port address is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL   Unable to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL   Unable to release mutex.
     ZL303XX_TABLE_ENTRY_NOT_FOUND  The expected entry was not found.

*******************************************************************************/
zlStatusE zl303xx_PtpAcceptableMasterSetAltPriority(zl303xx_PtpPortHandleT portHandle,
                                       zl303xx_AcceptableMaster *pAcceptableMaster)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_PortDataS *port = NULL;
   zl303xx_PtpAccMasterEntryS *amtEntry = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pAcceptableMaster);
   }

   /* Get a pointer to the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &port);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(port->clock);
      status = mStatus;
   }

   /* Update this entry in the Acceptable Master Table. */
   if (status == ZL303XX_OK)
   {
      amtEntry = zl303xx_PtpTableFind(&port->accMasterTbl,
                    &pAcceptableMaster->acceptableAddress);
      if (amtEntry == NULL)
      {
         status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
      }
   }

   if (status == ZL303XX_OK)
   {
      amtEntry->alternatePriority1 = pAcceptableMaster->alternatePriority1;
   }

   /* Unlock the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(port->clock, status);
   }

   return status;
}

/* zl303xx_PtpAcceptableMasterDroppedCountsGet */
/**
   Returns the count of dropped message that occurred due to the Acceptable
   Master Table of the specified port.

  Parameters:
   [in]   portHandle       Handle to a previous created port.
   [in]   msgCountArray    An Uint32T array of dropped-message counts. This
                                 array should be ZL303XX_PTP_V2_NUM_MSG_IDS
                                 entries long and indexed using the
                                 zl303xx_MessageTypeE enumeration
                                 (i.e. non-valid messageTypes will still have a
                                 space in the array).
   [in]   bClearCounts     Boolean indicating whether to clear the counters
                                 after this read.

  Return Value:
     ZL303XX_OK                     Success.
     ZL303XX_INVALID_POINTER        msgCountArray is NULL
     ZL303XX_PARAMETER_INVALID      portHandle or bClearCounts is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL   Unable to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL   Unable to release mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpAcceptableMasterDroppedCountsGet(
      zl303xx_PtpPortHandleT portHandle,
      Uint32T *msgCountArray,
      zl303xx_BooleanE bClearCounts)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T msgId;
   zl303xx_PortDataS *pPtpPort = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(msgCountArray);
   }

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(bClearCounts);
   }

   /* Get a pointer to the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPtpPort);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpPort->clock);
   }

   /* Retrieve the counters for this Acceptable Master Table. */
   if (status == ZL303XX_OK)
   {
      /* Copy the counters to the users array. */
      for (msgId = 0; msgId < ZL303XX_PTP_V2_NUM_MSG_IDS; msgId++)
      {
         /* Copy only valid message types. */
         if (ZL303XX_CHECK_MESSAGE_TYPE(msgId) == ZL303XX_OK)
         {
            msgCountArray[msgId] = pPtpPort->amtDroppedMsgCount[msgId];
         }
         else
         {
            msgCountArray[msgId] = 0;
         }
      }

      /* Clear all counters if required. */
      if (bClearCounts == ZL303XX_TRUE)
      {
         OS_MEMSET(&pPtpPort->amtDroppedMsgCount, 0x00,
                sizeof(pPtpPort->amtDroppedMsgCount));
      }

      /* Unlock the clock mutex. */
      status = zl303xx_PtpClockMutexUnlock(pPtpPort->clock, status);
   }

   return status;
}  /* zl303xx_PtpAcceptableMasterDroppedCountsGet */

/* zl303xx_PtpAcceptableMasterDroppedCountsClear */
/**
   Clears the count of dropped message that occurred due to the Acceptable
   Master Table of the specified port.

  Parameters:
   [in]   portHandle       Handle to a previous created port.

  Return Value:
     ZL303XX_OK                     Success.
     ZL303XX_PARAMETER_INVALID      portHandle is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL   Unable to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL   Unable to release mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpAcceptableMasterDroppedCountsClear(
      zl303xx_PtpPortHandleT portHandle)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPtpPort = NULL;

   /* Get a pointer to the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPtpPort);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(pPtpPort->clock);
   }

   /* Clear the counters. */
   if (status == ZL303XX_OK)
   {
      OS_MEMSET(&pPtpPort->amtDroppedMsgCount, 0x00,
             sizeof(pPtpPort->amtDroppedMsgCount)); /* sizeof the declared array */

      /* Unlock the clock mutex. */
      status = zl303xx_PtpClockMutexUnlock(pPtpPort->clock, status);
   }

   return status;
}  /* zl303xx_PtpAcceptableMasterDroppedCountsClear */

/* zl303xx_PtpAcceptableMasterTableEnSet */
/**
   Enables or disables the use of the Acceptable Master Table on a PTP port.

  Parameters:
   [in]   portHandle  Handle to a previously created PTP port.
   [in]   enabled     Flag to enable/disable the Acceptable Master Table.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     enabled is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Unable to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Unable to release mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpAcceptableMasterTableEnSet(Uint32T portHandle,
                                                zl303xx_BooleanE enabled)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(enabled);
   }

   /* When enabling the AMT, clear the counters automatically. */
   if (status == ZL303XX_OK)
   {
      if (enabled == ZL303XX_TRUE)
      {
         status = zl303xx_PtpAcceptableMasterDroppedCountsClear(portHandle);
      }
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

   /* Update the Acceptable Master Table enabled flag. */
   if (status == ZL303XX_OK)
   {
      pPort->config.acceptableMasterTableEnabled = enabled;

      /* Release the clock mutex. */
      status = zl303xx_PtpClockMutexUnlock(pPort->clock, status);
   }

   return status;
}

/* zl303xx_PtpAcceptableSlaveStructInit */
/**
   Initializes the acceptable slave data structure.

  Parameters:
   [out]  pAcceptableSlave  This initialized structure.

  Return Value:
     ZL303XX_OK               Success.
     ZL303XX_INVALID_POINTER  pAcceptableSlave is NULL.

*******************************************************************************/
zlStatusE zl303xx_PtpAcceptableSlaveStructInit(zl303xx_AcceptableSlave *pAcceptableSlave)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "zl303xx_PtpAcceptableSlaveStructInit:", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pAcceptableSlave);
   }

   /* Initialized the structure with default values. */
   if (status == ZL303XX_OK)
   {
      OS_MEMSET(pAcceptableSlave, 0, sizeof(*pAcceptableSlave));
      pAcceptableSlave->acceptableAddress.networkProtocol = ZL303XX_NETWORK_PROTOCOL_INVALID;
   }

   return status;
}

/* zl303xx_PtpAcceptableSlaveAdd */
/**
   Adds an entry in a port's acceptable slave table.

  Parameters:
   [in]   portHandle        Handle to a previous created port.
   [in]   pAcceptableSlave  Acceptable slave information to add. The
                                 alternatePriority1 member is ignored.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       pAcceptableSlave is NULL
     ZL303XX_PARAMETER_INVALID     The slave's port address is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Unable to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Unable to release mutex.
     ZL303XX_TABLE_FULL            No space left in the table.

*******************************************************************************/
zlStatusE zl303xx_PtpAcceptableSlaveAdd(zl303xx_PtpPortHandleT portHandle,
                                        zl303xx_AcceptableSlave *pAcceptableSlave)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_ERROR;
   zl303xx_PortDataS *port = NULL;
   zl303xx_PtpAccSlaveEntryS *astEntry;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pAcceptableSlave);
   }
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortAddressCheck(&pAcceptableSlave->acceptableAddress);
   }

   /* Get a pointer to the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &port);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(port->clock);
      status = mStatus;
   }

   /* Add or update this entry in the Acceptable Slave Table. */
   if (status == ZL303XX_OK)
   {
      astEntry = zl303xx_PtpTableAdd(&port->accSlaveTbl,
                                     &pAcceptableSlave->acceptableAddress);

      if (astEntry == NULL)
      {
         ZL303XX_TRACE_ALWAYS("zl303xx_PtpAcceptableSlaveAdd: Table full (port %u).",
                              portHandle, 0,0,0,0,0);
         status = ZL303XX_TABLE_FULL;
      }
   }

   /* Unlock the clock mutex. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(port->clock, status);
   }

   return status;
}

/* zl303xx_PtpAcceptableSlaveRemove */
/**
   Removes an entry from a port's acceptable slave table.

  Parameters:
   [in]   portHandle    Handle to a previous created port.
   [in]   pPortAddress  Address of the acceptable slave to remove.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_INVALID_POINTER       pPortAddress is NULL
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Unable to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Unable to release mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpAcceptableSlaveRemove(zl303xx_PtpPortHandleT portHandle,
                                           zl303xx_PortAddress *pPortAddress)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *port = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pPortAddress);
   }

   /* Get a pointer to the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &port);
   }

   /* Lock the clock mutex. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexLock(port->clock);
   }

   if (status == ZL303XX_OK)
   {
      /* Remove this entry from the Acceptable Slave Table. */
      zl303xx_PtpTableRemove(&port->accSlaveTbl, pPortAddress);

      /* Unlock the clock mutex. */
      status = zl303xx_PtpClockMutexUnlock(port->clock, status);
   }

   return status;
}

/* zl303xx_PtpAcceptableSlaveTableEnSet */
/**
   Enables or disables the use of the Acceptable Slave Table on a PTP port.

  Parameters:
   [in]   portHandle  Handle to a previously created PTP port.
   [in]   enabled     Flag to enable/disable the Acceptable Slave Table.

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     enabled is invalid.

*******************************************************************************/
zlStatusE zl303xx_PtpAcceptableSlaveTableEnSet(
      Uint32T portHandle,
      zl303xx_BooleanE enabled)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PortDataS *pPort = NULL;

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_BOOLEAN(enabled);
   }

   /* Get a pointer to the port structure. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortDataGet(portHandle, &pPort);
   }

   /* No need to lock the clock mutex for a Boolean change. */
   /* Update the Acceptable Master Table enabled flag. */
   if (status == ZL303XX_OK)
   {
      pPort->config.acceptableSlaveTableEnabled = enabled;
   }

   return status;
}

/****************   INTERNAL PTP FUNCTION DEFINITIONS   ***********************/

/* zl303xx_PtpAccMasterTblCreate */
/** 

   Allocates memory and initializes the Acceptable Master Table structure.

  Parameters:
   [in]   maxEntries  Number of entries to allocate for the table.
   [out]  amt         Initialized Acceptable Master Table.

*******************************************************************************/
zlStatusE zl303xx_PtpAccMasterTblCreate(Uint16T maxEntries, zl303xx_PtpTableS *amt)
{
   return zl303xx_PtpTableCreate(maxEntries, sizeof(zl303xx_PtpAccMasterEntryS),
                                 &Zl303xx_PtpAmtOps, amt);
}

/* zl303xx_PtpAccMasterTblDelete */
/** 

   Frees memory used by the Acceptable Master Table.

  Parameters:
   [in,out]  amt  Pointer to an Acceptable Master Table to delete.

*******************************************************************************/
void zl303xx_PtpAccMasterTblDelete(zl303xx_PtpTableS *amt)
{
   zl303xx_PtpTableDestroy(amt);
}

/* zl303xx_PtpAccSlaveTblCreate */
/** 

   Allocates memory and initializes the Acceptable Slave Table structure.

  Parameters:
   [in]   maxEntries  Number of entries to create in the table.
   [out]  ast         Initialized Acceptable Slave Table.

*******************************************************************************/
zlStatusE zl303xx_PtpAccSlaveTblCreate(Uint16T maxEntries, zl303xx_PtpTableS *ast)
{
   return zl303xx_PtpTableCreate(maxEntries, sizeof(zl303xx_PtpAccSlaveEntryS),
                                 &Zl303xx_PtpAmtOps, ast);
}

/* zl303xx_PtpAccSlaveTblDelete */
/** 

 Frees memory used by the Acceptable Slave Table.

  Parameters:
   [in,out]  ast  Pointer to an Acceptable Slave Table to delete.

*******************************************************************************/
void zl303xx_PtpAccSlaveTblDelete(zl303xx_PtpTableS *ast)
{
   zl303xx_PtpTableDestroy(ast);
}

/* zl303xx_PtpAmtEntryInit */
/** 

   Callback function for zl303xx_PtpTableOpsS::init().

  Parameters:
   [in]   entry  Pointer to the AMT/AST entry structure.
   [in]   key    Pointer to the portAddress passed to zl303xx_PtpTableAdd().

*******************************************************************************/
static void zl303xx_PtpAmtEntryInit(const void *key, void *entry)
{
   zl303xx_PtpAccMasterEntryS *amtEntry = entry;
   const zl303xx_PortAddress *addr = key;

   /* An alternate priority of 0 indicates to use the priority1 value in
    * the Announce packet header rather than the value from the AMT. */
   amtEntry->addr = *addr;
   amtEntry->alternatePriority1 = 0;
   amtEntry->streamHandle = (zl303xx_PtpStreamHandleT)ZL303XX_PTP_INVALID;
}

/* zl303xx_PtpAmtMatch */
/** 

   Callback function for zl303xx_PtpTableOpsS::keyMatch(). Determines if an AMT/AST
   entry matches the portAddress (key).

  Parameters:
   [in]   entry  Pointer to the AMT/AST entry structure.
   [in]   key    Pointer to the portAddress passed to zl303xx_PtpTableFind().

*******************************************************************************/
static zl303xx_BooleanE zl303xx_PtpAmtMatch(const void *key, const void *entry)
{
   const zl303xx_PtpAccMasterEntryS *amtEntry = entry;
   return !zl303xx_PtpComparePortAddress(&amtEntry->addr, key);
}

/******************************   END   **************************************/
