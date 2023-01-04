/*
 * @file    i_o-ran-fm.h
 */

#ifndef _H_i_o_ran_fm
#define _H_i_o_ran_fm


/*!
 * @addtogroup  SAA_MOD_O_RAN_FM
 * @brief       SAA o-ran-fm
 * @note        
 * @{
 */

/* include file */

#include <string.h>
#include <pthread.h>
#include "agt_util.h"

#include "f_saa_com_init.h"
#include "i_o-ran-uplane-conf.h"


/* define */

#define SAA_FM_MAX_ALARM_NUM        33
#define SAA_FM_DEF_VAL_FLT_ID       0xFFFF

#define SAA_FM_TIME_YEAR_OFFSET          1900
#define SAA_FM_TIME_MON_OFFSET           1
    
#define i_mod_o_ran_fm                          "o-ran-fm"
#define i_para_o_ran_fm_fault_source_llsdu      "fm_llsdu"
#define i_para_o_ran_fm_fault_source_port0      "fm_port0"
#define i_para_o_ran_fm_fault_source_port1      "fm_port1"
#define i_para_o_ran_fm_fault_source_fan1       "fm_fan1"
#define i_para_o_ran_fm_fault_source_fan2       "fm_fan2"
#define i_para_o_ran_fm_fault_source_fan3       "fm_fan3"
#define i_para_o_ran_fm_fault_source_external   "fm_external"
#define i_para_o_ran_fm_severity_critical       "CRITICAL"
#define i_para_o_ran_fm_severity_major          "MAJOR"
#define i_para_o_ran_fm_severity_minor          "MINOR"

/* typedef*/

#define SAA_FM_MAX_NUM_AFFECTED_OBJ 64      /* affected-objectの最大数 */
typedef struct affected_obj_info_t_{
    xmlChar             name[D_PARAM_NUM_MAX];
}affected_obj_info_t;

typedef struct alarm_info_t_{
    uint16              fault_id;
    xmlChar             fault_source[D_PARAM_NUM_MAX];
    unsigned char       num_of_affected_obj;
    affected_obj_info_t affected_obj[SAA_FM_MAX_NUM_AFFECTED_OBJ];
    xmlChar             fault_severity[D_PARAM_NUM_MAX];
    boolean             is_cleared;
    xmlChar             fault_text[D_PARAM_NUM_MAX];
    xmlChar             event_time[D_PARAM_NUM_MAX];
}alarm_info_t;

/* variable */


/* function */

extern status_t i_o_ran_fm_init(void);
extern void i_o_ran_fm_alarm_notif_handler(void *data, uint32_t size);
extern int i_o_ran_fm_conv_msi_to_alarm_tbl(void *data, char *event_time_buff, alarm_info_t *alarm_info);
extern int i_o_ran_fm_conv_fault_source_enum_to_string(E_FAULT_SOURCE fault_source_enum, xmlChar * fault_source_str);
extern int i_o_ran_fm_conv_fault_severity_enum_to_string(E_FAULT_SEVERITY fault_severity_enum, xmlChar * fault_severity_str);
extern status_t i_o_ran_fm_get_active_alarms(alarm_info_t *alarm_info_temp);
extern status_t i_o_ran_fm_get_affected_objects(affected_obj_info_t *affected_obj_temp);
extern  alarm_info_t *i_o_ran_fm_get_notification_data(void);
extern status_t i_o_ran_fm_init2(void);

#endif
