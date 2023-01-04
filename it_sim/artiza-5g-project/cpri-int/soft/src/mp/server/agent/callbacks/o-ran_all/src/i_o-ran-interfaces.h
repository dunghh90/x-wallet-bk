/*
 * @file    i_o-ran-interfaces.h
 */

#ifndef _H_i_o_ran_interfaces
#define _H_i_o_ran_interfaces


/*!
 * @addtogroup  SAA_MOD_O_RAN_INTERFACES
 * @brief       SAA o-ran-interfaces
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <string.h>
#include "i_ietf-interfaces.h"
//#include "u_o-ran-interfaces.h"

#include "f_mpsw_msg_o-ran_interface_def.h"
#include "f_mpsw_msg_o-ran_interface_typ.h"

#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define i_mod_o_ran_interfaces                   "o-ran-interfaces"                                           /* module_name    */
#define i_para_o_ran_interfaces_maximum_speed    "/if:interfaces/if:interface.physical-int.maximum-speed"    /* parameter_name */
#define i_para_o_ran_mac_address "interfaces.interface.mac-address"

#define D_O_RAN_INTERFACE_MPLANE             0                               /* Plane種別:M-Plane  */
#define D_O_RAN_INTERFACE_UCPLANE            1                               /* Plane種別:UC-Plane */
#define D_O_RAN_INTERFACE_INDEFINITE         0xffff                          /* 不定値             */

#define D_O_RAN_INTERFACE_UPLANE             0                               /* Plane種別:U-Plane */
#define D_O_RAN_INTERFACE_CPLANE             1                               /* Plane種別:C-Plane */

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

/* vlan_taggingのvalidate */
status_t i_o_ran_interfaces_interfaces_interface_vlan_tagging_validate (boolean *newval);

/* mac_addressのvalidate */
status_t i_o_ran_interfaces_interfaces_interface_mac_address_validate (
    const xmlChar *k_interfaces_interface_name,
    const xmlChar *newval_val);

/* EDIT_m_plane_marking取得関数 */
extern status_t i_o_ran_interfaces_m_plane_marking_edit (
    const xmlChar *k_interfaces_interface_name,
    uint8 newval_val);

/* EDIT_uc_plane_marking取得関数 */
extern status_t i_o_ran_interfaces_uc_plane_marking_edit (
    const xmlChar *k_interfaces_interface_name,
    uint8 newval_val,
    uint8 plane_kind);

/* EDIT_vlan_id取得関数 */
extern status_t i_o_ran_interfaces_vlan_id_edit (
    const xmlChar *k_interfaces_interface_name,
    uint16 newval_val);

/* MSI_O_RAN_INTERFACE_GET送信関数 */
extern status_t i_o_ran_interfaces_get_send_msi (void);

/* MSI_O_RAN_INTERFACE_EDIT送信関数 */
extern status_t i_o_ran_interfaces_edit_send_msi (
    T_MPSW_MSI_O_RAN_INTERFACE_EDIT_REQ  *edit_data);

#endif
