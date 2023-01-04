/*
 * @file    i_o-ran-externalio.h
 */

#ifndef _H_i_o_ran_externalio
#define _H_i_o_ran_externalio


/*!
 * @addtogroup  SAA_MOD_O_RAN_EXTERNALIO
 * @brief       SAA o-ran-externalio
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <string.h>
#include <xmlstring.h>

#include "f_mpsw_msg_o-ran_externalio_def.h"
#include "f_mpsw_msg_o-ran_externalio_typ.h"

/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define SAA_EXIO_OPEN                           0
#define SAA_EXIO_CLOSED                         1
#define i_o_ran_externalio_N_input_name          (const xmlChar *)"External-Input-0"
#define i_o_ran_externalio_N_input_port_in       (const xmlChar *)"0"
#define i_o_ran_externalio_N_output_name         (const xmlChar *)"External-Output-0"
#define i_o_ran_externalio_N_port_out            (const xmlChar *)"0"

/********************************************************************************************************************/
/*  typedef                                                                                                        */
/********************************************************************************************************************/

/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/
extern status_t i_o_ran_externalio_send_msi_get (
    T_MPSW_MSI_O_RAN_EXTERNALIO_GET_CFM  *get_data);

extern status_t i_o_ran_externalio_send_msi_edit (
    boolean  newval_val);

extern status_t i_o_ran_externalio_conv_line_state_to_bool(
    uint32 line_state,
    boolean *line_state_bool);

extern void i_o_ran_externalio_external_input_change_handler(
    void *data,
    uint32_t size);

extern T_MPSW_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND *
    i_o_ran_externalio_get_notification_data(void);

extern void i_o_ran_externalio_init2 (void);

#endif
