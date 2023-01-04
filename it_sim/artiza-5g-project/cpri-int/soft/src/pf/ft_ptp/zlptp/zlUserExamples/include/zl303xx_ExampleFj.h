

/*******************************************************************************
*
*  zl303xx_ExampleFj.h
*
*  Copyright 2018 
*  All rights reserved.
*
*  Module Description:
*     FJ customize functions
*
*******************************************************************************/

#ifndef ZL303XX_EXAMPLE_FJ_H_
#define ZL303XX_EXAMPLE_FJ_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>

/*****************   INCLUDE FILES   ******************************************/


/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/
zlStatusE set_rtc_tsu(zl303xx_AprTimeRepresentationS );
int getptpLockState(void *hwParams);


#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */

