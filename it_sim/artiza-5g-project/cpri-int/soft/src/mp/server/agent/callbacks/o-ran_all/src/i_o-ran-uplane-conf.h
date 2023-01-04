/*
 * @file    i_o-ran-uplane-conf.h
 */

#ifndef _H_i_o_ran_uplane_conf
#define _H_i_o_ran_uplane_conf


/*!
 * @addtogroup  SAA_MOD_O_RAN_UPLANE_CONF
 * @brief       SAA o-ran-uplane-conf
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <string.h>
#include <pthread.h>
#include "u_o-ran-uplane-conf.h"
#include "i_o-ran-processing-element.h"

#include "f_mpsw_msg_o-ran_uplane_conf_def.h"
#include "f_mpsw_msg_o-ran_uplane_conf_typ.h"

#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
#include "f_saa_ntf_lib.h"

#include "agt_util.h"


/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define SAA_UPCONF_MAX_DIR_NUM                  2               /* 送受信方向(UL/DL) */
#define SAA_UPCONF_MAX_ANT_NUM                  4               /* アンテナ数の最大値 */
#define SAA_UPCONF_MAX_CAR_NUM                  4               /* キャリア数の最大値 */
#define SAA_UPCONF_MAX_NUM_SSB                  2               /* SSB / non SSB */
#define SAA_UPCONF_ENABLE_SSB                   2               /* SSB対応 */
#define SAA_UPCONF_DISABLE_SSB                  1               /* SSB非対応 */
#define SAA_UPCONF_MAX_NUM_PRACH                2               /* PRACH / non PRACH */
#define SAA_UPCONF_MAX_NUM_PRACH_2ANT           4               /* 2ANT分のPRACH / non PRACH */
#define SAA_UPCONF_ENABLE_PRACH                 2               /* PRACH対応 */
#define SAA_UPCONF_DISABLE_PRACH                1               /* PRACH非対応 */
#define SAA_UPCONF_ENDPOINT_TYPES_NUM_SUB6      3               /* Sub6 Endpoint typesの数 */
#define SAA_UPCONF_ENDPOINT_TYPES_NUM_MMW       4               /* MMW Endpoint typesの数 */
#define SAA_UPCONF_POLARISATION_NUM             1               /* polarisationの数 */
#define SAA_UPCONF_SECTION_TYPE_NUM             1               /* section typeの数 */
#define SAA_UPCONF_DIR_UL                       0               /* UL(RX) */
#define SAA_UPCONF_DIR_DL                       1               /* DL(TX) */
#define SAA_UPCONF_CAR_ON_FLG                   1               /* キャリアONフラグ */
#define SAA_UPCONF_CAR_OFF_FLG                  0               /* キャリアOFFフラグ */
#define SAA_UPCONF_MAX_SCS_NUM                  7               /* SCS数の最大値 */
#define SAA_UPCONF_MAX_IF_NUM                   1               /* IF(eth0/eth1)数の最大値 */
#define SAA_UPCONF_DEFVAL_UL_FFT_SAMP_OFFSET    0xFFFFFFFF      /* ul-fft-sampling-offsetの初期値(未設定時) */
#define SAA_UPCONF_DEFVAL_N_TA_OFFSET           0xFFFFFFFF      /* n-ta-offsetの初期値(未設定時) */
#define SAA_UPCONF_UNIT_TYPE_SUB6               0               /* sub6 */
#define SAA_UPCONF_UNIT_TYPE_MMW                1               /* mmW */
#define SAA_UPCONF_ARFCN_TO_KHZ_PARA1_SUB6      3000000         /* ARFCNからkHzに変換するパラメータ */
#define SAA_UPCONF_ARFCN_TO_KHZ_PARA1_mmW       24250080        /* ARFCNからkHzに変換するパラメータ */
#define SAA_UPCONF_ARFCN_TO_KHZ_PARA2_SUB6      15              /* ARFCNからkHzに変換するパラメータ */
#define SAA_UPCONF_ARFCN_TO_KHZ_PARA2_mmW       60              /* ARFCNからkHzに変換するパラメータ */
#define SAA_UPCONF_ARFCN_TO_KHZ_PARA3_SUB6      600000          /* ARFCNからkHzに変換するパラメータ */
#define SAA_UPCONF_ARFCN_TO_KHZ_PARA3_mmW       2016667         /* ARFCNからkHzに変換するパラメータ */
#define SAA_UPCONF_DEFVAL_EAXC_ID               0xFFFF          /* eaxc-idの初期値(未設定時) */
#define SAA_UPCONF_DEFVAL_ANT_NUM               0xFFFFFFFF      /* antの初期値(未設定時) */
#define SAA_UPCONF_DEFVAL_CAR_NUM               0xFFFFFFFF      /* carの初期値(未設定時) */
#define SAA_UPCONF_DEFVAL_PORT_NUM              0xFFFFFFFF      /* portの初期値(未設定時) */
#define SAA_UPCONF_DEFVAL_EAXC_ID_NUM           0xFFFFFFFF      /* eaxc_id_numの初期値(未設定時) */
#define SAA_UPCONF_ENDP_INFO_NUM                0xFFFFFFFF      /* endp_info_numの初期値(未設定時) */
#define SAA_UPCONF_SECT_INFO_NUM                0xFFFFFFFF      /* section typeの初期値(未設定時) */
#define SAA_UPCONF_FHPORT_PORT0                 0               /* 物理ポート番号:0 */
#define SAA_UPCONF_FHPORT_PORT1                 1               /* 物理ポート番号:1 */
#define SAA_UPCONF_MAX_LINK_NUM                 (SAA_UPCONF_MAX_ANT_NUM * SAA_UPCONF_MAX_CAR_NUM * SAA_UPCONF_MAX_NUM_PRACH * SAA_UPCONF_MAX_IF_NUM)
#define SAA_UPCONF_MAX_ENDP_NUM                 (SAA_UPCONF_MAX_ANT_NUM * SAA_UPCONF_MAX_CAR_NUM * SAA_UPCONF_MAX_NUM_PRACH * SAA_UPCONF_MAX_IF_NUM)
#define SAA_UPCONF_MAX_ARR_CAR_NUM              (SAA_UPCONF_MAX_ANT_NUM * SAA_UPCONF_MAX_CAR_NUM)
#define SAA_UPCONF_ANT_A                        0               /* アンテナA */
#define SAA_UPCONF_ANT_B                        1               /* アンテナB */
#define SAA_UPCONF_ANT_C                        2               /* アンテナC */
#define SAA_UPCONF_ANT_D                        3               /* アンテナD */
#define SAA_UPCONF_BF_ON                        1               /* BF OFF   */
#define SAA_UPCONF_BF_OFF                       0               /* BF ON    */
#define SAA_UPCONF_MAX_ID_NUM                   4               /* 最大ID数 */
#define SAA_UPCONF_ENDP_SEC_TYPE_NON_PRACHSSB   1               /* Endpointのsection typeが1(PRACH or SSBでは無い) */
#define SAA_UPCONF_ENDP_SEC_TYPE_PRACHSSB       3               /* Endpointのsection typeが3(PRACH or SSB) */

/* port */
#define i_o_ran_uplane_conf_N_name_fheth0 (const xmlChar *)"fheth0"       /* interfase_name */
#define i_o_ran_uplane_conf_N_name_fheth1 (const xmlChar *)"fheth1"       /* interfase_name */

/* validate */
#define i_mod_o_ran_uplane_conf                                              "o-ran-uplane-conf"

#define i_para_o_ran_uplane_conf_llte_compression_type                       "user-plane-configuration.low-level-tx-endpoints.compression-type"
#define i_para_o_ran_uplane_conf_llte_bitwidth                               "user-plane-configuration.low-level-tx-endpoints.bitwidth"
#define i_para_o_ran_uplane_conf_llte_compression_format                     "user-plane-configuration.low-level-tx-endpoints.compression-format"

#define i_para_o_ran_uplane_conf_llre_compression_type                       "user-plane-configuration.low-level-rx-endpoints.compression-type"
#define i_para_o_ran_uplane_conf_llre_bitwidth                               "user-plane-configuration.low-level-rx-endpoints.bitwidth"
#define i_para_o_ran_uplane_conf_llre_compression_format                     "user-plane-configuration.low-level-rx-endpoints.compression-format"

#define i_para_o_ran_uplane_conf_llte_cp_type                                "user-plane-configuration.low-level-tx-endpoints.cp-type"
#define i_para_o_ran_uplane_conf_llte_cp_length                              "user-plane-configuration.low-level-tx-endpoints.cp-length"
#define i_para_o_ran_uplane_conf_llte_cp_length_other                        "user-plane-configuration.low-level-tx-endpoints.cp-length-other"

#define i_para_o_ran_uplane_conf_llre_cp_type                                "user-plane-configuration.low-level-rx-endpoints.cp-type"
#define i_para_o_ran_uplane_conf_llre_cp_length                              "user-plane-configuration.low-level-rx-endpoints.cp-length"
#define i_para_o_ran_uplane_conf_llre_cp_length_other                        "user-plane-configuration.low-level-rx-endpoints.cp-length-other"

#define i_para_o_ran_uplane_conf_llte_o_du_port_bitmask                      "user-plane-configuration.low-level-tx-endpoints.e-axcid.o-du-port-bitmask"
#define i_para_o_ran_uplane_conf_llte_band_sector_bitmask                    "user-plane-configuration.low-level-tx-endpoints.e-axcid.band-sector-bitmask"
#define i_para_o_ran_uplane_conf_llte_ccid_bitmask                           "user-plane-configuration.low-level-tx-endpoints.e-axcid.ccid-bitmask"
#define i_para_o_ran_uplane_conf_llte_ru_port_bitmask                        "user-plane-configuration.low-level-tx-endpoints.e-axcid.ru-port-bitmask"

#define i_para_o_ran_uplane_conf_llre_o_du_port_bitmask                      "user-plane-configuration.low-level-rx-endpoints.e-axcid.o-du-port-bitmask"
#define i_para_o_ran_uplane_conf_llre_band_sector_bitmask                    "user-plane-configuration.low-level-rx-endpoints.e-axcid.band-sector-bitmask"
#define i_para_o_ran_uplane_conf_llre_ccid_bitmask                           "user-plane-configuration.low-level-rx-endpoints.e-axcid.ccid-bitmask"
#define i_para_o_ran_uplane_conf_llre_ru_port_bitmask                        "user-plane-configuration.low-level-rx-endpoints.e-axcid.ru-port-bitmask"

#define i_para_o_ran_uplane_conf_llre_ul_fft_sampling_offsets_scs                      "user-plane-configuration.low-level-rx-endpoints.ul-fft-sampling-offsets.scs"
#define i_para_o_ran_uplane_conf_llre_ul_fft_sampling_offsets_ul_fft_sampling_offset   "user-plane-configuration.low-level-rx-endpoints.ul-fft-sampling-offsets.ul-fft-sampling-offset"
#define i_para_o_ran_uplane_conf_llre_non_time_managed_delay_enabled                   "user-plane-configuration.low-level-rx-endpoints.non-time-managed-delay-enabled"

#define i_para_o_ran_uplane_conf_tx_array_car_gain                                     "user-plane-configuration.tx-array-carriers.gain"
#define i_para_o_ran_uplane_conf_rx_array_car_gain_correction                          "user-plane-configuration.rx-array-carriers.gain-correction"
#define i_para_o_ran_uplane_conf_rx_array_car_n_ta_offset                              "user-plane-configuration.rx-array-carriers.n-ta-offset"

#define i_para_o_ran_uplane_conf_beamforming_mode                                    "beamforming_mode"
#define i_para_o_ran_uplane_conf                                                     "user-plane-configuration"
#define i_para_o_ran_uplane_conf_endpoints                                           "endpoints"

#define i_para_o_ran_uplane_conf_tx_array_car_abs_freq_cent                          "user-plane-configuration.tx-array-carriers.absolute-frequency-center"
#define i_para_o_ran_uplane_conf_tx_array_car_ch_bw                                  "user-plane-configuration.tx-array-carriers.channel-bandwidth"
#define i_para_o_ran_uplane_conf_tx_array_car_act                                    "user-plane-configuration.tx-array-carriers.active"
#define i_para_o_ran_uplane_conf_tx_array_car_dl_radio_frame_offset                  "user-plane-configuration.tx-array-carriers.downlink-radio-frame-offset"
#define i_para_o_ran_uplane_conf_tx_array_car_dl_sfn_offset                          "user-plane-configuration.tx-array-carriers.downlink-sfn-offset"

#define i_para_o_ran_uplane_conf_rx_array_car_abs_freq_cent                          "user-plane-configuration.rx-array-carriers.absolute-frequency-center"
#define i_para_o_ran_uplane_conf_rx_array_car_ch_bw                                  "user-plane-configuration.rx-array-carriers.channel-bandwidth"
#define i_para_o_ran_uplane_conf_rx_array_car_act                                    "user-plane-configuration.rx-array-carriers.active"


/* get */
#define i_para_o_ran_uplane_conf_tx_array_car_type                                   "user-plane-configuration.tx-array-carriers.0.type"
#define i_para_o_ran_uplane_conf_rx_array_car_type                                   "user-plane-configuration.rx-array-carriers.0.type"
#define i_para_o_ran_uplane_conf_tx_array_car_duplex_scheme                          "user-plane-configuration.tx-array-carriers.0.duplex-scheme"
#define i_para_o_ran_uplane_conf_rx_array_car_duplex_scheme                          "user-plane-configuration.rx-array-carriers.0.duplex-scheme"
#define i_para_o_ran_uplane_conf_tx_arrays                                           "user-plane-configuration.tx-arrays"
#define i_para_o_ran_uplane_conf_rx_arrays                                           "user-plane-configuration.rx-arrays"
#define i_para_o_ran_uplane_conf_name                                                "name"
#define i_para_o_ran_uplane_conf_nam_of_row                                          "number-of-rows"
#define i_para_o_ran_uplane_conf_num_of_col                                          "number-of-columns"
#define i_para_o_ran_uplane_conf_num_of_array_layers                                 "number-of-array-layers"
#define i_para_o_ran_uplane_conf_p                                                   "p"
#define i_para_o_ran_uplane_conf_id                                                  "id"
#define i_para_o_ran_uplane_conf_polarisation                                        "polarisation"
#define i_para_o_ran_uplane_conf_polarisations                                       "polarisations"
#define i_para_o_ran_uplane_conf_band_number                                         "band-number"
#define i_para_o_ran_uplane_conf_max_gain                                            "max-gain"
#define i_para_o_ran_uplane_conf_gain_correction_max                                 "gain-correction-range.max"
#define i_para_o_ran_uplane_conf_gain_correction_min                                 "gain-correction-range.min"
#define i_para_o_ran_uplane_conf_independent_pow_budget                              "independent-power-budget"
#define i_para_o_ran_uplane_conf_tx_endpoints                                        "user-plane-configuration.static-low-level-tx-endpoints"
#define i_para_o_ran_uplane_conf_rx_endpoints                                        "user-plane-configuration.static-low-level-rx-endpoints"
#define i_para_o_ran_uplane_conf_restricted_interface                                "restricted-interfaces"
#define i_para_o_ran_uplane_conf_array                                               "array"
#define i_para_o_ran_uplane_conf_endpoint_type                                       "endpoint-type"
#define i_para_o_ran_uplane_conf_endpoint_types                                      "endpoint-types"
#define i_para_o_ran_uplane_conf_state_disable                                       "DISABLED"
#define i_para_o_ran_uplane_conf_state_busy                                          "BUSY"
#define i_para_o_ran_uplane_conf_state_ready                                         "READY"
#define i_para_o_ran_uplane_conf_duplex_schema_tdd                                   "TDD"
#define i_para_o_ran_uplane_conf_section_type                                        "supported-section-types.section-type"
#define i_para_o_ran_uplane_conf_supported_frame_structure                           "supported-frame-structures"
#define i_para_o_ran_uplane_conf_managed_delay_support                               "managed-delay-support"
#define i_para_o_ran_uplane_conf_max_ctrl_sections_per_data                          "max-control-sections-per-data-section"
#define i_para_o_ran_uplane_conf_max_section_per_symbol                              "max-sections-per-symbol"
#define i_para_o_ran_uplane_conf_max_section_per_slot                                "max-sections-per-slot"
#define i_para_o_ran_uplane_conf_max_beam_per_symbol                                 "max-beams-per-symbol"
#define i_para_o_ran_uplane_conf_max_beam_per_slot                                   "max-beams-per-slot"
#define i_para_o_ran_uplane_conf_max_prb_per_symbol                                  "max-prb-per-symbol"

/* init */
#define i_para_o_ran_uplane_conf_unit_type                                           "unit_type"
#define i_para_o_ran_uplane_conf_max_ant_num                                         "max_ant_num"
#define i_para_o_ran_uplane_conf_max_car_num                                         "max_car_num"


/* enum */
#define i_enum_o_ran_uplane_conf_scs_khz_15                                          "KHZ_15"
#define i_enum_o_ran_uplane_conf_scs_khz_30                                          "KHZ_30"
#define i_enum_o_ran_uplane_conf_scs_khz_60                                          "KHZ_60"
#define i_enum_o_ran_uplane_conf_scs_khz_120                                         "KHZ_120"
#define i_enum_o_ran_uplane_conf_scs_khz_240                                         "KHZ_240"
#define i_enum_o_ran_uplane_conf_scs_khz_1_25                                        "KHZ_1_25"
#define i_enum_o_ran_uplane_conf_scs_khz_5                                           "KHZ_5"

/* edit */
#define i_o_ran_uplane_conf_N_carrier_inactive                                       (const xmlChar *)"INACTIVE"
#define i_o_ran_uplane_conf_N_carrier_active                                         (const xmlChar *)"ACTIVE"
#define i_o_ran_uplane_conf_N_carrier_sleep                                          (const xmlChar *)"SLEEP"

/********************************************************************************************************************/
/*  typedef                                                                                                        */
/********************************************************************************************************************/
typedef struct link_info_t_{
    xmlChar link_name[D_PARAM_NUM_MAX];
    xmlChar array_carrier[D_PARAM_NUM_MAX];
    xmlChar endpoints[D_PARAM_NUM_MAX];
    uint32  endp_info_num;
    uint32  local_eaxc_id_num;
    uint16  local_eaxc_id;
    uint16  eaxc_id;
    uint32  ant;
    uint32  car;
    uint32  port;
    uint32  section_type;
    uint32  active;
}link_info_t;

typedef struct endpoint_info_t_{
    xmlChar endp_name[D_PARAM_NUM_MAX];
    uint32  ant;
    uint32  car;
    uint32  local_eaxc_id_num;
    uint16  local_eaxc_id;
    uint32  port;
    uint32  active;
    uint32  section_type;
	uint32	ul_fft_sampling_offset;
}endpoint_info_t;

typedef struct tx_car_t_{
    xmlChar tx_car_name[D_PARAM_NUM_MAX];
    T_MPSW_TX_ARRAY_CAR_T tx_car_info;
}tx_car_t;

typedef struct rx_car_t_{
    xmlChar rx_car_name[D_PARAM_NUM_MAX];
    T_MPSW_RX_ARRAY_CAR_T rx_car_info;
}rx_car_t;

typedef struct array_ant_link_t_ {
    xmlChar array_name[D_PARAM_NUM_MAX];
}array_ant_link_t;

/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/
extern uint32           saa_upconf_unit_type;
extern uint32           saa_upconf_max_ant_num;
extern uint32           saa_upconf_max_car_num;
extern uint32           saa_upconf_max_tx_endpoints_num;
extern uint32           saa_upconf_max_rx_endpoints_num;
extern uint32           saa_upconf_max_rx_endpoints_types_num;
extern uint32           saa_upconf_max_ssb_num;
extern uint32           saa_upconf_max_prach_num;
extern uint32           saa_upconf_beamforming_mode;
extern link_info_t      saa_link_info_tbl[SAA_UPCONF_MAX_DIR_NUM][SAA_UPCONF_MAX_LINK_NUM];
extern endpoint_info_t  saa_endp_info_tbl[SAA_UPCONF_MAX_DIR_NUM][SAA_UPCONF_MAX_ENDP_NUM];
extern tx_car_t         saa_tx_car_tbl[SAA_UPCONF_MAX_ARR_CAR_NUM];
extern rx_car_t         saa_rx_car_tbl[SAA_UPCONF_MAX_ARR_CAR_NUM];
extern array_ant_link_t saa_array_ant_link_tbl[SAA_UPCONF_MAX_DIR_NUM][SAA_UPCONF_MAX_ANT_NUM];
extern pthread_mutex_t  mutex_for_upconf_array_ant_link_tbl;


extern char i_mod_upconf[];

extern char i_para_upconf_unit_type[];
extern char i_para_upconf_max_ant_num[];
extern char i_para_upconf_max_car_num[];
extern char i_para_upconf_tx_array_car_type[];
extern char i_para_upconf_rx_array_car_type[];
extern char i_para_upconf_tx_array_car_duplex_scheme[];
extern char i_para_upconf_rx_array_car_duplex_scheme[];
extern char i_para_upconf_tx_arrays[];
extern char i_para_upconf_rx_arrays[];
extern char i_para_upconf_name[];
extern char i_para_upconf_polarisation[];
extern char i_para_upconf_polarisations[];
extern char i_para_upconf_band_number[];
extern char i_para_upconf_tx_endpoints[];
extern char i_para_upconf_rx_endpoints[];
extern char i_para_upconf_array[];
extern char i_para_upconf_state_disable[];
extern char i_para_upconf_state_busy[];
extern char i_para_upconf_state_ready[];
extern char i_para_upconf_duplex_schema_tdd[];
extern char i_para_upconf[];
extern char i_para_upconf_endopints[];
extern char i_para_upconf_llte_compression_type[];
extern char i_para_upconf_llte_bitwidth[];        
extern char i_para_upconf_llte_compression_format[];
extern char i_para_upconf_llre_compression_type[];
extern char i_para_upconf_llre_bitwidth[];        
extern char i_para_upconf_llre_compression_format[];
extern char i_para_upconf_llte_cp_type[];         
extern char i_para_upconf_llte_cp_length[];       
extern char i_para_upconf_llte_cp_length_other[]; 
extern char i_para_upconf_llre_cp_type[];         
extern char i_para_upconf_llre_cp_length[];       
extern char i_para_upconf_llre_cp_length_other[]; 
extern char i_para_upconf_llte_o_du_port_bitmask[];
extern char i_para_upconf_llte_band_sector_bitmask[];
extern char i_para_upconf_llte_ccid_bitmask[];    
extern char i_para_upconf_llte_ru_port_bitmask[]; 
extern char i_para_upconf_llre_o_du_port_bitmask[];
extern char i_para_upconf_llre_band_sector_bitmask[];
extern char i_para_upconf_llre_ccid_bitmask[];    
extern char i_para_upconf_llre_ru_port_bitmask[]; 
extern char i_para_upconf_llre_ul_fft_sampling_offsets_scs[];
extern char i_para_upconf_llre_ul_fft_sampling_offsets_ul_fft_sampling_offset[];
extern char i_para_upconf_llre_non_time_managed_delay_enabled[];               
extern char i_para_upconf_conf_tx_array_car_gain[];
extern char i_para_upconf_rx_array_car_gain_correction[];
extern char i_para_upconf_rx_array_car_n_ta_offset[];
extern char i_para_upconf_beamforming_mode[];
extern char i_para_upconf_num_of_rows[];                
extern char i_para_upconf_num_of_col[];                 
extern char i_para_upconf_num_of_array_layers[];        
extern char i_para_upconf_p[];    
extern char i_para_upconf_id[];                        
extern char i_para_upconf_max_gain[];                   
extern char i_para_upconf_conf_gain_correction_max[];  
extern char i_para_upconf_conf_gain_correction_min[];   
extern char i_para_upconf_conf_independent_pow_budget[];
extern char i_para_upconf_restricted_interface[];
extern char i_para_upconf_endpoint_type[];
extern char i_para_upconf_endpoint_types[];
extern char i_para_upconf_conf_section_type[];
extern char i_para_upconf_conf_supported_frame_structure[]; 
extern char i_para_upconf_conf_managed_delay_support[];               
extern char i_para_upconf_conf_max_ctrl_sections_per_data[];             
extern char i_para_upconf_conf_max_section_per_symbol[];                 
extern char i_para_upconf_conf_max_section_per_slot[];                   
extern char i_para_upconf_conf_max_beam_per_symbol[];                    
extern char i_para_upconf_conf_max_beam_per_slot[];                    
extern char i_para_upconf_conf_max_prb_per_symbol[];   
extern char i_para_upconf_tx_array_car_abs_freq_cent[];
extern char i_para_upconf_tx_array_car_ch_bw[];
extern char i_para_upconf_tx_array_car_act[];
extern char i_para_upconf_tx_array_car_dl_radio_frame_offset[];
extern char i_para_upconf_tx_array_car_dl_sfn_offset[];
extern char i_para_upconf_rx_array_car_abs_freq_cent[];
extern char i_para_upconf_rx_array_car_ch_bw[];
extern char i_para_upconf_rx_array_car_act[];

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/
/* i_o-ran-uplane-conf_common.c */
extern status_t i_o_ran_uplane_conf_init(void);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_validate_list(char *para_name, const xmlChar *target);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_validate_list_us(char *para_name, uint16 target);
extern  status_t i_o_ran_uplane_conf_user_plane_configuration_validate_max_min(char *para_name, int64_t target);
extern int i_o_ran_uplane_conf_check_link_name_from_link_info_tbl(uint32 dir, const xmlChar *target_link_name, uint32 *link_info_num);
extern int i_o_ran_uplane_conf_set_link_name_to_link_info_tbl(uint32 dir, const xmlChar *target_link_name, uint32 *link_info_num);
extern int i_o_ran_uplane_conf_check_tx_car_name_from_tx_car_tbl(const xmlChar *target_car_name, uint32 *tx_car_num);
extern int i_o_ran_uplane_conf_set_tx_car_name_to_tx_car_tbl(const xmlChar *target_car_name, uint32 *tx_car_num);
extern int i_o_ran_uplane_conf_check_scs_from_tx_car_scs_spec_conf_t(uint32 tx_car_num, const xmlChar *target_scs, uint32 *scs_num);
extern int i_o_ran_uplane_conf_set_scs_to_tx_car_scs_spec_conf_t(uint32 tx_car_num, const xmlChar *target_scs, uint32 *scs_num);
extern int i_o_ran_uplane_conf_check_rx_car_name_from_rx_car_tbl(const xmlChar *target_car_name, uint32 *rx_car_num);
extern int i_o_ran_uplane_conf_set_rx_car_name_to_rx_car_tbl(const xmlChar *target_car_name, uint32 *rx_car_num);
extern int i_o_ran_uplane_conf_check_scs_from_rx_car_scs_spec_conf_t(uint32 rx_car_num, const xmlChar *target_scs, uint32 *scs_num);
extern int i_o_ran_uplane_conf_set_scs_to_rx_car_scs_spec_conf_t(uint32 rx_car_num, const xmlChar *target_scs, uint32 *scs_num);
extern status_t i_o_ran_uplane_conf_set_tx_carrier(const xmlChar *tx_car_name, const xmlChar *car_on_off_str);
extern int i_o_ran_uplane_conf_check_car_name_from_link_info_tbl(uint32 dir, const xmlChar *target_car_name, uint32 *link_info_num, uint32 *link_info_cnt);
extern int i_o_ran_uplane_conf_check_car_name_from_link_info_antnum_tbl(uint32 dir, const xmlChar *target_car_name, uint32 antnum, uint32 *link_info_num, uint32 *link_info_cnt);
extern int i_o_ran_uplane_conf_set_msi_data_tx(T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ *tx_msg_data, uint32 *link_info_num, uint32 tx_car_num, E_UP_CAR_ACTIVE car_on_off, uint32 link_cnt);
extern status_t i_o_ran_uplane_conf_set_rx_carrier(const xmlChar *rx_car_name, const xmlChar *car_on_off_str);
extern int i_o_ran_uplane_conf_set_msi_data_rx(T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ *rx_msg_data, uint32 *link_info_num, uint32 rx_car_num, E_UP_CAR_ACTIVE car_on_off, uint32 link_cnt);
extern int i_o_ran_uplane_conf_allocate_ant_car_from_local_eaxc_id(uint32 dir, uint16 target, uint32 *ant, uint32 *car, uint32 *local_eaxc_id_num);
extern int i_o_ran_uplane_conf_check_static_endpoints_name(uint32 dir, const xmlChar *target_endp_name);
extern int i_o_ran_uplane_conf_check_duplicated_local_eaxc_id_from_endp_info_tbl(uint32 dir, uint16 target);
extern int i_o_ran_uplane_conf_check_endp_name_from_endp_info_tbl(uint32 dir, const xmlChar *target_endp_name, uint32 *endp_info_num);
extern int i_o_ran_uplane_conf_set_endp_name_to_endp_info_tbl(uint32 dir, const xmlChar *target_endp_name, uint32 *endp_info_num);
extern status_t i_o_ran_uplane_conf_eaxc_id_validate(uint32 dir, val_value_t *newval, const xmlChar *target_endp_name);
extern int i_o_ran_uplane_conf_check_array_carrier_from_car_tbl(uint32 dir, const xmlChar *array_carriers);
extern int i_o_ran_uplane_conf_check_endpoints_from_endp_info_tbl(uint32 dir, const xmlChar *endpoints, uint32 *endp_info_num);
extern int i_o_ran_uplane_conf_check_pair_link_from_link_info_tbl(uint32 dir, const xmlChar *array_carriers, uint32 link_info_num, uint32 *pair_link_info_num);
extern void i_o_ran_uplane_conf_set_carrier_active_state(uint32 dir, uint32 car_num, uint32 *link_info_num, E_UP_CAR_ACTIVE car_on_off);
extern int i_o_ran_uplane_conf_get_dir_ant_from_fault_ant(E_FAULT_SOURCE fault_ant, uint32 *dir, uint32 *ant);
extern int i_o_ran_uplane_conf_check_port_from_endp_name(uint32 dir, const xmlChar *target_endp_name, uint32 *port);
extern void i_o_ran_uplane_conf_tx_array_carriers_state_change_handler(void *data, uint32_t size);
extern void i_o_ran_uplane_conf_rx_array_carriers_state_change_handler(void *data, uint32_t size);
extern status_t i_o_ran_uplane_conf_get_array_carrier_from_ant_car(uint32 dir, uint32 ant, uint32 car, xmlChar *array_carrier);
extern status_t i_o_ran_uplane_conf_conv_state_enum_to_xmlchar(E_RFCP_UPCNF_STS state_enum, xmlChar **state);
extern int i_o_ran_uplane_conf_allocate_array_name_and_ant_num(uint32 dir, uint32 ant, const xmlChar *target_endp_name);
extern int i_o_ran_uplane_conf_array_ant_validate(uint32 dir, uint32 ant, const xmlChar *array_temp);
extern int i_o_ran_uplane_conf_get_array_name_from_fm_info(E_FAULT_SOURCE fault_source, xmlChar *fault_source_str);
extern int i_o_ran_uplane_conf_get_link_name_from_ant(uint32 dir, uint32 ant, xmlChar *fault_source_str);
extern status_t i_o_ran_uplane_conf_sectiontype_get(uint32 dir, const xmlChar *target_endp_name, uint32 *section_type);
extern status_t i_o_ran_uplane_conf_name_validate(const xmlChar *target_name);
extern T_MPSW_MSI_O_RAN_UPLANE_CONF_TX_STATE_CHANGE_NTF_IND *
    i_o_ran_uplane_conf_get_tx_notification_data(void);
extern T_MPSW_MSI_O_RAN_UPLANE_CONF_RX_STATE_CHANGE_NTF_IND *
    i_o_ran_uplane_conf_get_rx_notification_data(void);

/* i_o-ran-uplane-conf_varidate_tx_links.c */
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_tx_links_tx_array_carrier_validate(val_value_t *newval, const xmlChar *k_user_plane_configuration_low_level_tx_links_name);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_tx_links_low_level_tx_endpoint_validate(val_value_t *newval, const xmlChar *k_user_plane_configuration_low_level_tx_links_name);

/* i_o-ran-uplane-conf_varidate_rx_links.c */
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_links_rx_array_carrier_validate(val_value_t *newval, const xmlChar *k_user_plane_configuration_low_level_rx_links_name);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_links_low_level_rx_endpoint_validate(val_value_t *newval, const xmlChar *k_user_plane_configuration_low_level_rx_links_name);

/* i_o-ran-uplane-conf_get_tx_endpoints.c */
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_endpoints_get_key_num(const xmlChar *k_user_plane_configuration_static_low_level_tx_endpoints_name, uint32 *curindex);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_endpoints_get_first_key(xmlChar **key_name_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_endpoints_get_target_key(uint32 curindex, xmlChar **key_name_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_endpoints_get_para(uint32 curindex, const xmlChar *target_para_name, xmlChar **para_temp);

/* i_o-ran-uplane-conf_get_rx_endpoints.c */
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_endpoints_get_key_num(const xmlChar *k_user_plane_configuration_static_low_level_rx_endpoints_name, uint32 *curindex);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_endpoints_get_first_key(xmlChar **key_name_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_endpoints_get_target_key(uint32 curindex, xmlChar **key_name_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_endpoints_get_para(uint32 curindex, const xmlChar *target_para_name, xmlChar **para_temp);

/* i_o-ran-uplane-conf_varidate_tx_endpoints.c */
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_tx_endpoints_e_axcid_ru_port_bitmask_validate(val_value_t *newval, val_value_t *curval);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_tx_endpoints_e_axcid_band_sector_bitmask_validate(val_value_t *newval, val_value_t *curval);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_tx_endpoints_e_axcid_ccid_bitmask_validate(val_value_t *newval, val_value_t *curval);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_tx_endpoints_e_axcid_o_du_port_bitmask_validate(val_value_t *newval, val_value_t *curval);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_tx_endpoints_e_axcid_eaxc_id_validate(val_value_t *newval, const xmlChar *k_user_plane_configuration_low_level_tx_endpoints_name);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_tx_endpoints_cp_type_validate(val_value_t *newval, val_value_t *curval);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_tx_endpoints_cp_length_validate(val_value_t *newval, val_value_t *curval, const xmlChar *k_user_plane_configuration_low_level_tx_endpoints_name);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_tx_endpoints_cp_length_other_validate(val_value_t *newval, val_value_t *curval, const xmlChar *k_user_plane_configuration_low_level_tx_endpoints_name);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_tx_endpoints_compression_compression_type_validate(val_value_t *newval, val_value_t *curval);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_tx_endpoints_compression_bitwidth_validate(val_value_t *newval, val_value_t *curval);

/* i_o-ran-uplane-conf_varidate_rx_endpoints.c */
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_e_axcid_ru_port_bitmask_validate(val_value_t *newval, val_value_t *curval);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_e_axcid_band_sector_bitmask_validate(val_value_t *newval, val_value_t *curval);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_e_axcid_ccid_bitmask_validate(val_value_t *newval, val_value_t *curval);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_e_axcid_o_du_port_bitmask_validate(val_value_t *newval, val_value_t *curval);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_e_axcid_eaxc_id_validate(val_value_t *newval, const xmlChar *k_user_plane_configuration_low_level_rx_endpoints_name);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_cp_type_validate(val_value_t *newval, val_value_t *curval);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_cp_length_validate(val_value_t *newval, val_value_t *curval, const xmlChar *k_user_plane_configuration_low_level_rx_endpoints_name);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_cp_length_other_validate(val_value_t *newval, val_value_t *curval, const xmlChar *k_user_plane_configuration_low_level_rx_endpoints_name);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_compression_compression_type_validate(val_value_t *newval, val_value_t *curval);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_compression_bitwidth_validate(val_value_t *newval, val_value_t *curval);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_non_time_managed_delay_enabled_validate(val_value_t *newval, val_value_t *curval);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_ul_fft_sampling_offsets_ul_fft_sampling_offset_validate(val_value_t *newval, val_value_t *curval, const xmlChar *k_user_plane_configuration_low_level_rx_endpoints_name);

/* i_o-ran-uplane-conf_varidate_tx_array_carriers.c */
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_absolute_frequency_center_validate(val_value_t *newval, val_value_t *curval, const xmlChar *k_user_plane_configuration_tx_array_carriers_name);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_center_of_channel_bandwidth_validate(val_value_t *newval, val_value_t *curval, const xmlChar *k_user_plane_configuration_tx_array_carriers_name);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_channel_bandwidth_validate(val_value_t *newval, val_value_t *curval, const xmlChar *k_user_plane_configuration_tx_array_carriers_name);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_gain_validate(val_value_t *newval, val_value_t *curval, const xmlChar *k_user_plane_configuration_tx_array_carriers_name);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_active_validate(val_value_t *newval, val_value_t *curval);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_downlink_radio_frame_offset_validate(val_value_t *newval, val_value_t *curval, const xmlChar *k_user_plane_configuration_tx_array_carriers_name);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_downlink_sfn_offset_validate(val_value_t *newval, val_value_t *curval, const xmlChar *k_user_plane_configuration_tx_array_carriers_name);

/* i_o-ran-uplane-conf_varidate_rx_array_carriers.c */
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_array_carriers_absolute_frequency_center_validate(val_value_t *newval, val_value_t *curval, const xmlChar *k_user_plane_configuration_rx_array_carriers_name);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_array_carriers_center_of_channel_bandwidth_validate(val_value_t *newval, val_value_t *curval, const xmlChar *k_user_plane_configuration_rx_array_carriers_name);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_array_carriers_channel_bandwidth_validate(val_value_t *newval, val_value_t *curval, const xmlChar *k_user_plane_configuration_rx_array_carriers_name);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_array_carriers_active_validate(val_value_t *newval, val_value_t *curval);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_array_carriers_n_ta_offset_validate(val_value_t *newval, val_value_t *curval, const xmlChar *k_user_plane_configuration_rx_array_carriers_name);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_array_carriers_gain_correction_validate(val_value_t *newval, val_value_t *curval);


/* i_o-ran-uplane-conf_get_array_carriers.c */
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_type_get(const xmlChar *k_user_plane_configuration_tx_array_carriers_name, xmlChar **v_type_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_array_carriers_type_get(const xmlChar *k_user_plane_configuration_rx_array_carriers_name, xmlChar **v_type_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_array_carriers_state_get(const xmlChar *target_array_name, xmlChar **v_state_temp, uint32 dir);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_array_carriers_duplex_get(xmlChar **v_duplex_temp, uint32 dir);

/* i_o-ran-uplane-conf_get_tx_arrays.c */
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_arrays_get_key_num(const xmlChar *k_user_plane_configuration_tx_arrays_name, uint32 *curindex);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_arrays_get_first_key(xmlChar **key_name_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_arrays_get_target_key(uint32 curindex, xmlChar **key_name_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_arrays_get_para(uint32 curindex, const xmlChar *target_para_name, xmlChar **para_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_arrays_polarisations_get_first_key(xmlChar **key_name_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_arrays_polarisations_get_target_key(uint32 curindex, const xmlChar *k_user_plane_configuration_tx_arrays_name, xmlChar **key_name_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_arrays_polarisations_get_para(uint32 curindex,const xmlChar *k_user_plane_configuration_tx_arrays_name, const xmlChar *target_para_name, xmlChar **para_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_tx_arrays_polarisations_get_key_num(const xmlChar *k_user_plane_configuration_tx_arrays_name, uint8 k_user_plane_configuration_tx_arrays_polarisations_p, uint32 *curindex);

/* i_o-ran-uplane-conf_get_rx_arrays.c */
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_get_key_num(const xmlChar *k_user_plane_configuration_rx_arrays_name, uint32 *curindex);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_get_first_key(xmlChar **key_name_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_get_target_key(uint32 curindex, xmlChar **key_name_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_get_para(uint32 curindex, const xmlChar *target_para_name, xmlChar **para_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_polarisations_get_first_key(xmlChar **key_name_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_polarisations_get_target_key(uint32 curindex, const xmlChar *k_user_plane_configuration_rx_arrays_name, xmlChar **key_name_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_polarisations_get_para(uint32 curindex,const xmlChar *k_user_plane_configuration_rx_arrays_name, const xmlChar *target_para_name, xmlChar **para_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_polarisations_get_key_num(const xmlChar *k_user_plane_configuration_tx_arrays_name, uint8 k_user_plane_configuration_rx_arrays_polarisations_p, uint32 *curindex);


/* i_o-ran-uplane-conf_get_endpoint_types.c */
extern status_t i_o_ran_uplane_conf_user_plane_configuration_endpoint_type_get_first_key(xmlChar **key_name_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_endpoint_type_get_target_key(uint32 curindex, xmlChar **key_name_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_endpoint_type_get_para(uint32 curindex, const xmlChar *target_para_name, xmlChar **para_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_endpoint_types_supported_section_types_get_first_key(uint16 k_user_plane_configuration_endpoint_types_id, xmlChar **key_name_temp);
extern status_t i_o_ran_uplane_conf_user_plane_configuration_endpoint_types_supported_section_types_get_target_key(uint32 curindex, uint16 k_user_plane_configuration_endpoint_types_id, xmlChar **key_name_temp);

#endif
