/*
 * @file    i_o-ran-operations.h
 */

#ifndef _H_i_o_ran_operations
#define _H_i_o_ran_operations


/*!
 * @addtogroup  SAA_MOD_O_RAN_OPERATIONS
 * @brief       SAA o-ran-operations
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <string.h>
#include "u_o-ran-operations.h"

#include "f_mpsw_msg_o-ran_operations_def.h"
#include "f_mpsw_msg_o-ran_operations_typ.h"

#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
#include "f_saa_utc_lib.h"
/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
/* common */
#define i_mod_o_ran_operations                           "o-ran-operations"

/* saa_init_data.conf */
#define i_para_o_ran_operations_mplane_version           "operational-info.declarations.supported-mplane-version"
#define i_para_o_ran_operations_cusplane_version         "operational-info.declarations.supported-cusplane-version"
#define i_para_o_ran_operations_protocol                 "operational-info.declarations.supported-header-mechanism.protocol"
#define i_para_o_ran_operations_ecpri_supported          "operational-info.declarations.supported-header-mechanism.ecpri-concatenation-support"
#define i_para_o_ran_operations_protocol_version         "operational-info.declarations.supported-header-mechanism.protocol-version"

/* saa_validate_data.conf */
#define i_para_o_ran_operations_re_callhome_no_ssh_timer "operational-info.re-call-home-no-ssh-timer"

/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/
extern bool o_ran_operations_msi_send_flag;  /* MSI??????????????? */
extern T_MPSW_MSI_O_RAN_OPERATIONS_GET_CFM   get_cfm_save_data;   /* ?????????????????????????????? */

#define i_o_ran_operations_N_restart_cause_on            "POWER-ON"                  /* restart-cause */
#define i_o_ran_operations_N_restart_cause_watchdog      "SUPERVISION-WATCHDOG"      /* restart-cause */
#define i_o_ran_operations_N_restart_cause_restart       "MPLANE-TRIGGERED-RESTART"  /* restart-cause */
#define i_o_ran_operations_N_restart_cause_failure       "SOFTWARE-FAILURE"          /* restart-cause */
#define i_o_ran_operations_N_restart_cause_other         "OTHER-WATCHDOG-TIMER"      /* restart-cause */
#define i_o_ran_operations_N_restart_cause_unknown        "UNKNOWN"                   /* restart-cause */

#define D_O_RAN_OPERATIONS_DB_SEND_MAX_LEN              256

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/
/* supported-header-mechanism key?????????????????? */
extern status_t i_o_ran_operations_operational_info_declarations_supported_header_mechanism_keyget( xmlChar **para_temp );

/* supported-header-mechanism???????????? */
extern status_t i_o_ran_operations_operational_info_declarations_supported_header_mechanism_get( const xmlChar *target_para_name, xmlChar **para_temp );

/* declarations???????????? */
extern status_t i_o_ran_operations_operational_info_declarations_get( const xmlChar *target_para_name, xmlChar **para_temp );

/* O_RAN_OPERATION_GET_REQ???????????? */
extern status_t i_o_ran_operations_operational_info_operational_state_get( void *get_data );

/* timezone-utc-offset???????????? */
extern status_t i_o_ran_operations_operational_info_clock_timezone_utc_offset_edit( int16 timezone_utc_offset );

/* re-call-home-no-ssh-timer???????????? */
extern status_t i_o_ran_operations_operational_info_re_call_home_no_ssh_timer_validate( uint16 re_call_home_no_ssh_timer);
extern status_t i_o_ran_operations_operational_info_re_call_home_no_ssh_timer_edit( uint16 re_call_home_no_ssh_timer);

/* O_RAN_OPERATION_RESET_IND???????????? */
extern status_t i_o_ran_operations_reset_invoke( E_RESTART_CAUSE reset_cause );

/* o-ran-operations?????????????????? */
extern void i_o_ran_operations_init2( void );

#endif
