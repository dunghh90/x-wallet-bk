

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*  Internal definitions related to the Reference Selection Application.
*
*******************************************************************************/

#ifndef _ZL303XX_EXAMPLE_REF_SELECT_INT_H_
#define _ZL303XX_EXAMPLE_REF_SELECT_INT_H_

/*****************   INCLUDE FILES   ******************************************/

/* Include the API Types. */
#include "zl303xx_ExampleRefSelectApi.h"

/* MODULE Interface Inclusions. */
#include "zl303xx_PtpApiTypes.h"   /* Protocol Inclusions */
#include "zl303xx_Ptsf.h"          /* PTSF Inclusions */

/*****************   DEFINES (INTERNAL)   *************************************/

/* Range limits on the Application Refresh Rate (milliseconds).
 * (cannot be '0' but can be DISABLED). */
#define REF_SELECT_APP_REFRESH_MIN        (Uint32T)(1)         /* 1 mSec */
#define REF_SELECT_APP_REFRESH_MAX        (Uint32T)(100000000) /* 100 KSec */

/* Range limits on the Hold-Off Timer (seconds). */
#define REF_SELECT_HOLDOFF_TIMER_MIN      (Uint32T)(0)         /* No Hold-off */
#define REF_SELECT_HOLDOFF_TIMER_MAX      (Uint32T)(300)       /* 300 Sec */

/* Range limits on the Wait-to-Restore Timer (seconds). */
#define REF_SELECT_WTR_TIMER_MIN          (Uint32T)(0)         /* No Wait-to-Restore */
#define REF_SELECT_WTR_TIMER_MAX          (Uint32T)(300)       /* 300 Sec */

/*****************   DATA TYPES (INTERNAL)  ***********************************/

/*****************   DATA STRUCTURES (INTERNAL)  ******************************/

/* This internal structure is not used within the code; it is merely to determine
 * the size of the message buffer required.  */
typedef union exampleGeneralMsgS
{
   exampleProtocolClockUpdateMsgS  clockUpdateMsg;
   exampleProtocolClockDeleteMsgS  clockDeleteMsg;
   exampleProtocolStreamAddMsgS    streamAddMsg;
   exampleProtocolStreamUpdateMsgS streamUpdateMsg;
   exampleProtocolStreamDeleteMsgS streamDeleteMsg;
   examplePtsfUpdateMsgS           ptsfUpdateMsg;
   exampleAlgorithmUpdateMsgS      algorithmUpdateMsg;
} exampleGeneralMsgS;

#define REF_SELECT_DB_MSG_BUFFER_LENGTH   (sizeof(exampleGeneralMsgS) + 1)

/* A local structure for passing messages to the handler task */
typedef struct exampleRefSelectMsgS
{
   /* Source Module Information */
   exampleRefSelectSrcModuleE sourceId;

   /* Connection Information */
   Uint32T connectionId;

   /* Intended Action */
   exampleRefSelectActionE action;

   void *device;
   Uint16T bufLen;
   Uint8T  buffer[REF_SELECT_DB_MSG_BUFFER_LENGTH];
} exampleRefSelectMsgS;

/***************************************************
 * Reference Selection Application structures
 *    (the Config structure is in the Api.h file).
 ***************************************************/
/* Structure to store run-time data for the Reference Selection Application. */
typedef struct exampleRefSelectRunTimeS
{
   /* Parameters related to the server selection behaviour. */
   struct
   {
      /* Data for the last selected server. */
      exampleRefSelectEntryS previousServer;
   } selection;

   /* Run-time Telecom Profile Protection parameters. */
   struct
   {
      /* Hold-off Timer value (for Telecom Profile). */
      struct
      {
         /* The hold-off period (in seconds). */
         zl303xx_BooleanE active;
         /* The CPU tick count when the timer started. */
         Uint32T tickStart;
         /* The QL (class) to use during the hold-off period. */
         Uint32T qlValue;
      } holdOff;
   } protection;

   /* Local Timer values (if active). */
   struct
   {
      /* The Application Refresh Rate (in CPU Ticks). */
      Uint32T appRefreshTicks;
      /* The number of CPU ticks until the Hold-Off Timer expires (if active). */
      Uint32T holdOffRemainingTicks;
      /* The number of CPU ticks until the lowest active WTR timer expires. */
      Uint32T wtrRemainingTicks;
   } timer;
} exampleRefSelectRunTimeS;

/* Structure to store all parameters for the Reference Selection Application. */
typedef struct exampleRefSelectParamS
{
   /* Configuration parameters for the application. */
   exampleRefSelectConfigS config;

   /* Dynamic and run-time parameters for the application. */
   exampleRefSelectRunTimeS rt;
} exampleRefSelectParamS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/********************************/
/*    Task Related Routines.    */
/********************************/
/* Task Initialization */
zlStatusE exampleRefSelectTaskStart(void);
zlStatusE exampleRefSelectTaskStop(void);
zlStatusE exampleRefSelectTaskStatus(void);

/* Core Task */
zlStatusE exampleRefSelectTaskFn(void);

/* Message Queue Creation and Deletion. */
zlStatusE exampleRefSelectMsgQueueCreate(void);
zlStatusE exampleRefSelectMsgQueueDelete(void);

/* Routines to Read & Write the Task Message Queue. */
zlStatusE exampleRefSelectMsgQueueRead(exampleRefSelectMsgS *msg);
zlStatusE exampleRefSelectMsgQueueSend(exampleRefSelectMsgS *msg);

/***************************************/
/*   Server Database Table routines.   */
/***************************************/
zlStatusE exampleRefSelectTableInit(void);
zlStatusE exampleRefSelectTableDelete(void);

/* Routines to manage the Database MUTEX. */
zlStatusE exampleRefSelectMutexCreate(void);
zlStatusE exampleRefSelectMutexDelete(void);
zlStatusE exampleRefSelectMutexLock(void);
zlStatusE exampleRefSelectMutexUnlock(void);

/* Row / Entry management Routines. */
zlStatusE exampleRefSelectRowInit(exampleRefSelectEntryS *pRowEntry);

/* Table search routines. */
zlStatusE exampleRefSelectTableFindProtocolStream(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_PtpClockHandleT clockHandle,
      Uint32T *entryIndex);
zlStatusE exampleRefSelectTableFindProtocolClock(
      zl303xx_PtpClockHandleT clockHandle,
      Uint32T *entryIndex);
zlStatusE exampleRefSelectTableFindConnection(
      Uint32T connectionId,
      Uint32T *entryIndex);

/********************************************************/
/*   Interface Routines (Input) for the module events   */
/********************************************************/
/* This was created with a 'void' return to follow exactly the callback format. */
void exampleInterfacePtsf(zl303xx_PtsfEventS *ptsfEventData);


/*********************************************************************/
/*   Handler Routines for each module (and each supported Action)    */
/*********************************************************************/
/* Protocol Clock */
zlStatusE exampleHandleProtocolClock(
      Uint32T clockId, exampleRefSelectActionE action, Uint8T msgLen, Uint8T *msgBuffer);
zlStatusE exampleHandleProtocolClockUpdate(
      Uint32T clockId, Uint8T msgLen, Uint8T *msgBuffer);
zlStatusE exampleHandleProtocolClockDelete(
      Uint32T clockId, Uint8T msgLen, Uint8T *msgBuffer);

/* Protocol Stream */
zlStatusE exampleHandleProtocolStream(
      Uint32T streamId, exampleRefSelectActionE action, Uint8T msgLen, Uint8T *msgBuffer);
zlStatusE exampleHandleProtocolStreamAdd(
      Uint32T streamId, Uint8T msgLen, Uint8T *msgBuffer);
zlStatusE exampleHandleProtocolStreamUpdate(
      Uint32T streamId, Uint8T msgLen, Uint8T *msgBuffer);
zlStatusE exampleHandleProtocolStreamDelete(
      Uint32T streamId, Uint8T msgLen, Uint8T *msgBuffer);

/* PTSF */
zlStatusE exampleHandlePtsf(
      Uint32T connectionId, exampleRefSelectActionE action, Uint8T msgLen, Uint8T *msgBuffer);
zlStatusE exampleHandlePtsfUpdate(
      Uint32T connectionId, Uint8T msgLen, Uint8T *msgBuffer);

/* Algorithm */
zlStatusE exampleHandleAlgorithm(
      Uint32T serverId, exampleRefSelectActionE action, Uint8T msgLen, Uint8T *msgBuffer);
zlStatusE exampleHandleAlgorithmUpdate(
      Uint32T serverId, Uint8T msgLen, Uint8T *msgBuffer);

/*********************************/
/*   Server Selection Routines   */
/*********************************/
zlStatusE exampleRefSelectFindBest(Uint32T *bestIndex, exampleRefSelectEntryS *bestEntry);
zlStatusE exampleRefSelectBmc1588(Uint32T *bestIndex, exampleRefSelectEntryS *bestEntry);
zlStatusE exampleRefSelectBmcTelecom(Uint32T *bestIndex, exampleRefSelectEntryS *bestEntry);
zlStatusE exampleRefSelectBmcTelecomPhase(Uint32T *bestIndex, exampleRefSelectEntryS *bestEntry);
zlStatusE exampleRefSelectBmcCustom(Uint32T *bestIndex, exampleRefSelectEntryS *bestEntry);

/****************************************/
/*   Local Debug and Utility Routines   */
/****************************************/
zlStatusE exampleRefSelectConfigCheck(exampleRefSelectConfigS *configParams);
zlStatusE exampleRefSelectAppInitRunTimeParams(exampleRefSelectRunTimeS *rtParams);
zl303xx_BooleanE exampleRefSelectEqualServerEntries(exampleRefSelectEntryS *serverA, exampleRefSelectEntryS *serverB);
zlStatusE exampleRefSelectEvaluateAllWtrTimers(void);
zlStatusE exampleRefSelectHandleWtrOfServer(exampleRefSelectEntryS *oldServer, exampleRefSelectEntryS *newServer);
zlStatusE exampleRefSelectConvertMsToTicks(Uint32T milliSec, Uint32T *cpuTicks);

/* Utility to show table entries. */
zlStatusE exampleRefSelectDisplay(
      zl303xx_BooleanE bShowHeader,
      zl303xx_BooleanE bShowValid,
      zl303xx_BooleanE bShowBest);
zlStatusE exampleRefSelectShow(void);        /* Shows all VALID entries in the table. */
zlStatusE exampleRefSelectShowAll(void);     /* Dumps the entire table (including empty rows). */
zlStatusE exampleRefSelectShowBest(void);    /* Shows only the BEST entry from the table (if it exists). */

zlStatusE exampleRefSelectConfigTrace(       /* Shows the data for 2 Task configuration structures. */
      exampleRefSelectConfigS *configA,
      exampleRefSelectConfigS *configB);

#endif /* MULTIPLE INCLUDE BARRIER */
