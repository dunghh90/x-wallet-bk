
/*
 * @file    i_o-ran-operations.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_OPERATIONS
 * @brief       SAA o-ran-operations
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "xml_util.h"
#include "i_o-ran-operations.h"

bool o_ran_operations_get_msi_send_flag = true;
T_MPSW_MSI_O_RAN_OPERATIONS_GET_CFM   get_cfm_save_data;
/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   supported-header-mechanism keyチェック関数
 * @note    supported-header-mechanismのkeyを"saa_init_data.conf"から取得し、チェックする
 * @param   [in]  target_para_name
 *          [out] para_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/06/19 FJT)吉田
 */
status_t i_o_ran_operations_operational_info_declarations_supported_header_mechanism_keyget( xmlChar **para_temp )
{
    status_t    res = NO_ERR;
    char        para_name[D_PARAM_NUM_MAX];
    char        mod_name[]          = i_mod_o_ran_operations;
    char        para_protocol[]     = i_para_o_ran_operations_protocol;

    if( LOGINFO ) {
        log_info("¥nEnter %s:", __func__);
    }

    /* protocol */
    snprintf( para_name, D_PARAM_NUM_MAX , "%s", para_protocol );
    *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_name);

    /* parameter get error from saa_init_data.conf */
    if( *para_temp == NULL ) {
        res = ERR_NCX_NO_INSTANCE;
        if( LOGWARN ) {
            log_warn("¥n%s: Operation Failed to get param, operations supported_header_mechanism para = %s", __func__, para_name);
        }
    /* key check */
    }
    return res;
}   /* i_o_ran_operations_operational_info_declarations_supported_header_mechanism_get */

/*!
 * @brief   supported-header-mechanism取得関数
 * @note    supported-header-mechanismを"saa_init_data.conf"から取得する
 * @param   [in]  target_para_name
 *          [out] para_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/06/17 FJT)吉田
 */
status_t i_o_ran_operations_operational_info_declarations_supported_header_mechanism_get( const xmlChar   *target_para_name,
                                                                                          xmlChar         **para_temp )
{
    status_t    res = NO_ERR;
    char        para_name[D_PARAM_NUM_MAX];
    char        mod_name[]          = i_mod_o_ran_operations;
    char        para_ecpri_sup[]    = i_para_o_ran_operations_ecpri_supported;
    char        para_protocol_ver[] = i_para_o_ran_operations_protocol_version;

    if( LOGINFO ) {
        log_info("¥nEnter %s:", __func__);
    }

    /* ecpri-concatenation-support */
    if( !xml_strcmp(y_o_ran_operations_N_ecpri_concatenation_support, target_para_name) ) {
        snprintf( para_name , D_PARAM_NUM_MAX , "%s" , para_ecpri_sup );

    /* protocol-version */
    } else if( !xml_strcmp(y_o_ran_operations_N_protocol_version, target_para_name) ) {
        snprintf( para_name , D_PARAM_NUM_MAX , "%s" , para_protocol_ver );

    /* etc */
    } else {
        if( LOGWARN ) {
            log_warn("¥n%s: Data Missing, operations supported_header_mechanism para = %s", __func__, target_para_name);
        }
        res = ERR_NCX_DATA_MISSING;
        return res;
    }

    *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_name);
    /* parameter get error from saa_init_data.conf */
    if( *para_temp == NULL ) {
        res = ERR_NCX_OPERATION_FAILED;
        if( LOGWARN ) {
            log_warn("¥n%s: Operation Failed to get param, operations supported_header_mechanism para = %s", __func__, para_name);
        }
    }
    return res;
}   /* i_o_ran_operations_operational_info_declarations_supported_header_mechanism_get */

/*!
 * @brief   declarations取得関数
 * @note    declarationsを"saa_init_data.conf"から取得する
 * @param   [in]  target_para_name
 *          [out] para_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/06/17 FJT)吉田
 */
status_t i_o_ran_operations_operational_info_declarations_get( const xmlChar   *target_para_name,
                                                               xmlChar         **para_temp )
{
    status_t    res = NO_ERR;
    char        para_name[D_PARAM_NUM_MAX];
    char        mod_name[]      = i_mod_o_ran_operations;
    char        para_mp_ver[]   = i_para_o_ran_operations_mplane_version;
    char        para_cusp_ver[] = i_para_o_ran_operations_cusplane_version;

    if( LOGINFO ) {
        log_info("¥nEnter %s:", __func__);
    }

    /* supported-mplane-version */
    if( !xml_strcmp(y_o_ran_operations_N_supported_mplane_version, target_para_name) ) {
        snprintf( para_name , D_PARAM_NUM_MAX , "%s" , para_mp_ver );

    /* supported-cusplane-version */
    } else if( !xml_strcmp(y_o_ran_operations_N_supported_cusplane_version, target_para_name) ) {
        snprintf( para_name , D_PARAM_NUM_MAX , "%s" , para_cusp_ver );

	/* etc */
    } else {
        if( LOGWARN ) {
            log_warn("¥n%s: Data Missing, operations declarations para = %s", __func__, target_para_name);
        }
        res = ERR_NCX_DATA_MISSING;
        return res;
    }

    *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_name);
    /* parameter get error from saa_init_data.conf */
    if( *para_temp == NULL ) {
        res = ERR_NCX_OPERATION_FAILED;
        if( LOGWARN ) {
            log_warn("¥n%s: Operation Failed to get param, operations declarations para = %s", __func__, para_name);
        }
    }
    return res;
} /* i_o_ran_operations_operational_info_declarations_get */

/*!
 * @brief   O_RAN_OPERATIONS_GET_REQ送信関数
 * @note    O_RAN_OPERATIONS_GET_REQを送信する
 * @param   [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/06/17 FJT)吉田
 */
status_t i_o_ran_operations_operational_info_operational_state_get( void *get_data )
{
    status_t    res = NO_ERR;
    uint32_t    response_id = 0;
    uint32_t    message_id  = 0;
    uint32_t    read_size   = 0;
    int         ret = D_MSG_BUFFER_GET_OK;
    (void)get_data;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* O_RAN_OPERATIONS_GETを既に1度送信済みの場合、内部保持データを返却する */
    if(!o_ran_operations_get_msi_send_flag) {
        if (LOGDEV1) {
            log_dev1("\n%s: Already O_RAN_OPERATIONS_GET send. ", __func__);
        }
        memcpy(get_data, &get_cfm_save_data, sizeof(T_MPSW_MSI_O_RAN_OPERATIONS_GET_CFM));
        return res;
    }

    /* O_RAN_OPERATIONS_GET_REQ送信 */
    response_id = f_saa_com_add_request( D_MSG_MSI_O_RAN_OPERATIONS_GET_REQ, 0, NULL );
    if(response_id == 0) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed.: cannot add a request.", __func__);
        }
        return res;
    }

    /* O_RAN_OPERATIONS_GET_REQレスポンス待ち */
    ret = f_saa_com_get_response( response_id,
                                  max(sizeof(T_MPSW_MSI_O_RAN_OPERATIONS_GET_CFM), sizeof(T_MPSW_MSI_O_RAN_OPERATIONS_GET_REJ)),
                                  &message_id,
                                  &read_size,
                                  get_data,
                                  D_MSG_WAIT_TIME );

    /* O_RAN_OPERATIONS_GET_REQレスポンス判定 */
    switch (ret) {
        case D_MSG_BUFFER_GET_OK:
            if(message_id != D_MSG_MSI_O_RAN_OPERATIONS_GET_CFM) {
                /* error */
                res = ERR_NCX_OPERATION_FAILED;
                /* error log */
                if(LOGWARN) {
                    log_warn("\n%s: MSI failed.: wrong message id. (%0x)", __func__, message_id);
                }
            } else {
                /* O_RAN_OPERATIONS_GETを送信済の状態に変更 */
                o_ran_operations_get_msi_send_flag = false;
                memcpy( &get_cfm_save_data, get_data, sizeof(T_MPSW_MSI_O_RAN_OPERATIONS_GET_CFM) );
            }
            break;
        case D_MSG_BUFFER_GET_ERR_SIZE:
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed.: buffer size too small. (%0x)", __func__, read_size);
            }
            break;
        case D_MSG_BUFFER_GET_ERR_TIMEOUT:
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed.: time out.", __func__);
            }
            break;
        case D_MSG_BUFFER_GET_ERR_OTHER:
        default:
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed.: other error.", __func__);
            }
            break;
    }

    return res;
} /* i_o_ran_operations_operational_info_operational_state_get */

/*!
 * @brief   timezone-utc-offset設定関数
 * @note    timezone-utc-offsetを設定する
 * @param   [in]  timezone_utc_offset
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/06/17 FJT)吉田
 */
status_t i_o_ran_operations_operational_info_clock_timezone_utc_offset_edit( int16 timezone_utc_offset )
{
    status_t    res = NO_ERR;
    int         ret = D_MPSW_OK;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    ret = f_saa_com_set_utc_offset( timezone_utc_offset ) ;
    if( ret != D_MPSW_OK ) {
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: UTC offset set error.", __func__);
        }
    }

    return res;
} /* i_o_ran_operations_operational_info_clock_timezone_utc_offset_edit */

/*!
 * @brief   re-call-home-no-ssh-timerパラメータチェック関数
 * @note    re-call-home-no-ssh-timerの範囲チェックする
 * @param   [in]  re_call_home_no_ssh_timer
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/06/17 FJT)吉田
 */
status_t i_o_ran_operations_operational_info_re_call_home_no_ssh_timer_validate( uint16 re_call_home_no_ssh_timer)
{
    status_t    res = NO_ERR;
    int         ret = D_MPSW_OK;

    char        mod_name[]  = i_mod_o_ran_operations;
    char        para_name[] = i_para_o_ran_operations_re_callhome_no_ssh_timer;

    char        *validate_str;
    int64_t     validate_min;
    int64_t     validate_max;

    /* get validate string from "saa_validate_data.conf" */
    validate_str = f_saa_com_get_validate_data(mod_name, para_name);
    if( validate_str == NULL ) {
        /* error */
        res = ERR_NCX_DATA_MISSING;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s:%d validate data get NG", __func__, __LINE__);
        }
        return res;
    }
    /* convert validate string to max_data and min_data */
    ret = f_saa_com_conversion_validate_data_max_min( validate_str, &validate_min, &validate_max );
    if( ret != D_MPSW_OK ) {
        /* error */
        res = ERR_NCX_DATA_MISSING;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s:%d validate data convert NG, string=%s", __func__, __LINE__, validate_str);
        }
        return res;
    }

    if (LOGDEV1) {
        log_dev1("\n%s: re-call-home-no-ssh-timer=%d", __func__, re_call_home_no_ssh_timer);
        log_dev1("\n    validate_min=%d", (uint16)validate_min);
        log_dev1("\n    validate_max=%d", (uint16)validate_max);
    }
    /* validate re-call-home-no-ssh-timer */
    if( (re_call_home_no_ssh_timer < (uint16)validate_min) || (re_call_home_no_ssh_timer > (uint16)validate_max) ) {
        /* validate NG */
        res = ERR_NCX_BAD_ELEMENT;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: re-call-home-no-ssh-timer is range error.", __func__);
            log_dev1("\n    re-call-home-no-ssh-timer=%d", re_call_home_no_ssh_timer);
            log_dev1("\n    validate_min=%d", (uint16)validate_min);
            log_dev1("\n    validate_max=%d", (uint16)validate_max);
        }
    }
    return res;
} /* i_o_ran_operations_operational_info_re_call_home_no_ssh_timer_validate */

/*!
 * @brief   re-call-home-no-ssh-timer設定関数
 * @note    re-call-home-no-ssh-timerを設定する
 * @param   [in]  re_call_home_no_ssh_timer
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/06/17 FJT)吉田
 */
status_t i_o_ran_operations_operational_info_re_call_home_no_ssh_timer_edit( uint16 re_call_home_no_ssh_timer)
{
    status_t                                res = NO_ERR;
    uint32_t                                response_id = 0;
    uint32_t                                message_id  = 0;
    uint32_t                                read_size   = 0;
    T_MPSW_MSI_O_RAN_OPERATIONS_EDIT_REQ    edit_data;
    T_MPSW_MSI_O_RAN_OPERATIONS_EDIT_REJ    read_data;
    int                                     ret = D_MSG_BUFFER_GET_OK;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* editデータ設定 */
    edit_data.callhome_retry_timer = re_call_home_no_ssh_timer;

    /* MSI_O_RAN_OPERATIONS_EDIT送信 */
    response_id = f_saa_com_add_request( D_MSG_MSI_O_RAN_OPERATIONS_EDIT_REQ, 
                                         sizeof(T_MPSW_MSI_O_RAN_OPERATIONS_EDIT_REQ), 
                                         (void*)&edit_data);
    if(response_id == 0) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return res;
    }

    /* MSI_O_RAN_OPERATIONS_EDITレスポンス待ち */
    ret = f_saa_com_get_response( response_id,
                                  sizeof(T_MPSW_MSI_O_RAN_OPERATIONS_EDIT_REJ),
                                  &message_id,
                                  &read_size,
                                  &read_data,
                                  D_MSG_WAIT_TIME );

    /* MSI_O_RAN_OPERATIONS_EDITレスポンス判定 */
    switch (ret) {
        case D_MSG_BUFFER_GET_OK:
            if(message_id != D_MSG_MSI_O_RAN_OPERATIONS_EDIT_CFM) {
                /* error */
                res = ERR_NCX_OPERATION_FAILED;
                /* error log */
                if(LOGWARN) {
                    log_warn("\n%s: MSI failed: wrong message id. (%0x).", __func__, message_id);
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
            break;
    }

    return res;
} /* i_o_ran_operations_operational_info_re_call_home_no_ssh_timer_edit */

/*!
 * @brief   O_RAN_OPERATIONS_RESET_IND送信関数
 * @note    O_RAN_OPERATIONS_RESET_INDを送信する
 * @param   [in] reset_cause
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/06/17 FJT)吉田
 */
status_t i_o_ran_operations_reset_invoke( E_RESTART_CAUSE reset_cause )
{
    status_t                                res = NO_ERR;
    T_MPSW_MSI_O_RAN_OPERATIONS_RESET_IND   req_data;
    uint32_t                                response_id = 0;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    req_data.restart_cause = reset_cause;
    /* O_RAN_OPERATIONS_RESET_IND送信 */
    response_id = f_saa_com_add_request( D_MSG_MSI_O_RAN_OPERATIONS_RESET_IND, 
                                         sizeof(T_MPSW_MSI_O_RAN_OPERATIONS_RESET_IND), 
                                         (void*)&req_data);
    if(response_id == 0) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed.: cannot add a request.", __func__);
        }
    }

    return res;
} /* i_o_ran_operations_reset_invoke */

/*!
 * @brief   re-call-home-no-ssh-timer初期値取得関数
 * @note    re-call-home-no-ssh-timerの初期値を取得する
 * @param   -
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2019/06/17 FJT)吉田
 */
void i_o_ran_operations_init2 (void)
{
    if (LOGINFO) {
        log_info("\nEnter %s", __func__);
    }
    return;
} /* i_o_ran_operations_init2 */
