

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Supporting code for the Example-specific utilities
*
*******************************************************************************/


/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_Os.h"
#include "zl303xx_Trace.h"
#include "zl303xx_HWTimer.h"
#include "zl303xx_ExampleUtils.h"
#include "zl303xx_DeviceIf.h"

#if defined ZLS30341_INCLUDED
    #include "zl303xx.h"
#endif

#if (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD) && defined OS_LINUX
#include "zl303xx_ZLE30360.h"
#endif






#if defined ZLS30771_INCLUDED
#include "zl303xx_Dpll77x.h"
#endif


    #include "zl303xx_ExampleAprGlobals.h"

#if defined ZLS30390_INCLUDED
    #include "zl303xx_ExamplePtpGlobals.h"
    #include "zl303xx_PortingFunctions.h"
#endif

#if defined ZLS30390_INCLUDED 
    #include "zl303xx_PtpApi.h"
    #include "zl303xx_Ptpd.h"
    #include "zl303xx_PtpFunctionExport.h"
#endif

#if defined OS_LINUX && defined OHLY_TIMESTAMPS && defined LINUX_3
    #include "zl303xx_LinuxOhlyTS.h"
#endif


#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
#include "zl303xx_DeviceSpec.h"
#include "zl303xx_Init.h"
#endif



#if defined ZLS30361_INCLUDED
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#include <string.h>
#include <stdlib.h>

#include "f_du_reg_eep.h"
#include "BPF_HM_DEVC.h"


/*****************   DEFINES     **********************************************/


#define EVENT_PORT_CONFIG     DEFAULT_PTP_UDP_EVENT_PORT
#define GENERAL_PORT_CONFIG   DEFAULT_PTP_UDP_GENERAL_PORT

/* This unique local address (ULA) and subnet are used when an IPv6 socket
 * protocol is being used, but the default IPv4 address(es) are passed into
 * setExampleIpSrc() and setExampleUnicastIpDest(). Multicast examples must
 * always use a valid IPv6 multicast address (starts with FF).
 *
 * The method used to generate a Global ID for the ULA can be found in
 * IETF RFC4193. A subnet ID is also defined, so the prefix length for these
 * "converted from IPv4" addresses have a prefix length of 64.
 *
 * The Local IPv6 addresses are created using a pseudo-randomly
 * allocated global ID.  They have the following format:
 *
 *    | 7 bits |1|  40 bits   |  16 bits  |          64 bits           |
 *    +--------+-+------------+-----------+----------------------------+
 *    | Prefix |L| Global ID  | Subnet ID |        Interface ID        |
 *    +--------+-+------------+-----------+----------------------------+
 *
 * Important: The pseudo-random global ID algorithm was not used to generate this.
 * These values should be changed, or actual IPv6 addresses should be set by
 * calling setExampleIpSrc() and setExampleUnicastIpDest(). */
#define EXAMPLE_IPV6_ULA_ID     "FC0D:8C4A:A990"
#define EXAMPLE_IPV6_SUBNET_ID  "BA23"

#define __APR_DEVICE_MODE_TO_STR(mode) \
            (((mode) == ZL303XX_PACKET_MODE) ? "PACKET" : \
            (((mode) == ZL303XX_HYBRID_MODE) ? "HYBRID" : \
            (((mode) == ZL303XX_ELECTRIC_MODE) ? "ELECTRIC" : "UNKNOWN)")))

/*****************   TYPEDEFS     **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/
char *exampleIpv4ToIpv6(const char *ipv4, char *ipv6, Uint16T bufLen);
void osAddIpv6Addr(const char *ifName, const char *addr);
void osSetIpv6Addr(const char *ifName, const char *addr);

/*****************   STATIC GLOBAL VARIABLES   ********************************/

extern zl303xx_BooleanE settingIsUnicast;

#if defined APR_INCLUDED
int useCaseId = 0;  /* Default - TCXO */
static exampleAprConfigIdentifiersE exampleAprConfigId = ACI_DEFAULT ;
#endif

FILE *tsLogFileP = NULL;  /* TimeStamp log file pointer*/
char tsLogFilename[32] = {0};  /* NULL filename: do NOT log to a file! */
/* UNCOMMENT TO ENABLE LOGGING TO A FILE */
/* char tsLogFilename[32] = "./TIMESTAMP_CAPTURE";*/  /* TimeStamp log filename */

#if defined APR_INCLUDED
/* Indicates whether to enable/disable type 2b pll (default is disable) */
static zl303xx_BooleanE type_2b_enabled = ZL303XX_FALSE;
#endif


/*****************   EXTERN GLOBAL VARIABLES   ******************************/
extern Uint8T TARGET_DPLL;
extern Uint8T NCO_ASSIST_DPLL;

extern zl303xx_ParamsS *zl303xx_Params0;


#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
extern zl303xx_BooleanE settingSlaveCapable;
extern zl303xx_DeviceModeE settingDeviceMode;
extern zl303xx_BooleanE runningCannedExample;
#endif


#if defined ZLS30701_INCLUDED || defined ZLS30721_INCLUDED || defined ZLS30751_INCLUDED
extern zl303xx_BooleanE exampleAppRunning;
#endif


/*****************   EXPORTED GLOBAL VARIABLES   ******************************/
#if defined APR_INCLUDED
usrOvr1HzArryS usrOvr1HzFwd[ZL303XX_APR_1HZ_MAX_X_PARAMETERS];                        /* User Override values of 1Hz XParams */
usrOvr1HzArryS usrOvr1HzRev[ZL303XX_APR_1HZ_MAX_X_PARAMETERS];                        /* User Override values of 1Hz XParams */

usrOvrXParamArryS usrOvrXParamsFwd[ZL303XX_ALGO_MAX_MODIFIED_PARAMETERS];                /* User Override of XParams */
usrOvrXParamArryS usrOvrXParamsRev[ZL303XX_ALGO_MAX_MODIFIED_PARAMETERS];                /* User Override of XParams */
#endif

/*****************   EXTERNAL FUNCTION DEFINITIONS   **************************/

#if defined APR_INCLUDED
/**
 * Updates the global APR xParam overrides with given settings.
 * The overrides need to be applied by using exampleAprOverrideXParamApply().
 *
 */
zlStatusE exampleAprOverrideXParam(zl303xx_BooleanE fwdPath, Uint32T param, Uint32T value)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T indx, empty = ~0;
    usrOvrXParamArryS *usrOvrXParams = (fwdPath == ZL303XX_TRUE) ? &usrOvrXParamsFwd[0] : &usrOvrXParamsRev[0];

    /* Search for existing or empty entry to change */
    for (indx = 0; indx < ZL303XX_ALGO_MAX_MODIFIED_PARAMETERS; indx++)
    {
        if (usrOvrXParams[indx].active == ZL303XX_FALSE && indx < empty)
        {
            empty = indx;
        }

        if (usrOvrXParams[indx].active == ZL303XX_TRUE &&
            usrOvrXParams[indx].param == param)
        {
            /* Found existing entry */
            break;
        }
    }

    if (indx >= ZL303XX_ALGO_MAX_MODIFIED_PARAMETERS)
    {
        /* No existing entry found */
        if (empty >= ZL303XX_ALGO_MAX_MODIFIED_PARAMETERS)
        {
            status = ZL303XX_TABLE_FULL;
        }
        else
        {
            indx = empty;  /* Use empty entry to activate */
        }
    }

    if (status == ZL303XX_OK)
    {
        /* Activate entry */
        usrOvrXParams[indx].active = ZL303XX_TRUE;
        usrOvrXParams[indx].param = param;
        usrOvrXParams[indx].value = value;
    }

    return status;
}

/**
 * Resets a single global APR xParam override to default values
 *
 */
zlStatusE exampleAprOverrideXParamReset(zl303xx_BooleanE fwdPath, Uint32T param)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T indx;
    usrOvrXParamArryS *usrOvrXParams = (fwdPath == ZL303XX_TRUE) ? &usrOvrXParamsFwd[0] : &usrOvrXParamsRev[0];

    /* Search for existing or empty entry to change */
    for (indx = 0; indx < ZL303XX_ALGO_MAX_MODIFIED_PARAMETERS; indx++)
    {
        if (usrOvrXParams[indx].active == ZL303XX_TRUE &&
            usrOvrXParams[indx].param == param)
        {
            usrOvrXParams[indx].active = ZL303XX_FALSE;
        }
    }

    return status;
}

/**
 * Resets all global APR xParam overrides to default values
 *
 */
zlStatusE exampleAprOverrideXParamResetAll(zl303xx_BooleanE fwdPath)
{
    if (fwdPath == ZL303XX_TRUE)
    {
        memset(&usrOvrXParamsFwd, 0, sizeof(usrOvrXParamsFwd));
    }
    else
    {
        memset(&usrOvrXParamsRev, 0, sizeof(usrOvrXParamsRev));
    }
    return ZL303XX_OK;
}

/**
 * Applies the global APR xParam overrides to a given zl303xx_AprAddServerS object
 *
 */
zlStatusE exampleAprOverrideXParamApply(zl303xx_AprAddServerS *aprAddServer)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T indx, j;

    status = ZL303XX_CHECK_POINTER(aprAddServer);

    if (status == ZL303XX_OK)
    {
        for (indx = 0, j = 0; indx < ZL303XX_ALGO_MAX_MODIFIED_PARAMETERS; indx++)
        {
            if (usrOvrXParamsFwd[indx].active == ZL303XX_FALSE)
                continue;

            aprAddServer->overrideAlgorithmData[0].active[j] = ZL303XX_TRUE;
            aprAddServer->overrideAlgorithmData[0].param[j] = usrOvrXParamsFwd[indx].param;
            aprAddServer->overrideAlgorithmData[0].value[j] = usrOvrXParamsFwd[indx].value;
            j++;
        }

        for (indx = 0, j = 0; indx < ZL303XX_ALGO_MAX_MODIFIED_PARAMETERS; indx++)
        {
            if (usrOvrXParamsRev[indx].active == ZL303XX_FALSE)
                continue;

            aprAddServer->overrideAlgorithmData[1].active[j] = ZL303XX_TRUE;
            aprAddServer->overrideAlgorithmData[1].param[j] = usrOvrXParamsRev[indx].param;
            aprAddServer->overrideAlgorithmData[1].value[j] = usrOvrXParamsRev[indx].value;
            j++;
        }
    }

    return status;
}

/**
 * Updates the global 1Hz xParam overrides with given settings
 * The overrides need to be applied by using exampleAprOverride1HzXParamApply().
 */
zlStatusE exampleAprOverride1HzXParam(zl303xx_BooleanE fwdPath, Uint32T param, Uint32T value)
{
    zlStatusE status = ZL303XX_OK;

    /* Select the override table based on the path selection */
    usrOvr1HzArryS *usrOvr1HzTable = (fwdPath == ZL303XX_TRUE) ? &usrOvr1HzFwd[0] : &usrOvr1HzRev[0];

    if (param >= ZL303XX_APR_1HZ_MAX_X_PARAMETERS)
        return ZL303XX_PARAMETER_INVALID;

    usrOvr1HzTable[param].active = ZL303XX_TRUE;
    usrOvr1HzTable[param].value = value;


    return status;
}

/**
 * Updates the global 1Hz xParam overrides with given settings
 * The overrides need to be applied by using exampleAprOverride1HzXParamApply().
 */
zlStatusE exampleAprOverride1HzXParamReset(zl303xx_BooleanE fwdPath, Uint32T param)
{
    zlStatusE status = ZL303XX_OK;

    /* Select the override table based on the path selection */
    usrOvr1HzArryS *usrOvr1HzTable = (fwdPath == ZL303XX_TRUE) ? &usrOvr1HzFwd[0] : &usrOvr1HzRev[0];

    if (param >= ZL303XX_APR_1HZ_MAX_X_PARAMETERS)
        return ZL303XX_PARAMETER_INVALID;

    usrOvr1HzTable[param].active = ZL303XX_FALSE;


    return status;
}

/**
 * Apply the global 1Hz xParam overrides to given APR 1Hz configuration
 *
 */
zlStatusE exampleAprOverride1HzXParamResetAll(zl303xx_BooleanE fwdPath)
{
    if (fwdPath == ZL303XX_TRUE)
    {
        memset(&usrOvr1HzFwd, 0, sizeof(usrOvr1HzFwd));
    }
    else
    {
        memset(&usrOvr1HzRev, 0, sizeof(usrOvr1HzRev));
    }

    return ZL303XX_OK;
}

/**
 * Apply the global 1Hz xParam overrides to given APR 1Hz configuration
 *
 */
zlStatusE exampleAprOverride1HzXParamApply(zl303xx_BooleanE fwdPath, zl303xx_AprConfigure1HzS *config1Hz)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T param;

    /* Select the override table based on the path selection */
    usrOvr1HzArryS *usrOvr1HzTable = (fwdPath == ZL303XX_TRUE) ? &usrOvr1HzFwd[0] : &usrOvr1HzRev[0];

    status = ZL303XX_CHECK_POINTER(config1Hz);

    if (status == ZL303XX_OK)
    {
        for (param = 0; param < ZL303XX_APR_1HZ_MAX_X_PARAMETERS; param++)
        {
            if (usrOvr1HzTable[param].active == ZL303XX_TRUE)
            {
                config1Hz->xParams.p[param] = usrOvr1HzTable[param].value;
            }
        }
    }

    return status;
}

#endif



#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
/* These examples are used by the advanced CLI included on our EVBs */
/**
   An example that uses external configuration globals to initialize clock and port settings. It is
   used in conjunction with exampleConfig().

*******************************************************************************/
zlStatusE exampleSetup(zl303xx_DeviceModeE deviceMode, zl303xx_PtpVersionE ptpVersion);
zlStatusE exampleSetup(zl303xx_DeviceModeE deviceMode, zl303xx_PtpVersionE ptpVersion)
{
    zlStatusE status = ZL303XX_OK;
    exampleAppClockS *clck;

    ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 2,
                "exampleSetup(): deviceMode=%d, ptpVersion=%d", deviceMode, ptpVersion, 0,0,0,0);

    if (!(deviceMode >= ZL303XX_MODE_REF_TOP) )
    {
        fprintf(stderr, "exampleSetup: deviceMode=%d is NOT SET CORRECTLY!\n", deviceMode);
        return ZL303XX_DATA_CORRUPTION;
    }

    if (ptpVersion)
    {
        /* warning removal */
    };

    /* If using Telecom profiles, then override with example code */
    if (zl303xx_GetPtpProfile() == ZL303XX_PTP_PROFILE_TELECOM_G8275_2)
    {
        runningCannedExample = ZL303XX_TRUE;

        if (settingDeviceMode == ZL303XX_MODE_REF_BC)
        {
            printf("\n\nexampleSetup(): calling examplePtpTelecomPhaseG8275p2Bc() This will override settings. (consider --ex -rce #)\n\n");
            return examplePtpTelecomPhaseG8275p2Bc();
        }
        else if (settingSlaveCapable == ZL303XX_TRUE)
        {
            printf("\n\nexampleSetup(): calling examplePtpTelecomPhaseG8275p2Slave() This will override settings. (consider --ex -rce #)\n\n");
            return examplePtpTelecomPhaseG8275p2Slave();
        }
        else
        {
            printf("\n\nexampleSetup(): calling examplePtpTelecomPhaseG8275p2Master() This will override settings. (consider --ex -rce #)\n\n");
            return examplePtpTelecomPhaseG8275p2Master();
        }
    }
    else if ((zl303xx_GetPtpProfile() == ZL303XX_PTP_PROFILE_TELECOM_G8275_1v1) ||
             (zl303xx_GetPtpProfile() == ZL303XX_PTP_PROFILE_TELECOM_G8275_1v2))
        {
            runningCannedExample = ZL303XX_TRUE;

            if (settingDeviceMode == ZL303XX_MODE_REF_BC)
            {
                printf("\n\nexampleSetup(): calling examplePtpTelecomPhaseBcIPv4() This will override settings. (consider --ex -rce #)\n\n");
                return examplePtpTelecomPhaseBcIPv4();
            }
            else if (settingSlaveCapable == ZL303XX_TRUE)
            {
                printf("\n\nexampleSetup(): calling examplePtpTelecomPhaseSlaveIPv4() This will override settings. (consider --ex -rce #)\n\n");
                return examplePtpTelecomPhaseSlaveIPv4();
            }
            else
            {
                printf("\n\nexampleSetup(): calling examplePtpTelecomPhaseMasterIPv4() This will override settings. (consider --ex -rce #)\n\n");
                return examplePtpTelecomPhaseMasterIPv4();
            }
    }
/* #warning MSCC: Additional Ptp Profiles are added here */
/* Power Profile examples use only 802.3 but we support only UDP/IPV4 */
/*        else if (zl303xx_GetPtpProfile() == ZL303XX_PTP_PROFILE_POWER_C37_238_2011)
    {
        if (settingSlaveCapable == ZL303XX_TRUE)
            examplePtpPowerProfileSlave();
        else
            examplePtpPowerProfileMaster();
        printf("Using ZL303XX_PTP_PROFILE_POWER_C37_238_2011\n");
    }
*/

    /* Initialize global environment variables. */
    if (status == ZL303XX_OK)
    {
        status = exampleEnvInit();
    }

   if (status == ZL303XX_OK)
   {
   #if defined _ZL303XX_ZLE30360_BOARD
       status = exampleAppStructInit(4, 4, 8, &zlExampleApp);    /* 4 Clocks, 4 ports, 8 streams (Servers)*/
   #elif defined _ZL303XX_ZLE1588_BOARD
       printf("ZLE30360: 4 Clocks, 4 ports, 8 streams\n");
       status = exampleAppStructInit(4, 4, 8, &zlExampleApp);    /* 4 Clocks, 4 ports, 8 streams (Servers)*/
   #else
       status = exampleAppStructInit(1, 4, 4, &zlExampleApp);    /* 1 Clock, 4 ports, 4 streams */
   #endif
       if (status != ZL303XX_OK)
           ZL303XX_TRACE_ALWAYS("exampleSetup(): exampleAppStructInit() failure = %d.", status, 0,0,0,0,0);
   }

    if (status == ZL303XX_OK)
    {
        exampleAppRunning = ZL303XX_TRUE;
    }

   /* Initialize clock using global accessors */
   if (status == ZL303XX_OK)
   {
   #if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
       clck = &zlExampleApp.clock[TARGET_DPLL];
   #else
      clck = &zlExampleApp.clock[0];
      if (clck->started == ZL303XX_TRUE)
            status = ZL303XX_MULTIPLE_INIT_ATTEMPT;
   #endif
    }

#ifdef APR_INCLUDED
    if (status == ZL303XX_OK)
    {
        /* Default configuration values:
        * clck->apr.device.pullInRange = ZL303XX_APR_PIR_INVALID;              - DEPRICATED - use zl303xx_AprAddServer.pullInRange
        * clck->apr.device.hwDcoResolutionInPpt = 1;                         - Device hardware DCO resolution in ppt;
        * clck->apr.device.bWarmStart = ZL303XX_FALSE;                         - Warm start or not;
        * clck->apr.device.warmStartInitialFreqOffset = 0;                   - Initial frequency offset for warmstart in ppt;
        * clck->apr.device.PFConfig.setTimeStepTimeThreshold = 1000000000;   - Threshold of setTime and stepTime in ns;
        * clck->apr.device.PFConfig.stepTimeAdjustFreqThreshold = 500000;    - Threshold of stepTime and adjustFreq in ns;
        * clck->apr.device.PFConfig.setTimeResolution = 1000000000;          - Resolution of setTime in ns;
        * clck->apr.device.PFConfig.stepTimeResolution = 500000;             - Resolution of stepTime in ns;
        * clck->apr.device.PFConfig.setTimeExecutionTime = 4000;             - Execution time of setTime in ms;
        * clck->apr.device.PFConfig.stepTimeExecutionTime = 4000;            - Execution time of stepTime in ms;
        * clck->apr.device.PFConfig.stepTimeMaxTimePerAdjustment = 1000000000;  - Max time (in ns) that each stepTime() adjustment is limited to;
        * clck->apr.device.PFConfig.TIEClear = TC_enabled;                   - Tie clear enabled;
        * clck->apr.device.PFConfig.hybridHwLockTimeoutMs = 37000;           - HW lock timeout (hybrid mode only) in ms;
        * clck->apr.device.fixedDFInPpb = 0;                                 - NOT USED - debugging only!;
        */

        if ((deviceMode == ZL303XX_MODE_REF_TOP) &&
            !((zl303xx_AprGetDeviceOptMode() == ZL303XX_PACKET_MODE) || (zl303xx_AprGetDeviceOptMode() == ZL303XX_HYBRID_MODE)) )
        {
            ZL303XX_TRACE_ALWAYS("Your APR device mode should be pkt to match the call to exampleSetup()", 0,0,0,0,0,0);
        }
        if ((deviceMode == ZL303XX_MODE_REF_EXTERNAL) &&
            !(zl303xx_AprGetDeviceOptMode() == ZL303XX_ELECTRIC_MODE) )
        {
            ZL303XX_TRACE_ALWAYS("Your APR device mode should be electric to match the call to exampleSetup()", 0,0,0,0,0,0);
        }

        clck->apr.device.devMode = zl303xx_AprGetDeviceOptMode();
        clck->apr.device.devHybridAdjMode = zl303xx_AprGetDeviceHybridAdjMode();
        clck->apr.device.dcs = zl303xx_AprGetDcs();
        clck->apr.device.hwDcoResolutionInPpt = zl303xx_AprGetHwDcoResolution();
        clck->apr.device.enterPhaseLockStatusThreshold = zl303xx_AprGetEnterPhaseLockStatusThreshold();
        clck->apr.device.exitPhaseLockStatusThreshold = zl303xx_AprGetExitPhaseLockStatusThreshold();
        clck->apr.device.bWarmStart = zl303xx_AprGetWarmStart();
        clck->apr.device.warmStartInitialFreqOffset = zl303xx_AprGetWsInitialOffset();
        clck->apr.device.PFConfig.stepTimeResolution = zl303xx_AprGetPFStepTimeResolution();
        clck->apr.device.PFConfig.stepTimeAdjustFreqThreshold = zl303xx_AprGetStepTimeAdjustFreqThreshold();
        clck->apr.device.PFConfig.lockedPhaseOutlierThreshold = zl303xx_AprGetPFLockedPhaseOutlierThreshold();
        clck->apr.device.PFConfig.APRFrequencyLockedPhaseSlopeLimit = zl303xx_AprGetAPRFrequencyLockedPhaseSlopeLimit();
        clck->apr.device.PFConfig.APRFrequencyNotLockedPhaseSlopeLimit = zl303xx_AprGetAPRFrequencyNotLockedPhaseSlopeLimit();
        clck->apr.device.PFConfig.APRFrequencyFastPhaseSlopeLimit = zl303xx_AprGetAPRFrequencyFastPhaseSlopeLimit();
        clck->apr.device.PFConfig.APRFrequencyLockedFrequencyChangeLimit = zl303xx_AprGetAPRFrequencyLockedFrequencyChangeLimit();
        clck->apr.device.PFConfig.APRFrequencyNotLockedFrequencyChangeLimit = zl303xx_AprGetAPRFrequencyNotLockedFrequencyChangeLimit();
        clck->apr.device.PFConfig.APRFrequencyFastFrequencyChangeLimit = zl303xx_AprGetAPRFrequencyFastFrequencyChangeLimit();
        clck->apr.device.PFConfig.onlyAllowSteps = zl303xx_AprGetOnlyAllowSteps();

        {
            Uint32T j;

            for (j = 0; j < ZL303XX_MAX_NUM_PSL_LIMITS; j++ )
            {
                zl303xx_AprGetPSL(j, &(clck->apr.device.PFConfig.adjSize1HzPSL[j]),
                                &(clck->apr.device.PFConfig.PSL_1Hz[j]) );
            }
            for (j = 0; j < ZL303XX_MAX_NUM_FCL_LIMITS; j++ )
            {
                zl303xx_AprGetFCL(j, &(clck->apr.device.PFConfig.adjSize1HzFCL[j]),
                                &(clck->apr.device.PFConfig.FCL_1Hz[j]) );
        }
            for (j = 0; j < ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS; j++ )
            {
                zl303xx_AprGetAdjScaling(j, &(clck->apr.device.PFConfig.adjSize1HzAdjScaling[j]),
                                &(clck->apr.device.PFConfig.adjScaling_1Hz[j]) );
            }
        }
        clck->apr.device.PFConfig.adjustFreqMinPhase = zl303xx_AprGetPFAdjustFreqMinPhase();
      #if defined ZLS30721_INCLUDED
        if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
        {
        /* Even though the 72x can step small values (default = 488ns), The
           step detection mechanism may not be able to detect such a small
           value. stepTimeAdjustFreqThreshold should be set such that the
           detection mechanism can detect the step. Any smaller value will
           cause packets to be discarded during the step. */
           if( zl303xx_GetStepTimeDetectableThr() > clck->apr.device.PFConfig.stepTimeAdjustFreqThreshold )
           {
        clck->apr.device.PFConfig.stepTimeAdjustFreqThreshold = zl303xx_GetStepTimeDetectableThr();
           }

        /* For the 72x, stepTimeResolution should be set to 1. */
        clck->apr.device.PFConfig.stepTimeResolution = 1;
        }
      #endif
        clck->apr.device.clkStabDelayLimit = zl303xx_AprGetClkStabDelayIterations();
        clck->apr.device.setTimePacketTreatment = zl303xx_AprGetSetTimePacketTreatment();
        clck->apr.device.stepTimePacketTreatment = zl303xx_AprGetStepTimePacketTreatment();
        clck->apr.device.adjustTimePacketTreatment = zl303xx_AprGetAdjustTimePacketTreatment();
        clck->apr.device.legacyTreatment = zl303xx_AprGetLegacyTreatment();

        clck->apr.device.PFConfig.adjustTimeMinThreshold = zl303xx_GetAdjustTimeMinThreshold();
        clck->apr.device.PFConfig.bUseAdjustTimeHybrid = zl303xx_GetUseAdjustTimeHybrid();
        clck->apr.device.PFConfig.bUseAdjustTimePacket = zl303xx_GetUseAdjustTimePacket();
        clck->apr.device.PFConfig.stepTimeDetectableThr = zl303xx_GetStepTimeDetectableThr();

    }
#endif

#if defined ZLS30390_INCLUDED
    if (status == ZL303XX_OK)
    {
        /* Create a PTP clock. This represents the global properties of the PTP
          clock on this node */

        /* Set other flags and create the clock. */
        clck->ptp.config.profile = zl303xx_GetPtpProfile();

        /* Set the default priority values. */
        clck->ptp.config.defaultDS.priority1 = zl303xx_GetPriority1();
        clck->ptp.config.defaultDS.priority2 = zl303xx_GetPriority2();

        /* Set the required clockQuality attributes */
        clck->ptp.config.defaultDS.clockQuality.clockClass = zl303xx_GetDefaultClockClass();
        clck->ptp.config.defaultDS.clockQuality.clockAccuracy = zl303xx_GetDefaultClockAccuracy();

        clck->ptp.config.defaultDS.domainNumber = zl303xx_GetDomainNumber();

        /* If BMC is on, this cannot be slave only.
         * If EXTERNAL reference is on, it cannot be slave only.      */
        if ((zl303xx_GetBmcFlag() == ZL303XX_TRUE) ||
            (deviceMode == ZL303XX_MODE_REF_EXTERNAL) ||
            (zl303xx_GetBoundaryFlag()))
        {
            clck->ptp.config.defaultDS.slaveOnly = ZL303XX_FALSE;
        }
        else
        {
            clck->ptp.config.defaultDS.slaveOnly = ZL303XX_TRUE;
        }

        clck->ptp.config.defaultDS.twoStepFlag = zl303xx_GetFollowUpFlag();
        clck->ptp.config.bmca.revertiveEn = zl303xx_GetBmcaRevertiveEn();
        clck->ptp.config.telecom.waitToRestoreSec = zl303xx_GetWaitToRestore();
        clck->ptp.config.telecom.qlHoldOffSec = zl303xx_GetQLHoldOff();
        clck->ptp.config.telecom.evtSquelchEn = zl303xx_GetEvtSquelchEn();

        clck->ptp.config.eventCallback = examplePtpEventHandler;
        /*
        clck->ptp.config.hwCmdFn = examplePtpHwCmdHandler;
        clck->ptp.config.extData = zl303xx_Params0;
        */
    }
#endif


    /* Create Clock */
    if (status == ZL303XX_OK)
    {
#if defined _ZL303XX_ZLE30360_BOARD
        status = exampleAppStartClock(&zlExampleApp, 0);
        if (status == ZL303XX_OK) status = exampleAppStartClock(&zlExampleApp, 1);
        if (status == ZL303XX_OK) status = exampleAppStartClock(&zlExampleApp, 2);
        if (status == ZL303XX_OK) status = exampleAppStartClock(&zlExampleApp, 3);
#elif defined _ZL303XX_ZLE1588_BOARD

        if (status == ZL303XX_OK)
        {
           status = exampleAppStartClock(&zlExampleApp, 0);
        }
        if(( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE )
           )
        {
        if (status == ZL303XX_OK) status = exampleAppStartClock(&zlExampleApp, 1);
        }
        if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
        {
        if (status == ZL303XX_OK) status = exampleAppStartClock(&zlExampleApp, 1);
        }
        if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
        {
            if (status == ZL303XX_OK) status = exampleAppStartClock(&zlExampleApp, 1);
            if (status == ZL303XX_OK) status = exampleAppStartClock(&zlExampleApp, 2);
            if (status == ZL303XX_OK) status = exampleAppStartClock(&zlExampleApp, 3);
        }
        if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
        {
            if (status == ZL303XX_OK) status = exampleAppStartClock(&zlExampleApp, 1);
            if (status == ZL303XX_OK) status = exampleAppStartClock(&zlExampleApp, 2);
            if (status == ZL303XX_OK) status = exampleAppStartClock(&zlExampleApp, 3);
        }
        if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
        {
            if (status == ZL303XX_OK) status = exampleAppStartClock(&zlExampleApp, 1);
        }
        if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
        {
            /* Create a zl303xx_ParamsS for APR. */
            if( TARGET_DPLL == 0 )
            {
                /* To test stepTime() on the GPs, we need to run APR on DPLL 0. But
                   the clock for DPLL has already been created above, so do not
                   try to create it again. */
            }
            else
            {
                if (status == ZL303XX_OK) status = exampleAppStartClock(&zlExampleApp, TARGET_DPLL);
            }
            /* Create a zl303xx_ParamsS for the NCO-assist DPLL. The NCO-assist DPLL is DPLL 3 for ZL30771, XL30772, and ZL30773 */
            if (status == ZL303XX_OK) status = exampleAppStartClock(&zlExampleApp, NCO_ASSIST_DPLL);
        }
#else
/* #warning MSCC: No supported board specified. */
#endif
#if defined ZLS30771_INCLUDED
      if (status == ZL303XX_OK)
      {
         if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
         {
            /* For the ZL30770 series, an extra DPLL is used to enhance
               ACI_BC_FULL_ON_PATH_PHASE_SYNCE. We need to maintain a mapping
               between APR's DPLL and the NCO-assist DPLL.

               TARGET_DPLL is APR's DPLL.
               The NCO-assist DPLL for ZL30771, ZL30772, and ZL30773 is DPLL 3.
            */
            zl303xx_Dpll77xSetNCOAssistParamsSAssociation(zlExampleApp.clock[TARGET_DPLL].apr.cguId,
                                           zlExampleApp.clock[NCO_ASSIST_DPLL].apr.cguId);
         }
      }
#endif

        if (status != ZL303XX_OK)
            ZL303XX_TRACE_ALWAYS("exampleSetup(): exampleAppStartClock() failure = %d.", status, 0,0,0,0,0);
    }

#if defined ZLS30390_INCLUDED
    if (status == ZL303XX_OK)
    {
        /* Save the clock handle */
        PtpClockHandle = 0;

        /* Associate flows = servers = streams = sockets */
        zlExampleApp.srvId = settingStreamHandle;
    }

    /* Create PTP Port on existing clock*/
    if (status == ZL303XX_OK)
    {
        status = examplePtpPortAdd(0, (Uint8T const*)zl303xx_GetSrcIpAddrStr());
    }
#endif

    if (status != ZL303XX_OK)
        ZL303XX_TRACE_ALWAYS("exampleSetup() status = %lu", (Uint32T)status, 0,0,0,0,0);

    return status;
}


/**
    An example that uses external configuration globals to initialize port and stream settings. It is used
    after calling exampleSetup(). This function may be called several times to initialize multiple
    streams.
*******************************************************************************/
zlStatusE exampleConfig(zl303xx_BooleanE isUnicast, zl303xx_BooleanE slaveCapable, zl303xx_PtpVersionE ptpVersion);
zlStatusE exampleConfig(zl303xx_BooleanE isUnicast, zl303xx_BooleanE slaveCapable, zl303xx_PtpVersionE ptpVersion)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T indx = 0;
    exampleAppStreamS *stream;
    zl303xx_BooleanE bStartStream = ZL303XX_TRUE;

#if defined ZLS30390_INCLUDED
    zl303xx_PtpPortCreateS portConfig;
    zl303xx_ProtocolTypesE protocol = zl303xx_GetProtocolType();

    memset(&portConfig, 0, sizeof(portConfig));
#endif

    ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 2,
                "exampleConfig(): isUnicast=%d, slaveCapable=%d ptpVersion=%d", isUnicast, slaveCapable, ptpVersion, 0,0,0);
    /* Find the first available stream */
    if (status == ZL303XX_OK)
    {
        for (indx = 0; indx < zlExampleApp.streamCount; indx++)
        {
            stream = &zlExampleApp.stream[indx];
            if (stream->started == ZL303XX_FALSE)
                break;
        }

        if (indx >= zlExampleApp.streamCount)
        {
            /* No free streams found */
            status = ZL303XX_INIT_ERROR;
            ZL303XX_TRACE_ALWAYS("exampleConfig(): indx=%d > streamCount=%d", indx, zlExampleApp.streamCount, 0,0,0,0);
            ZL303XX_ERROR_NOTIFY("No more streams available");
        }
    }

    /* Initialize stream using defaults from global accessors */
#ifdef APR_INCLUDED
    if (status == ZL303XX_OK)
    {
#if defined ZLS30390_INCLUDED
        zl303xx_AprSetPktRate(zl303xx_GetPps(settingStreamHandle), ZL303XX_TRUE);
        zl303xx_AprSetPktRate(zl303xx_GetDelayReqPps(settingStreamHandle), ZL303XX_FALSE);
        stream->apr.server.serverId = settingStreamHandle;                       /* Server flow */
#else
/* #warning MSCC: Hard-coded APR packet rates and serverId */
        zl303xx_AprSetPktRate(64, ZL303XX_TRUE);
        zl303xx_AprSetPktRate(16, ZL303XX_FALSE);
        stream->apr.server.serverId = 0;                       /* Server flow */
#endif
        stream->apr.server.algTypeMode = zl303xx_AprGetAlgTypeMode();
        stream->apr.server.bUseType2BPLL = zl303xx_AprGetUseType2BPLL();
        stream->apr.server.Type2BPLLFastLock =zl303xx_AprGetUseType2BPLLFastLock();
        stream->apr.server.oscillatorFilterType = zl303xx_AprGetOscillatorFilterType();
        stream->apr.server.osciHoldoverStability = zl303xx_AprGetHoldoverStability();
        stream->apr.server.sModeTimeout = zl303xx_AprGetSModeTimeout();
        stream->apr.server.sModeAgeout = zl303xx_AprGetSModeAgeOut();
        stream->apr.server.filterType = zl303xx_AprGetFilterType();
        stream->apr.server.fwdPacketRateType = zl303xx_AprGetPktRate(ZL303XX_TRUE);
        stream->apr.server.revPacketRateType = zl303xx_AprGetPktRate(ZL303XX_FALSE);
        stream->apr.server.thresholdForFlagV = zl303xx_AprGetCustomerThresholdForFlagV();
        stream->apr.server.bXdslHpFlag = zl303xx_AprGetXdslHpFlag();
        stream->apr.server.tsFormat = zl303xx_AprGetTsFormat();
        stream->apr.server.b32BitTs = zl303xx_AprGet32BitTsFlag();
        stream->apr.server.bUseRevPath = zl303xx_AprGetUseReversePath();
        stream->apr.server.bHybridMode = zl303xx_AprGetHybridServerFlag();
        stream->apr.server.packetDiscardDurationInSec = zl303xx_AprGetPacketDiscardDurationInSecFlag();
        stream->apr.server.pullInRange = zl303xx_AprGetPullInRange();
    }

    if (status == ZL303XX_OK)
    {
        /* Apply xparam overrides */
        status = exampleAprOverrideXParamApply(&stream->apr.server);
    }
#endif

#if defined ZLS30390_INCLUDED
    if (status == ZL303XX_OK)
    {
        status = zl303xx_PtpPortConfigGet(PtpClockPortHandle, &portConfig);
    }

    if (status == ZL303XX_OK)
    {
        if (isUnicast)
        {
            if (slaveCapable || (!slaveCapable && !zl303xx_GetUseUniNegFlag()))  /* UniNeg Masters don't require a dest. */
            {
                setExampleUnicastIpDest((char*)zl303xx_GetDestIpAddrStr());    /* Uni Dest addr */
                if (zl303xx_IsIPv6SktProtocol(protocol))
                    setExampleUnicastIPv6Dest(indx, (Uint8T*)exampleUniPtpStream.destIpAddr);
                else
                    status = examplePtpDestIpv4Set(exampleUniPtpStream.destIpAddr, ZL303XX_FALSE, &stream->ptp);

            }
        }
        else
        {
            setExampleMulticastIpDest((char*)zl303xx_GetDestIpAddrStr());    /* Multi Dest addr */
            if (zl303xx_IsIPv6SktProtocol(protocol))
                setExampleMulticastIPv6Dest(indx, (Uint8T*)exampleMultiPtpStream.destIpAddr);
            else
                status = examplePtpDestIpv4Set(exampleMultiPtpStream.destIpAddr, ZL303XX_FALSE, &stream->ptp);

        }

    }

    if (status == ZL303XX_OK)
    {
        stream->ptp.config.logAnnounceInterval        = log2PacketsPerSecond(zl303xx_GetAnnouncePps(settingStreamHandle));
        stream->ptp.config.logSyncInterval            = log2PacketsPerSecond(zl303xx_GetPps(settingStreamHandle));
        stream->ptp.config.logMinDelayReqInterval     = log2PacketsPerSecond(zl303xx_GetDelayReqPps(settingStreamHandle));

        /* Set the default mode for this timing packet stream */    /* zl303xx_GetBmcFlag() can override */
        stream->ptp.config.mode = ((slaveCapable) ? ZL303XX_PTP_STREAM_SLAVE_ONLY
                                   : ZL303XX_PTP_STREAM_MASTER_ONLY);
        if (zl303xx_GetBmcFlag())   /* Over-ride mode when a Best Master Clock */
        {
            stream->ptp.config.mode = ZL303XX_PTP_STREAM_USE_BMC;
            ZL303XX_TRACE_ALWAYS("BMC mode set.", 0,0,0,0,0,0);
        }

        /* PTPv2 Uni-negotiated */
        if ((portConfig.unicastNegotiation.enabled == ZL303XX_TRUE) &&
            (isUnicast == ZL303XX_TRUE) &&
            (ptpVersion == ZL303XX_PTP_VERSION_2))
        {

            /* Associate stream with specific clock/port pair */
            stream->ptp.config.portHandle = PtpClockPortHandle;

            /* If this is a Slave in Unicast Negotiation mode, it needs to start
                sending grant requests. */
            if (slaveCapable ==ZL303XX_TRUE)
            {
                stream->ptp.config.unicast                    = ZL303XX_TRUE;
                stream->ptp.config.portHandle                 = PtpClockPortHandle;
                stream->ptp.config.announceReceiptTimeout     = zl303xx_GetAnnounceReceiptTimeout();
                stream->ptp.config.unicastNegotiationDuration = zl303xx_GetUnicastNegDuration();
                stream->ptp.config.unicastNegotiationRenew    = zl303xx_GetUnicastNegRenew();

            }
            else
            {   /* MASTER */
                /* This is a Unicast negotiation master */
                /* We do not have to do anything. Once the slave requests a stream,
                * the master will set it up if the grant request is accepted. */
                bStartStream = ZL303XX_FALSE;

                stream->ptp.config.unicast                 = isUnicast;
            }
        }
        else if (ptpVersion == ZL303XX_PTP_VERSION_2)
        {
            /* This is V2 without Negotiation */

            /* Associate stream with specific clock/port pair */
            stream->ptp.config.portHandle = PtpClockPortHandle;

            /* Set the unicast/multicast attribute for the created stream */
            stream->ptp.config.unicast = isUnicast;

            /* Use the default initializers for the following values:
               stream->ptp.config.alternateMaster;
               stream->ptp.config.logAnnounceInterval;
               stream->ptp.config.announceReceiptTimeout;
               stream->ptp.config.unicastNegotiationDuration; */

            stream->ptp.config.announceReceiptTimeout = ZL303XX_PTP_DEFAULT_STREAM_ANNC_TIMEOUT;
            stream->ptp.config.unicastNegotiationDuration = ZL303XX_PTP_DEFAULT_STREAM_UNI_NEG_DUR;
        }
    }
#endif

    /* Create Stream */
    if (status == ZL303XX_OK && bStartStream == ZL303XX_TRUE)
    {
#if defined ZLS30390_INCLUDED
        zlExampleApp.srvId = settingStreamHandle;
#endif
        status = exampleAppStartStream(&zlExampleApp, indx);
    }

#if defined ZLS30390_INCLUDED
    /* If Telecom Profile then request Announce, Sync, and Delay_Resp service from master. */
    if (status == ZL303XX_OK && zl303xx_GetPtpProfile() == ZL303XX_PTP_PROFILE_TELECOM_G8265_1
          && bStartStream == ZL303XX_TRUE)
    {
        status = examplePtpContractsRequest(&zlExampleApp.stream[stream->ptp.handle].ptp,
                                            (1 << ZL303XX_MSG_ID_ANNOUNCE) | (1 << ZL303XX_MSG_ID_SYNC) | (1 << ZL303XX_MSG_ID_DELAY_RESP));
    }
#endif

    if (status != ZL303XX_OK)
        ZL303XX_TRACE_ALWAYS("exampleConfig() status = %lu", (Uint32T)status, 0,0,0,0,0);

    return status;
}

zlStatusE examplePtpShutdown(void);
zlStatusE examplePtpShutdown(void)
{
    return exampleShutdown();
}

#endif  /* 390_INCLUDED */

#if defined APR_INCLUDED
/**

  Function Name:
   exampleConfigure1Hz

  Details:

   Configures 1Hz for a stream.

   Either forward or reverse stream is modified by this routine.

   Basic parameters:
      - enabled
      - serverID
      - realignmentType
      - realignmentInterval
      - bForwardPath
      - packetRate

   Use exampleAprOverride1HzXParam() before this function to control other
   xParams.

   x-Parameters:

      1Hz has a number of x-parameters (parameters that normally do not
      need to be changed from the default but can be changed in certain
      scenarios)

      par.xParams.p[2]:

         This is the maximum number of checked packets collected before
         evaluating 1Hz alignment. This is normally equal to the packet rate
         * realignment interval.

         NOTE: this is the MAXIMUM number of packets collected; under normal
               conditions, fewer packets are needed to evaluate 1Hz alignment.

         NOTE: this value is overwritten if the packet rate changes
               on-the-fly

      par.xParams.p[5]:

         By default, during the very first 10-25 minutes, the realignment
         interval is decreased so that 1Hz can match a relatively slow moving
         APR. However, in the boundary clock scenario, APR moves quickly so
         this feature is unnecessary and should be disabled.

  Notes:
    See exampleConfigure1HzWithGlobals()

  Parameters:
   [in]    enabled i.e. TRUE or FALSE
   [in]    serverID - the server ID
   [in]    realignmentType - the realignment type. One of:
                  1HZ_REALIGNMENT_ON_GOOD_LOCK
                  1HZ_REALIGNMENT_PERIODIC
                  1HZ_REALIGNMENT_MANUAL
                  1HZ_REALIGNMENT_PERIODIC_U
                  1HZ_REALIGNMENT_PER_PACKET
   [in]    realignmentInterval - the realignment interval.
                  default: 120 (seconds)
   [in]    bForwardPath - TRUE if this is a forward stream
   [in]    packetRate - the packet rate
   [in]    startupModifier - see above par.xParams.p[5]

  Return Value:
   zlStatusE

 *****************************************************************************/
zlStatusE exampleConfigure1Hz
(
void *hwParams,
zl303xx_BooleanE enabled,
Uint32T serverID,
zl303xx_Apr1HzRealignmentTypeE realignmentType,
Uint32T realignmentInterval,
zl303xx_BooleanE bForwardPath,
Sint32T packetRate,
zl303xx_BooleanE startupModifier
)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_AprConfigure1HzS par;

    if ( status == ZL303XX_OK )
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }

    /* Get current settings */
    if (status == ZL303XX_OK && (bForwardPath || zl303xx_AprGetUseReversePath()))
    {
        status = zl303xx_AprGetServer1HzConfigData(hwParams, serverID, bForwardPath, &par);
        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("zl303xx_AprGetServer1HzConfigData device %p serverId %d fwd %d failure=%d", hwParams, serverID, bForwardPath, status,0,0);
        }

        if (status == ZL303XX_OK)
        {
            par.disabled = !enabled;
            par.realignmentType = realignmentType;
            par.realignmentInterval = realignmentInterval;
            par.xParams.p[2] = realignmentInterval * packetRate;
            par.xParams.p[5] = startupModifier;
            if ( realignmentInterval < 31 )
            {
                par.xParams.p[12] = 10;
            }
            else
            {
                par.xParams.p[12] = realignmentInterval - 30;
            }

            /* Apply xParam overrides (ACI settings, etc) */
            /* Note: These can override settings made above */
            if ((status = exampleAprOverride1HzXParamApply(bForwardPath, &par)) != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("exampleConfigure1Hz failed exampleAprOverride1HzXParamApply, status %d", status, 0,0,0,0,0);
            }

            if ((status = zl303xx_AprConfigServer1Hz(hwParams, serverID, bForwardPath, &par)) != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("exampleConfigure1Hz failed zl303xx_AprConfigServer1Hz=%d", status, 0,0,0,0,0);
            }
        }
    }

    if (status != ZL303XX_OK )
    {
        ZL303XX_TRACE_ALWAYS("exampleConfigure1Hz: Failed=%d (bFwd: %u)", status, bForwardPath, 0,0,0,0);
    }
    else
    {
        ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 2, "exampleConfigure1Hz: serverId=%d (RT %d, RI %d, FW %d, PR %d, SM %d)",
                           serverID,
                           realignmentType,
                           realignmentInterval,
                           bForwardPath,
                           packetRate,
                           startupModifier);
    }

    return( status );
}

/**

  Function Name:
    exampleConfigure1HzWithGlobals

  Details:
    Example code to configure APR's 1Hz module for a server clock on the
    specified device using previously configured globals.
    Note: 1Hz only can be configured after the device and the server clock
          have been registered to APR (after zl303xx_AprAddServer()).

  Parameters:
   [in]   hwParams       Pointer to the device instance
   [in]   serverId       Server clock ID.


  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE exampleConfigure1HzWithGlobals(void *hwParams, Uint16T serverId)
{
    zlStatusE status = ZL303XX_OK;

    if (hwParams == NULL)
    {
        status = ZL303XX_INVALID_POINTER;
    }

    if (status == ZL303XX_OK && zl303xx_AprIsAprRunning() != ZL303XX_TRUE)
    {
        status = ZL303XX_NOT_RUNNING;
    }

    /* APR-1Hz settings */
    if (status == ZL303XX_OK && !(zl303xx_AprGetTsFormat() == ZL303XX_APR_TS_RTP))
    {
        /* Forward path */
        status = exampleConfigure1Hz(hwParams,
                                     zl303xx_AprGet1HzEnabled(),
                                     serverId,
                                     zl303xx_AprGet1HzRealignmentType(),
                                     zl303xx_AprGet1HzRealignmentInterval(),
                                     ZL303XX_TRUE,
                                     zl303xx_AprGetPktRate(ZL303XX_TRUE),
                                     zl303xx_AprGet1HzStartupTimeInit());

        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("Error trying to initialize 1Hz config, status %d", status, 0,0,0,0,0);
        }
    }

    if (status == ZL303XX_OK && zl303xx_AprGetUseReversePath())
    {
        /* Reverse path */
        status = exampleConfigure1Hz(hwParams,
                                     zl303xx_AprGet1HzEnabled(),
                                     serverId,
                                     zl303xx_AprGet1HzRealignmentType(),
                                     zl303xx_AprGet1HzRealignmentInterval(),
                                     ZL303XX_FALSE,
                                     zl303xx_AprGetPktRate(ZL303XX_FALSE),
                                     zl303xx_AprGet1HzStartupTimeInit());

        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("Error trying to initialize 1Hz config, status %d", status, 0,0,0,0,0);
        }
    }

    return status;
}

/**

  Function Name:
*   exampleAprModeSwitching

  Details:
*   Reconfigures system to switch from any BC mode to the other.

  Parameters:
   [in]   hwParams       Pointer to the device instance
   [in]   serverId       Server clock ID.


  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE exampleAprModeSwitching(void *hwParams, Uint16T serverId, exampleAprConfigIdentifiersE configId)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T adjSize1HzPSL[ZL303XX_MAX_NUM_PSL_LIMITS];
    Uint32T adjSize1HzFCL[ZL303XX_MAX_NUM_FCL_LIMITS];
    Uint32T adjSize1HzAdjScaling[ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS];
    Uint32T PSL_1Hz[ZL303XX_MAX_NUM_PSL_LIMITS];
    Uint32T FCL_1Hz[ZL303XX_MAX_NUM_FCL_LIMITS];
    Uint32T adjScaling_1Hz[ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS];
    Uint32T j;
    zl303xx_AprDeviceRefModeE device_mode;

     ZL303XX_TRACE_ALWAYS("exampleBCModeSwitching: Changing mode to ACI%d", configId, 0,0,0,0,0);

    if (hwParams == NULL)
    {
       status = ZL303XX_INVALID_POINTER;
    }

    if (status == ZL303XX_OK && zl303xx_AprIsAprRunning() != ZL303XX_TRUE)
    {
       status = ZL303XX_NOT_RUNNING;
    }

    if (status == ZL303XX_OK)
    {
        /* Make sure these are set before 1Hz reconfigure */
            status = zl303xx_AprSetPFAdjustFreqMinPhase(0);
            status |= zl303xx_SetAdjustTimeMinThreshold(0);
        /* This function is intended to support the switching between ACI8 and ACI9.
           It may work for other ACIs, but they have not been tested yet.             */
        /* if ( (configId < ACI_BC_FULL_ON_PATH_PHASE) || (configId > ACI_BC_FULL_ON_PATH_PHASE_SYNCE) )
        {
            status = ZL303XX_PARAMETER_INVALID;
            ZL303XX_TRACE_ALWAYS("exampleBCModeSwitching: ACI %d is not supported by this function; status=%d.", configId, status, 0,0,0,0);
        }*/
    }

    /* Step 1: disable 1Hz temp. */
    if (status == ZL303XX_OK)
    {
        status = exampleAprSet1HzEnabled(hwParams, ZL303XX_FALSE);
    }
    else
    {
        ZL303XX_TRACE_ALWAYS("exampleBCModeSwitching: Failed to call exampleAprSet1HzEnabled(); status=%d.", status, 0,0,0,0,0);
    }

    /* Step 2: change 1Hz settings, including the flag of enabling 1Hz (just a flag, not really enable 1Hz yet) */
    if (status == ZL303XX_OK)
    {
        status = exampleAprSetConfigParameters(configId);
    }
    else
    {
        ZL303XX_TRACE_ALWAYS("exampleBCModeSwitching: Failed to call exampleAprSetConfigParameters(%d); status=%d.", configId, status, 0,0,0,0);
    }

    /* Step 3: make the new PSL, FCL to take effect */
    if (status == ZL303XX_OK)
    {
        for(j = 0; j < ZL303XX_MAX_NUM_PSL_LIMITS; j++ )
        {
            zl303xx_AprGetPSL(j, &(adjSize1HzPSL[j]), &(PSL_1Hz[j]) );
        }
        for (j = 0; j < ZL303XX_MAX_NUM_FCL_LIMITS; j++ )
        {
            zl303xx_AprGetFCL(j, &(adjSize1HzFCL[j]), &(FCL_1Hz[j]) );
        }
        for (j = 0; j < ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS; j++ )
        {
            zl303xx_AprGetAdjScaling(j, &(adjSize1HzAdjScaling[j]), &(adjScaling_1Hz[j]) );
        }

        status = exampleAprReConfigurePSLFCL(hwParams,
                                             zl303xx_AprGetPCLFCLStaticOffset(),
                                             zl303xx_AprGetAPRFrequencyLockedPhaseSlopeLimit(),
                                             zl303xx_AprGetAPRFrequencyNotLockedPhaseSlopeLimit(),
                                             zl303xx_AprGetAPRFrequencyFastPhaseSlopeLimit(),
                                             zl303xx_AprGetAPRFrequencyLockedFrequencyChangeLimit(),
                                             zl303xx_AprGetAPRFrequencyNotLockedFrequencyChangeLimit(),
                                             zl303xx_AprGetAPRFrequencyFastFrequencyChangeLimit(),
                                             adjSize1HzPSL,
                                             PSL_1Hz,
                                             adjSize1HzFCL,
                                             FCL_1Hz,
                                             adjSize1HzAdjScaling,
                                             adjScaling_1Hz);

        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("exampleBCModeSwitching: Failed to call exampleAprReConfigurePSLFCL(), status %d", status, 0,0,0,0,0);
        }
    }

    /* Step 4: make the new settings to take effect */

    /* APR-1Hz settings */
    if (status == ZL303XX_OK && !(zl303xx_AprGetTsFormat() == ZL303XX_APR_TS_RTP))
    {
        /* Forward path */
        status = exampleConfigure1Hz(hwParams,
                                      zl303xx_AprGet1HzEnabled(),
                                      serverId,
                                      zl303xx_AprGet1HzRealignmentType(),
                                      zl303xx_AprGet1HzRealignmentInterval(),
                                      ZL303XX_TRUE,
                                      zl303xx_AprGetPktRate(ZL303XX_TRUE),
                                      zl303xx_AprGet1HzStartupTimeInit());

        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("Error trying to initialize 1Hz config, status %d", status, 0,0,0,0,0);
        }
    }

    if (status == ZL303XX_OK && zl303xx_AprGetUseReversePath())
    {
        /* Reverse path */
        status = exampleConfigure1Hz(hwParams,
                                      zl303xx_AprGet1HzEnabled(),
                                      serverId,
                                      zl303xx_AprGet1HzRealignmentType(),
                                      zl303xx_AprGet1HzRealignmentInterval(),
                                      ZL303XX_FALSE,
                                      zl303xx_AprGetPktRate(ZL303XX_FALSE),
                                      zl303xx_AprGet1HzStartupTimeInit());

        if(status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("Error trying to initialize 1Hz config, status %d", status, 0,0,0,0,0);
        }
    }

    /* get APR device mode from global parameter */
    device_mode = zl303xx_AprGetDeviceOptMode();

    if ((configId == ACI_ELEC_FREQ) || (configId == ACI_ELEC_PHASE))  /* Use this for switching to electric mode */
    {
          status =zl303xx_AprSetActiveElecRef(hwParams, serverId);
    }
    else    /* Use this to switch to packet or hybrid*/
    {
         status = zl303xx_AprSetServerMode(hwParams, serverId, device_mode);
    }

    if (status == ZL303XX_OK)
    {
        status = zl303xx_AprSetDevice1HzEnabled(hwParams, ZL303XX_TRUE);
    }

    return status;
}


/**
  Function Name:
    exampleReConfigure1HzSettings

  Details:
    Configure 1Hz setting for ACI8 (i.e, ACI_BC_FULL_ON_PATH_PHASE) or
    ACI9 (i.e., ACI_BC_FULL_ON_PATH_PHASE_SYNCE)

  Parameters:
   [in]   hwParams       Pointer to the device instance
   [in]   serverId       Server clock ID.


  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE exampleReConfigure1HzSettings(void *hwParams, Uint16T serverId, exampleAprConfigIdentifiersE configId)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T adjSize1HzPSL[ZL303XX_MAX_NUM_PSL_LIMITS];
    Uint32T adjSize1HzFCL[ZL303XX_MAX_NUM_FCL_LIMITS];
    Uint32T adjSize1HzAdjScaling[ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS];
    Uint32T PSL_1Hz[ZL303XX_MAX_NUM_PSL_LIMITS];
    Uint32T FCL_1Hz[ZL303XX_MAX_NUM_FCL_LIMITS];
    Uint32T adjScaling_1Hz[ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS];
    Uint32T j;
    zl303xx_AprDeviceRefModeE device_mode;

     ZL303XX_TRACE_ALWAYS("exampleReConfigure1HzSettings: Changing mode to ACI%d", configId, 0,0,0,0,0);

    if (hwParams == NULL)
    {
       status = ZL303XX_INVALID_POINTER;
    }

    if (status == ZL303XX_OK && zl303xx_AprIsAprRunning() != ZL303XX_TRUE)
    {
       status = ZL303XX_NOT_RUNNING;
    }

    /* This function is intended to support the switching between ACI8 and ACI9.
       It may work for other ACIs, but they have not been tested yet.             */
    if ( (configId < ACI_BC_FULL_ON_PATH_PHASE) || (configId > ACI_BC_FULL_ON_PATH_PHASE_SYNCE) )
    {
        status = ZL303XX_PARAMETER_INVALID;
        ZL303XX_TRACE_ALWAYS("exampleReConfigure1HzSettings: ACI %d is not supported by this function; status=%d.", configId, status, 0,0,0,0);
    }

    /* Step 1: disable 1Hz */
    if (status == ZL303XX_OK)
    {
        status = exampleAprSet1HzEnabled(hwParams, ZL303XX_FALSE);
    }
    else
    {
        ZL303XX_TRACE_ALWAYS("exampleReConfigure1HzSettings: Failed to call exampleAprSet1HzEnabled(); status=%d.", status, 0,0,0,0,0);
    }

    /* Step 2: change 1Hz settings, including the flag of enabling 1Hz (just a flag, not really enable 1Hz yet) */
    if (status == ZL303XX_OK)
    {
        status = exampleAprSetConfigParameters(configId);
    }
    else
    {
        ZL303XX_TRACE_ALWAYS("exampleReConfigure1HzSettings: Failed to call exampleAprSetConfigParameters(%d); status=%d.", configId, status, 0,0,0,0);
    }

    /* Step 3: make the new PSL, FCL to take effect */
    if (status == ZL303XX_OK)
    {
        for(j = 0; j < ZL303XX_MAX_NUM_PSL_LIMITS; j++ )
        {
            zl303xx_AprGetPSL(j, &(adjSize1HzPSL[j]), &(PSL_1Hz[j]) );
        }
        for (j = 0; j < ZL303XX_MAX_NUM_FCL_LIMITS; j++ )
        {
            zl303xx_AprGetFCL(j, &(adjSize1HzFCL[j]), &(FCL_1Hz[j]) );
        }
        for (j = 0; j < ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS; j++ )
        {
            zl303xx_AprGetAdjScaling(j, &(adjSize1HzAdjScaling[j]), &(adjScaling_1Hz[j]) );
        }

        status = exampleAprReConfigurePSLFCL(hwParams,
                                             zl303xx_AprGetPCLFCLStaticOffset(),
                                             zl303xx_AprGetAPRFrequencyLockedPhaseSlopeLimit(),
                                             zl303xx_AprGetAPRFrequencyNotLockedPhaseSlopeLimit(),
                                             zl303xx_AprGetAPRFrequencyFastPhaseSlopeLimit(),
                                             zl303xx_AprGetAPRFrequencyLockedFrequencyChangeLimit(),
                                             zl303xx_AprGetAPRFrequencyNotLockedFrequencyChangeLimit(),
                                             zl303xx_AprGetAPRFrequencyFastFrequencyChangeLimit(),
                                             adjSize1HzPSL,
                                             PSL_1Hz,
                                             adjSize1HzFCL,
                                             FCL_1Hz,
                                             adjSize1HzAdjScaling,
                                             adjScaling_1Hz);

        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("exampleReConfigure1HzSettings: Failed to call exampleAprReConfigurePSLFCL(), status %d", status, 0,0,0,0,0);
        }
    }

    /* Step 4: make the new settings to take effect */

    /* APR-1Hz settings */
    if (status == ZL303XX_OK && !(zl303xx_AprGetTsFormat() == ZL303XX_APR_TS_RTP))
    {
        /* Forward path */
        status = exampleConfigure1Hz(hwParams,
                                      zl303xx_AprGet1HzEnabled(),
                                      serverId,
                                      zl303xx_AprGet1HzRealignmentType(),
                                      zl303xx_AprGet1HzRealignmentInterval(),
                                      ZL303XX_TRUE,
                                      zl303xx_AprGetPktRate(ZL303XX_TRUE),
                                      zl303xx_AprGet1HzStartupTimeInit());

        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("Error trying to initialize 1Hz config, status %d", status, 0,0,0,0,0);
        }
    }

    if (status == ZL303XX_OK && zl303xx_AprGetUseReversePath())
    {
        /* Reverse path */
        status = exampleConfigure1Hz(hwParams,
                                      zl303xx_AprGet1HzEnabled(),
                                      serverId,
                                      zl303xx_AprGet1HzRealignmentType(),
                                      zl303xx_AprGet1HzRealignmentInterval(),
                                      ZL303XX_FALSE,
                                      zl303xx_AprGetPktRate(ZL303XX_FALSE),
                                      zl303xx_AprGet1HzStartupTimeInit());

        if(status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("Error trying to initialize 1Hz config, status %d", status, 0,0,0,0,0);
        }
    }

    /* get APR device mode from global parameter */
    device_mode = zl303xx_AprGetDeviceOptMode();
    status = zl303xx_AprSetServerMode(hwParams, serverId, device_mode);

    return status;
}


/* exampleAprForceActiveRef */
/*
   Example code to manually force APR to a specific reference (either electrical or ToP).

   Note: if ZLS30390 (Microsemi's PTP stack) is included, then this routine
         will not function as expected.

*******************************************************************************/
zlStatusE exampleAprForceActiveRef(
      void *hwParams,
      zl303xx_AprDeviceRefModeE userMode,
      Uint16T serverOrRefId)
{
    zlStatusE status = ZL303XX_OK;
    char fnName[] = "exampleAprForceActiveRef";

    zl303xx_AprDeviceRefModeE currDevMode;
    Uint16T currRef = (Uint16T)(-1);
    zl303xx_AprServerRefModeE currModeOfNewServer;

    /* Verify the input parameters. */
    if ((status = ZL303XX_CHECK_POINTER(hwParams)) != ZL303XX_OK)
    {
        ZL303XX_TRACE_ALWAYS("%s: Invalid device pointer (%p)", fnName, hwParams, 0,0,0,0);
    }
    else if (userMode >= ZL303XX_NOT_SUPPORT_DEV_REF_MODE)
    {
        ZL303XX_TRACE_ALWAYS("%s: Invalid userMode requested (%d)", fnName, userMode, 0,0,0,0);
        status = ZL303XX_UNSUPPORTED_OPERATION;
    }

    /* Get the current device mode. */
    else if ((status = zl303xx_AprGetDeviceRefMode (hwParams, &currDevMode)) != ZL303XX_OK)
    {
        ZL303XX_TRACE_ALWAYS("%s: zl303xx_AprGetDeviceRefMode() failed=%d", fnName, status, 0,0,0,0);
    }

    /* Get the device's current active reference.
     * If the device is ELECTRIC, the default (above) will be used. */
    else if (currDevMode < ZL303XX_ELECTRIC_MODE)
    {
        if (serverOrRefId == (Uint16T)(-1))
        {
            ZL303XX_TRACE_ALWAYS("%s: Switch to protocol mode with invalid serverID=%d", fnName, serverOrRefId, 0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
        }
        else if ((status = zl303xx_AprGetDeviceCurrActiveRef (hwParams, &currRef)) != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("%s: zl303xx_AprGetDeviceCurrActiveRef() failed=%d", fnName, status, 0,0,0,0);
        }
    }

    /* Perform the switching actions.
     * 1) Do any required Mode changes first (if necessary).
     * 2) Switch to the requested server. */

    /* No matter what the current mode of the device, if switching to a PACKET
     * or HYBRID mode server, make sure the mode for that server is correct prior
     * to switching. If the requested server is already active, then this becomes
     * a mode change for the server.  */
    if (status == ZL303XX_OK)
    {
        if ((userMode == ZL303XX_PACKET_MODE) || (userMode == ZL303XX_HYBRID_MODE))
        {
            /* If going to a Packet or Hybrid server, the current mode of the requested server will be needed.    */
            zl303xx_BooleanE bNewRefHybridMode;

            /* Get the mode of the new server. */
            if ((status = zl303xx_AprIsServerInHybridMode(hwParams, serverOrRefId, &bNewRefHybridMode)) != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("%s: zl303xx_AprIsServerInHybridMode() failed=%d", fnName, status, 0,0,0,0);
            }
            else
            {
                /* Map the Boolean to the Enum type. */
                currModeOfNewServer = ((bNewRefHybridMode == ZL303XX_TRUE) ? ZL303XX_HYBRID_MODE : ZL303XX_PACKET_MODE);

                /* Determine if the mode of the new server needs to be changed. */
                if (currModeOfNewServer != userMode)
                {
                    ZL303XX_TRACE_ALWAYS("%s: Changing the mode of %s serverId=%d to %s mode",
                                       fnName,
                                       __APR_DEVICE_MODE_TO_STR(currModeOfNewServer), serverOrRefId,
                                       __APR_DEVICE_MODE_TO_STR(userMode), 0,0);

                    if ((status = zl303xx_AprSetServerMode(hwParams, serverOrRefId, userMode)) != ZL303XX_OK)
                    {
                        ZL303XX_TRACE_ALWAYS("%s: zl303xx_AprSetServerMode() failed=%d", fnName, status, 0,0,0,0);
                    }
                }
            }
        }
    }

    /* Perform the switch to the required protocol or electrical reference. */
    if (status == ZL303XX_OK)
    {
        ZL303XX_TRACE_ALWAYS("%s: Changing from %s ref=%d to %s ref=%d",
                           fnName,
                           __APR_DEVICE_MODE_TO_STR(currDevMode), currRef,
                           __APR_DEVICE_MODE_TO_STR(userMode), serverOrRefId, 0);

        /* If switching to ELEC mode (from any mode), just use the electric switch command. */
        if (userMode == ZL303XX_ELECTRIC_MODE)
        {
            if ((status = zl303xx_AprSetActiveElecRef (hwParams, serverOrRefId)) != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("%s: zl303xx_AprSetActiveElecRef() failed=%d", fnName, status, 0,0,0,0);
            }
        }

        /* Switching to a PKT/HYD server, just use the protocol switch command. */
        else
        {
            if ((status = zl303xx_AprSetActiveRef (hwParams, serverOrRefId)) != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("%s: zl303xx_AprSetActiveRef() failed=%d", fnName, status, 0,0,0,0);
            }
        }
    }
    return status;
}

/* exampleAprForceHoldover */
/**
   Example code to manually force APR into/out of holdover.

*******************************************************************************/
zlStatusE exampleAprForceHoldover (void *hwParams, zl303xx_BooleanE bHoldover)
{
    zlStatusE status = ZL303XX_OK;

    status = ZL303XX_CHECK_POINTER(hwParams);

    if (status == ZL303XX_OK &&
        (status = zl303xx_AprSetHoldover (hwParams, bHoldover)) != ZL303XX_OK)
    {
        ZL303XX_TRACE_ALWAYS("exampleAprForceHoldover: zl303xx_AprSetHoldover() failed=%d", status, 0,0,0,0,0);
    }

    return status;
}


/**

  Function Name:
    exampleAprReConfigurePullInRange

  Details:
   Example code to set pull-in range.

  Parameters:
   [in]   hwParams       pointer to the device instance
   [in]   serverId       the server ID
   [in]   pullInRange    the pull-in range

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE exampleAprReConfigurePullInRange (void *hwParams,
                                            Uint32T serverId,
                                            zl303xx_AprPullInRangeE pullInRange)
{
    zlStatusE status = ZL303XX_OK;

    status = ZL303XX_CHECK_POINTER(hwParams);

    if (status == ZL303XX_OK &&
        (status = zl303xx_AprReconfigureServerPullInRange (hwParams, serverId, pullInRange)) != ZL303XX_OK)
    {
        ZL303XX_TRACE_ALWAYS("exampleAprReConfigurePullInRange: zl303xx_AprReconfigureServerPullInRange() failed=%d", status, 0,0,0,0,0);
    }

    return status;
}


/**

  Function Name:
    exampleAprReConfigurePSLFCL

  Details:
   Example code to set PSL and FCL values.

   This routine allows the user to change PSL and FCL limits at runtime.

   When 1Hz makes an adjustment, the adjustment size is compared against each
   element in adjSize1HzPSL[]. When the smallest element that is still
   larger than the adjustment is found, the corresponding PSL value in
   PSL_1Hz[] is used.

   The defaults:
      defaultAdjSize1HzPSL[0] = 1000,   defaultPSL_1Hz[0] = 4
      defaultAdjSize1HzPSL[1] = 4000,   defaultPSL_1Hz[1] = APR_PSL_885_NS
      defaultAdjSize1HzPSL[2] = 10000,  defaultPSL_1Hz[2] = 4000
      defaultAdjSize1HzPSL[3] = 0,      defaultPSL_1Hz[3] = 0
      defaultAdjSize1HzPSL[4] = 0,      defaultPSL_1Hz[4] = 0

   eg. If an adjustment is 1200ns, then element [1] will be chosen
       (1000 < 1200 < 4000) and the PSL used will be 885ns/s.

   With these defaults, adjustments where adjustments < 1000 are corrected
   very slowly to comply with performance requirements, adjustment where
   4000 < adjustments < 1000 are corrected a more aggressive rate, and
   very large adjustments where 10000 < adjustments are considered fast
   lock and are adjusted very aggressively.

  Parameters:
   [in]   hwParams         pointer to the device instance
   [in]   APRFrequencyLockedPhaseSlopeLimit - APR's PSL when NOT in fast lock
                              units: ns/s
   [in]   APRFrequencyNotLockedPhaseSlopeLimit - APR's PSL when NOT in fast lock
                              units: ns/s
   [in]   APRFrequencyFastPhaseSlopeLimit - APR's PSL while in fast lock
                              units: ns/s
   [in]   APRFrequencyLockedFrequencyChangeLimit - APR's FCL when NOT in fast lock
                              units: ppt/s
   [in]   APRFrequencyNotLockedFrequencyChangeLimit - APR's FCL when NOT in fast lock
                              units: ppt/s
   [in]   APRFrequencyFastFrequencyChangeLimit - APR's FCL while in fast lock
                              units: ppt/s
   [in]   adjSize1HzPSL    The adjSize1HzPSL[] array
   [in]   PSL_1Hz          The PSL_1Hz[] array
   [in]   adjSize1HzFCL    The adjSize1HzFCL[] array
   [in]   FCL_1Hz          The FCL_1Hz[] array
   [in]   adjSize1HzAdjScaling    The adjSize1HzAdjScaling[] array
   [in]   adjScaling_1Hz          The adjScaling_1Hz[] array

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE exampleAprReConfigurePSLFCL (void *hwParams,
                                       Sint32T staticOffset,
                                       Uint32T APRFrequencyLockedPhaseSlopeLimit,
                                       Uint32T APRFrequencyNotLockedPhaseSlopeLimit,
                                       Uint32T APRFrequencyFastPhaseSlopeLimit,
                                       Uint32T APRFrequencyLockedFrequencyChangeLimit,
                                       Uint32T APRFrequencyNotLockedFrequencyChangeLimit,
                                       Uint32T APRFrequencyFastFrequencyChangeLimit,
                                       Uint32T adjSize1HzPSL[ZL303XX_MAX_NUM_PSL_LIMITS],
                                       Uint32T PSL_1Hz[ZL303XX_MAX_NUM_PSL_LIMITS],
                                       Uint32T adjSize1HzFCL[ZL303XX_MAX_NUM_FCL_LIMITS],
                                       Uint32T FCL_1Hz[ZL303XX_MAX_NUM_FCL_LIMITS],
                                       Uint32T adjSize1HzAdjScaling[ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS],
                                       Uint32T adjScaling_1Hz[ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS])
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_PFReConfigS par;
    Uint32T j;


    if ( status == ZL303XX_OK )
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if ( status == ZL303XX_OK )
    {
        status = zl303xx_PFReConfigurePFStructInit(&par);
    }
    if ( status == ZL303XX_OK )
    {
        par.staticOffset = staticOffset;
        par.APRFrequencyLockedPhaseSlopeLimit = APRFrequencyLockedPhaseSlopeLimit;
        par.APRFrequencyNotLockedPhaseSlopeLimit = APRFrequencyNotLockedPhaseSlopeLimit;
        par.APRFrequencyFastPhaseSlopeLimit = APRFrequencyFastPhaseSlopeLimit;
        par.APRFrequencyLockedFrequencyChangeLimit = APRFrequencyLockedFrequencyChangeLimit;
        par.APRFrequencyNotLockedFrequencyChangeLimit = APRFrequencyNotLockedFrequencyChangeLimit;
        par.APRFrequencyFastFrequencyChangeLimit = APRFrequencyFastFrequencyChangeLimit;
        for ( j = 0; j < ZL303XX_MAX_NUM_PSL_LIMITS; j++ )
        {
            par.adjSize1HzPSL[j] = adjSize1HzPSL[j];
            par.PSL_1Hz[j] = PSL_1Hz[j];
        }
        for ( j = 0; j < ZL303XX_MAX_NUM_FCL_LIMITS; j++ )
        {
            par.adjSize1HzFCL[j] = adjSize1HzFCL[j];
            par.FCL_1Hz[j] = FCL_1Hz[j];
        }
        for ( j = 0; j < ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS; j++ )
        {
            par.adjSize1HzAdjScaling[j] = adjSize1HzAdjScaling[j];
            par.adjScaling_1Hz[j] = adjScaling_1Hz[j];
        }

        status = zl303xx_PFReConfigurePF(hwParams, &par);
    }
    if (status != ZL303XX_OK )
    {
        ZL303XX_TRACE_ALWAYS("exampleAprReConfigurePSLFCL: Failed=%d", status, 0,0,0,0,0);
    }

    return status;
}

/**

  Function Name:
    exampleAprReConfigurePSLFCLWithGlobals

  Details:
   Example code to set PSL and FCL values using global settings.

   See exampleAprReConfigurePSLFCL for more details.

  Parameters:
   [in]   hwParams         pointer to the device instance

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE exampleAprReConfigurePSLFCLWithGlobals (void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T j;
    Uint32T adjSize1HzPSL[ZL303XX_MAX_NUM_PSL_LIMITS];
    Uint32T adjSize1HzFCL[ZL303XX_MAX_NUM_FCL_LIMITS];
    Uint32T adjSize1HzAdjScaling[ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS];
    Uint32T PSL_1Hz[ZL303XX_MAX_NUM_PSL_LIMITS];
    Uint32T FCL_1Hz[ZL303XX_MAX_NUM_FCL_LIMITS];
    Uint32T adjScaling_1Hz[ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS];

    for (j = 0; j < ZL303XX_MAX_NUM_PSL_LIMITS; j++ )
    {
        zl303xx_AprGetPSL(j, &(adjSize1HzPSL[j]), &(PSL_1Hz[j]) );
    }
    for (j = 0; j < ZL303XX_MAX_NUM_FCL_LIMITS; j++ )
    {
        zl303xx_AprGetFCL(j, &(adjSize1HzFCL[j]), &(FCL_1Hz[j]) );
    }
    for (j = 0; j < ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS; j++ )
    {
        zl303xx_AprGetAdjScaling(j, &(adjSize1HzAdjScaling[j]), &(adjScaling_1Hz[j]) );
    }

    status = exampleAprReConfigurePSLFCL(hwParams,
                                         zl303xx_AprGetPCLFCLStaticOffset(),
                                         zl303xx_AprGetAPRFrequencyLockedPhaseSlopeLimit(),
                                         zl303xx_AprGetAPRFrequencyNotLockedPhaseSlopeLimit(),
                                         zl303xx_AprGetAPRFrequencyFastPhaseSlopeLimit(),
                                         zl303xx_AprGetAPRFrequencyLockedFrequencyChangeLimit(),
                                         zl303xx_AprGetAPRFrequencyNotLockedFrequencyChangeLimit(),
                                         zl303xx_AprGetAPRFrequencyFastFrequencyChangeLimit(),
                                         adjSize1HzPSL,
                                         PSL_1Hz,
                                         adjSize1HzFCL,
                                         FCL_1Hz,
                                         adjSize1HzAdjScaling,
                                         adjScaling_1Hz);

    return status;
}


/**

  Function Name:
    exampleAprGetPSLFCLStateData

  Details:
   Example code to get usefull information about the PSL&FCL component.

   This routine prints the following data:
   - PF lock status - this is a little different than APR's lock status and can
                      be usefull while debugging
   - PF lock count - related to PF lock status
   - PF static offset - this offset is currently being applied to all 1Hz
                        adjustments (static can be used in asymetrical networks)

  Parameters:
   [in]   hwParams            pointer to the device instance

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE exampleAprGetPSLFCLStateData (void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_BooleanE PFLocked;
    Sint32T PFLockCount;
    Sint32T PFStaticOffset;


    if ( status == ZL303XX_OK )
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if ( status == ZL303XX_OK )
    {
        status = zl303xxgetPFStateData(hwParams, &PFLocked, &PFLockCount, &PFStaticOffset);
    }
    if (status == ZL303XX_OK )
    {
        ZL303XX_TRACE_ALWAYS("PSL/FCL data: lock:%d  lock count:%d  static offset:%d",
                           PFLocked, PFLockCount,PFStaticOffset,0,0,0);
    }
    else
    {
        ZL303XX_TRACE_ALWAYS("exampleAprReConfigurePSLFCL: Failed=%d", status, 0,0,0,0,0);
    }

    return status;
}


/**

  Function Name:
    exampleAprGetPSLFCLConfigData

  Details:
   Example code to get configuration data from the PSL&FCL component.

   This routine prints a subset of the available configuration data.

   For a routine that prints all elements, see zl303xx_DebugAprPrintPSLFCLData().

  Parameters:
   [in]   hwParams            pointer to the device instance

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE exampleAprGetPSLFCLConfigData (void *hwParams)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_PFConfigS par;


    if ( status == ZL303XX_OK )
    {
        status = ZL303XX_CHECK_POINTER(hwParams);
    }
    if ( status == ZL303XX_OK )
    {
        status = zl303xx_AprGetPSLFCLConfigureData(hwParams, &par);
    }
    if (status == ZL303XX_OK )
    {
        ZL303XX_TRACE_ALWAYS("PSL/FCL data: ", 0,0,0,0,0,0);
        switch ( par.PFMode )
        {
        case PFM_normal:
            ZL303XX_TRACE_ALWAYS("  PFMode             : packet", 0,0,0,0,0,0);
            break;
        case PFM_hybrid:
            ZL303XX_TRACE_ALWAYS("  PFMode             : hybrid", 0,0,0,0,0,0);
            break;
        case PFM_last:
        default :
            ZL303XX_TRACE_ALWAYS("  PFMode            : unknown", 0,0,0,0,0,0);
            break;
        }
        ZL303XX_TRACE_ALWAYS("  phase slope limits:", 0,0,0,0,0,0);
        ZL303XX_TRACE_ALWAYS("    APRFrequencyLockedPhaseS...: %d ns/s", par.APRFrequencyLockedPhaseSlopeLimit, 0,0,0,0,0 );
        ZL303XX_TRACE_ALWAYS("    APRFrequencyNotLockedPha...: %d ns/s", par.APRFrequencyNotLockedPhaseSlopeLimit, 0,0,0,0,0 );
        ZL303XX_TRACE_ALWAYS("    APRFrequencyFastPhaseSlo...: %d ns/s", par.APRFrequencyFastPhaseSlopeLimit, 0,0,0,0,0 );
        ZL303XX_TRACE_ALWAYS("    APRFrequencyLockedFCL......: %d ppt/s", par.APRFrequencyLockedFrequencyChangeLimit, 0,0,0,0,0 );
        ZL303XX_TRACE_ALWAYS("    APRFrequencyNotLockedFCL...: %d ppt/s", par.APRFrequencyNotLockedFrequencyChangeLimit, 0,0,0,0,0 );
        ZL303XX_TRACE_ALWAYS("    APRFrequencyFastFCL........: %d ppt/s", par.APRFrequencyFastFrequencyChangeLimit, 0,0,0,0,0 );
        ZL303XX_TRACE_ALWAYS("    adjSize1HzPSL[0] : %6d   PSL_1Hz[0]: %d", par.adjSize1HzPSL[0], par.PSL_1Hz[0],0,0,0,0);
        ZL303XX_TRACE_ALWAYS("    adjSize1HzPSL[1] : %6d   PSL_1Hz[1]: %d", par.adjSize1HzPSL[1], par.PSL_1Hz[1],0,0,0,0);
        ZL303XX_TRACE_ALWAYS("    adjSize1HzPSL[2] : %6d   PSL_1Hz[2]: %d", par.adjSize1HzPSL[2], par.PSL_1Hz[2],0,0,0,0);
        ZL303XX_TRACE_ALWAYS("    adjSize1HzPSL[3] : %6d   PSL_1Hz[3]: %d", par.adjSize1HzPSL[3], par.PSL_1Hz[3],0,0,0,0);
        ZL303XX_TRACE_ALWAYS("    adjSize1HzPSL[4] : %6d   PSL_1Hz[4]: %d", par.adjSize1HzPSL[4], par.PSL_1Hz[4],0,0,0,0);
        ZL303XX_TRACE_ALWAYS("    adjSize1HzFCL[0] : %6d   FCL_1Hz[0]: %d", par.adjSize1HzFCL[0], par.FCL_1Hz[0],0,0,0,0);
        ZL303XX_TRACE_ALWAYS("    adjSize1HzFCL[1] : %6d   FCL_1Hz[1]: %d", par.adjSize1HzFCL[1], par.FCL_1Hz[1],0,0,0,0);
        ZL303XX_TRACE_ALWAYS("    adjSize1HzFCL[2] : %6d   FCL_1Hz[2]: %d", par.adjSize1HzFCL[2], par.FCL_1Hz[2],0,0,0,0);
        ZL303XX_TRACE_ALWAYS("    adjSize1HzFCL[3] : %6d   FCL_1Hz[3]: %d", par.adjSize1HzFCL[3], par.FCL_1Hz[3],0,0,0,0);
        ZL303XX_TRACE_ALWAYS("    adjSize1HzFCL[4] : %6d   FCL_1Hz[4]: %d", par.adjSize1HzFCL[4], par.FCL_1Hz[4],0,0,0,0);

        ZL303XX_TRACE_ALWAYS("See zl303xx_DebugAprPrintPSLFCLData() for a full list of parameters", 0,0,0,0,0,0);
        /*
           There are many more parameters; see the doxygen documentation or
           zl303xx_DebugAprPrintPSLFCLData().
        */
    }
    else
    {
        ZL303XX_TRACE_ALWAYS("zl303xx_PFGetConfigureData: Failed=%d", status, 0,0,0,0,0);
    }

    return status;
}


/* exampleAprTsLoggingCtrl */
/**
   This routine allows timestamp info to be gathered from APR for a number of
   seconds and sent to a user function for processing.

  Parameters:
   [in]   hwParams
   [in]   serverId       the APR server ID
   [in]   secondsToLog   how many seconds to collect timestamps
   [in]   userCalloutFn  pointer to the user function to handle the TS

  Return Value: zlStatusE

*******************************************************************************/
zlStatusE exampleAprTsLoggingCtrl(void *hwParams, Uint32T serverId,
                                  Uint32T secondsToLog,
                                  swFuncPtrTSLogging userCalloutFn);
zlStatusE exampleAprTsLoggingCtrl(void *hwParams, Uint32T serverId,
                                  Uint32T secondsToLog,
                                  swFuncPtrTSLogging userCalloutFn)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_AprTsLogConfigS logCfg;

    if (status == ZL303XX_OK)
    {
        status = zl303xx_AprTsLogConfigStructInit(hwParams, &logCfg);
    }

    if (status == ZL303XX_OK)
    {
        logCfg.durationSec = secondsToLog;
        logCfg.callback = userCalloutFn;

        status = zl303xx_AprTsLogConfigSet(hwParams, serverId, &logCfg);
    }

    return status;
}


/**

  Function Name:
    exampleAprGetConfigParameters

  Details:
   Returns the current configuration setting for APR

  Return Value:
   exampleAprConfigIdentifiersE

 *******************************************************************************/
exampleAprConfigIdentifiersE exampleAprGetConfigParameters(void)
{
    return exampleAprConfigId;
}


/**

  Function Name:
    exampleAprSetConfigParameters

  Details:
   Example code to set global APR parameters for the given configId.

   Some of the settings are dependent on packet rate selections which be
   configured prior to calling this function (see zl303xx_AprSetPktRate).

  Notes:
   Note this function only configures global settings. The global settings
   must then be used when configuring the APR device / server.

   Note this function will reset globals to defaults prior to changing
   settings to desired configId.

  Parameters:
   [in]   configId      the desired config ID

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE exampleAprSetConfigParameters(exampleAprConfigIdentifiersE configId)
{
    zlStatusE status = ZL303XX_OK;

    /* These variables are used in some settings and should be configured
       prior to calling this function */
    Uint32T fwdPps = zl303xx_AprGetPktRate(ZL303XX_TRUE);
    Uint32T revPps = zl303xx_AprGetPktRate(ZL303XX_FALSE);

    if ( status == ZL303XX_OK )
    {
        if ( configId >= ACI_LAST )
        {
            ZL303XX_TRACE_ALWAYS("exampleAprSetConfigParameters: invalid configId=%d >= max=%d", configId, ACI_LAST, 0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
        }
        else
        {
            ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 2, "exampleAprSetConfigParameters: configId=%d (fwd: %u, rev: %u)", configId, fwdPps, revPps,0,0,0);
        }
    }

    if ( status == ZL303XX_OK )
    {
        /* Save the configId */
        exampleAprConfigId = configId;

        /* Setup the globals using configId */
        switch ( configId )
        {
        /* First set from 0-99 is for basic mode settings*/

        case ACI_DEFAULT :
            /* Defaults */
            status |= exampleAprSetConfigDefaults();
            break;
        case ACI_FREQ_XO: /* ACI16 */   /* For now it is the same as Defaults */
            /* Uni-directional */
            status |= zl303xx_AprSetUseReversePath(ZL303XX_FALSE);

            /* APR settings */
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE);
            status |= zl303xx_AprSetAlgTypeMode(ZL303XX_NATIVE_PKT_FREQ_UNI);
            status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_OCXO_S3E);
            status |= zl303xx_AprSetHoldoverStability(ZL303XX_USING_DEFAULT_HOLDOVER_STABILITY);

         /*   status |= zl303xx_AprSetHybridServerFlag(ZL303XX_FALSE); */

            /* APR-1Hz settings */
           /* status |= zl303xx_AprSet1HzEnabled(ZL303XX_FALSE);
            status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PERIODIC);
            status |= zl303xx_AprSet1HzRealignmentInterval(120); */ /* seconds */

      /*      status |= zl303xx_AprSetFastLock1HzEnable(ZL303XX_FALSE);
            status |= zl303xx_AprSetFastLock1HzInterval(0);
            status |= zl303xx_AprSetFastLock1HzDelay(0);
            status |= zl303xx_AprSetStepTimeAdjustTimeThreshold(500000);
            status |= zl303xx_AprSetStepTimeAdjustFreqThreshold(500000);

            zl303xx_AprSetAPRFrequencyLockedPhaseSlopeLimit(ZL303XX_DEFAULT_APR_FREQ_PSL);
            zl303xx_AprSetAPRFrequencyNotLockedPhaseSlopeLimit(ZL303XX_DEFAULT_APR_FREQ_PSL);
            zl303xx_AprSetAPRFrequencyFastPhaseSlopeLimit(ZL303XX_DEFAULT_APR_FREQ_FAST_PSL);

            zl303xx_AprSetAPRFrequencyLockedFrequencyChangeLimit(APR_FCL_MAX);
            zl303xx_AprSetAPRFrequencyNotLockedFrequencyChangeLimit(APR_FCL_MAX);
            zl303xx_AprSetAPRFrequencyFastFrequencyChangeLimit(APR_FCL_MAX);

            zl303xx_AprSetPSL(0, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_PSL_0, ZL303XX_DEFAULT_PSL_1HZ_0 );
            zl303xx_AprSetPSL(1, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_PSL_1, ZL303XX_DEFAULT_PSL_1HZ_1 );
            zl303xx_AprSetPSL(2, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_PSL_2, ZL303XX_DEFAULT_PSL_1HZ_2 );
            zl303xx_AprSetPSL(3, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_PSL_3, ZL303XX_DEFAULT_PSL_1HZ_3 );
            zl303xx_AprSetPSL(4, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_PSL_4, ZL303XX_DEFAULT_PSL_1HZ_4 );

            zl303xx_AprSetFCL(0, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_FCL_0, ZL303XX_DEFAULT_FCL_1HZ_0 );
            zl303xx_AprSetFCL(1, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_FCL_1, ZL303XX_DEFAULT_FCL_1HZ_1 );
            zl303xx_AprSetFCL(2, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_FCL_2, ZL303XX_DEFAULT_FCL_1HZ_2 );
            zl303xx_AprSetFCL(3, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_FCL_3, ZL303XX_DEFAULT_FCL_1HZ_3 );
            zl303xx_AprSetFCL(4, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_FCL_4, ZL303XX_DEFAULT_FCL_1HZ_4 ); */

        /*    status |= zl303xx_AprSetPFLockInThreshold(1000);
            status |= zl303xx_AprSetPFLockInCount(2);
            status |= zl303xx_AprSetPFLockOutThreshold(2000);
            status |= zl303xx_AprSetPFAdjustFreqMinPhase(20);

            status |= zl303xx_AprSet1HzStartupTimeInit(ZL303XX_TRUE);

            status |= zl303xx_AprSetEnterPhaseLockStatusThreshold(ZL303XX_ENTER_PHASE_LOCK_STATUS_THRESHOLD);
            status |= zl303xx_AprSetExitPhaseLockStatusThreshold(ZL303XX_EXIT_PHASE_LOCK_STATUS_THRESHOLD);
        */
            /* Reset xParam overrides to defaults */
            status |= exampleAprOverrideXParamResetAll(ZL303XX_TRUE);
            status |= exampleAprOverrideXParamResetAll(ZL303XX_FALSE);
            status |= exampleAprOverride1HzXParamResetAll(ZL303XX_TRUE);
            status |= exampleAprOverride1HzXParamResetAll(ZL303XX_FALSE);


            break;
        case ACI_PHASE_XO: /* ACI17 */   /* For now it is the same as ACI1 .... needs fixing*/
            status |= exampleAprSetConfigDefaults();
            break;
        case ACI_FREQ_TCXO: /* ACI0 */

            /* Defaults */
            status |= exampleAprSetConfigDefaults();
            /* APR */
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE);
            status |= zl303xx_AprSetAlgTypeMode(ZL303XX_NATIVE_PKT_FREQ);
            status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_TCXO);

            /* 1Hz: disabled */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_FALSE);

            break;

        case ACI_PHASE_TCXO: /* ACI1 */

            /* Defaults */
            status |= exampleAprSetConfigDefaults();

            status |= zl303xx_AprSetHoldoverFreezeValue((Uint32T) 0);
            /* APR */
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE);
            status |= zl303xx_AprSetAlgTypeMode(ZL303XX_NATIVE_PKT_FREQ);
            status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_TCXO);

            /* 1Hz: enabled */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_TRUE);
            status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PERIODIC);
            status |= zl303xx_AprSet1HzRealignmentInterval(240);  /* seconds */

            /* APR 1Hz overrides */
            {
                /* Don't use 1Hz startup timer */
                status |= zl303xx_AprSet1HzStartupTimeInit(ZL303XX_FALSE);

                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 2, fwdPps * (zl303xx_AprGet1HzRealignmentInterval() - 7));
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 5, zl303xx_AprGet1HzStartupTimeInit());
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 9, 488);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 10, 2);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 12, zl303xx_AprGet1HzRealignmentInterval() - 30);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 23, 1800);

                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 2, revPps * (zl303xx_AprGet1HzRealignmentInterval() - 6));
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 5, zl303xx_AprGet1HzStartupTimeInit());
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 9, 488);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 10, 5);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 12, zl303xx_AprGet1HzRealignmentInterval() - 30);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 23, 1800);

                status |= zl303xx_AprSetPSL(1, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_PSL_1, 25);  /* default APR_PSL_885_NS */
                status |= zl303xx_AprSetPFLockInThreshold(2000);              /* default 1000 */
                status |= zl303xx_AprSetPFLockInCount(1);                     /* default 2 */
                status |= zl303xx_AprSetPFLockOutThreshold(4000);             /* default 2000 */

                status |= zl303xx_AprSetEnterPhaseLockStatusThreshold(1000);  /* default 1000 */
                status |= zl303xx_AprSetExitPhaseLockStatusThreshold(2000);   /* default 1500 */

            }
            break;

        case ACI_FREQ_OCXO_S3E: /* ACI2 */

            /* Defaults */
            status |= exampleAprSetConfigDefaults();

            /* APR */
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE);
            status |= zl303xx_AprSetAlgTypeMode(ZL303XX_NATIVE_PKT_FREQ);
            status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_OCXO_S3E);
            status |= zl303xx_AprSetHoldoverStability(15000);  /* Ppt */

            /* 1Hz: disabled */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_FALSE);
            break;

        case ACI_PHASE_OCXO_S3E: /* ACI3 */

            /* Defaults */
            status |= exampleAprSetConfigDefaults();

            status |= zl303xx_AprSetHoldoverFreezeValue(0);
            /* APR */
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE);
            status |= zl303xx_AprSetAlgTypeMode(ZL303XX_NATIVE_PKT_FREQ);
            status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_OCXO_S3E);
            status |= zl303xx_AprSetHoldoverStability(15000);  /* Ppt */

            /* 1Hz: enabled */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_TRUE);
            status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PERIODIC);
            status |= zl303xx_AprSet1HzRealignmentInterval(300);  /* seconds */

            /* APR 1Hz overrides */
            {
                /* Don't use 1Hz startup timer */
                /* status |= zl303xx_AprSet1HzStartupTimeInit(ZL303XX_TRUE); */

                /*  status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 7, 120); replace 120 by 60 and see*/
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 2, fwdPps * (zl303xx_AprGet1HzRealignmentInterval() - 7));
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 5, zl303xx_AprGet1HzStartupTimeInit());
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 9, 488);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 10, 2);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 12, zl303xx_AprGet1HzRealignmentInterval() - 30);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 23, 1800);

                /*  status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 7, 120); replace 120 by 60 and see */
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 2, revPps * (zl303xx_AprGet1HzRealignmentInterval() - 6));
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 5, zl303xx_AprGet1HzStartupTimeInit());
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 9, 488);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 10, 5);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 12, zl303xx_AprGet1HzRealignmentInterval() - 30);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 23, 1800);

                status |= zl303xx_AprSetPFLockInThreshold(1500);              /* default 1000 */
                status |= zl303xx_AprSetPFLockInCount(1);                     /* default 2 */
                status |= zl303xx_AprSetPFLockOutThreshold(2500);             /* default 2000 */

                status |= zl303xx_AprSetEnterPhaseLockStatusThreshold(1000);  /* default 1000 */
                status |= zl303xx_AprSetExitPhaseLockStatusThreshold(2000);   /* default 1500 */

                /* Enhanced R 4.6.2 settings (15 min sync reroute files)*/
                status |= zl303xx_AprSetPSL(0, 400,  2);  /*  2 ppb */
                status |= zl303xx_AprSetPSL(1, 1000, 8);  /*  8 ppb */
                status |= zl303xx_AprSetPSL(2, 3000, 15);  /* 15 ppb */
                status |= zl303xx_AprSetPSL(3, 10000000, 885);  /* 885 ppb */
                status |= zl303xx_AprSetAdjScaling(0, 100, 1000);  /* Any algorithm phase adjustment less than 100 is scaled 1000/1000 */
                status |= zl303xx_AprSetAdjScaling(1, 400, 250);   /* Any algorithm phase adjustment less than 400 is scaled 250/1000 */
                status |= zl303xx_AprSetAdjScaling(2, 1000, 400);   /* Any algorithm phase adjustment less than 800 is scaled 400/1000 */
                status |= zl303xx_AprSetAdjScaling(3, 2000, 800);  /* Any algorithm phase adjustment less than 1000 is scaled 800/1000 */

                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 132, 2);
                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 132, 2);
                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 51, 256);
                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 51, 256);
                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 53, 2048);
                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 53, 2048);
                status |= zl303xx_AprSet1HzStartupTimeInit(ZL303XX_TRUE);
                status |= zl303xx_AprSetAllow1HzAdjustmentsInHoldover(ZL303XX_TRUE);
            }

            /* Activate soft reset upon A90000 error */
            /*status |= zl303xx_SetAprUserHandlesA_90000Reset(ZL303XX_TRUE); */

            break;

        case ACI_BC_PARTIAL_ON_PATH_FREQ: /* ACI4 */

            /* Defaults */
            status |= exampleAprSetConfigDefaults();

            /* APR */
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE);
            status |= zl303xx_AprSetAlgTypeMode(ZL303XX_NATIVE_PKT_FREQ_FLEX);
            status |= zl303xx_AprSetFilterType(ZL303XX_BW_0_FILTER);

            /* status |= zl303xx_AprSetHoldoverStability(15000);  */ /* Ppt */

            /* 1Hz: disabled */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_FALSE);

            /* Set PSL reasonably high so that APR does not exceed the limit. */
            zl303xx_AprSetPSL(0,1000000000,APR_PSL_885_NS);

            /* All adjustments should be applied; The default for this feature is
            20ns but that is still too high. */
            status |= zl303xx_AprSetPFAdjustFreqMinPhase(0);

            status |= zl303xx_SetAdjustTimeMinThreshold(0);  /* In case we perform mode switching from ACI4 to ACI9*/
            break;

        case ACI_BC_PARTIAL_ON_PATH_PHASE:  /* ACI5 */

            /* Defaults */
            status |= exampleAprSetConfigDefaults();
            /* APR */
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE);
            status |= zl303xx_AprSetAlgTypeMode(ZL303XX_NATIVE_PKT_FREQ_FLEX);
            status |= zl303xx_AprSetFilterType(ZL303XX_BW_0_FILTER);

            /* 1Hz: enabled */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_TRUE);
            status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PERIODIC);
            status |= zl303xx_AprSet1HzRealignmentInterval(100); /* seconds */


            /* Set PSL reasonably high so that APR does not exceed the limit. */
            zl303xx_AprSetPSL(0,1000000000,APR_PSL_885_NS);

            /* All adjustments should be applied; The default for this feature is
            20ns but that is still too high. */
            /* Changed to 10 ns*/
            status |= zl303xx_AprSetPFAdjustFreqMinPhase(10);

            status |= zl303xx_SetAdjustTimeMinThreshold(0);  /* In case we perform mode switching from ACI5 to ACI9 */

            /*fast Lock */
           /*   status |= zl303xx_AprSetFastLockBW(ZL303XX_BW_5_FILTER);
            status |= zl303xx_AprSetFastLockPktSelWindowSize(150);
            status |= zl303xx_AprSetFastLockTotalTimeInSecs(800); */

           /* status |= zl303xx_AprSetApsType(2); */

            /* Enable L2 lock for low forward PDV */
            status |= zl303xx_AprSetL2phase_varLimitValue(0); /* in ns */

            if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
            {
                /* These are not immediately used, but on a mode switch to
                   ACI_BC_PARTIAL_ON_PATH_PHASE_SYNCE, these are needed for
                   hitless operation. */

                /* Use frequency adjustments */
                status |= zl303xx_AprSetDeviceHybridAdjMode(ZL303XX_HYBRID_ADJ_FREQ);
                /* Enable NCO assist */
                status |= zl303xx_AprSetUseNCOAssist( ZL303XX_TRUE );

                status |= zl303xx_AprSetAdjFreqDynamicAdjustmentEnable(ZL303XX_FALSE);
            }

            break;

        case ACI_BC_PARTIAL_ON_PATH_PHASE_SYNCE: /* ACI6 */

            /* Defaults */
            status |= exampleAprSetConfigDefaults();

            if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
            {
                /* Enable NCO assist */
                status |= zl303xx_AprSetDeviceHybridAdjMode(ZL303XX_HYBRID_ADJ_FREQ);
                status |= zl303xx_AprSetUseNCOAssist( ZL303XX_TRUE );

                /* APR */
                status |= zl303xx_AprSetAlgTypeMode(ZL303XX_NATIVE_PKT_FREQ_FLEX);
                status |= zl303xx_AprSetFilterType(ZL303XX_BW_0_FILTER);
                status |= zl303xx_AprSetAllow1HzAdjustmentsInHoldover(ZL303XX_TRUE);

                /* hybrid: enabled */
                status |= zl303xx_AprSetDeviceOptMode(ZL303XX_HYBRID_MODE);
                status |= zl303xx_AprSetHybridServerFlag(ZL303XX_TRUE);

                /* 1Hz: enabled */
                status |= zl303xx_AprSet1HzEnabled(ZL303XX_TRUE);
                status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PERIODIC);
                status |= zl303xx_AprSet1HzRealignmentInterval(100); /* seconds */
                status |= zl303xx_AprSetAdjFreqDynamicAdjustmentEnable(ZL303XX_FALSE);

                /* Set PSL reasonably high so that APR does not exceed the limit. */
                zl303xx_AprSetPSL(0,1000000000,APR_PSL_885_NS);

                /* All adjustments should be applied; The default for this feature is
                   20ns but that is still too high. Changed to 10 ns */
                status |= zl303xx_AprSetPFAdjustFreqMinPhase(10);

                status |= zl303xx_SetAdjustTimeMinThreshold(0);

                /* Enable L2 lock for low forward PDV */
                status |= zl303xx_AprSetL2phase_varLimitValue(0); /* in ns */
            }
            else
            {
                /* APR */
                status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_TCXO);
                status |= zl303xx_AprSetAlgTypeMode(ZL303XX_NATIVE_PKT_FREQ_FLEX);
                status |= zl303xx_AprSetFilterType(ZL303XX_BW_0_FILTER);

                /* hybrid: enabled */
                status |= zl303xx_AprSetDeviceOptMode(ZL303XX_HYBRID_MODE);
                status |= zl303xx_AprSetHybridServerFlag(ZL303XX_TRUE);

                /* 1Hz: enabled */
                status |= zl303xx_AprSet1HzEnabled(ZL303XX_TRUE);
                status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PERIODIC);
                status |= zl303xx_AprSet1HzRealignmentInterval(300);  /* seconds */

                /* Set PSL reasonably high so that APR does not exceed the limit. */
                status |= zl303xx_AprSetPSL(0, 200, 4);
                status |= zl303xx_AprSetPSL(1, 2000, 40);
                status |= zl303xx_AprSetPSL(2, 1000000000, APR_PSL_7P5_US);

                /* zl303xx_AprSetPSL(0,1000000000,APR_PSL_885_NS); */

                /* All adjustments should be applied; The default for this feature is
                20ns but that is still too high. */
                status |= zl303xx_AprSetPFAdjustFreqMinPhase(0);

                status |= zl303xx_SetAdjustTimeMinThreshold(0);  /* In case we perform mode switching from ACI9 to ACI9*/
            }
            break;

        case ACI_BC_FULL_ON_PATH_FREQ: /* ACI7 */

            if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
            {
                /* ZL3077X devices do not support frequency-only modes.
                   Use ACI_BC_FULL_ON_PATH_PHASE. */
            }

            /* Defaults */
            status |= exampleAprSetConfigDefaults();

            /* APR */
            status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_TCXO);
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE);
            status |= zl303xx_AprSetAlgTypeMode(ZL303XX_BOUNDARY_CLK);
            status |= zl303xx_AprSetFilterType(ZL303XX_BW_1_FILTER);

            /* 1Hz: disabled */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_FALSE);

            status |= zl303xx_SetAdjustTimeMinThreshold(0);  /* In case we perform mode switching from ACI7 to ACI9*/

            /* Set PSL reasonably high so that APR does not exceed the limit. */
            zl303xx_AprSetPSL(0,1000000000,APR_PSL_885_NS);

            status |= zl303xx_AprSetUseOFM(ZL303XX_FALSE);
            break;

        case ACI_BC_FULL_ON_PATH_PHASE:  /* ACI8 */

            /* Defaults */
            status |= exampleAprSetConfigDefaults();

            if (type_2b_enabled == ZL303XX_TRUE) /* Type 2 PLL Enabled, complete settings */
            {
                /* APR Settings */
                status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE);
                status |= zl303xx_AprSetAlgTypeMode(ZL303XX_BOUNDARY_CLK);
                /* APR Type2Pll Enable/Settings */
                status |= zl303xx_AprSetUseType2BPLL(ZL303XX_TRUE);
                status |= zl303xx_AprSetFilterType(ZL303XX_BW_90mHz);
                status |= zl303xx_AprSetAprTaskBasePeriodMs(125);
                status |= zl303xx_AprSetUseType2BPLLFastLock(ZL303XX_TRUE);
                status |= zl303xx_AprSetDFSeedValue(0); /* Initial DF seed value for algorithm module in ppb */
                status |= zl303xx_AprSetPFInitialFrequencyOffset(0);  /* Initial DF seed value for PSL&FCL module in ppt */
                status |= zl303xx_AprSetType2BFastlockStartupIt(3);
                status |= zl303xx_AprSetType2BFastlockThreshold(100);

                /* APR-Phase(1 Hz) Settings */
                status |= zl303xx_AprSet1HzEnabled(ZL303XX_TRUE);
                status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PER_PACKET);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE,  38, ZL303XX_PER_PACKET_ADJ_BYPASS);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 38, ZL303XX_PER_PACKET_ADJ_BYPASS);

                /* General APR Settings */
                /* Set PSL */
                
				status |= zl303xx_AprSetAPRFrequencyLockedPhaseSlopeLimit(APR_PSL_MAX);
                status |= zl303xx_AprSetAPRFrequencyNotLockedPhaseSlopeLimit(APR_PSL_MAX);
                
                /* Set thresholds */
                status |= zl303xx_AprSetEnterPhaseLockStatusThreshold(1000);  /* default 1000 */
                status |= zl303xx_AprSetExitPhaseLockStatusThreshold(1500);   /* default 1500 */
                status |= zl303xx_AprSetStepTimeAdjustTimeThreshold(20000);
                status |= zl303xx_AprSetStepTimeAdjustFreqThreshold(20000);
                status |= zl303xx_AprSetPFStepTimeResolution(100);

                /* Forward */
                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 0, 4);
                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 30, 1);
                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 102, 10000);
                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 104, 0);
                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 106, 200000);
                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 109, 30000);
                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 128, 10);  /* 10 s L3*/
                /* Reverse */
                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 50, 4);
                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 80, 1);
                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 102, 10000);
                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 104, 0);
                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 106, 200000);
                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 109, 30000);
                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 128, 10); /* 10 s L3*/


                if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
                {
                    /* These are not immediately used, but on a mode switch to
                       ACI_BC_FULL_ON_PATH_PHASE_SYNCE, these are needed for hitless
                       operation. */

                    /* Use frequency adjustments */
                    status |= zl303xx_AprSetDeviceHybridAdjMode(ZL303XX_HYBRID_ADJ_FREQ);
                    /* Enable NCO assist */
                    status |= zl303xx_AprSetUseNCOAssist( ZL303XX_TRUE );
                }
            }
            else /* 4.9.0 Settings, complete settings */
            {
               /* APR */
               status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_TCXO);
               status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE);
               status |= zl303xx_AprSetAlgTypeMode(ZL303XX_BOUNDARY_CLK);
               status |= zl303xx_AprSetFilterType(ZL303XX_BW_1_FILTER);

               /* 1Hz: enabled */
               status |= zl303xx_AprSet1HzEnabled(ZL303XX_TRUE);
               status |= zl303xx_AprSet1HzStartupTimeInit(ZL303XX_FALSE);
               status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 5, zl303xx_AprGet1HzStartupTimeInit());
               status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PERIODIC);
               status |= zl303xx_AprSet1HzRealignmentInterval(10);  /* seconds */
               status |= zl303xx_AprSetAllow1HzAdjustmentsInHoldover(ZL303XX_TRUE);

               status |= zl303xx_AprSetPFStepTimeResolution(100);
               status |= zl303xx_AprSetStepTimeAdjustFreqThreshold(20000);

               /* Set PSL -> 周波数・位相補正値入力 */
               
				status |= zl303xx_AprSetAPRFrequencyLockedPhaseSlopeLimit(512);

               /* Set PSL */
               status |= zl303xx_AprSetPSL(0, 200, 4);
               status |= zl303xx_AprSetPSL(1, 2000, 40);
               status |= zl303xx_AprSetPSL(2, 1000000000, APR_PSL_7P5_US);

               /* All phase adjustments should be performed */
               status |= zl303xx_AprSetPFAdjustFreqMinPhase(0);
               status |= zl303xx_SetAdjustTimeMinThreshold(0);

               /* Common APR Xparam Uppm*/
               status |= exampleAprOverrideXParam(ZL303XX_TRUE, 123, 12000000);
               status |= exampleAprOverrideXParam(ZL303XX_FALSE, 123, 12000000);
            }
            break;

        case ACI_BC_FULL_ON_PATH_PHASE_SYNCE:  /* ACI9 */

            /* Defaults */
            status |= exampleAprSetConfigDefaults();

            if (type_2b_enabled == ZL303XX_TRUE)/* Type 2 PLL Enabled, complete settings */
            {
               /* APR Settings */
               status |= zl303xx_AprSetDeviceOptMode(ZL303XX_HYBRID_MODE);
               status |= zl303xx_AprSetHybridServerFlag(ZL303XX_TRUE);
               status |= zl303xx_AprSetAlgTypeMode(ZL303XX_BOUNDARY_CLK);
               /* APR Type2Pll Settings */
               status |= zl303xx_AprSetUseType2BPLL(ZL303XX_TRUE);
               status |= zl303xx_AprSetFilterType(ZL303XX_BW_90mHz);
               status |= zl303xx_AprSetAprTaskBasePeriodMs(125);
               status |= zl303xx_AprSetUseType2BPLLFastLock(ZL303XX_TRUE);
               status |= zl303xx_AprSetDFSeedValue(0); /* Initial DF seed value for algorithm module in ppb */
               status |= zl303xx_AprSetPFInitialFrequencyOffset(0);  /* Initial DF seed value for PSL&FCL module in ppt */
               status |= zl303xx_AprSetType2BFastlockStartupIt(3);
               status |= zl303xx_AprSetType2BFastlockThreshold(100);

               /* APR-Phase(1 Hz) Settings */
               status |= zl303xx_AprSet1HzEnabled(ZL303XX_TRUE);
               status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PER_PACKET);
               /* Bypass 1 Hz adjustment process.  Use TypeIIB architecture introduced in 4.10.0 release. */
               status |= exampleAprOverride1HzXParam(ZL303XX_TRUE,  38, ZL303XX_PER_PACKET_ADJ_BYPASS);
               status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 38, ZL303XX_PER_PACKET_ADJ_BYPASS);

               /* General APR Settings */
               /* Set PSL */
				status |= zl303xx_AprSetAPRFrequencyLockedPhaseSlopeLimit(APR_PSL_MAX);

               status |= zl303xx_AprSetAPRFrequencyNotLockedPhaseSlopeLimit(APR_PSL_MAX);
               /* Set thresholds */
               status |= zl303xx_AprSetEnterPhaseLockStatusThreshold(1000);  /* default 1000 */
               status |= zl303xx_AprSetExitPhaseLockStatusThreshold(1500);   /* default 1500 */
               status |= zl303xx_AprSetStepTimeAdjustTimeThreshold(20000);
               status |= zl303xx_AprSetStepTimeAdjustFreqThreshold(20000);
               status |= zl303xx_AprSetPFStepTimeResolution(100);

               /* Forward */
               status |= exampleAprOverrideXParam(ZL303XX_TRUE, 0, 4);
               status |= exampleAprOverrideXParam(ZL303XX_TRUE, 30, 1);
               status |= exampleAprOverrideXParam(ZL303XX_TRUE, 102, 10000);
               status |= exampleAprOverrideXParam(ZL303XX_TRUE, 104, 0);
               status |= exampleAprOverrideXParam(ZL303XX_TRUE, 106, 200000);
               status |= exampleAprOverrideXParam(ZL303XX_TRUE, 109, 30000);
               status |= exampleAprOverrideXParam(ZL303XX_TRUE, 128, 10);
               /* Reverse */
               status |= exampleAprOverrideXParam(ZL303XX_FALSE, 50, 4);
               status |= exampleAprOverrideXParam(ZL303XX_FALSE, 80, 1);
               status |= exampleAprOverrideXParam(ZL303XX_FALSE, 102, 10000);
               status |= exampleAprOverrideXParam(ZL303XX_FALSE, 104, 0);
               status |= exampleAprOverrideXParam(ZL303XX_FALSE, 106, 200000);
               status |= exampleAprOverrideXParam(ZL303XX_FALSE, 109, 30000);
               status |= exampleAprOverrideXParam(ZL303XX_FALSE, 128, 10);

               /* Settings for Microsemi and custom devices */
               if(zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE)
               {
                   /* The ZL3034X device family does not support hybrid mode */
                   status |= zl303xx_AprSetDeviceHybridAdjMode(ZL303XX_HYBRID_ADJ_NONE);
               }
               else if(zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE)
               {
                   /* Used phase adjustments */
                   status |= zl303xx_AprSetDeviceHybridAdjMode(ZL303XX_HYBRID_ADJ_PHASE);
               }
               else if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
               {
                   /* Used phase adjustments */
                   status |= zl303xx_AprSetDeviceHybridAdjMode(ZL303XX_HYBRID_ADJ_PHASE);
               }
               else if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
               {
                   /* Use frequency adjustments */
                   status |= zl303xx_AprSetDeviceHybridAdjMode(ZL303XX_HYBRID_ADJ_FREQ);
               }
               else if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
               {
                   /* Used phase adjustments */
                   status |= zl303xx_AprSetDeviceHybridAdjMode(ZL303XX_HYBRID_ADJ_PHASE);
               }
               else if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
               {
                   /* Use frequency adjustments */
                   status |= zl303xx_AprSetDeviceHybridAdjMode(ZL303XX_HYBRID_ADJ_FREQ);
                   /* Enable NCO assist */
                   status |= zl303xx_AprSetUseNCOAssist( ZL303XX_TRUE );
               }
               else if( zl303xx_GetDefaultDeviceType() == CUSTOM_DEVICETYPE ) /* Unknown non-Microsemi devices */
               {
                   /* Disable adjustment */
                   status |= zl303xx_AprSetDeviceHybridAdjMode(ZL303XX_HYBRID_ADJ_NONE);
                   /* Use custom adjust phase function (adjustTime) */
                   /* status |= zl303xx_AprSetDeviceHybridAdjMode(ZL303XX_HYBRID_ADJ_CUSTOM_PHASE); */
                   /* Use custom adjust frequency function (adjustFreq) */
                   /* status |= zl303xx_AprSetDeviceHybridAdjMode(ZL303XX_HYBRID_ADJ_CUSTOM_FREQ); */
               }
            }
            else /* 4.9.0 Backward compatible settings */
            {
               /* APR */
               status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_TCXO_FAST);
               status |= zl303xx_AprSetAlgTypeMode(ZL303XX_BOUNDARY_CLK);
               status |= zl303xx_AprSetFilterType(ZL303XX_BW_1_FILTER);

               /* hybrid: enabled */
               status |= zl303xx_AprSetDeviceOptMode(ZL303XX_HYBRID_MODE);
               status |= zl303xx_AprSetHybridServerFlag(ZL303XX_TRUE);

               /* 1Hz: enabled */
               status |= zl303xx_AprSet1HzEnabled(ZL303XX_TRUE);
               status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PERIODIC);
               status |= zl303xx_AprSetAllow1HzAdjustmentsInHoldover(ZL303XX_TRUE);

               /* Custom Settings for Microsemi devices */
               if(( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE ) ||
                  ( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE ) ||
                  ( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE ) ||
                  ( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE ) ||
                  ( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE ))
               {
                   /* MSCC Devices Utilize Per-Packet alignment mode for G.8273.2 compliance */
                   status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PER_PACKET);

                   status |= zl303xx_AprSetEnterPhaseLockStatusThreshold(1000);  /* default 1000 */
                   status |= zl303xx_AprSetExitPhaseLockStatusThreshold(1500);   /* default 1500 */

                   status |= zl303xx_AprSet1HzFilterWriteInterval(125);

                   status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 18, 8);
                   status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 25, 0);
                   status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 26, 21);
                   status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 23, 30);
                   status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 28, zl303xx_AprGet1HzFilterWriteInterval());

                   status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 18, 8);
                   status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 25, 0);
                   status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 26, 21);
                   status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 23, 30);
                   status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 28, zl303xx_AprGet1HzFilterWriteInterval());

                   status |= zl303xx_AprSetStepTimeAdjustTimeThreshold(20000);
                   status |= zl303xx_AprSetStepTimeAdjustFreqThreshold(20000);
                   status |= zl303xx_AprSetPFStepTimeResolution(100);


                   /* Forward */
                   status |= exampleAprOverrideXParam(ZL303XX_TRUE, 0, 4);
                   status |= exampleAprOverrideXParam(ZL303XX_TRUE, 30, 1);
                   status |= exampleAprOverrideXParam(ZL303XX_TRUE, 102, 10000);
                   status |= exampleAprOverrideXParam(ZL303XX_TRUE, 104, 0);
                   status |= exampleAprOverrideXParam(ZL303XX_TRUE, 106, 200000);
                   status |= exampleAprOverrideXParam(ZL303XX_TRUE, 109, 30000);
                   status |= exampleAprOverrideXParam(ZL303XX_TRUE, 128, 10);
                   /* Reverse */
                   status |= exampleAprOverrideXParam(ZL303XX_FALSE, 50, 4);
                   status |= exampleAprOverrideXParam(ZL303XX_FALSE, 80, 1);
                   status |= exampleAprOverrideXParam(ZL303XX_FALSE, 102, 10000);
                   status |= exampleAprOverrideXParam(ZL303XX_FALSE, 104, 0);
                   status |= exampleAprOverrideXParam(ZL303XX_FALSE, 106, 200000);
                   status |= exampleAprOverrideXParam(ZL303XX_FALSE, 109, 30000);
                   status |= exampleAprOverrideXParam(ZL303XX_FALSE, 128, 10);

                   /* Set the method of applying the per packet adjustment.
                      Please see zl303xx_AprPerPacketAdjTypeE enum for available options.
                      Current defaults are recommended method for meeting G.8273.2 standards.
                   */
                   if( (zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE) ||
                       (zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE) ||
                       (zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE) ||
                       (zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE))
                   {
                       /* Used phase adjustments */
                       status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 38, ZL303XX_PER_PACKET_ADJ_HYBRID_PHASE);
                       status |= exampleAprOverride1HzXParam(ZL303XX_FALSE,38, ZL303XX_PER_PACKET_ADJ_HYBRID_PHASE);
                   }
                   else if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
                   {
                       /* Use frequency adjustments */
                       status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 38, ZL303XX_PER_PACKET_ADJ_HYBRID_FREQ);
                       status |= exampleAprOverride1HzXParam(ZL303XX_FALSE,38, ZL303XX_PER_PACKET_ADJ_HYBRID_FREQ);
                   }
                   else
                   {
                       /* Use custom adjust time function */
                       status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 38, ZL303XX_PER_PACKET_ADJ_HYBRID_ADJTIME);
                       status |= exampleAprOverride1HzXParam(ZL303XX_FALSE,38, ZL303XX_PER_PACKET_ADJ_HYBRID_ADJTIME);
                   }
               }

               /* Set PSL */
               status |= zl303xx_AprSetPSL(0, 20000, 512);
               status |= zl303xx_AprSetPSL(1, 1000000000, APR_PSL_7P5_US);

               /* All phase adjustments should be performed */
               status |= zl303xx_AprSetPFAdjustFreqMinPhase(0);
               status |= zl303xx_SetAdjustTimeMinThreshold(0);
            }
            break;

        case ACI_FREQ_ACCURACY_FDD: /* ACI10 */

            /* Defaults */
            status |= exampleAprSetConfigDefaults();
            /* APR */
            status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_OCXO_S3E);
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE);
            status |= zl303xx_AprSetAlgTypeMode(ZL303XX_NATIVE_PKT_FREQ_ACCURACY_FDD);
            status |= zl303xx_AprSetFilterType(ZL303XX_BW_3_FILTER);

            /* 1Hz: disabled */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_FALSE);

            /* The following for fast lock */
            status |= zl303xx_AprSetFastLockBW(ZL303XX_BW_7_FILTER);
            status |= zl303xx_AprSetFastLockPktSelWindowSize(150);
            status |= zl303xx_AprSetFastLockTotalTimeInSecs(800);
            status |= exampleAprOverrideXParam(ZL303XX_TRUE, 0, 200);
            status |= exampleAprOverrideXParam(ZL303XX_FALSE, 50, 200);
            status |= exampleAprOverrideXParam(ZL303XX_TRUE, 102, 221600);
            status |= exampleAprOverrideXParam(ZL303XX_FALSE, 102, 221600);

            break;

        case ACI_FREQ_ACCURACY_XDSL: /* ACI11 */

            /* Defaults */
            status |= exampleAprSetConfigDefaults();

            /* APR */
            status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_TCXO);
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE);
            status |= zl303xx_AprSetAlgTypeMode(ZL303XX_XDSL_FREQ_ACCURACY);

            /* 1Hz: disabled */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_FALSE);
            break;

        case ACI_ELEC_FREQ:  /* ACI12 */
            /* APR - not shown because this is electrical */

            /* Defaults */
            status |= exampleAprSetConfigDefaults();

            /* electric enabled */
            status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_TCXO);
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_ELECTRIC_MODE);

            /* 1Hz: disabled */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_FALSE);
            break;

        case ACI_ELEC_PHASE:  /* ACI13 */
            /* APR - not shown because this is electrical */

            /* Defaults */
            status |= exampleAprSetConfigDefaults();

            /* electric enabled */
            status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_TCXO);
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_ELECTRIC_MODE);

            /* 1Hz: disabled */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_FALSE);
            break;



        /* Modes from 100-199 are for environment-specific custom-settings*/
        case ACI_PHASE_RELAXED_C60W:  /* ACI102 */
            /* Defaults */
            status |= exampleAprSetConfigDefaults();

            /* APR Settings*/
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE);
            status |= zl303xx_AprSetAlgTypeMode(ZL303XX_NATIVE_PKT_FREQ);
            status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_TCXO_FAST_ENHANCED);
            status |= zl303xx_AprSetEnterPhaseLockStatusThreshold(1500000);
            status |= zl303xx_AprSetExitPhaseLockStatusThreshold(3000000);
            status |= zl303xx_AprSetPullInRange(ZL303XX_APR_PIR_50_PPM);
            status |= zl303xx_AprSetLockMasks(7);

            /* Do NOT used APR algorithm warm-start in cable mode.  Use the following
               function to seed the intial DCO frequecny offset value in PPT.  APR
               warm-start will not work with large cable PDVs */
            /* Replace 0 with actual df offset in ppt */
            status |= zl303xx_AprSetPFInitialFrequencyOffset(0);

            /* PSL and FCL Settings */
            status |= zl303xx_AprSetMaxAdjFreqTime(240);

            status |= zl303xx_AprSetAPRFrequencyLockedPhaseSlopeLimit(200);
            status |= zl303xx_AprSetAPRFrequencyNotLockedPhaseSlopeLimit(512);
            status |= zl303xx_AprSetAPRFrequencyFastPhaseSlopeLimit(APR_PSL_MAX);
            status |= zl303xx_AprSetPSL(0, 1000000, 10);
            status |= zl303xx_AprSetPSL(1, 10000000, 20);

            status |= zl303xx_AprSetAPRFrequencyLockedFrequencyChangeLimit(1500);
            status |= zl303xx_AprSetAPRFrequencyNotLockedFrequencyChangeLimit(6500);
            status |= zl303xx_AprSetAPRFrequencyFastFrequencyChangeLimit(1);
            status |= zl303xx_AprSetFCL(0, 2000000, 6500 );
            status |= zl303xx_AprSetFCL(1, 100000000, APR_FCL_MAX );

            /* APR 1 Hz Phase Alignment Settings */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_TRUE);
            status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PERIODIC);
            status |= zl303xx_AprSet1HzRealignmentInterval(60);  /* seconds */
            status |= zl303xx_AprSetAllow1HzAdjustmentsInHoldover(ZL303XX_TRUE);
            status |= zl303xx_AprSetPFAdjustFreqMinPhase(500);

            status |= zl303xx_AprSetFastLock1HzEnable(ZL303XX_TRUE);
            status |= zl303xx_AprSetFastLock1HzInterval(2);
            status |= zl303xx_AprSetFastLock1HzTotalPeriod(40);
            status |= zl303xx_AprSetFastLock1HzDelay(10);

            status |= zl303xx_AprSetStepTimeAdjustTimeThreshold(500000);
            status |= zl303xx_AprSetStepTimeAdjustFreqThreshold(6000000);
            status |= zl303xx_AprSetPFStepTimeResolution(50);

            /* APR Freq overrides */
            {
                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 0,  3);
                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 14, 350);

                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 50, 3);
                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 64, 200);
            }

            /* APR 1Hz overrides */
            {
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 2, fwdPps * 10);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 5, ZL303XX_TRUE);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 6, 300);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 7, 20);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 10, 1);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 11, 1000000000);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 23, 1200);

                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 2, revPps * 10);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 5, ZL303XX_TRUE);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 6, 300);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 7, 20);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 10, 1);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 11, 1000000000);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 23, 1200);
            }

            break;

        case ACI_PHASE_RELAXED_C150:  /* ACI103 */
             /* Defaults */
            status |= exampleAprSetConfigDefaults();

            /* APR Settings*/
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE);
            status |= zl303xx_AprSetAlgTypeMode(ZL303XX_NATIVE_PKT_FREQ);
            status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_TCXO_FAST_ENHANCED);
            status |= zl303xx_AprSetEnterPhaseLockStatusThreshold(1500000);
            status |= zl303xx_AprSetExitPhaseLockStatusThreshold(3000000);
            status |= zl303xx_AprSetPullInRange(ZL303XX_APR_PIR_50_PPM);
            status |= zl303xx_AprSetLockMasks(7);;

            /* PSL and FCL Settings */
            status |= zl303xx_AprSetMaxAdjFreqTime(240);

            status |= zl303xx_AprSetAPRFrequencyLockedPhaseSlopeLimit(200);
            status |= zl303xx_AprSetAPRFrequencyNotLockedPhaseSlopeLimit(512);
            status |= zl303xx_AprSetAPRFrequencyFastPhaseSlopeLimit(APR_PSL_MAX);
            status |= zl303xx_AprSetPSL(0, 1000000, 10);
            status |= zl303xx_AprSetPSL(1, 10000000, 20);

            status |= zl303xx_AprSetAPRFrequencyLockedFrequencyChangeLimit(1500);
            status |= zl303xx_AprSetAPRFrequencyNotLockedFrequencyChangeLimit(6500);
            status |= zl303xx_AprSetAPRFrequencyFastFrequencyChangeLimit(100000);
            status |= zl303xx_AprSetFCL(0, 2000000, 6500 );
            status |= zl303xx_AprSetFCL(1, 100000000, APR_FCL_MAX );

            /* APR 1 Hz Phase Alignment Settings */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_TRUE);
            status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PERIODIC);
            status |= zl303xx_AprSet1HzRealignmentInterval(60);  /* seconds */
            status |= zl303xx_AprSetAllow1HzAdjustmentsInHoldover(ZL303XX_TRUE);
            status |= zl303xx_AprSetPFAdjustFreqMinPhase(500);

            status |= zl303xx_AprSetFastLock1HzEnable(ZL303XX_TRUE);
            status |= zl303xx_AprSetFastLock1HzInterval(2);
            status |= zl303xx_AprSetFastLock1HzTotalPeriod(125);
            status |= zl303xx_AprSetFastLock1HzDelay(90);

            status |= zl303xx_AprSetStepTimeAdjustTimeThreshold(500000);
            status |= zl303xx_AprSetStepTimeAdjustFreqThreshold(6000000);
            status |= zl303xx_AprSetPFStepTimeResolution(50);

            /* APR Freq overrides */
            {
                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 0,  5);
                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 14, 350);

                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 50, 5);
                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 64, 200);
            }

            /* APR 1Hz overrides */
            {
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 2, fwdPps * 10);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 5, ZL303XX_TRUE);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 6, 300);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 7, 20);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 10, 1);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 11, 1000000000);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 23, 1200);

                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 2, revPps * 10);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 5, ZL303XX_TRUE);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 6, 300);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 7, 20);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 10, 1);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 11, 1000000000);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 23, 1200);
            }

            break;
        case ACI_PHASE_RELAXED_C180:  /* ACI104 */
            /* Defaults */
            status |= exampleAprSetConfigDefaults();

            /* APR Settings*/
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE);
            status |= zl303xx_AprSetAlgTypeMode(ZL303XX_NATIVE_PKT_FREQ);
            status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_TCXO_FAST_ENHANCED);
            status |= zl303xx_AprSetEnterPhaseLockStatusThreshold(1500000);
            status |= zl303xx_AprSetExitPhaseLockStatusThreshold(3000000);
            status |= zl303xx_AprSetPullInRange(ZL303XX_APR_PIR_50_PPM);
            status |= zl303xx_AprSetLockMasks(7);

            /* PSL and FCL Settings */
            status |= zl303xx_AprSetMaxAdjFreqTime(240);

            status |= zl303xx_AprSetAPRFrequencyLockedPhaseSlopeLimit(200);
            status |= zl303xx_AprSetAPRFrequencyNotLockedPhaseSlopeLimit(512);
            status |= zl303xx_AprSetAPRFrequencyFastPhaseSlopeLimit(APR_PSL_MAX);
            status |= zl303xx_AprSetPSL(0, 1000000, 10);
            status |= zl303xx_AprSetPSL(1, 10000000, 20);

            status |= zl303xx_AprSetAPRFrequencyLockedFrequencyChangeLimit(1500);
            status |= zl303xx_AprSetAPRFrequencyNotLockedFrequencyChangeLimit(6500);
            status |= zl303xx_AprSetAPRFrequencyFastFrequencyChangeLimit(100000);
            status |= zl303xx_AprSetFCL(0, 2000000, 6500 );
            status |= zl303xx_AprSetFCL(1, 100000000, APR_FCL_MAX );

            /* APR 1 Hz Phase Alignment Settings */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_TRUE);
            status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PERIODIC);
            status |= zl303xx_AprSet1HzRealignmentInterval(60);  /* seconds */
            status |= zl303xx_AprSetAllow1HzAdjustmentsInHoldover(ZL303XX_TRUE);
            status |= zl303xx_AprSetPFAdjustFreqMinPhase(1);

            status |= zl303xx_AprSetFastLock1HzEnable(ZL303XX_TRUE);
            status |= zl303xx_AprSetFastLock1HzInterval(2);
            status |= zl303xx_AprSetFastLock1HzTotalPeriod(150);
            status |= zl303xx_AprSetFastLock1HzDelay(100);

            status |= zl303xx_AprSetStepTimeAdjustTimeThreshold(500000);
            status |= zl303xx_AprSetStepTimeAdjustFreqThreshold(6000000);
            status |= zl303xx_AprSetPFStepTimeResolution(50);

            /* APR Freq overrides */
            {
                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 0,  8);
                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 14, 350);

                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 50, 8);
                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 64, 200);
            }

            /* APR 1Hz overrides */
            {
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 2, fwdPps * 10);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 5, ZL303XX_TRUE);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 6, 300);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 7, 20);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 10, 1);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 11, 1000000000);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 23, 1200);

                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 2, revPps * 10);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 5, ZL303XX_TRUE);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 6, 300);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 7, 20);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 10, 1);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 11, 1000000000);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 23, 1200);
            }

            break;
        case ACI_PHASE_RELAXED_C240:  /* ACI105 */
            /* Defaults */
            status |= exampleAprSetConfigDefaults();

            /* Set to one to enable holdover Freeze */
            status |= zl303xx_AprSetHoldoverFreezeValue(2);

            /* APR Settings*/
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE);
            status |= zl303xx_AprSetAlgTypeMode(ZL303XX_NATIVE_PKT_FREQ);
            status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_TCXO_FAST_ENHANCED);
            status |= zl303xx_AprSetEnterPhaseLockStatusThreshold(1500000);
            status |= zl303xx_AprSetExitPhaseLockStatusThreshold(3000000);
            status |= zl303xx_AprSetPullInRange(ZL303XX_APR_PIR_50_PPM);
            status |= zl303xx_AprSetLockMasks(7);

            /* PSL and FCL Settings */
            status |= zl303xx_AprSetMaxAdjFreqTime(240);

            status |= zl303xx_AprSetAPRFrequencyLockedPhaseSlopeLimit(200);
            status |= zl303xx_AprSetAPRFrequencyNotLockedPhaseSlopeLimit(512);
            status |= zl303xx_AprSetAPRFrequencyFastPhaseSlopeLimit(APR_PSL_MAX);
            status |= zl303xx_AprSetPSL(0, 1000000, 10);
            status |= zl303xx_AprSetPSL(1, 10000000, 20);

            status |= zl303xx_AprSetAPRFrequencyLockedFrequencyChangeLimit(1500);
            status |= zl303xx_AprSetAPRFrequencyNotLockedFrequencyChangeLimit(6500);
            status |= zl303xx_AprSetAPRFrequencyFastFrequencyChangeLimit(100000);
            status |= zl303xx_AprSetFCL(0, 2000000, 6500 );
            status |= zl303xx_AprSetFCL(1, 100000000, APR_FCL_MAX );

            /* APR 1 Hz Phase Alignment Settings */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_TRUE);
            status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PERIODIC);
            status |= zl303xx_AprSet1HzRealignmentInterval(60);  /* seconds */
            status |= zl303xx_AprSetAllow1HzAdjustmentsInHoldover(ZL303XX_TRUE);
            status |= zl303xx_AprSetPFAdjustFreqMinPhase(500);

            status |= zl303xx_AprSetFastLock1HzEnable(ZL303XX_TRUE);
            status |= zl303xx_AprSetFastLock1HzInterval(2);
            status |= zl303xx_AprSetFastLock1HzTotalPeriod(225);
            status |= zl303xx_AprSetFastLock1HzDelay(185);

            status |= zl303xx_AprSetStepTimeAdjustTimeThreshold(500000);
            status |= zl303xx_AprSetStepTimeAdjustFreqThreshold(6000000);
            status |= zl303xx_AprSetPFStepTimeResolution(50);

            /* APR Freq overrides */
            {
                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 0,  12);
                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 14, 450);
                status |= exampleAprOverrideXParam(ZL303XX_TRUE, 32, 1000);

                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 50, 12);
                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 64, 400);
                status |= exampleAprOverrideXParam(ZL303XX_FALSE, 82, 1000);
            }

            /* APR 1Hz overrides */
            {
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 2, fwdPps * 10);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 5, ZL303XX_TRUE);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 6, 300);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 7, 20);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 10, 1);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 11, 1000000000);
                status |= exampleAprOverride1HzXParam(ZL303XX_TRUE, 23, 1200);

                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 2, revPps * 10);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 5, ZL303XX_TRUE);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 6, 300);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 7, 20);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 10, 1);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 11, 1000000000);
                status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 23, 1200);
            }
            break;


        case ACI_BASIC_PHASE:  /* ACI110 */
            /* Defaults */
            status |= exampleAprSetConfigDefaults();
            status |= zl303xx_AprSetPFStepTimeResolution( 100000 );

            /* APR */
            status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_TCXO_FAST);
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE);
            status |= zl303xx_AprSetAlgTypeMode(ZL303XX_BOUNDARY_CLK);
            status |= zl303xx_AprSetFilterType(ZL303XX_BW_8_FILTER);

           /* 1Hz: enabled */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_TRUE);
            status |= zl303xx_SetAdjustTimeMinThreshold(0);
            status |= zl303xx_AprSet1HzStartupTimeInit(ZL303XX_FALSE);
            status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 5, zl303xx_AprGet1HzStartupTimeInit());
            status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PERIODIC);
            status |= zl303xx_AprSet1HzRealignmentInterval(5);  /* seconds */
            status |= zl303xx_AprSetAllow1HzAdjustmentsInHoldover(ZL303XX_FALSE);
            status |= zl303xx_AprSetPFAdjustFreqMinPhase(0);

            break;

        case ACI_BASIC_PHASE_SYNCE:  /* ACI111 */
            /* Defaults */
            status |= exampleAprSetConfigDefaults();

            /* APR */
            status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_TCXO_FAST);
            status |= zl303xx_AprSetAlgTypeMode(ZL303XX_BOUNDARY_CLK);
            status |= zl303xx_AprSetFilterType(ZL303XX_BW_8_FILTER);

            /* hybrid: enabled */
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_HYBRID_MODE);
            status |= zl303xx_AprSetHybridServerFlag(ZL303XX_TRUE);

            /* 1Hz: enabled */
            /* NOTE: it is recommended that the realignment interval be set to 2s */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_TRUE);
            status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PERIODIC);
            status |= zl303xx_AprSet1HzRealignmentInterval(2);  /* seconds */
            status |= zl303xx_AprSetAllow1HzAdjustmentsInHoldover(ZL303XX_FALSE);

            status |= zl303xx_AprSet1HzFilterWriteInterval(125);
            status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PER_PACKET);
            status |= zl303xx_SetAdjustTimeMinThreshold(0);

            status |= zl303xx_AprSetPFAdjustFreqMinPhase(20);
            break;

        case ACI_BASIC_PHASE_LOW:  /* ACI113 */
            /* Defaults */
            status |= exampleAprSetConfigDefaults();
            status |= zl303xx_AprSetPFStepTimeResolution( 100000 );
            /* APR */
            status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_TCXO_FAST);
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE);
            status |= zl303xx_AprSetAlgTypeMode(ZL303XX_BOUNDARY_CLK);
            status |= zl303xx_AprSetFilterType(ZL303XX_BW_5_FILTER);

           /* 1Hz: enabled */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_TRUE);
            status |= zl303xx_SetAdjustTimeMinThreshold(0);
            status |= zl303xx_AprSet1HzStartupTimeInit(ZL303XX_FALSE);
            status |= exampleAprOverride1HzXParam(ZL303XX_FALSE, 5, zl303xx_AprGet1HzStartupTimeInit());
            status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PERIODIC);
            status |= zl303xx_AprSet1HzRealignmentInterval(5);  /* seconds */
            status |= zl303xx_AprSetAllow1HzAdjustmentsInHoldover(ZL303XX_FALSE);
            status |= zl303xx_AprSetPFAdjustFreqMinPhase(0);

            break;

        case ACI_BASIC_PHASE_LOW_SYNCE:  /* ACI114 */
            /* Defaults */
            status |= exampleAprSetConfigDefaults();

            /* APR */
            status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_TCXO_FAST);
            status |= zl303xx_AprSetAlgTypeMode(ZL303XX_BOUNDARY_CLK);
            status |= zl303xx_AprSetFilterType(ZL303XX_BW_5_FILTER);

            /* hybrid: enabled */
            status |= zl303xx_AprSetDeviceOptMode(ZL303XX_HYBRID_MODE);
            status |= zl303xx_AprSetHybridServerFlag(ZL303XX_TRUE);

            /* 1Hz: enabled */
            /* NOTE: it is recommended that the realignment interval be set to 2s */
            status |= zl303xx_AprSet1HzEnabled(ZL303XX_TRUE);
            status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PERIODIC);
            status |= zl303xx_AprSet1HzRealignmentInterval(2);  /* seconds */
            status |= zl303xx_AprSetAllow1HzAdjustmentsInHoldover(ZL303XX_FALSE);

            status |= zl303xx_AprSetPFAdjustFreqMinPhase(20);
            break;
        case ACI_LAST:
        default:
            ZL303XX_TRACE_ALWAYS("exampleAprSetConfigParameters: Unhandled configId=%d", configId, 0,0,0,0,0);
            status = ZL303XX_PARAMETER_INVALID;
            break;
        }
    }

    if (status != ZL303XX_OK)
    {
        ZL303XX_TRACE_ALWAYS("exampleAprSetConfigParameters: Setup for configId=%d; failure status=%d", configId, status, 0,0,0,0);
    }
    else{	/* Fujitsu modify */
		status = zl303xx_FJ_AprInitSetValue();
        ZL303XX_TRACE_ALWAYS("APR init status=%d", status, 0,0,0,0,0);
        status = ZL303XX_OK;
	}


    return( status );
}

/**

  Function Name:
    exampleAprSetConfigDefaults

  Details:
   Example code to reset global APR parameters to their defaults.

  Notes:
   Note this function only configures global settings. The global settings
   must then be used when configuring the APR device / server.

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE exampleAprSetConfigDefaults(void)
{
    zlStatusE status = ZL303XX_OK;

    /* Enable type 2b pll on a per device type basis */
    if (zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE)
    {
        type_2b_enabled = ZL303XX_TRUE;
    }

    /* All ACIs are bi-directional */
    status |= zl303xx_AprSetUseReversePath(ZL303XX_TRUE);

    /* APR settings */
    /* status |= zl303xx_AprSetDeviceOptMode(ZL303XX_PACKET_MODE); - Defaults to ZL303XX_PACKET_MODE */
    status |= zl303xx_AprSetAlgTypeMode(ZL303XX_NATIVE_PKT_FREQ);
    status |= zl303xx_AprSetOscillatorFilterType(ZL303XX_TCXO);
    status |= zl303xx_AprSetHoldoverStability(ZL303XX_USING_DEFAULT_HOLDOVER_STABILITY);
    status |= zl303xx_AprSetFilterType(ZL303XX_BW_0_FILTER);

    status |= zl303xx_AprSetHybridServerFlag(ZL303XX_FALSE);

    /* APR-1Hz settings */
    status |= zl303xx_AprSet1HzEnabled(ZL303XX_FALSE);
    status |= zl303xx_AprSet1HzRealignmentType(ZL303XX_1HZ_REALIGNMENT_PERIODIC);
    status |= zl303xx_AprSet1HzRealignmentInterval(120);  /* seconds */

    status |= zl303xx_AprSetFastLock1HzEnable(ZL303XX_FALSE);
    status |= zl303xx_AprSetFastLock1HzInterval(0);
    status |= zl303xx_AprSetFastLock1HzDelay(0);
    status |= zl303xx_AprSetStepTimeAdjustTimeThreshold(500000);
    status |= zl303xx_AprSetStepTimeAdjustFreqThreshold(500000);

    zl303xx_AprSetAPRFrequencyLockedPhaseSlopeLimit(ZL303XX_DEFAULT_APR_FREQ_PSL);
    zl303xx_AprSetAPRFrequencyNotLockedPhaseSlopeLimit(ZL303XX_DEFAULT_APR_FREQ_PSL);
    zl303xx_AprSetAPRFrequencyFastPhaseSlopeLimit(ZL303XX_DEFAULT_APR_FREQ_FAST_PSL);

    zl303xx_AprSetAPRFrequencyLockedFrequencyChangeLimit(APR_FCL_MAX);
    zl303xx_AprSetAPRFrequencyNotLockedFrequencyChangeLimit(APR_FCL_MAX);
    zl303xx_AprSetAPRFrequencyFastFrequencyChangeLimit(APR_FCL_MAX);

    zl303xx_AprSetPSL(0, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_PSL_0, ZL303XX_DEFAULT_PSL_1HZ_0 );
    zl303xx_AprSetPSL(1, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_PSL_1, ZL303XX_DEFAULT_PSL_1HZ_1 );
    zl303xx_AprSetPSL(2, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_PSL_2, ZL303XX_DEFAULT_PSL_1HZ_2 );
    zl303xx_AprSetPSL(3, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_PSL_3, ZL303XX_DEFAULT_PSL_1HZ_3 );
    zl303xx_AprSetPSL(4, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_PSL_4, ZL303XX_DEFAULT_PSL_1HZ_4 );

    zl303xx_AprSetFCL(0, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_FCL_0, ZL303XX_DEFAULT_FCL_1HZ_0 );
    zl303xx_AprSetFCL(1, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_FCL_1, ZL303XX_DEFAULT_FCL_1HZ_1 );
    zl303xx_AprSetFCL(2, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_FCL_2, ZL303XX_DEFAULT_FCL_1HZ_2 );
    zl303xx_AprSetFCL(3, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_FCL_3, ZL303XX_DEFAULT_FCL_1HZ_3 );
    zl303xx_AprSetFCL(4, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_FCL_4, ZL303XX_DEFAULT_FCL_1HZ_4 );

    zl303xx_AprSetAdjScaling(0, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_ADJ_SCALING_0, ZL303XX_DEFAULT_ADJ_SCALING_1HZ_0);
    zl303xx_AprSetAdjScaling(1, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_ADJ_SCALING_1, ZL303XX_DEFAULT_ADJ_SCALING_1HZ_1);
    zl303xx_AprSetAdjScaling(2, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_ADJ_SCALING_2, ZL303XX_DEFAULT_ADJ_SCALING_1HZ_2);
    zl303xx_AprSetAdjScaling(3, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_ADJ_SCALING_3, ZL303XX_DEFAULT_ADJ_SCALING_1HZ_3);
    zl303xx_AprSetAdjScaling(4, ZL303XX_DEFAULT_ADJ_SIZE_1HZ_ADJ_SCALING_4, ZL303XX_DEFAULT_ADJ_SCALING_1HZ_4);

    status |= zl303xx_AprSetPFLockInThreshold(1000);
    status |= zl303xx_AprSetPFLockInCount(2);
    status |= zl303xx_AprSetPFLockOutThreshold(2000);
    status |= zl303xx_AprSetPFAdjustFreqMinPhase(20);

    status |= zl303xx_AprSet1HzStartupTimeInit(ZL303XX_TRUE);

    status |= zl303xx_AprSetEnterPhaseLockStatusThreshold(ZL303XX_ENTER_PHASE_LOCK_STATUS_THRESHOLD);
    status |= zl303xx_AprSetExitPhaseLockStatusThreshold(ZL303XX_EXIT_PHASE_LOCK_STATUS_THRESHOLD);

    /* Reset xParam overrides to defaults */
    status |= exampleAprOverrideXParamResetAll(ZL303XX_TRUE);
    status |= exampleAprOverrideXParamResetAll(ZL303XX_FALSE);
    status |= exampleAprOverride1HzXParamResetAll(ZL303XX_TRUE);
    status |= exampleAprOverride1HzXParamResetAll(ZL303XX_FALSE);

    /* Enable Soft Reset */
    /* status |= zl303xx_SetAprUserHandlesA_90000Reset(ZL303XX_TRUE); */

    /* Set default APR task rate (500 ms default)
       This value must be a multiple of the PSLFCL task period */
    status |= zl303xx_AprSetAprTaskBasePeriodMs(ZL303XX_APR_TIMER1_PERIOD_MS);

    /* Set default PSLFCL task rate (125 ms default) */
    status |= zl303xx_AprSetPslFclTaskBasePeriodMs(ZL303XX_APR_TIMER2_PERIOD_MS);

    /* Set the NCO-assist value to FALSE  */
    status |= zl303xx_AprSetUseNCOAssist(ZL303XX_FALSE);

    return status;
}


#endif  /* APR_INCLUDED */

#if defined ZLS30390_INCLUDED
/* examplePtpTsLoggingCtrl */
/**
   This routine allows timestamp info to be gathered from PTP for a number of
   seconds and sent to a user function for processing.

  Parameters:
   [in]  serverId          the PTP stream handle
   [in]  secondsToLog      how many seconds to collect timestamps
   [in]  userCalloutFn     pointer to the user function to handle the TS

  Return Value: zlStatusE

*******************************************************************************/
zlStatusE examplePtpTsLoggingCtrl(Uint32T serverId, Uint32T secondsToLog,
                                  swFuncPtrTSLogging userCalloutFn)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_PtpTsLogConfigS logCfg;

    if (status == ZL303XX_OK)
    {
        status = zl303xx_PtpTsLogConfigStructInit(&logCfg);
    }

    if (status == ZL303XX_OK)
    {
        logCfg.durationSec = secondsToLog;
        logCfg.callback = userCalloutFn;

        status = zl303xx_PtpTsLogConfigSet(serverId, &logCfg);
    }

    return status;
}
#endif  /* ZLS30390_ */

/*

  Function Name:
    exampleTSLogging

  Details:
   Example code to handle timestamp logging.

   This routine allows the user to output timestamps for a server.
   You can output them in any order to anywhere you want.
   Currently output to stdout in CSV format if no tsLogFilename selected.

   Note: TS data is recorded BEFORE applying the correction field.

  Return Value:
   ZL303XX_OK

 *******************************************************************************/
Sint32T exampleTSLogging (zl303xx_TsLogDataS *tsLogData);
Sint32T exampleTSLogging (zl303xx_TsLogDataS *tsLogData)
{
    zlStatusE status = ZL303XX_OK;

    if (strlen(tsLogFilename))
    {
        if ((tsLogFileP = fopen(tsLogFilename, "a+")))
        {
            fprintf(tsLogFileP, "%d,%d,%d, %d,%d,%d, %d,%d,%d, %d,%d\n",
                    tsLogData->streamNum, tsLogData->fwd, tsLogData->sequenceNum, tsLogData->txTs.hi, tsLogData->txTs.lo, tsLogData->txSubSec,
                    tsLogData->rxTs.hi, tsLogData->rxTs.lo, tsLogData->rxSubSec, tsLogData->corr.hi, tsLogData->corr.lo);
            fclose(tsLogFileP);
        }
        else
        {
            ZL303XX_TRACE_ALWAYS("exampleTSLogging: fopen() of %s returned NULL", tsLogFilename, 0,0,0,0,0);
            status = ZL303XX_IO_ERROR;
        }
    }
    else
    {
        fprintf(stdout, "%d,%d,%d, %d,%d,%d, %d,%d,%d, %d,%d\n",
                tsLogData->streamNum, tsLogData->fwd, tsLogData->sequenceNum, tsLogData->txTs.hi, tsLogData->txTs.lo, tsLogData->txSubSec,
                tsLogData->rxTs.hi, tsLogData->rxTs.lo, tsLogData->rxSubSec, tsLogData->corr.hi, tsLogData->corr.lo);
    }

    return status;
}

/**

  Function Name:
   exampleEnableTSLoggingOnServer

  Details:
   Example code to enable timestamp logging.

   This routine allows the user to log timestamps (TS) on-the-fly for a server.
   Note: TS data is extracted prior to applying the correction field.

  Parameters:
   [in]   serverId       - the server ID to log
   [in]   secondsToLog   - how long to log (in seconds)

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE exampleEnableTSLoggingOnServer (Uint32T serverId, Sint32T secondsToLog)
{
    zlStatusE status = ZL303XX_OK;
    static const char fnName[] = "exampleEnableTSLoggingOnServer";

#if defined APR_INCLUDED
    if (status == ZL303XX_OK && zl303xx_Params0)
    {
        status = exampleAprTsLoggingCtrl(zl303xx_Params0, serverId, secondsToLog, exampleTSLogging);
        if (status == ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("%s: TimeStamp logging enabled on serverId=%d for approx. %d seconds",
                               fnName, serverId, secondsToLog, 0,0,0);
            ZL303XX_TRACE_ALWAYS("Flow#,FWD,Seq, TxH,L,Ns, RxH,L,Ns, CorrH,L", 0,0,0,0,0,0);
        }
        else
        {
            ZL303XX_TRACE_ALWAYS("%s: Call to exampleAprTsLoggingCtrl() failed=%d",
                               fnName, status, 0,0,0,0);
        }
    }
#elif defined ZLS30390_INCLUDED
    if (status == ZL303XX_OK)
    {
        status = examplePtpTsLoggingCtrl(serverId, secondsToLog, exampleTSLogging);
        if (status == ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("%s: TimeStamp logging enabled on serverId=%d for approx. %d seconds",
                               fnName, serverId, secondsToLog, 0,0,0);
            ZL303XX_TRACE_ALWAYS("Flow#,FWD,Seq, TxH,L,Ns, RxH,L,Ns, CorrH,L", 0,0,0,0,0,0);
        }
        else
        {
            ZL303XX_TRACE_ALWAYS("%s: Call to examplePtpTsLoggingCtrl() failed=%d",
                               fnName, status, 0,0,0,0);
        }
    }
#endif

    return status;
}

/**

  Function Name:
   exampleDisableTSLoggingOnServer

  Details:
   Example code to disable timestamp logging.

   This routine allows the user to disable logging timestamps on-the-fly for a server.
  Parameters:
   [in]   serverId       - the server ID to log

  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE exampleDisableTSLoggingOnServer(Uint32T serverId)
{
    zlStatusE status = ZL303XX_OK;
    static const char fnName[] = "exampleDisableTSLoggingOnServer";

#if defined APR_INCLUDED
    if (status == ZL303XX_OK && zl303xx_Params0)
    {
        status = exampleAprTsLoggingCtrl(zl303xx_Params0, serverId, 0, NULL);
        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("%s: Call to exampleAprTsLoggingCtrl()  failed=%d",
                               fnName, status, 0,0,0,0);
        }
    }
#elif defined ZLS30390_INCLUDED
    if (status == ZL303XX_OK)
    {
        status = examplePtpTsLoggingCtrl(serverId, 0, NULL);
        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("%s: Call to examplePtpTsLoggingCtrl() failed=%d",
                               fnName, status, 0,0,0,0);
        }
    }
#endif

    return status;
}


#if defined APR_INCLUDED
/**

  Function Name:
    exampleAprSet1HzEnabled

  Details:
    Example code to enable/disable APR's 1Hz module for all the specified device.

    If the hwParams APR device is NULL or has not been started yet then only
    the global setting is configured.

  Notes:
    This only enables/disables 1Hz and does not configure 1Hz using global
    settings, see exampleConfigure1Hz().

  Parameters:
   [in]   hwParams       Pointer to the device instance (can be NULL)
   [in]   bEnable1Hz     Flag used to enable/disable 1Hz for the device.


  Return Value:
   zlStatusE

 *******************************************************************************/
zlStatusE exampleAprSet1HzEnabled(void *hwParams, zl303xx_BooleanE enable)
{
    zlStatusE status = ZL303XX_OK;

    if (status == ZL303XX_OK)
    {
        /* Set global */
        if ((status = zl303xx_AprSet1HzEnabled(enable)) != ZL303XX_OK)
            ZL303XX_TRACE_ALWAYS("exampleAprSet1HzEnabled: zl303xx_AprSet1HzEnabled() failed", status, 0,0,0,0,0);
    }

    if (hwParams != NULL)
    {
        /* Configure 1Hz on the running device */
        if (status == ZL303XX_OK && zl303xx_AprIsAprRunning() != ZL303XX_TRUE)
        {
            status = ZL303XX_NOT_RUNNING;
        }

        if (status == ZL303XX_OK)
        {
            if ((status = zl303xx_AprSetDevice1HzEnabled(hwParams, enable)) != ZL303XX_OK)
                ZL303XX_TRACE_ALWAYS("exampleAprSet1HzEnabled: zl303xx_AprSetDevice1HzEnabled() failed", status, 0,0,0,0,0);
        }
    }

    return status;
}


zlStatusE exampleGetHoldoverAndWarmStartServer(void *hwParams, Uint16T serverNum);
zlStatusE exampleGetHoldoverAndWarmStartServer(void *hwParams, Uint16T serverNum)
{
    zlStatusE status = ZL303XX_OK;
    Sint32T holdoverValue = 0;

    if (serverNum == 65535)
    {
        status = ZL303XX_INVALID_OPERATION;
        ZL303XX_TRACE_ALWAYS( "exampleGetHoldoverAndWarmStartServer: Server=%d is not valid; status=%d",
                            serverNum, status, 0,0,0,0);
    }

    /* The warmstart value can be: */
    /* Current DF */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_AprGetCurrentDF(hwParams, serverNum, &holdoverValue);
        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS( "exampleGetHoldoverAndWarmStartServer: Get Server DF failed=%d",
                                status, 0,0,0,0,0);
        }
        else
        {
            ZL303XX_TRACE_ALWAYS("Server-specific DF=%d\n", holdoverValue, 0,0,0,0,0);
        }
    }

    /* OR the device holdover (current active serverNum is used) */
    if (status == ZL303XX_OK)
    {
        zl303xx_BooleanE holdoverV;
        status = zl303xx_AprGetCurrentHoldoverValue(hwParams, &holdoverValue, &holdoverV);
        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS( "exampleGetHoldoverAndWarmStartServer: Get Device HoldoverValue failed=%d",
                                status, 0,0,0,0,0);
        }
        else
        {
            ZL303XX_TRACE_ALWAYS("Device holdoverValue=%d\n", holdoverValue, 0,0,0,0,0);
        }
    }

    /* OR this value is found on a particular server */
    if (status == ZL303XX_OK)
    {
        zl303xx_BooleanE holdoverV;
        status = zl303xx_AprGetServerHoldoverValue(hwParams, serverNum, &holdoverValue, &holdoverV);
        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS( "exampleGetHoldoverAndWarmStartServer: Get Server HoldoverValue failed=%d",
                                status, 0,0,0,0,0);
        }
        else
        {
            ZL303XX_TRACE_ALWAYS("Server holdoverValue=%d\n", holdoverValue, 0,0,0,0,0);
        }
    }

    /* Warmstart server with offset value */
    if (status == ZL303XX_OK)
    {
        status = zl303xx_AprWarmStartServer(hwParams, serverNum, holdoverValue);
        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS( "exampleGetHoldoverAndWarmStartServer: Warm start with HoldoverValue failed=%d",
                                status, 0,0,0,0,0);
        }
    }

    return status;
}




#if defined ZLS30390_INCLUDED

/* Default the array of PLL overrides booleans to ZL303XX_FALSE */
zl303xx_BooleanE zl303xx_ClockSettlingTimeActive[ZL303XX_PTP_NUM_CLOCKS_MAX] = {ZL303XX_FALSE};
#define TMR_ID_COL 0    /* timer ID */
#define RT_SIG_COL 1    /* Real-time signal number */
#define TMRMAX_COL 2
Uint32T zl303xx_ClockSettlingTimers[ZL303XX_PTP_NUM_CLOCKS_MAX][TMRMAX_COL] = {{0, 0}};   /* {TMR_ID_COL, RT_SIG_COL} */

/** exampleSetPtpPLLSettlingTimeOverride

An example of how to override a specific flag for a PLL indicating that a reference switch
   or other event may have recently occurred and that the current reference should be
   viewed as LOCKED. Once this flag goes to FALSE, the actual state of the PLL will be
   used internally.

  Parameters:
   [in]   clockHandle               The clock index into the array of clocks.
   [in]   clockSettlingTimeActiveB  The override boolean.


  Return Value:     ZL303XX_OK or ZL303XX_TABLE_ENTRY_NOT_FOUND

*******************************************************************************/
zlStatusE exampleSetPtpPLLSettlingTimeOverride(zl303xx_PtpClockHandleT clockHandle, zl303xx_BooleanE clockSettlingTimeActiveB)
{
    zlStatusE status = ZL303XX_OK;

    if (clockHandle >= ZL303XX_PTP_NUM_CLOCKS_MAX)
    {
        status = ZL303XX_TABLE_FULL;
        ZL303XX_TRACE_ALWAYS("exampleSetPtpPLLSettlingTimeOverride(): provided clockHandle=%d exceeds max clocks=%d failure=%d ", clockHandle, ZL303XX_PTP_NUM_CLOCKS_MAX, status, 0,0,0);
    }
    else
    {
        zl303xx_ClockSettlingTimeActive[clockHandle] = clockSettlingTimeActiveB;
        ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
                    "exampleSetPtpPLLSettlingTimeOverride(): clockHandle=%d; ClockSettlingTimeActive=%d ", clockHandle, zl303xx_ClockSettlingTimeActive[clockHandle], 0,0,0,0);
    }

    return status;
}

/** exampleGetPtpPLLSettlingTimeOverride

Get the SettlingTimeOverride flag override for a specific PLL.

  Parameters:
   [in]   clockHandle               The clock index into the array of clocks.


  Return Value:     ZL303XX_TRUE or ZL303XX_FALSE or ZL303XX_FALSE on error

*******************************************************************************/
zl303xx_BooleanE exampleGetPtpPLLSettlingTimeOverride(zl303xx_PtpClockHandleT clockHandle)
{
    zlStatusE status = ZL303XX_OK;

    if (clockHandle >= ZL303XX_PTP_NUM_CLOCKS_MAX)
    {
        status = ZL303XX_TABLE_FULL;
        ZL303XX_TRACE_ALWAYS("exampleGetPtpPLLSettlingTimeOverride(): provided clockHandle=%d exceeds max clocks=%d failure=%d ", clockHandle, ZL303XX_PTP_NUM_CLOCKS_MAX, status, 0,0,0);
        return ZL303XX_FALSE;
    }
    else
    {
        ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
                    "exampleGetPtpPLLSettlingTimeOverride(): clockHandle=%d; ClockSettlingTimeActive=%d ", clockHandle, zl303xx_ClockSettlingTimeActive[clockHandle], 0,0,0,0);
        return zl303xx_ClockSettlingTimeActive[clockHandle];
    }
}

/* Settling timer handler function */
static void examplePtpSettlingTimerFn( Sint32T rtSigNum, Sint32T arg);
static void examplePtpSettlingTimerFn( Sint32T rtSigNum, Sint32T unused)
{
    zlStatusE status = ZL303XX_OK;
    Uint8T clockHandle;

    (void) unused; /* Warning removal */

    for (clockHandle = 0; clockHandle < ZL303XX_PTP_NUM_CLOCKS_MAX; clockHandle++)
    {
        if (zl303xx_ClockSettlingTimers[clockHandle][RT_SIG_COL] == (Uint32T)rtSigNum)
        {
            if ((status = exampleSetPtpPLLSettlingTimeOverride(clockHandle, ZL303XX_FALSE)) != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("examplePtpSettlingTimerFn(): Call to exampleSetPtpPLLSettlingTimeOverride() for clockHandle=%d failed=%d", clockHandle, status ,0,0,0,0);
            }
            else
            {
                zl303xx_ClockSettlingTimers[clockHandle][RT_SIG_COL] = 0;
                zl303xx_ClockSettlingTimers[clockHandle][TMR_ID_COL] = 0;
                ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
                            "examplePtpSettlingTimerFn(): rtSigNum=%d FIRED, FOUND and ClockSettlingTimeActive CLEARED at timerIndx=%d", rtSigNum, clockHandle, 0,0,0,0);
            }
        }
    }
}

/** examplePtpSettlingTimerSet

Sets the SettlingTimeOverride flag override for a specific PLL.
If it is to be active, then it starts a clock-specific timer to stay in "setting" mode until the timer expires

  Parameters:
   [in]   clockHandle               The clock index into the array of clocks.
   [in]   timeInSecs                Duration of the settling timer for this clockHandle.
   [in]   timerActiveB              Enable or disable the timer (and SettlingTimeOverride boolean) for this clockHandle.

  Return Value:     status of error or ZL303XX_OK

*******************************************************************************/
zlStatusE examplePtpSettlingTimerSet(zl303xx_PtpClockHandleT clockHandle, Uint32T timeInSecs, zl303xx_BooleanE timerActiveB)
{
    zlStatusE status = ZL303XX_OK;
    Uint32T timeInMsSecs = timeInSecs *1000;

    if ( timeInMsSecs >> 31)
    {
        ZL303XX_TRACE_ALWAYS("examplePtpSettlingTimerSet(): ms=%d is approaching overflow - check it; Continuing", timeInMsSecs, 0,0,0,0,0);
    }

    if (clockHandle >= ZL303XX_PTP_NUM_CLOCKS_MAX)
    {
        status = ZL303XX_TABLE_FULL;
        ZL303XX_TRACE_ALWAYS("examplePtpSettlingTimerSet(): provided clockHandle=%d exceeds max clocks=%d failure=%d ", clockHandle, ZL303XX_PTP_NUM_CLOCKS_MAX, status, 0,0,0);
    }

    if (status == ZL303XX_OK)
    {
        if (timerActiveB == ZL303XX_TRUE)
        {
            if ( zl303xx_ClockSettlingTimers[clockHandle][TMR_ID_COL] == 0 && zl303xx_ClockSettlingTimers[clockHandle][RT_SIG_COL] == 0 )  /* Setup a new one-shot timer for this clockhandle */ /* rtsignal = ZLCSTTIMERSIG -clockHandle */
            {
                if ((status = zl303xx_SetHWTimer((ZLCSTTIMERSIG -clockHandle), (timer_t*)&zl303xx_ClockSettlingTimers[clockHandle][TMR_ID_COL], (timeInSecs *1000), &examplePtpSettlingTimerFn, ZL303XX_FALSE)) != ZL303XX_OK)
                {
                    ZL303XX_TRACE_ALWAYS("examplePtpSettlingTimerSet(): Call to zl303xx_SetHWTimer() for clockHandle=%d failed=%d", clockHandle, status ,0,0,0,0);
                }
                else
                {
                    if ((status = exampleSetPtpPLLSettlingTimeOverride(clockHandle, ZL303XX_TRUE)) != ZL303XX_OK)
                    {
                        ZL303XX_TRACE_ALWAYS("examplePtpSettlingTimerSet(): Call to exampleSetPtpPLLSettlingTimeOverride() for clockHandle=%d failed=%d", clockHandle, status ,0,0,0,0);
                    }
                    else
                    {
                        zl303xx_ClockSettlingTimers[clockHandle] [RT_SIG_COL]= (ZLCSTTIMERSIG -clockHandle);
                        ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
                                    "examplePtpSettlingTimerSet(): rtsignal=%d for timerid=%d STORED at clockHandle=%d expires in sec=%d", (ZLCSTTIMERSIG -clockHandle), zl303xx_ClockSettlingTimers[clockHandle], clockHandle, timeInSecs, 0,0);
                    }
                }
            }
            else
            {
                status = ZL303XX_TABLE_ENTRY_DUPLICATE;
                ZL303XX_TRACE_ALWAYS("examplePtpSettlingTimerSet(): Timer for clockHandle=%d already exists. status=%d", clockHandle, status, 0,0,0,0);
            }
        }
        else /* Cancel timer and SettlingTimeActive */
        {
            if (zl303xx_ClockSettlingTimers[clockHandle][TMR_ID_COL] == 0 || zl303xx_ClockSettlingTimers[clockHandle][RT_SIG_COL] == 0)
            {
                status = ZL303XX_TABLE_ENTRY_NOT_FOUND;
                ZL303XX_TRACE_ALWAYS("examplePtpSettlingTimerSet(): No timer at clockHandle=%d to cancel. status=%d", clockHandle, status, 0,0,0,0);
            }
            else
            {   /* rtsignal = ZLCSTTIMERSIG -clockHandle */
                if ((status = zl303xx_DeleteHWTimer((ZLCSTTIMERSIG -clockHandle), (timer_t*)&zl303xx_ClockSettlingTimers[clockHandle][TMR_ID_COL])) != ZL303XX_OK)
                {
                    ZL303XX_TRACE_ALWAYS("examplePtpSettlingTimerSet(): Call to zl303xx_DeleteHWTimer() for clockHandle=%d failed=%d", clockHandle, status ,0,0,0,0);
                }
                else
                {
                    if ((status = exampleSetPtpPLLSettlingTimeOverride(clockHandle, ZL303XX_FALSE)) != ZL303XX_OK)
                    {
                        ZL303XX_TRACE_ALWAYS("examplePtpSettlingTimerSet(): Call to exampleSetPtpPLLSettlingTimeOverride() for clockHandle=%d failed=%d", clockHandle, status ,0,0,0,0);
                    }
                    else
                    {
                        zl303xx_ClockSettlingTimers[clockHandle][RT_SIG_COL] = 0; /* zl303xx_DeleteHWTimer() cleared TMR_ID_COL */
                        ZL303XX_TRACE(ZL303XX_MOD_ID_G781, 2,
                                    "examplePtpSettlingTimerSet(): rtsignal=%d timer DELETED at clockHandle=%d", (ZLCSTTIMERSIG -clockHandle), clockHandle, 0,0,0,0);
                    }
                }
            }
        }
    }

    return status;
}
#endif

#endif  /* APR_INCLUDED */


/*****************   END   ****************************************************/

