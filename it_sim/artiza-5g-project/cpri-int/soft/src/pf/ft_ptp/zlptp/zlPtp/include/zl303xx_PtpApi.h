



/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     PTP API
*
*******************************************************************************/

#ifndef _ZL303XX_PTP_API_H_
#define _ZL303XX_PTP_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"          /* Basic ZL datatypes. */
#include "zl303xx_Error.h"           /* For zlStatusE return type. */

#include "zl303xx_PtpApiTypes.h"     /* PTP API datatypes. */
#include "zl303xx_PtpDeprecated.h"   /* Support for deprecated function calls. */
#include "zl303xx_PtpDatatypes.h"    /* PTP data types */

/*****************   DEFINES   ************************************************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/*** PTP MANAGEMENT FUNCTIONS ***/
zlStatusE zl303xx_PtpInit(void);
zlStatusE zl303xx_PtpShutdown(void);

/*** CLOCK FUNCTIONS ***/
zlStatusE zl303xx_PtpClockCreateStructInit(zl303xx_PtpClockCreateS *pClockCreate);
zlStatusE zl303xx_PtpProfileClockStructInit(zl303xx_PtpProfileE profileId,
                                            zl303xx_PtpClockCreateS *pClockCreateS);
zlStatusE zl303xx_PtpClockCreate(zl303xx_PtpClockCreateS *pClockCreate,
                                 zl303xx_PtpClockHandleT *pClockHandle);

zlStatusE zl303xx_PtpClockDeleteStructInit(zl303xx_PtpClockDeleteS *pClockDelete);
zlStatusE zl303xx_PtpClockDelete(zl303xx_PtpClockHandleT clockHandle,
                                 zl303xx_PtpClockDeleteS *pClockDelete);

zlStatusE zl303xx_PtpClockConfigGet(zl303xx_PtpClockHandleT clockHandle,
                                    zl303xx_PtpClockCreateS *pClockConfig);
zlStatusE zl303xx_PtpClockConfigSet(zl303xx_PtpClockHandleT clockHandle,
                                    zl303xx_PtpClockCreateS *pClockConfig);
zlStatusE zl303xx_PtpClockExtDataGet(zl303xx_PtpClockHandleT clockHandle,
                                     void **ppExtData);
zlStatusE zl303xx_PtpClockExtDataSet(zl303xx_PtpClockHandleT clockHandle,
                                     void *pExtData);

zlStatusE zl303xx_PtpClockEventCallbackSet(zl303xx_PtpClockHandleT clockHandle,
                                           zl303xx_PtpEventFnT eventCallback);
zlStatusE zl303xx_PtpClockHardwareCmdFnSet(zl303xx_PtpClockHandleT clockHandle,
                                           zl303xx_PtpHwCmdFnT hwCmdFn);

zlStatusE zl303xx_PtpClockEnabledGet(zl303xx_PtpClockHandleT clockHandle,
                                     zl303xx_BooleanE *pEnabled);

/* Default Data Set Routines */
zlStatusE zl303xx_PtpClockDefaultDSDefaultParams(zl303xx_DefaultDS *pDefaultDS);
zlStatusE zl303xx_PtpClockDefaultDSGet(zl303xx_PtpClockHandleT clockHandle, zl303xx_DefaultDS *pDefaultDS);
zlStatusE zl303xx_PtpClockDefaultDSGetLocal(zl303xx_PtpClockHandleT clockHandle, zl303xx_DefaultDS *pDefaultDS);
zlStatusE zl303xx_PtpClockDefaultDSSet(zl303xx_PtpClockHandleT clockHandle, zl303xx_DefaultDS *pDefaultDS);

zlStatusE zl303xx_PtpClockQualityDefaultParams(zl303xx_ClockQuality *pClockQuality);
zlStatusE zl303xx_PtpClockQualitySet(zl303xx_PtpClockHandleT clockHandle, zl303xx_ClockQuality *pClockQuality);
zlStatusE zl303xx_PtpClockClassSet(zl303xx_PtpClockHandleT clockHandle, Uint8T clockClass);

zlStatusE zl303xx_PtpClockSlaveOnlySet(zl303xx_PtpClockHandleT clockHandle, zl303xx_BooleanE slaveOnly);
zlStatusE zl303xx_PtpClockTwoStepFlagSet(zl303xx_PtpClockHandleT clockHandle, zl303xx_BooleanE twoStepFlag);
zlStatusE zl303xx_PtpClockDomainNumberSet(zl303xx_PtpClockHandleT clockHandle, Uint8T domainNumber);

zlStatusE zl303xx_PtpClockTypeSet(zl303xx_PtpClockHandleT clockHandle, Uint16T clockType);
zlStatusE zl303xx_PtpClockRevertiveEnSet(zl303xx_PtpClockHandleT clockHandle, zl303xx_BooleanE revertiveEn);
zlStatusE zl303xx_PtpClockPrioritySet(zl303xx_PtpClockHandleT clockHandle, Uint8T *priority1, Uint8T *priority2);
zlStatusE zl303xx_PtpClockManualSwitch(zl303xx_PtpClockHandleT clockHandle, zl303xx_PortAddress *pPortAddr);
zlStatusE zl303xx_PtpClockUnicastStackedTlvEnSet(zl303xx_PtpClockHandleT clockHandle,
                                                 zl303xx_BooleanE stackedTlvEn);
zlStatusE zl303xx_PtpClockStackAllServicesOn1stRequest(zl303xx_PtpClockHandleT clockHandle,
                                                 zl303xx_BooleanE stackInitialRequest);
zlStatusE zl303xx_PtpClockBmcaUpdateIntervalSet(zl303xx_PtpClockHandleT clockHandle, Uint32T intervalSec);
zlStatusE zl303xx_PtpClockBmcaUpdateTriggerSet(zl303xx_PtpClockHandleT clockHandle, zl303xx_PtpClockBmcaTriggerE updateTrigger);

zlStatusE zl303xx_PtpClockBmcaFlagClear(zl303xx_PtpClockHandleT clockHandle, zl303xx_BMCAFlagsE flagBit);
zlStatusE zl303xx_PtpClockBmcaFlagStatusGet(zl303xx_PtpClockHandleT clockHandle,zl303xx_BMCAFlagsE flagBit,
                                                                       zl303xx_BooleanE *pFlagStatus);

/* Current Data Set Routines */
zlStatusE zl303xx_PtpClockCurrentDSGet(zl303xx_PtpClockHandleT clockHandle, zl303xx_CurrentDS *pCurrentDS);

/* Parent Data Set Routines */
zlStatusE zl303xx_PtpClockParentDSGet(zl303xx_PtpClockHandleT clockHandle, zl303xx_ParentDS *pParentDS);
zlStatusE zl303xx_PtpClockParentDSSet(zl303xx_PtpClockHandleT clockHandle, zl303xx_ParentDS *pParentDS, zl303xx_PortIdentity *receivingPortIdentity);
zlStatusE zl303xx_PtpClockParentDsSourceSet(zl303xx_PtpClockHandleT clockHandle, zl303xx_ParentDS *pParentDS, zl303xx_PtpClockServerSourceS *serverSource);

/* Routine to set Clock Data Sets based on a Comparison Data Structure. */
zlStatusE zl303xx_PtpClockApplyCompareData(zl303xx_PtpClockHandleT clockHandle, zl303xx_PtpBmcCompareDataS *pCompareDS);

/* Time Properties Data Set Routines */
zlStatusE zl303xx_PtpClockTimePropertiesDSGet(zl303xx_PtpClockHandleT clockHandle,
                                              zl303xx_TimePropertiesDS *pTimePropertiesDS);
zlStatusE zl303xx_PtpTimePropertiesDefaultParams(zl303xx_TimePropertiesDS *pTimeProperties);
zlStatusE zl303xx_PtpTimePropertiesSetLocal(zl303xx_PtpClockHandleT clockHandle,
                                            zl303xx_TimePropertiesDS *pTimeProperties);
zlStatusE zl303xx_PtpTimePropertiesGetLocal(zl303xx_PtpClockHandleT clockHandle,
                                            zl303xx_TimePropertiesDS *pTimeProperties);
zlStatusE zl303xx_PtpCurrentUtcOffsetSetLocal(zl303xx_PtpClockHandleT clockHandle,
                                              Sint16T localUtcOffset);

zlStatusE zl303xx_PtpClockTaiGet(zl303xx_PtpClockHandleT clockHandle, zl303xx_TimeStamp *pTai);
zlStatusE zl303xx_PtpClockUtcGet(zl303xx_PtpClockHandleT clockHandle, zl303xx_TimeStamp *pUtc);
zlStatusE zl303xx_PtpClockUtcGetWithValid(zl303xx_PtpClockHandleT clockHandle, zl303xx_TimeStamp *pUtc, zl303xx_BooleanE *utcValid);

/* Telecom Profile for Frequency-specific functions. */
zlStatusE zl303xx_PtpG8265p1ClockQLGet(zl303xx_PtpClockHandleT clockHandle,
                                       zl303xx_PtpTelecomQualityLevelS *ql);
zlStatusE zl303xx_PtpClockWaitToRestoreSet(zl303xx_PtpClockHandleT clockHandle, Uint32T wtrSec);
zlStatusE zl303xx_PtpClockEvtSquelchEnSet(zl303xx_PtpClockHandleT clockHandle, zl303xx_BooleanE enabled);
zlStatusE zl303xx_PtpClockQLHoldOffSet(zl303xx_PtpClockHandleT clockHandle, Uint32T holdOffSec);
zlStatusE zl303xx_DebugTelecomBMCA(zl303xx_PtpClockHandleT clockHandle);

/* Clock level over-ride functions. */
zlStatusE zl303xx_PtpClockIngressClockClassSet(zl303xx_PtpClockHandleT clockHandle, Uint8T ingressClass);
zlStatusE zl303xx_PtpClockEgressClockClassSet(zl303xx_PtpClockHandleT clockHandle, Uint8T egressClass);
zlStatusE zl303xx_PtpClockEgressQlUpdateSet(zl303xx_PtpClockHandleT clockHandle, zl303xx_PtpClockEgressQlUpdateE qlUpdate);
zlStatusE zl303xx_PtpClockEgressOverrideSet(zl303xx_PtpClockHandleT clockHandle, zl303xx_PtpClockOverrideEgressE overrideId, void *overrideValue);
zlStatusE zl303xx_PtpClockEgressOverrideShow(zl303xx_PtpClockHandleT clockHandle);

/* The maximum value of the ANNOUNCE stepsRemoved field that the clock will allow. */
zlStatusE zl303xx_PtpClockStepsRemovedMaxSet(zl303xx_PtpClockHandleT clockHandle, Uint16T maxStepsRemoved);

/* How the clock handles the Class during UNCALIBRATED phase. */
zlStatusE zl303xx_PtpClockUncalibratedUsePreviousClassSet(zl303xx_PtpClockHandleT clockHandle, zl303xx_BooleanE usePreviousClass);

/* Stream State update mode for the Clock. */
zlStatusE zl303xx_PtpClockStreamStateAutoUpdateEn(zl303xx_PtpClockHandleT clockHandle, zl303xx_BooleanE enabled);

/* PATH TRACE functionality for the clock. */
zlStatusE zl303xx_PtpClockPathTraceEnable(zl303xx_PtpClockHandleT clockHandle, zl303xx_BooleanE enable);
zlStatusE zl303xx_PtpClockPathTraceListGet(zl303xx_PtpClockHandleT clockHandle,
                                           Uint16T *listCountMax,
                                           zl303xx_ClockIdentity * pPtList);
zlStatusE zl303xx_PtpClockPathTraceForExtParent(zl303xx_PtpClockHandleT clockHandle,
                                                Uint16T listCount,
                                                zl303xx_ClockIdentity * pPtList);

/* Synchronization Uncertain functionality for the clock. */
zlStatusE zl303xx_PtpClockSynchronizationUncertainEn(zl303xx_PtpClockHandleT clockHandle,
                                                     zl303xx_BooleanE enable);

/* Command to Force the Slave selection of a Clock. */
zlStatusE zl303xx_PtpClockForceSlaveStreamSet(zl303xx_PtpClockHandleT clockHandle, zl303xx_PtpStreamHandleT streamHandle);
/* Command to Force the Clock to upload its Foreign Master Table. */
zlStatusE zl303xx_PtpClockForceBmcaUpdateEvent(zl303xx_PtpClockHandleT clockHandle);

/* Clock level contract info/limits */
zlStatusE zl303xx_PtpClockContractCountGet(zl303xx_PtpClockHandleT clockHandle,
                                           zl303xx_MessageTypeE msgType,
                                           Uint32T *count);
zlStatusE zl303xx_PtpClockContractCountMaxGet(zl303xx_PtpClockHandleT clockHandle,
                                              zl303xx_MessageTypeE msgType,
                                              Uint32T *limit);
zlStatusE zl303xx_PtpClockContractCountMaxSet(zl303xx_PtpClockHandleT clockHandle,
                                              zl303xx_MessageTypeE msgType,
                                              Uint32T limit);

/* Clock level PPS info/limits */
zlStatusE zl303xx_PtpClockTotalPpsMaxSet(zl303xx_PtpClockHandleT clockHandle,
                                         zl303xx_MessageTypeE msgType,
                                         Uint32T totalPpsMax);
zlStatusE zl303xx_PtpClockTotalPpsGet(zl303xx_PtpClockHandleT clockHandle,
                                      zl303xx_PtpTotalPpsS *pTotal);

zlStatusE zl303xx_PtpClockMessagingEnabled(zl303xx_PtpClockHandleT clockHandle,
                                           zl303xx_BooleanE enabled);

/*** PORT FUNCTIONS ***/
zlStatusE zl303xx_PtpPortCreateStructInit(zl303xx_PtpPortCreateS *pPortCreate);
zlStatusE zl303xx_PtpProfilePortStructInit(zl303xx_PtpProfileE profileId,
                                           zl303xx_PtpPortCreateS *pPortCreateS);
zlStatusE zl303xx_PtpPortCreate(zl303xx_PtpPortCreateS *pPortCreate,
                                zl303xx_PtpPortHandleT *pPortHandle);

zlStatusE zl303xx_PtpPortDeleteStructInit(zl303xx_PtpPortDeleteS *pPortDelete);
zlStatusE zl303xx_PtpPortDelete(zl303xx_PtpPortHandleT portHandle,
                                zl303xx_PtpPortDeleteS *pPortDelete);

zlStatusE zl303xx_PtpPortConfigGet(zl303xx_PtpPortHandleT portHandle,
                                   zl303xx_PtpPortCreateS *pPortConfig);
zlStatusE zl303xx_PtpPortExtDataGet(zl303xx_PtpPortHandleT portHandle,
                                    void **ppExtData);
zlStatusE zl303xx_PtpPortExtDataSet(zl303xx_PtpPortHandleT portHandle,
                                    void *pExtData);

zlStatusE zl303xx_PtpPortRateRatioEnabledGet(zl303xx_PtpPortHandleT portHandle,
                                    zl303xx_BooleanE *pRateRatioEnabled);
zlStatusE zl303xx_PtpPortRateRatioEnabledSet(zl303xx_PtpPortHandleT portHandle,
                                    zl303xx_BooleanE rateRatioEnabled);
                                    
zlStatusE zl303xx_PtpPortNumRateRatioPktsGet(zl303xx_PtpPortHandleT portHandle,
                                    zl303xxnumRateRatioPktsE *pNumRateRatioPkts);
zlStatusE zl303xx_PtpPortNumRateRatioPktsSet(zl303xx_PtpPortHandleT portHandle,
                                    zl303xxnumRateRatioPktsE numRateRatioPkts);
zlStatusE zl303xx_PtpNumRateRatioPktsValueCheck(zl303xxnumRateRatioPktsE numRateRatioPkts);

zlStatusE zl303xx_PtpPortPdelayRespHandleMethodSet(
                                    zl303xx_PtpPortHandleT portHandle,
                                    zl303xx_PtpPdelayRespMethodE pdelayRespMethod);
zlStatusE zl303xx_PtpPortPeerMeanPathDelayGet(zl303xx_PtpPortHandleT portHandle,
                                              zl303xx_TimeInterval *peerMeanPathDelay);
zlStatusE zl303xx_PtpPortNeighbourRateRatioGet(zl303xx_PtpPortHandleT portHandle,
                                             zl303xx_RateRatio *pNeighbourRateRatio);

zlStatusE zl303xx_PtpPortLogQueryIntervalSet(zl303xx_PtpPortHandleT portHandle,
                                             Sint8T logQueryInterval);
zlStatusE zl303xx_PtpPortUnicastStackedTlvEnSet(zl303xx_PtpPortHandleT portHandle,
                                                zl303xx_BooleanE stackedTlvEn);
zlStatusE zl303xx_PtpPortForceFaultySet(zl303xx_PtpPortHandleT portHandle);
zlStatusE zl303xx_PtpPortForceFaultyClear(zl303xx_PtpPortHandleT portHandle);
zlStatusE zl303xx_PtpPortGetLastFaultType(zl303xx_PtpPortHandleT portHandle,
                                          zl303xx_PtpPortFaultTypeE *lastFaultType);
zlStatusE zl303xx_PtpPortFaultPassThroughEnSet(zl303xx_PtpPortHandleT portHandle,
                                             zl303xx_BooleanE passThroughEn);
zlStatusE zl303xx_PtpPortFaultCounterGet(zl303xx_PtpPortHandleT portHandle,
                                             Uint32T *pFaultCounter);
zlStatusE  zl303xx_PtpPortGetLastTxError(zl303xx_PtpPortHandleT portHandle,
                                       zl303xx_PtpTxStatusE *lastTxError);

/* Port level PPS info/limits */
zlStatusE zl303xx_PtpPortTotalPpsMaxSet(zl303xx_PtpPortHandleT portHandle,
                                        zl303xx_MessageTypeE msgType,
                                        Uint32T totalPpsMax);
zlStatusE zl303xx_PtpPortTotalPpsGet(zl303xx_PtpPortHandleT portHandle,
                                     zl303xx_PtpTotalPpsS *pTotal);

/* Port level contract info/limits */
zlStatusE zl303xx_PtpPortContractCountGet(zl303xx_PtpPortHandleT portHandle,
                                         zl303xx_MessageTypeE msgType,
                                         Uint32T *count);
zlStatusE zl303xx_PtpPortContractCountMaxGet(zl303xx_PtpPortHandleT portHandle,
                                         zl303xx_MessageTypeE msgType,
                                         Uint32T *limit);
zlStatusE zl303xx_PtpPortContractCountMaxSet(zl303xx_PtpPortHandleT portHandle,
                                         zl303xx_MessageTypeE msgType,
                                         Uint32T limit);
zlStatusE zl303xx_PtpPortContractDurationMaxGet(zl303xx_PtpPortHandleT portHandle,
                                                zl303xx_MessageTypeE msgType,
                                                Uint32T *durationMax);
zlStatusE zl303xx_PtpPortContractDurationMaxSet(zl303xx_PtpPortHandleT portHandle,
                                                zl303xx_MessageTypeE msgType,
                                                Uint32T durationMax);
zlStatusE zl303xx_PtpPortContractIntervalMinGet(zl303xx_PtpPortHandleT portHandle,
                                                zl303xx_MessageTypeE msgType,
                                                Sint8T *intervalMin);
zlStatusE zl303xx_PtpPortContractIntervalMinSet(zl303xx_PtpPortHandleT portHandle,
                                                zl303xx_MessageTypeE msgType,
                                                Sint8T intervalMin);
zlStatusE zl303xx_PtpPortMasterOnlySet(zl303xx_PtpPortHandleT portHandle,
                                       zl303xx_BooleanE bMasterOnly);
zlStatusE zl303xx_PtpPortDenyServiceRequestsSet(zl303xx_PtpPortHandleT portHandle,
                                                zl303xx_BooleanE bDenyServiceRequests);

/* API to manage virtual PTP Ports (this is an external phase/time input interface
 * created to allow electrical sources to participate in the BMCA selection). */
zlStatusE zl303xx_PtpVirtualPortCreateStructInit(zl303xx_PtpVirtualPortConfigS *pVirtualPortCreate);
zlStatusE zl303xx_PtpVirtualPortCreate(zl303xx_PtpVirtualPortConfigS *pVirtualPortCreate,
                                       zl303xx_PtpPortHandleT *portHandle);
zlStatusE zl303xx_PtpVirtualPortDelete(zl303xx_PtpPortHandleT portHandle,
                                       zl303xx_PtpVirtualPortDeleteS *pVirtualPortDelete);
zlStatusE zl303xx_PtpVirtualPortGet(zl303xx_PtpPortHandleT portHandle,
                                    zl303xx_PtpVirtualPortConfigS *pVirtualPortConfig);
zlStatusE zl303xx_PtpVirtualPortPtsfSet(zl303xx_PtpPortHandleT portHandle, zl303xx_BooleanE ptsf);
zlStatusE zl303xx_PtpVirtualPortSet(zl303xx_PtpPortHandleT portHandle,
                                    zl303xx_PtpVirtualPortConfigS *pVirtualPortConfig);

/*** STREAM FUNCTIONS ***/
zlStatusE zl303xx_PtpStreamCreateStructInit(zl303xx_PtpStreamCreateS *pStreamCreate);
zlStatusE zl303xx_PtpProfileStreamStructInit(zl303xx_PtpProfileE profileId,
                                             zl303xx_PtpStreamCreateS *pStreamCreateS);
zlStatusE zl303xx_PtpStreamCreate(zl303xx_PtpStreamCreateS *pStreamCreate,
                                  zl303xx_PtpStreamHandleT *pStreamHandle);

zlStatusE zl303xx_PtpStreamDeleteStructInit(zl303xx_PtpStreamDeleteS *pStreamDelete);
zlStatusE zl303xx_PtpStreamDelete(zl303xx_PtpStreamHandleT streamHandle,
                                  zl303xx_PtpStreamDeleteS *pStreamDelete);

zlStatusE zl303xx_PtpStreamConfigGet(zl303xx_PtpStreamHandleT streamHandle,
                                     zl303xx_PtpStreamCreateS *pStreamConfig);
zlStatusE zl303xx_PtpStreamOverrideModeSet(zl303xx_PtpStreamHandleT streamHandle,
                                           zl303xx_PtpStreamModeE overrideMode);
zlStatusE zl303xx_PtpStreamExtDataGet(zl303xx_PtpStreamHandleT streamHandle, void **ppExtData);
zlStatusE zl303xx_PtpStreamExtDataSet(zl303xx_PtpStreamHandleT streamHandle, void *pExtData);

zlStatusE zl303xx_PtpStreamCurrentDSGet(zl303xx_PtpStreamHandleT streamHandle, zl303xx_CurrentDS *pCurrentDS);
zlStatusE zl303xx_PtpStreamParentDSGet(zl303xx_PtpStreamHandleT streamHandle, zl303xx_ParentDS *pParentDS);

zlStatusE zl303xx_PtpStreamCountersGet(zl303xx_PtpStreamHandleT streamHandle, zl303xx_PtpStreamCountersS *pCounters);
zlStatusE zl303xx_PtpStreamCountersReset(zl303xx_PtpStreamHandleT streamHandle);

zlStatusE zl303xx_PtpStreamLogSyncIntvlSet    (zl303xx_PtpStreamHandleT streamHandle, Sint8T logSyncIntvl);
zlStatusE zl303xx_PtpStreamLogDelayIntvlSet   (zl303xx_PtpStreamHandleT streamHandle, Sint8T logDelayIntvl);
zlStatusE zl303xx_PtpStreamLogPdelayIntvlSet  (zl303xx_PtpStreamHandleT streamHandle, Sint8T logPdelayIntvl);
zlStatusE zl303xx_PtpStreamLogAnnounceIntvlSet(zl303xx_PtpStreamHandleT streamHandle, Sint8T logAnncIntvl);
zlStatusE zl303xx_PtpStreamDelayReqTimeRequiredSet(zl303xx_PtpStreamHandleT streamHandle, zl303xx_BooleanE timeRequired);

zlStatusE zl303xx_PtpStreamAnnounceTimoutSet(zl303xx_PtpStreamHandleT streamHandle, Uint8T announceReceiptTimeout);
zlStatusE zl303xx_PtpStreamUniNegDurationSet(zl303xx_PtpStreamHandleT streamHandle, Uint32T durationSec);
zlStatusE zl303xx_PtpStreamUniNegRenewSet(zl303xx_PtpStreamHandleT streamHandle, Uint32T renewSec);
zlStatusE zl303xx_PtpStreamUniNegMonitorTimingSet(zl303xx_PtpStreamHandleT streamHandle, zl303xx_BooleanE bUniNegMonitorTiming);
zlStatusE zl303xx_PtpStreamUniNegDenialRetrySet(zl303xx_PtpStreamHandleT streamHandle, Uint32T retryCount, Uint32T lockoutSec);
zlStatusE zl303xx_PtpStreamUniNegTimeoutRetrySet(zl303xx_PtpStreamHandleT streamHandle, Uint32T retryCount, Uint32T lockoutSec);
zlStatusE zl303xx_PtpStreamKeepAliveDurationSet(zl303xx_PtpStreamHandleT streamHandle, Uint32T keepAliveSec);
zlStatusE zl303xx_PtpStreamUncalibratedIntervalSet(zl303xx_PtpStreamHandleT streamHandle, Uint8T anncIntervals);
zlStatusE zl303xx_PtpStreamUncalibratedLockRequiredSet(zl303xx_PtpStreamHandleT streamHandle, zl303xx_BooleanE lockRequired);

/* Event Message Length Extension (Zeros Padding). */
zlStatusE zl303xx_PtpStreamEventMsgExtensionSet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_MessageTypeE msgType,
      Uint32T padZerosLen);
/* Event Message PAD TLV. */
zlStatusE zl303xx_PtpStreamEventTlvPadLengthSet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_MessageTypeE msgType,
      Uint32T ttlPadTlvLen);

zlStatusE zl303xx_PtpStreamMaxClockClassSet(zl303xx_PtpStreamHandleT streamHandle, Uint8T max);
zlStatusE zl303xx_PtpStreamIngressClockClassSet(zl303xx_PtpStreamHandleT streamHandle, Uint8T ingressClass);
zlStatusE zl303xx_PtpStreamEgressClockClassSet(zl303xx_PtpStreamHandleT streamHandle, Uint8T egressClass);

zlStatusE zl303xx_PtpStreamEgressOverrideAnncFieldSet(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_PtpStreamOverrideAnncEgressE overrideId,
      void *overrideValue);
zlStatusE zl303xx_PtpStreamEgressOverrideShow(zl303xx_PtpStreamHandleT streamHandle);

zlStatusE zl303xx_PtpStreamEgressAnncDataGet(zl303xx_PtpStreamHandleT streamHandle,
                                             zl303xx_PtpV2MsgAnnounceS *pEgressAnncData);

/* Delay Asymmetry Corrections */
zlStatusE zl303xx_PtpStreamDelayAsymmetrySet(zl303xx_PtpStreamHandleT streamHandle,
                                             zl303xx_TimeInterval *ingressDelay,
                                             zl303xx_TimeInterval *egressDelay);
zlStatusE zl303xx_PtpStreamDelayAsymmetryNsSet(zl303xx_PtpStreamHandleT streamHandle,
                                               zl303xx_BooleanE isIngress,
                                               Sint32T delayNs);

/* Stream Best-Effort Unicast Negotiation */
zlStatusE zl303xx_PtpStreamBestEffortEn(zl303xx_PtpStreamHandleT streamHandle,
                                        zl303xx_MessageTypeE msgType,
                                        zl303xx_BooleanE enable);
zlStatusE zl303xx_PtpStreamBestEffortIntervalSet(zl303xx_PtpStreamHandleT streamHandle,
                                        zl303xx_MessageTypeE msgType,
                                        Sint8T maxInterval);

zlStatusE zl303xx_PtpStreamStateGet(zl303xx_PtpStreamHandleT streamHandle, zl303xx_PtpStreamOperatingStateE *state);
zlStatusE zl303xx_PtpStreamNextTxSequenceIdGet(zl303xx_PtpStreamHandleT streamHandle,
                                               Uint16T seqId[ZL303XX_PTP_V2_NUM_MSG_IDS]);

zlStatusE zl303xx_PtpTsLogConfigStructInit(zl303xx_PtpTsLogConfigS *pConfig);
zlStatusE zl303xx_PtpTsLogConfigSet(zl303xx_PtpStreamHandleT streamHandle, zl303xx_PtpTsLogConfigS *pConfig);

zlStatusE zl303xx_PtpContractStructInit(zl303xx_PtpContractS *pContract);
zlStatusE zl303xx_PtpContractRequest(zl303xx_PtpStreamHandleT streamHandle,
                                     zl303xx_MessageTypeE msgType,
                                     zl303xx_PtpContractS *pContract);
zlStatusE zl303xx_PtpContractCancel(zl303xx_PtpStreamHandleT streamHandle,
                                    zl303xx_MessageTypeE msgType);
zlStatusE zl303xx_PtpContractManualCancel(zl303xx_PtpPortHandleT portHandle,
                                          zl303xx_MessageTypeE msgType,
                                          zl303xx_PortAddress *pDestAddr);
zlStatusE zl303xx_PtpTxContractStop(zl303xx_PtpStreamHandleT streamHandle,
                                    zl303xx_MessageTypeE msgType);
zlStatusE zl303xx_PtpTxContractCancel(zl303xx_PtpStreamHandleT streamHandle,
                                      zl303xx_MessageTypeE msgType);
zlStatusE zl303xx_PtpTxContractAllow(zl303xx_PtpStreamHandleT streamHandle,
                                     zl303xx_MessageTypeE msgType);
zlStatusE zl303xx_PtpContractStatusGet(zl303xx_PtpStreamHandleT streamHandle,
                                       zl303xx_MessageTypeE msgType,
                                       zl303xx_PtpContractFlowE flow,
                                       zl303xx_PtpContractStatusS *pStatus);
zlStatusE zl303xx_PtpTxContractStatusSet(zl303xx_PtpPortHandleT portHandle,
                                         zl303xx_MessageTypeE msgType,
                                         zl303xx_PtpContractStatusS *pStatus,
                                         zl303xx_PtpStreamHandleT *pStreamHandle);

/* Send a basic Management TLV with just the tlvType, length and managementId fields. */
zlStatusE zl303xx_PtpV2MgmtTlvIssue(
      zl303xx_PtpStreamHandleT streamHandle,
      zl303xx_PortIdentity *targetPortIdentity,
      zl303xx_PtpV2MgmtActionE actionField,
      Uint16T managementId);

/*** PROTOCOL DATA FUNCTIONS ***/
zlStatusE zl303xx_PtpRxMsgProcess(Uint8T *buffer, Uint16T bufferLen,
                                  zl303xx_PtpRxMsgDataS *pMsgData);
zlStatusE zl303xx_PtpPortRxMsgProcess(Uint8T *buffer, Uint16T bufferLen,
                                      zl303xx_PtpPortRxMsgDataS *pMsgData);
zlStatusE zl303xx_PtpStreamRxMsgProcess(Uint8T *buffer, Uint16T bufferLen,
                                        zl303xx_PtpStreamRxMsgDataS *pMsgData);
zl303xx_BooleanE zl303xx_PtpRxMsgIsEvent(Uint8T *buffer, Uint16T bufferLen);
zl303xx_BooleanE zl303xx_PtpRxMsgIsUnicast(Uint8T *buffer, Uint16T bufferLen);
zlStatusE zl303xx_PtpTsRecordProcess(zl303xx_PtpTsRecordS *pTsRecord);

/*** ACCEPTABLE MASTER FUNCTIONS ***/
zlStatusE zl303xx_PtpAcceptableMasterTableEnSet(Uint32T portHandle,
                                                zl303xx_BooleanE enabled);

zlStatusE zl303xx_PtpAcceptableMasterStructInit(zl303xx_AcceptableMaster *pAcceptableMaster);
zlStatusE zl303xx_PtpAcceptableMasterAdd(zl303xx_PtpPortHandleT portHandle,
                                         zl303xx_AcceptableMaster *pAcceptableMaster);
zlStatusE zl303xx_PtpAcceptableMasterRemove(zl303xx_PtpPortHandleT portHandle,
                                            zl303xx_PortAddress *pPortAddress);
zlStatusE zl303xx_PtpAcceptableMasterSetAltPriority(zl303xx_PtpPortHandleT portHandle,
                                       zl303xx_AcceptableMaster *pAcceptableMaster);
zlStatusE zl303xx_PtpAcceptableMasterDroppedCountsGet(zl303xx_PtpPortHandleT portHandle,
                                                      Uint32T *msgCountArray,
                                                      zl303xx_BooleanE bClearCounts);
zlStatusE zl303xx_PtpAcceptableMasterDroppedCountsClear(zl303xx_PtpPortHandleT portHandle);

/*** ACCEPTABLE SLAVE FUNCTIONS ***/
zlStatusE zl303xx_PtpAcceptableSlaveTableEnSet(Uint32T portHandle,
                                               zl303xx_BooleanE enabled);

zlStatusE zl303xx_PtpAcceptableSlaveStructInit(zl303xx_AcceptableSlave *pAcceptableSlave);
zlStatusE zl303xx_PtpAcceptableSlaveAdd(zl303xx_PtpPortHandleT portHandle,
                                        zl303xx_AcceptableSlave *pAcceptableSlave);
zlStatusE zl303xx_PtpAcceptableSlaveRemove(zl303xx_PtpPortHandleT portHandle,
                                           zl303xx_PortAddress *pPortAddress);

/*** GRANT TABLE FUNCTIONS ***/
zlStatusE zl303xx_PtpGrantConfigStructInit(zl303xx_PtpGrantConfigS *pConfig);
zlStatusE zl303xx_PtpGrantConfigSet(zl303xx_PtpPortHandleT portHandle,
                                    zl303xx_PtpGrantConfigS *pConfig);
zlStatusE zl303xx_PtpGrantConfigGet(zl303xx_PtpPortHandleT portHandle,
                                    zl303xx_PortAddress *pAddress,
                                    zl303xx_PtpGrantConfigS *pConfig);
zlStatusE zl303xx_PtpGrantConfigDelete(zl303xx_PtpPortHandleT portHandle,
                                       zl303xx_PortAddress *pAddress);
zlStatusE zl303xx_PtpGrantConfigDeleteAll(zl303xx_PtpPortHandleT portHandle);

/* DEPRECATED: in favor of zl303xx_PtpPortDenyServiceRequests(). */
zlStatusE zl303xx_PtpGrantDefaultSet(zl303xx_PtpPortHandleT portHandle,
                                     zl303xx_BooleanE grantDefault);

/*** UNICAST MASTER TABLE FUNCTIONS ***/
zlStatusE zl303xx_PtpUmtActSizeGet(zl303xx_PtpPortHandleT portHandle, Uint16T *size);
zlStatusE zl303xx_PtpUmtShow(zl303xx_PtpPortHandleT portHandle);
zlStatusE zl303xx_PtpUmtEntryStructInit(zl303xx_PtpUmtEntryS *pEntry);
zlStatusE zl303xx_PtpUmtEntryAdd(zl303xx_PtpUmtEntryS *pEntry,
                                 zl303xx_PtpStreamHandleT *pStreamHandle);
zlStatusE zl303xx_PtpUmtEntryRemove(zl303xx_PtpPortHandleT portHandle,
                                    zl303xx_PortAddress *pPortAddr);
zlStatusE zl303xx_PtpUmtEntryRemoveAll(zl303xx_PtpPortHandleT portHandle);
zlStatusE zl303xx_PtpUmtEntryExists(zl303xx_PtpUmtEntryS *pEntry,
                                    zl303xx_PtpPortHandleT *pPortHandle,
                                    zl303xx_PtpStreamHandleT *pStreamHandle);
zlStatusE zl303xx_PtpUmtEntryReqAnncSet(zl303xx_PtpStreamHandleT streamHandle,
                                        zl303xx_BooleanE reqAnnc);
zlStatusE zl303xx_PtpUmtEntryReqTimingSet(zl303xx_PtpStreamHandleT streamHandle,
                                          zl303xx_BooleanE reqTiming);
zlStatusE zl303xx_PtpUmtEntryMaxIntvlSet(zl303xx_PtpStreamHandleT streamHandle,
                                         zl303xx_MessageTypeE msgType,
                                         Sint8T intvl);
zlStatusE zl303xx_PtpUmtEntrySyncTimeoutSet(zl303xx_PtpStreamHandleT streamHandle,
                                            Uint8T syncTimeoutSec);
zlStatusE zl303xx_PtpUmtEntryFollowUpTimeoutSet(zl303xx_PtpStreamHandleT streamHandle,
                                                Uint8T followUpTimeoutSec);
zlStatusE zl303xx_PtpUmtEntrySyncOrFollowUpTimeoutSet(zl303xx_PtpStreamHandleT streamHandle,
                                                      Uint8T timeoutSec);
zlStatusE zl303xx_PtpUmtEntryDelayTimeoutSet(zl303xx_PtpStreamHandleT streamHandle,
                                             Uint8T delayTimeoutSec);
zlStatusE zl303xx_PtpUmtEntryPdelayTimeoutSet(zl303xx_PtpStreamHandleT streamHandle,
                                             Uint8T pdelayTimeoutSec);
zlStatusE zl303xx_PtpUmtEntryPdelayFollowTimeoutSet(zl303xx_PtpStreamHandleT streamHandle,
                                              Uint8T pdelayFollowTimeoutSec);

zlStatusE zl303xx_PtpUmtEntryBmcaAllowSet(zl303xx_PtpStreamHandleT streamHandle,
                                          zl303xx_BooleanE bmcaAllow);
zlStatusE zl303xx_PtpUmtEntryPrioritySet(zl303xx_PtpStreamHandleT streamHandle,
                                         Uint8T priority);
zlStatusE zl303xx_PtpUmtEntryRetryReset(zl303xx_PtpStreamHandleT streamHandle);
zlStatusE zl303xx_PtpUmtEntryWtrSet(zl303xx_PtpStreamHandleT streamHandle, Uint32T sec);
zlStatusE zl303xx_PtpUmtEntryStatusGet(zl303xx_PtpStreamHandleT streamHandle,
                                       zl303xx_PtpUmtEntryStatusS *pStatus);

/*** UTILITY FUNCTIONS ***/
zlStatusE zl303xx_PtpConvertIPv4ToPortAddress(const char *ipAddr,
                                              zl303xx_PortAddress *pPortAddress);
zlStatusE zl303xx_PtpConvertPortAddressToIPv4(zl303xx_PortAddress *pPortAddress,
                                              char *ipAddr);
zlStatusE zl303xx_PtpConvertEth2ToPortAddress(
      const char *eth2Address,
      zl303xx_PortAddress *pPortAddress);
zlStatusE zl303xx_PtpConvertPortAddressToStr(zl303xx_PortAddress *pAddr, char *str);
zlStatusE zl303xx_PtpStreamHandleGetFromDestAddr(zl303xx_PortAddress *pDestAddr,
                                                 zl303xx_PtpStreamHandleT *pStreamHandle);
zlStatusE zl303xx_PtpStreamClockHandleGet(zl303xx_PtpStreamHandleT streamHandle, zl303xx_PtpClockHandleT *clockHandle);

Uint16T zl303xx_PtpPortAddressLengthGet(zl303xx_NetworkProtocolE protocol);
const char *zl303xx_PtpPortStateToStr(zl303xx_PortStateE state);
const char *zl303xx_PtpMessageTypeToStr(zl303xx_MessageTypeE msgType);
const char *zl303xx_PtpG781QLToStr(zl303xx_PtpG781QualityLevelE ql);
zlStatusE zl303xx_PtpClockCurrMasterGet(zl303xx_PtpClockHandleT clockHandle,
                                        zl303xx_PtpStreamHandleT *pStreamHandle);

/* Utility Routines to convert between TimeStamp & TimeInterval types. */
zlStatusE zl303xx_PtpConvTimeStampToTimeInterval(
      zl303xx_TimeStamp *pInTimeStamp,
      zl303xx_TimeInterval *pOutTimeInterval);
zlStatusE zl303xx_PtpConvTimeIntervalToTimeStamp(
      zl303xx_TimeInterval *pInTimeInterval,
      zl303xx_TimeStamp *pOutTimeStamp);

/* Utility Routines to do simple math on TimeInterval types.
 * (Used for correctionField updating and delay asymmetry calculations. */
zl303xx_TimeInterval zl303xx_PtpTimeIntervalAdd (zl303xx_TimeInterval tIntvlA, zl303xx_TimeInterval tIntvlB);
zl303xx_TimeInterval zl303xx_PtpTimeIntervalDiff(zl303xx_TimeInterval tIntvlA, zl303xx_TimeInterval tIntvlB);
zl303xx_TimeInterval zl303xx_PtpTimeIntervalNegate(zl303xx_TimeInterval timeIntvl);
zl303xx_TimeInterval zl303xx_PtpTimeIntervalRShift(zl303xx_TimeInterval timeIntvl, Uint8T rRhift);
Sint8T zl303xx_PtpTimeIntervalSignage(zl303xx_TimeInterval timeIntvl);

/* zl303xx_PtpStrings.h */
/* Formats a Clock or Port Identity To a String of appropriate format.*/
void zl303xx_PtpV2ClockIdentityToString(zl303xx_ClockIdentity clockIdentity, char *clkIdStr);
void zl303xx_PtpV2PortIdentityToString(zl303xx_PortIdentity *portIdentity, char *portIdStr);
/* Formats a Timestamp To a String of appropriate format.*/
void zl303xx_PtpV2TimestampToString(zl303xx_TimeStamp *timeStamp, char *tsStr);


/*** NODE DESCRIPTION FUNCTIONS ***/
zlStatusE zl303xx_PtpNodeManufacturerIdentitySet(Uint8T *pManufacturerId);
zlStatusE zl303xx_PtpNodeManufacturerIdentityGet(Uint8T *pManufacturerId);
zlStatusE zl303xx_PtpNodeProductDescriptionStructInit(zl303xx_PtpNodeProductDescriptionS *pDesc);
zlStatusE zl303xx_PtpNodeProductDescriptionSet(zl303xx_PtpNodeProductDescriptionS *pDesc);
zlStatusE zl303xx_PtpNodeProductDescriptionGet(char *productDescription);
zlStatusE zl303xx_PtpNodeRevisionDataStructInit(zl303xx_PtpNodeRevisionDataS *pRev);
zlStatusE zl303xx_PtpNodeRevisionDataSet(zl303xx_PtpNodeRevisionDataS *pRev);
zlStatusE zl303xx_PtpNodeRevisionDataGet(char *revisionData);
zlStatusE zl303xx_PtpNodeUserDescriptionStructInit(zl303xx_PtpNodeUserDescriptionS *pUser);
zlStatusE zl303xx_PtpNodeUserDescriptionSet(zl303xx_PtpNodeUserDescriptionS *pUser);
zlStatusE zl303xx_PtpNodeUserDescriptionGet(char *userDescription);

#ifdef __cplusplus
}
#endif

#endif
