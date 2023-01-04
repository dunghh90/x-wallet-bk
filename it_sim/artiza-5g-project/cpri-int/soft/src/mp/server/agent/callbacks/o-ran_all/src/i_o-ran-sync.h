/*
 * @file    i_o-ran-sync.h
 */
 
#ifndef _H_i_o_ran_sync
#define _H_i_o_ran_sync


/*!
 * @addtogroup  SAA_MOD_O_RAN_SYNC
 * @brief       SAA o-ran-sync
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <string.h>
#include <xmlstring.h>

#include "agt.h"
#include "agt_cb.h"
#include "dlq.h"
#include "ncxtypes.h"
#include "op.h"
#include "rpc.h"
#include "ses.h"
#include "status.h"
#include "val.h"

#include "y_o-ran-sync.h"

#include "f_mpsw_msg_o-ran_sync_def.h"
#include "f_mpsw_msg_o-ran_sync_typ.h"

#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
#include "f_saa_ntf_lib.h"
/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define i_o_ran_sync_domain_number_param_name    (const char *)"sync.ptp-config.domain-number"                           /* parameter_name */
#define i_o_ran_sync_clock_classes_param_name    (const char *)"sync.ptp-config.accepted-clock-classes.0.clock-classes"  /* parameter_name */
#define i_o_ran_sync_reporting_period_param_name (const char *)"sync.ptp-status.reporting-period"                        /* parameter_name */

#define D_O_RAN_SYNC_INT16_INDEFINITE        0x7fff      /* 不定値(int16) */
#define D_O_RAN_SYNC_UINT16_INDEFINITE       0xffffU     /* 不定値(uint16) */
#define D_O_RAN_SYNC_UINT32_INDEFINITE       0xffffffffU /* 不定値(uint32) */
#define SAA_SYNC_MAX_ITEM_NUM               2           /* supported_reference_types_item(PTP/SYNCE) */
    
#define i_mod_o_ran_sync                                                     "o-ran-sync"
#define i_para_o_ran_sync_domain_number_param_name                           "sync.ptp-config.domain-number"                             /* parameter_name */
#define i_para_o_ran_sync_clock_classes_param_name                           "sync.ptp-config.accepted-clock-classes.0.clock-classes"    /* parameter_name */
#define i_para_o_ran_sync_ptp_reporting_period_param_name                    "sync.ptp-status.reporting-period"                          /* parameter_name */
#define i_para_o_ran_sync_synce_reporting_period_param_name                  "sync.synce-status.reporting-period"                        /* parameter_name */
#define i_para_o_ran_sync_capability_sync_t_tsc_param_name                   "sync.sync-capability.sync-t-tsc"                           /* parameter_name */
#define i_para_o_ran_sync_status_supported_reference_types                   "sync.sync-status.supported-reference-types."
#define i_para_o_ran_sync_synce_config_ssm_timeout                           "sync.synce-config.ssm-timeout"
#define i_para_o_ran_sync_item                                               ".item"

#define i_o_ran_sync_N_state_freerun                 "FREERUN"           /* sync_status */
#define i_o_ran_sync_N_state_locked                  "LOCKED"            /* sync_status */
#define i_o_ran_sync_N_state_holdover                "HOLDOVER"          /* sync_status */
#define i_o_ran_sync_N_state_lock_unlocked           "UNLOCKED"          /* ptp_lock_state */
#define i_o_ran_sync_N_state_lock_locked             "LOCKED"            /* ptp_lock_state */
#define i_o_ran_sync_N_state_synce_lock_unlocked     "UNLOCKED"          /* lock_state */
#define i_o_ran_sync_N_state_synce_lock_locked       "LOCKED"            /* lock_state */
#define i_o_ran_sync_N_state_ptp_disable             "DISABLED"          /* ptp_state */
#define i_o_ran_sync_N_state_ptp_parent              "PARENT"            /* ptp_state */
#define i_o_ran_sync_N_state_ptp_ok                  "OK"                /* ptp_state */
#define i_o_ran_sync_N_state_ptp_nok                 "NOK"               /* ptp_state */
#define i_o_ran_sync_N_state_synce_disable           "DISABLED"          /* sync_state */
#define i_o_ran_sync_N_state_synce_parent            "PARENT"            /* sync_state */
#define i_o_ran_sync_N_state_synce_ok                "OK"                /* sync_state */
#define i_o_ran_sync_N_state_synce_nok               "NOK"               /* sync_state */
#define i_o_ran_sync_N_state_ptp_true                "true"              /* ptp_status_sources */
#define i_o_ran_sync_N_state_ptp_false               "false"             /* ptp_status_sources */
#define i_o_ran_sync_N_forwardable                   "FORWARDABLE"       /* multicast_mac_address */
#define i_o_ran_sync_N_nonforwardable                "NONFORWARDABLE"    /* multicast_mac_address */
#define i_o_ran_sync_N_ssm_prc                       "PRC"               /* acceptance-list-of-ssm */
#define i_o_ran_sync_N_ssm_prs                       "PRS"               /* acceptance-list-of-ssm */
#define i_o_ran_sync_N_ssm_ssu_a                     "SSU_A"             /* acceptance-list-of-ssm */
#define i_o_ran_sync_N_ssm_ssu_b                     "SSU_B"             /* acceptance-list-of-ssm */
#define i_o_ran_sync_N_ssm_st2                       "ST2"               /* acceptance-list-of-ssm */
#define i_o_ran_sync_N_ssm_st3                       "ST3"               /* acceptance-list-of-ssm */
#define i_o_ran_sync_N_ssm_st3e                      "ST3E"              /* acceptance-list-of-ssm */
#define i_o_ran_sync_N_ssm_eec1                      "EEC1"              /* acceptance-list-of-ssm */
#define i_o_ran_sync_N_ssm_eec2                      "EEC2"              /* acceptance-list-of-ssm */
#define i_o_ran_sync_N_ssm_dnu                       "DNU"               /* acceptance-list-of-ssm */
#define i_o_ran_sync_N_ssm_none                      "NONE"               /* acceptance-list-of-ssm */

/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/

/* ptp_config_domain_numberのValidate関数 */
extern status_t i_o_ran_sync_sync_ptp_config_domain_number_validate (
    val_value_t *newval,
    val_value_t *curval);

/* ptp_config_accepted_clock_classesのValidate関数 */
extern status_t i_o_ran_sync_sync_ptp_config_accepted_clock_classes_validate (
    uint8 k_sync_ptp_config_accepted_clock_classes_clock_classes);

/* ptp_status_reporting_periodのValidate関数 */
extern status_t i_o_ran_sync_sync_ptp_status_reporting_period_validate (
    val_value_t *newval,
    val_value_t *curval);

/* synce_status_reporting_periodのValidate関数 */
extern status_t i_o_ran_sync_sync_synce_status_reporting_period_validate (
    val_value_t *newval,
    val_value_t *curval);

/* sync_capabilityのget関数 */
extern status_t i_o_ran_sync_sync_sync_capability_get (
    xmlChar **sync_capability);

/* supported_reference_types_itemのkey番号のget関数 */
extern status_t i_o_ran_sync_sync_sync_status_supported_reference_types_get_key_num (
    const xmlChar *k_sync_sync_status_supported_reference_types_item,
    uint32 *curindex);

/* supported_reference_types_itemの先頭keyのget関数 */
extern status_t i_o_ran_sync_sync_sync_status_supported_reference_types_get_first_key (
    xmlChar **key_name_temp);

/* supported_reference_types_itemの指定keyのget関数 */
extern status_t i_o_ran_sync_sync_sync_status_supported_reference_types_get_target_key (
    uint32 curindex,
    xmlChar **key_name_temp);

/* partial_timing_supportedのvalidate関数 */
extern status_t i_o_ran_sync_sync_ptp_status_partial_timing_supported_validate (
    boolean *newval);

/* sync_synce_config_ssm_timeoutのValidate関数 */
extern status_t i_o_ran_sync_sync_synce_config_ssm_timeout_validate (
    val_value_t *newval,
    val_value_t *curval);


/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/* EDIT値初期化関数 */
extern void i_o_ran_sync_edit_table_init(T_MPSW_MSI_O_RAN_SYNC_EDIT_REQ *edit_data);

/* MSI送信関数 */
extern status_t i_o_ran_sync_edit_req_send_msi (
    T_MPSW_MSI_O_RAN_SYNC_EDIT_REQ  *edit_data);

/* GET MSI送信関数 */
extern status_t i_o_ran_sync_get_send_msi (
    void  *get_data);

/* notificationハンドラ */
extern void i_o_ran_sync_synchronization_state_change_handler(void *data, uint32_t size);
extern void i_o_ran_sync_ptp_state_change_send_handler(void *data, uint32_t size);
extern void i_o_ran_sync_synce_state_change_handler(void *data, uint32_t size);

#endif
