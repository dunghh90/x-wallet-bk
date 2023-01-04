



/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains miscellaneous APR debug functions.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_DebugApr.h"
#include "zl303xx_AprStatistics.h"
#include "zl303xx_Apr.h"
#include "zl303xx_Apr1Hz.h"


#include "zl303xx_ExampleAprGlobals.h"



#include <string.h>

/* Print pointer macro */
#define PRINT_PTR(ptr)                          \
      {                                         \
         Uint8T *p = (Uint8T *)(&(ptr));          \
         Uint8T idx;                            \
                                                \
         printf("0x");                          \
         for (idx = 0; idx < sizeof (ptr); idx++) \
         {                                      \
            printf("%02x", p[idx]);             \
         }                                      \
      }

/*****************   IMPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

extern const unsigned int zl303xx_AprReleaseVersionInt;


/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/**

  Function Name:
   zl303xx_GetAprDeviceConfigInfo

  Details:
    zl303xx code to show how to retrieve any desired configuration data for the
    specified device by calling API provided by APR

  Parameters:
   [in]   hwParams   Pointer to the device instance

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE zl303xx_GetAprDeviceConfigInfo(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_AprDeviceRefModeE devRefMode;


    if (hwParams != NULL)
    {
        printf("Device %p configuration:\n", hwParams);
    }
    else
    {
        return ZL303XX_INVALID_POINTER;
    }


    status = zl303xx_AprGetDeviceRefMode(hwParams, &devRefMode);
    if (status == ZL303XX_OK)
    {
        printf("     The reference clock mode is %d\n", devRefMode);
    }
    else
    {
        return status;
    }
    if (devRefMode != ZL303XX_ELECTRIC_MODE)
    {
    }


    return status;

}


/* zl303xx_DebugCGULockState */
/**
   Displays the CGU lock state - Assumes Server0 for V3

  Parameters:
   [in]   zl303xx_Params  Pointer to the clock device of interest.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugCGULockState(void *hwParams)
{
   zlStatusE status = ZL303XX_OK;

    if (status == ZL303XX_OK)
    {
        zl303xx_AprCGUStatusFlagsS cguStatusFlags;

        status = zl303xx_AprGetCGUStatusFlags(hwParams, &cguStatusFlags);
        if (status == ZL303XX_OK)
        {
            printf("\nCGUStatusFlags (details):\n");
            printf("Device = %p:\n\thwH = %d, hwL = %d, RF = %d, SE = %d, hwMH = %d, hwMF = %d\n",
                   hwParams,
                   cguStatusFlags.hwH, cguStatusFlags.hwL, cguStatusFlags.RF, cguStatusFlags.SE,
                   cguStatusFlags.hwMH, cguStatusFlags.hwMF);
            printf("\tswL = %d, swH = %d, V = %d, swMH = %d\n",
                   cguStatusFlags.swL, cguStatusFlags.swH, cguStatusFlags.V,
                   cguStatusFlags.swMH);
            printf("\tTS = %d, WS = %d\n\n",
                   cguStatusFlags.TS, cguStatusFlags.WS);

            printf("Device Lock state = ");
            switch(cguStatusFlags.state)
            {
                case ZL303XX_FREQ_LOCK_ACQUIRING :
                    printf("ZL303XX_FREQ_LOCK_ACQUIRING \n");
                    break;
                case ZL303XX_FREQ_LOCK_ACQUIRED :
                    printf("ZL303XX_FREQ_LOCK_ACQUIRED \n");
                    break;
                case ZL303XX_PHASE_LOCK_ACQUIRED :
                    printf("ZL303XX_PHASE_LOCK_ACQUIRED \n");
                    break;
                case ZL303XX_HOLDOVER :
                    printf("ZL303XX_HOLDOVER \n");
                    break;
                case ZL303XX_REF_FAILED :
                    printf("ZL303XX_REF_FAILED \n");
                    break;
                case ZL303XX_NO_ACTIVE_SERVER :
                    printf("ZL303XX_NO_ACTIVE_SERVER \n");
                    break;
                case ZL303XX_MANUAL_FREERUN :
                    printf("ZL303XX_MANUAL_FREERUN \n");
                    break;
                case ZL303XX_MANUAL_HOLDOVER :
                    printf("ZL303XX_MANUAL_HOLDOVER \n");
                    break;
                case ZL303XX_MANUAL_SERVO_HOLDOVER :
                    printf("ZL303XX_MANUAL_SERVO_HOLDOVER \n");
                    break;
                    case ZL303XX_UNKNOWN :
                    default :
                        printf("Invalid lock type: %d \n", cguStatusFlags.state);
                        return ZL303XX_ERROR;
                        break;
            }

            printf("Soft Reset Status = %d\n", cguStatusFlags.RStatus);
            printf("Validity of S/W Holdover Value = %d\n", cguStatusFlags.swHV);
            printf("Dropped Packets are more than 50%% = %d\n", cguStatusFlags.DroppedPackets);
        }
    }

    return status;
}

/**

  Function Name:
   zl303xx_GetAprDeviceStatus

  Details:
    Example code to retrieve the device status information for the specified device

  Parameters:
   [in]   hwParams   Pointer to the device instance

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE zl303xx_GetAprDeviceStatus(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_AprDeviceRefModeE devRefMode;
    Uint16T serverNum;
    Sint32T freqOffset, phaseStabilityIndicator;
    Uint64S phaseAlignAccuracy;


    if (hwParams != NULL)
    {
        printf("Device = %p\n", hwParams);
    }
    else
    {
        printf("Device = %p\n", hwParams);
        return ZL303XX_INVALID_POINTER;
    }

    status = zl303xx_AprGetDeviceRefMode(hwParams, &devRefMode);
    if (status == ZL303XX_OK)
    {
        printf("\tThe reference clock mode is %d\n", devRefMode);
    }
    else
    {
        printf(" Call to zl303xx_AprGetDeviceRefMode() failed=%d\n", status);
        return status;
    }


    if (devRefMode != ZL303XX_ELECTRIC_MODE)
    {
        status = zl303xx_AprGetDeviceCurrActiveRef(hwParams, &serverNum);
        if (status == ZL303XX_OK || status == ZL303XX_UNSUPPORTED_OPERATION)
        {
            printf("\tThe current reference clock is %d\n", serverNum);
            if (status == ZL303XX_UNSUPPORTED_OPERATION)
            {
                printf("\t1Hz data not applicable due to serverId= %d\n", serverNum);
                goto skip1Hz;
            }
        }
        else
        {
            printf(" Call to zl303xx_AprGetDeviceCurrActiveRef() failed=%d\n", status);
            return status;
        }

        status = zl303xx_AprGetDeviceRegisterServerNum(hwParams, &serverNum);
        if (status == ZL303XX_OK)
        {
            printf("\tThe registered packet/hybrid server count is %u\n", serverNum);
        }
        else
        {
            printf(" Call to zl303xx_AprGetDeviceRegisterServerNum() failed=%d\n", status);
            return status;
        }

        status = zl303xx_AprGetDevicePhaseStabilityIndicator(hwParams, &phaseStabilityIndicator);
        if (status == ZL303XX_OK)
        {
            printf("\tThe phase stability factor is %d ns\n", phaseStabilityIndicator);
        }
        else
        {
            printf(" Call to zl303xx_AprGetDevicePhaseStabilityIndicator() failed=%d\n", status);
            return status;
        }

        /* This is the last 1Hz alignment that was applied */
        status = zl303xx_AprGetDevicePhaseAlignmentAccuracy(hwParams, &phaseAlignAccuracy);
        if (status == ZL303XX_OK)
        {
            Sint64T T64_phaseAlignAccuracy;
            char sign[2] = { 0 };

            ZL303XX_CONVERT_TO_64_SIGNED(T64_phaseAlignAccuracy, phaseAlignAccuracy);

            if( T64_phaseAlignAccuracy < 0 )
            {
                T64_phaseAlignAccuracy = -T64_phaseAlignAccuracy;
                sign[0] = '-';
            }
            ZL303XX_CONVERT_FROM_64(phaseAlignAccuracy, T64_phaseAlignAccuracy);

            printf("\tThe phase alignment accuracy is %s0x%X%08X ns\n",
                    sign, phaseAlignAccuracy.hi, phaseAlignAccuracy.lo);
        }
        else
        {
            printf(" Call to zl303xx_AprGetDevicePhaseAlignmentAccuracy() failed=%d\n", status);
            return status;
        }

    {
            Uint16T serverId;
            zl303xx_AprConfigure1HzS fwd1HzPar;
            zl303xx_AprConfigure1HzS rev1HzPar;
        Uint64S mostRecentCalculatedOffset;
        Uint32T mostRecentEstimatedNetworkQuality;
        zl303xx_BooleanE mostRecentAssigned;


            status = zl303xx_AprGetCurrent1HzConfigData(hwParams, &serverId, &fwd1HzPar, &rev1HzPar);
            if (status == ZL303XX_OK)
            {
                if(( fwd1HzPar.disabled == ZL303XX_TRUE ) || ( rev1HzPar.disabled == ZL303XX_TRUE ))
                {
                    printf("\t  Note: 1Hz is NOT enabled on forward or reverse path.\n" );
                }
                else
                {
                    /* This is the current 1Hz alignment */
                    status = zl303xx_AprGetDevice1HzMostRecentAlignmentDifference(hwParams,
                                                    &mostRecentCalculatedOffset,
                                                    &mostRecentEstimatedNetworkQuality,
                                                    &mostRecentAssigned );
                    if (status == ZL303XX_OK)
                    {
                        Sint64T T64_mostRecentCalculatedOffset;
                        char sign[5] = " ";

                        ZL303XX_CONVERT_TO_64_SIGNED(T64_mostRecentCalculatedOffset, mostRecentCalculatedOffset);

                        if( T64_mostRecentCalculatedOffset < 0 )
                        {
                            T64_mostRecentCalculatedOffset = -T64_mostRecentCalculatedOffset;
                            sign[0] = '-';
                        }
                        ZL303XX_CONVERT_FROM_64(mostRecentCalculatedOffset, T64_mostRecentCalculatedOffset);

                        printf("\tThe current phase alignment accuracy is %s0x%X%08X ns\n", sign,
                                mostRecentCalculatedOffset.hi, mostRecentCalculatedOffset.lo);
                        printf("\tThe current phase alignment network quality is %d\n", mostRecentEstimatedNetworkQuality);
                        printf("\tThe current phase alignment assigned %d\n", mostRecentAssigned);
                    }
                    else
                    {
                        printf(" Call to zl303xx_AprGetDevice1HzMostRecentAlignmentDifference() failed=%d\n", status);
                        return status;
                    }

                    {   /* Metrics */
                        Sint32T mean;
                        Uint32T stdDev;

                        status = zl303xx_AprGet1HzMetric(hwParams, serverId, &mean, &stdDev);
                        if (status == ZL303XX_OK)
            {
                             printf("\t1Hz Metrics (after Phase_Lock): Mean= %d, Standard Deviation= %d\n", mean, stdDev);
            }
        else
        {
                            printf(" Call to zl303xx_AprGet1HzMetric() failed=%d\n", status);
        }
                    }

                    { /* Metrics data */
            Sint32T startElement = -1;
            zl303xx_BooleanE lastElement = ZL303XX_FALSE;
            Uint64S adjustment;
            zl303xx_BooleanE adjustmentUsable;
            zl303xx_BooleanE adjustmentApplied;
            Uint32T adjustmentNetworkQuality;
            Sint64T T64_mostRecentCalculatedOffset;
            char sign = ' ';

                        printf("\t1Hz Metrics Data:\n");

            do
            {
                status = zl303xx_AprGetNext1HzMetricElement(hwParams, serverId,
                            &startElement,
                            &lastElement,
                            &adjustment,
                            &adjustmentUsable,
                            &adjustmentApplied,
                            &adjustmentNetworkQuality);

                ZL303XX_CONVERT_TO_64_SIGNED(T64_mostRecentCalculatedOffset, adjustment);

                if( T64_mostRecentCalculatedOffset < 0 )
                {
                    T64_mostRecentCalculatedOffset = -T64_mostRecentCalculatedOffset;
                    sign = '-';
                }
                else
                {
                    sign = ' ';
                }
                ZL303XX_CONVERT_FROM_64(mostRecentCalculatedOffset, T64_mostRecentCalculatedOffset);

                printf("\t\t[%d] adjustment %c0x%X%08X ns usable %d applied %d quality %d\n",
                        startElement,
                        sign, mostRecentCalculatedOffset.hi, mostRecentCalculatedOffset.lo,
                        adjustmentUsable,
                        adjustmentApplied,
                        adjustmentNetworkQuality);

            } while (status == ZL303XX_OK && lastElement == ZL303XX_FALSE);
        }
                }
            }
            else
            {
                printf(" Call to zl303xx_AprGetCurrent1HzConfigData() failed=%d\n", status);
                return status;
            }

        }
    }

skip1Hz:

    if (status == ZL303XX_OK)
    {
        if ((status = zl303xx_DebugCGULockState(hwParams)) != ZL303XX_OK )
        {
            printf("zl303xx_DebugHybridState: No reference currently selected.\n");
        }
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_AprGetServerFreqOffset(hwParams, &freqOffset);
        if (status == ZL303XX_OK)
        {
            printf("     The frequency offset is %d ppt\n", freqOffset);
        }
        else
        {
            printf("zl303xx_DebugHybridState: Failed to get freq. status=%d\n", status);
            return status;
        }
    }

    if (status == ZL303XX_OK)
    {
        Sint32T holdoverValue = 0;
        zl303xx_BooleanE holdoverV;
        if ((status = zl303xx_AprGetCurrentHoldoverValue(hwParams, &holdoverValue, &holdoverV)) == ZL303XX_OK)
        {
            printf("\nThe current holdover value is %d ppt\n\n", holdoverValue);
        }
        else
        {
            if (status == ZL303XX_TABLE_EMPTY)
            {
                printf("Call to zl303xx_AprGetCurrentHoldoverValue() does not have valid holdover yet (%d) \n", status);
                status = ZL303XX_OK;
            }
            else
                printf("Call to zl303xx_AprGetCurrentHoldoverValue() failed=%d \n", status);
        }
    }

    if (status == ZL303XX_OK)
    {
        zl303xx_AprElecStatusFlagsS statusFlags;

        status = zl303xx_AprGetDeviceRefMode (hwParams,  &devRefMode);

        if (status == ZL303XX_OK)
        {
           /* The state machine only runs when the device is in Electrical/Hybrid mode */
           if ((devRefMode == ZL303XX_ELECTRIC_MODE) || (devRefMode == ZL303XX_HYBRID_MODE))
           {
              status = zl303xx_AprGetElecRefStatusFlags(hwParams, &statusFlags);
              if (status == ZL303XX_OK)
              {
                  printf("Electrical Reference Status Flags:\n");
                  printf(" RF = %s\n", (statusFlags.RF == ZL303XX_TRUE) ? "TRUE" : "FALSE");
                  printf(" L  = %s\n", (statusFlags.L == ZL303XX_TRUE) ? "TRUE" : "FALSE");
                  printf(" H  = %s\n", (statusFlags.H == ZL303XX_TRUE) ? "TRUE" : "FALSE");
                  printf(" SE = %s\n\n", (statusFlags.SE == ZL303XX_TRUE) ? "TRUE" : "FALSE");
              }
              else
                  printf("zl303xx_GetAprDeviceStatus: Call to zl303xx_AprGetElecRefStatusFlags(hwParams) failed=%d\n", status);
           }
        }
    }

    return status;
}

/**

  Function Name:
   zl303xx_GetAprServerConfigInfo

  Details:
    Example code to show how to get any desired configuration data for the specified
    packet/hybrid server by calling APIs provided by APR

  Parameters:
   [in]   hwParams   Pointer to the device instance
   [in]   serverId   Identifier of the server

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE zl303xx_GetAprServerConfigInfo(void *hwParams, Uint16T serverId)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_BooleanE hybrid, singlePath;
    zl303xx_AprTsFormatE tsFormat;
    zl303xx_AprPullInRangeE pullInRange;
    zl303xx_AprAlgTypeModeE algTypeMode;
    zl303xx_AprOscillatorFilterTypesE osciFilterType;
    zl303xx_AprFilterTypesE filterType;
    zl303xx_AprPktRateE fwdPktRate, revPktRate;

    if (hwParams != NULL)
    {
        printf("Device %p, Server clock %u configuration data:\n", hwParams, serverId);
    }
    else
    {
        return ZL303XX_INVALID_POINTER;
    }

    status = zl303xx_AprGetServerTimestampFormat(hwParams, serverId, &tsFormat);
    if (status == ZL303XX_OK)
    {
        printf("     Timestamp is in format of %d\n", tsFormat);
    }
    else
    {
        return status;
    }


    status = zl303xx_AprIsServerInHybridMode(hwParams, serverId, &hybrid);
    if (status == ZL303XX_OK)
    {
        printf("     The server clock is in mode of %d (1-hybrid, 0-packet)\n", hybrid);
    }
    else
    {
        return status;
    }

    status = zl303xx_AprIsServerUseSinglePath(hwParams, serverId, &singlePath);
    if (status == ZL303XX_OK)
    {
        if (singlePath == ZL303XX_TRUE)
        {
            printf("     Using SINGLE path\n");
        }
        else
        {
            printf("     Using BOTH (FORWARD & REVERSE) paths\n");
        }
    }
    else
    {
        return status;
    }

    status = zl303xx_AprGetServerPullInRange(hwParams, serverId, &pullInRange);
    if (status == ZL303XX_OK)
    {
        printf("     The pull-in range is +/- %d.%dppm\n",
               pullInRange/1000, pullInRange % 1000);
    }
    else
    {
        return status;
    }

    status = zl303xx_AprGetServerAlgTypeMode(hwParams, serverId, &algTypeMode);
    if (status == ZL303XX_OK)
    {
        printf("     The algorithm type mode is %d\n",
               algTypeMode);
    }
    else
    {
        return status;
    }

    status = zl303xx_AprGetServerOscillatorFilterType(hwParams, serverId, &osciFilterType);
    if (status == ZL303XX_OK)
    {
        printf("     The oscillator filter type is %d\n",
               osciFilterType);
    }
    else
    {
        return status;
    }

    status = zl303xx_AprGetServerFilterType(hwParams, serverId, &filterType);
    if (status == ZL303XX_OK)
    {
        printf("     The filter type is %d\n",
               filterType);
    }
    else
    {
        return status;
    }

    status = zl303xx_AprGetServerPktRate(hwParams, serverId, &fwdPktRate, &revPktRate);
    if (status == ZL303XX_OK)
    {
        printf("     The packet rate (fwd/rev): %d/%d\n",
               fwdPktRate, revPktRate);
    }
    else
    {
        return status;
    }

    return status;
}


/**

  Function Name:
   zl303xx_GetAprServerStatus

  Details:
    Example code to retrieve the status information for the specified packet/hybrid server

  Parameters:
   [in]   hwParams   Pointer to the device instance
   [in]   serverId   Identifier of the server

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE zl303xx_GetAprServerStatus(void *hwParams, Uint16T serverId)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_BooleanE tsValid, uPeriod;
    Uint32T clkstabilityIndicator;

    if (hwParams != NULL)
    {
        printf("Device %p, Server clock %u status:\n", hwParams, serverId);
    }
    else
    {
        return ZL303XX_INVALID_POINTER;
    }

    if (serverId == 0xFFFF)
    {
        status = ZL303XX_UNSUPPORTED_OPERATION;
    }
    else
    {
        status = zl303xx_AprIsServerClkStreamValid(hwParams, serverId, &tsValid);
    }

    if (status == ZL303XX_OK)
    {
        if (tsValid == ZL303XX_TRUE)
        {
            printf("     Timestamps are VALID\n");
        }
        else
        {
            printf("     Timestamps are INVALID\n");
        }
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_AprIsServerClkInUMode(hwParams, serverId, &uPeriod);
    }
    if (status == ZL303XX_OK)
    {
        if (uPeriod == ZL303XX_TRUE)
        {
            printf("     Clock is in 'U' period\n");
        }
        else
        {
            printf("     Clock is NOT in 'U' period\n");
        }
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_AprGetServerClkStabilityIndicator(hwParams, serverId, &clkstabilityIndicator);
    }
    if (status == ZL303XX_OK)
    {
        printf("     The server clock stability factor is %u ppt\n", clkstabilityIndicator);
    }

    if (status == ZL303XX_OK || status == ZL303XX_UNSUPPORTED_OPERATION)
    {
        zl303xx_AprServerStatusFlagsS serverStatusFlags;

        if ((status = zl303xx_AprGetServerStatusFlags (hwParams, serverId, &serverStatusFlags)) == ZL303XX_OK)
        {
            printf("\nServerStatusFlags (details):\n");
            printf("Device %p: server:%d\n\tL1 = %d, L2 = %d, L3 = %d, L4 = %d, L = %d, gstL = %d\n",
                   hwParams, serverId,
                   serverStatusFlags.L1, serverStatusFlags.L2, serverStatusFlags.L3, serverStatusFlags.L4,
                   serverStatusFlags.L, serverStatusFlags.gstL);
            printf("\tV = %d, gstV = %d, S = %d, U = %d, U1 = %d\n",
                   serverStatusFlags.V, serverStatusFlags.gstV, serverStatusFlags.S,
                   serverStatusFlags.U, serverStatusFlags.U1);
            printf("\tPE =%d, PA = %d, gstPA = %d, H = %d, gstH = %d\n",
                   serverStatusFlags.PE, serverStatusFlags.PA, serverStatusFlags.gstPA,
                   serverStatusFlags.H, serverStatusFlags.gstH);
            printf("\tOOR =%d, TTE = %d, OUT = %d, OL = %d, FWD_RR = %d, REV_RR = %d\n",
                   serverStatusFlags.OOR, serverStatusFlags.ttErrDetected, serverStatusFlags.outageDetected,
                   serverStatusFlags.outlierDetected, serverStatusFlags.frrDetected, serverStatusFlags.rrrDetected);
            printf("\tSTEP =%d, PLF = %d, PLR = %d, DIR = %d\n",
                   serverStatusFlags.stepDetected, serverStatusFlags.pktLossDetectedFwd,
                   serverStatusFlags.pktLossDetectedRev, serverStatusFlags.algPathFlag);
            printf("\tAF = S%d, AR = S%d, STATE= %d\n\n",
                   serverStatusFlags.algFwdState, serverStatusFlags.algRevState, serverStatusFlags.state);
        }

        else
            printf("zl303xx_GetAprServerStatus: Call to zl303xx_AprGetServerStatusFlags() failed=%d\n", status);
    }

    return status;
}


/* zl303xx_DebugAprBuildInfo */
/**
   Displays information about the APR release and build date/times.

*******************************************************************************/
void zl303xx_DebugAprBuildInfo(void)
{

   printf("APR Build Information\n" \
          "=====================\n" \
          "Release Version: %s\n" \
          "Release Date:    %s\n" \
          "Release Time:    %s\n" \
          "Release SW ID:   %s\n" \
          "Build Date:      %s\n" \
          "Build Time:      %s\n",
          zl303xx_AprReleaseVersion, zl303xx_AprReleaseDate, zl303xx_AprReleaseTime,
          zl303xx_AprReleaseSwId, zl303xx_AprBuildDate, zl303xx_AprBuildTime);
}

/*******************************************************************************

  Function Name:
   zl303xx_DebugPrintAprByStreamNum

  Details:
   Debug command to print APR and 1 Hz parameters

  Parameters:
   [in]   hwParams       Pointer to the device instance parameter structure
   [in]   streamNumber   Stream number
   [in]   algorithmNumber Algorithm number

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_DebugPrintAprByStreamNum(void *hwParams,
        Uint32T streamNumber, Uint32T algorithmNumber)
{
    Sint32T v[ZL303XX_ALGO_MAX_PARAMETERS] = {0};
    zlStatusE status = ZL303XX_OK;

    if (hwParams == NULL)
    {
        printf("NULL pointer passed in for hwParams.\n");
        status = ZL303XX_PARAMETER_INVALID;
    }

    (void) streamNumber; /* warning removal */
    (void) algorithmNumber;
    (void) v;
    printf("zl303xx_DebugPrintAprByStreamNum not available...\n");

    return(status);
}


/*******************************************************************************

  Function Name:
   zl303xx_DebugPrintAprByReferenceId

  Details:
   Debug command to print APR and 1 Hz parameters

  Parameters:
   [in]   hwParams       Pointer to the device instance parameter structure
   [in]   referenceID    Master reference ID

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_DebugPrintAprByReferenceId(void *hwParams,
        Uint32T referenceID)
{
    zlStatusE status = ZL303XX_OK;

    if (hwParams == NULL)
    {
        printf("NULL pointer passed in for hwParams.\n");
        status = ZL303XX_PARAMETER_INVALID;
    }

    zl303xx_AprPrintParameters(hwParams, referenceID);

    return(status);
}

/**

  Function Name:
    zl303xx_DebugGetPathStatistics

  Details:
*  Record and print path delay and offset from maste information

  Parameters:
   [in]
   hwParams

    [out]
   none

  Return Value:
   zlStatusE

  Notes:
   This function should only be used on client

*******************************************************************************/
zlStatusE zl303xx_DebugGetAprPathStatistics(void * hwParams)
{
    zlStatusE status = ZL303XX_OK;
    Uint8T algNum = 0;               /* Stats for the algorithm 0 */
    zl303xx_AprGetStatisticsS stats;
    zl303xx_AprStatisticsS aprStats;
    Uint16T serverId;
    zl303xx_AprConfigure1HzS fwd1HzPar;
    zl303xx_AprConfigure1HzS rev1HzPar;
    /*Sint32T ofm; */

    printf ("\n\n------------------ Get OFM and path delay information for device %p ------------------ \n", hwParams);

    /* Another way of computing offset from master */
    /*{
        zl303xx_AprPerfStatisticsS perfStats;

        if (status == ZL303XX_OK)
        {
            status = zl303xx_AprGetPerfStatistics(hwParams, &perfStats);
        }

        if (status == ZL303XX_OK)
        {
            ofm = (perfStats.dfPhiMaxNs - perfStats.dfPhiMinNs)/2;
            printf("\n Offset From Master = %d ns\n", ofm);
        }
        else
        {
            printf("\n ----- Failed to computed OFM for device %d\n ", hwParams);
        }

    }*/

    {
        status = ZL303XX_OK;

        if (status == ZL303XX_OK)
        {
            status = zl303xx_AprGetCurrent1HzConfigData(hwParams, &serverId, &fwd1HzPar, &rev1HzPar);
        }

        if (status == ZL303XX_OK)
        {
            if(( fwd1HzPar.disabled == ZL303XX_TRUE ) || ( rev1HzPar.disabled == ZL303XX_TRUE ))
            {
                status = ZL303XX_NOT_RUNNING;
                printf("\nNote: 1Hz not enabled on forward or reverse path.\n" );
                printf(  "      Only 'percentage selected packets' value is valid.\n" );
            }
        }

        if (status == ZL303XX_OK || status == ZL303XX_NOT_RUNNING)
        {
            status = zl303xx_AprStatisticsStartStructInit(hwParams, &aprStats);
            if( status != ZL303XX_OK )
            {
                printf( "Error initialising aprStats structure\n" );
            }
        }

        if (status == ZL303XX_OK)
        {
            aprStats.globalEnabled = ZL303XX_TRUE;
            aprStats.enabled = ZL303XX_TRUE;
            aprStats.intervalActive = ZL303XX_TRUE;
            /* Interval value specifies how long the Apr statistics should be captured */
            aprStats.interval = 10;
            aprStats.PDVPercentile = 90*100;
        }

        if (status == ZL303XX_OK)
        {
            /*clear old stats*/
            status = zl303xx_AprClearStatistics(hwParams, serverId);
            if( status != ZL303XX_OK )
            {
                printf( "Error clearing statistics\n" );
            }
        }

        if (status == ZL303XX_OK)
        {
            /* this will throw an exception if serverId is invalid */
            status = zl303xx_AprStatisticsStart(hwParams, serverId, &aprStats);
            if( status != ZL303XX_OK )
            {
                printf( "Error starting statistics\n" );
            }
        }

        if (status == ZL303XX_OK)
        {
            printf ("\n Collecting path statistics for server %u ...... \n", serverId);

            /* *
             * Delay the task to give Apr time to collect statistics
             * This delay time should be greater than the interval set in the zl303xx_AprGetStatisticsS
             * */
            OS_TASK_DELAY(aprStats.interval * 1000 + 500);    /* In ms */

            status = zl303xx_AprGetStatistics(hwParams, serverId, &stats);

            if (status == ZL303XX_OK)
            {
                /* this will throw exception if clock stream handle is invalid */
                zl303xx_AprStatisticsStop(hwParams, serverId);
            }

            if (status == ZL303XX_OK )
            {
                zl303xx_AprGetAlgStatsS *forwardAlgStats = stats.fwdAlgStats;
                zl303xx_AprGetAlgStatsS *revAlgStats = stats.revAlgStats;

                if (status == ZL303XX_OK)
                {
                    Sint64T T64_minTransitTime;
                    Sint64T T64_maxTransitTime;
                    Sint64T T64_transitTimeAtPercentile;
                    Sint64T TTMean_for;
                    Sint64T TTMean_rev = 0;
                    Sint64T MPD;
                    Sint64T OFM;

                    ZL303XX_CONVERT_TO_64_SIGNED(T64_transitTimeAtPercentile,
                                               forwardAlgStats[algNum].data.PDV.transitTimeAtPercentile);
                    printf ("  Forward PDV transit time at percentile %u%%: %u ns\n",
                            aprStats.PDVPercentile/100,
                            (Sint32T)T64_transitTimeAtPercentile);
                   /* printf ("  Clock measured accuracy in relation to the local oscillator: %u ppb\n",
                            forwardAlgStats[algNum].data.clockQuality.clkAccuracyPpb);*/
                    ZL303XX_CONVERT_TO_64_SIGNED(T64_minTransitTime, forwardAlgStats[algNum].data.raw.minTransitTime);
                    ZL303XX_CONVERT_TO_64_SIGNED(T64_maxTransitTime, forwardAlgStats[algNum].data.raw.maxTransitTime);

                    TTMean_for = T64_minTransitTime  + (T64_maxTransitTime - T64_minTransitTime)/2;
                    printf ("  Forward Path Delay: %d ns\n",
                            (Sint32T)TTMean_for);
                    printf( "  Percent of selected forward packets: %u\n", forwardAlgStats[algNum].data.percentSelectedPackets );
                    if(( forwardAlgStats[algNum].data.bPeerDelay == ZL303XX_TRUE ) ||
                       ( revAlgStats[algNum].data.bPeerDelay == ZL303XX_TRUE ))
                    {
                        /* Peer delay mecahnism is being used - there is no reverse path. */
                    }
                    else
                    {
                        ZL303XX_CONVERT_TO_64_SIGNED(T64_transitTimeAtPercentile,
                                               revAlgStats[algNum].data.PDV.transitTimeAtPercentile);
                        printf ("  Reverse PDV transit time at percentile %u%%: %u ns\n",
                               aprStats.PDVPercentile/100,
                               (Sint32T)T64_transitTimeAtPercentile);
                       /* printf ("  Clock measured accuracy in relation to the local oscillator: %u ppb\n",
                               revAlgStats[algNum].data.clockQuality.clkAccuracyPpb);*/
                        ZL303XX_CONVERT_TO_64_SIGNED(T64_minTransitTime, revAlgStats[algNum].data.raw.minTransitTime);
                        ZL303XX_CONVERT_TO_64_SIGNED(T64_maxTransitTime, revAlgStats[algNum].data.raw.maxTransitTime);
                        TTMean_rev = T64_minTransitTime  + (T64_maxTransitTime - T64_minTransitTime)/2;
                        printf ("  Reverse Path Delay: %d ns\n",
                            (Sint32T)TTMean_rev);
                        printf( "  Percent of selected reverse packets: %u\n", revAlgStats[algNum].data.percentSelectedPackets );
                    }

                    MPD = (TTMean_for + TTMean_rev)/2;
                    OFM = (TTMean_for - MPD);

                    printf("\n  Mean Path delay    = %d ns\n",(Sint32T) MPD);
                    printf("\n  Offset From Master = %d ns\n",(Sint32T) OFM);

                   /* if (( fwd1HzPar.disabled == ZL303XX_FALSE ) || ( rev1HzPar.disabled == ZL303XX_FALSE ))
                    {
                        if(( forwardAlgStats[algNum].data.bPeerDelay == ZL303XX_TRUE ) ||
                           ( revAlgStats[algNum].data.bPeerDelay == ZL303XX_TRUE ))
                        {
                            printf ("\nPeer-delay mean path delay delay: 0x%08X%08X ns\n",
                                        forwardAlgStats[algNum].data.peerMeanDelay.hi,
                                        forwardAlgStats[algNum].data.peerMeanDelay.lo >> 16);
                        }
                        else
                        {
                            printf ("\nOne way network delay: 0x%08X%08X ns\n",
                                        forwardAlgStats[algNum].data.STPNetDly.hi,
                                        forwardAlgStats[algNum].data.STPNetDly.lo);
                        }
                    }*/
                }
            }
            else
            {
                printf ("Error in AprGetStatistics=%d\n", status);
            }
        }
        else
        {
            printf ("\nError in getting APR data; status=%d", status);
        }
    }

    printf ("\n\n ------------- End of OFM/Path information query for server %u ----------- \n\n", serverId);

    return status;
}

/**

  Function Name:
*   zl303xx_AprGetDeviceCurrentPathDelays

  Details:
   Record and print all APR statistics

  Parameters:
   [in]
   hwParams

    [out]
   none

  Return Value:
   zlStatusE

  Notes:
   This function should only be used on client

*******************************************************************************/
zlStatusE zl303xx_AprGetDeviceCurrentPathDelays(void * hwParams)
{

      zlStatusE status = ZL303XX_OK;
      Uint64S mostRecentCalculatedOffset;
      Uint64S realTTFor;
      zl303xx_BooleanE mostRecentAssigned;

      Sint64T OFM;
      Sint64T MPD;

      status = zl303xx_AprGetDevice1HzMostRecentPathsData(
            hwParams,
            &mostRecentCalculatedOffset,
            &realTTFor,
            &mostRecentAssigned
            );
      if( status == ZL303XX_OK )
      {

         printf( "\n\n---------------------------------------------------------\n" );
         printf( "--- Most recent Path information for device %p:\n", hwParams);

        ZL303XX_CONVERT_TO_64_SIGNED(OFM, mostRecentCalculatedOffset);
        OFM = -OFM;
        printf( "          Offset From Master        : %d ns\n", (Sint32T)OFM);
        ZL303XX_CONVERT_TO_64_SIGNED(MPD, realTTFor);
        MPD = -MPD;
        printf( "          Mean Path Delay           : %d ns\n", (Sint32T)MPD);
        printf( "          Forward Path Delay        : %d ns\n", (Sint32T)(OFM+MPD));
        printf( "          Reverse Path Delay        : %d ns\n", (Sint32T)(MPD - OFM));
         if (mostRecentAssigned == ZL303XX_TRUE)
         {
             printf( "\n\t Current data was used in phase calculations. \n");
         }
         else
         {
             printf( "\n\t Current data was not used for phase calculations. \n");
         }
      }
      else
      {
         printf(" Path delay is calculated by APR-Phase module, which is not enabled on device %p \n", hwParams);
      }

      printf( "\n------------------------------------------------------------\n" );
      return status;
}

/**

  Function Name:
*   zl303xx_AprGetDeviceCurrentPathDelays

  Details:
   Record and print all APR statistics

  Parameters:
   [in]
   hwParams

    [out]
   none

  Return Value:
   zlStatusE

  Notes:
   This function should only be used on client

*******************************************************************************/
zlStatusE zl303xx_AprGetServerCurrentPathDelays(void * hwParams, Uint16T serverId)
{

      zlStatusE status = ZL303XX_OK;
      Uint64S mostRecentCalculatedOffset;
      Uint64S realTTFor;
      zl303xx_BooleanE mostRecentAssigned;

      Sint64T OFM;
      Sint64T MPD;

      status = zl303xx_AprGetServer1HzMostRecentPathsData(
            hwParams,
            serverId,
            &mostRecentCalculatedOffset,
            &realTTFor,
            &mostRecentAssigned
            );
      if( status == ZL303XX_OK )
      {

         printf( "\n\n--------------------------------------------------------\n" );
         printf( "--- Most recent Path information for server %u:\n", serverId);

        ZL303XX_CONVERT_TO_64_SIGNED(OFM, mostRecentCalculatedOffset);
        OFM = -OFM;
        printf( "          Offset From Master        : %d ns\n", (Sint32T)OFM);
        ZL303XX_CONVERT_TO_64_SIGNED(MPD, realTTFor);
        MPD = -MPD;
        printf( "          Mean Path Delay           : %d ns\n", (Sint32T)MPD);
        printf( "          Forward Path Delay        : %d ns\n", (Sint32T)(OFM+MPD));
        printf( "          Reverse Path Delay        : %d ns\n", (Sint32T)(MPD - OFM));
         if (mostRecentAssigned == ZL303XX_TRUE)
         {
             printf( "\n\t Current data was used in phase calculations. \n");
         }
         else
         {
             printf( "\n\t Current data was not used for phase calculations. \n");
         }
      }
      else
      {
         printf(" Path delay is calculated by the APR-Phase module, which is not enabled on server %d \n", serverId);
      }

      printf( "\n---------------------------------------------------------\n" );
      return status;
}


/**

  Function Name:
    zl303xx_DebugGetAllAprStatistics

  Details:
   Record and print all APR statistics

  Parameters:
   [in]
   hwParams

    [out]
   none

  Return Value:
   zlStatusE

  Notes:
   This function should only be used on client

*******************************************************************************/
zlStatusE zl303xx_DebugGetAllAprStatistics(void * hwParams)
{
    zlStatusE status = ZL303XX_OK;

    printf ("\n\n------------------ APR Get Statistics Starts for device %p ------------------ \n", hwParams);

    {
        zl303xx_AprPerfStatisticsS perfStats;
        zl303xx_AprPerfStatisticsS perfStatsAnyState;
        zl303xx_AprConfigure1HzS fwd1HzPar;
        zl303xx_AprConfigure1HzS rev1HzPar;
        Uint16T serverId;
        if (status == ZL303XX_OK)
        {
            status = zl303xx_AprGetCurrent1HzConfigData(hwParams, &serverId, &fwd1HzPar, &rev1HzPar);
        }


        if (status == ZL303XX_OK)
        {
            status = zl303xx_AprGetPerfStatistics(hwParams, &perfStats, &perfStatsAnyState);
        }


        if (status == ZL303XX_OK)
        {
            if(( fwd1HzPar.disabled == ZL303XX_TRUE ) || ( rev1HzPar.disabled == ZL303XX_TRUE ))
            {
                printf("\n" \
                               "\n<Phase Locking is disabled. Showing only Frequency Stats.>\n");

                if ( perfStats.FLocked == ZL303XX_FALSE )
                    {
                        printf("\n" \
                               "\n<Performance Statistics After Frequency Lock>\n" \
                               "--- Device is not frequency locked yet. \n");
                    }
                else
                    {
                        printf("\n" \
                               "\n<Performance Statistics After Frequency Lock>\n" \
                               "--- Frequency Offset\n" \
                               "             Average:         %d ppt\n" \
                               "             Current DF:      %d ppt\n" \
                               "             DF Min:          %d ppt\n" \
                               "             DF Max:          %d ppt\n" \

                               "--- Freq Stability\n" \
                               "             Average:         %d ppt\n" \
                               "             Current DFDelta: %d ppt\n" \
                               "             DFDelta Min:     %d ppt\n" \
                               "             DFDelta Max:     %d ppt\n"\

                               "--- L1 and L4 Active State Targets\n" \
                               "             L4:         %d ppt\n" \
                               "             L1:         %d ppt\n",

                               perfStats.dfAveragePpt, perfStats.dfCurrentPpt, perfStats.dfCurrentMinPpt,
                               perfStats.dfCurrentMaxPpt,

                               perfStats.FreqStabilityPpt, perfStats.dfDeltaPpt, perfStats.dfDeltaMinPpt,
                               perfStats.dfDeltaMaxPpt,

                               perfStats.L4Target, perfStats.L1Target);
                    }

                printf("\n" \
                           "\n<Any State Performance Statistics>\n" \
                           "--- Frequency Offset\n" \
                           "             Average:         %d ppt\n" \
                           "             Current DF:      %d ppt\n" \
                           "             DF Min:          %d ppt\n" \
                           "             DF Max:          %d ppt\n" \

                           "--- Freq Stability\n" \
                           "             Average:         %d ppt\n" \
                           "             Current DFDelta: %d ppt\n" \
                           "             DFDelta Min:     %d ppt\n" \
                           "             DFDelta Max:     %d ppt\n" \

                           "--- L1 and L4 Active State Targets\n" \
                           "             L4:         %d ppt\n" \
                           "             L1:         %d ppt\n",

                           perfStatsAnyState.dfAveragePpt, perfStatsAnyState.dfCurrentPpt, perfStatsAnyState.dfCurrentMinPpt,
                           perfStatsAnyState.dfCurrentMaxPpt,

                           perfStatsAnyState.FreqStabilityPpt, perfStatsAnyState.dfDeltaPpt, perfStatsAnyState.dfDeltaMinPpt,
                           perfStatsAnyState.dfDeltaMaxPpt,

                           perfStatsAnyState.L4Target, perfStatsAnyState.L1Target);


            }
            else /* 1 Hz is enabled */
            {
                if ( perfStats.PhaseOffsetNs ==  (Sint32T)817405952 )
                {
                    printf("\n" \
                           "\n<Performance Statistics are not available yet.>\n\n");
                }
                else
                {
                    if ( perfStats.FLocked == ZL303XX_FALSE )
                    {
                        printf("\n" \
                               "\n<Performance Statistics After Frequency Lock>\n" \
                               "--- Device is not frequency locked yet. \n");
                    }
                    else
                    {
                        printf("\n" \
                               "\n<Performance Statistics After Frequency Lock>\n" \
                               "--- Frequency Offset\n" \
                               "             Average:         %d ppt\n" \
                               "             Current DF:      %d ppt\n" \
                               "             DF Min:          %d ppt\n" \
                               "             DF Max:          %d ppt\n" \

                               "--- Phase Offset\n" \
                               "             Average:         %d ns\n" \
                               "             Current Offset:  %d ns\n" \
                               "             Phase Min:       %d ns\n" \
                               "             Phase Max:       %d ns\n"\

                               "--- Freq Stability\n" \
                               "             Average:         %d ppt\n" \
                               "             Current DFDelta: %d ppt\n" \
                               "             DFDelta Min:     %d ppt\n" \
                               "             DFDelta Max:     %d ppt\n"\

                               "--- Phase Stability\n" \
                               "             Average:         %d ns\n" \
                               "             Current DeltaPi: %d ns\n" \
                               "             DeltaPi Min:     %d ns\n" \
                               "             DeltaPi Max:     %d ns\n" \

                               "--- L1 and L4 Active State Targets\n" \
                               "             L4:         %d ppt\n" \
                               "             L1:         %d ppt\n",

                               perfStats.dfAveragePpt, perfStats.dfCurrentPpt, perfStats.dfCurrentMinPpt,
                               perfStats.dfCurrentMaxPpt,

                               perfStats.PhaseOffsetAvNs, perfStats.PhaseOffsetNs, perfStats.PhaseOffsetMinNs,
                               perfStats.PhaseOffsetMaxNs,

                               perfStats.FreqStabilityPpt, perfStats.dfDeltaPpt, perfStats.dfDeltaMinPpt,
                               perfStats.dfDeltaMaxPpt,

                               perfStats.dfAveragePhiNs, perfStats.dfPhiNs, perfStats.dfPhiMinNs, perfStats.dfPhiMaxNs,

                               perfStats.L4Target, perfStats.L1Target);
                    }


                    printf("\n" \
                           "\n<Any State Performance Statistics>\n" \
                           "--- Frequency Offset\n" \
                           "             Average:         %d ppt\n" \
                           "             Current DF:      %d ppt\n" \
                           "             DF Min:          %d ppt\n" \
                           "             DF Max:          %d ppt\n" \

                           "--- Phase Offset\n" \
                           "             Average:         %d ns\n" \
                           "             Current Offset:  %d ns\n" \
                           "             Phase Min:       %d ns\n" \
                           "             Phase Max:       %d ns\n" \

                           "--- Freq Stability\n" \
                           "             Average:         %d ppt\n" \
                           "             Current DFDelta: %d ppt\n" \
                           "             DFDelta Min:     %d ppt\n" \
                           "             DFDelta Max:     %d ppt\n" \

                           "--- Phase Stability\n" \
                           "             Average:         %d ns\n" \
                           "             Current DeltaPi: %d ns\n" \
                           "             DeltaPi Min:     %d ns\n" \
                           "             DeltaPi Max:     %d ns\n"\

                           "--- L1 and L4 Active State Targets\n" \
                           "             L4:         %d ppt\n" \
                           "             L1:         %d ppt\n",

                           perfStatsAnyState.dfAveragePpt, perfStatsAnyState.dfCurrentPpt, perfStatsAnyState.dfCurrentMinPpt,
                           perfStatsAnyState.dfCurrentMaxPpt,

                           perfStatsAnyState.PhaseOffsetAvNs, perfStatsAnyState.PhaseOffsetNs, perfStatsAnyState.PhaseOffsetMinNs,
                           perfStatsAnyState.PhaseOffsetMaxNs,

                           perfStatsAnyState.FreqStabilityPpt, perfStatsAnyState.dfDeltaPpt, perfStatsAnyState.dfDeltaMinPpt,
                           perfStatsAnyState.dfDeltaMaxPpt,

                           perfStatsAnyState.dfAveragePhiNs, perfStatsAnyState.dfPhiNs, perfStatsAnyState.dfPhiMinNs, perfStatsAnyState.dfPhiMaxNs,

                           perfStatsAnyState.L4Target, perfStatsAnyState.L1Target);
                }
            }
        }
    }

    printf ("\n\n ------------------ APR Statistics Ends ------------------ \n");

    return status;
}


/*******************************************************************************

  Function Name:
   zl303xx_DebugAprPrint1HzData

  Details:
   Debug command to print 1 Hz parameters

  Parameters:
   [in]   zl303xx_Params   Pointer to the device instance parameter structure
   [in]   streamNumber   Stream number
   [in]   algorithmNumber Algorithm number

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_DebugAprPrint1HzData
            (
            void *hwParams,
            Uint32T streamNumber,
            Uint32T algorithmNumber
            )
{
   zlStatusE status = ZL303XX_OK;

   if (hwParams == NULL)
   {
       printf("NULL pointer passed in for hwParams.\n");
       status = ZL303XX_PARAMETER_INVALID;
   }

   if (status == ZL303XX_OK)
   {
       status = zl303xx_AprPrint1HzData ( hwParams, streamNumber, algorithmNumber  );
   }

   return(status);
}



/*******************************************************************************

  Function Name:
   zl303xx_DebugAprGet1HzData

  Details:
   Debug command to collect 1 Hz parameters

  Parameters:
   [in]   hwParams       Pointer to the device instance parameter structure
   [in]   streamNumber   Stream number
   [in]   algorithmNumber Algorithm number

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_DebugAprGet1HzData
            (
            void *hwParams,
            Uint32T streamNumber,
            Uint32T algorithmNumber
            )
{
   zl303xx_Apr1HzDebugS Apr1HzDebug;

   zlStatusE status = ZL303XX_OK;
   Uint32T serverId = streamNumber / 2;

   if (hwParams == NULL)
   {
       printf("NULL pointer passed in for hwParams.\n");
       status = ZL303XX_PARAMETER_INVALID;
   }

   if (status == ZL303XX_OK)
   {
      status = zl303xx_AprGet1HzData ( hwParams, streamNumber, algorithmNumber, &Apr1HzDebug);
   }

   if (status == ZL303XX_OK)
   {
       if( Apr1HzDebug.isDisabled == ZL303XX_TRUE )         printf( "disabled                      : TRUE\n" );
       else                                               printf( "disabled                      : FALSE\n" );

       switch( Apr1HzDebug.realignmentType )
       {
          case ZL303XX_1HZ_REALIGNMENT_ON_GOOD_LOCK:  printf( "realignmentType               : REALIGNMENT_ON_GOOD_LOCK\n" ); break;
          case ZL303XX_1HZ_REALIGNMENT_PERIODIC:      printf( "realignmentType               : REALIGNMENT_PERIODIC\n" ); break;
          case ZL303XX_1HZ_REALIGNMENT_MANUAL:        printf( "realignmentType               : REALIGNMENT_MANUAL\n" ); break;
          case ZL303XX_1HZ_REALIGNMENT_LAST:          printf( "realignmentType               : REALIGNMENT_LAST\n" ); break;
          case ZL303XX_1HZ_REALIGNMENT_PERIODIC_U :   printf( "realignmentType               : REALIGNMENT_PERIODIC_U\n" ); break;
          case ZL303XX_1HZ_REALIGNMENT_PER_PACKET :   printf( "realignmentType               : REALIGNMENT_PER_PACKET\n" ); break;
          default:                                  printf( "realignmentType               : NOT VALID\n" ); break;
       }

       printf( "realignmentInterval           : %u\n", Apr1HzDebug.realignmentInterval );

       if( Apr1HzDebug.a24 == ZL303XX_TRUE ) printf( "a24                           : TRUE\n" );
       else                                printf( "a24                           : FALSE\n" );

       printf( "a2                            : %u.%u\n", Apr1HzDebug.a2.hi, Apr1HzDebug.a2.lo );

       if( Apr1HzDebug.a22 == ZL303XX_TRUE ) printf( "a22                           : TRUE\n" );
       else                                printf( "a22                           : FALSE\n" );
       {
           {
               Sint64T x4;
               ZL303XX_CONVERT_TO_64_SIGNED(x4, Apr1HzDebug.a4);
               if( x4 < 0 )
               {
                   x4 = -x4;
                   if( x4 < 0xffffffff )
                   {
                       printf( "a4                            : %u.%u  -(%u)\n",
                               Apr1HzDebug.a4.hi, Apr1HzDebug.a4.lo,
                               (Uint32T)(x4 & 0xffffffff) );
                   }
                   else
                   {
                       Uint64S t;
                       ZL303XX_CONVERT_FROM_64(t, x4);
                       printf( "a4                            : %u.%u  -(%u.%u)\n",
                               Apr1HzDebug.a4.hi, Apr1HzDebug.a4.lo, t.hi, t.lo );
                   }
               }
               else
               {
                   printf( "a4                            : %u.%u\n",
                           Apr1HzDebug.a4.hi, Apr1HzDebug.a4.lo );
               }
           }

           {
               Sint64T x31;
               ZL303XX_CONVERT_TO_64_SIGNED(x31, Apr1HzDebug.a31);
               if( x31 < 0 )
               {
                   x31 = -x31;
                   if( x31 < 0xffffffff )
                   {
                       printf( "a31                           : %u.%u  -(%u)\n",
                               Apr1HzDebug.a31.hi, Apr1HzDebug.a31.lo,
                               (Uint32T)(x31 & 0xffffffff) );
                   }
                   else
                   {
                       Uint64S t;
                       ZL303XX_CONVERT_FROM_64(t, x31);
                       printf( "a31                           : %u.%u  -(%u.%u)\n",
                               Apr1HzDebug.a31.hi, Apr1HzDebug.a31.lo, t.hi, t.lo );
                   }
               }
               else
               {
                   printf( "a31                           : %u.%u\n",
                           Apr1HzDebug.a31.hi, Apr1HzDebug.a31.lo );
               }
           }

           {
               Sint64T x5;
               ZL303XX_CONVERT_TO_64_SIGNED(x5, Apr1HzDebug.a5);
               if( x5 < 0 )
               {
                   x5 = -x5;
                   if( x5 < 0xffffffff )
                   {
                       printf( "a5                            : %u.%u  -(%u)\n",
                               Apr1HzDebug.a5.hi, Apr1HzDebug.a5.lo,
                               (Uint32T)(x5 & 0xffffffff) );
                   }
                   else
                   {
                       Uint64S t;
                       ZL303XX_CONVERT_FROM_64(t, x5);
                       printf( "a5                            : %u.%u  -(%u.%u)\n",
                               Apr1HzDebug.a5.hi, Apr1HzDebug.a5.lo, t.hi, t.lo );
                   }
               }
               else
               {
                   printf( "a5                            : %u.%u\n",
                           Apr1HzDebug.a5.hi, Apr1HzDebug.a5.lo );
               }
           }

           {
               Sint64T x32;
               ZL303XX_CONVERT_TO_64_SIGNED(x32, Apr1HzDebug.a32);
               if( x32 < 0 )
               {
                   x32 = -x32;
                   if( x32 < 0xffffffff )
                   {
                       printf( "a32                           : %u.%u  -(%u)\n",
                               Apr1HzDebug.a32.hi, Apr1HzDebug.a32.lo,
                               (Uint32T)(x32 & 0xffffffff) );
                   }
                   else
                   {
                       Uint64S t;
                       ZL303XX_CONVERT_FROM_64(t, x32);
                       printf( "a32                           : %u.%u  -(%u.%u)\n",
                               Apr1HzDebug.a32.hi, Apr1HzDebug.a32.lo, t.hi, t.lo );
                   }
               }
               else
               {
                   printf( "a32                           : %u.%u\n",
                           Apr1HzDebug.a32.hi, Apr1HzDebug.a32.lo );
               }
           }
       }

       if( Apr1HzDebug.a6 == ZL303XX_TRUE )      printf( "a6                            : TRUE\n" );
       else                                    printf( "a6                            : FALSE\n" );

       printf( "a7                            : %u\n", Apr1HzDebug.a7 );
       printf( "a10                           : %u\n", Apr1HzDebug.a10 );
       printf( "a18                           : %u\n", Apr1HzDebug.a18 );
       printf( "a19                           : %u\n", Apr1HzDebug.a19 );
       printf( "a20                           : %u\n", Apr1HzDebug.a20 );
       printf( "a21                           : %u\n", Apr1HzDebug.a21 );
       printf( "a26                           : %d\n", Apr1HzDebug.a26 );
       printf( "a30                           : %u\n", Apr1HzDebug.a30 );

       /* Delay to allow the output to finish printing. */
       OS_TASK_DELAY( 100 );
   }


   if (status == ZL303XX_OK)
   {
        Uint64S mostRecentCalculatedOffset;
        Uint32T mostRecentEstimatedNetworkQuality;
        zl303xx_BooleanE mostRecentAssigned;

        status = zl303xx_AprGetDevice1HzMostRecentAlignmentDifference(
                        hwParams,
                        &mostRecentCalculatedOffset,
                        &mostRecentEstimatedNetworkQuality,
                        &mostRecentAssigned);

        printf("Device most recent alignment:\n");
        printf("-----------------------------\n");

        if (status == ZL303XX_OK)
        {
            printf("calculatedOffset:            %d ns (0x%X%X)\n", mostRecentCalculatedOffset.lo, mostRecentCalculatedOffset.hi, mostRecentCalculatedOffset.lo);
            printf("estimatedNetworkQuality:     %u\n", mostRecentEstimatedNetworkQuality);
            printf("assigned:                    %u\n", mostRecentAssigned);
        }
        else
        {
            printf("N/A (status %d)\n", status);
            status = ZL303XX_OK;
        }
        printf("\n");

        status = zl303xx_AprGetServer1HzMostRecentAlignmentDifference(
                        hwParams,
                        serverId,
                        &mostRecentCalculatedOffset,
                        &mostRecentEstimatedNetworkQuality,
                        &mostRecentAssigned);

        printf("Server %d most recent alignment:\n", serverId);
        printf("--------------------------------\n");

        if (status == ZL303XX_OK)
        {
            printf("calculatedOffset:            %d ns (0x%X%X)\n", mostRecentCalculatedOffset.lo, mostRecentCalculatedOffset.hi, mostRecentCalculatedOffset.lo);
            printf("estimatedNetworkQuality:     %u\n", mostRecentEstimatedNetworkQuality);
            printf("assigned:                    %u\n", mostRecentAssigned);
        }
        else
        {
            printf("N/A (status %d)\n", status);
            status = ZL303XX_OK;
        }
        printf("\n");
    }

   return(status);
}


/*******************************************************************************

  Function Name:
   zl303xx_DebugAprPrintPSLFCLData

  Details:
   Debug command to print PSL/FCL parameters.

  Parameters:
   [in]   hwParams       Pointer to the device instance parameter structure

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_DebugAprPrintPSLFCLData
            (
            void *hwParams
            )
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PFConfigS par;
   Uint32T j;


   if( status == ZL303XX_OK )
   {
      status = ZL303XX_CHECK_POINTER(hwParams);
   }
   if( status == ZL303XX_OK )
   {
      status = zl303xx_AprGetPSLFCLConfigureData(hwParams, &par);
   }
   if (status == ZL303XX_OK )
   {
      printf("PSL/FCL data:\n");
      switch( par.PFMode )
      {
         case PFM_normal:
            printf("  PFMode                       : packet\n");
            break;
         case PFM_hybrid:
            printf("  PFMode                       : hybrid\n");
            break;
         case PFM_last:
         default :
            printf("  PFMode                       : unknown\n");
            break;
      }
      printf( "  setTimeStepTimeThreshold     : %d ns \n", par.setTimeStepTimeThreshold );
      printf( "  stepTimeAdjustTimeThreshold  : %d ns \n", par.stepTimeAdjustTimeThreshold );
      printf( "  stepTimeAdjustFreqThreshold  : %d ns \n", par.stepTimeAdjustFreqThreshold );
      printf( "  bUseAdjustTimeHybrid         : %s \n", (par.bUseAdjustTimeHybrid == ZL303XX_TRUE) ? "True": "False" );
      printf( "  bUseAdjustTimePacket         : %s \n", (par.bUseAdjustTimePacket == ZL303XX_TRUE) ? "True": "False" );
      printf( "  setTimeResolution            : %d ns\n", par.setTimeResolution );
      printf( "  stepTimeResolution           : %d ns\n", par.stepTimeResolution );
      printf( "  stepTimeMaxTimePerAdjustment : %d ns\n", par.stepTimeMaxTimePerAdjustment );
      printf( "  setTimeExecutionTime         : %d ms\n", par.setTimeExecutionTime );
      printf( "  stepTimeExecutionTime        : %d ms\n", par.stepTimeExecutionTime );
      printf( "  adjustTimeExecutionTime      : %d ms\n", par.adjustTimeExecutionTime );
      printf( "  adjustTimeMinThreshold       : %d ns\n", par.adjustTimeMinThreshold );
      printf( "  Phase Slope Limits \n" );
      printf( "     APRFrequencyLockedPhaseS...: %d ns/s\n", par.APRFrequencyLockedPhaseSlopeLimit );
      printf( "     APRFrequencyNotLockedPha...: %d ns/s\n", par.APRFrequencyNotLockedPhaseSlopeLimit );
      printf( "     APRFrequencyFastPhaseSlo...: %d ns/s\n", par.APRFrequencyFastPhaseSlopeLimit );
      for( j = 0; j < ZL303XX_MAX_NUM_PSL_LIMITS; j++ )
      {
         printf( "     adjSize1HzPSL[%d]: %6d   : PSL_1Hz: %d ns/s\n",
                                                      j, par.adjSize1HzPSL[j],
                                                      par.PSL_1Hz[j] );
      }
      printf( "  Frequency Change Limits \n" );
      printf( "     APRFrequencyLockedFreque...: %d ppt/s\n", par.APRFrequencyLockedFrequencyChangeLimit );
      printf( "     APRFrequencyNotLockedFre...: %d ppt/s\n", par.APRFrequencyNotLockedFrequencyChangeLimit );
      printf( "     APRFrequencyFastFrequenc...: %d ppt/s\n", par.APRFrequencyFastFrequencyChangeLimit );
      for( j = 0; j < ZL303XX_MAX_NUM_FCL_LIMITS; j++ )
      {
         printf( "     adjSize1HzFCL[%d]: %6d   : FCL_1Hz: %d ppt/s\n",
                                                      j, par.adjSize1HzFCL[j],
                                                      par.FCL_1Hz[j] );
      }
	  printf( "  Phase Adjustment Scaling Ratios \n" );
      for( j = 0; j < ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS; j++ )
      {
         printf( "     adjSize1HzAdjScaling[%d]: %6d   : adjScaling_1Hz: %d * 1/10 percent\n",
                                                      j, par.adjSize1HzAdjScaling[j],
                                                      par.adjScaling_1Hz[j] );
      }
      switch( par.TIEClear )
      {
         case TC_disabled:
            printf( "  TIEClear                     : disabled \n" );
            break;
         case TC_enabled:
            printf( "  TIEClear                     : enabled \n" );
            break;
         case TC_last:
         default:
            printf( "  TIEClear                     : unknown \n" );
            break;
      }
      printf( "  hybridLockTimeTarget         : %d s\n", par.hybridLockTimeTarget );
      printf( "  aTimer1                      : %d \n", par.aTimer1 );
      printf( "  acgd                         : %d \n", par.acgd );
      printf( "  a0cgq                        : %d \n", par.a0cgq );
      printf( "  a0cgs                        : %d \n", par.a0cgs );
      printf( "  a1cgq                        : %d \n", par.a1cgq );
      printf( "  a1cgs                        : %d \n", par.a1cgs );
      printf( "  lockInThreshold              : %d ns\n", par.lockInThreshold );
      printf( "  lockOutThreshold             : %d ns\n", par.lockOutThreshold );
      printf( "  lockInCount                  : %d \n", par.lockInCount );
      printf( "  lockedPhaseOutlierThreshold  : %d ns\n", par.lockedPhaseOutlierThreshold );
      printf( "  hybridHwLockTimeoutMs        : %d ms\n", par.hybridHwLockTimeoutMs );
      printf( "  staticOffset                 : %d ns\n", par.staticOffset );
      printf( "  setTimeRoundingZone          : %d ns\n", par.setTimeRoundingZone );
      printf( "  adjustFreqMinPhase           : %d ns\n", par.adjustFreqMinPhase );
      printf( "  maxHWDfRange                 : %d ppb\n", par.maxHWDfRange );
      printf( "  stepTimeDetectableThr        : %d ns\n", par.stepTimeDetectableThr );
      printf( "  adjustTimeDR                 : " );
      PRINT_PTR(par.adjustTimeDR); printf( "\n" );
      printf( "  packetTreatmentGuardTime     : %d ms (resolution is 125ms)\n", par.packetTreatmentGuardTime );
      printf( "  hybridCfReadFilter1          : %d \n", par.hybridCfReadFilter1 );
      printf( "  hybridCfReadFilter2          : %d \n", par.hybridCfReadFilter2 );
      printf( "  hybridCfOffsetDelay          : %d *125ms\n", par.hybridCfOffsetDelay );
      for( j = 0; j < ZL303XX_MAX_JUMP_TIME_SEQUENCE_STEPS; j++ )
      {
         if( par.jumpTimeSequence[j] == 0 )
         {
            printf( "  jumpTimeSequence[%d]          : %d (end)\n", j, par.jumpTimeSequence[j] );
            break;
         }
         else
         {
            printf( "  jumpTimeSequence[%d]          : %d \n", j, par.jumpTimeSequence[j] );
         }
      }
      for( j = 0; j < ZL303XX_MAX_JUMP_TIME_SEQUENCE_STEPS; j++ )
      {
         if( par.adjTimeSequence[j] == 0 )
         {
            printf( "  adjTimeSequence[%d]           : %d (end)\n", j, par.adjTimeSequence[j] );
            break;
         }
         else
         {
            printf( "  adjTimeSequence[%d]           : %d \n", j, par.adjTimeSequence[j] );
         }
      }
      printf( "  jumpTimeDelay_A              : %d *125ms\n", par.jumpTimeDelay_A );
      printf( "  jumpTimeDelay_B              : %d *125ms\n", par.jumpTimeDelay_B );
      printf( "  jumpTimeDelay_C              : %d *125ms\n", par.jumpTimeDelay_C );
      printf( "  jumpTimeDelay_D              : %d *125ms\n", par.jumpTimeDelay_D );
      printf( "  jumpTimeDelay_E              : %d *125ms\n", par.jumpTimeDelay_E );
      printf( "  jumpTimeDelay_F              : %d *125ms\n", par.jumpTimeDelay_F );
      printf( "  maxAdjFreqTime               : %d s\n", par.maxAdjFreqTime );
   }
   else
   {
      printf("zl303xx_AprGetPSLFCLConfigureData: Failed=%d\n", status);
   }

   return status;
}

/* zl303xx_DebugHybridState */
/**
   Displays the Hybrid DPLL parameters - Assumes DPLL 1 for ToP/Hybrid

  Parameters:
   [in]   zl303xx_Params  Pointer to the clock device of interest.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_DebugHybridState(void *hwParams)
{
   zlStatusE status = ZL303XX_OK;

   /*  Display the Hybrid Indication Flag */
   /*if (status == ZL303XX_OK)
   {
      printf("Hybrid Flag: %s\n", zl303xx_AprGetHybridServerFlag() ? "TRUE" : "FALSE");
   }*/

   if (hwParams != NULL)
   {
       printf("Device = %p\n", hwParams);
   }
   else
   {
       printf("Device = %p\n", hwParams);
       return ZL303XX_INVALID_POINTER;
   }

   return status;
}
/**

  Function Name:
   zl303xx_DebugGetServerStatus

  Details:
    Code to retrieve the status information for the specified packet/hybrid server

  Parameters:
   [in]   clkGenId   Pointer to the device instance
   [in]   serverId   Identifier of the server

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE zl303xx_DebugGetServerStatus(void *clkGenId, Uint16T serverId)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_BooleanE tsValid, uMode;
    zl303xx_AprTsFormatE tsFormat;
    Uint32T clkstabilityIndicator;
    zl303xx_BooleanE singlePath = ZL303XX_FALSE;

    if (clkGenId != NULL)
    {
        printf("Device %p, Server clock %u status:\n", clkGenId, (Uint32T)serverId);
    }
    else
    {
        printf("Device = %p\n", clkGenId);
        return ZL303XX_INVALID_POINTER;
    }

    status = zl303xx_AprGetServerTimestampFormat(clkGenId, serverId, &tsFormat);
    if (status == ZL303XX_OK)
    {
        printf("Timestamps from server %u on the device %p is in format of %s\n", (Uint32T)serverId, clkGenId, tsFormat? "Non-PTP" : "PTP");
    }
    else
    {
        printf("Call to zl303xx_AprGetServerTimestampFormat() failed=%d \n", status);
    }


    status = zl303xx_AprIsServerClkStreamValid(clkGenId, serverId, &tsValid);
    if (status == ZL303XX_OK)
    {
        if (tsValid == ZL303XX_TRUE)
        {
            Uint16T activeServerId;

            printf("Timestamps from server %u on the device %p are VALID.\n", (Uint32T)serverId, clkGenId);

            (void)zl303xx_AprGetDeviceCurrActiveRef(clkGenId, &activeServerId);
            if (activeServerId == serverId)
            {
                if (status == ZL303XX_OK)
                {
                    if ((status = zl303xx_DebugCGULockState(clkGenId)) != ZL303XX_OK )
                    {
                        printf("zl303xx_DebugHybridState: No reference currently selected.\n");
                    }
                }
            }
            else
            {
                printf("This Server is not the current active reference\n");
                printf("Therefore Device Lock state is Not Applicable to this server\n");
            }
        }
        else
        {
            printf("Timestamps from server %u on the device %p are INVALID.\n", (Uint32T)serverId, clkGenId);
            printf("Therefore Device Lock state is Not Applicable to this server\n");
        }

        status = zl303xx_AprGetServerClkStabilityIndicator(clkGenId, serverId, &clkstabilityIndicator);
        if (status == ZL303XX_OK)
        {
            printf("The server %u on the device %p has clock stability factor of %u ppt\n", (Uint32T)serverId, clkGenId, (Uint32T)clkstabilityIndicator);
        }
        else
        {
            printf("Call to zl303xx_AprGetServerClkStabilityIndicator() failed=%d \n", status);
        }
    }
    else
    {
        printf("Call to zl303xx_AprIsServerClkStreamValid() failed=%d \n", status);
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_AprIsServerClkInUMode(clkGenId, serverId, &uMode);
        if (status == ZL303XX_OK)
        {
            if (uMode == ZL303XX_TRUE)
            {
                printf("Clock from server %u on the device %p is in U Mode.\n", (Uint32T)serverId, clkGenId);
            }
            else
            {
                printf("Clock from server %u on the device %p is NOT in U Mode.\n", (Uint32T)serverId, clkGenId);
            }
        }
        else
        {
            printf("Call to zl303xx_AprGetServerClkStabilityIndicator() failed=%d \n", status);
        }
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_AprIsServerUseSinglePath(clkGenId, serverId, &singlePath);
        if (status == ZL303XX_OK)
        {
            if (singlePath == ZL303XX_TRUE)
            {
                printf("The server %u on the device %p is using SINGLE path.\n", (Uint32T)serverId, clkGenId);
            }
            else
            {
                printf("The server %u on the device %p is using BOTH (FORWARD & REVERSE) paths.\n", (Uint32T)serverId, clkGenId);
            }
        }
        else
        {
            printf("Call to zl303xx_AprIsServerUseSinglePath() failed=%d \n", status);
        }
    }

    if (status == ZL303XX_OK)
    {
        zl303xx_AprServerStatusFlagsS statusFlags;

        status = zl303xx_AprGetServerStatusFlags (clkGenId, serverId, &statusFlags);
        if (status == ZL303XX_OK)
        {
            printf("Server %d Status:\n", serverId);
            printf("\tLocked   = %s\n",  ZL303XX_BOOL_TO_STR(statusFlags.L));
            printf("\tValid    = %s\n",  ZL303XX_BOOL_TO_STR(statusFlags.V));
            printf("\tHoldover = %s\n",  ZL303XX_BOOL_TO_STR(statusFlags.H));
            printf("\tPA       = %s\n",  ZL303XX_BOOL_TO_STR(statusFlags.PA));
        }
        else
        {
            printf("Call to zl303xx_AprGetServerStatusFlags() failed=%d \n", status);
        }
    }

    return status;
} /* END zl303xx_DebugGetServerStatus */


zlStatusE zl303xx_DebugAprHoldoverQualityParams(void *hwParams)
{
    zlStatusE status = ZL303XX_OK;

    zl303xx_HoldoverQualityParamsS hoParam;
    OS_MEMSET(&hoParam, 0, sizeof(hoParam));

    status = zl303xx_AprGetCurrentHoldoverQualityParameters(hwParams, &hoParam);

    if (status == ZL303XX_OK)
    {
        printf("zl303xx_DebugAprHoldoverQualityParams:\n");
        #define _PRINTIT(x) printf("  %-35s: %d\n", #x, x);

        _PRINTIT(hoParam.oneHzEnabled);
        _PRINTIT(hoParam.make1HzAdjustmentsDuringHoldover);
        _PRINTIT(hoParam.serverId);
        _PRINTIT(hoParam.currentAlgHoldover);
        _PRINTIT(hoParam.lastAlgHoldover);
        _PRINTIT(hoParam.currentHWDF);
        _PRINTIT(hoParam.holdSecs);
        _PRINTIT(hoParam.devMode);
        _PRINTIT(hoParam.oscFilter);
        _PRINTIT(hoParam.aprState);
        _PRINTIT(hoParam.localOscillatorFreqCategory);
        _PRINTIT(hoParam.currentHoldoverCategory);
        _PRINTIT(hoParam.currentHoldoverAccuracyPpt);
        _PRINTIT(hoParam.holdoverAction);
        _PRINTIT(hoParam.holdoverQuality);
        _PRINTIT(hoParam.syncEStatus);
        _PRINTIT(hoParam.currentSyncEAccuracyPpt);
        _PRINTIT(hoParam.inSpecHoldoverLimit);
        _PRINTIT(hoParam.holdoverTIEns);
        _PRINTIT(hoParam.holdoverIncrementCount);
        _PRINTIT(hoParam.holdoverSecsPerIncrement);
        #undef _PRINTIT
    }

    return status;
}

/**zl303xx_DebugAprGetSWVersion

   This function is used to get the encoded software version and patch level

  Return Value: zlStatusE       ZL303XX_OK

*****************************************************************************/
zlStatusE zl303xx_DebugAprGetSWVersion(void)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T major = zl303xx_AprReleaseVersionInt >> 12;
    Uint32T minor = zl303xx_AprReleaseVersionInt >> 8 & 0xF;
    Uint32T micro = zl303xx_AprReleaseVersionInt >> 4 & 0xF;
    Uint32T patch = zl303xx_AprReleaseVersionInt & 0xF;

    printf("APR version = %d.%d.%d - Patch %d\n",
           major, minor, micro, patch);

    return status;
}

/*****************   END   ****************************************************/
