/*
 * @file    i_o-ran-uplane-conf_get_rx_arrays.c
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
 * @brief   rx arrayのkeyからkey番号の取得を行う
 * @note    
 * @param   [in] k_user_plane_configuration_rx_arrays_name
 * @param   [out] curindex
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_get_key_num(
    const xmlChar *k_user_plane_configuration_rx_arrays_name,
    uint32 *curindex)
{
    status_t res = ERR_NCX_DATA_MISSING;
    xmlChar *array_name;
    char para_name[D_PARAM_NUM_MAX];

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    for(uint32 i = 0; i < saa_upconf_max_ant_num; i++) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_upconf_rx_arrays, ".", i, ".", i_para_upconf_name);
        array_name = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, para_name);
        if (array_name == NULL) {
            if(LOGWARN) {
                log_warn("\n%s: Data Missing, name=%s", __func__, k_user_plane_configuration_rx_arrays_name);
            }
            return ERR_NCX_DATA_MISSING;
        }
        else if (!xml_strcmp(array_name, k_user_plane_configuration_rx_arrays_name)) {
            *curindex = i;
            if (LOGDEV1) {
                log_dev1("\n%s: Get rx arrays index, num = %d", __func__, i);
            }
            res = NO_ERR;
            break;
        }
    }

    return res;

}/* i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_get_key_num */

/*!
 * @brief   rx arrayの先頭のkey取得を行う
 * @note    
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/28 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_get_first_key(
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_upconf_rx_arrays, ".", 0, ".", i_para_upconf_name);

    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, para_name);
    if(*key_name_temp == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get first key.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    return res;

}/* i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_get_first_key */

/*!
 * @brief   rx arrayの指定されたkey取得を行う
 * @note    
 * @param   [in] curindex
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_get_target_key(
    uint32 curindex,
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_upconf_rx_arrays, ".", curindex, ".", i_para_upconf_name);

    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, para_name);
    if(*key_name_temp == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get target key, index = %d", __func__, curindex);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    return res;

}/* i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_get_target_key */

/*!
 * @brief   rx arrayの指定されたパラメータ取得を行う
 * @note    
 * @param   [in] curindex
 * @param   [in] target_para_name
 * @param   [out] para_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_get_para(
    uint32 curindex,
    const xmlChar *target_para_name,
    xmlChar **para_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if(!xml_strcmp(y_o_ran_uplane_conf_N_number_of_rows, target_para_name)){
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_upconf_rx_arrays, ".", curindex, ".", i_para_upconf_num_of_rows);
    }
    else if(!xml_strcmp(y_o_ran_uplane_conf_N_number_of_columns, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_upconf_rx_arrays, ".", curindex, ".", i_para_upconf_num_of_col);
    }
    else if(!xml_strcmp(y_o_ran_uplane_conf_N_number_of_array_layers, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_upconf_rx_arrays, ".", curindex, ".", i_para_upconf_num_of_array_layers);
    }
    else if(!xml_strcmp(y_o_ran_uplane_conf_N_band_number, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_upconf_rx_arrays, ".", curindex, ".", i_para_upconf_band_number);
    }
    else if(!xml_strcmp(y_o_ran_uplane_conf_N_max, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_upconf_rx_arrays, ".", curindex, ".", i_para_o_ran_uplane_conf_gain_correction_max);
    }
    else if(!xml_strcmp(y_o_ran_uplane_conf_N_min, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_upconf_rx_arrays, ".", curindex, ".", i_para_o_ran_uplane_conf_gain_correction_min);
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

} /* i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_get_para */



/*!
 * @brief   polarisationsの先頭のkey取得を行う
 * @note    
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/6/24 FJT)長谷川
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_polarisations_get_first_key(
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s%s%d%s%s", i_para_upconf_rx_arrays, ".", 0, ".",i_para_upconf_polarisations , ".", 0, ".", i_para_upconf_p);
    log_info("\nEnter %s: %s", __func__, para_name);
    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, para_name);
    if(*key_name_temp == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get first key.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    return res;

}/* i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_polarisations_get_first_key */

/*!
 * @brief   polarisationsの指定されたkey取得を行う
 * @note    
 * @param   [in] curindex
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2019/6/24 FJT)長谷川
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_polarisations_get_target_key(
    uint32 curindex,
    const xmlChar *k_user_plane_configuration_rx_arrays_name,
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    uint32 arrayindex;
    char para_name[D_PARAM_NUM_MAX];
    
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if(k_user_plane_configuration_rx_arrays_name != NULL) {
        i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_get_key_num(k_user_plane_configuration_rx_arrays_name, &arrayindex);
    }
    else {
        arrayindex = 0;
    }
    snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s%s%d%s%s", i_para_upconf_rx_arrays, ".", arrayindex, ".",i_para_upconf_polarisations , ".", curindex, ".", i_para_upconf_p);

    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, para_name);

    log_info("\nEnter %s: %s", __func__, para_name);
    if(*key_name_temp == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get target key, index = %d", __func__, curindex);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    return res;

}/* i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_polarisations_get_target_key */

/*!
 * @brief   polarisationsの指定されたパラメータ取得を行う
 * @note    
 * @param   [in] curindex
 * @param   [in] target_para_name
 * @param   [out] para_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2019/6/24 FJT)長谷川
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_polarisations_get_para(
    uint32 curindex,
    const xmlChar *k_user_plane_configuration_rx_arrays_name,
    const xmlChar *target_para_name,
    xmlChar **para_temp)
{
    status_t res = NO_ERR;
    uint32 arrayindex;
    char para_name[D_PARAM_NUM_MAX];
    
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if(k_user_plane_configuration_rx_arrays_name != NULL) {
        i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_get_key_num(k_user_plane_configuration_rx_arrays_name, &arrayindex);
    }
    else {
        arrayindex = 0;
    }
    if(!xml_strcmp(y_o_ran_uplane_conf_N_polarisation, target_para_name)){
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s%s%d%s%s", i_para_upconf_rx_arrays, ".", arrayindex, ".",i_para_upconf_polarisations , ".", curindex, ".", i_para_upconf_polarisation);
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

} /* i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_polarisations_get_para */

status_t i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_polarisations_get_key_num(
const xmlChar *k_user_plane_configuration_rx_arrays_name, uint8 k_user_plane_configuration_rx_arrays_polarisations_p, uint32 *curindex) {
    status_t res = ERR_NCX_DATA_MISSING;
    uint32 arrayindex;
    char para_name[D_PARAM_NUM_MAX];
    xmlChar *p_name;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }
    
    if(k_user_plane_configuration_rx_arrays_name != NULL) {
        i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_get_key_num(k_user_plane_configuration_rx_arrays_name, &arrayindex);
    }
    else {
        arrayindex = 0;
    }

    for(uint32 i = 0; i < SAA_UPCONF_POLARISATION_NUM; i++) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s%s%d%s%s", i_para_upconf_rx_arrays, ".", arrayindex, ".",i_para_upconf_polarisations , ".", i, ".", i_para_upconf_p);
        p_name = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, para_name);

        if (p_name == NULL) {
            if(LOGWARN) {
                log_warn("\n%s: Data Missing, name=%s", __func__, para_name);
            }
            return ERR_NCX_DATA_MISSING;
        }
        else if ((uint8)atoi((const char *)p_name) == k_user_plane_configuration_rx_arrays_polarisations_p) {
            *curindex = i;
            if (LOGDEV1) {
                log_dev1("\n%s: Get rx arrays index, num=%d", __func__, i);
            }
            res = NO_ERR;
            break;
        }
    }
    return res;
}/* i_o_ran_uplane_conf_user_plane_configuration_rx_arrays_polarisations_get_key_num */
