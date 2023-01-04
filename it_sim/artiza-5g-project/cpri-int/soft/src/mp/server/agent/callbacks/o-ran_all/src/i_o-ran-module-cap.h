/*
 * @file    i_o-ran-module-cap.h
 */

#ifndef _H_i_o_ran_module_cap
#define _H_i_o_ran_module_cap


/*!
 * @addtogroup  SAA_MOD_O_RAN_MODULE_CAP
 * @brief       SAA o-ran-module-cap
 * @note        
 * @{
 */


/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/

/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define SAA_MODCAP_KEY_SCS_A            1
#define SAA_MODCAP_KEY_SCS_B            2

#define i_mod_o_ran_module_cap                                  "o-ran-module-cap"

#define i_para_module_capabilities_cmp_sup                      "module-capability.ru-capabilities.format-of-iq-sample.dynamic-compression-supported"
#define i_para_module_capabilities_bit_wth_sup                  "module-capability.ru-capabilities.format-of-iq-sample.realtime-variable-bit-width-supported"
#define i_para_module_capabilities_sym_sup                      "module-capability.ru-capabilities.format-of-iq-sample.syminc-supported"

#define i_para_module_cap_compression_type                      "module-capability.ru-capabilities.format-of-iq-sample.compression-method-supported.0.compression-method.0.compression-type"
#define i_para_module_cap_bitwidth                              "module-capability.ru-capabilities.format-of-iq-sample.compression-method-supported.0.compression-method.0.bitwidth"

#define i_para_module_cap_exponent                              "module-capability.ru-capabilities.format-of-iq-sample.compression-method-supported.0.compression-method.0.compression-format.block-floating-point.exponent"

#define i_para_o_ran_module_req_ul_rbs                          "module-capability.ru-capabilities.ul-mixed-num-required-guard-rbs"
#define i_para_o_ran_module_rbs_ul                              "number-of-guard-rbs-ul"
#define i_para_o_ran_module_req_dl_rbs                          "module-capability.ru-capabilities.dl-mixed-num-required-guard-rbs"
#define i_para_o_ran_module_rbs_dl                              "number-of-guard-rbs-dl"
#define i_para_o_ran_module_scs_a                               "scs-a"
#define i_para_o_ran_module_scs_b                               "scs-b"

#define i_para_o_ran_module_cap_band_capabilities               "module-capability.band-capabilities"
#define i_para_o_ran_module_cap_band_number                     "band-number"
#define i_para_o_ran_module_cap_max_supported_frequency_dl      "max-supported-frequency-dl"
#define i_para_o_ran_module_cap_min_supported_frequency_dl      "min-supported-frequency-dl"
#define i_para_o_ran_module_cap_max_supported_bandwidth_dl      "max-supported-bandwidth-dl"
#define i_para_o_ran_module_cap_max_num_carriers_dl             "max-num-carriers-dl"
#define i_para_o_ran_module_cap_max_carrier_bandwidth_dl        "max-carrier-bandwidth-dl"
#define i_para_o_ran_module_cap_min_carrier_bandwidth_dl        "min-carrier-bandwidth-dl"
#define i_para_o_ran_module_cap_max_supported_frequency_ul      "max-supported-frequency-ul"
#define i_para_o_ran_module_cap_min_supported_frequency_ul      "min-supported-frequency-ul"
#define i_para_o_ran_module_cap_max_supported_bandwidth_ul      "max-supported-bandwidth-ul"
#define i_para_o_ran_module_cap_max_num_carriers_ul             "max-num-carriers-ul"
#define i_para_o_ran_module_cap_max_carrier_bandwidth_ul        "max-carrier-bandwidth-ul"
#define i_para_o_ran_module_cap_min_carrier_bandwidth_ul        "min-carrier-bandwidth-ul"
#define i_para_o_ran_module_cap_max_num_component_carriers      "max-num-component-carriers"
#define i_para_o_ran_module_cap_max_num_bands                   "max-num-bands"
#define i_para_o_ran_module_cap_max_num_sectors                 "max-num-sectors"
#define i_para_o_ran_module_cap_max_power_per_antenna           "max-power-per-antenna"
#define i_para_o_ran_module_cap_min_power_per_antenna           "min-power-per-antenna"
#define i_para_o_ran_module_cap_codebook_configuration_ng       "codebook-configuration_ng"
#define i_para_o_ran_module_cap_codebook_configuration_n1       "codebook-configuration_n1"
#define i_para_o_ran_module_cap_codebook_configuration_n2       "codebook-configuration_n2"

#define i_para_module_capabilities_category                     "module-capability.ru-capabilities.ru-supported-category"
#define i_para_module_capabilities_ruport                       "module-capability.ru-capabilities.number-of-ru-ports"
#define i_para_module_capabilities_max_pwr                      "module-capability.ru-capabilities.max-power-per-pa-antenna"
#define i_para_module_capabilities_min_pwr                      "module-capability.ru-capabilities.min-power-per-pa-antenna"
#define i_para_module_capabilities_slt_opt                      "module-capability.ru-capabilities.fronthaul-split-option"
#define i_para_module_capabilities_num_of_spatial_streams       "module-capability.ru-capabilities.number-of-spatial-streams"
#define i_para_module_capabilities_energy_saving                "module-capability.ru-capabilities.energy-saving-by-transmission-blanks"
#define i_para_module_capabilities_dynamic_transport            "module-capability.ru-capabilities.dynamic-transport-delay-management-supported"

/* init */
#define i_para_o_ran_module_cap_max_scs_a_num_dl                "max_scs_a_num_dl"
#define i_para_o_ran_module_cap_max_scs_b_num_dl                "max_scs_b_num_dl"
#define i_para_o_ran_module_cap_max_scs_a_num_ul                "max_scs_a_num_ul"
#define i_para_o_ran_module_cap_max_scs_b_num_ul                "max_scs_b_num_ul"
#define i_para_o_ran_module_cap_max_band_cap_num                "max_band_cap_num"


typedef enum
{
    E_MODULE_CAPA_CATEGORY = 0,     /* y_o_ran_module_cap_N_ru_supported_category                           */
    E_MODULE_CAPA_RU_PORT,          /* y_o_ran_module_cap_N_number_of_ru_ports                              */
    E_MODULE_CAPA_MAX_PWR,          /* y_o_ran_module_cap_N_max_power_per_pa_antenna                        */
    E_MODULE_CAPA_MIN_PWR,          /* y_o_ran_module_cap_N_min_power_per_pa_antenna                        */
    E_MODULE_CAPA_SPLT_OPT,         /* y_o_ran_module_cap_N_fronthaul_split_option                          */
    E_MODULE_CAPA_NUM_SPA_STREAM,   /* y_o_ran_module_cap_N_number_of_spatial_streams                       */
    E_MODULE_CAPA_ENERGY_SAVING,    /* y_o_ran_module_cap_N_energy_saving_by_transmission_blanks            */
    E_MODULE_CAPA_DYNAMIC_TRANS     /* y_o_ran_module_cap_N_dynamic_transport_delay_management_supported    */
}E_MODULE_CAPA ;

typedef enum
{
    E_IQ_SAMPLE_CMPSUP = 0 ,        /* y_o_ran_module_cap_N_dynamic_compression_supported           */
    E_IQ_SAMPLE_BWTSUP ,            /* y_o_ran_module_cap_N_realtime_variable_bit_width_supported   */
    E_IQ_SAMPLE_SYMSUP              /* y_o_ran_module_cap_N_syminc_supported                        */
}E_IQ_SAMPLE ;


/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/
extern uint32       saa_upconf_max_scs_a_num_dl;
extern uint32       saa_upconf_max_scs_b_num_dl;
extern uint32       saa_upconf_max_scs_a_num_ul;
extern uint32       saa_upconf_max_scs_b_num_ul;
extern uint32       saa_upconf_max_band_cap_num;


/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/
extern status_t i_o_ran_module_cap_module_capability_band_capabilities_get_key_num(const xmlChar *target_key_name, uint32 *curindex);
extern status_t i_o_ran_module_cap_module_capability_band_capabilities_get_first_key(xmlChar **key_name_temp);
extern status_t i_o_ran_module_cap_module_capability_band_capabilities_get_target_key(uint32 curindex, xmlChar **key_name_temp);
extern status_t i_o_ran_module_cap_module_capability_band_capabilities_get_para(uint32 curindex, const xmlChar *target_para_name, xmlChar **para_temp);

extern status_t i_o_ran_module_cap_module_capability_ru_capabilities_get_para(  unsigned int w_enum_beam ,
                                                                                xmlChar **para_temp ) ;
extern status_t i_o_ran_module_cap_module_capability_ru_capabilities_format_of_iq_sample_get_bool(  unsigned int w_enum_beam ,
                                                                                                    xmlChar **para_temp  ) ;
extern status_t i_o_ran_module_cap_module_capability_ru_capabilities_format_of_iq_sample_compression_method_supported_get_para(xmlChar **para_temp, const xmlChar *target_para_name);

extern status_t i_o_ran_module_cap_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_get_key
(
    const xmlChar *k_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_scs_a,
    const xmlChar *k_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_scs_b,
    uint32 *curindex_a,
    uint32 *curindex_b);
    
extern status_t i_o_ran_module_cap_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_get_first_key(
    uint32 scs_type,
    xmlChar **key_name_temp );
    
extern status_t i_o_ran_module_cap_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_get_target_key(
    uint32 curindex,
    uint32 scs_type,
    xmlChar **key_name_temp);

extern status_t i_o_ran_module_cap_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_get_para(
    uint32 curindex,
    const xmlChar *target_para_name,
    xmlChar **para_temp);

extern status_t i_o_ran_module_cap_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_get_key
(
    const xmlChar *k_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_scs_a,
    const xmlChar *k_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_scs_b,
    uint32 *curindex_a,
    uint32 *curindex_b);
    
extern status_t i_o_ran_module_cap_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_get_first_key(
    uint32 scs_type,
    xmlChar **key_name_temp );
    
extern status_t i_o_ran_module_cap_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_get_target_key(
    uint32 curindex,
    uint32 scs_type,
    xmlChar **key_name_temp);

extern status_t i_o_ran_module_cap_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_get_para(
    uint32 curindex,
    const xmlChar *target_para_name,
    xmlChar **para_temp);

extern status_t i_o_ran_module_cap_module_capability_ru_capabilities_format_of_iq_sample_compression_method_supported_compression_format_get(xmlChar **para_temp);

extern status_t i_o_ran_module_cap_init(void);

#endif
