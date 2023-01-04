/*
 * @file    i_o-ran-dhcp.h
 */

#ifndef _H_i_o_ran_dhcp
#define _H_i_o_ran_dhcp


/*!
 * @addtogroup  SAA_MOD_O_RAN_DHCP
 * @brief       SAA o-ran-dhcp
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <string.h>
#include "u_o-ran-dhcp.h"
#include "i_ietf-interfaces.h"

#include "f_mpsw_msg_o-ran_dhcp_def.h"
#include "f_mpsw_msg_o-ran_dhcp_typ.h"

#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define i_mod_o_ran_dhcp                     "o-ran-dhcp"                                 /* module_name    */
#define i_para_o_ran_dhcp_mtu                "dhcp.interfaces.0.dhcpv4.interface-mtu"    /* parameter_name */
#define i_para_o_ran_dhcp_vendor_class_data  "dhcp.m-plane-dhcp.vendor-class-data"       /* parameter_name */

#define D_O_RAN_DHCP_1BYTE_SHIFT     8       /* 1byteシフト演算用 */
#define D_O_RAN_DHCP_2BYTE_SHIFT     16      /* 2byteシフト演算用 */
#define D_O_RAN_DHCP_3BYTE_SHIFT     24      /* 3byteシフト演算用 */
#define D_O_RAN_DHCP_1BYTE_MASK      0xFF    /* 1byteマスク処理用 */

/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/


/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/
/* MSI_O_RAN_DHCP_GET_REQ送信関数 */
extern status_t i_o_ran_dhcp_send_msi (void *get_data);

/* init data取得関数 */
extern status_t i_o_ran_dhcp_get_init_data (char *init_param_name, xmlChar **v_init_data);

#endif
