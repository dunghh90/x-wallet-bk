

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Top level header file that includes all the other header files for the API
*
*******************************************************************************/

#ifndef ZL303XX_API_TOP_H
#define ZL303XX_API_TOP_H

/*****************   INCLUDE FILES                *****************************/

#include "zl303xx_Global.h"  /* This should always be the first file included */

/* Now include the porting library since most other components depend on it */
#include "zl303xx_Porting.h"
#include "zl303xx.h"
/* include other header files from this directory */
#include "zl303xx_Init.h"
#include "zl303xx_Spi.h"
#include "zl303xx_RdWr.h"
#if defined ZLS30341_INCLUDED
#include "zl303xx_ApiInterrupt.h"
#include "zl303xx_Interrupt.h"
#include "zl303xx_Dpll34xDco.h"
#include "zl303xx_TsEng.h"
#endif
/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/
/* API description strings */
extern const char zl303xx_ApiBuildDate[];
extern const char zl303xx_ApiBuildTime[];
extern const char zl303xx_ApiReleaseDate[];
extern const char zl303xx_ApiReleaseTime[];
extern const char zl303xx_ApiReleaseVersion[];
extern const char zl303xx_ApiPatchLevel[];
extern const char zl303xx_ApiReleaseSwId[];

#endif   /* MULTIPLE INCLUDE BARRIER */
