

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*
*  This module provides the database access and management functions related
*  to the client reference selection application.
*
******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

/* The Database & User Decision headers. */
#include "zl303xx_ExampleRefSelectApi.h"
#include "zl303xx_ExampleRefSelectInternal.h"

#include <string.h>


/*****************   EXTERNS     **********************************************/

/*****************   DEFINES     **********************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/* OS Control IDs. */
OS_MUTEX_ID  exampleRefSelectMutexId    = OS_MUTEX_INVALID;

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/* Reference Selection Application Configuration Parameters are kept in the main
 * Application module but reference here as well. */
extern exampleRefSelectParamS exampleRefSelectAppParam;

/* The actual server table is kept in the main Application module but referenced
 * frequently here. */
extern exampleRefSelectEntryS exampleRefSelectTable[REF_SELECT_DB_ENTRIES_MAX];


/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/*****************   CORE DATABASE ROUTINES         ***************************/

/**
  Function Name:
   exampleRefSelectTableInit

  Details:
   Initializes the entries of the Server DB Table to their default.

  Return Value:
   zlStatusE

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleRefSelectTableInit(void)
{
   zlStatusE status = ZL303XX_OK;

   /* Create the data mutex before initializing the table. */
   if (status == ZL303XX_OK)
   {
      /* If a failure occurs, an error will be printed within the routine. */
      status = exampleRefSelectMutexCreate();
   }

   /* Lock the clock mutex before clearing the table. */
   if (status == ZL303XX_OK)
   {
      /* If a failure occurs, an error will be printed within the routine. */
      status = exampleRefSelectMutexLock();
   }

   /* Clear the table. */
   if (status == ZL303XX_OK)
   {
      Uint32T rowIndex;

      for (rowIndex = 0; rowIndex < REF_SELECT_DB_ENTRIES_MAX; rowIndex++)
      {
         /* Since the MUTEX is taken locally and all other parameters are known,
          * ignore the return status.    */
         (void)exampleRefSelectRowInit(&exampleRefSelectTable[rowIndex]);
      }

      /* If we made it here, the MUTEX was locked so Release it. */
      status = exampleRefSelectMutexUnlock();
   }

   return status;
} /* exampleRefSelectTableInit */

/**
  Function Name:
   exampleRefSelectTableDelete

  Details:
   Deletes the entries of the Server DB Table by setting them to their defaults
   and deletes the table data Mutex.

  Return Value:
   zlStatusE

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleRefSelectTableDelete(void)
{
   zlStatusE status = ZL303XX_OK;

   /* Clear the table by re-initializing it first. This will try to re-create
    * the MUTEX but return OK since it already exists. */
   if (status == ZL303XX_OK)
   {
      /* If a failure occurs, an error will be printed within the routine. */
      status = exampleRefSelectTableInit();
   }

   /* Delete the Mutex. */
   if (status == ZL303XX_OK)
   {
      if (exampleRefSelectMutexId != OS_MUTEX_INVALID)
      {
         if ((status = exampleRefSelectMutexDelete()) != ZL303XX_OK)
         {
             ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
                   "exampleRefSelectMutexDelete() returned an error= %d",
                   status, 0,0,0,0,0);
         }

         /* Regardless of the return status, reset the Mutex ID */
         exampleRefSelectMutexId = OS_MUTEX_INVALID;
      }
   }

   return status;
} /* exampleRefSelectTableDelete */

/* exampleRefSelectMutexCreate */
/** 

   Creates a mutex for the Server Database.

  Parameters:
        None

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE exampleRefSelectMutexCreate(void)
{
   zlStatusE status = ZL303XX_OK;

   /* Make sure it doesn't already exist. */
   if (exampleRefSelectMutexId != OS_MUTEX_INVALID)
   {
      /* If already created, log a message but do not return an error.
       * (since it looks like everything is OK).*/
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 3,
            "exampleRefSelectMutexCreate: MUTEX already exists",
            0,0,0,0,0,0);
   }

   /* Create the mutex. */
   else
   {
      exampleRefSelectMutexId = OS_MUTEX_CREATE();

      if (exampleRefSelectMutexId == OS_MUTEX_INVALID)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
               "exampleRefSelectMutexCreate: (ERROR) Clock mutex create failed.",
               0,0,0,0,0,0);

         status = ZL303XX_RTOS_MUTEX_CREATE_FAIL;
      }
   }

   return status;
}

/* exampleRefSelectMutexDelete */
/** 

   Deletes a mutex associated with the Server Database.

  Parameters:
        None

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE exampleRefSelectMutexDelete(void)
{
   zlStatusE status = ZL303XX_OK;

   /* Make sure the Mutex exists. */
   if (exampleRefSelectMutexId == OS_MUTEX_INVALID)
   {
      /* If already deleted, log a message but do not return an error.
       * (since it looks like everything is OK).*/
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 3,
            "exampleRefSelectMutexDelete: MUTEX already deleted",
            0,0,0,0,0,0);
   }

   /* Delete the mutex. */
   else
   {
      if (OS_MUTEX_DELETE(exampleRefSelectMutexId) == OS_OK)
      {
         exampleRefSelectMutexId = OS_MUTEX_INVALID;
      }
      else
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
               "exampleRefSelectMutexDelete: (ERROR) Clock mutex delete failed.",
               0,0,0,0,0,0);

         status = ZL303XX_RTOS_MUTEX_DELETE_FAIL;
      }
   }

   return status;
}

/* exampleRefSelectMutexLock */
/** 

   Locks the mutex associated with the Server Database.

  Parameters:
        None

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE exampleRefSelectMutexLock(void)
{
   zlStatusE status = ZL303XX_OK;

   /* Make sure the Mutex exists first. */
   if (exampleRefSelectMutexId == OS_MUTEX_INVALID)
   {
      status = ZL303XX_RTOS_MUTEX_INVALID;
   }
   else if (OS_MUTEX_TAKE(exampleRefSelectMutexId) != OS_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectMutexLock: (ERROR) Clock mutex take failed.",
            0,0,0,0,0,0);

      status = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
   }

   return status;
}

/* exampleRefSelectMutexUnlock */
/** 

   Releases the mutex associated with the Server Database.

  Parameters:
        None

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE exampleRefSelectMutexUnlock(void)
{
   zlStatusE status = ZL303XX_OK;

   /* Make sure the Mutex exists first. */
   if (exampleRefSelectMutexId == OS_MUTEX_INVALID)
   {
      status = ZL303XX_RTOS_MUTEX_INVALID;
   }
   else if (OS_MUTEX_GIVE(exampleRefSelectMutexId) != OS_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 1,
            "exampleRefSelectMutexUnlock: (ERROR) Clock mutex give failed.",
            0,0,0,0,0,0);

      status = ZL303XX_RTOS_MUTEX_GIVE_FAIL;
   }

   return status;
}

/**
  Function Name:
   exampleRefSelectRowInit

  Details:
   Initializes an individual row (entry) of the Server DB Table to default values.

  Parameters:
   [in]  pRowEntry      Pointer to the table entry to initialize.

  Return Value:
     ZL303XX_OK                      Success.
     ZL303XX_INVALID_POINTER         Input pointer is NULL.
     ZL303XX_PARAMETER_INVALID       A supplied parameter is INVALID.

  Notes:
   This routine assumes that the caller has taken the data MUTEX if necessary.
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleRefSelectRowInit(
      exampleRefSelectEntryS *pRowEntry)
{
   zlStatusE status = ZL303XX_OK;

   /* Initialize the Entry. */
   if (status == ZL303XX_OK)
   {
      /* Clear all of the data */
      /* This will set the clockClass (QL) values to 0. */
      memset(pRowEntry, 0x00, sizeof(exampleRefSelectEntryS));

      /* Force the connection IDs to INVALID. */
      pRowEntry->clockHandle = REF_SELECT_CONN_ID_INVALID;
      pRowEntry->streamHandle = REF_SELECT_CONN_ID_INVALID;
      pRowEntry->connectionId = REF_SELECT_CONN_ID_INVALID;

      /* Invalidate the individual blocks of data. */
      pRowEntry->protocol.valid = ZL303XX_FALSE;
      pRowEntry->ptsf.valid = ZL303XX_FALSE;
      pRowEntry->algorithm.valid = ZL303XX_FALSE;
      pRowEntry->custom.valid = ZL303XX_FALSE;
   }

   return status;
} /* exampleRefSelectRowInit */

/*****************   DATABASE SEARCH ROUTINES (INTERNAL)   ********************/

/**
  Function Name:
   exampleRefSelectTableFindProtocolStream

  Details:
   Searches the Server Database for the specified entry for a Protocol Stream.

  Return Value:
   zlStatusE
      ZL303XX_OK                      The entry is found at 'entryIndex'.
      ZL303XX_TABLE_ENTRY_NOT_FOUND   The entry is not found; 'entryIndex' is the
                                       first empty index available.
      ZL303XX_TABLE_FULL              The entry is not found; 'entryIndex' is -1
                                       since there are no empty entries.

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleRefSelectTableFindProtocolStream(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_PtpClockHandleT clockHandle,
      Uint32T *entryIndex)
{
   zlStatusE status = ZL303XX_OK;

   Uint32T rowIndex;
   Uint32T firstEmptyRow = REF_SELECT_CONN_ID_INVALID;

   /* Set a default return value. */
   *entryIndex = REF_SELECT_CONN_ID_INVALID;

   for (rowIndex = 0; rowIndex < REF_SELECT_DB_ENTRIES_MAX; rowIndex++)
   {
      /* Exact Row */
      if ((exampleRefSelectTable[rowIndex].clockHandle == clockHandle) &&
          (exampleRefSelectTable[rowIndex].streamHandle == streamHandle))
      {
         *entryIndex = rowIndex;
         break;
      }

      /* Empty Row */
      else if ((firstEmptyRow == REF_SELECT_CONN_ID_INVALID) &&
               (exampleRefSelectTable[rowIndex].clockHandle == REF_SELECT_CONN_ID_INVALID) &&
               (exampleRefSelectTable[rowIndex].streamHandle == REF_SELECT_CONN_ID_INVALID))
      {
         firstEmptyRow = rowIndex;
      }

      /* ELSE: another in-use row. */
   }

   /* If the entry does not exist, return the index of the first empty row.
    * If their are no empty rows, then (-1) will indicate table full.   */
   if (*entryIndex == REF_SELECT_CONN_ID_INVALID)
   {
      if (firstEmptyRow == REF_SELECT_CONN_ID_INVALID)
      {
         /* If the table is full, just return -1. */
         status = ZL303XX_TABLE_FULL;
      }
      else
      {
         /* If the entry is not found but space is available, return
          * the index to the empty row in case the row will be added.  */
         *entryIndex = firstEmptyRow;

         status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
      }
   }

   return status;
}

/**
  Function Name:
   exampleRefSelectTableFindProtocolClock

  Details:
   Searches the Server Database for the specified Protocol Clock.

  Return Value:
   zlStatusE
      ZL303XX_OK                      The entry is found at 'entryIndex'.
      ZL303XX_TABLE_ENTRY_NOT_FOUND   The entry is not found; 'entryIndex' is the
                                       first empty index available.
      ZL303XX_TABLE_FULL              The entry is not found; 'entryIndex' is -1
                                       since there are no empty entries.

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleRefSelectTableFindProtocolClock(
      zl303xx_PtpClockHandleT clockHandle,
      Uint32T *entryIndex)
{
   zlStatusE status = ZL303XX_OK;

   Uint32T rowIndex;
   Uint32T firstEmptyRow = REF_SELECT_CONN_ID_INVALID;

   /* Set a default return value. */
   *entryIndex = REF_SELECT_CONN_ID_INVALID;

   for (rowIndex = 0; rowIndex < REF_SELECT_DB_ENTRIES_MAX; rowIndex++)
   {
      /* Exact Row */
      if ((exampleRefSelectTable[rowIndex].clockHandle == clockHandle) &&
          (exampleRefSelectTable[rowIndex].entryType == REF_SELECT_CONN_TYPE_PROTOCOL_CLOCK))
      {
         *entryIndex = rowIndex;
         break;
      }

      /* Empty Row */
      else if ((firstEmptyRow == REF_SELECT_CONN_ID_INVALID) &&
               (exampleRefSelectTable[rowIndex].clockHandle == REF_SELECT_CONN_ID_INVALID) &&
               (exampleRefSelectTable[rowIndex].streamHandle == REF_SELECT_CONN_ID_INVALID))
      {
         firstEmptyRow = rowIndex;
      }

      /* ELSE: another in-use row. */
   }

   /* If the entry does not exist, return the index of the first empty row.
    * If their are no empty rows, then (-1) will indicate table full.   */
   if (*entryIndex == REF_SELECT_CONN_ID_INVALID)
   {
      if (firstEmptyRow == REF_SELECT_CONN_ID_INVALID)
      {
         /* If the table is full, just return -1. */
         status = ZL303XX_TABLE_FULL;
      }
      else
      {
         /* If the entry is not found but space is available, return
          * the index to the empty row in case the row will be added.  */
         *entryIndex = firstEmptyRow;

         status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
      }
   }

   return status;
}

/**
  Function Name:
   exampleRefSelectTableFindConnection

  Details:
   Searches the Server Database for the specified connection (Server).

  Return Value:
   zlStatusE
      ZL303XX_OK                      The entry is found at 'entryIndex'.
      ZL303XX_TABLE_ENTRY_NOT_FOUND   The entry is not found; 'entryIndex' is the
                                       first empty index available.
      ZL303XX_TABLE_FULL              The entry is not found; 'entryIndex' is -1
                                       since there are no empty entries.

  Notes:
   Since this is a local routine, no checking of the input pointer is done.

*******************************************************************************/
zlStatusE exampleRefSelectTableFindConnection(
      Uint32T connectionId,
      Uint32T *entryIndex)
{
   zlStatusE status = ZL303XX_OK;

   Uint32T rowIndex;
   Uint32T firstEmptyRow = REF_SELECT_CONN_ID_INVALID;

   /* Set a default return value. */
   *entryIndex = REF_SELECT_CONN_ID_INVALID;

   for (rowIndex = 0; rowIndex < REF_SELECT_DB_ENTRIES_MAX; rowIndex++)
   {
      /* Exact Row */
      if ((exampleRefSelectTable[rowIndex].connectionId == connectionId) &&
          (exampleRefSelectTable[rowIndex].entryType == REF_SELECT_CONN_TYPE_PROTOCOL_STREAM))
      {
         *entryIndex = rowIndex;
         break;
      }

      /* Empty Row */
      else if ((firstEmptyRow == REF_SELECT_CONN_ID_INVALID) &&
               (exampleRefSelectTable[rowIndex].clockHandle == REF_SELECT_CONN_ID_INVALID) &&
               (exampleRefSelectTable[rowIndex].streamHandle == REF_SELECT_CONN_ID_INVALID))
      {
         firstEmptyRow = rowIndex;
      }

      /* ELSE: another in-use row. */
   }

   /* If the entry does not exist, return the index of the first empty row.
    * If their are no empty rows, then (-1) will indicate table full.   */
   if (*entryIndex == REF_SELECT_CONN_ID_INVALID)
   {
      if (firstEmptyRow == REF_SELECT_CONN_ID_INVALID)
      {
         /* If the table is full, just return -1. */
         status = ZL303XX_TABLE_FULL;
      }
      else
      {
         /* If the entry is not found but space is available, return
          * the index to the empty row in case the row will be added.  */
         *entryIndex = firstEmptyRow;

         status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
      }
   }

   return status;
}

/*****************   END   ****************************************************/

