

/*****************************************************************************
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


/****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_PtpV2MsgParse.h"
#include "zl303xx_PtpV2MgmtTlvParse.h"
#include "zl303xx_PtpV2MgmtTlvHandle.h"

/****************   DEFINES     **********************************************/

/* The maximum permitted length of the displayData field in a MGMT error TLV */
#define ZL303XX_MGMT_TLV_ERROR_DISPLAY_BYTES_MAX        50

/* TLVs with a single byte of data are actually 2-bytes long (in order to keep
 * the length even) with the following composition:
 *    byte[0] = data value
 *    byte[1] = reserved */
#define ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES          2

/****************   DATA TYPES   *********************************************/

/****************   STATIC FUNCTION DECLARATIONS   ***************************/

/* Since many TLV are single bit representations of a TRUE/FALSE Boolean,
 * re-use the following local routines.  */
static void zl303xx_PtpV2MgmtTlvBooleanUnpack(
      Uint8T *tlvByte,
      Uint8T bitMask,
      zl303xx_BooleanE *boolVal);
static void zl303xx_PtpV2MgmtTlvBooleanPack(
      zl303xx_BooleanE boolVal,
      Uint8T bitMask,
      Uint8T *tlvByte);

/* Since many TLV are single bytes, re-use the following local routines.  */
static Uint16T zl303xx_PtpV2MgmtTlvSingleByteTlvUnpack(
      Uint8T *tlvByte,
      Uint8T *byteVal);
static Uint16T zl303xx_PtpV2MgmtTlvSingleByteTlvPack(
      Uint8T byteVal,
      Uint8T *tlvByte);

/****************   STATIC GLOBAL VARIABLES   ********************************/

/****************   EXPORTED GLOBAL VARIABLES   ******************************/

/****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/****************   DEFINED MANAGEMENT TLV FUNCTION DEFINITIONS   ************/

/* zl303xx_PtpV2MgmtTlvTypeLengthIdGet */
/** 

   Unpacks the TYPE and/or LENGTH and/or MANAGEMENT_ID fields of a Management
   TLV buffer. (If either value is not required, it can be left as NULL to be
   ignored).

  Parameters:
   [in]   pInTlvBuf     Pointer to a buffer containing a TLV (starts with
                              the TYPE field).
   [out]  tlvType       The TLV TYPE field.
   [out]  tlvLength     The TLV LENGTH field.
   [out]  managementId  The Management TLV ID field.

  Return Value:  None

  Notes:
   Assumes the length of the buffer has already been checked.
   Assumes input pointers have already been checked.

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvTypeLengthIdGet(
      Uint8T *pInTlvBuf,
      Uint16T *tlvType,
      Uint16T *tlvLength,
      Uint16T *managementId)
{
   Uint16T type, length, id;

   /* The TYPE is the first 2 bytes. */
   type = UNPACK_16BIT(pInTlvBuf + 0);

   /* The LENGTH is the 3rd & 4th bytes. */
   length = UNPACK_16BIT(pInTlvBuf + 2);

   /* The MANAGEMENT ID is the 5th/6th bytes for MGMT and 7th/8th bytes for ERROR. */
   if (type == PTP_TLV_MANAGEMENT)
   {
      id = UNPACK_16BIT(pInTlvBuf + 4);
   }
   else /* (type == PTP_TLV_MANAGEMENT_ERROR_STATUS) */
   {
      id = UNPACK_16BIT(pInTlvBuf + 6);
   }

   /* Datafill any required fields. */
   if (tlvType != NULL) { *tlvType = type; }
   if (tlvLength != NULL) { *tlvLength = length; }
   if (managementId != NULL) { *managementId = id; }

   return ZL303XX_PTP_V2_MGMT_TLV_HEADER_LEN;
}  /* END zl303xx_PtpV2MgmtTlvTypeLengthIdGet */

/* zl303xx_PtpV2MgmtTlvTypeLengthIdSet */
/** 

   Packs the TYPE and/or LENGTH and/or MANAGEMENT_ID fields of a Management
   TLV buffer. (If either value is not required, it can be left as NULL to be
   ignored).

  Parameters:
   [in]  tlvType        The TLV TYPE field.
   [in]  tlvLength      The TLV LENGTH field.
   [in]  managementId   The Management TLV ID field.

   [out] pOutTlvBuf     Pointer to a buffer containing a TLV (starts with
                              the TYPE field).

  Return Value:  None

  Notes:
   Assumes input pointers have already been checked.

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvTypeLengthIdSet(
      Uint16T *tlvType,
      Uint16T *tlvLength,
      Uint16T *managementId,
      Uint8T *pOutTlvBuf)
{

   /* The TYPE is the first 2 bytes. */
   if (tlvType != NULL) { PACK_16BIT((pOutTlvBuf + 0), *tlvType); }

   /* The LENGTH is the 3rd & 4th bytes. */
   if (tlvLength != NULL) { PACK_16BIT((pOutTlvBuf + 2), *tlvLength); }

   /* The MANAGEMENT ID is the 5rd & 6th bytes. */
   if (managementId != NULL) { PACK_16BIT((pOutTlvBuf + 4), *managementId); }

   return ZL303XX_PTP_V2_MGMT_TLV_HEADER_LEN;
}  /* END zl303xx_PtpV2MgmtTlvTypeLengthIdSet */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvClockDescrUnpack

  Details:
   Unpacks a Clock Description TLV into a compatible data type for local use.

  Parameters:
   [in]  pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out] pClockDescrS   Pointer to the unpacked Clock Description structure.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvClockDescrUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_ClockDescriptionS *pClockDescrS)
{
   /* Clock Description TLV Composition (IEEE-1588-2008: Table 41):
    * -------------------------------------------------------------
    * Field                   Length    Offset              MAX
    * -------------------------------------------------------------
    * clockType                  2        0
    * physicalLayerProtocol      L        2                 Text:1 <= L <= (1+32)
    * physicalAddressLength      2        2+L
    * physicalAddress            S        4+L               0 <= S <= 16
    * protocolAddress            N        4+L+S
    * manufacturerIdentity       3        4+L+S+N
    * reserved                   1        7+L+S+N
    * productDescription         P        8+L+S+N           Text:1 <= P <= (1+64)
    * revisionData               Q        8+L+S+N+P         Text:1 <= Q <= (1+32)
    * userDescription            R        8+L+S+N+P+Q       Text:1 <= R <= (1+128)
    * profileIdentity            6        8+L+S+N+P+Q+R
    * pad                        M        14+L+S+N+P+Q+R    0 <= M <= 1
    * -------------------------------------------------------------
    * Total Length              14+L+S+N+P+Q+R+M    */

   Uint16T bytesProcessed = 0;


   /* clockType */
   pClockDescrS->clockType = UNPACK_16BIT(pInTlvBuf + bytesProcessed);
   bytesProcessed += 2;

   /* physicalLayerProtocol */
   bytesProcessed += zl303xx_PtpV2MsgTextUnpack(
                              (pInTlvBuf + bytesProcessed),
                              pClockDescrS->physicalLayerProtocol,
                              ZL303XX_PTP_NODE_PHYSICAL_PROTO_LEN);

   /* physicalAddressLength */
   pClockDescrS->physicalAddressLength = UNPACK_16BIT(pInTlvBuf + bytesProcessed);
   bytesProcessed += 2;

   /* physicalAddress */
   {
      /* Need to limit the buffer copy to the size of the array. */
      Uint16T  copyStringLen = ((pClockDescrS->physicalAddressLength <= ZL303XX_PTP_NODE_PHYSICAL_ADDR_LEN)
            ? pClockDescrS->physicalAddressLength
            : ZL303XX_PTP_NODE_PHYSICAL_ADDR_LEN);

      OS_MEMCPY(pClockDescrS->physicalAddress, (pInTlvBuf + bytesProcessed), copyStringLen);

      /* Jump over the entire TLV field even if array size was smaller than the
       * actual length of the packed TLV value.  */
      bytesProcessed += pClockDescrS->physicalAddressLength;
   }

   /* protocolAddress */
   bytesProcessed += zl303xx_PtpV2MsgProtocolAddressUnpack(
                              (pInTlvBuf + bytesProcessed),
                              &pClockDescrS->protocolAddress);

   /* manufacturerIdentity */
   OS_MEMCPY(pClockDescrS->manufacturerIdentity, (pInTlvBuf + bytesProcessed), ZL303XX_PTP_NODE_OUI_LEN);
   bytesProcessed += ZL303XX_PTP_NODE_OUI_LEN;

   /* Reserved Field */
   bytesProcessed += 1;

   /* productDescription */
   bytesProcessed += zl303xx_PtpV2MsgTextUnpack(
                              (pInTlvBuf + bytesProcessed),
                              pClockDescrS->productDescription,
                              ZL303XX_PTP_NODE_PROD_DESC_LEN);

   /* revisionData */
   bytesProcessed += zl303xx_PtpV2MsgTextUnpack(
                              (pInTlvBuf + bytesProcessed),
                              pClockDescrS->revisionData,
                              ZL303XX_PTP_NODE_REV_DATA_LEN);

   /* userDescription */
   bytesProcessed += zl303xx_PtpV2MsgTextUnpack(
                              (pInTlvBuf + bytesProcessed),
                              pClockDescrS->userDescription,
                              ZL303XX_PTP_NODE_USER_DESC_LEN);

   /* profileIdentity */
   OS_MEMCPY(pClockDescrS->profileIdentity, (pInTlvBuf + bytesProcessed), PTP_PROFILE_IDENTITY_LENGTH);
   bytesProcessed += PTP_PROFILE_IDENTITY_LENGTH;

   return bytesProcessed;
}  /* END zl303xx_PtpV2MgmtTlvClockDescrUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvClockDescrPack

  Details:
   Packs a Clock Description structure into its network TLV format.

  Parameters:
   [in]  pClockDescrS      Pointer to the Clock Description structure to pack.

   [out] pOutTlvDataBuf    Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvClockDescrPack(
      zl303xx_ClockDescriptionS *pClockDescrS,
      Uint8T *pOutTlvDataBuf)
{
   /* Clock Description TLV Composition (IEEE-1588-2008: Table 41):
    * -------------------------------------------------------------
    * Field                   Length    Offset              MAX
    * -------------------------------------------------------------
    * clockType                  2        0
    * physicalLayerProtocol      L        2                 Text:1 <= L <= (1+32)
    * physicalAddressLength      2        2+L
    * physicalAddress            S        4+L               0 <= S <= 16
    * protocolAddress            N        4+L+S
    * manufacturerIdentity       3        4+L+S+N
    * reserved                   1        7+L+S+N
    * productDescription         P        8+L+S+N           Text:1 <= P <= (1+64)
    * revisionData               Q        8+L+S+N+P         Text:1 <= Q <= (1+32)
    * userDescription            R        8+L+S+N+P+Q       Text:1 <= R <= (1+128)
    * profileIdentity            6        8+L+S+N+P+Q+R
    * pad                        M        14+L+S+N+P+Q+R    0 <= M <= 1
    * -------------------------------------------------------------
    * Total Length              14+L+S+N+P+Q+R+M    */

   Uint16T bytesPacked = 0;


   /* clockType */
   PACK_16BIT((pOutTlvDataBuf + bytesPacked), pClockDescrS->clockType);
   bytesPacked += 2;

   /* physicalLayerProtocol */
   bytesPacked += zl303xx_PtpV2MsgTextPack(
                           pClockDescrS->physicalLayerProtocol,
                           (pOutTlvDataBuf + bytesPacked),
                           ZL303XX_PTP_NODE_PHYSICAL_PROTO_LEN);

   /* physicalAddressLength */
   PACK_16BIT((pOutTlvDataBuf + bytesPacked), pClockDescrS->physicalAddressLength);
   bytesPacked += 2;

   /* physicalAddress */
   {
      /* Need to limit the buffer copy to the size of the array. */
      Uint16T copyStringLen = ((pClockDescrS->physicalAddressLength <= ZL303XX_PTP_NODE_PHYSICAL_ADDR_LEN)
            ? pClockDescrS->physicalAddressLength
            : ZL303XX_PTP_NODE_PHYSICAL_ADDR_LEN);

      OS_MEMCPY((pOutTlvDataBuf + bytesPacked), pClockDescrS->physicalAddress, copyStringLen);
      bytesPacked += copyStringLen;
   }

   /* protocolAddress */
   bytesPacked += zl303xx_PtpV2MsgProtocolAddressPack(
                        &pClockDescrS->protocolAddress,
                        (pOutTlvDataBuf + bytesPacked));

   /* manufacturerIdentity */
   OS_MEMCPY((pOutTlvDataBuf + bytesPacked), pClockDescrS->manufacturerIdentity, ZL303XX_PTP_NODE_OUI_LEN);
   bytesPacked += ZL303XX_PTP_NODE_OUI_LEN;

   /* Reserved Field */
   OS_MEMSET((pOutTlvDataBuf + bytesPacked), 0x00, 1);
   bytesPacked += 1;

   /* productDescription */
   bytesPacked += zl303xx_PtpV2MsgTextPack(
                           pClockDescrS->productDescription,
                           (pOutTlvDataBuf + bytesPacked),
                           ZL303XX_PTP_NODE_PROD_DESC_LEN);

   /* revisionData */
   bytesPacked += zl303xx_PtpV2MsgTextPack(
                           pClockDescrS->revisionData,
                           (pOutTlvDataBuf + bytesPacked),
                           ZL303XX_PTP_NODE_REV_DATA_LEN);

   /* userDescription */
   bytesPacked += zl303xx_PtpV2MsgTextPack(
                           pClockDescrS->userDescription,
                           (pOutTlvDataBuf + bytesPacked),
                           ZL303XX_PTP_NODE_USER_DESC_LEN);

   /* profileIdentity */
   OS_MEMCPY((pOutTlvDataBuf + bytesPacked), pClockDescrS->profileIdentity, PTP_PROFILE_IDENTITY_LENGTH);
   bytesPacked += PTP_PROFILE_IDENTITY_LENGTH;

   /* Add a PAD byte to make it even if necessary. */
   if ((bytesPacked % 2) != 0)
   {
      OS_MEMSET((pOutTlvDataBuf + bytesPacked), 0x00, 1);
      bytesPacked += 1;
   }

   return bytesPacked;
}  /* END zl303xx_PtpV2MgmtTlvClockDescrPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvDefaultDSUnpack

  Details:
   Unpacks a Default Data Set TLV into a compatible data type for local use.

  Parameters:
   [in]  pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out] pDefaultDS     Pointer to the unpacked Default Data Set structure.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvDefaultDSUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_DefaultDS *pDefaultDS)
{
   /* Default Data Set TLV Composition (IEEE-1588-2008: Table 50):
    * -------------------------------------------------------
    * Field             Length    Offset
    * ------------------------------------------------------
    * flags:              1        0
    *    TSC   (2-Step)       bit[0]   mask:0x01
    *    SO    (Slave-Only)   bit[1]   mask:0x02
    * RESERVED            1        1
    * numberPorts         2        2
    * priority1           1        4
    * clockQuality        4        5
    * priority2           1        9
    * clockIdentity       8        10
    * domainNumber        1        18
    * RESERVED            1        19
    * ------------------------------------------------------
    * Total Length                 20    */

   /* Flags (byte 0)*/
   (void)zl303xx_PtpV2MgmtTlvBooleanUnpack((pInTlvBuf + 0), 0x01, &pDefaultDS->twoStepFlag);
   (void)zl303xx_PtpV2MgmtTlvBooleanUnpack((pInTlvBuf + 0), 0x02, &pDefaultDS->slaveOnly);

   /* Number of Ports (bytes 2 & 3) */
   pDefaultDS->numberPorts = UNPACK_16BIT(pInTlvBuf + 2);

   /* Priority-1 (byte 4) */
   pDefaultDS->priority1 = UNPACK_8BIT(pInTlvBuf + 4);

   /* Clock Quality:
    *    Clock Class    (byte 5)
    *    Clock Accuracy (byte 6)
    *    Clock Variance (bytes 7 & 8) */
   (void)zl303xx_PtpV2MsgClockQualityUnpack((pInTlvBuf + 5), &pDefaultDS->clockQuality);

   /* Priority-2 (byte 9) */
   pDefaultDS->priority2 = UNPACK_8BIT(pInTlvBuf + 9);

   /* Clock identity (bytes 10 to 17) */
   (void)zl303xx_PtpV2MsgClockIdentityUnpack((pInTlvBuf + 10), pDefaultDS->clockIdentity);

   /* Domain Number (byte 18) */
   pDefaultDS->domainNumber = UNPACK_8BIT(pInTlvBuf + 18);

   return ZL303XX_PTP_V2_MGMT_TLV_DEFAULT_DATA_SET_LEN;
}  /* END zl303xx_PtpV2MgmtTlvDefaultDSUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvDefaultDSPack

  Details:
   Packs a Default Data Set structure into its network TLV format.

  Parameters:
   [in]  pDefaultDS        Pointer to the Default Data Set structure to pack.

   [out] pOutTlvDataBuf    Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvDefaultDSPack(
      zl303xx_DefaultDS *pDefaultDS,
      Uint8T *pOutTlvDataBuf)
{
   /* Default Data Set TLV Composition (IEEE-1588-2008: Table 50):
    * -------------------------------------------------------
    * Field             Length    Offset
    * ------------------------------------------------------
    * flags:              1        0
    *    TSC   (2-Step)       bit[0]   mask:0x01
    *    SO    (Slave-Only)   bit[1]   mask:0x02
    * RESERVED            1        1
    * numberPorts         2        2
    * priority1           1        4
    * clockQuality        4        5
    * priority2           1        9
    * clockIdentity       8        10
    * domainNumber        1        18
    * RESERVED            1        19
    * ------------------------------------------------------
    * Total Length                 20    */

   /* Clear the output buffer. */
   OS_MEMSET(pOutTlvDataBuf, 0x00, ZL303XX_PTP_V2_MGMT_TLV_DEFAULT_DATA_SET_LEN);

   /* Flags (byte 0)*/
   (void)zl303xx_PtpV2MgmtTlvBooleanPack(pDefaultDS->twoStepFlag, 0x01, (pOutTlvDataBuf + 0));
   (void)zl303xx_PtpV2MgmtTlvBooleanPack(pDefaultDS->slaveOnly,   0x02, (pOutTlvDataBuf + 0));

   /* Byte 1 is Reserved (cleared by OS_MEMSET). */

   /* Number of Ports (bytes 2 & 3) */
   PACK_16BIT((pOutTlvDataBuf + 2), pDefaultDS->numberPorts);

   /* Priority-1 (byte 4) */
   PACK_8BIT((pOutTlvDataBuf + 4), pDefaultDS->priority1);

   /* Clock Quality:
    *    Clock Class    (byte 5)
    *    Clock Accuracy (byte 6)
    *    Clock Variance (bytes 7 & 8) */
   zl303xx_PtpV2MsgClockQualityPack(&pDefaultDS->clockQuality, (pOutTlvDataBuf + 5));

   /* Priority-2 (byte 9) */
   PACK_8BIT((pOutTlvDataBuf + 9), pDefaultDS->priority2);

   /* Clock identity (bytes 10 to 17) */
   (void)zl303xx_PtpV2MsgClockIdentityPack(pDefaultDS->clockIdentity, (pOutTlvDataBuf + 10));

   /* Domain Number (byte 18) */
   PACK_8BIT((pOutTlvDataBuf + 18), pDefaultDS->domainNumber);

   /* Byte 19 is Reserved (cleared by OS_MEMSET). */

   return ZL303XX_PTP_V2_MGMT_TLV_DEFAULT_DATA_SET_LEN;
}  /* END zl303xx_PtpV2MgmtTlvDefaultDSPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvCurrentDSPack

  Details:
   Packs a Current Data Set TLV into a compatible data type for local use.

  Parameters:
   [in]  pCurrentDS        Pointer to the Current Data Set structure to be packed.

   [out] pOutTlvDataBuf    Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvCurrentDSPack(
      zl303xx_CurrentDS *pCurrentDS,
      Uint8T *pOutTlvDataBuf)
{
   /* Current Data Set TLV Composition (IEEE-1588-2008: Table 55):
    * -------------------------------------------------------
    * Field             Length    Offset
    * ------------------------------------------------------
    * stepsRemoved        2        0
    * offsetFromMaster    8        2
    * meanPathDelay       8        10
    * ------------------------------------------------------
    * Total Length                 18    */

   OS_MEMSET(pOutTlvDataBuf, 0x00, ZL303XX_PTP_V2_MGMT_TLV_CURRENT_DATA_SET_LENGTH);

   /* stepsRemoved */
   PACK_16BIT((pOutTlvDataBuf + 0), pCurrentDS->stepsRemoved);

   /* offset_from_master */
   PACK_64BIT((pOutTlvDataBuf + 2), pCurrentDS->offsetFromMaster.scaledNanoseconds);

   /* mean_path_delay */
   PACK_64BIT((pOutTlvDataBuf + 10), pCurrentDS->meanPathDelay.scaledNanoseconds);

   return ZL303XX_PTP_V2_MGMT_TLV_CURRENT_DATA_SET_LENGTH;
}  /* END zl303xx_PtpV2MgmtTlvCurrentDSPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvCurrentDSUnpack

  Details:
   Packs a Current Data Set TLV into a compatible data type for local use.

  Parameters:
   [in]  pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out] pCurrentDS     Pointer to the unpacked Current Data Set structure.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvCurrentDSUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_CurrentDS *pCurrentDS)
{
   /* Current Data Set TLV Composition (IEEE-1588-2008: Table 55):
    * -------------------------------------------------------
    * Field             Length    Offset
    * ------------------------------------------------------
    * stepsRemoved        2        0
    * offsetFromMaster    8        2
    * meanPathDelay       8        10
    * ------------------------------------------------------
    * Total Length                 18    */

   /* stepsRemoved */
   pCurrentDS->stepsRemoved = UNPACK_16BIT(pInTlvBuf + 0);

   /* offset_from_master */
   UNPACK_64BIT((pInTlvBuf + 2), pCurrentDS->offsetFromMaster.scaledNanoseconds);

   /* mean_path_delay */
   UNPACK_64BIT((pInTlvBuf + 10), pCurrentDS->meanPathDelay.scaledNanoseconds);

   return ZL303XX_PTP_V2_MGMT_TLV_CURRENT_DATA_SET_LENGTH;
}  /* END zl303xx_PtpV2MgmtTlvCurrentDSUnpack */


/*

  Function Name:
    zl303xx_PtpV2MgmtTlvParentDSPack

  Details:
   Packs a Parent Data Set TLV into a compatible data type for local use.

  Parameters:
   [in]  pParentDS         Pointer to the Parent Data Set structure to be packed.

   [out] pOutTlvDataBuf    Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvParentDSPack(
      zl303xx_ParentDS *pParentDS,
      Uint8T *pOutTlvDataBuf)
{
   OS_MEMSET(pOutTlvDataBuf, 0x00, ZL303XX_PTP_V2_MGMT_TLV_PARENT_DATA_SET_LENGTH);

   /* Parent Data Set TLV Composition (IEEE-1588-2008: Table 56):
    * -------------------------------------------------------
    * Field                                  Length    Offset
    * ------------------------------------------------------
    * parentPortIdentity                        10       0
    * flags                                     1        10
    *    PS   (Parent Stats)       bit[0]   mask:0x01
    * reserved                                  1        11
    * observedParentOffsetScaledLogVariance     2        12
    * observedParentClockPhaseChangeRate        4        14
    * grandmasterPriority1                      1        18
    * grandmasterClockQuality                   4        19
    * grandmasterPriority2                      1        23
    * grandmasterIdentity                       8        24
    * ------------------------------------------------------
    * Total Length                                       32   */

   /* parentPortIdentity */
   zl303xx_PtpV2MsgPortIdentityPack(&pParentDS->parentPortIdentity, (pOutTlvDataBuf +  0));

   /* Parent Statistics Supported flag */
   (void)zl303xx_PtpV2MgmtTlvBooleanPack(pParentDS->parentStats, 0x01, (pOutTlvDataBuf + 10));

   /* observedParentOffsetScaledLogVariance */
   PACK_16BIT((pOutTlvDataBuf + 12), pParentDS->observedParentOffsetScaledLogVariance);

   /* observedParentClockPhaseChangeRate */
   PACK_32BIT((pOutTlvDataBuf + 14), pParentDS->observedParentClockPhaseChangeRate);

   /* grandmasterPriority1 */
   PACK_8BIT((pOutTlvDataBuf + 18), pParentDS->grandmasterPriority1);

   /* grandmasterClockQuality */
   zl303xx_PtpV2MsgClockQualityPack(&pParentDS->grandmasterClockQuality, (pOutTlvDataBuf + 19));

   /* grandmasterPriority2 */
   PACK_8BIT((pOutTlvDataBuf + 23), pParentDS->grandmasterPriority2);

   /* grandmasterIdentity */
   zl303xx_PtpV2MsgClockIdentityPack(pParentDS->grandmasterIdentity, (pOutTlvDataBuf +  24));

   return ZL303XX_PTP_V2_MGMT_TLV_PARENT_DATA_SET_LENGTH;
}  /* END zl303xx_PtpV2MgmtTlvParentDSPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvParentDSUnpack

  Details:
   Unpacks a Parent Data Set TLV into a compatible data type for local use.

  Parameters:
   [in]  pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out] pParentDS      Pointer to the unpacked Parent Data Set structure.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvParentDSUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_ParentDS *pParentDS)
{
   /* Parent Data Set TLV Composition (IEEE-1588-2008: Table 56):
    * -------------------------------------------------------
    * Field                               Length    Offset
    * ------------------------------------------------------
    * parentPortIdentity                     10       0
    * flags                                  1        10
    *    PS   (Parent Stats)       bit[0]   mask:0x01
    * reserved                               1        11
    * observedParentOffsetScaledLogVariance  2        12
    * observedParentClockPhaseChangeRate     4        14
    * grandmasterPriority1                   1        18
    * grandmasterClockQuality                4        19
    * grandmasterPriority2                   1        23
    * grandmasterIdentity                    8        24
    * ------------------------------------------------------
    * Total Length                                    32   */

   /* parentPortIdentity */
   zl303xx_PtpV2MsgPortIdentityUnpack((pInTlvBuf +  0), &pParentDS->parentPortIdentity);

   /* Parent Statistics Supported flag */
   (void)zl303xx_PtpV2MgmtTlvBooleanUnpack((pInTlvBuf + 10), 0x01, &pParentDS->parentStats);

   /* observedParentOffsetScaledLogVariance */
   pParentDS->observedParentOffsetScaledLogVariance = UNPACK_16BIT(pInTlvBuf + 12);

   /* observedParentClockPhaseChangeRate */
   pParentDS->observedParentClockPhaseChangeRate = UNPACK_32BIT(pInTlvBuf + 14);

   /* grandmasterPriority1 */
   pParentDS->grandmasterPriority1 = UNPACK_8BIT(pInTlvBuf + 18);

   /* grandmasterClockQuality */
   zl303xx_PtpV2MsgClockQualityUnpack((pInTlvBuf + 19), &pParentDS->grandmasterClockQuality);

   /* grandmasterPriority2 */
   pParentDS->grandmasterPriority2 = UNPACK_8BIT(pInTlvBuf + 23);

   /* grandmasterIdentity */
   zl303xx_PtpV2MsgClockIdentityUnpack((pInTlvBuf + 24), pParentDS->grandmasterIdentity);

   return ZL303XX_PTP_V2_MGMT_TLV_PARENT_DATA_SET_LENGTH;
}  /* END zl303xx_PtpV2MgmtTlvParentDSUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvTimePropertiesDSPack

  Details:
   Packs a Time Properties Data Set TLV into a compatible data type for local use.

  Parameters:
   [in]  pTimePropertiesDS    Pointer to the TimeProperties Data Set structure
                                    to be packed.

   [out] pOutTlvDataBuf       Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvTimePropertiesDSPack(
      zl303xx_TimePropertiesDS *pTimePropertiesDS,
      Uint8T *pOutTlvDataBuf)
{
   /* Time Properties Data Set TLV Composition (IEEE-1588-2008: Table 57):
    * -------------------------------------------------------
    * Field                            Length    Offset
    * ------------------------------------------------------
    * currentUtcOffset                  2       0
    * flags:                            1       2
    *    LI61  (leap61)          bit[0]   mask:0x01
    *    LI59  (leap59)          bit[1]   mask:0x02
    *    UTCV  (UTC Valid)       bit[2]   mask:0x04
    *    PTP   (PTP Timescale)   bit[3]   mask:0x08
    *    TTRA  (Time Traceable)  bit[4]   mask:0x10
    *    FTRA  (Freq Traceable)  bit[5]   mask:0x20
    * timeSource                         1       3
    * ------------------------------------------------------
    * Total Length                               4    */

   OS_MEMSET(pOutTlvDataBuf, 0x00, ZL303XX_PTP_V2_MGMT_TLV_TIME_PROPERTIES_DATA_SET_LENGTH);

   /* currentUtcOffset  */
   PACK_16BIT((pOutTlvDataBuf + 0), pTimePropertiesDS->currentUtcOffset);

   /* packing flags into single variable*/
   (void)zl303xx_PtpV2MgmtTlvBooleanPack(pTimePropertiesDS->leap61,                0x01, (pOutTlvDataBuf + 2));
   (void)zl303xx_PtpV2MgmtTlvBooleanPack(pTimePropertiesDS->leap59,                0x02, (pOutTlvDataBuf + 2));
   (void)zl303xx_PtpV2MgmtTlvBooleanPack(pTimePropertiesDS->currentUtcOffsetValid, 0x04, (pOutTlvDataBuf + 2));
   (void)zl303xx_PtpV2MgmtTlvBooleanPack(pTimePropertiesDS->ptpTimescale,          0x08, (pOutTlvDataBuf + 2));
   (void)zl303xx_PtpV2MgmtTlvBooleanPack(pTimePropertiesDS->timeTraceable,         0x10, (pOutTlvDataBuf + 2));
   (void)zl303xx_PtpV2MgmtTlvBooleanPack(pTimePropertiesDS->frequencyTraceable,    0x20, (pOutTlvDataBuf + 2));

   /* timeSource */
   PACK_8BIT((pOutTlvDataBuf + 3), (Uint8T)pTimePropertiesDS->timeSource);

   return ZL303XX_PTP_V2_MGMT_TLV_TIME_PROPERTIES_DATA_SET_LENGTH;
}  /* END zl303xx_PtpV2MgmtTlvTimePropertiesDSPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvTimePropertiesDSUnpack

  Details:
   Unpacks a Time Properties Data Set TLV into a compatible data type for local use.

  Parameters:
   [in]  pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out] pTimePropertiesDS    Pointer to the unpacked TimeProperties
                                    Data Set structure.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvTimePropertiesDSUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_TimePropertiesDS *pTimePropertiesDS)
{
   /* Time Properties Data Set TLV Composition (IEEE-1588-2008: Table 57):
    * -------------------------------------------------------
    * Field                          Length    Offset
    * ------------------------------------------------------
    * currentUtcOffset                  2       0
    * flags:                            1       2
    *    LI61  (leap61)          bit[0]   mask:0x01
    *    LI59  (leap59)          bit[1]   mask:0x02
    *    UTCV  (UTC Valid)       bit[2]   mask:0x04
    *    PTP   (PTP Timescale)   bit[3]   mask:0x08
    *    TTRA  (Time Traceable)  bit[4]   mask:0x10
    *    FTRA  (Freq Traceable)  bit[5]   mask:0x20
    * timeSource                        1       3
    * ------------------------------------------------------
    * Total Length                              4    */

   /* currentUtcOffset  */
   pTimePropertiesDS->currentUtcOffset = UNPACK_16BIT(pInTlvBuf + 0);

   /* flags unpacked one-by-one */
   /* packing flags into single variable*/
   (void)zl303xx_PtpV2MgmtTlvBooleanUnpack((pInTlvBuf + 2), 0x01, &pTimePropertiesDS->leap61);
   (void)zl303xx_PtpV2MgmtTlvBooleanUnpack((pInTlvBuf + 2), 0x02, &pTimePropertiesDS->leap59);
   (void)zl303xx_PtpV2MgmtTlvBooleanUnpack((pInTlvBuf + 2), 0x04, &pTimePropertiesDS->currentUtcOffsetValid);
   (void)zl303xx_PtpV2MgmtTlvBooleanUnpack((pInTlvBuf + 2), 0x08, &pTimePropertiesDS->ptpTimescale);
   (void)zl303xx_PtpV2MgmtTlvBooleanUnpack((pInTlvBuf + 2), 0x10, &pTimePropertiesDS->timeTraceable);
   (void)zl303xx_PtpV2MgmtTlvBooleanUnpack((pInTlvBuf + 2), 0x20, &pTimePropertiesDS->frequencyTraceable);

   /* timeSource  */
   pTimePropertiesDS->timeSource = UNPACK_8BIT(pInTlvBuf + 3);

   return ZL303XX_PTP_V2_MGMT_TLV_TIME_PROPERTIES_DATA_SET_LENGTH;
}  /* END zl303xx_PtpV2MgmtTlvTimePropertiesDSUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvPortDSPack

  Details:
   Packs a Port Data Set TLV into a compatible data type for local use.

  Parameters:
   [in]  pPortDS           Pointer to the Port Data Set structure to be packed.

   [out] pOutTlvDataBuf    Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvPortDSPack(
      zl303xx_PortDS *pPortDS,
      Uint8T *pOutTlvDataBuf)
{
   /* Port Data Set TLV Composition (IEEE-1588-2008: Table 61):
    * ------------------------------------------------------
    * Field                    Length      Offset
    * ------------------------------------------------------
    * portIdentity               10          0
    * portState                  1           10
    * logMinDelayReqInterval     1           11
    * peerMeanPathDelay          8           12
    * logAnnounceInterval        1           20
    * announceReceiptTimeout     1           21
    * logSyncInterval            1           22
    * delayMechanism             1           23
    * logMinPdelayReqInterval    1           24
    * reserved/versionNumber     1           25
    * ------------------------------------------------------
    * Total Length                           26            */

   OS_MEMSET(pOutTlvDataBuf, 0x00, ZL303XX_PTP_V2_MGMT_TLV_PORT_DATA_SET_LENGTH);

   /* portIdentity  */
   zl303xx_PtpV2MsgPortIdentityPack(&pPortDS->portIdentity, (pOutTlvDataBuf + 0));

   /* portState  */
   PACK_8BIT((pOutTlvDataBuf + 10),pPortDS->portState);

   /* logMinDelayReqInterval  */
   PACK_8BIT((pOutTlvDataBuf + 11),pPortDS->logMinDelayReqInterval);

   /* peerMeanPathDelay  */
   PACK_64BIT((pOutTlvDataBuf + 12), pPortDS->peerMeanPathDelay.scaledNanoseconds);

   /* logAnnounceInterval  */
   PACK_8BIT((pOutTlvDataBuf + 20),pPortDS->logAnnounceInterval);

   /* announceReceiptTimeout  */
   PACK_8BIT((pOutTlvDataBuf + 21),pPortDS->announceReceiptTimeout);

   /* logSyncInterval  */
   PACK_8BIT((pOutTlvDataBuf + 22),pPortDS->logSyncInterval);

   /* delayMechanism  */
   PACK_8BIT((pOutTlvDataBuf + 23),pPortDS->delayMechanism);

   /* logMinPdelayReqInterval  */
   PACK_8BIT((pOutTlvDataBuf + 24),pPortDS->logMinPdelayReqInterval);

   /* For the version number we return the overall version number of the clock
    * since all the ports must support the same version number in this implementation. */
   PACK_LO_NIBBLE((pOutTlvDataBuf + 25), pPortDS->versionNumber);

   return ZL303XX_PTP_V2_MGMT_TLV_PORT_DATA_SET_LENGTH;
}  /* END zl303xx_PtpV2MgmtTlvPortDSPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvPortDSUnpack

  Details:
   Unpacks a Port Data Set TLV into a compatible data type for local use.

  Parameters:
   [in]  pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out] pPortDS        Pointer to the unpacked Port Data Set structure.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvPortDSUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_PortDS *pPortDS)
{
   /* Port Data Set TLV Composition (IEEE-1588-2008: Table 61):
    * ------------------------------------------------------
    * Field                    Length      Offset
    * ------------------------------------------------------
    * portIdentity               10          0
    * portState                  1           10
    * logMinDelayReqInterval     1           11
    * peerMeanPathDelay          8           12
    * logAnnounceInterval        1           20
    * announceReceiptTimeout     1           21
    * logSyncInterval            1           22
    * delayMechanism             1           23
    * logMinPdelayReqInterval    1           24
    * reserved/versionNumber     1           25
    * ------------------------------------------------------
    * Total Length                           26            */

   /* portIdentity  */
   zl303xx_PtpV2MsgPortIdentityUnpack((pInTlvBuf + 0), &pPortDS->portIdentity);

   /* portState  */
   pPortDS->portState = UNPACK_8BIT(pInTlvBuf + 10);

   /* logMinDelayReqInterval  */
   pPortDS->logMinDelayReqInterval = UNPACK_8BIT(pInTlvBuf + 11);

   /* peerMeanPathDelay  */
   UNPACK_64BIT((pInTlvBuf + 12), pPortDS->peerMeanPathDelay.scaledNanoseconds);

   /* logAnnounceInterval  */
   pPortDS->logAnnounceInterval = UNPACK_8BIT(pInTlvBuf + 20);

   /* announceReceiptTimeout  */
   pPortDS->announceReceiptTimeout = UNPACK_8BIT(pInTlvBuf + 21);

   /* logSyncInterval  */
   pPortDS->logSyncInterval = UNPACK_8BIT(pInTlvBuf + 22);

   /* delayMechanism  */
   pPortDS->delayMechanism = UNPACK_8BIT(pInTlvBuf + 23);

   /* logMinPdelayReqInterval  */
   pPortDS->logMinPdelayReqInterval = UNPACK_8BIT(pInTlvBuf + 24);

   /* For the version number we return the overall version number of the clock since all the ports
      must support the same version number in this implementation */
   pPortDS->versionNumber = UNPACK_LO_NIBBLE(pInTlvBuf + 25);

   return ZL303XX_PTP_V2_MGMT_TLV_PORT_DATA_SET_LENGTH;
}  /* END zl303xx_PtpV2MgmtTlvPortDSUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvPriority1Unpack

  Details:
   Unpacks a Priority1 TLV into a compatible data type for local use.

  Parameters:
   [in]  pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out] priority1      Pointer to the unpacked Uint8 priority1.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvPriority1Unpack(
      Uint8T *pInTlvBuf,
      Uint8T *priority1)
{
   return zl303xx_PtpV2MgmtTlvSingleByteTlvUnpack(pInTlvBuf, priority1);
}  /* END zl303xx_PtpV2MgmtTlvPriority1Unpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvPriority1Pack

  Details:
   Packs a Priority1 TLV into a compatible data type for local use.

  Parameters:
   [in]  priority1      Pointer to the Uint8 priority1 to be packed.

   [out] pOutTlvDataBuf Pointer to the data portion of the input TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:


*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvPriority1Pack(
      Uint8T *priority1,
      Uint8T *pOutTlvDataBuf)
{
   return zl303xx_PtpV2MgmtTlvSingleByteTlvPack(*priority1, pOutTlvDataBuf);
}  /* END zl303xx_PtpV2MgmtTlvPriority1Pack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvPriority2Unpack

  Details:
   Unpacks a Priority2 TLV into a compatible data type for local use.

  Parameters:
   [in]  pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out] priority2      Pointer to the unpacked Uint8 priority1.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvPriority2Unpack(
      Uint8T *pInTlvBuf,
      Uint8T *priority2)
{
   return zl303xx_PtpV2MgmtTlvSingleByteTlvUnpack(pInTlvBuf, priority2);
}  /* END zl303xx_PtpV2MgmtTlvPriority2Unpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvPriority2Pack

  Details:
   Packs a Priority2 TLV into a compatible data type for local use.

  Parameters:
   [in]  priority2      Pointer to the Uint8 priority2 to be packed.

   [out] pOutTlvDataBuf Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvPriority2Pack(
      Uint8T *priority2,
      Uint8T *pOutTlvDataBuf)
{
   return zl303xx_PtpV2MgmtTlvSingleByteTlvPack(*priority2, pOutTlvDataBuf);
}  /* END zl303xx_PtpV2MgmtTlvPriority2Pack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvDomainUnpack

  Details:
   Unpacks a Domain TLV into a compatible data type for local use.

  Parameters:
   [in]  pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out] domain         Pointer to the unpacked Uint8 domain.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvDomainUnpack(
      Uint8T *pInTlvBuf,
      Uint8T *domain)
{
   return zl303xx_PtpV2MgmtTlvSingleByteTlvUnpack(pInTlvBuf, domain);
}  /* END zl303xx_PtpV2MgmtTlvDomainUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvDomainPack

  Details:
   Packs a Domain TLV into a compatible data type for local use.

  Parameters:
   [in]  domain         Pointer to the Uint8 domain to be packed.

   [out] pOutTlvDataBuf Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvDomainPack(
      Uint8T *domain,
      Uint8T *pOutTlvDataBuf)
{
   return zl303xx_PtpV2MgmtTlvSingleByteTlvPack(*domain, pOutTlvDataBuf);
}  /* END zl303xx_PtpV2MgmtTlvDomainPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvSlaveOnlyUnpack

  Details:
   Unpacks a Slave-Only Management TLV.

  Parameters:
   [in]  pInTlvBuf      Pointer to the buffer containing the unpacked TLV.

   [out] bSlaveOnly     Pointer to a Slave-Only boolean return value.

  Return Value:
   Uint16T  Size of the unpacked TLV on success
            0 otherwise

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvSlaveOnlyUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_BooleanE *bSlaveOnly)
{
   /* SLAVE-ONLY TLV Composition (IEEE-1588-2008: Table 54):
    * -------------------------------------------------------
    * Field                          Length    Offset
    * ------------------------------------------------------
    * flags:                           1       0
    *    SO  (Slave-Only Enabled)   bit[0]   mask:0x01
    * reserved                         1       1
    * ------------------------------------------------------
    * Total Length                             2    */

   /* The Slave-Only bit is the lowest bit of the first byte. */
   zl303xx_PtpV2MgmtTlvBooleanUnpack((pInTlvBuf + 0), 0x01, bSlaveOnly);

   /* Even though data is only contained in a single byte, the TLV is defined
    * as being 2 bytes in length (1 byte is padding to keep it EVEN).  */
   return ZL303XX_PTP_V2_MGMT_TLV_SLAVE_ONLY_LEN;
}  /* END zl303xx_PtpV2MgmtTlvSlaveOnlyUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvSlaveOnlyPack

  Details:
   Packs a Slave-Only value into a Management TLV.

  Parameters:
   [in]  bSlaveOnly     Pointer to a Slave-Only boolean value.

   [out] pOutTlvBuf     Pointer to the buffer containing the packed TLV.

  Return Value:
   Uint16T  Size of the packed TLV on success
            0 otherwise

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvSlaveOnlyPack(
      zl303xx_BooleanE *bSlaveOnly,
      Uint8T *pOutTlvBuf)
{
   /* SLAVE-ONLY TLV Composition (IEEE-1588-2008: Table 54):
    * -------------------------------------------------------
    * Field                          Length    Offset
    * ------------------------------------------------------
    * flags:                           1       0
    *    SO  (Slave-Only Enabled)   bit[0]   mask:0x01
    * reserved                         1       1
    * ------------------------------------------------------
    * Total Length                             2    */

   /* Clear the entire buffer so we know all settings. */
   OS_MEMSET(pOutTlvBuf, 0x00, ZL303XX_PTP_V2_MGMT_TLV_SLAVE_ONLY_LEN);

   /* The Slave-Only bit is the lowest bit of the first byte. */
   zl303xx_PtpV2MgmtTlvBooleanPack(*bSlaveOnly, 0x01, (pOutTlvBuf + 0));

   /* Even though data is only contained in a single byte, the TLV is defined
    * as being 2 bytes in length (1 byte is padding to keep it EVEN).  */
   return ZL303XX_PTP_V2_MGMT_TLV_SLAVE_ONLY_LEN;
}  /* END zl303xx_PtpV2MgmtTlvSlaveOnlyPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvLogAnnounceIntervalUnpack

  Details:
   Unpacks a Log Announce Interval TLV into a compatible data type for local use.

  Parameters:
   [in]  pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out] pLogAnnounceInterval    Pointer to the unpacked Log Announce Interval.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvLogAnnounceIntervalUnpack(
      Uint8T *pInTlvBuf,
      Uint8T *pLogAnnounceInterval)
{
   return zl303xx_PtpV2MgmtTlvSingleByteTlvUnpack(pInTlvBuf, pLogAnnounceInterval);
}  /* END zl303xx_PtpV2MgmtTlvLogAnnounceIntervalUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvLogAnnounceIntervalPack

  Details:
   Packs a Log Announce Interval TLV into a compatible data type for local use.

  Parameters:
   [in]  pLogAnnounceInterval    Pointer to the Log Announce Interval to be packed.

   [out] pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvLogAnnounceIntervalPack(
      Uint8T *pLogAnnounceInterval,
      Uint8T *pOutTlvBuf)
{
   return zl303xx_PtpV2MgmtTlvSingleByteTlvPack(*pLogAnnounceInterval, pOutTlvBuf);
}  /* END zl303xx_PtpV2MgmtTlvLogAnnounceIntervalPack */


/*

  Function Name:
    zl303xx_PtpV2MgmtTlvAnnounceRecieptTimeoutUnpack

  Details:
   Unpacks a Announce Receipt Timeout TLV into a compatible data type for local use.

  Parameters:
   [in]  pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out] pAnnounceRecieptTimeout    Pointer to the unpacked Uint8 Announce Receipt Timeout.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvAnnounceRecieptTimeoutUnpack(
      Uint8T *pInTlvBuf,
      Uint8T *pAnnounceRecieptTimeout)
{
   return zl303xx_PtpV2MgmtTlvSingleByteTlvUnpack(pInTlvBuf, pAnnounceRecieptTimeout);
}  /* END zl303xx_PtpV2MgmtTlvAnnounceRecieptTimeoutUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvAnnounceRecieptTimeoutPack

  Details:
   Packs a Announce Receipt Timeout TLV into a compatible data type for local use.

  Parameters:
   [in]  pAnnounceRecieptTimeout    Pointer to the Uint8 Announce Receipt Timeout to be packed.

   [out] pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvAnnounceRecieptTimeoutPack(
      Uint8T *pAnnounceRecieptTimeout,
      Uint8T *pOutTlvBuf)
{
   return zl303xx_PtpV2MgmtTlvSingleByteTlvPack(*pAnnounceRecieptTimeout, pOutTlvBuf);
}  /* END zl303xx_PtpV2MgmtTlvAnnounceRecieptTimeoutPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvLogSyncIntervalUnpack

  Details:
   Unpacks a Log Sync Interval TLV into a compatible data type for local use.

  Parameters:
   [in]  pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out] pLogSyncInterval     Pointer to the unpacked Uint8 Log Sync Interval.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvLogSyncIntervalUnpack(
      Uint8T *pInTlvBuf,
      Uint8T *pLogSyncInterval)
{
   return zl303xx_PtpV2MgmtTlvSingleByteTlvUnpack(pInTlvBuf, pLogSyncInterval);
}  /* END zl303xx_PtpV2MgmtTlvLogSyncIntervalUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvLogSyncIntervalPack

  Details:
   Packs a Log Sync Interval TLV into a compatible data type for local use.

  Parameters:
   [in]  pLogSyncInterval     Pointer to the Uint8 Log Sync Interval to be packed.

   [out] pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvLogSyncIntervalPack(
      Uint8T *pLogSyncInterval,
      Uint8T *pOutTlvBuf)
{
   return zl303xx_PtpV2MgmtTlvSingleByteTlvPack(*pLogSyncInterval, pOutTlvBuf);
}  /* END zl303xx_PtpV2MgmtTlvLogSyncIntervalPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvVersionNumberUnpack

  Details:
   Unpacks a Version Number TLV into a compatible data type for local use.

  Parameters:
   [in]  pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out] pVersionNumber    Pointer to the unpacked Uint8 Version Number.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvVersionNumberUnpack(
      Uint8T *pInTlvBuf,
      Uint8T *pVersionNumber)
{
   *pVersionNumber = UNPACK_LO_NIBBLE((pInTlvBuf + 0));

   return ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES;
}  /* END zl303xx_PtpV2MgmtTlvVersionNumberUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvVersionNumberPack

  Details:
   Packs a Version Number TLV into a compatible data type for local use.

  Parameters:
   [in]   pVersionNumber        Pointer to the Uint8 Version Number to be packed.

   [out]  pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvVersionNumberPack(
      Uint8T *pVersionNumber,
      Uint8T *pOutTlvBuf)
{
   OS_MEMSET(pOutTlvBuf, 0x00, ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES);

   PACK_LO_NIBBLE((pOutTlvBuf + 0), *pVersionNumber);

   return ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES;
}  /* END zl303xx_PtpV2MgmtTlvVersionNumberPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvTimeUnpack

  Details:
   Unpacks a Time TLV into a compatible data type for local use.

  Parameters:
   [in]   pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out]  pTime     Pointer to the unpacked Time structure.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvTimeUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_TimeStamp *pTime)
{
   return zl303xx_PtpV2MsgTimestampUnpack(pInTlvBuf, pTime);
}  /* END zl303xx_PtpV2MgmtTlvTimeUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvTimePack

  Details:
   Packs a Time TLV into a compatible data type for local use.

  Parameters:
   [in]   pTime     Pointer to the Time structure to be packed.

   [out]  pOutTlvBuf     Pointer to the data portion of the input TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:


*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvTimePack(
      zl303xx_TimeStamp *pTime,
      Uint8T *pOutTlvBuf)
{
   return zl303xx_PtpV2MsgTimestampPack(pTime, pOutTlvBuf);
}  /* END zl303xx_PtpV2MgmtTlvTimePack */


/*

  Function Name:
    zl303xx_PtpV2MgmtTlvClockAccuracyUnpack

  Details:
   Unpacks a Clock Accuracy TLV into a compatible data type for local use.

  Parameters:
   [in]   pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out]  pClockAccuracy     Pointer to the unpacked Uint8 Clock Accuracy.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvClockAccuracyUnpack(
      Uint8T *pInTlvBuf,
      Uint8T *ClockAccuracy)
{
   return zl303xx_PtpV2MgmtTlvSingleByteTlvUnpack(pInTlvBuf, ClockAccuracy);
}  /* END zl303xx_PtpV2MgmtTlvClockAccuracyUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvClockAccuracyPack

  Details:
   Packs a Clock Accuracy TLV into a compatible data type for local use.

  Parameters:
   [in]   pClockAccuracy     Pointer to the Uint8 Clock Accuracy to be packed.

   [out]  pOutTlvBuf      Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvClockAccuracyPack(
      Uint8T *pClockAccuracy,
      Uint8T *pOutTlvBuf)
{
   return zl303xx_PtpV2MgmtTlvSingleByteTlvPack(*pClockAccuracy, pOutTlvBuf);
}  /* END zl303xx_PtpV2MgmtTlvClockAccuracyPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvUtcPropertiesUnpack

  Details:
   Unpacks a UTC Properties TLV into a compatible data type for local use.

  Parameters:
   [in]   pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out]  pUtcProperties     Pointer to the UTC Properties structure.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvUtcPropertiesUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_UtcProperties *pUtcProperties)
{
    /* UTC Properties TLV Composition (IEEE-1588-2008: Table 58):
    * -------------------------------------------------------
    * Field                            Length    Offset
    * ------------------------------------------------------
    * currentUtcOffset                  2       0
    * flags:                            1       2
    *    LI61  (leap61)          bit[0]   mask:0x01
    *    LI59  (leap59)          bit[1]   mask:0x02
    *    UTCV  (UTC Valid)       bit[2]   mask:0x04
    * reserved                          1       3
    * ------------------------------------------------------
    * Total Length                               4    */

   pUtcProperties->currentUtcOffset = UNPACK_16BIT(pInTlvBuf + 0);

   (void)zl303xx_PtpV2MgmtTlvBooleanUnpack((pInTlvBuf + 2), 0x01, &pUtcProperties->leap61);
   (void)zl303xx_PtpV2MgmtTlvBooleanUnpack((pInTlvBuf + 2), 0x02, &pUtcProperties->leap59);
   (void)zl303xx_PtpV2MgmtTlvBooleanUnpack((pInTlvBuf + 2), 0x04, &pUtcProperties->currentUtcOffsetValid);

   return ZL303XX_PTP_V2_MGMT_TLV_UTC_PROPERTIES_LEN;

}  /* END zl303xx_PtpV2MgmtTlvUtcPropertiesUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvUtcPropertiesPack

  Details:
   Packs a UTC Properties TLV into a compatible data type for local use.

  Parameters:
   [in]   pUtcProperties     Pointer to the UTC Properties structure to be packed.

   [out]  pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvUtcPropertiesPack(
      zl303xx_UtcProperties *pUtcProperties,
      Uint8T *pOutTlvBuf)
{
   OS_MEMSET(pOutTlvBuf, 0x00, ZL303XX_PTP_V2_MGMT_TLV_UTC_PROPERTIES_LEN);

   /* UTC Properties TLV Composition (IEEE-1588-2008: Table 58):
    * -------------------------------------------------------
    * Field                          Length    Offset
    * ------------------------------------------------------
    * currentUtcOffset                  2       0
    * flags:                            1       2
    *    LI61  (leap61)          bit[0]   mask:0x01
    *    LI59  (leap59)          bit[1]   mask:0x02
    *    UTCV  (UTC Valid)       bit[2]   mask:0x04
    * reserved                          1       3
    * ------------------------------------------------------
    * Total Length                              4    */

   PACK_16BIT((pOutTlvBuf + 0), pUtcProperties->currentUtcOffset);

   (void)zl303xx_PtpV2MgmtTlvBooleanPack(pUtcProperties->leap61, 0x01, (pOutTlvBuf + 2));
   (void)zl303xx_PtpV2MgmtTlvBooleanPack(pUtcProperties->leap59, 0x02, (pOutTlvBuf + 2));
   (void)zl303xx_PtpV2MgmtTlvBooleanPack(pUtcProperties->currentUtcOffsetValid, 0x04, (pOutTlvBuf + 2));

   return ZL303XX_PTP_V2_MGMT_TLV_UTC_PROPERTIES_LEN;

}  /* END zl303xx_PtpV2MgmtTlvUtcPropertiesPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvTraceabilityPropertiesUnpack

  Details:
   Unpacks a Traceability Properties TLV into a compatible data type for local use.

  Parameters:
   [in]   pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out]  pTraceabilityProperties     Pointer to the unpacked UTC Properties structure.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvTraceabilityPropertiesUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_TraceabilityProperties *pTraceabilityProperties)
{
   /* Traceability Properties TLV Composition (IEEE-1588-2008: Table 59):
    * -------------------------------------------------------
    * Field                          Length    Offset
    * ------------------------------------------------------
    * flags:                           1       0
    *    TTRA  (Time Traceable)  bit[4]   mask:0x10
    *    FTRA  (Freq Traceable)  bit[5]   mask:0x20
    * reserved                         1       1
    * ------------------------------------------------------
    * Total Length                             2    */

   (void)zl303xx_PtpV2MgmtTlvBooleanUnpack((pInTlvBuf + 0), 0x10, &pTraceabilityProperties->timeTraceable);
   (void)zl303xx_PtpV2MgmtTlvBooleanUnpack((pInTlvBuf + 0), 0x20, &pTraceabilityProperties->frequencyTraceable);

   return ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES;

}  /* END zl303xx_PtpV2MgmtTlvTraceabilityPropertiesUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvTraceabilityPropertiesPack

  Details:
   Packs a Traceability Properties TLV into a compatible data type for local use.

  Parameters:
   [in]   pTraceabilityProperties     Pointer to the Traceability Properties structure to be packed.

   [out]  pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvTraceabilityPropertiesPack(
      zl303xx_TraceabilityProperties *pTraceabilityProperties,
      Uint8T *pOutTlvBuf)
{
   /* Traceability Properties TLV Composition (IEEE-1588-2008: Table 59):
    * -------------------------------------------------------
    * Field                          Length    Offset
    * ------------------------------------------------------
    * flags:                           1       0
    *    TTRA  (Time Traceable)  bit[4]   mask:0x10
    *    FTRA  (Freq Traceable)  bit[5]   mask:0x20
    * reserved                         1       1
    * ------------------------------------------------------
    * Total Length                             2    */

   OS_MEMSET(pOutTlvBuf, 0x00, ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES);

   zl303xx_PtpV2MgmtTlvBooleanPack(pTraceabilityProperties->timeTraceable,      0x10, (pOutTlvBuf + 0));
   zl303xx_PtpV2MgmtTlvBooleanPack(pTraceabilityProperties->frequencyTraceable, 0x20, (pOutTlvBuf + 0));

   return ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES;

}  /* END zl303xx_PtpV2MgmtTlvTraceabilityPropertiesPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvTimescalePropertiesUnpack

  Details:
   Unpacks a Timescale Properties TLV into a compatible data type for local use.

  Parameters:
   [in]   pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out]  pTimescaleProperties     Pointer to the unpacked Timescale Properties structure.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvTimescalePropertiesUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_TimescaleProperties *pTimescaleProperties)
{
   /* UTC Properties TLV Composition (IEEE-1588-2008: Table 60):
    * -------------------------------------------------------
    * Field                          Length    Offset
    * ------------------------------------------------------
    * flags:                            1       0
    *    PTP   (PTP Timescale)   bit[3]   mask:0x08
    * timeSource                        1       1
    * ------------------------------------------------------
    * Total Length                              2    */

   (void)zl303xx_PtpV2MgmtTlvBooleanUnpack((pInTlvBuf + 0), 0x08, &pTimescaleProperties->ptpTimescale);

   pTimescaleProperties->timeSource = UNPACK_8BIT(pInTlvBuf + 1);

   return ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES;

}  /* END zl303xx_PtpV2MgmtTlvTimescalePropertiesUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvTimescalePropertiesPack

  Details:
   Packs a Timescale Properties TLV into a compatible data type for local use.

  Parameters:
   [in]   pTimescaleProperties     Pointer to the Timescale Properties structure to be packed.

   [out]  pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvTimescalePropertiesPack(
      zl303xx_TimescaleProperties *pTimescaleProperties,
      Uint8T *pOutTlvBuf)
{
   OS_MEMSET(pOutTlvBuf, 0x00, ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES);

   (void)zl303xx_PtpV2MgmtTlvBooleanPack(pTimescaleProperties->ptpTimescale, 0x08, (pOutTlvBuf + 0));

   PACK_8BIT((pOutTlvBuf + 1), pTimescaleProperties->timeSource);

   return ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES;
}  /* END zl303xx_PtpV2MgmtTlvTimescalePropertiesPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvUnicastNegEnUnpack

  Details:
   Unpacks a Unicast Negotiation Enable Management TLV.

  Parameters:
   [in]  pInTlvBuf   Pointer to the buffer containing the unpacked TLV.

   [out] bUniNegEn   Pointer to a Unicast Negotiation Enable return value.

  Return Value:
   Uint16T  Size of the unpacked TLV on success
            0 otherwise

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvUnicastNegEnUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_BooleanE *bUniNegEn)
{
   /* UNICAST NEGOTIATION_ENABLE TLV Composition (IEEE-1588-2008: Table 77):
    * -------------------------------------------------------
    * Field                          Length    Offset
    * ------------------------------------------------------
    * flags:                           1       0
    *    EN  (Unicast Enabled)   bit[0]   mask:0x01
    * reserved                         1       1
    * ------------------------------------------------------
    * Total Length                             2    */

   /* The Unicast Negotiation Enable bit is the lowest bit of the first byte. */
   zl303xx_PtpV2MgmtTlvBooleanUnpack((pInTlvBuf + 0), 0x01, bUniNegEn);

   /* Even though data is only contained in a single byte, the TLV is defined
    * as being 2 bytes in length (1 byte is padding to keep it EVEN).  */
   return ZL303XX_PTP_V2_MGMT_TLV_UNICAST_NEG_EN_LEN;
}  /* END zl303xx_PtpV2MgmtTlvUnicastNegEnUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvUnicastNegEnPack

  Details:
   Packs a Unicast Negotiation Enable value into a Management TLV.

  Parameters:
   [in]  bUniNegEn   Pointer to a Unicast Negotiation Enable value.

   [out] pOutTlvBuf  Pointer to the buffer containing the packed TLV.

  Return Value:
   Uint16T  Size of the packed TLV on success
            0 otherwise

*******************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvUnicastNegEnPack(
      zl303xx_BooleanE *bUniNegEn,
      Uint8T *pOutTlvBuf)
{
   /* UNICAST NEGOTIATION_ENABLE TLV Composition (IEEE-1588-2008: Table 77):
    * -------------------------------------------------------
    * Field                          Length    Offset
    * ------------------------------------------------------
    * flags:                           1       0
    *    EN  (Unicast Enabled)   bit[0]   mask:0x01
    * reserved                         1       1
    * ------------------------------------------------------
    * Total Length                             2    */

   /* Clear the entire buffer so we know all settings. */
   OS_MEMSET(pOutTlvBuf, 0x00, ZL303XX_PTP_V2_MGMT_TLV_UNICAST_NEG_EN_LEN);

   /* The Unicast Negotiation Enable bit is the lowest bit of the first byte. */
   zl303xx_PtpV2MgmtTlvBooleanPack(*bUniNegEn, 0x01, (pOutTlvBuf + 0));

   /* Even though data is only contained in a single byte, the TLV is defined
    * as being 2 bytes in length (1 byte is padding to keep it EVEN).  */
   return ZL303XX_PTP_V2_MGMT_TLV_UNICAST_NEG_EN_LEN;
}  /* END zl303xx_PtpV2MgmtTlvUnicastNegEnPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvUnicastMasterMaxTableSizeUnpack

  Details:
   Unpacks a Unicast Master Max Table Size TLV into a compatible data type for local use.

  Parameters:
   [in]   pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out]  pUnicastMasterMaxTableSize     Pointer to the unpacked Uint8 Unicast Master Max Table Size.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvUnicastMasterMaxTableSizeUnpack(
      Uint8T  *pInTlvBuf,
      Uint16T *pUnicastMasterMaxTableSize)
{
   *pUnicastMasterMaxTableSize = UNPACK_16BIT(pInTlvBuf + 0);

   return ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES;
}  /* END zl303xx_PtpV2MgmtTlvUnicastMasterMaxTableSizeUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvUnicastMasterMaxTableSizePack

  Details:
   Packs a Unicast Master Max Table Size TLV into a compatible data type for local use.

  Parameters:
   [in]   pUnicastMasterMaxTableSize        Pointer to the Uint8 Unicast Master Max Table Size to be packed.

   [out]  pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvUnicastMasterMaxTableSizePack(
      Uint16T *pUnicastMasterMaxTableSize,
      Uint8T  *pOutTlvBuf)
{
   OS_MEMSET(pOutTlvBuf, 0x00, ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES);

   PACK_16BIT((pOutTlvBuf + 0), *pUnicastMasterMaxTableSize);

   return ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES;
}  /* END zl303xx_PtpV2MgmtTlvUnicastMasterMaxTableSizePack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvAcceptableMasterMaxTableSizeUnpack

  Details:
   Unpacks a Acceptable Master Max Table Size TLV into a compatible data type for local use.

  Parameters:
   [in]   pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out]  pAcceptableMasterMaxTableSize     Pointer to the unpacked Uint8 Acceptable Master Max Table Size.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvAcceptableMasterMaxTableSizeUnpack(
      Uint8T  *pInTlvBuf,
      Uint16T *pAcceptableMasterMaxTableSize)
{
   *pAcceptableMasterMaxTableSize = UNPACK_16BIT((pInTlvBuf + 0));

   return ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES;
}  /* END zl303xx_PtpV2MgmtTlvAcceptableMasterMaxTableSizeUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvAcceptableMasterMaxTableSizePack

  Details:
   Packs a Acceptable Master Max Table Size TLV into a compatible data type for local use.

  Parameters:
   [in]   pAcceptableMasterMaxTableSize        Pointer to the Uint8 Acceptable Master Max Table Size to be packed.

   [out]  pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvAcceptableMasterMaxTableSizePack(
      Uint16T *pAcceptableMasterMaxTableSize,
      Uint8T  *pOutTlvBuf)
{
   OS_MEMSET(pOutTlvBuf, 0x00, ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES);

   PACK_16BIT((pOutTlvBuf + 0), *pAcceptableMasterMaxTableSize);

   return ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES;
}  /* END zl303xx_PtpV2MgmtTlvAcceptableMasterMaxTableSizePack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvAcceptableMasterTableEnableUnpack

  Details:
   Unpacks a Acceptable Master Table Enable TLV into a compatible data type for local use.

  Parameters:
   [in]   pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out]  bAcceptableMasterTableEnable     Pointer to the unpacked Uint8 Acceptable Master Table Enable.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvAcceptableMasterTableEnableUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_BooleanE *bAcceptableMasterTableEnable)
{
   (void)zl303xx_PtpV2MgmtTlvBooleanUnpack((pInTlvBuf + 0), 0x01, bAcceptableMasterTableEnable);

   return ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES;
}  /* END zl303xx_PtpV2MgmtTlvAcceptableMasterTableEnableUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvAcceptableMasterTableEnablePack

  Details:
   Packs a Acceptable Master Table Enable TLV into a compatible data type for local use.

  Parameters:
   [in]   bAcceptableMasterTableEnable        Pointer to the Uint8 Acceptable Master Table Enable to be packed.

   [out]  pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvAcceptableMasterTableEnablePack(
      zl303xx_BooleanE *bAcceptableMasterTableEnable,
      Uint8T *pOutTlvBuf)
{
   OS_MEMSET(pOutTlvBuf, 0x00, ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES);

   zl303xx_PtpV2MgmtTlvBooleanPack(*bAcceptableMasterTableEnable, 0x01, (pOutTlvBuf + 0));

   return ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES;
}  /* END zl303xx_PtpV2MgmtTlvAcceptableMasterTableEnablePack */


/*

  Function Name:
    zl303xx_PtpV2MgmtTlvDelayMechanismUnpack

  Details:
   Unpacks a Delay Mechanism TLV into a compatible data type for local use.

  Parameters:
   [in]   pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out]  pDelayMechanism     Pointer to the unpacked Uint8 Delay Mechanism.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvDelayMechanismUnpack(
      Uint8T *pInTlvBuf,
      Uint8T *pDelayMechanism)
{
   return zl303xx_PtpV2MgmtTlvSingleByteTlvUnpack(pInTlvBuf, pDelayMechanism);
}  /* END zl303xx_PtpV2MgmtTlvDelayMechanismUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvDelayMechanismPack

  Details:
   Packs a Delay Mechanism TLV into a compatible data type for local use.

  Parameters:
   [in]   pDelayMechanism        Pointer to the Uint8 Delay Mechanism to be packed.

   [out]  pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvDelayMechanismPack(
      Uint8T *pDelayMechanism,
      Uint8T *pOutTlvBuf)
{
   return zl303xx_PtpV2MgmtTlvSingleByteTlvPack(*pDelayMechanism, pOutTlvBuf);
}  /* END zl303xx_PtpV2MgmtTlvDelayMechanismPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvLogMinPdelayReqIntervalUnpack

  Details:
   Unpacks a Log Min Pdelay Req Interval TLV into a compatible data type for local use.

  Parameters:
   [in]   pInTlvBuf      Pointer to the data portion of the input TLV buffer.

   [out]  pLogMinPdelayReqInterval     Pointer to the unpacked Uint8 Log Min Pdelay Req Interval.

  Return Value:
   Uint16T  Number of bytes unpacked from the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvLogMinPdelayReqIntervalUnpack(
      Uint8T *pInTlvBuf,
      Uint8T *pLogMinPdelayReqInterval)
{
   return zl303xx_PtpV2MgmtTlvSingleByteTlvUnpack(pInTlvBuf, pLogMinPdelayReqInterval);
}  /* END zl303xx_PtpV2MgmtTlvLogMinPdelayReqIntervalUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvLogMinPdelayReqIntervalPack

  Details:
   Packs a Log Min Pdelay Req Interval TLV into a compatible data type for local use.

  Parameters:
   [in]   pLogMinPdelayReqInterval        Pointer to the Uint8 Log Min Pdelay Req Interval to be packed.

   [out]  pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes packed into the TLV buffer on success.
            0 otherwise

  Notes:
   Assumes the input pointers are valid.

*******************************************************************************/

Uint16T zl303xx_PtpV2MgmtTlvLogMinPdelayReqIntervalPack(
      Uint8T *pLogMinPdelayReqInterval,
      Uint8T *pOutTlvBuf)
{
   return zl303xx_PtpV2MgmtTlvSingleByteTlvPack(*pLogMinPdelayReqInterval, pOutTlvBuf);
}  /* END zl303xx_PtpV2MgmtTlvLogMinPdelayReqIntervalPack */


/*

  Function Name:
    zl303xx_PtpV2MgmtTlvErrorPack

  Details:
   Packs the contents of a management error TLV

  Parameters:
       pOutTlvBuf    Pointer to the beginning of the TLV buffer in which to
                          construct the error message. This will include both
                          the TLV header and data portions. (This points after
                          the standard Management Message data and any response).
       mgmtErrorId   The error value to return.
       mgmtTlvId     The managementID that caused the error.
       displayMsg    Pointer to a message string to be displayed.

  Return Value:
   the number of TLV message bytes added by this function

 *****************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvErrorPack(
      Uint8T *pOutTlvBuf,
      zl303xx_ManagementErrorIdE mgmtErrorId,
      zl303xx_PtpV2MgmtTlvIdE mgmtTlvId,
      const char *displayMsg)
{
   Uint16T displayDataLength = 0;
   Uint16T tlvDataLength = 0;

   /* Management ERROR TLV Composition (IEEE-1588-2008: Table 71):
    * -------------------------------------------------------
    * Field             Length    Offset
    * ------------------------------------------------------
    * tlvType              2        0
    * lengthField          2        2
    * mgmtErrorId          2        4
    * mgmtTlvId            2        6
    * RESERVED             4        8
    * displayData          N        12
    * pad                  M        12 + N
    * ------------------------------------------------------
    * Total Length                  12 + N + M(0/1)    */

   /* tlvType */
   PACK_16BIT((pOutTlvBuf + 0), PTP_TLV_MANAGEMENT_ERROR_STATUS);

   /* The length is stored at (pOutTlvBuf + 2) and is added later when the
    * length is known */

   /* ManagementErrorID */
   PACK_16BIT((pOutTlvBuf + 4), mgmtErrorId);

   /* ManagementID */
   PACK_16BIT((pOutTlvBuf + 6), mgmtTlvId);

   /* 4 octets reserved */
   OS_MEMSET((pOutTlvBuf + 8), 0, 4);

   /* Treat the displayMsg as an array of octets. We don't care about the
    * encoding except that it must be NULL terminated and must contain no other
    * NULL octets */
   if (displayMsg != NULL)
   {
      displayDataLength = zl303xx_PtpV2MsgTextPack(displayMsg, (pOutTlvBuf + 12),
                                          ZL303XX_MGMT_TLV_ERROR_DISPLAY_BYTES_MAX);
   }

   /* Calculate the length of the TLV data (includes the mgmtErrorId, mgmtTlvId,
    * reserved field, and displayData. */
   tlvDataLength = 2 + 2 + 4 + displayDataLength;

   /* Add padding byte if necessary to make the packet an even length */
   if (displayDataLength % 2)
   {
      *(Uint8T *)((pOutTlvBuf + 12) + displayDataLength) = 0;
      tlvDataLength++;
   }

   /* Store length into the TLV. Note, this is not strictly required because it
    * is also inserted by the caller function but do it here for completeness */
   zl303xx_PtpV2MgmtTlvTypeLengthIdSet(NULL, &tlvDataLength, NULL, pOutTlvBuf);

   /* The total length of the TLV includes type + length + data */
   return 2 + 2 + tlvDataLength;
}  /* END zl303xx_PtpV2MgmtTlvErrorPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvErrorUnpack

  Details:
   Packs the contents of a management error TLV

  Parameters:
   [in]  pInTlvBuf   Pointer to the beginning of the TLV buffer from which to
                           extract the error message. This will include both
                           the TLV header and data portions. (This points after
                           the standard Management Message data and any response).

   [out] mgmtErrorId    The error value contained in the message.
   [out] mgmtTlvId      The managementID contained in the message.
   [out] displayMsg     The error string associated with the message.

  Return Value:
   the number of TLV message bytes added by this function

 *****************************************************************************/
Uint16T zl303xx_PtpV2MgmtTlvErrorUnpack(
      Uint8T *pInTlvBuf,
      zl303xx_ManagementErrorIdE *mgmtErrorId,
      zl303xx_PtpV2MgmtTlvIdE *mgmtTlvId,
      char *displayMsg)
{
   Uint16T tlvLengthField = 0;
   Uint8T  displayDataLength = 0;

   /* Management ERROR TLV Composition (IEEE-1588-2008: Table 71):
    * -------------------------------------------------------
    * Field             Length    Offset
    * ------------------------------------------------------
    * tlvType              2        0
    * lengthField          2        2
    * mgmtErrorId          2        4
    * mgmtTlvId            2        6
    * RESERVED             4        8
    * displayData          N        12
    * pad                  M        12 + N
    * ------------------------------------------------------
    * Total Length                  12 + N + M(0/1)    */

   /* Extract the length from the received TLV. */
   zl303xx_PtpV2MgmtTlvTypeLengthIdGet(pInTlvBuf, NULL, &tlvLengthField, NULL);

   /* ManagementErrorID */
   *mgmtErrorId = UNPACK_16BIT(pInTlvBuf + 4);

   /* ManagementID */
   *mgmtTlvId = UNPACK_16BIT(pInTlvBuf + 6);

   /* displayData::Length*/
   displayDataLength = pInTlvBuf[12];

   /* displayData::Text*/
   OS_MEMSET(displayMsg, 0x00, displayDataLength+1);
   OS_MEMCPY(displayMsg, (pInTlvBuf + 13), displayDataLength);

   /* The total length of the TLV includes type + length + data */
   return 2 + 2 + tlvLengthField;
}  /* END zl303xx_PtpV2MgmtTlvErrorUnpack */

/*

  Function Name:
   zl303xx_PtpV2MsgTextUnpack

  Details:
   Unpacks a PTP Text formatted item from the supplied buffer into a text string.

  Parameters:
   [in]  pInTextBuf     Pointer to the buffer containing the PTP Text data.
                              The first byte contains the text length while the
                              rest contains the text string.
   [in]  stringLenMax   The maximum length (in bytes) of the outString buffer.

   [out] outString      Pointer to a text buffer contain the unpacked string.

  Return Value:
   The number of bytes consumed from the input buffer.

  Notes:
   If the received string is longer than the permitted length then it will be
   treated as an error and no string will be copied. The returned number of bytes
   consumed will be greater than the field size.

*******************************************************************************/
Uint16T zl303xx_PtpV2MsgTextUnpack(Uint8T *pInTextBuf, char *pOutStr, Uint16T stringLenMax)
{
   /* Set some defaults. */
   Uint8T packedStringLen = 0;
   Uint8T copyStringLen = 0;

   /* Get the length of the received PTP text (the first byte [0]). */
   packedStringLen = *(pInTextBuf + 0);

   /* If the output buffer is of length 0, do nothing but report the TLV field
    * as being processed (effectively, skip the field).  */
   if (stringLenMax != 0)
   {
      /* First, clear the input buffer (ensures NULL termination). */
      OS_MEMSET(pOutStr, 0x00, stringLenMax);

      /* Copy the packed string to the return buffer but limit it to 'stringLenMax -1'
       * bytes (-1 for NULL Termination character) so as not to overflow the buffer. */
      copyStringLen = ((packedStringLen > (stringLenMax-1)) ? (stringLenMax-1) : packedStringLen);

      /* Copy the received PTP text to the output string. */
      OS_MEMCPY(pOutStr, (pInTextBuf + 1), copyStringLen);
   }

   /* Even if the output buffer was smaller than the actual packed string or
    * the output buffer length was '0', return the byte count of the TLV field
    * (length byte + string) so that the pointer to the next field will be
    * incremented correctly by the parent function.  */
   return packedStringLen + 1;
}  /* END zl303xx_PtpV2MsgTextUnpack */

/*

  Function Name:
   zl303xx_PtpV2MsgTextPack

  Details:
   Writes the inputed text string into the supplied buffer in PTP Text format.

  Parameters:
   [in]  pInStr         Pointer to the string (null terminated) to convert
                              to PTP Text format.
   [in]  bufLenMax      The maximum length (in bytes) of the pOutTextBuf buffer.

   [out] pOutTextBuf    Pointer to the output buffer in which to pack the
                              input string in PTP Text format. The first byte
                              contains the text length while the rest of the
                              buffer contains the supplied text string.

  Return Value:
   The number of bytes written to the buffer. If the length of the input string
   exceeds the capacity of the output buffer, the string is truncated.

 *****************************************************************************/
Uint16T zl303xx_PtpV2MsgTextPack(const char *pInStr, Uint8T *pOutTextBuf, Uint16T bufLenMax)
{
   Uint16T inStringLen;
   Uint8T copyStringLen = 0;

   /* Determine the length of the supplied string. (The returned value does not
    * include the null terminating character). */
   inStringLen = (Uint16T)OS_STRLEN(pInStr);

   /* Determine the number of bytes to copy: */
   {
      /* - The string size is limited to the MAX size of the pOutTextBuf (255). */
      if (bufLenMax > 255) { bufLenMax = 255; }
      if (inStringLen > 255)  { inStringLen = 255; }

      /* Copy the smaller of the 2 limits. */
      copyStringLen = ((inStringLen > bufLenMax) ? bufLenMax : inStringLen);
   }

   /* Set the length field (this may be = 0). */
   *(pOutTextBuf + 0) = (Uint8T)copyStringLen;

   /* Copy the input String to the output string PTP Text buffer. */
   OS_MEMCPY((pOutTextBuf + 1), pInStr, copyStringLen);

   /* Includes the length field. */
   return (copyStringLen + 1);
}  /* END zl303xx_PtpV2MsgTextPack */


/****************   STATIC FUNCTION DEFINITIONS   ****************************/

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvBooleanUnpack

  Details:
   Generic routine for extracting a boolean TRUE/FALSE flag from a TLV field.

  Parameters:
   [in]  tlvByte  Pointer to the TLV byte from which to extract the Boolean.
   [in]  bitMask  8-bit mask indicating which bit within the byte contains
                        the boolean value.

   [out] boolVal  Boolean representation of the bit specified.

  Return Value:
   None

  Notes:
   Local routine so pointers are never NULL.

*******************************************************************************/
static void zl303xx_PtpV2MgmtTlvBooleanUnpack(
      Uint8T *tlvByte,
      Uint8T bitMask,
      zl303xx_BooleanE *boolVal)
{
   *boolVal = ZL303XX_INT_TO_BOOL(*tlvByte & bitMask);
}  /* END zl303xx_PtpV2MgmtTlvBooleanUnpack */


/*

  Function Name:
    zl303xx_PtpV2MgmtTlvBooleanPack

  Details:
   Generic routine for inserting a boolean TRUE/FALSE value into a bit field of
   the specified TLV byte.

  Parameters:
   [in]  boolVal  Boolean value to Pack.
   [in]  bitMask  8-bit mask indicating which bit within the byte to insert
                        the boolean value.

   [out] tlvByte  The TLV byte in which the boolean bit is set.

  Return Value:
   None

  Notes:
   Local routine so pointers are never NULL.

*******************************************************************************/
static void zl303xx_PtpV2MgmtTlvBooleanPack(
      zl303xx_BooleanE boolVal,
      Uint8T bitMask,
      Uint8T *tlvByte)
{
   if (boolVal == ZL303XX_TRUE)
   {
      *tlvByte |= bitMask;
   }
   else
   {
      *tlvByte &= ~(bitMask);
   }
}  /* END zl303xx_PtpV2MgmtTlvBooleanPack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvSingleByteTlvUnpack

  Details:
   Generic routine for extracting a TLV field that is a single byte long.

  Parameters:
   [in]  tlvByte  Pointer to the TLV byte from which to extract the value.

   [out] byteVal  The single byte value extracted (passed as an Unsigned
                        representation; Signed values need to be casted by the
                        calling routine).

  Return Value:
   None

  Notes:
   Local routine so pointers are never NULL.

*******************************************************************************/
static Uint16T zl303xx_PtpV2MgmtTlvSingleByteTlvUnpack(
      Uint8T *tlvByte,
      Uint8T *byteVal)
{
   *byteVal = *tlvByte;

   return ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES;
}  /* END zl303xx_PtpV2MgmtTlvSingleByteTlvUnpack */

/*

  Function Name:
    zl303xx_PtpV2MgmtTlvSingleByteTlvPack

  Details:
   Generic routine for inserting a TLV field that is a single byte long.

  Parameters:
   [in]  byteVal  The single byte value to insert into the TLV.

   [out] tlvByte  Pointer to the TLV byte into which to insert the value.

  Return Value:
   None

  Notes:
   Local routine so pointers are never NULL.

*******************************************************************************/
static Uint16T zl303xx_PtpV2MgmtTlvSingleByteTlvPack(
      Uint8T byteVal,
      Uint8T *tlvByte)
{
   /* All single-byte TLVs are actually 2-bytes long with the second byte reserved. */
   OS_MEMSET(tlvByte, 0x00, ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES);

   /* Copy the value into the first byte. */
   *tlvByte = byteVal;

   return ZL303XX_PTP_V2_MGMT_TLV_LENGTH_2_BYTES;
}  /* END zl303xx_PtpV2MgmtTlvSingleByteTlvPack */

/****************   END   ****************************************************/
