
/*
 * @file    i_o-ran-sync.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_SYNC
 * @brief       SAA o-ran-sync
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <xmlstring.h>

#include "procdefs.h"
#include "agt.h"
#include "agt_action.h"
#include "agt_cb.h"
#include "agt_rpc.h"
#include "agt_sil_lib.h"
#include "agt_timer.h"
#include "agt_util.h"
#include "dlq.h"
#include "getcb.h"
#include "ncx.h"
#include "ncx_feature.h"
#include "ncxmod.h"
#include "ncxtypes.h"
#include "obj.h"
#include "rpc.h"
#include "ses.h"
#include "sil_sa.h"
#include "status.h"
#include "val.h"
#include "val_util.h"
#include "xml_util.h"

#include "i_o-ran-sync.h"

/* SIL bundle: o_ran_all */
#include "y_o-ran-sync.h"
#include "u_o-ran-sync.h"


static char i_mod_sync[]                                        = i_mod_o_ran_sync;
static char i_para_sync_domain_number_param_name[]              = i_para_o_ran_sync_domain_number_param_name;
static char i_para_sync_clock_classes_param_name[]              = i_para_o_ran_sync_clock_classes_param_name;
static char i_para_sync_ptp_reporting_period_param_name[]       = i_para_o_ran_sync_ptp_reporting_period_param_name;
static char i_para_sync_synce_reporting_period_param_name[]     = i_para_o_ran_sync_synce_reporting_period_param_name;
static char i_para_sync_capability_sync_t_tsc_param_name[]      = i_para_o_ran_sync_capability_sync_t_tsc_param_name;
static char i_para_status_supported_reference_types[]           = i_para_o_ran_sync_status_supported_reference_types;
static char i_para_sync_item[]                                  = i_para_o_ran_sync_item;
static char i_para_sync_synce_config_ssm_timeout[]              = i_para_o_ran_sync_synce_config_ssm_timeout;

static char i_para_sync_state_freerun[]                         = i_o_ran_sync_N_state_freerun;
static char i_para_sync_state_locked[]                          = i_o_ran_sync_N_state_locked;
static char i_para_sync_state_holdover[]                        = i_o_ran_sync_N_state_holdover;
static char i_para_sync_lock_state_unlocked[]                   = i_o_ran_sync_N_state_lock_unlocked;
static char i_para_sync_lock_state_locked[]                     = i_o_ran_sync_N_state_lock_locked;
static char i_para_synce_lock_state_unlocked[]                  = i_o_ran_sync_N_state_synce_lock_unlocked;
static char i_para_synce_lock_state_locked[]                    = i_o_ran_sync_N_state_synce_lock_locked;
/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   T_MPSW_MSI_O_RAN_SYNC_EDIT_REQを初期化する
 * @note    
 * @param   [out] edit_data
 * @return  -
 * @date    2019/03/06 KCN)南條
 */
void i_o_ran_sync_edit_table_init(T_MPSW_MSI_O_RAN_SYNC_EDIT_REQ *edit_data)
{
    int count ;
    edit_data->ptp_config_t.domain_number            = D_O_RAN_SYNC_UINT16_INDEFINITE;
    edit_data->ptp_config_t.multicast_mac_address    = E_SYNC_MULTICAST_ADDR_UNCAHNGE;
    edit_data->ptp_config_t.clock_classes            = D_O_RAN_SYNC_UINT16_INDEFINITE;
    edit_data->ptp_config_t.delay_asymmetry          = D_O_RAN_SYNC_INT16_INDEFINITE;
    edit_data->ptp_config_t.ptp_reporting_period     = D_O_RAN_SYNC_UINT16_INDEFINITE;
    edit_data->ptp_config_t.syncE_reporting_period   = D_O_RAN_SYNC_UINT16_INDEFINITE;
    for(count = 0; count < 11; count++){
        edit_data->ptp_config_t.syncE_acceptance_ssm[count]     = D_O_RAN_SYNC_UINT32_INDEFINITE;
    }
    edit_data->ptp_config_t.syncE_ssm_timeout        = D_O_RAN_SYNC_UINT32_INDEFINITE;

    return;
} /* i_o_ran_sync_edit_table_init */

/*!
 * @brief   partial_timing_supportedのvalidateを行う
 * @note    
 * @param   [in] newval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_EXISTS
 *          [異常]ERR_NCX_BAD_ELEMENT
 * @date    2018/11/24 KCN)wang
 */
status_t i_o_ran_sync_sync_ptp_status_partial_timing_supported_validate (
    boolean *newval)
{
    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if(*newval != FALSE) {
        log_warn("\n%s: parameter isn't FALSE", __func__);
        res = ERR_NCX_BAD_ELEMENT;
    }

    return res;
} /* i_o_ran_sync_sync_ptp_status_partial_timing_supported_validate */


/*!
 * @brief   edit時のMSGを送信する
 * @note    MSI_O_RAN_SYNC_EDIT_REQを送信する
 * @param   [in] edit_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/24 KCN)wang
 */
status_t i_o_ran_sync_edit_req_send_msi (
    T_MPSW_MSI_O_RAN_SYNC_EDIT_REQ  *edit_data)
{
    status_t res = NO_ERR;
    uint32_t request_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    T_MPSW_MSI_O_RAN_SYNC_EDIT_REJ read_data;
    int ret = D_MSG_BUFFER_GET_OK;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* MSI_O_RAN_SYNC_EDIT_REQ送信 */
    request_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_SYNC_EDIT_REQ, sizeof(T_MPSW_MSI_O_RAN_SYNC_EDIT_REQ), (void*)edit_data);
    if(request_id == 0) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return res;
    }

    /* MSI_O_RAN_SYNC_EDIT_REQレスポンス待ち */
    ret = f_saa_com_get_response(request_id,
        sizeof(T_MPSW_MSI_O_RAN_SYNC_EDIT_REJ),
        &message_id,
        &read_size,
        &read_data,
        D_MSG_WAIT_TIME );
    if (LOGDEV1) {
        log_dev1("\n domain_number:%u", edit_data->ptp_config_t.domain_number);
        log_dev1("\n multicast_mac_address:%d", edit_data->ptp_config_t.multicast_mac_address);
        log_dev1("\n clock_classes:%u", edit_data->ptp_config_t.clock_classes);
        log_dev1("\n delay_asymmetry:%d", edit_data->ptp_config_t.delay_asymmetry);
        log_dev1("\n ptp_reporting_period:%u", edit_data->ptp_config_t.ptp_reporting_period);
        log_dev1("\n syncE_reporting_period:%u", edit_data->ptp_config_t.syncE_reporting_period);
    }

    /* MSI_O_RAN_SYNC_EDIT_REQレスポンス判定 */
    switch (ret) {
    case D_MSG_BUFFER_GET_OK:
        if (LOGDEV1) {
                log_dev1("\n%s: MSI : got a response.", __func__);
        }
        if(message_id != D_MSG_MSI_O_RAN_SYNC_EDIT_CFM) {
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: cannot add a request.", __func__);
            }
        }
        break;
    case D_MSG_BUFFER_GET_ERR_SIZE:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: too small buffer size.", __func__);
        }
        break;
    case D_MSG_BUFFER_GET_ERR_TIMEOUT:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: time out.", __func__);
        }
        break;
    case D_MSG_BUFFER_GET_ERR_OTHER:
    default:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: other.", __func__);
        }
    }

    return res;
} /* i_o_ran_sync_edit_req_send_msi */


/*!
 * @brief   MSI_O_RAN_SYNC_GET_REQ送信関数
 * @note    MSI_O_RAN_SYNC_GET_REQを送信する
 * @param   [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/24 KCN)wang
 */
status_t i_o_ran_sync_get_send_msi (
    void *get_data)
{
    status_t res = NO_ERR;
    uint32_t request_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    int ret = D_MSG_BUFFER_GET_OK;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* MSI_O_RAN_SYNC_GET_REQ送信 */
    request_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_SYNC_GET_REQ, 0, 0);
    if(request_id == 0) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return res;
    }

    /* MSI_IETF_INTERFACES_GETレスポンス待ち */
    ret = f_saa_com_get_response(request_id,
        sizeof(T_MPSW_MSI_O_RAN_SYNC_GET_CFM),
        &message_id,
        &read_size,
        get_data,
        D_MSG_WAIT_TIME );

    /* MSI_O_RAN_SYNC_GET_REQレスポンス判定 */
    switch (ret) {
    case D_MSG_BUFFER_GET_OK:
        if (LOGDEV1) {
                log_dev1("\n%s: MSI: got a response.", __func__);
        }
        if(message_id != D_MSG_MSI_O_RAN_SYNC_GET_CFM) {
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: wrong message id. (%0x)", __func__, message_id);
            }
        }
        break;
    case D_MSG_BUFFER_GET_ERR_SIZE:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: too small buffer size.", __func__);
        }
        break;
    case D_MSG_BUFFER_GET_ERR_TIMEOUT:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: time out.", __func__);
        }
        break;
    case D_MSG_BUFFER_GET_ERR_OTHER:
    default:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: other.", __func__);
        }
    }

    return res;
} /* i_ietf_interface_send_msi */

/*!
 * @brief   ptp_config_domain_numberのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/29 KCN)wang
 */
status_t i_o_ran_sync_sync_ptp_config_domain_number_validate(
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = ERR_NCX_INVALID_VALUE;
    int rtn_saa;
    char *validate_param_addr;
    int64_t validate_param_max;
    int64_t validate_param_min;
    int64_t target;


    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = (int64_t)VAL_UINT8(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, ptp config domain number, target=%ld", __func__, target);
        }
    }
    else if (curval) {
        target = (int64_t)VAL_UINT8(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, ptp config domain number, target= %ld", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing, ptp config domain number", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* validate用パラメータを取得 */
    validate_param_addr = f_saa_com_get_validate_data(i_mod_sync, i_para_sync_domain_number_param_name);
    if (validate_param_addr == NULL) {
        if(LOGWARN) {
            log_warn("\n%s:%d Operation Failed to get validate data.", __func__, __LINE__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate用パラメータの変換 */
    rtn_saa = f_saa_com_conversion_validate_data_max_min(validate_param_addr, &validate_param_min, &validate_param_max);
    if (rtn_saa == D_SYS_NG) {
        if(LOGWARN) {
            log_warn("\n%s:%d Operation Failed to get validate data.", __func__, __LINE__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate */
    if (validate_param_min <= target && target <=validate_param_max) {
        if (LOGDEV1) {
            log_dev1("\n%s:%d Validation completed.", __func__, __LINE__);
        }
        res = NO_ERR;
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s:%d Validation failed.", __func__, __LINE__);
        }
    }
    
    return res;

} /* i_o_ran_sync_sync_ptp_config_domain_number_validate */

/*!
 * @brief   ptp_config_accepted_clock_classesのValidateを行う
 * @note    
 * @param   [in] k_sync_ptp_config_accepted_clock_classes_clock_classes
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/29 KCN)wang
 * @date    2018/12/15 KCN)wang
 */
status_t i_o_ran_sync_sync_ptp_config_accepted_clock_classes_validate(
    uint8 k_sync_ptp_config_accepted_clock_classes_clock_classes)
{
    status_t res = ERR_NCX_INVALID_VALUE;
    int rtn_saa;
    char *validate_param_addr;
    int64_t validate_param_max;
    int64_t validate_param_min;
    int64_t target;


    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate対象のパラメータを選択 */
    target = (int64_t)k_sync_ptp_config_accepted_clock_classes_clock_classes;

    /* validate用パラメータを取得 */
    validate_param_addr = f_saa_com_get_validate_data(i_mod_sync, i_para_sync_clock_classes_param_name);
    if (validate_param_addr == NULL) {
        if(LOGWARN) {
            log_warn("\n%s:%d Operation Failed to get validate data.", __func__, __LINE__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate用パラメータの変換 */
    rtn_saa = f_saa_com_conversion_validate_data_max_min(validate_param_addr, &validate_param_min, &validate_param_max);
    if (rtn_saa == D_SYS_NG) {
        if(LOGWARN) {
            log_warn("\n%s:%d Operation Failed to get validate data.", __func__, __LINE__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate */
    if (validate_param_min <= target && target <=validate_param_max) {
        if (LOGDEV1) {
            log_dev1("\n%s:%d Validation completed.", __func__, __LINE__);
        }
        res = NO_ERR;
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s:%d Validation failed.", __func__, __LINE__);
        }
    }
    
    return res;

} /* i_o_ran_sync_sync_ptp_config_accepted_clock_classes_validate */

/*!
 * @brief   ptp_status_reporting_periodのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/29 KCN)wang
 */
status_t i_o_ran_sync_sync_ptp_status_reporting_period_validate(
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = ERR_NCX_INVALID_VALUE;
    int rtn_saa;
    char *validate_param_addr;
    int64_t validate_param_max;
    int64_t validate_param_min;
    int64_t target;


    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = (int64_t)VAL_UINT8(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, ptp status reporting period, target=%ld", __func__, target);
        }
    }
    else if (curval) {
        target = (int64_t)VAL_UINT8(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, ptp status reporting period, target=%ld", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing, ptp status reporting period.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* validate用パラメータを取得 */
    validate_param_addr = f_saa_com_get_validate_data(i_mod_sync, i_para_sync_ptp_reporting_period_param_name);
    if (validate_param_addr == NULL) {
        if(LOGWARN) {
            log_warn("\n%s:%d Operation Failed to get validate data.", __func__, __LINE__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    log_dev1("\n%s: validate_param_addr=%s", __func__, validate_param_addr);

    /* validate用パラメータの変換 */
    rtn_saa = f_saa_com_conversion_validate_data_max_min(validate_param_addr, &validate_param_min, &validate_param_max);
    if (rtn_saa == D_SYS_NG) {
        if(LOGWARN) {
            log_warn("\n%s:%d Operation Failed to get validate data.", __func__, __LINE__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate */
    if (validate_param_min <= target && target <=validate_param_max) {
        if (LOGDEV1) {
            log_dev1("\n%s:%d Validation completed.", __func__, __LINE__);
        }
        res = NO_ERR;
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s:%d Validation failed.", __func__, __LINE__);
        }
    }
    
    return res;

} /* i_o_ran_sync_sync_ptp_status_reporting_period_validate */

/*!
 * @brief   synce_status_reporting_periodのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/29 KCN)wang
 */
status_t i_o_ran_sync_sync_synce_status_reporting_period_validate(
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = ERR_NCX_INVALID_VALUE;
    int rtn_saa;
    char *validate_param_addr;
    int64_t validate_param_max;
    int64_t validate_param_min;
    int64_t target;


    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = (int64_t)VAL_UINT8(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, synce status reporting period, target=%ld", __func__, target);
        }
    }
    else if (curval) {
        target = (int64_t)VAL_UINT8(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, synce status reporting period, target=%ld", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing, synce status reporting period.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* validate用パラメータを取得 */
    validate_param_addr = f_saa_com_get_validate_data(i_mod_sync, i_para_sync_synce_reporting_period_param_name);
    if (validate_param_addr == NULL) {
        if(LOGWARN) {
            log_warn("\n%s:%d Operation Failed to get validate data.", __func__, __LINE__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate用パラメータの変換 */
    rtn_saa = f_saa_com_conversion_validate_data_max_min(validate_param_addr, &validate_param_min, &validate_param_max);
    if (rtn_saa == D_SYS_NG) {
        if(LOGWARN) {
            log_warn("\n%s:%d Operation Failed to get validate data.", __func__, __LINE__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate */
    if (validate_param_min <= target && target <=validate_param_max) {
        if (LOGDEV1) {
            log_dev1("\n%s:%d Validation completed.", __func__, __LINE__);
        }
        res = NO_ERR;
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s:%d Validation failed.", __func__, __LINE__);
        }
    }
    
    return res;

} /* i_o_ran_sync_sync_synce_status_reporting_period_validate */

/*!
 * @brief   i_o_ran_sync_sync_synce_config_ssm_timeout_validateのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/6/19 KCN)satou
 */
status_t i_o_ran_sync_sync_synce_config_ssm_timeout_validate(
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = ERR_NCX_INVALID_VALUE;
    int rtn_saa;
    char *validate_param_addr;
    int64_t validate_param_max;
    int64_t validate_param_min;
    int64_t target;


    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate対象のパラメータを選択 */
    if (newval) {
        target = (int64_t)VAL_UINT16(newval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set newval, synce ssm timeout, target=%ld", __func__, target);
        }
    }
    else if (curval) {
        target = (int64_t)VAL_UINT16(curval);
        if (LOGDEV1) {
            log_dev1("\n%s: Set curval, synce ssm timeout, target=%ld", __func__, target);
        }
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing, synce ssm timeout.", __func__);
        }
        return ERR_NCX_DATA_MISSING;
    }

    /* validate用パラメータを取得 */
    validate_param_addr = f_saa_com_get_validate_data(i_mod_sync, i_para_sync_synce_config_ssm_timeout);
    if (validate_param_addr == NULL) {
        if(LOGWARN) {
            log_warn("\n%s:%d Operation Failed to get validate data.", __func__, __LINE__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate用パラメータの変換 */
    rtn_saa = f_saa_com_conversion_validate_data_max_min(validate_param_addr, &validate_param_min, &validate_param_max);
    if (rtn_saa == D_SYS_NG) {
        if(LOGWARN) {
            log_warn("\n%s:%d Operation Failed to get validate data.", __func__, __LINE__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate */
    if (validate_param_min <= target && target <=validate_param_max) {
        if (LOGDEV1) {
            log_dev1("\n%s:%d Validation completed.", __func__, __LINE__);
        }
        res = NO_ERR;
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s:%d Validation failed.", __func__, __LINE__);
        }
    }
    
    return res;

} /* i_o_ran_sync_sync_synce_config_ssm_timeout_validate */
/*!
 * @brief   sync_capability取得を行う
 * @note    
 * @param   [in] sync_capability
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/29 KCN)wang
 */
status_t i_o_ran_sync_sync_sync_capability_get(xmlChar **sync_capability)
{
    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    *sync_capability = (xmlChar *)f_saa_com_get_init_data(i_mod_sync, i_para_sync_capability_sync_t_tsc_param_name);
    if(*sync_capability == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("\n%s: Operation Failed to get type, sync capability. ", __func__);
        }
    }
    
    return res;
} /* i_o_ran_sync_sync_sync_capability_get */

/*!
 * @brief   supported_reference_types_itemのkeyからkey番号の取得を行う
 * @note    
 * @param   [in] k_user_plane_configuration_rx_arrays_name
 * @param   [out] curindex
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/29 KCN)wang
 */
status_t i_o_ran_sync_sync_sync_status_supported_reference_types_get_key_num(
    const xmlChar *k_sync_sync_status_supported_reference_types_item,
    uint32 *curindex)
{
    status_t res = ERR_NCX_DATA_MISSING;
    xmlChar *item;
    char para_name[D_PARAM_NUM_MAX];

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    for(uint32 i = 0; i < SAA_SYNC_MAX_ITEM_NUM; i++) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%d%s", i_para_status_supported_reference_types, i, i_para_sync_item);
        item = (xmlChar *)f_saa_com_get_init_data(i_mod_sync, para_name);
        if (item == NULL) {
            if(LOGWARN) {
                log_warn("\nData Missing, supported reference types item = %s", k_sync_sync_status_supported_reference_types_item);
            }
            return res;
        }
        else if (!xml_strcmp(item, k_sync_sync_status_supported_reference_types_item)) {
            *curindex = i;
            if (LOGDEV1) {
                log_dev1("\nGet supported reference types item index, num = %d", i);
            }
            res = NO_ERR;
            break;
        }
    }

    return res;

}/* i_o_ran_sync_sync_sync_status_supported_reference_types_get_key_num */

/*!
 * @brief   supported_reference_types_itemの先頭のkey取得を行う
 * @note    
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/29 KCN)wang
 */
status_t i_o_ran_sync_sync_sync_status_supported_reference_types_get_first_key(
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%s%d%s", i_para_status_supported_reference_types, 0, i_para_sync_item);

    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_sync, para_name);
    if(*key_name_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("\nOperation Failed to get first key, upconf rx arrays");
        }
    }

    return res;

}/* i_o_ran_sync_sync_sync_status_supported_reference_types_get_first_key */

/*!
 * @brief   supported_reference_types_itemの指定されたkey取得を行う
 * @note    
 * @param   [in] curindex
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/29 KCN)wang
 */
status_t i_o_ran_sync_sync_sync_status_supported_reference_types_get_target_key(
    uint32 curindex,
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%s%d%s", i_para_status_supported_reference_types, curindex, i_para_sync_item);

    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_sync, para_name);
    if(*key_name_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("\nOperation Failed to get target key, supported reference types item index = %d", curindex);
        }
    }

    return res;

}/* i_o_ran_sync_sync_sync_status_supported_reference_types_get_target_key */

/*!
 * @brief   MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_IND受信関数
 * @note    MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_INDを受信する
 * @param   [in] data
 * @param   [in] size
 * @date    2019/02/12 KCN)南條
 */
void i_o_ran_sync_synchronization_state_change_handler(void *data, uint32_t size)
{
    xmlChar *sync_state = 0;
    T_MPSW_MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_IND *ntf_datap;

    if(sizeof(T_MPSW_MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_IND) != size)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: Size of notification-data is different : %d", __func__, size);
        }
        return;
    }
    ntf_datap = (T_MPSW_MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_IND*)data;

    /* state文字列変換 */
    switch(ntf_datap->sync_state)
    {
    case E_SYNC_STATE_FREERUN:
        sync_state = (xmlChar *)i_para_sync_state_freerun;
        break;
    case E_SYNC_STATE_LOCKED:
        sync_state = (xmlChar *)i_para_sync_state_locked;
        break;
    case E_SYNC_STATE_HOLDOVER:
        sync_state = (xmlChar *)i_para_sync_state_holdover;
        break;
    default:
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: wrong state. sync_state=%d.", __func__, ntf_datap->sync_state);
        }
        return;
    }

    /* notification送信 */
    u_o_ran_sync_synchronization_state_change_send((const xmlChar *)sync_state);

    return;
} /* i_o_ran_sync_synchronization_state_change_handler */

/*!
 * @brief   MSI_O_RAN_SYNC_PTPSTATE_CHANGE_NTF_IND受信関数
 * @note    MSI_O_RAN_SYNC_PTPSTATE_CHANGE_NTF_INDを受信する
 * @param   [in] data
 * @param   [in] size
 * @date    2019/02/12 KCN)南條
 */
void i_o_ran_sync_ptp_state_change_send_handler(void *data, uint32_t size)
{
    xmlChar *sync_lock_state = 0;
    T_MPSW_MSI_O_RAN_SYNC_PTPSTATE_CHANGE_NTF_IND *ntf_datap;

    if(sizeof(T_MPSW_MSI_O_RAN_SYNC_PTPSTATE_CHANGE_NTF_IND) != size)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: Size of notification-data is different : %d", __func__, size);
        }
        return;
    }
    ntf_datap = (T_MPSW_MSI_O_RAN_SYNC_PTPSTATE_CHANGE_NTF_IND*)data;

    /* state文字列変換 */
    switch(ntf_datap->state)
    {
    case E_PTP_STATE_UNLOCKED:
        sync_lock_state = (xmlChar *)i_para_sync_lock_state_unlocked;
        break;
    case E_PTP_STATE_LOCKED:
        sync_lock_state = (xmlChar *)i_para_sync_lock_state_locked;
        break;
    default:
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: wrong state. sync_state=%d.", __func__, ntf_datap->state);
        }
        return;
    }

    /* notification送信 */
    u_o_ran_sync_ptp_state_change_send((const xmlChar *)sync_lock_state);

    return;
} /* i_o_ran_sync_ptp_state_change_send_handler */

/*!
 * @brief   MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_IND受信関数
 * @note    MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_INDを受信する
 * @param   [in] data
 * @param   [in] size
 * @date    2019/02/12 KCN)南條
 */
void i_o_ran_sync_synce_state_change_handler(void *data, uint32_t size)
{
    xmlChar *synce_lock_state = 0;
    T_MPSW_MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_IND *ntf_datap;

    if(sizeof(T_MPSW_MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_IND) != size)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: Size of notification-data is different : %d", __func__, size);
        }
        return;
    }
    ntf_datap = (T_MPSW_MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_IND*)data;

    /* lockstate文字列変換 */
    switch(ntf_datap->lockstate)
    {
    case E_SYNCE_LOCK_STATE_UNLOCKED:
        synce_lock_state = (xmlChar *)i_para_synce_lock_state_unlocked;
        break;
    case E_SYNCE_LOCK_STATE_LOCKED:
        synce_lock_state = (xmlChar *)i_para_synce_lock_state_locked;
        break;
    default:
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: wrong state. lockstate=%d.", __func__, ntf_datap->lockstate);
        }
        return;
    }

    /* notification送信 */
    u_o_ran_sync_synce_state_change_send((const xmlChar *)synce_lock_state);

    return;
} /* i_o_ran_sync_synce_state_change_handler */

