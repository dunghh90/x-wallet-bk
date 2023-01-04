/*
 * @file    i_o-ran-performance-management.h
 */

#ifndef _H_i_o_ran_performance_management
#define _H_i_o_ran_performance_management


/*!
 * @addtogroup  SAA_MOD_O_RAN_PERFORMANCE_MANAGEMENT
 * @brief       SAA o-ran-performance-management
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>
#include <pthread.h>

#include "agt_util.h"

#include "f_mpsw_msg_o-ran_performance_management_def.h"
#include "f_mpsw_msg_o-ran_performance_management_typ.h"
/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define SAA_PFM_MNG_MAX_OBJ                6        /* 最大モジュール数     */
#define SAA_PFM_MNG_MAX_LEN                11       /* object名の最大文字数 */

#define SAA_PFM_MNG_RX_MEA_INTERVAL_FLG    1        /* 0x01 rx_measurement_interval変更フラグ */
#define SAA_PFM_MNG_NTF_INTERVAL_FLG       2        /* 0x02 notification_interval変更フラグ */
#define SAA_PFM_MNG_RX_ACT_FLG             4        /* 0x04 active/deactive変更フラグ  */
#define SAA_PFM_MNG_RU_TRS_FLG             8        /* 0x08 RU/TRANSPORT変更フラグ */

#define SAA_PFM_MNG_MEA_DEACTICVE          0        /* deactive */
#define SAA_PFM_MNG_MEA_ACTIVE             1        /* active */

#define SAA_PFM_MNG_MEA_NOSET              0        /* 未設定 初期値 */
#define SAA_PFM_MNG_MEA_RU                 1        /* RU */
#define SAA_PFM_MNG_MEA_TRANSPORT          2        /* TRANCPORT */


#define SAA_PFM_MNG_OBJECT_ALL             "ALL"  /* ワイルドカードobject名 */
#define SAA_PFM_MNG_HARD_MOD_NAME          0    /* "hardware/component/name" 要求 */
#define SAA_PFM_MNG_PROELE_MOD_NAME0       1    /* "processing-elements/ru-elements/name0" 要求 */
#define SAA_PFM_MNG_PROELE_MOD_NAME1       2    /* "processing-elements/ru-elements/name1" 要求 */

#define SAA_PFM_MNG_NO_SUPPORT_VAL         0    /* measurement-intervalの最低値 */

/********************************************************************************************************************/
/*  typedef                                                                                                        */
/********************************************************************************************************************/
extern const char object_name[SAA_PFM_MNG_MAX_OBJ][SAA_PFM_MNG_MAX_LEN];

/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/
extern UINT tr_ru_data[SAA_PFM_MNG_MAX_OBJ];    /* RU/TRANSPORT判別用テーブル */
extern pthread_mutex_t mutex_for_tr_ru_data;
/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/
/* init */
extern status_t i_o_ran_pfm_mng_init(void);

/* validate */
extern status_t i_pfm_mng_rx_window_active_validate(const xmlChar *name, bool newval_val);
extern status_t i_pfm_mng_rx_window_active_check(const xmlChar *name, UINT *obj_num, UINT16 *act_flg);

/* commit */
extern status_t i_pfm_mng_rx_window_edit(const xmlChar *name, UINT16 newval_val, bool newval_bool, UINT flg);
extern status_t i_pfm_mng_rx_window_act_flag_commit(void);

/* msi */
extern status_t i_pfm_mng_rx_window_send_edit_msi(T_MPSW_MSI_O_RAN_PFM_MNG_EDIT_REQ *msg_data);
extern status_t i_pfm_mng_rx_window_send_get_msi(void *get_data);  /* GET_REQ/GET_CFM/GET_REJ */

/* get */
extern status_t i_pfm_mng_get_other_module(UINT name,xmlChar *param);
extern int i_pfm_mng_get_module_num(void);

/* NTF_HDR */
extern void i_o_ran_performance_management_measurement_result_stats_handler(void *data, uint32_t size);
extern T_MPSW_MSI_O_RAN_PFM_MNG_NTF_IND *i_o_ran_performance_management_get_notification_data(void);

#endif
