

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
* Copyright 2006-2018 Microsemi Semiconductor Limited.
* All rights reserved.
*
*  Module Description:
*     The IEEE-1588 PTP Protocol main header file
*
*******************************************************************************/

#ifndef ZL303XX_PTPD_H
#define ZL303XX_PTPD_H

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Os.h"
#include "zl303xx_PtpFunctionExport.h"
#include "zl303xx_PtpEventHandling.h"
#include "zl303xx_PtpConstants.h"
#include "zl303xx_PtpConstants_dep.h"
#include "zl303xx_PtpDatatypes.h"
#include "zl303xx_Ptp_dep.h"
#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpInternal.h"
#include "zl303xx_PtpSetup.h"
#include "zl303xx_PtpApi.h"

#include <stddef.h>
#include <string.h>

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/
/* zl303xx_Ptpd.c */
void zl303xx_PtpPrintLocalUtcTime(zl303xx_PtpClockS *pClock);
void zl303xx_PtpPrintMasterUtcTime(void);

/* Control functions accessible from the command line */
zlStatusE zl303xx_PtpSetDebugLevel(Uint32T newLevel);
zlStatusE zl303xx_PtpEnableUtcTimeDisplay(zl303xx_PtpClockHandleT clockHandle, Uint32T enable);

/* zl303xx_PtpArith.c */
Sint32T log2IntvlToPps(Sint8T log2Intvl);
Uint32T crc_algorithm(Uint8T*,Sint16T);
void sub_pos_times(zl303xx_TimeStamp*,zl303xx_TimeStamp*,zl303xx_TimeStamp*);
void add_pos_times(zl303xx_TimeStamp*,zl303xx_TimeStamp*,zl303xx_TimeStamp*);
void subTime(zl303xx_TimeStamp*,zl303xx_TimeStamp*,zl303xx_TimeStamp*);
void addTime(zl303xx_TimeStamp*,zl303xx_TimeStamp*,zl303xx_TimeStamp*);
void divTime(zl303xx_TimeStamp*,zl303xx_TimeStamp*,Sint32T);
void mulTime(zl303xx_TimeStamp*,zl303xx_TimeStamp*,Uint32T);
void negateTime(zl303xx_TimeStamp *x);
zl303xx_BooleanE isNegativeTime(zl303xx_TimeStamp *x);
zl303xx_BooleanE isZeroTime(zl303xx_TimeStamp *x);
Sint32T networkNsToInternalNs(Uint32T netNs);
Uint32T internalNsToNetworkNs(Sint32T internalNs);
/* Function used to calculate log2PacketsPerSecond based on packets per second */
Sint32T log2PacketsPerSecond(Sint16T pktsPerSec);
Sint32T log2SecondsPerPacket(Sint16T secsPerPkt); /* Calculates log2(n) */

/* V1 bmc.c */

/* V2 bmc.c */

/* V1 protocol.c */
void zl303xx_PtpStreamInitGenericCalcValues(zl303xx_ClockStreamS *clockStream);

/* PTP Message Interval calculations. */
void zl303xx_PtpConvertLog2IntervalToTime(Sint8T log2Interval, zl303xx_TimeStamp *intervalTime);
void zl303xx_PtpGetSyncIntervalTimeout(zl303xx_ClockStreamS *streamData, zl303xx_TimeStamp *timeout);
void zl303xx_PtpGetSyncReceiptTimeout(zl303xx_ClockStreamS *streamData, zl303xx_TimeStamp *timeout);
void zl303xx_PtpGetDelayReqIntervalTimeout(zl303xx_ClockStreamS *streamData, zl303xx_TimeStamp *timeout);
void zl303xx_PtpGetPeerDelayReqIntervalTimeout(zl303xx_ClockStreamS *streamData, zl303xx_TimeStamp *timeout);
void zl303xx_PtpGetAnnounceIntervalTimeout(zl303xx_ClockStreamS *streamData, zl303xx_TimeStamp *timeout);
void zl303xx_PtpGetAnnounceReceiptTimeout(zl303xx_ClockStreamS *streamData, zl303xx_TimeStamp *timeout);
void zl303xx_PtpGetQualificationTimeout(zl303xx_ClockStreamS *streamData, zl303xx_TimeStamp *timeout);
void zl303xx_PtpGetPreMasterTimeout(zl303xx_ClockStreamS *streamData, zl303xx_TimeStamp *timeout);
void zl303xx_PtpGetUncalibratedTimeout(zl303xx_ClockStreamS *streamData, zl303xx_TimeStamp *timeout);

/* V2 protocol.c */
void zl303xx_PtpV2Protocol(zl303xx_PortDataS *portData, zl303xx_ClockStreamS *streamData);
void zl303xx_PtpV2handleRecvMsg(zl303xx_ClockStreamS *streamData, zl303xx_PortDataS *portData, zl303xx_PtpClockMsgQueueS *ptpData);

void zl303xx_PtpDoPortState(zl303xx_PortDataS *portData);
void zl303xx_PtpV2ToPortState(zl303xx_PortDataS *portData, zl303xx_PtpPortInternalStateE nextState);
zl303xx_PortStateE zl303xx_GetV2PortState(zl303xx_PortDataS *portData);

void zl303xx_PtpV2ToStreamState(zl303xx_PtpStreamSubStateE nextState, zl303xx_ClockStreamS *clockStream);

void zl303xx_PtpV2IssueAnnounce(zl303xx_ClockStreamS *clockStream);
void zl303xx_PtpV2IssueSync(zl303xx_ClockStreamS *clockStream);
void zl303xx_PtpV2IssueDelayReq(zl303xx_ClockStreamS *streamData);
void zl303xx_PtpV2IssuePeerDelayReq(zl303xx_ClockStreamS *streamData);
void zl303xx_PtpV2IssuePeerDelayResp(zl303xx_ClockStreamS *streamData,
                                     zl303xx_PtpV2MsgHeaderS *rxHeader,
                                     zl303xx_TimeStamp *delayReqRecvTime);

void zl303xx_PtpV2HandleTsRecordReady(zl303xx_ClockStreamS *clockStream,
                                      zl303xx_PtpTsRecordTblEntryS *tsRecord);
void zl303xx_IncrementMalformedPktCount(zl303xx_PortDataS *portData);

/* V2 unicast protocol.c */
zlStatusE zl303xx_PtpV2UnicastOncePerSecond(zl303xx_ClockStreamS *clockStream);
void _zl303xx_PtpV2DoUnicastState(zl303xx_ClockStreamS *clockStream);

zlStatusE zl303xx_PtpV2InitUnicastStreamData(zl303xx_ClockStreamS *clockStream);

zlStatusE zl303xx_PtpV2CheckUnicastSlaveData(Uint32T desiredAnnounceGrantSecs,
                                             Uint32T desiredSyncGrantSecs,
                                             Uint32T desiredDelayRespGrantSecs,
                                             Uint32T desiredPdelayRespGrantSecs,
                                             Uint32T renewSecs);
zlStatusE zl303xx_PtpV2SetUnicastSlaveData(zl303xx_ClockStreamS *clockStream,
                                    Sint8T maxAnnounceIntvl, Uint32T maxAnnounceGrantSec,
                                    Sint8T maxSyncIntvl, Uint32T maxSyncGrantSec,
                                    Sint8T maxDelayRespIntvl, Uint32T maxDelayRespGrantSec,
                                    Sint8T maxPdelayRespIntvl, Uint32T maxPdelayRespGrantSec,
                                    Uint32T renewSec);

void zl303xx_PtpV2StopUnicastStream(zl303xx_ClockStreamS *stream);

/* PtpClock.c */
zlStatusE zl303xx_PtpFindFirstValidClock(zl303xx_PtpClockS **ppPtpClock);
zlStatusE zl303xx_PtpFindFirstActiveStream(zl303xx_ClockStreamS **clockStream);

/* Ptpd.c */
/* Default Profile Information routines: linked to the ProfileDef module. */
zlStatusE zl303xx_PtpDefaultProfileE2EInfoGet(const zl303xx_PtpProfileS **profileInfo);
zlStatusE zl303xx_PtpDefaultProfileP2PInfoGet(const zl303xx_PtpProfileS **profileInfo);
zlStatusE zl303xx_PtpDefaultProfileDurationCheck(Uint32T *duration, zl303xx_BooleanE strictFlag);


#endif
