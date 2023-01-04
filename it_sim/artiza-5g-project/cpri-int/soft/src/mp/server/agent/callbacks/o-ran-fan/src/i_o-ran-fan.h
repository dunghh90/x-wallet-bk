/*
 * @file    i_o-ran-fan.h
 */

#ifndef _H_i_o_ran_fan
#define _H_i_o_ran_fan


/*!
 * @addtogroup  SAA_MOD_O_RAN_FAN
 * @brief       SAA o-ran-fan
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <string.h>
#include "u_o-ran-fan.h"

#include "f_mpsw_msg_o-ran_fan_def.h"
#include "f_mpsw_msg_o-ran_fan_typ.h"

#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
#include "f_saa_ntf_lib.h"
#include "f_saa_msi_lib.h"

#include "agt_util.h"


/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define SAA_FAN_MAX_FAN_NUM                     3
#define SAA_FAN_STATE_NORMAL                    0
#define SAA_FAN_STATE_FAULT                     1

#define i_mod_o_ran_fan                          "o-ran-fan"

/********************************************************************************************************************/
/*  typedef                                                                                                        */
/********************************************************************************************************************/

/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/
extern status_t i_o_ran_fan_send_msi_get (
    T_MPSW_MSI_O_RAN_FAN_GET_CFM  *get_data);

extern status_t i_o_ran_fan_fan_tray_fan_state_get_key_num(
    const xmlChar *key_str,
    uint32 *curindex);

extern status_t i_o_ran_fan_fan_tray_fan_state_get_first_key(
    xmlChar **key_name_temp);

extern status_t i_o_ran_fan_fan_tray_fan_state_get_target_key(
    uint32 curindex,
    xmlChar **key_name_temp);


#endif
