

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains the error handling function used within the
*     zl303xx API
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"
#include "zl303xx_ErrTrace.h"
#include "zl303xx_Os.h"

#include <string.h>

#if defined OS_LINUX
#include <execinfo.h>

#endif

/*****************   DEFINES     **********************************************/

/* Define a function which will abort the current thread. Make sure the prototype
   is also defined */
#ifndef ZL303XX_ABORT_FN
   #include <stdlib.h>
   #ifdef OS_VXWORKS
         #define ZL303XX_ABORT_FN exit(-1)
   #else
      #ifdef OS_LINUX
       #if defined USE_LEGACY_ABORT_FN_EXIT
         #include <pthread.h>
/*         #define ZL303XX_ABORT_FN pthread_exit((void*)-1);    // Call your function of choice here */
         #define ZL303XX_ABORT_FN exit(-1);    /* Call your function of choice here */
       #else
         #define ZL303XX_ABORT_FN abort();     /* Call your function of choice here */
       #endif
      #else
         #define ZL303XX_ABORT_FN exit(-1)
      #endif
   #endif
#endif



/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/
#if defined ENFORCE_STRICT_SHUTDOWN
static Uint32T semaMechErrCnt = 0;
static Uint32T mutexMechErrCnt = 0;
static Uint32T taskMechErrCnt = 0;
static Uint32T msgqMechErrCnt = 0;
zl303xx_AbortLevelE userAbortLevel = NO_ABORT; /* Settings above NO_ABORT will call abort() */
#endif


/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

#if defined ENFORCE_STRICT_SHUTDOWN
/* Calls abort() if userAbortLevel > NO_ABORT || mechAbortLevel == ABORT_ON_MAJOR.   
  If undefined, mechanism resource misuse is not dealt with by handleMechanismIssues() */  
void handleMechanismIssues(const char* fileName, Uint32T lineNumber, zl303xx_MechanismsE mechanism, void *mechP, Sint32T errorValue, zl303xx_AbortLevelE mechAbortLevel);
void handleMechanismIssues(const char* fileName, Uint32T lineNumber, zl303xx_MechanismsE mechanism, void *mechP, Sint32T errorValue, zl303xx_AbortLevelE mechAbortLevel)
{
    switch (mechanism)
    {
    case MECH_MSGQ:
        msgqMechErrCnt++;
        break;
    case MECH_MUTEX:
        mutexMechErrCnt++;
        break;
    case MECH_SEMA:
        semaMechErrCnt++;
        break;
    case MECH_TASK:
        taskMechErrCnt++;
        break;
    default:
        ZL303XX_TRACE_ALWAYS("handleMechanismIssues: Unknown mechanism=%d", mechanism, 0,0,0,0,0);
        return;
    }

    ZL303XX_TRACE_ALWAYS("e_0000=%d, e_0010=%d, e_0020=%d, e_0030=%d, e_0040=%d, e_addr=%p", 
                       msgqMechErrCnt, mutexMechErrCnt, semaMechErrCnt, taskMechErrCnt, errorValue, mechP);

    if (userAbortLevel > NO_ABORT || mechAbortLevel == ABORT_ON_MAJOR)
    {
        if (mechAbortLevel == ABORT_ON_MINOR || mechAbortLevel == ABORT_ON_MAJOR)
        {
            ZL303XX_TRACE_ALWAYS("handleMechanismIssues: Called from %s: %d  Calling abort() ...", fileName, lineNumber, 0,0,0,0);
            ZL303XX_TRACE_ALWAYS("msgqMechErrCnt=%d, mutexMechErrCnt=%d, semaMechErrCnt=%d, taskMechErrCnt=%d, errorValue=%d, mechP=%p", 
                               msgqMechErrCnt, mutexMechErrCnt, semaMechErrCnt, taskMechErrCnt, errorValue, mechP);
            OS_TASK_DELAY(500);    /* wait for TRACE_ALWAYS */


            abort();    /* Hopefully, create a core and a backtrace */
        }
    }
}
#endif  /* ENFORCE_STRICT_SHUTDOWN */



/**

  Function Name:
   zl303xx_ErrorTrapFn

  Details:
   Error handling function for the ZL303XX_ API.

  Parameters:
       bStopOnError   if ZL303XX_TRUE this will abort the current thread after
                  generating the error message
       errCodeString  A meaningful, explanatory error string
       fileName       pointer to the filename where the error occurred (may be NULL)
       lineNum        pointer to the line number in the file where the error
                  occurred (may be NULL)

  Return Value:
   Nothing

 *****************************************************************************/

void zl303xx_ErrorTrapFn(const Uint32T bStopOnError, const char *errCodeString,
                       const char *fileName, const char * const lineNum)
{
   if (errCodeString)
   {
      /* Build up the error message. We cannot assume that the error message will
         be displayed immediately so some rules apply:
            1. Any items identified by pointers must be constants
            2. Therefore we can't use any items (particularly) strings on the stack
         However, in our favour is the fact that all the parameters to this function
         are compiler generated constants so we can satisfy these rules if we are careful. */

      if (bStopOnError == ZL303XX_TRUE)
      {
         if (fileName)
         {
            if (lineNum)
            {
               fprintf(stderr, "Fatal Error: \"%s\", in %s, line %s" ZL303XX_NEWLINE,
                     errCodeString,
                     fileName,
                     lineNum);
               ZL303XX_TRACE_ERROR("Fatal Error: \"%s\", in %s, line %s" ZL303XX_NEWLINE,
                     errCodeString,
                     fileName,
                     lineNum,
                     0, 0, 0);
               OS_TASK_DELAY(1000);
            }
            else /* no linenumber given */
            {
               fprintf(stderr, "Fatal Error: \"%s\", in %s\n" ZL303XX_NEWLINE,
                     errCodeString,
                     fileName);
               ZL303XX_TRACE_ERROR("Fatal Error: \"%s\", in %s\n" ZL303XX_NEWLINE,
                     errCodeString,
                     fileName,
                     0, 0, 0, 0);
               OS_TASK_DELAY(1000);
            }
         }
         else
         {
            fprintf(stderr, "Fatal Error: \"%s\"\n",
                  errCodeString);
            ZL303XX_TRACE_ERROR("Fatal Error: \"%s\"\n",
                  errCodeString, 0, 0, 0, 0, 0);
            OS_TASK_DELAY(1000);
         }

         /* We should stop on error so abort the current thread */

         ZL303XX_ABORT_FN;

      }
      else  /* A "less than severe" error */
      {
         if (fileName)
         {
            if (lineNum)
            {
               fprintf(stderr, "Error: \"%s\", in %s, line %s\n",
                     errCodeString, fileName, lineNum);
               ZL303XX_TRACE_ERROR("Error: \"%s\", in %s, line %s\n",
                     errCodeString, fileName, lineNum, 0, 0, 0);
            }
            else /* no linenumber given */
            {
               fprintf(stderr, "Error: \"%s\", in %s\n",
                     errCodeString, fileName);
               ZL303XX_TRACE_ERROR("Error: \"%s\", in %s\n",
                     errCodeString, fileName, 0, 0, 0, 0);
            }
         }
         else
         {
            fprintf(stderr, "Error: \"%s\"\n",
                  errCodeString);
            ZL303XX_TRACE_ERROR("Error: \"%s\"\n",
                  errCodeString, 0, 0, 0, 0, 0);
         }
      }
   }
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/*****************   END   ****************************************************/

