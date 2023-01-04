

/*****************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Module Description:
*     Foreign Master Table Management routines for the PTP protocol.
*
*******************************************************************************/

/****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"
#include "zl303xx_ErrTrace.h"

#include "zl303xx_Ptsf.h"

#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpForeignMasterTbl.h"
#include "zl303xx_PtpG8265p1Api.h"
#include "zl303xx_PtpV2TlvApi.h"


/****************   DEFINES     **********************************************/

/****************   STATIC FUNCTION DECLARATIONS   ***************************/

/** Local FOREIGN MASTER TABLE Utility Routines.  */
/**************************************************/

/* Adds a new foreign master to the Table (if it does not already exist). */
static zlStatusE zl303xx_PtpFmtEntryAdd(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_PtpV2MsgHeaderS *header,
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PortAddress *pSrcAddr,
      Uint16T *matchIndex);

/* Searches the Foreign Master Table for a specific foreignMasterIdentity on
 * a specific stream. */
static zlStatusE zl303xx_PtpFmtEntrySearch(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_PortIdentity *srcPortIdentity,
      zl303xx_ClockStreamS *pPtpStream,
      Uint16T *matchIndex);

/* Evaluates every entry in the Foreign Master Table to
 * - move any empty records to the end of the table (for optimization)
 * - updates the active record count of the table
 * - performs a Sort on the table to re-evaluate the 'best' entry. */
static zlStatusE zl303xx_PtpFmtAudit(zl303xx_ForeignMasterDS *fmTbl);

/* Sorts the valid entries of a foreign master table. Non-validated entries
 * remain unsorted at the end of the table. */
static zlStatusE zl303xx_PtpFmtSort(zl303xx_ForeignMasterDS *fmTbl);

/* Test 2 Foreign Master Records using the BMC comparison routine and swaps them
 * if they are not in 'best' order. (Output is entryA > entryB). */
static zl303xx_BooleanE zl303xx_PtpFmtEntriesOrder(
      zl303xx_ForeignMasterRecordS *entryA,
      zl303xx_ForeignMasterRecordS *entryB);
static zl303xx_BooleanE zl303xx_PtpTelecomFmtEntriesOrder(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ForeignMasterRecordS *entryA,
      zl303xx_ForeignMasterRecordS *entryB);

static void zl303xx_PtpFmtEntriesSwap(
      zl303xx_ForeignMasterRecordS *recA,
      zl303xx_ForeignMasterRecordS *recB);

static void zl303xx_PtpFmtRecordDisqualify(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ForeignMasterRecordS *rec);

static zl303xx_BooleanE zl303xx_PtpFmtRecordWtrExpired(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ForeignMasterRecordS *rec);
static void zl303xx_PtpFmtRecordWtrStop(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ForeignMasterRecordS *rec);
static void zl303xx_PtpFmtRecordBestMasterOnce(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ForeignMasterRecordS *rec);

/* Routine to determine if 2 data sets are equal. */
zl303xx_BooleanE zl303xx_PtpEqualComparisonData(
      zl303xx_PtpBmcCompareDataS *dataSetA,
      zl303xx_PtpBmcCompareDataS *dataSetB);
/* Routine to collect BMCA comparison data from an Announce message. */
static void zl303xx_PtpConvertAnncMsgToCompareData(
      zl303xx_PtpV2MsgHeaderS *header,
      zl303xx_PtpV2MsgAnnounceS *announce,
      zl303xx_PortIdentity *rxPortIdentity,
      zl303xx_PtpBmcCompareDataS *compareData);

/* Debug Routine to display the Foreign Master Table.
 * (Purposely used CAPS to resemble other TRACE routines. */
static void ZL303XX_TRACE_FOREIGN_TABLE(
      Uint32T modId, Uint16T level,
      const char *label,
      zl303xx_ForeignMasterDS *fmTbl);

/* Local utility to determine if a Foreign Master Table entry matches a
 * specified rxPortIdentity. */
zl303xx_BooleanE zl303xx_PtpFmtEntryMatchesRxPortIdentity(
      zl303xx_ForeignMasterRecordS *fmtEntry,
      zl303xx_PortIdentity *rxPortIdentity);


/****************   STATIC GLOBAL VARIABLES   ********************************/

/****************   EXTERNAL GLOBAL VARIABLES   ******************************/

/****************   EXPORTED GLOBAL VARIABLES   ******************************/

/****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/****************   OVERALL TABLE MANAGEMENT FUNCTION DEFINITIONS   **********/

/* zl303xx_PtpFmtCreate */
/** 

  Details:
   Allocates memory for the specified Foreign Master Table. The size of
   the table is determined by the maxRecords member of the table structure.

  Parameters:
   [in]  fmTbl      Pointer to a Foreign Master Table object.

  Return Value:
     ZL303XX_OK                      Success.
     ZL303XX_INVALID_POINTER         fmTbl is NULL.
     ZL303XX_PARAMETER_INVALID       fmTbl size is 0 or INVALID.
     ZL303XX_MULTIPLE_INIT_ATTEMPT   Table already exists.
     ZL303XX_RTOS_MEMORY_FAIL        Required memory could not be allocated.

 *****************************************************************************/
zlStatusE zl303xx_PtpFmtCreate(zl303xx_ForeignMasterDS *fmTbl)
{
   zlStatusE status = ZL303XX_OK;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1,
            "FMT Initialize: ",
            0,0,0,0,0,0);

   /* Check the table pointer. */
   if (fmTbl == NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1,
            "FMT Create: Invalid Table Pointer.",
            0,0,0,0,0,0);

      status = ZL303XX_INVALID_POINTER;
   }

   /* Upper bound is set by the user.
    * However, the number of records must be 1 or more and the all 1's value
    * (0xFFFF) is reserved as the 'BEST_INVALID' indicator.  */
   else if ((fmTbl->maxRecords == 0) ||
            (fmTbl->maxRecords == ZL303XX_PTP_FMT_INDEX_INVALID))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1,
            "FMT Create: Invalid Table Size %d.",
            fmTbl->maxRecords, 0,0,0,0,0);

      status = ZL303XX_PARAMETER_INVALID;
   }

   /* Make sure the table has not already been allocated. */
   else if (fmTbl->foreign != NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1,
            "FMT Create: Table already exists.", 0,0,0,0,0,0);

      status = ZL303XX_MULTIPLE_INIT_ATTEMPT;
   }

   /* Create the table as specified. */
   else
   {
      /* Allocate memory for the table. */
      fmTbl->foreign = (zl303xx_ForeignMasterRecordS *)OS_CALLOC(fmTbl->maxRecords,
                                                   sizeof(zl303xx_ForeignMasterRecordS));

      /* Verify all is well. */
      if (fmTbl->foreign == NULL)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1,
               "FMT Create: Memory allocation failed.",
               0,0,0,0,0,0);

         status = ZL303XX_RTOS_MEMORY_FAIL;
      }
      else  /* Initialize the table. */
      {
         /* The individual entries in the table were cleared automatically by
          * the CALLOC function during creation.
          * Specifically, the streamData field will be cleared to '0' (NULL). */

         /* Reset the table control parameters. The MAX value remains the same. */
         fmTbl->numActiveRecords = 0;
         fmTbl->bestRecord = ZL303XX_PTP_FMT_INDEX_INVALID;
         fmTbl->tableUpdated = ZL303XX_FALSE;
      }
   }

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1,
         "\tStatus: %d ",
         status, 0,0,0,0,0);

   return status;
}  /* END zl303xx_PtpFmtCreate */

/* zl303xx_PtpFmtDelete */
/** 

  Details:
   Frees allocated memory for the specified Foreign Master Table and sets the
   control members of the table to their un-initialized values.

  Parameters:
   [in]  fmTbl      Pointer to a Foreign Master Table object.

  Return Value:
     ZL303XX_OK                      Success.
     ZL303XX_INVALID_POINTER         fmTbl is NULL.

 *****************************************************************************/
zlStatusE zl303xx_PtpFmtDelete(zl303xx_ForeignMasterDS *fmTbl)
{
   zlStatusE status = ZL303XX_OK;

   /* Check the table pointer. */
   if (fmTbl == NULL)
   {
      status = ZL303XX_INVALID_POINTER;

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1,
            "FMT Delete: Invalid Table Pointer. Error %d",
            status, 0,0,0,0,0);
   }

   /* Reset the table. */
   else
   {
      /* Before deleting the table, FREE any memory that was allocated for
       * PATH TRACE entries.   */
      Uint16T fmtRow = 0;
      for (fmtRow = 0; fmtRow < fmTbl->maxRecords; fmtRow++)
      {
         (void)zl303xx_PtpClockPathTraceListUpdate(
               &fmTbl->foreign[fmtRow].pathTraceList, 0, NULL);
      }

      /* FREE the Allocated portion of the table. */
      /* Event if the 'fmTbl->foreign' member is NULL, the FREE will pass.  So,
       * do not bother checking the entry pointer. */
      OS_FREE(fmTbl->foreign);
      fmTbl->foreign = NULL;     /* For good measure. */

      /* Clear the control parameters (leave maxRecords the same). */
      fmTbl->numActiveRecords = 0;
      fmTbl->bestRecord = ZL303XX_PTP_FMT_INDEX_INVALID;
      fmTbl->tableUpdated = ZL303XX_FALSE;
   }

   return status;
}  /* END zl303xx_PtpFmtDelete */

/* zl303xx_PtpFmtFlush */
/** 
 *
   Function to flush entries of a PTP Foreign Master Table.

  Parameters:
   [in]  fmTbl             Pointer to a Foreign Master Table object.
   [in]  rxPortIdentity    Pointer to the portIdentity to filter the table.
                                 If NULL: Flush all entries (no filter).
                                 If All 1's: Flush all entries (no filter).
                                 Else: only flush entries that match portIdentity.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   fmTbl is NULL (rxPortIdentity may be NULL).

*******************************************************************************/
zlStatusE zl303xx_PtpFmtFlush(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_PortIdentity *rxPortIdentity)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ForeignMasterRecordS * fmRec;
   Uint32T recordCount = 0;

   /* Check the table pointer. */
   if (fmTbl == NULL)
   {
      status = ZL303XX_INVALID_POINTER;

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1,
            "zl303xx_PtpFmtFlush: Invalid Table Pointer. Error %d",
            status, 0,0,0,0,0);
   }

   /* Flush the table. */
   else
   {
      /* Zero all MATCHING records in the table */
      fmRec = &fmTbl->foreign[recordCount];

      while ((fmRec != NULL) &&
             (recordCount < fmTbl->maxRecords))
      {
         /* FLUSH matching entries. */
         if (zl303xx_PtpFmtEntryMatchesRxPortIdentity(fmRec, rxPortIdentity) == ZL303XX_TRUE)
         {
            OS_MEMSET(fmRec, 0, sizeof(zl303xx_ForeignMasterRecordS));

            /* Decrement the number of active records. */
            fmTbl->numActiveRecords--;
         }

         ++recordCount;
         fmRec = &fmTbl->foreign[recordCount];
      }

      /* Reset the table control parameters. The MAX value remains the same. */
      fmTbl->bestRecord = ZL303XX_PTP_FMT_INDEX_INVALID;
      fmTbl->tableUpdated = ZL303XX_FALSE;
   }

   return status;
}  /* END zl303xx_PtpFmtFlush */

/* zl303xx_PtpFmtDisqualifyAll */
/** 

   Disqualifies all records in the Foreign Master Table associated with the
   clock and all Tables associated with the clock's ports.

  Parameters:
   [in]   pClock  Pointer to the clock to disqualify.

*******************************************************************************/
zlStatusE zl303xx_PtpFmtDisqualifyAll(zl303xx_PtpClockS *pClock)
{
   zlStatusE status = ZL303XX_OK;

   zl303xx_PortDataS *pPort = NULL;
   zl303xx_ClockStreamS *pStream = NULL;

   /* Disqualify every entry in all foreign master tables. */
   ZL303XX_PTP_FOR_EACH_PORT(&pClock->portList, pPort)
   {
      ZL303XX_PTP_FOR_EACH_STREAM(&pPort->streamList, pStream)
      {
         (void)zl303xx_PtpFmtEntryDisqualify(&pClock->foreignMasterDS, pStream);
      }
   }

   return status;
}  /* zl303xx_PtpFmtDisqualifyAll */

/* zl303xx_PtpFmtQualifyStreams */
/** 

   Updates the qualFmtEntry flag for all streams on a clock. If a stream has at
   least one associated FMT entry that is valid, this flag is set to TRUE.

  Parameters:
   [in]   pClock  Clock to qualify streams for.

*******************************************************************************/
zlStatusE zl303xx_PtpFmtQualifyStreams(zl303xx_PtpClockS *pClock)
{
   zlStatusE status = ZL303XX_OK;

   zl303xx_PortDataS *port;
   zl303xx_ClockStreamS *stream;
   Uint32T count;
   zl303xx_ForeignMasterRecordS *rec;


   ZL303XX_PTP_FOR_EACH_PORT(&pClock->portList, port)
   {
      ZL303XX_PTP_FOR_EACH_STREAM(&port->streamList, stream)
      {
         stream->qualFmtEntry = ZL303XX_FALSE;
      }
   }

   for (count = 0; count < pClock->foreignMasterDS.numActiveRecords; count++)
   {
      rec = &pClock->foreignMasterDS.foreign[count];

      if ((rec->valid == ZL303XX_TRUE) &&
          (rec->streamData != NULL))
      {
         rec->streamData->qualFmtEntry = ZL303XX_TRUE;
      }
   }

   return status;
}  /* zl303xx_PtpFmtQualifyStreams */

/* zl303xx_PtpFmtSelectedGet */
/** 

   Returns the index of the selected foreign master entry when in non-revertive
   mode.

  Parameters:
   [in]      fmTbl  Pointer to the clock's Foreign Master Table.

   [in,out]  pAddr  Pointer to the port address of the selected master. If no
                         entry in the FMT matches this address, the address of
                         the best master is returned in this struct. If no best
                         master exists, this struct is OS_MEMSET() to 0.

  Return Value:  Uint16T  Index into the FMT.

*******************************************************************************/
Uint16T zl303xx_PtpFmtSelectedGet(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_PortAddress *pAddr)
{
   Uint16T count;

   /* Search the FMT for the entry matching the selected port address. */
   for (count = 0; count < fmTbl->numActiveRecords; count++)
   {
      if ((fmTbl->foreign[count].streamData) &&
          (zl303xx_PtpComparePortAddress(&fmTbl->foreign[count].portAddress, pAddr) == 0))
      {
         /* Return index of matching entry. */
         return count;
      }
   }

   /* The search was done on the clock's FMT, and no match was found. */
   /* Use the bestRecord's portAddress as the selected address. */
   if (fmTbl->bestRecord != ZL303XX_PTP_FMT_INDEX_INVALID)
   {
      *pAddr = fmTbl->foreign[fmTbl->bestRecord].portAddress;
   }
   /* No best record, so invalidate the selected port address. */
   else
   {
      OS_MEMSET(pAddr, 0, sizeof(*pAddr));
   }

   return fmTbl->bestRecord;
}  /* zl303xx_PtpFmtSelectedGet */

/****************   INDIVIDUAL TABLE ENTRY MANAGEMENT FUNCTIONS   *************/

/*

  Function Name:
   zl303xx_PtpFmtEntryUpdate

  Details:
   Updates a foreign master record based on the received message. If the entry
   does not already exist, it is automatically added to the list of visible
   masters (for PTP v2).

  Parameters:
   [in]  fmTbl        Pointer to the Foreign Master Table of the clock
                            associated with the received message & stream.
   [in]  header       Pointer to the unpacked header contents of the
                            received message.
   [in]  announce     Pointer to the unpacked announce message contents of
                            the received message.
   [in]  pPtpStream   Pointer to the clock stream instance structure on which
                            the message was received.
   [in]  pSrcAddr     Source port address on the master that the ANNOUNCE
                            message was transmitted from.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   fmTbl, pPtpStream or compareData is NULL.
     ZL303XX_TABLE_FULL        This is a new entry that could not be added to
                                    the table because it is already full.

 *****************************************************************************/
zlStatusE zl303xx_PtpFmtEntryUpdate(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_PtpV2MsgHeaderS *header,
      zl303xx_PtpV2MsgAnnounceS *announce,
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PortAddress *pSrcAddr)
{
   zlStatusE status = ZL303XX_OK;

   /* Variables for the matching entry in the FMT table. */
   Uint16T matchIndex;
   zl303xx_ForeignMasterRecordS *tblEntry = NULL;

   /* Check the input pointers. */
   if ((fmTbl == NULL) || (header == NULL) || (announce == NULL) ||
       (pPtpStream == NULL) || (pSrcAddr == NULL))
   {
      status = ZL303XX_INVALID_POINTER;

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1,
            "FMT Update Entry: Invalid Pointer. Error %d",
            status,0,0,0,0,0);
   }

   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 3,
            "FMT Update Entry: Stream %u:  ",
            pPtpStream->clockStreamHandle, 0,0,0,0,0);

      /* This routine first returns the index of any duplicate entry. If none
       * exists, it adds the message as a new foreign record and returns that
       * index (if the table is not already full). */
      status = zl303xx_PtpFmtEntryAdd(fmTbl, header, pPtpStream, pSrcAddr, &matchIndex);
   }

   /* Evaluate the result of the ADD routine. */
   /*******************************************/
   /* Entry already exist OR it was brand new. */
   if ((status == ZL303XX_TABLE_ENTRY_DUPLICATE) ||   /* - Most common case first. */
       (status == ZL303XX_OK))
   {
      /* Save the old data to determine if an update event needs to be generated. */
      zl303xx_PtpBmcCompareDataS oldCompareData;

      /* We have the entry or a space so point at it. */
      tblEntry = &fmTbl->foreign[matchIndex];

      /* Save the old data for an update-check. */
      oldCompareData = tblEntry->compareData;

      /* Increment the counter for the current window. */
      tblEntry->msgCount.current++;

      /* Copy the latest received data to the record. */
      zl303xx_PtpConvertAnncMsgToCompareData(
            header, announce, &pPtpStream->portData->portDS.portIdentity,
            &tblEntry->compareData);
      tblEntry->alternateMaster = getFlag(header->flagField, PTP2_FLAGS_ALTERNATE_MASTER);

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 3,
            "FMT Update Entry: Total Records(%d): Stream %u: current count %d  ",
            fmTbl->numActiveRecords,
            pPtpStream->clockStreamHandle,
            tblEntry->msgCount.current, 0, 0,0);

      /* Determine if a BMC cycle needs to be performed. During steady-state
       * operation, no change in the message will have occurred (except the
       * time, sequence number, etc.). New entries will not yet be valid
       * and were added to the end of the table so no BMC (Sort) is necessary. */
      if (status == ZL303XX_TABLE_ENTRY_DUPLICATE)
      {
         if(zl303xx_PtpEqualComparisonData(&oldCompareData, &tblEntry->compareData) != ZL303XX_TRUE)
         {
            /* Force a BMCA Update if configured. */
            if (pPtpStream->portData->clock->config.bmca.updateTrigger == ZL303XX_BMCA_TRIGGER_ON_RX_ANNC_DELTA)
            {
               zl303xx_PtpClockBmcaUpdateInitiate(pPtpStream->portData->clock);
            }
            else
            {
               /* For a Stream Data change, this is the same as a Keep-Alive Event.
                * The Keep-Alive counter is reset within the Event generator. */
               zl303xx_PtpEventStreamKeepAlive(pPtpStream);
            }
         }

         /* If no data has changed in the received message, the sort will execute
          * quickly so perform a full sort.    */
         status = zl303xx_PtpFmtSort(fmTbl);
      }
      else  /* Re-Set the return status since both of these cases are OK. */
      {
         /* A new record was created. */
         ZL303XX_TRACE_V2_PORT_ID(ZL303XX_MOD_ID_PTP_FMT, 2,
               "FMT NEW Entry: identity ",
               &tblEntry->compareData.foreignPortIdentity);

         /* Upload the entire table through the notification interface. */
         fmTbl->tableUpdated = ZL303XX_TRUE;

         status = ZL303XX_OK;
      }
   }

   else if (status == ZL303XX_TABLE_FULL)
   {
      /* The Entry was NEW but the FMT is FULL. */
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1,
            "FMT Update Entry ERROR %d: Records(%d): Stream %u: Table Full - Discarding Announce: ",
            status,
            fmTbl->numActiveRecords,
            pPtpStream->clockStreamHandle, 0,0,0);
      ZL303XX_TRACE_V2_PORT_ID(ZL303XX_MOD_ID_PTP_FMT, 1,
            "FMT Discarded Entry: identity ",
            &header->sourcePortIdentity);
   }

   return status;
}  /* END zl303xx_PtpFmtEntryUpdate */

/*zl303xx_PtpFmtEntryDelete */
/** 

   Explicitly deletes all FMT entries associated with a stream. This function
   is only called when a stream is deleted.

  Parameters:
   [in,out]  fmTbl   Foreign Master Table to search and remove from.
   [in]      stream  Stream to search for.

*******************************************************************************/
zlStatusE zl303xx_PtpFmtEntryDelete(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ClockStreamS *stream)
{
   Uint16T matchIndex;

   /* In the case of multicast, there may be multiple FMT entries associated
    * with a stream. So, loop until the search function no longer matches. */
   while (zl303xx_PtpFmtEntrySearch(
             fmTbl, NULL, stream, &matchIndex) == ZL303XX_TABLE_ENTRY_DUPLICATE)
   {
      /* Setting streamData to NULL will cause the audit function to clear this entry. */
      fmTbl->foreign[matchIndex].streamData = NULL;
   }

   /* Upload the entire table through the notification interface. */
   fmTbl->tableUpdated = ZL303XX_TRUE;

   /* Audit the table so all empty entries get removed and numActiveRecords gets updated. */
   return zl303xx_PtpFmtAudit(fmTbl);
}  /* zl303xx_PtpFmtEntryDelete */

/*

  Function Name:
   zl303xx_PtpFmtEntryQualify

  Details:
   At the end of a PTP_FOREIGN_MASTER_TIME_WINDOW period, the qualification
   counters (current & previous windows) and record states for any entries
   associated with this stream are updated.
   If the state of any record changes (to/from valid) then a BMC cycle
   is performed. (This will also occur if any record is removed).

  Parameters:
   [in]  fmTbl        Pointer to the Foreign Master Table of the clock
                            associated with the received message & stream.
   [in]  pPtpStream   Pointer to the clock stream instance structure on which
                            the message was received.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   fmTbl or pPtpStream is NULL.

 *****************************************************************************/
zlStatusE zl303xx_PtpFmtEntryQualify(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ClockStreamS *pPtpStream)
{
   zlStatusE status = ZL303XX_OK;
   Uint16T tblIndex;

   /* 3 conditions under which an Audit of the table will occur.
    * - no 'best' master has been selected (but entries exist).
    * - the qualification window for the current 'best' master has expired.
    * - a previously used table entry is determined to be no longer in use.
    *   (determined below). */
   /* By default the audit is not performed. */
   zl303xx_BooleanE performAudit = ZL303XX_FALSE;
   zl303xx_BooleanE performSort = ZL303XX_FALSE;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 3,
         "zl303xx_PtpFmtEntryQualify : ", 0,0,0,0,0,0);

   /* Check the input pointers. */
   if ((fmTbl == NULL) || (pPtpStream == NULL))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1,
            "FMT Update Entry: Invalid Pointer.",
            0,0,0,0,0,0);

      /* Since this is the only failure scenario, return immediately. */
      return ZL303XX_INVALID_POINTER;
   }

   for(tblIndex = 0; tblIndex < fmTbl->numActiveRecords; ++tblIndex)
   {
      /* Only reset the count for the stream to which the timer is associated. */
      if (fmTbl->foreign[tblIndex].streamData == pPtpStream)
      {
         /* Determine if we should continue to keep this record. */
         /* A few things to consider here:
          * - if the CURRENT count is > 0 then there are messages being received
          *   so keep the record around. If the CURRENT count is < THRESHOLD
          *   then it's possible the entry is brand new OR it will time-out
          *   soon. Either way, keep the entry alive.
          * - if the CURRENT and PREVIOUS counts are both '0' then mark the entry
          *   as not in use anymore by making the streamData NULL. In this case
          *   remove the entry from the table and update the counts, etc.
          */

         /* If  the FMT entry corresponds to the ParentDS entry,
          * AND the ANNOUNCE_RECEIPT_TIMER is still running,
          * AND the maxClockClass requirement is met,
          * AND the Alternate Master flag is still FALSE,
          * then let the ANNOUNCE_RECEIPT_TIMER handle qualifying/disqualifying
          * it (done elsewhere).   */
         if ((zl303xx_PtpV2EqualPortIdentities(
                  &fmTbl->foreign[tblIndex].compareData.foreignPortIdentity,
                  &pPtpStream->portData->clock->parentDS.parentPortIdentity) == ZL303XX_TRUE) &&
             (zl303xx_PtpTimerActive(&pPtpStream->itimer[ANNOUNCE_RECEIPT_TIMER])) &&
             ZL303XX_STREAM_MAX_CLOCK_CLASS_OK(pPtpStream, fmTbl->foreign[tblIndex].compareData.grandmasterClockQuality.clockClass) &&
             (fmTbl->foreign[tblIndex].alternateMaster == ZL303XX_FALSE))
         {
            /* Update window counts. */
            fmTbl->foreign[tblIndex].msgCount.previous = fmTbl->foreign[tblIndex].msgCount.current;
            fmTbl->foreign[tblIndex].msgCount.current = 0;
         }

         else if ((fmTbl->foreign[tblIndex].msgCount.previous == 0) &&
                  (fmTbl->foreign[tblIndex].msgCount.current == 0))
         {
            ZL303XX_TRACE_V2_PORT_ID(ZL303XX_MOD_ID_PTP_FMT, 2,
                  "FMT Removing Entry: Port Identity ",
                  &fmTbl->foreign[tblIndex].compareData.foreignPortIdentity);
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 2,
                  "\tRecord index %d, Stream %u.",
                  tblIndex,
                  pPtpStream->clockStreamHandle, 0,0,0,0);

            /* 2 WINDOWs with no messages so mark the entry as not in use and
             * trigger an AUDIT on the table. */
            zl303xx_PtpFmtRecordDisqualify(fmTbl, &fmTbl->foreign[tblIndex]);
            if (pPtpStream->portData->clock->config.profile != ZL303XX_PTP_PROFILE_TELECOM_G8265_1)
            {
               fmTbl->foreign[tblIndex].streamData = NULL;

               /* Perform a Table Audit to remove the entry from the table and
                * update the active record count. This will also invoke a Sort
                * operation (i.e. BMC cycle). */
               performAudit = ZL303XX_TRUE;
            }
         }
         else  /* Keep the record alive. */
         {
            fmTbl->foreign[tblIndex].msgCount.previous = fmTbl->foreign[tblIndex].msgCount.current;
            fmTbl->foreign[tblIndex].msgCount.current = 0;

            /* If this FMT entry corresponds to a UMT entry that cannot be
             * selected by the BMCA, disqualify it. */
            if ((zl303xx_PtpV2BmcUmtQualified(pPtpStream) == ZL303XX_FALSE) ||
                /* Check that an Appropriate level of ClockClass is received. */
                (ZL303XX_STREAM_MAX_CLOCK_CLASS_OK(pPtpStream, fmTbl->foreign[tblIndex].compareData.grandmasterClockQuality.clockClass) == ZL303XX_FALSE) ||
                /* From an Alternate Master */
                (fmTbl->foreign[tblIndex].alternateMaster == ZL303XX_TRUE))
            {
               if (fmTbl->foreign[tblIndex].valid != ZL303XX_FALSE)
               {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 2,
                        "FMT Invalid Entry: Record index %d, Stream %u: because UMT BMCA disabled",
                        tblIndex,
                        pPtpStream->clockStreamHandle, 0,0,0,0);

                  /* Now that the record is invalidated, SORT the table. */
                  zl303xx_PtpFmtRecordDisqualify(fmTbl, &fmTbl->foreign[tblIndex]);
                  performSort = ZL303XX_TRUE;
               }
            }

            /* If the record has received ample messages, qualify it. */
            else if (fmTbl->foreign[tblIndex].msgCount.previous >= ZL303XX_PTP_DEFAULT_FOREIGN_WINDOW_THRESHOLD)
            {
               /* If the record was previously invalid, SORT the Table to
                * determine if the BEST record has changed.  */
               if (fmTbl->foreign[tblIndex].valid != ZL303XX_TRUE)
               {
                  /* Now that the record is valid, SORT the table. */
                  fmTbl->foreign[tblIndex].valid = ZL303XX_TRUE;
                  performSort = ZL303XX_TRUE;

                  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 2,
                        "FMT Valid Entry: Record index %d, Stream %u:",
                        tblIndex,
                        pPtpStream->clockStreamHandle,
                        0,0,0,0);
               }
            }

            /* Not enough messages received. */
            else
            {
               /* If the record was previously valid but is no longer any
                * good, determine if the BEST record needs to change.  */
               if (fmTbl->foreign[tblIndex].valid != ZL303XX_FALSE)
               {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 2,
                        "FMT Invalid Entry: Record index %d, Stream %u: no messages received",
                        tblIndex,
                        pPtpStream->clockStreamHandle, 0,0,0,0);

                  /* Now that the record is invalid, SORT the table. */
                  zl303xx_PtpFmtRecordDisqualify(fmTbl, &fmTbl->foreign[tblIndex]);
                  performSort = ZL303XX_TRUE;
               }
            }
         }
      }
   }

   /* Perform an Audit (& Sort) if necessary. */
   if (performAudit == ZL303XX_TRUE)
   {
      /* A previous entry was removed so upload the entire table through the notification interface. */
      fmTbl->tableUpdated = ZL303XX_TRUE;

      /* An AUDIT also performs a SORT, so if the SORT flag is currently TRUE
       * then this function will do both.     */
      status = zl303xx_PtpFmtAudit(fmTbl);
   }

   /* Perform a Sort if necessary (and Audit was FALSE) . */
   else if (performSort == ZL303XX_TRUE)
   {
      /* A previous entry was validated or invalidated during this cycle so upload
       * the entire table through the notification interface. */
      fmTbl->tableUpdated = ZL303XX_TRUE;
      status = zl303xx_PtpFmtSort(fmTbl);
   }

   /* Log the final table if required. */
   ZL303XX_TRACE_FOREIGN_TABLE(ZL303XX_MOD_ID_PTP_FMT, 4,
         "zl303xx_PtpFmtEntryQualify", fmTbl);

   return status;
}  /* END zl303xx_PtpFmtEntryQualify */

/*

  Function Name:
   zl303xx_PtpFmtEntryDisqualify

  Details:
   Disqualifies all records of the specified Foreign Master Table associated
   with the given clock stream. Automatically performs a sort operation on the
   table.

  Parameters:
   [in]  fmTbl        Pointer to the Foreign Master Table of the clock
                            associated with the received message & stream.
   [in]  pPtpStream   Pointer to the clock stream instance structure on which
                            the message was received.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   fmTbl or pPtpStream is NULL.

 *****************************************************************************/
zlStatusE zl303xx_PtpFmtEntryDisqualify(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ClockStreamS *pPtpStream)
{
   zlStatusE status = ZL303XX_OK;

   Uint16T tblIndex;

   /* Only re-sort the table if records become invalidated. There may be cases
    * where the record has already been deleted so we end up doing nothing.  */
   zl303xx_BooleanE sortTable = ZL303XX_FALSE;

   /* Check the input pointers. */
   if ((fmTbl == NULL) || (pPtpStream == NULL))
   {
      status = ZL303XX_INVALID_POINTER;

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1,
            "FMT Disqualify Entry: Invalid Pointer. Error %d",
            status, 0,0,0,0,0);
   }

   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 2,
            "FMT Disqualify Entry: All entries in Stream %u:  ",
            pPtpStream->clockStreamHandle, 0,0,0,0,0);

      /* In this case, every entry in the table associated with this stream must
       * be invalid. For multicast streams, there may be several potential
       * servers at the far end.       */
      /* Search the entire length of the table just to be safe. */
      for(tblIndex = 0; tblIndex < fmTbl->maxRecords; tblIndex++)
      {
         if (fmTbl->foreign[tblIndex].streamData == pPtpStream)
         {
            zl303xx_PtpFmtRecordDisqualify(fmTbl, &fmTbl->foreign[tblIndex]);

            /* The table is being altered so re-sort it. */
            sortTable = ZL303XX_TRUE;

            /* Add this as an extra check of the table.
             * Below numActiveRecords, the streamData should be NULL. */
            if (tblIndex >= fmTbl->numActiveRecords)
            {
               ZL303XX_TRACE_ALWAYS("FMT Disqualify Entry: CORRUPTION: Stream %u: Record %d: Count %d  ",
                     pPtpStream->clockStreamHandle,
                     tblIndex,
                     fmTbl->numActiveRecords, 0,0,0);
            }
         }
      }

      /* Re-order the table and evaluate any new best. */
      if (sortTable == ZL303XX_TRUE)
      {
         fmTbl->tableUpdated = ZL303XX_TRUE;
         status = zl303xx_PtpFmtSort(fmTbl);
      }
   }

   return status;
}  /* END zl303xx_PtpFmtEntryDisqualify */

/*

  Function Name:
   zl303xx_PtpFmtEntryDisqualifyForeignPort

  Details:
   Disqualifies all records of the specified Foreign Master Table associated
   with the given clock stream in which the ANNOUNCE message was received from
   the specified parent port.

  Parameters:
   [in]  fmTbl        Pointer to the Foreign Master Table of the clock
                            associated with the received message & stream.
   [in]  pPtpStream   Pointer to the clock stream instance structure on which
                            the message was received.
   [in]  fgnPortId    Port Identity of the transmitting port to disqualify.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   fmTbl, pPtpStream or fgnPortId is NULL.

 *****************************************************************************/
zlStatusE zl303xx_PtpFmtEntryDisqualifyForeignPort(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PortIdentity *fgnPortId)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_BooleanE sortTable = ZL303XX_FALSE;

   Uint16T tblIndex;

   /* Check the input pointers. */
   if ((fmTbl == NULL) || (pPtpStream == NULL) || (fgnPortId == NULL))
   {
      status = ZL303XX_INVALID_POINTER;

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1,
            "FMT Disqualify Foreign Port: Invalid Pointer. Error %d",
            status, 0,0,0,0,0);
   }

   else
   {
      /* Search the entire length of the table and disqualify every instance. */
      for(tblIndex = 0; tblIndex < fmTbl->maxRecords; tblIndex++)
      {
         if ((fmTbl->foreign[tblIndex].streamData == pPtpStream) &&
             (zl303xx_PtpV2EqualPortIdentities(
                  &fmTbl->foreign[tblIndex].compareData.foreignPortIdentity,
                  fgnPortId) == ZL303XX_TRUE))
         {
            zl303xx_PtpFmtRecordDisqualify(fmTbl, &fmTbl->foreign[tblIndex]);
            /* The table is being altered so re-sort it. */
             sortTable = ZL303XX_TRUE;
         }
      }
      /* Re-order the table and evaluate any new best. */
      if (sortTable == ZL303XX_TRUE)
      {
         fmTbl->tableUpdated = ZL303XX_TRUE;
         status = zl303xx_PtpFmtSort(fmTbl);
      }
   }

   return status;
}  /* END zl303xx_PtpFmtEntryDisqualifyForeignPort */

/*

  Function Name:
   zl303xx_PtpFmtEntryPathTraceUpdate

  Details:
   Updates the PATH TRACE information for the specified FMT entry.

  Parameters:
   [in]  fmTbl             Pointer to the Foreign Master Table of the clock
                                 associated with the received message & stream.
   [in]  srcPortIdentity   Identity of the Port that issued the ANNC message.
   [in]  pPtpStream        Pointer to the clock stream instance structure on
                                 which the message was received.
   [in]  pPathTraceTlvBufStart   Start of the Path Trace TLV buffer.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   Any input pointer is NULL.

 *****************************************************************************/
zlStatusE zl303xx_PtpFmtEntryPathTraceUpdate(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_PortIdentity *srcPortIdentity,
      zl303xx_ClockStreamS *pPtpStream,
      Uint8T *pPathTraceTlvBufStart)
{
   zlStatusE status = ZL303XX_OK;

   /* Variables for the matching entry in the FMT table. */
   Uint16T matchIndex;

   /* Check the input pointers. */
   if ((fmTbl == NULL) || (srcPortIdentity == NULL) ||
       (pPtpStream == NULL) || (pPathTraceTlvBufStart == NULL))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1,
            "FMT Update Path Trace: Invalid Pointer. Error %d",
            status, 0,0,0,0,0);

      status = ZL303XX_INVALID_POINTER;
   }
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 3,
            "FMT Update Path Trace: Stream %u:  ",
            pPtpStream->clockStreamHandle, 0,0,0,0,0);

      /* Determine if the entry exists.
       * The matchIndex is reset to INVALID in the Search routine. */
      status = zl303xx_PtpFmtEntrySearch(fmTbl,
                           srcPortIdentity, pPtpStream, &matchIndex);
   }

   /* Make sure the entry existed. */
   if (status == ZL303XX_TABLE_ENTRY_DUPLICATE)
   {
      Uint16T tlvType, tlvLength;

      /* Get the Type & Length of this TLV */
      zl303xx_PtpTlvTypeLengthGet(pPathTraceTlvBufStart, &tlvType, &tlvLength);

      status = zl303xx_PtpClockPathTraceListUpdate(
            &fmTbl->foreign[matchIndex].pathTraceList,
            tlvLength / PTP_V2_CLK_IDENTITY_LENGTH, /* Each entry is 8 bytes long. */
            (zl303xx_ClockIdentity *)(pPathTraceTlvBufStart + ZL303XX_PTP_TLV_BASE_LEN)); /* Entries start after 4 bytes. */
   }

   return status;
}  /* END zl303xx_PtpFmtEntryPathTraceUpdate */

/*

  Function Name:
   zl303xx_PtpFmtPathTraceToActive

  Details:
   Locates the PATH TRACE infor in the FMT for the stream and Parent Port
   Identity specified and copies it to the clock's active Path Trace List.

  Parameters:
   [in]  fmTbl          Pointer to the Foreign Master Table of the clock
                              associated with the stream.
   [in]  pPtpStream     Stream PATH TRACE list to locate.
   [in]  pParentPortId  Identity of the corresponding parent Port that
                              issued the ANNC message.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   Any input pointer is NULL.

 *****************************************************************************/
zlStatusE zl303xx_PtpFmtPathTraceToActive(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PortIdentity *pParentPortId)
{
   zlStatusE status = ZL303XX_OK;

   /* Variables for the matching entry in the FMT table. */
   Uint16T matchIndex;

   /* Check the input pointers. */
   if ((fmTbl == NULL) || (pPtpStream == NULL) || (pParentPortId == NULL))
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1,
            "FMT Update Path Trace Copy: Invalid Pointer.",
            0,0,0,0,0,0);

      status = ZL303XX_INVALID_POINTER;
   }
   else
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 3,
            "FMT Update Path Trace Copy: Stream %u:  ",
            pPtpStream->clockStreamHandle, 0,0,0,0,0);

      /* Determine if the entry exists.
       * The matchIndex is reset to INVALID in the Search routine. */
      status = zl303xx_PtpFmtEntrySearch(fmTbl, pParentPortId, pPtpStream, &matchIndex);
   }

   /* Make sure the entry already existed. */
   if (status == ZL303XX_TABLE_ENTRY_DUPLICATE)
   {
      /* Copy this to the clock's active member. */
      status = zl303xx_PtpClockPathTraceListUpdate(
                     &pPtpStream->portData->clock->pathTraceList,
                     fmTbl->foreign[matchIndex].pathTraceList.listLength,
                     fmTbl->foreign[matchIndex].pathTraceList.pClockIdList);
   }

   return status;
}  /* END zl303xx_PtpFmtPathTraceToActive */

/****************   LOCAL TABLE MANAGEMENT FUNCTION DEFINITIONS   *************/

/*

  Function Name:
   zl303xx_PtpFmtEntryAdd

  Details:
   Initializes a new foreign master record in the specified foreign master table
   (if it does not already exist). Generally, it will be added to the end.

  Parameters:
   [in]  fmTbl        Pointer to the Foreign Master Table of the clock
                            associated with the received message & stream.
   [in]  header       Pointer to the unpacked header contents of the
                            received message.
   [in]  pPtpStream   Pointer to the clock stream instance structure on which
                            the message was received.
   [in]  pSrcAddr     Source port address on the master that the ANNOUNCE
                            message was transmitted from.

   [out] matchIndex   Index to the entry that was added to the table (or the
                            existing index if it already existed, OR
                            ZL303XX_PTP_FMT_INDEX_INVALID if the table is full).

  Return Value:
    ZL303XX_OK                       The entry was added to the table.
    ZL303XX_TABLE_ENTRY_DUPLICATE    If a matching record already exists.
    ZL303XX_TABLE_FULL               The entry could not be added because the table is full.

  Notes:
   This routine is only called by zl303xx_PtpFmtEntryUpdate and
   therefore is not included in the external API list. External users should
   call 'Update' which will call 'Add' if needed.
   Therefore, the input pointers are assumed to be OK.

 *****************************************************************************/
static zlStatusE zl303xx_PtpFmtEntryAdd(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_PtpV2MsgHeaderS *header,
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PortAddress *pSrcAddr,
      Uint16T *matchIndex)
{
   /* Input pointers are not verified locally (see note above). */
   zlStatusE status = ZL303XX_OK;

   /* Determine if the entry already exists.
    * The matchIndex is reset to INVALID in the Search routine. */
   status = zl303xx_PtpFmtEntrySearch(fmTbl, &header->sourcePortIdentity,
                                                   pPtpStream, matchIndex);

   /* If this is a new entry AND space is available, Add it accordingly. */
   /**********************************************************************/
   if (status == ZL303XX_TABLE_ENTRY_NOT_FOUND)
   {
      /* Pointer to the new entry in the FMT. */
      zl303xx_ForeignMasterRecordS *newEntry = &fmTbl->foreign[*matchIndex];

      /* Expect any new records to be added at the end of the FMT so do a
       * check just in case.  */
      if (*matchIndex != fmTbl->numActiveRecords)
      {
         ZL303XX_TRACE_ALWAYS("FMT Add Entry: NOT at end of FMT!! New Record(%d): FMT count %d ",
               *matchIndex, fmTbl->numActiveRecords, 0,0,0,0);
      }

      /* Clear the record to reset all counters, controls and other fields. */
      OS_MEMSET(newEntry, 0x00, sizeof(zl303xx_ForeignMasterRecordS));

      /* Increment the record count in the table. */
      ++fmTbl->numActiveRecords;

      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 2,
            "FMT Add Entry: New Record(%d): Stream %u: current count %d  ",
            fmTbl->numActiveRecords,
            pPtpStream->clockStreamHandle,
            newEntry->msgCount.current, 0,0,0);

      /* Associate the stream with the record. */
      newEntry->streamData = pPtpStream;

      /* Immediately disqualify the new record. */
      zl303xx_PtpFmtRecordDisqualify(fmTbl, newEntry);

      /* Store the source address in the foreign master record. */
      newEntry->portAddress = *pSrcAddr;

      /* Re-Set the return status since this case is OK. */
      status = ZL303XX_OK;
   }

   return status;
}  /* END zl303xx_PtpFmtEntryAdd */

/*

  Function Name:
   zl303xx_PtpFmtEntrySearch

  Details:
   Searches the specified Foreign Master Table for a record with a
   foreignMasterIdentity matching that of the header as well as a stream ID
   of the receive stream (pPtpStream).

  Parameters:
   [in]  fmTbl             Pointer to the Foreign Master Table of a clock.
   [in]  srcPortIdentity   (Optional) Pointer to the up-stream foreign port
                                 identity. If NULL, portIdentity is not used in
                                 the search process (only pPtpStream).
   [in]  pPtpStream        Pointer to the clock stream instance structure on
                                 which a FMT entry was received.

   [out] matchIndex     Index of the first matching entry in the table. If no
                              match is found, then matchIndex will be the first
                              empty entry in the table (if one exists).
                              Otherwise, ZL303XX_PTP_FMT_INDEX_INVALID.

  Return Value:
    ZL303XX_TABLE_ENTRY_DUPLICATE    If a matching record was found.
    ZL303XX_TABLE_ENTRY_NOT_FOUND    If no match was found but empty space exists.
    ZL303XX_TABLE_FULL               If no match was found but the table is Full.

  Notes:
   Input pointers are assumed OK for this local routine.

 *****************************************************************************/
static zlStatusE zl303xx_PtpFmtEntrySearch(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_PortIdentity *srcPortIdentity,
      zl303xx_ClockStreamS *pPtpStream,
      Uint16T *matchIndex)
{
   /* Ignore input pointer & parameter checking for this LOCAL routine. */
   zlStatusE status = ZL303XX_TABLE_ENTRY_DUPLICATE;    /* Most likely case. */

   /* Table index counter. */
   Uint16T tblIndex;

   /* Set the default return values. */
   *matchIndex = ZL303XX_PTP_FMT_INDEX_INVALID;

   /* The table is not empty so search for a match or an empty entry. */
   if (fmTbl->numActiveRecords != 0)
   {
      /* The table is typically sorted in order of 'best' with invalidated
       * entries near the end of the table.  */
      /* Loop through the table for the specified Foreign Master Record. */
      for(tblIndex = 0; tblIndex < fmTbl->numActiveRecords; tblIndex++)
      {
         /* If the ANNOUNCE was received on the same stream as this entry &
          * it has the same far-end identity, then we have a match. */
         if (fmTbl->foreign[tblIndex].streamData == pPtpStream)
         {
            if ((srcPortIdentity == NULL) ||
                (zl303xx_PtpV2EqualPortIdentities(
                    &fmTbl->foreign[tblIndex].compareData.foreignPortIdentity,
                    srcPortIdentity) == ZL303XX_TRUE))
            {
               *matchIndex = tblIndex;
               break;
            }
         }
      }
   }

   /* Evaluate the search... */
   if (*matchIndex == ZL303XX_PTP_FMT_INDEX_INVALID)
   {
      /* No match was found so return an empty entry in case it is needed. */
      /* Always add new entries at the end, the Audit will remove expired ones. */
      if (fmTbl->numActiveRecords < fmTbl->maxRecords)
      {
         *matchIndex = fmTbl->numActiveRecords;
         status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
      }

      /* The entry was not found and there is no space for more! */
      else
      {
         status = ZL303XX_TABLE_FULL;
      }
   }

   return status;
}  /* END zl303xx_PtpFmtEntrySearch */

/*

  Function Name:
   zl303xx_PtpFmtAudit

  Details:
   Evaluates every entry in a foreign master table (up to the maximum table
   size) and removes any empty entries (i.e. marked for removal by the
   zl303xx_PtpFmtEntryQualify routine). It also updates the
   numActiveRecords member of the table and evaluates the BEST record by calling
   the zl303xx_PtpFmtSort routine (if the table is not empty).

  Parameters:
   [in]  fmTbl  Pointer to a foreign master data set (table).

  Return Value:  ZL303XX_OK  Always returns OK.

 *****************************************************************************/
static zlStatusE zl303xx_PtpFmtAudit(zl303xx_ForeignMasterDS *fmTbl)
{
   Uint16T tblIndex;             /* For looping the Table. */
   Uint16T fillEntry = 0;        /* Next entry to fill. */

   /* Check the table pointer. */
   if (fmTbl == NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1,
            "FMT Audit: Invalid Table Pointer.",
            0,0,0,0,0,0);

      /* Return right away since there is no other FAIL scenario. */
      return ZL303XX_INVALID_POINTER;
   }

   /* Check every entry of the table, compress it if necessary and update the
    * number of valid foreign records. */
   for (tblIndex = 0; tblIndex < fmTbl->maxRecords; tblIndex++)
   {
      /* Empty records have NULL streamData pointers. */
      if (fmTbl->foreign[tblIndex].streamData != NULL)
      {
         /* Back-fill this record if any holes were present. If the index values
          * are the same, then no holes have been found yet. */
         if (fillEntry != tblIndex)
         {
            /* Copy the record back the the empty space. */
            fmTbl->foreign[fillEntry] = fmTbl->foreign[tblIndex];

            /* Mark the original as empty. */
            zl303xx_PtpFmtRecordDisqualify(fmTbl, &fmTbl->foreign[tblIndex]);
            fmTbl->foreign[tblIndex].streamData = NULL;
         }

         /* An active record was just found so increase the record count. */
         fillEntry++;
      }

      else  /* Clear the record completely. */
      {
         OS_MEMSET(&fmTbl->foreign[tblIndex], 0x00, sizeof(fmTbl->foreign[tblIndex]));
      }
   }

   /* Update the number of active records in the table. */
   fmTbl->numActiveRecords = fillEntry;   /* ' 0' for empty table */

   /* If the table is empty there is no BEST, otherwise SORT the table to
    * evaluate the current BEST (if any).    */
   if (fmTbl->numActiveRecords == 0)
   {
      fmTbl->bestRecord = ZL303XX_PTP_FMT_INDEX_INVALID;
   }
   else
   {
      /* The SORT function can only return OK */
      (void)zl303xx_PtpFmtSort(fmTbl);
   }

   return ZL303XX_OK;
}  /* END zl303xx_PtpFmtAudit */

/*

  Function Name:
   zl303xx_PtpFmtSort

  Details:
   Sorts the validated entries of the specified Foreign Master Table.
   Non-valid entries remain unsorted at the end of the table.
   This routine should be called whenever:
   - a new record changes to the 'valid' state.
   - an existing record becomes 'invalid' (due to Announce timeout or
     foreign master window violation).

  Parameters:
   [in]  fmTbl    Pointer to the Foreign Master Table to sort.

  Return Value: ZL303XX_OK     Always returns OK.

  Notes:
   Input pointer are assumed OK for this local routine.

 *****************************************************************************/
static zlStatusE zl303xx_PtpFmtSort(zl303xx_ForeignMasterDS *fmTbl)
{
   /* Input pointers are not verified locally (see note above). */
   zlStatusE status = ZL303XX_OK;

   /* Clock pointer used to check if the Telecom Profile is enabled. */
   zl303xx_PtpClockS *pClock;

   /* Checks if a best record is currently chosen */
   Uint16T prevBest = fmTbl->bestRecord;

   /* Clear the 'best' indicator to 'none' until the sort is completed. */
   fmTbl->bestRecord = ZL303XX_PTP_FMT_INDEX_INVALID;

   /* There are multiple records to sort... */
   if (fmTbl->numActiveRecords > 1)
   {
      Uint16T iteration, swapCount;          /* Loop Controls. */
      Uint16T topEntry, lowEntry, tblIndex;  /* Table indexes. */

      /* Grab the clock associated with the first FMT entry. */
      pClock = fmTbl->foreign[0].streamData->portData->clock;

      /* In steady state operation, the entries will already be sorted so do
       * a bubble-sort to reduce effort in the typical scenario.    */
      iteration = 0;    /* maximum of (#records - 1) iterations will be needed. */
      swapCount = 1;    /* So we enter the loop the first time. */
      while (swapCount != 0)
      {
         /* If we ever go through a loop and nothing SWAPs, then the sort is
          * complete and we can exit the loop. */
         swapCount = 0;

         /* Bubble the 'best' records to the top of the table.
          * Start at the end. */
         for((tblIndex = fmTbl->numActiveRecords-1);
             (tblIndex > iteration);
             (tblIndex--))
         {
            /* To make the table indexing more obvious in the code. */
            lowEntry = tblIndex;
            topEntry = tblIndex - 1;

            /* Telecom Profile: Use the comparison function specified in G.8265.1. */
            if (pClock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1)
            {
               if (zl303xx_PtpTelecomFmtEntriesOrder(fmTbl, &fmTbl->foreign[topEntry],
                                                   &fmTbl->foreign[lowEntry]) == ZL303XX_TRUE)
               {
                  swapCount++;
                  /* Reset the WTR timer for a best master which has dropped so
                   * that Expire check will restart it next time */
                  if (topEntry == 0 && prevBest != (Uint16T)-1)
                  {
                     zl303xx_PtpFmtRecordWtrStop(fmTbl, &fmTbl->foreign[lowEntry]);

                     /* Telecom Profile: A new best has been chosen.
                      * Flag the old BEST as having been the best at one point */
                     zl303xx_PtpFmtRecordBestMasterOnce(fmTbl, &fmTbl->foreign[lowEntry]);
                  }
               }
            }
            else
            {
               if (zl303xx_PtpFmtEntriesOrder(&fmTbl->foreign[topEntry],
                                             &fmTbl->foreign[lowEntry]) == ZL303XX_TRUE)
               {
                  swapCount++;
               }
            }

            /* else: The current (lower) is invalid OR worse. */
         }

         iteration++;
      }
   }

   /* Whether numActiveRecords is 0, 1, or more the possible best is always
    * in the first entry (index 0).    */
   if (fmTbl->foreign[0].valid == ZL303XX_TRUE)
   {
      if (zl303xx_PtpFmtRecordWtrExpired(fmTbl, &fmTbl->foreign[0]))
      {
         fmTbl->bestRecord = 0;
      }
   }
   /* Telecom Profile: The only BEST is invalidated. Mark it as having been best before */
   else if (prevBest != (Uint16T)-1)
   {
      zl303xx_PtpFmtRecordBestMasterOnce(fmTbl, &fmTbl->foreign[0]);
   }

   return status;
}  /* END zl303xx_PtpFmtSort */

/*

  Function Name:
   zl303xx_PtpFmtEntriesOrder

  Details:
   Determines the correct order of the 2 records provided using the BMC Data-set
   Comparison routine and swaps the records if necessary.

  Parameters:
   [in]  entryA   First Foreign Master Record to compare.
   [in]  entryB   Second Foreign Master Record to compare.

  Return Value:
     ZL303XX_TRUE     if the records were swapped.
     ZL303XX_FALSE    if the records were not swapped.

  Notes:
   Input pointer are assumed OK for this local routine.

 *****************************************************************************/
static zl303xx_BooleanE zl303xx_PtpFmtEntriesOrder(
      zl303xx_ForeignMasterRecordS *entryA,
      zl303xx_ForeignMasterRecordS *entryB)
{
   zl303xx_BooleanE swapped = ZL303XX_FALSE;
   zl303xx_BooleanE allowA = (entryA->streamData->pUmtEntry)
                              ? entryA->streamData->pUmtEntry->entryConfig.bmcaAllow
                              : ZL303XX_TRUE;
   zl303xx_BooleanE allowB = (entryB->streamData->pUmtEntry)
                              ? entryB->streamData->pUmtEntry->entryConfig.bmcaAllow
                              : ZL303XX_TRUE;

   /* If the lower entry is valid... */
   if ((entryB->valid == ZL303XX_TRUE) && (allowB == ZL303XX_TRUE))
   {
      /* AND */
      /* (1) If the higher entry IS NOT valid OR...
       * (2) If the higher entry IS valid AND the lower is better
       * THEN a swap is needed.  */
      if ((entryA->valid == ZL303XX_FALSE) ||
          (allowA == ZL303XX_FALSE) ||
          (zl303xx_PtpV2BmcDataSetComparison(&entryB->compareData, &entryA->compareData) > 0))
      {
         /* Lower record is valid but the Higher record is NOT
          * OR both are valid and the lower record is better.
          * Both cases require a SWAP. */
         zl303xx_PtpFmtEntriesSwap(entryA, entryB);
         swapped = ZL303XX_TRUE;
      }
   }

   return swapped;
}  /* END zl303xx_PtpFmtEntriesOrder */

/* zl303xx_PtpFmtEntriesSwap */
/** 

   Swaps the data in two foreign master records.

  Parameters:
   [in]  recA   Record to swap with recB.
   [in]  recB   Record to swap with recA.

*******************************************************************************/
static void zl303xx_PtpFmtEntriesSwap(
      zl303xx_ForeignMasterRecordS *recA,
      zl303xx_ForeignMasterRecordS *recB)
{
   zl303xx_ForeignMasterRecordS recTmp = *recA;
   *recA = *recB;
   *recB = recTmp;
}


/*

  Function Name:
    zl303xx_PtpTelecomFmtEntriesOrder

  Details:
   Gathers the data required to run the Telecom Profile BMCA compare function and
   executes it. A stream is always compared to the best stream in a foreign
   master table.

  Parameters:
   [in]   currHandle   Handle to the current stream to evaluate.
   [in]   bestHandle   Handle to the best stream in the foreign master table.

  Return Value:
   zl303xx_BooleanE          ZL303XX_TRUE if a swap occurred. B is better than A.
                           ZL303XX_FALSE if A is better than B. No Swap.

*******************************************************************************/
static zl303xx_BooleanE zl303xx_PtpTelecomFmtEntriesOrder(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ForeignMasterRecordS *entryA,
      zl303xx_ForeignMasterRecordS *entryB)
{
   const char *fnName = "zl303xx_PtpTelecomFmtEntriesOrder";

   if (entryA->streamData->pUmtEntry == NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1, "%s: Error: No UMT entry for stream %lu.",
            fnName, entryA->streamData->clockStreamHandle, 0,0,0,0);

      /* Assume B is valid. */
      zl303xx_PtpFmtEntriesSwap(entryA, entryB);
      return ZL303XX_TRUE;
   }

   if (entryB->streamData->pUmtEntry == NULL)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1, "%s: Error: No UMT entry for stream %lu.",
            fnName, entryA->streamData->clockStreamHandle, 0,0,0,0);
      /* A is valid from previous check. */
      return ZL303XX_FALSE;
   }

   {
      zl303xx_BooleanE allowA = (entryA->streamData->pUmtEntry)
                                 ? entryA->streamData->pUmtEntry->entryConfig.bmcaAllow
                                 : ZL303XX_TRUE;
      zl303xx_BooleanE allowB = (entryB->streamData->pUmtEntry)
                                 ? entryB->streamData->pUmtEntry->entryConfig.bmcaAllow
                                 : ZL303XX_TRUE;

      if ((entryB->valid == ZL303XX_TRUE) && (allowB == ZL303XX_TRUE))
      {
         /* Package the table record for comparison. */
         zl303xx_PtpTelecomCompareDataS telecomDataA;
         zl303xx_PtpTelecomCompareDataS telecomDataB;
         zl303xx_PtpBmcTelecomProfileExtraDataS extraConfig;

         /* entryA */
         telecomDataA.ptsf = zl303xx_PtsfFlagGetAndMask(entryA->streamData->clockStreamHandle, ZL303XX_PTSF_MAIN);
         OS_MEMCPY(telecomDataA.grandmasterIdentity, entryA->compareData.grandmasterIdentity, PTP_V2_CLK_IDENTITY_LENGTH);

         /* From the UMT */
         if (entryA->streamData->pUmtEntry != NULL)
         {
            telecomDataA.priority = entryA->streamData->pUmtEntry->telecom.priority;
            telecomDataA.clockClass = entryA->streamData->pUmtEntry->telecom.clockClass;
            telecomDataA.stepsRemoved = entryA->streamData->pUmtEntry->telecom.stepsRemoved;
         }
         else
         {
            /* G.8265.1 expects Unicast Neg */
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1,
                  "%s: Error: No UMT entry for stream %lu: May be Static Unicast Operation.",
                  fnName, entryA->streamData->clockStreamHandle, 0,0,0,0);
            telecomDataA.priority = 0;
            telecomDataA.clockClass = 0;
            telecomDataA.stepsRemoved = 0;
         }

         /* entryB */
         telecomDataB.ptsf = zl303xx_PtsfFlagGetAndMask(entryB->streamData->clockStreamHandle, ZL303XX_PTSF_MAIN);
         OS_MEMCPY(telecomDataB.grandmasterIdentity, entryB->compareData.grandmasterIdentity, PTP_V2_CLK_IDENTITY_LENGTH);

         /* From the UMT */
         if (entryB->streamData->pUmtEntry != NULL)
         {
            telecomDataB.priority = entryB->streamData->pUmtEntry->telecom.priority;
            telecomDataB.clockClass = entryB->streamData->pUmtEntry->telecom.clockClass;
            telecomDataB.stepsRemoved = entryB->streamData->pUmtEntry->telecom.stepsRemoved;
         }
         else
         {
            /* G.8265.1 expects Unicast Neg */
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_FMT, 1,
                  "%s: Error: No UMT entry for stream %lu: May be Static Unicast Operation.",
                  fnName, entryA->streamData->clockStreamHandle, 0,0,0,0);
            telecomDataB.priority = 0;
            telecomDataB.clockClass = 0;
            telecomDataB.stepsRemoved = 0;
         }

         /* extra config data */
         extraConfig.revertiveEn = entryB->streamData->portData->clock->config.bmca.revertiveEn;

         if ((entryA->valid == ZL303XX_FALSE) ||
             (allowA == ZL303XX_FALSE) ||
             /* Do the Telecom Profile BMCA data comparison. */
             (zl303xx_PtpBmcaTelecomProfileComparison(&telecomDataA, &telecomDataB, &extraConfig) < 0))
         {
            /* Check if WTR applies to better record before swapping */
            if (zl303xx_PtpFmtRecordWtrExpired(fmTbl, entryB) == ZL303XX_TRUE)
            {
               zl303xx_PtpFmtEntriesSwap(entryA, entryB);
               return ZL303XX_TRUE;
            }
         }
      }
   }

   return ZL303XX_FALSE;
}

/* zl303xx_PtpFmtRecordDisqualify */
/** 

   Sets the valid flag for a FMT record to FALSE and performs any other
   necessary actions.

  Parameters:
   [in]  fmTbl    Pointer to a foreign master table.
   [in]  rec      Pointer to a record within fmTbl.

*******************************************************************************/
static void zl303xx_PtpFmtRecordDisqualify(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ForeignMasterRecordS *rec)
{
   zl303xx_BooleanE notifyBp = ZL303XX_FALSE;
   /* Erase any Far-End-Data that this stream has discovered. */
   if (rec->streamData != NULL)
   {
      zl303xx_PtpConvertDefaultDsToParentDs(
            &rec->streamData->portData->clock->config.defaultDS,
            &rec->streamData->farEndParentDS.u.v2);

      OS_MEMSET(&rec->streamData->farEndCurrentDS, 0x00, sizeof(rec->streamData->farEndCurrentDS));
      /* We should be notifying the backplane if the record is
       * associated with an active stream */
      notifyBp = ZL303XX_TRUE;
   }

   rec->valid = ZL303XX_FALSE;

   /* There are many ways to disqualify an entry, but only one way to qualify
    * it: with the stream qualification timer. A situation may occur where the
    * entry gets invalidated, then quickly re-validated by the timer. This will
    * cause unwanted stream state toggling (e.g., LISTENING to SLAVE and back).
    * To prevent this, clear the message counters used to qualify the entry. */
   rec->msgCount.previous = 0;
   rec->msgCount.current = 0;

   /* Telecom Profile: Stop the WTR timer so zl303xx_PtpFmtRecordWtrExpired()
    * will restart it properly. */
   zl303xx_PtpFmtRecordWtrStop(fmTbl, rec);

   /* Notify the backplane of a disqualified record if needed */
   if(notifyBp == ZL303XX_TRUE)
   {
      zl303xx_PtpEventStreamDisqualified(rec->streamData);
   }
}

/* zl303xx_PtpFmtRecordWtrExpired */
/** 

   Only starts the wait to restore timer if Telecom Profile is enabled and this
   is the clock's FMT.

  Parameters:
   [in]   fmTbl   Pointer to a foreign master table.
   [in]   rec     Pointer to a record within fmTbl.

*******************************************************************************/
static zl303xx_BooleanE zl303xx_PtpFmtRecordWtrExpired(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ForeignMasterRecordS *rec)
{
   zl303xx_ClockStreamS *pStream = rec->streamData;

   if ((pStream->portData->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1) &&
       (&pStream->portData->clock->foreignMasterDS == fmTbl) &&
       (pStream->pUmtEntry))
   {
      if (!zl303xx_PtpTelecomWtrIsActive(&pStream->pUmtEntry->telecom))
      {
         zl303xx_PtpTelecomWtrStart(pStream->clockStreamHandle,
                                    &pStream->pUmtEntry->telecom,
                                    pStream->portData->clock->config.telecom.waitToRestoreSec);
      }

      return zl303xx_PtpTelecomWtrExpired(&pStream->pUmtEntry->telecom);
   }

   return ZL303XX_TRUE;
}

/* zl303xx_PtpFmtRecordWtrStop */
/** 

   Stops the wait to restore timer if Telecom Profile is enabled and this is
   the clock's FMT.

  Parameters:
   [in]  fmTbl    Pointer to a foreign master table.
   [in]  rec      Pointer to a record within fmTbl.

*******************************************************************************/
static void zl303xx_PtpFmtRecordWtrStop(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ForeignMasterRecordS *rec)
{
   zl303xx_ClockStreamS *pStream = rec->streamData;

   if ((pStream != NULL) &&
       (pStream->portData->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1) &&
       (&pStream->portData->clock->foreignMasterDS == fmTbl) &&
       (pStream->pUmtEntry))
   {
      zl303xx_PtpTelecomWtrStop(&pStream->pUmtEntry->telecom);
   }
}

/* zl303xx_PtpFmtRecordBestMasterOnce */
/** 

   Marks a stream as having been a best master at least once. Only applicable if
   Telecom Profile is enabled, and this is the clock's FMT.

  Parameters:
   [in]  fmTbl    Pointer to a foreign master table.
   [in]  rec      Pointer to a record within fmTbl.

*******************************************************************************/
static void zl303xx_PtpFmtRecordBestMasterOnce(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ForeignMasterRecordS *rec)
{
   zl303xx_ClockStreamS *pStream = rec->streamData;

   /* Only set the bestMasterOnce flag for the stream if using Telecom Profile
    * and the entry is being validated on the clock's FMT. */
   if ((pStream->portData->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1) &&
       (&pStream->portData->clock->foreignMasterDS == fmTbl) &&
       (pStream->pUmtEntry))
   {
      zl303xx_PtpTelecomStreamBestMasterOnce(&pStream->pUmtEntry->telecom);
   }
}

/*

  Function Name:
    zl303xx_PtpEqualComparisonData

  Details:
   Compares 2 zl303xx_PtpBmcCompareDataS data sets and determines if there has
   been any change.

  Parameters:
   [in]  dataSetA    Data Set A.
   [in]  dataSetB    Data Set B.

  Return Value:
   ZL303XX_TRUE     Both data Sets are the same.
   ZL303XX_FALSE    The data Sets are different.

  Notes:   Since this routine is called internally, it is assumed that all
         input pointers are valid and already verified.

 *****************************************************************************/
zl303xx_BooleanE zl303xx_PtpEqualComparisonData(
      zl303xx_PtpBmcCompareDataS *dataSetA,
      zl303xx_PtpBmcCompareDataS *dataSetB)
{

   if ((dataSetA->grandmasterPriority1 != dataSetB->grandmasterPriority1) ||
       (dataSetA->grandmasterPriority2 != dataSetB->grandmasterPriority2) ||
       (dataSetA->grandmasterClockQuality.clockClass != dataSetB->grandmasterClockQuality.clockClass) ||
       (dataSetA->grandmasterClockQuality.clockAccuracy != dataSetB->grandmasterClockQuality.clockAccuracy) ||
       (dataSetA->grandmasterClockQuality.offsetScaledLogVariance != dataSetB->grandmasterClockQuality.offsetScaledLogVariance) ||
       (dataSetA->stepsRemoved != dataSetB->stepsRemoved))
   {
      return ZL303XX_FALSE;
   }
   else
   {
      return ZL303XX_TRUE;
   }
}  /* zl303xx_PtpEqualComparisonData */


/*

  Function Name:
    zl303xx_PtpConvertAnncMsgToCompareData

  Details:
   Collects the necessary data fields from a received ANNOUNCE message to be
   used at some point to make the BMC comparison. Other data from the message
   is kept since it is required for the clock data sets if the potential server
   becomes the master.

  Parameters:
   [in]   header          PTP header structure from the received message
   [in]   announce        ANNOUNCE structure from the received message
   [in]   rxPortIdentity  Identity of the port this Announce was received on.

   [out]  compareData     Pointer to the BMC comparison data structure to
                               data-fill from this received message.

  Return Value:
   None

  Notes:   Since this routine is called internally, it is assumed that all
         input pointers are valid and already verified.

 *****************************************************************************/
static void zl303xx_PtpConvertAnncMsgToCompareData(
      zl303xx_PtpV2MsgHeaderS *header,
      zl303xx_PtpV2MsgAnnounceS *announce,
      zl303xx_PortIdentity *rxPortIdentity,
      zl303xx_PtpBmcCompareDataS *compareData)
{
   /* Get the necessary Announce fields. */
   compareData->grandmasterPriority1 = announce->grandmasterPriority1;
   compareData->grandmasterPriority2 = announce->grandmasterPriority2;
   OS_MEMCPY(compareData->grandmasterIdentity,
          announce->grandmasterIdentity,
          PTP_V2_CLK_IDENTITY_LENGTH);
   compareData->grandmasterClockQuality = announce->grandmasterClockQuality;

   /* Current Data Set. */
   compareData->stepsRemoved = announce->stepsRemoved + 1;

   /* Time Properties. */
   /* Update the Streams's Time Properties data set from the received message. */
   compareData->timeProperties.currentUtcOffset = announce->currentUtcOffset;
   compareData->timeProperties.timeSource = announce->timeSource;
   compareData->timeProperties.currentUtcOffsetValid = getFlag(header->flagField, PTP2_FLAGS_CURRENT_UTC_OFFSET_VALID);
   compareData->timeProperties.leap59 = getFlag(header->flagField, PTP2_FLAGS_LI_59);
   compareData->timeProperties.leap61 = getFlag(header->flagField, PTP2_FLAGS_LI_61);
   compareData->timeProperties.timeTraceable = getFlag(header->flagField, PTP2_FLAGS_TIME_TRACEABLE);
   compareData->timeProperties.frequencyTraceable = getFlag(header->flagField, PTP2_FLAGS_FREQ_TRACEABLE);
   compareData->timeProperties.ptpTimescale = getFlag(header->flagField, PTP2_FLAGS_PTP_TIMESCALE);
   compareData->timeProperties.synchronizationUncertain = getFlag(header->flagField, PTP2_FLAGS_SYNCHRO_UNCERTAIN);

   /* End Points */
   compareData->receivingPortIdentity = *rxPortIdentity;
   compareData->foreignPortIdentity = header->sourcePortIdentity;
}  /* END zl303xx_PtpConvertAnncMsgToCompareData */

/* ZL303XX_TRACE_FOREIGN_TABLE */
/** 

   Displays the control parameters and record entries of the specified Foreign
   Master Table. (Intentionally used CAPS to resemble other TRACE calls).

  Parameters:
   [in]  modId    Module ID from which the trace is called. Used to determine
                        if the trace will be executed based on the trace level.
   [in]  level    The trace level for the module specified.
   [in]  label    A prefix (label) string to display. Can be empty but not a
                        null pointer.
   [in]  fmTbl    The Foreign Master Table to display.

  Return Value:
   Nothing

  Notes:
   Input pointer are assumed OK for this local routine.

 *****************************************************************************/
static void ZL303XX_TRACE_FOREIGN_TABLE(
      Uint32T modId, Uint16T level,
      const char *label,
      zl303xx_ForeignMasterDS *fmTbl)
{
   /* Use the FILTER/TRACE_ALWAYS combination to avoid processing the string
    * if the trace level is set too low.    */
   ZL303XX_DEBUG_FILTER(modId, level)
   {
      zl303xx_PtpFmtDump(label, fmTbl, NULL);
   }
}  /* END ZL303XX_TRACE_FOREIGN_TABLE */

/* zl303xx_PtpFmtDump */
/** 

   Displays the control parameters and record entries of the specified Foreign
   Master Table.

  Parameters:
   [in]  label             A prefix (label) string to display. Can be empty
                                 but not a NULL pointer.
   [in]  fmTbl             The Foreign Master Table to display.
   [in]  rxPortIdentity    Pointer to the portIdentity to filter the table.
                                 If NULL: Display all entries (no filter).
                                 If All 1's: Display all entries (no filter).
                                 Else: only display entries that match portIdentity.
  Return Value:
   Nothing

  Notes:
   Input pointer are assumed OK for this local routine.

 *****************************************************************************/
void zl303xx_PtpFmtDump(
      const char *label,
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_PortIdentity *rxPortIdentity)
{
   Uint16T tblIndex;
   zl303xx_ClockStreamS *pPtpStream;
   char portStr[] = "aa:bb:cc:dd:ee:ff:gg:hh (port: 0x0000)\0";

   /* Check for a NULL table pointer and return early. */
   if (fmTbl == NULL)
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_PtpFmtDump: ERROR - NULL table pointer!! ", 0,0,0,0,0,0);

      /* RETURN early from this VOID routine */
      return;
   }

   ZL303XX_TRACE_ALWAYS("Foreign Master Table (%s)",
         (label == NULL) ? "" : label, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("Active Records:%-5u Max Records:%-5u Best Record:%-d",
         fmTbl->numActiveRecords,
         fmTbl->maxRecords,
         (Sint16T)fmTbl->bestRecord,0,0,0);
   ZL303XX_TRACE_ALWAYS("------+---------+------+-----------+---------------------------------------", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("Record|  Valid  |Stream| Messages  |                  Port", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("Index |         |Handle|Prev |Curr |                Identity", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("------+---------+------+-----+-----+---------------------------------------", 0,0,0,0,0,0);

   for(tblIndex = 0; tblIndex < fmTbl->numActiveRecords; tblIndex++)
   {
      if (zl303xx_PtpFmtEntryMatchesRxPortIdentity(
                  &fmTbl->foreign[tblIndex], rxPortIdentity) == ZL303XX_TRUE)
      {
         pPtpStream = fmTbl->foreign[tblIndex].streamData;

         if (zl303xx_PtpConvertPortAddressToStr(&fmTbl->foreign[tblIndex].portAddress, portStr) != ZL303XX_OK)
         {
            /* Null address */
            portStr[0] = '\0';
         }
         ZL303XX_TRACE_ALWAYS("%-6u|%-9s|%-6d|%-5u|%-5u|%-39s",
               tblIndex,
               ZL303XX_BOOL_TO_STR(fmTbl->foreign[tblIndex].valid),
               (pPtpStream != NULL) ? pPtpStream->clockStreamHandle : (UnativeT)-1,
               fmTbl->foreign[tblIndex].msgCount.previous,
               fmTbl->foreign[tblIndex].msgCount.current,
               portStr);

         zl303xx_PtpV2PortIdentityToString(&fmTbl->foreign[tblIndex].compareData.foreignPortIdentity, portStr);

         /* Telecom Profile: Show WTR timer */
         if (pPtpStream != NULL &&
               pPtpStream->portData->clock->config.profile == ZL303XX_PTP_PROFILE_TELECOM_G8265_1 &&
               pPtpStream->pUmtEntry != NULL &&
               !zl303xx_PtpTelecomWtrExpired(&pPtpStream->pUmtEntry->telecom))
         {
            ZL303XX_TRACE_ALWAYS("      |WTR %-5d|      |     |     |%s",
                  pPtpStream->pUmtEntry->telecom.wtr,
                  portStr,0,0,0,0);
         }
         else
         {
            ZL303XX_TRACE_ALWAYS("      |         |      |     |     |%s",
                                 portStr,0,0,0,0,0);
         }
      }

      ZL303XX_TRACE_ALWAYS("------+---------+------+-----+-----+---------------------------------------", 0,0,0,0,0,0);
   }
}

/* zl303xx_PtpFmtPathTraceDump */
/** 

   Displays any Path Trace data associated with each targeted entry of the
   specified Foreign Master Table.

  Parameters:
   [in]  fmTbl             The Foreign Master Table to display.
   [in]  rxPortIdentity    Pointer to the portIdentity to filter the table.
                                 If NULL: Display all entries (no filter).
                                 If All 1's: Display all entries (no filter).
                                 Else: only display entries that match portIdentity.
  Return Value:
   Nothing

  Notes:
   Input pointer are assumed OK for this local routine.

 *****************************************************************************/
void zl303xx_PtpFmtPathTraceDump(
      Uint32T clockHandle,
      zl303xx_PortIdentity *rxPortIdentity)
{
   Uint16T tblIndex;
   static char srcPortStr[] = "aa:bb:cc:dd:ee:ff:gg:hh (port: 0x0000)\0";
   static char rxPortStr[] = "aa:bb:cc:dd:ee:ff:gg:hh (port: 0x0000)\0";

   /* Pointer to the targeted clock & FMT. */
   zl303xx_PtpClockS * pPtpClock = NULL;
   zl303xx_ForeignMasterDS *fmTbl = NULL;


   /* Get the clock data */
   if (zl303xx_PtpClockDataGet(clockHandle, &pPtpClock) != ZL303XX_OK)
   {
      /* If non-existing, Log and RETURN early from this VOID routine */
      ZL303XX_TRACE_ALWAYS(
            "zl303xx_PtpFmtPathTraceDump(): zl303xx_PtpClockDataGet(%d) failed!\n",
            clockHandle, 0,0,0,0,0);

      return;
   }
   else
   {
      fmTbl = &pPtpClock->foreignMasterDS;
   }

   ZL303XX_TRACE_ALWAYS("Foreign Master Table: PATH TRACE Data for Clock(%d)",
         clockHandle, 0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("Active Records:%-5u Max Records:%-5u Best Record:%-d",
         fmTbl->numActiveRecords,
         fmTbl->maxRecords,
         (Sint16T)fmTbl->bestRecord, 0,0,0);
   ZL303XX_TRACE_ALWAYS("------+---------------------------------------+---------------------------------------", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("Record|      Source Port                      |      Receive Port                     | ", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("Index |        Identity                       |        Identity                       | ", 0,0,0,0,0,0);
   ZL303XX_TRACE_ALWAYS("------+---------------------------------------+---------------------------------------", 0,0,0,0,0,0);

   for(tblIndex = 0; tblIndex < fmTbl->numActiveRecords; tblIndex++)
   {
      if (zl303xx_PtpFmtEntryMatchesRxPortIdentity(
                  &fmTbl->foreign[tblIndex], rxPortIdentity) == ZL303XX_TRUE)
      {
         zl303xx_PtpV2PortIdentityToString(&fmTbl->foreign[tblIndex].compareData.foreignPortIdentity, srcPortStr);
         zl303xx_PtpV2PortIdentityToString(&fmTbl->foreign[tblIndex].compareData.receivingPortIdentity, rxPortStr);

         ZL303XX_TRACE_ALWAYS(
               "%-6u| %-30s| %-40s",
               tblIndex, srcPortStr, rxPortStr, 0,0,0);

         zl303xx_PtpClockPathTraceListShow(
               &fmTbl->foreign[tblIndex].pathTraceList);
      }

      ZL303XX_TRACE_ALWAYS("------+---------------------------------------+---------------------------------------", 0,0,0,0,0,0);
   }
}  /* END zl303xx_PtpFmtPathTraceDump */


/****************   FUNCTION DEFINITIONS FOR EXTERNAL FMT ACCESS   ************/

/* zl303xx_PtpFmtGet */
/**

   Gets the Foreign Master Table data for a specified PTP Clock. The first entry
   in the table is the DefaultDS data of the target Clock.

  Parameters:
   [in]  clockHandle    Handle of the targeted PTP Clock.
   [in]  numberEntries  As an input, the maximum number of entries requested
                              by the user - should correspond with the size of
                              the array table passed in as the output.
                              (Use -1 or 0 to get all entries).

   [out] numberEntries  As an output, the number of entries returned.
   [out] tblEntries     Pointer to the array of FMT entries returned.
                              tblEntries[0] is always the default Clock data.

  Return Value:
     ZL303XX_OK                   Success
     ZL303XX_PARAMETER_INVALID    The clockHandle is invalid.
     ZL303XX_INVALID_POINTER      The numberEntries or tblEntries pointer is NULL.

 *****************************************************************************/
zlStatusE zl303xx_PtpFmtGet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint16T *numberEntries,
      zl303xx_PtpFmtEntryS *tblEntries)
{
   /* Status variables */
   zlStatusE status = ZL303XX_OK;
   zlStatusE mStatus = ZL303XX_ERROR;

   /* Pointer to the targeted clock. */
   zl303xx_PtpClockS *pPtpClock = NULL;

   /* Loop counters and Range boundaries on the number of entries to return. */
   Uint16T maxEntries = 0;
   Uint16T actualEntries = 0;

   /* Check the input pointers */
   if ((numberEntries == NULL) || (tblEntries == NULL))
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_PtpFmtGet(): INVALID PTR;   ",
            0,0,0,0,0,0);
      status = ZL303XX_INVALID_POINTER;
   }

   /* Get the Clock Data for the target */
   if (status == ZL303XX_OK)
   {
      if ((status = zl303xx_PtpClockDataGet(clockHandle, &pPtpClock)) != ZL303XX_OK )
      {
         ZL303XX_TRACE_ALWAYS ("zl303xx_PtpFmtGet(): Call to zl303xx_PtpClockDataGet() failed!",
               0,0,0,0,0,0);
      }

      /* Set the bounds on the number of entries to return. */
      else
      {
         if ((*numberEntries == 0) ||
             (*numberEntries == (Uint16T)(-1)))
         {
            /* MAX count will be the clock data + all table entries. */
            maxEntries = 1 + pPtpClock->config.maxForeignRecords;
         }
         else
         {
            /* The supplied array is smaller than the internal table. */
            maxEntries = *numberEntries;
         }
      }
   }

   /* Lock the Clock mutex before accessing the configuration data and dynamic
    * remote server table. */
   if (status == ZL303XX_OK)
   {
      mStatus = zl303xx_PtpClockMutexLock(pPtpClock);
      status = mStatus;
   }

   /* Get the Default entry for the local Clock */
   if (status == ZL303XX_OK)
   {
      /* PTP Object Handles. */
      tblEntries[0].clockHandle  = pPtpClock->clockHandle;
      tblEntries[0].portHandle   = (Uint32T)(-1);
      tblEntries[0].streamHandle = (Uint32T)(-1);

      /* Receiving portIdentity is the local clockIdentity and port 0. */
      OS_MEMCPY(tblEntries[0].receivingPortIdentity.clockIdentity,
             pPtpClock->config.defaultDS.clockIdentity,
             PTP_V2_CLK_IDENTITY_LENGTH);
      tblEntries[0].receivingPortIdentity.portNumber = 0;

      /* The foreignPortIdentity is the same as receivingPortIdentity */
      tblEntries[0].foreignPortIdentity = tblEntries[0].receivingPortIdentity;

      /* Steps removed is always '0' for the Clock */
      tblEntries[0].stepsRemoved = 0;

      /* Clock Time Properties Parameters */
      tblEntries[0].timeProperties = pPtpClock->config.localTimeProperties;

      /* All Grandmaster Data comes from the local DefaultDS. */
      OS_MEMCPY(tblEntries[0].gm.clockIdentity,
             pPtpClock->config.defaultDS.clockIdentity,
             PTP_V2_CLK_IDENTITY_LENGTH);
      tblEntries[0].gm.priority1 = pPtpClock->config.defaultDS.priority1;
      tblEntries[0].gm.priority2 = pPtpClock->config.defaultDS.priority2;
      tblEntries[0].gm.clockQuality = pPtpClock->config.defaultDS.clockQuality;

      /* The Telecom Profile priority of the Clock is '0'. */
      tblEntries[0].tp.priority = 0;

      /* Since none of the PTSF flags are local Clock specific, set them all
       * to CLEAR STATE. */
      tblEntries[0].ptsf.overall = 0;
      tblEntries[0].ptsf.flags[0] = 0;
      tblEntries[0].ptsf.masks[0] = 0;

      /* Update the number of items in the returned table.  */
      actualEntries = 1;
   }

   /* Add any remote data entries to the list. */
   if (status == ZL303XX_OK)
   {
      Uint16T entryIndex = 0;
      zl303xx_ForeignMasterRecordS *iFmtEntry = NULL; /* Internal table Entry to simplify table pointers */
      zl303xx_PtpFmtEntryS         *xFmtEntry = NULL; /* Exported table Entry to simplify table pointers */

      for (entryIndex = 0;
           ((entryIndex < pPtpClock->config.maxForeignRecords) &&
            (actualEntries < maxEntries));
           entryIndex++)
      {
         /* Dereference the internal table entry (to make the code easier to read).*/
         iFmtEntry = &pPtpClock->foreignMasterDS.foreign[entryIndex];

         /* Copy each entry from the foreign Master Table. */
         if (iFmtEntry->streamData != NULL)
         {
            /* Dereference the exported table entry (to make the code easier to read).*/
            xFmtEntry = &tblEntries[actualEntries];

            /* Update the number of entries that will now be in the table. */
            actualEntries++;

            /* PTP Object Handles. */
            xFmtEntry->clockHandle  = iFmtEntry->streamData->portData->clock->clockHandle;
            xFmtEntry->portHandle   = iFmtEntry->streamData->portData->clockPortHandle;
            xFmtEntry->streamHandle = iFmtEntry->streamData->clockStreamHandle;

            /* End-Point Information. */
            xFmtEntry->receivingPortIdentity = iFmtEntry->compareData.receivingPortIdentity;
            xFmtEntry->foreignPortIdentity = iFmtEntry->compareData.foreignPortIdentity;
            xFmtEntry->stepsRemoved = iFmtEntry->compareData.stepsRemoved;

            /* Time Property Information */
            xFmtEntry->timeProperties = iFmtEntry->compareData.timeProperties;

            /* Grandmaster parameters of the clock associated with this connection. */
            OS_MEMCPY(xFmtEntry->gm.clockIdentity,
                   iFmtEntry->compareData.grandmasterIdentity,
                   PTP_V2_CLK_IDENTITY_LENGTH);
            xFmtEntry->gm.priority1    = iFmtEntry->compareData.grandmasterPriority1;
            xFmtEntry->gm.priority2    = iFmtEntry->compareData.grandmasterPriority2;
            xFmtEntry->gm.clockQuality = iFmtEntry->compareData.grandmasterClockQuality;

            /* The Telecom Profile priority Value (if applicable). */
            if (iFmtEntry->streamData->pUmtEntry != NULL)
            {
               xFmtEntry->tp.priority = iFmtEntry->streamData->pUmtEntry->telecom.priority;
               /* xFmtEntry->tp.clockClass = iFmtEntry->streamData->pUmtEntry->telecom.clockClass;
               xFmtEntry->tp.stepsRemoved = iFmtEntry->streamData->pUmtEntry->telecom.stepsRemoved; */
            }
            else
            {
               xFmtEntry->tp.priority = 0;
              /* xFmtEntry->tp.clockClass = 0;
               xFmtEntry->tp.stepsRemoved = 0; */
            }

            /* The PTSF Flags and Masks. */
            {
               Uint32T ptsfFlag;

               xFmtEntry->ptsf.overall = zl303xx_PtsfFlagGetAndMask(iFmtEntry->streamData->clockStreamHandle, ZL303XX_PTSF_MAIN);

               for (ptsfFlag = 0; ptsfFlag < ZL303XX_PTSF_NUM_FLAGS; ptsfFlag++)
               {
                  xFmtEntry->ptsf.flags[0] |= (zl303xx_PtsfFlagGet(xFmtEntry->streamHandle, ptsfFlag) << ptsfFlag);
                  xFmtEntry->ptsf.masks[0] |= (zl303xx_PtsfMaskGet(xFmtEntry->streamHandle, ptsfFlag) << ptsfFlag);
               }
            }
         }
      }
   }  /* Loop through internal table. */

   /* If the Clock mutex was taken, release it. */
   if (mStatus == ZL303XX_OK)
   {
      status = zl303xx_PtpClockMutexUnlock(pPtpClock, status);
   }

   /* Regardless of the final status, return the #entries returned. */
   if (numberEntries)
      *numberEntries = actualEntries;

   return status;
}  /* zl303xx_PtpFmtGet */


/* zl303xx_PtpFmtShow */
/**

   Debug routine to display the Foreign Master Table data for a specified PTP
   Clock (in exported data format).

  Parameters:
   [in]  clockHandle    Handle of the targeted PTP Clock.

  Return Value:
     ZL303XX_OK                   Success
     ZL303XX_PARAMETER_INVALID    The clockHandle is invalid.

 *****************************************************************************/
zlStatusE zl303xx_PtpFmtShow(
      zl303xx_PtpClockHandleT clockHandle)
{
   zlStatusE status = ZL303XX_OK;

   /* Return parameters for the GET routine. */
   Uint16T numEntries = ZL303XX_PTP_DEFAULT_PORT_MAX_FOREIGN_REC + 1;
   zl303xx_PtpFmtEntryS entryList[ZL303XX_PTP_DEFAULT_PORT_MAX_FOREIGN_REC + 1];

   /* Get the data from the table in Exported format. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpFmtGet(clockHandle, &numEntries, entryList);
   }

   /* Print each of the retrieved entries. */
   if (status == ZL303XX_OK)
   {
      Uint16T entryCount = 0;
      zl303xx_BooleanE showHeader = ZL303XX_TRUE; /* For the 1st row */

      ZL303XX_TRACE_ALWAYS("---------------------------------------", 0,0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("FMT for PTP Clock (%d) : Number of Entries (%d) => Clock + %d Others  ",
            clockHandle, numEntries, (numEntries-1), 0,0,0);
      ZL303XX_TRACE_ALWAYS("---------------------------------------", 0,0,0,0,0,0);

      for (entryCount = 0; entryCount < numEntries; entryCount++)
      {
         (void)zl303xx_PtpFmtEntryShow(&entryList[entryCount], showHeader);

         /* Turn off printing of the header after the 1st row. */
         showHeader = ZL303XX_FALSE;
      }
      ZL303XX_TRACE_ALWAYS("--------------- END -------------------", 0,0,0,0,0,0);

   }

   return status;
}  /* zl303xx_PtpFmtShow */

/* zl303xx_PtpFmtEntryShow */
/** 

   Displays the control parameters for the specified record in the Foreign
   Master Table.

  Parameters:
   [in]  fmtEntry    Pointer to an entry in the Foreign Master Table.
   [in]  showHeader  Flag indicating whether or not to print a Heading Row
                           for the entry.

   [out] NONE

  Return Value:
     ZL303XX_OK                   Success
     ZL303XX_INVALID_POINTER      The fmtEntry member is NULL.

 *****************************************************************************/
zlStatusE zl303xx_PtpFmtEntryShow(
      zl303xx_PtpFmtEntryS *fmtEntry,
      zl303xx_BooleanE printHeader)
{
   char headStr[]  = "StrId | ClkId | rxPt# | foreignPortIdentity             | Steps | Grandmaster ClockId     | P1 | P2 | CLS| ACC| VARNC | TpP | PTSF (FLAGS:MASKS)   \0";
   char lineStr[]  = "---------------------------------------------------------------------------------------------------------------------------------------------------\0";
   /*                           1         2         3         4         5         6         7         8         9        10        11        12        13        14        15      */
   /*                 012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345 */
   char entryStr[] = "65535 | 65535 | 65535 | aa:bb:cc:dd:ee:ff:gg:hh: 0x0000 | 65535 | aa:bb:cc:dd:ee:ff:gg:hh | 255; 255; 255; 255; -65535| 255 | 1(FFFFFFFF:FFFFFFFF) \0";
   char *idByte = NULL;

   /* The following are used to construct the entry string. (Corresponds to the
    * index into the string where the target field will be displayed.  */
   const Uint8T fpiStart    = 24;
   const Uint8T stepsStart  = 58;
   const Uint8T gmIdStart   = 66;
   const Uint8T gmDataStart = 92;
   const Uint8T tpStart     = 120;
   const Uint8T ptsfStart   = 126;

   /* Verify that a valid pointer is passed in. */
   if (fmtEntry == NULL)
   {
      return ZL303XX_INVALID_POINTER;
   }

   if (printHeader == ZL303XX_TRUE)
   {
      ZL303XX_TRACE_ALWAYS("%s", headStr, 0,0,0,0,0);
      ZL303XX_TRACE_ALWAYS("%s", lineStr, 0,0,0,0,0);
   }

   /* Entry Handles */
   OS_SNPRINTF(entryStr, sizeof(entryStr), "%5d | %5d | %5d |  " ,
         fmtEntry->streamHandle, fmtEntry->clockHandle, fmtEntry->receivingPortIdentity.portNumber);

   /* End Point Data */
   idByte = (char *)&fmtEntry->foreignPortIdentity.clockIdentity[0];
   OS_SNPRINTF(&entryStr[fpiStart], sizeof(entryStr)-fpiStart,
           "%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx: 0x%04hx |  ",
           idByte[0], idByte[1], idByte[2], idByte[3], idByte[4], idByte[5], idByte[6], idByte[7],
           fmtEntry->foreignPortIdentity.portNumber);

   /* The Rx PortIdentity.clockIdentity is the same as the local clock ID so don't display. */
   /*idByte = (char *)&fmtEntry->receivingPortIdentity.clockIdentity[0];
   OS_SPRINTF(&entryStr[51],
           "%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx: 0x%04hx |  ",
           idByte[0], idByte[1], idByte[2], idByte[3], idByte[4], idByte[5], idByte[6], idByte[7],
           fmtEntry->receivingPortIdentity.portNumber); */
   OS_SNPRINTF(&entryStr[stepsStart],  sizeof(entryStr)-stepsStart, "%5d |  ", fmtEntry->stepsRemoved);

   /* Grandmaster Data */
   idByte = (char *)&fmtEntry->gm.clockIdentity[0];
   OS_SNPRINTF(&entryStr[gmIdStart], sizeof(entryStr)-gmIdStart,
           "%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx |  ",
           idByte[0], idByte[1], idByte[2], idByte[3], idByte[4], idByte[5], idByte[6], idByte[7]);
   OS_SNPRINTF(&entryStr[gmDataStart], sizeof(entryStr)-gmDataStart,
            "%3d; %3d; %3d; 0x%02X; 0x%04X|  " ,
            fmtEntry->gm.priority1, fmtEntry->gm.priority2,
            fmtEntry->gm.clockQuality.clockClass, fmtEntry->gm.clockQuality.clockAccuracy,
            fmtEntry->gm.clockQuality.offsetScaledLogVariance);

   /* Telecom Profile Specific */
   OS_SNPRINTF(&entryStr[tpStart], sizeof(entryStr)-tpStart, "%3d |  ", fmtEntry->tp.priority);

   /* PTSF Flags and Masks */
   OS_SNPRINTF(&entryStr[ptsfStart], sizeof(entryStr)-ptsfStart, "%1d(%08hx:%08hx)",
         fmtEntry->ptsf.overall, fmtEntry->ptsf.flags[0], fmtEntry->ptsf.masks[0]);

   ZL303XX_TRACE_ALWAYS("%s", entryStr,0,0,0,0,0);

   return ZL303XX_OK;
}  /* zl303xx_PtpFmtEntryShow */

/* zl303xx_PtpFmtEntryMatchesRxPortIdentity */
/** 

   Local utility to determine if a Foreign MAster TAble entry matches the
   specified rxPortIdentity.

  Parameters:
   [in]  fmtEntry          Pointer to a FMT record entry.
   [in]  rxPortIdentity    Pointer to a rxPortIdentity to filter.
                                 If NULL: All entries MATCH (no filter).
                                 If All 1's: All entries MATCH (no filter).
                                 Else: only MATCH entries with the specified
                                       rxPortIdentity.
  Return Value:
   zl303xx_BooleanE

  Notes:
   Input pointer are assumed OK for this local routine.

 *****************************************************************************/
zl303xx_BooleanE zl303xx_PtpFmtEntryMatchesRxPortIdentity(
      zl303xx_ForeignMasterRecordS *fmtEntry,
      zl303xx_PortIdentity *rxPortIdentity)
{
   /* Logic on the rxPortIdentity:
      -------------------------------------------
             |  All 1's  | RX == REC | RX != REC |
      -------------------------------------------
       NULL  |   MATCH   |   MATCH   |   MATCH   |
      -------------------------------------------
      !NULL  |   MATCH   |   MATCH   |  NO MATCH |
      -------------------------------------------   */
   if ((rxPortIdentity == NULL) ||
       (zl303xx_PtpV2EqualPortIdentities(
               &fmtEntry->compareData.receivingPortIdentity,
               rxPortIdentity)) ||
       (zl303xx_PtpV2IsAllClockIdentity(rxPortIdentity->clockIdentity) == ZL303XX_TRUE))
   {
      return ZL303XX_TRUE;
   }
   else
   {
      return ZL303XX_FALSE;
   }
}  /* END zl303xx_PtpFmtEntryMatchesRxPortIdentity */

