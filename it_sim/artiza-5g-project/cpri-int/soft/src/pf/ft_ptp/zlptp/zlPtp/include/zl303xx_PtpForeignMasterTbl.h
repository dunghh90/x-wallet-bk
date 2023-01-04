

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Top level header file that defines the exported functions interface for the
*     PTP stack
*
*******************************************************************************/

#ifndef DEV_PTPFOREIGNMASTERTBL_H_
#define DEV_PTPFOREIGNMASTERTBL_H_

/*****************   INCLUDE FILES                *****************************/
/* Include key files from the zlApi */
#include "zl303xx_Global.h"

#include "zl303xx_PtpDatatypes.h"

/*****************   DEFINES     **********************************************/

/*****************   DATA TYPES   *********************************************/

/** Implementation specific structure for storing all data related to a
 *  single row of the Foreign Master Table. (Uses exported data types for user
 *  interface purposes). */
typedef struct zl303xx_PtpFmtEntryS
{
   /** Handle of the stream on which this foreign record was received. (Empty
    *  will have stream, port and clock Handles set to INVALID (-1)). */
   zl303xx_PtpStreamHandleT streamHandle;
   /** Handle of the port associated with the receiving stream. */
   zl303xx_PtpPortHandleT portHandle;
   /** Handle of the clock associated with the receiving stream. */
   zl303xx_PtpClockHandleT clockHandle;

   /* End point Data. */
   /** The Port Identity of the far-end transmitting port. */
   zl303xx_PortIdentity foreignPortIdentity;
   /** The Port Identity of the local receiving port. */
   zl303xx_PortIdentity receivingPortIdentity;

   /* Network parameters. */
   /** Network Hops (i.e. steps removed). */
   Uint16T stepsRemoved;

   /* Time Properties parameters. */
   zl303xx_TimePropertiesDS timeProperties;

   /** Grandmaster parameters of the clock associated with this connection. */
   struct
   {
      /** Grandmaster Clock Identity (used for both Default & Telecom Profiles). */
      zl303xx_ClockIdentity clockIdentity;

      /** Grandmaster Priority1. */
      Uint8T   priority1;
      /** Grandmaster Priority2. */
      Uint8T   priority2;
      /** Grandmaster Clock Quality (the clockClass member is used for both
       *  Default & Telecom Profiles). */
      zl303xx_ClockQuality  clockQuality;
   } gm;

   /** Telecom Profile specific parameters.  */
   struct
   {
      /** Server Priority in G.8265.1; localPriority in G.8275.1 */
      Uint8T priority;
   } tp;

   /** PTSF flags and masks.  Generally used only for Telecom Profile but provided
    *  in case they are required for general purposes. */
   struct
   {
      /** PTSF - Overall value */
      zl303xx_BooleanE overall;

      /** PTSF - individual flags */
      Uint32T flags[1];
      /** PTSF - individual masks */
      Uint32T masks[1];
   } ptsf;
} zl303xx_PtpFmtEntryS;

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/* Table Management Routines */
zlStatusE zl303xx_PtpFmtCreate(zl303xx_ForeignMasterDS *fmTbl);
zlStatusE zl303xx_PtpFmtDelete(zl303xx_ForeignMasterDS *fmTbl);
zlStatusE zl303xx_PtpFmtFlush (
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_PortIdentity *rxPortIdentity);

zlStatusE zl303xx_PtpFmtDisqualifyAll(
      zl303xx_PtpClockS *pClock);
zlStatusE zl303xx_PtpFmtQualifyStreams(
      zl303xx_PtpClockS *pClock);
Uint16T zl303xx_PtpFmtSelectedGet(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_PortAddress *pAddr);

/* Table Entry Management */
zlStatusE zl303xx_PtpFmtEntryUpdate(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_PtpV2MsgHeaderS *header,
      zl303xx_PtpV2MsgAnnounceS *announce,
      zl303xx_ClockStreamS *streamData,
      zl303xx_PortAddress *pSrcAddr);
zlStatusE zl303xx_PtpFmtEntryDelete(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ClockStreamS *stream);
zlStatusE zl303xx_PtpFmtEntryQualify(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ClockStreamS *streamData);
zlStatusE zl303xx_PtpFmtEntryDisqualify(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ClockStreamS *streamData);
zlStatusE zl303xx_PtpFmtEntryDisqualifyForeignPort(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ClockStreamS *streamData,
      zl303xx_PortIdentity *fgnPortId);
zlStatusE zl303xx_PtpFmtEntryPathTraceUpdate(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_PortIdentity *srcPortIdentity,
      zl303xx_ClockStreamS *streamData,
      Uint8T *pPathTraceTlvBufStart);
zlStatusE zl303xx_PtpFmtPathTraceToActive(
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PortIdentity *pParentPortId);

/* Dumps internal data related to the FMT. */
void zl303xx_PtpFmtDump(const char *label,
      zl303xx_ForeignMasterDS *fmTbl,
      zl303xx_PortIdentity *rxPortIdentity);
void zl303xx_PtpFmtPathTraceDump(
      Uint32T clockHandle,
      zl303xx_PortIdentity *rxPortIdentity);

/*****************   FUNCTION DECLARATIONS FOR CUSTOMER ACCESS  ***************/

zlStatusE zl303xx_PtpFmtGet(
      zl303xx_PtpClockHandleT clockHandle,
      Uint16T *numberEntries,
      zl303xx_PtpFmtEntryS *tblEntries);
zlStatusE zl303xx_PtpFmtShow(zl303xx_PtpClockHandleT clockHandle);
zlStatusE zl303xx_PtpFmtEntryShow(zl303xx_PtpFmtEntryS *fmtEntry, zl303xx_BooleanE printHeader);


#endif /* DEV_PTPFOREIGNMASTERTBL_H_ */

