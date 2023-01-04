

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Functions to implement a circular log buffer in memory
*
*******************************************************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Macros.h"
#include "zl303xx_LogBuffer.h"

/************************ DEFINES  ************************************/
typedef struct zl303xx_LogBufferS {
    Uint32T start;          /* Offset to oldest log */
    Uint32T end;            /* Offset for next log entry (newest log ends at this offset) */
    Uint32T count;          /* Count number of bytes logged (saturates at size) */
    Uint32T size;           /* Max size of the buffer (in bytes) */
    OS_MUTEX_ID mutexId;    /* Mutex for parallel access */
    char *data;             /* Pointer to data buffer */
} zl303xx_LogBufferS;

/* Note: 2 GB (2147483647) is max size for Sint32T calculations. Can make smaller as desired. */
#define ZL303XX_LOG_BUFFER_MAX_SIZE 2147483647

/************************ STATIC VARS  ************************************/

/************************ FWD DECL  ************************************/
static void zl303xx_LogBufferEraseInternal(zl303xx_LogBufferT buffer, Uint32T len);

/*********************************************************************/

zlStatusE zl303xx_LogBufferIsValid(zl303xx_LogBufferT buffer)
{
    zlStatusE status = ZL303XX_OK;

    if (buffer == NULL || buffer->mutexId == OS_MUTEX_INVALID)
    {
        /* ZL303XX_CHECK_POINTER is not used here since it is expected to sometimes be null */
        status = ZL303XX_INIT_ERROR;
    }
    else if (buffer->size == 0)     /* Note: not accessed behind mutex for speed and low risk */
    {
        /* Return table empty such that any operation on the buffer is a noop but can be resized */
        status = ZL303XX_TABLE_EMPTY;
    }
    
    return status;
}

zlStatusE zl303xx_LogBufferCreate(zl303xx_LogBufferT *pBuffer, Uint32T size)
{
    zlStatusE status;
    zl303xx_LogBufferT buffer = NULL;
    
    status = ZL303XX_CHECK_POINTER(pBuffer);
    
    if (status == ZL303XX_OK)
    {
        status = zl303xx_LogBufferIsValid(*pBuffer);
        
        if (status == ZL303XX_INIT_ERROR)
        {
            status = ZL303XX_OK;
        }
        else
        {
            status = ZL303XX_MULTIPLE_INIT_ATTEMPT;
        }
    }
    
    if (status == ZL303XX_OK)
    {
        if (size > ZL303XX_LOG_BUFFER_MAX_SIZE)
        {
            status = ZL303XX_PARAMETER_INVALID;
        }
    }

    if (status == ZL303XX_OK)
    {
        *pBuffer = OS_CALLOC(sizeof(zl303xx_LogBufferS), 1);
        if (*pBuffer == NULL)
        {
            status = ZL303XX_RTOS_MEMORY_FAIL;
        }
    }
    
    if (status == ZL303XX_OK)
    {
        buffer = *pBuffer;
        
        buffer->mutexId = OS_MUTEX_CREATE();
        if (buffer->mutexId == OS_MUTEX_INVALID)
        {
            status = ZL303XX_RTOS_MUTEX_CREATE_FAIL;
        }
    }
    
    if (status == ZL303XX_OK)
    {
        
        buffer->start = 0;
        buffer->end = 0;
        buffer->count = 0;
        buffer->size = size;
        if (buffer->size == 0)
        {
            /* Empty buffer, don't calloc. */
            buffer->data = NULL;
        }
        else
        {
            buffer->data = OS_CALLOC(sizeof(char), size);
            if (buffer->data == NULL)
            {
                /* Memory allocation failed. Delete the object */
                OS_FREE(*pBuffer);
                *pBuffer = NULL;
                
                status = ZL303XX_RTOS_MEMORY_FAIL;
            }
        }
    }
    
    return status;
}

zlStatusE zl303xx_LogBufferDelete(zl303xx_LogBufferT *pBuffer)
{
    zlStatusE status;
    zl303xx_LogBufferT buffer = NULL;

    status = ZL303XX_CHECK_POINTER(pBuffer);
    
    if (status == ZL303XX_OK)
    {
        status = zl303xx_LogBufferIsValid(*pBuffer); 
        if (status == ZL303XX_TABLE_EMPTY)
        {
            status = ZL303XX_OK;
        }
    }        
    
    if (status == ZL303XX_OK)
    {
        buffer = *pBuffer;

        if (buffer->mutexId != OS_MUTEX_INVALID)
        {
            if (OS_MUTEX_TAKE(buffer->mutexId) == OS_OK)
            {
                (void) OS_MUTEX_DELETE(buffer->mutexId);    /* Ignore error */
            }
            
            buffer->mutexId = OS_MUTEX_INVALID;
        }
        
        if (buffer->data != NULL)
        {
            OS_FREE(buffer->data);
        }
        OS_FREE(buffer);
    }
    
    if (status == ZL303XX_OK)
    {
        *pBuffer = ZL303XX_LOG_BUFFER_INIT;
    }
    
    return status;
}

zlStatusE zl303xx_LogBufferResize(zl303xx_LogBufferT buffer, Uint32T size)
{
    zlStatusE status;
    char *oldData;
    zl303xx_BooleanE bMutexTaken = ZL303XX_FALSE;

    status = zl303xx_LogBufferIsValid(buffer);
    if (status == ZL303XX_TABLE_EMPTY)
    {
        /* Allow resize of empty buffer */
        status = ZL303XX_OK;
    }
  
    if (status == ZL303XX_OK)
    {
        if (size > ZL303XX_LOG_BUFFER_MAX_SIZE)
        {
            status = ZL303XX_PARAMETER_INVALID;
        }
    }
    
    if (status == ZL303XX_OK)
    {
        if (OS_MUTEX_TAKE(buffer->mutexId) != OS_OK)
        {
            status = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
        }
        else
        {
            bMutexTaken = ZL303XX_TRUE;
        }
    }
  
    if (status == ZL303XX_OK)
    {
        oldData = buffer->data;
        
        buffer->data = OS_CALLOC(size, sizeof(char));
        if (buffer->data == NULL)
        {
            /* Memory allocation issue, revert back to old data and size */
            buffer->data = oldData;
            
            status = ZL303XX_RTOS_MEMORY_FAIL;
        }
    }

    if (status == ZL303XX_OK)
    {
        if (oldData == NULL)
        {
            /* Empty buffer, no copy */
            buffer->size = size;
            buffer->start = 0;
            buffer->end = 0;
            buffer->count = 0;
        }
        else if (size >= buffer->size)
        {
            /* Growing. Copy data from the start to the end into offset 0 (everything fits) */
            if (buffer->start + buffer->count > buffer->size)
            {
                /* Two parts */
                Uint32T len1 = buffer->size - buffer->start;
                Uint32T len2 = buffer->count - len1;
                
                OS_MEMCPY(&buffer->data[0], &oldData[buffer->start], len1);
                OS_MEMCPY(&buffer->data[len1], &oldData[0], len2);
            }
            else
            {
                /* One part */
                OS_MEMCPY(&buffer->data[0], &oldData[buffer->start], buffer->count);
            }
            
            buffer->size = size;
            buffer->start = 0;
            buffer->end = buffer->count;
        }
        else
        {
            /* Shrinking. Copy enough data backwards from the end up to min(new size, current count) */
            Uint32T len = (buffer->count < size) ? buffer->count : size;

            if (buffer->end >= len)
            {
                /* One part */
                OS_MEMCPY(&buffer->data[0], &oldData[buffer->end - len], len);
            }
            else
            {
                /* Two parts */
                Uint32T len2 = buffer->end; /* Second part calculated first */
                Uint32T len1 = len - len2;

                OS_MEMCPY(&buffer->data[0],     &oldData[buffer->size - len1], len1);
                OS_MEMCPY(&buffer->data[len1],  &oldData[0], len2); 
            }
            
            buffer->size = size;
            buffer->count = len;
            buffer->start = 0;
            buffer->end = buffer->count;
        }
        
        if (oldData != NULL)
        {
            OS_FREE(oldData);
        }
    }
    
    if (bMutexTaken == ZL303XX_TRUE)
    {
        (void) OS_MUTEX_GIVE(buffer->mutexId);
    }
    
    return status;
}

zlStatusE zl303xx_LogBufferGetInfo(zl303xx_LogBufferT buffer, Uint32T *size, Uint32T *count)
{
    zlStatusE status;

    status = zl303xx_LogBufferIsValid(buffer);
    if (status == ZL303XX_TABLE_EMPTY)
    {
        status = ZL303XX_OK;
    }
    
    if (status == ZL303XX_OK)
    {
        if (OS_MUTEX_TAKE(buffer->mutexId) != OS_OK)
        {
            status = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
        }
    }
    
    if (status == ZL303XX_OK)
    {
        if (size != NULL)
        {
            *size = buffer->size;
        }
        
        if (count != NULL)
        {
            *count = buffer->count;
        }
        
        (void) OS_MUTEX_GIVE(buffer->mutexId);
    }
    
    return status;
}

zlStatusE zl303xx_LogBufferWrite(zl303xx_LogBufferT buffer, const char *buf, Uint32T len)
{
    zlStatusE status;
    Uint32T spaceToWrap;
    
    status = zl303xx_LogBufferIsValid(buffer);
    
    if (status == ZL303XX_OK)
    {
        if (OS_MUTEX_TAKE(buffer->mutexId) != OS_OK)
        {
            /* Mutex/buffer may be deleted */
            status = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
        }
    }
    
    if (status == ZL303XX_OK)
    {
        if (len > buffer->size)
        {
            len = buffer->size;
        }
        
        /* New writes go on the end of the buffer and may overwrite some of the start if full */
        
        /* Calc spaceToWrap remaining to wrap */
        spaceToWrap = buffer->size - buffer->end;
        if (buffer->size < buffer->end)
        {
            spaceToWrap = 0;  /* For safety, should never get here */
        }
        
        if (len >= spaceToWrap)
        {
            /* Too large to fit (or just enough). Split into two parts wrapping at the size boundary */
            Uint32T len1 = spaceToWrap;
            Uint32T len2 = len - spaceToWrap;
            
            OS_MEMCPY((void*)&buffer->data[buffer->end], &buf[0],    len1); /* First half */
            OS_MEMCPY((void*)&buffer->data[0],           &buf[len1], len2);    /* Second half wraps */
        }
        else
        {
            /* Enough space. Copy directly into buffer */
            OS_MEMCPY((void*)&buffer->data[buffer->end], buf, len);
        }
        
        buffer->end += len;
        if (buffer->end >= buffer->size)
        {
            buffer->end -= buffer->size;    /* Wrapped */
        }
            
        buffer->count += len;
        if (buffer->count >= buffer->size)
        {
            /* When full the start is the end */
            buffer->count = buffer->size;
            buffer->start = buffer->end;
        }
        
        (void) OS_MUTEX_GIVE(buffer->mutexId);
    }
    
    return status;
}

Uint32T zl303xx_LogBufferDump(zl303xx_LogBufferT buffer, void* strm, Sint32T len, zl303xx_BooleanE bErase)
{    
    zlStatusE status;
    Uint32T ret = 0;
    Uint32T offset;
    Uint32T ulen;
    
    status = zl303xx_LogBufferIsValid(buffer);
    
    if (status == ZL303XX_OK)
    {
        if (OS_MUTEX_TAKE(buffer->mutexId) != OS_OK)
        {
            /* Mutex/buffer may be deleted */
            status = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
        }
    }
    
    if (status == ZL303XX_OK)
    {
        if (len >= 0)
        {
            /* Dump from the oldest entry (start) */
            ulen = (Uint32T) len;
            if (ulen == 0 || ulen > buffer->count)
            {
                ulen = buffer->count;  /* Whole buffer */
            }
            
            offset = buffer->start;
        }
        else
        {
            /* Dump from the newest entry (end) */
            ulen = (Uint32T) -len;
            if (ulen == 0 || ulen > buffer->count)
            {
                ulen = buffer->count;  /* Whole buffer */
            }

            /* Rewind from the end */
            if (ulen <= buffer->end)
            {
                offset = buffer->end - ulen;
            }
            else
            {
                /* Rewind wraps */
                offset = buffer->size - (ulen - buffer->end);
            }
        }

        /* Dump buffer starting from oldest entry (start) */    
        if ((offset + ulen) > buffer->size)
        {
            /* Two parts */
            Uint32T len1 = buffer->size - offset;
            Uint32T len2 = ulen - len1;
            
            ret =  fwrite((void*)&buffer->data[offset], sizeof(char), len1, (FILE*)strm);
            ret += fwrite((void*)&buffer->data[0],      sizeof(char), len2, (FILE*)strm);
        }
        else
        {
            /* One part */
            ret = fwrite((void*)&buffer->data[offset], sizeof(char), ulen, (FILE*)strm);
        }
        
        if (bErase == ZL303XX_TRUE)
        {
            /* Erase from the oldest */
            zl303xx_LogBufferEraseInternal(buffer, ulen);
        }
        
        (void) OS_MUTEX_GIVE(buffer->mutexId);
    }
    
    return ret;
}

Uint32T zl303xx_LogBufferDumpToFile(zl303xx_LogBufferT buffer, const char* filename, Sint32T len, zl303xx_BooleanE bErase)
{
    Sint32T ret = 0;
    FILE *fp;
    
    if (zl303xx_LogBufferIsValid(buffer) == ZL303XX_OK)
    {
        fp = fopen(filename, "a");
        
        if (fp != NULL)
        {
            ret = zl303xx_LogBufferDump(buffer, fp, len, bErase);
            fclose(fp);
        }
    }
    
    return ret;
}

zlStatusE zl303xx_LogBufferErase(zl303xx_LogBufferT buffer, Uint32T len)
{
    zlStatusE status;

    status = zl303xx_LogBufferIsValid(buffer);

    if (status == ZL303XX_OK)
    {
        if (OS_MUTEX_TAKE(buffer->mutexId) == OS_OK)
        {
            zl303xx_LogBufferEraseInternal(buffer, len);

            (void) OS_MUTEX_GIVE(buffer->mutexId);
        }
        else 
        {
            status = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
        }
    }
    
    return status;
}

void zl303xx_LogBufferEraseInternal(zl303xx_LogBufferT buffer, Uint32T len)
{    
    if (len == 0 || len > buffer->count)
    {
        len = buffer->count;  /* Clear whole buffer */
    }

    /* Erase from oldest entry (start) and adjust count. Note: Protected from underflow above */
    buffer->count -= len;
    buffer->start += len;
    if (buffer->start >= buffer->size)
    {
        buffer->start -= buffer->size; /* Wrap around */
    }
    
    return;
}

