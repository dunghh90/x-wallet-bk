

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     ZL3077X DPLL function and function bindings for APR.
*
*******************************************************************************/

#ifndef ZL303XX_DPLL_77X_H_
#define ZL303XX_DPLL_77X_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_AddressMap77x.h"
#include "zl303xx_DeviceSpec.h"
#include "zl303xx_DeviceIf.h"

/*****************   DEFINES   ************************************************/

/* Scaled dco count per ppm for 77x. Each 1 count of the 77x register is:
      x           1
   ------- = -----------    => x = 281474977
     2^48    1000000(ppm)
*/
#define ZLS3077X_DCO_COUNT_PER_PPM 281474977

#define ZLS3077X_REG_READ_INTERVAL (10)

#define ZLS3077X_TIMER_DELAY_MS (1000)
#if defined ZLE30360_770_FRANKEN_BOARD
#define ZLS3077X_GUARD_TIMER_LENGTH_IN_SEC (30)
#else
#define ZLS3077X_GUARD_TIMER_LENGTH_IN_SEC (1)
#endif
#define ZLS3077X_STEP_TIME_SANITY_TIMEROUT (60)


/*****************   DATA TYPES   *********************************************/

typedef struct {
    Uint32T GPOUT0Freq;
    Uint32T GPOUT1Freq;
    Uint32T HPSynth1IntFreq;
    Uint32T HPSynth1FracFreq;
    Uint32T HPSynth2IntFreq;
    Uint32T HPSynth2FracFreq;
    struct {
        Uint32T msdivFreq;
        Uint32T lsdivFreq;
    } freq[8];
} zl303xx_Dpll77xOutputConfS;

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/* Short sanity timeout when accessing registers that should return quickly */
extern Uint32T Dpll77xShortSanityTO;
/* Medium sanity timeout when accessing registers */
extern Uint32T Dpll77xMeduiumSanityTO;
/* Long sanity timeout when accessing registers */
extern Uint32T Dpll77xLongSanityTO;

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/* Init */
zlStatusE zl303xx_Dpll77xParamsMutexInit(void *hwParams);
zlStatusE zl303xx_Dpll77xParamsMutexDelete(void *hwParams);
zlStatusE zl303xx_Dpll77xParamsInit(void *hwParams);
zlStatusE zl303xx_Dpll77xParamsClose(void *hwParams);

/* Mailbox */
zlStatusE zl303xx_Dpll77xGetMailBoxSemaphoreStatus(void *hwParams,
                ZLS3077X_MailboxE mbId, ZLS3077X_SemStatusE *semStatus);
zlStatusE zl303xx_Dpll77xWaitForAvailableMailBoxSemaphore(void *hwParams,
                                        ZLS3077X_MailboxE mbId);
zlStatusE zl303xx_Dpll77xSetupMailboxForRead(void *hwParams, ZLS3077X_MailboxE mbId,
                                        Uint32T mbNum);
zlStatusE zl303xx_Dpll77xSetupMailboxForWrite(void *hwParams, ZLS3077X_MailboxE mbId,
                                    Uint32T mbNum);
zlStatusE zl303xx_Dpll77xSetMailboxSemWrite(void *hwParams, ZLS3077X_MailboxE mbId);
zlStatusE zl303xx_Dpll77xUpdateMailboxCopy(void *hwParams, ZLS3077X_MailboxE mbId,
                                Uint32T mbNum);
zlStatusE zl303xx_Dpll77xUpdateAllMailboxCopies(void *hwParams);

/* Configure/Status */
zlStatusE zl303xx_Ref77xMonBitsClear(void *hwParams, Uint32T refId);
zlStatusE zl303xx_Dpll77xStickyLockSet(void *zl303xx_Params,
                                       ZLS3077X_StickyLockE lock);
Sint32T zl303xx_Dpll77xSetFreq(void *hwParams, Sint32T freqOffsetUppm);
Sint32T zl303xx_Dpll77xGetHwLockStatus(void *hwParams, Sint32T *lockStatus);
Sint32T zl303xx_Dpll77xGetHwManualHoldoverStatus(void *hwParams, Sint32T *status);
zlStatusE zl303xx_Dpll77xMitigationEnabledSet(void *hwParams, zl303xx_BooleanE mitigationEnabled);
Sint32T zl303xx_Dpll77xGetHwManualFreerunStatus(void *hwParams, Sint32T *status);
zlStatusE zl303xx_Ref77xFreqDetectedGet(void *zl303xx_Params,
                                        Uint32T refId, Uint32T *freqHz);
Sint32T zl303xx_Dpll77xGetHwSyncInputEnStatus(void *hwParams, Sint32T *en);
Sint32T zl303xx_Dpll77xGetHwOutOfRangeStatus(void *hwParams, Sint32T *oor);
zlStatusE zl303xx_Dpll77xOverridePllPriorMode(void *hwParams,
                    ZLS3077X_DpllHWModeE overrridePllPriorMode, zl303xx_RefIdE refId);
zlStatusE zl303xx_Dpll77xModeSet(void *hwParams, ZLS3077X_DpllHWModeE mode);
zlStatusE zl303xx_Dpll77xModeGet(void *hwParams, ZLS3077X_DpllHWModeE *mode);
zlStatusE zl303xx_Dpll77xHWModeSet(void *hwParams, ZLS3077X_DpllHWModeE mode,
                                Uint8T refId, Sint32T freqOffsetToSetInppt);
zlStatusE zl303xx_Dpll77xCurrRefSet(void *hwParams,  Uint8T refId);
zlStatusE zl303xx_Dpll77xCurrRefGet(void *hwParams, Uint32T *refId);
zlStatusE zl303xx_Dpll77xTieClearGet(void *hwParams, zl303xx_BooleanE *tieClear);
zlStatusE zl303xx_Dpll77xTieClearSet(void *hwParams, zl303xx_BooleanE tieClear);
zlStatusE zl303xx_Dpll77xFastLockSet(void *hwParams, zl303xx_BooleanE fastLock);
zlStatusE zl303xx_Dpll77xFastLockGet(void *hwParams, zl303xx_BooleanE *fastLock);
zlStatusE zl303xx_Dpll77xPullInRangeSet(void *hwParams, Uint32T PullInRange);
zlStatusE zl303xx_Dpll77xPullInRangeGet(void *hwParams, Uint32T *PullInRange);
zlStatusE zl303xx_Dpll77xPhaseSlopeLimitSet(void *hwParams, Uint32T PSL);
zlStatusE zl303xx_Dpll77xPhaseSlopeLimitGet(void *hwParams, Uint32T *PSL);
zlStatusE zl303xx_Dpll77xBandwidthSet(void *hwParams, ZLS3077X_BandwidthE Bandwidth);
zlStatusE zl303xx_Dpll77xBandwidthGet(void *hwParams, ZLS3077X_BandwidthE *Bandwidth);
zlStatusE zl303xx_Dpll77xBandwidthCustomSet(void *hwParams, Uint32T BandwidthCustom);
zlStatusE zl303xx_Dpll77xBandwidthCustomGet(void *hwParams, Uint32T *BandwidthCustom);
zlStatusE zl303xx_Dpll77xGetOutputClockConfig(void *hwParams,
                                            zl303xx_Dpll77xOutputConfS *conf);
zlStatusE zl303xx_Dpll77xPhaseStepMaskSet(void *hwParams,
                                            ZLS3077X_OutputTypesE outputType,
                                            ZLS3077X_OutputsE outputNum,
                                            Uint32T v);
zlStatusE zl303xx_Dpll77xOutputThatDrivesTimeStamperSet(void *hwParams,
                                            ZLS3077X_OutputTypesE outputType,
                                            ZLS3077X_OutputsE outputNum);
zlStatusE zl303xx_Dpll77xPhaseStepMaskGet(void *hwParams,
                        Uint32T *phaseStepMaskGp,
                        Uint32T *phaseStepMaskHp,
                        ZLS3077X_OutputTypesE *outputTypeThatDrivesTimeStamper,
                        ZLS3077X_OutputsE *outputNumThatDrivesTimeStamper);
zlStatusE zl303xx_Dpll77xModeSwitchFastLockEnableSet(void *hwParams,
                zl303xx_BooleanE nco, zl303xx_BooleanE force, zl303xx_BooleanE en);
zlStatusE zl303xx_Dpll77xInputPWMSet(void *hwParams, Uint32T refId,
                                    ZLS3077X_RefSynchPairModeE mode);
zlStatusE zl303xx_Dpll77xInputPWMGet(void *hwParams, Uint32T refId,
                                    ZLS3077X_RefSynchPairModeE *mode);
zlStatusE zl303xx_Dpll77xNCOAutoReadSet(void *hwParams,
                                    zl303xx_BooleanE enabled);
zlStatusE zl303xx_Dpll77xNCOAutoReadGetData(void *hwParams,
                                    Sint32T *freqOffsetInppt);
zlStatusE zl303xx_Dpll77xPhaseStepMaxSet(void *hwParams, Uint32T psm);

/* Output step */
zlStatusE zl303xx_Dpll77xOutputPhaseStepStatusGet(void *hwParams, Uint32T *complete);
zlStatusE zl303xx_Dpll77xWaitForPhaseCtrlReady(void *hwParams);
zlStatusE zl303xx_Dpll77xStepTimeClearOutputStickyBits(void *hwParams);
zlStatusE zl303xx_Dpll77xOutputPhaseErrorClear(void *hwParams, Sint32T phaseToRemove,
                                                Uint32T clockFreq);
zlStatusE zl303xx_Dpll77xModifyStepTimeNs(void *hwParams, Sint32T deltaTime,
                                       Sint32T *modDeltaTime);
zlStatusE zl303xx_Dpll77xOutputPhaseStepRead(void *hwParams, Uint32T clockFreq,
                        Sint32T *phaseInFreqClockCycles, Sint32T *accumPhase);
Sint32T zl303xx_Dpll77xOutputPhaseStepWrite(void *hwParams, Sint32T deltaTime,
                        zl303xx_BooleanE inCycles, Uint32T clockFreq);
zlStatusE zl303xx_Dpll77xStepTimePhaseGet(void *hwParams, Sint32T *tieNs);
zlStatusE zl303xx_Dpll77xAccuStepTimePhase1588HostRegSet(void *hwParams, Sint32T tieNs);
zlStatusE zl303xx_Dpll77xAccuStepTimePhase1588HostRegGet(void *hwParams, Sint32T *tieNs);
zlStatusE zl303xx_Dpll77xAccuInputPhaseError1588HostRegSet(void *hwParams, Sint32T tieNs);
zlStatusE zl303xx_Dpll77xAccuInputPhaseError1588HostRegGet(void *hwParams, Sint32T *tieNs);

/* Redundancy */
Sint32T zl303xx_Dpll77xjumpActiveCGU(void* hwParams, Uint64S seconds,
                            Uint32T nanoSeconds, zl303xx_BooleanE bBackwardAdjust);

/* time-of-day (ToD) */
zlStatusE zl303xx_Dpll77xToDWaitForSpecificRollover(void *hwParams, Uint32T timeoutMs,
                        ZLS3077X_TODReadTypeE readType, Sint32T initNs,
                        Uint64S *sec, Uint32T *ns);
zlStatusE zl303xx_Dpll77xToDWaitForRollover(void *hwParams, Uint32T timeoutMs,
                                ZLS3077X_TODReadTypeE readType,
                                Uint64S *sec, Uint32T *ns);
zlStatusE zl303xx_Dpll77xLatchedToDReadSetup(void *hwParams, ZLS3077X_TODReadTypeE readType);
zlStatusE zl303xx_Dpll77xToDRead(void *hwParams,
                                ZLS3077X_TODReadTypeE readType,
                                Uint64S *sec, Uint32T *ns);
zlStatusE zl303xx_Dpll77xToDWrite(void *hwParams, ZLS3077X_ToDWriteTypeE writeType,
                        zl303xx_BooleanE relativeAdjust,
                        Uint64S sec, Uint32T ns, zl303xx_BooleanE bBackwardAdjust);

/* Hitless compensation */
Sint32T zl303xx_Dpll77xAdjustIfHitlessCompensationBeingUsed(void *hwParams,
                        zl303xx_HitlessCompE hitlessType,
                        zl303xx_BooleanE *apply);
/* Misc */
zl303xx_BooleanE zl303xx_Dpll77xInNCOMode(void *hwParams);
zlStatusE zl303xx_Dpll77xGetRefSyncPairMode(void *hwParams, Uint32T refId, Uint32T *syncId,
                                        zl303xx_BooleanE *bRefSyncPair, zl303xx_BooleanE *bEPPS );
zlStatusE zl303xx_Dpll77xSetRefSyncPair(void *hwParams, Uint32T refId, Uint32T syncId);
zlStatusE zl303xx_Dpll77xReset1588HostRegisters(void *hwParams);
zlStatusE zl303xx_Dpll77xInputPhaseErrorWrite(void *hwParams, Sint32T tieNs,
                                                zl303xx_BooleanE snapAlign);

/* task */
zlStatusE zl303xx_Dpll77xStoreDeviceParam(void *hwParams);
zlStatusE zl303xx_Dpll77xRemoveDeviceParam(void *hwParams);
zlStatusE zl303xx_Dpll77xRegisterStepDoneFunc(void *hwParams,
                        hwFuncPtrTODDone stepDoneFuncPtr);

/* NCO-Assist DPLL */
zlStatusE zl303xx_Dpll77xSetNCOAssistParamsSAssociation(zl303xx_ParamsS *APR,
                                                      zl303xx_ParamsS *NCOAssist);
zlStatusE zl303xx_Dpll77xGetNCOAssistParamsSAssociation(zl303xx_ParamsS *APR,
                                                      zl303xx_ParamsS **NCOAssist);

Sint32T zl303xx_Dpll77xGetNCOAssistHwLockStatus(void *hwParams, Sint32T *lockStatus);

#if defined ZLS30771_INCLUDED
zlStatusE zl303xx_Dpll77xTaskStart(void);
zlStatusE zl303xx_Dpll77xTaskStop(void);
#endif




#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
