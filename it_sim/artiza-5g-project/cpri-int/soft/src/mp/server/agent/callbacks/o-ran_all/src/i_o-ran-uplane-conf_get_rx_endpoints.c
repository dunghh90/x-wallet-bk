/*
 * @file    i_o-ran-uplane-conf_get_rx_endpoints.c
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
 * @brief   low level rx endpointsのkeyからkey番号の取得を行う
 * @note    
 * @param   [in] k_user_plane_configuration_static_low_level_rx_endpoints_name
 * @param   [out] curindex
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)吉木
 * @date    2019/02/18 KCN)吉木 3rdハソ対応
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_rx_endpoints_get_key_num(
    const xmlChar *k_user_plane_configuration_static_low_level_rx_endpoints_name,
    uint32 *curindex)
{
    status_t res = ERR_NCX_DATA_MISSING;
    xmlChar *array_name;
    char para_name[D_PARAM_NUM_MAX];

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    for(uint32 i = 0; i < saa_upconf_max_rx_endpoints_num; i++) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_upconf_rx_endpoints, ".", i, ".", i_para_upconf_name);
        array_name = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, para_name);
        if (array_name == NULL) {
            if(LOGWARN) {
                log_warn("\n%s: Data Missing, name=%s", __func__, k_user_plane_configuration_static_low_level_rx_endpoints_name);
            }
            return ERR_NCX_DATA_MISSING;
        }
        else if (!xml_strcmp(array_name, k_user_plane_configuration_static_low_level_rx_endpoints_name)) {
            *curindex = i;
            if (LOGDEV1) {
                log_dev1("\n%s: Get rx endpoints index, num=%d", __func__, i);
            }
            res = NO_ERR;
            break;
        }
    }

    return res;

}/* i_o_ran_uplane_conf_user_plane_configuration_rx_endpoints_get_key_num */

/*!
 * @brief   rx endpointsの先頭のkey取得を行う
 * @note    
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/28 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_rx_endpoints_get_first_key(
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_upconf_rx_endpoints, ".", 0, ".", i_para_upconf_name);

    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, para_name);
    if(*key_name_temp == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get first key", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    return res;

}/* i_o_ran_uplane_conf_user_plane_configuration_rx_endpoints_get_first_key */

/*!
 * @brief   rx endpointsの指定されたkey取得を行う
 * @note    
 * @param   [in] curindex
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_rx_endpoints_get_target_key(
    uint32 curindex,
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_upconf_rx_endpoints, ".", curindex, ".", i_para_upconf_name);

    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, para_name);
    if(*key_name_temp == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get target key, index=%d", __func__, curindex);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    return res;

}/* i_o_ran_uplane_conf_user_plane_configuration_rx_endpoints_get_target_key */

/*!
 * @brief   rx endpointsの指定されたパラメータ取得を行う
 * @note    
 * @param   [in] curindex
 * @param   [in] target_para_name
 * @param   [out] para_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_rx_endpoints_get_para(
    uint32 curindex,
    const xmlChar *target_para_name,
    xmlChar **para_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    (void)curindex;/* remove yumaworks warning */
    (void)target_para_name;/* remove yumaworks warning */

    if(!xml_strcmp(y_o_ran_uplane_conf_N_restricted_interfaces, target_para_name)){
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_upconf_rx_endpoints, ".", curindex, ".", i_para_upconf_restricted_interface);
    }
    else if(!xml_strcmp(y_o_ran_uplane_conf_N_array, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_upconf_rx_endpoints, ".", curindex, ".", i_para_upconf_array);
    }
    else if(!xml_strcmp(y_o_ran_uplane_conf_N_endpoint_type, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%s%d%s%s", i_para_upconf_rx_endpoints, ".", curindex, ".", i_para_upconf_endpoint_type);
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
            log_warn("\n%s: Failed to get param, para=%s", __func__, para_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_rx_endpoints_get_para */
