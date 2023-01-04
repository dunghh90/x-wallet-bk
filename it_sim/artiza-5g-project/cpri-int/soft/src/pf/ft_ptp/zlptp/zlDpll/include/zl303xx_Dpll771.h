/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     ZL30361 DPLL phase step (jump) function protos and registers.
*
*******************************************************************************/

#ifndef ZL303XX_DPLL_771_H_
#define ZL303XX_DPLL_771_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_AddressMap77x.h"

/*****************   TYPES   ************************************************/

typedef enum
{
    ZLS3077X_NORMAL_I_PART                                  = 0,
    ZLS3077X_NORMAL_HOLDOVER                                = 1,
    ZLS3077X_NORMAL_P_PLUS_I_PARTS                          = 2,
    ZLS3077X_NORMAL_P_PART                                  = 3,

    ZLS3077X_HOLDOVER_FILTER_1                              = 0,
    ZLS3077X_HOLDOVER_I_PART_LATCHED_BEFORE_HOLDOVER        = 4,
    ZLS3077X_HOLDOVER_FILTER_2                              = 5,
    ZLS3077X_HOLDOVER_P_PLUS_I_PART_LATCHED_BEFORE_HOLDOVER = 6,
    ZLS3077X_HOLDOVER_P_PART_LATCHED_BEFORE_HOLDOVER        = 7,

    ZLS3077X_NCO_I_PART                                     = 0,
    ZLS3077X_NCO_HOLDOVER                                   = 1,
    ZLS3077X_NCO_NORMAL_OR_HOLDOVER                         = 2,
    ZLS3077X_NCO_P_PLUS_I_PARTS                             = 6,
    ZLS3077X_NCO_P_PART                                     = 3

} ZLS3077X_MemPartE;


typedef enum
{
   ZLS3077X_DPLL_TIE_COMPLETE   = 0,
   ZLS3077X_DPLL_MTIE_SNAP      = 1,
   ZLS3077X_DPLL_TIE_READ       = 2,
   ZLS3077X_DPLL_TIE_WRITE      = 3
} ZLS3077X_DpllTieCtrlE;

#define ZLS3077X_DPLL_TIE_CTRL_MASK     (0x03)


/*****************   DEFINES   ************************************************/

/* TIE control registers */
#define ZLS3077X_DPLLX_TIE                          ZL303XX_MAKE_MEM_ADDR_77X(0x063A, ZL303XX_MEM_SIZE_1_BYTE)
#define ZLS3077X_DPLLX_TIE_WR_THRESH                ZL303XX_MAKE_MEM_ADDR_77X(0x063B, ZL303XX_MEM_SIZE_1_BYTE)
#define ZLS3077X_TIE_WR_STATUS_REG                  ZL303XX_MAKE_MEM_ADDR_77X(0x018D, ZL303XX_MEM_SIZE_1_BYTE)
#define ZLS3077X_TIE_WR_STATUS_MASK                 (0x3)


/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

zlStatusE zl303xx_Dpll77xRequestFreqRead(void *hwParams, ZLS3077X_MemPartE memPart);
Sint32T zl303xx_Dpll77xGetFreq(void *hwParams, Sint32T *freqOffsetInppt,
                                            ZLS3077X_MemPartE memPart);
Sint32T zl303xx_Dpll77xGetDeviceFreq(void *hwParams, Sint32T *freqOffsetInppt,
                                            ZLS3077X_MemPartE memPart);
zlStatusE zl303xx_Dpll77xInputPhaseErrorThresholdSet(void *hwParams, Uint32T threshNs);
zlStatusE zl303xx_Dpll77xInputPhaseErrorClearSet(void *hwParams, zl303xx_BooleanE tieWrClear);
zlStatusE zl303xx_Dpll77xTieCtrlSet(void *hwParams, ZLS3077X_DpllTieCtrlE oper);
zlStatusE zl303xx_Dpll77xTieCtrlReady(void *hwParams, zl303xx_BooleanE *ready);
zlStatusE zl303xx_Dpll77xInputPhaseErrorRead(void *hwParams, Uint32T timeoutMs,
                                                            Sint32T *tieNs);
zlStatusE zl303xx_Dpll77xInputPhaseErrorClear(void *hwParams);
zlStatusE zl303xx_Dpll77xInputPhaseErrorStatusSet(void *hwParams, Uint32T v);
zlStatusE zl303xx_Dpll77xInputPhaseErrorStatusGet(void *hwParams, zl303xx_BooleanE *complete);
zlStatusE zl303xx_Dpll77xAdjustPhaseUsingHitlessCompensation(void *hwParams);
Sint32T zl303xx_Dpll77xTakeHwNcoControl(void *hwParams);
Sint32T zl303xx_Dpll77xReturnHwNcoControl(void *hwParams);

/* Callback functions */
Sint32T zl303xx_Dpll77xGetDeviceInfo(void *hwParams, zl303xx_DevTypeE *devType, zl303xx_DeviceIdE *devId, Uint32T *devRev);
Sint32T zl303xx_Dpll77xBCHybridActionPhaseLock(void *hwParams);
Sint32T zl303xx_Dpll77xBCHybridActionOutOfLock(void *hwParams);
Sint32T zl303xx_Dpll77xSetActiveElecActionsInitial(void *hwParams, Sint32T refId, Sint32T syncId);
Sint32T zl303xx_Dpll77xSetActiveElecActionsTransientStage1(void *hwParams, Uint32T refId);
Sint32T zl303xx_Dpll77xSetActiveElecActionsTransientStage2(void *hwParams, Uint32T refId);
zlStatusE zl303xx_Dpll77xSetNCOAssistEnable(void *hwParams, zl303xx_BooleanE enable);
zlStatusE zl303xx_Dpll77xGetNCOAssistEnable(void *hwParams, zl303xx_BooleanE *enable);

/* CGU Message Router callback function */
Sint32T zl303xx_Dpll77xMsgRouter(void *hwParams, void *inData, void *outData);

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
