/*
 * @file    i_o-ran-uplane-conf_get_array_carriers.c
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
 * @brief   tx arrayのtype取得を行う
 * @note    
 * @param   [in] void
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_type_get(const xmlChar *k_user_plane_configuration_tx_array_carriers_name, xmlChar **v_type_temp)
{
    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if(k_user_plane_configuration_tx_array_carriers_name != NULL) {
        *v_type_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, i_para_upconf_tx_array_car_type);
        if(*v_type_temp == NULL) {
            if(LOGWARN) {
                log_warn("\n%s: Failed to get type.", __func__);
            }
            return ERR_NCX_OPERATION_FAILED;
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing, key = %s", __func__, k_user_plane_configuration_tx_array_carriers_name);
        }
        res = ERR_NCX_DATA_MISSING;
    }
    
    return res;
} /* i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_type_get */

/*!
 * @brief   rx arrayのtype取得を行う
 * @note    
 * @param   [in] void
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_rx_array_carriers_type_get(const xmlChar *k_user_plane_configuration_rx_array_carriers_name, xmlChar **v_type_temp)
{
    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if(k_user_plane_configuration_rx_array_carriers_name != NULL) {
        *v_type_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, i_para_upconf_rx_array_car_type);
        if(*v_type_temp == NULL) {
            if(LOGWARN) {
                log_warn("\n%s: Failed to get type, upconf rx array carriers.", __func__);
            }
            return ERR_NCX_OPERATION_FAILED;
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing, upconf key = %s", __func__, k_user_plane_configuration_rx_array_carriers_name);
        }
        res = ERR_NCX_DATA_MISSING;
    }
    
    return res;
} /* i_o_ran_uplane_conf_user_plane_configuration_rx_array_carriers_type_get */


/*!
 * @brief   tx/rx arrayのstate取得を行う
 * @note    
 * @param   [in] void
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_array_carriers_state_get(
    const xmlChar *target_array_name,
    xmlChar **v_state_temp,
    uint32 dir)
{
    status_t res = ERR_NCX_OPERATION_FAILED;
    T_MPSW_MSI_O_RAN_UPLANE_CONF_GET_REQ get_msg_data;
    uint32_t response_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    char buffer[max(sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_GET_CFM), sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_GET_REJ))];
    T_MPSW_MSI_O_RAN_UPLANE_CONF_GET_CFM *cfm = NULL;

    int rtn_msi = D_MSG_BUFFER_GET_OK;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if(target_array_name == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing, upconf key = %s, dir = %d", __func__, target_array_name, dir);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* buffer初期化 */
    memset(buffer, 0, sizeof(buffer));
    cfm = (T_MPSW_MSI_O_RAN_UPLANE_CONF_GET_CFM *)buffer;

    /* keyに対応するアンテナ/キャリアの番号を検索 */
    if(dir == SAA_UPCONF_DIR_DL) {
        for (uint32 i = 0; i < SAA_UPCONF_MAX_ARR_CAR_NUM; i++){
            if (!xml_strcmp((xmlChar *)saa_tx_car_tbl[i].tx_car_name, target_array_name)) {
                if(saa_tx_car_tbl[i].tx_car_info.ant != SAA_UPCONF_DEFVAL_ANT_NUM 
                    && saa_tx_car_tbl[i].tx_car_info.car != SAA_UPCONF_DEFVAL_CAR_NUM) {
                        get_msg_data.tx_rx = dir;
                        get_msg_data.ant = saa_tx_car_tbl[i].tx_car_info.ant;
                        get_msg_data.car = saa_tx_car_tbl[i].tx_car_info.car;
                        res = NO_ERR;
                        if (LOGDEV1) {
                            log_dev1("\n%s: tx array name registered, array_num = %d", __func__, i);
                        }
                        break;
                }
            }
        }
    }
    else {
        for (uint32 i = 0; i < SAA_UPCONF_MAX_ARR_CAR_NUM; i++){
            if (!xml_strcmp((xmlChar *)saa_rx_car_tbl[i].rx_car_name, target_array_name)) {
                if(saa_rx_car_tbl[i].rx_car_info.ant != SAA_UPCONF_DEFVAL_ANT_NUM 
                    && saa_rx_car_tbl[i].rx_car_info.car != SAA_UPCONF_DEFVAL_CAR_NUM) {
                        get_msg_data.tx_rx = dir;
                        get_msg_data.ant = saa_rx_car_tbl[i].rx_car_info.ant;
                        get_msg_data.car = saa_rx_car_tbl[i].rx_car_info.car;
                        res = NO_ERR;
                        if (LOGDEV1) {
                            log_dev1("\n%s: rx array name registered, array_num = %d", __func__, i);
                        }
                        break;
                }
            }
        }
    }

    /* アンテナ/キャリアの割り当てが未実施の場合はDISABLEDを返す */
    if(res != NO_ERR) {
         *v_state_temp = (xmlChar *)i_para_upconf_state_disable;
        if(LOGDEV1) {
            log_dev1("\n%s: Not allocated Ant/Car to %s,  dir = %d", __func__, target_array_name, dir);
        }
        return NO_ERR;
    }

    /* MSI_O_RAN_UPLANE_CONF_GET_REQ送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_UPLANE_CONF_GET_REQ, sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_GET_REQ), (void*)&get_msg_data);
    if(response_id == 0) {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* MSI_O_RAN_UPLANE_CONF_GETレスポンス待ち */
    rtn_msi = f_saa_com_get_response(response_id,
        sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_GET_REJ),
        &message_id,
        &read_size,
        (void *)&buffer,
        D_MSG_WAIT_TIME );

    /* MSI_O_RAN_UPLANE_CONF_GETレスポンス判定 */
    switch (rtn_msi) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id != D_MSG_MSI_O_RAN_UPLANE_CONF_GET_CFM) {
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: wrong message id. (%0x)", __func__, message_id);
            }
            return ERR_NCX_OPERATION_FAILED;
        } else {
            if(cfm->state == E_RFCP_UPCNF_STS_DISABLE) {
                *v_state_temp = (xmlChar *)i_para_upconf_state_disable;
            }
            else if (cfm->state == E_RFCP_UPCNF_STS_BUSY) {
                *v_state_temp = (xmlChar *)i_para_upconf_state_busy;
            }
            else if (cfm->state == E_RFCP_UPCNF_STS_READY) {
                *v_state_temp = (xmlChar *)i_para_upconf_state_ready;
            }
            else {
                if(LOGWARN) {
                    log_warn("\n%s: MSI failed: wrong state. (%d)", __func__, cfm->state);
                }
            }
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

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_array_carriers_state_get */

/*!
 * @brief   tx/rx arrayのduplex取得を行う
 * @note    
 * @param   [in] v_duplex_temp
 * @param   [in] dir
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2019/6/27 FJT)長谷川哲
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_array_carriers_duplex_get(
    xmlChar **v_duplex_temp,
    uint32 dir)
{
    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if(dir == SAA_UPCONF_DIR_DL) {
        *v_duplex_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, i_para_upconf_tx_array_car_duplex_scheme);
    }
    else if(dir == SAA_UPCONF_DIR_UL) {
        *v_duplex_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, i_para_upconf_rx_array_car_duplex_scheme);
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing, upconf dir = %d", __func__, dir);
        }
        res = ERR_NCX_OPERATION_FAILED;
    }

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_array_carriers_duplex_get */
