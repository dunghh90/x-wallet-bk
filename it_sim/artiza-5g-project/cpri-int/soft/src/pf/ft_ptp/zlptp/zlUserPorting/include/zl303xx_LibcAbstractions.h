

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     The zl303xx API relies on various functions that need to be provided by
*     the 'host' operating system.  In order to make porting the API to a different
*     'host' easier to achieve an abstraction layer has been used to provide
*     the required functionality.
*
******************************************************************************/

#ifndef _ZL_LIBC_ABSTRACTIONS
#define _ZL_LIBC_ABSTRACTIONS

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/

/*****************   PROTOTYPES      ******************************************/

#if defined REPLACE_OS_PRINTF
#include <stdio.h>
int zl303xx_OsPrintf(const char *format, ...);
int zl303xx_OsFprintf(FILE* strm, const char *format, ...);
#endif

int zl303xx_OsAbs(int value);
void *zl303xx_OsMemcpy(void * destP, const void *sourceP, size_t size);
void *zl303xx_OsMemset(void *addrP, int value, size_t size);
int zl303xx_OsSprintf(char *destP, const char *formatP, ...);
int zl303xx_OsSnprintf(char *destP, int size, const char *formatP, ...);
size_t zl303xx_OsStrlen(const char *stringP);
char *zl303xx_OsStrncat(char *destP, const char *sourceP, size_t size);
int zl303xx_OsErrno(void);


#ifdef __cplusplus
}
#endif

#endif  /* MULTIPLE INCLUDE BARRIER */

