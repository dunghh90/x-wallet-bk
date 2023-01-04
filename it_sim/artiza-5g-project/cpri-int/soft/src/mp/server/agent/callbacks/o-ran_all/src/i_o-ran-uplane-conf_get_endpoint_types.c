/*
 * @file    i_o-ran-uplane-conf_get_endpoint_types.c
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
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   endpoint typeの先頭のkey取得を行う
 * @note    
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/6/24 FJT)長谷川
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_endpoint_type_get_first_key(
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    static char para_name[D_PARAM_NUM_MAX];

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%d", 0);

    log_info("\nEnter %s: %s", __func__, para_name);
    *key_name_temp = (xmlChar *)para_name;

    return res;

}/* i_o_ran_uplane_conf_user_plane_configuration_endpoint_type_get_first_key */

/*!
 * @brief   endpoint typeの指定されたkey取得を行う
 * @note    
 * @param   [in] curindex
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2019/6/24 FJT)長谷川
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_endpoint_type_get_target_key(
    uint32 curindex,
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    static char para_name[D_PARAM_NUM_MAX];
    
    if (LOGINFO) {
        log_info("\nEnter %s:  curindex:%d", __func__, curindex);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%d", curindex);
    *key_name_temp = (xmlChar *)para_name;
    log_info("\nEnter %s: %s", __func__, para_name);

    return res;

}/* i_o_ran_uplane_conf_user_plane_configuration_endpoint_type_get_target_key */

/*!
 * @brief   endpoint typeの指定されたパラメータ取得を行う
 * @note    
 * @param   [in] curindex
 * @param   [in] target_para_name
 * @param   [out] para_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2019/6/24 FJT)長谷川
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_endpoint_type_get_para(
    uint32 curindex,
    const xmlChar *target_para_name,
    xmlChar **para_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if(!xml_strcmp(y_o_ran_uplane_conf_N_supported_frame_structures, target_para_name)){
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%s%s%d%s%s", i_para_o_ran_uplane_conf, ".", i_para_upconf_endpoint_types, ".", curindex, ".", i_para_upconf_conf_supported_frame_structure);
    }
    else if(!xml_strcmp(y_o_ran_uplane_conf_N_managed_delay_support, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%s%s%d%s%s", i_para_o_ran_uplane_conf, ".", i_para_upconf_endpoint_types, ".", curindex, ".", i_para_upconf_conf_managed_delay_support);
    }
    else if(!xml_strcmp(y_o_ran_uplane_conf_N_max_control_sections_per_data_section, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%s%s%d%s%s", i_para_o_ran_uplane_conf, ".", i_para_upconf_endpoint_types, ".", curindex, ".", i_para_upconf_conf_max_ctrl_sections_per_data);
    }
    else if(!xml_strcmp(y_o_ran_uplane_conf_N_max_sections_per_symbol, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%s%s%d%s%s", i_para_o_ran_uplane_conf, ".", i_para_upconf_endpoint_types, ".", curindex, ".", i_para_upconf_conf_max_section_per_symbol);
    }
    else if(!xml_strcmp(y_o_ran_uplane_conf_N_max_sections_per_slot, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%s%s%d%s%s", i_para_o_ran_uplane_conf, ".", i_para_upconf_endpoint_types, ".", curindex, ".", i_para_upconf_conf_max_section_per_slot);
    }
    else if(!xml_strcmp(y_o_ran_uplane_conf_N_max_beams_per_symbol, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%s%s%d%s%s", i_para_o_ran_uplane_conf, ".", i_para_upconf_endpoint_types, ".", curindex, ".", i_para_upconf_conf_max_beam_per_symbol);
    }
    else if(!xml_strcmp(y_o_ran_uplane_conf_N_max_beams_per_slot, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%s%s%d%s%s", i_para_o_ran_uplane_conf, ".", i_para_upconf_endpoint_types, ".", curindex, ".", i_para_upconf_conf_max_beam_per_slot);
    }
    else if(!xml_strcmp(y_o_ran_uplane_conf_N_max_prb_per_symbol, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%s%s%d%s%s", i_para_o_ran_uplane_conf, ".", i_para_upconf_endpoint_types, ".", curindex, ".", i_para_upconf_conf_max_prb_per_symbol);
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing, para=%s", __func__, target_para_name);
        }
        return ERR_NCX_DATA_MISSING;
    }
    
    *para_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, para_name);
    if(*para_temp == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get param, para = %s", __func__, para_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_endpoint_type_get_para */

/*!
 * @brief   endpoint typeの先頭のkey取得を行う
 * @note    
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/6/24 FJT)長谷川
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_endpoint_types_supported_section_types_get_first_key(
    uint16 k_user_plane_configuration_endpoint_types_id,
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%s%s%d%s%s", i_para_o_ran_uplane_conf, ".", i_para_upconf_endpoint_types, ".", k_user_plane_configuration_endpoint_types_id, ".", i_para_upconf_conf_section_type);

    log_info("\nEnter %s: %s", __func__, para_name);
    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, para_name);
    if(*key_name_temp == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get first key.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    return res;

}/* i_o_ran_uplane_conf_user_plane_configuration_endpoint_types_supported_section_types_get_first_key */

/*!
 * @brief   endpoint typeの指定されたkey取得を行う
 * @note    
 * @param   [in] curindex
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2019/6/24 FJT)長谷川
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_endpoint_types_supported_section_types_get_target_key(
    uint32 curindex,
    uint16 k_user_plane_configuration_endpoint_types_id,
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* section typesは1つのため本関数が呼ばれることは無い */
    
    snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%s%s%d%s%s%s%d", i_para_o_ran_uplane_conf, ".", i_para_upconf_endpoint_types, ".", k_user_plane_configuration_endpoint_types_id, ".", i_para_upconf_conf_section_type, ".", curindex);

    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, para_name);

    if(*key_name_temp == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get target key, index = %d", __func__, curindex);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    return res;

}/* i_o_ran_uplane_conf_user_plane_configuration_endpoint_types_supported_section_types_get_target_key */
