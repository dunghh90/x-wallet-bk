

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Definitions for parsing (packing/unpacking) and handling Management TLVs.
*     These are covered under Clause 15 of the IEEE-1588-2008 Standard with
*     associated managementId definitions listed in Table 40.
*
*     The point of this header is to provide a structured, automated method of
*     processing standard defined management TLVs.
*
******************************************************************************/

#ifndef _ZL303XX_PTP_V2_MGMT_TLV_DEF_H_
#define _ZL303XX_PTP_V2_MGMT_TLV_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"

#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpConstants.h"
#include "zl303xx_PtpV2MgmtTlvHandle.h"  /* For the Handling Routines. */
#include "zl303xx_PtpDatatypes.h"

/*****************   DEFINES   ************************************************/

/*** GROUP and INDEX definitions ***/
/***********************************/
/* Define some macros for breaking down the managementId. */
/* For a 16-bit managementId: b5432109876543210
 *                     Group: b1110000000000000
 *                     Index: b0001111111111111       */
#define ZL303XX_PTP_MGMT_TLV_GRP(mgmtId)     (Uint16T)(((mgmtId)>>13)&(0x0007))
#define ZL303XX_PTP_MGMT_TLV_INDEX(mgmtId)   (Uint16T)((mgmtId)&(0x1FFF))

/* Set a MAX entry value for each group. Right now,
 * PTP2_MGMT_ALTERNATE_TIME_OFFSET_PROPERTIES (0x2021) is the largest entry
 * in the ZL303XX_PTP_MGMT_TLV_GRP_OC_BC Group so set the array large enough
 * for this (Index would convert to 33 decimal).   */
#define ZL303XX_PTP_MGMT_TLV_GRP_ENTRY_MAX   0x21

/*** TARGETED PTP OBJECT definitions ***/
/***************************************/
/* Refer to IEEE-1588-2008, Tables 36 & 40. */
#define ZL303XX_PTP_MGMT_TLV_TARGET_OBJ_NONE      (Uint16T)(0)
#define ZL303XX_PTP_MGMT_TLV_TARGET_OBJ_CLOCK     (Uint16T)(1 << 0)
#define ZL303XX_PTP_MGMT_TLV_TARGET_OBJ_PORT      (Uint16T)(1 << 1)
#define ZL303XX_PTP_MGMT_TLV_TARGET_OBJ_ALL       (Uint16T)((1 << 2) - 1)

/*** SUPPORTED ACTIONS definitions ***/
/*************************************/
/* Base Defines for supported actionField values. */
#define ZL303XX_PTP_MGMT_TLV_ACTION_GET   (Uint16T)(1 << PTP_MANAGEMENT_ACTION_GET)
#define ZL303XX_PTP_MGMT_TLV_ACTION_SET   (Uint16T)(1 << PTP_MANAGEMENT_ACTION_SET)
#define ZL303XX_PTP_MGMT_TLV_ACTION_RSP   (Uint16T)(1 << PTP_MANAGEMENT_ACTION_RSP)
#define ZL303XX_PTP_MGMT_TLV_ACTION_CMD   (Uint16T)(1 << PTP_MANAGEMENT_ACTION_CMD)
#define ZL303XX_PTP_MGMT_TLV_ACTION_ACK   (Uint16T)(1 << PTP_MANAGEMENT_ACTION_ACK)
#define ZL303XX_PTP_MGMT_TLV_ACTION_ALL   (Uint16T)((1 << PTP_MANAGEMENT_ACTION_MAX) - 1)
#define ZL303XX_PTP_MGMT_TLV_ACTION_NONE  (Uint16T)(0)

/* Sub-Types to keep definitions in array simpler. */
#define ZL303XX_PTP_MGMT_TLV_ACTION_G_R   (ZL303XX_PTP_MGMT_TLV_ACTION_GET | ZL303XX_PTP_MGMT_TLV_ACTION_RSP)

/* MACRO for comparing the actionField value to the supported operations mask. */
#define ZL303XX_PTP_MGMT_TLV_ACTION_SUPPORTED(action,mask)  (((1 << (action))&(mask)) ? ZL303XX_TRUE : ZL303XX_FALSE)

/*** RECEIVED Length definitions ***/
/***********************************/
/* The TLV lengthField includes the size of the managementId field (2-bytes)
 * so every MANAGEMENT TLV must be at least 2-bytes long. */
#define ZL303XX_PTP_MGMT_TLV_RX_LEN_MIN   2

/*****************   DATA TYPES   *********************************************/

/* For Automated processing, the Management TLVs in the standard can be broken
 * into several groupings (defined by the upper 3 bits of the managementId).  */
typedef enum zl303xx_PtpV2MgmtTlvGroupE
{
   ZL303XX_PTP_MGMT_TLV_GRP_ALL_NODES = 0,
   ZL303XX_PTP_MGMT_TLV_GRP_OC_BC,
   ZL303XX_PTP_MGMT_TLV_GRP_TC,
   ZL303XX_PTP_MGMT_TLV_GRP_OC_BC_TC,

   ZL303XX_PTP_MGMT_TLV_GRP_ID_MAX
} zl303xx_PtpV2MgmtTlvGroupE;

/* Define some function binding types to handle the processing. */
typedef Uint16T (*zl303xx_PtpV2MgmtTlvProcessT)(
      zl303xx_PortDataS *pPtpPort,
      zl303xx_PtpV2MgmtActionE *actionField,
      Uint8T *pInTlvBuf,
      Uint8T *pOutTlvBuf);
typedef Uint16T (*zl303xx_PtpV2MgmtTlvPackT)(
      void *pInMgmtTlvDataS,     /* Pointer to the TLV Data (or Structure) to Pack. */
      Uint8T *pOutTlvDataBuf);   /* TLV Data Buffer to reply. */
typedef Uint16T (*zl303xx_PtpV2MgmtTlvUnPackT)(
      Uint8T *pInTlvDataBuf,     /* TLV Buffer to parse */
      void *pOutMgmtTlvDataS);   /* Pointer to the TLV Data (or Structure) to UnPack. */


/*****************   DATA STRUCTURES   ****************************************/

/* Define a TLV Definition / Handler structure so that TLVs can be managed
 * automatically.  */
typedef struct zl303xx_PtpV2MgmtTlvDefS
{
   /* The Full Management ID for this TLV */
   Uint16T managementId;
   char descr[32];

   /* The PTP Object for which this TLV is targeted. */
   Uint16T targetObj;

   /* Which management Actions to support. */
   /* If set to '0' then the TLV is not supported. */
   Uint16T actionsMask;

   /* Minimum or Expected data length. */
   Uint16T rxTlvLength;
   /*Uint16T txDataLength; */

   /* Routine to Handle the TLV. */
   zl303xx_PtpV2MgmtTlvProcessT fnProcess;

   /* Routines to Pack and Unpack the TLV Data. */
   zl303xx_PtpV2MgmtTlvPackT fnPackData;
   zl303xx_PtpV2MgmtTlvUnPackT fnUnPackData;

} zl303xx_PtpV2MgmtTlvDefS;


/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/* Define an array of TLV definitions with handlers, etc. */
static zl303xx_PtpV2MgmtTlvDefS MgmtTlvDef[ZL303XX_PTP_MGMT_TLV_GRP_ID_MAX][ZL303XX_PTP_MGMT_TLV_GRP_ENTRY_MAX] =
   {
      /* ALL NODES GROUP */
      {
         { PTP2_MGMT_ID_NULL,
            /* Descr   */  "NULL MANAGEMENT",
            /* Target  */  ZL303XX_PTP_MGMT_TLV_TARGET_OBJ_PORT,
            /* Action  */  ZL303XX_PTP_MGMT_TLV_ACTION_ALL,
            /* RX Len  */  ZL303XX_PTP_MGMT_TLV_RX_LEN_MIN,
            /* Handler */  zl303xx_PtpV2MgmtTlvNullHandle,
            /* Pack    */  NULL,
            /* Unpack  */  NULL},
         { PTP2_MGMT_ID_CLOCK_DESCRIPTION,
            /* Descr   */  "CLOCK_DESCRIPTION",
            /* Target  */  ZL303XX_PTP_MGMT_TLV_TARGET_OBJ_PORT,
            /* Action  */  ZL303XX_PTP_MGMT_TLV_ACTION_G_R,
            /* RX Len  */  ZL303XX_PTP_MGMT_TLV_RX_LEN_MIN,
            /* Handler */  zl303xx_PtpV2MgmtTlvClockDescrHandle,
            /* Pack    */  NULL,
            /* Unpack  */  NULL},
         { PTP2_MGMT_ID_USER_DESCRIPTION,
               "USER_DESCRIPTION",
               /* Target  */  ZL303XX_PTP_MGMT_TLV_TARGET_OBJ_NONE, /* Not implemented yet */
               0x0000,
               -1,
               NULL,
               NULL,
               NULL},
         { PTP2_MGMT_ID_SAVE_IN_NVS,
               "SAVE_IN_NVS",       ZL303XX_PTP_MGMT_TLV_TARGET_OBJ_NONE, 0x0000, -1,   NULL, NULL, NULL},
         { PTP2_MGMT_ID_RESET_NVS,
               "RESET_NVS",         ZL303XX_PTP_MGMT_TLV_TARGET_OBJ_NONE, 0x0000, -1,   NULL, NULL, NULL},
         { PTP2_MGMT_ID_INITIALIZE,
               "INITIALIZE",        ZL303XX_PTP_MGMT_TLV_TARGET_OBJ_NONE, 0x0000, -1,   NULL, NULL, NULL},
         { PTP2_MGMT_ID_FAULT_LOG,
               "FAULT_LOG",
               ZL303XX_PTP_MGMT_TLV_TARGET_OBJ_CLOCK,
               ZL303XX_PTP_MGMT_TLV_ACTION_NONE,
               ZL303XX_PTP_MGMT_TLV_RX_LEN_MIN,
               NULL, NULL, NULL},
         { PTP2_MGMT_ID_FAULT_LOG_RESET,
               "FAULT_LOG_RESET",   ZL303XX_PTP_MGMT_TLV_TARGET_OBJ_NONE, 0x0000, -1,   NULL, NULL, NULL},
      },

      /* OC & BC GROUP */
      {
         { PTP2_MGMT_ID_DEFAULT_DATA_SET,
               "DEFAULT_DATA_SET",  ZL303XX_PTP_MGMT_TLV_TARGET_OBJ_NONE, 0x0000,  -1,   NULL, NULL, NULL},
      },
   };

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

#define zl303xx_PtpV2MgmtTlvIdMigrated(mgmtId) \
   (((ZL303XX_PTP_MGMT_TLV_GRP(mgmtId) < ZL303XX_PTP_MGMT_TLV_GRP_ID_MAX) && \
       (ZL303XX_PTP_MGMT_TLV_INDEX(mgmtId) < ZL303XX_PTP_MGMT_TLV_GRP_ENTRY_MAX) && \
       (MgmtTlvDef[ZL303XX_PTP_MGMT_TLV_GRP(mgmtId)][ZL303XX_PTP_MGMT_TLV_INDEX(mgmtId)].targetObj != ZL303XX_PTP_MGMT_TLV_TARGET_OBJ_NONE)) \
         ? ZL303XX_TRUE \
         :ZL303XX_FALSE)

#define zl303xx_PtpV2MgmtTlvDescr(mgmtId) \
   MgmtTlvDef[ZL303XX_PTP_MGMT_TLV_GRP(mgmtId)][ZL303XX_PTP_MGMT_TLV_INDEX(mgmtId)].descr

#define zl303xx_PtpV2MgmtTlvProcessFuncExists(mgmtId) \
   ((MgmtTlvDef[ZL303XX_PTP_MGMT_TLV_GRP(mgmtId)][ZL303XX_PTP_MGMT_TLV_INDEX(mgmtId)].fnProcess != NULL) \
            ? ZL303XX_TRUE \
            : ZL303XX_FALSE)
#define zl303xx_PtpV2MgmtTlvProcessFunc(mgmtId) \
   MgmtTlvDef[ZL303XX_PTP_MGMT_TLV_GRP(mgmtId)][ZL303XX_PTP_MGMT_TLV_INDEX(mgmtId)].fnProcess

#define zl303xx_PtpV2MgmtTlvTargetObject(mgmtId, ptpObj) \
   ((MgmtTlvDef[ZL303XX_PTP_MGMT_TLV_GRP(mgmtId)][ZL303XX_PTP_MGMT_TLV_INDEX(mgmtId)].targetObj & (ptpObj)) \
         ? ZL303XX_TRUE \
         : ZL303XX_FALSE)

#define zl303xx_PtpV2MgmtTlvIdSupported(mgmtId)   zl303xx_PtpV2MgmtTlvProcessFuncExists(mgmtId)

#define zl303xx_PtpV2MgmtTlvActionSupported(mgmtId, action) \
   (((ZL303XX_PTP_MGMT_TLV_GRP(mgmtId) < ZL303XX_PTP_MGMT_TLV_GRP_ID_MAX) && \
       (ZL303XX_PTP_MGMT_TLV_INDEX(mgmtId) < ZL303XX_PTP_MGMT_TLV_GRP_ENTRY_MAX) && \
       (ZL303XX_PTP_MGMT_TLV_ACTION_SUPPORTED(action, \
             MgmtTlvDef[ZL303XX_PTP_MGMT_TLV_GRP(mgmtId)][ZL303XX_PTP_MGMT_TLV_INDEX(mgmtId)].actionsMask))) \
         ? ZL303XX_TRUE \
         :ZL303XX_FALSE)

#define zl303xx_PtpV2MgmtTlvExpectedRxLength(mgmtId) \
   MgmtTlvDef[ZL303XX_PTP_MGMT_TLV_GRP(mgmtId)][ZL303XX_PTP_MGMT_TLV_INDEX(mgmtId)].rxTlvLength
#define zl303xx_PtpV2MgmtTlvCheckRxLength(mgmtId,rxLength) \
   (((rxLength) == MgmtTlvDef[ZL303XX_PTP_MGMT_TLV_GRP(mgmtId)][ZL303XX_PTP_MGMT_TLV_INDEX(mgmtId)].rxTlvLength) \
         ? ZL303XX_TRUE \
         : ZL303XX_FALSE)

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
