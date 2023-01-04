

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Definitions and utility functions for related to all PTP V2 TLV types
*     and structures.
*
******************************************************************************/

#ifndef _ZL303XX_PTP_TLV_TYPES_H_
#define _ZL303XX_PTP_TLV_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"

/* Exported inclusions */
#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpConstants.h"

/*****************   DEFINES   ************************************************/

/* The required lengths of generic TLV structures as well as specific TLVs. */
/***********************/
/****** TLV Data Composition  ******/
/* The following Definitions/Offsets are relative to the start of each TLV,
 * not from the beginning of the PTP packet. */

/* IEEE-1588-2008: Section 5.3.8 */
/* ------------------------------*/
/* Field       Length   Offset   */
/* ------------------------------*/
/* tlvType        2        0     */
/* lengthField    2        2     */
/* valueField     M        4     */
/* ------------------------------*/
/* Total Length: 4 + M           */
#define ZL303XX_PTP_TLV_TYPE_OFFSET            (Uint16T)(0)
#define ZL303XX_PTP_TLV_LENGTH_OFFSET          (Uint16T)(2)
#define ZL303XX_PTP_TLV_VALUE_OFFSET           (Uint16T)(4)

/* TLV minimum length (2 bytes for TYPE; 2 bytes for LENGTH). */
#define ZL303XX_PTP_TLV_BASE_LEN               ZL303XX_PTP_TLV_VALUE_OFFSET

/* Organization Extension TLV headers have the following structure:
   2 bytes for type (ORG_EXT)
   2 bytes for length
   3 bytes for organizationId
   3 bytes for organizationSubType  */
#define ZL303XX_PTP_TLV_ORG_EXT_HEADER_LEN     (Uint16T)(10)
#define ZL303XX_PTP_TLV_ORG_EXT_OUI_OFFSET     (Uint16T)(4)
#define ZL303XX_PTP_TLV_ORG_EXT_SUBTYPE_OFFSET (Uint16T)(7)

/* Alternate Time Offset Indicator TLV have the following minimum structure:
   2 bytes for type (ALT_TIME_OFFSET_INDC)
   2 bytes for length
   1 byte  for keyField
   4 bytes for currentOffset
   4 bytes for jumpSeconds
   6 bytes for timeOfNextJump
   displayName field of variable length. */
#define ZL303XX_PTP_TLV_ALT_TIME_OFFSET_MIN_LEN         (Uint16T)(19)
#define ZL303XX_PTP_TLV_ALT_TIME_OFFSET_DISPLAY_MAX_LEN (Uint16T)(10)

#define ZL303XX_PTP_ALT_TIME_OFFSET_TBL_ENTRIES         (Uint16T)(5)

/* Macro to convert a 3-byte OUI to a 32-bit value (with MSB = 0). */
#define ZL303XX_PTP_TLV_OUI_TO_32BIT(oui8)  (Uint32T)((oui8[0] << 16) | (oui8[1] << 8) | (oui8[2] << 0))

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/** Implementation specific data type for Organization Extension Header TLV. */
typedef struct zl303xx_PtpTlvOrgExtHdrS
{
   /** Organization Id: the OUI assigned to the vendor or standards organization. */
   Uint8T organizationId[ZL303XX_PTP_NODE_OUI_LEN];

   /** Organization Sub-Type: defines a sub-type within the scope of the
    *  organizationId field (values are assigned by the vendor or standards
    *  organization identified by the organizationId). */
   Uint8T organizationSubType[ZL303XX_PTP_NODE_OUI_SUBTYPE_LEN];
} zl303xx_PtpTlvOrgExtHdrS;

/** Implementation specific data type for Alternate Time Offset
 *  Indicator operation. */
typedef struct zl303xx_PtpTlvAltTimeOffsetS
{
   /** Key Field: indicates the alternate timescale reported in this TLV entity.
    *  A value of 0xFF indicates the entry is empty.  */
   Uint8T keyField;

   /** Enabled Field: indicates if the alternate timescale is enabled. */
   zl303xx_BooleanE enabled;

   /** Current Offset: offset of the alternate time, in seconds, from the node’s
    *  time. The alternate time is the sum of this value and the node’s time. */
   Sint32T currentOffset;

   /** Jump Seconds: the size of the next discontinuity, in seconds, of the
    *  alternate time. A value of zero indicates that no discontinuity is
    *  expected. A positive value indicates that the discontinuity will cause
    *  the currentOffset of the alternate time to increase. */
   Sint32T jumpSeconds;

   /** Time of Next Jump: the value of the seconds portion of the transmitting
    *  node’s time at the time that the next discontinuity will occur. The
    *  discontinuity occurs at the start of the second indicated by the value
    *  of timeOfNextJump. */
   Uint64S timeOfNextJump;

   /** Display Name: the text name of the alternate timescale.  */
   char displayName[ZL303XX_PTP_TLV_ALT_TIME_OFFSET_DISPLAY_MAX_LEN];
} zl303xx_PtpTlvAltTimeOffsetS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/


/*****************   OTHER PTP TLV UTILITY FUNCTION DECLARATIONS   ************/


#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
