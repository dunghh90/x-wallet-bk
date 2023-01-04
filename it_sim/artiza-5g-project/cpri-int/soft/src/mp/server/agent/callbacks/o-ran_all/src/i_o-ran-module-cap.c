/*
 * @file    i_o-ran-module-cap.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_MODULE_CAP
 * @brief       SAA o-ran-module-cap
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "u_o-ran-module-cap.h"
#include "i_o-ran-module-cap.h"
#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"


/********************************************************************************************************************/
/*  static valuable                                                                                                 */
/********************************************************************************************************************/
static char i_mod_modcap[]                               = i_mod_o_ran_module_cap;
static char i_para_modcap_band_capabilities[]            = i_para_o_ran_module_cap_band_capabilities;
static char i_para_modcap_band_number[]                  = i_para_o_ran_module_cap_band_number;
static char i_para_modcap_max_supported_frequency_dl[]   = i_para_o_ran_module_cap_max_supported_frequency_dl;
static char i_para_modcap_min_supported_frequency_dl[]   = i_para_o_ran_module_cap_min_supported_frequency_dl;
static char i_para_modcap_max_supported_bandwidth_dl[]   = i_para_o_ran_module_cap_max_supported_bandwidth_dl;
static char i_para_modcap_max_num_carriers_dl[]          = i_para_o_ran_module_cap_max_num_carriers_dl;
static char i_para_modcap_max_carrier_bandwidth_dl[]     = i_para_o_ran_module_cap_max_carrier_bandwidth_dl;
static char i_para_modcap_min_carrier_bandwidth_dl[]     = i_para_o_ran_module_cap_min_carrier_bandwidth_dl;
static char i_para_modcap_max_supported_frequency_ul[]   = i_para_o_ran_module_cap_max_supported_frequency_ul;
static char i_para_modcap_min_supported_frequency_ul[]   = i_para_o_ran_module_cap_min_supported_frequency_ul;
static char i_para_modcap_max_supported_bandwidth_ul[]   = i_para_o_ran_module_cap_max_supported_bandwidth_ul;
static char i_para_modcap_max_num_carriers_ul[]          = i_para_o_ran_module_cap_max_num_carriers_ul;
static char i_para_modcap_max_carrier_bandwidth_ul[]     = i_para_o_ran_module_cap_max_carrier_bandwidth_ul;
static char i_para_modcap_min_carrier_bandwidth_ul[]     = i_para_o_ran_module_cap_min_carrier_bandwidth_ul;
static char i_para_modcap_max_num_component_carriers[]   = i_para_o_ran_module_cap_max_num_component_carriers;
static char i_para_modcap_max_num_bands[]                = i_para_o_ran_module_cap_max_num_bands;
static char i_para_modcap_max_num_sectors[]              = i_para_o_ran_module_cap_max_num_sectors;
static char i_para_modcap_max_power_per_antenna[]        = i_para_o_ran_module_cap_max_power_per_antenna;
static char i_para_modcap_min_power_per_antenna[]        = i_para_o_ran_module_cap_min_power_per_antenna;
static char i_para_modcap_codebook_configuration_ng[]    = i_para_o_ran_module_cap_codebook_configuration_ng;
static char i_para_modcap_codebook_configuration_n1[]    = i_para_o_ran_module_cap_codebook_configuration_n1;
static char i_para_modcap_codebook_configuration_n2[]    = i_para_o_ran_module_cap_codebook_configuration_n2;

static char i_mod_module_cap_char[] = i_mod_o_ran_module_cap;

static char i_para_modcap_max_scs_a_num_dl[]             = i_para_o_ran_module_cap_max_scs_a_num_dl;
static char i_para_modcap_max_scs_b_num_dl[]             = i_para_o_ran_module_cap_max_scs_b_num_dl;
static char i_para_modcap_max_scs_a_num_ul[]             = i_para_o_ran_module_cap_max_scs_a_num_ul;
static char i_para_modcap_max_scs_b_num_ul[]             = i_para_o_ran_module_cap_max_scs_b_num_ul;
static char i_para_modcap_max_band_cap_num[]             = i_para_o_ran_module_cap_max_band_cap_num;


uint32       saa_upconf_max_scs_a_num_dl;
uint32       saa_upconf_max_scs_b_num_dl;
uint32       saa_upconf_max_scs_a_num_ul;
uint32       saa_upconf_max_scs_b_num_ul;
uint32       saa_upconf_max_band_cap_num;



/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   各種パラメータの初期化を行う
 * @note    
 * @param   [in] void
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/12/08 KCN)吉木
 */
status_t i_o_ran_module_cap_init(void)
{
    status_t res = NO_ERR;
    char *para_temp;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* scs_a(dl)の初期化 */
    para_temp = f_saa_com_get_init_data(i_mod_modcap, i_para_modcap_max_scs_a_num_dl);
    if(para_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("¥n%s: Operation Failed to get param, modcap para = %s", __func__, i_para_modcap_max_scs_a_num_dl);
        }
        return res;
    }
    sscanf (para_temp, "%d", &saa_upconf_max_scs_a_num_dl);
    if (LOGDEV1) {
        log_dev1("¥n%s: saa_upconf_max_scs_a_num_dl = %d", __func__, saa_upconf_max_scs_a_num_dl);
    }


    /* scs_b(dl)の初期化 */
    para_temp = f_saa_com_get_init_data(i_mod_modcap, i_para_modcap_max_scs_b_num_dl);
    if(para_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("¥n%s: Operation Failed to get param, modcap para = %s", __func__, i_para_modcap_max_scs_b_num_dl);
        }
        return res;
    }
    sscanf (para_temp, "%d", &saa_upconf_max_scs_b_num_dl);
    if (LOGDEV1) {
        log_dev1("¥n%s saa_upconf_max_scs_b_num_dl = %d", __func__, saa_upconf_max_scs_b_num_dl);
    }

    /* scs_a(ul)の初期化 */
    para_temp = f_saa_com_get_init_data(i_mod_modcap, i_para_modcap_max_scs_a_num_ul);
    if(para_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("¥n%s: Operation Failed to get param, modcap para = %s", __func__, i_para_modcap_max_scs_a_num_ul);
        }
        return res;
    }
    sscanf (para_temp, "%d", &saa_upconf_max_scs_a_num_ul);
    if (LOGDEV1) {
        log_dev1("¥n%s: saa_upconf_max_scs_a_num_ul = %d", __func__, saa_upconf_max_scs_a_num_ul);
    }

    /* scs_b(ul)の初期化 */
    para_temp = f_saa_com_get_init_data(i_mod_modcap, i_para_modcap_max_scs_b_num_ul);
    if(para_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("¥n%s: Operation Failed to get param, modcap para = %s", __func__, i_para_modcap_max_scs_b_num_ul);
        }
        return res;
    }
    sscanf (para_temp, "%d", &saa_upconf_max_scs_b_num_ul);
    if (LOGDEV1) {
        log_dev1("¥n%s: saa_upconf_max_scs_b_num_ul = %d", __func__, saa_upconf_max_scs_b_num_ul);
    }

    /* band_cap_numの初期化 */
    para_temp = f_saa_com_get_init_data(i_mod_modcap, i_para_modcap_max_band_cap_num);
    if(para_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("¥n%s: Operation Failed to get param, modcap para = %s", __func__, i_para_modcap_max_band_cap_num);
        }
        return res;
    }
    sscanf (para_temp, "%d", &saa_upconf_max_band_cap_num);
    if (LOGDEV1) {
        log_dev1("¥n%s: saa_upconf_max_band_cap_num = %d", __func__, saa_upconf_max_band_cap_num);
    }

    return res;

} /* i_o_ran_module_cap_init */

/*!
 * @brief   band capabilitiesのkeyからkey番号の取得を行う
 * @note    
 * @param   [in] target_key_name
 * @param   [out] curindex
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/29 KCN)吉木
 */
status_t i_o_ran_module_cap_module_capability_band_capabilities_get_key_num(
    const xmlChar *target_key_name,
    uint32 *curindex)
{
    status_t res = ERR_NCX_DATA_MISSING;
    xmlChar *band_number;
    char para_name[D_PARAM_NUM_MAX];

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    for(uint32 i = 0; i < saa_upconf_max_band_cap_num; i++) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", i, ".",  i_para_modcap_band_number);
        band_number = (xmlChar *)f_saa_com_get_init_data(i_mod_modcap, para_name);
        if (band_number == NULL) {
            if(LOGWARN) {
                log_warn("¥n%s: Data Missing, modcap band-number = %s", __func__, target_key_name);
            }
            return res;
        }
        else if (!xml_strcmp(band_number, target_key_name)) {
            *curindex = i;
            if (LOGDEV1) {
                log_dev1("¥n%s: Get modcap band-cap index, num = %d", __func__, i);
            }
            res = NO_ERR;
            break;
        }
    }

    return res;

}/* i_o_ran_module_cap_module_capability_band_capabilities_get_key_num */

/*!
 * @brief   band capabilitiesの先頭のkey取得を行う
 * @note    
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/29 KCN)吉木
 */
status_t i_o_ran_module_cap_module_capability_band_capabilities_get_first_key(
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", 0, ".",  i_para_modcap_band_number);

    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_modcap, para_name);
    if(*key_name_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("¥n%s: Operation Failed to get first key, modcap band-cap", __func__);
        }
    }

    return res;

}/* i_o_ran_module_cap_module_capability_band_capabilities_get_first_key */

/*!
 * @brief   band capabilitiesの指定されたkey取得を行う
 * @note    
 * @param   [in] curindex
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/29 KCN)吉木
 */
status_t i_o_ran_module_cap_module_capability_band_capabilities_get_target_key(
    uint32 curindex,
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_band_number);

    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_modcap, para_name);
    if(*key_name_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("¥n%s: Operation Failed to get target key, modcap band-cap index = %d", __func__, curindex);
        }
    }

    return res;

}/* i_o_ran_module_cap_module_capability_band_capabilities_get_target_key */

/*!
 * @brief   band capabilitiesの指定されたパラメータ取得を行う
 * @note    
 * @param   [in] curindex
 * @param   [in] target_para_name
 * @param   [out] para_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/29 KCN)吉木
 */
status_t i_o_ran_module_cap_module_capability_band_capabilities_get_para(
    uint32 curindex,
    const xmlChar *target_para_name,
    xmlChar **para_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if(!xml_strcmp(y_o_ran_module_cap_N_max_supported_frequency_dl, target_para_name)){
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_max_supported_frequency_dl);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_min_supported_frequency_dl, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_min_supported_frequency_dl);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_max_supported_bandwidth_dl, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_max_supported_bandwidth_dl);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_max_num_carriers_dl, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_max_num_carriers_dl);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_max_carrier_bandwidth_dl, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_max_carrier_bandwidth_dl);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_min_carrier_bandwidth_dl, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_min_carrier_bandwidth_dl);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_max_supported_frequency_ul, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_max_supported_frequency_ul);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_min_supported_frequency_ul, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_min_supported_frequency_ul);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_max_supported_bandwidth_ul, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_max_supported_bandwidth_ul);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_max_num_carriers_ul, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_max_num_carriers_ul);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_max_carrier_bandwidth_ul, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_max_carrier_bandwidth_ul);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_min_carrier_bandwidth_ul, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_min_carrier_bandwidth_ul);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_max_num_component_carriers, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_max_num_component_carriers);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_max_num_bands, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_max_num_bands);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_max_num_sectors, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_max_num_sectors);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_max_power_per_antenna, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_max_power_per_antenna);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_min_power_per_antenna, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_min_power_per_antenna);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_codebook_configuration_ng, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_codebook_configuration_ng);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_codebook_configuration_n1, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_codebook_configuration_n1);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_codebook_configuration_n2, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_modcap_band_capabilities, ".", curindex, ".",  i_para_modcap_codebook_configuration_n2);
    }
    else {
        if(LOGWARN) {
            log_warn("¥n%s: Data Missing, modcap band-cap para = %s", __func__, target_para_name);
        }
        res = ERR_NCX_DATA_MISSING;
        return res;
    }

    *para_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_modcap, para_name);
    if(*para_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("¥n%s: Operation Failed to get param, modcap band-cap para = %s", __func__, para_name);
        }
    }

    return res;

} /* i_o_ran_module_cap_module_capability_band_capabilities_get_para */

/*!
 * @brief   static-properties取得関数
 * @note    static-propertiesを取得する
 * @param   [in]  *k_hardware_component_nameポインタ
 * @        [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)上本
 */
status_t i_o_ran_module_cap_module_capability_ru_capabilities_get_para( unsigned int w_enum_beam,
                                                                        xmlChar **para_temp )
{
    if (LOGINFO) {
        log_info("¥nEnter %s: w_enum_beam = %d", __func__, w_enum_beam);
    }

    status_t        res = NO_ERR ;
    char            mod_name[]  = i_mod_o_ran_module_cap;
    char            para_catg[] = i_para_module_capabilities_category;
    char            para_rprt[] = i_para_module_capabilities_ruport;
    char            para_mxpw[] = i_para_module_capabilities_max_pwr;
    char            para_mnpw[] = i_para_module_capabilities_min_pwr;
    char            para_sopt[] = i_para_module_capabilities_slt_opt;
    char            para_noss[] = i_para_module_capabilities_num_of_spatial_streams;
    char            para_ensv[] = i_para_module_capabilities_energy_saving;
    char            para_dytr[] = i_para_module_capabilities_dynamic_transport;

    switch( w_enum_beam )
    {
        case E_MODULE_CAPA_CATEGORY:            /* y_o_ran_module_cap_N_ru_supported_category */
            *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_catg) ;
            break;
        case E_MODULE_CAPA_RU_PORT:             /* y_o_ran_module_cap_N_number_of_ru_ports */
            *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_rprt);
            break;
        case E_MODULE_CAPA_MAX_PWR:             /* y_o_ran_module_cap_N_max_power_per_pa_antenna */
            *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_mxpw);
            break;
        case E_MODULE_CAPA_MIN_PWR:             /* y_o_ran_module_cap_N_min_power_per_pa_antenna */
            *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_mnpw);
            break;
        case E_MODULE_CAPA_SPLT_OPT:            /* y_o_ran_module_cap_N_fronthaul_split_option */
            *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_sopt);
            break;
        case E_MODULE_CAPA_NUM_SPA_STREAM:      /* y_o_ran_module_cap_N_number_of_spatial_streams */
            *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_noss);
            break;
        case E_MODULE_CAPA_ENERGY_SAVING:       /* y_o_ran_module_cap_N_energy_saving_by_transmission_blanks */
            *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_ensv);
            break;
        case E_MODULE_CAPA_DYNAMIC_TRANS:       /* y_o_ran_module_cap_N_dynamic_transport_delay_management_supported */
            *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_dytr);
            break;
        default:
            res = ERR_NCX_OPERATION_FAILED;
            return res;
    }
    if(*para_temp == NULL)
    {
        res = ERR_NCX_OPERATION_FAILED;
    }

    return res ;
}   /* i_o_ran_module_cap_module_capability_ru_capabilities_get_para */

/*!
 * @brief   static-properties取得関数
 * @note    static-propertiesを取得する
 * @param   [in]  *k_hardware_component_nameポインタ
 * @        [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)上本
 */
status_t i_o_ran_module_cap_module_capability_ru_capabilities_format_of_iq_sample_get_bool( unsigned int w_enum_smpl ,
                                                                                            xmlChar **para_temp )
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    status_t        res = NO_ERR ;
    char            mod_name[]      = i_mod_o_ran_module_cap ;
    char            para_cmpsup[]   = i_para_module_capabilities_cmp_sup ;
    char            para_bwtsup[]   = i_para_module_capabilities_bit_wth_sup ;
    char            para_symsup[]   = i_para_module_capabilities_sym_sup ;

    switch( w_enum_smpl )
    {
        case E_IQ_SAMPLE_CMPSUP:                    /* y_o_ran_module_cap_N_dynamic_compression_supported                   */
            *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_cmpsup) ;
            break;
        case E_IQ_SAMPLE_BWTSUP:                    /* y_o_ran_module_cap_N_realtime_variable_bit_width_supported           */
            *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_bwtsup) ;
            break;
        case E_IQ_SAMPLE_SYMSUP:                    /* y_o_ran_module_cap_N_syminc_supported                                */
            *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_symsup);
            break;
        default:
            res = ERR_NCX_OPERATION_FAILED;
            return res ;
    }

    if(*para_temp == NULL)
    {
        res = ERR_NCX_OPERATION_FAILED;
    }
    return res ;
}   /* i_o_ran_module_cap_module_capability_ru_capabilities_format_of_iq_sample_get_bool */

/*!
 * @brief   static-properties取得関数
 * @note    static-propertiesを取得する
 * @param   [in]  *k_hardware_component_nameポインタ
 * @        [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)上本
 */
status_t i_o_ran_module_cap_module_capability_ru_capabilities_format_of_iq_sample_compression_method_supported_get_para(xmlChar **para_temp, const xmlChar *target_para_name)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    status_t        res = NO_ERR ;
    char            para_name[D_PARAM_NUM_MAX];
    char            mod_name[]              = i_mod_o_ran_module_cap ;
    char            para_compression_type[] = i_para_module_cap_compression_type;
    char            para_bitwidth[]         = i_para_module_cap_bitwidth;

    if(!xml_strcmp(y_o_ran_module_cap_N_compression_type, target_para_name))
    {
        /* leaf compression-type */
        snprintf(para_name, D_PARAM_NUM_MAX, "%s", para_compression_type);
    }
    else if(!xml_strcmp(y_o_ran_module_cap_N_bitwidth, target_para_name))
    {
        /* leaf bitwidth */
        snprintf(para_name, D_PARAM_NUM_MAX, "%s", para_bitwidth);
    }
    else
    {
        if( LOGWARN )
        {
            log_warn("¥n%s: Data Missing, para = %s", __func__, target_para_name);
        }
        res = ERR_NCX_DATA_MISSING;
        return res;
    }

    *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_name) ;

    if(*para_temp == NULL)
    {
        res = ERR_NCX_OPERATION_FAILED;
    }
    return res ;
}   /* i_o_ran_module_cap_module_capability_ru_capabilities_format_of_iq_sample_compression_method_supported_get_para */


/*!
 * @brief   (UL) key取得関数
 * @note    
 * @param   [in] k_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_scs_a
 *          [in] k_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_scs_b
 *          [out] curindex_a
 *          [out] curindex_b
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/29 KCN)Suehiro
 */
status_t i_o_ran_module_cap_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_get_key(
    const xmlChar *k_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_scs_a,
    const xmlChar *k_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_scs_b,
    uint32 *curindex_a,
    uint32 *curindex_b)
{
    status_t res = ERR_NCX_DATA_MISSING;
    xmlChar *array_name;
    char para_name[D_PARAM_NUM_MAX];            /* 値確認 */


    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* SCS_A */
    for(uint32 i = 0; i < saa_upconf_max_scs_a_num_ul; i++) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_o_ran_module_req_ul_rbs, ".", i, ".", i_para_o_ran_module_scs_a);
        
        array_name = (xmlChar *)f_saa_com_get_init_data(i_mod_module_cap_char, para_name);
        
        /* 取れたものが妥当かチェック */
        if (array_name == NULL) {
            if(LOGWARN) {
                log_warn("¥n%s:%d Data Missing, name = %s", __func__, __LINE__, k_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_scs_a);
            }
            return res;         /* ERR_NCX_DATA_MISSING */
        }
        else if (!xml_strcmp(array_name, k_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_scs_a)) {
            *curindex_a = i;
            if (LOGDEV1) {
                log_dev1("¥n%s:%d Get key_num. index = %d", __func__, __LINE__, i);
            }
            res = NO_ERR;
            break;
        }
    }

    /* SCS_B */
    for(uint32 i = 0; i < saa_upconf_max_scs_b_num_ul; i++) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_o_ran_module_req_ul_rbs, ".", i, ".", i_para_o_ran_module_scs_b);
        
        array_name = (xmlChar *)f_saa_com_get_init_data(i_mod_module_cap_char, para_name);
        
        /* 取れたものが妥当かチェック */
        if (array_name == NULL) {
            if(LOGWARN) {
                log_warn("¥n%s:%d Data Missing, name = %s", __func__, __LINE__, k_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_scs_b);
            }
            return res;         /* ERR_NCX_DATA_MISSING */
        }
        else if (!xml_strcmp(array_name, k_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_scs_b)) {
            *curindex_b = i;
            if (LOGDEV1) {
                log_dev1("¥n%s:%d Get key_num. index = %d", __func__, __LINE__, i);
            }
            res = NO_ERR;
            break;
        }
    }
    return res;
};      /* i_o_ran_module_cap_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_get_key_num */


/*!
 * @brief   先頭のkey取得を行う
 * @note    
 * @param   [in]  scs_type  scs_a(=1) or scs_b(=2)
 *          [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/28 KCN)Suehiro
 */
status_t i_o_ran_module_cap_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_get_first_key(
    uint32 scs_type,
    xmlChar **key_name_temp )
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if (scs_type == SAA_MODCAP_KEY_SCS_A)
    {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_o_ran_module_req_ul_rbs, ".", 0, ".", i_para_o_ran_module_scs_a);
    }
    else if (scs_type == SAA_MODCAP_KEY_SCS_B){
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_o_ran_module_req_ul_rbs, ".", 0, ".", i_para_o_ran_module_scs_b);
    }
    else{
        /* エラー */
    }

    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_module_cap_char, para_name);
    if(*key_name_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("¥n%s: Operation Failed to get first key scs_a", __func__);
        }
    }

    return res;

} /* i_o_ran_module_cap_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_get_first_key */


/*!
 * @brief   mixed_num_required_guard_rbsの指定されたkey取得を行う
 * @note    
 * @param   [in] curindex
 * @param   [in] scs_type  scs_a(=1) or scs_b(=2)
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)Suehiro
 */
status_t i_o_ran_module_cap_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_get_target_key(
    uint32 curindex,
    uint32 scs_type,
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }
    
    if (scs_type == SAA_MODCAP_KEY_SCS_A)
    {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_o_ran_module_req_ul_rbs, ".", curindex, ".", i_para_o_ran_module_scs_a);
    }
    else if (scs_type == SAA_MODCAP_KEY_SCS_B){
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_o_ran_module_req_ul_rbs, ".", curindex, ".", i_para_o_ran_module_scs_b);
    }
    else{
        if(LOGWARN) {
            log_warn("¥n%s: scs_type NG, scs_type = %d", __func__, scs_type);
        }
        res = ERR_NCX_DATA_MISSING;
        return res;
    }
    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_module_cap_char, para_name);
    
    if(*key_name_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("¥n%s: Operation Failed to get target key, index = %d.", __func__, curindex);
        }
    }
    return res;

}/* i_o_ran_module_cap_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_get_target_key */


/*!
 * @brief   mixed_num_required_guard_rbsの指定されたパラメータ取得を行う
 * @note    取得パラメータはnumber-of-guard-rbだけだが一応汎用的に
 * @param   [in] curindex
 * @param   [in] target_para_name
 * @param   [out] para_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)Suehiro
 */
status_t i_o_ran_module_cap_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_get_para(
    uint32 curindex,
    const xmlChar *target_para_name,
    xmlChar **para_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if(!xml_strcmp(y_o_ran_module_cap_N_number_of_guard_rbs_ul, target_para_name))
    {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_o_ran_module_req_ul_rbs, ".", curindex, ".", i_para_o_ran_module_rbs_ul);
    }
    else 
    {
        if(LOGWARN) {
            log_warn("¥n%s: Data Missing, para = %s", __func__, target_para_name);
        }
        res = ERR_NCX_DATA_MISSING;
        return res;
    }

    *para_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_module_cap_char, para_name);
    if(*para_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("¥n%s: Operation Failed to get param,  para = %s", __func__, para_name);
        }
    }

    return res;

}

/*!
 * @brief   (DL) key取得関数
 * @note    
 * @param   [in] k_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_scs_a
 *          [in] k_module_capability_ru_capabilities_ul_mixed_num_required_guard_rbs_scs_b
 *          [out] curindex_a
 *          [out] curindex_b
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/29 KCN)Suehiro
 */
status_t i_o_ran_module_cap_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_get_key
(
    const xmlChar *k_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_scs_a,
    const xmlChar *k_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_scs_b,
    uint32 *curindex_a,
    uint32 *curindex_b)
{
    status_t res = ERR_NCX_DATA_MISSING;
    xmlChar *array_name;
    char para_name[D_PARAM_NUM_MAX];            /* 値確認 */


    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* SCS_A */
    for(uint32 i = 0; i < saa_upconf_max_scs_a_num_dl; i++) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_o_ran_module_req_dl_rbs, ".", i, ".", i_para_o_ran_module_scs_a);
        
        array_name = (xmlChar *)f_saa_com_get_init_data(i_mod_module_cap_char, para_name);
        
        /* 取れたものが妥当かチェック */
        if (array_name == NULL) {
            if(LOGWARN) {
                log_warn("¥n%s: Data Missing, name = %s", __func__, k_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_scs_a);
            }
            return res;         /* ERR_NCX_DATA_MISSING */
        }
        else if (!xml_strcmp(array_name, k_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_scs_a)) {
            *curindex_a = i;
            if (LOGDEV1) {
                log_dev1("¥n%s: Get key_num. index = %d", __func__, i);
            }
            res = NO_ERR;
            break;
        }
    }

    /* SCS_B */
    for(uint32 i = 0; i < saa_upconf_max_scs_b_num_dl; i++) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_o_ran_module_req_dl_rbs, ".", i, ".", i_para_o_ran_module_scs_b);
        
        array_name = (xmlChar *)f_saa_com_get_init_data(i_mod_module_cap_char, para_name);
        
        /* 取れたものが妥当かチェック */
        if (array_name == NULL) {
            if(LOGWARN) {
                log_warn("¥n%s: Data Missing, name = %s", __func__, k_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_scs_b);
            }
            return res;         /* ERR_NCX_DATA_MISSING */
        }
        else if (!xml_strcmp(array_name, k_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_scs_b)) {
            *curindex_b = i;
            if (LOGDEV1) {
                log_dev1("¥n%s: Get key_num. index = %d", __func__, i);
            }
            res = NO_ERR;
            break;
        }
    }
    return res;
};      /* i_o_ran_module_cap_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_get_key_num */


/*!
 * @brief   先頭のkey取得を行う
 * @note    
 * @param   [in]  scs_type  scs_a(=1) or scs_b(=2)
 *          [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/28 KCN)Suehiro
 */
status_t i_o_ran_module_cap_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_get_first_key(
    uint32 scs_type,
    xmlChar **key_name_temp )
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if (scs_type == SAA_MODCAP_KEY_SCS_A)
    {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_o_ran_module_req_dl_rbs, ".", 0, ".", i_para_o_ran_module_scs_a);
    }
    else if (scs_type == SAA_MODCAP_KEY_SCS_B){
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_o_ran_module_req_dl_rbs, ".", 0, ".",i_para_o_ran_module_scs_b);
    }
    else{
        /* エラー */
    }

    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_module_cap_char, para_name);
    if(*key_name_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("¥n%s: Operation Failed to get first key scs_a/scs_b", __func__);
        }
    }

    return res;

} /* i_o_ran_module_cap_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_get_first_key */


/*!
 * @brief   mixed_num_required_guard_rbsの指定されたkey取得を行う
 * @note    
 * @param   [in] curindex
 * @param   [in] scs_type  scs_a(=1) or scs_b(=2) 
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)Suehiro
 */
status_t i_o_ran_module_cap_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_get_target_key(
    uint32 curindex,
    uint32 scs_type,
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }
    
    if (scs_type == SAA_MODCAP_KEY_SCS_A)
    {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_o_ran_module_req_dl_rbs, ".", curindex, ".", i_para_o_ran_module_scs_a);
    }
    else if (scs_type == SAA_MODCAP_KEY_SCS_B){
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_o_ran_module_req_dl_rbs, ".", curindex, ".", i_para_o_ran_module_scs_b);
    }
    else{
        if(LOGWARN) {
            log_warn("¥n%s: scs_type NG, scs_type = %d", __func__, scs_type);
        }
        res = ERR_NCX_DATA_MISSING;
        return res;
    }
    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_module_cap_char, para_name);
    
    if(*key_name_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("¥n%s: Operation Failed to get target key, index = %d", __func__, curindex);
        }
    }
    return res;

}/* i_o_ran_module_cap_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_get_target_key */



/*!
 * @brief   mixed_num_required_guard_rbsの指定されたパラメータ取得を行う
 * @note    取得パラメータはnumber-of-guard-rbだけだが一応汎用的に
 * @param   [in] curindex
 * @param   [in] target_para_name
 * @param   [out] para_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)Suehiro
 */
status_t i_o_ran_module_cap_module_capability_ru_capabilities_dl_mixed_num_required_guard_rbs_get_para(
    uint32 curindex,
    const xmlChar *target_para_name,
    xmlChar **para_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if(!xml_strcmp(y_o_ran_module_cap_N_number_of_guard_rbs_dl, target_para_name))
    {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_o_ran_module_req_dl_rbs, ".", curindex, ".", i_para_o_ran_module_rbs_dl);
    }
    else 
    {
        if(LOGWARN) {
            log_warn("¥n%s: Data Missing, para = %s", __func__, target_para_name);
        }
        res = ERR_NCX_DATA_MISSING;
        return res;
    }

    *para_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_module_cap_char, para_name);
    if(*para_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("¥n%s: Operation Failed to get param,  para = %s", __func__, para_name);
        }
    }

    return res;

}

/*!
 * @brief   exponent取得関数
 * @note    exponentをinitファイルから取得する
 * @param   [in]  -
 * @        [out] **para_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/06/11 KCN)南條
 */
status_t i_o_ran_module_cap_module_capability_ru_capabilities_format_of_iq_sample_compression_method_supported_compression_format_get(xmlChar **para_temp)
{
    if (LOGINFO) {
        log_info("¥nEnter %s", __func__);
    }

    status_t    res = NO_ERR;
    char        mod_name[]  = i_mod_o_ran_module_cap;
    char        para_name[] = i_para_module_cap_exponent;

    *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_name);
    if(*para_temp == NULL)
    {
        res = ERR_NCX_OPERATION_FAILED;
    }

    return res ;
}   /* i_o_ran_module_cap_module_capability_ru_capabilities_format_of_iq_sample_compression_method_supported_compression_format_get */
