

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Definitions and utility functions for parsing (packing/unpacking)
*     Management ID structures to/from buffers. These are covered under
*     Clause 15 of the IEEE-1588-2008 Standard with associated managementId
*     definitions listed in Table 40.
*
******************************************************************************/

#ifndef _ZL303XX_PTP_V2_MGMT_TLV_PARSE_H_
#define _ZL303XX_PTP_V2_MGMT_TLV_PARSE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"

#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpConstants.h"

/*****************   DEFINES   ************************************************/

/* Management TLV headers all have the same length:
   2 bytes for type (MGMT)
   2 bytes for length
   2 bytes for id (MGMT_ID) */
#define ZL303XX_PTP_V2_MGMT_TLV_HEADER_LEN              (Uint16T)6

/* Other Management TLV Buffer Lengths. */
#define ZL303XX_PTP_V2_MGMT_TLV_DEFAULT_DATA_SET_LEN             20
#define ZL303XX_PTP_V2_MGMT_TLV_PARENT_DATA_SET_LENGTH           32
#define ZL303XX_PTP_V2_MGMT_TLV_CURRENT_DATA_SET_LENGTH          18
#define ZL303XX_PTP_V2_MGMT_TLV_TIME_PROPERTIES_DATA_SET_LENGTH  4
#define ZL303XX_PTP_V2_MGMT_TLV_PORT_DATA_SET_LENGTH             26
#define ZL303XX_PTP_V2_MGMT_TLV_UTC_PROPERTIES_LEN               4
#define ZL303XX_PTP_V2_MGMT_TLV_SLAVE_ONLY_LEN                   2
#define ZL303XX_PTP_V2_MGMT_TLV_UNICAST_NEG_EN_LEN               2

/* Transparent Clock Definitions. */
#define ZL303XX_PTP_V2_MGMT_TLV_TRANS_CLOCK_DATA_SET_LENGTH      12
#define ZL303XX_PTP_V2_MGMT_TLV_TRANS_PORT_DATA_SET_LENGTH       20

/*****************   DATA TYPES   *********************************************/

typedef struct zl303xx_ClockDescriptionS
{
   /** Taken from the PTP Clock associated with the Port. */
   Uint16T clockType;

   /** The Physical Layer protocol string. */
   char physicalLayerProtocol[ZL303XX_PTP_NODE_PHYSICAL_PROTO_LEN];
   /** The length of the physical address string. */
   Uint16T physicalAddressLength;
   /** The physical address string. */
   Uint8T physicalAddress[ZL303XX_PTP_NODE_PHYSICAL_ADDR_LEN];

   /** The local address of this port. */
   zl303xx_PortAddress protocolAddress;

   /** Manufacturer's OUI. */
   Uint8T manufacturerIdentity[ZL303XX_PTP_NODE_OUI_LEN];

   char productDescription[ZL303XX_PTP_NODE_PROD_DESC_LEN];
   char revisionData[ZL303XX_PTP_NODE_REV_DATA_LEN];
   char userDescription[ZL303XX_PTP_NODE_USER_DESC_LEN];

   /** The profile identity indicating which profile is implemented by the port.
    *  It consists of OUI (3 octets) followed by an ID number (3 octets).  */
   char profileIdentity[PTP_PROFILE_IDENTITY_LENGTH];
} zl303xx_ClockDescriptionS;

typedef struct zl303xx_UtcProperties
{
   /** This member is ZL303XX_TRUE if zl303xx_TimePropertiesDS::currentUtcOffset is known to
    *  be correct. Otherwise, it is ZL303XX_FALSE. */
   zl303xx_BooleanE currentUtcOffsetValid;

   /** This member is ZL303XX_TRUE if the epoch is the PTP epoch, and the last minute
    *  of the current UTC day contains 59 seconds. Otherwise, it is ZL303XX_FALSE. */
   zl303xx_BooleanE leap59;

   /** This member is ZL303XX_TRUE if the epoch is the PTP epoch, and the last minute
    *  of the current UTC day contains 61 seconds. Otherwise, it is ZL303XX_FALSE. */
   zl303xx_BooleanE leap61;

   /** The offset between TAI and UTC. This values is only meaningful in PTP
    *  systems whose epoch is the PTP epoch. This value is in units of seconds. */
   Sint16T currentUtcOffset;
} zl303xx_UtcProperties;

typedef struct zl303xx_TraceabilityProperties
{
   /** This member is ZL303XX_TRUE if the timescale and the value of
    *  zl303xx_TimePropertiesDS::currentUtcOffset is traceable to a primary reference.
    *  Otherwise, it is ZL303XX_FALSE. */
   zl303xx_BooleanE timeTraceable;

   /** This member is ZL303XX_TRUE if the frequency determining the timescale is
    *  traceable to a primary reference. Otherwise, it is ZL303XX_FALSE. */
   zl303xx_BooleanE frequencyTraceable;
} zl303xx_TraceabilityProperties;

typedef struct zl303xx_TimescaleProperties
{
   /** This member is ZL303XX_TRUE if the clock timescale of the grandmaster clock
    *  is PTP. Otherwise, it is ZL303XX_FALSE. */
   zl303xx_BooleanE ptpTimescale;

   /** The source of time used by the grandmaster clock. */
   zl303xx_TimeSourceE timeSource;
} zl303xx_TimescaleProperties;

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/*****************   DEFINED MANAGEMENT TLV FUNCTION DECLARATIONS   ***********/

/* Utility routines to GET or SET the first 3 fields of a Management TLV. */
Uint16T zl303xx_PtpV2MgmtTlvTypeLengthIdGet(
      Uint8T *pInTlvBuf,
      Uint16T *tlvType, Uint16T *tlvLength, Uint16T *managementId);
Uint16T zl303xx_PtpV2MgmtTlvTypeLengthIdSet(
      Uint16T *tlvType, Uint16T *tlvLength, Uint16T *managementId,
      Uint8T *pOutTlvBuf);

/* These are TLV Parse routines for members of zl303xx_PtpV2MgmtTlvIdE */

/**** CLOCK DESCRIPTION TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvClockDescrUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_ClockDescriptionS *pClockDescrS);
Uint16T zl303xx_PtpV2MgmtTlvClockDescrPack(
      zl303xx_ClockDescriptionS *pClockDescrS,
      Uint8T *pOutTlvBuf);

/**** DEFAULT DATA SET TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvDefaultDSUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_DefaultDS *pDefaultDS);
Uint16T zl303xx_PtpV2MgmtTlvDefaultDSPack(
      zl303xx_DefaultDS *pDefaultDS,
      Uint8T *pOutTlvBuf);

/**** CURRENT DATA SET TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvCurrentDSUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_CurrentDS *pCurrentDS);
Uint16T zl303xx_PtpV2MgmtTlvCurrentDSPack(
      zl303xx_CurrentDS *pCurrentDS,
      Uint8T *pOutTlvBuf);

/**** PARENT DATA SET TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvParentDSUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_ParentDS *pParentDS);
Uint16T zl303xx_PtpV2MgmtTlvParentDSPack(
      zl303xx_ParentDS *pParentDS,
      Uint8T *pOutTlvBuf);

/**** TIME PROPERTIES DATA SET TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvTimePropertiesDSUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_TimePropertiesDS *pTimePropertiesDS);
Uint16T zl303xx_PtpV2MgmtTlvTimePropertiesDSPack(
      zl303xx_TimePropertiesDS *pTimePropertiesDS,
      Uint8T *pOutTlvBuf);

/**** PORT DATA SET TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvPortDSUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_PortDS *pPortDS);
Uint16T zl303xx_PtpV2MgmtTlvPortDSPack(
      zl303xx_PortDS *pPortDS,
      Uint8T *pOutTlvBuf);

/**** PRIORITY1 TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvPriority1Unpack(
      Uint8T *pInTlvBuf,
      Uint8T *priority1);
Uint16T zl303xx_PtpV2MgmtTlvPriority1Pack(
      Uint8T *priority1,
      Uint8T *pOutTlvBuf);

/**** PRIORITY2 TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvPriority2Unpack(
      Uint8T *pInTlvBuf,
      Uint8T *priority2);
Uint16T zl303xx_PtpV2MgmtTlvPriority2Pack(
      Uint8T *priority2,
      Uint8T *pOutTlvBuf);

/**** DOMAIN TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvDomainUnpack(
      Uint8T *pInTlvBuf,
      Uint8T *domain);
Uint16T zl303xx_PtpV2MgmtTlvDomainPack(
      Uint8T *domain,
      Uint8T *pOutTlvBuf);

/**** SLAVE ONLY TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvSlaveOnlyUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_BooleanE *bSlaveOnly);
Uint16T zl303xx_PtpV2MgmtTlvSlaveOnlyPack(
      zl303xx_BooleanE *bSlaveOnly,
      Uint8T *pOutTlvBuf);

/**** LOG ANNOUNCE INTERVAL TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvLogAnnounceIntervalUnpack(
      Uint8T *pInTlvBuf,
      Uint8T *pLogAnnounceInterval);
Uint16T zl303xx_PtpV2MgmtTlvLogAnnounceIntervalPack(
      Uint8T *pLogAnnounceInterval,
      Uint8T *pOutTlvBuf);

/**** ANNOUNCE RECIEPT TIMEOUT TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvAnnounceRecieptTimeoutUnpack(
      Uint8T *pInTlvBuf,
      Uint8T *pAnnounceRecieptTimeout);
Uint16T zl303xx_PtpV2MgmtTlvAnnounceRecieptTimeoutPack(
      Uint8T *pAnnounceRecieptTimeout,
      Uint8T *pOutTlvBuf);

/**** LOG SYNC INTERVAL TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvLogSyncIntervalUnpack(
      Uint8T *pInTlvBuf,
      Uint8T *pLogSyncInterval);
Uint16T zl303xx_PtpV2MgmtTlvLogSyncIntervalPack(
      Uint8T *pLogSyncInterval,
      Uint8T *pOutTlvBuf);

/**** VERSION NUMBER INTERVAL TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvVersionNumberUnpack(
      Uint8T *pInTlvBuf,
      Uint8T *pVersionNumber);
Uint16T zl303xx_PtpV2MgmtTlvVersionNumberPack(
      Uint8T *pVersionNumber,
      Uint8T *pOutTlvBuf);

/**** TIME TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvTimeUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_TimeStamp *pTime);
Uint16T zl303xx_PtpV2MgmtTlvTimePack(
      zl303xx_TimeStamp *pTime,
      Uint8T *pOutTlvBuf);

/**** CLOCK ACCURACY TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvClockAccuracyUnpack(
      Uint8T *pInTlvBuf,
      Uint8T *ClockAccuracy);
Uint16T zl303xx_PtpV2MgmtTlvClockAccuracyPack(
      Uint8T *ClockAccuracy,
      Uint8T *pOutTlvBuf);

/**** UTC PROPERTIES TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvUtcPropertiesUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_UtcProperties *pUtcProperties);
Uint16T zl303xx_PtpV2MgmtTlvUtcPropertiesPack(
      zl303xx_UtcProperties *pUtcProperties,
      Uint8T *pOutTlvBuf);

/**** TRACEABILITY PROPERTIES TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvTraceabilityPropertiesUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_TraceabilityProperties *pTraceabilityProperties);
Uint16T zl303xx_PtpV2MgmtTlvTraceabilityPropertiesPack(
      zl303xx_TraceabilityProperties *pTraceabilityProperties,
      Uint8T *pOutTlvBuf);

/**** TIMESCALE PROPERTIES TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvTimescalePropertiesUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_TimescaleProperties *pTimescaleProperties);
Uint16T zl303xx_PtpV2MgmtTlvTimescalePropertiesPack(
      zl303xx_TimescaleProperties *pTimescaleProperties,
      Uint8T *pOutTlvBuf);

/**** UNICAST ENABLE TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvUnicastNegEnUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_BooleanE *bUniNegEn);
Uint16T zl303xx_PtpV2MgmtTlvUnicastNegEnPack(
      zl303xx_BooleanE *bUniNegEn,
      Uint8T *pOutTlvBuf);

/**** UNICAST MASTER MAX TABLE SIZE TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvUnicastMasterMaxTableSizeUnpack(
      Uint8T  *pInTlvBuf,
      Uint16T *pUnicastMasterMaxTableSize);
Uint16T zl303xx_PtpV2MgmtTlvUnicastMasterMaxTableSizePack(
      Uint16T *pUnicastMasterMaxTableSize,
      Uint8T  *pOutTlvBuf);

/**** ACCEPTABLE MASTER MAX TABLE SIZE TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvAcceptableMasterMaxTableSizeUnpack(
      Uint8T  *pInTlvBuf,
      Uint16T *pAcceptableMasterMaxTableSize);
Uint16T zl303xx_PtpV2MgmtTlvAcceptableMasterMaxTableSizePack(
      Uint16T *pAcceptableMasterMaxTableSize,
      Uint8T  *pOutTlvBuf);

/**** ACCEPTABLE MASTER TABLE ENABLE TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvAcceptableMasterTableEnableUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_BooleanE *bAcceptableMasterMaxTableEnable);
Uint16T zl303xx_PtpV2MgmtTlvAcceptableMasterTableEnablePack(
      zl303xx_BooleanE *bAcceptableMasterMaxTableEnable,
      Uint8T *pOutTlvBuf);


/*******************************************************/
/**** Any CLOCK Type Management TLVs (0x6000 range). ***/
/*******************************************************/

/**** DELAY MECHANISM TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvDelayMechanismUnpack(
      Uint8T *pInTlvBuf,
      Uint8T *pDelayMechanism);
Uint16T zl303xx_PtpV2MgmtTlvDelayMechanismPack(
      Uint8T *pDelayMechanism,
      Uint8T *pOutTlvBuf);

/**** LOG MIN PDELAY REQ INTERVAL TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvLogMinPdelayReqIntervalUnpack(
      Uint8T *pInTlvBuf,
      Uint8T *pLogMinPdelayReqInterval);
Uint16T zl303xx_PtpV2MgmtTlvLogMinPdelayReqIntervalPack(
      Uint8T *pLogMinPdelayReqInterval,
      Uint8T *pOutTlvBuf);


/**************************************************************************/
/**** MANAGEMENT ERROR TLV (tlvType = 0x02; see Table 34 in IEEE-1588). ***/
/**************************************************************************/

/**** MANAGEMENT ERROR TLV utilities ***/
Uint16T zl303xx_PtpV2MgmtTlvErrorPack(
      Uint8T *pOutTlvBuf,
      zl303xx_ManagementErrorIdE mgmtErrorId,
      zl303xx_PtpV2MgmtTlvIdE mgmtTlvId,
      const char *displayMsg);
Uint16T zl303xx_PtpV2MgmtTlvErrorUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_ManagementErrorIdE *mgmtErrorId,
      zl303xx_PtpV2MgmtTlvIdE *mgmtTlvId,
      char *displayMsg);

/* Some MACROS to pack TLV Errors with consistent messages without having
 * to know all the details (or search for block and copy). */
#define zl303xx_PtpV2MgmtTlvErrorPack_InvalidMgmtId(pOutTlvBuf, mgmtTlvId) \
            zl303xx_PtpV2MgmtTlvErrorPack((pOutTlvBuf), ZL303XX_MGMT_ERR_NO_SUCH_ID, (mgmtTlvId), "Unrecognized Management ID")
#define zl303xx_PtpV2MgmtTlvErrorPack_UnsupportedMgmtId(pOutTlvBuf, mgmtTlvId) \
            zl303xx_PtpV2MgmtTlvErrorPack((pOutTlvBuf), ZL303XX_MGMT_ERR_NOT_SUPPORTED, (mgmtTlvId), "Unsupported Management ID")
#define zl303xx_PtpV2MgmtTlvErrorPack_InvalidLength(pOutTlvBuf, mgmtTlvId) \
            zl303xx_PtpV2MgmtTlvErrorPack((pOutTlvBuf), ZL303XX_MGMT_ERR_WRONG_LENGTH, (mgmtTlvId), "Invalid TLV Length")
#define zl303xx_PtpV2MgmtTlvErrorPack_InvalidPort(pOutTlvBuf, mgmtTlvId) \
            zl303xx_PtpV2MgmtTlvErrorPack((pOutTlvBuf), ZL303XX_MGMT_ERR_WRONG_VALUE, (mgmtTlvId), "Port number not found")
#define zl303xx_PtpV2MgmtTlvErrorPack_SetFailed(pOutTlvBuf, mgmtTlvId) \
            zl303xx_PtpV2MgmtTlvErrorPack((pOutTlvBuf), ZL303XX_MGMT_ERR_NOT_SETABLE, (mgmtTlvId), "SET Action Failed")
#define zl303xx_PtpV2MgmtTlvErrorPack_InvalidAction(pOutTlvBuf, mgmtTlvId) \
            zl303xx_PtpV2MgmtTlvErrorPack((pOutTlvBuf), ZL303XX_MGMT_ERR_NOT_SUPPORTED, (mgmtTlvId), "Invalid Action for Management ID")


/*****************   MANAGEMENT TLV FRAGMENT FUNCTION DECLARATIONS   **********/
/* These are not complete TLVs but form parts (fragments) of other TLVs.
 * Separate routines are created to take advantage of code reuse. */

/**** PTP TEXT utilities ***/
Uint16T zl303xx_PtpV2MsgTextUnpack(Uint8T *pInTextBuf, char *outString, Uint16T OS_STRLENMax);
Uint16T zl303xx_PtpV2MsgTextPack(const char *inString, Uint8T *pOutTextBuf, Uint16T bufLenMax);


#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
