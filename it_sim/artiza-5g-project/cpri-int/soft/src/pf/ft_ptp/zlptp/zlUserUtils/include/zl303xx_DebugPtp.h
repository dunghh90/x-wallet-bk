

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains miscellaneous debug functions for PTP.
*
*******************************************************************************/

#ifndef _ZL303XX_DEBUG_PTP_H_
#define _ZL303XX_DEBUG_PTP_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_PtpApiTypes.h"

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/
zlStatusE zl303xx_DebugClockDataSet(zl303xx_PtpClockHandleT clockHandle);
zlStatusE zl303xx_DebugPortDataSet(zl303xx_PtpPortHandleT clockPortHandle);
zlStatusE zl303xx_DebugStreamDataSet(zl303xx_PtpStreamHandleT clockStreamHandle);
zlStatusE zl303xx_DebugAllDataSet(void);

zlStatusE zl303xx_DebugPtpClockProfileShow(zl303xx_PtpClockHandleT clockHandle);

zlStatusE zl303xx_DebugPtpMsgCount(zl303xx_PtpStreamHandleT streamHandle);
zlStatusE zl303xx_DebugPtpMsgCountWithOptionalReset(zl303xx_PtpStreamHandleT streamHandle,zl303xx_BooleanE reset);
zlStatusE zl303xx_DebugPtpMsgCountToString(zl303xx_PtpStreamHandleT streamHandle,char *str);


zlStatusE zl303xx_DebugPtpPortAmt(zl303xx_PtpPortHandleT portHandle);
zlStatusE zl303xx_DebugPtpPortAst(zl303xx_PtpPortHandleT portHandle);
zlStatusE zl303xx_DebugPtpPortContracts(zl303xx_PtpPortHandleT portHandle);
zlStatusE zl303xx_DebugPtpPortGrantTbl(zl303xx_PtpPortHandleT portHandle);
zlStatusE zl303xx_DebugPtpPortFmt(zl303xx_PtpPortHandleT portHandle);
zlStatusE zl303xx_DebugPtpClockFmt(zl303xx_PtpClockHandleT clockHandle);
zlStatusE zl303xx_DebugPtpPortFmtFlush(zl303xx_PtpPortHandleT portHandle);
zlStatusE zl303xx_DebugPtpClockFmtFlush(zl303xx_PtpClockHandleT clockHandle);
zlStatusE zl303xx_DebugPtpPortUmt(zl303xx_PtpPortHandleT portHandle);
zlStatusE zl303xx_DebugStreamEgressAnncShow(zl303xx_PtpStreamHandleT clockStreamHandle);


zlStatusE zl303xx_DebugPtpProductInfo(void);
void zl303xx_DebugPtpBuildInfo(void);

zlStatusE zl303xx_DebugPtpStreamRequestGrant(zl303xx_PtpStreamHandleT streamHandle,
      Uint32T messageType, Sint8T *pLogInterval, Uint32T *pDuration);

zlStatusE zl303xx_DebugPtpClockUnicastServiceLimits(zl303xx_PtpClockHandleT clockHandle);
void zl303xx_DebugStreamPointersShow(zl303xx_PtpClockHandleT clockHandle);

void zl303xx_DebugPtpStreamEgressOverrideSet(
      zl303xx_PtpStreamHandleT streamHandle,
      Uint32T overrideId,
      Sint32T value);
	  
/*structure containing  PTP messages counters for the specified stream.*/
typedef struct zl303xx_DebugPtpMsgCountStruct
{
	/*PTP ingress message count:
	10 rows and 3 columns
	
	column 0: # messages received
	column 1: # messages dropped
	column 2: # messages lost
	
	row 0: SYNC
	row 1: DELAY REQUEST
	row 2: PEER DELAY REQUEST
	row 3: PEER DELAY RESPONSE
	row 4: FOLLOW UP
	row 5: DELAY RESPONSE
	row 6: PEER DELAY FOLLOW UP
	row 7: ANNOUNCE
	row 8: SIGNALING
	row 9: MANAGEMENT	
	*/
	Uint32T PTP_ingressMessages[10][3];
	
	
	/*PTP egress message count:
	10 rows and 1 column
	
	column 0: # messages transmitted
	
	row 0: SYNC
	row 1: DELAY REQUEST
	row 2: PEER DELAY REQUEST
	row 3: PEER DELAY RESPONSE
	row 4: FOLLOW UP
	row 5: DELAY RESPONSE
	row 6: PEER DELAY FOLLOW UP
	row 7: ANNOUNCE
	row 8: SIGNALING
	row 9: MANAGEMNET
	*/
	Uint32T PTP_egressMessages[10];
  
} zl303xx_DebugPtpMsgCountStruct;

zlStatusE zl303xx_DebugPtpMsgCountGet(zl303xx_PtpStreamHandleT streamHandle,zl303xx_DebugPtpMsgCountStruct *PtpMsgPtr);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
