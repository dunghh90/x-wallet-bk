/*
 * @file    i_o-ran-uplane-conf_varidate_tx_array_carriers.c
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
 * @brief   tx array carriers absolute frequency centerのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/23 KCN)吉木
 * @date    2019/02/12 KCN)吉木 3rdハソ対応
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_absolute_frequency_center_validate(
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_user_plane_configuration_tx_array_carriers_name)
{
    status_t res = NO_ERR;
    int64_t target;
    int rtn_saa;
    uint32 tx_car_num = 0;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = (int64_t)VAL_UINT(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, target=%ld", __func__, target);
        }
    }
    else if (curval) {
        target = (int64_t)VAL_UINT(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, target=%ld", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* convert ARFCN to kHz */
    if(saa_upconf_unit_type == SAA_UPCONF_UNIT_TYPE_SUB6) {
        /* sub6 */
        target = SAA_UPCONF_ARFCN_TO_KHZ_PARA1_SUB6 + SAA_UPCONF_ARFCN_TO_KHZ_PARA2_SUB6 * (target - SAA_UPCONF_ARFCN_TO_KHZ_PARA3_SUB6);
    }
    else {
        /* mmw */
        target = SAA_UPCONF_ARFCN_TO_KHZ_PARA1_mmW + SAA_UPCONF_ARFCN_TO_KHZ_PARA2_mmW * (target - SAA_UPCONF_ARFCN_TO_KHZ_PARA3_mmW);
    }

    /* validate */
    res = i_o_ran_uplane_conf_user_plane_configuration_validate_max_min(i_para_upconf_tx_array_car_abs_freq_cent, target);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    /* saa_tx_car_tblからtx_car_name検索 */
    rtn_saa = i_o_ran_uplane_conf_check_tx_car_name_from_tx_car_tbl(k_user_plane_configuration_tx_array_carriers_name, &tx_car_num);

    /* saa_tx_car_tblにtx_car_name未登録の場合は登録する */
    if(rtn_saa == D_SYS_NG) {
        if (LOGDEV1) {
            log_dev1("\n%s: Add tx car name to control table, name=%s", __func__, k_user_plane_configuration_tx_array_carriers_name);
        }
        rtn_saa = i_o_ran_uplane_conf_set_tx_car_name_to_tx_car_tbl(k_user_plane_configuration_tx_array_carriers_name, &tx_car_num);
        if(rtn_saa == D_SYS_NG) {
            if(LOGWARN) {
                log_warn("\n%s: Failed to add control table, name=%s", __func__, k_user_plane_configuration_tx_array_carriers_name);
            }
            return ERR_NCX_RESOURCE_DENIED;
        }
    }

    /* saa_tx_car_tblのactive/sleepがONの場合は変更不可 */
    if (saa_tx_car_tbl[tx_car_num].tx_car_info.active != E_UP_CAR_ACTIVE_INACTIVE) {
        if(LOGWARN) {
            log_warn("\n%s: Failed, tx car(%s) is active", __func__, k_user_plane_configuration_tx_array_carriers_name);
        }
        return ERR_NCX_RESOURCE_DENIED;
    }

    /* saa_tx_car_tblにabsolute_frequency_center登録 */
    saa_tx_car_tbl[tx_car_num].tx_car_info.abs_freq_center = (UINT)target;

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_absolute_frequency_center_validate */

/*!
 * @brief   tx array carriers center of channel bandwidthのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/12/12 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_center_of_channel_bandwidth_validate(
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_user_plane_configuration_tx_array_carriers_name)
{
    status_t res = NO_ERR;
    int64_t target;
    int rtn_saa;
    uint32 tx_car_num = 0;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = (int64_t)VAL_ULONG(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, target=%ld", __func__, target);
        }
    }
    else if (curval) {
        target = (int64_t)VAL_ULONG(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, target=%ld", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* validate */
    /* 5GDUでは本パラメータのvalidateは行わない */

    /* saa_tx_car_tblからtx_car_name検索 */
    rtn_saa = i_o_ran_uplane_conf_check_tx_car_name_from_tx_car_tbl(k_user_plane_configuration_tx_array_carriers_name, &tx_car_num);

    /* saa_tx_car_tblにtx_car_name未登録の場合は登録する */
    if(rtn_saa == D_SYS_NG) {
        if (LOGDEV1) {
            log_dev1("\n%s: Add tx car name to control table, name=%s", __func__, k_user_plane_configuration_tx_array_carriers_name);
        }
        rtn_saa = i_o_ran_uplane_conf_set_tx_car_name_to_tx_car_tbl(k_user_plane_configuration_tx_array_carriers_name, &tx_car_num);
        if(rtn_saa == D_SYS_NG) {
            if(LOGWARN) {
                log_warn("\n%s: Failed to add control table, name=%s", __func__, k_user_plane_configuration_tx_array_carriers_name);
            }
            return ERR_NCX_RESOURCE_DENIED;
        }
    }

    /* saa_tx_car_tblのactive/sleepがONの場合は変更不可 */
    if (saa_tx_car_tbl[tx_car_num].tx_car_info.active != E_UP_CAR_ACTIVE_INACTIVE) {
        if(LOGWARN) {
            log_warn("\n%s: Failed, tx car(%s) is active", __func__, k_user_plane_configuration_tx_array_carriers_name);
        }
        return ERR_NCX_RESOURCE_DENIED;
    }

    /* saa_tx_car_tblにcent_of_ch_bandwidth登録 */
    saa_tx_car_tbl[tx_car_num].tx_car_info.cent_of_ch_bandwidth = (UINT64)target;

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_center_of_channel_bandwidth_validate */

/*!
 * @brief   tx array carriers channel bandwidthのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/23 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_channel_bandwidth_validate(
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_user_plane_configuration_tx_array_carriers_name)
{
    status_t res = ERR_NCX_INVALID_VALUE;
    int rtn_saa;
    T_SAA_COM_PARAM_LIST_TBL validate_param_list;
    uint64 target;
    uint32 tx_car_num = 0;
    uint64 validate_param = 0;
    char *validate_param_addr;
    
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate_param_list初期化 */
    memset((void *)&validate_param_list, 0, sizeof(T_SAA_COM_PARAM_LIST_TBL));

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = VAL_ULONG(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, target=%ld", __func__, target);
        }
    }
    else if (curval) {
        target = VAL_ULONG(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, target=%ld", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* validate用パラメータを取得 */
    validate_param_addr = f_saa_com_get_validate_data(i_mod_upconf, i_para_upconf_tx_array_car_ch_bw);
    if (validate_param_addr == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get validate data.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate用パラメータの変換 */
    rtn_saa = f_saa_com_conversion_validate_data_list(validate_param_addr, &validate_param_list);
    if (rtn_saa == D_SYS_NG) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to convert validate data", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate */
    for (int i = 0; validate_param_list.list[i][0] != '\0'; i++) {
        sscanf ((const char *)validate_param_list.list[i], "%ld", &validate_param);
        if (target == validate_param) {
            if (LOGDEV1) {
                log_dev1("\n%s: Validation Completed, target=%ld", __func__, target);
            }
            res = NO_ERR;
            break;
        }
    }
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    /* saa_tx_car_tblからtx_car_name検索 */
    rtn_saa = i_o_ran_uplane_conf_check_tx_car_name_from_tx_car_tbl(k_user_plane_configuration_tx_array_carriers_name, &tx_car_num);

    /* saa_tx_car_tblにtx_car_name未登録の場合は登録する */
    if(rtn_saa == D_SYS_NG) {
        if (LOGDEV1) {
            log_dev1("\n%s: Add tx car name to control table, name=%s", __func__, k_user_plane_configuration_tx_array_carriers_name);
        }
        rtn_saa = i_o_ran_uplane_conf_set_tx_car_name_to_tx_car_tbl(k_user_plane_configuration_tx_array_carriers_name, &tx_car_num);
        if(rtn_saa == D_SYS_NG) {
            if(LOGWARN) {
                log_warn("\n%s: Failed to add control table, name=%s", __func__, k_user_plane_configuration_tx_array_carriers_name);
            }
            return ERR_NCX_RESOURCE_DENIED;
        }
    }

    /* saa_tx_car_tblのactive/sleepがONの場合は変更不可 */
    if (saa_tx_car_tbl[tx_car_num].tx_car_info.active != E_UP_CAR_ACTIVE_INACTIVE) {
        if(LOGWARN) {
            log_warn("\n%s: Failed, tx car(%s) is active.", __func__, k_user_plane_configuration_tx_array_carriers_name);
        }
        return ERR_NCX_RESOURCE_DENIED;
    }

    /* saa_tx_car_tblにch_bandwidth登録 */
    saa_tx_car_tbl[tx_car_num].tx_car_info.ch_bandwidth = (UINT64)target;

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_channel_bandwidth_validate */

/*!
 * @brief   tx array carriers gainのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/12/12 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_gain_validate(
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_user_plane_configuration_tx_array_carriers_name)
{
    status_t res = NO_ERR;
    int rtn_saa;
    T_SAA_COM_PARAM_LIST_TBL validate_param_list;
    int64 target;
    uint32 tx_car_num = 0;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate_param_list初期化 */
    memset((void *)&validate_param_list, 0, sizeof(T_SAA_COM_PARAM_LIST_TBL));

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = VAL_DEC64(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, target=%.4f", __func__, (double)target);
        }
    }
    else if (curval) {
        target = VAL_DEC64(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, target=%.4f", __func__, (double)target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* validate */
    res = i_o_ran_uplane_conf_user_plane_configuration_validate_max_min(i_para_upconf_conf_tx_array_car_gain, target);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    /* saa_tx_car_tblからtx_car_name検索 */
    rtn_saa = i_o_ran_uplane_conf_check_tx_car_name_from_tx_car_tbl(k_user_plane_configuration_tx_array_carriers_name, &tx_car_num);

    /* saa_tx_car_tblにtx_car_name未登録の場合は登録する */
    if(rtn_saa == D_SYS_NG) {
        if (LOGDEV1) {
            log_dev1("\n%s: Add tx car name to control table, name=%s", __func__, k_user_plane_configuration_tx_array_carriers_name);
        }
        rtn_saa = i_o_ran_uplane_conf_set_tx_car_name_to_tx_car_tbl(k_user_plane_configuration_tx_array_carriers_name, &tx_car_num);
        if(rtn_saa == D_SYS_NG) {
            if(LOGWARN) {
                log_warn("\n%s: Failed to add control table, name=%s", __func__, k_user_plane_configuration_tx_array_carriers_name);
            }
            return ERR_NCX_RESOURCE_DENIED;
        }
    }

    /* saa_tx_car_tblのactive/sleepがONの場合は変更不可 */
    if (saa_tx_car_tbl[tx_car_num].tx_car_info.active != E_UP_CAR_ACTIVE_INACTIVE) {
        if(LOGWARN) {
            log_warn("\n%s: Failed, tx car(%s) is active.", __func__, k_user_plane_configuration_tx_array_carriers_name);
        }
        return ERR_NCX_RESOURCE_DENIED;
    }

    /* saa_tx_car_tblにgain登録 */
    saa_tx_car_tbl[tx_car_num].tx_car_info.gain = target;

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_gain_validate */

/*!
 * @brief   tx array carriers activeのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/23 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_active_validate(
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = NO_ERR;
    const xmlChar *target;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = VAL_ENUM_NAME(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, target=%s", __func__, target);
        }
    }
    else if (curval) {
        target = VAL_ENUM_NAME(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, target=%s", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* validate */
    res = i_o_ran_uplane_conf_user_plane_configuration_validate_list(i_para_upconf_tx_array_car_act, target);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_active_validate */

/*!
 * @brief   tx array carriers downlink radio frame offsetのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/23 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_downlink_radio_frame_offset_validate(
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_user_plane_configuration_tx_array_carriers_name)
{
    status_t res = NO_ERR;
    int64_t target;
    int rtn_saa;
    uint32 tx_car_num = 0;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = (int64_t)VAL_UINT(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, target=%ld", __func__, target);
        }
    }
    else if (curval) {
        target = (int64_t)VAL_UINT(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, target=%ld", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* validate */
    res = i_o_ran_uplane_conf_user_plane_configuration_validate_max_min(i_para_upconf_tx_array_car_dl_radio_frame_offset, target);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    /* saa_tx_car_tblからtx_car_name検索 */
    rtn_saa = i_o_ran_uplane_conf_check_tx_car_name_from_tx_car_tbl(k_user_plane_configuration_tx_array_carriers_name, &tx_car_num);

    /* saa_tx_car_tblにtx_car_name未登録の場合は登録する */
    if(rtn_saa == D_SYS_NG) {
        if (LOGDEV1) {
            log_dev1("\n%s: Add tx car name to control table, name=%s", __func__, k_user_plane_configuration_tx_array_carriers_name);
        }
        rtn_saa = i_o_ran_uplane_conf_set_tx_car_name_to_tx_car_tbl(k_user_plane_configuration_tx_array_carriers_name, &tx_car_num);
        if(rtn_saa == D_SYS_NG) {
            if(LOGWARN) {
                log_warn("\n%s: Failed to add control table, name=%s", __func__, k_user_plane_configuration_tx_array_carriers_name);
            }
            return ERR_NCX_RESOURCE_DENIED;
        }
    }

    /* saa_tx_car_tblのactive/sleepがONの場合は変更不可 */
    if (saa_tx_car_tbl[tx_car_num].tx_car_info.active != E_UP_CAR_ACTIVE_INACTIVE) {
        if(LOGWARN) {
            log_warn("\n%s: Failed, tx car(%s) is active.", __func__, k_user_plane_configuration_tx_array_carriers_name);
        }
        return ERR_NCX_RESOURCE_DENIED;
    }

    /* saa_tx_car_tblにdl_radio_frame_offset登録 */
    saa_tx_car_tbl[tx_car_num].tx_car_info.dl_radio_frame_offset = (UINT)target;

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_downlink_radio_frame_offset_validate */

/*!
 * @brief   tx array carriers downlink sfn offsetのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/23 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_downlink_sfn_offset_validate(
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_user_plane_configuration_tx_array_carriers_name)
{
    status_t res = NO_ERR;
    int64_t target;
    int rtn_saa;
    uint32 tx_car_num = 0;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = (int64_t)VAL_INT16(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, target=%ld", __func__, target);
        }
    }
    else if (curval) {
        target = (int64_t)VAL_INT16(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, target=%ld", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* validate */
    res = i_o_ran_uplane_conf_user_plane_configuration_validate_max_min(i_para_upconf_tx_array_car_dl_sfn_offset, target);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    /* saa_tx_car_tblからtx_car_name検索 */
    rtn_saa = i_o_ran_uplane_conf_check_tx_car_name_from_tx_car_tbl(k_user_plane_configuration_tx_array_carriers_name, &tx_car_num);

    /* saa_tx_car_tblにtx_car_name未登録の場合は登録する */
    if(rtn_saa == D_SYS_NG) {
        if (LOGDEV1) {
            log_dev1("\n%s: Add tx car name to control table, name=%s", __func__, k_user_plane_configuration_tx_array_carriers_name);
        }
        rtn_saa = i_o_ran_uplane_conf_set_tx_car_name_to_tx_car_tbl(k_user_plane_configuration_tx_array_carriers_name, &tx_car_num);
        if(rtn_saa == D_SYS_NG) {
            if(LOGWARN) {
                log_warn("\n%s: Failed to add control table, name = %s", __func__, k_user_plane_configuration_tx_array_carriers_name);
            }
            return ERR_NCX_RESOURCE_DENIED;
        }
    }

    /* saa_tx_car_tblのactive/sleepがONの場合は変更不可 */
    if (saa_tx_car_tbl[tx_car_num].tx_car_info.active != E_UP_CAR_ACTIVE_INACTIVE) {
        if(LOGWARN) {
            log_warn("\n%s: Failed, tx car(%s) is active.", __func__, k_user_plane_configuration_tx_array_carriers_name);
        }
        return ERR_NCX_RESOURCE_DENIED;
    }

    /* saa_tx_car_tblにdl_sfn_offset登録 */
    saa_tx_car_tbl[tx_car_num].tx_car_info.dl_sfn_offset = (INT16)target;

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_tx_array_carriers_downlink_sfn_offset_validate */