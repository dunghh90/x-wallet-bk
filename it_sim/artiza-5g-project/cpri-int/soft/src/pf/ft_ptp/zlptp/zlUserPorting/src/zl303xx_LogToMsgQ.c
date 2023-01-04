

/******************************************************************************
 *

   $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa

   Copyright 2006-2018 Microsemi Semiconductor Limited.
   All rights reserved.
   The timing algorithms implemented in the software code are Patent Pending.

*******************************************************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Int64.h"
#include "zl303xx_LogToMsgQ.h"
#include "zl303xx_Macros.h"

#include <string.h>

#include "BPF_COM_PLOG.h"


/************************ DEFINES  ************************************/
#define OUTPUT_STRM stdout  /* Output stream = STDOUT or could be a file (see fdToLogInto) */

/*#define LOG2MSGQ_STATS 1*/    /* Show log2msgQ stats */
/*#define LOG2MSGQ_STATS_PRINT_INTERVAL 500*/           /* Print stats every X number of calls */
/************************ STATIC VARS  ************************************/

#if !defined ZL_MSGQ_FMT_IN_MSG
static Uint8T fmtArray[FMT_ARRAY_SIZE][MAX_FMT_LEN] = {{0},{0}};
static Uint64T busyFmtBits = 0;
static Uint8T lastFmtIndex = 0;
#endif

static FILE *fdToLogInto = NULL;

static OS_MSG_Q_ID zl303xx_LogToMsgQId     = OS_MSG_Q_INVALID;
static OS_MSG_Q_ID zl303xx_ActiveMsgQId    = OS_MSG_Q_INVALID;

#if ! defined ZL_BYPASS_MSGQ
static OS_TASK_ID  zl303xx_LogToMsgQTaskId = OS_TASK_INVALID;
#endif

#if !defined LOG2MSGQ_STATS
#define LOG2MSGQ_STATS_COUNT(x)         /* Do nothing */
#define LOG2MSGQ_STATS_PRINT(x)         /* Do nothing */
#else
/* NOTE: Stats are not protected from concurrent access */
#define LOG2MSGQ_STATS_COUNT(x) ++log2msgQstats.x   /* Increment stat count */
#define LOG2MSGQ_STATS_PRINT(x) do { \
        if (log2msgQstats.calls % LOG2MSGQ_STATS_PRINT_INTERVAL == 0) \
            zl303xx_LogToMsgQPrintStats(x); \
    } while (0)
typedef struct {
    Uint32T calls;              /* Number of zl303xx_LogToMsgQ calls */
    Uint32T sent;               /* Number of logs sent to Q successfuly */
    Uint32T printed;            /* Number of logs received and printed by logging task */
    Uint32T error_nostr;        /* Number of logs missing format string */
    Uint32T error_toobig;       /* Number of logs with format string too big */
    Uint32T error_busy;         /* Number of logs failed to print (format buffer busy) */
    Uint32T error_notactive;    /* Number of logs failed to print (Q not active) */
    Uint32T error_full;         /* Number of logs failed to print (Q full) */
} log2msgQstats_t;
static log2msgQstats_t log2msgQstats;
#endif

/*********************************************************************/

#if defined ZL303XX_LOG_HISTORY_ENABLED
#include "zl303xx_LogBuffer.h"

#include <stdarg.h> /* For va_arg etc */

static zl303xx_LogBufferT zl303xx_LogHistoryBuffer = ZL303XX_LOG_BUFFER_INIT;

/* Wrapper for fprintf */
static Sint32T zl303xx_LogHistoryFprintf(FILE* strm, const char *format, ...);

/* Prints log buffer to given filename (appends) and returns number of bytes printed */
Sint32T zl303xx_LogHistoryDumpToFile(const char* filename, Uint32T len, zl303xx_BooleanE bErase)
{
    FILE *fp = fopen(filename, "a");
    if (fp != NULL)
    {
        Sint32T ret= zl303xx_LogHistoryDump(fp, len, bErase);
        fclose(fp);
        return ret;
    }
    else
    {
        return 0;   
    }
}

/* Prints log buffer to given stream/file and returns number of bytes printed */
Sint32T zl303xx_LogHistoryDump(FILE* strm, Uint32T len, zl303xx_BooleanE bErase)
{
    return zl303xx_LogBufferDump(zl303xx_LogHistoryBuffer, strm, len, bErase);
}

/* Erases contents of log buffer */
zlStatusE zl303xx_LogHistoryErase(Uint32T len)
{
    return zl303xx_LogBufferErase(zl303xx_LogHistoryBuffer, len);
}

/* Resizes the log buffer */
zlStatusE zl303xx_LogHistoryResize(Uint32T size)
{
    return zl303xx_LogBufferResize(zl303xx_LogHistoryBuffer, size);
}

/* Gets info from log buffer */
zlStatusE zl303xx_LogHistoryGetInfo(Uint32T *size, Uint32T *count)
{
    return zl303xx_LogBufferGetInfo(zl303xx_LogHistoryBuffer, size, count);
}

/* fprintf wrapper to print into the history buffer and/or to given `strm` */
Sint32T zl303xx_LogHistoryFprintf(FILE* strm, const char *format, ...)
{
    Sint32T ret;
    char buf[MAX_FMT_LEN];
    
    va_list args;
    va_start(args, format);

    /* Safely print to a local stack buffer */
    ret = vsnprintf(buf, sizeof(buf), format, args);
    buf[sizeof(buf)-1] = '\0';
    
    va_end(args);
    
    if (ret < 0)
    {
        /* Error occurred */
        return ret;
    }
    else if ((Uint32T)ret >= sizeof(buf))
    {
        ret = sizeof(buf)-1; /* Don't count nul char */
    }

    /* Copy the log string into the circular buffer */
    (void) zl303xx_LogBufferWrite(zl303xx_LogHistoryBuffer, buf, ret);

    /* Copy log string into other stream (if provided) */
    if (strm != NULL)
    {
        ret = fputs(buf, strm);
    }
    
    return ret;
}

/* Redefine libc fprintf for rest of this file to use the wrapper (NOTE: must be redefined after zl303xx_LogHistoryFprintf to avoid recursion) */
#define fprintf zl303xx_LogHistoryFprintf
#endif

/************************ FWD DECL  ************************************/

#if defined LOG2MSGQ_STATS
void zl303xx_LogToMsgQPrintStats(FILE *filePtr);
void zl303xx_LogToMsgQPrintStats(FILE *filePtr)
{
    if (filePtr == NULL)
    {
        fprintf(stderr, "zl303xx_LogToMsgQPrintStats: NULL argument\n");
        return;
    }

    fprintf(filePtr, "******************* LOG2MSGQ STATS ********************\n");
    #define PRINT_STAT(x) fprintf(filePtr, "* %s : %u\n", #x, log2msgQstats.x)
    PRINT_STAT(calls);
    PRINT_STAT(sent);
    PRINT_STAT(printed);
    PRINT_STAT(error_nostr);
    PRINT_STAT(error_toobig);
    PRINT_STAT(error_busy);
    PRINT_STAT(error_notactive);
    PRINT_STAT(error_full);
    #undef PRINT_STAT
    fprintf(filePtr, "******************* LOG2MSGQ STATS ********************\n");
}
#endif

/** zl303xxsprintSignedLong

   This routine sprints a 64-bit number (n) into the given character
   array (a); the start of the generated number is at position idx+1.

****************************************************************************/
#define ZL303XX_STRNG_SIZE (50)
void zl303xxsprintSignedLong(Uint64S n, char *a, Sint32T *idx);
void zl303xxsprintSignedLong(Uint64S n, char *a, Sint32T *idx)
{
   Sint64T T64_n;
   char c;
   Sint32T sign;


   ZL303XX_CONVERT_TO_64_SIGNED(T64_n, n);

   sign = 1;
   if( T64_n < 0 )
   {
      sign = -1;
      T64_n = -T64_n;
   }

   a[ZL303XX_STRNG_SIZE - 2] = '\0';
   *idx = ZL303XX_STRNG_SIZE - 3;

   do {
      c = (char)(T64_n % 10);
      a[*idx] = (char)(48 + c);
      (*idx)--;

      T64_n = T64_n / 10;
   } while( T64_n > 0 );

   if( sign == -1 )
   {
      a[*idx] = '-';
      (*idx)--;
   }
}

/** zl303xxbLogsWith64BitNumbers

   This routine returns TRUE if the given enum is a log that includes a
   64-bit number.

****************************************************************************/
zl303xx_BooleanE zl303xxbLogsWith64BitNumbers(zl303xx_LoggingE eNumId);
zl303xx_BooleanE zl303xxbLogsWith64BitNumbers(zl303xx_LoggingE eNumId)
{
   if(( eNumId == b_20000 ) ||
      ( eNumId == b_20400 ) ||
      ( eNumId == b_20500 ) ||
      ( eNumId == b_20600 ) ||
      ( eNumId == b_23001 ) ||
      ( eNumId == b_23007 ) ||
      ( eNumId == b_23301 ) ||
      ( eNumId == b_23700 ) ||
      ( eNumId == b_23900 ) ||
      ( eNumId == b_24301 ) ||
      ( eNumId == b_25000 ) ||
      ( eNumId == b_25001 ) ||
      ( eNumId == b_25400 ))
   {
      return( ZL303XX_TRUE );
   }
   else
   {
      return( ZL303XX_FALSE );
   }
}

char ptpapr_path[] = "/proc/rru/ptpapr";


/** zl303xxhandleBLogs

   This routine handle special considerations associated with b_xxxx logs.

   Currently, sSpecial considerations include:

   1) print 64-bit numbers. Only 1 64-bit number is processed - it is
      added to the end of the formatted string.

****************************************************************************/
void zl303xxhandleBLogs(FILE *logFd, zl303xx_MsgQLogMsgS *msg);
void zl303xxhandleBLogs(FILE *logFd, zl303xx_MsgQLogMsgS *msg)
{
   Uint64S n;
   char a[ZL303XX_STRNG_SIZE];
   Sint32T idx;
   char*  buf = (char*)logFd;
   int len = 0;


   if( zl303xxbLogsWith64BitNumbers(msg->eNumId) == ZL303XX_TRUE )
   {
      n.hi = msg->arg0;
      n.lo = msg->arg1;
      zl303xxsprintSignedLong(n, a, &idx);

      len += snprintf(buf,MAX_FMT_LEN - len,(const char*)msg->pFmtStr);
      len += snprintf(buf + len,MAX_FMT_LEN - len, "%s", &(a[idx+1]));

   }
   else
   {
      len += snprintf(buf + len,MAX_FMT_LEN - len, (const char*)msg->pFmtStr, msg->arg0, msg->arg1, msg->arg2, msg->arg3, msg->arg4, msg->arg5);
   }
}


/** zl303xx_MsgQLogTaskFn

  The task receives "trace" msgs from the msgQ for customer
  logging.

  Return Value: Sint32T

****************************************************************************/
#if defined ZL_BYPASS_MSGQ
Sint32T zl303xx_MsgQLogTaskFn(zl303xx_MsgQLogMsgS msg);
Sint32T zl303xx_MsgQLogTaskFn(zl303xx_MsgQLogMsgS msg)
{
#else
Sint32T zl303xx_MsgQLogTaskFn(void);
Sint32T zl303xx_MsgQLogTaskFn(void)
{
    zl303xx_MsgQLogMsgS msg;
#endif
    zlStatusE status = OS_OK;
    Uint32T nBytes;
    FILE *logFd = OUTPUT_STRM;  /* Default logging stream (stdout) */

    char buf[MAX_FMT_LEN];
    int len;

    /* VxWorks #defines stdin/stdout/stderr to expand to a function that returns
     * a (FILE *) based on the current task context. This (FILE *) cannot be
     * used from within the context of a different task, so passing it around in
     * a global is invalid. For VxWorks, force the logging to OUTPUT_STRM. */

#if ! defined ZL_BYPASS_MSGQ

    while (status == ZL303XX_OK)
    {

     memset(buf,0,sizeof(buf));
     len = 0;
 
  #ifndef OS_VXWORKS
      if (fdToLogInto)    /* Deal with dynamic file pointers */
      {
          logFd = fdToLogInto;    /* Customer provided */
          logFd = logFd;
      }
  #endif
      if (zl303xx_ActiveMsgQId == OS_MSG_Q_INVALID)
       {
          OS_TASK_DELAY(1000);  /* yield processor */
          continue;
       }
       else /* Wait until a message is received */
       {
           memset(&msg, 0, sizeof(msg));
           nBytes = OS_MSG_Q_RECEIVE(zl303xx_LogToMsgQId,
                                     (Sint8T*)&msg,
                                     sizeof(msg),
                                     OS_WAIT_FOREVER);

           if (msg.logMsgType == SHUTDOWN_MSGQ)
           {
               OS_TASK_DELAY(2000);
               break;
           }

    #if defined ZL_MSGQ_FMT_IN_MSG
          /* Contents are inside the message itself, update pointer due to msgQ memcpy */
          msg.pFmtStr = &msg.fmtStr[0];
    #endif
#else
    {   /* scope */
        if (fdToLogInto)    /* Deal with dynamic file pointers */
        {
            logFd = fdToLogInto;    /* Customer provided */
        }

        /* Deal with the logging msg as a function call */
        {
            nBytes = sizeof(msg);
#endif
          OS_TASKMON_FUNC_START();

          /* Check if we actually received a message */
          if (nBytes != (Uint32T) OS_ERROR)
          {

              LOG2MSGQ_STATS_COUNT(printed);

              if (msg.logMsgType == APR_ENUM_TYPE)                                   /* eNum-based msgs from APR */
              {
                  if (msg.eNumId >= C_0000 && msg.eNumId < A_90000)                                 /* Print eNums keyed C_xxxx */
                  {
                      if (msg.pFmtStr)
                      {
                          len += snprintf(&buf[len],sizeof(buf) - len,(const char*)msg.pFmtStr, msg.arg0, msg.arg1, msg.arg2, msg.arg3, msg.arg4, msg.arg5);
                      }
                      else
                          printf( "\nzl303xx_MsgQLogTaskFn: Format string (for C_xxxx) missing for Enum=%d", msg.eNumId);
                  }
                  else if (msg.eNumId >= A_0000 && msg.eNumId <= A_END)                             /* Print eNums keyed A_xxxx */
                  {
                      if (msg.pFmtStr)
                          len += snprintf(&buf[len],sizeof(buf) - len,"%s ", msg.pFmtStr);
                      len += snprintf(&buf[len],sizeof(buf) - len, "a_%d=%d, a_%d=%d, a_%d=%d, a_%d=%d, a_%d=%d, a_%d=%d",
                              msg.eNumId, (SINT_T)msg.arg0, msg.eNumId+10, (SINT_T)msg.arg1, msg.eNumId+20, (SINT_T)msg.arg2,
                              msg.eNumId+30, (SINT_T)msg.arg3, msg.eNumId+40, (SINT_T)msg.arg4, msg.eNumId+50, (SINT_T)msg.arg5);
                  }
                  else if (msg.eNumId >= b_1000 && msg.eNumId <= b_END)                             /* Print eNums keyed B_xxxx */
                  {
                      if (msg.pFmtStr)
                      {
                          zl303xxhandleBLogs((FILE*)buf, &msg);
                      }
                      else
                      {
                          len += snprintf(&buf[len],sizeof(buf) - len, "a_%d=%d, a_%d=%d, a_%d=%d, a_%d=%d, a_%d=%d, a_%d=%d" ,
                                 msg.eNumId, (SINT_T)msg.arg0, msg.eNumId+10, (SINT_T)msg.arg1, msg.eNumId+20, (SINT_T)msg.arg2,
                                 msg.eNumId+30, (SINT_T)msg.arg3, msg.eNumId+40, (SINT_T)msg.arg4, msg.eNumId+50, (SINT_T)msg.arg5);
                      }
                  }

                  else if (msg.eNumId >= A_90000 && msg.eNumId < LAST_LOG_ENUM)                     /* Print eNums keyed A_90xxx */
                  {
                      if (msg.pFmtStr)
                      {
                          len += snprintf(&buf[len],sizeof(buf) - len, (const char*)msg.pFmtStr, msg.arg0, msg.arg1, msg.arg2, msg.arg3, msg.arg4, msg.arg5);
                      }
                      else
                      {
                          len += snprintf(&buf[len],sizeof(buf) - len,"a_%d=%d, a_%d=%d, a_%d=%d, a_%d=%d, a_%d=%d, a_%d=%d",
                                  msg.eNumId, (SINT_T)msg.arg0, msg.eNumId+10, (SINT_T)msg.arg1, msg.eNumId+20, (SINT_T)msg.arg2,
                                  msg.eNumId+30, (SINT_T)msg.arg3, msg.eNumId+40, (SINT_T)msg.arg4, msg.eNumId+50, (SINT_T)msg.arg5);
                      }
                  }
                  else
                      printf( "\nzl303xx_MsgQLogTaskFn: msg.logMsgType=%d, Unhandled Enum=%d" ZL303XX_NEWLINE, msg.logMsgType, msg.eNumId);
              }
              else if (msg.logMsgType == FMT_STR_TYPE && msg.eNumId == LOG_FMT_STR)  /* Fully Formatted msgs (i.e., ZL303XX_TRACE) */
              {
                  if (msg.pFmtStr)
                  {
                      len += snprintf(&buf[len],sizeof(buf) - len, "[%u.%09u] %s", msg.cpuTime.hi, msg.cpuTime.lo,(const char*)msg.pFmtStr);
                  }
                  else
                      printf( "zl303xx_MsgQLogTaskFn: No format string provided!\n");
              }
              else  /* ??? */
              {
                  printf("zl303xx_MsgQLogTaskFn: Unknown msg.logMsgType=%d, msg.eNumId=%d\n", msg.logMsgType, msg.eNumId);
              }


          #ifdef CUSTOMER_SPECIFIC_CODE
              /* INSERT CUSTOM CODE HERE - to handle the APR log enum values you want */

              /* EXAMPLE - Scale SYNC PPS rate (@ a_1050) for the ~9.6 second sample period */
              if (msg.eNumId == A_1000) fprintf(logFd,"Sync PPS rate= %d\n", msg.arg5*10/96);



              if (msg.logMsgType == APR_ENUM_TYPE)  /* APR EnuM ID is used as the base for args 0-5 (see above) */
              {
                  if (msg.pFmtStr)
                      fprintf(logFd, "%s ", msg.pFmtStr);
                  fprintf(logFd, "AprEnum=%d, arg0=%d, %d, %d, %d, %d, %d"  ZL303XX_NEWLINE,
                          msg.eNumId, msg.arg0, msg.arg1, msg.arg2, msg.arg3, msg.arg4, msg.arg5);
              }
              else if (msg.logMsgType == FMT_STR_TYPE && msg.eNumId == LOG_FMT_STR)                     /* Formatted msgs */
              {
                  fprintf(logFd, (const char*)msg.pFmtStr, msg.arg0, msg.arg1, msg.arg2, msg.arg3, msg.arg4, msg.arg5);
              }
              else  /* ??? */
              {
                  printf("zl303xx_MsgQLogTaskFn: Unknown msg.logMsgType=%d, msg.eNumId=%d\n", msg.logMsgType, msg.eNumId);
              }
          #endif
         }

#if !defined ZL_MSGQ_FMT_IN_MSG
          if (msg.pFmtStr)
          {
              busyFmtBits &= ~((Uint64T)1 << msg.indx);        /* clear this busy buffer bit */
          }
#endif

			bpf_com_plog_ast(  ptpapr_path ,E_AST_LV_INFO,msg.logMsgType,buf);


          OS_TASKMON_FUNC_END();
       }

    }   /* while */

    return status;
}

/** zl303xx_SetupLogToMsgQ

*  The function sets the msgQ for customer logging.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_SetupLogToMsgQ(FILE *logfd)
{
    zlStatusE status = ZL303XX_OK;
   if (logfd)
    {
        fdToLogInto = logfd;
    }
#if ! defined ZL_BYPASS_MSGQ
    if (zl303xx_LogToMsgQId != OS_MSG_Q_INVALID)
    {
        status = ZL303XX_MULTIPLE_INIT_ATTEMPT;
    }
    
#if defined ZL303XX_LOG_HISTORY_ENABLED
    if (status == ZL303XX_OK && zl303xx_LogBufferIsValid(zl303xx_LogHistoryBuffer) != ZL303XX_OK)
    {
        status = zl303xx_LogBufferCreate(&zl303xx_LogHistoryBuffer, ZL303XX_LOG_HISTORY_BUFFER_SIZE);
        if (status != ZL303XX_OK)
        {
           ZL303XX_ERROR_NOTIFY("zl303xx_SetupLogToMsgQ: zl303xx_LogBufferCreate failed");
        }
    }
#endif

    if (status == ZL303XX_OK)
    {
        zl303xx_LogToMsgQId = OS_MSG_Q_CREATE(ZL303XX_LOG_TASK_MSGQ_SIZE,
                                                  sizeof(zl303xx_MsgQLogMsgS));
          /* Make sure the queue creation was successful */
        if (zl303xx_LogToMsgQId == OS_MSG_Q_INVALID)
        {
           status = ZL303XX_RTOS_MSGQ_CREATE_FAIL;
           ZL303XX_ERROR_TRAP("zl303xx_SetupLogToMsgQ: queue creation failed");
        }
        else
            zl303xx_ActiveMsgQId = zl303xx_LogToMsgQId;
    }

    if (status == ZL303XX_OK)
    {
        /* Make sure the task is not already running */
        if (zl303xx_LogToMsgQTaskId != OS_TASK_INVALID)
        {
           status = ZL303XX_MULTIPLE_INIT_ATTEMPT;
           ZL303XX_ERROR_NOTIFY("zl303xx_SetupLogToMsgQ: task already exists");

        }
        else  /* Create the Logging message processing task. */
        {
           zl303xx_LogToMsgQTaskId = OS_TASK_SPAWN("zlLogToMsgQTaskzl303xx",
                                                 ZL303XX_LOG_MSGQ_TASK_PRIORITY,
                                                 0,
                                                 ZL303XX_LOG_MSGQ_TASK_STACK_SIZE,
                                                 (OS_TASK_FUNC_PTR)zl303xx_MsgQLogTaskFn,
                                                 ZL303XX_FALSE);

           /* Make sure we started */
           if (zl303xx_LogToMsgQTaskId == OS_TASK_INVALID)
           {
              status = ZL303XX_RTOS_TASK_CREATE_FAIL;
              ZL303XX_ERROR_TRAP("zl303xx_SetupLogToMsgQ: task creation failed");
           }
        }
    }
#endif

    /* Initialize static globals. */
#if !defined ZL_MSGQ_FMT_IN_MSG
    if (status == ZL303XX_OK)
    {
       memset(fmtArray, 0, sizeof(fmtArray));
       busyFmtBits = 0;
       lastFmtIndex = 0;
    }
#endif

    return status;
}

/** zl303xx_ShutdownLogToMsgQ

*  The function stops the msgQ tasks for customer logging.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_ShutdownLogToMsgQ(FILE *logFd)
{
    zlStatusE status = ZL303XX_OK;

#if ! defined ZL_BYPASS_MSGQ
    zl303xx_MsgQLogMsgS msg;

    memset(&msg, 0, sizeof(msg));
    msg.logMsgType = SHUTDOWN_MSGQ;

    OS_MSG_Q_SEND(zl303xx_LogToMsgQId,(Sint8T *)&msg, sizeof(msg), OS_WAIT_FOREVER);
    OS_TASK_DELAY(100);

    /* Delete the MsgQ logging task. */
    if (status == ZL303XX_OK && zl303xx_LogToMsgQTaskId != OS_TASK_INVALID)
    {
       if (OS_TASK_DELETE(zl303xx_LogToMsgQTaskId) == OS_OK)
       {
          zl303xx_LogToMsgQTaskId = OS_TASK_INVALID;
       }
       else
       {
          printf("   (ERROR) MsgQ Logging task delete failed.\n");
          status = ZL303XX_RTOS_TASK_DELETE_FAIL;
       }
    }

    /* Check if the queue was created */
    if (zl303xx_LogToMsgQId == OS_MSG_Q_INVALID)
    {
       /* The queue does not exist. NOTIFY the user of the scenario but do not
          return an error since the state of the queue is what the user wants. */
       printf("zl303xx_ShutdownLogToMsgQ: Logging queue does not exist\n");
    }
    else  /* Delete the queue */
    {
       OS_STATUS osStatus = OS_MSG_Q_DELETE(zl303xx_LogToMsgQId);

       /* Make sure we were able to delete the queue */
       if (osStatus != OS_OK)
       {
          status = ZL303XX_RTOS_MSGQ_DELETE_FAIL;
          printf("zl303xx_ShutdownLogToMsgQ: Error deleting queue\n");
       }
    }
#endif

    /* Reset the queue ids */
    zl303xx_LogToMsgQId     = OS_MSG_Q_INVALID;
    zl303xx_ActiveMsgQId = OS_MSG_Q_INVALID;

    fdToLogInto = NULL;

    if (logFd)
    {
       fflush(logFd);
    }
    
#if defined ZL303XX_LOG_HISTORY_ENABLED
    if (status == ZL303XX_OK && zl303xx_LogBufferIsValid(zl303xx_LogHistoryBuffer) == ZL303XX_OK)
    {
        status = zl303xx_LogBufferDelete(&zl303xx_LogHistoryBuffer);
        if (status != ZL303XX_OK)
        {
            printf("zl303xx_ShutdownLogToMsgQ: Error deleting queue\n");
        }
    }
#endif

    return status;
}

/** zl303xx_ReEnableLogToMsgQ

*  The function re-enables the msgQ for customer logging.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_ReEnableLogToMsgQ(void)
{
    zlStatusE status = ZL303XX_OK;

    zl303xx_ActiveMsgQId = zl303xx_LogToMsgQId;

    ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 2,
          "zl303xx_ReEnableLogToMsgQ", 0, 0, 0, 0, 0, 0);

    return status;
}

/** zl303xx_DisableLogToMsgQ

*  The function disables the msgQ for customer logging.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_DisableLogToMsgQ(void)
{
    zlStatusE status = ZL303XX_OK;

    ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 2,
          "zl303xx_DisableLogToMsgQ", 0, 0, 0, 0, 0, 0);
    OS_TASK_DELAY(500);

    zl303xx_ActiveMsgQId = OS_MSG_Q_INVALID;

    return status;
}

/* zl303xx_ChangeLogToMsgQFilePtr

*  The function changes the file pointer for customer logging.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_ChangeLogToMsgQFilePtr(FILE *logFd)
{
    zlStatusE status = ZL303XX_OK;

    ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 2,
          "zl303xx_ChangeLogToMsgQFilePtr", 0, 0, 0, 0, 0, 0);

    if (logFd)
    {
        if (fdToLogInto)
            fclose(fdToLogInto);

        fdToLogInto = logFd;
    }
    else
    {
        ZL303XX_ERROR_NOTIFY("zl303xx_ChangeLogToMsgQFilePtr: NULL pointer");
        status = ZL303XX_INVALID_POINTER;
    }

    return status;
}

/* zl303xx_Log3ToMsgQ

A wrapper used by APR internal function to bring additional info: time, device and server IDs to the user.

  Return Value: zlStatusE

****************************************************************************/
Sint32T zl303xx_Log3ToMsgQ(zl303xx_LoggingE eNum, const char *str, UnativeT arg0, UnativeT arg1,
                         UnativeT arg2, UnativeT arg3, UnativeT arg4, UnativeT arg5, UnativeT deviceId, UnativeT serverId)
{
    #define ADD_TIME_IDS_LEN 40 /* [1415146058.1234, 0x102c2c20, x] */
    char buf[MAX_FMT_LEN] = {0};
    Uint64S currentTime = {0,0};
    Uint32T usrLen = 0;

    OS_TIME_GET(&currentTime, NULL);

    if (str)
        usrLen = strlen(str);

    if (usrLen +ADD_TIME_IDS_LEN >= MAX_FMT_LEN)
    {
        fprintf(stderr, "zl303xx_Log3ToMsgQ: Formatted length too long (%d >= MAX_FMT_LEN=%d); cannot add time and ids.\n", usrLen, MAX_FMT_LEN);
        memcpy(buf, str, usrLen);
    }
    else
    {
        if (str)
            snprintf(buf, MAX_FMT_LEN, "[%u.%09u, %p:%u] %s", currentTime.hi, currentTime.lo, (void*)deviceId, (Uint16T)serverId, str);
        else
            snprintf(buf, MAX_FMT_LEN, "[%u.%09u, %p:%u]", currentTime.hi, currentTime.lo, (void*)deviceId, (Uint16T)serverId);
    }

    return zl303xx_LogToMsgQ(eNum, buf, (UnativeT)arg0, (UnativeT)arg1, (UnativeT)arg2, (UnativeT)arg3, (UnativeT)arg4, (UnativeT)arg5);
}

/** zl303xx_LogToMsgQ

  The function sends "trace" msgs to the msgQ for customer
  logging.

  Those encoded as eNums carry the data in arg0-5 so the
  values can be used by customer code (whether they are
  formatted strings or not). This is used for a majority of APR
  messages.

  Fully formatted (i.e., ZL303XX_TRACE) are handled here and
   APR-generated messages (fully formatted or not) are usually
   forwarded through the msgQ to a receiver task (zl303xx_MsgQLogTaskFn) for
   re-constitution and printing and/or further customer post-processing.

  Notes:
 When exact output (i.e., debugging sessions with Microsemi) is the priority rather than
 algorithm performance, setting the flags (ZL_BYPASS_MSGQ, ZL_MSGQ_FMT_IN_MSG or
 ZL_FORCE_FMT_THRU_Q_TOO) may (slightly/greatly) degrade timing performance due to I/O logging
 blocking task scheduling.
 "Q Full for Formatted" and/or "Q Full for Non-Formatted" output means your system is too
 busy to handle logging at the current priority of the receiver task ( zl303xx_MsgQLogTaskFn()
 see ZL303XX_LOG_MSGQ_TASK_PRIORITY ).

  Return Value: zlStatusE

****************************************************************************/
Sint32T zl303xx_LogToMsgQ(zl303xx_LoggingE eNum, const char *str, UnativeT arg0, UnativeT arg1,
                         UnativeT arg2, UnativeT arg3, UnativeT arg4, UnativeT arg5)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_MsgQLogMsgS msg;
    Uint32T msgLen = sizeof(msg);
    Uint64S currentTime;

#if !defined ZL_MSGQ_FMT_IN_MSG
    Uint8T fmtIndex = 0;
#endif
    FILE *logFd = OUTPUT_STRM;  /* Default logging stream (stdout) */

    /* VxWorks #defines stdin/stdout/stderr to expand to a function that returns
     * a (FILE *) based on the current task context. This (FILE *) cannot be
     * used from within the context of a different task, so passing it around in
     * a global is invalid. For VxWorks, force the logging to OUTPUT_STRM. */
#ifndef OS_VXWORKS
    if (fdToLogInto)
    {
        logFd = fdToLogInto;    /* Customer provided */
        logFd = logFd;
    }
#endif

    LOG2MSGQ_STATS_COUNT(calls);
    LOG2MSGQ_STATS_PRINT(logFd);

    OS_TIME_GET(&currentTime, NULL);

    memset(&msg, 0, msgLen);
    msg.cpuTime = currentTime;


    if (eNum == LOG_FMT_STR)      /* Non-Apr msgs are NOT sent through the Q (for backward compatibility) */
    {
        if (str)
        {
          #ifdef ZL_FORCE_FMT_THRU_Q_TOO
            /* Performance testing has proven that more intense logging works better through the Q so msgs may be dropped */
            msg.eNumId = eNum;
            msg.logMsgType = FMT_STR_TYPE;  /* fully formatted */
          #else
            char buf[MAX_FMT_LEN] = {0};

            snprintf((char*)buf, MAX_FMT_LEN, str , arg0, arg1, arg2, arg3, arg4, arg5);   /* formatted str */
			bpf_com_plog_ast(  ptpapr_path ,E_AST_LV_INFO,0,buf);
            return 0;
          #endif
        }
        else
        {
            printf( "zl303xx_LogToMsgQ: No format string provided!\n");
            LOG2MSGQ_STATS_COUNT(error_nostr);
            return ZL303XX_ERROR;
        }
    }
    else
    {
        /* Prep the msg */
        msg.eNumId = eNum;
        msg.logMsgType = APR_ENUM_TYPE; /* Apr eNUM types carry values */
        msg.arg0 = arg0;
        msg.arg1 = arg1;
        msg.arg2 = arg2;
        msg.arg3 = arg3;
        msg.arg4 = arg4;
        msg.arg5 = arg5;
    }

    if (str)     /* Enums with formatted strings  */
    {
        Uint16T fmtLen = strlen(str);

        if (fmtLen > MAX_FMT_LEN-3)
        {
            printf("zl303xx_LogToMsgQ: String length exceeds MAX_FMT_LEN configuration!\n");
            LOG2MSGQ_STATS_COUNT(error_toobig);
            return 0;
        }

#if !defined ZL_MSGQ_FMT_IN_MSG
        fmtIndex = ((lastFmtIndex + 1) % FMT_ARRAY_SIZE);   /* wrap if needed */

        if (busyFmtBits & ((Uint64T)1 << fmtIndex))         /* buffer busy? */
        {
            /* fputs("\n***** Buffer Full!\n", stderr);*/   /* Printing this will definitely kill the ability to catch up and continue!!! */
            LOG2MSGQ_STATS_COUNT(error_busy);
            return 0;
        }

        memset(&fmtArray[fmtIndex][0], 0, MAX_FMT_LEN);
        busyFmtBits |= ((Uint64T)1 << fmtIndex);            /* Set this busy bit */
        msg.indx = fmtIndex;
        msg.pFmtStr = &(fmtArray[fmtIndex][0]);             /* Put address of local copy into msg */
#else
        /* Place formatted string inside the msg itself */
        msg.pFmtStr = &msg.fmtStr[0];
#endif
        snprintf((char*)msg.pFmtStr, MAX_FMT_LEN-3, str, arg0, arg1, arg2, arg3, arg4, arg5);   /* put formatted str into array  - null terminated at LEN-3 or before */
    }

#if ! defined ZL_BYPASS_MSGQ
    if (zl303xx_ActiveMsgQId == OS_MSG_Q_INVALID)    /* Not yet or logging disabled */
    {

#if !defined ZL_MSGQ_FMT_IN_MSG
        /* Cleanup unused space */
        busyFmtBits &= ~((Uint64T)1 << fmtIndex);
#endif
        LOG2MSGQ_STATS_COUNT(error_notactive);
        return ZL303XX_NOT_RUNNING;
    }

    /* Queue the message. If there is no space then discard the message.
     This saves the Q from becoming overloaded */
   if (OS_MSG_Q_SEND(zl303xx_LogToMsgQId,
                     (Sint8T *)&msg, msgLen,
                     OS_NO_WAIT) != OS_OK)
   {
       LOG2MSGQ_STATS_COUNT(error_full);
       status = ZL303XX_RTOS_MSGQ_SEND_FAIL;
       if (str)
       {
#if !defined ZL_MSGQ_FMT_IN_MSG
           busyFmtBits &= ~((Uint64T)1 << fmtIndex);        /* clear this busy buffer bit */
           fmtIndex = lastFmtIndex;
#endif
           fputs("\n***** Q Full for Formatted\n", stderr);
       }
       else
           fputs("\n***** Q Full for Non-Formatted\n", stderr);
   }
   else
   {
       LOG2MSGQ_STATS_COUNT(sent);
#if !defined ZL_MSGQ_FMT_IN_MSG
       lastFmtIndex = fmtIndex;
#endif
       return msgLen;   /* Normal */
   }


#else
  #if !defined ZL_MSGQ_FMT_IN_MSG
   lastFmtIndex = fmtIndex;
  #endif

 #if defined ONLY_USE_PRINTF
   /* #warning IF ALL ELSE FAILS - USE THESE LINES (TEMPORARY - JUST TO GET STARTED!) */
   if (msg.logMsgType == APR_ENUM_TYPE)
       printf("a_%d=%d, a_%d=%d, a_%d=%d, a_%d=%d, a_%d=%d, a_%d=%d" ZL303XX_NEWLINE,
              msg.eNumId, (SINT_T)msg.arg0, msg.eNumId+10, (SINT_T)msg.arg1, msg.eNumId+20, (SINT_T)msg.arg2,
              msg.eNumId+30, (SINT_T)msg.arg3, msg.eNumId+40, (SINT_T)msg.arg4, msg.eNumId+50, (SINT_T)msg.arg5);
   if (msg.logMsgType == FMT_STR_TYPE)
   printf("%s\n", msg.pFmtStr);
   busyFmtBits &= ~((Uint64T)1 << fmtIndex);
   return sizeof(msg);
 #else
   return zl303xx_MsgQLogTaskFn(msg);            /* This will print from the global buffer */
 #endif
#endif
   return status;
}


