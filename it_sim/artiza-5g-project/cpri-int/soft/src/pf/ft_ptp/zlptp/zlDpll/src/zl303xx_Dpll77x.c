



/*******************************************************************************
* 
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Wrappers for various ZL3077x DPLL registers and bindings for APR.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Int64.h"
#include "zl303xx_Error.h"
#include "zl303xx_AddressMap77x.h"
#include "zl303xx_DeviceSpec.h"
#include "zl303xx_RdWr.h"
#include "zl303xx_Macros.h"
#include "zl303xx_HWTimer.h"
#include "zl303xx_Dpll771.h"
#include "zl303xx_Dpll77x.h"
#include "zl303xx.h"

#include "BPF_COM_PLOG.h"

/*****************   DEFINES     **********************************************/

#if !defined TEN_e9
  #define TEN_e9 1000000000
#endif

#define ZL303XX_MAX_NUM_77X_PARAMS (8)

/*****************   TYPES     **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

static Sint32T zl303xx_Dpll77xAccuStepTimePhase1588HostRegUpdate(void *hwParams, Sint32T deltaTime);

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   IMPORTED GLOBAL VARIABLES   ******************************/

/* Count of how many zl303xx_Params have been initialised. If this count is
   greater than 0, then ZLS3077X_DPLL_TASK is running. When this count
   goes from 1 to 0, ZLS3077X_DPLL_TASK is stopped. */
Uint32T zl303xx_Dpll77xParamsCount = 0;

/* Short sanity timeout when accessing registers that should return quickly */
Uint32T Dpll77xShortSanityTO = 100;
/* Medium sanity timeout when accessing registers */
Uint32T Dpll77xMediumSanityTO = 1000;
/* Long sanity timeout when accessing registers */
Uint32T Dpll77xLongSanityTO = 3000;

zl303xx_ParamsS *zl303xx_Params77x[ZL303XX_MAX_NUM_77X_PARAMS] = {0};

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/


/* zl303xx_Dpll77xParamsMutexInit */
/** 

    Initialize 77x mutex.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xParamsMutexInit(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xParamsMutexInit: ", 0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(zl303xx_Params);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params->pllParams.d77x.mailboxMutex = OS_MUTEX_CREATE();
        if(zl303xx_Params->pllParams.d77x.mailboxMutex == OS_MUTEX_INVALID)
        {
            status = ZL303XX_RTOS_MUTEX_CREATE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xParamsMutexInit: OS_MUTEX_CREATE failed. status: %d",
                    status,0,0,0,0,0);
        }
    }

    return status;
}


/* zl303xx_Dpll77xParamsMutexDelete */
/** 

    Delete 77x mutex.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xParamsMutexDelete(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xParamsMutexDelete: ", 0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(zl303xx_Params);
    }

    if( status == ZL303XX_OK )
    {
        if (zl303xx_Params->pllParams.d77x.mailboxMutex != OS_MUTEX_INVALID)
        {
            if (OS_MUTEX_TAKE(zl303xx_Params->pllParams.d77x.mailboxMutex) == OS_OK &&
                OS_MUTEX_DELETE(zl303xx_Params->pllParams.d77x.mailboxMutex) == OS_OK)
            {
                zl303xx_Params->pllParams.d77x.mailboxMutex = OS_MUTEX_INVALID;
            }
            else
            {
                status = ZL303XX_RTOS_MUTEX_DELETE_FAIL;
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xParamsMutexDelete: OS_MUTEX_DELETE failed. status: %d",
                        status,0,0,0,0,0);
            }
        }
    }

    return status;
}


/* zl303xx_Dpll77xParamsInit */
/** 

    Initialize APR parameters in the 77x zl303xx_ParamsS structure.

    These are values that APR manipulates. Values that are manipulated by
    the driver should be initialised in example77xClockCreateStructInit().

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xParamsInit(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xParamsInit: ", 0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(zl303xx_Params);
    }

    if (status == ZL303XX_OK)
    {
        zl303xx_Params->pllParams.d77x.restoreRegValues = ZL303XX_FALSE;
        zl303xx_Params->pllParams.d77x.prev_ZLS3077X_DPLL_CTRL_x_REG = 0;
        zl303xx_Params->pllParams.d77x.prev_ZLS3077X_DPLLX_TIE_REG = 0;

        zl303xx_Params->pllParams.mitigationEnabled = ZL303XX_FALSE;
    }

    if (status == ZL303XX_OK)
    {
        if( zl303xx_Params->pllParams.d77x.drvInitialized == ZL303XX_FALSE )
        {
            if( zl303xx_Dpll77xParamsCount == 0 )
            {
                /* Start ZLS3077X_DPLL_TASK */
                status = zl303xx_Dpll77xTaskStart();
                if (status != ZL303XX_OK)
                {
                    ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xParamsInit: Failed starting 77x task=%d", status, 0,0,0,0,0);
                }
            }
            if (status == ZL303XX_OK)
            {
                zl303xx_Params->pllParams.d77x.drvInitialized = ZL303XX_TRUE;
                zl303xx_Dpll77xParamsCount++;
            }
        }
    }

    return status;
}


/* zl303xx_Dpll77xParamsClose */
/** 

    Cleanup the 77x zl303xx_ParamsS structure.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xParamsClose(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xParamsClose: ", 0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(zl303xx_Params);
    }

    if (status == ZL303XX_OK)
    {
        zl303xx_Params->pllParams.d77x.drvInitialized = ZL303XX_FALSE;

        if( zl303xx_Dpll77xParamsCount != 0 )
        {
            zl303xx_Dpll77xParamsCount--;
        }

        if( zl303xx_Dpll77xParamsCount == 0 )
        {
            status = zl303xx_Dpll77xTaskStop();
            if (status != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xParamsClose: Failed stopping 77x task=%d", status, 0,0,0,0,0);
            }
        }
    }

    return status;
}


/* zl303xx_Dpll77xSetupMailBoxRegisters */
/**
   Returns the control and semaphore registers used for a given mailbox.

  Parameters:
   [in]   mbId       The mailbox: ref, DPLL, or syth

   [out]  ctrlReg    The control register of mbId
   [out]  semReg     The semaphore register of mbId

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
static zlStatusE zl303xx_Dpll77xSetupMailBoxRegisters(ZLS3077X_MailboxE mbId,
                                Uint32T *ctrlReg, Uint32T *semReg)
{
    zlStatusE status = ZL303XX_OK;

    /* No need to check the mailbox ID here since the SWITCH below will
     * catch any errors.   */
    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xSetupMailBoxRegisters: mbId=%d ",
            mbId,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(ctrlReg);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(semReg);
    }
    if( status == ZL303XX_OK )
    {
        switch( mbId )
        {
            case ZLS3077X_MB_ref:
                *ctrlReg = ZLS3077X_REF_CTRL_REG;
                *semReg = ZLS3077X_REF_SEMAPHORE_REG;
                break;
            case ZLS3077X_MB_DPLL:
                *ctrlReg = ZLS3077X_DPLL_CTRL_REG;
                *semReg = ZLS3077X_DPLL_SEMAPHORE_REG;
                break;
            case ZLS3077X_MB_last:
            default:
                status = ZL303XX_PARAMETER_INVALID;
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xSetupMailBoxRegisters: bad mbId",
                        0, 0,0,0,0,0);
                break;
        }
    }

    if( status == ZL303XX_OK )
    {
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                "zl303xx_Dpll77xSetupMailBoxRegisters: *ctrlReg=%d  *semReg=%d ",
                *ctrlReg, *semReg, 0,0,0,0);
    }

    return status;
}


/* zl303xx_Dpll77xGetMailBoxSemaphoreStatus */
/**
   Returns semaphore status: either busy or available.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure
   [in]   mbId      The mailbox: ref, DPLL, or syth

   [out]  semStatus The status: either busy or available

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xGetMailBoxSemaphoreStatus(void *hwParams,
                ZLS3077X_MailboxE mbId, ZLS3077X_SemStatusE *semStatus)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T ctrlReg;
    Uint32T semReg;
    Uint32T readWriteSemStatus;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xGetMailBoxSemaphoreStatus: mbId=%d ",
            mbId,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_MAILBOX_ID(mbId);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(semStatus);
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetupMailBoxRegisters(mbId, &ctrlReg, &semReg);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, semReg, &readWriteSemStatus);
    }
    if (status == ZL303XX_OK)
    {
        if( readWriteSemStatus == 0 )
        {
            *semStatus = ZLS3077X_SemStatus_available;
        }
        else
        {
            *semStatus = ZLS3077X_SemStatus_busy;
        }
    }
    if( status == ZL303XX_OK )
    {
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                "zl303xx_Dpll77xGetMailBoxSemaphoreStatus: *semStatus=%d",
                *semStatus, 0, 0,0,0,0);
    }

    return status;
}


/* zl303xx_Dpll77xWaitForAvailableMailBoxSemaphore */
/**
   Blocks until the given mailbox is available.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure
   [in]   mbId      The mailbox: ref, DPLL, or syth

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xWaitForAvailableMailBoxSemaphore(void *hwParams,
                                        ZLS3077X_MailboxE mbId)
{
    zlStatusE status = ZL303XX_OK;
    ZLS3077X_SemStatusE semStatus;
    Sint32T sanityLoopCount;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xWaitForAvailableMailBoxSemaphore: mbId=%d ",
            mbId,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_MAILBOX_ID(mbId);
    }

    if (status == ZL303XX_OK)
    {
        sanityLoopCount = ZLS3077X_MAX_MAILBOX_SEMAPHORE_WAIT / ZLS3077X_MAILBOX_SEMAPHORE_CHECK_INTERVAL;

        while( sanityLoopCount > 0 )
        {
            status = zl303xx_Dpll77xGetMailBoxSemaphoreStatus(hwParams, mbId, &semStatus);

            if( status != ZL303XX_OK )
            {
                /* Error reading the semaphore status; exit the loop */
                break;
            }
            if( semStatus == ZLS3077X_SemStatus_available )
            {
                /* Semaphore is avaiable; exit the loop */
                break;
            }

            OS_TASK_DELAY( ZLS3077X_MAILBOX_SEMAPHORE_CHECK_INTERVAL );

            sanityLoopCount--;
        }

        if( sanityLoopCount == 0 )
        {
            status = ZL303XX_TIMEOUT;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xWaitForAvailableMailBoxSemaphore: timeout: %d",
                    mbId, 0,0,0,0,0);
        }
    }

    return status;
}


/* zl303xx_Dpll77xSetupMailboxForRead */
/*
    Sets up the given mailbox for reading.

    This routine:
    1) waits for the mailbox to become available
    2) write the mbNum into the control register
    3) sets the semaphore read bit so that the 77x device loads the mailbox
       with data
    4) waits for the 77x to load the mailbox with data

    After calling this routine, the caller can look at the data in the mailbox.

  Parameters:
   [in]  hwParams        Pointer to the device instance parameter structure.
   [in]  mbId            The mailbox ID: ref or dpll
   [in]  mbNum           The mailbox number: ref 0-9 or dpll 0-6

  Return Value:     zlStatusE

*******************************************************************************/
zlStatusE zl303xx_Dpll77xSetupMailboxForRead(void *hwParams, ZLS3077X_MailboxE mbId,
                                        Uint32T mbNum)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T ctrlReg;
    Uint32T semReg;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xSetupMailboxForRead: mbId=%d  mbNum=%d ",
            mbId, mbNum, 0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_MAILBOX_ID(mbId);
    }

    /* Wait for the mailbox to become available */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xWaitForAvailableMailBoxSemaphore(hwParams, mbId);
        if( status != ZL303XX_OK )
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xSetupMailboxForRead: wait (1) failed: %d  %d",
                    mbId, mbNum,0,0,0,0);
        }
    }
    /* Setup registers */
    if( status == ZL303XX_OK )
    {
        status = zl303xx_Dpll77xSetupMailBoxRegisters(mbId, &ctrlReg, &semReg);
    }
    /* Set the mailbox number */
    if( status == ZL303XX_OK )
    {
        status = zl303xx_Write(zl303xx_Params, NULL, ctrlReg, 1 << mbNum);
        if( status != ZL303XX_OK )
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xSetupMailboxForRead: write (1) failed. status: %d  %d  %d  %x",
                    status, mbId, mbNum, ctrlReg, 0,0);
        }
    }
    /* Populate the mailbox */
    if( status == ZL303XX_OK )
    {
        status = zl303xx_Write(zl303xx_Params, NULL, semReg, ZL303XX_MB_SEM_READ);
        if( status != ZL303XX_OK )
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xSetupMailboxForRead: write (2) failed. status: %d  %d  %d  %x",
                    status, mbId, mbNum, semReg, 0,0);
        }
    }
    /* Wait for the mailbox to become populated */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xWaitForAvailableMailBoxSemaphore(hwParams, mbId);
        if( status != ZL303XX_OK )
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xSetupMailboxForRead: wait (2) failed: %d  %d",
                    mbId, mbNum,0,0,0,0);
        }
    }

    return status;
}


/* zl303xx_Dpll77xSetupMailboxForWrite */
/*
    Sets up the given mailbox for writing.

    This routine:
    1) waits for the mailbox to become available
    2) write the mbNum into the control register
    3) sets the semaphore write bit so that the 77x device loads the mailbox
       with data
    4) waits for the 77x to load the mailbox with data

    It is then up to the calling routine to write register data into the
    mailbox and call zl303xx_Dpll77xSetMailboxSemWrite() to apply those data.

  Parameters:
   [in]  hwParams        Pointer to the device instance parameter structure.
   [in]  mbId            The mailbox ID: ref or dpll
   [in]  mbNum           The mailbox number: ref 0-9 or dpll 0-6

  Return Value:     zlStatusE

*******************************************************************************/
zlStatusE zl303xx_Dpll77xSetupMailboxForWrite(void *hwParams, ZLS3077X_MailboxE mbId,
                                    Uint32T mbNum)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T ctrlReg;
    Uint32T semReg;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xSetupMailboxForWrite: mbId=%d  mbNum=%d ",
            mbId, mbNum, 0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_MAILBOX_ID(mbId);
    }

    /* Wait for the mailbox to become available */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xWaitForAvailableMailBoxSemaphore(hwParams, mbId);
        if( status != ZL303XX_OK )
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xSetupMailboxForWrite: wait (1) failed: %d  %d",
                    mbId, mbNum,0,0,0,0);
        }
    }
    /* Setup registers */
    if( status == ZL303XX_OK )
    {
        status = zl303xx_Dpll77xSetupMailBoxRegisters(mbId, &ctrlReg, &semReg);
    }
    /* Set the mailbox number */
    if( status == ZL303XX_OK )
    {
        status = zl303xx_Write(zl303xx_Params, NULL, ctrlReg, 1 << mbNum);
        if( status != ZL303XX_OK )
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xSetupMailboxForWrite: write (1) failed. status: %d  %d  %d  %x",
                    status, mbId, mbNum, ctrlReg, 0,0);
        }
    }
    /* Populate the the mailbox */
    if( status == ZL303XX_OK )
    {
        status = zl303xx_Write(zl303xx_Params, NULL, semReg, ZL303XX_MB_SEM_READ);
        if( status != ZL303XX_OK )
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xSetupMailboxForWrite: write (2) failed. status: %d  %d  %d  %x",
                    status, mbId, mbNum, semReg, 0,0);
        }
    }
    /* Wait for the mailbox to become populated */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xWaitForAvailableMailBoxSemaphore(hwParams, mbId);
        if( status != ZL303XX_OK )
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xSetupMailboxForWrite: wait (2) failed: %d  %d",
                    mbId, mbNum,0,0,0,0);
        }
    }

    return status;
}


/* zl303xx_Dpll77xSetMailboxSemWrite */
/*
    Set the mailbox semaphore write bit. This cause the 77x device to apply the
    current mailbox data.

  Parameters:
   [in]  hwParams        Pointer to the device instance parameter structure.
   [in]  mbId            The mailbox ID: ref or dpll

  Return Value:     zlStatusE

*******************************************************************************/
zlStatusE zl303xx_Dpll77xSetMailboxSemWrite(void *hwParams, ZLS3077X_MailboxE mbId)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T ctrlReg;
    Uint32T semReg;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xSetMailboxSemWrite: mbId=%d ",
            mbId, 0, 0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_MAILBOX_ID(mbId);
    }

    /* Setup registers */
    if( status == ZL303XX_OK )
    {
        status = zl303xx_Dpll77xSetupMailBoxRegisters(mbId, &ctrlReg, &semReg);
    }
    /* Set the mailbox semaphore write bit */
    if( status == ZL303XX_OK )
    {
        status = zl303xx_Write(zl303xx_Params, NULL, semReg, ZL303XX_MB_SEM_WRITE);
        if( status != ZL303XX_OK )
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xSetMailboxSemWrite: write failed: %d  %d  %d",
                    status, mbId, semReg, 0,0,0);
        }
    }

    return status;
}


/* zl303xx_Dpll77xUpdateMailboxCopy */
/*
    Reads a single mailbox and stores its data in software.

    Each time a mailbox register value is changed in the device, this routine
    should be called to update the software copy.

    Mailbox data should not need to be modified often. Therfor, This routine
    should not need to be called often.

  Parameters:
   [in]  hwParams        Pointer to the device instance parameter structure.
   [in]  mbId            The mailbox ID: ref or dpll
   [in]  mbNum           The mailbox number

  Return Value:     zlStatusE

*******************************************************************************/
zlStatusE zl303xx_Dpll77xUpdateMailboxCopy(void *hwParams, ZLS3077X_MailboxE mbId,
                                Uint32T mbNum)
{
    zlStatusE status = ZL303XX_OK;
    osStatusT osStatus = OS_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    zl303xx_BooleanE mutexTaken = ZL303XX_FALSE;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xUpdateMailboxCopy: mbId=%d  mbNum=%d ",
            mbId, mbNum, 0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_MAILBOX_ID(mbId);
    }
    if (status == ZL303XX_OK)
    {
        osStatus = OS_MUTEX_TAKE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xUpdateMailboxCopy: OS_MUTEX_TAKE failed: %d  %d  %d",
                    status, mbId, mbNum, 0,0,0);
        }
    }
    if (status == ZL303XX_OK)
    {
        mutexTaken = ZL303XX_TRUE;
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetupMailboxForRead(hwParams, mbId, mbNum);
    }
    if (status == ZL303XX_OK)
    {
        switch( mbId )
        {
            case ZLS3077X_MB_ref:
                {
                    zl303xx77xRefMailBoxS *p = &(zl303xx_Params->pllParams.d77x.refMb[mbNum]);

                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_FREQ_BASE_REG,        &(p->ref_freq_base));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_FREQ_MULT_REG,        &(p->ref_freq_mult));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_RATIO_M_REG,          &(p->ref_ratio_m));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_RATIO_N_REG,          &(p->ref_ratio_n));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_CONFIG_REG,           &(p->ref_config));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_SCM_REG,              &(p->ref_scm));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_CFM_REG,              &(p->ref_cfm));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_GST_DISQUAL_REG,      &(p->ref_gst_disqual));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_GST_QUAL_REG,         &(p->ref_gst_qual));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_PFM_CTRL_REG,         &(p->ref_pfm_ctrl));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_PFM_DISQUALIFY_REG,   &(p->ref_pfm_disqualify));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_PFM_QUALIFY_REG,      &(p->ref_pfm_qualify));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_PFM_PERIOD_REG,       &(p->ref_pfm_period));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_PFM_FILTER_LIMIT_REG, &(p->ref_pfm_filter_limit));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_SYNC_REG,             &(p->ref_sync));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_SYNC_MISC_REG,        &(p->ref_sync_misc));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_SYNC_OFFSET_COMP_REG, &(p->ref_sync_offset_comp));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_PHASE_OFFSET_COMP_REG,&(p->ref_phase_offset_compensation));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_REFX_SCM_FINE_REG,         &(p->ref_scm_fine));
                }
                break;

            case ZLS3077X_MB_DPLL:
                {
                    zl303xx77xDpllMailboxS *p = &(zl303xx_Params->pllParams.d77x.dpllMb[mbNum]);

                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_BW_FIXED_REG,              &(p->dpll_bw_fixed));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_BW_VAR_REG,                &(p->dpll_bw_var));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_CONFIG_REG,                &(p->dpll_config));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_PSL_REG,                   &(p->dpll_psl));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_PSL_MAX_PHASE_REG,         &(p->dpll_psl_max_phase));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_PSL_SCALING_REG,           &(p->dpll_psl_scaling));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_PSL_DECAY_REG,             &(p->dpll_psl_decay));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_RANGE_REG,                 &(p->dpll_range));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_SW_MASK_REG,           &(p->dpll_ref_sw_mask));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_HO_MASK_REG,           &(p->dpll_ref_ho_mask));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_DPER_SW_MASK_REG,          &(p->dpll_dper_sw_mask));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_DPER_HO_MASK_REG,          &(p->dpll_dper_ho_mask));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_0_REG,            &(p->dpll_ref_prio_0));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_1_REG,            &(p->dpll_ref_prio_1));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_2_REG,            &(p->dpll_ref_prio_2));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_3_REG,            &(p->dpll_ref_prio_3));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_REF_PRIO_4_REG,            &(p->dpll_ref_prio_4));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_DPER_PRIO_1_0_REG,         &(p->dpll_dper_prio_1_0));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_DPER_PRIO_3_2_REG,         &(p->dpll_dper_prio_3_2));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_HO_FILTER_REG,             &(p->dpll_ho_filter));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_HO_DELAY_REG,              &(p->dpll_ho_delay));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_SPLIT_XO_CONFIG_REG,       &(p->dpll_split_xo_config));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FAST_LOCK_CTRL_REG,        &(p->dpll_fast_lock_ctrl));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FAST_LOCK_PHASE_ERR_REG,   &(p->dpll_fast_lock_phase_err));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FAST_LOCK_FREQ_ERR_REG,    &(p->dpll_fast_lock_freq_err));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FAST_LOCK_IDEAL_TIME_REG,  &(p->dpll_fast_lock_ideal_time));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FAST_LOCK_NOTIFY_TIME_REG, &(p->dpll_fast_lock_notify_time));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FAST_LOCK_FCL_REG,         &(p->dpll_fast_lock_fcl));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FCL_REG,                   &(p->dpll_fcl));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_DAMPING_REG,               &(p->dpll_damping));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_PHASE_BAD_REG,             &(p->dpll_phase_bad));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_PHASE_GOOD_REG,            &(p->dpll_phase_good));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_GOOD_DURATION_REG,         &(p->dpll_duration_good));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_LOCK_DELAY_REG,            &(p->dpll_lock_delay));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_TIE_REG,                   &(p->dpll_tie));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_TIE_THRESH_REG,            &(p->dpll_tie_wr_thresh));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FP_FIRST_REALIGN_REG,      &(p->dpll_fp_first_realign));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FP_REALIGN_INTVL_REG,      &(p->dpll_fp_align_intvl));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_FP_LOCK_THRESH_REG,        &(p->dpll_fp_lock_thresh));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_STEP_TIME_THRESH_REG,      &(p->dpll_step_time_thresh));
                    status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DPLLX_STEP_TIME_RESO_REG,        &(p->dpll_step_time_reso));
                }
                break;

            case ZLS3077X_MB_last:
            default:
                status = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xUpdateMailboxCopy: bad mbId: %d  (mbNm: %d)",
                        status, mbId, mbNum, 0,0,0);
                break;
        }
    }

    if( mutexTaken == ZL303XX_TRUE )
    {
        osStatus = OS_MUTEX_GIVE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_GIVE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xUpdateMailboxCopy: OS_MUTEX_GIVE failed: %d  %d  %d",
                    status, mbId, mbNum, 0,0,0);
        }
    }

    return status;
}


/* zl303xx_Dpll77xUpdateAllMailboxCopies */
/*
    Reads all the mailboxes and stores their data in zl303xx_ParamsS.

  Parameters:
   [in]  hwParams        Pointer to the device instance parameter structure.

  Return Value:     zlStatusE

*******************************************************************************/
zlStatusE zl303xx_Dpll77xUpdateAllMailboxCopies(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T idx;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xUpdateAllMailboxCopies: ", 0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }

    if (status == ZL303XX_OK)
    {
        for( idx = ZLS3077X_RMBN_0; idx < ZLS3077X_RMBN_last; idx++ )
        {
            status |= zl303xx_Dpll77xUpdateMailboxCopy(hwParams, ZLS3077X_MB_ref, idx);
        }
    }
    if (status == ZL303XX_OK)
    {
        for( idx = ZLS3077X_DMBN_0; idx < ZLS3077X_DMBN_last; idx++ )
        {
            status |= zl303xx_Dpll77xUpdateMailboxCopy(hwParams, ZLS3077X_MB_DPLL, idx);
        }
    }

    return status;
}


/* zl303xx_Ref77xMonBitsClear */
/**
    If the reference id is valid, then this routine clears the reference
    monitor sticky bits for the give reference id.

    If the reference id is not valid, then this routine clears the reference
    monitor sticky bits for ALL references.

    The sticky bits must be locked prior to calling this function.

  Parameters:
   [in]   hwParams  Device instance parameter structure.
   [in]   refId     Reference bits to clear. If this is out of range,
                         all reference bits will be cleared.

*******************************************************************************/
zlStatusE zl303xx_Ref77xMonBitsClear(void *hwParams, Uint32T refId)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T start;
    Uint32T end;
    Uint32T count;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Ref77xMonBitsClear: refID=%d", refId, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        if (refId < ZLS3077X_REF_MAX)
        {
            start = refId;
            end = refId;
        }
        else
        {
            start = 0;
            end = ZLS3077X_REF_MAX - 1;
        }

        for (count = start; count <= end && status == ZL303XX_OK; count++)
        {
            status = zl303xx_Write(zl303xx_Params, NULL,
                                ZLS3077X_REF_MON_TH_STICKY_REG(count),
                                0);
            if( status != ZL303XX_OK )
            {
               break;
            }
            status = zl303xx_Write(zl303xx_Params, NULL,
                                ZLS3077X_REF_MON_TL_STICKY_REG(count),
                                0);
            if( status != ZL303XX_OK )
            {
               break;
            }
        }
    }

    return status;
}


/* zl303xx_Dpll77xStickyReadMon */
/** 
   Helper function to read the reference monitor register.

   We read:
   1) the status register
   2) the sticky high transition register
   3) the sticky lo transition register

   If any of the register bits is 1, then the returned value has that bit
   set to 1.

  Parameters:
   [in]   hwParams      Pointer to a device parameter structure.
   [in]   refId         Which reference.
   [out]  data          Pointer to returned data.

  Notes:  Assumes input parameters are valid.

*******************************************************************************/
static Sint32T zl303xx_Dpll77xStickyReadMon(void *hwParams, Uint32T refId,
                                            Uint32T *data)
{
    zlStatusE status = ZL303XX_OK;
    zlStatusE stickyStatus = ZL303XX_ERROR;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T refStatus;
    Uint32T refTh;
    Uint32T refTl;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xStickyReadMon: refId=%d",
            refId, 0, 0, 0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(data);
    }
    if (status == ZL303XX_OK)
    {
        /* Read the status bits */
        status = zl303xx_Read(zl303xx_Params, NULL,
                            ZLS3077X_REF_MON_FAIL_REG(refId),
                            &refStatus);
    }

#if defined ZLS30771_WAITING_FOR_STICKY_BITS
    /* We will get the "current" value in 25ms */
#else
    if (status == ZL303XX_OK)
    {
        /* Read the previous transition-high sticky bits */
        status = zl303xx_Read(zl303xx_Params, NULL,
                            ZLS3077X_REF_MON_TH_STICKY_REG(refId),
                            &refTh);
    }
    if (status == ZL303XX_OK)
    {
        /* Read the previous transition-low sticky bits */
        status = zl303xx_Read(zl303xx_Params, NULL,
                            ZLS3077X_REF_MON_TL_STICKY_REG(refId),
                            &refTl);
    }
    if (status == ZL303XX_OK)
    {
        /* If the current state is 1 and either of the sticky bits are 1, then
           we should return 1 - a failure. */
        *data = refStatus | refTh | refTl;
    }
    else
    {
        *data = 0;
        ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xStickyReadMon: Sticky read of refId=%d failed=%d",
                refId, status, 0,0,0,0);
    }
#endif

    if (status == ZL303XX_OK)
    {
        /* Lock all sticky registers */
        stickyStatus = zl303xx_Dpll77xStickyLockSet(hwParams, ZLS3077X_STICKY_LOCK);
        status = stickyStatus;
    }
    if (status == ZL303XX_OK)
    {
        if (refId != (Uint32T)ZL303XX_INVALID)
        {
            /* Clear the sticky bits. */
            status = zl303xx_Ref77xMonBitsClear(hwParams, refId);
        }
    }
    /* Unlock the sticky_lock register */
    if (stickyStatus == ZL303XX_OK)
    {
        stickyStatus = zl303xx_Dpll77xStickyLockSet(hwParams, ZLS3077X_STICKY_UNLOCK);
        status = (status == ZL303XX_OK) ? stickyStatus : status;
    }

#if defined ZLS30771_WAITING_FOR_STICKY_BITS
    if (status == ZL303XX_OK)
    {
        OS_TASK_DELAY(ZLS3077X_STICKY_UPDATE_DELAY_MS);
    }
    if (status == ZL303XX_OK)
    {
        /* Read the new transition-high sticky bits */
        status = zl303xx_Read(zl303xx_Params, NULL,
                            ZLS3077X_REF_MON_TH_STICKY_REG(refId),
                            &refTh);
    }
    if (status == ZL303XX_OK)
    {
        /* Read the new transition-low sticky bits */
        status = zl303xx_Read(zl303xx_Params, NULL,
                            ZLS3077X_REF_MON_TL_STICKY_REG(refId),
                            &refTl);
    }
    if (status == ZL303XX_OK)
    {
        /* If the current state is 1 and either of the sticky bits are 1, then
           we should return 1 - a failure. */
        *data = refStatus | refTh | refTl;
    }
    else
    {
        *data = 0;
        ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xStickyReadMon: Real read of refId=%d failed=%d",
                refId, status, 0,0,0,0);
    }
#else
    /* DO NOT WAIT for 77x to update registers with new data - we will get
       them next time through */
#endif

    if (status == ZL303XX_OK)
    {
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                "zl303xx_Dpll77xStickyReadMon: *data=%d", *data, 0,0,0,0,0);
    }

    return (Sint32T)status;
}


/* zl303xx_Dpll77xStickyLockSet */
/**
   Locks or unlocks the sticky registers on a device.

  Parameters:
   [in]   hwParams      Pointer to a device parameter structure.
   [in]   lock          Lock or unlock the sticky registers.

  Return Value:  ZL303XX_OK  Success

*******************************************************************************/
zlStatusE zl303xx_Dpll77xStickyLockSet(void *hwParams,
                                       ZLS3077X_StickyLockE lock)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;


    /* This trace generates lots of output that is not very usefull.
       Comment it out for now. */
    /*
    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xStickyLockSet: ", 0,0,0,0,0,0);
    */

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_STICKY_LOCK(lock);
    }

    if (status == ZL303XX_OK && lock)
    {
        if (zl303xx_Params->pllParams.d77x.stickyLockCallout)
        {
            /* bound-in callback; call it to inform of lock taken */
            if ((status = zl303xx_Params->pllParams.d77x.stickyLockCallout(hwParams,
                            zl303xx_Params->pllParams.pllId, lock)) != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xStickyLockSet: User stickyLockCallout (Lock=%d) failed=%d",
                        lock, status, 0,0,0,0);
            }
        }
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_Write(zl303xx_Params, NULL, ZLS3077X_STICKY_LOCK_REG, lock);
    }

    if (status == ZL303XX_OK && !lock)
    {
        if (zl303xx_Params->pllParams.d77x.stickyLockCallout)
        {
            /* bound-in callback; call it to inform of lock given */
            if ((status = zl303xx_Params->pllParams.d77x.stickyLockCallout(hwParams,
                zl303xx_Params->pllParams.pllId, lock)) != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xStickyLockSet: User stickyLockCallout (Unlock=%d) failed=%d",
                        lock, status, 0,0,0,0);
            }
        }
    }

    return status;
}


/* zl303xx_Dpll77xSetFreq */
/**
    Sets the frequency offset while in NCO mode.

    Callback function for zl303xx_AprAddDeviceS::adjustFreq().

  Parameters:
   [in]   hwParams        Pointer to the device parameter structure.
   [in]   freqOffsetInppt Frequency offset to set in ppt.

*******************************************************************************/
Sint32T zl303xx_Dpll77xSetFreq(void *hwParams, Sint32T freqOffsetInppt)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;
    Uint32T dcoOffset;
    Uint64S reg;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xSetFreq: freqOffsetInppt=%d", freqOffsetInppt, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        Uint8T round;
        Uint32T absFreq;

        /* Only do unsigned math */
        absFreq = (freqOffsetInppt < 0) ? -freqOffsetInppt : freqOffsetInppt;

        /* Convert the frequency offset into the equivalent DCO offset register
        value. */
        dcoOffset = Mult_Mult_Div_U32(absFreq,
                                    ZLS3077X_DCO_COUNT_PER_PPM,
                                    ONE_MILLION, &round, NULL);
        dcoOffset += round;

        /* Need to negate the applied offset */
        if (freqOffsetInppt > 0)
        {
            reg.hi = 0xFFFF;   /* dpllx_df_offset is 6 bytes */
            reg.lo = -dcoOffset;
        }
        else
        {
            reg.hi = 0;
            reg.lo = dcoOffset;
        }

        /* Write the equivalent DCO offset to the device register (MSB byte 5) */
        status = zl303xx_Write(zl303xx_Params, NULL,
                           ZLS3077X_NCO_FREQ_OFFSET_WR_HI_DPLL(pllId),
                           reg.hi);

        /* Write the equivalent DCO offset to the device register */
        status |= zl303xx_Write(zl303xx_Params, NULL,
                           ZLS3077X_NCO_FREQ_OFFSET_WR_LO_DPLL(pllId),
                           reg.lo);
    }

    if (status == ZL303XX_OK)
    {
        /* Store the freq. provided */
        zl303xx_Params->pllParams.dcoFreq = freqOffsetInppt;
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                "zl303xx_Dpll77xSetFreq: DPLL=%d dcoOffset=0x%x", pllId, dcoOffset,
                0,0,0,0);
    }
    else
    {
        ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xSetFreq: Call to zl303xx_Write failed=%d",
                status, 0,0,0,0,0);
    }

    return (Sint32T)status;
}


/* zl303xx_Dpll77xGetNCOAssistHwLockStatus */
/**
 *  Only certain devices (starting with ZL3077X) will support this feature
 *  This function returns the lockstatus if a SYNCE dpll exists on this device

  Parameters:
   [in]   hwParams    Pointer to the device parameter structure.
   [out]  lockStatus  SYNCE DPLL lock status mapped to zl303xx_AprLockStatusE.

  Return Value:     ZL303XX_OK                Success or
            ZL303XX_INVALID_MODE      If deviceType has no dedicated pllId
*******************************************************************************/
Sint32T zl303xx_Dpll77xGetNCOAssistHwLockStatus(void *hwParams, Sint32T *lockStatus)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId = 0;
    Uint32T data = 0;
    Uint32T refId = (Uint32T)ZL303XX_INVALID;
    Uint32T refFailBits = 0;
    zl303xx_LockStatusE *lastLockStatus = NULL;

    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xGetNCOAssistHwLockStatus: ", 0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTERS(hwParams, lockStatus);
    }

    if (status == ZL303XX_OK)
    {
        ZLS3077X_DpllHWModeE dpllMode = ZLS3077X_DPLL_MODE_FREERUN;

        lastLockStatus = &zl303xx_Params->pllParams.lastLockStatus;
        pllId = zl303xx_Params->pllParams.pllId;

        status = ZLS3077X_CHECK_DPLLID(pllId);

        /* Confirm this device is in the correct mode (not freerun or nco) */
        if (status == ZL303XX_OK)
        {
            /* Get current ref and dpll mode */
            status = zl303xx_Read(zl303xx_Params, NULL,
                          ZLS3077X_DPLL_MODE_REFSEL_REG(pllId), &data);
            if (status != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetNCOAssistHwLockStatus: Read of dpllMode failed=%d!",
                        status, 0,0,0,0,0);
            }
            else
            {
                dpllMode = data & ZLS3077X_MODE_ONLY_MASK;
            }

            if (dpllMode == ZLS3077X_DPLL_MODE_FREERUN ||
                dpllMode == ZLS3077X_DPLL_MODE_NCO)
            {
                status = ZL303XX_INVALID_MODE;
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetNCOAssistHwLockStatus: NCOAssistHw device in the wrong mode=%d; return failure=%d!",
                                   data, status, 0,0,0,0);
            }

            if ((status == ZL303XX_OK) &&
            ((dpllMode == ZLS3077X_DPLL_MODE_REFLOCK) ||
                (dpllMode == ZLS3077X_DPLL_MODE_AUTO_LOCK)))
            {
                /* Detect refId */
                status = zl303xx_Dpll77xCurrRefGet(zl303xx_Params, &refId);

                if (status != ZL303XX_OK)
                {
                    ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetNCOAssistHwLockStatus: Call to zl303xx_Dpll77xCurrRefGet() failed=%d !!",
                            status, 0,0,0,0,0);
                }
            }
        }

        /* For proper sticky status register monitoring:
        * a) set lockstatus UNKNOWN
        * b) if we are not waiting for a sticky bit update, read the prior value
        * c) write non-zero value (e.g. 0x1) into Sticky Bit Lock register
        * d) clear sticky bits (one or more)
        * e) write 0 into Sticky Bit Lock register
        * f) For current values, we wait for 77x to update sticky status register(s)
             (25ms) - with APR we skip this as we know we will be back soon
        * g) if we waited then read current sticky status register(s) value(s)
        * h) Check lock and holdover bits
        */

        /* Check the sticky registers to determine the lockStatus */
        if (status == ZL303XX_OK)
        {
            zlStatusE stickyStatus = ZL303XX_ERROR;
            Uint32T dpllStatus;
            Uint32T dpllTh = 0, dpllTl = 0;

            *lockStatus = ZL303XX_LOCK_STATUS_UNKNOWN;

        #if !defined ZLS30771_WAITING_FOR_STICKY_BITS
            /* Read the PRIOR transition-high sticky bits */
            status = zl303xx_Read(zl303xx_Params, NULL,
                                ZLS3077X_HOLDOVER_LOCK_TH_STICKY_REG(pllId),
                                &dpllTh);
            if (status == ZL303XX_OK)
            {
                /* Read the PRIOR transition-low sticky bits */
                status = zl303xx_Read(zl303xx_Params, NULL,
                                    ZLS3077X_HOLDOVER_LOCK_TL_STICKY_REG(pllId),
                                    &dpllTl);
            }
        #endif

            /* Indicate we will Lock all sticky registers */
            if (status == ZL303XX_OK)
            {
                stickyStatus = zl303xx_Dpll77xStickyLockSet(zl303xx_Params,
                                                        ZLS3077X_STICKY_LOCK);
                status = stickyStatus;
            }

            /* Clear transition-high and -low lock and holdover sticky bits */
            if (status == ZL303XX_OK)
            {
                /* This clears both lock and holdover in the transition-high reg. */
                status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                                         ZLS3077X_HOLDOVER_LOCK_TH_STICKY_REG(pllId),
                                         0x00,
                                         ZLS3077X_HOLDOVER_LOCK_STATUS_MASK_DPLL,
                                         0);
            }
            if (status == ZL303XX_OK)
            {
                /* This clears both lock and holdover in the transition-low reg. */
                status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                                         ZLS3077X_HOLDOVER_LOCK_TL_STICKY_REG(pllId),
                                         0x00,
                                         ZLS3077X_HOLDOVER_LOCK_STATUS_MASK_DPLL,
                                         0);
            }

        #if defined ZLS30771_WAITING_FOR_STICKY_BITS
            if (status == ZL303XX_OK)
            {
                /* Wait for 77x to update registers with new data */
                OS_TASK_DELAY(ZLS3077X_STICKY_UPDATE_DELAY_MS);

                if (status == ZL303XX_OK)
                {
                    /* Read the CURRENT transition-high sticky bits */
                    status = zl303xx_Read(zl303xx_Params, NULL,
                                        ZLS3077X_HOLDOVER_LOCK_TH_STICKY_REG(pllId),
                                        &dpllTh);
                }
                if (status == ZL303XX_OK)
                {
                    /* Read the CURRENT transition-low sticky bits */
                    status = zl303xx_Read(zl303xx_Params, NULL,
                                        ZLS3077X_HOLDOVER_LOCK_TL_STICKY_REG(pllId),
                                        &dpllTl);
                }
            }
        #endif

            if (status == ZL303XX_OK)
            {
                /* Read DPLL status bits */
                status = zl303xx_Read(zl303xx_Params, NULL,
                                    ZLS3077X_HOLDOVER_LOCK_STATUS_REG(pllId),
                                    &dpllStatus);
            }

            if (status == ZL303XX_OK)
            {
                dpllStatus &= ZLS3077X_HOLDOVER_LOCK_STATUS_MASK_DPLL;

                /* If either sticky bit is set, we are not locked. */
                if(( (dpllTh & 0x1) > 0 ) || ( (dpllTl & 0x1) > 0 ))
                {
                    /* Clear just the lock bit - leave the holdover bit */
                    dpllStatus &= 0x2;
                }

                /* Just use the holdover bit from the status register for now. */
            }

            /* Indicate we are done with sticky reg */
            if (stickyStatus == ZL303XX_OK)
            {
                stickyStatus = zl303xx_Dpll77xStickyLockSet(zl303xx_Params,
                                                        ZLS3077X_STICKY_UNLOCK);
                status = (status == ZL303XX_OK) ? stickyStatus : status;
            }

            if (status == ZL303XX_OK)
            {
                if ((ZLS3077X_LOCK_STATUS_DPLL_GET(dpllStatus) == 1) && (ZLS3077X_HOLDOVER_STATUS_DPLL_GET(dpllStatus) == 0))
                {
                    /* H/W is locked */
                    *lockStatus = (Sint32T)ZL303XX_LOCK_STATUS_LOCKED;
                }
                else
                {
                    if (ZLS3077X_HOLDOVER_STATUS_DPLL_GET(dpllStatus) == 1)
                    {
                        /* H/W is in holdover */
                        *lockStatus = (Sint32T)ZL303XX_LOCK_STATUS_HOLDOVER;
                    }
                    else
                    {
                      /* H/W is NOT locked nor holdover */
                     *lockStatus = (Sint32T)ZL303XX_LOCK_STATUS_ACQUIRING;
                    }
                }

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xGetNCOAssistHwLockStatus: dpllStatus=0x%x LOCK=%d HO=%d; lockStatus=%d",
                            dpllStatus, ZLS3077X_LOCK_STATUS_DPLL_GET(dpllStatus), ZLS3077X_HOLDOVER_STATUS_DPLL_GET(dpllStatus),
                            *lockStatus, 0,0);
            }

            /* Get DPLL status bits if it's REF H/W mode and it's NOT locked */
            if ( (status == ZL303XX_OK) &&
                 (*lockStatus != (Sint32T)ZL303XX_LOCK_STATUS_LOCKED) &&
                 (dpllMode == ZLS3077X_DPLL_MODE_REFLOCK || dpllMode == ZLS3077X_DPLL_MODE_AUTO_LOCK) &&
                 (refId != (Uint32T)ZL303XX_INVALID))
             {
                 /* Clear all the sticky bit registers used */
                 status = zl303xx_Dpll77xStickyReadMon(zl303xx_Params, refId,&refFailBits);
                 if (status != ZL303XX_OK)
                 {
                   ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetNCOAssistHwLockStatus: Unknown; Sticky read of refId=%d mask=0x%x failed=%d",
                           refId, ZLS3077X_REF_MON_FAIL_MASK, status, 0,0,0);
                 }

                 if (status == ZL303XX_OK)
                 {
                     refFailBits &= ZLS3077X_REF_MON_FAIL_MASK;

                     /* Dpll lost H/W lock */
                     if (refFailBits)
                     {
                         ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                                 "zl303xx_Dpll77xGetNCOAssistHwLockStatus: HW Ref moving from lockStatus=%d to lockStatus=%d",
                                 *lockStatus, ZL303XX_LOCK_STATUS_REF_FAILED, 0,0,0,0);

                         /* H/W had Ref. Fail */
                         *lockStatus = (Sint32T)ZL303XX_LOCK_STATUS_REF_FAILED;

                         ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetNCOAssistHwLockStatus: refFailBits=0x%x for refId=%d lost H/W lock in FORCE REF mode=%d",
                                 refFailBits, refId, dpllMode, 0,0,0);
                     }
                     else
                     {
                         /* Reference is valid while DPLL is not LOCKED.  We upgrade the status to LOCKED for 1588 software. */
                         ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3,
                                   "zl303xx_Dpll77xGetNCOAssistHwLockStatus: HW Ref moving from lockStatus=%d to lockStatus=%d", *lastLockStatus, ZL303XX_LOCK_STATUS_LOCKED, 0,0,0,0);
                         *lockStatus = (Sint32T)ZL303XX_LOCK_STATUS_LOCKED;
                     }
                 }
             }
        }   /* Check the sticky registers */
    }

    if (status == ZL303XX_OK && *lockStatus != (Sint32T)*lastLockStatus)
    {
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
                    "zl303xx_Dpll77xGetNCOAssistHwLockStatus: Lockstatus CHANGED: lastLockStatus=%d and now lockStatus=%d",
                    *lastLockStatus, *lockStatus, 0,0,0,0);

        *lastLockStatus = (zl303xx_LockStatusE)*lockStatus;
    }

    if (status == ZL303XX_OK)
    {
       ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3,
                   "zl303xx_Dpll77xGetNCOAssistHwLockStatus: returns with lockStatus=%d and status=%d",
                   *lockStatus, status, 0,0,0,0);
    }
    else
    {
       ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 1,
                   "zl303xx_Dpll77xGetNCOAssistHwLockStatus: return FAILURE status=%d",
                   status, 0,0,0,0,0);
    }

    return status;
}


/* zl303xx_Dpll77xGetHwLockStatus */
/**
   Callback function for zl303xx_AprAddDeviceS::getHwLockStatus().

  Parameters:
   [in]   hwParams    Pointer to the device parameter structure.
   [out]  lockStatus  DPLL lock status mapped to zl303xx_AprLockStatusE.

*******************************************************************************/
Sint32T zl303xx_Dpll77xGetHwLockStatus(void *hwParams, Sint32T *lockStatus)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T data;
    Uint32T dpllMode = ZLS3077X_DPLL_MODE_FREERUN;
    Uint32T pllId = 0;
    Uint32T refId = (Uint32T)ZL303XX_INVALID;
    Uint32T dpllStatus;
    Uint32T dpllTh = 0;
    Uint32T dpllTl = 0;
    Uint32T refFailBits = 0;
    zl303xx_LockStatusE *lastLockStatus = NULL;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xGetHwLockStatus: ", 0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTERS(hwParams, lockStatus);
    }
    if (status == ZL303XX_OK)
    {
        lastLockStatus = &zl303xx_Params->pllParams.lastLockStatus;
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    /* Get current ref and dpll mode */
    if (status == ZL303XX_OK)
    {
        /* Detect dpllMode */
        status = zl303xx_Read(zl303xx_Params, NULL,
                      ZLS3077X_DPLL_MODE_REFSEL_REG(pllId), &data);
        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetHwLockStatus: read of dpllMode failed=%d!",
                    status, 0,0,0,0,0);
        }
        else
        {
            dpllMode = data & ZLS3077X_DPLL_MODE_MASK;
        }

        if ((status == ZL303XX_OK) &&
            ((dpllMode == ZLS3077X_DPLL_MODE_REFLOCK) ||
                (dpllMode == ZLS3077X_DPLL_MODE_AUTO_LOCK)))
        {
            /* Detect refId */
            status = zl303xx_Dpll77xCurrRefGet(zl303xx_Params, &refId);

            if (status != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetHwLockStatus: Call to zl303xx_Dpll77xCurrRefGet() failed=%d !!",
                        status, 0,0,0,0,0);
            }
            else
            {
                /* Not in H/W mode */
                if (refId == (Uint32T)ZL303XX_INVALID)
                {
                    *lockStatus = *lastLockStatus;

                    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                            "zl303xx_Dpll77xGetHwLockStatus: Invalid dpllMode=%d for this operation",
                            dpllMode, 0,0,0,0,0);
                }
            }
        }
    }

   /* Note: For 77x devices and 1588, we cannot check for the actual h/w
      lock because it's a sticky reg. and requires time - we cannot wait
      here  because this is called by _TakeHwDcoControl() and
      _ReturnHwDcoControl() in hybrid mode */

    /* For proper sticky status register monitoring:
    * a) indicate lock
    * b) write non-zero value (e.g. 0x1) into Sticky Bit Lock register
    * c) clear sticky bits (one or more)
    * d) write 0 into Sticky Bit Lock register
    * e) indicate unlock
    * f) For current values, we wait for 77x to update sticky status register(s)
         (25ms) - with APR we skip this as we know we will be back soon
    * g) read updated sticky status register(s) value(s)
    */

    /* Get the dpll status */
    if (status == ZL303XX_OK)
    {
        zlStatusE stickyStatus = ZL303XX_ERROR;

#if !defined ZLS30771_WAITING_FOR_STICKY_BITS
        if (status == ZL303XX_OK)
        {
            /* Read the previous transition-high sticky bits */
            status = zl303xx_Read(zl303xx_Params, NULL,
                                ZLS3077X_HOLDOVER_LOCK_TH_STICKY_REG(pllId),
                                &dpllTh);
        }
        if (status == ZL303XX_OK)
        {
            /* Read the previous transition-low sticky bits */
            status = zl303xx_Read(zl303xx_Params, NULL,
                                ZLS3077X_HOLDOVER_LOCK_TL_STICKY_REG(pllId),
                                &dpllTl);
        }
#endif

        /* Indicate we will Lock all sticky registers */
        if (status == ZL303XX_OK)
        {
            stickyStatus = zl303xx_Dpll77xStickyLockSet(zl303xx_Params,
                                                    ZLS3077X_STICKY_LOCK);
            status = stickyStatus;
        }

        /* Clear transition-high and -low lock and holdover sticky bits */
        if (status == ZL303XX_OK)
        {
            /* This clears both lock and holdover in the transition-high reg. */
            status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                                     ZLS3077X_HOLDOVER_LOCK_TH_STICKY_REG(pllId),
                                     0x00,
                                     ZLS3077X_HOLDOVER_LOCK_STATUS_MASK_DPLL,
                                     0);
        }
        if (status == ZL303XX_OK)
        {
            /* This clears both lock and holdover in the transition-low reg. */
            status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                                     ZLS3077X_HOLDOVER_LOCK_TL_STICKY_REG(pllId),
                                     0x00,
                                     ZLS3077X_HOLDOVER_LOCK_STATUS_MASK_DPLL,
                                     0);
        }

        /* Indicate we are done with sticky reg */
        if (stickyStatus == ZL303XX_OK)
        {
            stickyStatus = zl303xx_Dpll77xStickyLockSet(zl303xx_Params,
                                                    ZLS3077X_STICKY_UNLOCK);
            status = (status == ZL303XX_OK) ? stickyStatus : status;
        }

    #if defined ZLS30771_WAITING_FOR_STICKY_BITS
        if (status == ZL303XX_OK)
        {
            /* Wait for 77x to update registers with new data */
            OS_TASK_DELAY(ZLS3077X_STICKY_UPDATE_DELAY_MS);

            if (status == ZL303XX_OK)
            {
                /* Read the current transition-high sticky bits */
                status = zl303xx_Read(zl303xx_Params, NULL,
                                    ZLS3077X_HOLDOVER_LOCK_TH_STICKY_REG(pllId),
                                    &dpllTh);
            }
            if (status == ZL303XX_OK)
            {
                /* Read the current transition-low sticky bits */
                status = zl303xx_Read(zl303xx_Params, NULL,
                                    ZLS3077X_HOLDOVER_LOCK_TL_STICKY_REG(pllId),
                                    &dpllTl);
            }
        }
    #endif

        if (status == ZL303XX_OK)
        {
            /* Read DPLL status bits */
            status = zl303xx_Read(zl303xx_Params, NULL,
                                ZLS3077X_HOLDOVER_LOCK_STATUS_REG(pllId),
                                &dpllStatus);
        }
        if (status == ZL303XX_OK)
        {
            dpllStatus &= ZLS3077X_HOLDOVER_LOCK_STATUS_MASK_DPLL;

            /* If either sticky bit is set, we are not locked. */
            if(( (dpllTh & 0x1) > 0 ) || ( (dpllTl & 0x1) > 0 ))
            {
                /* Clear just the lock bit - leave the holdover bit */
                dpllStatus &= 0x2;
            }

            /* Just use the holdover bit from the status register for now. */
        }
    }

    if (status == ZL303XX_OK)
    {
        if (ZLS3077X_HOLDOVER_STATUS_DPLL_GET(dpllStatus) == 1)
        {
            /* H/W went to holdover */
            *lockStatus = (Sint32T)ZL303XX_LOCK_STATUS_HOLDOVER;

            ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                    "zl303xx_Dpll77xGetHwLockStatus: Holdover", 0,0,0,0,0,0);
        }
        else if (ZLS3077X_LOCK_STATUS_DPLL_GET(dpllStatus) == 1)
        {
            /* H/W is locked */
            *lockStatus = (Sint32T)ZL303XX_LOCK_STATUS_LOCKED;

            if (*lockStatus != (Sint32T)*lastLockStatus)
            {
                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                        "zl303xx_Dpll77xGetHwLockStatus: Locked", 0,0,0,0,0,0);
            }
        }
        else
        {
            /* H/W is NOT locked nor holdover */
            *lockStatus = (Sint32T)ZL303XX_LOCK_STATUS_ACQUIRING;

            ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
                    "zl303xx_Dpll77xGetHwLockStatus: Acquiring", 0,0,0,0,0,0);
        }

        /* Check for H/W ref if forced ref. mode */
        if (status == ZL303XX_OK && *lockStatus != (Sint32T)ZL303XX_LOCK_STATUS_LOCKED)
        {
            if (dpllMode == ZLS3077X_DPLL_MODE_REFLOCK)
            {
                /* Get the current forced ref */
                if (status == ZL303XX_OK)
                {
                    status = zl303xx_Read(zl303xx_Params, NULL,
                                ZLS3077X_DPLL_MODE_REFSEL_REG(pllId), &data);
                    if (status == ZL303XX_OK)
                    {
                        refId = ZLS3077X_DPLL_REFSEL_FORCE_GET(data);
                    }
                    else
                    {
                        refId = (Uint32T)ZL303XX_INVALID;
                    }
                }
            }
        }

        /* Get DPLL status bits if it's REF H/W mode and it's NOT locked */
        if ( (status == ZL303XX_OK) &&
              (*lockStatus != (Sint32T)ZL303XX_LOCK_STATUS_LOCKED) &&
              (dpllMode == ZLS3077X_DPLL_MODE_REFLOCK || dpllMode == ZLS3077X_DPLL_MODE_AUTO_LOCK) &&
              (refId != (Uint32T)ZL303XX_INVALID))
        {
            /* Clear all the sticky bit registers used */
            status = zl303xx_Dpll77xStickyReadMon(zl303xx_Params, refId,&refFailBits);
            if (status != ZL303XX_OK)
            {
              ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetHwLockStatus: Unknown; Sticky read of refId=%d mask=0x%x failed=%d",
                      refId, ZLS3077X_REF_MON_FAIL_MASK, status, 0,0,0);
            }

            if (status == ZL303XX_OK)
            {
                refFailBits &= ZLS3077X_REF_MON_FAIL_MASK;

                /* Dpll lost H/W lock */
                if (refFailBits)
                {
                    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                            "zl303xx_Dpll77xGetHwLockStatus: HW Ref moving from lockStatus=%d to lockStatus=%d",
                            *lockStatus, ZL303XX_LOCK_STATUS_REF_FAILED, 0,0,0,0);

                    /* H/W had Ref. Fail */
                    *lockStatus = (Sint32T)ZL303XX_LOCK_STATUS_REF_FAILED;

                    ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetHwLockStatus: refFailBits=0x%x for refId=%d lost H/W lock in FORCE REF mode=%d",
                            refFailBits, refId, dpllMode, 0,0,0);
                }
                else
                {

                    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3,
                              "zl303xx_Dpll77xGetHwLockStatus: HW Ref moving from lockStatus=%d to lockStatus=%d", *lockStatus, ZL303XX_LOCK_STATUS_LOCKED, 0,0,0,0);
                    *lockStatus = (Sint32T)ZL303XX_LOCK_STATUS_LOCKED;

                }
            }
        }

        /* In automatic mode, it is possible to have a valid reference with never lock priority.
         In this case this function will return reference failure */
        if (status == ZL303XX_OK && dpllMode == ZLS3077X_DPLL_MODE_AUTO_LOCK && refId != (Uint32T)ZL303XX_INVALID)
        {
            Uint32T refPriority = ZLS3077X_DPLL_REF_PRIORITY_NEVER_LOCK;

            status = zl303xx_Dpll77xSetupMailboxForRead(hwParams, ZLS3077X_MB_DPLL, pllId);
            if (status == ZL303XX_OK)
            {
                status = zl303xx_Read(hwParams, NULL, ZLS3077X_DPLL_REF_PRIORITY_REG(refId), &data); /* Retrieve priority data */
            }

            if (status == ZL303XX_OK)
            {
                refPriority = ZLS3077X_DPLL_REF_PRIORITY_GET(data,refId);
            }

            if ( refPriority == ZLS3077X_DPLL_REF_PRIORITY_NEVER_LOCK) /* The reference is invalid in auto mode, downgrade status to ref failure */
            {
                *lockStatus = (Sint32T)ZL303XX_LOCK_STATUS_REF_FAILED;
                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3,
                                "zl303xx_Dpll36xGetHwLockStatus: No valid references in DPLL automatic mode", 0,0,0,0,0,0);
            }
        }
    }

    if ((status == ZL303XX_OK) &&
        (lastLockStatus != NULL))
    {
       if (( *lastLockStatus != (Sint32T)ZL303XX_LOCK_STATUS_REF_FAILED ) &&
           ( *lockStatus == (Sint32T)ZL303XX_LOCK_STATUS_REF_FAILED ))
       {
          /* Due to the sticky nature of the ref status registers, the status
           * may change to ref_fail for a single function call during mode
           * transitions.  We override this behaviour here. */

          *lockStatus = (Sint32T)ZL303XX_LOCK_STATUS_LOCKED;
          *lastLockStatus = (Sint32T)ZL303XX_LOCK_STATUS_REF_FAILED;
          ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3,
                "zl303xx_Dpll77xGetHwLockStatus: first lock failed, overriding to lockStatus=%d",
                *lockStatus,0,0,0,0,0);
       }

       else if (( *lastLockStatus == (Sint32T)ZL303XX_LOCK_STATUS_LOCKED ) &&
                ( *lockStatus == (Sint32T)ZL303XX_LOCK_STATUS_ACQUIRING || *lockStatus == (Sint32T)ZL303XX_LOCK_STATUS_REF_FAILED ) &&
                ( dpllMode == ZLS3077X_DPLL_MODE_REFLOCK || dpllMode == ZLS3077X_DPLL_MODE_AUTO_LOCK ) )
       {
          /* Decision whether to report the chip is in "acquiring" or "reference failure" */
          /* Implement a rough GST when going from locked to not locked. */

          /* Check if this is the 1st time through */
          if( zl303xx_Params->pllParams.lockStateGST == 0 )
          {
             /* 1st time through; Set the rough GST to 10 calls of this
               routine. Microsemi code calls this routine every second. */
             zl303xx_Params->pllParams.lockStateGST = 10;

             /* During the rough GST, return locked. */
             *lockStatus = (Sint32T)ZL303XX_LOCK_STATUS_LOCKED;
          }
          else
          {
             /* Decrement the GST counter */
             zl303xx_Params->pllParams.lockStateGST--;

             /* Are we still in the GST delay? */
             if( zl303xx_Params->pllParams.lockStateGST > 0 )
             {
                /* If still in GST, return locked. */
                *lockStatus = (Sint32T)ZL303XX_LOCK_STATUS_LOCKED;
             }
             else
             {
                /* No; take the new state */
                *lastLockStatus = *lockStatus;
                zl303xx_Params->pllParams.lockStateGST = 0;
             }
          }
       }
       else
       {
          *lastLockStatus = *lockStatus;
          zl303xx_Params->pllParams.lockStateGST = 0;
       }
    }

    if ((status == ZL303XX_OK) && (lockStatus != NULL))
    {
       ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
               "zl303xx_Dpll77xGetHwLockStatus: return lockStatus=%d and status=%d",
               *lockStatus, status, 0,0,0,0);
    }
    else
    {
       ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3,
             "zl303xx_Dpll77xGetHwLockStatus: return status=%d",
             status, 0,0,0,0,0);
    }

    return (Sint32T)status;
}


/* zl303xx_Dpll77xHwModeToBoolean */
/** 

   Checks if the current DPLL is in a specific mode.

  Parameters:
   [in]   hwParams   Pointer to a device parameter structure.
   [in]   modeCheck  The current DPLL's mode to check for.
   [out]  matched    1 if the the current DPLL's mode is the same as
                          modeCheck. 0 otherwise.

*******************************************************************************/
static Sint32T zl303xx_Dpll77xHwModeToBoolean(void *hwParams,
                                              ZLS3077X_DpllHWModeE modeCheck,
                                              Sint32T *matched)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
   Uint32T data;


   if (status == ZL303XX_OK)
   {
       status = ZL303XX_CHECK_POINTERS(hwParams, matched);
   }

   if (status == ZL303XX_OK)
   {
      *matched = 0;
   }
   if (status == ZL303XX_OK)
   {
      status = ZLS3077X_CHECK_DPLLID(zl303xx_Params->pllParams.pllId);
   }
   if (status == ZL303XX_OK)
   {
      status = zl303xx_Read(hwParams, NULL,
                 ZLS3077X_DPLL_MODE_REFSEL_REG(zl303xx_Params->pllParams.pllId),
                 &data);
   }
   if (status == ZL303XX_OK)
   {
      if ((Uint32T)modeCheck == (data & ZLS3077X_DPLL_MODE_MASK))
      {
         *matched = 1;
      }
   }

   return (Sint32T)status;
}


/* zl303xx_Dpll77xGetHwManualHoldoverStatus */
/**
   Callback function for zl303xx_AprAddDeviceS::getHwManualHoldoverStatus().

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [out]  HOStatus  ZL303XX_TRUE if the current DPLL is in manual holdover.
                         ZL303XX_FALSE otherwise.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
Sint32T zl303xx_Dpll77xGetHwManualHoldoverStatus(void *hwParams, Sint32T *HOStatus)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;


    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTERS(hwParams, HOStatus);
    }
    if (status == ZL303XX_OK)
    {
        if( zl303xx_Params->pllParams.mitigationEnabled == ZL303XX_TRUE )
        {
            /* While in mitigation, we may take control of the NCO to make phase
               adjustments. If the device is in NCO, this routine would return
               not-in-manual-holdover but we want this routine to return in-manual
               holdover. */
            *HOStatus = ZL303XX_TRUE;
        }
        else
        {
            status = zl303xx_Dpll77xHwModeToBoolean(hwParams, ZLS3077X_DPLL_MODE_HOLDOVER, HOStatus);
        }
    }

   return (Sint32T)status;
}


/* zl303xx_Dpll77xMitigationEnabledSet */
/*
    Sets the mitigationEnabled boolean.

    Currently, this boolean affects manual holdover.

  Parameters:
   [in]  hwParams            Pointer to a device parameter structure.
   [in]  mitigationEnabled   Value to set

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xMitigationEnabledSet(void *hwParams, zl303xx_BooleanE mitigationEnabled)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
            "zl303xx_Dpll77xMitigationEnabledSet: mitigationEnabled=%d ", mitigationEnabled, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params->pllParams.mitigationEnabled = mitigationEnabled;
    }

    return status;
}


/* zl303xx_Dpll77xGetHwManualFreerunStatus */
/**
   Callback function for zl303xx_AprAddDeviceS::getHwManualFreerunStatus().

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [out]  status    ZL303XX_TRUE if the current DPLL is in manual freerun.
                         ZL303XX_FALSE otherwise.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
Sint32T zl303xx_Dpll77xGetHwManualFreerunStatus(void *hwParams, Sint32T *status)
{
   return zl303xx_Dpll77xHwModeToBoolean(hwParams, ZLS3077X_DPLL_MODE_FREERUN, status);
}


/* zl303xx_Ref77xFreqDetectedGet */
/**
   Returns the frequency a reference input is configured to detect.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [in]   refId     Reference input to query.
   [out]  freqHz    Configured frequency, in Hz.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Ref77xFreqDetectedGet(void *hwParams, Uint32T refId,
                                        Uint32T *freqHz)
{
    zlStatusE status = ZL303XX_OK;
    osStatusT osStatus = OS_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T prediv;
    zl303xx77xRefMailBoxS *mb;
    zl303xx_BooleanE mutexTaken = ZL303XX_FALSE;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Ref77xFreqDetectedGet: refID=%d", refId, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTERS(hwParams, freqHz);
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_REF_ID(refId);
    }
    if (status == ZL303XX_OK)
    {
        osStatus = OS_MUTEX_TAKE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Ref77xFreqDetectedGet: OS_MUTEX_TAKE failed: %d",
                    status, 0, 0, 0,0,0);
        }
    }
    if (status == ZL303XX_OK)
    {
        mutexTaken = ZL303XX_TRUE;
    }
    if (status == ZL303XX_OK)
    {
        mb = &(zl303xx_Params->pllParams.d77x.refMb[refId]);

        prediv = mb->ref_config;
        /* Set prediv to 1 if bit is low, 2 if bit is high */
        prediv = ZLS3077X_REF_PRE_DIVIDE(prediv) + 1;

        *freqHz = (Uint32T)((Uint64T)prediv *
                            (Uint64T)mb->ref_freq_base *     /* br */
                            (Uint64T)mb->ref_freq_mult *     /* kr */
                            (Uint64T)mb->ref_ratio_m /       /* mr */
                            (Uint64T)mb->ref_ratio_n);       /* nr */
    }
    if( mutexTaken == ZL303XX_TRUE )
    {
        osStatus = OS_MUTEX_GIVE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_GIVE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Ref77xFreqDetectedGet: OS_MUTEX_GIVE failed: %d",
                    status, 0, 0, 0,0,0);
        }
    }

    if (status == ZL303XX_OK)
    {
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                "zl303xx_Ref77xFreqDetectedGet: freqHz=%d", *freqHz,0,0,0,0,0);
    }

   return status;
}


/* zl303xx_Dpll77xGetHwSyncInputEnStatus */
/**
   Callback function for zl303xx_AprAddDeviceS::getHwSyncInputEnStatus().

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [out]  en        1 if the input frequency of the current ref is 1Hz and
                       there is no ref sync pair configured. The DPLL must be
                       in "forced reference" or "automatic" mode.
                       0 otherwise.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
Sint32T zl303xx_Dpll77xGetHwSyncInputEnStatus(void *hwParams, Sint32T *en)
{
    zlStatusE status = ZL303XX_OK;
    osStatusT osStatus = OS_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T refId;
    Uint32T syncId;
    ZLS3077X_RefSynchPairModeE refSyncMode;
    Uint32T data;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xGetHwSyncInputEnStatus: ", 0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTERS(hwParams, en);
    }
    if (status == ZL303XX_OK)
    {
        *en = ZL303XX_FALSE;
        status = zl303xx_Dpll77xCurrRefGet(hwParams, &refId);
    }

    if (status == ZL303XX_OK)
    {
        /* Return early if the reference could not be determined */
        if (refId == (Uint32T)ZL303XX_INVALID)
        {
            return (Sint32T)status;
        }
    }
    if (status == ZL303XX_OK)
    {
        osStatus = OS_MUTEX_TAKE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetHwSyncInputEnStatus: OS_MUTEX_TAKE failed: %d",
                    status, 0, 0, 0,0,0);
        }
    }
    if (status == ZL303XX_OK)
    {
        /* Get the current reference's sync input */
        data = zl303xx_Params->pllParams.d77x.refMb[refId].ref_sync;
    }
    if (status == ZL303XX_OK)
    {
        osStatus = OS_MUTEX_GIVE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_GIVE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetHwSyncInputEnStatus: OS_MUTEX_GIVE failed: %d",
                    status, 0, 0, 0,0,0);
        }
    }
    if (status == ZL303XX_OK)
    {
        syncId = ZLS3077X_REF_CLK_SYNC_PAIR_GET(data);
        refSyncMode = ZLS3077X_REF_CLK_SYNC_MODE_GET(data);
    }
    if (status == ZL303XX_OK)
    {
        if (refSyncMode == ZLS3077X_RSPM_enabled)
        {
            /* A ref-sync pair. Look at the current status of the 1pps input. */
            if (syncId >= ZLS3077X_REF_MAX)
            {
                /* Bad configuration */
                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                        "zl303xx_Dpll77xGetHwSyncInputEnStatus: Bad refSyncPair: ref=%d  pair=%d",
                        refId, syncId, 0,0,0,0);
            }
            else
            {
                status = zl303xx_Dpll77xStickyReadMon(hwParams, syncId, &data);
                if (status != ZL303XX_OK)
                {
                    ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetHwSyncInputEnStatus: Sticky read of syncId=%d mask=0x%x failed=%d",
                            syncId, ZLS3077X_REF_MON_FAIL_MASK, status, 0,0,0);
                }

                if (status == ZL303XX_OK)
                {
                    *en = !(data & ZLS3077X_REF_MON_FAIL_MASK);
                    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                            "zl303xx_Dpll77xGetHwSyncInputEnStatus Enabled: %d", *en, 0,0,0,0,0);
                }
            }
        }
        else if (refSyncMode == ZLS3077X_RSPM_disabled)
        {
            /* Not a ref-sync pair. If the input freq = 1Hz, then APR should
               report PHASE-LOCK */
            Uint32T freqHz;

            if (status == ZL303XX_OK)
            {
                status = zl303xx_Ref77xFreqDetectedGet(hwParams, refId, &freqHz);
            }
            if (status == ZL303XX_OK)
            {
                if (freqHz == 1)
                {
                    *en = ZL303XX_TRUE;
                }

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                        "zl303xx_Dpll77xGetHwSyncInputEnStatus: %dHz and No refSyncPair, enabled=%d",
                        freqHz, *en, 0,0,0,0);
            }
        }
        else if( (refSyncMode >= ZLS3077X_RSPM_50_50_clk_25_75_PPS_sync) &&
                 (refSyncMode <= ZLS3077X_RSPM_75_25_clk_25_75_PPS2_sync) )
        {
            /* PWM is providing the 1pps signal */
            status = zl303xx_Dpll77xStickyReadMon(hwParams, refId, &data);
            if (status != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetHwSyncInputEnStatus: Sticky read of refId=%d mask=0x%x failed=%d",
                        refId, ZLS3077X_REF_MON_FAIL_MASK, status, 0,0,0);
            }

            if (status == ZL303XX_OK)
            {
                *en = !(data & ZLS3077X_REF_MON_FAIL_MASK);
                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                        "zl303xx_Dpll77xGetHwSyncInputEnStatus PWM: %d", *en, 0,0,0,0,0);
            }

        }
    }

    return (Sint32T)status;
}


/* zl303xx_Dpll77xGetHwOutOfRangeStatus */
/**
   Callback function for zl303xx_AprAddDeviceS::getHwOutOfRangeStatus().

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [out]  oor       1 if the PFM for the active reference on the current DPLL
                         has failed. The DPLL must be in "forced reference" or
                         "automatic" mode. 0 otherwise.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
Sint32T zl303xx_Dpll77xGetHwOutOfRangeStatus(void *hwParams, Sint32T *oor)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T refId;
    Uint32T data;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xGetHwOutOfRangeStatus: ", 0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(oor);
    }

    if (status == ZL303XX_OK)
    {
        *oor = 0;
        status = zl303xx_Dpll77xCurrRefGet(hwParams, &refId);
    }
    if (status == ZL303XX_OK)
    {
        /* Return early if the reference could not be determined. */
        if (refId == (Uint32T)ZL303XX_INVALID)
        {
            return status;
        }
    }
    if (status == ZL303XX_OK)
    {
        /* Clear all the sticky bit registers used */
        if ((status = zl303xx_Dpll77xStickyReadMon(hwParams, ZL303XX_INVALID,
                                                &data)) != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetHwOutOfRangeStatus: Sticky read of refId=%d mask=0x%x failed=%d",
                    refId, ZLS3077X_REF_MON_FAIL_PFM, status, 0,0,0);
        }
    }
    /* Set oor to 1 if PFM bit is high */
    if (status == ZL303XX_OK)
    {
        *oor = !!(data & ZLS3077X_REF_MON_FAIL_PFM);
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                "zl303xx_Dpll77xGetHwOutOfRangeStatus: %d", *oor, 0,0,0,0,0);
    }

    return (Sint32T)status;
}


/* zl303xx_Dpll77xOverridePllPriorMode */
/**
   Force new values into stored pllPriorMode and selectedRef.

  Parameters:
   [in]  hwParams   Pointer to the device instance parameter structure.
   [in]  overrridePllPriorMode  New mode.
   [in]  refId  H/W reference to follow (or NONE).
            Note: ZL303XX_REF_AUTO means no change to h/w ref

  Return Value:  ZL303XX_OK  Success.
*******************************************************************************/
zlStatusE zl303xx_Dpll77xOverridePllPriorMode(void *hwParams,
                    ZLS3077X_DpllHWModeE overrridePllPriorMode, zl303xx_RefIdE refId)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    ZLS3077X_DpllHWModeE *priorMode;
    ZLS3077X_DpllHWModeE oldPriorMode;
    zl303xx_RefIdE *selectedRef;
    zl303xx_RefIdE oldSelectedRef;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xOverridePllPriorMode: overrridePllPriorMode=%d  refId=%d",
            overrridePllPriorMode, refId, 0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(zl303xx_Params);
    }

    if (status == ZL303XX_OK)
    {
        priorMode = &zl303xx_Params->pllParams.d77x.pllPriorMode;
        selectedRef = &zl303xx_Params->pllParams.selectedRef;
        oldPriorMode = zl303xx_Params->pllParams.d77x.pllPriorMode;
        oldSelectedRef = zl303xx_Params->pllParams.selectedRef;
    }
    if (status == ZL303XX_OK)
    {
        if (overrridePllPriorMode != ZLS3077X_DPLL_MODE_NCO)
        {
            /* Store mode provided */
            *priorMode = overrridePllPriorMode;

            if (refId > ZL303XX_REF_AUTO)
            {
                /* Store mode provided */
                *selectedRef = refId;
            }
            ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
                    "zl303xx_Dpll77xOverridePllPriorMode: Old pllPriorMode=%d now pllPriorMode=%d; Old selectedRef=%d now selectedRef=%d",
                    oldPriorMode, *priorMode, oldSelectedRef, *selectedRef, 0,0);
        }
        else
        {
            /* All others: (FR/HO/RL/AUTO) are valid prior modes */
            status = ZL303XX_INVALID_MODE;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xOverridePllPriorMode: ZL303XX_DPLL_MODE_NCO is NOT a valid prior mode",
                    0,0,0,0,0,0);
        }
    }

    return status;
}


/* zl303xx_Dpll77xModeSet */
/*
   Sets the current DPLL mode of operation.
   If mode is REFLOCK then the reference was set elsewhere (via
    zl303xx_Dpll77xHWModeSet()).

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [out]  mode      The DPLL operating mode to set.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xModeSet(void *hwParams, ZLS3077X_DpllHWModeE mode)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
            "zl303xx_Dpll77xModeSet: mode=%d", mode, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }

    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLL_MODE(mode);
    }

    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        if (mode == ZLS3077X_DPLL_MODE_NCO)
        {
            ZL303XX_TRACE_ALWAYS ("zl303xx_Dpll77xModeSet: NCO", 0, 0,0,0,0,0);
        }
    }

    /* Set the mode for non-RefLock */
    if (status == ZL303XX_OK &&
       (mode == ZLS3077X_DPLL_MODE_NCO ||
            mode == ZLS3077X_DPLL_MODE_HOLDOVER ||
            mode == ZLS3077X_DPLL_MODE_AUTO_LOCK))
    {
        status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                                    ZLS3077X_DPLL_MODE_REFSEL_REG(pllId),
                                    mode, ZLS3077X_DPLL_MODE_MASK, NULL);
        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS ("zl303xx_Dpll77xModeSet: Call to set device MODE=%d failed=%d",
                    mode, status, 0,0,0,0);
        }
    }

    if (status == ZL303XX_OK &&
      (mode == ZLS3077X_DPLL_MODE_REFLOCK || mode == ZLS3077X_DPLL_MODE_AUTO_LOCK))
    {
        if (mode == ZLS3077X_DPLL_MODE_REFLOCK)
        {
            Uint32T hwRef = ((zl303xx_Params->pllParams.selectedRef & ZLS3077X_DPLL_REF_SEL_MASK) <<
                            ZLS3077X_REF_SEL_OFFSET_SHIFT);

            status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                                       ZLS3077X_DPLL_MODE_REFSEL_REG(pllId),
                                       (hwRef | mode),
                                       ZLS3077X_DPLL_REFMODE_MASK, NULL);
            if (status != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS ("zl303xx_Dpll77xModeSet: Call to set device MODE+REF failed=%d",
                        status, 0,0,0,0,0);
            }
            else
            {
                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
                        "zl303xx_Dpll77xModeSet: selectedRef=%d",
                        zl303xx_Params->pllParams.selectedRef, 0,0,0,0,0);
            }
        }
        else /* Auto */
        {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
                    "zl303xx_Dpll77xModeSet: AUTO", 0,0,0,0,0,0);
        }
    }

    /* Save PLL H/W modes only */
    if (status == ZL303XX_OK && mode != ZLS3077X_DPLL_MODE_NCO)  /* Save the new mode */
    {   /* Note: ZL303XX_REF_AUTO means no change to h/w ref */
        if ((status = zl303xx_Dpll77xOverridePllPriorMode(hwParams, mode,
                                            ZL303XX_REF_AUTO)) != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xModeSet: Call to zl303xx_Dpll77xOverridePllPriorMode() failed=%d",
                    status, 0,0,0,0,0);
        }
    }
    return status;
}


/* zl303xx_Dpll77xModeGet */
/**
   Returns the current DPLL mode of operation.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [out]  mode      The DPLL operating mode.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xModeGet(void *hwParams, ZLS3077X_DpllHWModeE *mode)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;
    Uint32T data;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xModeGet: ", 0, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTERS(hwParams, mode);
    }

    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL,
                     ZLS3077X_DPLL_MODE_REFSEL_REG(pllId), &data);
    }

    if (status == ZL303XX_OK)
    {
        *mode = data & ZLS3077X_DPLL_MODE_MASK;
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                "zl303xx_Dpll77xModeGet: %d", *mode, 0,0,0,0,0);
    }

    return status;
}


/* zl303xx_Dpll77xHWModeSet */
/*
   Extended user interface to set the DPLL mode of operation (HARDWARE modes AND refs).
   This user interface in not used internally.

  Parameters:
   [in]  hwParams   Pointer to a device parameter structure.
   [in]  mode       The DPLL operating mode to set.
   [in]  refId      The H/W reference to save.
   [in]  freqOffsetToSetInppt  The freq. to save in manual holdover or freerun.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xHWModeSet(void *hwParams, ZLS3077X_DpllHWModeE mode,
                                Uint8T refId, Sint32T freqOffsetToSetInppt)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
            "zl303xx_Dpll77xHWModeSet: mode=%d  refId=%d  freqOffsetToSetInppt=%d",
            mode, refId, freqOffsetToSetInppt,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_REF_ID(refId);
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_DPLL_MODE(mode);
    }

    if (status == ZL303XX_OK)
    {
      pllId = zl303xx_Params->pllParams.pllId;
      status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK )
    {
        if (freqOffsetToSetInppt != 0)  /* Use customer-provided value */
        {
            status = zl303xx_Dpll77xModeSet(hwParams, ZLS3077X_DPLL_MODE_NCO);
            if (status != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xHWModeSet: Call to zl303xx_Dpll77xModeSet() for customer freq override failed=%d",
                        status, 0,0,0,0,0);
            }
            else
            {
                status = zl303xx_Dpll77xSetFreq(zl303xx_Params, freqOffsetToSetInppt);
                if (status != ZL303XX_OK)
                {
                    ZL303XX_TRACE_ALWAYS ("zl303xx_Dpll77xHWModeSet: Call to zl303xx_Dpll77xSetFreq() failed=%d",
                            status, 0,0,0,0,0);
                }
            }
        }
    }

    if (status == ZL303XX_OK)  /* Set the ref, mode and priorMode */
    {
        if ((mode == ZLS3077X_DPLL_MODE_FREERUN) ||
            (mode == ZLS3077X_DPLL_MODE_HOLDOVER))
        {
            /* special case logging */
            ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
                    "zl303xx_Dpll77xHWModeSet: DPLL in FREERUN or HOLDOVER; Keeping stored freqOffsetInppt=%d (0x%x)",
                    zl303xx_Params->pllParams.dcoFreq,
                    zl303xx_Params->pllParams.dcoFreq, 0,0,0,0);
        }
        if (mode == ZLS3077X_DPLL_MODE_NCO)
        {
            zl303xx_Params->pllParams.d77x.pllPriorMode = mode;
            ZL303XX_TRACE_ALWAYS ("zl303xx_Dpll77xHWModeSet: Forced priorMode=%d in NCO mode",
                    mode, 0,0,0,0,0);
        }
        if (mode == ZLS3077X_DPLL_MODE_REFLOCK)
        {
            zl303xx_Params->pllParams.selectedRef = refId;
            ZL303XX_TRACE_ALWAYS ("zl303xx_Dpll77xHWModeSet: Forced selectedRef=%d in HW mode",
                    refId, 0,0,0,0,0);
        }

        if (status == ZL303XX_OK)  /* Save the new mode */
        {
            status = zl303xx_Dpll77xModeSet(hwParams, mode);
            if (status != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xHWModeSet: Call to zl303xx_Dpll77xModeSet() failed=%d",
                        status, 0,0,0,0,0);
            }

            if ((status == ZL303XX_OK) && (mode == ZLS3077X_DPLL_MODE_REFLOCK))
            {
                Sint32T lockStatus;

                /* Check for lock status */
                if (status == ZL303XX_OK)
                {
                    status = zl303xx_Dpll77xGetHwLockStatus(zl303xx_Params, &lockStatus);
                    if (status != ZL303XX_OK)
                    {
                        ZL303XX_TRACE_ALWAYS ("zl303xx_Dpll77xHWModeSet: Call to zl303xx_Dpll77xGetHwLockStatus() failed=%d",
                                status, 0,0,0,0,0);
                    }
                }

                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
                        "zl303xx_Dpll77xHWModeSet: selectedRef=%d, lockStatus=%d",
                        zl303xx_Params->pllParams.selectedRef, lockStatus, 0,0,0,0);
            }
        }
    }

    return status;
}


/* zl303xx_Dpll77xCurrRefSet */
/*
   Force the HW reference, even if we are not in FORCED_REF mode of operation

  Parameters:
   [in]  hwParams   Pointer to a device parameter structure.
   [in]  refId      The H/W reference to save.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xCurrRefSet(void *hwParams, Uint8T refId)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
            "zl303xx_Dpll77xCurrRefSet: refId=%d ", refId, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }

    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
		/* pllidが0 or 3の場合は0x214と0x21cに対して設定を行う。(0x210には設定しない) */
		/* 0x21CはDPLL0時は未使用ですが共通手順とする。*/
		if((pllId == 0)||(pllId == 3))
		{
        	status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                ZLS3077X_DPLL_MODE_REFSEL_REG(1),
                (refId << ZLS3077X_REF_SEL_OFFSET_SHIFT),
                (ZLS3077X_DPLL_REF_SEL_MASK << ZLS3077X_REF_SEL_OFFSET_SHIFT),
                NULL);
                
        	status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                ZLS3077X_DPLL_MODE_REFSEL_REG(3),
                (refId << ZLS3077X_REF_SEL_OFFSET_SHIFT),
                (ZLS3077X_DPLL_REF_SEL_MASK << ZLS3077X_REF_SEL_OFFSET_SHIFT),
                NULL);
        }
        else{
			/* 従来処理 */
        	status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                ZLS3077X_DPLL_MODE_REFSEL_REG(pllId),
                (refId << ZLS3077X_REF_SEL_OFFSET_SHIFT),
                (ZLS3077X_DPLL_REF_SEL_MASK << ZLS3077X_REF_SEL_OFFSET_SHIFT),
                NULL);
		}
        zl303xx_Params->pllParams.selectedRef = refId;
    }

    return status;
}


/* zl303xx_Dpll77xCurrRefGet */
/**
   Returns the current reference of the current DPLL.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [out]  refId     The current reference. ZL303XX_INVALID is returned if the
                         DPLL is not in "forced reference" or "automatic" mode.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xCurrRefGet(void *hwParams, Uint32T *refId)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;
    Uint32T data;
    Uint32T dpllMode;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xCurrRefGet: ", 0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTERS(hwParams, refId);
    }
    if (status == ZL303XX_OK)
    {
        *refId = (Uint32T)ZL303XX_INVALID;

        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    /* Get the current DPLL's mode */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL,
                     ZLS3077X_DPLL_MODE_REFSEL_REG(pllId), &data);
    }
    /* Get the DPLL's current reference. Register to read depends on the DPLL
       mode: forced or auto. All other DPLL modes return early from this
       function. */
    if (status == ZL303XX_OK)
    {
        dpllMode = data & ZLS3077X_DPLL_MODE_MASK;

        if (dpllMode == ZLS3077X_DPLL_MODE_REFLOCK)
        {
            *refId = ZLS3077X_DPLL_REFSEL_FORCE_GET(data);
        }
        else if (dpllMode == ZLS3077X_DPLL_MODE_AUTO_LOCK)
        {
            if ((status = zl303xx_Read(zl303xx_Params, NULL,
                        ZLS3077X_DPLL_REFSEL_STAT_REG(pllId), &data)) == ZL303XX_OK)
            {
                *refId = ZLS3077X_DPLL_REFSEL_AUTO_GET(data);
            }
        }
        else
        {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                    "zl303xx_Dpll77xCurrRefGet: Invalid mode=%d to read h/w ref",
                    dpllMode, 0,0,0,0,0);
        }
    }
    if (status == ZL303XX_OK)
    {
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                "zl303xx_Dpll77xCurrRefGet: Current h/w ref=%d", *refId, 0,0,0,0,0);
    }

    return status;
}


/* zl303xx_Dpll77xTieClearGet */
/**
   Gets the TIE clear bit for a DPLL.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [in]   tieClear  Pointer to zl303xx_BooleanE valueZL303XX_TRUE

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xTieClearGet(void *hwParams, zl303xx_BooleanE *tieClear)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ParamsS *zl303xx_Params;
   Uint32T pllId  = 0;
   Uint32T regVal = 0;

   ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 3, "zl303xx_Dpll77xTieClearGet: tieClear=%d",
                 tieClear, 0,0,0,0,0);

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(hwParams);
   }

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tieClear);
   }

   if (status == ZL303XX_OK)
   {
      zl303xx_Params = hwParams;
      pllId = zl303xx_Params->pllParams.pllId;
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_Read(hwParams, NULL, ZLS3077X_DPLL_TIE_CLEAR_REG(pllId), &regVal);
      *tieClear = 0x1 & (regVal >> ZLS3077X_DPLL_TIE_CLEAR_EN_SHIFT);
   }

   return status;
}


/* zl303xx_Dpll77xTieClearSet */
/**
   Sets the TIE clear bit for a DPLL.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [in]   tieClear  ZL303XX_TRUE to enable TIE clear. ZL303XX_FALSE for hitless
                         switching.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xTieClearSet(void *hwParams, zl303xx_BooleanE tieClear)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xTieClearSet: tieClear=%d",
            tieClear, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_BOOLEAN(tieClear);
    }

    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                        ZLS3077X_DPLL_TIE_CLEAR_REG(pllId),
                        tieClear << ZLS3077X_DPLL_TIE_CLEAR_EN_SHIFT,
                        ZLS3077X_DPLL_TIE_CLEAR_EN_MASK << ZLS3077X_DPLL_TIE_CLEAR_EN_SHIFT,
                        NULL);
    }

    return status;
}


/* zl303xx_Dpll77xFastLockSet */
/**
   Sets the fast lock bit for a DPLL.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [in]   fastLock  ZL303XX_TRUE to enable fast lock. ZL303XX_FALSE to disable.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xFastLockSet(void *hwParams, zl303xx_BooleanE fastLock)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;
    Uint32T v;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xFastLockSet: fastLock=%d",
            fastLock, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_BOOLEAN(fastLock);
    }

    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetupMailboxForWrite(hwParams,
                                                ZLS3077X_MB_DPLL, pllId);
    }
    if (status == ZL303XX_OK)
    {
        v  = (fastLock == ZL303XX_TRUE) ? (1 << ZLS3077X_FAST_LOCK_NCO_ENABLE_MASK) : 0;
        v |= (1 << ZLS3077X_FAST_LOCK_ENABLE_MASK);

        status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                                ZLS3077X_DPLLX_FAST_LOCK_CTRL_REG,
                                v, ZLS3077X_FAST_LOCK_MASK, NULL);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetMailboxSemWrite(hwParams, ZLS3077X_MB_DPLL);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xUpdateMailboxCopy(hwParams, ZLS3077X_MB_DPLL, pllId);
    }

    return status;
}


/* zl303xx_Dpll77xFastLockGet */
/**
   Gets the fast lock bit for a DPLL.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [out]  fastLock  Pointer to retrieved fast lock register value

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xFastLockGet(void *hwParams, zl303xx_BooleanE *fastLock)
{
    zlStatusE status = ZL303XX_OK;
    osStatusT osStatus = OS_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xFastLockGet: ",
            0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(fastLock);
    }

    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        osStatus = OS_MUTEX_TAKE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xFastLockGet: OS_MUTEX_GIVE failed: %d",
                    status, 0, 0, 0,0,0);
        }
    }
    if (status == ZL303XX_OK)
    {
        *fastLock = zl303xx_Params->pllParams.d77x.dpllMb[pllId].dpll_fast_lock_ctrl &
                        (1 << ZLS3077X_FAST_LOCK_NCO_ENABLE_MASK);
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xFastLockGet: *fastLock=%d",
                *fastLock,0,0,0,0,0);
    }
    if (status == ZL303XX_OK)
    {
        osStatus = OS_MUTEX_GIVE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_GIVE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xFastLockGet: OS_MUTEX_GIVE failed: %d",
                    status, 0, 0, 0,0,0);
        }
    }

    return status;
}


/* zl303xx_Dpll77xPullInRangeSet */
/**
   Sets the pull in range register value for a DPLL.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [in]   PullInRange       Pull-in range register value to set in 0.1ppm
                               resolution eg. for a 12ppm pull-in range, the
                               calling routine would pass 120.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xPullInRangeSet(void *hwParams, Uint32T PullInRange)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xPullInRangeSet: PullInRange=%d",
            PullInRange, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetupMailboxForWrite(hwParams, ZLS3077X_MB_DPLL, pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Write(zl303xx_Params, NULL, ZLS3077X_DPLLX_RANGE_REG, PullInRange );
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetMailboxSemWrite(hwParams, ZLS3077X_MB_DPLL);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xUpdateMailboxCopy(hwParams, ZLS3077X_MB_DPLL, pllId);
    }

    return status;
}


/* zl303xx_Dpll77xPullInRangeGet */
/**
   Gets the pull in range register value for a DPLL.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [out]  PullInRange       Pull-in range in 0.1ppm resolution
                               eg. for a 12ppm pull-in range, the calling
                               routine would pass 120.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xPullInRangeGet(void *hwParams, Uint32T *PullInRange)
{
    zlStatusE status = ZL303XX_OK;
    osStatusT osStatus = OS_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xPullInRangeSet: ",
            0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(PullInRange);
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        osStatus = OS_MUTEX_TAKE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xPullInRangeGet: OS_MUTEX_TAKE failed: %d",
                    status, 0, 0, 0,0,0);
        }
    }
    if (status == ZL303XX_OK)
    {
        *PullInRange = zl303xx_Params->pllParams.d77x.dpllMb[pllId].dpll_range;
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xPullInRangeSet: *PullInRange=%d",
                *PullInRange, 0,0,0,0,0);
    }
    if (status == ZL303XX_OK)
    {
        osStatus = OS_MUTEX_GIVE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_GIVE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xPullInRangeGet: OS_MUTEX_GIVE failed: %d",
                    status, 0, 0, 0,0,0);
        }
    }


    return status;
}


/* zl303xx_Dpll77xPhaseSlopeLimitSet */
/**
   Sets the phase slope limit register value for a DPLL.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [in]   PSL       phase slope limit register value to set

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xPhaseSlopeLimitSet(void *hwParams, Uint32T PSL)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xPhaseSlopeLimitSet: PSL=%d",
            PSL, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetupMailboxForWrite(hwParams, ZLS3077X_MB_DPLL, pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Write(zl303xx_Params, NULL, ZLS3077X_DPLLX_PSL_REG, PSL );
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetMailboxSemWrite(hwParams, ZLS3077X_MB_DPLL);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xUpdateMailboxCopy(hwParams, ZLS3077X_MB_DPLL, pllId);
    }

    return status;
}


/* zl303xx_Dpll77xPhaseSlopeLimitGet */
/**
   Sets the phase slope limit register value for a DPLL.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [out]  PSL       Pointer to retrieved phase slope limit register value

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xPhaseSlopeLimitGet(void *hwParams, Uint32T *PSL)
{
    zlStatusE status = ZL303XX_OK;
    osStatusT osStatus = OS_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xPhaseSlopeLimitGet: ",
            0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTERS(hwParams, PSL);
    }

    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        osStatus = OS_MUTEX_TAKE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xPhaseSlopeLimitGet: OS_MUTEX_TAKE failed: %d",
                    status, 0, 0, 0,0,0);
        }
    }
    if (status == ZL303XX_OK)
    {
        *PSL = zl303xx_Params->pllParams.d77x.dpllMb[pllId].dpll_psl;
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xPhaseSlopeLimitGet: *PSL=%d",
                *PSL, 0,0,0,0,0);
    }
    if (status == ZL303XX_OK)
    {
        osStatus = OS_MUTEX_GIVE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_GIVE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xPhaseSlopeLimitGet: OS_MUTEX_GIVE failed: %d",
                    status, 0, 0, 0,0,0);
        }
    }

    return status;
}


/* zl303xx_Dpll77xBandwidthSet */
/**
   Sets the bandwidth register value for a DPLL.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [in]   Bandwidth Bandwidth register value to set

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xBandwidthSet(void *hwParams, ZLS3077X_BandwidthE Bandwidth)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xBandwidthSet: Bandwidth=%d",
            Bandwidth, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_BANDWIDTH(Bandwidth);
    }

    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetupMailboxForWrite(hwParams, ZLS3077X_MB_DPLL, pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Write(zl303xx_Params, NULL,
                            ZLS3077X_DPLLX_BW_FIXED_REG,
                            Bandwidth & ZL303XX_DPLL_77X_BANDWIDTH_MASK);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetMailboxSemWrite(hwParams, ZLS3077X_MB_DPLL);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xUpdateMailboxCopy(hwParams, ZLS3077X_MB_DPLL, pllId);
    }

    return status;
}


/* zl303xx_Dpll77xBandwidthGet */
/**
   Gets the bandwidth register value for a DPLL.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.
   [out]  Bandwidth Pointer to bandwidth register value

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xBandwidthGet(void *hwParams, ZLS3077X_BandwidthE *Bandwidth)
{
    zlStatusE status = ZL303XX_OK;
    osStatusT osStatus = OS_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xBandwidthGet: ",
            0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(Bandwidth);
    }

    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        osStatus = OS_MUTEX_TAKE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xBandwidthGet: OS_MUTEX_TAKE failed: %d",
                    status, 0, 0, 0,0,0);
        }
    }
    if (status == ZL303XX_OK)
    {
        *Bandwidth = (ZLS3077X_BandwidthE)(zl303xx_Params->pllParams.d77x.dpllMb[pllId].
                                    dpll_bw_fixed & ZL303XX_DPLL_77X_BANDWIDTH_MASK);
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xBandwidthGet: *Bandwidth=%d",
                *Bandwidth, 0,0,0,0,0);
    }
    if (status == ZL303XX_OK)
    {
        osStatus = OS_MUTEX_GIVE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_GIVE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xBandwidthGet: OS_MUTEX_GIVE failed: %d",
                    status, 0, 0, 0,0,0);
        }
    }

    return status;
}




/* zl303xx_Dpll77xBandwidthCustomSet */
/**
   Sets the custom bandwidth register value for a DPLL.

  Parameters:
   [in]   hwParams               Pointer to a device parameter structure.
   [in]   BandwidthCustom        The bandwidth in register form i.e.
                                    BandwidthCustom = round(32 * log(BW * 10^4))

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xBandwidthCustomSet(void *hwParams,
                                                Uint32T BandwidthCustom)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;
    Uint32T v;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xBandwidthCustomSet: BandwidthCustom=%d",
            BandwidthCustom, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        v = BandwidthCustom;
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetupMailboxForWrite(hwParams, ZLS3077X_MB_DPLL, pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Write(zl303xx_Params, NULL,
                            ZLS3077X_DPLLX_BW_VAR_REG,
                            v);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetMailboxSemWrite(hwParams, ZLS3077X_MB_DPLL);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xUpdateMailboxCopy(hwParams, ZLS3077X_MB_DPLL, pllId);
    }

    return status;
}


/* zl303xx_Dpll77xBandwidthCustomGet */
/**
   Gets the custom bandwidth register value for a DPLL.

  Parameters:
   [in]   hwParams        Pointer to a device parameter structure.
   [out]  BandwidthCustom Pointer to custom bandwidth register value

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xBandwidthCustomGet(void *hwParams,
                                                Uint32T *BandwidthCustom)
{
    zlStatusE status = ZL303XX_OK;
    osStatusT osStatus = OS_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xBandwidthCustomGet: ",
            0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTERS(hwParams, BandwidthCustom);
    }

    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        osStatus = OS_MUTEX_TAKE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xBandwidthCustomGet: OS_MUTEX_TAKE failed: %d",
                    status, 0, 0, 0,0,0);
        }
    }
    if (status == ZL303XX_OK)
    {
        *BandwidthCustom = zl303xx_Params->pllParams.d77x.dpllMb[pllId].dpll_bw_var;
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xBandwidthCustomGet: *BandwidthCustom=%d",
                *BandwidthCustom, 0,0,0,0,0);
    }
    if (status == ZL303XX_OK)
    {
        osStatus = OS_MUTEX_GIVE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_GIVE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xBandwidthCustomGet: OS_MUTEX_GIVE failed: %d",
                    status, 0, 0, 0,0,0);
        }
    }

    return status;
}


/* zl303xx_Dpll77xAssignOutputClockFreq */
/** 

    Populate the frequencies  (msdiv and lsdiv) for the given HP output

  Parameters:
   [in]   hpOutput       The output
   [in]   hpMux          The synth that drives this output
   [in]   hpMsdiv        The msdiv value
   [in]   hpLsdiv        The lsdiv value
   [in]   hpLsctrl       The final choice of source register
   [out]  conf[]         Table of synthesizer/output data

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
static zlStatusE zl303xx_Dpll77xAssignOutputClockFreq(zl303xx_Dpll77xOutputConfS *conf,
                                            ZLS3077X_OutputsE hpOutput,
                                            Uint32T hpMux,
                                            Uint32T hpMsdiv,
                                            Uint32T hpLsdiv,
                                            Uint32T hpLsctrl)
{
    zlStatusE status = ZL303XX_OK;


    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(conf);
    }
    if (status == ZL303XX_OK)
    {
        if( hpMsdiv == 0 )
        {
            conf->freq[hpOutput].msdivFreq = 0xffffffff;
            conf->freq[hpOutput].lsdivFreq = 0xffffffff;
        }
        else
        {
            switch(hpMux & 0x3)
            {
                case 0x0:
                    conf->freq[hpOutput].msdivFreq = conf->HPSynth1IntFreq / hpMsdiv;
                    break;
                case 0x1:
                    conf->freq[hpOutput].msdivFreq = conf->HPSynth1FracFreq / hpMsdiv;
                    break;
                case 0x2:
                    conf->freq[hpOutput].msdivFreq = conf->HPSynth2IntFreq / hpMsdiv;
                    break;
                case 0x3:
                    conf->freq[hpOutput].msdivFreq = conf->HPSynth2FracFreq / hpMsdiv;
                    break;
                default:
                    break;
            }
            if( hpLsdiv == 0 )
            {
                /* This is ok and conf->freq[hpOutput].lsdivFreq will be assigned
                   with the check of hpLsctrl below. But we can't do the following
                   calculation if hpLsdiv == 0. */
            }
            else
            {
                switch(hpMux & 0x3)
                {
                    case 0x0:
                        conf->freq[hpOutput].lsdivFreq = conf->freq[hpOutput].msdivFreq / hpLsdiv;
                        break;
                    case 0x1:
                        conf->freq[hpOutput].lsdivFreq = conf->freq[hpOutput].msdivFreq / hpLsdiv;
                        break;
                    case 0x2:
                        conf->freq[hpOutput].lsdivFreq = conf->freq[hpOutput].msdivFreq / hpLsdiv;
                        break;
                    case 0x3:
                        conf->freq[hpOutput].lsdivFreq = conf->freq[hpOutput].msdivFreq / hpLsdiv;
                        break;
                    default:
                        break;
                }
            }
            /* Change the MSDIV if necessary */
            if(( hpLsctrl & 0x10 ) == 0 )
            {
                /* HPOUTx is sourced from MSDIV - leave the value */
            }
            else
            {
                /* HPOUTx is sourced from LSDIV - change HPOUTxP to the MSDIV value */
                conf->freq[hpOutput].msdivFreq = conf->freq[hpOutput].lsdivFreq;
            }
            /* Change the LSDIV if necessary */
            if(( hpLsctrl & 0x20 ) == 0 )
            {
                /* HPOUTxN is the same as from HPOUTxP */
                conf->freq[hpOutput].lsdivFreq = conf->freq[hpOutput].msdivFreq;
            }
            else
            {
                /* HPOUTx is sourced from LSDIV - leave it */
            }
        }
    }

    return status;
}


/* zl303xx_Dpll77xGetOutputClockConfig */
/** 

    Returns useful output configuration data. In particular, this routine
    returns:
    - all synthesizers/output combinations
    - the output frequency of each synthesizer/divider

  Parameters:
   [in]   hwParams      Pointer to a device parameter structure.
   [out]  conf[]        Table of synthesizer/output data

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xGetOutputClockConfig(void *hwParams,
                                            zl303xx_Dpll77xOutputConfS *conf)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T gpFreqBase;
    Uint32T gpFreqMult;
    Uint32T gpFreqM;
    Uint32T gpFreqN;
    Uint32T gpOutDiv0;
    Uint32T gpOutDiv1;
    Uint32T hpFreqBase;
    Uint32T hpFreqM;
    Uint32T hpFreqN;
    Uint32T hpIntDiv;
    Uint32T hpInt;
    Uint32T hpSynthIntDivToInt[] = {40, 45, 50, 55, 60, 65, 70, 75, 80, 90, 100, 110, 120, 130, 140, 150};
    Uint32T hpFdivBase;
    Uint32T hpFdivN;
    Uint32T hpFdivD;
    Uint32T hpMux;
    Uint32T hpMsdiv;
    Uint32T hpLsdiv;
    Uint32T hpLsctrl;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xGetOutputClockConfig:",
            0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(conf);
    }

    /* Get the GP data */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_GP_FREQ_BASE_REG, &gpFreqBase);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_GP_FREQ_MULT_REG, &gpFreqMult);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_GP_FREQ_M_REG, &gpFreqM);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_GP_FREQ_N_REG, &gpFreqN);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_GP_OUT_DIV_0_REG, &gpOutDiv0);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_GP_OUT_DIV_1_REG, &gpOutDiv1);
    }
    if (status == ZL303XX_OK)
    {
        conf->GPOUT0Freq = (Uint64T)gpFreqBase *
                           (Uint64T)gpFreqMult *
                           (Uint64T)gpFreqM /
                           (Uint64T)gpFreqN /
                           (Uint64T)gpOutDiv0;
        conf->GPOUT1Freq = (Uint64T)gpFreqBase *
                           (Uint64T)gpFreqMult *
                           (Uint64T)gpFreqM /
                           (Uint64T)gpFreqN /
                           (Uint64T)gpOutDiv1;
    }

    /* Get synth 1 data */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_FREQ_BASE_1_REG, &hpFreqBase);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_FREQ_M_1_REG, &hpFreqM);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_FREQ_N_1_REG, &hpFreqN);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_HSDIV_1_REG, &hpIntDiv);
    }
    if (status == ZL303XX_OK)
    {
        hpInt = hpSynthIntDivToInt[(hpIntDiv & 0xf)];
        /* We multiply by 10 because hpInt is 10x bigger to accomodate for the
           .5s of the register value. */
        conf->HPSynth1IntFreq = (Uint64T)(Uint64T)10 * hpFreqBase *
                                (Uint64T)hpFreqM / (Uint64T)hpFreqN / (Uint64T)hpInt;
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_FDIV_BASE_1_REG, &hpFdivBase);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_FDIV_NUM_1_REG, &hpFdivN);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_FDIV_DEV_1_REG, &hpFdivD);
    }
    if (status == ZL303XX_OK)
    {
        conf->HPSynth1FracFreq = (Uint64T)hpFdivBase * (Uint64T)hpFdivN / (Uint64T)hpFdivD;
    }

    /* Get synth 2 data */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_FREQ_BASE_2_REG, &hpFreqBase);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_FREQ_M_2_REG, &hpFreqM);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_FREQ_N_2_REG, &hpFreqN);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_HSDIV_2_REG, &hpIntDiv);
    }
    if (status == ZL303XX_OK)
    {
        hpInt = hpSynthIntDivToInt[(hpIntDiv & 0xf)];
        /* We multiply by 10 because hpInt is 10x bigger to accomodate for the
           .5s of the register value. */
        conf->HPSynth2IntFreq = (Uint64T)(Uint64T)10 * (Uint64T)hpFreqBase *
                                (Uint64T)hpFreqM / (Uint64T)hpFreqN / (Uint64T)hpInt;
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_FDIV_BASE_2_REG, &hpFdivBase);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_FDIV_NUM_2_REG, &hpFdivN);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_FDIV_DEV_2_REG, &hpFdivD);
    }
    if (status == ZL303XX_OK)
    {
        conf->HPSynth2FracFreq = (Uint64T)hpFdivBase * (Uint64T)hpFdivN / (Uint64T)hpFdivD;
    }

    /* Get the frequency data for all 8 HP outputs  */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_MUX_REG, &hpMux);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_MSDIV_0_REG, &hpMsdiv);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_LSDIV_0_REG, &hpLsdiv);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_LSCTRL_0_REG, &hpLsctrl);
        status |= zl303xx_Dpll77xAssignOutputClockFreq(conf, ZLS3077X_HPOUT0,
                                            ((hpMux >> 0) & 0x3), hpMsdiv, hpLsdiv, hpLsctrl );
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_MSDIV_1_REG, &hpMsdiv);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_LSDIV_1_REG, &hpLsdiv);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_LSCTRL_1_REG, &hpLsctrl);
        status |= zl303xx_Dpll77xAssignOutputClockFreq(conf, ZLS3077X_HPOUT1,
                                            ((hpMux >> 0) & 0x3), hpMsdiv, hpLsdiv, hpLsctrl );
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_MSDIV_2_REG, &hpMsdiv);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_LSDIV_2_REG, &hpLsdiv);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_LSCTRL_2_REG, &hpLsctrl);
        status |= zl303xx_Dpll77xAssignOutputClockFreq(conf, ZLS3077X_HPOUT2,
                                            ((hpMux >> 2) & 0x3), hpMsdiv, hpLsdiv, hpLsctrl );
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_MSDIV_3_REG, &hpMsdiv);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_LSDIV_3_REG, &hpLsdiv);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_LSCTRL_3_REG, &hpLsctrl);
        status |= zl303xx_Dpll77xAssignOutputClockFreq(conf, ZLS3077X_HPOUT3,
                                            ((hpMux >> 2) & 0x3), hpMsdiv, hpLsdiv, hpLsctrl );
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_MSDIV_4_REG, &hpMsdiv);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_LSDIV_4_REG, &hpLsdiv);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_LSCTRL_4_REG, &hpLsctrl);
        status |= zl303xx_Dpll77xAssignOutputClockFreq(conf, ZLS3077X_HPOUT4,
                                            ((hpMux >> 4) & 0x3), hpMsdiv, hpLsdiv, hpLsctrl );
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_MSDIV_5_REG, &hpMsdiv);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_LSDIV_5_REG, &hpLsdiv);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_LSCTRL_5_REG, &hpLsctrl);
        status |= zl303xx_Dpll77xAssignOutputClockFreq(conf, ZLS3077X_HPOUT5,
                                            ((hpMux >> 4) & 0x3), hpMsdiv, hpLsdiv, hpLsctrl );
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_MSDIV_6_REG, &hpMsdiv);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_LSDIV_6_REG, &hpLsdiv);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_LSCTRL_6_REG, &hpLsctrl);
        status |= zl303xx_Dpll77xAssignOutputClockFreq(conf, ZLS3077X_HPOUT6,
                                            ((hpMux >> 6) & 0x3), hpMsdiv, hpLsdiv, hpLsctrl );
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_MSDIV_7_REG, &hpMsdiv);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_LSDIV_7_REG, &hpLsdiv);
        status |= zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_HP_OUT_LSCTRL_7_REG, &hpLsctrl);
        status |= zl303xx_Dpll77xAssignOutputClockFreq(conf, ZLS3077X_HPOUT7,
                                            ((hpMux >> 6) & 0x3), hpMsdiv, hpLsdiv, hpLsctrl );
    }

    return status;
}


/* zl303xx_Dpll77xPhaseStepMaskSet */
/**

    Sets 1 output clock to be moved by stepTime().

    Typically, one output clock will have a frequency of 1pps and be an input
    to a timestamper.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xPhaseStepMaskSet(void *hwParams,
                                            ZLS3077X_OutputTypesE outputType,
                                            ZLS3077X_OutputsE outputNum,
                                            Uint32T v)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint8T psm;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xPhaseStepMaskSetHP: ", 0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        switch( outputType )
        {
            case ZLS3077X_OUTPUT_TYPE_GP:
                if( outputNum >= ZLS3077X_NUM_GP_OUTPUTS )
                {
                    ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xPhaseStepMaskSet: bad gp Output value",
                            0,0,0,0,0,0);
                    status = ZL303XX_PARAMETER_INVALID;
                }
                break;

            case ZLS3077X_OUTPUT_TYPE_HP:
                if( outputNum >= ZLS3077X_NUM_HP_OUTPUTS )
                {
                    ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xPhaseStepMaskSet: bad hp Output value",
                            0,0,0,0,0,0);
                    status = ZL303XX_PARAMETER_INVALID;
                }
                break;

            default:
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xPhaseStepMaskSet: bad output type value",
                        0,0,0,0,0,0);
                status = ZL303XX_PARAMETER_INVALID;
                break;
        }
    }
    if (status == ZL303XX_OK)
    {
        if( v > 1 )
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xPhaseStepMaskSet: bad set value",
                    0,0,0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
        }
    }
    if (status == ZL303XX_OK)
    {
        if( outputType == ZLS3077X_OUTPUT_TYPE_GP )
        {
            psm = zl303xx_Params->pllParams.d77x.std.phaseStepMaskGp;
        }
        if( outputType == ZLS3077X_OUTPUT_TYPE_HP )
        {
            psm = zl303xx_Params->pllParams.d77x.std.phaseStepMaskHp;
        }

        /* Clear the output bit */
        psm = psm & (~(1 << outputNum));

        /* Load the output bit with the passed-in value */
        psm = psm | ((v & 0x1) << outputNum);

        if( outputType == ZLS3077X_OUTPUT_TYPE_GP )
        {
            zl303xx_Params->pllParams.d77x.std.phaseStepMaskGp = psm;
        }
        if( outputType == ZLS3077X_OUTPUT_TYPE_HP )
        {
            zl303xx_Params->pllParams.d77x.std.phaseStepMaskHp = psm;
        }
    }

    return status;
}


/* zl303xx_Dpll77xOutputThatDrivesTimeStamperSet */
/**

    Sets 1 output clock to be moved by stepTime().

    Typically, one output clock will have a frequency of 1pps and be an input
    to a timestamper.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xOutputThatDrivesTimeStamperSet(void *hwParams,
                                            ZLS3077X_OutputTypesE outputType,
                                            ZLS3077X_OutputsE outputNum)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xOutputThatDrivesTimeStamperSet: ", 0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        switch( outputType )
        {
            case ZLS3077X_OUTPUT_TYPE_GP:
                if( outputNum >= ZLS3077X_NUM_GP_OUTPUTS )
                {
                    ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xOutputThatDrivesTimeStamperSet: bad gpOutput value",
                            0,0,0,0,0,0);
                    status = ZL303XX_PARAMETER_INVALID;
                }
                break;

            case ZLS3077X_OUTPUT_TYPE_HP:
                if( outputNum >= ZLS3077X_NUM_HP_OUTPUTS )
                {
                    ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xOutputThatDrivesTimeStamperSet: bad hpOutput value",
                            0,0,0,0,0,0);
                    status = ZL303XX_PARAMETER_INVALID;
                }
                break;

            default:
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xOutputThatDrivesTimeStamperSet: bad output type value",
                        0,0,0,0,0,0);
                status = ZL303XX_PARAMETER_INVALID;
                break;
        }
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_Params->pllParams.d77x.std.outputTypeThatDrivesTimeStamper = outputType;
        zl303xx_Params->pllParams.d77x.std.outputNumThatDrivesTimeStamper = outputNum;
    }

    return status;
}


/* zl303xx_Dpll77xPhaseStepMaskGet */
/**
   Returns the mask that will be used to determine which HP outputs that a
   phase_step operation will be applied to.

  Parameters:
   [in]   hwParams  Pointer to a device parameter structure.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xPhaseStepMaskGet(void *hwParams,
                        Uint32T *phaseStepMaskGp,
                        Uint32T *phaseStepMaskHp,
                        ZLS3077X_OutputTypesE *outputTypeThatDrivesTimeStamper,
                        ZLS3077X_OutputsE *outputNumThatDrivesTimeStamper)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xPhaseStepMaskGet: ", 0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(phaseStepMaskGp);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(phaseStepMaskHp);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(outputTypeThatDrivesTimeStamper);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(outputNumThatDrivesTimeStamper);
    }

    if (status == ZL303XX_OK)
    {
        *phaseStepMaskGp = zl303xx_Params->pllParams.d77x.std.phaseStepMaskGp;
        *phaseStepMaskHp = zl303xx_Params->pllParams.d77x.std.phaseStepMaskHp;
        *outputTypeThatDrivesTimeStamper = zl303xx_Params->pllParams.d77x.std.outputTypeThatDrivesTimeStamper;
        *outputNumThatDrivesTimeStamper = zl303xx_Params->pllParams.d77x.std.outputNumThatDrivesTimeStamper;
    }

    return status;
}


/* zl303xx_Dpll77xModeSwitchFastLockEnableSet */
/**

   Sets the bit that controls whether fast lock is forcibly disabled during
   transitions out of NCO mode.

  Parameters:
   [in]   hwParams   Pointer to a device parameter structure.
   [in]   nco        TRUE = Fast-lock is allowed during transitions out
                        of NCO mode. NOTE: master must also be TRUE
   [in]   force      TRUE = force-enable the fast-lock feature; will
                        ignore the outputs of the frequency and phase error
                        monitors. NOTE: master must also be TRUE
   [in]   en         TRUE enables fast lock.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xModeSwitchFastLockEnableSet(void *hwParams,
                zl303xx_BooleanE nco, zl303xx_BooleanE force, zl303xx_BooleanE en)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;
    Uint32T v;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xModeSwitchFastLockEnableSet: nco=%d  force=%d  en=%d",
            nco, force, en, 0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_BOOLEAN(nco);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_BOOLEAN(force);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_BOOLEAN(en);
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetupMailboxForWrite(hwParams,
                                                ZLS3077X_MB_DPLL, pllId);
    }
    if (status == ZL303XX_OK)
    {
        v  = (nco == ZL303XX_TRUE)    ? (1 << ZLS3077X_FAST_LOCK_NCO_ENABLE_MASK)    : 0;
        v |= (force == ZL303XX_TRUE)  ? (1 << ZLS3077X_FAST_LOCK_FORCE_ENABLE_MASK)  : 0;
        v |= (en == ZL303XX_TRUE)     ? (1 << ZLS3077X_FAST_LOCK_ENABLE_MASK)        : 0;

        status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                                ZLS3077X_DPLLX_FAST_LOCK_CTRL_REG,
                                v, ZLS3077X_FAST_LOCK_MASK, NULL);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetMailboxSemWrite(hwParams, ZLS3077X_MB_DPLL);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xUpdateMailboxCopy(hwParams, ZLS3077X_MB_DPLL, pllId);
    }

    return status;
}


/* zl303xx_Dpll77xInputPWMSet */
/**
   Sets the mode (PWM) register value for a reference.

  Parameters:
   [in]   hwParams   Pointer to a device parameter structure.
   [in]   refId      The reference
   [in]   mode       The mode

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xInputPWMSet(void *hwParams, Uint32T refId,
                                    ZLS3077X_RefSynchPairModeE mode)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xInputPWMSet: refId=%d  mode=%d",
            refId, mode, 0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_REF_ID(refId);
    }
    if (status == ZL303XX_OK)
    {
        if(( mode == ZLS3077X_RSPM_enabled ) || ( mode >= ZLS3077X_RSPM_last ))
        {
            status = ZL303XX_PARAMETER_INVALID;
        }
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetupMailboxForWrite(hwParams, ZLS3077X_MB_ref, refId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                            ZLS3077X_REFX_SYNC_REG,
                            mode, ZLS3077X_REF_CLK_SYNC_MODE_MASK, NULL);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xSetMailboxSemWrite(hwParams, ZLS3077X_MB_ref);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xUpdateMailboxCopy(hwParams, ZLS3077X_MB_ref, refId);
    }

    return status;
}
/* zl303xx_Dpll77xInputPWMGet */
/**
   Gets the mode (PWM) register value for a reference.

  Parameters:
   [in]   hwParams   Pointer to a device parameter structure.
   [in]   refId      The reference

   [out]  mode       The mode

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xInputPWMGet(void *hwParams, Uint32T refId,
                                    ZLS3077X_RefSynchPairModeE *mode)
{
    zlStatusE status = ZL303XX_OK;
    osStatusT osStatus = OS_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xInputPWMGet: refId=%d ",
            refId,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_REF_ID(refId);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(mode);
    }

    if (status == ZL303XX_OK)
    {
        osStatus = OS_MUTEX_TAKE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xInputPWMGet: OS_MUTEX_TAKE failed: %d",
                    status, 0, 0, 0,0,0);
        }
    }
    if (status == ZL303XX_OK)
    {
        *mode = (zl303xx_Params->pllParams.d77x.refMb[refId].ref_sync &
                        ZLS3077X_REF_CLK_SYNC_MODE_MASK);
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xInputPWMGet: *mode=%d",
                *mode, 0,0,0,0,0);
    }
    if (status == ZL303XX_OK)
    {
        osStatus = OS_MUTEX_GIVE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_GIVE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xInputPWMGet: OS_MUTEX_GIVE failed: %d",
                    status, 0, 0, 0,0,0);
        }
    }

    return status;
}


/* zl303xx_Dpll77xNCOAutoReadSet */
/**
   Sets the NCO auto read register value for a dpll.

  Parameters:
   [in]   hwParams   Pointer to a device parameter structure.
   [in]   enabled    enabled/disabled

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xNCOAutoReadSet(void *hwParams, zl303xx_BooleanE enabled)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xNCOAutoReadSet: enabled=%d",
            enabled, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                            ZLS3077X_DPLL_NCO_AUTO_READ_REG(pllId),
                            enabled << ZLS3077X_DPLL_NCO_AUTO_READ_SHIFT,
                            ZLS3077X_DPLL_NCO_AUTO_READ_MASK << ZLS3077X_DPLL_NCO_AUTO_READ_SHIFT,
                            NULL);
    }

    return status;
}
/* zl303xx_Dpll77xNCOAutoReadGetData */
/**
   Reads the last saved frequency offset value of NCO auto read feature.

   NOTE: The following assumes the dcoOffset will not exceed 32 bits

  Parameters:
   [in]   hwParams           Pointer to a device parameter structure.
   [out]  freqOffsetInppt    Pointer to the frequency offset

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xNCOAutoReadGetData(void *hwParams, Sint32T *freqOffsetInppt)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;
    Uint32T dcoOffset = 0;
    zl303xx_BooleanE isNegative = ZL303XX_FALSE;
    Uint8T round;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xNCOAutoReadGetData:",
            0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(freqOffsetInppt);
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        /* Read the DCO offset from the device register (MSB byte 5) */
        status = zl303xx_Read(hwParams, NULL,
                           ZLS3077X_NCO_FREQ_OFFSET_WR_HI_DPLL(pllId),
                           &dcoOffset);
    }
    if (status == ZL303XX_OK)
    {
        /* Check the high order bit to determine sign */
        if (dcoOffset & 0x8000)
        {
            isNegative = ZL303XX_TRUE;
        }

        /* Read the DCO offset from the device register */
        status = zl303xx_Read(hwParams, NULL,
                         ZLS3077X_NCO_FREQ_OFFSET_WR_LO_DPLL(pllId),
                         &dcoOffset);
    }
    if (status == ZL303XX_OK)
    {
        /* Convert to positive numbers for the math library */
        if (isNegative)
        {
            dcoOffset = ~dcoOffset + 1;
        }

        *freqOffsetInppt = Mult_Mult_Div_U32((Uint32T)dcoOffset,
                         1000000,  /* ONE_MILLION */
                         ZLS3077X_DCO_COUNT_PER_PPM,
                         &round, NULL);
        *freqOffsetInppt += round;

        /* Convert for the original signage of the input variable */
        /* For 77X dco offset has a reversed sign, so this logic is
           inverted */
        if (isNegative != ZL303XX_TRUE)
        {
            *freqOffsetInppt = -(*freqOffsetInppt);
        }
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xNCOAutoReadGetData: *freqOffsetInppt=%d",
                *freqOffsetInppt, 0,0,0,0,0);
    }

    return status;
}


/* zl303xx_Dpll77xPhaseStepMaxSet */
/**
   Sets phase step max register.

  Parameters:
   [in]   hwParams   Pointer to a device parameter structure.
   [in]   pwm        The phase step max: 1-49

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xPhaseStepMaxSet(void *hwParams, Uint32T psm)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xPhaseStepMaxSet: psm=%d",
            psm, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        if(( psm < 1 ) || ( psm > 49 ))
        {
            status = ZL303XX_PARAMETER_INVALID;
        }
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Write(hwParams, NULL,
                             ZLS3077X_PHASE_STEP_MAX_REG,
                             psm);
    }

    return status;
}


/* zl303xx_Dpll77xOutputPhaseStepStatusGet */
/**

    Returns the phase_step (complete) status for the output that is driving
    the timestamper.

  Parameters:
   [in]   hwParams     Pointer to a device parameter structure.
   [out]  complete     Pointer to the completion status

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xOutputPhaseStepStatusGet(void *hwParams, Uint32T *complete)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T data;
    Uint32T phaseStepMaskGp;
    Uint32T phaseStepMaskHp;
    ZLS3077X_OutputTypesE outputTypeThatDrivesTimeStamper;
    ZLS3077X_OutputsE outputNumThatDrivesTimeStamper;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xOutputPhaseStepStatusGet: ",
            0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(complete);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xPhaseStepMaskGet(hwParams,
                                            &phaseStepMaskGp,
                                            &phaseStepMaskHp,
                                            &outputTypeThatDrivesTimeStamper,
                                            &outputNumThatDrivesTimeStamper);
    }
    if (status == ZL303XX_OK)
    {
        if( outputTypeThatDrivesTimeStamper == ZLS3077X_OUTPUT_TYPE_GP )
        {
            status = zl303xx_Read(zl303xx_Params, NULL,
                                ZLS3077X_GP_OUT_PHASE_STEP_STICKY_REG,
                                &data);
        }
        if( outputTypeThatDrivesTimeStamper == ZLS3077X_OUTPUT_TYPE_HP )
        {
            status = zl303xx_Read(zl303xx_Params, NULL,
                                ZLS3077X_HP_OUT_PHASE_STEP_STICKY_REG,
                                &data);
        }
    }
    if (status == ZL303XX_OK)
    {
        if( outputTypeThatDrivesTimeStamper == ZLS3077X_OUTPUT_TYPE_GP )
        {
            *complete = (data & phaseStepMaskGp) > 0;
            ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xOutputPhaseStepStatusGet: *complete=%d",
                    *complete, 0, 0,0,0,0);
        }
        if( outputTypeThatDrivesTimeStamper == ZLS3077X_OUTPUT_TYPE_HP )
        {
            *complete = (data & phaseStepMaskHp) > 0;
            ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xOutputPhaseStepStatusGet: *complete=%d",
                    *complete, 0, 0,0,0,0);
        }
    }

    return status;
}


/* zl303xx_Dpll77xWaitForPhaseCtrlReady */
/** 
    Waits for the phase step mechanism to become available.

  Parameters:
   [in]   hwParams     Pointer to the device parameter structure.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_Dpll77xWaitForPhaseCtrlReady(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T phaseStepBusy = 0;
    Uint16T attempts = 0;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xWaitForPhaseCtrlReady: ",
            0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }

    if (status == ZL303XX_OK)
    {
        /* Verify hardware is ready for phase operation */
        do
        {
            status = zl303xx_Read(zl303xx_Params, NULL,
                              ZLS3077X_PHASE_STEP_CTRL_REG,
                              &phaseStepBusy);

            if (status == ZL303XX_OK &&
               ((phaseStepBusy & ZLS3077X_PHASE_STEP_CTRL_MASK) != ZLS3077X_PHASE_STEP_CTRL_READY))
            {
                /* Hardware is busy with another phase operation, wait a while */
                OS_TASK_DELAY(ZLS3077X_REG_READ_INTERVAL);
            }

            /* Only delay up to, very roughly, mediumSanityTO */
            if (++attempts > (Dpll77xShortSanityTO/ZLS3077X_REG_READ_INTERVAL))
            {
                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xWaitForPhaseCtrlReady: timeout!",
                        0,0,0,0,0,0);
                status = ZL303XX_TIMEOUT;
            }

        } while (status == ZL303XX_OK &&
               ((phaseStepBusy & ZLS3077X_PHASE_STEP_CTRL_MASK) != ZLS3077X_PHASE_STEP_CTRL_READY));
    }

    return status;
}


/* zl303xx_Dpll77xStepTimePhaseGet */
/**

    Returns the cumulative zl303xx_Dpll77xOutputPhaseStepWrite() phase from the
    DPLL.

  Parameters:
   [in]   hwParams      Pointer to the zl303xx device data.
   [out]  phaseNs       Cumulative NCO phase applied, in
                             nanoseconds.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_Dpll77xStepTimePhaseGet(void *hwParams, Sint32T *phaseNs)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;
    Uint32T n;
    Sint64T T64_n;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xStepTimePhaseGet: ",
            0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        ZL303XX_CHECK_POINTERS(hwParams, phaseNs);
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    /* Wait for device to be ready for a TOD step read */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xWaitForPhaseCtrlReady(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Write(zl303xx_Params, NULL,
                            ZLS3077X_PHASE_STEP_CTRL_REG,
                            ((pllId << 4) |
                            ZLS3077X_PHASE_STEP_CTRL_TOD_STEP |
                            ZLS3077X_PHASE_STEP_CTRL_READ));
    }
    /* Wait for device to populate the data register */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xWaitForPhaseCtrlReady(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL,
                            ZLS3077X_PHASE_STEP_DATA_REG,
                            &n);
    }
    if (status == ZL303XX_OK)
    {
        T64_n = (Sint32T)n;
        T64_n = T64_n * 10 / 8;
        *phaseNs = (Sint32T)T64_n;
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xStepTimePhaseGet: *phaseNs=%d",
                *phaseNs, 0, 0,0,0,0);
    }

    return status;
}


/* zl303xx_Dpll77xStepTimeClearOutputStickyBits */
/**
    Clears the phase step sticky bits for the outputs that are being stepped.

  Parameters:
   [in]   hwParams  Pointer to the zl303xx device data.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_Dpll77xStepTimeClearOutputStickyBits(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    zlStatusE stickyStatus = ZL303XX_ERROR;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T phaseStepMaskGp;
    Uint32T phaseStepMaskHp;
    ZLS3077X_OutputTypesE outputTypeThatDrivesTimeStamper;
    ZLS3077X_OutputsE outputNumThatDrivesTimeStamper;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xStepTimeClearOutputStickyBits:",
            0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }

    /* Get the mask we use to clear the sticky bits and test if the phase step
       is done */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xPhaseStepMaskGet(hwParams,
                                            &phaseStepMaskGp,
                                            &phaseStepMaskHp,
                                            &outputTypeThatDrivesTimeStamper,
                                            &outputNumThatDrivesTimeStamper);
    }
    /* Clear the sticky bits */
    if (status == ZL303XX_OK)
    {
        stickyStatus = zl303xx_Dpll77xStickyLockSet(hwParams, ZLS3077X_STICKY_LOCK);
        status = stickyStatus;
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                        ZLS3077X_GP_OUT_PHASE_STEP_STICKY_REG,
                        0x00, phaseStepMaskGp, NULL);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                        ZLS3077X_HP_OUT_PHASE_STEP_STICKY_REG,
                        0x00, phaseStepMaskHp, NULL);
    }
    if (stickyStatus == ZL303XX_OK)
    {
        stickyStatus = zl303xx_Dpll77xStickyLockSet(hwParams, ZLS3077X_STICKY_UNLOCK);
        status = (status == ZL303XX_OK) ? stickyStatus : status;
    }

    return status;
}


/* zl303xx_Dpll77xOutputPhaseErrorClear */
/**

   Remove output phase from a specific synth and postdivider in clock cycles.

  Parameters:
   [in]   hwParams       Pointer to a device parameter structure.
   [in]   phaseToRemove  In clockCycles (-ve will add).
   [in]   clockFreq      Current input freq. (if working in cycles).

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xOutputPhaseErrorClear(void *hwParams, Sint32T phaseToRemove,
                                                Uint32T clockFreq)
{
   return zl303xx_Dpll77xOutputPhaseStepWrite(hwParams, phaseToRemove,
                                            ZL303XX_TRUE, clockFreq);
}


/* zl303xx_Dpll77xGetLSDIV */
/**

    Return the current value of LSDIV for the HP output that drives the
    timestamper.

    If a GP is driving the timestamper, then this routine returns the LSDIV
    of the 1st HP that will be steped.

  Parameters:
   [in]   hwParams       Pointer to a device parameter structure.

   [out]  lsdiv          The LSDIV value

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
static zlStatusE zl303xx_Dpll77xGetLSDIV(void *hwParams, Uint32T *lsdiv)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Sint32T reg;
    Uint32T phaseStepMaskGp;
    Uint32T phaseStepMaskHp;
    ZLS3077X_OutputTypesE outputTypeThatDrivesTimeStamper;
    ZLS3077X_OutputsE outputNumThatDrivesTimeStamper;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xGetLSDIV: ",
            0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(lsdiv);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xPhaseStepMaskGet(hwParams,
                                            &phaseStepMaskGp,
                                            &phaseStepMaskHp,
                                            &outputTypeThatDrivesTimeStamper,
                                            &outputNumThatDrivesTimeStamper);
    }
    if (status == ZL303XX_OK)
    {
        switch( outputTypeThatDrivesTimeStamper )
        {
            case ZLS3077X_OUTPUT_TYPE_GP:
            {
                /* A GP output drives the timestamper but we still need to provide
                   a lsdiv value in case the user is moving an HP as well. This
                   code chooses the 1st hp lsdiv value. If more HPs are being
                   steped, they should all have the same lsdiv. */
                if     (( phaseStepMaskHp & 0x01 ) > 0 ) reg = ZLS3077X_HP_OUT_LSDIV_0_REG;
                else if(( phaseStepMaskHp & 0x02 ) > 0 ) reg = ZLS3077X_HP_OUT_LSDIV_1_REG;
                else if(( phaseStepMaskHp & 0x04 ) > 0 ) reg = ZLS3077X_HP_OUT_LSDIV_2_REG;
                else if(( phaseStepMaskHp & 0x08 ) > 0 ) reg = ZLS3077X_HP_OUT_LSDIV_3_REG;
                else if(( phaseStepMaskHp & 0x10 ) > 0 ) reg = ZLS3077X_HP_OUT_LSDIV_4_REG;
                else if(( phaseStepMaskHp & 0x20 ) > 0 ) reg = ZLS3077X_HP_OUT_LSDIV_5_REG;
                else if(( phaseStepMaskHp & 0x40 ) > 0 ) reg = ZLS3077X_HP_OUT_LSDIV_6_REG;
                else if(( phaseStepMaskHp & 0x80 ) > 0 ) reg = ZLS3077X_HP_OUT_LSDIV_7_REG;
                else
                {
                    /* The configuration is messed-up. We don't want to return
                       status=error but we need to make sure the returned lsdiv=0
                       so the caller can handle this situation. */
                    *lsdiv = 0;
                    reg = 0;
                    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                            "zl303xx_Dpll77xGetLSDIV: HP phase step mask miss-configured",
                            0,0,0,0,0,0);
                }

                break;
            }
            case ZLS3077X_OUTPUT_TYPE_HP:
            {
                switch( outputNumThatDrivesTimeStamper )
                {
                    case ZLS3077X_HPOUT0: reg = ZLS3077X_HP_OUT_LSDIV_0_REG; break;
                    case ZLS3077X_HPOUT1: reg = ZLS3077X_HP_OUT_LSDIV_1_REG; break;
                    case ZLS3077X_HPOUT2: reg = ZLS3077X_HP_OUT_LSDIV_2_REG; break;
                    case ZLS3077X_HPOUT3: reg = ZLS3077X_HP_OUT_LSDIV_3_REG; break;
                    case ZLS3077X_HPOUT4: reg = ZLS3077X_HP_OUT_LSDIV_4_REG; break;
                    case ZLS3077X_HPOUT5: reg = ZLS3077X_HP_OUT_LSDIV_5_REG; break;
                    case ZLS3077X_HPOUT6: reg = ZLS3077X_HP_OUT_LSDIV_6_REG; break;
                    case ZLS3077X_HPOUT7: reg = ZLS3077X_HP_OUT_LSDIV_7_REG; break;
                    default:
                        status = ZL303XX_DATA_CORRUPTION;
                        break;
                }

                break;
            }
            default:
            {
                status = ZL303XX_DATA_CORRUPTION;
                break;
            }
        }
    }
    if (status == ZL303XX_OK)
    {
        /* Only read lsdviv if we need to */
        if( reg != 0 )
        {
            status = zl303xx_Read(zl303xx_Params, NULL, reg, lsdiv);
        }
    }


    return status;
}


/* zl303xx_Dpll77xModifyStepTimeNs */
/**

    HP output stepTime()s can jump a discrete number of nanoseconds.

    If an HP output drives the timestamper, this routine returns the
    stepTime() size that will land on a discrete boundary.

    If a GP output drives the timestamper, this routine returns the
    passed-in deltaTime i.e. no modification is performed.

  Parameters:
   [in]   hwParams           Pointer to a device parameter structure.
   [in]   deltaTime          The stepTime() in nanoseconds

   [out]  deltaTimeModified  The actual stepTime() in ns

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xModifyStepTimeNs(void *hwParams, Sint32T deltaTime,
                                       Sint32T *modDeltaTime)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T lsdiv;
    Sint64T T64_deltaTime;
    Uint32T phaseStepMaskGp;
    Uint32T phaseStepMaskHp;
    ZLS3077X_OutputTypesE outputTypeThatDrivesTimeStamper;
    ZLS3077X_OutputsE outputNumThatDrivesTimeStamper;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xModifyStepTimeNs: deltaTime=%d",
            deltaTime, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(modDeltaTime);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xPhaseStepMaskGet(hwParams,
                                            &phaseStepMaskGp,
                                            &phaseStepMaskHp,
                                            &outputTypeThatDrivesTimeStamper,
                                            &outputNumThatDrivesTimeStamper);
    }
    if (status == ZL303XX_OK)
    {
        switch( outputTypeThatDrivesTimeStamper )
        {
            case ZLS3077X_OUTPUT_TYPE_GP:
            {
                /* A GP output is driving the timestamper. Do not modify the
                   step size. */
                *modDeltaTime = deltaTime;
                break;
            }
            case ZLS3077X_OUTPUT_TYPE_HP:
            {
                if (status == ZL303XX_OK)
                {
                    status = zl303xx_Dpll77xGetLSDIV(hwParams, &lsdiv);
                }
                if (status == ZL303XX_OK)
                {
                    if( lsdiv == 0 )
                    {
                        *modDeltaTime = deltaTime;
                    }
                    else
                    {
                        T64_deltaTime = deltaTime;
                        T64_deltaTime = (Sint64T)T64_deltaTime *
                                        (Sint64T)lsdiv / (Sint64T)1000000000 *
                                        (Sint64T)1000000000 / (Sint64T)lsdiv;
                        *modDeltaTime = (Sint32T)T64_deltaTime;
                    }
                }
                break;
            }
            default:
            {
                status = ZL303XX_DATA_CORRUPTION;
                break;
            }
        }
    }


    return status;
}


/* zl303xx_Dpll77xConvertStepTimeNsToRegisterUnits */
/**

    The GP and HP step times hardware registers take different resolutions:
    GP needs VCO cycles, HP needs MSDIV cycles.

    This routine converts the passed-in ns value to the correct units.

  Parameters:
   [in]   hwParams       Pointer to a device parameter structure.
   [in]   deltaTime      The stepTime() in nanoseconds
   [in]   outputType     The output type - either GP or HP

   [out]  deltaTimeRU    The stepTime() in the configured register units

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
static zlStatusE zl303xx_Dpll77xConvertStepTimeNsToRegisterUnits(void *hwParams,
                                Sint32T deltaTime, Sint32T *deltaTimeRU,
                                ZLS3077X_OutputTypesE outputType)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T lsdiv;
    
    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xConvertStepTimeNsToRegisterUnits: deltaTime=%d outputType=%d",
            deltaTime, outputType, 0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(deltaTimeRU);
    }
    if (status == ZL303XX_OK)
    {
        if( outputType == ZLS3077X_OUTPUT_TYPE_GP )
        {
            Uint32T Bs, Ks, Ms, Ns;
            Sint64T T64_vcoFreq;

            if (status == ZL303XX_OK)
            {
                status = zl303xx_Read(zl303xx_Params, NULL,
                                  ZLS3077X_GP_FREQ_BASE_REG,
                                  &Bs);
            }
            if (status == ZL303XX_OK)
            {
                status = zl303xx_Read(zl303xx_Params, NULL,
                                  ZLS3077X_GP_FREQ_MULT_REG,
                                  &Ks);
            }
            if (status == ZL303XX_OK)
            {
                status = zl303xx_Read(zl303xx_Params, NULL,
                                  ZLS3077X_GP_FREQ_M_REG,
                                  &Ms);
            }
            if (status == ZL303XX_OK)
            {
                status = zl303xx_Read(zl303xx_Params, NULL,
                                  ZLS3077X_GP_FREQ_N_REG,
                                  &Ns);
            }
            if (status == ZL303XX_OK)
            {
/*                Sint64T T64_deltaTime;		Fujitsu modify not use this param */
				char ptplog_path[] = "/proc/rru/ptplog";

/*                T64_deltaTime = deltaTime;		Fujitsu modify not use this param */

                T64_vcoFreq = (Sint64T)Bs * (Sint64T)Ks * (Sint64T)Ms / (Sint64T)Ns;
                *deltaTimeRU = deltaTime * T64_vcoFreq / (Sint64T)1000000000;

				bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
						"StepTimeNsToRegisterUnits dtRU:0x%x vF:0x%lx dT:0x%x Bs:0x%x Ks:0x%x Ms:0x%x Ns:0x%x",
						*deltaTimeRU,T64_vcoFreq,deltaTime,Bs,Ks,Ms,Ns);

            }
        }
        else if( outputType == ZLS3077X_OUTPUT_TYPE_HP )
        {
            /* deltaTimeRU must be in MSDIV cycles */

            /* This shortcut assumes that the output frequency is 1pps. In
               this case, each MSDIV cycle in ns is 1/LSDIV since there are
               LSDIV cycles in 1s.*/

            if (status == ZL303XX_OK)
            {
               status = zl303xx_Dpll77xGetLSDIV(hwParams, &lsdiv);
            }
            if (status == ZL303XX_OK)
            {
                if( lsdiv == 0 )
                {
                    /* There is no output configured to step. */
                    *deltaTimeRU = 0;
                }
                else
                {
                    /* Round deltaTime to the nearest increment. Note that it is
                       expected that routine zl303xx_Dpll77xModifyStepTimeNs() will
                       have been called to make deltaTime evenly divisible by LSDIV.
                       If not, the following calculation will introduce inaccuracy. */
                    if( deltaTime > 0 )
                    {
                        *deltaTimeRU = ((Sint64T)deltaTime * (Sint64T)lsdiv + 500000000) /
                                        (Sint64T)1000000000;
                    }
                    else
                    {
                        *deltaTimeRU = ((Sint64T)deltaTime * (Sint64T)lsdiv - 500000000) /
                                        (Sint64T)1000000000;
                    }
                }
            }
        }
        else
        {
            status = ZL303XX_PARAMETER_INVALID;
        }
    }


    return status;
}


/* zl303xx_Dpll77xOutputPhaseStepRead */
/**
   Reads the current phase step accumulation of the 77X device.

  Parameters:
   [in]    hwParams      Pointer to the device parameter structure.
   [in]    clockFreq     Freq to convert ns to cycles.
   [out]   phaseInFreqClockCycles    Pointer to the current phase offset
                                        in freq cycles.
   [out]   accumPhase    Pointer to the accumulated phase offset.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xOutputPhaseStepRead(void *hwParams, Uint32T clockFreq,
                        Sint32T *phaseInFreqClockCycles, Sint32T *accumPhase)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xOutputPhaseStepRead: clockFreq=%d",
            clockFreq, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(phaseInFreqClockCycles);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(accumPhase);
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xAccuStepTimePhase1588HostRegGet(hwParams, accumPhase);
        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xOutputPhaseStepRead: Call to zl303xx_Dpll77xAccuStepTimePhase1588HostRegGet() failed=%d",
                    status, 0,0,0,0,0);
        }
    }

    /* Convert phaseInNs to clock cycles based on clockFreq  */
    if (status == ZL303XX_OK)
    {
        *phaseInFreqClockCycles = ((*accumPhase * (Uint64T)clockFreq) /
                                        (Uint64T)TEN_e9); /* Scaled for integer math */
        *phaseInFreqClockCycles = abs( *phaseInFreqClockCycles );

        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2, "zl303xx_Dpll77xOutputPhaseStepRead: phaseInFreqClockCycles: %u accumPhase=%d",
                *phaseInFreqClockCycles, *accumPhase, 0,0,0,0);
    }

    return status;
}


/* zl303xx_Dpll77xOutputPhaseStepWritePerOutputType */
/**
    Starts a step of the output frame pulse of a ZL3077X device. This routine
    can be called for both types of output synthesizer: GP or HP.

    This routine:
    1) converts deltaTime to the appropriate register units
    2) waits for any previous step to complete
    3) writes the appropriate step mask
    4) writes the converted step size to registers
    5) tells the DPLL to start the step

  Parameters:
   [in]   hwParams     Pointer to the device parameter structure.
   [in]   deltaTime    Signed time difference in nanoseconds.
   [in]   inCycles     Not used - for future
   [in]   clockFreq    Not used - for future
   [in]   outputType   Which outputs to step: either HP or GP

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
static Sint32T zl303xx_Dpll77xOutputPhaseStepWritePerOutputType(void *hwParams,
                                    Sint32T deltaTime,
                                    zl303xx_BooleanE inCycles, Uint32T clockFreq,
                                    ZLS3077X_OutputTypesE outputType)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;
    Uint32T phaseStepMaskGp;
    Uint32T phaseStepMaskHp;
    ZLS3077X_OutputTypesE outputTypeThatDrivesTimeStamper;
    ZLS3077X_OutputsE outputNumThatDrivesTimeStamper;
    Sint32T deltaTimeRU;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xOutputPhaseStepWrite: deltaTime=%d  inCycles=%d  clockFreq=%d ",
            deltaTime, inCycles, clockFreq, 0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_BOOLEAN(inCycles);
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    /* Get the outputs that we want to move. */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xPhaseStepMaskGet(hwParams,
                                            &phaseStepMaskGp,
                                            &phaseStepMaskHp,
                                            &outputTypeThatDrivesTimeStamper,
                                            &outputNumThatDrivesTimeStamper);
    }
    /* Convert deltaTime so it can be written into a register. Either:
       1) For HP outputs, deltaTime is converted to a multiple of LSDIV or
       2) For GP outputs, deltaTime is converted to VCO cycles.
    */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xConvertStepTimeNsToRegisterUnits(hwParams,
                                            deltaTime, &deltaTimeRU, outputType);
    }
    /* Wait for any previous step to complete */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xWaitForPhaseCtrlReady(hwParams);
    }
    /* Clear all step output masks. */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Write(zl303xx_Params, NULL,
                            ZLS3077X_PHASE_STEP_MASK_GP_REG,
                            0);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Write(zl303xx_Params, NULL,
                            ZLS3077X_PHASE_STEP_MASK_HP_REG,
                            0);
    }

	char ptplog_path[] = "/proc/rru/ptplog";
    /* Write the appropriate step mask. */
    if (status == ZL303XX_OK)
    {
        if( outputType == ZLS3077X_OUTPUT_TYPE_GP )
        {
            status = zl303xx_Write(zl303xx_Params, NULL,
                                ZLS3077X_PHASE_STEP_MASK_GP_REG,
                                phaseStepMaskGp);

		    bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
		    				"PhaseStepWrite(0x%x) phaseStepMaskGp(%d) status(%d)",
		    				ZLS3077X_PHASE_STEP_MASK_GP_REG, phaseStepMaskGp, status );

        }
        if( outputType == ZLS3077X_OUTPUT_TYPE_HP )
        {
            status = zl303xx_Write(zl303xx_Params, NULL,
                                ZLS3077X_PHASE_STEP_MASK_HP_REG,
                                phaseStepMaskHp);
 
		    bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
		    				"PhaseStepWrite(0x%x) phaseStepMaskHp(%d) status(%d)",
					ZLS3077X_PHASE_STEP_MASK_HP_REG, phaseStepMaskHp, status );

 
        }
    }
    /* Write the converted deltaTime into the register. */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Write(zl303xx_Params, NULL,
                            ZLS3077X_PHASE_STEP_DATA_REG,
                            (Sint32T)deltaTimeRU);


		    bpf_com_plog_trace( ptplog_path   ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
		    				"PhaseStepWrite(0x%x) deltaTimeRU(%d) status(%d)",
					ZLS3077X_PHASE_STEP_DATA_REG, deltaTimeRU, status );


    }
    /* Tell the DPLL to start the step. */
    if (status == ZL303XX_OK)
    {
        if( outputType == outputTypeThatDrivesTimeStamper )
        {
            status = zl303xx_Write(zl303xx_Params, NULL,
                                ZLS3077X_PHASE_STEP_CTRL_REG,
                                ((pllId << 4) |
                                  ZLS3077X_PHASE_STEP_CTRL_TOD_STEP |
                                  ZLS3077X_PHASE_STEP_CTRL_WRITE));


		    bpf_com_plog_trace( ptplog_path   ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
		    				"PhaseStepWrite(0x%x) val(0x%x) status(%d) outputType(0x%x)",
					ZLS3077X_PHASE_STEP_CTRL_REG, ((pllId << 4) |
                                  ZLS3077X_PHASE_STEP_CTRL_TOD_STEP |
                                  ZLS3077X_PHASE_STEP_CTRL_WRITE), status ,outputType);


        }
        else
        {
            status = zl303xx_Write(zl303xx_Params, NULL,
                                ZLS3077X_PHASE_STEP_CTRL_REG,
                                ((pllId << 4) |
                                  ZLS3077X_PHASE_STEP_CTRL_WRITE));


		    bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
		    			"PhaseStepWrite(0x%x) val(0x%x) status(%d) outputType(0x%x)",
					ZLS3077X_PHASE_STEP_CTRL_REG, ((pllId << 4) |
                                  ZLS3077X_PHASE_STEP_CTRL_WRITE), status ,outputType);

        }
    }
    if (status == ZL303XX_OK)
    {
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2, "zl303xx_Dpll77xOutputPhaseStepWrite: of %ld ns",
                deltaTime, 0,0,0,0,0);
    }
    else
    {
        ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xOutputPhaseStepWrite: return failed=%d",
                status, 0,0,0,0,0);
    }

    return status;
}


/* zl303xx_Dpll77xOutputPhaseStepWrite */
/**
    Starts the step of the output frame pulse of a ZL3077X device. If
    the device (dpll) drives more than one output then each will have
    a phase step applied.

    Depending on configuration, it can take many seconds for the step to
    complete. We can not block waiting for the step to complete so this
    routine hands-off the step completion mechanism to the zlDpll77xTask
    task (zl303xx_Dpll77xStepTimeStateMachine) by setting
    STState = ZLS3077X_STS_WAITING_FOR_STEP_END.

  Parameters:
   [in]   hwParams     Pointer to the device parameter structure.
   [in]   deltaTime    Signed time difference in nanoseconds.
   [in]   inCycles     TRUE if deltaTime is in ClockCycles.
   [in]   clockFreq    Current input freq. (if working in cycles).

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
Sint32T zl303xx_Dpll77xOutputPhaseStepWrite(void *hwParams, Sint32T deltaTime,
                        zl303xx_BooleanE inCycles, Uint32T clockFreq)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T phaseStepMaskGp;
    Uint32T phaseStepMaskHp;
    ZLS3077X_OutputTypesE outputTypeThatDrivesTimeStamper;
    ZLS3077X_OutputsE outputNumThatDrivesTimeStamper;

	/* for debug  */
    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xOutputPhaseStepWrite: deltaTime=%d  inCycles=%d  clockFreq=%d ",
            deltaTime, inCycles, clockFreq, 0,0,0);

    /* Clear the sticky bits we are going to use. */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xStepTimeClearOutputStickyBits(hwParams);
    }
    /* Get the HP outputs that we want to move. */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xPhaseStepMaskGet(hwParams,
                                            &phaseStepMaskGp,
                                            &phaseStepMaskHp,
                                            &outputTypeThatDrivesTimeStamper,
                                            &outputNumThatDrivesTimeStamper);
    }

    if (status == ZL303XX_OK)
    {
        /* Explicitly configure "Phase Step Max" register:
           Specifies the maximum phase step to be applied to an output
           clock/frame pulse. This value is specified as a percentage of the
           output clock/frame pulse period.
           This value must be between 1 and 49.
        */
        status = zl303xx_Write(hwParams, NULL,
                             ZLS3077X_PHASE_STEP_MAX_REG,
                             49);
    }
    if (status == ZL303XX_OK)
    {
        if( outputTypeThatDrivesTimeStamper == ZLS3077X_OUTPUT_TYPE_GP )
        {
            if (status == ZL303XX_OK)
            {
                /* Move the GP outputs */
                status = zl303xx_Dpll77xOutputPhaseStepWritePerOutputType(hwParams, deltaTime,
                                        inCycles, clockFreq, ZLS3077X_OUTPUT_TYPE_GP);
            }
            if( phaseStepMaskHp > 0 )
            {
                /* Move the HP outputs */
                status = zl303xx_Dpll77xOutputPhaseStepWritePerOutputType(hwParams, deltaTime,
                                        inCycles, clockFreq, ZLS3077X_OUTPUT_TYPE_HP);
            }
        }
    }
    if (status == ZL303XX_OK)
    {
        if( outputTypeThatDrivesTimeStamper == ZLS3077X_OUTPUT_TYPE_HP )
        {
            if (status == ZL303XX_OK)
            {
                /* Move the GP outputs */
                status = zl303xx_Dpll77xOutputPhaseStepWritePerOutputType(hwParams, deltaTime,
                                        inCycles, clockFreq, ZLS3077X_OUTPUT_TYPE_HP);
            }
            if( phaseStepMaskGp > 0 )
            {
                /* Move the HP outputs */
                status = zl303xx_Dpll77xOutputPhaseStepWritePerOutputType(hwParams, deltaTime,
                                        inCycles, clockFreq, ZLS3077X_OUTPUT_TYPE_GP);
            }
        }
    }
    if (status == ZL303XX_OK)
    {
        /* The step is in progress. Set the stepTime state variable so that the
           77x task know to moitor for completion of the step. When the step
           is done, the task will call back to APR to finish up. */
        zl303xx_Params->pllParams.d77x.std.sanityStartTick = OS_TICK_GET();
        zl303xx_Params->pllParams.d77x.std.STState = ZLS3077X_STS_WAITING_FOR_STEP_END;

        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
                    "zl303xx_Dpll77xOutputPhaseStepWrite: starting step time wait timer", 0,0,0,0,0,0);

        /* Add the step size to the total stored in the device. */
        status = zl303xx_Dpll77xAccuStepTimePhase1588HostRegUpdate(hwParams, deltaTime);
    }

    return (Sint32T)status;
}


/* zl303xx_Dpll77xStepTimeStateMachine */
/**
    This routine runs an overall state machine to manage the end of the
    step time feature.


    The possible states of the state machine are:

    ZLS3077X_STS_IDLE

        No action is being taken

    ZLS3077X_STS_WAITING_FOR_STEP_END

        We are waiting for the end of a step.

    ZLS3077X_SS_IN_GUARD_TIMER

        The step has been completed.

  Return Value:
   zlStatusE

*******************************************************************************/
static void zl303xx_Dpll77xStepTimeStateMachine(void)
{
    Uint32T idx;
    Uint32T complete;
    Uint32T secondsSinceStartOfTimer;


    for( idx = 0; idx < ZL303XX_MAX_NUM_77X_PARAMS; idx++ )
    {
        if( zl303xx_Params77x[idx] != NULL )
        {
            zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS *)zl303xx_Params77x[idx];

            switch( zl303xx_Params->pllParams.d77x.std.STState )
            {
                case ZLS3077X_STS_IDLE:
                    break;

                case ZLS3077X_STS_WAITING_FOR_STEP_END:

                    if( zl303xx_Dpll77xOutputPhaseStepStatusGet(zl303xx_Params, &complete) == ZL303XX_OK )
                    {
                        if( complete == 1 )
                        {
                            zl303xx_Params->pllParams.d77x.std.guardStartTick = OS_TICK_GET();
                            zl303xx_Params->pllParams.d77x.std.STState = ZLS3077X_STS_IN_GUARD_TIMER;

                            ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
                                        "zl303xx_Dpll77xStepTimeStateMachine: starting guard timer", 0,0,0,0,0,0);
                        }
                        else
                        {
                            secondsSinceStartOfTimer =
                                ( OS_TICK_GET() - zl303xx_Params->pllParams.d77x.std.sanityStartTick )
                                / OS_TICK_RATE_GET();

                            if( secondsSinceStartOfTimer > ZLS3077X_STEP_TIME_SANITY_TIMEROUT )
                            {
                                /* stepTime() has still not completed. Something is
                                   probably wrong. */
                                zl303xx_Params->pllParams.d77x.std.STState = ZLS3077X_STS_IDLE;

                                ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xStepTimeStateMachine: timeout! %u",
                                        secondsSinceStartOfTimer, 0,0,0,0,0);
                            }
                        }
                    }
                    else
                    {
                        ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xStepTimeStateMachine: zl303xx_Dpll77xOutputPhaseStepStatusGet() failed",
                                0,0,0,0,0,0);
                    }
                    break;

                case ZLS3077X_STS_IN_GUARD_TIMER:

                    secondsSinceStartOfTimer =
                        ( OS_TICK_GET() - zl303xx_Params->pllParams.d77x.std.guardStartTick )
                        / OS_TICK_RATE_GET();


                    if( secondsSinceStartOfTimer > (ZLS3077X_GUARD_TIMER_LENGTH_IN_SEC * (1000 / ZLS3077X_TIMER_DELAY_MS)))
                    {
                        /* Signal the upper layer that the step is done and
                           that the timestamper can be re-aligned. */
                        if( zl303xx_Params->pllParams.d77x.std.stepDoneFuncPtr != NULL )
                        {
                            (zl303xx_Params->pllParams.d77x.std.stepDoneFuncPtr)(zl303xx_Params);
                        }

                        zl303xx_Params->pllParams.d77x.std.STState = ZLS3077X_STS_IDLE;

                        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
                                    "zl303xx_Dpll77xStepTimeStateMachine: ending guard timer", 0,0,0,0,0,0);
                    }

                    break;

                default:
                    break;
            }
        }
    }
}


/* zl303xx_Dpll77xjumpActiveCGU */
/**
   Wrapper function used for redundancy code.

  Parameters:
   [in]   hwParams     Pointer to the device parameter structure.
   [in]   seconds      Not used.
   [in]   nanoSeconds  Unsigned time difference in nanoseconds.
   [in]   bBackwardAdjust  Sign of nanoSeconds.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
Sint32T zl303xx_Dpll77xjumpActiveCGU(void* hwParams, Uint64S seconds,
                            Uint32T nanoSeconds, zl303xx_BooleanE bBackwardAdjust)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xjumpActiveCGU: seconds=%d  nanoSeconds=%x.%x  bBackwardAdjust=%d",
            seconds.hi, seconds.lo, nanoSeconds, bBackwardAdjust, 0,0);

    if(seconds.hi) {}; /* warning removal */

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }

    if( bBackwardAdjust == ZL303XX_TRUE )
    {
        nanoSeconds = -nanoSeconds;
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xOutputPhaseStepWrite(hwParams, nanoSeconds,
                    ZL303XX_FALSE, 0);  /* In ns so no clock-related data */
    }

    return( status );
}


/* zl303xx_Dpll77xToDGetCtrlRegCmd */
/** 
   Returns the current value of the cmd bit field of the 77x ToD control
   register.

  Parameters:
   [in]   hwParams     Pointer to the device parameter structure.
   [out]  cmd          The cmd field of the register

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
static zlStatusE zl303xx_Dpll77xToDGetCtrlRegCmd(void *hwParams, Uint32T *cmd)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xToDGetCtrlRegCmd: ",
            0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(cmd);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL,
                          ZLS3077X_DPLL_TOD_CTRL_REG(pllId),
                          cmd);
    }
    if( status == ZL303XX_OK )
    {
        *cmd = *cmd & ZLS3077X_DPLL_TOD_CMD_MASK;
    }

    return status;
}


/* zl303xx_Dpll77xToDCleanupCtrlReg */
/** 
    Makes the ToD control register available to use.

    If we get caught waiting for ToD latch value, we may need to abort that
    command if something more important needs to be done.

    This routine sets the cmd field to 0 and waits for a little while for the
    ZL3077x device to recover.

  Parameters:
   [in]   hwParams     Pointer to the device parameter structure.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
static zlStatusE zl303xx_Dpll77xToDCleanupCtrlReg(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xToDCleanupCtrlReg: ",
            0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if( status == ZL303XX_OK )
    {
        status = zl303xx_Write(zl303xx_Params, NULL,
                      ZLS3077X_DPLL_TOD_CTRL_REG(pllId),
                      (ZLS3077X_DPLL_TOD_CMD_DONE & ZLS3077X_DPLL_TOD_CMD_MASK));
        if( status != ZL303XX_OK )
        {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
                        "zl303xx_Dpll77xToDCleanupCtrlReg: write ctrl reg failed",
                        status,0,0,0,0,0);
        }

        OS_TASK_DELAY( ZLS3077X_STICKY_UPDATE_DELAY_MS );
    }

    return status;
}


/* zl303xx_Dpll77xToDWaitForCtrlRegAvailable */
/** 
   Wait for the 77x ToD control register to become available

  Parameters:
   [in]   hwParams   Pointer to the device parameter structure.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
static zlStatusE zl303xx_Dpll77xToDWaitForCtrlRegAvailable(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;
    Uint32T attempts = 0;
    Uint32T cmd;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xToDWaitForCtrlRegAvailable:",
            0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        do
        {
            status = zl303xx_Dpll77xToDGetCtrlRegCmd( zl303xx_Params, &cmd );

            if( status == ZL303XX_OK )
            {
                if( cmd == ZLS3077X_DPLL_TOD_CMD_DONE )
                {
                    /* The register is available */
                    return status;
                }

                OS_TASK_DELAY(ZLS3077X_REG_READ_INTERVAL);

                /* Only delay up to, very roughly, MediumSanityTO */
                if (++attempts > (Dpll77xMediumSanityTO/ZLS3077X_REG_READ_INTERVAL))
                {
                    ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xToDWaitForCtrlRegAvailable: timeout!",
                            0,0,0,0,0,0);
                    status = ZL303XX_TIMEOUT;
                }
            }
        } while (status == ZL303XX_OK);
    }

    return status;
}


/* zl303xx_Dpll77xToDWaitForSpecificRollover */
/** 
    Wait for the seconds rollover of the ToD register.

  Parameters:
   [in]   hwParams   Pointer to the device parameter structure.
   [in]   timeoutMs  Very approximate max time to wait for this routine to
                        complete.
   [in]   readType   Either waiting for the current time or the predicted time
   [in]   initNs     Usually 0 which means this routine returns when the
                        seconds rollover. If not 0, then this routine returns
                        when the seconds rollover and the ns rollover.

   [out]  sec        Pointer to the seconds after the rollover
   [out]  ns         Pointer to the ns after the rollover

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xToDWaitForSpecificRollover(void *hwParams, Uint32T timeoutMs,
                        ZLS3077X_TODReadTypeE readType, Sint32T initNs,
                        Uint64S *sec, Uint32T *ns)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;
    Uint32T initSecLo = 0;
    Sint64T prevNs = 0;
    Sint64T newNs = 0;
    Uint16T attempts = 0;
    Uint32T cmd;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xToDWaitForSpecificRollover: timeoutMs=%d  readType=%d  initNs=%",
            timeoutMs, readType, initNs, 0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xToDGetCtrlRegCmd( zl303xx_Params, &cmd );
    }
    if (status == ZL303XX_OK)
    {
        if(( cmd == ZLS3077X_TRT_ToDLatchedOnNCOWrite ) ||
           ( cmd == ZLS3077X_TRT_ToDLatchedOnNCORead ))
        {
            /* We are waiting for the latched value. Discard that request. */
            ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xToDWaitForSpecificRollover: Latch request discarded",
                    0,0,0,0,0,0);
            status = zl303xx_Dpll77xToDCleanupCtrlReg(zl303xx_Params);
        }
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xToDWaitForCtrlRegAvailable(zl303xx_Params);
    }
    if (status == ZL303XX_OK)
    {
        do
        {
            if (status == ZL303XX_OK)
            {
                status = zl303xx_Write(zl303xx_Params, NULL,
                                      ZLS3077X_DPLL_TOD_CTRL_REG(pllId),
                                      (readType & ZLS3077X_DPLL_TOD_CMD_MASK));
            }
            if (status == ZL303XX_OK)
            {
                status = zl303xx_Dpll77xToDWaitForCtrlRegAvailable(zl303xx_Params);
            }
            if (status == ZL303XX_OK)
            {
                status = zl303xx_Read(zl303xx_Params, NULL,
                                  ZLS3077X_DPLL_TOD_SEC_HI_REG(pllId),
                                  &(sec->hi));
            }
            if (status == ZL303XX_OK)
            {
                status = zl303xx_Read(zl303xx_Params, NULL,
                                  ZLS3077X_DPLL_TOD_SEC_LO_REG(pllId),
                                  &(sec->lo));
            }
            if (status == ZL303XX_OK)
            {
                status = zl303xx_Read(zl303xx_Params, NULL,
                                  ZLS3077X_DPLL_TOD_NS_REG(pllId),
                                  ns);
            }
            if (status == ZL303XX_OK)
            {
                if( initSecLo == 0 )
                {
                    initSecLo = sec->lo;
                    if( initSecLo >= 0xfffffffe )
                    {
                        /* Unusual case where ToD is rolling over from
                           ffffffffffff to 0. In this case, just fail.
                           This is a fictitious scenario that will only happen
                           when testing - maybe. */
                        return ZL303XX_ERROR;
                    }
                }
                else
                {
                    newNs  = ((Sint64T)sec->lo   * (Sint64T)1000000000) +
                                (Sint64T)*ns;
                    prevNs = ((Sint64T)initSecLo * (Sint64T)1000000000) +
                                (Sint64T)initNs;

                    if( (newNs - prevNs) > 1000000000 )
                    {
                        /* rollover */
                        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xToDWaitForSpecificRollover: sec=0x%x.%x  initNs=0x%x",
                                sec->hi, sec->lo, *ns, 0,0,0);
                        return status;
                    }
                }
            }

            if( status == ZL303XX_OK )
            {
                OS_TASK_DELAY(ZLS3077X_REG_READ_INTERVAL);

                /* Only delay up to timeoutMs */
                if (++attempts > (timeoutMs/ZLS3077X_REG_READ_INTERVAL))
                {
                    ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xToDWaitForSpecificRollover: timeout!",
                            0,0,0,0,0,0);
                    status = ZL303XX_TIMEOUT;
                }
            }
        } while (status == ZL303XX_OK);
    }

    return status;
}


/* zl303xx_Dpll77xToDWaitForRollover */
/** 
    Wrapper for zl303xx_Dpll77xToDWaitForSpecificRollover().

  Parameters:
   [in]   hwParams     Pointer to the device parameter structure.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xToDWaitForRollover(void *hwParams, Uint32T timeoutMs,
                        ZLS3077X_TODReadTypeE readType, Uint64S *sec, Uint32T *ns)
{
    return zl303xx_Dpll77xToDWaitForSpecificRollover(hwParams, timeoutMs,
                                                    readType, 0, sec, ns);
}


/* zl303xx_Dpll77xLatchedToDReadSetup */
/** 
    Setup the latched ToD read values from the 77x.

    To use this feature:
    1) Tell the ZL3077x device to latch the value the next time an NCO
       value is read or written
    2) Wait for something to read or write the df.
       a) If in NCO mode, writes occur at least every 1s.
          Latched ToD reads are not compatible in this mode.
       b) It not in NCO mode, reads and writes are unexpected but not
          an error.
    3) Read the ToD value using zl303xx_Dpll77xToDRead() with the same readType
       passed to retreive the latched value.

  Parameters:
   [in]   hwParams   Pointer to the device parameter structure.
   [in]   readType   Either reading from the current time or the predicted time

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xLatchedToDReadSetup(void *hwParams, ZLS3077X_TODReadTypeE readType)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;
    Uint32T cmd;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xLatchedToDReadSetup: readType=%d  ",
            readType, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        if(( readType != ZLS3077X_TRT_ToDLatchedOnNCOWrite ) &&
           ( readType != ZLS3077X_TRT_ToDLatchedOnNCORead ))
        {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xLatchedToDReadSetup: bad read type: %d",
                    readType, 0,0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
        }
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        if(( zl303xx_Dpll77xInNCOMode(hwParams) == ZL303XX_TRUE ) &&
           ( readType == ZLS3077X_TRT_ToDLatchedOnNCORead ))
        {
            /* Getting a latch value on a df read while in NCO mode is an
               invalid request.

               To trigger the ToD read on a df read, we need to write register
               ZLS3077X_DPLL_DF_CTRL (via zl303xx_Dpll77xRequestFreqRead()).
               We do not write this register in NCO mode because the latched
               value returned is the df at the time we switched to NCO
               mode - not the value currently applied in NCO mode. This latched
               value is not useful in NCO mode so we do not allow it. */
            status = ZL303XX_PARAMETER_INVALID;
        }
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xToDGetCtrlRegCmd( zl303xx_Params, &cmd );
    }
    if (status == ZL303XX_OK)
    {
        if(( cmd == ZLS3077X_TRT_ToDLatchedOnNCOWrite ) ||
           ( cmd == ZLS3077X_TRT_ToDLatchedOnNCORead ))
        {
            /* We are already waiting for a latch value. Overwrite that one
               with this one. */
            ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xLatchedToDReadSetup: New latch type specified",
                    0,0,0,0,0,0);
            status = zl303xx_Dpll77xToDCleanupCtrlReg(zl303xx_Params);
        }
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xToDWaitForCtrlRegAvailable(zl303xx_Params);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Write(zl303xx_Params, NULL,
                              ZLS3077X_DPLL_TOD_CTRL_REG(pllId),
                              (readType & ZLS3077X_DPLL_TOD_CMD_MASK));
    }
    /* At this point we wait for the next NCO read or NCO write to populate
       registers dpll_tod_sec_1 (0x38a) and dpll_tod_ns_1 (0x392).

       They are read, later, using zl303xx_Dpll77xToDRead().
    */


    return status;
}


/* zl303xx_Dpll77xToDRead */
/** 
    Read the time-of-day from the 77x.

  Parameters:
   [in]   hwParams   Pointer to the device parameter structure.
   [in]   readType   Either reading from the current time or the predicted time

   [out]  sec        The seconds read
   [out]  ns         The ns read

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xToDRead(void *hwParams, ZLS3077X_TODReadTypeE readType,
                                Uint64S *sec, Uint32T *ns)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;
    Uint32T cmd;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xToDRead: readType=%d  ",
            readType, 0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(sec);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(ns);
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xToDGetCtrlRegCmd( zl303xx_Params, &cmd );
    }
    if (status == ZL303XX_OK)
    {
        if(( cmd == ZLS3077X_TRT_ToDLatchedOnNCOWrite ) ||
           ( cmd == ZLS3077X_TRT_ToDLatchedOnNCORead ))
        {
            /* We are waiting for the latched value but it is not ready yet. */

            if(( readType == ZLS3077X_TRT_ToDLatchedOnNCOWrite ) ||
               ( readType == ZLS3077X_TRT_ToDLatchedOnNCORead ))
            {
                /* This request is for a read of the latched value. Return
                   INVALID_OPERATION and keep waiting. */
                status = ZL303XX_INVALID_OPERATION;
            }
            else
            {
                /* This request is for a ToD read. Abort the latched value
                   read and do this request. */
                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xToDRead: Latch request discarded",
                        0,0,0,0,0,0);
                status = zl303xx_Dpll77xToDCleanupCtrlReg(zl303xx_Params);
            }
        }
    }
    if (status == ZL303XX_OK)
    {
        if(( readType == ZLS3077X_TRT_ToDLatchedOnNCOWrite ) ||
           ( readType == ZLS3077X_TRT_ToDLatchedOnNCORead ))
        {
            /* We are requesting a read of the ToD that was previously latched.
               We just want to read the value already stored in registers. */
        }
        else
        {
            /* Issue the command to read the ToD. */
            if (status == ZL303XX_OK)
            {
                status = zl303xx_Dpll77xToDWaitForCtrlRegAvailable(zl303xx_Params);
            }
            if (status == ZL303XX_OK)
            {
                status = zl303xx_Write(zl303xx_Params, NULL,
                                      ZLS3077X_DPLL_TOD_CTRL_REG(pllId),
                                      (readType & ZLS3077X_DPLL_TOD_CMD_MASK));
            }
            if (status == ZL303XX_OK)
            {
                status = zl303xx_Dpll77xToDWaitForCtrlRegAvailable(zl303xx_Params);
            }
        }
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL,
                          ZLS3077X_DPLL_TOD_SEC_HI_REG(pllId),
                          &(sec->hi));
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL,
                          ZLS3077X_DPLL_TOD_SEC_LO_REG(pllId),
                          &(sec->lo));
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Read(zl303xx_Params, NULL,
                          ZLS3077X_DPLL_TOD_NS_REG(pllId),
                          ns);
    }
    if( status == ZL303XX_OK )
    {
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xToDRead: sec=0x%x.%x  ns=0x%x",
                sec->hi, sec->lo, *ns, 0,0,0);
    }

    return status;
}


/* zl303xx_Dpll77xToDWrite */
/** 
    Write the time-of-day to the 77x.

  Parameters:
   [in]   hwParams   Pointer to the device parameter structure.
   [in]   relativeAdjust TRUE if the given time should be added to the current
                            time. FALSE if the given value is absolute time.
   [in]   readType   Either reading from the current time or the predicted time
   [in]   sec        The seconds read
   [in]   ns         The ns read
   [in]   bBackwardAdjust TRUE if the given time is negative i.e. earlier
                             in time

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xToDWrite(void *hwParams, ZLS3077X_ToDWriteTypeE writeType,
                        zl303xx_BooleanE relativeAdjust,
                        Uint64S sec, Uint32T ns, zl303xx_BooleanE bBackwardAdjust)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T pllId;
    Uint64S currSec;
    Uint32T currNs;
    Uint64S newSec;
    Sint32T newNs;
    Uint64S rollSec;
    Uint32T rollNs;

    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xToDWrite: writeType=%d  relativeAdjust=%d  sec=%x.%x  bBackwardAdjust=%d",
            writeType, relativeAdjust, sec.hi, sec.lo, bBackwardAdjust, 0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        if(( relativeAdjust == ZL303XX_FALSE ) && ( bBackwardAdjust == ZL303XX_TRUE ))
        {
            /* Can not set a negative time */
            status = ZL303XX_PARAMETER_INVALID;
        }
    }
    if (status == ZL303XX_OK)
    {
        pllId = zl303xx_Params->pllParams.pllId;
        status = ZLS3077X_CHECK_DPLLID(pllId);
    }
    if (status == ZL303XX_OK)
    {
        if( writeType == ZLS3077X_TWT_writeOnNextInputSync )
        {
            /* Get the current difference between the internal 1Hz pulse and
               any steps that have already been performed. We are assuming
               that we are aligned or trying to become aligned to the sync
               pulse - so 'writeOnNextInputSync' really means write-just-after-
               where-we-curently-think-the-sync-pulse-is.
            */
            status = zl303xx_Dpll77xToDRead( hwParams, ZLS3077X_TRT_predictedToD, &rollSec, &rollNs );
            if( status != ZL303XX_OK )
            {
                ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xToDWrite (1): failed status=%d",
                        status, 0,0,0,0,0);
            }
        }
    }
    if (status == ZL303XX_OK)
    {
        /* Wait for the right time to write the data to the device. Also, the
           following routines return the seconds and nanoseconds of the current
           time; for relative times, we then add the requested adjustment to
           these current times.

           Note: 2 of these routines block waiting for the roll-over */
        switch( writeType )
        {
            case ZLS3077X_TWT_writeOnInternal1Hz:

                /* Typical; wait for the seconds field to roll-over */
                status = zl303xx_Dpll77xToDWaitForSpecificRollover(hwParams,
                                Dpll77xLongSanityTO, ZLS3077X_TRT_currentToD, 0,
                                &currSec, &currNs);
                break;

            case ZLS3077X_TWT_writeOnNextInputSync:

                /* Wait for the rollover of the input-sync to internal-1Hz offset
                   roll-over */
                status = zl303xx_Dpll77xToDWaitForSpecificRollover(hwParams,
                                Dpll77xLongSanityTO, ZLS3077X_TRT_currentToD, rollNs,
                                &currSec, &currNs);
                break;

            default:
                status = ZL303XX_PARAMETER_INVALID;
                break;
        }
    }
    /* We now have 1s to write the ToD registers. First read the predicted ToD
       and then add the given time to it. */
    if (status == ZL303XX_OK)
    {
        if(( writeType == ZLS3077X_TWT_writeOnInternal1Hz ) ||
           ( writeType == ZLS3077X_TWT_writeOnNextInputSync ))
        {
            status = zl303xx_Dpll77xToDRead(hwParams, ZLS3077X_TRT_predictedToD,
                            &currSec, &currNs);
        }
    }
    /* We have the current time. If this is a relative adjustment, add the given
       adjustment to the current time. Otherwise, just use the given values. */
    if (status == ZL303XX_OK)
    {
        if( relativeAdjust == ZL303XX_TRUE )
        {
            Sint64T T64_currSec;
            Sint64T T64_sec;
            Sint64T T64_newSec;

            ZL303XX_CONVERT_TO_64(T64_currSec, currSec);
            ZL303XX_CONVERT_TO_64(T64_sec, sec);

            if( bBackwardAdjust == ZL303XX_TRUE )
            {
                T64_newSec = T64_currSec - T64_sec;

                newNs = currNs - ns;
                if( newNs < 0 )
                {
                    T64_newSec--;
                    newNs = TEN_e9 + newNs;
                }
            }
            else
            {
                T64_newSec = T64_currSec + T64_sec;

                newNs = currNs + ns;
                if( newNs > TEN_e9 )
                {
                    T64_newSec++;
                    newNs = newNs - TEN_e9;
                }
            }
            ZL303XX_CONVERT_FROM_64(newSec, T64_newSec);
        }
        else
        {
            newSec = sec;
            newNs = ns;
        }
    }
    if (status == ZL303XX_OK)
    {
        if( newNs > TEN_e9 )
        {
            /* Writing a value larger than 1000000000 into the ns field and
               then trying to activate it will cause the ToD control register
               to be stuck. Do not allow that here. */
            status = ZL303XX_PARAMETER_INVALID;
        }
    }
    
    /* If everything is good, write the registers. */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Write(zl303xx_Params, NULL,
                          ZLS3077X_DPLL_TOD_SEC_HI_REG(pllId),
                          newSec.hi);
        /* for Debug */
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xToDWrite : reg_addr=%x, newSec.hi=%d, status=%d",
                      ZLS3077X_DPLL_TOD_SEC_HI_REG(pllId), newSec.hi,status,0,0,0);
        
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Write(zl303xx_Params, NULL,
                          ZLS3077X_DPLL_TOD_SEC_LO_REG(pllId),
                          newSec.lo);
        /* for Debug */
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xToDWrite : reg_addr=%x, newSec.lo=%d, status=%d",
                      ZLS3077X_DPLL_TOD_SEC_LO_REG(pllId), newSec.lo,status,0,0,0);
                      
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Write(zl303xx_Params, NULL,
                          ZLS3077X_DPLL_TOD_NS_REG(pllId),
                          newNs);
                          
        /* for Debug */
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xToDWrite : reg_addr=%x, newNs=%d, status=%d",
                      ZLS3077X_DPLL_TOD_NS_REG(pllId), newNs, status,0,0,0);
                          
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Write(zl303xx_Params, NULL,
                              ZLS3077X_DPLL_TOD_CTRL_REG(pllId),
                              (writeType & ZLS3077X_DPLL_TOD_CMD_MASK));

        /* for Debug */
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xToDWrite : reg_addr=%x, writeType=%d, status=%d",
                      ZLS3077X_DPLL_TOD_CTRL_REG(pllId), (writeType & ZLS3077X_DPLL_TOD_CMD_MASK), status,0,0,0);

    }
    if( status == ZL303XX_OK )
    {
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4, "zl303xx_Dpll77xToDWrite: sec=0x%x.%x  initNs=0x%x",
                newSec.hi, newSec.lo, newNs, 0,0,0);
    }

    return status;
}


/* zl303xx_Dpll77xInNCOMode */
/* 

   Decide if the 77X device is in NCO mode.

  Parameters:
   [in]   hwParams  Pointer to an APR device structure.

\returns     Boolean

*******************************************************************************/
zl303xx_BooleanE zl303xx_Dpll77xInNCOMode(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Sint32T inNCOMode = 0;


    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xHwModeToBoolean(hwParams,
                                        ZLS3077X_DPLL_MODE_NCO, &inNCOMode);
    }
    if (status == ZL303XX_OK && inNCOMode)
    {
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                "zl303xx_Dpll77xInNCOMode: DPLL=%d is in NCO mode",
                zl303xx_Params->pllParams.pllId, 0,0,0,0,0);
        return ZL303XX_TRUE;
    }
    else
    {
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                "zl303xx_Dpll77xInNCOMode: DPLL=%d is NOT in NCO mode",
                zl303xx_Params->pllParams.pllId ,0,0,0,0,0);
        return ZL303XX_FALSE;
    }
}


/* zl303xx_Dpll77xGetRefSyncPairMode */
/**
   Returns whether the current reference is a ref sync pair of the current DPLL.

  Parameters:
   [in]   hwParams         Pointer to a device parameter structure.
   [in]   refId            Reference input to query.
   [out]  syncId           Uint32T pointer, returns sync number of ref sync
                              pair.
   [out]  bRefSyncPair     Boolean pointer, returns ZL303XX_TRUE if reference
                              is a ref sync pair.
   [out]  bEPPS            Boolean pointer, returns ZL303XX_TRUE if reference
                              is an ePPS.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xGetRefSyncPairMode(void *hwParams, Uint32T refId, Uint32T *syncId,
                                        zl303xx_BooleanE *bRefSyncPair, zl303xx_BooleanE *bEPPS )
{
    zlStatusE status = ZL303XX_OK;
    osStatusT osStatus = OS_OK;
    zl303xx_ParamsS *zl303xx_Params = (zl303xx_ParamsS*)hwParams;
    Uint32T data;
    ZLS3077X_RefSynchPairModeE refSyncMode;


    ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
            "zl303xx_Dpll77xGetRefSyncPairMode: ", 0,0,0,0,0,0);

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_REF_ID(refId);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(syncId);
    }
    if (status == ZL303XX_OK)
    {
        status = ZLS3077X_CHECK_SYNC_ID(*syncId);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(bRefSyncPair);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(bEPPS);
    }
    if (status == ZL303XX_OK)
    {
        osStatus = OS_MUTEX_TAKE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetRefSyncPairMode: OS_MUTEX_TAKE failed: %d",
                    status, 0, 0, 0,0,0);
        }
    }
    if (status == ZL303XX_OK)
    {
        data = zl303xx_Params->pllParams.d77x.refMb[refId].ref_sync;
    }
    if (status == ZL303XX_OK)
    {
        osStatus = OS_MUTEX_GIVE(zl303xx_Params->pllParams.d77x.mailboxMutex);
        if( osStatus != OS_OK )
        {
            status = ZL303XX_RTOS_MUTEX_GIVE_FAIL;
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xGetRefSyncPairMode: OS_MUTEX_GIVE failed: %d",
                    status, 0, 0, 0,0,0);
        }
    }
    if (status == ZL303XX_OK)
    {
        *syncId = ZLS3077X_REF_CLK_SYNC_PAIR_GET(data);
        refSyncMode = ZLS3077X_REF_CLK_SYNC_MODE_GET(data);

        if (refSyncMode == ZLS3077X_RSPM_enabled)
        {
            *bRefSyncPair = ZL303XX_TRUE;
            *bEPPS = ZL303XX_FALSE;
        }
        else if ((refSyncMode >= ZLS3077X_RSPM_50_50_clk_25_75_PPS_sync) &&
                 (refSyncMode < ZLS3077X_RSPM_last))
        {
            *bRefSyncPair = ZL303XX_FALSE;
            *bEPPS = ZL303XX_TRUE;
        }
        else
        {
            *bRefSyncPair = ZL303XX_FALSE;
        }
    }
    if (status == ZL303XX_OK)
    {
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 4,
                "zl303xx_Dpll77xGetRefSyncPairMode: *syncId=%d  *bRefSyncPair=%d",
                *syncId, *bRefSyncPair, 0,0,0,0);
    }

    return status;
}


/* zl303xx_Dpll77xSetRefSyncPair */
/**
   Enables a ref+sync pair for a set reference

  Parameters:
   [in]   hwParams         Pointer to a device parameter structure.
   [out]  refId            Reference Id
   [out]  syncId           Sync Id.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xSetRefSyncPair(void *hwParams, Uint32T refId, Uint32T syncId)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T data = 0;
   zl303xx_ParamsS *zl303xx_Params = NULL;

   status = ZL303XX_CHECK_POINTER(hwParams);

   if (status == ZL303XX_OK)
   {
       zl303xx_Params = hwParams;
   }
   if (status == ZL303XX_OK)
   {
       status = zl303xx_Dpll77xSetupMailboxForWrite(hwParams, ZLS3077X_MB_ref, refId);
   }
   if (status == ZL303XX_OK)
   {
       data = ZLS3077X_REF_CLK_SYNC_MODE_GET(syncId) << 4; /* Upper 4 bits is the refId */
       data = data + ZLS3077X_RSPM_enabled;               /* Lower 4 bits is the anble flag */

       status = zl303xx_Write(zl303xx_Params, NULL,
                            ZLS3077X_REF_CLK_SYNC_PAIR_REG,
                            data);
   }
   if (status == ZL303XX_OK)
   {
       status = zl303xx_Dpll77xSetMailboxSemWrite(hwParams, ZLS3077X_MB_ref);
   }
   if (status == ZL303XX_OK)
   {
       status = zl303xx_Dpll77xUpdateMailboxCopy(hwParams, ZLS3077X_MB_ref, refId);
   }

   return status;
} /* zl303xx_Dpll77xSetRefSyncPair */





/* zl303xx_Dpll77xAccuStepTimePhase1588HostRegSet */
/**

  Parameters:
   [in]   hwParams   Pointer to a device parameter structure.
   [out]  tieNs      The integer structure to set the accumulated
                        (software performed) TIE write value, in nanoseconds.
                        No accumulation takes place in driver, calculation performed
                        in software.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xAccuStepTimePhase1588HostRegSet(void *hwParams, Sint32T tieNs)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ParamsS *zl303xx_Params;
   Uint32T pllId;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(hwParams);
   }

   if (status == ZL303XX_OK)
   {
      zl303xx_Params = hwParams;
      pllId = zl303xx_Params->pllParams.pllId;
   }

   if (status == ZL303XX_OK)
   {
      /* Ensure phase is between +/- 500ms */
      if (tieNs > 0)
         for (; tieNs > 500000000; tieNs -= TEN_e9);
      else
         for (; tieNs <= -500000000; tieNs += TEN_e9);

      status = zl303xx_Write(hwParams, NULL, ZLS3077X_PHASE_STEP_TIME_REG(pllId),
                             (Uint32T)tieNs);
   }
   return status;
}


/* zl303xx_Dpll77xAccuStepTimePhase1588HostRegGet */
/**

  Parameters:
   [in]   hwParams   Pointer to a device parameter structure.
   [out]  tieNs      Pointer to the integer structure to return the accumulated
                        (software performed) Step Time value, in nanoseconds.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xAccuStepTimePhase1588HostRegGet(void *hwParams, Sint32T *tieNs)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ParamsS *zl303xx_Params;
   Uint32T pllId;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(hwParams);
   }

   if (status == ZL303XX_OK)
   {
      zl303xx_Params = hwParams;
      pllId = zl303xx_Params->pllParams.pllId;
   }

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tieNs);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_Read(hwParams, NULL, ZLS3077X_PHASE_STEP_TIME_REG(pllId),
                             (Uint32T *)tieNs);
   }

   return status;
}


/* zl303xx_Dpll77xAccuStepTimePhase1588HostRegUpdate */
/**
    Adds deltaTime to the total store in the device's memory.

  Parameters:
   [in]   hwParams     Pointer to the device parameter structure.
   [in]   deltaTime    Signed time difference in nanoseconds.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
static Sint32T zl303xx_Dpll77xAccuStepTimePhase1588HostRegUpdate(void *hwParams, Sint32T deltaTime)
{
    zlStatusE status = ZL303XX_OK;
    Sint32T phase = 0;


    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    /* Accumulate phase for user in 1588 host registers. */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xAccuStepTimePhase1588HostRegGet(hwParams, &phase);
        if (status !=ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xAccuStepTimePhase1588HostRegUpdate: Call to zl303xx_Dpll77xAccuStepTimePhase1588HostRegGet() failed=%d", status, 0,0,0,0,0);
        }
    }
    if (status == ZL303XX_OK)
    {
        status = zl303xx_Dpll77xAccuStepTimePhase1588HostRegSet(hwParams, phase + deltaTime);
        if (status !=ZL303XX_OK)
        {
          ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xAccuStepTimePhase1588HostRegUpdate: Call to zl303xx_Dpll77xAccuStepTimePhase1588HostRegSet() failed=%d", status, 0,0,0,0,0);
        }
    }

    return status;
}


/* zl303xx_Dpll77xAccuInputPhaseError1588HostRegSet */
/**

  Parameters:
   [in]   hwParams   Pointer to a device parameter structure.
   [out]  tieNs      The integer structure to set the accumulated
                        (software performed) TIE write value, in nanoseconds.
                        No accumulation takes place in driver, calculation performed
                        in software.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xAccuInputPhaseError1588HostRegSet(void *hwParams, Sint32T tieNs)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ParamsS *zl303xx_Params;
   Uint32T pllId;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(hwParams);
   }

   if (status == ZL303XX_OK)
   {
       zl303xx_Params = hwParams;
       pllId = zl303xx_Params->pllParams.pllId;
   }

   if (status == ZL303XX_OK)
   {
      /* Ensure phase is between +/- 500ms */
      if (tieNs > 0)
         for (; tieNs > 500000000; tieNs -= TEN_e9);
      else
         for (; tieNs <= -500000000; tieNs += TEN_e9);

      status = zl303xx_Write(hwParams, NULL, ZLS3077X_INPUT_PHASE_STEP_REG(pllId),
                             (Uint32T)tieNs);
   }
   return status;
}


/* zl303xx_Dpll77xAccuInputPhaseError1588HostRegGet */
/**

  Parameters:
   [in]   hwParams   Pointer to a device parameter structure.
   [out]  tieNs      Pointer to the integer structure to return the accumulated
                        (software performed) Step Time value, in nanoseconds.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xAccuInputPhaseError1588HostRegGet(void *hwParams, Sint32T *tieNs)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_ParamsS *zl303xx_Params;
   Uint32T pllId;

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(hwParams);
   }

   if (status == ZL303XX_OK)
   {
       zl303xx_Params = hwParams;
       pllId = zl303xx_Params->pllParams.pllId;
   }

   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_POINTER(tieNs);
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_Read(hwParams, NULL, ZLS3077X_INPUT_PHASE_STEP_REG(pllId),
                             (Uint32T *)tieNs);
   }

   return status;
}


/* zl303xx_Dpll77xReset1588HostRegisters */
/**
   Resets the 1588 host registers which are not included in the GUI generated configuration file.

  Parameters:
   [in]   hwParams   Pointer to a device parameter structure.

  Return Value:  ZL303XX_OK  Success.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xReset1588HostRegisters(void *hwParams)
{
   zl303xx_ParamsS *zl303xx_Params = NULL;
   zlStatusE status = ZL303XX_OK;
   zlStatusE retStatus = ZL303XX_OK;

   if ( status == ZL303XX_OK )
   {
       status = ZL303XX_CHECK_POINTER(hwParams);
   }

   if (status == ZL303XX_OK)
   {
       zl303xx_Params = hwParams;
   }

   if ( status == ZL303XX_OK )
   {
       /* Clear specific 1588 reserved registers based upon pllId */
       Uint32T address;
       ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xReset1588HostRegisters: reset 1588 host registers.",0,0,0,0,0,0 );

       /* Write register value to the device */
       address = ZLS3077X_PHASE_STEP_TIME_REG(zl303xx_Params->pllParams.pllId);
       if ((status = zl303xx_Write(zl303xx_Params, NULL, address, 0x0)) != ZL303XX_OK)
       {
           ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xReset1588HostRegisters: Write to addr=0x%x failed with ret=%d.",address,status,0,0,0,0 );
           retStatus = ZL303XX_ERROR;
       }

       address = ZLS3077X_INPUT_PHASE_STEP_REG(zl303xx_Params->pllParams.pllId);
       if ((status = zl303xx_Write(zl303xx_Params, NULL, address, 0x0)) != ZL303XX_OK)
       {
           ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xReset1588HostRegisters: Write to addr=0x%x failed with ret=%d.",address,status,0,0,0,0 );
           retStatus = ZL303XX_ERROR;
       }
   }

   if ( status == ZL303XX_OK )
   {
       return retStatus;
   }
   else
   {
       return status;
   }
}


/* zl303xx_Dpll77xStoreDeviceParam */
/**

    Stores hwParams.

    This routine is called each time a new device is added. The new device is
    added to a list that is checked every time the hardware timer expires
    (default=125ms).

  Parameters:
   [in]   zl303xx_Params  Pointer to a device parameter structure.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_Dpll77xStoreDeviceParam(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T idx;
    zl303xx_BooleanE found;


    if( status == ZL303XX_OK )
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if( status == ZL303XX_OK )
    {
        for( idx = 0; idx < ZL303XX_MAX_NUM_77X_PARAMS; idx++ )
        {
            if( zl303xx_Params77x[idx] == hwParams )
            {
                /* Entry already exists */
                status = ZL303XX_TABLE_ENTRY_DUPLICATE;
            }
        }
    }
    if( status == ZL303XX_OK )
    {
        status = ZL303XX_TABLE_FULL;
        idx = 0;
        found = ZL303XX_FALSE;
        while(( found == ZL303XX_FALSE ) && (idx < ZL303XX_MAX_NUM_77X_PARAMS))
        {
            if( zl303xx_Params77x[idx] == 0 )
            {
                status = ZL303XX_OK;
                found = ZL303XX_TRUE;
            }
            else
            {
                idx++;
            }
        }
        if( found == ZL303XX_TRUE )
        {
            zl303xx_Params77x[idx] = hwParams;
            ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
                        "zl303xx_Dpll77xStoreDeviceParam: Added: %x  idx: %d",
                        (Uint32T *)hwParams, idx,0,0,0,0);
            ZL303XX_TRACE_ALWAYS( "zl303xx_Dpll77xStoreDeviceParam: Added: %x  idx: %d",
                        (Uint32T *)hwParams, idx,0,0,0,0);
        }
    }

    return ( status );
}


/* zl303xx_Dpll77xRemoveDeviceParam */
/**

    Removes hwParams from the drivers data structure.

  Parameters:
   [in]   zl303xx_Params  Pointer to a device parameter structure.

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_Dpll77xRemoveDeviceParam(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T idx;
    zl303xx_BooleanE found;


    status = ZL303XX_TABLE_FULL;
    idx = 0;
    found = ZL303XX_FALSE;
    while(( found == ZL303XX_FALSE ) && (idx < ZL303XX_MAX_NUM_77X_PARAMS))
    {
        if( zl303xx_Params77x[idx] == hwParams )
        {
            status = ZL303XX_OK;
            found = ZL303XX_TRUE;
        }
        else
        {
            idx++;
        }
    }
    if( found == ZL303XX_TRUE )
    {
        zl303xx_Params77x[idx] = NULL;
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
                    "zl303xx_Dpll77xRemoveDeviceParam: Removed: %x  idx: %d",
                    (Uint32T *)hwParams, idx,0,0,0,0);
    }
    else
    {
        status = ZL303XX_PARAMETER_INVALID;
        ZL303XX_TRACE(ZL303XX_MOD_ID_PLL, 2,
                    "zl303xx_Dpll77xRemoveDeviceParam: Remove FAILED!: %x",
                    (Uint32T *)hwParams, 0,0,0,0,0);
    }

    return ( status );
}


/* zl303xx_Dpll77xRegisterStepDoneFunc */
/**
   Registers a routine that is called when setTime() is done.

  Parameters:
   [in]  zl303xx_Params    Pointer to the device parameter structure
   [in]  stepDoneFuncPtr Pointer to the routine to call

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_Dpll77xRegisterStepDoneFunc(void *hwParams,
                        hwFuncPtrTODDone stepDoneFuncPtr)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_ParamsS *zl303xx_Params = hwParams;


    if( status == ZL303XX_OK )
    {
        status = ZL303XX_CHECK_POINTER(zl303xx_Params);
    }

    if( status == ZL303XX_OK )
    {
        zl303xx_Params->pllParams.d77x.std.stepDoneFuncPtr = stepDoneFuncPtr;
    }

    return( status );
}


/* zl303xx_Dpll77xSetCOHybridParamsSAssociation */
/**
   Routine to associate 2 DPLLs.

   Needed for the NCO-Assist DPLL.

*******************************************************************************/
zl303xx_ParamsS *zl303xx_ParamsAPR;
zl303xx_ParamsS *zl303xx_ParamsNCOAssist;

zlStatusE zl303xx_Dpll77xSetNCOAssistParamsSAssociation(zl303xx_ParamsS *APR,
                                                      zl303xx_ParamsS *NCOAssist)
{
    zlStatusE status = ZL303XX_OK;

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(APR);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(NCOAssist);
    }
    if (status == ZL303XX_OK)
    {
        zl303xx_ParamsAPR = APR;
        zl303xx_ParamsNCOAssist = NCOAssist;
    }
    return status;
}


/* zl303xx_Dpll77xGetNCOAssistParamsSAssociation */
/**
   Routine to get the associated DPLLs given APR's DPLL.

   Needed for the NCO-Assist DPLL.

*******************************************************************************/
zlStatusE zl303xx_Dpll77xGetNCOAssistParamsSAssociation(zl303xx_ParamsS *APR,
                                                      zl303xx_ParamsS **NCOAssist)
{
    zlStatusE status = ZL303XX_OK;

    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(APR);
    }
    if (status == ZL303XX_OK)
    {
        status = ZL303XX_CHECK_POINTER(NCOAssist);
    }
    if (status == ZL303XX_OK)
    {
        if( APR == zl303xx_ParamsAPR )
        {
            *NCOAssist = zl303xx_ParamsNCOAssist;
        }
        else
        {
            status = ZL303XX_PARAMETER_INVALID;
        }
    }
    return status;
}


#define ZLS3077X_DPLL_TASK_NAME     "zlDpll77xTask"
#ifdef OS_VXWORKS
#define ZLS3077X_DPLL_TASK_PRIORITY (Uint32T)33
#endif
#ifdef OS_LINUX
#define ZLS3077X_DPLL_TASK_PRIORITY (Uint32T)80
#endif
#define ZLS3077X_DPLL_TASK_STACK_SZ 16384

static OS_TASK_ID zl303xx_Dpll77xTaskId = OS_TASK_INVALID;
static volatile zl303xx_BooleanE zl303xx_Dpll77xTaskRunning = ZL303XX_FALSE;


/**
  Function Name:
   zl303xx_Dpll77xTaskFn

  Details:
   Runs the 77x's step time state machine.

  Return Value: zlStatusE

*******************************************************************************/
static zlStatusE zl303xx_Dpll77xTaskFn(Uint32T arg)
{
    zlStatusE status = ZL303XX_OK;

    if(arg){}; /* Warning removal */

    zl303xx_Dpll77xTaskRunning = ZL303XX_TRUE;

    while (zl303xx_Dpll77xTaskRunning && status == ZL303XX_OK)
    {
        OS_TASKMON_FUNC_START();

        zl303xx_Dpll77xStepTimeStateMachine();

        OS_TASKMON_FUNC_END();

        OS_TASK_DELAY(ZLS3077X_TIMER_DELAY_MS);
    }

    if (status != ZL303XX_OK)
        ZL303XX_TRACE_ALWAYS("zl303xx_Dpll77xTask: Exit loop with failure= %d", status, 0,0,0,0,0);

    OS_TASK_DELAY(2000);    /* Wait to be deleted */

    return status;
}


/**
  Function Name:
   zl303xx_Dpll77xTaskStart

  Details:
   Starts the task that runs the 77x's step time state machine.

  Return Value: zlStatusE

*******************************************************************************/
zlStatusE zl303xx_Dpll77xTaskStart(void)
{
    zlStatusE status = ZL303XX_OK;

    if (zl303xx_Dpll77xTaskId != OS_TASK_INVALID)
    {
        return ZL303XX_OK;
    }
    zl303xx_Dpll77xTaskId = OS_TASK_SPAWN(ZLS3077X_DPLL_TASK_NAME,
                                        ZLS3077X_DPLL_TASK_PRIORITY,
                                        0,
                                        ZLS3077X_DPLL_TASK_STACK_SZ,
                                        (OS_TASK_FUNC_PTR)&zl303xx_Dpll77xTaskFn,
                                        0);

    if (zl303xx_Dpll77xTaskId == OS_TASK_INVALID)
    {
        zl303xx_Dpll77xTaskRunning = ZL303XX_FALSE;
        status = ZL303XX_RTOS_TASK_CREATE_FAIL;
        ZL303XX_ERROR_TRAP("zl303xx_Dpll77xTaskStart: Could not spawn DPLL 77X task!");
    }

    return status;
}


/**
  Function Name:
   zl303xx_Dpll77xTaskStop

  Details:
   Stops the task that runs the 77x's step time state machine.

  Return Value: zlStatusE

*******************************************************************************/
zlStatusE zl303xx_Dpll77xTaskStop(void)
{
    zlStatusE status = ZL303XX_OK;

    zl303xx_Dpll77xTaskRunning = ZL303XX_FALSE;

    OS_TASK_DELAY(1000);     /* ms */

    if (zl303xx_Dpll77xTaskId != OS_TASK_INVALID)
    {
        if (OS_TASK_DELETE(zl303xx_Dpll77xTaskId) != OS_OK)
        {
            status = ZL303XX_RTOS_TASK_DELETE_FAIL;
        }

        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("Could not delete DPLL 77X task, failed= %d", status, 0,0,0,0,0);
        }

        zl303xx_Dpll77xTaskId = OS_TASK_INVALID;

   }

    return status;
}


/*****************   END   ****************************************************/
