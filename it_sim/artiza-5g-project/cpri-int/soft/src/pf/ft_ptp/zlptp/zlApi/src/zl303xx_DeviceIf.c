



/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains functions related to the ...
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_DeviceIf.h"

/*****************   DEFINES     **********************************************/

/*****************   STATIC DATA TYPES   **************************************/

/*****************   STATIC DATA STRUCTURES   *********************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

zl303xx_DevTypeE       globalDevType      = CUSTOM_DEVICETYPE;


/* zl303xx_GetDefaultDeviceType */
/**
    Function to get the device family.

    This routine may be used early in initilisation before the device is
    available.

*******************************************************************************/
zl303xx_DevTypeE zl303xx_GetDefaultDeviceType(void)
{
    return( globalDevType );
}


/* zl303xx_SetDefaultDeviceFamily */
/**
    Function to set the device family.

*******************************************************************************/
unsigned int zl303xx_SetDefaultDeviceType(zl303xx_DevTypeE f)
{
    globalDevType = f;

    return 0;
}


/*****************   END   ****************************************************/
