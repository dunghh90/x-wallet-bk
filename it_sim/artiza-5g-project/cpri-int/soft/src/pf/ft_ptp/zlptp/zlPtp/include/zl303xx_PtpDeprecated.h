

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains deprecated PTP data types, structures, and function
*     declarations. These functions are no longer supported and may be removed
*     in the future.
*
*******************************************************************************/

#ifndef _ZL303XX_PTP_DEPRECATED_H_
#define _ZL303XX_PTP_DEPRECATED_H_

#ifdef __cplusplus
extern "C" {
#endif


/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"

#include "zl303xx_PtpConstants.h"
#include "zl303xx_PtpConstants_dep.h"
#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpPortAddrQueryTbl.h"

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/
typedef Uint32T clockHandleT;
typedef Uint32T clockPortHandleT;
typedef Uint32T clockStreamHandleT;

/* The Unicast Discovery Table is of zl303xx_GenericPortAddrTblS type */
typedef zl303xx_GenericPortAddrTblS   zl303xx_UniDiscoveryTblS;

/* Allows customer code to keep using the zl303xx_PtpTelecomPtsfE enum type.
 * zl303xx_PtsfFlagE should be used instead. */
typedef Sint32T zl303xx_PtpTelecomPtsfE;

/*****************   DATA STRUCTURES   ****************************************/
/* The Unicast Discovery data type is a subset of the Generic Type */
typedef struct
{
   /* A 4-byte handle to the stream associated with this master node */
   /* For internal use */
   Uint32T portAddressHandle;

   /* The portAddress member is needed for IEEE 1588 */
   /* The protocolAddress member is stored in NETWORK order */
   zl303xx_PortAddress portAddress;
} zl303xx_MasterAddressS;

/**  (Deprecated) The ZL303XX_PTP_EVENT_SERVO_STATUS_GET is no longer generated.
 *  PTSF changes must now be set directly from inside servo code. */
typedef struct zl303xx_PtpEventServoStatusGetS
{
   struct
   {
      /** Handle to a previously created stream. */
      zl303xx_PtpStreamHandleT streamHandle;
      /** Pointer to the stream's external data. */
      void *streamExtData;
   } in;

   struct
   {
      /** Return ZL303XX_TRUE if the time stamp pairs being sent to the algorithm can
       *  be used to recover a clock with acceptable performance. Otherwise, return
       *  ZL303XX_FALSE. */
      zl303xx_BooleanE valid;
   } out;
} zl303xx_PtpEventServoStatusGetS;

/**  (Deprecated) The ZL303XX_PTP_EVENT_PTSF_CHANGE is no longer generated. PTSF
 *  data is now provided in the zl303xx_PtsfEventS structure. */
typedef struct zl303xx_PtpEventPtsfChangeS
{
   /** Handle to the stream the flag changed on. */
   zl303xx_PtpStreamHandleT streamHandle;
   /** Pointer to the stream's external data. */
   void *streamExtData;

   /** The current value of all PTSF flags for this stream (i.e., after modFlag
    *  has changed). Not all flag values are mutually exclusive. */
   Sint32T currFlags;

   /** The flag that changed to cause this callout. */
   Sint32T modFlag;

   /** ZL303XX_TRUE if modFlag has been set. ZL303XX_FALSE if modFlag has been cleared. */
   zl303xx_BooleanE flagSet;
} zl303xx_PtpEventPtsfChangeS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/
/* Exported strings to be defined by the application. */
#define PTP_MANUFACTURER_ID_LENGTH           (Uint16T)64
#define PTP_REVISION_DATA_LENGTH             (Uint16T)32
#define PTP_USER_DESCRIPTION_LENGTH          (Uint16T)128

extern char PtpManufacturerID[PTP_MANUFACTURER_ID_LENGTH];
extern char PtpRevisionData[PTP_REVISION_DATA_LENGTH];
extern char PtpUserDescription[PTP_USER_DESCRIPTION_LENGTH];

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/
zlStatusE zl303xx_MasterAddressStructInit(zl303xx_MasterAddressS *maPar);

/*** UNICAST DISCOVERY FUNCTIONS ***/
zlStatusE zl303xx_PtpUniDiscoveryMasterExists(zl303xx_PtpPortHandleT portHandle,
                                              zl303xx_PortAddress *pMasterAddr,
                                              zl303xx_BooleanE *pExists);
zlStatusE zl303xx_PtpUniDiscoveryMasterAdd(zl303xx_PtpPortHandleT portHandle,
                                           zl303xx_PortAddress *pMasterAddr);

/*****************/
/* Unicast Discovery Table control functions */
zlStatusE zl303xx_UniDiscoveryTblClearAllEntries(zl303xx_UniDiscoveryTblS *udt);

/*****************/
/* Queries of the Unicast Discovery Table size attributes */
zlStatusE zl303xx_UniDiscoveryTblMaxSize(zl303xx_UniDiscoveryTblS *udt, Uint16T *maxSize);
zlStatusE zl303xx_UniDiscoveryTblActSize(zl303xx_UniDiscoveryTblS *udt, Uint16T *actSize);

/*****************/
/* Determine if a masterAddress entry exists in the Unicast Discovery Table */
zlStatusE zl303xx_UniDiscoveryMasterExists(zl303xx_UniDiscoveryTblS *udt,
                                           zl303xx_MasterAddressS *maPar,
                                           zl303xx_BooleanE *present);

/* Get the i-th, masterAddress entry in the Unicast Discovery Table
   (if it exists) */
zlStatusE zl303xx_UniDiscoveryMasterIndex(zl303xx_UniDiscoveryTblS *udt,
                                          zl303xx_MasterAddressS *maPar,
                                          Uint16T tblIndex);

/*****************/
/* Add/remove a masterAddress entry to/from a Unicast Discovery Table */
zlStatusE zl303xx_UniDiscoveryAddMaster(zl303xx_UniDiscoveryTblS *udt, zl303xx_MasterAddressS *maPar);
zlStatusE zl303xx_UniDiscoveryDelMaster(zl303xx_UniDiscoveryTblS *udt, zl303xx_MasterAddressS *maPar);

/* Updates info for the specified Master in the Unicast Discovery Table */
zlStatusE zl303xx_UniDiscoveryUpdateMaster(zl303xx_UniDiscoveryTblS *udt, zl303xx_MasterAddressS *maPar);

/*****************/
/* Functions to set/retrieve the logQueryInterval setting of a Unicast Discovery
   Table in Pkts/Sec or Secs/Pkt format. */

/* For set/getting logQueryInterval to a -ve value (Interval < 1 sec) */
zlStatusE zl303xx_UniDiscoveryPktPerSecGet(zl303xx_UniDiscoveryTblS *udt, Uint32T *pps);
zlStatusE zl303xx_UniDiscoveryPktPerSecSet(zl303xx_UniDiscoveryTblS *udt, Uint32T *pps);

/* For set/getting logQueryInterval to a +ve value (Interval > 1 sec) */
zlStatusE zl303xx_UniDiscoverySecPerPktGet(zl303xx_UniDiscoveryTblS *udt, Uint32T *spp);
zlStatusE zl303xx_UniDiscoverySecPerPktSet(zl303xx_UniDiscoveryTblS *udt, Uint32T *spp);

/* For checking/setting the raw logQueryInterval value */
zlStatusE zl303xx_UniDiscoveryLogIntervalCheck(zl303xx_UniDiscoveryTblS *udt);
zlStatusE zl303xx_UniDiscoveryLogIntervalSet(zl303xx_UniDiscoveryTblS *udt, Sint32T logInterval);

void zl303xx_PtpGetLocalUtcTime(zl303xx_PtpPortHandleT clockPortHandle, zl303xx_TimeStamp *utcTime);

/* Internal function for controlling the low level packet transmit */
zlStatusE zl303xx_EnableTsPacketTransmit(zl303xx_PtpStreamHandleT clockId, zl303xx_BooleanE enable);

/* Deprecated PTSF API Functions */
zlStatusE zl303xx_PtpUmtEntryPtsfMaskSet(zl303xx_PtpStreamHandleT streamHandle,
                                         Sint32T ptsfMask);
zlStatusE zl303xx_PtpUmtEntryPtsfUserSet(zl303xx_PtpStreamHandleT streamHandle,
                                         zl303xx_BooleanE setFlag);
zlStatusE zl303xx_PtpUmtEntryPtsfNotLockedSet(zl303xx_PtpStreamHandleT streamHandle,
                                              zl303xx_BooleanE setFlag);
const char *zl303xx_PtpTelecomPtsfToStr(Sint32T ptsf);

/*** Deprecated unicast negotiation functions ***/
zlStatusE zl303xx_PtpV2UniNegContractStart(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_MessageTypeE messageType);

zlStatusE zl303xx_PtpV2UniNegContractCancel(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_MessageTypeE messageType);

/*** Deprecated Clock Control Routine that did nothing. ***/
zlStatusE zl303xx_ForcePtpMode(zl303xx_PtpStreamHandleT clockId, zl303xx_PtpStreamModeE mode);

zlStatusE zl303xx_SetDelayReqPacketsPerSecond(zl303xx_PtpStreamHandleT streamHandle, Sint16T pktsPerSec);
zlStatusE zl303xx_SetDelayReqSecondsPerPacket(zl303xx_PtpStreamHandleT streamHandle, Sint16T secsPerPkt);
zlStatusE zl303xx_SetSyncPacketsPerSecond(zl303xx_PtpStreamHandleT streamHandle, Sint16T pktsPerSec);
zlStatusE zl303xx_SetSyncSecondsPerPacket(zl303xx_PtpStreamHandleT streamHandle, Sint16T secsPerPkt);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */

