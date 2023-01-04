

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Generic PTP table API. This table has the following properties:
*        - Fixed number of entries allocated at initialization
*        - Each entry must be uniquely identifiable using some key data
*        - Deleting an entry compresses the array, leaving no holes
*
*     Each table has the following operations, bound in using a
*     const zl303xx_PtpTableOpsS structure:
*        - init():     called when adding an entry that does not exist in the
*                        table (may be NULL).
*        - keyMatch(): called whenever the table is searched for an entry
*                        matching the key data (must not be NULL).
*
*     Only the key data is used to add, remove or find a table entry.
*
*******************************************************************************/

#ifndef ZL303XX_PTP_TABLE_H_
#define ZL303XX_PTP_TABLE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/
struct zl303xx_PtpTableOpsS;

typedef struct
{
   Uint16T maxTableSize;
   Uint16T actualTableSize;

   Uint32T entrySize;
   void *array;

   const struct zl303xx_PtpTableOpsS *ops;
} zl303xx_PtpTableS;

typedef struct zl303xx_PtpTableOpsS
{
   void (*init)(const void *key, void *entry);
   zl303xx_BooleanE (*keyMatch)(const void *key, const void *entry);
} zl303xx_PtpTableOpsS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   INTERNAL PTP FUNCTION DECLARATIONS   *********************/
zlStatusE zl303xx_PtpTableCreate(Uint16T maxTableSize, Uint32T entrySize,
                                 const zl303xx_PtpTableOpsS *ops,
                                 zl303xx_PtpTableS *tbl);
void zl303xx_PtpTableDestroy(zl303xx_PtpTableS *tbl);

void *zl303xx_PtpTableAdd(zl303xx_PtpTableS *tbl, const void *key);
void zl303xx_PtpTableRemove(zl303xx_PtpTableS *tbl, const void *key);
void zl303xx_PtpTableRemoveAll(zl303xx_PtpTableS *tbl);

void *zl303xx_PtpTableFind(zl303xx_PtpTableS *tbl, const void *key);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
