

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     LibC abstractions of some functions as some compilers have issues.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_LibcAbstractions.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#if !defined OS_VXWORKS
#include <memory.h>
#endif
#include <string.h>
#include <errno.h>

/** Libc Wrappers are used to allow us to deliver compiled
   code without directly calling libc functions */

int zl303xx_OsAbs(int value)
{
    return abs(value);  /* Call the hosts function */
}

void *zl303xx_OsMemcpy(void *destP, const void *sourceP, size_t size)
{
    return memcpy(destP, sourceP, size); /* Call the hosts function */
}

void *zl303xx_OsMemset(void *addrP, int value, size_t size)
{
    return memset(addrP, value, size); /* Call the hosts function */
}

static int host_snprintf(char *destP, int size, const char *formatP, va_list varList);
static int host_snprintf(char *destP, int size, const char *formatP, va_list varList)
{
    return vsnprintf(destP, size, formatP, varList); /* Call the hosts function */
}

int zl303xx_OsSnprintf(char *destP, int size, const char *formatP, ...)
{
    int retVal = 0;
    va_list argp;

    va_start(argp, formatP);
    retVal = host_snprintf(destP, size, formatP, argp);
    va_end(argp);

    return retVal;
}

static int host_sprintf(char *destP, const char *formatP, va_list varList);
static int host_sprintf(char *destP, const char *formatP, va_list varList)
{
    return vsprintf(destP, formatP, varList);    /* Call the hosts function */
}

int zl303xx_OsSprintf(char *destP, const char *formatP, ...)
{
    int retVal = 0;
    va_list argp;

    va_start(argp, formatP);
    retVal = host_sprintf(destP, formatP, argp);
    va_end(argp);

    return retVal;
}
 
size_t zl303xx_OsStrlen(const char *stringP)
{
    return strlen(stringP); /* Call the hosts function */
}

char *zl303xx_OsStrncat(char *destP, const char *sourceP, size_t size)
{
    return strncat(destP, sourceP, size); /* Call the hosts function */
}

int zl303xx_OsErrno(void)
{
    return errno; /* Return the errno value */
}

#if defined MISSING_SNPRINTF || defined __VXWORKS_54
int snprintf(char *destP, int size, const char *formatP, ...)
{
    va_list argp;
    int ret = 0;
    char *buffer = calloc(2, size); /* extra room */
    char *ptr = NULL;

    if (buffer)
    {
        va_start(argp, formatP);
        ret = vsprintf(buffer, formatP, argp);
        va_end(argp);

        ret = min(size, ret + 1);      /* Smallest wins. Add 1 to ret for NULL terminator. */
        ptr = strncpy(destP, buffer, ret);
        destP[ret] = '\0';             /* Insurance str termination */
    }

    if (ptr)
    {
        int len = strlen(buffer);

        free(buffer);
        return len;
    }
    else
        return OS_ERROR;
}

#endif  /* MISSING_SNPRINTF */

#if defined REPLACE_OS_PRINTF
/* BEWARE - You must have very special needs if you are even contenplating defining this!!! */



int zl303xx_OsFprintf(FILE* fp, const char *format, ...)
{
    Uint32T n;
    char buffer[256] = {0};
    va_list args;
    va_start(args, format);

    n = vsnprintf(&buffer[0], sizeof(buffer), format, args);
    if (n > 0)
    {
        if (n > sizeof(buffer))
        {
            n = sizeof(buffer);
            buffer[n-1] = '\0';
            n--;
        }

        fputs(buffer, fp);
        fflush(fp);
    }

    va_end(args);

    return n;
}

/* Alternative :
int zl303xx_OsPrintf(const char *format, ...)
{
    return zl303xx_OsFprintf(stdout, format);
}
*/


int zl303xx_OsPrintf(const char *formatP, ...)
{
/* #warning REPLACING printf! */
    int retVal = -1;
    va_list argp;

    if (formatP == NULL)
        return retVal;

    va_start(argp, formatP);
    retVal = vprintf(formatP, argp);
    va_end(argp);

    return retVal;
}


#endif  /* REPLACE_OS_PRINTF */

