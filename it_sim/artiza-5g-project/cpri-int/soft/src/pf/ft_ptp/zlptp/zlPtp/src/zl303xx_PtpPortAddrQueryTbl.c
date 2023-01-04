

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Functions related to managing a port address query table data structure.
*     This table is required for several of the 1588 features (including Unicast
*     Discovery & Master Clusters). The following definitions and functions
*     provide a one-stop-shop for any feature requiring such a table.
*
*******************************************************************************/

/****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_ErrTrace.h"
#include "zl303xx_Os.h"
#include "zl303xx_Trace.h"

#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpPortAddrQueryTbl.h"
#include "zl303xx_PtpV2MsgParse.h"

/****************   DEFINES     **********************************************/

/****************   DATA TYPES   *********************************************/

/* Query Rate enum */
typedef enum
{
   ZL303XX_QUERY_RATE_TYPE_ZERO = 0,
   ZL303XX_QUERY_RATE_TYPE_PPS = 1,
   ZL303XX_QUERY_RATE_TYPE_SPP = 2,

   /* define a couple of bounds values */
   ZL303XX_QUERY_RATE_TYPE_INVALID,
   ZL303XX_QUERY_RATE_TYPE_MAX = (ZL303XX_QUERY_RATE_TYPE_INVALID - 1)
} zl303xx_QueryRateTypeE;

/* Query Rate type */
typedef struct
{
   zl303xx_QueryRateTypeE type;
   Uint32T rate;
} zl303xx_QueryRateS;

/****************   STATIC FUNCTION DECLARATIONS   ***************************/

zlStatusE zl303xx_GenericPortAddrTblRateGet(zl303xx_GenericPortAddrTblS *tblPar,
                                            zl303xx_QueryRateS *queryRate);

zlStatusE zl303xx_GenericPortAddrTblRateSet(zl303xx_GenericPortAddrTblS *tblPar,
                                            zl303xx_QueryRateS *queryRate);

zlStatusE zl303xx_V2PackPortAddrTblTlvFromGen(zl303xx_GenericPortAddrTblS *tblPar,
                                              Uint8T *buffer, Uint16T *len);

/****************   STATIC GLOBAL VARIABLES   ********************************/

/* Create an empty table row (used by various functions)                      */
zl303xx_GenericPortInfoS emptyRow = { ZL303XX_TRUE,       /* empty                */
                                    0,                /* portAddressHandle    */
                                    0,                /* priority             */
                                    { 0, 0, {0}}      /* zl303xx_PortAddress    */
                                  };

/****************   EXPORTED GLOBAL VARIABLES   ******************************/

/****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/*

  Function Name:
    zl303xx_GenericPortAddrTblStructInit

  Details:
   Initializes a Generic Port Address Query Table structure

  Parameters:
   [in]    tblPar   Pointer to the Generic Port Address Table structure to be
               initialized

  Return Value:
   zlStatusE

  Notes:
   This function assumes that the table has never been initialized and so sets
   the portInfo pointer to NULL without calling DE-ALLOC. Just before calling
   this INIT function, the user should set the status member of the structure
   to UNINIT and let the Generic Port Table functions control it afterwards. If
   the user manipulates the status manually after previously MALLOCing space,
   the MALLOCed space can be lost if INIT is called again.

*******************************************************************************/

zlStatusE zl303xx_GenericPortAddrTblStructInit(zl303xx_GenericPortAddrTblS *tblPar)
{
   zlStatusE status = ZL303XX_OK;

   /* Check that the table parameter pointer is valid. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tblPar);
   }

   /* See function header  Notes:(above) */
   /* Make sure the structure has never MALLOCed any space previously */
   if (status == ZL303XX_OK)
   {
      if (tblPar->status > ZL303XX_GPA_TBL_STATUS_INIT)
      {
         status = ZL303XX_INVALID_MODE;
      }
   }

   /* Assign some necessary & reasonable default values */
   if (status == ZL303XX_OK)
   {
      /* Allow the user to create & manipulate the table from here on */
      tblPar->status = ZL303XX_GPA_TBL_STATUS_INIT;

      /* The user application will set the MAX size during creation */
      tblPar->maxTableSize = 0;
      tblPar->actualTableSize = 0;

      /* Each application should set its own range and interval but for now
         set some reasonable defaults (Interval = 1 PPS; MAX = 1024 PPS) */
      tblPar->logQueryInterval = GEN_TBL_LOG_QUERY_INTERVAL_DEF;
      tblPar->logQueryIntervalMax = GEN_TBL_LOG_QUERY_INTERVAL_DEF_MAX;
      tblPar->logQueryIntervalMin = GEN_TBL_LOG_QUERY_INTERVAL_DEF_MIN;

      /* See function header  Notes:(above) */
      tblPar->portInfo = NULL;
   }

   return status;
}  /* END zl303xx_GenericPortAddrTblStructInit */

/*

  Function Name:
    zl303xx_GenericPortAddrTblStructReInit

  Details:
   Re-Initializes an already initialized Generic Port Address Query Table
   structure. It calls Destroy to de-allocate and memory before resetting all
   other structure members to default values.

  Parameters:
   [in]    tblPar   Pointer to the Generic Port Address Table structure to be
               re-initialized

  Return Value:
   zlStatusE

  Notes:
   This function can be used in place of zl303xx_GenericPortAddrTblInit with the
   same condition that the user has set the status member of the structure to
   UNINIT before calling it FOR THE FIRST TIME (to actually init). If a true
   re-initialization is needed, the user should not manipulate the status
   member manually.

*******************************************************************************/

zlStatusE zl303xx_GenericPortAddrTblStructReInit(zl303xx_GenericPortAddrTblS *tblPar)
{
   zlStatusE status = ZL303XX_OK;

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tblPar);
   }

   /* Make sure the table is in some known state */
   if (status == ZL303XX_OK)
   {
      if (tblPar->status > ZL303XX_GPA_TBL_STATUS_MAX)
      {
         status = ZL303XX_INVALID_MODE;
      }
   }

   /* If memory was allocated for the portInfo entries, then deallocate it */
   if (status == ZL303XX_OK)
   {
      if (tblPar->status >= ZL303XX_GPA_TBL_STATUS_CREATED)
      {
         /* Destroy will set the table state back to INIT (from CREATED) */
         status = zl303xx_GenericPortAddrTblDestroy(tblPar);
      }
   }

   /* Assign the default structure values */
   if (status == ZL303XX_OK)
   {
      /* We should be in INIT or UNINIT state at this point */
      status = zl303xx_GenericPortAddrTblStructInit(tblPar);
   }

   /* We could set the table status to INIT here (if status == OK). However, if
      status is OK then the StructInit function will have already set it. */

   return status;
}  /* END zl303xx_GenericPortAddrTblStructReInit */

/*

  Function Name:
    zl303xx_GenericPortAddrTblCreate

  Details:
   Creates a specified number of portInfo entries (maxTableSize) in a Generic
   Port Address Query Table

  Parameters:
   [in]    tblPar   Pointer to the Generic Port Address Query Table structure in
               which to create the maxTableSize portInfo entries

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_GenericPortAddrTblCreate(zl303xx_GenericPortAddrTblS *tblPar)
{
   zlStatusE status = ZL303XX_OK;
   OS_STATUS osStatus = OS_OK;

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tblPar);
   }

   /* Make sure we are in a valid state to MALLOC table space */
   if (status == ZL303XX_OK)
   {
      if (tblPar->status != (ZL303XX_GPA_TBL_STATUS_CREATED - 1))
      {
         status = ZL303XX_INVALID_MODE;
      }
      else if (tblPar->portInfo != NULL)
      {
         /* Appears we are in a ready-to-create state but the table pointer is
            already assigned. We cannot be sure if the caller simply neglected
            to initialize the table structure or if the table really contains
            good data (and allocated memory). Either way, force the calling
            routine to manage the existing table. */
         status = ZL303XX_INIT_ERROR;
      }
   }

   /* Checking that the user is not attempting to create an empty table */
   if (status == ZL303XX_OK)
   {
      if (tblPar->maxTableSize == 0)
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Check if we can support the user supplied Query Interval & Range */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_GenericPortAddrTblLogIntervalChk(tblPar);
   }

   /* Allocate the memory needed for this generic port address table */
   if (status == ZL303XX_OK)
   {
      tblPar->portInfo = (zl303xx_GenericPortInfoS *)OS_CALLOC(
                     tblPar->maxTableSize, sizeof(zl303xx_GenericPortInfoS));

      if (tblPar->portInfo == NULL)
      {
         ZL303XX_ERROR_TRAP("Memory allocation error");
         osStatus = OS_ERROR;
         status = ZL303XX_RTOS_MEMORY_FAIL;
      }
   }

   /* Initialize each of the portInfo Entries */
   if (status == ZL303XX_OK)
   {
      /* Clearing a table requires that it is in CREATE state. So, fake out the
         function. (We would have had to do this after anyway. If the call fails
         the Destroy call will fix it). */
      tblPar->status = ZL303XX_GPA_TBL_STATUS_CREATED;

      status = zl303xx_GenericPortAddrTblClearEntries(tblPar);
   }

   /* Destroy the table if something went wrong */
   if (status != ZL303XX_OK)
   {
      /* if failure occurred after memory was allocated, free it */
      if (osStatus == OS_OK)
      {
         /* attempt to free memory but maintain the original status code */
         (void)zl303xx_GenericPortAddrTblDestroy(tblPar);
      }
   }

   return status;
}  /* END zl303xx_GenericPortAddrTblCreate */

/*

  Function Name:
    zl303xx_GenericPortAddrTblDestroy

  Details:
   Deletes the portInfo entries from a Generic Port Address Query Table and
   frees any allocated memory

  Parameters:
   [in]    tblPar   Pointer to the Generic Port Address Query Table structure from
               which to remove the portInfo entries & de-allocate memory

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_GenericPortAddrTblDestroy(zl303xx_GenericPortAddrTblS *tblPar)
{
   zlStatusE status = ZL303XX_OK;

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tblPar);
   }

   /* Make sure the table exists before we destroy it */
   if (status == ZL303XX_OK)
   {
      if (tblPar->status != ZL303XX_GPA_TBL_STATUS_CREATED)
      {
         status = ZL303XX_INVALID_MODE;
      }
      else if ((tblPar->portInfo == NULL) || (tblPar->maxTableSize == 0))
      {
         /* Technically we do not have to check this since DESTROY would have
            cleaned it up. However, if the table is in CREATED state, then
            maxTableSize could not have been 0 before creation and the
            portInfo pointer should not be NULL afterwards. Since something
            is mismatched, report the situation rather than continue. */
         status = ZL303XX_INIT_ERROR;
      }
   }

   /* If any zl303xx_GenericPortInfoS entries exist, destroy them */
   if (status == ZL303XX_OK)
   {
      OS_FREE(tblPar->portInfo);

      /* The table now has 0 entries & no memory */
      tblPar->portInfo = NULL;
      tblPar->actualTableSize = 0;
   }

   /* Update the table status if all went well */
   if (status == ZL303XX_OK)
   {
      tblPar->status = (ZL303XX_GPA_TBL_STATUS_CREATED - 1);
   }

   return status;
}  /* END zl303xx_GenericPortAddrTblDestroy */

/*

  Function Name:
    zl303xx_GenericPortAddrTblClearEntries

  Details:
   Clears all of the portInfo entries in a Generic Port Address Query Table
   but does not free its allocated memory

  Parameters:
   [in]    tblPar   Pointer to the Generic Port Address Query Table structure from
               which to delete the portInfo entries

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_GenericPortAddrTblClearEntries(zl303xx_GenericPortAddrTblS *tblPar)
{
   zlStatusE status = ZL303XX_OK;
   Uint16T tblIndex = 0;

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tblPar);
   }

   /* Make sure the table exists before we clear it */
   if (status == ZL303XX_OK)
   {
      if (tblPar->status != ZL303XX_GPA_TBL_STATUS_CREATED)
      {
         /* Technically, if status = UNINIT or INIT then the table is already
            cleared. However, report an error and let the user handle it. */
         status = ZL303XX_INVALID_MODE;
      }
      else if ((tblPar->portInfo == NULL) || (tblPar->maxTableSize == 0))
      {
         /* The maxTableSize & portInfo pointer are not consistent with the
            CREATED table state. */
         status = ZL303XX_INIT_ERROR;
      }
   }

   /* Clear the portInfo Entries and reset the actual table size */
   if (status == ZL303XX_OK)
   {
      /* Initialize each of the portInfo Entries to some default*/
      for (tblIndex = 0; tblIndex < tblPar->maxTableSize; tblIndex++)
      {
         tblPar->portInfo[tblIndex] = emptyRow;
      }

      /* The number of entries in the table is now 0 */
      tblPar->actualTableSize = 0;
   }

   /* The table state is still ZL303XX_GPA_TBL_STATUS_CREATED, just empty */

   return status;
}  /* END zl303xx_GenericPortAddrTblClearEntries */

/*

  Function Name:
    zl303xx_GenericPortAddrTblEntryExists

  Details:
   Determines if a particular portInfo entry exists in the given Generic Port
   Address Query Table & returns a pointer to it

  Parameters:
   [in]    tblPar   Pointer to the Generic Port Address Query Table in which to look
               for the particular zl303xx_GenericPortInfoS entry
   [in]    gpaPar   Pointer to the zl303xx_GenericPortInfoS entry to look for
   [in]    present  Pointer to the zl303xx_GenericPortInfoS entry in the Generic Port
               Address Query Table (NULL if not found)
  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_GenericPortAddrTblEntryExists(zl303xx_GenericPortAddrTblS *tblPar,
                                             zl303xx_GenericPortInfoS *gpaPar,
                                             zl303xx_GenericPortInfoS **present)
{
   zlStatusE status = ZL303XX_OK;

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tblPar) |
               ZL303XX_CHECK_POINTER(gpaPar);

      /* The *present pointer SHOULD be NULL */
      if (status == ZL303XX_OK)
      {
         if (*present != NULL)
         {
            status = ZL303XX_PARAMETER_INVALID;
         }
      }
   }

   /* The table must be in CREATED state with a non-NULL portInfo pointer */
   if (status == ZL303XX_OK)
   {
      if (tblPar->status != ZL303XX_GPA_TBL_STATUS_CREATED)
      {
         /* Technically, if status = UNINIT or INIT then the entry is guaranteed
            not to exist so we could return *present = NULL. However, since the
            user seems to expect entries in the table, report an error and let
            it decide what to do next. */
         status = ZL303XX_INVALID_MODE;
      }
      else if ((tblPar->portInfo == NULL) || (tblPar->maxTableSize == 0))
      {
         /* The maxTableSize or portInfo pointer are not consistent with the
            CREATED table state. */
         status = ZL303XX_INIT_ERROR;
      }
   }

   /* Find the requested entry in the table. We are only interested in the
      actual portAddress member (protocol, addrLen & address) since the user may
      be updating another attribute for it (ex: priority, etc.) */
   if (status == ZL303XX_OK)
   {
      Uint16T tblIndex;
      zl303xx_GenericPortInfoS *entry = tblPar->portInfo;

      for (tblIndex = 0; tblIndex < tblPar->maxTableSize; tblIndex++, entry++)
      {
         if (zl303xx_PtpComparePortAddress(&entry->portAddress, &gpaPar->portAddress) == 0)
         {
            *present = entry;
            break;
         }
      }
   }

   /* No change in table state is required */

   return status;
}  /* END zl303xx_GenericPortAddrTblEntryExists */

/*

  Function Name:
    zl303xx_GenericPortAddrTblEntryIndex

  Details:
   Gets the Port Address Query Table entry specified by Index. It counts only
   non-empty entries so that passing an index value of 0 will return the first
   entry regardless of where it is in table. If the index value is too big for
   the actual table size, a general error is returned.

  Parameters:
   [in]    tblPar      Pointer to the Generic Port Address Query Table in which to
                  look for the i-th entry
   [in]    tblIndex    Index of the row to return

   [out]   gpaPar   The info at the i-th row of the table (if status == ZL303XX_OK)

  Return Value:
   zlStatusE

 Remarks
   Index starts at 0 and goes up to 1 less than the maximum table size.

*******************************************************************************/

zlStatusE zl303xx_GenericPortAddrTblEntryIndex(zl303xx_GenericPortAddrTblS *tblPar,
                                               zl303xx_GenericPortInfoS *gpaPar,
                                               Uint16T tblIndex)
{
   zlStatusE status = ZL303XX_OK;

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tblPar) |
               ZL303XX_CHECK_POINTER(gpaPar);
   }

   /* If the index is larger than the table size, return an error */
   if (status == ZL303XX_OK)
   {
      if (tblIndex >= tblPar->actualTableSize)
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if ((tblPar->portInfo == NULL) || (tblPar->maxTableSize == 0))
   {
      /* The maxTableSize or portInfo pointer are not consistent with the
         CREATED table state. */
      status = ZL303XX_INIT_ERROR;
   }

   /* Find the requested entry in the table.  */
   if (status == ZL303XX_OK)
   {
      Uint16T row, count = 0;
      zl303xx_GenericPortInfoS *entry = tblPar->portInfo;

      for (row = 0; row < tblPar->maxTableSize; row++, entry++)
      {
         if (entry->empty == ZL303XX_FALSE)
         {
            if (++count > tblIndex)
            {
               /* We have found the i-th entry */
               OS_MEMCPY(gpaPar, entry, sizeof(zl303xx_GenericPortInfoS));
               break;
            }
         }
      }
   }

   /* No change in table state is required */

   return status;
}  /* END zl303xx_GenericPortAddrTblEntryIndex */

/*

  Function Name:
    zl303xx_GenericPortAddrTblAddEntry

  Details:
   Adds a portInfo entry to the given Generic Port Address Query Table if it
   does not already exist

  Parameters:
   [in]    tblPar   Pointer to the Generic Port Address Query Table to which the
               zl303xx_GenericPortInfoS entry will be added
   [in]    gpaPar   Pointer to the zl303xx_GenericPortInfoS entry to add

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_GenericPortAddrTblAddEntry(zl303xx_GenericPortAddrTblS *tblPar,
                                             zl303xx_GenericPortInfoS *gpaPar)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_GenericPortInfoS *present = NULL;

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tblPar) |
               ZL303XX_CHECK_POINTER(gpaPar);
   }

   /* The table must be in CREATED state with a non-NULL portInfo pointer */
   if (status == ZL303XX_OK)
   {
      if (tblPar->status != ZL303XX_GPA_TBL_STATUS_CREATED)
      {
         /* No space has been allocated for the table */
         status = ZL303XX_INVALID_MODE;
      }
      else if ((tblPar->portInfo == NULL) || (tblPar->maxTableSize == 0))
      {
         /* The maxTableSize or portInfo pointer are not consistent with the
            CREATED table state. */
         status = ZL303XX_INIT_ERROR;
      }
   }

   /*  Make sure the new entry does not already exist */
   if (status == ZL303XX_OK)
   {
      present = NULL;
      status = zl303xx_GenericPortAddrTblEntryExists(tblPar, gpaPar, &present);

      /* Notify the caller if the entry already exists */
      if (status == ZL303XX_OK)
      {
         if (present != NULL)
         {
            status = ZL303XX_TABLE_ENTRY_DUPLICATE;
         }
      }
   }

   /* Determine if there is still room to add to the table */
   if (status == ZL303XX_OK)
   {
      if (tblPar->actualTableSize >= tblPar->maxTableSize)
      {
         status = ZL303XX_TABLE_FULL;
      }
   }

   /* Find an empty entry in the table */
   if (status == ZL303XX_OK)
   {
      present = NULL;
      status = zl303xx_GenericPortAddrTblEntryExists(tblPar, &emptyRow, &present);
   }

   /* Since we verified earlier that there is space in the table, an empty
      row should have been found. If not, something is wrong. */
   if (status == ZL303XX_OK)
   {
      if (present == NULL)
      {
         status = ZL303XX_INIT_ERROR;
      }
      else  /* copy the new entry into the empty location and adjust tbl size */
      {
         *present = *gpaPar;
         present->empty = ZL303XX_FALSE;
         tblPar->actualTableSize++;
      }
   }

   /* No change in table state is required */

   return status;
}  /* END zl303xx_GenericPortAddrTblAddEntry */

/*

  Function Name:
    zl303xx_GenericPortAddrTblDelEntry

  Details:
   Removes a portInfo entry from the given Generic Port Address Query Table
   if it exist

  Parameters:
   [in]    tblPar   Pointer to the Generic Port Address Query Table from which the
               zl303xx_GenericPortInfoS entry will be removed
   [in]    gpaPar   Pointer to the zl303xx_GenericPortInfoS entry to remove

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_GenericPortAddrTblDelEntry(zl303xx_GenericPortAddrTblS *tblPar,
                                             zl303xx_GenericPortInfoS *gpaPar)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_GenericPortInfoS *present = NULL;

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tblPar) |
               ZL303XX_CHECK_POINTER(gpaPar);
   }

   /* The table must be in CREATED state with a non-NULL portInfo pointer */
   if (status == ZL303XX_OK)
   {
      if (tblPar->status != ZL303XX_GPA_TBL_STATUS_CREATED)
      {
         /* No space has been allocated for the table */
         status = ZL303XX_INVALID_MODE;
      }
      else if ((tblPar->portInfo == NULL) || (tblPar->maxTableSize == 0))
      {
         /* The maxTableSize or portInfo pointer are not consistent with the
            CREATED table state. */
         status = ZL303XX_INIT_ERROR;
      }
   }

   /* Find the requested zl303xx_GenericPortInfoS entry if it exists */
   if (status == ZL303XX_OK)
   {
      present = NULL;
      status = zl303xx_GenericPortAddrTblEntryExists(tblPar, gpaPar, &present);
   }

   /* Not finding the entry does not constitute an error since, technically,
      it is already removed (which was the user intent). */
   if (status == ZL303XX_OK)
   {
      if (present != NULL)
      {
         /* remove the entry and adjust the tbl size */
         *present = emptyRow;
         tblPar->actualTableSize--;
      }
   }

   /* No change in table state is required */

   return status;
}  /* END zl303xx_GenericPortAddrTblDelEntry */

/*

  Function Name:
    zl303xx_GenericPortAddrTblPpsGet

  Details:
   Converts the logQueryInterval member of a Generic Port Address Query Table to
   a corresponding Packet/Second value

  Parameters:
   [in]    tblPar   Pointer to the Generic Port Address Query Table
   [in]    pps      Pointer to a Pkt/Sec value converted from logQueryInterval

  Return Value:
   zlStatusE

  Notes:
   The logQueryInterval member must be -ve or 0 (interval <= 1 sec) in order for
   the Packet/Second value to be greater than 1.

   This function passes all parameters to zl303xx_GenericPortAddrTblRateGet
   for appropriate error checking.

*******************************************************************************/

zlStatusE zl303xx_GenericPortAddrTblPpsGet(zl303xx_GenericPortAddrTblS *tblPar,
                                           Uint32T *pps)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_QueryRateS curRate;

   /* Pass everything along for parameter checking & calculation */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_GenericPortAddrTblRateGet(tblPar, &curRate);
   }

   /* If curRate type is SPP then logQueryInterval member must have been +ve.
      See REMARKS in function header */
   if (status == ZL303XX_OK)
   {
      if (curRate.type == ZL303XX_QUERY_RATE_TYPE_SPP)
      {
         /* +ve values produce fractional PPS rates */
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Return the Packet/Second Rate */
   if (status == ZL303XX_OK)
   {
      *pps = curRate.rate;
   }

   return status;
} /* END zl303xx_GenericPortAddrTblPpsGet */

/*

  Function Name:
    zl303xx_GenericPortAddrTblPpsSet

  Details:
   Sets the logQueryInterval member of a Generic Port Address Query Table by
   converting the supplied Packet/Second value to a log2 equivalent

  Parameters:
   [in]    tblPar   Pointer to the Generic Port Address Query Table
   [in]    pps      Pointer to the Pkt/Sec value to convert to logQueryInterval

  Return Value:
   zlStatusE

  Notes:
   If the supplied Packet/Second Rate is not an even log2 value, then
   logQueryInterval will be set so that the Rate is the fastest possible that
   is LESS-THAN the Rate requested.
   The actual Packet/Second Rate set is passed back via the pps pointer.
   Ex:   requested PPS = 15 (0000 1111)
            actual PPS = 8  (0000 1000)
         logQueryInterval = -3

   This function passes all parameters to zl303xx_GenericPortAddrTblRateSet
   for appropriate error checking.

*******************************************************************************/

zlStatusE zl303xx_GenericPortAddrTblPpsSet(zl303xx_GenericPortAddrTblS *tblPar,
                                           Uint32T *pps)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_QueryRateS curRate = { ZL303XX_QUERY_RATE_TYPE_PPS, 0 };

   /* Only check the rate pointer since we need to access it first */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(pps);
   }

   /* Pass everything along for parameter checking & calculation */
   if (status == ZL303XX_OK)
   {
      curRate.rate = *pps;
      status =  zl303xx_GenericPortAddrTblRateSet(tblPar, &curRate);
   }

   /* Return the actual set rate */
   if (status == ZL303XX_OK)
   {
      *pps = curRate.rate;
   }

   return status;
} /* END zl303xx_GenericPortAddrTblPpsSet */

/*

  Function Name:
    zl303xx_GenericPortAddrTblSppGet

  Details:
   Converts the logQueryInterval member of a Generic Port Address Query Table to
   a corresponding Seconds/Packet value (ie. time between query packets)

  Parameters:
   [in]    tblPar   Pointer to the Generic Port Address Query Table
   [in]    spp      Pointer to a Sec/Pkt value converted from logQueryInterval

  Return Value:
   zlStatusE

  Notes:
   The logQueryInterval member must be a +ve or 0 (interval >= 1 sec) in order
   for the Seconds/Packet value to be greater than 1.

   This function passes all parameters to zl303xx_GenericPortAddrTblRateGet
   for appropriate error checking.

*******************************************************************************/

zlStatusE zl303xx_GenericPortAddrTblSppGet(zl303xx_GenericPortAddrTblS *tblPar,
                                           Uint32T *spp)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_QueryRateS curRate;

   /* Pass everything along for parameter checking & calculation */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_GenericPortAddrTblRateGet(tblPar, &curRate);
   }

   /* If curRate type is PPS then logQueryInterval member must have been -ve.
      See REMARKS in function header */
   if (status == ZL303XX_OK)
   {
      if (curRate.type == ZL303XX_QUERY_RATE_TYPE_PPS)
      {
         /* -ve values produce fractional SPP rates */
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Return the Second/Packet Rate */
   if (status == ZL303XX_OK)
   {
      *spp = curRate.rate;
   }

   return status;
} /* END zl303xx_GenericPortAddrTblSppGet */


/*

  Function Name:
    zl303xx_GenericPortAddrTblSppSet

  Details:
   Sets the logQueryInterval member of a port address query table by converting
   the corresponding Seconds/Packet value

  Parameters:
   [in]    tblPar   Pointer to the Generic Port Address Query Table
   [in]    spp      Pointer to the Secs/Pkt value to convert to logQueryInterval

  Return Value:
   zlStatusE

  Notes:
   If the passed in Seconds/Packet Interval is not an even log2 value, then
   logQueryInterval will be set so that the Interval is the largest possible
   that is GREATER-THAN the Interval requested
   The actual Seconds/Packet Interval set is passed back via the spp pointer.
   Ex:   requested SPP = 15 (0000 1111)
            actual SPP = 16 (0001 0000) (Thus, a slower pps).
         logQueryInterval = 4

   This function passes all parameters to zl303xx_GenericPortAddrTblRateSet
   for appropriate error checking.

*******************************************************************************/

zlStatusE zl303xx_GenericPortAddrTblSppSet(zl303xx_GenericPortAddrTblS *tblPar,
                                           Uint32T *spp)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_QueryRateS curRate = { ZL303XX_QUERY_RATE_TYPE_SPP, 0 };

   /* Only check the rate pointer since we need to access it first */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(spp);
   }

   /* Pass everything along for parameter checking & calculation */
   if (status == ZL303XX_OK)
   {
      curRate.rate = *spp;
      status =  zl303xx_GenericPortAddrTblRateSet(tblPar, &curRate);
   }

   /* Return the actual set rate */
   if (status == ZL303XX_OK)
   {
      *spp = curRate.rate;
   }

   return status;
} /* END zl303xx_GenericPortAddrTblSppSet */

/*

  Function Name:
    zl303xx_GenericPortAddrTblLogIntervalChk

  Details:
   Checks that the logQueryInterval values of the calling routine meet the
   limits defined by the Generic Port Address Query Table. Also does some other
   simple limit/range consistency checks.

  Parameters:
   [in]  tblPar   Pointer to the Generic Port Address Query Table containing the
                        logQueryInterval value to check

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_GenericPortAddrTblLogIntervalChk(zl303xx_GenericPortAddrTblS *tblPar)
{
   zlStatusE status = ZL303XX_OK;

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tblPar);
   }

   /* We can ignore table state since (perhaps) the calling routine is trying
      to work out limits, etc.*/

   /* Check the logQueryInterval range limits against the MAX/MIN supported
      by a Uint32T */
   if (status == ZL303XX_OK)
   {
      if ((tblPar->logQueryIntervalMax > GEN_TBL_LOG_QUERY_INTERVAL_MAX) ||
          (tblPar->logQueryIntervalMin < GEN_TBL_LOG_QUERY_INTERVAL_MIN))
      {
         /* This condition will produce and initialization error */
         ZL303XX_TRACE_ERROR(
            "zl303xx_GenericPortAddrTblLogIntervalChk: Invalid table initialization" \
            " intervalMax (%d) : MAX Allowed (%d)" \
            " intervalMin (%d) : MIN Allowed (%d)",
            tblPar->logQueryIntervalMax, GEN_TBL_LOG_QUERY_INTERVAL_MAX,
            tblPar->logQueryIntervalMin, GEN_TBL_LOG_QUERY_INTERVAL_MIN, 0,0);

         status = ZL303XX_INIT_ERROR;
      }
   }

   /* Make sure there is no funny business going on */
   if (status == ZL303XX_OK)
   {
      if (tblPar->logQueryIntervalMax < tblPar->logQueryIntervalMin)
      {
         ZL303XX_TRACE_ERROR(
            "zl303xx_GenericPortAddrTblLogIntervalChk: Invalid table query: MAX (%d) < MIN (%d)",
            tblPar->logQueryIntervalMax, tblPar->logQueryIntervalMin, 0,0,0,0);

         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Check the logQueryInterval against the users MAX/MIN limits */
   if (status == ZL303XX_OK)
   {
      if ((tblPar->logQueryInterval < tblPar->logQueryIntervalMin) ||
          (tblPar->logQueryInterval > tblPar->logQueryIntervalMax))
      {
         ZL303XX_TRACE_ERROR(
            "zl303xx_GenericPortAddrTblLogIntervalChk: Invalid table query: OOR" \
            " interval (%d) : intervalMax (%d) : intervalMin (%d)",
            tblPar->logQueryInterval,
            tblPar->logQueryIntervalMax,
            tblPar->logQueryIntervalMin, 0,0,0);

         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   return status;
} /* END zl303xx_GenericPortAddrTblLogIntervalChk */

/*

  Function Name:
    zl303xx_GenericPortAddrTblLogIntervalSet

  Details:
   Sets the logQueryInterval value of a port address query table in raw format

  Parameters:
   [in]    tblPar         Pointer to the Generic Port Address Query Table
   [in]    logInterval    The raw logQueryInterval value

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_GenericPortAddrTblLogIntervalSet(zl303xx_GenericPortAddrTblS *tblPar,
                                                   Sint8T logInterval)
{
   zlStatusE status = ZL303XX_OK;
   Sint32T originalLogInterval = 0;

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tblPar);
   }

   /* Check all logInterval limits (in case user has manipulated MAX/MIN) */
   if (status == ZL303XX_OK)
   {
      originalLogInterval = tblPar->logQueryInterval;
      tblPar->logQueryInterval = logInterval;

      status = zl303xx_GenericPortAddrTblLogIntervalChk(tblPar);
   }

   /* If an error has occured, revert back to the supplied value */
   if (status != ZL303XX_OK)
   {
      tblPar->logQueryInterval = originalLogInterval;
   }

   return status;
} /* END zl303xx_GenericPortAddrTblLogIntervalSet */


/****************   STATIC FUNCTIONS   ***************************************/

/*

  Function Name:
    zl303xx_GenericPortAddrTblRateGet

  Details:
   Converts the logQueryInterval member of a Generic Port Address Query Table to
   a corresponding Packet/Second or Seconds/Packet Rate

  Parameters:
   [in]    tblPar      Pointer to the Generic Port Address Query Table
   [in]    queryRate   Pointer to a QueryRate structure in which to return the
                  appropriate rate flag and value

  Return Value:
   zlStatusE

 *****************************************************************************/

zlStatusE zl303xx_GenericPortAddrTblRateGet(zl303xx_GenericPortAddrTblS *tblPar,
                                            zl303xx_QueryRateS *queryRate)
{
   zlStatusE status = ZL303XX_OK;

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tblPar) |
               ZL303XX_CHECK_POINTER(queryRate);
   }

   /* Table state must be at least INIT (since interval limits are needed) */
   if (status == ZL303XX_OK)
   {
      /* Table status range check. */
      if ((tblPar->status < ZL303XX_GPA_TBL_STATUS_INIT) ||
          (tblPar->status > ZL303XX_GPA_TBL_STATUS_MAX))
      {
         ZL303XX_TRACE_ERROR(
            "zl303xx_GenericPortAddrTblRateGet: Invalid table status (%lu).",
            tblPar->status, 0,0,0,0,0);
         status = ZL303XX_INVALID_MODE;
      }
   }

   /* Check the current logQueryInterval value against PPS limits to insure
      proper initialization */
   if (status == ZL303XX_OK)
   {
      if ((tblPar->logQueryInterval < tblPar->logQueryIntervalMin) ||
          (tblPar->logQueryInterval > tblPar->logQueryIntervalMax))
      {
         ZL303XX_TRACE_ERROR(
            "zl303xx_GenericPortAddrTblRateGet: Invalid logQueryInterval (%d).",
            tblPar->logQueryInterval, 0,0,0,0,0);
         status = ZL303XX_INIT_ERROR;
      }
   }

   /* Calculate the appropriate rate type and rate */
   if (status == ZL303XX_OK)
   {
      /* Set the rate type ... */
      if (tblPar->logQueryInterval == 0)
      {
         queryRate->type = ZL303XX_QUERY_RATE_TYPE_ZERO;
      }
      else if (tblPar->logQueryInterval > 0)
      {
         queryRate->type = ZL303XX_QUERY_RATE_TYPE_SPP;
      }
      else /* (tblPar->logQueryInterval is -ve) */
      {
         queryRate->type = ZL303XX_QUERY_RATE_TYPE_PPS;
      }

      /* ... and associated rate */
      queryRate->rate = (Uint32T)(1 << ABS_VAL(tblPar->logQueryInterval));
   }

   return status;
} /* END zl303xx_GenericPortAddrTblRateGet */

/*

  Function Name:
    zl303xx_GenericPortAddrTblRateSet

  Details:
   Converts a Packet/Second or Seconds/Packet Rate to a corresponding log2 value
   and sets the logQueryInterval member of a Generic Port Address Query Table

  Parameters:
       tblPar      Pointer to the Generic Port Address Query Table
       queryRate   Pointer to a QueryRate structure from which to determine the
                  appropriate logQueryInterval Rate

  Return Value:
   zlStatusE

 *****************************************************************************/

zlStatusE zl303xx_GenericPortAddrTblRateSet(zl303xx_GenericPortAddrTblS *tblPar,
                                            zl303xx_QueryRateS *queryRate)
{
   zlStatusE status = ZL303XX_OK;
   Sint8T log2Val = GEN_TBL_LOG_QUERY_INTERVAL_MAX;
   Uint32T derivedRate = (1 << log2Val);

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tblPar) |
               ZL303XX_CHECK_POINTER(queryRate);
   }

   /* Table state must be at least INIT (since interval limits are needed). */
   if (status == ZL303XX_OK)
   {
      /* Table status range check. */
      if ((tblPar->status < ZL303XX_GPA_TBL_STATUS_INIT) ||
          (tblPar->status > ZL303XX_GPA_TBL_STATUS_MAX))
      {
         ZL303XX_TRACE_ERROR(
            "zl303xx_GenericPortAddrTblRateSet: Invalid table status (%lu).",
            tblPar->status, 0,0,0,0,0);
         status = ZL303XX_INVALID_MODE;
      }
   }

   /* Check for a valid rate type */
   if (status == ZL303XX_OK)
   {
      if (queryRate->type > ZL303XX_QUERY_RATE_TYPE_MAX)
      {
         ZL303XX_TRACE_ERROR(
            "zl303xx_GenericPortAddrTblRateSet: Invalid query rate (%lu).",
            queryRate->type, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* There are a few of trouble possibilities... */
   if (status == ZL303XX_OK)
   {
      if ( (queryRate->rate == 0) ||  /* RATE = 0 is not possible */

           ( (queryRate->type == ZL303XX_QUERY_RATE_TYPE_PPS) &&
             (tblPar->logQueryIntervalMin > 0) ) ||
            /* Table has a +ve minInterval so no whole pps rate is possible */

           ( (queryRate->type == ZL303XX_QUERY_RATE_TYPE_SPP) &&
             (tblPar->logQueryIntervalMax < 0) )
            /* Table has a -ve maxInterval so no whole spp rate is possible */
          )
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Convert the supplied Rate to its best logQueryInterval equivalent */
   if (status == ZL303XX_OK)
   {
      /* This is a special case */
      if (queryRate->type == ZL303XX_QUERY_RATE_TYPE_ZERO)
      {
         derivedRate = 1;
         log2Val = 0;
      }

      /* for all other cases, Convert the supplied Rate to its best
         logQueryInterval equivalent */
      else
      {
         /* A (derivedRate > 0) check is not needed by the while loop below
            since we have previously verified that Rate contains at least 1
            valid bit. For the same reason, we will not check whether
            (derivedRate == 0) after the loop since that scenerio is also
            impossible */

         /* Search for the highest bit in the requested Rate */
         while ( !(queryRate->rate & derivedRate) )
         {
            derivedRate >>= 1;
            log2Val--;
         }

         /* Perhaps the supplied rate is not the same as this derived value (for
            instance, the suplied rate was not a whole log2 match). In this case
            we: - slow the PPS rate down to the next lowest match OR
                - increase the SPP interval to the next highest match */
         if (derivedRate != queryRate->rate)
         {
            /* If this is a PPS call, we have the highest bit so discarding the
               rest of the lower bits will reduce the rate as we want (so use
               derivedRate as is). We do need to flip the sign. */
            if (queryRate->type == ZL303XX_QUERY_RATE_TYPE_PPS)
            {
               log2Val *= (-1);
            }

            /* On the other hand, if this is a SPP we need to increase the
               interval so just shift the rate up 1. The following scenario will
               be covered in the final check:
                  log2Val = 31
                  derivedRate = 0x80000000;
               (since a left shift will produce derivedRate = 0 (an error!)) */
            else if (queryRate->type == ZL303XX_QUERY_RATE_TYPE_SPP)
            {
               derivedRate <<= 1;
               log2Val++;
            }
         }
      }

      /* We have found the best match for a 32-bit range but verify it is OK for
         the range supplied in the user table (and adjust accordingly) */
      if (log2Val < tblPar->logQueryIntervalMin)
      {
         /* A faster PPS rate than the application allows has been requested, so
            set the applications max PPS rate instead */
         log2Val = tblPar->logQueryIntervalMin;
      }
      else if (log2Val > tblPar->logQueryIntervalMax)
      {
         /* A longer SPP interval than the application allows has been requested,
            so set the applications max interval instead */
         log2Val = tblPar->logQueryIntervalMax;
      }

      /* Set the query interval... */
      tblPar->logQueryInterval = log2Val;

      /* ... and pass back the rate */
      queryRate->rate = (Uint32T)(1 << ABS_VAL(log2Val));
   }

   return status;
} /* END zl303xx_GenericPortAddrTblRateSet */

/*

  Function Name:
    zl303xx_V2PackPortAddrTlvFromIeee

  Details:
   Packs a Port Address Entry data structure (as defined by IEEE 1588) into TLV
   format specified by IEEE 1588 (Section 5.3.5).
      Enum16         netProtocol
      Uint16T        addrLength
      Uint8T[len]    netAddress

  Parameters:
       gpaPar   Pointer to the Port Address Entry data structure to copy
       buffer   Pointer to the area in which to copy the port address info
       len      Final length of the TLV formatted info

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_V2PackPortAddrTlvFromIeee(zl303xx_PortAddress *gpaPar,
                                            Uint8T *buffer, Uint16T *len)
{
   zlStatusE status = ZL303XX_OK;

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(gpaPar) |
               ZL303XX_CHECK_POINTER(buffer) |
               ZL303XX_CHECK_POINTER(len);
   }

   /* Set a default length & Check the Port Address length param */
   if (status == ZL303XX_OK)
   {
      /* Initialize the length to 0 in case we encounter an error before
         finishing. Compute it at the end of the copying */
      *len = 0;

      /* Do not trample the buffer (if Port Address has a bad address length) */
      if (gpaPar->addressLength > ZL303XX_PORT_ADDRESS_LENGTH_MAX)
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   /* Start copying the values into the buffer in network order */
   if (status == ZL303XX_OK)
   {
      /* networkProtocol... */
      PACK_16BIT(buffer, gpaPar->networkProtocol);
      buffer += sizeof(Uint16T);

      /* protocolAddressLength... */
      PACK_16BIT(buffer, gpaPar->addressLength);
      buffer += sizeof(Uint16T);

      /* protocolAddress... */
      OS_MEMCPY(buffer, gpaPar->addressField, gpaPar->addressLength);

      /* Compute the length of the TLV buffer */
      *len = (2 * sizeof(Uint16T)) + gpaPar->addressLength;
   }

   return status;
}  /* END zl303xx_V2PackPortAddrTlvFromIeee */

/*

  Function Name:
    zl303xx_V2PackPortAddrTlvFromGen

  Details:
   Packs the Port Address Entry portion of a Generic Port Address Entry data
   structure into TLV format specified by IEEE 1588 (Section 5.3.5).
      Enum16         netProtocol
      Uint16T        addrLength
      Uint8T[len]    netAddress

  Parameters:
       gpaPar   Pointer to the Generic Port Address Entry data structure
       buffer   Pointer to the area in which to copy the port address info
       len      Final length of the TLV formatted info

  Return Value:
   zlStatusE

  Notes:
   Passes all parameters and the portAddress portion of the Generic Port
   Address data type on to zl303xx_V2PackPortAddrTlvFromIeee for verification
   and processing.

*******************************************************************************/

zlStatusE zl303xx_V2PackPortAddrTlvFromGen(zl303xx_GenericPortInfoS *gpaPar,
                                           Uint8T *buffer, Uint16T *len)
{
   zlStatusE status = ZL303XX_OK;

   /* Only check the gpaPar pointer here since we reference it below. Other
      pointers will be verified in zl303xx_V2PackPortAddrTlvFromIeee */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(gpaPar);
   }

   /* Pack the Port Address member of the Generic Port Address data structure */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_V2PackPortAddrTlvFromIeee(&(gpaPar->portAddress),
                                                 buffer, len);
   }

   return status;
}  /* END zl303xx_V2PackPortAddrTlvFromGen */

/*

  Function Name:
    zl303xx_V2UnpackPortAddrTlvToIeee

  Details:
   Unpacks a Port Address Entry TLV (as defined by IEEE 1588) into a Port
   Address Entry data structure specified by IEEE 1588 (Section 5.3.5).
      Enum16         netProtocol
      Uint16T        addrLength
      Uint8T[len]    netAddress

  Parameters:
       gpaPar   Pointer to the Port Address Entry data structure
       buffer   Pointer to the IEEE port address info TLV
       len      Number of bytes processed from buffer

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_V2UnpackPortAddrTlvToIeee(zl303xx_PortAddress *gpaPar,
                                            Uint8T *buffer, Uint16T *len)
{
   zlStatusE status = ZL303XX_OK;

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(gpaPar) |
               ZL303XX_CHECK_POINTER(buffer) |
               ZL303XX_CHECK_POINTER(len);
   }

   /* Start copying the values into the gpaPar structure */
   if (status == ZL303XX_OK)
   {
      /* networkProtocol... */
      gpaPar->networkProtocol = UNPACK_16BIT(buffer);
      buffer += sizeof(Uint16T);

      /* protocolAddressLength... */
      gpaPar->addressLength = UNPACK_16BIT(buffer);
      buffer += sizeof(Uint16T);

      /* Do not trample the protocolAddress buffer */
      if (gpaPar->addressLength > ZL303XX_PORT_ADDRESS_LENGTH_MAX)
      {
         *len = 0;
         status = ZL303XX_PARAMETER_INVALID;
      }
      else
      {
         /* protocolAddress... */
         OS_MEMCPY(gpaPar->addressField, buffer, gpaPar->addressLength);

         /* indicate how many bytes were processed (perhaps this is called from
            a loop that is processing a full table */
         *len = (2 * sizeof(Uint16T)) + gpaPar->addressLength;
      }
   }

   return status;
}  /* END zl303xx_V2UnpackPortAddrTlvToIeee */

/*

  Function Name:
    zl303xx_V2UnpackPortAddrTlvToGen

  Details:
   Unpacks a Port Address Entry TLV (as defined by IEEE 1588) into the Port
   Address Entry portion of a Generic Port Address Entry data structure
      Enum16         netProtocol
      Uint16T        addrLength
      Uint8T[len]    netAddress

  Parameters:
       gpaPar   Pointer to the Generic Port Address Entry data structure
       buffer   Pointer to the IEEE port address info TLV
       len      Number of bytes processed from buffer

  Return Value:
   zlStatusE

  Notes:
   Passes all parameters and the portAddress portion of the Generic Port
   Address data type on to zl303xx_V2UnpackPortAddrTlvToIeee for verification
   and processing.

*******************************************************************************/

zlStatusE zl303xx_V2UnpackPortAddrTlvToGen(zl303xx_GenericPortInfoS *gpaPar,
                                           Uint8T *buffer, Uint16T *len)
{
   zlStatusE status = ZL303XX_OK;

   /* Only check the gpaPar pointer here since we reference it below. Other
      pointers will be verified in zl303xx_V2UnpackPortAddrTlvToIeee */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(gpaPar);
   }

   /* Unpack the Port Address TLV into the Port Address member of the Generic
      Port Address data structure */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_V2UnpackPortAddrTlvToIeee(&(gpaPar->portAddress),
                                                 buffer, len);
   }

   /* Mark the row as filled */
   if (status == ZL303XX_OK)
   {
      gpaPar->empty = ZL303XX_FALSE;
   }

   return status;
}  /* END zl303xx_V2UnpackPortAddrTlvToGen */

/*

  Function Name:
    zl303xx_V2PackPortAddrTblTlvFromIeee

  Details:
   Packs a Port Address Query Table (as defined by IEEE 1588) into TLV format
   specified by IEEE 1588. This is needed for UNICAST_MASTER_TABLE GET messages.

  Parameters:
       tblPar   Pointer to the Port Address Table data structure
       buffer   Pointer to the area in which to copy the Port Address table
       len      Final length of the TLV formated info

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_V2PackPortAddrTblTlvFromIeee(zl303xx_PortAddressQueryTable *tblPar,
                                               Uint8T *buffer, Uint16T *len)
{
   zlStatusE status = ZL303XX_OK;
   Uint16T paLen, numEntries = 0;
   zl303xx_PortAddress *entry = NULL;

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tblPar) |
               ZL303XX_CHECK_POINTER(buffer) |
               ZL303XX_CHECK_POINTER(len);

      /* Check the zl303xx_PortAddress pointer */
      if (status == ZL303XX_OK)
      {
         status = ZL303XX_CHECK_POINTER(tblPar->portAddress);
      }
   }

   /* Pack the logQueryInterval & actualTableSize values in network order */
   if (status == ZL303XX_OK)
   {
      Uint16T tblIndex = 0;

      /* logQueryInterval... */
      *(Sint8T *)buffer = tblPar->logQueryInterval;
      buffer += sizeof(Sint8T);

      /* actualTableSize... */
      PACK_16BIT(buffer, tblPar->logQueryInterval);
      buffer += sizeof(Uint16T);

      /* set the length before adding each of the portAddress entries */
      *len = sizeof(Sint8T) + sizeof(Uint16T);

      /* Set the Port Address Table location */
      entry = tblPar->portAddress;

      /* There may be empty entries in the table so only pack the filled ones */
      for (tblIndex = 0; tblIndex < tblPar->maxTableSize; tblIndex++, entry++)
      {
         if (status == ZL303XX_OK)
         {
            /* Assume an empty entry has networkProtocol = 0 */
            if (entry->networkProtocol != 0)
            {
               /* Count num entries found and check against actualTableSize */
               numEntries++;
               if (numEntries > tblPar->actualTableSize)
               {
                  /* The table is somehow corrupted or we haven't flagged empty
                     entries properly */
                  status = ZL303XX_IO_ERROR;
                  ZL303XX_TRACE_ALWAYS("zl303xx_V2PackPortAddrTblTlvFromIeee() too many entries; status = %lu", (Uint32T)status, 0,0,0,0,0);
               }
               else
               {
                  /* Pack this port address entry at the end of the buffer */
                  paLen = 0;
                  status = zl303xx_V2PackPortAddrTlvFromIeee(entry, buffer,
                                                             &paLen);

                  /* Adding portAddress Len has no affect if status != OK */
                  *len += paLen;
                  buffer += paLen;
               }
            }
         }
      }
   }

   /* Make sure we found actualTableSize entries */
   if (status == ZL303XX_OK)
   {
      if (numEntries != tblPar->actualTableSize)
      {
         /* We found less entries than we anticipated. */
         status = ZL303XX_IO_ERROR;
         ZL303XX_TRACE_ALWAYS("zl303xx_V2PackPortAddrTblTlvFromIeee() too few entries; status = %lu", (Uint32T)status, 0,0,0,0,0);
      }
   }

   /* Assign the TLV Length */
   if (status == ZL303XX_OK)
   {
      /* Odd sized port address table TLVs must be padded with 0s until even
         (IEEE 1588 17.5.3.4) */
      if ((*len % 2) == 1)
      {
         *buffer = (Uint8T)(0);
         *len += 1;
      }
   }

   return status;
}  /* END zl303xx_V2PackPortAddrTblTlvFromIeee */

/*

  Function Name:
    zl303xx_V2PackPortAddrTblTlvFromGen

  Details:
   Packs a Generic Port Address Query Table into TLV format specified by
   IEEE 1588. This is needed for UNICAST_MASTER_TABLE GET messages.

  Parameters:
       tblPar   Pointer to the Generic Port Address Table data structure
       buffer   Pointer to the area in which to copy the Port Address table
       len      Final length of the TLV formated info

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_V2PackPortAddrTblTlvFromGen(zl303xx_GenericPortAddrTblS *tblPar,
                                              Uint8T *buffer, Uint16T *len)
{
   zlStatusE status = ZL303XX_OK;
   Uint16T paLen, numEntries = 0;
   zl303xx_GenericPortInfoS *entry = NULL;

   /* Check the parameter pointers */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tblPar) |
               ZL303XX_CHECK_POINTER(buffer) |
               ZL303XX_CHECK_POINTER(len);

      /* Check the zl303xx_PortAddress pointer */
      if (status == ZL303XX_OK)
      {
         status = ZL303XX_CHECK_POINTER(tblPar->portInfo);
      }
   }

   /* Pack the logQueryInterval & actualTableSize values in network order */
   if (status == ZL303XX_OK)
   {
      Uint16T tblIndex = 0;

      /* logQueryInterval... */
      *(Sint8T *)buffer = tblPar->logQueryInterval;
      buffer += sizeof(Sint8T);

      /* actualTableSize... */
      PACK_16BIT(buffer, tblPar->actualTableSize);
      buffer += sizeof(Uint16T);

      /* set the length before adding each of the portAddress entries */
      *len = sizeof(Sint8T) + sizeof(Uint16T);

      /* Set the Port Address Table location */
      entry = tblPar->portInfo;

      /* There may be empty entries in the table so only pack the filled ones */
      for (tblIndex = 0; tblIndex < tblPar->maxTableSize; tblIndex++, entry++)
      {
         if (status == ZL303XX_OK)
         {
            /* Look for used entries */
            if (entry->empty != ZL303XX_TRUE)
            {
               /* Count num entries found and check against actualTableSize */
               numEntries++;
               if (numEntries > tblPar->actualTableSize)
               {
                  /* The table is somehow corrupted */
                  status = ZL303XX_IO_ERROR;
                  ZL303XX_TRACE_ALWAYS("zl303xx_V2PackPortAddrTblTlvFromGen() too many entries; status = %lu", (Uint32T)status, 0,0,0,0,0);
               }
               else
               {
                  /* Pack this port address entry at the end of the buffer */
                  paLen = 0;
                  status = zl303xx_V2PackPortAddrTlvFromIeee(&(entry->portAddress),
                                                          buffer, &paLen);

                  /* Adding portAddress Len has no affect if status != OK */
                  *len += paLen;
                  buffer += paLen;
               }
            }
         }
      }
   }

   /* Make sure we found actualTableSize entries */
   if (status == ZL303XX_OK)
   {
      if (numEntries != tblPar->actualTableSize)
      {
         /* We found less entries than we anticipated. */
         status = ZL303XX_IO_ERROR;
         ZL303XX_TRACE_ALWAYS("zl303xx_V2PackPortAddrTblTlvFromGen() too few entries; status = %lu", (Uint32T)status, 0,0,0,0,0);
      }
   }

   /* Assign the TLV Length */
   if (status == ZL303XX_OK)
   {
      /* Odd sized port address table TLVs must be padded with 0s until even
         (IEEE 1588 17.5.3.4) */
      if ((*len % 2) == 1)
      {
         *buffer = (Uint8T)(0);
         *len += 1;
      }
   }

   return status;
}  /* END zl303xx_V2PackPortAddrTblTlvFromGen */

