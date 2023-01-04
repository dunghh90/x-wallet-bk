

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains interface functions to:
*       1) Start, configure, and stop the Reference Selection Application.
*       2) Send data updates to the server database.
*       3) Retrieve data from the database to make Reference Selection decisions.
*
*******************************************************************************/

#ifndef _ZL303XX_EXAMPLE_REF_SELECT_API_H_
#define _ZL303XX_EXAMPLE_REF_SELECT_API_H_

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_ExampleRefSelectSetup.h"

#include "zl303xx_Os.h"            /* For TASK related OS definitions. */
#include "zl303xx_Error.h"         /* Defines zlStatusE type and includes:
                                      - zl303xx_ErrorLabels.h
                                      - zl303xx_ErrorLabelsGeneric.h   */
#include "zl303xx_Trace.h"         /* For Trace Logging. */

/* MODULE Interface Inclusions. */
#include "zl303xx_PtpApiTypes.h"   /* Protocol Inclusions */
#include "zl303xx_Ptsf.h"          /* PTSF Inclusions */

#if defined APR_INCLUDED
   #include "zl303xx_Apr.h"
   #define ALGORITHM_NUM_FLAGS ZL303XX_SERVER_CLK_NUM_TYPES
#else
   #define ALGORITHM_NUM_FLAGS 256
#endif

/*****************   DEFINES   ************************************************/

/* Default definition of an undefined Connection ID or Clock/Stream Handle. */
#define REF_SELECT_CONN_ID_INVALID        (Uint32T)(-1)

/* Generic DISABLED setting for all Timers (Same as WAIT_FOREVER). */
#define REF_SELECT_TIMER_DISABLED         (Uint32T)(-1)

/*****************   DATA TYPES   *********************************************/

/* A list of target modules that can send or receive data. */
typedef enum exampleRefSelectSrcModuleE
{
   REF_SELECT_SRC_MOD_UNKNOWN   =  0,
   REF_SELECT_SRC_MOD_APP,
   REF_SELECT_SRC_MOD_PROT_CLOCK,
   REF_SELECT_SRC_MOD_PROT_STREAM,
   REF_SELECT_SRC_MOD_PTSF,
   REF_SELECT_SRC_MOD_ALGORITHM,
   REF_SELECT_SRC_MOD_CUSTOM
} exampleRefSelectSrcModuleE;

/* A list of actions to perform on the data to / from modules. */
typedef enum exampleRefSelectActionE
{
   REF_SELECT_ACTION_UNKNOWN   =  0,
   REF_SELECT_ACTION_ADD,        /* Adds a new entry (typically protocol stream). */
   REF_SELECT_ACTION_UPDATE,     /* Updating an existing entry. */
   REF_SELECT_ACTION_DELETE,     /* Delete a entry or record. */
   REF_SELECT_ACTION_STATUS,     /* Status update. */
   REF_SELECT_ACTION_TIMESTAMP   /* Valid for STREAM data. */
} exampleRefSelectActionE;

/* A list of entry types in the table. */
typedef enum exampleRefSelectConnTypeE
{
   REF_SELECT_CONN_TYPE_UNKNOWN   =  0,
   REF_SELECT_CONN_TYPE_PROTOCOL_CLOCK,
   REF_SELECT_CONN_TYPE_PROTOCOL_STREAM,
   REF_SELECT_CONN_TYPE_ELECTRICAL        /* Not used currently. */
} exampleRefSelectConnTypeE;

/** Enum for the User Decision BMC Algorithm selected. */
typedef enum exampleRefSelectBmcaTypeE
{
   /* Use the Default IEEE 1588 Algorithm. */
   /* (Default at startup) */
   REF_SELECT_BMCA_DEFAULT_PROFILE = 0,

   /* Use the ITU-T G.8265: Telecom Profile for Frequency Algorithm. */
   REF_SELECT_BMCA_TELECOM_PROFILE_FREQ,

   /* Use the ITU-T G.8275: Telecom Profile for Phase Algorithm. */
   REF_SELECT_BMCA_TELECOM_PROFILE_PHASE,

   /* Use the C37.238 v1 (2011) Profile. */
   REF_SELECT_BMCA_POWER_PROFILE_V1,

   /* Use a custom algorithm. */
   REF_SELECT_BMCA_CUSTOM
} exampleRefSelectBmcaTypeE;

/*****************   DATA STRUCTURES   ****************************************/

/********  Reference Selection Application  *************/
/* Configuration structure for the Reference Selection Application   */
typedef struct exampleRefSelectConfigS
{
   /* Local hardware parameter for the Time-Sync Device (CGU). */
   void *hwAlgParam;

   /* Application Refresh Timer (in milliseconds). */
   Uint32T refreshRateMs;

   /* Parameters related to the server selection behaviour. */
   struct
   {
      /* Flag indicating whether to automatically run the Server Selection Algorithm. */
      zl303xx_BooleanE autoRun;

      /* Which Server Selection Algorithm should be used. */
      exampleRefSelectBmcaTypeE bmcAlg;

      /* ID of a forced server selection. */
      Uint32T forcedRefId;
   } selection;

   /* Parameters related to Telecom Profile protection mechanisms. */
   struct
   {
      /* Flag indicating if revertive switching is used for Telecom Profile. */
      zl303xx_BooleanE revertiveEn;

      /* The Hold-Off period (in seconds). */
      Uint32T holdOffSec;

      /* Flag indicating if Squelch is used for Telecom Profile. */
      zl303xx_BooleanE squelchEn;
   } protection;

} exampleRefSelectConfigS;

/* Structure for the Stopping the Reference Selection Application   */
/* (No members used currently but added for backwards compatibility). */
typedef struct exampleRefSelectStopS
{
   Uint32T empty;
} exampleRefSelectStopS;

/********  Reference Application Message Structures  *************/
/* Basic structure for sending Protocol Clock updates. */
/* ---------- */
typedef struct exampleProtocolClockUpdateMsgS
{
   zl303xx_PtpClockHandleT clockHandle;
   zl303xx_PtpProfileE profile;

   zl303xx_PtpBmcCompareDataS bmcaData;
   zl303xx_PtpTelecomCompareDataS telecomData;
   zl303xx_PtpG8275ProfileCompareDataS tpPhaseData;
} exampleProtocolClockUpdateMsgS;

typedef struct exampleProtocolClockDeleteMsgS
{
   zl303xx_PtpClockHandleT clockHandle;
} exampleProtocolClockDeleteMsgS;

/* Basic structure for sending Protocol Stream updates. */
/* ---------- */
typedef struct exampleProtocolStreamAddMsgS
{
   zl303xx_PtpStreamHandleT streamHandle;
   zl303xx_PtpClockHandleT clockHandle;
   Uint32T serverId;
} exampleProtocolStreamAddMsgS;

typedef struct exampleProtocolStreamUpdateMsgS
{
   zl303xx_PtpStreamHandleT streamHandle;
   zl303xx_PtpClockHandleT clockHandle;
   zl303xx_PtpProfileE profile;
   zl303xx_BooleanE streamDataValid;

   zl303xx_PtpBmcCompareDataS bmcaData;
   zl303xx_PtpTelecomCompareDataS telecomData;
   zl303xx_PtpG8275ProfileCompareDataS tpPhaseData;
} exampleProtocolStreamUpdateMsgS;

typedef struct exampleProtocolStreamDeleteMsgS
{
   zl303xx_PtpStreamHandleT streamHandle;
   zl303xx_PtpClockHandleT clockHandle;
} exampleProtocolStreamDeleteMsgS;

/* Basic structure for sending PTSF updates. */
/* ---------- */
typedef struct examplePtsfUpdateMsgS
{
   Uint32T connectionId;
   Uint32T flag;
   Uint32T value;
   Uint32T mask;
} examplePtsfUpdateMsgS;

/* Basic structure for sending Algorithm Status Flag updates. */
/* ---------- */
typedef struct exampleAlgorithmUpdateMsgS
{
   Uint32T serverId;
   Uint32T statusFlagId;
   Uint32T value;
} exampleAlgorithmUpdateMsgS;


/* A structure representing a single row of the database server table. */
typedef struct exampleRefSelectEntryS
{
   /* Connection Identification */
   Uint32T connectionId;

   /* Each Protocol connection is uniquely defined by a clock & stream handle. */
   zl303xx_PtpClockHandleT clockHandle;
   zl303xx_PtpStreamHandleT streamHandle;

   /* The IP addresses of the end points of the connection. */
   zl303xx_PortAddress localPortAddress;
   zl303xx_PortAddress farEndPortAddress;

   /* Reference Mode: Protocol-Clock, Protocol-Stream, ELEC-REF  */
   exampleRefSelectConnTypeE entryType;

   /* Protection parameters (configuration & dynamic). */
   struct
   {
      /* Force the entry to be ignored. */
      zl303xx_BooleanE lockout;

      /* Wait-to-Restore parameters. */
      struct
      {
         /* Wait-to-Restore config for the connection. */
         Uint32T seconds;
         /* Wait-to-Restore config for the connection. */
         zl303xx_BooleanE active;
         /* CPU tick count when the timer started. */
         Uint32T tickStart;
         /* CPU tick count when the timer started. */
         Uint32T ticksRemaining;
      } wtr;
   } protection;


   /* Protocol related data regarding the reference. */
   struct
   {
      zl303xx_BooleanE valid;
      zl303xx_PtpProfileE profile;
      zl303xx_PtpBmcCompareDataS bmcaData;
      zl303xx_PtpTelecomCompareDataS telecomData;
      zl303xx_PtpG8275ProfileCompareDataS tpPhaseData;
   } protocol;

   /* PTSF bits regarding the reference. */
   struct
   {
      zl303xx_BooleanE valid;
      Uint32T value[ZL303XX_PTSF_NUM_FLAGS];
      Uint32T mask[ZL303XX_PTSF_NUM_FLAGS];
   } ptsf;

   /* Algorithm status of the reference. */
   struct
   {
      zl303xx_BooleanE valid;
      Uint32T statusFlag[ALGORITHM_NUM_FLAGS];
   } algorithm;

   struct
   {
      zl303xx_BooleanE valid;
      Uint32T empty;
   } custom;
} exampleRefSelectEntryS;

/* Generic Backplane message for all message exchanges. */
typedef struct exampleRefSelectBpMsgS
{
   /* Message Control parameters. */
   exampleRefSelectSrcModuleE source;
   exampleRefSelectActionE    action;

   /* Data Parameters. */
   union
   {
      /* Reference selection members. */
      exampleRefSelectEntryS bestRef;

      /* Protocol Clock Members */
      exampleProtocolClockUpdateMsgS clockUpdate;
      exampleProtocolClockDeleteMsgS clockDelete;

      /* Protocol Stream Members */
      exampleProtocolStreamAddMsgS    streamAdd;
      exampleProtocolStreamUpdateMsgS streamUpdate;
      exampleProtocolStreamDeleteMsgS streamDelete;

   } data;
} exampleRefSelectBpMsgS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS (MESSAGES)   **************/

/******************************************************************************/
/* Routines to Configure the Reference Selection Application at startup.      */
/******************************************************************************/
zlStatusE exampleRefSelectAppStructInit(exampleRefSelectConfigS *configParams);
zlStatusE exampleRefSelectAppConfigGet(exampleRefSelectConfigS *configParams);
zlStatusE exampleRefSelectAppConfigSet(exampleRefSelectConfigS *configParams);

/******************************************************************************/
/* Routines to Start and Stop the Reference Selection Application.            */
/******************************************************************************/
zlStatusE exampleRefSelectAppStart(exampleRefSelectConfigS *configParams);
zlStatusE exampleRefSelectAppStop(exampleRefSelectStopS *stopParams);

/*******************************************************************/
/* Configuration Routines for the Reference Selection Application. */
/*******************************************************************/
zlStatusE exampleRefSelectCguDeviceSet       (void *devicePtr);
zlStatusE exampleRefSelectRefreshTimerSet    (Uint32T refreshRateMs);
zlStatusE exampleRefSelectAutoSelectEn       (zl303xx_BooleanE autoSelectEn);
zlStatusE exampleRefSelectBmcAlgorithmSet    (exampleRefSelectBmcaTypeE bmcAlg);
zlStatusE exampleRefSelectManualServerSet    (Uint32T serverId);
zlStatusE exampleRefSelectRevertiveEn        (zl303xx_BooleanE revertiveEn);
zlStatusE exampleRefSelectQlHoldoffTimeSet   (Uint32T holdoffSec);
zlStatusE exampleRefSelectSquelchEn          (zl303xx_BooleanE squelchEn);

/*****************************************************/
/* Routines to configure server entry parameters. */
/*****************************************************/
zlStatusE exampleRefSelectEntryLockoutSet(Uint32T serverId, zl303xx_BooleanE lockout);
zlStatusE exampleRefSelectEntryWtrSet    (Uint32T serverId, Uint32T wtrSec);

/***************************************************************************/
/* Command to retrieve the best server based on the Algorithm configured. */
/***************************************************************************/
/* Retrieves the data related to the current 'best' entry used by the Application.
 * This does not re-execute the selection algorithm, but returns the previous entry
 * stored in the selection history. */
zlStatusE exampleRefSelectActiveServerGet(exampleRefSelectEntryS *bestEntry);
zlStatusE exampleRefSelectBestServersGet(
      Uint32T *numEntries,
      exampleRefSelectEntryS *bestEntries);

/* Executes the selection algorithm configured by the Application and returns
 * the data of the best server available. This does not replace the internal
 * server being used or the server history.
 * (To replace the internal data, enable Auto-Selection). */
zlStatusE exampleRefSelectExecuteBmca(exampleRefSelectEntryS *bestEntry);

/******************************************************************************/
/* Routines to Cross-Reference ConnectionId (i.e. serverId) & Protocol IDs
 * (clockId & streamId) pairs.                                                */
/******************************************************************************/
zlStatusE exampleRefSelectGetConnIdFromProtocolId(
      Uint32T clockId, Uint32T streamId,
      Uint32T *connectionId);
zlStatusE exampleRefSelectGetProtocolIdFromConnId(
      Uint32T connectionId,
      Uint32T *clockId, Uint32T *streamId);

/******************************************************/
/* Message Sending Routines for updating the database */
/******************************************************/
/* Application Configuration Change. */
zlStatusE exampleRefSelectConfigChange(void);

/* Protocol Clock Messages */
zlStatusE exampleRefSelectProtocolClockUpdate(
      Uint32T clockId, exampleProtocolClockUpdateMsgS *protocolClockDataS);
zlStatusE exampleRefSelectProtocolClockDelete(
      Uint32T clockId, exampleProtocolClockDeleteMsgS *protocolClockDataS);

/* Protocol Stream Messages */
zlStatusE exampleRefSelectProtocolStreamAdd(
      Uint32T streamId, exampleProtocolStreamAddMsgS *protocolStreamDataS);
zlStatusE exampleRefSelectProtocolStreamUpdate(
      Uint32T streamId, exampleProtocolStreamUpdateMsgS *protocolStreamDataS);
zlStatusE exampleRefSelectProtocolStreamDelete(
      Uint32T streamId, exampleProtocolStreamDeleteMsgS *protocolStreamDataS);

/* PTSF Messages */
zlStatusE exampleRefSelectPtsfUpdate(
      Uint32T connectionID, examplePtsfUpdateMsgS *ptsfUpdateData);

/* Time-Sync Algorithm Messages */
zlStatusE exampleRefSelectAlgorithmUpdate(
      Uint32T serverID, exampleAlgorithmUpdateMsgS *algUpdateData);

/* General Task Wake-up. */
zlStatusE exampleRefSelectAppWakeUp(void);

/*****************   EXTERNAL FUNCTION DECLARATIONS (INPUT INTERFACES)  *******/
/* This routine is used to translate the data events from other modules into
 * compatible data for the Reference Selection Application. */
zlStatusE exampleRefSelectSendMsgToBackplane(exampleRefSelectBpMsgS *bpMsg);



#endif /* MULTIPLE INCLUDE BARRIER */
