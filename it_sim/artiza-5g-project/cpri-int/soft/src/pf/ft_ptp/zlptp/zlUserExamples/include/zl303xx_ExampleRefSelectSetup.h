

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*  Contains default settings and parameters to manage the Reference Selection
*  Application.
*
*******************************************************************************/

#ifndef _ZL303XX_EXAMPLE_REF_SELECT_SETUP_H_
#define _ZL303XX_EXAMPLE_REF_SELECT_SETUP_H_

/*****************   INCLUDE FILES   ******************************************/

/*****************   DEFINES   ************************************************/

/******************************/
/*      Task Definition.      */
/******************************/
#define REF_SELECT_TASK_NAME                       "RefSelectTask"
#define REF_SELECT_TASK_STACK_SIZE                 4000

#ifdef OS_LINUX
   #define REF_SELECT_TASK_PRIORITY                90
#else
   #define REF_SELECT_TASK_PRIORITY                60
#endif

/* Message Queue Definition. */
#define REF_SELECT_DB_MSGQ_SIZE                    16

/* Default Message Queue Time-Out */
#define REF_SELECT_DEFAULT_MSGQ_TIME_OUT           OS_TICK_RATE_GET()   /* 1 Second */


/***********************************************************/
/*    Reference Selection Application default settings.    */
/***********************************************************/
/* Application Refresh Rate. */
#define REF_SELECT_DEFAULT_APP_REFRESH_RATE_MS        1000           /* 1000 milliseconds = 1 sec */

/* Selection Behaviour. */
#define REF_SELECT_DEFAULT_AUTO_SELECTION_MODE        ZL303XX_TRUE     /* Automatically Run Server Selection. */
#define REF_SELECT_DEFAULT_BMCA_ALG                   REF_SELECT_BMCA_DEFAULT_PROFILE  /* Use IEEE-1588 BMC */
#define REF_SELECT_DEFAULT_FORCED_REF_ID              REF_SELECT_CONN_ID_INVALID       /* No forced reference selection. */

/* Telecom Profile Protection Mechanism. */
#define REF_SELECT_DEFAULT_TP_REVERTIVE_MODE          ZL303XX_FALSE    /* No revertive switching by default. */
#define REF_SELECT_DEFAULT_TP_HOLDOFF_SECONDS         0              /* QL Hold-off disabled by default. */
#define REF_SELECT_DEFAULT_TP_SQUELCH_MODE            ZL303XX_FALSE    /* Squelch Protection off by default. */

/*****************************************/
/*    The Database Table Definitions.    */
/*****************************************/
/* Maximum number of entries */
#define REF_SELECT_DB_ENTRIES_MAX                  128

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS (MESSAGES)   **************/



#endif /* MULTIPLE INCLUDE BARRIER */
