/*
 * @file    i_o-ran-uplane-conf_varidate_tx_links.c
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
 * @brief   tx link array carrierのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_RESOURCE_DENIED
 *          [異常]ERR_NCX_DATA_EXISTS
 * @date    2018/11/24 KCN)吉木
 * @date    2019/02/21 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_tx_links_tx_array_carrier_validate(
    val_value_t *newval,
    const xmlChar *k_user_plane_configuration_low_level_tx_links_name)
{
    status_t res = NO_ERR;
    uint32 link_info_num = 0;
    int rtn_saa;
    uint32 dir = SAA_UPCONF_DIR_DL;
    uint32 array_carrier_cnt = 0;
    uint32 pair_link_info_num = 0;
    int pair_link_invalid_flag;

    if (LOGINFO) {
        log_info("\nEnter %s: key=%s", __func__, k_user_plane_configuration_low_level_tx_links_name);
    }

    /* 引数チェック */
    if(!newval) {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing. (newval)", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }
    else if (!k_user_plane_configuration_low_level_tx_links_name) {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing. (links_name)", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* array_carrierがsaa_tx_car_tblに未登録の場合はエラーを返す */
    rtn_saa = i_o_ran_uplane_conf_check_array_carrier_from_car_tbl(dir, VAL_STRING(newval));
    if(rtn_saa == D_SYS_NG) {
        if(LOGWARN) {
            log_warn("\n%s: %s is not exist in tx-array-carriers", __func__, VAL_STRING(newval));
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* saa_link_info_tblからlink_name検索 */
    rtn_saa = i_o_ran_uplane_conf_check_link_name_from_link_info_tbl(dir, k_user_plane_configuration_low_level_tx_links_name, &link_info_num);

    /* saa_link_info_tblにlink_name未登録の場合は登録する */
    if(rtn_saa == D_SYS_NG) {
        if (LOGDEV1) {
            log_dev1("\n%s: Add tx links name to control table, name = %s", __func__, k_user_plane_configuration_low_level_tx_links_name);
        }
        rtn_saa = i_o_ran_uplane_conf_set_link_name_to_link_info_tbl(dir, k_user_plane_configuration_low_level_tx_links_name, &link_info_num);
        if(rtn_saa == D_SYS_NG) {
            if(LOGWARN) {
                log_warn("\n%s: Failed to add control table, name = %s", __func__, k_user_plane_configuration_low_level_tx_links_name);
            }
            return ERR_NCX_RESOURCE_DENIED;
        }
    }

    /* saa_link_info_tblのactive/sleepがONの場合は変更不可 */
    if(saa_link_info_tbl[dir][link_info_num].active != E_UP_CAR_ACTIVE_INACTIVE) {
        if(LOGWARN) {
            log_warn("\n%s: Failed, tx links(%s) has active carrier.", __func__, k_user_plane_configuration_low_level_tx_links_name);
        }
        return ERR_NCX_RESOURCE_DENIED;
    }

    pair_link_invalid_flag = D_SYS_OK;
    /* saa_link_info_tblにarray_carrierがsaa_upconf_max_ssb_num以上登録されている場合はエラーを返す */
    for (uint32 i = 0; i < SAA_UPCONF_MAX_LINK_NUM; i++){
        if (!xml_strcmp((xmlChar *)saa_link_info_tbl[dir][i].array_carrier, VAL_STRING(newval)) && i != link_info_num) {
            array_carrier_cnt++;
            pair_link_info_num = i;
            
            if((saa_upconf_beamforming_mode == SAA_UPCONF_BF_ON) && (saa_upconf_unit_type == SAA_UPCONF_UNIT_TYPE_SUB6)) {
            /* Sub 6でBeamforming ONの場合Rx linksに割り付けられるarray-carrierは最大 saa_upconf_max_ssb_num*2 */
                if (array_carrier_cnt >= saa_upconf_max_ssb_num*2) {
                    if(LOGWARN) {
                        log_warn("\n%s: DATA EXISTS, target array carries(%s) is already used", __func__, VAL_STRING(newval));
                    }
                    return ERR_NCX_DATA_EXISTS;
                }
            }
            else {
            /* Sub 6でBeamforming ON以外の場合Rx linksに割り付けられるarray-carrierは最大 saa_upconf_max_ssb_num */
                if (array_carrier_cnt >= saa_upconf_max_ssb_num) {
                    if(LOGWARN) {
                        log_warn("\n%s: DATA EXISTS, target array carries(%s) is already used", __func__, VAL_STRING(newval));
                    }
                    return ERR_NCX_DATA_EXISTS;
                }
            }

            /* saa_link_info_tblにendpoints登録済みかつsaa_link_info_tblにarray_carrierが登録済みの場合はant/car/port/local_eaxc_idを検証 */
            if(saa_link_info_tbl[dir][link_info_num].endpoints[0] != '\0' 
                && array_carrier_cnt != 0 
                && saa_link_info_tbl[dir][pair_link_info_num].endpoints[0] != '\0') {
                if(saa_link_info_tbl[dir][link_info_num].local_eaxc_id == saa_link_info_tbl[dir][pair_link_info_num].local_eaxc_id) {
                    pair_link_invalid_flag = D_SYS_NG;
                    break;
                }
            }
        }
    }

    if(pair_link_invalid_flag == D_SYS_NG) {
        if(LOGWARN) {
            log_warn("\n%s: Failed, target array carrier(%s) is allocated other ant/car/port or same eaxc-id", __func__, saa_link_info_tbl[dir][link_info_num].array_carrier);
        }
        return ERR_NCX_RESOURCE_DENIED;
    }

    /* saa_tx_link_info_tblにarray_carrier登録 */
    xml_strcpy(saa_link_info_tbl[dir][link_info_num].array_carrier, VAL_STRING(newval));
    if (LOGINFO) {
        log_info("\n%s: Set array_carrier, saa_link_info_tbl[%d][%d].array_carrier = %s", __func__, dir, link_info_num, saa_link_info_tbl[dir][link_info_num].array_carrier);
    }

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_low_level_tx_links_tx_array_carrier_validate */

/*!
 * @brief   tx link endpointsのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_RESOURCE_DENIED
 * @date    2018/11/24 KCN)吉木
 * @date    2019/02/21 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_tx_links_low_level_tx_endpoint_validate(
    val_value_t *newval,
    const xmlChar *k_user_plane_configuration_low_level_tx_links_name)
{
    status_t res = NO_ERR;
    uint32 link_info_num = 0;
    int rtn_saa;
    uint32 dir = SAA_UPCONF_DIR_DL;
    endpoint_info_t endp_info;
    uint32 endp_info_num = 0;
    uint32 pair_link_info_num = 0;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* 引数チェック */
    if(!newval) {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing. (newval)", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }
    else if (!k_user_plane_configuration_low_level_tx_links_name) {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing. (links_name)", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* endpointsがsaa_endp_info_tblに未登録の場合はエラーを返す */
    rtn_saa = i_o_ran_uplane_conf_check_endpoints_from_endp_info_tbl(dir, VAL_STRING(newval), &endp_info_num);
    if(rtn_saa == D_SYS_NG) {
        if(LOGWARN) {
            log_warn("\n%s: %s is not exist in low-level-tx-endpoints", __func__, VAL_STRING(newval));
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* saa_endp_info_tblからant/car/local_eaxc_id_numを取得 */
    endp_info = saa_endp_info_tbl[dir][endp_info_num];

    /* saa_link_info_tblにendpointsが登録済みの場合はエラーを返す */
    for (int i = 0; i < SAA_UPCONF_MAX_LINK_NUM; i++){
        if (!xml_strcmp((xmlChar *)saa_link_info_tbl[dir][i].endpoints, VAL_STRING(newval))) {
            if(LOGWARN) {
                log_warn("\n%s: DATA EXISTS, target endpoint(%s) is already used", __func__, VAL_STRING(newval));
            }
            return ERR_NCX_DATA_EXISTS;
        }
    }

    /* saa_link_info_tblからlink_name検索 */
    rtn_saa = i_o_ran_uplane_conf_check_link_name_from_link_info_tbl(dir, k_user_plane_configuration_low_level_tx_links_name, &link_info_num);

    /* saa_link_info_tblにlink_name未登録の場合は登録する */
    if(rtn_saa == D_SYS_NG) {
        if (LOGDEV1) {
            log_dev1("\n%s: Add tx links name to control table, name=%s", __func__, k_user_plane_configuration_low_level_tx_links_name);
        }
        rtn_saa = i_o_ran_uplane_conf_set_link_name_to_link_info_tbl(dir, k_user_plane_configuration_low_level_tx_links_name, &link_info_num);
        if(rtn_saa == D_SYS_NG) {
            if(LOGWARN) {
                log_warn("\n%s: Failed to add control table, name=%s", __func__, k_user_plane_configuration_low_level_tx_links_name);
            }
            return ERR_NCX_RESOURCE_DENIED;
        }
    }

    /* saa_link_info_tblのactive/sleepがONの場合は変更不可 */
    if(saa_link_info_tbl[dir][link_info_num].active != E_UP_CAR_ACTIVE_INACTIVE) {
        if(LOGWARN) {
            log_warn("\n%s: Failed, tx links(%s) has active carrier.", __func__, k_user_plane_configuration_low_level_tx_links_name);
        }
        return ERR_NCX_RESOURCE_DENIED;
    }

    /* saa_link_info_tblにarray_carrier登録済みの場合は、同一array_carrierが登録済みか検索 */
    if(saa_link_info_tbl[dir][link_info_num].array_carrier[0] != '\0') {
        rtn_saa = i_o_ran_uplane_conf_check_pair_link_from_link_info_tbl(dir, saa_link_info_tbl[dir][link_info_num].array_carrier, link_info_num, &pair_link_info_num);
        if(rtn_saa == D_SYS_OK) {
            /* 同一array_carrierが登録済みの場合はendpointsの登録有無を確認し、ant/car/port/local_eaxc_id_numを検証 */
            if(saa_link_info_tbl[dir][pair_link_info_num].endpoints[0] != '\0') {
                if(saa_link_info_tbl[dir][pair_link_info_num].local_eaxc_id == endp_info.local_eaxc_id) {
                        if(LOGWARN) {
                            log_warn("\n%s: Failed, target array carrier(%s) is allocated other ant/car/port or same eaxc-id", __func__, saa_link_info_tbl[dir][link_info_num].array_carrier);
                        }
                        return ERR_NCX_RESOURCE_DENIED;
                }
            }
        }
    }

    /* saa_tx_link_info_tblにendpoints登録 */
    xml_strcpy(saa_link_info_tbl[dir][link_info_num].endpoints, VAL_STRING(newval));
    if (LOGINFO) {
        log_info("\n%s: Set endpoints, saa_link_info_tbl[%d][%d].endpoints=%s", __func__, dir, link_info_num, saa_link_info_tbl[dir][link_info_num].endpoints);
    }

    /* saa_tx_link_info_tblにant/car/local eaxc-idを登録 */
    saa_link_info_tbl[dir][link_info_num].ant = endp_info.ant;
    saa_link_info_tbl[dir][link_info_num].car = endp_info.car;
    saa_link_info_tbl[dir][link_info_num].port = endp_info.port;
    saa_link_info_tbl[dir][link_info_num].local_eaxc_id_num = endp_info.local_eaxc_id_num;
    saa_link_info_tbl[dir][link_info_num].local_eaxc_id = endp_info.local_eaxc_id;
    saa_link_info_tbl[dir][link_info_num].endp_info_num = endp_info_num;
    saa_link_info_tbl[dir][link_info_num].section_type = endp_info.section_type;
    if (LOGINFO) {
        log_info("\n%s: Set saa_link_info_tbl[%d][%d].ant=%d", __func__, dir, link_info_num, saa_link_info_tbl[dir][link_info_num].ant);
        log_info("\n%s: Set saa_link_info_tbl[%d][%d].car=%d", __func__, dir, link_info_num, saa_link_info_tbl[dir][link_info_num].car);
        log_info("\n%s: Set saa_link_info_tbl[%d][%d].port=%d", __func__, dir, link_info_num, saa_link_info_tbl[dir][link_info_num].port);
        log_info("\n%s: Set saa_link_info_tbl[%d][%d].local_eaxc_id_num=%d", __func__, dir, link_info_num, saa_link_info_tbl[dir][link_info_num].local_eaxc_id_num);
        log_info("\n%s: Set saa_link_info_tbl[%d][%d].local_eaxc_id=%d", __func__, dir, link_info_num, saa_link_info_tbl[dir][link_info_num].local_eaxc_id);
        log_info("\n%s: Set saa_link_info_tbl[%d][%d].endp_info_num=%d", __func__, dir, link_info_num, saa_link_info_tbl[dir][link_info_num].endp_info_num);
        log_info("\n%s: Set saa_link_info_tbl[%d][%d].section_type=%d", __func__, dir, link_info_num, saa_link_info_tbl[dir][link_info_num].section_type);
    }

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_low_level_tx_links_low_level_tx_endpoint_validate */