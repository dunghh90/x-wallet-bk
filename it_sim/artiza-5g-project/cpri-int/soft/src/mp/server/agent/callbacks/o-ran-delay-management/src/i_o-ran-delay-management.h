/*
 * @file    i_o-ran-delay-management.h
 */

#ifndef _H_i_o_ran_delay_management
#define _H_i_o_ran_delay_management


/*!
 * @addtogroup  SAA_MOD_O_RAN_DELAY_MANAGEMENT
 * @brief       SAA o-ran-delay-management
 * @note        
 * @{
 */


/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "u_o-ran-delay-management.h"
#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"


/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define i_mod_o_ran_delay_management                               "o-ran-delay-management"


/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/


/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/
extern status_t i_o_ran_delay_management_delay_management_bandwidth_scs_delay_state_ru_delay_profile_get_para(
    uint32 k_delay_management_bandwidth_scs_delay_state_bandwidth,
    uint32 k_delay_management_bandwidth_scs_delay_state_subcarrier_spacing,
    const xmlChar *target_para_name,
    xmlChar **para_temp);


#endif
