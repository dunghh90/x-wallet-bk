

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains feature support for ZL303XX_ variants.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#if defined ZLS30341_INCLUDED
#include "zl303xx.h"        /* Required for zl303xx_ParamS Definition   */
#else
#include "zl303xx_DeviceSpec.h"
#endif
#include "zl303xx_RdWr.h"
#include "zl303xx_Var.h"
#include "zl303xx_Trace.h"

#if defined ZLS30341_INCLUDED
#include "zl303xx_Dpll34xDutLow.h"
#endif
#if defined ZLS30751_INCLUDED
#include "zl303xx_AddressMap75x.h"
#endif
#if defined ZLS30721_INCLUDED
#include "zl303xx_AddressMap72x.h"
#endif
#if defined ZLS30701_INCLUDED
#include "zl303xx_AddressMap70x.h"
#endif
#if defined ZLS30361_INCLUDED
#include "zl303xx_AddressMap36x.h"
#endif
#if defined ZLS30341_INCLUDED
#include "zl303xx_AddressMap34x.h"
#endif
#include "zl303xx_AddressMap.h"

#if defined ZLS30771_INCLUDED
#include "zl303xx_AddressMap77x.h"
#endif

/*****************   DEFINES     **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/**

  Function Name:
   zl303xx_UpdateDeviceIdAndRev

  Details:
   Fills the Device ID and Revision fields in zl303xx_Params

  Parameters:
   [in]    zl303xx_Params      Pointer to the device instance parameter structure

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_InitDeviceIdAndRev(zl303xx_ParamsS *zl303xx_Params)
{
    zlStatusE status = ZL303XX_CHECK_POINTER(zl303xx_Params);
    Uint32T regValue = 0;

   /* Read register containing Device ID and Revision */
   if(status == ZL303XX_OK)
   {
#if defined ZLS30721_INCLUDED
      if( zl303xx_Params->deviceType == ZL3072X_DEVICETYPE )
      {
         status = zl303xx_Read(zl303xx_Params, NULL, ZLS3072X_ID2_REG, &regValue);

         zl303xx_Params->deviceId = (regValue & ZL303XX_DEVICE_ID_MASK_72X) >> ZL303XX_DEVICE_ID_SHIFT_72X;
         zl303xx_Params->deviceRev = (regValue & ZL303XX_DEVICE_REV_MASK_72X) >> ZL303XX_DEVICE_REV_SHIFT_72X;
      }
#endif
#if defined ZLS30361_INCLUDED
      if( zl303xx_Params->deviceType == ZL3036X_DEVICETYPE )
      {
         status = zl303xx_Read(zl303xx_Params, NULL, ZLS3036X_DEVICE_ID_REG, &regValue);

         zl303xx_Params->deviceId = regValue;
         status = zl303xx_Read(zl303xx_Params, NULL, ZLS3036X_DEVICE_REV_REG, &regValue);
         if (status == ZL303XX_OK)
         {
            #define ZL3036X_PROD_HW_REV 0x03

            zl303xx_Params->deviceRev = regValue;
            if (zl303xx_Params->deviceId < ZL30361_DEV_TYPE)
            {
                ZL303XX_TRACE_ALWAYS("This code is intended to run on ZL3036X devices", 0,0,0,0,0,0);
            }
            if (zl303xx_Params->deviceRev < ZL3036X_PROD_HW_REV)
            {
                ZL303XX_TRACE_ALWAYS("This code should only be run on PRODUCTION ZL3036X devices", 0,0,0,0,0,0);
            }
         }
      }
#endif
#if defined ZLS30701_INCLUDED
      if( zl303xx_Params->deviceType == ZL3070X_DEVICETYPE )
      {
         status = zl303xx_Read(zl303xx_Params, NULL, ZLS3070X_DEVICE_ID_REG, &regValue);

         zl303xx_Params->deviceId = regValue;
         status = zl303xx_Read(zl303xx_Params, NULL, ZLS3070X_DEVICE_REV_REG, &regValue);
         if (status == ZL303XX_OK)
         {
            zl303xx_Params->deviceRev = regValue;
            if ((zl303xx_Params->deviceId < ZL303XX_DEVICE_ID_ZL30174) ||
                (zl303xx_Params->deviceId > ZL303XX_DEVICE_ID_ZL80029))
            {
               ZL303XX_TRACE_ALWAYS("This code is intended to run on ZL3070X devices", 0,0,0,0,0,0);
            }
         }
      }
#endif
#if defined ZLS30751_INCLUDED
      if( zl303xx_Params->deviceType == ZL3075X_DEVICETYPE )
      {
         status = zl303xx_Read(zl303xx_Params, NULL, ZLS3075X_DEVICE_ID_REG, &regValue);

         zl303xx_Params->deviceId = regValue;
         status = zl303xx_Read(zl303xx_Params, NULL, ZLS3075X_DEVICE_REV_REG, &regValue);
         if (status == ZL303XX_OK)
         {
            zl303xx_Params->deviceRev = regValue;
            if ((zl303xx_Params->deviceId < ZL303XX_DEVICE_ID_ZL30652) ||
                (zl303xx_Params->deviceId > ZL303XX_DEVICE_ID_ZL30754))
            {
               ZL303XX_TRACE_ALWAYS("This code is intended to run on ZL3075X devices", 0,0,0,0,0,0);
            }
         }
      }
#endif
#if defined ZLS30341_INCLUDED
      if(( zl303xx_Params->deviceType == ZL3034X_DEVICETYPE )
        )
      {
         status = zl303xx_Read(zl303xx_Params, NULL, 0x0, &regValue);

         zl303xx_Params->deviceId = (Uint8T)((regValue >> ZL303XX_DUT_ID_SHIFT) & ZL303XX_DUT_ID_MASK);
         zl303xx_Params->deviceRev = (Uint8T)((regValue >> ZL303XX_DUT_REVISION_SHIFT) & ZL303XX_DUT_REVISION_MASK);
      }
#endif
#if defined ZLS30771_INCLUDED
      if( zl303xx_Params->deviceType == ZL3077X_DEVICETYPE )
      {
         status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DEVICE_ID_REG, &regValue);

         zl303xx_Params->deviceId = regValue;
         status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DEVICE_REV_REG, &regValue);
         if (status == ZL303XX_OK)
         {
            zl303xx_Params->deviceRev = regValue;
            if (zl303xx_Params->deviceId < ZL303XX_DEVICE_ID_ZL30773)
            {
               ZL303XX_TRACE_ALWAYS("This code is intended to run on ZL3077X devices", 0,0,0,0,0,0);
            }
         }
      }
#endif
   }


 return status;
}


/**

  Function Name:
   zl303xx_GetDeviceId

  Details:
   Returns the Device ID stored in zl303xx_Params

  Parameters:
   [in]    zl303xx_Params      Pointer to the device instance parameter structure

   [out]    deviceId          Pointer to Uint8T Device ID

  Return Value:
   zlStatusE


*******************************************************************************/
zlStatusE zl303xx_GetDeviceId(zl303xx_ParamsS *zl303xx_Params, Uint8T *deviceId)
{
    zlStatusE status = ZL303XX_CHECK_POINTER(zl303xx_Params) |
                       ZL303XX_CHECK_POINTER(deviceId);

    if (status == ZL303XX_OK)
    {
        *deviceId = zl303xx_Params->deviceId;
    }

    return status;
}


/**

  Function Name:
   zl303xx_GetDeviceRevision

  Details:
   Returns the Device Revision stored in zl303xx_Params

  Parameters:
   [in]    zl303xx_Params      Pointer to the device instance parameter structure

   [out]    revision          Pointer to Uint8T Device Revision

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_GetDeviceRev(zl303xx_ParamsS *zl303xx_Params, Uint8T *revision)
{
    zlStatusE status = ZL303XX_CHECK_POINTER(zl303xx_Params) |
                       ZL303XX_CHECK_POINTER(revision);

    if (status == ZL303XX_OK)
    {
        *revision = zl303xx_Params->deviceRev;
    }

    return status;
}

/**

  Function Name:
   zl303xx_PrintDeviceId

  Details:
   Prints the Device ID to the console

  Parameters:
   [in]    zl303xx_Params      Pointer to the device instance parameter structure

  Return Value:
   N/A

*******************************************************************************/
void zl303xx_PrintDeviceId(zl303xx_ParamsS *zl303xx_Params)
{
    if (ZL303XX_CHECK_POINTER(zl303xx_Params) == ZL303XX_OK)
    {
        ZL303XX_TRACE_ALWAYS("Device Id = %d", zl303xx_Params->deviceId, 0,0,0,0,0);
    }
    else
    {
        ZL303XX_TRACE_ALWAYS("Invalid pointer.",0,0,0,0,0,0);
    }
}


/**

  Function Name:
   zl303xx_PrintDeviceRev

  Details:
   Prints the Device Revision to the console

  Parameters:
   [in]    zl303xx_Params      Pointer to the device instance parameter structure

  Return Value:
   N/A

*******************************************************************************/
void zl303xx_PrintDeviceRev(zl303xx_ParamsS *zl303xx_Params)
{
    if (ZL303XX_CHECK_POINTER(zl303xx_Params) == ZL303XX_OK)
    {
        ZL303XX_TRACE_ALWAYS("Device Revision = %d", zl303xx_Params->deviceRev, 0,0,0,0,0);
    }
    else
    {
        ZL303XX_TRACE_ALWAYS("Invalid pointer.",0,0,0,0,0,0);
    }
}


/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/**
  Function Name:
   zl303xx_ReadDeviceId

  Details:
   Calls zl303xx_DutIdGet() to read Device ID from chip then converts to Uint8T

  Parameters:
   [in]    zl303xx_Params    Pointer to the device instance parameter structure

   [out]    deviceId        Pointer to Uint8T Device ID

  Return Value:
    zlStatusE

 *****************************************************************************/
zlStatusE zl303xx_ReadDeviceId(zl303xx_ParamsS *zl303xx_Params, Uint8T *deviceId)
{
    zlStatusE status = ZL303XX_INIT_ERROR;
    zl303xx_DeviceIdE id = 0;

    /* zl303xx_Params is not referenced locally, so let called functions check */

#if defined ZLS30721_INCLUDED
    if( zl303xx_Params->deviceType == ZL3072X_DEVICETYPE )
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3072X_ID2_REG, &id);
        if( status == ZL303XX_OK )
        {
            id = (id & ZL303XX_DEVICE_ID_MASK_72X) >> ZL303XX_DEVICE_ID_SHIFT_72X;
        }
    }
#endif
#if defined ZLS30361_INCLUDED
    if( zl303xx_Params->deviceType == ZL3036X_DEVICETYPE )
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3036X_DEVICE_ID_REG, (Uint32T*)&id);
    }
#endif
#if defined ZLS30701_INCLUDED
    if( zl303xx_Params->deviceType == ZL3070X_DEVICETYPE )
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3070X_DEVICE_ID_REG, (Uint32T*)&id);
    }
#endif
#if defined ZLS30751_INCLUDED
    if( zl303xx_Params->deviceType == ZL3075X_DEVICETYPE )
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3075X_DEVICE_ID_REG, (Uint32T*)&id);
    }
#endif
#if defined ZLS30341_INCLUDED
    if(( zl303xx_Params->deviceType == ZL3034X_DEVICETYPE )
      )
    {
        status = zl303xx_DutIdGet(zl303xx_Params, &id);
    }
#endif
#if defined ZLS30771_INCLUDED
    if( zl303xx_Params->deviceType == ZL3077X_DEVICETYPE )
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DEVICE_ID_REG, (Uint32T*)&id);
    }
#endif

    if (status == ZL303XX_OK)
    {
        *deviceId = (Uint8T)id;
    }

    return status;
}


/**
  Function Name:
   zl303xx_ReadDeviceRevision

  Details:
   Calls zl303xx_DutRevisionGet() to read Rev ID from chip then converts to Uint8T

  Parameters:
   [in]    zl303xx_Params    Pointer to the device instance parameter structure

   [out]    revision        Pointer to Uint8T Device Revision

  Return Value:
    zlStatusE

 *****************************************************************************/
zlStatusE zl303xx_ReadDeviceRevision(zl303xx_ParamsS *zl303xx_Params, Uint8T *revision)
{
    zlStatusE status = ZL303XX_INIT_ERROR;
    Uint32T rev = 0;

    /* zl303xx_Params is not referenced locally, so let called functions check */

#if defined ZLS30721_INCLUDED
    if( zl303xx_Params->deviceType == ZL3072X_DEVICETYPE )
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3072X_ID2_REG, &rev);
        if( status == ZL303XX_OK )
        {
            rev = (rev & ZL303XX_DEVICE_REV_MASK_72X) >> ZL303XX_DEVICE_REV_SHIFT_72X;
        }
    }
#endif
#if defined ZLS30361_INCLUDED
    if( zl303xx_Params->deviceType == ZL3036X_DEVICETYPE )
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3036X_DEVICE_REV_REG, &rev);
    }
#endif
#if defined ZLS30701_INCLUDED
    if( zl303xx_Params->deviceType == ZL3070X_DEVICETYPE )
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3070X_DEVICE_REV_REG, &rev);
    }
#endif
#if defined ZLS30751_INCLUDED
    if( zl303xx_Params->deviceType == ZL3075X_DEVICETYPE )
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3075X_DEVICE_REV_REG, &rev);
    }
#endif
#if defined ZLS30341_INCLUDED
    if(( zl303xx_Params->deviceType == ZL3034X_DEVICETYPE )
      )
    {
        status = zl303xx_DutRevisionGet(zl303xx_Params, &rev);
    }
#endif
#if defined ZLS30771_INCLUDED
    if( zl303xx_Params->deviceType == ZL3077X_DEVICETYPE )
    {
        status = zl303xx_Read(zl303xx_Params, NULL, ZLS3077X_DEVICE_REV_REG, &rev);
    }
#endif
    if (status == ZL303XX_OK)
    {
        *revision = (Uint8T)rev;
    }

    return status;
}


/*****************   END   ****************************************************/

