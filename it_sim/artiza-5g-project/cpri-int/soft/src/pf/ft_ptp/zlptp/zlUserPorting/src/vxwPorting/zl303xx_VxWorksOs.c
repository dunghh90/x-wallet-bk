

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
*     This file is the VxWorks implementation of the abstract interface defined
*     in zl303xx_Os.h
*
******************************************************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Os.h"
#include "zl303xx_Macros.h"
#include "zl303xx_LogToMsgQ.h"

#ifdef __VXWORKS_54
    #include "zl303xx_OsHeaders.h"

    #include <stdarg.h>
    #include <stdio.h>
#endif    /* __VXWORKS_54 */
#if defined __VXWORKS_69
#include <taskLib.h>
#endif

#define WAIT_TIMEOUT                    -2

/*****************   FWD DECL   **********************************************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

#if defined MPC8260 || defined PPCEC603 || defined PPC860
  /* defined in EP8260 BSP sysLib.c */
  UINT32   vxImmrGet (void);
  #ifdef __VXWORKS_65
    #include <drv/mem/m82xxDpramLib.h>
        #if defined (MPC8313)
          /* defined in RDB8313 BSP sysLib.c */
          UINT32 sysGetPeripheralBase(void);
        #endif
  #else
    /* defined in EP8260 BSP sys82xxDpramLib.h */
    void *sys82xxDpramAlignedMalloc(size_t Length, size_t Alignment);
    void sys82xxDpramFree (void* Addr);
  #endif
#elif defined (MPC8313)
  /* defined in RDB8313 BSP sysLib.c */
  UINT32 sysGetPeripheralBase(void);
#else
  #error Target CPU not defined
#endif

/*****************   General Constants   **************************************/

const osStatusT OsStatusOk = (osStatusT)OK;
const osStatusT OsStatusError = (osStatusT)ERROR;
const Uint32T OsRandMax = (Uint32T)RAND_MAX;

const osSMiscT OsWaitForever = (osSMiscT)WAIT_FOREVER;
const osSMiscT OsTimeout = (osSMiscT)WAIT_TIMEOUT;
const osSMiscT OsNoWait = (osSMiscT)NO_WAIT;

const osMiscT OsSemInvalid = (osMiscT)NULL;

const osMiscT OsMsgQInvalid = (osMiscT)NULL;

/*  Functions  *******************/

/*
   int taskSpawn
       (
       char *  name,      - name of new task (stored at pStackBase) -
       int     priority,  - priority of new task -
       int     options,   - VX_NO_STACK_FILL (0x0100) do not fill the stack for use by checkStack( ).
       int     stackSize, - size (bytes) of stack needed plus name -
       FUNCPTR entryPt,   - entry point of new task -
       int     taskArg    - argument to pass to entryPt func
       )

   RETURNS
   The task ID, or OS_TASK_INVALID if memory is insufficient or the task cannot be created.
*/
OS_TASK_ID osTaskSpawn(
    const char *name,
    Sint32T priority,
    Sint32T options,
    Sint32T stackSize,
    OS_TASK_FUNC_PTR entryPt,
    Sint32T taskArg
)
{
    OS_TASK_ID status;
    char tName[OS_MAX_TASK_NAME];
    snprintf(tName, OS_MAX_TASK_NAME, name);

    if (priority < OS_TASK_MAX_PRIORITY || priority >= OS_TASK_MIN_PRIORITY)
    {
        fprintf(stderr, "osTaskSpawn: Error in provided priority=%d > %d or <= %d\n", priority, OS_TASK_MAX_PRIORITY, OS_TASK_MIN_PRIORITY);
        ZL303XX_ERROR_TRAP("osTaskSpawn: Error in provided priority");
        return OS_TASK_INVALID;
    }

    status = (OS_TASK_ID)taskSpawn(
                 tName,
                 priority,
                 options,
                 stackSize,
                 (FUNCPTR)entryPt,
                 taskArg,
                 0,0,0,0,0,0,0,0,0); /* VxWorks taskSpawn takes 10 task arguments. We only use one */

    if (status == (OS_TASK_ID)ERROR)
    {
        /* if the spawn failed, then best to return an invalid ID rather than error code, which
           may be different */
        status = OS_TASK_INVALID;
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_TASK, NULL, status, ABORT_ON_MAJOR);
#endif
    }
    else
    {
        OS_TASKMON_INIT(status);
    }

    return status;
}

/*
   STATUS NetTaskPriority
       (
       OS_TASK_ID requestedPriority   - priority for NetTask
       )

   RETURNS
   OK, or ERROR if the priority could not be set.
*/
osStatusT osNetTaskPriority(OS_TASK_ID requestedPriority)
{
    char netTstName[] = "tNetTask";
    OS_TASK_ID netTaskId = taskNameToId(netTstName);

    /* Make sure we started */
    if (netTaskId != (OS_TASK_ID)OS_ERROR)
    {
       return taskPrioritySet(netTaskId, requestedPriority);
    }
    else
    {
       return (osStatusT) netTaskId;
    }

}

/*
   STATUS taskDelete
       (
       int tid   - task ID of task to delete
       )

   RETURNS
   OK, or ERROR if the task cannot be deleted.
*/
osStatusT osTaskDelete (OS_TASK_ID tid)
{
    if (tid == NULL)
    {
        ZL303XX_ERROR_TRAP("osTaskSpawn: Cannot delete task");
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_TASK, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
    }

    OS_TASKMON_END(tid);
    return (osStatusT)taskDelete(tid);
}

/*
   STATUS osTaskLock (void)

   RETURNS
   OK, or ERROR if the system cannot be locked.
*/
osStatusT osTaskLock (void)
{
    return (osStatusT)taskLock();
}

/*
   STATUS osTaskUnLock (void)

   RETURNS
   OK, or ERROR if the system cannot be un-locked.
*/
osStatusT osTaskUnLock (void)
{
    return (osStatusT)taskUnlock();
}

/*
   This routine causes the calling task to relinquish the CPU for the duration specified (in ticks).
   This is commonly referred to as manual rescheduling, but it is also useful when waiting for
   some external condition that does not have an interrupt associated with it.

   STATUS taskDelay
       (
       int ticks - number of ticks to delay task -
       )

   RETURNS
   OK, or ERROR if called from interrupt level or if the calling task receives a signal that is
   not blocked or ignored.
*/
osStatusT osTaskDelay(Sint32T ticks)
{
    return (osStatusT)taskDelay(ticks);
}

/*
   This routine sets the system clock rate.

   int osTickRateSet (void)

   RETURNS
   Success of failure of the set.
*/
osStatusT osTickRateSet(Uint32T clkTicksPerSec)
{
    return (Sint32T)sysClkRateSet(clkTicksPerSec);
}

/*
   This routine returns the system clock rate.

   int sysClkRateGet (void)

   RETURNS
   The number of ticks per second of the system clock.
*/
Uint32T osTickRateGet(void)
{
    return (Uint32T)sysClkRateGet();
}

/*
   This routine returns the current value of the tick counter. This value is set to zero at
   startup, incremented by tickAnnounce( ), and can be changed using tickSet( ).

   ULONG tickGet (void)

   RETURNS
   The most recent tickSet( ) value, plus all tickAnnounce( ) calls since.
*/
Uint32T osTickGet(void)
{
    return (Uint32T)tickGet();
}


/*
   This routine connects a specified C routine to a specified interrupt vector. The address of routine is
   generally stored at vector so that routine is called with parameter when the interrupt occurs. The routine
   is invoked in supervisor mode at interrupt level. A proper C environment is established, the necessary registers
   saved, and the stack set up.

   The routine can be any normal C code, except that it must not invoke certain operating system functions
   that may block or perform I/O operations.

   STATUS intConnect
       (
       VOIDFUNCPTR * vector,   - interrupt vector to attach to -
       VOIDFUNCPTR   routine,  - routine to be called -
       int           parameter - parameter to be passed to routine -
       )

   RETURNS
   OK, or ERROR if the interrupt handler cannot be built.
*/
osStatusT osInterruptConnect(OS_VOID_FUNC_PTR *vector, OS_VOID_FUNC_PTR routine, Sint32T parameter)
{
    return (osStatusT)intConnect((VOIDFUNCPTR *)vector, (VOIDFUNCPTR)routine, parameter);
}

/*
   This routine disables interrupts. The intLock( ) routine returns an architecture-dependent lock-out key
   representing the interrupt level prior to the call; this key can be passed to intUnlock( ) to re-enable
   interrupts.

   int intLock (void)

   RETURNS
   An architecture-dependent lock-out key for the interrupt level prior to the call.
*/
Uint32T osInterruptLock(void)
{
    return (Uint32T)intLock();
}

/*
   This routine re-enables interrupts that have been disabled by intLock( ). The parameter
   lockKey is an architecture-dependent lock-out key returned by a preceding intLock( ) call.

   void intUnlock
       (
       int lockKey - lock-out key returned by preceding intLock() -
       )
*/
void osInterruptUnlock(Uint32T lockKey)
{
    (void)intUnlock(lockKey);
}

/*
   This routine enables the input interrupt bits on the present status register of the
   MIPS and PowerPC processors.

   int intEnable
       (
       int level - new interrupt bits (0x00 - 0xff00)
       )

   RETURNS
   OK or ERROR. (MIPS: The previous contents of the status register).
*/
osStatusT osInterruptEnable(Uint32T level)
{
    return (osStatusT)intEnable(level);
}

/*
   On MIPS and PowerPC architectures, this routine disables the corresponding interrupt bits from the
   present status register.

   int intDisable
       (
       int level - new interrupt bits (0x0 - 0xff00) -
       )
*/
osStatusT osInterruptDisable(Uint32T level)
{
    return (osStatusT)intDisable(level);
}


/*****************   Semaphore *****************************************/

/*
   This routine allocates and initializes a counting semaphore. The semaphore is initialized to
   the specified initial count.

   SEM_ID semCCreate
       (
       int options,     - semaphore option modes -
       int initialCount - initial count -
       )

   The options parameter specifies the queuing style for blocked tasks. Tasks may be
   queued on a priority basis or a first-in-first-out basis. These options are
   SEM_Q_PRIORITY (0x1) and SEM_Q_FIFO (0x0), respectively.

   RETURNS
   The semaphore ID, or OS_SEM_INVALID if memory cannot be allocated.
*/
OS_SEM_ID osSema4Create(Sint32T initialCount)
{
    SEM_ID createID;
    createID = semCCreate(SEM_Q_PRIORITY, initialCount);
    return (OS_SEM_ID) createID;
}

/*
   Create and initialise a binary semaphore

   SEM_ID semBCreate
       (
       Sint32T     options,         - semaphore options
       SEM_B_STATE initialState     - initial semaphore state full=1 or empty=0.

       )

   The options are SEM_Q_PRIORITY (0x1) and SEM_Q_FIFO (0x0) and specify the
   queuing style for blocked tasks. Tasks may be queued on a priority basis
   or a first-in-first-out basis.
   The initialState is SEM_FULL (0x1) or SEM_EMPTY (0x0).

   RETURNS
   Returns a semaphore ID or OS_SEM_INVALID on failure.
*/
OS_SEM_ID osSema4CreateBinary(osMiscT initialState)
{
    SEM_ID createID;
    createID = semBCreate(SEM_Q_PRIORITY, (SEM_B_STATE)initialState);
    return (OS_SEM_ID) createID;
}

/*
   This routine performs the give operation on a specified semaphore. The state of the
   semaphore and of the pending tasks may be affected.

   STATUS semGive
       (
       SEM_ID semId - semaphore ID to give -
       )

   RETURNS
   OK, or ERROR if the semaphore ID is invalid.
*/
osStatusT osSema4Give(OS_SEM_ID semId)
{
    if (semId == NULL)
    {
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_SEMA, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
    }
    return (osStatusT)semGive((SEM_ID)semId);
}

/*
   This routine performs the take operation on a specified semaphore. The state of the
   semaphore and the calling task may be affected.

   STATUS semTake
       (
       SEM_ID semId,  - semaphore ID to take -
       int    timeout - timeout in ticks -
       )

   A timeout in ticks may be specified. If a task times out, semTake( ) will return ERROR.
   Timeouts of WAIT_FOREVER (-1) and NO_WAIT (0) indicate to wait indefinitely or not to wait at all.

   RETURNS
   OK, or ERROR if the semaphore ID is invalid or the task timed out.
*/
osStatusT osSema4Take(OS_SEM_ID semId, Sint32T timeout)
{
    if (semInfo == NULL)
    {
        fprintf(stderr, "osSema4Take: Mutex NULL error\n");
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_SEMA, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
    }
    return (osStatusT)semTake((SEM_ID)semId, timeout);
}

/*
   This routine terminates and deallocates any memory associated with a specified semaphore.
   Any pended tasks will unblock and return ERROR.

   STATUS semDelete
       (
       SEM_ID semId - semaphore ID to delete -
       )

   WARNING
   Take care when deleting semaphores, particularly those used for mutual exclusion, to avoid
   deleting a semaphore out from under a task that already has taken (owns) that semaphore.
   Applications should adopt the protocol of only deleting semaphores that the deleting task
   has successfully taken.

   RETURNS
   OK, or ERROR if the semaphore ID is invalid
*/
osStatusT osSema4Delete(OS_SEM_ID semId)
{
    if (semInfo == NULL)
    {
        fprintf(stderr, "osSema4Delete: Mutex NULL error\n");
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_SEMA, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
    }
    return (osStatusT)semDelete((SEM_ID)semId);
}

/*****************   Mutex *****************************************/

/*
   This routine allocates and initializes a mutual exclusion semaphore. The semaphore state
   is initialized to full.

   SEM_ID semMCreate
       (
       int         options,     - mutex semaphore options -
       )

   Semaphore options include the following:
      SEM_Q_PRIORITY (0x1)
         Queue pended tasks on the basis of their priority
      SEM_Q_FIFO (0x0)
         Queue pended tasks on a first-in-first-out basis
      SEM_DELETE_SAFE (0x4)
         Protect a task that owns the semaphore from unexpected deletion.
      SEM_INVERSION_SAFE (0x8)
         Protect the system from priotiy inversion

   RETURNS
   The semaphore ID, or OS_SEM_INVALID if memory cannot be allocated.
*/
OS_SEM_ID osMutexCreate(void)
{
    /* No flags are supported and the supplied flags value should always be 0 */

    /* We require the following properties for mutexes:
          - Tasks pending on the mutex will be released in priority order
          - The mutex protects against priority inversion due to pre-emption
             of a task holding the mutex by elevating the holding task
             to the priority of the highest pending task.
          - Multiple calls to take the mutex may be nested within the same
             calling task */

    SEM_ID createID;
    createID = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE | SEM_DELETE_SAFE);
    return (OS_SEM_ID) createID;
}

/*
   This routine performs the give operation on a specified semaphore. The state of the
   semaphore and of the pending tasks may be affected.

   STATUS semGive
       (
       SEM_ID semId - semaphore ID to give -
       )

   RETURNS
   OK, or ERROR if the semaphore ID is invalid.
*/
osStatusT osMutexGive(OS_MUTEX_ID mutex)
{
    if (mutex == NULL)
    {
        fprintf(stderr, "osMutexGive: Mutex NULL error\n");
  #if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MUTEX, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
  #endif
    }
    return (osStatusT)semGive((SEM_ID)mutex);
}

/*
   This routine performs the take operation on a specified semaphore. The state of the
   semaphore and the calling task may be affected.

   STATUS semTake
       (
       SEM_ID semId,  - semaphore ID to take -
       int    timeout - timeout in ticks -
       )

   A timeout in ticks may be specified. If a task times out, semTake( ) will return ERROR.
   Timeouts of WAIT_FOREVER (-1) and NO_WAIT (0) indicate to wait indefinitely or not to wait at all.

   RETURNS
   OK, or ERROR if the semaphore ID is invalid or the task timed out.
*/
osStatusT osMutexTake(OS_MUTEX_ID mutex)
{
    if (mutex == NULL)
    {
        fprintf(stderr, "osMutexTake: Mutex NULL error\n");
  #if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MUTEX, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
  #endif
    }
    return (osStatusT)semTake((SEM_ID)mutex, (SINT_T)OS_WAIT_FOREVER);
}

/*
   This routine performs the take operation on a specified semaphore. The state of the
   semaphore and the calling task may be affected.

   STATUS semTake
       (
       SEM_ID semId,  - semaphore ID to take -
       int    timeout - timeout in ticks -
       )

   A timeout in ticks may be specified. If a task times out, semTake( ) will return ERROR.
   Timeouts of WAIT_FOREVER (-1) and NO_WAIT (0) indicate to wait indefinitely or not to wait at all.

   RETURNS
   OK, or ERROR if the semaphore ID is invalid or the task timed out.
*/
osStatusT osMutexTakeT(OS_MUTEX_ID mutex, Sint32T timeout)
{
    if (mutex == NULL)
    {
        fprintf(stderr, "osMutexTakeT: Mutex NULL error\n");
  #if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MUTEX, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
  #endif
    }
    return (osStatusT)semTake((SEM_ID)mutex, timeout);
}

/*
   This routine terminates and deallocates any memory associated with a specified semaphore.
   Any pended tasks will unblock and return ERROR.

   STATUS semDelete
       (
       SEM_ID semId - semaphore ID to delete -
       )

   WARNING
   Take care when deleting semaphores, particularly those used for mutual exclusion, to avoid
   deleting a semaphore out from under a task that already has taken (owns) that semaphore.
   Applications should adopt the protocol of only deleting semaphores that the deleting task
   has successfully taken.

   RETURNS
   OK, or ERROR if the semaphore ID is invalid
*/
osStatusT osMutexDelete(OS_MUTEX_ID mutex)
{
    if (mutex == NULL)
    {
        fprintf(stderr, "osMutexDelete: Mutex NULL error\n");
  #if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MUTEX, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
  #endif
    }
    return (osStatusT)semDelete((SEM_ID)mutex);
}

/*****************   Message Queues   *****************************************/

/*
   This routine creates a message queue capable of holding up to maxMsgs messages,
   each up to maxMsgLength bytes long. The routine returns a message queue ID used
   to identify the created message queue in all subsequent calls.

   MSG_Q_ID msgQCreate
       (
       int maxMsgs,      - max messages that can be queued -
       int maxMsgLength, - max bytes in a message -
       int options       - message queue options -
       )

   Options are:
   MSG_Q_FIFO (0x00)
   queue pended tasks in FIFO order.

   MSG_Q_PRIORITY (0x01)
   queue pended tasks in priority order.

   RETURNS
   MSG_Q_ID, or OS_MSG_Q_INVALID if error.
*/
OS_MSG_Q_ID osMsgQCreate(Sint32T maxMsgs, Sint32T maxMsgLength)
{
    MSG_Q_ID queueID;
    queueID = msgQCreate(maxMsgs, maxMsgLength, MSG_Q_PRIORITY);
    if (queueID == NULL)
    {
        ZL303XX_ERROR_TRAP("osMsgQCreate: Message queue structure memory allocation error");
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MSGQ, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
    }
    return (OS_MSG_Q_ID) queueID;
}

/*
   This routine sends the message in buffer of length nBytes to the message queue msgQId.
   If any tasks are already waiting to receive messages on the queue, the message will
   immediately be delivered to the first waiting task. If no task is waiting to receive
   messages, the message is saved in the message queue.

   STATUS msgQSend
       (
       MSG_Q_ID msgQId,  - message queue on which to send -
       char *   buffer,  - message to send -
       UINT     nBytes,  - length of message -
       int      timeout, - ticks to wait -
       int      priority - MSG_PRI_NORMAL or MSG_PRI_URGENT -
       )

   The timeout parameter specifies the number of ticks to wait for free space if the message
   queue is full. The timeout parameter can also have the following special values:

   NO_WAIT (0)
   return immediately, even if the message has not been sent.

   WAIT_FOREVER (-1)
   never time out.
   The priority parameter specifies the priority of the message being sent. The possible values are:

   MSG_PRI_NORMAL (0)
   normal priority; add the message to the tail of the list of queued messages.

   MSG_PRI_URGENT (1)
   urgent priority; add the message to the head of the list of queued messages.

   USE BY INTERRUPT SERVICE ROUTINES
   This routine can be called by interrupt service routines as well as by tasks. This is one
   of the primary means of communication between an interrupt service routine and a task.
   When called from an interrupt service routine, timeout must be NO_WAIT.

   RETURNS
   OK or ERROR.
*/
osStatusT osMsgQSend(OS_MSG_Q_ID msgQId, Sint8T *buffer, Uint32T nBytes, Sint32T timeout)
{
    if (msgQId == NULL)
    {
        fprintf(stderr, "osMsgQSend: Queue NULL error\n");
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MSGQ, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
    }
    return (osStatusT)msgQSend((MSG_Q_ID)msgQId, (char *)buffer, nBytes, timeout, MSG_PRI_NORMAL);
}

/*
   This routine receives a message from the message queue msgQId. The received message
   is copied into the specified buffer, which is maxNBytes in length. If the message
   is longer than maxNBytes, the remainder of the message is discarded
   (no error indication is returned).

   int msgQReceive
       (
       MSG_Q_ID msgQId,    - message queue from which to receive -
       char *   buffer,    - buffer to receive message -
       UINT     maxNBytes, - length of buffer -
       int      timeout    - ticks to wait -
       )

   The timeout parameter specifies the number of ticks to wait for a message to be sent
   to the queue, if no message is available when msgQReceive( ) is called. The timeout
   parameter can also have the following special values:

   NO_WAIT (0)
   return immediately, even if the message has not been sent.

   WAIT_FOREVER (-1)
   never time out.

   WARNING
   This routine must not be called by interrupt service routines.

   RETURNS
   The number of bytes copied to buffer, or ERROR.
*/
osStatusT osMsgQReceive(OS_MSG_Q_ID msgQId, Sint8T *buffer, Uint32T maxNBytes, Sint32T timeout)
{
    if (msgQId == NULL)
    {
        fprintf(stderr, "osMsgQReceive: Mutex NULL error\n");
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MSGQ, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
    }
    if (timeout > 0 )
    {
        Sint32T retVal = 0;

        retVal = (Sint32T)msgQReceive((MSG_Q_ID)msgQId, (char *)buffer, maxNBytes, timeout);
        if (retVal == -1)
        {
            return WAIT_TIMEOUT;
        }
        else
        {
            return retVal;
        }
    }
    else
    {
        return (Sint32T)msgQReceive((MSG_Q_ID)msgQId, (char *)buffer, maxNBytes, timeout);
    }
}

/*
   This routine deletes a message queue. Any task blocked on either a msgQSend( )
   or msgQReceive( ) will be unblocked and receive an error from the call. The msgQId
   parameter will no longer be a valid message queue ID.

   STATUS msgQDelete
       (
       MSG_Q_ID msgQId - message queue to delete -
       )

   RETURNS
   OK or ERROR.
*/
osStatusT osMsgQDelete(OS_MSG_Q_ID msgQId)
{
    if (msgQId == NULL)
    {
        fprintf(stderr, "osMsgQDelete: Invalid message queue ID\n");
#if defined ENFORCE_STRICT_SHUTDOWN
        handleMechanismIssues(__FILE__, __LINE__, MECH_MSGQ, NULL, ZL303XX_INVALID_POINTER, ABORT_ON_MAJOR);
#endif
    }
    return (osStatusT)msgQDelete((MSG_Q_ID)msgQId);
}

/*
   This routine returns the number of messages currently queued to a specified message queue.

   int msgQNumMsgs
       (
       MSG_Q_ID msgQId - message queue to examine -
       )

   RETURNS
   The number of messages queued, or ERROR.
*/
Sint32T osMsgQNumMsgs(OS_MSG_Q_ID msgQId)
{
    return (Uint32T)msgQNumMsgs((MSG_Q_ID)msgQId);
}

/*
   This routine changes the file descriptor where messages from logMsg( ) are written, allowing
   the log device to be changed from the default specified by logInit( ). It first removes the
   old file descriptor (if one had been previously set) from the log file descriptor list, then
   adds the new fd.

   void logFdSet
       (
       int fd - file descriptor to use as logging device -
       )

   The old logging file descriptor is not closed or affected by this call; it is simply no longer
   used by the logging facilities.
*/
void osLogInit(FILE *fd)
{
    /* VxWorks logging requires the low level file descriptor instead of a file pointer */
    logFdSet(fileno(fd));
}

/*
   This routine stops logging on the specified file descriptor.

   void osLogClose
       (
       int fd - file descriptor to close -
       )

*/
void osLogClose(FILE *fd)
{
    (void)logFdDelete(fileno(fd));
}

/*
   This routine logs a specified message via the logging task. This routine's syntax is
   similar to printf( ) -- a format string is followed by arguments to format. However,
   the logMsg( ) routine requires a fixed number of arguments (6).

   int logMsg
       (
       char * fmt,  - format string for print -
       int    arg1, - first of six required args for fmt -
       int    arg2,
       int    arg3,
       int    arg4,
       int    arg5,
       int    arg6
       )

   The task ID of the caller is prepended to the specified message.

   SPECIAL CONSIDERATIONS
   Because logMsg( ) does not actually perform the output directly to the logging streams,
   but instead queues the message to the logging task, logMsg( ) can be called from
   interrupt service routines.

   However, since the arguments are interpreted by the logTask( ) at the time of actual
   logging, instead of at the moment when logMsg( ) is called, arguments to logMsg( )
   should not be pointers to volatile entities (e.g., dynamic strings on the caller stack).

   RETURNS
   The number of bytes written to the log queue, or EOF if the routine is unable to write a message.
*/
Sint32T osLogMsg(const char *fmt, UnativeT arg1, UnativeT arg2, UnativeT arg3, UnativeT arg4, UnativeT arg5, UnativeT arg6)
{
    /* NOTE: if the warning level includes -Wcast-qual you will get:
     * 'warning: passing arg 1 of `logMsg' discards `const' from pointer target type'
     * since we have created an interface for multiple compilers we have set the fmt string as a const char *
     * normally we would copy the const string into a local string and use that but this is not realistic for logging.
     * There are three other options to remove this warning:
     * 1. don't use -Wcast-qual,
     * 2. change the interface from 'const char *' to 'char *' for VxWorks builds, or,
     * 3. use 'printf()' instead of calling the VxWorks function.
     * */

    /* Log a formatted msg through a MsgQ to a logging task */
    return zl303xx_LogToMsgQ(LOG_FMT_STR, fmt, (UnativeT)arg1, (UnativeT)arg2, (UnativeT)arg3, (UnativeT)arg4, (UnativeT)arg5, (UnativeT)arg6);
}

/*
   Uint32T  vxImmrGet(void)

   RETURNS
   This routine returns the current IMMR value
*/
Uint32T osImmrGet(void)
{
#if defined (MPC8260) /* EP8260 BSP defines vxImmrGet() */
    return (Uint32T)vxImmrGet();
#elif defined (MPC8313) /* RDB8313 BSP defines sysGetPeripheralBase() */
    return (Uint32T)sysGetPeripheralBase();
#else
   return 1;
#endif
}

#ifdef MPC8260 /* MPC8313 does not have a CPM */

/*
   void *sys82xxDpramAlignedMalloc
       (
       int length    - Size of DPRAM space to allocate.
       int alignment - Byte alignment.
       )

   RETURNS
   Pointer of space allocated or NULL.
*/
void * osDpramMalloc (Sint32T length, Sint32T alignment)
{
#ifdef __VXWORKS_65
    return (void *)m82xxDpramAlignedMalloc (length, alignment);
#else

    return (void *)sys82xxDpramAlignedMalloc (length, alignment);
#endif
}

/*
   void sys82xxDpramFree
      (
         void *addr  - free allocated memory from DPRAM main pool
      )

   RETURNS: None
*/

void osDpramFree(void *addr)
{
#ifdef __VXWORKS_65
    m82xxDpramFree(addr);
#else

    sys82xxDpramFree(addr);
#endif
}

#endif /* MPC8260 */

/**

  Function Name:
   osSysTimestampEnable

  Details:
   Enable the h/w timer counter used to timestamp events.

  Parameters:
        None

  Return Value:
   OS_OK, or OS_ERROR on failure

 *****************************************************************************/

osStatusT osSysTimestampEnable(void)
{
    return sysTimestampEnable();
}

/**

  Function Name:
   osSysTimestampDisable

  Details:
   Disable the h/w timer counter used to timestamp events.

  Parameters:
        None

  Return Value:
   OS_OK, or OS_ERROR on failure

 *****************************************************************************/

osStatusT osSysTimestampDisable(void)
{
    return sysTimestampDisable();
}

/**

  Function Name:
   osSysTimestampFreq

  Details:
   Returns the frequency of the timestamp counter

  Parameters:
        None

  Return Value:
   frequency of the counter

 *****************************************************************************/

Uint32T osSysTimestampFreq(void)
{
    return sysTimestampFreq();
}

/**

  Function Name:
   osSysTimestamp

  Details:
   Returns the current timestamp counter value

  Parameters:
        None

  Return Value:
   current timestamp counter value

 *****************************************************************************/

Uint32T osSysTimestamp(void)
{
    return sysTimestamp();
}




/**
  Function Name:
   osTimeGet

  Details:
   Obtains the current time (in seconds since epoch 00:00:00 January 1, 1970 GMT)

  Parameters:
     pTime       Pointer to memory which will be filled with the current OS time
                    .hi = seconds
                    .lo = nanoseconds
     pEpoch      Pointer to memory which will be filled with the current epoch
                    (May be null if not requested by caller)

  Return Value: OS_OK     Success obtaining time information
        OS_ERROR  Failure obtaining time information


*******************************************************************************/
osStatusT osTimeGet(Uint64S *pTime, Uint32T *pEpoch)
{
    struct timespec tm;

    if (ZL303XX_CHECK_POINTER(pTime) != ZL303XX_OK)
        ZL303XX_ERROR_TRAP("osTimeGet: NULL pTime" );

    if (pEpoch)
        *pEpoch = 0; /* For future use */

    if(clock_gettime(CLOCK_MONOTONIC, &tm) == 0)
    {
        pTime->lo = tm.tv_nsec;
        pTime->hi = tm.tv_sec;
        return OS_OK;
    }
    else
        return OS_ERROR;
}


/*******************************************************************************

  Function Name:  osSetup

  Details:  O/S-specific setup calls

*******************************************************************************/
void osSetup(void)
{
    /* OS-Specific setup features are called here */
}

/*******************************************************************************

  Function Name:  osCleantup

  Details:  O/S-specific cleanup calls

*******************************************************************************/
void osCleanup(void)
{
    /* OS-Specific cleanup features are called here */
}

