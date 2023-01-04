/*
 * @file    i_ietf-ip.h
 */

#ifndef _H_i_ietf_ip
#define _H_i_ietf_ip


/*!
 * @addtogroup  SAA_MOD_IETF_IP
 * @brief       SAA ietf-ip
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "u_ietf-ip.h"
#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
#include "agt_util.h"

/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define i_mod_ietf_ip                                                      "ietf-ip"
#define i_para_ietf_ip_ipv4_address_origin                                 "ietf-ip.ipv4.address.origin"
#define IP_ADD_MAX                                                         16
#define IP_ZERO                                                            0


/* get */
extern status_t i_ietf_ip_interfaces_interface_ipv4_address_origin_get(const xmlChar *k_interfaces_interface_name, const xmlChar *k_interfaces_interface_ipv4_address_ip, xmlChar **v_origin_temp);



#endif
