

/*****************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Implementation of utility functions for interfacing to custom Management
*      TLV handlers.
*
******************************************************************************/

/****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Error.h"

#include "zl303xx_PtpV2TlvApi.h"
#include "zl303xx_PtpV2TlvCustom.h"
#include "zl303xx_PtpDatatypes.h"
#include "zl303xx_PtpV2MgmtTlvHandle.h"
#include "zl303xx_PtpFunctionExport.h"
#include "zl303xx_PtpInternal.h"

/****************   DEFINES     **********************************************/

/****************   DATA TYPES   *********************************************/

/****************   STATIC FUNCTION DECLARATIONS   ***************************/

/****************   STATIC GLOBAL VARIABLES   ********************************/

/****************   EXPORTED GLOBAL VARIABLES   ******************************/

/****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* zl303xx_PtpV2TlvCustomHandler */
/** 

   Attempts to processes custom implemented TLVs by searching through the
   Clock's list of configured custom TLV handlers. Triggered when a normal
   TLV message handler cannot resolve a received TLV message.

   The user can set the handler of non-standard Management TLVs via the
   zl303xx_PtpTlvCustomHandlerSet() routine (see API for details).

  Parameters:
   [in]  pPtpCustomTbl  Pointer to the Table of configured Custom Handlers.
   [in]  streamHandle   Handle to the Stream processing the Custom TLV (passed
                              to the custom handler).
   [in]  actionField    The management action requested (GET, SET, etc.)
   [in]  pInTlvBuf      The beginning of the received TLV; starts at the
                              tlvType field and includes the lengthField and any
                              associated data.

   [out] pPtpMsgRx      Pointer to the received PTP message structure (all
                              header and message data fields are parsed)
   [out] pOutTlvBuf     The output TLV buffer; includes tlvType, lengthField
                              and any associated data, packed in the format that
                              will be sent on-the-wire.

  Return Value:
   Uint32T     Length of the prepared outTlvBuf (output TLV buffer) on success.
               Otherwise, one of the defined error codes if an error occurs.

  Notes:
   Assumes all input pointers are valid (for this local routine).

*******************************************************************************/
Uint16T zl303xx_PtpV2TlvCustomHandler(
      zl303xx_PtpV2TlvCustomS *pPtpCustomTbl,
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf,
      Uint8T *pOutTlvBuf)
{
   /* Default return value is 'NO CUSTOM HANDLER FOUND' */
   Uint16T outBufTotalLength = ZL303XX_PTP_TLV_CUSTOM_RSP_NO_HANDLER;

   /* Data from the custom TLV message. */
   Uint16T tlvType = 0;
   Uint16T tlvLength = 0;
   Uint32T tlvId = 0;

   /* Table counter. */
   Uint16T tblEntry = 0;

   /* Flag whether a Custom handler was found. */
   zl303xx_BooleanE tlvProcessed = ZL303XX_FALSE;

   /* Get the tlvType and tlvId fields. */
   zl303xx_PtpTlvTypeLengthGet(pInTlvBuf, &tlvType, &tlvLength);

   if (tlvType == PTP_TLV_MANAGEMENT)
   {
      Uint16T managementId;
      zl303xx_PtpV2MgmtTlvTypeLengthIdGet(pInTlvBuf, NULL, NULL, &managementId);
      tlvId = managementId;
   }
   else if (tlvType == PTP_TLV_VENDOR_EXTENSION)
   {
      /* The OUI is only 24-bit but for look-up, convert it to 32-bit. */
      zl303xx_PtpTlvConvert3ByteFieldTo32Bit(
                     (pInTlvBuf + ZL303XX_PTP_TLV_ORG_EXT_OUI_OFFSET), &tlvId);
   }
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
            "zl303xx_PtpV2TlvCustomHandler: ERROR: tlvType(%u) not handled",
            tlvType, 0,0,0,0,0);
      return ZL303XX_PTP_TLV_CUSTOM_RSP_NO_HANDLER;
   }

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
         "zl303xx_PtpV2TlvCustomHandler: Stream(%u),tlvType(%u),tlvId(0x%X)",
         streamHandle, tlvType, tlvId, 0,0,0);

   /* Search for a custom handler entry for the Clock and execute. */
   for (tblEntry = 0; tblEntry < ZL303XX_PTP_TLV_CUSTOM_HANDLERS_MAX; tblEntry++)
   {
      if ((pPtpCustomTbl[tblEntry].tlvType == tlvType) &&
          (pPtpCustomTbl[tblEntry].tlvId == tlvId))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
               "      >>>> Custom Handler Found...", 0,0,0,0,0,0);

         outBufTotalLength = pPtpCustomTbl[tblEntry].fnTlvHandler(
                                 streamHandle, pPtpMsgRx, pInTlvBuf, pOutTlvBuf);
         tlvProcessed = ZL303XX_TRUE;
         break;
      }
   }

   /* If we couldn't process the TLV, notify the user. */
   if (tlvProcessed == ZL303XX_FALSE)
   {
      zl303xx_ClockStreamS *pPtpStream = NULL;

      /* Get a pointer to the stream structure. */
      if (zl303xx_PtpStreamDataGet(streamHandle, &pPtpStream) == ZL303XX_OK)
      {
         zl303xx_PtpEventUnknownTlv(pPtpStream->portData, pPtpStream, pPtpMsgRx, tlvLength, pInTlvBuf);
      }
   }

   return outBufTotalLength;
}

/* zl303xx_PtpTlvCustomHandlerExists */
/** 

   Searches the specified custom TLV handler table for the tlvType and Id.

  Parameters:
   [in]  pPtpCustomTbl  Pointer to the Table of configured Custom Handlers.
   [in]  tlvType        tlvType value to look for. Currently only MGMT or
                              ORG_EXT TLVs can have customized handlers.
   [in]  tlvId          The TLV ID to look for.
                              If tlvType == MGMT then tlvId is managementId as
                                 a 32-bit value.
                              If tlvType == ORG_EXT then tlvId is 3-byte OUI as
                                 a 32-bit value (MSB = 0).
  Return Value:
   zl303xx_BooleanE    ZL303XX_TRUE: Entry exists.
                     ZL303XX_FALSE: Otherwise.

  Notes:
   Assumes all input pointers are valid (for this local routine).

*******************************************************************************/
zl303xx_BooleanE zl303xx_PtpTlvCustomHandlerExists(
      zl303xx_PtpV2TlvCustomS *pPtpCustomTbl,
      Uint16T tlvType,
      Uint32T tlvId)
{
   /* Default return value is 'NO CUSTOM HANDLER FOUND' */
   zl303xx_BooleanE bEntryExists = ZL303XX_FALSE;

   /* Table counter. */
   Uint16T tblEntry = 0;

   /* Currently only MGMT or ORG_EXT TLVs can have customized handlers. */
   if ((tlvType == PTP_TLV_MANAGEMENT) ||
       (tlvType == PTP_TLV_VENDOR_EXTENSION))
   {
      /* Search for a custom handler entry for the Clock and execute. */
      for (tblEntry = 0; tblEntry < ZL303XX_PTP_TLV_CUSTOM_HANDLERS_MAX; tblEntry++)
      {
         if ((pPtpCustomTbl[tblEntry].tlvType == tlvType) &&
             (pPtpCustomTbl[tblEntry].tlvId == tlvId))
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 3,
                  "      >>>> Custom Handler Found...", 0,0,0,0,0,0);

            bEntryExists = ZL303XX_TRUE;
            break;
         }
      }
   }

   return bEntryExists;
}

/* zl303xx_PtpTlvCustomHandlerSet */
/** 

   API to allow the user to add a Custom TLV Handler routine for a specified
   PTP Clock.

  Parameters:
   [in]  clockHandle    Handle to the Clock to attach the Custom TLV.
   [in]  tlvType        The tlvType of the handler.
   [in]  tlvId          The custom TLV ID.
   [in]  fnTlvHandler   Pointer to the custom TLV handler of type
                              zl303xx_PtpV2TlvCustomHandlerT.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_PtpTlvCustomHandlerSet(
      Uint32T clockHandle,
      Uint16T tlvType,
      Uint32T tlvId,
      zl303xx_PtpV2TlvCustomHandlerT fnTlvHandler)
{
   zlStatusE status = ZL303XX_OK;

   /* PTP Clock structure containing the table. */
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Table indexes. */
   Uint16T tblEntry = 0;
   Uint16T emptyEntry = ZL303XX_PTP_TLV_CUSTOM_HANDLERS_MAX;

   /* Get a pointer to the clock structure. */
   if ((status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock)) != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
            "zl303xx_PtpTlvCustomHandlerSet: Invalid Clock (%d)",
            clockHandle, 0,0,0,0,0);
   }

   /* Look for the first empty entry in the table. */
   else
   {
      /* An empty entry is defined as having:
       * - tlvType == 0x0000  (Reserved as per Table 34 in the standard).
       * - tlvId   == 0x0000  (NULL MANAGEMENT value).  */
      /* Also check for a duplicate entry. */
      for (tblEntry = 0; tblEntry < ZL303XX_PTP_TLV_CUSTOM_HANDLERS_MAX; tblEntry++)
      {
         if ((pPtpClock->customTlvTable[tblEntry].tlvType == tlvType) &&
             (pPtpClock->customTlvTable[tblEntry].tlvId == tlvId))
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
                  "zl303xx_PtpTlvCustomHandlerDel: Clock(%u),Entry($u) duplicate:: tlvType(%u),tlvId(%u)",
                  clockHandle, tblEntry, tlvType, tlvId, 0,0);

            status = ZL303XX_TABLE_ENTRY_DUPLICATE;
            break;
         }

         else if ((pPtpClock->customTlvTable[tblEntry].tlvType == 0x0000) &&
                  (pPtpClock->customTlvTable[tblEntry].tlvId   == 0x0000) &&
                  (emptyEntry == ZL303XX_PTP_TLV_CUSTOM_HANDLERS_MAX))
         {
            /* First Empty Entry available. */
            emptyEntry = tblEntry;
         }
      }
   }

   if (status == ZL303XX_OK)
   {
      /* Make sure an empty row was found. */
      if (emptyEntry >= ZL303XX_PTP_TLV_CUSTOM_HANDLERS_MAX)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
               "zl303xx_PtpTlvCustomHandlerDel: Clock(%u) Table full:: tlvType(%u),tlvId(%u)",
               clockHandle, tlvType, tlvId, 0,0,0);

         status = ZL303XX_TABLE_FULL;
      }

      /* Add the Handler to the Clock Handler list. */
      else
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 2,
               "zl303xx_PtpTlvCustomHandlerDel: Clock(%u),Entry($u) Set:: tlvType(%u),tlvId(%u)",
               clockHandle, emptyEntry, tlvType, tlvId, 0,0);

         pPtpClock->customTlvTable[emptyEntry].tlvType = tlvType;
         pPtpClock->customTlvTable[emptyEntry].tlvId   = tlvId;
         pPtpClock->customTlvTable[emptyEntry].fnTlvHandler = fnTlvHandler;
      }
   }

   return status;
}  /* END zl303xx_PtpTlvCustomHandlerSet */

/* zl303xx_PtpTlvCustomHandlerDel */
/** 

   API to allow the user to delete a Custom TLV Handler routine that was
   previously added PTP Clock.

  Parameters:
   [in]  clockHandle    Handle to the Clock from which to delete the Custom TLV.
   [in]  tlvType        The tlvType of the handler.
   [in]  tlvId          The custom TLV ID.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_PtpTlvCustomHandlerDel(
      Uint32T clockHandle,
      Uint16T tlvType,
      Uint32T tlvId)
{
   zlStatusE status = ZL303XX_OK;

   /* PTP Clock structure containing the table. */
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Table indexes. */
   Uint16T tblEntry = 0;

   /* Get a pointer to the clock structure. */
   if ((status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock)) != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
            "zl303xx_PtpTlvCustomHandlerDel: Invalid Clock (%d)",
            clockHandle, 0,0,0,0,0);
   }

   /* Look for the entry in the table. */
   else
   {
      /* If the entry is not found, return ZL303XX_OK anyway since the entry
       * is not in the table, which was the goal of the API call. */
      /* Also, check the entire table, not just the first instance. */
      for (tblEntry = 0; tblEntry < ZL303XX_PTP_TLV_CUSTOM_HANDLERS_MAX; tblEntry++)
      {
         if ((pPtpClock->customTlvTable[tblEntry].tlvType == tlvType) &&
             (pPtpClock->customTlvTable[tblEntry].tlvId   == tlvId))
         {
            pPtpClock->customTlvTable[tblEntry].tlvType = 0x0000;
            pPtpClock->customTlvTable[tblEntry].tlvId   = 0x0000;

            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_TLV, 1,
                  "zl303xx_PtpTlvCustomHandlerDel: Clock(%u),Entry($u) removed:: tlvType(%u),tlvId(%u)",
                  clockHandle, tblEntry, tlvType, tlvId, 0,0);
         }
      }
   }

   return status;
}  /* END zl303xx_PtpTlvCustomHandlerDel */

/****************   STATIC FUNCTION DEFINITIONS   ****************************/

/****************   END   ****************************************************/
