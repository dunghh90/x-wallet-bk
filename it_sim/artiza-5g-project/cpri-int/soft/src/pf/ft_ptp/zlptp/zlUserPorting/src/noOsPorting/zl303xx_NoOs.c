

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
*     This file is a stub implementation of the abstract interface defined
*     in zl303xx_Os.h
*
******************************************************************************/

#include "zl303xx_Os.h"
#include "zl303xx_OsHeaders.h"
#include "zl303xx_Int64.h"

#include <time.h>
#include "zl303xx_LogToMsgQ.h"

/*****************   # DEFINES   **********************************************/
#define ZL303XX_OneHzTau 0.1

/*
**  Timeout options
*/
#define NO_WAIT                         0
#define WAIT_FOREVER                    -1
#define WAIT_TIMEOUT                    -2
#define CANT_WAIT                       -3

/*****************   Global Variables   ***************************************/
/* A global file pointer to use for tracing */
FILE *LogFp = NULL;

/*****************   General Constants   **************************************/

const osStatusT OsStatusOk = (osStatusT)OK;
const osStatusT OsStatusError = (osStatusT)ERROR;
const Uint32T OsRandMax = (Uint32T)RAND_MAX;

const osSMiscT OsWaitForever = (osSMiscT)WAIT_FOREVER;
const osSMiscT OsTimeout = (osSMiscT)WAIT_TIMEOUT;
const osSMiscT OsNoWait = (osSMiscT)NO_WAIT;
const osSMiscT OsCantWait = (osSMiscT)CANT_WAIT;

const osMiscT OsSemInvalid = (osMiscT)NULL;

const osMiscT OsMsgQInvalid = (osMiscT)NULL;


/*****************   Functions  *******************/


/*
   OS_TASK_ID taskSpawn
       (
       char *  name,      - name of new task (stored at pStackBase) -
       Sint32T     priority,  - priority of new task -
       Sint32T     options,   - VX_NO_STACK_FILL (0x0100) do not fill the stack for use by checkStack( ).
       Sint32T     stackSize, - size (bytes) of stack needed plus name -
       OS_TASK_FUNC_PTR entryPt,   - entry point of new task -
       Sint32T     taskArg    - argument to pass to the task function
       )

   RETURNS
   The task ID, or ERROR if memory is insufficient or the task cannot be created.
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
   (void)name;
   (void)priority;
   (void)options;
   (void)stackSize;
   (void)entryPt;
   (void)taskArg;
   return (Sint32T)1;
}

/*
   osStatusT taskDelete
       (
       int tid   - task ID of task to delete
       )

   RETURNS
   OK, or ERROR if the task cannot be deleted.
*/
osStatusT osTaskDelete (OS_TASK_ID tid)
{
   (void)tid;
   return OS_OK;
}

/*
   This routine causes the calling task to relinquish the CPU for the duration specified (in ticks).
   This is commonly referred to as manual rescheduling, but it is also useful when waiting for
   some external condition that does not have an interrupt associated with it.

   osStatusT taskDelay
       (
       Sint32T ticks - number of ticks to delay task -
       )

   RETURNS
   OK, or ERROR if called from interrupt level or if the calling task receives a signal that is
   not blocked or ignored.
*/
osStatusT  osTaskDelay(Sint32T ticks)
{  
   (void)ticks;
   return OS_OK;
}

/*
   This routine sets the system clock rate.

   osStatusT sysClkRateSet (clkTicksPerSec)

   RETURNS
   Success or failure.
*/
osStatusT osTickRateSet(Uint32T clkTicksPerSec)
{
   (void)clkTicksPerSec;
   return OS_OK;
}

/*
   This routine returns the system clock rate.

   Uint32T sysClkRateGet (void)

   RETURNS
   The number of ticks per second of the system clock.
*/

Uint32T osTickRateGet(void)
{
    return 100;
}

/*
   This routine returns the current value of the tick counter. This value is set to zero at
   startup, incremented by tickAnnounce( ), and can be changed using tickSet( ).

   ULONG osTickGet (void)

   RETURNS
   The most recent tickSet( ) value, plus all tickAnnounce( ) calls since.
*/
Uint32T osTickGet(void)
{
    static Uint32T tickCnt = 0;

    tickCnt += osTickRateGet()* ZL303XX_OneHzTau;
    return (tickCnt);
}


/*
   This routine connects a specified C routine to a specified interrupt vector. The address of routine is
   generally stored at vector so that routine is called with parameter when the interrupt occurs. The routine
   is invoked in supervisor mode at interrupt level. A proper C environment is established, the necessary registers
   saved, and the stack set up.

   The routine can be any normal C code, except that it must not invoke certain operating system functions
   that may block or perform I/O operations.

   osStatusT intConnect
       (
       OS_VOID_FUNC_PTR * vector,   - interrupt vector to attach to -
       OS_VOID_FUNC_PTR   routine,  - routine to be called -
       Sint32T           parameter - parameter to be passed to routine -
       )

   RETURNS
   OK, or ERROR if the interrupt handler cannot be built.
*/
osStatusT osInterruptConnect(OS_VOID_FUNC_PTR *vector, OS_VOID_FUNC_PTR routine, Sint32T parameter)
{
   (void)vector;
   (void)routine;
   (void)parameter;
   return OS_OK;
}

/*
   This routine disables interrupts. The intLock( ) routine returns an architecture-dependent lock-out key
   representing the interrupt level prior to the call; this key can be passed to intUnlock( ) to re-enable
   interrupts.

   Uint32T intLock (void)

   RETURNS
   An architecture-dependent lock-out key for the interrupt level prior to the call.
*/
Uint32T osInterruptLock(void)
{
   return 1234;
}

/*
   This routine re-enables interrupts that have been disabled by intLock( ). The parameter
   lockKey is an architecture-dependent lock-out key returned by a preceding intLock( ) call.

   void intUnlock
       (
       Uint32T lockKey - lock-out key returned by preceding intLock() -
       )
*/
void osInterruptUnlock(Uint32T lockKey)
{
    (void)lockKey;
}

/*
   This routine enables the input interrupt bits on the present status register of the
   MIPS and PowerPC processors.

   osStatusT intEnable
       (
       Uint32T level - new interrupt bits (0x00 - 0xff00)
       )

   RETURNS
   OK or ERROR. (MIPS: The previous contents of the status register).
*/
osStatusT osInterruptEnable(Uint32T level)
{
   (void)level;
   return OS_OK;
}

/*
   On MIPS and PowerPC architectures, this routine disables the corresponding interrupt bits from the
   present status register.

   osStatusT intDisable
       (
       Uint32T level - new interrupt bits (0x0 - 0xff00) -
       )
*/
osStatusT osInterruptDisable(Uint32T level)
{
   (void)level;
   return OS_OK;
}

/*****************   Semaphore *****************************************/

/*
   This routine allocates and initializes a counting semaphore. The semaphore is initialized to
   the specified initial count.

   OS_SEM_ID semCCreate
       (
       Sint32T initialCount - initial count -
       )

   The options parameter specifies the queuing style for blocked tasks. Tasks may be
   queued on a priority basis or a first-in-first-out basis. These options are
   SEM_Q_PRIORITY (0x1) and SEM_Q_FIFO (0x0), respectively.

   RETURNS
   The semaphore ID, or NULL if memory cannot be allocated.
*/
OS_SEM_ID osSema4Create(Sint32T initialCount)
{
   (void)initialCount;
   return (OS_SEM_ID)12345;
}

/*
   Create and initialise a binary semaphore

   OS_SEM_ID semBCreate
       (
       osMiscT initialState     - initial semaphore state full=1 or empty=0.

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
   (void)initialState;
   return (OS_SEM_ID)23456;
}

/*
   This routine performs the give operation on a specified semaphore. The state of the
   semaphore and of the pending tasks may be affected.

   osStatusT osSema4Give
       (
       OS_SEM_ID semId - semaphore ID to give -
       )

   RETURNS
   OK, or ERROR if the semaphore ID is invalid.
*/
osStatusT osSema4Give(OS_SEM_ID semId)
{
   (void)semId;
   return OS_OK;
}

/*
   This routine performs the take operation on a specified semaphore. The state of the
   semaphore and the calling task may be affected.

   osStatusT semTake
       (
       OS_SEM_ID semId,  - semaphore ID to take -
       Sint32T    timeout - timeout in ticks -
       )

   A timeout in ticks may be specified. If a task times out, semTake( ) will return ERROR.
   Timeouts of WAIT_FOREVER (-1) and NO_WAIT (0) indicate to wait indefinitely or not to wait at all.

   RETURNS
   OK, or ERROR if the semaphore ID is invalid or the task timed out.
*/
osStatusT osSema4Take(OS_SEM_ID semId, Sint32T timeout)
{
   (void)semId;
   (void)timeout;
   return OS_OK;
}

/*
   This routine terminates and deallocates any memory associated with a specified semaphore.
   Any pended tasks will unblock and return ERROR.

   osStatusT semDelete
       (
       OS_SEM_ID semId - semaphore ID to delete -
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
   (void)semId;
   return OS_OK;
}

/*****************   Mutex *****************************************/

/*
   This routine allocates and initializes a binary mutex.

   OS_SEM_ID osMutexCreate(void)

   RETURNS
   The semaphore ID, or NULL if memory cannot be allocated.
*/
OS_SEM_ID osMutexCreate(void)
{
   return (OS_SEM_ID)123456;
}

/*
   This routine performs the give operation on a specified mutex. The state of the
   mutex and of the pending tasks may be affected.

   osStatusT osMutexGive
       (
       OS_MUTEX_ID mutex - mutex ID to give -
       )

   RETURNS
   OK, or ERROR if the semaphore ID is invalid.
*/
osStatusT osMutexGive(OS_MUTEX_ID mutex)
{
   (void)mutex;
   return OS_OK;
}

/*
   This routine performs the take operation on a specified mutex. The state of the
   semaphore and the calling task may be affected.

   osStatusT osMutexTake
       (
       OS_MUTEX_ID mutex,  - mutex ID to take -
       )

   RETURNS
   OK, or ERROR if the semaphore ID is invalid or the task timed out.
*/
osStatusT osMutexTake(OS_MUTEX_ID mutex)
{
   (void)mutex;
   return OS_OK;
}

/*
   This routine performs the take operation on a specified mutex. The state of the
   mutex and the calling task may be affected.

   osStatusT osMutexTakeT
       (
       OS_MUTEX_ID mutex,  - mutex ID to take -
       Sint32T    timeout - timeout in ticks -
       )

   A timeout in ticks may be specified. If a task times out, osMutexTakeT( ) will return ERROR.
   Timeouts of WAIT_FOREVER (-1) and NO_WAIT (0) indicate to wait indefinitely or not to wait at all.

   RETURNS
   OK, or ERROR if the semaphore ID is invalid or the task timed out.
*/
osStatusT osMutexTakeT(OS_MUTEX_ID mutex, Sint32T timeout)
{
   (void)mutex;
   (void)timeout;
   return OS_OK;
}

/*
   This routine terminates and deallocates any memory associated with a specified mutex.
   Any pended tasks will unblock and return ERROR.

   osStatusT osMutexDelete
       (
       OS_MUTEX_ID mutex - mutex ID to delete -
       )

   WARNING
   Take care when deleting mutex, particularly those used for mutual exclusion, to avoid
   deleting a mutex out from under a task that already has taken (owns) that mutex.
   Applications should adopt the protocol of only deleting mutex that the deleting task
   has successfully taken.

   RETURNS
   OK, or ERROR if the semaphore ID is invalid
*/
osStatusT osMutexDelete(OS_MUTEX_ID mutex)
{
   (void)mutex;
   return OS_OK;
}

/*****************   Message Queues   *****************************************/

/*
   This routine creates a message queue capable of holding up to maxMsgs messages,
   each up to maxMsgLength bytes long. The routine returns a message queue ID used
   to identify the created message queue in all subsequent calls.

   MSG_Q_ID msgQCreate
       (
       Sint32T maxMsgs,      - max messages that can be queued -
       Sint32T maxMsgLength, - max bytes in a message -
       )

   RETURNS
   MSG_Q_ID, or NULL if error.
*/
OS_MSG_Q_ID osMsgQCreate(Sint32T maxMsgs, Sint32T maxMsgLength)
{
   (void)maxMsgs;
   (void)maxMsgLength;
   return (OS_MSG_Q_ID)1234567;
}

/*
   This routine sends the message in buffer of length nBytes to the message queue msgQId.
   If any tasks are already waiting to receive messages on the queue, the message will
   immediately be delivered to the first waiting task. If no task is waiting to receive
   messages, the message is saved in the message queue.

   osStatusT osMsgQSend
       (
       OS_MSG_Q_ID msgQId,  - message queue on which to send -
       Sint8T *   buffer,  - message to send -
       Uint32T     nBytes,  - length of message -
       Sint32T      timeout, - ticks to wait -
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
   (void)msgQId;
   (void)buffer;
   (void)nBytes;
   (void)timeout;
   return OS_OK;
}

/*
   This routine receives a message from the message queue msgQId. The received message
   is copied into the specified buffer, which is maxNBytes in length. If the message
   is longer than maxNBytes, the remainder of the message is discarded
   (no error indication is returned).

   osStatusT osMsgQReceive
       (
       OS_MSG_Q_ID msgQId,    - message queue from which to receive -
       Sint8T *   buffer,    - buffer to receive message -
       Uint32T     maxNBytes, - length of buffer -
       Sint32T      timeout    - ticks to wait -
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
   (void)msgQId;
   (void)buffer;
   (void)maxNBytes;
   (void)timeout;
   return (Uint32T)0;
}

/*
   This routine deletes a message queue. Any task blocked on either a msgQSend( )
   or msgQReceive( ) will be unblocked and receive an error from the call. The msgQId
   parameter will no longer be a valid message queue ID.

   osStatusT msgQDelete
       (
       OS_MSG_Q_ID msgQId - message queue to delete -
       )

   RETURNS
   OK or ERROR.
*/
osStatusT osMsgQDelete(OS_MSG_Q_ID msgQId)
{
   (void)msgQId;
   return OS_OK;
}

/*
   This routine returns the number of messages currently queued to a specified message queue.

   Sint32T msgQNumMsgs
       (
       OS_MSG_Q_ID msgQId - message queue to examine -
       )

   RETURNS
   The number of messages queued, or ERROR.
*/
Sint32T osMsgQNumMsgs(OS_MSG_Q_ID msgQId)
{
   (void)msgQId;
   return 0;
}

/*
   This routine changes the file descriptor where messages from logMsg( ) are written, allowing
   the log device to be changed from the default specified by logInit( ). It first removes the
   old file descriptor (if one had been previously set) from the log file descriptor list, then
   adds the new fd.

   void logFdSet
       (
       FILE * fd - ptr to file descriptor to use as logging device -
       )

   The old logging file descriptor is not closed or affected by this call; it is simply no longer
   used by the logging facilities.
*/
void osLogInit(FILE *fd)
{
   if (LogFp != NULL)
   {
       LogFp = fd;
   }

   zl303xx_ReEnableLogToMsgQ();
}

/*
   This routine stops logging on the specified file descriptor.

   void osLogClose
       (
       FILE * fd - ptr to file descriptor to close -
       )

*/
void osLogClose(FILE *fd)
{
   if (LogFp == fd)
   {
      LogFp = NULL;
   }

   zl303xx_DisableLogToMsgQ();
}

/*
   This routine logs a specified message via the logging task. This routine's syntax is
   similar to printf( ) -- a format string is followed by arguments to format. However,
   the logMsg( ) routine requires a fixed number of arguments (6).

   Sint32T logMsg
       (
       const char * fmt,  - format string for print -
       UnativeT    arg1, - first of six required args for fmt -
       UnativeT    arg2,
       UnativeT    arg3,
       UnativeT    arg4,
       UnativeT    arg5,
       UnativeT    arg6
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
    /* Log a formatted msg through a MsgQ to a logging task */
    return zl303xx_LogToMsgQ(LOG_FMT_STR, fmt, (UnativeT)arg1, (UnativeT)arg2, (UnativeT)arg3, (UnativeT)arg4, (UnativeT)arg5, (UnativeT)arg6);
}


/*

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
   return OS_OK;
}

/*

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
   return OS_OK;
}

/*

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
   return 66000000;
}

/*

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
   return 1;
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
    time_t t1;

    if (ZL303XX_CHECK_POINTER(pTime) != ZL303XX_OK)
        ZL303XX_ERROR_TRAP("osTimeGet: NULL pTime" );

    if (pEpoch)
        *pEpoch = 0; /* For future use */

    t1 = time(NULL);

    pTime->lo = t1&0xFFFFFFFF;
    pTime->hi = ((Uint64T)t1>>32);
     
    return OS_OK;
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
    /* OS-Specific features/functions are called here */
    (void)NumberOfElements;
    (void)size;

    return NULL;
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
    /* OS-Specific features/functions are called here */
    (void)ptrToMem;
}

/**
  Function Name: taskIdSelf

  Details: Return a uniques taskID for the thread/process

  Return Value: Uint32T

 *****************************************************************************/
Uint32T taskIdSelf(void)
{
    /* OS-Specific features/functions are called here */
    return 1;
}

/**
  Function Name:
   osSignalHandlerRegister

  Details:
   Registers a callback function to be executed when given signal number is received by the
   synchronous OS signal handler task osSignalTask().

  Parameters:
     sigNum      Signal number to handle
     callout     Function pointer to execute on signal event (function gets passed the signal number
                    and the process id of the task that fired it)

  Return Value: zlStatusE

*******************************************************************************/
zlStatusE osSignalHandlerRegister(Uint32T sigNum, void (*callout)(Sint32T sigNum, Sint32T pid));
zlStatusE osSignalHandlerRegister(Uint32T sigNum, void (*callout)(Sint32T sigNum, Sint32T pid))
{
    (void)sigNum;
    (void)callout;
        /* See zl303xx_LinuxOsTimersEtc.c */
    return ZL303XX_OK;
}

/**
  Function Name:
   osSignalHandlerUnregister

  Details:
   Un-registers the callback function for the given signal number.

  Return Value: zlStatusE

*******************************************************************************/
zlStatusE osSignalHandlerUnregister(Uint32T sigNum);
zlStatusE osSignalHandlerUnregister(Uint32T sigNum)
{
    /* See zl303xx_LinuxOsTimersEtc.c */
    (void)sigNum;
    return ZL303XX_OK;
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

