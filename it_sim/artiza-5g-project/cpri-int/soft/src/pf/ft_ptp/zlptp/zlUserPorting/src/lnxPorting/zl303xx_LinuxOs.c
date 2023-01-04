

/******************************************************************************
 *
 *  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
 *
 *  Copyright 2006-2018 Microsemi Semiconductor Limited.
 *  All rights reserved.
 *
 *  Module Description:
 *     This provides an abstraction for various functions provided by an RTOS. The
 *     aim is to make it easier to port a system to a different 'host'
 *
 *     This file is an example Linux implementation of the abstract interface defined
 *     in zl303xx_Os.h
 *
 ******************************************************************************/

#ifdef ZL_LNX_CODESOURCERY
#define NEED_SYSDEP_TIME
#endif

#if !defined ZL_LNX_INTEL
#define NEED_LINUX_TIME
#endif
#define NEED_POSIX_TIME

#include "zl303xx_Global.h"
#include "zl303xx_LinuxOs.h"
#include "zl303xx_Trace.h"
#include "zl303xx_Macros.h"

#include <pthread.h>
#include <sys/prctl.h>
#include <linux/prctl.h>

#if defined ZL_LNX_DENX || defined ZL_LNX_CODESOURCERY || defined ZL_LNX_INTEL /* warning removal */
extern int clock_gettime(clockid_t clk_id, struct timespec *tp);
extern int pthread_condattr_setclock (pthread_condattr_t *__attr, __clockid_t __clock_id);
#endif

/*****************   # DEFINES   **********************************************/

/*****************   FUNCTION DECLARATIONS   *************************/
int nanosleep(const struct timespec *req, struct timespec *rem);
int pthread_mutex_timedlock(pthread_mutex_t *, const struct timespec *);


    static Uint8T cliTaskPriority = 0;

    Uint8T zl303xx_GetCliPriority(void);
    Uint8T zl303xx_GetCliPriority(void)
    {
        return cliTaskPriority;
    }
    zlStatusE zl303xx_SetCliPriority(Uint8T taskPriority);
    zlStatusE zl303xx_SetCliPriority(Uint8T taskPriority)
    {
        cliTaskPriority = taskPriority;
        return ZL303XX_OK;
    }

/*****************   Data structures   **************************************/

/*****************    Extern Global Variables   ***********************************/
#if defined OS_CANNOT_PTHREAD_CANCEL_CORRECTLY && defined APR_INCLUDED
extern zl303xx_BooleanE stopTaskA;
#endif

/*****************    Global Variables   ***********************************/
Uint32T parentTaskId = 0xFFFFFFFF;                           /* Command Line Task ID (the parent of all Linux pThreads) */

#if defined USE_DEFERRED_FREE 
deferredFreeT deferredLstHead = {NULL, NULL, NULL, NULL}; /* List of locations to free at the end of shutdown (helps to work around async pthread condvar issues) */
deferredFreeT *pDeferredLstHead = NULL;
#endif

/*****************   Static Global variables    ***********************************/

static taskInfoS parentTaskStruct;                              /*  CLI's taskInfoS struct. will start the list in exampleSetup() */
static taskInfoS *taskList = NULL;                             /*  taskList is a linked list of taskInfoS structs */

/*****************   Fwd Declaration of Static functions   ***************************************/
static void * entryPtWrapper(void *arg);
static semInfoS *osCreateSemaphoreStruct(void);
static void ticksToTime(struct timespec *tm, int ticks);
static void copyMsgToQ(msgQInfoS *msgQInfo, Sint8T *buffer, Uint32T nBytes);
static void copyMsgFromQ(msgQInfoS *msgQInfo, Sint8T *buffer, Uint32T nBytes);

/*****************   General Constants   **************************************/

const osStatusT OsStatusOk = (osStatusT)OK;
const osStatusT OsStatusError = (osStatusT)ERROR;
const Uint32T OsRandMax = (Uint32T)RAND_MAX;

const osSMiscT OsWaitForever = (osSMiscT)WAIT_FOREVER;
const osSMiscT OsTimeout = (osSMiscT)WAIT_TIMEOUT;
const osSMiscT OsNoWait = (osSMiscT)NO_WAIT;

const osMiscT OsSemInvalid = (osMiscT)NULL;
const osMiscT OsMsgQInvalid = (osMiscT)NULL;


/***************************************   Functions   ***************************************/

#if !defined OS_LINUX_USE_ASYNC_CANCEL
/* Cancellation handler to unlock the given mutex
 */
static void cancellationUnlockMutex(void *arg)
{
    pthread_mutex_unlock((pthread_mutex_t*) arg);
}

    /* Wrappers to force pthread_cond_wait and pthread_cond_timedwait to unlock
       mutex if the task is cancelled while waiting for condition. This is only
       necessary with PTHREAD_CANCEL_DEFERRED task cancellation type (default).
       See Posix Std. IEEE 1003.1-2008 line 51015
     */
    #define RELEASE_MUTEX_IF_CANCELLED_BEGIN(pMutex) pthread_cleanup_push(cancellationUnlockMutex, pMutex);
    #define RELEASE_MUTEX_IF_CANCELLED_END(pMutex) pthread_cleanup_pop(0);
#else
    #define RELEASE_MUTEX_IF_CANCELLED_BEGIN(pMutex) /* Do nothing */
    #define RELEASE_MUTEX_IF_CANCELLED_END(pMutex)   /* Do nothing */
#endif

void osPrintTasks(void);
void osPrintTasks(void)
{
    taskInfoS *t = taskList;     /* Head of the task list */
    Uint32T i = 0;

    if (!t)
    {
        fprintf(stderr, "osPrintTasks: NULL taskList!\n");
        return;
    }

    for (t = taskList; t != NULL; t = t->nxtTask)
    {
        i++;
        fprintf(stderr, "%d. %s %p (pthreadid %#x)\n", i, t->taskName, (void *)t, (unsigned int)t->thread_id);
        fflush(stderr);
    }

    fprintf(stderr, "\n");
}

static void cleanTaskList(taskInfoS *taskInfo)
{
    taskInfoS *t = taskList;     /* Head of the task list */
    taskInfoS *prvTask = taskList;

    if (!t)
    {
        fprintf(stderr, "cleanTaskList: NULL taskList!\n");
        return;
    }

#ifdef ZL_DEBUG_TASKS
    fprintf(stderr, "Trying to delete tid=0x%x (%s) from taskList\n", taskInfo->thread_id, taskInfo->taskName);
#endif
    for (t = taskList; t != NULL; t = (taskInfoS*)t->nxtTask)
    {
        if (t->thread_id == taskInfo->thread_id)
        {
            prvTask->nxtTask = t->nxtTask; /* remove from linked list */

            OS_FREE(t);
            t = NULL;
            break;
        }
        else
        {
        #ifdef ZL_DEBUG_TASKS
            fprintf(stderr, "prvTask=0x%x (%s) and t->nxtTask=0x%x \n", prvTask, prvTask->taskName, t->nxtTask);
        #endif
            prvTask = t;     /* Move down the list */
        }
    }
}

/* Mark the resource as DONE */
static void cancellationMarkDone(void *arg)
{
    taskInfoS *taskInfo = (taskInfoS *) arg;
    if (taskInfo != NULL)
    {
        taskInfo->isDone = 1;

    }
}



static void * entryPtWrapper(void *arg) __attribute__((noreturn));
static void * entryPtWrapper(void *arg)
{
    SnativeT returnVal;
    /* Cast argument type to its correct type */
    taskInfoS *taskInfo = (taskInfoS *)arg;



#if !defined OLDER_LINUX
    if ((returnVal = prctl(PR_SET_NAME, taskInfo->taskName, 0, 0, 0)) != 0)
    {
        fprintf(stderr, "entryPtWrapper: Error in prctl, err=%d\n", (int)returnVal);
    }
#endif

    if (1)
    {
        int oldtype;

#if !defined OS_LINUX_USE_ASYNC_CANCEL
        /* Use PTHREAD_CANCEL_DEFERRED type (default) */
        (void) pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldtype);
#else
        /* Use PTHREAD_CANCEL_ASYNCHRONOUS type (not recommended) explicit override required*/
        #error PTHREAD_CANCEL_ASYNCHRONOUS cancel type is not recommended
        (void) pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
#endif
    }

    /* Prepare for to mark as done on async cancellation */
    pthread_cleanup_push(&cancellationMarkDone, (void*)taskInfo);

    /* Call the function */
    returnVal = (*(taskInfo->entryPt))(taskInfo->taskArg);

    /* Mark as done and remove cancellation handler */
    pthread_cleanup_pop(1);

    taskInfo->returnVal = returnVal;

#if defined ENFORCE_STRICT_SHUTDOWN
    fprintf(stderr, "entryPtWrapper for %s was not cancelled; exit return=%d \n", taskInfo->taskName, returnVal);

    handleMechanismIssues(__FILE__, __LINE__, MECH_TASK, NULL, taskInfo->thread_id, NO_ABORT);
#endif

    pthread_exit(&returnVal);
}

/*****************************************************************************
 ** taskName - returns the name string of the calling task
 *****************************************************************************/
Uint8T* taskName(pthread_t threadId)
{
    taskInfoS *t = NULL;

    for (t = taskList; t != NULL; t = t->nxtTask) {   /* Threads numbers increase */
        if (pthread_equal(threadId, t->thread_id)) {
#ifdef ZL_DEBUG_TASK_TOOLS
            fprintf(stderr, "Found taskName=%s\n", t->taskName);
#endif
            return t->taskName;
        }
    }

    return NULL;
}


/*****************************************************************************
 ** taskIdSelf - returns the identifier of the calling task
 *****************************************************************************/
Uint32T taskIdSelf(void)
{
    pthread_t myThreadId = pthread_self();
    taskInfoS *t = NULL;

    for (t = taskList; t != NULL; t = t->nxtTask) {   /* Threads increase in number */
        if (pthread_equal(myThreadId, t->thread_id)) {
#ifdef ZL_DEBUG_TASK_TOOLS
            fprintf(stderr, "Found myself=0x%x (%s)\n", t->thread_id, t->taskName);
#endif
            return t->thread_id;
        }
#ifdef ZL_DEBUG_TASK_TOOLS
        else
            fprintf(stderr, "Not me=0x%x (%s); NextAddr=%p\n", t->thread_id, t->taskName, t->nxtTask);
#endif
    }

    return OsStatusError;
}

/*
  Function Name: osTaskSpawn

  Details:
   Creates and then starts a new concurrent programming object (i.e. a task,
   a thread or a process)

  Parameters:
       name        name of new task
       priority    priority of new task
               In the range OS_TASK_MIN_PRIORITY to OS_TASK_MAX_PRIORITY
       unused      An unused parameter for backwards compatibility
       stackSize   size in bytes of stack needed plus name
       entryPt     entry point of new task
       taskArg     argument to pass to entryPt func

  Return Value:
   ID of the created task or OS_TASK_INVALID if it could not be created

*****************************************************************************/
OS_TASK_ID osTaskSpawn(
        const char *name,
        Sint32T priority,
        Sint32T unused,
        Sint32T stackSize,
        OS_TASK_FUNC_PTR entryPt,
        Sint32T taskArg
)
{
    struct sched_param schedParam;
    pthread_t thread_id;

    Sint32T ret = 0;
    /* Create a taskInfo structure */
    taskInfoS *taskInfo = (taskInfoS *)OS_CALLOC(1, sizeof(taskInfoS));

    unused++;   /* warning removal */

    if (taskInfo == NULL)
    {
        ZL303XX_ERROR_TRAP("osTaskSpawn: Cannot create task");
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_TASK, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
        return OS_TASK_INVALID;
    }


    /* Set the parameters that we pass through the wrapper */
    taskInfo->entryPt = entryPt;
    taskInfo->taskArg = taskArg;
    taskInfo->returnVal = 0;
    taskInfo->isDone = 0;

    memset(taskInfo->taskName, 0x00, sizeof(taskInfo->taskName));
    strncpy((char*)taskInfo->taskName, name, sizeof(taskInfo->taskName)-1);

    /* Initialise and then set the attributes */
    if ((ret = pthread_attr_init(&taskInfo->attr)) != 0)
    {
        OS_FREE(taskInfo);
        fprintf(stderr, "osTaskSpawn: Error in pthread_attr_init, err=%d\n", ret);
        ZL303XX_ERROR_TRAP("osTaskSpawn: Error in pthread_attr_init");
        return OS_TASK_INVALID;
    }

#if !defined USE_LEGACY_PTHREAD_DETACH
    if ((ret = pthread_attr_setdetachstate(&taskInfo->attr, PTHREAD_CREATE_JOINABLE)) != 0)
    {
        OS_FREE(taskInfo);
        fprintf(stderr, "osTaskSpawn: Error in pthread_attr_setdetachstate, err=%d\n", ret);
        ZL303XX_ERROR_TRAP("osTaskSpawn: Error in pthread_attr_setdetachstate");
        return OS_TASK_INVALID;
    }
#endif

    /* Set the stacksize */
    stackSize += PTHREAD_STACK_MIN;
    
    #if !defined STACK_ALIGN
    #define STACK_ALIGN 16
    #endif
    /* ceil up to nearest STACK_ALIGN size */
    stackSize = ((stackSize + STACK_ALIGN-1) / STACK_ALIGN) * STACK_ALIGN;
    
#if defined ZL_DEBUG_TASKS 
   fprintf(stderr, "%s - Configured stackSize NOW= %d \n", taskInfo->taskName, stackSize);
#endif


    if ((ret = pthread_attr_setstacksize(&taskInfo->attr, (Uint32T)stackSize)) != 0)
    {
        OS_FREE(taskInfo);
        fprintf(stderr, "osTaskSpawn: Error in pthread_attr_setstacksize, errno=%d\n", errno);
        ZL303XX_ERROR_TRAP("osTaskSpawn: Error in pthread_attr_setstacksize");
        return OS_TASK_INVALID;
    }

    /* Set the scheduling to allow specific priorities to be set and inherited. */
    if ((ret = pthread_attr_setinheritsched(&taskInfo->attr, CHILDREN_INHERIT)) != 0)
    {
        OS_FREE(taskInfo);
        fprintf(stderr, "osTaskSpawn: Error in pthread_attr_setinheritsched, err=%d\n", ret);
        ZL303XX_ERROR_TRAP("osTaskSpawn: Error in pthread_attr_setinheritsched");
        return OS_TASK_INVALID;
    }

    if ((ret = pthread_attr_getschedparam(&taskInfo->attr, &schedParam)) != 0)
    {
        OS_FREE(taskInfo);
        fprintf(stderr, "osTaskSpawn: Error in pthread_attr_getschedparam, err=%d\n", ret);
        ZL303XX_ERROR_TRAP("osTaskSpawn: Error in pthread_attr_getschedparam");
        return OS_TASK_INVALID;
    }

{
    int cliPriority = 0;

    if (priority > OS_TASK_MAX_PRIORITY || priority <= OS_TASK_MIN_PRIORITY)
    {
        OS_FREE(taskInfo);
        fprintf(stderr, "osTaskSpawn: Error in provided priority=%d > %d or <= %d\n", priority, OS_TASK_MAX_PRIORITY, OS_TASK_MIN_PRIORITY);
        ZL303XX_ERROR_TRAP("osTaskSpawn: Error in provided priority");
        return OS_TASK_INVALID;
    }

    if ((cliPriority = zl303xx_GetCliPriority()) == OS_OK)    /* If not user-defined then just use priority */
    {
        schedParam.sched_priority = priority;
    }
    else    /* Calc the diff from the max. and apply against the user-defined priority */
    {
        schedParam.sched_priority = cliPriority - abs(sched_get_priority_max(SCHED_POLICY) - priority);
    }
}

    /* Real-time using SCHED_POLICY */
    if ((ret = pthread_attr_setschedpolicy(&taskInfo->attr, SCHED_POLICY)) != 0)
    {
        OS_FREE(taskInfo);
        fprintf(stderr, "osTaskSpawn: Error in pthread_attr_setschedpolicy, err=%d\n", ret);
        ZL303XX_ERROR_TRAP("osTaskSpawn: Error in pthread_attr_setschedpolicy");
        return OS_TASK_INVALID;
    }

    if ((ret = pthread_attr_setschedparam(&taskInfo->attr, &schedParam)) != 0)
    {
        OS_FREE(taskInfo);
        fprintf(stderr, "osTaskSpawn: Error in pthread_attr_setschedparam, err=%d\n", ret);
        ZL303XX_ERROR_TRAP("osTaskSpawn: Error in pthread_attr_setschedparam");
        return OS_TASK_INVALID;
    }

    if ((ret = pthread_create(&thread_id, &taskInfo->attr, entryPtWrapper, (void *)taskInfo)) != 0)
    {
        OS_FREE(taskInfo);
        fprintf(stderr, "osTaskSpawn: Error in pthread_create, err=%d\n", ret);
        ZL303XX_ERROR_TRAP("osTaskSpawn: Error in pthread_create");
        return OS_TASK_INVALID;
    }

    taskInfo->thread_id = thread_id;

    /* Apply to this thread */
    if ((ret = pthread_setschedparam(taskInfo->thread_id, SCHED_POLICY, &schedParam)) != 0)
    {
        OS_FREE(taskInfo);
        fprintf(stderr, "osTaskSpawn: Error in pthread_setschedparam, err=%d\n", ret);
        ZL303XX_ERROR_TRAP("osTaskSpawn: Error in pthread_setschedparam");
        return OS_TASK_INVALID;
    }

    if ((ret = pthread_attr_destroy(&taskInfo->attr)) != 0)
    {
        OS_FREE(taskInfo);
        fprintf(stderr, "osTaskSpawn: Error in pthread_attr_destroy, err=%d\n", ret);
        ZL303XX_ERROR_TRAP("osTaskSpawn: Error in pthread_attr_destroy");
        return OS_TASK_INVALID;
    }

#if defined USE_LEGACY_PTHREAD_DETACH
    /* Detach this thread */
    if ((ret = pthread_detach(taskInfo->thread_id)) != 0)
    {
        OS_FREE(taskInfo);
        fprintf(stderr, "osTaskSpawn: Error in pthread_detach, err=%d\n", ret);
        ZL303XX_ERROR_TRAP("osTaskSpawn: Error in pthread_detach");
        return OS_TASK_INVALID;
    }
#endif

    /* Keep a master task list */
    {
        taskInfoS *t = NULL;

        if (taskList == NULL)
        {
           /* Setup the taskInfo list head "taskList" */
           memset(&parentTaskStruct, 0, sizeof(taskInfoS));
           strncpy((char*)parentTaskStruct.taskName, "CLI", sizeof(parentTaskStruct.taskName));
           if ((parentTaskStruct.taskPri = zl303xx_GetCliPriority()) != 0)    /* If not user-defined then just use highest priority */
           {
               parentTaskStruct.taskPri = sched_get_priority_max(SCHED_POLICY);
           }
           parentTaskStruct.thread_id = pthread_self();
           parentTaskId = parentTaskStruct.thread_id;  /* Parent of all pthreads */
           taskList = &parentTaskStruct;               /* First task of the list */
        }

        for (t = taskList; t->nxtTask != NULL; t = t->nxtTask)
            continue; /* Skip to last known task */

        if(taskInfo != t) {
            t->nxtTask = taskInfo;     /* Add our new task to the list */
        }
        strncpy((char*)&(taskInfo->taskName), name, sizeof(taskInfo->taskName));        /* Just in case */
        taskInfo->taskName[sizeof(taskInfo->taskName)-1] = '\0';
        taskInfo->taskPri = schedParam.sched_priority;
#ifdef ZL_DEBUG_TASKS
        fprintf(stderr, "ThreadId= 0x%x == %s, taskPri=%d\n", taskInfo->thread_id, taskInfo->taskName, taskInfo->taskPri);
#endif

        OS_TASKMON_INIT(taskInfo->thread_id);
    }

    return (OS_TASK_ID)taskInfo;
}

/*
  Function Name: osTaskDelete

  Details:
   Deletes a previously created task or thread

  Parameters:
       tid         The task identifier to be destroyed

  Return Value:
   OS_OK, or OS_ERROR if the task cannot be deleted

  Notes:
  WARNING: For proper shutdown a few sequencing rules are required:
  1) Tasks to be deleted should take some external stimulus to cause it to enter into
  an OS_TASK_DELAY(2000); and
  2) The task being deleted cannot be holding a mutex or semaphore nor be pending on a
  message queue or the task will not be deleted cleanly.
 *****************************************************************************/
osStatusT osTaskDelete (OS_TASK_ID tid)
{
    taskInfoS *taskInfo = (taskInfoS *)tid;
    Sint32T retVal = OS_OK;

    if (!taskInfo->thread_id)
    {
        fprintf(stderr, "osTaskDelete: tid invalid!\n");
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_TASK, (void*)tid, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
        return OS_ERROR;
    }

#if defined OS_CANNOT_PTHREAD_CANCEL_CORRECTLY && defined APR_INCLUDED
    if ( 0 == strcmp("zlAprDelayTask", (const char *)taskInfo->taskName))
    {
        fprintf(stderr, "osTaskDelete: forcedStop taskName= %s \n", taskInfo->taskName);
        stopTaskA = ZL303XX_TRUE;
    }
    else
    {
        fprintf(stderr, "osTaskDelete: forcedStop taskName= %s \n", taskInfo->taskName);
    }

    /* Task already exited by returning from entryPtWrapper function - Not recommended! */
    retVal = OS_OK;
#else
     if ((retVal = pthread_cancel(taskInfo->thread_id)) != OS_OK)
     {
         fprintf(stderr, "osTaskDelete: pthread_cancel of tid=0x%x (%s) failed; errno=%d!\n",
                 (unsigned int)taskInfo->thread_id, (UnativeT)taskInfo->taskName, (int)retVal);
     }
#endif

#if !defined USE_LEGACY_PTHREAD_DETACH
    OS_TASK_DELAY(500);	/* Allow cancellation time to write to taskInfo->isDone */

    if (retVal == OS_OK)
    {
        void *thrReturnVal = NULL;

        /* Wait for this thread */
        if ((retVal = pthread_join(taskInfo->thread_id, (void**)&thrReturnVal)) != OS_OK)
        {
            fprintf(stderr, "osTaskDelete: Error in pthread_join for %p thrReturnVal=%p, err=%d (%s)\n", (void *)taskInfo->thread_id, (void *)thrReturnVal, retVal, (char*)strerror(retVal));
            ZL303XX_ERROR_NOTIFY("osTaskDelete: Error in pthread_join");
        }
        else
            fprintf(stderr, "osTaskDelete: taskName= %s had returnVal=%d\n", taskInfo->taskName, taskInfo->returnVal);
    }
#endif

    fflush(stderr);

    OS_TASKMON_END(taskInfo->thread_id);

    cleanTaskList(taskInfo);

    if (retVal == OS_OK)
    {
        return(retVal);
    }
    else
    {
        fprintf(stderr, "osTaskDelete: Error deleting task. retVal = %d\n", retVal);
        return (OS_ERROR);
    }
}

/*
  Function Name: osTaskDelay

  Details:
   Suspends the calling task until the specified number of ticks has elapsed

  Parameters:
       milli       The number of millisecs to delay.

  Return Value:
   OS_OK, or OS_ERROR if an error occurs or if the calling task receives
   a signal that is not blocked or ignored.

  Notes:
   Should not be called from interrupt level

*******************************************************************************/
osStatusT osTaskDelay(Sint32T milli)
{
    #ifndef TEN_e6
        #define TEN_e6 1000000
    #endif
    osStatusT retVal = OS_OK;
    struct timespec deltaTime;

    deltaTime.tv_sec = 0;
    if (milli >= 1000)  /* Seconds */
    {
        deltaTime.tv_sec = milli / 1000;
        milli = milli % 1000;
    }

    if (!deltaTime.tv_sec && !milli)
        milli = 1000 / OS_TICK_RATE_GET();  /* Min. is one userspace tick */

    deltaTime.tv_nsec = milli * TEN_e6;    /* Nanos */

    while ((retVal = nanosleep(&deltaTime, &deltaTime)) == -1 && errno == EINTR)
    {
        pthread_testcancel();
    }
    pthread_testcancel();   /* Create an artificial cancellation point */

    return retVal;

}

/*****************   Semaphores *****************************************/

/*
  Function Name: osSema4Create

   This routine allocates and initializes a counting semaphore. The semaphore is initialized to
   the specified initial count.

   OS_SEM_ID osSema4Create
       (
       int initialCount - initial count -
       )

   RETURNS
   The semaphore ID, or OS_SEM_INVALID if memory cannot be allocated.
*/
OS_SEM_ID osSema4Create(Sint32T initialCount)
{
    semInfoS *semInfo = NULL;

    semInfo = osCreateSemaphoreStruct();

    if (semInfo == NULL)
    {
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_SEMA, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
        return OS_SEM_INVALID;
    }
    semInfo->semType = SEM_TYPE_COUNT;

    /* Set initial value */
    semInfo->count = initialCount;

    return (OS_SEM_ID)semInfo;
}

/*
  Function Name: osSema4CreateBinary

   Create and initialise a binary semaphore

   OS_SEM_ID osSema4CreateBinary
       (
       osMiscT initialState     - initial semaphore state full=1 or empty=0.

       )

   The initialState is SEM_FULL (0x1) or SEM_EMPTY (0x0).

   RETURNS
   Returns a semaphore ID or OS_SEM_INVALID on failure.
*/
OS_SEM_ID osSema4CreateBinary(osMiscT initialState)
{
    semInfoS *semInfo = NULL;

    semInfo = osCreateSemaphoreStruct();

    if (semInfo == NULL)
    {
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_SEMA, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
        return OS_SEM_INVALID;
    }
    semInfo->semType = SEM_TYPE_BINARY;

    /* Set initial value */
    semInfo->count = (initialState ? 1 : 0);

    return (OS_SEM_ID)semInfo;
}

/* Semaphore Create helper function for Linux */
static semInfoS *osCreateSemaphoreStruct(void)
{
    semInfoS *semInfo = NULL;
    pthread_condattr_t attr;

    semInfo = (semInfoS *)OS_CALLOC(1, sizeof(semInfoS));

    if (semInfo == NULL)
    {
        fprintf(stderr, "osCreateSemaphoreStruct: Semaphore memory allocation error\n");
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_SEMA, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
        return NULL;
    }
    if (pthread_mutex_init(&semInfo->lock, NULL) != 0)
    {
        fprintf(stderr, "osCreateSemaphoreStruct: Mutex create error\n");
        OS_FREE(semInfo);
        return NULL;
    }
    if (pthread_condattr_init(&attr) != 0)
    {
        ZL303XX_ERROR_TRAP("osCreateSemaphoreStruct: Condition attr error");
        (void)pthread_mutex_destroy(&semInfo->lock);
        OS_FREE(semInfo);
        return NULL;
    }
    else
    {
        if (pthread_condattr_setclock(&attr, CLOCK_MONOTONIC) != 0)
        {
            ZL303XX_ERROR_TRAP("osCreateSemaphoreStruct: Condition attr_setclock error");
            (void)pthread_condattr_destroy(&attr);
            (void)pthread_mutex_destroy(&semInfo->lock);
            OS_FREE(semInfo);
            return NULL;
        }
    }

    if (pthread_cond_init(&semInfo->condv, &attr) != 0)
    {
        fprintf(stderr, "osCreateSemaphoreStruct: Condition variable create error\n");
        (void)pthread_condattr_destroy(&attr);
        (void)pthread_mutex_destroy(&semInfo->lock);
        OS_FREE(semInfo);
        return NULL;
    }

    (void)pthread_condattr_destroy(&attr);

    semInfo->inShutdown = ZL303XX_FALSE;

    return semInfo;
}

/*
  Function Name: osSema4Give

   This routine performs the give operation on a specified semaphore. The state of the
   semaphore and of the pending tasks may be affected.

   osStatusT osSema4Give
       (
       OS_SEM_ID semId - semaphore ID to give -
       )

   RETURNS
   OS_OK, or OS_ERROR if the semaphore ID is invalid.
*/
osStatusT osSema4Give(OS_SEM_ID semId)
{
    semInfoS *semInfo = (semInfoS *)semId;
    osStatusT retVal = OS_OK;

    if (semId == 0)
    {
        fprintf(stderr, "osSema4Give: Mutex NULL error\n");
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_SEMA, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
        return OS_ERROR;
    }

    /* This check requires the memory to still be valid (not free) */
    if (semInfo->inShutdown)
    {
        fprintf(stderr, "!!!!!!!!!!!!!!!! osSema4Give: semInfo=0x%x (%d) Give during shutdown by %s\n",  semInfo, semInfo, taskName(taskIdSelf()));
        fflush(stderr);
    #if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_SEMA, (void*)semId, semInfo->inShutdown, ABORT_ON_MINOR);
    #endif
        return OS_ERROR; /* Don't call any pthread functions if going down */
    }

    /* Lock access to the counter */
    if (pthread_mutex_lock(&semInfo->lock) != 0)
    {
        fprintf(stderr, "osSema4Give: Error locking mutex\n");
        return OS_ERROR;
    }

    semInfo->count++;

    if (semInfo->semType == SEM_TYPE_BINARY)
    {  /* Binary semaphores only have value 0 or 1 */
        if (semInfo->count > 1)
        {
            semInfo->count = 1;
        }
    }

    /* Wake up only one waiting thread waiting on this condition variable (resolved by priority) */
    if (pthread_cond_signal(&semInfo->condv) != 0)
    {
        fprintf(stderr, "osSema4Give: Error broadcasting condition\n");
        retVal = OS_ERROR;
    }

    /* Unlock access to the counter */
    if (pthread_mutex_unlock(&semInfo->lock) != 0)
    {
        fprintf(stderr, "osSema4Give: Error unlocking mutex\n");
        retVal = OS_ERROR;
    }

    return retVal;
}

/*
  Function Name: osSema4Take

   This routine performs the take operation on a specified semaphore. The state of the
   semaphore and the calling task may be affected.

   osStatusT osSema4Take
       (
       SEM_ID semId,  - semaphore ID to take -
       int    timeout - timeout in ticks -
       )

   A timeout in ticks may be specified. If a task times out, semTake( ) will return ERROR.
   Timeouts of WAIT_FOREVER (-1) and NO_WAIT (0) indicate to wait indefinitely or not to wait at all.

   RETURNS
   OS_OK, or OS_ERROR if the semaphore ID is invalid or the task timed out.
*/
osStatusT osSema4Take(OS_SEM_ID semId, Sint32T timeout)
{
    semInfoS *semInfo = (semInfoS *)semId;
    int locked = 0;
    Sint32T retVal = OS_OK;

    if (semId == 0)
    {
        fprintf(stderr, "osSema4Take: Mutex NULL error\n");
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_SEMA, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
        return OS_ERROR;
    }

    /* This check should be behind the mutex (since it is in heap) but if caller still has pointer to it,
     it is safer to read these 4 bytes than to call pthread_mutex_lock */
    if (semInfo->inShutdown)
    {
        fprintf(stderr, "!!!!!!!!!!!!!!!! osSema4Take: semInfo=0x%x (%d) Take during shutdown by %s\n",
                semInfo, semInfo, taskName(taskIdSelf()));
    #if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_SEMA, (void*)semId, semInfo->inShutdown, ABORT_ON_MINOR);
    #endif
        return OS_ERROR;    /* Do not call any pthread functions if being deleted */
    }


    /* Lock access to the counter */
    if (pthread_mutex_lock(&semInfo->lock) != OS_OK)
    {
        fprintf(stderr, "osSema4Take: Error locking mutex\n");
        return OS_ERROR;
    }

    if (timeout == (Sint32T)OS_WAIT_FOREVER)
    {
        if (semInfo->count == 0)
        {
            /* Unlock the mutex if the thread gets cancelled while in pthread_cond_wait */
            RELEASE_MUTEX_IF_CANCELLED_BEGIN(&semInfo->lock);

            while ((semInfo->count == 0) && (retVal == OS_OK))
            {
                retVal = pthread_cond_wait(&semInfo->condv, &semInfo->lock);

                if (semInfo->inShutdown)
                {
                    retVal = -1; /* Return an error and unlock the mutex */
                    fprintf(stderr, "!!!!!!!!!!!!!!!! osSema4Take: Mutex %p is shutting down (WAIT_FOREVER)\n", (void *)semInfo);
                    fprintf(stderr, "!!!!!!!!!!!!!!!! osSema4Take: semInfo=0x%x (%d) Take during shutdown by %s\n",
                            semInfo, semInfo, taskName(taskIdSelf()));
                #if defined ENFORCE_STRICT_SHUTDOWN
                    handleMechanismIssues(__FILE__, __LINE__, MECH_SEMA, (void*)semId, semInfo->inShutdown, ABORT_ON_MINOR);
                #endif
                    break;
                }
            }

            RELEASE_MUTEX_IF_CANCELLED_END(&semInfo->lock);
        }

        if (retVal == OS_OK && semInfo->count > 0)
        {
            /* Semaphore is now set so take it */
            semInfo->count--;
            locked = 1;
        }
    }
    else if (timeout == (Sint32T)OS_NO_WAIT)
    {
        if (semInfo->count > 0)
        {
            /* Semaphore is now set so take it */
            locked = 1;
            semInfo->count--;
        }
        else
        {
            /* Not locked, but continue anyway */
            locked = 0;
        }
    }
    else  /* Specified timeout value */
    {
        if (semInfo->count > 0)
        {
            /* Semaphore is now set so take it */
            locked = 1;
            semInfo->count--;
        }
        else
        {
            /* Not locked, try to get it within the timeout */
            struct timespec endtime;
            struct timespec deltaTime;

            /* Unlock the mutex if the thread gets cancelled while in pthread_cond_timedwait */
            RELEASE_MUTEX_IF_CANCELLED_BEGIN(&semInfo->lock);

            /* Get absolute time now */
            (void)clock_gettime(CLOCK_MONOTONIC, &endtime);

            /* Calculate stop time as an absolute time */
            ticksToTime(&deltaTime, timeout);
            endtime.tv_sec += deltaTime.tv_sec;
            endtime.tv_nsec += deltaTime.tv_nsec;
            if (endtime.tv_nsec >= NANOSECONDS_IN_1SEC)
            {
                endtime.tv_sec++;
                endtime.tv_nsec -= NANOSECONDS_IN_1SEC;
            }

            /* repeat this operation until either the semaphore is set or the timeout occurs */
            while ((semInfo->count == 0) && (retVal == OS_OK))
            {
                retVal = pthread_cond_timedwait(&semInfo->condv, &semInfo->lock, &endtime);
                if (retVal)
                {
                    if (retVal != ETIMEDOUT)
                    {
                        fprintf(stderr, "osSema4Take: pthread_cond_timedwait failure=%d", retVal);

                        if (retVal != EINVAL)
                        {
                            ZL303XX_ERROR_TRAP("osSema4Take: pthread_cond_timedwait cannot continue");
                        }
                        #if defined ENFORCE_STRICT_SHUTDOWN
                            handleMechanismIssues(__FILE__, __LINE__, MECH_SEMA, (void*)semId, retVal, ABORT_ON_MINOR);
                        #endif
                    }
                    else
                        retVal = WAIT_TIMEOUT;
                    break;  /* timeout or invalid*/
                }

                if (semInfo->inShutdown)
                {
                    retVal = -1; /* Return an error and unlock the mutex */
                    fprintf(stderr, "!!!!!!!!!!!!!!!! osSema4Take: Mutex %p is shutting down (timeout)\n", (void *)semInfo);
                    fprintf(stderr, "!!!!!!!!!!!!!!!! osSema4Take: semInfo=0x%x (%d) Take during shutdown by %s\n",
                            semInfo, semInfo, taskName(taskIdSelf()));
                #if defined ENFORCE_STRICT_SHUTDOWN
                    handleMechanismIssues(__FILE__, __LINE__, MECH_SEMA, (void*)semId, semInfo->inShutdown, ABORT_ON_MINOR);
                #endif
                    break;
                }
            }

            RELEASE_MUTEX_IF_CANCELLED_END(&semInfo->lock);

            if (retVal == OS_OK && semInfo->count > 0)
            {
                /* Semaphore is now set so take it */
                locked = 1;
                semInfo->count--;
            }
        }
    }

    /* Unlock access to the counter */
    (void)pthread_mutex_unlock(&semInfo->lock);

    if (locked == 1)
    {
        return OS_OK;
    }
    else
    {
        return retVal;
    }
}

/*
  Function Name: osSema4Delete

   This routine terminates and deallocates any memory associated with a specified semaphore.
   Any pended tasks will unblock and return ERROR.

   osStatusT osSema4Delete
       (
       SEM_ID semId - semaphore ID to delete -
       )

  Notes:
   WARNING:
   Take care when deleting semaphores, particularly those used for mutual exclusion, to avoid
   deleting a semaphore out from under a task that already has taken (owns) that semaphore.
   Applications should adopt the protocol of only deleting semaphores that the deleting task
   has successfully taken.

   WARNING: For proper shutdown a few sequencing rules are required:
   1) Tasks with a queue to be deleted should take some external stimulous (shutdown msg)
   to cause it enter into an OS_TASK_DELAY(2000); and
   2) The task being deleted cannot be pending on a message queue or the task will not be deleted cleanly.

   RETURNS
   OS_OK, or OS_ERROR if the semaphore ID is invalid
*/
osStatusT osSema4Delete(OS_SEM_ID semId)
{
    semInfoS *semInfo = (semInfoS *)semId;
    osStatusT status = OS_OK;
    Sint8T retVal = OS_OK;

    if (semId == 0)
    {
        fprintf(stderr, "osSema4Delete: Mutex NULL error\n");
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_SEMA, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
        return OS_ERROR;
    }

    /* Caller has guaranteed to have called take before delete */

    if (pthread_mutex_lock(&semInfo->lock) != OS_OK)
    {
        fprintf(stderr, "osSema4Delete: Warning, Tried and couldn't take the lock errno=%d.\n", errno);
        return OS_ERROR;
    }

    semInfo->inShutdown = ZL303XX_TRUE;

#if defined USE_DEFERRED_FREE
    /* Defer freeing the memory until it can be assured no one will reference this memory */
    {
        deferredFreeT *newHd;

        if ((newHd = OS_CALLOC(1, sizeof(deferredFreeT))) != NULL)
        {
            newHd->nxtFree = pDeferredLstHead;
            newHd->deferredConvToFree = &semInfo->condv;
            newHd->deferredMemToFree = semInfo;
            pDeferredLstHead = newHd;
        }
    }
#endif
    /* Inform any waiters of shutdown so they can stop waiting cleanly */
    if (pthread_cond_broadcast(&semInfo->condv) != OS_OK)
    {
       fprintf(stderr, "osSema4Delete: Error broadcasting condition\n");
       retVal = OS_ERROR;
    }

    if (pthread_mutex_unlock(&semInfo->lock) == OS_OK)     /* In Linux, you have to unlock before destroy */
    {
    #if !defined ENFORCE_STRICT_SHUTDOWN
        OS_TASK_DELAY(50);  /* This will CAUSE Give or Take during shutdown errors (See the warning in osSema4Delete()) */
    #endif

        if (retVal == OS_OK && (retVal = pthread_mutex_destroy(&semInfo->lock)) == EBUSY)
        {
#ifdef ZL_DEBUG_TASKS
            fprintf(stderr, "osSema4Delete: Warning: Sema4 Mutex was locked when destroyed.\n");
#endif
            retVal = OS_ERROR;
        }
    }

#if !defined USE_DEFERRED_FREE
    if (retVal == OS_OK && (status = pthread_cond_destroy(&semInfo->condv)) == EBUSY)
    {
        fprintf(stderr, "osSema4Delete: Warning, Condition variable in use when destroyed = %d.\n", status);
        retVal = OS_ERROR;
    }

    if (retVal == ZL303XX_OK)
        OS_FREE((void*)semId);
#endif

    return retVal;
}

/*****************   Mutex *****************************************/

/*
  Function Name: osMutexCreate

  Details:
   Creates a mutex

  Parameters:
        None

  Return Value:
   ID of the mutex, or OS_MUTEX_INVALID if the mutex cannot be created

  Notes:
   The following properties are required for mutexes:
      - Tasks pending on the mutex will be released in priority order
      - The mutex protects against priority inversion due to pre-emption
         of a task holding the mutex by elevating the holding task
         to the priority of the highest pending task.
      - Multiple calls to take the mutex may be nested within the same
         calling task

   Note that MinGW pthread-win32 does not support priority inheritance mutexes

 *******************************************************************************/
OS_MUTEX_ID osMutexCreate(void)
{
    return (OS_MUTEX_ID) osSema4CreateBinary(1); /* Initially unlocked */
}

/*
  Function Name: osMutexGive

  Details:
   Performs the "give" operation on the specified mutex.

  Parameters:
       mutex    Mutex identifier

  Return Value:
   OS_OK, or OS_ERROR if the mutex cannot be unlocked

*****************************************************************************/
osStatusT osMutexGive(OS_MUTEX_ID mutex)
{
    osStatusT status = OS_OK;
    if (mutex == 0)
    {
        fprintf(stderr, "osMutexGive: Mutex NULL error\n");
  #if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MUTEX, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
  #endif
        return OS_ERROR;
    }

    status = osSema4Give((OS_SEM_ID) mutex);

    return status;
}

/*
  Function Name: osMutexTake

  Details:
   Performs the "take" operation on the specified mutex.

  Parameters:
       mutex    Mutex identifier

  Return Value:
   OS_OK, or OS_ERROR if the mutex cannot be locked

*****************************************************************************/
osStatusT osMutexTake(OS_MUTEX_ID mutex)
{
    osStatusT status = OS_OK;

    if (mutex == 0)
    {
        fprintf(stderr, "osMutexTake: Mutex NULL error\n");
  #if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MUTEX, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
  #endif
        return OS_ERROR;
    }

    status = osSema4Take((OS_SEM_ID) mutex, OS_WAIT_FOREVER);

    return status;
}

/*
  Function Name: osMutexTakeT

  Details:
   Performs the "take" operation on the specified mutex with a timeout

  Parameters:
       mutex    Mutex identifier
       timeout  Timeout in ticks or one of the following special values:
               OS_NO_WAIT (0) return immediately, even if the semaphore could not be taken
               OS_WAIT_FOREVER (-1) never time out.

  Return Value:
   OS_OK, or OS_ERROR if the mutex cannot be locked or if the task timed out

*****************************************************************************/
osStatusT osMutexTakeT(OS_MUTEX_ID mutex, Sint32T timeout)
{
    osStatusT status = OS_OK;

    if (mutex == 0)
    {
        fprintf(stderr, "osMutexTakeT: Mutex NULL error\n");
  #if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MUTEX, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
  #endif
        return OS_ERROR;
    }

    status = osSema4Take((OS_SEM_ID) mutex, timeout);

    return status;
}

/*
  Function Name: osMutexDelete

  Details:
   Deletes a mutex

  Parameters:
       mutex    Mutex identifier

  Return Value:
   OS_OK, or OS_ERROR if the mutex cannot be deleted

  Notes:
   WARNING:
   Take care when deleting mutexes, particularly those used for mutual exclusion, to avoid
   deleting a mutex out from under a task that already has taken (owns) that mutex.
   Applications should adopt the protocol of only deleting mutex that the deleting task
   has successfully taken.

*****************************************************************************/
osStatusT osMutexDelete(OS_MUTEX_ID mutex)
{

    if (mutex == 0)
    {
        fprintf(stderr, "osMutexDelete: Mutex NULL error\n");
  #if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MUTEX, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
  #endif
        return OS_ERROR;
    }

    /* Caller has guaranteed to have called take before delete */
    return osSema4Delete((OS_SEM_ID) mutex);
}


/*****************   Message Queues   *****************************************/

/**
  Function Name: osMsgQCreate

  Details:
   Creates a message queue

  Parameters:
       maxMsgs        The maximum number of messages that can be queued
       maxMsgLength   The maximum size of each message

  Return Value:
   The identifier for the message queue, or OS_MSG_Q_INVALID if the queue cannot be created

  Notes:
   The following constraints shall apply to message queue usage and implementation:
     Multiple tasks may send packets to a message queue
     Only one task may receive packets from a message queue

*******************************************************************************/
OS_MSG_Q_ID osMsgQCreate(Sint32T maxMsgs, Sint32T maxMsgLength)
{
    msgQInfoS *msgQInfo = NULL;
    pthread_condattr_t attr;

    msgQInfo = (msgQInfoS *)OS_CALLOC(1, sizeof(msgQInfoS));
    if (msgQInfo == NULL)
    {
        ZL303XX_ERROR_TRAP("osMsgQCreate: Message queue structure memory allocation error");
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MSGQ, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
        return OS_MSG_Q_INVALID;
    }

    msgQInfo->msgBufs = (char  *)OS_CALLOC((Uint32T)maxMsgs, (Uint32T)maxMsgLength);
    if (msgQInfo->msgBufs == NULL)
    {
        ZL303XX_ERROR_TRAP("osMsgQCreate: Message queue memory allocation error");
        OS_FREE(msgQInfo);
        return OS_MSG_Q_INVALID;
    }

    if (pthread_mutex_init(&msgQInfo->lock, NULL) != OS_OK)
    {
        ZL303XX_ERROR_TRAP("osMsgQCreate: Mutex create error");
        OS_FREE(msgQInfo->msgBufs);
        OS_FREE(msgQInfo);
        return OS_MSG_Q_INVALID;
    }
    if (pthread_condattr_init(&attr) != OS_OK)
    {
        ZL303XX_ERROR_TRAP("osMsgQCreate: Condition attr error");
        (void)pthread_mutex_destroy(&msgQInfo->lock);
        OS_FREE(msgQInfo->msgBufs);
        OS_FREE(msgQInfo);
        return OS_MSG_Q_INVALID;
    }
    else
    {
        if (pthread_condattr_setclock(&attr, CLOCK_MONOTONIC) != OS_OK)
        {
            ZL303XX_ERROR_TRAP("osMsgQCreate: Condition attr_setclock error");
            (void)pthread_condattr_destroy(&attr);
            (void)pthread_mutex_destroy(&msgQInfo->lock);
            OS_FREE(msgQInfo->msgBufs);
            OS_FREE(msgQInfo);
            return OS_MSG_Q_INVALID;
        }
    }

    if (pthread_cond_init(&msgQInfo->condv, &attr) != OS_OK)
    {
        ZL303XX_ERROR_TRAP("osMsgQCreate: Condition variable create error");
        (void)pthread_condattr_destroy(&attr);
        (void)pthread_mutex_destroy(&msgQInfo->lock);
        OS_FREE(msgQInfo->msgBufs);
        OS_FREE(msgQInfo);
        return OS_MSG_Q_INVALID;
    }

    (void)pthread_condattr_destroy(&attr);

    msgQInfo->inPtr = 0;
    msgQInfo->outPtr = 0;
    msgQInfo->numPendingMsgs = 0;
    msgQInfo->maxMsgSize = maxMsgLength;
    msgQInfo->numBufs = maxMsgs;
    msgQInfo->inShutdown = ZL303XX_FALSE;

    return (OS_MSG_Q_ID)msgQInfo;
}

/*
  Function Name: osMsgQSend

  Details:
   Sends the message in 'buffer' of length 'nBytes' to the message queue msgQId.

  Parameters:
       msgQId         The message queue on which to send
       buffer         The message to send
       nBytes         The length of the message
       timeout        Number of ticks to wait to send the message if there is no space
                  or one of the following special values
                     OS_NO_WAIT (0) return immediately, even if the message could not be sent
                     WAIT_FOREVER (-1) never time out.

  Return Value:
   OS_OK, or OS_ERROR if the message could not be sent to the queue

  Notes:
   The following constraints shall apply to message queue usage and implementation:
     Messages are always queued in FIFO order
     This routine can be called by interrupt service routines as well as by tasks. But
     when called from an interrupt service routine, timeout must be NO_WAIT.

*******************************************************************************/
osStatusT osMsgQSend(OS_MSG_Q_ID msgQId, Sint8T *buffer, Uint32T nBytes, Sint32T timeout)
{
    msgQInfoS *msgQInfo = (msgQInfoS *)msgQId;
    SINT_T msgSent = 0;
    SINT_T retVal = OS_OK;
    zl303xx_BooleanE bMutexLocked = ZL303XX_FALSE;

    if (msgQInfo == NULL)
    {
        fprintf(stderr, "osMsgQSend: Queue NULL error\n");
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MSGQ, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
        return OS_ERROR;
    }

    if (msgQInfo->inShutdown)
    {
        fprintf(stderr, "!!!!!!!!!!!!!!!! osMsgQSend: %p is shutdown\n", (void *)msgQInfo);
        fprintf(stderr, "!!!!!!!!!!!!!!!! osMsgQSend: Send during shutdown by %s\n", taskName(taskIdSelf()));
    #if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MSGQ, (void*)msgQId, msgQInfo->inShutdown, ABORT_ON_MINOR);
    #else
        OS_TASK_DELAY(250);  /* Not advised! */ */
    #endif
        return OS_ERROR;    /* Do not call any pthread functions if being deleted */
    }

    /* Lock access to the message queue variables */
    if (pthread_mutex_lock(&msgQInfo->lock) != OS_OK)
    {
        fprintf(stderr, "osMsgQSend: Error locking mutex\n");
        return OS_ERROR;
    }
    else
        bMutexLocked = ZL303XX_TRUE;

#ifdef EXTRA_DEBUG
   {
      if (Zl303xx_PtpClockArray[0] && msgQId == Zl303xx_PtpClockArray[0]->msgQId)   /* Monitor Clock events only */
      {
          mutexLocked++;
          if (mutexLocked > 2) /* It's normal to be behind 1 msg */
          {
              fprintf(stderr, "osMsgQSend: Lock - mutexLocked=%d inPtr=%d outPtr=%d Pend=%d!!!!\n",
                       mutexLocked, msgQInfo->inPtr, msgQInfo->outPtr, msgQInfo->numPendingMsgs);
          }
      }
   }
#endif

    /* Check for valid size */
    if (nBytes > msgQInfo->maxMsgSize)
    {
        fprintf(stderr, "osMsgQSend: Invalid message size=%d exceeded max=%d\n", nBytes, msgQInfo->maxMsgSize);
        if (bMutexLocked == ZL303XX_TRUE)
            (void)pthread_mutex_unlock(&msgQInfo->lock);
        return OS_ERROR;
    }

    if (timeout == (Sint32T)OS_WAIT_FOREVER)
    {
        if (msgQInfo->numPendingMsgs >= msgQInfo->numBufs)
        {
            /* Buffers are all full. Wait until something changes */

            /* Unlock the mutex if the thread gets cancelled while in pthread_cond_wait */
            RELEASE_MUTEX_IF_CANCELLED_BEGIN(&msgQInfo->lock);

            while ((msgQInfo->numPendingMsgs >= msgQInfo->numBufs) && (retVal == OS_OK))
            {
                retVal = pthread_cond_wait(&msgQInfo->condv, &msgQInfo->lock);

                if (msgQInfo->inShutdown)
                {
                    retVal = OS_ERROR; /* Return an error and unlock the mutex */
                    fprintf(stderr, "!!!!!!!!!!!!!!!! osMsgQSend: %p is shutting down (WAIT_FOREVER)\n", (void *)msgQInfo);
                    fprintf(stderr, "!!!!!!!!!!!!!!!! osMsgQSend: Send during shutdown by %s\n", taskName(taskIdSelf()));
                #if defined ENFORCE_STRICT_SHUTDOWN
                    handleMechanismIssues(__FILE__, __LINE__, MECH_MSGQ, (void*)msgQId, msgQInfo->inShutdown, ABORT_ON_MINOR);
                #endif
                    break;
                }
            }

            RELEASE_MUTEX_IF_CANCELLED_END(&msgQInfo->lock);
        }

        if (retVal == OS_OK && msgQInfo->numPendingMsgs < msgQInfo->numBufs)
        {
            /* There is now space so copy the message */
            copyMsgToQ(msgQInfo, buffer, nBytes);
            msgSent = 1;
        }
    }
    else if (timeout == (Sint32T)OS_NO_WAIT)
    {
        if (msgQInfo->numPendingMsgs < msgQInfo->numBufs)
        {
            /* There is space so copy the message */
            copyMsgToQ(msgQInfo, buffer, nBytes);
            msgSent = 1;
        }
        else
        {
            /* No space, but continue anyway */
            msgSent = 0;
        }
    }
    else  /* Specified timeout value */
    {
        if (msgQInfo->numPendingMsgs < msgQInfo->numBufs)
        {
            /* There is space so copy the message straightaway */
            copyMsgToQ(msgQInfo, buffer, nBytes);
            msgSent = 1;
        }
        else
        {
            /* No space, wait for the timeout to see if it becomes available */
            struct timespec endtime;
            struct timespec deltaTime;

            /* Unlock the mutex if the thread gets cancelled while in pthread_cond_timedwait */
            RELEASE_MUTEX_IF_CANCELLED_BEGIN(&msgQInfo->lock);

            /* Get absolute time now */
            (void)clock_gettime(CLOCK_MONOTONIC, &endtime);

            /* Calculate stop time as an absolute time */
            ticksToTime(&deltaTime, timeout);
            endtime.tv_sec += deltaTime.tv_sec;
            endtime.tv_nsec += deltaTime.tv_nsec;
            if (endtime.tv_nsec >= NANOSECONDS_IN_1SEC)
            {
                endtime.tv_sec++;
                endtime.tv_nsec -= NANOSECONDS_IN_1SEC;
            }

            /* repeat this operation until either there is space or the timeout occurs */
            while ((msgQInfo->numPendingMsgs >= msgQInfo->numBufs) && (retVal == OS_OK))
            {
                retVal = pthread_cond_timedwait(&msgQInfo->condv, &msgQInfo->lock, &endtime);
                if (retVal)
                {
                    if (retVal != ETIMEDOUT)
                    {
                        fprintf(stderr, "osMsgQSend: pthread_cond_timedwait failure=%d", retVal);

                        if (retVal != EINVAL)
                        {
                            ZL303XX_ERROR_TRAP("osMsgQSend: pthread_cond_timedwait cannot continue");
                        }
                    #if defined ENFORCE_STRICT_SHUTDOWN
                        handleMechanismIssues(__FILE__, __LINE__, MECH_MSGQ, (void*)msgQId, retVal, ABORT_ON_MINOR);
                    #endif
                    }
                    else
                        retVal = WAIT_TIMEOUT;
                    break;  /* timeout or invalid*/
                }

                if (msgQInfo->inShutdown)
                {
                    retVal = OS_ERROR; /* Return an error and unlock the mutex */
                    fprintf(stderr, "!!!!!!!!!!!!!!!! osMsgQSend: %p is shutting down (timeout)\n", (void *)msgQInfo);
                    fprintf(stderr, "!!!!!!!!!!!!!!!! osMsgQSend: Send during shutdown by %s\n", taskName(taskIdSelf()));
                #if defined ENFORCE_STRICT_SHUTDOWN
                    handleMechanismIssues(__FILE__, __LINE__, MECH_MSGQ, (void*)msgQId, msgQInfo->inShutdown, ABORT_ON_MINOR);
                #endif
                    break;
                }
            }

            RELEASE_MUTEX_IF_CANCELLED_END(&msgQInfo->lock);

            if (retVal == OS_OK && msgQInfo->numPendingMsgs < msgQInfo->numBufs)
            {
                /* There is now space so copy the message */
                copyMsgToQ(msgQInfo, buffer, nBytes);
                msgSent = 1;
            }
            else
            {  /* Could not send */
                msgSent = 0;
            }
        }
    }

#ifdef EXTRA_DEBUG
   if (Zl303xx_PtpClockArray[0] && msgQId == Zl303xx_PtpClockArray[0]->msgQId)
   {
       mutexLocked--;
       if (mutexLocked > 1)
       {
           fprintf(stderr, "osMsgQSend: Unlock - mutexLocked=%d inPtr=%d outPtr=%d Pend=%d!!!!\n",
                     mutexLocked, msgQInfo->inPtr, msgQInfo->outPtr, msgQInfo->numPendingMsgs);
       }
   }
#endif

   /* Inform pending senders/receivers that there is now a message */
   if (!msgQInfo->inShutdown && pthread_cond_broadcast(&msgQInfo->condv) != OS_OK)
   {
       fprintf(stderr, "osMsgQSend: Error broadcasting condition\n");
       retVal = OS_ERROR;
   }

   /* Unlock access to the queue */
   if (bMutexLocked == ZL303XX_TRUE && pthread_mutex_unlock(&msgQInfo->lock) != OS_OK)
   {
       fprintf(stderr, "osMsgQSend: Error unlocking mutex\n");
       retVal = OS_ERROR;
   }

    if (msgSent && retVal == OS_OK)
    {
        return OS_OK;
    }
    else
    {
        return retVal;
    }
}

/* Static helper function */
static void copyMsgToQ(msgQInfoS *msgQInfo, Sint8T *buffer, Uint32T nBytes)
{
    /* Copy nBytes from buffer to the message queue */
    char *msgBufToUse = msgQInfo->msgBufs + (msgQInfo->maxMsgSize * msgQInfo->inPtr);

    memcpy(msgBufToUse, buffer, nBytes);
    msgQInfo->numPendingMsgs++;
    msgQInfo->inPtr++;
    if (msgQInfo->inPtr >= msgQInfo->numBufs)
    {  /* Wrap around the end of the queue */
        if (msgQInfo->inPtr > msgQInfo->numBufs)
            fprintf(stderr, "copyMsgToQ: msgQInfo->inPtr=%d > msgQInfo->numBufs=%d ; msgQInfo->numPendingMsgs=%d. Possible OS mutex issue?\n",
                     msgQInfo->inPtr, msgQInfo->numBufs, msgQInfo->numPendingMsgs);
        msgQInfo->inPtr = 0;
    }
}

/*
  Function Name: osMsgQReceive

  Details:
   Receives a message from the specified message queue if available.

  Parameters:
       msgQId         The message queue on which to receive
       buffer         The buffer to receive the message
       maxNBytes      The length of the buffer
       timeout        Number of ticks to wait to receive a message if there is none pending
                  or one of the following special values
                     OS_NO_WAIT (0) return immediately, even if there was no message to receive
                     WAIT_FOREVER (-1) never time out.

  Return Value:
   The number of bytes copied to the buffer or OS_ERROR if no message could be received

  Notes:
   The following constraints shall apply to message queue usage and implementation:
     The received message is copied into the specified buffer, which is maxNBytes in
     length. If the message is longer than maxNBytes, the remainder of the message is discarded
      (no error indication is returned).
      This routine will not be called by interrupt service routines.

*******************************************************************************/
osStatusT osMsgQReceive(OS_MSG_Q_ID msgQId, Sint8T *buffer, Uint32T maxNBytes, Sint32T timeout)
{
    msgQInfoS *msgQInfo = (msgQInfoS *)msgQId;
    int nBytesToCopy;
    int msgRecvd = 0;
    SINT_T retVal = OS_OK;

    if (msgQInfo == NULL)
    {
        fprintf(stderr, "osMsgQReceive: Mutex NULL error\n");
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MSGQ, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
        return OS_ERROR;
    }

    /* This check should be behind the mutex (since it is in heap) but if caller still has pointer to it,
     it is safer to read these 4 bytes than to call pthread_mutex_lock */
    if (msgQInfo->inShutdown)
    {
        fprintf(stderr, "!!!!!!!!!!!!!!!! osMsgQReceive: %p is shutdown\n", (void *)msgQInfo);
        fprintf(stderr, "!!!!!!!!!!!!!!!! osMsgQReceive: Receive during shutdown by %s\n", taskName(taskIdSelf()));
    #if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MSGQ, (void*)msgQId, msgQInfo->inShutdown, ABORT_ON_MINOR);
    #else
        OS_TASK_DELAY(250);  /* Not advised! */
    #endif
        return OS_ERROR;    /* Do not call any pthread functions if being deleted */
    }

    /* Lock access to the message queue variables */
    if (pthread_mutex_lock(&msgQInfo->lock) != OS_OK)
    {
        fprintf(stderr, "osMsgQReceive: Error locking mutex\n");
        return OS_ERROR;
    }
#ifdef EXTRA_DEBUG
    else
    {
      if (Zl303xx_PtpClockArray[0] && msgQId == Zl303xx_PtpClockArray[0]->msgQId)
      {
          mutexLocked++;
          if (mutexLocked > 1)
          {
              fprintf(stderr, "osMsgQReceive: Lock - mutexLocked=%d inPtr=%d outPtr=%d Pend=%d!!!!\n",
                        mutexLocked, msgQInfo->inPtr, msgQInfo->outPtr, msgQInfo->numPendingMsgs);
          }
      }
    }
#endif

    /* Calculate size of message to copy */
    if (maxNBytes > msgQInfo->maxMsgSize)
    {
        nBytesToCopy = msgQInfo->maxMsgSize;
    }
    else
    {
        nBytesToCopy = maxNBytes;
    }

    if (timeout == (Sint32T)OS_WAIT_FOREVER)
    {
        if (msgQInfo->numPendingMsgs == 0)
        {
            /* Queue is empty. Wait until something changes */

           /* Unlock the mutex if the thread gets cancelled while in pthread_cond_wait */
           RELEASE_MUTEX_IF_CANCELLED_BEGIN(&msgQInfo->lock);

           while ((msgQInfo->numPendingMsgs == 0) && (retVal == OS_OK))
            {
                retVal = pthread_cond_wait(&msgQInfo->condv, &msgQInfo->lock);

                if (msgQInfo->inShutdown)
                {
                    retVal = OS_ERROR; /* Return an error and unlock the mutex */
                    fprintf(stderr, "!!!!!!!!!!!!!!!! osMsgQReceive: %p is shutting down (WAIT_FOREVER)\n", (void *)msgQInfo);
                    fprintf(stderr, "!!!!!!!!!!!!!!!! osMsgQReceive: Receive during shutdown by %s\n", taskName(taskIdSelf()));
                #if defined ENFORCE_STRICT_SHUTDOWN
                    handleMechanismIssues(__FILE__, __LINE__, MECH_MSGQ, (void*)msgQId, msgQInfo->inShutdown, ABORT_ON_MINOR);
                #endif
                    break;
                }
            }

            RELEASE_MUTEX_IF_CANCELLED_END(&msgQInfo->lock);
        }

        if (retVal == OS_OK && msgQInfo->numPendingMsgs > 0)
        {
            /* A message is now available so copy it */
            copyMsgFromQ(msgQInfo, buffer, (Uint32T)nBytesToCopy);
            msgRecvd = 1;
        }
    }
    else if (timeout == (Sint32T)OS_NO_WAIT)
    {
        if (msgQInfo->numPendingMsgs > 0)
        {
            /* A message is available so copy it */
            copyMsgFromQ(msgQInfo, buffer, (Uint32T)nBytesToCopy);
            msgRecvd = 1;
        }
        else
        {
            /* No message, but continue anyway */
            msgRecvd = 0;
        }
    }
    else  /* Specified timeout value */
    {
        if (msgQInfo->numPendingMsgs > 0)
        {
            /* A message is available so copy it straightaway */
            copyMsgFromQ(msgQInfo, buffer, (Uint32T)nBytesToCopy);
            msgRecvd = 1;
        }
        else
        {
            /* No message, wait for the timeout to see if one becomes available */
            struct timespec endtime;
            struct timespec deltaTime;

            /* Unlock the mutex if the thread gets cancelled while in pthread_cond_timedwait */
            RELEASE_MUTEX_IF_CANCELLED_BEGIN(&msgQInfo->lock);

            /* Get absolute time now */
            (void)clock_gettime(CLOCK_MONOTONIC, &endtime);

            /* Calculate stop time as an absolute time */
            ticksToTime(&deltaTime, timeout);
            endtime.tv_sec += deltaTime.tv_sec;
            endtime.tv_nsec += deltaTime.tv_nsec;
            if (endtime.tv_nsec >= NANOSECONDS_IN_1SEC)
            {
                endtime.tv_sec++;
                endtime.tv_nsec -= NANOSECONDS_IN_1SEC;
            }

            /* repeat this operation until either there is a message or the timeout occurs */
            while ((msgQInfo->numPendingMsgs == 0) && (retVal == OS_OK))
            {
                retVal = pthread_cond_timedwait(&msgQInfo->condv, &msgQInfo->lock, &endtime);
                if (retVal)
                {
                    if (retVal != ETIMEDOUT)
                    {
                        fprintf(stderr, "osMsgQReceive: pthread_cond_timedwait failure=%d", retVal);

                        if (retVal != EINVAL)
                        {
                            ZL303XX_ERROR_TRAP("osMsgQReceive: pthread_cond_timedwait cannot continue");
                        }
                      #if defined ENFORCE_STRICT_SHUTDOWN
                        handleMechanismIssues(__FILE__, __LINE__, MECH_MSGQ, (void*)msgQId, retVal, ABORT_ON_MINOR);
                      #endif
                    }
                    else
                        retVal = WAIT_TIMEOUT;
                    break;  /* timeout or invalid*/
                }

                if (msgQInfo->inShutdown)
                {
                    retVal = OS_ERROR; /* Return an error and unlock the mutex */
                    fprintf(stderr, "!!!!!!!!!!!!!!!! osMsgQReceive: %p is shutting down\n",(void *) msgQInfo);
                    fprintf(stderr, "!!!!!!!!!!!!!!!! osMsgQReceive: Receive during shutdown by %s\n", taskName(taskIdSelf()));
                #if defined ENFORCE_STRICT_SHUTDOWN
                    handleMechanismIssues(__FILE__, __LINE__, MECH_MSGQ, (void*)msgQId, msgQInfo->inShutdown, ABORT_ON_MINOR);
                #endif
                    break;
                }
            }

            RELEASE_MUTEX_IF_CANCELLED_END(&msgQInfo->lock);

            if (retVal == OS_OK && msgQInfo->numPendingMsgs > 0)
            {
                /* A message is available so copy it */
                copyMsgFromQ(msgQInfo, buffer, (Uint32T)nBytesToCopy);
                msgRecvd = 1;
            }
            else
            {  /* No message */
                msgRecvd = 0;
                retVal = WAIT_TIMEOUT;
            }
        }
    }

#ifdef EXTRA_DEBUG
    if (Zl303xx_PtpClockArray[0] && msgQId == Zl303xx_PtpClockArray[0]->msgQId)
    {
        mutexLocked--;
        if (mutexLocked > 1)
        {
            fprintf(stderr, "osMsgQReceive: Unlock - mutexLocked=%d inPtr=%d outPtr=%d Pend=%d!!!!\n",
                      mutexLocked, msgQInfo->inPtr, msgQInfo->outPtr, msgQInfo->numPendingMsgs);
        }
    }
#endif

    /* Inform pending senders that there is now space in the queue */
    if (!msgQInfo->inShutdown && pthread_cond_broadcast(&msgQInfo->condv) != OS_OK)
    {
        fprintf(stderr, "osMsgQReceive: Error broadcasting condition\n");
        retVal = OS_ERROR;
    }

    /* Unlock access to the queue */
    if (pthread_mutex_unlock(&msgQInfo->lock) != OS_OK)
    {
        fprintf(stderr, "osMsgQReceive: Error unlocking mutex\n");
        retVal = OS_ERROR;
    }

    if (msgRecvd && retVal == OS_OK)
    {
        return nBytesToCopy;
    }
    else
    {
        return retVal;
    }
}

/* Static helper function */
static void copyMsgFromQ(msgQInfoS *msgQInfo, Sint8T *buffer, Uint32T nBytes)
{
    /* Copy nBytes from message queue to the buffer */
    char *msgBufToUse = msgQInfo->msgBufs + (msgQInfo->maxMsgSize * msgQInfo->outPtr);

    memcpy(buffer, msgBufToUse, nBytes);
    msgQInfo->numPendingMsgs--;
    msgQInfo->outPtr++;
    if (msgQInfo->outPtr >= msgQInfo->numBufs)
    {  /* Wrap around the end of the queue */
        if (msgQInfo->outPtr > msgQInfo->numBufs)
            fprintf(stderr, "copyMsgFromQ: msgQInfo->outPtr=%d > msgQInfo->numBufs=%d ; msgQInfo->numPendingMsgs=%d. Possible OS mutex issue?\n",
                     msgQInfo->outPtr, msgQInfo->numBufs, msgQInfo->numPendingMsgs);
        msgQInfo->outPtr = 0;
    }
}

/*
  Function Name: osMsgQDelete

  Details:
   Destroys a previously created message queue

  Parameters:
       msgQId       The identifier for the semaphore

  Return Value:
   OS_OK, or OS_ERROR if the message queue cannot be destroyed

  Notes:
   Note: In this implementation the message queue will not be properly
   destroyed if a task has either the mutex locked or is waiting on the
   condition variable.

   WARNING: For proper shutdown a few sequencing rules are required:
   1) Tasks with a queue to be deleted should take some external stimulous (shutdown msg)
   to cause it enter into an OS_TASK_DELAY(2000); and
   2) The task being deleted cannot be pending on a message queue or the task will not be deleted cleanly.
*******************************************************************************/
osStatusT osMsgQDelete(OS_MSG_Q_ID msgQId)
{
    msgQInfoS *msgQInfo = (msgQInfoS *)msgQId;
    osStatusT status = OS_OK;

    if (msgQInfo == NULL)
    {
        fprintf(stderr, "osMsgQDelete: Invalid message queue ID\n");
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MSGQ, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
        return OS_ERROR;
    }
    /* This check should be behind the mutex (since it is in heap) but if caller still has pointer to it,
     it is safer to read these 4 bytes than to call pthread_mutex_lock */
    if (msgQInfo->inShutdown)
    {
        fprintf(stderr, "!!!!!!!!!!!!!!!! osMsgQDelete: %p is ALREADY in shutdown\n", (void *)msgQInfo);
        fprintf(stderr, "!!!!!!!!!!!!!!!! osMsgQDelete: Already in shutdown by %s\n", taskName(taskIdSelf()));
    #if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MSGQ, (void*)msgQId, msgQInfo->inShutdown, ABORT_ON_MINOR);
    #else
        OS_TASK_DELAY(250);  /* Not advised! */
    #endif
        return OS_ERROR;    /* Do not call any pthread functions if being deleted */
    }

    if (pthread_mutex_lock(&msgQInfo->lock) != OS_OK)
    {
        fprintf(stderr, "osMsgQDelete: Could not lock mutex\n");
        return OS_ERROR;
    }

    msgQInfo->inShutdown = ZL303XX_TRUE;

#if defined USE_DEFERRED_FREE
    /* Defer freeing the memory until it can be assured no one will reference this memory */
    {
        deferredFreeT *newHd;

        if ((newHd = OS_CALLOC(1, sizeof(deferredFreeT))) != NULL)
        {
            newHd->nxtFree = pDeferredLstHead;
            newHd->deferredConvToFree = &msgQInfo->condv;
            newHd->deferredMemToFree = msgQInfo->msgBufs;
            newHd->deferredMemToFree2 = msgQInfo;
            pDeferredLstHead = newHd;
        }
    }
#endif
    /* Wake all waiters so they can stop waiting cleanly */
    if (pthread_cond_broadcast(&msgQInfo->condv) != OS_OK)
    {
       fprintf(stderr, "osMsgQDelete: Error could not broadcast condv\n");
       status = OS_ERROR;
    }

    if (pthread_mutex_unlock(&msgQInfo->lock) == OS_OK)     /* In Linux, you have to unlock to destroy */
    {
    #if !defined ENFORCE_STRICT_SHUTDOWN
        OS_TASK_DELAY(50);  /* This will CAUSE Send or Receive during shutdown errors (See the warning in osMsgQDelete()) */
    #endif

        if ((status = pthread_mutex_destroy(&msgQInfo->lock)) == EBUSY)
        {
            /* Some task has the mutex locked. But continue anyway */
#ifdef ZL_DEBUG_TASKS
            fprintf(stderr, "osMsgQDelete: Warning: MsgQ mutex was locked when destroyed.\n");
#endif
        }
    }

#if !defined USE_DEFERRED_FREE
    if ((status = pthread_cond_destroy(&msgQInfo->condv)) == EBUSY)
    {
        fprintf(stderr, "osMsgQDelete: Warning, Condition variable in use when destroyed = %d.\n", status);
        status = OS_ERROR;
    }

    if (msgQInfo && msgQInfo->msgBufs)
    {
        OS_FREE(msgQInfo->msgBufs);
        msgQInfo->msgBufs = NULL;
    }
    if (msgQInfo)
    {
        OS_FREE(msgQInfo);
        msgQInfo = NULL;
    }
#endif

    return status;
}

/*
  Function Name: osMsgQNumMsgs

  Details:
   Returns the number of messages currently queued to a specified message queue

  Parameters:
        msgQId - message queue to examine****



  Return Value:  The number of messages queued, or ERROR.
 */

Sint32T osMsgQNumMsgs(OS_MSG_Q_ID msgQId)
{
    Sint32T numPendingMsgs;
    msgQInfoS *msgQInfo = (msgQInfoS *)msgQId;

    if (NULL == msgQInfo)
    {
        fprintf(stderr, "osMsgQNumMsgs: NULL message queue ID\n");
        return OS_ERROR;
    }

    if (pthread_mutex_lock(&msgQInfo->lock) != OS_OK)
    {
        fprintf(stderr, "osMsgQNumMsgs: Could not lock mutex\n");
        return OS_ERROR;
    }

    numPendingMsgs = msgQInfo->numPendingMsgs;

    pthread_mutex_unlock(&msgQInfo->lock);

    return numPendingMsgs;
}



/* Static helper function. Converts a number of ticks to an absolute time interval */
static void ticksToTime(struct timespec *tm, int ticks)
{
    int tickSeconds;
    int tickSubSeconds;
    int ticksPerSec = osTickRateGet();

    /* Separate required ticks into seconds and subseconds */
    tickSeconds = ticks / ticksPerSec;
    tickSubSeconds = ticks % ticksPerSec;

    tm->tv_sec = tickSeconds;
    tm->tv_nsec = tickSubSeconds * (NANOSECONDS_IN_1SEC / ticksPerSec ) +
    (tickSubSeconds * (NANOSECONDS_IN_1SEC % ticksPerSec )) / ticksPerSec;
}

/**

  Function Name: osCalloc

  Details:
Returns pointer to new allocated memory

  Parameters:
        NumberOfElements
        SizeOfElement

  Return Value: Void pointer to new memory or NULL

 *****************************************************************************/

void *osCalloc(size_t NumberOfElements, size_t size)
{
    return(calloc(NumberOfElements, size));
}


/**
  Function Name: osFree

  Details: Frees previously allocated memory (from malloc or calloc)

  Parameters:
        ptrToMem - pointer to previously allocated memory

  Return Value: None

 *****************************************************************************/

void osFree(void *ptrToMem)
{
    if (ptrToMem)
    {
        free(ptrToMem);
        ptrToMem = NULL;
    }
    else
    {
        fprintf(stderr, "Null pointer passed to osFree()\n");
    }
}


#if defined USE_DEFERRED_FREE 
/**

  Function Name: zl303xx_DeferredConVFree

  Details: Frees previously allocated pThreads condition variable and other
  memory (from malloc or calloc) used via shared services

  Parameters:
        N/A

  Return Value: status

 *****************************************************************************/
osStatusT zl303xx_DeferredConVFree(void);
osStatusT zl303xx_DeferredConVFree(void)
{
    osStatusT status = OS_OK;
    struct deferredFreeT *f, *t;
    Uint32T freed = 0;


    for (f = pDeferredLstHead; f != NULL; )
    {
        t = f;          /* Current */
        f = f->nxtFree; /* Next */
        freed++;

        /* A pthread condition variable or mutex may not be deleted if other
             threads are waiting on it. However, in this system this could be a normal
             state of affairs but we assume that since we are destroying objects then we
             must be shutting down so we note the error but continue anyway */
        if (t->deferredConvToFree && (status = pthread_cond_destroy(t->deferredConvToFree)) == EBUSY)
        {
            fprintf(stderr, "zl303xx_DeferredConVFree: Warning, Condition variable in use when destroyed = %d.\n", status);
            status = OS_ERROR;
            break;
        }

        if (t->deferredMemToFree)   /* Associated memory */
        {
            OS_FREE(t->deferredMemToFree);
            t->deferredMemToFree = NULL;
        }
        if (t->deferredMemToFree2)   /* More Associated memory */
        {
            OS_FREE(t->deferredMemToFree2);
            t->deferredMemToFree2 = NULL;
        }

        OS_FREE(t);
        t = NULL;
    }

    if (status == OS_OK)
    {
        memset(&deferredLstHead, 0, sizeof(deferredFreeT));
        pDeferredLstHead = NULL;
    }

    return status;
}
#endif

