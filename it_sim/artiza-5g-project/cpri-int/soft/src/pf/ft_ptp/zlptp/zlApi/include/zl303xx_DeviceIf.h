

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This file contains functions related to the ...
*
*******************************************************************************/

#ifndef _ZL303XX_DEVICE_IF_H_
#define _ZL303XX_DEVICE_IF_H_

#if defined __cplusplus
extern "C" {
#endif


/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/* Device family defines */
typedef enum
{
    UNKNOWN_DEVICETYPE = 0,
    ZL3031X_DEVICETYPE,
    ZL30330_DEVICETYPE,
    ZL3034X_DEVICETYPE,
    ZL3036X_DEVICETYPE,
    ZL3075X_DEVICETYPE,
    ZL3072X_DEVICETYPE,
    ZL3070X_DEVICETYPE,
    ZL3077X_DEVICETYPE,
    CUSTOM_DEVICETYPE
} zl303xx_DevTypeE;


/* DeviceId valid values */
typedef enum
{
    UNKNOWN_DEV_TYPE = 0,

    ZL30161_DEV_TYPE = 0xA1,
    ZL30162_DEV_TYPE = 0xA2,
    ZL30163_DEV_TYPE = 0xA3,
    ZL30164_DEV_TYPE = 0xA4,
    ZL30165_DEV_TYPE = 0xA5,
    ZL30166_DEV_TYPE = 0xA6,
    ZL30167_DEV_TYPE = 0xA7,
    ZL30168_DEV_TYPE = 0x14,    /* Corrected in EBinder 14522 */

    ZL30342_DEV_TYPE = 0xEC,
    ZL30343_DEV_TYPE = 0xED,
    ZL30347_DEV_TYPE = 0xEF,

    ZL30361_DEV_TYPE = 0x3D,
    ZL30362_DEV_TYPE = 0x3E,
    ZL30363_DEV_TYPE = 0x3F,
    ZL30364_DEV_TYPE = 0x40,
    ZL30365_DEV_TYPE = 0x41,
    ZL30366_DEV_TYPE = 0x42,
    ZL30367_DEV_TYPE = 0x43,

    ZL303XX_DEVICE_ID_ZL30250 = 0x00,
    ZL303XX_DEVICE_ID_ZL30251 = 0x01,
    ZL303XX_DEVICE_ID_ZL30252 = 0x02,
    ZL303XX_DEVICE_ID_ZL30253 = 0x03,
    ZL303XX_DEVICE_ID_ZL30244 = 0x04,
    ZL303XX_DEVICE_ID_ZL30151 = 0x05,
    ZL303XX_DEVICE_ID_ZL30245 = 0x06,
    ZL303XX_DEVICE_ID_ZL30169 = 0x07,
    ZL303XX_DEVICE_ID_ZL30182 = 0x08,
    ZL303XX_DEVICE_ID_ZL30621 = 0x09,
    ZL303XX_DEVICE_ID_ZL30622 = 0x0A,
    ZL303XX_DEVICE_ID_ZL30623 = 0x0B,
    ZL303XX_DEVICE_ID_ZL30255 = 0x0E, /* End of real ones! */
    /* Encoded with rev 2 so these are FAKE IDS we use within the s/w */
    ZL303XX_DEVICE_ID_ZL30721 = 0x29, /* Rev 2, ID 0x9 */
    ZL303XX_DEVICE_ID_ZL30722 = 0x2A, /* Rev 2, ID 0xA */
    ZL303XX_DEVICE_ID_ZL30723 = 0x2B,  /* Rev 2, ID 0xB */

    ZL303XX_DEVICE_ID_ZL30174 = 0x0c66,
    ZL303XX_DEVICE_ID_ZL30611 = 0x0e1b,
    ZL303XX_DEVICE_ID_ZL30612 = 0x0e1c,
    ZL303XX_DEVICE_ID_ZL30614 = 0x0e1e,
    ZL303XX_DEVICE_ID_ZL30601 = 0x0e11,
    ZL303XX_DEVICE_ID_ZL30602 = 0x0e12,
    ZL303XX_DEVICE_ID_ZL30603 = 0x0e13,
    ZL303XX_DEVICE_ID_ZL30604 = 0x0e14,
    ZL303XX_DEVICE_ID_ZL30701 = 0x0e75,
    ZL303XX_DEVICE_ID_ZL30702 = 0x0e76,
    ZL303XX_DEVICE_ID_ZL30703 = 0x0e77,
    ZL303XX_DEVICE_ID_ZL30704 = 0x0e78,
    ZL303XX_DEVICE_ID_ZL80029 = 0x1e5d,

    ZL303XX_DEVICE_ID_ZL30652 = 0x0E44,
    ZL303XX_DEVICE_ID_ZL30653 = 0x0E45,
    ZL303XX_DEVICE_ID_ZL30654 = 0x0E46,
    ZL303XX_DEVICE_ID_ZL30752 = 0x0EA8,
    ZL303XX_DEVICE_ID_ZL30753 = 0x0EA9,
    ZL303XX_DEVICE_ID_ZL30754 = 0x0EAA,

    ZL303XX_DEVICE_ID_ZL30671 = 0x0E57,
    ZL303XX_DEVICE_ID_ZL30672 = 0x0E58,
    ZL303XX_DEVICE_ID_ZL30673 = 0x0E59,
    ZL303XX_DEVICE_ID_ZL80062 = 0x1F7E,
    ZL303XX_DEVICE_ID_ZL30771 = 0x0EBB,
    ZL303XX_DEVICE_ID_ZL30772 = 0x0EBC,
    ZL303XX_DEVICE_ID_ZL30773 = 0x0EBD

} zl303xx_DeviceIdE;


typedef enum
{
   ZL303XX_REF_AUTO = -1,
   ZL303XX_REF_ID_0 = 0,
   ZL303XX_REF_ID_1 = 1,
   ZL303XX_REF_ID_2 = 2,
   ZL303XX_REF_ID_3 = 3,
   ZL303XX_REF_ID_4 = 4,
   ZL303XX_REF_ID_5 = 5,
   ZL303XX_REF_ID_6 = 6,
   ZL303XX_REF_ID_7 = 7,
   ZL303XX_REF_ID_8 = 8,
   ZL303XX_REF_ID_9 = 9,
   ZL303XX_REF_ID_10= 10,
   ZL303XX_LAST_REF=ZL303XX_REF_ID_10
} zl303xx_RefIdE;


/* DpllId valid values */
typedef enum
{
    ZL303XX_DPLL_ID_1 = 0,
    ZL303XX_DPLL_ID_2,
    ZL303XX_DPLL_ID_3,
    ZL303XX_DPLL_ID_4
} zl303xx_DpllIdE;


/* A driver TOD done function type */
typedef Sint32T (*hwFuncPtrTODDone)(void*);


/* Shared types between APR and the drivers */
/* 1) Lock status */
typedef enum
{
   ZL303XX_LOCK_STATUS_ACQUIRING = 0,
   ZL303XX_LOCK_STATUS_LOCKED,
   ZL303XX_LOCK_STATUS_PHASE_LOCKED,
   ZL303XX_LOCK_STATUS_HOLDOVER,
   ZL303XX_LOCK_STATUS_REF_FAILED,
   ZL303XX_LOCK_NO_ACTIVE_SERVER,
   ZL303XX_LOCK_STATUS_UNKNOWN,
   ZL303XX_APRLOCKSTATE_MAX = ZL303XX_LOCK_STATUS_UNKNOWN
} zl303xx_AprLockStatusE;

#define  zl303xx_LockStatusE zl303xx_AprLockStatusE

/* 2) Holdover Quality */
typedef enum {
    HOLDOVER_QUALITY_UNKNOWN        = 0,
    HOLDOVER_QUALITY_IN_SPEC,
    HOLDOVER_QUALITY_LOCKED_TO_SYNCE,
    HOLDOVER_QUALITY_OUT_OF_SPEC,

    HOLDOVER_QUALITY_LAST = HOLDOVER_QUALITY_OUT_OF_SPEC
} zl303xx_HoldoverQualityTypesE;

/* 3) Hitless compensation types */
typedef enum
{
   ZL303XX_HITLESS_COMP_FALSE,
   ZL303XX_HITLESS_COMP_TRUE,
   ZL303XX_HITLESS_COMP_AUTO
} zl303xx_HitlessCompE;


/* CGU Message Router Section */
/* The CGU message router is a softare mechanism for the APR algorithm to
   interface with a Micorsemi or custom device.  The CGU message router
   defines all necessary HW device operations that will be required at
   run time.  */

/* Driver callout function type. A routine of this type is provided by each
   driver and passed to APR at initialisation. APR uses this callout to access
   driver functions. */
typedef Sint32T (*hwFuncPtrDriverMsgRouter)(void*, void*, void*);

/* Main Message Router Message Type ENUM*/
typedef enum {
    ZL303XX_DPLL_DRIVER_MSG_GET_DEVICE_INFO                               = 0,
    ZL303XX_DPLL_DRIVER_MSG_GET_FREQ_I_OR_P                               = 1,
    ZL303XX_DPLL_DRIVER_MSG_SET_FREQ                                      = 2,
    ZL303XX_DPLL_DRIVER_MSG_TAKE_HW_NCO_CONTROL                           = 3,
    ZL303XX_DPLL_DRIVER_MSG_RETURN_HW_NCO_CONTROL                         = 4,
    ZL303XX_DPLL_DRIVER_MSG_SET_TIME                                      = 5,
    ZL303XX_DPLL_DRIVER_MSG_STEP_TIME                                     = 6,
    ZL303XX_DPLL_DRIVER_MSG_JUMP_ACTIVE_CGU                               = 7,
    /* DEPRECATED                                                       = 8,*/
    ZL303XX_DPLL_DRIVER_MSG_GET_HW_MANUAL_FREERUN_STATUS                  = 9,
    ZL303XX_DPLL_DRIVER_MSG_GET_HW_MANUAL_HOLDOVER_STATUS                 = 10,
    ZL303XX_DPLL_DRIVER_MSG_GET_HW_SYNC_INPUT_EN_STATUS                   = 11,
    ZL303XX_DPLL_DRIVER_MSG_GET_HW_OUT_OF_RANGE_STATUS                    = 12,
    /* DEPRECATED                                                       = 13,*/
    ZL303XX_DPLL_DRIVER_MSG_DEVICE_PARAMS_INIT                            = 14,
    ZL303XX_DPLL_DRIVER_MSG_CURRENT_REF_GET                               = 15,
    ZL303XX_DPLL_DRIVER_MSG_CURRENT_REF_SET                               = 16,
    /* DEPRECATED                                                       = 17,*/
    /* DEPRECATED                                                       = 18,*/
    /* DEPRECATED                                                       = 19,*/
    /* DEPRECATED                                                       = 20,*/
    /* DEPRECATED                                                       = 21,*/
    /* DEPRECATED                                                       = 22,*/
    /* DEPRECATED                                                       = 23,*/
    /* DEPRECATED                                                       = 24,*/
    ZL303XX_DPLL_DRIVER_MSG_INPUT_PHASE_ERROR_WRITE                       = 25,
    ZL303XX_DPLL_DRIVER_MSG_INPUT_PHASE_ERROR_WRITE_CTRL_READY            = 26,
    /* DEPRECATED                                                       = 27,*/
    /* DEPRECATED                                                       = 28,*/
    /* DEPRECATED                                                       = 29,*/
    /* DEPRECATED                                                       = 30,*/
    /* DEPRECATED                                                       = 31,*/
    /* DEPRECATED                                                       = 32, */
    ZL303XX_DPLL_DRIVER_MSG_REGISTER_TOD_DONE_FUNC                        = 33,
    ZL303XX_DPLL_DRIVER_MSG_CONFIRM_HW_CNTRL                              = 34,
    ZL303XX_DPLL_DRIVER_MSG_GET_HW_LOCK_STATUS                            = 35,
    ZL303XX_DPLL_DRIVER_MSG_GET_STEP_TIME_CURR_MAX_STEP_SIZE              = 36,
    /* DEPRECATED                                                       = 37,*/
    ZL303XX_DPLL_DRIVER_MSG_CLEAR_HOLDOVER_FFO                            = 38,
    ZL303XX_DPLL_DRIVER_MSG_SET_AFBDIV                                    = 40,
    /* DEPRECATED                                                       = 41, */
    ZL303XX_DPLL_DRIVER_MSG_DETERMINE_MAX_STEP_SIZE_PER_ADJUSTMENT        = 42,
    /* DEPRECATED                                                       = 43,*/
    /* DEPRECATED                                                       = 44,*/
    /* DEPRECATED                                                       = 45,*/
    /* DEPRECATED                                                       = 46,*/
    ZL303XX_DPLL_DRIVER_MSG_ADJUST_TIME                                   = 47,  /* New for 4.9.0 */
    ZL303XX_DPLL_DRIVER_MSG_ADJUST_TIME_DCO_READABLE                      = 48,  /* New for 4.9.0 */
    ZL303XX_DPLL_DRIVER_MSG_BC_HYBRID_ACTION_PHASE_LOCK                   = 49,  /* New for 4.9.0 */
    ZL303XX_DPLL_DRIVER_MSG_BC_HYBRID_ACTION_OUT_OF_LOCK                  = 50,  /* New for 4.9.0 */
    ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_INITIAL                = 51,  /* New for 4.9.0 */
    ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_STAGE1                 = 52,  /* New for 4.9.0 */
    ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_STAGE2                 = 53,  /* New for 4.9.0 */
    ZL303XX_DPLL_DRIVER_MSG_SET_NCO_ASSIST_ENABLE                         = 54,
    ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_ENABLE                         = 55,
    ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_PARAMS                         = 56,
    ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_FREQ_OFFSET                    = 57,
    ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_HW_LOCK_STATUS                 = 58,
    ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_SYNC_INPUT_EN_STATUS           = 59,
    ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_OUT_OF_RANGE_STATUS            = 60,
    ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_MANUAL_HOLDOVER_STATUS         = 61,
    ZL303XX_DPLL_DRIVER_MSG_GET_NCO_ASSIST_MANUAL_FREERUN_STATUS          = 62,
    ZL303XX_DPLL_DRIVER_MSG_GET_MODIFY_STEP_TIME_NS                       = 63,
    ZL303XX_DPLL_DRIVER_MSG_PHASE_STEP_OUTPUT_IS_HP                       = 64,
    ZL303XX_DPLL_DRIVER_MSG_SET_MODE_HOLDOVER                             = 65
} zl303xx_DpllDriverMsgTypesE;


/* The following is the Dpll Driver Meessage ENUM for 4.8.5
typedef enum {
    ZL303XX_DPLL_DRIVER_MSG_GET_DEVICE_INFO                               = 0,
    ZL303XX_DPLL_DRIVER_MSG_TSXHGF                                        = 1,
    ZL303XX_DPLL_DRIVER_MSG_SET_FREQ                                      = 2,
    ZL303XX_DPLL_DRIVER_MSG_TSXHHWT                                       = 3,
    ZL303XX_DPLL_DRIVER_MSG_TSXHHWR                                       = 4,
    ZL303XX_DPLL_DRIVER_MSG_SET_TIME                                      = 5,
    ZL303XX_DPLL_DRIVER_MSG_STEP_TIME                                     = 6,
    ZL303XX_DPLL_DRIVER_MSG_JUMP_TIME_CGU                                 = 7,
    ZL303XX_DPLL_DRIVER_MSG_ADJUST_IF_HITLESS_COMPENSATION_BEING_USED     = 8,
    ZL303XX_DPLL_DRIVER_MSG_GET_HW_MANUAL_FREERUN_STATUS                  = 9,
    ZL303XX_DPLL_DRIVER_MSG_GET_HW_MANUAL_HOLDOVER_STATUS                 = 10,
    ZL303XX_DPLL_DRIVER_MSG_GET_HW_SYNC_INPUT_EN_STATUS                   = 11,
    ZL303XX_DPLL_DRIVER_MSG_GET_HW_OUT_OF_RANGE_STATUS                    = 12,
    ZL303XX_DPLL_DRIVER_MSG_NCO_STEP_TIME_PHASE_SET                       = 13,
    ZL303XX_DPLL_DRIVER_MSG_DEVICE_PARAMS_INIT                            = 14,
    ZL303XX_DPLL_DRIVER_MSG_CURRENT_REF_GET                               = 15,
    ZL303XX_DPLL_DRIVER_MSG_CURRENT_REF_SET                               = 16,
    ZL303XX_DPLL_DRIVER_MSG_FREQ_DETECT_GET                               = 17,
    ZL303XX_DPLL_DRIVER_MSG_CHECK_REF_SYNC_PAIR                           = 18,
    ZL303XX_DPLL_DRIVER_MSG_SET_ACTIVE_ELEC_REF_HITLESS_ACTIONS           = 19,
    ZL303XX_DPLL_DRIVER_MSG_GET_TRANSIENT_HITLESS_COMPENSATION_PARAMS     = 20,
    ZL303XX_DPLL_DRIVER_MSG_GET_USER_HITLESS_COMPENSATION_PARAMS          = 21,
    // DEPRECATED                                                       = 22,
    ZL303XX_DPLL_DRIVER_MSG_REF_SWITCH_HITLESS_COMPENSATION               = 23,
    ZL303XX_DPLL_DRIVER_MSG_NCO_STEP_TIME_PHASE_GET                       = 24,
    ZL303XX_DPLL_DRIVER_MSG_TSXHTW                                        = 25,
    ZL303XX_DPLL_DRIVER_MSG_TSXHTWSG                                      = 26,
    ZL303XX_DPLL_DRIVER_MSG_TSXHTWSTS                                     = 27,
    ZL303XX_DPLL_DRIVER_MSG_TSXHTWCS                                      = 28,
    ZL303XX_DPLL_DRIVER_MSG_TSXHPSSG                                      = 29,
    ZL303XX_DPLL_DRIVER_MSG_TSXHPSSC                                      = 30,
    ZL303XX_DPLL_DRIVER_MSG_TSXHNRSCS                                     = 31,
    ZL303XX_DPLL_DRIVER_MSG_GET_SYNTH_MASK                                = 32,
    ZL303XX_DPLL_DRIVER_MSG_REGISTER_TOD_DONE_FUNC                        = 33,
    ZL303XX_DPLL_DRIVER_MSG_CONFIRM_HW_CNTRL                              = 34,
    ZL303XX_DPLL_DRIVER_MSG_GET_HW_LOCK_STATUS                            = 35,
    ZL303XX_DPLL_DRIVER_MSG_GET_STEP_TIME_CURR_MAX_STEP_SIZE              = 36,
    ZL303XX_DPLL_DRIVER_MSG_TSXHTR                                        = 37,
    ZL303XX_DPLL_DRIVER_MSG_72X_CLEAR_HOLDOVER_FFO                        = 38,
    ZL303XX_DPLL_DRIVER_MSG_SET_AFBDIV                                    = 40,
    // DEPRECATED                                                       = 41,
    ZL303XX_DPLL_DRIVER_MSG_72X_DETERMINE_MAX_STEP_SIZE_PER_ADJUSTMENT    = 42,
    ZL303XX_DPLL_DRIVER_MSG_SET_TRANSIENT_HITLESS_COMPENSATION_PARAMS     = 43,
    ZL303XX_DPLL_DRIVER_MSG_SET_USER_HITLESS_COMPENSATION_PARAMS          = 44,
    ZL303XX_DPLL_DRIVER_MSG_ACCU_INPUT_PHASE_ERROR_SET                    = 45,
    ZL303XX_DPLL_DRIVER_MSG_ACCU_INPUT_PHASE_ERROR_GET                    = 46
} zl303xx_DpllDriverMsgTypesE;

For the 4.9.0 to 4.8.5 changes, we have DEPRECATED the following unused ENUMS
   - ZL303XX_DPLL_DRIVER_MSG_TSXHTWSTS
   - ZL303XX_DPLL_DRIVER_MSG_TSXHTWCS
   - ZL303XX_DPLL_DRIVER_MSG_TSXHPSSG
   - ZL303XX_DPLL_DRIVER_MSG_TSXHPSSC
   - ZL303XX_DPLL_DRIVER_MSG_TSXHNRSCS
   - ZL303XX_DPLL_DRIVER_MSG_TSXHTR
   - ZL303XX_DPLL_DRIVER_MSG_ACCU_INPUT_PHASE_ERROR_SET
   - ZL303XX_DPLL_DRIVER_MSG_ACCU_INPUT_PHASE_ERROR_GET
   - ZL303XX_DPLL_DRIVER_MSG_GET_TRANSIENT_HITLESS_COMPENSATION_PARAMS
   - ZL303XX_DPLL_DRIVER_MSG_GET_USER_HITLESS_COMPENSATION_PARAMS
   - ZL303XX_DPLL_DRIVER_MSG_SET_TRANSIENT_HITLESS_COMPENSATION_PARAMS
   - ZL303XX_DPLL_DRIVER_MSG_SET_USER_HITLESS_COMPENSATION_PARAMS
   - ZL303XX_DPLL_DRIVER_MSG_SET_ACTIVE_ELEC_REF_HITLESS_ACTIONS
   - ZL303XX_DPLL_DRIVER_MSG_CHECK_REF_SYNC_PAIR
   - ZL303XX_DPLL_DRIVER_MSG_NCO_STEP_TIME_PHASE_SET
   - ZL303XX_DPLL_DRIVER_MSG_NCO_STEP_TIME_PHASE_GET
   - ZL303XX_DPLL_DRIVER_MSG_FREQ_DETECT_GET
   - ZL303XX_DPLL_DRIVER_MSG_GET_SYNTH_MASK
   - ZL303XX_DPLL_DRIVER_MSG_REF_SWITCH_HITLESS_COMPENSATION
   - ZL303XX_DPLL_DRIVER_MSG_ADJUST_IF_HITLESS_COMPENSATION_BEING_USED

For the 4.9.0 to 4.8.5 changes, we have ADDED the following ENUMS
    ZL303XX_DPLL_DRIVER_MSG_ADJUST_TIME
    ZL303XX_DPLL_DRIVER_MSG_ADJUST_TIME_DCO_READABLE
    ZL303XX_DPLL_DRIVER_MSG_BC_HYBRID_ACTION_PHASE_LOCK
    ZL303XX_DPLL_DRIVER_MSG_BC_HYBRID_ACTION_OUT_OF_LOCK
    ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_INITIAL
    ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_STAGE1
    ZL303XX_DPLL_DRIVER_MSG_HITLESS_ELEC_SWITCHING_STAGE2

For the 4.9.0 to 4.8.5 changes, we have MODIFIED the following ENUMS for clarity
   - ZL303XX_DPLL_DRIVER_MSG_JUMP_TIME_CGU => ZL303XX_DPLL_DRIVER_MSG_JUMP_ACTIVE_CGU
   - ZL303XX_DPLL_DRIVER_MSG_TSXHGF    => ZL303XX_DPLL_DRIVER_MSG_GET_FREQ_I_OR_P
   - ZL303XX_DPLL_DRIVER_MSG_TSXHHWT   => ZL303XX_DPLL_DRIVER_MSG_TAKE_HW_NCO_CONTROL
   - ZL303XX_DPLL_DRIVER_MSG_TSXHHWR   => ZL303XX_DPLL_DRIVER_MSG_RETURN_HW_NCO_CONTROL
   - ZL303XX_DPLL_DRIVER_MSG_TSXHTW    => ZL303XX_DPLL_DRIVER_MSG_INPUT_PHASE_ERROR_WRITE
   - ZL303XX_DPLL_DRIVER_MSG_TSXHTWSG  => ZL303XX_DPLL_DRIVER_MSG_INPUT_PHASE_ERROR_WRITE_CTRL_READY
   - ZL303XX_DPLL_DRIVER_MSG_72X_CLEAR_HOLDOVER_FFO                      => ZL303XX_DPLL_DRIVER_MSG_CLEAR_HOLDOVER_FFO
   - ZL303XX_DPLL_DRIVER_MSG_72X_DETERMINE_MAX_STEP_SIZE_PER_ADJUSTMENT  => ZL303XX_DPLL_DRIVER_MSG_DETERMINE_MAX_STEP_SIZE_PER_ADJUSTMENT
*/

/* Data structure passed to and received from the given driver.
   - in data  = data into the driver
   - out data = data out of the driver
*/

/* get device info */
typedef struct {
    zl303xx_DevTypeE devType;
    zl303xx_DeviceIdE devId;
    Uint32T devRev;
} zl303xx_GetDeviceInfoOutDataS;

/* Get freq data */
typedef struct {
    Sint32T memPart;
} zl303xx_GetFreqInDataS;
typedef struct {
    Sint32T freqOffsetInPpt;
} zl303xx_GetFreqOutDataS;

/* Set freq data */
typedef struct {
    Sint32T freqOffsetInPpt;
} zl303xx_SetFreqInDataS;

/* SetTime */
typedef struct {
    Uint32T pllId;
    Uint64S seconds;
    Uint32T nanoSeconds;
    zl303xx_BooleanE bBackwardAdjust;
} zl303xx_SetTimeInDataS;

/* stepTime data */
typedef struct {
    Uint32T pllId;
    Sint32T deltaTime;
    zl303xx_BooleanE inCycles;
    Uint32T clockFreq;
} zl303xx_StepTimeInDataS;

/* jumpActiveCGU */
typedef struct {
    Uint32T pllId;
    Uint64S seconds;
    Uint32T nanoSeconds;
    zl303xx_BooleanE bBackwardAdjust;
} zl303xx_JumpActiveCGUInDataS;

/* Get HW manual freerun status data */
typedef struct {
    Sint32T status;
} zl303xx_GetHWManualFreerunOutDataS;

/* Get HW manual holdover status data */
typedef struct {
    Sint32T status;
} zl303xx_GetHWManualHoldoverOutDataS;

/* Get HW sync output enabled status data */
typedef struct {
    Sint32T status;
} zl303xx_GetHWSyncInputEnOutDataS;

/* Get HW out-of-range status data */
typedef struct {
    Sint32T status;
} zl303xx_GetHWOutOfRangeOutDataS;

/* device initialisation data */
typedef struct {
    void *hwParams;         /* DEPRECATED - hwParams is now passed as parameter */
} zl303xx_DeviceInitInDataS;

/* Get current ref data */
typedef struct {
    Uint32T ref;
} zl303xx_GetCurrRefOutDataS;

/* Set ref data */
typedef struct {
    Uint32T ref;
} zl303xx_SetCurrRefInDataS;

/* Input phase error write data */
typedef struct {
    Uint32T tieNs;
} zl303xx_InputPhaseErrorWriteInDataS;

/* Input phase error write control data data */
typedef struct {
    zl303xx_BooleanE ready;
} zl303xx_InputPhaseErrorWriteCtrlReadyOutDataS;

/* Set TOD done callback function data */
typedef struct {
   hwFuncPtrTODDone TODdoneFuncPtr;
} zl303xx_SetTODDoneFuncInDataS;

/* confirm Hw Cntrl data */
typedef struct {
    Uint32T addr;
} zl303xx_ConfirmHwCntrlInDataS;
typedef struct {
    Uint32T data;
} zl303xx_ConfirmHwCntrlOutDataS;

/* Get HW lock status data */
typedef struct {
    Sint32T lockStatus;
} zl303xx_GetHWLockStatusOutDataS;

/* get step time current max step size*/
typedef struct {
    Sint32T size;
} zl303xx_GetStepTimeCurrMaxStepSizeOutDataS;

/* Set AFBDIV */
typedef struct {
    Sint32T df;
} zl303xx_SetAFBDIVInDataS;

/* determine the max step size that the  */
typedef struct {
    Uint32T maxStepFaster;
    Uint32T maxStepSlower;
} zl303xx_DetermineMaxStepSizePerAdjustmentOutDataS;

/* Adjust time function from customer */
typedef struct {
    Sint32T adjustment;
    Uint32T recomendedTime;
} zl303xx_AdjustTimeInDataS;

/* Called before adjustTime, determine if DCO is readable during adjustTime */
typedef struct {
    Sint32T adjustment;
} zl303xx_AdjustTimeDCOReadableInDataS;
typedef struct {
    zl303xx_BooleanE dcoReadable;
} zl303xx_AdjustTimeDCOReadableOutDataS;

/* Action for electrical reference switching (Initial call) */
typedef struct {
    Sint32T refId;
    Sint32T syncId;
} zl303xx_SetActiveElecActionsInitialInDataS;

/* Action for hitless electrical reference switching (Stage 1, after first delay, RefSync and 1pps only) */
typedef struct {
    Uint32T refId;
} zl303xx_SetActiveElecActionsStage1InDataS;

/* Action for hitless electrical reference switching (Stage 2, after second delay, RefSync and 1pps only) */
typedef struct {
    Uint32T refId;
} zl303xx_SetActiveElecActionsStage2InDataS;

/* Get/Set 77X NCOAssist enable/disable */
typedef struct {
    zl303xx_BooleanE enable;
} zl303xx_SetNCOAssistEnableInDataS;
typedef struct {
    zl303xx_BooleanE enable;
} zl303xx_GetNCOAssistEnableOutDataS;

/* Get 77X NCOAssist ParamsS */
typedef struct {
    void *NCOAssistParams;
} zl303xx_GetNCOAssistParamsSOutDataS;

/* Get 77X NCOAssist pair freq offset */
typedef struct {
    Sint32T memPart;
} zl303xx_GetNCOAssistPairFreqOffsetInDataS;
typedef struct {
    Sint32T freqOffsetInPpt;
} zl303xx_GetNCOAssistPairFreqOffsetOutDataS;

/* Get 77X NCOAssist pair HW lock status data */
typedef struct {
    Sint32T lockStatus;
} zl303xx_GetNCOAssistPairHWLockStatusOutDataS;

/* Get NCO-assist sync output enabled status data */
typedef struct {
    Sint32T status;
} zl303xx_GetNCOAssistSyncInputEnOutDataS;

/* Get NCO-assist out-of-range status data */
typedef struct {
    Sint32T status;
} zl303xx_GetNCOAssistOutOfRangeOutDataS;

/* Get NCO-assist manual holdover status data */
typedef struct {
    Sint32T status;
} zl303xx_GetNCOAssistManualHoldoverOutDataS;

/* Get NCO-assist manual freerun status data */
typedef struct {
    Sint32T status;
} zl303xx_GetNCOAssistManualFreerunOutDataS;

/* Get 77X modified (for resolution) step time value */
typedef struct {
    Sint32T stepTimeValueIn;
} zl303xx_GetModifiedStepTimeValueInDataS;
typedef struct {
    Sint32T stepTimeValueOut;
} zl303xx_GetModifiedStepTimeValueOutDataS;

/* Get output type that drives the timestamper */
typedef struct {
    zl303xx_BooleanE b;
} zl303xx_GetIsOutputTypeThatDrivesTimeStamperHP;



typedef struct {
    zl303xx_DpllDriverMsgTypesE dpllMsgType;
    union {
        zl303xx_SetFreqInDataS setFreq;
        zl303xx_GetFreqInDataS getFreq;
        zl303xx_SetTimeInDataS setTime;
        zl303xx_StepTimeInDataS stepTime;
        zl303xx_JumpActiveCGUInDataS jumpActiveCGU;
        zl303xx_DeviceInitInDataS deviceInit;
        zl303xx_SetCurrRefInDataS setCurrRef;
        zl303xx_InputPhaseErrorWriteInDataS inputPhaseErrorWrite;
        zl303xx_SetTODDoneFuncInDataS setTODDoneFunc;
        zl303xx_ConfirmHwCntrlInDataS confirmHwCntrl;
        zl303xx_SetAFBDIVInDataS setAFBDIV;
        zl303xx_AdjustTimeInDataS adjustTime;
        zl303xx_AdjustTimeDCOReadableInDataS adjustTimeDCOReadable;
        zl303xx_SetActiveElecActionsInitialInDataS setActiveElecActionsInitial;
        zl303xx_SetActiveElecActionsStage1InDataS setActiveElecActionsStage1;
        zl303xx_SetActiveElecActionsStage2InDataS setActiveElecActionsStage2;
        zl303xx_SetNCOAssistEnableInDataS NCOAssistEnable;
        zl303xx_GetNCOAssistPairFreqOffsetInDataS getNCOAssistPairFreqOffset;
        zl303xx_GetModifiedStepTimeValueInDataS getModifiedStepTimeValue;
    } d;
} zl303xx_DriverMsgInDataS;

typedef struct {
    zl303xx_DpllDriverMsgTypesE dpllMsgType;
    union {
        zl303xx_GetFreqOutDataS getFreq;
        zl303xx_GetHWLockStatusOutDataS getHWLockStatus;
        zl303xx_GetHWManualFreerunOutDataS getHWManualFreerun;
        zl303xx_GetHWManualHoldoverOutDataS getHWManualHoldover;
        zl303xx_GetHWSyncInputEnOutDataS getHWSyncInputEn;
        zl303xx_GetHWOutOfRangeOutDataS getHWOutOfRange;
        zl303xx_GetCurrRefOutDataS getCurrRef;
        zl303xx_InputPhaseErrorWriteCtrlReadyOutDataS inputPhaseErrorWriteCtrlReady;
        zl303xx_ConfirmHwCntrlOutDataS confirmHwCntrl;
        zl303xx_GetDeviceInfoOutDataS getDeviceInfo;
        zl303xx_GetStepTimeCurrMaxStepSizeOutDataS getStepTimeCurrMaxStepSize;
        zl303xx_DetermineMaxStepSizePerAdjustmentOutDataS determineMaxStepSizePerAdjustment;
        zl303xx_AdjustTimeDCOReadableOutDataS adjustTimeDCOReadable;
        zl303xx_GetNCOAssistEnableOutDataS NCOAssistEnable;
        zl303xx_GetNCOAssistParamsSOutDataS getNCOAssistParams;
        zl303xx_GetNCOAssistPairFreqOffsetOutDataS getNCOAssistPairFreqOffset;
        zl303xx_GetNCOAssistPairHWLockStatusOutDataS getNCOAssistPairHWLockStatus;
        zl303xx_GetNCOAssistSyncInputEnOutDataS getNCOAssistSyncInputEn;
        zl303xx_GetNCOAssistOutOfRangeOutDataS getNCOAssistOutOfRange;
        zl303xx_GetNCOAssistManualHoldoverOutDataS getNCOAssistManualHoldover;
        zl303xx_GetNCOAssistManualFreerunOutDataS getNCOAssistManualFreerun;
        zl303xx_GetModifiedStepTimeValueOutDataS getModifiedStepTimeValue;
        zl303xx_GetIsOutputTypeThatDrivesTimeStamperHP isOutputTypeThatDrivesTimeStamperHP;
    } d;
} zl303xx_DriverMsgOutDataS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/* Global data used for device identification */
extern zl303xx_DevTypeE       globalDevType;


/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

/* Device identification routines */
zl303xx_DevTypeE zl303xx_GetDefaultDeviceType(void);
unsigned int zl303xx_SetDefaultDeviceType(zl303xx_DevTypeE f);



#if defined __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
