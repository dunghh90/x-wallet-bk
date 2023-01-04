

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This provides an abstraction to allow the user to implement a task monitoring
      utility.
*
******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_Trace.h"
#include "zl303xx_Macros.h"

#if defined OS_LINUX
#include "zl303xx_LinuxOs.h"
#endif
#if defined __VXWORKS_69
#include <taskLib.h>
#endif

#include <string.h>

#include <string.h>

/*****************   DEFINES     **********************************************/
#define MAX_TASKS 32
#define MAX_TASKSNAMELEN 64


/*****************   TYPEDEFS    **********************************************/

typedef struct {      /* Adjust struct to your needs */
    Uint8T taskName[MAX_TASKSNAMELEN];
    SINT_T taskId;
} taskNameS;

typedef struct {      /* Adjust struct to your needs */
    taskNameS tasks[MAX_TASKS];
} taskMonConfigS;

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/
taskMonConfigS taskMonConfigObj;
void *taskMonConfig = &taskMonConfigObj;

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   IMPORTED GLOBAL VARIABLES   ******************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************   EXPORTED FUNCTION DECLARATIONS   *************************/

/* Use for your own purposes BUT DO NOT DELAY, TAKE MUTEXES, ETC. within these functions */

/* These functions are called at task spawn/delete and the start and end of every task loop. */

/* Notes: PtpTimer task is running at 8ms, performance could be impacted 
          PTP Rx task is running at pkt rate, performance could be impacted 
          QIF task is running at pkt rate and 125ms, performance could be impacted 
          Signalhandler task has to handle all h/w timers (8ms and 125ms), performance could be impacted 
*/


/**
  Function Name: OS_TASKMON_CONFIG

  Details:
   Create a monitor config info entry for a created task or thread

  Parameters:
       taskId        The task identifier to be created
       taskMonCfg    Pointer to some struct of task info

  Return Value:
   OS_OK, or OS_ERROR if the task monitor cannot be created
********************************************************************/
zlStatusE OS_TASKMON_CONFIG(SINT_T taskId, void *taskMonCfg);
zlStatusE OS_TASKMON_CONFIG(SINT_T taskId, void *taskMonCfg)
{
    zlStatusE status = ZL303XX_OK;
#if defined STUB_TASKMON_UTILS
    Uint8T indx = 0;
    /* Initialise whatever structure you require (currently taskMonConfig) (Called once per task during OS_TASK_SPAWN()) */
    taskMonConfigS *taskMonPtr = taskMonCfg;

    while (indx < MAX_TASKS)
    {
        if (taskMonPtr->tasks[indx].taskId == 0)    /* Add new task info. to the array of tasks */
        {
            taskMonPtr->tasks[indx].taskId = taskId;
            strncpy((char*)taskMonPtr->tasks[indx].taskName, (char*)taskName(taskId), sizeof(taskMonPtr->tasks[indx].taskName) -1);

            printf("OS_TASKMON_CONFIG: @indx=%d, New taskId=0x%x (%d) is %s\n", indx, taskMonPtr->tasks[indx].taskId, taskMonPtr->tasks[indx].taskId, taskMonPtr->tasks[indx].taskName);

            /* Add additional logic here on a per TASK basis */

            break;
        }
        indx++;
    }

#else
    (void) taskId;
    (void) taskMonCfg;
#endif  /* STUB_TASKMON_UTILS */
    return status;
}

/**
  Function Name: OS_TASKMON_DELETE

  Details:
   Delete a monitor config info entry for a created task or thread

  Parameters:
       taskId        The task identifier to be deleted
       taskMonCfg    Pointer to some struct of task info

  Return Value:
   OS_OK, or OS_ERROR if the task monitor cannot be deleted
********************************************************************/
zlStatusE OS_TASKMON_DELETE(SINT_T taskId, void *taskMonCfg);
zlStatusE OS_TASKMON_DELETE(SINT_T taskId, void *taskMonCfg)
{
    zlStatusE status = ZL303XX_OK;
#if defined STUB_TASKMON_UTILS
    Uint8T indx = 0;
    /* Destroy whatever structure you require (currently taskMonConfig) (Called once per task during OS_TASK_DELETE() ) */
    taskMonConfigS *taskMonPtr = taskMonCfg;

    (void)taskMonCfg;   /* Warning removal */

    while (indx < MAX_TASKS)
    {
        if (taskMonPtr->tasks[indx].taskId == taskId)    /* Remove task info. from the array of tasks */
        {
            printf("OS_TASKMON_DELETE: @indx=%d, Deleted taskId=0x%x (%d) is %s\n", indx, taskMonPtr->tasks[indx].taskId, taskMonPtr->tasks[indx].taskId, taskMonPtr->tasks[indx].taskName);

            taskMonPtr->tasks[indx].taskId = 0;
            memset((char*)taskMonPtr->tasks[indx].taskName, 0, sizeof(taskMonPtr->tasks[indx].taskName));

            break;
       }
        indx++;
    }

#else
    (void) taskId;
    (void) taskMonCfg;
#endif  /* STUB_TASKMON_UTILS */
    return status;
}

/**
  Function Name: OS_TASKMON_CHECK_IN

  Details:
   Start of a loop (monitor a task or thread)

  Parameters:
       taskId        The task identifier to be monitored
       taskMonCfg    Pointer to some struct of task info

  Return Value:
   OS_OK, or OS_ERROR if the task monitor cannot be started
********************************************************************/
zlStatusE OS_TASKMON_CHECK_IN(SINT_T taskId, void *taskMonCfg);
zlStatusE OS_TASKMON_CHECK_IN(SINT_T taskId, void *taskMonCfg)
{
    /* Called at the START of a task loop operation */
    /* DO NOT delay, take mutexes, etc. within these functions */
    zlStatusE status = ZL303XX_OK;
#if defined STUB_TASKMON_UTILS
    Uint8T indx = 0;
    taskMonConfigS *taskMonPtr = taskMonCfg;

    while (indx < MAX_TASKS)
    {
        if (taskMonPtr->tasks[indx].taskId == taskId)
        {
            /* Do something simple! */    

            break;
        }
        indx++;
    }

#else
    (void) taskId;
    (void) taskMonCfg;
#endif  /* STUB_TASKMON_UTILS */
    return status;
}

/**
  Function Name: OS_TASKMON_CHECK_OUT

  Details:
   End of a loop (monitor a task or thread)

  Parameters:
       taskId        The task identifier to be monitored
       taskMonCfg    Pointer to some struct of task info

  Return Value:
   OS_OK, or OS_ERROR if the task monitor cannot be ended
********************************************************************/
zlStatusE OS_TASKMON_CHECK_OUT(SINT_T taskId, void *taskMonCfg);
zlStatusE OS_TASKMON_CHECK_OUT(SINT_T taskId, void *taskMonCfg)
{
    /* Called at the END of a task loop operation */
    /* DO NOT delay, take mutexes, etc. within these functions */
    zlStatusE status = ZL303XX_OK;
#if defined STUB_TASKMON_UTILS
    Uint8T indx = 0;
    taskMonConfigS *taskMonPtr = taskMonCfg;

    while (indx < MAX_TASKS)
    {
        if (taskMonPtr->tasks[indx].taskId == taskId)
        {
            /* Finish whatever was done in OS_TASKMON_CHECK_IN */

            break;
        }

        indx++;
    }   /* while */

#else
    (void) taskId;
    (void) taskMonCfg;
#endif  /* STUB_TASKMON_UTILS */
    return status;
}


