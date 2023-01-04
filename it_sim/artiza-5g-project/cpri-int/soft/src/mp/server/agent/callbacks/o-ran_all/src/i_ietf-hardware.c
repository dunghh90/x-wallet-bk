
/*
 * @file    i_ietf-hardware.c
 */

/*!
 * @addtogroup  SAA_MOD_IETF_HARDWARE
 * @brief       SAA ietf-hardware
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "f_saa_ntf_lib.h"
#include "f_saa_msg_buffer.h"

#include "u_ietf-hardware.h"
#include "i_ietf-hardware.h"


ietf_hardware_t ietf_hardware_state;
pthread_mutex_t mutex_for_hardware_name = PTHREAD_MUTEX_INITIALIZER;

static char ietf_hardware_admin_state_unknown[]      = i_ietf_hardware_N_admin_state_unknown;
static char ietf_hardware_admin_state_locked[]      = i_ietf_hardware_N_admin_state_locked;
static char ietf_hardware_admin_state_shutdown[]    = i_ietf_hardware_N_admin_state_shutdown;
static char ietf_hardware_admin_state_unlocked[]    = i_ietf_hardware_N_admin_state_unlocked;

static char o_ran_hardware_availability_state_unknown[]       = i_o_ran_hardware_N_availability_state_unknown;
static char o_ran_hardware_availability_state_normal[]       = i_o_ran_hardware_N_availability_state_normal;
static char o_ran_hardware_availability_state_degraded[]     = i_o_ran_hardware_N_availability_state_degraded;
static char o_ran_hardware_availability_state_faulty[]       = i_o_ran_hardware_N_availability_state_faulty;


/* notificationハンドラ */
void i_ietf_hardware_hardware_state_change_handler(void *data, uint32_t size);
void i_ietf_hardware_hardware_state_oper_enabled_handler(void *data, uint32_t size);
void i_ietf_hardware_hardware_state_oper_disabled_handler(void *data, uint32_t size);

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   MSI_IETF_HARDWARE_GET送信関数
 * @note    MSI_IETF_HARDWARE_GETを送信する
 * @param   [in] k_hardware_component_name
 * @param   [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_NOT_SUPPORTED
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/21 KCN)南條
 */
status_t i_ietf_hardware_send_msi (void  *get_data)
{
    status_t res = NO_ERR;
    uint32_t response_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    int ret = D_MSG_BUFFER_GET_OK;
    (void)get_data;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* MSI_IETF_HARDWARE_GET送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_IETF_HARDWARE_GET_REQ, 0, NULL);
    if(response_id == 0) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return res;
    }

    /* MSI_IETF_HARDWARE_GETレスポンス待ち */
    ret = f_saa_com_get_response(response_id,
        max(sizeof(T_MPSW_MSI_IETF_HARDWARE_GET_CFM), sizeof(T_MPSW_MSI_IETF_HARDWARE_GET_REJ)),
        &message_id,
        &read_size,
        get_data,
        D_MSG_WAIT_TIME );

    /* MSI_IETF_HARDWARE_GETレスポンス判定 */
    switch (ret) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id != D_MSG_MSI_IETF_HARDWARE_GET_CFM) {
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
    }

    return res;
} /* i_ietf_interface_send_msi */

/*!
 * @brief   MSI_IETF_HARDWARE_STATE_GET送信関数
 * @note    MSI_IETF_HARDWARE_STATE_GETを送信する
 * @param   [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/21 KCN)南條
 */
status_t i_ietf_hardware_state_send_msi (void *get_data)
{
    status_t res = NO_ERR;
    uint32_t response_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    int ret = D_MSG_BUFFER_GET_OK;
    (void)get_data;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* MSI_IETF_HARDWARE_STATE_GET送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_IETF_HARDWARE_STATE_GET_REQ, 0, NULL);
    if(response_id == 0) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return res;
    }

    /* MSI_IETF_HARDWARE_STATE_GETレスポンス待ち */
    ret = f_saa_com_get_response(response_id,
        max(sizeof(T_MPSW_MSI_IETF_HARDWARE_STATE_GET_CFM), sizeof(T_MPSW_MSI_IETF_HARDWARE_STATE_GET_REJ)),
        &message_id,
        &read_size,
        get_data,
        D_MSG_WAIT_TIME );

    /* MSI_IETF_HARDWARE_STATE_GETレスポンス判定 */
    switch (ret) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id != D_MSG_MSI_IETF_HARDWARE_STATE_GET_CFM) {
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
    }

    return res;
} /* i_ietf_hardware_state_send_msi */

/*!
 * @brief   MSI_IETF_HARDWARE_STATE_EDIT送信関数
 * @note    MSI_IETF_HARDWARE_STATE_EDITを送信する
 * @param   [in] newval_val
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/21 KCN)南條
 */
status_t i_ietf_hardware_admin_state_edit(const xmlChar *newval_val)
{
    status_t res = NO_ERR;
    uint32_t response_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    T_MPSW_MSI_IETF_HARDWARE_STATE_EDIT_REQ edit_data;
    T_MPSW_MSI_IETF_HARDWARE_STATE_EDIT_REJ read_data;
    int ret = D_MSG_BUFFER_GET_OK;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* editデータ設定 */
    if(!xml_strcmp(newval_val, (const xmlChar *)i_ietf_hardware_N_admin_state_unknown))
    {
        edit_data.admin_state  = E_MPSW_ADMIN_STATE_UNKNOWN;
    }
    else if(!xml_strcmp(newval_val, (const xmlChar *)i_ietf_hardware_N_admin_state_locked))
    {
        edit_data.admin_state  = E_MPSW_ADMIN_STATE_LOCKED;
    }
    else if(!xml_strcmp(newval_val, (const xmlChar *)i_ietf_hardware_N_admin_state_shutdown))
    {
        edit_data.admin_state  = E_MPSW_ADMIN_STATE_SHUTTINGDOWN;
    }
    else if(!xml_strcmp(newval_val, (const xmlChar *)i_ietf_hardware_N_admin_state_unlocked))
    {
        edit_data.admin_state  = E_MPSW_ADMIN_STATE_UNLOCKED;
    }
    else
    {
        /* error */
        res = ERR_NCX_DATA_MISSING;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s wrong state. admin_state=%s", __func__, newval_val);
        }
        return res;
    }

    /* MSI_IETF_HARDWARE_STATE_EDIT送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_IETF_HARDWARE_STATE_EDIT_REQ, sizeof(T_MPSW_MSI_IETF_HARDWARE_STATE_EDIT_REQ), (void*)&edit_data);
    if(response_id == 0) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return res;
    }

    /* MSI_IETF_HARDWARE_STATE_EDITレスポンス待ち */
    ret = f_saa_com_get_response(response_id,
        sizeof(T_MPSW_MSI_IETF_HARDWARE_STATE_EDIT_REJ),
        &message_id,
        &read_size,
        &read_data,
        D_MSG_WAIT_TIME );

    /* MSI_IETF_HARDWARE_STATE_EDITレスポンス判定 */
    switch (ret) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id != D_MSG_MSI_IETF_HARDWARE_STATE_EDIT_CFM) {
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: wrong message id. (%0x).", __func__, message_id);
            }
        }
        else
        {
            /* lock */
            pthread_mutex_lock(&mutex_for_hardware_name);

            /* admin_stateの設定値更新 */
            ietf_hardware_state.present_admin_state = edit_data.admin_state;

            /* unlock */
            pthread_mutex_unlock(&mutex_for_hardware_name);
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
} /* i_ietf_hardware_admin_state_edit */

/*!
 * @brief   description取得関数
 * @note    descriptionを取得する
 * @param   [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/21 KCN)南條
 */
status_t i_ietf_hardware_hardware_component_description_get (const xmlChar *k_hardware_component_name, xmlChar **v_description)
{
    status_t res = NO_ERR;
    char mod_name[]  = i_mod_ietf_hardware;
    char para_name0[] = i_para_ietf_hardware_description0;
    char para_name1[] = i_para_ietf_hardware_description1;
    char para_name2[] = i_para_ietf_hardware_description2;
    (void)v_description;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if( !xml_strcmp(k_hardware_component_name, i_ietf_hardware_N_name_sub6_37) ||
        !xml_strcmp(k_hardware_component_name, i_ietf_hardware_N_name_sub6_45) ||
        !xml_strcmp(k_hardware_component_name, i_ietf_hardware_N_name_mmw_28) )
    {
        /* 5GDU_sub6_37G_LLS、5GDU_sub6_45G_LLS、5GDU_mmw_28G_LLS */
        *v_description = (xmlChar *)f_saa_com_get_init_data(mod_name, para_name0);

    } else if( !xml_strcmp(k_hardware_component_name, i_ietf_hardware_N_name_fhport0)) {
        /* fheth0 */
        *v_description = (xmlChar *)f_saa_com_get_init_data(mod_name, para_name1);

    } else if( !xml_strcmp(k_hardware_component_name, i_ietf_hardware_N_name_fhport1)) {
        /* fheth1 */
        *v_description = (xmlChar *)f_saa_com_get_init_data(mod_name, para_name2);

    } else {
        /* error */
        res = ERR_NCX_DATA_MISSING;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: description is Not Data : %s", __func__, k_hardware_component_name);
        }
        return res;
    }

    return res;
} /* i_ietf_hardware_hardware_component_description_get */

/*!
 * @brief   physical-index取得関数
 * @note    physical-indexを取得する
 * @param   [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/12/20 KCN)若水
 */
status_t i_ietf_hardware_hardware_component_physical_index_get (const xmlChar *k_hardware_component_name, xmlChar **v_physical_index)
{
    status_t res = NO_ERR;
    char mod_name[]  = i_mod_ietf_hardware;
    char para_name0[] = i_para_ietf_hardware_physical_index0;
    char para_name1[] = i_para_ietf_hardware_physical_index1;
    char para_name2[] = i_para_ietf_hardware_physical_index2;
    (void)v_physical_index;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if( !xml_strcmp(k_hardware_component_name, i_ietf_hardware_N_name_sub6_37) ||
        !xml_strcmp(k_hardware_component_name, i_ietf_hardware_N_name_sub6_45) ||
        !xml_strcmp(k_hardware_component_name, i_ietf_hardware_N_name_mmw_28) )
    {
        /* 5GDU_sub6_37G_LLS、5GDU_sub6_45G_LLS、5GDU_mmw_28G_LLS */
        *v_physical_index = (xmlChar *)f_saa_com_get_init_data(mod_name, para_name0);

    } else if( !xml_strcmp(k_hardware_component_name, i_ietf_hardware_N_name_fhport0)) {
        /* fheth0 */
        *v_physical_index = (xmlChar *)f_saa_com_get_init_data(mod_name, para_name1);

    } else if( !xml_strcmp(k_hardware_component_name, i_ietf_hardware_N_name_fhport1)) {
        /* fheth1 */
        *v_physical_index = (xmlChar *)f_saa_com_get_init_data(mod_name, para_name2);

    } else {
        /* error */
        res = ERR_NCX_DATA_MISSING;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: physical-index is Not Data : %s", __func__, k_hardware_component_name);
        }
        return res;
    }

    return res;
} /* i_ietf_hardware_hardware_component_physical_index_get */

/*!
 * @brief   MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND受信関数
 * @note    MSI_IETF_HARDWARE_STATE_CHANGE_NTF_INDを受信する
 * @param   [in] data
 * @param   [in] size
 * @date    2019/02/12 KCN)南條
 */
void i_ietf_hardware_hardware_state_change_handler(void *data, uint32_t size)
{
    T_MPSW_MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND *ntf_datap;
//    char *admin_state = 0;

    if(sizeof(T_MPSW_MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND) != size)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: Size of notification-data is different : %d", __func__, size);
        }
        return;
    }
    ntf_datap = (T_MPSW_MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND*)data;

//    /* admin_state文字列変換 */
//    switch(ntf_datap->admin_state)
//    {
//    case E_MPSW_ADMIN_STATE_UNKNOWN:
//        admin_state = ietf_hardware_admin_state_unknown;
//        break;
//    case E_MPSW_ADMIN_STATE_LOCKED:
//        admin_state = ietf_hardware_admin_state_locked;
//        break;
//    case E_MPSW_ADMIN_STATE_SHUTTINGDOWN:
//        admin_state = ietf_hardware_admin_state_shutdown;
//        break;
//    case E_MPSW_ADMIN_STATE_UNLOCKED:
//        admin_state = ietf_hardware_admin_state_unlocked;
//        break;
//    default:
//        /* error log */
//        if(LOGWARN) {
//            log_warn("\n%s: wrong state. admin_state=%d.", __func__, ntf_datap->admin_state);
//        }
//        return;
//    }

    /* lock */
    pthread_mutex_lock(&mutex_for_hardware_name);

    /* admin-stateに更新がある場合DBを更新する */
    if(ntf_datap->admin_state != ietf_hardware_state.present_admin_state)
    {
        /* admin_stateの設定値更新 */
        ietf_hardware_state.present_admin_state = ntf_datap->admin_state;

        /* DB更新 */
//		  このタイミングで即db更新されず、CUからの状態取得でまだ反映されてないケースがあるため、PF側でDB更新するように変更する
//        i_ietf_hardware_send_edit_config(ietf_hardware_state.hardware_name, admin_state);
    }

    /* unlock */
    pthread_mutex_unlock(&mutex_for_hardware_name);

    /* notification送信 */
    u_ietf_hardware_hardware_state_change_send();

    return;
} /* i_ietf_hardware_hardware_state_change_handle */

/*!
 * @brief   MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND受信関数
 * @note    MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_INDを受信する
 * @param   [in] data
 * @param   [in] size
 * @date    2019/02/12 KCN)南條
 */
void i_ietf_hardware_hardware_state_oper_enabled_handler(void *data, uint32_t size)
{
    xmlChar *admin_state = 0;
    xmlChar *alarm_state = 0;
    xmlChar *availability_state = 0;
    T_MPSW_MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND ntf_data;

    if(sizeof(T_MPSW_MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND) != size)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: Size of notification-data is different : %d", __func__, size);
        }
        return;
    }
    memcpy(&ntf_data, data, size);

    /* admin_state文字列変換 */
    switch(ntf_data.admin_state)
    {
    case E_MPSW_ADMIN_STATE_UNKNOWN:
        admin_state = (xmlChar *)ietf_hardware_admin_state_unknown;
        break;
    case E_MPSW_ADMIN_STATE_LOCKED:
        admin_state = (xmlChar *)ietf_hardware_admin_state_locked;
        break;
    case E_MPSW_ADMIN_STATE_SHUTTINGDOWN:
        admin_state = (xmlChar *)ietf_hardware_admin_state_shutdown;
        break;
    case E_MPSW_ADMIN_STATE_UNLOCKED:
        admin_state = (xmlChar *)ietf_hardware_admin_state_unlocked;
        break;
    default:
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: wrong state. admin_state=%d.", __func__, ntf_data.admin_state);
        }
        return;
    }

    /* availability_state文字列変換 */
    switch(ntf_data.availability_state)
    {
    case E_AVAILABILITY_TYPE_UNKNOWN:
        availability_state = (xmlChar *)o_ran_hardware_availability_state_unknown;
        break;
    case E_AVAILABILITY_TYPE_NORMAL:
        availability_state = (xmlChar *)o_ran_hardware_availability_state_normal;
        break;
    case E_AVAILABILITY_TYPE_DEGRADED:
        availability_state = (xmlChar *)o_ran_hardware_availability_state_degraded;
        break;
    case E_AVAILABILITY_TYPE_FAULTY:
        availability_state = (xmlChar *)o_ran_hardware_availability_state_faulty;
        break;
    default:
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: wrong state. availability_state=%d.", __func__, ntf_data.availability_state);
        }
        return;
    }

    /* lock */
    pthread_mutex_lock(&mutex_for_hardware_name);

    /* notification送信 */
    u_ietf_hardware_hardware_state_oper_enabled_send(
        (const xmlChar *)ietf_hardware_state.hardware_name,
        (const xmlChar *)admin_state,
        (const xmlChar *)alarm_state,
        (const xmlChar *)availability_state);

    /* unlock */
    pthread_mutex_unlock(&mutex_for_hardware_name);

    return;
} /* i_ietf_hardware_hardware_state_oper_enabled_handle */

/*!
 * @brief   MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND受信関数
 * @note    MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_INDを受信する
 * @param   [in] data
 * @param   [in] size
 * @date    2019/02/12 KCN)南條
 */
void i_ietf_hardware_hardware_state_oper_disabled_handler(void *data, uint32_t size)
{
    xmlChar *admin_state = 0;
    xmlChar *alarm_state = 0;
    xmlChar *availability_state = 0;
    T_MPSW_MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND ntf_data;

    if(sizeof(T_MPSW_MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND) != size)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: Size of notification-data is different : %d", __func__, size);
        }
        return;
    }
    memcpy(&ntf_data, data, size);

    /* admin_state文字列変換 */
    switch(ntf_data.admin_state)
    {
    case E_MPSW_ADMIN_STATE_UNKNOWN:
        admin_state = (xmlChar *)ietf_hardware_admin_state_unknown;
        break;
    case E_MPSW_ADMIN_STATE_LOCKED:
        admin_state = (xmlChar *)ietf_hardware_admin_state_locked;
        break;
    case E_MPSW_ADMIN_STATE_SHUTTINGDOWN:
        admin_state = (xmlChar *)ietf_hardware_admin_state_shutdown;
        break;
    case E_MPSW_ADMIN_STATE_UNLOCKED:
        admin_state = (xmlChar *)ietf_hardware_admin_state_unlocked;
        break;
    default:
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: wrong state. admin_state=%d.", __func__, ntf_data.admin_state);
        }
        return;
    }

    /* availability_state文字列変換 */
    switch(ntf_data.availability_state)
    {
    case E_AVAILABILITY_TYPE_UNKNOWN:
        availability_state = (xmlChar *)o_ran_hardware_availability_state_unknown;
        break;
    case E_AVAILABILITY_TYPE_NORMAL:
        availability_state = (xmlChar *)o_ran_hardware_availability_state_normal;
        break;
    case E_AVAILABILITY_TYPE_DEGRADED:
        availability_state = (xmlChar *)o_ran_hardware_availability_state_degraded;
        break;
    case E_AVAILABILITY_TYPE_FAULTY:
        availability_state = (xmlChar *)o_ran_hardware_availability_state_faulty;
        break;
    default:
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: wrong state. availability_state=%d.", __func__, ntf_data.availability_state);
        }
        return;
    }

    /* lock */
    pthread_mutex_lock(&mutex_for_hardware_name);

    /* notification送信 */
    u_ietf_hardware_hardware_state_oper_disabled_send(
        (const xmlChar *)ietf_hardware_state.hardware_name,
        (const xmlChar *)admin_state,
        (const xmlChar *)alarm_state,
        (const xmlChar *)availability_state);

    /* unlock */
    pthread_mutex_unlock(&mutex_for_hardware_name);

    return;
} /* i_ietf_hardware_hardware_state_oper_disabled_handle */


/*!
 * @brief   component/nameを渡す
 * @note    
 * @param   [out] xmlChar *data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2019/03/01 KCN)若水
 */
void i_ietf_hardware_name_give (xmlChar *data)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* lock */
    pthread_mutex_lock(&mutex_for_hardware_name);

    /* dataに値を詰める */
    xml_strcpy(data, ietf_hardware_state.hardware_name);

    /* unlock */
    pthread_mutex_unlock(&mutex_for_hardware_name);

    return;
} /* i_ietf_hardware_name_give */

/*!
 * @brief   DB更新関数
 * @note    DBを更新する
 * @param   admin_state
 * @return  -
 * @date    2019/04/11 KCN)南條
 */
void i_ietf_hardware_send_edit_config (xmlChar * hardware_name, char * admin_state)
{
    if (LOGINFO) {
        log_info("\nEnter %s : hardware_name=%s, admin-state=%s", __func__, hardware_name, admin_state);
    }

    char value_buff[D_IETF_HARDWARE_DB_SEND_MAX_LEN] = "";

    /* send-data作成 */
    snprintf(value_buff, D_IETF_HARDWARE_DB_SEND_MAX_LEN,
        "/opt/agent/bin/DU_dbapi "
        "subsys_ietfhardware "
        "'<config>"
            "<hardware xmlns=\"urn:ietf:params:xml:ns:yang:ietf-hardware\">"
                "<component>"
                    "<name>%s</name>"
                        "<state>"
                            "<admin-state>%s</admin-state>"
                        "</state>"
                "</component>"
            "</hardware>"
        "</config>'"
        " --log-level=off 1>/dev/null 2>/dev/null &",
        hardware_name,
        admin_state);

    if (LOGINFO) {
        log_info("\n value_buff : %s", value_buff);
    }
    system(value_buff);

    return;
} /* i_ietf_hardware_send_edit_config */


/*!
 * @brief   admin-state初期値取得関数
 * @note    admin-stateの初期値を取得する
 * @param   -
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2019/03/01 KCN)南條
 */
void i_ietf_hardware_init2 (void)
{
    if (LOGINFO) {
        log_info("\nEnter %s", __func__);
    }

    char mod_name[]  = i_mod_ietf_hardware;
    char param_name[D_PARAM_NUM_MAX] = "";
    char *init_data = 0;
    uint admin_state = E_MPSW_ADMIN_STATE_UNKNOWN;

    /* initデータ取得 */
    /* admin-state */
    snprintf(param_name, D_PARAM_NUM_MAX, "%s.%s.%s.%s.%s", y_ietf_hardware_N_hardware,
        y_ietf_hardware_N_component,
        y_ietf_hardware_N_name,
        y_ietf_hardware_N_state,
        y_ietf_hardware_N_admin_state);

    init_data = f_saa_com_get_init_data(mod_name, param_name);
    /* f_saa_com_get_init_data NGチェック */
    if(init_data == NULL)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: init_data is NULL", __func__);
        }
    }
    else
    {
        /* admin_state保持 */
        admin_state = (uint)atoi((const char *)init_data);
    }

    /* lock */
    pthread_mutex_lock(&mutex_for_hardware_name);

    /* admin_state保持 */
    ietf_hardware_state.present_admin_state = admin_state;

    /* unlock */
    pthread_mutex_unlock(&mutex_for_hardware_name);

    /* notificationハンドラ登録 */
    f_saa_com_reg_notification_handler(D_MSG_MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND, i_ietf_hardware_hardware_state_change_handler);
    f_saa_com_reg_notification_handler(D_MSG_MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND, i_ietf_hardware_hardware_state_oper_enabled_handler);
    f_saa_com_reg_notification_handler(D_MSG_MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND, i_ietf_hardware_hardware_state_oper_disabled_handler);

    (void)f_saa_com_add_request(D_MSG_MSI_IETF_HARDWARE_STATE_CHANGE_START_IND, 0, NULL);
    if (LOGINFO) {
        log_info("\n    Send MSI_IETF_HARDWARE_STATE_CHANGE_START_IND.");
    }

    return ;
} /* i_ietf_hardware_init2 */


/*!
 * @brief   hardware初期値取得関数
 * @note    hardwareの初期値を取得する
 * @param   -
 * @return  N/A
 * @date    2019/10/27 taniguchi
 */
void i_ietf_hardware_init (void)
{
    if (LOGINFO) {
        log_info("\nEnter %s", __func__);
    }

    char mod_name[]  = i_mod_ietf_hardware;
    char param_name[D_PARAM_NUM_MAX] = "";
    char *init_data = 0;

    /* initデータ取得 */
    /* admin-state */
    snprintf(param_name, D_PARAM_NUM_MAX, "%s.%s.%s", y_ietf_hardware_N_hardware,y_ietf_hardware_N_component,y_ietf_hardware_N_name);
    init_data = f_saa_com_get_init_data(mod_name, param_name);
    /* f_saa_com_get_init_data NGチェック */
    if(init_data == NULL)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: init_data is NULL", __func__);
        }
    }
    else
    {
      /* lock */
      pthread_mutex_lock(&mutex_for_hardware_name);
      /* type:O-RAN-RADIOのnameを保持 */
      xml_strcpy(ietf_hardware_state.hardware_name, (xmlChar*)init_data);
      if (LOGDEV1) {
          log_dev1("\n%s Entry Name %s ", __func__ , ietf_hardware_state.hardware_name);
      }
      ietf_hardware_state.present_admin_state = E_MPSW_ADMIN_STATE_UNKNOWN;
      /* unlock */
      pthread_mutex_unlock(&mutex_for_hardware_name);
    }
    return ;
} /* i_ietf_hardware_init */

