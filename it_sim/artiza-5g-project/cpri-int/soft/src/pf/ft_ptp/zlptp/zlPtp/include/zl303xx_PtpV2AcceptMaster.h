

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Header file for the Acceptable Master functionality.
*
******************************************************************************/

#ifndef _ZL303XX_PTP_V2_ACCEPT_MASTER_H_
#define _ZL303XX_PTP_V2_ACCEPT_MASTER_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpTable.h"

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/
/* Acceptable Master Table entry */
typedef struct
{
   zl303xx_PortAddress addr;
   Uint8T alternatePriority1;
   zl303xx_PtpStreamHandleT streamHandle;
} zl303xx_PtpAccMasterEntryS, zl303xx_PtpAccSlaveEntryS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   INTERNAL PTP FUNCTION DECLARATIONS   *********************/
zlStatusE zl303xx_PtpAccMasterTblCreate(Uint16T maxEntries, zl303xx_PtpTableS *amt);
void zl303xx_PtpAccMasterTblDelete(zl303xx_PtpTableS *amt);

zlStatusE zl303xx_PtpAccSlaveTblCreate(Uint16T maxEntries, zl303xx_PtpTableS *ast);
void zl303xx_PtpAccSlaveTblDelete(zl303xx_PtpTableS *ast);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
