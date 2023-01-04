/*
 * @file    i_o-ran-hardware.h
 */

#ifndef _H_i_o_ran_hardware
#define _H_i_o_ran_hardware


/*!
 * @addtogroup  SAA_MOD_O_RAN_HARDWARE
 * @brief       SAA o-ran-hardware
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <stdlib.h>
#include <string.h>

/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define i_mod_o_ran_hardware                 "o-ran-hardware"                                           /* module_name    */
#define i_para_o_ran_hardware_model_name     "/hw:hardware/hw:component.label-content.model-name"      /* parameter_name */
#define i_para_o_ran_hardware_serial_num     "/hw:hardware/hw:component.label-content.serial-number"   /* parameter_name */

#define i_o_ran_hardware_N_boolean_state_name               (const xmlChar *)"true"                  /* boolean */
#define i_o_ran_hardware_N_power_state_awake                (const xmlChar *)"AWAKE"                 /* AWAKE */
#define i_o_ran_hardware_N_power_state_sleeping             (const xmlChar *)"SLEEPING"              /* SLEEPING */

/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/

/* struct of model-name & serial-number */
typedef struct label_content_t_{
    boolean model_name;
    boolean serial_number;
}label_content_t;

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/* MSI_O_RAN_HARDWARE_GET送信関数 */
extern status_t i_o_ran_hardware_send_msi (void *get_data);

/* label_content取得関数 */
extern status_t i_o_ran_hardware_hardware_component_label_content_get (label_content_t *get_data);

/* MSI_O_RAN_HARDWARE_EDIT送信関数 */
extern status_t i_o_ran_hardware_send_msi_edit (boolean energy_saving_enabled);

#endif
