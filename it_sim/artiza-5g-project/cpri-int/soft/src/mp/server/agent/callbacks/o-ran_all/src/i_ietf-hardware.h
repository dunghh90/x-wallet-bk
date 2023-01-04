/*
 * @file    i_ietf-hardware.h
 */

#ifndef _H_i_ietf_hardware
#define _H_i_ietf_hardware


/*!
 * @addtogroup  SAA_MOD_IETF_HARDWARE
 * @brief       SAA ietf-hardware
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "f_saa_com_init.h"
#include "f_mpsw_msg_ietf_hardware_def.h"
#include "f_mpsw_msg_ietf_hardware_typ.h"
/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define i_mod_ietf_hardware                 "ietf-hardware"                     /* module_name    */
#define i_para_ietf_hardware_description0   "hardware.component.0.description"  /* parameter_name */
#define i_para_ietf_hardware_description1   "hardware.component.1.description"  /* parameter_name */
#define i_para_ietf_hardware_description2   "hardware.component.2.description"  /* parameter_name */
#define i_para_ietf_hardware_physical_index0   "hardware.component.0.physical_index"  /* parameter_name */
#define i_para_ietf_hardware_physical_index1   "hardware.component.1.physical_index"  /* parameter_name */
#define i_para_ietf_hardware_physical_index2   "hardware.component.2.physical_index"  /* parameter_name */


#define i_ietf_hardware_N_name_sub6_37  (const xmlChar *)"37GLLA-SDU" /* name */
#define i_ietf_hardware_N_name_sub6_45  (const xmlChar *)"45GLLA-SDU" /* name */
#define i_ietf_hardware_N_name_mmw_28   (const xmlChar *)"280GLLA-SDU" /* name */
#define i_ietf_hardware_N_name_fhport0  (const xmlChar *)"hw-fhport0" /* name */
#define i_ietf_hardware_N_name_fhport1  (const xmlChar *)"hw-fhport1" /* name */

#define i_ietf_hardware_N_oper_state_unknown            "unknown"    /* oper_status */
#define i_ietf_hardware_N_oper_state_disabled           "disabled"   /* oper_status */
#define i_ietf_hardware_N_oper_state_enabled            "enabled"    /* oper_status */
#define i_ietf_hardware_N_oper_state_testing            "testing"    /* oper_status */

#define i_ietf_hardware_N_usage_state_unknown           "unknown"   /* usage_status */
#define i_ietf_hardware_N_usage_state_idle              "idle"      /* usage_status */
#define i_ietf_hardware_N_usage_state_active            "active"    /* usage_status */
#define i_ietf_hardware_N_usage_state_busy              "busy"      /* usage_status */

#define i_ietf_hardware_N_alarm_state_unknown           "unknown"          /* alarm_status */
#define i_ietf_hardware_N_alarm_state_idle              "under-repair"     /* alarm_status */
#define i_ietf_hardware_N_alarm_state_critical          "critical"         /* alarm_status */
#define i_ietf_hardware_N_alarm_state_major             "major"            /* alarm_status */
#define i_ietf_hardware_N_alarm_state_minor             "minor"            /* alarm_status */
#define i_ietf_hardware_N_alarm_state_warning           "warning"          /* alarm_status */
#define i_ietf_hardware_N_alarm_state_indeterminate     "indeterminate"    /* alarm_status */

#define i_ietf_hardware_N_admin_state_unknown       "unknown"           /* admin_state */
#define i_ietf_hardware_N_admin_state_locked        "locked"            /* admin_state */
#define i_ietf_hardware_N_admin_state_shutdown      "shutting-down"     /* admin_state */
#define i_ietf_hardware_N_admin_state_unlocked      "unlocked"          /* admin_state */

#define i_o_ran_hardware_N_availability_state_unknown       "UNKNOWN"                                /* availability_status */
#define i_o_ran_hardware_N_availability_state_normal        "NORMAL"                                 /* availability_status */
#define i_o_ran_hardware_N_availability_state_degraded      "DEGRADED"                               /* availability_status */
#define i_o_ran_hardware_N_availability_state_faulty        "FAULTY"                                 /* availability_status */

#define i_ietf_hardware_N_class_o_ran_radio  (const xmlChar *)"O-RAN-RADIO"   /* class */

#define D_IETF_HARDWARE_DB_SEND_MAX_LEN   512           /* dbapi送信時の最大文字列長 */
/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/

/* struct of name & interface_name */
typedef struct allocate_hardware_name_t_{
    const xmlChar *name;
}allocate_hardware_name_t;

typedef struct ietf_hardware_t_{
    xmlChar hardware_name[D_PARAM_NUM_MAX];
    E_MPSW_ADMIN_STATE  present_admin_state;
}ietf_hardware_t;

/* hardware_component_name(O-RAN-RADIO) */
extern ietf_hardware_t ietf_hardware_state;  /* /hw:hardware/component/name */
extern pthread_mutex_t mutex_for_hardware_name;
/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/* MSI_IETF_HARDWARE_GET送信関数 */
extern status_t i_ietf_hardware_send_msi (void *get_data);

/* MSI_IETF_HARDWARE_STATE_GET送信関数 */
extern status_t i_ietf_hardware_state_send_msi (void *get_data);

/* MSI_IETF_HARDWARE_STATE_EDIT送信関数 */
extern status_t i_ietf_hardware_admin_state_edit(const xmlChar *newval_val);

/* description取得関数 */
extern status_t i_ietf_hardware_hardware_component_description_get (const xmlChar *k_hardware_component_name, xmlChar **v_description);
extern status_t i_ietf_hardware_hardware_component_physical_index_get (const xmlChar *k_hardware_component_name, xmlChar **v_physical_index);

/* hardware/component/nameアクセス関数 */
extern void i_ietf_hardware_name_give(xmlChar *data);

/* DB更新関数 */
extern void i_ietf_hardware_send_edit_config (xmlChar * hardware_name, char * admin_state);

/* 初期化処理 */

extern void i_ietf_hardware_init2 (void);
extern void i_ietf_hardware_init (void);

#endif
