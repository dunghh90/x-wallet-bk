

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Time stamp logging/debug data types.
*
*******************************************************************************/

#ifndef ZL303XX_TS_DEBUG_H
#define ZL303XX_TS_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"

/*****************   DATA TYPES   *********************************************/
typedef struct
{
    Uint8T fwd;
    Uint16T sequenceNum;
    Uint32T streamNum;
    Uint64S txTs; 
    Uint64S rxTs; 
    Uint64S corr; 
    Uint32T txSubSec;
    Uint32T rxSubSec;
} zl303xx_TsLogDataS;

typedef Sint32T (*swFuncPtrTSLogging)(zl303xx_TsLogDataS *tsData);

#ifdef __cplusplus
}
#endif

#endif   /* MULTIPLE INCLUDE BARRIER */
