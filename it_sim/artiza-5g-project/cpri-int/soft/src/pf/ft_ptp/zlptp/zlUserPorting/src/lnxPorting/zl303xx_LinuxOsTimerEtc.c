

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
*     This file is the Linux implementation of the abstract interface defined
*     in zl303xx_Os.h
*
******************************************************************************/

#include "zl303xx_Global.h"
#include "zl303xx_LogToMsgQ.h"

#include <asm/param.h>  /* For HZ */

#define NEED_SIGINFO

#ifdef ZL_LNX_DENX
#define NEED_POSIX_TIME
#endif

#ifdef ZL_LNX_CODESOURCERY
#define NEED_POSIX_TIME
#endif

#ifdef ZL_LNX_PICO
#define __USE_POSIX199309
#include <time.h>
#endif

#ifdef ZL_LNX_INTEL
#define NEED_POSIX_TIME
/*    #define NEED_LINUX_TIME*/
#endif

#include "zl303xx_LnxVariants.h"
#include "zl303xx_LinuxOs.h"
#include "zl303xx_Macros.h"

#if defined _ZL303XX_OS_SIGNAL_HANDLER
/* #include <bits/local_lim.h>   For SIGRTMAX */

#if defined _LINUX_NP    /* Linux, Non-portable */
#include <sys/syscall.h>
#include <unistd.h>
#endif

#if !defined _ZL303XX_OS_SIGNAL_HANDLER_USE_PIPE
#include <signal.h>
#include <pthread.h>
/* These were still missing: */
int sigtimedwait(const sigset_t *set, siginfo_t *info, const struct timespec *timeout);
int pthread_sigmask(int how, const sigset_t *newmask, sigset_t *oldmask);
#endif
#endif


/*****************    Static Global Variables   ***********************************/
static FILE *LogFile = NULL;        /* The file descriptor for logging messages */

#ifndef TEN_e9
    #define TEN_e9 1000000000
#endif

#if defined _ZL303XX_OS_SIGNAL_HANDLER

#define ZL303XX_SIG_WAIT_TIMEOUT_SEC   1
#define ZL303XX_OS_SIGNAL_HANDLERS_MAX SIGRTZLBLOCK /* see SIGRTMAX in bits/local_lim.h or bits/signum.h or ... */
#warning MSCC: ZL303XX_OS_SIGNAL_HANDLERS_MAX must match or exceed SIGRTZLBLOCK used in zl303xx_LnxVariants.h

typedef struct {
    void (*callout)(Sint32T sigNum, Sint32T pid);
} zl303xx_OsSignalHandlers_t;

static volatile zl303xx_OsSignalHandlers_t zl303xx_OsSignalHandlers[ZL303XX_OS_SIGNAL_HANDLERS_MAX +1];
static volatile zl303xx_BooleanE zl303xx_OsSignalHandlersChanged = ZL303XX_FALSE;

static OS_TASK_ID zl303xx_OsSignalTaskId = OS_TASK_INVALID;
static volatile zl303xx_BooleanE zl303xx_OsSignalTaskRunning = ZL303XX_FALSE;

static zlStatusE osSignalTask(Uint32T arg);


#if defined _ZL303XX_OS_SIGNAL_HANDLER_USE_PIPE
#define INVALID_FD -1
static volatile SINT_T osSignalHandlerPipeFds[2] = {INVALID_FD,INVALID_FD};
#endif

#endif
/*****************    Global Variables   ***********************************/
#if defined _ZL303XX_OS_SIGNAL_HANDLER && defined _LINUX_NP    /* Linux, Non-portable */
OS_TASK_ID sigThreadId = OS_TASK_INVALID;

/*

  Function Name:
   getSignalTaskId

  Details:
   Returns the signal handler thread Id.

  Parameters:
        None

  Return Value:
   OS_TASK_ID
*****************************************************************************/
OS_TASK_ID getSignalTaskId(void)
{
    return sigThreadId;
}
#endif

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
    /*  Kernel based in Linux. This call is left here for porting only */
    return (OS_OK);
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
    /*  Kernel based in Linux. This call is left here for porting only */
    return (OS_OK);
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

    #ifdef ZL_NO_DIRECT_ACCESS_TO_SYSTEM_TICKS
    /*#warning Using ZL_NO_DIRECT_ACCESS_TO_SYSTEM_TICKS*/
        return (NANOSECONDS_IN_1SEC);   /* We count the system clock in nanosecs */
    #else
        /*#warning This value is SPECIFIC TO YOUR BOARD!
        #warning Force 66MHz!*/
        printf("!!! Warning !!! This value is SPECIFIC TO YOUR BOARD - forced to 66 Mhz\n");
        return (66000000);
    #endif
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

    #ifdef ZL_NO_DIRECT_ACCESS_TO_SYSTEM_TICKS
        struct timespec tm;

        clock_gettime(CLOCK_MONOTONIC, &tm);
        return ((tm.tv_sec * NANOSECONDS_IN_1SEC) + tm.tv_nsec);
    #else
        /*#warning Insert YOUR syscall here to get system tick counter. Must be matched with osSysTimestampFreq()*/
        printf("!!! Warning !!! Insert YOUR syscall here to get system tick counter.\n\tMust be matched with osSysTimestampFreq() \n");
        return (0);
    #endif
}

/*
   This routine sets the system clock rate.

   osStatusT osTickRateSet (void)

   RETURNS
   Success of failure of the set.
*/
osStatusT osTickRateSet(Uint32T unUsedInt)
{
    unUsedInt++;   /* warning removal */
    /* This function does not exist in Linux since the BSP controls the clock rate */
    return (Uint32T)OS_INVALID;
}

/*
   This routine returns the system clock rate.

   Uint32T osTickRateGet (void)

   RETURNS
   The number of ticks per second of the system clock.
*/
Uint32T osTickRateGet(void)
{
    /* In Linux, the BSP controls the tick rate */
    /* A tick rate of 250HZ (or a multiple) is required to ensure a
       correct telecom-profile mandated inter-packet gap */
    /* The tick rate is set using parameters in the .config:
        CONFIG_HZ_250=y
        CONFIG_HZ=250 */
    /* Either fix linux/asm/param.h: HZ and return (Uint32T) HZ; or: */

/* #warning MSCC: This system tick must match your BSP. */
    return (Uint32T) 250;
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
    struct timespec tm;

    /* Get the real clock time and convert it to ticks */
    clock_gettime(CLOCK_MONOTONIC, &tm);

    return ((Uint32T)tm.tv_sec * osTickRateGet()) + ((Uint32T)tm.tv_nsec / (TEN_e9 / osTickRateGet()));
}

/*
   This routine connects a specified C routine to a specified interrupt vector. The address of routine is
   generally stored at vector so that routine is called with parameter when the interrupt occurs. The routine
   is invoked in supervisor mode at interrupt level. A proper C environment is established, the necessary registers
   saved, and the stack set up.

   The routine can be any normal C code, except that it must not invoke certain operating system functions
   that may block or perform I/O operations.

   osStatusT osInterruptConnect
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
    /* Interrupt level calls are Kernel based in Linux. This call is left here for porting only */
    if (vector || routine || parameter) /* warning removal only */
        (void)osLogMsg("osInterruptConnect: Either vector, routine or parameter is NULL",0,0,0,0,0,0);

    return OS_OK;
}

/*
   This routine disables interrupts. The intLock( ) routine returns an architecture-dependent lock-out key
   representing the interrupt level prior to the call; this key can be passed to intUnlock( ) to re-enable
   interrupts.

   Uint32T osInterruptLock (void)

   RETURNS
   An architecture-dependent lock-out key for the interrupt level prior to the call.
*/
Uint32T osInterruptLock(void)
{
    /* Interrupt level calls are Kernel based in Linux. This call is left here for porting only */
    return 1234;
}

/*
   This routine re-enables interrupts that have been disabled by intLock( ). The parameter
   lockKey is an architecture-dependent lock-out key returned by a preceding intLock( ) call.

   void osInterruptUnlock
       (
       Uint32T lockKey - lock-out key returned by preceding intLock() -
       )
*/
void osInterruptUnlock(Uint32T lockKey)
{
    /* Interrupt level calls are Kernel based in Linux. This call is left here for porting only */
    lockKey++;  /* warning removal */
}

/*

   osStatusT osInterruptEnable
       (
       Uint32T level - new interrupt bits (0x00 - 0xff00)
       )

   RETURNS
   OK or ERROR. (MIPS: The previous contents of the status register).
*/
osStatusT osInterruptEnable(Uint32T level)
{
    /* Interrupt level calls are Kernel based in Linux. This call is left here for porting only */
    level++;  /* warning removal */
    return OS_OK;
}

/*

   osStatusT osInterruptDisable
       (
       Uint32T level - new interrupt bits (0x0 - 0xff00) -
       )
*/
osStatusT osInterruptDisable(Uint32T level)
{
    /* Interrupt level calls are Kernel based in Linux. This call is left here for porting only */
    level++;  /* warning removal */
    return OS_OK;
}


/*
  Function Name:
   osLogInit

  Details:
   Initialises logging to the specified file

  Parameters:
       FILE *fd       An already open file pointer to use for logging
            Can also be set to one of 'stdout' or 'stderr' to use a predefined
            i/o stream

  Return Value:
   Nothing

 *****************************************************************************/
void osLogInit(FILE *fd)
{
    if (fd != NULL)
    {
        LogFile = fd;
    }

    zl303xx_ReEnableLogToMsgQ();
}

/*
  Function Name:
   osLogClose

  Details:
   Terminates logging to the specified file

  Parameters:
   [in]   FILE *fd   An already open file pointer used for logging. The old
                    logging file descriptor is not closed or affected by this
                    call; it is simply no longer used by the logging facilities.

  Return Value:
   Nothing

 *****************************************************************************/
void osLogClose(FILE *fd)
{
    if (LogFile == fd)
    {
       LogFile = NULL;
    }

    zl303xx_DisableLogToMsgQ();
}

/*
  Function Name:
   osLogMsg

  Details:
   Sends a message to the logging output stream

  Parameters:
       fmt      Printf style format string
       arg1     first of six required args for fmt
       arg2
       arg3
       arg4
       arg5
       arg6

  Return Value:
   The number of bytes written to the log queue, or 0 if the routine is unable to write
   a message.

  Notes:
   arg1-arg6 are required.
   Logging should be performed by a separate task in order to allow it to be
   called from interrupt context. However, this is not done in this case.

   Depending on the implementation, the interpretation of the arguments may be deferred
   to the logging task instead of at the moment when osLogMsg( ) is called. Therefore
   the arguments to logMsg( ) should not be pointers to volatile entities
   (e.g., dynamic strings on the caller stack).

*******************************************************************************/
Sint32T osLogMsg(const char *fmt, UnativeT arg1, UnativeT arg2, UnativeT arg3, UnativeT arg4, UnativeT arg5, UnativeT arg6)
{
    /* Log a formatted msg through a MsgQ to a logging task */
    return zl303xx_LogToMsgQ(LOG_FMT_STR, fmt, (UnativeT)arg1, (UnativeT)arg2, (UnativeT)arg3, (UnativeT)arg4, (UnativeT)arg5, (UnativeT)arg6);
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

#if defined _ZL303XX_OS_SIGNAL_HANDLER
#if defined _ZL303XX_OS_SIGNAL_HANDLER_USE_PIPE
/**
  Function Name:
   shutdownSignalPipes

  Details:
  Shuts down the pipe pair used for signal handling.

  Return Value: N/A
*******************************************************************************/
static void shutdownSignalPipes(void);
static void shutdownSignalPipes(void)
{
    if (osSignalHandlerPipeFds[0] != INVALID_FD)
    {
        close(osSignalHandlerPipeFds[0]);
        osSignalHandlerPipeFds[0] = INVALID_FD;
    }
    if (osSignalHandlerPipeFds[1] != INVALID_FD)
    {
        close(osSignalHandlerPipeFds[1]);
        osSignalHandlerPipeFds[1] = INVALID_FD;
    }
}
#endif

/**
  Function Name:
   osSignalHandlerWrapper

  Details:
   Handles the asynchronous signal fire event with async-safe function calls.
   Passes the signal to the osSignalTask to be handled synchronously where
   non async-safe calls can be made without restriction.

  Parameters:
     sigNum      Signal number

  Return Value: zlStatusE

*******************************************************************************/
static void osSignalHandlerWrapper(int sigNum)
{
    /* WARNING: Signal handlers have a limited set of async-safe functions,
     *          be careful in here!
     */

#if !defined _ZL303XX_OS_SIGNAL_HANDLER_USE_PIPE
    (void) sigNum; /* Warning removal */
    const char buff[] = "osSignalHandlerWrapper: ERROR Signal received on wrong task; Signal lost, check sigmask\n";
    write(2, buff, sizeof(buff));
#else

    char msg = (char) sigNum; /* One byte message safer to manage in pipe */

    if (!zl303xx_OsSignalHandlers[sigNum].callout)
    {
        const char buff[] = "osSignalHandlerWrapper: ERROR No callout registered\n";
        write(2, buff, sizeof(buff));

        return;
    }

    /* Send message to signal handler through the pipe */
    if (osSignalHandlerPipeFds[1] >= 0)
    {
        UINT_T attempts = 0, n;

    tryagain:
        if ((n = write(osSignalHandlerPipeFds[1], &msg, sizeof(msg))) < sizeof(msg))
        {
            /* Failure to write full message into pipe */
            /* Pipe is full or reader died, try again a few times */
            if (errno == EPIPE)
            {
                /* Fail, print error to stderr */
                const char buff[] = "osSignalHandlerWrapper: Error EPIPE!\n";
                write(2, buff, sizeof(buff));
            }
            else if (++attempts <= 2)
            {
                goto tryagain;
            }
            else
            {
                /* Abort, consider this a lost signal! */
                write(2, "*", 1); /* Write to stderr safely */
            }
        }
    }
    else
    {
        /* Pipe not available */
        write(2, "|", 1); /* Write to stderr safely */
    }
#endif
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
zlStatusE osSignalHandlerRegister(Uint32T sigNum, void (*callout)(Sint32T sigNum, Sint32T pid))
{
    zlStatusE status = ZL303XX_OK;
    if ((status = ZL303XX_CHECK_POINTER(callout)) != ZL303XX_OK)
    {
        ZL303XX_ERROR_TRAP("osSignalHandlerRegister: NULL callout provided" );
    }

    if (sigNum > ZL303XX_OS_SIGNAL_HANDLERS_MAX)
    {
        ZL303XX_TRACE_ALWAYS("osSignalHandlerRegister: signal %d handler, not enough space", sigNum, 0,0,0,0,0);
        status = ZL303XX_TABLE_FULL;
    }

    if (status == ZL303XX_OK)
    {
        zl303xx_OsSignalHandlers[sigNum].callout = callout;
    }

    if (status == ZL303XX_OK)
    {
        /* Notify change so that signal handler task can reconfigure itself */
        zl303xx_OsSignalHandlersChanged = ZL303XX_TRUE;
    }

    /* Bind the signal handler wrapper to this signal
     * (sigmask: used as failsafe if signals aren't blocked properly prints error message)
     * (pipe: used to redirect signal to signal handler task)
     */
    if (status == ZL303XX_OK)
    {
        struct sigaction wrapperAction;

        memset(&wrapperAction, 0, sizeof(wrapperAction));
        wrapperAction.sa_handler = &osSignalHandlerWrapper;
        wrapperAction.sa_flags = SA_RESTART;

        sigaction(sigNum, &wrapperAction, NULL);  /* Enable */

        ZL303XX_TRACE(ZL303XX_MOD_ID_SIGNALHNDLR, 2, "osSignalHandlerRegister: New sigNum=%d", sigNum, 0, 0, 0, 0, 0);
        ZL303XX_TRACE(ZL303XX_MOD_ID_SIGPIPEHNDLR, 2, "osSignalHandlerRegister: New sigNum=%d", sigNum, 0, 0, 0, 0, 0);
    }

    return status;
}

/**
  Function Name:
   osSignalHandlerUnregister

  Details:
   Un-registers the callback function for the given signal number.

  Return Value: zlStatusE

*******************************************************************************/
zlStatusE osSignalHandlerUnregister(Uint32T sigNum)
{
    zlStatusE status = ZL303XX_OK;

    if (sigNum > ZL303XX_OS_SIGNAL_HANDLERS_MAX)
    {
        ZL303XX_TRACE_ALWAYS("Error Unregistering signal %d handler, out of range", sigNum,0,0,0,0,0);
        status = ZL303XX_TABLE_FULL;
    }

    if (status == ZL303XX_OK)
    {
        zl303xx_OsSignalHandlers[sigNum].callout = NULL;

        /* Notify change so that signal handler task can reconfigure itself */
        zl303xx_OsSignalHandlersChanged = ZL303XX_TRUE;

        ZL303XX_TRACE(ZL303XX_MOD_ID_SIGNALHNDLR, 2, "osSignalHandlerUnregister: Removed sigNum=%d", sigNum, 0, 0, 0, 0, 0);
        ZL303XX_TRACE(ZL303XX_MOD_ID_SIGPIPEHNDLR, 2, "osSignalHandlerUnregister: Removed sigNum=%d", sigNum, 0, 0, 0, 0, 0);
    }

    {
        struct sigaction noAction;

        memset(&noAction, 0, sizeof(noAction));
        noAction.sa_handler = SIG_IGN;
        sigaction(sigNum, &noAction, NULL);  /* Disable */
        OS_TASK_DELAY(10);  /* Signal handling should finish */
    }

    return status;
}

/**
  Function Name:
   osSignalTask

  Details:
   Starts the synchronous OS signal handler task.

  Return Value: zlStatusE

*******************************************************************************/
zlStatusE osSignalTask(Uint32T arg)
{
    zlStatusE status = ZL303XX_OK;
    Sint32T ret = 0;

#if !defined _ZL303XX_OS_SIGNAL_HANDLER_USE_PIPE
    sigset_t signalSet;
    siginfo_t signalInfo;
    const struct timespec signalWaitTimeout = {ZL303XX_SIG_WAIT_TIMEOUT_SEC, 0}; /* 1 sec timeout */

    /* Always perform a reconfig first time around */
    zl303xx_OsSignalHandlersChanged = ZL303XX_TRUE;
#endif

    ZL303XX_TRACE_ALWAYS("osSignalTask: started", 0,0,0,0,0,0);

#if defined _ZL303XX_OS_SIGNAL_HANDLER && defined _LINUX_NP    /* Linux, Non-portable */
    sigThreadId = syscall(SYS_gettid);
#endif

    if (arg) {}; /* warning cleanup */

    zl303xx_OsSignalTaskRunning = ZL303XX_TRUE;

    /* Start signal wait loop */
    while (zl303xx_OsSignalTaskRunning)
    {

#if !defined _ZL303XX_OS_SIGNAL_HANDLER_USE_PIPE
        if (zl303xx_OsSignalHandlersChanged)
        {
            SINT_T i;

            sigemptyset(&signalSet);

            for (i = 0; i < ZL303XX_OS_SIGNAL_HANDLERS_MAX; i++)
            {
                if (zl303xx_OsSignalHandlers[i].callout != NULL)
                    sigaddset(&signalSet, i);
            }

            zl303xx_OsSignalHandlersChanged = ZL303XX_FALSE;
        }

        /* Block until signals are received (or timeout, or through task cancellation) */
        ret = sigtimedwait(&signalSet, &signalInfo, &signalWaitTimeout);
#else
        /* Block until signals are received through the pipe */
        if (osSignalHandlerPipeFds[0] >= 0)
        {
            char msg;
            UINT_T n;

            if ((n = read(osSignalHandlerPipeFds[0], &msg, sizeof(msg))) < sizeof(msg))
            {
                ret = 0;
            }
            else
            {
                ret = msg;
            }
        }
        else
        {
            /* Pipe not open yet? Be nice until opened */
            OS_TASK_DELAY(100);
        }
#endif
        OS_TASKMON_FUNC_START();


        if (ret <= 0)
        {
            /* Error, timeout or interrupted */
            if (errno == EAGAIN || errno == EINTR)
            {
                continue;
            }
#if defined _ZL303XX_OS_SIGNAL_HANDLER_USE_PIPE
            else if (errno == EPIPE || errno == EOF)
            {
                ZL303XX_TRACE(ZL303XX_MOD_ID_SIGPIPEHNDLR, 0, "READ PIPE CLOSED!", 0,0,0,0,0,0);
                zl303xx_OsSignalTaskRunning = ZL303XX_FALSE;
                shutdownSignalPipes();
                break;
            }
#endif
            else
                ZL303XX_TRACE_ALWAYS("osSignalTask: unhandled error %d", errno, 0,0,0,0,0);
        }
        else if (ret < ZL303XX_OS_SIGNAL_HANDLERS_MAX && zl303xx_OsSignalHandlers[ret].callout != NULL)
        {
            ZL303XX_TRACE(ZL303XX_MOD_ID_SIGNALHNDLR, 3, "osSignalTask: Run handler=%p for sigNum=%d",
                        zl303xx_OsSignalHandlers[ret].callout, ret, 0,0,0,0);
            ZL303XX_TRACE(ZL303XX_MOD_ID_SIGPIPEHNDLR, 3,  "osSignalTask: Run handler=%p for sigNum=%d",
                        zl303xx_OsSignalHandlers[ret].callout, ret, 0,0,0,0);

            /* Pass the signal number and the sending process id as arguments */
#if defined _ZL303XX_OS_SIGNAL_HANDLER_USE_PIPE
            zl303xx_OsSignalHandlers[ret].callout(ret, 0);  /* Pipe has no PID info available */
#else
            zl303xx_OsSignalHandlers[ret].callout(ret, signalInfo.si_pid);
#endif
        }
        else
        {
            ZL303XX_TRACE_ALWAYS("osSignalTask: unhandled signal=%d received or > %d", ret, ZL303XX_OS_SIGNAL_HANDLERS_MAX,
                               0,0,0,0);
        }

        OS_TASKMON_FUNC_END();

    }   /* while */

    if (status != ZL303XX_OK)
        ZL303XX_TRACE_ALWAYS("osSignalTask: Exit loop with failure= %d", status, 0,0,0,0,0);

    OS_TASK_DELAY(2000);    /* Wait for task to be deleted */

#if defined _ZL303XX_OS_SIGNAL_HANDLER_USE_PIPE
    shutdownSignalPipes();
#endif
    return status;
}

/**
  Function Name:
   osSignalTaskStart

  Details:
   Runs the synchronous OS signal handler. This permits non-async safe functions
   from being executed within signal handlers.

*******************************************************************************/
zlStatusE osSignalTaskStart(void)
{
    zlStatusE status = ZL303XX_OK;

    printf("osSignalTaskStart: starting signal task\n");

    if (zl303xx_OsSignalTaskId != OS_TASK_INVALID)
    {
        printf("osSignalTaskStart: task already started!\n");
        return ZL303XX_MULTIPLE_INIT_ATTEMPT;
    }

#if !defined _ZL303XX_OS_SIGNAL_HANDLER_USE_PIPE
    {
        SINT32T ret = 0;
        sigset_t signalSet;

        /* Change the signal mask of the calling task (main task) so that asynchronous signals are
           blocked and can be handled by the signal handler task "synchronously". Newly created tasks will
           inherit this signal mask and also block asynchronous signals.
         */
        ret = sigfillset(&signalSet);
        ret |= sigdelset(&signalSet, SIGABRT); /* Don't handle these signals since they generate core dumps by default */
        ret |= sigdelset(&signalSet, SIGQUIT);
        ret |= pthread_sigmask(SIG_BLOCK, &signalSet, NULL);

        if (ret)
        {
            ZL303XX_ERROR_TRAP("osSignalTaskStart: Signal handlers NOT SETUP CORRECTLY!");
        }
    }
#else
    /* Create the signal handler pipe. All threads use the write end to notify
     * the signal handler task (read end) of asynchronous signals.
     */
    if (osSignalHandlerPipeFds[0] != INVALID_FD || osSignalHandlerPipeFds[1] != INVALID_FD)
    {
        /* Pipes already open? Bad shutdown? */
        ZL303XX_TRACE(ZL303XX_MOD_ID_SIGPIPEHNDLR, 0, "osSignalTaskStart: ERROR, signal pipes still open??? %d %d", osSignalHandlerPipeFds[0], osSignalHandlerPipeFds[1], 0,0,0,0);
        return ZL303XX_ERROR;
    }
    else
    {
		SINT_T ret = -1;
		SINT_T temp[2] = {osSignalHandlerPipeFds[0],osSignalHandlerPipeFds[1]};

        if ((ret = pipe(temp)) != 0)
        {
            osSignalHandlerPipeFds[0] = temp[0];
			osSignalHandlerPipeFds[1] = temp[1];
            printf("osSignalTaskStart: Error creating pipe: %d, errno %d\n", ret, errno);
            return ZL303XX_MULTIPLE_INIT_ATTEMPT;
        }
        else
        {
            int flags;

			osSignalHandlerPipeFds[0] = temp[0];
			osSignalHandlerPipeFds[1] = temp[1];

            /* Change to blocking reader */
            flags = fcntl(osSignalHandlerPipeFds[0], F_GETFL, 0);
            if( fcntl(osSignalHandlerPipeFds[0], F_SETFL, flags & ~O_NONBLOCK) == -1 )
            {
                printf("osSignalTaskStart: fcntl(1) failed\n");
            }
            /* Change to non-blocking writer */
            flags = fcntl(osSignalHandlerPipeFds[1], F_GETFL, 0);
            if (flags > 0)
            {
                if( fcntl(osSignalHandlerPipeFds[1], F_SETFL, flags | O_NONBLOCK) == -1)
                {
                    printf("osSignalTaskStart: fcntl(2) failed\n");
                }
            }
            else
                printf("osSignalTaskStart: Error pipe failed O_NONBLOCK : %d, errno %d\n", flags, errno);
        }
    }
#endif

    zl303xx_OsSignalTaskId = OS_TASK_SPAWN(OS_SIGHNDLR_NAME, OS_SIGHNDLR_PRIORITY, 0,
                                         OS_SIGHNDLR_STACK_SZ, (OS_TASK_FUNC_PTR)&osSignalTask, 0);

    if (zl303xx_OsSignalTaskId == OS_TASK_INVALID)
    {
        zl303xx_OsSignalTaskRunning = ZL303XX_FALSE;
        status = ZL303XX_RTOS_TASK_CREATE_FAIL;
#if defined _ZL303XX_OS_SIGNAL_HANDLER_USE_PIPE
        shutdownSignalPipes();
#endif
        ZL303XX_ERROR_TRAP("osSignalTaskStart: Could not spawn signal handling task!");
    }

    return status;
}

/**
  Function Name:
   osSignalTaskStop

  Details:
   Stops the synchronous OS signal handler.

*******************************************************************************/
zlStatusE osSignalTaskStop(void)
{
    zlStatusE status = ZL303XX_OK;

    printf("osSignalTaskStop: stopping signal task\n");
    zl303xx_OsSignalTaskRunning = ZL303XX_FALSE;

    OS_TASK_DELAY(ZL303XX_SIG_WAIT_TIMEOUT_SEC * 1000 + 10);     /* ms */
#if defined _ZL303XX_OS_SIGNAL_HANDLER && defined _LINUX_NP        /* Linux, Non-portable */
    sigThreadId = OS_TASK_INVALID;
#endif


    if (zl303xx_OsSignalTaskId != OS_TASK_INVALID)
    {
        status = OS_TASK_DELETE(zl303xx_OsSignalTaskId);

        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("Could not delete signal task, failed= %d", status, 0,0,0,0,0);
        }

        zl303xx_OsSignalTaskId = OS_TASK_INVALID;

    #if defined _ZL303XX_OS_SIGNAL_HANDLER_USE_PIPE
        shutdownSignalPipes();
    #endif

   }

    return status;
}

#endif

/**
  Function Name:
   osSetup

  Details:
   OS-specific setup (e.g., start the signal handler and register the usual
   shutdown signals with the task.

*******************************************************************************/
void osSetup(void)
{
    /* OS-Specific setup features are called here */
#if defined _ZL303XX_OS_SIGNAL_HANDLER
    (void)osSignalTaskStart();
#else
    /* #warning MSCC: Linux pthreads cond_var deadlocks require _ZL303XX_OS_SIGNAL_HANDLER to be defined for proper operation! */
#endif
}

/**
  Function Name:
   osCleanup

  Details:
   OS-specific cleanup (e.g., stop the signal handler and
   delete the condition variable memory used by pthreads.

*******************************************************************************/
void osCleanup(void)
{
    /* OS-Specific cleanup features are called here */

#if defined _ZL303XX_OS_SIGNAL_HANDLER
    (void)osSignalTaskStop();   /* Delete signal handler */
#endif

#if defined USE_DEFERRED_FREE 
    zl303xx_DeferredConVFree();   /* Delete cond_var memory */
#endif

}


