/*
 * @file    i_o-ran-transceiver.h
 */

#ifndef _H_i_o_ran_transceiver
#define _H_i_o_ran_transceiver


/*!
 * @addtogroup  SAA_MOD_O_RAN_TRANSCEIVER
 * @brief       SAA o-ran-transceiver
 * @note        
 * @{
 */


/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <string.h>
#include "u_o-ran-transceiver.h"
#include "i_ietf-interfaces.h"
#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
#include "f_mpsw_msg_o-ran_transceiver_def.h"
#include "f_mpsw_msg_o-ran_transceiver_typ.h"


/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define SAA_TRANS_SFP_PRESENT           1
#define SAA_TRANS_PORT_NUM_0            0
#define SAA_TRANS_PORT_NUM_1            1
#define SAA_TRANS_VENDOR_ID_LEN         16
#define SAA_TRANS_VENDOR_PART_LEN       16
#define SAA_TRANS_VENDOR_REV_LEN        2
#define SAA_TRANS_SERIAL_NO_LEN         16

#define i_o_ran_transceiver_N_sfp8472_rev_undefined           (const xmlChar *)"diagnostics-undefined"
#define i_o_ran_transceiver_N_sfp8472_rev_9_3                 (const xmlChar *)"rev9.3-diagnostics"
#define i_o_ran_transceiver_N_sfp8472_rev_9_5                 (const xmlChar *)"rev9.5-diagnostics"
#define i_o_ran_transceiver_N_sfp8472_rev_10_2                (const xmlChar *)"rev10.2-diagnostics"
#define i_o_ran_transceiver_N_sfp8472_rev_11_0                (const xmlChar *)"rev11.0-diagnostics"
#define i_o_ran_transceiver_N_sfp8472_rev_11_3                (const xmlChar *)"rev11.3-diagnostics"
#define i_o_ran_transceiver_N_sfp8472_rev_11_4                (const xmlChar *)"rev11.4-diagnostics"
#define i_o_ran_transceiver_N_sfp8472_rev_12_0                (const xmlChar *)"rev12.0-diagnostics"

#define i_o_ran_transceiver_N_conn_type_unknown               (const xmlChar *)"unknown"
#define i_o_ran_transceiver_N_conn_type_sub_conn              (const xmlChar *)"subscrber-connector"
#define i_o_ran_transceiver_N_conn_type_fib_jck               (const xmlChar *)"fiber-jack"
#define i_o_ran_transceiver_N_conn_type_luc_conn              (const xmlChar *)"lucent-connector"
#define i_o_ran_transceiver_N_conn_type_mt_rj                 (const xmlChar *)"mt-rj"
#define i_o_ran_transceiver_N_conn_type_mul_opt               (const xmlChar *)"multiple-optical"
#define i_o_ran_transceiver_N_conn_type_sg                    (const xmlChar *)"sg"
#define i_o_ran_transceiver_N_conn_type_opt_pig               (const xmlChar *)"optical-pigtail"
#define i_o_ran_transceiver_N_conn_type_mfpo_1x12             (const xmlChar *)"multi-fiber-parralel-optic-1x12"
#define i_o_ran_transceiver_N_conn_type_mfpo_2x16             (const xmlChar *)"multi-fiber-parralel-optic-2x16"
#define i_o_ran_transceiver_N_conn_type_hss_2                 (const xmlChar *)"hssdc_2"
#define i_o_ran_transceiver_N_conn_type_copp_pig              (const xmlChar *)"copper-pigtail"
#define i_o_ran_transceiver_N_conn_type_rj45                  (const xmlChar *)"rj45"
#define i_o_ran_transceiver_N_conn_type_no_sep_conn           (const xmlChar *)"no-separable-connector"
#define i_o_ran_transceiver_N_conn_type_mxc_2x16              (const xmlChar *)"mxc-2x16"

#define i_o_ran_transceiver_N_rx_power_type_oma               (const xmlChar *)"oma"
#define i_o_ran_transceiver_N_rx_power_type_avp               (const xmlChar *)"avp"


/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/


/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/
extern status_t i_o_ran_transceiver_send_msi (
    void  *get_data,
    const xmlChar *k_port_transceivers_port_transceiver_data_interface_name,
    uint32 *port_num);

extern void i_o_ran_transceiver_string_correction (
    xmlChar *target_str,
    xmlChar *source_str,
    unsigned int length);

#endif
