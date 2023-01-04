

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Architecture dependent datatypes
*
*******************************************************************************/

#ifndef ZL303XX_PTP_DEP_H
#define ZL303XX_PTP_DEP_H

#include <signal.h>
#include "zl303xx_PtpDatatypes.h"

/* define a string for the time format and macros for the individual components.
   To use this, with for example printf, use:
   printf("Time string" PTPD_FORMAT_TIME_STR "\n", formatTimeStringSign(time),
                                                   formatTimeStringSec(time),
                                                   formatTimeStringNs(time));
   */
#define PTPD_FORMAT_TIME_STR  "%c%u.%09u"
/* This is not strictly correct as the seconds field is actually 48 bits, but we
   just limit it for now */
#define formatTimeStringSign(tr)   (((tr)->nanosecondsField & 0x80000000) ? '-' : '+')
#define formatTimeStringSec(tr)    (((tr)->secondsField.hi > 0) ? 0xFFFFFFFF : (tr)->secondsField.lo)
#define formatTimeStringNs(tr)     ((tr)->nanosecondsField & ~0x80000000)

/* system messages */

/* endian corrections */
/* shift8 shifts an 8 bit value to byte position 0,1,2 or 3 (numbered left to
   right) within a 32 bit network order word */
/* shift16 shifts a 16 bit value to word position 0 or 1 within a 32 bit network
   order word */
#if defined(PTPD_MSBF)
#define shift8(x,y)       ( (Uint32T)(x) << ((3-y)<<3) )
#define shift16(x,y)      ( (Uint32T)(x) << ((1-y)<<4) )
#elif defined(PTPD_LSBF)
#define shift8(x,y)       ( (Uint32T)(x) << ((y)<<3) )
#define shift16(x,y)      ( (Uint32T)(x) << ((y)<<4) )
#endif

#define flip16(x)         (Uint16T)htons((x))
#define flip32(x)         (Uint32T)htonl((x))
/* i don't know any target platforms that do not have htons and htonl,
   but here is an agnostic form just in case */
/*
#if defined(PTPD_MSBF)
#define flip16(x)         (x)
#define flip32(x)         (x)
#elif defined(PTPD_LSBF)
#define flip16(x)         ((((x) >> 8) & 0x00ff) | \
                           (((x) << 8) & 0xff00))
#define flip32(x)         ((((x) >> 24) & 0x000000ff) | \
                           (((x) >> 8 ) & 0x0000ff00) | \
                           (((x) << 8 ) & 0x00ff0000) | \
                           (((x) << 24) & 0xff000000) )

#endif
*/

/* bit manipulation macros for the header flag.
   In each case:
   x is the overall 16-bit flag value
   y is a 16-bit wide bit mask (to set or clear) */
#define setFlag(x,y)       ((x) |= (Uint16T)(y))
#define getFlag(x,y)       ((((x) & (Uint16T)(y)) == 0) ? ZL303XX_FALSE : ZL303XX_TRUE)
#define clearFlag(x,y)     ((x) &= (Uint16T)(~((Uint16T)(y))))
#define clearAllFlags(x)   ((x) = (Uint16T)(0))

/* msg.c */


/* Management Message Routines */
Uint16T zl303xx_V2HandleManagementTLV(
      zl303xx_PtpV2MgmtMsgScopeE msgScope, zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *iBuf, Uint16T ibufLength, Uint8T *oBuf, zl303xx_PortDataS *portData, Uint16T *addedBytes);


/* servo.c */
void zl303xx_PtpUpdateDelay(zl303xx_ClockStreamS *clockStream, zl303xx_TimeStamp *send_time,
                            zl303xx_TimeStamp *recv_time, Uint64S *correctionField,
                            Uint16T seqId, zl303xx_PortIdentity *srcPort);
void zl303xx_PtpUpdateOffset(zl303xx_ClockStreamS *clockStream, zl303xx_TimeStamp *send_time,
                             zl303xx_TimeStamp *recv_time, Uint64S *correctionField,
                             Uint16T seqId, zl303xx_PortIdentity *srcPort);
void zl303xx_PtpUpdatePeerDelay(zl303xx_ClockStreamS *clockStream,
                                zl303xx_TimeStamp *send_time,   /* PReq: T1 */
                                zl303xx_TimeStamp *recv_time,   /* PResp: T4 */
                                Uint64S * correctionField,    /* From the RX PResp or FollowUp message. */
                                Uint16T seqId);


#endif
