

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Global variables for use in examples - APR
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"  /* This should always be the first include file */
#include "zl303xx_Trace.h"


#if defined APR_INCLUDED
#include "zl303xx_Apr.h"
#include "zl303xx_Apr1Hz.h"
#endif

#include "zl303xx_ExampleAprGlobals.h"

/*****************   DATATYPES   **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/


/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   APR FUNCTION DEFINITIONS   **************************/


/******************************************************************************
                              AddDevice() start
*******************************************************************************/

/* Use getters and setters to access following static globals */
static zl303xx_Apr1HzRealignmentTypeE default1HzRealignmentType = ZL303XX_1HZ_REALIGNMENT_PERIODIC;
static zl303xx_AprPerPacketAdjTypeE defaultPerPacketAdjType = ZL303XX_PER_PACKET_ADJ_HYBRID_PHASE;
static Uint32T default1HzRealignmentInterval = 120; /* seconds */
Sint32T defaultStaticOffset = 0;
Uint32T default1HzFilterBandwidth = 100000;  /* uHz */
Uint32T default1HzFilterWriteInterval = 125;  /* ms */
zl303xx_AprDeviceRefModeE defaultDeviceOptMode = ZL303XX_PACKET_MODE;
zl303xx_AprDeviceHybridAdjModeE defaultDeviceHybridAdjMode = ZL303XX_HYBRID_ADJ_NONE;
zl303xx_BooleanE zl303xx_AprDcs = ZL303XX_FALSE;
Uint32T defaultHwDcoResolutionInPpt = 1;
Uint32T defaultEnterPhaseLockStatusThreshold = ZL303XX_ENTER_PHASE_LOCK_STATUS_THRESHOLD;
Uint32T defaultExitPhaseLockStatusThreshold = ZL303XX_EXIT_PHASE_LOCK_STATUS_THRESHOLD;
zl303xx_BooleanE defaultWarmStartFlag = ZL303XX_FALSE;
Sint32T defaultWsInitFreqOffset = 0;
zl303xx_AprPhaseSlopeLimitE defaultAPRFrequencyLockedPhaseSlopeLimit = ZL303XX_DEFAULT_APR_FREQ_PSL;
zl303xx_AprPhaseSlopeLimitE defaultAPRFrequencyNotLockedPhaseSlopeLimit = ZL303XX_DEFAULT_APR_FREQ_PSL;
zl303xx_AprPhaseSlopeLimitE defaultAPRFrequencyFastPhaseSlopeLimit = ZL303XX_DEFAULT_APR_FREQ_FAST_PSL;
zl303xx_AprFrequencyChangeLimitE defaultAPRFrequencyLockedFrequencyChangeLimit = ZL303XX_DEFAULT_APR_FREQ_FCL;
zl303xx_AprFrequencyChangeLimitE defaultAPRFrequencyNotLockedFrequencyChangeLimit = ZL303XX_DEFAULT_APR_FREQ_FCL;
zl303xx_AprFrequencyChangeLimitE defaultAPRFrequencyFastFrequencyChangeLimit = ZL303XX_DEFAULT_APR_FREQ_FCL;
Uint32T defaultAdjSize1HzPSL[ZL303XX_MAX_NUM_PSL_LIMITS] = {
   ZL303XX_DEFAULT_ADJ_SIZE_1HZ_PSL_0,
   ZL303XX_DEFAULT_ADJ_SIZE_1HZ_PSL_1,
   ZL303XX_DEFAULT_ADJ_SIZE_1HZ_PSL_2,
   ZL303XX_DEFAULT_ADJ_SIZE_1HZ_PSL_3,
   ZL303XX_DEFAULT_ADJ_SIZE_1HZ_PSL_4
   };
Uint32T defaultPSL_1Hz[ZL303XX_MAX_NUM_PSL_LIMITS] = {
   ZL303XX_DEFAULT_PSL_1HZ_0,
   ZL303XX_DEFAULT_PSL_1HZ_1,
   ZL303XX_DEFAULT_PSL_1HZ_2,
   ZL303XX_DEFAULT_PSL_1HZ_3,
   ZL303XX_DEFAULT_PSL_1HZ_4
   };
Uint32T defaultAdjSize1HzFCL[ZL303XX_MAX_NUM_FCL_LIMITS] = {
   ZL303XX_DEFAULT_ADJ_SIZE_1HZ_FCL_0,
   ZL303XX_DEFAULT_ADJ_SIZE_1HZ_FCL_1,
   ZL303XX_DEFAULT_ADJ_SIZE_1HZ_FCL_2,
   ZL303XX_DEFAULT_ADJ_SIZE_1HZ_FCL_3,
   ZL303XX_DEFAULT_ADJ_SIZE_1HZ_FCL_4
   };
Uint32T defaultFCL_1Hz[ZL303XX_MAX_NUM_FCL_LIMITS] = {
   ZL303XX_DEFAULT_FCL_1HZ_0,
   ZL303XX_DEFAULT_FCL_1HZ_1,
   ZL303XX_DEFAULT_FCL_1HZ_2,
   ZL303XX_DEFAULT_FCL_1HZ_3,
   ZL303XX_DEFAULT_FCL_1HZ_4
   };
Uint32T defaultAdjSize1HzAdjScaling[ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS] = {
   ZL303XX_DEFAULT_ADJ_SIZE_1HZ_ADJ_SCALING_0,
   ZL303XX_DEFAULT_ADJ_SIZE_1HZ_ADJ_SCALING_1,
   ZL303XX_DEFAULT_ADJ_SIZE_1HZ_ADJ_SCALING_2,
   ZL303XX_DEFAULT_ADJ_SIZE_1HZ_ADJ_SCALING_3,
   ZL303XX_DEFAULT_ADJ_SIZE_1HZ_ADJ_SCALING_4
   };
Uint32T defaultAdjScaling_1Hz[ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS] = {
   ZL303XX_DEFAULT_ADJ_SCALING_1HZ_0,
   ZL303XX_DEFAULT_ADJ_SCALING_1HZ_1,
   ZL303XX_DEFAULT_ADJ_SCALING_1HZ_2,
   ZL303XX_DEFAULT_ADJ_SCALING_1HZ_3,
   ZL303XX_DEFAULT_ADJ_SCALING_1HZ_4
   };

static Uint32T defaultPFLockInThreshold = 1000;
static Uint32T defaultPFLockInCount = 2;
static Uint32T defaultPFLockOutThreshold = 2000;
       Uint32T defaultMaxAdjFreqTime = 1200;
Sint32T defaultPFAdjustFreqMinPhase = 20;
Sint32T defaultThresholdForFlagV = -1;
zl303xx_PacketTreatmentE defaultSetTimePacketTreatment = PT_detect;
zl303xx_PacketTreatmentE defaultStepTimePacketTreatment = PT_detect;
zl303xx_PacketTreatmentE defaultAdjustTimePacketTreatment = PT_detect;
zl303xx_BooleanE defaultLegacyTreatment = ZL303XX_TRUE;
zl303xx_BooleanE exampleUseLegacyStreamStartUp = ZL303XX_TRUE;
zl303xx_BooleanE exampleAllow1HzAdjustmentsInHoldover = ZL303XX_FALSE;
#if defined ZLS30361_INCLUDED
   Uint32T defaultPFStepTimeResolution = 125000;
#elif defined ZLS30721_INCLUDED
   Uint32T defaultPFStepTimeResolution = 125000;
#elif defined ZLS30701_INCLUDED
   Uint32T defaultPFStepTimeResolution = 125000;
#elif defined ZLS30751_INCLUDED
   Uint32T defaultPFStepTimeResolution = 125000;
#elif defined ZLS30771_INCLUDED
   Uint32T defaultPFStepTimeResolution = 125000;
#else
   Uint32T defaultPFStepTimeResolution = 500000;
#endif

   Uint32T defaultPFLockedPhaseOutlierThreshold = 600;
   Sint32T defaultPFInitialFrequencyOffset      = 0;

zl303xx_BooleanE exampleOnlyAllowSteps = ZL303XX_FALSE;

/** zl303xx_AprSetDeviceOptMode

   The function sets the default device reference mode for APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetDeviceOptMode(zl303xx_AprDeviceRefModeE devOptMode)
{
    defaultDeviceOptMode = devOptMode;
    return ZL303XX_OK;
}
/** zl303xx_AprGetDeviceOptMode

   The function returns the default device reference mode for APR.

  Return Value: zl303xx_AprDeviceRefModeE       Device Reference Type

****************************************************************************/
zl303xx_AprDeviceRefModeE zl303xx_AprGetDeviceOptMode(void)
{
    return defaultDeviceOptMode;
}

/** zl303xx_AprSetDeviceHybridAdjMode

The function sets the default device reference mode for APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetDeviceHybridAdjMode(zl303xx_AprDeviceHybridAdjModeE deviceHybridAdjMode)
{
    defaultDeviceHybridAdjMode = deviceHybridAdjMode;
    return ZL303XX_OK;
}
/** zl303xx_AprGetDeviceHybridAdjMode

The function returns the default device reference mode for APR.

  Return Value: zl303xx_AprDeviceHybridAdjModeE       Device Hybrid Adjustment Type

****************************************************************************/
zl303xx_AprDeviceHybridAdjModeE zl303xx_AprGetDeviceHybridAdjMode(void)
{
    return defaultDeviceHybridAdjMode;
}

/** zl303xx_AprSetDcs

   The function is for future use.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetDcs(zl303xx_BooleanE dcs)
{
    zl303xx_AprDcs = dcs;
    return ZL303XX_OK;
}
/** zl303xx_AprGetDcs

   The function is for future use.

  Return Value: zlStatusE

****************************************************************************/
zl303xx_BooleanE zl303xx_AprGetDcs(void)
{
    return zl303xx_AprDcs;
}


/** zl303xx_AprSetHwDcoResolution

   The function sets the default device hardware DCO resolution for APR.
   Unit: ppt

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetHwDcoResolution(Uint32T hwDcoResolution)
{
    defaultHwDcoResolutionInPpt = hwDcoResolution;
    return ZL303XX_OK;
}
/** zl303xx_AprGetHwDcoResolution

   The function returns the default device hardware DCO resolution for APR.

  Return Value: Uint32T   Hardware DCO resolution in ppt

****************************************************************************/
Uint32T zl303xx_AprGetHwDcoResolution(void)
{
    return defaultHwDcoResolutionInPpt;
}


/** zl303xx_AprSetEnterPhaseLockStatusThreshold

   The function sets the default enter phase lock status threshold for APR.
   Unit: ns

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetEnterPhaseLockStatusThreshold(Uint32T threshold)
{
    defaultEnterPhaseLockStatusThreshold = threshold;
    return ZL303XX_OK;
}
/** zl303xx_AprGetEnterPhaseLockStatusThreshold

   The function returns the default enter phase lock status threshold for APR.

  Return Value: Uint32T   Enter phase lock status threshold in ns

****************************************************************************/
Uint32T zl303xx_AprGetEnterPhaseLockStatusThreshold(void)
{
    return defaultEnterPhaseLockStatusThreshold;
}


/** zl303xx_AprSetExitPhaseLockStatusThreshold

   The function sets the default exit phase lock status threshold for APR.
   Unit: ns

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetExitPhaseLockStatusThreshold(Uint32T threshold)
{
    defaultExitPhaseLockStatusThreshold = threshold;
    return ZL303XX_OK;
}
/** zl303xx_AprGetExitPhaseLockStatusThreshold

   The function returns the default exit phase lock status threshold for APR.

  Return Value: Uint32T   Exit phase lock status threshold in ns

****************************************************************************/
Uint32T zl303xx_AprGetExitPhaseLockStatusThreshold(void)
{
    return defaultExitPhaseLockStatusThreshold;
}


/** zl303xx_AprSetWarmStart

   The function sets the warm start flag for APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetWarmStart(zl303xx_BooleanE warmStart)
{
    defaultWarmStartFlag = warmStart;
    return ZL303XX_OK;
}
/** zl303xx_AprGetWarmStart

   The function gets the warm start flag for APR.

  Return Value: zlStatusE

****************************************************************************/
zl303xx_BooleanE zl303xx_AprGetWarmStart(void)
{
    return defaultWarmStartFlag;
}


/** zl303xx_AprSetWsInitialOffset

   The function sets the warm start initial offset for APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetWsInitialOffset(Sint32T initOffset)
{
    defaultWsInitFreqOffset = initOffset;
    return ZL303XX_OK;
}
/** zl303xx_AprGetWsInitialOffset

   The function gets the warm start initial offset for APR.

  Return Value: zlStatusE

****************************************************************************/
Sint32T zl303xx_AprGetWsInitialOffset(void)
{
    return defaultWsInitFreqOffset;
}

/** zl303xx_AprSetAPRFrequencyLockedPhaseSlopeLimit

   The function sets the PSL/FCL's phase slope limit for locked APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetAPRFrequencyLockedPhaseSlopeLimit(zl303xx_AprPhaseSlopeLimitE psl)
{
    defaultAPRFrequencyLockedPhaseSlopeLimit = psl;
    return ZL303XX_OK;
}
/** zl303xx_AprGetAPRFrequencyLockedPhaseSlopeLimit

   The function gets the PSL/FCL's phase slope limit for locked APR.

  Return Value: zl303xx_AprPhaseSlopeLimitE

****************************************************************************/
zl303xx_AprPhaseSlopeLimitE zl303xx_AprGetAPRFrequencyLockedPhaseSlopeLimit(void)
{
    return defaultAPRFrequencyLockedPhaseSlopeLimit;
}

/** zl303xx_AprSetAPRFrequencyNotLockedPhaseSlopeLimit

   The function sets the PSL/FCL's phase slope limit for not-locked APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetAPRFrequencyNotLockedPhaseSlopeLimit(zl303xx_AprPhaseSlopeLimitE psl)
{
    defaultAPRFrequencyNotLockedPhaseSlopeLimit = psl;
    return ZL303XX_OK;
}
/** zl303xx_AprGetAPRFrequencyNotLockedPhaseSlopeLimit

   The function gets the PSL/FCL's phase slope limit for not-locked APR.

  Return Value: zl303xx_AprPhaseSlopeLimitE

****************************************************************************/
zl303xx_AprPhaseSlopeLimitE zl303xx_AprGetAPRFrequencyNotLockedPhaseSlopeLimit(void)
{
    return defaultAPRFrequencyNotLockedPhaseSlopeLimit;
}

/** zl303xx_AprSetAPRFrequencyLockedFrequencyChangeLimit

   The function sets the PSL/FCL's phase slope limit for locked APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetAPRFrequencyLockedFrequencyChangeLimit(zl303xx_AprFrequencyChangeLimitE fcl)
{
    defaultAPRFrequencyLockedFrequencyChangeLimit = fcl;
    return ZL303XX_OK;
}
/** zl303xx_AprGetAPRFrequencyLockedFrequencyChangeLimit

   The function gets the PSL/FCL's phase slope limit for locked APR.

  Return Value: zl303xx_AprFrequencyChangeLimitE

****************************************************************************/
zl303xx_AprFrequencyChangeLimitE zl303xx_AprGetAPRFrequencyLockedFrequencyChangeLimit(void)
{
    return defaultAPRFrequencyLockedFrequencyChangeLimit;
}

/** zl303xx_AprSetAPRFrequencyNotLockedFrequencyChangeLimit

   The function sets the PSL/FCL's phase slope limit for not-locked APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetAPRFrequencyNotLockedFrequencyChangeLimit(zl303xx_AprFrequencyChangeLimitE fcl)
{
    defaultAPRFrequencyNotLockedFrequencyChangeLimit = fcl;
    return ZL303XX_OK;
}
/** zl303xx_AprGetAPRFrequencyNotLockedFrequencyChangeLimit

   The function gets the PSL/FCL's phase slope limit for not-locked APR.

  Return Value: zl303xx_AprFrequencyChangeLimitE

****************************************************************************/
zl303xx_AprFrequencyChangeLimitE zl303xx_AprGetAPRFrequencyNotLockedFrequencyChangeLimit(void)
{
    return defaultAPRFrequencyNotLockedFrequencyChangeLimit;
}

/** zl303xx_AprSetAPRFrequencyFastPhaseSlopeLimit

   The function sets the PSL/FCL's phase slope limit for fast lock APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetAPRFrequencyFastPhaseSlopeLimit(zl303xx_AprPhaseSlopeLimitE psl)
{
    defaultAPRFrequencyFastPhaseSlopeLimit = psl;
    return ZL303XX_OK;
}
/** zl303xx_AprGetAPRFrequencyFastPhaseSlopeLimit

   The function gets the PSL/FCL's phase slope limit for fast lock APR.

  Return Value: zl303xx_AprPhaseSlopeLimitE

****************************************************************************/

zl303xx_AprPhaseSlopeLimitE zl303xx_AprGetAPRFrequencyFastPhaseSlopeLimit(void)
{
    return defaultAPRFrequencyFastPhaseSlopeLimit;
}

/** zl303xx_AprSetAPRFrequencyFastFrequencyChangeLimit

The function sets the PSL/FCL's phase slope limit for fast lock APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetAPRFrequencyFastFrequencyChangeLimit(zl303xx_AprFrequencyChangeLimitE fcl)
{
    defaultAPRFrequencyFastFrequencyChangeLimit = fcl;
    return ZL303XX_OK;
}
/** zl303xx_AprGetAPRFrequencyFastFrequencyChangeLimit

The function gets the PSL/FCL's phase slope limit for fast lock APR.

  Return Value: zl303xx_AprFrequencyChangeLimitE

****************************************************************************/
zl303xx_AprFrequencyChangeLimitE zl303xx_AprGetAPRFrequencyFastFrequencyChangeLimit(void)
{
    return defaultAPRFrequencyFastFrequencyChangeLimit;
}


/** zl303xx_AprSetAdjSize1HzPSL

   The function sets an element in the PSL size ranges for 1Hz.

   When 1Hz makes an adjustment, the adjustment size is compared against each
   element in defaultAdjSize1HzPSL[]. When the smallest element that is still
   larger than the adjustment is found, the corresponding PSL value in
   defaultPSL_1Hz[] is used.

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

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetPSL(Uint32T idx, Uint32T adjSize, Uint32T psl)
{
    if( idx >= ZL303XX_MAX_NUM_PSL_LIMITS )
    {
         return( ZL303XX_PARAMETER_INVALID );
    }

    defaultAdjSize1HzPSL[idx] = adjSize;
    defaultPSL_1Hz[idx] = psl;

    return ZL303XX_OK;
}
/** zl303xx_AprGetPSL

   The function gets the PSL size ranges for 1Hz.

  Return Value: zlStatusE

****************************************************************************/
 zlStatusE zl303xx_AprGetPSL(Uint32T idx, Uint32T *adjSize, Uint32T *psl )
{
    if( idx >= ZL303XX_MAX_NUM_PSL_LIMITS )
    {
         return( ZL303XX_PARAMETER_INVALID );
    }

    *adjSize = defaultAdjSize1HzPSL[idx];
    *psl = defaultPSL_1Hz[idx];

    return ZL303XX_OK;
}

/** zl303xx_AprSetPFLockInThreshold

   The function sets the PSL/FCL's lockin threshold for APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetPFLockInThreshold(Uint32T lit)
{
    defaultPFLockInThreshold = lit;
    return ZL303XX_OK;
}

/** zl303xx_AprGetPFLockInThreshold

   The function gets the PSL/FCL's lockin threshold for APR.

  Return Value: Uint32T

****************************************************************************/
Uint32T zl303xx_AprGetPFLockInThreshold(void)
{
    return defaultPFLockInThreshold;
}

/** zl303xx_AprSetPFLockInCount

   The function sets the PSL/FCL's lockin count for APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetPFLockInCount(Uint32T lic)
{
    defaultPFLockInCount = lic;
    return ZL303XX_OK;
}

/** zl303xx_AprGetPFLockInCount

   The function gets the PSL/FCL's lockin count for APR.

  Return Value: Uint32T

****************************************************************************/
Uint32T zl303xx_AprGetPFLockInCount(void)
{
    return defaultPFLockInCount;
}

/** zl303xx_AprSetPFLockOutThreshold

   The function sets the PSL/FCL's lockout threshold for APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetPFLockOutThreshold(Uint32T lot)
{
    defaultPFLockOutThreshold = lot;
    return ZL303XX_OK;
}

/** zl303xx_AprGetPFLockOutThreshold

   The function gets the PSL/FCL's lockout threshold for APR.

  Return Value: Uint32T

****************************************************************************/
Uint32T zl303xx_AprGetPFLockOutThreshold(void)
{
    return defaultPFLockOutThreshold;
}


/** zl303xx_AprSetPFAdjustFreqMinPhase

   The function sets the PSL/FCL's minimum phase at which it will make an
   adjustment.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetPFAdjustFreqMinPhase(Sint32T afmp)
{
    defaultPFAdjustFreqMinPhase = afmp;
    return ZL303XX_OK;
}
/** zl303xx_AprGetPFAdjustFreqMinPhase

   The function gets the PSL/FCL's minimum phase at which it will make an
   adjustment.

  Return Value: zlStatusE

****************************************************************************/
Sint32T zl303xx_AprGetPFAdjustFreqMinPhase(void)
{
    return defaultPFAdjustFreqMinPhase;
}


/** zl303xx_AprSetCustomerThresholdForFlagV

   The function sets the customer threshold of V flag for APR.

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetCustomerThresholdForFlagV(Sint32T thresholdForFlagV)
{
   defaultThresholdForFlagV = thresholdForFlagV;
   return ZL303XX_OK;
}

/** zl303xx_AprGetCustomerThresholdForFlagV

   The function returns the customer threshold of V flag for APR.

  Return Value: Sint32T

****************************************************************************/
Sint32T zl303xx_AprGetCustomerThresholdForFlagV(void)
{
    return defaultThresholdForFlagV;
}


/** zl303xx_AprSetSetTimePacketTreatment

   The function sets the packet treatment when setTime() is called.

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetSetTimePacketTreatment(zl303xx_PacketTreatmentE setTimePacketTreatment)
{
   defaultSetTimePacketTreatment = setTimePacketTreatment;
   return ZL303XX_OK;
}

/** zl303xx_AprGetSetTimePacketTreatment

   The function returns the packet setTime() treatment.

  Return Value: zl303xx_PacketTreatmentE

****************************************************************************/
zl303xx_PacketTreatmentE zl303xx_AprGetSetTimePacketTreatment(void)
{
    return defaultSetTimePacketTreatment;
}


/** zl303xx_AprSetStepTimePacketTreatment

   The function sets the packet treatment when stepTime() is called.

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetStepTimePacketTreatment(zl303xx_PacketTreatmentE stepTimePacketTreatment)
{
   defaultStepTimePacketTreatment = stepTimePacketTreatment;
   return ZL303XX_OK;
}

/** zl303xx_AprGetStepTimePacketTreatment

   The function returns the packet stepTime() treatment.

  Return Value: zl303xx_PacketTreatmentE

****************************************************************************/
zl303xx_PacketTreatmentE zl303xx_AprGetStepTimePacketTreatment(void)
{
    return defaultStepTimePacketTreatment;
}


/** zl303xx_AprSetAdjustTimePacketTreatment

   The function sets the packet treatment when adjustTime() is called.

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetAdjustTimePacketTreatment(zl303xx_PacketTreatmentE adjustTimePacketTreatment)
{
   defaultAdjustTimePacketTreatment = adjustTimePacketTreatment;
   return ZL303XX_OK;
}

/** zl303xx_AprGetAdjustTimePacketTreatment

   The function returns the packet adjustTime() treatment.

  Return Value: zl303xx_PacketTreatmentE

****************************************************************************/
zl303xx_PacketTreatmentE zl303xx_AprGetAdjustTimePacketTreatment(void)
{
    return defaultAdjustTimePacketTreatment;
}


/** zl303xx_AprSetLegacyTreatment

   The function sets the legacy treatment.

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetLegacyTreatment(zl303xx_BooleanE legacyTreatment)
{
   defaultLegacyTreatment = legacyTreatment;
   return ZL303XX_OK;
}

/** zl303xx_AprGetLegacyTreatment

   The function returns the legacy treatment.

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE zl303xx_AprGetLegacyTreatment(void)
{
    return defaultLegacyTreatment;
}

/** zl303xx_AprSetPFStepTimeResolution

   The function sets the PSL/FCL's step time resolution

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetPFStepTimeResolution(Uint32T stepTimeResolution)
{
    defaultPFStepTimeResolution = stepTimeResolution;
    return ZL303XX_OK;
}

/** zl303xx_AprGetPFStepTimeResolution

   The function gets the PSL/FCL's step time resolution

  Return Value: Uint32T

****************************************************************************/
Uint32T zl303xx_AprGetPFStepTimeResolution(void)
{
    return defaultPFStepTimeResolution;
}

/** zl303xx_AprSetPFLockedPhaseOutlierThreshold

   The function sets the PSL/FCL's locked phase outlier threshold

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetPFLockedPhaseOutlierThreshold(Uint32T lockedPhaseOutlierThreshold)
{
    defaultPFLockedPhaseOutlierThreshold = lockedPhaseOutlierThreshold;
    return ZL303XX_OK;
}

/** zl303xx_AprGetPFLockedPhaseOutlierThreshold

   The function gets the PSL/FCL's locked phase outlier threshold

  Return Value: Uint32T

****************************************************************************/
Uint32T zl303xx_AprGetPFLockedPhaseOutlierThreshold(void)
{
    return defaultPFLockedPhaseOutlierThreshold;
}

/** zl303xx_AprSetPFInitialFrequencyOffset

    The function sets the initial DCO frequency offset in ppt

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetPFInitialFrequencyOffset(Sint32T initialFrequencyOffset)
{
    defaultPFInitialFrequencyOffset = initialFrequencyOffset;
    return ZL303XX_OK;
}

/** zl303xx_AprGetPFInitialFrequencyOffset

    The function gets the initial DCO frequency offset at startup in ppt

  Return Value: Uint32T

****************************************************************************/
Uint32T zl303xx_AprGetPFInitialFrequencyOffset(void)
{
    return defaultPFInitialFrequencyOffset;
}

/******************************************************************************
                              AddServer() start
*******************************************************************************/

zl303xx_AprAlgTypeModeE defaultAlgTypeMode = ZL303XX_NATIVE_PKT_FREQ;
zl303xx_BooleanE defaultbUseT2PLL = ZL303XX_FALSE;
zl303xx_BooleanE defaultenableXOComp = ZL303XX_FALSE;
zl303xx_BooleanE defaultbT2PLLFLEnb = ZL303XX_FALSE;
Uint32T T2FLMinPhaseNs = 8;
zl303xx_AprOscillatorFilterTypesE defaultOscillatorFilter = ZL303XX_TCXO;
Uint32T defaultOscHoldoverStability = ZL303XX_USING_DEFAULT_HOLDOVER_STABILITY;
Uint32T defaultXOsModeTimeout = ZL303XX_USING_DEFAULT_XO_SMODE_TIMEOUT;
Uint32T  defaultSModeAgeOut = ZL303XX_USING_DEFAULT_XO_SMODE_AGEOUT;
Uint32T defaultReRouteHoldover2 = ZL303XX_USING_DEFAULT_RE_ROUTE_HOLDOVER2;
zl303xx_AprFilterTypesE defaultFilter = ZL303XX_BW_0_FILTER;
zl303xx_BooleanE defaultXdslHpFlag = ZL303XX_FALSE;
zl303xx_AprPktRateE defaultFwdPPS = ZL303XX_64_PPS;
zl303xx_AprPktRateE defaultRevPPS = ZL303XX_16_PPS;
zl303xx_AprTsFormatE defaultTsFormat = ZL303XX_APR_TS_PTP;
zl303xx_BooleanE defaultTs32BitFlag = ZL303XX_FALSE;
zl303xx_BooleanE zl303xx_AprUseReversePath = ZL303XX_FALSE;
zl303xx_BooleanE defaultHybridServerFlag = ZL303XX_FALSE;
Uint32T defaultPacketDiscardDurationInSecFlag = 0;
zl303xx_AprPullInRangeE defaultPullInRange = ZL303XX_APR_PIR_12_PPM;
Uint32T defaultEnterHoldeverGST = 0;
Uint32T defaultExitValidGST = 0;
Uint32T defaultExitLockGST = 0;
Uint32T defaultExitPhaseAlignGST = 0;
Uint8T defaultLockMasks = 4;  /* L4 is masked */
Uint32T FastLockTotalTimeInSecs = 0;
Uint32T HoldoverFreezeValue = 0;
Uint32T AprTaskBasePeriodMs = ZL303XX_APR_TIMER1_PERIOD_MS;  /* Apr Task Period */
Uint32T PslFclTaskBasePeriodMs = ZL303XX_APR_TIMER2_PERIOD_MS; /* PSL&FCL, AprSample Task Period */
Sint32T DFSeedValue = 0;
Sint32T Type2BFastlockStartupIt = 3;
Sint32T Type2BFastlockThreshold = 1000;
Sint32T OutlierTimerValue = 3*3600*1000/ZL303XX_APR_TIMER1_PERIOD_MS; /* 3 hours @ ZL303XX_APR_TIMER1_PERIOD_MS rate */
Sint32T ClkInvalidCntrValue = 100;  /* defaults to 960 s*/
Uint32T FastLockPktSelWindowSize = 0;
Sint32T L2phase_varLimitValue = 0;
Uint32T L4Threshold = 40000; /* 40 ppb threshold*/
zl303xx_BooleanE DefaultUseOFM = ZL303XX_TRUE; /* The default is to use OFM with BC mode*/
zl303xx_AprFilterTypesE FastLockBW = 0;
Uint32T exampleAprFCL = APR_FCL_MAX;
zl303xx_BooleanE fastLock1HzEnable = ZL303XX_FALSE;
Uint32T fastLock1HzInterval = 0;
Uint32T fastLock1HzTotalPeriod = 0;
Uint32T fastLock1HzDelay = 0;
Uint32T stepTimeAdjustTimeThreshold = 500000;
Uint32T stepTimeAdjustFreqThreshold = 500000;


Uint32T defaultAdjustTimeMinThreshold = 20;
zl303xx_BooleanE defaultUseAdjustTimeHybrid = ZL303XX_FALSE;
zl303xx_BooleanE defaultUseAdjustTimePacket = ZL303XX_FALSE;
Uint32T defaultStepTimeDetectableThr = 10000;
zl303xx_BooleanE defaultAprDynamicPhaseCorrectionEnabled = ZL303XX_FALSE;
zl303xx_BooleanE defaultAdjFreqDynamicAdjustmentEnable = ZL303XX_TRUE;
Uint32T defaultAprDynamicPhaseCorrectionThr = 1000;
zl303xx_BooleanE DefaultUseNCOAssistDpll = ZL303XX_FALSE;
Uint32T defaultHybridLockTimeTarget = 120;

/** zl303xx_AprSetAlgTypeMode

   The function sets the default algorithm type mode for APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetAlgTypeMode(zl303xx_AprAlgTypeModeE algTypeMode)
{
   defaultAlgTypeMode = algTypeMode;
    return ZL303XX_OK;
}
/** zl303xx_AprGetAlgTypeMode

   The function returns the default algorithm type mode for APR.

  Return Value: zl303xx_AprOscillatorTypesE

****************************************************************************/
zl303xx_AprAlgTypeModeE zl303xx_AprGetAlgTypeMode(void)
{
    return defaultAlgTypeMode;
}


/** zl303xx_AprSetUseType2BPLL

*  The function sets the Boolean for using Type 2 PLL default to
*  FALSE.

  Return Value: zlStatusE*

****************************************************************************/
zlStatusE zl303xx_AprSetUseType2BPLL(zl303xx_BooleanE bUseT2PLL)
{
   defaultbUseT2PLL = bUseT2PLL;
    return ZL303XX_OK;
}
/** zl303xx_AprGetUseType2BPLL

   The function returns the Boolean for using Type 2 PLL.

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE zl303xx_AprGetUseType2BPLL(void)
{
    return defaultbUseT2PLL;
}

/** zl303xx_AprSetEnableXOCompensation

*  The function sets the Boolean enableXOComp default to
*  FALSE.

  Return Value: zlStatusE*

****************************************************************************/
zlStatusE zl303xx_AprSetEnableXOCompensation(zl303xx_BooleanE enableXOComp)
{
   defaultenableXOComp = enableXOComp;
    return ZL303XX_OK;
}

/** zl303xx_AprGetEnableXOCompensation

   The function returns the Boolean for enableXOComp.

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE zl303xx_AprGetEnableXOCompensation(void)
{
    return defaultenableXOComp;
}


/** zl303xx_AprSetUseType2BPLLFastLock

*  The function sets the Boolean for enabling T2PLL Fast Lock.

  Return Value: zlStatusE*

****************************************************************************/
zlStatusE zl303xx_AprSetUseType2BPLLFastLock(zl303xx_BooleanE bUseT2PLLFLE)
{
   defaultbT2PLLFLEnb = bUseT2PLLFLE;
    return ZL303XX_OK;
}
/** zl303xx_AprGetUseType2BPLLFastLock

*  The function returns the Boolean for enabling T2PLL Fast
*  Lock.

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE zl303xx_AprGetUseType2BPLLFastLock(void)
{
    return defaultbT2PLLFLEnb;
}


/** zl303xx_AprSetType2bFastLockMinPhase

*  The function sets the value for Type2bFastLockMinPhase.

  Return Value: zlStatusE*

****************************************************************************/
zlStatusE zl303xx_AprSetType2bFastLockMinPhase(Uint32T MinPhase)
{
   T2FLMinPhaseNs = MinPhase;
    return ZL303XX_OK;
}
/** zl303xx_AprGetUseType2BPLLFastLock

*  The function returns the value for Type2bFastLockMinPhase
*  Lock.

  Return Value: zl303xx_BooleanE

****************************************************************************/
Uint32T zl303xx_AprGetType2bFastLockMinPhase(void)
{
    return T2FLMinPhaseNs;
}




/** zl303xx_AprSetOscillatorFilterType

   The function sets the default oscillator filter type for APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetOscillatorFilterType(zl303xx_AprOscillatorFilterTypesE oscillatorFilter)
{
   defaultOscillatorFilter = oscillatorFilter;
    return ZL303XX_OK;
}
/** zl303xx_AprGetOscillatorFilterType

   The function returns the default oscillator filter type for APR.

  Return Value: zl303xx_AprOscillatorFilterTypesE

****************************************************************************/
zl303xx_AprOscillatorFilterTypesE zl303xx_AprGetOscillatorFilterType(void)
{
    return defaultOscillatorFilter;
}


/** zl303xx_AprSetHoldoverStability

   The function sets the oscillator holdover stability for APR X-Params.

  Parameters:
   [in] Uint32T       Oscillator Holdover Stability

****************************************************************************/
zlStatusE zl303xx_AprSetHoldoverStability(Uint32T holdoverStabilityPpt)
{
    defaultOscHoldoverStability = holdoverStabilityPpt;
    return ZL303XX_OK;
}
/** zl303xx_AprGetHoldoverStability

   The function returns the oscillator setting for APR X-Params.

  Return Value: Uint32T       Oscillator Holdover Stability

****************************************************************************/
Uint32T zl303xx_AprGetHoldoverStability(void)
{
    return defaultOscHoldoverStability;
}

/** zl303xx_AprSetSModeTimeout

*  The function sets the SMode timeout for APR X-Params.

  Parameters:
   [in] sModeTimeoutSecs       Timeout (in 10s of secs)

****************************************************************************/
zlStatusE zl303xx_AprSetSModeTimeout(Uint32T sModeTimeoutSecs)
{
    defaultXOsModeTimeout = sModeTimeoutSecs ;
    return ZL303XX_OK;
}

/** zl303xx_AprGetSModeTimeout

*  The function returns the SMode timeout setting for APR
*  X-Params.

  Return Value: Uint32T       SMode timeout

****************************************************************************/
Uint32T zl303xx_AprGetSModeTimeout(void)
{
    return defaultXOsModeTimeout;
}

/** zl303xx_AprSetSModeAgeOut

*  The function sets the SMode age out for APR X-Params.

  Parameters:
   [in] sModeAgeOutSecs       Timeout (in 10s of secs)

****************************************************************************/
zlStatusE zl303xx_AprSetSModeAgeOut(Uint32T sModeAgeOutSecs)
{
    defaultSModeAgeOut = sModeAgeOutSecs ;
    return ZL303XX_OK;
}

/** zl303xx_AprGetSModeAgeOut

*  The function returns the SMode age out setting for APR
*  X-Params.

  Return Value: Uint32T       Oscillator Holdover Stability

****************************************************************************/
Uint32T zl303xx_AprGetSModeAgeOut(void)
{
    return defaultSModeAgeOut;
}


/** zl303xx_AprSetFilterType

   The function sets the default filter type for APR.

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetFilterType(zl303xx_AprFilterTypesE filter)
{
   defaultFilter = filter;
   return ZL303XX_OK;
}
/** zl303xx_AprGetFilterType

   The function returns the default filter type for APR.

  Return Value: zl303xx_AprFilterTypesE

****************************************************************************/
zl303xx_AprFilterTypesE zl303xx_AprGetFilterType(void)
{
    return defaultFilter;
}



/** zl303xx_AprSetXdslHpFlag

   The function sets the default G8261 flag of XDSL algorithm type mode for APR.

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetXdslHpFlag(zl303xx_BooleanE bXdslHpFlag)
{
   defaultXdslHpFlag = bXdslHpFlag;
   return ZL303XX_OK;
}
/** zl303xx_AprGetXdslHpFlag

   The function returns the default filter type for APR.

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE zl303xx_AprGetXdslHpFlag(void)
{
    return defaultXdslHpFlag;
}



/** zl303xx_AprSetPktRate

   The function sets the Packet rate type for APR X-Params.

  Parameters:
   [in] zl303xx_AprPktRateE        Packet rate Type
   [in] bFwdPath              forward path or not

****************************************************************************/
zlStatusE zl303xx_AprSetPktRate(zl303xx_AprPktRateE pps, zl303xx_BooleanE bFwdPath)
{
   if (bFwdPath == ZL303XX_TRUE)
   {
      defaultFwdPPS = pps;
   }
   else
   {
      defaultRevPPS = pps;
   }

   return ZL303XX_OK;
}

/** zl303xx_AprGetPktRate

   The function returns the default packet rate setting for APR X-Params.

  Return Value: zl303xx_AprPktRateE       Packet Rate Type

****************************************************************************/
zl303xx_AprPktRateE zl303xx_AprGetPktRate(zl303xx_BooleanE bFwdPath)
{
   if (bFwdPath == ZL303XX_TRUE)
   {
      return defaultFwdPPS;
   }
   else
   {
      return defaultRevPPS;
   }
}


/** zl303xx_AprSetTsFormat

   The function sets the default timestamp format for APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetTsFormat(zl303xx_AprTsFormatE tsFormat)
{
    defaultTsFormat = tsFormat;
    return ZL303XX_OK;
}
/** zl303xx_AprGetTsFormat

   The function returns the default timestamp format for APR.

  Return Value: zl303xx_AprTsFormatE

****************************************************************************/
zl303xx_AprTsFormatE zl303xx_AprGetTsFormat(void)
{
    return defaultTsFormat;
}


/** zl303xx_AprSet32BitTsFlag

   The function sets the default 32-bit timestamp flag for APR.

  Parameters:
   [in] zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSet32BitTsFlag(zl303xx_BooleanE b32BitTs)
{
    defaultTs32BitFlag = b32BitTs;
    return ZL303XX_OK;
}
/** zl303xx_AprGet32BitTsFlag

   The function returns the default value of 32-bit timestamp flag in APR.

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE zl303xx_AprGet32BitTsFlag(void)
{
    return defaultTs32BitFlag;
}

/**
  Function Name:
   zl303xx_AprSetUseReversePath

  Details:
   Set the flag to indicate if using reverse path or not

  Parameters:
   [in]    bUserRevPath

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_AprSetUseReversePath(zl303xx_BooleanE bUseRevPath)
{
   zl303xx_AprUseReversePath = bUseRevPath;
   return ZL303XX_OK;
}

/**
  Function Name:
   zl303xx_AprGetUseReversePath

  Details:
   Get the flag of using reverse path or not

  Parameters:
   [in]    None

  Return Value:
   zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_AprGetUseReversePath(void)
{
   return zl303xx_AprUseReversePath;
}


/** zl303xx_AprSetHybridServer

   The function sets the default hybrid server flag for APR.

  Parameters:
   [in] zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetHybridServerFlag(zl303xx_BooleanE bHybrid)
{
    defaultHybridServerFlag = bHybrid;
    return ZL303XX_OK;
}
/** zl303xx_AprGetHybridServerFlag

   The function returns the default settings of hybrid server flag in APR.

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE zl303xx_AprGetHybridServerFlag(void)
{
    return defaultHybridServerFlag;
}


/** zl303xx_AprSetPacketDiscardDurationInSecFlag

   The function sets the default settings of the packet dicard duration
   in seconds flag in APR.

  Parameters:
   [in] zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetPacketDiscardDurationInSecFlag(Uint32T durationIsSec)
{
    defaultPacketDiscardDurationInSecFlag = durationIsSec;
    return ZL303XX_OK;
}
/** zl303xx_AprGetPacketDiscardDurationInSecFlag

   The function returns the default settings of the packet dicard duration
   in seconds flag in APR.

  Return Value: zl303xx_BooleanE

****************************************************************************/
Uint32T zl303xx_AprGetPacketDiscardDurationInSecFlag(void)
{
    return defaultPacketDiscardDurationInSecFlag;
}


/* zl303xx_AprSetPullInRange

   This function sets the default pull-in range for APR.

  Parameters:
   [in]   pullInRange   Default pull-in range to use for all created APR
                             devices.

  Return Value:  ZL303XX_OK  Success.

****************************************************************************/
zlStatusE zl303xx_AprSetPullInRange(zl303xx_AprPullInRangeE pullInRange)
{
    defaultPullInRange = pullInRange;
    return ZL303XX_OK;
}
/* zl303xx_AprGetPullInRange

   This function returns the default pull-in range for APR.

  Parameters:
      None.

  Return Value:  #zl303xx_AprPullInRangeE  Default pull-in range.

****************************************************************************/
zl303xx_AprPullInRangeE zl303xx_AprGetPullInRange(void)
{
    return defaultPullInRange;
}

/** zl303xx_AprSetEnterHoldoverGST

   The function sets the default guard soak timer to enter holdover status flag
   in APR.
   Unit: ten second

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetEnterHoldoverGST(Uint32T gstTime)
{
    defaultEnterHoldeverGST = gstTime;
    return ZL303XX_OK;
}
/** zl303xx_AprGetEnterHoldoverGST

   The function returns the default guard soak timer to enter holdover status
   flag in APR.

  Return Value: Uint32T   time in ten second

****************************************************************************/
Uint32T zl303xx_AprGetEnterHoldoverGST(void)
{
    return defaultEnterHoldeverGST;
}

/** zl303xx_AprSetExitValidGST

   The function sets the default guard soak timer to exit Valid status flag
   (V from 1 to 0) in APR.
   Unit: ten second

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetExitValidGST(Uint32T gstTime)
{
   defaultExitValidGST = gstTime;
    return ZL303XX_OK;
}
/** zl303xx_AprGetExitValidGST

   The function returns the default guard soak timer to exit Valid status flag
   (V from 1 to 0) in APR.

  Return Value: Uint32T   time in ten second

****************************************************************************/
Uint32T zl303xx_AprGetExitValidGST(void)
{
    return defaultExitValidGST;
}


/** zl303xx_AprSetExitLockGST

   The function sets the default guard soak timer to exit Lock status flag
   (L from 1 to 0) in APR.
   Unit: ten second

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetExitLockGST(Uint32T gstTime)
{
   defaultExitLockGST = gstTime;
    return ZL303XX_OK;
}

/** zl303xx_AprGetExitLockGST

   The function returns the default guard soak timer to exit Lock status flag
   (L from 1 to 0) in APR.

  Return Value: Uint32T   time in ten second

****************************************************************************/
Uint32T zl303xx_AprGetExitLockGST(void)
{
    return defaultExitLockGST;
}

/** zl303xx_AprSetExitPhaseAlignGST

   The function sets the default guard soak timer to exit PhaseAlign status flag
   (PA from 1 to 0) in APR.
   Unit: ten second

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetExitPhaseAlignGST(Uint32T gstTime)
{
   defaultExitPhaseAlignGST = gstTime;
    return ZL303XX_OK;
}

/** zl303xx_AprGetExitPhaseAlignGST

   The function returns the default guard soak timer to exit PhaseAlign status flag
   (PA from 1 to 0) in APR.

  Return Value: Uint32T   time in ten second

****************************************************************************/
Uint32T zl303xx_AprGetExitPhaseAlignGST(void)
{
    return defaultExitPhaseAlignGST;
}

/** zl303xx_AprSetLockMasks

*  The function sets the default masks for L2, L3 and L4 flags
*  in APR.
*
*
*           0 - unmask L2, L3 and L4 flags (use all of them);
            1 - mask L2 flag (not use L2);
            2 - mask L3 flag (not use L3);
            3 - mask both L2 and L3 flags (not use both of them)
            4 - mask only L4 (not use L4); (This is the default of Struct_init for backward compatibility).
            5 - unmask only L3 (Not use L2, L4).
            6 - unmask only L2 (Not use L3, L4)
            7 - mask all flages (Not use L2, L3 and L4 flags).


  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetLockMasks(Uint8T maskVal)
{
   defaultLockMasks = maskVal;
    return ZL303XX_OK;
}

/** zl303xx_AprGetLockMasks

*  The function returns the default masks for L2, L3 and L4
*  flags in APR.
*
*
            0 - unmask L2, L3 and L4 flags (use all of them);
            1 - mask L2 flag (not use L2);
            2 - mask L3 flag (not use L3);
            3 - mask both L2 and L3 flags (not use both of them)
            4 - mask only L4 (not use L4); (This is the default of Struct_init for backward compatibility).
            5 - unmask only L3 (Not use L2, L4).
            6 - unmask only L2 (Not use L3, L4)
            7 - mask all flages (Not use L2, L3 and L4 flags).

  Return Value: Uint8T

****************************************************************************/
Uint8T zl303xx_AprGetLockMasks(void)
{
    return defaultLockMasks;
}





/** zl303xx_SetAdjustTimeMinThreshold

   The function sets the adjustTime() minimum threshold.

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_SetAdjustTimeMinThreshold(Uint32T adjustTimeMinThreshold)
{
   defaultAdjustTimeMinThreshold = adjustTimeMinThreshold;
   return ZL303XX_OK;
}

/** zl303xx_GetAdjustTimeMinThreshold

   The function returns the adjustTime() minimum threshold.

  Return Value: zl303xx_BooleanE

****************************************************************************/
Uint32T zl303xx_GetAdjustTimeMinThreshold(void)
{
    return defaultAdjustTimeMinThreshold;
}


/** zl303xx_SetUseAdjustTimeHybrid

   The function sets the useAdjustTimeHybrid boolean.

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_SetUseAdjustTimeHybrid(Uint32T useAdjustTimeHybrid)
{
   defaultUseAdjustTimeHybrid = useAdjustTimeHybrid;
   return ZL303XX_OK;
}

/** zl303xx_GetUseAdjustTimeHybrid

   The function returns the useAdjustTimeHybrid boolean.

  Return Value: zl303xx_BooleanE

****************************************************************************/
Uint32T zl303xx_GetUseAdjustTimeHybrid(void)
{
    return defaultUseAdjustTimeHybrid;
}


/** zl303xx_SetUseAdjustTimePacket

   The function sets the useAdjustTimePacket boolean.

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_SetUseAdjustTimePacket(Uint32T useAdjustTimePacket)
{
   defaultUseAdjustTimePacket = useAdjustTimePacket;
   return ZL303XX_OK;
}

/** zl303xx_GetUseAdjustTimePacket

   The function returns the useAdjustTimePacket boolean.

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE zl303xx_GetUseAdjustTimePacket(void)
{
    return defaultUseAdjustTimePacket;
}


/** zl303xx_SetStepTimeDetectableThr

   The function sets the stepTimeDetectableThr value.

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_SetStepTimeDetectableThr(Uint32T stepTimeDetectableThr)
{
   defaultStepTimeDetectableThr = stepTimeDetectableThr;
   return ZL303XX_OK;
}

/** zl303xx_GetStepTimeDetectableThr

   The function returns the stepTimeDetectableThr value.

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE zl303xx_GetStepTimeDetectableThr(void)
{
    return defaultStepTimeDetectableThr;
}

/** zl303xx_SetAprDynamicPhaseCorrectionEnabled

   The function sets the AprDynamicPhaseCorrectionEnabled value.

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_SetAprDynamicPhaseCorrectionEnabled(zl303xx_BooleanE AprDynamicPhaseCorrectionEnabled)
{
   defaultAprDynamicPhaseCorrectionEnabled = AprDynamicPhaseCorrectionEnabled;
   return ZL303XX_OK;
}

/** zl303xx_GetAprDynamicPhaseCorrectionEnabled

   The function returns the AprDynamicPhaseCorrectionEnabled value.

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE zl303xx_GetAprDynamicPhaseCorrectionEnabled(void)
{
    return defaultAprDynamicPhaseCorrectionEnabled;
}

/** zl303xx_AprSetAdjFreqDynamicAdjustmentEnable

   The function sets the AdjFreqDynamicAdjustmentEnable value.

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetAdjFreqDynamicAdjustmentEnable(zl303xx_BooleanE AdjFreqDynamicAdjustmentEnable)
{
   defaultAdjFreqDynamicAdjustmentEnable = AdjFreqDynamicAdjustmentEnable;
   return ZL303XX_OK;
}

/** zl303xx_AprGetAdjFreqDynamicAdjustmentEnable

   The function returns the AdjFreqDynamicAdjustmentEnable value.

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE zl303xx_AprGetAdjFreqDynamicAdjustmentEnable(void)
{
    return defaultAdjFreqDynamicAdjustmentEnable;
}

/** zl303xx_AprSetHybridLockTimeTarget

   The function sets the hybridLockTimeTarget value.

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetHybridLockTimeTarget(zl303xx_BooleanE hybridLockTimeTarget)
{
   defaultHybridLockTimeTarget = hybridLockTimeTarget;
   return ZL303XX_OK;
}

/** zl303xx_AprGetHybridLockTimeTarget

   The function returns the hybridLockTimeTarget value.

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE zl303xx_AprGetHybridLockTimeTarget(void)
{
    return defaultHybridLockTimeTarget;
}

/** zl303xx_SetAprDynamicPhaseCorrectionThr

   The function sets the defaultAprDynamicPhaseCorrectionThr value.

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_SetAprDynamicPhaseCorrectionThr(Uint32T AprDynamicPhaseCorrectionThr)
{
   defaultAprDynamicPhaseCorrectionThr = AprDynamicPhaseCorrectionThr;
   return ZL303XX_OK;
}

/** zl303xx_GetAprDynamicPhaseCorrectionThr

   The function returns the defaultAprDynamicPhaseCorrectionThr value.

  Return Value: zl303xx_BooleanE

****************************************************************************/
Uint32T zl303xx_GetAprDynamicPhaseCorrectionThr(void)
{
    return defaultAprDynamicPhaseCorrectionThr;
}

/** zl303xx_AprSetUseNCOAssist

The function sets the use-NCO-assist-dpll boolean

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetUseNCOAssist(zl303xx_BooleanE UseNCOAssistDpllValue)
{
    DefaultUseNCOAssistDpll = UseNCOAssistDpllValue;
    return ZL303XX_OK;
}

/** zl303xx_AprGetUseNCOAssist

*   The function gets the the use-NCO-assist-dpll boolean

  Return Value: zl303xx_BooleanE
****************************************************************************/
zl303xx_BooleanE zl303xx_AprGetUseNCOAssist(void)
{
    return DefaultUseNCOAssistDpll;
}


/******************************************************************************
                               APR Misc start
*******************************************************************************/

Uint32T clkStabDelayIter = 10;


/** zl303xx_AprGetClkStabDelayIterations

* The function returns the default value (10 ~= 10*9.6s +3*9.6s
* valid clock time) or the customer-provided value of number of
* iterations to pass prior to starting to calculate the Clock
* Stability value.

  Return Value: Uint32T

****************************************************************************/
Uint32T zl303xx_AprGetClkStabDelayIterations(void)
{
    return clkStabDelayIter;
}
/** zl303xx_AprSetClkStabDelayIterations

  The function overwrites the default value (0) with the
  customer-provided value of number of iterations to pass prior
  to starting to calculate the Clock Stability value. Setting
  this to non-zero allows calulation of non-active servers.

  Return Value: Uint32T

****************************************************************************/
Uint32T zl303xx_AprSetClkStabDelayIterations(Uint32T customDelayIter)
{
    clkStabDelayIter = customDelayIter;
    return clkStabDelayIter;
}



/******************************************************************************
                                 1Hz start
*******************************************************************************/

zl303xx_BooleanE default1HzEnabledFlag = ZL303XX_FALSE;
zl303xx_BooleanE startupTimeInit  = ZL303XX_TRUE;


/** zl303xx_AprSet1HzEnabled

   The function sets the default 1Hz enabled flag for APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSet1HzEnabled(zl303xx_BooleanE enable1Hz)
{
   default1HzEnabledFlag = enable1Hz;

    return ZL303XX_OK;
}
/** zl303xx_AprGet1HzEnabled

   The function returns the default 1Hz enabled flag for APR.

  Return Value: zlStatusE

****************************************************************************/
zl303xx_BooleanE zl303xx_AprGet1HzEnabled(void)
{
    return default1HzEnabledFlag;
}

/** zl303xx_AprSet1HzRealignmentType

   The function sets the default 1Hz realignment type for APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSet1HzRealignmentType(zl303xx_Apr1HzRealignmentTypeE realignmentType)
{
    default1HzRealignmentType = realignmentType;
    return ZL303XX_OK;
}

/** zl303xx_AprGet1HzRealignmentType

   The function returns the default 1Hz realignment type for APR.

  Return Value: zl303xx_Apr1HzRealignmentTypeE

****************************************************************************/
zl303xx_Apr1HzRealignmentTypeE zl303xx_AprGet1HzRealignmentType(void)
{
    return default1HzRealignmentType;
}

/** zl303xx_AprSetPerPacketAdjType

   The function sets the default per packet realignment adjustment type.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetPerPacketAdjType(zl303xx_AprPerPacketAdjTypeE perPacketAdjustmentType)
{
    defaultPerPacketAdjType = perPacketAdjustmentType;
    return ZL303XX_OK;
}

/** zl303xx_AprGetPerPacketAdjType

   The function returns the default per packet realignment adjustment method.

  Return Value: zl303xx_AprPerPacketAdjTypeE

****************************************************************************/
zl303xx_AprPerPacketAdjTypeE zl303xx_AprGetPerPacketAdjType(void)
{
    return defaultPerPacketAdjType;
}


/** zl303xx_AprSet1HzRealignmentInterval

   The function sets the default 1Hz realignment interval for APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSet1HzRealignmentInterval(Uint32T realignmentInterval)
{
    default1HzRealignmentInterval = realignmentInterval;
    return ZL303XX_OK;
}

/** zl303xx_AprGet1HzRealignmentInterval

   The function returns the default 1Hz realignment interval for APR.

  Return Value: Uint32T

****************************************************************************/
Uint32T zl303xx_AprGet1HzRealignmentInterval(void)
{
    return default1HzRealignmentInterval;
}

/** zl303xx_AprSetPCLFCLStaticOffset

   The function sets the default defaultStaticOffset for APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetPCLFCLStaticOffset(Sint32T StaticOffset)
{
    defaultStaticOffset = StaticOffset;
    return ZL303XX_OK;
}

/** zl303xx_AprGet1HzRealignmentInterval

   The function returns the default defaultStaticOffset for APR.

  Return Value: Sint32T*

****************************************************************************/
Sint32T zl303xx_AprGetPCLFCLStaticOffset(void)
{
    return defaultStaticOffset;
}


/** zl303xx_AprSet1HzFilterBandwidth

   The function sets the default 1Hz filter bandwidth for APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSet1HzFilterBandwidth(Uint32T filterBandwidth)
{
    default1HzFilterBandwidth = filterBandwidth;
    return ZL303XX_OK;
}

/** zl303xx_AprGet1HzFilterBandwidth

   The function returns the default 1Hz filter bandwidth for APR.

  Return Value: Uint32T

****************************************************************************/
Uint32T zl303xx_AprGet1HzFilterBandwidth(void)
{
    return default1HzFilterBandwidth;
}

/** zl303xx_AprSet1HzFilterWriteInterval

   The function sets the default 1Hz filter write interval for APR.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSet1HzFilterWriteInterval(Uint32T filterWriteInterval)
{
    default1HzFilterWriteInterval = filterWriteInterval;
    return ZL303XX_OK;
}

/** zl303xx_AprGet1HzFilterWriteInterval

   The function returns the default 1Hz filter write interval for APR.

  Return Value: Uint32T

****************************************************************************/
Uint32T zl303xx_AprGet1HzFilterWriteInterval(void)
{
    return default1HzFilterWriteInterval;
}

/** zl303xx_AprSet1HzStartupTimeInit

   The function sets the default 1Hz startupTime flag for APR. The default is normally
   ZL303XX_TRUE which means there is startup timer which affects among other things, the
   realignment interval.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSet1HzStartupTimeInit(zl303xx_BooleanE init1HzStartupTime)
{
    startupTimeInit = init1HzStartupTime;
    return ZL303XX_OK;
}

/** zl303xx_AprGet1HzStartupTimeInit

   The function gets the default 1Hz startupTime flag for APR.

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE zl303xx_AprGet1HzStartupTimeInit(void)
{
    return startupTimeInit;
}


/** zl303xx_AprSetUseLegacyStreamStartUp

    The function sets the global for old startup behaviour settings flag

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetUseLegacyStreamStartUp(zl303xx_BooleanE enabled)
{
    exampleUseLegacyStreamStartUp = enabled;

    return ZL303XX_OK;
}

/** zl303xx_AprGetUseLegacyStreamStartUp

    The function gets the global for old startup behaviour settings flag

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE zl303xx_AprGetUseLegacyStreamStartUp(void)
{
    return exampleUseLegacyStreamStartUp;
}


/** zl303xx_AprSetAllow1HzAdjustmentsInHoldover

    The function sets the global for allowing 1Hz adjustments to take place
    while in holdover

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetAllow1HzAdjustmentsInHoldover(zl303xx_BooleanE enabled)
{
    exampleAllow1HzAdjustmentsInHoldover = enabled;

    return ZL303XX_OK;
}

/** zl303xx_AprGetAllow1HzAdjustmentsInHoldover

    The function gets the global for allowing 1Hz adjustments to take place
    while in holdover

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE zl303xx_AprGetAllow1HzAdjustmentsInHoldover(void)
{
    return exampleAllow1HzAdjustmentsInHoldover;
}


/** zl303xx_AprSetOnlyAllowSteps

    The function sets the global for allowing 1Hz adjustments to only apply
    setTime() and stepTime() operations

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetOnlyAllowSteps(zl303xx_BooleanE enabled)
{
    exampleOnlyAllowSteps = enabled;

    return ZL303XX_OK;
}

/** zl303xx_AprGetOnlyAllowSteps

    The function gets the global for allowing 1Hz adjustments to only apply
    setTime() and stepTime() operations

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE zl303xx_AprGetOnlyAllowSteps(void)
{
    return exampleOnlyAllowSteps;
}



/** zl303xx_AprGetFastLockPktSelWindowSize

    The function gets the fast lock packet rate select window size

  Return Value: Uint32T
****************************************************************************/
Uint32T zl303xx_AprGetFastLockPktSelWindowSize(void)
{
    return FastLockPktSelWindowSize;
}

/** zl303xx_AprSetFastLockPktSelWindowSize

The function sets the fast lock packet rate select window size

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetFastLockPktSelWindowSize(Uint32T PktSelWindowSize)
{
    FastLockPktSelWindowSize = PktSelWindowSize;
    return ZL303XX_OK;
}


/** zl303xx_AprGetL2phase_varLimitValue

*   The function gets L2phase_varLimit

  Return Value: Uint32T
****************************************************************************/
Sint32T zl303xx_AprGetL2phase_varLimitValue(void)
{
    return L2phase_varLimitValue;
}

/** zl303xx_AprSetL2phase_varLimitValue

The function sets L2phase_varLimit

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetL2phase_varLimitValue(Sint32T VarLimit)
{
    L2phase_varLimitValue = VarLimit;
    return ZL303XX_OK;
}



/** zl303xx_AprSetL4Threshold

The function sets the L4 lock indicator threshold****

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetL4Threshold(Uint32T L4ThresholdValue)
{
    L4Threshold = L4ThresholdValue;
    return ZL303XX_OK;
}


/** zl303xx_AprGetL4Threshold

*   The function gets the L4 lock indicator threshold

  Return Value: Uint32T
****************************************************************************/
Uint32T zl303xx_AprGetL4Threshold(void)
{
    return L4Threshold;
}

/** zl303xx_AprSetUseOFM

The function sets the Use OFM logic value*
*

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetUseOFM(zl303xx_BooleanE UseOFMValue)
{
    DefaultUseOFM = UseOFMValue;
    return ZL303XX_OK;
}


/** zl303xx_AprGetUseOFM

*   The function gets the the Use OFM logic value

  Return Value: zl303xx_BooleanE
****************************************************************************/
zl303xx_BooleanE zl303xx_AprGetUseOFM(void)
{
    return DefaultUseOFM;
}

/** zl303xx_AprGetFastLockTotalTimeInSecs

    The function gets the fast lock total time in seconds

  Return Value: Uint32T
****************************************************************************/
Uint32T zl303xx_AprGetFastLockTotalTimeInSecs(void)
{
    return FastLockTotalTimeInSecs;
}

/** zl303xx_AprSetFastLockTotalTimeInSecs

The function sets the fast lock total time in seconds

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetFastLockTotalTimeInSecs(Uint32T TotalTimeInSecs)
{
    FastLockTotalTimeInSecs = TotalTimeInSecs;
    return ZL303XX_OK;
}

/** zl303xx_AprGetHoldoverFreezeValue

*   The function gets the value of HoldoverFreeze control

  Return Value: Uint32T
****************************************************************************/
Uint32T zl303xx_AprGetHoldoverFreezeValue(void)
{
    return HoldoverFreezeValue;
}

/** zl303xx_AprSetHoldoverFreezeValue

*   The function sets the value of HoldoverFreeze control

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetHoldoverFreezeValue(Uint32T ControlValue)
{
    HoldoverFreezeValue = ControlValue;
    return ZL303XX_OK;
}

/** zl303xx_AprGetAprTaskBasePeriodMs

*   The function gets the value of AprTaskBasePeriodMs

  Return Value: Uint32T
****************************************************************************/
Uint32T zl303xx_AprGetAprTaskBasePeriodMs(void)
{
    return AprTaskBasePeriodMs;
}

/** zl303xx_AprSetAprTaskBasePeriodMs

*   The function sets the value of AprTaskBasePeriodMs

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetAprTaskBasePeriodMs(Uint32T period)
{
    AprTaskBasePeriodMs = period;
    return ZL303XX_OK;
}

/** zl303xx_AprGetPslFclTaskBasePeriodMs

*   The function gets the value of PslFclTaskBasePeriodMs

  Return Value: Uint32T
****************************************************************************/
Uint32T zl303xx_AprGetPslFclTaskBasePeriodMs(void)
{
    return PslFclTaskBasePeriodMs;
}

/** zl303xx_AprSetPslFclTaskBasePeriodMs

*   The function sets the value of PslFclTaskBasePeriodMs

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetPslFclTaskBasePeriodMs(Uint32T period)
{
    PslFclTaskBasePeriodMs = period;
    return ZL303XX_OK;
}

/** zl303xx_AprGetDFSeedValue

*   The function gets the value of DFSeed

  Return Value: Uint32T
****************************************************************************/
Sint32T zl303xx_AprGetDFSeedValue(void)
{
    return DFSeedValue;
}

/** zl303xx_AprSetDFSeedValue

*   The function sets the value of DFSeed

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetDFSeedValue(Sint32T SValue)
{
    DFSeedValue = SValue;
    return ZL303XX_OK;
}

/** zl303xx_AprGetType2BFastlockStartupIt

*   The function gets the value of Type2BFastlockStartupIt

  Return Value: Uint32T
****************************************************************************/
Sint32T zl303xx_AprGetType2BFastlockStartupIt(void)
{
    return Type2BFastlockStartupIt;
}

/** zl303xx_AprSetType2BFastlockStartupIt

*   The function sets the value of Type2BFastlockStartupIt

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetType2BFastlockStartupIt(Sint32T Value)
{
    Type2BFastlockStartupIt = Value;
    return ZL303XX_OK;
}

/** zl303xx_AprGetType2BFastlockThreshold

*   The function gets the value of Type2BFastlockThreshold

  Return Value: Uint32T
****************************************************************************/
Sint32T zl303xx_AprGetType2BFastlockThreshold(void)
{
    return Type2BFastlockThreshold;
}

/** zl303xx_AprSetType2BFastlockThreshold

*   The function sets the value of Type2BFastlockThreshold

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetType2BFastlockThreshold(Sint32T Value)
{
    Type2BFastlockThreshold = Value;
    return ZL303XX_OK;
}

/** zl303xx_AprSetOutlierTimerValue

*   The function sets the value of OutlierTimer

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetOutlierTimerValue(Sint32T OValue)
{
    OutlierTimerValue = OValue;
    return ZL303XX_OK;
}

/** zl303xx_AprGetOutlierTimerValue

*   The function gets the value of OutlierTimer control

  Return Value: Sint32T*
****************************************************************************/
Sint32T zl303xx_AprGetOutlierTimerValue(void)
{
    return OutlierTimerValue;
}




/** zl303xx_AprGetClkInvalidCntr

*   The function gets the value of ClkInvalidCntr

  Return Value: Sint32T*
****************************************************************************/
Sint32T zl303xx_AprGetClkInvalidCntr(void)
{
    return ClkInvalidCntrValue;
}

/** zl303xx_AprSetClkInvalidCntr

*   The function sets the value of ClkInvalidCntr

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetClkInvalidCntr(Sint32T CValue)
{
    ClkInvalidCntrValue = CValue;
    return ZL303XX_OK;
}



/** zl303xx_AprGetFastLockBW

The function gets the fast lock BW

  Return Value: Uint32T
****************************************************************************/
Uint32T zl303xx_AprGetFastLockBW(void)
{
    return FastLockBW;
}

/** zl303xx_AprSetFastLockBW

The function sets the fast lock BW

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetFastLockBW(zl303xx_AprFilterTypesE filter_type)
{
    zlStatusE status = ZL303XX_OK;

    if (filter_type > ZL303XX_BW_7_FILTER)
    {
        status =  ZL303XX_PARAMETER_INVALID;
        ZL303XX_TRACE_ALWAYS("zl303xx_AprSetFastLockBW: filter_type=%d outside of range: %d to %d; return=%d", filter_type,
                              ZL303XX_BW_0_FILTER, ZL303XX_BW_7_FILTER, status ,0,0);
    }
    else
    {
        FastLockBW = filter_type;
    }

    return status;
}

/** zl303xx_AprSetAdjSize1HzFCL

   The function sets an element in the FCL size ranges for 1Hz.

   When 1Hz makes an adjustment, the adjustment size is compared against each
   element in defaultAdjSize1HzFCL[]. When the smallest element that is still
   larger than the adjustment is found, the corresponding FCL value in
   defaultFCL_1Hz[] is used.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetFCL(Uint32T idx, Uint32T adjSize, Uint32T fcl)
{
    if( idx >= ZL303XX_MAX_NUM_FCL_LIMITS )
    {
         return( ZL303XX_PARAMETER_INVALID );
    }

    defaultAdjSize1HzFCL[idx] = adjSize;
    defaultFCL_1Hz[idx] = fcl;

    return ZL303XX_OK;
}

/** zl303xx_AprGetFCL

   The function gets the FCL size ranges for 1Hz.

  Return Value: zlStatusE

****************************************************************************/
 zlStatusE zl303xx_AprGetFCL(Uint32T idx, Uint32T *adjSize, Uint32T *fcl )
{
    if( idx >= ZL303XX_MAX_NUM_FCL_LIMITS )
    {
         return( ZL303XX_PARAMETER_INVALID );
    }

    *adjSize = defaultAdjSize1HzFCL[idx];
    *fcl = defaultFCL_1Hz[idx];

    return ZL303XX_OK;
}

/** zl303xx_AprSetAdjSize1HzAdjScaling

    The function sets an element in the adjustment scaling factor size ranges for 1Hz.

    When 1Hz makes an adjustment, the adjustment size is compared against each
    element in defaultAdjSize1HzAdjScaling[]. When the smallest element that is still
    larger than the adjustment is found, the corresponding adjustment scaling factor value in
    defaultAdjScaling_1Hz[] is used.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_AprSetAdjScaling(Uint32T idx, Uint32T adjSize, Uint32T adjScaling)
{
    if( idx >= ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS  )
    {
         return( ZL303XX_PARAMETER_INVALID );
    }

    defaultAdjSize1HzAdjScaling[idx] = adjSize;
    defaultAdjScaling_1Hz[idx] = adjScaling;

    return ZL303XX_OK;
}

/** zl303xx_AprGetAdjScaling

    The function gets the adjustment scaling factor size for 1Hz.

  Return Value: zlStatusE

****************************************************************************/
 zlStatusE zl303xx_AprGetAdjScaling(Uint32T idx, Uint32T *adjSize, Uint32T *adjScaling )
{
    if( idx >= ZL303XX_MAX_NUM_ADJ_SCALING_LIMITS  )
    {
         return( ZL303XX_PARAMETER_INVALID );
    }

    *adjSize = defaultAdjSize1HzAdjScaling[idx];
    *adjScaling = defaultAdjScaling_1Hz[idx];

    return ZL303XX_OK;
}

/** zl303xx_AprSetFastLock1HzEnable

The function sets the 1 Hz fast lock enable flag

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetFastLock1HzEnable(zl303xx_BooleanE set)
{
    fastLock1HzEnable = set;

    return ZL303XX_OK;
}

/** zl303xx_AprGetFastLock1HzEnable

The function gets the 1 Hz fast lock enable flag

  Return Value: zl303xx_BooleanE
****************************************************************************/
zl303xx_BooleanE zl303xx_AprGetFastLock1HzEnable(void)
{
    return fastLock1HzEnable;
}

/** zl303xx_AprSetFastLock1HzInterval

The function sets the 1 Hz fast lock adjustment interval

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetFastLock1HzInterval(Uint32T set)
{
    fastLock1HzInterval = set;

    return ZL303XX_OK;
}

/** zl303xx_AprGetFastLock1HzInterval

The function gets the 1 Hz fast lock total period in seconds after startup

  Return Value: Uint32T
****************************************************************************/
Uint32T zl303xx_AprGetFastLock1HzInterval(void)
{
    return fastLock1HzInterval;
}


/** zl303xx_AprSetFastLock1HzTotalPeriod

The function sets the 1 Hz fast lock adjustment interval

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetFastLock1HzTotalPeriod(Uint32T set)
{
    fastLock1HzTotalPeriod = set;

    return ZL303XX_OK;
}

/** zl303xx_AprGetFastLock1HzTotalPeriod

The function gets the 1 Hz fast lock total period in seconds after startup

  Return Value: Uint32T
****************************************************************************/
Uint32T zl303xx_AprGetFastLock1HzTotalPeriod(void)
{
    return fastLock1HzTotalPeriod;
}


/** zl303xx_AprSetFastLock1HzDelay

The function sets the 1 Hz fast lock delay interval in seconds

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetFastLock1HzDelay(Uint32T set)
{
    fastLock1HzDelay = set;

    return ZL303XX_OK;
}

/** zl303xx_AprGetFastLock1HzDelay

The function gets the 1 Hz fast lock delay interval in seconds

  Return Value: Uint32T
****************************************************************************/
Uint32T zl303xx_AprGetFastLock1HzDelay(void)
{
    return fastLock1HzDelay;
}

/** zl303xx_AprSetStepTimeAdjustTimeThreshold

The function sets the Step Time Adjust Time Threshold

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetStepTimeAdjustTimeThreshold(Uint32T set)
{
    stepTimeAdjustTimeThreshold = set;

    return ZL303XX_OK;
}

/** zl303xx_AprGetStepTimeAdjustTimeThreshold

The function gets the Step Time Adjust Time Threshold

  Return Value: Uint32T
****************************************************************************/
Uint32T zl303xx_AprGetStepTimeAdjustTimeThreshold(void)
{
    return stepTimeAdjustTimeThreshold;
}


/** zl303xx_AprSetStepTimeAdjustFreqThreshold

The function sets the Step Time Adjust Frequency Threshold

  Return Value: zlStatusE
****************************************************************************/
zlStatusE zl303xx_AprSetStepTimeAdjustFreqThreshold(Uint32T set)
{
    stepTimeAdjustFreqThreshold = set;

    return ZL303XX_OK;
}

/** zl303xx_AprGetStepTimeAdjustFreqThreshold

The function gets the Step Time Adjust Frequency Threshold

  Return Value: Uint32T
****************************************************************************/
Uint32T zl303xx_AprGetStepTimeAdjustFreqThreshold(void)
{
    return stepTimeAdjustFreqThreshold;
}


/** zl303xx_AprSetMaxAdjFreqTime
 *
 * The function sets the maximum adjust frequency adjustment period in seconds.
 *
 *  Return Value: zlStatusE
 * ****************************************************************************/
zlStatusE zl303xx_AprSetMaxAdjFreqTime(Uint32T maxAdjFreqTime)
{
    defaultMaxAdjFreqTime = maxAdjFreqTime;

    return ZL303XX_OK;
}

/** zl303xx_AprGetMaxAdjFreqTime
 *
 * The function gets the maximum adjust frequency adjustment period in seconds.
 *
 *  Return Value: Uint32T
 * ****************************************************************************/
Uint32T zl303xx_AprGetMaxAdjFreqTime(void)
{
    return defaultMaxAdjFreqTime;
}


#include <stdio.h>
#include <string.h>
#include "BPF_RU_IPCM.h"

typedef struct {
	const char*	name;
	void*	tblPtr;
	int		size;
}T_FJ_APR_PARM;

static const T_FJ_APR_PARM  FJ_APR_PARM[] = {
	{"default1HzRealignmentType"                        ,&default1HzRealignmentType                        ,sizeof(default1HzRealignmentType                        )},
	{"defaultPerPacketAdjType"                          ,&defaultPerPacketAdjType                          ,sizeof(defaultPerPacketAdjType                          )},
	{"default1HzRealignmentInterval"                    ,&default1HzRealignmentInterval                    ,sizeof(default1HzRealignmentInterval                    )},
	{"defaultStaticOffset"                              ,&defaultStaticOffset                              ,sizeof(defaultStaticOffset                              )},
	{"default1HzFilterBandwidth"                        ,&default1HzFilterBandwidth                        ,sizeof(default1HzFilterBandwidth                        )},
	{"default1HzFilterWriteInterval"                    ,&default1HzFilterWriteInterval                    ,sizeof(default1HzFilterWriteInterval                    )},
	{"defaultDeviceOptMode"                             ,&defaultDeviceOptMode                             ,sizeof(defaultDeviceOptMode                             )},
	{"defaultDeviceHybridAdjMode"                       ,&defaultDeviceHybridAdjMode                       ,sizeof(defaultDeviceHybridAdjMode                       )},
	{"zl303xx_AprDcs"                                   ,&zl303xx_AprDcs                                   ,sizeof(zl303xx_AprDcs                                   )},
	{"defaultHwDcoResolutionInPpt"                      ,&defaultHwDcoResolutionInPpt                      ,sizeof(defaultHwDcoResolutionInPpt                      )},
	{"defaultEnterPhaseLockStatusThreshold"             ,&defaultEnterPhaseLockStatusThreshold             ,sizeof(defaultEnterPhaseLockStatusThreshold             )},
	{"defaultExitPhaseLockStatusThreshold"              ,&defaultExitPhaseLockStatusThreshold              ,sizeof(defaultExitPhaseLockStatusThreshold              )},
	{"defaultWarmStartFlag"                             ,&defaultWarmStartFlag                             ,sizeof(defaultWarmStartFlag                             )},
	{"defaultWsInitFreqOffset"                          ,&defaultWsInitFreqOffset                          ,sizeof(defaultWsInitFreqOffset                          )},
	{"defaultAPRFrequencyLockedPhaseSlopeLimit"         ,&defaultAPRFrequencyLockedPhaseSlopeLimit         ,sizeof(defaultAPRFrequencyLockedPhaseSlopeLimit         )},
	{"defaultAPRFrequencyNotLockedPhaseSlopeLimit"      ,&defaultAPRFrequencyNotLockedPhaseSlopeLimit      ,sizeof(defaultAPRFrequencyNotLockedPhaseSlopeLimit      )},
	{"defaultAPRFrequencyFastPhaseSlopeLimit"           ,&defaultAPRFrequencyFastPhaseSlopeLimit           ,sizeof(defaultAPRFrequencyFastPhaseSlopeLimit           )},
	{"defaultAPRFrequencyLockedFrequencyChangeLimit"    ,&defaultAPRFrequencyLockedFrequencyChangeLimit    ,sizeof(defaultAPRFrequencyLockedFrequencyChangeLimit    )},
	{"defaultAPRFrequencyNotLockedFrequencyChangeLimit" ,&defaultAPRFrequencyNotLockedFrequencyChangeLimit ,sizeof(defaultAPRFrequencyNotLockedFrequencyChangeLimit )},
	{"defaultAPRFrequencyFastFrequencyChangeLimit"      ,&defaultAPRFrequencyFastFrequencyChangeLimit      ,sizeof(defaultAPRFrequencyFastFrequencyChangeLimit      )},
	{"defaultAdjSize1HzPSL[0]"                          ,&defaultAdjSize1HzPSL[0]                          ,sizeof(defaultAdjSize1HzPSL[0]                          )},
	{"defaultAdjSize1HzPSL[1]"                          ,&defaultAdjSize1HzPSL[1]                          ,sizeof(defaultAdjSize1HzPSL[1]                          )},
	{"defaultAdjSize1HzPSL[2]"                          ,&defaultAdjSize1HzPSL[2]                          ,sizeof(defaultAdjSize1HzPSL[2]                          )},
	{"defaultAdjSize1HzPSL[3]"                          ,&defaultAdjSize1HzPSL[3]                          ,sizeof(defaultAdjSize1HzPSL[3]                          )},
	{"defaultAdjSize1HzPSL[4]"                          ,&defaultAdjSize1HzPSL[4]                          ,sizeof(defaultAdjSize1HzPSL[4]                          )},
	{"defaultPSL_1Hz[0]"                                ,&defaultPSL_1Hz[0]                                ,sizeof(defaultPSL_1Hz[0]                                )},
	{"defaultPSL_1Hz[1]"                                ,&defaultPSL_1Hz[1]                                ,sizeof(defaultPSL_1Hz[1]                                )},
	{"defaultPSL_1Hz[2]"                                ,&defaultPSL_1Hz[2]                                ,sizeof(defaultPSL_1Hz[2]                                )},
	{"defaultPSL_1Hz[3]"                                ,&defaultPSL_1Hz[3]                                ,sizeof(defaultPSL_1Hz[3]                                )},
	{"defaultPSL_1Hz[4]"                                ,&defaultPSL_1Hz[4]                                ,sizeof(defaultPSL_1Hz[4]                                )},
	{"defaultAdjSize1HzFCL[0]"                          ,&defaultAdjSize1HzFCL[0]                          ,sizeof(defaultAdjSize1HzFCL[0]                          )},
	{"defaultAdjSize1HzFCL[1]"                          ,&defaultAdjSize1HzFCL[1]                          ,sizeof(defaultAdjSize1HzFCL[1]                          )},
	{"defaultAdjSize1HzFCL[2]"                          ,&defaultAdjSize1HzFCL[2]                          ,sizeof(defaultAdjSize1HzFCL[2]                          )},
	{"defaultAdjSize1HzFCL[3]"                          ,&defaultAdjSize1HzFCL[3]                          ,sizeof(defaultAdjSize1HzFCL[3]                          )},
	{"defaultAdjSize1HzFCL[4]"                          ,&defaultAdjSize1HzFCL[4]                          ,sizeof(defaultAdjSize1HzFCL[4]                          )},
	{"defaultFCL_1Hz[0]"                                ,&defaultFCL_1Hz[0]                                ,sizeof(defaultFCL_1Hz[0]                                )},
	{"defaultFCL_1Hz[1]"                                ,&defaultFCL_1Hz[1]                                ,sizeof(defaultFCL_1Hz[1]                                )},
	{"defaultFCL_1Hz[2]"                                ,&defaultFCL_1Hz[2]                                ,sizeof(defaultFCL_1Hz[2]                                )},
	{"defaultFCL_1Hz[3]"                                ,&defaultFCL_1Hz[3]                                ,sizeof(defaultFCL_1Hz[3]                                )},
	{"defaultFCL_1Hz[4]"                                ,&defaultFCL_1Hz[4]                                ,sizeof(defaultFCL_1Hz[4]                                )},
	{"defaultAdjSize1HzAdjScaling[0]"                   ,&defaultAdjSize1HzAdjScaling[0]                   ,sizeof(defaultAdjSize1HzAdjScaling[0]                   )},
	{"defaultAdjSize1HzAdjScaling[1]"                   ,&defaultAdjSize1HzAdjScaling[1]                   ,sizeof(defaultAdjSize1HzAdjScaling[1]                   )},
	{"defaultAdjSize1HzAdjScaling[2]"                   ,&defaultAdjSize1HzAdjScaling[2]                   ,sizeof(defaultAdjSize1HzAdjScaling[2]                   )},
	{"defaultAdjSize1HzAdjScaling[3]"                   ,&defaultAdjSize1HzAdjScaling[3]                   ,sizeof(defaultAdjSize1HzAdjScaling[3]                   )},
	{"defaultAdjSize1HzAdjScaling[4]"                   ,&defaultAdjSize1HzAdjScaling[4]                   ,sizeof(defaultAdjSize1HzAdjScaling[4]                   )},
	{"defaultAdjScaling_1Hz[0]"                         ,&defaultAdjScaling_1Hz[0]                         ,sizeof(defaultAdjScaling_1Hz[0]                         )},
	{"defaultAdjScaling_1Hz[1]"                         ,&defaultAdjScaling_1Hz[1]                         ,sizeof(defaultAdjScaling_1Hz[1]                         )},
	{"defaultAdjScaling_1Hz[2]"                         ,&defaultAdjScaling_1Hz[2]                         ,sizeof(defaultAdjScaling_1Hz[2]                         )},
	{"defaultAdjScaling_1Hz[3]"                         ,&defaultAdjScaling_1Hz[3]                         ,sizeof(defaultAdjScaling_1Hz[3]                         )},
	{"defaultAdjScaling_1Hz[4]"                         ,&defaultAdjScaling_1Hz[4]                         ,sizeof(defaultAdjScaling_1Hz[4]                         )},
	{"defaultPFLockInThreshold"                         ,&defaultPFLockInThreshold                         ,sizeof(defaultPFLockInThreshold                         )},
	{"defaultPFLockInCount"                             ,&defaultPFLockInCount                             ,sizeof(defaultPFLockInCount                             )},
	{"defaultPFLockOutThreshold"                        ,&defaultPFLockOutThreshold                        ,sizeof(defaultPFLockOutThreshold                        )},
	{"defaultMaxAdjFreqTime"                            ,&defaultMaxAdjFreqTime                            ,sizeof(defaultMaxAdjFreqTime                            )},
	{"defaultPFAdjustFreqMinPhase"                      ,&defaultPFAdjustFreqMinPhase                      ,sizeof(defaultPFAdjustFreqMinPhase                      )},
	{"defaultThresholdForFlagV"                         ,&defaultThresholdForFlagV                         ,sizeof(defaultThresholdForFlagV                         )},
	{"defaultSetTimePacketTreatment"                    ,&defaultSetTimePacketTreatment                    ,sizeof(defaultSetTimePacketTreatment                    )},
	{"defaultStepTimePacketTreatment"                   ,&defaultStepTimePacketTreatment                   ,sizeof(defaultStepTimePacketTreatment                   )},
	{"defaultAdjustTimePacketTreatment"                 ,&defaultAdjustTimePacketTreatment                 ,sizeof(defaultAdjustTimePacketTreatment                 )},
	{"defaultLegacyTreatment"                           ,&defaultLegacyTreatment                           ,sizeof(defaultLegacyTreatment                           )},
	{"exampleUseLegacyStreamStartUp"                    ,&exampleUseLegacyStreamStartUp                    ,sizeof(exampleUseLegacyStreamStartUp                    )},
	{"exampleAllow1HzAdjustmentsInHoldover"             ,&exampleAllow1HzAdjustmentsInHoldover             ,sizeof(exampleAllow1HzAdjustmentsInHoldover             )},
	{"defaultPFStepTimeResolution"                      ,&defaultPFStepTimeResolution                      ,sizeof(defaultPFStepTimeResolution                      )},
	{"defaultPFLockedPhaseOutlierThreshold"             ,&defaultPFLockedPhaseOutlierThreshold             ,sizeof(defaultPFLockedPhaseOutlierThreshold             )},
	{"defaultPFInitialFrequencyOffset"                  ,&defaultPFInitialFrequencyOffset                  ,sizeof(defaultPFInitialFrequencyOffset                  )},
	{"exampleOnlyAllowSteps"                            ,&exampleOnlyAllowSteps                            ,sizeof(exampleOnlyAllowSteps                            )},
	{"defaultAlgTypeMode"                               ,&defaultAlgTypeMode                               ,sizeof(defaultAlgTypeMode                               )},
	{"defaultbUseT2PLL"                                 ,&defaultbUseT2PLL                                 ,sizeof(defaultbUseT2PLL                                 )},
	{"defaultenableXOComp"                              ,&defaultenableXOComp                              ,sizeof(defaultenableXOComp                              )},
	{"defaultbT2PLLFLEnb"                               ,&defaultbT2PLLFLEnb                               ,sizeof(defaultbT2PLLFLEnb                               )},
	{"T2FLMinPhaseNs"                                   ,&T2FLMinPhaseNs                                   ,sizeof(T2FLMinPhaseNs                                   )},
	{"defaultOscillatorFilter"                          ,&defaultOscillatorFilter                          ,sizeof(defaultOscillatorFilter                          )},
	{"defaultOscHoldoverStability"                      ,&defaultOscHoldoverStability                      ,sizeof(defaultOscHoldoverStability                      )},
	{"defaultXOsModeTimeout"                            ,&defaultXOsModeTimeout                            ,sizeof(defaultXOsModeTimeout                            )},
	{"defaultSModeAgeOut"                               ,&defaultSModeAgeOut                               ,sizeof(defaultSModeAgeOut                               )},
	{"defaultReRouteHoldover2"                          ,&defaultReRouteHoldover2                          ,sizeof(defaultReRouteHoldover2                          )},
	{"defaultFilter"                                    ,&defaultFilter                                    ,sizeof(defaultFilter                                    )},
	{"defaultXdslHpFlag"                                ,&defaultXdslHpFlag                                ,sizeof(defaultXdslHpFlag                                )},
	{"defaultFwdPPS"                                    ,&defaultFwdPPS                                    ,sizeof(defaultFwdPPS                                    )},
	{"defaultRevPPS"                                    ,&defaultRevPPS                                    ,sizeof(defaultRevPPS                                    )},
	{"defaultTsFormat"                                  ,&defaultTsFormat                                  ,sizeof(defaultTsFormat                                  )},
	{"defaultTs32BitFlag"                               ,&defaultTs32BitFlag                               ,sizeof(defaultTs32BitFlag                               )},
	{"zl303xx_AprUseReversePath"                        ,&zl303xx_AprUseReversePath                        ,sizeof(zl303xx_AprUseReversePath                        )},
	{"defaultHybridServerFlag"                          ,&defaultHybridServerFlag                          ,sizeof(defaultHybridServerFlag                          )},
	{"defaultPacketDiscardDurationInSecFlag"            ,&defaultPacketDiscardDurationInSecFlag            ,sizeof(defaultPacketDiscardDurationInSecFlag            )},
	{"defaultPullInRange"                               ,&defaultPullInRange                               ,sizeof(defaultPullInRange                               )},
	{"defaultEnterHoldeverGST"                          ,&defaultEnterHoldeverGST                          ,sizeof(defaultEnterHoldeverGST                          )},
	{"defaultExitValidGST"                              ,&defaultExitValidGST                              ,sizeof(defaultExitValidGST                              )},
	{"defaultExitLockGST"                               ,&defaultExitLockGST                               ,sizeof(defaultExitLockGST                               )},
	{"defaultExitPhaseAlignGST"                         ,&defaultExitPhaseAlignGST                         ,sizeof(defaultExitPhaseAlignGST                         )},
	{"defaultLockMasks"                                 ,&defaultLockMasks                                 ,sizeof(defaultLockMasks                                 )},
	{"FastLockTotalTimeInSecs"                          ,&FastLockTotalTimeInSecs                          ,sizeof(FastLockTotalTimeInSecs                          )},
	{"HoldoverFreezeValue"                              ,&HoldoverFreezeValue                              ,sizeof(HoldoverFreezeValue                              )},
	{"AprTaskBasePeriodMs"                              ,&AprTaskBasePeriodMs                              ,sizeof(AprTaskBasePeriodMs                              )},
	{"PslFclTaskBasePeriodMs"                           ,&PslFclTaskBasePeriodMs                           ,sizeof(PslFclTaskBasePeriodMs                           )},
	{"DFSeedValue"                                      ,&DFSeedValue                                      ,sizeof(DFSeedValue                                      )},
	{"Type2BFastlockStartupIt"                          ,&Type2BFastlockStartupIt                          ,sizeof(Type2BFastlockStartupIt                          )},
	{"Type2BFastlockThreshold"                          ,&Type2BFastlockThreshold                          ,sizeof(Type2BFastlockThreshold                          )},
	{"OutlierTimerValue"                                ,&OutlierTimerValue                                ,sizeof(OutlierTimerValue                                )},
	{"ClkInvalidCntrValue"                              ,&ClkInvalidCntrValue                              ,sizeof(ClkInvalidCntrValue                              )},
	{"FastLockPktSelWindowSize"                         ,&FastLockPktSelWindowSize                         ,sizeof(FastLockPktSelWindowSize                         )},
	{"L2phase_varLimitValue"                            ,&L2phase_varLimitValue                            ,sizeof(L2phase_varLimitValue                            )},
	{"L4Threshold"                                      ,&L4Threshold                                      ,sizeof(L4Threshold                                      )},
	{"DefaultUseOFM"                                    ,&DefaultUseOFM                                    ,sizeof(DefaultUseOFM                                    )},
	{"FastLockBW"                                       ,&FastLockBW                                       ,sizeof(FastLockBW                                       )},
	{"exampleAprFCL"                                    ,&exampleAprFCL                                    ,sizeof(exampleAprFCL                                    )},
	{"fastLock1HzEnable"                                ,&fastLock1HzEnable                                ,sizeof(fastLock1HzEnable                                )},
	{"fastLock1HzInterval"                              ,&fastLock1HzInterval                              ,sizeof(fastLock1HzInterval                              )},
	{"fastLock1HzTotalPeriod"                           ,&fastLock1HzTotalPeriod                           ,sizeof(fastLock1HzTotalPeriod                           )},
	{"fastLock1HzDelay"                                 ,&fastLock1HzDelay                                 ,sizeof(fastLock1HzDelay                                 )},
	{"stepTimeAdjustTimeThreshold"                      ,&stepTimeAdjustTimeThreshold                      ,sizeof(stepTimeAdjustTimeThreshold                      )},
	{"stepTimeAdjustFreqThreshold"                      ,&stepTimeAdjustFreqThreshold                      ,sizeof(stepTimeAdjustFreqThreshold                      )},
	{"defaultAdjustTimeMinThreshold"                    ,&defaultAdjustTimeMinThreshold                    ,sizeof(defaultAdjustTimeMinThreshold                    )},
	{"defaultUseAdjustTimeHybrid"                       ,&defaultUseAdjustTimeHybrid                       ,sizeof(defaultUseAdjustTimeHybrid                       )},
	{"defaultUseAdjustTimePacket"                       ,&defaultUseAdjustTimePacket                       ,sizeof(defaultUseAdjustTimePacket                       )},
	{"defaultStepTimeDetectableThr"                     ,&defaultStepTimeDetectableThr                     ,sizeof(defaultStepTimeDetectableThr                     )},
	{"defaultAprDynamicPhaseCorrectionEnabled"          ,&defaultAprDynamicPhaseCorrectionEnabled          ,sizeof(defaultAprDynamicPhaseCorrectionEnabled          )},
	{"defaultAdjFreqDynamicAdjustmentEnable"            ,&defaultAdjFreqDynamicAdjustmentEnable            ,sizeof(defaultAdjFreqDynamicAdjustmentEnable            )},
	{"defaultAprDynamicPhaseCorrectionThr"              ,&defaultAprDynamicPhaseCorrectionThr              ,sizeof(defaultAprDynamicPhaseCorrectionThr              )},
	{"DefaultUseNCOAssistDpll"                          ,&DefaultUseNCOAssistDpll                          ,sizeof(DefaultUseNCOAssistDpll                          )},
	{"defaultHybridLockTimeTarget"                      ,&defaultHybridLockTimeTarget                      ,sizeof(defaultHybridLockTimeTarget                      )},
	{"clkStabDelayIter"                                 ,&clkStabDelayIter                                 ,sizeof(clkStabDelayIter                                 )},
	{"default1HzEnabledFlag"                            ,&default1HzEnabledFlag                            ,sizeof(default1HzEnabledFlag                            )},
	{"startupTimeInit"                                  ,&startupTimeInit                                  ,sizeof(startupTimeInit                                  )},
	{""                                                 ,NULL                                              ,0                                                        }
};

zlStatusE zl303xx_FJ_AprGetValue(char* name,void* value)
{
	Uint32T count;
	zlStatusE result = ZL303XX_PARAMETER_INVALID;
	int ret;

	for(count=0;count < (sizeof(FJ_APR_PARM)/sizeof(FJ_APR_PARM[0])); count++){
		if(FJ_APR_PARM[count].size == 0){
			break;
		}
		ret = strncmp(name,FJ_APR_PARM[count].name,strlen(FJ_APR_PARM[count].name));
		if(ret == 0){
			memcpy(value,FJ_APR_PARM[count].tblPtr,FJ_APR_PARM[count].size);
			result = ZL303XX_OK;
			break;
		}
	}
	return result;
}


void zl303xx_FJ_AprGetPrint(void* fp)
{
	Uint32T count;
	Uint32T value;
	FILE* wfp=fp;
	
	for(count=0;count < (sizeof(FJ_APR_PARM)/sizeof(FJ_APR_PARM[0])); count++){
		if(FJ_APR_PARM[count].size == 0){
			break;
		}
		memcpy(&value,FJ_APR_PARM[count].tblPtr,FJ_APR_PARM[count].size);
		fprintf(wfp,"Id:%3d %-50s size:%02d value:%10d(0x%08x)\n",count,FJ_APR_PARM[count].name,FJ_APR_PARM[count].size,value,value);
	}
	return;
}

zlStatusE zl303xx_FJ_AprInitSetValue(void)
{
	Uint32T count;
	zlStatusE result = ZL303XX_PARAMETER_INVALID;
	T_RRH_PTP_APR_PARM*	aprparm;
	int endcd;
	int errcd;
	
	endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_PTP_TIME_ADJ,  		/* 共有メモリ番号               */
		(void **)&aprparm,    			   	/* 共有メモリアドレス           */
		&errcd                              /* 詳細NGコード                 */
	);

	if(endcd != 0){
		return result;
	}

	if(aprparm->saveflg == 0x0){
		return result;
	}

	if(aprparm->length > sizeof(T_RRH_PTP_APR_PARM)){
		return result;
	}

	for(count=0;count < (sizeof(FJ_APR_PARM)/sizeof(FJ_APR_PARM[0])); count++){
		if(FJ_APR_PARM[count].size == 0){
			break;
		}
		if(aprparm->info[count].valid == 0){
			continue;
		}
		if(aprparm->info[count].index != count){
			continue;
		}
		if(aprparm->info[count].len != FJ_APR_PARM[count].size){
			continue;
		}
		memcpy(FJ_APR_PARM[count].tblPtr,&(aprparm->info[count].val),FJ_APR_PARM[count].size);
		result = ZL303XX_OK;
	}
	return result;
}




/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/*****************   END   ****************************************************/
