

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Generic PTP table implementation.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Os.h"
#include "zl303xx_Error.h"
#include "zl303xx_PtpTable.h"

#include <string.h>

/*****************   DEFINES     **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   INTERNAL PTP FUNCTION DEFINITIONS   **********************/

/* zl303xx_PtpTableCreate */
/** 

   Initializes a generic table structure and allocates memory for the array
   entries.

  Parameters:
   [in]   maxTableSize  The number of array entries to allocate.
   [in]   entrySize     Size of a single table entry, in bytes. e.g.,
                             sizeof(someStructS).
   [in]   ops           Table operations structure. keyMatch() must be defined.
   [out]  tbl           The initialized table structure.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   ops or ops->keyMatch is NULL.
     ZL303XX_RTOS_MEMORY_FAIL  Failed to allocate memory for table entries.

*******************************************************************************/
zlStatusE zl303xx_PtpTableCreate(Uint16T maxTableSize, Uint32T entrySize,
                                 const zl303xx_PtpTableOpsS *ops,
                                 zl303xx_PtpTableS *tbl)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      if (ops == NULL || ops->keyMatch == NULL)
      {
         status = ZL303XX_INVALID_POINTER;
      }
   }

   if (status == ZL303XX_OK)
   {
      tbl->array = (void*)OS_CALLOC(maxTableSize, entrySize);
      if (tbl->array == NULL)
      {
         status = ZL303XX_RTOS_MEMORY_FAIL;
      }
   }

   if (status == ZL303XX_OK)
   {
      tbl->maxTableSize = maxTableSize;
      tbl->actualTableSize = 0;
      tbl->entrySize = entrySize;
      tbl->ops = ops;
   }

   return status;
}

/* zl303xx_PtpTableDestroy */
/** 

   Frees memory used by a table.

  Parameters:
   [in,out]  tbl  Pointer to the table to destroy.

*******************************************************************************/
void zl303xx_PtpTableDestroy(zl303xx_PtpTableS *tbl)
{
   OS_FREE(tbl->array);
   tbl->array = NULL;
}

/* zl303xx_PtpTableAdd */
/** 

   Adds an entry to a table. The the entry does not exist and a function is
   defined for zl303xx_PtpTableOpsS::init(), the init() function will be called
   with the key data. If an entry matching the key data already exists, it will
   be returned.

  Parameters:
   [in,out]  tbl  The table to add the entry to.
   [in]      key  Data that will be used to search for an existing entry.

  Return Value:
     (void *)  Pointer to the new/existing entry structure.
     NULL      The table is full.

*******************************************************************************/
void *zl303xx_PtpTableAdd(zl303xx_PtpTableS *tbl, const void *key)
{
   void *entry;

   entry = zl303xx_PtpTableFind(tbl, key);
   if (entry != NULL)
   {
      return entry;
   }

   if (tbl->actualTableSize == tbl->maxTableSize)
   {
      return NULL;
   }

   /* Get a pointer to the first free entry and initialize it */
   entry = (char *)tbl->array + tbl->entrySize * tbl->actualTableSize;
   if (tbl->ops->init != NULL)
   {
      tbl->ops->init(key, entry);
   }

   tbl->actualTableSize++;
   return entry;
}

/* zl303xx_PtpTableRemove */
/** 

   Removes an entry matching the key data from a table. If no matching entry is
   found, no action is taken.

  Parameters:
   [in,out]  tbl  The table to remove an entry from.
   [in]      key  Data that will be used to search for an existing entry.

*******************************************************************************/
void zl303xx_PtpTableRemove(zl303xx_PtpTableS *tbl, const void *key)
{
   char *entry;  /* Use (char *) to ensure 1-byte pointer math */

   entry = zl303xx_PtpTableFind(tbl, key);
   if (entry == NULL)
   {
      return;
   }

   tbl->actualTableSize--;

   /* Compress tbl->array, so there are no holes in it */
   memmove(entry, entry + tbl->entrySize,
           tbl->actualTableSize * tbl->entrySize - (entry - (char *)tbl->array));
}

/* zl303xx_PtpTableRemoveAll */
/** 

   Removes all entries from a table.

  Parameters:
   [in,out]  tbl  The table to clear.

*******************************************************************************/
void zl303xx_PtpTableRemoveAll(zl303xx_PtpTableS *tbl)
{
   tbl->actualTableSize = 0;
}

/* zl303xx_PtpTableFind */
/** 

   Finds an entry matching some key data.

  Parameters:
   [in]   tbl  The table to search.
   [in]   key  Data that will be used to search for an existing entry.

  Return Value:
     (void *)  Pointer to the matching entry structure.
     NULL      No matching entry was found.

*******************************************************************************/
void *zl303xx_PtpTableFind(zl303xx_PtpTableS *tbl, const void *key)
{
   char *entry;  /* Use (char *) to ensure 1-byte pointer math */
   Uint32T count;

   for (count = 0, entry = tbl->array;
        count < tbl->actualTableSize;
        count++, entry += tbl->entrySize)
   {
      if (tbl->ops->keyMatch(key, entry))
      {
         return entry;
      }
   }

   return NULL;
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/*****************   END   ****************************************************/
