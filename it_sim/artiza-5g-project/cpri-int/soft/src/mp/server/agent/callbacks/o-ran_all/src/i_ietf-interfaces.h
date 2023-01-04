/*
 * @file    i_ietf-interfaces.h
 */

#ifndef _H_i_ietf_interfaces
#define _H_i_ietf_interfaces


/*!
 * @addtogroup  SAA_MOD_IETF_INTERFACES
 * @brief       SAA ietf-interfaces
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "xml_util.h"
#include "f_saa_com_init.h"
#include "f_mpsw_msg_ietf_interfaces_def.h"
#include "f_mpsw_msg_ietf_interfaces_typ.h"

#define i_ietf_interface_N_name_fheth0      (const xmlChar *)"fheth0"       /* interfase_name */
#define i_ietf_interface_N_name_m_vlan0     (const xmlChar *)"m-vlan0"      /* interfase_name */
#define i_ietf_interface_N_name_m_ip        (const xmlChar *)"mplane-ip"    /* interfase_name */

#define i_ietf_interface_N_oper_state_up                (const xmlChar *)"up"               /* oper_status */
#define i_ietf_interface_N_oper_state_down              (const xmlChar *)"down"             /* oper_status */
#define i_ietf_interface_N_oper_state_testing           (const xmlChar *)"testing"          /* oper_status */
#define i_ietf_interface_N_oper_state_unknown           (const xmlChar *)"unknown"           /* oper_status */
#define i_ietf_interface_N_oper_state_dormant           (const xmlChar *)"dormant"          /* oper_status */
#define i_ietf_interface_N_oper_state_not_present       (const xmlChar *)"not-present"      /* oper_status */
#define i_ietf_interface_N_oper_state_low_layer_down    (const xmlChar *)"lower-layer-down" /* oper_status */

#define D_IETF_INTERFACE_TYPE_O_RAN   0  /* Type種別:O_RAN-INTERFACE-TYPE */
#define D_IETF_INTERFACE_TYPE_ETHER   1  /* Type種別:ethernetCsmacd      */
#define D_IETF_INTERFACE_TYPE_L2VLAN  2  /* Type種別:l2vlan              */
#define D_IETF_INTERFACE_TYPE_IP      3  /* Type種別:ip                  */

#define D_IETF_INTERFACE_MPLANE       0  /* Plane種別:M-Plane  */
#define D_IETF_INTERFACE_UCPLANE      1  /* Plane種別:UC-Plane */
#define D_IETF_INTERFACE_ETH          2  /* Plane種別:Eth      */

#define D_IETF_INTERFACE_FHETH_PORT0        0  /* 物理ポート番号:0 */
#define D_IETF_INTERFACE_FHETH_PORT1        1  /* 物理ポート番号:1 */

#define D_IETF_INTERFACE_OPER_STATE_UP          1 /* oper-status:up  */
#define D_IETF_INTERFACE_OPER_STATE_DOWN        2 /* oper-status:down */
#define D_IETF_INTERFACE_OPER_STATE_TESTING     3 /* oper-status:testing */
#define D_IETF_INTERFACE_OPER_STATE_UNKOWN      4 /* oper-status:unkown  */
#define D_IETF_INTERFACE_OPER_STATE_DOWMANT     5 /* oper-status:dormant */
#define D_IETF_INTERFACE_OPER_STATE_NOT_PRESENT 6 /* oper-status:not-present */
#define D_IETF_INTERFACE_OPER_STATE_LOW_DOWN    7 /* oper-status:lower-layer-down */

#define D_IETF_INTERFACE_SPEED 25000000000LL

/* struct of higher-layer-if and low_layer_if */
typedef struct layer_if_t_{
    const xmlChar *layer_if[3];     /* higher-layer-if and low-layer-if */
}layer_if_t;

/* UC-Plane interfase-name list */
typedef struct uc_if_name_t_{
    xmlChar uc_name[D_PARAM_NUM_MAX];     /* interfase-name */
}uc_if_name_t;

/* interface_name */
extern uc_if_name_t uc_if_name;
extern xmlChar  m_ip_lower_layer_if[D_PARAM_NUM_MAX];


/* MSI送信関数 */
extern status_t i_ietf_interface_send_msi (const xmlChar *interface_name, void *get_data);

/* oper_state取得関数 */
extern status_t i_ietf_interface_get_oper_state (const xmlChar *interface_name, const xmlChar **oper_status);

/* speed取得関数 */
extern status_t i_ietf_interface_get_speed (const xmlChar *interface_name, uint64 *speed);

/* higher_layer_if取得関数 */
extern status_t i_ietf_interface_get_higher_layer_if (const xmlChar *interface_name, layer_if_t *get_data);

/* low_layer_if取得関数 */
extern status_t i_ietf_interface_get_low_layer_if (const xmlChar *interface_name, layer_if_t *get_data);
#endif
