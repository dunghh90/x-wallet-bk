

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Header file for the port address query table data structure. This data
*     structure is required for several of the 1588 features (including Unicast
*     Discovery & Master Clusters). The following definitions and functions
*     provide a one-stop-shop for any feature requiring such a table.
*
******************************************************************************/

#ifndef _ZL303XX_PTP_PORT_ADDR_QUERY_TBL_H_
#define _ZL303XX_PTP_PORT_ADDR_QUERY_TBL_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"

#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpStdTypes.h"

/*****************   DEFINES   ************************************************/

/* Define the query interval absolute limits as +/-31 since we only return
   and/or accept a Uint32T type for the packets/sec & sec/pkt functions */
#define GEN_TBL_LOG_QUERY_INTERVAL_MAX       (Sint8T)(31)
#define GEN_TBL_LOG_QUERY_INTERVAL_MIN       (Sint8T)(-31)

/* Define the query interval reasonable default limits as +/-10 (1024 pps/spp)
   for the Structure Init function. (user can change if required) */
#define GEN_TBL_LOG_QUERY_INTERVAL_DEF_MAX       (Sint8T)(10)
#define GEN_TBL_LOG_QUERY_INTERVAL_DEF_MIN       (Sint8T)(-10)
#define GEN_TBL_LOG_QUERY_INTERVAL_DEF   (Sint8T)(1) /* 2^1 secs per pkt  */

/* A couple of macros to make some of the math operations neater */
#define ABS_VAL(val)    (Uint32T)((val < 0) ? ((-1) * val) : val)


/*****************   DATA TYPES   *********************************************/

/* Port Address Table Status enum */
typedef enum
{
   ZL303XX_GPA_TBL_STATUS_UNINIT = 0,
   ZL303XX_GPA_TBL_STATUS_INIT = 1,
   ZL303XX_GPA_TBL_STATUS_CREATED = 2,

   /* define a couple of bounds values */
   ZL303XX_GPA_TBL_STATUS_INVALID,
   ZL303XX_GPA_TBL_STATUS_MAX = (ZL303XX_GPA_TBL_STATUS_INVALID - 1)
} zl303xx_GenericPaTblStatusE;

/*****************   DATA STRUCTURES   ****************************************/

/*******************/
/* Generic Port/Master address data structure (used as 1 row in the generic
   Port/Master address table implementation below) */
typedef struct
{
   zl303xx_BooleanE empty;

   /* This is a generic 4-byte place holder for a handle to the port */
   Uint32T portAddressHandle;

   /* Relative priority of the node represented by portAddress */
   Uint8T priority;

   /* Maintaining this type for the portAddress makes data handling transparent
      between generic and IEEE defined structures and function calls */
   /* The protocolAddress member is stored in NETWORK order */
   zl303xx_PortAddress portAddress;
} zl303xx_GenericPortInfoS;

/* Generic Port address table. Can be used for Unicast Discovery, Acceptable
   Master, Master Cluster, etc. Specific TLV calls satisfy IEEE 1588       */
typedef struct
{
   zl303xx_GenericPaTblStatusE status;
   Uint16T maxTableSize;
   Uint16T actualTableSize;

   /* Different applications may set its own packet rates & intervals */
   Sint8T logQueryInterval;
   Sint8T logQueryIntervalMax;
   Sint8T logQueryIntervalMin;

   /* Table of Port Information */
   zl303xx_GenericPortInfoS *portInfo;
} zl303xx_GenericPortAddrTblS;
/*******************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/*****************/
/* Generic Port Address Table Control functions */
zlStatusE zl303xx_GenericPortAddrTblStructInit(zl303xx_GenericPortAddrTblS *tblPar);
zlStatusE zl303xx_GenericPortAddrTblStructReInit(zl303xx_GenericPortAddrTblS *tblPar);
zlStatusE zl303xx_GenericPortAddrTblCreate(zl303xx_GenericPortAddrTblS *tblPar);
zlStatusE zl303xx_GenericPortAddrTblDestroy(zl303xx_GenericPortAddrTblS *tblPar);
zlStatusE zl303xx_GenericPortAddrTblClearEntries(zl303xx_GenericPortAddrTblS *tblPar);

/*****************/
/* Functions to add/remove a portAddress entry to/from a Port Address Query
   Table */
zlStatusE zl303xx_GenericPortAddrTblEntryExists(zl303xx_GenericPortAddrTblS *tblPar,
                                            zl303xx_GenericPortInfoS *paPar,
                                            zl303xx_GenericPortInfoS **present);

/* Get the i-th, entry in the Port Address Query Table (if it exists) */
zlStatusE zl303xx_GenericPortAddrTblEntryIndex(zl303xx_GenericPortAddrTblS *tblPar,
                                          zl303xx_GenericPortInfoS *gpaPar,
                                          Uint16T tblIndex);

zlStatusE zl303xx_GenericPortAddrTblAddEntry(zl303xx_GenericPortAddrTblS *tblPar,
                                             zl303xx_GenericPortInfoS *gpaPar);

zlStatusE zl303xx_GenericPortAddrTblDelEntry(zl303xx_GenericPortAddrTblS *tblPar,
                                             zl303xx_GenericPortInfoS *gpaPar);

/*****************/
/* Functions to set/retrieve the logQueryInterval setting of a Port Address
   Query Table in Pkts/Sec (Pps) or Secs/Pkt (Spp) format. */

/* For set/getting logQueryInterval to a -ve value (Interval < 1 sec) */
zlStatusE zl303xx_GenericPortAddrTblPpsGet(zl303xx_GenericPortAddrTblS *tblPar,
                                           Uint32T *pps);

zlStatusE zl303xx_GenericPortAddrTblPpsSet(zl303xx_GenericPortAddrTblS *tblPar,
                                           Uint32T *pps);

/* For set/getting logQueryInterval to a +ve value (Interval > 1 sec) */
zlStatusE zl303xx_GenericPortAddrTblSppGet(zl303xx_GenericPortAddrTblS *tblPar,
                                           Uint32T *spp);

zlStatusE zl303xx_GenericPortAddrTblSppSet(zl303xx_GenericPortAddrTblS *tblPar,
                                           Uint32T *spp);

/* For checking/setting the raw logQueryInterval value */
zlStatusE zl303xx_GenericPortAddrTblLogIntervalChk(zl303xx_GenericPortAddrTblS *tblPar);

zlStatusE zl303xx_GenericPortAddrTblLogIntervalSet(zl303xx_GenericPortAddrTblS *tblPar,
                                                   Sint8T logInterval);


/**** IEEE 1588 Type-Length-Value (TLV) Pack/Unpack functions ****/
/**** Port Address Structure Pack/Unpack ****/
zlStatusE zl303xx_V2PackPortAddrTlvFromIeee(zl303xx_PortAddress *gpaPar,
                                            Uint8T *buffer, Uint16T *len);

zlStatusE zl303xx_V2PackPortAddrTlvFromGen(zl303xx_GenericPortInfoS *gpaPar,
                                           Uint8T *buffer, Uint16T *len);

zlStatusE zl303xx_V2UnpackPortAddrTlvToIeee(zl303xx_PortAddress *gpaPar,
                                            Uint8T *buffer, Uint16T *len);

zlStatusE zl303xx_V2UnpackPortAddrTlvToGen(zl303xx_GenericPortInfoS *gpaPar,
                                           Uint8T *buffer, Uint16T *len);

/**** Port Address Table Pack/Unpack ****/
zlStatusE zl303xx_V2PackPortAddrTblTlvFromIeee(zl303xx_PortAddressQueryTable *tblPar,
                                               Uint8T *buffer, Uint16T *len);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
