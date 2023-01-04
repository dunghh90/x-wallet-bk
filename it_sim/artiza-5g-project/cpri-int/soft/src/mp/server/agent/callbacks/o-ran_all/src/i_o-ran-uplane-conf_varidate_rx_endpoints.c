/*
 * @file    i_o-ran-uplane-conf_varidate_rx_endpoints.c
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
 * @brief   rx endpoints ru port bitmaskのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/23 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_e_axcid_ru_port_bitmask_validate(
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = ERR_NCX_INVALID_VALUE;
    uint16 target;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = VAL_UINT16(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, target=%x", __func__, target);
        }
    }
    else if (curval) {
        target = VAL_UINT16(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, target=%x", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* validate */
    res = i_o_ran_uplane_conf_user_plane_configuration_validate_list_us(i_para_upconf_llre_ru_port_bitmask, target);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_e_axcid_ru_port_bitmask_validate */

/*!
 * @brief   rx endpoints band sector bitmaskのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/23 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_e_axcid_band_sector_bitmask_validate(
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = ERR_NCX_INVALID_VALUE;
    uint16 target;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = VAL_UINT16(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, target=%x", __func__, target);
        }
    }
    else if (curval) {
        target = VAL_UINT16(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, target=%x", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* validate */
    res = i_o_ran_uplane_conf_user_plane_configuration_validate_list_us(i_para_upconf_llre_band_sector_bitmask, target);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_e_axcid_band_sector_bitmask_validate */

/*!
 * @brief   rx endpoints cc id bitmaskのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/23 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_e_axcid_ccid_bitmask_validate(
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = ERR_NCX_INVALID_VALUE;
    uint16 target;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = VAL_UINT16(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, target=%x", __func__, target);
        }
    }
    else if (curval) {
        target = VAL_UINT16(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, target=%x", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* validate */
    res = i_o_ran_uplane_conf_user_plane_configuration_validate_list_us(i_para_upconf_llre_ccid_bitmask, target);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_e_axcid_ccid_bitmask_validate */

/*!
 * @brief   rx endpoints o-du port bitmaskのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/23 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_e_axcid_o_du_port_bitmask_validate(
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = ERR_NCX_INVALID_VALUE;
    uint16 target;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = VAL_UINT16(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, target=%x", __func__, target);
        }
    }
    else if (curval) {
        target = VAL_UINT16(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, target=%x", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* validate */
    res = i_o_ran_uplane_conf_user_plane_configuration_validate_list_us(i_para_upconf_llre_o_du_port_bitmask, target);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_e_axcid_o_du_port_bitmask_validate */

/*!
 * @brief   rx endpoints eaxc-idのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] k_user_plane_configuration_low_level_rx_endpoints_name
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/02/18 KCN)吉木
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_e_axcid_eaxc_id_validate(
    val_value_t *newval,
    const xmlChar *k_user_plane_configuration_low_level_rx_endpoints_name)
{
    if (LOGINFO) {
        log_info("\nEnter %s: key = %s", __func__, k_user_plane_configuration_low_level_rx_endpoints_name);
    }

    return i_o_ran_uplane_conf_eaxc_id_validate(SAA_UPCONF_DIR_UL, newval, k_user_plane_configuration_low_level_rx_endpoints_name);

} /* i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_e_axcid_eaxc_id_validate */


/*!
 * @brief   rx endpoints cp-typeのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @param   [in] k_user_plane_configuration_low_level_rx_endpoints_name
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/6/21 FJT)長谷川
 */

status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_cp_type_validate(
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
    res = i_o_ran_uplane_conf_user_plane_configuration_validate_list(i_para_upconf_llre_cp_type, target);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_cp_type_validate */


/*!
 * @brief   rx endpoints cp-lengthのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @param   [in] k_user_plane_configuration_low_level_rx_endpoints_name
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/6/21 FJT)長谷川
 */

status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_cp_length_validate(
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_user_plane_configuration_low_level_rx_endpoints_name)
{
    status_t res = NO_ERR;
    uint16 target;
    uint32 section_type;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = VAL_UINT16(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, target=%d", __func__, target);
        }
    }
    else if (curval) {
        target = VAL_UINT16(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, target=%d", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* PRACHの場合はvalidate不要 */
    res = i_o_ran_uplane_conf_sectiontype_get(SAA_UPCONF_DIR_UL, k_user_plane_configuration_low_level_rx_endpoints_name, &section_type);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    if(section_type == SAA_UPCONF_ENDP_SEC_TYPE_PRACHSSB ){
        return res;
    }

    /* validate */
    res = i_o_ran_uplane_conf_user_plane_configuration_validate_list_us(i_para_upconf_llre_cp_length, target);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_cp_length_validate */

/*!
 * @brief   rx endpoints cp-length-otherのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @param   [in] k_user_plane_configuration_low_level_rx_endpoints_name
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/6/21 FJT)長谷川
 */

status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_cp_length_other_validate(
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_user_plane_configuration_low_level_rx_endpoints_name)
{
    status_t res = NO_ERR;
    uint16 target;
    uint32 section_type;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = VAL_UINT16(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, target=%d", __func__, target);
        }
    }
    else if (curval) {
        target = VAL_UINT16(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, target=%d", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* PRACHの場合はvalidate不要 */
    res = i_o_ran_uplane_conf_sectiontype_get(SAA_UPCONF_DIR_UL, k_user_plane_configuration_low_level_rx_endpoints_name, &section_type);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    if(section_type == SAA_UPCONF_ENDP_SEC_TYPE_PRACHSSB ){
        return res;
    }

    /* validate */
    res = i_o_ran_uplane_conf_user_plane_configuration_validate_list_us(i_para_upconf_llre_cp_length_other, target);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_cp_length_other_validate */


/*!
 * @brief   rx endpoints compresion typeのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @param   [in] k_user_plane_configuration_low_level_rx_endpoints_name
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/6/21 FJT)長谷川
 */

status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_compression_compression_type_validate(
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
    res = i_o_ran_uplane_conf_user_plane_configuration_validate_list(i_para_upconf_llre_compression_type, target);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_compression_compression_type_validate */

/*!
 * @brief   rx endpoints compression bitwidthのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @param   [in] k_user_plane_configuration_low_level_rx_endpoints_name
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/6/21 FJT)長谷川
 */

status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_compression_bitwidth_validate(
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = NO_ERR;
    uint16 target;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = VAL_UINT16(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, target=%d", __func__, target);
        }
    }
    else if (curval) {
        target = VAL_UINT16(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, target=%d", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* validate */
    res = i_o_ran_uplane_conf_user_plane_configuration_validate_list_us(i_para_upconf_llre_bitwidth, target);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_compression_bitwidth_validate */

/*!
 * @brief   rx endpoints compression bitwidthのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/6/21 FJT)長谷川
 */

status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_non_time_managed_delay_enabled_validate(
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = NO_ERR;
    uint16 target;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = (uint16)VAL_BOOL(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, target=%d", __func__, target);
        }
    }
    else if (curval) {
        target = (uint16)VAL_BOOL(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, target=%d", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* validate */
    res = i_o_ran_uplane_conf_user_plane_configuration_validate_list_us(i_para_upconf_llre_non_time_managed_delay_enabled, target);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_non_time_managed_delay_enabled_validate */


/*!
 * @brief   rx ul fft sampling offsetのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/6/21 FJT)長谷川
 */
status_t i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_ul_fft_sampling_offsets_ul_fft_sampling_offset_validate(
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_user_plane_configuration_low_level_rx_endpoints_name)
{
    status_t res = ERR_NCX_INVALID_VALUE;
    int64_t target;
    int rtn_saa;
    uint32 dir = SAA_UPCONF_DIR_UL;
    uint32 endp_info_num = 0;
    uint32 section_type;

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
        }
            log_dev1("\n%s: Set curval, target=%ld", __func__, target);
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* PRACHの場合はvalidate、設定不要 */
    res = i_o_ran_uplane_conf_sectiontype_get(SAA_UPCONF_DIR_UL, k_user_plane_configuration_low_level_rx_endpoints_name, &section_type);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    if(section_type == SAA_UPCONF_ENDP_SEC_TYPE_PRACHSSB ){
        return res;
    }

    /* validate */
    res = i_o_ran_uplane_conf_user_plane_configuration_validate_max_min(i_para_upconf_llre_ul_fft_sampling_offsets_ul_fft_sampling_offset, target);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    /* saa_endp_info_tblからendp_name検索 */
    rtn_saa = i_o_ran_uplane_conf_check_endp_name_from_endp_info_tbl(dir, k_user_plane_configuration_low_level_rx_endpoints_name, &endp_info_num);

    /* saa_endp_info_tblにendp_name未登録の場合は登録する */
    if(rtn_saa == D_SYS_NG) {
        if (LOGDEV1) {
            log_dev1("\n%s: Add tx/rx endp name to control table, dir = %d name = %s", __func__, dir, k_user_plane_configuration_low_level_rx_endpoints_name);
        }
        rtn_saa = i_o_ran_uplane_conf_set_endp_name_to_endp_info_tbl(dir, k_user_plane_configuration_low_level_rx_endpoints_name, &endp_info_num);
        if(rtn_saa == D_SYS_NG) {
            if(LOGWARN) {
                log_warn("\n%s: Failed to add control table, dir = %d name = %s", __func__, dir, k_user_plane_configuration_low_level_rx_endpoints_name);
            }
            return ERR_NCX_RESOURCE_DENIED;
        }
    }

    /* saa_endp_info_tblにul_fft_sampling_offset登録 */
    saa_endp_info_tbl[dir][endp_info_num].ul_fft_sampling_offset = (uint32)target;

    return res;

} /* i_o_ran_uplane_conf_user_plane_configuration_low_level_rx_endpoints_ul_fft_sampling_offsets_ul_fft_sampling_offset_validate */
