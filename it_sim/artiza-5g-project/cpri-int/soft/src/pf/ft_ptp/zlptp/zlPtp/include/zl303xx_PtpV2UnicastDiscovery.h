

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Header file for the Unicast Discovery functionality.
*
******************************************************************************/

#ifndef _ZL303XX_PTP_UNICAST_DISCOVERY_H_
#define _ZL303XX_PTP_UNICAST_DISCOVERY_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpPortAddrQueryTbl.h"
#include "zl303xx_PtpG8265p1Types.h"

/*****************   DEFINES   ************************************************/

/* Table to store information about the Unicast Master streams (UMT).
   This type is defined in the IEEE 1588 standard */
#define UNICAST_DISC_TABLE_MAX_LENGTH     3

/* The default Unicast Discovery Query Interval is 4 seconds */
#define UNICAST_DISC_INTERVAL_DEFAULT     2   /* 2^2 secs between pkts   */

/* Define query interval limits */
#define UNICAST_DISC_INTERVAL_MAX   (Sint8T)(10)   /* = 1024 pkt/sec */
#define UNICAST_DISC_SPP_MAX        (Uint32T)(1 << UNICAST_DISC_INTERVAL_MAX)

#define UNICAST_DISC_INTERVAL_MIN   (Sint8T)(-10)
#define UNICAST_DISC_PPS_MAX     (Uint32T)(1 << OS_ABS(UNICAST_DISC_INTERVAL_MIN))


/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/* Internal unicast master entry structure */
typedef struct
{
   zl303xx_BooleanE inUse;

   /* Port address of this master. */
   zl303xx_PortAddress portAddress;

   /* A handle to the stream data for the connection to this master. */
   Uint32T streamHandle;

   /* Configuration parameters used to add this UMT entry. */
   zl303xx_PtpUmtEntryConfigS  entryConfig;
   zl303xx_PtpTelecomUmtEntryS telecomConfig;

   /* Telecom Profile-specific members. */
   zl303xx_PtpTelecomUmtEntryIntS telecom;
} zl303xx_PtpUmtEntryIntS;

/* Internal Unicast Master Table structure */
typedef struct
{
   Uint32T portHandle;

   Uint16T maxTableSize;
   Uint16T actualTableSize;

   /* Different applications may set its own packet rates & intervals */
   Sint8T logQueryInterval;

   /* Table of far-end master ports. */
   zl303xx_PtpUmtEntryIntS *master;
} zl303xx_PtpUmtS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/*****************   INTERNAL PTP FUNCTION DECLARATIONS   *********************/
void      zl303xx_PtpUmtStructInit(zl303xx_PtpUmtS *pUmt);
zlStatusE zl303xx_PtpUmtCreate(zl303xx_PtpUmtS *pUmt, Uint16T size);
void      zl303xx_PtpUmtDelete(zl303xx_PtpUmtS *pUmt);
zlStatusE zl303xx_PtpUmtCheck(zl303xx_PtpUmtS *pUmt);
zlStatusE zl303xx_PtpUmtStartAllStreams(zl303xx_PtpUmtS *pUmt);

zlStatusE zl303xx_PtpUmtTlvPack(zl303xx_PtpUmtS *pUmt, Uint8T *buffer, Uint16T *pBufferLen);
zlStatusE zl303xx_PtpUmtTlvUnpack(Uint8T *buffer, Uint16T bufferLen, zl303xx_PtpUmtS *pUmt);
zlStatusE zl303xx_PtpUmtReplace(zl303xx_PtpUmtS *pCurrUmt, zl303xx_PtpUmtS *pNewUmt);
void      zl303xx_PtpUmtFree(zl303xx_PtpUmtS *pUmt);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
