/*
 * @file    i_o-ran-processing-element.h
 */
 
#ifndef _H_i_o_ran_processing_element
#define _H_i_o_ran_processing_element


/*!
 * @addtogroup  SAA_MOD_O_RAN_PROCESSING_ELEMENT
 * @brief       SAA o-ran-processing-element
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <string.h>

#include "f_mpsw_msg_o-ran_processing_element_def.h"
#include "f_mpsw_msg_o-ran_processing_element_typ.h"

#include "f_saa_com_init.h"
/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define i_mod_o_ran_processing                  "o-ran-processing-element"                                          /* module_name */
#define i_para_o_ran_processing_element_vlanid  "processing-elements.ru-elements.transport-flow.eth-flow.vlan-id"  /* parameter_name */

#define i_o_ran_processing_element_N_name_fheth0 (const xmlChar *)"fheth0"       /* interfase_name */
#define i_o_ran_processing_element_N_name_fheth1 (const xmlChar *)"fheth1"       /* interfase_name */
#define D_PROCESSING_ELEMENT_FHETH_PORT0     0                                  /* 物理ポート番号:0 */
#define D_PROCESSING_ELEMENT_FHETH_PORT1     1                                  /* 物理ポート番号:1 */

/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/

/* struct of name & interface_name */
typedef struct allocate_name_t_{
    xmlChar name[D_PARAM_NUM_MAX];  /* CU Edit */
}allocate_name_t;
extern allocate_name_t  allocate_name[2];  /* ru-elements/name */
extern pthread_mutex_t mutex_for_allocate_name;

typedef struct process_send_msi_t_{
    bool        msi_send_flg;
    UCHAR       ru_mac_address[6];                  /* RU-MACアドレス  */
    UINT16      vlan_id;                            /* vlan_id  */
    UCHAR       o_du_mac_address[6];               /* CU-MACアドレス  */
}process_send_msi_t;
extern process_send_msi_t  process_send_msi[2];

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/* vlan_idのvalidate関数 */
extern status_t i_o_ran_processing_element_processing_elements_ru_elements_transport_flow_eth_flow_vlan_id_validate (
    const xmlChar *k_processing_elements_ru_elements_name,
    const xmlChar *vlan_id );

/* ru_mac_address Edit値保持関数 */
extern status_t i_o_ran_processing_element_processing_elements_ru_elements_transport_flow_eth_flow_ru_mac_address_validate (
    const xmlChar *k_processing_elements_ru_elements_name,
    const xmlChar *ru_mac_address );

/* o_du_mac_address Edit値保持関数 */
extern status_t i_o_ran_processing_element_processing_elements_ru_elements_transport_flow_eth_flow_o_du_mac_address_validate (
    const xmlChar *k_processing_elements_ru_elements_name,
    const xmlChar *o_du_mac_address );

/* MSI送信関数 */
extern status_t i_o_ran_processing_element_processing_elements_send_msi (
    const xmlChar *k_processing_elements_ru_elements_name);

/* processing-elements/ru-elements/name アクセス関数 */
extern void i_o_ran_processing_element_processing_elements_ru_elements_name_give(UINT num, xmlChar *data);
#endif
