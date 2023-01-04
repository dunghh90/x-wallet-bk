

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

#ifndef _ZL303XX_PTP_API_INTERNAL_H_
#define _ZL303XX_PTP_API_INTERNAL_H_

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_Trace.h"
#include "zl303xx_List.h"

#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpDatatypes.h"
#include "zl303xx_PtpSetup.h"
#include "zl303xx_PtpInternalTypes.h"
#include "zl303xx_PtpEventHandling.h"
#include "zl303xx_PtpTimer.h"
#include "zl303xx_PtpV2SignalTlvParse.h"

/*****************   DEFINES   ************************************************/
#define TEN_e6 1000000
#define TEN_e9 1000000000

/* For any Invalid Indication (Handles, indexes, etc.) */
#ifndef ZL303XX_PTP_INVALID
#define ZL303XX_PTP_INVALID -1
#endif

/* Value to indicate that a Foreign Master Table index is invalid. */
#ifndef ZL303XX_PTP_FMT_INDEX_INVALID
#define ZL303XX_PTP_FMT_INDEX_INVALID (Uint16T)(-1)
#endif

#ifndef ZL303XX_CHECK_POINTER
#define ZL303XX_CHECK_POINTER(p) \
           ((p == NULL) \
               ? (ZL303XX_TRACE_ERROR("Invalid pointer: "#p, 0,0,0,0,0,0), \
                     ZL303XX_PARAMETER_INVALID) \
               : (ZL303XX_OK))
#endif

/* MACROS to check the Clock, Port & Stream Ranges. */
#define ZL303XX_CHECK_CLOCK_HANDLE(handle) \
   ((handle >= ZL303XX_PTP_NUM_CLOCKS_MAX) ? \
      (ZL303XX_PARAMETER_INVALID) : (ZL303XX_OK))

#define ZL303XX_CHECK_PORT_HANDLE(handle) \
   ((handle >= ZL303XX_PTP_NUM_PORTS_MAX) ? \
      (ZL303XX_PARAMETER_INVALID) : (ZL303XX_OK))

#define ZL303XX_CHECK_STREAM_HANDLE(handle) \
   ((handle >= ZL303XX_PTP_NUM_STREAMS_MAX) ? \
      (ZL303XX_PARAMETER_INVALID) : (ZL303XX_OK))

#define ZL303XX_STREAM_MAX_CLOCK_CLASS_OK(stream, clockClass)      \
            ((((stream)->config.maxClockClass == 0) ||           \
             ((clockClass) <= (stream)->config.maxClockClass)) ? \
            (ZL303XX_TRUE) : (ZL303XX_FALSE))

/* Linked list FOR_EACH wrappers. */
#define ZL303XX_PTP_FOR_EACH_CLOCK(clockList, currClock) \
           ZL303XX_LIST_FOR_EACH_TYPE(clockList, currClock, zl303xx_PtpClockS, listEntry)
#define ZL303XX_PTP_FOR_EACH_CLOCK_SAFE(clockList, currClock, temp) \
           ZL303XX_LIST_FOR_EACH_TYPE_SAFE(clockList, currClock, temp, zl303xx_PtpClockS, listEntry)

#define ZL303XX_PTP_FOR_EACH_PORT(portList, currPort) \
           ZL303XX_LIST_FOR_EACH_TYPE(portList, currPort, zl303xx_PortDataS, listEntry)
#define ZL303XX_PTP_FOR_EACH_PORT_SAFE(portList, currPort, temp) \
           ZL303XX_LIST_FOR_EACH_TYPE_SAFE(portList, currPort, temp, zl303xx_PortDataS, listEntry)

#define ZL303XX_PTP_FOR_EACH_STREAM(streamList, currStream) \
           ZL303XX_LIST_FOR_EACH_TYPE(streamList, currStream, zl303xx_ClockStreamS, listEntry)
#define ZL303XX_PTP_FOR_EACH_STREAM_SAFE(streamList, currStream, temp) \
           ZL303XX_LIST_FOR_EACH_TYPE_SAFE(streamList, currStream, temp, zl303xx_ClockStreamS, listEntry)

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   INTERNAL PTP FUNCTION DECLARATIONS   *********************/

/* Internal PTP node functions. */
zl303xx_ListS *zl303xx_PtpNodeClockListGet(void);
zlStatusE zl303xx_PtpNodeMutexLock(void);
zlStatusE zl303xx_PtpNodeMutexUnlock(zlStatusE status);
zlStatusE zl303xx_PtpNodeAttachClock(zl303xx_PtpClockS *pPtpClock);
zlStatusE zl303xx_PtpNodeDetachClock(zl303xx_PtpClockS *pPtpClock);
zlStatusE zl303xx_PtpNodeLastClockGet(zl303xx_PtpClockS **ppPtpClock);
zl303xx_BooleanE zl303xx_PtpNodeLastClockExists(void);
zlStatusE zl303xx_PtpNodeEnable(zl303xx_PtpNodeEnableS *nodeEnable);
zlStatusE zl303xx_PtpNodeStringsInit(void);
void zl303xx_PtpNodeDescriptionSet(Uint8T *mId,  char *desc,  char *rev,  char *user);
void zl303xx_PtpNodeDescriptionGet(Uint8T **mId, char **desc, char **rev, char **user);
zl303xx_PtpClockS *zl303xx_PtpNodeClockIdentityExists(zl303xx_ClockIdentity clockId);

/* Task message functions */
zlStatusE zl303xx_PtpSendTimerTick(zl303xx_PtpClockS *pClock, zl303xx_BooleanE secondFlag);
zlStatusE zl303xx_PtpSendManualCancel(zl303xx_PortDataS *pPort, zl303xx_ClockStreamS *pStream,
                                      zl303xx_MessageTypeE msgType, zl303xx_PortAddress *pDest);

/* Internal PTP clock functions. */
zlStatusE zl303xx_PtpClockDeleteInternal(zl303xx_PtpClockS *pPtpClock, zl303xx_PtpClockDeleteS *pClockDelete);
zlStatusE zl303xx_PtpClockStructAllocate(zl303xx_PtpClockCreateS *pClockCreate, zl303xx_PtpClockS **ppPtpClock);
zlStatusE zl303xx_PtpClockStructFree(zl303xx_PtpClockS **ppPtpClock);
zlStatusE zl303xx_PtpClockMutexCreate(zl303xx_PtpClockS *pPtpClock);
zlStatusE zl303xx_PtpClockMutexDelete(zl303xx_PtpClockS *pPtpClock);
zlStatusE zl303xx_PtpClockMutexLock(zl303xx_PtpClockS *pPtpClock);
zlStatusE zl303xx_PtpClockMutexUnlock(zl303xx_PtpClockS *pPtpClock, zlStatusE status);
zlStatusE zl303xx_PtpClockMsgQCreate(zl303xx_PtpClockS *pPtpClock);
zlStatusE zl303xx_PtpClockMsgQDelete(zl303xx_PtpClockS *pPtpClock);
zlStatusE zl303xx_PtpClockMsgQSend(zl303xx_PtpClockS *pPtpClock, void *msg, Uint32T msgSize);
zlStatusE zl303xx_PtpClockMsgQRecv(zl303xx_PtpClockS *pPtpClock, zl303xx_PtpTaskMsgU *msg);
zlStatusE zl303xx_PtpClockTaskStart(zl303xx_PtpClockS *pPtpClock);
zlStatusE zl303xx_PtpClockTaskStop(zl303xx_PtpClockS *pPtpClock);
void zl303xx_PtpClockParamSet(zl303xx_PtpClockS *pClock, zl303xx_PtpClockParamE param, const void *data);
Uint32T zl303xx_PtpClockParamPack(zl303xx_PtpClockParamE param, const void *data, Uint8T *buffer);

zlStatusE zl303xx_PtpClockConfigureDefaultDataSet(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_DefaultDS *pDefaultDS,
      zl303xx_BooleanE lockMutex);
zlStatusE zl303xx_PtpClockConfigureParentDataSet(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_ParentDS *pParentDS,
      zl303xx_BooleanE lockMutex);
zlStatusE zl303xx_PtpClockCheckParentDS(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_ParentDS *pParentDS);
zlStatusE zl303xx_PtpClockCompareDataGet(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpBmcCompareDataS *pCompareData);

zlStatusE zl303xx_PtpClockAttachPort(zl303xx_PtpClockS *pPtpClock, zl303xx_PortDataS *pPort);
zlStatusE zl303xx_PtpClockDetachPort(zl303xx_PtpClockS *pPtpClock, zl303xx_PortDataS *pPort);

/* Start/Stop the Timers during Clock creation or via reconfig API. */
void zl303xx_PtpClockTimersInit(zl303xx_PtpClockS *pPtpClock);

/* Command to Force the Clock to upload its Foreign Master Table. */
zlStatusE zl303xx_PtpClockBmcaUpdateInitiate(zl303xx_PtpClockS *pPtpClock);

/* Log why a port went into faulty state */
void zl303xx_PtpPortLogFaultyState(zl303xx_PtpTxStatusE txStatus,
                                 zl303xx_PtpPortFaultTypeE faultType, zl303xx_PortDataS *portData);

zl303xx_BooleanE zl303xx_PtpClockHasSlavePorts(zl303xx_PtpClockS *pPtpClock);
zl303xx_BooleanE zl303xx_PtpClockHasStreamInState(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_PtpStreamSubStateE streamState);
zl303xx_BooleanE zl303xx_PtpClockIsOwnMaster(zl303xx_PtpClockS *pPtpClock);
zl303xx_BooleanE zl303xx_PtpClockHasSelectedVirtualPort(zl303xx_PtpClockS *pPtpClock, zl303xx_PortDataS **pPtpPort);

void zl303xx_PtpClockTypeEvaluate(zl303xx_PtpClockS *pClock);
void zl303xx_PtpClockNumberPortsEvaluate(zl303xx_PtpClockS *pClock);
void zl303xx_PtpClockUncalibratedClassEvaluate(
      zl303xx_PtpClockS *pPtpClock,
      Uint8T *exportedClass);
zlStatusE zl303xx_PtpClockSynchronizationUncertainEvaluate(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_BooleanE *synchroUncertain);

zlStatusE zl303xx_PtpClockUtcGetInternal(zl303xx_PtpClockS *pClock, zl303xx_TimeStamp *pUtc, zl303xx_BooleanE  *pUtcValid);
zlStatusE zl303xx_PtpClockPathTraceListUpdate(zl303xx_PtpPathTraceListS *pPtStruct, Uint16T newCount, zl303xx_ClockIdentity *pNewPtList);
zlStatusE zl303xx_PtpClockPathTraceListShow(zl303xx_PtpPathTraceListS *pPtStruct);

Sint32T zl303xx_PtpTask(void);

/* General Routines for converting a PTP structure from one type to another. */
void zl303xx_PtpConvertDefaultDsToParentDs(zl303xx_DefaultDS *pDefaultDS, zl303xx_ParentDS *pParentDS);

/* Internal PTP port functions. */
zlStatusE zl303xx_PtpPortDeleteInternal(zl303xx_PortDataS *pPort, zl303xx_PtpPortDeleteS *pPortDelete);
zlStatusE zl303xx_PtpPortDeleteAllStreams(zl303xx_PortDataS *pPort);
zlStatusE zl303xx_PtpPortStructAllocate(zl303xx_PtpPortCreateS *pPortCreate, zl303xx_PortDataS **ppPort);
zlStatusE zl303xx_PtpPortStructFree(zl303xx_PortDataS **ppPort);
zlStatusE zl303xx_PtpPortAttachStream(zl303xx_PortDataS *pPort, zl303xx_ClockStreamS *stream);
zlStatusE zl303xx_PtpPortDetachStream(zl303xx_PortDataS *pPort, zl303xx_ClockStreamS *stream);

zl303xx_BooleanE zl303xx_PtpGrantTblCheck(zl303xx_PortDataS *pPort, zl303xx_PortAddress *pAddr,
                                          zl303xx_MessageTypeE msgType);
void zl303xx_PtpGrantEntryUpdate(zl303xx_PortDataS *pPort, zl303xx_PortAddress *addr,
                                 zl303xx_MessageTypeE msgType, zl303xx_BooleanE val);

/* Internal PTP stream functions. */
zlStatusE zl303xx_PtpStreamCreateStructCheck(zl303xx_PtpStreamCreateS *streamCreate,
                                             zl303xx_PortDataS *pPort);
zlStatusE zl303xx_PtpStreamStructAllocate(zl303xx_PtpStreamCreateS *pStreamCreate, zl303xx_ClockStreamS **ppStream);
zlStatusE zl303xx_PtpStreamStructFree(zl303xx_ClockStreamS **ppStream);
zlStatusE zl303xx_PtpStreamCreateInternal(zl303xx_PtpStreamCreateS *streamCreate, zl303xx_PortDataS *port,
                                          Uint8T *rxExtData, zl303xx_ClockStreamS **ppStream);
zlStatusE zl303xx_PtpStreamDeleteInternal(zl303xx_ClockStreamS *stream, zl303xx_PtpStreamDeleteS *pStreamDelete);

zl303xx_ClockStreamS *zl303xx_PtpStreamExistsOnPort(zl303xx_PortDataS *pPort, zl303xx_PortAddress *srcAddr);
zl303xx_ClockStreamS *zl303xx_PtpStreamExists(zl303xx_PortDataS *pPort, zl303xx_PortAddress *pAddr);
void      zl303xx_PtpStreamCountersInit(zl303xx_ClockStreamS *pStream);
zlStatusE zl303xx_PtpStreamAlarmsCheck(zl303xx_ClockStreamS *pStream);
void zl303xx_PtpStreamLostMsgCheck(zl303xx_ClockStreamS *stream, zl303xx_PtpV2MsgS *pPtpMsgRx);
void      zl303xx_PtpStreamPtsfCheck(zl303xx_ClockStreamS *pStream);
zlStatusE zl303xx_PtpStreamTimerStart(zl303xx_ClockStreamS *stream, zl303xx_TimerIdE timerId);
void zl303xx_PtpStreamConfigIntvlSet(zl303xx_ClockStreamS *stream,
                                     zl303xx_MessageTypeE msgType, Sint8T intvl);

void zl303xx_PtpUmtEntryServicesCheck(zl303xx_PortDataS *pPort,
                                      zl303xx_PtpUmtEntryIntS *pEntry);
zlStatusE zl303xx_PtpUmtEntryIntRemove(zl303xx_PortDataS *pPort, zl303xx_PtpUmtEntryIntS *pEntryInt);

void zl303xx_PtpContractRxStateSet(zl303xx_ClockStreamS *stream,
                                   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType,
                                   zl303xx_PtpContractRxStateE state);
void zl303xx_PtpContractTxStateSet(zl303xx_ClockStreamS *stream,
                                   zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType,
                                   zl303xx_PtpContractTxStateE state);

void zl303xx_PtpStreamIntvlDetect(zl303xx_ClockStreamS *pStream, zl303xx_MessageTypeE type, Sint8T intvl);
void zl303xx_PtpStreamPtsfReset(zl303xx_ClockStreamS *pStream);
void zl303xx_PtpStreamTsLogSend(zl303xx_ClockStreamS *streamData,
                                zl303xx_PtpEventServoDataS *servoData);

void zl303xx_PtpNextTxSequenceIdCheck(Uint16T seqId[ZL303XX_PTP_V2_NUM_MSG_IDS]);

void zl303xx_PtpV2SetRequiredTimers(zl303xx_ClockStreamS *streamData);

void zl303xx_PtpTelecomSofTimerReset(zl303xx_ClockStreamS *stream);

zl303xx_ClockStreamS *zl303xx_PtpPortFindStreamForParentDS(
      zl303xx_PortDataS *pPtpPort,
      zl303xx_ParentDS *pParentDS);

/* Stream State Change routines. Called from the Protocol Layer or by the
 * Clock State Machine. */
void zl303xx_PtpStreamStateToS0(zl303xx_ClockStreamS *pPtpStream, zl303xx_ClockStreamS *streamBest);
void zl303xx_PtpStreamStateToM1(zl303xx_ClockStreamS *pPtpStream, zl303xx_ClockStreamS *streamBest);
void zl303xx_PtpStreamStateToM2(zl303xx_ClockStreamS *pPtpStream);
void zl303xx_PtpStreamStateToM3(zl303xx_ClockStreamS *pPtpStream, zl303xx_ClockStreamS *streamBest);

const char *zl303xx_PtpStreamSubStateToStr(zl303xx_PtpStreamSubStateE state);

/* Internal Timestamp Record functions. */
void zl303xx_PtpTsRecordReserve(
            zl303xx_ClockStreamS *stream,
            zl303xx_MessageTypeE msgType,
            Uint16T seqId);
void zl303xx_PtpTsRecordRegisterEgress(
            zl303xx_ClockStreamS *stream,
            zl303xx_MessageTypeE msgType,
            Uint16T seqId,
            zl303xx_TimeStamp *txTs);
void zl303xx_PtpTsRecordRegisterIngress(
            zl303xx_ClockStreamS *stream,
            zl303xx_PortIdentity *srcPortId,
            zl303xx_MessageTypeE msgType,
            Uint16T seqId,
            zl303xx_TimeStamp *rxTs,
            Uint64S *correction);
void zl303xx_PtpTsRecordRegisterPdelayResp(
            zl303xx_ClockStreamS *stream,
            zl303xx_PtpV2MsgS *pPtpMsgRx,
            zl303xx_TimeStamp *rxTs);
void zl303xx_PtpTsRecordRegisterPdelayFollowup(
            zl303xx_ClockStreamS *stream,
            Uint16T seqId,
            Uint64S *correction,
            zl303xx_TimeStamp *pktTsField);
void zl303xx_PtpStreamTwoStepRegisterTx(
            zl303xx_ClockStreamS *stream,
            Uint16T seqId,
            zl303xx_TimeStamp *txTs,
            Uint64S *correction,
            zl303xx_PortIdentity *srcPortId);
void zl303xx_PtpStreamTwoStepRegisterRx(
            zl303xx_ClockStreamS *stream,
            Uint16T seqId,
            zl303xx_TimeStamp *rxTs,
            Uint64S *correction,
            zl303xx_PortIdentity *srcPortId);


/* Internal BMC functions */
/* Function to determine if the UMT entry associated with an FMT entry
 * is qualified. This is used as an input to determine if the FMT entry
 * can be qualified. */
zl303xx_BooleanE zl303xx_PtpV2BmcUmtQualified(zl303xx_ClockStreamS *pStream);
zl303xx_BooleanE zl303xx_PtpTelecomUmtEntryQualified(zl303xx_ClockStreamS *pStream);


/* Utility functions. */
Uint16T zl303xx_PtpComparePortAddress(const zl303xx_PortAddress *a, const zl303xx_PortAddress *b);
zlStatusE zl303xx_PtpPortAddressCheck(zl303xx_PortAddress *pa);

zlStatusE zl303xx_PtpV2ClockIdentityCheck(zl303xx_ClockIdentity clockIdentity);
zlStatusE zl303xx_PtpV2PortIdentityCheck(zl303xx_PortIdentity *portIdentity);

void zl303xx_PtpConvertCurrentDSInternal(zl303xx_CurrentDSInternal *cds_i, zl303xx_CurrentDS *cds);
Uint32T zl303xx_PtpMsgCountExpected(Sint8T log2Intvl, Uint16T periodSec);

/* Utility Routines for evaluating Clock/Port Identities. */
zl303xx_BooleanE zl303xx_PtpV2EqualClockIdentities(zl303xx_ClockIdentity first, zl303xx_ClockIdentity second);
zl303xx_BooleanE zl303xx_PtpV2IsAllClockIdentity(zl303xx_ClockIdentity pClockIdentity);
zl303xx_BooleanE zl303xx_PtpV2EqualPortIdentities(zl303xx_PortIdentity *first, zl303xx_PortIdentity *second);

/* Convert the Unicast message type value to a local index type to make array access smoother. */
zlStatusE zl303xx_PtpV2MessageTypeToIndex(zl303xx_MessageTypeE messageType,
                                                 zl303xx_V2UnicastNegotiatedMsgTypeE *uniNegMsgType);
zlStatusE zl303xx_PtpV2IndexToMessageType(zl303xx_V2UnicastNegotiatedMsgTypeE uniNegMsgType,
                                          zl303xx_MessageTypeE *messageType);

Uint32T zl303xx_PtpV2GetMsgTypeClockTotalPps(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_ClockStreamS *pPtpStreamIngore,
      zl303xx_V2UnicastNegotiatedMsgTypeE uniMsgType);
Uint32T zl303xx_PtpV2GetMsgTypePortTotalPps(
      zl303xx_PortDataS *pPtpPort,
      zl303xx_ClockStreamS *pPtpStreamIngore,
      zl303xx_V2UnicastNegotiatedMsgTypeE uniMsgType);

Uint32T zl303xx_PtpV2GetPortContractCount(zl303xx_PortDataS *pPort,zl303xx_ClockStreamS *pStream,
                                        zl303xx_V2UnicastNegotiatedMsgTypeE msgType);
Uint32T zl303xx_PtpV2GetClockContractCount(zl303xx_PtpClockS *pClock,zl303xx_ClockStreamS *pStream,
                                         zl303xx_V2UnicastNegotiatedMsgTypeE msgType);

zlStatusE zl303xx_PtpUniNegStreamCreate(zl303xx_PortDataS *pPort,
                                        zl303xx_PortAddress *pDestAddr,
                                        zl303xx_MessageTypeE msgType,
                                        Sint8T intvl, void *extData,
                                        zl303xx_ClockStreamS **ppStream);
zlStatusE zl303xx_PtpUniNegRequestValid(zl303xx_PortDataS *pPort,
                                        zl303xx_MessageTypeE msgType,
                                        Uint32T duration,
                                        zl303xx_BooleanE *pMalformed);

/* Intentionally made to resemble other TRACE calls */
void ZL303XX_TRACE_V2_CLOCK_ID (Uint32T modId, Uint8T level, const char *label, zl303xx_ClockIdentity clockIdentity);
void ZL303XX_TRACE_V2_PORT_ID  (Uint32T modId, Uint8T level, const char *label, zl303xx_PortIdentity *portIdentity);
void ZL303XX_TRACE_V2_TIMESTAMP(Uint32T modId, Uint8T level, Uint32T streamId, const char *label, zl303xx_TimeStamp *timestamp);

/* Extra Trace routines. */
void zl303xx_PtpTracePortAddress(zl303xx_PortAddress *portAddr);
void zl303xx_PtpTraceParentDataSet(zl303xx_ParentDS *pParentDS);

/* Wrappers for external hardware commands. */
zlStatusE zl303xx_PtpClockTimeGet(zl303xx_PtpClockS *pPtpClock, zl303xx_PtpInterfaceE interface, zl303xx_TimeStamp *todTime);
zlStatusE zl303xx_PtpClockTimeSet(zl303xx_PtpClockS *pPtpClock, zl303xx_TimeStamp *todTime);
zlStatusE zl303xx_PtpPortPhysicalAddrGet(zl303xx_PortDataS *pPort, char *physicalLayerProtocol,
                                         Uint16T *physicalAddressLength, Uint8T *physicalAddress);
zlStatusE zl303xx_PtpClockLockStatusGet(zl303xx_PtpClockS *pPtpClock,
                                        Uint32T *lockStatus);
zlStatusE zl303xx_PtpClockStabilityStatusGet(zl303xx_PtpClockS *pPtpClock,
                                             Uint32T *freqStability,
                                             Sint32T *phaseStability);
zlStatusE zl303xx_PtpClockTimeSetStatusGet(zl303xx_PtpClockS *pPtpClock,
                                           zl303xx_BooleanE *timeSetStatus);
zlStatusE zl303xx_PtpClockPerformanceGet(zl303xx_PtpClockS *pPtpClock,
                                         zl303xx_PtpPllPerformanceDataS *pPllPerfData);
zl303xx_PtpTxStatusE zl303xx_PtpTxMsg(Uint8T *buffer, Uint16T bufLen, zl303xx_PtpInterfaceE ifType,
                           zl303xx_PortDataS *pPort, zl303xx_PortAddress *destAddr,
                           zl303xx_ClockStreamS *stream, zl303xx_MessageTypeE messageType);

/* Event wrappers. */
void zl303xx_PtpEventClockCreate(zl303xx_PtpClockS *pPtpClock);
void zl303xx_PtpEventStreamCreate(zl303xx_ClockStreamS *pStream, Uint8T *rxExtData,
                                  zl303xx_PtpStreamCreateS *newConfig);
void zl303xx_PtpEventContractRejected(zl303xx_PtpClockS *pClock, Uint32T handle,
                                      zl303xx_MessageTypeE msgType,
                                      zl303xx_PtpUniNegContractErrorE contractErr);
void zl303xx_PtpEventStreamStateChange(zl303xx_ClockStreamS *pPtpStream,
                                       zl303xx_PtpStreamOperatingStateE operState);
void zl303xx_PtpEventStreamKeepAlive(zl303xx_ClockStreamS *pPtpStream);
void zl303xx_PtpEventClockBmcaUpdate(zl303xx_PtpClockS *pPtpClock);
void zl303xx_PtpEventCounterAlarm(zl303xx_ClockStreamS *pStream, zl303xx_MessageTypeE msgType,
                                  Uint32T rxCountExpected);
void zl303xx_PtpEventSquelch(zl303xx_PtpClockS *pClock, zl303xx_BooleanE doSquelch);
void zl303xx_PtpEventMsgIntvlChange(zl303xx_ClockStreamS *pStream,
                                    zl303xx_MessageTypeE type,
                                    Sint8T prevIntvl, Sint8T currIntvl);
void zl303xx_PtpEventParentDsChange(zl303xx_PtpClockS *pPtpClock);
void zl303xx_PtpEventContractGranted(zl303xx_ClockStreamS *pStream,
                                     zl303xx_PtpV2SignalGrantS *pGrant,
                                     zl303xx_PtpContractFlowE flow);
void zl303xx_PtpEventContractExpired(zl303xx_ClockStreamS *pStream,
                                     zl303xx_MessageTypeE msgType,
                                     zl303xx_PtpContractFlowE flow);
void zl303xx_PtpEventMultiplePeerResp(zl303xx_ClockStreamS *pStream,
                                      Uint16T sequenceId,
                                      zl303xx_TimeStamp *rxTs,
                                      zl303xx_PortIdentity *txPortIdentity,
                                      zl303xx_PortIdentity *reqPortIdentity);
void zl303xx_PtpEventUnknownTlv(zl303xx_PortDataS *pPtpPort,
                                zl303xx_ClockStreamS *pPtpStream,
                                zl303xx_PtpV2MsgS *pPtpMsgRx,
                                Uint16T tlvLength,
                                Uint8T *pInTlvBuf);
void zl303xx_PtpEventLeapSecondsFlagChange(zl303xx_PtpClockS *pPtpClock,
                                           zl303xx_TimePropertiesDS *previousTimeProperties);
void zl303xx_PtpEventUtcOffsetChange(zl303xx_PtpClockS *pPtpClock,
                                     zl303xx_TimePropertiesDS *previousTimeProperties);
void zl303xx_PtpEventRxStepsRemovedExceeded(zl303xx_PortDataS *pPtpPort,
                                            zl303xx_PtpV2MsgS *ptpAnncMsg);
void zl303xx_PtpEventStreamDisqualified(zl303xx_ClockStreamS *pPtpStream);

#endif
