

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains support for Tracing which is used to output log messages
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Os.h"
#include "zl303xx_Trace.h"
#include "zl303xx_LogBuffer.h"
#include "zl303xx_Error.h"
#include "zl303xx_LogToMsgQ.h"

#include "BPF_COM_PLOG.h"

#include <string.h> /* for memcpy */

/*****************   DEFINES     **********************************************/
char trclog_path[] = "/proc/rru/trc_ptp";
char trclog_path_high[] = "/proc/rru/trc_ptp_h";

/**   STATIC FUNCTION DECLARATIONS   ***************************/

/**   STATIC GLOBAL VARIABLES   ********************************/

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/
/* The trace control table. Initialised to all tracing off (except error
   messages). */
zl303xx_TraceFilterS Zl303xx_TraceFilter[ZL303XX_NUM_TRACE_MODULES] = {{0,0}};

FILE *fdOfTrace = NULL;

zl303xx_LogBufferT zl303xx_TraceLogBuffer = ZL303XX_LOG_BUFFER_INIT;

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/**

  Function Name:
   zl303xx_TraceInit

  Details:
   Initialise tracing facilities

  Parameters:
   [in]   logFd       A file pointer to use for logging. The file descriptor must have
                         been previously created and opened with fopen() or
                         else one of the predefined values: stdin or stderr.

  Return Value:
   Nothing

  Notes:
   Does not affect the tracing filter state

*******************************************************************************/

void zl303xx_TraceInit(FILE *logFd)
{
   fdOfTrace = logFd;   /* Store in a global in case someone else needs it */
   
   OS_LOG_INIT(logFd);

/*#warning MSCC: Open logging file descriptor here if required!*/
   if ((fdOfTrace = fopen("/var/log/PtpApr.log", "w+")) == NULL)
   {
       printf("zl303xx_TraceInit(): open failed on fdOfTrace!\n", 0, 0,0,0,0,0);
       ZL303XX_ERROR_TRAP("zl303xx_TraceInit(): open failed");
   }
   else
   {
       if (zl303xx_SetupLogToMsgQ(fdOfTrace) != ZL303XX_OK)
           printf("zl303xx_TraceInit: zl303xx_SetupLogToMsgQ() failed but system will try to continue\n");
   }

}

/**

  Function Name:
   zl303xx_TraceClose

  Details:
   Close down tracing facilities

  Parameters:
       logFd       The file pointer previously opened for logging.

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_TraceClose(FILE *logFd)
{   
   OS_LOG_CLOSE(logFd);

   zl303xx_ShutdownLogToMsgQ(logFd);

   /* Close file descriptor here if opened in zl303xx_TraceInit(). */
#if 0
   /*fclose(logFd);*/
#if defined ZL303XX_TRACE_BUFFER_ENABLED
   if (zl303xx_LogBufferIsValid(zl303xx_TraceLogBuffer) == ZL303XX_OK)
   {
       zlStatusE status = zl303xx_LogBufferDelete(&zl303xx_TraceLogBuffer);
       if (status == ZL303XX_OK)
       {
           zl303xx_TraceLogBuffer = ZL303XX_LOG_BUFFER_INIT;
       }
       else
       {
           printf("zl303xx_TraceClose: Error zl303xx_LogBufferDelete returned %d\n", status);
       }
   }
#endif
#endif	/* if 0 */
	fclose(logFd);
}


/**

  Function Name:
   zl303xx_TraceSetLevel

  Details:
   Sets the trace level for a specific module.
   Trace messages with a level greater than or equal to the trace level will
   be output to the log stream

  Parameters:
       modId       The module number (see zl303xx_ModuleIdE in zl303xx_Trace.h)
                      to apply.
               Range: 0 (i.e. no tracing) to ZL303XX_NUM_MODULE_IDS-1
       level       Trace level to apply
               Range: 0-255

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_TraceSetLevel(Uint16T modId, Uint8T level)
{
   if (modId < ZL303XX_NUM_TRACE_MODULES)
   {
      Zl303xx_TraceFilter[modId].level = level;
   }
}

/**

  Function Name:
    zl303xx_TraceSetLevelAll

  Details:
    Sets specified trace level for all modules

  Parameters:
       level       Trace level to apply.

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_TraceSetLevelAll(Uint8T level)
{
   Sint32T indx;

   for (indx=0; indx<ZL303XX_NUM_TRACE_MODULES; indx++)
   {
      Zl303xx_TraceFilter[indx].level = level;
   }
}

/**

  Function Name:
   zl303xx_TraceGetLevel

  Details:
   Gets the trace level for a specific module.

  Parameters:
       modId       The module number (see zl303xx_ModuleIdE in zl303xx_Trace.h)
                      to apply.
               Range: 0 (i.e. no tracing) to ZL303XX_NUM_MODULE_IDS-1

  Return Value:
   The level of given module or 255 for error (invalid module)

 *****************************************************************************/
Uint8T zl303xx_TraceGetLevel(Uint16T modId)
{
   if (modId < ZL303XX_NUM_TRACE_MODULES)
   {
      return Zl303xx_TraceFilter[modId].level;
   }
   else
   {
       return 255;
   }
}

/**

  Function Name:
   zl303xx_TraceBufferSetLevel

  Details:
   Sets the trace buffer level for a specific module.
   Trace messages with a level greater than or equal to the trace level will
   be output to the trace buffer

  Parameters:
       modId       The module number (see zl303xx_ModuleIdE in zl303xx_Trace.h)
                      to apply.
               Range: 0 (i.e. no tracing) to ZL303XX_NUM_MODULE_IDS-1
       level       Trace level to apply
               Range: 0-255

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_TraceBufferSetLevel(Uint16T modId, Uint8T level)
{
#if !defined ZL303XX_TRACE_BUFFER_ENABLED
    level = 0; /* Force disabled */
#endif
   if (modId < ZL303XX_NUM_TRACE_MODULES)
   {
      Zl303xx_TraceFilter[modId].bufferLevel = level;
   }
}

/**

  Function Name:
    zl303xx_TraceBufferSetLevelAll

  Details:
    Sets specified trace level for all modules

  Parameters:
       level       Trace level to apply.

  Return Value:
   Nothing

 *****************************************************************************/
void zl303xx_TraceBufferSetLevelAll(Uint8T level)
{
   Sint32T indx;
#if !defined ZL303XX_TRACE_BUFFER_ENABLED
    level = 0; /* Force disabled */
#endif
   for (indx=0; indx<ZL303XX_NUM_TRACE_MODULES; indx++)
   {
      Zl303xx_TraceFilter[indx].bufferLevel = level;
   }
}

/**

  Function Name:
   zl303xx_TraceBufferGetLevel

  Details:
   Gets the trace buffered level for a specific module.

  Parameters:
       modId       The module number (see zl303xx_ModuleIdE in zl303xx_Trace.h)
                      to apply.
               Range: 0 (i.e. no tracing) to ZL303XX_NUM_MODULE_IDS-1

  Return Value:
   The level of given module or 255 for error (invalid module)

 *****************************************************************************/
Uint8T zl303xx_TraceBufferGetLevel(Uint16T modId)
{
   if (modId < ZL303XX_NUM_TRACE_MODULES)
   {
      return Zl303xx_TraceFilter[modId].bufferLevel;
   }
   else
   {
       return 255;
   }
}

/**

  Function Name:
   zl303xx_TraceBufferDump

  Details:
   Dumps the trace buffered data to specified file pointer up to given length.

  Parameters:
       filename    Output file stream (FILE*)
       len         Maximum output length, 0 full buffer size
       bErase      Optionally erase the given length after dumping

  Return Value:
   The number of bytes written or zero

 *****************************************************************************/
Sint32T zl303xx_TraceBufferDump(void *fp, Uint32T len, zl303xx_BooleanE bErase)
{
   Sint32T ret = 0;
   ret = zl303xx_LogBufferDump(zl303xx_TraceLogBuffer, fp, len, bErase);
   return ret;
}

/**

  Function Name:
   zl303xx_TraceBufferDumpToFile

  Details:
   Dumps the trace buffered data to specified file up to given length.

  Parameters:
       filename    Output filename
       len         Maximum output length, 0 full buffer size
       bErase      Optionally erase the given length after dumping

  Return Value:
   The number of bytes written or zero

 *****************************************************************************/
Sint32T zl303xx_TraceBufferDumpToFile(const char *filename, Uint32T len, zl303xx_BooleanE bErase)
{
   Sint32T ret = 0;
   ret = zl303xx_LogBufferDumpToFile(zl303xx_TraceLogBuffer, filename, len, bErase);
   return ret;
}

/**

  Function Name:
   zl303xx_TraceBufferErase

  Details:
   Erases contents of trace buffer.

  Parameters:
       len         Amount to erase

 *****************************************************************************/
Sint32T zl303xx_TraceBufferErase(Uint32T len)
{
    return zl303xx_LogBufferErase(zl303xx_TraceLogBuffer, len);
}

/* Resizes the trace buffer */
/**

  Function Name:
   zl303xx_TraceBufferResize

  Details:
   Resizes the trace buffer. If smaller some content may be erased.

  Parameters:
       size         New size (bytes)

 *****************************************************************************/
Sint32T zl303xx_TraceBufferResize(Uint32T size)
{
    return zl303xx_LogBufferResize(zl303xx_TraceLogBuffer, size);
}

/**

  Function Name:
   zl303xx_TraceBufferGetInfo

  Details:
   Gets information from the buffer.

  Parameters:
       len         Amount to erase

 *****************************************************************************/
Sint32T zl303xx_TraceBufferGetInfo(Uint32T *size, Uint32T *count)
{
    return zl303xx_LogBufferGetInfo(zl303xx_TraceLogBuffer, size, count);
}

/**

  Function Name:
    zl303xx_TraceFnBuffered

  Details:
    Trace function for the ZL303XX_ API to place output into buffer.

  Parameters:
       modId    A module ID number (from zl303xxh.)
       level    The requested level of the trace message
       str      A format string, optionally followed by up to 6 arguments

  Return Value:
   Nothing

*******************************************************************************/
void zl303xx_TraceFnBuffered(Uint32T modId, Uint32T level, const char *str,
      UnativeT arg0, UnativeT arg1, UnativeT arg2, UnativeT arg3, UnativeT arg4, UnativeT arg5)
{
    /* Wrap string to include CPU time, module/level, and line termination */
    char buf[256];
    Sint32T ret;
    Uint32T n = 0;
    
    Uint64S cpuTime;
    OS_TIME_GET(&cpuTime, NULL);
    
    /* Prepend trace with CPU time and module:level (for analysis) */
    ret = OS_SNPRINTF(buf, sizeof(buf), "[%u.%09u, %02u:%u] ", cpuTime.hi, cpuTime.lo, (Uint8T)modId, level);
    if (ret > 0)
    {
        if ((Uint32T)ret >= sizeof(buf))
        {
            ret = sizeof(buf);
        }
        n += ret;
    }

    /* Format trace into remaining space */
    ret = OS_SNPRINTF(&buf[n], sizeof(buf) - n, str, arg0, arg1, arg2, arg3, arg4, arg5); 
    if (ret > 0)
    {
        if ((Uint32T)ret >= sizeof(buf) - n)
        {
            ret = sizeof(buf) - n;
        }
        n += ret;
    }
    
    /* Append newline, efficiently, even for truncated lines */
    if ((Uint32T)n > (sizeof(buf) - sizeof(ZL303XX_NEWLINE)))
    {
        n = sizeof(buf) - sizeof(ZL303XX_NEWLINE); /* Make room */
    }
    OS_MEMCPY(&buf[n], ZL303XX_NEWLINE, sizeof(ZL303XX_NEWLINE));
    n += sizeof(ZL303XX_NEWLINE) - 1; /* Ignore nul */

    /* Write to the buffer */
    (void) zl303xx_LogBufferWrite(zl303xx_TraceLogBuffer, buf, n);
    
    return;
}

/**

  Function Name:
    zl303xx_TraceFnFiltered

  Details:
    Trace function for the ZL303XX_ API.
    Determines whether a string should be displayed or not if so sends
    it to the logging stream

  Parameters:
       modId    A module ID number (from zl303xxh.)
       level    The requested level of the trace message
       str      A format string, optionally followed by up to 6 arguments

  Return Value:
   Nothing

  Notes:
   It should not be assumed that the error string will be displayed immediately
   since the error log may be running in a separate thread. Therefore all the
   items supplied to the function must be constant (especially important where
   pointers are used)
   The main purpose of this function is to map a macro with variable arguments
   onto a function with a fixed number of parameters
   

*******************************************************************************/
/* original start
void zl303xx_TraceFnFiltered(Uint32T modId, Uint32T level, const char *str,
      UnativeT arg0, UnativeT arg1, UnativeT arg2, UnativeT arg3, UnativeT arg4, UnativeT arg5)
{
   ZL303XX_DEBUG_FILTER(modId, level)
   { */
      /* This message should be traced */
/*      OS_LOG_MSG(str, arg0, arg1, arg2, arg3, arg4, arg5); */
/*   } */

/*
#if defined ZL303XX_TRACE_BUFFER_ENABLED
   ZL303XX_DEBUG_FILTER_BUFFER(modId, level)
   { */
      /* This message should be traced to the buffer */
/*      zl303xx_TraceFnBuffered(modId, level, str, arg0, arg1, arg2, arg3, arg4, arg5);
   }
#endif

}
original end */

void zl303xx_TraceFnFiltered(Uint32T modId, Uint32T level, const char *str,
      UnativeT arg0, UnativeT arg1, UnativeT arg2, UnativeT arg3, UnativeT arg4, UnativeT arg5, const char* filename, int line)
{
   Uint32T e_level;
   ZL303XX_DEBUG_FILTER(modId, level)
   {
      /* This message should be traced */
      /* OS_LOG_MSG(str, arg0, arg1, arg2, arg3, arg4, arg5);*/

      /* This message should be traced */
      if(level == 1)
      {
          e_level = E_TRC_LV_CRITICAL;
      }
      else if(level == 2)
      {
          e_level = E_TRC_LV_SERIOUS;
      }
      else if(level == 3)
      {
          e_level = E_TRC_LV_INFO;
      } 
      else if(level > 3)
      {
          e_level = E_TRC_LV_DEBUG;
      } 
      else
      {
          e_level = E_TRC_LV_BUG;
      }

		if( (line > 10000) || (line <= 0) ) {
			bpf_com_plog_trace( trclog_path, M_BPF_COM_PLOG_LEVEL(e_level, E_TRC_NO_PTP), 
								"MicroSemiCode1.c?", line  ,str, arg0, arg1, arg2, arg3, arg4, arg5 );
		} else {
			bpf_com_plog_trace( trclog_path, M_BPF_COM_PLOG_LEVEL(e_level, E_TRC_NO_PTP), 
								filename, line, str, arg0, arg1, arg2, arg3, arg4, arg5);
		}
   }

}

/**

  Function Name:
    zl303xx_TraceFnNoFilter

  Details:
    Error trace function for the ZL303XX_ API.
    Always sends its trace parameters to the logging stream

  Parameters:
       str      A format string, optionally followed by up to 6 arguments

  Return Value:
   Nothing

  Notes:
   It should not be assumed that the error string will be displayed immediately
   since the error log may be running in a separate thread. Therefore all the
   items supplied to the function must be constant (especially important where
   pointers are used)
   The main purpose of this function is to map a macro with variable arguments
   onto a function with a fixed number of parameters

*******************************************************************************/
/* original */
/* void zl303xx_TraceFnNoFilter(const char * str,
      UnativeT arg0, UnativeT arg1, UnativeT arg2, UnativeT arg3, UnativeT arg4, UnativeT arg5)
{
   OS_LOG_MSG(str, arg0, arg1, arg2, arg3, arg4, arg5);
*/
/* #if defined ZL303XX_TRACE_BUFFER_ENABLED */
   /* This message should be traced to the buffer */
/*   zl303xx_TraceFnBuffered(-1, 0, str, arg0, arg1, arg2, arg3, arg4, arg5); */
/* } */
/* original end */


void zl303xx_TraceFnNoFilter(const char * str,
      UnativeT arg0, UnativeT arg1, UnativeT arg2, UnativeT arg3, UnativeT arg4, UnativeT arg5, const char* filename, int line, Uint32T level)
{
    Uint32T e_level;
    if(level == 1)
    {
        e_level = E_TRC_LV_CRITICAL;
    } 
    else if(level == 2)
    {
        e_level = E_TRC_LV_SERIOUS;
    } 
    else
    {
        e_level = E_TRC_LV_BUG;
    }

	if((line > 10000)|| (line <= 0) || (level > 2)){
		bpf_com_plog_trace( trclog_path_high, M_BPF_COM_PLOG_LEVEL(E_TRC_LV_CRITICAL, E_TRC_NO_PTP_H),
							"MicroSemiCode2.c?", line  ,str, arg0, arg1, arg2, arg3, arg4, arg5 );
	} else {
		bpf_com_plog_trace( trclog_path_high, M_BPF_COM_PLOG_LEVEL(e_level, E_TRC_NO_PTP_H),
							filename, line, str, arg0, arg1, arg2, arg3, arg4, arg5);
	}

}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/*****************   END   ****************************************************/

