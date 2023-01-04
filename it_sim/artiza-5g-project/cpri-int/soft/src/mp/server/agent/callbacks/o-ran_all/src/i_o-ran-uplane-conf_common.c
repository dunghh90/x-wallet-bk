/*
 * @file    i_o-ran-uplane-conf_common.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_UPLANE_CONF
 * @brief       SAA o-ran-uplane-conf
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "i_o-ran-uplane-conf.h"


/********************************************************************************************************************/
/*   valuable                                                                                                       */
/********************************************************************************************************************/
char i_mod_upconf[]                                                      = i_mod_o_ran_uplane_conf;
char i_para_upconf_unit_type[]                                           = i_para_o_ran_uplane_conf_unit_type;
char i_para_upconf_max_ant_num[]                                         = i_para_o_ran_uplane_conf_max_ant_num;
char i_para_upconf_max_car_num[]                                         = i_para_o_ran_uplane_conf_max_car_num;
char i_para_upconf_tx_array_car_type[]                                   = i_para_o_ran_uplane_conf_tx_array_car_type;
char i_para_upconf_rx_array_car_type[]                                   = i_para_o_ran_uplane_conf_rx_array_car_type;
char i_para_upconf_tx_array_car_duplex_scheme[]                          = i_para_o_ran_uplane_conf_tx_array_car_duplex_scheme;
char i_para_upconf_rx_array_car_duplex_scheme[]                          = i_para_o_ran_uplane_conf_rx_array_car_duplex_scheme;
char i_para_upconf_tx_arrays[]                                           = i_para_o_ran_uplane_conf_tx_arrays;
char i_para_upconf_rx_arrays[]                                           = i_para_o_ran_uplane_conf_rx_arrays;
char i_para_upconf_name[]                                                = i_para_o_ran_uplane_conf_name;
char i_para_upconf_polarisation[]                                        = i_para_o_ran_uplane_conf_polarisation;
char i_para_upconf_polarisations[]                                       = i_para_o_ran_uplane_conf_polarisations;
char i_para_upconf_band_number[]                                         = i_para_o_ran_uplane_conf_band_number;
char i_para_upconf_tx_endpoints[]                                        = i_para_o_ran_uplane_conf_tx_endpoints;
char i_para_upconf_rx_endpoints[]                                        = i_para_o_ran_uplane_conf_rx_endpoints;
char i_para_upconf_restricted_interface[]                                = i_para_o_ran_uplane_conf_restricted_interface;
char i_para_upconf_endpoint_types[]                                      = i_para_o_ran_uplane_conf_endpoint_types;
char i_para_upconf_endpoint_type[]                                       = i_para_o_ran_uplane_conf_endpoint_type;
char i_para_upconf_array[]                                               = i_para_o_ran_uplane_conf_array;
char i_para_upconf_state_disable[]                                       = i_para_o_ran_uplane_conf_state_disable;
char i_para_upconf_state_busy[]                                          = i_para_o_ran_uplane_conf_state_busy;
char i_para_upconf_state_ready[]                                         = i_para_o_ran_uplane_conf_state_ready;
char i_para_upconf_duplex_schema_tdd[]                                   = i_para_o_ran_uplane_conf_duplex_schema_tdd;
char i_para_upconf_llte_compression_type[]                               = i_para_o_ran_uplane_conf_llte_compression_type;
char i_para_upconf_llte_bitwidth[]                                       = i_para_o_ran_uplane_conf_llte_bitwidth;
char i_para_upconf_llte_compression_format[]                             = i_para_o_ran_uplane_conf_llte_compression_format;
char i_para_upconf_llre_compression_type[]                               = i_para_o_ran_uplane_conf_llre_compression_type; 
char i_para_upconf_llre_bitwidth[]                                       = i_para_o_ran_uplane_conf_llre_bitwidth;
char i_para_upconf_llre_compression_format[]                             = i_para_o_ran_uplane_conf_llre_compression_format;
char i_para_upconf_llte_cp_type[]                                        = i_para_o_ran_uplane_conf_llte_cp_type;
char i_para_upconf_llte_cp_length[]                                      = i_para_o_ran_uplane_conf_llte_cp_length;
char i_para_upconf_llte_cp_length_other[]                                = i_para_o_ran_uplane_conf_llte_cp_length_other;                
char i_para_upconf_llre_cp_type[]                                        = i_para_o_ran_uplane_conf_llre_cp_type;
char i_para_upconf_llre_cp_length[]                                      = i_para_o_ran_uplane_conf_llre_cp_length;
char i_para_upconf_llre_cp_length_other[]                                = i_para_o_ran_uplane_conf_llre_cp_length_other;  
char i_para_upconf_llte_o_du_port_bitmask[]                              = i_para_o_ran_uplane_conf_llte_o_du_port_bitmask;                 
char i_para_upconf_llte_band_sector_bitmask[]                            = i_para_o_ran_uplane_conf_llte_band_sector_bitmask;               
char i_para_upconf_llte_ccid_bitmask[]                                   = i_para_o_ran_uplane_conf_llte_ccid_bitmask;                      
char i_para_upconf_llte_ru_port_bitmask[]                                = i_para_o_ran_uplane_conf_llte_ru_port_bitmask; 
char i_para_upconf_llre_o_du_port_bitmask[]                              = i_para_o_ran_uplane_conf_llre_o_du_port_bitmask;                 
char i_para_upconf_llre_band_sector_bitmask[]                            = i_para_o_ran_uplane_conf_llre_band_sector_bitmask;               
char i_para_upconf_llre_ccid_bitmask[]                                   = i_para_o_ran_uplane_conf_llre_ccid_bitmask;                      
char i_para_upconf_llre_ru_port_bitmask[]                                = i_para_o_ran_uplane_conf_llre_ru_port_bitmask; 
char i_para_upconf_llre_ul_fft_sampling_offsets_scs[]                    = i_para_o_ran_uplane_conf_llre_ul_fft_sampling_offsets_scs;                     
char i_para_upconf_llre_ul_fft_sampling_offsets_ul_fft_sampling_offset[] = i_para_o_ran_uplane_conf_llre_ul_fft_sampling_offsets_ul_fft_sampling_offset;  
char i_para_upconf_llre_non_time_managed_delay_enabled[]                 = i_para_o_ran_uplane_conf_llre_non_time_managed_delay_enabled;                 
char i_para_upconf_conf_tx_array_car_gain[]                              = i_para_o_ran_uplane_conf_tx_array_car_gain;                                    
char i_para_upconf_rx_array_car_gain_correction[]                        = i_para_o_ran_uplane_conf_rx_array_car_gain_correction;                       
char i_para_upconf_rx_array_car_n_ta_offset[]                            = i_para_o_ran_uplane_conf_rx_array_car_n_ta_offset;
char i_para_upconf_beamforming_mode[]                                    = i_para_o_ran_uplane_conf_beamforming_mode;
char i_para_upconf_num_of_rows[]                                         = i_para_o_ran_uplane_conf_nam_of_row;
char i_para_upconf_num_of_col[]                                          = i_para_o_ran_uplane_conf_num_of_col;
char i_para_upconf_num_of_array_layers[]                                 = i_para_o_ran_uplane_conf_num_of_array_layers;
char i_para_upconf_p[]                                                   = i_para_o_ran_uplane_conf_p;
char i_para_upconf_id[]                                                  = i_para_o_ran_uplane_conf_id;
char i_para_upconf_max_gain[]                                            = i_para_o_ran_uplane_conf_max_gain;
char i_para_upconf_conf_gain_correction_max[]                            = i_para_o_ran_uplane_conf_gain_correction_max;
char i_para_upconf_conf_gain_correction_min[]                            = i_para_o_ran_uplane_conf_gain_correction_min;
char i_para_upconf_conf_independent_pow_budget[]                         = i_para_o_ran_uplane_conf_independent_pow_budget;
char i_para_upconf_conf_section_type[]                                   = i_para_o_ran_uplane_conf_section_type;         
char i_para_upconf_conf_supported_frame_structure[]                      = i_para_o_ran_uplane_conf_supported_frame_structure;
char i_para_upconf_conf_managed_delay_support[]                          = i_para_o_ran_uplane_conf_managed_delay_support;
char i_para_upconf_conf_max_ctrl_sections_per_data[]                     = i_para_o_ran_uplane_conf_max_ctrl_sections_per_data;
char i_para_upconf_conf_max_section_per_symbol[]                         = i_para_o_ran_uplane_conf_max_section_per_symbol;
char i_para_upconf_conf_max_section_per_slot[]                           = i_para_o_ran_uplane_conf_max_section_per_slot;
char i_para_upconf_conf_max_beam_per_symbol[]                            = i_para_o_ran_uplane_conf_max_beam_per_symbol;
char i_para_upconf_conf_max_beam_per_slot[]                              = i_para_o_ran_uplane_conf_max_beam_per_slot;
char i_para_upconf_conf_max_prb_per_symbol[]                             = i_para_o_ran_uplane_conf_max_prb_per_symbol;
char i_para_upconf_tx_array_car_abs_freq_cent[]                          = i_para_o_ran_uplane_conf_tx_array_car_abs_freq_cent;
char i_para_upconf_tx_array_car_ch_bw[]                                  = i_para_o_ran_uplane_conf_tx_array_car_ch_bw;
char i_para_upconf_tx_array_car_act[]                                    = i_para_o_ran_uplane_conf_tx_array_car_act;
char i_para_upconf_tx_array_car_dl_radio_frame_offset[]                  = i_para_o_ran_uplane_conf_tx_array_car_dl_radio_frame_offset;
char i_para_upconf_tx_array_car_dl_sfn_offset[]                          = i_para_o_ran_uplane_conf_tx_array_car_dl_sfn_offset;
char i_para_upconf_rx_array_car_abs_freq_cent[]                          = i_para_o_ran_uplane_conf_rx_array_car_abs_freq_cent;
char i_para_upconf_rx_array_car_ch_bw[]                                  = i_para_o_ran_uplane_conf_rx_array_car_ch_bw;
char i_para_upconf_rx_array_car_act[]                                    = i_para_o_ran_uplane_conf_rx_array_car_act;

char i_para_upconf[]                                                     = i_para_o_ran_uplane_conf;
char i_para_upconf_endopints[]                                           = i_para_o_ran_uplane_conf_endpoints;

uint32              saa_upconf_unit_type;
uint32              saa_upconf_max_ant_num;
uint32              saa_upconf_max_car_num;
uint32              saa_upconf_max_tx_endpoints_num;
uint32              saa_upconf_max_rx_endpoints_num;
uint32              saa_upconf_max_rx_endpoints_types_num;
uint32              saa_upconf_max_ssb_num;
uint32              saa_upconf_max_prach_num;
uint32              saa_upconf_beamforming_mode;
link_info_t         saa_link_info_tbl[SAA_UPCONF_MAX_DIR_NUM][SAA_UPCONF_MAX_LINK_NUM];
endpoint_info_t     saa_endp_info_tbl[SAA_UPCONF_MAX_DIR_NUM][SAA_UPCONF_MAX_ENDP_NUM];
tx_car_t            saa_tx_car_tbl[SAA_UPCONF_MAX_ARR_CAR_NUM];
rx_car_t            saa_rx_car_tbl[SAA_UPCONF_MAX_ARR_CAR_NUM];
array_ant_link_t    saa_array_ant_link_tbl[SAA_UPCONF_MAX_DIR_NUM][SAA_UPCONF_MAX_ANT_NUM];

pthread_mutex_t     mutex_for_upconf_array_ant_link_tbl = PTHREAD_MUTEX_INITIALIZER;

static T_MPSW_MSI_O_RAN_UPLANE_CONF_TX_STATE_CHANGE_NTF_IND m_tx_notification_data;
static T_MPSW_MSI_O_RAN_UPLANE_CONF_RX_STATE_CHANGE_NTF_IND m_rx_notification_data;

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   アンテナ/キャリア、管理テーブルの初期化を行う
 * @note    
 * @param   [in] void
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/27 KCN)吉木
 * @date    2019/02/12 KCN)吉木 3rdハソ対応
 * @date    2019/02/18 KCN)吉木 3rdハソ対応
 */
status_t i_o_ran_uplane_conf_init(void)
{
    status_t res = NO_ERR;
    char *init_unit_type;
    char *init_max_ant_num;
    char *init_max_car_num;
    char *init_beamforming_mode;
    int ret;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* 装置種別、アンテナ数、キャリア数、Beamforming_modeの初期化 */
    init_unit_type = f_saa_com_get_init_data(i_mod_upconf, i_para_upconf_unit_type);
    init_max_ant_num = f_saa_com_get_init_data(i_mod_upconf, i_para_upconf_max_ant_num);
    init_max_car_num = f_saa_com_get_init_data(i_mod_upconf, i_para_upconf_max_car_num);
    init_beamforming_mode = f_saa_com_get_init_data(i_mod_upconf, i_para_upconf_beamforming_mode);
    if(init_unit_type == NULL || init_max_ant_num == NULL || init_max_car_num == NULL) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("\n%s: Data Missing.", __func__);
        }
        return res;
    }
    ret = sscanf (init_unit_type, "%d", &saa_upconf_unit_type);
    if(ret != 1) {
        if(LOGWARN) {
            log_warn("\n%s: sscanf error, saa_upconf_unit_type.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }
    ret = sscanf (init_max_ant_num, "%d", &saa_upconf_max_ant_num);
    if(ret != 1) {
        if(LOGWARN) {
            log_warn("\n%s: sscanf error, saa_upconf_max_ant_num.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }
    ret = sscanf (init_max_car_num, "%d", &saa_upconf_max_car_num);
    if(ret != 1) {
        if(LOGWARN) {
            log_warn("\n%s: sscanf error, saa_upconf_max_car_num.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    if(saa_upconf_unit_type == SAA_UPCONF_UNIT_TYPE_SUB6) {
        saa_upconf_max_ssb_num = SAA_UPCONF_DISABLE_SSB;
        saa_upconf_max_prach_num = SAA_UPCONF_ENABLE_PRACH;
        saa_upconf_max_rx_endpoints_types_num = SAA_UPCONF_ENDPOINT_TYPES_NUM_SUB6;
    }
    else {
        saa_upconf_max_ssb_num = SAA_UPCONF_ENABLE_SSB;
        saa_upconf_max_prach_num = SAA_UPCONF_ENABLE_PRACH;
        saa_upconf_max_rx_endpoints_types_num = SAA_UPCONF_ENDPOINT_TYPES_NUM_MMW;
    }

    saa_upconf_max_tx_endpoints_num = saa_upconf_max_ant_num * saa_upconf_max_car_num * saa_upconf_max_ssb_num * SAA_UPCONF_MAX_IF_NUM;
    saa_upconf_max_rx_endpoints_num = saa_upconf_max_ant_num * saa_upconf_max_car_num * saa_upconf_max_prach_num * SAA_UPCONF_MAX_IF_NUM;
    if (LOGDEV1) {
        log_dev1("\n%s: saa_upconf_unit_type=%d(0:sub6, 1:mmW)", __func__, saa_upconf_unit_type);
        log_dev1("\n%s: saa_upconf_max_ant_num=%d", __func__, saa_upconf_max_ant_num);
        log_dev1("\n%s: saa_upconf_max_car_num=%d", __func__, saa_upconf_max_car_num);
        log_dev1("\n%s: saa_upconf_max_ssb_num=%d", __func__, saa_upconf_max_ssb_num);
        log_dev1("\n%s: saa_upconf_max_prach_num=%d", __func__, saa_upconf_max_prach_num);
        log_dev1("\n%s: saa_upconf_max_tx_endpoints_num=%d", __func__, saa_upconf_max_tx_endpoints_num);
        log_dev1("\n%s: saa_upconf_max_rx_endpoints_num=%d", __func__, saa_upconf_max_rx_endpoints_num);
    }

    ret = sscanf (init_beamforming_mode, "%d", &saa_upconf_beamforming_mode);
    if(ret != 1) {
        if(LOGWARN) {
            log_warn("\n%s: sscanf error, saa_upconf_beamforming_mode.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }
   
    /* saa_link_info_tbl, saa_endp_info_tbl初期化 */
    for (int dir_cnt = 0; dir_cnt < SAA_UPCONF_MAX_DIR_NUM; dir_cnt++) {
        /* saa_link_info_tbl */
        for (int link_cnt = 0; link_cnt < SAA_UPCONF_MAX_LINK_NUM; link_cnt++) {
            saa_link_info_tbl[dir_cnt][link_cnt].link_name[0] = '\0';
            saa_link_info_tbl[dir_cnt][link_cnt].array_carrier[0] = '\0';
            saa_link_info_tbl[dir_cnt][link_cnt].endpoints[0] = '\0';
            saa_link_info_tbl[dir_cnt][link_cnt].endp_info_num = 0;
            saa_link_info_tbl[dir_cnt][link_cnt].local_eaxc_id_num = SAA_UPCONF_DEFVAL_EAXC_ID_NUM;
            saa_link_info_tbl[dir_cnt][link_cnt].local_eaxc_id = SAA_UPCONF_DEFVAL_EAXC_ID;
            saa_link_info_tbl[dir_cnt][link_cnt].eaxc_id = SAA_UPCONF_DEFVAL_EAXC_ID;
            saa_link_info_tbl[dir_cnt][link_cnt].ant = SAA_UPCONF_DEFVAL_ANT_NUM;
            saa_link_info_tbl[dir_cnt][link_cnt].car = SAA_UPCONF_DEFVAL_CAR_NUM;
            saa_link_info_tbl[dir_cnt][link_cnt].port = SAA_UPCONF_DEFVAL_PORT_NUM;
            saa_link_info_tbl[dir_cnt][link_cnt].section_type = SAA_UPCONF_SECT_INFO_NUM;
            saa_link_info_tbl[dir_cnt][link_cnt].active = E_UP_CAR_ACTIVE_INACTIVE;
        }
        /* saa_endp_info_tbl */
        for (int endp_cnt = 0; endp_cnt < SAA_UPCONF_MAX_ENDP_NUM; endp_cnt++) {
            saa_endp_info_tbl[dir_cnt][endp_cnt].endp_name[0] = '\0';
            saa_endp_info_tbl[dir_cnt][endp_cnt].ant = SAA_UPCONF_DEFVAL_ANT_NUM;
            saa_endp_info_tbl[dir_cnt][endp_cnt].car = SAA_UPCONF_DEFVAL_CAR_NUM;
            saa_endp_info_tbl[dir_cnt][endp_cnt].local_eaxc_id_num = SAA_UPCONF_DEFVAL_EAXC_ID_NUM;
            saa_endp_info_tbl[dir_cnt][endp_cnt].local_eaxc_id = SAA_UPCONF_DEFVAL_EAXC_ID;
            saa_endp_info_tbl[dir_cnt][endp_cnt].port = SAA_UPCONF_DEFVAL_PORT_NUM;
            saa_endp_info_tbl[dir_cnt][endp_cnt].section_type = SAA_UPCONF_SECT_INFO_NUM;
            saa_endp_info_tbl[dir_cnt][endp_cnt].active = E_UP_CAR_ACTIVE_INACTIVE;
        }
    }

    /* saa_tx_car_tbl, saa_rx_car_tbl初期化 */
    for(int i = 0; i < SAA_UPCONF_MAX_ARR_CAR_NUM; i++) {
        /* saa_tx_car_tbl */
        saa_tx_car_tbl[i].tx_car_name[0] = '\0';
        saa_tx_car_tbl[i].tx_car_info.ant = SAA_UPCONF_DEFVAL_ANT_NUM;
        saa_tx_car_tbl[i].tx_car_info.car = SAA_UPCONF_DEFVAL_CAR_NUM;
        saa_tx_car_tbl[i].tx_car_info.abs_freq_center = 0;
        saa_tx_car_tbl[i].tx_car_info.cent_of_ch_bandwidth = 0;
        saa_tx_car_tbl[i].tx_car_info.ch_bandwidth = 0;
        saa_tx_car_tbl[i].tx_car_info.gain = 0;
        saa_tx_car_tbl[i].tx_car_info.active = E_UP_CAR_ACTIVE_INACTIVE;
        saa_tx_car_tbl[i].tx_car_info.fft_size = 0;
        saa_tx_car_tbl[i].tx_car_info.dl_radio_frame_offset = 0;
        saa_tx_car_tbl[i].tx_car_info.dl_sfn_offset = 0;
        saa_tx_car_tbl[i].tx_car_info.default_scs = E_UP_CAR_DEF_SCS_KHZ_EMPTY;
        saa_tx_car_tbl[i].tx_car_info.cp_length = 0;
        
        /* saa_rx_car_tbl */
        saa_rx_car_tbl[i].rx_car_name[0] = '\0';
        saa_rx_car_tbl[i].rx_car_info.ant = SAA_UPCONF_DEFVAL_ANT_NUM;
        saa_rx_car_tbl[i].rx_car_info.car = SAA_UPCONF_DEFVAL_CAR_NUM;
        saa_rx_car_tbl[i].rx_car_info.abs_freq_center = 0;
        saa_rx_car_tbl[i].rx_car_info.cent_of_ch_bandwidth = 0;
        saa_rx_car_tbl[i].rx_car_info.ch_bandwidth = 0;
        saa_rx_car_tbl[i].rx_car_info.active = E_UP_CAR_ACTIVE_INACTIVE;
        saa_rx_car_tbl[i].rx_car_info.fft_size = 0;
        saa_rx_car_tbl[i].rx_car_info.ul_fft_sampling_offset = SAA_UPCONF_DEFVAL_UL_FFT_SAMP_OFFSET;
        saa_rx_car_tbl[i].rx_car_info.n_ta_offset = SAA_UPCONF_DEFVAL_N_TA_OFFSET;
        saa_rx_car_tbl[i].rx_car_info.default_scs = E_UP_CAR_DEF_SCS_KHZ_EMPTY;
        saa_rx_car_tbl[i].rx_car_info.cp_length = 0;

        for(int j = 0; j < SAA_UPCONF_MAX_SCS_NUM; j++) {
            /* saa_tx_car_tbl */
            saa_tx_car_tbl[i].tx_car_info.scs_spec_conf_t[j].scs = E_UP_CAR_SCS_KHZ_EMPTY;
            saa_tx_car_tbl[i].tx_car_info.scs_spec_conf_t[j].offset_to_abs_freq_center = 0;
            saa_tx_car_tbl[i].tx_car_info.scs_spec_conf_t[j].num_of_prbs = 0;
            /* saa_rx_car_tbl */
            saa_rx_car_tbl[i].rx_car_info.scs_spec_conf_t[j].scs = E_UP_CAR_SCS_KHZ_EMPTY;
            saa_rx_car_tbl[i].rx_car_info.scs_spec_conf_t[j].offset_to_abs_freq_center = 0;
            saa_rx_car_tbl[i].rx_car_info.scs_spec_conf_t[j].num_of_prbs = 0;
        }
    }

    return res;

} /* i_o_ran_uplane_conf_init */

/*!
 * @brief   listのValidateを行う
 * @note    
 * @param   [in] para_name
 * @param   [in] target
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/12/05 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_validate_list(
    char *para_name,
    const xmlChar *target)
{
    status_t res = ERR_NCX_INVALID_VALUE;
    int rtn_saa;
    char *validate_param_addr;
    T_SAA_COM_PARAM_LIST_TBL validate_param_list;

    if (LOGINFO) {
        log_info("\nEnter %s: para_name=%s, target=%s", __func__, para_name, target);
    }

    /* validate_param_list初期化 */
    memset((void *)&validate_param_list, 0, sizeof(T_SAA_COM_PARAM_LIST_TBL));

    /* validate用パラメータを取得 */
    validate_param_addr = f_saa_com_get_validate_data(i_mod_upconf, para_name);
    if (validate_param_addr == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get validate data, para_name=%s", __func__, para_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate用パラメータの変換 */
    rtn_saa = f_saa_com_conversion_validate_data_list(validate_param_addr, &validate_param_list);
    if (rtn_saa == D_SYS_NG) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to convert validate data, para_name=%s", __func__, para_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate */
    for (int i = 0; validate_param_list.list[i][0] != '\0'; i++) {
        if (!xml_strcmp((xmlChar *)validate_param_list.list[i], target)){
            if (LOGDEV1) {
                log_dev1("\n%s: Validation Completed, para_name=%s, target=%s", __func__, para_name, target);
            }
            res = NO_ERR;
            break;
        }
    }
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed, para_name=%s", __func__, para_name);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    return res;

}

/*!
* @brief   listのValidateを行う(uint16)
 * @note    
 * @param   [in] para_name
 * @param   [in] target
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/12/05 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_validate_list_us(
    char *para_name,
    uint16 target)
{
    status_t res = ERR_NCX_INVALID_VALUE;
    int rtn_saa;
    char *validate_param_addr;
    T_SAA_COM_PARAM_LIST_TBL validate_param_list;
    uint16 validate_param = 0;

    if (LOGINFO) {
        log_info("\nEnter %s: para_name = %s, target = %d", __func__, para_name, target);
    }

    /* validate_param_list初期化 */
    memset((void *)&validate_param_list, 0, sizeof(T_SAA_COM_PARAM_LIST_TBL));

    /* validate用パラメータを取得 */
    validate_param_addr = f_saa_com_get_validate_data(i_mod_upconf, para_name);
    if (validate_param_addr == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get validate data, para_name=%s", __func__, para_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate用パラメータの変換 */
    rtn_saa = f_saa_com_conversion_validate_data_list(validate_param_addr, &validate_param_list);
    if (rtn_saa == D_SYS_NG) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to convert validate data, para_name=%s", __func__, para_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate */
    for (int i = 0; validate_param_list.list[i][0] != '\0'; i++) {
        sscanf ((const char *)validate_param_list.list[i], "%hx", &validate_param);
        if (target == validate_param) {
            if (LOGDEV1) {
                log_dev1("\n%s: Validation Completed, para_name=%s, target=%x", __func__, para_name, target);
            }
            res = NO_ERR;
            break;
        }
    }
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed, para_name=%s", __func__, para_name);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    return res;

}

/*!
 * @brief   max/minのValidateを行う
 * @note    
 * @param   [in] para_name
 * @param   [in] target
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/12/05 KCN)吉木
 */
 status_t i_o_ran_uplane_conf_user_plane_configuration_validate_max_min(
    char *para_name,
    int64_t target)
{
    status_t res = ERR_NCX_INVALID_VALUE;
    int rtn_saa;
    char *validate_param_addr;
    int64_t validate_param_max;
    int64_t validate_param_min;

    if (LOGINFO) {
        log_info("\nEnter %s: para_name = %s, target = %ld", __func__, para_name, target);
    }

    /* validate用パラメータを取得 */
    validate_param_addr = f_saa_com_get_validate_data(i_mod_upconf, para_name);
    if (validate_param_addr == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get validate data, para_name=%s", __func__, para_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate用パラメータの変換 */
    rtn_saa = f_saa_com_conversion_validate_data_max_min(validate_param_addr, &validate_param_min, &validate_param_max);
    if (rtn_saa == D_SYS_NG) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to convert validate data, para_name=%s", __func__, para_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate */
    if (validate_param_min <= target && target <=validate_param_max) {
        if (LOGDEV1) {
            log_dev1("\n%s: Validation Completed, para_name=%s, target = %ld", __func__, para_name, target);
        }
        res = NO_ERR;
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed, para_name=%s", __func__, para_name);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    return res;

}

/*!
 * @brief   link_info_tblからlink_nameの検索を行う
 * @note    
 * @param   [in] dir
 * @param   [in] target_link_name
 * @param   [out] link_info_num
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2018/11/24 KCN)吉木
 */
int i_o_ran_uplane_conf_check_link_name_from_link_info_tbl(
    uint32 dir,
    const xmlChar *target_link_name,
    uint32 *link_info_num)
{
    int res = D_SYS_NG;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    for (uint32 i = 0; i < SAA_UPCONF_MAX_LINK_NUM; i++){
        if (!xml_strcmp((xmlChar *)saa_link_info_tbl[dir][i].link_name, target_link_name)) {
            *link_info_num = i;
            res = D_SYS_OK;
            if (LOGDEV1) {
                log_dev1("\n%s: Links name registered, link_info_num = %d", __func__, i);
            }
            break;
        }
    }

    if (res == D_SYS_NG) {
        if (LOGDEV1) {
            log_dev1("\n%s: Missing links name, name = %s", __func__, target_link_name);
        }
        return D_SYS_NG;
    }

    return res;

} /* i_o_ran_uplane_conf_check_link_name_from_link_info_tbl */

/*!
 * @brief   link_info_tblにlink_nameの登録を行う
 * @note    
 * @param   [in] target_link_name
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2018/11/24 KCN)吉木
 */
int i_o_ran_uplane_conf_set_link_name_to_link_info_tbl(
    uint32 dir,
    const xmlChar *target_link_name,
    uint32 *link_info_num)
{
    int res = D_SYS_NG;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    for (uint32 i = 0; i < SAA_UPCONF_MAX_LINK_NUM; i++){
        if (LOGDEV1) {
            log_dev1("\n%s: saa_link_info_tbl[dir][i].link_name = %s, i = %d", __func__, saa_link_info_tbl[dir][i].link_name, i);
        }
        if (saa_link_info_tbl[dir][i].link_name[0] == '\0') {
            /* link_info_tblにlink_nameを登録 */
            xml_strcpy(saa_link_info_tbl[dir][i].link_name, target_link_name);
            *link_info_num = i;
            res = D_SYS_OK;
            if (LOGDEV1) {
                log_dev1("\n%s: Add links name to control table, name = %s, num = %d", __func__, saa_link_info_tbl[dir][i].link_name, i);
            }
            break;
        }
    }

    if(res == D_SYS_NG){
        if(LOGWARN) {
            log_warn("\n%s: Resource denied, capacity lack of control table.", __func__);
        }
        return D_SYS_NG;
    }

    return res;

} /* i_o_ran_uplane_conf_set_link_name_to_link_info_tbl */

/*!
 * @brief   saa_tx_car_tblからtx_car_nameの検索を行う
 * @note    
 * @param   [in] target_car_name
 * @param   [out] tx_car_num
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2018/12/11 KCN)吉木
 */
int i_o_ran_uplane_conf_check_tx_car_name_from_tx_car_tbl(
    const xmlChar *target_car_name,
    uint32 *tx_car_num)
{
    int res = D_SYS_NG;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    for (uint32 i = 0; i < SAA_UPCONF_MAX_ARR_CAR_NUM; i++){
        if (!xml_strcmp((xmlChar *)saa_tx_car_tbl[i].tx_car_name, target_car_name)) {
            *tx_car_num = i;
            res = D_SYS_OK;
            if (LOGDEV1) {
                log_dev1("\n%s: Tx car name registered, target_car_name=%d", __func__, i);
            }
            break;
        }
    }

    if (res == D_SYS_NG) {
        if (LOGDEV1) {
            log_dev1("\n%s: Missing car name, name=%s", __func__, target_car_name);
        }
        return D_SYS_NG;
    }

    return res;

} /* i_o_ran_uplane_conf_check_tx_car_name_from_tx_car_tbl */

/*!
 * @brief   saa_tx_car_tblにtx_car_nameの登録を行う
 * @note    
 * @param   [in] target_car_name
 * @param   [out] tx_car_num
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2018/12/12 KCN)吉木
 */
int i_o_ran_uplane_conf_set_tx_car_name_to_tx_car_tbl(
    const xmlChar *target_car_name,
    uint32 *tx_car_num)
{
    int res = D_SYS_NG;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    for (uint32 i = 0; i < SAA_UPCONF_MAX_ARR_CAR_NUM; i++){
        if (LOGDEV1) {
            log_dev1("\n%s: saa_tx_car_tbl[%d].tx_car_name=%s", __func__, i, saa_tx_car_tbl[i].tx_car_name);
        }
        if (saa_tx_car_tbl[i].tx_car_name[0] == '\0') {
            /* saa_tx_car_tblにtarget_car_nameを登録 */
            xml_strcpy(saa_tx_car_tbl[i].tx_car_name, target_car_name);
            *tx_car_num = i;
            res = D_SYS_OK;
            if (LOGDEV1) {
                log_dev1("\n%s: Add Tx car name to control table, name=%s, num = %d", __func__, target_car_name, i);
            }
            break;
        }
    }

    if(res == D_SYS_NG){
        if(LOGWARN) {
            log_warn("\n%s: Resource denied, capacity lack of control table(saa_tx_car_tbl)", __func__);
        }
        return D_SYS_NG;
    }

    return res;

} /* i_o_ran_uplane_conf_set_tx_car_name_to_tx_car_tbl */

/*!
 * @brief   saa_tx_car_tblのscs_spec_conf_tからscsの検索を行う
 * @note    
 * @param   [in] target_car_name
 * @param   [out] tx_car_num
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2018/12/12 KCN)吉木
 */
int i_o_ran_uplane_conf_check_scs_from_tx_car_scs_spec_conf_t(
    uint32 tx_car_num,
    const xmlChar *target_scs,
    uint32 *scs_num)
{
    int res = D_SYS_NG;
    E_UP_SCS target_scs_temp;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* target_scsをENUMに変換*/
    if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_15)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_15;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_30)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_30;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_60)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_60;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_120)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_120;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_240)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_240;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_1_25)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_1_25;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_5)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_5;
    }
    else {
        target_scs_temp = E_UP_CAR_SCS_KHZ_EMPTY;
    }

    for (uint32 i = 0; i < SAA_UPCONF_MAX_SCS_NUM; i++){
        if (saa_tx_car_tbl[tx_car_num].tx_car_info.scs_spec_conf_t[i].scs == target_scs_temp) {
            *scs_num = i;
            res = D_SYS_OK;
            if (LOGDEV1) {
                log_dev1("\n%s: scs registered, scs_num=%d", __func__, i);
            }
            break;
        }
    }

    if(res == D_SYS_NG){
        if (LOGDEV1) {
            log_dev1("\n%s: Missing scs, scs=%s", __func__, target_scs);
        }
        return D_SYS_NG;
    }

    return res;

} /* i_o_ran_uplane_conf_check_scs_from_tx_car_scs_spec_conf_t */

/*!
 * @brief   saa_tx_car_tblのscs_spec_conf_tからscsの検索を行う
 * @note    
 * @param   [in] target_car_name
 * @param   [out] tx_car_num
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2018/12/12 KCN)吉木
 */
int i_o_ran_uplane_conf_set_scs_to_tx_car_scs_spec_conf_t(
    uint32 tx_car_num,
    const xmlChar *target_scs,
    uint32 *scs_num)
{
    int res = D_SYS_NG;
    E_UP_SCS target_scs_temp;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* target_scsをENUMに変換*/
    if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_15)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_15;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_30)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_30;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_60)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_60;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_120)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_120;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_240)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_240;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_1_25)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_1_25;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_5)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_5;
    }
    else {
        target_scs_temp = E_UP_CAR_SCS_KHZ_EMPTY;
    }

    for (uint32 i = 0; i < SAA_UPCONF_MAX_SCS_NUM; i++){
        if (LOGDEV1) {
            log_dev1("\n%s: saa_tx_car_tbl[%d].tx_car_info.scs_spec_conf_t[%d].scs = %d", __func__, tx_car_num, i, saa_tx_car_tbl[tx_car_num].tx_car_info.scs_spec_conf_t[i].scs);
        }
        if (saa_tx_car_tbl[tx_car_num].tx_car_info.scs_spec_conf_t[i].scs == E_UP_CAR_SCS_KHZ_EMPTY) {
            /* saa_tx_car_tblのscs_spec_conf_tにtarget_scs_tempを登録 */
            saa_tx_car_tbl[tx_car_num].tx_car_info.scs_spec_conf_t[i].scs = target_scs_temp;
            *scs_num = i;
            res = D_SYS_OK;
            if (LOGDEV1) {
                log_dev1("\n%s: Add scs to control table, name=%s, num=%d", __func__, target_scs, i);
            }
            break;
        }
    }

    if(res == D_SYS_NG){
        if(LOGWARN) {
            log_warn("\n%s: Resource denied, capacity lack of control table(saa_tx_car_tbl)", __func__);
        }
        return D_SYS_NG;
    }

    return res;

} /* i_o_ran_uplane_conf_set_scs_to_tx_car_scs_spec_conf_t */

/*!
 * @brief   saa_rx_car_tblからrx_car_nameの検索を行う
 * @note    
 * @param   [in] target_car_name
 * @param   [out] rx_car_num
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2018/12/11 KCN)吉木
 */
int i_o_ran_uplane_conf_check_rx_car_name_from_rx_car_tbl(
    const xmlChar *target_car_name,
    uint32 *rx_car_num)
{
    int res = D_SYS_NG;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    for (uint32 i = 0; i < SAA_UPCONF_MAX_ARR_CAR_NUM; i++){
        if (!xml_strcmp((xmlChar *)saa_rx_car_tbl[i].rx_car_name, target_car_name)) {
            *rx_car_num = i;
            res = D_SYS_OK;
            if (LOGDEV1) {
                log_dev1("\n%s: rx car name registered, target_car_name=%d", __func__, i);
            }
            break;
        }
    }

    if (res == D_SYS_NG) {
        if (LOGDEV1) {
            log_dev1("\n%s: Missing car name, name=%s", __func__, target_car_name);
        }
        return D_SYS_NG;
    }

    return res;

} /* i_o_ran_uplane_conf_check_rx_car_name_from_rx_car_tbl */

/*!
 * @brief   saa_rx_car_tblにrx_car_nameの登録を行う
 * @note    
 * @param   [in] target_car_name
 * @param   [out] rx_car_num
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2018/12/12 KCN)吉木
 */
int i_o_ran_uplane_conf_set_rx_car_name_to_rx_car_tbl(
    const xmlChar *target_car_name,
    uint32 *rx_car_num)
{
    int res = D_SYS_NG;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    for (uint32 i = 0; i < SAA_UPCONF_MAX_ARR_CAR_NUM; i++){
        if (LOGDEV1) {
            log_dev1("\n%s: saa_rx_car_tbl[%d].rx_car_name=%s", __func__, i, saa_rx_car_tbl[i].rx_car_name);
        }
        if (saa_rx_car_tbl[i].rx_car_name[0] == '\0') {
            /* saa_rx_car_tblにtarget_car_nameを登録 */
            xml_strcpy(saa_rx_car_tbl[i].rx_car_name, target_car_name);
            *rx_car_num = i;
            res = D_SYS_OK;
            if (LOGDEV1) {
                log_dev1("\n%s: Add rx car name to control table, name=%s, num = %d", __func__, target_car_name, i);
            }
            break;
        }
    }

    if(res == D_SYS_NG){
        if(LOGWARN) {
            log_warn("\n%s: Resource denied, capacity lack of control table(saa_rx_car_tbl)", __func__);
        }
        return D_SYS_NG;
    }

    return res;

} /* i_o_ran_uplane_conf_set_rx_car_name_to_rx_car_tbl */

/*!
 * @brief   saa_rx_car_tblのscs_spec_conf_tからscsの検索を行う
 * @note    
 * @param   [in] target_car_name
 * @param   [out] rx_car_num
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2018/12/12 KCN)吉木
 */
int i_o_ran_uplane_conf_check_scs_from_rx_car_scs_spec_conf_t(
    uint32 rx_car_num,
    const xmlChar *target_scs,
    uint32 *scs_num)
{
    int res = D_SYS_NG;
    E_UP_SCS target_scs_temp;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* target_scsをENUMに変換*/
    if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_15)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_15;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_30)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_30;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_60)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_60;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_120)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_120;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_240)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_240;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_1_25)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_1_25;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_5)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_5;
    }
    else {
        target_scs_temp = E_UP_CAR_SCS_KHZ_EMPTY;
    }

    for (uint32 i = 0; i < SAA_UPCONF_MAX_SCS_NUM; i++){
        if (saa_rx_car_tbl[rx_car_num].rx_car_info.scs_spec_conf_t[i].scs == target_scs_temp) {
            *scs_num = i;
            res = D_SYS_OK;
            if (LOGDEV1) {
                log_dev1("\n%s: scs registered, scs_num=%d", __func__, i);
            }
            break;
        }
    }

    if (res == D_SYS_NG) {
        if (LOGDEV1) {
            log_dev1("\n%s: Missing scs, scs=%s", __func__, target_scs);
        }
        return D_SYS_NG;
    }

    return res;

} /* i_o_ran_uplane_conf_check_scs_from_rx_car_scs_spec_conf_t */

/*!
 * @brief   saa_rx_car_tblのscs_spec_conf_tからscsの検索を行う
 * @note    
 * @param   [in] target_car_name
 * @param   [out] rx_car_num
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2018/12/12 KCN)吉木
 */
int i_o_ran_uplane_conf_set_scs_to_rx_car_scs_spec_conf_t(
    uint32 rx_car_num,
    const xmlChar *target_scs,
    uint32 *scs_num)
{
    int res = D_SYS_NG;
    E_UP_SCS target_scs_temp;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* target_scsをENUMに変換*/
    if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_15)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_15;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_30)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_30;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_60)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_60;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_120)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_120;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_240)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_240;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_1_25)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_1_25;
    }
    else if(!xml_strcmp(target_scs, (const xmlChar*)i_enum_o_ran_uplane_conf_scs_khz_5)) {
        target_scs_temp = E_UP_CAR_SCS_KHZ_5;
    }
    else {
        target_scs_temp = E_UP_CAR_SCS_KHZ_EMPTY;
    }

    for (uint32 i = 0; i < SAA_UPCONF_MAX_SCS_NUM; i++){
        if (LOGDEV1) {
            log_dev1("\n%s: saa_rx_car_tbl[%d].rx_car_info.scs_spec_conf_t[%d].scs = %d", __func__, rx_car_num, i, saa_rx_car_tbl[rx_car_num].rx_car_info.scs_spec_conf_t[i].scs);
        }
        if (saa_rx_car_tbl[rx_car_num].rx_car_info.scs_spec_conf_t[i].scs == E_UP_CAR_SCS_KHZ_EMPTY) {
            /* saa_rx_car_tblのscs_spec_conf_tにtarget_scs_tempを登録 */
            saa_rx_car_tbl[rx_car_num].rx_car_info.scs_spec_conf_t[i].scs = target_scs_temp;
            *scs_num = i;
            res = D_SYS_OK;
            if (LOGDEV1) {
                log_dev1("\n%s: Add scs to control table, name=%s, num=%d", __func__, target_scs, i);
            }
            break;
        }
    }

    if(res == D_SYS_NG){
        if(LOGWARN) {
            log_warn("\n%s: Resource denied, capacity lack of control table(saa_rx_car_tbl)", __func__);
        }
        return D_SYS_NG;
    }

    return res;

} /* i_o_ran_uplane_conf_set_scs_to_rx_car_scs_spec_conf_t */

/*!
 * @brief   tx carrier on/offのMSI送信を行う
 * @note    
 * @param   [in] tx_car_name
 * @param   [in] car_on_off_str
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 * @date    2018/12/12 KCN)吉木
 * @date    2019/03/29 KCN)吉木 SLEEP対応
 */
status_t i_o_ran_uplane_conf_set_tx_carrier(
    const xmlChar *tx_car_name,
    const xmlChar *car_on_off_str)
{
    status_t res = NO_ERR;
    int rtn_saa;
    uint32 dir = SAA_UPCONF_DIR_DL;
    uint32 link_info_num[SAA_UPCONF_MAX_NUM_SSB] = {0};
    uint32 tx_car_num = 0;
    uint32 link_info_cnt = 0;
    T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ tx_msg_data;
    uint32_t response_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REJ read_data;
    int rtn_msi = D_MSG_BUFFER_GET_OK;
    E_UP_CAR_ACTIVE car_on_off;

    if (LOGINFO) {
        log_info("\nEnter %s: %s, %s", __func__, tx_car_name, car_on_off_str);
    }

    /* 状態遷移判別 */
    if(!xml_strcmp(car_on_off_str, i_o_ran_uplane_conf_N_carrier_inactive)) {
        car_on_off = E_UP_CAR_ACTIVE_INACTIVE;
    }
    else if(!xml_strcmp(car_on_off_str, i_o_ran_uplane_conf_N_carrier_active)) {
        car_on_off = E_UP_CAR_ACTIVE_ACTIVE;
    }
    else {
        car_on_off = E_UP_CAR_ACTIVE_SLEEP;
    }

    /* tx_msg_data初期化 */
    memset(&tx_msg_data, 0, sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ));

    /* read_data初期化 */
    memset(&read_data, 0 , sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REJ));

    /* saa_tx_car_tblからtx_car_name検索 */
    rtn_saa = i_o_ran_uplane_conf_check_tx_car_name_from_tx_car_tbl(tx_car_name, &tx_car_num);
    if(rtn_saa == D_SYS_NG) {
        if (LOGWARN) {
            log_warn("\n%s: Failed to get name from tx_car_tbl, name=%s", __func__, tx_car_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* link_info_tblからtx_car_name検索 */
    rtn_saa = i_o_ran_uplane_conf_check_car_name_from_link_info_tbl(dir, tx_car_name, link_info_num, &link_info_cnt);
    if(rtn_saa == D_SYS_NG) {
        if (LOGWARN) {
            log_warn("\n%s: Failed to get name from link_info_tbl, name = %s", __func__, tx_car_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* キャリアの状態を判別 */
    /* 同じ状態への遷移はNG */
    if(saa_tx_car_tbl[tx_car_num].tx_car_info.active == car_on_off) {
        if (LOGWARN) {
            log_warn("\n%s: Failed, carrier(%s) is already target state(%s)", __func__, tx_car_name, car_on_off_str);
        }
        return ERR_NCX_INVALID_VALUE;
    }
    /* INACTIVEからSLEEPへの遷移はNG */
    else if(saa_tx_car_tbl[tx_car_num].tx_car_info.active != E_UP_CAR_ACTIVE_ACTIVE && car_on_off == E_UP_CAR_ACTIVE_SLEEP) {
        if (LOGWARN) {
            log_warn("\n%s: Failed, carrier(%s) is not ACTIVE", __func__, tx_car_name);
        }
        return ERR_NCX_INVALID_VALUE;
    }
    /* SLEEPからINACTIVEへの遷移はNG */
    else if(saa_tx_car_tbl[tx_car_num].tx_car_info.active != E_UP_CAR_ACTIVE_ACTIVE && car_on_off == E_UP_CAR_ACTIVE_INACTIVE) {
        if (LOGWARN) {
            log_warn("\n%s: Failed, carrier(%s) is not ACTIVE", __func__, tx_car_name);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    if((saa_upconf_beamforming_mode == SAA_UPCONF_BF_ON) && (saa_upconf_unit_type == SAA_UPCONF_UNIT_TYPE_SUB6)) {
    /* Sub6かつBF有りの場合はlink_info_numからANT A/Bのlink_info_numのみ抽出する */
        if(saa_link_info_tbl[dir][link_info_num[0]].ant == SAA_UPCONF_ANT_A || saa_link_info_tbl[dir][link_info_num[0]].ant == SAA_UPCONF_ANT_C){
            i_o_ran_uplane_conf_check_car_name_from_link_info_antnum_tbl(dir, tx_car_name, SAA_UPCONF_ANT_A, link_info_num, &link_info_cnt);
        }
        else {
            i_o_ran_uplane_conf_check_car_name_from_link_info_antnum_tbl(dir, tx_car_name, SAA_UPCONF_ANT_B, link_info_num, &link_info_cnt);
        }
    }
	
    /* MSI送信用のデータ設定 */
    rtn_saa = i_o_ran_uplane_conf_set_msi_data_tx(&tx_msg_data, link_info_num, tx_car_num, car_on_off, link_info_cnt);
    if(rtn_saa == D_SYS_NG) {
        if (LOGWARN) {
            log_warn("\n%s: Failed to set tx car on MSI data, name = %s", __func__, tx_car_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* MSI送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ, sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ), (void*)&tx_msg_data);
    if(response_id == 0) {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* MSIレスポンス待ち */
    rtn_msi = f_saa_com_get_response(response_id,
        sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REJ),
        &message_id,
        &read_size,
        (void *)&read_data,
        D_MSG_WAIT_TIME );


    /* MSIレスポンス判定 */
    switch (rtn_msi) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id != D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_CFM) {
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: wrong message id. (%0x)", __func__, message_id);
            }
            return ERR_NCX_OPERATION_FAILED;
        }
        else {
            i_o_ran_uplane_conf_set_carrier_active_state(dir, tx_car_num, link_info_num, car_on_off);
        }
        break;
    case D_MSG_BUFFER_GET_ERR_SIZE:
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: too small buffer size.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
        break;
    case D_MSG_BUFFER_GET_ERR_TIMEOUT:
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: time out.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
        break;
    case D_MSG_BUFFER_GET_ERR_OTHER:
    default:
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: other.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* Sub6かつBF有りの場合は対応する別アンテナの設定も実施する */
    if((saa_upconf_beamforming_mode == SAA_UPCONF_BF_ON) && (saa_upconf_unit_type == SAA_UPCONF_UNIT_TYPE_SUB6)) {
        uint32 pair_ant=0;

        /* tx_msg_data初期化 */
        memset(&tx_msg_data, 0, sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ));

        /* read_data初期化 */
        memset(&read_data, 0 , sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REJ));

        /* saa_tx_car_tblからtx_car_name検索 */
        rtn_saa = i_o_ran_uplane_conf_check_tx_car_name_from_tx_car_tbl(tx_car_name, &tx_car_num);
        if(rtn_saa == D_SYS_NG) {
            if (LOGWARN) {
                log_warn("\n%s: Failed to get name from tx_car_tbl, name=%s", __func__, tx_car_name);
            }
            return ERR_NCX_OPERATION_FAILED;
        }

        /* link_info_tblからtx_car_name検索 */
        switch(saa_link_info_tbl[dir][link_info_num[0]].ant){
            case SAA_UPCONF_ANT_A:
                pair_ant = SAA_UPCONF_ANT_C;
                break;
            case SAA_UPCONF_ANT_B:
                pair_ant = SAA_UPCONF_ANT_D;
                break;
            case SAA_UPCONF_ANT_C:
                pair_ant = SAA_UPCONF_ANT_A;
                break;
            case SAA_UPCONF_ANT_D:
                pair_ant = SAA_UPCONF_ANT_B;
                break;
            default :
                log_warn("\n%s: Failed to get name from tx_car_tbl, name=%s ant=%d", __func__, tx_car_name, saa_link_info_tbl[dir][link_info_num[0]].ant);
                break;
        }

        rtn_saa = i_o_ran_uplane_conf_check_car_name_from_link_info_antnum_tbl(dir, tx_car_name,pair_ant ,link_info_num, &link_info_cnt);
        if(rtn_saa == D_SYS_NG) {
            if (LOGWARN) {
                log_warn("\n%s: Failed to get name from link_info_tbl, name = %s", __func__, tx_car_name);
            }
            return ERR_NCX_OPERATION_FAILED;
        }

        /* MSI送信用のデータ設定 */
        rtn_saa = i_o_ran_uplane_conf_set_msi_data_tx(&tx_msg_data, link_info_num, tx_car_num, car_on_off, link_info_cnt);
        if(rtn_saa == D_SYS_NG) {
            if (LOGWARN) {
                log_warn("\n%s: Failed to set tx car on MSI data, name = %s", __func__, tx_car_name);
            }
            return ERR_NCX_OPERATION_FAILED;
        }

        /* MSI送信 */
        response_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ, sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ), (void*)&tx_msg_data);
        if(response_id == 0) {
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: cannot add a request.", __func__);
            }
            return ERR_NCX_OPERATION_FAILED;
        }

    /* MSIレスポンス待ち */
        rtn_msi = f_saa_com_get_response(response_id,
            sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REJ),
            &message_id,
            &read_size,
            (void *)&read_data,
            D_MSG_WAIT_TIME );

    /* MSIレスポンス判定 */
        switch (rtn_msi) {
            case D_MSG_BUFFER_GET_OK:
                if(message_id != D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_CFM) {
                /* error log */
                if(LOGWARN) {
                    log_warn("\n%s: MSI failed: wrong message id. (%0x)", __func__, message_id);
                 }
                return ERR_NCX_OPERATION_FAILED;
                }
                else {
                    i_o_ran_uplane_conf_set_carrier_active_state(dir, tx_car_num, link_info_num, car_on_off);
                }
                break;
            case D_MSG_BUFFER_GET_ERR_SIZE:
             /* error log */
                if(LOGWARN) {
                    log_warn("\n%s: MSI failed: too small buffer size.", __func__);
                }
                return ERR_NCX_OPERATION_FAILED;
                break;
            case D_MSG_BUFFER_GET_ERR_TIMEOUT:
             /* error log */
                if(LOGWARN) {
                    log_warn("\n%s: MSI failed: time out.", __func__);
                }
                return ERR_NCX_OPERATION_FAILED;
                break;
            case D_MSG_BUFFER_GET_ERR_OTHER:
            default:
            /* error log */
                if(LOGWARN) {
                    log_warn("\n%s: MSI failed: other.", __func__);
                }
                return ERR_NCX_OPERATION_FAILED;
        }
    }
    return res;

} /* i_o_ran_uplane_conf_set_tx_carrier */

/*!
 * @brief   link_info_tblからcar_nameの検索を行う
 * @note    
 * @param   [in] dir
 * @param   [in] target_car_name
 * @param   [out] link_info_num
 * @param   [out] link_info_cnt
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2018/12/12 KCN)吉木
 */
int i_o_ran_uplane_conf_check_car_name_from_link_info_tbl(
    uint32 dir,
    const xmlChar *target_car_name,
    uint32 *link_info_num,
    uint32 *link_info_cnt)
{
    int res = D_SYS_NG;
    uint32 max_ssb_prach_num = 0;

    *link_info_cnt = 0;
    if (LOGINFO) {
        log_info("\nEnter %s: target=%s", __func__, target_car_name);
    }

    if (dir == SAA_UPCONF_DIR_DL) {
        max_ssb_prach_num = saa_upconf_max_ssb_num;
    }
    else {
        max_ssb_prach_num = saa_upconf_max_prach_num;
    }

    /* Sub 6でBeamforming ONの場合 */
	if((saa_upconf_beamforming_mode == SAA_UPCONF_BF_ON) && (saa_upconf_unit_type == SAA_UPCONF_UNIT_TYPE_SUB6)) {
        max_ssb_prach_num *=2;
    }

    for (uint32 i = 0; i < SAA_UPCONF_MAX_LINK_NUM; i++){
        if (!xml_strcmp((xmlChar *)saa_link_info_tbl[dir][i].array_carrier, target_car_name)) {
            link_info_num[*link_info_cnt] = i;
            (*link_info_cnt)++;
            if (LOGDEV1) {
                log_dev1("\n%s: Car name registered, link_info_num[%d]=%d", __func__, *link_info_cnt, i);
            }
            /* 1つ以上見つかったらOK */
            res = D_SYS_OK;
            if(*link_info_cnt > max_ssb_prach_num) {
                if (LOGWARN) {
                    log_warn("\n%s: Impossible transition route. link_info_cnt=%d max_ssb_prach_num=%d", __func__, *link_info_cnt, max_ssb_prach_num);
                }
                return D_SYS_NG;
            }
        }
    }

    if (res == D_SYS_NG) {
        if (LOGWARN) {
            log_warn("\n%s: Failed to get link_info_num, car_name=%s link_info_cnt=%d", __func__, target_car_name, *link_info_cnt);
        }
    }

    return res;

} /* i_o_ran_uplane_conf_check_car_name_from_link_info_tbl */


/*!
 * @brief   link_info_tblからアンテナ番号指定でcar_nameの検索を行う
 * @note    
 * @param   [in] dir
 * @param   [in] target_car_name
 * @param   [out] link_info_num
 * @param   [out] link_info_cnt
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/7/2 FJT)長谷川哲
 */
int i_o_ran_uplane_conf_check_car_name_from_link_info_antnum_tbl(
    uint32 dir,
    const xmlChar *target_car_name,
    uint32 antnum,
    uint32 *link_info_num,
    uint32 *link_info_cnt)
{
    int res = D_SYS_NG;
    uint32 max_ssb_prach_num = 0;

    *link_info_cnt = 0;
    if (LOGINFO) {
        log_info("\nEnter %s: target=%s", __func__, target_car_name);
    }

    if (dir == SAA_UPCONF_DIR_DL) {
        max_ssb_prach_num = saa_upconf_max_ssb_num;
    }
    else {
        max_ssb_prach_num = saa_upconf_max_prach_num;
    }

    for (uint32 i = 0; i < SAA_UPCONF_MAX_LINK_NUM; i++){
        if (!xml_strcmp((xmlChar *)saa_link_info_tbl[dir][i].array_carrier, target_car_name) && antnum == saa_link_info_tbl[dir][i].ant) {
            link_info_num[*link_info_cnt] = i;
            (*link_info_cnt)++;
            if (LOGDEV1) {
                log_dev1("\n%s: Car name registered, link_info_num[%d]=%d", __func__, *link_info_cnt, i);
            }
            /* 1つ以上見つかったらOK */
            res = D_SYS_OK;
            if(*link_info_cnt > max_ssb_prach_num) {
                if (LOGWARN) {
                    log_warn("\n%s: Impossible transition route. link_info_cnt=%d max_ssb_prach_num=%d", __func__, *link_info_cnt, max_ssb_prach_num);
                }
                return D_SYS_NG;
            }
        }
    }

    if (res == D_SYS_NG) {
        if (LOGWARN) {
            log_warn("\n%s: Failed to get link_info_num, car_name=%s link_info_cnt=%d", __func__, target_car_name, *link_info_cnt);
        }
    }

    return res;

} /* i_o_ran_uplane_conf_check_car_name_from_link_info_antnum_tbl */

/*!
 * @brief   MSI送信用のtxデータ設定を行う
 * @note    
 * @param   [in] link_info_num
 * @param   [in] tx_car_num
 * @param   [out] T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2018/12/12 KCN)吉木
 * @date    2019/02/19 KCN)吉木
 */
int i_o_ran_uplane_conf_set_msi_data_tx(
    T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ *tx_msg_data,
    uint32 *link_info_num,
    uint32 tx_car_num,
    E_UP_CAR_ACTIVE car_on_off,
    uint32 link_cnt)
{
    int res = D_SYS_OK;
    int dir = SAA_UPCONF_DIR_DL;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if(saa_link_info_tbl[dir][link_info_num[0]].ant == SAA_UPCONF_DEFVAL_ANT_NUM
        || saa_link_info_tbl[dir][link_info_num[0]].car == SAA_UPCONF_DEFVAL_CAR_NUM) {
        if(LOGWARN) {
            log_warn("\n%s: ant:%d car:%d NG", __func__, saa_link_info_tbl[dir][link_info_num[0]].ant, saa_link_info_tbl[dir][link_info_num[0]].car);
        }
            return D_SYS_NG;
    }

    /* mmwの場合はSCS 120kHzを設定 */
    if(saa_upconf_unit_type == SAA_UPCONF_UNIT_TYPE_MMW) {
        saa_tx_car_tbl[tx_car_num].tx_car_info.default_scs = E_UP_CAR_DEF_SCS_KHZ_120;
    }
    else if(saa_upconf_unit_type == SAA_UPCONF_UNIT_TYPE_SUB6) {
        /* Sub6の場合はSCS 30kHzを設定 */
        saa_tx_car_tbl[tx_car_num].tx_car_info.default_scs = E_UP_CAR_DEF_SCS_KHZ_30;
    }

    /* tx_array_carriersのパラメータをセット */
    memcpy(&(tx_msg_data->tx_array_car_t), &saa_tx_car_tbl[tx_car_num].tx_car_info, sizeof(T_MPSW_TX_ARRAY_CAR_T));
    
    /* ant */
    tx_msg_data->tx_array_car_t.ant = saa_link_info_tbl[dir][link_info_num[0]].ant;
    saa_tx_car_tbl[tx_car_num].tx_car_info.ant = saa_link_info_tbl[dir][link_info_num[0]].ant;

    /* car */
    tx_msg_data->tx_array_car_t.car = saa_link_info_tbl[dir][link_info_num[0]].car;
    saa_tx_car_tbl[tx_car_num].tx_car_info.car = saa_link_info_tbl[dir][link_info_num[0]].car;

    /* port */
    tx_msg_data->fh_port = saa_link_info_tbl[dir][link_info_num[0]].port;

    /* active */
    tx_msg_data->tx_array_car_t.active = car_on_off;

    /* eaxc-id */
    tx_msg_data->eaxc_id[0] = SAA_UPCONF_DEFVAL_EAXC_ID;
    tx_msg_data->eaxc_id[1] = SAA_UPCONF_DEFVAL_EAXC_ID;

    for(uint32 i=0; i<link_cnt; i++) {
        if(saa_link_info_tbl[dir][link_info_num[i]].section_type == SAA_UPCONF_ENDP_SEC_TYPE_NON_PRACHSSB) {
            tx_msg_data->eaxc_id[0] = saa_link_info_tbl[dir][link_info_num[i]].local_eaxc_id;
        }
        else { 
            tx_msg_data->eaxc_id[1] = saa_link_info_tbl[dir][link_info_num[i]].local_eaxc_id;
        }
    }

    if(LOGINFO) {
        log_info("\n%s: eaxc-id ant:%d nonSSB:%d  SSB:%d", __func__, tx_msg_data->tx_array_car_t.ant, tx_msg_data->eaxc_id[0], tx_msg_data->eaxc_id[1] );
    }

    return res;

} /* i_o_ran_uplane_conf_set_msi_data_tx */

/*!
 * @brief   rx carrier on/offのMSI送信を行う
 * @note    
 * @param   [in] rx_car_name
 * @param   [in] car_on_off_str
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 * @date    2018/12/12 KCN)吉木
 * @date    2019/03/29 KCN)吉木 SLEEP対応
 */
status_t i_o_ran_uplane_conf_set_rx_carrier(
    const xmlChar *rx_car_name,
    const xmlChar *car_on_off_str)
{
    status_t res = NO_ERR;
    int rtn_saa;
    uint32 dir = SAA_UPCONF_DIR_UL;
    uint32 link_info_num[SAA_UPCONF_MAX_NUM_PRACH_2ANT] = {0};
    uint32 rx_car_num = 0;
    uint32 link_info_cnt = 0;
    T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ rx_msg_data;
    uint32_t response_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ read_data;
    int rtn_msi = D_MSG_BUFFER_GET_OK;
    E_UP_CAR_ACTIVE car_on_off;

    if (LOGINFO) {
        log_info("\nEnter %s: %s, %s", __func__, rx_car_name, car_on_off_str);
    }

    /* 状態遷移判別 */
    if(!xml_strcmp(car_on_off_str, i_o_ran_uplane_conf_N_carrier_inactive)) {
        car_on_off = E_UP_CAR_ACTIVE_INACTIVE;
    }
    else if(!xml_strcmp(car_on_off_str, i_o_ran_uplane_conf_N_carrier_active)) {
        car_on_off = E_UP_CAR_ACTIVE_ACTIVE;
    }
    else {
        car_on_off = E_UP_CAR_ACTIVE_SLEEP;
    }

    /* rx_msg_data初期化 */
    memset(&rx_msg_data, 0, sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ));

    /* read_data初期化 */
    memset(&read_data, 0 , sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ));

    /* saa_rx_car_tblからrx_car_name検索 */
    rtn_saa = i_o_ran_uplane_conf_check_rx_car_name_from_rx_car_tbl(rx_car_name, &rx_car_num);
    if(rtn_saa == D_SYS_NG) {
        if (LOGWARN) {
            log_warn("\n%s: Failed to get name from rx_car_tbl, name=%s", __func__, rx_car_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* link_info_tblからrx_car_name検索 */
    rtn_saa = i_o_ran_uplane_conf_check_car_name_from_link_info_tbl(dir, rx_car_name, link_info_num, &link_info_cnt);
    if(rtn_saa == D_SYS_NG) {
        if (LOGWARN) {
            log_warn("\n%s: Failed to get name from link_info_tbl,  name=%s", __func__, rx_car_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* キャリアの状態を判別 */
    /* 同じ状態への遷移はNG */
    if(saa_rx_car_tbl[rx_car_num].rx_car_info.active == car_on_off) {
        if (LOGWARN) {
            log_warn("\n%s: Failed, carrier(%s) is already target state(%s)", __func__, rx_car_name, car_on_off_str);
        }
        return ERR_NCX_INVALID_VALUE;
    }
    /* INACTIVEからSLEEPへの遷移はNG */
    else if(saa_rx_car_tbl[rx_car_num].rx_car_info.active != E_UP_CAR_ACTIVE_ACTIVE && car_on_off == E_UP_CAR_ACTIVE_SLEEP) {
        if (LOGWARN) {
            log_warn("\n%s: Failed, carrier(%s) is not ACTIVE", __func__, rx_car_name);
        }
        return ERR_NCX_INVALID_VALUE;
    }
    /* SLEEPからINACTIVEへの遷移はNG */
    else if(saa_rx_car_tbl[rx_car_num].rx_car_info.active != E_UP_CAR_ACTIVE_ACTIVE && car_on_off == E_UP_CAR_ACTIVE_INACTIVE) {
        if (LOGWARN) {
            log_warn("\n%s: Failed, carrier(%s) is not ACTIVE", __func__, rx_car_name);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    if((saa_upconf_beamforming_mode == SAA_UPCONF_BF_ON) && (saa_upconf_unit_type == SAA_UPCONF_UNIT_TYPE_SUB6)) {
    /* Sub6かつBF有りの場合はlink_info_numからANT A/Bのlink_info_numのみ抽出する */
        if(saa_link_info_tbl[dir][link_info_num[0]].ant == SAA_UPCONF_ANT_A || saa_link_info_tbl[dir][link_info_num[0]].ant == SAA_UPCONF_ANT_C){
            i_o_ran_uplane_conf_check_car_name_from_link_info_antnum_tbl(dir, rx_car_name, SAA_UPCONF_ANT_A, link_info_num, &link_info_cnt);
        }
        else {
            i_o_ran_uplane_conf_check_car_name_from_link_info_antnum_tbl(dir, rx_car_name, SAA_UPCONF_ANT_B, link_info_num, &link_info_cnt);
        }
    }

    /* MSI送信用のデータ設定 */
    rtn_saa = i_o_ran_uplane_conf_set_msi_data_rx(&rx_msg_data, link_info_num, rx_car_num, car_on_off, link_info_cnt);
    if(rtn_saa == D_SYS_NG) {
        if (LOGWARN) {
            log_warn("\n%s: Failed to set rx car on MSI data, name=%s", __func__, rx_car_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* MSI送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ, sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ), (void*)&rx_msg_data);
    if(response_id == 0) {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* MSIレスポンス待ち */
    rtn_msi = f_saa_com_get_response(response_id,
        sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ),
        &message_id,
        &read_size,
        (void *)&read_data,
        D_MSG_WAIT_TIME );


    /* MSIレスポンス判定 */
    switch (rtn_msi) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id != D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_CFM) {
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: wrong message id. (%0x)", __func__, message_id);
            }
            return ERR_NCX_OPERATION_FAILED;
        }
        else {
            i_o_ran_uplane_conf_set_carrier_active_state(dir, rx_car_num, link_info_num, car_on_off);
        }
        break;
    case D_MSG_BUFFER_GET_ERR_SIZE:
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: too small buffer size.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
        break;
    case D_MSG_BUFFER_GET_ERR_TIMEOUT:
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: time out.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
        break;
    case D_MSG_BUFFER_GET_ERR_OTHER:
    default:
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: other.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }


    /* Sub6かつBF有りの場合は対応する別アンテナの設定も実施する */
    if((saa_upconf_beamforming_mode == SAA_UPCONF_BF_ON) && (saa_upconf_unit_type == SAA_UPCONF_UNIT_TYPE_SUB6)) {
        uint32 pair_ant=0;

        /* rx_msg_data初期化 */
        memset(&rx_msg_data, 0, sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ));

        /* read_data初期化 */
        memset(&read_data, 0 , sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ));

        /* saa_rx_car_tblからrx_car_name検索 */
        rtn_saa = i_o_ran_uplane_conf_check_rx_car_name_from_rx_car_tbl(rx_car_name, &rx_car_num);
        if(rtn_saa == D_SYS_NG) {
            if (LOGWARN) {
                log_warn("\n%s: Failed to get name from rx_car_tbl, name=%s", __func__, rx_car_name);
            }
            return ERR_NCX_OPERATION_FAILED;
        }

        /* link_info_tblからrx_car_name検索 */
        switch(saa_link_info_tbl[dir][link_info_num[0]].ant){
            case SAA_UPCONF_ANT_A:
                pair_ant = SAA_UPCONF_ANT_C;
                break;
            case SAA_UPCONF_ANT_B:
                pair_ant = SAA_UPCONF_ANT_D;
                break;
            case SAA_UPCONF_ANT_C:
                pair_ant = SAA_UPCONF_ANT_A;
                break;
            case SAA_UPCONF_ANT_D:
                pair_ant = SAA_UPCONF_ANT_B;
                break;
            default :
                log_warn("\n%s: Failed to get name from rx_car_tbl, name=%s ant=%d", __func__, rx_car_name, saa_link_info_tbl[dir][link_info_num[0]].ant);
                break;
         }

        /* link_info_tblからrx_car_name検索 */
        rtn_saa = i_o_ran_uplane_conf_check_car_name_from_link_info_antnum_tbl(dir, rx_car_name, pair_ant, link_info_num, &link_info_cnt);
        if(rtn_saa == D_SYS_NG) {
            if (LOGWARN) {
                log_warn("\n%s: Failed to get name from link_info_tbl,  name=%s", __func__, rx_car_name);
             }
            return ERR_NCX_OPERATION_FAILED;
        }

        /* MSI送信用のデータ設定 */
        rtn_saa = i_o_ran_uplane_conf_set_msi_data_rx(&rx_msg_data, link_info_num, rx_car_num, car_on_off, link_info_cnt);
        if(rtn_saa == D_SYS_NG) {
            if (LOGWARN) {
                log_warn("\n%s: Failed to set rx car on MSI data, name=%s", __func__, rx_car_name);
            }
            return ERR_NCX_OPERATION_FAILED;
        }

        /* MSI送信 */
        response_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ, sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ), (void*)&rx_msg_data);
        if(response_id == 0) {
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: cannot add a request.", __func__);
            }
            return ERR_NCX_OPERATION_FAILED;
        }

         /* MSIレスポンス待ち */
         rtn_msi = f_saa_com_get_response(response_id,
             sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ),
             &message_id,
             &read_size,
             (void *)&read_data,
             D_MSG_WAIT_TIME );

        /* MSIレスポンス判定 */
        switch (rtn_msi) {
        case D_MSG_BUFFER_GET_OK:
            if(message_id != D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_CFM) {
                /* error log */
                if(LOGWARN) {
                    log_warn("\n%s: MSI failed: wrong message id. (%0x)", __func__, message_id);
                }
                return ERR_NCX_OPERATION_FAILED;
            }
            else {
                i_o_ran_uplane_conf_set_carrier_active_state(dir, rx_car_num, link_info_num, car_on_off);
            }
            break;
        case D_MSG_BUFFER_GET_ERR_SIZE:
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: too small buffer size.", __func__);
            }
            return ERR_NCX_OPERATION_FAILED;
            break;
        case D_MSG_BUFFER_GET_ERR_TIMEOUT:
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: time out.", __func__);
            }
            return ERR_NCX_OPERATION_FAILED;
            break;
        case D_MSG_BUFFER_GET_ERR_OTHER:
        default:
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: other.", __func__);
            }
            return ERR_NCX_OPERATION_FAILED;
        }
    }
    return res;

} /* i_o_ran_uplane_conf_set_rx_carrier */

/*!
 * @brief   MSI送信用のrxデータ設定を行う
 * @note    
 * @param   [in] link_info_num
 * @param   [in] rx_car_num
 * @param   [out] T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2018/12/12 KCN)吉木
 */
int i_o_ran_uplane_conf_set_msi_data_rx(
    T_MPSW_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ *rx_msg_data,
    uint32 *link_info_num,
    uint32 rx_car_num,
    E_UP_CAR_ACTIVE car_on_off,
    uint32 link_cnt)
{
    int res = D_SYS_OK;
    int dir = SAA_UPCONF_DIR_UL;
    uint32 endp_info_num = 0;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if(saa_link_info_tbl[dir][link_info_num[0]].ant == SAA_UPCONF_DEFVAL_ANT_NUM
        || saa_link_info_tbl[dir][link_info_num[0]].car == SAA_UPCONF_DEFVAL_CAR_NUM) {
        if(LOGWARN) {
            log_warn("\n%s: ant:%d car:%d NG", __func__, saa_link_info_tbl[dir][link_info_num[0]].ant, saa_link_info_tbl[dir][link_info_num[0]].car);
        }
            return D_SYS_NG;
    }

    /* rx_endpointのパラメーターをセット */
    /* endpointsがsaa_endp_info_tblに未登録の場合はエラーを返す */
    res = i_o_ran_uplane_conf_check_endpoints_from_endp_info_tbl((uint32)dir, saa_link_info_tbl[dir][link_info_num[0]].endpoints, &endp_info_num);
    if(res == D_SYS_NG) {
        if(LOGWARN) {
            log_warn("\n%s: %s is not exist in low-level-rx-endpoints", __func__, saa_link_info_tbl[dir][link_info_num[0]].endpoints);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    saa_rx_car_tbl[rx_car_num].rx_car_info.ul_fft_sampling_offset = saa_endp_info_tbl[dir][endp_info_num].ul_fft_sampling_offset;

    /* mmwの場合はSCS 120kHzを設定 */
    if(saa_upconf_unit_type == SAA_UPCONF_UNIT_TYPE_MMW) {
        saa_rx_car_tbl[rx_car_num].rx_car_info.default_scs = E_UP_CAR_DEF_SCS_KHZ_120;
    }
    else if(saa_upconf_unit_type == SAA_UPCONF_UNIT_TYPE_SUB6) {
        /* Sub6の場合はSCS 30kHzを設定 */
        saa_rx_car_tbl[rx_car_num].rx_car_info.default_scs = E_UP_CAR_DEF_SCS_KHZ_30;
    }

    /* rx_array_carriersのパラメータをセット */
    memcpy(&(rx_msg_data->rx_array_car_t), &saa_rx_car_tbl[rx_car_num].rx_car_info, sizeof(T_MPSW_RX_ARRAY_CAR_T));

    /* ant */
    rx_msg_data->rx_array_car_t.ant = saa_link_info_tbl[dir][link_info_num[0]].ant;
    saa_rx_car_tbl[rx_car_num].rx_car_info.ant = saa_link_info_tbl[dir][link_info_num[0]].ant;

    /* car */
    rx_msg_data->rx_array_car_t.car = saa_link_info_tbl[dir][link_info_num[0]].car;
    saa_rx_car_tbl[rx_car_num].rx_car_info.car = saa_link_info_tbl[dir][link_info_num[0]].car;

    /* port */
    rx_msg_data->fh_port = saa_link_info_tbl[dir][link_info_num[0]].port;

    /* active */
    rx_msg_data->rx_array_car_t.active = car_on_off;

    /* eaxc-id */
    rx_msg_data->eaxc_id[0] = SAA_UPCONF_DEFVAL_EAXC_ID;
    rx_msg_data->eaxc_id[1] = SAA_UPCONF_DEFVAL_EAXC_ID;

    for(uint32 i=0; i<link_cnt; i++) {
        if(saa_link_info_tbl[dir][link_info_num[i]].section_type == SAA_UPCONF_ENDP_SEC_TYPE_NON_PRACHSSB) {
            rx_msg_data->eaxc_id[0] = saa_link_info_tbl[dir][link_info_num[i]].local_eaxc_id;
        }
        else { 
            rx_msg_data->eaxc_id[1] = saa_link_info_tbl[dir][link_info_num[i]].local_eaxc_id;
        }
    }

    if(LOGINFO) {
        log_info("\n%s: eaxc-id ant:%d nonPRACH:%d  PRACH:%d", __func__, rx_msg_data->rx_array_car_t.ant, rx_msg_data->eaxc_id[0], rx_msg_data->eaxc_id[1] );
    }

    return res;

} /* i_o_ran_uplane_conf_set_msi_data_rx */

/*!
 * @brief   local eaxc-idからant/carの割り当てを行う
 * @note    
 * @param   [in] dir
 * @param   [in] target
 * @param   [out] ant
 * @param   [out] car
 * @param   [out] local_eaxc_id_num
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/02/18 KCN)吉木
 */
int i_o_ran_uplane_conf_allocate_ant_car_from_local_eaxc_id(
    uint32 dir,
    uint16 target,
    uint32 *ant,
    uint32 *car,
    uint32 *local_eaxc_id_num)
{
    int res = D_SYS_NG;
    uint32 max_num_of_eaxc_id;
    char base_para_name[D_PARAM_NUM_MAX];
    char para_name[D_PARAM_NUM_MAX];
    char target_para[D_PARAM_NUM_MAX];
    char *stored_para;

    if (LOGINFO) {
        log_info("\nEnter %s: dir=%d, target=%x", __func__, dir, target);
    }

    snprintf(target_para, D_PARAM_NUM_MAX, "%hd", target);
    if (dir == SAA_UPCONF_DIR_DL) {
        max_num_of_eaxc_id = saa_upconf_max_ssb_num;
        snprintf(base_para_name, D_PARAM_NUM_MAX, "%s.%s.%s.%s",
            y_o_ran_uplane_conf_N_user_plane_configuration,
            y_o_ran_uplane_conf_N_low_level_tx_endpoints,
            y_o_ran_uplane_conf_N_e_axcid,
            y_o_ran_uplane_conf_N_eaxc_id);
    }
    else {
        max_num_of_eaxc_id = saa_upconf_max_prach_num;
        snprintf(base_para_name, D_PARAM_NUM_MAX, "%s.%s.%s.%s",
            y_o_ran_uplane_conf_N_user_plane_configuration,
            y_o_ran_uplane_conf_N_low_level_rx_endpoints,
            y_o_ran_uplane_conf_N_e_axcid,
            y_o_ran_uplane_conf_N_eaxc_id);
    }

    for(uint32 ant_cnt = 0; ant_cnt < saa_upconf_max_ant_num; ant_cnt++) {
        for(uint32 car_cnt = 0; car_cnt < saa_upconf_max_car_num; car_cnt++) {
            for(uint32 eaxc_cnt = 0; eaxc_cnt < max_num_of_eaxc_id; eaxc_cnt++) {
                snprintf(para_name, D_PARAM_NUM_MAX, "%s.%d.%d.%d", base_para_name, ant_cnt, car_cnt, eaxc_cnt);
                stored_para = f_saa_com_get_validate_data(i_mod_upconf, para_name);
                if (stored_para == NULL) {
                    if(LOGWARN) {
                        log_warn("\n%s: Failed to get validate data, para_name=%s", __func__, para_name);
                    }
                }
                else if (!strcmp(stored_para, target_para)) {
                    *ant = ant_cnt;
                    *car = car_cnt;
                    *local_eaxc_id_num = eaxc_cnt;
                    res = D_SYS_OK;
                    break;
                }
            }
            if (res == D_SYS_OK) {
                break;
            }
        }
        if (res == D_SYS_OK) {
            break;
        }
    }

    return res;

} /* i_o_ran_uplane_conf_allocate_ant_car_from_local_eaxc_id */

/*!
 * @brief   keyがstatic-low-level-tx/rx-endpointsに存在するか検索を行う
 * @note    
 * @param   [in] dir
 * @param   [in] target_endp_name
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/02/18 KCN)吉木
 */
int i_o_ran_uplane_conf_check_static_endpoints_name(
    uint32 dir,
    const xmlChar *target_endp_name)
{
    int res = D_SYS_NG;
    char base_para_name[D_PARAM_NUM_MAX];
    char para_name[D_PARAM_NUM_MAX];
    char *stored_para;
    uint32 max_endp_num;

    if (LOGINFO) {
        log_info("\nEnter %s: dir=%d, target_endp_name=%s", __func__, dir, target_endp_name);
    }

    if (dir == SAA_UPCONF_DIR_DL) {
        max_endp_num = saa_upconf_max_tx_endpoints_num;
        snprintf(base_para_name, D_PARAM_NUM_MAX, "%s.%s",
            y_o_ran_uplane_conf_N_user_plane_configuration,
            y_o_ran_uplane_conf_N_static_low_level_tx_endpoints);
    }
    else {
        max_endp_num = saa_upconf_max_rx_endpoints_num;
        snprintf(base_para_name, D_PARAM_NUM_MAX, "%s.%s",
            y_o_ran_uplane_conf_N_user_plane_configuration,
            y_o_ran_uplane_conf_N_static_low_level_rx_endpoints);
    }

    for (uint32 endp_num = 0; endp_num < max_endp_num; endp_num++) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s.%d.%s", base_para_name, endp_num, y_o_ran_uplane_conf_N_name);
        stored_para = f_saa_com_get_init_data(i_mod_upconf, para_name);
        if(stored_para == NULL) {
            if(LOGWARN) {
                log_warn("\n%s: Failed to get init data, para_name=%s", __func__, para_name);
            }
        }
        if(!xml_strcmp((xmlChar *)stored_para, target_endp_name)) {
            res = D_SYS_OK;
            break;
        }
    }

    return res;

} /* i_o_ran_uplane_conf_check_static_endpoints_name */

/*!
 * @brief   同一のlocal eaxc-idが既に登録済みでないか検索
 * @note    
 * @param   [in] dir
 * @param   [in] target
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/02/18 KCN)吉木
 */
int i_o_ran_uplane_conf_check_duplicated_local_eaxc_id_from_endp_info_tbl(
    uint32 dir,
    uint16 target)
{
    int res = D_SYS_OK;
    uint32 max_endp_num;

    if (LOGINFO) {
        log_info("\nEnter %s: dir=%d, target=%x", __func__, dir, target);
    }

    if (dir == SAA_UPCONF_DIR_DL) {
        max_endp_num = saa_upconf_max_tx_endpoints_num;
    }
    else {
        max_endp_num = saa_upconf_max_rx_endpoints_num;
    }

    for (uint32 endp_num = 0; endp_num < max_endp_num; endp_num++) {
        if(saa_endp_info_tbl[dir][endp_num].local_eaxc_id == target) {
            res = D_SYS_NG;
            break;
        }
    }

    return res;

} /* i_o_ran_uplane_conf_check_duplicated_local_eaxc_id_from_endp_info_tbl */

/*!
 * @brief   saa_endp_info_tblからendp_nameの検索を行う
 * @note    
 * @param   [in] dir
 * @param   [in] target_link_name
 * @param   [out] link_info_num
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/02/18 KCN)吉木
 */
int i_o_ran_uplane_conf_check_endp_name_from_endp_info_tbl(
    uint32 dir,
    const xmlChar *target_endp_name,
    uint32 *endp_info_num)
{
    int res = D_SYS_NG;
    uint32 max_endp_num;

    if (LOGINFO) {
        log_info("\nEnter %s: dir=%d, target_endp_name=%s", __func__, dir, target_endp_name);
    }

    if (dir == SAA_UPCONF_DIR_DL) {
        max_endp_num = saa_upconf_max_tx_endpoints_num;
    }
    else {
        max_endp_num = saa_upconf_max_rx_endpoints_num;
    }

    for (uint32 endp_num = 0; endp_num < max_endp_num; endp_num++){
        if (!xml_strcmp(saa_endp_info_tbl[dir][endp_num].endp_name, target_endp_name)) {
            *endp_info_num = endp_num;
            res = D_SYS_OK;
            if (LOGDEV1) {
                log_dev1("\n%s: Endpoints name registered, endp_info_num = %d", __func__, *endp_info_num);
            }
            break;
        }
    }

    if (res == D_SYS_NG) {
        if (LOGDEV1) {
            log_dev1("\n%s: Missing Endpoints name, name = %s", __func__, target_endp_name);
        }
        return D_SYS_NG;
    }

    return res;

} /* i_o_ran_uplane_conf_check_endp_name_from_endp_info_tbl */

/*!
 * @brief   saa_endp_info_tblにendp_nameの登録を行う
 * @note    
 * @param   [in] dir
 * @param   [in] target_endp_name
 * @param   [in] endp_info_num
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/02/18 KCN)吉木
 */
int i_o_ran_uplane_conf_set_endp_name_to_endp_info_tbl(
    uint32 dir,
    const xmlChar *target_endp_name,
    uint32 *endp_info_num)
{
    int res = D_SYS_NG;
    uint32 max_endp_num;

    if (LOGINFO) {
        log_info("\nEnter %s: dir=%d, target_endp_name=%s", __func__, dir, target_endp_name);
    }

    if (dir == SAA_UPCONF_DIR_DL) {
        max_endp_num = saa_upconf_max_tx_endpoints_num;
    }
    else {
        max_endp_num = saa_upconf_max_rx_endpoints_num;
    }

    for (uint32 endp_num = 0; endp_num < max_endp_num; endp_num++){
        if (LOGDEV1) {
            log_dev1("\n%s: saa_endp_info_tbl[%d][%d].endp_name = %s", __func__, dir, endp_num, saa_endp_info_tbl[dir][endp_num].endp_name);
        }
        if (saa_endp_info_tbl[dir][endp_num].endp_name[0] == '\0') {
            /* saa_endp_info_tblにendp_nameを登録 */
            xml_strcpy(saa_endp_info_tbl[dir][endp_num].endp_name, target_endp_name);
            *endp_info_num = endp_num;
            res = D_SYS_OK;
            if (LOGDEV1) {
                log_dev1("\n%s: Add endpoints name to control table, name = %s, num = %d", __func__, saa_endp_info_tbl[dir][endp_num].endp_name, endp_num);
            }
            break;
        }
    }

    if(res == D_SYS_NG){
        if(LOGWARN) {
            log_warn("\n%s: Resource denied, capacity lack of control table.", __func__);
        }
        return D_SYS_NG;
    }

    return res;

} /* i_o_ran_uplane_conf_set_endp_name_to_endp_info_tbl */

/*!
 * @brief   tx/rx endpoints eaxc-idのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] target_endp_name
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/02/18 KCN)吉木
 */
status_t i_o_ran_uplane_conf_eaxc_id_validate(
    uint32 dir,
    val_value_t *newval,
    const xmlChar *target_endp_name)
{
    status_t res = NO_ERR;
    uint16 target;
    uint32 endp_info_num = 0;
    int rtn_saa;
    uint32 ant = 0;
    uint32 car = 0;
    uint32 local_eaxc_id_num = 0;
    uint32 port = 0;
    uint32 section_type=0;
	
    if (LOGINFO) {
        log_info("\nEnter %s:, dir = %d, target_endp_name = %s", __func__, dir, target_endp_name);
    }

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = VAL_UINT16(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, target=%x", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* keyがstatic-low-level-tx/rx-endpointsに存在するか検索 */
    rtn_saa = i_o_ran_uplane_conf_check_static_endpoints_name(dir, target_endp_name);
    if (rtn_saa != D_SYS_OK) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed, endpoint(%s) is not exist in satic endpoints.", __func__, target_endp_name);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    /* 同一のlocal eaxc-idが既に登録済みでないか検索 */
    rtn_saa = i_o_ran_uplane_conf_check_duplicated_local_eaxc_id_from_endp_info_tbl(dir, target);
    if (rtn_saa != D_SYS_OK) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed, eaxc-id(%d) is already used.", __func__, target);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    /* saa_endp_info_tblからendp_name検索 */
    rtn_saa = i_o_ran_uplane_conf_check_endp_name_from_endp_info_tbl(dir, target_endp_name, &endp_info_num);

    /* saa_endp_info_tblにendp_name未登録の場合は登録する */
    if(rtn_saa == D_SYS_NG) {
        if (LOGDEV1) {
            log_dev1("\n%s: Add tx/rx endp name to control table, dir = %d name = %s", __func__, dir, target_endp_name);
        }
        rtn_saa = i_o_ran_uplane_conf_set_endp_name_to_endp_info_tbl(dir, target_endp_name, &endp_info_num);
        if(rtn_saa == D_SYS_NG) {
            if(LOGWARN) {
                log_warn("\n%s: Failed to add control table, dir = %d name = %s", __func__, dir, target_endp_name);
            }
            return ERR_NCX_RESOURCE_DENIED;
        }
    }

    /* saa_endp_info_tblのactive/sleepがONの場合は変更不可 */
    if(saa_endp_info_tbl[dir][endp_info_num].active != E_UP_CAR_ACTIVE_INACTIVE) {
        if(LOGWARN) {
            log_warn("\n%s: Failed, tx/rx endpoints(%s) has active carrier, dir = %d", __func__, target_endp_name, dir);
        }
        return ERR_NCX_RESOURCE_DENIED;
    }

    /* local eaxc-idからant/carの特定 */
    rtn_saa = i_o_ran_uplane_conf_allocate_ant_car_from_local_eaxc_id(dir, target, &ant, &car, &local_eaxc_id_num);
    if (rtn_saa != D_SYS_OK) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed, eaxc-id(%d) is not supported.", __func__, target);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    /* arrayとantの割り当て*/
    rtn_saa = i_o_ran_uplane_conf_allocate_array_name_and_ant_num(dir, ant, target_endp_name);
    if (rtn_saa != D_SYS_OK) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to allocate array name and physical ant num.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    /* portの取得 */
    rtn_saa = i_o_ran_uplane_conf_check_port_from_endp_name(dir, target_endp_name, &port);
    if (rtn_saa != D_SYS_OK) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get port.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    /* saa_endp_info_tblにlocal_eaxc_id_num, local_eaxc_id, ant/carを登録 */
    saa_endp_info_tbl[dir][endp_info_num].ant = ant;
    saa_endp_info_tbl[dir][endp_info_num].car = car;
    saa_endp_info_tbl[dir][endp_info_num].local_eaxc_id_num = local_eaxc_id_num;
    saa_endp_info_tbl[dir][endp_info_num].local_eaxc_id = target;
    saa_endp_info_tbl[dir][endp_info_num].port = port;
	
    res = i_o_ran_uplane_conf_sectiontype_get(dir, target_endp_name, &section_type);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: section type check Failed. target_endp_name=%s", __func__, target_endp_name);
        }
        return ERR_NCX_INVALID_VALUE;
    }
    saa_endp_info_tbl[dir][endp_info_num].section_type = section_type;
	
    if (LOGINFO) {
        log_info("\n%s: set saa_endp_info_tbl[%d][%d].ant=%d", __func__, dir, endp_info_num, saa_endp_info_tbl[dir][endp_info_num].ant);
        log_info("\n%s: set saa_endp_info_tbl[%d][%d].car=%d", __func__, dir, endp_info_num, saa_endp_info_tbl[dir][endp_info_num].car);
        log_info("\n%s: set saa_endp_info_tbl[%d][%d].local_eaxc_id_num=%d", __func__, dir, endp_info_num, saa_endp_info_tbl[dir][endp_info_num].local_eaxc_id_num);
        log_info("\n%s: set saa_endp_info_tbl[%d][%d].local_eaxc_id=%d", __func__, dir, endp_info_num, saa_endp_info_tbl[dir][endp_info_num].local_eaxc_id);
        log_info("\n%s: set saa_endp_info_tbl[%d][%d].port=%d", __func__, dir, endp_info_num, saa_endp_info_tbl[dir][endp_info_num].port);
        log_info("\n%s: set saa_endp_info_tbl[%d][%d].section_type=%d", __func__, dir, endp_info_num, saa_endp_info_tbl[dir][endp_info_num].section_type);
	}

    return res;

} /* i_o_ran_uplane_conf_eaxc_id_validate */

/*!
 * @brief   指定されたarray_carriersがtx/rx-array-carriersに登録済みか検索する
 * @note    
 * @param   [in] dir
 * @param   [in] array_carriers
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/02/19 KCN)吉木
 */
int i_o_ran_uplane_conf_check_array_carrier_from_car_tbl(
    uint32 dir,
    const xmlChar *array_carriers)
{
    int res = D_SYS_NG;

    if (LOGINFO) {
        log_info("\nEnter %s: dir=%d, array_carriers=%s", __func__, dir, array_carriers);
    }

    if (dir == SAA_UPCONF_DIR_DL) {
        for(uint32 i = 0; i < SAA_UPCONF_MAX_ARR_CAR_NUM; i++) {
            if (!xml_strcmp((xmlChar *)saa_tx_car_tbl[i].tx_car_name, array_carriers)) {
                res = D_SYS_OK;
                break;
            }
        }
    }
    else {
        for(uint32 i = 0; i < SAA_UPCONF_MAX_ARR_CAR_NUM; i++) {
            if (!xml_strcmp((xmlChar *)saa_rx_car_tbl[i].rx_car_name, array_carriers)) {
                res = D_SYS_OK;
                break;
            }
        }
    }

    return res;

} /* i_o_ran_uplane_conf_check_array_carrier_from_car_tbl */

/*!
 * @brief   指定されたendpointsがtx/rx-array-carriersに登録済みか検索する
 * @note    
 * @param   [in] dir
 * @param   [in] endpoints
 * @param   [out] endp_info_num
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/02/19 KCN)吉木
 */
int i_o_ran_uplane_conf_check_endpoints_from_endp_info_tbl(
    uint32 dir,
    const xmlChar *endpoints,
    uint32 *endp_info_num)
{
    int res = D_SYS_NG;

    if (LOGINFO) {
        log_info("\nEnter %s: dir=%d, endpoints=%s", __func__, dir, endpoints);
    }

    for(uint32 endp_cnt = 0; endp_cnt < SAA_UPCONF_MAX_ENDP_NUM; endp_cnt++) {
        if (!xml_strcmp((xmlChar *)saa_endp_info_tbl[dir][endp_cnt].endp_name, endpoints)) {
            res = D_SYS_OK;
            *endp_info_num = endp_cnt;
            break;
        }
    }

    return res;

} /* i_o_ran_uplane_conf_check_endpoints_from_endp_info_tbl */

/*!
 * @brief   指定されたarray_carriersがtx/rx-array-carriersに登録済みか検索する
 * @note    
 * @param   [in] dir
 * @param   [in] array_carriers
 * @param   [in] link_info_num
 * @param   [out] pair_link_info_num
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/02/19 KCN)吉木
 */
int i_o_ran_uplane_conf_check_pair_link_from_link_info_tbl(
    uint32 dir,
    const xmlChar *array_carriers,
    uint32 link_info_num,
    uint32 *pair_link_info_num)
{
    int res = D_SYS_NG;

    if (LOGINFO) {
        log_info("\nEnter %s: dir=%d, array_carriers=%s", __func__, dir, array_carriers);
    }

    for (uint32 i = 0; i < SAA_UPCONF_MAX_LINK_NUM; i++){
        if (!xml_strcmp((xmlChar *)saa_link_info_tbl[dir][i].array_carrier, array_carriers) && i != link_info_num) {
            *pair_link_info_num = i;
            res = D_SYS_OK;
            break;
        }
    }

    return res;

} /* i_o_ran_uplane_conf_check_pair_link_from_link_info_tbl */

/*!
 * @brief   link_info_tbl, endp_info_tblにキャリア状態を設定する
 * @note    
 * @param   [in] dir
 * @param   [in] car_num
 * @param   [in] link_info_num
 * @param   [in] car_on_off
 * @return  void
 * @date    2019/02/21 KCN)吉木
 */
void i_o_ran_uplane_conf_set_carrier_active_state(
    uint32 dir,
    uint32 car_num,
    uint32 *link_info_num,
    E_UP_CAR_ACTIVE car_on_off)
{
    uint32 max_ssb_prach_num = 0;

    if (LOGINFO) {
        log_info("\nEnter %s: dir=%d, car_num=%d, active=%d", __func__, dir, car_num, car_on_off);
    }

    if (dir == SAA_UPCONF_DIR_DL) {
        max_ssb_prach_num = saa_upconf_max_ssb_num;
        saa_tx_car_tbl[car_num].tx_car_info.active = car_on_off;
    }
    else {
        max_ssb_prach_num = saa_upconf_max_prach_num;
        saa_rx_car_tbl[car_num].rx_car_info.active = car_on_off;
    }

    for (uint32 i = 0; i < max_ssb_prach_num; i++){
        saa_link_info_tbl[dir][link_info_num[i]].active = car_on_off;
        saa_endp_info_tbl[dir][saa_link_info_tbl[dir][link_info_num[i]].endp_info_num].active = car_on_off;
    }

} /* i_o_ran_uplane_conf_set_carrier_active_state */

/*!
 * @brief   falut sourceに対応したdir, antを取得する
 * @note    
 * @param   [in] fault_ant
 * @param   [out] fault_source_num
 * @param   [out] fm_ant_name
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/02/21 KCN)吉木
 */
int i_o_ran_uplane_conf_get_dir_ant_from_fault_ant(
    E_FAULT_SOURCE fault_ant,
    uint32 *dir,
    uint32 *ant)
{

    if (LOGINFO) {
        log_info("\nEnter %s: fault source=%d", __func__, fault_ant);
    }

    switch (fault_ant) {
        case E_FAULT_SOURCE_TX_ANT_A:
            *dir = SAA_UPCONF_DIR_DL;
            *ant = 0;
            break;
        case E_FAULT_SOURCE_TX_ANT_B:
            *dir = SAA_UPCONF_DIR_DL;
            *ant = 1;
            break;
        case E_FAULT_SOURCE_TX_ANT_C:
            *dir = SAA_UPCONF_DIR_DL;
            *ant = 2;
            break;
        case E_FAULT_SOURCE_TX_ANT_D:
            *dir = SAA_UPCONF_DIR_DL;
            *ant = 3;
            break;
        case E_FAULT_SOURCE_TX_ANT_E:
            *dir = SAA_UPCONF_DIR_DL;
            *ant = 4;
            break;
        case E_FAULT_SOURCE_TX_ANT_F:
            *dir = SAA_UPCONF_DIR_DL;
            *ant = 5;
            break;
        case E_FAULT_SOURCE_TX_ANT_G:
            *dir = SAA_UPCONF_DIR_DL;
            *ant = 6;
            break;
        case E_FAULT_SOURCE_TX_ANT_H:
            *dir = SAA_UPCONF_DIR_DL;
            *ant = 7;
            break;
        case E_FAULT_SOURCE_RX_ANT_A:
            *dir = SAA_UPCONF_DIR_UL;
            *ant = 0;
            break;
        case E_FAULT_SOURCE_RX_ANT_B:
            *dir = SAA_UPCONF_DIR_UL;
            *ant = 1;
            break;
        case E_FAULT_SOURCE_RX_ANT_C:
            *dir = SAA_UPCONF_DIR_UL;
            *ant = 2;
            break;
        case E_FAULT_SOURCE_RX_ANT_D:
            *dir = SAA_UPCONF_DIR_UL;
            *ant = 3;
            break;
        case E_FAULT_SOURCE_RX_ANT_E:
            *dir = SAA_UPCONF_DIR_UL;
            *ant = 4;
            break;
        case E_FAULT_SOURCE_RX_ANT_F:
            *dir = SAA_UPCONF_DIR_UL;
            *ant = 5;
            break;
        case E_FAULT_SOURCE_RX_ANT_G:
            *dir = SAA_UPCONF_DIR_UL;
            *ant = 6;
            break;
        case E_FAULT_SOURCE_RX_ANT_H:
            *dir = SAA_UPCONF_DIR_UL;
            *ant = 7;
            break;
        default:
        return D_SYS_NG;
    }
    return D_SYS_OK;

} /* i_o_ran_uplane_conf_get_dir_ant_from_fault_ant */

/*!
 * @brief   指定されたendp_nameに対応するportを取得する
 * @note    
 * @param   [in] dir
 * @param   [in] target_endp_name
 * @param   [out] port
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/02/21 KCN)吉木
 */
int i_o_ran_uplane_conf_check_port_from_endp_name(
    uint32 dir,
    const xmlChar *target_endp_name,
    uint32 *port)
{
    int res = D_SYS_NG;
    xmlChar *endpoint_name_temp;
    xmlChar *interface_temp;
    char para_name[D_PARAM_NUM_MAX];
    char base_para_name[D_PARAM_NUM_MAX];

    if (LOGINFO) {
        log_info("\nEnter %s: dir=%d, endp_name=%s", __func__, dir, target_endp_name);
    }

    if(dir == SAA_UPCONF_DIR_DL) {
        snprintf(base_para_name, D_PARAM_NUM_MAX, "%s.", i_para_upconf_tx_endpoints);
    }
    else {
        snprintf(base_para_name, D_PARAM_NUM_MAX, "%s.", i_para_upconf_rx_endpoints);
    }

    /* endpointsからportを判別 */
    for(uint32 i = 0; i < SAA_UPCONF_MAX_ENDP_NUM; i++) {
        /* i番目のendpoint nameを取得する */
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%d.%s", base_para_name,  i,  i_para_upconf_name);
        endpoint_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, para_name);
        if(endpoint_name_temp == NULL) {
            if(LOGWARN) {
                log_warn("\n%s: Failed to get endpoints name, index = %d", __func__, i);
            }
            return ERR_NCX_OPERATION_FAILED;
        }
        /* endpoint nameが一致した場合はinterfaceを取得する */
        if (!xml_strcmp(target_endp_name, endpoint_name_temp)) {
            snprintf(para_name, D_PARAM_NUM_MAX, "%s%d.%s", base_para_name, i, i_para_upconf_restricted_interface);
            interface_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, para_name);
            if(interface_temp == NULL) {
                if(LOGWARN) {
                    log_warn("\n%s: Failed to get endpoints interface, index=%d", __func__, i);
                }
                return ERR_NCX_OPERATION_FAILED;
            }
            /* fhport0 */
            if(!xml_strcmp(interface_temp, i_o_ran_uplane_conf_N_name_fheth0)) {
                *port = SAA_UPCONF_FHPORT_PORT0;
                if (LOGINFO) {
                    log_info("\n%s: Set fh_port, dir=%d, endp_name=%s, port=%d", __func__, dir, target_endp_name, *port);
                }
                res = D_SYS_OK;
                break;
            }
            /* fhport1 */
            else if (!xml_strcmp(interface_temp, i_o_ran_uplane_conf_N_name_fheth1)) {
                *port = SAA_UPCONF_FHPORT_PORT1;
                if (LOGINFO) {
                    log_info("\n%s: Set fh_port, dir=%d, endp_name=%s, port=%d", __func__, dir, target_endp_name, *port);
                }
                res = D_SYS_OK;
                break;
            }
        }
    }
    return res;

} /* i_o_ran_uplane_conf_check_port_from_endp_name */

/*!
 * @brief   tx state changeを送信する
 * @note    
 * @param   [in] data
 * @param   [in] size
 * @return  void
 * @date    2019/02/22 KCN)吉木
 */
void i_o_ran_uplane_conf_tx_array_carriers_state_change_handler(
    void *data,
    uint32_t size)
{

    if (LOGINFO) {
        log_info("\nEnter %s: size=%d", __func__, size);
    }

    memcpy(&m_tx_notification_data, data,sizeof(m_tx_notification_data));
    u_o_ran_uplane_conf_tx_array_carriers_state_change_send(NULL);

} /* i_o_ran_uplane_conf_tx_array_carriers_state_change_handler */

/*!
 * @brief   rx state changeを送信する
 * @note    
 * @param   [in] data
 * @param   [in] size
 * @return  void
 * @date    2019/02/22 KCN)吉木
 */
void i_o_ran_uplane_conf_rx_array_carriers_state_change_handler(
    void *data,
    uint32_t size)
{

    if (LOGINFO) {
        log_info("\nEnter %s: size=%d", __func__, size);
    }

    memcpy(&m_rx_notification_data, data,sizeof(m_rx_notification_data));
    u_o_ran_uplane_conf_rx_array_carriers_state_change_send(NULL);

} /* i_o_ran_uplane_conf_rx_array_carriers_state_change_handler */

/*!
 * @brief   notificationデータを取得する。(tx)
 * @note    
 * @param   なし
 * @return  データ
 * @date    2019/12/23 KCN)中井
 */
T_MPSW_MSI_O_RAN_UPLANE_CONF_TX_STATE_CHANGE_NTF_IND *
    i_o_ran_uplane_conf_get_tx_notification_data(void){
    return &m_tx_notification_data;
}

/*!
 * @brief   notificationデータを取得する。(rx)
 * @note    
 * @param   なし
 * @return  データ
 * @date    2019/12/23 KCN)中井
 */
T_MPSW_MSI_O_RAN_UPLANE_CONF_RX_STATE_CHANGE_NTF_IND *
    i_o_ran_uplane_conf_get_rx_notification_data(void){
    return &m_rx_notification_data;
}

/*!
 * @brief   ant/carからtx/rx arraycarrierを取得する
 * @note    
 * @param   [in] ant
 * @param   [in] car
 * @param   [out] array_carrier
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2019/02/22 KCN)吉木
 */
status_t i_o_ran_uplane_conf_get_array_carrier_from_ant_car(
    uint32 dir,
    uint32 ant,
    uint32 car,
    xmlChar *array_carrier)
{

    status_t res = ERR_NCX_DATA_MISSING;

    if (LOGINFO) {
        log_info("\nEnter %s: dir=%d, ant=%d, car=%d", __func__, dir, ant, car);
    }

    for(uint32 i = 0; i < SAA_UPCONF_MAX_ARR_CAR_NUM; i++) {
        if(dir == SAA_UPCONF_DIR_DL
            && ant == saa_tx_car_tbl[i].tx_car_info.ant
            && car == saa_tx_car_tbl[i].tx_car_info.car) {
                xml_strcpy(array_carrier, saa_tx_car_tbl[i].tx_car_name);
                res = NO_ERR;
                break;
        }
        else if(dir == SAA_UPCONF_DIR_UL
            && ant == saa_rx_car_tbl[i].rx_car_info.ant
            && car == saa_rx_car_tbl[i].rx_car_info.car) {
                xml_strcpy(array_carrier, saa_rx_car_tbl[i].rx_car_name);
                res = NO_ERR;
                break;
        }
    }

    return res;

} /* i_o_ran_uplane_conf_get_array_carrier_from_ant_car */

/*!
 * @brief   stateをenumからxmlCharに変換する
 * @note    
 * @param   [in] state_enum
 * @param   [out] state
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 * @date    2019/02/25 KCN)吉木
 */
status_t i_o_ran_uplane_conf_conv_state_enum_to_xmlchar(
    E_RFCP_UPCNF_STS state_enum,
    xmlChar **state)
{

    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s: state_enum=%d", __func__, state_enum);
    }

    if(state_enum == E_RFCP_UPCNF_STS_DISABLE) {
        *state = (xmlChar *)i_para_upconf_state_disable;
    }
    else if (state_enum == E_RFCP_UPCNF_STS_BUSY) {
        *state = (xmlChar *)i_para_upconf_state_busy;
    }
    else if (state_enum == E_RFCP_UPCNF_STS_READY) {
        *state = (xmlChar *)i_para_upconf_state_ready;
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Failed to conv state enum", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    return res;

} /* i_o_ran_uplane_conf_conv_state_enum_to_xmlchar */

/*!
 * @brief   array nameと物理アンテナ番号を割り当てる
 * @note    
 * @param   [in] dir
 * @param   [in] ant
 * @param   [in] target_endp_name
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/03/03 KCN)吉木
 */
int i_o_ran_uplane_conf_allocate_array_name_and_ant_num(
    uint32 dir,
    uint32 ant,
    const xmlChar *target_endp_name)
{
    int res = D_SYS_NG;
    xmlChar *endpoint_name_temp;
    xmlChar *array_temp;
    char para_name[D_PARAM_NUM_MAX];
    char base_para_name[D_PARAM_NUM_MAX];

    if (LOGINFO) {
        log_info("\nEnter %s: dir=%d, endp_name=%s", __func__, dir, target_endp_name);
    }

    if(dir == SAA_UPCONF_DIR_DL) {
        snprintf(base_para_name, D_PARAM_NUM_MAX, "%s.", i_para_upconf_tx_endpoints);
    }
    else {
        snprintf(base_para_name, D_PARAM_NUM_MAX, "%s.", i_para_upconf_rx_endpoints);
    }

    /* mutex lock (mutex_for_upconf_array_ant_link_tbl) */
    pthread_mutex_lock(&mutex_for_upconf_array_ant_link_tbl);

    /* endpointsからportを判別 */
    for(uint32 i = 0; i < SAA_UPCONF_MAX_ENDP_NUM; i++) {
        /* i番目のendpoint nameを取得する */
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%d.%s", base_para_name,  i,  i_para_upconf_name);
        endpoint_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, para_name);
        if(endpoint_name_temp == NULL) {
            if(LOGWARN) {
                log_warn("\n%s: Failed to get endpoints name, index = %d", __func__, i);
            }
            res = D_SYS_NG;
            break;
        }
        /* endpoint nameが一致した場合はarrayを取得する */
        if (!xml_strcmp(target_endp_name, endpoint_name_temp)) {
            snprintf(para_name, D_PARAM_NUM_MAX, "%s%d.%s", base_para_name, i, i_para_upconf_array);
            array_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, para_name);
            if(array_temp == NULL) {
                if(LOGWARN) {
                    log_warn("\n%s: Failed to get endpoints array, index=%d", __func__, i);
                }
                res = D_SYS_NG;
                break;
            }

            res = i_o_ran_uplane_conf_array_ant_validate(dir, ant, (const xmlChar *)array_temp);
            if(res != D_SYS_OK) {
                if(LOGWARN) {
                    log_warn("\n%s: physical ant#%d(dir=%d) is not allowed to allocate to %s", __func__, ant, dir, array_temp);
                }
                res = D_SYS_NG;
                break;
            }

            if(saa_array_ant_link_tbl[dir][ant].array_name[0]=='\0') {
                xml_strcpy(saa_array_ant_link_tbl[dir][ant].array_name, array_temp);
                if (LOGINFO) {
                    log_info("\n%s: Set array name, saa_array_ant_link_tbl[%d][%d].array_name=%s", __func__, dir, ant, saa_array_ant_link_tbl[dir][ant].array_name);
                }
                res = D_SYS_OK;
                break;
            }
            else if(!xml_strcmp(saa_array_ant_link_tbl[dir][ant].array_name, array_temp)){
                res = D_SYS_OK;
                break;
            }
            else {
                if(LOGWARN) {
                    log_warn("\n%s: physhical ant#%d(dir=%d) is already allocated to %s", __func__, ant, dir, saa_array_ant_link_tbl[dir][ant].array_name);
                }
                res = D_SYS_NG;
                break;
            }
        }
    }

    /* mutex unlock (mutex_for_upconf_array_ant_link_tbl) */
    pthread_mutex_unlock(&mutex_for_upconf_array_ant_link_tbl);

    return res;

} /* i_o_ran_uplane_conf_allocate_array_name_and_ant_num */

/*!
 * @brief   指定されたarray nameが指定されたphysical antに割り当て可能かvalidateする
 * @note    
 * @param   [in] dir
 * @param   [in] ant
 * @param   [in] array_temp
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/03/04 KCN)吉木
 */
int i_o_ran_uplane_conf_array_ant_validate(
    uint32 dir,
    uint32 ant,
    const xmlChar *array_temp)
{
    int ret = D_SYS_OK;
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    char base_para_name[D_PARAM_NUM_MAX];

    if (LOGINFO) {
        log_info("\nEnter %s: dir=%d, array_temp=%s", __func__, dir, array_temp);
    }

    if(dir == SAA_UPCONF_DIR_DL) {
        snprintf(base_para_name, D_PARAM_NUM_MAX, "%s.%s", i_para_upconf_tx_arrays, i_para_upconf_name);
    }
    else {
        snprintf(base_para_name, D_PARAM_NUM_MAX, "%s.%s", i_para_upconf_rx_arrays, i_para_upconf_name);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%s.%d",base_para_name, ant);

    res = i_o_ran_uplane_conf_user_plane_configuration_validate_list(para_name, array_temp);
    if(res != NO_ERR) {
        ret = D_SYS_NG;
    }

    return ret;

} /* i_o_ran_uplane_conf_array_ant_validate */

/*!
 * @brief   fault sourceに対応したarray nameを取得する
 * @note    
 * @param   [in] fault_source
 * @param   [out] fault_source_str
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/03/04 KCN)吉木
 */
int i_o_ran_uplane_conf_get_array_name_from_fm_info(
    E_FAULT_SOURCE fault_source,
    xmlChar *fault_source_str)
{
    int res = D_SYS_OK;
    uint32 dir;
    uint32 ant;

    if (LOGINFO) {
        log_info("\nEnter %s: fault source=%d", __func__, fault_source);
    }

    res = i_o_ran_uplane_conf_get_dir_ant_from_fault_ant(fault_source, &dir, &ant);
    if (res != D_SYS_OK) {
        return res;
    }

    res = i_o_ran_uplane_conf_get_link_name_from_ant(dir, ant, fault_source_str);
    if (res != D_SYS_OK) {
        return res;
    }

    return res;


} /* i_o_ran_uplane_conf_get_array_name_from_fm_info */

/*!
 * @brief   dir, antに対応したarray nameを取得する
 * @note    
 * @param   [in] fault_source
 * @param   [out] fault_link_num
 * @param   [out] fm_ant_name
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/02/21 KCN)吉木
 */
int i_o_ran_uplane_conf_get_link_name_from_ant(
    uint32 dir,
    uint32 ant,
    xmlChar *fault_source_str)
{
    int res = D_SYS_NG;

    if (LOGINFO) {
        log_info("\nEnter %s: dir=%d, ant=%d", __func__, dir, ant);
    }

    /* mutex lock (mutex_for_upconf_array_ant_link_tbl) */
    pthread_mutex_lock(&mutex_for_upconf_array_ant_link_tbl);

    if(ant < SAA_UPCONF_MAX_ANT_NUM) {
        if(saa_array_ant_link_tbl[dir][ant].array_name[0] != '\0') {
            xml_strcpy(fault_source_str, saa_array_ant_link_tbl[dir][ant].array_name);
            res = D_SYS_OK;
        }
    }

    /* mutex unlock (mutex_for_upconf_array_ant_link_tbl) */
    pthread_mutex_unlock(&mutex_for_upconf_array_ant_link_tbl);

    return res;

} /* i_o_ran_uplane_conf_get_link_name_from_ant */


/*!
 * @brief   tx/rx endpoints section-typeを取得する
 * @note    
 * @param   [in] dir
 * @param   [in] target_endp_name
 * @param   [out] section_type
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/07/10 FJT)長谷川哲
 */
status_t i_o_ran_uplane_conf_sectiontype_get(
    uint32 dir,
    const xmlChar *target_endp_name,
    uint32 *section_type)
{
    status_t res = NO_ERR;
    uint32 curindex;
    uint32 endp_type;
    char para_name[D_PARAM_NUM_MAX];
    char *para_temp;

    if (LOGINFO) {
        log_info("\nEnter %s:, dir = %d, target_endp_name = %s", __func__, dir, target_endp_name);
    }

    /* endpoint type/section-typeを取得 */
    if(dir == SAA_UPCONF_DIR_DL) {
        i_o_ran_uplane_conf_user_plane_configuration_tx_endpoints_get_key_num(target_endp_name, &curindex);
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_upconf_tx_endpoints, ".", curindex, ".", i_para_upconf_endpoint_type);
        para_temp = (char *)f_saa_com_get_init_data(i_mod_upconf, para_name);
    }
    else {
        i_o_ran_uplane_conf_user_plane_configuration_rx_endpoints_get_key_num(target_endp_name, &curindex);
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_upconf_rx_endpoints, ".", curindex, ".", i_para_upconf_endpoint_type);
        para_temp = (char *)f_saa_com_get_init_data(i_mod_upconf, para_name);
    }

    if(para_temp == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get param, para=%s", __func__, para_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    endp_type = (uint32)atoi(para_temp);
    snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%s%s%d%s%s", i_para_o_ran_uplane_conf, ".", i_para_upconf_endpoint_types, ".", endp_type, ".", i_para_upconf_conf_section_type);
    para_temp = (char *)f_saa_com_get_init_data(i_mod_upconf, para_name);

    if(para_temp == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get param, para=%s", __func__, para_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    *section_type = (uint32)atoi(para_temp);

    return res;

} /* i_o_ran_uplane_conf_sectiontype_get */


/*!
 * @brief   nameをvalidateする
 * @note    
 * @param   [in] name
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_LEN_EXCEEDED
 * @date    2019/07/10 FJT)長谷川哲
 */
status_t i_o_ran_uplane_conf_name_validate(
    const xmlChar *target_name)
{
    status_t res = NO_ERR;

    /* 引数チェック */
    if(!target_name) {
        if(LOGWARN) {
            log_warn("\n%s:Data Missing. (target_name)", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    if(xml_strlen(target_name) > D_PARAM_NUM_MAX -1 ){
        res = ERR_NCX_LEN_EXCEEDED;
    }
    return res;

} /* i_o_ran_uplane_conf_name_validate */
